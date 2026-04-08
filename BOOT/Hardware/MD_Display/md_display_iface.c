#include "MD_Display/md_display_iface.h"

#if(boardDISPLAY_EN)

#if(boardUSE_OS)
#include "freertos.h"
#include "task.h"
#endif  //boardUSE_OS

unsigned char gsDisplayBuff1[16];
unsigned char gsDisplayBuff2[16];

const unsigned char NUM_COM_SEG_TAB[]=
{	
 LCD_IC2_1A_ICON_COM_INDEX,				
 LCD_IC2_1B_ICON_COM_INDEX,				
 LCD_IC2_1C_ICON_COM_INDEX,				
 LCD_IC2_1D_ICON_COM_INDEX,				
 LCD_IC2_1E_ICON_COM_INDEX,				
 LCD_IC2_1F_ICON_COM_INDEX,				
 LCD_IC2_1G_ICON_COM_INDEX,				

 LCD_IC2_1A_ICON_SEG_INDEX,				 
 LCD_IC2_1B_ICON_SEG_INDEX, 				 
 LCD_IC2_1C_ICON_SEG_INDEX, 				 
 LCD_IC2_1D_ICON_SEG_INDEX, 				 
 LCD_IC2_1E_ICON_SEG_INDEX, 				 
 LCD_IC2_1F_ICON_SEG_INDEX, 				 
 LCD_IC2_1G_ICON_SEG_INDEX,

 LCD_IC2_2A_ICON_COM_INDEX,				
 LCD_IC2_2B_ICON_COM_INDEX,				
 LCD_IC2_2C_ICON_COM_INDEX,				
 LCD_IC2_2D_ICON_COM_INDEX,				
 LCD_IC2_2E_ICON_COM_INDEX,				
 LCD_IC2_2F_ICON_COM_INDEX,				
 LCD_IC2_2G_ICON_COM_INDEX,				

 LCD_IC2_2A_ICON_SEG_INDEX, 				 
 LCD_IC2_2B_ICON_SEG_INDEX, 				 
 LCD_IC2_2C_ICON_SEG_INDEX, 				 
 LCD_IC2_2D_ICON_SEG_INDEX, 				 
 LCD_IC2_2E_ICON_SEG_INDEX, 				 
 LCD_IC2_2F_ICON_SEG_INDEX, 				 
 LCD_IC2_2G_ICON_SEG_INDEX, 				 
 
 LCD_IC2_3A_ICON_COM_INDEX,				
 LCD_IC2_3B_ICON_COM_INDEX,				
 LCD_IC2_3C_ICON_COM_INDEX,				
 LCD_IC2_3D_ICON_COM_INDEX,				
 LCD_IC2_3E_ICON_COM_INDEX,				
 LCD_IC2_3F_ICON_COM_INDEX,				
 LCD_IC2_3G_ICON_COM_INDEX,				

 LCD_IC2_3A_ICON_SEG_INDEX, 				 
 LCD_IC2_3B_ICON_SEG_INDEX, 				 
 LCD_IC2_3C_ICON_SEG_INDEX, 				 
 LCD_IC2_3D_ICON_SEG_INDEX, 				
 LCD_IC2_3E_ICON_SEG_INDEX, 				
 LCD_IC2_3F_ICON_SEG_INDEX, 				 
 LCD_IC2_3G_ICON_SEG_INDEX, 				

 LCD_IC2_4A_ICON_COM_INDEX,				
 LCD_IC2_4B_ICON_COM_INDEX,				
 LCD_IC2_4C_ICON_COM_INDEX,				
 LCD_IC2_4D_ICON_COM_INDEX,				
 LCD_IC2_4E_ICON_COM_INDEX,				
 LCD_IC2_4F_ICON_COM_INDEX,				
 LCD_IC2_4G_ICON_COM_INDEX,				

 LCD_IC2_4A_ICON_SEG_INDEX, 				 
 LCD_IC2_4B_ICON_SEG_INDEX, 				
 LCD_IC2_4C_ICON_SEG_INDEX, 				 
 LCD_IC2_4D_ICON_SEG_INDEX, 				 
 LCD_IC2_4E_ICON_SEG_INDEX, 				 
 LCD_IC2_4F_ICON_SEG_INDEX, 				
 LCD_IC2_4G_ICON_SEG_INDEX, 				 

 LCD_IC2_5A_ICON_COM_INDEX,				
 LCD_IC2_5B_ICON_COM_INDEX,				
 LCD_IC2_5C_ICON_COM_INDEX,				
 LCD_IC2_5D_ICON_COM_INDEX,				
 LCD_IC2_5E_ICON_COM_INDEX,				
 LCD_IC2_5F_ICON_COM_INDEX,				
 LCD_IC2_5G_ICON_COM_INDEX,				

 LCD_IC2_5A_ICON_SEG_INDEX, 				  
 LCD_IC2_5B_ICON_SEG_INDEX, 				 
 LCD_IC2_5C_ICON_SEG_INDEX, 				 
 LCD_IC2_5D_ICON_SEG_INDEX, 				 
 LCD_IC2_5E_ICON_SEG_INDEX, 				 
 LCD_IC2_5F_ICON_SEG_INDEX, 				 
 LCD_IC2_5G_ICON_SEG_INDEX, 				

 LCD_IC2_6A_ICON_COM_INDEX,				
 LCD_IC2_6B_ICON_COM_INDEX,				
 LCD_IC2_6C_ICON_COM_INDEX,				
 LCD_IC2_6D_ICON_COM_INDEX,				
 LCD_IC2_6E_ICON_COM_INDEX,				
 LCD_IC2_6F_ICON_COM_INDEX,				
 LCD_IC2_6G_ICON_COM_INDEX,				

 LCD_IC2_6A_ICON_SEG_INDEX, 				 
 LCD_IC2_6B_ICON_SEG_INDEX, 			  
 LCD_IC2_6C_ICON_SEG_INDEX, 				
 LCD_IC2_6D_ICON_SEG_INDEX, 				 
 LCD_IC2_6E_ICON_SEG_INDEX, 				 
 LCD_IC2_6F_ICON_SEG_INDEX, 				 
 LCD_IC2_6G_ICON_SEG_INDEX, 				 

 LCD_IC1_7A_ICON_COM_INDEX,				
 LCD_IC1_7B_ICON_COM_INDEX,				
 LCD_IC1_7C_ICON_COM_INDEX,				
 LCD_IC1_7D_ICON_COM_INDEX,				
 LCD_IC1_7E_ICON_COM_INDEX,				
 LCD_IC1_7F_ICON_COM_INDEX,				
 LCD_IC1_7G_ICON_COM_INDEX,

 LCD_IC1_7A_ICON_SEG_INDEX, 				 
 LCD_IC1_7B_ICON_SEG_INDEX, 				 
 LCD_IC1_7C_ICON_SEG_INDEX, 				 
 LCD_IC1_7D_ICON_SEG_INDEX, 				 
 LCD_IC1_7E_ICON_SEG_INDEX, 				 
 LCD_IC1_7F_ICON_SEG_INDEX, 				
 LCD_IC1_7G_ICON_SEG_INDEX, 				 

 LCD_IC1_8A_ICON_COM_INDEX,				
 LCD_IC1_8B_ICON_COM_INDEX,				
 LCD_IC1_8C_ICON_COM_INDEX,				
 LCD_IC1_8D_ICON_COM_INDEX,				
 LCD_IC1_8E_ICON_COM_INDEX,				
 LCD_IC1_8F_ICON_COM_INDEX,				
 LCD_IC1_8G_ICON_COM_INDEX,				

 LCD_IC1_8A_ICON_SEG_INDEX, 				  
 LCD_IC1_8B_ICON_SEG_INDEX, 				
 LCD_IC1_8C_ICON_SEG_INDEX, 				 
 LCD_IC1_8D_ICON_SEG_INDEX, 				 
 LCD_IC1_8E_ICON_SEG_INDEX, 				 
 LCD_IC1_8F_ICON_SEG_INDEX, 				 
 LCD_IC1_8G_ICON_SEG_INDEX, 				 

 LCD_IC1_9A_ICON_COM_INDEX,				
 LCD_IC1_9B_ICON_COM_INDEX,				
 LCD_IC1_9C_ICON_COM_INDEX,				
 LCD_IC1_9D_ICON_COM_INDEX,				
 LCD_IC1_9E_ICON_COM_INDEX,				
 LCD_IC1_9F_ICON_COM_INDEX,				
 LCD_IC1_9G_ICON_COM_INDEX,				

 LCD_IC1_9A_ICON_SEG_INDEX, 				  
 LCD_IC1_9B_ICON_SEG_INDEX, 				 
 LCD_IC1_9C_ICON_SEG_INDEX, 				
 LCD_IC1_9D_ICON_SEG_INDEX, 				 
 LCD_IC1_9E_ICON_SEG_INDEX, 				 
 LCD_IC1_9F_ICON_SEG_INDEX, 				 
 LCD_IC1_9G_ICON_SEG_INDEX, 				 

 LCD_IC1_10A_ICON_COM_INDEX,			
 LCD_IC1_10B_ICON_COM_INDEX,				
 LCD_IC1_10C_ICON_COM_INDEX,				
 LCD_IC1_10D_ICON_COM_INDEX,				
 LCD_IC1_10E_ICON_COM_INDEX,				
 LCD_IC1_10F_ICON_COM_INDEX,				
 LCD_IC1_10G_ICON_COM_INDEX,			

 LCD_IC1_10A_ICON_SEG_INDEX, 				  
 LCD_IC1_10B_ICON_SEG_INDEX, 				 
 LCD_IC1_10C_ICON_SEG_INDEX, 				 
 LCD_IC1_10D_ICON_SEG_INDEX, 				 
 LCD_IC1_10E_ICON_SEG_INDEX, 				
 LCD_IC1_10F_ICON_SEG_INDEX, 				
 LCD_IC1_10G_ICON_SEG_INDEX, 			 

 LCD_IC1_11A_ICON_COM_INDEX,			
 LCD_IC1_11B_ICON_COM_INDEX,				
 LCD_IC1_11C_ICON_COM_INDEX,				
 LCD_IC1_11D_ICON_COM_INDEX,				
 LCD_IC1_11E_ICON_COM_INDEX,				
 LCD_IC1_11F_ICON_COM_INDEX,				
 LCD_IC1_11G_ICON_COM_INDEX,				

 LCD_IC1_11A_ICON_SEG_INDEX, 				  
 LCD_IC1_11B_ICON_SEG_INDEX, 				 
 LCD_IC1_11C_ICON_SEG_INDEX, 				 
 LCD_IC1_11D_ICON_SEG_INDEX, 				 
 LCD_IC1_11E_ICON_SEG_INDEX, 				 
 LCD_IC1_11F_ICON_SEG_INDEX, 				
 LCD_IC1_11G_ICON_SEG_INDEX, 				 

 LCD_IC1_12A_ICON_COM_INDEX,				
 LCD_IC1_12B_ICON_COM_INDEX,				
 LCD_IC1_12C_ICON_COM_INDEX,				
 LCD_IC1_12D_ICON_COM_INDEX,				
 LCD_IC1_12E_ICON_COM_INDEX,				
 LCD_IC1_12F_ICON_COM_INDEX,				
 LCD_IC1_12G_ICON_COM_INDEX,				

 LCD_IC1_12A_ICON_SEG_INDEX, 				 
 LCD_IC1_12B_ICON_SEG_INDEX, 				
 LCD_IC1_12C_ICON_SEG_INDEX, 				
 LCD_IC1_12D_ICON_SEG_INDEX, 				 
 LCD_IC1_12E_ICON_SEG_INDEX, 				
 LCD_IC1_12F_ICON_SEG_INDEX, 				 
 LCD_IC1_12G_ICON_SEG_INDEX, 					
};

