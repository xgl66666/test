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
/// @file   Demo_Audio.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for audio functions
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_AUDIO_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "drvGPIO.h"
#include "MsMemory.h"

#if (DEMO_AUDIO_HDMI_TEST == 1)
#include "apiHDMITx.h"
#endif

#include "../data/JAPAN_48_2_mp3.h"
#include "../data/WAV_44K.h"

#include "demo_utility.h"
#include "demo_audio.h"

#include "SysBsp.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
static Task_Info _audioDemo_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Demo Task"};
static Task_Info _audioMonitor_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Monitor"};
typedef struct
{
    MS_U32 u32HeaderSize;
    MS_U32 u32RIFFChunkSize;
    MS_U32 u32fmtChunkSize;
    MS_U32 u32DataChunkSize;
    MS_U16 u16FormatTag;
    MS_U16 u16Channels;
    MS_U32 u32SamplesPerSec;
    MS_U32 u32AvgBytesPerSec;
    MS_U16 u16BlockSize;
    MS_U16 u16BitsPerSample;
    MS_U16 u16CExtensionSize;
} ST_WAVINFO;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
unsigned char *pAudioData = NULL;
MS_S32 AudioLen = 0;
static MS_U32 _clipWriteLastAddr=0, _u32AudioFileIndex=0;
static MS_BOOL _bNewMM_Demo = FALSE;
static MS_U32 _u32StartTime = 0;

static MS_BOOL _bExit_Monitor_Task = FALSE;
static MS_BOOL _bExit_Demo_Task = FALSE;

#if (DEMO_AUDIO_HDMI_TEST == 1)
static AUDIO_FS_TYPE _stCurSampleType = AUDIO_FS_NONE;
static HDMI_TX_OUTPUT_TYPE _stCurHDMIType = HDMI_OUT_PCM;
#endif

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
MS_U16 AudioVolumeTable[] =
{ //    1     2       3        4      5       6        7      8        9       10
                                                                                                                    0x7F00, //  00
    0x4700, 0x4400, 0x4100, 0x3E00, 0x3C00, 0x3A00, 0x3800, 0x3600, 0x3400, 0x3200, //  10
    0x3000, 0x2E00, 0x2D00, 0x2C00, 0x2B00, 0x2A00, 0x2900, 0x2800, 0x2700, 0x2600, //  20
    0x2500, 0x2400, 0x2300, 0x2200, 0x2100, 0x2000, 0x1F00, 0x1E80, 0x1E00, 0x1D80, //  30
    0x1D00, 0x1C80, 0x1C00, 0x1B80, 0x1B00, 0x1A80, 0x1A00, 0x1980, 0x1900, 0x1880, //  40
    0x1800, 0x1780, 0x1700, 0x1680, 0x1600, 0x1580, 0x1540, 0x1500, 0x14C0, 0x1480, //  50
    0x1440, 0x1400, 0x13C0, 0x1380, 0x1340, 0x1300, 0x12C0, 0x1280, 0x1240, 0x1200, //  60
    0x11C0, 0x1180, 0x1140, 0x1100, 0x10C0, 0x1080, 0x1040, 0x1000, 0x0FE0, 0x0FC0, //  70
    0x0FA0, 0x0F80, 0x0F60, 0x0F40, 0x0F20, 0x0F00, 0x0EE0, 0x0EC0, 0x0EA0, 0x0E80, //  80
    0x0E60, 0x0E40, 0x0E20, 0x0E00, 0x0DE0, 0x0DC0, 0x0DA0, 0x0D80, 0x0D60, 0x0D40, //  90
    0x0D20, 0x0D00, 0x0CE0, 0x0CC0, 0x0CA0, 0x0C80, 0x0C60, 0x0C40, 0x0C20, 0x0C00  //  100
};

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief The sample code to feed data to audio codec
/// @param[in] writeAddress The address that audio data should put in
/// @param[in] writeNumber The length that audio data should put in
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_Audio_File_play(MS_U32 writeAddress, MS_U32 writeNumber, MS_BOOL bNewMMmode)
{
    //printf("\nDemo_Audio_File_play(0x%lx,0x%lx)...\n",writeAddress,writeNumber);
    if(bNewMMmode)
    {
        //new MM mode

        MS_U32 u32RemainedSize = AudioLen- _u32AudioFileIndex;
        MS_U32 u32es_size = 0;
        MS_U64 u64PTS = 0;

        //TRUE: Set PTS; FALSE: Not set PTS (New MM mode support this feature)
        MS_BOOL bSetPTS = TRUE;

        if (_u32AudioFileIndex ==  0 )
        {
            if (_u32StartTime == 0)
            {
                _u32StartTime = MsOS_GetSystemTime();
            }
        }

        if (u32RemainedSize > 0)
        {
            if(writeNumber>0)
            {
                u32es_size = MIN(writeNumber, u32RemainedSize);

                memcpy((void*)MS_PA2KSEG1(MApi_AUDIO_GetDDRInfo(AU_DEC_ID1, E_AUD_MEMORY_BASE)+writeAddress), (pAudioData+_u32AudioFileIndex), u32es_size);

                _u32AudioFileIndex += u32es_size;

                //example of setting system time differemce (ticks) as PTS
                u64PTS = (MsOS_GetSystemTime() - _u32StartTime) * 90;

                //info DSP data is ready
                MApi_AUDIO_MM2_inputAesFinished(AU_DEC_ID1, u32es_size, bSetPTS, u64PTS);
            }
        }
        else
        {
            printf("[%s][%d] play finished!!!\n", __FUNCTION__, __LINE__);

            //reset file index
            _u32AudioFileIndex = 0;
        }
    }
    else
    {
        //old MM mode
        if( writeAddress == _clipWriteLastAddr )
        {
            MApi_AUDIO_SetInput();
            return;
        }

        _clipWriteLastAddr = writeAddress ;
        if (_u32AudioFileIndex < AudioLen )
        {
            if ((_u32AudioFileIndex + writeNumber) <= AudioLen )
            {
                memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+_u32AudioFileIndex),writeNumber);
                _u32AudioFileIndex += writeNumber;
            }
            else
            {
                // clear the buffer first
                memset((void*)MS_PA2KSEG1(writeAddress), 0, writeNumber);

                if (writeNumber <= AudioLen )
                {
                    // normal case
                    int feedsize = 0;

                    memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+_u32AudioFileIndex),AudioLen  - _u32AudioFileIndex);
                    feedsize += (AudioLen  -_u32AudioFileIndex);

                    _u32AudioFileIndex = 0;
                    memcpy((void*)MS_PA2KSEG1(writeAddress+feedsize), (void*)(pAudioData+_u32AudioFileIndex), writeNumber - feedsize);

                    _u32AudioFileIndex = writeNumber- feedsize;
                }
            }
        }

        MApi_AUDIO_SetInput();
    }

}

