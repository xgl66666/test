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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_Zapper.h
/// @author MStar Semiconductor Inc.
/// @brief  Still image zapper, using XC freeze image
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_Zapper.c
   @brief Still image zapper, using XC freeze image
*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsMemory.h"
#include "drvGPIO.h"
#include "drvAUDIO.h"
#include "drvTVEncoder.h"
#include "drvMVOP.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiAUDIO.h"
#include "apiDMX.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiDMX.h"
#include "Panel.h"
#include "msAPI_XC.h"
#include "msAPI_VE.h"
#include "appDemo_Video.h"
#include "appDemo_Audio.h"
#include "appDemo_AV.h"


//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define SIZE_OF_XC_FULL_FUNCTION                    0xA00000

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_APPDEMO_ZAPPER_FREQ = 0,
    E_APPDEMO_ZAPPER_QAM,
    E_APPDEMO_ZAPPER_SR,
    E_APPDEMO_ZAPPER_VIDEOPID,
    E_APPDEMO_ZAPPER_AUDIOPID,
    E_APPDEMO_ZAPPER_PCRPID,
    E_APPDEMO_ZAPPER_VDECTYPE,
    E_APPDEMO_ZAPPER_ADECTYPE,
    E_APPDEMO_ZAPPER_INFONUM,
} E_APPDEMO_ZAPPER_CHANNEL_INFO;

typedef enum
{
    E_APPDEMO_ZAPPER_CH1 = 0,
    E_APPDEMO_ZAPPER_CH2,
    E_APPDEMO_ZAPPER_CHNUM,
} E_APPDEMO_ZAPPER_CHANNEL;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define AVSYNC_TIMEOUT_INTERVAL         2000
#define DEFAULT_VOL                     50
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL _bAudioInit = FALSE;
static MS_U32 u32LastChannel = E_APPDEMO_ZAPPER_CHNUM;

static MS_U32 stChannelInfo[E_APPDEMO_ZAPPER_CHNUM][E_APPDEMO_ZAPPER_INFONUM] =
{
    { // Ch1
        418000,     // Freq
        4,          // Qam
        5200,       // S/R
        33,        // Video PID
        36,        // Audio PID
        33,        // PCR PID
        0,           // VDEC type
        6            // ADEC type
    },
    { // Ch2
        354000,     // Freq
        2,          // Qam
        6875,       // S/R
        2001,        // Video PID
        2002,        // Audio PID
        2001,        // PCR PID
        1,           // VDEC type
        4            // ADEC type
    }
};


