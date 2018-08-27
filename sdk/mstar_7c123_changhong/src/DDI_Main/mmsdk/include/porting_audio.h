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

#ifndef __mmsdk_porting_audio__
#define __mmsdk_porting_audio__
#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// @file
/// @defgroup porting_audio porting_audio functions
/// @{

/// define audio multiple-instance item
typedef void* PT_AUDIOITEM;


/// XPCM Type Ref to: XPCM_TYPE_
typedef enum
{
    /// pcm codecs:lpcm
    E_MMSDK_XPCM_TYPE_LPCM                                   = 1,
    /// adpcm codecs
    E_MMSDK_XPCM_TYPE_MS_ADPCM                               = 2,
    /// pcm codecs:alaw
    E_MMSDK_XPCM_TYPE_LPCM_ALAW                               = 6,
    /// pcm codecs:mulaw
    E_MMSDK_XPCM_TYPE_LPCM_MULAW                             = 7,
    /// pcm codecs: dvd lpcm
    E_MMSDK_XPCM_TYPE_DVD_LPCM                               = 8,
    /// pcm codecs: private1 lpcm
    E_MMSDK_XPCM_TYPE_PRIVATE1_LPCM                          = 9,
    /// adpcm codecs
    E_MMSDK_XPCM_TYPE_IMA_ADPCM                              = 17,
    /// pcm codecs: apple IMA4
    E_MMSDK_XPCM_TYPE_IMA_ADPCM_APPLE                        = 18,
} EN_MMSDK_XPCM_TYPE;

/// Define the audio capability
typedef enum
{
    /// EAC3 Audio
    E_AUDIO_UNSUPPORT_CODEC_EAC3              = 0x0001,
    /// MPG/MP3 Audio
    E_AUDIO_UNSUPPORT_CODEC_MPG_AUDIO         = 0x0040,
    /// RM/COOK Audio
    E_AUDIO_UNSUPPORT_CODEC_RM_AUDIO          = 0x0080,
    /// AC3 Audio
    E_AUDIO_UNSUPPORT_CODEC_AC3               = 0x0100,
    /// AAC/EAAC Audio
    E_AUDIO_UNSUPPORT_CODEC_AAC               = 0x0200,
    /// PCM Audio
    E_AUDIO_UNSUPPORT_CODEC_PCM               = 0x0400,
    /// ADPCM Audio
    E_AUDIO_UNSUPPORT_CODEC_ADPCM             = 0x0800,
    /// WMA Audio
    E_AUDIO_UNSUPPORT_CODEC_WMA               = 0x1000,
    /// DTS Audio
    E_AUDIO_UNSUPPORT_CODEC_DTS               = 0x2000,
    /// FLAC Audio
    E_AUDIO_UNSUPPORT_CODEC_FLAC              = 0x4000,
    /// VORBIS Audio
    E_AUDIO_UNSUPPORT_CODEC_VORBIS            = 0x8000,
    /// WaveFormatEX Audio
    E_AUDIO_UNSUPPORT_CODEC_WAVEFORMATEX      = 0x10000,
    /// AMR NB Audio
    E_AUDIO_UNSUPPORT_CODEC_AMR_NB            = 0x20000,
    /// AMR WB Audio
    E_AUDIO_UNSUPPORT_CODEC_AMR_WB            = 0x40000,
    /// DRA Audio
    E_AUDIO_UNSUPPORT_CODEC_DRA               = 0x80000,
    /// DTS-LBR Audio
    E_AUDIO_UNSUPPORT_CODEC_DTS_LBR           = 0x100000,
    /// All Codec unsupport
    E_AUDIO_UNSUPPORT_CODEC_ALL               = 0xFFFFFFFFFFFFFFFFULL
} EN_AUDIO_UNSUPPORT_CODEC;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_AUDIO_START.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// The video container is TS or not. (Use Hareware TPS)
    MMSDK_BOOL bTSAudio;
    /// new audio mode (movie only)
    MMSDK_BOOL bISNewAudioMode;
    /// audio auto sync STC by itself
    MMSDK_BOOL bAutoSyncSTC;
} ST_MMSDK_AUDIO_START_INFO;


