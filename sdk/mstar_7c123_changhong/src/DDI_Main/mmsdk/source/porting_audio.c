#include <assert.h>
#include <string.h>
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"

#include "MsOS.h"
#include "porting_audio.h"
#include "porting_database.h"
#include "porting_sysinfo.h"
#include "porting_os.h"
/*#include "mapi_audio.h"
#include "mapi_interface.h"
#include "MSrv_System_Database.h"
#include "MSrv_Control.h"
#include "MSrv_SSSound.h"*/

//may not suitable for the following header file
#include "Board.h"

#define PT_AUDIO_ERR(fmt, arg...)   PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_ERR, "\033[1;33m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);
#define PT_AUDIO_DBG(fmt, arg...)   PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_DBG, "\033[1;33m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);

/*
#if defined (MS_DEBUG)
  #define MS_DEBUG_MSG(x)       x
#else
  #define MS_DEBUG_MSG(x)               // retail version remove debug message
#endif
*/
#ifndef ASSERT
#define ASSERT(x)  assert(x)
#endif

/// set single frame mode to true that indicate start multi frame mode
#define SET_AAC_MULTI_FRAME(bIsAACMultiFrame) _MApi_AUDIO_SetAACInfo(Audio_AAC_mm_single_frame_mode, bIsAACMultiFrame, 0);

typedef struct
{
    EN_MMSDK_AUDIO_CODEC eCodecType;
    MMSDK_BOOL bAutoSyncSTC;
}_ST_MMSDK_AUDIO_INSTANCE;

static MS_U8 g_u8AudioRefCount = 0;
static MMSDK_U32 g_u32AudioBaseBufferAddress = 0;
static MMSDK_BOOL g_bISNewAudioMode = FALSE;
static MMSDK_BOOL g_bIsStopDecode = TRUE;
extern PT_SYS_CUS_FunPtr gstSysCusFunPtr;

typedef enum
{
    PT_AUD_INFO_SPDIF_TYPE = 0,
    PT_AUD_INFO_SND_MODE,
    PT_AUD_INFO_VOL_LEVEL,
    PT_AUD_INFO_MOV_SD_MUTE,

    PT_AUD_INFO_ALL
}ePT_AudInfo;

//audio interface translation
AUDIO_DEC_ID _gADECId = AU_DEC_INVALID;

//static void _MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode)
//{
    //not in used
//}

static void _MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        AudioDecStatus_t stAudDecStatus;

        MApi_AUDIO_SetAudioParam2(_gADECId , Audio_ParamType_playControl, MMA_STOP);

        if(MApi_AUDIO_GetDecodeSystem(_gADECId,  &stAudDecStatus) == FALSE)
        {
            PT_AUDIO_ERR("[ERR] MApi_AUDIO_GetDecodeSystem FAIL");
        }

        stAudDecStatus.eAudFormat = enDecSystem;
        if(MApi_AUDIO_SetDecodeSystem(_gADECId, &stAudDecStatus) == FALSE)
        {
            PT_AUDIO_ERR("[ERR] MApi_AUDIO_SetDecodeSystem FAIL");
        }

    }
    else
    {
        MApi_AUDIO_Init(enDecSystem);
    }
}

static void _MApi_AUDIO_ADSetMute(MS_BOOL EnMute)
{
    MApi_AUDIO_ADSetMute(EnMute);
}

static void _MApi_AUDIO_SetADOutputMode(MS_U8 out_mode)
{
    MApi_AUDIO_SetADOutputMode(out_mode);
}

static void _MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetDecodeCmd(_gADECId, enDecComamnd);
    }
    else
    {
        MApi_AUDIO_SetCommand(enDecComamnd);
    }
}

static MS_BOOL _MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    MS_BOOL bRet = FALSE;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        switch(infoType)
        {
            // Please add Audio_COMM_infoType case and its corresponding Audio_ParamType
            case Audio_Comm_infoType_MMFileSize:
                bRet = MApi_AUDIO_SetAudioParam2(_gADECId, Audio_ParamType_reqDataSize, param1);
                break;
            default:
                bRet = MApi_AUDIO_SetCommAudioInfo(infoType, param1, param2);
                break;
        }
    }
    else
    {
        MApi_AUDIO_SetCommAudioInfo(infoType, param1, param2);
    }

    return bRet;
}

static long long _MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
{
    return MApi_AUDIO_GetCommAudioInfo(infoType);
}

static MS_BOOL _MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    MS_BOOL bRet = FALSE;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        switch(infoType)
        {
            // Please add Audio_AAC_infoType case and its corresponding Audio_ParamType
//            case Audio_AAC_infoType_MMFileSize:
//                bRet = MApi_AUDIO_SetAudioParam2(_gADECId, Audio_ParamType_reqDataSize, param1);
//                break;
            default:
                bRet = MApi_AUDIO_SetAACInfo(infoType, param1, param2);
                break;
        }
    }
    else
    {
        MApi_AUDIO_SetAACInfo(infoType, param1, param2);
    }

    return bRet;
}


static MS_BOOL _MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    return MApi_AUDIO_SetDTSCommonCtrl(infoType, ctrl_type);
}

static MS_U8 _MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                                MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                                MS_U16* FrameSize)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        Audio_COOK_Param stAudCoolParam={};
        MS_U32 u32Size;

        stAudCoolParam.mNumCodecs = mNumCodecs;
        stAudCoolParam.mSamples = mSamples;
        stAudCoolParam.mSampleRate = mSampleRate;

        u32Size = sizeof(MS_U16)*mNumCodecs;
        memcpy((void *)stAudCoolParam.Channels, (void *)Channels, u32Size);
        memcpy((void *)stAudCoolParam.Regions, (void *)Regions, u32Size);
        memcpy((void *)stAudCoolParam.cplStart, (void *)cplStart, u32Size);
        memcpy((void *)stAudCoolParam.cplQbits, (void *)cplQbits, u32Size);
        memcpy((void *)stAudCoolParam.FrameSize, (void *)FrameSize, u32Size);

        return MApi_AUDIO_SetAudioParam2(_gADECId, Audio_ParamType_setCOOKDecParam, (MS_U32)&stAudCoolParam);
    }
    else
    {
        return MApi_AUDIO_RA8_Param(mNumCodecs, mSamples, mSampleRate, Channels, Regions, cplStart, cplQbits, FrameSize);
    }
    return 0;
}

static MS_BOOL _MApi_AUDIO_SetASFParm(EN_MMSDK_AUDIO_CODEC eCodecType, Audio_ASF_Param *param) //not finish
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        return MApi_AUDIO_SetAudioParam2(_gADECId, Audio_ParamType_setWMADecParam, (MS_U32)param);
    }
    else
    {
        switch(eCodecType)
        {
            case E_MMSDK_AUDIO_CODEC_WMA:
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_PARSINGBYAPP, param->u32ParsingByApp);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_VERSION, param->u32Version);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_CHANNELS, param->u32Channels);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_SAMPLERATE, param->u32SampleRate);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_BYTERATE, param->u32ByteRate);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_BLOCKALIGN, param->u32BlockAlign);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_ENCOPT, param->u32Encopt);
                break;
            case E_MMSDK_AUDIO_CODEC_WMA_PRO:
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_PARSINGBYAPP, param->u32ParsingByApp);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_VERSION, param->u32Version);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_CHANNELS, param->u32Channels);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_SAMPLERATE, param->u32SampleRate);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_BYTERATE, param->u32ByteRate);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_BLOCKALIGN, param->u32BlockAlign);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_ENCOPT, param->u32Encopt);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_BITS_PER_SAMPLE, param->u32BitsPerSample);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_CHANNELMASK, param->u32ChannelMask);
                MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_DRC_PARAM_EXIST, param->u32DrcParamExist);

                if(param->u32DrcParamExist)
                {
                    MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_DRC_PEAK_AMP_REF, param->u32DrcPeakAmpRef);
                    MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_DRC_PEAK_AMP_TARGET, param->u32DrcPeakAmpTarget);
                    MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_DRC_RMS_AMP_REF, param->u32DrcRmsAmpRef);
                    MApi_AUDIO_SetASFParm(WMA_PARAMTYPE_DRC_RMS_AMP_TARGET, param->u32DrcRmsAmpTarget);
                }
                break;
            default:
                return FALSE;
                break;
        }

        return TRUE;
    }
    return FALSE;
}

