/*******************************************************************************************************************************
 * Project : BOOT
 * Module  : G:\1-Baiku_Projects\24-P36\1.software\P3601\BOOT\Hardware\Key
 * File    : key_iface.h
 * Date    : 2026-09-10
 * Author  : LJD(291483914@qq.com)
 * Desc    : 按键硬件驱动底层接口定义
 * -------------------------------------------------------
 * todo    :
 * 1. none
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
 *******************************************************************************************************************************/

#ifndef KEY_IFACE_H
#define KEY_IFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ==========================================includes====================================*/
#include "board_config.h"

#if(boardKEY_EN)

/* ==========================================types=======================================*/
/* 按键ID枚举: 枚举序与硬件配置表严格一致 */
typedef enum
{
	keyPOWER = 0,

	#if(boardDCAC_EN)
	keyAC,
	#endif

	#if(boardLIGHT_EN)
	keyLIGHT,
	#endif

	#if(boardUSB_EN)
	keyUSB,
	#endif

	#if(boardDC_EN)
	keyDC,
	#endif

	keyNUM
} KeyId_E;

/* ==========================================extern======================================*/
void vKey_IfaceInit(void);
bool bKey_IsPressById(KeyId_E e_id);

#if(boardLOW_POWER)
void vKey_IoEnterLowPower(void);
void vKey_IoExitLowPower(void);
#endif  //boardLOW_POWER

#endif  //boardKEY_EN

#ifdef __cplusplus
}
#endif

#endif  //KEY_IFACE_H
