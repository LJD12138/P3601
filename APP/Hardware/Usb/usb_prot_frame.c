
#include "Usb/usb_prot_frame.h"

#if(boardUSB_EN)
#include "Usb/usb_queue_task.h"
#include "Usb/usb_task.h"
#include "Usb/usb_iface.h"
#if(boardPRINT_IFACE)
#include "Print/print_task.h"
#endif  //boardPRINT_IFACE

// #include "check.h"
#include "filtration.h"
#include "math.h"

// #define       	usbDEV_ADRR                          	0x01
// #define  		usbWAIT_NOTIFY_OUTTIME              	1000     //任务通知超时时间 MS
// #define       	usbTX_PROTO_BUFF_LEN                   	128
// #define       	usbRX_PROTO_BUFF_LEN                   	256

//*********************************寄存器地址********************************
#define     	SW3516_SYS_STATE1_ADDR        			0x08//系统状态1
#define     	SW3516_VOUT_ADDR              			0x31//VOUT
#define     	SW3516_IOUT1_ADDR             			0x33//IOUT1
#define     	SW3516_ADC_CFG_ADDR           			0x3A//ADC配置
#define     	SW3516_ADC_DATA_H_ADDR        			0x3B//ADC-DATA
#define     	SW3516_ADC_DATA_L_ADDR        			0x3C//ADC-DATA

//****************************************************参数初始化**************************************************//
// __ALIGNED(4) 	ModbusProtoTx_t *tpUsbProtoTx = NULL;	//发送协议
// __ALIGNED(4) 	ModbusProtoRx_t *tpUsbProtoRx = NULL;	//接受协议
#pragma pack(1)
typedef struct
{
	vs8            		cTemp;              //温度
    vs16           		sPower;             //1W 总功率 
    vu16           		usVolt;             //mV
	vu16           		usCurr;       		//mA
    vu16           		usPdCurr;       	//mA
	vu16           		usQcCurr;       	//mA
	vu16           		usPdPwr;        	//1W
    vu16           		usQcPwr;        	//1W
}USB_IC_T; 
#pragma pack()

//PD100W温度滤波器
#define 		usbPD_TEMP_FILTER_BUFF_SIZE     		10 
static s32 usa_pd_temp_buff[usbPD_TEMP_FILTER_BUFF_SIZE];
FilterHandler_T    tAdc_PDTempFilterMadAvg = {usa_pd_temp_buff, usbPD_TEMP_FILTER_BUFF_SIZE, 0, 0, 0, 0, 0};

//****************************************************函数声明****************************************************//
// static s8 c_usb_data_trans(u8 cmd, u16 reg_addr, u8* data, u8 len);



/***********************************************************************************************************************
-----函数功能    通讯协议初始化
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
************************************************************************************************************************/
bool bUsb_SendProtInit(void)
{
	// s8 c_result = 1;

	// c_result = cModbus_TransProtoInit(&tpUsbProtoTx, usbTX_PROTO_BUFF_LEN, usbDEV_ADRR);
	// if(c_result <= 0)
	// {
	// 	if(uPrint.tFlag.bUsbTask || uPrint.tFlag.bImportant)
	// 		log_e("bUsbTask:tpUsbProtoTx协议对象初始化失败,代码%d",c_result);
		
	// 	return false;
	// }
	
	return true;
}

bool bUsb_RecProtInit(void)
{
	// s8 c_result = cModbus_RecProtoInit(&tpUsbProtoRx, 	//协议指针
	// 							usbRX_PROTO_BUFF_LEN,	//协议缓存器大小
	// 							usbDEV_ADRR,			//协议设备ID
	// 							boardREPET_TIMER_CYCLE_TMIE);			//计数器采样时间
	// if(c_result <= 0)
	// {
	// 	if(uPrint.tFlag.bUsbRecTask || uPrint.tFlag.bImportant)
	// 		log_e("bUsbRecTask:tpUsbProtoRx协议对象初始化失败,代码%d",c_result);
	// 	return false;
	// }
	
	return true;
}

