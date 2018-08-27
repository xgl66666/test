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
// (“MStar Confidential Information”) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_VDEC_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"
#include "MsFS.h"

#include "apiDMX.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiXC.h"
#include "apiGOP.h"
#include "apiGFX.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#endif
#include "drvMVOP.h"
#include "drvDTC.h"


#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif

#if (DEMO_XC_DIPMultiWin_TEST == 1)
#include "msAPI_GEGOP.h"
#endif

#include "demo_xc.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_utility.h"
#include "demo_dip.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/// VDEC check device ID
#if (DEMO_VDEC_MULTI_TEST == 1 || DEMO_VDEC_NDECODE_TEST == 1)
#define DEMO_VDEC_CHECK_DEVICE_ID(u8Id)                            \
    if ((EN_VDEC_Device)u8Id >= E_VDEC_DEVICE_MAX)        \
    {                                                             \
        printf("[%s][%d] Wrong Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#else
#define DEMO_VDEC_CHECK_DEVICE_ID(u8Id)                            \
    if ((EN_VDEC_Device)u8Id > 0)        \
    {                                                             \
        printf("[%s][%d] Not support vdec multi Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#endif

//-------------------------------------------------------------------------------------------------
// Global Definition
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define VDEC_DBG                0
#define VDEC_NdecodeDBG         0
#define VDEC_FRAMECOUNT_LOOP    300
//#define E_HVD_GDATA_DEC_Q_NUMB 0x1307
//#define E_HVD_GDATA_DISP_Q_NUMB 0x1308

#define INVALID_TASK_ID     -1
#define INVALID_EVENT_ID    -1
#define INVALID_PTS         -1

#define TASK_STACK_SIZE             0x2000
#define PLAYLOOP_PUSH_SIZE          0x180000

#if (DEMO_DTV_DS_TEST == 1)
#if (DEMO_4K2K_TEST == 1)
#define DYNAMIC_SCALING_VB_H 4096
#define DYNAMIC_SCALING_VB_V 2160
#else
#define DYNAMIC_SCALING_VB_H 1920
#define DYNAMIC_SCALING_VB_V 1088
#endif
#endif

#if defined(DEMO_DTV_HDR_VER_TEST) && (DEMO_DTV_HDR_VER_TEST >= 1)
#ifdef UFO_XC_HDR
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
#define DTV_HDR_VDEC_FLOW
#endif
#endif
#endif

#if VDEC_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)
#endif

#if VDEC_NdecodeDBG
#define NdecodeDBG_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define NdecodeDBG_print(fmt, args...)  while(0);
#endif


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
void *pStackbase = NULL;
static Task_Info PlayLoop_Task[E_VDEC_DEVICE_MAX] =
{
    {-1, E_TASK_PRI_MEDIUM, NULL, TASK_STACK_SIZE, "threadEvt1"},
    {-1, E_TASK_PRI_MEDIUM, NULL, TASK_STACK_SIZE, "threadEvt2"},
    //{-1, E_TASK_PRI_MEDIUM, NULL, TASK_STACK_SIZE, "threadEvt3"},
    //{-1, E_TASK_PRI_MEDIUM, NULL, TASK_STACK_SIZE, "threadEvt4"},
};

static MS_BOOL bVDEC_WaitAVSync = TRUE;
static MS_BOOL bPlayLoopActive = FALSE;
static MS_U8 u8VDECDbgMsg = 0;
static MS_U32 u32VDECDbgIntervalTime = 1000;
static MS_U32 u32VDEC_DelayTime = 10;
static MS_S32 s32_VDECDbgTaskID = INVALID_TASK_ID;
static MS_U32 u32VDECDbgCnt = 0;

static FILE *fpPush = NULL;
static FILE *fpSync = NULL;
static FILE *fpCallBackDBG = NULL;
static char Pushoutfilename[256];
static char Syncoutfilename[256];
static char CallBackDBGoutfilename[256];

#if(DEMO_VDEC_NDECODE_TEST == 0)
static VDEC_StreamId stNullVDECStreamID = {0,0};
#endif

static VDEC_EX_DbgLevel eVDECDebuglevel = E_VDEC_EX_DBG_LEVEL_NONE;

static ST_VDEC_INFORMATION stVDECInfo[E_VDEC_DEVICE_MAX];
static ST_VDEC_DIP_DispInfo stVDECDIPInfo[VDEC_MAX_SUPPORT_STREAM_NUM];

#if (DEMO_XC_DIPMultiWin_TEST == 1)
static ST_VDEC_DIP_FrameController stVDECDIPFrameController = {{-1, E_TASK_PRI_HIGH, NULL, 0, "GetFrameTask"},E_VDEC_NotInital,{-1, E_TASK_PRI_HIGH, NULL, 0, "RelFrameTask"},E_VDEC_NotInital};
#endif

//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief file play task
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_VDEC_PlayLoopTask(MS_U32 u32indexId)
{
    MS_BOOL bStart = FALSE;
    MS_BOOL bEnableSecondStream = FALSE;
    MS_U32 u32EntryNumber = 0;
    MS_U32 u32EntryNumberStart = 0;
    MS_U32 u32PushTime = 20;
    MS_U32 u32TotalSize = 0;
    MS_U32 u32TotalSize2 = 0;

    VDEC_EX_DecCmd stDecCmd;
    VDEC_EX_DecCmd stDecCmd2;
    EN_VDEC_Device eDevice = u32indexId;

    VDEC_EX_DispFrame *pstDispFrame=NULL;

    memset(&stDecCmd, 0, sizeof(VDEC_EX_DecCmd));
    memset(&stDecCmd2, 0, sizeof(VDEC_EX_DecCmd));

    bPlayLoopActive = TRUE;
    while(1)
    {

#if(DEMO_MVC_TEST == 1)
        if (stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize2 != 0)
        {
            bEnableSecondStream = TRUE;
        }
#endif

        if (bStart == FALSE)
        {
            bStart = TRUE;
            if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
            || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
            || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
            || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
            || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
            {
                if (stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize > PLAYLOOP_PUSH_SIZE)
                {
                    u32TotalSize = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize - PLAYLOOP_PUSH_SIZE;
                    stDecCmd.u32StAddr = DRV_PROC_SIZE;
                }
                else
                {
                    u32TotalSize = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize;
                }

                if (bEnableSecondStream)
                {
                    if (stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize2 > PLAYLOOP_PUSH_SIZE)
                    {
                        u32TotalSize2 = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize2 - PLAYLOOP_PUSH_SIZE;
                        stDecCmd2.u32StAddr = stVDECInfo[eDevice].stFilePlayInfo.u32Address2;
                    }
                    else
                    {
                        u32TotalSize2 = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize2;
                    }
                }
            }
            else if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY)
            {
                stDecCmd.u32StAddr = DRV_PROC_SIZE;
                u32TotalSize = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize;
            }
            u32EntryNumberStart = MApi_VDEC_EX_GetDecQVacancy(&(stVDECInfo[eDevice].stVDECSteamID));
        }

        if (1)// main
        {
            u32EntryNumber = MApi_VDEC_EX_GetDecQVacancy(&(stVDECInfo[eDevice].stVDECSteamID));

            if ( (u32EntryNumberStart - u32EntryNumber) <= 4)
            {
                u32PushTime = 4;
                while (u32PushTime--)
                {
                    if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
                    || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
                    || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
                    || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
                    || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
                    {
                        if (u32TotalSize == stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize)
                        {
                            if (u32TotalSize > PLAYLOOP_PUSH_SIZE)
                            {
                                stDecCmd.u32StAddr = DRV_PROC_SIZE;
                                stDecCmd.u32Size = PLAYLOOP_PUSH_SIZE;
                                u32TotalSize = u32TotalSize - PLAYLOOP_PUSH_SIZE;
                            }
                            else
                            {
                                stDecCmd.u32StAddr = DRV_PROC_SIZE;
                                stDecCmd.u32Size = u32TotalSize;
                            }
                        }
                        else
                        {
                            if (u32TotalSize > PLAYLOOP_PUSH_SIZE)
                            {
                                stDecCmd.u32StAddr = stDecCmd.u32StAddr + PLAYLOOP_PUSH_SIZE;
                                stDecCmd.u32Size = PLAYLOOP_PUSH_SIZE;
                                u32TotalSize = u32TotalSize - PLAYLOOP_PUSH_SIZE;
                            }
                            else
                            {
                                stDecCmd.u32StAddr = stDecCmd.u32StAddr + PLAYLOOP_PUSH_SIZE;
                                stDecCmd.u32Size = u32TotalSize;
                                u32TotalSize = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize;
                            }
                        }
                        stDecCmd.u32ID_H = 0;
                        stDecCmd.u32ID_L = 0;
                        stDecCmd.u32Timestamp = 0;

                        if (bEnableSecondStream)
                        {
                            if (u32TotalSize2 == stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize2)
                            {
                                if (u32TotalSize2 > PLAYLOOP_PUSH_SIZE)
                                {
                                    stDecCmd2.u32StAddr = stVDECInfo[eDevice].stFilePlayInfo.u32Address2;
                                    stDecCmd2.u32Size = PLAYLOOP_PUSH_SIZE;
                                    u32TotalSize2 = u32TotalSize2 - PLAYLOOP_PUSH_SIZE;
                                }
                                else
                                {
                                    stDecCmd2.u32StAddr = stVDECInfo[eDevice].stFilePlayInfo.u32Address2;
                                    stDecCmd2.u32Size = u32TotalSize2;
                                }
                            }
                            else
                            {
                                if (u32TotalSize2 > PLAYLOOP_PUSH_SIZE)
                                {
                                    stDecCmd2.u32StAddr = stDecCmd2.u32StAddr + PLAYLOOP_PUSH_SIZE;
                                    stDecCmd2.u32Size = PLAYLOOP_PUSH_SIZE;
                                    u32TotalSize2 = u32TotalSize2 - PLAYLOOP_PUSH_SIZE;
                                }
                                else
                                {
                                    stDecCmd2.u32StAddr = stDecCmd2.u32StAddr + PLAYLOOP_PUSH_SIZE;
                                    stDecCmd2.u32Size = u32TotalSize2;
                                    u32TotalSize2 = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize2;
                                }
                             }
                        }
                        stDecCmd2.u32ID_H = 0;
                        stDecCmd2.u32ID_L = 0;
                        stDecCmd2.u32Timestamp = 0;

                    }
                    else if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY)
                    {
                        stDecCmd.u32StAddr = DRV_PROC_SIZE;
                        stDecCmd.u32Size = stVDECInfo[eDevice].stFilePlayInfo.u32PlaySize;
                        stDecCmd.u32ID_H = 0;
                        stDecCmd.u32ID_L = 0;
                        stDecCmd.u32Timestamp = 0;
                    }

                    if (MApi_VDEC_EX_PushDecQ(&(stVDECInfo[eDevice].stVDECSteamID),&stDecCmd)!= E_VDEC_EX_OK)
                    {
                        printf("[%s][%d] MApi_VDEC_EX_PushDecQ return : %d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_PushDecQ(&(stVDECInfo[eDevice].stVDECSteamID),&stDecCmd));
                    }
                    if (MApi_VDEC_EX_FireDecCmd(&(stVDECInfo[eDevice].stVDECSteamID)) != E_VDEC_EX_OK)
                    {
                        printf("[%s][%d] MApi_VDEC_EX_FireDecCmd return : %d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_FireDecCmd(&(stVDECInfo[eDevice].stVDECSteamID)));
                    }

                    if (bEnableSecondStream)
                    {
                        if (MApi_VDEC_EX_SetControl(&(stVDECInfo[eDevice].stVDECSteamID),E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET, (MS_U32)&stDecCmd2) != E_VDEC_EX_OK)
                        {
                            printf("[%s][%d] MApi_VDEC_EX_SetControl E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET return : %d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_SetControl(&(stVDECInfo[eDevice].stVDECSteamID),E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET, (MS_U32)&stDecCmd2));
                            return;
                        }
                        if (MApi_VDEC_EX_SetControl(&(stVDECInfo[eDevice].stVDECSteamID),E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD, (MS_U32)&stDecCmd2) != E_VDEC_EX_OK)
                        {
                            printf("[%s][%d] MApi_VDEC_EX_SetControl E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD return : %d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_SetControl(&(stVDECInfo[eDevice].stVDECSteamID),E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD, (MS_U32)&stDecCmd2));
                            return;
                        }
                    }
                    MsOS_FlushMemory();
                    MsOS_DelayTask(u32VDEC_DelayTime);
                }
            }

            MsOS_DelayTask(u32VDEC_DelayTime);


            if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_SWDetile_MODE)
            {
                if(MApi_VDEC_EX_GetNextDispFrame(&stVDECInfo[eDevice].stVDECSteamID,&pstDispFrame) != E_VDEC_EX_OK)
                {
                    printf("MApi_VDEC_EX_GetNextDispFrame FAIL!!!!!\n");
                }
                else
                {
                    MS_U8 u8GOPNum;
                    VDEC_EX_FrameInfo stFrmInfo;

                    memset(&stFrmInfo, 0, sizeof(VDEC_EX_FrameInfo));

                    u8GOPNum = 0;
                    stFrmInfo = pstDispFrame->stFrmInfo;

                    while(1)
                    {
                        if(Demo_VDEC_SW_DeTile(&eDevice, &u8GOPNum, &stFrmInfo) == TRUE)
                        {
                            if(MApi_VDEC_EX_ReleaseFrame(&stVDECInfo[eDevice].stVDECSteamID, pstDispFrame) == E_VDEC_EX_OK)
                                break;
                            else
                                printf("MApi_VDEC_EX_ReleaseFrame FAIL!!!\n");
                        }
                    }
                }
            }
            else if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
            {
#if (DEMO_XC_DIPMultiWin_TEST == 1)
                Demo_VDEC_DIP_FrameController(&stVDECInfo[eDevice].stVDECSteamID,&eDevice);
#endif
            }
        }
    }
}

//------------------------------------------------------------------------------
/// @brief VDEC Call back function
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_VDEC_Callback(MS_U32 u32Event, void *param)
{
#if(DEMO_XC_DIPMultiWin_TEST == 1)
    EN_VDEC_Device eDevice = 0;
    if(Demo_VDEC_GetMode(&eDevice,E_VDEC_GetCmd_GetDevice_ByStreamID,param) == FALSE)
        printf("\033[31m [%s][%d]Demo_VDEC_Callback get device fail by streamid\033[0m\n",__FUNCTION__,__LINE__);
#endif

    if (u32Event & E_VDEC_EX_EVENT_DISP_INFO_CHG)
    {
        printf("\n[0x%"DTC_MS_U32_x"] E_VDEC_EVENT_DISP_INFO_CHG !!!\n",((VDEC_StreamId*)param)->u32Id);

        MApi_VDEC_EX_SetBlueScreen(param, FALSE);

#if(DEMO_XC_DIPMultiWin_TEST == 1)
        if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
        {
            VDEC_EX_DispInfo stDispinfo;
            memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
            if(MApi_VDEC_EX_GetDispInfo(param,&stDispinfo) == E_VDEC_EX_OK)
            {
                if(stDispinfo.u32FrameRate != stVDECDIPInfo[eDevice].u32FrameRate)
                {
                    printf("[%s][%d]FrameRate Change old[%"DTC_MS_U32_d"] NEW[%"DTC_MS_U32_d"]\n",__FUNCTION__,__LINE__,stVDECDIPInfo[eDevice].u32FrameRate,stDispinfo.u32FrameRate);
                    stVDECDIPInfo[eDevice].u32FrameRate = stDispinfo.u32FrameRate;

                    if(stVDECDIPInfo[eDevice].stDIPFrameSyncInfo.u16InputTolerance == 0)
                    {
                        stVDECDIPInfo[eDevice].stDIPFrameSyncInfo.u16Tolerance = (float)(1/(float)(stVDECDIPInfo[eDevice].u32FrameRate/1000)*1000); //ms
                        printf("[%s][%d]New Tolerance[%d]\n",__FUNCTION__,__LINE__,stVDECDIPInfo[eDevice].stDIPFrameSyncInfo.u16Tolerance);
                    }
                }
            }
        }
#endif
    }

    MS_U32 u32BufferSize = 0;

#if(DEMO_VDEC_NDECODE_TEST == 1)
    VDEC_EX_BufferInfo parms;
    memset(&parms, 0 , sizeof(VDEC_EX_BufferInfo));

    if(Demo_VDEC_GetMode(&eDevice,E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS,&parms))
    {
        u32BufferSize  = parms.szSize;
    }
#else
    u32BufferSize = VDEC_BIT_STREAM_LEN;
#endif

    if (u32Event & E_VDEC_EX_EVENT_DISP_ONE)
    {
        MS_U32 u32VESlevel = 0;
        MS_U32 u32VWptr = 0;
        MS_U32 u32VRptr = 0;
        MS_U32 u32stc32 = 0;
        MS_U32 u32stc = 0;
        MS_U32 u32pcr32 = 0;
        MS_U32 u32pcr = 0;
        MS_U32 u32STCEng = 0;
        MS_U32 u32MvopBank = 0;
        MS_U8 u8Field = 0;
        MS_U8 u8Fdmask = 0;
        MS_U8 u8Streamidx = (MS_U8)(((VDEC_StreamId*)param)->u32Id >> 24);

        u32STCEng = stVDECInfo[u8Streamidx].u32STCEng;

        u32VWptr = MApi_VDEC_EX_GetESWritePtr(param);
        u32VRptr = MApi_VDEC_EX_GetESReadPtr(param);

        if (u32VWptr>=u32VRptr)
        {
            u32VESlevel = u32VWptr - u32VRptr;
        }
        else
        {
            u32VESlevel = u32BufferSize - ( u32VRptr - u32VWptr );
        }

        MApi_DMX_Stc_Eng_Get(u32STCEng,&u32stc32, &u32stc);
        MApi_DMX_Pcr_Eng_Get(u32STCEng,&u32pcr32, &u32pcr);

#if(DEMO_XC_DIPMultiWin_TEST == 1)
        if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_MVOP_MODE)
#endif
        {
            if (u8Streamidx == 0 )
                u32MvopBank = 0x101400;
            else if (u8Streamidx == 1 )
                u32MvopBank = 0x103D00;

            MApi_XC_Write2ByteMask((u32MvopBank | 0x5E), 0x4000, 0xE000);//RIU debug register select = 2

            u8Field  = (MApi_XC_ReadByte((u32MvopBank | 0x5D)) & 0x10)>>4;
            u8Fdmask = (MApi_XC_ReadByte((u32MvopBank | 0x5D)) & 0x20)>>5;
        }

        if(fpCallBackDBG)
        {
            fprintf(fpCallBackDBG,"[%d] systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\n",
                u8Streamidx,MsOS_GetSystemTime(),MApi_VDEC_EX_GetFrameCnt(param),MApi_VDEC_EX_GetDispCnt(param),MApi_VDEC_EX_GetErrCnt(param),
                MApi_VDEC_EX_GetSkipCnt(param),MApi_VDEC_EX_GetDropCnt(param),u32pcr / 90,u32stc / 90,MApi_VDEC_EX_GetPTS(param),
                (MApi_VDEC_EX_GetPTS(param)- (u32stc / 90)),u32VESlevel,u32VWptr,u32VRptr);
            MsFS_Fflush(fpCallBackDBG);
        }
        else
        {
            printf("[%d] systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",[Field]%u ,[FDMASK]%u ,Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\n",
            u8Streamidx,MsOS_GetSystemTime(),MApi_VDEC_EX_GetFrameCnt(param),MApi_VDEC_EX_GetDispCnt(param),MApi_VDEC_EX_GetErrCnt(param),
            MApi_VDEC_EX_GetSkipCnt(param),MApi_VDEC_EX_GetDropCnt(param),u32pcr / 90,u32stc / 90,MApi_VDEC_EX_GetPTS(param),u8Field,u8Fdmask,
            (MApi_VDEC_EX_GetPTS(param)- (u32stc / 90)),u32VESlevel,u32VWptr,u32VRptr);
        }

     }
}

//------------------------------------------------------------------------------
/// @brief VDEC DBG MSG Task
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_VDEC_DBGMSG(void)
{
    MS_U32 u32VESlevel = 0;
    MS_U32 u32VWptr = 0;
    MS_U32 u32VRptr = 0;
    MS_U32 u32BufferSize = VDEC_BIT_STREAM_LEN;
    MS_U32 u32stc32 = 0;
    MS_U32 u32stc = 0;
    MS_U32 u32pcr32 = 0;
    MS_U32 u32pcr = 0;

#if(DEMO_VDEC_MULTI_TEST == 1)
    MS_U32 u32VESlevel1 = 0;
    MS_U32 u32VWptr1 = 0;
    MS_U32 u32VRptr1 = 0;
    MS_U32 u32BufferSize1 = VDEC_BIT_STREAM1_LEN;
    MS_U32 u32stc321 = 0;
    MS_U32 u32stc1 = 0;
    MS_U32 u32pcr321 = 0;
    MS_U32 u32pcr1 = 0;
#endif

    while(1)
    {
        if(u8VDECDbgMsg & 0x1) //Main stream
        {
            if(stVDECInfo[0].bInited)
            {
                u32VWptr = MApi_VDEC_EX_GetESWritePtr(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID);
                u32VRptr = MApi_VDEC_EX_GetESReadPtr(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID);
                if (u32VWptr>=u32VRptr)
                {
                    u32VESlevel = u32VWptr - u32VRptr;
                }
                else
                {
                    u32VESlevel = u32BufferSize - ( u32VRptr - u32VWptr );
                }

                MApi_DMX_Stc_Eng_Get(stVDECInfo[E_VDEC_DEVICE_MAIN].u32STCEng,&u32stc32, &u32stc);
                MApi_DMX_Pcr_Eng_Get(stVDECInfo[E_VDEC_DEVICE_MAIN].u32STCEng,&u32pcr32, &u32pcr);

                printf("Main: systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\n",
                MsOS_GetSystemTime(),
                MApi_VDEC_EX_GetFrameCnt(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID),
                MApi_VDEC_EX_GetDispCnt(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID),
                MApi_VDEC_EX_GetErrCnt(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID),
                MApi_VDEC_EX_GetSkipCnt(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID),
                MApi_VDEC_EX_GetDropCnt(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID),
                u32pcr / 90,
                u32stc / 90,
                MApi_VDEC_EX_GetPTS(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID),
                (MApi_VDEC_EX_GetPTS(&stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID)- (u32stc / 90)),
                u32VESlevel,
                u32VWptr,
                u32VRptr
                //HAL_HVD_EX_GetData(stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID.u32Id & 0x00ffffff, E_HVD_GDATA_DEC_Q_NUMB),
                //HAL_HVD_EX_GetData(stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID.u32Id & 0x00ffffff, E_HVD_GDATA_DISP_Q_NUMB)DecQ: %ld, DispQ: %ld
                );
            }
        }

#if(DEMO_VDEC_MULTI_TEST == 1)
        if(u8VDECDbgMsg & 0x2)
        {
            if(stVDECInfo[1].bInited)
            {
                u32VWptr1 = MApi_VDEC_EX_GetESWritePtr(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID);
                u32VRptr1 = MApi_VDEC_EX_GetESReadPtr(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID);
                if (u32VWptr1>=u32VRptr1)
                {
                    u32VESlevel1 = u32VWptr1 - u32VRptr1;
                }
                else
                {
                    u32VESlevel1 = u32BufferSize1 - ( u32VRptr1 - u32VWptr1 );
                }

                MApi_DMX_Stc_Eng_Get(stVDECInfo[E_VDEC_DEVICE_SUB].u32STCEng,&u32stc321, &u32stc1);
                MApi_DMX_Pcr_Eng_Get(stVDECInfo[E_VDEC_DEVICE_SUB].u32STCEng,&u32pcr321, &u32pcr1);

                printf("\033[1;31m Sub: systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\033[0m\n",
                MsOS_GetSystemTime(),
                MApi_VDEC_EX_GetFrameCnt(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID),
                MApi_VDEC_EX_GetDispCnt(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID),
                MApi_VDEC_EX_GetErrCnt(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID),
                MApi_VDEC_EX_GetSkipCnt(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID),
                MApi_VDEC_EX_GetDropCnt(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID),
                u32pcr1 / 90,
                u32stc1 / 90,
                MApi_VDEC_EX_GetPTS(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID),
                (MApi_VDEC_EX_GetPTS(&stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID)- (u32stc1 / 90)),
                u32VESlevel1,
                u32VWptr1,
                u32VRptr1
                //HAL_HVD_EX_GetData(stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID.u32Id & 0x00ffffff, E_HVD_GDATA_DEC_Q_NUMB),
                //HAL_HVD_EX_GetData(stVDECInfo[E_VDEC_DEVICE_SUB].stVDECSteamID.u32Id & 0x00ffffff, E_HVD_GDATA_DISP_Q_NUMB)DecQ: %ld, DispQ: %ld
                );
            }
        }
#endif
        else if(u8VDECDbgMsg & 0x0)
        {
            printf("[%s][%d]close the VDEC debug task\n",__FUNCTION__,__LINE__);
            break;
        }
        MsOS_DelayTask(u32VDECDbgIntervalTime);
     }
}

//------------------------------------------------------------------------------
/// @brief Create VDEC DBG Task
/// @return TRUE: Task create success.
/// @return FALSE: Task create fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_VDEC_DBGCreateTask(void)
{
    MS_S32 s32MstarNonCachedPoolID = 0;
    pStackbase = NULL;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    /* Create task for VDEC DBG */
    if (pStackbase == NULL)
    {
        pStackbase = MsOS_AllocateMemory(TASK_STACK_SIZE,s32MstarNonCachedPoolID);
        db_print("[%s][%d] MsOS_AllocateMemory OK\n",__FUNCTION__,__LINE__);
    }

    if (pStackbase == NULL)
    {
        printf("[%s][%d] pStackbase == NULL\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (s32_VDECDbgTaskID == INVALID_TASK_ID)
    {
        s32_VDECDbgTaskID = MsOS_CreateTask((TaskEntry)_Demo_VDEC_DBGMSG,
                                             0,
                                             E_TASK_PRI_MEDIUM, // E_TASK_PRI_MEDIUM,
                                             TRUE,
                                             pStackbase,
                                             TASK_STACK_SIZE,
                                             "_VDEC_DBG");

        if (INVALID_TASK_ID == s32_VDECDbgTaskID)
        {
           printf("[%s][%d] Create VDEC DBG Task Failed INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
           return FALSE;
        }
        else
        {
           db_print("[%s][%d] s32_VDECDbgTaskID %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,s32_VDECDbgTaskID);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief function to get Diff time
/// @param[in] Start time or not any time
/// @return Diff time.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_U32 _Demo_VDEC_TimeDiffFromNow(MS_U32 u32Time)
{

    #if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
        MS_U32 u32Now = MsOS_GetSystemTime();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        MS_U32 u32Now = ts.tv_sec * 1000+ ts.tv_nsec/1000000;
    #endif

    if(u32Now >= u32Time)
    {
        return u32Now - u32Time;
    }
    else
    {
        return 0xFFFFFFFF - u32Time + u32Now;
    }
}

//------------------------------------------------------------------------------
/// @brief get MVOP input select
/// @param[in] Main or Sub Device
/// @param[in] VDEC StreamId
/// @return tMvopInputSel.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MVOP_InputSel _Demo_VDEC_GetMVOPInputSel(EN_VDEC_Device eDevice,VDEC_StreamId* pStreamId)
{
    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    if ((stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MPEG2_IFRAME)
        ||(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_IFRAME)
        ||(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_IFRAME)
        ||(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_IFRAME)
        ||(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_IFRAME))
    {
        eMvopInputSel = MVOP_INPUT_DRAM;
        db_print("MVOP_INPUT_MEMORY \n");
    }
    else if ((MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VC1_ADV) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG4) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_DIVX412) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_H263))
    {
        eMvopInputSel = MVOP_INPUT_MVD;
        db_print("MVOP_INPUT_MVD \n");
    }
    else if((MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_H264) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VP8) ||
        (MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_AVS))
    {
        eMvopInputSel = MVOP_INPUT_H264;
        db_print("MVOP_INPUT_HVD \n");
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_HEVC)
    {
        eMvopInputSel = MVOP_INPUT_EVD;
        db_print("MVOP_INPUT_EVD \n");
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_MVC)
    {
        eMvopInputSel = MVOP_INPUT_HVD_3DLR;
        db_print("MVOP_INPUT_HVD_3DLR \n");
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VP9)
    {
        eMvopInputSel = MVOP_INPUT_VP9;
        db_print("MVOP_INPUT_VP9 \n");
    }
    return eMvopInputSel;
}

//------------------------------------------------------------------------------
/// @brief get now system time
/// @return now system time.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U32 _Demo_VDEC_GetTime(void)
{
    #if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
        return MsOS_GetSystemTime();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec* 1000+ ts.tv_nsec/1000000;
    #endif
}

//------------------------------------------------------------------------------
/// @brief Mapping DDI Codec type to VDEC codec type
/// @return VDEC codec type
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
VDEC_EX_CodecType _Demo_VDEC_MappingCodec(EN_VDEC_DDI_CodecType eDDICodecType)
{
    VDEC_EX_CodecType eCodecType = E_VDEC_EX_CODEC_TYPE_MPEG2;

    switch (eDDICodecType)
    {
        case E_VDEC_DDI_CODEC_TYPE_H264:            /* fall through */
        case E_VDEC_DDI_CODEC_TYPE_H264_IFRAME:     /* fall through */
        case E_VDEC_DDI_CODEC_TYPE_H264_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_H264;
            db_print("Codec: H.264 \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_MPEG4_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_MPEG4;
            db_print("Codec: MPEG4 \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_VC1_ADV_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_VC1_ADV;
            db_print("Codec: VC1_ADV \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_VC1_MAIN_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_VC1_MAIN;
            db_print("Codec: VC1_MAIN \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_AVS:
        case E_VDEC_DDI_CODEC_TYPE_AVS_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_AVS;
            db_print("Codec: AVS \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_HEVC:
        case E_VDEC_DDI_CODEC_TYPE_HEVC_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_HEVC;
            db_print("Codec: HEVC \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_VP9:
        case E_VDEC_DDI_CODEC_TYPE_VP9_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_VP9;
            db_print("Codec: VP9 \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_MVC:
        case E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_MVC;
            db_print("Codec: MVC \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_MPEG2:
        case E_VDEC_DDI_CODEC_TYPE_MPEG2_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY:
        default:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
            db_print("Codec: MPEG2 \n");
            break;
        }
    }
    return eCodecType;
}

//------------------------------------------------------------------------------
/// @brief Mapping DDI Source mode to VDEC Source Mode
/// @return VDEC Source Mode
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
VDEC_EX_SrcMode _Demo_VDEC_MappingSourceMode(EN_VDEC_DDI_SrcMode eDDISourceMode)
{
    VDEC_EX_SrcMode eVDECSrcMode = E_VDEC_EX_SRC_MODE_DTV;

    switch (eDDISourceMode)
    {
        ///DTV mode
        case E_VDEC_DDI_SRC_MODE_DTV:
            eVDECSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            db_print("Play Mode : DTV \n");
            break;
        ///TS file mode
        case E_VDEC_DDI_SRC_MODE_TS_FILE:
            eVDECSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
            db_print("Play Mode : TS_FILE \n");
            break;
        ///generic file mode
        case E_VDEC_DDI_SRC_MODE_FILE:
            eVDECSrcMode = E_VDEC_EX_SRC_MODE_FILE;
            db_print("Play Mode : FILE \n");
            break;
        /// TS file and Demo ES buffer mode
        case E_VDEC_DDI_SRC_MODE_TS_FILE_DUAL_ES:
            eVDECSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES;
            db_print("Play Mode : TS_FILE_Demo_ES \n");
            break;
        ///generic file and Demo ES buffer mode
        case E_VDEC_DDI_SRC_MODE_FILE_DUAL_ES:
            eVDECSrcMode = E_VDEC_EX_SRC_MODE_FILE_DUAL_ES;
            db_print("Play Mode : FILE_Demo_ES \n");
            break;
        default :
            eVDECSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            db_print("Play Mode : DTV \n");
            break;
    }
    return eVDECSrcMode;
}

//------------------------------------------------------------------------------
/// @brief Mapping DDI Source mode to VDEC Source Mode
/// @return VDEC Source Mode
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
void _Demo_VDEC_InitSysConfigParam(EN_VDEC_Device eDevice,VDEC_EX_SysCfg *pstSysConfig)
{
    pstSysConfig->u32CodeBufAddr = MEM_ADR_BY_MIU(VDEC_AEON_ADR, VDEC_AEON_MEMORY_TYPE);
    pstSysConfig->u32CodeBufSize = VDEC_AEON_LEN;

#if(DEMO_VDEC_NDECODE_TEST == 1)
    pstSysConfig->u32FrameBufAddr       = 0;
    pstSysConfig->u32FrameBufSize       = 0;
    pstSysConfig->u32BitstreamBufAddr   = 0;
    pstSysConfig->u32BitstreamBufSize   = 0;
    pstSysConfig->u32DrvProcBufAddr     = 0;
#else
    if (eDevice == E_VDEC_DEVICE_MAIN)
    {
        pstSysConfig->u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
        pstSysConfig->u32FrameBufSize = VDEC_FRAME_BUF_LEN;
        pstSysConfig->u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
        pstSysConfig->u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
        pstSysConfig->u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (eDevice == E_VDEC_DEVICE_SUB)
    {
        pstSysConfig->u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
        pstSysConfig->u32FrameBufSize = VDEC_FRAME_BUF1_LEN;
        pstSysConfig->u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        pstSysConfig->u32BitstreamBufSize = VDEC_BIT_STREAM1_LEN;
        pstSysConfig->u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
    }
#endif
#endif

#if(DEMO_MVC_TEST == 1)
    if(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC)
    {
        pstSysConfig->u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_FRAME_BUF_ADR, VDEC_MVC_FRAME_BUF_MEMORY_TYPE);
        pstSysConfig->u32FrameBufSize = VDEC_MVC_FRAME_BUF_LEN;
        pstSysConfig->u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
        pstSysConfig->u32BitstreamBufSize = VDEC_MVC_BIT_STREAM_LEN;
        pstSysConfig->u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
    }
#endif

#if(DEMO_CMA_TEST == 1)
    pstSysConfig->u32FrameBufAddr = 0;
#endif

    pstSysConfig->eDbgMsgLevel = eVDECDebuglevel;
    db_print("VDEC FW add = 0x%"DTC_MS_PHY_x"\n", pstSysConfig->u32CodeBufAddr);
    db_print("VDEC FB add = 0x%"DTC_MS_PHY_x"\n", pstSysConfig->u32FrameBufAddr);
    db_print("VDEC BS add = 0x%"DTC_MS_PHY_x"\n", pstSysConfig->u32BitstreamBufAddr);

}

void _Demo_VDEC_PreSetControl_NdecodeSetting(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId)
{

#if(DEMO_XC_DIPMultiWin_TEST == 1)  //need to remove
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_CONNECT_INPUT_TSP, *peDevice);
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_PRESET_STC, stVDECInfo[*peDevice].u32STCEng);

    if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
    {
        NdecodeDBG_print("\033[35m =========DIP init + Main/Sub/N=========== \033[0m\n");
        MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_DISPLAY_MODE, E_VDEC_EX_DISPLAY_MODE_MCU);
    }
    else if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_MVOP_MODE)
    {
        NdecodeDBG_print("\033[35m =========MVOP[%d]Hardwire mode + Main/Sub/N =========== \033[0m\n",stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_MvopPath);
        VDEC_EX_DynmcDispPath stDynmcDispPath;
        stDynmcDispPath.bConnect = TRUE;
        stDynmcDispPath.eMvopPath = (VDEC_EX_DISPLAY_PATH)stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_MvopPath;
        MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_CONNECT_DISPLAY_PATH, (MS_VIRT)&stDynmcDispPath);
    }
#if(DEMO_DMS_TEST == 1)
    else if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
    {
        NdecodeDBG_print("\033[35m =========PureMCU_MODE + Main/Sub/N=========== \033[0m\n");
        MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_DRIVER_MCU_MODE,TRUE);

        VDEC_WIN_INFO stVDECWinInfo;
        memset(&stVDECWinInfo,0,sizeof(VDEC_WIN_INFO));

        stVDECWinInfo.win_id = stVDECInfo[*peDevice].stShowFrameInfo.stVDECWinControl.win_id;
        stVDECWinInfo.u8EnableSetDigitalSignal   = stVDECInfo[*peDevice].stShowFrameInfo.stVDECWinControl.u8EnableSetDigitalSignal;
        stVDECWinInfo.u8EnableClearDigitalSignal = stVDECInfo[*peDevice].stShowFrameInfo.stVDECWinControl.u8EnableClearDigitalSignal;
        MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_WIN_INIT,(MS_VIRT)&stVDECWinInfo);
    }

    if(stVDECInfo[*peDevice].stShowFrameInfo.u32Window == 0)
    {
        NdecodeDBG_print("\033[35m ========= u32Window 0 : E_VDEC_EX_USER_CMD_CONNECT_DISPLAY_PATH=========== \033[0m\n");
        VDEC_EX_DynmcDispPath stDynmcDispPath;
        stDynmcDispPath.bConnect = TRUE;
        stDynmcDispPath.eMvopPath = E_VDEC_EX_DISPLAY_PATH_MVOP_MAIN;
        MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_CONNECT_DISPLAY_PATH, (MS_VIRT)&stDynmcDispPath);
    }
#endif
#endif

#if(DEMO_VDEC_NDECODE_TEST == 1)
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_FRAMEBUFFER_MONOPOLY, FALSE);
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_BITSTREAMBUFFER_MONOPOLY, FALSE);

    // Preset Bitstream Buffer
    VDEC_EX_BufferInfo stParms;
    stParms.eType     = E_VDEC_EX_BUFFERTYPE_BS_MAIN;
    stParms.phyAddr   = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
    stParms.szSize    = VDEC_BIT_STREAM_LEN;
    stParms.u32Config = 0;
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_BUFFER_INFO, (MS_VIRT)&stParms);

    stParms.eType     = E_VDEC_EX_BUFFERTYPE_BS_SUB;
    stParms.phyAddr   = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
    stParms.szSize    = VDEC_BIT_STREAM1_LEN;
    stParms.u32Config = 0;
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_BUFFER_INFO, (MS_VIRT)&stParms);

    // Preset Frame Buffer
    stParms.eType     = E_VDEC_EX_BUFFERTYPE_FB_MAIN;
    stParms.phyAddr   = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
    stParms.szSize    = VDEC_FRAME_BUF_LEN;
    stParms.u32Config = 0;
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_BUFFER_INFO, (MS_VIRT)&stParms);

    stParms.eType     = E_VDEC_EX_BUFFERTYPE_FB_SUB;
    stParms.phyAddr   = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
    stParms.szSize    = VDEC_FRAME_BUF1_LEN;
    stParms.u32Config = 0;
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_BUFFER_INFO, (MS_VIRT)&stParms);
#endif

}

