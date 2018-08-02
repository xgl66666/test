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
#include "drvDTC.h"



/****************** START CONSTANT ******************/
#define PVR_AUDIO_MAX_NUM 2

#define EN_PVR_AUDPLAYERTYPE_NORMAL 0
#define EN_PVR_AUDPLAYERTYPE_ESFILE 1

#define EN_PVR_AUDPLAYMODE_AVSYNC 0
#define EN_PVR_AUDPLAYMODE_FREERUN 1

#define DEFAULT_VOLUME 100
/******************* END CONSTANT *******************/


/****************** START LOCAL GLOBAL ******************/
static MS_U8 enAudPlayType = EN_PVR_AUDPLAYERTYPE_NORMAL;
static MS_U8 enAudPlayMode = EN_PVR_AUDPLAYMODE_AVSYNC;
static MS_U32 u32PlayTimeInMs=0;
static MS_BOOL _bAudioInit=FALSE;
static MS_U32  _u32PVRPLAUDIODbgLevel = PVRPL_DBG_ERR;
#define PVRPL_AUDIO_DBGMSG(_level,_f) {if(_u32PVRPLAUDIODbgLevel >= (_level)) (_f);}

/******************* END LOCAL GLOBAL *******************/

/****************** START LOCAL FUNCTIONS ******************/
static MS_BOOL _PVRPL_Audio_SetNormalMode(AUDIO_DEC_ID AudioID);
static MS_BOOL _PVRPL_Audio_SetTrickMode(AUDIO_DEC_ID AudioID);
static void _PVRPL_Audio_Set(En_DVB_decSystemType AudioCodecType);

//mutiple audio setting function
static MS_BOOL _PVRPL_Audio_OpenDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo, AudioDecStatus_t *stAudioDecStatus,EN_AUDIO_APP_TYPE eAppType, En_DVB_decSystemType eCodecType);
static MS_BOOL _PVRPL_Audio_CloseDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo,AudioDecStatus_t *stAudioDecStatus);
static MS_BOOL _PVRPL_Audio_SetSystem(PVRPL_AUDIO_DEC_INFO *AudioInfo, En_DVB_decSystemType eCodecType);
static MS_BOOL _PVRPL_Audio_STC_Switch(PVRPL_AUDIO_DEC_INFO *AudioInfo, AUDIO_STC_SOURCE eSTC_Source);
/******************* END LOCAL FUNCTIONS *******************/


// Audio Module
PVRPL_AUDIO_STATUS PVRPL_Audio_Init(PVRPL_AUDIO_DEC_INFO *AudioInfo,MS_U32 u32ACodec,MS_U8 u8STCEng,void *pAudioID)
{

#if (DEMO_PVR_V4_TEST == 1)
//@NOTE Demo_Audio_Init
// todo  new audio interface needs to add
// audio get system => adec ID  and return audio Handle to PVR MW
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    AudioDecStatus_t stAudioDecStatus;
    En_DVB_decSystemType AudioCodecType;

    switch (u32ACodec)
    {
    case 0x01:
        AudioCodecType = MSAPI_AUD_DVB_MPEG;
        break;
    case 0x02:
        AudioCodecType = MSAPI_AUD_DVB_AC3;
        break;
    case 0x03:
        AudioCodecType = MSAPI_AUD_DVB_AC3P;
        break;
    case 0x04:
        AudioCodecType = MSAPI_AUD_DVB_AAC;
        break;
    default:
        AudioCodecType = MSAPI_AUD_DVB_NONE;
        break;
    }

    if (TRUE == _bAudioInit)
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

        //MW_AUD_GetBinAddress();//expand to next two functions:
#if (DEMO_AUDIO_R2_MEM_ARCHI_TEST)
        // if new IC (Keres/Clippers)
        MDrv_AUDIO_SetDspBaseAddr(DSP_ADV, 0, MAD_ADV_BUF_ADR);
#else
        MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
#endif

        //Todo SE Buffer need to check if added or not  => Keltic not need to add
        //MDrv_AUDIO_SetDspBaseAddr(DSP_SE,  0, MAD_SE_BUF_ADR);

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

    AudioInfo->eDecID = Pool_GetAudioID(AudioInfo->u8PathIdx);

    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        PVRPL_AUDIO_DBGMSG(PVRPL_DBG_TRACE, printf("[%s][%d] Getfrom demoaudio multi Audio Decode ID:%d Path Idx:%d\n",__FUNCTION__,__LINE__, AudioInfo->eDecID,AudioInfo->u8PathIdx));

        //maybe have problem =>
        //1. call audio API function=> AudioInfo->stAudioDecStatus_t be inited
        //2. memset(&(AudioInfo->stAudioDecStatus_t), 0x00, sizeof(AudioDecStatus_t))
        // cause core dump in audio_module destructor function
        if (MApi_AUDIO_GetDecodeSystem(AudioInfo->eDecID, &stAudioDecStatus) == FALSE)
        {
            printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        /* release decode system */
        //_PVRPL_Audio_CloseDecSys(AudioInfo,&stAudioDecStatus);

        /* init decoder system for main */
        if(stAudioDecStatus.bConnect == FALSE)
        {
            if(_PVRPL_Audio_OpenDecSys(AudioInfo,&stAudioDecStatus, E_AUDIO_INFO_DTV_IN, MSAPI_AUD_DVB_MPEG) == FALSE)
            {
                printf("[%s][%d] open decode system (%d) fail.\n", __FUNCTION__, __LINE__,AudioInfo->u8PathIdx);
            }
        }

        //set default channel path, or set output source may be fail
        //DSP1 -> main channel (ch5)
        //DSP2 -> sub channel (ch7)
        if(AudioInfo->u8PathIdx == 0)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
        }
        else if(AudioInfo->u8PathIdx == 1)
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
            printf("[%s][%d] PathIdx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, AudioInfo->u8PathIdx);
        }

        MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_AVSYNC);
        MApi_AUDIO_SourceConnect(stAudioDecStatus.eGroup, TRUE);

    }
    else
    {
        MApi_AUDIO_SetSystem(AudioCodecType);
        MApi_AUDIO_SetCommand((En_DVB_decCmdType)MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
    }

    //TODO
    //need to consider reset audio or audio3D
    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(AudioInfo->u8PathIdx), FALSE);

    //printf("[%s][%d] Audio Decode ID:%d \n",__FUNCTION__,__LINE__, AudioInfo->eDecID);

