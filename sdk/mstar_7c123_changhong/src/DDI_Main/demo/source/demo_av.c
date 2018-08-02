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
#if (DEMO_AV_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>
#include "MsMemory.h"

#include "apiDMX.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiAUDIO.h"
#include "apiXC.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#endif
#include "drvMVOP.h"
#include "drvDTC.h"

#include "demo_audio_common.h"
#include "demo_dmx.h"
#include "demo_xc.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_zapping.h"
#include "demo_utility.h"
#include "demo_main.h"
#include "demo_osd.h"

#if(DEMO_AUDIO_TEST == 1)
#include "demo_audio.h"
#endif

#if(DEMO_AUDIO_MULTI_TEST == 1)
#include "demo_audio_multi.h"
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/// AV check device ID
#if (DEMO_VDEC_MULTI_TEST == 1)
#define DEMO_AV_CHECK_DEVICE_ID(u8Id)                            \
    if ((EN_AV_Device)u8Id >= E_AV_DEVICE_MAX)        \
    {                                                             \
        printf("[%s][%d] Wrong Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#else
#define DEMO_AV_CHECK_DEVICE_ID(u8Id)                            \
    if ((EN_AV_Device)u8Id > 0)        \
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
#define AV_DBG          1
#define WAITTIMEOUT     5000
#define INVALID_TASK_ID     -1
#define INVALID_PTS         -1

#if AV_DBG
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
static ST_AV_INFO stAVInfo[E_AV_DEVICE_MAX] = {{FALSE,{INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {E_DMX_FLOW_INVALID,E_DMX_FLOW_INPUT_INVALID,0,0,0},E_AV_SYNC_PCR_MODE,E_AV_MVOP_MODE,FALSE,E_AV_AVSYNC_STATE_RESET},
                                                {FALSE,{INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,E_DMX_FLT_TYPE_INVALID,{INVALID_FILEIN_TYPE,INVALID_FILEIN_ID},{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,AUDIO_CODEC_MPEG},
                                                {E_DMX_FLOW_INVALID,E_DMX_FLOW_INPUT_INVALID,0,0,0},E_AV_SYNC_PCR_MODE,E_AV_MVOP_MODE,FALSE,E_AV_AVSYNC_STATE_RESET}};

Task_Info AV_SYNC_Task = {-1, E_TASK_PRI_HIGH, NULL, 0x4000, "_AVSYNC_TASK"};

//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------

static EN_AUDIO_CODEC_TYPE _Demo_AV_SI_Audio_CodecType_Mapping(EN_SI_AudioFormat eAudioFmt)
{
    switch ( eAudioFmt )
    {
        case E_SI_AUD_MPEG :
        case E_SI_AUD_MPEG_AD:
            return AUDIO_CODEC_MPEG;

        case E_SI_AUD_AC3 :
        case E_SI_AUD_AC3_AD :
            return AUDIO_CODEC_AC3;

        case E_SI_AUD_AC3P :
            return AUDIO_CODEC_AC3P;

        case E_SI_AUD_MPEG4:
        case E_SI_AUD_AAC:
            return AUDIO_CODEC_AAC;

        default:
            //DemoDmx_Print("Unsopprot Audio Format %ld!\n",eAudioFmt);
            return AUDIO_CODEC_NONE ;
    }

}


#if(DEMO_AUDIO_MULTI_TEST == 1)
//------------------------------------------------------------------------------
/// @brief transform AV Audio AFIFO to TSP AFIFO
/// @param[in] AUDIO_AFIFO_SOURC Audio AFIFO Source
/// @return the TSP AFIFO
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static EN_DEMO_DMX_FLT_TYPE _Demo_AV_Audio_AFIFO_Map_TSP_AFIFO(AUDIO_AFIFO_SOURCE stAfifoSource)
{
    switch(stAfifoSource)
    {
        case E_AFIFO_0:
            return E_DMX_FLT_TYPE_AUD0;
        case E_AFIFO_1:
            return E_DMX_FLT_TYPE_AUD1;
        case E_AFIFO_2:
            return E_DMX_FLT_TYPE_AUD2;
        default:
            return E_DMX_FLT_TYPE_AUD0;
    }
}

//------------------------------------------------------------------------------
/// @brief get Audio info from Audio.c and add to struct
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetAudioInfo(EN_AV_Device eDevice)
{
    ST_AUDIO_DEC_INFO stAudioInfo;

    if (eDevice >= (MS_U32)E_AV_DEVICE_MAX)
    {
        db_print("Wrong Decoder \n");
        return FALSE;
    }

    Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);

    return _Demo_AV_Audio_AFIFO_Map_TSP_AFIFO(stAudioInfo.stAudioDecStatus_t.eAfifoSource);
}

#endif

//------------------------------------------------------------------------------
/// @brief open timeout machenism
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_AV_TimeoutSet(EN_AV_Device eDevice)
{
    VDEC_EX_Decode_Timeout_Param stParam;
    memset(&stParam,0,sizeof(VDEC_EX_Decode_Timeout_Param));
    stParam.bEnable = TRUE;
    stParam.u32Timeout = 80;
    MApi_VDEC_EX_SetControl(&(stAVInfo[eDevice].stVideoParams.stVDECSteamID),E_VDEC_EX_USER_CMD_ENABLE_DECODE_ENGINE_TIMEOUT,(MS_U32)(&stParam));

}

//------------------------------------------------------------------------------
/// @brief SYNC state machine Task
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_AV_SYNCTASK(MS_U32 u32Device)
{
    EN_AV_Device eDevice = 0;

    MS_U64 u64Stc = INVALID_PTS;

    MS_U64 u64Pts = INVALID_PTS;
    MS_U64 u64Vfpts = INVALID_PTS;
    MS_U64 u64Afpts = INVALID_PTS;

    //time difference
    MS_S64 s64Td = 0;

    MS_U8 u8Count[E_AV_DEVICE_MAX] = {0};

    MS_U8 u8Volume = 0;


#if(DEMO_AUDIO_MULTI_TEST == 1)
    ST_AUDIO_DEC_INFO stAudioInfo = {0};
    MS_U32 u32AudioSTCSYNCstatus = 0;
#endif

    while(1)
    {
        for (eDevice = 0; eDevice < E_AV_DEVICE_MAX; eDevice++)
        {
            if(stAVInfo[eDevice].bEnableAVSYNC_Task != FALSE)
            {
                switch(stAVInfo[eDevice].eAVSYNC_Task_State)
                {

                    case E_AV_AVSYNC_STATE_RESET:

                            u64Stc = INVALID_PTS;
                            u64Pts = INVALID_PTS;
                            u64Vfpts = INVALID_PTS;
                            u64Afpts = INVALID_PTS;

                            s64Td = 0;
                            u8Count[eDevice] = 0;

                            u8Volume = 0;

                            stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_IDLE;

                            break;

                    case E_AV_AVSYNC_STATE_IDLE:

                            if(stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
                            {
                                if(stAVInfo[eDevice].bInited == TRUE)
                                {
                                    stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_INIT;
                                }
                            }
                            else
                            {
                                stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_INIT;
                            }
                            break;

                    case E_AV_AVSYNC_STATE_INIT:

                            u64Stc = Demo_AV_InitSTC(eDevice);

                            if(u64Stc == INVALID_PTS)
                            {
                                //db_print("E_AV_AVSYNC_STATE_INIT : u64Stc = %"DTC_MS_U64_d"\n", u64Stc);
                                break;
                            }

                            if(stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
                            {
                                stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_WAIT;
                            }
                            else
                            {
                                stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_MONITOR;
                            }

                            break;

                    case E_AV_AVSYNC_STATE_WAIT:

                            //mute audio
                            Demo_Audio_SetAbsoluteVolume(&u8Volume);

#if(DEMO_AUDIO_MULTI_TEST == 1)

                            Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);

                            // ask Audio DSP to sync STC
                            MApi_AUDIO_SetAudioParam2(stAudioInfo.eDecID, Audio_ParamType_syncSTC, TRUE);

                            // enable sync STC flag
                            MApi_AUDIO_GetAudioInfo2(stAudioInfo.eDecID, Audio_infoType_syncSTC_in_mmTs, &u32AudioSTCSYNCstatus);

                            if(u32AudioSTCSYNCstatus == 0)
                            {
                                //db_print("\033[35m  Audio_infoType_syncSTC_in_mmTs break\033[0m\n");
                                break;
                            }

                            // disable sync STC flag
                            MApi_AUDIO_SetAudioParam2(stAudioInfo.eDecID, Audio_ParamType_syncSTC, FALSE);

#else
                            // ask Audio DSP to sync STC
                            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, 1, 0);

                            if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs) == 0)
                            {
                                //db_print("\033[35m  Audio_Comm_infoType_syncSTC_in_mmTs break\033[0m\n");
                                break;
                            }

                            // disable sync STC flag
                            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, 0, 0);

#endif

                            //unmute audio
                            u8Volume = DEFAULT_VOLUME;
                            Demo_Audio_SetAbsoluteVolume(&u8Volume);

                            stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_MONITOR;

                            break;
                    case E_AV_AVSYNC_STATE_MONITOR:

                            u64Stc = Demo_AV_GetSTC(eDevice);

                            switch(stAVInfo[eDevice].eAVSYNC_Mode)
                            {
                                case E_AV_SYNC_VIDEO_MASTER_MODE:
                                    Demo_VDEC_GetVideoPTS(eDevice,NULL,&u64Vfpts);
                                    u64Pts = u64Vfpts;
                                    break;
                                case E_AV_SYNC_AUDIO_MASTER_MODE:
                                    {
#if(DEMO_AUDIO_MULTI_TEST == 1)
                                        MS_U8 u8DecStatus = 0;
                                        Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);
                                        MApi_AUDIO_GetAudioInfo2(stAudioInfo.eDecID, Audio_infoType_DecStatus, &u8DecStatus);
                                        if (u8DecStatus)
                                        {
                                            MApi_AUDIO_GetAudioInfo2(stAudioInfo.eDecID, Audio_infoType_33bit_PTS, &u64Afpts);
                                            u64Pts = u64Afpts / 90;
                                        }
#else
                                        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_currAudDecStatus))
                                        {
                                            u64Afpts = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS) / 90;
                                            u64Pts = u64Afpts;
                                        }
#endif
                                    }
                                    break;
                                default:
                                    break;
                            }

                            if(u64Pts == INVALID_PTS || u64Stc == INVALID_PTS)
                            {
                                db_print("E_AV_AVSYNC_STATE_MONITOR : u64Pts = %"DTC_MS_U64_d"\n", u64Pts);
                                db_print("E_AV_AVSYNC_STATE_MONITOR : u64Stc = %"DTC_MS_U64_d"\n", u64Stc);
                                break;
                            }

                            s64Td = (MS_S64)(u64Stc - u64Pts);

                            if((s64Td > 33) || (s64Td < -33))
                            {
                                u8Count[eDevice]++;
                            }
                            else
                            {
                                u8Count[eDevice] = 0;
                            }

                            //if 2 continous Td are bigger or smaller than 33ms then STC update by VPTS (follow MM algorithm)
                            if(u8Count[eDevice] == 2)
                            {
                                //update PTS -> STC
                                Demo_AV_SetSTC(eDevice,u64Pts);
                                u8Count[eDevice] = 0;
                            }

                            break;
                    default:
                            break;

                }
            }
        }

        MsOS_DelayTask(1);
    }

}

