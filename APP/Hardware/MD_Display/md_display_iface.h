#ifndef __HT1621_H
#define __HT1621_H

#include "board_config.h"

#if(boardDISPLAY_EN)
#include "i2c.h"


#define   LCD_RCCA          RCU_GPIOA
#define   LCD_RCCB          RCU_GPIOB
#define   LCD_RCCC          RCU_GPIOC
         
#define   LCD_PORTA          GPIOA
#define   LCD_PORTB          GPIOB
#define   LCD_PORTC          GPIOC

#define         dispLIGHT_POWER_PORT	                GPIOB
#define         dispLIGHT_POWER_PIN		                GPIO_PIN_4
#define     	dispLIGHT_POWER_ON()    				GPIO_BOP(dispLIGHT_POWER_PORT) = dispLIGHT_POWER_PIN
#define     	dispLIGHT_POWER_OFF()    				GPIO_BC(dispLIGHT_POWER_PORT)  = dispLIGHT_POWER_PIN


#define   LCD_CS1_PORT	    GPIOB
#define   LCD_CS1_PIN		BIT(9)
#define   CS1_H             (GPIO_BOP(LCD_CS1_PORT)=LCD_CS1_PIN)
#define   CS1_L             (GPIO_BC(LCD_CS1_PORT)=LCD_CS1_PIN)


#define   LCD_CS2_PORT	    GPIOB
#define   LCD_CS2_PIN		BIT(7)
#define   CS2_H             (GPIO_BOP(LCD_CS2_PORT)=LCD_CS2_PIN)
#define   CS2_L             (GPIO_BC(LCD_CS2_PORT)=LCD_CS2_PIN)


#define   LCD_WR_PORT	    GPIOB
#define   LCD_WR_PIN		BIT(6)
#define   WR_H              (GPIO_BOP(LCD_WR_PORT)=LCD_WR_PIN)
#define   WR_L              (GPIO_BC(LCD_WR_PORT)=LCD_WR_PIN)


#define   LCD_DATA_PORT	    GPIOB
#define   LCD_DATA_PIN		BIT(5)
#define   DATA_H            (GPIO_BOP(LCD_DATA_PORT)=LCD_DATA_PIN)
#define   DATA_L            (GPIO_BC(LCD_DATA_PORT)=LCD_DATA_PIN)




#define  LCD_IC2_1A_ICON_COM_INDEX				3
#define  LCD_IC2_1B_ICON_COM_INDEX				2
#define  LCD_IC2_1C_ICON_COM_INDEX				1
#define  LCD_IC2_1D_ICON_COM_INDEX				0
#define  LCD_IC2_1E_ICON_COM_INDEX				0
#define  LCD_IC2_1F_ICON_COM_INDEX				2
#define  LCD_IC2_1G_ICON_COM_INDEX				1


#define LCD_IC2_1A_ICON_SEG_INDEX 				1  
#define LCD_IC2_1B_ICON_SEG_INDEX 				1 
#define LCD_IC2_1C_ICON_SEG_INDEX 				1 
#define LCD_IC2_1D_ICON_SEG_INDEX 				1 
#define LCD_IC2_1E_ICON_SEG_INDEX 				0 
#define LCD_IC2_1F_ICON_SEG_INDEX 				0 
#define LCD_IC2_1G_ICON_SEG_INDEX 				0 




#define LCD_IC2_2A_ICON_COM_INDEX				3
#define LCD_IC2_2B_ICON_COM_INDEX				2
#define LCD_IC2_2C_ICON_COM_INDEX				1
#define LCD_IC2_2D_ICON_COM_INDEX				0
#define LCD_IC2_2E_ICON_COM_INDEX				0
#define LCD_IC2_2F_ICON_COM_INDEX				2
#define LCD_IC2_2G_ICON_COM_INDEX				1


#define LCD_IC2_2A_ICON_SEG_INDEX 				3  
#define LCD_IC2_2B_ICON_SEG_INDEX 				3 
#define LCD_IC2_2C_ICON_SEG_INDEX 				3 
#define LCD_IC2_2D_ICON_SEG_INDEX 				3 
#define LCD_IC2_2E_ICON_SEG_INDEX 				2 
#define LCD_IC2_2F_ICON_SEG_INDEX 				2 
#define LCD_IC2_2G_ICON_SEG_INDEX 				2 


