/*****************************************************************************************************************
*                                                                                                                *
 *                                         Disp显示任务                                                          *
*                                                                                                                *
******************************************************************************************************************/
#include "MD_Display/md_display_task.h"

#if(boardDISPLAY_EN)
#include "MD_Display/md_display_api.h"
#include "MD_Display/md_display_iface.h"
#include "Sys/sys_task.h"
#include "Print/print_task.h"

#include "app_info.h"


#if(boardKEY_EN)
#include "Key/key_task.h"
#endif  //boardKEY_EN

#if(boardUSB_EN)
#include "Usb/usb_task.h"
#endif  //boardUSB_EN

#if(boardDC_EN)
#include "Dc/dc_task.h"
#endif  //boardDC_EN

#if(boardLIGHT_EN)
#include "MD_Light/md_light_task.h"
#endif  //boardLIGHT_EN

#if(boardDISPLAY_EN)
#include "MD_Display/md_display_task.h"
#endif  //boardDISPLAY_EN

#if(boardBUZ_EN)
#include "Buz/buz_task.h"
#endif  //boardBUZ_EN

#if(boardBMS_EN)
#include "MD_Bms/md_bms_rec_task.h"
#include "MD_Bms/md_bms_task.h"
#endif  //boardBMS_EN

#if(boardMPPT_EN)
#include "MD_Mppt/md_mppt_rec_task.h"
#include "MD_Mppt/md_mppt_task.h"
#endif  //boardMPPT_EN

#if(boardDCAC_EN)
#include "MD_Dcac/md_dcac_task.h"
#include "MD_Dcac/md_dcac_rec_task.h"
#endif  //DCAC使能

#if(boardUPDATA)
#include "Sys/sys_queue_task_updata.h"
#endif  //boardUPDATA

#if(boardHEAT_MANAGE_EN)
#include "MD_HeatManage/md_hm_task.h"
#endif  //boardHEAT_MANAGE_EN


//****************************************************任务参数初始化**********************************************//
#if(boardUSE_OS)
#define			dispTASK_PRIO                   2       //任务优先级 
#define			dispTASK_STK_SIZE               256     //任务堆栈  实际字节数 *4
TaskHandle_t	tDispTaskHandler = NULL; 
void vDisp_Task(void *pvParameters);
#endif  //boardUSE_OS

//****************************************************参数初始化**************************************************//
Disp_T   tDisp; 

//****************************************************局部函数定义************************************************//
static void v_disp_init(void);
static void v_disp_closing(void);
static void v_disp_shut_down(void);
static void v_disp_booting(void);
static void v_disp_work(void);

