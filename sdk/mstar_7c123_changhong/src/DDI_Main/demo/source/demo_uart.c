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
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_UART_TEST == 1)
#include "MsCommon.h"
#include "MsMemory.h"
#include "drvUART.h"
#include <string.h>
#include <ctype.h>
#include "demo_utility.h"

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define UARTBUF_RX_TEST_SIZE        128
#define UARTBUF_SIZE        256
#define UARTDRV_LEN     64
#define XOFF_THRASHOLD ( UARTBUF_SIZE - 64 )
#define XON_THRASHOLD  ( UARTBUF_SIZE - 128 )
#define XON  17
#define XOFF 19

#define UART_DEMO_UART0    0
#define UART_DEMO_UART1    1
#define UART_DEMO_UART2    2
#define UART_DEMO_FUART0   3

//-------------------------------------------------------------------------------------------------
// Global Variable
//-------------------------------------------------------------------------------------------------
MS_U8 uart_test_data[]=
{
    #include "uart_test.dat"
};

static MS_U8 *UartRXBuffer;
static MS_U32 u32UartHandle ;
static MS_U32 u32UartRead = 0;
static MS_U32 u32UartWrite = 0;
static MS_U32 UartUartCharNum = 0;
static MS_BOOL UartUartStop = FALSE;

static MS_U8 *UartDrvBuf;

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
static void UartPutChar(MS_U8 u8Ch)
{
    MS_U8 u8Str[2] ;

    u8Str[0] = u8Ch ;
    u8Str[1] = 0 ;
    mdrv_uart_write( u32UartHandle, u8Str, 0x01) ;
}

static void Uart_XONOFF_FlowControl(void)
{
    MS_U32 size ;
    if (u32UartRead > u32UartWrite)
    {
        size = UARTBUF_SIZE - (u32UartRead - u32UartWrite );
    }
    else
    {
        size = u32UartWrite - u32UartRead ;
    }
    if ((size > XOFF_THRASHOLD ) && (!UartUartStop))
    {
        UartUartCharNum++;
        UartUartStop = TRUE ;
        UartPutChar((MS_U8)XOFF);
    }
    else if ((size <= XON_THRASHOLD ) && (UartUartStop))
    {
        UartUartStop = FALSE ;
        UartPutChar((MS_U8)XON);
    }

    return ;
}

void Demo_UART_RX_callback(int c)
{
    MS_U8 InCh;
    MS_U8 u8Str[2];

    mdrv_uart_read(u32UartHandle, u8Str, 0x01);

    u8Str[1] = 0 ;

    InCh = u8Str[0];

    if (InCh != (MS_U8)c)
    {
        return;
    }

    UartRXBuffer[u32UartWrite] = InCh ;
    u32UartWrite = (u32UartWrite + 1 ) % UARTBUF_SIZE ;

    Uart_XONOFF_FlowControl();
}

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Initial the UART and test the RX with file transfer
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b UART_RX_Test\n
///
/// Sample Command: \b UART_RX_Test \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_UART_RX_Test(MS_U32 *u32Mode)
{
    MS_S32 gs32NonCachedPoolID = INVALID_POOL_ID;
    UART_DEVICE_TYPE UartType = E_UART_INVALID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&gs32NonCachedPoolID);

	UartRXBuffer = MsOS_AllocateMemory(UARTBUF_SIZE, gs32NonCachedPoolID);

	UartDrvBuf = MsOS_AllocateMemory(UARTDRV_LEN, gs32NonCachedPoolID);

    switch (*u32Mode)
    {
        case UART_DEMO_UART0:
            UartType = E_UART_PIU_UART0;
            printf("Start to test PIU_UART0 to Receive Data\n");
            break;
        case UART_DEMO_UART1:
            UartType = E_UART_PIU_UART1;
            printf("Start to test PIU_UART1 to Receive Data\n");
            break;
        case UART_DEMO_UART2:
            UartType = E_UART_PIU_UART2;
            printf("Start to test PIU_UART2 to Receive Data\n");
            break;
        case UART_DEMO_FUART0:
            UartType = E_UART_PIU_FUART0;
            printf("Start to test PIU_FUART0 to Receive Data\n");
            break;
        default:
            UartType = E_UART_PIU_UART0;
            printf("Start to test PIU_UART0 to Receive Data\n");
            break;
    }

	printf("Please send the file uart_rx_test.bin\n");

	u32UartHandle = mdrv_uart_open(UartType);

    mdrv_uart_set_baudrate( u32UartHandle, 115200) ; // set default baud rate

    mdrv_uart_rx_enable(u32UartHandle);
    mdrv_uart_set_rx_buffer(u32UartHandle, UartDrvBuf, UARTDRV_LEN);
    mdrv_uart_set_rx_callback(u32UartHandle, Demo_UART_RX_callback);

    while(1)
    {
        //add volatile to prevent optimized by compiler
        if (*(volatile MS_U32*)(&u32UartWrite) >= UARTBUF_RX_TEST_SIZE)
        {
            int i;
            for (i=0; i<UARTBUF_RX_TEST_SIZE; i++)
            {
                if (UartRXBuffer[i] != uart_test_data[i])
                {
                    printf("\nError - compare different at buffer %d\n", i);
                    printf("UartRXBuffer[%d]=0x%x ; uart_test_data[%d]=0x%x\n",
                        i, UartRXBuffer[i], i, uart_test_data[i]);
                    return FALSE;
                }
            }
            u32UartWrite = 0;
            printf("\nReceive file and compare pass\n");
            return TRUE;
        }
    }

    return TRUE;
}
#endif
