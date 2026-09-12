/*******************************************************************************************************************************
 * Project : APP
 * Module  : G:\1-Baiku_Projects\24-P36\1.software\P3601\APP\Hardware\Adc
 * File    : adc_iface.c
 * Date    : 2026-09-10
 * Author  : LJD(291483914@qq.com)
 * Desc    : ADC硬件驱动底层接口实现(含GPIO查表配置、DMA多通道循环采样及低功耗控制)
 * -------------------------------------------------------
 * todo    :
 * 1. none
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
 *******************************************************************************************************************************/

//****************************************************Includes******************************************************************//
#include "Adc/adc_iface.h"
#include "Adc/adc_task.h"

#if(boardADC_EN)

//****************************************************Types*******************************************************************//
typedef struct
{
	rcu_periph_enum rcu;
	uint32_t        gpio;
	uint32_t        pin;
	uint32_t        ch;
} AdcCfgGpio_T;

//****************************************************Parameter Initialization************************************************//
/* ADC引脚与通道硬件配置表: 顺序需严格对应 */
static const AdcCfgGpio_T S_tAdcGpioCfg[adcCHANNEL_NUM] =
{
	[adcSYS_IN_VOLT] = {	//电源输入电压 BAT_ADC
		.rcu  = RCU_GPIOA,
		.gpio = GPIOA,
		.pin  = GPIO_PIN_7,
		.ch   = ADC_CHANNEL_7,
	},
	[adcDC_TEMP] = {		//DC_360W 温度 DC-NTC1
		.rcu  = RCU_GPIOA,
		.gpio = GPIOA,
		.pin  = GPIO_PIN_4,
		.ch   = ADC_CHANNEL_4,
	},
	[adcDC_CURR] = {		//DC_360W 电流 DC-I
		.rcu  = RCU_GPIOA,
		.gpio = GPIOA,
		.pin  = GPIO_PIN_6,
		.ch   = ADC_CHANNEL_6,
	},
	[adcDC_VOLT] = {		//DC_360W 电压 DC-V
		.rcu  = RCU_GPIOA,
		.gpio = GPIOA,
		.pin  = GPIO_PIN_5,
		.ch   = ADC_CHANNEL_5,
	},
	[adcUSB_VOLT] = {		//USB 电压 USB-V
		.rcu  = RCU_GPIOC,
		.gpio = GPIOC,
		.pin  = GPIO_PIN_2,
		.ch   = ADC_CHANNEL_12,
	},
	[adcUSB_A_CURR] = {		//USB_A 电流 USB-I
		.rcu  = RCU_GPIOA,
		.gpio = GPIOA,
		.pin  = GPIO_PIN_0,
		.ch   = ADC_CHANNEL_0,
	},
	[adcUSB_A_VOLT] = {		//USB_A 电压 USB-V
		.rcu  = RCU_GPIOA,
		.gpio = GPIOA,
		.pin  = GPIO_PIN_1,
		.ch   = ADC_CHANNEL_1,
	},
	[adcKEY_POWER] = {		//Key Power按键
		.rcu  = RCU_GPIOC,
		.gpio = GPIOC,
		.pin  = GPIO_PIN_5,
		.ch   = ADC_CHANNEL_15,
	},
	[adcFAN_VOLT] = {		//FAN_供电 电压 12V-V
		.rcu  = RCU_GPIOC,
		.gpio = GPIOC,
		.pin  = GPIO_PIN_4,
		.ch   = ADC_CHANNEL_14,
	},
};

u16 adc_value[adcCHANNEL_NUM];

//****************************************************Local Functions*********************************************************//
/***********************************************************************************************************************
 * 函数功能    : 毫秒级延时函数
 * 说明(备注)  : 用于ADC启动稳定等待
 * 传入参数    : time - 延时计数值
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
static void v_delay_1ms(u16 time)
{
	vu32 i = 0;
	while(time--)
	{
		i = 24000;
		while(i--);
	}
}

/***********************************************************************************************************************
 * 函数功能    : ADC采样引脚GPIO初始化
 * 说明(备注)  : 遍历配置表初始化各模拟通道引脚
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
static void v_gpio_config(void)
{
	for(uint8_t i = 0; i < adcCHANNEL_NUM; ++i)
	{
		rcu_periph_clock_enable(S_tAdcGpioCfg[i].rcu);
		gpio_init(S_tAdcGpioCfg[i].gpio, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, S_tAdcGpioCfg[i].pin);
	}
}

/***********************************************************************************************************************
 * 函数功能    : ADC DMA传输配置
 * 说明(备注)  : 配置DMA单数据循环搬运ADC转换结果至 adc_value 缓存
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
#if(ADC_DMAX)
static void v_dma_config(void)
{
	dma_parameter_struct dma_data_parameter;

	/* ADC DMA通道复位 */
	dma_deinit(adcDMA, adcDMA_CH);

	/* 初始化DMA单数据模式 */
	dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADCX));
	dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
	dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_data_parameter.number       = adcCHANNEL_NUM;
	dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
	dma_init(adcDMA, adcDMA_CH, &dma_data_parameter);

	/* 使能DMA循环模式 */
	dma_circulation_enable(adcDMA, adcDMA_CH);

	/* 使能DMA通道 */
	dma_channel_enable(adcDMA, adcDMA_CH);
}
#endif