const unsigned char LCD_NUM_DISP_TAB[]=
{
	0x3F,//0	1010 1111	//0
	0x06,//1	0000 0110	//1
	0x5B,//2	1100 1101	//2
	0x4F,//3	1000 1111	//3
	0x66,//4	1100 0110	//4
	0x6D,//5	1100 1011	//5	
	0x7D,//6	1110 1011	//6
	0x07,//7	0000 1110	//7
	0x7F,//8	1110 1111	//8
	0xEF,//9	1100 1111	//9
	0x40,//-	0100 0000	//10
	0x38,//L	1010 0001	//11
	0x77,//A	1110 1110	//12
	0x7C,//b	1110 0011	//13
	0x58,//c	0110 0001	//14
	0x5E,//d	0110 0111	//15
	0x79,//E	1110 1001	//16
	0x71,//F	1110 1000	//17
	0x74,//h	1110 0010	//18
	0x73,//P	1110 1100	//19
	0x39,//C	1010 1001	//20
	0x76,//H	1110 0110	//21
	0x78,//t	1110 0001	//22
	0x48,//=	0100 0001	//23
	0x5C,//o	0110 0011	//24
	0x54,//n  				//25
    0x37,//N  				//26
	0x72,//Y    0111 0010	//27
	0x50,//r	0110 0000	//28
	0x3E,//U	1010 1110	//29
};


