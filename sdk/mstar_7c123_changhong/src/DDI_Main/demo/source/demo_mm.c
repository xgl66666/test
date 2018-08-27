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
/// @file   demo_mm_mmsdk.c
/// @author MStar Semiconductor Inc.
/// @brief Multi-Media file playback control Demo
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#if (DEMO_MM_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include<ctype.h>
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include <string.h>
#include <errno.h>
#include "Board.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "drvAUDIO.h"
#include "apiDMX.h"
#include "apiHDMITx.h"
#include "apiVDEC_EX.h"

#include "demo_utility.h"
#include "demo_osd.h"
#include "demo_audio.h"

#if (DEMO_APM_TEST == 1)
#include "demo_dmx.h"
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiPNL_EX.h"
#else
#include "apiPNL.h"
#endif

#include "mmsdk_interface_def.h"
#include "mmsdk_interface.h"
#include "msapi_MM_Common.h"
#include "msapi_MM_Display.h"

#if (DEMO_MM_AVP_TEST ==1)
#include "drvMVOP.h"
#include "msAPI_XC_EX.h"
#endif

#include "porting_sysinfo.h"
#include <sys/stat.h>
#include "MsFS.h"
#include "demo_mm.h"
#include <pthread.h>
#include <assert.h>
#include "MM_Player_Def.h"
#include "MApi_MEMConfig_Interface.h"
#include "drvDTC.h"

#define MM_PRINT(fmt, args...) printf("[%"DTC_MS_U32_d"] %15s(%5d) " fmt, MsOS_GetSystemTime(), __FUNCTION__, __LINE__, ##args)

#define MM_MUST(fmt, args...)   do{if(1) MM_PRINT(fmt, ##args);}while(0)
#define MM_ERR(fmt, args...)    do{if( _geDebugLevel >= E_MMSDK_DBG_LEVEL_ERR )        MM_PRINT(fmt, ##args);}while(0)
#define MM_INFO(fmt, args...)   do{if( _geDebugLevel >= E_MMSDK_DBG_LEVEL_INFO )       MM_PRINT(fmt, ##args);}while(0)
#define MM_DBG(fmt, args...)    do{if( _geDebugLevel >= E_MMSDK_DBG_LEVEL_DBG )        MM_PRINT(fmt, ##args);}while(0)
#define MM_TRACE(fmt, args...)  do{if( _geDebugLevel >= E_MMSDK_DBG_LEVEL_TRACE )      MM_PRINT(fmt, ##args);}while(0)

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL b_MM_Initied = FALSE;

extern unsigned char Customer_info[];
extern MS_U8 Customer_hash[];
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static void _Demo_Photo_EvtProcess(MS_U32 argc, void *arg);
static void _Demo_Music_EvtProcess(MS_U32 argc, void *arg);
static void _Demo_Video_EvtProcess(MS_U32 argc, void *arg);

static void _Demo_MM_InitESDemuxerParameter(void);
static void _Demo_MM_DeInitESDemuxerParameter(void);
static MS_BOOL _Demo_MM_CreateParseEsTask(void);
static MS_BOOL _Demo_MM_DeleteParseEsTask(void);

static MS_U32 _Demo_MM_GetAliveMediaItem(void);
static MS_U32 _Demo_MM_GetEmptyMediaItem(const EN_MMSDK_MEDIA_TYPE eMediaType);
static MS_U32 _Demo_MM_GetMediaType(MS_U32 u32Idx);
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/// DivX DRM model ID //0x3130 is default ID ,Divx for Mstar
#ifndef ASSERT
#define ASSERT(x)  assert(x)
#endif
#define MMSDK_MEDIA_TYPE_MIN (E_MMSDK_MEDIA_TYPE_PHOTO)
#define MMSDK_MEDIA_TYPE_NUM (E_MMSDK_MEDIA_TYPE_MOVIE+1)
#define MMSDK_MEDIA_TYPE_INVALID 0xFFFFFFFF

#define MMSDK_MEDIA_ITEM_MIN 0
#if (DEMO_MM_AVP_TEST ==1)
#define MMSDK_MEDIA_ITEM_MAX 4
#else
#define MMSDK_MEDIA_ITEM_MAX 1
#endif
#define MMSDK_MEDIA_ITEM_INVALID 0xFFFFFFFF


#define DRM_MODEL_ID                    0x3130
#define MAX_PARA_SIZE                   50

#define FILE_NAME_LENGTH                1024
#define EXTENTION_NAME_LENGTH          10

#define INVALID_UNICODE                 0xFFFF
#define MM_FILL_MEM_INFO(_buf, _addr, _len, _mtype) \
    do{ \
        _buf.u32VirtualAddr = MS_PA2KSEG1((MS_U32)MEM_ADR_BY_MIU(_addr, _mtype)); \
        _buf.u32Size = _len; \
        _buf.bIsMiu0 = (_mtype & MIU1) ? FALSE : TRUE; \
        _buf.u32MiuNo = (_buf.bIsMiu0 == TRUE) ? 0 : 1; \
        _buf.u32Addr = (_buf.bIsMiu0 == TRUE) ? _addr : (_addr | MIU_INTERVAL); \
    }while(0)

#define MM_GET_OPTION(stParam, Option, SubOptioin, ParamSize, Param, Item) \
    do{ \
        stParam.u32Option = Option; \
        stParam.u32SubOption = SubOptioin; \
        stParam.u32Paramsize = ParamSize; \
        stParam.pParam = Param; \
        MMSDK_GetOptionByPara(Item, &stParam, 1); \
    }while(0)

#define MM_SET_PHOTO_NOTIFY_TYPE(PhotoNotiyType) \
    do{ \
        MsOS_ObtainMutex(_gs32Mutex, MSOS_WAIT_FOREVER); \
        _gePhotoNotifyType = PhotoNotiyType; \
        MsOS_ReleaseMutex(_gs32Mutex); \
        MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_NOTIFY); \
    }while(0)

#define MM_SET_MUSIC_NOTIFY_TYPE(MusicNotiyType) \
    do{ \
        MsOS_ObtainMutex(_gs32Mutex, MSOS_WAIT_FOREVER); \
        _geMusicNotifyType = MusicNotiyType; \
        MsOS_ReleaseMutex(_gs32Mutex); \
        MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_NOTIFY); \
    }while(0)

#define MM_SET_VIDEO_NOTIFY_TYPE(VideoNotiyType, Idx) \
    do{ \
        MsOS_ObtainMutex(_gs32Mutex, MSOS_WAIT_FOREVER); \
        _geVideoNotifyType = VideoNotiyType; \
        MsOS_ReleaseMutex(_gs32Mutex); \
        MsOS_SetEvent(_gs32MediaEvtGrp, (MM_EVENT_VIDEO_NOTIFY ^ _Demo_MM_GetEventIndex(Idx))); \
    }while(0)

#define MM_SET_ES_STREAM_NOTIFY_TYPE(EsStreamNotiyType) \
    do{ \
        MsOS_ObtainMutex(_gs32Mutex, MSOS_WAIT_FOREVER); \
        _geEsStreamNotifyType = EsStreamNotiyType; \
        MsOS_ReleaseMutex(_gs32Mutex); \
        MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_ES_NOTIFY); \
    }while(0)

#define MM_SET_MM_PLAY_STATUS(Status, Idx) \
    do{ \
        MsOS_ObtainMutex(_gs32Mutex, MSOS_WAIT_FOREVER); \
        _gePlayStatus[Idx] = Status; \
        MsOS_ReleaseMutex(_gs32Mutex); \
        MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS); \
    }while(0)

#define MM_WAIT_EVENT(EvtGroupID, EvtGroupFlag) \
    do { \
        MS_U32  pu32RetrievedEventFlag; \
        MsOS_WaitEvent(EvtGroupID, EvtGroupFlag, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER); \
    }while(0)


#define SETU16(ptr, x, data) \
        (ptr)[(x)] = (data) & 0xFF; \
        (ptr)[(x)+1] = ((data) >> 8) & 0xFF;

#define SETU32(ptr, x, data) \
        (ptr)[(x)] = (data) & 0xFF; \
        (ptr)[(x)+1] = ((data) >> 8) & 0xFF; \
        (ptr)[(x)+2] = ((data) >> 16) & 0xFF; \
        (ptr)[(x)+3] = ((data) >> 24) & 0xFF;

#define B_ENDIAN2L_ENDIAN16(ptr)                                \
    _gu8Tmp_a = *((MS_U8*)(ptr)+1);          \
    *((MS_U8*)(ptr)+1) = *(MS_U8*)(ptr);      \
    *(MS_U8*)(ptr) = _gu8Tmp_a;              \

#define B_ENDIAN2L_ENDIAN32(ptr)                                \
    _gu8Tmp_a = *((MS_U8*)(ptr));            \
    _gu8Tmp_b = *((MS_U8*)(ptr)+1);          \
    *((MS_U8*)(ptr)) =*((MS_U8*)(ptr)+3);     \
    *((MS_U8*)(ptr)+1) =*((MS_U8*)(ptr)+2);   \
    *((MS_U8*)(ptr)+2) = _gu8Tmp_b;          \
    *((MS_U8*)(ptr)+3) = _gu8Tmp_a;          \


#define CASE_FOR_STR(x) case x:return (MS_U8 *)#x

#define MMSDK_BUF_PHOTO_MEMORY_POOL_LEN 0x0000100000
#if (MMSDK_BUF_PHOTO_MEMORY_POOL_LEN > JPD_INTER_BUF_LEN)
    #error "MMSDK_BUF_PHOTO_MEMORY_POOL_LEN is larger than MMSDK_BUF_PHOTO_PROGRESSIVE, so the memory pool cannot be reconfigured properly when playing GIF file"
#endif

#ifndef VDPLAYER_BS_EXT_AVAILABLE
    #if (DEMO_HD_TEST != 1)
    #define MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO (0x0000400000/2)
    #else
    #define MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO 0x0000400000
    #endif

    #define BITSTREAM_AUDIO_BUFFER_ALIGNMENT 0x1000
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
#define IPANEL(dev, func) g_IPanelEx.func(dev)
#else
#define IPANEL(dev, func) g_IPanel.func()
#endif

#define EXT_INFO_PARAM_NUM 7
#define INVALID_PLAY_FLAG 0x00000000
#define MASK_FILE_MODE 0xF
#define MASK_AVP_PLAY_FLAG 0xF0

// For ES player
#define ES_PKT_HEADER_SIZE      16          // ES_PKT_HEADER_SIZE
#define ES_VDP_SUPPORT_VIDEO    0x1000000   // E_VDP_CODEC_ID_SUPPORT_VIDEO
#define ES_VDP_SUPPORT_AUDIO    0x2000000   // E_VDP_CODEC_ID_SUPPORT_AUDIO
#define ES_CODECINFO_HEADER     0x0002      // CODECINFO_HEADER
#define ES_VIDEO_PARAM_SIZE     32          // E_SHAREMEM_ES_VIDEO_PARAM_SIZE
#define ES_AUDIO_PARAM_SIZE     32          // E_SHAREMEM_ES_AUDIO_PARAM_SIZE
#define GetByte3(x)         (((x)&0xff000000)>>24)
#define GetByte2(x)         (((x)&0x00ff0000)>>16)
#define GetByte1(x)         (((x)&0x0000ff00)>>8)
#define GetByte0(x)         ((x)&0x000000ff)

#define ESBUFFER_SIZE 64
#define ES_BUFFER_CLEAR() \
{ \
    memset(_gstESBuffer, 0x00, ESBUFFER_SIZE * sizeof(ST_MMSDK_ES_DATA_INFO)); \
    _gu8ESBufferWptr = 0; \
    _gu8ESBufferRptr = 0; \
    _gbESBufferFull = FALSE; \
}
#define ES_BUFFER_IS_EMPTY  (_gu8ESBufferRptr == _gu8ESBufferWptr)
#define ES_BUFFER_IS_FULL	_gbESBufferFull
#define ES_BUFFER_CURRENT_POP (_gstESBuffer[_gu8ESBufferRptr])
#define ES_BUFFER_CURRENT_POP_DONE() \
{ \
    if(_gbESBufferFull) \
    { \
        _gbESBufferFull = FALSE; \
    } \
    _gu8ESBufferRptr++; \
    _gu8ESBufferRptr %= ESBUFFER_SIZE; \
}
#define ES_BUFFER_CURRENT_PUSH _gstESBuffer+ _gu8ESBufferWptr
#define ES_BUFFER_CURRENT_PUSH_DONE() \
{ \
    _gu8ESBufferWptr++; \
    _gu8ESBufferWptr %= ESBUFFER_SIZE; \
    if(_gu8ESBufferRptr == _gu8ESBufferWptr) \
    { \
        _gbESBufferFull = TRUE; \
    } \
}

#define MM_ES_FLUSH_DATA(MediaIdx, MediaType, SeekTimeMs, DataType) \
do{ \
    _gu64CurPostn[MediaIdx][MediaType] = (ES_AUDIO_PARAM_SIZE + ES_VIDEO_PARAM_SIZE) * 4; \
    _gu32SeekPTS = SeekTimeMs; \
    _gbStartPush = FALSE; \
    _gbWaitEsFlushDataReady = TRUE; \
    _gu32LastAudioPTS = 0xFFFFFFFF; \
    MMSDK_ES_Flush(_gItem[MediaIdx][MediaType], DataType); \
}while(0)


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static EN_MMSDK_MEDIA_TYPE _geMediaType = E_MMSDK_MEDIA_TYPE_MOVIE;
static MS_U8* _gpu8FileName = NULL;
static MS_U8 _gu8IsRepeat = FALSE;