/***********************************************************************************************************************
-----函数功能    参数初始化
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
static void v_disp_param_init(void)
{
	memset(&tDisp, 0, sizeof(tDisp));
	
	Display_ClearData();   //Buff清零,避免残留
	
	tDisp.usAutoOffTime = boardDISP_OFF_TIME;
	tDisp.bSleepShow =true;//待机强制打开亮屏
}

/***********************************************************************************************************************
-----函数功能    Disp显示任务初始化
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
bool bDisp_TaskInit(void)
{
	v_disp_param_init();
	
	HT1621_IfaceInit();
	
	#if(boardUSE_OS)
	xTaskCreate((TaskFunction_t )vDisp_Task,			//任务函数
                (const char* )"DispTask",				//任务名称
                (u16 ) dispTASK_STK_SIZE,				//任务堆栈大小
                (void* )NULL,							//传递给任务函数的参数
                (UBaseType_t ) dispTASK_PRIO,           //任务优先级
                (TaskHandle_t*)&tDispTaskHandler);      //任务句柄
	#endif  //boardUSE_OS
	
	return true;
}

/***********************************************************************************************************************
-----函数功能    tDisp显示任务
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
void vDisp_Task(void *pvParameters)
{
	static bool flag = false;
	static DevState_E next_state;
	
	#if(boardUSE_OS)
	for(;;)
	#endif  //boardUSE_OS
	{
		if(next_state != tSysInfo.eDevState)
		{
			flag = false;
			next_state = tSysInfo.eDevState;
		}
		
		switch(tSysInfo.eDevState)
		{
			//--------------------------初始化----------------------------------
			case DS_INIT:
			{	
				if(flag ==false)
				{
					flag = true;
					v_disp_init();
				}
			}
			break;
			
			//---------------------------关闭中---------------------------------
			case DS_CLOSING:
			{
//				if(flag ==false)
				{
					flag = true;
					v_disp_closing();
				}
			}
			break;
			
			//---------------------------关闭----------------------------------
			case DS_SHUT_DOWN:
			{
				if(bKey_PowerIsPress() == false)
					v_disp_shut_down();
			}
			break;
			//----------------------------装载中---------------------------------
			case DS_BOOTING:
			{
				v_disp_booting();
			}
			break;
			
			//----------------------------工作中---------------------------------
			//---------------------------错误------------------------------------
			case DS_ERR: 
			case DS_WORK:
			{
				if(flag ==false)
				{
					flag = true;
				}
				v_disp_work();
			}
			break;
			
			//----------------------------升级模式---------------------------------
			case DS_UPDATA_MODE:
			{
				#if(boardUPDATA)
				bDisp_Switch(ST_ON, true);
				
				Display_IconUpdata();
				
				Display_ShowErrCode(tBootMemParam.tParam.eAppState);
				Display_UpdataState(2, tUpdata.eProtoType, 0);
				Display_UpdataProgress(tUpdata.usRecFrameCnt, tUpdata.usTotalFrmValue);
				Display_UpdataTime(tUpdata.usRecOverTimeCnt/10);
				Display_RefreshData();            //发送数据    
				#endif  //boardUPDATA     
			}
			break;
			
			#if(boardENG_MODE_EN)
			//----------------------------工程模式---------------------------------
			case DS_ENG_MODE:
			{
				bDisp_Switch(ST_ON, true);
				
				vDisp_EnginModeDis();
				vTaskDelay(200);
			}
			break;
			#endif	
				
			default:
				vTaskDelay(boardDISP_REFRESH_TMIE);
				break;
		}
		
		#if(boardUSE_OS)
		vTaskDelay(boardDISP_REFRESH_TMIE);
		#endif  //boardUSE_OS
	}
}

/***********************************************************************************************************************
-----函数功能    初始化显示
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
__STATIC_INLINE void v_disp_init(void)
{
	bDisp_Switch(ST_OFF, false);
	Display_RefreshData();      //发送数据
}


/***********************************************************************************************************************
-----函数功能    关闭中显示
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
__STATIC_INLINE void v_disp_closing(void)
{
	bDisp_Switch(ST_ON, false);
	Display_ShowOFF();
	Display_RefreshData();      //发送数据
}

/***********************************************************************************************************************
-----函数功能    关闭显示
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
__STATIC_INLINE void v_disp_shut_down(void)
{
	bDisp_Switch(ST_OFF, false);
	Display_RefreshData();            //发送数据
}

/***********************************************************************************************************************
-----函数功能    开启显示
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
__STATIC_INLINE void v_disp_booting(void)
{
	static u8 j=1;
	
	bDisp_Switch(ST_ON, false);
	
	Display_InPwr(tAppMemParam.tDCAC.usInPwrRating);
	Display_OutPwr(tAppMemParam.tDCAC.usOutPwrRating);
	Display_TimRoll(1);//TIM时间滚动
	
	DisplayNum2(0,10);//TIM --
	DisplayNum2(1,10);//TIM --
	Y_Display(11);//M
	

	Display_BAT(0,false,0);//BAT等级界面	
    T_Display(j);
	
	DisplayNum1(10,10);          //SOC界面--
	DisplayNum1(11,10);	         //SOC界面--
	

    j++;if(j>10)j=1;
	Display_RefreshData();            //发送数据
}

/***********************************************************************************************************************
-----函数功能    LCD工作显示函数
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
__STATIC_INLINE void v_disp_work(void)
{
	vu8 uc_soc = 0;
	vu16 us_err_code = 0;
	static bool b_dis_twintle_flag;   //显示闪烁标记

	#if(boardBMS_EN)
	uc_soc = ucBms_GetSoc();
	#endif  //boardBMS_EN
	
	//息屏
	if(tDisp.bLight == false) 
		return;
	
	//闪烁
	if(b_dis_twintle_flag)
		b_dis_twintle_flag = false;
	else 
		b_dis_twintle_flag = true;
				
	
	Display_ClearData();   //Buff清零,避免残留
	
	//------------------------------------数据显示-----------------------------------------
	//时间
	if(bSys_IsChgState() == true)
	{
		if(uc_soc <= 90)
		{
			Display_BatChgRoll(uc_soc);//BAT充电滚动等级界面
			Display_BAT(0, false, uc_soc);
		}
		else
			Display_BAT(bSys_IsChgState(), false, uc_soc);
	}
	else
		Display_BAT(bSys_IsChgState(), true, uc_soc);

	//SOC
	Display_Soc(uc_soc);
	
	//功率
	Display_OutPwr(tSysInfo.usOutPwr);
	Display_InPwr(tSysInfo.usInPwr);
	
	//错误代码
	us_err_code = usDisp_ErrCodeDisplay();
	if(us_err_code > 0 && us_err_code < 100)
	{
		Display_ShowErrCode(us_err_code);
		Display_IconSysErr();
	}
	else
	{
		//时间
		#if(boardBMS_EN)
		if(bSys_IsChgState() == true)
		{
			if(tSysInfo.usOutPwr)
			{
				Display_Time(0,TIM_MAX);//99+
				if(uc_soc != 100)
					Display_TimRoll(1);//长时间滚动
				else 
					Display_ShowChgFullTime();//充满显示
			}
			else 
			{
				if(uc_soc != 100)
				{
					//默认<100,时间都一直显示1分钟
					if(tBmsRx.tParam.usChgFullTime==0)
					   Display_Time(0,1);//充满时间
					else
						Display_Time(0,tBmsRx.tParam.usChgFullTime);//充满时间

					Display_TimRoll(1);//时间滚动
				}
				else 
				{
					Display_Time(9,0);//99+
					Display_ShowChgFullTime();//充满显示
				}
			}
		}
		else
		{
			if(tSysInfo.usOutPwr == 0 && uc_soc!= 0)
				Display_Time(9,TIM_MAX);//剩余时间>99H+
			else
				Display_Time(9,tBmsRx.tParam.usDisChgEmptyTime);//剩余时间
		}
		#endif  //boardBMS_EN
	}
		
	//------------------------------------错误显示-----------------------------------------
	//输出过温标识
	if(tSysInfo.uErrCode.tCode.bOT && b_dis_twintle_flag)
	{
		Display_IconOT();
		Display_IconOutOT();
	}
	
	//输出低温标识
	if(tSysInfo.uErrCode.tCode.bUT && b_dis_twintle_flag)
		Display_IconUT();
	
	//过载
	if(tSysInfo.uErrCode.tCode.bOL == 1)
		Display_IconOL();

	//电池存在故障
	#if(boardBMS_EN)
	if(tBms.uErrCode.ulCode)
		Display_IconBatErr();
	#endif  //boardBMS_EN

	//电池存在不许可
	#if(boardBMS_EN)
	if(tBms.uPerm.tPerm.bDisChgPerm == 0 || 
		tBms.uPerm.tPerm.bChgPerm == 0)
		Display_IconBatLock();
	#endif  //boardBMS_EN
	
	//------------------------------------设备状态显示-----------------------------------------
	// 蜂鸣器
	#if(boardBUZ_EN)
	// if(tAppMemParam.tSYS.bBuzSwitchOff == 0)
	// 	Display_IconBuz();
	#endif  //boardBUZ_EN
	
	//散热开启
	// #if(boardHEAT_MANAGE_EN)
	// if(eFan_GetWorkMode() > FWM_OFF)
	// 	Display_IconFan();
	// #endif  //boardHEAT_MANAGE_EN
	
	//照明状态显示
	#if(boardLIGHT_EN)
	if(tLight.eDevState == DS_WORK)
		Display_IconLight();
	#endif  //boardLIGHT_EN
	
	//USB状态显示
	#if(boardUSB_EN)
	if(tUsb.eDevState >= DS_BOOTING)
		Display_IconUsbOut();
	else if(tUsb.eDevState >= DS_ERR && b_dis_twintle_flag)
		Display_IconUsbOut();
	#endif  //boardUSB_EN
	
	//DC状态显示
	#if(boardDC_EN)
	if(tDc.eDevState >= DS_BOOTING)
		Display_IconDcOut();
	else if(tDc.eDevState >= DS_ERR && b_dis_twintle_flag)
		Display_IconDcOut();
	#endif  //boardDC_EN
	
	//AC输出状态显示
	#if(boardDCAC_EN)
	if(tDcac.eDisChgState >= IOS_STARTING)
		Display_IconAcOut();
	else if(tDcac.eDisChgState == IOS_ERR && b_dis_twintle_flag)
		Display_IconAcOut();
	#endif  //boardDCAC_EN
		
	//AC输入状态显示
	#if(boardDCAC_EN)
	if(tDcac.eChgState >= IOS_STARTING) //常亮
		Display_IconAcIn();
	else if(tDcac.eChgState == IOS_PROTE && 
		    tDcac.eChgState == IOS_ERR   &&
		    b_dis_twintle_flag)
	{
		Display_IconAcIn();
	}
	#endif  //boardDCAC_EN

	//MPPT输入状态显示
	#if(boardMPPT_EN)
	if(tMppt.eDevState >= DS_BOOTING)
		Display_IconDcIn();
	else if((tMppt.eDevState == DS_ERR) && b_dis_twintle_flag)
	{
		Display_IconDcIn();
	}
	#endif  //boardMPPT_EN
	
	//并网
	#if(boardDCAC_PARA_IN && boardDCAC_EN)
	if(tDcac.eParanInState >= IOS_STARTING)
	{
		Display_Save();
		if(tMppt.eDevState >= DS_BOOTING)
			Display_IconParaIn();
	}
	#endif  //boardDCAC_PARA_IN

	//固定显示内容
	Display_ForeverShow();
	
	// //测试模式
	// if(G_TestMode == true)
	// 	Display_Eco();

	Display_RefreshData();
}


/***********************************************************************************************************************
-----函数功能    显示开关
-----说明(备注)  none
-----传入参数    type:类型   fore_en:强制打开
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
bool bDisp_Switch(SwitchType_E type, bool fore_en)
{
	Display_ClearData();   //Buff清零,避免残留
	
	switch(type)
	{
		case ST_ON:
			goto LoopOn;
		
		case ST_OFF:
			goto LoopOff;
		
		default:
		{
			if(tDisp.bLight == false)
			{
				LoopOn:
				dispLIGHT_POWER_ON();
				tDisp.bLight = true;
				
				//关闭息屏
				if(fore_en == true)
					tDisp.usAutoOffTime = 0;
				
				//更新显示时间
				if(tDisp.usAutoOffTime)
					tDisp.usAutoOffCnt =  tDisp.usAutoOffTime;
			}
			else 
			{
				LoopOff:
				dispLIGHT_POWER_OFF();
				v_disp_param_init();
				Display_RefreshData(); //发送数据
				tDisp.bLight = false;
			}
		}
		break;
	}
	
	return true;
}

/***********************************************************************************************************************
-----函数功能    背光自动关闭计时
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
void vDisp_TickTimer(void) 
{
	//非工作状态下退出
	if(tSysInfo.eDevState != DS_WORK) 
		return;
	
	//非亮屏幕状态
	if(tDisp.bLight == false)   
		return;
	
	//-----自动关闭背光--------------------------------------   
	if(tDisp.usAutoOffTime)
	{
		if(tDisp.usAutoOffCnt)
		{
			tDisp.usAutoOffCnt--;
			if(tDisp.usAutoOffCnt == 0)
			{
				v_disp_shut_down();
				if(uPrint.tFlag.bDispTask|| uPrint.tFlag.bImportant)
					sMyPrint("Lcd_Task:倒计时结束,进入息屏 时间 = %dS\r\n",tDisp.usAutoOffTime);
			}
		}
	}
}

/*****************************************************************************************************************
-----函数功能    初始化参数
-----说明(备注)  none
-----传入参数    p_disp_mem : disp记忆参数结构体
-----输出参数    none
-----返回值      true:设置成功  反之失败
*****************************************************************************************************************/
bool bDisp_MemParamInit(DispMemParam_T* p_disp_mem)
{
	p_disp_mem->ucHighLightValue = boardDISP_HIGH_LIGHT_VALUE;
	p_disp_mem->ucLowLightValue = boardDISP_LOW_LIGHT_VALUE;
	p_disp_mem->usAutoOffTime = boardDISP_OFF_TIME;
	return true;
}

