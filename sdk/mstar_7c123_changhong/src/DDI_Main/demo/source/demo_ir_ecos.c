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

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _Demo_Input_IRCallback(MS_U8 u8Key, MS_U8 u8RepeatFlag)
{
    printf("u8Key = 0x%02x\n",u8Key);
    MsOS_ClearEvent(s32InpuEventId, APP_INPUT_EVT_MASK);
    if (((u8Key == IRKEY_POWER) && (u8RepeatFlag == 0))||((u8Key == u8PreviousKey) && (u8RepeatFlag == 1)))
    {
        MsOS_SetEvent(s32InpuEventId, APP_INPUT_EVT_POWER);
    }
    else
    {
        MsOS_SetEvent(s32InpuEventId, APP_INPUT_EVT_GENERIC_KEY);
    }
    u8PreviousKey = u8Key;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
static void _Demo_Input_Task(void)
{
    while(bTaskActive)
    {
        MsOS_WaitEvent(s32InpuEventId, APP_INPUT_EVT_POWER | APP_INPUT_EVT_GENERIC_KEY,
                &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);


		if (u32Events == APP_INPUT_EVT_POWER)
		{
		    //Demo_PM_PowerDown();
		    printf("Demo_PM_PowerDown\n");
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

    // Create Input demo task
    if (s32InpuTaskId > 0)
    {
        bTaskActive = FALSE;
        s32InpuTaskId = -1;
        printf("[%s][%d] bRet %d \n",__FUNCTION__,__LINE__,bRet);
    }
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
    if ((s32InpuEventId > 0)||(s32InpuTaskId > 0)||(u8InputTaskStack != NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

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
