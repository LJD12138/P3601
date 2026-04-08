
/***********************************************************************************************************************
 * Project : ProjectTeam
 * Module  : G:\1Baiku_Projects\15-M50\1.software\M5004-1\APP\Hardware\MD_Display
 * File    : md_display_api.h
 * Date    : 2026-01-10 16:10:01
 * Author  : LJD(291483914@qq.com)
 * Description: description
 * -------------------------------------------------------
 * todo    :
 * 1.
 * -------------------------------------------------------
 * Copyright (c) 2026 -inc
************************************************************************************************************************/
#ifndef MD_DISPLAY_API_H
#define MD_DISPLAY_API_H


#ifdef __cplusplus
extern "C" {
#endif

/* ==========================================includes====================================*/
#include "board_config.h"

#if(boardDISPLAY_EN)
/* ==========================================macros======================================*/
#define TIM_MAX    5941 //99+
#define POWER_MAX  9999 //9999
#define SOC_MAX    188 //188
#define O_ERR      0xFFFF 
 
//#define IN_OUT_ZEOR_POWER_HOR   1//输入输出0功率横杠
#define IN_OUT_ONE_ZEOR_POWER     2//输入输出1个0功率


/* ==========================================globals=====================================*/


/* ==========================================types=======================================*/


/* ==========================================extern======================================*/
//设备状态
void Display_IconWifi(void);
void Display_IconBL(void);
void Display_IconFan(void);
void Display_IconBuz(void);
void Display_IconLight(void);
void Display_IconUsbOut(void);
void Display_IconDcOut(void);
void Display_IconAcOut(void);
void Display_IconAcIn(void);
void Display_IconDcIn(void);
void Display_IconPvIn(void);
void Display_InputAnderson(void);//安德森输入标识
void Display_IconUps(void);

//符号
void Display_IconUpdata(void);//更新标识
void Display_SymbolPerCent(void);//百分号%标识

//错误
void Display_IconSysErr(void);
void Display_IconBatErr(void);
void Display_IconBatLock(void);
void Display_IconOT(void);
void Display_IconOutOT(void);
void Display_IconUT(void);
void Display_IconOL(void);

//设置指令
void Display_SetStandbyMode(void);
void Display_SetRunMode(void);
void Display_ClearData(void);
void Display_RefreshData(void);

//数据显示
void Display_NoShowAll(void);
void Display_ShowAll(void);
void Display_ShowON(void);
void Display_ShowOFF(void);
void Display_ForeverShow(void);
void Display_ShowInitState(void);
void Display_Time(u8 sw,u16 min);
void Display_TimRoll(u8 tim);
void Display_ShowChgFullTime(void);
void Display_BatChgRoll(u8 soc);
void Display_BAT(u8 cds, bool en_sw, u8 soc);
void Display_Soc(u8 soc);
void Display_InPwr(u16 power);
void Display_OutPwr(u16 power);

void Display_ShowErrCode(uint32_t list);
void Display_OutNum(s16 num);
void Display_UpdataProgress(u16 frame_num, u16 rec_frame_num);
void Display_UpdataState(u8 obj, u8 proto, u8 num);
void Display_UpdataTime(u16 min);
#endif  //

#ifdef __cplusplus
}
#endif

#endif  //MD_DISPLAY_API_H
