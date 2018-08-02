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
#include "apiVDEC.h"
#include "apiDMX.h"
#include "apiHDMITx.h"
#include "apiVDEC_EX.h"

#include "demo_utility.h"
#include "demo_osd.h"
#include "demo_audio.h"
#include "demo_audio_multi.h"

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

#include "porting_sysinfo.h"
#include <sys/stat.h>
#include "MsFS.h"
#include "demo_mm.h"
#include <pthread.h>
#include <assert.h>
#include "MM_Player_Def.h"
#include "MApi_MEMConfig_Interface.h"
#include "drvDTC.h"

#define MM_DBG 0
#if MM_DBG
#define mm_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define mm_print(fmt, args...)  while(0);
#endif
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

#define MM_SET_VIDEO_NOTIFY_TYPE(VideoNotiyType) \
    do{ \
        MsOS_ObtainMutex(_gs32Mutex, MSOS_WAIT_FOREVER); \
        _geVideoNotifyType = VideoNotiyType; \
        MsOS_ReleaseMutex(_gs32Mutex); \
        MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_NOTIFY); \
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
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static EN_MMSDK_MEDIA_TYPE _geMediaType = E_MMSDK_MEDIA_TYPE_MOVIE;
static MS_U32 _gu32FileOption = MPLAYER_MODE_INVALID;
static MS_U8* _gpu8FileName = NULL;
static MS_U8 _gu8IsRepeat = FALSE;