#else
    AUDIO_DEC_ID *pAudioDecID = (AUDIO_DEC_ID *)pAudioID;
    AudioInfo->bAInit = TRUE;
    AudioInfo->eDecID = *pAudioDecID;
    //printf("[%s][%s][%d] Adec Dec id:%d\n",__FILE__,__FUNCTION__,__LINE__,AudioInfo->eDecID);
#endif

    return PVRPL_AUDIO_STATUS_OK;
}


PVRPL_AUDIO_STATUS PVRPL_Audio_GetInfoWithPARAU64(PVRPL_AUDIO_DEC_INFO *AudioInfo, EN_AUDIO_CPL_INFO eAudioCmd, MS_U64 *u64pmtr, void* structure)
{

    switch(eAudioCmd)
    {
        case EN_AUDIO_CPL_INFO_STCPTS_DIFF:
            if(AudioInfo->u8PathIdx==0)
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF);
            }
            else
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF);
            }
            break;
        case EN_AUDIO_CPL_INFO_PCMBUFF_LEVEL:
            if(AudioInfo->u8PathIdx==0)
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_pcmBuf_currLevel);
            }
            else
            {
                *u64pmtr = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_pcmBuf_currLevel);
            }
            break;
        case EN_AUDIO_CPL_INFO_ESBUFF_LEVEL:
            if(AudioInfo->u8PathIdx==0)
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
    switch(eAudioCmd)
    {
        case EN_AUDIO_CPL_INFO_PTS_IN_33BITS:
            if(AudioInfo->u8PathIdx==0)
            {
                *u32pmtr=(MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS);
            }
            else
            {
                *u32pmtr=(MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_33bit_PTS);
            }
            break;
        case EN_AUDIO_CPL_INFO_GET_WATER_LEVEL:
            if(AudioInfo->u8PathIdx==0)
            {
                *u32pmtr = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_currLevel);
            }
            else
            {
                *u32pmtr = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_currLevel);
            }

            break;
        case EN_AUDIO_CPL_INFO_GET_TIMESTAMP:
            MApi_AUDIO_GetAudioInfo2(AudioInfo->eDecID, Audio_infoType_esBuf_req, (void *)u32pmtr);
            break;
        case EN_AUDIO_CPL_INFO_IS_SYNC:
            *u32pmtr = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Is_Audio_Sync);
            break;
        default:

            break;
    }
    return PVRPL_AUDIO_STATUS_OK;

}


