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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

#ifndef MAPP_MUSIC_H
#define MAPP_MUSIC_H

//-------------------------------------------------------------------------------------------------
// Standard include files:
//-------------------------------------------------------------------------------------------------

#include "MsMMTypes.h"
#include "msapi_MM_Audio.h"
#include "MApp_UiMediaPlayer_Define.h"
#include "msapi_MM_fs.h"
#include "mapp_photo.h"


// For MM. This define should be in bd_xxx.h. Define it here for test.
//#define ENABLE_MPLAYER_EXTRA_FLASH  0 //enable it to store LOGO and power-on music in 2nd flash(SPI_CZ1)

#ifdef MAPP_MUSIC_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifndef M4APARSER
#define M4APARSER
#endif

#ifndef OGG_PARSER
#define OGG_PARSER
#endif

#ifdef M4APARSER
#define M4APARSER_DBG
#define AAC_DBG(x) //x
#define AAC_ERR(x) //x
#define AAC_ERR2(x) //x
#define AAC_ERR3(x) //x

#define MP3_TOC_LEN 100
#define STSC_ENTRY_LIMIT 3
#define MP3_LYC_DISPLAY_LEN 512

#define FLAC_PARSE_BUFFER_SIZE       0x4000


typedef struct{
    U8 u8CurrentEntry;
    U32 u32TotalEntry;
    U32 u32EntryInfo[STSC_ENTRY_LIMIT][3]; //[0]:first_chunk      [1]:samples_per_chunk   [2]:Total samples
} STSC_INFO;

typedef struct   {
    U32 dwstsdSampleRate;
    U32 dwmdatLocation;
    U32 dwmdatSize;
    BOOLEAN bFoundstsd;
    BOOLEAN bFoundmdat;
    BOOLEAN bFoundmeta_hdlr;
    U8 u8SampleRateIndex;
    U8 u8channelConfiguration;
    U32 dwFileSize;
    BOOLEAN bM4aHeader;
    U32 u32stcoEntry;
    BOOLEAN    bFoundstco;
    U32 u32stcoFrameLocation;
    BOOLEAN bFoundstsz;
    U32 u32stszLocation;
    U32 u32stszSize;
    BOOLEAN bFoundstsc;
    U32 u32stscLocation;
    U32 u32stscSize;
    STSC_INFO stscInfo;
    //U32 u32RecognizedFrameNumber;
    BOOLEAN bFrameJump;
    U32 u32Duration;
    BOOLEAN bFoundesds;
    U32 u32STDuration;
} BoxInfo;

/// Flac Seek table
typedef struct
{
    /// Sample number of first sample in the target frame,
    /// or 0xFFFFFFFFFFFFFFFF for a placeholder point.
    MS_U64 u64SampleNum;
    /// Offset (in bytes) from the first byte of the first frame header to the first byte of the target frame's header.
    MS_U64 u64FrameOffset;
    /// Number of samples in the target frame.
    MS_U16 u16SamplesOfFrame;
}FLAC_SEEK_TABLE;

/// Flac info
typedef struct
{
    /// offset of the First valid audio data
    MS_U64 u64DataOffset;
    /// Total samples in stream. 'Samples' means inter-channel sample,
    /// i.e. one second of 44.1Khz audio will have 44100 samples regardless of the number of channels.
    /// A value of zero here means the number of total samples is unknown.
    MS_U64 u64TotalSamples;

    FLAC_SEEK_TABLE *pSeekTable;
    MS_U32 u32SeekEntryCount;
} ST_FLAC_INFO;

typedef struct
{
    U16 u16Channel;//now, for ogg only
    U16 u16BitRate;
    U16 u16SampleRate;
    U8 u8Flag;
    EN_MP3_VERSION enVersion;
    EN_MP3_LAYER enLayer;

    BOOLEAN bID3TagV1;
    U16 u16ID3TagV1Size;
    BOOLEAN bID3TagV2;
    U32 u32ID3TagV2Size;
    BOOLEAN bCheckFfFb;
    MP3_INFO stID3v1;
    MP3_INFO stID3v2;

    U32 u32FileSize;
    U32 u32MusicDataBytesLeft;
    U32 u32Duration;
    U32 u32CurrentPlayTime;
    U32 u32PlayTimer;
    U16 u16TOCLength;
    U8 u8TOC[MP3_TOC_LEN]; // Table of Contents for VBR file seek

    ST_FLAC_INFO stFLAC;
} ST_MP3_INFO;