/***********************************************************************************************************************
 * 函数功能    : ADC工作模式与常规通道配置
 * 说明(备注)  : 启用扫描与连续转换模式，遍历配置表配置各通道采样周期并校准ADC
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
static void v_adc_config(void)
{
	/* ADC自由模式 */
	adc_mode_config(ADC_MODE_FREE);
	/* ADC连续转换模式使能 */
	adc_special_function_config(ADCX, ADC_CONTINUOUS_MODE, ENABLE);
	/* ADC扫描模式使能 */
	adc_special_function_config(ADCX, ADC_SCAN_MODE, ENABLE);
	/* ADC数据右对齐 */
	adc_data_alignment_config(ADCX, ADC_DATAALIGN_RIGHT);

	/* 配置常规通道长度 */
	adc_channel_length_config(ADCX, ADC_REGULAR_CHANNEL, adcCHANNEL_NUM);

	/* 遍历配置常规通道序列及采样时间 */
	for(uint8_t i = 0; i < adcCHANNEL_NUM; ++i)
	{
		adc_regular_channel_config(ADCX, i, S_tAdcGpioCfg[i].ch, ADC_SAMPLETIME_239POINT5);
	}

	/* 软件触发配置 */
	adc_external_trigger_source_config(ADCX, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADCX, ADC_REGULAR_CHANNEL, ENABLE);

	/* 使能ADC DMA传输 */
	adc_dma_mode_enable(ADCX);

	/* 使能ADC外设 */
	adc_enable(ADCX);
	/* 等待ADC稳定 */
	v_delay_1ms(1);
	/* ADC校准 */
	adc_calibration_enable(ADCX);

	/* 软件触发启动转换 */
	adc_software_trigger_enable(ADCX, ADC_REGULAR_CHANNEL);
}

//****************************************************Global Functions********************************************************//
/***********************************************************************************************************************
 * 函数功能    : ADC底层硬件初始化
 * 说明(备注)  : 使能时钟、配置分频、依次初始化GPIO、DMA和ADC外设
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vAdc_Init(void)
{
	/* 使能ADC时钟 */
	rcu_periph_clock_enable(ADCX_RCU);
	/* 使能DMA时钟 */
	rcu_periph_clock_enable(adcDMA_RCU);
	/* 配置ADC时钟分频 */
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV16);

	/* 配置GPIO */
	v_gpio_config();

	/* 配置DMA */
#if (ADC_DMAX)
	v_dma_config();
#endif

	/* 配置ADC */
	v_adc_config();
}

/***********************************************************************************************************************
 * 函数功能    : ADC底层硬件复位
 * 说明(备注)  : 复位ADC外设
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vAdc_DeInit(void)
{
	adc_deinit(ADCX);
}

/***********************************************************************************************************************
 * 函数功能    : 获取指定通道 ADC 原始采集值
 * 说明(备注)  : none
 * 传入参数    : channel - 通道
 * 输出参数    : none
 * 返回值      : 选定通道的 16 位 AD 转换数据，越界返回 0
 ************************************************************************************************************************/
u16 usAdc_GetChannelValue(AdcChannel_E e_channel)
{
	if(e_channel >= adcCHANNEL_NUM)
	{
		return 0;
	}

	return adc_value[e_channel];
}

#if(boardLOW_POWER)
/***********************************************************************************************************************
 * 函数功能    : ADC引脚及外设进入低功耗状态
 * 说明(备注)  : 引脚置为模拟输入低功耗态，关闭ADC/DMA外设并停止时钟
 * 传入参数    : none
 * 输出参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vAdc_IoEnterLowPower(void)
{
	for(uint8_t i = 0; i < adcCHANNEL_NUM; ++i)
	{
		gpio_init(S_tAdcGpioCfg[i].gpio, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, S_tAdcGpioCfg[i].pin);
	}

	adc_disable(ADCX);
	dma_channel_disable(adcDMA, adcDMA_CH);
	rcu_periph_clock_disable(ADCX_RCU);
	rcu_periph_clock_disable(adcDMA_RCU);
}
#endif  //boardLOW_POWER

#endif  //boardADC_EN