static void _MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        Audio_XPCM_Param stAudXPCMParam;

        stAudXPCMParam.audioType = audioType;
        stAudXPCMParam.channels = channels;
        stAudXPCMParam.sampleRate = sampleRate;
        stAudXPCMParam.bitsPerSample = bitsPerSample;
        stAudXPCMParam.blockSize = blockSize;
        stAudXPCMParam.samplePerBlock = samplePerBlock;

        MApi_AUDIO_SetAudioParam2(_gADECId, Audio_ParamType_setXPCMDecParam, (MS_U32)&stAudXPCMParam);
    }
    else
    {
        MApi_AUDIO_SetXPCMParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
    }
}

static void _MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute)
{
    MApi_AUDIO_SetMute(u8Path, EnMute);
}

static void _MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
{
    MApi_AUDIO_HDMI_Tx_SetMute(mute_en);
}

static MS_U32 _MApi_AUDIO_GetPlayTick(void)
{
    MS_U32 u32TimeStamp = 0;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_GetAudioInfo2(_gADECId, Audio_infoType_1ms_timeStamp, (void *)&u32TimeStamp);
    }
    else
    {
        u32TimeStamp = MApi_AUDIO_GetPlayTick();
    }
    return u32TimeStamp;
}

static void _MApi_AUDIO_StopDecode(void)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetDecodeCmd(_gADECId, MSAPI_AUD_DVB_DECCMD_STOP);
    }
    else
    {
        MApi_AUDIO_StopDecode();
    }
}

static void _MApi_AUDIO_PauseDecode(void)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetDecodeCmd(_gADECId, MSAPI_AUD_DVB_DECCMD_PAUSE);
    }
    else
    {
        MApi_AUDIO_PauseDecode();
    }
}

static MS_U8 _MApi_AUDIO_CheckPlayDone(void)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MS_U32 u32Level = 0;
        MS_U32 u32OkFrmCnt = 0;
        MS_U32 u32ErrFrmCnt = 0;

        if (MApi_AUDIO_GetAudioInfo2(_gADECId, Audio_infoType_pcmBuf_currLevel, &u32Level) &&
            MApi_AUDIO_GetAudioInfo2(_gADECId, Audio_infoType_okFrmCnt, &u32OkFrmCnt) &&
            MApi_AUDIO_GetAudioInfo2(_gADECId, Audio_infoType_errFrmCnt, &u32ErrFrmCnt))
        {
            if((u32OkFrmCnt != 0 || u32ErrFrmCnt != 0) && u32Level == 0)
            {
                MApi_AUDIO_SetDecodeCmd(_gADECId, MSAPI_AUD_DVB_DECCMD_STOP);
                return TRUE;
            }
        }
        else
        {
            PT_AUDIO_ERR("[ERR] Get Audio Info failed!\n");
        }
    }
    else
    {
        return MApi_AUDIO_CheckPlayDone();
    }
    return FALSE;
}

static void _MApi_AUDIO_SetInput(void)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetAudioParam2(_gADECId, Audio_ParamType_esBuf_informDataRdy, 0);
    }
    else
    {
        MApi_AUDIO_SetInput();
    }
}

static MS_U8 _MApi_AUDIO_CheckInputRequest(MS_PHY *phyWrtAddr, MS_U32 *pU32WrtBytes)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if((phyWrtAddr == NULL) || (pU32WrtBytes == NULL))
        {
            return FALSE;
        }

        Audio_MM_Data_Request st_Data_Request;

        if (MApi_AUDIO_GetAudioInfo2(_gADECId, Audio_infoType_esBuf_req, &st_Data_Request))
        {
            *phyWrtAddr = st_Data_Request.U32WrtAddr;
            *pU32WrtBytes = st_Data_Request.U32WrtBytes;

            return TRUE;
        }
    }
    else
    {
        return MApi_AUDIO_CheckInputRequest(phyWrtAddr, pU32WrtBytes);
    }

    return FALSE;
}

static void _MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        //this function do nothing
    }
    else
    {
        MApi_AUDIO_FileEndDataHandle(u32DataLeft);
    }
}

static MMSDK_BOOL _PT_CheckAudioCapability(const EN_MMSDK_AUDIO_CODEC eCodecType)
{
    MMA_AudioType AudioType = Audio_DEC_NULL;
    MMA_AudioType AudioCapability = (MMA_AudioType)_MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_capability);

    switch(eCodecType)
    {
        case E_MMSDK_AUDIO_CODEC_AAC:
            AudioType = (Audio_DEC_AAC_LC | Audio_DEC_HEAAC_V1 | Audio_DEC_HEAAC_V2);   // GAAC
            AudioType |= Audio_DEC_DOLBYPULSE;  // MS10
            break;
        case E_MMSDK_AUDIO_CODEC_AC3:
            AudioType = Audio_DEC_AC3;
            break;
        case E_MMSDK_AUDIO_CODEC_AC3P:
            AudioType = Audio_DEC_AC3P;
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
            AudioType = Audio_DEC_AMR_NB;
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
            AudioType = Audio_DEC_AMR_WB;
            break;
        case E_MMSDK_AUDIO_CODEC_DTS:
            AudioType = Audio_DEC_DTS;
            break;
        case E_MMSDK_AUDIO_CODEC_DTSLBR:
            AudioType = Audio_DEC_DTSLBR;
            break;
        case E_MMSDK_AUDIO_CODEC_FLAC:
            AudioType = Audio_DEC_FLAC;
            break;
        case E_MMSDK_AUDIO_CODEC_MP3:
            AudioType = Audio_DEC_MP3;
            break;
        case E_MMSDK_AUDIO_CODEC_MPEG:
            AudioType = Audio_DEC_MPEG;
            break;
        case E_MMSDK_AUDIO_CODEC_RA8LBR:
            AudioType = Audio_DEC_RA8;
            break;
        case E_MMSDK_AUDIO_CODEC_VORBIS:
            AudioType = Audio_DEC_VORBIS;
            break;
        case E_MMSDK_AUDIO_CODEC_WMA:
            AudioType = Audio_DEC_WMA;
            break;
        case E_MMSDK_AUDIO_CODEC_WMA_PRO:
            AudioType = Audio_DEC_WMAPRO;
            break;
        case E_MMSDK_AUDIO_CODEC_XPCM: //AUDIO_CODEC_WAV
            AudioType = Audio_DEC_XPCM | Audio_DEC_PCM;
            break;
        case E_MMSDK_AUDIO_CODEC_DRA:
            AudioType = Audio_DEC_DRA;
            break;
        default :
            AudioType = Audio_DEC_NULL;
            break;
    }

    if ((AudioCapability & AudioType) != 0)
    {
        return TRUE;
    }
    return FALSE;
}


