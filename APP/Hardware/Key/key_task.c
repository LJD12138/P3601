/*******************************************************************************************************************************
 * Project : APP
 * Module  : G:\1-Baiku_Projects\24-P36\1.software\P3601\APP\Hardware\Key
 * File    : key_task.c
 * Date    : 2026-09-10
 * Author  : LJD(291483914@qq.com)
 * Desc    : 按键任务及中间件集成胶水层
 *           1. 硬件层(key_iface.c/.h): 负责GPIO引脚/模式/极性配置与电平读取(极性归一化)
 *           2. 中间件(Middlewares/MultiFuncKey): 负责状态机/去抖/长短按/超长按/组合键时序
 *           3. 任务胶水层(本文件): 组装中间件配置、任务调度与系统状态联动
 * -------------------------------------------------------
 * todo    :
 * 1. none
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
 *******************************************************************************************************************************/

//****************************************************Includes******************************************************************//
#include "Key/key_task.h"

#if(boardKEY_EN)
#include "Key/key_iface.h"
#include "Key/key_func.h"
#include "Sys/sys_task.h"
#include "Print/print_task.h"

#include "mf_key.h"

#if(boardUSE_OS)
#include "freertos.h"
#include "task.h"
#endif  //boardUSE_OS

#if(boardBUZ_EN)
#include "Buz/buz_task.h"
#endif  //boardBUZ_EN

#if(boardDISPLAY_EN)
#include "MD_Display/md_display_task.h"
#endif  //boardDISPLAY_EN

//****************************************************任务初始化*******************************************************//
#if(boardUSE_OS)
#define 		KEY_TASK_PRIO                  			2     //任务优先级
#define 		KEY_TASK_STK_SIZE              			256     //任务堆栈
TaskHandle_t 	tKeyTaskHandler = NULL;
void 			vKey_Task(void *pvParameters);
#endif  //boardUSE_OS

//****************************************************参数初始化*******************************************************//
static bool b_key_lock = false;   /* 开机长按锁定标志 */

//****************************************************函数声明*********************************************************//
static bool b_key_event_pre_proc(u8 uc_idx, bool b_long);
static void v_key_on_super_long(u8 uc_idx);
static void v_key_on_any_press(void);

//****************************************************按键表**********************************************************//
/* 中间件按键配置表: 事件码映射 + 触发方式配置(表序与 KeyId_E 严格一致) */
static const MfKeyItemCfg_T S_tKeyMwKeyCfg[] =
{
	/* 短按事件           长按事件          多功能  长按累加 */
	[keyPOWER] = {KTE_POWER_SHORT,  KTE_POWER_LONG,  false,  false},

	#if(boardDCAC_EN)
	[keyAC]    = {KTE_AC_SHORT,     KTE_AC_LONG,     true,   false},
	#endif  //boardDCAC_EN

	#if(boardDC_EN)
	[keyDC]    = {KTE_DC_SHORT,     KTE_DC_LONG,     false,  false},
	#endif  //boardDC_EN
};

#define KEY_MW_KEY_TBL_NUM (sizeof(S_tKeyMwKeyCfg) / sizeof(S_tKeyMwKeyCfg[0]))

/* 编译期校验: KeyId_E 枚举序必须与 S_tKeyMwKeyCfg[] 表序严格一致 */
typedef char __key_mw_tbl_order_assert[(KEY_MW_KEY_TBL_NUM == keyNUM) ? 1 : -1];

/* 事件序列缓冲与中间件全局配置 */
static u8 S_ucKeySeqBuff[keyGROUP_NUM];

static const MfKeyCfg_T S_tKeyMwCfg =
{
	/* 硬件接口回调 */
	(bool (*)(u8))bKey_IsPressById,    /* 查表读取按键按下电平(极性已归一化) */
	/* 扫描时序参数(单位: keyTASK_CYCLE_TIME 周期数) */
	keySHORT_PRESS_TIME,               /* 短按最小时间 */
	keyLONG_PRESS_TIME,                /* 长按最小时间 */
	keySUPER_LONG_PRESS_TIME,          /* 超长按最小时间 */
	keyNUPRESS_MAX_TIME,               /* 组合键最大等待时间 */
	keyADD_SPACE_TIME,                 /* 长按累加间隔 */
	/* 事件序列缓冲 */
	S_ucKeySeqBuff,                    /* 缓冲地址 */
	keyGROUP_NUM,                      /* 缓冲长度 */
	KTE_FUN_NULL,                      /* 序列空闲填充码 */
	/* 业务回调 */
	vKey_ProcKeyFunc,                   /* 序列就绪 -> 业务动作分发 */
	b_key_event_pre_proc,              /* 事件录入前预处理(息屏唤醒/调试日志) */
	v_key_on_super_long,               /* 超长按提示(打印+蜂鸣) */
	v_key_on_any_press                 /* 任意键按下(清休眠计数) */
};


