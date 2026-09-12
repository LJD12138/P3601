/*******************************************************************************************************************************
 * Project : APP
 * Module  : G:\1-Baiku_Projects\24-P36\1.software\P3601\APP\Hardware\Key
 * File    : key_iface.c
 * Date    : 2026-09-10
 * Author  : LJD(291483914@qq.com)
 * Desc    : 按键硬件驱动底层接口实现(含GPIO查表配置、电平极性归一化读取及低功耗控制)
 * -------------------------------------------------------
 * todo    :
 * 1. none
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
 *******************************************************************************************************************************/

//****************************************************Includes******************************************************************//
#include "Key/key_iface.h"
#include "Adc/adc_iface.h"

#if(boardKEY_EN)

//****************************************************Types*******************************************************************//
typedef struct
{
	rcu_periph_enum rcu;              /* 外设时钟 */
	uint32_t        gpio;             /* GPIO 端口 (如 GPIOA/GPIOB/GPIOC) */
	uint32_t        pin;              /* GPIO 引脚 (如 GPIO_PIN_9) */
	uint32_t        mode;             /* GPIO 模式 (GPIO_MODE_IPU 或 GPIO_MODE_IN_FLOATING) */
	bool            bPressHigh;       /* 按下有效电平: true=高电平按下(浮空Power), false=低电平按下(上拉) */
	const char     *pName;            /* 按键调试名称 */
} KeyCfgGpio_T;

//****************************************************Parameter Initialization************************************************//
/* 按键引脚硬件配置表: 顺序需严格对应 KeyId_E */
static const KeyCfgGpio_T S_tKeyHwConfig[keyNUM] =
{
	[keyPOWER] = {
		.rcu        = RCU_GPIOC,
		.gpio       = GPIOC,
		.pin        = GPIO_PIN_9,
		.mode       = GPIO_MODE_IN_FLOATING,
		.bPressHigh = true,
		.pName      = "Power",
	},

	#if(boardDCAC_EN)
	[keyAC] = {
		.rcu        = RCU_GPIOB,
		.gpio       = GPIOB,
		.pin        = GPIO_PIN_12,
		.mode       = GPIO_MODE_IPU,
		.bPressHigh = false,
		.pName      = "AC",
	},
	#endif  //boardDCAC_EN

	#if(boardDC_EN)
	[keyDC] = {
		.rcu        = RCU_GPIOB,
		.gpio       = GPIOB,
		.pin        = GPIO_PIN_1,
		.mode       = GPIO_MODE_IPU,
		.bPressHigh = false,
		.pName      = "DC",
	},
	#endif  //boardDC_EN
};

#define KEY_HW_NUM (sizeof(S_tKeyHwConfig) / sizeof(S_tKeyHwConfig[0]))

//****************************************************Global Functions********************************************************//
/***********************************************************************************************************************
 * 函数功能    : 按键 GPIO 初始化 (根据配置表统一配置时钟与模式)
 * 说明(备注)  : none
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_IfaceInit(void)
{
	for(uint8_t i = 0; i < KEY_HW_NUM; ++i)
	{
		// Power 按键不初始化, 由 ADC 模块负责
		if(i == keyPOWER)
		{
			continue;
		}

		rcu_periph_clock_enable(S_tKeyHwConfig[i].rcu);
		gpio_init(S_tKeyHwConfig[i].gpio, S_tKeyHwConfig[i].mode, GPIO_OSPEED_2MHZ, S_tKeyHwConfig[i].pin);
	}
}

/***********************************************************************************************************************
 * 函数功能    : 查表读取按键电平状态(硬件层唯一电平数据源, 极性已归一化)
 * 说明(备注)  : 按下返回 true, 未按下或越界返回 false
 * 传入参数    : e_id - 按键ID枚举
 * 输出参数    : none
 * 返回值      : true / false
 ************************************************************************************************************************/
bool bKey_IsPressById(KeyId_E e_id)
{
	uint8_t uc_idx = (uint8_t)e_id;
	if(uc_idx >= KEY_HW_NUM)
	{
		return false;
	}

	// Power 按键由 ADC 模块负责
	if(uc_idx == keyPOWER)
	{
		if(usAdc_GetChannelValue(adcKEY_POWER) > 200)//读取按键
			return true;
		else
			return false;
	}

	bool b_pin_level = (gpio_input_bit_get(S_tKeyHwConfig[uc_idx].gpio, S_tKeyHwConfig[uc_idx].pin) != RESET);
	return (b_pin_level == S_tKeyHwConfig[uc_idx].bPressHigh);
}

#if(boardLOW_POWER)
/***********************************************************************************************************************
 * 函数功能    : 按键引脚及外设进入低功耗状态
 * 说明(备注)  : 唤醒源沿用原代码 EXTI 配置(PC13/PA0, 与 Power 按键 PC9 独立),
 *               低功耗路径当前未启用, 启用前需按实际唤醒源实机验证
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_IoEnterLowPower(void)
{
	rcu_periph_clock_enable(RCU_PMU);
	rcu_periph_clock_enable(RCU_AF);

	for(uint8_t i = 0; i < KEY_HW_NUM; ++i)
	{
		rcu_periph_clock_enable(S_tKeyHwConfig[i].rcu);
	}

	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_9);
	#if(boardDCAC_EN)
	gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_12);
	#endif  //boardDCAC_EN

	nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
	nvic_irq_enable(EXTI0_IRQn, 2U, 0U);

	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOC, GPIO_PIN_SOURCE_13);
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);

	exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);
	exti_interrupt_flag_clear(EXTI_13);
	exti_interrupt_flag_clear(EXTI_0);
}

/***********************************************************************************************************************
 * 函数功能    : 按键引脚退出低功耗状态
 * 说明(备注)  : none
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_IoExitLowPower(void)
{
	vKey_IfaceInit();
}
#endif  //boardLOW_POWER

#endif  //boardKEY_EN