/*****************************************************************************************************************
-----函数功能    指令:初始化IC
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
******************************************************************************************************************/
s8 c_usb_cs_ic1_init(void)
{
	// u8 data[1] = {0};
	// static u8 index = 0;
	// static u8 uc_lost_cnt = 0;
	
	// switch(index)
	// {
	// 	case 0:
	// 	{
	// 		data[0] = 0x7F;
	// 		if(cI2C_WriteBytes(&tUSB_IC1_I2C, SW3516_REG1_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}

	// 	case 1:
	// 	{
	// 		data[0] = 0x06;
	// 		if(cI2C_WriteBytes(&tUSB_IC1_I2C, 0x03, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}
		
	// 	case 2:
	// 	{
	// 		data[0] = 0xA0;
	// 		if(cI2C_WriteBytes(&tUSB_IC1_I2C, SW3516_ADC_EN_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}

	// 	case 3:
	// 	{
	// 		// data[0] = 0xFF;
	// 		// if(cI2C_WriteBytes(&tUSB_IC1_I2C, SW3516_PD1_ADDR, data, sizeof(data)) <= 0)
	// 		// {
	// 		// 	if(uc_lost_cnt < 0xff) 
	// 		// 		uc_lost_cnt++;
	// 		// 	break;
	// 		// }
	// 		// else
	// 		{
	// 			data[0] = 0x48;
	// 			cI2C_WriteBytes(&tUSB_IC1_I2C, 0x30, data, sizeof(data));

	// 			data[0] = 0xFF;
	// 			cI2C_WriteBytes(&tUSB_IC1_I2C, 0x31, data, sizeof(data));

	// 			data[0] = 0x80;
	// 			cI2C_WriteBytes(&tUSB_IC1_I2C, 0x4A, data, sizeof(data));

	// 			data[0] = 0x40;
	// 			cI2C_WriteBytes(&tUSB_IC1_I2C, 0x70, data, sizeof(data));

	// 			data[0] = 0x8C;
	// 			cI2C_WriteBytes(&tUSB_IC1_I2C, 0x04, data, sizeof(data));

	// 			// data[0] = 0xFF;
	// 			// cI2C_WriteBytes(&tUSB_IC1_I2C, 0x31, data, sizeof(data));

	// 			// data[0] = 0x40;
	// 			// cI2C_WriteBytes(&tUSB_IC1_I2C, 0x70, data, sizeof(data));

	// 			// data[0] = (u8)(280/2);
	// 			// cI2C_WriteBytes(&tUSB_IC1_I2C, 0x46, data, sizeof(data));

	// 			// data[0] = 0x64;
	// 			// cI2C_WriteBytes(&tUSB_IC1_I2C, 0x47, data, sizeof(data));

	// 			// data[0] = 0xFF;
	// 			// cI2C_WriteBytes(&tUSB_IC1_I2C, 0x33, data, sizeof(data));

	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}
		
	// 	case 4:
	// 	{
	// 		index = 0;
			return 1;
	// 	}
		
	// 	default:
	// 		index = 0;
	// 	break;
	// }

	// if(uc_lost_cnt >= 10)
	// {
	// 	if(tUsb.uErrCode.tCode.bIc1Lost == 0)
	// 	{
	// 		bUsb_SetErrCode(UEC_IC1_LOST,true);
		
	// 		if(uPrint.tFlag.bUsbTask || uPrint.tFlag.bImportant)
	// 			log_e("bUsbTask:IC1丢失");
	// 	}
	// 	return -1;
	// }
	// else 
	// {
	// 	if(tUsb.uErrCode.tCode.bIc1Lost == 1)
	// 		bUsb_SetErrCode(UEC_IC1_LOST,false);
	// 	return 0;
	// }
}