void DelayMS(uint Ms) /*延时 ms*/
{
	uchar j=0;
	while(Ms--)
	for(j=0;j<100;j++); 
}

void Ht1621Wr_Comd(unsigned short int mDat,unsigned char mBitCnt)
{
	unsigned char i=0;
	unsigned short int tmp=0;
	if (mBitCnt==0)
	return;
	CS1_L;
	CS2_L;
	DelayMS(1);
	tmp = (1<<(mBitCnt-1));
	for (i=0;i<mBitCnt;i++)
	{
		if (mDat&tmp)
		DATA_H ;
		else  
		DATA_L;	
		WR_L;
		DelayMS(1);
		WR_H;
		DelayMS(1);
		mDat <<= 1;
	}  
	  DATA_H;
	  CS1_H;
	  CS2_H;
}

void FillLcdAll1(unsigned char mFillData)
{  
	unsigned char i=0;
	for (i=0;i<16;i++)
	{
		gsDisplayBuff1[i]=mFillData;
	}
} 

void FillLcdAll2(unsigned char mFillData)
{  
	unsigned char i=0;
	for (i=0;i<16;i++)
	{
		gsDisplayBuff2[i]=mFillData;
	}
}



void FillLcdFromBuff1(void)
{	
	unsigned char i=0,j=0;
	unsigned short int mDat=0,tmp=0;
	CS1_L;
	DelayMS(1);
	mDat = (0x05<<6); //写数据命令
	tmp = (1<<(9-1));
	for (i=0;i<9;i++)//写入1 01aa aaaa,a=0表示首地址，即1 0100 0000
	{
		if (mDat&tmp)
		DATA_H;
		else  
		DATA_L;	
		WR_L;
		DelayMS(1);
		WR_H;
		DelayMS(1);
		mDat <<= 1;
	}
	for (i=0;i<16;i++)//写入数据
	{
		mDat = gsDisplayBuff1[i];
		for (j=0;j<8;j++)
		{
			if (mDat&1)
			DATA_H;
			else  
			DATA_L;	
			WR_L;
			DelayMS(1);
			WR_H;
			DelayMS(1);
			mDat >>= 1;	 
		}
	}
	  
	DATA_H;
	CS1_H;
}


