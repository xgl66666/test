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
#if (DEMO_VDEC_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <time.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"

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
#include "demo_xc.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/// VDEC check device ID
#if (DEMO_VDEC_MULTI_TEST == 1)
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
#define VDEC_DBG                1
#define VDEC_FRAMECOUNT_LOOP    300
//#define E_HVD_GDATA_DEC_Q_NUMB 0x1307
//#define E_HVD_GDATA_DISP_Q_NUMB 0x1308

#define INVALID_TASK_ID     -1
#define INVALID_EVENT_ID    -1
#define INVALID_PTS         -1

#define TASK_STACK_SIZE             0x2000
#define PLAYLOOP_PUSH_SIZE          0x180000


#if VDEC_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
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

static VDEC_EX_DbgLevel eVDECDebuglevel = E_VDEC_EX_DBG_LEVEL_NONE;
static VDEC_StreamId stNullVDECStreamID = {0,0};

static ST_VDEC_INFORMATION stVDECInfo[E_VDEC_DEVICE_MAX]= {{FALSE,E_VDEC_DDI_CODEC_TYPE_MPEG2,0,0,0,0,0,{0,0},E_VDEC_MVOP_MODE},
                                                           {FALSE,E_VDEC_DDI_CODEC_TYPE_MPEG2,1,0,0,0,0,{0,0},E_VDEC_MVOP_MODE}};

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
        if (stVDECInfo[eDevice].u32PlaySize2 != 0)
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
                if (stVDECInfo[eDevice].u32PlaySize > PLAYLOOP_PUSH_SIZE)
                {
                    u32TotalSize = stVDECInfo[eDevice].u32PlaySize - PLAYLOOP_PUSH_SIZE;
                    stDecCmd.u32StAddr = DRV_PROC_SIZE;
                }
                else
                {
                    u32TotalSize = stVDECInfo[eDevice].u32PlaySize;
                }

                if (bEnableSecondStream)
                {
                    if (stVDECInfo[eDevice].u32PlaySize2 > PLAYLOOP_PUSH_SIZE)
                    {
                        u32TotalSize2 = stVDECInfo[eDevice].u32PlaySize2 - PLAYLOOP_PUSH_SIZE;
                        stDecCmd2.u32StAddr = stVDECInfo[eDevice].u32Address2;
                    }
                    else
                    {
                        u32TotalSize2 = stVDECInfo[eDevice].u32PlaySize2;
                    }
                }
            }
            else if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY)
            {
                stDecCmd.u32StAddr = DRV_PROC_SIZE;
                u32TotalSize = stVDECInfo[eDevice].u32PlaySize;
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
                        if (u32TotalSize == stVDECInfo[eDevice].u32PlaySize)
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
                                u32TotalSize = stVDECInfo[eDevice].u32PlaySize;
                            }
                        }
                        stDecCmd.u32ID_H = 0;
                        stDecCmd.u32ID_L = 0;
                        stDecCmd.u32Timestamp = 0;

                        if (bEnableSecondStream)
                        {
                            if (u32TotalSize2 == stVDECInfo[eDevice].u32PlaySize2)
                            {
                                if (u32TotalSize2 > PLAYLOOP_PUSH_SIZE)
                                {
                                    stDecCmd2.u32StAddr = stVDECInfo[eDevice].u32Address2;
                                    stDecCmd2.u32Size = PLAYLOOP_PUSH_SIZE;
                                    u32TotalSize2 = u32TotalSize2 - PLAYLOOP_PUSH_SIZE;
                                }
                                else
                                {
                                    stDecCmd2.u32StAddr = stVDECInfo[eDevice].u32Address2;
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
                                    u32TotalSize2 = stVDECInfo[eDevice].u32PlaySize2;
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
                        stDecCmd.u32Size = stVDECInfo[eDevice].u32PlaySize;
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


            if(stVDECInfo[eDevice].eShowFrame_Mode == E_VDEC_SWDetile_MODE)
            {
                if(MApi_VDEC_EX_GetNextDispFrame(&stVDECInfo[eDevice].stVDECSteamID,&pstDispFrame) != E_VDEC_EX_OK)
                {
                    db_print("MApi_VDEC_EX_GetNextDispFrame FAIL!!!!!\n");
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
                                db_print("MApi_VDEC_EX_ReleaseFrame FAIL!!!\n");
                        }
                    }
                }
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
    if (u32Event & E_VDEC_EVENT_DISP_INFO_CHG)
    {
       printf("E_VDEC_EVENT_DISP_INFO_CHG!!!\n");
       MApi_VDEC_EX_SetBlueScreen(param, FALSE);
    }

    if (u32Event & E_VDEC_EX_EVENT_DISP_ONE)
    {
        MS_U32 u32VESlevel = 0;
        MS_U32 u32VWptr = 0;
        MS_U32 u32VRptr = 0;
        MS_U32 u32BufferSize = VDEC_BIT_STREAM_LEN;
        MS_U32 u32stc32 = 0;
        MS_U32 u32stc = 0;
        MS_U32 u32pcr32 = 0;
        MS_U32 u32pcr = 0;
        MS_U32 u32SetSTC = 0;
        MS_U32 u32MvopBank = 0;
        MS_U8 u8Streamidx = (MS_U8)(((VDEC_StreamId*)param)->u32Id >> 24);

        if (u8Streamidx == 0 )
        {
            u32MvopBank = 0x101400;
        }
        else if (u8Streamidx == 1 )
        {
            u32MvopBank = 0x103D00;
        }

        u32SetSTC = stVDECInfo[u8Streamidx].u32SetSTC;

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

        MApi_DMX_Stc_Eng_Get(u32SetSTC,&u32stc32, &u32stc);
        MApi_DMX_Pcr_Eng_Get(u32SetSTC,&u32pcr32, &u32pcr);
        MApi_XC_Write2ByteMask((u32MvopBank | 0x5E), 0x4000, 0xE000);//RIU debug register select = 2

        printf("[%d] systime: %"DTC_MS_U32_d",F: %"DTC_MS_U32_d",Dis: %"DTC_MS_U32_d",Err: %"DTC_MS_U32_d",Skip: %"DTC_MS_U32_d",Drop: %"DTC_MS_U32_d",PCR: %"DTC_MS_U32_d",STC: %"DTC_MS_U32_d",PTS: %"DTC_MS_U32_d",[Field]%u ,[FDMASK]%u ,Delta: %"DTC_MS_U32_d",VES: 0x%"DTC_MS_U32_x",WPtr: 0x%"DTC_MS_U32_x",Rptr: 0x%"DTC_MS_U32_x"\n",
        u8Streamidx,
        MsOS_GetSystemTime(),
        MApi_VDEC_EX_GetFrameCnt(param),
        MApi_VDEC_EX_GetDispCnt(param),
        MApi_VDEC_EX_GetErrCnt(param),
        MApi_VDEC_EX_GetSkipCnt(param),
        MApi_VDEC_EX_GetDropCnt(param),
        u32pcr / 90,
        u32stc / 90,
        MApi_VDEC_EX_GetPTS(param),
        (MApi_XC_ReadByte((u32MvopBank | 0x5D)) & 0x10)>>4,
        (MApi_XC_ReadByte((u32MvopBank | 0x5D)) & 0x20)>>5,
        (MApi_VDEC_EX_GetPTS(param)- (u32stc / 90)),
        u32VESlevel,
        u32VWptr,
        u32VRptr
        //HAL_HVD_EX_GetData(stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID.u32Id & 0x00ffffff, E_HVD_GDATA_DEC_Q_NUMB),
        //HAL_HVD_EX_GetData(stVDECInfo[E_VDEC_DEVICE_MAIN].stVDECSteamID.u32Id & 0x00ffffff, E_HVD_GDATA_DISP_Q_NUMB)DecQ: %ld, DispQ: %ld
        );
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

                MApi_DMX_Stc_Eng_Get(stVDECInfo[E_VDEC_DEVICE_MAIN].u32SetSTC,&u32stc32, &u32stc);
                MApi_DMX_Pcr_Eng_Get(stVDECInfo[E_VDEC_DEVICE_MAIN].u32SetSTC,&u32pcr32, &u32pcr);

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

                MApi_DMX_Stc_Eng_Get(stVDECInfo[E_VDEC_DEVICE_SUB].u32SetSTC,&u32stc321, &u32stc1);
                MApi_DMX_Pcr_Eng_Get(stVDECInfo[E_VDEC_DEVICE_SUB].u32SetSTC,&u32pcr321, &u32pcr1);

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
        printf("[%s][%d] MsOS_AllocateMemory OK\n",__FUNCTION__,__LINE__);
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
           printf("[%s][%d] s32_VDECDbgTaskID %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,s32_VDECDbgTaskID);
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
    if ((MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2) ||
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
#if(DEMO_VDEC_MULTI_TEST == 1)  //wait apple add vp9 interface to all line
    else if(MApi_VDEC_EX_GetActiveCodecType(pStreamId) == E_VDEC_EX_CODEC_TYPE_VP9)
    {
        eMvopInputSel = MVOP_INPUT_VP9;
        db_print("MVOP_INPUT_VP9 \n");
    }
#endif
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


#if(DEMO_VDEC_MULTI_TEST == 1)
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
        printf("[%s][%d] %d eSTC_Source:%"DTC_MS_U32_d" : Switch\n",__FUNCTION__,__LINE__,eDevice,u32STCSource);
        stDecModCfg.u32Arg = E_VDEC_EX_DEC_PIP_SYNC_SWITCH;
    }
    else
    {
        printf("[%s][%d] %d eSTC_Source:%"DTC_MS_U32_d" : INDIE\n",__FUNCTION__,__LINE__,eDevice,u32STCSource);
        stDecModCfg.u32Arg = E_VDEC_EX_DEC_PIP_SYNC_INDIE;
    }

    if (MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_SET_DECODE_MODE,(MS_U32)&stDecModCfg) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]MApi_VDEC_EX_SetControl return fail!!! :%d\n",__FUNCTION__,__LINE__,MApi_VDEC_EX_SetControl(&stVDECInfo[eDevice].stVDECSteamID,E_VDEC_EX_USER_CMD_SET_DECODE_MODE,(MS_U32)&stDecModCfg));
    }

    return TRUE;

}
#endif

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
MS_U32 Demo_VDEC_UtlLoadStream(MS_U32 u32Addr,MS_U32 u32CodecType,char* pPath)
{
    MS_U8* pu8Fw = NULL;
    MS_U8* pu8BS = NULL;
    MS_U32 u32i = 0;
    MS_U32 u32FileRSize = 0;
    MS_U32 u32FileSize = 0;
    MS_S32 s32MstarNonCachedPoolID = 0;

    FILE* s32FdRead =NULL ;
    printf("=====[%s][%d]===== pPath: %s\n",__FUNCTION__,__LINE__,pPath);

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if (pPath == NULL)
    {
        printf("Please enter the Path\n");
    }
    else
    {
        s32FdRead = fopen(pPath, "rb");
    }

    if (s32FdRead == NULL)
    {
        printf("File open error, please check dynamicavc.bin, dynamic.bin in storage \n");
        printf("File open path : /root/dynamicscale/xxxx.bin \n");
        return FALSE;
    }

    fseek(s32FdRead,0 ,SEEK_END);
    u32FileSize = ftell(s32FdRead);
    printf("[%s][%d]File Size %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,u32FileSize);
    fseek(s32FdRead,0 ,SEEK_SET);

    if (pu8BS == NULL)
    {
        pu8BS = MsOS_AllocateMemory((u32FileSize + 1) *sizeof(MS_U8),s32MstarNonCachedPoolID);//add 1 byte for null terminated
    }

    if (pu8BS == NULL)
    {
         fclose(s32FdRead);
         printf("Memory allocate failed, file size = %"DTC_MS_U32_x" \n", u32FileSize);
         return FALSE;
    }

    printf("[%s][%d] addr:0x%"DTC_MS_U32_x" size:%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,u32Addr, u32FileSize);
    {
        u32FileRSize = fread(pu8BS,1,u32FileSize,s32FdRead);
        if (u32FileRSize == 0)
        {
            fclose(s32FdRead);
            printf("[%s][%d] fread Failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            *(pu8BS+u32FileRSize) = '\0';//for coverity issue String not null terminated (STRING_NULL)
        }
    }

    printf("[%s][%d] Load bitstream from 0x%p to 0x%"DTC_MS_U32_x", size 0x%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,pu8BS, u32Addr, u32FileSize);
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
        memset(pu8Fw, 0, VDEC_BIT_STREAM_LEN- DRV_PROC_SIZE);
        if (u32FileSize > (VDEC_BIT_STREAM_LEN - DRV_PROC_SIZE))
        {
            u32FileSize = (VDEC_BIT_STREAM_LEN - DRV_PROC_SIZE);
        }
        for (u32i = 0; u32i < u32FileSize; u32i++)
        {
            if (u32i<20)
            {
                printf("%.2x ,",pu8Fw[u32i]);
            }
            pu8Fw[u32i] = pu8BS[u32i];
        }
    }

    MsOS_Sync();
    MsOS_FlushMemory();
    MsOS_FreeMemory((void *)pu8BS,s32MstarNonCachedPoolID);
    fclose(s32FdRead);

    printf("\n[%s][%d] Demo_VDEC_UtlLoadStream Done\n",__FUNCTION__,__LINE__);
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

//------------------------------------------------------------------------------
/// @brief return the VDEC stream id
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
void* Demo_VDEC_GetStreamID(EN_VDEC_Device eDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    VDEC_StreamId stStreamId = {0}, stGetStreamId = {0};
    VDEC_EX_User_Cmd eUserCmd = E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;

    if(eDevice == E_VDEC_DEVICE_MAIN)
        eUserCmd =  E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;
    else
        eUserCmd =  E_VDEC_EX_USER_CMD_GET_SUB_STREAM_ID;

    if(MApi_VDEC_EX_GetControl(&stStreamId,eUserCmd,(MS_U32 *)&stGetStreamId)!= E_VDEC_EX_OK )
    {
        printf("Error, video[%d] not init!!\n Demo_VDEC_GetStreamID failed\n", eDevice);
        return (void *)&stNullVDECStreamID;
    }
    stVDECInfo[eDevice].bInited = TRUE;
    stVDECInfo[eDevice].stVDECSteamID = stGetStreamId;

    return (void *)&stVDECInfo[eDevice].stVDECSteamID;
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
EN_VDEC_FrmPackMode Demo_VDEC_GetFramePackMode(EN_VDEC_Device eDevice)
{
    VDEC_EX_Frame_packing_SEI_EX stFramePack;

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    if (stVDECInfo[eDevice].bInited  == FALSE)
    {
        db_print("Not Inited \n");
        return FALSE;
    }
    memset(&stFramePack, 0 , sizeof(VDEC_EX_Frame_packing_SEI_EX));
    stFramePack.stVerCtl.u32size = sizeof(VDEC_EX_Frame_packing_SEI_EX);
    MApi_VDEC_EX_GetControl(&(stVDECInfo[eDevice].stVDECSteamID),E_VDEC_EX_USER_CMD_GET_FPA_SEI_EX,(MS_U32 *)&stFramePack);

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
        db_print("MApi_VDEC_EX_GetDispInfo Failed !! \n");
        return FALSE;
    }
    else
    {
        db_print("MApi_VDEC_EX_GetDispInfo Success !!\n");

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
        db_print("MApi_VDEC_EX_GetDecFrameInfo Failed !! \n");
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
MS_BOOL Demo_VDEC_GetVideoPTS(EN_VDEC_Device eDevice, MS_U32* pu32VPTS, MS_U64* pu64VPTS)
{
    MS_U32 u32VPTS = INVALID_PTS;

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    if((pu32VPTS == NULL) && (pu64VPTS == NULL))
    {
        db_print("pu32VPTS or pu64VPTS is NULL\n");
        return FALSE;
    }

    if(MApi_VDEC_EX_IsFrameRdy(&stVDECInfo[eDevice].stVDECSteamID) != E_VDEC_EX_OK )
    {
        return FALSE;
    }

    u32VPTS = MApi_VDEC_EX_GetPTS(&stVDECInfo[eDevice].stVDECSteamID);

    if(pu32VPTS)
    {
        *pu32VPTS = u32VPTS;
    }

    if(pu64VPTS)
    {
        if(MApi_VDEC_EX_GetControl(&stVDECInfo[eDevice].stVDECSteamID, E_VDEC_EX_USER_CMD_GET_U64PTS, (MS_U32*)pu64VPTS) != E_VDEC_EX_OK)
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
MS_BOOL Demo_VDEC_IsAVSyncDone(EN_VDEC_Device eDevice)
{
    MS_BOOL bSyncFlag = FALSE;
    MS_U32 u32SyncStarTime = 0;
    MS_U32 u32SyncEndTime = 0;
    MS_U32 u32SyncTimeDiff = 0;

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

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

            if(!MApi_VDEC_EX_IsAVSyncOn(&stVDECInfo[eDevice].stVDECSteamID))
            {
                db_print("MApi_VDEC_EX_IsAVSyncOn Not ON\n");
                bSyncFlag = TRUE;
            }

            if(E_VDEC_EX_OK == MApi_VDEC_EX_IsReachSync(&stVDECInfo[eDevice].stVDECSteamID))
            {
                db_print("MApi_VDEC_EX_IsReachSync OK\n");
                db_print("*****************Sync Done %"DTC_MS_U32_d" ms\n", u32SyncTimeDiff);
                bSyncFlag = TRUE;
            }
            else if(E_VDEC_EX_FAIL == MApi_VDEC_EX_IsReachSync(&stVDECInfo[eDevice].stVDECSteamID))
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

    u32SyncEndTime = _Demo_VDEC_GetTime();
    printf("DIFF TIME to wait AV-Sync = %"DTC_MS_U32_u" ms\n", u32SyncEndTime - u32SyncStarTime);

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
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    MS_U32 u32Loop = VDEC_FRAMECOUNT_LOOP;
    MS_U8 u8MvopMiuSel = 0;

    MVOP_InputSel eMvopInputSel = MVOP_INPUT_UNKNOWN;

    VDEC_EX_DispInfo stDispinfo;
    MSAPI_XC_VDEC_DispInfo stMSAPIDispinfo;

    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

    if (stVDECInfo[*peDevice].bInited == FALSE)
    {
        db_print("%d Decoder Not Init \n",*peDevice);
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


    /////////////////////////
    //get video information//
    /////////////////////////

    memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));

    Demo_VDEC_GetDispInfo(peDevice,&stDispinfo);

    eMvopInputSel = _Demo_VDEC_GetMVOPInputSel(*peDevice,&stVDECInfo[*peDevice].stVDECSteamID);

    Demo_XC_VdecDispInfo2msAPIVdecDispInfo(&stDispinfo, &stMSAPIDispinfo);

    msAPI_XC_SetVDECInfo_EX_ByPath(*peDevice, stMSAPIDispinfo);

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

    if(msAPI_XC_SetMVOPMiuSel_EX((E_MSAPI_XC_DECODER_PATH)*peDevice, TRUE, u8MvopMiuSel) != TRUE)
    {
        db_print("msAPI_XC_SetMVOPMiuSel_EX Failed !! \n");
        return FALSE;
    }

    if(msAPI_XC_SetMVOPConfig_EX((E_MSAPI_XC_DECODER_PATH)*peDevice, eMvopInputSel) != TRUE)
    {
        db_print("msAPI_XC_SetMVOPConfig_EX Failed !! \n");
        return FALSE;
    }


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
            db_print("Timeout!! MApi_VDEC_EX_IsFrameRdy is Not Ready!!\n");
            db_print("VDEC Error Code :%"DTC_MS_U32_d"!!\n",MApi_VDEC_EX_GetErrCode(&stVDECInfo[*peDevice].stVDECSteamID));
            return FALSE;
        }
        MsOS_DelayTask(u32VDEC_DelayTime);
    }
    u32EndTime = _Demo_VDEC_GetTime();

    db_print("MApi_VDEC_EX_IsFrameRdy is Ready!!\n");
    db_print("DIFF TIME to check VDEC Frame ready = %"DTC_MS_U32_u" ms\n",u32EndTime - u32StartTime);

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

MS_BOOL Demo_VDEC_Init(EN_VDEC_Device eDevice,VDEC_StreamId* pstVDECStreamId,EN_VDEC_DDI_CodecType eVideoCodec,EN_VDEC_DDI_SrcMode eMode,MS_U32 u32SetSTC)
{
    VDEC_EX_Status stVDECStatus;
    VDEC_EX_Stream eVDECStream;
    VDEC_EX_CodecType eCodecType;
    VDEC_EX_CodecType eCodecTypeCur;
    VDEC_EX_InitParam stInitParam;

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    //Clear VDEC
    MApi_VDEC_EX_Exit(pstVDECStreamId);


#if(DEMO_MVC_TEST == 1)
    if(eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY)
    {
        if (eDevice > 0)
        {
            printf("[%s][%d] Wrong Decoder \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
#endif

    switch (eVideoCodec)
    {
        case E_VDEC_DDI_CODEC_TYPE_H264:            /* fall through */
        case E_VDEC_DDI_CODEC_TYPE_H264_IFRAME:     /* fall through */
        case E_VDEC_DDI_CODEC_TYPE_H264_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_H264;
            printf("Codec: H.264 \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_MPEG4_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_MPEG4;
            printf("Codec: MPEG4 \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_VC1_ADV_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_VC1_ADV;
            printf("Codec: VC1_ADV \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_VC1_MAIN_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_VC1_MAIN;
            printf("Codec: VC1_MAIN \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_AVS:
        case E_VDEC_DDI_CODEC_TYPE_AVS_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_AVS;
            printf("Codec: AVS \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_HEVC:
        case E_VDEC_DDI_CODEC_TYPE_HEVC_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_HEVC;
            printf("Codec: HEVC \n");
            break;
        }
#if(DEMO_VDEC_MULTI_TEST == 1)  //wait apple add vp9 interface to all line
        case E_VDEC_DDI_CODEC_TYPE_VP9:
        case E_VDEC_DDI_CODEC_TYPE_VP9_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_VP9;
            printf("Codec: VP9 \n");
            break;
        }
#endif
        case E_VDEC_DDI_CODEC_TYPE_MVC:
        case E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_MVC;
            printf("Codec: MVC \n");
            break;
        }
        case E_VDEC_DDI_CODEC_TYPE_MPEG2:
        case E_VDEC_DDI_CODEC_TYPE_MPEG2_IFRAME:
        case E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY:
        default:
        {
            eCodecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
            printf("Codec: MPEG2 \n");
            break;
        }
    }

    memset(&stVDECStatus, 0, sizeof(VDEC_EX_Status));
    eCodecTypeCur = MApi_VDEC_EX_GetActiveCodecType(pstVDECStreamId);
    MApi_VDEC_EX_GetStatus(pstVDECStreamId,&stVDECStatus);
    printf("eCodecTypeCur : %d \n",eCodecTypeCur);

    memset(&stInitParam, 0, sizeof(VDEC_EX_InitParam));
    stInitParam.bDisableDropErrFrame = TRUE;
    stInitParam.eCodecType = eCodecType;

    switch (eMode)
    {
        ///DTV mode
        case E_VDEC_DDI_SRC_MODE_DTV:
            stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            printf("Play Mode : DTV \n");
            break;
        ///TS file mode
        case E_VDEC_DDI_SRC_MODE_TS_FILE:
            stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
            printf("Play Mode : TS_FILE \n");
            break;
        ///generic file mode
        case E_VDEC_DDI_SRC_MODE_FILE:
            stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_FILE;
            printf("Play Mode : FILE \n");
            break;
        /// TS file and Demo ES buffer mode
        case E_VDEC_DDI_SRC_MODE_TS_FILE_DUAL_ES:
            stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES;
            printf("Play Mode : TS_FILE_Demo_ES \n");
            break;
        ///generic file and Demo ES buffer mode
        case E_VDEC_DDI_SRC_MODE_FILE_DUAL_ES:
            stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_FILE_DUAL_ES;
            printf("Play Mode : FILE_Demo_ES \n");
            break;
        default :
            stInitParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            printf("Play Mode : DTV \n");
            break;
    }

    stInitParam.SysConfig.u32CodeBufAddr = MEM_ADR_BY_MIU(VDEC_AEON_ADR, VDEC_AEON_MEMORY_TYPE);
    stInitParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    if (eDevice == E_VDEC_DEVICE_MAIN)
    {
        if(eCodecType == E_VDEC_EX_CODEC_TYPE_MVC)
        {
#if(DEMO_MVC_TEST == 1)
            stInitParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_FRAME_BUF_ADR, VDEC_MVC_FRAME_BUF_MEMORY_TYPE);
            stInitParam.SysConfig.u32FrameBufSize = VDEC_MVC_FRAME_BUF_LEN;
            stInitParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
            stInitParam.SysConfig.u32BitstreamBufSize = VDEC_MVC_BIT_STREAM_LEN;
            stInitParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
#endif
        }
        else
        {
            stInitParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
            stInitParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
            stInitParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
            stInitParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
            stInitParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
        }
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (eDevice == E_VDEC_DEVICE_SUB)
    {
        stInitParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
        stInitParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF1_LEN;
        stInitParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        stInitParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM1_LEN;
        stInitParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
    }
#endif

#if(DEMO_CMA_TEST == 1)
    stInitParam.SysConfig.u32FrameBufAddr = 0;
#endif

    stInitParam.SysConfig.eDbgMsgLevel = eVDECDebuglevel;
    printf("VDEC FW add = 0x%"DTC_MS_PHY_x"\n", stInitParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%"DTC_MS_PHY_x"\n", stInitParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%"DTC_MS_PHY_x"\n", stInitParam.SysConfig.u32BitstreamBufAddr);


    if (eDevice == E_VDEC_DEVICE_MAIN)
    {
        eVDECStream = E_VDEC_EX_MAIN_STREAM;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (eDevice == E_VDEC_DEVICE_SUB)
    {
        eVDECStream = E_VDEC_EX_SUB_STREAM;
    }
#endif

    if (MApi_VDEC_EX_GetFreeStream((VDEC_StreamId *)pstVDECStreamId, sizeof(VDEC_StreamId), eVDECStream, (VDEC_EX_CodecType)stInitParam.eCodecType) != E_VDEC_EX_OK)
    {
        printf("[%s][%d]....GetFreeStream Failed Stream ID:  0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__, pstVDECStreamId->u32Id);
        return FALSE;
    }
    else
    {
        printf("[%s][%d] Stream ID:  0x%"DTC_MS_U32_x", Version 0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,pstVDECStreamId->u32Id, pstVDECStreamId->u32Version);
    }

#if(DEMO_VDEC_MULTI_TEST == 1)

    if(eCodecType == E_VDEC_EX_CODEC_TYPE_MPEG2
        || eCodecType == E_VDEC_EX_CODEC_TYPE_H264
        || eCodecType == E_VDEC_EX_CODEC_TYPE_AVS
        || eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC
#if(DEMO_VDEC_MULTI_TEST == 1)  //wait apple add vp9 interface to all line
        || eCodecType == E_VDEC_EX_CODEC_TYPE_VP9
#endif
        )
    {
        if(_Demo_VDEC_STC_Switch(eDevice,pstVDECStreamId,u32SetSTC) == FALSE)
        {
            printf("[%s][%d] eDevice = %d, Demo_VDEC_STC_Switch fail!!!\n",__FUNCTION__,__LINE__, eDevice);
        }
    }

#endif

#if(DEMO_VDEC_ONE_PENDING_BUFFER_MODE_TEST == 1)
    if (Demo_VDEC_PreSetControl(eDevice,pstVDECStreamId) != TRUE)
    {
        printf("[%s][%d] Demo_VDEC_PreSetControl fail!!!\n",__FUNCTION__,__LINE__);
    }
#endif

    //Init/Re-Init VDEC
    if (MApi_VDEC_EX_Init(pstVDECStreamId,&stInitParam) != E_VDEC_EX_OK)
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }

    stVDECInfo[eDevice].bInited = TRUE;
    stVDECInfo[eDevice].eVideoCodec = eVideoCodec;
    stVDECInfo[eDevice].stVDECSteamID = *pstVDECStreamId;
    stVDECInfo[eDevice].u32SetSTC = u32SetSTC;

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

MS_BOOL Demo_VDEC_Play(EN_VDEC_Device eDevice,VDEC_StreamId* pstVDECStreamId,EN_VDEC_AVSYNC_Type eAVSYNC_Mode)
{
    MS_U32 u32StartTime = 0;
    MS_U32 u32EndTime = 0;
    MS_U32 u32AVSyncDelay = 0;

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    u32StartTime = _Demo_VDEC_GetTime();

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

    //If you want to change this value, please inform video and audio team at the same time
    if(MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,TRUE, u32AVSyncDelay, 0) != E_VDEC_EX_OK)
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

    if (eDevice == E_VDEC_DEVICE_MAIN)
    {
        MDrv_MVOP_Enable(DISABLE);
        printf("Display Main MVOP Disable\n");
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (eDevice == E_VDEC_DEVICE_SUB)
    {
        MDrv_MVOP_SubEnable(DISABLE);
        printf("Display Sub MVOP Disable\n");
    }
#endif
    MApi_VDEC_EX_SetEvent(pstVDECStreamId, E_VDEC_EX_EVENT_DISP_INFO_CHG , _Demo_VDEC_Callback, pstVDECStreamId);

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_EX_SetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);
    MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xff);

    //set for FRC
    MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD, TRUE);

    /* Control for Crop information */
    MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP,TRUE);

    if (MApi_VDEC_EX_Play(pstVDECStreamId) != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_Play Main fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    u32EndTime = _Demo_VDEC_GetTime();
    printf("[%s][%d] DIFF TIME of DemoAV_PlayAV = %"DTC_MS_U32_u" ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);

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

MS_BOOL Demo_VDEC_Stop(EN_VDEC_Device eDevice,VDEC_StreamId* pstVDECStreamId)
{

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    if (MApi_VDEC_EX_Exit(pstVDECStreamId) != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_Exit Not OK\n",__FUNCTION__,__LINE__);
    }

    stVDECInfo[eDevice].bInited = FALSE;

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
    MS_U32 u32FrameBufAddr = 0;
    MS_U32 u32StreamBufAddr = 0;
    MS_U32 u32StreamBufEnd = 0;
    MS_U32 u32ActualBitstreamSize = 0;
    VDEC_EX_FrameInfo stFrameInfo;

    printf("=====[%s][%d]===== pLogopath: %s\n",__FUNCTION__,__LINE__,pLogopath);

    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)
    memset(&stFrameInfo,0,sizeof(stFrameInfo));

    stVDECInfo[*peDevice].eVideoCodec = *pCodec;

    if (Demo_VDEC_Init(*peDevice,&(stVDECInfo[*peDevice].stVDECSteamID),stVDECInfo[*peDevice].eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE,NULL) == FALSE) // default mepg2
    {
        printf("[%s][%d] Demo_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
         printf("[%s][%d] Stream ID After Init  0x%"DTC_MS_U32_x"\n",__FUNCTION__, __LINE__,stVDECInfo[*peDevice].stVDECSteamID.u32Id);
    }

    if (*peDevice == E_VDEC_DEVICE_MAIN)
    {
        u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
        u32StreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE) ;
        u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32StreamBufAddr,stVDECInfo[*peDevice].eVideoCodec,pLogopath);
        u32StreamBufEnd = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE) + u32ActualBitstreamSize;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_VDEC_DEVICE_SUB)
    {
        u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_MEMORY_TYPE);
        u32StreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
        u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32StreamBufAddr,stVDECInfo[*peDevice].eVideoCodec,pLogopath);
        u32StreamBufEnd = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE) + u32ActualBitstreamSize;
    }
#endif

#if(DEMO_CMA_TEST == 1)
    u32FrameBufAddr = 0;
#endif

    if (MApi_VDEC_EX_MHEG_DecodeIFrame(&stVDECInfo[*peDevice].stVDECSteamID,u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEnd) != E_VDEC_EX_OK)
    {
        db_print("MApi_VDEC_EX_MHEG_DecodeIFrame failed!\n");
        return FALSE;
    }
    else
    {
        db_print("*******************************************************\n");
        db_print("MApi_VDEC_MHEG_DecodeIFrame good\n");
        db_print("*******************************************************\n");
    }

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
/// @param[in]  eShowFrame_Mode : EN_VDEC_ShowFrame_Type : 0:E_VDEC_MVOP_MODE, 1:E_VDEC_SWDetile_MODE, 2:E_VDEC_DIP_MODE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b none  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_FilePlay(EN_VDEC_Device eDevice,VDEC_StreamId* pstVDECStreamId,MS_U32 u32Address,MS_U32 u32ActualBitstreamSize,MS_U32 u32PushTime,MS_U32 u32Address2,MS_U32 u32ActualBitstreamSize2,EN_VDEC_ShowFrame_Type eShowFrame_Mode)
{
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

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    if(stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY)
    {
        if (eDevice > 0)
        {
            printf("[%s][%d] Wrong Decoder \n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        u32PlaySizeLim = PLAY_SIZE_LIMITITATION;
    }
    else if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
    {
        u32PlaySizeLim = VDEC_BIT_STREAM_LEN;
    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    stVDECInfo[eDevice].u32PlaySize = u32ActualBitstreamSize;
    stVDECInfo[eDevice].u32Address = u32Address;
    stVDECInfo[eDevice].u32PlaySize2 = u32ActualBitstreamSize2;
    stVDECInfo[eDevice].u32Address2 = u32Address2;
    stVDECInfo[eDevice].eShowFrame_Mode = eShowFrame_Mode;

    if (MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,TRUE, 0, 0) != E_VDEC_EX_OK)
    {
        db_print("set MApi_VDEC_EX_AVSyncOn fail\n");
        return FALSE;
    }

    if(stVDECInfo[eDevice].eShowFrame_Mode != E_VDEC_MVOP_MODE)
    {
        if(MApi_VDEC_EX_SetBlockDisplay(pstVDECStreamId, TRUE)!= E_VDEC_EX_OK)
        {
            db_print("set MApi_VDEC_EX_SetBlockDisplay fail\n");
            return FALSE;
        }
        else
            db_print("MApi_VDEC_EX_SetBlockDisplay OK!\n");

        if(MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE, TRUE) != E_VDEC_EX_OK)
        {
            db_print("set control fail\n");
            return FALSE;
        }
        else
            db_print("MApi_VDEC_EX_SetControl OK!\n");

        if (MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,FALSE, 0, 0) != E_VDEC_EX_OK)
        {
            db_print("set MApi_VDEC_EX_AVSyncOn fail\n");
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
    printf("[%s][%d]Play!\n",__FUNCTION__,__LINE__);

    if ((u32Address2 != 0) && (u32ActualBitstreamSize2 != 0))
    {
        bEnableSecondStream = TRUE;
        printf("[%s][%d]play mvc with 2 stream !\n",__FUNCTION__,__LINE__);
     }

    // push BBU
    memset(&stDecCmd, 0, sizeof(VDEC_EX_DecCmd));
    memset(&stDecCmd2, 0, sizeof(VDEC_EX_DecCmd));

    if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
        || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
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
        printf("=====[%s][%d]=====MApi_VDEC_EX_PushDecQ OK\n",__FUNCTION__,__LINE__);
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

    printf("bRdy2SetVOP becomes TRUE when u32i = %"DTC_MS_U32_d"\n", u32i);
    printf("before SetMVOP:: picCnt = %"DTC_MS_U32_d"\n", MApi_VDEC_EX_GetFrameCnt(pstVDECStreamId));

    if (u32PushTime == 0)
    {
        /* Create task for video play */
        if (PlayLoop_Task[eDevice].iId == INVALID_TASK_ID)
        {
            PlayLoop_Task[eDevice].pStack = MsOS_AllocateMemory(PlayLoop_Task[eDevice].u32StackSize,s32MstarNonCachedPoolID);
            printf("[%s][%d] MsOS_AllocateMemory OK \n",__FUNCTION__,__LINE__);
        }

        if (PlayLoop_Task[eDevice].pStack == NULL )
        {
            printf("[%s][%d] allocate stack fail\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        PlayLoop_Task[eDevice].iId = MsOS_CreateTask((TaskEntry)_Demo_VDEC_PlayLoopTask,
                                             (MS_U32) eDevice,
                                             PlayLoop_Task[eDevice].ePriority, // E_TASK_PRI_MEDIUM,
                                             TRUE,
                                             PlayLoop_Task[eDevice].pStack,
                                             PlayLoop_Task[eDevice].u32StackSize,
                                             PlayLoop_Task[eDevice].szName);
        if (PlayLoop_Task[eDevice].iId == INVALID_TASK_ID)
        {
            printf("[%s][%d] Create Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,PlayLoop_Task[eDevice].iId);
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
            if (stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_H264_MEMORY
                || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY
                || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY
                || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY
                || stVDECInfo[eDevice].eVideoCodec == E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY)
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
MS_BOOL Demo_VDEC_FileStop(EN_VDEC_Device eDevice)
{
    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    Demo_VDEC_Stop(eDevice,&(stVDECInfo[eDevice].stVDECSteamID));

    if (PlayLoop_Task[eDevice].iId !=  INVALID_EVENT_ID)
    {
        MS_S32 s32MstarNonCachedPoolID = 0;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        MsOS_FreeMemory(PlayLoop_Task[eDevice].pStack,s32MstarNonCachedPoolID);
        PlayLoop_Task[eDevice].iId =  INVALID_EVENT_ID;
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
    printf("Set TimeshiftSeamless Control(0x%"DTC_MS_U32_x", %"DTC_MS_U32_u")\n", pstVDECStreamId->u32Id, u32Param);
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

        VDEC_StreamId *stVDECSteamID = NULL;
        EN_VDEC_Device eDevice = *u32IntervalTime;
        stVDECSteamID = Demo_VDEC_GetStreamID(eDevice);

        if (stVDECSteamID == NULL)
            return FALSE;

        if(u8VDECDbgMsg == 0x4)
        {
            printf("[%s][%d]VDEC %d Call Back function\n",__FUNCTION__,__LINE__,eDevice);

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

    DEMO_VDEC_CHECK_DEVICE_ID(*peDevice)

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

#if(DEMO_VDEC_ONE_PENDING_BUFFER_MODE_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Pre Set Control  : set the ONE PENDING BUFFER MODE \n
/// @param[in] Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Sample Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VDEC_PreSetControl(EN_VDEC_Device eDevice,VDEC_StreamId* pstVDECStreamId)
{

    DEMO_VDEC_CHECK_DEVICE_ID(eDevice)

    MApi_VDEC_EX_PreSetControl(pstVDECStreamId,E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE, TRUE);
    return TRUE;
}
#endif

#endif