static const Z_File_Ext_Info _gstZmmSupportExtTable[] =
{
    //Auio support Suffixes name
    {"AAC",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"M4A",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"MP3",      E_MMSDK_MEDIA_TYPE_MUSIC},
    {"WMA",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"OGG",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"WAV",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"PCM",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"AC3",     E_MMSDK_MEDIA_TYPE_MUSIC},
    {"FLAC",     E_MMSDK_MEDIA_TYPE_MUSIC},

    //Video support Suffixes name
    {"MP4",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPG",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"AVI",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DAT",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"VOB",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MOV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MKV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DIV",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DIVX",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TS",        E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPEG",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"DAT",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPA",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"M2TS",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"M2T1",    E_MMSDK_MEDIA_TYPE_MOVIE},
    {"MPE",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TRP",      E_MMSDK_MEDIA_TYPE_MOVIE},
    {"TP",        E_MMSDK_MEDIA_TYPE_MOVIE},
    {"3GP",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"RMVB",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"RM",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"FLV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"ASF",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"WMV",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"WEBM",     E_MMSDK_MEDIA_TYPE_MOVIE},
    {"SWF",     E_MMSDK_MEDIA_TYPE_MOVIE},

    //Photo support Suffixes name
    {"BMP",     E_MMSDK_MEDIA_TYPE_PHOTO},
    {"PNG",     E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPG",      E_MMSDK_MEDIA_TYPE_PHOTO},
    {"JPEG",    E_MMSDK_MEDIA_TYPE_PHOTO},
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
static MS_U64 _gu64Size[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
static MS_BOOL _gbKTVMode = FALSE;
static MS_S32 _gs32MutexOpenStop = -1;
static MS_S32 _gs32Mutex = -1;

//static MS_S32 condVideoNotifyTypeValid = -1;
//static MS_S32 condMusicNotifyTypeValid = -1;
//static MS_S32 condPhotoNotifyTypeValid = -1;
static MS_S32 _gs32MediaEvtGrp = -1;

#define MM_EVENT_VIDEO_NOTIFY 0x00000001
#define MM_EVENT_MUSIC_NOTIFY 0x00000002
#define MM_EVENT_PHOTO_NOTIFY 0x00000004
#define MM_EVENT_VIDEO_PROC_EXIT 0x00000008
#define MM_EVENT_MUSIC_PROC_EXIT 0x00000010
#define MM_EVENT_PHOTO_PROC_EXIT 0x00000020

static EN_MPLAYER_NOTIFY_TYPE _geVideoNotifyType = MPLAYER_NOTIFY_NULL;
static EN_MPLAYER_NOTIFY_TYPE _geMusicNotifyType = MPLAYER_NOTIFY_NULL;
static EN_MPLAYER_NOTIFY_TYPE _gePhotoNotifyType = MPLAYER_NOTIFY_NULL;

static void (*_gpEvtProcess[MMSDK_MEDIA_TYPE_NUM])(MS_U32, void*) = {_Demo_Photo_EvtProcess, _Demo_Music_EvtProcess, _Demo_Video_EvtProcess};
#if defined(MSOS_TYPE_ECOS)
#define TH_EVT_PROC_STACK_SIZE (8*(16<<10))
#elif defined(MSOS_TYPE_LINUX)
#define TH_EVT_PROC_STACK_SIZE 0 //use pthread default stack
#endif
static Task_Info _gThreadEvtProcess[MMSDK_MEDIA_TYPE_NUM] =
{
    {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadEvt1"},
    {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadEvt2"},
    {-1, E_TASK_PRI_MEDIUM, NULL, TH_EVT_PROC_STACK_SIZE, "threadEvt3"},
};

//static MS_S32 _s32EvtProcExitEvt[MMSDK_MEDIA_TYPE_NUM] = {-1, -1, -1};

static MS_U64 _gu64CurPostn[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
static void *_gpStream[E_MPLAYER_MEDIA_STREAM_NUM] = {NULL, NULL, NULL, NULL, NULL, NULL};
static MEDIAITEM _gItem[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
static ST_MMSDK_CREATE_MEDIA_INFO _gstMedia;
static MS_BOOL _gbPlay[MMSDK_MEDIA_TYPE_NUM] = {FALSE, FALSE, FALSE};
//static MS_BOOL _gbSeekRightNow[MMSDK_MEDIA_TYPE_NUM] = { FALSE,FALSE,FALSE};
static MS_BOOL _gbUseNewAudioMode = TRUE;

static MS_U32 _gu32Recursion = 0;
static MS_S32 _gs32Owner = -1;

static MS_U64 _gu64AudioCurPostn = 0;
static MS_U64 _gu64Audio2CurPostn = 0;
static MS_U64 _gu64SubCurPostn = 0;

static MS_BOOL m_bInitialize = FALSE;
static MS_BOOL _gbBackwardRePlayMode = FALSE;
static MS_BOOL _gbExit[MMSDK_MEDIA_TYPE_NUM] = {FALSE, FALSE, FALSE};
static EN_MPLAYER_FILE_MODE _geStrmMode[MMSDK_MEDIA_TYPE_NUM] = {MPLAYER_FILE_MODE, MPLAYER_FILE_MODE, MPLAYER_FILE_MODE};
//static MS_U8 _gu8PreviewFlag[MMSDK_MEDIA_TYPE_NUM] = {0, 0, 0};
static EN_MMSDK_TRICK_MODE _geTrickMod[MMSDK_MEDIA_TYPE_NUM] = {E_MMSDK_TRICK_INVALID, E_MMSDK_TRICK_INVALID, E_MMSDK_TRICK_INVALID};
static MS_U32 _gu32TotalTime = 0;
static MS_U32 _gu32StartTime = 0;
static MS_BOOL _gbMusicBackwardRePlayMode = TRUE;  //for music
static EN_MMSDK_ROTATE_ANGLE _geRotaAng = E_MMSDK_ROTATE_0;
static EN_MMSDK_VIEW_ZOOM_FACTOR _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
static MS_U8 _gu8AudioPara[MAX_PARA_SIZE];
static EN_MPLAYER_SUB_TYPE _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
static ST_RECT _gstRect[MMSDK_MEDIA_TYPE_NUM];
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

//for debug info.....
static EN_MMSDK_DBG_LEVEL _geDebugLevel  = E_MMSDK_DBG_LEVEL_ERR;

static MS_U8 _gu8Tmp_b = 0;
static MS_U8 _gu8Tmp_a = 0;
#ifndef VDPLAYER_BS_EXT_AVAILABLE
static void* _gpBSAudBufVA;
#endif
static MS_BOOL _Demo_MM_Initialize(void)
{
    MS_BOOL bRet = FALSE;
    mm_print("_Demo_MM_Initialize.\n");

    if (m_bInitialize == TRUE)
    {
        mm_print("Already initialized\n");
        return bRet;
    }

    bRet = MMSDK_Initialize();

    MMSDK_DebugMSGLevel(_geDebugLevel);

    if (!bRet)
        MMSDK_Finalize();

    m_bInitialize = TRUE;
    mm_print("_Demo_MM_Initialize Done.\n");

    return bRet;
}

static MS_BOOL _Demo_MM_Finalize(void)
{
    MS_BOOL bRet = FALSE;
    mm_print("_Demo_MM_Finalize.\n");

    if (m_bInitialize == FALSE)
    {
        mm_print("Not initialized\n");
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
    mm_print("MM_SetCropDisplayRectangle.\n");

    memcpy(&(stInfo.stCropRect), (ST_MMSDK_RECT*)stSignedCropRect, sizeof(ST_MMSDK_RECT));
    memcpy(&(stInfo.stDispRect), (ST_MMSDK_RECT*)stSignedDispRect, sizeof(ST_MMSDK_RECT));
    stInfo.bkeepRatio = bKeepRatio;
    stInfo.bBlackScreen = bVideoMute;

    return MMSDK_SetCropDisplayRectangle(u32Item, &stInfo);
}

static MS_BOOL _Demo_MM_SetDisplayRectangle(MEDIAITEM u32Item, ST_RECT_SIGNED_POSITION *stSignedDispRect, const MS_BOOL bVideoMute, const MS_BOOL bKeepRatio)
{
    ST_RECT_SIGNED_POSITION stSignedCropRect;
    mm_print("MM_SetDisplayRectangle.\n");

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

#if(DEMO_AUDIO_MULTI_TEST == 1)
MS_BOOL Demo_MM_AllocAudDec(void)
{
    //open audio decode system
    ST_AUDIO_DEC_INFO stAudDecInfo;

    if((Demo_Audio_GetDecInfo(0, &stAudDecInfo) == FALSE) || (stAudDecInfo.eDecID == AU_DEC_INVALID))
    {
        mm_print("get audio decoder info failed!\n");
        return FALSE;
    }

    if(PT_SYS_SetAudDecID(stAudDecInfo.eDecID) == FALSE)
    {
        mm_print("set audio decoder ID failed!\n");
        return FALSE;
    }

    return TRUE;
}

MS_BOOL Demo_MM_FreeAudDec(void)
{
    //release audio decode system
    if(PT_SYS_SetAudDecID(AU_DEC_INVALID) == FALSE)
    {
        mm_print("clear audio decoder ID failed!\n");
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
        printf("=========== _gu32MetalLen is 0 ===============\n");
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
            printf("WARNING WARNING _gpu16UnicdBuffer == NULL!!!!!!\n");
        }
        ASSERT(_gpu16UnicdBuffer);
    }
    else
    {
        _gpu16UnicdBuffer = (MS_U16*)MsOS_ReallocateMemory(_gpu16UnicdBuffer, u32UnicdBufferSize, s32MstarNonCachedPoolID);
        if (_gpu16UnicdBuffer == NULL)
        {
            printf("WARNING WARNING _gpu16UnicdBuffer realloc NULL!!!!!!\n");
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
        mm_print("----------> info: %d ---UTF8-----\n", (MS_U8)_gu32MetalLen);
        u32UnicdBytes = _Demo_UTF8ToUCS2(_gpu8Buffer, _gpu16UnicdBuffer, _gu32MetalLen, 0xffff);
        u32Ret = u32UnicdBytes;
    }
    else if (eStrFormt == E_PLAYER_STRING_FORMAT_ASCII)
    {
        MS_U32 i = 0;
        MS_U32 j = 0;
        MS_S16 s16GBCode = 0;
        MS_U16 u16Unicode = 0;

        mm_print("----------> info: %d  bytes---ASCII-----\n", (int)_gu32MetalLen);
        for (j = 0, i = 0; i < _gu32MetalLen; i++, j++)
        {
            u32UnicdBytes++;
            s16GBCode = *((MS_S8 *)(_gpu8Buffer + i));
            if (s16GBCode > 0) //basic ASCII code.
            {
                u16Unicode = _Demo_ConvBig5ToUnicode((MS_U16)s16GBCode);
                mm_print("----English %d\n", s16GBCode);
            }
            else            //chinese char
            {
                MS_U16 u16EndianChange = *(MS_U16 *)(_gpu8Buffer + i);
                if(bEndianChange)
                    u16EndianChange = (u16EndianChange >> 8) + ((u16EndianChange & 0xff) << 8);

                u16Unicode = _Demo_ConvBig5ToUnicode(u16EndianChange);
                mm_print("----Chinese %d\n", u16EndianChange);
                mm_print("----Chinese Unicode %x\n", u16Unicode);

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
        mm_print("-----> len is %d-----UNICODE\n", (int)_gu32MetalLen);
    }

    return u32Ret;
}

static unsigned int _Demo_MPlayer_GetOption(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32SubOption)
{
    MS_U32 u32Ret = 0;
    EN_PLAYER_STRING_FORMAT eStrFormt = E_PLAYER_STRING_FORMAT_UNICODE;
    MS_S8 numStr[256];
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
                E_MMSDK_INFO_MOVIE_AUDIO_TRACK_LANGUAGE, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_AUDIO_TRACK_INFO:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_AUDIO_CODEC_TRACK_ID, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_SUBTITLE_TRACK_NAME:
            MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                E_MMSDK_INFO_MOVIE_SUBTITLE_TRACK_LANGUAGE, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
            break;

        case MPLAYER_OPTION_MOVIE_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_INDEX_FRAME_EXIST:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_INDEX_EXISTED, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_TOTAL_TIME:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    mm_print("-------> total_time is %d ms-----\n", (int)u32Ret);
                    break;

                case MPLAYER_INFO_CUR_TIME:
                    MMSDK_GetPlayPosition(_gItem[eMediaType], &u32Ret);
                    break;

                case MPLAYER_INFO_NB_PROGRAM:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_PROGRAM, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_NB_AUDIO_TRACK:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_NB_SUBTITLE_TRACK:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PROGRAM_ID:
                    MMSDK_GetProgramID(_gItem[eMediaType], &u8TrackID);
                    u32Ret = u8TrackID;
                    break;

                case MPLAYER_INFO_AUDIO_TRACK_ID:
                    MMSDK_GetAudioTrackID(_gItem[eMediaType], &u8TrackID);
                    u32Ret = u8TrackID;
                    break;

                case MPLAYER_INFO_SUBTITLE_TRACK_ID:
                    if (_Demo_MM_GetSubtitleTrackInfo(_gItem[eMediaType], &stSubTrackInfo))
                        u32Ret = stSubTrackInfo.u8TrackID;
                    break;

                case MPLAYER_INFO_AUDIO_CODEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_VIDEO_CODEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_VIDEO_CODEC, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_OPTION_GET_MUSIC_TYPE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_AUDIO_CODEC, 4, (void*)&u32Ret, _gItem[eMediaType]);

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
                        E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_V_SIZE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_FRM_PER_SEC:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
                        E_MMSDK_INFO_MOVIE_FRAME_PER_SEC, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                default:
                    mm_print("[ERROR] Unknown MOVIE INFO\n");
                    break;
            }
            break;

        case MPLAYER_OPTION_TRICK_MODE:
            u32Ret = (MS_U32)_geTrickMod[eMediaType];
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
            if (eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
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
                        E_MMSDK_INFO_PHOTO_OUTPUT_WIDTH, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODED_HEIGHT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_HEIGHT, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODED_PITCH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_PITCH, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODE_PROGRESS:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_DECODE_PROGRESS, sizeof(MS_U32), (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_WIDTH:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_HRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_IMAGE_ORIGINAL_HEIGHT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_VRES, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_HEIGHT:
                    u32Ret = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_WIDTH:
                    u32Ret = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width;
                    break;

                case MPLAYER_INFO_PHOTO_DISPLAYING_SIZE:
                    u32Ret = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width * _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    break;

                case MPLAYER_INFO_PHOTO_CODEC_TYPE:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_TYPE, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_PHOTO_DECODE_FORMAT:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_PHOTO_INFO,
                        E_MMSDK_INFO_PHOTO_OUTPUT_COLOR_FORMAT, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                default:
                    mm_print("[ERROR] Unknown PHOTO INFO\n");
                    break;
            }
            break;

        case MPLAYER_OPTION_MUSIC_INFO:
            switch(u32SubOption)
            {
                case MPLAYER_INFO_TOTAL_TIME:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_TOTAL_TIME, 4, (void*)&u32Ret, _gItem[eMediaType]);
                    break;

                case MPLAYER_INFO_CUR_TIME:
                    MMSDK_GetPlayPosition(_gItem[eMediaType], &u32Ret);
                    break;

                case MPLAYER_INFO_MUSIC_BITRATE_LEN:
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                        E_MMSDK_INFO_MUSIC_BITRATE, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                    if(!_Demo_GetOption_AllocateMemory(2UL * 2 * _gu32MetalLen))
                        return 0;

                    if (_gpu8Buffer)
                    {
                        MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                            E_MMSDK_INFO_MUSIC_BITRATE, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);
                    }

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
                    E_MMSDK_INFO_MUSIC_SAMPLE_FREQ, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                if(!_Demo_GetOption_AllocateMemory(4 * 4))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_SAMPLE_FREQ, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);
                }

                _Demo_itoa(*((MS_U32 *)_gpu8Buffer), numStr, 10);
                _Demo_UTF8ToUCS2((MS_U8 *)numStr, _gpu16UnicdBuffer, (MS_U16)strlen((const char *)numStr), 0xffff);
                u32Ret = strlen((const char*)numStr) * 2;
                break;

            case MPLAYER_INFO_MUSIC_YEAR_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_YEAR, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_YEAR_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_YEAR, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, FALSE);
                }
                break;

            case MPLAYER_INFO_MUSIC_YEAR:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_ARTIST_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_ARTIST, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_ARTIST_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_ARTIST, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_ARTIST:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_ALBUM_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_ALBUM, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_ALBUM_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_ALBUM, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

                    u32Ret = _Demo_GetOption_CharEncConv(eStrFormt, TRUE);
                }
                break;

            case MPLAYER_INFO_MUSIC_ALBUM:
                u32Ret = (MS_U32)_gpu16UnicdBuffer;
                break;

            case MPLAYER_INFO_MUSIC_TITLE_LEN:
                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO_LENGTH,
                    E_MMSDK_INFO_MUSIC_TITLE, sizeof(MS_U32), (void*)&_gu32MetalLen, _gItem[eMediaType]);

                MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                    E_MMSDK_INFO_MUSIC_TITLE_FORMAT, 4, (void*)&eStrFormt, _gItem[eMediaType]);

                if (eStrFormt == E_PLAYER_STRING_FORMAT_UTF8)
                    u32UnicdBufferSize = 2UL * 2 * _gu32MetalLen;
                else
                    u32UnicdBufferSize = 2 * _gu32MetalLen;

                if(!_Demo_GetOption_AllocateMemory(u32UnicdBufferSize))
                    return 0;

                if (_gpu8Buffer)
                {
                    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MUSIC_INFO,
                        E_MMSDK_INFO_MUSIC_TITLE, _gu32MetalLen, (void*)_gpu8Buffer, _gItem[eMediaType]);

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

static MS_BOOL _Demo_MPlayer_SetOption(const EN_MMSDK_MEDIA_TYPE eMediaType, const EN_MPLAYER_OPTION_TYPE eOption, const unsigned int u32Param)
{
    MS_U8 u8R = 0, u8G = 0, u8B = 0;
    MS_BOOL bRet = FALSE;
    MS_U32 u32TotalTrackCnt = 0;
    MS_BOOL bPlayerStatus = TRUE;
    MS_U32 u32TrackNum = u32Param;
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
        mm_print("-------> _gu32TotalTime(%ld)\n", _gu32TotalTime);
        return TRUE;
    }

    if (eOption == MPLAYER_OPTION_SET_STARTTIME) //avoid recursion lock(both playfile and set_option called)
    {
        _gu32StartTime = (MS_U32)u32Param;
        mm_print("-------> _gu32StartTime(%ld)\n", _gu32StartTime);
        return TRUE;
    }

    if (eOption == MPLAYER_OPTION_ENABLE_NEW_AUDIO_MODE)
    {
        _gbUseNewAudioMode = (MS_BOOL)u32Param;
        mm_print("-------> _gbUseNewAudioMode(%d)\n", _gbUseNewAudioMode);
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
            if ((MS_BOOL)u32Param == TRUE)
                bRet = MMSDK_SetSlideShowEffect(E_MMSDK_SLIDE_SHOW_EFFECT_RANDOM);
            break;

        case MPLAYER_OPTION_DISABLE_SLIDE_SHOW:
            if ((MS_BOOL)u32Param == TRUE)
                bRet = MMSDK_SetSlideShowEffect(E_MMSDK_SLIDE_SHOW_EFFECT_NORMAL);
            break;

        case MPLAYER_OPTION_STOP_PHOTO_DISPLAY:
            bRet = MMSDK_ClearPhotoDisplay(u8R, u8G, u8B);
            break;

        case MPLAYER_OPTION_REPEATAB:
            bRet = MMSDK_SetRepeatAB(_gItem[eMediaType], (EN_MMSDK_REPEAT_AB)u32Param);
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
            bRet = MMSDK_SetZoomFactor(_gItem[eMediaType], u32Param);
            if(bRet)
                _geZoomFctor = u32Param;
            break;

        case MPLAYER_OPTION_ROTATION:
            bRet = MMSDK_SetRotateAngle(_gItem[eMediaType], u32Param);
            if(bRet)
                _geRotaAng = u32Param;
            break;

        case MPLAYER_OPTION_MOVE_VIEW:
            bRet = MMSDK_MoveViewWindow(
                                _gItem[eMediaType],
                                (EN_MMSDK_VIEW_MOVE_DIRECTION)pMoveView_Param->eDirection,
                                pMoveView_Param->u16Offset);
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
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                ST_MMSDK_TS_INFO stMMSDK_TSInfo = {};
                bRet = MMSDK_SetProgramID(_gItem[eMediaType], u32TrackNum, &stMMSDK_TSInfo);
            }
            break;

        case MPLAYER_OPTION_CHANGE_AUDIO:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_AUDIO_TRACK;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                bRet = MMSDK_SetAudioTrackID(_gItem[eMediaType], u32TrackNum);
            }
            break;

        case MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE:
            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
            {
                stParam.u32SubOption = E_MMSDK_INFO_MOVIE_NB_SUBTITLE_TRACK;
                bPlayerStatus = MMSDK_GetOptionByPara(_gItem[eMediaType], &stParam, 1);
                if ((bPlayerStatus == FALSE) || (u32TrackNum >= u32TotalTrackCnt))
                {
                    goto FUNC_END;
                }

                bRet = MMSDK_SetSubtitleTrackID(_gItem[eMediaType], u32TrackNum);
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
            sDispWindow.s32X = pDisplayWindow->u32DispX;
            sDispWindow.s32Y = pDisplayWindow->u32DispY;
            sDispWindow.u16Width = pDisplayWindow->u32DispW;
            sDispWindow.u16Height = pDisplayWindow->u32DispH;
            mm_print("--------DISP_WINDOW[%ld][%ld][%d][%d]\n", sDispWindow.s32X, sDispWindow.s32Y, sDispWindow.u16Width, sDispWindow.u16Height);

            if (eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
                bRet = _Demo_MM_SetDisplayRectangle(_gItem[eMediaType], &sDispWindow, TRUE, TRUE);
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
            printf("Error!! MsOS_GetTaskStatus() fail!! \n");
            break;
        }

        if(eTaskStatus == E_TASK_NOT_EXIST)
            break;
        else
        {
            mm_print("\33[32m[FUNC %s] [LINE %d] Wait for Task#%x %s termination \33[m \n",
                __FUNCTION__,__LINE__,stTaskInfo.iId,stTaskInfo.szName);
            MsOS_DelayTask(1);
        }
    }
}
#endif

void _Demo_MPlayer_Finalize(void)
{
    _gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] = TRUE;
    _gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] = TRUE;
    _gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] = TRUE;
    MS_U32  pu32RetrievedEventFlag;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    //Photo
    if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].iId != -1)
    {
        MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);

        MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)
        _Demo_PollingThreadStatus(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO]);