/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_WMA_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// Audio parsing by application or not.
     MMSDK_U32 u32ParsingByApp;
     /// WMA version.
     MMSDK_U32 u32Version;
     /// WMA channel number.
     MMSDK_U32 u32Channels;
     /// WMA sample rate
     MMSDK_U32 u32SampleRate;
     /// WMA byte rate
     MMSDK_U32 u32ByteRate;
     /// WMA block align number.
     MMSDK_U32 u32BlockAlign;
     /// WMA encryption information.
     MMSDK_U32 u32Encopt;
     /// Bit Per Sample.
     MMSDK_U32 u32BitPerSample;
     /// WMA Channel Mask.
     MMSDK_U32 u32ChannelMask;
     /// WMA Drc Param Exist or not.
     MMSDK_U32 u32DrcParamExist;
     /// WMA Drc Peak Amp Ref.
     MMSDK_U32 u32DrcPeakAmpRef;
     /// WMA Drc Peak Amp Target
     MMSDK_U32 u32DrcPeakAmpTarget;
     /// WMA Drc Rms Amp Ref.
     MMSDK_U32 u32DrcRmsAmpRef;
     /// WMA Drc Rms Amp Target.
     MMSDK_U32 u32DrcRmsAmpTarget;
     /// WMA file request size.
     MMSDK_U32 u32RequestSize;
     /// WMA Max Packect size.
     MMSDK_U32 u32MaxPacketSize;
     /// WMA advance encoding option
     /// @since version 1.03
     MMSDK_U32 u32AdvanceEncodeOpt;
 } ST_MMSDK_AUDIO_WMA_INIT_INFO;

 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_DTS_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// dts is littendian
     MMSDK_BOOL bIsLittleEndian;
 } ST_MMSDK_AUDIO_DTS_INIT_INFO;


 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_MP3_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// MP3 file request size.
     MMSDK_U32 u32RequestSize;
 } ST_MMSDK_AUDIO_MP3_INIT_INFO;

 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_MP2_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// MPG file request size.
     MMSDK_U32 u32RequestSize;
 } ST_MMSDK_AUDIO_MP2_INIT_INFO;

 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_AC3_EAC3_INIT.
 typedef struct
 {
    /// Media object relative information.
    void* Media;
    /// AC3 file request size.
    MMSDK_U32 u32RequestSize;
    /// Is EAC3 file
    MMSDK_BOOL bIsEAC3;
 } ST_MMSDK_AUDIO_AC3_EAC3_INIT_INFO;


 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_AAC_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// AAC sample rate.
     MMSDK_U32 u32SampleRate;
     /// AAC type.
     MMSDK_U32 u32Type;
     /// AAC file request size.
     MMSDK_U32 u32RequestSize;
     /// AAC Single Frame audio data, MM music is single frame, MM movie is multi frame for some streams.
     /// @since version 1.03
     MMSDK_BOOL bIsSingleFrame;
 } ST_MMSDK_AUDIO_AAC_INIT_INFO;


 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_FLAC_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// FLAC file request size.
     MMSDK_U32 u32RequestSize;
 } ST_MMSDK_AUDIO_FLAC_INIT_INFO;


 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_PCM_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// PCM sample rate.
     MMSDK_U16 u16SampleRate;
     /// PCM bit per sample.
     MMSDK_U16 u16BitsPerSample;
     /// PCM channel number.
     MMSDK_U8 u8ChannelNum;
     /// ADPCM block size. //PCM:0
     MMSDK_U16 u16BlockSize;
     /// PCM sample per block.
     /// 0xFF: for big endian
     /// 0x00: for little endian
     /// 0x77: Auto detect (DLNA server doesn't provide endian info)
     /// When u16SamplePerBlock is 0X77, means porting layer should auto detect, due to some application does not has endian info.
     MMSDK_U16 u16SamplePerBlock;
     /// xpcm type
     EN_MMSDK_XPCM_TYPE ePCMType;
 } ST_MMSDK_AUDIO_PCM_INIT_INFO;


 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_COOK_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// Audio codec number
     MMSDK_U16 u16CodecNum;
     /// Sample number
     MMSDK_U16 u16SamplesChannel;
     /// Sample rate
     MMSDK_U16 u16SampleRate;
     /// Number of channel per frame.
     MMSDK_U16 u16ChannelsA[5];
     /// Number of region per frame.
     MMSDK_U16 u16RegionsA[5];
     /// Start of coupling information.
     MMSDK_U16 u16cplStartA[5];
     /// Coupling Q bit.
     MMSDK_U16 u16cplQbitsA[5];
     /// Frame size.
     MMSDK_U16 u16FrameSizeA[5];
 } ST_MMSDK_AUDIO_COOK_INIT_INFO;

 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_VORBIS_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// VORBIS file request size.
     MMSDK_U32 u32RequestSize;
     /// VORBIS stream header size
     /// @since version 1.03
     MMSDK_U32 u32HeaderSize;
 } ST_MMSDK_AUDIO_VORBIS_INIT_INFO;

     /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_AMR_NB_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// AMR NB file request size.
     MMSDK_U32 u32RequestSize;
 } ST_MMSDK_AUDIO_AMR_NB_INIT_INFO;

 /// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_AMR_WB_INIT.
 typedef struct
 {
     /// Media object relative information.
     void* Media;
     /// AMR WB file request size.
     MMSDK_U32 u32RequestSize;
 } ST_MMSDK_AUDIO_AMR_WB_INIT_INFO;