typedef enum
{
    E_MUSIC_ERR_NONE = 0,
    E_MUSIC_ERR_NOT_SUPPORT_CHANNEL_NUM,
    E_MUSIC_ERR_NOT_SUPPORT_SAMPLE_RATE,
    E_MUSIC_ERR_DECODE_HEADER_FAILURE,
    E_MUSIC_ERR_DECODE_FAILURE,
    E_MUSIC_ERR_DATA_OPERATION_FAILURE,
} EN_MUSIC_ERRCODE;

INTERFACE BOOLEAN M4AParser_Get_M4A_Info(BoxInfo *boxInfo);
U8 * M4AParser_ExtractAtomName(const U8 *fileData);
U32 M4AParser_GetSampleRate(const U8 *buffer);
U32 M4AParser_GetMdatSize(const U8 *buffer);
U32 M4AParser_DealPostfix(U32 nIndex, const U8 *buffer, const U8 * fixbuffer);
U8  * M4AParser_ExtractFix(U8 *fileData);
BOOLEAN M4AParser_DealEdge(const U8 *buffer, const U8 *postfix, U32 dwLocation);
U8 M4AParser_GetSampleRateIndex(U32 dwValue);
BOOLEAN M4AParser_WriteDSP(U32 dwReqAddress, U32 u32ReqBytes);
BOOLEAN M4AParser_WriteDSPNoHead(U32 dwReqAddress, U32 u32ReqBytes);
U32 M4AParser_DealPostfixstco(U32 u32Index, const char *buffer, const char * fixbuffer);
U32 M4AParser_SetCurrentFrameIndex(void);
BOOLEAN M4AParser_SetFrameIndexbyBtn(U8 u8Mode);
U8  * M4AParser_ExtractFixFromXdata(U32 u32PostSaveLocation);
BOOLEAN M4AParser_SaveFrameLocation(void);
U32 M4aParser_GetOneStcoEntryFromDRAM(U32 u32Index);
U32 M4aParser_GetOneStszEntryFromDRAM(U32 u32Index);

#endif

#ifdef OGG_PARSER
#define OGGPARSER_DBG
#define OGG_INFO(x) x
#define OGG_DBG(x) //x
#define OGG_ERR(x) x
#endif
//WAV
#define WAV_DBG(x) //x
#define WAV_ERR(x) //x

#define RIFF_TAG_RIFF    0x52494646 // 'RIFF'
#define RIFF_TAG_WAVE    0x57415645 // 'WAVE'
#define RIFF_TAG_fmt    0x666D7420 // 'fmt '
#define RIFF_TAG_data    0x64617461 // 'data'

typedef enum
{
    WAVE_FORMAT_PCM = 0x0001, //PCM
    WAVE_FORMAT_M_ADPCM = 0x0002, //M ADPCM
    WAVE_FORMAT_IEEE_FLOAT = 0x0003, //IEEE float
    WAVE_FORMAT_ALAW = 0x0006, //8-bit ITU-T G.711 A-law
    WAVE_FORMAT_MULAW = 0x0007, //8-bit ITU-T G.711 £g-law
    WAVE_FORMAT_IMA_ADPCM = 0x0011, //8-bit ITU-T G.711 £g-law
    WAVE_FORMAT_EXTENSIBLE = 0xFFFE, //Determined by SubFormat
} EN_WAV_FORMAT_CODE;

typedef struct{
    //ckID;
    U32 u32HeaderSize;
    U32 u32RIFFChunkSize;
    U32 u32fmtChunkSize;
    U32 u32DataChunkSize;
    U16 u16FormatTag;
    U16 u16Channels;
    U32 u32SamplesPerSec;
    U32 u32AvgBytesPerSec;
    U16 u16BlockSize;
    U16 u16BitsPerSample;
    U16 u16CExtensionSize;
    BOOLEAN bIsDTS;
    BOOLEAN bDTSIsLittleEndian;
    //U16 u16ValidBitsPerSample;
    //dwChannelMask;
    //SubFormat;
} ST_WAV_CHUNK;

typedef struct{
    U16 u16FormatTag;
    U16 u16Channels;
    U32 u32SamplesPerSec;
    U32 u32AvgBytesPerSec;
    U16 u16BlockSize;
    U16 u16BitsPerSample;
    BOOLEAN bBigEndian;
    BOOLEAN bIsConfig;
} ST_PCM_PARAM;

