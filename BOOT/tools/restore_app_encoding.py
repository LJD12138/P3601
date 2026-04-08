# -*- coding: gbk -*-

from pathlib import Path


APP_DIR = Path(r"g:\1-Baiku_Projects\15-M50\1.software\M5004-4\APP\Application")


MAIN_C = r'''/*!
    \file    main.c
    \brief   GPIO running led demo

    \version 2020-09-04, V2.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

****************************************************/

    /*                                             
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
            .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    ???汣??       ????崻?     ????BUG
***************************************************/

#include "main.h"
#include "..\..\BOOT\Application\flash_allot_table.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board_config.h"

#if(boardPRINT_IFACE)
#include "Print/print_task.h"
#endif  //boardPRINT_IFACE

//****************************************************???????**************************************************//
//APP?ж????????????? ?????Linker????????USE Memory Layout from Target Dialog????
//?????Target????е?ROM??Start???

#define 		USER_BOOT_EXIST				         	1			         		//?????bootloader
#if(USER_BOOT_EXIST)
#define 		NVIC_VECTTAB_RAM1                   	((uint32_t)SRAM_START) 		//RAM????
#define 		NVIC_VECTTAB_FLASH1                 	((uint32_t)flashAPP_START) 	//Flash????
#define 		VECT_TAB_OFFSET				         	flashAPP_START      		//?????
#endif	//USER_BOOT_EXIST

//****************************************************????????**************************************************//
TaskHandle_t    StartTask_Handler; 
#define      	START_TASK_PRIO             			1                   		//????????? 
void vBoard_StartTask(void *pvParameters);

/*****************************************************************************************************************
-----????????    ?ж?????
-----???(???)  none
-----???????    none
-----???????    none
-----?????      none
******************************************************************************************************************/
static void nvic_init(void)
{
     /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
}


/*****************************************************************************************************************
-----????????    APP?????
-----???(???)  none
-----???????    none
-----???????    none
-----?????      none
******************************************************************************************************************/
int main(void)
{
	
	#if(USER_BOOT_EXIST == 1)
	nvic_vector_table_set(NVIC_VECTTAB_FLASH1, VECT_TAB_OFFSET);  //????NVIC?ж????????????
	__enable_irq();	//????ж?????
	#endif	//USER_BOOT_EXIST
	
	SystemInit();
	
	nvic_init();        //?ж?????
	
	vBoard_SysInit();
	
	#if(boardWDGT_EN)
	vFwdgt_PrintResetReason();
	#endif  //boardWDGT_EN
	
    #if		printSEGGER
    SEGGER_RTT_printf(0,"------------------APP OK--------------------!\r\n");
    #endif	//printSEGGER
	
	//???????????
    xTaskCreate((TaskFunction_t )vBoard_StartTask,        //??????
                (const char* )"StartTask",          //????????
                (uint16_t ) 512,                     //????????С
                (void* )NULL,                       //????????????????
                (UBaseType_t ) START_TASK_PRIO,     //?????????
                (TaskHandle_t*)&StartTask_Handler); //??????
    
    vTaskStartScheduler(); 

    while(1) 
	{
		
    }
}
'''