#ifdef DTV_HDR_VDEC_FLOW
static MS_U8 _Demo_VDEC_IsHDRStream(EN_VDEC_Device eDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    VDEC_EX_AVC_VUI_DISP_INFO stHDRVUIInfo;
    memset(&stHDRVUIInfo,0,sizeof(stHDRVUIInfo));
    MApi_VDEC_EX_GetControl(&stVDECInfo[eDevice].stVDECSteamID ,E_VDEC_EX_USER_CMD_GET_VUI_DISP_INFO,(MS_U32 *)&stHDRVUIInfo);

    VDEC_EX_DisplayColourVolume_SEI stHDRSEIInfo;
    memset(&stHDRSEIInfo,0,sizeof(stHDRSEIInfo));
    MApi_VDEC_EX_GetControl(&stVDECInfo[eDevice].stVDECSteamID ,E_VDEC_EX_USER_CMD_GET_DCV_SEI,(MS_U32 *)&stHDRSEIInfo);

    VDEC_EX_ContentLightLevelInfo_SEI stHDRSEICLLIInfo;
    memset(&stHDRSEICLLIInfo,0,sizeof(stHDRSEICLLIInfo));
    MApi_VDEC_EX_GetControl(&stVDECInfo[eDevice].stVDECSteamID ,E_VDEC_EX_USER_CMD_GET_CLLI_SEI,(MS_U32 *)&stHDRSEICLLIInfo);

    printf("Is HDR : %d %d %d\n",stHDRVUIInfo.bColour_description_present_flag,
                                    stHDRSEIInfo.bColourVolumeSEIEnabled,
                                    stHDRSEICLLIInfo.bContentLightLevelEnabled);

    stVDECInfo[eDevice].u8IsHDRStream = (stHDRVUIInfo.bColour_description_present_flag|stHDRSEIInfo.bColourVolumeSEIEnabled|stHDRSEICLLIInfo.bContentLightLevelEnabled);

    return stVDECInfo[eDevice].u8IsHDRStream;
}