static const Z_File_Ext_Info _gstZmmSupportExtTable[] =
{
    //Auio support Suffixes name
    {"AAC",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"M4A",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"MP3",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"WMA",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"OGG",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"WAV",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"PCM",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"AC3",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"FLAC",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"EC3",      E_MMSDK_MEDIA_TYPE_MUSIC},

    //Video support Suffixes name
    {"MP4",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPG",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"AVI",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DAT",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"VOB",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MOV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MKV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DIV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DIVX",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TS",       E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPEG",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DAT",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPA",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"M2TS",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"M2T1",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPE",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TRP",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TP",       E_MMSDK_MEDIA_TYPE_MOVIE},
    {"3GP",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"RMVB",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"RM",       E_MMSDK_MEDIA_TYPE_MOVIE},
    {"FLV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"ASF",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"WMV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"WEBM",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"SWF",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"SDP",      E_MMSDK_MEDIA_TYPE_MOVIE},

    //Photo support Suffixes name
    {"BMP",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"PNG",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPG",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPEG",     E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPE",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"GIF",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"MPO",      E_MMSDK_MEDIA_TYPE_PHOTO},
};

typedef struct
{
    char fileID[4];
    MS_U32 fileleth;
    char wavTag[4];
    char FmtHdrID[4];
    MS_U32  FmtHdrLeth;
    MS_U16 FormatTag;
    MS_U16 Channels;
    MS_U32 SamplesPerSec;
    MS_U32 AvgBytesPerSec;
    MS_U16 BlockAlign;
    MS_U16 BitsPerSample;
    char DataHdrID[4];
    MS_U32  DataHdrLeth;
} WAVE_HDR;

#if (DEMO_XC_DUALXC_TEST == 1)
static PNL_DeviceId gDevId = {0,E_PNL_EX_DEVICE0};
#endif

static WAVE_HDR _gstWaveHdr;
static MS_U32 _gu32SaveFilePos = 0;

static MS_BOOL _gbKTVMode = FALSE;
static MS_S32 _gs32MutexOpenStop = -1;
static MS_S32 _gs32Mutex = -1;
static MS_S32 _gs32MutexESPlayer = -1;

//static MS_S32 condVideoNotifyTypeValid = -1;
//static MS_S32 condMusicNotifyTypeValid = -1;
//static MS_S32 condPhotoNotifyTypeValid = -1;
static MS_S32 _gs32MediaEvtGrp = -1;

#define MM_EVENT_VIDEO_NOTIFY        0x00000001
#define MM_EVENT_MUSIC_NOTIFY        0x00000002
#define MM_EVENT_PHOTO_NOTIFY        0x00000004
#define MM_EVENT_ES_NOTIFY           0x00000008
#define MM_EVENT_VIDEO_PROC_EXIT     0x00000010
#define MM_EVENT_MUSIC_PROC_EXIT     0x00000020
#define MM_EVENT_PHOTO_PROC_EXIT     0x00000040
#define MM_EVENT_ES_PROC_EXIT        0x00000080
#define MM_EVENT_MM_PLAY_STATUS      0x00000100
#define MM_EVENT_ES_DATA_FLUSH_READY 0x00000200


#define MM_EVENT_VIDEO_ITEM_1        0x00001001
#define MM_EVENT_VIDEO_ITEM_2        0x00002001
#define MM_EVENT_VIDEO_ITEM_3        0x00004001
#define MM_EVENT_VIDEO_ITEM_4        0x00008001
#define MM_EVENT_VIDEO_ITEM_INVALID  0x00000001

#define MM_MAX_ZORDER_LAYER 100

static EN_MPLAYER_NOTIFY_TYPE _geVideoNotifyType = MPLAYER_NOTIFY_NULL;
static EN_MPLAYER_NOTIFY_TYPE _geMusicNotifyType = MPLAYER_NOTIFY_NULL;
static EN_MPLAYER_NOTIFY_TYPE _gePhotoNotifyType = MPLAYER_NOTIFY_NULL;
static EN_MPLAYER_NOTIFY_TYPE _geEsStreamNotifyType = MPLAYER_NOTIFY_NULL;

#if defined(MSOS_TYPE_ECOS)
#define TH_EVT_PROC_STACK_SIZE (8*(16<<10))
#elif defined(MSOS_TYPE_LINUX)
#define TH_EVT_PROC_STACK_SIZE 0 //use pthread default stack
#endif

static void (*_gpEvtProcess[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM])(MS_U32, void*);
static Task_Info _gThreadEvtProcess[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_U64 _gu64Size[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_U64 _gu64CurPostn[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_U64 _gu64AudioCurPostn[MMSDK_MEDIA_ITEM_MAX];
static MS_U64 _gu64Audio2CurPostn[MMSDK_MEDIA_ITEM_MAX];
static MS_U64 _gu64SubCurPostn[MMSDK_MEDIA_ITEM_MAX];
static void *_gpStream[MMSDK_MEDIA_ITEM_MAX][E_MPLAYER_MEDIA_STREAM_NUM];
static MEDIAITEM _gItem[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_BOOL _gbPlay[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_BOOL _gbExit[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_BOOL _gbESPlay[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static EN_MMSDK_TRICK_MODE _geTrickMod[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static MS_U32 _gu32FilePlayFlag[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static ST_RECT _gstRect[MMSDK_MEDIA_ITEM_MAX][MMSDK_MEDIA_TYPE_NUM];
static ST_RECT _gstDipRect[MMSDK_MEDIA_ITEM_MAX];
static EN_MM_PLAY_STATUS _gePlayStatus[MMSDK_MEDIA_ITEM_MAX];

static ST_MMSDK_CREATE_MEDIA_INFO _gstMedia;

//static MS_BOOL _gbSeekRightNow[MMSDK_MEDIA_TYPE_NUM] = { FALSE,FALSE,FALSE};
static MS_BOOL _gbUseNewAudioMode = TRUE;
static MS_BOOL _gbEnableFastForward = FALSE;
static MS_BOOL _gbKeepRatio = TRUE;

static MS_U32 _gu32Recursion = 0;
static MS_S32 _gs32Owner = -1;

static MS_BOOL m_bInitialize = FALSE;
static MS_BOOL _gbBackwardRePlayMode = FALSE;

static MS_U32 _gu32TotalTime = 0;
static MS_U32 _gu32StartTime = 0;
static MS_S32 _gs32PreViewOffsetX = 0;
static MS_S32 _gs32PreViewOffsetY = 0;
static MS_BOOL _gbMusicBackwardRePlayMode = TRUE;  //for music
static EN_MMSDK_ROTATE_ANGLE _geRotaAng = E_MMSDK_ROTATE_0;
static EN_MMSDK_VIEW_ZOOM_FACTOR _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
static MS_U8 _gu8AudioPara[MAX_PARA_SIZE];
static EN_MPLAYER_SUB_TYPE _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
static MS_U32 _gu32MetalLen = 0;
static MS_U8 *_gpu8Buffer = NULL;
static MS_U16 *_gpu16UnicdBuffer = NULL;

//for ring buffer info
static MS_U32 _gu32VdplayerReadTimerOut = 0xFFFFFFFF;
static MS_BOOL _gbFastInit = FALSE;
static MS_U32 _gu32InitDataSize = 0x20000;
static EN_SEAMLESS_MODE _geSeamlessMode = MPLAYER_SEAMLESS_NONE;
static MS_S32 _gs32MinBufferSec = 10;
static MS_S32 _gs32MaxBufferSec = 30;

// for gif
static MS_U8 _gu8FileExtName[EXTENTION_NAME_LENGTH] = {};
static MS_U32 _gu32GifAddrOffset = 0;
static MS_BOOL _gbEnableMotionGif = FALSE;

//for debug info.....
static EN_MMSDK_DBG_LEVEL _geDebugLevel  = E_MMSDK_DBG_LEVEL_ERR;

// for es player
static ST_MMSDK_ES_DATA_INFO _gstESBuffer[ESBUFFER_SIZE];
static MS_BOOL _gbESBufferFull = FALSE;
static MS_U8 _gu8ESBufferRptr = 0;
static MS_U8 _gu8ESBufferWptr = 0;
static MS_BOOL _gbStartPush = FALSE;
static MS_BOOL _gbExitParseThread = FALSE;
static Task_Info _gThreadPushEsProcess = {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadParseES1"};
static void* _gpEsFileHandle = NULL;
static MS_BOOL _gbWaitEsFlushDataReady = FALSE;
static MS_U32 _gu32SeekPTS = 0;
static MS_U8* _gpu8FeedBuf = NULL;
static MS_U32 _gu32FeedBufSize = 0x80000;
static MS_U32 _gu32LastAudioPTS = 0xFFFFFFFF;

static MS_U8 _gu8Tmp_b = 0;
static MS_U8 _gu8Tmp_a = 0;
#ifndef VDPLAYER_BS_EXT_AVAILABLE
static void* _gpBSAudBufVA;
#endif

static MS_U32 _gu32CurrentMediaItem = MMSDK_MEDIA_ITEM_MIN;
#if (DEMO_MM_AVP_TEST ==1)
static MS_U32 _gu32CurrentPlayAudioItem = MMSDK_MEDIA_ITEM_MIN;
static EN_MM_AUDIO_SWITCH_MODE _geAudioSwitchMode = E_MM_AUDIO_SWITCH_LAST_PLAY;
#endif

#if(DEMO_AUDIO_TEST == 1)
static MS_U32 _gu32AudioDecoderId = 0;  // 0 = main, 1 = sub (vdplayer only support main by KC.Lee (20160526))
#endif

static MS_BOOL _Demo_MM_Initialize(void)
{
    MS_BOOL bRet = FALSE;
    MM_TRACE("_Demo_MM_Initialize.\n");

    if (m_bInitialize == TRUE)
    {
        MM_ERR("Already initialized\n");
        return bRet;
    }

    bRet = MMSDK_Initialize();

    MMSDK_DebugMSGLevel(_geDebugLevel);

    if (!bRet)
        MMSDK_Finalize();

    m_bInitialize = TRUE;
    MM_TRACE("_Demo_MM_Initialize Done.\n");

    return bRet;
}

static MS_BOOL _Demo_MM_Finalize(void)
{
    MS_BOOL bRet = FALSE;
    MM_TRACE("_Demo_MM_Finalize.\n");

    if (m_bInitialize == FALSE)
    {
        MM_ERR("Not initialized\n");
        return TRUE;
    }

    //Finalize
    bRet = MMSDK_Finalize();
    m_bInitialize = FALSE;

    return bRet;
}

static MS_BOOL _Demo_MM_SetCropDisplayRectangle(MEDIAITEM u32Item, ST_RECT_SIGNED_POSITION *stSignedCropRect, ST_RECT_SIGNED_POSITION *stSignedDispRect, const MS_BOOL bVideoMute, const MS_BOOL bKeepRatio)
{
    ST_MMSDK_CROP_DISPLAY_WINDOW stInfo;
    MM_TRACE("MM_SetCropDisplayRectangle.\n");

    memcpy(&(stInfo.stCropRect), (ST_MMSDK_RECT*)stSignedCropRect, sizeof(ST_MMSDK_RECT));
    memcpy(&(stInfo.stDispRect), (ST_MMSDK_RECT*)stSignedDispRect, sizeof(ST_MMSDK_RECT));
    stInfo.bkeepRatio = bKeepRatio;
    stInfo.bBlackScreen = bVideoMute;

    return MMSDK_SetCropDisplayRectangle(u32Item, &stInfo);
}

static MS_BOOL _Demo_MM_SetDisplayRectangle(MEDIAITEM u32Item, ST_RECT_SIGNED_POSITION *stSignedDispRect, const MS_BOOL bVideoMute, const MS_BOOL bKeepRatio)
{
    ST_RECT_SIGNED_POSITION stSignedCropRect;
    MM_TRACE("MM_SetDisplayRectangle.\n");

    memset(&stSignedCropRect, 0, sizeof(ST_RECT_SIGNED_POSITION));

    return _Demo_MM_SetCropDisplayRectangle(u32Item, &stSignedCropRect, stSignedDispRect, bVideoMute, bKeepRatio);
}

static MS_BOOL _Demo_MM_GetSubtitleTrackInfo(MEDIAITEM u32Item, ST_SUBTITLE_TRACK_INFO * pTrackInfo)
{
    ST_MMSDK_PARAM stParam;
    MS_BOOL bRet = FALSE;
    MS_U32 u32SubtitleType = 0;
    memset(&stParam, 0, sizeof(ST_MMSDK_PARAM));
    stParam.u32Option = E_MMSDK_OPTION_MOVIE_INFO;
    stParam.u32SubOption = E_MMSDK_INFO_MOVIE_SUBTITLE_TYPE;
    stParam.pParam = (void*)&u32SubtitleType; //assign from UI(buffer adderss)
    stParam.u32Paramsize = sizeof(u32SubtitleType); //assign from UI
    bRet = MMSDK_GetOptionByPara(u32Item, &stParam,1);
    if (bRet == TRUE)
    {
        switch(u32SubtitleType)
        {
            case E_MMSDK_SUBTITLE_TYPE_TEXT_UTF8:
                pTrackInfo->eTextFormat = E_SUBTITLE_TEXT_UTF8;
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_TEXT;
                break;
            case E_MMSDK_SUBTITLE_TYPE_TEXT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SRT:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_ASS:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_SSA:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_3GPP:
            case E_MMSDK_SUBTITLE_TYPE_TEXT_USF:
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_TEXT;
                break;
            case E_MMSDK_SUBTITLE_TYPE_IMAGE:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_BMP:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_VOBSUB:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_BMP:
            case E_MMSDK_SUBTITLE_TYPE_IMAGE_TS_PGS:
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_IMAGE;
                pTrackInfo->eImageFormat = E_SUBTITLE_IMAGE_ARGB8888;
                break;
            default:
                pTrackInfo->eSubtitleType = E_SUBTITLE_DATA_TYPE_TEXT;
                pTrackInfo->eTextFormat = E_SUBTITLE_TEXT_UTF8;
                break;
        }

        MMSDK_GetSubtitleTrackID(u32Item, &pTrackInfo->u8TrackID);
    }

    return bRet;
}

static MS_BOOL _Demo_ObtainRecursiveMutex(MS_S32 s32MutexId)
{
    MS_S32 s32Self = MsOS_InfoTaskID();
    if(_gu32Recursion == 0)
    {
        MsOS_ObtainMutex(s32MutexId, MSOS_WAIT_FOREVER);
        _gu32Recursion = 1;
        _gs32Owner = s32Self;
    }
    else
    {
        if (_gs32Owner == s32Self)
            _gu32Recursion++;
        else
        {
            MsOS_ObtainMutex(s32MutexId, MSOS_WAIT_FOREVER);
            _gu32Recursion = 1;
            _gs32Owner = s32Self;
        }
    }

    return TRUE;
}

static MS_BOOL _Demo_ReleaseRecursiveMutex(MS_S32 s32MutexId)
{
    if (_gs32Owner == MsOS_InfoTaskID())
    {
        if (--_gu32Recursion == 0)
        {
            _gs32Owner = -1;//reset owner
            MsOS_ReleaseMutex(s32MutexId);
        }
    }
    else
        return FALSE;

    return TRUE;
}

#if(DEMO_AUDIO_TEST == 1)
MS_BOOL Demo_MM_AllocAudDec(void)
{
    //open audio decode system
    ST_AUDIO_DEC_INFO stAudDecInfo;

    if((Demo_Audio_GetDecInfo(_gu32AudioDecoderId, &stAudDecInfo) == FALSE) || (stAudDecInfo.eDecID == AU_DEC_INVALID))
    {
        MM_ERR("get audio decoder info failed!\n");
        return FALSE;
    }

    if(PT_SYS_SetAudDecID(stAudDecInfo.eDecID) == FALSE)
    {
        MM_ERR("set audio decoder ID failed!\n");
        return FALSE;
    }

    return TRUE;
}

MS_BOOL Demo_MM_FreeAudDec(void)
{
    //release audio decode system
    if(PT_SYS_SetAudDecID(AU_DEC_INVALID) == FALSE)
    {
        MM_ERR("clear audio decoder ID failed!\n");
        return FALSE;
    }

    return TRUE;
}
#endif

static MS_U16 _Demo_UTF8ToUCS2(const MS_U8 * const pu8Str, MS_U16* const pu16Str, const MS_U16 srcByteLen, const MS_U16 dstWideCharLen)
{
    //see reference from http://en.wikipedia.org/wiki/Utf-8
    MS_U8 u8Value;
    MS_U16 i, j;

    if ((pu8Str == 0) || (srcByteLen == 0) || (pu16Str == 0) || (dstWideCharLen <= 1))
    {
        return 0;
    }

    i = 0;
    j = 0;
    do
    {
        if ((i >= srcByteLen) ||(j >= (dstWideCharLen - 1)))
        {
            break;
        }
        u8Value = pu8Str[i];
        if (u8Value == 0)
        {
            break;
        }
        if ((u8Value & 0x80) == 0)
        {
            // 0zzzzzzz(00-7F) => 00000000 0zzzzzzz
            pu16Str[j] = (MS_U16)(u8Value & 0x7F);
        }
        else if(((u8Value & 0xE0) == 0xC0) && ((i + 1) < srcByteLen))
        {
            // 110yyyyy(C2-DF) 10zzzzzz(80-BF) => 00000yyy yyzzzzzz
            pu16Str[j] = (MS_U16)(((u8Value & 0x1F) << 6) | (pu8Str[i+1] & 0x3F));
            i += 1;
        }
        else if(((u8Value & 0xF0) == 0xE0) && ((i + 2) < srcByteLen))
        {
            // 1110xxxx(E0-EF) 10yyyyyy 10zzzzzz => xxxxyyyy yyzzzzzz
            pu16Str[j] = (MS_U16)(((u8Value & 0xF) << 12) | ((pu8Str[i+1] & 0x3F) << 6) | (pu8Str[i+2] & 0x3F));
            i += 2;
        }
        else if(((u8Value & 0xF8) == 0xF0) && ((i + 3) < srcByteLen))
        {
            MS_U32 u32Code = 0;
            /////11110XX 10XXXXXX ....
            u32Code = (MS_U16)(((u8Value & 0xF7) << 18) | ((pu8Str[i+1] & 0x3F) << 12) | ((pu8Str[i+2] & 0x3F) << 6) | (pu8Str[i+3] & 0x3F));
            u32Code -= 0x10000;
            pu16Str[j+1]  = (MS_U16)(0xd800 | (u32Code >> 10));
            pu16Str[j] = (MS_U16)(0xdc00 | (u32Code & 0x3ff));
            j += 1;
            i += 3;
        }
        else
        {
            // Unknown, skip
            j--;
        }

        i++;
        j++;
    }
    while(1);

    return j * 2;
}

static MS_S8 *_Demo_itoa(MS_U32 num, MS_S8 *str, const MS_U32 radix)
{
    const MS_U8 index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    MS_U32 unum;
    MS_U32 i = 0, j, k;

    unum = num;
    do
    {
        str[i++] = index[unum%radix];
        unum /= radix;
    }
    while(unum);

    str[i] = '\0';
    if ((i - 1) / 2 == 0)
        k = 1;
    else
        k = (i - 1) / 2;

    for (j = 0; j < k; j++)
    {
        num = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = num;
    }

    return str;
}

static MS_U16 _Demo_ConvBig5ToUnicode(const MS_U16 u16Big5Code)
{
    MS_U16 u16Unicode = INVALID_UNICODE;
    if ((u16Big5Code & 0x00FF) == 0x0000)
    {
        u16Unicode = u16Big5Code;
    }

    return u16Unicode;
}

static MS_BOOL _Demo_GetOption_AllocateMemory(MS_U32 u32UnicdBufferSize)
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if (_gu32MetalLen == 0)
    {
        MM_ERR("=========== _gu32MetalLen is 0 ===============\n");
        return FALSE;
    }
    if (_gpu8Buffer == NULL)
    {
        _gpu8Buffer = (MS_U8 *)MsOS_AllocateMemory(sizeof(MS_U8) * _gu32MetalLen, s32MstarNonCachedPoolID);
        ASSERT(_gpu8Buffer);
    }
    else
    {
        _gpu8Buffer = (MS_U8 *)MsOS_ReallocateMemory(_gpu8Buffer, sizeof(MS_U8) * _gu32MetalLen, s32MstarNonCachedPoolID);
        ASSERT(_gpu8Buffer);
    }

    if (_gpu16UnicdBuffer == NULL)
    {
        _gpu16UnicdBuffer = (MS_U16 *)MsOS_AllocateMemory(u32UnicdBufferSize, s32MstarNonCachedPoolID); //malloc memory for unicode
        if (_gpu16UnicdBuffer == NULL)
        {
            MM_ERR("WARNING WARNING _gpu16UnicdBuffer == NULL!!!!!!\n");
        }
        ASSERT(_gpu16UnicdBuffer);
    }
    else
    {
        _gpu16UnicdBuffer = (MS_U16*)MsOS_ReallocateMemory(_gpu16UnicdBuffer, u32UnicdBufferSize, s32MstarNonCachedPoolID);
        if (_gpu16UnicdBuffer == NULL)
        {
            MM_ERR("WARNING WARNING _gpu16UnicdBuffer realloc NULL!!!!!!\n");
        }
        ASSERT(_gpu16UnicdBuffer);
    }

    return TRUE;
}

static MS_BOOL _Demo_GetOption_CharEncConv(EN_PLAYER_STRING_FORMAT eStrFormt, MS_BOOL bEndianChange)
{
    MS_U32 u32Ret = 0;
    MS_U32 u32UnicdBytes = 0;

    if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
    {
        MM_DBG("----------> info: %d ---UTF8-----\n", (MS_U8)_gu32MetalLen);
        u32UnicdBytes = _Demo_UTF8ToUCS2(_gpu8Buffer, _gpu16UnicdBuffer, _gu32MetalLen, 0xffff);
        u32Ret = u32UnicdBytes;
    }
    else if (eStrFormt == E_PLAYER_STRING_FORMAT_ASCII)
    {
        MS_U32 i = 0;
        MS_U32 j = 0;
        MS_S16 s16GBCode = 0;
        MS_U16 u16Unicode = 0;

        MM_DBG("----------> info: %d  bytes---ASCII-----\n", (int)_gu32MetalLen);
        for (j = 0, i = 0; i < _gu32MetalLen; i++, j++)
        {
            u32UnicdBytes++;
            s16GBCode = *((MS_S8 *)(_gpu8Buffer + i));
            if (s16GBCode > 0) //basic ASCII code.
            {
                u16Unicode = _Demo_ConvBig5ToUnicode((MS_U16)s16GBCode);
                MM_DBG("----English %d\n", s16GBCode);
            }
            else            //chinese char
            {
                MS_U16 u16EndianChange = *(MS_U16 *)(_gpu8Buffer + i);
                if(bEndianChange)
                    u16EndianChange = (u16EndianChange >> 8) + ((u16EndianChange & 0xff) << 8);

                u16Unicode = _Demo_ConvBig5ToUnicode(u16EndianChange);
                MM_DBG("----Chinese %d\n", u16EndianChange);
                MM_DBG("----Chinese Unicode %x\n", u16Unicode);

                i += 1;
            }

            _gpu16UnicdBuffer[j] = u16Unicode;
        }

        u32Ret = u32UnicdBytes * 2;
    }
    else  //unicode
    {
        u32Ret = _gu32MetalLen;
        memcpy(_gpu16UnicdBuffer, _gpu8Buffer, _gu32MetalLen);
        MM_DBG("-----> len is %d-----UNICODE\n", (int)_gu32MetalLen);
    }

    return u32Ret;
}

static unsigned int _Demo_MPlayer_GetOption(const MS_U32 u32Idx, const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32SubOption)
{
    MM_TRACE("_Demo_MPlayer_GetOption, option type = %d\n", eOption);

    MS_U32 u32Ret = 0;
    EN_PLAYER_STRING_FORMAT eStrFormt = E_PLAYER_STRING_FORMAT_UNICODE;
    MS_S8 numStr[256] = {0};
    MS_U8 u8TrackID = 0;
    ST_SUBTITLE_TRACK_INFO stSubTrackInfo;
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32UnicdBufferSize;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    switch(eOption)
    {
        case MPLAYER_OPTION_MOVIE_AUDIO_TRACK_NAME:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_AUDIO_TRACK_LANGUAGE, sizeof(MS_U32), (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_AUDIO_TRACK_INFO:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_AUDIO_CODEC_TRACK_ID, sizeof(MS_U32), (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_SUBTITLE_TRACK_NAME:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_SUBTITLE_TRACK_LANGUAGE, sizeof(MS_U32), (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_INDEX_FRAME_EXIST:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_INDEX_EXISTED, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_TOTAL_TIME:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    MM_DBG("-------> total_time is %d ms-----\n", (int)u32Ret);
                    break;

                case MPLAYER_INFO_CUR_TIME:
                    MMSDK_GetPlayPosition(_gItem[u32Idx][eMediaType], &u32Ret);
                    break;

                case MPLAYER_INFO_NB_PROGRAM:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_PROGRAM, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_NB_AUDIO_TRACK:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_NB_SUBTITLE_TRACK:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PROGRAM_ID:
                    MMSDK_GetProgramID(_gItem[u32Idx][eMediaType], &u8TrackID);
                    u32Ret = u8TrackID;
                    break;

                case MPLAYER_INFO_AUDIO_TRACK_ID:
                    MMSDK_GetAudioTrackID(_gItem[u32Idx][eMediaType], &u8TrackID);
                    u32Ret = u8TrackID;
                    break;

                case MPLAYER_INFO_SUBTITLE_TRACK_ID:
                    if (_Demo_MM_GetSubtitleTrackInfo(_gItem[u32Idx][eMediaType], &stSubTrackInfo))
                        u32Ret = stSubTrackInfo.u8TrackID;
                    break;

                case MPLAYER_INFO_AUDIO_CODEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_VIDEO_CODEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_VIDEO_CODEC, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_OPTION_GET_MUSIC_TYPE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);

                    if (_geAudioType == MPLAYER_SUBTYPE_PCM)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_PCM;
                    else if(u32Ret == E_MMSDK_AUDIO_CODEC_MP3)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_MP3;
                    else if(u32Ret == E_MMSDK_AUDIO_CODEC_AAC)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_AAC;
                    else if((u32Ret == E_MMSDK_AUDIO_CODEC_PCM) || (u32Ret == E_MMSDK_AUDIO_CODEC_ADPCM))
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_WAV;
                    else if(u32Ret == E_MMSDK_AUDIO_CODEC_WMA)
                        u32Ret = (MS_U32)MPLAYER_SUBTYPE_WMA;
                    break;

                case MPLAYER_INFO_H_SIZE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_V_SIZE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_FRM_PER_SEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_FRAME_PER_SEC, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                default:
                    MM_ERR("[ERROR] Unknown MOVIE INFO\n");
                    break;
            }
            break;

        case MPLAYER_OPTION_TRICK_MODE:
            u32Ret = (MS_U32)_geTrickMod[u32Idx][eMediaType];
            break;

        case MPLAYER_OPTION_ROTATION:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
            {
                u32Ret = (MS_U32)_geRotaAng;
            }
            else
            {
                u32Ret = (MS_U32)0xFFFF;
            }
            break;

        case MPLAYER_OPTION_ZOOM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO || eMediaType ==E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                u32Ret = (MS_U32)_geZoomFctor;
            }
            else
            {
                u32Ret = (MS_U32)0xFFFF;
            }
            break;


        case MPLAYER_OPTION_GET_PHOTO_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_PHOTO_DECODED_WIDTH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_WIDTH, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODED_HEIGHT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_HEIGHT, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODED_PITCH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_PITCH, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODE_PROGRESS:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_DECODE_PROGRESS, sizeof(MS_U32), (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_WIDTH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_HEIGHT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_HEIGHT:
                    u32Ret = _gstRect[u32Idx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_WIDTH:
                    u32Ret = _gstRect[u32Idx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Width;
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_SIZE:
                    u32Ret = _gstRect[u32Idx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Width * _gstRect[u32Idx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    break;

                case MPLAYER_INFO_PHOTO_CODEC_TYPE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_TYPE, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODE_FORMAT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_COLOR_FORMAT, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                default:
                    MM_ERR("[ERROR] Unknown PHOTO INFO\n");
                    break;
            }
            break;

        case MPLAYER_OPTION_MUSIC_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_TOTAL_TIME:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[u32Idx][eMediaType]);
                    break;

                case MPLAYER_INFO_CUR_TIME:
                    MMSDK_GetPlayPosition(_gItem[u32Idx][eMediaType], &u32Ret);
                    break;

                case MPLAYER_INFO_MUSIC_BITRATE_LEN:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                        E_MMSDK_INFO_MUSIC_BITRATE, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[u32Idx][eMediaType]);

                    if(!_Demo_GetOption_AllocateMemory(2UL * 2 * _gu32MetalLen))
                        return 0;

                    if (_gpu8Buffer  == NULL)
                        return 0;

                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_BITRATE, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[u32Idx][eMediaType]);

                    _Demo_itoa(*((MS_U32*)_gpu8Buffer), numStr, 10);
                    u32Ret = strlen((const char *)numStr);
                    numStr[u32Ret] = 'K';
                    numStr[u32Ret+1] = '\0';
                    _Demo_UTF8ToUCS2((MS_U8 *)numStr, _gpu16UnicdBuffer, (MS_U16)strlen((const char*)numStr), 0xffff);
                    u32Ret = strlen((const char*)numStr) * 2;
                    break;

            case MPLAYER_INFO_MUSIC_LAYER_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_LAYER:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)MsOS_AllocateMemory(4 * 1, s32MstarNonCachedPoolID); //malloc memory for unicode
                    ASSERT(_gpu16UnicdBuffer);
                }

                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_GENRE_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_GENRE:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)MsOS_AllocateMemory(4 * 1, s32MstarNonCachedPoolID); //malloc memory for unicode
                    ASSERT(_gpu16UnicdBuffer);
                }

                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_COMMENT_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_COMMENT:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)MsOS_AllocateMemory(4 * 1, s32MstarNonCachedPoolID); //malloc memory for unicode
                }
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_CHECK_FF_FB_LEN:
                u32Ret = 1;
                break;

            case MPLAYER_INFO_MUSIC_CHECK_FF_FB:
                if (_gpu16UnicdBuffer == NULL)
                {
                    _gpu16UnicdBuffer = (MS_U16*)MsOS_AllocateMemory(4 * 1, s32MstarNonCachedPoolID); //malloc memory for unicode
                    ASSERT(_gpu16UnicdBuffer);
                }

                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_SAMPLINGRATE:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_SAMPLINGRATE_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_SAMPLE_FREQ, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[u32Idx][eMediaType]);

                if(!_Demo_GetOption_AllocateMemory(4 * 4))
                    return 0;

                if (_gpu8Buffer == NULL)
                    return 0;

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                E_MMSDK_INFO_MUSIC_SAMPLE_FREQ, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[u32Idx][eMediaType]);

                _Demo_itoa(*((MS_U32 *)_gpu8Buffer), numStr, 10);
                _Demo_UTF8ToUCS2((MS_U8 *)numStr, _gpu16UnicdBuffer, (MS_U16)strlen((const char *)numStr), 0xffff);
                u32Ret = strlen((const char*)numStr) * 2;
                break;

            case MPLAYER_INFO_MUSIC_YEAR_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_YEAR, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[u32Idx][eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_YEAR_FORMAT, 4, (void*)&eStrFormt, _gItem[u32Idx][eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_YEAR, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[u32Idx][eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, FALSE);
                }
                break;

            case MPLAYER_INFO_MUSIC_YEAR:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_ARTIST_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_ARTIST, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[u32Idx][eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_ARTIST_FORMAT, 4, (void*)&eStrFormt, _gItem[u32Idx][eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_ARTIST, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[u32Idx][eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_ARTIST:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_ALBUM_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_ALBUM, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[u32Idx][eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_ALBUM_FORMAT, 4, (void*)&eStrFormt, _gItem[u32Idx][eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_ALBUM, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[u32Idx][eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_ALBUM:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_TITLE_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_TITLE, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[u32Idx][eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_TITLE_FORMAT, 4, (void*)&eStrFormt, _gItem[u32Idx][eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_TITLE, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[u32Idx][eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_TITLE:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;
            }
            break;

    default:
        break;
    }

    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    return u32Ret;
}

static MS_BOOL _Demo_MPlayer_SetOption(const MS_U32 u32Idx, const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32Param)
{
    MM_TRACE("_Demo_MPlayer_SetOption, option type = %d\n", eOption);

    MS_U8 u8R = 0, u8G = 0, u8B = 0;
    MS_BOOL bRet = FALSE;
    MS_U32 u32TotalTrackCnt = 0;
    MS_BOOL bPlayerStatus = TRUE;
    MS_U32 u32TrackNum = u32Param;
    MS_S32 s32MoveViewOffsetX = 0;
    MS_S32 s32MoveViewOffsetY = 0;
    ST_RECT_SIGNED_POSITION sDispWindow;
    const MPLAYER_MOVEVIEW_PARAM *pMoveView_Param = (const MPLAYER_MOVEVIEW_PARAM *)u32Param;
    const MPLAYER_DISP_WINDOW *pDisplayWindow = (const MPLAYER_DISP_WINDOW *)u32Param;
    const MPLAYER_PCM_PARAM *pPCM_Param = (const MPLAYER_PCM_PARAM *)u32Param;
    ST_MMSDK_PARAM stParam = {.u32Option = E_MMSDK_OPTION_MOVIE_INFO, .u32Paramsize = sizeof(u32TotalTrackCnt), .pParam = (void*)&u32TotalTrackCnt};

    if (eMediaType >= MMSDK_MEDIA_TYPE_NUM)
    {
        return FALSE;
    }

    if (eOption == MPLAYER_OPTION_SPECIFIC_TOTAL_TIME) //avoid recursion lock(both playfile and set_option called)
    {
        _gu32TotalTime = (MS_U32)u32Param;
        MM_DBG("-------> _gu32TotalTime(%"DTC_MS_U32_d")\n", _gu32TotalTime);
        return TRUE;
    }

    if (eOption == MPLAYER_OPTION_SET_STARTTIME) //avoid recursion lock(both playfile and set_option called)
    {
        _gu32StartTime = (MS_U32)u32Param;
        MM_DBG("-------> _gu32StartTime(%"DTC_MS_U32_d")\n", _gu32StartTime);
        return TRUE;
    }

    if (eOption == MPLAYER_OPTION_ENABLE_NEW_AUDIO_MODE)
    {
        _gbUseNewAudioMode = (MS_BOOL)u32Param;
        MM_DBG("-------> _gbUseNewAudioMode(%d)\n", _gbUseNewAudioMode);
        return TRUE;
    }

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    switch(eOption)
    {
        case MPLAYER_OPTION_SUBTITLE_MEM_LESS_MODE://currently not supported
            break;
        case MPLAYER_OPTION_SUBTITLE_DYN_ONSCRREN_MODE://currently not supported
            break;

        case MPLAYER_OPTION_SET_SEAMLESS_MODE:
            _geSeamlessMode = (EN_SEAMLESS_MODE)u32Param;
            break;

        case MPLAYER_OPTION_UP_BOUNDARY:
            _gs32MaxBufferSec = (MS_S32)u32Param;
            break;

        case MPLAYER_OPTION_DOWN_BOUNDARY:
            _gs32MinBufferSec = (MS_S32)u32Param;
            break;

        case MPLAYER_OPTION_READ_TIME_OUT:
            _gu32VdplayerReadTimerOut = u32Param;
            break;

        case MPLAYER_OPTION_SET_DBG_LEVEL:
            _geDebugLevel = (EN_MMSDK_DBG_LEVEL)u32Param;
            bRet = MMSDK_DebugMSGLevel(_geDebugLevel);
            break;

        case MPLAYER_OPTION_INIT_DATA_SIZE:
            _gu32InitDataSize = u32Param;
            break;

        case MPLAYER_OPTION_ENABLE_FAST_INIT:
            _gbFastInit = (MS_BOOL)u32Param;
            break;

        case MPLAYER_OPTION_ENABLE_SLIDE_SHOW:
            bRet = MMSDK_SetSlideShowEffect(u32Param);
            break;

        case MPLAYER_OPTION_DISABLE_SLIDE_SHOW:
            if ((MS_BOOL)u32Param == TRUE)
                bRet = MMSDK_SetSlideShowEffect(E_MMSDK_SLIDE_SHOW_EFFECT_NORMAL);
            break;

        case MPLAYER_OPTION_STOP_PHOTO_DISPLAY:
            bRet = MMSDK_ClearPhotoDisplay(u8R, u8G, u8B);
            break;

        case MPLAYER_OPTION_REPEATAB:
            bRet = MMSDK_SetRepeatAB(_gItem[u32Idx][eMediaType], (EN_MMSDK_REPEAT_AB)u32Param);
            break;

        case MPLAYER_OPTION_FB_REPLAY:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
            {
                _gbMusicBackwardRePlayMode = (MS_BOOL)u32Param;
            }
            else
            {
                _gbBackwardRePlayMode = (MS_BOOL)u32Param;
            }
            bRet = TRUE;
            break;

        case MPLAYER_OPTION_ZOOM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                PT_SYS_SetFlagResizeWindowByUser(u32Idx, TRUE);
            }

            bRet = MMSDK_SetZoomFactor(_gItem[u32Idx][eMediaType], u32Param);
            if(bRet)
                _geZoomFctor = u32Param;
            break;

        case MPLAYER_OPTION_ROTATION:
            bRet = MMSDK_SetRotateAngle(_gItem[u32Idx][eMediaType], u32Param);
            if(bRet)
                _geRotaAng = u32Param;
            break;

        case MPLAYER_OPTION_MOVE_VIEW:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                PT_SYS_SetFlagResizeWindowByUser(u32Idx, TRUE);
            }

            MMSDK_GetViewOffset(_gItem[u32Idx][eMediaType], &_gs32PreViewOffsetX, &_gs32PreViewOffsetY);
            bRet = MMSDK_MoveViewWindow(
                                _gItem[u32Idx][eMediaType],
                                (EN_MMSDK_VIEW_MOVE_DIRECTION)pMoveView_Param->eDirection,
                                pMoveView_Param->u16Offset);
            MMSDK_GetViewOffset(_gItem[u32Idx][eMediaType], &s32MoveViewOffsetX, &s32MoveViewOffsetY);

            if (pMoveView_Param->eDirection == E_MPLAYER_DIRECTION_UP || pMoveView_Param->eDirection == E_MPLAYER_DIRECTION_DOWN)
            {
                    if (s32MoveViewOffsetY == _gs32PreViewOffsetY)
                    {
                        bRet = FALSE;
                    }
            }
            else if (pMoveView_Param->eDirection == E_MPLAYER_DIRECTION_LEFT || pMoveView_Param->eDirection == E_MPLAYER_DIRECTION_RIGHT)
            {
                    if (s32MoveViewOffsetX == _gs32PreViewOffsetX)
                    {
                        bRet = FALSE;
                    }
            }

            break;

        case MPLAYER_OPTION_ASPECT_RATIO:
            bRet = MMSDK_SetAspectRatio(E_MMSDK_MEDIA_TYPE_MOVIE, u32Param);
            break;

        case MPLAYER_OPTION_SET_KTV_MODE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                _gbKTVMode = (MS_BOOL)u32Param;
                bRet = TRUE;
            }
            break;

        case MPLAYER_OPTION_CHANGE_PROGRAM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_PROGRAM;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[u32Idx][eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                ST_MMSDK_TS_INFO stMMSDK_TSInfo = {};
                bRet = MMSDK_SetProgramID(_gItem[u32Idx][eMediaType], u32TrackNum, &stMMSDK_TSInfo);
            }
            break;

        case MPLAYER_OPTION_CHANGE_AUDIO:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[u32Idx][eMediaType], &stParam, 1);
                if (bPlayerStatus == FALSE || u32TotalTrackCnt == 0)
                {
                    goto FUNC_END;
                }

                bRet = MMSDK_SetAudioTrackID(_gItem[u32Idx][eMediaType], u32TrackNum);
            }
            break;

        case MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[u32Idx][eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                bRet = MMSDK_SetSubtitleTrackID(_gItem[u32Idx][eMediaType], u32TrackNum);
            }
            break;

        case MPLAYER_OPTION_SET_MUSIC_TYPE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
            {
                _geAudioType = (EN_MPLAYER_SUB_TYPE)u32Param;
                bRet = TRUE;
            }
            break;

        case MPLAYER_OPTION_MUSIC_SET_PCM_PARAM:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
            {
                _gu8AudioPara[0] = (MS_U8)pPCM_Param->eFormatTag;
                SETU16(_gu8AudioPara, 1, pPCM_Param->u16Channels);
                SETU16(_gu8AudioPara, 3, pPCM_Param->u16BitsPerSample);
                SETU32(_gu8AudioPara, 5, pPCM_Param->u32SamplesPerSec);
                _gu8AudioPara[9] = (MS_U8)pPCM_Param->bBigEndian;
                bRet = TRUE;
            }
            break;

        case MPLAYER_OPTION_DISPLAY_WINDOW:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                PT_SYS_SetFlagResizeWindowByUser(u32Idx, TRUE);
            }

            sDispWindow.s32X = pDisplayWindow->u32DispX;
            sDispWindow.s32Y = pDisplayWindow->u32DispY;
            sDispWindow.u16Width = pDisplayWindow->u32DispW;
            sDispWindow.u16Height = pDisplayWindow->u32DispH;
            MM_DBG("--------DISP_WINDOW[%"DTC_MS_U32_d"][%"DTC_MS_U32_d"][%d][%d]\n", sDispWindow.s32X, sDispWindow.s32Y, sDispWindow.u16Width, sDispWindow.u16Height);

            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
                bRet = _Demo_MM_SetDisplayRectangle(_gItem[u32Idx][eMediaType], &sDispWindow, TRUE, FALSE);
            break;

        case MPLAYER_OPTION_SET_AVP_ZORDER:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                PT_SYS_SetFlagResizeWindowByUser(u32Idx, TRUE);
            }

            sDispWindow.s32X = _gstDipRect[u32Idx].u16X;
            sDispWindow.s32Y = _gstDipRect[u32Idx].u16Y;
            sDispWindow.u16Width = _gstDipRect[u32Idx].u16Width;
            sDispWindow.u16Height = _gstDipRect[u32Idx].u16Height;

            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
                bRet = _Demo_MM_SetDisplayRectangle(_gItem[u32Idx][eMediaType], &sDispWindow, FALSE, FALSE);
            break;

        default:
            break;
    }

FUNC_END:
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    return bRet;
}

#if defined(MSOS_TYPE_ECOS)
static void _Demo_PollingThreadStatus(Task_Info stTaskInfo)
{
    TaskStatus eTaskStatus;
    while(1)
    {
        if(!MsOS_GetTaskStatus(&stTaskInfo,&eTaskStatus))
        {
            MM_MUST("Error!! MsOS_GetTaskStatus() fail!! \n");
            break;
        }

        if(eTaskStatus == E_TASK_NOT_EXIST)
            break;
        else
        {
            MM_DBG("\33[32m Wait for Task#%x %s termination \33[m \n",
                stTaskInfo.iId,stTaskInfo.szName);
            MsOS_DelayTask(1);
        }
    }
}
#endif

MS_U32 _Demo_MM_GetEventIndex(MS_U32 u32Idx)
{
    switch(u32Idx)
    {
        case 0:
            return MM_EVENT_VIDEO_ITEM_1;
        case 1:
            return MM_EVENT_VIDEO_ITEM_2;
        case 2:
            return MM_EVENT_VIDEO_ITEM_3;
        case 3:
            return MM_EVENT_VIDEO_ITEM_4;
        default:
            return MM_EVENT_VIDEO_ITEM_INVALID;
    }
}

void _Demo_MPlayer_Finalize(void)
{
    MS_U32 u32ItemIdx;
    MS_U32 u32MediaType;
    MS_U32  pu32RetrievedEventFlag;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    for (u32ItemIdx = MMSDK_MEDIA_ITEM_MIN; u32ItemIdx< MMSDK_MEDIA_ITEM_MAX; u32ItemIdx++)
    {
        _gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = TRUE;
        _gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = TRUE;
        _gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] = TRUE;

        for ( u32MediaType = MMSDK_MEDIA_TYPE_MIN; u32MediaType < MMSDK_MEDIA_TYPE_NUM; u32MediaType++)
        {
            if (_gThreadEvtProcess[u32ItemIdx][u32MediaType].iId != -1)
            {
                if(_gbESPlay[u32ItemIdx][u32MediaType] == TRUE)
                {
                    MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);
                    MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_ES_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
                    _gbESPlay[u32ItemIdx][u32MediaType] = FALSE;
                }
                else
                {
                    switch(u32MediaType)
                    {
                        case E_MMSDK_MEDIA_TYPE_PHOTO:
                            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);
                            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
                            break;
                        case E_MMSDK_MEDIA_TYPE_MUSIC:
                            MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);
                            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
                            break;
                        case E_MMSDK_MEDIA_TYPE_MOVIE:
                            MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL, u32ItemIdx);
                            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
                            break;
                    }
                }
#if defined(MSOS_TYPE_ECOS)
                    _Demo_PollingThreadStatus(_gThreadEvtProcess[u32ItemIdx][u32MediaType]);
#endif
                    _gThreadEvtProcess[u32ItemIdx][u32MediaType].iId = -1;

                    if (_gThreadEvtProcess[u32ItemIdx][u32MediaType].pStack)
                    {
                        MsOS_FreeMemory(_gThreadEvtProcess[u32ItemIdx][u32MediaType].pStack, s32MstarNonCachedPoolID);
                        _gThreadEvtProcess[u32ItemIdx][u32MediaType].pStack = NULL;
                    }
            }
        }
    }

#ifndef VDPLAYER_BS_EXT_AVAILABLE
    if(_gpBSAudBufVA)
    {
        MsOS_FreeMemory(_gpBSAudBufVA, s32MstarNonCachedPoolID);
        _gpBSAudBufVA = NULL;
    }
#endif
    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);

    if (_gpu8Buffer)
    {
        MsOS_FreeMemory(_gpu8Buffer, s32MstarNonCachedPoolID);
        _gpu8Buffer = NULL;
    }
    if (_gpu16UnicdBuffer)
    {
        MsOS_FreeMemory(_gpu16UnicdBuffer, s32MstarNonCachedPoolID);
        _gpu16UnicdBuffer = NULL;
    }

    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    MsOS_DeleteMutex(_gs32Mutex);
    if (_gs32MediaEvtGrp != -1)
    {
        MsOS_DeleteEventGroup(_gs32MediaEvtGrp);
        _gs32MediaEvtGrp = -1;
    }

    MsOS_DeleteMutex(_gs32MutexESPlayer);
    MsOS_DeleteMutex(_gs32MutexOpenStop);
    if (!_Demo_MM_Finalize())
    {
        MM_ERR("-----> MM_Finalize ERROR ERROR!!!!\n");
    }
}