static MS_BOOL _PT_AudGetInfo(ePT_AudInfo eCmd, void *pvVal, MS_U32 u32Len)
{
    if(pvVal == NULL || u32Len == 0)
    {
        return FALSE;
    }

    if(eCmd == PT_AUD_INFO_MOV_SD_MUTE)
    {
        if(u32Len<sizeof(MMSDK_BOOL))
        {
            return FALSE;
        }
#ifdef K3_MMSDK_PORTING
        ST_MMSDK_DIVXDRM_INFO stDRMInfo={};

        if(PT_GetDivXDRMSetting(&stDRMInfo) == FALSE)
        {
            return FALSE;
        }

        if((stDRMInfo.eOutputProtectFlg & E_MMSDK_DRM_OP_ACP_AGC) ||
           (stDRMInfo.eOutputProtectFlg & E_MMSDK_DRM_OP_ACP_AGC_2LINE_COLOR) ||
           (stDRMInfo.eOutputProtectFlg & E_MMSDK_DRM_OP_ACP_AGC_4LINE_COLOR))
        {
            *((MS_BOOL *)pvVal) = TRUE;
        }
        else
        {
            *((MS_BOOL *)pvVal) = FALSE;
        }
#else
        *((MS_BOOL *)pvVal) = FALSE;
#endif

    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*#if (PIP_ENABLE == 1)
extern MAPI_SCALER_WIN g_ScalerWin;
extern MAPI_DECODER_PATH g_DecoderPath;
#endif
*/

/*
static En_DVB_decSystemType_ _GetAudioDecoderType(EN_MMSDK_AUDIO_CODEC eCodecType)
{
    En_DVB_decSystemType_ eDecoderType = MSAPI_AUD_DVB_NONE_;

    switch(eCodecType)
    {
        case E_MMSDK_AUDIO_CODEC_AAC:
            eDecoderType = MSAPI_AUD_DVB_AAC_;
            break;
        case E_MMSDK_AUDIO_CODEC_AC3:
            eDecoderType = MSAPI_AUD_DVB_AC3_;
            break;
        case E_MMSDK_AUDIO_CODEC_AC3P:
            eDecoderType = MSAPI_AUD_DVB_AC3P_;
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
            eDecoderType = MSAPI_AUD_DVB_AMR_NB_;
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
            eDecoderType = MSAPI_AUD_DVB_AMR_WB_;
            break;
        case E_MMSDK_AUDIO_CODEC_DTS:
            eDecoderType = MSAPI_AUD_DVB_DTS_;
            break;
        case E_MMSDK_AUDIO_CODEC_FLAC:
            eDecoderType = MSAPI_AUD_DVB_FLAC_;
            break;
        case E_MMSDK_AUDIO_CODEC_MP3:
            eDecoderType = MSAPI_AUD_DVB_MP3_;
            break;
        case E_MMSDK_AUDIO_CODEC_MPEG:
            eDecoderType = MSAPI_AUD_DVB_MPEG_;
            break;
        case E_MMSDK_AUDIO_CODEC_RA8LBR:
            eDecoderType = MSAPI_AUD_DVB_RA8LBR_;
            break;
        case E_MMSDK_AUDIO_CODEC_VORBIS:
            eDecoderType = MSAPI_AUD_DVB_VORBIS_;
            break;
        case E_MMSDK_AUDIO_CODEC_WMA:
            eDecoderType = MSAPI_AUD_DVB_WMA_;
            break;
        case E_MMSDK_AUDIO_CODEC_WMA_PRO:
            eDecoderType = MSAPI_AUD_DVB_WMA_PRO_;
            break;
        case E_MMSDK_AUDIO_CODEC_XPCM: //AUDIO_CODEC_WAV
            eDecoderType = MSAPI_AUD_DVB_XPCM_;
            break;
        default :
            eDecoderType = MSAPI_AUD_DVB_NONE_;
            break;
    }
    return eDecoderType;
}

static BOOL _setSPDIFMode()
{
    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    if ( pAudio == NULL)
    {
        return FALSE;
    }

    MS_USER_SYSTEM_SETTING stToGetSystemSetting;
    SPDIF_TYPE_  enSPDIF_Mode=MSAPI_AUD_SPDIF_PCM_;
    MSrv_Control::GetMSrvSystemDatabase()->GetUserSystemSetting(&stToGetSystemSetting);
    enSPDIF_Mode = stToGetSystemSetting.enSPDIFMODE;
    pAudio->SPDIF_SetMode(enSPDIF_Mode);
    return TRUE;
}
static BOOL _setOutputMode()
{
    MS_USER_SOUND_SETTING stSoundSetting;

    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();

    if ( pAudio == NULL)
    {
        return FALSE;
    }

    MSrv_Control::GetMSrvSystemDatabase()->GetAudio(&stSoundSetting);
    pAudio->DECODER_SetOutputMode((En_DVB_soundModeType_) stSoundSetting.enSoundAudioChannel);
    return TRUE;
    return 0;
}

*/

static En_DVB_decSystemType _GetAudioDecoderType(EN_MMSDK_AUDIO_CODEC eCodecType)
{
    En_DVB_decSystemType eDecoderType = MSAPI_AUD_DVB_NONE;

    switch(eCodecType)
    {
        case E_MMSDK_AUDIO_CODEC_AAC:
            eDecoderType = MSAPI_AUD_DVB_AAC;
            break;
        case E_MMSDK_AUDIO_CODEC_AC3:
            eDecoderType = MSAPI_AUD_DVB_AC3;
            break;
        case E_MMSDK_AUDIO_CODEC_AC3P:
            eDecoderType = MSAPI_AUD_DVB_AC3P;
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
            eDecoderType = MSAPI_AUD_DVB_AMR_NB;
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
            eDecoderType = MSAPI_AUD_DVB_AMR_WB;
            break;
        case E_MMSDK_AUDIO_CODEC_DTS:
            eDecoderType = MSAPI_AUD_DVB_DTS;
            break;
        case E_MMSDK_AUDIO_CODEC_FLAC:
            eDecoderType = MSAPI_AUD_DVB_FLAC;
            break;
        case E_MMSDK_AUDIO_CODEC_MP3:
            eDecoderType = MSAPI_AUD_DVB_MP3;
            break;
        case E_MMSDK_AUDIO_CODEC_MPEG:
            eDecoderType = MSAPI_AUD_DVB_MPEG;
            break;
        case E_MMSDK_AUDIO_CODEC_RA8LBR:
            eDecoderType = MSAPI_AUD_DVB_RA8LBR;
            break;
        case E_MMSDK_AUDIO_CODEC_VORBIS:
            eDecoderType = MSAPI_AUD_DVB_VORBIS;
            break;
        case E_MMSDK_AUDIO_CODEC_WMA:
            eDecoderType = MSAPI_AUD_DVB_WMA;
            break;
        case E_MMSDK_AUDIO_CODEC_WMA_PRO:
            eDecoderType = MSAPI_AUD_DVB_WMA_PRO;
            break;
        case E_MMSDK_AUDIO_CODEC_XPCM: //AUDIO_CODEC_WAV
            eDecoderType = MSAPI_AUD_DVB_XPCM;
            break;
        case E_MMSDK_AUDIO_CODEC_DTSLBR:
            eDecoderType = MSAPI_AUD_DVB_DTSLBR;
            break;
        case E_MMSDK_AUDIO_CODEC_DRA:
            eDecoderType = MSAPI_AUD_DVB_DRA;
            break;
        default :
            eDecoderType = MSAPI_AUD_DVB_NONE;
            break;
    }
    return eDecoderType;
}

/*
static MMSDK_BOOL _setSPDIFMode(void)
{
    AUDIO_SPDIF_OUTPUT_TYPE enSPDIF_Mode = SPDIF_OUT_PCM;

    if(_PT_AudGetInfo(PT_AUD_INFO_SPDIF_TYPE, (void *)&enSPDIF_Mode, sizeof(AUDIO_SPDIF_OUTPUT_TYPE)) == FALSE)
    {
        return FALSE;
    }

    if (enSPDIF_Mode == SPDIF_OUT_PCM)
    {
        enSPDIF_Mode = SPDIF_OUT_PCM;
    }
    else
    {
        enSPDIF_Mode = SPDIF_OUT_NONPCM;
    }

    _MApi_AUDIO_SPDIF_SetMode(enSPDIF_Mode);
    return TRUE;
}
*/

static void _Music_Init(const En_DVB_decSystemType enDecSystem)
{
    MMSDK_BOOL  bMusicInit = FALSE;
    En_DVB_decSystemType enDriverDecSystem = MSAPI_AUD_DVB_INVALID;

    //m_mmAudioDSPSystem = enDecSystem;

    switch(enDecSystem)
    {
        case MSAPI_AUD_DVB_MPEG:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_MPEG;
            break;
        case MSAPI_AUD_DVB_AC3:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_AC3;
            break;
        case MSAPI_AUD_DVB_AC3P:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_AC3P;
            break;
        case MSAPI_AUD_DVB_AAC:
            {
                bMusicInit = true;
                MMA_AudioType AudioCapability = (MMA_AudioType)_MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_capability);
                enDriverDecSystem = MSAPI_AUD_DVB_AAC;
                if ((AudioCapability & (Audio_DEC_AAC_LC | Audio_DEC_HEAAC_V1 | Audio_DEC_HEAAC_V2)) == 0) // Not support GAAC
                {
                    enDriverDecSystem = MSAPI_AUD_DVB_MS10_DDT; // MS10 AAC
                }
            }
            break;
        case MSAPI_AUD_DVB_MP3:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_MP3;
            break;
        case MSAPI_AUD_DVB_WMA:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_WMA;
            break;
        case MSAPI_AUD_DVB_RA8LBR:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_RA8LBR;
            break;
        case MSAPI_AUD_DVB_XPCM:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_XPCM;
            break;
        case MSAPI_AUD_DVB_TONE:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_TONE;
            break;
        case MSAPI_AUD_DVB_DTS:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_DTS;
            break;
        case MSAPI_AUD_DVB_MS10_DDT:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_MS10_DDT;
            break;
        case MSAPI_AUD_DVB_MS10_DDC:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_MS10_DDC;
            break;
        case MSAPI_AUD_DVB_WMA_PRO:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_WMA_PRO;
            break;
        case MSAPI_AUD_DVB_FLAC:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_FLAC;
            break;
        case MSAPI_AUD_DVB_VORBIS:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_VORBIS;
            break;
        case MSAPI_AUD_DVB_DTSLBR:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_DTSLBR;
            break;
        case MSAPI_AUD_DVB_AMR_NB:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_AMR_NB;
            break;
        case MSAPI_AUD_DVB_AMR_WB:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_AMR_WB;
            break;
        case MSAPI_AUD_DVB_DRA:
            bMusicInit = TRUE;
            enDriverDecSystem = MSAPI_AUD_DVB_DRA;
            break;
        default:
            break;
    }

    if(bMusicInit == TRUE)
    {
        _MApi_AUDIO_Init(enDriverDecSystem);
    }
    else
    {
        PT_AUDIO_ERR("[%s][%d] Assert fail here!!!!\n", __FUNCTION__, __LINE__);
        ASSERT(0);
    }
}

static MMSDK_U32 _GetAudioBaseAddress(void)
{
    MMSDK_U32 u32Ret = 0;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 0)
    {
        _gADECId = AU_DEC_ID1;
    }

    u32Ret = (MMSDK_U32)(MApi_AUDIO_GetDDRInfo(_gADECId, E_AUD_MEMORY_BASE));

    u32Ret = MsOS_PA2KSEG1(u32Ret);

    return u32Ret;
}