static MS_BOOL _Demo_Audio_CheckInputRequest(MS_U32* WrtAddr, MS_U32* WrtBytes, MS_BOOL bNewMMmode)
{
    AES_INFO es_info;
    MS_BOOL bRet = FALSE;
    if(bNewMMmode)
    {
        bRet = MApi_AUDIO_MM2_checkAesInfo(AU_DEC_ID1, &es_info);
        *WrtAddr = es_info.aes_write_addr;
        *WrtBytes = es_info.aes_freeSpace;
    }
    else
    {
        bRet = MApi_AUDIO_CheckInputRequest(WrtAddr, WrtBytes);
    }
    return bRet;
}

static void _appAudioDemo_task(MS_U32 argc, void *argv)
{
    MS_U32 WrtAddr = 0;
    MS_U32 WrtBytes = 0;

    while(_bExit_Demo_Task == FALSE)
    {
        if(_Demo_Audio_CheckInputRequest(&WrtAddr, &WrtBytes, _bNewMM_Demo))
        {
            _Demo_Audio_File_play(WrtAddr, WrtBytes, _bNewMM_Demo);
        }
        MsOS_DelayTask(1);
    }

    _bExit_Demo_Task = FALSE;
    _audioDemo_Task.iId = -1;
}

static MS_BOOL _Demo_Audio_CheckAppSupport(EN_AUDIO_APP_TYPE eAppType)
{
    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
#if (DEMO_AUDIO_AD_TEST == 1)
    case AUDIO_APP_DTV_AD:
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
#endif
        return TRUE;
    default:
        return FALSE;
    }
}

