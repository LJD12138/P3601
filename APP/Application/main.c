/*!
    \file    main.c
    \brief   APP entry

    \version 2020-09-04, V2.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.
*/

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
===============================================================
                     KEEP RUNNING, NO BUG
===============================================================
*/

#include "main.h"
#include "..\..\BOOT\Application\flash_allot_table.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board_config.h"

#if(boardPRINT_IFACE)
#include "Print/print_task.h"
#endif  //boardPRINT_IFACE

#if(boardWDGT_EN)
#include "fwdgt.h"
#endif  // boardWDGT_EN

// APP vector table offset.
// In Keil, enable "USE Memory Layout from Target Dialog" and set ROM start address correctly.

#define USER_BOOT_EXIST             1       // use bootloader or not
#if(USER_BOOT_EXIST)
#define NVIC_VECTTAB_RAM1           ((uint32_t)SRAM_START)
#define NVIC_VECTTAB_FLASH1         ((uint32_t)flashAPP_START)
#define VECT_TAB_OFFSET             flashAPP_START
#endif  // USER_BOOT_EXIST

// Task init
TaskHandle_t StartTask_Handler;
#define START_TASK_PRIO             1
void vBoard_StartTask(void *pvParameters);

/*****************************************************************************************************************
Function: NVIC init
*****************************************************************************************************************/
static void nvic_init(void)
{
    /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
}


/*****************************************************************************************************************
Function: APP entry
*****************************************************************************************************************/
int main(void)
{
    #if(USER_BOOT_EXIST == 1)
    nvic_vector_table_set(NVIC_VECTTAB_FLASH1, VECT_TAB_OFFSET);
    __enable_irq();
    #endif  // USER_BOOT_EXIST

    SystemInit();

    nvic_init();

    vBoard_SysInit();

    #if(boardWDGT_EN)
    vFwdgt_PrintResetReason();
    #endif  // boardWDGT_EN

    #if(boardSEGGER)
    SEGGER_RTT_printf(0, "------------------APP OK--------------------!\r\n");
    #endif  // boardSEGGER

    // Create start task
    xTaskCreate((TaskFunction_t)vBoard_StartTask,
                (const char *)"StartTask",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIO,
                (TaskHandle_t *)&StartTask_Handler);

    vTaskStartScheduler();

    while(1)
    {
    }
}