#if(boardLOW_POWER)
/*****************************************************************************************************************
-----函数功能    检查系统的输入电源:外接电池
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
*****************************************************************************************************************/
void v_dis_power_select( void )
{
	if(tDisp.bLight)
	{
		/*************************************电源有输入*********************************************************/
		if( tAdcSamp.usBMS_Vin >= boardBMS_MIN_VOLT )   
		{
			Disp_EN_OFF();          //关闭显示屏的电池供电
		}
		/*************************************没有电源输入*******************************************************/
		else
		{
			Disp_EN_ON();          //打开显示屏的电池供电
		}	
	}
	else 
	{
		Disp_EN_OFF();          //关闭显示屏的电池供电
	}
}


/*****************************************************************************************************************
-----函数功能    进入低功耗
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
*****************************************************************************************************************/
void vLcd_EnterLowPower(void)
{
//	vLcd_IoEnterLowPower();
//	bAtti_EnterLowPower();
//	vExRTC_EnterLowPower();
	vTaskSuspend(tDispTaskHandler);
}

/*****************************************************************************************************************
-----函数功能    退出低功耗
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
*****************************************************************************************************************/
void vLcd_ExitLowPower(void)
{
//	vExRTC_ExitLowPower();
	vTaskResume(tDispTaskHandler);
}
#endif //boardLOW_POWER

#endif //boardDISPLAY_EN