//------------------------------------------------------------------------------
/// @brief create SYNC state machine Task
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_AV_SYNCCreateTask(EN_AV_Device eDevice)
{
    MS_S32 s32MstarCachedPoolID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    if(AV_SYNC_Task.iId == INVALID_TASK_ID)
    {
        AV_SYNC_Task.pStack = MsOS_AllocateMemory(AV_SYNC_Task.u32StackSize, s32MstarCachedPoolID);
        if(!AV_SYNC_Task.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }
        AV_SYNC_Task.iId = MsOS_CreateTask((TaskEntry)_Demo_AV_SYNCTASK,
                                            eDevice,
                                            AV_SYNC_Task.ePriority,
                                            TRUE,
                                            AV_SYNC_Task.pStack,
                                            AV_SYNC_Task.u32StackSize,
                                            AV_SYNC_Task.szName);

        if(AV_SYNC_Task.iId == INVALID_TASK_ID )
        {
            printf("[%s][%d] Create AV SYNC Task Failed INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] s32_AVSYNCTaskID %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,AV_SYNC_Task.iId);
            return TRUE;
        }
    }

    return TRUE;
}

MS_BOOL Demo_AV_SetLanguage(EN_AV_Device* peDevice, MS_U32 *u32LangIndex)
{
    MS_U32 i=0;
    MS_U32 u32PgNum=0;
    st_PG *pstPG = NULL;
    st_PG *pstcurrPG = NULL;
    MS_U32 u32VideoPid =stAVInfo[*peDevice].stVideoParams.u16PID;
    MS_U32 u32AudioPid =stAVInfo[*peDevice].stAudioParams.u16PID;
    MS_U32 u32PCRPid = stAVInfo[*peDevice].stPCRParams.u16PID;
    EN_DEMO_DMX_FLOW eDmxFlow = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if(stAVInfo[*peDevice].bInited == FALSE)
    {
        printf("[%s][%d] DTV %d not init\n", __FUNCTION__, __LINE__, *peDevice);
        return FALSE;
    }

    if(*u32LangIndex >= MAX_AUDIO_PG)
    {
        printf("[%s][%d] invalid language index = %ld\n", __FUNCTION__, __LINE__, *u32LangIndex);
        return FALSE;
    }

    if(Demo_DMX_Scan(eDmxFlow, TRUE)==TRUE)
    {
        Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);
        for (i= 0; i< u32PgNum; i++)
        {
            if( ( (u32PCRPid == INVALID_PID_ID)||(u32PCRPid == (pstPG+i)->u32PidPcr) ) &&
                ( (u32VideoPid == INVALID_PID_ID)||(u32VideoPid == (pstPG+i)->u32PidVideo) ) &&
                ( (u32AudioPid == INVALID_PID_ID)||Demo_DMX_CheckAudioPgExist(pstPG+i, u32AudioPid)) )
            {
                pstcurrPG = pstPG+i;

                //get pid from u32LangIndex
                if(pstcurrPG->Audio_PG[*u32LangIndex].bISO_Lang_exist)
                {
                    stAVInfo[*peDevice].stAudioParams.u16PID = pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio;
                    stAVInfo[*peDevice].stAudioParams.eAudioCodec = _Demo_AV_SI_Audio_CodecType_Mapping(pstcurrPG->Audio_PG[*u32LangIndex].eAudioFmt);
                    printf("[%s][%d] set PID = 0x%04lx, Lang = %s\n", __FUNCTION__, __LINE__, pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio, pstcurrPG->Audio_PG[*u32LangIndex].u8Lang);
                }
                else if(pstcurrPG->Audio_PG[*u32LangIndex].bSupplematary_Audio_exist)
                {
                    stAVInfo[*peDevice].stAudioParams.u16PID = pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio;
                    stAVInfo[*peDevice].stAudioParams.eAudioCodec = _Demo_AV_SI_Audio_CodecType_Mapping(pstcurrPG->Audio_PG[*u32LangIndex].eAudioFmt);
                    printf("[%s][%d] set PID = 0x%04lx, Lang = %s\n", __FUNCTION__, __LINE__, pstcurrPG->Audio_PG[*u32LangIndex].u32PidAudio, pstcurrPG->Audio_PG[*u32LangIndex].u8Supplementary_Lang);
                }
                else
                {
                    printf("[%s][%d] invalid u32LangIndex = %ld\n", __FUNCTION__, __LINE__, *u32LangIndex);
                    return FALSE;
                }
            }
        }
    }

    MS_U32 u32AudioPID = stAVInfo[*peDevice].stAudioParams.u16PID;

    Demo_AV_ChangeAudioTrack(peDevice, &u32AudioPID, &stAVInfo[*peDevice].stAudioParams.eAudioCodec);

    return TRUE;
}

MS_BOOL Demo_AV_LanguageList(EN_AV_Device* peDevice)
{
    MS_U32 i=0, j=0;
    MS_U32 u32PgNum=0;
    st_PG *pstPG = NULL;
    st_PG *pstcurrPG = NULL;
    MS_U32 u32VideoPid =stAVInfo[*peDevice].stVideoParams.u16PID;
    MS_U32 u32AudioPid =stAVInfo[*peDevice].stAudioParams.u16PID;
    MS_U32 u32PCRPid = stAVInfo[*peDevice].stPCRParams.u16PID;
    EN_DEMO_DMX_FLOW eDmxFlow = stAVInfo[*peDevice].stDMXflowParams.eFlow;

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if(stAVInfo[*peDevice].bInited == FALSE)
    {
        printf("[%s][%d] DTV %d not init\n", __FUNCTION__, __LINE__, *peDevice);
        return FALSE;
    }

    if(Demo_DMX_Scan(eDmxFlow, TRUE)==TRUE)
    {
        Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);

        for (i= 0; i< u32PgNum; i++)
        {
            if( ( (u32PCRPid == INVALID_PID_ID)||(u32PCRPid == (pstPG+i)->u32PidPcr) ) &&
                ( (u32VideoPid == INVALID_PID_ID)||(u32VideoPid == (pstPG+i)->u32PidVideo) ) &&
                ( (u32AudioPid == INVALID_PID_ID)||Demo_DMX_CheckAudioPgExist(pstPG+i, u32AudioPid)) )
            {
                printf("[%s][%d] start =============================================================\n", __FUNCTION__, __LINE__);
                pstcurrPG = pstPG+i;
                for (j=0;j<MAX_AUDIO_PG;j++)
                {
                    if ((pstcurrPG->Audio_PG[j].bISO_Lang_exist) || (pstcurrPG->Audio_PG[j].bSupplematary_Audio_exist))
                    {
                        printf("[%s][%d]    index = %ld, pid = 0x%04lx", __FUNCTION__, __LINE__, j, pstcurrPG->Audio_PG[j].u32PidAudio);

                        //print language
                        if(strcmp((char*)pstcurrPG->Audio_PG[j].u8Lang, "") != 0)
                        {
                            printf(", Lang = %s", pstcurrPG->Audio_PG[j].u8Lang);
                        }
                        else if(strcmp((char*)pstcurrPG->Audio_PG[j].u8Supplementary_Lang, "") != 0)
                        {
                            printf(", Lang = %s", pstcurrPG->Audio_PG[j].u8Supplementary_Lang);
                        }
                        else
                        {
                            printf(", Lang = N/A");
                        }

                        //print AD
                        if ((pstcurrPG->Audio_PG[j].u8Lang_audio_type == 0x03) || (pstcurrPG->Audio_PG[j].u8Supplementary_classification == 0x01))
                        {
                            printf(", AD = y");
                        }
                        else
                        {
                            printf(", AD = n");
                        }

                        //print if current language
                        if(u32AudioPid == pstcurrPG->Audio_PG[j].u32PidAudio)
                        {
                            printf(" (current language)\n");
                        }
                        else
                        {
                            printf(" \n");
                        }

                    }
                }
                printf("[%s][%d] end ===============================================================\n", __FUNCTION__, __LINE__);
            }
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SYNC mode to Init STC
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U64 Demo_AV_InitSTC(EN_AV_Device eDevice)
{
    MS_U64 u64InitStc = INVALID_PTS;

    switch(stAVInfo[eDevice].eAVSYNC_Mode)
    {
        case E_AV_SYNC_VIDEO_MASTER_MODE:
                Demo_VDEC_GetVideoPTS(eDevice,NULL,&u64InitStc);
            break;
        case E_AV_SYNC_AUDIO_MASTER_MODE:
                {
#if(DEMO_AUDIO_MULTI_TEST == 1)
                    ST_AUDIO_DEC_INFO stAudioInfo = {0};
                    MS_U8 u8DecStatus = 0;
                    Demo_Audio_GetDecInfo(eDevice, &stAudioInfo);
                    MApi_AUDIO_GetAudioInfo2(stAudioInfo.eDecID, Audio_infoType_DecStatus, &u8DecStatus);
                    if (u8DecStatus)
                    {
                        MApi_AUDIO_GetAudioInfo2(stAudioInfo.eDecID, Audio_infoType_33bit_PTS, &u64InitStc);
                        u64InitStc = u64InitStc / 90;
                    }
#else
                    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_currAudDecStatus))
                    {
                        u64InitStc = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS) / 90;
                    }
#endif
                }
                break;
        default:
            break;
    }

    if(u64InitStc == INVALID_PTS || u64InitStc == 0)
    {
        //db_print("return u64InitStc = %"DTC_MS_U64_d"\n", u64InitStc);
        return INVALID_PTS;
    }

    db_print("STC : u64InitStc = %"DTC_MS_U64_d"\n", u64InitStc);

    if(Demo_AV_SetSTC(eDevice,u64InitStc) == FALSE)
    {
        db_print("Demo_AV_SetSTC fail \n");
        return INVALID_PTS;
    }

    return u64InitStc;
}