static void _Demo_MM_CreateThreadEvtName(MS_U32 u32ItemIdx, MS_U32 u32TypeIdx, char* pu8ThreadName)
{
    MS_U8 u8ThreadName[16] = "threadEvt";
    MS_S8 str[256] = {0};

    _Demo_itoa(u32ItemIdx, str, 10);
    strncat((char*)u8ThreadName, (char*)str, strlen((char*)str));
    strncat((char*)u8ThreadName, "-", 1);
    _Demo_itoa(u32TypeIdx, str, 10);
    strncat((char*)u8ThreadName, (char*)str, strlen((char*)str));

    snprintf((char*)pu8ThreadName, 16, "%s", u8ThreadName);
}

static void _DEMO_MM_ClearVariables(void)
{
    MS_U32 u32ItemIdx;
    MS_U32 u32MediaTypeIdx;

    for (u32ItemIdx = MMSDK_MEDIA_ITEM_MIN; u32ItemIdx < MMSDK_MEDIA_ITEM_MAX; u32ItemIdx++)
    {
        for(u32MediaTypeIdx = MMSDK_MEDIA_TYPE_MIN; u32MediaTypeIdx < MMSDK_MEDIA_TYPE_NUM; u32MediaTypeIdx++)
        {
            _gu64Size[u32ItemIdx][u32MediaTypeIdx] = 0;
            _gu64CurPostn[u32ItemIdx][u32MediaTypeIdx] = 0;
            _gpStream[u32ItemIdx][u32MediaTypeIdx] = NULL;
            _gItem[u32ItemIdx][u32MediaTypeIdx] = 0;
            _gbPlay[u32ItemIdx][u32MediaTypeIdx] = FALSE;
            _gbExit[u32ItemIdx][u32MediaTypeIdx] = FALSE;
            _gbESPlay[u32ItemIdx][u32MediaTypeIdx] = FALSE;
            _geTrickMod[u32ItemIdx][u32MediaTypeIdx] = E_MMSDK_TRICK_NORMAL;
            _gu32FilePlayFlag[u32ItemIdx][u32MediaTypeIdx] = INVALID_PLAY_FLAG;

            _gstRect[u32ItemIdx][u32MediaTypeIdx].u16X= 0;
            _gstRect[u32ItemIdx][u32MediaTypeIdx].u16Y= 0;
            _gstRect[u32ItemIdx][u32MediaTypeIdx].u16Height= 0;
            _gstRect[u32ItemIdx][u32MediaTypeIdx].u16Width= 0;

            _gThreadEvtProcess[u32ItemIdx][u32MediaTypeIdx].iId = -1;
            _gThreadEvtProcess[u32ItemIdx][u32MediaTypeIdx].ePriority = E_TASK_PRI_MEDIUM;
            _gThreadEvtProcess[u32ItemIdx][u32MediaTypeIdx].pStack = NULL;
            _gThreadEvtProcess[u32ItemIdx][u32MediaTypeIdx].u32StackSize = TH_EVT_PROC_STACK_SIZE;
            _Demo_MM_CreateThreadEvtName(u32ItemIdx, u32MediaTypeIdx, _gThreadEvtProcess[u32ItemIdx][u32MediaTypeIdx].szName);
        }
        _gpEvtProcess[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = _Demo_Photo_EvtProcess;
        _gpEvtProcess[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = _Demo_Music_EvtProcess;
        _gpEvtProcess[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] = _Demo_Video_EvtProcess;

        _gu64AudioCurPostn[u32ItemIdx] = 0;
        _gu64Audio2CurPostn[u32ItemIdx] = 0;
        _gu64SubCurPostn[u32ItemIdx] = 0;

        _gePlayStatus[u32ItemIdx] = E_MM_PLAY_STATUS_INVALID;

        MS_BOOL bSysValue = FALSE;
        PT_SYS_SetOptions(0, E_PT_SYS_STEP_DBG, &bSysValue);
        PT_SYS_SetOptions(0, E_PT_SYS_STEP_NEXTFRAME, &bSysValue);
    }

#if (DEMO_MM_AVP_TEST ==1)
     _geAudioSwitchMode = E_MM_AUDIO_SWITCH_LAST_PLAY;
    Demo_MM_ResetWindow();
#endif
}

MS_BOOL _Demo_MPlayer_Initialize(void)
{
    const MS_U8* pMutexName1 = (MS_U8*)"_gs32MutexOpenStop";
    const MS_U8* pMutexName2 = (MS_U8*)"VarMutex";
    const MS_U8* pMutexName3 = (MS_U8*)"ESPlayerMutex";
    const MS_U8* sEvtGrpName = (MS_U8*)"MMEvnGrp";

    _DEMO_MM_ClearVariables();

    _gu32VdplayerReadTimerOut = 500;
    _gbFastInit = FALSE;
    _gu32InitDataSize = 0x20000;

    _gs32MinBufferSec = 1;
    _gs32MaxBufferSec = 5;

    _gs32MutexOpenStop = MsOS_CreateMutex(E_MSOS_FIFO, (char*)pMutexName1, MSOS_PROCESS_SHARED);
    if(_gs32MutexOpenStop == -1)
    {
        goto FUNC_END;
    }

    _gs32Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)pMutexName2, MSOS_PROCESS_SHARED);
    if(_gs32Mutex == -1)
    {
        goto FUNC_END;
    }

    _gs32MutexESPlayer = MsOS_CreateMutex(E_MSOS_FIFO, (char*)pMutexName3, MSOS_PROCESS_SHARED);
    if(_gs32MutexESPlayer == -1)
    {
        goto FUNC_END;
    }

    _gs32MediaEvtGrp = MsOS_CreateEventGroup((char*)sEvtGrpName);
    if(_gs32MediaEvtGrp == -1)
    {
        goto FUNC_END;
    }

    if (!_Demo_MM_Initialize())
    {
        MM_ERR("-----> MM_Initialize ERROR ERROR!!!!\n");
    }

    _gbKTVMode = FALSE;
    _gbBackwardRePlayMode = FALSE;


FUNC_END:
    return FALSE;
}

static void _Demo_PT_SYS_ReadDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    //To read from NVM
}

static void _Demo_PT_SYS_WriteDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    //To write to NVM
}

MS_BOOL _Demo_PT_SYS_MemInit(void)
{
    PT_SYS_MmapInfo stMmapInfo={};
    ST_MMSDK_MMAP_INFO stMIUInfo={};

    //DRAM info
    stMIUInfo.u32Miu0MemSize  =  MIU_DRAM_LEN0;
#if (ENABLE_MIU_1 == 1)
    stMIUInfo.u32Miu1MemSize  =  MIU_DRAM_LEN1;
#endif
    stMIUInfo.u32MiuBoundary  =  MIU_INTERVAL;
    stMIUInfo.u32NMmapItems   =  sizeof(stMmapInfo)/sizeof(ST_MMSDK_BUF_INFO);
    stMIUInfo.u32TotalMemSize =  MIU_DRAM_LEN;

    //E_MMSDK_BUF_PHOTO_DISPLAY (need to check)
    MM_FILL_MEM_INFO(stMmapInfo.stPhotoDispBufAddr, PHOTO_DISPLAY_ADR, PHOTO_DISPLAY_LEN, PHOTO_DISPLAY_MEMORY_TYPE);
#if (DEMO_MM_MPO_TEST == 1)
    //E_MMSDK_BUF_PHOTO_DISPLAY_MPO
    MM_FILL_MEM_INFO(stMmapInfo.stPhotoMPODispBufAddr, PHOTO_MPO_DISPLAY_ADR, PHOTO_MPO_DISPLAY_LEN, PHOTO_MPO_DISPLAY_MEMORY_TYPE);
#endif
    //E_MMSDK_BUF_PHOTO_INTERNAL
    MM_FILL_MEM_INFO(stMmapInfo.stJPDInterBufAddr, JPD_INTER_BUF_ADR, JPD_INTER_BUF_LEN, JPD_INTER_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_PHOTO_MEMORY_POOL
    MM_FILL_MEM_INFO(stMmapInfo.stPhotoMemPoolAddr, JPD_INTER_BUF_ADR, MMSDK_BUF_PHOTO_MEMORY_POOL_LEN, JPD_INTER_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_PHOTO_OUTPUT
    MM_FILL_MEM_INFO(stMmapInfo.stJPDOutBufAddr, JPD_OUT_ADR, JPD_OUT_LEN, JPD_OUT_MEMORY_TYPE);

    //E_MMSDK_BUF_PHOTO_PROGRESSIVE
    MM_FILL_MEM_INFO(stMmapInfo.stPhotoProgAddr, JPD_INTER_BUF_ADR, JPD_INTER_BUF_LEN, JPD_INTER_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_PHOTO_READ
    MM_FILL_MEM_INFO(stMmapInfo.stJPDReadBufAddr, JPD_READ_BUF_ADR, JPD_READ_BUF_LEN, JPD_READ_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_PHOTO_SHAREMEMORY
    MM_FILL_MEM_INFO(stMmapInfo.stPhotoShareMemAddr, PHOTO_SHARE_MEM_ADR, PHOTO_SHARE_MEM_LEN, PHOTO_SHARE_MEM_MEMORY_TYPE);

    //E_MMSDK_BUF_VDPLAYER_AEON_DATA
    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerDataAddr, VDPLAYER_DATA_ADR, VDPLAYER_DATA_LEN, VDPLAYER_DATA_MEMORY_TYPE);

    //E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO
#ifdef VDPLAYER_BS_EXT_AVAILABLE
    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerBSExtAddr, VDPLAYER_BS_EXT_ADR, VDPLAYER_BS_EXT_LEN, VDPLAYER_BS_EXT_MEMORY_TYPE);
#else
    MS_S32 s32MstarNonCachedPoolID=INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    _gpBSAudBufVA = MsOS_AllocateMemory(MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO + BITSTREAM_AUDIO_BUFFER_ALIGNMENT, s32MstarNonCachedPoolID);
    if(!_gpBSAudBufVA)
        MM_MUST("ERROR!!! No enough free mem for E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO!\n");

    MS_PHY PhyBSAudBufPA = MsOS_VA2PA((MS_VIRT)MEMALIGN(BITSTREAM_AUDIO_BUFFER_ALIGNMENT, (MS_VIRT)_gpBSAudBufVA));

    MS_U32 u32BSAudioMemType = 0;
    if(PhyBSAudBufPA & MIU_INTERVAL)
        u32BSAudioMemType |= MIU1;
    else
        u32BSAudioMemType |= MIU0;

    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerBSExtAddr, PhyBSAudBufPA, MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO, u32BSAudioMemType);
#endif

    //E_MMSDK_BUF_VDPLAYER_BITSTREAM_SUBTITLE
    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerSubBSAddr, VDPLAYER_SUB_BS_ADR, VDPLAYER_SUB_BS_LEN, VDPLAYER_SUB_BS_MEMORY_TYPE);

    //E_MMSDK_BUF_VDPLAYER_BITSTREAM_VIDEO
    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerBSAddr, VDPLAYER_BS_ADR, VDPLAYER_BS_LEN, VDPLAYER_BS_MEMORY_TYPE);

    //E_MMSDK_BUF_VDPLAYER_SHAREMEMORY: /* needed size >= 4K*/
    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerShmMemAddr, VDPLAYER_SHARE_MEM_ADR, VDPLAYER_SHARE_MEM_LEN, VDPLAYER_SHARE_MEM_MEMORY_TYPE);

    //E_MMSDK_BUF_SUBTITLE_PUSH
    MM_FILL_MEM_INFO(stMmapInfo.stVdplayerSubAddr, VDPLAYER_SUB_ADR, VDPLAYER_SUB_LEN, VDPLAYER_SUB_MEMORY_TYPE);

    //E_MMSDK_BUF_DEMUX_SEC
    MM_FILL_MEM_INFO(stMmapInfo.stTspFWAddr, TSP_FW_BUF_ADR, TSP_FW_BUF_LEN, TSP_FW_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_MAD_DECODE
#if (DEMO_AUDIO_R2_MEM_ARCHI_TEST)
    MM_FILL_MEM_INFO(stMmapInfo.stMADDecBufAddr, MAD_ADV_BUF_ADR, MAD_ADV_BUF_LEN, MAD_ADV_BUF_MEMORY_TYPE);
#else
    MM_FILL_MEM_INFO(stMmapInfo.stMADDecBufAddr, MAD_DEC_BUF_ADR, MAD_DEC_BUF_LEN, MAD_DEC_BUF_MEMORY_TYPE);
#endif

    //E_MMSDK_BUF_VDEC_CPU
    MM_FILL_MEM_INFO(stMmapInfo.stVdecAeonAddr, VDEC_AEON_ADR, VDEC_AEON_LEN, VDEC_AEON_MEMORY_TYPE);

    //E_MMSDK_BUF_VDEC_FRAMEBUFFER
    MM_FILL_MEM_INFO(stMmapInfo.stVdecFrameBufAddr, VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_LEN, VDEC_FRAME_BUF_MEMORY_TYPE);
#if (DEMO_MM_DUAL_TEST == 1)
    //E_MMSDK_BUF_VDEC_SUB_FRAMEBUFFER
    MM_FILL_MEM_INFO(stMmapInfo.stVdecSubFrameBufAddr, VDEC_FRAME_BUF1_ADR, VDEC_FRAME_BUF1_LEN, VDEC_FRAME_BUF1_MEMORY_TYPE);

    //E_MMSDK_BUF_VDEC_SUB_STREAMBUFFER
    MM_FILL_MEM_INFO(stMmapInfo.stVdecSubBitStreamAddr, VDEC_BIT_STREAM1_ADR, VDEC_BIT_STREAM1_LEN, VDEC_BIT_STREAM1_MEMORY_TYPE);
#endif
#if (DEMO_MM_MVC_TEST == 1)
    //E_MMSDK_BUF_VDEC_FRAMEBUFFER_MVC
    MM_FILL_MEM_INFO(stMmapInfo.stVdecMVCFrameBufAddr, VDEC_MVC_FRAME_BUF_ADR, VDEC_MVC_FRAME_BUF_LEN, VDEC_MVC_FRAME_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_VDEC_STREAMBUFFER_MVC
    MM_FILL_MEM_INFO(stMmapInfo.stVdecMVCBitStreamAddr, VDEC_MVC_BIT_STREAM_ADR, VDEC_MVC_BIT_STREAM_LEN, VDEC_MVC_BIT_STREAM_MEMORY_TYPE);
#endif
    //E_MMSDK_BUF_VDEC_FRAMEBUFFER_SD
    MM_FILL_MEM_INFO(stMmapInfo.stVdecFrameBufferSDAddr, VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_LEN, VDEC_FRAME_BUF_MEMORY_TYPE);

    //E_MMSDK_BUF_VDEC_STREAMBUFFER
    MM_FILL_MEM_INFO(stMmapInfo.stVdecBitStreamAddr, VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_LEN, VDEC_BIT_STREAM_MEMORY_TYPE);

    //E_MMSDK_BUF_XC_MAIN_FB
    MM_FILL_MEM_INFO(stMmapInfo.stXCMainFrameBufAddr, SC0_MAIN_FB_ADR, SC0_MAIN_FB_LEN, SC0_MAIN_FB_MEMORY_TYPE);

    //E_MMSDK_BUF_PHY_ADDR_FOR_COPY_DATA (need to check)
    MM_FILL_MEM_INFO(stMmapInfo.stPHYAddrForCopyDataAddr, PHY_COPY_DATA_ADR, PHY_COPY_DATA_LEN, PHY_COPY_DATA_MEMORY_TYPE);

#if (DEMO_MM_AVP_TEST == 1)
    // E_MMSDK_BUF_XC_DS
    MM_FILL_MEM_INFO(stMmapInfo.stXCMCUDynamicScalingAddr, SC0_MCU_DS_ADR, SC0_MCU_DS_LEN, SC0_MCU_DS_MEMORY_TYPE);
#endif

#ifdef XC_HDR_ADR
    // E_MMSDK_BUF_DV_XC
    MM_FILL_MEM_INFO(stMmapInfo.stXCHdrAddr, XC_HDR_ADR, XC_HDR_LEN, XC_HDR_MEMORY_TYPE);
#endif

    PT_SYS_SetMmapInfo(&stMmapInfo, &stMIUInfo);

    return TRUE;
}

static const MS_U8 *_Demo_GetTrickModeString(EN_MMSDK_TRICK_MODE eMode)
{
    switch(eMode)
    {
        CASE_FOR_STR(E_MMSDK_TRICK_PAUSE);
        CASE_FOR_STR(E_MMSDK_TRICK_NORMAL);
        CASE_FOR_STR(E_MMSDK_TRICK_FF_2X);
        CASE_FOR_STR(E_MMSDK_TRICK_FFS_2X);
        CASE_FOR_STR(E_MMSDK_TRICK_FF_4X);
        CASE_FOR_STR(E_MMSDK_TRICK_FF_8X);
        CASE_FOR_STR(E_MMSDK_TRICK_FF_16X);
        CASE_FOR_STR(E_MMSDK_TRICK_FF_32X);
        CASE_FOR_STR(E_MMSDK_TRICK_FB_2X);
        CASE_FOR_STR(E_MMSDK_TRICK_FB_4X);
        CASE_FOR_STR(E_MMSDK_TRICK_FB_8X);
        CASE_FOR_STR(E_MMSDK_TRICK_FB_16X);
        CASE_FOR_STR(E_MMSDK_TRICK_FB_32X);
        default: return (MS_U8*)"Unknown MMSDK Trick Mode";
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize multi-media function
/// @param none
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b MM_INIT \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Init(void)
{
    MS_BOOL bMute = FALSE;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID,s32MstarNonCachedPoolID=INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if (b_MM_Initied)
        return FALSE;

    _gpu8FileName=(MS_U8 *)MsOS_AllocateMemory(sizeof(MS_U8) * FILE_NAME_LENGTH, s32MstarNonCachedPoolID);
    if(!_gpu8FileName)
        return FALSE;
    memset(_gpu8FileName, 0x00, sizeof(MS_U8)*FILE_NAME_LENGTH);

    //Prepare sys related info before MMSDK_Initialize()
    PT_SYS_SetCusInfo(Customer_info, Customer_hash);
    PT_SYS_SetMemPoolID(s32MstarCachedPoolID, s32MstarNonCachedPoolID);
    PT_SYS_SetTspVQInfo(TSP_VQ_BUF_ADR, TSP_VQ_BUF_LEN);
    PT_SYS_SetDBReadWrite(_Demo_PT_SYS_ReadDB, _Demo_PT_SYS_WriteDB);
    PT_SYS_SetDRMID(DRM_MODEL_ID);//to revise? by teddy.chen
    _Demo_PT_SYS_MemInit();

    //Create and initialize MM player.
    _Demo_MPlayer_Initialize();

    //Force disable SLIDE SHOW for MVOP/GOP show photo behavior are consistent
    //Set media item id = invalid means the option do not need it
    // coverity[overrun-call]
    _Demo_MPlayer_SetOption(MMSDK_MEDIA_ITEM_MIN, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_DISABLE_SLIDE_SHOW, TRUE);

    //For image type subtitle
    msAPI_MM_Scaler_ForceOpen();

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);

    //audio de-mute and initial volume setting , move from mm mdw
    bMute = FALSE;
    Demo_Audio_SetMute(&bMute);

    b_MM_Initied = TRUE;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to De-Initialize multi-media function
/// @param none
/// @return TRUE: De-Initial success.
/// @return FALSE: De-Initial fail.
/// @sa
/// @note
/// Command: \b MM_DEINIT \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_DeInit(void)
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if (b_MM_Initied == FALSE)
        return FALSE;

    MsOS_FreeMemory(_gpu8FileName, s32MstarNonCachedPoolID);
    _gpu8FileName=NULL;

    _Demo_MPlayer_Finalize();

    b_MM_Initied = FALSE;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to goto specific position of multi-media file
/// @param[in] pGotoTimeMs seek position, unit is millisecond
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_SEEK \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Seek(MS_U32 *pGotoTimeMs)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_BOOL bRet = FALSE;

    if(_gItem[u32Idx][_geMediaType] == 0 || pGotoTimeMs == NULL)
    {
        return FALSE;
    }

    if(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_ES_STREAM)
    {
        MsOS_ObtainMutex(_gs32MutexESPlayer, MSOS_WAIT_FOREVER);
        if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], E_MMSDK_TRICK_PAUSE) == FALSE)
        {
            goto ES_SEEK_FAIL;
        }
        MM_ES_FLUSH_DATA(u32Idx, _geMediaType, *pGotoTimeMs, E_MMSDK_ES_DATA_TYPE_VIDEO_AUDIO);
        if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], E_MMSDK_TRICK_NORMAL) == FALSE)
        {
            goto ES_SEEK_FAIL;
        }

        bRet = TRUE;

ES_SEEK_FAIL:
        MsOS_ReleaseMutex(_gs32MutexESPlayer);
        if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && bRet == TRUE)    MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);
        return bRet;
    }
    {
        MS_U32 u32Value = 0;
        if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)//for mantis 0242114
        {
            u32Value = _Demo_MPlayer_GetOption(u32Idx, _geMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_TOTAL_TIME);
            if((u32Value > 0) && ((*pGotoTimeMs) > u32Value))
            {
                MM_ERR("Goto time exceeds file length (=%d ms)\n",(int)u32Value);
                return FALSE;
            }
        }

        if(_geMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
        {
            u32Value = _Demo_MPlayer_GetOption(u32Idx, _geMediaType, MPLAYER_OPTION_MUSIC_INFO, MPLAYER_INFO_TOTAL_TIME);
            if((u32Value > 0) && ((*pGotoTimeMs) > u32Value * 1000))
            {
                MM_ERR("Goto time exceeds file length (=%d s)\n", (int)u32Value);
                return FALSE;
            }
        }

        Demo_MM_Resume();

        _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
        if(MMSDK_SetPlayPosition(_gItem[u32Idx][_geMediaType], *pGotoTimeMs) != TRUE)
        {
            MM_ERR(" fail to set position.\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return FALSE;
        }

        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)    MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);

        return TRUE;
    }
}

