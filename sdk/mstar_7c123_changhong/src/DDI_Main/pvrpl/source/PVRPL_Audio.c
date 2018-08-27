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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_Audio.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "drvDTC.h"

#include "PVRPL_Audio.h"
#include "PVRPL_FileIn.h"

#include <string.h>
#include "MsMemory.h"
#include "drvAUDIO.h"
#include "drvGPIO.h"
#include "drvAUDIO_if.h"

#include "apiAUDIO.h"
#include "apiHDMITx.h"
#include "apiDMX.h"
#include "PVRPL_ResourcePool.h"


/****************** START CONSTANT ******************/
#define PVR_AUDIO_MAX_NUM           2
#define DEFAULT_VOLUME              100
/******************* END CONSTANT *******************/

/****************** START LOCAL GLOBAL ******************/
static MS_BOOL _bAudioInit      = FALSE;
static MS_U32  _u32PVRPLAUDIODbgLevel = PVRPL_DBG_ERR;
/******************* END LOCAL GLOBAL *******************/

/****************** START LOCAL FUNCTIONS ******************/
static void _PVRPL_Audio_Set(En_DVB_decSystemType AudioCodecType);

//mutiple audio setting function
static MS_BOOL _PVRPL_Audio_OpenDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo, AudioDecStatus_t *stAudioDecStatus,EN_AUDIO_APP_TYPE eAppType, En_DVB_decSystemType eCodecType);
static MS_BOOL _PVRPL_Audio_CloseDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo,AudioDecStatus_t *stAudioDecStatus);
static MS_BOOL _PVRPL_Audio_SetSystem(PVRPL_AUDIO_DEC_INFO *AudioInfo, En_DVB_decSystemType eCodecType);
static MS_BOOL _PVRPL_Audio_STC_Switch(PVRPL_AUDIO_DEC_INFO *AudioInfo, AUDIO_STC_SOURCE eSTC_Source);
/******************* END LOCAL FUNCTIONS *******************/

#define PVRPL_AUDIO_DBGMSG(_level, msg, args...)    {if(_u32PVRPLAUDIODbgLevel >= (_level)) printf("[%s][%d] " msg, __FUNCTION__, __LINE__, ## args);}
#define MOD_NAME                                    PVR

#if defined(HB_ERR)
    #define PVRPL_AUDIO_DBGMSG_ERR(msg, args...)    HB_ERR(msg, ##args)
#else
    #define PVRPL_AUDIO_DBGMSG_ERR(msg, args...)    PVRPL_AUDIO_DBGMSG(PVRPL_DBG_ERR, msg, ##args)
#endif

#if defined(HB_ERR)
    #define PVRPL_AUDIO_DBGMSG_MUST(msg, args...)    HB_ERR(msg, ##args)
#else
    #define PVRPL_AUDIO_DBGMSG_MUST(msg, args...)    PVRPL_AUDIO_DBGMSG(PVRPL_DBG_MUST, msg, ##args)
#endif

#if defined(HB_INFO)
    #define PVRPL_AUDIO_DBGMSG_INFO(msg, args...)   HB_INFO(msg, ##args)
#else
    #define PVRPL_AUDIO_DBGMSG_INFO(msg, args...)   PVRPL_AUDIO_DBGMSG(PVRPL_DBG_INFO, msg, ##args)
#endif

#if defined(HB_TRACE)
    #define PVRPL_AUDIO_DBGMSG_TRACE(msg, args...)  HB_TRACE(msg, ##args)
#else
    #define PVRPL_AUDIO_DBGMSG_TRACE(msg, args...)  PVRPL_AUDIO_DBGMSG(PVRPL_DBG_TRACE, msg, ##args)
#endif

#if defined(HB_DBG)
    #define PVRPL_AUDIO_DBGMSG_DEBUG(msg, args...)  HB_DBG(msg, ##args)
#else
    #define PVRPL_AUDIO_DBGMSG_DEBUG(msg, args...)  PVRPL_AUDIO_DBGMSG(PVRPL_DBG_FUNC, msg, ##args)
