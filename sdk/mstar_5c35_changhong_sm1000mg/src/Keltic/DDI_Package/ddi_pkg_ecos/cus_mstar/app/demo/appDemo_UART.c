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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_CA.c
/// @author MStar Semiconductor Inc.
/// @brief  Descrambler Function Sample Demo
///
////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsMemory.h"
#include "drvUART.h"
#include <string.h>
#include <ctype.h>

//--------------------------------------------------------------------------------------------------
// constant
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// enumerate
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Structure define
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
// Implementation parts
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Implementation parts
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// local function
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// API implmentation
//--------------------------------------------------------------------------------------------------

static MS_U32          _u32UartHandle ;
#define FASTUARTBUF_SIZE        512

static MS_U8           FastUartAppBuffer[FASTUARTBUF_SIZE] ;

static MS_U32  u32FastUartRead = 0 , u32FastUartWrite = 0 , FastUartUartCharNum = 0;
static MS_BOOL FastUartUartStop = FALSE ;

#define      FastUartDrvLen     64
static MS_U8 FastUartDrvBuf[FastUartDrvLen];

#define XOFF_THRASHOLD ( FASTUARTBUF_SIZE - 64 )
#define XON_THRASHOLD  ( FASTUARTBUF_SIZE - 128 )
#define XON  17
#define XOFF 19

void _FastUartPutChar(MS_U8 u8Ch)
{
    MS_U8 u8Str[2] ;

    u8Str[0] = u8Ch ;
    u8Str[1] = 0 ;
    mdrv_uart_write( _u32UartHandle, u8Str, 0x01) ;
}

void FastUart_XONOFF_FlowControl(void)
{
    MS_U32 size ;
    if (u32FastUartRead > u32FastUartWrite)
    {
        size = FASTUARTBUF_SIZE - (u32FastUartRead - u32FastUartWrite );
    }
    else
    {
        size = u32FastUartWrite - u32FastUartRead ;
    }
    if ((size > XOFF_THRASHOLD ) && (!FastUartUartStop))
    {
        FastUartUartCharNum++;
        FastUartUartStop = TRUE ;
        _FastUartPutChar((MS_U8)XOFF);
    }
    else if ((size <= XON_THRASHOLD ) && (FastUartUartStop))
    {
        FastUartUartStop = FALSE ;
        _FastUartPutChar((MS_U8)XON);
    }

    return ;
}


void appDemo_uart_rx_callback(int c)
{
    MS_U8 InCh;
    MS_U8 u8Str[2] ;

    mdrv_uart_read( _u32UartHandle, u8Str, 0x01) ;

    u8Str[1] = 0 ;

    InCh = u8Str[0];

    if (InCh != (MS_U8)c)
    {
        return;
    }

    FastUartAppBuffer[u32FastUartWrite] = InCh ;
    u32FastUartWrite = (u32FastUartWrite + 1 ) % FASTUARTBUF_SIZE ;

    FastUart_XONOFF_FlowControl();
}

MS_BOOL MApi_FastUart_Init(void)
{
    //fast uart pad and clock selection
    *(volatile MS_U16*)(0xBF203C00+(0x53<<2)) |= 0x0070;        // CHIPTOP: select piu_uart //0111:  PIU FAST UART
    *(volatile MS_U16*)(0xBF203C00+(0x03<<2)) |= 0x0002;        // second uart = 2 [1:0]

    *(volatile MS_U16*)(0xBF203C00+(0x04<<2)) &= ~0x0030;        // reg_mcu_jtag_mode [5:4]
    *(volatile MS_U16*)(0xBF203C00+(0x05<<2)) &= ~0x0100;        // reg_i2s_in_mode [8]

    *(volatile MS_U16*)(0xBF201600+(0x17<<2)) &= ~0xFF;         // CLKGEN0: reg_ckg_fuart [3:0]
    *(volatile MS_U16*)(0xBF201600+(0x17<<2)) |= 0x300;         //  [3:2]: as following 'd0: 432MHZ
    *(volatile MS_U16*)(0xBF201600+(0x18<<2)) = 0x1C10;         //  clock dividor
    *(volatile MS_U16*)(0xBF001C00+(0x09<<2)) |= 0x0800;        // PM_SLEEP: reg_uart_enable

    MDrv_UART_SetIOMapBase();
    //_u32UartHandle = mdrv_uart_open(E_UART_PIU_UART1);
    _u32UartHandle = mdrv_uart_open(E_UART_PIU_FUART0);
    mdrv_uart_set_baudrate( _u32UartHandle, 115200) ; // set default baud rate


    mdrv_uart_rx_enable(_u32UartHandle);
    mdrv_uart_set_rx_buffer(_u32UartHandle, FastUartDrvBuf, FastUartDrvLen);
    mdrv_uart_set_rx_callback(_u32UartHandle,appDemo_uart_rx_callback);

    return TRUE;
}