static MS_BOOL _Demo_IsCapEnoughForFFx2(void)
{
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32Codec = 0;
    MS_U32 u32FrameRate = 0;
    MS_U32 u32Width = 0;
    MS_U32 u32Height = 0;

    MS_U32 u32Idx = _gu32CurrentMediaItem;

    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_MOVIE_VIDEO_CODEC, sizeof(MS_U32), (void*)&u32Codec, _gItem[u32Idx][E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_MOVIE_FRAME_PER_SEC, sizeof(MS_U32), (void*)&u32FrameRate, _gItem[u32Idx][E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_HRES, sizeof(MS_U32), (void*)&u32Width, _gItem[u32Idx][E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_VRES, sizeof(MS_U32), (void*)&u32Height, _gItem[u32Idx][E_MMSDK_MEDIA_TYPE_MOVIE]);

    MM_DBG("E_MMSDK_INFO_MOVIE_FRAME_PER_SEC = %d, E_MMSDK_INFO_HRES = %d, E_MMSDK_INFO_VRES = %d\n", (int)u32FrameRate, (int)u32Width, (int)u32Height);

#if defined(VDEC_VP9_FFX2_CAP)
    if((u32Codec == E_MMSDK_VIDEO_CODEC_VP9) && ((u32FrameRate/1000*2) * u32Width * u32Height > VDEC_VP9_FFX2_CAP))
        return FALSE;
#endif
#if defined(VDEC_H265_FFX2_CAP)
    if((u32Codec == E_MMSDK_VIDEO_CODEC_HEVC) && ((u32FrameRate/1000*2) * u32Width * u32Height > VDEC_H265_FFX2_CAP))
        return FALSE;
#endif
#if defined(VDEC_H264_FFX2_CAP)
    if((u32Codec == E_MMSDK_VIDEO_CODEC_H264) && ((u32FrameRate/1000*2) * u32Width * u32Height > VDEC_H264_FFX2_CAP))
        return FALSE;
#endif

    return TRUE;
}

static MS_BOOL _Demo_IsAudioOnly(void)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;

    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32VideoCodec = 0;
    MS_U32 u32AudioCodec = 0;

    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_MOVIE_VIDEO_CODEC, sizeof(MS_U32), (void*)&u32VideoCodec, _gItem[u32Idx][E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, sizeof(MS_U32), (void*)&u32AudioCodec, _gItem[u32Idx][E_MMSDK_MEDIA_TYPE_MOVIE]);

    if( (u32VideoCodec == E_MMSDK_VIDEO_CODEC_UNKNOWN) && (u32AudioCodec != E_MMSDK_AUDIO_CODEC_UNKNOWN) )
        return TRUE;
    else
        return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to fast-forward multi-media file
/// @param[in] None
/// @sa
/// @note
/// Command: \b MM_FF \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_FF(void)
{
    EN_MMSDK_TRICK_MODE eTrickMode;
    MS_U32 u32Idx = _gu32CurrentMediaItem;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gItem[u32Idx][_geMediaType] == 0)
    {
        MM_ERR(" invalid MediaItem.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    if(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_ES_STREAM)
    {
        MM_ERR("Test stream has no I-frame. Trick mode Fail\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    switch(_geTrickMod[u32Idx][_geMediaType])
    {
        case E_MMSDK_TRICK_PAUSE :
        case E_MMSDK_TRICK_NORMAL:eTrickMode=E_MMSDK_TRICK_FF_2X; break;
        case E_MMSDK_TRICK_FF_2X :eTrickMode=E_MMSDK_TRICK_FF_4X; break;
        case E_MMSDK_TRICK_FFS_2X :eTrickMode=E_MMSDK_TRICK_FF_4X; break;
        case E_MMSDK_TRICK_FF_4X :eTrickMode=E_MMSDK_TRICK_FF_8X; break;
        case E_MMSDK_TRICK_FF_8X :eTrickMode=E_MMSDK_TRICK_FF_16X;break;
        case E_MMSDK_TRICK_FF_16X:eTrickMode=E_MMSDK_TRICK_FF_32X;break;
        case E_MMSDK_TRICK_FF_32X:eTrickMode=E_MMSDK_TRICK_NORMAL;break;
        case E_MMSDK_TRICK_FB_2X :eTrickMode=E_MMSDK_TRICK_NORMAL;break;
        case E_MMSDK_TRICK_FB_4X :eTrickMode=E_MMSDK_TRICK_FB_2X; break;
        case E_MMSDK_TRICK_FB_8X :eTrickMode=E_MMSDK_TRICK_FB_4X; break;
        case E_MMSDK_TRICK_FB_16X:eTrickMode=E_MMSDK_TRICK_FB_8X; break;
        case E_MMSDK_TRICK_FB_32X:eTrickMode=E_MMSDK_TRICK_FB_16X;break;
        default:eTrickMode=E_MMSDK_TRICK_NORMAL;
    }

    if((eTrickMode == E_MMSDK_TRICK_FF_2X) && (_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE))
    {
        if(!_gbEnableFastForward )
        {
            if (!(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_AVP))
                eTrickMode = E_MMSDK_TRICK_FFS_2X;
        }
        else
        {
            if(_Demo_IsCapEnoughForFFx2() == FALSE)
            {
                MM_MUST("Do E_MMSDK_TRICK_FFS_2X instead of E_MMSDK_TRICK_FF_2X due to capability is not enough!!\n");
                eTrickMode = E_MMSDK_TRICK_FFS_2X;
            }
        }
    }

    if(eTrickMode == E_MMSDK_TRICK_FF_4X)
    {
        if(_Demo_IsAudioOnly() == TRUE)
        {
            MM_ERR(" Fail to do over FF_4X in Audio only mode!!\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return FALSE;
        }
    }

    if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], eTrickMode) == FALSE)
    {
        MM_ERR(" Fail to set trick mode.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    _geTrickMod[u32Idx][_geMediaType] = eTrickMode;

    MM_MUST("%s\n", _Demo_GetTrickModeString(eTrickMode));
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)    MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to back-forward multi-media file
/// @param[in] None
/// @sa
/// @note
/// Command: \b MM_RW \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_RW(void)
{
    EN_MMSDK_TRICK_MODE eTrickMode;
    MS_U32 u32Idx = _gu32CurrentMediaItem;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gItem[u32Idx][_geMediaType] == 0)
    {
        MM_ERR(" invalid MediaItem.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    if(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_ES_STREAM)
    {
        MM_ERR("Test stream has no I-frame. Trick mode Fail\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    switch(_geTrickMod[u32Idx][_geMediaType])
    {
        case E_MMSDK_TRICK_PAUSE :
        case E_MMSDK_TRICK_NORMAL:eTrickMode=E_MMSDK_TRICK_FB_2X; break;
        case E_MMSDK_TRICK_FF_2X :eTrickMode=E_MMSDK_TRICK_NORMAL;break;
        case E_MMSDK_TRICK_FFS_2X :eTrickMode=E_MMSDK_TRICK_NORMAL;break;
        case E_MMSDK_TRICK_FF_4X :eTrickMode=E_MMSDK_TRICK_FF_2X; break;
        case E_MMSDK_TRICK_FF_8X :eTrickMode=E_MMSDK_TRICK_FF_4X; break;
        case E_MMSDK_TRICK_FF_16X:eTrickMode=E_MMSDK_TRICK_FF_8X; break;
        case E_MMSDK_TRICK_FF_32X:eTrickMode=E_MMSDK_TRICK_FF_16X;break;
        case E_MMSDK_TRICK_FB_2X :eTrickMode=E_MMSDK_TRICK_FB_4X; break;
        case E_MMSDK_TRICK_FB_4X :eTrickMode=E_MMSDK_TRICK_FB_8X; break;
        case E_MMSDK_TRICK_FB_8X :eTrickMode=E_MMSDK_TRICK_FB_16X;break;
        case E_MMSDK_TRICK_FB_16X:eTrickMode=E_MMSDK_TRICK_FB_32X;break;
        case E_MMSDK_TRICK_FB_32X:eTrickMode=E_MMSDK_TRICK_NORMAL;break;
        default:eTrickMode=E_MMSDK_TRICK_NORMAL;
    }

    if(eTrickMode == E_MMSDK_TRICK_FF_2X)
        if(_Demo_IsCapEnoughForFFx2() == FALSE)
        {
            MM_MUST("Do E_MMSDK_TRICK_FFS_2X instead of E_MMSDK_TRICK_FF_2X due to capability is not enough!!\n");
            eTrickMode = E_MMSDK_TRICK_FFS_2X;
        }

    if(eTrickMode == E_MMSDK_TRICK_FB_2X)
        if(_Demo_IsAudioOnly() == TRUE)
        {
            MM_ERR(" Fail to do MM_FB in Audio only mode!!\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return FALSE;
        }

    if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], eTrickMode) == FALSE)
    {
        MM_ERR(" Fail to set trick mode.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    _geTrickMod[u32Idx][_geMediaType] = eTrickMode;

    MM_MUST("%s\n", _Demo_GetTrickModeString(eTrickMode));
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)    MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);

    return TRUE;
}

MS_BOOL Demo_MM_Step(MS_U32 *u32Cnt)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    EN_MMSDK_TRICK_MODE eTrickMode = E_MMSDK_TRICK_INVALID;
    MS_U32 u32Counter = 0;
    MS_BOOL bSysValue = TRUE;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);

    if(_gbPlay[u32Idx][_geMediaType] == TRUE)
    {
        PT_SYS_GetOptions(u32Idx, E_PT_SYS_STEP_DBG, &bSysValue);
        if(bSysValue == TRUE)
        {
            PT_SYS_SetOptions(u32Idx, E_PT_SYS_STEP_NEXTFRAME, &bSysValue);
            MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], E_MMSDK_TRICK_PAUSE);
        }

        for(u32Counter = 0; u32Counter < *u32Cnt; u32Counter++)
        {
            if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], E_MMSDK_TRICKMODE_STEP) == FALSE)
            {
                MM_ERR("Not in the pause mode, or no more frames !\n");
                bRet = FALSE;
                break;
            }
            else
            {
                _geTrickMod[u32Idx][_geMediaType] = eTrickMode;
                bRet = TRUE;
            }
        }
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)    MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);
    }
    else
    {
        MM_ERR(" Video is not playing!\n");
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL Demo_MM_StepPlay(MS_U32 *pFileOption, MS_U8* U8FileName)
{
    MS_BOOL bSysValue = TRUE;

    PT_SYS_SetOptions(0, E_PT_SYS_STEP_DBG, &bSysValue);

    bSysValue = FALSE;
    PT_SYS_SetOptions(0, E_PT_SYS_STEP_NEXTFRAME, &bSysValue);


    if (TRUE != Demo_MM_Play(pFileOption, U8FileName))
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to pause multi-media file
/// @param[in] None
/// @sa
/// @note
/// Command: \b MM_PAUSE \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Pause(void)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    EN_MMSDK_TRICK_MODE eTrickMode = E_MMSDK_TRICK_PAUSE;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gbPlay[u32Idx][_geMediaType] == TRUE)
    {
        if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], eTrickMode) == FALSE)
        {
            MM_ERR(" fail to pause media.\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return FALSE;
        }
    }

    _geTrickMod[u32Idx][_geMediaType] = eTrickMode;
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to resume multi-media file
/// @param[in] None
/// @sa
/// @note
/// Command: \b MM_RESUME \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Resume(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    EN_MMSDK_TRICK_MODE eTrickMode = E_MMSDK_TRICK_NORMAL;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);

    if(_gbPlay[u32Idx][_geMediaType] == TRUE)
    {
        if(_geTrickMod[u32Idx][_geMediaType] == E_MMSDK_TRICK_NORMAL)
        {
            MM_DBG("Now is normal play, do not resume again.\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return TRUE;
        }

        if(MMSDK_SetTrickMode(_gItem[u32Idx][_geMediaType], eTrickMode) == TRUE)
        {
            bRet = TRUE;
        }
    }

    if(bRet == FALSE)
    {
        MM_ERR(" fail to resume media.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    _geTrickMod[u32Idx][_geMediaType] = eTrickMode;
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)    MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);

    return bRet;
}

static void* _Demo_FileOpen(const char* const pathname,const char* const mode)//open stream, return stream handle
{//implemented by AP
    MM_TRACE("--Into %s --pathname %s --\n", __FUNCTION__, pathname);
    FILE* fp = MsFS_Fopen(pathname, "rb");
    if (NULL==fp)
    {
        MM_ERR("ERROR!!! fopen fail, errno=%d\n", errno);
    }

    return fp;
}

static int _Demo_FileClose(void* const stream)//close stream
{//implemented by AP
    return (MsFS_Fclose((FILE*)stream));
}

static int _Demo_FileSeek(void* const stream, const MS_U64 offset, const int fromwhere)//seek to postition
{//implemented by AP
    return (MsFS_Fseek((FILE*)stream, offset, fromwhere));
}

static int _Demo_FileRead(void* const ptr, const int size, const int nitems,  void* const stream)//read data from stream, return size of readed data
{//implemented by AP
    return (MsFS_Fread(ptr, size, nitems, (FILE *)stream));
}

static MS_U64 _Demo_FileLength(void* const stream)  //get stream length
{//implemented by AP
    MS_U64 len = 0;
    if(stream)
    {
        _Demo_FileSeek(stream, 0, SEEK_END);
        len = MsFS_Ftell((FILE*)stream);
        _Demo_FileSeek(stream, 0, SEEK_SET);
    }

    return len;
}

static MS_U32 _Demo_MM_GetAliveMediaItem(void)
{
    MS_U32 i=0, j=0;
    //MS_U32 ret = FALSE;

    for ( i = MMSDK_MEDIA_ITEM_MIN; i < MMSDK_MEDIA_ITEM_MAX; i++)
    {
        for ( j = MMSDK_MEDIA_TYPE_MIN; j < MMSDK_MEDIA_TYPE_NUM; j++)
        {
            if(_gItem[i][j] != 0)
            {
                return i;
            }
        }
    }
    return MMSDK_MEDIA_ITEM_INVALID;  // -1 means no alive media item
}

static MS_U32 _Demo_MM_GetEmptyMediaItem(const EN_MMSDK_MEDIA_TYPE eMediaType)
{
    MS_U32 i=0;

    for ( i = MMSDK_MEDIA_ITEM_MIN; i < MMSDK_MEDIA_ITEM_MAX; i++)
    {
        if(_gItem[i][eMediaType] != 0)
        {
                continue;
        }
        return i;
    }
    return MMSDK_MEDIA_ITEM_INVALID;
}

static MS_U32 _Demo_MM_GetMediaType(MS_U32 u32Idx)
{
    MS_U32 j=0;
    //MS_U32 ret = FALSE;

    for ( j = MMSDK_MEDIA_TYPE_MIN; j < MMSDK_MEDIA_TYPE_NUM; j++)
    {
        if(_gItem[u32Idx][j] != 0)
        {
                return j;
        }
    }
    return MMSDK_MEDIA_TYPE_INVALID;
}

BOOLEAN _Demo_MPlayer_Stop(const EN_MMSDK_MEDIA_TYPE eMediaType, const MS_U32 u32Idx)
{
    MM_TRACE("--------> enter _Demo_MPlayer_Stop---------------\n ");
    BOOLEAN bRet = FALSE;
    MEDIAITEM u32ItemTmp = NULL;
    MS_S32 u32IdxTmp = 0;
    MS_U32 u32MediaType = MMSDK_MEDIA_TYPE_INVALID;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_BOOL bSysValue = FALSE;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);

    u32MediaType = _Demo_MM_GetMediaType(u32Idx);
    if(_gu32FilePlayFlag[u32Idx][u32MediaType] & MPLAYER_PLAY_FLAG_ES_STREAM)
    {
        _gbExitParseThread = TRUE;
        _Demo_MM_DeleteParseEsTask();
        _Demo_MM_DeInitESDemuxerParameter();

        if (_gpu8FeedBuf != NULL)
        {
            MsOS_FreeMemory(_gpu8FeedBuf, s32MstarNonCachedPoolID);
            _gpu8FeedBuf = NULL;
        }
    }

    _geTrickMod[u32Idx][eMediaType] = E_MMSDK_TRICK_NORMAL;
    _gu64CurPostn[u32Idx][eMediaType] = 0;
    _gu64SubCurPostn[u32Idx] = 0;
    _gu64AudioCurPostn[u32Idx] = 0;
    _gu64Audio2CurPostn[u32Idx] = 0;
    _gePlayStatus[u32Idx] = E_MM_PLAY_STATUS_INVALID;

    _gbPlay[u32Idx][eMediaType] = FALSE;
    _gu32SeekPTS = 0;
    _gu32LastAudioPTS = 0xFFFFFFFF;

    PT_SYS_GetOptions(u32Idx, E_PT_SYS_STEP_DBG, &bSysValue);
    if(bSysValue == TRUE)
    {
        PT_SYS_SetOptions(u32Idx, E_PT_SYS_STEP_NEXTFRAME, &bSysValue);

        bSysValue = FALSE;
        PT_SYS_SetOptions(0, E_PT_SYS_STEP_DBG, &bSysValue);
    }

    if(MMSDK_Stop(_gItem[u32Idx][eMediaType]) != TRUE)
    {
        MM_ERR("-----> MM_Stop fail \n");
    }

    if(_gItem[u32Idx][eMediaType] != NULL)
    {
        u32ItemTmp = _gItem[u32Idx][eMediaType];
        _gItem[u32Idx][eMediaType] = NULL;
        MMSDK_DeleteMediaItem(u32ItemTmp);
    }

    if(eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
        _gu32SaveFilePos = 0;


        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            if(!(_gu32FilePlayFlag[u32Idx][eMediaType] & MPLAYER_PLAY_FLAG_AVP))
            {
                Demo_MM_FreeAudDec();
                if (Demo_Audio_Dynamic_Allocate_Resource() == TRUE)
                {
                    Demo_Audio_Release(&_gu32AudioDecoderId);
                }
            }
        }
    }
    else if(eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO || eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        _geRotaAng = E_MMSDK_ROTATE_0;
        _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
        _gu32GifAddrOffset = 0;
    }
    else
    {
        _gu32TotalTime = 0;
        _gu32StartTime = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
                Demo_MM_FreeAudDec();
                if (Demo_Audio_Dynamic_Allocate_Resource() == TRUE)
                {
                    Demo_Audio_Release(&_gu32AudioDecoderId);
                }
        }
    }
    _gstMedia.pCallback = NULL;

    if(_gpStream[u32Idx][eMediaType] != NULL)
    {
        _Demo_FileClose(_gpStream[u32Idx][eMediaType]);
        _gpStream[u32Idx][eMediaType] = NULL;
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_SUBTITLE])
        {
            _Demo_FileClose(_gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_SUBTITLE]);
            _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_SUBTITLE] = NULL;
        }
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO])
        {
            _Demo_FileClose(_gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO]);
            _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO] = NULL;
        }
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO2])
        {
            _Demo_FileClose(_gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO2]);
            _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO2] = NULL;
        }
    }

    MMSDK_SetBackwardRePlayMode(_gbBackwardRePlayMode);
    //_gbSeekRightNow[eMediaType] = FALSE;
    _gu32FilePlayFlag[u32Idx][eMediaType] = INVALID_PLAY_FLAG;
    bRet = TRUE;

    u32IdxTmp = _Demo_MM_GetAliveMediaItem();
    if (u32IdxTmp == MMSDK_MEDIA_ITEM_INVALID )
    {
        _gu32CurrentMediaItem = MMSDK_MEDIA_ITEM_MIN;
        MM_MUST("No Media Item active, set media item to default (%"DTC_MS_U32_d")\n", _gu32CurrentMediaItem);
    }
    else
    {
        _gu32CurrentMediaItem = u32IdxTmp;
        MM_MUST("Current Media Item Index = %"DTC_MS_U32_d" \n", _gu32CurrentMediaItem);
    }

    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    MM_TRACE("--------> leave _Demo_MPlayer_Stop---------------\n ");
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop play multi-media file
/// @param[in] None
/// @sa
/// @note
/// Command: \b MM_STOP \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Stop(void)
{
    if(_Demo_MPlayer_Stop(_geMediaType, _gu32CurrentMediaItem) == TRUE)
    {
        if(_geMediaType==E_MMSDK_MEDIA_TYPE_PHOTO)
            Demo_MM_ClearPhoto();
        return TRUE;
    }
    else
        return FALSE;

    return TRUE;
}