#endif
        _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].iId = -1;

        if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].pStack)
        {
            MsOS_FreeMemory(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].pStack, s32MstarNonCachedPoolID);
            _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_PHOTO].pStack = NULL;
        }
    }

    //Movie
    if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].iId != -1)
    {
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);

        MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)
        _Demo_PollingThreadStatus(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE]);
#endif
        _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].iId = -1;

        if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].pStack)
        {
            MsOS_FreeMemory(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].pStack, s32MstarNonCachedPoolID);
            _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MOVIE].pStack = NULL;
        }
    }

    //Music
    if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].iId != -1)
    {
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_NOTIFY_NULL);

        MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)
        _Demo_PollingThreadStatus(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC]);
#endif
        _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].iId = -1;

        if (_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].pStack)
        {
            MsOS_FreeMemory(_gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].pStack, s32MstarNonCachedPoolID);
            _gThreadEvtProcess[E_MMSDK_MEDIA_TYPE_MUSIC].pStack = NULL;
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

    MsOS_DeleteMutex(_gs32MutexOpenStop);
    if (!_Demo_MM_Finalize())
    {
        mm_print("-----> MM_Finalize ERROR ERROR!!!!\n");
    }
}

MS_BOOL _Demo_MPlayer_Initialize(void)
{
    const MS_U8* pMutexName1 = (MS_U8*)"_gs32MutexOpenStop";
    const MS_U8* pMutexName2 = (MS_U8*)"VarMutex";
    const MS_U8* sEvtGrpName = (MS_U8*)"MMEvnGrp";
    MS_U8 i = 0;

    _gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
    _gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] = FALSE;
    _gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
    _geStrmMode[E_MMSDK_MEDIA_TYPE_PHOTO] = MPLAYER_FILE_MODE;
    _geStrmMode[E_MMSDK_MEDIA_TYPE_MUSIC] = MPLAYER_FILE_MODE;
    _geStrmMode[E_MMSDK_MEDIA_TYPE_MOVIE] = MPLAYER_FILE_MODE;

    _gu32VdplayerReadTimerOut = 500;
    _gbFastInit = FALSE;
    _gu32InitDataSize = 0x20000;

    _gs32MinBufferSec = 1;
    _gs32MaxBufferSec = 5;

    for(i = 0; i < E_MPLAYER_MEDIA_STREAM_NUM; i++)
    {
        _gpStream[i] = NULL;
    }

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

    _gs32MediaEvtGrp = MsOS_CreateEventGroup((char*)sEvtGrpName);
    if(_gs32MediaEvtGrp == -1)
    {
        goto FUNC_END;
    }

    if (!_Demo_MM_Initialize())
    {
        mm_print("-----> MM_Initialize ERROR ERROR!!!!\n");
    }

    for (i = 0; i < MMSDK_MEDIA_TYPE_NUM; i++)
    {
        //_gu8PreviewFlag[i] = 0;
        _geTrickMod[i] = E_MMSDK_TRICK_NORMAL;
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

    //E_MMSDK_BUF_PHOTO_DISPLAY_MPO
    //currenty doesn't support 3D display .mpo file, no need for this buffer
    //MM_FILL_MEM_INFO(stMmapInfo.stPhotoMPODispBufAddr, PHOTO_MPO_DISPLAY_ADR, PHOTO_MPO_DISPLAY_LEN, PHOTO_MPO_DISPLAY_MEMORY_TYPE);

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
        printf("ERROR!!! No enough free mem for E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO!\n");

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
    MS_U8 u8Volume = DEFAULT_VOLUME;
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

    //For image type subtitle
    msAPI_MM_Scaler_ForceOpen();

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);

    //audio de-mute and initial volume setting , move from mm mdw
    bMute = FALSE;
    Demo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    Demo_Audio_SetAbsoluteVolume(&u8Volume);

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
    if(_gItem[_geMediaType] == 0 || pGotoTimeMs == NULL)
    {
        return FALSE;
    }

    MS_U32 u32Value = 0;
    if(_geMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)//for mantis 0242114
    {
        u32Value = _Demo_MPlayer_GetOption(_geMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_TOTAL_TIME);
        if((u32Value > 0) && ((*pGotoTimeMs) > u32Value))
        {
            printf("Goto time exceeds file length (=%d ms)\n",(int)u32Value);
            return FALSE;
        }
    }

    if(_geMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        u32Value = _Demo_MPlayer_GetOption(_geMediaType, MPLAYER_OPTION_MUSIC_INFO, MPLAYER_INFO_TOTAL_TIME);
        if((u32Value > 0) && ((*pGotoTimeMs) > u32Value * 1000))
        {
            printf("Goto time exceeds file length (=%d s)\n", (int)u32Value);
            return FALSE;
        }
    }

    Demo_MM_Resume();

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(MMSDK_SetPlayPosition(_gItem[_geMediaType], *pGotoTimeMs) != TRUE)
    {
        mm_print(" fail to set position.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    return TRUE;
}

static MS_BOOL _Demo_IsCapEnoughForFFx2(void)
{
    ST_MMSDK_PARAM stParam = {};
    MS_U32 u32Codec = 0;
    MS_U32 u32FrameRate = 0;
    MS_U32 u32Width = 0;
    MS_U32 u32Height = 0;
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_MOVIE_VIDEO_CODEC, sizeof(MS_U32), (void*)&u32Codec, _gItem[E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_MOVIE_FRAME_PER_SEC, sizeof(MS_U32), (void*)&u32FrameRate, _gItem[E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_HRES, sizeof(MS_U32), (void*)&u32Width, _gItem[E_MMSDK_MEDIA_TYPE_MOVIE]);
    MM_GET_OPTION(stParam, E_MMSDK_OPTION_MOVIE_INFO,
        E_MMSDK_INFO_VRES, sizeof(MS_U32), (void*)&u32Height, _gItem[E_MMSDK_MEDIA_TYPE_MOVIE]);

    printf("E_MMSDK_INFO_MOVIE_FRAME_PER_SEC = %d\n", (int)u32FrameRate);

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

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gItem[_geMediaType] == 0)
    {
        mm_print(" invalid MediaItem.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    switch(_geTrickMod[_geMediaType])
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

    if(eTrickMode == E_MMSDK_TRICK_FF_2X)
        if(_Demo_IsCapEnoughForFFx2() == FALSE)
        {
            printf("Do E_MMSDK_TRICK_FFS_2X instead of E_MMSDK_TRICK_FF_2X due to capability is not enough!!\n");
            eTrickMode = E_MMSDK_TRICK_FFS_2X;
        }

    if(MMSDK_SetTrickMode(_gItem[_geMediaType], eTrickMode) == FALSE)
    {
        mm_print(" fail to set trick mode.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    _geTrickMod[_geMediaType] = eTrickMode;

    printf("%s\n", _Demo_GetTrickModeString(eTrickMode));
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
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

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gItem[_geMediaType] == 0)
    {
        mm_print(" invalid MediaItem.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    switch(_geTrickMod[_geMediaType])
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
            printf("Do E_MMSDK_TRICK_FFS_2X instead of E_MMSDK_TRICK_FF_2X due to capability is not enough!!\n");
            eTrickMode = E_MMSDK_TRICK_FFS_2X;
        }

    if(MMSDK_SetTrickMode(_gItem[_geMediaType], eTrickMode) == FALSE)
    {
        mm_print(" fail to set trick mode.\n");
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        return FALSE;
    }

    _geTrickMod[_geMediaType] = eTrickMode;

    printf("%s\n", _Demo_GetTrickModeString(eTrickMode));
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
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
    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gbPlay[_geMediaType] == TRUE)
    {
        if(MMSDK_SetTrickMode(_gItem[_geMediaType], E_MMSDK_TRICK_PAUSE) == FALSE)
        {
            mm_print(" fail to pause media.\n");
            _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
            return FALSE;
        }
    }

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

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    _geTrickMod[_geMediaType] = E_MMSDK_TRICK_NORMAL;

    if(_gbPlay[_geMediaType] == TRUE)
    {
        if(MMSDK_SetTrickMode(_gItem[_geMediaType], E_MMSDK_TRICK_NORMAL) == TRUE)
        {
            bRet = TRUE;
        }
    }

    if(bRet == FALSE)
    {
        mm_print(" fail to resume media.\n");
    }

    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    return bRet;
}

static void* _Demo_FileOpen(const char* const pathname,const char* const mode)//open stream, return stream handle
{//implemented by AP
    mm_print("--Into %s --pathname %s --\n", __FUNCTION__, pathname);
    FILE* fp = MsFS_Fopen(pathname, "rb");
    if (NULL==fp)
    {
        printf("ERROR!!! fopen fail, errno=%d\n", errno);
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

BOOLEAN _Demo_MPlayer_Stop(const EN_MMSDK_MEDIA_TYPE eMediaType)
{
    mm_print("--------> enter _Demo_MPlayer_Stop---------------\n ");
    BOOLEAN bRet = FALSE;
    MEDIAITEM u32ItemTmp = NULL;

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);

    _geTrickMod[eMediaType] = E_MMSDK_TRICK_NORMAL;
    _gu64CurPostn[eMediaType] = 0;
    _gu64SubCurPostn = 0;

    _gbPlay[eMediaType] = FALSE;

    if(MMSDK_Stop(_gItem[eMediaType]) != TRUE)
    {
        mm_print("-----> MM_Stop fail \n");
    }

    if(_gItem[eMediaType] != NULL)
    {
        u32ItemTmp = _gItem[eMediaType];
        _gItem[eMediaType] = NULL;
        MMSDK_DeleteMediaItem(u32ItemTmp);
    }

    if(eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        _geAudioType = MPLAYER_SUBTYPE_UNKNOWN;
        _gu32SaveFilePos = 0;

#if(DEMO_AUDIO_MULTI_TEST == 1)
        Demo_MM_FreeAudDec();
#endif
    }
    else if(eMediaType == E_MMSDK_MEDIA_TYPE_PHOTO)
    {
        _geRotaAng = E_MMSDK_ROTATE_0;
        _geZoomFctor = E_MMSDK_VIEW_ZOOM_1X;
    }
    else
    {
        _gu32TotalTime = 0;
        _gu32StartTime = 0;

#if(DEMO_AUDIO_MULTI_TEST == 1)
        Demo_MM_FreeAudDec();
#endif
    }
    _gstMedia.pCallback = NULL;

    if(_gpStream[eMediaType] != NULL)
    {
        _Demo_FileClose(_gpStream[eMediaType]);
        _gpStream[eMediaType] = NULL;
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])
        {
            _Demo_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = NULL;
        }
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO])
        {
            _Demo_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = NULL;
        }
        if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2])
        {
            _Demo_FileClose(_gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2]);
            _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = NULL;
        }
    }

    MMSDK_SetBackwardRePlayMode(_gbBackwardRePlayMode);
    //_gbSeekRightNow[eMediaType] = FALSE;
    _geStrmMode[eMediaType] = MPLAYER_FILE_MODE;
    bRet = TRUE;
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);

    mm_print("--------> leave _Demo_MPlayer_Stop---------------\n ");
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
    if(_Demo_MPlayer_Stop(_geMediaType) == TRUE)
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
    if(_Demo_MPlayer_Stop((EN_MMSDK_MEDIA_TYPE)*pMediaType) == TRUE)
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
    if((*pu32On != 0) && (*pu32On != 1))
        return FALSE;

    if(_gpu8FileName && ((_gu32FileOption == MPLAYER_MODE_INVALID) || (_gu32FileOption == MPLAYER_FILE_MODE)))
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

    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    if(_gbPlay[_geMediaType] == TRUE)
    {
        if(MMSDK_SetLoop(_gItem[_geMediaType], _gu8IsRepeat) == FALSE)
        {
            mm_print(" fail to set loop.\n");
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
    MS_U32 u32Value;
    u32Value = _Demo_MPlayer_GetOption(_geMediaType, (EN_MPLAYER_OPTION_TYPE)*pOption, (unsigned int)*pMMInfo);
    printf("GetOption Result = %"DTC_MS_U32_d"\n",u32Value);

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

static int _Demo_Ringbuf_Seek(MS_U32 u32Hdl, MS_U64 u64SeekPos, int fromwhere)//seek to postition
{
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = (EN_MPLAYER_MEDIA_STREAM_TYPE)u32Hdl;
    EN_MMSDK_MEDIA_TYPE eMediaType = _Demo_MapMediaStreamType2MediaType(eStreamType);
    MS_U32 u32Ret = 0; //be cautious , 0 indicates OK; 1 means failure!!!!!
    //_gbSeekRightNow[eMediaType] = TRUE;
    mm_print("-----> Enter _Demo_Ringbuf_Seek %d\n",(int)u64SeekPos);

    if(_geStrmMode[eMediaType] == MPLAYER_FILE_MODE)
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
            //printf("Warning!!!!!!, sizeof(off_t) is not equal to sizeof(unsinged long long)\n");
        }

        void* pStream = NULL;
        pStream = _gpStream[eStreamType];

        seekOff =  _Demo_FileSeek(pStream, offset, SEEK_SET);
        u32Ret = (seekOff >= 0) ? 0 : 1;
    }

    return u32Ret;
}

