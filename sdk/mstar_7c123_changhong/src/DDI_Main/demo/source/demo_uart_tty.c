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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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
/// @file   appDemo_UART_TTY.c
/// @author MStar Semiconductor Inc.
/// @brief  Generate Segmentation fault
///
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#if (DEMO_UART_TTY_TEST == 1)

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "MsCommon.h"
//#include "msAPI_uart.h"
#include "drvMMIO.h"
#include "drvUART.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
//#define O_RDWR 0x02
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//static MS_BOOL _bEnd = FALSE;
int g_iFD = 0;

struct termios t_tios;
struct termios tios;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


static void die(const char *s)
{
    perror(s);
    exit(2);
}



static void keypress_init(struct termios *old_tios)
{
//    struct termios tios;

    if(old_tios) {
        memset(old_tios, 0, sizeof(struct termios));
        if(tcgetattr(g_iFD, old_tios) < 0)
            die("tcgetattr");
    }

    if(tcgetattr(g_iFD, &tios) < 0)
            die("tcgetattr");
    if(tcgetattr(g_iFD, &t_tios) < 0)
            die("tcgetattr");

//tios.c_lflag &= ~(ECHO | ECHONL | ICANON);
tios.c_lflag &= ~( ECHO | ECHONL | ICANON | ISIG | IEXTEN );
//    tios.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
tios.c_iflag &= ~(INLCR | ICRNL | IXON);
tios.c_iflag |= IXANY;

    if(tcsetattr(g_iFD, TCSAFLUSH, &tios) < 0)
        die("tcsetattr");
}


static void keypress_exit(struct termios *old_tios)
{
    if(old_tios) {
        if(tcsetattr(g_iFD, TCSAFLUSH, old_tios) < 0)
            die("tcsetattr");
    } else {
        if(tcsetattr(g_iFD, TCSAFLUSH, &t_tios) < 0)
            die("tcsetattr");
    }
}

//-------------------------------------------------------------------------------------------------
/// @brief Initialize UART for data transmission
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: UART_TTY_INIT \n
//-------------------------------------------------------------------------------------------------

MS_BOOL Demo_UART_TTY_Init(void)
{
    int sts=0, iCount = 0, iRet = 0;
    fd_set read_fds;
    struct timeval timeout;

//    struct termios old_tios;
    unsigned char buf[512];

    MS_S32 u32Counter = 0;
    // redirect stdout
    freopen("/dev/null", "w", stdout);

    // open tty for uart transmission
    g_iFD = open("/dev/tty", O_RDWR);

    // must call MDrv_UART_SetIOMapBase before mdrv_uart_connect
    // switch uart source
    MDrv_UART_SetIOMapBase();
//    mdrv_uart_connect(E_UART_PORT0, E_UART_PIU_UART0);

    // due to stdout already redirected, the following printf would not be shown
    for(u32Counter = 0; u32Counter < 1234; u32Counter ++ ) printf(".");

    keypress_init(NULL);
    int size=0;
    while(1) {
        // Asynchronous read
        FD_ZERO(&read_fds);
        FD_SET(g_iFD, &read_fds);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        iRet = select(g_iFD + 1, &read_fds, NULL, NULL, &timeout);
        if( iRet <= 0 ) // Timeout or something error
        {
            break;
        }
        // read from tty
        size=read(g_iFD, buf+sts, 512-sts);
        if(buf[sts] == 'q')
            break;
        if(size!=0) {
            // write something to tty
//            write(g_iFD, &buf, 1);

            sts+=size;
        }
    }

    keypress_exit(NULL);

    // close fd after all operations
    close(g_iFD);

    // restore stdout
    freopen ("/dev/tty", "a", stdout);

    // switch uart source to mask all messages
//    mdrv_uart_connect(E_UART_PORT0, E_UART_PIU_UART1);
    printf("\n");
    printf("R %d\n",sts);

    for(iCount = 0; iCount <= sts ; iCount++)
    {
        printf("%02x ", buf[iCount]);
        if(iCount % 16 == 15) printf("\n");
    }

    return TRUE;
}



//-------------------------------------------------------------------------------------------------
/// @brief The UART test help.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:UART_TTY_HELP \n
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_UART_TTY_Help(void)
{
    return TRUE;
}

#endif
