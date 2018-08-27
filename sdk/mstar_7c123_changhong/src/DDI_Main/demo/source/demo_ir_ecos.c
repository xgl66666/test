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
#if (DEMO_IR_ECOS_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvMMIO.h"
#include "demo_ir_ecos.h"
#include "demo_pm.h"

#include "mdrv_ir.h"
#include "Board.h"
#include "demo_utility.h"
#if (IR_TYPE_SEL == IR_TYPE_MSTAR_DTV)
#include "IR_MSTAR_DTV.h"
#else
#include "IR_MSTAR_DTV.h"
#endif
#if (DEMO_ZUI_TEST == 1)
#include "DataType.h" //For Memory.h
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_OsdId.h"
#include "MApp_ZUI_POP.h"
#include "MApp_ZUI_Utility.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_ACTglobal.h"
#endif
//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_1ST_DELAY_TIME_MS    800
#define IR_2ST_DELAY_TIME_MS    100
#define APPINPUT_TASK_STACK_SIZE      4096
#define IR_RECIEVE_DURATION     100

#define APP_INPUT_EVT_MASK            0x0000000F
#define APP_INPUT_EVT_POWER           0x00000001
#define APP_INPUT_EVT_GENERIC_KEY     0x00000002
#define APP_INPUT_EVT_IN3             0x00000004
#define APP_INPUT_EVT_IN4             0x00000008

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_U8    *u8InputTaskStack = NULL;
static MS_S32   s32InpuTaskId = -1 , s32InpuEventId = -1;
static MS_U32   u32Events = 0;
static MS_U8    u8PreviousKey = 0;
static MS_BOOL  bTaskActive = FALSE;
static MS_BOOL (*_Func_ptr_handlekey)(MS_U32);
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
/*
#if (DEMO_ZUI_TEST == 1)
//#ifdef DDI_MISC_INUSE

typedef struct
{
    MS_U16 u16IRkeyData;
    MS_U8  u8KeyVal;
}IrKeyMapping;

static MS_U8 _IR_ParseKey(MS_U16 u16KeyData)
{
    MS_U8 u8RetVal = KEY_DUMMY;

    const IrKeyMapping stIrKeyMappingArray[] =
    {
        { .u16IRkeyData = IRKEY_TV_RADIO,              .u8KeyVal = KEY_TV_RADIO},
        { .u16IRkeyData = IRKEY_CHANNEL_LIST,          .u8KeyVal = KEY_CHANNEL_LIST },
        { .u16IRkeyData = IRKEY_CHANNEL_FAV_LIST,      .u8KeyVal = KEY_CHANNEL_FAV_LIST},
        { .u16IRkeyData = IRKEY_CHANNEL_RETURN,        .u8KeyVal = KEY_CHANNEL_RETURN},
        { .u16IRkeyData = IRKEY_CHANNEL_PLUS,          .u8KeyVal = KEY_CHANNEL_PLUS},
        { .u16IRkeyData = IRKEY_CHANNEL_MINUS,         .u8KeyVal = KEY_CHANNEL_MINUS},
        { .u16IRkeyData = IRKEY_AUDIO,                 .u8KeyVal = KEY_AUDIO},
        { .u16IRkeyData = IRKEY_VOLUME_PLUS,           .u8KeyVal = KEY_VOLUME_PLUS},
        { .u16IRkeyData = IRKEY_VOLUME_MINUS,          .u8KeyVal = KEY_VOLUME_MINUS},

        { .u16IRkeyData = IRKEY_UP,                    .u8KeyVal = KEY_UP},
        { .u16IRkeyData = IRKEY_POWER,                 .u8KeyVal = KEY_POWER},
        { .u16IRkeyData = IRKEY_EXIT,                  .u8KeyVal = KEY_EXIT},
        { .u16IRkeyData = IRKEY_MENU,                  .u8KeyVal = KEY_MENU},
        { .u16IRkeyData = IRKEY_DOWN,                  .u8KeyVal = KEY_DOWN},
        { .u16IRkeyData = IRKEY_LEFT,                  .u8KeyVal = KEY_LEFT},
        { .u16IRkeyData = IRKEY_SELECT,                .u8KeyVal = KEY_SELECT},
        { .u16IRkeyData = IRKEY_RIGHT,                 .u8KeyVal = KEY_RIGHT},

        { .u16IRkeyData = IRKEY_NUM_0,                 .u8KeyVal = KEY_NUMERIC_0},
        { .u16IRkeyData = IRKEY_NUM_1,                 .u8KeyVal = KEY_NUMERIC_1},
        { .u16IRkeyData = IRKEY_NUM_2,                 .u8KeyVal = KEY_NUMERIC_2},
        { .u16IRkeyData = IRKEY_NUM_3,                 .u8KeyVal = KEY_NUMERIC_3},
        { .u16IRkeyData = IRKEY_NUM_4,                 .u8KeyVal = KEY_NUMERIC_4},
        { .u16IRkeyData = IRKEY_NUM_5,                 .u8KeyVal = KEY_NUMERIC_5},
        { .u16IRkeyData = IRKEY_NUM_6,                 .u8KeyVal = KEY_NUMERIC_6},
        { .u16IRkeyData = IRKEY_NUM_7,                 .u8KeyVal = KEY_NUMERIC_7},
        { .u16IRkeyData = IRKEY_NUM_8,                 .u8KeyVal = KEY_NUMERIC_8},
        { .u16IRkeyData = IRKEY_NUM_9,                 .u8KeyVal = KEY_NUMERIC_9},

        { .u16IRkeyData = IRKEY_MUTE,                  .u8KeyVal = KEY_MUTE},
        { .u16IRkeyData = IRKEY_PAGE_UP,               .u8KeyVal = KEY_PAGE_UP},
        { .u16IRkeyData = IRKEY_PAGE_DOWN,             .u8KeyVal = KEY_PAGE_DOWN},
        { .u16IRkeyData = IRKEY_CLOCK,                 .u8KeyVal = KEY_CLOCK},

        { .u16IRkeyData = IRKEY_INFO,                  .u8KeyVal = KEY_INFO},
        { .u16IRkeyData = IRKEY_RED,                   .u8KeyVal = KEY_RED},
        { .u16IRkeyData = IRKEY_GREEN,                 .u8KeyVal = KEY_GREEN},
        { .u16IRkeyData = IRKEY_YELLOW,                .u8KeyVal = KEY_YELLOW},
        { .u16IRkeyData = IRKEY_BLUE,                  .u8KeyVal = KEY_BLUE},
        { .u16IRkeyData = IRKEY_MTS,                   .u8KeyVal = KEY_MTS},

        { .u16IRkeyData = IRKEY_NINE_LATTICE,          .u8KeyVal = KEY_NINE_LATTICE},
        { .u16IRkeyData = IRKEY_TTX,                   .u8KeyVal = KEY_TTX},
        { .u16IRkeyData = IRKEY_CC,                    .u8KeyVal = KEY_CC},
        { .u16IRkeyData = IRKEY_OUTPUT_SOURCE,         .u8KeyVal = KEY_OUTPUT_SOURCE},
        { .u16IRkeyData = IRKEY_CRADRD,                .u8KeyVal = KEY_CRADRD},

        { .u16IRkeyData = IRKEY_ZOOM,                  .u8KeyVal = KEY_ZOOM},
        { .u16IRkeyData = IRKEY_DASH,                  .u8KeyVal = KEY_DASH},
        { .u16IRkeyData = IRKEY_SLEEP,                 .u8KeyVal = KEY_SLEEP},
        { .u16IRkeyData = IRKEY_EPG,                   .u8KeyVal = KEY_EPG},
        { .u16IRkeyData = IRKEY_PIP,                   .u8KeyVal = KEY_PIP},

        { .u16IRkeyData = IRKEY_MIX,                   .u8KeyVal = KEY_MIX},
        { .u16IRkeyData = IRKEY_INDEX,                 .u8KeyVal = KEY_INDEX},
        { .u16IRkeyData = IRKEY_HOLD,                  .u8KeyVal = KEY_HOLD},

        { .u16IRkeyData = IRKEY_PREVIOUS,              .u8KeyVal = KEY_PREVIOUS},
        { .u16IRkeyData = IRKEY_NEXT,                  .u8KeyVal = KEY_NEXT},

        { .u16IRkeyData = IRKEY_BACKWARD,              .u8KeyVal = KEY_BACKWARD},
        { .u16IRkeyData = IRKEY_FORWARD,               .u8KeyVal = KEY_FORWARD},
        { .u16IRkeyData = IRKEY_PLAY,                  .u8KeyVal = KEY_PLAY},
        { .u16IRkeyData = IRKEY_RECORD,                .u8KeyVal = KEY_RECORD},
        { .u16IRkeyData = IRKEY_STOP,                  .u8KeyVal = KEY_STOP},
        { .u16IRkeyData = IRKEY_PAUSE,                 .u8KeyVal = KEY_PAUSE},

        { .u16IRkeyData = IRKEY_SIZE,                  .u8KeyVal = KEY_SIZE},
        { .u16IRkeyData = IRKEY_REVEAL,                .u8KeyVal = KEY_REVEAL},
        { .u16IRkeyData = IRKEY_SUBCODE,               .u8KeyVal = KEY_SUBCODE},
        { .u16IRkeyData = IRKEY_GUIDE,                 .u8KeyVal = KEY_GUIDE},
        { .u16IRkeyData = IRKEY_PLAYPAUSE,             .u8KeyVal = KEY_PLAYPAUSE},
        { .u16IRkeyData = IRKEY_REPEATAB,              .u8KeyVal = KEY_REPEATAB},
        { .u16IRkeyData = IRKEY_SLOW,                  .u8KeyVal = KEY_SLOW},
        { .u16IRkeyData = IRKEY_STEP,                  .u8KeyVal = KEY_STEP},
        { .u16IRkeyData = IRKEY_SHIFT,                 .u8KeyVal = KEY_SHIFT},
        { .u16IRkeyData = IRKEY_COPY,                  .u8KeyVal = KEY_COPY},
        { .u16IRkeyData = IRKEY_REPEAT,                .u8KeyVal = KEY_REPEAT},
        { .u16IRkeyData = IRKEY_GOTO,                  .u8KeyVal = KEY_GOTO},
        { .u16IRkeyData = IRKEY_DEVICE,                .u8KeyVal = KEY_DEVICE},
        { .u16IRkeyData = IRKEY_SLIDESHOW,             .u8KeyVal = KEY_SLIDESHOW},
        { .u16IRkeyData = IRKEY_THUMBNAIL,             .u8KeyVal = KEY_THUMBNAIL},
        { .u16IRkeyData = IRKEY_TVSYSTEM,              .u8KeyVal = KEY_TVSYSTEM},
        { .u16IRkeyData = IRKEY_ASPECT_RATIO,          .u8KeyVal = KEY_ASPECT_RATIO},
        { .u16IRkeyData = IRKEY_CAPTURE,               .u8KeyVal = KEY_CAPTURE},
    };

    MS_U16 i = 0;
    for (; i < sizeof(stIrKeyMappingArray) / sizeof(IrKeyMapping); i++)
    {
        if (stIrKeyMappingArray[i].u16IRkeyData == u16KeyData)
        {
            u8RetVal = stIrKeyMappingArray[i].u8KeyVal;
            break;
        }
    }

    return u8RetVal;
}
//#endif
#endif
*/
static void _Demo_Input_IRCallback(MS_U8 u8Key, MS_U8 u8RepeatFlag)
{
    printf("u8Key = 0x%02x\n",u8Key);
//#if (DEMO_ZUI_TEST == 1)
//#ifdef DDI_MISC_INUSE
//    u8Key = _IR_ParseKey(u8Key);
//#endif
//#endif

    MsOS_ClearEvent(s32InpuEventId, APP_INPUT_EVT_MASK);
    if (((u8Key == IRKEY_POWER) && (u8RepeatFlag == 0))||((u8Key == u8PreviousKey) && (u8RepeatFlag == 1)))
    {
        MsOS_SetEvent(s32InpuEventId, APP_INPUT_EVT_POWER);
    }
    else
    {
        MsOS_SetEvent(s32InpuEventId, APP_INPUT_EVT_GENERIC_KEY);
    }
#if (DEMO_ZUI_TEST == 1)
    MApp_ZUI_ProcessKey(MApp_ZUI_ACT_MapToVirtualKeyCode(u8Key));
#endif
    u8PreviousKey = u8Key;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/// @brief Create the app demo set customer handle key function.
/// @param[in] func_ptr pointer to a function of key handler
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Sample Command: \b none \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_Input_Set_Handlekey_Func_Ptr(MS_BOOL (*func_ptr)(MS_U32))
{
    _Func_ptr_handlekey = func_ptr;
    return TRUE;
}

static void _Demo_Input_Task(void)
{
    while(bTaskActive)
    {
        MsOS_WaitEvent(s32InpuEventId, APP_INPUT_EVT_POWER | APP_INPUT_EVT_GENERIC_KEY,
                &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        if (_Func_ptr_handlekey)
        {
            //customer key handle
            _Func_ptr_handlekey(u8PreviousKey);
        }
        else
        {
            if (u32Events == APP_INPUT_EVT_POWER)
            {
                printf("Demo_PM_PowerDown\n");
            #if (DEMO_PM_STR_AUTO_IR_DC_OFF == 1)
                MsOS_DelayTask(200);
                Demo_PM_STR();
                Demo_Input_Init_ecos();

            #endif
            }
        }

        MsOS_DelayTask(IR_RECIEVE_DURATION);
        u32Events = 0;
    }
}

static void _Demo_Input_FreeInputResource(void)
{
    MS_BOOL bRet = FALSE;

    if (s32InpuEventId > 0)
    {
        bRet = MsOS_DeleteEventGroup(s32InpuEventId);
        s32InpuEventId = -1;
        printf("[%s][%d] bRet %d \n",__FUNCTION__,__LINE__,bRet);
    }

    if (u8InputTaskStack != NULL)
    {
        MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        bRet = MsOS_FreeMemory(u8InputTaskStack,s32MstarNonCachedPoolID);
        u8InputTaskStack = MsOS_AllocateMemory(APPINPUT_TASK_STACK_SIZE,s32MstarNonCachedPoolID);
        u8InputTaskStack = NULL;
        printf("[%s][%d] bRet %d \n",__FUNCTION__,__LINE__,bRet);
    }

    // Delete Input demo task
    if (s32InpuTaskId > 0)
    {
        bTaskActive = FALSE;
        s32InpuTaskId = -1;
        printf("[%s][%d] bRet %d \n",__FUNCTION__,__LINE__,bRet);
    }

    //delete customner key handler
    _Func_ptr_handlekey = NULL;

}

//------------------------------------------------------------------------------
/// @brief Create the app demo task of IR input device and initialze all parameters.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b IR_Init \n
///
/// Sample Command: \b IR_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_Input_Init_ecos(void)
{
  #if 0
    if ((s32InpuEventId > 0)||(s32InpuTaskId > 0)||(u8InputTaskStack != NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
 #endif
    MDrv_IR_HK_Init();

    MDrv_IR_HK_SetCallback(_Demo_Input_IRCallback);

    if (s32InpuEventId < 0)
    {
        s32InpuEventId = MsOS_CreateEventGroup("appInput_Event");
        if (s32InpuEventId > 0)
        {
            printf("[%s][%d] Event create ok\n",__FUNCTION__,__LINE__);
        }
        else
        {
            printf("[%s][%d] create failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }

    if (u8InputTaskStack == NULL)
    {
        MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        u8InputTaskStack = MsOS_AllocateMemory(APPINPUT_TASK_STACK_SIZE,s32MstarNonCachedPoolID);
        if (u8InputTaskStack !=  NULL)
        {
            printf("[%s][%d] Memory create ok\n",__FUNCTION__,__LINE__);
        }
        else
        {
            _Demo_Input_FreeInputResource();
            printf("[%s][%d] create failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }

    // Create Input demo task
    if (s32InpuTaskId < 0)
    {
        bTaskActive = TRUE;
        s32InpuTaskId = MsOS_CreateTask( (TaskEntry) _Demo_Input_Task,
                                      (MS_U32)NULL,
                                      E_TASK_PRI_HIGH,
                                      TRUE,
                                      u8InputTaskStack,
                                      APPINPUT_TASK_STACK_SIZE,
                                      "appInput_Task" );
        if (s32InpuTaskId > 0)
        {
            printf("[%s][%d] Task create ok\n",__FUNCTION__,__LINE__);
        }
        else
        {
            printf("[%s][%d] create failed \n",__FUNCTION__,__LINE__);
            _Demo_Input_FreeInputResource();
            return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief close the app demo task of IR input device and initialze all parameters.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b IR_Exit \n
///
/// Sample Command: \b IR_Init \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_Input_Exit_ecos(void)
{
    MDrv_IR_HK_Enable(FALSE);

    _Demo_Input_FreeInputResource();
    return TRUE;
}



//------------------------------------------------------------------------------
/// @brief The IR User input demo help.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b HelpIR \n
///
/// Sample Command: \b HelpIR \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_Input_Help_ecos(void)
{

    printf ("------------------------------------IR Help--------------------------------------\n");
    printf ("press \"IR_Init\" to Initial IR\n");
    printf ("---------------------------------End of IR Help----------------------------------\n");


    return TRUE;
}

#endif
