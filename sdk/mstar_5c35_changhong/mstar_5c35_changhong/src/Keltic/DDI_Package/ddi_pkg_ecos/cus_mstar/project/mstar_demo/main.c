//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    main.c
/// @brief  main entry point of the application program
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "drvUART.h"
#include "SysInit.h"

#ifndef DDI_MAIN_TEST
#include "appDemo_DMX.h"
#include "appDemo_Video.h"
#else
#include "demo_dmx.h"
#endif

#include <cyg/hal/hal_cache.h>
#include "Board.h"
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define TASK_STK_SIZE       4096

#define UARTBUF_SIZE        512

#define XOFF_THRASHOLD ( UARTBUF_SIZE - 64 )
#define XON_THRASHOLD  ( UARTBUF_SIZE - 128 )
#define XON  17
#define XOFF 19


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_S32  gs32SystemPoolID = 0; // for MSOS_ECOS


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 _u8TaskStartStk[TASK_STK_SIZE];


static MS_U8   UartBuffer[UARTBUF_SIZE] ;

static MS_U32  u32Read = 0 , u32Write = 0 , CharNum = 0;
static MS_BOOL UartStop = FALSE ;

#ifndef FE_AUTO_TEST
MS_U8 UartBuf[64];
#endif

static MS_BOOL DisableUart = FALSE  ;
static MS_U32  UartDisableCounter = 0 ;

/* ********************************************************************* */
/* Global definitions */
extern  MS_BOOL appDemo_Main(void);

extern UART_Result MDrv_UART_SetRxMode
       (MS_BOOL bEnable, void *rx_buf, MS_U16 buf_len, ms_uart_rx_callback rx_cb);



//[NOTE] Simple XON/XOFF flowcontrol mechanism .
void XONOFF_FlowControl(void)
{

    MS_U32 size  = 0;
    if (u32Read > u32Write)
    {
        size = UARTBUF_SIZE - (u32Read - u32Write );
    }
    else
    {
        size = u32Write - u32Read ;
    }
    if ((size > XOFF_THRASHOLD ) && (!UartStop))
    {
        CharNum++;
        UartStop = TRUE ;
        MDrv_UART_PutChar((MS_U8)XOFF);
    }
    else if ((size <= XON_THRASHOLD ) && (UartStop))
    {
        UartStop = FALSE ;
        MDrv_UART_PutChar((MS_U8)XON);
    }

}




MS_U8 UartMatch[] = {'0','0','1','1','2','2','3','3'};
void UartDisableCheck(MS_U8 InCh)
{
    // Check with Uart Match Table //
    if ( UartMatch[UartDisableCounter] == InCh )
    {
            UartDisableCounter++ ;
    }
    else
    {
        if (InCh != UartMatch[0])
            UartDisableCounter = 0 ;
        else
            UartDisableCounter = 1 ;
    }
    if (UartDisableCounter == 8)
    {
        DisableUart = ~DisableUart ;
        UartDisableCounter = 0 ;
    }
}

void UartCB(int c)
{
    MS_U8 InCh;

    InCh = MDrv_UART_GetChar();

    if (InCh != (MS_U8)c)
    {
        return;
    }

    UartBuffer[u32Write] = InCh ;
    u32Write = (u32Write + 1 ) % UARTBUF_SIZE ;
    XONOFF_FlowControl();

    UartDisableCheck(InCh);

    return ;
}



void TaskStart (MS_U32 argc, VOID *argv)
{
    MApi_SystemDeviceHost_Init();
#ifndef FE_AUTO_TEST
    appDemo_Main();
#endif

#ifndef DDI_MAIN_TEST
            appDemo_TSP_Init();
            appDemo_Video_PreSetControl();
#else
            Demo_DMX_Init();
#endif


    printf("Looping...\n");

    while(1)
    {
        MsOS_DelayTask(10000);
    }
}

/* ********************************************************************* */
/* Global functions */
int AppMain_main (void *pPoolStart)
{

        // Set the TLB exception handler
        extern unsigned int utlb_vector[];
        memcpy((void *)0x80000000, utlb_vector, 0x80);
        HAL_DCACHE_FLUSH(0x80000000, 0x80);
        HAL_ICACHE_SYNC();


    MS_BOOL msBool;

    //----------------------------------------------------------------------------------------------
    //  System Initialization
    //----------------------------------------------------------------------------------------------
    msBool = MApi_SystemInit_Init(pPoolStart);

    MsOS_CreateTask((TaskEntry) TaskStart,
                    (MS_U32)NULL,
                    E_TASK_PRI_HIGH,
                    TRUE,
                    _u8TaskStartStk,
                    TASK_STK_SIZE,
                    "App" );

    u32Write = u32Read = CharNum = 0 ;
    UartStop = FALSE ;
    memset( UartBuffer,0x0 , UARTBUF_SIZE );

#ifndef FE_AUTO_TEST
    MDrv_UART_SetRxMode(TRUE,UartBuf,64,UartCB);
#endif

    return 0;
}

void cyg_user_start(void)
{
    extern char __heap1_end[];
    void *pFirstAvailableMemory = (void *)__heap1_end;
    AppMain_main(pFirstAvailableMemory);
}

void cyg_uart_init(void)
{
    *(volatile MS_U16*)(0xBF203C00+(0x53<<2)) |= 0x0004;        // CHIPTOP: select piu_uart
    *(volatile MS_U16*)(0xBF201600+(0x13<<2)) &= ~0x1F00;       // CLKGEN0: ckg_uart [12:8]
    *(volatile MS_U16*)(0xBF201600+(0x13<<2)) |= 0x0C00;        //          [12:8] UART_CLK : 123Mhz [0] wired UART_CLK
    *(volatile MS_U16*)(0xBF001C00+(0x09<<2)) |= 0x0800;        // PM_SLEEP: reg_uart_enable

#if (MS_BOARD_TYPE_SEL == BD_KELTIC_FPGA)
    MDrv_UART_Init(E_UART_PIU_UART0, 38400);
#else
    MDrv_UART_Init(E_UART_PIU_UART0, 115200);
#endif

}

void cyg_uart_putchar(unsigned char c)
{
    if(!DisableUart)
    MDrv_UART_PutChar((MS_U8)c);
}

unsigned char cyg_uart_getchar(void)
{
    //return MDrv_UART_GetChar();
    MS_U8 tmp ;

    while(1)
    {
        if(u32Read != u32Write)
        {
            tmp =  UartBuffer[u32Read];
            u32Read = (u32Read + 1) % UARTBUF_SIZE;

            XONOFF_FlowControl();

            return  tmp;
        }
        MsOS_DelayTask(1);
    }


}


void __cxa_atexit(void (*arg1)(void*), void* arg2, void* arg3)
{

}

/* ********************************************************************* */