static En_DVB_decSystemType _Demo_Audio_CodecTypeMap(EN_AUDIO_CODEC_TYPE eCodecType)
{
    switch(eCodecType)
    {
        case AUDIO_CODEC_NONE: return MSAPI_AUD_DVB_NONE;
        case AUDIO_CODEC_MPEG: return MSAPI_AUD_DVB_MPEG;
        case AUDIO_CODEC_AC3: return MSAPI_AUD_DVB_AC3;
        case AUDIO_CODEC_AC3P: return MSAPI_AUD_DVB_AC3P;
        case AUDIO_CODEC_AAC: return MSAPI_AUD_DVB_AAC;
        case AUDIO_CODEC_MP3: return MSAPI_AUD_DVB_MP3;
        case AUDIO_CODEC_WMA: return MSAPI_AUD_DVB_WMA;
        case AUDIO_CODEC_CDLPCM: return MSAPI_AUD_DVB_CDLPCM;
        case AUDIO_CODEC_RA8LBR: return MSAPI_AUD_DVB_RA8LBR;
        case AUDIO_CODEC_XPCM: return MSAPI_AUD_DVB_XPCM;
        case AUDIO_CODEC_TONE: return MSAPI_AUD_DVB_TONE;
        case AUDIO_CODEC_DTS: return MSAPI_AUD_DVB_DTS;
        case AUDIO_CODEC_MS10_DDT: return MSAPI_AUD_DVB_MS10_DDT;
        case AUDIO_CODEC_MS10_DDC: return MSAPI_AUD_DVB_MS10_DDC;
        case AUDIO_CODEC_WMA_PRO: return MSAPI_AUD_DVB_WMA_PRO;
        case AUDIO_CODEC_FLAC: return MSAPI_AUD_DVB_FLAC;
        case AUDIO_CODEC_VORBIS: return MSAPI_AUD_DVB_VORBIS;
        case AUDIO_CODEC_DTSLBR: return MSAPI_AUD_DVB_DTSLBR;
        case AUDIO_CODEC_AMR_NB: return MSAPI_AUD_DVB_AMR_NB;
        case AUDIO_CODEC_AMR_WB: return MSAPI_AUD_DVB_AMR_WB;
        case AUDIO_CODEC_DRA: return MSAPI_AUD_DVB_DRA;
        case AUDIO_CODEC_DolbyTrueHDBypass: return MSAPI_AUD_DVB_DolbyTrueHDBypass;

        //AD
        case AUDIO_CODEC_AD_NONE: return MSAPI_AUD_DVB2_NONE;
        case AUDIO_CODEC_AD_MPEG: return MSAPI_AUD_DVB2_MPEG;
        case AUDIO_CODEC_AD_AC3: return MSAPI_AUD_DVB2_AC3;
        case AUDIO_CODEC_AD_MP3: return MSAPI_AUD_DVB2_MP3;
        case AUDIO_CODEC_AD_AAC: return MSAPI_AUD_DVB2_AAC;
        case AUDIO_CODEC_AD_GAAC: return MSAPI_AUD_DVB2_GAAC;
        case AUDIO_CODEC_AD_AC3P: return MSAPI_AUD_DVB2_AC3P;
        case AUDIO_CODEC_AD_DDE: return MSAPI_AUD_DVB2_DDE;
        case AUDIO_CODEC_AD_DTSE: return MSAPI_AUD_DVB2_DTSE;
        case AUDIO_CODEC_AD_XPCM: return MSAPI_AUD_DVB2_XPCM;
        case AUDIO_CODEC_AD_KTV: return MSAPI_AUD_DVB2_KTV;
        case AUDIO_CODEC_AD_KTV2: return MSAPI_AUD_DVB2_KTV2;
        case AUDIO_CODEC_AD_DTS: return MSAPI_AUD_DVB2_DTS;
        case AUDIO_CODEC_AD_TONEGEN: return MSAPI_AUD_DVB2_TONEGEN;

        //ATV SIF
        case AUDIO_CODEC_ATV_NONE: return MSAPI_AUD_ATV_NONE;
        case AUDIO_CODEC_ATV_BTSC: return MSAPI_AUD_ATV_BTSC;
        case AUDIO_CODEC_ATV_EIAJ: return MSAPI_AUD_ATV_EIAJ;
        case AUDIO_CODEC_ATV_PAL: return MSAPI_AUD_ATV_PAL;
        case AUDIO_CODEC_ATV_FM_RADIO: return MSAPI_AUD_ATV_FM_RADIO;
        case AUDIO_CODEC_ATV_MAX: return MSAPI_AUD_ATV_MAX;

        //ATV ENC
        case AUDIO_CODEC_ATVENC_NONE: return MSAPI_AUD_ATVENC_NONE;
        case AUDIO_CODEC_ATVENC_FMTX: return MSAPI_AUD_ATVENC_FMTX;
        case AUDIO_CODEC_ATVENC_BTSC: return MSAPI_AUD_ATVENC_BTSC;
        default: return MSAPI_AUD_DVB_INVALID;
    }
}

#if (DEMO_AUDIO_HDMI_TEST == 1)
static void _appDemo_HDMI_TX_Status_Monitor(void)
{
    MS_BOOL hdmi_tx_en;
    AUDIO_FS_TYPE fs_type;
    HDMI_TX_OUTPUT_TYPE outType;
    HDMITX_AUDIO_FREQUENCY hreq = HDMITX_AUDIO_FREQ_NO_SIG;
    HDMITX_AUDIO_CODING_TYPE htype = HDMITX_AUDIO_PCM;

    MApi_AUDIO_HDMI_Tx_GetStatus(&hdmi_tx_en, &fs_type, &outType);

    if (hdmi_tx_en)
    {
        if((_stCurSampleType == fs_type) && (_stCurHDMIType == outType))
        {
            return;
        }
        else
        {
            _stCurSampleType = fs_type;
            _stCurHDMIType = outType;
        }

        if (outType == HDMI_OUT_NONPCM)
        {
            htype = HDMITX_AUDIO_NONPCM;
            switch (fs_type)
            {
            case AUDIO_FS_32KHZ:
                hreq = HDMITX_AUDIO_32K;
                break;
            case AUDIO_FS_44KHZ:
                hreq = HDMITX_AUDIO_44K;
                break;
            case AUDIO_FS_176KHZ:
                hreq = HDMITX_AUDIO_176K;
                break;
            case AUDIO_FS_192KHZ:
                hreq = HDMITX_AUDIO_192K;
                break;
            case AUDIO_FS_48KHZ:
            default:
                hreq = HDMITX_AUDIO_48K;
                break;
            }
        }
        else
        {
            htype = HDMITX_AUDIO_PCM;
            hreq = HDMITX_AUDIO_48K;                            //for PCM mode always SRC to 48K
        }

        printf("Hdmx Tx:%x , %x\n", hreq, htype);
        MApi_HDMITx_SetAudioOnOff(TRUE);
        MApi_HDMITx_SetAudioFrequency(hreq);
        MApi_HDMITx_SetAudioConfiguration(hreq, HDMITX_AUDIO_CH_2, htype);
    }

}
#endif