//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
extern MS_BOOL appDemo_DigiTuner_SetFreq(MS_U32 *Freq, MS_U32 *u32Arg1, MS_U32 *u32Arg2);
extern MS_BOOL DemoAV_TSP_SetPid(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
extern MS_BOOL appDemo_Video_GetDecoderStatus(void);
static MS_BOOL _DemoAV_VDEC_Init(int codec)
{
#if ENABLE_MIU_1
    MS_U32 u32MiuInterval = MIU_INTERVAL;
#else
    MS_U32 u32MiuInterval = 0;
#endif
    VDEC_EX_Status vdecStatus;
    VDEC_EX_InitParam initParam;
    VDEC_EX_CodecType codecType;
    VDEC_EX_CodecType codecTypeCur;

    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();
    MApi_VDEC_EX_Exit(stVDECStreamId);

    switch (codec)
    {
        default:
        case E_DDI_VDEC_CODEC_TYPE_MPEG2:
        case E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME:
        {
            codecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
            break;
        }
        case E_DDI_VDEC_CODEC_TYPE_H264:
        case E_DDI_VDEC_CODEC_TYPE_H264_IFRAME:
        {
            codecType = E_VDEC_EX_CODEC_TYPE_H264;
            break;
        }
    }

    memset(&vdecStatus, 0, sizeof(VDEC_EX_Status));
    codecTypeCur = MApi_VDEC_EX_GetActiveCodecType(stVDECStreamId);
    MApi_VDEC_EX_GetStatus(stVDECStreamId,&vdecStatus);
    if ((TRUE == vdecStatus.bInit) && (codecTypeCur == codecType))
    {
        return TRUE;
    }

    memset(&initParam, 0, sizeof(VDEC_EX_InitParam));
    initParam.bDisableDropErrFrame = TRUE;
    initParam.eCodecType = codecType;

    /* TODO START: H264.IFrame */
    //Temp Solution for I-Frame decoder on H264
    if(codec == E_DDI_VDEC_CODEC_TYPE_H264_IFRAME)
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_FILE;
    }
    else
    {
        initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_DTV;
    }

    initParam.SysConfig.u32CodeBufAddr = VDEC_AEON_ADR + u32MiuInterval;
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    initParam.SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR + u32MiuInterval;
    initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR + u32MiuInterval;
    initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
    initParam.SysConfig.u32DrvProcBufAddr = VDEC_BIT_STREAM_ADR + u32MiuInterval;
    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    printf("VDEC FW add = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr);

    MApi_VDEC_EX_EnableTurboMode(TRUE);

    if (E_VDEC_EX_OK != MApi_VDEC_EX_GetFreeStream(stVDECStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, (VDEC_EX_CodecType)initParam.eCodecType))
    {
        printf(" [%d]....GetFreeStream Failed\n", __LINE__);
        return FALSE;
    }
    else
    {
        printf("_DemoAV_VDEC_Init - stVDECStreamId:  0x%lx, Version 0x%lx \n",stVDECStreamId->u32Id, stVDECStreamId->u32Version);
    }

    //Init/Re-Init VDEC
    if (E_VDEC_EX_OK != MApi_VDEC_EX_Init(stVDECStreamId,&initParam))
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_EX_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _DemoAV_ADEC_Init(void)
{
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;

    if (TRUE == _bAudioInit)
    {
        return TRUE;
    }
    SystemInfo.tv_system=TV_PAL;
    SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
    SystemInfo.dtv_system=0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    OutputInfo.SpeakerOut=AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut=AUDIO_OUTPUT_HP;
    OutputInfo.ScartOut=AUDIO_OUTPUT_SIFOUT;
    OutputInfo.MonitorOut=AUDIO_OUTPUT_LINEOUT;
    MApi_AUDIO_SetOutputInfo(&OutputInfo);

    PathInfo.SpeakerOut = AUDIO_PATH_MAIN;
    PathInfo.HpOut = 0;
    PathInfo.MonitorOut = AUDIO_PATH_1;
    PathInfo.SpdifOut = AUDIO_PATH_2;
    PathInfo.ScartOut = AUDIO_PATH_1;
    MApi_AUDIO_SetPathInfo(&PathInfo);

    //MW_AUD_GetBinAddress();//expand to next two functions:
    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
    MDrv_AUDIO_SetDspBaseAddr(DSP_SE,  0, MAD_SE_BUF_ADR);
    MApi_AUDIO_Initialize();
    //MApi_AUDIO_EnableTone(TRUE);
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);

#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif
    _bAudioInit = TRUE;
    return TRUE;
}

static En_DVB_decSystemType _DemoAV_ADEC_Fmt(MS_U32 u32ACodec)
{
    switch (u32ACodec)
    {
    case 0x01:
        return MSAPI_AUD_DVB_MPEG;
    case 0x02:
        return MSAPI_AUD_DVB_AC3;
    case 0x03:
        return MSAPI_AUD_DVB_AC3P;
    case 0x04:
        return MSAPI_AUD_DVB_AAC;
/*
    case 0x05:
        return MSAPI_AUD_DVB_MP3;
    case 0x0B:
        return MSAPI_AUD_DVB_DTS;
*/
    default:
        return MSAPI_AUD_DVB_NONE;
    }
    return MSAPI_AUD_DVB_NONE;
}

static void _DemoAV_ADEC_Play(En_DVB_decSystemType AudioCodecType)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;

    if (MSAPI_AUD_DVB_NONE == AudioCodecType)
    {
        return;
    }
    _DemoAV_ADEC_Init();

    //Set MAD
    //MDrv_AUDIO_SetNormalPath(AUDIO_PATH_MAIN_SPEAKER, AUDIO_SOURCE_DTV, AUDIO_OUTPUT_MAIN_SPEAKER);
    MApi_AUDIO_SetSystem(AudioCodecType);
    MApi_AUDIO_SetCommand((En_DVB_decCmdType)MSAPI_AUD_PLAY);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);

    bMute = FALSE;
    appDemo_Audio_SetMute(&bMute);

    /* TODO START:  HDMITx Audio*/
    //MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    //MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    //MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
    /* TODO END:    HDMITx Audio*/

    u8Volume = DEFAULT_VOLUME;
    appDemo_Audio_SetAbsoluteVolume(&u8Volume);

}