void FillLcdFromBuff2(void)
{	
	unsigned char i=0,j=0;
	unsigned short int mDat=0,tmp=0;
	CS2_L;
	DelayMS(1);
	mDat = (0x05<<6); //写数据命令
	tmp = (1<<(9-1));
	for (i=0;i<9;i++)//写入1 01aa aaaa,a=0表示首地址，即1 0100 0000
	{
		if (mDat&tmp)
		DATA_H;
		else  
		DATA_L;	
		WR_L;
		DelayMS(1);
		WR_H;
		DelayMS(1);
		mDat <<= 1;
	}
	for (i=0;i<16;i++)//写入数据
	{
		mDat = gsDisplayBuff2[i];
		for (j=0;j<8;j++)
		{
			if (mDat&1)
			DATA_H;
			else  
			DATA_L;	
			WR_L;
			DelayMS(1);
			WR_H;
			DelayMS(1);
			mDat >>= 1;	 
		}
	}
	DATA_H;
	CS2_H;
}


void HT1621DispIcon1(unsigned char mSegIndex,unsigned char mComIndex)
{
	if (mSegIndex>17)
	return;	 
	if (mComIndex>3)
	return;	
	if (mSegIndex&0x01)
	{
		gsDisplayBuff1[(mSegIndex>>1)] |= (1<<(mComIndex+4));
	}
	else
	{  
		gsDisplayBuff1[(mSegIndex>>1)] |= (1<<(mComIndex));
	}
}  