static void _appAudioMonitor_task(MS_U32 argc, void *argv)
{
    while(_bExit_Monitor_Task == FALSE)
    {
        //printf("_appAudioMonitor_task\n");
        MApi_Audio_Monitor();

#if (DEMO_AUDIO_HDMI_TEST == 1)
        //monitor if HDMI Tx status change
        _appDemo_HDMI_TX_Status_Monitor();
#endif
        
        //monitor smple rate change
        MApi_Audio_SPDIF_Monitor();
   			
        MsOS_DelayTask(1);
    }

    _bExit_Monitor_Task = FALSE;
    _audioMonitor_Task.iId = -1;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#if(DEMO_AUDIO_SE_DV258_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to enable/disable Dolby Volume 258
/// @param[in] u8Enable
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DV258 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DV258(MS_U8* u8Enable)
{
    MS_BOOL bret = FALSE;
    bret = MApi_AUDIO_ADVSOUND_SetParam(DV258_ENABLE, *u8Enable, 0);
    return bret;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set DRC scale value
/// @param[in] u32CutScale
/// @param[in] u32BoostScale
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DRC_Scale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Scale(MS_U32* u32CutScale, MS_U32* u32BoostScale)
{
    MS_BOOL bret = FALSE, bret1 = FALSE;
    bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_Drc_HighCutScale, *u32CutScale, 0);
    bret1 = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_Drc_LowBootScale, *u32BoostScale, 0);
    if (bret && bret1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set down mix mode
/// @param[in] eCodecType 2: AC3 3: AC3P 4: AAC
/// @param[in] mode 0: LtRt mode 1: LoRo mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_Downmix \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DownMix_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode)
{
    MS_BOOL bret = FALSE;
    switch (*eCodecType)
    {
        case AUDIO_CODEC_AC3:
        case AUDIO_CODEC_AC3P:
        case AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DownmixMode, *mode, 0);
            break;
        default:
            bret = TRUE;
            printf("Down Mix mode not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set dyanmic range control mode (DRC mode)
/// @param[in] eCodecType 2: AC3 3: AC3P 4: AAC
/// @param[in] mode 0: LINE mode 1: RF mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_DRC \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode)
{
    MS_BOOL bret = FALSE;
    switch (*eCodecType)
    {
        case AUDIO_CODEC_AC3:
        case AUDIO_CODEC_AC3P:
        case AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode, *mode, 0);
            break;
        default:
            bret = TRUE;
            printf("Down Mix mode not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}


#if (DEMO_AUDIO_FMTX_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode FMTX signal
/// @param[in] u8Filter 0: PAL 1: NTSC
/// @return none
/// @sa
/// @note
/// Command: \b Audio_FMTX_start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_FMTX_Start(MS_U8* u8Filter, MS_U16* u16Attenuation, MS_U16* u16OutScaling)
{
    ST_AUDIO_FMTX_PARAM stParam;

    stParam.u8Filter = *u8Filter;
    stParam.u16Attenuation = *u16Attenuation;
    stParam.u16OutScaling = *u16OutScaling;

    return Demo_Audio_Play(0, AUDIO_APP_ATV_FMTX, AUDIO_CODEC_ATVENC_FMTX, (void *)&stParam);
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop FMTX
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_ATV_stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_FMTX_Stop(void)
{
    return Demo_Audio_Stop(0, AUDIO_APP_ATV_FMTX);
}
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode BTSC signal
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_BTSC_start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_BTSC_Start(MS_U16* u16Attenuation, MS_U16* u16OutScaling, MS_U16* u16_M_Gain, MS_U16* u16_D_Gain, MS_U16* u16_SAP_Gain)
{
    ST_AUDIO_BTSC_PARAM stParam;

    stParam.u16Attenuation = *u16Attenuation;
    stParam.u16OutScaling = *u16OutScaling;
    stParam.u16_M_Gain = *u16_M_Gain;
    stParam.u16_D_Gain = *u16_D_Gain;
    stParam.u16_SAP_Gain = *u16_SAP_Gain;

    return Demo_Audio_Play(0, AUDIO_APP_ATV_BTSC, AUDIO_CODEC_ATVENC_BTSC, (void *)&stParam);
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop BTSC
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_ATV_stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_BTSC_Stop(void)
{
    return Demo_Audio_Stop(0, AUDIO_APP_ATV_BTSC);
}
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set S/PDIF mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x1 : SPDIF off
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: Compare with HDMI, no additional actions needs for SPDIF non PCM mode.
///        (For more detail steps, please refer to the test plan.)
///
/// Command: \b Audio_SPDIF_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_SPDIF_Mode(MS_U8* u8Mode)
{
    MApi_AUDIO_SPDIF_SetMode(*u8Mode);
    return TRUE;
}
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set HDMI Tx mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - 0x0 : HDMIT Tx, PCM mode
///                - 0x1 : HDMIT Tx, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: constraints on HDMI non PCM mode.
///        If set HDMI to non pcm mode, you need do the following actions. (For more
///        detail steps, please refer to the test plan.)
/// ================================================================================
///           |         48K          |        44.1K         |         32K          |
/// ================================================================================
///  Kronus   | script:              | script:              | script:              |
///  Kaiserin | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///  Kaiser   |                      | Audio_SPDIF_Mode 2   | Audio_SPDIF_Mode 2   |
///           |                      |                      |                      |
///           | (HDMI, SPDIF can be  | (If set HDMI to non  | (If set HDMI to non  |
///           | different mode)      | PCM mode, need set   | PCM mode, need set   |
///           |                      | SPDIF to non PCM     | SPDIF to non PCM     |
///           |                      | mode)                | mode)                |
/// ================================================================================
///  Kappa    | script:              | script:              | script:              |
///  Keltic   | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///  Kenya    |                      |                      |                      |
///  Kriti    | (If set HDMI to non  | (If set HDMI to non  | (If set HDMI to non  |
///           | PCM mode, SPDIF will | PCM mode, SPDIF will | PCM mode, SPDIF will |
///           | change to non PCM)   | change to non PCM)   | change to non PCM)   |
/// ================================================================================
///  Keres    | script:              | script:              | script:              |
///  Clippers | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///           |                      |                      |                      |
///           | (HDMI, SPDIF can be  | (HDMI, SPDIF can be  | (HDMI, SPDIF can be  |
///           | different mode)      | different mode)      | different mode)      |
/// ================================================================================
///
/// Command: \b Audio_HDMITx_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_HDMITx_Mode(MS_U8* u8Mode)
{
    MApi_AUDIO_HDMI_TX_SetMode(*u8Mode);
    return TRUE;
}

MS_BOOL Demo_Audio_DDP(MS_U8* u8Mode)
{
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, *u8Mode, 0);      //enable
    return TRUE;
}

#endif

MS_BOOL Demo_Audio_SetMonitor(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    if(_audioMonitor_Task.iId == -1)
    {
        _audioMonitor_Task.pStack = MsOS_AllocateMemory(_audioMonitor_Task.u32StackSize, s32MstarCachedPoolID);
        if(!_audioMonitor_Task.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }
        _audioMonitor_Task.iId = MsOS_CreateTask( _appAudioMonitor_task,
                                            NULL,
                                            _audioMonitor_Task.ePriority,
                                            TRUE,
                                            _audioMonitor_Task.pStack,
                                            _audioMonitor_Task.u32StackSize,
                                            _audioMonitor_Task.szName);

        if(_audioMonitor_Task.iId < 0)
        {
            printf("create task fail \n");
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to set audio monitor debug level and enable monitor task
/// @param u8Level      \b : debug level
///                            0: Audio_DumpInfo_Off          = 0x0,
///                            1: Audio_DumpInfo_Avsync       = 0x1,
///                            2: Audio_DumpInfo_Dec1         = 0x2,
///                            4: Audio_DumpInfo_IoInfo       = 0x4,
///                            8: Audio_DumpInfo_mmFileAVsync = 0x8,
///                           21: Audio_DumpInfo_All          = 0x15,
///                           25: Audio_DumpInfo_testSTC      = 0x19,
///                          128: Audio_DumpInfo_AutoTest     = 0x80,
/// @param u16Interval  \b : print message interval
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Debug \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval)
{
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Dump_Dsp_Info, *u8Level, *u16Interval);
    if(*u8Level != 0)
    {
        Demo_Audio_SetMonitor();
    }
    else
    {
        _bExit_Monitor_Task = TRUE;
    }
    return TRUE;
}

#if (DEMO_AUDIO_AD_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio description
/// @param[in] vol 0~100(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume_ad \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetADAbsoluteVolume(MS_U8* vol)
{
    MS_U16 u16VolumeValue = 0;
    MS_U8 value1 = 0;
    u16VolumeValue = AudioVolumeTable[*vol];
    value1 = (MS_U8)(u16VolumeValue >> 8);
    MApi_AUDIO_SetADAbsoluteVolume(value1);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute_ad \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_ADSetMute(MS_BOOL* bEnable)
{
    //mute AD
    MApi_AUDIO_ADSetMute(*bEnable);

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code to set audio output mode
/// @param mode
///     0:STEREO
///     1: LL
///     2: RR
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_SetOutputMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetOutputMode(MS_U32* mode)
{
    MS_BOOL ret;

    ret = MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, *mode, 0);

    return ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio
/// @param[in] vol 0~99(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAbsoluteVolume(MS_U8* vol)
{
    MS_U8 value1, value2;
    MS_U16 u16VolumeValue=0;
    MS_BOOL bMute = FALSE;

    MS_U16 u16VolumeValue_LR=0;
    MS_S16 s16LR_Compensation = AUDIO_VOL_LR_COMPENSATION;

#if (DEMO_AUDIO_HDMI_TEST == 1)
    MS_U16 u16VolumeValue_HDMI=0;
    MS_S16 s16HDMI_Compensation = AUDIO_VOL_HDMI_COMPENSATION;
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    MS_U16 u16VolumeValue_SPDIF=0;
    MS_S16 s16SPDIF_Compensation = AUDIO_VOL_SPDIF_COMPENSATION;
#endif

    if( *vol > 100 )
    {
        *vol = 100;
    }

    if(*vol == 0)
    {
        bMute = TRUE;
        Demo_Audio_SetMute(&bMute);
    }
    else
    {
        bMute = FALSE;
        Demo_Audio_SetMute(&bMute);
    }

    printf("\n[%s][%d]########## audio volume = %d ##########\n\n", __FUNCTION__, __LINE__, *vol);
    u16VolumeValue = AudioVolumeTable[*vol];

    //Line-out (LR) vol + compensation
    u16VolumeValue_LR = u16VolumeValue + s16LR_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_LR > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_LR>>8);
        value2 = (MS_U8)((u16VolumeValue_LR&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_MAIN_SPEAKER, value1, value2);

#if (DEMO_AUDIO_HDMI_TEST == 1)
    //HDMI vol + compensation
    u16VolumeValue_HDMI = u16VolumeValue + s16HDMI_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_HDMI > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_HDMI>>8);
        value2 = (MS_U8)((u16VolumeValue_HDMI&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_HDMI, value1, value2);
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    //SPDIF vol + compensation
    u16VolumeValue_SPDIF = u16VolumeValue + s16SPDIF_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_SPDIF > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_SPDIF>>8);
        value2 = (MS_U8)((u16VolumeValue_SPDIF&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_SPDIF, value1, value2);
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
    //BTSC output to ch34 via dac circuit, the compensation is equal to Line-out
    //vol + compensation
    u16VolumeValue_LR = u16VolumeValue + s16LR_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_LR > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_LR>>8);
        value2 = (MS_U8)((u16VolumeValue_LR&0x00FF)>>5);
    }

    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_BTSC1, value1, value2);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_BTSC2, value1, value2);

#endif
    return TRUE;
}

#if (DEMO_AUDIO_DMA_READER_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to do DMA Reader test
/// @param[in] pFilename input file name
/// @param[in] u8SampleRate file sampling rate
///            0: 48K
///            1: 44.1K
///            2: 32K
///            3: 24K
///            4: 22.05K
///            5: 16K
///            6: 12K
///            7: 11.025K
///            8: 8K
///            9: 4K
/// @param[in] u8RepeatTime repeat time
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_DMA_Test \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime)
{
    FILE *file = NULL;
    MS_U16 read_size = 0;
    MS_U32 file_size = 0;
    MS_S32 residual_size = 0;
    MS_U8* pbuf = NULL;
    MS_U32 buf_size = 4096;
    MS_U64 u64RingInbuf_size = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferSize);
    MS_U64 u64pcm_level = 0;
    MS_S8 u8loop_left = *u8RepeatTime;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_BOOL bResend = FALSE;

    if (u64RingInbuf_size == 0)
    {
        printf("[%s][%d] Ring buffer size is 0 !!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    pbuf = MsOS_AllocateMemory(buf_size, s32MstarNonCachedPoolID);
    if (pbuf == NULL)
    {
        printf("allocate memory fail!!!\n");
        return FALSE;
    }

    file = fopen((char*)pFilename, "rb");

    if (file == NULL)
    {
        printf("open file fail!!!\n");
        return FALSE;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);           // Get total file size
    fseek(file, 0, SEEK_SET);

    // init DMA reader
#if 1 /* darrenC */
    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
#endif
    MApi_AUDIO_DmaReader_Init(*u8SampleRate);
    printf("==========> dma reader init, smp rate is 48k\n");

    // set DMA reader command start
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DMAReader_Command, 1, 0);

    while ((u8loop_left!=0) || (residual_size!=0))
    {
        if (residual_size == 0)
        {
            fseek(file, 0, SEEK_SET);     // Seek to file begin if no data can read
            residual_size = file_size;
            //u32index=0;
            u8loop_left--;
            printf("count = %d\n", u8loop_left);
        }

        read_size = (residual_size >= buf_size) ? buf_size : residual_size;

        // check dma_reader buffer level
        u64pcm_level = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferLevel);
        printf("pcm lvl = %llx\n", u64pcm_level);

        // residual buffer size > read size, keep sending PCM data to DMA reader buffer
        if ((u64RingInbuf_size - u64pcm_level) >= read_size)
        {
            if (bResend == FALSE)
            {
                // read new data from usb if no need re-send
                read_size = fread((void *)pbuf, 1, read_size, file);
            }

            // write data to buffer
            if(MApi_AUDIO_DmaReader_WritePCM((void*)(MS_VA2PA((MS_U32)pbuf)), (MS_U32)read_size) == FALSE)
            {
                printf("write PCM ng!\n");
                bResend = TRUE;
            }
            else
            {
                residual_size -= read_size;
                bResend = FALSE;
            }
        }

        MsOS_DelayTask(20);    // for 48K ring tone
    }

    fclose(file);

    MsOS_FreeMemory(pbuf, s32MstarNonCachedPoolID);
    return TRUE;
}
#endif  //#if (DEMO_AUDIO_DMA_READER_TEST == 1)

//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] vol
///     1: mute
///     0: non-mute
/// @return none.
/// @sa
/// @note
/// Command: \b Audio_mute \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMute(MS_BOOL* bEnable)
{
    //mute line out
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, *bEnable);
#if (DEMO_AUDIO_HDMI_TEST == 1)
    //mute HDMI PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, *bEnable);

    //mute HDMI PCM + non PCM
    MApi_AUDIO_HDMI_Tx_SetMute(*bEnable);
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    //mute SPDIF PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, *bEnable);

    //mute SPDIF PCM + non PCM
    MApi_AUDIO_SPDIF_SetMute(*bEnable);
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
    //mute BTSC
    MApi_AUDIO_SetMute(AUDIO_PATH_BTSC1, *bEnable);
    MApi_AUDIO_SetMute(AUDIO_PATH_BTSC2, *bEnable);
#endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop music
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_demoStop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Stop_Demo(void)
{
    _clipWriteLastAddr=0;
    _u32AudioFileIndex=0;
    MApi_AUDIO_StopDecode();
    _bExit_Demo_Task = TRUE;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize audio function
/// @param[in] Init type
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Init(MS_U32 *Type)
{
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    AUDIO_SOURCE_INFO_TYPE eSrcType;
    EN_AUDIO_APP_TYPE eType;

    printf("%s\n", __FUNCTION__);


    eType = *(EN_AUDIO_APP_TYPE *)Type;
    if(_Demo_Audio_CheckAppSupport(eType) == FALSE)
    {
        printf("unsupport application type: %u\n", (unsigned int)eType);
        return FALSE;
    }

    MApi_AUDIO_WritePreInitTable();

    //======================================
    // Set system info
    //======================================
    SystemInfo.tv_system=TV_PAL;
    SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
    SystemInfo.dtv_system=0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    //======================================
    // Set output info(Output data from)
    //======================================
    OutputInfo.SpeakerOut = AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut = AUDIO_OUTPUT_HP;
    OutputInfo.ScartOut = AUDIO_OUTPUT_SCART1;
    OutputInfo.MonitorOut = AUDIO_NULL_OUTPUT;
    MApi_AUDIO_SetOutputInfo(&OutputInfo);


    //======================================
    // Set path info (volume path control)
    //======================================
    PathInfo.SpeakerOut = AUDIO_PATH_MAIN_SPEAKER;
    PathInfo.HpOut = AUDIO_PATH_HP;
    PathInfo.MonitorOut = AUDIO_NULL_OUTPUT;
#if (DEMO_AUDIO_SPDIF_TEST == 1)
    PathInfo.SpdifOut = AUDIO_PATH_SPDIF;
#else
    PathInfo.SpdifOut = AUDIO_PATH_NULL;
#endif
    PathInfo.ScartOut = AUDIO_PATH_SCART1;
    MApi_AUDIO_SetPathInfo(&PathInfo);

#if (DEMO_AUDIO_R2_MEM_ARCHI_TEST)
    // if new IC (Keres/Clippers)
    MDrv_AUDIO_SetDspBaseAddr(DSP_ADV,  0, MAD_ADV_BUF_ADR);
#else
    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
#endif

    MApi_AUDIO_Initialize();

    //======================================
    // Set source type
    //======================================
    switch(eType)
    {
    case AUDIO_APP_ES_PLAY:
        eSrcType = E_AUDIO_INFO_GAME_IN;
        break;
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
    case AUDIO_APP_DTV_AD:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
    case AUDIO_APP_ATV_FMTX:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
    default:
        eSrcType = E_AUDIO_INFO_GAME_IN;
    }

    MApi_AUDIO_SetSourceInfo(eSrcType);

    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);
    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);

#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Start audio dec play
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @param[in] eCodecType
///     codec type
/// @param[in] pvParam
///     input parameter (depend on codec type)
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Play(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, EN_AUDIO_CODEC_TYPE eCodecType, void *pvParam)
{
    En_DVB_decSystemType eDecSysType;

    if(u32DevIdx > 0)
    {
        printf("unsupport device index: %d\n", (unsigned int)u32DevIdx);
        return FALSE;
    }

    if(_Demo_Audio_CheckAppSupport(eAppType) == FALSE)
    {
        //printf("unsupport application type: %u\n", (unsigned int)eAppType);
        return FALSE;
    }

    eDecSysType = _Demo_Audio_CodecTypeMap(eCodecType);

    if(eDecSysType == MSAPI_AUD_DVB_INVALID)
    {
        return FALSE;
    }

    MApi_AUDIO_SetSystem(eDecSysType);

    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
        if(eDecSysType == MSAPI_AUD_DVB_XPCM)
        {
            ST_WAVINFO *pstWAV = (ST_WAVINFO *)pvParam;

            MApi_AUDIO_SetXPCMParam(
                LPCM,
                pstWAV->u16Channels-1,
                pstWAV->u32SamplesPerSec,
                pstWAV->u16BitsPerSample,
                pstWAV->u16BlockSize,
                pstWAV->u16BitsPerSample
            );
        }

        if(_bNewMM_Demo == TRUE)
        {
            //new MM mode play command
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
            //can set av sync or free run
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        else
        {
            MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        break;
    case AUDIO_APP_DTV:
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
        break;
    case AUDIO_APP_DTV_AM:
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        break;
#if (DEMO_AUDIO_AD_TEST == 1)
    case AUDIO_APP_DTV_AD:
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB2_DECCMD_PLAY);
        MApi_AUDIO_SetADOutputMode(AD_OUT_BOTH);
        break;
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
        {
            ST_AUDIO_FMTX_PARAM *stFMTX = (ST_AUDIO_FMTX_PARAM *)pvParam;
            MS_U8 u8Filter = stFMTX->u8Filter;
            MS_U8 u8Attenuation_H = (MS_U8)((stFMTX->u16Attenuation) >> 8);
            MS_U8 u8Attenuation_L = (MS_U8)((stFMTX->u16Attenuation) & 0xFF);
            MS_U8 u8OutScaling_H = (MS_U8)((stFMTX->u16OutScaling) >> 8);
            MS_U8 u8OutScaling_L = (MS_U8)((stFMTX->u16OutScaling) & 0xFF);

            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);

            if(u8Filter > 1)
            {
                u8Filter = 1;
            }

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP, u8Filter, 0);

            //start encode
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);
        }
        break;
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
        {
            ST_AUDIO_BTSC_PARAM *stBTSC = (ST_AUDIO_BTSC_PARAM *)pvParam;
            MS_U8 u8Attenuation_H = (stBTSC->u16Attenuation) >> 8;
            MS_U8 u8Attenuation_L = (stBTSC->u16Attenuation) & 0xFF;
            MS_U8 u8OutScaling_H = (stBTSC->u16OutScaling) >> 8;
            MS_U8 u8OutScaling_L = (stBTSC->u16OutScaling) & 0xFF;
            MS_U8 u8_M_Gain_H = (stBTSC->u16_M_Gain) >> 8;
            MS_U8 u8_M_Gain_L = (stBTSC->u16_M_Gain) & 0xFF;
            MS_U8 u8_D_Gain_H = (stBTSC->u16_D_Gain) >> 8;
            MS_U8 u8_D_Gain_L = (stBTSC->u16_D_Gain) & 0xFF;
            MS_U8 u8_SAP_Gain_H = (stBTSC->u16_SAP_Gain) >> 8;
            MS_U8 u8_SAP_Gain_L = (stBTSC->u16_SAP_Gain) & 0xFF;

            MApi_AUDIO_SetSystem(eDecSysType);

            //disable Dolby SPDIF volume control, when BTSC start
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain, u8_M_Gain_H, u8_M_Gain_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain, u8_D_Gain_H, u8_D_Gain_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain, u8_SAP_Gain_H, u8_SAP_Gain_L);

            //used for keres and later BTSC supported CHIPs. Kaiser will ignore the setting, it hard code in the DSP.
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_BTSC1);
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_BTSC2);

            //start encode
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);
        }
        break;