#endif

static AUDIO_DEC_ID _PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(EN_AUDIO_CPL_DEC_ID eDecID);
static EN_AUDIO_CPL_DEC_ID _AUDIO_DEC_ID_To_PVRPL_AUDIO_CPL_DEC_ID_Mapping(AUDIO_DEC_ID eDecID);


// Audio Module
PVRPL_AUDIO_STATUS PVRPL_Audio_Init(PVRPL_AUDIO_DEC_INFO *AudioInfo,PVRPL_ACODEC_TYPE u32ACodec,MS_U8 u8STCEng,void *pAudioID)
{
//@NOTE Demo_Audio_Init
// todo  new audio interface needs to add
// audio get system => adec ID  and return audio Handle to PVR MW
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(AudioInfo->u8PathIdx);
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    AudioDecStatus_t stAudioDecStatus = {0};
    En_DVB_decSystemType AudioCodecType;

    switch (u32ACodec)
    {
        case PVRPL_ACODEC_MPEG:
            AudioCodecType = MSAPI_AUD_DVB_MPEG;
            PVRPL_AUDIO_DBGMSG_TRACE("[Audio] ACodec=%d  [MPEG]\n",AudioCodecType);
            break;
        case PVRPL_ACODEC_AC3:
            AudioCodecType = MSAPI_AUD_DVB_AC3;
            PVRPL_AUDIO_DBGMSG_TRACE("[Audio] ACodec=%d [AC3]\n",AudioCodecType);
            break;
        case PVRPL_ACODEC_AC3P:
            AudioCodecType = MSAPI_AUD_DVB_AC3P;
            PVRPL_AUDIO_DBGMSG_TRACE("[Audio] ACodec=%d [AC3P]\n",AudioCodecType);
            break;
        case PVRPL_ACODEC_AAC:
            AudioCodecType = MSAPI_AUD_DVB_AAC;
            PVRPL_AUDIO_DBGMSG_TRACE("[Audio] ACodec=%d [AAC]\n",AudioCodecType);
            break;
        case PVRPL_ACODEC_DRA:
            AudioCodecType = MSAPI_AUD_DVB_DRA;
            PVRPL_AUDIO_DBGMSG_TRACE("[Audio] ACodec=%d [DRA]\n",AudioCodecType);
            break;
        default:
            AudioCodecType = MSAPI_AUD_DVB_NONE;
            PVRPL_AUDIO_DBGMSG_TRACE("[Audio] ACodec=%d [NONE]\n",AudioCodecType);
            break;
    }

    if (FALSE == _bAudioInit)
    {
        MApi_AUDIO_WritePreInitTable();

        SystemInfo.tv_system=TV_PAL;
        SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
        SystemInfo.dtv_system=0;
        MApi_AUDIO_SetSystemInfo(&SystemInfo);

        OutputInfo.SpeakerOut=AUDIO_OUTPUT_MAIN_SPEAKER;
        OutputInfo.HpOut=AUDIO_OUTPUT_HP;
        OutputInfo.ScartOut=AUDIO_OUTPUT_SCART1;//AUDIO_OUTPUT_SIFOUT;
        OutputInfo.MonitorOut=AUDIO_NULL_OUTPUT;//AUDIO_OUTPUT_LINEOUT;
        MApi_AUDIO_SetOutputInfo(&OutputInfo);

        PathInfo.SpeakerOut = AUDIO_PATH_MAIN_SPEAKER;
        PathInfo.HpOut = AUDIO_PATH_HP;
        PathInfo.MonitorOut = AUDIO_NULL_OUTPUT;
        PathInfo.SpdifOut = AUDIO_PATH_SPDIF;
        PathInfo.ScartOut = AUDIO_PATH_SCART1;
        MApi_AUDIO_SetPathInfo(&PathInfo);

#if (DEMO_AUDIO_R2_MEM_ARCHI_TEST)
        MDrv_AUDIO_SetDspBaseAddr(DSP_ADV, 0, MAD_ADV_BUF_ADR);
#else
        MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
#endif

        MApi_AUDIO_Initialize();
        MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_DTV_IN);
        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);

    //need to think  the vaule depends on boards