//-------------------------------------------------------------------------------------------------
// Project include files:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
//#if(ENABLE_MPLAYER_EXTRA_FLASH)
//#define USER_MP3_FLASH_START_ADDR   (0x40000)
//#else
//#define USER_MP3_FLASH_START_ADDR   (0x3C0000)
//#endif


#define USER_MP3_FLASH_END          (USER_LOGO_FLASH_START_ADDR-0x1000)//(FLASH_SIZE)
#define USER_MP3_CAPTURE_TIMEOUT    (8000)

typedef enum
{
    MP3_STATUS_STOP,
    MP3_STATUS_PLAYING,
    MP3_STATUS_PAUSE,
    MP3_STATUS_SCAN,
} MP3_PLAY_STATUS;

typedef enum
{
    STATE_MUSIC_INIT =0,
    STATE_MUSIC_OPEN_FILE,
    STATE_MUSIC_INPUT,
    STATE_MUSIC_WAIT_DECODE_DONE,
    STATE_MUSIC_WAIT,
    STATE_MUSIC_EXIT,
    STATE_MUSIC_SKIP_PLAY_NEXT,
    STATE_MUSIC_SKIP_PLAY_PREV,
    STATE_MUSIC_NONE,
} EN_MUSIC_STATE;

typedef enum
{
    MP3_RETURN_SUCCESS,
    MP3_RETURN_ERR_FILE,
    MP3_RETURN_ERR_NO_ID3,
} EN_MP3_RETURN;

#ifdef NEW_VDPLAYER
typedef enum
{
    MP3_INFO_LAYER,
    MP3_INFO_BITRATE,
    MP3_INFO_SAMPLINGRATE,
    MP3_INFO_ALBUM,
    MP3_INFO_ALBUM_FORMAT,
    MP3_INFO_TITLE,
    MP3_INFO_TITLE_FORMAT,
    MP3_INFO_ARTIST,
    MP3_INFO_ARTIST_FORMAT,
    MP3_INFO_YEAR,
    MP3_INFO_YEAR_FORMAT,
    MP3_INFO_GENRE,
    MP3_INFO_GENRE_FORMAT,
    MP3_INFO_HAS_PICTURE,
    MP3_INFO_PICTURE_OFFSET,
    MP3_INFO_PICTURE_SIZE,
    MP3_INFO_COMMENT,
    MP3_INFO_CHECK_FF_FB,
    MP3_INFO_END,
} EN_MP3_INFO;
#else
typedef enum
{
    MP3_INFO_LAYER,
    MP3_INFO_BITRATE,
    MP3_INFO_SAMPLINGRATE,
    MP3_INFO_ALBUM,
    MP3_INFO_TITLE,
    MP3_INFO_ARTIST,
    MP3_INFO_YEAR,
    MP3_INFO_GENRE,
    MP3_INFO_COMMENT,
    MP3_INFO_CHECK_FF_FB,
    MP3_INFO_END,
} EN_MP3_INFO;
#endif

typedef enum
{
    EN_MP3_SPEED_X1 = 0,
    EN_MP3_SPEED_X2,
    EN_MP3_SPEED_X4,
    EN_MP3_SPEED_X8,
    EN_MP3_SPEED_X16,
    EN_MP3_SPEED_X32,
    EN_MP3_SPEED_MAX = EN_MP3_SPEED_X32,
} EN_MP3_SPEED;

typedef enum
{
    EN_MUSIC_MP3,              /// MP3
    EN_MUSIC_AAC,              /// AAC
    EN_MUSIC_WAV,              /// WAV
    EN_MUSIC_LOAS,              /// LOAS
    EN_MUSIC_PCM,              /// PCM
#ifdef OGG_PARSER
    EN_MUSIC_OGG,              /// OGG
#endif
    EN_MUSIC_FLAC,              /// FLAC
    EN_MUSIC_NUM,              /// Total Number
} EN_MUSIC_TYPE;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct
{
    U32 u32Time;
    U16 u16LyricPos;
} Music_Lyric_Tag;
#if ENABLE_LYRIC
typedef enum
{
    EN_ANSI,
    EN_UTF_8,
    EN_UNICODE,
}EN_CHARACTER_CODING_TYPE;