#endif

    default:
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop audio dec
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @return TRUE: success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Stop(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    MS_BOOL bMute = TRUE;

    if(u32DevIdx > 0)
    {
        printf("unsupport device index: %d\n", (unsigned int)u32DevIdx);
        return FALSE;
    }

    if(_Demo_Audio_CheckAppSupport(eAppType) == FALSE)
    {
        //printf("unsupport application type: %u\n", (unsigned int)eAppType);
        return FALSE;
    }

    //if(MDrv_AUDIO_GET_INIT_FLAG())
    {
        switch(eAppType)
        {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            Demo_Audio_SetMute(&bMute);
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
            break;
#if (DEMO_AUDIO_AD_TEST == 1)
        case AUDIO_APP_DTV_AD:
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB2_DECCMD_STOP);
            MApi_AUDIO_SetADOutputMode(AD_OUT_NONE);
            break;
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
        case AUDIO_APP_ATV_FMTX:
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , TRUE, 0);
            break;
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
        case AUDIO_APP_ATV_BTSC:
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , TRUE, 0);
            break;
#endif
        default:
            return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to playback music
/// @param[in] pFileFmt
///     0: mp3
///     1: wav
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_demo \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Out_Init(MS_U32 *pFileFmt)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
    MS_U32 u32DevIdx=0;
    EN_AUDIO_APP_TYPE eAppType;
    EN_AUDIO_CODEC_TYPE eCodecType;
    ST_WAVINFO stWAV;
    void *pvParam = NULL;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    eAppType = AUDIO_APP_ES_PLAY;
    Demo_Audio_Init((MS_U32 *)&eAppType);

    if(_audioDemo_Task.iId == -1)
    {
        _audioDemo_Task.pStack = MsOS_AllocateMemory(_audioDemo_Task.u32StackSize, s32MstarCachedPoolID);
        if(!_audioDemo_Task.pStack)
        {
            printf("fail 1\n");
            return FALSE;
        }

        /// - Create Task Application
        _audioDemo_Task.iId = MsOS_CreateTask( _appAudioDemo_task,
                                            NULL,
                                            _audioDemo_Task.ePriority,
                                            TRUE,
                                            _audioDemo_Task.pStack,
                                            _audioDemo_Task.u32StackSize,
                                            _audioDemo_Task.szName);
        if(_audioDemo_Task.iId < 0)
        {
            printf("fail 2\n");
            return FALSE;
        }
    }

    _clipWriteLastAddr=0;
    _u32AudioFileIndex=0;
    if(*pFileFmt == 0)
    {
        //demo code for playing mp3 with using old MM mode
        eCodecType = AUDIO_CODEC_MP3;
        pAudioData = mp3_bin_var;
        AudioLen = MP3_BIN_SIZE;
    }
    else if(*pFileFmt == 1)
    {
        //demo code for playing wav with using old MM mode

        eCodecType = AUDIO_CODEC_XPCM;

        memcpy(&stWAV.u32fmtChunkSize, wav_bin_var + 16, 4);
        memcpy(&stWAV.u16FormatTag, wav_bin_var+ 20, 2);
        memcpy(&stWAV.u16Channels, wav_bin_var + 22, 2);
        memcpy(&stWAV.u32SamplesPerSec, wav_bin_var+ 24, 4);
        memcpy(&stWAV.u32AvgBytesPerSec, wav_bin_var + 28, 4);
        memcpy(&stWAV.u16BlockSize, wav_bin_var + 32, 2);
        memcpy(&stWAV.u16BitsPerSample, wav_bin_var + 34, 2);

        pAudioData = &wav_bin_var[44];
        AudioLen = WAV_BIN_SIZE;
        pvParam = (void *)&stWAV;
    }
    else if(*pFileFmt == 2)
    {
        //demo code for playing mp3 with using new MM mode
        eCodecType = AUDIO_CODEC_MP3;
        MApi_AUDIO_MM2_initAesInfo(AU_DEC_ID1);
        pAudioData = mp3_bin_var;
        AudioLen = MP3_BIN_SIZE;
        _bNewMM_Demo=TRUE;
    }
    else
    {
        return FALSE;
    }

#if 0
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif

    Demo_Audio_Play(u32DevIdx, eAppType, eCodecType, (void *)pvParam);

    bMute = FALSE;
    Demo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    Demo_Audio_SetAbsoluteVolume(&u8Volume);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Show all demo functions of audio
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b HelpAudio \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Help(void)
{
    printf("*************************Audio function List*************************\n");
    printf("1.Audio_Init --Initial Audio\n");
    printf("2.Audio_demo --Audio demo 0:mp3 1:wav\n");
    printf("3.Audio_demoStop -- Stop audio demo\n");
    printf("4.Audio_volume -- Volume control; 0~100\n");
    printf("5.Audio_mute -- mute control\n");


    return TRUE;
}

#if (DEMO_AUDIO_SOUND_EFFECT_TEST == 1)
#endif

#if (DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
#endif

#if (DEMO_AUDIO_AD_TEST == 1)
#endif

#endif