#define LCD_IC2_3A_ICON_COM_INDEX				3
#define LCD_IC2_3B_ICON_COM_INDEX				2
#define LCD_IC2_3C_ICON_COM_INDEX				1
#define LCD_IC2_3D_ICON_COM_INDEX				0
#define LCD_IC2_3E_ICON_COM_INDEX				0
#define LCD_IC2_3F_ICON_COM_INDEX				2
#define LCD_IC2_3G_ICON_COM_INDEX				1



#define LCD_IC2_3A_ICON_SEG_INDEX 				10 
#define LCD_IC2_3B_ICON_SEG_INDEX 				10 
#define LCD_IC2_3C_ICON_SEG_INDEX 				10 
#define LCD_IC2_3D_ICON_SEG_INDEX 				10
#define LCD_IC2_3E_ICON_SEG_INDEX 				9 
#define LCD_IC2_3F_ICON_SEG_INDEX 				9 
#define LCD_IC2_3G_ICON_SEG_INDEX 				9


#define LCD_IC2_4A_ICON_COM_INDEX				3
#define LCD_IC2_4B_ICON_COM_INDEX				2
#define LCD_IC2_4C_ICON_COM_INDEX				1
#define LCD_IC2_4D_ICON_COM_INDEX				0
#define LCD_IC2_4E_ICON_COM_INDEX				0
#define LCD_IC2_4F_ICON_COM_INDEX				2
#define LCD_IC2_4G_ICON_COM_INDEX				1

#define LCD_IC2_4A_ICON_SEG_INDEX 				12 
#define LCD_IC2_4B_ICON_SEG_INDEX 				12
#define LCD_IC2_4C_ICON_SEG_INDEX 				12 
#define LCD_IC2_4D_ICON_SEG_INDEX 				12 
#define LCD_IC2_4E_ICON_SEG_INDEX 				11 
#define LCD_IC2_4F_ICON_SEG_INDEX 				11
#define LCD_IC2_4G_ICON_SEG_INDEX 				11 


#define LCD_IC2_5A_ICON_COM_INDEX				3
#define LCD_IC2_5B_ICON_COM_INDEX				2
#define LCD_IC2_5C_ICON_COM_INDEX				1
#define LCD_IC2_5D_ICON_COM_INDEX				0
#define LCD_IC2_5E_ICON_COM_INDEX				0
#define LCD_IC2_5F_ICON_COM_INDEX				2
#define LCD_IC2_5G_ICON_COM_INDEX				1

#define LCD_IC2_5A_ICON_SEG_INDEX 				14  
#define LCD_IC2_5B_ICON_SEG_INDEX 				14 
#define LCD_IC2_5C_ICON_SEG_INDEX 				14 
#define LCD_IC2_5D_ICON_SEG_INDEX 				14 
#define LCD_IC2_5E_ICON_SEG_INDEX 				13 
#define LCD_IC2_5F_ICON_SEG_INDEX 				13 
#define LCD_IC2_5G_ICON_SEG_INDEX 				13


#define LCD_IC2_6A_ICON_COM_INDEX				3
#define LCD_IC2_6B_ICON_COM_INDEX				2
#define LCD_IC2_6C_ICON_COM_INDEX				1
#define LCD_IC2_6D_ICON_COM_INDEX				0
#define LCD_IC2_6E_ICON_COM_INDEX				0
#define LCD_IC2_6F_ICON_COM_INDEX				2
#define LCD_IC2_6G_ICON_COM_INDEX				1

#define LCD_IC2_6A_ICON_SEG_INDEX 				16  
#define LCD_IC2_6B_ICON_SEG_INDEX 			  16
#define LCD_IC2_6C_ICON_SEG_INDEX 				16 
#define LCD_IC2_6D_ICON_SEG_INDEX 				16 
#define LCD_IC2_6E_ICON_SEG_INDEX 				15 
#define LCD_IC2_6F_ICON_SEG_INDEX 				15 
#define LCD_IC2_6G_ICON_SEG_INDEX 				15 


#define LCD_IC1_7A_ICON_COM_INDEX				0
#define LCD_IC1_7B_ICON_COM_INDEX				1
#define LCD_IC1_7C_ICON_COM_INDEX				2
#define LCD_IC1_7D_ICON_COM_INDEX				3
#define LCD_IC1_7E_ICON_COM_INDEX				3
#define LCD_IC1_7F_ICON_COM_INDEX				1
#define LCD_IC1_7G_ICON_COM_INDEX				2