#ifdef AUDIO_MUTE_GPIO
        AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif
        _bAudioInit = TRUE;
    }

    //@TODO
    //add returning handler to pvr mw and pvr using the handler to operate Audio
    AudioInfo->bAInit = TRUE;
    AudioInfo->eDecID = _AUDIO_DEC_ID_To_PVRPL_AUDIO_CPL_DEC_ID_Mapping(Pool_GetAudioID(u8PathIdx));

    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        PVRPL_AUDIO_DBGMSG_TRACE("Getfrom demoaudio multi Audio Decode ID:%d Path Idx:%u\n", AudioInfo->eDecID,u8PathIdx);

        //maybe have problem =>
        //1. call audio API function=> AudioInfo->stAudioDecStatus_t be inited
        //2. memset(&(AudioInfo->stAudioDecStatus_t), 0x00, sizeof(AudioDecStatus_t))
        // cause core dump in audio_module destructor function
        if (MApi_AUDIO_GetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), &stAudioDecStatus) == FALSE)
        {
            PVRPL_AUDIO_DBGMSG_ERR("MApi_AUDIO_GetDecodeSystem fail!!!\n");
            return FALSE;
        }

        /* release decode system */
        //_PVRPL_Audio_CloseDecSys(AudioInfo,&stAudioDecStatus);

        /* init decoder system for main */
        if(stAudioDecStatus.bConnect == FALSE)
        {
            if(_PVRPL_Audio_OpenDecSys(AudioInfo,&stAudioDecStatus, E_AUDIO_INFO_DTV_IN, AudioCodecType) == FALSE)
            {
                PVRPL_AUDIO_DBGMSG_TRACE("open decode system (%u) fail.\n", u8PathIdx);
            }
        }

        //set default channel path, or set output source may be fail
        //DSP1 -> main channel (ch5)
        //DSP2 -> sub channel (ch7)
        if(u8PathIdx == 0)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
        }
        else if(u8PathIdx == 1)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP3_DVB_INPUT, E_AUDIO_GROUP_SUB);
        }
    }

    //@NOTE Demo_Audio_Play

    _PVRPL_Audio_Set(AudioCodecType);

    AUDIO_STC_SOURCE eSTC_Source;
    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        _PVRPL_Audio_SetSystem(AudioInfo, AudioCodecType);
        // source connect and set codec type

        //TODO: the PCR Eng need care to get from filein module
        if(u8STCEng == E_PVRPL_PCR_ENG0)
        {
            eSTC_Source = E_TSP_0;
        }
        else//E_PCR_ENG1
        {
            eSTC_Source = E_TSP_1;
        }

        if(_PVRPL_Audio_STC_Switch(AudioInfo, eSTC_Source) == FALSE)
        {
            PVRPL_AUDIO_DBGMSG_TRACE("PathIdx=%u, set STC fail!!!\n", u8PathIdx);
        }

        MApi_AUDIO_SourceConnect(stAudioDecStatus.eGroup, TRUE);
    }
    else
    {
        MApi_AUDIO_SetSystem(AudioCodecType);
    }
    // The audio play is for both program change and set audio track
    PVRPL_Audio_Play(AudioInfo);
    //TODO
    //need to consider reset audio or audio3D
    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), FALSE);
    PVRPL_AUDIO_DBGMSG_TRACE("Audio Decode ID:%d \n", AudioInfo->eDecID);
    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_GetInfoWithPARAU64(PVRPL_AUDIO_DEC_INFO *AudioInfo, EN_AUDIO_CPL_INFO eAudioCmd, MS_U64 *u64pmtr, void* structure)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(AudioInfo->u8PathIdx);
    switch(eAudioCmd)
    {
        case EN_AUDIO_CPL_INFO_STCPTS_DIFF:
            if(u8PathIdx==0)
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF);
            }
            else
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF);
            }
            break;
        case EN_AUDIO_CPL_INFO_PCMBUFF_LEVEL:
            if(u8PathIdx==0)
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_pcmBuf_currLevel);
            }
            else
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_pcmBuf_currLevel);
            }
            break;
        case EN_AUDIO_CPL_INFO_ESBUFF_LEVEL:
            if(u8PathIdx==0)
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_currLevel);
            }
            else
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_currLevel);
            }
            break;
        case EN_AUDIO_CPL_INFO_IS_SYNC:
            *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Is_Audio_Sync);
            break;
        default:
            break;
    }
    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_GetInfoWithPARAU32(PVRPL_AUDIO_DEC_INFO *AudioInfo, EN_AUDIO_CPL_INFO eAudioCmd, MS_U32 *u32pmtr, void* structure)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(AudioInfo->u8PathIdx);
    switch(eAudioCmd)
    {
        case EN_AUDIO_CPL_INFO_PTS_IN_33BITS:
            if(u8PathIdx==0)
            {
                *u32pmtr=(MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS);
            }
            else
            {
                *u32pmtr=(MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_33bit_PTS);
            }
            break;
        case EN_AUDIO_CPL_INFO_GET_WATER_LEVEL:
            if(u8PathIdx==0)
            {
                *u32pmtr = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_currLevel);
            }
            else
            {
                *u32pmtr = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_currLevel);
            }

            break;
        case EN_AUDIO_CPL_INFO_GET_TIMESTAMP:
            if(MApi_AUDIO_GetAudioInfo2(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), Audio_infoType_esBuf_req, (void *)u32pmtr) == FALSE)
            {
                PVRPL_AUDIO_DBGMSG_ERR("Get audio information (%u) failed!!!\n", AudioInfo->eDecID);
                return PVRPL_AUDIO_STATUS_ERROR;
            }
            break;
        case EN_AUDIO_CPL_INFO_IS_SYNC:
            *u32pmtr = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Is_Audio_Sync);
            break;
        default:
            break;
    }

    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_SetSpeed(PVRPL_AUDIO_DEC_INFO *AudioInfo, MS_S32 s32Speed)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(AudioInfo->u8PathIdx);
    if (u8PathIdx >= PVR_AUDIO_MAX_NUM)
    {
        PVRPL_AUDIO_DBGMSG_ERR("ERROR PVR_Audio Play , audio path index(%u)!!!\n", u8PathIdx);
        return PVRPL_AUDIO_STATUS_ERROR;
    }
    AudioInfo->s32ASpeed = s32Speed;

    if (AudioInfo->s32ASpeed == 0) //Mute audio
    {
       if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
       {
           MApi_AUDIO_SetDecodeCmd(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), MSAPI_AUD_DVB_DECCMD_PAUSE);
       }
       else
       {
           MApi_AUDIO_PauseDecode();
       }
    }
    else if (AudioInfo->s32ASpeed == 1) //Mute audio
    {
        PVRPL_Audio_SetMute(AudioInfo,u8PathIdx, TRUE);
        MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), TRUE);
    }
    else
    {
        if (AudioInfo->s32ASpeed== 1000)
        {
            MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), FALSE);
            PVRPL_AUDIO_DBGMSG_TRACE("Set Normal mode \n");
            PVRPL_Audio_SetMute(AudioInfo,u8PathIdx, TRUE);

            AudioInfo->u32StartSyncTime = MsOS_GetSystemTime();
            AudioInfo->bMonitorSyncStatus = MAPI_TRUE;

            PVRPL_Audio_Play(AudioInfo);
        }
        else
        {
            PVRPL_AUDIO_DBGMSG_TRACE("Set Trick mode \n");
            PVRPL_Audio_SetMute(AudioInfo,u8PathIdx, TRUE);
            MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathIdx), TRUE);
        }
    }

    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_Play(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    if (GET_PVR_PATH_IDX(AudioInfo->u8PathIdx) >= PVR_AUDIO_MAX_NUM)
    {
        PVRPL_AUDIO_DBGMSG_ERR("ERROR PVR_Audio Mute , audio path index(%u)!!!\n",GET_PVR_PATH_IDX(AudioInfo->u8PathIdx));
        return PVRPL_AUDIO_STATUS_ERROR;
    }
    //_CHECK_ADEC_VID_FAIL_WITH_RETURN(*AudioID);

    if(AudioInfo->eAVSyncMode == E_PVRPL_AV_SYNC_PCR_MASTER)
    {
        // AV sync
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), MSAPI_AUD_DVB_DECCMD_AVSYNC);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
        }

        // Play with audio decoder delay
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), MSAPI_AUD_DVB_DECCMD_PLAY);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
        }
    }
    else if((AudioInfo->eAVSyncMode == E_PVRPL_AV_SYNC_AUDIO_MASTER) || (AudioInfo->eAVSyncMode ==  E_PVRPL_AV_SYNC_FREE_RUN))
    {
        // Free run
        PVRPL_AUDIO_DBGMSG_TRACE("Audio free run\n");
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }

        // Play without audio decoder delay
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
        }
    }
    MsOS_DelayTask(20); // 20 ms, suggested by audio team
    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_SetMute(PVRPL_AUDIO_DEC_INFO *AudioInfo,MS_U8 u8PathID, MS_BOOL bMute)
{
    //_CHECK_ADEC_VID_FAIL_WITH_RETURN(*AudioID);
    //AudioDecStatus_t p_AudioDecStatus;
    //MS_BOOL bConnect = !bMute;
    MS_U32 u32AudParm = bMute ? 1 : 0;
    AUDIO_DEC_ID AudioID = AU_DEC_INVALID;

    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        AudioID = AudioInfo->bAInit ? AudioInfo->eDecID : Pool_GetAudioID(u8PathID);
        MApi_AUDIO_SetAudioParam2(AudioID,Audio_ParamType_mute,u32AudParm);
    }
    else
    {
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_mute, u32AudParm, 0);
    }
    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_FlushData(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    if (GET_PVR_PATH_IDX(AudioInfo->u8PathIdx) >= PVR_AUDIO_MAX_NUM)
    {
        PVRPL_AUDIO_DBGMSG_ERR("ERROR PVR_Audio Clear , audio path Handler(%d)!!!\n",GET_PVR_PATH_IDX(AudioInfo->u8PathIdx));
        return PVRPL_AUDIO_STATUS_ERROR;
    }

    //1. ADEC flsuh
    //if(enAudPlayType==EN_PVR_AUDPLAYERTYPE_ESFILE)
    //{
        // mantis 138014
    //    MsOS_DelayTask(30);  // 30 ms, suggested by audio team. Stop and play command have about 30ms time duration to let audio DSP recognize stop command.
    //}
    PVRPL_Audio_Stop(AudioInfo);
    MsOS_DelayTask(20); // 20 ms, suggested by audio team
    PVRPL_Audio_Play(AudioInfo);

    //2. Resume audio cmd
    PVRPL_Audio_SetSpeed(AudioInfo,AudioInfo->s32ASpeed);

    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_Stop(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetDecodeCmd(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), MSAPI_AUD_DVB_DECCMD_STOP);
    }
    else
    {
        MApi_AUDIO_StopDecode();
    }

    return PVRPL_AUDIO_STATUS_OK;
}