static int _Demo_Ringbuf_Read(void *pBuffAddr, int size, MS_U32 u32Length , MS_BOOL bBlockRead, MS_U32 u32Hdl)
{
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = (EN_MPLAYER_MEDIA_STREAM_TYPE)u32Hdl;
    EN_MMSDK_MEDIA_TYPE eMediaType = _Demo_MapMediaStreamType2MediaType(eStreamType);
    mm_print("-----> Enter _Demo_Ringbuf_Read, %d\n", (int)u32Length);
    //printf("u32Hdl=%ld \n", u32Hdl);
    //printf("bReadDataFromRingbuf [%d] \n", __LINE__);

    if (_geStrmMode[eMediaType] == MPLAYER_FILE_MODE)
    {
        void* pStream = NULL;
        pStream = _gpStream[eStreamType];

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

static EN_MPLAYER_MEDIA_STREAM_TYPE _Demo_MapFileID2MediaStreamType(void* pFileHandle)
{
    MS_S32 i;
    for(i = MMSDK_MEDIA_TYPE_MIN; i < MMSDK_MEDIA_TYPE_NUM; i++)
    {
        if(pFileHandle == _gpStream[i])
        {
            //printf("_Demo_MapFileID2MediaStreamType=%ld \n", i);
            return (EN_MPLAYER_MEDIA_STREAM_TYPE)i;
        }
    }
    if(pFileHandle == _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO])
    {
        //printf("_Demo_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_AUDIO);
        return E_MPLAYER_MEDIA_STREAM_AUDIO;
    }
    if(pFileHandle == _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2])
    {
        //printf("_Demo_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_AUDIO2);
        return E_MPLAYER_MEDIA_STREAM_AUDIO2;
    }
    if(pFileHandle == _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])
    {
        //printf("_Demo_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_SUBTITLE);
        return E_MPLAYER_MEDIA_STREAM_SUBTITLE;
    }
    //printf("_Demo_MapFileID2MediaStreamType=%ld \n",(MS_U32)E_MPLAYER_MEDIA_STREAM_INVALID);
    return E_MPLAYER_MEDIA_STREAM_INVALID;
}