static MS_BOOL _Demo_VDEC_SetHDRCmd2msAPI(EN_VDEC_Device eDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    MS_BOOL bSeamlessEnable = FALSE;
    msAPI_XC_GetHDRSeamlessFlag(&bSeamlessEnable);

    if(_Demo_VDEC_IsHDRStream(eDevice) || bSeamlessEnable)
    {
        msAPI_XC_CFD_Control(NULL,E_MSAPI_XC_CFD_SET_SINK_EDID_TO_CFD,NULL);
        msAPI_XC_CFD_Control(NULL,E_MSAPI_XC_CFD_INIT,NULL);
#if (DEMO_DTV_HDR_VER_TEST == 1)
        MS_BOOL bHDR = TRUE;
        msAPI_XC_SetCommand(NULL, E_MSAPI_XC_CMD_SET_HDR, &bHDR);
#endif
    }
    else
    {
        // Non-HDR, set E_XC_CFD_HDR_TYPE_NONE
        XC_CFD_HDR stCfdHdr = {CFD_HDR_VERSION, sizeof(XC_CFD_HDR), 0, E_XC_CFD_HDR_TYPE_NONE};
        MSAPI_XC_CFD_SENDCFD_INFO stSendCFDInfo = {E_XC_CFD_CTRL_TYPE_HDR, sizeof(XC_CFD_HDR), &stCfdHdr};
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_SEND_CFD, &stSendCFDInfo);
        msAPI_XC_CFD_Control(NULL, E_MSAPI_XC_CFD_FIRE, NULL);
    }

    return TRUE;
}

#endif

#if(DEMO_VDEC_MULTI_TEST == 1 && DEMO_XC_DIPMultiWin_TEST == 0)
//------------------------------------------------------------------------------
/// @brief check the VDEC STC will change or not
/// @param[in] Main or Sub Device
/// @param[in] u32STCSource
/// @return TRUE : Process success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_VDEC_STC_Switch(EN_VDEC_Device eDevice,VDEC_StreamId* pstVDECStreamId, MS_U32 u32STCSource)
{
    //  Indep Command
    VDEC_EX_DecModCfg stDecModCfg;

    memset(&stDecModCfg, 0, sizeof(VDEC_EX_DecModCfg));

    stDecModCfg.eDecMod = E_VDEC_EX_DEC_MODE_DUAL_INDIE;
    stDecModCfg.u8ArgSize = 1;


    /* Control for STC Usage by PCR Filter ID  information */
    if( eDevice != u32STCSource )
    {
        db_print("[%s][%d] %d eSTC_Source:%"DTC_MS_U32_d" : Switch\n",__FUNCTION__,__LINE__,eDevice,u32STCSource);
        stDecModCfg.u32Arg = E_VDEC_EX_DEC_PIP_SYNC_SWITCH;
    }
    else
    {
        db_print("[%s][%d] %d eSTC_Source:%"DTC_MS_U32_d" : INDIE\n",__FUNCTION__,__LINE__,eDevice,u32STCSource);
        stDecModCfg.u32Arg = E_VDEC_EX_DEC_PIP_SYNC_INDIE;
    }

    if (MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_SET_DECODE_MODE,(MS_U32)&stDecModCfg) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]MApi_VDEC_EX_SetControl return fail!!! :%d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_SetControl(&stVDECInfo[eDevice].stVDECSteamID,E_VDEC_EX_USER_CMD_SET_DECODE_MODE,(MS_U32)&stDecModCfg));
    }

    return TRUE;

}
#endif

// check frame need display and sync or not
EN_VDEC_DIP_FrameStatus _Demo_VDEC_CheckFrameSync(MS_U32 *pu32decode,MS_BOOL *pbSync,MS_U32 *pu32STCEng,MS_U64 *pu64VPTS)
{
    EN_VDEC_DIP_FrameStatus eStatus = 0;

    MS_U32 u32STC32 = 0;    //90hz
    MS_U32 u32STC = 0;      //90hz
    MS_U64 u64STC = 0;      //ms

    MS_U64 u64PTS = *pu64VPTS/90;   //90hz
    MS_S64 s64Diff = 0;

    if(*pbSync)
    {
        MApi_DMX_Stc_Eng_Get(*pu32STCEng,&u32STC32, &u32STC);
        u64STC = (((MS_U64)u32STC32) << 32 | u32STC)/90;

        s64Diff = u64PTS + (MS_U64)stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u32SyncDelay - u64STC;

        if (s64Diff >= 0)
        {
            if(s64Diff < (MS_U64)stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u32FreerunThreshold)
            {
                if(s64Diff <= (MS_U64)stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u16Tolerance)
                {
                    eStatus=  E_VDEC_Frame_Display;
                }
                else
                {
                    eStatus= E_VDEC_Frame_Repeat;
                }
            }
            else
            {
                eStatus= E_VDEC_Frame_Display;
            }
        }
        else if (s64Diff < 0)
        {
            if(-s64Diff < (MS_U64)stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u32FreerunThreshold)
            {
                if(-s64Diff <= (MS_U64)stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u16Tolerance)
                {
                    eStatus= E_VDEC_Frame_Display;
                }
                else
                {
                    stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u32DropCnt++;

                    if(stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u32DropCnt % 5 == 0)
                    {
                        eStatus= E_VDEC_Frame_Release;
                    }
                    else
                        eStatus= E_VDEC_Frame_Display;
                }
            }
            else
            {
                eStatus= E_VDEC_Frame_Display;
            }
        }

        if(fpSync)
        {
            MS_U32 u32PCR32 = 0;    //90hz
            MS_U32 u32PCR = 0;      //90hz
            MS_U64 u64PCR = 0;      //ms

            MApi_DMX_Pcr_Eng_Get(*pu32STCEng,&u32PCR32, &u32PCR);
            u64PCR = (((MS_U64)u32PCR32) << 32 | u32PCR)/90;

            fprintf(fpSync,"[SYS: %"DTC_MS_U32_d"] id[%"DTC_MS_U32_d"] STCEng[%"DTC_MS_U32_d"] PTS[%"DTC_MS_U64_d"] SyncDelay[%"DTC_MS_U64_d"] u64STC[%"DTC_MS_U64_d"] u64PCR[%"DTC_MS_U64_d"] u64Diff[%"DTC_MS_U64_d"] eStatus[%d]\n",MsOS_GetSystemTime(),*pu32decode,*pu32STCEng,u64PTS,(MS_U64)stVDECDIPInfo[*pu32decode].stDIPFrameSyncInfo.u32SyncDelay,u64STC,u64PCR,s64Diff,eStatus);
            fflush(fpSync);
        }
    }
    else
    {
        eStatus= E_VDEC_Frame_Display;
    }

    return eStatus;
}

#if (DEMO_XC_DIPMultiWin_TEST == 1)
//Transform format
E_MSAPI_XC_VIDEO_DATA_FMT _Demo_VDEC_GetDIPRDataFMT(VDEC_EX_CodecType eCodec,MS_BOOL b10bit)
{
    E_MSAPI_XC_VIDEO_DATA_FMT eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_MAX;

    switch(eCodec)
    {
        case E_VDEC_EX_CODEC_TYPE_HEVC:
                {
                    if(b10bit)
                        eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_H265_10BITS;
                    else
                        eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_H265;
                    break;
                }
        case E_VDEC_EX_CODEC_TYPE_H264:
        default :
                eFmt = E_MSAPI_XC_VIDEO_DATA_FMT_YUV420;
                break;
    }

    return eFmt;
}

//codec mapping
E_MSAPI_XC_VIDEO_CODEC _Demo_VDEC_CodecVDEC2msAPI(VDEC_EX_CodecType eCodec)
{
    E_MSAPI_XC_VIDEO_CODEC eMsapiCodec = E_MSAPI_XC_VIDEO_CODEC_UNKNOWN;

    switch (eCodec)
    {
        case E_VDEC_EX_CODEC_TYPE_H264:
            eMsapiCodec = E_MSAPI_XC_VIDEO_CODEC_H264;
            break;
        case E_VDEC_EX_CODEC_TYPE_AVS:
            eMsapiCodec = E_MSAPI_XC_VIDEO_CODEC_AVS;
            break;
        case E_VDEC_EX_CODEC_TYPE_HEVC:
            eMsapiCodec = E_MSAPI_XC_VIDEO_CODEC_HEVC;
            break;
        case E_VDEC_EX_CODEC_TYPE_MPEG2:
        default:
            eMsapiCodec = E_MSAPI_XC_VIDEO_CODEC_MPEG;
            break;
    }
    return eMsapiCodec;
}

//Scantype mapping
E_MSAPI_XC_VIDEO_SCAN_TYPE _Demo_VDEC_ScanTypeVDEC2msAPI(MS_U32 u32ScanType)
{
    // ProgressiveFrame = 0,   /** Picture is a progressive frame */
    // InterlacedFrame  = 2,    /** Picture is an interlaced frame */
    // InterlacedField  = 3,      /** Picture is two interlaced fields */

    E_MSAPI_XC_VIDEO_SCAN_TYPE eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX;

    switch (u32ScanType)
    {
        case 0:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_PROGRESSIVE;
            break;
        case 2:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FRAME;
            break;
        case 3:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD;
            break;
        default:
            eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX;
            break;
    }
    return eMsapiScanType;
}

//Scantype mapping
E_MSAPI_XC_VIDEO_SCAN_TYPE _Demo_VDEC_DisPlayInfoVDEC2msAPI(VDEC_EX_CodecType eCodec,MS_U8 u8interlace)
{
    E_MSAPI_XC_VIDEO_SCAN_TYPE eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX;

    switch (eCodec)
    {
        case E_VDEC_EX_CODEC_TYPE_HEVC:
            {
                if(u8interlace == 1)
                    eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD;
                else if(u8interlace == 2)
                    eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FRAME;
                break;
            }
        default:
            {
                if(u8interlace == 1)
                    eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FRAME;
                else
                    eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX;
                break;
            }
    }

    if(u8interlace == 0)
        eMsapiScanType = E_MSAPI_XC_VIDEO_SCAN_TYPE_PROGRESSIVE;

    return eMsapiScanType;
}


void _Demo_VDEC_StoreDispFrameInfoVDEC2msAPI(MSAPI_XC_FRAME_FORMAT *pstFrame,int i,MS_U16 u16WritePointer,MS_U8 u8FrameIdx)
{

    pstFrame->u32FrameIndex     = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.u32Idx;
    pstFrame->u32PriData        = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.u32PriData;

    pstFrame->u16SrcWidth       = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.u16Width;
    pstFrame->u16SrcHeight      = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.u16Height;
    pstFrame->eFieldType        = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.eFieldType;

    pstFrame->u16CropRight      = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispCropInfo.u16CropRight;
    pstFrame->u16CropLeft       = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispCropInfo.u16CropLeft;
    pstFrame->u16CropBottom     = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispCropInfo.u16CropBottom;
    pstFrame->u16CropTop        = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispCropInfo.u16CropTop;

    pstFrame->stHWFormat.u16SrcPitch                = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.u16Pitch;
    pstFrame->stHWFormat.u32SrcLumaAddr             = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.u32LumaAddr;
    pstFrame->stHWFormat.u32SrcChromaAddr           = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.u32ChromaAddr;

    //10 bit
    pstFrame->b10bitData       = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].b10bit;
    if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].b10bit == TRUE)
    {
        pstFrame->stHWFormat.u16Src10bitPitch      = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u16Pitch_2bit ;
        pstFrame->stHWFormat.u32SrcLumaAddr_2bit   = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32LumaAddr_2bit;
        pstFrame->stHWFormat.u32SrcChromaAddr_2bit = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32ChromaAddr_2bit;
        pstFrame->stHWFormat.u8LumaBitdepth        = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u8LumaBitdepth;
        pstFrame->stHWFormat.u8ChromaBitdepth      = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u8ChromaBitdepth ;
    }

    //mfcodec info
    pstFrame->stHWFormat.stMFdecInfo.bMFDec_Enable = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32MFCodecInfo & 0xFF ;
    if(pstFrame->stHWFormat.stMFdecInfo.bMFDec_Enable != 0 && pstFrame->stHWFormat.stMFdecInfo.bMFDec_Enable != 0xFF)
    {
        pstFrame->stHWFormat.stMFdecInfo.bMFDec_Enable      = 1;
        pstFrame->stHWFormat.stMFdecInfo.bBypass_codec_mode = 0;
        pstFrame->stHWFormat.stMFdecInfo.u8MFDec_Select     = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32MFCodecInfo>>8) & 0x3;     //Bit[8]      MFDec ID. 1: MFDec 1; 0: MFDec 0; 2: MFDec 2
        pstFrame->stHWFormat.stMFdecInfo.bUncompress_mode   = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32MFCodecInfo >> 28) & 0x1;  //Bit[28]     1: MFCodec uncompress mode; 0: data is compressed
        pstFrame->stHWFormat.stMFdecInfo.en_MFDecVP9_mode   = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32MFCodecInfo >> 29) & 0x1;  //Bit[29]     1: MFCodec 3.0 vp9 mode; 0: MFCodec 3.0 h26x mode
        pstFrame->stHWFormat.stMFdecInfo.u16Bitlen_Pitch    = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32MFCodecInfo >> 16) & 0xFF; //Bits[23:16] MFCodec pitch setting

        pstFrame->stHWFormat.stMFdecInfo.u16StartX = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispCropInfo.u16CropRight;
        pstFrame->stHWFormat.stMFdecInfo.u16StartY = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispCropInfo.u16CropTop;

        MS_U8 u8BitLenmiu = 0;
        u8BitLenmiu = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32MFCodecInfo >> 24) & 0xF ;
        pstFrame->stHWFormat.stMFdecInfo.phyBitlen_Base     = (u8BitLenmiu == 0) ? (MS_PHY)stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32LumaMFCbitlen : ((MS_PHY)stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfoEXT.u32LumaMFCbitlen | MIU_INTERVAL);
    }
    else
    {
        pstFrame->stHWFormat.stMFdecInfo.bMFDec_Enable = 0;
    }

}

void _Demo_VDEC_StoreFrameInfoVDEC2msAPI(MSAPI_XC_VDECFRAME_INFO *pstXC_VDECFRAME_INFO,int i,MS_U16 u16WritePointer)
{
    MS_U8 u8FieldNum = 0;
    MS_U8 u8FrameIdx = 0;

    pstXC_VDECFRAME_INFO->eWindow          = stVDECDIPInfo[i].eWindow;
    pstXC_VDECFRAME_INFO->u32FrameRate     = stVDECDIPInfo[i].u32FrameRate;
    pstXC_VDECFRAME_INFO->eTileMode        = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].eDIPFrameTileMode;
    pstXC_VDECFRAME_INFO->eFieldOrderType  = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].bFieldOrder;
    pstXC_VDECFRAME_INFO->u64Pts           = stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp;
    pstXC_VDECFRAME_INFO->eCODEC           = _Demo_VDEC_CodecVDEC2msAPI(stVDECDIPInfo[i].eCodec);
    pstXC_VDECFRAME_INFO->eFmt             = _Demo_VDEC_GetDIPRDataFMT(stVDECDIPInfo[i].eCodec,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].b10bit);
    pstXC_VDECFRAME_INFO->eScanType        = _Demo_VDEC_ScanTypeVDEC2msAPI(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u32ScanType);

    pstXC_VDECFRAME_INFO->stVDECStream_Id_Version.u32VDECStreamID       = stVDECDIPInfo[i].StreamId.u32Id;
    pstXC_VDECFRAME_INFO->stVDECStream_Id_Version.u32VDECStreamVersion  = stVDECDIPInfo[i].StreamId.u32Version;

    if(pstXC_VDECFRAME_INFO->eScanType == E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
    {
        u8FieldNum = 1;
    }

    for(u8FrameIdx = 0;u8FrameIdx <= u8FieldNum; u8FrameIdx++)
    {
        MSAPI_XC_FRAME_FORMAT stFrame;
        memset(&stFrame,0,sizeof(MSAPI_XC_FRAME_FORMAT));

        _Demo_VDEC_StoreDispFrameInfoVDEC2msAPI(&stFrame,i,u16WritePointer,u8FrameIdx);
        memcpy(&(pstXC_VDECFRAME_INFO->stFrames[u8FrameIdx]),&stFrame,sizeof(MSAPI_XC_FRAME_FORMAT));

        NdecodeDBG_print("\033[35m ========u8FrameIdx:%d u32ScanType:%d bFieldOrder:%d eFieldType:%d============ \033[0m\n",u8FrameIdx,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u32ScanType,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].bFieldOrder,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[u8FrameIdx].stDIPDispFrameInfo.stFrmInfo.eFieldType);
        NdecodeDBG_print("\033[35m ========id[%d] bMFDec_Enable:[%d]  bMFDec_Sel:[%d] 10bit:[%d]============ \033[0m\n",i,pstXC_VDECFRAME_INFO->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.bMFDec_Enable,pstXC_VDECFRAME_INFO->stFrames[u8FrameIdx].stHWFormat.stMFdecInfo.u8MFDec_Select,pstXC_VDECFRAME_INFO->stFrames[u8FrameIdx].b10bitData);
    }


}
void _Demo_VDEC_StoreFrameInfoFromVDEC(int i,MS_U16 u16WritePointer,MS_BOOL bIsSecondField)
{
    if(bIsSecondField == FALSE)
    {
        stVDECDIPInfo[i].eCodec = MApi_VDEC_EX_GetActiveCodecType(&stVDECDIPInfo[i].StreamId);

        stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].b10bit       = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ID_L >> 21)& 0x01;   // 10bit or not, [ 21]
        stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u32ScanType  = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ID_L >> 19)& 0x03;  // Picture format, [ 19-20],
        stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].bFieldOrder  = (stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ID_L >> 16)& 0x01;  //    0-> bottom first, 1-> top first
        stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp = (((MS_U64)stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ID_H) << 32) | stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32TimeStamp;

        VDEC_EX_FrameInfoExt_v6 stFrmInfo_v6;
        memset(&stFrmInfo_v6,0,sizeof(VDEC_EX_FrameInfoExt_v6));

        VDEC_EX_VerCtl *stVerCrl = (VDEC_EX_VerCtl *) &stFrmInfo_v6;
        stVerCrl->u32version = 6;
        stVerCrl->u32size = sizeof(VDEC_EX_FrameInfoExt_v6);

        if (E_VDEC_EX_OK == MApi_VDEC_EX_GetControl(&stVDECDIPInfo[i].StreamId, E_VDEC_EX_USER_CMD_GET_NEXT_DISP_FRAME_INFO_EXT, (MS_U32 *)&stFrmInfo_v6))
        {
            stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].eDIPFrameTileMode = stFrmInfo_v6.eTileMode;

            stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispCropInfo.u16CropRight     = stFrmInfo_v6.sFrameInfoExt_v5.u16CropRight;
            stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispCropInfo.u16CropLeft      = stFrmInfo_v6.sFrameInfoExt_v5.u16CropLeft;
            stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispCropInfo.u16CropBottom    = stFrmInfo_v6.sFrameInfoExt_v5.u16CropBottom;
            stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispCropInfo.u16CropTop       = stFrmInfo_v6.sFrameInfoExt_v5.u16CropTop;

            VDEC_EX_FrameInfoExt *stFrameInfoExt = (VDEC_EX_FrameInfoExt *) &stFrmInfo_v6;
            memcpy(&(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfoEXT),stFrameInfoExt,sizeof(VDEC_EX_FrameInfoExt));
        }
    }
    else
    {
        VDEC_EX_DispFrame *pstDispFrame=NULL;

        if(MApi_VDEC_EX_GetNextDispFrame(&stVDECDIPInfo[i].StreamId,&pstDispFrame) != E_VDEC_EX_OK)
        {
            //NdecodeDBG_print("[id:%d]MApi_VDEC_EX_GetNextDispFrame FAIL!!!!!\n",i);
        }
        else
        {
            memcpy(&(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].stDIPDispFrameInfo),pstDispFrame,sizeof(VDEC_EX_DispFrame));

            VDEC_EX_FrameInfoExt_v6 stFrmInfo_v6;
            memset(&stFrmInfo_v6,0,sizeof(VDEC_EX_FrameInfoExt_v6));

            VDEC_EX_VerCtl *stVerCrl = (VDEC_EX_VerCtl *) &stFrmInfo_v6;
            stVerCrl->u32version = 6;
            stVerCrl->u32size = sizeof(VDEC_EX_FrameInfoExt_v6);

            if (E_VDEC_EX_OK == MApi_VDEC_EX_GetControl(&stVDECDIPInfo[i].StreamId, E_VDEC_EX_USER_CMD_GET_NEXT_DISP_FRAME_INFO_EXT, (MS_U32 *)&stFrmInfo_v6))
            {
                stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].eDIPFrameTileMode = stFrmInfo_v6.eTileMode;

                stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].stDIPDispCropInfo.u16CropRight     = stFrmInfo_v6.sFrameInfoExt_v5.u16CropRight;
                stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].stDIPDispCropInfo.u16CropLeft      = stFrmInfo_v6.sFrameInfoExt_v5.u16CropLeft;
                stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].stDIPDispCropInfo.u16CropBottom    = stFrmInfo_v6.sFrameInfoExt_v5.u16CropBottom;
                stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].stDIPDispCropInfo.u16CropTop       = stFrmInfo_v6.sFrameInfoExt_v5.u16CropTop;

                VDEC_EX_FrameInfoExt *stFrameInfoExt = (VDEC_EX_FrameInfoExt *) &stFrmInfo_v6;
                memcpy(&(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[1].stDIPDispFrameInfoEXT),stFrameInfoExt,sizeof(VDEC_EX_FrameInfoExt));
            }
        }
    }
}