MS_BOOL Demo_MM_Stop_MediaType(MS_U32 * pMediaType)
{
    if(_Demo_MPlayer_Stop((EN_MMSDK_MEDIA_TYPE)*pMediaType, _gu32CurrentMediaItem) == TRUE)
    {
        if((EN_MMSDK_MEDIA_TYPE)*pMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
            Demo_MM_ClearPhoto();

        return TRUE;
    }
    else
        return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to repeatly play multi-media file
/// @param[in] pu32On: 1 for repeat on, 0 for repeat off
/// @sa
/// @note
/// Command: \b MM_Repeat \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Repeat(MS_U32 *pu32On)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    return Demo_MM_Repeat_EX(&u32Idx, pu32On);
}

MS_BOOL Demo_MM_Repeat_EX(MS_U32 *pu32ItemId, MS_U32 *pu32On)
{
    if((*pu32On != 0) && (*pu32On != 1))
        return FALSE;

    if(_gpu8FileName )
    {
        if(*pu32On)
        {
            _gu8IsRepeat = TRUE;
        }
        else
        {
            _gu8IsRepeat = FALSE;
        }
    }
    else
    {
        _gu8IsRepeat=FALSE;
        return FALSE;
    }

    MS_U32 u32Idx = *pu32ItemId;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gbPlay[u32Idx][_geMediaType] == TRUE)
    {
        if(MMSDK_SetLoop(_gItem[u32Idx][_geMediaType], _gu8IsRepeat) == FALSE)
        {
            MM_ERR(" fail to set loop.\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return FALSE;
        }
    }

    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to get information of multi-media file
/// @param[in] pOption enumeration of EN_MM_OPTION_TYPE in msAPI_MM.h
/// @param[in] pMMInfo enumeration of EN_MM_INFO in msAPI_MM.h
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_GETOPTION \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_GetOption(MS_U32 *pOption, MS_U32 *pMMInfo)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_U32 u32Value;
    u32Value = _Demo_MPlayer_GetOption(u32Idx, _geMediaType, (EN_MPLAYER_OPTION_TYPE)*pOption, (unsigned int)*pMMInfo);
    MM_MUST("GetOption Result = %"DTC_MS_U32_d"\n",u32Value);

    return TRUE;
}

static EN_MMSDK_MEDIA_TYPE _Demo_MapMediaStreamType2MediaType(EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType)
{
    if (eStreamType == E_MPLAYER_MEDIA_STREAM_PHOTO)
    {
        return E_MMSDK_MEDIA_TYPE_PHOTO;
    }
    else if (eStreamType == E_MPLAYER_MEDIA_STREAM_MUSIC)
    {
        return E_MMSDK_MEDIA_TYPE_MUSIC;
    }
    else
    {
        return E_MMSDK_MEDIA_TYPE_MOVIE;
    }
}

static int _Demo_Ringbuf_Seek(MS_U32 u32Hdl, MS_U64 u64SeekPos, int fromwhere, const MS_U32 u32ItemIdx)//seek to postition
{
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = (EN_MPLAYER_MEDIA_STREAM_TYPE)u32Hdl;
    EN_MMSDK_MEDIA_TYPE eMediaType = _Demo_MapMediaStreamType2MediaType(eStreamType);
    MS_U32 u32Ret = 0; //be cautious , 0 indicates OK; 1 means failure!!!!!
    //_gbSeekRightNow[eMediaType] = TRUE;
    MM_TRACE("-----> Enter _Demo_Ringbuf_Seek %d\n",(int)u64SeekPos);

    if(_gu32FilePlayFlag[u32ItemIdx][eMediaType] & MPLAYER_PLAY_FLAG_FILEIN)
    {
        int seekOff = -1;
        off_t offset;
        if(sizeof(off_t) > 4)
        {
            offset = u64SeekPos;
        }
        else
        {
            offset = u64SeekPos & 0xFFFFFFFF;
            MM_DBG("Warning!!!!!!, sizeof(off_t) is not equal to sizeof(unsinged long long)\n");
        }

        void* pStream = NULL;
        pStream = _gpStream[u32ItemIdx][eStreamType];

        seekOff =  _Demo_FileSeek(pStream, offset, SEEK_SET);
        u32Ret = (seekOff >= 0) ? 0 : 1;
    }

    return u32Ret;
}

static int _Demo_Ringbuf_Read(void *pBuffAddr, int size, MS_U32 u32Length , MS_BOOL bBlockRead, MS_U32 u32Hdl, const MS_U32 u32ItemIdx)
{
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = (EN_MPLAYER_MEDIA_STREAM_TYPE)u32Hdl;
    EN_MMSDK_MEDIA_TYPE eMediaType = _Demo_MapMediaStreamType2MediaType(eStreamType);
    //MM_DBG("-----> Enter _Demo_Ringbuf_Read, %d\n", (int)u32Length);

    if(_gu32FilePlayFlag[u32ItemIdx][eMediaType] & MPLAYER_PLAY_FLAG_FILEIN)
    {
        void* pStream = NULL;
        pStream = _gpStream[u32ItemIdx][eStreamType];

        int rdCnt = _Demo_FileRead(pBuffAddr, sizeof(MS_U8), u32Length, pStream);
        if(rdCnt <= 0)
        {
            return 0;
        }
        else
        {
            return rdCnt;
        }
    }

    return u32Length;
}

static EN_MPLAYER_MEDIA_STREAM_TYPE _Demo_MapFileID2MediaStreamType(void* pFileHandle, const MMSDK_U32 u32ItemIdx)
{
    MS_S32 i;

    for(i = MMSDK_MEDIA_TYPE_MIN; i < MMSDK_MEDIA_TYPE_NUM; i++)
    {
        if(pFileHandle == _gpStream[u32ItemIdx][i])
        {
            //MM_DBG("_Demo_MapFileID2MediaStreamType=%"DTC_MS_S32_d" \n", i);
            return (EN_MPLAYER_MEDIA_STREAM_TYPE)i;
        }
    }
    if(pFileHandle == _gpStream[u32ItemIdx][E_MPLAYER_MEDIA_STREAM_AUDIO])
    {
        //MM_DBG("_Demo_MapFileID2MediaStreamType=%"DTC_MS_U32_d" \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_AUDIO);
        return E_MPLAYER_MEDIA_STREAM_AUDIO;
    }
    if(pFileHandle == _gpStream[u32ItemIdx][E_MPLAYER_MEDIA_STREAM_AUDIO2])
    {
        //MM_DBG("_Demo_MapFileID2MediaStreamType=%"DTC_MS_U32_d" \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_AUDIO2);
        return E_MPLAYER_MEDIA_STREAM_AUDIO2;
    }
    if(pFileHandle == _gpStream[u32ItemIdx][E_MPLAYER_MEDIA_STREAM_SUBTITLE])
    {
        //MM_DBG("_Demo_MapFileID2MediaStreamType=%"DTC_MS_U32_d" \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_SUBTITLE);
        return E_MPLAYER_MEDIA_STREAM_SUBTITLE;
    }
    MM_ERR("_Demo_MapFileID2MediaStreamType=%"DTC_MS_U32_d" \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_INVALID);
    return E_MPLAYER_MEDIA_STREAM_INVALID;
}

static void _Demo_MM_AssignESDataType(ST_MMSDK_ES_DATA_INFO* pstESPKT, const MS_U8 au8Header[])
{
    if ((au8Header[0] == GetByte3(ES_VDP_SUPPORT_AUDIO)) &&
        (au8Header[1] == GetByte2(ES_VDP_SUPPORT_AUDIO)) &&
        (au8Header[2] == GetByte1(ES_VDP_SUPPORT_AUDIO)) &&
        (au8Header[3] == GetByte0(ES_VDP_SUPPORT_AUDIO)))
    {
        if ((au8Header[12] == GetByte3(ES_CODECINFO_HEADER)) &&
             (au8Header[13] == GetByte2(ES_CODECINFO_HEADER)) &&
             (au8Header[14] == GetByte1(ES_CODECINFO_HEADER)) &&
             (au8Header[15] == GetByte0(ES_CODECINFO_HEADER)))
        {
            pstESPKT->enDataType = E_MMSDK_ES_DATA_INFO_AUDIO;
        }
        else
        {
            pstESPKT->enDataType = E_MMSDK_ES_DATA_TYPE_AUDIO;
        }
    }
    else if ((au8Header[0] == GetByte3(ES_VDP_SUPPORT_VIDEO)) &&
              (au8Header[1] == GetByte2(ES_VDP_SUPPORT_VIDEO)) &&
              (au8Header[2] == GetByte1(ES_VDP_SUPPORT_VIDEO)) &&
              (au8Header[3] == GetByte0(ES_VDP_SUPPORT_VIDEO)))
    {
        if ((au8Header[12] == GetByte3(ES_CODECINFO_HEADER)) &&
             (au8Header[13] == GetByte2(ES_CODECINFO_HEADER)) &&
             (au8Header[14] == GetByte1(ES_CODECINFO_HEADER)) &&
             (au8Header[15] == GetByte0(ES_CODECINFO_HEADER)))
        {
            pstESPKT->enDataType = E_MMSDK_ES_DATA_INFO_VIDEO;
        }
        else
        {
            pstESPKT->enDataType = E_MMSDK_ES_DATA_TYPE_VIDEO;
        }
    }
}


static void _Demo_MM_AssignESDataInfo(ST_MMSDK_ES_DATA_INFO* pstESPKT, const MS_U8 au8Header[], const MS_U64 u64CurPos)
{
    _Demo_MM_AssignESDataType(pstESPKT, au8Header);

    // Starting address
    pstESPKT->u32StartAddr = u64CurPos + ES_PKT_HEADER_SIZE;

    /// Size
    pstESPKT->u32Size |=  (au8Header[4] << 24) & 0xff000000;
    pstESPKT->u32Size |=  (au8Header[5] << 16) & 0x00ff0000;
    pstESPKT->u32Size |=  (au8Header[6] <<  8) & 0x0000ff00;
    pstESPKT->u32Size |=  (au8Header[7])       & 0x000000ff;

    /// Time stamp
    pstESPKT->u32TimeStamp |=  (au8Header[8] << 24) & 0xff000000;
    pstESPKT->u32TimeStamp |=  (au8Header[9] << 16) & 0x00ff0000;
    pstESPKT->u32TimeStamp |=  (au8Header[10] << 8) & 0x0000ff00;
    pstESPKT->u32TimeStamp |=  (au8Header[11])      & 0x000000ff;
}

static void _Demo_MM_RepeatESStream(MS_U32 u32Idx, EN_MMSDK_MEDIA_TYPE eMediaType, MS_BOOL bRepeat)
{
    MS_U32 u32BitStreamBufferLevel;
    MEDIAITEM pMediaItem = _gItem[u32Idx][eMediaType];

    MsOS_ObtainMutex(_gs32MutexESPlayer, MSOS_WAIT_FOREVER);
    if ((_gu64CurPostn[u32Idx][eMediaType] >= _gu64Size[u32Idx][eMediaType]) && (ES_BUFFER_IS_EMPTY))
    {
        if(bRepeat)
        {
            MM_MUST("Repeat stream. Seek to file start.\n");
            MMSDK_ES_GetDataBufferInfo(pMediaItem, E_MMSDK_VIDEO_BUFFER_LEVEL_TIME, &u32BitStreamBufferLevel);

            if(u32BitStreamBufferLevel > 0)
            {
                MsOS_DelayTask(20);
            }
            else
            {
                MM_ES_FLUSH_DATA(u32Idx, eMediaType, 0, E_MMSDK_ES_DATA_TYPE_VIDEO_AUDIO);
            }
        }
        else
        {
            _gbExitParseThread = TRUE;
            MM_MUST("It is EOF, ES push finished.\n");
            MMSDK_SetDataEnd(pMediaItem);
        }
    }
    MsOS_ReleaseMutex(_gs32MutexESPlayer);
}

static MS_BOOL _Demo_MM_ValidAudioPTS(ST_MMSDK_ES_DATA_INFO *pstESDataInfo, MS_U32 u32LastAPTS)
{
    MS_BOOL bRet = TRUE;

    if(pstESDataInfo->enDataType == E_MMSDK_ES_DATA_TYPE_AUDIO)
    {
        if(pstESDataInfo->u32TimeStamp == u32LastAPTS)
        {
            bRet = FALSE;
        }
    }
    return bRet;
}

static MS_BOOL _Demo_MM_PushES(MS_U32 u32Idx, EN_MMSDK_MEDIA_TYPE eMediaType)
{
    MS_BOOL bRet = FALSE;
    ST_MMSDK_ES_DATA_INFO stData;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    memset(&stData, 0, sizeof(stData));

    MsOS_ObtainMutex(_gs32MutexESPlayer, MSOS_WAIT_FOREVER);
    if (!(ES_BUFFER_IS_EMPTY) && (_gbStartPush == TRUE))
    {
        int ReadSize = 0;

        // get codec info
        stData.enDataType = ES_BUFFER_CURRENT_POP.enDataType;
        stData.u32Size = ES_BUFFER_CURRENT_POP.u32Size;
        stData.u32TimeStamp = ES_BUFFER_CURRENT_POP.u32TimeStamp;
        _Demo_FileSeek(_gpEsFileHandle, ES_BUFFER_CURRENT_POP.u32StartAddr, SEEK_SET);

        ReadSize = _Demo_FileRead(_gpu8FeedBuf, sizeof(MS_U8), stData.u32Size, _gpEsFileHandle);
        if (ReadSize != stData.u32Size)
        {
            MM_ERR("fread pkt size fail, need read:0x%"DTC_MS_U32_x", only read:0x%lx\n", stData.u32Size, (U32)ReadSize);
            MsOS_DelayTask(10);

            if (_gpu8FeedBuf != NULL)
            {
                ES_BUFFER_CURRENT_POP_DONE();
            }
            MsOS_ReleaseMutex(_gs32MutexESPlayer);
            return FALSE;
        }

//        MM_DBG("[ES_PUSH], Addr:%"DTC_MS_U32_u", Size:%"DTC_MS_U32_u", Time:%"DTC_MS_U32_u", Type:%d\n",
//            ES_BUFFER_CURRENT_POP.u32StartAddr, ES_BUFFER_CURRENT_POP.u32Size,
//            ES_BUFFER_CURRENT_POP.u32TimeStamp, ES_BUFFER_CURRENT_POP.enDataType);
        stData.u32StartAddr = (U32)_gpu8FeedBuf;

        ST_MMSDK_ES_DATA_INFO stEDataInfo;
        memset(&stEDataInfo, 0, sizeof(ST_MMSDK_ES_DATA_INFO));
        stEDataInfo.u32StartAddr = stData.u32StartAddr;
        stEDataInfo.u32Size = stData.u32Size;
        stEDataInfo.u32TimeStamp = stData.u32TimeStamp;
        stEDataInfo.enDataType = (EN_MMSDK_ES_DATA_TYPE)stData.enDataType;

        if (stEDataInfo.u32TimeStamp < _gu32SeekPTS)
        {
            if (_gu64CurPostn[u32Idx][eMediaType] >= _gu64Size[u32Idx][eMediaType])
            {
                MM_MUST("Seek time is larger than the stream length\n");
            }
            //MM_DBG("\033[1;36m stEDataInfo.u32TimeStamp = %ld \033[0m\n", stEDataInfo.u32TimeStamp);
            ES_BUFFER_CURRENT_POP_DONE();
            bRet = TRUE;
        }
        else
        {
            if(!(_Demo_MM_ValidAudioPTS(&stEDataInfo, _gu32LastAudioPTS)))
            {
                stEDataInfo.u32TimeStamp = 0xFFFFFFFF;   // if audio PTS is invalid, send -1 to mmsdk
            }

            MMSDK_ES_SetESCusHeaderInfo(NULL, 0);
            if (MMSDK_ES_PushPayload(_gItem[u32Idx][eMediaType], &stEDataInfo))
            {
                //MM_DBG("\033[1;36m OK, rptr = %d, wptr = %d \033[0m\n",  _gu8ESBufferRptr, _gu8ESBufferWptr);
                if(stEDataInfo.enDataType == E_MMSDK_ES_DATA_TYPE_AUDIO)
                    _gu32LastAudioPTS = stEDataInfo.u32TimeStamp;
                ES_BUFFER_CURRENT_POP_DONE();
                bRet = TRUE;
            }
            else
            {
                //MM_DBG("\033[1;36m FAIL, rptr = %d, wptr = %d \033[0m\n", _gu8ESBufferRptr, _gu8ESBufferWptr);
                MsOS_DelayTask(10);
            }
        }
    }
    MsOS_ReleaseMutex(_gs32MutexESPlayer);
    return bRet;
}

static void _Demo_MM_ParseES(MS_U32 argc, void *arg)
{
    MS_U32 u32ItemIdx = _gu32CurrentMediaItem;
    MS_U32  pu32RetrievedEventFlag;
    EN_MMSDK_MEDIA_TYPE eMediaType = _geMediaType;
    MS_U8 u8Header[ES_PKT_HEADER_SIZE];
    ST_MMSDK_ES_DATA_INFO stESPKT;

    _gu64CurPostn[u32ItemIdx][eMediaType] = (ES_AUDIO_PARAM_SIZE + ES_VIDEO_PARAM_SIZE) * 4;
    _gbExitParseThread = FALSE;

    while(_gbExitParseThread == FALSE)
    {
        if(_gbWaitEsFlushDataReady)
        {
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_ES_DATA_FLUSH_READY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            ES_BUFFER_CLEAR();
            _gbWaitEsFlushDataReady = FALSE;
            _gbStartPush = TRUE;
        }
        MsOS_ObtainMutex(_gs32MutexESPlayer, MSOS_WAIT_FOREVER);
        if(_gu64CurPostn[u32ItemIdx][eMediaType] < _gu64Size[u32ItemIdx][eMediaType])
        {
            int ReadSize = 0;
            memset(&stESPKT, 0, sizeof(ST_MMSDK_ES_DATA_INFO));
            _Demo_FileSeek(_gpEsFileHandle, _gu64CurPostn[u32ItemIdx][eMediaType], SEEK_SET);
            ReadSize = _Demo_FileRead(&u8Header[0], sizeof(MS_U8), ES_PKT_HEADER_SIZE, _gpEsFileHandle);

            if (ReadSize != ES_PKT_HEADER_SIZE)
            {
                MM_ERR("fread pkt header fail\n");
                continue;
            }

            _Demo_MM_AssignESDataInfo(&stESPKT, u8Header, _gu64CurPostn[u32ItemIdx][eMediaType]);

            // insert pkt to vector
            while(ES_BUFFER_IS_FULL)
            {
                MM_ERR("User ES buffer is full...\n");
                MsOS_DelayTask(500);

                if(_gbExitParseThread == TRUE)  break;
            }
            memcpy(ES_BUFFER_CURRENT_PUSH, &stESPKT, sizeof(ST_MMSDK_ES_DATA_INFO));
            ES_BUFFER_CURRENT_PUSH_DONE();

            _gu64CurPostn[u32ItemIdx][eMediaType] += ES_PKT_HEADER_SIZE + stESPKT.u32Size;
        }
        MsOS_ReleaseMutex(_gs32MutexESPlayer);

        // Push data into MMSDK
        while (!(ES_BUFFER_IS_EMPTY) && (TRUE == _gbStartPush) && (_Demo_MM_PushES(u32ItemIdx, eMediaType) == FALSE));

        _Demo_MM_RepeatESStream(u32ItemIdx, eMediaType, _gu8IsRepeat);
    }
}

static MS_BOOL _Demo_MM_CreateParseEsTask(void)
{
    //return TRUE;

    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
    MS_BOOL bRet = FALSE;

    if(_gThreadPushEsProcess.iId == -1)
    {
        if(TH_EVT_PROC_STACK_SIZE)
        {
            _gThreadPushEsProcess.pStack = MsOS_AllocateMemory(_gThreadPushEsProcess.u32StackSize, s32MstarNonCachedPoolID);
            if(!_gThreadPushEsProcess.pStack)
            {
                 MM_ERR("_gThreadPushEsProcess stack malloc failed......\n");
                 return bRet;
            }
        }
    }
    void (*EvtProcess)(MS_U32, void*);
    EvtProcess = _Demo_MM_ParseES;

    _gThreadPushEsProcess.iId = MsOS_CreateTask(EvtProcess,
                                    (MS_U32)NULL,
                                    _gThreadPushEsProcess.ePriority,
                                    TRUE,
                                    _gThreadPushEsProcess.pStack,
                                    _gThreadPushEsProcess.u32StackSize,
                                    _gThreadPushEsProcess.szName);

    if(_gThreadPushEsProcess.iId == -1)
    {
        MM_ERR("create Thread _gpEvtProcess failed......\n");
        return bRet;
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
}

static MS_BOOL _Demo_MM_DeleteParseEsTask(void)
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if (_gThreadPushEsProcess.iId != -1)
    {
#if defined(MSOS_TYPE_ECOS)
        _Demo_PollingThreadStatus(_gThreadPushEsProcess);
#endif
        _gThreadPushEsProcess.iId = -1;
        if (_gThreadPushEsProcess.pStack)
        {
            MsOS_FreeMemory(_gThreadPushEsProcess.pStack, s32MstarNonCachedPoolID);
            _gThreadPushEsProcess.pStack = NULL;
        }
    }

    return TRUE;
}

static void _Demo_MM_InitESDemuxerParameter(void)
{
    MsOS_ObtainMutex(_gs32MutexESPlayer, MSOS_WAIT_FOREVER);
    ES_BUFFER_CLEAR();
    _gbStartPush = FALSE;
    MsOS_ReleaseMutex(_gs32MutexESPlayer);
}

static void _Demo_MM_DeInitESDemuxerParameter(void)
{
    MsOS_ObtainMutex(_gs32MutexESPlayer, MSOS_WAIT_FOREVER);
    // clear ES vector
    if (!(ES_BUFFER_IS_EMPTY) && (_gbStartPush == FALSE))
    {
        ES_BUFFER_CLEAR();
    }
    _gbStartPush = FALSE;
    MsOS_ReleaseMutex(_gs32MutexESPlayer);
}

MS_BOOL _Demo_MM_Callback_Photo(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo, const MS_U32 u32ItemIdx)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_NOTIFY_INFO *pNotifyInfo = (ST_MMSDK_CALLBACK_NOTIFY_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;

    MS_BOOL bRet = FALSE;
    //MM_DBG("eEvent is  %d\n", (int)eEvent);
    switch(eEvent)
    {
        /// The notify event is file read request.
        case E_MMSDK_CALLBACK_FILE_READ:
            //MM_DBG("Get E_MMSDK_CALLBACK_FILE_READ event. need %ld bytes data\n", pFileEvent->u32FileRequestSize);
            memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
            pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8),  pFileEvent->u32FileRequestSize, !_gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO], (MS_U32)E_MMSDK_MEDIA_TYPE_PHOTO, u32ItemIdx);
            _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = pstFileAck->u32ReadSize;
            if (_gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] >= _gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO])
            {
                pstFileAck->bIsFileEnd = TRUE;
            }

            bRet = TRUE;
            break;

        /// The notify event is file seek request.
        case E_MMSDK_CALLBACK_FILE_SEEK:
            //MM_DBG("Get E_EVENT_FILE_SEEK event.\n");
            if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO])
            {
                if(_Demo_Ringbuf_Seek(E_MMSDK_MEDIA_TYPE_PHOTO, pFileEvent->u64FileSeekPos, SEEK_SET, u32ItemIdx) == 0)
                {
                    _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = pFileEvent->u64FileSeekPos;
                    bRet = TRUE;
                }
                else
                {
                    MM_ERR("File seek fail.\n");
                }
            }
            break;

            /// The notify event is file tell request.
        case E_MMSDK_CALLBACK_FILE_TELL:
            //MM_DBG("Get E_MMSDK_CALLBACK_FILE_TELL event.\n");
            memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
            pstFileAck->u64FilePosition = _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO];
            bRet = TRUE;
            break;

            /// The notify event is file init OK.
        case E_MMSDK_CALLBACK_INIT_OK:
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_START_PLAY);
            MM_DBG("E_MMSDK_CALLBACK_INIT_OK: Send MPLAYER_START_PLAY to Photo_EvtProcess.\n");
            break;

            /// The notify event is file init fail.
        case E_MMSDK_CALLBACK_INIT_FAIL:
            MM_DBG("Get E_MMSDK_CALLBACK_INIT_FAIL event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;

            /// The notify event is invalid file.
        case E_MMSDK_CALLBACK_INVALID_FILE:
            MM_DBG("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;

            /// The notify event is thumbnail decoding done.
        case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
            break;

            /// The notify event is thumbnail decoding error.
        case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
            MM_DBG("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
            break;

            /// The notify event is decoding error.
        case E_MMSDK_CALLBACK_DECODE_ERROR:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_FAILED);
            break;

            /// The notify event is decoding done.
        case E_MMSDK_CALLBACK_DECODE_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_DONE);
            break;

            /// The notify event is video first frame decoding done.
        case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_DONE);
            break;

        /// The notify event is video one frame decoding done.
        case E_MMSDK_CALLBACK_DECODE_ONE_FRAME_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_ONE_FRAME_DONE event.\n");
            _gu32GifAddrOffset = pNotifyInfo->u32Ret;
            if(Demo_MM_StartDrawPhoto())
            {
                Demo_MM_DrawPhoto();
            }
            break;

        default:
            break;
    }

    return bRet;
}

static MS_BOOL _Demo_MM_Callback_Music(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo, const MS_U32 u32ItemIdx)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;
    MS_U8 *p_stWaveHdr = (MS_U8 *)&_gstWaveHdr;
    MS_BOOL bRet = FALSE;

    switch(eEvent)
    {
    /// The notify event is file read request.
    case E_MMSDK_CALLBACK_FILE_READ:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        //MM_DBG("-------> E_MMSDK_CALLBACK_FILE_READ Needs %d\n", (int)pFileEvent->u32FileRequestSize);
        if(_geAudioType == MPLAYER_SUBTYPE_PCM && _gu32SaveFilePos <= sizeof(_gstWaveHdr)) // just PCM audio format
        {
            if((_gu32SaveFilePos < sizeof(_gstWaveHdr)) &&
                (pFileEvent->u32FileRequestSize + _gu32SaveFilePos < sizeof(_gstWaveHdr)))// within WAV header
            {
                memcpy(pFileEvent->pu8FileBuff, p_stWaveHdr + _gu32SaveFilePos, pFileEvent->u32FileRequestSize);
                pstFileAck->u32ReadSize = pFileEvent->u32FileRequestSize;
                _gu32SaveFilePos += pstFileAck->u32ReadSize;
            }
            else // within both WAV header
            {
                memcpy(pFileEvent->pu8FileBuff, p_stWaveHdr + _gu32SaveFilePos, sizeof(_gstWaveHdr) - _gu32SaveFilePos);
                pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos, sizeof(MS_U8), pFileEvent->u32FileRequestSize - (sizeof(_gstWaveHdr) - _gu32SaveFilePos), !_gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC], (MS_U32)E_MMSDK_MEDIA_TYPE_MUSIC, u32ItemIdx);
                _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] += pstFileAck->u32ReadSize;
                if(_gu8AudioPara[9] == 0)
                {
                    MS_U16 i = 0;
                    if(*((U16*)(_gu8AudioPara + 3)) == 16)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 2)
                        {
                            B_ENDIAN2L_ENDIAN16(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos + i);//Name error, big endian, but in fact little endian
                        }
                    }
                    else if(*((U16*)(_gu8AudioPara + 3)) == 32)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 4)
                        {
                            B_ENDIAN2L_ENDIAN16(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos + i);
                        }
                    }
                }

                pstFileAck->u32ReadSize = pFileEvent->u32FileRequestSize;
                _gu32SaveFilePos += pFileEvent->u32FileRequestSize;
            }
        }
        else
        {
            pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8), pFileEvent->u32FileRequestSize, !_gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC], (MS_U32)E_MMSDK_MEDIA_TYPE_MUSIC, u32ItemIdx);
            _gu32SaveFilePos += pstFileAck->u32ReadSize;
            _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] +=  pstFileAck->u32ReadSize;
            if(_geAudioType == MPLAYER_SUBTYPE_PCM)
            {
                if(_gu8AudioPara[9] == 0)
                {
                    MS_U16 i = 0;
                    if(*((U16*)(_gu8AudioPara + 3)) == 16)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 2)
                        {
                            B_ENDIAN2L_ENDIAN16(pFileEvent->pu8FileBuff + i); //Name error, big endian, but in fact little endian
                        }
                    }
                    else if(*((U16*)(_gu8AudioPara + 3)) == 32)
                    {
                        for(i = 0; i < pstFileAck->u32ReadSize ; i += 4)
                        {
                            B_ENDIAN2L_ENDIAN32(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos + i);
                        }
                    }
                }

                if(_gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] + 44 >= _gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC])
                {
                    pstFileAck->bIsFileEnd = TRUE;
                }
            }

            else
            {
                if(_gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] >= _gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC])
                {
                    pstFileAck->bIsFileEnd = TRUE;
                }
            }
        }
        //MM_DBG("-------> E_MMSDK_CALLBACK_FILE_READ %d\n", (int)pstFileAck->u32ReadSize);
        bRet = TRUE;
        break;

        /// The notify event is file seek request.
    case E_MMSDK_CALLBACK_FILE_SEEK:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        if(_geAudioType == MPLAYER_SUBTYPE_PCM)  // just PCM audio format
        {
            _gu32SaveFilePos = (MS_U32)pFileEvent->u64FileSeekPos;
            if(pFileEvent->u64FileSeekPos > sizeof(_gstWaveHdr)) // both within WAV header and file
            {
                if(_Demo_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, pFileEvent->u64FileSeekPos - sizeof(_gstWaveHdr), SEEK_SET, u32ItemIdx) == 0)
                {
                    _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = pFileEvent->u64FileSeekPos - sizeof(_gstWaveHdr);
                    bRet = TRUE;
                }
                else
                {
                    MM_ERR("File seek fail.\n");
                }
            }
            else  //just within WAV header
            {
                if(_Demo_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, 0, SEEK_SET, u32ItemIdx)==0)
                {
                    _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = 0;
                    bRet = TRUE;
                }
            }
        }
        else if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC])
        {
            if(_Demo_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, pFileEvent->u64FileSeekPos, SEEK_SET, u32ItemIdx) == 0)
            {
                _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = pFileEvent->u64FileSeekPos;
                //MM_DBG("------->fseek :%lld successfully-----------\n",pFileEvent->u64FileSeekPos);
                bRet = TRUE;
            }
            else
            {
                MM_ERR("File seek fail.\n");
            }
        }

        //MM_DBG("-------seeking %d bytes  needs time is %d ms\n", (int)pFileEvent->u64FileSeekPos,(int)_gu32TotalTime);
        break;

        /// The notify event is file tell request.
    case E_MMSDK_CALLBACK_FILE_TELL:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        if(_geAudioType == MPLAYER_SUBTYPE_PCM)  // just PCM audio format
        {
            pstFileAck->u64FilePosition = (_gu32SaveFilePos > (MS_U32)_gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC])?(MS_U64)_gu32SaveFilePos : _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC];
        }
        else
        {
            pstFileAck->u64FilePosition = _gu64CurPostn[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC];
        }
        bRet = TRUE;
        break;

        /// The notify event is file init OK.
    case E_MMSDK_CALLBACK_INIT_OK:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_START_PLAY);
        MM_DBG("Send MPLAYER_START_PLAY to Music_EvtProcess.\n");
        break;

        /// The notify event is file init fail.
    case E_MMSDK_CALLBACK_INIT_FAIL:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is invalid file.
    case E_MMSDK_CALLBACK_INVALID_FILE:
        MM_DBG("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding error.
    case E_MMSDK_CALLBACK_DECODE_ERROR:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is audio unsupported.
    case E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED:
        MM_DBG("Get E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding done.
    case E_MMSDK_CALLBACK_DECODE_DONE:
        MM_DBG("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;

        /// The notify event is video first frame decoding done.
    case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
        MM_DBG("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
        break;

        /// The notify event is media backward to file start and re-start the media playback.
    case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
        if (_gbMusicBackwardRePlayMode)
            _geTrickMod[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = E_MMSDK_TRICK_NORMAL;
        else
            MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;
    case E_MMSDK_CALLBACK_PLAYING_OK:
        MM_DBG("Get E_MMSDK_CALLBACK_PLAYING_OK event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_PLAY_OK);
        break;

    default:
        break;
    }

    return bRet;
}

static MS_BOOL _Demo_MM_Callback_Video(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo, const MS_U32 u32ItemIdx)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;
    MS_BOOL bRet = FALSE;
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = E_MPLAYER_MEDIA_STREAM_INVALID;
    MS_U64* u64TempCurPostn = NULL;

    //MM_DBG("eEvent is  %d\n", (int)eEvent);
    switch(eEvent)
    {
        /// The notify event is file read request.
    case E_MMSDK_CALLBACK_FILE_READ:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        eStreamType = _Demo_MapFileID2MediaStreamType(pFileEvent->pFileHandle, u32ItemIdx);
        if(eStreamType == E_MPLAYER_MEDIA_STREAM_INVALID)
        {
            MM_ERR("u32ItemIdx(%"DTC_MS_U32_d") invalid handler\n", u32ItemIdx);
            break;
        }
        pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8), pFileEvent->u32FileRequestSize, !_gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE], eStreamType, u32ItemIdx);

        if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
            u64TempCurPostn = &_gu64SubCurPostn[u32ItemIdx];
        else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
            u64TempCurPostn = &_gu64AudioCurPostn[u32ItemIdx];
        else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
            u64TempCurPostn = &_gu64Audio2CurPostn[u32ItemIdx];
        else
            u64TempCurPostn = &_gu64CurPostn[u32ItemIdx][(EN_MMSDK_MEDIA_TYPE)eStreamType];

        *u64TempCurPostn += pstFileAck->u32ReadSize;
        if((*u64TempCurPostn >= _gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE]))
        {
            MM_DBG("-------> E_MMSDK_CALLBACK_FILE_READ fileEnd,needs %"DTC_MS_U32_x" \n", pFileEvent->u32FileRequestSize);
            pstFileAck->bIsFileEnd = TRUE;
        }

        //MM_DBG("-------> E_MMSDK_CALLBACK_FILE_READ %d\n", (int)pstFileAck->u32ReadSize);

        bRet = TRUE;
        break;

        /// The notify event is file seek request.
    case E_MMSDK_CALLBACK_FILE_SEEK:
        eStreamType = _Demo_MapFileID2MediaStreamType(pFileEvent->pFileHandle, u32ItemIdx);
        if(eStreamType == E_MPLAYER_MEDIA_STREAM_INVALID)
        {
            MM_ERR("u32ItemIdx(%"DTC_MS_U32_d") invalid handler\n", u32ItemIdx);
            break;
        }
        //MM_DBG("--- File size: 0x%llx, Seek position: 0x%llx.\n", _gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE], pFileEvent->u64FileSeekPos);
        if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE])
        {
            int SeekResult = 1;//be cautious , 0 indicates OK; 1 means failure!!!!!
            SeekResult = _Demo_Ringbuf_Seek(eStreamType, pFileEvent->u64FileSeekPos, SEEK_SET, u32ItemIdx);
            if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
                u64TempCurPostn = &_gu64SubCurPostn[u32ItemIdx];
            else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
                u64TempCurPostn = &_gu64AudioCurPostn[u32ItemIdx];
            else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
                u64TempCurPostn = &_gu64Audio2CurPostn[u32ItemIdx];
            else
                u64TempCurPostn = &_gu64CurPostn[u32ItemIdx][(EN_MMSDK_MEDIA_TYPE)eStreamType];

            if(SeekResult == 0)
            {
                *u64TempCurPostn = pFileEvent->u64FileSeekPos;
                bRet = TRUE;
            }
            else
            {
                MM_ERR("File seek fail.\n");
            }
        }
        break;

        /// The notify event is file tell request.
    case E_MMSDK_CALLBACK_FILE_TELL:
        //MM_DBG("Get E_MMSDK_CALLBACK_FILE_TELL event.\n");
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        eStreamType = _Demo_MapFileID2MediaStreamType(pFileEvent->pFileHandle, u32ItemIdx);
        if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
            pstFileAck->u64FilePosition = _gu64SubCurPostn[u32ItemIdx];
        else if(eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
            pstFileAck->u64FilePosition = _gu64AudioCurPostn[u32ItemIdx];
        else if(eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
            pstFileAck->u64FilePosition = _gu64Audio2CurPostn[u32ItemIdx];
        else
            pstFileAck->u64FilePosition = _gu64CurPostn[u32ItemIdx][(EN_MMSDK_MEDIA_TYPE)eStreamType];

        bRet = TRUE;
        break;

        /// The notify event is file init OK.
    case E_MMSDK_CALLBACK_INIT_OK:
        MM_DBG("Movie Init ok!\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_START_PLAY, u32ItemIdx);
        break;

        /// The notify event is file init fail.
    case E_MMSDK_CALLBACK_INIT_FAIL:
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE, u32ItemIdx);
        break;

        /// The notify event is invalid file.
    case E_MMSDK_CALLBACK_INVALID_FILE:
        MM_DBG("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE, u32ItemIdx);
        break;

        /// The notify event is thumbnail decoding done.
    case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
        MM_DBG("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
        break;

        /// The notify event is thumbnail decoding error.
    case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
        MM_DBG("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
        break;

        /// The notify event is decoding error.
    case E_MMSDK_CALLBACK_DECODE_ERROR:
        MM_DBG("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE, u32ItemIdx);
        break;

        /// The notify event is decoding done.
    case E_MMSDK_CALLBACK_DECODE_DONE:
        MM_DBG("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_OK, u32ItemIdx);
        break;

        /// The notify event is video first frame decoding done.
    case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
        MM_DBG("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_CMD_DONE, u32ItemIdx);
        break;

        /// The notify event is media backward to file start and re-start the media playback.
    case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
        MM_DBG("Get E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START event.\n");
        _geTrickMod[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] = E_MMSDK_TRICK_NORMAL;
        break;

        ///  The notify event is program been changed.
    case E_MMSDK_CALLBACK_PROGRAM_CHANGED:
        MM_DBG("Get E_MMSDK_CALLBACK_PROGRAM_CHANGED event.\n");
        break;

        ///  The notify event is audio track been changed.
    case E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED:
        MM_DBG("Get E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED event.\n");
        break;

    case E_MMSDK_CALLBACK_PLAYING_OK:
        MM_DBG("Get E_MMSDK_CALLBACK_PLAYING_OK event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_PLAY_OK, u32ItemIdx);
        break;
    case E_MMSDK_CALLBACK_SETPLAYPOSITION_ACK:
        MM_DBG("Get E_MMSDK_CALLBACK_SETPLAYPOSITION_ACK event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_CMD_DONE, u32ItemIdx);
        MsOS_DelayTask(5);
        break;
    case E_MMSDK_CALLBACK_SETTRICKMODE_ACK:
        MM_DBG("Get E_MMSDK_CALLBACK_SETTRICKMODE_ACK event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_CMD_DONE, u32ItemIdx);
        MsOS_DelayTask(5);
        break;
    default:
        break;
    }

    return bRet;
}

static MS_BOOL _Demo_MM_Callback_ES(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo, const MS_U32 u32ItemIdx)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    MS_BOOL bRet = FALSE;

    //MM_DBG("eEvent is  %d\n", (int)eEvent);

    switch(eEvent)
    {
        case E_MMSDK_CALLBACK_FILE_READ:
        case E_MMSDK_CALLBACK_FILE_SEEK:
        case E_MMSDK_CALLBACK_FILE_TELL:
            MM_DBG("No necessary (eEvent = %d).\n", eEvent);
            break;
        case E_MMSDK_CALLBACK_INIT_OK:
            MM_DBG("Get E_MMSDK_CALLBACK_INIT_OK event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_START_PLAY);
            break;
        case E_MMSDK_CALLBACK_INIT_FAIL:
            MM_DBG("Get E_MMSDK_CALLBACK_INIT_FAIL event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;
        case E_MMSDK_CALLBACK_INVALID_FILE:
            MM_DBG("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;
        case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
            break;
        case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
            MM_DBG("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
            break;
        case E_MMSDK_CALLBACK_DECODE_ERROR:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;
        case E_MMSDK_CALLBACK_DECODE_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_EXIT_OK);
            break;
        case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
            break;
        case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
            MM_DBG("Get E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START event.\n");
            break;
        case E_MMSDK_CALLBACK_DEMUX_HEADER_OK:
            MM_DBG("Get E_MMSDK_CALLBACK_DEMUX_HEADER_OK event.\n");
            break;
        case E_MMSDK_CALLBACK_DEMUX_HEADER_FAIL:
            MM_DBG("Get E_MMSDK_CALLBACK_DEMUX_HEADER_FAIL event.\n");
            break;
        case E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED:
            MM_DBG("Get E_MMSDK_CALLBACK_AUDIO_UNSUPPORTED event.\n");
            break;
        case E_MMSDK_CALLBACK_NOT_SUPPORT:
            MM_DBG("Get E_MMSDK_CALLBACK_NOT_SUPPORT event.\n");
            break;
        case E_MMSDK_CALLBACK_PLAYING_OK:
            MM_DBG("Get E_MMSDK_CALLBACK_PLAYING_OK event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_PLAY_OK);
            break;
        case E_MMSDK_CALLBACK_SETPLAYPOSITION_ACK:
            MM_DBG("Get E_MMSDK_CALLBACK_SETPLAYPOSITION_ACK event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_CMD_DONE);
            MsOS_DelayTask(5);
            break;
        case E_MMSDK_CALLBACK_SETTRICKMODE_ACK:
            MM_DBG("Get E_MMSDK_CALLBACK_SETTRICKMODE_ACK event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_CMD_DONE);
            MsOS_DelayTask(5);
            break;
        case E_MMSDK_CALLBACK_ES_FLUSH_DATA_RDY:
            MM_DBG("Get E_MMSDK_CALLBACK_ES_FLUSH_DATA_RDY event.\n");
            MM_SET_ES_STREAM_NOTIFY_TYPE(MPLAYER_ES_FLUSH_DATA_READY);
            break;
        case E_MMSDK_CALLBACK_DEMUXRESETADVANCE_ACK:
            MM_DBG("Get E_MMSDK_CALLBACK_DEMUXRESETADVANCE_ACK event.\n");
            break;
        default:
            break;
    }

    return bRet;
}



/// @param pClass          \b IN: Up-layer application class point.(for application to distinguish)
/// @param u32Msg         \b IN: Callback message type (EN_MMSDK_CALLBACK_MSG)
/// @param pMsgInfo       \b IN: Callback message info
/// @param pAckInfo       \b IN: Callback Ack info
MMSDK_BOOL _Demo_MM_Callback(void* pClass, const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    MS_BOOL bRet = FALSE;
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    MS_U32 i = 0, j = 0;

    if (pMsgInfo == NULL)
    {
        MM_ERR("No event attached data.\n");
        goto FUN_END;
    }

    for(i = MMSDK_MEDIA_ITEM_MIN; i < MMSDK_MEDIA_ITEM_MAX; i++)
    {
        for(j = MMSDK_MEDIA_TYPE_MIN; j < MMSDK_MEDIA_TYPE_NUM; j++)
        {
            if(pFileEvent->Item == _gItem[i][j])
            {
                if(_gu32FilePlayFlag[i][j] & MPLAYER_PLAY_FLAG_ES_STREAM)
                {
                    bRet = _Demo_MM_Callback_ES(u32Msg, pMsgInfo, pAckInfo, i);
                }
                else
                {
                    switch((EN_MMSDK_MEDIA_TYPE)j)
                    {
                        case E_MMSDK_MEDIA_TYPE_PHOTO:
                            bRet = _Demo_MM_Callback_Photo(u32Msg, pMsgInfo, pAckInfo, i);
                            break;
                        case E_MMSDK_MEDIA_TYPE_MUSIC:
                            bRet = _Demo_MM_Callback_Music(u32Msg, pMsgInfo, pAckInfo, i);
                            break;
                        case E_MMSDK_MEDIA_TYPE_MOVIE:
                            bRet = _Demo_MM_Callback_Video(u32Msg, pMsgInfo, pAckInfo, i);
                            break;
                    }
                }
            }
        }
    }

FUN_END:
    return bRet;
}

static void _Demo_Photo_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE ePhotoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
    MS_U32 u32ItemIdx = _gu32CurrentMediaItem;

    while (_gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] != TRUE)
    {
        while((_gePhotoNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        ePhotoNotifyTypeTmp = _gePhotoNotifyType;
        _gePhotoNotifyType = MPLAYER_NOTIFY_NULL;

        MM_INFO("in Photo_EvtProcess thread\n");
        if (ePhotoNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            MM_DBG("Receive signal=%d from MM_Callback_Photo .\n", (int)ePhotoNotifyTypeTmp);
            if (ePhotoNotifyTypeTmp ==  MPLAYER_START_PLAY)
            {
                _Demo_MPlayer_SetOption(u32ItemIdx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_STOP_PHOTO_DISPLAY, 0);

                _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
                _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = TRUE;
                if (_gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Height +
                     _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Width +
                     _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16X +
                     _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Y != 0) //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16X;
                    sDispWindow.s32Y = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Y;
                    sDispWindow.u16Width = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Width;
                    sDispWindow.u16Height = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    MM_DBG("--------DISP_WINDOW[%"DTC_MS_U32_d"][%"DTC_MS_U32_d"][%d][%d]\n", sDispWindow.s32X, sDispWindow.s32Y, sDispWindow.u16Width, sDispWindow.u16Height);

                    if (_Demo_MM_SetDisplayRectangle(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO], &sDispWindow, TRUE, _gbKeepRatio) != TRUE)
                    {
                        _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
                    }
                }
                if (MMSDK_Play(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO]) != TRUE)
                {
                    _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
                }
                _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_PHOTO_DECODE_DONE)
            {
                MM_MUST("---------------->Decode Successfully!\n");
                MM_DBG("---------------------after  process MPLAYER_PHOTO_DISPLAYED .\n");
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_OK, u32ItemIdx);
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_PHOTO_DECODE_FAILED)
            {
                MM_MUST("---------------->Decode Error! \n");
                MM_DBG("---------------------after  process MPLAYER_PHOTO_DECODE_FAILED .\n");
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_FAIL, u32ItemIdx);
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                MM_MUST("---------------->Unsupported File! \n");
                MM_DBG("---------------------after  process MPLAYER_PHOTO_ERROR_FILE .\n");
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_FAIL, u32ItemIdx);
            }

            ePhotoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
    }
    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_PROC_EXIT);
}

static void _Demo_Music_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE eMusicNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
    MS_U32 u32ItemIdx = _gu32CurrentMediaItem;

    while(_gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] != TRUE)
    {
        while(_geMusicNotifyType == MPLAYER_NOTIFY_NULL && _gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] != TRUE)
        {
            MS_U32  pu32RetrievedEventFlag;
            //MsOS_WaitEvent(condMusicNotifyTypeValid, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eMusicNotifyTypeTmp = _geMusicNotifyType;
        _geMusicNotifyType = MPLAYER_NOTIFY_NULL;

        _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
        MM_INFO("----- in Music_EvtProcess thread\n");

        if((eMusicNotifyTypeTmp != MPLAYER_NOTIFY_NULL) && (_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] != 0))
        {
            MM_DBG("Receive signal=%d from MM_Callback_Music .\n", (int)eMusicNotifyTypeTmp);
            if(eMusicNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
                MM_MUST("Music exit ok!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MUSIC, u32ItemIdx);
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                MM_MUST("Music unsupport file!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MUSIC, u32ItemIdx);
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_FAIL, u32ItemIdx);
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_INSUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC],  E_MMSDK_TRICK_PAUSE) == FALSE)
                {
                    MM_MUST("-----> MM_SetTrickMode fail 1\n");
                }
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_SUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC],  E_MMSDK_TRICK_NORMAL) == FALSE)
                {
                    MM_MUST("-----> MM_SetTrickMode fail 2\n");
                }
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC] = MMSDK_Play(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MUSIC]);
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_PLAY_OK)
            {
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_OK, u32ItemIdx);
            }
            eMusicNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    }
    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_PROC_EXIT);
}