MS_BOOL _Demo_MM_Callback_Photo(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;

    MS_BOOL bRet = FALSE;
    mm_print("eEvent is  %d\n", (int)eEvent);
    switch(eEvent)
    {
        /// The notify event is file read request.
        case E_MMSDK_CALLBACK_FILE_READ:
            mm_print("Get E_MMSDK_CALLBACK_FILE_READ event. need %ld bytes data\n", pFileEvent->u32FileRequestSize);
            pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8),  pFileEvent->u32FileRequestSize, !_gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO], (MS_U32)E_MMSDK_MEDIA_TYPE_PHOTO);
            _gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO] = pstFileAck->u32ReadSize;
            if (_gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO] >= _gu64Size[E_MMSDK_MEDIA_TYPE_PHOTO])
            {
                pstFileAck->bIsFileEnd = TRUE;
            }

            bRet = TRUE;
            break;

        /// The notify event is file seek request.
        case E_MMSDK_CALLBACK_FILE_SEEK:
            mm_print("Get E_EVENT_FILE_SEEK event.\n");
            if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[E_MMSDK_MEDIA_TYPE_PHOTO])
            {
                if(_Demo_Ringbuf_Seek(E_MMSDK_MEDIA_TYPE_PHOTO, pFileEvent->u64FileSeekPos, SEEK_SET) == 0)
                {
                    _gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO] = pFileEvent->u64FileSeekPos;
                    bRet = TRUE;
                }
                else
                {
                    mm_print("File seek fail.\n");
                }
            }
            break;

            /// The notify event is file tell request.
        case E_MMSDK_CALLBACK_FILE_TELL:
            mm_print("Get E_MMSDK_CALLBACK_FILE_TELL event.\n");
            pstFileAck->u64FilePosition = _gu64CurPostn[E_MMSDK_MEDIA_TYPE_PHOTO];
            bRet = TRUE;
            break;

            /// The notify event is file init OK.
        case E_MMSDK_CALLBACK_INIT_OK:
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_START_PLAY);
            mm_print("E_MMSDK_CALLBACK_INIT_OK: Send MPLAYER_START_PLAY to Photo_EvtProcess.\n");
            break;

            /// The notify event is file init fail.
        case E_MMSDK_CALLBACK_INIT_FAIL:
            mm_print("Get E_MMSDK_CALLBACK_INIT_FAIL event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;

            /// The notify event is invalid file.
        case E_MMSDK_CALLBACK_INVALID_FILE:
            mm_print("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
            break;

            /// The notify event is thumbnail decoding done.
        case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
            mm_print("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
            break;

            /// The notify event is thumbnail decoding error.
        case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
            mm_print("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
            break;

            /// The notify event is decoding error.
        case E_MMSDK_CALLBACK_DECODE_ERROR:
            mm_print("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_FAILED);
            break;

            /// The notify event is decoding done.
        case E_MMSDK_CALLBACK_DECODE_DONE:
            mm_print("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
            MM_SET_PHOTO_NOTIFY_TYPE(MPLAYER_PHOTO_DECODE_DONE);
            break;

            /// The notify event is video first frame decoding done.
        case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
            mm_print("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
            break;

        default:
            break;
    }

    return bRet;
}

static MS_BOOL _Demo_MM_Callback_Music(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
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
        mm_print("-------> E_MMSDK_CALLBACK_FILE_READ Needs %d\n", (int)pFileEvent->u32FileRequestSize);
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
                pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff + sizeof(_gstWaveHdr) - _gu32SaveFilePos, sizeof(MS_U8), pFileEvent->u32FileRequestSize - (sizeof(_gstWaveHdr) - _gu32SaveFilePos), !_gbPlay[E_MMSDK_MEDIA_TYPE_MUSIC], (MS_U32)E_MMSDK_MEDIA_TYPE_MUSIC);
                _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] += pstFileAck->u32ReadSize;
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
            pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8), pFileEvent->u32FileRequestSize, !_gbPlay[E_MMSDK_MEDIA_TYPE_MUSIC], (MS_U32)E_MMSDK_MEDIA_TYPE_MUSIC);
            _gu32SaveFilePos += pstFileAck->u32ReadSize;
            _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] +=  pstFileAck->u32ReadSize;
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

                if(_gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] + 44 >= _gu64Size[E_MMSDK_MEDIA_TYPE_MUSIC])
                {
                    pstFileAck->bIsFileEnd = TRUE;
                }
            }

            else
            {
                if(_gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] >= _gu64Size[E_MMSDK_MEDIA_TYPE_MUSIC])
                {
                    pstFileAck->bIsFileEnd = TRUE;
                }
            }
        }
        mm_print("-------> E_MMSDK_CALLBACK_FILE_READ %d\n", (int)pstFileAck->u32ReadSize);
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
                if(_Demo_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, pFileEvent->u64FileSeekPos - sizeof(_gstWaveHdr), SEEK_SET) == 0)
                {
                    _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] = pFileEvent->u64FileSeekPos - sizeof(_gstWaveHdr);
                    bRet = TRUE;
                }
                else
                {
                    printf("File seek fail.\n");
                }
            }
            else  //just within WAV header
            {
                if(_Demo_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, 0, SEEK_SET)==0)
                {
                    _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] = 0;
                    bRet = TRUE;
                }
            }
        }
        else if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[E_MMSDK_MEDIA_TYPE_MUSIC])
        {
            if(_Demo_Ringbuf_Seek(E_MPLAYER_MEDIA_STREAM_MUSIC, pFileEvent->u64FileSeekPos, SEEK_SET) == 0)
            {
                _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC] = pFileEvent->u64FileSeekPos;
                mm_print("------->fseek :%lld successfully-----------\n",pFileEvent->u64FileSeekPos);
                bRet = TRUE;
            }
            else
            {
                printf("File seek fail.\n");
            }
        }

        mm_print("-------seeking %d bytes  needs time is %d ms\n", (int)pFileEvent->u64FileSeekPos,(int)_gu32TotalTime);
        break;

        /// The notify event is file tell request.
    case E_MMSDK_CALLBACK_FILE_TELL:
        memset(pstFileAck, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));
        if(_geAudioType == MPLAYER_SUBTYPE_PCM)  // just PCM audio format
        {
            pstFileAck->u64FilePosition = (_gu32SaveFilePos > (MS_U32)_gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC])?(MS_U64)_gu32SaveFilePos : _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC];
        }
        else
        {
            pstFileAck->u64FilePosition = _gu64CurPostn[E_MMSDK_MEDIA_TYPE_MUSIC];
        }
        bRet = TRUE;
        break;

        /// The notify event is file init OK.
    case E_MMSDK_CALLBACK_INIT_OK:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_START_PLAY);
        mm_print("Send MPLAYER_START_PLAY to Music_EvtProcess.\n");
        break;

        /// The notify event is file init fail.
    case E_MMSDK_CALLBACK_INIT_FAIL:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is invalid file.
    case E_MMSDK_CALLBACK_INVALID_FILE:
        mm_print("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding error.
    case E_MMSDK_CALLBACK_DECODE_ERROR:
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding done.
    case E_MMSDK_CALLBACK_DECODE_DONE:
        mm_print("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
        MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;

        /// The notify event is video first frame decoding done.
    case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
        mm_print("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
        break;

        /// The notify event is media backward to file start and re-start the media playback.
    case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
        if (_gbMusicBackwardRePlayMode)
            _geTrickMod[E_MMSDK_MEDIA_TYPE_MUSIC] = E_MMSDK_TRICK_NORMAL;
        else
            MM_SET_MUSIC_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;

    default:
        break;
    }

    return bRet;
}

static MS_BOOL _Demo_MM_Callback_Video(const MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo)
{
    EN_MMSDK_CALLBACK_MSG eEvent = (EN_MMSDK_CALLBACK_MSG)((MS_U32)u32Msg);
    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    ST_MMSDK_CALLBACK_IO_ACK* pstFileAck = (ST_MMSDK_CALLBACK_IO_ACK*)pAckInfo;
    MS_BOOL bRet = FALSE;
    EN_MPLAYER_MEDIA_STREAM_TYPE eStreamType = E_MPLAYER_MEDIA_STREAM_INVALID;
    MS_U64* u64TempCurPostn = NULL;

    mm_print("eEvent is  %d\n", (int)eEvent);
    switch(eEvent)
    {
        /// The notify event is file read request.
    case E_MMSDK_CALLBACK_FILE_READ:
        eStreamType = _Demo_MapFileID2MediaStreamType(pFileEvent->pFileHandle);
        pstFileAck->u32ReadSize = _Demo_Ringbuf_Read(pFileEvent->pu8FileBuff, sizeof(MS_U8), pFileEvent->u32FileRequestSize, !_gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE], eStreamType);

        if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
            u64TempCurPostn = &_gu64SubCurPostn;
        else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
            u64TempCurPostn = &_gu64AudioCurPostn;
        else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
            u64TempCurPostn = &_gu64Audio2CurPostn;
        else
            u64TempCurPostn = &_gu64CurPostn[(EN_MMSDK_MEDIA_TYPE)eStreamType];

        *u64TempCurPostn += pstFileAck->u32ReadSize;
        if((*u64TempCurPostn >= _gu64Size[E_MMSDK_MEDIA_TYPE_MOVIE]))
        {
            mm_print("-------> E_MMSDK_CALLBACK_FILE_READ fileEnd,needs %lx \n", pFileEvent->u32FileRequestSize);
            pstFileAck->bIsFileEnd = TRUE;
        }

        mm_print("-------> E_MMSDK_CALLBACK_FILE_READ %d\n", (int)pstFileAck->u32ReadSize);
        bRet = TRUE;
        break;

        /// The notify event is file seek request.
    case E_MMSDK_CALLBACK_FILE_SEEK:
        eStreamType = _Demo_MapFileID2MediaStreamType(pFileEvent->pFileHandle);
        mm_print("--- File size: 0x%llx, Seek position: 0x%llx.\n", _gu64Size[E_MMSDK_MEDIA_TYPE_MOVIE], pFileEvent->u64FileSeekPos);
        if(pFileEvent->u64FileSeekPos <= (MS_U64)_gu64Size[E_MMSDK_MEDIA_TYPE_MOVIE])
        {
            int SeekResult = 1;//be cautious , 0 indicates OK; 1 means failure!!!!!
            SeekResult = _Demo_Ringbuf_Seek(eStreamType, pFileEvent->u64FileSeekPos, SEEK_SET);
            if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
                u64TempCurPostn = &_gu64SubCurPostn;
            else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
                u64TempCurPostn = &_gu64AudioCurPostn;
            else if (eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
                u64TempCurPostn = &_gu64Audio2CurPostn;
            else
                u64TempCurPostn = &_gu64CurPostn[(EN_MMSDK_MEDIA_TYPE)eStreamType];

            if(SeekResult == 0)
            {
                *u64TempCurPostn = pFileEvent->u64FileSeekPos;
                bRet = TRUE;
            }
            else
            {
                printf("File seek fail.\n");
            }
        }
        break;

        /// The notify event is file tell request.
    case E_MMSDK_CALLBACK_FILE_TELL:
        mm_print("Get E_MMSDK_CALLBACK_FILE_TELL event.\n");
        eStreamType = _Demo_MapFileID2MediaStreamType(pFileEvent->pFileHandle);
        if(eStreamType == E_MPLAYER_MEDIA_STREAM_SUBTITLE)
            pstFileAck->u64FilePosition = _gu64SubCurPostn;
        else if(eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO)
            pstFileAck->u64FilePosition = _gu64AudioCurPostn;
        else if(eStreamType == E_MPLAYER_MEDIA_STREAM_AUDIO2)
            pstFileAck->u64FilePosition = _gu64Audio2CurPostn;
        else
            pstFileAck->u64FilePosition = _gu64CurPostn[(EN_MMSDK_MEDIA_TYPE)eStreamType];

        bRet = TRUE;
        break;

        /// The notify event is file init OK.
    case E_MMSDK_CALLBACK_INIT_OK:
        mm_print("Movie Init ok!\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_START_PLAY);
        break;

        /// The notify event is file init fail.
    case E_MMSDK_CALLBACK_INIT_FAIL:
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is invalid file.
    case E_MMSDK_CALLBACK_INVALID_FILE:
        mm_print("Get E_MMSDK_CALLBACK_INVALID_FILE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is thumbnail decoding done.
    case E_MMSDK_CALLBACK_THUMBNAIL_DONE:
        mm_print("Get E_MMSDK_CALLBACK_THUMBNAIL_DONE event.\n");
        break;

        /// The notify event is thumbnail decoding error.
    case E_MMSDK_CALLBACK_THUMBNAIL_ERROR:
        mm_print("Get E_MMSDK_CALLBACK_THUMBNAIL_ERROR event.\n");
        break;

        /// The notify event is decoding error.
    case E_MMSDK_CALLBACK_DECODE_ERROR:
        mm_print("Get E_MMSDK_CALLBACK_DECODE_ERROR event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_ERROR_FILE);
        break;

        /// The notify event is decoding done.
    case E_MMSDK_CALLBACK_DECODE_DONE:
        mm_print("Get E_MMSDK_CALLBACK_DECODE_DONE event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_EXIT_OK);
        break;

        /// The notify event is video first frame decoding done.
    case E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE:
        mm_print("Get E_MMSDK_CALLBACK_DECODE_FIRST_FRAME_DONE event.\n");
        break;

        /// The notify event is media backward to file start and re-start the media playback.
    case E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START:
        mm_print("Get E_MMSDK_CALLBACK_DECODE_BACKWARD_TO_FILE_START event.\n");
        _geTrickMod[E_MMSDK_MEDIA_TYPE_MOVIE] = E_MMSDK_TRICK_NORMAL;
        break;

        ///  The notify event is program been changed.
    case E_MMSDK_CALLBACK_PROGRAM_CHANGED:
        mm_print("Get E_MMSDK_CALLBACK_PROGRAM_CHANGED event.\n");
        break;

        ///  The notify event is audio track been changed.
    case E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED:
        mm_print("Get E_MMSDK_CALLBACK_AUDIO_TRACK_CHANGED event.\n");
        break;

    case E_MMSDK_CALLBACK_PLAYING_OK:
        mm_print("Get E_MMSDK_CALLBACK_PLAYING_OK event.\n");
        MM_SET_VIDEO_NOTIFY_TYPE(MPLAYER_PLAY_OK);
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
    if (pMsgInfo == NULL)
    {
        printf("No event attached data.\n");
        goto FUN_END;
    }

    ST_MMSDK_CALLBACK_IO_INFO* pFileEvent = (ST_MMSDK_CALLBACK_IO_INFO*)pMsgInfo;
    if(pAckInfo)
        memset(pAckInfo, 0, sizeof(ST_MMSDK_CALLBACK_IO_ACK));

    MS_U32 i;
    for(i = MMSDK_MEDIA_TYPE_MIN; i < MMSDK_MEDIA_TYPE_NUM; i++)
        if(pFileEvent->Item == _gItem[i])
            switch((EN_MMSDK_MEDIA_TYPE)i)
            {
                case E_MMSDK_MEDIA_TYPE_PHOTO:
                    bRet = _Demo_MM_Callback_Photo(u32Msg, pMsgInfo, pAckInfo);
                    break;
                case E_MMSDK_MEDIA_TYPE_MUSIC:
                    bRet = _Demo_MM_Callback_Music(u32Msg, pMsgInfo, pAckInfo);
                    break;
                case E_MMSDK_MEDIA_TYPE_MOVIE:
                    bRet = _Demo_MM_Callback_Video(u32Msg, pMsgInfo, pAckInfo);
                    break;
            }

FUN_END:
    return bRet;
}

static void _Demo_Photo_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE ePhotoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;

    while (_gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] != TRUE)
    {
        while((_gePhotoNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[E_MMSDK_MEDIA_TYPE_PHOTO] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        ePhotoNotifyTypeTmp = _gePhotoNotifyType;
        _gePhotoNotifyType = MPLAYER_NOTIFY_NULL;

        mm_print("in Photo_EvtProcess thread\n");
        if (ePhotoNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            mm_print("Receive signal=%d from MM_Callback_Photo .\n", (int)ePhotoNotifyTypeTmp);
            if (ePhotoNotifyTypeTmp ==  MPLAYER_START_PLAY)
            {
                _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_STOP_PHOTO_DISPLAY, 0);

                _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
                _gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO] = TRUE;
                if (_gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height + _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width + _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16X + _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Y != 0) //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16X;
                    sDispWindow.s32Y = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Y;
                    sDispWindow.u16Width = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Width;
                    sDispWindow.u16Height = _gstRect[E_MMSDK_MEDIA_TYPE_PHOTO].u16Height;
                    mm_print("--------DISP_WINDOW[%ld][%ld][%d][%d]\n", sDispWindow.s32X, sDispWindow.s32Y, sDispWindow.u16Width, sDispWindow.u16Height);
                    if (_Demo_MM_SetDisplayRectangle(_gItem[E_MMSDK_MEDIA_TYPE_PHOTO], &sDispWindow, TRUE, TRUE))
                    {
                        _gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
                    }
                }
                if (MMSDK_Play(_gItem[E_MMSDK_MEDIA_TYPE_PHOTO]) != TRUE)
                {
                    _gbPlay[E_MMSDK_MEDIA_TYPE_PHOTO] = FALSE;
                }
                _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
        }
            else if (ePhotoNotifyTypeTmp == MPLAYER_PHOTO_DECODE_DONE)
            {
                printf("---------------->Decode Successfully!\n");
                mm_print("---------------------after  process MPLAYER_PHOTO_DISPLAYED .\n");
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_PHOTO_DECODE_FAILED)
            {
                printf("---------------->Decode Error! \n");
                mm_print("---------------------after  process MPLAYER_PHOTO_DECODE_FAILED .\n");
            }
            else if (ePhotoNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                printf("---------------->Unsupported File! \n");
                mm_print("---------------------after  process MPLAYER_PHOTO_ERROR_FILE .\n");
            }

            ePhotoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
    }
    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_PHOTO_PROC_EXIT);
}

static void _Demo_Music_EvtProcess(MS_U32 argc, void *arg)
{
    EN_MPLAYER_NOTIFY_TYPE eMusicNotifyTypeTmp = MPLAYER_NOTIFY_NULL;

    while(_gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] != TRUE)
    {
        while(_geMusicNotifyType == MPLAYER_NOTIFY_NULL && _gbExit[E_MMSDK_MEDIA_TYPE_MUSIC] != TRUE)
        {
            MS_U32  pu32RetrievedEventFlag;
            //MsOS_WaitEvent(condMusicNotifyTypeValid, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_MUSIC_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eMusicNotifyTypeTmp = _geMusicNotifyType;
        _geMusicNotifyType = MPLAYER_NOTIFY_NULL;

        _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
        mm_print("----- in Music_EvtProcess thread\n");

        if((eMusicNotifyTypeTmp != MPLAYER_NOTIFY_NULL) && (_gItem[E_MMSDK_MEDIA_TYPE_MUSIC] != 0))
        {
            mm_print("Receive signal=%d from MM_Callback_Music .\n", (int)eMusicNotifyTypeTmp);
            if(eMusicNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
                printf("Music exit ok!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MUSIC);
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_INSUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MUSIC],  E_MMSDK_TRICK_PAUSE) == FALSE)
                {
                    printf("-----> MM_SetTrickMode fail 1\n");
                }
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_SUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MUSIC],  E_MMSDK_TRICK_NORMAL) == FALSE)
                {
                    printf("-----> MM_SetTrickMode fail 2\n");
                }
            }
            else if(eMusicNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                _gbPlay[E_MMSDK_MEDIA_TYPE_MUSIC] = MMSDK_Play(_gItem[E_MMSDK_MEDIA_TYPE_MUSIC]);
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
    while(_gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] != TRUE)
    {
        while((_geVideoNotifyType == MPLAYER_NOTIFY_NULL) && (_gbExit[E_MMSDK_MEDIA_TYPE_MOVIE] != TRUE))
        {
            MS_U32  pu32RetrievedEventFlag;
            //MsOS_WaitEvent(condVideoNotifyTypeValid, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            MsOS_WaitEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_NOTIFY, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        }
        eVideoNotifyTypeTmp = _geVideoNotifyType;
        _geVideoNotifyType = MPLAYER_NOTIFY_NULL;

        _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
        mm_print("----- in Video_EvtProcess thread\n");
        if(eVideoNotifyTypeTmp != MPLAYER_NOTIFY_NULL)
        {
            if(eVideoNotifyTypeTmp == MPLAYER_FRAME_READY)
            {
                mm_print("==================Movie frame ready!\n");
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_EXIT_OK)
            {
                printf("Movie exit ok!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MOVIE);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_EXIT_ERROR_FILE)
            {
                printf("Movie unsupport file!\n");
                _Demo_MPlayer_Stop(E_MMSDK_MEDIA_TYPE_MOVIE);
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_BLACKSCREEN_OFF)
            {
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_INSUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], E_MMSDK_TRICK_PAUSE) == FALSE)
                {
                    printf("-----> MM_SetTrickMode fail 2\n");
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_SUFFICIENT_DATA)
            {
                if(MMSDK_SetTrickMode(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], E_MMSDK_TRICK_NORMAL) == FALSE)
                {
                    printf("-----> MM_SetTrickMode fail 2\n");
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_START_PLAY)
            {
                if(_gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Height + _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Width + _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16X + _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Y != 0)  //should set display win
                {
                    ST_RECT_SIGNED_POSITION sDispWindow;

                    sDispWindow.s32X = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16X;
                    sDispWindow.s32Y = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Y;
                    sDispWindow.u16Width = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Width;
                    sDispWindow.u16Height = _gstRect[E_MMSDK_MEDIA_TYPE_MOVIE].u16Height;
                    if(_Demo_MM_SetDisplayRectangle(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], &sDispWindow, TRUE, TRUE))
                    {
                        _gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
                    }
                }

                if(MMSDK_Play(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE]) != TRUE)
                {
                    _gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE] = FALSE;
                }
                else
                {
                    _gbPlay[E_MMSDK_MEDIA_TYPE_MOVIE] = TRUE;
                }
            }
            else if(eVideoNotifyTypeTmp == MPLAYER_PLAY_OK)
            {
                if(MMSDK_SetSubtitleOnOff(_gItem[E_MMSDK_MEDIA_TYPE_MOVIE], TRUE) == FALSE)
                {
                    mm_print(" fail to set subtitle on.\n");
                }
            }

            eVideoNotifyTypeTmp = MPLAYER_NOTIFY_NULL;
        }
        _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
    }

    MsOS_SetEvent(_gs32MediaEvtGrp, MM_EVENT_VIDEO_PROC_EXIT);
}