PVRPL_AUDIO_STATUS PVRPL_Audio_SetSpeed(PVRPL_AUDIO_DEC_INFO *AudioInfo, MS_S16 speed)
{
    if (AudioInfo->u8PathIdx >= PVR_AUDIO_MAX_NUM)
    {
        printf("ERROR PVR_Audio Play , audio path index(%d)!!!\n",AudioInfo->u8PathIdx);
        return PVRPL_AUDIO_STATUS_ERROR;
    }
    AudioInfo->s16ASpeed = speed;
    if (AudioInfo->s16ASpeed == 0) //Mute audio
    {
       if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
       {
           MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_PAUSE);
       }
       else
       {
           MApi_AUDIO_PauseDecode();
       }
    }
    else if (AudioInfo->s16ASpeed == 1) //Mute audio
    {
        PVRPL_Audio_SetMute(AudioInfo,AudioInfo->u8PathIdx, TRUE);
        _PVRPL_Audio_SetTrickMode(AudioInfo->eDecID);
    }
    else
    {
        if (AudioInfo->s16ASpeed== 1000)
        {
            PVRPL_AUDIO_DBGMSG(PVRPL_DBG_TRACE, printf("Set Normal mode \n"));

            PVRPL_Audio_SetMute(AudioInfo,AudioInfo->u8PathIdx, TRUE);

            AudioInfo->u32StartSyncTime = MsOS_GetSystemTime();
            AudioInfo->bMonitorSyncStatus = MAPI_TRUE;

            PVRPL_Audio_Play(AudioInfo);
            _PVRPL_Audio_SetNormalMode(AudioInfo->eDecID);
        }
        else
        {
            PVRPL_AUDIO_DBGMSG(PVRPL_DBG_TRACE, printf("Set Trick mode \n"));
            PVRPL_Audio_SetMute(AudioInfo,AudioInfo->u8PathIdx, TRUE);
            _PVRPL_Audio_SetTrickMode(AudioInfo->eDecID);
        }
    }
    return PVRPL_AUDIO_STATUS_OK;

}


PVRPL_AUDIO_STATUS PVRPL_Audio_Play(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    if (AudioInfo->u8PathIdx >= PVR_AUDIO_MAX_NUM)
    {
        printf("ERROR PVR_Audio Mute , audio path index(%d)!!!\n",AudioInfo->u8PathIdx);
        return PVRPL_AUDIO_STATUS_ERROR;
    }

    //_CHECK_ADEC_VID_FAIL_WITH_RETURN(*AudioID);
    if(enAudPlayType==EN_PVR_AUDPLAYERTYPE_ESFILE)
    {
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_PLAYFILE);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
        }
        u32PlayTimeInMs=0;
    }
    else
    {
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_PLAY);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
        }
    }

    if(enAudPlayMode==EN_PVR_AUDPLAYMODE_FREERUN)
    {
        if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
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
    MS_U32 u32AudParm;
    AUDIO_DEC_ID AudioID=AU_DEC_INVALID;
    if(bMute)
    {
        u32AudParm = 1;
    }
    else
    {
        u32AudParm = 0;
    }

    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {

        if (AudioInfo->bAInit == FALSE)
        {
            AudioID = Pool_GetAudioID(u8PathID);
        }
        else
        {
            AudioID = AudioInfo->eDecID;
        }
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
    if (AudioInfo->u8PathIdx >= PVR_AUDIO_MAX_NUM)
    {
        printf("ERROR PVR_Audio Clear , audio path Handler(%d)!!!\n",AudioInfo->u8PathIdx);
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
    PVRPL_Audio_SetSpeed(AudioInfo,AudioInfo->s16ASpeed);

    return PVRPL_AUDIO_STATUS_OK;

}


PVRPL_AUDIO_STATUS PVRPL_Audio_Stop(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetDecodeCmd(AudioInfo->eDecID, MSAPI_AUD_DVB_DECCMD_STOP);
    }
    else
    {
        MApi_AUDIO_StopDecode();
    }
    return PVRPL_AUDIO_STATUS_OK;

}


PVRPL_AUDIO_STATUS PVRPL_Audio_Exit(PVRPL_AUDIO_DEC_INFO *AudioInfo)
{
    //_bAudioInit = FALSE;
    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(AudioInfo->u8PathIdx), TRUE);
#if (DEMO_PVR_V4_TEST == 1)
    PVRPL_Audio_Stop(AudioInfo);
#endif

    AudioDecStatus_t stAudioDecStatus;
    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_GetDecodeSystem(AudioInfo->eDecID, &stAudioDecStatus);
        _PVRPL_Audio_CloseDecSys(AudioInfo,&stAudioDecStatus);
    }
    AudioInfo->bAInit = FALSE;
    _bAudioInit = FALSE;
    return PVRPL_AUDIO_STATUS_OK;

}

