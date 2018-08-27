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
#if (DEMO_ZUI_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>

#include "MsMemory.h"
#include "MsCommon.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "apiGFX.h"
#include "apiGOP.h"
#include "apiPNL.h"
#include "apiVDEC_EX.h"
#include "apiXC.h"
#include "drvDTC.h"
#include "drvSEM.h"
//#include "apiVectorFont.h"
#include "demo_utility.h"
#include "demo_ui.h"
#include "demo_av.h"
#include "demo_vdec.h"

#include "DataType.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_OsdId.h"
#include "MApp_ZUI_POP.h"
#include "MApp_ZUI_Utility.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_ACTglobal.h"
#include "MSTAR_strings_enum.h"
#include "OSDcp_readbin.h"
#include "MApp_ZUI_Resource.h"
#include "apiVectorFont.h"
#include "SysBsp.h"
#include "MApp_ZUI_GDISetup.h"
//-------------------------------------------------------------------------------------------------
//  Debug Msg
//-------------------------------------------------------------------------------------------------

#define ZUI_DBG 0
#if ZUI_DBG
#define DemoZui_print(fmt, args...)   { printf("[DemoZUI][%06d]\t", __LINE__); printf( fmt, ## args); }
#else
#define DemoZui_print(fmt, args...)
#endif

#define ZUI_EVENT_DBG 0

#if ZUI_EVENT_DBG
#define DemoZuiEvent_print(fmt, args...)  { printf("[DemoZUI][%s][%06d]    ",__FUNCTION__, __LINE__); printf( fmt, ## args); }
#else
#define DemoZuiEvent_print(fmt, args...)
#endif

#define ZUI_ERROR 1
#if ZUI_ERROR
#define DemoZui_Errorprint(fmt, args...)   { printf("[DemoZUI][ERROR][%s][%06d]\t",__FUNCTION__,__LINE__); printf( fmt, ## args); }
#else
#define DemoZui_Errorprint(fmt, args...)
#endif