void HT1621DispIcon2(unsigned char mSegIndex,unsigned char mComIndex)
{
	if (mSegIndex>17)
	return;	 
	if (mComIndex>3)
	return;	
	if (mSegIndex&0x01)
	{
		gsDisplayBuff2[(mSegIndex>>1)] |= (1<<(mComIndex+4));
	}
	else
	{  
		gsDisplayBuff2[(mSegIndex>>1)] |= (1<<(mComIndex));
	}
} 



void DisplayNum2(signed short int mStartX,unsigned char mNum)
{
	unsigned char i=0,tmp=0;
	tmp = LCD_NUM_DISP_TAB[mNum];
	
	//与屏一致性
	//if(mStartX==0){return;}
	//mStartX=mStartX-1;
	
	
	for (i=0;i<7;i++)
	{
		if (tmp&1)
		HT1621DispIcon2(NUM_COM_SEG_TAB[mStartX*14+7+i],NUM_COM_SEG_TAB[mStartX*14+i]);
		tmp >>= 1;
	} 	
}


void DisplayIN_OUT_OOOO(u8 *set_task)//循环显示
{

	unsigned char i=0;
	static u8 s = 6,tmp=0x01;
	
	
	
	
	for (i=0;i<7;i++)
	{
		if (tmp&1)
		{
		  HT1621DispIcon1(NUM_COM_SEG_TAB[s*14+7+i],NUM_COM_SEG_TAB[s*14+i]);
		  HT1621DispIcon2(NUM_COM_SEG_TAB[(s-4)*14+7+i],NUM_COM_SEG_TAB[(s-4)*14+i]);		
		}
		tmp >>= 1;
	} 	
	
	//printf("任务:%d,tmp:0x%.2x,s:%d\r\n",*set_task,tmp,s);
	
	switch(*set_task)
	{
		case 0:
		{
			tmp=0x01;
			s++;
			if(s>=9){s=9;(*set_task)++;}
		}break;
		case 1:
		{
			tmp=0x02; 
			(*set_task)++;
		}break;
		case 2:
		{
			tmp=0x04; 
			(*set_task)++;
		}break;
		case 3:
		{
			tmp=0x08; 
            (*set_task)++;
		}break;
		case 4:
		{
			tmp=0x08;		
			s--;
			if(s<=6){s=6;(*set_task)++;}
		}break;
		case 5:
		{
			tmp=0x10; 	
			(*set_task)++;
		}break;
		case 6:
		{
			tmp=0x20; 	
			(*set_task)++;
		}break;
		case 7:
		{
			tmp=0x01;	
			*set_task=0;
		}break;
	}
	
	
	
}
void DisplayTIM_OO(u8 *set_task)//循环显示
{

	unsigned char i=0;
	static u8 s = 0,tmp=0x01;
	
	
	
	
	for (i=0;i<7;i++)
	{
		if (tmp&1)
		{
		  HT1621DispIcon2(NUM_COM_SEG_TAB[(s)*14+7+i],NUM_COM_SEG_TAB[(s)*14+i]);		
		}
		tmp >>= 1;
	} 	
	
	//printf("任务:%d,tmp:0x%.2x,s:%d\r\n",*set_task,tmp,s);
	
	switch(*set_task)
	{
		case 0:
		{
			tmp=0x01;
			s++;
			if(s>=1){s=1;(*set_task)++;}
		}break;
		case 1:
		{
			tmp=0x02; 
			(*set_task)++;
		}break;
		case 2:
		{
			tmp=0x04; 
			(*set_task)++;
		}break;
		case 3:
		{
			tmp=0x08; 
            (*set_task)++;
		}break;
		case 4:
		{
			tmp=0x08;		
			s--;
			if(s<=0){s=0;(*set_task)++;}
		}break;
		case 5:
		{
			tmp=0x10; 	
			(*set_task)++;
		}break;
		case 6:
		{
			tmp=0x20; 	
			(*set_task)++;
		}break;
		case 7:
		{
			tmp=0x01;	
			*set_task=0;
		}break;
	}		
}
void DisplaySOC_OO(u8 *set_task)//循环显示
{

	unsigned char i=0;
	static u8 s = 10,tmp=0x01;
	
	
	
	
	for (i=0;i<7;i++)
	{
		if (tmp&1)
		{
		  HT1621DispIcon1(NUM_COM_SEG_TAB[(s)*14+7+i],NUM_COM_SEG_TAB[(s)*14+i]);		
		}
		tmp >>= 1;
	} 	
	
	//printf("任务:%d,tmp:0x%.2x,s:%d\r\n",*set_task,tmp,s);
	
	switch(*set_task)
	{
		case 0:
		{
			tmp=0x01;
			s++;
			if(s>=11){s=11;(*set_task)++;}
		}break;
		case 1:
		{
			tmp=0x02; 
			(*set_task)++;
		}break;
		case 2:
		{
			tmp=0x04; 
			(*set_task)++;
		}break;
		case 3:
		{
			tmp=0x08; 
            (*set_task)++;
		}break;
		case 4:
		{
			tmp=0x08;		
			s--;
			if(s<=10){s=10;(*set_task)++;}
		}break;
		case 5:
		{
			tmp=0x10; 	
			(*set_task)++;
		}break;
		case 6:
		{
			tmp=0x20; 	
			(*set_task)++;
		}break;
		case 7:
		{
			tmp=0x01;	
			*set_task=0;
		}break;
	}		
}
//void DisplayOOOO(void)//循环显示
//{