static void _Demo_Video_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE eVideoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
    MS_U32 u32ItemIdx = _gu32CurrentMediaItem;
    MS_U32 u32EventItemIdx = _Demo_MM_GetEventIndex(u32ItemIdx);

    while(_gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] != TRUE)
    {
        while((_geVideoNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            //MsOS_WaitEvent(condVideoNotifyTypeValid, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_NOTIFY ^ u32EventItemIdx, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eVideoNotifyTypeTmp = _geVideoNotifyType;
        _geVideoNotifyType = MPLAYER_NOTIFY_NULL;

        _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
        MM_INFO("----- in Video_EvtProcess thread\n");
        if(eVideoNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            if(eVideoNotifyTypeTmp == MPLAYER_FRAME_READY)
            {
                MM_DBG("==================Movie frame ready!\n");
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
                MM_MUST("Movie exit ok!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MOVIE, u32ItemIdx);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                MM_MUST("Movie unsupport file!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MOVIE, u32ItemIdx);
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_FAIL, u32ItemIdx);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_BLACKSCREEN_OFF)
            {
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_INSUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE], E_MMSDK_TRICK_PAUSE) == FALSE)
                {
                    MM_MUST("-----> MM_SetTrickMode fail 2\n");
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_SUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE], E_MMSDK_TRICK_NORMAL) == FALSE)
                {
                    MM_MUST("-----> MM_SetTrickMode fail 2\n");
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                if(_gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16Height +
                    _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16Width +
                    _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16X +
                    _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16Y != 0)  //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16X;
                    sDispWindow.s32Y = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16Y;
                    sDispWindow.u16Width = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16Width;
                    sDispWindow.u16Height = _gstRect[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE].u16Height;

                    if(_Demo_MM_SetDisplayRectangle(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE], &sDispWindow, TRUE,
                        (_gu32FilePlayFlag[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] & MPLAYER_PLAY_FLAG_MULTI_DEC) ? FALSE : _gbKeepRatio) )
                    {
                        _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
                    }
                }

                if(MMSDK_Play(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE]) != TRUE)
                {
                    MM_ERR("u32ItemIdx(%"DTC_MS_U32_d") Play FAIL\n", u32ItemIdx);
                    _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
                }
                else
                {
                    MM_DBG("u32ItemIdx(%"DTC_MS_U32_d") Play OK\n", u32ItemIdx);
                    _gbPlay[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE] = TRUE;
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_PLAY_OK)
            {
                if(MMSDK_SetSubtitleOnOff(_gItem[u32ItemIdx][E_MMSDK_MEDIA_TYPE_MOVIE], TRUE) == FALSE)
                {
                    MM_ERR(" fail to set subtitle on.\n");
                }
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_OK, u32ItemIdx);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_CMD_DONE)
            {
                MM_DBG("Get CMD DONE ACK\n");
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_CMD_DONE, u32ItemIdx);
            }

            eVideoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    }

    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_PROC_EXIT);
}

static void _Demo_ESplayer_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE eEsStreamNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
    MS_U32 u32ItemIdx = _gu32CurrentMediaItem;
    EN_MMSDK_MEDIA_TYPE eMediaType = _geMediaType;

    while(_gbExit[u32ItemIdx][eMediaType] != TRUE)
    {
        while((_geEsStreamNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[u32ItemIdx][eMediaType] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_ES_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eEsStreamNotifyTypeTmp = _geEsStreamNotifyType;
        _geEsStreamNotifyType = MPLAYER_NOTIFY_NULL;

        _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
        MM_INFO("----- in ES_Stream_EvtProcess thread\n");
        if(eEsStreamNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            if(eEsStreamNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
                MM_MUST("Movie exit ok!\n");
                _Demo_MPlayer_Stop(eMediaType, u32ItemIdx);
            }
            else if(eEsStreamNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                MM_MUST("Movie unsupport file!\n");
                _Demo_MM_DeleteParseEsTask();
                _Demo_MPlayer_Stop(eMediaType, u32ItemIdx);
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_FAIL, u32ItemIdx);
            }
            else if(eEsStreamNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                if(_gstRect[u32ItemIdx][eMediaType].u16Height +
                    _gstRect[u32ItemIdx][eMediaType].u16Width +
                    _gstRect[u32ItemIdx][eMediaType].u16X +
                    _gstRect[u32ItemIdx][eMediaType].u16Y != 0)  //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[u32ItemIdx][eMediaType].u16X;
                    sDispWindow.s32Y = _gstRect[u32ItemIdx][eMediaType].u16Y;
                    sDispWindow.u16Width = _gstRect[u32ItemIdx][eMediaType].u16Width;
                    sDispWindow.u16Height = _gstRect[u32ItemIdx][eMediaType].u16Height;

                    if(_Demo_MM_SetDisplayRectangle(_gItem[u32ItemIdx][eMediaType], &sDispWindow, TRUE, _gbKeepRatio))
                    {
                        _gbPlay[u32ItemIdx][eMediaType] = FALSE;
                    }
                }

                if(MMSDK_Play(_gItem[u32ItemIdx][eMediaType]) != TRUE)
                {
                    MM_ERR("u32ItemIdx(%"DTC_MS_U32_d") Play FAIL\n", u32ItemIdx);
                    _gbPlay[u32ItemIdx][eMediaType] = FALSE;
                }
                else
                {
                    MM_DBG("u32ItemIdx(%"DTC_MS_U32_d") Play OK\n", u32ItemIdx);
                    _gbPlay[u32ItemIdx][eMediaType] = TRUE;
                }
            }
            else if(eEsStreamNotifyTypeTmp == MPLAYER_PLAY_OK)
            {
                if(MMSDK_SetSubtitleOnOff(_gItem[u32ItemIdx][eMediaType], TRUE) == FALSE)
                {
                    MM_ERR(" fail to set subtitle on.\n");
                }
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_OK, u32ItemIdx);
            }
            else if(eEsStreamNotifyTypeTmp == MPLAYER_ES_FLUSH_DATA_READY)
            {
                MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_ES_DATA_FLUSH_READY);
            }
            else if(eEsStreamNotifyTypeTmp == MPLAYER_CMD_DONE)
            {
                MM_SET_MM_PLAY_STATUS(E_MM_PLAY_STATUS_CMD_DONE, u32ItemIdx);
            }
            eEsStreamNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    }

    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_ES_PROC_EXIT);
}

static MS_BOOL _Demo_MM_ExtractESParam(EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType, MS_U8 *pu8Memory, MS_U8 u8Memorysize)
{
    MS_U32 u32TotalSize = 0;
    MS_U8* pReadBuf = NULL;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
    {
        _Demo_FileSeek(_gpEsFileHandle, (ES_AUDIO_PARAM_SIZE * 4) , SEEK_SET);
        u32TotalSize = ES_AUDIO_PARAM_SIZE * 4;
    }
    else if (eStreamType == E_MPLAYER_MEDIA_STREAM_MOVIE)
    {
        _Demo_FileSeek(_gpEsFileHandle, 0, SEEK_SET);
        u32TotalSize = ES_VIDEO_PARAM_SIZE * 4;
    }
    else
    {
        return FALSE;
    }

    pReadBuf = (MS_U8*) MsOS_AllocateMemory(sizeof(MS_U8)*u32TotalSize, s32MstarNonCachedPoolID);
    if (pReadBuf == NULL)
    {
        MM_ERR("Allocate FeedBuf[size %"DTC_MS_U32_d"] fail\n", u32TotalSize);
        return FALSE;
    }

    _Demo_FileRead(pReadBuf, sizeof(MS_U8), u32TotalSize, _gpEsFileHandle);
    memcpy((void*)pu8Memory, (void*)pReadBuf, u8Memorysize);

    if (pReadBuf != NULL)
    {
        MsOS_FreeMemory(pReadBuf, s32MstarNonCachedPoolID);
        pReadBuf = NULL;
    }
    return TRUE;
}

static EN_MMSDK_VIDEO_CODEC _Demo_MM_VideoCodecPlayer2Mmsdk(EN_MM_VIDEO_CODEC eVideoCodec)
{
    EN_MMSDK_VIDEO_CODEC eCodecType = E_MMSDK_VIDEO_CODEC_UNKNOWN;
    switch(eVideoCodec)
    {
        case E_MM_VIDEO_CODEC_UNKNOW:
            eCodecType = E_MMSDK_VIDEO_CODEC_UNKNOWN;
            break;
        case E_MM_VIDEO_CODEC_MPEG1VIDEO:
            eCodecType = E_MMSDK_VIDEO_CODEC_MPG;
            break;
        case E_MM_VIDEO_CODEC_MPEG2VIDEO:
            eCodecType = E_MMSDK_VIDEO_CODEC_MPG;
            break;
        case E_MM_VIDEO_CODEC_MPEG4:
            eCodecType = E_MMSDK_VIDEO_CODEC_MPEG4;
            break;
        case E_MM_VIDEO_CODEC_MJPEG:
            eCodecType = E_MMSDK_VIDEO_CODEC_MJPEG;
            break;
        case E_MM_VIDEO_CODEC_H264:
            eCodecType = E_MMSDK_VIDEO_CODEC_H264;
            break;
        case E_MM_VIDEO_CODEC_TS:
            eCodecType = E_MMSDK_VIDEO_CODEC_TS;
            break;
        case E_MM_VIDEO_CODEC_VC1:
            eCodecType = E_MMSDK_VIDEO_CODEC_VC1;
            break;
        case E_MM_VIDEO_CODEC_AVS:
            eCodecType = E_MMSDK_VIDEO_CODEC_AVS;
            break;
        case E_MM_VIDEO_CODEC_FLV:
            eCodecType = E_MMSDK_VIDEO_CODEC_FLV;
            break;
        case E_MM_VIDEO_CODEC_MVC:
            eCodecType = E_MMSDK_VIDEO_CODEC_MVC;
            break;
        case E_MM_VIDEO_CODEC_VP6:
            eCodecType = E_MMSDK_VIDEO_CODEC_VP6;
            break;
        case E_MM_VIDEO_CODEC_VP8:
            eCodecType = E_MMSDK_VIDEO_CODEC_VP8;
            break;
        case E_MM_VIDEO_CODEC_HEVC:
            eCodecType = E_MMSDK_VIDEO_CODEC_HEVC;
            break;
        case E_MM_VIDEO_CODEC_VP9:
            eCodecType = E_MMSDK_VIDEO_CODEC_VP9;
            break;
        default:
            break;
    }

    return eCodecType;
}

static EN_MMSDK_AUDIO_CODEC _Demo_MM_AudioCodecPlayer2Mmsdk(EN_MM_AUDIO_CODEC eAudioCodec)
{
    EN_MMSDK_AUDIO_CODEC eCodecType = E_MMSDK_AUDIO_CODEC_UNKNOWN;
    switch(eAudioCodec)
    {
        case E_MM_AUDIO_CODEC_UNKNOW:
            eCodecType = E_MMSDK_AUDIO_CODEC_UNKNOWN;
            break;
        case E_MM_AUDIO_CODEC_MPEG:
            eCodecType = E_MMSDK_AUDIO_CODEC_MPEG;
            break;
        case E_MM_AUDIO_CODEC_MP3:
            eCodecType = E_MMSDK_AUDIO_CODEC_MP3;
            break;
        case E_MM_AUDIO_CODEC_AAC:
        case E_MM_AUDIO_CODEC_RAAC:
        case E_MM_AUDIO_CODEC_HEAAC:
        case E_MM_AUDIO_CODEC_MPEG4AAC:
            eCodecType = E_MMSDK_AUDIO_CODEC_AAC;
            break;
        case E_MM_AUDIO_CODEC_WMA:
            eCodecType = E_MMSDK_AUDIO_CODEC_WMA;
            break;
        case E_MM_AUDIO_CODEC_VORBIS:
            eCodecType = E_MMSDK_AUDIO_CODEC_VORBIS;
            break;
        case E_MM_AUDIO_CODEC_DTS:
            eCodecType = E_MMSDK_AUDIO_CODEC_DTS;
            break;
        case E_MM_AUDIO_CODEC_FLAC:
            eCodecType = E_MMSDK_AUDIO_CODEC_FLAC;
            break;
        case E_MM_AUDIO_CODEC_AC3:
            eCodecType = E_MMSDK_AUDIO_CODEC_AC3;
            break;
        case E_MM_AUDIO_CODEC_AC3_PLUS:
            eCodecType = E_MMSDK_AUDIO_CODEC_AC3P;
            break;
        case E_MM_AUDIO_CODEC_COOK:
            eCodecType = E_MMSDK_AUDIO_CODEC_RA8LBR;
            break;
        case E_MM_AUDIO_CODEC_AMR_NB:
            eCodecType = E_MMSDK_AUDIO_CODEC_AMR_NB;
            break;
        case E_MM_AUDIO_CODEC_AMR_WB:
            eCodecType = E_MMSDK_AUDIO_CODEC_AMR_WB;
            break;
        case E_MM_AUDIO_CODEC_PCM:
            eCodecType = E_MMSDK_AUDIO_CODEC_PCM;
            break;
        case E_MM_AUDIO_CODEC_ADPCM:
            eCodecType = E_MMSDK_AUDIO_CODEC_ADPCM;
            break;
        case E_MM_AUDIO_CODEC_PCM_MULAW:
        case E_MM_AUDIO_CODEC_PCM_ALAW:
            eCodecType = E_MMSDK_AUDIO_CODEC_XPCM;
            break;
        case E_MM_AUDIO_CODEC_WMA3:
            eCodecType = E_MMSDK_AUDIO_CODEC_WMA_PRO;
            break;
        case E_MM_AUDIO_CODEC_WMA2:
            eCodecType = E_MMSDK_AUDIO_CODEC_WMA2;
            break;
        case E_MM_AUDIO_CODEC_DRA:
            eCodecType = E_MMSDK_AUDIO_CODEC_DRA;
            break;
        default:
            break;
    }

    return eCodecType;
}

static void _Demo_MM_MediaInfo2Mmsdk(EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType, void* pMmsdkAVInfo, void* pAVInfo)
{
    if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
    {
        ST_MM_AUDIO_PARAM *stAudioParam = pAVInfo;
        ST_MMSDK_ES_AUDIO_INFO *stEsAudioInfo = pMmsdkAVInfo;

        stEsAudioInfo->enCodecID = _Demo_MM_AudioCodecPlayer2Mmsdk(stAudioParam->eCodecID);
        stEsAudioInfo->stParam.u32Scale = stAudioParam->stParam.u32Scale;
        stEsAudioInfo->stParam.u32Rate = stAudioParam->stParam.u32Rate;
        stEsAudioInfo->stParam.u32Length = stAudioParam->stParam.u32Length;
        stEsAudioInfo->stParam.u32SampleSize = stAudioParam->stParam.u32SampleSize;
        stEsAudioInfo->stParam.u32Channel = stAudioParam->stParam.u32Channel;
        stEsAudioInfo->stParam.u32SampleRate = stAudioParam->stParam.u32SampleRate;
        stEsAudioInfo->stParam.u32BitRate = stAudioParam->stParam.u32BitRate;
        stEsAudioInfo->stParam.u32BlockAlign = stAudioParam->stParam.u32BlockAlign;
        stEsAudioInfo->stParam.u32BitsPerSample = stAudioParam->stParam.u32BitsPerSample;
        stEsAudioInfo->stParam.u32SamplesPerBlock= stAudioParam->stParam.u32SamplesPerBlock;
        stEsAudioInfo->stParam.u32ObjectTypeID = stAudioParam->stParam.u32ObjectTypeID;
        stEsAudioInfo->stParam.u32ChannelMask = stAudioParam->stParam.u32ChannelMask;
        stEsAudioInfo->stParam.u16EncodeOpt = stAudioParam->stParam.u16EncodeOpt;
        stEsAudioInfo->stParam.u16FormatTag = stAudioParam->stParam.u16FormatTag;
        stEsAudioInfo->stParam.u8Version = stAudioParam->stParam.u8Version;
        stEsAudioInfo->stParam.u16AdvanceEncodeOpt = stAudioParam->stParam.u16AdvanceEncodeOpt;
    }
    else
    {
        ST_MM_VIDEO_INFO *stVideoInfo = pAVInfo;
        ST_MMSDK_ES_VIDEO_INFO *stEsVideoInfo = pMmsdkAVInfo;
        stEsVideoInfo->enVideoType = _Demo_MM_VideoCodecPlayer2Mmsdk(stVideoInfo->eVideoType);
        stEsVideoInfo->u32VideoWidth = stVideoInfo->u32VideoWidth;
        stEsVideoInfo->u32VideoHeight = stVideoInfo->u32VideoHeight;
        stEsVideoInfo->u32VideoRate = stVideoInfo->u32FrameRate;
        stEsVideoInfo->bAvSyncDisable = stVideoInfo->bAvsyncDisable;
        stEsVideoInfo->bDropErrorFrame = stVideoInfo->bDropErrorFrame;
        stEsVideoInfo->bPtsMode = stVideoInfo->bPTS_Mode;
        stEsVideoInfo->bLivePlayback = stVideoInfo->bLivePlayback;
        stEsVideoInfo->u8NalLenSizeUnit = stVideoInfo->u8NalLenSizeUnit;
        stEsVideoInfo->bEnableSeamless = stVideoInfo->bEnableSeamless;
    }
}