/***********************************************************************************************************************
 * 函数功能    : 按键任务初始化 (底层GPIO初始化 + 中间件初始化 + 创建OS任务)
 * 传入参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_TaskInit(void)
{
	/* 底层硬件接口初始化 (RCU + GPIO) */
	vKey_IfaceInit();

	/* 多功能按键中间件初始化 (时序 + 回调 + 按键表) */
	vMfKey_Init(&S_tKeyMwCfg, S_tKeyMwKeyCfg, KEY_MW_KEY_TBL_NUM);

	#if(boardUSE_OS)
	xTaskCreate((TaskFunction_t )vKey_Task,
	            (const char*    )"bKeyTask",
	            (uint16_t       )KEY_TASK_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )KEY_TASK_PRIO,
	            (TaskHandle_t*  )&tKeyTaskHandler);
	#endif  //boardUSE_OS
}

/***********************************************************************************************************************
 * 函数功能    : 按键循环扫描任务 (胶水层: 任务调度 + 电源键触发方式策略 + 中间件扫描)
 * 传入参数    : pvParameters
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_Task(void *pvParameters)
{
	#if(boardUSE_OS)
	for(;;)
	#endif  //boardUSE_OS
	{
		//GPIO初始化未完成 (长按开机锁定)
		if(tSysInfo.uInit.tFinish.bIF_Gpio == 0)
		{
			b_key_lock = bKey_IsPressById(keyPOWER);

			#if(boardUSE_OS)
			vTaskDelay(500);
			continue;
			#else
			return;
			#endif
		}

		//长按开启不松开
		if(b_key_lock == true && bKey_IsPressById(keyPOWER) == true)
		{
			#if(boardUSE_OS)
			vTaskDelay(keyTASK_CYCLE_TIME);
			continue;
			#else
			return;
			#endif
		}

		b_key_lock = false;

		//动态配置电源键多功能触发方式(保持原隐蔽语义:
		//ENG模式使能时任何状态恒为true; 否则仅关机态为false)
		#if(boardENG_MODE_EN)
		vMfKey_SetMultiKeyEn(keyPOWER, true);
		#else
		vMfKey_SetMultiKeyEn(keyPOWER, (tSysInfo.eDevState != DS_SHUT_DOWN));
		#endif

		//按键扫描(中间件: 去抖/长短按/超长按/组合键时序状态机)
		vMfKey_Scan();

		#if(boardUSE_OS)
		vTaskDelay(keyTASK_CYCLE_TIME);
		#endif
	}
}

/***********************************************************************************************************************
 * 函数功能    : 电源按键已经被外部处理 (防止全局按键重复触发)
 * 传入参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_PowerIsTri(void)
{
	vMfKey_MarkProcessed(keyPOWER);
}

/***********************************************************************************************************************
 * 函数功能    : 按键参数初始化/清空事件缓冲区
 * 传入参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_ParamInit(void)
{
	vMfKey_ClearSeq();
}

/***********************************************************************************************************************
 * 函数功能    : 检查是否有任意按键按下
 * 传入参数    : none
 * 返回值      : true: 存在按键按下, false: 无按键按下
 ************************************************************************************************************************/
bool bKey_IsAnyPress(void)
{
	uint8_t i;
	for(i = 0; i < keyNUM; i++)
	{
		if(bKey_IsPressById((KeyId_E)i) == true)
		{
			return true;
		}
	}
	return false;
}

/***********************************************************************************************************************
 * 函数功能    : 工厂模式组合按键检测 (Power + DC 按下, 其他按键未按下)
 * 说明(备注)  : 原逻辑 bKey_UsbIsPress()/bKey_LightIsPress() 为硬件不存在的按键(恒false),
 *               现按键表无该键, 行为等价
 * 传入参数    : none
 * 返回值      : true: 满足工厂模式组合键, false: 不满足
 ************************************************************************************************************************/