/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_DRA_INIT.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// DRA file request size.
    MMSDK_U32 u32RequestSize;
} ST_MMSDK_AUDIO_DRA_INIT_INFO;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_OPUS_INIT.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// OPUS file request size.
    MMSDK_U32 u32RequestSize;
} ST_MMSDK_AUDIO_OPUS_INIT_INFO;

//-------------------------------------------------------------------------------------------------
/// Audio Initialize: malloc resource return audio intance item
/// @param pAudioItem  \b OUT: audio instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_Initialize(PT_AUDIOITEM* pAudioItem);


//-------------------------------------------------------------------------------------------------
/// Audio Finalize: free resource
/// @param pAudioItem  \b IN: audio instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_Finalize(PT_AUDIOITEM* pAudioItem);


//-------------------------------------------------------------------------------------------------
/// Allocate audio decoder.
/// @param AudioItem              \b IN: audio instance item
/// @param eCodecType           \b IN: The audio codec type.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_AllocateDecoder(PT_AUDIOITEM AudioItem, const EN_MMSDK_AUDIO_CODEC eCodecType);

//-------------------------------------------------------------------------------------------------
/// Allocate audio decoder with the real audio codec type.
/// @param AudioItem            \b IN: audio instance item
/// @param eCodecType           \b IN: The audio codec type.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_AllocateDecoderEx(PT_AUDIOITEM AudioItem, const EN_MMSDK_AUDIO_CODEC eCodecType) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Release audio decoder.
/// @param AudioItem              \b IN: audio instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_ReleaseDecoder(PT_AUDIOITEM AudioItem);

//-------------------------------------------------------------------------------------------------
/// set audio parameter
/// @param AudioItem              \b IN: audio instance item
/// @param eCodecType         \b IN: audio codec type
/// @param pstParamInfo        \b IN: input parameters
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_SetParam(PT_AUDIOITEM AudioItem, const EN_MMSDK_AUDIO_CODEC eCodecType, const void* pstParamInfo);

//-------------------------------------------------------------------------------------------------
/// Start decode
/// @param AudioItem              \b IN: audio instance item
/// @param pstAudioStart        \b IN: input parameters
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_StartDecode(PT_AUDIOITEM AudioItem, const ST_MMSDK_AUDIO_START_INFO* pstAudioStart);

//-------------------------------------------------------------------------------------------------
/// Set Stop to audio decoder.
/// @param AudioItem              \b IN: audio instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_PauseDecode(PT_AUDIOITEM AudioItem);

//-------------------------------------------------------------------------------------------------
/// Set Pause to audio decoder.
/// @param AudioItem              \b IN: audio instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_StopDecode(PT_AUDIOITEM AudioItem);