#define LCD_IC1_7A_ICON_SEG_INDEX 				3  
#define LCD_IC1_7B_ICON_SEG_INDEX 				3 
#define LCD_IC1_7C_ICON_SEG_INDEX 				3 
#define LCD_IC1_7D_ICON_SEG_INDEX 				3 
#define LCD_IC1_7E_ICON_SEG_INDEX 				2 
#define LCD_IC1_7F_ICON_SEG_INDEX 				2
#define LCD_IC1_7G_ICON_SEG_INDEX 				2 


#define LCD_IC1_8A_ICON_COM_INDEX				0
#define LCD_IC1_8B_ICON_COM_INDEX				1
#define LCD_IC1_8C_ICON_COM_INDEX				2
#define LCD_IC1_8D_ICON_COM_INDEX				3
#define LCD_IC1_8E_ICON_COM_INDEX				3
#define LCD_IC1_8F_ICON_COM_INDEX				1
#define LCD_IC1_8G_ICON_COM_INDEX				2


#define LCD_IC1_8A_ICON_SEG_INDEX 				5  
#define LCD_IC1_8B_ICON_SEG_INDEX 				5
#define LCD_IC1_8C_ICON_SEG_INDEX 				5 
#define LCD_IC1_8D_ICON_SEG_INDEX 				5 
#define LCD_IC1_8E_ICON_SEG_INDEX 				4 
#define LCD_IC1_8F_ICON_SEG_INDEX 				4 
#define LCD_IC1_8G_ICON_SEG_INDEX 				4 


#define LCD_IC1_9A_ICON_COM_INDEX				0
#define LCD_IC1_9B_ICON_COM_INDEX				1
#define LCD_IC1_9C_ICON_COM_INDEX				2
#define LCD_IC1_9D_ICON_COM_INDEX				3
#define LCD_IC1_9E_ICON_COM_INDEX				3
#define LCD_IC1_9F_ICON_COM_INDEX				1
#define LCD_IC1_9G_ICON_COM_INDEX				2


#define LCD_IC1_9A_ICON_SEG_INDEX 				7  
#define LCD_IC1_9B_ICON_SEG_INDEX 				7 
#define LCD_IC1_9C_ICON_SEG_INDEX 				7 
#define LCD_IC1_9D_ICON_SEG_INDEX 				7 
#define LCD_IC1_9E_ICON_SEG_INDEX 				6 
#define LCD_IC1_9F_ICON_SEG_INDEX 				6 
#define LCD_IC1_9G_ICON_SEG_INDEX 				6 



#define LCD_IC1_10A_ICON_COM_INDEX				0
#define LCD_IC1_10B_ICON_COM_INDEX				1
#define LCD_IC1_10C_ICON_COM_INDEX				2
#define LCD_IC1_10D_ICON_COM_INDEX				3
#define LCD_IC1_10E_ICON_COM_INDEX				3
#define LCD_IC1_10F_ICON_COM_INDEX				1
#define LCD_IC1_10G_ICON_COM_INDEX				2

#define LCD_IC1_10A_ICON_SEG_INDEX 				9  
#define LCD_IC1_10B_ICON_SEG_INDEX 				9 
#define LCD_IC1_10C_ICON_SEG_INDEX 				9 
#define LCD_IC1_10D_ICON_SEG_INDEX 				9 
#define LCD_IC1_10E_ICON_SEG_INDEX 				8 
#define LCD_IC1_10F_ICON_SEG_INDEX 				8 
#define LCD_IC1_10G_ICON_SEG_INDEX 				8 


#define LCD_IC1_11A_ICON_COM_INDEX				0
#define LCD_IC1_11B_ICON_COM_INDEX				1
#define LCD_IC1_11C_ICON_COM_INDEX				2
#define LCD_IC1_11D_ICON_COM_INDEX				3
#define LCD_IC1_11E_ICON_COM_INDEX				3
#define LCD_IC1_11F_ICON_COM_INDEX				1
#define LCD_IC1_11G_ICON_COM_INDEX				2

#define LCD_IC1_11A_ICON_SEG_INDEX 				11  
#define LCD_IC1_11B_ICON_SEG_INDEX 				11 
#define LCD_IC1_11C_ICON_SEG_INDEX 				11 
#define LCD_IC1_11D_ICON_SEG_INDEX 				11 
#define LCD_IC1_11E_ICON_SEG_INDEX 				10 
#define LCD_IC1_11F_ICON_SEG_INDEX 				10
#define LCD_IC1_11G_ICON_SEG_INDEX 				10 