PVRPL_AUDIO_STATUS PVRPL_Audio_Exit(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(GET_PVR_PATH_IDX(AudioInfo->u8PathIdx)), TRUE);
    PVRPL_Audio_Stop(AudioInfo);

    AudioDecStatus_t stAudioDecStatus;
    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if(MApi_AUDIO_GetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), &stAudioDecStatus) == FALSE)
        {
            PVRPL_AUDIO_DBGMSG_ERR("Get decode system (%u) failed!!!\n", AudioInfo->eDecID);
        }
        _PVRPL_Audio_CloseDecSys(AudioInfo,&stAudioDecStatus);
    }
    AudioInfo->bAInit = FALSE;
    _bAudioInit = FALSE;
    return PVRPL_AUDIO_STATUS_OK;
}



void _PVRPL_Audio_Set(En_DVB_decSystemType AudioCodecType)
{
    //MS_U8 u8Volume = DEFAULT_VOLUME;
    if (MSAPI_AUD_DVB_NONE == AudioCodecType)
    {
        return;
    }

    //Set MAD
    //MDrv_AUDIO_SetNormalPath(AUDIO_PATH_MAIN_SPEAKER, AUDIO_SOURCE_DTV, AUDIO_OUTPUT_MAIN_SPEAKER);

    //PVRPL_Audio_SetMute(u8AudioHandle, bMute);
    //mute line out
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, FALSE);
    //mute HDMI PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, FALSE);
    //mute HDMI PCM + non PCM
    MApi_AUDIO_HDMI_Tx_SetMute(FALSE);
    //mute SPDIF PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, FALSE);
    //mute SPDIF PCM + non PCM
    MApi_AUDIO_SPDIF_SetMute(FALSE);

    /* TODO START:  HDMITx Audio*/
    MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
    /* TODO END:    HDMITx Audio*/

    //u8Volume = DEFAULT_VOLUME;
    //appDemo_Audio_SetAbsoluteVolume(&u8Volume);
}