/*
AUDIO_DEC_ID _PVRPL_Audio_GETADEC_ID(MS_U8 u8AudioHandle)
{
    if (u8AudioHandle >= PVR_AUDIO_MAX_NUM)
    {
        return AU_DEC_ID1;
    }
    return stDualAudioInfo[u8AudioHandle].eDecID;
}
*/
MS_BOOL _PVRPL_Audio_SetNormalMode(AUDIO_DEC_ID AudioID)
{
    return TRUE;
}

MS_BOOL _PVRPL_Audio_SetTrickMode(AUDIO_DEC_ID AudioID)
{

    if(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
         MApi_AUDIO_SetDecodeCmd(AudioID, MSAPI_AUD_DVB_DECCMD_SKIP);
    }
    else
    {
         MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_SKIP);
    }
    return TRUE;
}


void _PVRPL_Audio_Set(En_DVB_decSystemType AudioCodecType)

{
    //MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;

    if (MSAPI_AUD_DVB_NONE == AudioCodecType)
    {
        return;
    }

    //Set MAD
    //MDrv_AUDIO_SetNormalPath(AUDIO_PATH_MAIN_SPEAKER, AUDIO_SOURCE_DTV, AUDIO_OUTPUT_MAIN_SPEAKER);


    //PVRPL_Audio_SetMute(u8AudioHandle, bMute);
    //mute line out
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, bMute);
    //mute HDMI PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, bMute);
    //mute HDMI PCM + non PCM
    MApi_AUDIO_HDMI_Tx_SetMute(bMute);
    //mute SPDIF PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, bMute);
    //mute SPDIF PCM + non PCM
    MApi_AUDIO_SPDIF_SetMute(bMute);

    /* TODO START:  HDMITx Audio*/
    MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
    /* TODO END:    HDMITx Audio*/

    //u8Volume = DEFAULT_VOLUME;
    //appDemo_Audio_SetAbsoluteVolume(&u8Volume);

}


#if 1
MS_BOOL _PVRPL_Audio_OpenDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo,AudioDecStatus_t *stAudioDecStatus, EN_AUDIO_APP_TYPE eAppType, En_DVB_decSystemType eCodecType)
{
    AUDIO_SOURCE_INFO_TYPE eSrcType;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType;

    if(stAudioDecStatus->bConnect == TRUE)
    {
        printf("[%s][%d] decode system (%u) already opened!!!\n", __FUNCTION__, __LINE__, AudioInfo->u8PathIdx);
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

    if(AudioInfo->u8PathIdx== 0)
    {
        eOutSrcType = E_CONNECT_MAIN;
    }
    else if(AudioInfo->u8PathIdx == 1)
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
    AudioInfo->eDecID = MApi_AUDIO_OpenDecodeSystem(stAudioDecStatus);
    if(AudioInfo->eDecID == (EN_AUDIO_CPL_DEC_ID)AU_DEC_INVALID)
    {
        PVRPL_AUDIO_DBGMSG(PVRPL_DBG_ERR, printf("[%s][%d] decode system (%u) failed!!!\n", __FUNCTION__, __LINE__, AudioInfo->u8PathIdx));
        return FALSE;
    }

    MApi_AUDIO_GetDecodeSystem(AudioInfo->eDecID, stAudioDecStatus);

    return TRUE;
}

MS_BOOL _PVRPL_Audio_CloseDecSys(PVRPL_AUDIO_DEC_INFO *AudioInfo,AudioDecStatus_t *stAudioDecStatus)
{
    if((AudioInfo->eDecID != EN_AUDIO_CPL_DEC_INVALID) && (stAudioDecStatus->bConnect == TRUE))
    {
        printf("_PVRPL_Audio_CloseDecSys \n");
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

    bret = MApi_AUDIO_GetDecodeSystem(AudioInfo->eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    stDecStatus.eAudFormat = eCodecType;
    bret = MApi_AUDIO_SetDecodeSystem(AudioInfo->eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_SetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL _PVRPL_Audio_STC_Switch(PVRPL_AUDIO_DEC_INFO *AudioInfo, AUDIO_STC_SOURCE eSTC_Source)
{
    AudioDecStatus_t stDecStatus;
    MS_BOOL bret = FALSE;

    bret = MApi_AUDIO_GetDecodeSystem(AudioInfo->eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    stDecStatus.eStcSource = eSTC_Source;
    bret = MApi_AUDIO_SetDecodeSystem(AudioInfo->eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_SetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

#endif