MS_U32 _Demo_MM_FileMode2PlayFlag(MS_U32 u32FileOption)
{
    MS_U32 u32FileMode = u32FileOption & MASK_FILE_MODE;
    MS_U32 u32PlayFlag = u32FileOption;

    u32PlayFlag &= (~MASK_FILE_MODE);

    switch(u32FileMode)
    {
        case MPLAYER_FILE_MODE:
            u32PlayFlag |=  MPLAYER_PLAY_FLAG_FILEIN;
            break;
        case MPLAYER_SEEKABLE_STREAM_MODE:
            u32PlayFlag |=  MPLAYER_PLAY_FLAG_SEEKABLE_STREAM;
            break;
        case MPLAYER_UNSEEKABLE_STREAM_MODE:
            u32PlayFlag |=  MPLAYER_PLAY_FLAG_UNSEEKABLE_STREAM;
            break;
        case MPLAYER_ES_MODE:
            u32PlayFlag |=  MPLAYER_PLAY_FLAG_ES_STREAM;
            break;
        default:
            break;
    }

#if (DEMO_DMS_TEST == 1 && DEMO_MM_AVP_TEST == 1)
    if(u32PlayFlag & MPLAYER_PLAY_FLAG_AVP)
    {
        if( (u32PlayFlag & MPLAYER_PLAY_FLAG_DIP) && !(u32PlayFlag & MPLAYER_PLAY_FLAG_MULTI_DEC) )
        {
            MM_MUST("DMS structure \"DO NOT\" support DIP + single flow, Convert MVOP + single automatically\n");
        }
        u32PlayFlag |= MPLAYER_PLAY_FLAG_DIP; // in DMS structure, both MVOP & DIP flow must raise DIP flag
    }
#endif
    MM_MUST("u32PlayFlag = 0x%"DTC_MS_U32_x"\n", u32PlayFlag);

    return u32PlayFlag;
}

void _Demo_MM_SelectPlayAudioItem(const MS_U32 u32Idx)
{
#if (DEMO_MM_AVP_TEST == 1)
    switch(_geAudioSwitchMode)
    {
        case E_MM_AUDIO_SWITCH_FIRST_PLAY:
            if( _Demo_MM_GetAliveMediaItem() != MMSDK_MEDIA_ITEM_INVALID )    // Another streams are playing
            {
                _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_VIDEO_ONLY;
            }
            else
            {
                _gu32CurrentPlayAudioItem = u32Idx;
            }
            break;
        case E_MM_AUDIO_SWITCH_LAST_PLAY:
            if( _Demo_MM_GetMediaType(_gu32CurrentPlayAudioItem) != MMSDK_MEDIA_TYPE_INVALID )  // Another streams are playing
            {
                _Demo_MPlayer_SetOption(_gu32CurrentPlayAudioItem, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_AUDIO, -1);
            }
            _gu32CurrentPlayAudioItem = u32Idx;
            break;
        case E_MM_AUDIO_SWITCH_VIDEO_ONLY:
            _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_VIDEO_ONLY;
            break;
        default:
            break;
    }
    MM_DBG("_gu32CurrentPlayAudioItem = %"DTC_MS_U32_d"\n", _gu32CurrentPlayAudioItem);
#endif
}

MS_BOOL _Demo_MPlayer_PlayFile(const EN_MMSDK_MEDIA_TYPE eMediaType, MPLAYER_MEDIA* const pMplayerMedia)
{
    MM_TRACE("------> Enter func MApi_MPlayer_PlayFile \n");
    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    BOOLEAN bRet = FALSE;
    MS_U32 u32Idx = 0;
    ST_MMSDK_PARAM astExtenInfoParam[EXT_INFO_PARAM_NUM];
    MS_U32 u32ExtParamIndex = 0;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MMSDK_U8 au8DisplayOutputPath[MMSDK_DISP_OUTPUT_PATH_MAX_LEN] = "dip";
    MMSDK_U8 au8AppTypeName[MMSDK_AP_TYPE_MAX_LEN];
    ST_MM_VIDEO_INFO stVideoInfo;
    ST_MM_AUDIO_PARAM stAudioParam;
    ST_MMSDK_ES_VIDEO_INFO stEsVideoInfo;
    ST_MMSDK_ES_AUDIO_INFO stEsAudioInfo;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    memset(&astExtenInfoParam, 0, sizeof(ST_MMSDK_PARAM) * EXT_INFO_PARAM_NUM);
    memset(&stVideoInfo, 0, sizeof(ST_MM_VIDEO_INFO));
    memset(&stAudioParam, 0, sizeof(ST_MM_AUDIO_PARAM));
    memset(&stEsVideoInfo, 0, sizeof(ST_MMSDK_ES_VIDEO_INFO));
    memset(&stEsAudioInfo, 0, sizeof(ST_MMSDK_ES_AUDIO_INFO));
    memset(&_gstMedia, 0, sizeof(ST_MMSDK_CREATE_MEDIA_INFO));

    _gstMedia.u32Version = MMSDK_VERSION;

    u32Idx = _Demo_MM_GetEmptyMediaItem(eMediaType);
    if (u32Idx == MMSDK_MEDIA_ITEM_INVALID)
    {
        MM_ERR("The item has already exist, return fail");
        goto FUNC_END;
    }

    _gu32CurrentMediaItem = u32Idx;
    MM_DBG("Current Media Item Index = %"DTC_MS_U32_d"\n", u32Idx);

    _gbExit[u32Idx][eMediaType] = FALSE;
    _gbESPlay[u32Idx][eMediaType] = FALSE;
    _gu32FilePlayFlag[u32Idx][eMediaType] = pMplayerMedia->u32PlayFlag;

    if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_MULTI_DEC)
    {
        _gstRect[u32Idx][eMediaType].u16Height = _gstDipRect[u32Idx].u16Height;
        _gstRect[u32Idx][eMediaType].u16Width = _gstDipRect[u32Idx].u16Width;
        _gstRect[u32Idx][eMediaType].u16X = _gstDipRect[u32Idx].u16X;
        _gstRect[u32Idx][eMediaType].u16Y = _gstDipRect[u32Idx].u16Y;
    }
    else
    {
        _gstRect[u32Idx][eMediaType].u16Height = pMplayerMedia->u32DispH;
        _gstRect[u32Idx][eMediaType].u16Width = pMplayerMedia->u32DispW;
        _gstRect[u32Idx][eMediaType].u16X = pMplayerMedia->u32DispX;
        _gstRect[u32Idx][eMediaType].u16Y = pMplayerMedia->u32DispY;
    }

    MM_DBG("Disp x=%d, y=%d, W=%d, H=%d\n", _gstRect[u32Idx][eMediaType].u16X,
        _gstRect[u32Idx][eMediaType].u16Y, _gstRect[u32Idx][eMediaType].u16Width, _gstRect[u32Idx][eMediaType].u16Height);

    if(pMplayerMedia->u32PlayFlag & (MPLAYER_PLAY_FLAG_FILEIN | MPLAYER_PLAY_FLAG_ES_STREAM))
    {
        if(pMplayerMedia->filename != NULL)
        {
            _gpStream[u32Idx][eMediaType] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
            if(_gpStream[u32Idx][eMediaType] != NULL)
                _gu64Size[u32Idx][eMediaType] = _Demo_FileLength(_gpStream[u32Idx][eMediaType]);
            else
            {
                MM_ERR("open stream failed!\n");
                goto FUNC_END;
            }
        }
        else
        {
            MM_ERR("MApi_MPlayer_PlayFile(): Filename is NULL ......\n");
            goto FUNC_END;
        }
    }

    /// Up-layer application class point of call back function.(for application to distinguish)
    _gstMedia.pClass = &_gstMedia;
    //Regiter callback.
    _gstMedia.pCallback = _Demo_MM_Callback;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if((eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE || eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC) &&
            !(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_AVP))
        {
            if (Demo_Audio_Dynamic_Allocate_Resource() == TRUE)
            {
                EN_AUDIO_APP_TYPE _geAppType = AUDIO_APP_ES_PLAY;
                AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType = E_CONNECT_NULL;
                if (_gu32AudioDecoderId == 0)
                {
                    eOutSrcType = E_CONNECT_MAIN;
                }
                else
                {
                    eOutSrcType = E_CONNECT_SUB;
                }
                Demo_Audio_Open(&_gu32AudioDecoderId, &eOutSrcType, &_geAppType);
            }
            if(Demo_MM_AllocAudDec() == FALSE)
            {
                goto FUNC_END;
            }
        }
    }

    if(_geAudioType == MPLAYER_SUBTYPE_PCM && eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC) // PCM audio format
    {
        _gstWaveHdr.fileID[0] = 'R';
        _gstWaveHdr.fileID[1] = 'I';
        _gstWaveHdr.fileID[2] = 'F';
        _gstWaveHdr.fileID[3] = 'F';
        _gstWaveHdr.fileleth = 0;
        _gstWaveHdr.wavTag[0] = 'W';
        _gstWaveHdr.wavTag[1] = 'A';
        _gstWaveHdr.wavTag[2] = 'V';
        _gstWaveHdr.wavTag[3] = 'E';

        _gstWaveHdr.FmtHdrID[0] = 'f';
        _gstWaveHdr.FmtHdrID[1] = 'm';
        _gstWaveHdr.FmtHdrID[2] = 't';
        _gstWaveHdr.FmtHdrID[3] = ' ';
        _gstWaveHdr.FmtHdrLeth = 0;

        _gstWaveHdr.DataHdrID[0] = 'd';
        _gstWaveHdr.DataHdrID[1] = 'a';
        _gstWaveHdr.DataHdrID[2] = 't';
        _gstWaveHdr.DataHdrID[3] = 'a';
        _gstWaveHdr.DataHdrLeth = 0;

        _gstWaveHdr.fileleth = (MS_U32)_gu64Size[u32Idx][eMediaType] + 36;
        _gstWaveHdr.FmtHdrLeth = 16;
        _gstWaveHdr.BitsPerSample = *(MS_U16*)&_gu8AudioPara[3];
        _gstWaveHdr.Channels = *(MS_U16*)&_gu8AudioPara[1];
        _gstWaveHdr.FormatTag = 0x0001;
        _gstWaveHdr.SamplesPerSec = *(MS_U16*)&_gu8AudioPara[5];
        _gstWaveHdr.BlockAlign = _gstWaveHdr.Channels * _gstWaveHdr.BitsPerSample / 8;
        _gstWaveHdr.AvgBytesPerSec = _gstWaveHdr.BlockAlign * _gstWaveHdr.SamplesPerSec;
        _gstWaveHdr.DataHdrLeth = (MS_U32)_gu64Size[u32Idx][eMediaType];
        _gu64Size[u32Idx][eMediaType] = (MS_S64)_gu64Size[u32Idx][eMediaType] + 44;

        MM_INFO("--------> PCM music format----------\n");
        MM_INFO("---BitsPerSample is %d----------\n", _gstWaveHdr.BitsPerSample);
        MM_INFO("---Channels is %d----------\n", _gstWaveHdr.Channels);
        MM_INFO("---SamplesPerSec is %d----------\n", (int)_gstWaveHdr.SamplesPerSec);
        MM_INFO("---Endian is %d----------\n", _gu8AudioPara[9]);
    }

    MM_DBG("------> _gu64Size[eMediaType] : %"DTC_MS_U64_d" \n", _gu64Size[u32Idx][eMediaType]);

    _gstMedia.u32CtlFlag = 0;
    if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && (pMplayerMedia->u32PlayFlag & (MPLAYER_PLAY_FLAG_FILEIN | MPLAYER_PLAY_FLAG_ES_STREAM)))
    {
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_ENABLE_AUDIO_HANDLER;
        if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_FILEIN)
        {
            _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_SUBTITLE] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
            _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
            _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO2] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        }
    }
    else
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_SUBTITLE_DISABLE;

    if (_gbUseNewAudioMode)
    {
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_ENABLE_NEW_AUDIO_MODE;
    }

    if(_gbKTVMode)
    {
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_VIDEO_ONLY;
    }

    if( pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_DISABLE_DYNSCALING)
    {
         _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_DISABLE_DYNAMIC_SCALING;
    }

    _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_TRICKMODE_ON;
    _gstMedia.u64FileLength = (MS_U64)_gu64Size[u32Idx][eMediaType];
    _gstMedia.pFileHandle1 = _gpEsFileHandle = _gpStream[u32Idx][eMediaType];
    if(_gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_SUBTITLE])//subtile
        _gstMedia.pFileHandle2 = _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_SUBTITLE];
    else
        _gstMedia.pFileHandle2 = _gpStream[u32Idx][eMediaType];
    _gstMedia.pFileHandle3 = _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO];
    _gstMedia.pFileHandle4 = _gpStream[u32Idx][E_MPLAYER_MEDIA_STREAM_AUDIO2];

    _gstMedia.enMediaType = eMediaType;

    if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_FILEIN)
    {
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_MASS_STORAGE;    //For USB content
    }
    else if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_ES_STREAM)
    {
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_ES;
    }

    if(_gu64Size[u32Idx][eMediaType] == (MS_U64) - 1)
    {
        MM_ERR("----> stream without seek\n");
        _gu32FilePlayFlag[u32Idx][eMediaType] |= MPLAYER_PLAY_FLAG_UNSEEKABLE_STREAM;
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_NETWORK_STREAM_WITHOUT_SEEK;
    }

    // Do not demo motion GIF, so setting E_MMSDK_CREATE_MODE_PREVIEW to decode only first frame
    if(strcmp("GIF", (char *)_gu8FileExtName) == 0 && _gbEnableMotionGif == FALSE)
    {
        _gstMedia.enMediaCreateMode = E_MMSDK_CREATE_MODE_PREVIEW;
    }
    else
    {
        _gstMedia.enMediaCreateMode = E_MMSDK_CREATE_MODE_NORMAL_PLAY;
    }

    ST_MMSDK_TIMEOUT_VALUE stTimeOutInfo;
    stTimeOutInfo.u32FileIOTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32FileReadTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32FileSeekTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32DecodeCtrlTimeOut = 0;//0xFFFFFFFF;
    stTimeOutInfo.u32ESBuffCtrlTimeOut = 0;//0xFFFFFFFF;
    MMSDK_SetPlayerTimeOutValue(&stTimeOutInfo);
    MMSDK_SetBackwardRePlayMode(_gbBackwardRePlayMode);
    if(_gu32TotalTime != 0)
    {
        MMSDK_SetStreamTotalTime(_gu32TotalTime);
    }
    if(_gu32StartTime != 0)
    {
        MMSDK_SetStreamStartTime(_gu32StartTime);
        _gu32StartTime = 0;
    }

    _Demo_MM_SelectPlayAudioItem(u32Idx);
    if( pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_DIP)
    {
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_DISPLAY_OUTPUT_PATH;
        astExtenInfoParam[u32ExtParamIndex].u32Paramsize = sizeof(au8DisplayOutputPath);
        astExtenInfoParam[u32ExtParamIndex].pParam = &au8DisplayOutputPath;
        u32ExtParamIndex++;
    }
    if( pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_MULTI_DEC)
    {
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_MULTI_DECODE_CAPABILITY;
        u32ExtParamIndex++;
    }
    if ( pMplayerMedia->u32PlayFlag & ( MPLAYER_PLAY_FLAG_SEEKABLE_STREAM | MPLAYER_PLAY_FLAG_UNSEEKABLE_STREAM ))
    {
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_URL_STRING;
        astExtenInfoParam[u32ExtParamIndex].u32Paramsize = strlen(pMplayerMedia->filename);
        astExtenInfoParam[u32ExtParamIndex].pParam = pMplayerMedia->filename;
        u32ExtParamIndex++;
    }
    else if( pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_ES_STREAM )
    {
        _Demo_MM_ExtractESParam(E_MPLAYER_MEDIA_STREAM_MOVIE, (MS_U8*)(&stVideoInfo), sizeof(ST_MM_VIDEO_INFO));
        _Demo_MM_MediaInfo2Mmsdk(E_MPLAYER_MEDIA_STREAM_MOVIE, (void*)&stEsVideoInfo, (void*)&stVideoInfo);
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_MOVIE_ESPLAYER_VIDEOINFO;
        astExtenInfoParam[u32ExtParamIndex].u32Paramsize = sizeof(ST_MMSDK_ES_VIDEO_INFO);
        astExtenInfoParam[u32ExtParamIndex].pParam = &stEsVideoInfo;
        u32ExtParamIndex++;

        _Demo_MM_ExtractESParam(E_MPLAYER_MEDIA_STREAM_AUDIO, (MS_U8*)(&stAudioParam), sizeof(ST_MM_AUDIO_PARAM));
        _Demo_MM_MediaInfo2Mmsdk(E_MPLAYER_MEDIA_STREAM_AUDIO, (void*)&stEsAudioInfo, (void*)&stAudioParam);
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_MOVIE_ESPLAYER_AUDIOINFO;
        astExtenInfoParam[u32ExtParamIndex].u32Paramsize = sizeof(ST_MMSDK_ES_AUDIO_INFO);
        astExtenInfoParam[u32ExtParamIndex].pParam = &stEsAudioInfo;
        u32ExtParamIndex++;
    }

    if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_AVP)
    {
        snprintf((char*)au8AppTypeName, MMSDK_AP_TYPE_MAX_LEN, "AVP_Flow");
        _gstMedia.eAppType = E_MMSDK_AP_TYPE_STRING;
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_AP_TYPE_STRING;
        astExtenInfoParam[u32ExtParamIndex].u32Paramsize = sizeof(au8AppTypeName);
        astExtenInfoParam[u32ExtParamIndex].pParam = &au8AppTypeName;
        u32ExtParamIndex++;
    }
    else if (pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_FORCE_8bit)
    {
        snprintf((char*)au8AppTypeName, MMSDK_AP_TYPE_MAX_LEN, "Force_8bit");
        _gstMedia.eAppType = E_MMSDK_AP_TYPE_STRING;
        astExtenInfoParam[u32ExtParamIndex].u32Option = E_MMSDK_MEDIAINFO_EXT_AP_TYPE_STRING;
        astExtenInfoParam[u32ExtParamIndex].u32Paramsize = sizeof(au8AppTypeName);
        astExtenInfoParam[u32ExtParamIndex].pParam = &au8AppTypeName;
        u32ExtParamIndex++;
    }

    if(MMSDK_CreateMediaItem(&_gstMedia, (ST_MMSDK_PARAM*)&astExtenInfoParam, u32ExtParamIndex, &_gItem[u32Idx][eMediaType]) != TRUE)
    {
        MM_ERR("!!!!!MM_CreateMediaItem returns fail in playfile \n");
        goto FUNC_END;
    }

    if(_gItem[u32Idx][eMediaType] == NULL)
    {
        MM_ERR("Create Meida item fail. u32Item is NULL in playfile\n");
        goto FUNC_END;
    }

    if( _geSeamlessMode == MPLAYER_SEAMLESS_SMOTH )
    {
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            MMSDK_SetSeamlessMode(E_MMSDK_SEAMLESS_SMOTH);
        }
    }
    else
    {
        MMSDK_SetSeamlessMode(E_MMSDK_SEAMLESS_NONE);
    }

    MMSDK_SetLoop(_gItem[u32Idx][eMediaType], _gu8IsRepeat);

    if(_gThreadEvtProcess[u32Idx][eMediaType].iId == -1)
    {
        if(TH_EVT_PROC_STACK_SIZE)
        {
            _gThreadEvtProcess[u32Idx][eMediaType].pStack = MsOS_AllocateMemory(_gThreadEvtProcess[u32Idx][eMediaType].u32StackSize, s32MstarNonCachedPoolID);
            if(!_gThreadEvtProcess[u32Idx][eMediaType].pStack)
            {
                MM_ERR("MApi_MPlayer_PlayFile(): _gThreadEvtProcess[%d] stack malloc failed......\n",eMediaType);
                goto FUNC_END;
            }
        }

        void (*EvtProcess)(MS_U32, void*);
        if (pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_ES_STREAM)
        {
            EvtProcess = _Demo_ESplayer_EvtProcess;
        }
        else
        {
            EvtProcess = _gpEvtProcess[u32Idx][eMediaType];
        }

        _gThreadEvtProcess[u32Idx][eMediaType].iId = MsOS_CreateTask(EvtProcess,
                                    (MS_U32)NULL,
                                    _gThreadEvtProcess[u32Idx][eMediaType].ePriority,
                                    TRUE,
                                    _gThreadEvtProcess[u32Idx][eMediaType].pStack,
                                    _gThreadEvtProcess[u32Idx][eMediaType].u32StackSize,
                                    _gThreadEvtProcess[u32Idx][eMediaType].szName);

        if(_gThreadEvtProcess[u32Idx][eMediaType].iId == -1)
        {
            MM_ERR("MApi_MPlayer_PlayFile(): create Thread _gpEvtProcess failed......\n");
            _gstMedia.pCallback = NULL;
            goto FUNC_END;
        }
    }

    if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_ES_STREAM)
    {
        // allocate memory to contain the ES data

        _gpu8FeedBuf = (MS_U8*) MsOS_AllocateMemory(_gu32FeedBufSize, s32MstarNonCachedPoolID);
        if (_gpu8FeedBuf == NULL)
        {
            MM_ERR("Allocate FeedBuf[size %"DTC_MS_U32_d"] fail\n", _gu32FeedBufSize);
            return FALSE;
        }

        _Demo_MM_InitESDemuxerParameter();
        _Demo_MM_CreateParseEsTask();
        _gbStartPush = TRUE;
        _gbESPlay[u32Idx][eMediaType] = TRUE;
    }

    bRet = TRUE;

FUNC_END:
    if(pMplayerMedia->u32PlayFlag & MPLAYER_PLAY_FLAG_FILEIN)
    {
        if(u32Idx < MMSDK_MEDIA_ITEM_MAX)
        {
            if(bRet == FALSE && _gpStream[u32Idx][eMediaType])
            {
                _Demo_FileClose(_gpStream[u32Idx][eMediaType]);
                _gpStream[u32Idx][eMediaType] = NULL;
            }
        }
    }
    MM_TRACE("------> Leave func MApi_MPlayer_PlayFile \n");
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    if (bRet == TRUE)
    {
        while(TRUE)
        {
            MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);

            if (E_MM_PLAY_STATUS_OK == _gePlayStatus[u32Idx])
            {
                bRet = TRUE;
                break;
            }
            else if (E_MM_PLAY_STATUS_FAIL == _gePlayStatus[u32Idx])
            {
                bRet = FALSE;
                break;
            }
        }
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to play multi-media file
/// @param[in] pFileOption enumeration of EN_MM_OPTION_TYPE in msAPI_MM.h
///     0: File Mode
/// @sa
/// @note
/// Command: \b MM_FILE \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Play(MS_U32 *pFileOption, MS_U8* U8FileName)
{
    MS_BOOL B_Ret = FALSE;
    MPLAYER_MEDIA sMplayerMeida;
    MS_U32 u32FileNameLen;
    MS_U8 U8File[FILE_NAME_LENGTH];
    MS_U8 u8FileExtName[EXTENTION_NAME_LENGTH]={};
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_U8 *pu8ExtName=NULL;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U32 u32PlayFlag;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(!b_MM_Initied)
        return B_Ret;

    printf("\n================== MM file mode setting ==================\n");
    printf("Bit 0-3 = File mode (0=File in, 1=Seekable stream, 2=Unseekable stream, 3=es stream)\n");
    printf("Bit 4 = Enable AVP\n");
    printf("Bit 5 = Enable DIP\n");
    printf("Bit 6 = Enable Multi Dec\n");
    printf("Bit 8 = Enable force 8 bit mode\n");
    printf("Bit 9 = Disable Dynamic Scaling\n");
    printf("Ex1: 0x70: Enable AVP + DIP + Multi DEC + File in mode\n");
    printf("Ex2: 0x102: Enable force 8 bit mode + Unseekable stream\n");
    printf("==========================================================\n");

    u8Volume = DEFAULT_VOLUME;
    Demo_Audio_SetAbsoluteVolume(&u8Volume);
    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_SPDIF_SetMode(MSAPI_AUD_SPDIF_PCM);

    if(strlen((char*)U8FileName) >= FILE_NAME_LENGTH-1)
    {
        MM_ERR("file length too long\n");
        return B_Ret;
    }

    strncpy((char*)U8File, (char*)U8FileName, FILE_NAME_LENGTH);
    strncpy((char*)_gpu8FileName, (char*)U8FileName, FILE_NAME_LENGTH);
    memset(&sMplayerMeida, 0, sizeof(MPLAYER_MEDIA));

    u32PlayFlag = _Demo_MM_FileMode2PlayFlag(*pFileOption);

#if (DEMO_MM_AVP_TEST == 0)
    if (u32PlayFlag & MASK_AVP_PLAY_FLAG)
    {
        MM_ERR("Do not support AVP play flags\n");
        return B_Ret;
    }
#endif

    //if(*pFileOption != MPLAYER_FILE_MODE)
    //if(!(u32PlayFlag & MPLAYER_PLAY_FLAG_FILEIN))
    //    _gu8IsRepeat = FALSE; //currently only support E_MMSDK_FILE_TYPE_MASS_STORAGE

#if (DEMO_MM_AVP_TEST == 0)
    if(u32PlayFlag & (MPLAYER_PLAY_FLAG_FILEIN | MPLAYER_PLAY_FLAG_ES_STREAM))
#else
    if(u32PlayFlag & (MPLAYER_PLAY_FLAG_FILEIN | MPLAYER_PLAY_FLAG_SEEKABLE_STREAM |
                      MPLAYER_PLAY_FLAG_UNSEEKABLE_STREAM | MPLAYER_PLAY_FLAG_ES_STREAM))
#endif
    {
        u32FileNameLen = strlen((char *)U8File);
        pu8ExtName = (MS_U8*)U8File + u32FileNameLen - 1;
        while(pu8ExtName > U8File)
        {
            if(pu8ExtName[0] == '.')
            {
                pu8ExtName++;
                break;
            }
            else if(pu8ExtName[0] == '/')
            {   // no extension name.
                return B_Ret;
            }
            pu8ExtName--;
        }

        if(pu8ExtName > U8File)
        {
            MS_U32 i = 0;
            while(pu8ExtName[i])
            {
                //prevent the extension becomes upper case, and may cause the file can't be opened
                u8FileExtName[i] = toupper(pu8ExtName[i]);
                i++;
            }

            // coverity[buffer_size_warning]
            strncpy((char *)_gu8FileExtName, (char *)u8FileExtName, EXTENTION_NAME_LENGTH);

            if (strcmp("PCM", (char *)u8FileExtName) == 0)
            {
                //=========================PCM========================
                _Demo_MPlayer_SetOption(MMSDK_MEDIA_ITEM_MIN, E_MMSDK_MEDIA_TYPE_MUSIC, MPLAYER_OPTION_SET_MUSIC_TYPE, MPLAYER_SUBTYPE_PCM);
                MPLAYER_PCM_PARAM* pPCM_Param;
                pPCM_Param = (MPLAYER_PCM_PARAM*)MsOS_AllocateMemory(sizeof(MPLAYER_PCM_PARAM), s32MstarNonCachedPoolID);
                if(pPCM_Param == NULL)
                {
                    MM_ERR("PCM stream, Allocate Memory Fail\n");
                    return B_Ret;
                }
                pPCM_Param->eFormatTag = PCM_FORMAT_PCM;
                pPCM_Param->u16Channels = 2;
                pPCM_Param->u16BitsPerSample = 16;
                pPCM_Param->u32SamplesPerSec = 44100;
                pPCM_Param->bBigEndian = TRUE;
                // coverity[overrun-call]
                _Demo_MPlayer_SetOption(MMSDK_MEDIA_ITEM_MIN, E_MMSDK_MEDIA_TYPE_MUSIC, MPLAYER_OPTION_MUSIC_SET_PCM_PARAM, (unsigned int)pPCM_Param);
                MsOS_FreeMemory(pPCM_Param, s32MstarNonCachedPoolID);
            }

            for (i = 0; i < sizeof(_gstZmmSupportExtTable) / sizeof(Z_File_Ext_Info); i++)
            {
                if (strcmp(&_gstZmmSupportExtTable[i].FileExt[0], (char *)u8FileExtName) == 0)
                {
                    _geMediaType = _gstZmmSupportExtTable[i].FileType;
                    break;
                }
            }
            if(i == sizeof(_gstZmmSupportExtTable) / sizeof(Z_File_Ext_Info))
                return B_Ret; //extension name is not support yet
        }

        MM_MUST("File name = %s\n", U8File);

        if(_geMediaType != E_MMSDK_MEDIA_TYPE_MOVIE && (u32PlayFlag & MASK_AVP_PLAY_FLAG))
        {
            MM_ERR("Music & Photo do not support AVP play flags\n");
            return B_Ret;   // Music & Phot do not support AVP play flags
        }
#if (DEMO_DMS_TEST == 1)
        else if (_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && !(u32PlayFlag & MPLAYER_PLAY_FLAG_AVP))
        {
            MM_ERR("Enable DMS feature, Do not support VDP flow\n");
            return B_Ret;
        }
#endif

        //set display window, {0, 0, 0, 0} means full panel
        sMplayerMeida.u32PlayFlag = u32PlayFlag;
        sMplayerMeida.u32DispX = 0;
        sMplayerMeida.u32DispY = 0;
        sMplayerMeida.u32DispW = IPANEL(&gDevId ,Width);
        sMplayerMeida.u32DispH = IPANEL(&gDevId ,Height);
        sMplayerMeida.u32GotoTimeMs = 0;
        sMplayerMeida.filename = (char*)U8File;//utf8fn;

        B_Ret = _Demo_MPlayer_PlayFile(_geMediaType, &sMplayerMeida);
    }
    else
        MM_ERR("Error!! Not supportted file type (=%d)!!", (int)(*pFileOption & MASK_FILE_MODE));

    return B_Ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code to back-forward replay multi-media file , need set before mm play and after mm init
/// @param[in] pu32On: 1 for back-forward replay on, 0 for back-forward replay off
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_RW_Replay \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_RW_Replay(MS_U32 *pu32On)
{
    if ((*pu32On != 0) && (*pu32On != 1))
    {
        return FALSE;
    }

    if (_gpu8FileName)
    {
        _Demo_MPlayer_SetOption(MMSDK_MEDIA_ITEM_MIN, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_FB_REPLAY, (unsigned int)*pu32On);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to get MM-photo decode buffer physical address
/// @return decode buffer physical address
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_U32 Demo_MM_GetOutAdr(void)
{
    return MEM_ADR_BY_MIU(JPD_OUT_ADR, JPD_OUT_MEMORY_TYPE) + _gu32GifAddrOffset;
}

MS_BOOL Demo_MM_IsPlay(void)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    return _gbPlay[u32Idx][_geMediaType];
}

//------------------------------------------------------------------------------
/// @brief The sample code to change Audio track
/// @param u32TrackId:
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_AudioTrack \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_SetAudioTrack(MS_U32 *u32TrackId)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_U32 u32AudioTrackNum;
    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return FALSE;
    }

    u32AudioTrackNum = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_AUDIO_TRACK);
    MM_MUST("Audio Track Num  = %"DTC_MS_U32_d"\n", u32AudioTrackNum);

    if (u32AudioTrackNum <= *u32TrackId)
    {
        MM_ERR("Track Id(%"DTC_MS_U32_d") must <= u32AudioTrackNum\n",u32AudioTrackNum);

        return FALSE;
    }

    _Demo_MPlayer_SetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_AUDIO, *u32TrackId);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change subtitle track