#define LCD_IC1_12A_ICON_COM_INDEX				0
#define LCD_IC1_12B_ICON_COM_INDEX				1
#define LCD_IC1_12C_ICON_COM_INDEX				2
#define LCD_IC1_12D_ICON_COM_INDEX				3
#define LCD_IC1_12E_ICON_COM_INDEX				3
#define LCD_IC1_12F_ICON_COM_INDEX				1
#define LCD_IC1_12G_ICON_COM_INDEX				2

#define LCD_IC1_12A_ICON_SEG_INDEX 				13  
#define LCD_IC1_12B_ICON_SEG_INDEX 				13
#define LCD_IC1_12C_ICON_SEG_INDEX 				13
#define LCD_IC1_12D_ICON_SEG_INDEX 				13 
#define LCD_IC1_12E_ICON_SEG_INDEX 				12
#define LCD_IC1_12F_ICON_SEG_INDEX 				12 
#define LCD_IC1_12G_ICON_SEG_INDEX 				12






//WIFI//S1
#define S1_ICON_SEG_INDEX					6
#define S1_ICON_COM_INDEX					0
//BT//S2
#define S2_ICON_SEG_INDEX					6
#define S2_ICON_COM_INDEX					1
//S3
#define S3_ICON_SEG_INDEX					6
#define S3_ICON_COM_INDEX					2
//LED//S4
#define S4_ICON_SEG_INDEX					6
#define S4_ICON_COM_INDEX					3
//S5
#define S5_ICON_SEG_INDEX					  7
#define S5_ICON_COM_INDEX					   3
//S6
#define S6_ICON_SEG_INDEX						7
#define S6_ICON_COM_INDEX						2
//S7
#define S7_ICON_SEG_INDEX					7
#define S7_ICON_COM_INDEX					1
//S8
#define S8_ICON_SEG_INDEX					7
#define S8_ICON_COM_INDEX					0
//S9
#define S9_ICON_SEG_INDEX					8
#define S9_ICON_COM_INDEX					1
//S10
#define S10_ICON_SEG_INDEX					8
#define S10_ICON_COM_INDEX					2
//S11
#define S11_ICON_SEG_INDEX					8
#define S11_ICON_COM_INDEX					3

//S12
#define S12_ICON_SEG_INDEX						14
#define S12_ICON_COM_INDEX						1
//13
#define S13_ICON_SEG_INDEX						14
#define S13_ICON_COM_INDEX						2
//14
#define S14_ICON_SEG_INDEX						14
#define S14_ICON_COM_INDEX						3
//15
#define S15_ICON_SEG_INDEX						15
#define S15_ICON_COM_INDEX						3
//S16
#define S16_ICON_SEG_INDEX					13
#define S16_ICON_COM_INDEX					3
//S17
#define S17_ICON_SEG_INDEX					11
#define S17_ICON_COM_INDEX					3
//S18
#define S18_ICON_SEG_INDEX					9
#define S18_ICON_COM_INDEX					3
//S19
#define S19_ICON_SEG_INDEX					5
#define S19_ICON_COM_INDEX					0
//S20
#define S20_ICON_SEG_INDEX						4
#define S20_ICON_COM_INDEX						0
//S21
#define S21_ICON_SEG_INDEX					6
#define S21_ICON_COM_INDEX					0
//S22
#define S22_ICON_SEG_INDEX					8
#define S22_ICON_COM_INDEX					0
//S23
#define S23_ICON_SEG_INDEX					2
#define S23_ICON_COM_INDEX					0
//S24
#define S24_ICON_SEG_INDEX					1
#define S24_ICON_COM_INDEX					0
//S25
#define S25_ICON_SEG_INDEX					1
#define S25_ICON_COM_INDEX					1
//S26
#define S26_ICON_SEG_INDEX					1
#define S26_ICON_COM_INDEX					2
//S27
#define S27_ICON_SEG_INDEX					1
#define S27_ICON_COM_INDEX					3
//S28
#define S28_ICON_SEG_INDEX					10
#define S28_ICON_COM_INDEX					0
//S29
#define S29_ICON_SEG_INDEX					12
#define S29_ICON_COM_INDEX					0
//S30
#define S30_ICON_SEG_INDEX					14
#define S30_ICON_COM_INDEX					0