//------------------------------------------------------------------------------
/// @brief The sample code to zapper channel
/// @param[in] u32Ch channel number
///        0: CH1
///        1: CH2
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Zapper_Channel 0 \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_Zapper_Channel(MS_U32 *u32Ch)
{
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    printf("Demo_Zapper_Ch1 \n");

    if(SC0_MAIN_FB_LEN < SIZE_OF_XC_FULL_FUNCTION)
    {
        printf("XC memory size is not enough for FB mode!\n");
        return FALSE;
    }

    if((*u32Ch >= E_APPDEMO_ZAPPER_CHNUM) || (*u32Ch < E_APPDEMO_ZAPPER_CH1))
    {
        printf("Error channel number!\n");
        return FALSE;
    }

    if(*u32Ch == u32LastChannel)
    {
        printf("No channel switch!\n");
        return TRUE;
    }

    /***** XC enable freeze image *****/
    MApi_XC_FreezeImg(TRUE, MAIN_WINDOW);


    /***** DigiTuner *****/
    if(u32LastChannel == E_APPDEMO_ZAPPER_CHNUM)  // First time
    {
        if(!appDemo_DigiTuner_SetFreq(&stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_FREQ], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_QAM], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_SR]))
        {
            return FALSE;
        }
    }
    else
    {
        if(stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_FREQ] != stChannelInfo[u32LastChannel][E_APPDEMO_ZAPPER_FREQ])
        {
            if(!appDemo_DigiTuner_SetFreq(&stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_FREQ], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_QAM], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_SR]))
            {
                return FALSE;
            }
        }
    }


    /***** SetPid *****/
    if(!DemoAV_TSP_SetPid(&stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_VIDEOPID], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_AUDIOPID], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_PCRPID], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_VDECTYPE], &stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_ADECTYPE]))
    {
        return FALSE;
    }

    /***** PlayAV *****/
    if (TRUE != _DemoAV_VDEC_Init(stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_VDECTYPE])) // default mepg2
    {
        printf("[%s][%d] _DemoAV_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    #if ENABLE_MIU_1
    #else
    // FOR Single MIU0 we have to reduce bandwidth usage
    if(E_VDEC_EX_OK != MApi_VDEC_EX_DisableDeblocking(stVDECStreamId,TRUE))
    {
        printf("MApi_VDEC_EX_DisableDeblocking != E_VDEC_EX_OK");
    }

    if(E_VDEC_EX_OK != MApi_VDEC_EX_DisableQuarterPixel(stVDECStreamId,TRUE))
    {
        printf("MApi_VDEC_EX_DisableQuarterPixel != E_VDEC_EX_OK");
    }
    #endif

    //change sync offset to 120ms, this value will influence lip-sync
    //If you want to change this value, please inform video and audio team at the same time
    if(E_VDEC_EX_OK != MApi_VDEC_EX_AVSyncOn(stVDECStreamId,TRUE, 180, 0))
    {
        printf("[%s][%d] MApi_VDEC_EX_AVSyncOn fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    //set the threshold of Freerun condition.If the difference of STC and PTS large than
    // threshold, FW will free run. The threshold value is 90KHZ base.
    if(E_VDEC_EX_OK != MApi_VDEC_EX_SetAVSyncFreerunThreshold(stVDECStreamId,450000))
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    MDrv_MVOP_Enable(DISABLE);

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);

    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xff);

    if (E_VDEC_EX_OK != MApi_VDEC_EX_Play(stVDECStreamId))
    {
        printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
    _DemoAV_ADEC_Play(_DemoAV_ADEC_Fmt(stChannelInfo[*u32Ch][E_APPDEMO_ZAPPER_ADECTYPE]));
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);


    /***** XC_PlayVideo *****/
    msAPI_XC_EnableCCIRInput(0, DISABLE);

    //Check Video Decoder Status
    if( TRUE != appDemo_Video_GetDecoderStatus())
    {
        printf("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }

    //Set VDEC FRC
    msAPI_XC_DTV_SetMode();

    //Set Scaler(HD PATH)
    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW);

    msAPI_VE_SetMode();

    //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
    MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);

    //Disable Blue Screen to avoid garbage video
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);


    /***** XC disable freeze image *****/
    MApi_XC_FreezeImg(FALSE, MAIN_WINDOW);

    u32LastChannel = *u32Ch;
    return TRUE;
}

static void _DemoAV_ADEC_Stop(void)
{
    MS_BOOL bMute = FALSE;

    bMute = TRUE;
    appDemo_Audio_SetMute(&bMute);

    MApi_AUDIO_SetCommand( (En_DVB_decCmdType)MSAPI_AUD_STOP );
}