//	unsigned char i,tmp;
//	static u8 t,s = 6,set=0;
//	
//	DisplayNum2(0,9);
//	HT1621DispIcon2(S18_ICON_SEG_INDEX,S18_ICON_COM_INDEX);
//	S_Display(19);//W	
//	S_Display(22);//W	
//	
//	if(s>9){s=6;}
//	//SOC十位
//	if(t>7){t=0;set=0;}
//	set|=(1<<t);
//	tmp=set;
//	
//	for (i=0;i<7;i++)
//	{
//		if (tmp&1)
//		{
//		  HT1621DispIcon1(NUM_COM_SEG_TAB[s*14+7+i],NUM_COM_SEG_TAB[s*14+i]);
//		  HT1621DispIcon2(NUM_COM_SEG_TAB[(s-4)*14+7+i],NUM_COM_SEG_TAB[(s-4)*14+i]);		
//		}
//		tmp >>= 1;
//	} 	

//	t++;
//	s++;
//}
void DisplayNum1(signed short int mStartX,unsigned char mNum)
{
	unsigned char i=0,tmp=0;
	tmp = LCD_NUM_DISP_TAB[mNum];
	
	//与屏一致性
	//if(mStartX<6){return;}
	//mStartX=mStartX-6;
	
	for (i=0;i<7;i++)
	{
		if (tmp&1)
		HT1621DispIcon1(NUM_COM_SEG_TAB[mStartX*14+7+i],NUM_COM_SEG_TAB[mStartX*14+i]);
		tmp >>= 1;
	} 	
}