void _Demo_VDEC_Release_VDEC_Frame(int i,MS_U16 u16ReadPointer,MS_BOOL bIsSecondField)
{
    if(bIsSecondField == FALSE)
    {
        VDEC_EX_DispFrame stDispFrame;
        stDispFrame.u32Idx = stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.u32Idx;
        stDispFrame.u32PriData = stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.u32PriData;

        if(MApi_VDEC_EX_ReleaseFrame(&stVDECDIPInfo[i].StreamId, &stDispFrame) != E_VDEC_EX_OK)
        {
            NdecodeDBG_print("MApi_VDEC_EX_ReleaseFrame FAIL!!!\n");
        }
    }
    else
    {
        VDEC_EX_DispFrame stDispFrame;
        stDispFrame.u32Idx = stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[1].stDIPDispFrameInfo.u32Idx;
        stDispFrame.u32PriData = stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[1].stDIPDispFrameInfo.u32PriData;

        if(MApi_VDEC_EX_ReleaseFrame(&stVDECDIPInfo[i].StreamId, &stDispFrame) != E_VDEC_EX_OK)
        {
            NdecodeDBG_print("MApi_VDEC_EX_ReleaseFrame FAIL!!!\n");
        }
    }
}

MS_BOOL _Demo_VDEC_GetFrame(void)
{
    MS_U16 u16WritePointer = 0;
    MS_U32 u32TempTime[VDEC_MAX_SUPPORT_STREAM_NUM] = {MsOS_GetSystemTime()};
    MS_U32 u32TempCnt[VDEC_MAX_SUPPORT_STREAM_NUM] = {0};

    while(stVDECDIPFrameController.eGetTaskStatus != E_VDEC_Stop)
    {
        int i = 0;
        for(i = 0 ; i < VDEC_MAX_SUPPORT_STREAM_NUM ; i++)
        {
            if(stVDECDIPInfo[i].eDecodeStatus == E_VDEC_Start)
            {
                u16WritePointer = stVDECDIPInfo[i].stFrameWriteReadPointer.u16WritePointer;

                if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Repeat)
                {
                    stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus = _Demo_VDEC_CheckFrameSync((MS_U32 *)&i,&stVDECDIPInfo[i].stDIPFrameSyncInfo.bNeedSync,&stVDECDIPInfo[i].stDIPFrameSyncInfo.u32STCEng,&stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp);
                }
                else if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Invalid)
                {
                    VDEC_EX_DispFrame *pstDispFrame = NULL;
                    if(MApi_VDEC_EX_GetNextDispFrame(&stVDECDIPInfo[i].StreamId,&pstDispFrame) != E_VDEC_EX_OK)
                    {
                        //NdecodeDBG_print("[id:%d]MApi_VDEC_EX_GetNextDispFrame FAIL!!!!!\n",i);
                    }
                    else
                    {
                        memcpy(&(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo),pstDispFrame,sizeof(VDEC_EX_DispFrame));

                        MS_BOOL bIsSecondField = FALSE;
                        _Demo_VDEC_StoreFrameInfoFromVDEC(i,u16WritePointer,bIsSecondField);

                        if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u32ScanType == 3) //InterlacedField  = 3
                        {
                            bIsSecondField = TRUE;
                            _Demo_VDEC_StoreFrameInfoFromVDEC(i,u16WritePointer,bIsSecondField);
                        }

                        stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus = _Demo_VDEC_CheckFrameSync((MS_U32 *)&i,&stVDECDIPInfo[i].stDIPFrameSyncInfo.bNeedSync,&stVDECDIPInfo[i].stDIPFrameSyncInfo.u32STCEng,&stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp);

                        if(fpPush)
                        {
                            fprintf(fpPush,"GET Frame[id:%"DTC_MS_U32_d"][stream ID: 0x%8"DTC_MS_U32_x"][WP:%"DTC_MS_U32_d"][SYS: %"DTC_MS_U32_d"] [Idx:%"DTC_MS_U32_d"][%"DTC_MS_U32_d"x%"DTC_MS_U32_d"]Luma : 0x%"DTC_MS_PHY_x" chroma : 0x%"DTC_MS_PHY_x" (FRM: %"DTC_MS_U32_d")(PTS :%"DTC_MS_U64_d") (STSTUS :%"DTC_MS_U32_d")\n"
                                    ,i,stVDECDIPInfo[i].StreamId.u32Id,u16WritePointer,MsOS_GetSystemTime(),stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.u32Idx ,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Width, stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Height,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32LumaAddr,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ChromaAddr,MApi_VDEC_EX_GetFrameCnt(&stVDECDIPInfo[i].StreamId),stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus);
                            fflush(fpPush);
                        }
                    }

                }

//fprintf(fpPush,"[id:%"DTC_MS_U32_d"][stream ID: 0x%8"DTC_MS_U32_x"][%"DTC_MS_U32_d"][SYS: %"DTC_MS_U32_d"] [%"DTC_MS_U32_d"]PUSH [%"DTC_MS_U32_d"x%"DTC_MS_U32_d"]Luma : 0x%"DTC_MS_PHY_x" chroma : 0x%"DTC_MS_PHY_x" (FRM: %"DTC_MS_U32_d")(PTS :%"DTC_MS_U64_d") (STSTUS :%"DTC_MS_U32_d")\n"
    //,i,stVDECDIPInfo[i].StreamId.u32Id,u16WritePointer,MsOS_GetSystemTime(),stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.u32Idx ,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Width, stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Height,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32LumaAddr,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ChromaAddr,MApi_VDEC_EX_GetFrameCnt(&stVDECDIPInfo[i].StreamId),stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus);
//fflush(fpPush);

//===================================

                if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Display)
                {
                    MSAPI_XC_VDECFRAME_INFO stXC_VDECFRAME_INFO;
                    memset(&stXC_VDECFRAME_INFO,0,sizeof(MSAPI_XC_VDECFRAME_INFO));

                    _Demo_VDEC_StoreFrameInfoVDEC2msAPI(&stXC_VDECFRAME_INFO,i,u16WritePointer);

                    msAPI_XC_DIPMultiView_PushFrame(&stXC_VDECFRAME_INFO);

                    stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus = E_VDEC_Frame_Release;

                    stVDECDIPInfo[i].stDIPFrameSyncInfo.u32DispCnt++;

                    if(fpPush)
                    {
                        fprintf(fpPush,"[id:%"DTC_MS_U32_d"][stream ID: 0x%8"DTC_MS_U32_x"][WP:%"DTC_MS_U32_d"][SYS: %"DTC_MS_U32_d"] [Idx:%"DTC_MS_U32_d"]PUSH [%"DTC_MS_U32_d"x%"DTC_MS_U32_d"] Mfdec:[%d] MfdecSel:[%d] Luma : 0x%"DTC_MS_PHY_x" chroma : 0x%"DTC_MS_PHY_x" (FRM: %"DTC_MS_U32_d")(PTS :%"DTC_MS_U64_d") (STCENG :%"DTC_MS_U32_d")(STSTUS :%"DTC_MS_U32_d")\n"
                                ,i,stVDECDIPInfo[i].StreamId.u32Id,u16WritePointer,MsOS_GetSystemTime(),stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.u32Idx ,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Width, stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Height,stXC_VDECFRAME_INFO.stFrames[0].stHWFormat.stMFdecInfo.bMFDec_Enable,stXC_VDECFRAME_INFO.stFrames[0].stHWFormat.stMFdecInfo.u8MFDec_Select,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32LumaAddr,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ChromaAddr,MApi_VDEC_EX_GetFrameCnt(&stVDECDIPInfo[i].StreamId),stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].u64TimeStemp,stVDECDIPInfo[i].stDIPFrameSyncInfo.u32STCEng,stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus);
                        fflush(fpPush);

                        MS_U32 u32DiffTime = MsOS_GetSystemTime()- u32TempTime[i];

                        if(u32DiffTime >= 1000)
                        {
                            printf("[id:%"DTC_MS_U32_d"][SYS:%"DTC_MS_U32_d"][NowSYS:%"DTC_MS_U32_d"][Disp Cnt:%"DTC_MS_U32_d"] \n",i,u32TempTime[i],MsOS_GetSystemTime(),stVDECDIPInfo[i].stDIPFrameSyncInfo.u32DispCnt-u32TempCnt[i]);
                            u32TempTime[i] = MsOS_GetSystemTime();
                            u32TempCnt[i] = stVDECDIPInfo[i].stDIPFrameSyncInfo.u32DispCnt;
                        }
                    }
                }

                if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Display
                     || stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Release)
                {
                    stVDECDIPInfo[i].stFrameWriteReadPointer.u16WritePointer = (u16WritePointer + 1) % VDEC_MAX_DQNUM;

                }
                else if(stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Repeat
                    || stVDECDIPInfo[i].stDIPFrameInfo[u16WritePointer].eDIPFrameStatus == E_VDEC_Frame_Invalid)
                {
                    stVDECDIPInfo[i].stFrameWriteReadPointer.u16WritePointer = u16WritePointer;
                }
            }
        }
        MsOS_DelayTask(1);
    }

    //NdecodeDBG_print("\033[35m _Demo_VDEC_GetFrame E_VDEC_ReallyStop \033[0m\n");
    stVDECDIPFrameController.eGetTaskStatus = E_VDEC_ReallyStop;

    return TRUE;
}

MS_BOOL _Demo_VDEC_ReleaseFrame(void)
{
    MS_U16 u16ReadPointer = 0;

    while(stVDECDIPFrameController.eReleaseTaskStatus != E_VDEC_Stop)
    {
        int i = 0;

        for(i = 0 ; i < VDEC_MAX_SUPPORT_STREAM_NUM ; i++)
        {
            if(stVDECDIPInfo[i].eDecodeStatus == E_VDEC_Start)
            {
                u16ReadPointer = stVDECDIPInfo[i].stFrameWriteReadPointer.u16ReadPointer;

                NdecodeDBG_print("\033[36m[id:%"DTC_MS_U32_d"][stream ID: 0x%8"DTC_MS_U32_x"][ReadPointer: %"DTC_MS_U32_d"][SYS: %"DTC_MS_U32_d"] [u32Idx: %"DTC_MS_U32_d"]Release [%"DTC_MS_U32_d"x%"DTC_MS_U32_d"]Luma : 0x%"DTC_MS_PHY_x" chroma : 0x%"DTC_MS_PHY_x" (FRM: %"DTC_MS_U32_d")(PTS :%"DTC_MS_U64_d") (STSTUS :%"DTC_MS_U32_d")\033[0m\n"
                ,i,stVDECDIPInfo[i].StreamId.u32Id,u16ReadPointer,MsOS_GetSystemTime(),stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.u32Idx ,stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Width, stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u16Height,stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32LumaAddr,stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].stDispFrames[0].stDIPDispFrameInfo.stFrmInfo.u32ChromaAddr,MApi_VDEC_EX_GetFrameCnt(&stVDECDIPInfo[i].StreamId),stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].u64TimeStemp,stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].eDIPFrameStatus);

                if(stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].eDIPFrameStatus == E_VDEC_Frame_Release)
                {
                    MS_BOOL bIsSecondField = FALSE;
                    _Demo_VDEC_Release_VDEC_Frame(i,u16ReadPointer,bIsSecondField);

                    if(stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].u32ScanType == 3) //InterlacedField  = 3
                    {
                        bIsSecondField = TRUE;
                        _Demo_VDEC_Release_VDEC_Frame(i,u16ReadPointer,bIsSecondField);
                    }

                    //NdecodeDBG_print("\033[37m Release OKOKOK!!!!!!\033[0m\n");
                    memset(&stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer],0,sizeof(ST_VDEC_DIP_FrameInfo));
                    stVDECDIPInfo[i].stDIPFrameInfo[u16ReadPointer].eDIPFrameStatus = E_VDEC_Frame_Invalid;

                    stVDECDIPInfo[i].stFrameWriteReadPointer.u16ReadPointer = (u16ReadPointer + 1) % VDEC_MAX_DQNUM;
                }
            }
        }
        MsOS_DelayTask(1);
    }

    //NdecodeDBG_print("\033[35m [%s][%d] _Demo_VDEC_ReleaseFrame E_VDEC_ReallyStop \033[0m\n",__FUNCTION__,__LINE__);
    stVDECDIPFrameController.eReleaseTaskStatus = E_VDEC_ReallyStop;

    return TRUE;
}