MS_BOOL _Demo_MPlayer_PlayFile(const EN_MMSDK_MEDIA_TYPE eMediaType, MPLAYER_MEDIA* const pMplayerMedia)
{
    mm_print("------> Enter func MApi_MPlayer_PlayFile \n");
    _Demo_ObtainRecursiveMutex(_gs32MutexOpenStop);
    BOOLEAN bRet = FALSE;
    const MS_U32 handle = eMediaType;
    ST_MMSDK_PARAM astExtenInfoParam;
    MMSDK_U32 u32ExtenInfoParamNum = 0;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    memset(&_gstMedia, 0, sizeof(ST_MMSDK_CREATE_MEDIA_INFO));
    _gstMedia.u32Version = MMSDK_VERSION;

    _gbExit[eMediaType] = FALSE;
    _geStrmMode[handle]     =  pMplayerMedia->eFileMode;

    _gstRect[eMediaType].u16Height = pMplayerMedia->u32DispH;
    _gstRect[eMediaType].u16Width = pMplayerMedia->u32DispW;
    _gstRect[eMediaType].u16X = pMplayerMedia->u32DispX;
    _gstRect[eMediaType].u16Y = pMplayerMedia->u32DispY;

    if(pMplayerMedia->filename != NULL)
    {
        _gpStream[eMediaType] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        if(_gpStream[eMediaType] != NULL)
            _gu64Size[eMediaType] = _Demo_FileLength(_gpStream[eMediaType]);
        else
        {
            mm_print("open stream failed!\n");
            goto FUNC_END;
        }
    }
    else
    {
        printf("MApi_MPlayer_PlayFile(): Filename is NULL ......\n");
        goto FUNC_END;
    }

    /// Up-layer application class point of call back function.(for application to distinguish)
    _gstMedia.pClass = &_gstMedia;
    //Regiter callback.
    _gstMedia.pCallback = _Demo_MM_Callback;

#if(DEMO_AUDIO_MULTI_TEST == 1)
    if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE || eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        if(Demo_MM_AllocAudDec() == FALSE)
        {
            goto FUNC_END;
        }
    }