//-------------------------------------------------------------------------------------------------
/// Audio mute
/// @param AudioItem              \b IN: audio instance item
/// @param bMuteOnOff        \b IN: TRUE mute; FALSE unmute
///
/// @return none.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_Mute(PT_AUDIOITEM AudioItem, const MMSDK_BOOL bMuteOnOff);

//-------------------------------------------------------------------------------------------------
/// Get Play Time stamp.
/// @param AudioItem              \b IN: audio instance item
///
/// @return MMSDK_U32: play time stamp(ms).
//-------------------------------------------------------------------------------------------------
MMSDK_U32 PT_Audio_GetPlayTick(PT_AUDIOITEM AudioItem);




//TODO (mapi_audio.h)Old function, please don't use, it will be remove soon.

//-------------------------------------------------------------------------------------------------
/// Check file play done. If file play done, will return E_PT_OK.
/// @param AudioItem              \b IN: audio instance item
///
/// @return TREU: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_CheckPlayDone(PT_AUDIOITEM AudioItem);

//-------------------------------------------------------------------------------------------------
/// This function will inform DSP that MCU already write data to ES buffer.
/// @param AudioItem              \b IN: audio instance item
/// @param u32DataBuffSize        \b IN: data buffer size. (Requst Size by Decoder)
/// @param u32ValidDataSize       \b IN: Valid data Size (Read Size by MMSDK)
///
/// @return None
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_SetInput(PT_AUDIOITEM AudioItem, const MMSDK_U32 u32DataBuffSize, const MMSDK_U32 u32ValidDataSize);

//-------------------------------------------------------------------------------------------------
/// If MCU receive DSP interrupt, this function will return MAPI_TRUE
/// @param AudioItem              \b IN: audio instance item
/// @param pu32WrtAddr          \b IN: write pointer addr (Virtual Address)
/// @param pu32WrtBytes         \b IN: write bytes
///
/// @return TREU: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_CheckInputRequest(PT_AUDIOITEM AudioItem, MMSDK_U32 *pu32WrtAddr, MMSDK_U32 *pu32WrtBytes);

//-------------------------------------------------------------------------------------------------
/// Info DSP that the last ES data left size.
/// @param AudioItem            \b IN: audio instance item
/// @param u32DataLeft          \b IN: data left size.
///
/// @return None
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_FileEndDataHandle(PT_AUDIOITEM AudioItem, const MMSDK_U32 u32DataLeft);

//-------------------------------------------------------------------------------------------------
/// Set Audio Sync Mode.
/// @param AudioItem            \b IN: audio instance item
/// @param pstAudioStart        \b IN: input parameters
///
/// @return None
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_SetAudioSyncMode(PT_AUDIOITEM AudioItem, const ST_MMSDK_AUDIO_START_INFO* pstAudioStart)__attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Get the current deatil codec from decoder
/// Only used by ac3 and ac3p currently(for tee case)
/// @since version 1.04
/// @param AudioItem            \b IN: audio instance item
/// @param pu32InfoType         \b OUT: 0:invalid, others: codec type
///
/// @return TREU: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_GetAudioCodecType(PT_AUDIOITEM AudioItem, MMSDK_U32 *pu32InfoType) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Get the current audio unsupport
/// @since version 1.04
/// @param AudioItem            \b IN: audio instance item
/// @param pu64Info             \b OUT: 0: support, other: error code
///
/// @return TREU: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Audio_GetAudioUnsupport(PT_AUDIOITEM AudioItem, MMSDK_U64 *pu64Info) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Get the audio unsupport codec, which is the result of the bitwise or operation with
/// the unsupported codec
/// @since version 1.04
///
/// @return Audio unsupport codec
//-------------------------------------------------------------------------------------------------
MMSDK_U64 PT_Audio_GetUnsupportAudioCodec(void) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Trigger system to set audio delay
/// 1. After set window
/// 2. After init ok in audio only case
/// @param AudioItem            \b IN: audio instance item
///
/// @return None
//-------------------------------------------------------------------------------------------------
void PT_Audio_SetAudioDelay(PT_AUDIOITEM AudioItem) __attribute__ ((weak));

/// @} // end of porting_audio


#ifdef __cplusplus
}
#endif
#endif // __mmsdk_porting_audio__

///*** please do not remove change list tag ***///
///***$Change: 1460642 $***///