void  S_Display(uint8_t  Num)
{
 	switch(Num)
        {  
            case 1:
            HT1621DispIcon2(S1_ICON_SEG_INDEX,S1_ICON_COM_INDEX);
            break;						
            case 2:
            HT1621DispIcon2(S2_ICON_SEG_INDEX,S2_ICON_COM_INDEX);
            break;
            case 3:   
            HT1621DispIcon2(S3_ICON_SEG_INDEX,S3_ICON_COM_INDEX);
            break;
            case 4:
            HT1621DispIcon2(S4_ICON_SEG_INDEX,S4_ICON_COM_INDEX); 
            break;
            case 5:
            HT1621DispIcon2(S5_ICON_SEG_INDEX,S5_ICON_COM_INDEX);
            break;
            case 6:
            HT1621DispIcon2(S6_ICON_SEG_INDEX,S6_ICON_COM_INDEX);
            break;
            case 7:
            HT1621DispIcon2(S7_ICON_SEG_INDEX,S7_ICON_COM_INDEX);
            break;
            case 8: 
            HT1621DispIcon2(S8_ICON_SEG_INDEX,S8_ICON_COM_INDEX);							
            break;
						case 9:
            HT1621DispIcon2(S9_ICON_SEG_INDEX,S9_ICON_COM_INDEX);
            break;						
            case 10:
            HT1621DispIcon2(S10_ICON_SEG_INDEX,S10_ICON_COM_INDEX);
            break;
            case 11:   
            HT1621DispIcon2(S11_ICON_SEG_INDEX,S11_ICON_COM_INDEX);
            break;
            case 12:
            HT1621DispIcon1(S12_ICON_SEG_INDEX,S12_ICON_COM_INDEX); 
            break;
            case 13:
            HT1621DispIcon1(S13_ICON_SEG_INDEX,S13_ICON_COM_INDEX);
            break;
            case 14:
            HT1621DispIcon1(S14_ICON_SEG_INDEX,S14_ICON_COM_INDEX);
            break;
            case 15:
            HT1621DispIcon2(S15_ICON_SEG_INDEX,S15_ICON_COM_INDEX);
            break;
            case 16: 
            HT1621DispIcon2(S16_ICON_SEG_INDEX,S16_ICON_COM_INDEX);							
            break;
						case 17:
            HT1621DispIcon2(S17_ICON_SEG_INDEX,S17_ICON_COM_INDEX);
            break;						
            case 18:
            HT1621DispIcon2(S18_ICON_SEG_INDEX,S18_ICON_COM_INDEX);
            break;
            case 19:   
            HT1621DispIcon2(S19_ICON_SEG_INDEX,S19_ICON_COM_INDEX);
            break;
            case 20:
            HT1621DispIcon1(S20_ICON_SEG_INDEX,S20_ICON_COM_INDEX); 
            break;
            case 21:
            HT1621DispIcon1(S21_ICON_SEG_INDEX,S21_ICON_COM_INDEX);
            break;
            case 22:
            HT1621DispIcon1(S22_ICON_SEG_INDEX,S22_ICON_COM_INDEX);
            break;
            case 23:
            HT1621DispIcon1(S23_ICON_SEG_INDEX,S23_ICON_COM_INDEX);
            break;
            case 24: 
            HT1621DispIcon1(S24_ICON_SEG_INDEX,S24_ICON_COM_INDEX);							
            break;
						case 25:
            HT1621DispIcon1(S25_ICON_SEG_INDEX,S25_ICON_COM_INDEX);
            break;						
            case 26:
            HT1621DispIcon1(S26_ICON_SEG_INDEX,S26_ICON_COM_INDEX);
            break;
            case 27:   
            HT1621DispIcon1(S27_ICON_SEG_INDEX,S27_ICON_COM_INDEX);
            break;
            case 28:
            HT1621DispIcon1(S28_ICON_SEG_INDEX,S28_ICON_COM_INDEX); 
            break;
            case 29:
            HT1621DispIcon1(S29_ICON_SEG_INDEX,S29_ICON_COM_INDEX);
            break;
            case 30:
            HT1621DispIcon1(S30_ICON_SEG_INDEX,S30_ICON_COM_INDEX);
            break;					
			}
}