MS_BOOL Demo_VDEC_DIP_FrameController(VDEC_StreamId *pHandle,MS_U32 *pu32DecoderIdx)
{
    if(stVDECDIPInfo[*pu32DecoderIdx].eDecodeStatus == E_VDEC_NotInital)
    {
        stVDECDIPInfo[*pu32DecoderIdx].eDecodeStatus = E_VDEC_Start;
        stVDECDIPInfo[*pu32DecoderIdx].StreamId = *pHandle;
        NdecodeDBG_print("\033[35m Demo_VDEC_DIP_FrameController [%d] \033[0m\n",*pu32DecoderIdx);
    }

    // --------------------------------------------------------
    // Step :  create GET Task_Info

    if(stVDECDIPFrameController.eGetTaskStatus == E_VDEC_NotInital)
    {
        if (stVDECDIPFrameController.stGetFrameTask.iId == INVALID_TASK_ID)
        {
            stVDECDIPFrameController.eGetTaskStatus = E_VDEC_Start;
            stVDECDIPFrameController.stGetFrameTask.iId = MsOS_CreateTask((TaskEntry)_Demo_VDEC_GetFrame,
                                                 0,
                                                 stVDECDIPFrameController.stGetFrameTask.ePriority,
                                                 TRUE,
                                                 stVDECDIPFrameController.stGetFrameTask.pStack,
                                                 stVDECDIPFrameController.stGetFrameTask.u32StackSize,
                                                 stVDECDIPFrameController.stGetFrameTask.szName);

            if (stVDECDIPFrameController.stGetFrameTask.iId == INVALID_TASK_ID)
            {
                printf("[%s][%d] Create Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            else
            {
                NdecodeDBG_print("%"DTC_MS_U32_d" \n",stVDECDIPFrameController.stGetFrameTask.iId);
            }

            // All success, delay and return 0
            MsOS_DelayTask(10);
        }

    }


    // --------------------------------------------------------
    // Step :  create RELEASE Task_Info
    if(stVDECDIPFrameController.eReleaseTaskStatus == E_VDEC_NotInital)
    {
        if (stVDECDIPFrameController.stReleaseFrameTask.iId == INVALID_TASK_ID)
        {
            stVDECDIPFrameController.eReleaseTaskStatus = E_VDEC_Start;
            stVDECDIPFrameController.stReleaseFrameTask.iId = MsOS_CreateTask((TaskEntry)_Demo_VDEC_ReleaseFrame,
                                                 0,
                                                 stVDECDIPFrameController.stReleaseFrameTask.ePriority,
                                                 TRUE,
                                                 stVDECDIPFrameController.stReleaseFrameTask.pStack,
                                                 stVDECDIPFrameController.stReleaseFrameTask.u32StackSize,
                                                 stVDECDIPFrameController.stReleaseFrameTask.szName);

            if (stVDECDIPFrameController.stReleaseFrameTask.iId == INVALID_TASK_ID)
            {
                printf("[%s][%d] Create Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            else
            {
                NdecodeDBG_print("%"DTC_MS_U32_d" \n",stVDECDIPFrameController.stReleaseFrameTask.iId);
            }

            MsOS_DelayTask(10);
        }
    }

    return TRUE;
}

MS_BOOL Demo_VDEC_DIP_StopWindow(EN_VDEC_Device* peDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    printf("\033[35m ========[%s][%d] u32Window[%d] bDIPStopType [%d]============ \033[0m\n",__FUNCTION__,__LINE__,stVDECInfo[*peDevice].stShowFrameInfo.u32Window,stVDECDIPInfo[*peDevice].bDIPStopType);
    // False: normal  TRUE:seamless
    if(stVDECDIPInfo[*peDevice].bDIPStopType == TRUE)
    {
        if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
        {
            msAPI_XC_DIPMultiView_InvalidateWindow((E_MSAPI_XC_WINDOW)stVDECInfo[*peDevice].stShowFrameInfo.u32Window); //need stop first
        }
#if(DEMO_DMS_TEST == 1)
        else if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
        {
            ST_VDEC_WIN_INFO stVDECWinInfo;
            memset(&stVDECWinInfo,0,sizeof(VDEC_WIN_INFO));

            stVDECWinInfo.win_id = stVDECInfo[*peDevice].stShowFrameInfo.u32Window;
            stVDECWinInfo.u8EnableSetDigitalSignal   = E_VSYNC_DIGITAL_SYNC_CTRL_FORCE_DIS;
            stVDECWinInfo.u8EnableClearDigitalSignal = E_VSYNC_DIGITAL_SYNC_CTRL_FORCE_DIS;
            Demo_VDEC_SetMode(peDevice,E_VDEC_SetCmd_SetVDECWinInfo,(void *)(&stVDECWinInfo));
        }
#endif
        if (MApi_VDEC_EX_Exit(&stVDECInfo[*peDevice].stVDECSteamID) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_EX_Exit Not OK %"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,stVDECInfo[*peDevice].stVDECSteamID.u32Id);
        }

    }
    else
    {
#if(DEMO_DMS_TEST == 1)
        if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
        {
            ST_VDEC_WIN_INFO stVDECWinInfo;
            memset(&stVDECWinInfo,0,sizeof(VDEC_WIN_INFO));

            stVDECWinInfo.win_id = stVDECInfo[*peDevice].stShowFrameInfo.u32Window;
            stVDECWinInfo.u8EnableSetDigitalSignal   = E_VSYNC_DIGITAL_SYNC_CTRL_DEFAULT;
            stVDECWinInfo.u8EnableClearDigitalSignal = E_VSYNC_DIGITAL_SYNC_CTRL_DEFAULT;
            Demo_VDEC_SetMode(peDevice,E_VDEC_SetCmd_SetVDECWinInfo,(void *)(&stVDECWinInfo));

            if(stVDECInfo[*peDevice].stShowFrameInfo.u32Window == 0)
            {
                // setting DMS main window DS
                MS_BOOL bEnableDS = FALSE;
                Demo_XC_SetDSOnOFF(NULL, bEnableDS);
            }

            if (MApi_VDEC_EX_Exit(&stVDECInfo[*peDevice].stVDECSteamID) != E_VDEC_EX_OK)
            {
                printf("[%s][%d] MApi_VDEC_EX_Exit Not OK %"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,stVDECInfo[*peDevice].stVDECSteamID.u32Id);
            }


        }
#endif
        msAPI_XC_DIPMultiView_Finalize((E_MSAPI_XC_WINDOW)stVDECInfo[*peDevice].stShowFrameInfo.u32Window);
    }

    return TRUE;
}

MS_BOOL Demo_VDEC_DIP_StopFrameController(EN_VDEC_Device* peDevice)
{
    int i=0;

    stVDECDIPInfo[*peDevice].eDecodeStatus = E_VDEC_Stop;

    NdecodeDBG_print("Demo_VDEC_DIP_StopFrameController [%d] eDecodeStatus:[%d] window:[%d]\n",*peDevice,stVDECDIPInfo[*peDevice].eDecodeStatus,stVDECDIPInfo[*peDevice].eWindow);
    NdecodeDBG_print("eGetTaskStatus[%d]eReleaseTaskStatus[%d]\n",stVDECDIPFrameController.eGetTaskStatus,stVDECDIPFrameController.eReleaseTaskStatus);

    for(i=0 ;  i < VDEC_MAX_SUPPORT_STREAM_NUM ; i++)
    {
        if(stVDECDIPInfo[i].eDecodeStatus == E_VDEC_Start)
        {
            memset(&stVDECDIPInfo[*peDevice], 0, sizeof(ST_VDEC_DIP_DispInfo));
            goto END;
        }
     }

    memset(&stVDECDIPInfo[*peDevice], 0, sizeof(ST_VDEC_DIP_DispInfo));

    stVDECDIPFrameController.eGetTaskStatus = E_VDEC_Stop;
    stVDECDIPFrameController.eReleaseTaskStatus = E_VDEC_Stop;

    NdecodeDBG_print("eGetTaskStatus[%d]eReleaseTaskStatus[%d]\n",stVDECDIPFrameController.eGetTaskStatus,stVDECDIPFrameController.eReleaseTaskStatus);

    while(1)
    {
        if(stVDECDIPFrameController.stGetFrameTask.iId == INVALID_TASK_ID)
        {
            NdecodeDBG_print("Demo_VDEC_DIP_StopFrameController no need stop\n");
            return TRUE;
        }

        if ((stVDECDIPFrameController.eGetTaskStatus == E_VDEC_ReallyStop)
            && (stVDECDIPFrameController.eReleaseTaskStatus == E_VDEC_ReallyStop))
        {
            stVDECDIPFrameController.stGetFrameTask.iId = INVALID_TASK_ID;
            stVDECDIPFrameController.stReleaseFrameTask.iId = INVALID_TASK_ID;

            stVDECDIPFrameController.eGetTaskStatus = E_VDEC_NotInital;
            stVDECDIPFrameController.eReleaseTaskStatus = E_VDEC_NotInital;
            NdecodeDBG_print("Demo_VDEC_DIP_StopFrameController STOP\n");
            return TRUE;
        }
        MsOS_DelayTask(10);
    }

END:
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief VDEC Is Support DS \n
/// @param[in] VDEC Stream ID
/// @return TRUE: Support DS.
/// @return FALSE: Not Support DS
/// @sa
/// @note
/// Sample Command:  \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_IsSupportDS(VDEC_StreamId stVDECStreamId)
{
     if (MApi_VDEC_EX_GetActiveCodecType(&stVDECStreamId) == E_VDEC_EX_CODEC_TYPE_AVS) // In DTV DS, AVS codec not support DS.
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL Demo_VDEC_FrameSyncOn(EN_VDEC_Device* peDevice,MS_BOOL *pbSync,MS_U32 *pu32SyncDelay,MS_U16 *pu16tolerance,MS_U32 *pu32FreerunThreshold)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    NdecodeDBG_print("================Demo_VDEC_FrameSyncOn [Device][SyncOnOff][SyncDelay][Tolerance][FreerunThreshold]================\n");
    NdecodeDBG_print("Device    [%d]\n",*peDevice);
    NdecodeDBG_print("SyncOnOff [%d]\n",*pbSync);
    NdecodeDBG_print("SyncDelay [%d]\n",*pu32SyncDelay);
    NdecodeDBG_print("Tolerance [%d]\n",*pu16tolerance);
    NdecodeDBG_print("FreerunThreshold [%d]\n",*pu32FreerunThreshold);
    NdecodeDBG_print("==============End Demo_VDEC_FrameSyncOn==============\n");


    if(*pu16tolerance == 0)
    {
        stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u16Tolerance = (float)(1/(float)(stVDECDIPInfo[*peDevice].u32FrameRate/1000)*1000); //ms
        printf("FrameRate[%"DTC_MS_U32_d"] New Tolerance[%d]\n",stVDECDIPInfo[*peDevice].u32FrameRate,stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u16Tolerance);
    }
    else
    {
        stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u16Tolerance= *pu16tolerance;
    }

    stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u16InputTolerance= *pu16tolerance;
    stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.bNeedSync = *pbSync;
    stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u32SyncDelay = *pu32SyncDelay;
    stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u32FreerunThreshold= *pu32FreerunThreshold;

    return TRUE;
}

MS_BOOL Demo_VDEC_SetMode(EN_VDEC_Device* peDevice,EN_VDEC_Cmd eVDECCmd,void* pVDECInfo)
{
    switch(eVDECCmd)
    {
        case E_VDEC_SetCmd_ShowFrameInfo:
            stVDECInfo[*peDevice].stShowFrameInfo = *((ST_VDEC_ShowFrameInfo *) pVDECInfo);

            NdecodeDBG_print("Demo_VDEC_SetMode eVDECCmd E_VDEC_SetCmd_ShowFrame\n");
            return TRUE;
        case E_VDEC_SetCmd_Initialize:

            memset(&stVDECInfo[*peDevice],0,sizeof(ST_VDEC_INFORMATION));
            memset(&stVDECDIPInfo[*peDevice],0,sizeof(ST_VDEC_DIP_DispInfo));
            stVDECInfo[*peDevice].eMvopInputSel = MVOP_INPUT_UNKNOWN;

            NdecodeDBG_print("Demo_VDEC_SetMode eVDECCmd E_VDEC_SetCmd_Initialize\n");
            return TRUE;
        case E_VDEC_SetCmd_DIPStopType:

            // False: normal  TRUE:seamless
            stVDECDIPInfo[*peDevice].bDIPStopType = *((MS_BOOL *) pVDECInfo);

            NdecodeDBG_print("Demo_VDEC_SetMode eVDECCmd E_VDEC_SetCmd_DIPStopType\n");
            return TRUE;
        case E_VDEC_SetCmd_StreamType:

            stVDECInfo[*peDevice].eVDECStreamType = *((VDEC_EX_Stream *) pVDECInfo);
            NdecodeDBG_print("Demo_VDEC_SetMode eVDECCmd E_VDEC_SetCmd_StreamType [%d][%d]\n",*peDevice,stVDECInfo[*peDevice].eVDECStreamType);
            return TRUE;
        case E_VDEC_SetCmd_PUSIControl:
            {
                MS_BOOL bEnable =  *((MS_BOOL *) pVDECInfo);
                MApi_VDEC_EX_SetControl(&stVDECInfo[*peDevice].stVDECSteamID, E_VDEC_EX_USER_CMD_PUSI_CONTROL,bEnable);

                db_print("Demo_VDEC_SetMode E_VDEC_SetCmd_PUSIControl bEnable[%d]\n",bEnable);
                return TRUE;
            }
        case E_VDEC_SetCmd_TimeOutMech:
            {
                VDEC_EX_Decode_Timeout_Param stParam;
                memset(&stParam,0,sizeof(VDEC_EX_Decode_Timeout_Param));
                stParam.bEnable = TRUE;
                stParam.u32Timeout = 80;
                MApi_VDEC_EX_SetControl(&stVDECInfo[*peDevice].stVDECSteamID,E_VDEC_EX_USER_CMD_ENABLE_DECODE_ENGINE_TIMEOUT,(MS_VIRT)&stParam);

                db_print("Demo_VDEC_SetMode E_VDEC_SetCmd_TimeOutMech\n");
                return TRUE;
            }
        case E_VDEC_SetCmd_SetVDECWinInfo:
#if (DEMO_DMS_TEST == 1)
            {
                VDEC_WIN_INFO stVDECWinInfo;
                memcpy(&stVDECWinInfo,(VDEC_WIN_INFO*)pVDECInfo,sizeof(VDEC_WIN_INFO));

                MApi_VDEC_EX_SetControl(&stVDECInfo[*peDevice].stVDECSteamID, E_VDEC_EX_USER_CMD_SET_WIN_INIT,(MS_VIRT)&stVDECWinInfo);

                printf("Demo_VDEC_SetMode E_VDEC_SetCmd_SetVDECWinInfo Device[%d] window[%d]\n",*peDevice,stVDECWinInfo.win_id);
                return TRUE;
            }
#endif
        case E_VDEC_SetCmd_MAX:
        default:
            NdecodeDBG_print("wrong eVDECCmd:%d!\n",eVDECCmd);
            return FALSE;
    }
}

// set dip config
MS_BOOL Demo_VDEC_SetDispConfig(EN_VDEC_Device* peDevice,EN_VDEC_ShowFrame_Mode* peMode)
{
    EN_VDEC_ShowFrame_Mode eMode = *peMode;

    VDEC_EX_DispInfo stDispinfo;
    memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
    Demo_VDEC_GetDispInfo(peDevice,&stDispinfo);

    switch(eMode)
    {
        case E_VDEC_MVOP_MODE:
        {
#ifdef DTV_HDR_VDEC_FLOW
            _Demo_VDEC_SetHDRCmd2msAPI(*peDevice);
#endif
            /////////////////////////
            //get video information//
            /////////////////////////
            MS_U8 u8MvopMiuSel = 0;
            MSAPI_XC_VDEC_DispInfo stMSAPIDispinfo;

            stVDECInfo[*peDevice].eMvopInputSel = _Demo_VDEC_GetMVOPInputSel(*peDevice,&stVDECInfo[*peDevice].stVDECSteamID);

            Demo_XC_VdecDispInfo2msAPIVdecDispInfo(&stDispinfo, &stMSAPIDispinfo);

#if (DEMO_DTV_DS_TEST == 1)
            if((stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_DTV) && Demo_VDEC_IsSupportDS(stVDECInfo[*peDevice].stVDECSteamID))
            {
                stMSAPIDispinfo.u16HorSize          = DYNAMIC_SCALING_VB_H;
                stMSAPIDispinfo.u16VerSize          = DYNAMIC_SCALING_VB_V;
            }
#endif
            E_MSAPI_XC_DECODER_PATH eXCDECPath;

#if(DEMO_VDEC_NDECODE_TEST == 1)
            eXCDECPath = stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_MvopPath;
            msAPI_XC_SetVDECStreamId(&stVDECInfo[*peDevice].stVDECSteamID, stVDECInfo[*peDevice].stShowFrameInfo.u32Window);
#else
            eXCDECPath = *peDevice;
#endif

            msAPI_XC_SetVDECInfo_EX_ByPath(eXCDECPath, stMSAPIDispinfo);


#if(DEMO_VDEC_NDECODE_TEST == 1)
            u8MvopMiuSel = VDEC_FRAME_BUF_MEMORY_TYPE & BIT(0);
#else
            if(E_VDEC_DEVICE_MAIN == *peDevice)
            {
                u8MvopMiuSel = VDEC_FRAME_BUF_MEMORY_TYPE & BIT(0);
            }
#if(DEMO_VDEC_MULTI_TEST == 1)
            else if(E_VDEC_DEVICE_SUB == *peDevice)
            {
                u8MvopMiuSel = VDEC_FRAME_BUF1_MEMORY_TYPE & BIT(0);
            }
#endif
#endif
            if(msAPI_XC_SetMVOPMiuSel_EX(eXCDECPath, TRUE, u8MvopMiuSel) != TRUE)
            {
                db_print("msAPI_XC_SetMVOPMiuSel_EX Failed !! \n");
                return FALSE;
            }
            if(msAPI_XC_SetMVOPConfig_EX(eXCDECPath, stVDECInfo[*peDevice].eMvopInputSel) != TRUE)
            {
                db_print("msAPI_XC_SetMVOPConfig_EX Failed !! \n");
                return FALSE;
            }

#if (DEMO_DTV_DS_TEST == 1)
            if((stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_DTV) && Demo_VDEC_IsSupportDS(stVDECInfo[*peDevice].stVDECSteamID))
            {
                MS_BOOL bEnableDS = TRUE;
                Demo_XC_SetDSOnOFF(&stDispinfo, bEnableDS);
            }
#endif

#if (DEMO_XC_DSSWDB_TEST == 1)
            if(stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_FILE)
            {
                db_print("Enable SWDBDS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                MS_U32 bEnableDS = TRUE;
                Demo_XC_EnableDS_SWDB(&bEnableDS);
            }
#endif
            return TRUE;
        }
        case E_VDEC_DIP_MODE:
        {
            stVDECDIPInfo[*peDevice].u32FrameRate = stDispinfo.u32FrameRate;
            stVDECDIPInfo[*peDevice].eWindow = stVDECInfo[*peDevice].stShowFrameInfo.u32Window;
            stVDECDIPInfo[*peDevice].stDIPFrameSyncInfo.u32STCEng = stVDECInfo[*peDevice].u32STCEng;

            MS_BOOL bSync = TRUE;
            MS_U32 u32SyncDelay = 180;
            MS_U16 u16tolerance = 0;
            MS_U32 u32FreerunThreshold = 5000; //ms
            Demo_VDEC_FrameSyncOn(peDevice,&bSync,&u32SyncDelay,&u16tolerance,&u32FreerunThreshold);

#if(DEMO_XC_DIPMultiWin_TEST == 1)
            MSAPI_GEGOP_TRANSPARENT_AREA_ID eAreaID = stVDECDIPInfo[*peDevice].eWindow;

            MSAPI_GEGOP_WINDOW_RECT stGEGOPRect;
            stGEGOPRect.x =  stVDECInfo[*peDevice].stShowFrameInfo.u16X;
            stGEGOPRect.y = stVDECInfo[*peDevice].stShowFrameInfo.u16Y;
            stGEGOPRect.width = stVDECInfo[*peDevice].stShowFrameInfo.u16Width;
            stGEGOPRect.height = stVDECInfo[*peDevice].stShowFrameInfo.u16Height;

            msAPI_GEGOP_SetTransparentArea(eAreaID,stGEGOPRect);
#endif
            return TRUE;

        }
        case E_VDEC_PureMCU_MODE:
        {

#ifdef DTV_HDR_VDEC_FLOW
            _Demo_VDEC_SetHDRCmd2msAPI(*peDevice);
#endif

#if((DEMO_XC_DIPMultiWin_TEST == 1) && (DEMO_DMS_TEST == 1))
            if(stVDECInfo[*peDevice].stShowFrameInfo.u32Window == 0)
            {
                // setting DMS main window DS
                MS_U8 u8parms = 0;
                Demo_VDEC_GetMode(peDevice,E_VDEC_GetCmd_GetDSIndexDepth,&u8parms);

                // assign dms using VDEC variable to setting ds
                VDEC_EX_DispInfo stDispinfo;
                memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));

                stDispinfo.u32DynScalingAddr = MEM_ADR_BY_MIU(SC0_MCU_DS_ADR, SC0_MCU_DS_MEMORY_TYPE);
                stDispinfo.bEnableMIUSel     = SC0_MCU_DS_MEMORY_TYPE & BIT(0);
                stDispinfo.u8DynScalingDepth = u8parms;

                MS_BOOL bEnableDS = TRUE;
                Demo_XC_SetDSOnOFF(&stDispinfo, bEnableDS);

            }
#endif
            return TRUE;
        }
        case E_VDEC_SWDetile_MODE:
        case E_VDEC_ShowFrame_MODE_MAX:
        default:
            return TRUE;

    }
}

//------------------------------------------------------------------------------
/// @brief Load the file video stream
/// @param[in] stream buffer address
/// @param[in] H.264 or MPEG2 or AVS or MVC
/// @Param[in] file path
/// @return u32FileSize.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U32 Demo_VDEC_UtlLoadStream(MS_U32 u32Addr,MS_U32 u32AddrSize,MS_U32 u32CodecType,char* pPath)
{
    MS_U8* pu8Fw = NULL;
    MS_U8* pu8BS = NULL;
    MS_U32 u32i = 0;
    MS_U32 u32FileRSize = 0;
    MS_U32 u32FileSize = 0;
    MS_S32 s32MstarNonCachedPoolID = 0;

    FILE* s32FdRead =NULL ;
    db_print("=====[%s][%d]===== pPath: %s\n",__FUNCTION__,__LINE__,pPath);

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if (pPath == NULL)
    {
        printf("Please enter the Path\n");
    }
    else
    {
        s32FdRead = MsFS_Fopen(pPath, "rb");
    }

    if (s32FdRead == NULL)
    {
        printf("File open error, please check dynamicavc.bin, dynamic.bin in storage \n");
        printf("File open path : /root/dynamicscale/xxxx.bin \n");
        return FALSE;
    }

    MsFS_Fseek(s32FdRead,0 ,SEEK_END);
    u32FileSize = MsFS_Ftell(s32FdRead);
    db_print("[%s][%d]File Size %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,u32FileSize);
    MsFS_Fseek(s32FdRead,0 ,SEEK_SET);

    if (pu8BS == NULL)
    {
        pu8BS = MsOS_AllocateMemory((u32FileSize + 1) *sizeof(MS_U8),s32MstarNonCachedPoolID);//add 1 byte for null terminated
    }

    if (pu8BS == NULL)
    {
         MsFS_Fclose(s32FdRead);
         printf("Memory allocate failed, file size = %"DTC_MS_U32_x" \n", u32FileSize);
         return FALSE;
    }

    db_print("[%s][%d] addr:0x%"DTC_MS_U32_x" size:%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,u32Addr, u32FileSize);
    {
        u32FileRSize = MsFS_Fread(pu8BS,1,u32FileSize,s32FdRead);
        if (u32FileRSize == 0)
        {
            MsFS_Fclose(s32FdRead);
            printf("[%s][%d] fread Failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            *(pu8BS+u32FileRSize) = '\0';//for coverity issue String not null terminated (STRING_NULL)
        }
    }

    db_print("[%s][%d] Load bitstream from 0x%p to 0x%"DTC_MS_U32_x", size 0x%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,pu8BS, u32Addr, u32FileSize);
    pu8Fw = (MS_U8 *)MS_PA2KSEG1(u32Addr);

    if (u32CodecType == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY)
    {
        memset(pu8Fw, 0, PLAY_SIZE_LIMITITATION - DRV_PROC_SIZE);
        if (u32FileSize > (PLAY_SIZE_LIMITITATION - DRV_PROC_SIZE))
        {
            u32FileSize = (PLAY_SIZE_LIMITITATION - DRV_PROC_SIZE);
        }

        memcpy(pu8Fw, pu8BS,u32FileSize );//VDEC_MVC_BIT_STREAM_LEN- DRV_PROC_SIZE
    }
    else
    {
        memset(pu8Fw, 0, u32AddrSize- DRV_PROC_SIZE);
        if (u32FileSize > (u32AddrSize - DRV_PROC_SIZE))
        {
            u32FileSize = (u32AddrSize - DRV_PROC_SIZE);
        }
        for (u32i = 0; u32i < u32FileSize; u32i++)
        {
            if (u32i<20)
            {
                db_print("%.2x ,",pu8Fw[u32i]);
            }
            pu8Fw[u32i] = pu8BS[u32i];
        }
    }

    MsOS_Sync();
    MsOS_FlushMemory();
    MsOS_FreeMemory((void *)pu8BS,s32MstarNonCachedPoolID);
    MsFS_Fclose(s32FdRead);

    db_print("\n[%s][%d] Demo_VDEC_UtlLoadStream Done\n",__FUNCTION__,__LINE__);
    return u32FileSize;
}

//------------------------------------------------------------------------------
/// @brief Set Flag to wait AV Sync or not \n
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
///          \b DTV_WaitAVSync 0 \n
///          \b DTV_WaitAVSync 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_WaitAVSync(MS_U32* pu32Enable)
{
    bVDEC_WaitAVSync = *pu32Enable;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set delay task time to Demo_VDEC_CheckDecoderStatus \n
/// @param[in] delay task time
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
///          \b DTV_DelayTaskTime 10 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_DelayTaskTime(MS_U32* pu32DelayTaskTime)
{
    u32VDEC_DelayTime = *pu32DelayTaskTime;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Set debug level for message
/// @param[in] pVideoDebugLv Debug Level
/// @return TRUE: Process success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_SetVDECDebugLevel 0 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_VDEC_SetVDECDebugLevel(EN_VDEC_Device* peDevice,MS_U32* u32VDECDebugLv)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    if (u32VDECDebugLv == NULL)
    {
        printf("[%s][%d]u32VDECDebugLv == NULL\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
        if (*u32VDECDebugLv > E_VDEC_EX_DBG_LEVEL_FW)
        {
            eVDECDebuglevel = E_VDEC_EX_DBG_LEVEL_TRACE;
        }
        else
        {
            eVDECDebuglevel = *u32VDECDebugLv;
        }
        MApi_VDEC_EX_SetDbgLevel(&(stVDECInfo[*peDevice].stVDECSteamID),eVDECDebuglevel);
        return TRUE;
    }
}

MS_BOOL Demo_VDEC_GetMode(EN_VDEC_Device* peDevice,EN_VDEC_Cmd eVDECCmd,void* pVDECInfo)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    switch(eVDECCmd)
    {
        case E_VDEC_GetCmd_VideoSrcMode:
            *((EN_VDEC_DDI_SrcMode *)pVDECInfo) = stVDECInfo[*peDevice].eVideoSrcMode;
            db_print("[%s][%d] Demo_VDEC_GetMode eVDECCmd:%d!\n", __FUNCTION__, __LINE__,eVDECCmd);
            return TRUE;
        case E_VDEC_GetCmd_GetDevice_ByStreamID:
        {
            int i ;
            for(i = 0;i < E_VDEC_DEVICE_MAX;i++)
            {
                if(stVDECInfo[i].stVDECSteamID.u32Id == ((VDEC_StreamId*)pVDECInfo)->u32Id)
                {
                    *((EN_VDEC_Device *)peDevice) = (EN_VDEC_Device)i;
                    return TRUE;
                }
            }
            return FALSE;
        }
        case E_VDEC_GetCmd_GetIsHDRStream:
            *((MS_U8 *)pVDECInfo) = stVDECInfo[*peDevice].u8IsHDRStream;
            db_print("[%s][%d] Demo_VDEC_GetMode eVDECCmd:%d!\n", __FUNCTION__, __LINE__,eVDECCmd);
            return TRUE;

        case E_VDEC_GetCmd_MvopInputSel:
            *((MVOP_InputSel *)pVDECInfo) = stVDECInfo[*peDevice].eMvopInputSel;
            db_print("[%s][%d] Demo_VDEC_GetMode eVDECCmd:%d!\n", __FUNCTION__, __LINE__,eVDECCmd);
            return TRUE;

        case E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS:
#if(DEMO_VDEC_NDECODE_TEST == 1)
        {

            VDEC_EX_BufferInfo parms;
            VDEC_EX_Result eRet;

            memset(&parms, 0 , sizeof(VDEC_EX_BufferInfo));

            parms.eType     = E_VDEC_EX_GET_BUFFERTYPE_ALLOC_BS;
            eRet = MApi_VDEC_EX_GetControl(&stVDECInfo[*peDevice].stVDECSteamID, E_VDEC_EX_USER_CMD_GET_BUFFER_INFO, (MS_U32 *)&parms);
            if(eRet == E_VDEC_EX_OK)
            {
                ((VDEC_EX_BufferInfo *)pVDECInfo)->phyAddr = parms.phyAddr;
                ((VDEC_EX_BufferInfo *)pVDECInfo)->szSize = parms.szSize;

                db_print("[%s][%d] Demo_VDEC_GetMode E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS: phyAddr:0x%llx, szSize:0x%x \n", __FUNCTION__, __LINE__,((VDEC_EX_BufferInfo *)pVDECInfo)->phyAddr,((VDEC_EX_BufferInfo *)pVDECInfo)->szSize);
                return TRUE;
            }
            else
                return FALSE;
        }
#endif

        case E_VDEC_GetCmd_GetDSIndexDepth:
        {
            MS_U8 u8IndexDepth = 0;
            #define DS_BUFFER_DEPTH     32
            // #define DS_BUFFER_DEPTH_EX  255 //prepare for HDR TCH & DV

            u8IndexDepth = DS_BUFFER_DEPTH;
            *((MS_U8 *)pVDECInfo) = u8IndexDepth;

            db_print("[%s][%d] Demo_VDEC_GetMode eVDECCmd:%d!\n", __FUNCTION__, __LINE__,eVDECCmd);
            return TRUE;
        }

        case E_VDEC_GetCmd_MAX:
        default:
            db_print("[%s][%d] wrong eVDECCmd:%d!\n", __FUNCTION__, __LINE__,eVDECCmd);
            return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief return the VDEC stream id
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
void* Demo_VDEC_GetStreamID(EN_VDEC_Device* peDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

#if(DEMO_VDEC_NDECODE_TEST == 0)

    VDEC_StreamId stStreamId = {0}, stGetStreamId = {0};
    VDEC_EX_User_Cmd eUserCmd = E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;

    if(*peDevice == E_VDEC_DEVICE_MAIN)
        eUserCmd =  E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;
    else
        eUserCmd =  E_VDEC_EX_USER_CMD_GET_SUB_STREAM_ID;

    if(MApi_VDEC_EX_GetControl(&stStreamId,eUserCmd,(MS_U32 *)&stGetStreamId)!= E_VDEC_EX_OK )
    {
        printf("Error, video[%d] not init!!\n Demo_VDEC_GetStreamID failed\n", *peDevice);
        return (void *)&stNullVDECStreamID;
    }

    stVDECInfo[*peDevice].bInited = TRUE;
    stVDECInfo[*peDevice].stVDECSteamID = stGetStreamId;
#endif

    return (void *)&stVDECInfo[*peDevice].stVDECSteamID;
}

//------------------------------------------------------------------------------
/// @brief to Get 3D Video Type \n
/// @param[in]  Main or Sub Device
/// @return Frame Pack type from VDEC
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Sample Command: \n
///          \b DTV_Get3DVideoTYPE 0 \n
//------------------------------------------------------------------------------
EN_VDEC_FrmPackMode Demo_VDEC_GetFramePackMode(EN_VDEC_Device* peDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    VDEC_EX_Frame_packing_SEI_EX stFramePack;

    if (stVDECInfo[*peDevice].bInited  == FALSE)
    {
        printf("Not Inited \n");
        return FALSE;
    }
    memset(&stFramePack, 0 , sizeof(VDEC_EX_Frame_packing_SEI_EX));
    stFramePack.stVerCtl.u32size = sizeof(VDEC_EX_Frame_packing_SEI_EX);
    if(MApi_VDEC_EX_GetControl(&(stVDECInfo[*peDevice].stVDECSteamID),E_VDEC_EX_USER_CMD_GET_FPA_SEI_EX,(MS_U32 *)&stFramePack) == E_VDEC_EX_FAIL )
    {
        printf("E_VDEC_EX_USER_CMD_GET_FPA_SEI_EX get fail \n");
        return FALSE;
    }

    db_print("Frame Packed Type %d \n",stFramePack.u8Frm_packing_arr_type);

    return (EN_VDEC_FrmPackMode)stFramePack.u8Frm_packing_arr_type;
}

//------------------------------------------------------------------------------
/// @brief Get Disp Info \n
/// @param[in]  Main or Sub Device
/// @param[in][out]  VDEC_EX_DispInfo pstDispinfo
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_GetDispInfo(EN_VDEC_Device* peDevice,VDEC_EX_DispInfo* pstDispinfo)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    if(MApi_VDEC_EX_GetDispInfo(&stVDECInfo[*peDevice].stVDECSteamID,pstDispinfo) != E_VDEC_EX_OK)
    {
        printf("MApi_VDEC_EX_GetDispInfo Failed !! \n");
        return FALSE;
    }
    else
    {
        printf("MApi_VDEC_EX_GetDispInfo Success !!\n");

    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get DecFrame Info \n
/// @param[in]  Main or Sub Device
/// @param[in][out]  VDEC_EX_FrameInfo pstFrameInfo
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_GetDecFrameInfo(EN_VDEC_Device* peDevice,VDEC_EX_FrameInfo* pstFrameInfo)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    if(MApi_VDEC_EX_GetDecFrameInfo(&stVDECInfo[*peDevice].stVDECSteamID,pstFrameInfo) != E_VDEC_EX_OK)
    {
        printf("MApi_VDEC_EX_GetDecFrameInfo Failed !! \n");
        return FALSE;
    }
    else
    {
        db_print("MApi_VDEC_EX_GetDecFrameInfo Success !!\n");
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get GetVideoPTS \n
/// @param[in]  Main or Sub Device
/// @param[in][out]  pu32VPTS (unit:ms)
/// @param[in][out]  pu64VPTS (unit:ms)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_GetVideoPTS(EN_VDEC_Device* peDevice, MS_U32* pu32VPTS, MS_U64* pu64VPTS)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32VPTS = INVALID_PTS;

    if((pu32VPTS == NULL) && (pu64VPTS == NULL))
    {
        printf("pu32VPTS or pu64VPTS is NULL\n");
        return FALSE;
    }

    if(MApi_VDEC_EX_IsFrameRdy(&stVDECInfo[*peDevice].stVDECSteamID) != E_VDEC_EX_OK )
    {
        return FALSE;
    }

    u32VPTS = MApi_VDEC_EX_GetPTS(&stVDECInfo[*peDevice].stVDECSteamID);

    if(pu32VPTS)
    {
        *pu32VPTS = u32VPTS;
    }

    if(pu64VPTS)
    {
        if(MApi_VDEC_EX_GetControl(&stVDECInfo[*peDevice].stVDECSteamID, E_VDEC_EX_USER_CMD_GET_U64PTS, (MS_U32*)pu64VPTS) != E_VDEC_EX_OK)
        {
            // Get 32bits if failed to get 64bits.
            *pu64VPTS = (MS_U64)u32VPTS;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief function to check AV sync is reach
/// @param[in] Main or Sub Device
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_IsAVSyncDone(EN_VDEC_Device* peDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    MS_BOOL bSyncFlag = FALSE;
    MS_U32 u32SyncStarTime = 0;
    MS_U32 u32SyncTimeDiff = 0;

    //////////////////////
    //check AV sync done  //
    //////////////////////

    u32SyncStarTime = _Demo_VDEC_GetTime();

    if(bVDEC_WaitAVSync == TRUE)
    {
        while(bSyncFlag != TRUE)
        {
            u32SyncTimeDiff = _Demo_VDEC_TimeDiffFromNow(u32SyncStarTime);
            db_print("SyncTimeDiff = %"DTC_MS_U32_u" \n",u32SyncTimeDiff);

            if(!MApi_VDEC_EX_IsAVSyncOn(&stVDECInfo[*peDevice].stVDECSteamID))
            {
                db_print("MApi_VDEC_EX_IsAVSyncOn Not ON\n");
                bSyncFlag = TRUE;
            }

            if(E_VDEC_EX_OK == MApi_VDEC_EX_IsReachSync(&stVDECInfo[*peDevice].stVDECSteamID))
            {
                db_print("MApi_VDEC_EX_IsReachSync OK\n");
                db_print("*****************Sync Done %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
                bSyncFlag = TRUE;
            }
            else if(E_VDEC_EX_FAIL == MApi_VDEC_EX_IsReachSync(&stVDECInfo[*peDevice].stVDECSteamID))
            {
                db_print("MApi_VDEC_EX_IsReachSync Not OK \n");
            }

            if (u32SyncTimeDiff > AVSYNC_TIMEOUT_INTERVAL) // set timeout
            {
                db_print("*****************Sync Timeout %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
                bSyncFlag = TRUE;
            }
            MsOS_DelayTask(u32VDEC_DelayTime);
        }
    }

    db_print("DIFF TIME to wait AV-Sync = %"DTC_MS_U32_u" ms\n", _Demo_VDEC_GetTime() - u32SyncStarTime);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Check Video Decoder Status and Set Output Config \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b DTV_CheckDecoderStatus 0 \n
///          \b DTV_CheckDecoderStatus 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_CheckDecoderStatus(EN_VDEC_Device* peDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    MS_U32 u32Loop = VDEC_FRAMECOUNT_LOOP;

    if (stVDECInfo[*peDevice].bInited == FALSE)
    {
        printf("%d Decoder Not Init \n",*peDevice);
        return FALSE;
    }

    ////////////////////////////////////////////////////////////
    //Check if information for display setting is ready or not                  //
    //And disable VDEC blue screen                                                  //
    ////////////////////////////////////////////////////////////

    u32StartTime = _Demo_VDEC_GetTime();

    while(MApi_VDEC_EX_CheckDispInfoRdy(&stVDECInfo[*peDevice].stVDECSteamID) != E_VDEC_EX_OK)
    {
        u32Loop--;
        if(u32Loop==0)
        {
            u32EndTime = _Demo_VDEC_GetTime();
            printf("Timeout!! Video DispInfo is Not Ready!!\n");
            printf("DIFF TIME to check Disp Info = %"DTC_MS_U32_u" ms\n", u32EndTime - u32StartTime);
            return FALSE;
        }
        MsOS_DelayTask(u32VDEC_DelayTime);
    }

    u32EndTime = _Demo_VDEC_GetTime();

    db_print("MApi_VDEC_EX_CheckDispInfoRdy is Ready!!\n");
    db_print("DIFF TIME to check Disp Info = %"DTC_MS_U32_u" ms\n", u32EndTime - u32StartTime);

    MApi_VDEC_EX_SetBlueScreen(&stVDECInfo[*peDevice].stVDECSteamID, FALSE );

    Demo_VDEC_SetDispConfig(peDevice,&stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode);

    if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_MVOP_MODE
        || stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
    {
        /////////////////////
        //  check frame Rdy   //
        /////////////////////

        u32Loop = VDEC_FRAMECOUNT_LOOP;
        u32StartTime = _Demo_VDEC_GetTime();
        while(MApi_VDEC_EX_IsFrameRdy(&stVDECInfo[*peDevice].stVDECSteamID) != E_VDEC_EX_OK )
        {
            u32Loop--;
            if(u32Loop==0)
            {
                printf("Timeout!! MApi_VDEC_EX_IsFrameRdy is Not Ready!!\n");
                printf("VDEC Error Code :%"DTC_MS_U32_d"!! %"DTC_MS_U32_u" ms \n",MApi_VDEC_EX_GetErrCode(&stVDECInfo[*peDevice].stVDECSteamID), u32EndTime - u32StartTime);
                return FALSE;
            }
            MsOS_DelayTask(u32VDEC_DelayTime);
        }
        u32EndTime = _Demo_VDEC_GetTime();

        db_print("MApi_VDEC_EX_IsFrameRdy is Ready!!\n");
        db_print("DIFF TIME to check VDEC Frame ready = %"DTC_MS_U32_u" ms\n",u32EndTime - u32StartTime);
    }
    else if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
    {
#if (DEMO_XC_DIPMultiWin_TEST == 1)
        NdecodeDBG_print("Dip control start!!!!! \n");
        Demo_VDEC_DIP_FrameController(&stVDECInfo[*peDevice].stVDECSteamID,peDevice);
#endif
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief VDEC init \n
/// @param[in]  Main or Sub Device
/// @param[in][out]  When init VDEC can get Stream ID
/// @param[in]  eVideoCodec video codec
/// @param[in]  VDEC Source Mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b none \n
//------------------------------------------------------------------------------

MS_BOOL Demo_VDEC_Init(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId,EN_VDEC_DDI_CodecType eVideoCodec,EN_VDEC_DDI_SrcMode eMode,MS_U32 u32STCEng)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    VDEC_EX_Status stVDECStatus;
    VDEC_EX_Stream eVDECStream = 0;
    VDEC_EX_CodecType eCodecType = 0;
    VDEC_EX_InitParam stInitParam;

    NdecodeDBG_print("\033[35m =========eShowFrame_Mode[%d]=========== \033[0m\n",stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode);

    stVDECInfo[*peDevice].eVideoCodec = eVideoCodec;
    stVDECInfo[*peDevice].u32STCEng = u32STCEng;
    stVDECInfo[*peDevice].eVideoSrcMode = eMode;

#if(DEMO_MVC_TEST == 1)
    if(stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY)
    {
        if (*peDevice > 0)
        {
            printf("[%s][%d] Wrong Decoder \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
#endif

    memset(&stVDECStatus, 0, sizeof(VDEC_EX_Status));
    MApi_VDEC_EX_GetStatus(pstVDECStreamId,&stVDECStatus);
    db_print("eCodecTypeCur : %d \n",MApi_VDEC_EX_GetActiveCodecType(pstVDECStreamId));

    memset(&stInitParam, 0, sizeof(VDEC_EX_InitParam));
    stInitParam.bDisableDropErrFrame = TRUE;
    stInitParam.eCodecType = eCodecType = _Demo_VDEC_MappingCodec(stVDECInfo[*peDevice].eVideoCodec);
    stInitParam.VideoInfo.eSrcMode = _Demo_VDEC_MappingSourceMode(stVDECInfo[*peDevice].eVideoSrcMode);

    _Demo_VDEC_InitSysConfigParam(*peDevice,&stInitParam.SysConfig);

#if (DEMO_DTV_DS_TEST == 1)
    if((stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_DTV) && Demo_VDEC_IsSupportDS(*pstVDECStreamId))
    {
        stInitParam.EnableDynaScale = TRUE;
        stInitParam.u32DSVirtualBoxHeight = DYNAMIC_SCALING_VB_V;
        stInitParam.u32DSVirtualBoxWidth = DYNAMIC_SCALING_VB_H;
        db_print("Enable DS\n");
    }
#endif

#if(DEMO_VDEC_NDECODE_TEST == 1)
    eVDECStream = stVDECInfo[*peDevice].eVDECStreamType;
#else
    if (*peDevice == E_VDEC_DEVICE_MAIN)
    {
        eVDECStream = E_VDEC_EX_MAIN_STREAM;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_VDEC_DEVICE_SUB)
    {
        eVDECStream = E_VDEC_EX_SUB_STREAM;
    }
#endif
#endif

    if (MApi_VDEC_EX_GetFreeStream((VDEC_StreamId *)pstVDECStreamId, sizeof(VDEC_StreamId), eVDECStream, (VDEC_EX_CodecType)stInitParam.eCodecType) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]....GetFreeStream Failed Stream ID:  0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__, pstVDECStreamId->u32Id);
        return FALSE;
    }
    else
    {
        db_print("[%s][%d] Stream ID:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,pstVDECStreamId->u32Id, pstVDECStreamId->u32Version);
    }

#if(DEMO_VDEC_MULTI_TEST == 1 && DEMO_XC_DIPMultiWin_TEST == 0)
    if(eCodecType == E_VDEC_EX_CODEC_TYPE_MPEG2
        || eCodecType == E_VDEC_EX_CODEC_TYPE_H264
        || eCodecType == E_VDEC_EX_CODEC_TYPE_AVS
        || eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC
        || eCodecType == E_VDEC_EX_CODEC_TYPE_VP9)
    {
        if(_Demo_VDEC_STC_Switch(*peDevice,pstVDECStreamId,stVDECInfo[*peDevice].u32STCEng) == FALSE)
        {
            printf("[%s][%d] eDevice = %d, Demo_VDEC_STC_Switch fail!!!\n",__FUNCTION__,__LINE__, *peDevice);
        }
    }

#endif

    if (Demo_VDEC_PreSetControl(peDevice,pstVDECStreamId) != TRUE)
    {
        printf("[%s][%d] Demo_VDEC_PreSetControl fail!!!\n",__FUNCTION__,__LINE__);
    }

    //MS_U32 u32StartTime= MsOS_GetSystemTime();

    //Init/Re-Init VDEC
    if (MApi_VDEC_EX_Init(pstVDECStreamId,&stInitParam) != E_VDEC_EX_OK)
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }

    NdecodeDBG_print("\033[34m [Time : %d]\033[0m\n",MsOS_GetSystemTime()-u32StartTime);

    stVDECInfo[*peDevice].stVDECSteamID = *pstVDECStreamId;

    NdecodeDBG_print("\033[35m ==========u32STCEng [%d]========== \033[0m\n",stVDECInfo[*peDevice].u32STCEng);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief VDEC Play \n
/// @param[in]  Main or Sub Device
/// @param[in]  VDEC Stream ID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b none \n
//------------------------------------------------------------------------------

MS_BOOL Demo_VDEC_Play(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId,EN_VDEC_AVSYNC_Type eAVSYNC_Mode)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32StartTime = 0;
    MS_U32 u32EndTime = 0;
    MS_U32 u32AVSyncDelay = 0;
    MS_BOOL bAVSyncOnOff = TRUE;

    u32StartTime = _Demo_VDEC_GetTime();

    NdecodeDBG_print("\033[35m =========eShowFrame_Mode[%d]=========== \033[0m\n",stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode);

    if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_MVOP_MODE
        || stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
    {
        if (eAVSYNC_Mode == E_VDEC_SYNC_PCR_MODE
            || eAVSYNC_Mode == E_VDEC_SYNC_VIDEO_MASTER_MODE)
        {
            u32AVSyncDelay = 180;
        }
        else if(eAVSYNC_Mode == E_VDEC_SYNC_AUDIO_MASTER_MODE)
        {
            //audio master with using TS file play command, and no decoder delay in this mode
            u32AVSyncDelay = 0;
        }

        if(eAVSYNC_Mode == E_VDEC_SYNC_VIDEO_MASTER_MODE)
            bAVSyncOnOff = FALSE;

        //If you want to change this value, please inform video and audio team at the same time
        if(MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,bAVSyncOnOff, u32AVSyncDelay, 0) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_AVSyncOn fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        //set the threshold of Freerun condition.If the difference of STC and PTS large than
        // threshold, FW will free run. The threshold value is 90KHZ base.
        if(MApi_VDEC_EX_SetAVSyncFreerunThreshold(pstVDECStreamId,450000) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

#if((DEMO_XC_DIPMultiWin_TEST == 1) && (DEMO_DMS_TEST == 0))
        if (stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_MvopPath == E_VDEC_DDI_DISPLAY_PATH_MVOP_MAIN)
        {
            MDrv_MVOP_Enable(DISABLE);
            printf("Display Main MVOP Disable\n");
        }
        else if (stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_MvopPath == E_VDEC_DDI_DISPLAY_PATH_MVOP_SUB)
        {
            MDrv_MVOP_SubEnable(DISABLE);
            printf("Display Sub MVOP Disable\n");
        }
#elif(DEMO_XC_DIPMultiWin_TEST == 0)
        if (*peDevice == E_VDEC_DEVICE_MAIN)
        {
            MDrv_MVOP_Enable(DISABLE);
            db_print("Display Main MVOP Disable\n");
        }
#if(DEMO_VDEC_MULTI_TEST == 1)
        else if (*peDevice == E_VDEC_DEVICE_SUB)
        {
            MDrv_MVOP_SubEnable(DISABLE);
            db_print("Display Sub MVOP Disable\n");
        }
#endif
#endif

        MApi_VDEC_EX_SetEvent(pstVDECStreamId, E_VDEC_EX_EVENT_DISP_INFO_CHG , _Demo_VDEC_Callback, pstVDECStreamId);

        //Set Field Delay count befor VDEC play
        MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
        MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT, ENABLE);  //set cmd to vdec need first decode push to the display Q,that can quickly framerdy
        MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xff);

#ifdef DTV_HDR_VDEC_FLOW
#ifdef XC_HDR_ADR
        MS_PHY phyAddr = MEM_ADR_BY_MIU(XC_HDR_ADR, XC_HDR_MEMORY_TYPE);
        MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_DV_XC_SHM_ADDR, (MS_U32)&phyAddr);
#endif
#endif

#if (DEMO_DTV_DS_TEST == 1)
        if((stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_DTV) && Demo_VDEC_IsSupportDS(stVDECInfo[*peDevice].stVDECSteamID))
        {
            // To fix DS P to I to P stream
            MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_FORCE_INTERLACE_MODE,6);
        }
#endif

        //set for FRC
        MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD, TRUE);

        /* Control for Crop information */
        MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP,TRUE);

    }
    else if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
    {

        NdecodeDBG_print("\033[35m =========DIP Play===========\033[0m\n");

        MApi_VDEC_EX_SetEvent(pstVDECStreamId, E_VDEC_EX_EVENT_DISP_INFO_CHG , _Demo_VDEC_Callback, pstVDECStreamId);

        if(MApi_VDEC_EX_SetBlockDisplay(pstVDECStreamId, TRUE)!= E_VDEC_EX_OK)
        {
            NdecodeDBG_print("set MApi_VDEC_EX_SetBlockDisplay fail\n");
            return FALSE;
        }
        else
            NdecodeDBG_print("MApi_VDEC_EX_SetBlockDisplay OK!\n");

        if(MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE, TRUE) != E_VDEC_EX_OK)
        {
            NdecodeDBG_print("set control fail\n");
            return FALSE;
        }
        else
            NdecodeDBG_print("MApi_VDEC_EX_SetControl OK!\n");

        MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_DROP_ERR_FRAME, TRUE);
        NdecodeDBG_print("set E_VDEC_EX_USER_CMD_DROP_ERR_FRAME TRUE\n");

    }

    if (MApi_VDEC_EX_Play(pstVDECStreamId) != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    u32EndTime = _Demo_VDEC_GetTime();
    printf("[%s][%d] DIFF TIME of DemoAV_PlayAV = %"DTC_MS_U32_u" ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);

    stVDECInfo[*peDevice].bInited = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief VDEC Stop \n
/// @param[in]  Main or Sub Device
/// @param[in]  VDEC Stream ID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b none \n
//------------------------------------------------------------------------------

MS_BOOL Demo_VDEC_Stop(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

#ifdef DTV_HDR_VDEC_FLOW
    if(stVDECInfo[*peDevice].u8IsHDRStream)
    {
        MsOS_DelayTask(50);  // wait cfd update variable from shm
        //End fire for CFD
        msAPI_XC_CFD_Control(NULL,E_MSAPI_XC_CFD_FIRE,NULL);

        stVDECInfo[*peDevice].u8IsHDRStream = 0;
    }
#endif

#if (DEMO_DTV_DS_TEST == 1)
    if(stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_DTV)
    {
        MS_BOOL bEnableDS = FALSE;
        Demo_XC_SetDSOnOFF(NULL, bEnableDS);
    }
#endif

#if (DEMO_XC_DSSWDB_TEST == 1)
    if(stVDECInfo[*peDevice].eVideoSrcMode == E_VDEC_DDI_SRC_MODE_FILE)
    {
        db_print("Disable SWDBDS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        MS_U32 bEnableDS = FALSE;
        Demo_XC_EnableDS_SWDB(&bEnableDS);
    }
#endif

#if (DEMO_XC_DIPMultiWin_TEST == 1)
    if(stVDECInfo[*peDevice].stShowFrameInfo.bValid == TRUE)
    {
        if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
        {
            Demo_VDEC_DIP_StopWindow(peDevice);
            Demo_VDEC_DIP_StopFrameController(peDevice);
            memset(&stVDECDIPInfo[*peDevice],0,sizeof(ST_VDEC_DIP_DispInfo));
        }
        else if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
        {
            Demo_VDEC_DIP_StopWindow(peDevice);
            memset(&stVDECDIPInfo[*peDevice],0,sizeof(ST_VDEC_DIP_DispInfo));
        }

        memset(&stVDECInfo[*peDevice],0,sizeof(ST_VDEC_INFORMATION));
        return TRUE;
    }
#endif

    if (MApi_VDEC_EX_Exit(pstVDECStreamId) != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_Exit Not OK %"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,pstVDECStreamId->u32Id);
    }

    memset(&stVDECInfo[*peDevice],0,sizeof(ST_VDEC_INFORMATION));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Decode I-Frame \n
/// @param[in]  Main or Sub Device
/// @param[in] 0:MPEG2 1.H.264 2.AVS 3.HEVC
/// @Param[in] file path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: NA\n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_DecIFrame(EN_VDEC_Device* peDevice,int* pCodec, char* pLogopath)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    MS_U32 u32FrameBufAddr = 0;
    MS_U32 u32StreamBufAddr = 0;
    MS_U32 u32StreamBufEnd = 0;
    MS_U32 u32ActualBitstreamSize = 0;
    VDEC_EX_FrameInfo stFrameInfo;

    db_print("=====[%s][%d]===== pLogopath: %s\n",__FUNCTION__,__LINE__,pLogopath);

    memset(&stFrameInfo,0,sizeof(stFrameInfo));

    stVDECInfo[*peDevice].eVideoCodec = *pCodec;

    if (Demo_VDEC_Init(peDevice,&(stVDECInfo[*peDevice].stVDECSteamID),stVDECInfo[*peDevice].eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE,INVALID_FILEIN_ID) == FALSE) // default mepg2
    {
        printf("[%s][%d] Demo_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
         db_print("[%s][%d] Stream ID After Init  0x%"DTC_MS_U32_x"\n",__FUNCTION__, __LINE__,stVDECInfo[*peDevice].stVDECSteamID.u32Id);
    }
#if(DEMO_VDEC_NDECODE_TEST == 1)

    VDEC_EX_BufferInfo parms;
    memset(&parms, 0 , sizeof(VDEC_EX_BufferInfo));
    if(Demo_VDEC_GetMode(peDevice,E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS,&parms))
    {
        MS_SIZE szDstSize = 0;
        u32StreamBufAddr = parms.phyAddr;
        szDstSize        = parms.szSize;
        u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32StreamBufAddr,szDstSize,stVDECInfo[*peDevice].eVideoCodec,pLogopath);
        u32StreamBufEnd = u32StreamBufAddr + u32ActualBitstreamSize;
    }
#else
    if (*peDevice == E_VDEC_DEVICE_MAIN)
    {
        u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
        u32StreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE) ;
        u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32StreamBufAddr,VDEC_BIT_STREAM_LEN,stVDECInfo[*peDevice].eVideoCodec,pLogopath);
        u32StreamBufEnd = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE) + u32ActualBitstreamSize;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_VDEC_DEVICE_SUB)
    {
        u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
        u32StreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32StreamBufAddr,VDEC_BIT_STREAM1_LEN,stVDECInfo[*peDevice].eVideoCodec,pLogopath);
        u32StreamBufEnd = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE) + u32ActualBitstreamSize;
    }
#endif
#endif

#if(DEMO_CMA_TEST == 1)
    u32FrameBufAddr = 0;
#endif

    if (MApi_VDEC_EX_MHEG_DecodeIFrame(&stVDECInfo[*peDevice].stVDECSteamID,u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEnd) != E_VDEC_EX_OK)
    {
        printf("MApi_VDEC_EX_MHEG_DecodeIFrame failed!\n");
        return FALSE;
    }
    else
    {
        db_print("*******************************************************\n");
        db_print("MApi_VDEC_MHEG_DecodeIFrame good\n");
        db_print("*******************************************************\n");
    }

    stVDECInfo[*peDevice].bInited = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_VDEC_FilePlay \n
/// @param[in]  0 : for Main 1: for Sub
/// @param[in]  VDEC Stream ID
/// @param[in]  u32Address : Video1 Stream address
/// @param[in]  u32ActualBitstreamSize : Video1 Stream file size
/// @param[in]  u32PushTime : push times
/// @param[in]  u32Address1 : Video2 Stream address
/// @param[in]  u32ActualBitstreamSize1 : Video2 Stream file size
/// @param[in]  eShowFrame_Mode : EN_VDEC_ShowFrame_Mode : 0:E_VDEC_MVOP_MODE, 2:E_VDEC_DIP_MODE, 3:E_VDEC_SWDetile_MODE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b none  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_FilePlay(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId,MS_U32 u32Address,MS_U32 u32ActualBitstreamSize,MS_U32 u32PushTime,MS_U32 u32Address2,MS_U32 u32ActualBitstreamSize2,EN_VDEC_ShowFrame_Mode eShowFrame_Mode)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    MS_BOOL bRdy2SetVOP = FALSE;
    MS_BOOL bEnableSecondStream = FALSE;
    MS_U32 u32i = 0;
    MS_U32 u32TotalSize = u32ActualBitstreamSize;
    MS_U32 u32TotalSize2 = u32ActualBitstreamSize2;
    MS_U32 u32PlaySizeLim = 0;

    VDEC_EX_DecCmd stDecCmd;
    VDEC_EX_DecCmd stDecCmd2;
    VDEC_EX_DispInfo stDispinfo;

    MS_S32 s32MstarNonCachedPoolID = 0;

    if(stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY)
    {
        if (*peDevice > 0)
        {
            printf("[%s][%d] Wrong Decoder \n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        u32PlaySizeLim = PLAY_SIZE_LIMITITATION;
    }
    else if (stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
    {

#if(DEMO_VDEC_NDECODE_TEST == 1)
        VDEC_EX_BufferInfo parms;
        memset(&parms, 0 , sizeof(VDEC_EX_BufferInfo));

        if(Demo_VDEC_GetMode(peDevice,E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS,&parms))
        {
            u32PlaySizeLim  = parms.szSize;
        }
#else
        u32PlaySizeLim = VDEC_BIT_STREAM_LEN;
#endif
    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    stVDECInfo[*peDevice].stFilePlayInfo.u32PlaySize = u32ActualBitstreamSize;
    stVDECInfo[*peDevice].stFilePlayInfo.u32Address = u32Address;
    stVDECInfo[*peDevice].stFilePlayInfo.u32PlaySize2 = u32ActualBitstreamSize2;
    stVDECInfo[*peDevice].stFilePlayInfo.u32Address2 = u32Address2;
    stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode = eShowFrame_Mode;

    if (MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,TRUE, 0, 0) != E_VDEC_EX_OK)
    {
        printf("set MApi_VDEC_EX_AVSyncOn fail\n");
        return FALSE;
    }

    if(stVDECInfo[*peDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_DIP_MODE)
    {
        if(MApi_VDEC_EX_SetBlockDisplay(pstVDECStreamId, TRUE)!= E_VDEC_EX_OK)
        {
            printf("set MApi_VDEC_EX_SetBlockDisplay fail\n");
            return FALSE;
        }
        else
            db_print("MApi_VDEC_EX_SetBlockDisplay OK!\n");

        if(MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE, TRUE) != E_VDEC_EX_OK)
        {
            printf("set control fail\n");
            return FALSE;
        }
        else
            db_print("MApi_VDEC_EX_SetControl OK!\n");

        if(MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_DISP_FINISH_MODE, TRUE) != E_VDEC_EX_OK)
        {
            db_print("set DISP_FINISH control fail\n");
            //return FALSE;
        }
        else
            db_print("MApi_VDEC_EX_SetControl DISP_FINISH OK!\n");

        if (MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,FALSE, 0, 0) != E_VDEC_EX_OK)
        {
            printf("set MApi_VDEC_EX_AVSyncOn fail\n");
            return FALSE;
        }
     /*
        if (MApi_VDEC_EX_SetTrickMode(pstVDECStreamId, E_VDEC_EX_TRICK_DEC_I) != E_VDEC_EX_OK)
        {
            printf("[%s][%d]MApi_VDEC_EX_SetTrickMode FAIL!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        printf("[%s][%d]DECODE I Play!\n",__FUNCTION__,__LINE__);
        */
    }

    if (MApi_VDEC_EX_Play(pstVDECStreamId) != E_VDEC_EX_OK)
    {
        return FALSE;
    }
    db_print("[%s][%d]Play!\n",__FUNCTION__,__LINE__);

    if ((u32Address2 != 0) && (u32ActualBitstreamSize2 != 0))
    {
        bEnableSecondStream = TRUE;
        db_print("[%s][%d]play mvc with 2 stream !\n",__FUNCTION__,__LINE__);
     }

    // push BBU
    memset(&stDecCmd, 0, sizeof(VDEC_EX_DecCmd));
    memset(&stDecCmd2, 0, sizeof(VDEC_EX_DecCmd));

    if (stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
        || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
    {
        stDecCmd.u32StAddr = DRV_PROC_SIZE;
        if (u32TotalSize > PLAYLOOP_PUSH_SIZE)
        {
            stDecCmd.u32Size = PLAYLOOP_PUSH_SIZE;
            u32TotalSize = u32TotalSize - PLAYLOOP_PUSH_SIZE;
        }
        else
        {
            stDecCmd.u32Size = u32TotalSize;
        }
        stDecCmd.u32ID_H = 0;
        stDecCmd.u32ID_L = 0;
        stDecCmd.u32Timestamp = 0;

        if (bEnableSecondStream)
        {
            stDecCmd2.u32StAddr = u32Address2;
            if (u32TotalSize2 > PLAYLOOP_PUSH_SIZE)
            {
                 stDecCmd2.u32Size = PLAYLOOP_PUSH_SIZE;
                 u32TotalSize2 = u32TotalSize2 - PLAYLOOP_PUSH_SIZE;
            }
            else
            {
                 stDecCmd2.u32Size = u32TotalSize2;
            }
            stDecCmd2.u32ID_H = 0;
            stDecCmd2.u32ID_L = 0;
            stDecCmd2.u32Timestamp = 0;

        }
    }
    else
    {
        stDecCmd.u32StAddr = DRV_PROC_SIZE;
        stDecCmd.u32Size = u32ActualBitstreamSize;
        stDecCmd.u32ID_H = 0;
        stDecCmd.u32ID_L = 0;
        stDecCmd.u32Timestamp = 0;
    }

    if (MApi_VDEC_EX_PushDecQ(pstVDECStreamId,&stDecCmd) != E_VDEC_EX_OK)
    {
        printf("=====[%s][%d]=====MApi_VDEC_EX_PushDecQ fail\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        db_print("=====[%s][%d]=====MApi_VDEC_EX_PushDecQ OK\n",__FUNCTION__,__LINE__);
    }

    if (MApi_VDEC_EX_FireDecCmd(pstVDECStreamId) != E_VDEC_EX_OK)
    {
        printf("=====[%s][%d]=====MApi_VDEC_EX_FireDecCmd fail\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (bEnableSecondStream)
    {
        if (MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET, (MS_U32)&stDecCmd2) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_EX_SetControl fail\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        if (MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD, (MS_U32)&stDecCmd2) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_EX_SetControl fail\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }

    MsOS_FlushMemory();
    MsOS_DelayTask(u32VDEC_DelayTime);

    while (!bRdy2SetVOP)
    {
        bRdy2SetVOP = (MApi_VDEC_EX_CheckDispInfoRdy(pstVDECStreamId) == E_VDEC_EX_OK);

        //Get Frame info to set video timing

        if (MApi_VDEC_EX_GetDispInfo(pstVDECStreamId,&stDispinfo) != E_VDEC_EX_OK)
        {
            printf("[%s][%d] MApi_VDEC_EX_GetDispInfo fail pstVDECStreamId: %"DTC_MS_U32_x" bRdy2SetVOP:%d\n",__FUNCTION__,__LINE__,pstVDECStreamId->u32Id,bRdy2SetVOP);
        }

        db_print("vdecDispinfo.u16HorSize = %u\n", stDispinfo.u16HorSize);
        db_print("vdecDispinfo.u16VerSize = %u\n", stDispinfo.u16VerSize);
        db_print("vdecDispinfo.u8Interlace = %u\n", stDispinfo.u8Interlace);
        db_print("vdecDispinfo.u32FrameRate = %"DTC_MS_U32_u"\n", stDispinfo.u32FrameRate);

        stDispinfo.u8AFD = MApi_VDEC_EX_GetActiveFormat(pstVDECStreamId);

        if (u32i++ >= 100)
        {
            MApi_VDEC_EX_Exit(pstVDECStreamId);
            printf("[%s][%d] MApi_VDEC_EX_GetDispInfo Failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }

    db_print("bRdy2SetVOP becomes TRUE when u32i = %"DTC_MS_U32_d"\n", u32i);
    db_print("before SetMVOP:: picCnt = %"DTC_MS_U32_d"\n", MApi_VDEC_EX_GetFrameCnt(pstVDECStreamId));

    if (u32PushTime == 0)
    {
        /* Create task for video play */
        if (PlayLoop_Task[*peDevice].iId == INVALID_TASK_ID)
        {
            PlayLoop_Task[*peDevice].pStack = MsOS_AllocateMemory(PlayLoop_Task[*peDevice].u32StackSize,s32MstarNonCachedPoolID);
            db_print("[%s][%d] MsOS_AllocateMemory OK \n",__FUNCTION__,__LINE__);
        }

        if (PlayLoop_Task[*peDevice].pStack == NULL )
        {
            printf("[%s][%d] allocate stack fail\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        PlayLoop_Task[*peDevice].iId = MsOS_CreateTask((TaskEntry)_Demo_VDEC_PlayLoopTask,
                                             (MS_U32) *peDevice,
                                             PlayLoop_Task[*peDevice].ePriority, // E_TASK_PRI_MEDIUM,
                                             TRUE,
                                             PlayLoop_Task[*peDevice].pStack,
                                             PlayLoop_Task[*peDevice].u32StackSize,
                                             PlayLoop_Task[*peDevice].szName);
        if (PlayLoop_Task[*peDevice].iId == INVALID_TASK_ID)
        {
            printf("[%s][%d] Create Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            db_print("[%s][%d] %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,PlayLoop_Task[*peDevice].iId);
        }


        while(bPlayLoopActive != TRUE)
        {
            MsOS_DelayTask(u32VDEC_DelayTime);
        }
    }
    else
    {
        while (u32PushTime--)
        {
            if (stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
                || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
                || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
                || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
                || stVDECInfo[*peDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
            {
                if ((u32TotalSize > PLAYLOOP_PUSH_SIZE) && (u32TotalSize < u32PlaySizeLim))
                {
                     if (u32TotalSize == u32ActualBitstreamSize)
                     {
                         stDecCmd.u32StAddr = DRV_PROC_SIZE;
                     }
                     else
                     {
                         stDecCmd.u32StAddr = stDecCmd.u32StAddr + PLAYLOOP_PUSH_SIZE;
                     }

                     if (u32TotalSize >=  PLAYLOOP_PUSH_SIZE)
                     {
                         stDecCmd.u32Size = PLAYLOOP_PUSH_SIZE;
                         u32TotalSize = u32TotalSize - PLAYLOOP_PUSH_SIZE;
                     }
                     else
                     {
                         stDecCmd.u32Size = u32TotalSize;
                         u32TotalSize = u32ActualBitstreamSize;
                     }

                }
                else
                {
                     if (u32TotalSize  == u32ActualBitstreamSize)
                     {
                         stDecCmd.u32StAddr = DRV_PROC_SIZE;
                         stDecCmd.u32Size = u32ActualBitstreamSize;
                     }
                     else
                     {
                         stDecCmd.u32StAddr = stDecCmd.u32StAddr + PLAYLOOP_PUSH_SIZE;
                         stDecCmd.u32Size = u32TotalSize;

                     }
                     u32TotalSize = u32ActualBitstreamSize;
                }

                stDecCmd.u32ID_H = 0;
                stDecCmd.u32ID_L = 0;
                stDecCmd.u32Timestamp = 0;
            }

            if (MApi_VDEC_EX_PushDecQ(pstVDECStreamId,&stDecCmd) != E_VDEC_EX_OK)
            {
                printf("[%s][%d] MApi_VDEC_EX_PushDecQ fail\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            if (MApi_VDEC_EX_FireDecCmd(pstVDECStreamId) != E_VDEC_EX_OK)
            {
                printf("[%s][%d] MApi_VDEC_EX_FireDecCmd fail\n",__FUNCTION__,__LINE__);
                return FALSE;
            }

            if (bEnableSecondStream)
            {
                if ((u32TotalSize2 > PLAYLOOP_PUSH_SIZE) && (u32TotalSize2 < PLAY_SIZE_LIMITITATION))
                {
                     if (u32TotalSize2 == u32ActualBitstreamSize2)
                     {
                         stDecCmd2.u32StAddr = u32Address2;
                     }
                     else
                     {
                         stDecCmd2.u32StAddr = stDecCmd.u32StAddr + PLAYLOOP_PUSH_SIZE;
                     }

                     if (u32TotalSize2 >=  PLAYLOOP_PUSH_SIZE)
                     {
                         stDecCmd2.u32Size = PLAYLOOP_PUSH_SIZE;
                         u32TotalSize2 = u32TotalSize2 - PLAYLOOP_PUSH_SIZE;
                     }
                     else
                     {
                         stDecCmd2.u32Size = u32TotalSize2;
                         u32TotalSize2 = u32ActualBitstreamSize2;
                     }

                }
                else
                {
                     if (u32TotalSize2  == u32ActualBitstreamSize2)
                     {
                         stDecCmd2.u32StAddr = u32Address2;
                         stDecCmd2.u32Size = u32ActualBitstreamSize2;
                     }
                     else
                     {
                         stDecCmd2.u32StAddr = stDecCmd2.u32StAddr + PLAYLOOP_PUSH_SIZE;
                         stDecCmd2.u32Size = u32TotalSize2;

                     }
                     u32TotalSize2 = u32ActualBitstreamSize2;
                }

                stDecCmd2.u32ID_H = 0;
                stDecCmd2.u32ID_L = 0;
                stDecCmd2.u32Timestamp = 0;

                if (MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET, (MS_U32)&stDecCmd2) != E_VDEC_EX_OK)
                {
                    printf("[%s][%d] MApi_VDEC_EX_SetControl fail\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }

                if (MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD, (MS_U32)&stDecCmd2) != E_VDEC_EX_OK)
                {
                    printf("[%s][%d] MApi_VDEC_EX_SetControl fail\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            MsOS_FlushMemory();
            MsOS_DelayTask(u32VDEC_DelayTime);
        }
    }

    printf("[%s][%d] Decnt %"DTC_MS_U32_d", Errorcnt %"DTC_MS_U32_d", Skipcnt %"DTC_MS_U32_d", Display cnt %"DTC_MS_U32_d" PTS %"DTC_MS_U32_d"\n",__FUNCTION__,__LINE__,
        MApi_VDEC_EX_GetFrameCnt(pstVDECStreamId),
        MApi_VDEC_EX_GetErrCnt(pstVDECStreamId),
        MApi_VDEC_EX_GetSkipCnt(pstVDECStreamId),
        MApi_VDEC_EX_GetDispCnt(pstVDECStreamId),
        MApi_VDEC_EX_GetPTS(pstVDECStreamId));

    stVDECInfo[*peDevice].bInited = TRUE;

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief to Demo_VDEC_FileStop \n
/// @param[in]  0 : for Main 1: for Sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b none  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_FileStop(EN_VDEC_Device* peDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    Demo_VDEC_Stop(peDevice,&(stVDECInfo[*peDevice].stVDECSteamID));

    if (PlayLoop_Task[*peDevice].iId !=  INVALID_EVENT_ID)
    {
        MS_S32 s32MstarNonCachedPoolID = 0;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        MsOS_FreeMemory(PlayLoop_Task[*peDevice].pStack,s32MstarNonCachedPoolID);
        PlayLoop_Task[*peDevice].iId =  INVALID_EVENT_ID;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SET VDEC TIMESHIFT SEAMLESS FEATURE
/// @param[in]  VDEC Stream ID
/// @param[in]  1 : decode pause 2: reset HW (ES wp/rp reset) 3: seek to I mode (error handle)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: no
//------------------------------------------------------------------------------

MS_BOOL Demo_VDEC_TimeshiftSeamlessControl(VDEC_StreamId* pstVDECStreamId, MS_U32 u32Param)
{
    db_print("Set TimeshiftSeamless Control(0x%"DTC_MS_U32_x", %"DTC_MS_U32_u")\n", pstVDECStreamId->u32Id, u32Param);
    return MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE, u32Param);
}

//------------------------------------------------------------------------------
/// @brief OPEN VDEC DBG MSG
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b VDEC_OpenDBGMSG 3 1000 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_VDEC_OpenDBGMSG(MS_U8 *u8DbgMsg , MS_U32 *u32IntervalTime)
{

    printf("======================VDEC_DBGMSG======================\n");
    printf("0x0 : No DEBUG\n");
    printf("0x1 : Main VDEC\n");
    printf("0x2 : Sub VDEC\n");
    printf("0x3 : All VDEC\n");
    printf("0x4 0 : VDEC Main Call Back function: DISP_ONE\n");
    printf("0x4 1 : VDEC Sub Call Back function: DISP_ONE\n");
    printf("0x5 0 : VDEC Main DISP Info\n");
    printf("0x5 1 : VDEC Sub DISP Info\n");
    printf("0x6 0 : disable VDEC Main Call Back function: DISP_ONE\n");
    printf("0x6 1 : disable VDEC Sub Call Back function: DISP_ONE\n");
    printf("====================End VDEC_DBGMSG====================\n");

    u8VDECDbgMsg = *u8DbgMsg;

    if(*u32IntervalTime == 0 || *u32IntervalTime >= 5000)
    {
       u32VDECDbgIntervalTime = 1000;
    }
    else
    {
       u32VDECDbgIntervalTime = *u32IntervalTime;
    }

    if(u8VDECDbgMsg > 0 && u8VDECDbgMsg < 0x4 && s32_VDECDbgTaskID == INVALID_TASK_ID)
    {
        if(!_Demo_VDEC_DBGCreateTask())
        {
            printf("[%s][%d]create VDEC DBG task fail!!!!!!!!!!! OH NO \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
    else if(u8VDECDbgMsg == 0 && s32_VDECDbgTaskID != INVALID_TASK_ID)
    {
        MS_S32 s32MstarNonCachedPoolID = 0;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

        MsOS_FreeMemory(pStackbase,s32MstarNonCachedPoolID);
        s32_VDECDbgTaskID = INVALID_TASK_ID;
        printf("[%s][%d]close the VDEC debug task\n",__FUNCTION__,__LINE__);
    }

    if(u8VDECDbgMsg > 0x3)
    {
        printf("[%s][%d]u8VDECDbgMsg  : 0x%x\n",__FUNCTION__,__LINE__,u8VDECDbgMsg);
        if(u8VDECDbgMsg < 0x7)
        {
            VDEC_StreamId *stVDECSteamID = NULL;
            EN_VDEC_Device eDevice = *u32IntervalTime;
            stVDECSteamID = Demo_VDEC_GetStreamID(&eDevice);

            if (stVDECSteamID == NULL)
                return FALSE;

            if(u8VDECDbgMsg == 0x4)
            {
                if(stVDECInfo[eDevice].stShowFrameInfo.eShowFrame_Mode == E_VDEC_PureMCU_MODE)
                {
                    printf("[%s][%d]open file\n",__FUNCTION__,__LINE__);

                    if(u32VDECDbgCnt == 0)
                    {
                        FILE *fp;
                        int FileHandleIdx = 0;

                        while(TRUE)
                        {
                            sprintf(CallBackDBGoutfilename, "/mnt/sda1/%s_%03u.txt", "PureMCUDBG_Callback", FileHandleIdx++);
                            fp = MsFS_Fopen(CallBackDBGoutfilename, "r");
                            if (!fp)
                            {
                                // doesnt exist
                                break;
                            }
                            // exists
                            MsFS_Fclose(fp);
                        }

                        printf("[%s][%d]CallBackDBGoutfilename[%s]!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__,CallBackDBGoutfilename);

                        fpCallBackDBG = MsFS_Fopen(CallBackDBGoutfilename, "w");

                        if(NULL == fpCallBackDBG)
                        {
                            printf("[%s][%d]CallBackDBGoutfilename Fail!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
                        }
                        else
                        {
                            printf("[%s][%d]CallBackDBGoutfilename OK!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
                        }
                    }

                }

                printf("[%s][%d]VDEC %d Call Back function u32VDECDbgCnt [%"DTC_MS_U32_d"]\n",__FUNCTION__,__LINE__,eDevice,u32VDECDbgCnt);
                u32VDECDbgCnt++;

                MApi_VDEC_EX_SetEvent(stVDECSteamID,E_VDEC_EX_EVENT_DISP_ONE, _Demo_VDEC_Callback, stVDECSteamID);
            }
            if(u8VDECDbgMsg == 0x5)
            {
                VDEC_EX_DispInfo stDispinfo;
                memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
                Demo_VDEC_GetDispInfo(&eDevice,&stDispinfo);

                printf("\033[35m =========DISPINFO=========== \033[0m\n");
                printf("\033[35m u16HorSize: %d u16VerSize: %d \033[0m\n",stDispinfo.u16HorSize,stDispinfo.u16VerSize);
                printf("\033[35m u32FrameRate: %"DTC_MS_U32_d" u8Interlace: %d\033[0m\n",stDispinfo.u32FrameRate,stDispinfo.u8Interlace);
                printf("\033[35m =========DISPINFO Done=========== \033[0m\n");

            }
            if(u8VDECDbgMsg == 0x6)
            {
                printf("[%s][%d]Disable VDEC %d Call Back function u32VDECDbgCnt[%"DTC_MS_U32_d"]\n",__FUNCTION__,__LINE__,eDevice,u32VDECDbgCnt);
                u32VDECDbgCnt--;

                MApi_VDEC_EX_UnsetEvent(stVDECSteamID,E_VDEC_EX_EVENT_DISP_ONE);
                if(fpCallBackDBG && u32VDECDbgCnt == 0)
                {
                    MsFS_Sync();
                    MsFS_Fflush(fpCallBackDBG);
                    MsFS_Fclose(fpCallBackDBG);
                }
            }
        }
        else if(u8VDECDbgMsg == 0x7)
        {
            printf("[%s][%d]open file\n",__FUNCTION__,__LINE__);

            FILE *fp;
            int FileHandleIdx = 0;
            int FileHandleIdx1 = 0;

            while(TRUE)
            {
                sprintf(Pushoutfilename, "/mnt/sda1/%s_%03u.txt", "DTVPlayerDBG_PUSH", FileHandleIdx++);
                fp = MsFS_Fopen(Pushoutfilename, "r");
                if (!fp)
                {
                    // doesnt exist
                    break;
                }
                // exists
                MsFS_Fclose(fp);
            }

            printf("[%s][%d]Pushoutfilename[%s]!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__,Pushoutfilename);

            fpPush = MsFS_Fopen(Pushoutfilename, "w");

            if(NULL == fpPush)
            {
                printf("[%s][%d]Pushoutfilename Fail!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
            }
            else
            {
                printf("[%s][%d]Pushoutfilename OK!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
            }

            while(TRUE)
            {
                sprintf(Syncoutfilename, "/mnt/sda1/%s_%03u.txt", "DTVPlayerDBG_SYNC", FileHandleIdx1++);
                fp = MsFS_Fopen(Syncoutfilename, "r");
                if (!fp)
                {
                    // doesnt exist
                    break;
                }
                // exists
                MsFS_Fclose(fp);
            }

            printf("[%s][%d]Syncoutfilename[%s]!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__,Syncoutfilename);

            fpSync = MsFS_Fopen(Syncoutfilename, "w");

            if(NULL == fpSync)
            {
                printf("[%s][%d]Syncoutfilename Fail!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
            }
            else
            {
                printf("[%s][%d]Syncoutfilename OK!!!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
            }
        }
        else if(u8VDECDbgMsg == 0x8)
        {
            if(fpPush)
            {
                MsFS_Sync();
                MsFS_Fflush(fpPush);
                MsFS_Fclose(fpPush);

                printf("[%s][%d]close [%s] file\n",__FUNCTION__,__LINE__,Pushoutfilename);
            }

            if(fpSync)
            {
                MsFS_Sync();
                MsFS_Fflush(fpSync);
                MsFS_Fclose(fpSync);

                printf("[%s][%d]close [%s] file\n",__FUNCTION__,__LINE__,Syncoutfilename);
            }
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show SW De-Tile
/// @param[in] *pu32Device Main or Sub decoder
/// @param[in] *GOPNum the GOP Number
/// @param[in] VDEC_EX_FrameInfo info
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_SW_DeTile(EN_VDEC_Device* peDevice, MS_U8 *pu8GOPNum, VDEC_EX_FrameInfo *pstDispFrame)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    MS_U16 frameHSize = 0;
    MS_U16 frameVSize = 0;
    MS_U16 framePitch = 0;
    GOP_GwinInfo gwinAttr;
    MS_U8 GOPMaxNum = 0;
    MS_U8 GWINNum = 0;
    MS_U32 i,j = 0;
    // set VA address
    MS_U32 virGOPAddr = 0;
    MS_U32 virLumaAddr = 0;
    MS_U32 virChromaAddr = 0;
    MS_U8* pvirGOPAddr = NULL;
    MS_U8* pvirLumaAddr = NULL;
    MS_U8* pvirChromaAddr = NULL;

    // Clear memory
    memset(&gwinAttr,0,sizeof(gwinAttr));

    //Get support GOP number
    GOPMaxNum = MApi_GOP_GWIN_GetMaxGOPNum();

    if(*pu8GOPNum >= GOPMaxNum)
    {
        printf("WANRING: GOP Max Number is %d, GOP Number %d exceeded\n",GOPMaxNum,*pu8GOPNum);
        return FALSE;
    }

    // Get VDEC I-Frame Info
    frameHSize = pstDispFrame->u16Width;
    frameVSize = pstDispFrame->u16Height;
    framePitch = pstDispFrame->u16Pitch;

    //Get GOP GWIN number
    switch(*pu8GOPNum)
    {
        case 0:
            GWINNum = GOP0_GWIN_START;
            break;
        case 1:
            GWINNum = GOP1_GWIN_START;
            break;
        case 2:
            GWINNum = GOP2_GWIN_START;
            break;
        case 3:
            GWINNum = GOP3_GWIN_START;
            break;
        case 4:
            GWINNum = GOP4_GWIN_START;
            break;
        case 5:
            GWINNum = GOP5_GWIN_START;
            break;
        default:
            return FALSE;
    }

    // set GWIN FB adderss
    MApi_GOP_GWIN_GetWinInfo(GWINNum, &gwinAttr);

    // set VA address
    virGOPAddr = (MS_U32)MsOS_PA2KSEG1(gwinAttr.u32DRAMRBlkStart);
    virLumaAddr = (MS_U32)MsOS_PA2KSEG1(pstDispFrame->u32LumaAddr);
    virChromaAddr = (MS_U32)MsOS_PA2KSEG1(pstDispFrame->u32ChromaAddr);
    pvirGOPAddr = (MS_U8*)virGOPAddr;
    pvirLumaAddr = (MS_U8*)virLumaAddr;
    pvirChromaAddr = (MS_U8*)virChromaAddr;
    memset((void*)(MS_U32*)virGOPAddr,0x00,frameHSize*frameVSize*2);

// tile format
    #define TILE_W 16UL
    #define TILE_H 32UL

    if(frameHSize % TILE_W)
        printf("WANRING:VDEC HSize is %d ,not aligned tile format HSize : %ld .\n",frameHSize, TILE_W);
    if(framePitch % TILE_W)
        printf("WANRING:VDEC Putch is %d ,not aligned tile format HSize : %ld .\n",framePitch, TILE_W);
    if(frameVSize % TILE_H)
        printf("WANRING:VDEC VSize is %d ,not aligned tile format VSize : %ld .\n",frameVSize, TILE_H);

    for(j = 0 ; j < frameVSize; j++)
    {
        for(i = 0 ; i < frameHSize; i++)
        {
            // Calculate tile Y-component,x and y adderss.
            //Calculate x is whitch one tile block : (i / TILE_W) *(TILE_W * TILE_H) , and index in the block :(i % TILE_W)
            MS_U32 tilei = (i / TILE_W) * (TILE_W * TILE_H) + (i % TILE_W);
            //Calculate y is whitch one tile block : (j / TILE_H) * ((TILE_W * TILE_H) * (framePitch/ TILE_W)), and index in the block :((j % TILE_H) * TILE_W)
            MS_U32 tilej = (j / TILE_H) * ((TILE_W * TILE_H) * (framePitch/ TILE_W)) + ((j % TILE_H) * TILE_W);
            //Get tile Y-component value
            MS_U8 value = *(pvirLumaAddr+tilei+tilej);

            //Set tile Y-component value to YUV422 Y-component
            *(pvirGOPAddr+i*2+(j*frameHSize*2)) = value;

            // Because tile is four Y-component use one group UV-component
            if(j%2==0)
            {
                // Calculate tile UV-component,x and y adderss
                //Calculate x is whitch one tile block : (i / TILE_W) *(TILE_W * TILE_H) , and index in the block :(i % TILE_W)
                MS_U32 uvtilei = (i / TILE_W) * (TILE_W * TILE_H) + (i % TILE_W);
                //Calculate y is whitch one tile block : ((j/2) / TILE_H) * ((TILE_W * TILE_H) * (framePitch / TILE_W)), and index in the block :(((j/2) % TILE_H) * TILE_W)
                MS_U32 uvtilej = ((j/2) / TILE_H) * ((TILE_W * TILE_H) * (framePitch / TILE_W)) + (((j/2) % TILE_H) * TILE_W);
                //Get tile UV-component value
                MS_U8 valueUV = *(pvirChromaAddr+uvtilei+uvtilej);
                //Do UV swap here,because tile UV-component and GOP UV-component array is different
                if(i % 2 == 0)
                {
                    *(pvirGOPAddr+(i+1)*2+(j*frameHSize*2)+1) = valueUV;
                }
                else
                {
                    *(pvirGOPAddr+(i-1)*2+(j*frameHSize*2)+1) = valueUV;
                }
            }
            else
            {
                //Because tile is four Y-component use one group UV-component and YUV422 s two Y-component use one group UV-component,
                //so need copy tile UV-component to next line
                MS_U8 valueUV = *(pvirGOPAddr+i*2+((j-1)*frameHSize*2)+1);
                *(pvirGOPAddr+i*2+(j*frameHSize*2)+1) = valueUV;
            }
        }
    }
    MApi_GOP_GWIN_Enable(GWINNum, TRUE);//enable GWIN

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief cmd to Pre Set Control  : cmd to set the control before VDEC init \n
/// @param[in] Main or Sub Device
/// @param[in] PreSetControl
/// @return TRUE: Process success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Sample Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_CmdPreSetControl(EN_VDEC_Device* peDevice,MS_U32* pu32CmdPreSetControl)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    printf("======================VDEC_CmdPreSetControl======================\n");
    printf("0x1 : FORCE_8BIT_DEC_MODE\n");
    printf("====================End VDEC_CmdPreSetControl====================\n");

    stVDECInfo[*peDevice].u32CmdPreSetControl = *pu32CmdPreSetControl;

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Pre Set Control  : set the control before VDEC init \n
/// @param[in] Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Sample Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_PreSetControl(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId)
{
    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

#if(DEMO_VDEC_ONE_PENDING_BUFFER_MODE_TEST == 1)
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE, TRUE);
#endif

#if(DEMO_VDEC_LOWESTCLK_TEST == 1)
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_SYSTEM_PRESET_MVD_CLOCK,E_VDEC_EX_CLOCK_SPEED_LOWEST);
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_SYSTEM_PRESET_HVD_CLOCK,E_VDEC_EX_CLOCK_SPEED_LOWEST);
    db_print("\033[35m Preset : E_VDEC_EX_CLOCK_SPEED_LOWEST \033[0m\n");
#endif

    //VDEC will CAL FRAMERATE when frame rate not in SPS
    MApi_VDEC_EX_PreSetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_CAL_FRAMERATE, TRUE);

    _Demo_VDEC_PreSetControl_NdecodeSetting(peDevice,pstVDECStreamId);

    if(stVDECInfo[*peDevice].u32CmdPreSetControl & 0x1)
        MApi_VDEC_EX_PreSetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_FORCE_8BIT_DEC_MODE, TRUE);

    return TRUE;
}

#endif