//Y1
#define Y1_ICON_SEG_INDEX						4
#define Y1_ICON_COM_INDEX						3
//Y2
#define Y2_ICON_SEG_INDEX					5
#define Y2_ICON_COM_INDEX					3
//Y3
#define Y3_ICON_SEG_INDEX					5
#define Y3_ICON_COM_INDEX					2
//Y4
#define Y4_ICON_SEG_INDEX					0
#define Y4_ICON_COM_INDEX					0
//Y5
#define Y5_ICON_SEG_INDEX					0
#define Y5_ICON_COM_INDEX					1
//Y6
#define Y6_ICON_SEG_INDEX						0
#define Y6_ICON_COM_INDEX						2
//Y7
#define Y7_ICON_SEG_INDEX					0
#define Y7_ICON_COM_INDEX					3
//Y8
#define Y8_ICON_SEG_INDEX					0
#define Y8_ICON_COM_INDEX					3
//Y9
#define Y9_ICON_SEG_INDEX					2
#define Y9_ICON_COM_INDEX					3
//Y10
#define Y10_ICON_SEG_INDEX					5
#define Y10_ICON_COM_INDEX					1
//Y11
#define Y11_ICON_SEG_INDEX						4
#define Y11_ICON_COM_INDEX						2
//Y12
#define Y12_ICON_SEG_INDEX					4
#define Y12_ICON_COM_INDEX					1
//Y13
#define Y13_ICON_SEG_INDEX					4
#define Y13_ICON_COM_INDEX					0






//T1
#define T1_ICON_SEG_INDEX					15
#define T1_ICON_COM_INDEX					2
//T2
#define T2_ICON_SEG_INDEX					15
#define T2_ICON_COM_INDEX					1
//T3
#define T3_ICON_SEG_INDEX					16
#define T3_ICON_COM_INDEX					0
//T4
#define T4_ICON_SEG_INDEX					16
#define T4_ICON_COM_INDEX					1
//T5
#define T5_ICON_SEG_INDEX					16
#define T5_ICON_COM_INDEX					2
//T6
#define T6_ICON_SEG_INDEX					16
#define T6_ICON_COM_INDEX					3
//T7
#define T7_ICON_SEG_INDEX					17
#define T7_ICON_COM_INDEX					3
//T8
#define T8_ICON_SEG_INDEX					17
#define T8_ICON_COM_INDEX					2
//T9
#define T9_ICON_SEG_INDEX					17
#define T9_ICON_COM_INDEX					1
//T10
#define T10_ICON_SEG_INDEX					17
#define T10_ICON_COM_INDEX				 0
//T11
#define T11_ICON_SEG_INDEX					15
#define T11_ICON_COM_INDEX					3






#define BIAS3DUTY4	0x0852 //定义1 3 偏压4 背极
#define RC256 		0x0830 //使用内部256KRC 振荡器
#define SYSDIS  	0x0800 //SYS AS_20210610
#define SYSEN 		0x0802 //打开振荡发生器
#define LCDON 		0x0806 //打开LCD 
#define LCDOFF		0x0804//关闭LCD
#define TIMERDIS 	0x0808 //关闭时基输出

#define uchar unsigned char
#define uint  unsigned int
	

extern unsigned char gsDisplayBuff1[16];
extern unsigned char gsDisplayBuff2[16];



void HT1621_IfaceInit(void); //HT1621-IO初始化
void DelayMS(uint Ms);
void Ht1621Wr_Comd(unsigned short int mDat,unsigned char mBitCnt);

void FillLcdAll1(unsigned char mFillData);
void FillLcdFromBuff1(void);
void HT1621DispIcon1(unsigned char mSegIndex,unsigned char mComIndex);


void FillLcdAll2(unsigned char mFillData);
void FillLcdFromBuff2(void);
void HT1621DispIcon2(unsigned char mSegIndex,unsigned char mComIndex);

void DisplayNum1(signed short int mStartX,unsigned char mNum);
void DisplayNum2(signed short int mStartX,unsigned char mNum);
	

void  S_Display(uint8_t  Num);
void  Y_Display(uint8_t  Num);
void  T_Display(uint8_t  Num);

void DisplayIN_OUT_OOOO(u8 *set_task);//循环显示
void DisplayTIM_OO(u8 *set_task);//循环显示
void DisplaySOC_OO(u8 *set_task);//循环显示

#endif  //boardDISPLAY_EN

#endif  // __HT1621_H