void  Y_Display(uint8_t  Num)
{
 	switch(Num)
        {  
            case 1:
            HT1621DispIcon2(Y1_ICON_SEG_INDEX,Y1_ICON_COM_INDEX);
            break;						
            case 2:
            HT1621DispIcon2(Y2_ICON_SEG_INDEX,Y2_ICON_COM_INDEX);
            break;
            case 3:   
            HT1621DispIcon2(Y3_ICON_SEG_INDEX,Y3_ICON_COM_INDEX);
            break;
            case 4:
            HT1621DispIcon1(Y4_ICON_SEG_INDEX,Y4_ICON_COM_INDEX); 
            break;
            case 5:
            HT1621DispIcon1(Y5_ICON_SEG_INDEX,Y5_ICON_COM_INDEX);
            break;
            case 6:
            HT1621DispIcon1(Y6_ICON_SEG_INDEX,Y6_ICON_COM_INDEX);
            break;
            case 7:
            HT1621DispIcon1(Y7_ICON_SEG_INDEX,Y7_ICON_COM_INDEX);
            break;
            case 8: 
            HT1621DispIcon2(Y8_ICON_SEG_INDEX,Y8_ICON_COM_INDEX);							
            break;
						case 9:
            HT1621DispIcon2(Y9_ICON_SEG_INDEX,Y9_ICON_COM_INDEX);
            break;						
            case 10:
            HT1621DispIcon2(Y10_ICON_SEG_INDEX,Y10_ICON_COM_INDEX);
            break;
            case 11:   
            HT1621DispIcon2(Y11_ICON_SEG_INDEX,Y11_ICON_COM_INDEX);
            break;
            case 12:
            HT1621DispIcon2(Y12_ICON_SEG_INDEX,Y12_ICON_COM_INDEX); 
            break;
            case 13:
            HT1621DispIcon2(Y13_ICON_SEG_INDEX,Y13_ICON_COM_INDEX);
            break;	
			}
}

void  T_Display(uint8_t  Num)
{
 	switch(Num)
        {  
            case 1:
            HT1621DispIcon1(T1_ICON_SEG_INDEX,T1_ICON_COM_INDEX);
            break;						
            case 2:
            HT1621DispIcon1(T2_ICON_SEG_INDEX,T2_ICON_COM_INDEX);
            break;
            case 3:   
            HT1621DispIcon1(T3_ICON_SEG_INDEX,T3_ICON_COM_INDEX);
            break;
            case 4:
            HT1621DispIcon1(T4_ICON_SEG_INDEX,T4_ICON_COM_INDEX); 
            break;
            case 5:
            HT1621DispIcon1(T5_ICON_SEG_INDEX,T5_ICON_COM_INDEX);
            break;
            case 6:
            HT1621DispIcon1(T6_ICON_SEG_INDEX,T6_ICON_COM_INDEX);
            break;
            case 7:
            HT1621DispIcon1(T7_ICON_SEG_INDEX,T7_ICON_COM_INDEX);
            break;
            case 8: 
            HT1621DispIcon1(T8_ICON_SEG_INDEX,T8_ICON_COM_INDEX);							
            break;
						case 9:
            HT1621DispIcon1(T9_ICON_SEG_INDEX,T9_ICON_COM_INDEX);
            break;						
            case 10:
            HT1621DispIcon1(T10_ICON_SEG_INDEX,T10_ICON_COM_INDEX);
            break;
            case 11:   
            HT1621DispIcon1(T11_ICON_SEG_INDEX,T11_ICON_COM_INDEX);
            break;			
			}
}


void HT1621_IfaceInit(void) /*HT1621-IO初始化*/
{ 	
	rcu_periph_clock_enable(RCU_AF);  //使能时钟
	rcu_periph_clock_enable(LCD_RCCB);//使能时钟
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);//使用SW模式	禁用JTAG

	gpio_init( LCD_PORTB,               /*端口*/
	           GPIO_MODE_OUT_PP,       /*推挽输出*/
	           GPIO_OSPEED_50MHZ,      /*频率*/
	           dispLIGHT_POWER_PIN|LCD_WR_PIN|LCD_DATA_PIN|LCD_CS1_PIN|LCD_CS2_PIN             /*IO*/
	);
	dispLIGHT_POWER_OFF();
	
    DelayMS(10);
    
	Ht1621Wr_Comd(BIAS3DUTY4,12);
	Ht1621Wr_Comd(RC256,12);
	Ht1621Wr_Comd(SYSEN,12);
	Ht1621Wr_Comd(LCDON,12);
	Ht1621Wr_Comd(TIMERDIS,12);
	
//	Display_SetStandbyMode();//待机模式
}

#endif  //boardDISPLAY_EN
