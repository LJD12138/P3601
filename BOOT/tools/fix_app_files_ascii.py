from pathlib import Path


APP_DIR = Path(r"g:\1-Baiku_Projects\15-M50\1.software\M5004-4\APP\Application")


MAIN_C = r'''/*!
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

    #if printSEGGER
    SEGGER_RTT_printf(0, "------------------APP OK--------------------!\r\n");
    #endif  // printSEGGER

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
'''


FWDGT_C = r'''/*****************************************************************************************************************
*                                                                                                                *
*                                    Independent watchdog                                                        *
*  The watchdog clock comes from internal low speed clock. Even if the main clock fails, it can still reset.    *
*  The watchdog uses IRC40K and can also work in standby / deep sleep modes.                                     *
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
Function: watchdog init
*****************************************************************************************************************/
void vFwdgt_Init(void)
{
    uint16_t timeout_t = 0xFFFFU;

    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);

    /* wait till IRC40K is ready */
    while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K))
    {
        if(timeout_t > 0)
        {
            timeout_t--;
        }
        else
        {
            break;
        }
    }

    /* configure FWDGT counter clock: 40KHz(IRC40K) / 128 = 0.312 KHz */
    fwdgt_config(2 * 500, FWDGT_PSC_DIV128);    // t = (1 / 0.312) x (2 x 500) = 3.2 s

    fwdgt_write_disable();
    fwdgt_enable();
}


/*****************************************************************************************************************
Function: feed watchdog
*****************************************************************************************************************/
void vFwdgt_Reload(void)
{
    fwdgt_write_enable();
    fwdgt_counter_reload();
}


/*****************************************************************************************************************
Function: watchdog enter low power
*****************************************************************************************************************/
void vFwdgt_EnterLowPower(void)
{
    vFwdgt_Reload();

    /* configure FWDGT counter clock: 40KHz(IRC40K) / 256 = 0.156 KHz */
    fwdgt_config(0xfff, FWDGT_PSC_DIV256);      // t = 26 s

    fwdgt_write_disable();
    fwdgt_enable();
}


/*****************************************************************************************************************
Function: watchdog exit low power
*****************************************************************************************************************/
void vFwdgt_ExitLowPower(void)
{
    vFwdgt_Reload();
    vFwdgt_Init();
}


/*****************************************************************************************************************
Function: capture reset reason
*****************************************************************************************************************/
void vResetReason_Capture(void)
{
    g_reset_reason.ext_pin   = (rcu_flag_get(RCU_FLAG_EPRST) == SET);
    g_reset_reason.por       = (rcu_flag_get(RCU_FLAG_PORRST) == SET);
    g_reset_reason.sw        = (rcu_flag_get(RCU_FLAG_SWRST) == SET);
    g_reset_reason.fwdgt     = (rcu_flag_get(RCU_FLAG_FWDGTRST) == SET);
    g_reset_reason.wwdgt     = (rcu_flag_get(RCU_FLAG_WWDGTRST) == SET);
    g_reset_reason.low_power = (rcu_flag_get(RCU_FLAG_LPRST) == SET);

    // Clear flags immediately after reading to avoid false detection next time.
    rcu_all_reset_flag_clear();
}


/*****************************************************************************************************************
Function: print reset reason
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

#endif  // boardWDGT_EN
'''


def write_ascii_file(path: Path, text: str) -> None:
    backup = path.with_suffix(path.suffix + ".bak_ascii_fix")
    backup.write_bytes(path.read_bytes())
    path.write_text(text.replace("\n", "\r\n"), encoding="ascii", newline="")


write_ascii_file(APP_DIR / "main.c", MAIN_C)
write_ascii_file(APP_DIR / "fwdgt.c", FWDGT_C)

print("APP main.c and fwdgt.c restored with ASCII-safe content.")