/*****************************************************************************************************************
*                                                                                                                *
 *                                         LCD错误显示                                                          *
*                                                                                                                *
******************************************************************************************************************/
#include "MD_Display/md_display_task.h"

#if(boardDISPLAY_EN)
#include "Sys/sys_task.h"

#if(boardBUZ_EN)
#include "Buz/buz_task.h"
#endif  //boardBUZ_EN

#if(boardUSB_EN)
#include "Usb/usb_task.h"
#endif  //boardUSB_EN

#if(boardDC_EN)
#include "Dc/dc_task.h"
#endif  //boardDC_EN

#if(boardBMS_EN)
#include "MD_Bms/md_bms_task.h"
#endif  //boardBMS_EN

#if(boardMPPT_EN)
#include "MD_Mppt/md_mppt_task.h"
#endif  //boardMPPT_EN

#if(boardDCAC_EN)
#include "MD_Dcac/md_dcac_task.h"
#include "MD_Dcac/md_dcac_rec_task.h"
#endif  //boardDCAC_EN



/***********************************************************************************************************************
-----函数功能    LCD错误代码显示
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      错误代码,0没有错误
                 错误码分配:
                 Sys:1-99    Bms:100-199   Mppt:200-299
                 Dcac:300-399  Dc:400-499   Usb:500-599
************************************************************************************************************************/
u16 usDisp_ErrCodeDisplay(void)
{
	static vu16 us_err_step = 1;
	static vu16 us_err_last_step = 0;
	static vu16 us_err_disp_cnt = 0;


	if(tSysInfo.uErrCode.usCode == 0

		#if(boardUSB_EN)
		&& tUsb.uErrCode.ucErrCode == 0
		#endif  //boardUSB_EN

		#if(boardDC_EN)
		&& tDc.uErrCode.ucErrCode == 0
		#endif  //boardDC_EN

		#if(boardBMS_EN)
		&& tBms.uErrCode.ulCode == 0
		#endif  //boardBMS_EN

		#if(boardMPPT_EN)
		&& tMppt.uErrCode.ulCode == 0
		#endif  //boardMPPT_EN

		#if(boardDCAC_EN)
		&& tDcac.uErrCode.ulCode == 0
		#endif  //boardDCAC_EN

		)
	{
		return 0;
	}

	switch(us_err_step)
	{
		//------------------------SYS 1~99------------------------------------
		{
		case 1:
			if(tSysInfo.uErrCode.tCode.bOT)
				break;
			else
				us_err_step++;

		case 2:
			if(tSysInfo.uErrCode.tCode.bUT)
				break;
			else
				us_err_step++;

		case 3:
			if(tSysInfo.uErrCode.tCode.bOV)
				break;
			else
				us_err_step++;

		case 4:
			if(tSysInfo.uErrCode.tCode.bUV)
				break;
			else
				us_err_step++;

		case 5:
			if(tSysInfo.uErrCode.tCode.bOL)
				break;
			else
				us_err_step = 100;
		}

		//------------------------BMS 100~199----------------------------------
		{
		case 100:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCellOV)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 101:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCellUV)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 102:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bEnvOT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 103:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bEnvUT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 104:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCOT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 105:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCUT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 106:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bDCOT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 107:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bDCUT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 108:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCOC)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 109:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bDCOC)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 110:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bSC)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 111:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bBatFull)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 112:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bAfeLost)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 113:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCurrErr)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 114:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bPrechgFault)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 115:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bLowVoltOL)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 116:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bParaLost)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 117:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bConsoleLost)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 118:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bDisChgMosErr)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 119:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bChgMosErr)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 120:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bMosErr1)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 121:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bMosErr2)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 122:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bMosErr3)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 123:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bAfeErr)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 124:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bVoltLow)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 125:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bNtcLost)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 126:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bCloseFault)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 127:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bBootFault)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 128:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bBmsErr)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 129:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bUnbalanced)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 130:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.uBmsCode.tCode.bBalanceWireLost)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 131:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.bSysDevLost)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 132:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.bSysChgOT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 133:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.bSysDisChgOT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 134:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.bSysChgUT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 135:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.bSysDisChgUT)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step++;

		case 136:
			#if(boardBMS_EN)
			if(tBms.uErrCode.tCode.bSysLV)
				break;
			else
			#endif  //boardBMS_EN
				us_err_step = 200;
		}

		//------------------------MPPT 200~299---------------------------------
		{
		case 200:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptInOV)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 201:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptInUV)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 202:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptInOC)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 203:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptInSC)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 204:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptOutOV)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 205:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptOutUV)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 206:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptOutOC)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 207:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptOutSC)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 208:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptOL)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 209:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptOT)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 210:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptEnFault)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 211:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bMpptInUP)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 212:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bDevLost)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 213:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bSysOT)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 214:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bSysUT)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 215:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bSysOV)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step++;

		case 216:
			#if(boardMPPT_EN)
			if(tMppt.uErrCode.tCode.bSysOL)
				break;
			else
			#endif  //boardMPPT_EN
				us_err_step = 300;
		}

		//------------------------DCAC 300~399---------------------------------
		{
		case 300:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacInVolt)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 301:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacInFreq)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 302:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacInOther)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 303:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacOutVolt)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 304:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacOutOther)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 305:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacHighVolt)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 306:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacBatOV)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 307:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacBatUV)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 308:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacOT)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 309:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacOL)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 310:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacOC)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 311:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacSC)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 312:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacFuse)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 313:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacRelay)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 314:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacPara)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 315:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacNtc)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 316:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacOther)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 317:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bDcacEeprom)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 318:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysDevLost)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 319:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysOT)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 320:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysUT)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 321:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysOV)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 322:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysLV)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 323:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysSetInProte)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 324:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysOutOL)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 325:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysOutErr)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step++;

		case 326:
			#if(boardDCAC_EN)
			if(tDcac.uErrCode.tCode.bSysInOC)
				break;
			else
			#endif  //boardDCAC_EN
				us_err_step = 400;
		}

		//------------------------DC 400~499------------------------------------
		{
		case 400:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bPowerErr)
				break;
			else
			#endif  //boardDC_EN
				us_err_step++;

		case 401:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bOT)
				break;
			else
			#endif  //boardDC_EN
				us_err_step++;

		case 402:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bOL)
				break;
			else
			#endif  //boardDC_EN
				us_err_step++;

		case 403:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bCloseFail)
				break;
			else
			#endif  //boardDC_EN
				us_err_step++;

		case 404:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bOutLow)
				break;
			else
			#endif  //boardDC_EN
				us_err_step++;

		case 405:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bOutHigh)
				break;
			else
			#endif  //boardDC_EN
				us_err_step++;

		case 406:
			#if(boardDC_EN)
			if(tDc.uErrCode.tCode.bNtcLost)
				break;
			else
			#endif  //boardDC_EN
				us_err_step = 500;
		}

		//------------------------USB 500~599------------------------------------
		{
		case 500:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bPowerErr)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 501:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bOT)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 502:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bOL)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 503:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bBatUV)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 504:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bIc1Lost)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 505:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bIc2Lost)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 506:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bBootFault)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step++;

		case 507:
			#if(boardUSB_EN)
			if(tUsb.uErrCode.tCode.bCloseFault)
				break;
			else
			#endif  //boardUSB_EN
				us_err_step = 0;
		}

		default:
			break;

	}

	if(us_err_last_step != us_err_step)
	{
		us_err_last_step = us_err_step;
		us_err_disp_cnt = 0;
	}

	us_err_disp_cnt++;
	if(us_err_disp_cnt >= (1000/boardDISP_REFRESH_TIME))
	{
		us_err_disp_cnt = 0;
		us_err_step++;
		if(us_err_step == 6)			us_err_step = 100;		//SYS->BMS
		else if(us_err_step == 137)	us_err_step = 200;		//BMS->MPPT
		else if(us_err_step == 217)	us_err_step = 300;		//MPPT->DCAC
		else if(us_err_step == 327)	us_err_step = 400;		//DCAC->DC
		else if(us_err_step == 407)	us_err_step = 500;		//DC->USB
		else if(us_err_step > 507)	us_err_step = 1;		//USB->SYS

		return 0;
	}
	else
	{
		return us_err_step;
	}
}
#endif  //boardDISPLAY_EN