FWDGT_C = r'''/*****************************************************************************************************************
*                                                                                                                *
 *                                         ?????????                                                           *
*                       ?????????????????????????????????Ч??????????????λ????????????????            *
*                       ????????????????????IRC40K,????????????????????                                  *
******************************************************************************************************************/

#include "fwdgt.h"
#include "gd32f30x_rcu.h"
#include "Print/print_api.h"

#if(boardWDGT_EN)
reset_reason_t g_reset_reason;

static void vFwdgt_ResetPrintf(const char *str, ...)
{
    char log_buf[128] = {0};
    va_list args;

    va_start(args, str);
    vsnprintf(log_buf, sizeof(log_buf), str, args);
    va_end(args);

    #if(boardPRINT_IFACE)
    sMyPrint("%s", log_buf);
    #endif

    #if(printSEGGER)
    SEGGER_RTT_printf(0, "%s", log_buf);
    #endif
}

/*****************************************************************************************************************
-----????????    ?????????
-----???(???)  none
-----???????    none
-----???????    none
-----?????      none
*****************************************************************************************************************/
void vFwdgt_Init(void)
{
  uint16_t timeout_t=0xFFFFU;
	/* enable IRC40K */
  rcu_osci_on(RCU_IRC40K);
	
	/* wait till IRC40K is ready */
  while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K))
  {
	if(timeout_t > 0) timeout_t--;
	else			  break;
  }
	
  /* configure FWDGT counter clock: 40KHz(IRC40K)  / 128 = 0.312 KHz */
  fwdgt_config(2*500,FWDGT_PSC_DIV128);				 //t = (1/0.312)x(2x500) = 3.2s
	
  fwdgt_write_disable();
  /* After 1.6 seconds to generate a reset */
  fwdgt_enable();
}


/*****************************************************************************************************************
-----????????    ι??
-----???(???)  ???????κε??ι??,??????????????
-----???????    none
-----???????    none
-----?????      none
*****************************************************************************************************************/
void vFwdgt_Reload(void)
{
	/* uncock fwdgt write protect*/
	fwdgt_write_enable();
	/* feed fwdgt */
	fwdgt_counter_reload();	
}


/*****************************************************************************************************************
-----????????    ?????????????
-----???(???)  ι???????????26S
-----???????    none
-----???????    none
-----?????      none
*****************************************************************************************************************/
void vFwdgt_EnterLowPower(void)
{
	vFwdgt_Reload();
  /* configure FWDGT counter clock: 40KHz(IRC40K)  / 256 = 0.156 KHz */
  fwdgt_config(0xfff,FWDGT_PSC_DIV256);				 //t = 26S
	
  fwdgt_write_disable();
  /* After 1.6 seconds to generate a reset */
  fwdgt_enable();
}


/*****************************************************************************************************************
-----????????    ?????????????
-----???(???)  none
-----???????    none
-----???????    none
-----?????      none
*****************************************************************************************************************/
void vFwdgt_ExitLowPower(void)
{
	vFwdgt_Reload();
    vFwdgt_Init();
}

/*****************************************************************************************************************
-----????????    ??????????λ???
-----???(???)  none
-----???????    none
-----???????    none
-----?????      none
*****************************************************************************************************************/
void vResetReason_Capture(void)
{
    g_reset_reason.ext_pin   = (rcu_flag_get(RCU_FLAG_EPRST)    == SET);
    g_reset_reason.por       = (rcu_flag_get(RCU_FLAG_PORRST)   == SET);
    g_reset_reason.sw        = (rcu_flag_get(RCU_FLAG_SWRST)    == SET);
    g_reset_reason.fwdgt     = (rcu_flag_get(RCU_FLAG_FWDGTRST) == SET);
    g_reset_reason.wwdgt     = (rcu_flag_get(RCU_FLAG_WWDGTRST) == SET);
    g_reset_reason.low_power = (rcu_flag_get(RCU_FLAG_LPRST)    == SET);

    // ????????????????????????
    rcu_all_reset_flag_clear();
}

/*****************************************************************************************************************
-----????????    ?????λ???
-----???(???)  ??????????????,???????????????????λ???
-----???????    none
-----???????    none
-----?????      none
*****************************************************************************************************************/
void vFwdgt_PrintResetReason(void)
{
    vResetReason_Capture();

    vFwdgt_ResetPrintf("\r\n[Reset] flag ext=%d por=%d sw=%d fwdgt=%d wwdgt=%d low_power=%d\r\n",
                       g_reset_reason.ext_pin,
                       g_reset_reason.por,
                       g_reset_reason.sw,
                       g_reset_reason.fwdgt,
                       g_reset_reason.wwdgt,
                       g_reset_reason.low_power);

    if(g_reset_reason.fwdgt)
    {
        vFwdgt_ResetPrintf("[Reset] reason: FWDGT reset\r\n");
    }
    else if(g_reset_reason.wwdgt)
    {
        vFwdgt_ResetPrintf("[Reset] reason: WWDGT reset\r\n");
    }
    else if(g_reset_reason.sw)
    {
        vFwdgt_ResetPrintf("[Reset] reason: software reset\r\n");
    }
    else if(g_reset_reason.ext_pin)
    {
        vFwdgt_ResetPrintf("[Reset] reason: external pin reset\r\n");
    }
    else if(g_reset_reason.por)
    {
        vFwdgt_ResetPrintf("[Reset] reason: power on reset\r\n");
    }
    else if(g_reset_reason.low_power)
    {
        vFwdgt_ResetPrintf("[Reset] reason: low power reset\r\n");
    }
    else
    {
        vFwdgt_ResetPrintf("[Reset] reason: unknown\r\n");
    }
}

#endif
'''


def write_gbk(name: str, text: str) -> None:
    path = APP_DIR / name
    backup = path.with_suffix(path.suffix + '.bak_before_encoding_fix')
    backup.write_bytes(path.read_bytes())
    path.write_bytes(text.replace("\n", "\r\n").encode("gbk"))


write_gbk("main.c", MAIN_C)
write_gbk("fwdgt.c", FWDGT_C)

print("restored and encoded as gbk")