//------------------------------------------------------------------------------
/// @brief SYNC mode to Set STC
/// @param[in] Main or Sub Device or Other Device
/// @param[in] u64Stc (unit:ms)
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetSTC(EN_AV_Device eDevice,MS_U64 u64Stc)
{
    MS_U32 u32StcH =0;
    MS_U32 u32Stc = 0;

    u64Stc = u64Stc*90;

    u64Stc &= 0x1FFFFFFFFLL;

    u32StcH = (MS_U32)((u64Stc >> 32) & 0xFFFFFFFF);
    u32Stc = (MS_U32)(u64Stc & 0xFFFFFFFF);

    if(MApi_DMX_Stc_Eng_Set(stAVInfo[eDevice].stPCRParams.u8PCREngID, u32StcH, u32Stc)!= DMX_FILTER_STATUS_OK)
    {
        printf("[%s][%d] MApi_DMX_Stc_Set Fail\n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    db_print("eDevice : %d : PTS -> STC, u64Stc=%"DTC_MS_U64_d"\n",eDevice, u64Stc/90);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SYNC mode to Get STC
/// @param[in] Main or Sub Device or Other Device
/// @return u64Stc (unit:ms)
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U64 Demo_AV_GetSTC(EN_AV_Device eDevice)
{
    MS_U32 u32StcH = 0;
    MS_U32 u32Stc = 0;
    MS_U64 u64Stc = 0;

    if(MApi_DMX_Stc_Eng_Get(stAVInfo[eDevice].stPCRParams.u8PCREngID, &u32StcH, &u32Stc) != DMX_FILTER_STATUS_OK)
    {
        printf("[%s][%d] MApi_DMX_Stc_Get Fail\n",__FUNCTION__,__LINE__);
        return INVALID_PTS;
    }

    u64Stc = ((MS_U64)u32StcH << 32) | u32Stc;
    u64Stc &= 0x1FFFFFFFFLL;

    u64Stc = u64Stc/90;

    return u64Stc;
}

//------------------------------------------------------------------------------
/// @brief open the SYNC mode
/// @param[in] Main or Sub Device or Other Device
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetSYNC(EN_AV_Device eDevice)
{
    stAVInfo[eDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;
    stAVInfo[eDevice].bEnableAVSYNC_Task = TRUE;

    if(AV_SYNC_Task.iId == INVALID_TASK_ID)
    {
        if(!_Demo_AV_SYNCCreateTask(eDevice))
        {
            printf("[%s][%d]create SYNC task fail!!!!!!!!!!! OH NO \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief if other program wants AV info that can use this to get info
/// @param[in] Main or Sub Device or Other Device
/// @param[out] gtAVInfo
///     pointer to output buffer
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_GetAVInfo(EN_AV_Device eDevice,ST_AV_INFO* pstAVInfo)
{

    DEMO_AV_CHECK_DEVICE_ID(eDevice)

    pstAVInfo->bInited = stAVInfo[eDevice].bInited;


    pstAVInfo->stVideoParams.u16PID = stAVInfo[eDevice].stVideoParams.u16PID;
    pstAVInfo->stVideoParams.stFilterType = stAVInfo[eDevice].stVideoParams.stFilterType;
    pstAVInfo->stVideoParams.eVideoCodec = stAVInfo[eDevice].stVideoParams.eVideoCodec;
    pstAVInfo->stVideoParams.u8Filter = stAVInfo[eDevice].stVideoParams.u8Filter;
    pstAVInfo->stVideoParams.eFIFO_ID = stAVInfo[eDevice].stVideoParams.eFIFO_ID;
    pstAVInfo->stVideoParams.stVDECSteamID = stAVInfo[eDevice].stVideoParams.stVDECSteamID;


    pstAVInfo->stAudioParams.u16PID = stAVInfo[eDevice].stAudioParams.u16PID;
    pstAVInfo->stAudioParams.stFilterType = stAVInfo[eDevice].stAudioParams.stFilterType;
    pstAVInfo->stAudioParams.eAudioCodec= stAVInfo[eDevice].stAudioParams.eAudioCodec;
    pstAVInfo->stAudioParams.u8Filter = stAVInfo[eDevice].stAudioParams.u8Filter;
    pstAVInfo->stAudioParams.eFIFO_ID = stAVInfo[eDevice].stAudioParams.eFIFO_ID;


    pstAVInfo->stAudioADParams.u16PID = stAVInfo[eDevice].stAudioADParams.u16PID;
    pstAVInfo->stAudioADParams.u8Filter = stAVInfo[eDevice].stAudioADParams.u8Filter;


    pstAVInfo->stPCRParams.u16PID = stAVInfo[eDevice].stPCRParams.u16PID;
    pstAVInfo->stPCRParams.u8PCREngID = stAVInfo[eDevice].stPCRParams.u8PCREngID;
    pstAVInfo->stPCRParams.u8Filter = stAVInfo[eDevice].stPCRParams.u8Filter;


    pstAVInfo->stDMXflowParams.eFlow = stAVInfo[eDevice].stDMXflowParams.eFlow;
    pstAVInfo->stDMXflowParams.eDmxInput = stAVInfo[eDevice].stDMXflowParams.eDmxInput;
    pstAVInfo->stDMXflowParams.DmxClkInv = stAVInfo[eDevice].stDMXflowParams.DmxClkInv;
    pstAVInfo->stDMXflowParams.DmxExtSync = stAVInfo[eDevice].stDMXflowParams.DmxExtSync;
    pstAVInfo->stDMXflowParams.DmxParallal = stAVInfo[eDevice].stDMXflowParams.DmxParallal;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief if other program wants AV info that can use this to set AV info
/// @param[in] Main or Sub Device or Other Device
/// @param[in] AVInfo enum
/// @param[in] Value of AVInfo element
/// @return TRUE: Process success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetAVInfo(EN_AV_Device eDevice,EN_AV_INFO eAVInfo,void* pAVInfo)
{

    DEMO_AV_CHECK_DEVICE_ID(eDevice)

    switch(eAVInfo)
    {
        case E_AV_INFO_DMX_FLOW:
            stAVInfo[eDevice].stDMXflowParams.eFlow = *((EN_DEMO_DMX_FLOW *)pAVInfo);
            return TRUE;
        default:
            printf("[%s][%d] wrong eAVInfo:%d!\n", __FUNCTION__, __LINE__,eAVInfo);
            return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @brief The sample code to set the av-sync mode
/// @param[in] u8mode 0x0: av-sync mode
///                             0x1: audio master mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_SetAVSyncMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetAVSYNCMode(EN_AV_Device* peDevice, EN_AV_AVSYNC_Type* peMode)
{
    stAVInfo[*peDevice].eAVSYNC_Mode = *peMode;

    db_print("================Demo_AV_SetAVSYNCMode================\n");
    db_print("0x0 : PCR_MODE\n");
    db_print("0x1 : AUDIO_MASTER_MODE\n");
    db_print("0x2 : VIDEO_MASTER_MODE\n");
    db_print("==============End Demo_AV_SetAVSYNCMode==============\n");


    db_print("Demo_AV_SetAVSYNCMode :%d\n",stAVInfo[*peDevice].eAVSYNC_Mode);

    if(stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE ||
        stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        Demo_AV_SetSYNC(*peDevice);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the Decode I frame mode
/// @param[in] u8mode 0x0: Mvop mode
///                                0x1: SW-detile + GOP
///                                0x2: DIP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_SetShowFrameMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_SetShowFrameMode(EN_AV_Device* peDevice, EN_AV_ShowFrame_Type* peMode)
{
    stAVInfo[*peDevice].eShowFrame_Mode= *peMode;

    db_print("================Demo_AV_SetShowFrameMode================\n");
    db_print("0x0 : MVOP_MODE\n");
    db_print("0x1 : SWDetile_MODE\n");
    db_print("0x2 : DIP_MODE\n");
    db_print("==============End Demo_AV_SetShowFrameMode==============\n");

    db_print("Demo_AV_SetShowFrame : %d\n",stAVInfo[*peDevice].eShowFrame_Mode);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Set Demux flow
/// @param[in] Main or Sub Device
/// @param[in] flow DMX flow
///        0: E_DMX_FLOW_LIVE0
///        1: E_DMX_FLOW_LIVE1
///        2: E_DMX_FLOW_LIVE2
///        3: E_DMX_FLOW_LIVE3
/// @param[in] pInsrc DMX flow input
///        0: E_DMX_FLOW_INPUT_DEMOD0
///        1: E_DMX_FLOW_INPUT_DEMOD1
///        2: E_DMX_FLOW_INPUT_MEM
/// @param[in] pClkInv TSin options: clock phase inversion
///        0: phases of these clocks are non-inverted
///        1: clock phase inversion
/// @param[in] pExtSync TSin options: sync by external signal
///        0: sync by internal signal
///        1: sync by external signal
/// @param[in] pParal TSin is parallel interface or serial interface
///        0: serial interface
///        1: parallel interface
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_Tuner_Config 0 0 0 0 0 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_Tuner_Config(
    EN_AV_Device* peDevice,
    EN_DEMO_DMX_FLOW* pePlayback,
    EN_DEMO_DMX_FLOW_INPUT* peDmxInput,
    int* pClkInv,
    int* pExtSync,
    int* pParal)
{

    MS_BOOL bClkInv;
    MS_BOOL bExtSync;
    MS_BOOL bParallel;

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if ((pClkInv == NULL) || (pExtSync == NULL) || (pParal == NULL)|| (pePlayback == NULL)||(peDmxInput == NULL))
    {
        db_print("Can not enter space\n");
        return FALSE;
    }

    db_print("========================================================\nplease enter(0/1):\n1. Device:%d\n2. Playpack:%d\n3. Tuner_Idx:%d\n4. Clk inv:%d\n5. ExtSync:%d\n6. Parallel:%d\n========================================================\n",
    *peDevice,*pePlayback,*peDmxInput,*pClkInv,*pExtSync,*pParal);

    bClkInv = (0 == *pClkInv)? FALSE: TRUE;
    bExtSync = (0 == *pExtSync)? FALSE: TRUE;
    bParallel = (0 == *pParal)? FALSE: TRUE;

    stAVInfo[*peDevice].stDMXflowParams.eFlow = *pePlayback;
    stAVInfo[*peDevice].stDMXflowParams.eDmxInput = *peDmxInput;

    stAVInfo[*peDevice].stDMXflowParams.DmxClkInv = *pClkInv;
    stAVInfo[*peDevice].stDMXflowParams.DmxExtSync = *pExtSync;
    stAVInfo[*peDevice].stDMXflowParams.DmxParallal = *pParal;


    if ( TRUE != Demo_DMX_FlowSet(stAVInfo[*peDevice].stDMXflowParams.eFlow, *peDmxInput, bClkInv, bExtSync, bParallel))
    {
        db_print("Demo_DMX_FlowSet fail\n");
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Set Video / Audio / PCR pid to device decoder path
/// @param[in] Main or Sub Device
/// @param[in] VideoPid Video PID
/// @param[in] AudioPid Audio PID
/// @param[in] PCRPid PCR PID
/// @param[in] pu32VCodec Video Codec (0: MPEG 1: H.264 2:AVS 3:HEVC)
/// @param[in] pu32ACodec Audio Codec Type
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_SetPid 0 101 102 101 0 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_TSP_SetPid(EN_AV_Device* peDevice,MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{

    EN_DEMO_DMX_FLT_SOURCE eDMX_SOURCE_TYPE;

    db_print("%d\n",*peDevice);
    db_print("%"DTC_MS_U32_d"\n",*pu32VideoPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32AudioPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32PCRPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32VCodec);
    db_print("%"DTC_MS_U32_d"\n",*pu32ACodec);

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

#if(DEMO_AUDIO_MULTI_TEST == 1)
    stAVInfo[*peDevice].stAudioParams.eFIFO_ID = Demo_AV_SetAudioInfo(*peDevice);
#else
    stAVInfo[*peDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
#endif

    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID0;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_AV_DEVICE_SUB)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID1;
    }
#endif

    stAVInfo[*peDevice].stVideoParams.u16PID = (*pu32VideoPid) & 0x1FFF;
    stAVInfo[*peDevice].stAudioParams.u16PID = (*pu32AudioPid) & 0x1FFF;
    stAVInfo[*peDevice].stPCRParams.u16PID = (*pu32PCRPid) & 0x1FFF;
    stAVInfo[*peDevice].stVideoParams.eVideoCodec = (*pu32VCodec) & 0xFF;
    stAVInfo[*peDevice].stAudioParams.eAudioCodec = (*pu32ACodec) & 0xFF;

    eDMX_SOURCE_TYPE = Demo_DMX_FlowToFltSrcMapping(stAVInfo[*peDevice].stDMXflowParams.eFlow);


    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX VideoFIFO Reset fail\n");
        goto Demo_AV_SetPid_fail;
    }
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioFIFO Reset fail\n");
        goto Demo_AV_SetPid_fail;
    }


    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
        db_print("MApi_DMX_Stop VideoFilter:%d\n",stAVInfo[*peDevice].stVideoParams.u8Filter);
    }
    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
        db_print("MApi_DMX_Stop AudioFilter:%d\n",stAVInfo[*peDevice].stAudioParams.u8Filter);
    }
    if (stAVInfo[*peDevice].stPCRParams.u8PCREngID!= INVALID_FILTER_ID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        stAVInfo[*peDevice].stPCRParams.u8PCREngID = INVALID_FILTER_ID;
        db_print("MApi_DMX_Stop PCREngID:%d\n",stAVInfo[*peDevice].stPCRParams.u8PCREngID);
    }

    if (TRUE != Demo_DMX_FltOpen( eDMX_SOURCE_TYPE, stAVInfo[*peDevice].stVideoParams.eFIFO_ID, &stAVInfo[*peDevice].stVideoParams.u8Filter, stAVInfo[*peDevice].stVideoParams.u16PID))
    {
        db_print("Demo DMX VideoFlt Open fail \n");
        goto Demo_AV_SetPid_fail;
    }
    if (TRUE != Demo_DMX_FltOpen( eDMX_SOURCE_TYPE, stAVInfo[*peDevice].stAudioParams.eFIFO_ID, &stAVInfo[*peDevice].stAudioParams.u8Filter, stAVInfo[*peDevice].stAudioParams.u16PID))
    {
        db_print("Demo DMX AudioFlt Open fail \n");
        goto Demo_AV_SetPid_fail;
    }
    if (TRUE != Demo_DMX_PCR_FltOpen(eDMX_SOURCE_TYPE, stAVInfo[*peDevice].stPCRParams.u16PID, &stAVInfo[*peDevice].stPCRParams.u8PCREngID))
    {
        db_print("Demo DMX PCRFlt Open fail \n");
        goto Demo_AV_SetPid_fail;
    }


    //DMX Flt Start
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stVideoParams.u8Filter))
    {
        db_print("MApi DMX VideoFlt start fail \n");
        goto Demo_AV_SetPid_fail;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stAudioParams.u8Filter))
    {
        db_print("MApi DMX AudioFlt start fail \n");
        goto Demo_AV_SetPid_fail;
    }


    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE)
    {
        if (TRUE != Demo_DMX_PCR_FltStart(stAVInfo[*peDevice].stPCRParams.u8PCREngID))
        {
            db_print("MApi DMX PCRFlt start fail \n");
            goto Demo_AV_SetPid_fail;
        }
    }

    db_print("Device filters are ready\n");

    return TRUE;

Demo_AV_SetPid_fail:

    db_print("Demo_AV_SetPid_fail \n");

    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stPCRParams.u8PCREngID != INVALID_FILTER_ID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        stAVInfo[*peDevice].stPCRParams.u8PCREngID = INVALID_FILTER_ID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Set Video / Audio pid to device decoder path (file-in)
/// @param[in] Main or Sub Device
/// @param[in] File-in engine type ("FILE" or "MMFI")
/// @param[in] File-in engine ID (MMFI: 0~1 , FILE: 0~3)
/// @param[in] VideoPid Video PID
/// @param[in] AudioPid Audio PID
/// @param[in] pu32VCodec Video Codec (0: MPEG 1: H.264 2:AVS 3:HEVC)
/// @param[in] pu32ACodec Audio Codec Type
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b DTV_TSP_SetPid 0 101 102 101 0 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_TSP_FileIn_SetPid(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *pu8EngID,char *pVideoPid,char *pAudioPid,MS_U32 *pu32VCodec,MS_U32 *pu32ACodec)
{
    EN_AV_AVSYNC_Type eAVsyncType = E_AV_SYNC_AUDIO_MASTER_MODE;
    DMX_MMFI_FLTTYPE eMMFI_VideoFltType = DMX_MMFI_FLTTYPE_AUD;
    DMX_MMFI_FLTTYPE eMMFI_AudioFltType = DMX_MMFI_FLTTYPE_AUD;
    EN_DEMO_DMX_FLT_SOURCE eDMX_Video_SOURCE_TYPE = 0;
    EN_DEMO_DMX_FLT_SOURCE eDMX_Audio_SOURCE_TYPE = 0;


    db_print("%d\n",*peDevice);
    db_print("%s\n",pFileInEngType);
    db_print("%d\n",*pu8EngID);
    db_print("%s\n",pVideoPid);
    db_print("%s\n",pAudioPid);
    db_print("%"DTC_MS_U32_d"\n",*pu32VCodec);
    db_print("%"DTC_MS_U32_d"\n",*pu32ACodec);

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //Dummy PCR Pid
    if (TRUE != Demo_DMX_PCR_FltOpen(E_DMX_FLT_SOURCE_LIVE0, INVALID_PID_ID, &stAVInfo[*peDevice].stPCRParams.u8PCREngID))
    {
        db_print("Demo DMX PCRFlt Open fail \n");
        goto Demo_AV_TSP_FileIn_SetPid_fail;
    }

#if(DEMO_AUDIO_MULTI_TEST == 1)
    stAVInfo[*peDevice].stAudioParams.eFIFO_ID = Demo_AV_SetAudioInfo(*peDevice);
#else
    stAVInfo[*peDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
#endif

    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID0;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_AV_DEVICE_SUB)
    {
        stAVInfo[*peDevice].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_VID1;
    }
#endif

    if(isalpha(pVideoPid[0]))
    {
        if(!Demo_Util_strcmp(pVideoPid,"VOID"))
        {
            db_print("not update video Pid !!\n");
        }
        else
        {
            db_print("video Pid is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }
    else
    {
        stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID = *pu8EngID;
        stAVInfo[*peDevice].stVideoParams.eVideoCodec = (*pu32VCodec) & 0xFF;

        if((pVideoPid[0]=='0') && (pVideoPid[1]=='X'|| pVideoPid[1]=='x')) //hex
        {
            stAVInfo[*peDevice].stVideoParams.u16PID = strtoul(pVideoPid,NULL,16) & 0x1FFF;
        }
        else
        {
            stAVInfo[*peDevice].stVideoParams.u16PID = strtoul(pVideoPid,NULL,10) & 0x1FFF;
        }

        //VFIFO Reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
        {
            db_print("Demo DMX VideoFIFO Reset fail\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }

        if(!Demo_Util_strcmp(pFileInEngType,"MMFI"))
        {
            stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType = E_AV_FileIn_Eng_MMFI;

            if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
            {
                MApi_DMX_MMFI_Pid_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
                db_print("MApi_DMX_MMFI_Pid_Close VideoFilter:%d\n",stAVInfo[*peDevice].stVideoParams.u8Filter);
            }

            switch(stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID)
            {
                case 0:
                    eMMFI_VideoFltType &= ~DMX_MMFI1_FLTTYPE_MASK;
                    break;
                case 1:
                    eMMFI_VideoFltType |= DMX_MMFI1_FLTTYPE_MASK;
                    break;
                default:
                    db_print("Not support this MMFI engine number !!\n");
                    break;
            }

            //EN_DEMO_DMX_FLT_TYPE -> DMX_MMFI_FLTTYPE
            eMMFI_VideoFltType |= Demo_DMX_MMFI_FltTypeMapping(stAVInfo[*peDevice].stVideoParams.eFIFO_ID);

            if(DMX_FILTER_STATUS_OK != MApi_DMX_MMFI_Pid_Open(eMMFI_VideoFltType,stAVInfo[*peDevice].stVideoParams.u16PID,&stAVInfo[*peDevice].stVideoParams.u8Filter))
            {
                db_print("Demo DMX MMFI VideoFlt Open fail \n");
                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }
        }
        else if(!Demo_Util_strcmp(pFileInEngType,"FILE"))
        {
            stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType = E_AV_FileIn_Eng_FILE;

            if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
            {
                MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
                MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
                db_print("MApi_DMX_Stop VideoFilter:%d\n",stAVInfo[*peDevice].stVideoParams.u8Filter);
            }

            switch(stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID)
            {
                case 0:
                    eDMX_Video_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE0;
                    break;
                case 1:
                    eDMX_Video_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE1;
                    break;
                case 2:
                    eDMX_Video_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE2;
                    break;
                case 3:
                    eDMX_Video_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE3;
                    break;
                default:
                    db_print("Not support this file-in engine num !!\n");
                    break;
            }

            if (TRUE != Demo_DMX_FltOpen(eDMX_Video_SOURCE_TYPE, stAVInfo[*peDevice].stVideoParams.eFIFO_ID, &stAVInfo[*peDevice].stVideoParams.u8Filter, stAVInfo[*peDevice].stVideoParams.u16PID))
            {
                db_print("Demo DMX VideoFlt Open fail \n");
                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }

            if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stVideoParams.u8Filter))
            {
                db_print("MApi DMX VideoFlt start fail \n");

                if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
                {
                    MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
                    MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                    stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
                }

                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }
        }
        else
        {
            db_print("video Engine type is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }

    if(isalpha(pAudioPid[0]))
    {
        if(!Demo_Util_strcmp(pAudioPid,"VOID"))
        {
            db_print("Not update audio Pid !!\n");
        }
        else
        {
            db_print("audio Pid is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }
    else
    {
        stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID = *pu8EngID;
        stAVInfo[*peDevice].stAudioParams.eAudioCodec = (*pu32ACodec) & 0xFF;

        if((pAudioPid[0]=='0') && (pAudioPid[1]=='X'|| pAudioPid[1]=='x')) //hex
        {
            stAVInfo[*peDevice].stAudioParams.u16PID = strtoul(pAudioPid,NULL,16) & 0x1FFF;
        }
        else
        {
            stAVInfo[*peDevice].stAudioParams.u16PID = strtoul(pAudioPid,NULL,10) & 0x1FFF;
        }

        //AFIFO reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            db_print("Demo DMX AudioFIFO Reset fail\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }

        if(!Demo_Util_strcmp(pFileInEngType,"MMFI"))
        {
            stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType = E_AV_FileIn_Eng_MMFI;

            if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
            {
                MApi_DMX_MMFI_Pid_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
                db_print("MApi_DMX_MMFI_Pid_Close AudioFilter:%d\n",stAVInfo[*peDevice].stAudioParams.u8Filter);
            }

            switch(stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID)
            {
                case 0:
                    eMMFI_AudioFltType &= ~DMX_MMFI1_FLTTYPE_MASK;
                    break;
                case 1:
                    eMMFI_AudioFltType |= DMX_MMFI1_FLTTYPE_MASK;
                    break;
                default:
                    db_print("Not support this MMFI engine number !!\n");
                    break;
            }

            //EN_DEMO_DMX_FLT_TYPE -> DMX_MMFI_FLTTYPE
            eMMFI_AudioFltType |= Demo_DMX_MMFI_FltTypeMapping(stAVInfo[*peDevice].stAudioParams.eFIFO_ID);

            if(DMX_FILTER_STATUS_OK != MApi_DMX_MMFI_Pid_Open(eMMFI_AudioFltType,stAVInfo[*peDevice].stAudioParams.u16PID,&stAVInfo[*peDevice].stAudioParams.u8Filter))
            {
                db_print("Demo DMX MMFI AudioFlt Open fail \n");
                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }
        }
        else if(!Demo_Util_strcmp(pFileInEngType,"FILE"))
        {
            stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType = E_AV_FileIn_Eng_FILE;

            if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
            {
                MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
                MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
                db_print("MApi_DMX_Stop AudioFilter:%d\n",stAVInfo[*peDevice].stAudioParams.u8Filter);
            }

            switch(stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID)
            {
                case 0:
                    eDMX_Audio_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE0;
                    break;
                case 1:
                    eDMX_Audio_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE1;
                    break;
                case 2:
                    eDMX_Audio_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE2;
                    break;
                case 3:
                    eDMX_Audio_SOURCE_TYPE = E_DMX_FLT_SOURCE_FILE3;
                    break;
                default:
                    db_print("Not support this file-in engine num !!\n");
                    break;
            }

            if (TRUE != Demo_DMX_FltOpen(eDMX_Audio_SOURCE_TYPE, stAVInfo[*peDevice].stAudioParams.eFIFO_ID, &stAVInfo[*peDevice].stAudioParams.u8Filter, stAVInfo[*peDevice].stAudioParams.u16PID))
            {
                db_print("Demo DMX AudioFlt Open fail \n");
                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }

            if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stAudioParams.u8Filter))
            {
                db_print("MApi DMX AudioFlt start fail \n");

                if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
                {
                    MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
                    MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                    stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
                }

                goto Demo_AV_TSP_FileIn_SetPid_fail;
            }
        }
        else
        {
            db_print("video Engine type is not valid !!\n");
            goto Demo_AV_TSP_FileIn_SetPid_fail;
        }
    }

    /**** [Need to Modify] ****/ // @Vincent
    Demo_AV_SetAVSYNCMode(peDevice,&eAVsyncType);//AUDIO master mode
    /**************************/

    db_print("Device filters are ready\n");
    return TRUE;

Demo_AV_TSP_FileIn_SetPid_fail:

    db_print("Demo_AV_TSP_FileIn_SetPid_fail \n");
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief  Play decoder
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayAV 0 \n
///                \b DTV_PlayAV 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayAV(EN_AV_Device* peDevice)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;

    EN_PCR_ENG eSetSTC = stAVInfo[*peDevice].stPCRParams.u8PCREngID;
    EN_VDEC_DDI_SrcMode eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_DTV;


    db_print("========[%s]========\n",__FUNCTION__);
    db_print("peDevice : %d ,Stream ID Before Init  0x%"DTC_MS_U32_x"\n",*peDevice,stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);
    db_print("Demo_AV_Set_STC : %d  \n",eSetSTC);

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {

        db_print("E_AV_SYNC_VIDEO_MASTER_MODE \n");
        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            db_print("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        //AVFIFO Reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            db_print("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

        //add audio unmute
        Demo_Audio_SetAbsoluteVolume(&u8Volume);

    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        //Audio Master mode
        db_print("E_AV_SYNC_AUDIO_MASTER_MODE \n");

        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV_AM,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            db_print("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            db_print("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

    }

    switch(stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType)
    {
        case E_AV_FileIn_Eng_MMFI:
            //eVDEC_SrcMode = E_DDI_VDEC_SRC_MODE_FILE;
            eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_TS_FILE;
            break;
        case E_AV_FileIn_Eng_FILE:
            eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_TS_FILE;
            break;
        default:
            eVDEC_SrcMode = E_VDEC_DDI_SRC_MODE_DTV;
            break;
    }

    if (TRUE != Demo_VDEC_Init((EN_VDEC_Device)*peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID),stAVInfo[*peDevice].stVideoParams.eVideoCodec,eVDEC_SrcMode,eSetSTC)) // default mepg2
    {
        db_print("Demo_VDEC_Init fail\n");
        return FALSE;
    }
    else
    {
        stAVInfo[*peDevice].bInited = TRUE;
        db_print("peDevice : %d ,Stream ID After Init  0x%"DTC_MS_U32_x"\n",*peDevice,stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);
    }
    if (TRUE != Demo_VDEC_Play((EN_VDEC_Device)*peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID),stAVInfo[*peDevice].eAVSYNC_Mode))
    {
        db_print("Demo_VDEC_Play fail\n");
        return FALSE;
    }
    else
    {
        db_print("Demo_VDEC_Play OK\n");
    }

    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,FALSE))
    {
        db_print("Demo DMX VideoFIFO reset fail\n");
        return FALSE;
    }

    //Check Video Decoder Status
    if( TRUE != Demo_VDEC_CheckDecoderStatus((EN_VDEC_Device*)peDevice))
    {
        db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }

    Demo_VDEC_IsAVSyncDone(*peDevice);

    //open timeout machenism => the data is enough to decode
    // if this function calls in the begining of vdec playback, the mosaic might happen
    _Demo_AV_TimeoutSet(*peDevice);

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief  Pause decoder
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PauseAV 0 \n
/// Command: \b DTV_PauseAV 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_PauseAV(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX VideoFIFO reset fail\n");
    }

    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioFIFO reset fail\n");
    }

    if (MApi_VDEC_EX_Pause(&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID)) != E_VDEC_EX_OK)
    {
        db_print("MApi_VDEC_EX_Pause fail\n");
        return FALSE;
    }

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV) != TRUE)
        {
            db_print("Demo_Audio_Stop fail\n");
            return FALSE;
        }
    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
        {
            db_print("Demo_Audio_Stop fail\n");
            return FALSE;
        }
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief  Stop decoder
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopAV 0 \n
/// Command: \b DTV_StopAV 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopAV(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    Demo_AV_MuteVideo(peDevice);

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX VideoFIFO reset fail\n");
    }
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioFIFO reset fail\n");
    }

    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stPCRParams.u8PCREngID != INVALID_FILTER_ID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        stAVInfo[*peDevice].stPCRParams.u8PCREngID = INVALID_FILTER_ID;
    }

    Demo_VDEC_Stop((EN_VDEC_Device)*peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID));

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV);
    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
        {
            db_print("Demo_Audio_Stop fail\n");
            return FALSE;
        }
    }

    stAVInfo[*peDevice].bInited = FALSE;
    stAVInfo[*peDevice].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;
    stAVInfo[*peDevice].eAVSYNC_Mode = E_AV_SYNC_PCR_MODE;

    db_print("Demo_AV_StopAV\n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop decoder (file-in)
/// @param[in] Main or Sub Device or Other Device
/// @param[in] File-in engine type ("FILE" or "MMFI")
/// @param[in] File-in engine ID (MMFI: 0~1 , FILE: 0~3)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopAV 0 \n
/// Command: \b DTV_StopAV 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_FileIn_StopAV(EN_AV_Device *peDevice,char *pFileInEngType,MS_U8 *u8EngID)
{
    EN_AV_FileIn_Eng_Type eEngType;


    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if(!Demo_Util_strcmp(pFileInEngType,"MMFI"))
    {
        eEngType = E_AV_FileIn_Eng_MMFI;
    }
    else if(!Demo_Util_strcmp(pFileInEngType,"FILE"))
    {
        eEngType = E_AV_FileIn_Eng_FILE;
    }
    else
    {
        db_print("Not support this engine type !!\n");
        return FALSE;
    }

    //PCR
    if (stAVInfo[*peDevice].stPCRParams.u8PCREngID != INVALID_FILTER_ID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        stAVInfo[*peDevice].stPCRParams.u8PCREngID = INVALID_FILTER_ID;
    }

    //Video
    if((eEngType == stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType) && (*u8EngID == stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID))
    {
        Demo_AV_MuteVideo(peDevice);

        //reset VFIFO
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
        {
            db_print("Demo DMX VideoFIFO reset fail\n");
        }

        if(stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
        {
            switch(eEngType)
            {
                case E_AV_FileIn_Eng_MMFI: MApi_DMX_MMFI_Pid_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                                        break;
                case E_AV_FileIn_Eng_FILE: MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
                                        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
                                        break;
                default:printf("[%s][%d] Impossible !!\n",__FUNCTION__, __LINE__);
                        break;
            }

            stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
        }

        Demo_VDEC_Stop((EN_VDEC_Device)*peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID));

        stAVInfo[*peDevice].stVideoParams.u16PID = INVALID_PID_ID;
        stAVInfo[*peDevice].stVideoParams.stFilterType.eEngType = INVALID_FILEIN_TYPE;
        stAVInfo[*peDevice].stVideoParams.stFilterType.u8EngID = INVALID_FILEIN_ID;
    }

    //Audio
    if((eEngType == stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType) && (*u8EngID == stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID))
    {
        //reset AFIFO
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
        {
            db_print("Demo DMX AudioFIFO reset fail\n");
        }

        if(stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
        {
            switch(eEngType)
            {
                case E_AV_FileIn_Eng_MMFI: MApi_DMX_MMFI_Pid_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                                        break;
                case E_AV_FileIn_Eng_FILE: MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
                                        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
                                        break;
                default:printf("[%s][%d] Impossible !!\n",__FUNCTION__, __LINE__);
                        break;
            }

            stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
        }

        //Audio
        if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
            || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
        {
            Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV);
        }
        else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
        {
            if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
            {
                db_print("Demo_Audio_Stop fail\n");
                return FALSE;
            }
        }

        stAVInfo[*peDevice].stAudioParams.u16PID = INVALID_PID_ID;
        stAVInfo[*peDevice].stAudioParams.stFilterType.eEngType = INVALID_FILEIN_TYPE;
        stAVInfo[*peDevice].stAudioParams.stFilterType.u8EngID = INVALID_FILEIN_ID;
    }

    if((stAVInfo[*peDevice].stVideoParams.u8Filter == INVALID_FILTER_ID) && (stAVInfo[*peDevice].stAudioParams.u8Filter == INVALID_FILTER_ID))
    {
        db_print("bInited = FALSE \n");
        stAVInfo[*peDevice].bInited = FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop AV FIFO
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b NA \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopAVFIFO(EN_AV_Device *peDevice)
{

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stVideoParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX VideoFIFO reset fail\n");
    }
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioFIFO reset fail\n");
    }

    if (stAVInfo[*peDevice].stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stVideoParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stVideoParams.u8Filter);
        stAVInfo[*peDevice].stVideoParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stPCRParams.u8PCREngID != INVALID_FILTER_ID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        stAVInfo[*peDevice].stPCRParams.u8PCREngID = INVALID_FILTER_ID;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Play Radio
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayRadio 0 \n
///                \b DTV_PlayRadio 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayRadio(EN_AV_Device* peDevice)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;

    EN_PCR_ENG eSetSTC = stAVInfo[*peDevice].stPCRParams.u8PCREngID;


    db_print("========[%s]========\n",__FUNCTION__);
    db_print("Demo_AV_Set_STC : %d  \n",eSetSTC);

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE)
    {
        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            db_print("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        //AVFIFO Reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            db_print("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

        //add audio unmute
        Demo_Audio_SetAbsoluteVolume(&u8Volume);

    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        //Audio Master mode

        if (Demo_Audio_Play(*peDevice,AUDIO_APP_DTV_AM,stAVInfo[*peDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            db_print("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            db_print("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

    }

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief  Stop Radio
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopRadio 0 \n
/// Command: \b DTV_StopRadio 1 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopRadio(EN_AV_Device *peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioFIFO reset fail\n");
    }

    if (stAVInfo[*peDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioParams.u8Filter);
        stAVInfo[*peDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }
    if (stAVInfo[*peDevice].stPCRParams.u8PCREngID != INVALID_FILTER_ID)
    {
        Demo_DMX_PCR_FltStop(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        Demo_DMX_PCR_FltClose(stAVInfo[*peDevice].stPCRParams.u8PCREngID);
        stAVInfo[*peDevice].stPCRParams.u8PCREngID = INVALID_FILTER_ID;
    }

    if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE)
    {
        Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV);
    }
    else if (stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AM) != TRUE)
        {
            db_print("Demo_Audio_Stop fail\n");
            return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  mute video
/// @param[in] Main or Sub Device or Other Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_MuteVideo(EN_AV_Device *peDevice)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = *peDevice;
    MS_U32 u32DisplayMute = ENABLE;
    EN_ZAPPING_TYPE eZappingType = E_ZAPPING_NORMAL;

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    Demo_Zapping_GetZappingType(&eZappingType);

    switch(eZappingType)
    {
        case E_ZAPPING_XC_SEAMLESS:
    #if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
            if(Demo_XC_SeamlessZapping_IsFreeze(&u32XCDevice, &u32XCWindow) == FALSE)
            {
                Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute HDMI  :XC0

                if(*peDevice == E_AV_DEVICE_MAIN)
                {
                #if (DEMO_XC_DUALXC_TEST == 1)
                    u32XCDevice = 1;                                          //XC1
                    Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute CVBS
                #endif
                    Demo_VE_SetVideoMute(&u32DisplayMute);                             // Mute CVBS

                    db_print("Demo_VE_SetVideoMute:%ld\n",u32DisplayMute);
                }
            }
    #endif
            break;

        case E_ZAPPING_GOP_SEAMLESS:
        case E_ZAPPING_FQ:
            break;

        case E_ZAPPING_NORMAL:
        default:
            Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute HDMI  :XC0

            if(*peDevice == E_AV_DEVICE_MAIN)
            {
            #if (DEMO_XC_DUALXC_TEST == 1)
                u32XCDevice = 1;                                          //XC1
                Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute CVBS
            #endif
                Demo_VE_SetVideoMute(&u32DisplayMute);                             // Mute CVBS

                db_print("Demo_VE_SetVideoMute:%"DTC_MS_U32_d"\n",u32DisplayMute);
            }
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_PlayFromMemory \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  0 : for Main 1: for Sub
/// @param[in]  0: MPEG2  1:AVC  2:AVS 3:HEVC 4:MPEG4  5:VC1 6:VP9
/// @param[in]  play loop number
/// @param[in]  Reserved for dynamic scale demo
/// @param[in]  input source
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
//    \b DTV_PlayFromMemory 0 1 0  /applications/avc.bin     \n
//    \b DTV_PlayFromMemory 0 0 0  /applications/mpeg2.bin  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayFromMemory(EN_AV_Device* peDevice,int* pVCodec, int* pPlayLoop, char* pPath)
{
    MS_U32 u32DstAddr = 0;
    MS_U32 u32DstAddr2 = 0;
    MS_U32 u32ActualBitstreamSize = 0;
    MS_U32 u32ActualBitstreamSize2 = 0;

    db_print("=====[%s]=====\n",__FUNCTION__);

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    switch(*pVCodec)
    {
        case E_VDEC_DDI_CODEC_TYPE_H264:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_H264_MEMORY;
            db_print(" VideoCodec : H264 \n");
            break;
        case E_VDEC_DDI_CODEC_TYPE_AVS:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY;
            db_print(" VideoCodec : AVS \n");
            break;
        case E_VDEC_DDI_CODEC_TYPE_HEVC:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY;
            db_print(" VideoCodec : HEVC \n");
            break;
        case 4:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MPEG4_MEMORY;
            db_print(" VideoCodec : MPEG4 \n");
            break;
        case 5:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_VC1_ADV_MEMORY;
            db_print(" VideoCodec : VC1 \n");
            break;
        case 6:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY;
            db_print(" VideoCodec : VP9 \n");
        case E_VDEC_DDI_CODEC_TYPE_MPEG2:
        default:
            stAVInfo[*peDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY;
            db_print(" VideoCodec : MPEG2 \n");
            break;
    }

    db_print(" Stream ID Before Init  0x%"DTC_MS_U32_x"\n",stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);

    if (Demo_VDEC_Init((EN_VDEC_Device)*peDevice,&(stAVInfo[*peDevice].stVideoParams.stVDECSteamID),stAVInfo[*peDevice].stVideoParams.eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE,0) == FALSE) // default mepg2
    {
        db_print("Demo_VDEC_Init fail\n");
        return FALSE;
    }
    else
    {
        db_print(" Stream ID After Init 0x%"DTC_MS_U32_x"\n",stAVInfo[*peDevice].stVideoParams.stVDECSteamID.u32Id);
    }

    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        u32DstAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if (*peDevice == E_AV_DEVICE_SUB)
    {
        u32DstAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_MEMORY_TYPE);
    }
#endif

    /* Load Stream to ES Buffer */

    u32DstAddr = u32DstAddr+ DRV_PROC_SIZE;
    db_print("B bitstream 0x%08"DTC_MS_U32_x"\n",u32DstAddr);
    u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32DstAddr,stAVInfo[*peDevice].stVideoParams.eVideoCodec ,pPath);
    db_print("u32ActualBitstreamSize %"DTC_MS_U32_d" \n",u32ActualBitstreamSize);


    if (u32ActualBitstreamSize == 0)
    {
        db_print("File Size 0 \n");
        return FALSE;
    }

    if (TRUE != Demo_VDEC_FilePlay((EN_VDEC_Device)*peDevice,&stAVInfo[*peDevice].stVideoParams.stVDECSteamID,u32DstAddr,u32ActualBitstreamSize,*pPlayLoop,u32DstAddr2,u32ActualBitstreamSize2,stAVInfo[*peDevice].eShowFrame_Mode))
    {
        db_print("Demo_VDEC_FilePlay fail\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_StopFromMemory \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  0 : for Main 1: for Sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b DTV_StopFromMemory 0  \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_StopFromMemory(EN_AV_Device* peDevice)
{
    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    Demo_AV_MuteVideo(peDevice);

    if (TRUE != Demo_VDEC_FileStop((EN_VDEC_Device)*peDevice))
    {
        db_print("Demo_VDEC_FileStop fail\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_DecodeIFrame \n
/// - For NO MVOP SCALER:
///     - VDEC decode I Frame
///     - get video information and do SW Detile
///     - Use GOP to show
/// @param[in]  Main or Sub Device
/// @param[in] 0:MPEG2 1.H.264 2.AVS 3.HEVC
/// @Param[in] file path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
///  DTV_DecIFrame 0 1 /applications/ifram.bin
///  DTV_DecIFrame 1 1 /applications/ifram.bin
///  DTV_DecIFrame 1 0 /applications/ifram2.bin
///  DTV_DecIFrame 0 0 /applications/ifram2.bin
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_DecodeIFrame(EN_AV_Device* peDevice,int* pCodec,char* pLogopath)
{
    MS_U8 u8GOPNum;
    MS_U32 u32ColorType;
    MS_U32 u32GWinWidth;
    MS_U32 u32GWinHeight;

    VDEC_EX_FrameInfo stFrameInfo;

    DEMO_AV_CHECK_DEVICE_ID(*peDevice)

    memset(&stFrameInfo, 0, sizeof(VDEC_EX_FrameInfo));

    if(Demo_VDEC_DecIFrame((EN_VDEC_Device*)peDevice,pCodec,pLogopath) != TRUE)
    {
        db_print("Demo_VDEC_DecIFrame FAIL!!!\n");
        return FALSE;
    }

    if(stAVInfo[*peDevice].eShowFrame_Mode == E_AV_SWDetile_MODE)
    {
        if(Demo_VDEC_GetDecFrameInfo((EN_VDEC_Device*)peDevice,&stFrameInfo) != TRUE)
        {
            db_print("Demo_VDEC_GetDecFrameInfo FAIL!!!\n");
            return FALSE;
        }

        db_print("Demo_VDEC_GetDecFrameInfo !!\n");
        db_print("LumaAddr [0x%"DTC_MS_PHY_x"], ChromaAddr [0x%"DTC_MS_PHY_x"]\n",stFrameInfo.u32LumaAddr,stFrameInfo.u32ChromaAddr);
        db_print("H [%d], V [%d], Pitch [%d]\n",stFrameInfo.u16Width,stFrameInfo.u16Height,stFrameInfo.u16Pitch);

        u8GOPNum = 0;
        u32ColorType = 4;
        u32GWinWidth = stFrameInfo.u16Width;
        u32GWinHeight = stFrameInfo.u16Height;

        Demo_OSD_CreateWin(&u32ColorType, &u32GWinWidth, &u32GWinHeight, &u32GWinWidth, &u32GWinHeight);

        if(Demo_VDEC_SW_DeTile((EN_VDEC_Device*)peDevice, &u8GOPNum,&stFrameInfo) != TRUE)
        {
            db_print("Demo_VDEC_SW_DeTile FAIL!!!\n");
            return FALSE;
        }
    }

    return TRUE;
}

#if(DEMO_AUDIO_AD_TEST == 1)
//------------------------------------------------------------------------------
/// @brief  Play AD
/// @param[in] Main or Sub Device
/// @param[in] u16AudioADPID Audio AD PID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_PlayAD 0 102\n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_PlayAD(EN_AV_Device* peDevice, MS_U16* u16AudioADPID)
{
    MS_BOOL bADMute = FALSE;
    MS_U8 u8Volume = DEFAULT_VOLUME;
    EN_DEMO_DMX_FLT_SOURCE eDMX_SOURCE_TYPE;

    if (*peDevice > 0)
    {
        db_print("Not support multi AD \n");
        return FALSE;
    }

    stAVInfo[*peDevice].stAudioADParams.u16PID = *u16AudioADPID;
    stAVInfo[*peDevice].stAudioADParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD1;

    eDMX_SOURCE_TYPE = Demo_DMX_FlowToFltSrcMapping(stAVInfo[*peDevice].stDMXflowParams.eFlow);

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioADParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioADFIFO Reset fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    if (stAVInfo[*peDevice].stAudioADParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        stAVInfo[*peDevice].stAudioADParams.u8Filter = INVALID_FILTER_ID;
        db_print("MApi_DMX_Stop AudioADFilters:%d\n",stAVInfo[*peDevice].stAudioADParams.u8Filter);
    }

    //DMX Flt Open and SetPid
    if (TRUE != Demo_DMX_FltOpen( eDMX_SOURCE_TYPE, stAVInfo[*peDevice].stAudioADParams.eFIFO_ID, &stAVInfo[*peDevice].stAudioADParams.u8Filter, stAVInfo[*peDevice].stAudioADParams.u16PID))
    {
        db_print("Demo DMX AudioADFilter Open fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    //DMX Flt Start
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[*peDevice].stAudioADParams.u8Filter))
    {
        db_print("MApi DMX AudioADFilter Start fail\n");
        goto Demo_AV_PlayAD_SetPid_fail;
    }

    if(*peDevice == E_AV_DEVICE_MAIN)
    {
        db_print("Main AD filters are ready\n");
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if(*peDevice == E_AV_DEVICE_SUB)
    {
        db_print("Sub AD filters are ready\n");
    }
#endif

    Demo_Audio_Play(*peDevice, AUDIO_APP_DTV_AD, stAVInfo[*peDevice].stAudioParams.eAudioCodec, NULL);

    //open audio AD FIFO
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioADParams.eFIFO_ID,FALSE))
    {
        db_print("Demo DMX AudioADFIFO Reset fail\n");
    }

    MApi_AUDIO_SetADOutputMode(AD_OUT_BOTH);
    Demo_Audio_SetADAbsoluteVolume(&u8Volume);
    Demo_Audio_ADSetMute(&bADMute);

    return TRUE;

Demo_AV_PlayAD_SetPid_fail:

    db_print("Demo_AV_PlayAD_SetPid_fail\n");

    if (stAVInfo[*peDevice].stAudioADParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        stAVInfo[*peDevice].stAudioADParams.u8Filter = INVALID_FILTER_ID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief  Stop AD
/// @param[in] Main or Sub Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DTV_StopAD 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_StopAD(EN_AV_Device* peDevice)
{
    if (*peDevice > 0)
    {
        db_print("Not support multi AD \n");
        return FALSE;
    }

    //Stop audio AD FIFO
    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[*peDevice].stAudioADParams.eFIFO_ID,TRUE))
    {
        db_print("Demo DMX AudioADFIFO Reset fail\n");
    }

    if (stAVInfo[*peDevice].stAudioADParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        MApi_DMX_Close(stAVInfo[*peDevice].stAudioADParams.u8Filter);
        stAVInfo[*peDevice].stAudioADParams.u8Filter = INVALID_FILTER_ID;
    }

    if (TRUE != Demo_Audio_Stop(*peDevice, AUDIO_APP_DTV_AD))
    {
        db_print("Demo_Audio_Stop fail\n");
        return FALSE;
    }

    return TRUE;
}
#endif

#if(DEMO_MVC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief to Demo_AV_MVC_PlayFromMemory \n
/// @param[in] play loop number
/// @param[in] file path
/// @param[in] file path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b DTV_MVC_PlayFromMemory \n
//  DTV_MVC_PlayFromMemory 0 /applications/mvc.bin
//  DTV_MVC_PlayFromMemory 8 /applications/mvc_0.bin  /applications/mvc_1.bin
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_MVC_PlayFromMemory(int* pPlayLoop, char* pPath1,char* pPath2)
{
    MS_U32 u32ActualBitstreamSize = 0;
    MS_U32 u32ActualBitstreamSize2 = 0;
    MS_U32 u32DstAddr = 0;
    MS_U32 u32DstAddr2 = 0;
    EN_AV_Device eDevice = 0;

    memset(&stAVInfo[eDevice],  0 , sizeof(ST_AV_INFO));

    stAVInfo[eDevice].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY;

    if (*pPath2 != NULL)
    {
        Demo_VDEC_Init((EN_VDEC_Device)eDevice,&stAVInfo[eDevice].stVideoParams.stVDECSteamID,stAVInfo[eDevice].stVideoParams.eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE_DUAL_ES,0);
    }
    else
    {
        Demo_VDEC_Init((EN_VDEC_Device)eDevice,&stAVInfo[eDevice].stVideoParams.stVDECSteamID,stAVInfo[eDevice].stVideoParams.eVideoCodec,E_VDEC_DDI_SRC_MODE_FILE,0);
    }

    u32DstAddr = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
    u32DstAddr = u32DstAddr+ DRV_PROC_SIZE;
    db_print("A bitstream 0x%08lx\n",u32DstAddr);
    u32ActualBitstreamSize = Demo_VDEC_UtlLoadStream(u32DstAddr,E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY,pPath1);
    db_print("u32ActualBitstreamSize %ld \n",u32ActualBitstreamSize);

    if (*pPath2 != NULL)
    {
        db_print("=======Load 2nd Binary for Sub=======\n");
        u32DstAddr2 = MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR + PLAY_SIZE_LIMITITATION, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
        u32ActualBitstreamSize2 = Demo_VDEC_UtlLoadStream(u32DstAddr2,E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY,pPath2);

        if (u32ActualBitstreamSize2 == 0)
        {
            db_print("File2 Size 0\n");
            return FALSE;
        }
    }

    if (u32ActualBitstreamSize == 0)
    {
        db_print("File1 Size 0\n");
        return FALSE;
    }

    u32DstAddr = u32DstAddr - MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
    u32DstAddr2 = u32DstAddr2 - MEM_ADR_BY_MIU(VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);

    if (TRUE != Demo_VDEC_FilePlay((EN_VDEC_Device)eDevice,&stAVInfo[eDevice].stVideoParams.stVDECSteamID,u32DstAddr,u32ActualBitstreamSize,*pPlayLoop,u32DstAddr2,u32ActualBitstreamSize2,stAVInfo[eDevice].eShowFrame_Mode))
    {
        db_print("Demo_VDEC_FilePlay fail\n");
        return FALSE;
    }


   return TRUE;
}

//------------------------------------------------------------------------------
/// @brief to Demo_AV_MVC_StopFromMemory \n
/// - For Video Decoder:
///     - check display information is ready
///     - check frame count is increasing
///     - check AV sync done
/// - For MVOP and SCALER:
///     - get video information and set output information of MVOP
///     - the output of MVOP is the input of SCALER
/// @param[in]  0 : for Main 1: for Sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b DTV_MVC_StopFromMemory  \n
//------------------------------------------------------------------------------

MS_BOOL Demo_AV_MVC_StopFromMemory(void)
{
    EN_AV_Device eDevice = 0;

    Demo_AV_MuteVideo(&eDevice);

    if (TRUE != Demo_VDEC_FileStop((EN_VDEC_Device)eDevice))
    {
        db_print("Demo_VDEC_FileStop fail\n");
        return FALSE;
    }

    return TRUE;
}
#endif


//------------------------------------------------------------------------------
/// @brief to Demo_AV_ChangeAudioTrack \n
/// - For HbbTV:
///     - Change DTV language
/// @param[in]  0 : for Main 1: for Sub
/// @param[in]  1 : audio pid
/// @param[in]  2 : audio codec type. Ex. AC3 or AAC ....etc
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b Demo_AV_ChangeAudioTrack  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_AV_ChangeAudioTrack(const EN_AV_Device* peDevice, const MS_U32* pu32AudioPid, const EN_AUDIO_CODEC_TYPE* peAudioCodecType)
{

    EN_DEMO_DMX_FLT_SOURCE eDMX_SOURCE_TYPE;
    const EN_AV_Device eDevice = *peDevice;
    MS_U8 u8Volume = DEFAULT_VOLUME;

    printf("[%s][%d] device %d\n", __FUNCTION__, __LINE__,(int)peDevice);
    printf("[%s][%d] audio pid %d\n", __FUNCTION__, __LINE__, (int)(*pu32AudioPid));

    DEMO_AV_CHECK_DEVICE_ID(eDevice)

#if(DEMO_AUDIO_MULTI_TEST == 1)
    stAVInfo[eDevice].stAudioParams.eFIFO_ID = Demo_AV_SetAudioInfo(eDevice);
#else
    stAVInfo[eDevice].stAudioParams.eFIFO_ID = E_DMX_FLT_TYPE_AUD0;
#endif

    stAVInfo[eDevice].stAudioParams.u16PID = (*pu32AudioPid) & 0x1FFF;
    stAVInfo[eDevice].stAudioParams.eAudioCodec = *peAudioCodecType;

    eDMX_SOURCE_TYPE = Demo_DMX_FlowToFltSrcMapping(stAVInfo[*peDevice].stDMXflowParams.eFlow);

    if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[eDevice].stAudioParams.eFIFO_ID,TRUE))
    {
        printf("[%s][%d]Demo DMX AudioFIFO Reset fail\n", __FUNCTION__, __LINE__);
        goto Demo_AV_ChangeAudioTrack_fail;
    }

    if (stAVInfo[eDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[eDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[eDevice].stAudioParams.u8Filter);
        stAVInfo[eDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
        printf("[%s][%d] MApi_DMX_Stop AudioFilter:%d\n", __FUNCTION__, __LINE__,stAVInfo[eDevice].stAudioParams.u8Filter);
    }

    if (stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        Demo_Audio_Stop(eDevice, AUDIO_APP_DTV);
    }
    else if (stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        if (Demo_Audio_Stop(eDevice, AUDIO_APP_DTV_AM) != TRUE)
        {
            printf("[%s][%d]Demo_Audio_Stop fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }

    if (TRUE != Demo_DMX_FltOpen( eDMX_SOURCE_TYPE, stAVInfo[eDevice].stAudioParams.eFIFO_ID, &stAVInfo[eDevice].stAudioParams.u8Filter, stAVInfo[eDevice].stAudioParams.u16PID))
    {
        printf("[%s][%d]Demo DMX AudioFlt Open fail \n", __FUNCTION__, __LINE__);
        goto Demo_AV_ChangeAudioTrack_fail;
    }


    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(stAVInfo[eDevice].stAudioParams.u8Filter))
    {
        printf("[%s][%d]MApi DMX AudioFlt start fail \n", __FUNCTION__, __LINE__);
        goto Demo_AV_ChangeAudioTrack_fail;
    }


    printf("[%s][%d] Device filters are ready\n", __FUNCTION__, __LINE__);

    EN_PCR_ENG eSetSTC = stAVInfo[*peDevice].stPCRParams.u8PCREngID;

    if (stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_PCR_MODE
        || stAVInfo[*peDevice].eAVSYNC_Mode == E_AV_SYNC_VIDEO_MASTER_MODE)
    {
        if (Demo_Audio_Play(eDevice,AUDIO_APP_DTV,stAVInfo[eDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            printf("[%s][%d] Demo_Audio_Play fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] Demo_Audio_Play ok\n", __FUNCTION__, __LINE__);
        }

        //AVFIFO Reset
        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[eDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            printf("[%s][%d]Demo DMX AudioFIFO reset fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        //add audio unmute
        Demo_Audio_SetAbsoluteVolume(&u8Volume);

    }
    else if (stAVInfo[eDevice].eAVSYNC_Mode == E_AV_SYNC_AUDIO_MASTER_MODE)
    {
        //Audio Master mode

        if (Demo_Audio_Play(eDevice,AUDIO_APP_DTV_AM,stAVInfo[eDevice].stAudioParams.eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            db_print("Demo_Audio_Play fail\n");
            return FALSE;
        }
        else
        {
            db_print("Demo_Audio_Play ok\n");
        }

        if (TRUE != Demo_DMX_AVFifo_Reset(stAVInfo[eDevice].stAudioParams.eFIFO_ID,FALSE))
        {
            db_print("Demo DMX AudioFIFO reset fail\n");
            return FALSE;
        }

    }


    return TRUE;

Demo_AV_ChangeAudioTrack_fail:

    printf("Demo_AV_SetPid_fail \n");

    if (stAVInfo[eDevice].stAudioParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stAVInfo[eDevice].stAudioParams.u8Filter);
        MApi_DMX_Close(stAVInfo[eDevice].stAudioParams.u8Filter);
        stAVInfo[eDevice].stAudioParams.u8Filter = INVALID_FILTER_ID;
    }

    return FALSE;
}

#endif