MS_BOOL appDemo_FQ_Init(MS_U32* pu32FillFQTime)
{
    DMX_FQ_Info stFQInfo = {FALSE, TSP_FQ_BUF_ADR, TSP_FQ_BUF_LEN};
    if(MApi_DMX_FQ_Init(0, &stFQInfo) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MsOS_DelayTask(*pu32FillFQTime);//for FQ filling
    return TRUE;
}

MS_BOOL appDemo_FQ_Exit(void)
{
    MApi_DMX_FQ_Exit(0);
    return TRUE;
}

MS_BOOL appDemo_FQ_Zapping(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    MS_U32 u32During = 0;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    printf("appDemo_FQ_Zapping \n");
    u32During = MsOS_GetSystemTime();

    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);

    MS_BOOL bMute = TRUE;
    appDemo_Audio_SetMute(&bMute);

    msAPI_VE_SetVideoMute(ENABLE);
    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

    /***** SetPid *****/
    if(!DemoAV_TSP_SetPid(VideoPid, AudioPid, PCRPid, pu32VCodec, pu32ACodec))
        return FALSE;

    /***** PlayAV *****/
    if (TRUE != _DemoAV_VDEC_Init(*pu32VCodec))
    {
        printf("[%s][%d] _DemoAV_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //change sync offset to 120ms, this value will influence lip-sync
    //If you want to change this value, please inform video and audio team at the same time
    if(E_VDEC_EX_OK != MApi_VDEC_EX_AVSyncOn(stVDECStreamId,TRUE, 180, 0))
    {
        printf("[%s][%d] MApi_VDEC_EX_AVSyncOn fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    //set the threshold of Freerun condition.If the difference of STC and PTS large than
    // threshold, FW will free run. The threshold value is 90KHZ base.
    //set 0xFFFFFFFF to make sure VDEC will keep doing AV-sync till rush data is comsumed.
    if(E_VDEC_EX_OK != MApi_VDEC_EX_SetAVSyncFreerunThreshold(stVDECStreamId,0xFFFFFFFF))
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    MDrv_MVOP_Enable(DISABLE);

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 8);
    //MApi_VDEC_SetControl(VDEC_USER_CMD_FAST_DISPLAY, ENABLE);//default set in utopia VDEC driver

    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT,1);
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH,0xFF);
    if (E_VDEC_EX_OK != MApi_VDEC_EX_Play(stVDECStreamId))
    {
        printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    DemoAV_TSP_SetVideoDmxFltRushPass(TRUE);
    MApi_DMX_FQ_RushEnable(0);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);

    /***** XC_PlayVideo *****/
    msAPI_XC_EnableCCIRInput(0, DISABLE);

    MS_U32 u32EnableAVSync = FALSE;
    appDemo_Video_WaitAVSync(&u32EnableAVSync);

    appDemo_Video_SetDecStatusDelay(1);
    //Check Video Decoder Status
    if( TRUE != appDemo_Video_GetDecoderStatus())
    {
        printf("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }
    appDemo_Video_SetDecStatusDelay(10);

    //Set Scaler(HD PATH)
    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW);

    msAPI_VE_SetMode();

    //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
    //MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);//no need? by teddy.chen

    //Disable Blue Screen to avoid garbage video
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);
    printf("[%s][%d]Check Point 1:  %ld \n",__FUNCTION__,__LINE__,(MsOS_GetSystemTime()-u32During));

    //Handle audio later to manifestly demo the improvement of FQ
    if (*AudioPid != 0x1FFF)
    {
        _DemoAV_ADEC_Stop();
        _DemoAV_ADEC_Play(_DemoAV_ADEC_Fmt(*pu32ACodec));
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
    }

    //recover the AV sync freerun threshold anyway
    MS_U32 u32SyncTimes = 0;
    while(u32SyncTimes < AVSYNC_TIMEOUT_INTERVAL)
    {
        if(MApi_VDEC_EX_IsReachSync(stVDECStreamId))
        {
            MApi_VDEC_EX_SetAVSyncFreerunThreshold(stVDECStreamId,450000);
            break;
        }
        else if(u32SyncTimes == AVSYNC_TIMEOUT_INTERVAL)
            MApi_VDEC_EX_SetAVSyncFreerunThreshold(stVDECStreamId,450000);

        u32SyncTimes ++;

        MsOS_DelayTask(10);
    }
    printf("[%s][%d]Check Point 2: %ld \n",__FUNCTION__,__LINE__,(MsOS_GetSystemTime()-u32During));
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Print help message
/// @return None
/// @sa
/// Command: \b Command:Zapper_Help \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Zapper_Help(void)
{
    printf ("------------------------------------appDemo_Zapper Help--------------------------------------\n");
    printf ("press \"Zapper_Channel\" to switch channel\n");
    printf ("---------------------------------End of appDemo_Zapper Help----------------------------------\n");

    return TRUE;
}
