/*******************************************************************************************************************************
 * Project : BOOT
 * Module  : G:\1-Baiku_Projects\24-P36\1.software\P3601\BOOT\Hardware\Key
 * File    : key_task.h
 * Date    : 2026-09-10
 * Author  : LJD(291483914@qq.com)
 * Desc    : 按键任务头文件及对外业务接口定义
 * -------------------------------------------------------
 * todo    :
 * 1. none
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
 *******************************************************************************************************************************/

#ifndef KEY_TASK_H_
#define KEY_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ==========================================includes====================================*/
#include "Key/key_iface.h"

#if(boardKEY_EN)

/* ==========================================macros======================================*/
#define     keyTASK_CYCLE_TIME                10   //按键任务更新时间 (ms)
#define     keyGROUP_NUM                      10   //组合按键种类/最大事件缓冲深度
#define     keySHORT_PRESS_TIME               2    //短按按键的最小时间 * 10ms = 20ms
#define     keyLONG_PRESS_TIME                90   //长按按键的最小时间 * 10ms = 900ms
#define     keySUPER_LONG_PRESS_TIME          250  //超长按按键的最小时间 * 10ms = 2500ms
#define     keyNUPRESS_MAX_TIME               35   //组合按键最大的等待时间 * 10ms = 350ms
#define     keyADD_SPACE_TIME                 20   //长按累加间隔 * 10ms = 200ms

/* ==========================================types=======================================*/
/* 触发事件枚举 */
typedef enum
{
	KTE_FUN_NULL = 0,
	KTE_POWER_LONG,
	KTE_POWER_SHORT,
	KTE_POWER_SUPER_LONG,
	KTE_AC_LONG,
	KTE_AC_SHORT,
	KTE_AC_SUPER_LONG,
	KTE_LIGHT_LONG,
	KTE_LIGHT_SHORT,
	KTE_LIGHT_SUPER_LONG,
	KTE_USB_LONG,
	KTE_USB_SHORT,
	KTE_USB_SUPER_LONG,
	KTE_DC_LONG,
	KTE_DC_SHORT,
	KTE_DC_SUPER_LONG,
} KeyTriEvent_E;

typedef KeyTriEvent_E KeyTriEvent_e;

/* ==========================================extern======================================*/
void vKey_TaskInit(void);
void vKey_PowerIsTri(void);
void vKey_ParamInit(void);
bool bKey_IsAnyPress(void);
bool bKey_IsFactoryModePress(void);
bool bKey_IsEngModePress(void);

#if(!boardUSE_OS)
void vKey_Task(void *pvParameters);
#endif

#if(boardLOW_POWER)
void vKey_EnterLowPower(void);
void vKey_ExitLowPower(void);
#endif

#endif  //boardKEY_EN

#ifdef __cplusplus
}
#endif

#endif  //KEY_TASK_H_