MS_BOOL _PVRPL_Audio_OpenDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo,AudioDecStatus_t *stAudioDecStatus, EN_AUDIO_APP_TYPE eAppType, En_DVB_decSystemType eCodecType)
{
    PVR_PATH u8PathIdx = GET_PVR_PATH_IDX(AudioInfo->u8PathIdx);
    AUDIO_SOURCE_INFO_TYPE eSrcType;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType;

    if(stAudioDecStatus->bConnect == TRUE)
    {
        PVRPL_AUDIO_DBGMSG_INFO("decode system (%u) already opened!!!\n", u8PathIdx);
        return TRUE;
    }

    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
        eSrcType = E_AUDIO_INFO_GAME_IN;
        break;
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AD:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
    default:
        eSrcType = E_AUDIO_INFO_GAME_IN;
    }

    if(u8PathIdx== 0)
    {
        eOutSrcType = E_CONNECT_MAIN;
    }
    else if(u8PathIdx == 1)
    {
        eOutSrcType = E_CONNECT_SUB;
    }
    else
    {
        return FALSE;
    }

    memset(stAudioDecStatus, 0x00, sizeof(AudioDecStatus_t));

    //set status for main
    stAudioDecStatus->bIsAD = FALSE;
    stAudioDecStatus->eAudFormat  = eCodecType;
    stAudioDecStatus->eSourceType = eSrcType;
    stAudioDecStatus->eGroup = eOutSrcType;

    //open main decoder system
    AudioInfo->eDecID = _AUDIO_DEC_ID_To_PVRPL_AUDIO_CPL_DEC_ID_Mapping(MApi_AUDIO_OpenDecodeSystem(stAudioDecStatus));
    if(AudioInfo->eDecID == _AUDIO_DEC_ID_To_PVRPL_AUDIO_CPL_DEC_ID_Mapping(AU_DEC_INVALID));
    {
        PVRPL_AUDIO_DBGMSG_ERR("decode system (%u) failed!!!\n", u8PathIdx);
        return FALSE;
    }

    if(MApi_AUDIO_GetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), stAudioDecStatus) == FALSE)
    {
        PVRPL_AUDIO_DBGMSG_ERR("Get decode system (%u) failed!!!\n", AudioInfo->eDecID);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL _PVRPL_Audio_CloseDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo,AudioDecStatus_t *stAudioDecStatus)
{
    if((AudioInfo->eDecID != EN_AUDIO_CPL_DEC_INVALID) && (stAudioDecStatus->bConnect == TRUE))
    {
        PVRPL_AUDIO_DBGMSG_TRACE("_PVRPL_Audio_CloseDecSys \n");
        //@NOTE In PVR there is no need to release decode system
        //No need to release Audio decode when mode not change ex: DTV mode to MM mode
        //If call this api the DDI main can't go back live becuase DTV flow does not open dec sys again
        /* MApi_AUDIO_ReleaseDecodeSystem(AudioInfo->eDecID); */
        memset(stAudioDecStatus, 0x00, sizeof(AudioDecStatus_t));
        AudioInfo->eDecID = -1;
    }

    return TRUE;
}