void MApi_FastUart_PutChar(MS_U8 ch)
{
    MS_U8 u8Str[2] ;

    u8Str[0] = ch ;
    u8Str[1] = 0 ;
    mdrv_uart_write( _u32UartHandle, u8Str, 0x01) ;

}

MS_U8 MApi_FastUart_GetChar(void)
{
    MS_U8 u8Str[2] ;

    mdrv_uart_read( _u32UartHandle, u8Str, 0x01) ;
    u8Str[1] = 0 ;

    return u8Str[0] ;
}

//static MS_U8 test_string[] = {"Jeff test string "};
static MS_U8 test_string[] = {"Jeff,"};
MS_BOOL appDemo_UART_Uart2Test(void)
{
    printf("Uart2 test, test\n");

#if 1 //uart 1
    //*(volatile MS_U16*)(0xBF203C00+(0x53<<2)) |= 0x0050;        // CHIPTOP: select piu_uart //0101:  PIU FAST UART
    *(volatile MS_U16*)(0xBF201600+(0x14<<2)) &= ~0x1F;         // CLKGEN0: reg_ckg_uart1 [4:0]
    *(volatile MS_U16*)(0xBF201600+(0x14<<2)) |= 0x0C;        //          [4:0] UART1_CLK : 123Mhz [0] wired UART1_CLK
    *(volatile MS_U16*)(0xBF001C00+(0x09<<2)) |= 0x0800;        // PM_SLEEP: reg_uart_enable
#endif

#if 1 //fast uart
    *(volatile MS_U16*)(0xBF203C00+(0x53<<2)) |= 0x0070;        // CHIPTOP: select piu_uart //0111:  PIU FAST UART
    *(volatile MS_U16*)(0xBF203C00+(0x03<<2)) |= 0x0002;        // second uart = 2 [1:0]

    *(volatile MS_U16*)(0xBF203C00+(0x04<<2)) &= ~0x0030;        // reg_mcu_jtag_mode [5:4]
    *(volatile MS_U16*)(0xBF203C00+(0x05<<2)) &= ~0x0100;        // reg_i2s_in_mode [8]

    *(volatile MS_U16*)(0xBF201600+(0x17<<2)) &= ~0xFF;         // CLKGEN0: reg_ckg_fuart [3:0]
    *(volatile MS_U16*)(0xBF201600+(0x17<<2)) |= 0x300;         //  [3:2]: as following 'd0: 432MHZ
    *(volatile MS_U16*)(0xBF201600+(0x18<<2)) = 0x1C10;         //  clock dividor
    *(volatile MS_U16*)(0xBF001C00+(0x09<<2)) |= 0x0800;        // PM_SLEEP: reg_uart_enable
#endif



    MDrv_UART_SetIOMapBase();
    //_u32UartHandle = mdrv_uart_open(E_UART_PIU_UART1);
    _u32UartHandle = mdrv_uart_open(E_UART_PIU_FUART0);
    mdrv_uart_set_baudrate( _u32UartHandle, 115200) ; // set default baud rate


    mdrv_uart_rx_enable(_u32UartHandle);
    mdrv_uart_set_rx_buffer(_u32UartHandle, FastUartDrvBuf, FastUartDrvLen);
    mdrv_uart_set_rx_callback(_u32UartHandle,appDemo_uart_rx_callback);


    MS_U8 u8Str[2] ;

    u8Str[0] = 0x30 ;
    u8Str[1] = 0 ;

    while(1)
    {
        //mdrv_uart_write( _u32UartHandle, test_string, sizeof(test_string)) ;
        MS_U8 i;
        for(i=0;i<sizeof(test_string);i++)
        {
            u8Str[0] = test_string[i];
            mdrv_uart_write( _u32UartHandle, u8Str, 0x1) ;
        }
        MsOS_DelayTask(10);

        for(i=0;i<sizeof(test_string);i++)
        {
            //u8Str[0] = test_string[i];
            MDrv_UART_PutChar(test_string[i]);
        }
        MsOS_DelayTask(10);
    }

    return TRUE;
}