/*=====================================================================================*/
/* User-defined assert */
/*=====================================================================================*/
#define UI_ASSERT(_bool_)                                                                                   \
        {                                                                                                   \
            if(!( _bool_ ))                                                                                 \
            {                                                                                               \
               printf("UI ASSERT: %s, %s %s %d\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);       \
               return;                                                                                      \
            }                                                                                               \
        }

#define UI_ASSERT_RETURN(_bool_,_expr_)                                                                     \
        {                                                                                                   \
            if(!( _bool_ ))                                                                                 \
            {                                                                                               \
               printf("UI ASSERT: %s, %s %s %d\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);       \
               return(_expr_);                                                                              \
            }                                                                                               \
        }

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_ECOS
#define TASK_STACK_SIZE                 4096
#endif
#define INVALID_TASK_ID                 -1
#define INVALID_EVENT_ID                -1
#define INVALID_MUTEX_ID                -1
#define INVALID_POOL_ID                 -1
#define MUTEX_TIMEOUT                   MSOS_WAIT_FOREVER
#define TASK_DELAY_TIME                 100 //ms

/*define event*/
#define DEMO_UI_CMD_READY               0x1
#define DEMO_UI_CMD_END                 0x2
#define DEMO_UI_CMD_EXIT                0x4
#define DEMO_UI_RELEASE                 0x8
#define DEMO_UI_CMD_ALL                 (DEMO_CMD_READY | DEMO_CMD_END)
#define DEMO_EVENT_WAIT_TIME            100
#define DEMO_EVENT_WAIT_FOREVER         0xffffffff
#define MAX_SUPPORT_LIST_DEPTH (16)

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_ECOS
static void *_pTaskStack = NULL;
#endif
static MS_S32 _s32TaskId = INVALID_TASK_ID;
static MS_BOOL bInitFirst = FALSE;
static MS_BOOL bUIInit = FALSE;
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief UI initialization function.
/// @param[in] None.
/// @return TRUE - Success
/// @return FALSE - Failure
//------------------------------------------------------------------------------
static MS_BOOL Demo_UI_Init(void)
{
    if(!bInitFirst)
    {
        _s32TaskId       = INVALID_TASK_ID;         // UI task index.
#ifdef MSOS_TYPE_ECOS
        _pTaskStack      = NULL;                    // UI task stack memory pointer.
#endif

        MApi_VectorFont_Init();                     // Init the MVF Font.
        printf("Init VectorFont Done!\n");
        MApp_ZUI_DeInit();
        MApp_ZUI_ACT_SetupResource();               // prepare the UI resources
        MApp_ZUI_Init(HEX);                         // Init the ZUI
        printf("Init ZUI Done!\n");

        MApp_ZUI_API_SetLanguageIdx(ENGLISH);       // set Default OSD Language == English
        MApp_ZUI_OSD_VfConfigUnknowGlyph(0x7F);     //0x7F is ' ' and 0x3F instead of '?'
        printf("Setup OSD Language!\n");
        bInitFirst = TRUE;
    }
    return TRUE;
}

static void _Demo_UI_Task(void)
{
    MS_BOOL bRetVal = FALSE;
    while (1)
    {
        bRetVal = MApp_ZUI_MainTask(); // call this will trigger to MApp_ZUI_ACT_HandleGlobalKey()

        switch (bRetVal)
        {
            case RET_ZUI_EXIT:
            {
                printf("[%s][%d] EXIT ZUI OK\n",__FUNCTION__,__LINE__);
                break;
            }

            case RET_ZUI_RUNNING:
            {
                //MApp_ZUI_timeout_handler(); // this state do nothing now. not need to test item.
                if(bInitFirst)
                {
                    MApp_ZUI_InitBootOsd(); // To startup MainMenu OSD.
                    bInitFirst = FALSE;
                }
                break;
            }

            default:
                break;
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// @brief Create UI Task
/// @return TRUE: Task create success.
/// @return FALSE: Task create fail.
/// @sa
/// @note
/// Command: \b none \n
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_UI_Create(void)
{
    if(!bUIInit)
    {
        Demo_UI_Init();

#ifdef MSOS_TYPE_ECOS
        MS_BOOL bRet = FALSE;
        MS_S32 s32NonCachedPoolID = INVALID_POOL_ID;
        bRet = Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);

        if(!bRet)
        {
            printf("Get NonCachedPoolID failed!\n");
            return FALSE;
        }

        /* Create task for UI */
        if(_pTaskStack == NULL)
        {
            _pTaskStack = MsOS_AllocateMemory(TASK_STACK_SIZE,s32NonCachedPoolID);
            printf("[%s][%d] MsOS_AllocateMemory OK\n",__FUNCTION__,__LINE__);
        }
        UI_ASSERT_RETURN(_pTaskStack != NULL,FALSE)

        if(_s32TaskId == INVALID_TASK_ID)
        {
            _s32TaskId = MsOS_CreateTask((TaskEntry) _Demo_UI_Task,
                                   (MS_U32)NULL,
                                   E_TASK_PRI_MEDIUM,
                                   TRUE,
                                   _pTaskStack,
                                   TASK_STACK_SIZE,
                                   "UI_TASK");
        }
#else
        /*linux case*/
        if(_s32TaskId == INVALID_TASK_ID)
        {
            _s32TaskId = MsOS_CreateTask((TaskEntry) _Demo_UI_Task,
                                   (MS_U32)NULL,
                                   E_TASK_PRI_MEDIUM,
                                   TRUE,
                                   NULL,
                                   0,
                                   "UI_TASK");
        }
#endif
        UI_ASSERT_RETURN(_s32TaskId != INVALID_TASK_ID,FALSE)

        bUIInit = TRUE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief UI exit function.
/// @param[in] None.
/// @return TRUE - Success
/// @return FALSE - Failure
//------------------------------------------------------------------------------
MS_BOOL Demo_UI_Exit(void)
{
    MApp_ZUI_SetState(E_ZUI_STATE_TERMINATE);
    MApp_ZUI_CloseGDI();
    MApp_ZUI_CloseDisplayGDI();

#ifdef MSOS_TYPE_ECOS
    MS_BOOL bRet = TRUE;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    if (_pTaskStack != NULL)
    {
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        bRet = MsOS_FreeMemory(_pTaskStack,s32MstarNonCachedPoolID);
        _pTaskStack = MsOS_AllocateMemory(TASK_STACK_SIZE,s32MstarNonCachedPoolID);
        _pTaskStack = NULL;
    }
#endif

    if (_s32TaskId > 0)
    {
        _s32TaskId = INVALID_TASK_ID;
        bUIInit = FALSE;
    }
    return TRUE;
}

#endif

#if(DEMO_AUTOIN_ZUI_TEST == 1)
MS_U8 Demo_UI_AutoIn(void)
{
    MS_BOOL bCmdPass;
    MS_U32  u32XCDevice;
    MS_U32  u32AudioType;

    MS_U32 u32HD_GOP = 0;
    MS_U32 u32SD_GOP = 1;
    MS_U32 u32HD_DST = 0;
    MS_U32 u32SD_DST = 0;

    do
    {
        u32XCDevice = 0;
        for( u32XCDevice = 0; u32XCDevice < 2 ; u32XCDevice++ )
        {
            bCmdPass = Demo_XC_Init(&u32XCDevice);      // xc_init 0 & xc_init 1
            if( bCmdPass == FALSE )
            {
                 printf("XC_Init %d >> fail\n",u32XCDevice);
                 break;
            }
        }
        if( bCmdPass == FALSE )break;

        bCmdPass = Demo_VE_Init();                              // ve_init
        if( bCmdPass == FALSE )
        {
            printf("VE_Init >> fail\n");
            break;
        }

        bCmdPass = Demo_CEC_init();                             //cec_init
        if( bCmdPass == FALSE )
        {
            printf("VE_Init >> fail\n");
            break;
        }

        u32AudioType = 1;
        bCmdPass = Demo_Audio_Init(&u32AudioType);  //Audio_Init 1
        if( bCmdPass == FALSE )
        {
            printf("Audio_Init %d >> fail\n",u32AudioType);
            break;
        }

        bCmdPass = Demo_Audio_SetMonitor();             // Audio_Monitor
         if( bCmdPass == FALSE )
        {
            printf("Audio_Monitor >> fail\n");
            break;
        }

        bCmdPass = Demo_MM_Init();                              // MM_INIT
        if( bCmdPass == FALSE )
        {
            printf("MM_INIT >> fail\n");
            break;
        }

        bCmdPass = Demo_UI_GEGOPInit(&u32HD_GOP, &u32SD_GOP, &u32HD_DST, &u32SD_DST);
        if( bCmdPass == FALSE )
        {
            printf("UI_GEGOPInit 0 1 0 0 >> fail\n");
            break;
        }

        bCmdPass = Demo_Input_Init_linux();
        if( bCmdPass == FALSE )
        {
            printf("IR_Init >> fail\n");
            break;
        }

        bCmdPass = Demo_UI_Create();
        if( bCmdPass == FALSE )
        {
            printf("UI_Init >> fail\n");
            break;
        }
    }
    while(bCmdPass == FALSE );

    return bCmdPass;
}
#endif