/// @param u32TrackId:
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_SubtitleTrack \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_SetSubtitleTrack(MS_U32 *u32TrackId)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_U32 u32SubtitleTrackNum;
    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return FALSE;
    }

    u32SubtitleTrackNum = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_SUBTITLE_TRACK);
    MM_MUST("Subtitle Track Num  = %"DTC_MS_U32_d"\n", u32SubtitleTrackNum);

    if (u32SubtitleTrackNum <= *u32TrackId)
    {
        MM_ERR("Track Id(%"DTC_MS_U32_d") must <= u32SubtitleTrackNum\n", u32SubtitleTrackNum);
        return FALSE;
    }

    _Demo_MPlayer_SetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE, *u32TrackId);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change program track
/// @param u32ProgramId:
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_Program \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_SetProgramTrack(MS_U32 *u32ProgramId)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_U32 u32SelectProgID = *u32ProgramId;
    MS_U32 u32ProgramNum;
    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return FALSE;
    }

    if(_Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO,MPLAYER_INFO_PROGRAM_ID) == u32SelectProgID)
    {
        MM_ERR("Program Id(%"DTC_MS_U32_d") == Current Program id\n", u32SelectProgID);
        return FALSE;
    }

    u32ProgramNum = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_PROGRAM);
    MM_MUST("Program Num  = %"DTC_MS_U32_d"\n", u32ProgramNum);

    if (u32ProgramNum <= u32SelectProgID)
    {
        MM_ERR("Program Id(%"DTC_MS_U32_d") must <= Program Num\n", u32SelectProgID);
        return FALSE;
    }

    if(_Demo_MPlayer_SetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_PROGRAM, u32SelectProgID))
    {
        MM_WAIT_EVENT(_gs32MediaEvtGrp, MM_EVENT_MM_PLAY_STATUS);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to rotate the photo
/// @param u32Direction: 0=>left, 1=>right
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_Rotate \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Display_Rotate(MS_U32* u32Direction)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_BOOL bRet = FALSE;
    MS_U32 u32RotationAngle;
    u32RotationAngle = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_ROTATION, 0);

    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_PHOTO)
    {
        return bRet;
    }

    if (PT_SYS_GetMMPhotoPath() != 1)
    {
        MM_ERR("If you want to use the photo effect feature, you need to set the display path to MVOP!!\n");
        return bRet;
    }

    switch(*u32Direction)
    {
        case 0:
            //set the action of rotate to left 90 degrees.
            u32RotationAngle = (u32RotationAngle+3) % 4;
            break;
        case 1 :
            //set the action of rotate to right 90 degrees.
            u32RotationAngle = (u32RotationAngle+1) % 4;
            break;
        default:
            MM_ERR("The Direction of rotation must be left(0) or right(1) !!\n");
            return bRet;
    }

    bRet = _Demo_MPlayer_SetOption(u32Idx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_ROTATION, u32RotationAngle);

    return bRet;

}

//------------------------------------------------------------------------------
/// @brief The sample code to Zoom-In/Zoom-Out
/// @param u32ZoomMode: 0=>Zoom-Out, 1=>Zoom-In
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_Zoom \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Display_Zoom(MS_U32* u32ZoomMode)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_BOOL bRet = FALSE;
    MS_U32 u32ZoomFactor;
    u32ZoomFactor = _Demo_MPlayer_GetOption(u32Idx, _geMediaType, MPLAYER_OPTION_ZOOM, 0);

    if(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_MULTI_DEC)
    {
        MM_ERR("This feature does not support to n-decode!!\n");
        return bRet;
    }

    if (_geMediaType != E_MMSDK_MEDIA_TYPE_PHOTO && _geMediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return bRet;
    }

    if (_geMediaType == E_MMSDK_MEDIA_TYPE_PHOTO && PT_SYS_GetMMPhotoPath() != 1)
    {
        MM_ERR("If you want to use the photo effect feature, you need to set the display path to MVOP!!\n");
        return bRet;
    }

    switch(*u32ZoomMode)
    {
        case 0:
            //Zoom-Out
            switch(u32ZoomFactor)
            {
                case E_MMSDK_VIEW_ZOOM_1_DIV_8:
                    MM_MUST("The Zoom Size has been reduced to the minimum!!\n");
                    return bRet;
                case E_MMSDK_VIEW_ZOOM_1_DIV_4 :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1_DIV_8;
                    break;
                case E_MMSDK_VIEW_ZOOM_1_DIV_2 :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1_DIV_4;
                    break;
                case E_MMSDK_VIEW_ZOOM_1X  :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1_DIV_2;
                    break;
                case E_MMSDK_VIEW_ZOOM_2X  :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1X;
                    break;
                case E_MMSDK_VIEW_ZOOM_4X :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_2X;
                    break;
                case E_MMSDK_VIEW_ZOOM_8X :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_4X;
                    break;
                default:
                    return bRet;
            }
            break;
        case 1 :
            //Zoom-In
            switch(u32ZoomFactor)
            {
                case E_MMSDK_VIEW_ZOOM_1_DIV_8:
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1_DIV_4;
                    break;
                case E_MMSDK_VIEW_ZOOM_1_DIV_4 :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1_DIV_2;
                    break;
                case E_MMSDK_VIEW_ZOOM_1_DIV_2 :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_1X;
                    break;
                case E_MMSDK_VIEW_ZOOM_1X  :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_2X;
                    break;
                case E_MMSDK_VIEW_ZOOM_2X  :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_4X;
                    break;
                case E_MMSDK_VIEW_ZOOM_4X :
                    u32ZoomFactor=E_MMSDK_VIEW_ZOOM_8X;
                    break;
                case E_MMSDK_VIEW_ZOOM_8X :
                    MM_ERR("The Zoom Size has been enlarged to the maximum!!\n");
                    return bRet;
                default:
                    return bRet;
            }
            break;
        default:
            MM_ERR("The ZoomMode must be Zoom-Out(0) or Zoom-In(1) !!\n");
            return bRet;
    }
    //Zoom2X

    bRet = _Demo_MPlayer_SetOption(u32Idx, _geMediaType, MPLAYER_OPTION_ZOOM, u32ZoomFactor);

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to resume the photo file
/// @param none:
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_Resume \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Display_Resume(void)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_BOOL bRet = FALSE;

    if(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_MULTI_DEC)
    {
        MM_ERR("This feature does not support to n-decode!!\n");
        return bRet;
    }

    if (_geMediaType != E_MMSDK_MEDIA_TYPE_PHOTO && _geMediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return bRet;
    }

    if (_geMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
    {
        if ( PT_SYS_GetMMPhotoPath() != 1)
        {
            MM_ERR("If you want to use the photo effect feature, you need to set the display path to MVOP!!\n");
            return bRet;
        }
    else
        {
            bRet = (_Demo_MPlayer_SetOption(u32Idx, _geMediaType, MPLAYER_OPTION_ROTATION, E_MMSDK_ROTATE_0)
                && _Demo_MPlayer_SetOption(u32Idx, _geMediaType, MPLAYER_OPTION_ZOOM, E_MMSDK_VIEW_ZOOM_1X) );
        }
    }
    else if (_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        bRet = _Demo_MPlayer_SetOption(u32Idx, _geMediaType, MPLAYER_OPTION_ZOOM, E_MMSDK_VIEW_ZOOM_1X);
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to move view window if in Zoom-In mode
/// @param u32Direction: 0=>up , 1=>down, 2=>left, 3=>right
/// @param moveOffset: 0=>default is 250, other is by value
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_MoveView \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Display_MoveView(MS_U32* u32Direction, MS_U16* moveOffset)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    MS_BOOL bRet = FALSE;
    MS_U32 uDirect;
    uDirect = *u32Direction;

    if(_gu32FilePlayFlag[u32Idx][_geMediaType] & MPLAYER_PLAY_FLAG_MULTI_DEC)
    {
        MM_ERR("This feature does not support to n-decode!!\n");
        return bRet;
    }

    if (_geMediaType != E_MMSDK_MEDIA_TYPE_PHOTO && _geMediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return bRet;
    }

    if (_geMediaType == E_MMSDK_MEDIA_TYPE_PHOTO && PT_SYS_GetMMPhotoPath() != 1)
    {
        MM_ERR("If you want to use the photo effect feature, you need to set the display path to MVOP!!\n");
        return bRet;
    }

    if (E_MPLAYER_DIRECTION_MAX  <= *u32Direction)
    {
        MM_ERR("The direction must be set in [0-3]!!\n");
        return bRet;
    }

    MPLAYER_MOVEVIEW_PARAM* pMove_Param = NULL;
    pMove_Param = (MPLAYER_MOVEVIEW_PARAM*)u32Direction;
    memset(pMove_Param, 0, sizeof(MPLAYER_MOVEVIEW_PARAM));
    pMove_Param->eDirection= uDirect;
    if (*moveOffset == 0)
    {
        pMove_Param->u16Offset= 250;
    }
    else
    {
        pMove_Param->u16Offset= *moveOffset;
    }

    bRet = _Demo_MPlayer_SetOption(u32Idx, _geMediaType, MPLAYER_OPTION_MOVE_VIEW, (unsigned int)pMove_Param);

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set photo media item slide show effect
/// @param eEffect          \b IN: Slide show effect.
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_MoveView \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_Set_SlideShowEffect(MS_U32* u32Effect)
{
    MS_BOOL bRet = FALSE;
    if (PT_SYS_GetMMPhotoPath() != 1)
    {
        MM_ERR("If you want to use the photo effect feature, you need to set the display path to MVOP!!\n");
        return bRet;
    }
    //MMSDK will not support FADE-IN/OUT effects.
    if (E_MMSDK_SLIDE_SHOW_EFFECT_FADE_IN <= *u32Effect)
    {
        MM_ERR("The slide show effectt must be set in [0-60]!!\n");
        return bRet;
    }

    // coverity[overrun-call]
    bRet = _Demo_MPlayer_SetOption(MMSDK_MEDIA_ITEM_MIN, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_ENABLE_SLIDE_SHOW, *u32Effect);

    return bRet;
}

MS_BOOL Demo_MM_Get_DrawPhotoInfo(MS_U16* u16width, MS_U16* u16height, MS_U16* u16pitch, MS_U16* u16format)
{
    MS_U32 u32Idx = _gu32CurrentMediaItem;
    *u16width = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODED_WIDTH);
    *u16height = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODED_HEIGHT);
    *u16pitch = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODED_PITCH);
    *u16format = _Demo_MPlayer_GetOption(u32Idx, E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODE_FORMAT);
    MM_MUST("[Demo_MM_Get_DrawPhotoInfo] %d %d %d %d\n",*u16width,*u16height,*u16pitch,*u16format);
    return TRUE;
}

MS_BOOL Demo_MM_Set_DrawPhotoPath(MS_U32* u32Path)
{
    if(*u32Path == 1)
    {
        PT_SYS_SetMMPhotoPath(1);
        MM_MUST("[Demo_MM_Set_DrawPhotoPath] use MVOP output\n");
    }
    else
    {
        PT_SYS_SetMMPhotoPath(0);
        MM_MUST("[Demo_MM_Set_DrawPhotoPath] use GOP output\n");
    }
    return TRUE;
}

MS_BOOL Demo_MM_SetDebugLevel(MS_U32* u32DebugLevel)
{
    if (u32DebugLevel == NULL)
    {
        MM_ERR("u32DebugLevel == NULL\n");
        return FALSE;
    }
    else
    {
        EN_MMSDK_DBG_LEVEL eMMSDKDebugLevel;
        if (*u32DebugLevel > E_MMSDK_DBG_LEVEL_FW)
        {
            eMMSDKDebugLevel = E_MMSDK_DBG_LEVEL_TRACE;
        }
        else
        {
            eMMSDKDebugLevel = *u32DebugLevel;
        }
        MM_MUST("Debug Level is %"DTC_MS_U32_d"\n", (MS_U32)eMMSDKDebugLevel);

        PT_SYS_SetDebugLevel(eMMSDKDebugLevel);

        // coverity[overrun-call]
        return _Demo_MPlayer_SetOption(MMSDK_MEDIA_ITEM_MIN, _geMediaType, MPLAYER_OPTION_SET_DBG_LEVEL, (unsigned int)(eMMSDKDebugLevel));
    }
}

MS_BOOL Demo_MM_SetEnableMotionGif(MS_U32* bEnableMGIF)
{
    MM_DBG("Set Motion GIF: %"DTC_MS_U32_d"\n", *bEnableMGIF);

    _gbEnableMotionGif = (MS_BOOL)*bEnableMGIF;
    return TRUE;
}

MS_BOOL Demo_MM_SetWindow(MS_U32* u32windowId, MS_U16* u16X, MS_U16* u16Y, MS_U16* u16Width, MS_U16* u16Height)
{
    MS_U32 u32Idx = *u32windowId;
    MS_U32 u32MediaType;
    ST_RECT stRect = {0, 0, 0, 0};    // temp, need get real size
    stRect.u16Width = IPANEL(&gDevId ,Width);
    stRect.u16Height = IPANEL(&gDevId ,Height);

    MPLAYER_DISP_WINDOW *pDispWindow = NULL;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(u32Idx >= MMSDK_MEDIA_ITEM_MAX)
    {
        MM_ERR("Wrong Item ID\n");
        return FALSE;
    }

    if((*u16X < stRect.u16X ||*u16X > stRect.u16Width) ||
        (*u16Y < stRect.u16Y ||*u16Y > stRect.u16Height) ||
        (*u16Width < stRect.u16X ||*u16Width > stRect.u16Width) ||
        (*u16Height < stRect.u16Y ||*u16Height > stRect.u16Height))
    {
        MM_ERR("Invalid window size(%d, %d, %d, %d), the value must in (%d, %d, %d, %d)\n",
           *u16X, *u16Y, *u16Width, *u16Height, stRect.u16X, stRect.u16Y, stRect.u16Width, stRect.u16Height);
        return FALSE;
    }

    _gstDipRect[u32Idx].u16X = *u16X;
    _gstDipRect[u32Idx].u16Y = *u16Y;
    _gstDipRect[u32Idx].u16Width= *u16Width;
    _gstDipRect[u32Idx].u16Height= *u16Height;

    u32MediaType = _Demo_MM_GetMediaType(u32Idx);
    if(u32MediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MM_ERR("The item(%"DTC_MS_U32_d") is not playing Movie (media type=%"DTC_MS_U32_d")\n",u32Idx,  u32MediaType);
    }
    else
    {
        pDispWindow = (MPLAYER_DISP_WINDOW*)MsOS_AllocateMemory(sizeof(MPLAYER_DISP_WINDOW), s32MstarNonCachedPoolID);
        if(pDispWindow == NULL)
        {
            MM_ERR("Alloc Rect memory fail\n");
            return FALSE;
        }
        pDispWindow->u32DispX= (MS_U32)_gstDipRect[u32Idx].u16X;
        pDispWindow->u32DispY = (MS_U32)_gstDipRect[u32Idx].u16Y;
        pDispWindow->u32DispW = (MS_U32)_gstDipRect[u32Idx].u16Width;
        pDispWindow->u32DispH = (MS_U32)_gstDipRect[u32Idx].u16Height;
        _Demo_MPlayer_SetOption(u32Idx, (EN_MMSDK_MEDIA_TYPE)u32MediaType, MPLAYER_OPTION_DISPLAY_WINDOW, (unsigned int)pDispWindow);
        MsOS_FreeMemory(pDispWindow, s32MstarNonCachedPoolID);
    }

    return TRUE;
}

MS_BOOL Demo_MM_SetKeepRatio(MS_U32* pu32EnableKeepRatio)
{
    if((*pu32EnableKeepRatio != 0) && (*pu32EnableKeepRatio != 1))
    {
        MM_ERR("Wrong value(%"DTC_MS_U32_d") for setting KeepRation\n", *pu32EnableKeepRatio);
        return FALSE;
    }

    _gbKeepRatio = (MS_BOOL)*pu32EnableKeepRatio;

    return TRUE;
}

#if (DEMO_MM_AVP_TEST == 1)

MS_BOOL Demo_MM_SwitchMediaItemAudio(MS_U32 *u32ItemId)
{
    MS_U32 u32Idx = *u32ItemId;
    MS_U32 u32MediaType = _Demo_MM_GetMediaType(u32Idx);
    MS_U32 u32Mute = TRUE;

    if ((EN_MMSDK_MEDIA_TYPE)u32MediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MM_ERR("The stream ID(%"DTC_MS_U32_d") is not playing Movie\n", u32Idx);
        return FALSE;
    }

    u32Mute = TRUE;
    Demo_MM_MuteMediaItem(&_gu32CurrentPlayAudioItem, &u32Mute);

    u32Mute = FALSE;
    Demo_MM_MuteMediaItem(&u32Idx, &u32Mute);

    return TRUE;
}

MS_BOOL Demo_MM_MuteMediaItem(MS_U32* u32ItemId, MS_U32* bMute)
{
    MS_U32 u32Idx = *u32ItemId;
    MS_U32 u32MediaType = _Demo_MM_GetMediaType(u32Idx);
    MS_BOOL bIsMute = *bMute;

    if ((EN_MMSDK_MEDIA_TYPE)u32MediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MM_ERR("The media item ID(%"DTC_MS_U32_d") is not playing Movie\n", u32Idx);
        return FALSE;
    }

    MM_DBG("Media Item Id(%"DTC_MS_U32_d") \n", u32Idx);
    if(bIsMute == TRUE)
    {
        _Demo_MPlayer_SetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_AUDIO, -1);
    }
    else
    {
        _Demo_MPlayer_SetOption(u32Idx, E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_AUDIO, 0);  // set default audio track when changing playing stream
        _gu32CurrentPlayAudioItem = u32Idx;
    }

    return TRUE;
}

MS_BOOL Demo_MM_MuteMediaItem_All(void)
{
    MS_U32 u32ItemIdx = 0;
    MS_U32 u32Mute = 1; //mute
    for (u32ItemIdx = MMSDK_MEDIA_ITEM_MIN; u32ItemIdx< MMSDK_MEDIA_ITEM_MAX; u32ItemIdx++)
    {
        if (Demo_MM_MuteMediaItem(&u32ItemIdx, &u32Mute) == FALSE)
        {
            MM_ERR("Mute Media Item ID(%"DTC_MS_U32_d") fail\n", u32ItemIdx);
            return FALSE;
        }
    }
    return TRUE;
}

MS_BOOL Demo_MM_SetOperableMediaItem(MS_U32* u32ItemId)
{
    MS_U32 u32Idx = *u32ItemId;
    MS_U32 u32MediaType = _Demo_MM_GetMediaType(u32Idx);

    if(u32MediaType == MMSDK_MEDIA_TYPE_INVALID)
    {
        MM_ERR("The item ID(%"DTC_MS_U32_d") is NULL\n", u32Idx);
        return FALSE;
    }

    _gu32CurrentMediaItem = u32Idx;
    return TRUE;
}

MS_BOOL Demo_MM_SetAVPZOrder(MS_U32* u32windowId, MS_U32* u32Layer)
{
    MS_U32 u32Idx = *u32windowId;
    MS_U8 u8Layer = (MS_U8)*u32Layer;
    MS_U32 u32MediaType = _Demo_MM_GetMediaType(u32Idx);
    MS_BOOL bRet = FALSE;

    if(u32Idx < MMSDK_MEDIA_ITEM_MIN || u32Idx >= MMSDK_MEDIA_ITEM_MAX)
    {
        MM_ERR("Wrong Item ID\n");
        return bRet;
    }

    if(u8Layer > MM_MAX_ZORDER_LAYER)
    {
        MM_ERR("Wrong Z-Order Layer!\n");
        return bRet;
    }

    bRet = PT_SYS_SetAVPZOrder((MS_U8)u32Idx, u8Layer);

    if (TRUE != bRet)
    {
        MM_ERR("The layer_%"DTC_MS_U32_d" has already been used for other window!\n", u8Layer);
        return bRet;
    }

    if(u32MediaType != E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        MM_ERR("The item(%"DTC_MS_U32_d") is not playing Movie (media type=%"DTC_MS_U32_d")\n",u32Idx,  u32MediaType);
    }
    else
    {
        _Demo_MPlayer_SetOption(u32Idx, (EN_MMSDK_MEDIA_TYPE)u32MediaType, MPLAYER_OPTION_SET_AVP_ZORDER, 0);
    }

    MM_MUST("The layer of window:[%"DTC_MS_U32_d"] is set to LAYER_%"DTC_MS_U32_d"!\n",u32Idx, u8Layer);
    return bRet;
}

MS_BOOL Demo_MM_ResetWindow(void)
{
    MS_U32 u32ItemIdx;
    MS_U16 u16HalfDispWidth = IPANEL(&gDevId ,Width) / 2;
    MS_U16 u16HalfDispHeight = IPANEL(&gDevId ,Height) / 2;

    for (u32ItemIdx = MMSDK_MEDIA_ITEM_MIN; u32ItemIdx < MMSDK_MEDIA_ITEM_MAX; u32ItemIdx++)
    {
        _gstDipRect[u32ItemIdx].u16Width = u16HalfDispWidth;
        _gstDipRect[u32ItemIdx].u16Height = u16HalfDispHeight;

        PT_SYS_InitAVPZOrder();
    }

    _gstDipRect[0].u16X = 0;
    _gstDipRect[0].u16Y = 0;
    _gstDipRect[1].u16X = u16HalfDispWidth;
    _gstDipRect[1].u16Y = 0;
    _gstDipRect[2].u16X = 0;
    _gstDipRect[2].u16Y = u16HalfDispHeight;
    _gstDipRect[3].u16X = u16HalfDispWidth;
    _gstDipRect[3].u16Y = u16HalfDispHeight;

    return TRUE;
}

MS_BOOL Demo_MM_SetAudioSwitchMode(MS_U32* u32Mode)
{
    _geAudioSwitchMode = *u32Mode;
    return TRUE;
}

MS_U32 _Demo_MM_Str2Dec(MS_U8 *pu8Str)
{
    if((pu8Str[0]=='0') && (pu8Str[1]=='X'||pu8Str[1]=='x'))
    {
        MS_U32 u32Val = 0;
        MS_U32 u32CharNo = 0;
        for(u32CharNo=2; u32CharNo<strlen((char*)pu8Str); u32CharNo++)
        {
            if(pu8Str[u32CharNo]==' '||pu8Str[u32CharNo]=='\t')
            {
                break;
            }
            u32Val <<=4;
            if('0'<=pu8Str[u32CharNo]&&pu8Str[u32CharNo]<='9')
            {
                u32Val += (pu8Str[u32CharNo]-'0');
            }
            else if('a'<=pu8Str[u32CharNo]&&pu8Str[u32CharNo]<='f')
            {
                u32Val += (pu8Str[u32CharNo]- 'a' + 10);
            }
            else if('A'<=pu8Str[u32CharNo]&&pu8Str[u32CharNo]<='F')
            {
                u32Val += (pu8Str[u32CharNo]- 'A' + 10);
            }
        }

        return u32Val;
    }
    return 0;

}

MS_BOOL Demo_MM_PlayMultiStreams(MS_U8* u8SetIdx)
{
    const char *FilePath = "/config/MM/demo_multi_play.txt";
    FILE *pfTestPlayMulti = (FILE*)_Demo_FileOpen(FilePath, (char* const)"r");

    MS_U32 u32StrLen = 256;
    char pu8Line[u32StrLen];
    char *pu8Tok = NULL;
    char pu8StreamPath[u32StrLen];
    MS_U32 u32PlayMode = 0;

    if(pfTestPlayMulti == NULL)
    {
        MM_ERR("Read File Fail, return FALSE\n");
        return FALSE;
    }

    while(fgets(pu8Line, u32StrLen, pfTestPlayMulti) != NULL)
    {
        pu8Line[strlen((char*)pu8Line) - 1] = '\0';
        if(strcmp(pu8Line, (char *)u8SetIdx) == 0)
        {
            MS_U32 u32Count = 4;
            while(u32Count-- > 0)
            {
                if(fgets(pu8Line, 256, pfTestPlayMulti) == NULL)
                    continue;

                pu8Tok = strtok(pu8Line, " \n");                // MM_PLAY

                pu8Tok = strtok(NULL, " \n");                   // play mode
                u32PlayMode = _Demo_MM_Str2Dec((MS_U8*)pu8Tok);

                pu8Tok = strtok(NULL, " \n");                   // stream path
                snprintf(pu8StreamPath, u32StrLen, "%s", pu8Tok);
                MM_MUST("MM_PLAY 0x%x %s\n", u32PlayMode, pu8StreamPath);
                Demo_MM_Play(&u32PlayMode, (MS_U8*)pu8StreamPath);
            }
            break;
        }
    }

    _Demo_FileClose((void* const)pfTestPlayMulti);
    return TRUE;
}


#endif

#if (DEMO_APM_TEST == 1)
#include "drvGPIO.h"

MS_BOOL b_IPC_MM_Initied=FALSE;
MS_BOOL Demo_MM_IPCINIT(void)
{
    MS_BOOL B_Ret = TRUE;
    MS_U8 u8Volume = DEFAULT_VOLUME;

    Demo_DMX_Init();

#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif
    u8Volume = DEFAULT_VOLUME;

    Demo_Audio_SetAbsoluteVolume(&u8Volume);

    b_IPC_MM_Initied = TRUE;
    return B_Ret;
}

MS_BOOL Demo_MM_IPCDeInit(void)
{
    if(b_IPC_MM_Initied == FALSE)
    {
        return FALSE;
    }
    b_IPC_MM_Initied = FALSE;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code is merely for HbbTV case to prevent from missing audio when playing VOD.
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b Demo_MM_AUDIO \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MM_AUDIO(void)
{
    MS_U32 type = 0;
    Demo_Audio_Init((MS_U32*)&type);
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        Demo_MM_AllocAudDec();
    }

    return TRUE;
}
#endif // #if (DEMO_APM_TEST == 1)
#endif