/*****************************************************************************************************************
-----函数功能    指令:初始化IC
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
******************************************************************************************************************/
s8 c_usb_cs_ic2_init(void)
{
	// u8 data[1] = {0};
	// static u8 index = 0;
	// static u8 uc_lost_cnt = 0;
	
	// switch(index)
	// {
	// 	case 0:
	// 	{
	// 		data[0] = 0x7F;
	// 		if(cI2C_WriteBytes(&tUSB_IC2_I2C, SW3516_REG1_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}

	// 	case 1:
	// 	{
	// 		data[0] = 0x06;
	// 		if(cI2C_WriteBytes(&tUSB_IC2_I2C, 0x03, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}
		
	// 	case 2:
	// 	{
	// 		data[0] = 0xA0;
	// 		if(cI2C_WriteBytes(&tUSB_IC2_I2C, SW3516_ADC_EN_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}

	// 	case 3:
	// 	{
	// 		// data[0] = 0xFF;
	// 		// if(cI2C_WriteBytes(&tUSB_IC2_I2C, SW3516_PD1_ADDR, data, sizeof(data)) <= 0)
	// 		// {
	// 		// 	if(uc_lost_cnt < 0xff) 
	// 		// 		uc_lost_cnt++;
	// 		// 	break;
	// 		// }
	// 		// else
	// 		{
	// 			data[0] = 0x48;
	// 			cI2C_WriteBytes(&tUSB_IC2_I2C, 0x30, data, sizeof(data));

	// 			data[0] = 0xFF;
	// 			cI2C_WriteBytes(&tUSB_IC2_I2C, 0x31, data, sizeof(data));

	// 			data[0] = 0x80;
	// 			cI2C_WriteBytes(&tUSB_IC2_I2C, 0x4A, data, sizeof(data));

	// 			data[0] = 0x40;
	// 			cI2C_WriteBytes(&tUSB_IC2_I2C, 0x70, data, sizeof(data));

	// 			data[0] = 0x8C;
	// 			cI2C_WriteBytes(&tUSB_IC2_I2C, 0x04, data, sizeof(data));

	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}
		
	// 	case 4:
	// 	{
	// 		index = 0;
			return 1;
	// 	}
		
	// 	default:
	// 		index = 0;
	// 	break;
	// }

	// if(uc_lost_cnt >= 10)
	// {
	// 	if(tUsb.uErrCode.tCode.bIc2Lost == 0)
	// 	{
	// 		bUsb_SetErrCode(UEC_IC2_LOST,true);
		
	// 		if(uPrint.tFlag.bUsbTask || uPrint.tFlag.bImportant)
	// 			log_e("bUsbTask:IC2丢失");
	// 	}
		
	// 	return -1;
	// }
	// else 
	// {
	// 	if(tUsb.uErrCode.tCode.bIc2Lost == 1)
	// 		bUsb_SetErrCode(UEC_IC2_LOST,false);
		
	// 	return 0;
	// }
}
USB_IC_T tUsbIc1 = {0}; //PD100W芯片
/*****************************************************************************************************************
-----函数功能    指令:获取参数
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
******************************************************************************************************************/
s8 c_usb_cs_get_ic1_param(void)
{
	static u8 index;
	u8 buff[6] = {0};
	u8 data[1] = {0};
	
	static u8 uc_lost_cnt = 0;

	switch (index)
	{
		//获取状态
		case 0:
		{
			if(cI2C_ReadBytes(&tUSB_IC1_I2C, SW3516_SYS_STATE1_ADDR, data, sizeof(data)) <= 0)
			{
				if(uc_lost_cnt < 0xff) 
					uc_lost_cnt++;
				break;
			}
			else
			{
				uc_lost_cnt = 0;
				index++;
			}
		}
		
		//获取参数
		case 1:
		{
			memset(&data, 0, sizeof(data));
			if(cI2C_ReadBytes(&tUSB_IC1_I2C, SW3516_VOUT_ADDR, data, sizeof(data)) <= 0)
			{
				if(uc_lost_cnt < 0xff) uc_lost_cnt++;
				break;
			}
			else 
				uc_lost_cnt = 0;
			buff[0] = data[0];
			
			memset(&data, 0, sizeof(data));
			if(cI2C_ReadBytes(&tUSB_IC1_I2C, SW3516_IOUT1_ADDR, data, sizeof(data)) <= 0)
			{
				if(uc_lost_cnt < 0xff) uc_lost_cnt++;
				break;
			}
			else 
				uc_lost_cnt = 0;
			buff[1] = data[0];
			
			memset(&data, 0, sizeof(data));
			data[0] = 0x06;
			if(cI2C_WriteBytes(&tUSB_IC1_I2C, SW3516_ADC_CFG_ADDR, data, sizeof(data)) <= 0)
			{
				if(uc_lost_cnt < 0xff) uc_lost_cnt++;
				break;
			}
			else 
				uc_lost_cnt = 0;
			
			memset(&data, 0, sizeof(data));
			if(cI2C_ReadBytes(&tUSB_IC1_I2C, SW3516_ADC_DATA_H_ADDR, data, sizeof(data)) <= 0)
			{
				if(uc_lost_cnt < 0xff) uc_lost_cnt++;
				break;
			}
			else 
				uc_lost_cnt = 0;
			buff[2] = data[0];
			
			memset(&data, 0, sizeof(data));
			if(cI2C_ReadBytes(&tUSB_IC1_I2C, SW3516_ADC_DATA_L_ADDR, data, sizeof(data)) <= 0)
			{
				if(uc_lost_cnt < 0xff) uc_lost_cnt++;
				break;
			}
			else 
				uc_lost_cnt = 0;
			buff[3] = data[0];
			
			tUsbIc1.usVolt = buff[0] * 96;//mv
			tUsbIc1.usCurr = buff[1] * 40;//ma

			s16 s_temp = (buff[3] & 0x0f) | (buff[2] << 4);



			// tUsbIc1.usVolt = (buff[0] << 8) | buff[1];    //mV
			// if(tUsbIc1.usVolt >= 100)
			// 	tUsbIc1.usVolt -= 100;
			
			// tUsbIc1.usPdCurr = (buff[2] << 8) | buff[3];    //mA
			// if(tUsbIc1.usPdCurr >= 255)
			// 	tUsbIc1.usPdCurr -= 255;
			
			// tUsbIc1.usQcCurr = (buff[4] << 8) | buff[5];    //mA
			// if(tUsbIc1.usQcCurr >= 255)
			// 	tUsbIc1.usQcCurr -= 255;
			
			//***************************************处理数据******************************************
			// tUsbIc1.usPdPwr = (tUsbIc1.usPdCurr / 1000.0f) * (tUsbIc1.usVolt / 1000.0f);
			// tUsbIc1.usQcPwr = (tUsbIc1.usQcCurr / 1000.0f) * (tUsbIc1.usVolt / 1000.0f);
			// tUsbIc1.sPower = tUsbIc1.usPdPwr + tUsbIc1.usQcPwr;
			tUsbIc1.sPower = (tUsbIc1.usCurr / 1000.0f) * (tUsbIc1.usVolt / 1000.0f);
			us_usb_total_out_pwr += tUsbIc1.sPower;
			
			s_temp = lFilter_MadianAverage(&tAdc_PDTempFilterMadAvg, (s32*)&s_temp);
			//避免没通讯上
			if(s_temp > 100)  
				tUsbIc1.cTemp = LIMIT((307 - (37 * log((float)s_temp))), -128, 127) / 2;

			index = 0;
			
			if(uPrint.tFlag.bUsbTask)
			{
				sMyPrint("bUsbTask:IC1电压 = %dmV, 功率 = %dW \r\n",tUsbIc1.usVolt,tUsbIc1.sPower);
				sMyPrint("USB_Task:IC1温度 = %d摄氏度", tUsbIc1.cTemp);
			}
		}
		break;

		default:
			index = 0;
			break ;
	}

	//处理状态
	if(uc_lost_cnt >= 16)  //标志
	{
		if(tUsb.uErrCode.tCode.bIc1Lost == 0)
		{
			bUsb_SetErrCode(UEC_IC1_LOST, true);
		
			if(uPrint.tFlag.bUsbTask || uPrint.tFlag.bImportant)
				log_e("bUsbTask:IC1丢失");
		}
		return -1;
	}
	else if(!uc_lost_cnt)  //清除
	{
		if(tUsb.uErrCode.tCode.bIc1Lost == 1)
			bUsb_SetErrCode(UEC_IC1_LOST, false);
		return 1;
	}
	else
		return 0;
}