bool bKey_IsFactoryModePress(void)
{
	#if(boardDC_EN)
	return (bKey_IsPressById(keyPOWER)
		&& bKey_IsPressById(keyDC)
		#if(boardDCAC_EN)
		&& !bKey_IsPressById(keyAC)
		#endif
	);
	#else
	return false;
	#endif
}

/***********************************************************************************************************************
 * 函数功能    : 工程模式组合按键检测 (Power + DC 按下, 其他按键未按下)
 * 说明(备注)  : 原逻辑工厂模式与工程模式判断条件相同(死分支), 保持等价
 * 传入参数    : none
 * 返回值      : true: 满足工程模式组合键, false: 不满足
 ************************************************************************************************************************/
bool bKey_IsEngModePress(void)
{
	#if(boardDC_EN)
	return (bKey_IsPressById(keyPOWER)
			&& bKey_IsPressById(keyDC)
			#if(boardDCAC_EN)
			&& !bKey_IsPressById(keyAC)
			#endif
	);
	#else
	return false;
	#endif
}

#if(boardLOW_POWER)
/***********************************************************************************************************************
 * 函数功能    : 按键进入低功耗
 * 传入参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_EnterLowPower(void)
{
	vKey_IoEnterLowPower();

	#if(boardUSE_OS)
	vTaskSuspend(tKeyTaskHandler);
	#endif  //boardUSE_OS
}

/***********************************************************************************************************************
 * 函数功能    : 按键退出低功耗
 * 传入参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
void vKey_ExitLowPower(void)
{
	vKey_IoExitLowPower();

	#if(boardUSE_OS)
	vTaskResume(tKeyTaskHandler);
	#endif  //boardUSE_OS
}
#endif  //boardLOW_POWER


//****************************************************Business Callbacks (中间件钩子)*****************************************//
/***********************************************************************************************************************
 * 函数功能    : 事件录入前预处理回调: 息屏唤醒吞掉首个事件; 正常事件打印调试日志
 * 传入参数    : uc_idx: 按键索引, b_long: 是否为长按
 * 返回值      : false: 吞掉该事件, true: 正常录入
 ************************************************************************************************************************/
static bool b_key_event_pre_proc(u8 uc_idx, bool b_long)
{
	#if(boardDISPLAY_EN)
	if(!tDisp.bLight && bSys_IsWorkState() == true) //非关机状态下,息屏第一个功能不执行
	{
		bDisp_Switch(ST_ON, false);
		if(uPrint.tFlag.bKeyTask)
		{
			sMyPrint("Key_Task:当前息屏,按键功能退出\r\n");
		}
		return false;
	}
	#endif  //boardDISPLAY_EN

	if(uPrint.tFlag.bKeyTask)
	{
		static const char * const S_pKeyNames[] =
		{
			"Power",
			#if(boardDCAC_EN)
			"AC",
			#endif
			#if(boardDC_EN)
			"DC",
			#endif
		};
		const char *p_name = (uc_idx < KEY_MW_KEY_TBL_NUM) ? S_pKeyNames[uc_idx] : "Unknown";
		sMyPrint("Key_Task:%s%s\r\n", p_name, b_long ? "长按" : "短按");
	}

	return true;
}

/***********************************************************************************************************************
 * 函数功能    : 超长按提示回调: 打印 + 蜂鸣
 * 传入参数    : uc_idx: 按键索引
 * 返回值      : none
 ************************************************************************************************************************/
static void v_key_on_super_long(u8 uc_idx)
{
	(void)uc_idx;

	if(uPrint.tFlag.bKeyTask)
	{
		sMyPrint("Key_Task:触发长按事件\r\n");
	}

	#if(boardBUZ_EN)
	bBuz_Tweet(SHORT_1);
	#endif  //boardBUZ_EN
}

/***********************************************************************************************************************
 * 函数功能    : 任意按键按下回调: 清休眠计数
 * 传入参数    : none
 * 返回值      : none
 ************************************************************************************************************************/
static void v_key_on_any_press(void)
{
	tSysInfo.usNeedSleepCnt = 0;
}
#endif  //boardKEY_EN