MS_BOOL _PVRPL_Audio_SetSystem(PVRPL_AUDIO_DEC_INFO *AudioInfo, En_DVB_decSystemType eCodecType)
{
    AudioDecStatus_t stDecStatus;
    MS_BOOL bret = FALSE;

    bret = MApi_AUDIO_GetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), &stDecStatus);
    if (bret == FALSE)
    {
        PVRPL_AUDIO_DBGMSG_ERR("MApi_AUDIO_GetDecodeSystem fail!!!\n");
        return FALSE;
    }

    stDecStatus.eAudFormat = eCodecType;
    bret = MApi_AUDIO_SetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), &stDecStatus);
    if (bret == FALSE)
    {
        PVRPL_AUDIO_DBGMSG_ERR("MApi_AUDIO_SetDecodeSystem fail!!!\n");
        return FALSE;
    }

    return TRUE;
}

MS_BOOL _PVRPL_Audio_STC_Switch(PVRPL_AUDIO_DEC_INFO *AudioInfo, AUDIO_STC_SOURCE eSTC_Source)
{
    AudioDecStatus_t stDecStatus;
    MS_BOOL bret = MApi_AUDIO_GetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), &stDecStatus);

    if (bret == FALSE)
    {
        PVRPL_AUDIO_DBGMSG_ERR("MApi_AUDIO_GetDecodeSystem fail!!!\n");
        return FALSE;
    }

    stDecStatus.eStcSource = eSTC_Source;
    bret = MApi_AUDIO_SetDecodeSystem(_PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(AudioInfo->eDecID), &stDecStatus);
    if (bret == FALSE)
    {
        PVRPL_AUDIO_DBGMSG_ERR("MApi_AUDIO_SetDecodeSystem fail!!!\n");
        return FALSE;
    }

    return TRUE;
}