USB_IC_T tUsbIc2 = {0}; //无线充芯片
/*****************************************************************************************************************
-----函数功能    指令:获取参数
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
******************************************************************************************************************/
s8 c_usb_cs_get_ic2_param(void)
{
	// static u8 index;
	// u8 buff[6] = {0};
	// u8 data[3] = {0};
	
	// static u8 uc_lost_cnt = 0;

	// switch (index)
	// {
	// 	case 0:
	// 	{
	// 		data[0] = 0xA0;
	// 		if(cI2C_WriteBytes(&tUSB_IC2_I2C, SW3516_ADC_EN_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) 
	// 				uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			uc_lost_cnt = 0;
	// 			index++;
	// 		}
	// 	}
		
	// 	//获取功率
	// 	case 1:
	// 	{
	// 		//******************************获取参数*********************************
	// 		memset(&data, 0, sizeof(data));
	// 		if(cI2C_ReadBytes(&tUSB_IC2_I2C, SW3516_VOUT1_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else 
	// 			uc_lost_cnt = 0;
	// 		buff[0] = data[1];
			
	// 		memset(&data, 0, sizeof(data));
	// 		if(cI2C_ReadBytes(&tUSB_IC2_I2C, SW3516_VOUT2_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else 
	// 			uc_lost_cnt = 0;
	// 		buff[1] = data[1];

	// 		memset(&data, 0, sizeof(data));
	// 		if(cI2C_ReadBytes(&tUSB_IC2_I2C, SW3516_PD_IOUT1_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else 
	// 			uc_lost_cnt = 0;
	// 		buff[2] = data[1];
			
	// 		memset(&data, 0, sizeof(data));
	// 		if(cI2C_ReadBytes(&tUSB_IC2_I2C, SW3516_PD_IOUT2_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else 
	// 			uc_lost_cnt = 0;
	// 		buff[3] = data[1];
			
	// 		memset(&data, 0, sizeof(data));
	// 		if(cI2C_ReadBytes(&tUSB_IC2_I2C, SW3516_QC_IOUT1_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else 
	// 			uc_lost_cnt = 0;
	// 		buff[4] = data[1];
			
	// 		memset(&data, 0, sizeof(data));
	// 		if(cI2C_ReadBytes(&tUSB_IC2_I2C, SW3516_QC_IOUT2_ADDR, data, sizeof(data)) <= 0)
	// 		{
	// 			if(uc_lost_cnt < 0xff) uc_lost_cnt++;
	// 			break;
	// 		}
	// 		else 
	// 			uc_lost_cnt = 0;
	// 		buff[5] = data[1];
			
	// 		tUsbIc2.usVolt = (buff[0] << 8) | buff[1];    //mV
	// 		if(tUsbIc2.usVolt >= 100)
	// 			tUsbIc2.usVolt -= 100;
			
	// 		tUsbIc2.usPdCurr = (buff[2] << 8) | buff[3];    //mA
	// 		if(tUsbIc2.usPdCurr >= 255)
	// 			tUsbIc2.usPdCurr -= 255;
			
	// 		tUsbIc2.usQcCurr = (buff[4] << 8) | buff[5];    //mA
	// 		if(tUsbIc2.usQcCurr >= 255)
	// 			tUsbIc2.usQcCurr -= 255;
			
	// 		//***************************************处理参数******************************************
	// 		tUsbIc2.usPdPwr = (tUsbIc2.usPdCurr / 1000.0f) * (tUsbIc2.usVolt / 1000.0f);
	// 		tUsbIc2.usQcPwr = (tUsbIc2.usQcCurr / 1000.0f) * (tUsbIc2.usVolt / 1000.0f);
	// 		tUsbIc2.sPower = tUsbIc2.usPdPwr + tUsbIc2.usQcPwr;
	// 		us_usb_total_out_pwr += tUsbIc2.sPower;
			
	// 		index = 0;
			
	// 		if(uPrint.tFlag.bUsbTask)
	// 			sMyPrint("bUsbTask:SW3518电压 = %dmV, 功率 = %dW \r\n", tUsbIc2.usVolt, tUsbIc2.sPower);
	// 	}
	// 	break;

	// 	default:
	// 		index = 0;
	// 		break ;
	// }
	
	// //处理状态
	// if(uc_lost_cnt >= 16)  //标志
	// {
	// 	if(tUsb.uErrCode.tCode.bIc2Lost == 0)
	// 	{
	// 		bUsb_SetErrCode(UEC_IC2_LOST,true);
		
	// 		if(uPrint.tFlag.bUsbTask || uPrint.tFlag.bImportant)
	// 			log_e("bUsbTask:IC2丢失");	
	// 	}
	// 	return -1;
	// }
	// else if(!uc_lost_cnt)  //清除
	// {
	// 	if(tUsb.uErrCode.tCode.bIc2Lost == 1)
	// 		bUsb_SetErrCode(UEC_IC2_LOST,false);
		return 1;
	// }
	// else
	// 	return 0;
}

