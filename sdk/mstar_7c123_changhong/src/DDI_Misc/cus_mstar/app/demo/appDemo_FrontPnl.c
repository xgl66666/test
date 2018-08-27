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
/// @file   appDemo_FrontPnl.c
/// @author MStar Semiconductor Inc.
/// @brief A sample code for illustrating how to control front panel
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvFrontPnl.h"
#include "msAPI_Gpio.h"
#include "drvGPIO.h"
#include "appDemo_FrontPnl.h"
#include "drvDTC.h"


//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define APP_DEMO_FRONTPNL_TASK_STACK         (4096)


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S32 gs32CachedPoolID;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static void *_pTaskStack = NULL;
static MS_S32 _s32TaskId = -1;

#if ( FRONTPNL_TYPE ==  FRONTPANEL_TYPE_74HC164)

static MS_U8 gDigit0 = 0;
static MS_U8 gDigit1 = 0;
static MS_U8 gDigit2 = 0;
static MS_U8 gDigit3 = 0;
static MS_U8 gEnableDigit0 = 0;
static MS_U8 gEnableDigit1 = 0;
static MS_U8 gEnableDigit2 = 0;
static MS_U8 gEnableDigit3 = 0;

static MS_BOOL gDot = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static MS_BOOL _appDemo_Display_Digits(MS_U8 u8Digit0, MS_U8 u8Digit1, MS_U8 u8Digit2, MS_U8 u8Digit3, MS_BOOL bDot)
{
    gDigit0 = u8Digit0;
    gDigit1 = u8Digit1;
    gDigit2 = u8Digit2;
    gDigit3 = u8Digit3;
    gDot = bDot;

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
// app demo of input device to handle all interrupts and events.
// return None
//-------------------------------------------------------------------------------------------------
static void _appDemo_FrontPnl_Task(MS_U32 argc, void *argv)
{
    while(1)
    {
        MDrv_FrontPnl_Enable_Digit(3, gEnableDigit3);
        MDrv_FrontPnl_Show_Digit(3, gDigit3, FALSE);
        MsOS_DelayTask(1);
        MDrv_FrontPnl_Enable_Digit(3, FALSE);

        MDrv_FrontPnl_Enable_Digit(2, gEnableDigit2);
        MDrv_FrontPnl_Show_Digit(2, gDigit2, gDot);
        MsOS_DelayTask(1);
        MDrv_FrontPnl_Enable_Digit(2, FALSE);;

        MDrv_FrontPnl_Enable_Digit(1, gEnableDigit1);
        MDrv_FrontPnl_Show_Digit(1, gDigit1, FALSE);
        MsOS_DelayTask(1);
        MDrv_FrontPnl_Enable_Digit(1, FALSE);

        MDrv_FrontPnl_Enable_Digit(0, gEnableDigit0);
        MDrv_FrontPnl_Show_Digit(0, gDigit0, FALSE);
        MsOS_DelayTask(1);
        MDrv_FrontPnl_Enable_Digit(0, FALSE);
    }

}



//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Create the app demo task of front panel and initialze all parameters
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_Init \n
///
/// Sample Command: \b FrontPnl_Init
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_Init(void)
{
    MDrv_FrontPnl_Init();

    // create app demo task of input device

    _pTaskStack = MsOS_AllocateMemory( APP_DEMO_FRONTPNL_TASK_STACK, gs32CachedPoolID);

    if (_pTaskStack == NULL)
    {
        printf("_pTaskStack == NULL!!!\n");
        return FALSE;
    }
    _s32TaskId = MsOS_CreateTask(_appDemo_FrontPnl_Task,
                                         NULL,
                                         E_TASK_PRI_HIGHEST, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pTaskStack,
                                         APP_DEMO_FRONTPNL_TASK_STACK,
                                         "Demo Input Tsk");

    printf("_s32TaskId = 0x%x\n",_s32TaskId);
    if (_s32TaskId < 0)
    {
        printf("_s32TaskId < 0!!!\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set digits on the front panel
/// @param[in] u32D4 The fourth digit to be shown
/// @param[in] u32D3 The third digit to be shown
/// @param[in] u32D2 The second digit to be shown
/// @param[in] u32D1 The first digit to be shown
/// @param[in] u32Dot Enable dot
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_DigitalSet \n
///
/// Sample Command: \b FrontPnl_DigitalSet 1 2 3 0 1
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_Digital_Set(MS_U32 *u32D4, MS_U32 *u32D3, MS_U32 *u32D2, MS_U32 *u32D1, MS_U32 *u32Dot)
{

    gDigit0 = (MS_U8)(*u32D1);
    gDigit1 = (MS_U8)(*u32D2);
    gDigit2 = (MS_U8)(*u32D3);
    gDigit3 = (MS_U8)(*u32D4);
    gDot = (MS_U8) (*u32Dot);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Enable/Disable front panel display
/// @param[in] u32ShowOpen Enable showing digits
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_ShowEnable \n
///
/// Sample Command: \b FrontPnl_ShowEnable 1
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_ShowOpen_Enable(MS_U32 *u32ShowOpen)
{
    gEnableDigit0 = (MS_U8)(*u32ShowOpen);
    gEnableDigit1 = (MS_U8)(*u32ShowOpen);
    gEnableDigit2 = (MS_U8)(*u32ShowOpen);
    gEnableDigit3 = (MS_U8)(*u32ShowOpen);

    return TRUE;
}



#elif ( FRONTPNL_TYPE ==  FRONTPANEL_TYPE_PT6958)

#define APP_DEMO_FRONTPNL_TIMER_TASK_STACK         (4096)

static MS_S32 _s32TimerId = -1;
static MS_S32 _s32EventId = -1;
static void *_pTimerTaskStack = NULL;
static MS_S32 _s32TimerTaskId = -1;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void appDemo_FrontPnl_TimerCb(MS_U32 u32StTimer, MS_U32 u32TimerID)
{
    MsOS_SetEvent(_s32EventId, 0x0001);

}
//-------------------------------------------------------------------------------------------------
/// app demo of input device to handle all interrupts and events.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _appDemo_FrontPnl_Task(MS_U32 argc, void *argv)
{

    MS_U32 u32KeyValue = 0;
    // coverity[no_escape]
    while(1)
    {
        MDrv_FrontPnl_ReadKeyPad(&u32KeyValue);

        if(u32KeyValue != 0){
            printf("MDrv_FrontPnl_ReadKeyPad KeyValue = 0x%"DTC_MS_U32_u"\n", u32KeyValue);
        }
        MsOS_DelayTask(2);
        //delay(500);
        #if 1
        MDrv_FrontPnl_Show();
        MsOS_DelayTask(2);
        #endif
    }
}

static void _appDemo_Timer_Task(MS_U32 argc, void *argv)
{
    static MS_U8 u8dot = 0;
    static MS_U8 u8D4 = 0;
    static MS_U8 u8D3 = 0;
    static MS_U8 u8D2 = 0;
    static MS_U8 u8D1 = 0;
    static MS_U8 u8LED = 0;

    MS_U32 u32Events;
    MS_BOOL arLedEnable[3];
    MS_U8 arDigital[4];

    // coverity[no_escape]
    while(1)
    {
        MsOS_WaitEvent(_s32EventId,
                       0x0001,
                       &u32Events,
                       E_OR_CLEAR,
                       MSOS_WAIT_FOREVER);

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

}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Create the app demo task of front panel and initialze all parameters
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_Init \n
///
/// Sample Command: \b FrontPnl_Init
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_Init(void)
{
    MDrv_FrontPnl_Init();

    // create app demo task of input device
    _pTaskStack = MsOS_AllocateMemory( APP_DEMO_FRONTPNL_TASK_STACK, gs32CachedPoolID);
    if (_pTaskStack == NULL)
    {
        printf("_pTaskStack == NULL!!!\n");
        return FALSE;
    }
    _s32TaskId = MsOS_CreateTask(_appDemo_FrontPnl_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pTaskStack,
                                         APP_DEMO_FRONTPNL_TASK_STACK,
                                         "Demo FrontPnl Task");
    if (_s32TaskId < 0)
    {
        printf("MsOS_CreateTask fail!\n");
        return FALSE;
    }
    #if 1
    //Create frontpanel event
    _s32EventId = MsOS_CreateEventGroup("appDemo_Frontpnl_Event");
    if (_s32EventId < 0)
    {
        printf("CreateEventGroup error \n");
        return FALSE;
    }
    //Create Task for timer callback
    _pTimerTaskStack = MsOS_AllocateMemory( APP_DEMO_FRONTPNL_TIMER_TASK_STACK, gs32CachedPoolID);
    if (_pTimerTaskStack == NULL)
    {
        printf("_pTimerTaskStack == NULL!!!\n");
        return FALSE;
    }

    _s32TimerTaskId = MsOS_CreateTask(_appDemo_Timer_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pTimerTaskStack,
                                         APP_DEMO_FRONTPNL_TIMER_TASK_STACK,
                                         "Demo FrontPnl Timer Task");
    if (_s32TimerTaskId < 0)
    {
        printf("MsOS_CreateTask error \n");
        return FALSE;
    }

    //Create timer
    _s32TimerId = MsOS_CreateTimer(appDemo_FrontPnl_TimerCb,500,500,TRUE,"Demo FrontPnl Timer");
    if (_s32TimerTaskId < 0)
    {
        printf("MsOS_CreateTimer error \n");
        return FALSE;
    }

    #endif


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Enable/Disable front panel display
/// @param[in] u32LedRed Enable Red LED
/// @param[in] u32LedGreen Enable Green LED
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_LedEnable \n
///
/// Sample Command: \b FrontPnl_LedEnable 1 1
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_LED_Enable(MS_U32 *u32LedRed,  MS_U32 *u32LedGreen)
{
    MS_BOOL arLedEnable[2];

    arLedEnable[0] = (MS_BOOL)(*u32LedRed);
    arLedEnable[1] = (MS_BOOL)(*u32LedGreen);

    return MDrv_FrontPnl_LED_Enable(arLedEnable,3);

}

//------------------------------------------------------------------------------
/// @brief Enable/Disable all front panel dots
/// @param[in] u32Dot Enable all dots
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_DotEnable \n
///
/// Sample Command: \b FrontPnl_DotEnable 1
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_Dot_Enable(MS_U32 *u32Dot)
{
    MDrv_FrontPnl_Dot_Enable((MS_BOOL)(*u32Dot));
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set digits on the front panel
/// @param[in] u32D4 The fourth digit to be shown
/// @param[in] u32D3 The third digit to be shown
/// @param[in] u32D2 The second digit to be shown
/// @param[in] u32D1 The first digit to be shown
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_DigitalSet \n
///
/// Sample Command: \b FrontPnl_DigitalSet 1 2 3 0
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_Digital_Set(MS_U32 *u32D4, MS_U32 *u32D3, MS_U32 *u32D2, MS_U32 *u32D1)
{
    MS_U8 arDigital[4];

    arDigital[0] = (MS_U8)(*u32D1);
    arDigital[1] = (MS_U8)(*u32D2);
    arDigital[2] = (MS_U8)(*u32D3);
    arDigital[3] = (MS_U8)(*u32D4);

    return MDrv_FrontPnl_Digital_Set(arDigital,4);

}

//------------------------------------------------------------------------------
/// @brief Enable/Disable front panel display
/// @param[in] u32ShowOpen Enable showing digits
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b FrontPnl_ShowEnable \n
///
/// Sample Command: \b FrontPnl_ShowEnable 1
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_FrontPnl_ShowOpen_Enable(MS_U32 *u32ShowOpen)
{
    return MDrv_FrontPnl_Show_Enable((MS_U8)(*u32ShowOpen));
}

#endif