static AUDIO_DEC_ID _PVRPL_AUDIO_CPL_DEC_ID_To_AUDIO_DEC_ID_Mapping(EN_AUDIO_CPL_DEC_ID eDecID)
{
    switch(eDecID)
    {
        case EN_AUDIO_CPL_DEC_ID1:
            return AU_DEC_ID1;
        case EN_AUDIO_CPL_DEC_ID2:
            return AU_DEC_ID2;
        case EN_AUDIO_CPL_DEC_ID3:
            return AU_DEC_ID3;
        default:
            PVRPL_AUDIO_DBGMSG_ERR("Invalid DEC ID  %u\n", eDecID);
            return AU_DEC_INVALID;
    }
}

static EN_AUDIO_CPL_DEC_ID _AUDIO_DEC_ID_To_PVRPL_AUDIO_CPL_DEC_ID_Mapping(AUDIO_DEC_ID eDecID)
{
    switch(eDecID)
    {
        case AU_DEC_ID1:
            return EN_AUDIO_CPL_DEC_ID1;
        case AU_DEC_ID2:
            return EN_AUDIO_CPL_DEC_ID2;
        case AU_DEC_ID3:
            return EN_AUDIO_CPL_DEC_ID3;
        default:
            PVRPL_AUDIO_DBGMSG_ERR("Invalid DEC ID  %u\n", eDecID);
            return EN_AUDIO_CPL_DEC_INVALID;
    }
}