static void PT_Audio_InitializeAudioPath(void)
{
/*    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    if(g_u8AudioRefCount > 0)
    {
        g_u8AudioRefCount++;
        return;
    }

#if (PIP_ENABLE == 1)
    if (g_ScalerWin==MAPI_MAIN_WINDOW)
    {
        if(MSrv_Control::GetInstance()->GetCurrentMainInputSource() == MAPI_INPUT_SOURCE_KTV)
    {
            pAudio->InputSource_ChangeAudioSource(MAPI_INPUT_SOURCE_KTV, AUDIO_PROCESSOR_MAIN);
    }
    else
    {
        pAudio->InputSource_ChangeAudioSource(MAPI_INPUT_SOURCE_STORAGE, AUDIO_PROCESSOR_MAIN);
        }
    }
    else
    {
        if(MSrv_Control::GetInstance()->GetCurrentSubInputSource() == MAPI_INPUT_SOURCE_KTV)
        {
            pAudio->InputSource_ChangeAudioSource(MAPI_INPUT_SOURCE_KTV, AUDIO_PROCESSOR_SUB);
        }
        else
        {
            pAudio->InputSource_ChangeAudioSource(MAPI_INPUT_SOURCE_STORAGE, AUDIO_PROCESSOR_SUB);
        }
    }
#else
    pAudio->InputSource_ChangeAudioSource(MAPI_INPUT_SOURCE_STORAGE);
#endif

#if 0//MULTI_AUDIO_CODEC
    pAudio->MMA_Init();
#endif
    g_u8AudioRefCount++;*/

    if(g_u8AudioRefCount > 0)
    {
        g_u8AudioRefCount++;
        return;
    }

    //MApi_SND_ProcessEnable(Sound_ENABL_Type_KTVEcho, FALSE);//no need, suggested by vincent.tsai

    //MApi_AUDIO_SPDIF_ChannelStatus_CTRL(SPDIF_CS_CategoryCode, SPDIF_CS_Category_General);//no need, suggested by vincent.tsai
    _MApi_AUDIO_ADSetMute(TRUE);
    _MApi_AUDIO_SetADOutputMode(AD_OUT_NONE);

    //MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);//no need, suggesteds by vincent.tsai
    //MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DownmixMode, DOLBY_DOWNMIX_MODE_LTRT, 0);//no need, suggested by vincent.tsai
    g_u8AudioRefCount++;
}

static void PT_Audio_FinalizeAudioPath(void)
{

/*
#if 0//MULTI_AUDIO_CODEC
    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
#endif

#if (PIP_ENABLE == 1)
    if (g_ScalerWin==MAPI_SUB_WINDOW)
    {
        mapi_interface::Get_mapi_audio()->InputSource_ChangeAudioSource(MAPI_INPUT_SOURCE_NONE, AUDIO_PROCESSOR_SUB);
    }
#endif
    if(g_u8AudioRefCount > 0)
    {
        g_u8AudioRefCount--;
    }
    if(g_u8AudioRefCount == 0)
    {
#if 0//MULTI_AUDIO_CODEC
        pAudio->MMA_Exit();
#endif
    }
*/

    if(g_u8AudioRefCount > 0)
    {
        g_u8AudioRefCount--;
    }
    if(g_u8AudioRefCount == 0)
    {
        //according to the SN code, we do nothing here now
    }

}

