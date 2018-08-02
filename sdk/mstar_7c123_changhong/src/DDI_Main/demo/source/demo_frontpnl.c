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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_FrontPnl.h
/// @author MStar Semiconductor Inc.
/// @brief  FrontPnl Sample Demo
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_FRONTPNL_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvMMIO.h"
#include "msAPI_Gpio.h"
#include "drvFrontPnl.h"
#include "demo_utility.h"
//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define APP_DEMO_FrontPnl_TASK_STACK         (4096)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static void *_pTaskStack = NULL;
static MS_S32 _s32TaskId = -1;
static MS_S32 _s32TimerId = -1;

static void appDemo_FrontPnl_TimerCb(MS_U32 u32StTimer, MS_U32 u32TimerID)
{
    static MS_U8 u8dot = 0;
    static MS_U8 u8D4 = 0;
    static MS_U8 u8D3 = 0;
    static MS_U8 u8D2 = 0;
    static MS_U8 u8D1 = 0;
    static MS_U8 u8LED = 0;

    MS_BOOL arLedEnable[3];
    MS_U8 arDigital[4];

    u8LED++;

    arLedEnable[0] = FALSE;
    arLedEnable[1] = FALSE;
    arLedEnable[2] = FALSE;

    if(u8LED == 1)
    {
        arLedEnable[0] = TRUE;
    }

    if(u8LED == 2)
    {
         arLedEnable[1] = TRUE;
    }

    if(u8LED == 3)
    {
        arLedEnable[2] = TRUE;
        u8LED = 0;
    }

    MDrv_FrontPnl_LED_Enable(arLedEnable , 3);

    u8dot++;

    if(u8dot == 1) // 500 ms
    {

        MDrv_FrontPnl_Dot_Enable(0);
    }

    if(u8dot == 2) // 1000 ms
    {
        //printf("1\n");
        MDrv_FrontPnl_Dot_Enable(1);

        u8D1++;
        if(u8D1 == 10)
        {
            u8D1 = 0;
            u8D2++;
        }

        if(u8D2 == 6)
        {
            u8D2 = 0;
            u8D3++;
        }

        if(u8D3 == 10)
        {
            u8D3 = 0;
            u8D4++;
        }

        if(u8D4 == 6)
        {
            u8D4 = 0;
        }

        arDigital[0] = u8D1;
        arDigital[1] = u8D2;
        arDigital[2] = u8D3;
        arDigital[3] = u8D4;
        MDrv_FrontPnl_Digital_Set(arDigital,4);
        u8dot = 0;
    }
}
//-------------------------------------------------------------------------------------------------
/// app demo of input device to handle all interrupts and events.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _appDemo_FrontPnl_Task(MS_U32 argc, void *argv)
{
    MS_U32 u32KeyValue = 0;
    MDrv_FrontPnl_Init();

    _s32TimerId = MsOS_CreateTimer(appDemo_FrontPnl_TimerCb,500,500,TRUE,"Demo FrontPnl Timer");

    while(1)
    {
        MDrv_FrontPnl_ReadKeyPad(&u32KeyValue);

        if(u32KeyValue != 0){
            //printf("MDrv_FrontPnl_ReadKeyPad KeyValue = 0x%lu\n",u32KeyValue);
        }

        MsOS_DelayTask(2);
        //delay(500);

        #if 1
        MDrv_FrontPnl_Show();

        MsOS_DelayTask(2);
        #endif
    }
}

MS_BOOL Demo_FrontPnl_Init(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);
    // create app demo task of input device
    _pTaskStack = MsOS_AllocateMemory( APP_DEMO_FrontPnl_TASK_STACK, s32MstarCachedPoolID);
    if (_pTaskStack == NULL)
    {
        GEN_EXCEP;
    }
    _s32TaskId = MsOS_CreateTask(_appDemo_FrontPnl_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pTaskStack,
                                         APP_DEMO_FrontPnl_TASK_STACK,
                                         "Demo FrontPnl Tsk");
    if (_s32TaskId < 0)
    {
        GEN_EXCEP;
    }

    return TRUE;
}

MS_BOOL Demo_FrontPnl_LED_Enable(MS_U32 *u32LedRed, MS_U32 *u32LedYellow, MS_U32 *u32LedGreen)
{
    MS_BOOL arLedEnable[3];

    arLedEnable[0] = (MS_BOOL)(*u32LedRed);
    arLedEnable[1] = (MS_BOOL)(*u32LedGreen);
    arLedEnable[2] = (MS_BOOL)(*u32LedYellow);

    return MDrv_FrontPnl_LED_Enable(arLedEnable,3);
}

MS_BOOL Demo_FrontPnl_Dot_Enable(MS_U32 *u32Dot)
{
    MDrv_FrontPnl_Dot_Enable((MS_BOOL)(*u32Dot));
    return TRUE;
}

MS_BOOL Demo_FrontPnl_Digital_Set(MS_U32 *u32D4, MS_U32 *u32D3, MS_U32 *u32D2, MS_U32 *u32D1)
{
    MS_U8 arDigital[4];

    arDigital[0] = (MS_U8)(*u32D1);
    arDigital[1] = (MS_U8)(*u32D2);
    arDigital[2] = (MS_U8)(*u32D3);
    arDigital[3] = (MS_U8)(*u32D4);

    return MDrv_FrontPnl_Digital_Set(arDigital,4);
}

MS_BOOL Demo_FrontPnl_ShowOpen_Enable(MS_U32 *u32ShowOpen)
{
    return MDrv_FrontPnl_Show_Enable((MS_U8)(*u32ShowOpen));
}
#endif
