#include "gpio_init.h"
#include "Print/print_iface.h"

/***********************************************************************************************************************
-----函数功能    IO口初始化
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/ 
void vGPIO_Init(void)
{
	//	//初始化所有IO,已达到最低功耗.
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOE);
	rcu_periph_clock_enable(RCU_GPIOF);
	rcu_periph_clock_enable(RCU_GPIOG);
	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	gpio_init(GPIOD, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	gpio_init(GPIOE, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	gpio_init(GPIOF, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	gpio_init(GPIOG, GPIO_MODE_AIN, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
	
	
	rcu_periph_clock_enable(KEY_POWER_RCU);
	gpio_init(KEY_POWER_GPIO,GPIO_MODE_IPU,GPIO_OSPEED_2MHZ,KEY_POWER_PIN);
	
	rcu_periph_clock_enable(gpioASSIST_OPEN_RCU);
	gpio_init(gpioASSIST_OPEN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, gpioASSIST_OPEN_PIN);
	gpioASSIST_OPEN_ON();

	//风扇供电使能
	rcu_periph_clock_enable(fanPWM_GPIO_RCU);
	gpio_init(fanPWM_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ,fanPWM_PIN);
	GPIO_BC(GPIO_MODE_OUT_PP) = (uint32_t)fanPWM_PIN;
}

/***********************************************************************************************************************
-----函数功能    获取跳转到APP程序信号
-----说明(备注)  1MS刷新
-----传入参数    none
-----输出参数    none
-----返回值      true:跳转APP   false:留在Boot
************************************************************************************************************************/ 
bool bGPIO_BootJumpApp(void)
{
//	static  u8  uc_key_tri_cnt;
	
//	if(KEY_POWER_IsPress() == true)
	{
//		if(++uc_key_tri_cnt >= 30)
		{
//			uc_key_tri_cnt = 0;
			return true;
		}
	}
//	else 
//	{
//		uc_key_tri_cnt = 0;
//	}
//	return false;
}


#if(boardLOW_POWER)
/***********************************************************************************************************************
-----函数功能    IO口进入低功耗
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/ 
void vKey_EnterLowPower(void)
{
	rcu_periph_clock_disable(RCU_GPIOA);
	rcu_periph_clock_disable(RCU_GPIOB);
	rcu_periph_clock_disable(RCU_GPIOD); 
	rcu_periph_clock_disable(RCU_GPIOE);
	rcu_periph_clock_disable(RCU_GPIOF);
	rcu_periph_clock_disable(RCU_GPIOG);
	
	/* enable clock */
    rcu_periph_clock_enable(RCU_PMU);
	rcu_periph_clock_enable(KEY_POWER_RCU);     //C
	rcu_periph_clock_enable(KEY_WP_RCU);        //A
	rcu_periph_clock_enable(attiSENSOR_INT_RCU);//A
	rcu_periph_clock_enable(PRINT_RX_RCU);      //D
	rcu_periph_clock_enable(RCU_AF);
	
	gpio_init(KEY_POWER_GPIO,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,KEY_POWER_PIN);             //中断 电源按键 PC13
	gpio_init(KEY_WP_GPIO,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,KEY_WP_PIN);                   //中断 唤醒脚 PA0
	gpio_init(KEY_KEY1_GPIO,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,KEY_KEY1_PIN);
	gpio_init(attiSENSOR_INT_GPIO,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,attiSENSOR_INT_PIN);   //中断 状态传感器 PA9
	gpio_init(PRINT_RX_GPIO,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,PRINT_RX_PIN);               //中断 串口接收   PD2
	
	/* enable and set key EXTI interrupt to the lowest priority */
	nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
	nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);
	nvic_irq_enable(EXTI2_IRQn, 2U, 0U);
	nvic_irq_enable(EXTI0_IRQn, 2U, 0U);

	/* connect key EXTI line to key GPIO pin */
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOC, GPIO_PIN_SOURCE_13); //PC13
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_9);  //PA9
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOD, GPIO_PIN_SOURCE_2);  //PD2
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);  //PA0

	/* configure key EXTI line */
	exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING); //下降沿触发
	exti_init(EXTI_9, EXTI_INTERRUPT, EXTI_TRIG_RISING);   //上升沿触发
	exti_init(EXTI_2, EXTI_INTERRUPT, EXTI_TRIG_RISING);   //上升沿触发
	exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);   //上升沿触发
	exti_interrupt_flag_clear(EXTI_13);
	exti_interrupt_flag_clear(EXTI_9);
	exti_interrupt_flag_clear(EXTI_2);
	exti_interrupt_flag_clear(EXTI_0);
	exti_interrupt_enable(EXTI_13);//
	exti_interrupt_enable(EXTI_9);//
	exti_interrupt_enable(EXTI_2);//
	exti_interrupt_enable(EXTI_0);//
}

/***********************************************************************************************************************
-----函数功能    IO口退出低功耗
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/ 
void vGPIO_ExitLowPower(void)
{
	vGPIO_Init();
}



/***********************************************************************************************************************
-----函数功能    IO口进入APP程序
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/ 
void vGPIO_EnterApp(void)
{
	gpio_init(KEY_POWER_GPIO,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,KEY_POWER_PIN);             //中断 电源按键   PC13
	gpio_init(KEY_WP_GPIO,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,KEY_WP_PIN);                   //中断 唤醒脚     PA0
	gpio_init(KEY_KEY1_GPIO,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,KEY_KEY1_PIN);
	gpio_init(attiSENSOR_INT_GPIO,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,attiSENSOR_INT_PIN);   //中断 状态传感器 PA9
//	gpio_init(PRINT_RX_GPIO,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,PRINT_RX_PIN);               //中断 串口接收   PD2
	
	nvic_irq_disable(EXTI10_15_IRQn);
	nvic_irq_disable(EXTI5_9_IRQn);
	nvic_irq_disable(EXTI2_IRQn);
	nvic_irq_disable(EXTI0_IRQn);
	
	exti_interrupt_flag_clear(EXTI_13);
	exti_interrupt_flag_clear(EXTI_9);
	exti_interrupt_flag_clear(EXTI_2);
	exti_interrupt_flag_clear(EXTI_0);
	exti_interrupt_disable(EXTI_13);//
	exti_interrupt_disable(EXTI_9);//
	exti_interrupt_disable(EXTI_2);//
	exti_interrupt_disable(EXTI_0);//
}

#endif