MMSDK_BOOL PT_Audio_Initialize(PT_AUDIOITEM* pAudioItem)
{
    _ST_MMSDK_AUDIO_INSTANCE* pAudioInstance = NULL;

    if (pAudioItem == NULL)
    {
        PT_AUDIO_ERR("Error: pAudioItem=%p\n" ,pAudioItem);
        return FALSE;
    }

    pAudioInstance = (_ST_MMSDK_AUDIO_INSTANCE*) PT_MsOS_AllocateMemory(sizeof(_ST_MMSDK_AUDIO_INSTANCE));

    if (pAudioInstance !=  NULL)
    {
        *pAudioItem = (void*)pAudioInstance;
        PT_Audio_InitializeAudioPath();

        if (gstSysCusFunPtr.pAudInitSetup != NULL)
        {
            return gstSysCusFunPtr.pAudInitSetup(NULL);
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MMSDK_BOOL PT_Audio_Finalize(PT_AUDIOITEM* pAudioItem)
{
    if ((pAudioItem == NULL) || (*pAudioItem == NULL))
    {
        PT_AUDIO_ERR("Error: pAudioItem=%p\n",pAudioItem);
        return FALSE;
    }

    if (gstSysCusFunPtr.pAudFinalizeSetup != NULL)
    {
        return gstSysCusFunPtr.pAudFinalizeSetup(NULL);
    }


    PT_Audio_FinalizeAudioPath();
    PT_MsOS_FreeMemory(*pAudioItem);
    *pAudioItem = NULL;

    return TRUE;
}

MMSDK_BOOL PT_Audio_ReleaseDecoder(PT_AUDIOITEM AudioItem)
{
    _ST_MMSDK_AUDIO_INSTANCE* pAudioInstance = (_ST_MMSDK_AUDIO_INSTANCE*)AudioItem;

    if (AudioItem == NULL)
    {
        return FALSE;
    }

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        _gADECId = AU_DEC_INVALID;
    }

    // Leave aac multiple frame mode
    if (pAudioInstance->eCodecType == E_MMSDK_AUDIO_CODEC_AAC)
    {
        SET_AAC_MULTI_FRAME(FALSE);
    }

    pAudioInstance->eCodecType = E_MMSDK_AUDIO_CODEC_UNKNOWN;

    g_bISNewAudioMode = FALSE;
    g_bIsStopDecode = TRUE;

    return TRUE;
}

MMSDK_BOOL PT_Audio_StartDecode(PT_AUDIOITEM AudioItem, const ST_MMSDK_AUDIO_START_INFO* pstAudioStart)
{
/*
    PT_AUDIO_DBG("");
    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();

    if (pAudio == NULL)
    {
        return FALSE;
    }
    ST_MMSDK_AUDIO_START_INFO* pstAudioStart = (ST_MMSDK_AUDIO_START_INFO*)pstParamInfo;
    if ( pstAudioStart->bTSAudio )
    {
        pAudio->DECODER_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP_);
    }
    else
    {
        pAudio->StartDecode();
    }

    return TRUE;*/

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 0)
    {
        _gADECId = AU_DEC_ID1;
    }

    _ST_MMSDK_AUDIO_INSTANCE* pAudioInstance = (_ST_MMSDK_AUDIO_INSTANCE*)AudioItem;

    if (AudioItem == NULL)
    {
        return FALSE;
    }

    if (pstAudioStart != NULL)
    {
        if ( pstAudioStart->bTSAudio )
        {
            // TS Audio always single frame
            if (pAudioInstance->eCodecType == E_MMSDK_AUDIO_CODEC_AAC)
            {
                SET_AAC_MULTI_FRAME(FALSE);
            }
            //TS file mode
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
         else if(pstAudioStart->bISNewAudioMode)
        {
            if (TRUE == g_bIsStopDecode)
            {
                // Reset audio es pointer
                MApi_AUDIO_MM2_initAesInfo(_gADECId);
                g_bIsStopDecode = FALSE;
            }

            g_bISNewAudioMode = TRUE;
            g_u32AudioBaseBufferAddress = _GetAudioBaseAddress();

            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        else
        {
            //ES file mode
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }

    }
    else
    {
        //ES file mode
        _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
        _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
    }

    return TRUE;
}
MMSDK_BOOL PT_Audio_AllocateDecoder(PT_AUDIOITEM AudioItem, const EN_MMSDK_AUDIO_CODEC eCodecType)
{
/*    PT_AUDIO_DBG("");
    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();

    if ( pAudio == NULL)
    {
        return FALSE;
    }
    _setOutputMode();
    PT_Audio_Mute(TRUE);
    pAudio->Music_Init(_GetAudioDecoderType(eCodecType));
    PT_Audio_Mute(FALSE);
    _setSPDIFMode();
    pAudio->DECODER_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN_);
    //clear DSP residual bytes when init
    pAudio->FileEndDataHandle(0);
    MSrv_Control::GetMSrvSSSound()->SetAbsoluteVolume();

    return TRUE;*/

    _ST_MMSDK_AUDIO_INSTANCE* pAudioInstance = (_ST_MMSDK_AUDIO_INSTANCE*)AudioItem;

    if (AudioItem == NULL)
    {
        return FALSE;
    }

    if (_PT_CheckAudioCapability(eCodecType) == FALSE)
    {
        PT_AUDIO_DBG("Not support codec %d\n", eCodecType);
        return FALSE;
    }

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if((PT_SYS_GetAudDecID(&_gADECId) == FALSE) || (_gADECId == AU_DEC_INVALID))
        {
            PT_AUDIO_DBG("Get audio decoder ID fail.\n");
            return FALSE;
        }
    }

    pAudioInstance->eCodecType = eCodecType;

    PT_Audio_Mute(AudioItem, TRUE);
    _Music_Init(_GetAudioDecoderType(eCodecType));

    PT_Audio_Mute(AudioItem, FALSE);
    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);

    //clear DSP residual bytes when init
    PT_Audio_FileEndDataHandle(AudioItem, 0);

    g_bISNewAudioMode = FALSE;
    g_bIsStopDecode = TRUE;

    return TRUE;

}

MMSDK_BOOL PT_Audio_AllocateDecoderEx(PT_AUDIOITEM AudioItem, const EN_MMSDK_AUDIO_CODEC eCodecType)
{
    if (AudioItem == NULL)
    {
        return FALSE;
    }
    return PT_Audio_AllocateDecoder(AudioItem, eCodecType);
}

MMSDK_BOOL PT_Audio_SetParam(PT_AUDIOITEM AudioItem, const EN_MMSDK_AUDIO_CODEC eCodecType, const void* pstParamInfo)
{
/*    PT_AUDIO_DBG("");

    ST_MMSDK_AUDIO_WMA_INIT_INFO* pstWMAInfo = (ST_MMSDK_AUDIO_WMA_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_MP3_INIT_INFO* pstMP3Info = (ST_MMSDK_AUDIO_MP3_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_MP2_INIT_INFO* pstMP2Info = (ST_MMSDK_AUDIO_MP2_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_AC3_EAC3_INIT_INFO* pstAC3Info = (ST_MMSDK_AUDIO_AC3_EAC3_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_AAC_INIT_INFO* pstAACInfo = (ST_MMSDK_AUDIO_AAC_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_PCM_INIT_INFO* pstPCMInfo = (ST_MMSDK_AUDIO_PCM_INIT_INFO* )pstParamInfo;
    //ST_MMSDK_AUDIO_RAAC_RACP_INIT_INFO* pstRAACInfo = (ST_MMSDK_AUDIO_RAAC_RACP_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_COOK_INIT_INFO* pstCOOkInfo = (ST_MMSDK_AUDIO_COOK_INIT_INFO* )pstParamInfo;
    MMSDK_BOOL* p_bDTSIsLittleEndian = (MMSDK_BOOL* )pstParamInfo;

    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    if ( pAudio == NULL)
    {
        return FALSE;
    }
    switch(eCodecType)
    {
        case E_MMSDK_AUDIO_CODEC_AAC:
            if(pstAACInfo->u32RequestSize == 0x400)
            {
                pAudio->DECODER_SetAACInfo(Audio_AAC_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
            }
            else if(pstAACInfo->u32RequestSize == 0x800)
            {
                pAudio->DECODER_SetAACInfo(Audio_AAC_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_2KB_, 0);
            }
            else
            {
                pAudio->DECODER_SetAACInfo(Audio_AAC_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_4KB_, 0);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_AC3:
            break;
        case E_MMSDK_AUDIO_CODEC_AC3P:
            if(pstAC3Info->u32RequestSize == 0x4000)
            {
                pAudio->StopDecode();
                pAudio->DECODER_SetAC3PInfo(Audio_AC3P_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_16KB_, 0);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
            break;
        case E_MMSDK_AUDIO_CODEC_DTS:
            if(*p_bDTSIsLittleEndian)
            {
                pAudio->DECODER_SetDTSInfo(Audio_DTS_infoType_ESData_ByteSwap_, Audio_DTS_Ctrl_ByteSwap_ON_);
            }
            else
            {
                pAudio->DECODER_SetDTSInfo(Audio_DTS_infoType_ESData_ByteSwap_ , Audio_DTS_Ctrl_ByteSwap_OFF_);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_FLAC:
            break;
        case E_MMSDK_AUDIO_CODEC_MP3:
            if(pstMP3Info->u32RequestSize == 0x400)
            {
                pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
            }
            else if(pstMP3Info->u32RequestSize == 0x800)
            {
                pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_2KB_, 0);
            }
            else
            {
                pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_4KB_, 0);
            }

            break;
        case E_MMSDK_AUDIO_CODEC_MPEG:
            if(pstMP2Info->u32RequestSize == 0x400)
            {
                pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
            }
            else if(pstMP2Info->u32RequestSize == 0x800)
            {
                pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_2KB_, 0);
            }
            else
            {
                pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_4KB_, 0);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_RA8LBR:
            pAudio->DECODER_SetRA8Info(pstCOOkInfo->u16CodecNum, pstCOOkInfo->u16SamplesChannel, pstCOOkInfo->u16SampleRate, pstCOOkInfo->u16ChannelsA, pstCOOkInfo->u16RegionsA, pstCOOkInfo->u16cplStartA, pstCOOkInfo->u16cplQbitsA, pstCOOkInfo->u16FrameSizeA);
            //pAudio->WriteDecMailBox(5, 0x00);
            break;
        case E_MMSDK_AUDIO_CODEC_VORBIS:
            break;
        case E_MMSDK_AUDIO_CODEC_WMA:
            if ( pstWMAInfo->u32RequestSize == 0x400 )
            {
                pAudio->DECODER_SetWmaInfo(Audio_WMA_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
            }
            pAudio->SetASFParm(WMA_PARAMTYPE_PARSINGBYAPP_, pstWMAInfo->u32ParsingByApp);
            pAudio->SetASFParm(WMA_PARAMTYPE_VERSION_, pstWMAInfo->u32Version);
            pAudio->SetASFParm(WMA_PARAMTYPE_CHANNELS_, pstWMAInfo->u32Channels);
            pAudio->SetASFParm(WMA_PARAMTYPE_SAMPLERATE_, pstWMAInfo->u32SampleRate);
            pAudio->SetASFParm(WMA_PARAMTYPE_BYTERATE_, pstWMAInfo->u32ByteRate);
            pAudio->SetASFParm(WMA_PARAMTYPE_BLOCKALIGN_, pstWMAInfo->u32BlockAlign);
            pAudio->SetASFParm(WMA_PARAMTYPE_ENCOPT_, pstWMAInfo->u32Encopt);
            break;
        case E_MMSDK_AUDIO_CODEC_WMA_PRO:
            pAudio->SetASFParm(WMA_PARAMTYPE_PARSINGBYAPP_, pstWMAInfo->u32ParsingByApp);
            pAudio->SetASFParm(WMA_PARAMTYPE_VERSION_, pstWMAInfo->u32Version);
            pAudio->SetASFParm(WMA_PARAMTYPE_CHANNELS_, pstWMAInfo->u32Channels);
            pAudio->SetASFParm(WMA_PARAMTYPE_SAMPLERATE_, pstWMAInfo->u32SampleRate);
            pAudio->SetASFParm(WMA_PARAMTYPE_BYTERATE_, pstWMAInfo->u32ByteRate);
            pAudio->SetASFParm(WMA_PARAMTYPE_BLOCKALIGN_, pstWMAInfo->u32BlockAlign);
            pAudio->SetASFParm(WMA_PARAMTYPE_ENCOPT_, pstWMAInfo->u32Encopt);

            pAudio->SetASFParm(WMA_PARAMTYPE_BITS_PER_SAMPLE_, pstWMAInfo->u32BitPerSample);
            pAudio->SetASFParm(WMA_PARAMTYPE_CHANNELMASK_, pstWMAInfo->u32ChannelMask);
            pAudio->SetASFParm(WMA_PARAMTYPE_DRC_PARAM_EXIST_, pstWMAInfo->u32DrcParamExist);
            if(pstWMAInfo->u32DrcParamExist)
            {
                pAudio->SetASFParm(WMA_PARAMTYPE_DRC_PEAK_AMP_REF_, pstWMAInfo->u32DrcPeakAmpRef);
                pAudio->SetASFParm(WMA_PARAMTYPE_DRC_PEAK_AMP_TARGET_, pstWMAInfo->u32DrcPeakAmpTarget);
                pAudio->SetASFParm(WMA_PARAMTYPE_DRC_RMS_AMP_REF_, pstWMAInfo->u32DrcRmsAmpRef);
                pAudio->SetASFParm(WMA_PARAMTYPE_DRC_RMS_AMP_TARGET_, pstWMAInfo->u32DrcRmsAmpTarget);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_XPCM:
        {
            XPCM_TYPE_ ePCMType = LPCM_;
            switch (pstPCMInfo->ePCMType)
            {
                case E_MMSDK_XPCM_TYPE_LPCM:
                    ePCMType = LPCM_;
                    break;
                case E_MMSDK_XPCM_TYPE_MS_ADPCM:
                    ePCMType = MS_ADPCM_;
                    break;
                case E_MMSDK_XPCM_TYPE_LPCM_ALAW:
                    ePCMType = LPCM_ALAW;
                    break;
                case E_MMSDK_XPCM_TYPE_LPCM_MULAW:
                    ePCMType = LPCM_MULAW;
                    break;
                case E_MMSDK_XPCM_TYPE_IMA_ADPCM:
                    ePCMType = IMA_ADPCM_;
                    break;
                default:
                    PT_AUDIO_ERR("[ERR] error PCM codec\n");
                    break;
            }
            pAudio->SetXPCMParam(ePCMType, pstPCMInfo->u8ChannelNum - 1, pstPCMInfo->u16SampleRate, pstPCMInfo->u16BitsPerSample, pstPCMInfo->u16BlockSize, pstPCMInfo->u16SamplePerBlock);
        }
            break;
        default :
            break;
    }
    return TRUE;*/

    ST_MMSDK_AUDIO_WMA_INIT_INFO* pstWMAInfo = (ST_MMSDK_AUDIO_WMA_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_MP3_INIT_INFO* pstMP3Info = (ST_MMSDK_AUDIO_MP3_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_MP2_INIT_INFO* pstMP2Info = (ST_MMSDK_AUDIO_MP2_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_AC3_EAC3_INIT_INFO* pstAC3Info = (ST_MMSDK_AUDIO_AC3_EAC3_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_AAC_INIT_INFO* pstAACInfo = (ST_MMSDK_AUDIO_AAC_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_PCM_INIT_INFO* pstPCMInfo = (ST_MMSDK_AUDIO_PCM_INIT_INFO* )pstParamInfo;
    //ST_MMSDK_AUDIO_RAAC_RACP_INIT_INFO* pstRAACInfo = (ST_MMSDK_AUDIO_RAAC_RACP_INIT_INFO* )pstParamInfo;
    ST_MMSDK_AUDIO_COOK_INIT_INFO* pstCOOkInfo = (ST_MMSDK_AUDIO_COOK_INIT_INFO* )pstParamInfo;

    ST_MMSDK_AUDIO_DTS_INIT_INFO * pstDTSInfo = (ST_MMSDK_AUDIO_DTS_INIT_INFO *) pstParamInfo;

    ST_MMSDK_AUDIO_VORBIS_INIT_INFO* pstVORBIS = (ST_MMSDK_AUDIO_VORBIS_INIT_INFO* )pstParamInfo;


    switch(eCodecType)
    {
        case E_MMSDK_AUDIO_CODEC_AAC:
            if(pstAACInfo->u32RequestSize == 0x400)
            {
                //pAudio->DECODER_SetAACInfo(Audio_AAC_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_1KB, 0);
            }
            else if(pstAACInfo->u32RequestSize == 0x800)
            {
                //pAudio->DECODER_SetAACInfo(Audio_AAC_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_2KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_2KB, 0);
            }
            else if(pstAACInfo->u32RequestSize == 0x1000)
            {
                //pAudio->DECODER_SetAACInfo(Audio_AAC_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_4KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_4KB, 0);
            }

            // MM movie add the multi frame header(except TS audio), music is always single frame(for AAC)
            if (pstAACInfo->bIsSingleFrame == FALSE)
            {
                SET_AAC_MULTI_FRAME(TRUE);
            }
            else
            {
                SET_AAC_MULTI_FRAME(FALSE);
            }


            break;
        case E_MMSDK_AUDIO_CODEC_AC3:
            break;
        case E_MMSDK_AUDIO_CODEC_AC3P:
            if(pstAC3Info->u32RequestSize == 0x4000)
            {
                //pAudio->StopDecode();
                //pAudio->DECODER_SetAC3PInfo(Audio_AC3P_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_16KB_, 0);
                _MApi_AUDIO_StopDecode();
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_16KB, 0);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_NB:
            break;
        case E_MMSDK_AUDIO_CODEC_AMR_WB:
            break;
        case E_MMSDK_AUDIO_CODEC_DTS:
            if(pstDTSInfo->bIsLittleEndian)
            {
                //pAudio->DECODER_SetDTSInfo(Audio_DTS_infoType_ESData_ByteSwap_, Audio_DTS_Ctrl_ByteSwap_ON_);
                _MApi_AUDIO_SetDTSCommonCtrl(Audio_DTS_infoType_ESData_ByteSwap, Audio_DTS_Ctrl_ByteSwap_ON);
            }
            else
            {
                //pAudio->DECODER_SetDTSInfo(Audio_DTS_infoType_ESData_ByteSwap_ , Audio_DTS_Ctrl_ByteSwap_OFF_);
                _MApi_AUDIO_SetDTSCommonCtrl(Audio_DTS_infoType_ESData_ByteSwap, Audio_DTS_Ctrl_ByteSwap_OFF);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_FLAC:
            break;
        case E_MMSDK_AUDIO_CODEC_MP3:
            if(pstMP3Info->u32RequestSize == 0x400)
            {
                //pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_1KB, 0);
            }
            else if(pstMP3Info->u32RequestSize == 0x800)
            {
                //pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_2KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_2KB, 0);
            }
            else
            {
                //pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_4KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_4KB, 0);
            }

            break;
        case E_MMSDK_AUDIO_CODEC_MPEG:
            if(pstMP2Info->u32RequestSize == 0x400)
            {
                //pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_1KB, 0);
            }
            else if(pstMP2Info->u32RequestSize == 0x800)
            {
                //pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_2KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_2KB, 0);
            }
            else
            {
                //pAudio->DECODER_SetMpegInfo(Audio_MPEG_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_4KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_4KB, 0);
            }
            break;
        case E_MMSDK_AUDIO_CODEC_RA8LBR:
            //pAudio->DECODER_SetRA8Info(pstCOOkInfo->u16CodecNum, pstCOOkInfo->u16SamplesChannel, pstCOOkInfo->u16SampleRate, pstCOOkInfo->u16ChannelsA, pstCOOkInfo->u16RegionsA, pstCOOkInfo->u16cplStartA, pstCOOkInfo->u16cplQbitsA, pstCOOkInfo->u16FrameSizeA);
            _MApi_AUDIO_RA8_Param(pstCOOkInfo->u16CodecNum, pstCOOkInfo->u16SamplesChannel, pstCOOkInfo->u16SampleRate, pstCOOkInfo->u16ChannelsA, pstCOOkInfo->u16RegionsA, pstCOOkInfo->u16cplStartA, pstCOOkInfo->u16cplQbitsA, pstCOOkInfo->u16FrameSizeA);

            //pAudio->WriteDecMailBox(5, 0x00);
            break;
        case E_MMSDK_AUDIO_CODEC_VORBIS:
            //pAudio->SetCommAudioInfo(Audio_Comm_infoType_Vorbis_header_size_, 0, pstVORBIS->u32HeaderSize);
            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Vorbis_header_size, 0, pstVORBIS->u32HeaderSize);

            break;
        case E_MMSDK_AUDIO_CODEC_WMA:
        {
            if ( pstWMAInfo->u32RequestSize == 0x400 )
            {
                //pAudio->DECODER_SetWmaInfo(Audio_WMA_infoType_MMFileSize_, MSAPI_AUD_FILE_SIZE_1KB_, 0);
                _MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_MMFileSize, FILE_SIZE_1KB, 0);
            }

            PT_AUDIO_ERR("WMA_PARAMTYPE_PARSINGBYAPP_ :%d\n", (int)pstWMAInfo->u32ParsingByApp);
            PT_AUDIO_ERR("WMA_PARAMTYPE_VERSION_ %d\n", (int)pstWMAInfo->u32Version);
            PT_AUDIO_ERR("WMA_PARAMTYPE_CHANNELS_ :%d", (int)pstWMAInfo->u32Channels);
            PT_AUDIO_ERR("WMA_PARAMTYPE_SAMPLERATE_:%d", (int)pstWMAInfo->u32SampleRate);
            PT_AUDIO_ERR("WMA_PARAMTYPE_BYTERATE_ %d", (int)pstWMAInfo->u32ByteRate);
            PT_AUDIO_ERR("WMA_PARAMTYPE_BLOCKALIGN_ :%d", (int)pstWMAInfo->u32BlockAlign);
            PT_AUDIO_ERR("WMA_PARAMTYPE_ENCOPT_ :%d", (int)pstWMAInfo->u32Encopt);

            Audio_ASF_Param stParam={};

            stParam.u32ParsingByApp = pstWMAInfo->u32ParsingByApp;
            stParam.u32Version = pstWMAInfo->u32Version;
            stParam.u32Channels = pstWMAInfo->u32Channels;
            stParam.u32SampleRate = pstWMAInfo->u32SampleRate;
            stParam.u32ByteRate = pstWMAInfo->u32ByteRate;
            stParam.u32BlockAlign = pstWMAInfo->u32BlockAlign;
            stParam.u32Encopt = pstWMAInfo->u32Encopt;

            _MApi_AUDIO_SetASFParm(E_MMSDK_AUDIO_CODEC_WMA, &stParam);

        }
            break;
        case E_MMSDK_AUDIO_CODEC_WMA_PRO:
        {
            //pAudio->SetASFParm(WMA_PARAMTYPE_PARSINGBYAPP_, pstWMAInfo->u32ParsingByApp);
            //pAudio->SetASFParm(WMA_PARAMTYPE_VERSION_, pstWMAInfo->u32Version);
            //pAudio->SetASFParm(WMA_PARAMTYPE_CHANNELS_, pstWMAInfo->u32Channels);
            //pAudio->SetASFParm(WMA_PARAMTYPE_SAMPLERATE_, pstWMAInfo->u32SampleRate);
            //pAudio->SetASFParm(WMA_PARAMTYPE_BYTERATE_, pstWMAInfo->u32ByteRate);
            //pAudio->SetASFParm(WMA_PARAMTYPE_BLOCKALIGN_, pstWMAInfo->u32BlockAlign);
            //pAudio->SetASFParm(WMA_PARAMTYPE_ENCOPT_, pstWMAInfo->u32Encopt);
            //pAudio->SetASFParm(WMA_PARAMTYPE_BITS_PER_SAMPLE_, pstWMAInfo->u32BitPerSample);
            //pAudio->SetASFParm(WMA_PARAMTYPE_CHANNELMASK_, pstWMAInfo->u32ChannelMask);
            //pAudio->SetASFParm(WMA_PARAMTYPE_DRC_PARAM_EXIST_, pstWMAInfo->u32DrcParamExist);

            Audio_ASF_Param stParam={};

            stParam.u32ParsingByApp = pstWMAInfo->u32ParsingByApp;
            stParam.u32Version = pstWMAInfo->u32Version;
            stParam.u32Channels = pstWMAInfo->u32Channels;
            stParam.u32SampleRate = pstWMAInfo->u32SampleRate;
            stParam.u32ByteRate = pstWMAInfo->u32ByteRate;
            stParam.u32BlockAlign = pstWMAInfo->u32BlockAlign;
            stParam.u32Encopt = pstWMAInfo->u32Encopt;
            stParam.u32BitsPerSample = pstWMAInfo->u32BitPerSample;
            stParam.u32ChannelMask = pstWMAInfo->u32ChannelMask;
            stParam.u32DrcParamExist = pstWMAInfo->u32DrcParamExist;
            if(pstWMAInfo->u32DrcParamExist)
            {
                stParam.u32DrcPeakAmpRef = pstWMAInfo->u32DrcPeakAmpRef;
                stParam.u32DrcPeakAmpTarget = pstWMAInfo->u32DrcPeakAmpTarget;
                stParam.u32DrcRmsAmpRef = pstWMAInfo->u32DrcRmsAmpRef;
                stParam.u32DrcRmsAmpTarget = pstWMAInfo->u32DrcRmsAmpTarget;
            }

            _MApi_AUDIO_SetASFParm(E_MMSDK_AUDIO_CODEC_WMA_PRO, &stParam);

        }
            break;
        case E_MMSDK_AUDIO_CODEC_XPCM:
        {
            EN_MMSDK_XPCM_TYPE ePCMType = E_MMSDK_XPCM_TYPE_LPCM;
            switch (pstPCMInfo->ePCMType)
            {
                case E_MMSDK_XPCM_TYPE_LPCM:
                    ePCMType = E_MMSDK_XPCM_TYPE_LPCM;
                    break;
                case E_MMSDK_XPCM_TYPE_MS_ADPCM:
                    ePCMType = E_MMSDK_XPCM_TYPE_MS_ADPCM;
                    break;
                case E_MMSDK_XPCM_TYPE_LPCM_ALAW:
                    ePCMType = E_MMSDK_XPCM_TYPE_LPCM_ALAW;
                    break;
                case E_MMSDK_XPCM_TYPE_LPCM_MULAW:
                    ePCMType = E_MMSDK_XPCM_TYPE_LPCM_MULAW;
                    break;
                case E_MMSDK_XPCM_TYPE_IMA_ADPCM:
                    ePCMType = E_MMSDK_XPCM_TYPE_IMA_ADPCM;
                    break;
                default:
                    PT_AUDIO_ERR("[ERR] error PCM codec\n");
                    break;
            }
            //pAudio->SetXPCMParam(ePCMType, pstPCMInfo->u8ChannelNum - 1, pstPCMInfo->u16SampleRate, pstPCMInfo->u16BitsPerSample, pstPCMInfo->u16BlockSize, pstPCMInfo->u16SamplePerBlock);
            _MApi_AUDIO_SetXPCMParam(ePCMType, pstPCMInfo->u8ChannelNum - 1, pstPCMInfo->u16SampleRate, pstPCMInfo->u16BitsPerSample, pstPCMInfo->u16BlockSize, pstPCMInfo->u16SamplePerBlock);
        }
            break;
        default :
            break;
    }
    return TRUE;

}


MMSDK_BOOL PT_Audio_Mute(PT_AUDIOITEM AudioItem, const MMSDK_BOOL bMuteOnOff)
{
    MMSDK_BOOL bMovSDMute;

/*    PT_AUDIO_DBG("Mute=%d (0: Off 1: On)\n",bMuteOnOff);

    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    if (pAudio == NULL)
    {
        return FALSE;
    }
    if (bMuteOnOff == TRUE)
    {
        pAudio->SetSoundMuteStatus(E_AUDIO_PERMANENT_MUTEON_, E_AUDIOMUTESOURCE_ACTIVESOURCE_);
    }
    else
    {
        pAudio->SetSoundMuteStatus(E_AUDIO_PERMANENT_MUTEOFF_, E_AUDIOMUTESOURCE_ACTIVESOURCE_);
    }
    return TRUE;*/

    //Line-out (LR)

    if (NULL != gstSysCusFunPtr.pAudMute)
    {
        MMSDK_BOOL bRet = FALSE;
        bRet = gstSysCusFunPtr.pAudMute(bMuteOnOff);
        return bRet;
    }

    if(_PT_AudGetInfo(PT_AUD_INFO_MOV_SD_MUTE, (void *)&bMovSDMute, sizeof(MMSDK_BOOL)) == FALSE)
    {
        return FALSE;
    }

    if ((TRUE == bMuteOnOff) || (TRUE != bMovSDMute))
    {
        _MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, bMuteOnOff);
    }

    //HDMI Tx
    _MApi_AUDIO_HDMI_Tx_SetMute(bMuteOnOff);

    //SPDIF
    _MApi_AUDIO_SetMute(AUDIO_PATH_SPDIF,bMuteOnOff);

    //SCART
    _MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2,bMuteOnOff);

    return TRUE;
}


MMSDK_U32 PT_Audio_GetPlayTick(PT_AUDIOITEM AudioItem)
{
/*    U32 u32TimeStamp = 0;
    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();

    if (pAudio == NULL)
    {
        return FALSE;
    }

    //Get play time stamp
    u32TimeStamp = pAudio->GetPlayTick();

    return u32TimeStamp;*/

    MS_U32 u32TimeStamp = 0;
    u32TimeStamp = _MApi_AUDIO_GetPlayTick();

    return u32TimeStamp;
}

MMSDK_BOOL PT_Audio_StopDecode(PT_AUDIOITEM AudioItem)
{
    _MApi_AUDIO_StopDecode();
    MsOS_DelayTask(50); // this delay is critical, can't be too small
    _MApi_AUDIO_SetDTSCommonCtrl(Audio_DTS_infoType_ESData_ByteSwap, Audio_DTS_Ctrl_ByteSwap_OFF);

    g_bIsStopDecode = TRUE;

    return TRUE;

}

MMSDK_BOOL PT_Audio_PauseDecode(PT_AUDIOITEM AudioItem)
{
    _MApi_AUDIO_PauseDecode();
    return TRUE;
}

MMSDK_BOOL PT_Audio_SetAudioSyncMode(PT_AUDIOITEM AudioItem, const ST_MMSDK_AUDIO_START_INFO* pstAudioStart)
{
    MMSDK_BOOL bRet = FALSE;

    if ((AudioItem == NULL) || (pstAudioStart == NULL))
    {
        return bRet;
    }

    if ((pstAudioStart->bISNewAudioMode) && (pstAudioStart->bTSAudio))
    {
        // set audio auto sync stc
        if (pstAudioStart->bAutoSyncSTC)
        {
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
            bRet = TRUE;
        }
        else
        {
            _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
    }
    return bRet;
}

#if 1 // need to check // TODO (mapi_audio.h)Old function, please don't use, it will be remove soon.

MMSDK_BOOL PT_Audio_CheckPlayDone(PT_AUDIOITEM AudioItem)
{
/*    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    return pAudio->CheckPlayDone();*/
    return _MApi_AUDIO_CheckPlayDone();
}

MMSDK_BOOL PT_Audio_SetInput(PT_AUDIOITEM AudioItem, const MMSDK_U32 u32DataBuffSize, const MMSDK_U32 u32ValidDataSize)
{
/*    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    pAudio->SetInput();*/

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 0)
    {
        _gADECId = AU_DEC_ID1;
    }

    if (TRUE == g_bISNewAudioMode)
    {
        MApi_AUDIO_MM2_inputAesFinished(_gADECId, u32ValidDataSize, FALSE, 0);
    }
    else
    {
        _MApi_AUDIO_SetInput();
    }

    return TRUE;
}
MMSDK_BOOL PT_Audio_CheckInputRequest(PT_AUDIOITEM AudioItem, MMSDK_U32 *pu32WrtAddr, MMSDK_U32 *pu32WrtBytes)
{
/*    MMSDK_BOOL bret = FALSE;
    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    bret = (MMSDK_BOOL)pAudio->CheckInputRequest(pU32WrtAddr, pU32WrtBytes);
    return bret;*/
    MMSDK_BOOL bret = FALSE;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 0)
    {
        _gADECId = AU_DEC_ID1;
    }

    if (TRUE == g_bISNewAudioMode)
    {
        AES_INFO aes_info;
        memset((void*)&aes_info, 0, sizeof(AES_INFO));

        bret = (MMSDK_BOOL)MApi_AUDIO_MM2_checkAesInfo(_gADECId, &aes_info);

        if (TRUE == bret)
        {
            *pu32WrtAddr = g_u32AudioBaseBufferAddress + aes_info.aes_write_addr;
            *pu32WrtBytes = aes_info.aes_freeSpace;
        }
    }
    else
    {
        bret = _MApi_AUDIO_CheckInputRequest((void*)(MS_PHYADDR)pu32WrtAddr, pu32WrtBytes);
        if (pu32WrtAddr != NULL)
        {
            *pu32WrtAddr = MsOS_PA2KSEG1(*pu32WrtAddr);
        }
    }

    return bret;
}

MMSDK_BOOL PT_Audio_FileEndDataHandle(PT_AUDIOITEM AudioItem, const MMSDK_U32 u32DataLeft)
{
/*    mapi_audio* pAudio = mapi_interface::Get_mapi_audio();
    pAudio->FileEndDataHandle(u32DataLeft);*/

    _MApi_AUDIO_FileEndDataHandle(u32DataLeft);
    return TRUE;
}

MMSDK_BOOL PT_Audio_GetAudioUnsupport(PT_AUDIOITEM AudioItem, MMSDK_U64 *pu64Info)
{
    if ((AudioItem == NULL) || (pu64Info == NULL))
    {
        PT_AUDIO_ERR("[ERR] GetAudioUnsupport failed!\n");
        return FALSE;
    }

    *pu64Info = (MMSDK_U64)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_audio_unsupported);

    return TRUE;
}

MMSDK_U64 PT_Audio_GetUnsupportAudioCodec()
{
    MMSDK_U64 u64AudioCapability = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_capability);
    MMSDK_U64 u64UnsupportAudioCodec = 0;

    if (0 == (u64AudioCapability & Audio_DEC_PCM))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_PCM;
    }
    if (0 == (u64AudioCapability & Audio_DEC_XPCM))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_PCM;
    }
    if (0 == (u64AudioCapability & Audio_DEC_MPEG))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_MPG_AUDIO;
    }
    if (0 == (u64AudioCapability & Audio_DEC_MP3))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_MPG_AUDIO;
    }
    if (0 == (u64AudioCapability & Audio_DEC_AC3))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_AC3;
    }
    if (0 == (u64AudioCapability & Audio_DEC_AC3P))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_EAC3;
    }
    if ((0 == (u64AudioCapability & Audio_DEC_AAC_LC)) &&
            (0 == (u64AudioCapability & Audio_DEC_HEAAC_V1)) &&
            (0 == (u64AudioCapability & Audio_DEC_HEAAC_V2)) &&
            (0 == (u64AudioCapability & Audio_DEC_DOLBYPULSE)))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_AAC;
    }
    if (0 == (u64AudioCapability & Audio_DEC_WMA))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_WMA;
    }
    if (0 == (u64AudioCapability & Audio_DEC_WMAPRO))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_WMA;
    }
    if (0 == (u64AudioCapability & Audio_DEC_RA8))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_RM_AUDIO;
    }
    if (0 == (u64AudioCapability & Audio_DEC_DTS))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_DTS;
    }
    if (0 == (u64AudioCapability & Audio_DEC_FLAC))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_FLAC;
    }
    if (0 == (u64AudioCapability & Audio_DEC_VORBIS))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_VORBIS;
    }
    if (0 == (u64AudioCapability & Audio_DEC_AMR_NB))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_AMR_NB;
    }
    if (0 == (u64AudioCapability & Audio_DEC_AMR_WB))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_AMR_WB;
    }
    if (0 == (u64AudioCapability & Audio_DEC_DRA))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_DRA;
    }
    if (0 == (u64AudioCapability & Audio_DEC_DTSLBR))
    {
        u64UnsupportAudioCodec |= E_AUDIO_UNSUPPORT_CODEC_DTS_LBR;
    }

    return u64UnsupportAudioCodec;
}

#endif