#endif

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

        _gstWaveHdr.fileleth = (MS_U32)_gu64Size[eMediaType] + 36;
        _gstWaveHdr.FmtHdrLeth = 16;
        _gstWaveHdr.BitsPerSample = *(MS_U16*)&_gu8AudioPara[3];
        _gstWaveHdr.Channels = *(MS_U16*)&_gu8AudioPara[1];
        _gstWaveHdr.FormatTag = 0x0001;
        _gstWaveHdr.SamplesPerSec = *(MS_U16*)&_gu8AudioPara[5];
        _gstWaveHdr.BlockAlign = _gstWaveHdr.Channels * _gstWaveHdr.BitsPerSample / 8;
        _gstWaveHdr.AvgBytesPerSec = _gstWaveHdr.BlockAlign * _gstWaveHdr.SamplesPerSec;
        _gstWaveHdr.DataHdrLeth = (MS_U32)_gu64Size[eMediaType];
        _gu64Size[eMediaType] = (MS_S64)_gu64Size[eMediaType] + 44;

        mm_print("--------> PCM music format----------\n");
        mm_print("---BitsPerSample is %d----------\n", _gstWaveHdr.BitsPerSample);
        mm_print("---Channels is %d----------\n", _gstWaveHdr.Channels);
        mm_print("---SamplesPerSec is %d----------\n", (int)_gstWaveHdr.SamplesPerSec);
        mm_print("---Endian is %d----------\n", _gu8AudioPara[9]);
    }

    mm_print("------> _gu64Size[eMediaType] : %lld \n", _gu64Size[eMediaType]);

    _gstMedia.u32CtlFlag = 0;
    if(eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE && pMplayerMedia->eFileMode == MPLAYER_FILE_MODE)
    {
        _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_ENABLE_AUDIO_HANDLER;
        _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
        _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2] = _Demo_FileOpen((char* const )pMplayerMedia->filename, (char* const)"rb");
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
    _gstMedia.u32CtlFlag |= E_MMSDK_CTL_FLAG_TRICKMODE_ON;
    _gstMedia.u64FileLength = (MS_U64)_gu64Size[eMediaType];
    _gstMedia.pFileHandle1 = _gpStream[eMediaType];
    if(_gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE])//subtile
        _gstMedia.pFileHandle2 = _gpStream[E_MPLAYER_MEDIA_STREAM_SUBTITLE];
    else
        _gstMedia.pFileHandle2 = _gpStream[eMediaType];
    _gstMedia.pFileHandle3 = _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO];
    _gstMedia.pFileHandle4 = _gpStream[E_MPLAYER_MEDIA_STREAM_AUDIO2];

    _gstMedia.enMediaType = eMediaType;

    if(pMplayerMedia->eFileMode == MPLAYER_FILE_MODE)
    {
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_MASS_STORAGE;    //For USB content
    }

    if(_gu64Size[eMediaType] == (MS_U64) - 1)
    {
        mm_print("----> stream without seek\n");
        _geStrmMode[handle] = MPLAYER_UNSEEKABLE_STREAM_MODE;
        _gstMedia.enMediaFileType = E_MMSDK_FILE_TYPE_NETWORK_STREAM_WITHOUT_SEEK;
    }

    // Do not demo motion GIF, so setting E_MMSDK_CREATE_MODE_PREVIEW to decode only first frame
    if(strcmp("GIF", (char *)_gu8FileExtName) == 0)
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

    if(MMSDK_CreateMediaItem(&_gstMedia, &astExtenInfoParam, u32ExtenInfoParamNum, &_gItem[eMediaType]) != TRUE)
    {
        printf("!!!!!MM_CreateMediaItem returns fail in playfile \n");
        goto FUNC_END;
    }

    if(_gItem[eMediaType] == NULL)
    {
        printf("Create Meida item fail. u32Item is NULL in playfile\n");
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

    MMSDK_SetLoop(_gItem[eMediaType], _gu8IsRepeat);

    if(_gThreadEvtProcess[eMediaType].iId == -1)
    {
        if(TH_EVT_PROC_STACK_SIZE)
        {
            _gThreadEvtProcess[eMediaType].pStack = MsOS_AllocateMemory(_gThreadEvtProcess[eMediaType].u32StackSize, s32MstarNonCachedPoolID);
            if(!_gThreadEvtProcess[eMediaType].pStack)
            {
                printf("MApi_MPlayer_PlayFile(): _gThreadEvtProcess[%d] stack malloc failed......\n",eMediaType);
                goto FUNC_END;
            }
        }

        _gThreadEvtProcess[eMediaType].iId = MsOS_CreateTask( _gpEvtProcess[eMediaType],
                                    (MS_U32)NULL,
                                    _gThreadEvtProcess[eMediaType].ePriority,
                                    TRUE,
                                    _gThreadEvtProcess[eMediaType].pStack,
                                    _gThreadEvtProcess[eMediaType].u32StackSize,
                                    _gThreadEvtProcess[eMediaType].szName);

        if(_gThreadEvtProcess[eMediaType].iId == -1)
        {
            printf("MApi_MPlayer_PlayFile(): create Thread _gpEvtProcess failed......\n");
            _gstMedia.pCallback = NULL;
            goto FUNC_END;
        }
    }

    bRet = TRUE;

FUNC_END:
    if(bRet == FALSE && _gpStream[eMediaType])
    {
        _Demo_FileClose(_gpStream[eMediaType]);
        _gpStream[eMediaType] = NULL;
    }

    mm_print("------> Leave func MApi_MPlayer_PlayFile \n");
    _Demo_ReleaseRecursiveMutex(_gs32MutexOpenStop);
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
    MS_U8 *pu8ExtName=NULL;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(!b_MM_Initied)
        return B_Ret;

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_SPDIF_SetMode(MSAPI_AUD_SPDIF_PCM);

    if(strlen((char*)U8FileName) >= FILE_NAME_LENGTH-1)
    {
        mm_print("file length too long\n");
        return B_Ret;
    }

    strcpy((char*)U8File, (char*)U8FileName);
    memset(&sMplayerMeida, 0, sizeof(MPLAYER_MEDIA));

    _gu32FileOption = *pFileOption;
    strcpy((char*)_gpu8FileName, (char*)U8File);

    if(*pFileOption != MPLAYER_FILE_MODE)
        _gu8IsRepeat = FALSE; //currently only support E_MMSDK_FILE_TYPE_MASS_STORAGE

    if(*pFileOption == MPLAYER_FILE_MODE)
    {
        u32FileNameLen = strlen((char *)U8FileName);
        pu8ExtName = (MS_U8*)U8FileName + u32FileNameLen - 1;
        while(pu8ExtName > U8FileName)
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

        if(pu8ExtName > U8FileName)
        {
            MS_U32 i = 0;
            while(pu8ExtName[i])
            {
                //prevent the extension becomes upper case, and may cause the file can't be opened
                u8FileExtName[i] = toupper(pu8ExtName[i]);
                i++;
            }

            strcpy((char *)_gu8FileExtName, (char *)u8FileExtName);

            if (strcmp("PCM", (char *)u8FileExtName) == 0)
            {
                //=========================PCM========================
                _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_MUSIC, MPLAYER_OPTION_SET_MUSIC_TYPE, MPLAYER_SUBTYPE_PCM);
                MPLAYER_PCM_PARAM* pPCM_Param;
                pPCM_Param = (MPLAYER_PCM_PARAM*)MsOS_AllocateMemory(sizeof(MPLAYER_PCM_PARAM), s32MstarNonCachedPoolID);
                pPCM_Param->eFormatTag = PCM_FORMAT_PCM;
                pPCM_Param->u16Channels = 2;
                pPCM_Param->u16BitsPerSample = 16;
                pPCM_Param->u32SamplesPerSec = 44100;
                pPCM_Param->bBigEndian = TRUE;
                _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_MUSIC, MPLAYER_OPTION_MUSIC_SET_PCM_PARAM, (unsigned int)pPCM_Param);
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

        //Force disable SLIDE SHOW for MVOP/GOP show photo behavior are consistent
        _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_DISABLE_SLIDE_SHOW, TRUE);

        mm_print("\n[%d] %s :   -> %s %s\n", __LINE__, __FILE__, U8FileName,U8File);
        //set display window, {0, 0, 0, 0} means full panel
        sMplayerMeida.eFileMode = MPLAYER_FILE_MODE;
        sMplayerMeida.u32DispX = 0;
        sMplayerMeida.u32DispY = 0;
        sMplayerMeida.u32DispW = IPANEL(&gDevId ,Width);
        sMplayerMeida.u32DispH = IPANEL(&gDevId ,Height);
        sMplayerMeida.u32GotoTimeMs = 0;
        sMplayerMeida.filename = (char*)U8File;//utf8fn;

        B_Ret = _Demo_MPlayer_PlayFile(_geMediaType, &sMplayerMeida);
    }
    else
        printf("Error!! Not supportted file type (=%d)!!", (int)*pFileOption);

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

    if (_gpu8FileName && ((_gu32FileOption == MPLAYER_MODE_INVALID) || (_gu32FileOption == MPLAYER_FILE_MODE)))
    {
        _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_FB_REPLAY, (MS_BOOL)*pu32On);
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
    return MEM_ADR_BY_MIU(JPD_OUT_ADR, JPD_OUT_MEMORY_TYPE);
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
    MS_U32 u32AudioTrackNum;
    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return FALSE;
    }

    u32AudioTrackNum = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_AUDIO_TRACK);
    printf("Audio Track Num  = %"DTC_MS_U32_d"\n", u32AudioTrackNum);

    if (u32AudioTrackNum <= *u32TrackId)
    {
        printf("Track Id(%"DTC_MS_U32_d") must <= u32AudioTrackNum\n",u32AudioTrackNum);

        return FALSE;
    }

    _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_AUDIO, *u32TrackId);
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
    MS_U32 u32SubtitleTrackNum;
    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return FALSE;
    }

    u32SubtitleTrackNum = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_SUBTITLE_TRACK);
    printf("Subtitle Track Num  = %"DTC_MS_U32_d"\n", u32SubtitleTrackNum);

    if (u32SubtitleTrackNum <= *u32TrackId)
    {
        printf("Track Id(%"DTC_MS_U32_d") must <= u32SubtitleTrackNum\n", u32SubtitleTrackNum);
        return FALSE;
    }

    _Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE, *u32TrackId);
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
    MS_U32 u32ProgramNum;
    if (_geMediaType !=E_MMSDK_MEDIA_TYPE_MOVIE)
    {
        return FALSE;
    }

    u32ProgramNum = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_PROGRAM);
    printf("Program Num  = %"DTC_MS_U32_d"\n", u32ProgramNum);

    if (u32ProgramNum <= *u32ProgramId)
    {
        printf("Program Id(%"DTC_MS_U32_d") must <= Program Num\n", *u32ProgramId);
        return FALSE;
    }

    if(_Demo_MPlayer_SetOption(E_MMSDK_MEDIA_TYPE_MOVIE, MPLAYER_OPTION_CHANGE_PROGRAM, *u32ProgramId))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL Demo_MM_Get_DrawPhotoInfo(MS_U16* u16width, MS_U16* u16height, MS_U16* u16pitch, MS_U16* u16format)
{
    *u16width = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODED_WIDTH);
    *u16height = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODED_HEIGHT);
    *u16pitch = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODED_PITCH);
    *u16format = _Demo_MPlayer_GetOption(E_MMSDK_MEDIA_TYPE_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, MPLAYER_INFO_PHOTO_DECODE_FORMAT);
    printf("[Demo_MM_Get_DrawPhotoInfo] %d %d %d %d\n",*u16width,*u16height,*u16pitch,*u16format);
    return TRUE;
}

MS_BOOL Demo_MM_Set_DrawPhotoPath(MS_U32* u32Path)
{
    if(*u32Path == 1)
    {
        PT_SYS_SetMMPhotoPath(1);
        printf("[Demo_MM_Set_DrawPhotoPath] use MVOP output\n");
    }
    else
    {
        PT_SYS_SetMMPhotoPath(0);
        printf("[Demo_MM_Set_DrawPhotoPath] use GOP output\n");
    }
    return TRUE;
}

MS_BOOL Demo_MM_SetDebugLevel(MS_U32* u32DebugLevel)
{
    if (u32DebugLevel == NULL)
    {
        printf("u32DebugLevel == NULL\n");
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
        printf("Debug Level is %"DTC_MS_U32_d"\n", (MS_U32)eMMSDKDebugLevel);

        return _Demo_MPlayer_SetOption(_geMediaType, MPLAYER_OPTION_SET_DBG_LEVEL, (unsigned int)(eMMSDKDebugLevel));
    }
}

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
#if(DEMO_AUDIO_MULTI_TEST == 1)
    Demo_MM_AllocAudDec();
#endif

    return TRUE;
}
#endif // #if (DEMO_APM_TEST == 1)
#endif