#endif

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Extern Functions
///-------------------------------------------------------------------------------------------------
INTERFACE void MApp_Music_SetFileEntry(FileEntry *pEntry);
INTERFACE EN_RET MApp_Music_Main(EN_MUSIC_TYPE enMusicType);
//INTERFACE BOOLEAN MApp_Music_ProcessKey (void);
INTERFACE EN_MP3_RETURN MApp_Music_GetInfo(MP3_INFO *pMp3InfoPtr);
INTERFACE U32 MApp_Music_GetTotalTime(void);
INTERFACE U16 MApp_Music_GetCurrentTime(void);
INTERFACE U32 MApp_Music_GetCurrentTimeMs(void);
INTERFACE U32 MApp_Music_GetPlayTick(void);
#if ENABLE_LYRIC
INTERFACE BOOLEAN MApp_Music_LyricInit(void);
INTERFACE BOOLEAN MApp_Music_LyricUnInit(void);
INTERFACE BOOLEAN MApp_Music_GetLyricTag(Music_Lyric_Tag *pLyricTag, U32 u32Time);
INTERFACE BOOLEAN MApp_Music_LRC_Lyrics_Parser(U32 u32FileSize);
INTERFACE MS_BOOL MApp_Music_Lyric_PollingDisplay(U32 LRCCurTime);
INTERFACE MS_U16* MApp_Lyric_GetString(void);
INTERFACE EN_CHARACTER_CODING_TYPE MApp_Lyric_GetCharacterCodingType(void);
INTERFACE MS_BOOL MApp_Lyric_GetLyricDisplay(void);
INTERFACE void MApp_Lyric_SetLyricDisplay(MS_BOOL LyricDisplay);
#endif
INTERFACE BOOLEAN MApp_Music_PCM_SetInfo(U16 u16FormatTag, U16 u16Channels, U32 u32SamplesPerSec, U16 u16BitsPerSample, BOOLEAN bBigEndian);
INTERFACE void MApp_Music_Stop(void);
INTERFACE BOOLEAN MApp_Mp3_Goto(U16 u16Time);
INTERFACE void MApp_Music_Pause(void);
INTERFACE void MApp_Music_Resume(void);
//INTERFACE void MApp_Music_DrawEQ(void);
INTERFACE U8* MApp_Music_GetInfoString(EN_MP3_INFO enInfo);
INTERFACE U8 MApp_Music_GetInfoStringLength(EN_MP3_INFO enInfo);
INTERFACE void MApp_Music_StartCapture(void);
INTERFACE void MApp_Music_StopCapture(void);

#if 0
INTERFACE void MApp_Music_KeySoundEffect_Initial(void);
INTERFACE void MApp_Music_KeySoundEffect_Play(void);
INTERFACE void MApp_Music_KeySoundEffect_Exit(void);
#endif

INTERFACE BOOLEAN MApp_Music_ProcessTimeOffset(U32 u32GotoTimeMs);
INTERFACE BOOLEAN MApp_Music_FastForwardBackward(BOOLEAN bForward, EN_MP3_SPEED eSpeed);
#if 0//(ENABLE_POWERON_MUSIC)
INTERFACE void MApp_Music_StateInit(void);
INTERFACE BOOLEAN MApp_Music_Decode(void);
INTERFACE void MApp_Music_StopDecode(void);
INTERFACE U32 MApp_Music_GetRecordingTime(void);
#endif
INTERFACE EN_MUSIC_ERRCODE MApp_Music_GetErrCode(void);
INTERFACE void MApp_Music_Config(BOOLEAN bEnabledStopAfterFBBegin, BOOLEAN bEnableStreamMode, BOOLEAN bSeekable);
INTERFACE void MApp_Music_GetRIFFTag(U8* buf, U32* tag, U32* tagsize);
INTERFACE void MApp_Music_InputBackGroundWavData(void);
INTERFACE EN_MP3_SPEED MApp_Music_GetFfRewSpeed(void);
INTERFACE U8 MApp_Music_GetFFFWMode(void);
INTERFACE MS_BOOL MApp_Mp3_ReadId3ImageInfo(void);
INTERFACE MS_U16 MApp_MP3_GetMp3BitRate(void);
INTERFACE void MApp_Music_MallocMemoryForHighRateMP3(void);
INTERFACE void MApp_Music_FreeMemoryForHighRateMP3(void);
INTERFACE void MApp_Music_FlacMemoryInit(void);
INTERFACE void MApp_Music_FreeMemory_Flac(void);



#undef INTERFACE

#endif // _MAPP_SUBTITLE_H