/*****************************************************************************************************************
-----函数功能    指令:
-----说明(备注)  none
-----传入参数    none
-----输出参数    none
-----返回值      none
******************************************************************************************************************/
s8 c_usb_set_pwr_cs(u16 pwr)
{
	// if(c_usb_data_trans(modbusWRITE_SINGLE_REG, 
	// 					usbREG_ADDR_SET_CHG_PWR, 
	// 					(u8*)&pwr, 
	// 					1) <= 0)
	// 	return false;
	
	return 1;
}

/***********************************************************************************************************************
-----函数功能	数据传输
-----说明(备注) 
-----传入参数	cmd:指令
				data:指向数据指针
				len:数据的长度
-----输出参数	none
-----返回值		-1:写入的Len超出最大长度
				-2:等会回复超时
				-3:数据发送错误
				0:无操作
				1:操作成功
************************************************************************************************************************/
// static s8 c_usb_data_trans(u8 cmd, u16 reg_addr, u8* data, u8 len)
// {
// 	s8 result = 0;
	
// 	if(dcacSemaphoreMutex == NULL)
// 		return 0;
	
// 	if(tpProtoTx == NULL)
// 		return 0;
	
// 	//开始互斥
// 	#if(boardUSE_OS)
// 	if(xSemaphoreTake(dcacSemaphoreMutex, pdMS_TO_TICKS(1000)) == pdFAIL)
// 		return -99;
// 	#endif  //boardUSE_OS
	
// 	//开始发送
// 	#if(boardDCAC_EN)
// 	result = cModbus_ProtoCreate(tpProtoTx, cmd, reg_addr, data, len);
// 	if(result > 0)
// 	{
// 		if(bDcac_DataSendStart(tpProtoTx->ucaFrameData, tpProtoTx->ucFrameLen) == true)
// 		{
// 			//等待任务通知,等待时间为1S
// 			#if(boardUSE_OS)
// 			if(ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(usbWAIT_NOTIFY_OUTTIME)) <= 0) 
// 			{
// 				if(uPrint.tFlag.bUsbTask)
// 					log_w("bUsbTask:等待指令0x%x,地址0x%x回复超时", cmd, reg_addr);
				
// 				result = -2;
// 			}
// 			#endif  //boardUSE_OS
// 		}
// 		else 
// 			result = -3;
// 	}
// 	#endif
	
// 	cModbus_ResetTx(tpUsbProtoTx, usbTX_PROTO_BUFF_LEN);
	
// 	//释放互斥量
// 	#if(boardUSE_OS)
// 	xSemaphoreGive(dcacSemaphoreMutex);
// 	#endif  //boardUSE_OS
	
// 	return result;
// }

#endif  //boardUSB_EN
