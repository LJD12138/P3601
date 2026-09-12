/*******************************************************************************************************************************
 * Project : APP
 * Module  : G:\1-Baiku_Projects\24-P36\1.software\P3601\APP\Hardware\Adc
 * File    : adc_iface.h
 * Date    : 2026-09-10
 * Author  : LJD(291483914@qq.com)
 * Desc    : ADC硬件驱动底层接口定义
 * -------------------------------------------------------
 * todo    :
 * 1. none
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
 *******************************************************************************************************************************/

#ifndef ADC_IFACE_H
#define ADC_IFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ==========================================includes====================================*/
#include "board_config.h"

#if(boardADC_EN)

/* ==========================================macros======================================*/
#define     ADC_DMAX                                2

#if (ADC_DMAX == 1)
#define     ADCX_RCU                                RCU_ADC0
#define     ADCX                                    ADC0
#define     adcDMA_RCU                              RCU_DMA1
#define     adcDMA                                  DMA1
#define     adcDMA_CH                               DMA_CH4

#define     DMA_SUBPERIX                            DMA_SUBPERI0
#elif (ADC_DMAX == 2)
#define     ADCX_RCU                                RCU_ADC0
#define     ADCX                                    ADC0
#define     adcDMA_RCU                              RCU_DMA0
#define     adcDMA                                  DMA0
#define     adcDMA_CH                               DMA_CH0
//#define     DMA_SUBPERIX                            DMA_SUBPERI0
#endif

/* ==========================================types=======================================*/
/* ADC通道枚举: 枚举序与硬件配置表、DMA缓存及规则通道序列严格一致 */
typedef enum
{
    adcSYS_IN_VOLT = 0,     //电源输入电压 BAT_ADC
    adcDC_TEMP,             //DC_360W 温度 DC-NTC1
    adcDC_CURR,             //DC_360W 电流 DC-I
    adcDC_VOLT,             //DC_360W 电压 DC-V
    adcUSB_VOLT,            //USB 电压 USB-V
    adcUSB_A_CURR,          //USB_A 电流 USB-I
    adcUSB_A_VOLT,          //USB_A 电压 USB-V
    adcKEY_POWER,           //Key Power按键
    adcFAN_VOLT,            //FAN_供电 电压 12V-V
    adcCHANNEL_NUM          //DMA缓存大小
} AdcChannel_E;

/* ==========================================extern======================================*/
void vAdc_Init(void);
void vAdc_DeInit(void);
u16 usAdc_GetChannelValue(AdcChannel_E e_channel);

#if(boardLOW_POWER)
void vAdc_IoEnterLowPower(void);
#endif

#endif  //boardADC_EN

#ifdef __cplusplus
}
#endif

#endif  //ADC_IFACE_H
