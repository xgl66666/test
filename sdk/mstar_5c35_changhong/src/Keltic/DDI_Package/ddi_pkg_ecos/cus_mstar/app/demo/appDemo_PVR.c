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
#ifdef ENABLE_PVR_PACKAGE

////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_PVR.c
/// @author MStar Semiconductor Inc.
/// @brief  PVR Function Sample Demo
///
/// @name PVR Function Sample Command Usage
/// @verbatim
/// ----------------------------------------------------------------------------
/// Note: DigiTuner_SetFreq must be called to set tuner/demod before any operations
///
/// 1. pvr_pat
/// Usage: pvr_pat
///
/// Sample UART Command:pvr_pat
///
/// ---------------------------------------------------------------------
///
///
///=================================================================================================



//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>

#include "MsCommon.h"
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cyg/fileio/fileio.h>
#include <errno.h>

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiHDMITx.h"
#include "apiDMX.h"
#include "apiPVR.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "MsMemory.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "appDemo_PVR.h"
#include "appDemo_AV.h"
#include "appDemo_DMX.h"
#include "appDemo_XC.h"
#include "appDemo_GOP.h"
#include "appDemo_Video.h"
#include "appDemo_Audio.h"
#include "MsFS.h"
#include "apiDAC.h"


//Drv
#include "drvTVEncoder.h"
#include "drvMVOP.h"

#include "drvSYS.h"
#include "drvMVOP.h"
#include "drvPQ.h"

//MApi
#include "msAPI_XC.h"
#include "msAPI_VE.h"
#include "msAPI_GEGOP.h"
#include "Panel.h"



#include "apiGFX.h"
#include "apiGOP.h"
#include "apiPNL.h"


#include "drvDSCMB.h"
#include "drvBDMA.h"





extern MS_S32 gs32CachedPoolID ;
extern MS_S32 gs32NonCachedPoolID;

#define PVR_DBG 0
#if PVR_DBG
#define appDemoPvr_print(fmt, args...)   { printf("[appDemoPvr][%06d]\t", __LINE__); printf( fmt, ## args); }
#else
#define appDemoPvr_print(fmt, args...)
#endif

#define PVR_EVENT_DBG 0

#if PVR_EVENT_DBG
#define appDemoPvrEvent_print(fmt, args...)  { printf("[appDemoPvr][%s][%06d]    ",__FUNCTION__, __LINE__); printf( fmt, ## args); }
#else
#define appDemoPvrEvent_print(fmt, args...)
#endif
#define INVALID_FILTER_ID                       0xFF

#define PVR_SYNC_DELAY          (0)

#define PVR_DEFAULT_PROGRAM_IDX 0
#define PVR_MAX_RECORDING_FILE EN_APIPVR_DUAL_TUNER
#define PVR_THREAD_NAME "PVR_MSG"
#define MAX_PROGRAM_NUM PVR_MAX_RECORDING_FILE
#define PVR_FILE_LINEAR  0
#define PVR_FILE_CIRCULAR  1
#define APIPVR_PLAYBACK_SPEED_1X  1000
#define PVR_FILE_SIZE_TO_STOP     (30*1024) /*30M space for USB reserver */
#define MIN_TIME_SHIFT_DISK_SPACE_IN_KB (512*1024)  /* 512MB */
#define INVALID_PID 0x1FFF
#define PVR_IS_VALID_PID(x)  ((x!= 0)&& (x< INVALID_PID))
#define PVR_IS_VALID_HANDLER(x) (x!=APIPVR_INVALID_HANDLER)
#define INVALID_TASK_ID -1
#define TASK_STACK_SIZE 0x2000
#define PVR_Main_Window 0


//Dwin buffer size set to panel width and height to capture the whole frame
#define PVR_THUMBNAIL_CAPTURE_WIDTH     (g_IPanel.Width()) /* 1920 */
#define PVR_THUMBNAIL_CAPTURE_HEIGHT   (g_IPanel.Height()) /*1080 */
#define PVR_THUMBNAIL_SURFACE_SIZE    (PVR_THUMBNAIL_CAPTURE_WIDTH*PVR_THUMBNAIL_CAPTURE_HEIGHT*2)
#define PVR_THUMBNAIL_DISPLAY_X     0
#define PVR_THUMBNAIL_DISPLAY_Y    0
#define PVR_THUMBNAIL_DISPLAY_WIDTH (PVR_THUMBNAIL_CAPTURE_WIDTH>>1)
#define PVR_THUMBNAIL_DISPLAY_HEIGHT (PVR_THUMBNAIL_CAPTURE_HEIGHT>>1)
#define PVR_THUMBNAIL_FRAME_BUFFER_FORMAT E_MS_FMT_ARGB4444

typedef enum
{
    EN_PVR_RECDMX_TYPE_AUDIO=0,
    EN_PVR_RECDMX_TYPE_TTX,
    EN_PVR_RECDMX_TYPE_CC,
    EN_PVR_RECDMX_TYPE_SUB,
    EN_PVR_RECDMX_TYPE_TTXSUB,
    EN_PVR_RECDMX_TYPE_MAX
}EN_PVR_RECDMX_TYPE;


typedef struct
{
    EN_GOP_DST_TYPE GopDest;
    MS_U8 GeWinId;
    MS_U8 u8FBId;
    MS_U16 GopBitmapWidth;
    MS_U16 GopBitmapHeight;
    MS_U16 GeDstFmt;
}GWinInfo;

typedef enum
{
    SiNOT_VALID                    = 0x00,
    SiMPEG                         = 0x01,
    SiAC3                          = 0x02,
    SiMPEG_AD                      = 0x03,
    SiMPEG4                        = 0x04,
    SiAAC                          = 0x05,
    SiAC3P                         = 0x06,
    SiAC3_AD                       = 0x07,
} SI_AudioFormat;

typedef struct
{
    char recPath[32];
}PVR_FSINFO;

typedef enum
{
    PVR_TIMESHIFT_PATH_IDX=0,
    PVR_NORMALREC_PATH_IDX=1,
}PVR_REC_PATH_IDX;

typedef struct PVR_AV_ID_s
{
    void *vstreamID;
    void *astreamID;

}PVR_AV_ID_t;

typedef struct
{
    MS_U8 PIDType;
    MS_BOOL isOpen;
}PVR_PIDSWITCH_SET;

static PVR_AV_ID_t _AVStreamID = {NULL,NULL};


/* PVR_DOWNLOAD   */
static void *            _pRec = NULL;
/* PVR_UPLOAD   */
static void *            _pPlay = NULL;
/* PVR_AUDIO_BUF   */
static void *            _pAudioBuf = NULL;
/* PVR_MN_VIDEO_BUF   */
static void *            _pMnVid = NULL;
/* PVR_MN_AUDIO_BUF   */
static void *            _pMnAud = NULL;
/* PVR_TSR_VIDEO_BUF   */
static void *            _pTsrVid = NULL;
/* PVR_TSR_AUDIO_BUF   */
static void *            _pTsrAud = NULL;
/* PVR_AESBUFFER   */
static void *            _pAesBuf = NULL;
/* PVR_DOWNLOAD   */
#define PVR_DOWNLOAD_LEN                                       0x0000360000  /* 3.375 M*/
/* PVR_UPLOAD   */
#define PVR_UPLOAD_LEN                                         0x0000120000 /* 1.125 M*/
/* PVR_AUDIO_BUF   */
#define PVR_AUDIO_BUFFER_LEN                                      0x0000010000 /* 64 KB*/
/* PVR_MN_VIDEO_BUF   */
#define PVR_MN_VIDEO_BUFFER_LEN                                   0x0000300000
/* PVR_MN_AUDIO_BUF   */
#define PVR_MN_AUDIO_BUFFER_LEN                                   0x0000010000
/* PVR_TSR_VIDEO_BUF   */
#define PVR_TSR_VIDEO_BUFFER_LEN                                  0x0000360000
/* PVR_TSR_AUDIO_BUF   */
#define PVR_TSR_AUDIO_BUFFER_LEN                                  0x0000020000
/* PVR_AESBUFFER   */
#define PVR_AESBUFFER_LEN                                      16



#define PVR_STACK_SIZE 0x00004000
static MS_U8 u8PvrEventTaskStack[PVR_STACK_SIZE];
static Task_Info _pvrEventTask = {-1, E_TASK_PRI_MEDIUM, u8PvrEventTaskStack, PVR_STACK_SIZE, "PVR MSG Task"};

//For Unplugging USB
static MS_BOOL _bUplugMsg=FALSE;
static char _moutPath[FILE_PATH_SIZE]={0};

// time
static MS_BOOL _bShowPlayTime=FALSE;
static MS_BOOL _bShowRecordTime=FALSE;

//For PVR
static MS_BOOL _bPvrInit= FALSE;
static MS_S32 _s32QueueID = INVALID_TASK_ID;
static MS_S32 _s32ThreadID = INVALID_TASK_ID;
static MS_S32 _s32DbgPVRTaskID = INVALID_TASK_ID;
static MS_U8 _u8TotalProgramNum=0;
static MS_U8 _u8CurProgramIdx=0;
static MS_U8 _u8TimeshiftIdx=0xFF;
static MS_U8 _u8hRecord[PVR_MAX_RECORDING_FILE]={APIPVR_INVALID_HANDLER};
static MS_U8 _u8hPlayback;
static PVRProgramInfo_t _recprogramInfo[PVR_MAX_RECORDING_FILE];
static PVRProgramInfo_t _plyprogramInfo;
static PVRProgramInfo_t _livePromInfo;

static MS_BOOL _gbMute=FALSE;
static PVR_FSINFO _recFSInfo[PVR_MAX_RECORDING_FILE];
static MS_BOOL _bScreenFrozen =TRUE;
static GWinInfo _gWinInfo={0};
static MS_U8 _u8TotalRecorder=2;


static MS_U8 _u8PcrDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8VidDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8AudDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_AUDIOINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8TtxDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_TTXINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8CcDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8EBUSubtDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_SUBTITLEINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8DVBSubtDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_SUBTITLEINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};



//For still image zapping
static MS_BOOL _bEnStillImageZapping = FALSE;

//For Debug
static MS_U8 _u8DBGLevel=0;
static MS_U32 _u32DBGIntervel=1000;

static MS_U8 _u8CurPlaybackProgramPath=PVR_Main_Window;

//--------------------------------------------------------------------------------------------------
// API prototype
//--------------------------------------------------------------------------------------------------
void* _appDemo_PVR_MEMALLOC_FUNC(MS_U32 u32Size);
MS_BOOL _appDemo_PVR_MEMFREE_FUNC(void *pBuf);
MS_U32  _appDemo_PVR_MEMPA2VA_FUNC(MS_U32 u32PhyAdr);
void _appDemo_PVR_MEMFLUSH_FUNC(MS_U32 u32Adr,MS_U32 u32Len);
MS_BOOL _appDemo_PVR_KLADDER_FUNC(void);
static MS_BOOL _appDemo_PVR_AllocateMem(int pvrId);
static MS_BOOL _appDemo_PVR_FreeMem(int pvrId);
static MS_BOOL _appDemo_PVR_AudioMute(MS_BOOL bMute, MS_BOOL bPvrPlayback);
//record start
static MS_BOOL _appDemo_PVR_RecordStart(MS_BOOL bTimeshift, MS_U32 u32FreeSizeInMB,const MS_U32* VideoPid,const  MS_U32* AudioPid,const  MS_U32* PCRPid,const  MS_U32* pu32VCodec,const  MS_U32* pu32ACodec, char* fileName);
static MS_BOOL _appDemo_PVR_RecStart(MS_BOOL bTimeshift, MS_U32 u32FreeSizeInMB);
static MS_BOOL _appDemo_PVR_RecStop(MS_BOOL bTimeshift, MS_U8 u8Idx);
static MS_BOOL _appDemo_PVR_AV_Play(MS_BOOL bLive);
static MS_BOOL _appDemo_PVR_AV_Start(MS_BOOL bIsLiveStream,PVRProgramInfo_t *programInfo, MS_BOOL bEsplayer);
static void _appDemo_PVR_Callback(PVREventInfo_t *event);
static MS_BOOL _appDemo_PVR_SetProgramInfo(PVRProgramInfo_t *pPromInfo,MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32VCodec, MS_U32 u32ACodec,MS_U32 u32PmtPid, MS_U32 u32LCN);
static void _appDemo_PVR_Task(MS_U32 argc, void *argv);
static void _appDemo_PVR_StopEvent(MS_BOOL bRecord,MS_U8 pvrRecIdx);
static MS_BOOL _appDemo_PVR_InsertFilter(PVRFilterInfo_t *pFilter, MS_U16 u16PID, MS_U32 u32DmxFltType,MS_U32 *pu32Idx);
static MS_U16 _appDemo_PVR_FindPidinFilter(PVRFilterInfo_t *pFilter,MS_U32 u32DmxFltType);
static  void _appDemo_PVR_Remove_Capture_Video(void);
static MS_BOOL _appDemo_PVR_Show_Capture_Video(MS_U32 u32PhysicalAddr);
static MS_U32 _appDemo_PVR_IsH264(APIPVR_CODEC_TYPE enPvrVCodec);
static MS_BOOL _appDemo_PVR_ResetProgramInfo(PVRProgramInfo_t *pPromInfo);
static void _appDemo_PVR_FreezeScreen(MS_BOOL bFrezon);
static MS_BOOL _appDemo_PVR_FileModePlay(MS_U32 u32VCodec, MS_U32 u32ACodec);
static MS_BOOL _appDemo_PVR_InitAudioInfo(MS_BOOL bReload,MS_BOOL bFileMode, MS_U32 u32ACodec,MS_BOOL bSPDIFMODE,MS_BOOL bHDMIMODE);
static MS_U32 _appDemo_PVR_ADEC_Fmt(MS_U32 u32ACodec);
static APIPVR_CODEC_TYPE _appDemo_PVR_VDEC_Fmt(MS_U32 u32VCodec);
static MS_U8 _appDemo_PVR_GetRecordIndex(MS_U32 u32RecHandler);
static  MS_BOOL  _appDemo_PVR_GetRecMountPath(MS_BOOL bTimeshift, char *mountpath);
static MS_BOOL appDemo_PVR_Dmx_Shutdown(MS_U8 *pu8DmxId);
static MS_BOOL appDemo_PVR_Dmx_Setup(MS_U16 u16PID,MS_U8 *pu8DmxId);
static MS_BOOL _appDemo_PVR_GetFileInfo(char *FileName, PVRProgramInfo_t *promInfo);
static MS_BOOL _PVR_TSP_OpenPidFlt(MS_U8 u8Idx);
static MS_BOOL _PVR_TSP_ClosePidFlt(MS_U8 u8Idx);
static MS_BOOL _appdemo_PVR_DBGCreateTask(void);
static void _appdemo_PVR_DBG_MSG(void);
static MS_BOOL _appDemo_PVR_SetMotionPatch(MS_BOOL *bEnable);
static MS_BOOL _appDemo_PVR_PIDSWITCH_FUNC(PVR_PIDSWITCH_SET *PidSwitch_Info);

//--------------------------------------------------------------------------------------------------
// API implmentation
//--------------------------------------------------------------------------------------------------

static MS_U8 _appDemo_PVR_GetRecordIndex(MS_U32 u32RecHandler)
{
    MS_U8 u8Idx=0;
    for(u8Idx=0;u8Idx<PVR_MAX_RECORDING_FILE;u8Idx++)
    {
        if(((MS_U32)_u8hRecord[u8Idx])==u32RecHandler)
        {
            return u8Idx;
        }
    }
    return APIPVR_INVALID_HANDLER;
}

void* _appDemo_PVR_MEMALLOC_FUNC(MS_U32 u32Size)
{
    return MsOS_AllocateMemory (u32Size, gs32NonCachedPoolID);
}

MS_BOOL _appDemo_PVR_MEMFREE_FUNC(void *pBuf)
{
    return MsOS_FreeMemory(pBuf, gs32NonCachedPoolID);
}

MS_U32  _appDemo_PVR_MEMPA2VA_FUNC(MS_U32 u32PhyAdr)
{
    MS_U32 u32VirAdr=MsOS_PA2KSEG1(u32PhyAdr);
    return u32VirAdr;
}

void _appDemo_PVR_MEMFLUSH_FUNC(MS_U32 u32Adr,MS_U32 u32Len)
{
    MsOS_Sync();
    MsOS_FlushMemory();
}


MS_BOOL _appDemo_PVR_KLADDER_FUNC(void)
{
    MS_BOOL ret = FALSE;
    MS_U8 u8KLInputKey[48]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
    MS_U8 key[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};


    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_AES,
        .eSrc = E_DSCMB_KL_SRC_ACPU,
        .eDst = E_DSCMB_KL_DST_DMA_AES,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = E_DSCMB_KEY_CLEAR,      // Don't care when dst is Crypto DMA
        .u32Level = 3,
        .u32EngID = 0,                      // Don't care when dst is Crypto DMA
        .u32DscID = 0,                      // Don't care when dst is Crypto DMA
        .u8KeyACPU = key,                  //Don't care when src is NOT ACPU
        .pu8KeyKLIn = u8KLInputKey,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
    };

    // Key Ladder
    ret = MDrv_DSCMB_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!!\n");
        return FALSE;
    }

    return TRUE;
}

static void _appDemo_PVR_FreezeScreen(MS_BOOL bFrezon)
{
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();
    MApi_VDEC_EX_SetFreezeDisp(stVDECStreamId,bFrezon);
}


static MS_BOOL _appDemo_PVR_FreeMem(int pvrId)
{

    switch(pvrId)
    {
        case PVR_ID_DOWNLOAD:

            appDemoPvr_print("[%s][%d] _pRec=%p\n",__FUNCTION__,__LINE__,_pRec);
            if(_pRec != NULL)
            {
                appDemoPvr_print("[%s][%d] --> try to free _pRec\n",__FUNCTION__,__LINE__);
                if(!_appDemo_PVR_MEMFREE_FUNC(_pRec))
                {
                    printf("_pRec Free Fail\n");
                    return FALSE;
                }
                _pRec = NULL;
            }
            break;
        case PVR_ID_UPLOAD:

            appDemoPvr_print("[%s][%d] _pPlay=%p\n",__FUNCTION__,__LINE__,_pPlay);
            if(_pPlay != NULL)
            {
                appDemoPvr_print("[%s][%d] --> try to free _pPlay\n",__FUNCTION__,__LINE__);
                if(!_appDemo_PVR_MEMFREE_FUNC(_pPlay))
                {
                    printf("_pPlay Free Fail\n");
                    return FALSE;
                }
                _pPlay = NULL;
            }
            break;

        case PVR_ID_BUFFER:
            appDemoPvr_print("[%s][%d] _pAesBuf=%p\n",__FUNCTION__,__LINE__,_pAesBuf);
            if(_pAesBuf != NULL)
            {
                appDemoPvr_print("[%s][%d] --> try to free _pAesBuf\n",__FUNCTION__,__LINE__);
                if(!_appDemo_PVR_MEMFREE_FUNC(_pAesBuf))
                {
                    printf("_pAesBuf Free Fail\n");
                    return FALSE;
                }
                _pAesBuf = NULL;
            }
            break;

        case PVR_ID_ES_VIDEO_BUFFER:
            return TRUE;
            break;
        case PVR_ID_ES_AUDIO_BUFFER:

            appDemoPvr_print("[%s][%d] _pAudioBuf=%p\n",__FUNCTION__,__LINE__,_pAudioBuf);
            if(_pAudioBuf != NULL)
            {
                if(!_appDemo_PVR_MEMFREE_FUNC(_pAudioBuf))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                _pAudioBuf = NULL;
            }
            break;
        case PVR_ID_MN_VIDEO_BUFFER:

            appDemoPvr_print("[%s][%d] _pMnVid=%p\n",__FUNCTION__,__LINE__,_pMnVid);
            if(_pMnVid != NULL)
            {
                if(!_appDemo_PVR_MEMFREE_FUNC(_pMnVid))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                _pMnVid = NULL;
            }
            break;
        case PVR_ID_MN_AUDIO_BUFFER:

            appDemoPvr_print("[%s][%d] _pMnAud=%p\n",__FUNCTION__,__LINE__,_pMnAud);
            if(_pMnAud != NULL)
            {
                if(!_appDemo_PVR_MEMFREE_FUNC(_pMnAud))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                _pMnAud = NULL;
            }
            break;
        case PVR_ID_TSR_VIDEO_BUFFER:

            appDemoPvr_print("[%s][%d] _pTsrVid=%p\n",__FUNCTION__,__LINE__,_pTsrVid);
            if(_pTsrVid != NULL)
            {
                if(!_appDemo_PVR_MEMFREE_FUNC(_pTsrVid))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                _pTsrVid = NULL;
            }
            break;
        case PVR_ID_TSR_AUDIO_BUFFER:

            appDemoPvr_print("[%s][%d] _pTsrAud=%p\n",__FUNCTION__,__LINE__,_pTsrAud);
            if(_pTsrAud != NULL)
            {
                if(!_appDemo_PVR_MEMFREE_FUNC(_pTsrAud))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                _pTsrAud = NULL;
            }
            break;

        default:
            printf("[%s][%d] Should not be here. pvrId=%d\n",__FUNCTION__,__LINE__,pvrId);
            return FALSE;
            break;

    }
    return MApi_PVR_ResetMmap(pvrId);

}



static MS_BOOL _appDemo_PVR_AllocateMem(int pvrId)
{
    MS_U32 u32PhyStartAdr=0;
    MS_U32 u32Size=0;



    switch(pvrId)
    {
        case PVR_ID_DOWNLOAD:

            if(_pRec == NULL)
            {
                _pRec = _appDemo_PVR_MEMALLOC_FUNC(PVR_DOWNLOAD_LEN);
                if(_pRec == NULL)
                {
                    printf("[%s][%d] _pRec HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pRec);
            u32Size = PVR_DOWNLOAD_LEN;

            appDemoPvr_print("[%s][%d] PVR_ID_DOWNLOAD %8lX (0x%8lX+0x%8lX=0x%8lX) \n",__FUNCTION__,__LINE__,_pRec,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));

            break;
        case PVR_ID_UPLOAD:

            if(_pPlay == NULL)
            {
                _pPlay= _appDemo_PVR_MEMALLOC_FUNC(PVR_UPLOAD_LEN);
                if(_pPlay == NULL)
                {
                    printf("[%s][%d] _pPlay HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pPlay);
            u32Size = PVR_UPLOAD_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_UPLOAD %8lX (0x%8lX+0x%8lX=0x%8lX) \n",__FUNCTION__,__LINE__,_pPlay,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;

        case PVR_ID_BUFFER:
            if(_pAesBuf == NULL)
            {
                _pAesBuf= _appDemo_PVR_MEMALLOC_FUNC(PVR_AESBUFFER_LEN);
                if(_pAesBuf == NULL)
                {
                    printf("[%s][%d] _pAesBuf HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pAesBuf);
            u32Size = PVR_AESBUFFER_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,_pAesBuf,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;

        case PVR_ID_ES_VIDEO_BUFFER:
#if (VDEC_BIT_STREAM_MEMORY_TYPE&MIU1)
            u32PhyStartAdr = VDEC_BIT_STREAM_ADR +MIU_INTERVAL;
#else
            u32PhyStartAdr = VDEC_BIT_STREAM_ADR;
#endif
            u32Size = VDEC_BIT_STREAM_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_ES_VIDEO_BUFFER (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));

            break;
        case PVR_ID_ES_AUDIO_BUFFER: // MIU1

            if(_pAudioBuf == NULL)
            {
                _pAudioBuf= _appDemo_PVR_MEMALLOC_FUNC(PVR_AUDIO_BUFFER_LEN);
                if(_pAudioBuf == NULL)
                {
                    printf("[%s][%d] _pAudioBuf HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pAudioBuf);
            u32Size = PVR_AUDIO_BUFFER_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_ES_AUDIO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,_pAudioBuf,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;

        case PVR_ID_MN_VIDEO_BUFFER:

            if(_pMnVid== NULL)
            {
                _pMnVid= _appDemo_PVR_MEMALLOC_FUNC(PVR_MN_VIDEO_BUFFER_LEN);
                if(_pMnVid == NULL)
                {
                    printf("[%s][%d] _pMnVid HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pMnVid);
            u32Size = PVR_MN_VIDEO_BUFFER_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_MN_VIDEO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,_pMnVid,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;
        case PVR_ID_MN_AUDIO_BUFFER:

            if(_pMnAud== NULL)
            {
                _pMnAud= _appDemo_PVR_MEMALLOC_FUNC(PVR_MN_AUDIO_BUFFER_LEN);
                if(_pMnAud == NULL)
                {
                    printf("[%s][%d] _pMnAud HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pMnAud);
            u32Size = PVR_MN_AUDIO_BUFFER_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_MN_AUDIO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,_pMnAud,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;

        case PVR_ID_TSR_VIDEO_BUFFER:

            if(_pTsrVid== NULL)
            {
                _pTsrVid= _appDemo_PVR_MEMALLOC_FUNC(PVR_TSR_VIDEO_BUFFER_LEN);
                if(_pTsrVid == NULL)
                {
                    printf("[%s][%d] _pTsrVid HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pTsrVid);
            u32Size = PVR_TSR_VIDEO_BUFFER_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_TSR_VIDEO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,_pTsrVid,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;

        case PVR_ID_TSR_AUDIO_BUFFER:

            if(_pTsrAud== NULL)
            {
                _pTsrAud= _appDemo_PVR_MEMALLOC_FUNC(PVR_TSR_AUDIO_BUFFER_LEN);
                if(_pTsrAud == NULL)
                {
                    printf("[%s][%d] _pTsrAud HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pTsrAud);
            u32Size = PVR_TSR_AUDIO_BUFFER_LEN;
            appDemoPvr_print("[%s][%d] PVR_ID_TSR_AUDIO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,_pTsrAud,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;
        default:
            printf("[%s][%d] Should not be here. pvrId=%d\n",__FUNCTION__,__LINE__,pvrId);
            return FALSE;
            break;
    }

    if(MApi_PVR_Mmap(pvrId,u32PhyStartAdr,u32Size)==FALSE)
    {
        printf("[%s][%d] PVR Mmap failed\n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    return TRUE;


}


static  MS_BOOL  _appDemo_PVR_GetRecIdx(MS_U8 *u8RecIdx, MS_BOOL bTimeshift)
{
    int i=0;

    if(bTimeshift)
    {
        *u8RecIdx=_u8TimeshiftIdx;
        return TRUE;
    }

    for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
    {
        if(_u8hRecord[i]==0xFF)
        {
            continue;
        }

        if(_u8TimeshiftIdx!=0xFF)
        {
            if(i!=_u8TimeshiftIdx)
            {
                *u8RecIdx=i;
                return TRUE;
            }
        }
    }
    return FALSE;
}


static  MS_BOOL  _appDemo_PVR_GetRecMountPath(MS_BOOL bTimeshift, char *mountpath)
{
    int len = 0;
    char *path=mountpath;
    PVR_REC_PATH_IDX subPathIdx = PVR_NORMALREC_PATH_IDX;
    if(bTimeshift)
    {
        subPathIdx=PVR_TIMESHIFT_PATH_IDX;
    }

    if(strlen(_recFSInfo[subPathIdx].recPath)==0)
    {
        len = strlen(MApi_PVR_GetMouthPath());
        memcpy(path,MApi_PVR_GetMouthPath(),len);
    }
    else
    {
        sprintf(path,"%s/%s",MApi_PVR_GetMouthPath(),_recFSInfo[subPathIdx].recPath);
    }


    return TRUE;
}


static  MS_BOOL  _appDemo_PVR_CheckUSB(char *mountPath)
{
    //TODO:
    int ret=-1;
    DIR *dp=NULL;
    dp = MsFS_OpenDir(mountPath);
    if(dp == NULL)
    {
        ret = MsFS_MkDir(mountPath, 0755);
        if(ret)
        {

            if(errno != EEXIST)
            {
                printf("[PVR ERROR] : fail to mkdir %s.\n", mountPath);
                return FALSE;
            }
            else
            {
                printf("[PVR] : %s exists\n", mountPath);
            }
        }
    }
    else
    {
        MsFS_CloseDir(dp);
    }
    return TRUE;
}

static MS_BOOL  _appDemo_PVR_GetDiskSpace(char *mountPath,MS_U64* u64FreeSpaceInKB, MS_U64* u64TotalSpaceInKB)
{
    MApi_FsInfo fsInfo;

    if(mountPath==NULL)
    {
        printf("[%s][%d] mountPath==NULL\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    _appDemo_PVR_CheckUSB(mountPath);
    appDemoPvr_print("[%s][%d] mountPath=%s\n",__FUNCTION__,__LINE__,mountPath);
    if(MApi_FS_Info((char *)mountPath, FALSE, &fsInfo)==TRUE)
    {
        *u64TotalSpaceInKB = (((MS_U64)fsInfo.u32ClusTotal* (MS_U64)fsInfo.u32ClusSize) / 1024);
        *u64FreeSpaceInKB = (((MS_U64)fsInfo.u32ClusFree* (MS_U64)fsInfo.u32ClusSize) / 1024);
        appDemoPvr_print("*u64TotalSpaceInKB=%lld *u64FreeSpaceInKB=%lld\n",*u64TotalSpaceInKB,*u64FreeSpaceInKB);
        return TRUE;
    }
    return FALSE;
}

static  MS_BOOL  _appDemo_PVR_IsFileExist(const char *m_mount_path,char *filename)
{
    struct stat buf;
    char path[FILE_PATH_SIZE]={0};
    sprintf(path,"%s/%s",m_mount_path,filename);
    appDemoPvr_print("path = %s\n",path);
    if( MsFS_Stat(path,&buf)<0)
    {
        appDemoPvr_print("errno = %d\n",errno);
        return FALSE;
    }

    return TRUE;
}

static  void _appDemo_PVR_Remove_Capture_Video(void)
{
    if((_gWinInfo.GeWinId==0xff)&&(_gWinInfo.u8FBId==0xff))
    {
        //printf("_gWinInfo.GeWinId=0x%x,_gWinInfo.u8FBId=0x%x\n",_gWinInfo.GeWinId,_gWinInfo.u8FBId);
        return;
    }
    MApi_GOP_GWIN_Enable(_gWinInfo.GeWinId, FALSE);
    MApi_GOP_GWIN_ReleaseWin(_gWinInfo.GeWinId);
    MApi_GOP_GWIN_DeleteWin(_gWinInfo.GeWinId);
    MApi_GOP_GWIN_DestroyWin(_gWinInfo.GeWinId);
    MApi_GOP_GWIN_DestroyFB(_gWinInfo.u8FBId);
    _gWinInfo.GeWinId=_gWinInfo.u8FBId=0xff;

}

static MS_BOOL _appDemo_PVR_Show_Capture_Video(MS_U32 u32PhysicalAddr)
{

    //GE bitblt to copy DWin captured frame to GWin buffer to display
    //Gwin size maybe different to panel size; GE_Bitblt with scaling to change size to display
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;
    GFX_Point gfxPt0,gfxPt1;
    GFX_BufferInfo srcbuf;
    GFX_DrawRect bitbltInfo;

    _appDemo_PVR_Remove_Capture_Video();


    MApi_GOP_GWIN_SwitchGOP(0);//use GOP 0
    MApi_GOP_GWIN_Set_STRETCHWIN(0, E_GOP_DST_OP0, 0, 0, _gWinInfo.GopBitmapWidth, _gWinInfo.GopBitmapHeight);
    _gWinInfo.u8FBId=MApi_GOP_GWIN_GetFreeFBID();
    MApi_GOP_GWIN_CreateFB(_gWinInfo.u8FBId, PVR_THUMBNAIL_DISPLAY_X, PVR_THUMBNAIL_DISPLAY_Y, _gWinInfo.GopBitmapWidth, _gWinInfo.GopBitmapHeight, PVR_THUMBNAIL_FRAME_BUFFER_FORMAT);


    _gWinInfo.GeWinId=MApi_GOP_GWIN_CreateWin_Assign_FB(0, _gWinInfo.u8FBId,PVR_THUMBNAIL_DISPLAY_X, PVR_THUMBNAIL_DISPLAY_Y);
    MApi_GOP_GWIN_SetBlending(0, FALSE, 0xff);
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);

    MApi_GOP_GWIN_MapFB2Win(_gWinInfo.u8FBId, _gWinInfo.GeWinId);
    appDemoPvr_print("[%s][%d] _gWinInfo.u8FBId=%d  _gWinInfo.GeWinId=%d\n", __FUNCTION__, __LINE__,_gWinInfo.u8FBId, _gWinInfo.GeWinId);
    //Set Clip window for rendering
    gfxPt0.x = PVR_THUMBNAIL_DISPLAY_X;
    gfxPt0.y = PVR_THUMBNAIL_DISPLAY_Y;
    gfxPt1.x = _gWinInfo.GopBitmapWidth;
    gfxPt1.y = _gWinInfo.GopBitmapHeight;

    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);
    MApi_GOP_GWIN_Switch2FB( _gWinInfo.u8FBId );    //set dst buffer
    // Bitble the bitmap to DRAM of GE
    srcbuf.u32ColorFmt = E_MS_FMT_YUV422;
    srcbuf.u32Addr = u32PhysicalAddr;
    srcbuf.u32Width = PVR_THUMBNAIL_CAPTURE_WIDTH;
    srcbuf.u32Height = PVR_THUMBNAIL_CAPTURE_HEIGHT;
    srcbuf.u32Pitch = PVR_THUMBNAIL_CAPTURE_WIDTH<<1;
    MApi_GFX_SetSrcBufferInfo(&srcbuf, 0);


    bitbltInfo.srcblk.x = PVR_THUMBNAIL_DISPLAY_X;
    bitbltInfo.srcblk.y = PVR_THUMBNAIL_DISPLAY_Y;
    bitbltInfo.srcblk.width = PVR_THUMBNAIL_CAPTURE_WIDTH;
    bitbltInfo.srcblk.height = PVR_THUMBNAIL_CAPTURE_HEIGHT;

    bitbltInfo.dstblk.x = PVR_THUMBNAIL_DISPLAY_X;
    bitbltInfo.dstblk.y = PVR_THUMBNAIL_DISPLAY_Y;
    bitbltInfo.dstblk.width = _gWinInfo.GopBitmapWidth;
    bitbltInfo.dstblk.height = _gWinInfo.GopBitmapHeight;

    MApi_GFX_BitBlt(&bitbltInfo, u32Flag);

    MApi_GFX_FlushQueue();


    if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(_gWinInfo.GeWinId, TRUE) )
    {
        printf("[%s][%d] MApi_GOP_GWIN_Enable failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    return TRUE;
}



static MS_U32 _appDemo_PVR_IsH264(APIPVR_CODEC_TYPE enPvrVCodec)
{
    if((EN_APIPVR_CODEC_TYPE_H264_IFRAME==enPvrVCodec)||(EN_APIPVR_CODEC_TYPE_H264==enPvrVCodec))
    {
        return 1;
    }
    else if(EN_APIPVR_CODEC_TYPE_AVS==enPvrVCodec)
    {
        return 2;
    }
    return 0;
}
static MS_BOOL _appDemo_PVR_InitAudioInfo(MS_BOOL bReload,MS_BOOL bFileMode, MS_U32 u32ACodec,MS_BOOL bSPDIFMODE,MS_BOOL bHDMIMODE)
{
    if(bReload )
    {
        printf("[Audio] ACodec=%lX\n",u32ACodec);
        switch (u32ACodec)
        {
            case SiMPEG:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
                break;
            case SiAC3:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3);
                break;
            case SiMPEG4:
            case SiAAC:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AAC);
                break;
            case SiAC3P:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3P);
                break;
            default:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
                break;
        }

    }


    if(!bSPDIFMODE)
    {
        MApi_AUDIO_SPDIF_SetMode(SPDIF_OUT_NONPCM);
        printf("SPIDF mode is AUTO mode\n");

    }
    else
    {

        MApi_AUDIO_SPDIF_SetMode(SPDIF_OUT_PCM);
        printf("SPIDF mode is PCM mode\n");

    }

    if(!bHDMIMODE)
    {
        MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_NONPCM);
        printf("HDMI mode is AUTO mode\n");
    }
    else
    {

        MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_PCM);
        printf("HDMI mode is PCM mode\n");
    }


    MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);


    if(bFileMode)
    {
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        printf("[Audio] Play File\n");
    }
    else
    {
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
        printf("[Audio] Play TS File\n");
    }

    MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);


    return TRUE;
}

static MS_BOOL _appDemo_PVR_FileModePlay(MS_U32 u32VCodec, MS_U32 u32ACodec)
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

    //Clear VDEC
    {
        MApi_VDEC_EX_Exit(stVDECStreamId);
    }

    switch (u32VCodec)
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
    initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_FILE;
    initParam.SysConfig.u32CodeBufAddr = VDEC_AEON_ADR + u32MiuInterval;
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    initParam.SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR + u32MiuInterval;
    initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR + u32MiuInterval;
    initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
    initParam.SysConfig.u32DrvProcBufAddr = VDEC_BIT_STREAM_ADR + u32MiuInterval;
    initParam.VideoInfo.eTimeStampType=E_VDEC_EX_TIME_STAMP_PTS;
    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    printf("FILE MODE\n");
    printf("VDEC FW add = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr);

    if (E_VDEC_EX_OK != MApi_VDEC_EX_GetFreeStream(stVDECStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, (VDEC_EX_CodecType)initParam.eCodecType))
    {
        printf(" [%d]....GetFreeStream Failed\n", __LINE__);
        return FALSE;
    }
    else
    {
        printf("ZOE~~appdemo_PVR~~~!!!\n");
        printf(" Stream ID1:  0x%lx, Version 0x%lx \n",stVDECStreamId->u32Id, stVDECStreamId->u32Version);

    }


    //Init/Re-Init VDEC
    if (E_VDEC_EX_OK != MApi_VDEC_EX_Init(stVDECStreamId,&initParam))
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_EX_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }

    //Move MVOP Setting to XC Demo code
    //MDrv_MVOP_Init();
    //MDrv_MVOP_Enable(TRUE);

    //change sync offset to 120ms, this value will influence lip-sync
    //If you want to change this value, please inform video and audio team at the same time
    VDEC_EX_Result enRst = MApi_VDEC_EX_AVSyncOn(stVDECStreamId,TRUE, PVR_SYNC_DELAY, 66);
    if(enRst != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_AVSyncOn fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_EX_SetControl(stVDECStreamId,E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);

    if(MApi_VDEC_EX_SetAVSyncFreerunThreshold(stVDECStreamId,450000)!= E_VDEC_EX_OK)//VDES_AVSYNCTIME
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    MApi_VDEC_EX_EnableESBuffMalloc(stVDECStreamId,TRUE);
    if (MApi_VDEC_EX_Play(stVDECStreamId) != E_VDEC_EX_OK)
    {
        printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }


    _appDemo_PVR_InitAudioInfo(TRUE,TRUE, u32ACodec,TRUE,TRUE);

    return TRUE;
}
static void _appDemo_PVR_PhyClose(void)
{
    MApi_AUDIO_FileEndNotification();
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    MsOS_DelayTask(30);
}

static MS_BOOL _appDemo_PVR_AV_Start(MS_BOOL bIsLiveStream,PVRProgramInfo_t *programInfo, MS_BOOL bEsplayer)
{
    MS_BOOL bRet=TRUE;
    MS_U32 u32VideoPid=_appDemo_PVR_FindPidinFilter(programInfo->Filters, DMX_FILTER_TYPE_VIDEO);
    MS_U32 u32AudioPid=programInfo->u16AudioPid;
    MS_U32 u32PCRPid=_appDemo_PVR_FindPidinFilter(programInfo->Filters, DMX_FILTER_TYPE_PCR);
    appDemoPvr_print("[%s][%d] u32VideoPid=%lu u32AudioPid=%lu u32PCRPid=%lu \n", __FUNCTION__, __LINE__,u32VideoPid,u32AudioPid,u32PCRPid);
    MS_U32 u32VCodec=0;
    int flow = DMX_FLOW_PLAYBACK;
#if (DMX_INPUT == DMX_INPUT_DEMOD)
    int inSrc = DMX_FLOW_INPUT_DEMOD;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT0)
    int inSrc = DMX_FLOW_INPUT_EXT_INPUT0;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT1)
    int inSrc = DMX_FLOW_INPUT_EXT_INPUT1;
#else
    int inSrc = DMX_FLOW_INPUT_DEMOD;
#endif

    int clkInv = 0;
    int extSync = 1;
    int parallel = 1;

    if(!bIsLiveStream)
    {
        inSrc = DMX_FLOW_INPUT_MEM;
    }
    u32VCodec=_appDemo_PVR_IsH264(programInfo->enVCodec);

    if(_bEnStillImageZapping)
    {
        MS_BOOL bEnable = TRUE;
        appDemo_XC_EnableStillImage(&bEnable);
    }
    else /* Black Video*/
    {
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);
    }

    appDemoPvr_print("[%s][%d]  bIsLiveStream=%d\n",__FUNCTION__,__LINE__,bIsLiveStream);

    if(bIsLiveStream)
    {
        printf("\n*****Switch to live mode.*****\n");
        if(bEsplayer)
        {
            printf("\tES Player to Live mode\n");
            _appDemo_PVR_PhyClose();
            DemoAV_TSP_StopPVRFlt(FALSE);
        }
        else
        {
            printf("\tBasic Player to Live mode\n");
            DemoAV_TSP_ResetPVRPid();
            DemoAV_Tuner_Config(&flow, &inSrc, &clkInv, &extSync, &parallel);
            bRet=DemoAV_TSP_SetPid(&u32VideoPid, &u32AudioPid, &u32PCRPid, &u32VCodec, &programInfo->u32ACodec);
        }


        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_FF2X, 0, 0);
        DemoAV_PlayAV();
        printf("***************************\n\n");

    }
    else
    {
        if(bEsplayer)
        {
            printf("\n*****Switch to Es Player mode.*****\n");
            DemoAV_TSP_StopPVRFlt(TRUE);
            _appDemo_PVR_FileModePlay(u32VCodec,programInfo->u32ACodec);
            _appDemo_PVR_AudioMute(FALSE,(!bIsLiveStream));
            printf("***************************\n");
            printf("\n");

        }
        else
        {
            printf("\n*****Switch to Basic Player mode.*****\n");
            DemoAV_TSP_ResetDTVPid();
            DemoAV_Tuner_Config(&flow, &inSrc, &clkInv, &extSync, &parallel);
            bRet=DemoAV_TSP_SetPVRPid(&u32VideoPid, &u32AudioPid, &u32PCRPid, &u32VCodec, &programInfo->u32ACodec);
            printf("***************************\n");
            printf("\n");

        }
    }


    return bRet;
}


static MS_BOOL _appDemo_PVR_AV_Play(MS_BOOL bVideoMute)
{
    MS_U16 u16FrameTHreshold= 2;
    MS_U32 u32Loop=400;
    MS_U32 u32StartTime=0 , u32EndTime=0;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    while(MApi_VDEC_EX_GetFrameCnt(stVDECStreamId)<u16FrameTHreshold)
    {
        u32Loop--;
        if(u32Loop<=0)
        {
           printf("TimeOut MApi_VDEC_EX_GetFrameCnt=0x%lx\n",MApi_VDEC_EX_GetFrameCnt(stVDECStreamId));
           break;
        }
        MsOS_DelayTask(10);
    }

    if(bVideoMute)
    {
        if(_bEnStillImageZapping)
        {
            if(!msAPI_GEGOP_IsStillVideoEnable())
            {
                MS_BOOL bEnable = TRUE;
                appDemo_XC_EnableStillImage(&bEnable);
            }
        }
        else /* Black Video*/
        {
            msAPI_VE_SetVideoMute(ENABLE);
            msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);
        }
    }

    msAPI_XC_EnableCCIRInput(0, DISABLE);



    //Check Video Decoder Status
    if( TRUE != appDemo_Video_GetDecoderStatus())
    {
        printf("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }


    u32StartTime = MsOS_GetSystemTime();

    //Set VDEC FRC
    msAPI_XC_DTV_SetMode();

    //Set Scaler(HD PATH)
    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW);

    //reset SD Capture window
    //_Demo_VE_ResetCapWindow();

    msAPI_VE_SetMode();

    //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
    MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);

    //Disable Blue Screen
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);

    if(_bEnStillImageZapping)
    {
        MS_BOOL bEnable = FALSE;
        appDemo_XC_EnableStillImage(&bEnable);
    }

    u32EndTime = MsOS_GetSystemTime();
    appDemoPvr_print("[%s][%d] DIFF TIME after MVOP and AV-Sync = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);
    appDemoPvr_print("########## XC Bluescreen DISABLE!!\n");

    return TRUE;
}


static void _appDemo_PVR_StopEvent(MS_BOOL bRecord,MS_U8 pvrRecIdx)
{
    appDemoPvr_print("[%s][%d]bRecord=%d,pvrRecIdx=%d,_u8TimeshiftIdx=%d\n",__FUNCTION__,__LINE__,bRecord,pvrRecIdx,_u8TimeshiftIdx);

    if(bRecord)
    {
        if(pvrRecIdx>=PVR_MAX_RECORDING_FILE)
        {
            printf("[%s][%s][%d]\n",__FILE__,__FUNCTION__,__LINE__);
            return ;
        }
        if(!MApi_PVR_IsRecording(_u8hRecord[pvrRecIdx]))
        {
            printf("[%s][%s][%d]  not recording _u8hRecord[%u]=%u\n",__FILE__,__FUNCTION__,__LINE__,pvrRecIdx,_u8hRecord[pvrRecIdx]);
            return ;
        }
        if(pvrRecIdx!=_u8TimeshiftIdx)
        {
            _u8CurProgramIdx = pvrRecIdx;
            appDemo_PVR_Record_Stop();
            printf("\nRecord Stop\n\n>>");
        }
        else
        {
            appDemo_PVR_Timeshift_RecordStop();
            printf("\nTimeshift Record Stop\n\n>>");
        }

    }
    else
    {
        if(!MApi_PVR_IsPlaybacking())
        {
            printf("[%s][%s][%d]  not playing\n",__FILE__,__FUNCTION__,__LINE__);
            return ;
        }
        if(!MApi_PVR_IsTimeShift())
        {
            if(MApi_PVR_PlaybackStop(&_u8hPlayback)==TRUE)
            {
                _appDemo_PVR_AV_Start(TRUE,&_livePromInfo,FALSE);
                _appDemo_PVR_AV_Play(TRUE);
            }
            printf("\nPlayback Stop\n\n>>");
        }
        else
        {
            appDemo_PVR_Timeshift_PlaybackStop();
            printf("\nTimeshift Playback Stop\n\n>>");
        }

   }

}


static void _appDemo_PVR_Callback(PVREventInfo_t *event)
{
    //MS_U32 u32Data[APIPVR_MAX_PROGRAM_NUM]={0};
    MS_U64 u64FreeSpaceInKB, u64TotalSpaceInKB;
    int idx = 0;
    static MS_U32 u32PlaybackTime = 0;
    static MS_U32 u32PlaybackSize = 0;
    static MS_U32 u32RecordTime[APIPVR_MAX_PROGRAM_NUM] = {0,0};
    static MS_U32 u32RecordSize[APIPVR_MAX_PROGRAM_NUM] = {0,0};
    //printf("pvrEvent->pvrEvent  %d\n",pvrEvent);
    char mountPath[72]={0};
    switch(event->pvrEvent)
    {
        case EN_APIPVR_EVENT_NOTIFY_FILE_END:
            appDemoPvrEvent_print("EN_APIPVR_EVENT_NOTIFY_FILE_END,_u8TimeshiftIdx=%d\n",_u8TimeshiftIdx);
            if((MApi_PVR_IsPlaybacking())&&(MApi_PVR_PlaybackGetSpeed(_u8hPlayback)<=EN_APIPVR_PLAYBACK_SPEED_1XFB) )
            {

                if((MApi_PVR_IsTimeShift())&&(_u8TimeshiftIdx!=0xFF))
                {
                    MApi_PVR_TimeshiftPlaybackStop(&_u8hPlayback,_u8TimeshiftIdx);
                    _appDemo_PVR_AudioMute(FALSE, TRUE);
                    _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();
                    MApi_PVR_EX_TimeshiftPlaybackStart(_u8CurPlaybackProgramPath,&_u8hPlayback,_u8CurPlaybackProgramPath,(void *)&_AVStreamID,_plyprogramInfo.FileName,0,0);
                }
                else
                {
                    MApi_PVR_PlaybackStop(&_u8hPlayback);
                    _appDemo_PVR_AudioMute(FALSE, TRUE);
                    _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();
                    MApi_PVR_EX_PlaybackStart(_u8CurPlaybackProgramPath,&_u8hPlayback,(void *)&_AVStreamID,_plyprogramInfo.FileName,0,0);
                }
            }
            else
            {
                _appDemo_PVR_StopEvent(FALSE,event->u32Data[0]);
            }
            break;
        case EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE:
            appDemoPvrEvent_print("EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE\n");
            appDemoPvrEvent_print("_u8TimeshiftIdx=%d\n",_u8TimeshiftIdx);
            for(idx = 0;idx < MAX_PROGRAM_NUM;idx++)
            {
                _appDemo_PVR_StopEvent(TRUE,idx);
            }
            _appDemo_PVR_StopEvent(FALSE,0xff);
            if(_bUplugMsg)
            {
                if(MsFS_Umount(_moutPath)==0)
                {
                    printf("Umount \"%s\" successfully!\n",_moutPath);
                }
                _bUplugMsg=FALSE;
            }
            break;
        case EN_APIPVR_EVENT_ERROR_READ_FILE_FAIL:
            _appDemo_PVR_StopEvent(FALSE,0xff);
            break;
        case EN_APIPVR_EVENT_ERROR_INVALID_ADDR:
        case EN_APIPVR_EVENT_ERROR_WRITE_FILE_FAIL:
            appDemoPvrEvent_print("[%s][%d]EN_APIPVR_EVENT_ERROR_WRITE_FILE_FAIL,%lu   _u8TimeshiftIdx=%u\n",__FUNCTION__,__LINE__,event->u32Data[0],_u8TimeshiftIdx);
            if(event->u32Data[0]>=MAX_PROGRAM_NUM)
            {
                printf("Invalid Program Indx  (%lu)\n",event->u32Data[0]);
                break;
            }
            _appDemo_PVR_StopEvent(TRUE,event->u32Data[0]);
            if(event->u32Data[0] ==_u8TimeshiftIdx)
            {
                _appDemo_PVR_GetRecMountPath(PVR_TIMESHIFT_PATH_IDX,&mountPath[0]);
            }
            else
            {
                _appDemo_PVR_GetRecMountPath(PVR_NORMALREC_PATH_IDX,&mountPath[0]);
            }

            if(_appDemo_PVR_GetDiskSpace(mountPath,&u64FreeSpaceInKB, &u64TotalSpaceInKB))
            {
                printf("Free Disk Space:%llu KB\n",u64FreeSpaceInKB);
            }
            break;

        case EN_APIPVR_EVENT_NOTIFY_RECORDED_TIME: //Second

            if(event->u32Data[0]>=MAX_PROGRAM_NUM)
            {
                printf("Invalid Program Indx  (%lu)\n",event->u32Data[0]);
                break;
            }
            idx = _appDemo_PVR_GetRecordIndex(event->u32Data[0]);
            u32RecordTime[idx] =event->u32Data[1];
            if(_bShowRecordTime)
            {
                printf("Record Time [%d]%8lu  %8lu\n",idx,u32RecordTime[idx],MApi_PVR_GetRecordTime(_u8hRecord[idx],_u8hRecord[idx]));
            }

#if 0
            if(_appDemo_PVR_GetDiskSpace((char *)MApi_PVR_GetMouthPath(),&u64FreeSpaceInKB, &u64TotalSpaceInKB))
            {
            	//appDemoPvrEvent_print("u64FreeSpaceInKB=%llu\n",u64FreeSpaceInKB/1024);

                if(u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
                {
                    printf("Out of Reserved Disk Space\n");
                    _appDemo_PVR_StopEvent(TRUE,event->pvrRecMode[0]);
                    _appDemo_PVR_StopEvent(TRUE,event->pvrRecMode[1]);
                }
            }
#endif
            break;

        case EN_APIPVR_EVENT_NOTIFY_RECORDED_SIZE:  //KByte
            appDemoPvrEvent_print("[%s][%d]program idx = %lu  record size=%lu KByte MApi_PVR_IsTimeShift()=%d\n",__FUNCTION__,__LINE__,event->u32Data[0],event->u32Data[1],MApi_PVR_IsTimeShift());
            if(event->u32Data[0]>=MAX_PROGRAM_NUM)
            {
                printf("Invalid Program Indx  (%lu)\n",event->u32Data[0]);
                break;
            }
            u32RecordSize[event->u32Data[0]] = event->u32Data[1];
#if 0
            /* see if an user needs */
            if(_appDemo_PVR_GetDiskSpace((char *)MApi_PVR_GetMouthPath(),&u64FreeSpaceInKB, &u64TotalSpaceInKB))
            {
            	//appDemoPvrEvent_print("u64FreeSpaceInKB=%llu\n",u64FreeSpaceInKB/1024);

                if(u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
                {
                    printf("Out of Reserved Disk Space\n");
                    _appDemo_PVR_StopEvent(TRUE,event->pvrRecMode[0]);
                    _appDemo_PVR_StopEvent(TRUE,event->pvrRecMode[1]);
                }
            }
#endif
            break;

        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_TIME:

            appDemoPvrEvent_print("[%s][%d]EN_APIPVR_EVENT_NOTIFY_PLAYBACK_TIME,%lu sec\n",__FUNCTION__,__LINE__,event->u32Data[1]);
            u32PlaybackTime = event->u32Data[1];
            if(_bShowPlayTime)
            {
                printf("Play Time [%02d:%02d:%02d] %8lu\n",(int)(u32PlaybackTime/3600),(int)((u32PlaybackTime%3600)/60),(int)((u32PlaybackTime%3600)%60),MApi_PVR_GetPlaybackTime(_u8hPlayback));
            }
            break;
        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SIZE:
            u32PlaybackSize = event->u32Data[1];
            break;
        case EN_APIPVR_EVENT_NOTIFY_FILE_BEGIN:
            appDemoPvrEvent_print("EN_PVR_EVENT_NOTIFY_FILE_BEGIN \n");
            MApi_PVR_PlaybackSetSpeed(_u8hPlayback,APIPVR_PLAYBACK_SPEED_1X);
            _appDemo_PVR_AudioMute(FALSE, TRUE);
            appDemoPvrEvent_print("SetSpeed APIPVR_PLAYBACK_SPEED_1X\n");
            break;
        case EN_APIPVR_EVENT_ERROR_SYNCBYTE_ERROR:
        case EN_APIPVR_EVENT_ERROR_OVERRUN:
        case EN_APIPVR_EVENT_NOTIFY_THUMBNAIL_AUTO_CAPTURE:
            break;
        default :
            appDemoPvrEvent_print("default  enEvent=%d\n",event->pvrEvent);
            break;
    }

}

static void _appDemo_PVR_Task(MS_U32 argc, void *argv)
{
    PVREventInfo_t pvrEvent;
    MS_U32 u32MessageSize ;
    memset((void *)&pvrEvent,0,sizeof(PVREventInfo_t));

    while(1)
    {
        if(TRUE==MsOS_RecvFromQueue(_s32QueueID, (MS_U8 *)&pvrEvent, sizeof(PVREventInfo_t), &u32MessageSize, MSOS_WAIT_FOREVER))
        {
            //printf("Recv pvrEvent %d,%d,%d\n",pvrEvent.pvrMode,pvrEvent.pvrEvent,pvrEvent.u32Data);
            _appDemo_PVR_Callback(&pvrEvent);
        }
    }

}


static MS_BOOL appDemo_PVR_Dmx_Shutdown(MS_U8 *pu8DmxId)
{

    if(pu8DmxId==NULL)
    {
        printf("Invalid DmxId\n");
        return FALSE;
    }

    if(*pu8DmxId==INVALID_FILTER_ID)
    {
        printf("Already Close DmxId=%u\n",*pu8DmxId);
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK!=MApi_DMX_Close( *pu8DmxId))
    {
        printf("Fail to MApi_DMX_Close with (%u)\n",*pu8DmxId);
        return FALSE;
    }
    *pu8DmxId=INVALID_FILTER_ID;

    return TRUE;


}

MS_BOOL _PVR_TSP_OpenPidFlt(MS_U8 u8Idx)
{
    int i=0;
    PVRProgramInfo_t *pPromInfo=&_recprogramInfo[u8Idx];
    MS_U16 u16VidPid=INVALID_PID;
    MS_U16 u16PCRPid=INVALID_PID;

    u16PCRPid=_appDemo_PVR_FindPidinFilter(pPromInfo->Filters,DMX_FILTER_TYPE_PCR);
    if (appDemo_PVR_Dmx_Setup(u16PCRPid,&_u8PcrDmxId[u8Idx]) != TRUE)
    {
        goto _PVR_TSP_OpenPidFlt_fail;
    }

    u16VidPid=_appDemo_PVR_FindPidinFilter(pPromInfo->Filters,DMX_FILTER_TYPE_VIDEO);
    if (appDemo_PVR_Dmx_Setup(u16VidPid,&_u8VidDmxId[u8Idx]) != TRUE)
    {
        goto _PVR_TSP_OpenPidFlt_fail;
    }

    for(i=0;i<APIPVR_MAX_AUDIOINFO_NUM;i++)
    {
        if(pPromInfo->AudioInfo[i].u16AudPID!=INVALID_PID)
        {
            if (appDemo_PVR_Dmx_Setup(pPromInfo->AudioInfo[i].u16AudPID,&_u8AudDmxId[u8Idx][i]) != TRUE)
            {
                goto _PVR_TSP_OpenPidFlt_fail;
            }
        }
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        if(pPromInfo->DVBSubtInfo[i].u16Sub_Pid!=INVALID_PID)
        {
            if (appDemo_PVR_Dmx_Setup(pPromInfo->DVBSubtInfo[i].u16Sub_Pid,&_u8DVBSubtDmxId[u8Idx][i]) != TRUE)
            {
                goto _PVR_TSP_OpenPidFlt_fail;
            }
        }
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        if(pPromInfo->EBUSubtInfo[i].u16TTX_Pid!=INVALID_PID)
        {
            if (appDemo_PVR_Dmx_Setup(pPromInfo->EBUSubtInfo[i].u16TTX_Pid,&_u8EBUSubtDmxId[u8Idx][i]) != TRUE)
            {
                goto _PVR_TSP_OpenPidFlt_fail;
            }
        }
    }

    if(pPromInfo->u16CCPid!=INVALID_PID)
    {
        if (appDemo_PVR_Dmx_Setup(pPromInfo->u16CCPid,&_u8CcDmxId[u8Idx]) != TRUE)
        {
            goto _PVR_TSP_OpenPidFlt_fail;
        }
    }

    printf("!!!!![%s][%d] u8PCRFltId[%d] PCRPid[%u] u8VideoFltId[%d] VideoPid[%u] u8AudioFltId[%d] AudioPid[%u]!!!!!\n",__FILE__,__LINE__,_u8PcrDmxId[u8Idx],u16PCRPid,_u8VidDmxId[u8Idx],u16VidPid,_u8AudDmxId[u8Idx][0],pPromInfo->AudioInfo[0].u16AudPID);

    return TRUE;
_PVR_TSP_OpenPidFlt_fail:
    printf("[%s][%d] fail\n", __FUNCTION__, __LINE__);
    if (_u8VidDmxId[u8Idx] != INVALID_FILTER_ID)
    {
        appDemo_PVR_Dmx_Shutdown(&_u8VidDmxId[u8Idx]);
    }

    for(i=0;i<APIPVR_MAX_AUDIOINFO_NUM;i++)
    {
        if (_u8AudDmxId[u8Idx][i] != INVALID_FILTER_ID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8AudDmxId[u8Idx][i]);
        }
    }

    if (_u8PcrDmxId[u8Idx] != INVALID_FILTER_ID)
    {
        appDemo_PVR_Dmx_Shutdown(&_u8PcrDmxId[u8Idx]);
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        if (_u8DVBSubtDmxId[u8Idx][i] != INVALID_FILTER_ID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8DVBSubtDmxId[u8Idx][i]);
        }
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        if (_u8EBUSubtDmxId[u8Idx][i] != INVALID_FILTER_ID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8EBUSubtDmxId[u8Idx][i]);
        }
    }

    if (_u8CcDmxId[u8Idx] != INVALID_FILTER_ID)
    {
        appDemo_PVR_Dmx_Shutdown(&_u8CcDmxId[u8Idx]);
    }
    return FALSE;
}



static MS_BOOL _PVR_TSP_ClosePidFlt(MS_U8 u8Idx)
{

    PVRProgramInfo_t *pPromInfo=&_recprogramInfo[u8Idx];
    int i=0;
    MS_U16 u16PID=INVALID_PID;

    u16PID=_appDemo_PVR_FindPidinFilter(pPromInfo->Filters,DMX_FILTER_TYPE_VIDEO);
    if(u16PID!=INVALID_PID)
    {
        appDemo_PVR_Dmx_Shutdown(&_u8VidDmxId[u8Idx]);
    }

    u16PID=_appDemo_PVR_FindPidinFilter(pPromInfo->Filters,DMX_FILTER_TYPE_PCR);
    if(u16PID!=INVALID_PID)
    {
        appDemo_PVR_Dmx_Shutdown(&_u8PcrDmxId[u8Idx]);
    }

    for(i=0;i<APIPVR_MAX_AUDIOINFO_NUM;i++)
    {
        if(pPromInfo->AudioInfo[i].u16AudPID!=INVALID_PID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8AudDmxId[u8Idx][i]);
        }
    }

    for(i=0;i<APIPVR_MAX_TTXINFO_NUM;i++)
    {
        if(pPromInfo->TXTInfo[i].u16TTX_Pid!=INVALID_PID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8TtxDmxId[u8Idx][i]);
        }
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        if(pPromInfo->DVBSubtInfo[i].u16Sub_Pid!=INVALID_PID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8DVBSubtDmxId[u8Idx][i]);
        }
    }
    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        if(pPromInfo->EBUSubtInfo[i].u16TTX_Pid!=INVALID_PID)
        {
            appDemo_PVR_Dmx_Shutdown(&_u8EBUSubtDmxId[u8Idx][i]);
        }
    }
    if(pPromInfo->u16CCPid!=INVALID_PID)
    {
        appDemo_PVR_Dmx_Shutdown(&_u8CcDmxId[u8Idx]);
    }

    _appDemo_PVR_ResetProgramInfo(&_recprogramInfo[u8Idx]);

    return TRUE;

}


static MS_BOOL appDemo_PVR_Dmx_Setup(MS_U16 u16PID,MS_U8 *pu8DmxId)
{

    DMX_FILTER_TYPE DmxFltType=0;
    DmxFltType=DMX_FILTER_TYPE_REC | DMX_FILTER_SOURCE_TYPE_LIVE;

    if(pu8DmxId==NULL)
    {
        printf("Invalid DmxId\n");
        return FALSE;
    }

    if(*pu8DmxId!=INVALID_FILTER_ID)
    {
        printf("Already Alloc DmxId=%u\n",*pu8DmxId);
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK!=MApi_DMX_Open(DmxFltType, pu8DmxId))
    {
        printf("MApi_DMX_Open Error with (%u)\n",u16PID);
        return FALSE;
    }

    if (DMX_FILTER_STATUS_OK!=MApi_DMX_Pid( *pu8DmxId, &u16PID,TRUE))
    {
        printf("MApi_DMX_Pid Error with (%u)\n",u16PID);

        return FALSE;
    }
    return TRUE;
}

static MS_BOOL _appDemo_PVR_InsertFilter(PVRFilterInfo_t *pFilter, MS_U16 u16PID, MS_U32 u32DmxFltType,MS_U32 *pu32Idx)
{
    PVRFilterInfo_t *pflt= (PVRFilterInfo_t *)(pFilter);
    MS_U32 u32Idx=*pu32Idx;
    if(pflt==NULL)
    {
        return FALSE;
    }

    if(u32Idx>APIPVR_MAX_PID_NUM)
    {
        return FALSE;
    }

    if(u16PID==INVALID_PID)
    {
        return FALSE;
    }

    appDemoPvr_print("[%s][%d] Insert u16PID=%u u32DmxFltType=%lu \n", __FUNCTION__, __LINE__,u16PID,u32DmxFltType);
    pflt[u32Idx].u16PID = u16PID;
    pflt[u32Idx].u32DmxFltType = u32DmxFltType;
    *pu32Idx=u32Idx+1;

    return TRUE;
}

static MS_U16 _appDemo_PVR_FindPidinFilter(PVRFilterInfo_t *pFilter,MS_U32 u32DmxFltType)
{
    int i=0;
    if(pFilter==NULL)
    {
        return INVALID_PID;
    }
    for(i=0;i<APIPVR_MAX_PID_NUM;i++)
    {
        if((pFilter[i].u32DmxFltType == u32DmxFltType)&&(pFilter[i].u16PID!=0))
        {
            appDemoPvr_print("[%s@%d] Find Pid : %u \n", __FUNCTION__, __LINE__,pFilter[i].u16PID);
            return pFilter[i].u16PID;
        }

    }
    return INVALID_PID;

}



static MS_BOOL _appDemo_PVR_ResetProgramInfo(PVRProgramInfo_t *pPromInfo)
{
    int i=0;
    appDemoPvr_print("%s@%d :: %p\n",__FUNCTION__,__LINE__,pPromInfo);

    memset(pPromInfo,0,sizeof(PVRProgramInfo_t));

    for(i=0;i<APIPVR_MAX_PID_NUM;i++)
    {
        pPromInfo->Filters[i].u16PID=INVALID_PID;
    }

    for(i=0;i<APIPVR_MAX_TTXINFO_NUM;i++)
    {
        pPromInfo->TXTInfo[i].u16TTX_Pid=INVALID_PID;
    }
    for(i=0;i<APIPVR_MAX_AUDIOINFO_NUM;i++)
    {
        pPromInfo->AudioInfo[i].u16AudPID=INVALID_PID;
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        pPromInfo->DVBSubtInfo[i].u16Sub_Pid=INVALID_PID;
    }
    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        pPromInfo->EBUSubtInfo[i].u16TTX_Pid=INVALID_PID;
    }
    pPromInfo->u16CCPid=INVALID_PID;
    return TRUE;
}

static MS_U32 _appDemo_PVR_ADEC_Fmt(MS_U32 u32ACodec)
{
    switch (u32ACodec)
    {
        case 0x01:
            return EN_APIPVR_AUD_DVB_MPEG;
        case 0x02:
            return EN_APIPVR_AUD_DVB_AC3;
        case 0x03:
            return EN_APIPVR_AUD_DVB_AC3P;
        case 0x04:
            return EN_APIPVR_AUD_DVB_AAC;
        default:
            break;
    }
    return EN_APIPVR_AUD_DVB_NONE;
}

static APIPVR_CODEC_TYPE _appDemo_PVR_VDEC_Fmt(MS_U32 u32VCodec)
{
    switch (u32VCodec)
    {
        case 0x00:
            return EN_APIPVR_CODEC_TYPE_MPEG2;
        case 0x01:
            return EN_APIPVR_CODEC_TYPE_H264;
        case 0x02:
            return EN_APIPVR_CODEC_TYPE_AVS;
        default:
            break;
    }
    return EN_APIPVR_CODEC_TYPE_NONE;
}


static MS_BOOL _appDemo_PVR_SetProgramInfo(PVRProgramInfo_t *pPromInfo,MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32VCodec, MS_U32 u32ACodec,MS_U32 u32PmtPid, MS_U32 u32LCN)
{

    MS_U32 u32Idx=0;

    if(pPromInfo==NULL)
    {
        printf( "Invalid PromInfo!\n");
        return FALSE;
    }
    _appDemo_PVR_ResetProgramInfo(pPromInfo);

    if(u32VideoPid==INVALID_PID)
    {
        pPromInfo->enServiceType = EN_APIPVR_SERVICETYPE_RADIO;
    }
    else
    {
        pPromInfo->enServiceType = EN_APIPVR_SERVICETYPE_DTV;
    }
    pPromInfo->u16AudioPid = u32AudioPid;
    pPromInfo->u32ACodec = _appDemo_PVR_ADEC_Fmt(u32ACodec);
    pPromInfo->enVCodec = _appDemo_PVR_VDEC_Fmt(u32VCodec);

    _appDemo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)u32VideoPid, (MS_U32)DMX_FILTER_TYPE_VIDEO,&u32Idx);
    _appDemo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)u32PCRPid, (MS_U32)DMX_FILTER_TYPE_PCR,&u32Idx);
    _appDemo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)u32PmtPid, (MS_U32)DMX_FILTER_TYPE_SECTION,&u32Idx);

    pPromInfo->u32LCN       = u32LCN;
    pPromInfo->u32StartTime =MsOS_GetSystemTime();
    //First ACodec
    pPromInfo->u8AudioInfoNum = 1;
    pPromInfo->AudioInfo[0].u16AudPID = (MS_U16)u32AudioPid;
    pPromInfo->AudioInfo[0].u8AudType = (MS_U16)u32ACodec;


#ifdef PVR_DBG
    appDemoPvr_print("========= Start =========  %d\n",_u8CurProgramIdx);
    appDemoPvr_print("[%d]::\n",_u8CurProgramIdx);
    appDemoPvr_print("u32ACodec=%lu\n",pPromInfo->u32ACodec);
    appDemoPvr_print("u32AudioPid=%ld\n",pPromInfo->u16AudioPid);
    appDemoPvr_print("u32LCN=%ld\n",pPromInfo->u32LCN);
    appDemoPvr_print("u32VCodec=%d\n",pPromInfo->enVCodec);
    appDemoPvr_print("========= End ========= %d\n",_u8CurProgramIdx);
#endif

#if ENABLE_CAPVR /*ENABLE_CAPVR*/
    pPromInfo->bIsEncrypted = TRUE;
    pPromInfo->enEncryptionType     = EN_APIPVR_ENCRYPTION_SMARTCARD;
#else
    pPromInfo->bIsEncrypted = FALSE;
    pPromInfo->enEncryptionType     = EN_APIPVR_ENCRYPTION_NONE;
#endif
    //Insert as last item;
    _appDemo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)0x00, (MS_U32)DMX_FILTER_TYPE_SECTION,&u32Idx);

    return TRUE;
}




static MS_BOOL _appDemo_PVR_RecStart(MS_BOOL bTimeshift, MS_U32 u32FreeSizeInMB)
{
    MS_BOOL bRet=FALSE;
    MS_BOOL bRecordAll=FALSE;
    MS_U64 u64FreeSpaceInKB=0;
    DMX_PVR_ENG ePVREng = DMX_PVR_EGN0;


    // [01] Set dmx flow input for pvr record engine  ==============================================//

    switch (_u8CurProgramIdx)
    {
        case 0:
            ePVREng = DMX_PVR_EGN0;
            break;
        case 1:
            ePVREng = DMX_PVR_EGN1;
            break;
        default :
            printf("Wrong PVR Eng\n");
            return FALSE;
     }



    //Connect PVR's SRC to tsif0
    printf(" \" DMX_PVR_FLOWSET \"\n");
    MApi_DMX_PVR_FlowSet(ePVREng,DMX_TSIF_LIVE0,FALSE);



    // [02] Start recording  ==============================================//
    if(bTimeshift)
    {
        u64FreeSpaceInKB=u32FreeSizeInMB<<10;
        MApi_PVR_FreezeScreenInTimeshiftRecord(EN_APIPVR_ACTION_TYPE_SET, &_bScreenFrozen);
        MApi_PVR_SetTimeshiftFileSize(u64FreeSpaceInKB);//limited to 4G
        _appDemo_PVR_AudioMute(_bScreenFrozen, FALSE);
        _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();

        bRet = MApi_PVR_EX_TimeshiftRecordEngSet(&_u8hRecord[_u8CurProgramIdx],(void *)&_AVStreamID,_u8CurProgramIdx,_u8CurPlaybackProgramPath);
        //set key in here ---->

        MApi_PVR_EX_TimeshiftRecordEngEnable(&_u8hRecord[_u8CurProgramIdx]);
        appDemoPvr_print("[%s][%d] timeshift record start [%d]=%d bRet=%d\n",__FUNCTION__,__LINE__,_u8CurProgramIdx,_u8hRecord[_u8CurProgramIdx],bRet);

    }
    else
    {
        _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();

        //Dscmb Flow
        bRet = MApi_PVR_EX_RecordEngSet(&_u8hRecord[_u8CurProgramIdx],(void *)&_AVStreamID,_u8CurProgramIdx, APIPVR_FILE_LINEAR, u32FreeSizeInMB,0);
        //set key in here ---->

        MApi_PVR_EX_RecordEngEnable(&_u8hRecord[_u8CurProgramIdx],bRecordAll);
        appDemoPvr_print("[%s][%d] record start %d\n",__FUNCTION__,__LINE__,_u8hRecord[_u8CurProgramIdx]);
    }

    return bRet;


}

/// Using this function to stop recording!!!!!!!!!!!!!
static MS_BOOL _appDemo_PVR_RecStop(MS_BOOL bTimeshift, MS_U8 u8Idx)
{
    MS_BOOL bRet=FALSE;
    if(!PVR_IS_VALID_HANDLER(_u8hRecord[u8Idx]))
    {
        printf("[%s][%d] Already stop recording %d\n",__FUNCTION__,__LINE__,_u8hRecord[u8Idx]);
        return TRUE;
    }
    if(!MApi_PVR_IsRecording(_u8hRecord[u8Idx]))
    {
        printf("Idx %d stopped recording already!\n",u8Idx);
        return FALSE;
    }

    if(bTimeshift)
    {
        _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();
        bRet=MApi_PVR_EX_TimeshiftRecordStop(&_u8hRecord[u8Idx],(void *)&_AVStreamID,&_u8hPlayback,PVR_DEFAULT_PROGRAM_IDX,_u8CurPlaybackProgramPath);
        _appDemo_PVR_AudioMute(FALSE,MApi_PVR_IsPlaybacking());
        appDemoPvr_print("[%s][%d] timeshift record stop %d\n",__FUNCTION__,__LINE__,_u8hRecord[u8Idx]);
    }
    else
    {
        bRet=MApi_PVR_RecordStop(&_u8hRecord[u8Idx],PVR_DEFAULT_PROGRAM_IDX);
        appDemoPvr_print("[%s][%d] record stop %d\n",__FUNCTION__,__LINE__,_u8hRecord[u8Idx]);
    }
    _PVR_TSP_ClosePidFlt(u8Idx);

    return bRet;
}


static MS_BOOL _appDemo_PVR_StoreProgramInfo(MS_BOOL bTimeshift,MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32ACodec, MS_U32 u32VCodec , char *fileName)
{
    MS_U16 u16PmtPid=INVALID_PID;
    MS_U16 u16LCN=0xFFFF;
    char FileName[128];
    MS_U8 enIdx=_u8CurProgramIdx;

    // [01] Get relative PID info ==============================================//
    if(appDemo_Dmx_GetCurProgram(u32PCRPid,u32VideoPid,u32AudioPid,&u16PmtPid,&u16LCN) == FALSE)
    {
        appDemoPvr_print("fail to appDemo_Dmx_GetCurProgram.\n");
        return FALSE;
    }


    enIdx=_u8CurProgramIdx;

    if(bTimeshift)
    {
        if(FALSE==_appDemo_PVR_SetProgramInfo(&_livePromInfo,u32PCRPid, u32VideoPid, u32AudioPid, u32VCodec, u32ACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN))
        {
            printf("Can't store program info!\n");
            return FALSE;
        }
    }


    if(FALSE==_appDemo_PVR_SetProgramInfo(&_recprogramInfo[_u8CurProgramIdx],u32PCRPid, u32VideoPid, u32AudioPid, u32VCodec, u32ACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN))
    {
        printf("Can't set program info!\n");
        return FALSE;
    }

    strcpy(FileName,fileName);



    if(strlen(_recFSInfo[_u8CurProgramIdx].recPath)>0)
    {
        sprintf(FileName,"%s/%s",_recFSInfo[_u8CurProgramIdx].recPath,fileName);
    }


    strcpy(_recprogramInfo[_u8CurProgramIdx].FileName,FileName);

    appDemoPvr_print(" _u8CurProgramIdx=%d   fileName=%s\n",_u8CurProgramIdx,_recprogramInfo[_u8CurProgramIdx].FileName);

#if ENABLE_CAPVR
    _recprogramInfo[_u8CurProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_SMARTCARD;
#else
    _recprogramInfo[_u8CurProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_NONE;
#endif


    // [02] Store meta data ==============================================//

    if(!MApi_PVR_SetMetaData(&_recprogramInfo[_u8CurProgramIdx],enIdx,0,0,0,0))
    {
        printf("Can't set program info!\n");
        return FALSE;
    }

    _PVR_TSP_OpenPidFlt(_u8CurProgramIdx);

    return TRUE;
}

static MS_BOOL _appDemo_PVR_RecordStart(MS_BOOL bTimeshift, MS_U32 u32FreeSizeInMB,const MS_U32* VideoPid,const  MS_U32* AudioPid,const  MS_U32* PCRPid,const  MS_U32* pu32VCodec,const  MS_U32* pu32ACodec, char* fileName)
{


    // [01] Check if  available ==============================================//
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if(PVR_IS_VALID_HANDLER(_u8hRecord[_u8CurProgramIdx]))
    {
        printf("Already record in idx:%d!\n",_u8CurProgramIdx);
        return FALSE;

    }

    // [02] Store Current Program info ==============================================//

    if(!_appDemo_PVR_StoreProgramInfo(bTimeshift,*PCRPid, *VideoPid, *AudioPid, *pu32ACodec, *pu32VCodec , fileName))
    {
        printf("Fail to store program info!\n");
        return FALSE;
    }

    appDemoPvr_print(" _u8TotalProgramNum=%d\n",_u8TotalProgramNum);

    // [03] Record  ==============================================//
    return _appDemo_PVR_RecStart( bTimeshift,  u32FreeSizeInMB);

}


static MS_BOOL _appDemo_PVR_GetFileInfo(char *FileName, PVRProgramInfo_t *promInfo)
{
    MS_U32 u32VideoPid=INVALID_PID;
    MS_U32 u32AudioPid=INVALID_PID;
    MS_U32 u32PCRPid=INVALID_PID;


    char AudioFmtStr[][21]={"SiNOT_VALID","SiMPEG","SiAC3","SiAC3P","SiAAC"};
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if(promInfo==NULL)
    {
        printf("Invalid promInfo\n");
        return FALSE;
    }

    appDemoPvr_print("memset program info\n");

    _appDemo_PVR_ResetProgramInfo(promInfo);

    if(MApi_PVR_GetProgramInfo(FileName,promInfo)==FALSE)
    {
        printf("GetFileInformation fail!\n");
        return FALSE;
    }
    u32VideoPid=_appDemo_PVR_FindPidinFilter(promInfo->Filters, DMX_FILTER_TYPE_VIDEO);
    u32AudioPid=promInfo->u16AudioPid;
    u32PCRPid=_appDemo_PVR_FindPidinFilter(promInfo->Filters, DMX_FILTER_TYPE_PCR);
    printf("\n***************************\n\n");
    printf("\tvideo pid = 0x%lx\n",u32VideoPid);
    printf("\taudio pid = 0x%lx\n",u32AudioPid);
    printf("\tpcr   pid = 0x%lx\n",u32PCRPid);
    printf("\t264       = %s\n",(_appDemo_PVR_IsH264(promInfo->enVCodec)==1)?"TRUE":"FALSE");
    printf("\tAudioFmt  = %s\n",AudioFmtStr[promInfo->u32ACodec]);
    printf("\tduration  = %lu sec\n",promInfo->u32Duration);
    printf("\tFile Len  = %llu Bytes\n",promInfo->u64FileLength);
    printf("\tLast Time = %lu sec\n",promInfo->u32LastPlayPositionInSec);
    printf("\tAge       = %u \n",promInfo->u8Age);
    printf("\tLock      = %u \n",promInfo->bLocked); // lock by user and it cannot be deleted even if disk is full
    printf("\n***************************\n\n");
    return TRUE;
}


static MS_BOOL _appDemo_PVR_AudioMute(MS_BOOL bMute, MS_BOOL bPvrPlayback)
{

    if(bPvrPlayback)
    {
        DemoAV_TSP_SetPVRAudioPid(&_plyprogramInfo.u16AudioPid,FALSE);
    }

    if(_gbMute!=bMute)
    {
        if(!bMute)
        {
            _appDemo_PVR_InitAudioInfo(FALSE,FALSE, _plyprogramInfo.u32ACodec,TRUE,TRUE);
        }

        appDemo_Audio_SetMute(&bMute);

        _gbMute=bMute;
        printf("[%s][%d]  %s\n",__FUNCTION__,__LINE__,(bMute)?"Mute":"!Mute");
    }

    return TRUE;
}

static APIPVR_PLAYBACK_SPEED _appDemo_PVR_GetSlowForward(APIPVR_PLAYBACK_SPEED enPlaybackSpeed)
{
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;

    switch(enPlaybackSpeed)
    {
        case EN_APIPVR_PLAYBACK_SPEED_1X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_2X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_2X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_4X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_4X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_8X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_8X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_16X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_16X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_32X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_32X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            break;
        default :
            break;
    }
    return enSpeed;

}

static APIPVR_PLAYBACK_SPEED _appDemo_PVR_GetFastForward(APIPVR_PLAYBACK_SPEED enPlaybackSpeed)
{
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;

    switch(enPlaybackSpeed)
    {
        case EN_APIPVR_PLAYBACK_SPEED_32XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_16XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_16XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_8XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_8XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_4XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_4XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_2XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_2XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_1X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_2X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_4X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_8X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_16X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_32X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_2XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_2XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_4XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_4XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_8XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_8XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_16XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_16XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_32XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_32XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_32XFF;
            break;
        default :
            break;
    }
    return enSpeed;

}


static APIPVR_PLAYBACK_SPEED _appDemo_PVR_GetFastBackward(APIPVR_PLAYBACK_SPEED enPlaybackSpeed)
{
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;
    switch(enPlaybackSpeed)
    {
        case EN_APIPVR_PLAYBACK_SPEED_32XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_32XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_16XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_32XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_8XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_16XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_4XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_8XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_2XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_4XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_1X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_2X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_4X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_8X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_16X:
        case EN_APIPVR_PLAYBACK_SPEED_FF_1_32X:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_2XFB;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_2XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_4XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_2XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_8XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_4XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_16XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_8XFF;
            break;
        case EN_APIPVR_PLAYBACK_SPEED_32XFF:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_16XFF;
            break;
        default :
            break;
    }


    return enSpeed;


}



static MS_BOOL _appDemo_PVR_MemAlloc(void)
{
    if(!_appDemo_PVR_AllocateMem(PVR_ID_DOWNLOAD))
    {
        return FALSE;
    }

    if(!_appDemo_PVR_AllocateMem(PVR_ID_UPLOAD))
    {
        return FALSE;
    }

    if(!_appDemo_PVR_AllocateMem(PVR_ID_BUFFER))
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL appDemo_PVR_USB_UnplugEvent(char *moutPath)
{
    PVREventInfo_t event_info;
    memset((void *)&event_info,0,sizeof(PVREventInfo_t));

    if(_bPvrInit)
    {
        if(strstr(MApi_PVR_GetMouthPath(),moutPath)!=NULL)
        {
            event_info.pvrEvent = EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE;
            _bUplugMsg=TRUE;
            strcpy(_moutPath,moutPath);
            MsOS_SendToQueue(_s32QueueID, (MS_U8 *)&event_info, sizeof(PVREventInfo_t), 1000);
            appDemoPvr_print("send to Queue <2> _u8TimeshiftIdx=%d\n",_u8TimeshiftIdx);
            return TRUE;
        }
    }
    return FALSE;
}

MS_BOOL _appDemo_PVR_SetTrickMode(MS_U32 type, void *cmd)
{
    switch(type)
    {
        case EN_DEMOPVR_TRICK_TYPE_FORCEMOTION:
            return _appDemo_PVR_SetMotionPatch((MS_BOOL *)cmd);
        break;
        case EN_DEMOPVR_TRICK_TYPE_PIDSWITCH:
            return _appDemo_PVR_PIDSWITCH_FUNC((PVR_PIDSWITCH_SET *)cmd);
        break;
        default :
        appDemoPvr_print("Error type %d\n",type);
        break;

    }
    return FALSE;
}

MS_BOOL _appDemo_PVR_SetMotionPatch(MS_BOOL *bEnable)
{

    MS_PQ_RFBL_INFO stRFBLInfo;
    //MS_BOOL bRfbl = FALSE;//marked by teddy.chen
    MDrv_PQ_GetRFblMode(&stRFBLInfo);
    //bRfbl = MDrv_PQ_GetRFblMode();//marked by teddy.chen
    MDrv_PQ_MADiForceMotionY(PQ_MAIN_WINDOW, *bEnable);
    appDemoPvr_print("[%s]-[%d] : Y/Cmotion : ( %d , x ) \n", __FUNCTION__, __LINE__,bEnable);

    if((!stRFBLInfo.bEnable) || (stRFBLInfo.bFilm))
    {
        MDrv_PQ_MADiForceMotionC(PQ_MAIN_WINDOW, *bEnable);
        appDemoPvr_print("[%s]-[%d] : Y/Cmotion : ( %d , %d ) \n", __FUNCTION__, __LINE__,bEnable,bEnable);
    }
    if(*bEnable)
    {
        if(MApi_VDEC_EX_SetControl(appDemo_Video_GetStreamID(),E_VDEC_EX_USER_CMD_DISP_ONE_FIELD, 1) != E_VDEC_EX_OK)
        {
            return FALSE;
        }
        MDrv_MVOP_SEL_OP_FIELD(TRUE);
    }
    else
    {
        if(MApi_VDEC_EX_SetControl(appDemo_Video_GetStreamID(),E_VDEC_EX_USER_CMD_DISP_ONE_FIELD, 0) != E_VDEC_EX_OK)
        {
            return FALSE;
        }
        MDrv_MVOP_SEL_OP_FIELD(FALSE);
    }
    return TRUE;

}

MS_BOOL _appDemo_PVR_PIDSWITCH_FUNC(PVR_PIDSWITCH_SET *PidSwitch_Info)
{
    if(PidSwitch_Info==NULL)
    {
        printf("ERROR Pid Switch Failed!!!");
        return FALSE;
    }

    MS_U8 PIDType = PidSwitch_Info->PIDType;
    MS_BOOL isOpen = PidSwitch_Info->isOpen;

    MS_U16 u16PCRPid=INVALID_PID;
    PVRProgramInfo_t *pPromInfo=&_plyprogramInfo;
    if(PIDType == DEMOPVR_PCR_PID)
    {
        if(isOpen == TRUE)
        {
            printf("[%s][%d]Open PCR FILTER!!!!\n", __FUNCTION__, __LINE__);
            u16PCRPid = _appDemo_PVR_FindPidinFilter(pPromInfo->Filters,DMX_FILTER_TYPE_PCR);
            DualAV_StartPCRFLT(&u16PCRPid);
        }
        else
        {
            printf("[%s][%d]CLOSE PCR FILTER!!!!\n", __FUNCTION__, __LINE__);
            DualAV_StopPCRFLT();
        }
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize pvr.
/// @param[in] u32FSType The type of file system
///         0: UNKNOWN
///         1: JFFS2
///         2: VFAT
///         3: EXT2
///         4: EXT3
///         5: MSDOS
///         6: NTFS
/// @param[in] pMountPath The mount path of an USB storage
/// @param[in] subTimeshiftRecPath The mount path of an USB storage
/// @param[in] subRecPath The mount path of an USB storage
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Init \n
///
/// Sample Command:PVR_Init 0 /mnt/tmp
//------------------------------------------------------------------------------
MS_BOOL appDemo_PVR_Init(MS_U32 *u32FSType, char *pMountPath, char *subTimeshiftRecPath, char *subRecPath)
{
    appDemoPvr_print("========= appDemo_PVR_Init =========\n");
    PVRFuncInfo_t pvrFunc={NULL};
    DMX_FLOW_INPUT DmxFlowInput;
    PVR_TSPInfo_t pvrTspInfo;
    char path[64]={0};
    int i=0;
    MS_U8 PlaybackPathNum = 1;

#if (DMX_INPUT == DMX_INPUT_DEMOD)
    DmxFlowInput = DMX_FLOW_INPUT_DEMOD;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT0)
    DmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT1)
    DmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
#else
    DmxFlowInput = DMX_FLOW_INPUT_DEMOD;
#endif

    // [00] check display path  ==============================================//
    if(_bPvrInit)
    {
        printf( "Already init PVR!\n");
        return FALSE;
    }
    if(pMountPath==NULL)
    {
        printf("No Such Mounting Path\n");
        return FALSE;
    }
    if(strlen(pMountPath)<=0)
    {
        printf("No Such Path\n");
        return FALSE;
    }
    _recFSInfo[0].recPath[0]='\0';
    _recFSInfo[1].recPath[0]='\0';

    if((subTimeshiftRecPath!=NULL)&&(strlen(subTimeshiftRecPath)>0))
    {
        strcpy(_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath,subTimeshiftRecPath);
    }

    if((subRecPath!=NULL)&&(strlen(subRecPath)>0))
    {
        strcpy(_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath,subRecPath);
    }



    appDemoPvr_print("========= PVR Init =========\n");


    MsFS_Init(FALSE);
    // [01] Init PVR  ==============================================//
    appDemoPvr_print("enFSType=%lu pMountPath=%s\n",*u32FSType,pMountPath);
    pvrTspInfo.u16DmxFlowInput =(MS_U16) DmxFlowInput;
    pvrTspInfo.bClkInv = FALSE;
    pvrTspInfo.bExtSync = TRUE;
    pvrTspInfo.bParallel = TRUE;
    if(!MApi_PVR_Init(pMountPath,(EN_APIPVR_FILE_SYSTEM_TYPE)*u32FSType,PVR_MAX_RECORDING_FILE,pvrTspInfo))
    {
        printf("Fail to Init PVR!\n");
        goto PVRInit_Fail;
    }

    if(!MApi_PVR_SetPlaybackPathNum(PlaybackPathNum))
    {
        printf("Fail to Set PlaybackPathNum!\n");
        return FALSE;
    }

    if(strlen(_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath)>0)
    {
        _appDemo_PVR_GetRecMountPath(TRUE,path);
        if(! _appDemo_PVR_CheckUSB(path))
        {
            printf("No USB Disk Deteded!\n");
            return FALSE;
        }
    }



    if(strlen(_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath)>0)
    {
        _appDemo_PVR_GetRecMountPath(FALSE,path);
        if(! _appDemo_PVR_CheckUSB(path))
        {
            printf("No USB Disk Deteded!\n");
            return FALSE;
        }
    }

    // [02] Mem Layout: The base address/length of physical memory in MApi_PVR_Mmap ==============//
    if(!_appDemo_PVR_MemAlloc())
    {
        printf("Mem Alloc fail\n");
        goto PVRInit_Fail;
    }
    appDemoPvr_print("\n");

    // [03] Callback Function for PVR ==============//

    pvrFunc.ALLOC=_appDemo_PVR_MEMALLOC_FUNC;
    pvrFunc.FREE=_appDemo_PVR_MEMFREE_FUNC;
    pvrFunc.PA2VA=_appDemo_PVR_MEMPA2VA_FUNC;
    pvrFunc.MemFlush=_appDemo_PVR_MEMFLUSH_FUNC;
    pvrFunc.KLADDER =_appDemo_PVR_KLADDER_FUNC;
    pvrFunc.TrickSetting =_appDemo_PVR_SetTrickMode;
    MApi_PVR_SetFuncInfo(pvrFunc);


    // [04] Recorder and Player type ==============//
    MApi_PVR_SetRecordType(EN_APIPVR_RECORD_TYPE_SINGLE);
    MApi_PVR_SetPlaybackType(EN_APIPVR_PLAYBACK_TYPE_BASICPLAYER);
    MApi_PVR_SetSyncInfo( 180, 66);
    appDemoPvr_print(" DmxFlowInput = %x\n", DmxFlowInput );

    // [05] Create Message queue to receive info ==============//
    MApi_PVR_SetMsgWaitMs(0);
    _s32QueueID = MApi_PVR_GetMessageQueueID();
    if(_s32ThreadID==-1)
    {
        _s32ThreadID=MsOS_CreateTask(_appDemo_PVR_Task,0,_pvrEventTask.ePriority,TRUE,_pvrEventTask.pStack,_pvrEventTask.u32StackSize,_pvrEventTask.szName);
        if (-1 ==_s32ThreadID)
        {
            printf("Create Task fail\n");
            goto PVRInit_Fail;
        }
    }
    // [06] Init PVR relative info ==============//
    _bPvrInit = TRUE;
    _u8TotalProgramNum=0;
    _u8CurProgramIdx=0;
    _u8TimeshiftIdx=0xFF;
    _bUplugMsg=FALSE;
    _moutPath[0]='\0';
    _u8hPlayback=APIPVR_INVALID_HANDLER;
    memset(&_livePromInfo,0,sizeof(PVRProgramInfo_t));
    memset(&_plyprogramInfo,0,sizeof(PVRProgramInfo_t));
    for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
    {
         _u8hRecord[i]=APIPVR_INVALID_HANDLER;
         memset(&_recprogramInfo[i],0,sizeof(PVRProgramInfo_t));
    }
    _gWinInfo.GopDest=E_GOP_DST_OP0;
    _gWinInfo.GeWinId=0xff;
    _gWinInfo.u8FBId=0xff;
    _gWinInfo.GeDstFmt=E_MS_FMT_ARGB4444;
    _gWinInfo.GopBitmapWidth=PVR_THUMBNAIL_DISPLAY_WIDTH;
    _gWinInfo.GopBitmapHeight=PVR_THUMBNAIL_DISPLAY_HEIGHT;
    //Audio Issue
    //If not initial Audio, file play or record without play live would core dump
    //need to fixed
    if(!MDrv_AUDIO_GET_INIT_FLAG())
    {
         MApi_AUDIO_Initialize();
    }
    printf("\n***************************\n");
    printf("PVR Info:  \n");
    _appDemo_PVR_GetRecMountPath(TRUE,path);
    printf("\tTimeshift Path:%s  \n",path);
    _appDemo_PVR_GetRecMountPath(FALSE,path);
    printf("\tNormal Path:%s  \n",path);
    printf("\n***************************\n");
    printf("Init PVR successfully  \n");
    return TRUE;

PVRInit_Fail:
    appDemo_PVR_Destroy();
    return FALSE;

}




//------------------------------------------------------------------------------
/// @brief The sample code to exit pvr
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Exit \n
///
/// Sample Command:PVR_Exit
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_Destroy(void)
{
    appDemoPvr_print("========= appDemo_PVR_Destroy =========\n");
    int i=0;

    if(!_bPvrInit)
    {
        printf( "Not init PVR!\n");
        return FALSE;
    }

    if(MApi_PVR_Destroy())
    {
        _bPvrInit= FALSE;
        _s32QueueID = INVALID_TASK_ID;
        if(MsOS_DeleteTask(_s32ThreadID))
        {
            _s32ThreadID = INVALID_TASK_ID;
        }
        //*Init relative info*//
        _u8TotalProgramNum=0;
        _u8CurProgramIdx=0;
        _u8TimeshiftIdx=0xFF;
        _bUplugMsg=FALSE;
        _moutPath[0]='\0';
        _u8hPlayback=APIPVR_INVALID_HANDLER;
        memset(&_livePromInfo,0,sizeof(PVRProgramInfo_t));
        memset(&_plyprogramInfo,0,sizeof(PVRProgramInfo_t));
        for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
        {
             _u8hRecord[i]=APIPVR_INVALID_HANDLER;
             memset(&_recprogramInfo[i],0,sizeof(PVRProgramInfo_t));
        }

        _appDemo_PVR_FreeMem(PVR_ID_DOWNLOAD);
        _appDemo_PVR_FreeMem(PVR_ID_UPLOAD);
        _appDemo_PVR_FreeMem(PVR_ID_BUFFER);

        printf("Destroy PVR successfully\n");
        return TRUE;
    }
    printf("Destroy PVR failed!\n");
    return FALSE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to record a live stream
/// @param[in] VideoPid Video PID of the current program
/// @param[in] AudioPid Audio PID of the current program
/// @param[in] PCRPid PCR PID of the current program
/// @param[in] pu32VCodec Video Codec  of the current program  0 is for MPEG2. 1 is for H264
/// @param[in] pu32ACodec Audio Codec  of the current program
///         1: SiMPEG
///         2: SiAC3
///         3: SiAC3P
///         4: SiAAC
/// @param[in] fileName  file name
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RecordStart \n
///
/// Sample Command:PVR_RecordStart
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_Record_Start(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char *fileName)
{
    /* For detect USB space */
    MS_U64 u64FreeSpaceInKB, u64TotalSpaceInKB;
    char mountPath[72]={0};
    MS_BOOL bRet = FALSE;
    appDemoPvr_print("========= PVR Record Start =========\n");
    appDemoPvr_print("(%lu,%lu,%lu,%lu,%lu)\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);


    if(_u8CurProgramIdx>=_u8TotalRecorder)
    {
        printf("Invalid Recorder Idx\n");
        return FALSE;
    }


    if( _appDemo_PVR_IsFileExist(MApi_PVR_GetMouthPath(),fileName))
    {
        printf("%s exist.\n",fileName);
        return FALSE;
    }


    appDemoPvr_print("<2> _appDemo_PVR_GetDiskSpace\n");
    _appDemo_PVR_GetRecMountPath(FALSE,&mountPath[0]);
    if(!_appDemo_PVR_GetDiskSpace(mountPath,&u64FreeSpaceInKB,&u64TotalSpaceInKB))
    {
        printf("Disk Space can't be detected! \n");
        return FALSE;
    }
    if(u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
    {
        printf("Disk Space is not enough! (less than %dMB)\n",PVR_FILE_SIZE_TO_STOP/1024);
        return FALSE;
    }



    bRet = _appDemo_PVR_RecordStart(FALSE,  u64FreeSpaceInKB/1024,VideoPid, AudioPid, PCRPid, pu32VCodec, pu32ACodec, fileName);

    if(bRet)
    {
        _u8TotalProgramNum++;
        // display info  ==============================================//
        printf("\n***** Normal Recorder *****\n");
        printf("File Name: %s\n",fileName);
        printf("Program Info:\n");
        printf("\t Video PID: 0x%lX\n",*VideoPid);
        printf("\t Audio PID: 0x%lX\n",*AudioPid);
        printf("\t PCR   PID: 0x%lX\n",*PCRPid);
        printf("\t Video Dec: %lu\n",*pu32VCodec);
        printf("\t Audio Dec: %lu\n",*pu32ACodec);
        printf("***************************\n");
        printf("\n");
        printf("Normal Record Successfully.\n");
    }
    appDemoPvr_print(" MApi_PVR_RecordStart _u8TotalProgramNum=%d\n",_u8TotalProgramNum);
    return bRet;

}

//------------------------------------------------------------------------------
/// @brief The sample code to stop recording a live stream
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RecordStop \n
///
/// Sample Command:PVR_RecordStop
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Record_Stop(void)
{
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if(_u8TimeshiftIdx==_u8CurProgramIdx)
    {
        printf("Error!! Timeshift Mode! %d\n",_u8TimeshiftIdx);
        return FALSE;
    }
    appDemoPvr_print("appDemo_PVR_Record_Stop %d ::%s\n",_u8CurProgramIdx,_recprogramInfo[_u8CurProgramIdx].FileName);
    _appDemo_PVR_RecStop(FALSE, _u8CurProgramIdx);
    appDemoPvr_print("appDemo_PVR_Record_Stop -->end \n");
    _u8TotalProgramNum--;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to pause when recording a live stream
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RecordPause \n
///
/// Sample Command:PVR_RecordPause
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Record_Pause(void)
{
    appDemoPvr_print("=========  appDemo_PVR_Record_Pause =========\n");
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    return MApi_PVR_RecordPause(_u8CurProgramIdx);
}

//------------------------------------------------------------------------------
/// @brief The sample code to resume when pausing
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RecordResume \n
///
/// Sample Command:PVR_RecordResume
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Record_Resume(void)
{
    appDemoPvr_print("=========  appDemo_PVR_Record_Resume =========\n");
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }


    return MApi_PVR_RecordResume(_u8CurProgramIdx);
}

//------------------------------------------------------------------------------
/// @brief The sample code to record a live stream in timeshift mode
/// @param[in] VideoPid Video PID of the current live program
/// @param[in] AudioPid Audio PID of the current live program
/// @param[in] PCRPid PCR PID of the current live program
/// @param[in] pu32VCodec Video Codec  of the current live program  0 is for MPEG2. 1 is for H264
/// @param[in] pu32ACodec Audio Codec  of the current live program
///         1: SiMPEG
///         2: SiAC3
///         3: SiAC3P
///         4: SiAAC
/// @param[in] fileName  file name
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_TimeshiftRecordStart \n
///
/// Sample Command:PVR_TimeshiftRecordStart
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Timeshift_RecordStart(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char* fileName)
{
    char FileName[32]={0};
    MS_BOOL bRet=FALSE;
    MS_U64 u64FreeSpaceInKB=0;
    MS_U64 u64TotalSpaceInKB=0;
    char mountPath[72]={0};


    // [01] Check if the current program is recorded now ==============================================//

    if(_u8CurProgramIdx>=_u8TotalRecorder)
    {
        printf("Invalid Recorder Idx\n");
        return FALSE;
    }

    if((_u8CurProgramIdx!=0xFF)&&MApi_PVR_IsRecording(_u8CurProgramIdx))
    {
        printf("Record now. Fail to start timeshift record.\n");
        return FALSE;
    }

    // [02] Should not play now ==============================================//
    if(MApi_PVR_IsPlaybacking())
    {
         printf("Play now. Fail to start timeshift record.\n");
         return FALSE;
    }

    // [03] Remove timeshift file ==============================================//
    printf("Try to Remove timeshift file.\n");
    if(strlen(_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath)==0)
    {
        strcpy(FileName,fileName);
    }
    else
    {
        sprintf(FileName,"%s/%s",_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath,fileName);
    }
    MApi_PVR_RemoveFile(FileName);


    // [04] Check the remain free space ==============================================//
    _appDemo_PVR_GetRecMountPath(TRUE,&mountPath[0]);
    printf("[%d] mountPath=%s\n",__LINE__,mountPath);
    appDemoPvr_print("_moutPath=%s\n",(char *)MApi_PVR_GetMouthPath());
    if(!_appDemo_PVR_GetDiskSpace(mountPath,&u64FreeSpaceInKB, &u64TotalSpaceInKB))
    {
        printf("No Free Disk Deteced!\n");
        return FALSE;
    }

    printf("u64FreeSpaceInMB=%llu\n",u64FreeSpaceInKB/1024);
    appDemoPvr_print("=========  appDemo_PVR_Record_Stop =========\n");
    if(u64FreeSpaceInKB < MIN_TIME_SHIFT_DISK_SPACE_IN_KB)
    {
        printf("Out of Disk Space. Timeshift need at least 512MB.\n");
        return FALSE;
    }


    // [04] Store the idx that timeshift record used ==============================================//
    _u8TimeshiftIdx=_u8CurProgramIdx;
    printf("_u8TimeshiftIdx=%d\n",_u8TimeshiftIdx);


    // [05] Record ==============================================//
    bRet = _appDemo_PVR_RecordStart(TRUE,  u64FreeSpaceInKB/1024,VideoPid, AudioPid, PCRPid, pu32VCodec, pu32ACodec, fileName);

    if(bRet)
    {
        _u8TotalProgramNum++;
        printf("\n***** Timeshift Recorder *****\n");
        printf("Screen Frezon: %s\n",(_bScreenFrozen)?"TRUE":"FALSE");
        printf("File Name: %s\n",fileName);
        printf("Program Info:\n");
        printf("\t Video PID: 0x%lX\n",*VideoPid);
        printf("\t Audio PID: 0x%lX\n",*AudioPid);
        printf("\t PCR   PID: 0x%lX\n",*PCRPid);
        printf("\t Video Dec: %lu\n",*pu32VCodec);
        printf("\t Audio Dec: %lu\n",*pu32ACodec);
        printf("***************************\n");
        printf("\n");

        printf("Timeshift Record Successfully.\n");
    }
    appDemoPvr_print(" MApi_PVR_RecordStart _u8TotalProgramNum=%d\n",_u8TotalProgramNum);


    appDemoPvr_print("_u8TimeshiftIdx=%d\n",_u8TimeshiftIdx);
    return bRet;
}
//------------------------------------------------------------------------------
/// @brief The sample code to stop recording in timeshift mode
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_TimeshiftRecordStop \n
///
/// Sample Command:PVR_TimeshiftRecordStop
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Timeshift_RecordStop(void)
{
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if(_u8TimeshiftIdx==0xFF)
    {
        printf("Already stop recording\n");
        return FALSE;
    }


    if(!_appDemo_PVR_RecStop(TRUE, _u8TimeshiftIdx))
    {
        printf("Fail to stop recording\n");
        return FALSE;
    }

    //check if there is timeshift playback
    if(!MApi_PVR_IsPlaybacking())
    {
        _u8TimeshiftIdx=0xFF;
    }
    _u8TotalProgramNum--;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop recording and playbacking in timeshift mode
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_TimeshiftStop \n
///
/// Sample Command:PVR_TimeshiftStop
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Timeshift_Stop(void)
{
    appDemoPvr_print("=========  appDemo_PVR_Timeshift_Stop =========\n");
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(_u8TimeshiftIdx==0xFF)
    {
        printf("Already stop timeshift mode!\n");
        return FALSE;
    }

    appDemo_PVR_Timeshift_PlaybackStop();
    appDemo_PVR_Timeshift_RecordStop();

    _u8TimeshiftIdx=0xFF;
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to playback in timeshift mode
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_TimeshiftPlaybackStart \n
///
/// Sample Command:PVR_TimeshiftPlaybackStart
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Timeshift_PlaybackStart(MS_U32 *u32PlaybackTimeInSec)
{
    appDemoPvr_print("=========  appDemo_PVR_Timeshift_PlaybackStart =========\n");
    PVRFilterInfo_t *Filters;
    MS_BOOL bRet = FALSE;
    MS_U32 u32VideoPid;
    MS_U32 u32AudioPid;
    MS_U32 u32PCRPid;
    MS_U32 u32VCodec;
    MS_U32 u32ACodec;
    MS_U32 u32PmtPid;
    MS_U32 u32LCN;
    char PathName[32]={0};

    // [01] Check if relative info is correct ==============================================//
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(_u8TimeshiftIdx==0xFF)
    {
        printf("Not to Start timeshiftrecording!\n");
        return FALSE;
    }

    _appDemo_PVR_GetRecMountPath(TRUE, PathName);
    if(! _appDemo_PVR_CheckUSB(PathName))
    {
        printf("USB is not ready!\n");
        return FALSE;
    }

    // [02] chnage to normal speed when playing  ==============================================//

    if(MApi_PVR_IsPlaybacking())
    {
        appDemoPvr_print("Normal Play\n");
        _appDemo_PVR_AudioMute(FALSE, TRUE);
        return MApi_PVR_PlaybackSetSpeed(_u8hPlayback,EN_APIPVR_PLAYBACK_SPEED_1X);
    }


    // [03]  ==============================================//
    appDemoPvr_print("Play FileName[%d] = %s\n",_u8TimeshiftIdx,_recprogramInfo[_u8TimeshiftIdx].FileName);
    Filters= _recprogramInfo[_u8TimeshiftIdx].Filters;
    u32VideoPid=_appDemo_PVR_FindPidinFilter(Filters, DMX_FILTER_TYPE_VIDEO);
    u32AudioPid=_recprogramInfo[_u8TimeshiftIdx].u16AudioPid;
    u32PCRPid=_appDemo_PVR_FindPidinFilter(Filters, DMX_FILTER_TYPE_PCR);
    u32VCodec=_appDemo_PVR_IsH264(_recprogramInfo[_u8TimeshiftIdx].enVCodec);
    u32ACodec=_recprogramInfo[_u8TimeshiftIdx].u32ACodec;
    u32PmtPid=_appDemo_PVR_FindPidinFilter(Filters, DMX_FILTER_TYPE_SECTION);
    u32LCN=_recprogramInfo[_u8TimeshiftIdx].u32LCN;

    appDemoPvr_print("(%lu,%lu,%lu,%lu,%lu)  %d\n",u32VideoPid,u32AudioPid,u32PCRPid,u32VCodec,u32ACodec,_recprogramInfo[_u8TimeshiftIdx].enVCodec);


    _appDemo_PVR_SetProgramInfo(&_plyprogramInfo,u32PCRPid, u32VideoPid, u32AudioPid,  u32VCodec,  u32ACodec, u32PmtPid,  u32LCN);
    // [04]   ==============================================//


    strcpy(_plyprogramInfo.FileName,_recprogramInfo[_u8TimeshiftIdx].FileName);
    appDemoPvr_print("Play FileName = %s\n",_plyprogramInfo.FileName);

    _appDemo_PVR_AV_Start(FALSE,&_plyprogramInfo,FALSE);
    MApi_PVR_PlaybackABLoopReset();
    _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();
    bRet=MApi_PVR_EX_TimeshiftPlaybackStart(_u8CurPlaybackProgramPath,&_u8hPlayback,_u8CurPlaybackProgramPath,(void *)&_AVStreamID,_plyprogramInfo.FileName,*u32PlaybackTimeInSec,0);
    if( bRet!=TRUE)
    {
        printf("Fail to timeshift play.\n");
        printf("Play live stream...\n\n");
        _appDemo_PVR_AV_Start(TRUE,&_livePromInfo,FALSE);
    }
    else
    {
        DemoAV_PlayAV();

        printf("\n***************************\n");
        printf("\nTimeshiftPlay Info:  \n");
        printf("\tFile: %s/%s  \n",MApi_PVR_GetMouthPath(),_recprogramInfo[_u8TimeshiftIdx].FileName);
        printf("\tPlayer Type: %s Player \n","Basic");
        printf("\tPlaybackStart time: %lu sec\n",*u32PlaybackTimeInSec);
        printf("\n***************************\n");
        printf("Timeshift Play successfully  \n");

    }
    _appDemo_PVR_FreezeScreen(FALSE);
    _appDemo_PVR_AV_Play(TRUE);


    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop a playback in timeshift mode
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_TimeshiftPlaybackStop \n
///
/// Sample Command:PVR_TimeshiftPlaybackStop
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_Timeshift_PlaybackStop(void)
{
    MS_BOOL bRet=FALSE;
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(_u8TimeshiftIdx==0xFF)
    {
        printf("Already stop playing\n");
        return FALSE;
    }
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No to play!\n");
        return FALSE;
    }
    bRet=MApi_PVR_TimeshiftPlaybackStop(&_u8hPlayback,_u8hRecord[_u8TimeshiftIdx]);
    if(MApi_PVR_IsPlaybacking()==FALSE)
    {
        printf("play live stream!\n");
        if(_appDemo_PVR_AV_Start(TRUE,&_livePromInfo,FALSE))   /* play live stream */
        {
            _appDemo_PVR_AV_Play(FALSE);
        }
    }
    _u8hPlayback=APIPVR_INVALID_HANDLER;
    //check if there is timeshift record
    if(!MApi_PVR_IsRecording(_u8hRecord[_u8TimeshiftIdx]))
    {
        _u8TimeshiftIdx=0xFF;
    }


    return bRet;
}



//------------------------------------------------------------------------------
/// @brief The sample code to start a playback
/// @param[in] VideoPid Video PID of the current live program
/// @param[in] AudioPid Audio PID of the current live program
/// @param[in] PCRPid PCR PID of the current live program
/// @param[in] pu32VCodec Video Codec  of the current live program  0 is for MPEG2. 1 is for H264
/// @param[in] pu32ACodec Audio Codec  of the current live program
///         0: SiNOT_VALID
///         1: SiMPEG
///         2: SiAC3
///         3: SiMPEG_AD
///         4: SiMPEG4
///         5: SiAAC
///         6: SiAC3P
///         7: SiAC3_AD
/// @param[in] filename  the file name of a recored file
/// @param[in] fileVideoPid Video PID of the file
/// @param[in] fileAudioPid Audio PID of the file
/// @param[in] filePCRPid PCR PID of the file
/// @param[in] fileVCodec Video Codec  of the file  0 is for MPEG2. 1 is for H264
/// @param[in] fileACodec Audio Codec  of the file
///         0: SiNOT_VALID
///         1: SiMPEG
///         2: SiAC3
///         3: SiMPEG_AD
///         4: SiMPEG4
///         5: SiAAC
///         6: SiAC3P
///         7: SiAC3_AD
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackStart \n
///
/// Sample Command:PVR_PlaybackStart 101 102 101 0 1 28106_101_102_101_0_TEST_19921009_212744.ts 101 102 101 0 1
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackStart(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid,const  MS_U32* pu32VCodec,const  MS_U32* pu32ACodec,char *filename, MS_U32 *u32PlaybackTimeInSec)
{
    char mountPath[32]={0};
    char u8FileName[32]={0};
    MS_BOOL bRet=FALSE;
    MS_U16 u16PmtPid=INVALID_PID;
    MS_U16 u16LCN=0xFFFF;
    MS_U8 u8RecIdx=0xFF;
    MS_U32 u32VCodec = *pu32VCodec;
    MS_U32 u32ACodec = *pu32ACodec;

    // [01] Check if the current program is recorded now ==============================================//
    appDemoPvr_print("=========  appDemo_PVR_PlaybackStart =========\n");
    appDemoPvr_print("filename=%s\n",(filename)?filename:"NULL");
    appDemoPvr_print("live:(%ld,%ld,%ld,%ld,%ld)\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);

    if ((!filename)||(strlen(filename)<=0))
    {
        printf("No Such File Name\n");
        return FALSE;
    }

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    _appDemo_PVR_GetRecIdx(&u8RecIdx, FALSE);

    printf("[%s][%s][%d] u8RecIdx=%u\n",__FILE__,__FUNCTION__,__LINE__,u8RecIdx);
    _appDemo_PVR_GetRecMountPath(FALSE,&mountPath[0]);
    if(! _appDemo_PVR_CheckUSB(mountPath))
    {
        printf("USB is not ready!\n");
        return FALSE;
    }

    appDemoPvr_print("[%s][%d]  _appDemo_PVR_IsFileExist\n", __FUNCTION__, __LINE__);


    if(!_appDemo_PVR_IsFileExist(mountPath,filename))
    {
        printf("%s/%s did not exist.\n",mountPath,filename);
        return FALSE;
    }

    // [02] change to normal speed when playing ==============================================//

    if(MApi_PVR_IsPlaybacking())
    {
        printf("Normal Play\n");
        _appDemo_PVR_AudioMute(FALSE, TRUE);
        return MApi_PVR_PlaybackSetSpeed(_u8hPlayback,EN_APIPVR_PLAYBACK_SPEED_1X);
    }

    appDemoPvr_print("[%s][%d] appDemo_Dmx_GetCurProgram\n", __FUNCTION__, __LINE__);

    // [03] Check if the current program is recorded now ==============================================//

    if(appDemo_Dmx_GetLiveProgram(PCRPid, VideoPid, AudioPid, &u32VCodec, &u32ACodec,&u16PmtPid,&u16LCN) == FALSE)
    {
        appDemoPvr_print("fail to appDemo_Dmx_GetCurProgram.\n");
    }

    appDemoPvr_print("[%s][%d] _appDemo_PVR_SetProgramInfo\n", __FUNCTION__, __LINE__);

    if(FALSE==_appDemo_PVR_SetProgramInfo(&_livePromInfo,(*PCRPid), (*VideoPid), (*AudioPid), u32VCodec, u32ACodec,(MS_U32)u16PmtPid, (MS_U32)u16LCN))
    {
        printf("Can't set program info!\n");
        return FALSE;
    }
    if(strlen(_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath)!=0)
    {
        sprintf(u8FileName,"%s/%s",_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath,filename);
    }
    else
    {
        strcpy(u8FileName,filename);
    }

    if(_appDemo_PVR_GetFileInfo(u8FileName, &_plyprogramInfo)==FALSE)
    {
        printf("Can't get program info\n");
        return FALSE;
    }

    appDemoPvr_print("[%s][%d] before string copy\n", __FUNCTION__, __LINE__);

    //strcpy(_plyprogramInfo.FileName,filename);
    appDemoPvr_print("[%s][%d] path=%s \n", __FUNCTION__, __LINE__,_plyprogramInfo.FileName);
    MApi_PVR_PlaybackABLoopReset();
    _appDemo_PVR_AV_Start(FALSE,&_plyprogramInfo,FALSE);
    _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();
    bRet=MApi_PVR_EX_PlaybackStart(_u8CurPlaybackProgramPath,&_u8hPlayback,(void *)&_AVStreamID,u8FileName,*u32PlaybackTimeInSec,0);
    if(!bRet)
    {
        printf("Fail to Normal Play. Swith to Live mode.\n");
        _appDemo_PVR_AV_Start(TRUE,&_livePromInfo,FALSE);
    }
    else
    {
        printf("\n***************************\n");
        printf("\nNormal Play Info:  \n");
        printf("\tFile: %s/%s  \n",MApi_PVR_GetMouthPath(),_plyprogramInfo.FileName);
        printf("\tPlayer Type: %s Player \n","Basic");
        printf("\tPlaybackStart time: %lu sec\n",*u32PlaybackTimeInSec);
        printf("\n***************************\n");
        printf("Normal Play successfully  \n");

        DemoAV_PlayAV();

    }

    _appDemo_PVR_AV_Play(TRUE);
    return bRet;


}

//------------------------------------------------------------------------------
/// @brief The sample code to stop a playback
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackStop \n
///
/// Sample Command:PVR_PlaybackStop
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackStop(void)
{
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()==FALSE)
    {
        printf("Already stop playing\n");
        return FALSE;
    }

    if(_bEnStillImageZapping)
    {
        MS_BOOL bEnable = FALSE;
        appDemo_XC_EnableStillImage(&bEnable);
    }

    if(!MApi_PVR_PlaybackStop(&_u8hPlayback))
    {
        printf("Fail to playback\n");
        return FALSE;
    }
    _appDemo_PVR_AV_Start(TRUE,&_livePromInfo,FALSE);
    _appDemo_PVR_AV_Play(TRUE);
    memset(&_plyprogramInfo,0,sizeof(PVRProgramInfo_t));
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to pause a playback
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackPause \n
///
/// Sample Command:PVR_PlaybackPause
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackPause(void)
{
    appDemoPvr_print("=========  appDemo_PVR_PlaybackPause =========\n");
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }


    return MApi_PVR_PlaybackPause(_u8hPlayback);
}
//------------------------------------------------------------------------------
/// @brief The sample code to resume a playback
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackResume \n
///
/// Sample Command:PVR_PlaybackResume
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackResume(void)
{
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }


    if(MApi_PVR_PlaybackGetSpeed(_u8hPlayback)==EN_APIPVR_PLAYBACK_SPEED_0X)
    {
        _appDemo_PVR_AudioMute(FALSE, TRUE);
        return MApi_PVR_PlaybackResume(_u8hPlayback);
    }

    _appDemo_PVR_AudioMute(FALSE, TRUE);
    return MApi_PVR_PlaybackSetSpeed(_u8hPlayback,EN_APIPVR_PLAYBACK_SPEED_1X);

}

//------------------------------------------------------------------------------
/// @brief The sample code to playback slow forward
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackSF \n
///
/// Sample Command:PVR_PlaybackSF
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackSlowForward(void)
{
    MS_BOOL bRet=FALSE;
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }

    enSpeed=MApi_PVR_PlaybackGetSpeed(_u8hPlayback);
    if(enSpeed==EN_APIPVR_PLAYBACK_SPEED_FF_1_32X)
    {
        enSpeed=EN_APIPVR_PLAYBACK_SPEED_1X;
    }
    else
    {
        enSpeed=_appDemo_PVR_GetSlowForward(enSpeed);
    }

    bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback,enSpeed);
    if(EN_APIPVR_PLAYBACK_SPEED_1X != MApi_PVR_PlaybackGetSpeed(_u8hPlayback))
    {
        printf("%ldxFF Play \n",MApi_PVR_PlaybackGetSpeed(_u8hPlayback));
        _appDemo_PVR_AudioMute(TRUE, TRUE);
    }
    else
    {
        printf("Normal Play \n");
        _appDemo_PVR_AudioMute(FALSE, TRUE);
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to playback fast forward
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackFF \n
///
/// Sample Command:PVR_PlaybackFF
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackFastForward(void)
{
    MS_BOOL bRet=FALSE;
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }

    enSpeed=MApi_PVR_PlaybackGetSpeed(_u8hPlayback);
    if(enSpeed==EN_APIPVR_PLAYBACK_SPEED_32XFF)
    {
        enSpeed=EN_APIPVR_PLAYBACK_SPEED_1X;
    }
    else
    {
        enSpeed=_appDemo_PVR_GetFastForward(enSpeed);
    }

    bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback,enSpeed);
    if(EN_APIPVR_PLAYBACK_SPEED_1X != MApi_PVR_PlaybackGetSpeed(_u8hPlayback))
    {
        printf("%ldxFF Play \n",MApi_PVR_PlaybackGetSpeed(_u8hPlayback)/1000);
        _appDemo_PVR_AudioMute(TRUE, TRUE);
    }
    else
    {
        printf("Normal Play \n");
        _appDemo_PVR_AudioMute(FALSE, TRUE);
    }
    return bRet;
}
//------------------------------------------------------------------------------
/// @brief The sample code to playback fast backward
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackFB \n
///
/// Sample Command:PVR_PlaybackFB
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackFastBackward(void)
{
    MS_BOOL bRet=FALSE;
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }
    enSpeed=MApi_PVR_PlaybackGetSpeed(_u8hPlayback);
    if(enSpeed==EN_APIPVR_PLAYBACK_SPEED_32XFB)
    {
        enSpeed=EN_APIPVR_PLAYBACK_SPEED_1X;
    }
    else{
        enSpeed=_appDemo_PVR_GetFastBackward(enSpeed);
    }
    bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback,enSpeed);
    if(EN_APIPVR_PLAYBACK_SPEED_1X != MApi_PVR_PlaybackGetSpeed(_u8hPlayback))
    {
        printf("%ldxFB Play \n",MApi_PVR_PlaybackGetSpeed(_u8hPlayback)/1000);
        _appDemo_PVR_AudioMute(TRUE, TRUE);
    }
    else{
        printf("Normal Play \n");
        _appDemo_PVR_AudioMute(FALSE, TRUE);
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to jump to a specific time when playback
/// @param[in] u32Hour hour 0~23
/// @param[in] u32Minute minute 0~59
/// @param[in] u32Second second 0~59
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackJumpToTime \n
///
/// Sample Command:PVR_PlaybackJumpToTime 0 0 10
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackJumpToTime(MS_U32 *u32Hour,MS_U32 *u32Minute,MS_U32 *u32Second)
{
    MS_U32 u32JumpToTimeInSeconds = (*u32Hour)*60*60+(*u32Minute)*60+(*u32Second);
    appDemoPvr_print("Target Time: %2ld:%2ld:%2ld\n",*u32Hour,*u32Minute,*u32Second);
    appDemoPvr_print("Target Time: %ld seconds\n",u32JumpToTimeInSeconds);
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if(((*u32Hour)>23) || ((*u32Hour)<0))
    {
        printf("The value of a hour should be between 0~23.\n");
        return FALSE;
    }
    if(((*u32Minute)>59) || ((*u32Minute)<0))
    {
        printf("The value of a minute should be between 0~59.\n");
        return FALSE;
    }
    if(((*u32Second)>59) || ((*u32Second)<0))
    {
        printf("The value of a second should be between 0~59.\n");
        return FALSE;
    }
    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }

    return MApi_PVR_PlaybackJumpToTime(_u8hPlayback,u32JumpToTimeInSeconds);
}





//------------------------------------------------------------------------------
/// @brief The sample code to get file information
/// @param[in] FileName
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_GetFileInfo \n
///
/// Sample Command:PVR_GetFileInfo tt.ts
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_GetFileInfo(char *FileName)
{
    PVRProgramInfo_t promInfo;

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }


    if(FileName==NULL)
    {
        printf("Invalid File Name");
        return FALSE;
    }
    if(strlen(FileName)==0)
    {
        printf("Invalid File Name");
        return FALSE;
    }


    _appDemo_PVR_SetProgramInfo(&promInfo,INVALID_PID, INVALID_PID, INVALID_PID, 0, 0,INVALID_PID, 0);

    if(_appDemo_PVR_GetFileInfo(FileName, &promInfo)==FALSE)
    {
        printf("GetFileInformation fail!\n");
        return FALSE;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to Set current record idx
/// @param[in] idx record idx number
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetIdx \n
///
/// Sample Command:PVR_SetIdx 0
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_SetIdx(MS_U32 *idx)
{

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if((*idx)>=_u8TotalRecorder)
    {
         printf("Invalid Idx!\n");
         return FALSE;
    }
    _u8CurProgramIdx = (MS_U8) *idx;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to get current record idx
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_GetIdx \n
///
/// Sample Command:PVR_GetIdx
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_GetIdx(void)
{
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    printf("Current idx =%d\n",_u8CurProgramIdx);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to playback with one frame
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_StepInPlay \n
///
/// Sample Command:PVR_StepInPlay
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackStepIn(void)
{
    appDemoPvr_print("=========  appDemo_PVR_PlaybackStepIn =========\n");


    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }


    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not play!\n");
        return FALSE;
    }

    if(MApi_PVR_PlaybackGetSpeed(_u8hPlayback)==EN_APIPVR_PLAYBACK_SPEED_1X)
    {
        _appDemo_PVR_AudioMute(TRUE, TRUE);
    }


    if( MApi_PVR_PlaybackStepIn(_u8hPlayback)!=TRUE)
    {
         printf("StepIn play failed!\n");
         return FALSE;
    }


    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to playback in timeshift mode
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_NormalPlay \n
///
/// Sample Command:PVR_NormalPlay
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_PlaybackWithNormalSpeed(void)
{
    appDemoPvr_print("=========  appDemo_PVR_PlaybackWithNormalSpeed =========\n");
    MS_BOOL bRestart = FALSE;
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if(MApi_PVR_IsPlaybacking()!=TRUE)
    {
        printf("Not play!\n");
        return FALSE;
    }



    if(EN_APIPVR_PLAYBACK_SPEED_STEP_IN == MApi_PVR_PlaybackGetSpeed(_u8hPlayback))
    {
        bRestart = TRUE;
    }

    _appDemo_PVR_AudioMute(FALSE, TRUE);
    MApi_PVR_PlaybackSetSpeed(_u8hPlayback,EN_APIPVR_PLAYBACK_SPEED_1X);


    if(bRestart)
    {
        _appDemo_PVR_AV_Play(FALSE);
    }
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to Jump to the pts of Thumbnail
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_CaptureThumbnail \n
///
/// Sample Command:PVR_CaptureThumbnail 1280x720.yuyv.07
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_CaptureThumbnail(char *filename)
{
    appDemoPvr_print("=========  appDemo_PVR_CaptureThumbnail =========\n");
    MS_U32 u32capturepts=0;
    MS_U32 u32PhysicalAddr;
    MS_U32 u32Timestamp;
    MS_U32 u32Width =PVR_THUMBNAIL_CAPTURE_WIDTH;
    MS_U32 u32Height =PVR_THUMBNAIL_CAPTURE_HEIGHT;
    char fullpathname[FILE_PATH_SIZE]={0};
    MS_U32 u32VirtualAddr = 0;
    MS_U8 u8FBId =0xff;

    if(strlen(filename)==0)
    {
        printf("Please input file name!\n");
        return FALSE;
    }
    if(!GOP_IsGOPInit())
    {
        printf("Not initialize GOP!\n");
        return FALSE;
    }
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }


    if(MApi_PVR_IsPlaybacking())
    {
        u32Timestamp = MApi_PVR_GetPlaybackTime(_u8hPlayback);
    }
    else if(MApi_PVR_IsRecording(_u8CurProgramIdx))
    {
        u32Timestamp = MApi_PVR_GetRecordTime(_u8hRecord[_u8CurProgramIdx],_u8CurProgramIdx);
    }
    else{
        printf("Fail to capture video!\n");
        return FALSE;
    }

    MApi_GOP_GWIN_SwitchGOP(0);//use GOP 0
    u8FBId=MApi_GOP_GWIN_GetFreeFBID();
    //OP scan type is progressive
    MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
    //OP data format only YUV; Set data format to YUV422
    MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);
    MApi_GOP_GWIN_CreateFB(u8FBId, PVR_THUMBNAIL_DISPLAY_X, PVR_THUMBNAIL_DISPLAY_Y, PVR_THUMBNAIL_CAPTURE_WIDTH, PVR_THUMBNAIL_CAPTURE_HEIGHT, E_MS_FMT_YUV422);
    //MApi_GOP_GWIN_GetFBInfo(u8FBId, &bufAttr);
    GOP_GwinFBAttr DstFBInfo;
    MApi_GOP_GWIN_GetFBInfo( u8FBId, &DstFBInfo);
    MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0x00);
    MApi_GFX_FlushQueue();
    MApi_GOP_GWIN_GetFBAddr(u8FBId, &u32PhysicalAddr);
    //printf("[%d]u8FBId=%d ,u32PhysicalAddr=0x%08lx\n",__LINE__,u8FBId,u32PhysicalAddr);
    _AVStreamID.vstreamID = (void*)appDemo_Video_GetStreamID();
    if(!MApi_PVR_EX_CaptureVideo((void *)&_AVStreamID,u32PhysicalAddr, u32Width, u32Height,&u32capturepts))
    {
        MApi_GOP_GWIN_DestroyFB(u8FBId);
        printf("Fail to capture video!\n");
        return FALSE;
    }



    //--------Save file--------
    sprintf(fullpathname,"%s/%s",MApi_PVR_GetMouthPath(),filename);
    FILE *filePointer = MsFS_Fopen(fullpathname, "w");
    if(filePointer == NULL)
    {
       printf("can't open file\n");
       MApi_GOP_GWIN_DestroyFB(u8FBId);
       return FALSE;
    }
    else
    {
       u32VirtualAddr = MsOS_PA2KSEG1(u32PhysicalAddr);
       MsFS_Fwrite((void *)&u32Timestamp, sizeof(u32Timestamp), 1, filePointer);
       MsFS_Fwrite((void *)&u32capturepts, sizeof(u32capturepts), 1, filePointer);
       MsFS_Fwrite((void *)&u32Width, sizeof(u32Width), 1, filePointer);
       MsFS_Fwrite((void *)&u32Height, sizeof(u32Height), 1, filePointer);
       MsFS_Fwrite((void *)u32VirtualAddr, 1,u32Width * u32Height * 2, filePointer);
    }
    MsFS_Fflush(filePointer);
    MsFS_Fclose(filePointer);
    //--------Show capture Video--------
    _appDemo_PVR_Show_Capture_Video(u32PhysicalAddr);
    MApi_GOP_GWIN_DestroyFB(u8FBId);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to Jump to the pts of Thumbnail
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_JumpToThumbnail \n
///
/// Sample Command:PVR_JumpToThumbnail 1280x720.yuyv.07
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_JumpToThumbnail(char *filename)
{
    appDemoPvr_print("=========  appDemo_PVR_CaptureThumbnail =========\n");
    MS_U32 u32capturepts=0;
    MS_U32 u32Timestamp;
    char fullpathname[FILE_PATH_SIZE]={0};
    void * virtualAddr=MsOS_AllocateMemory(PVR_THUMBNAIL_SURFACE_SIZE, gs32NonCachedPoolID);
    MS_U32 u32PhysicalAddr;
    MS_U32 u32Width =PVR_THUMBNAIL_CAPTURE_WIDTH;
    MS_U32 u32Height =PVR_THUMBNAIL_CAPTURE_HEIGHT;
    FILE *filePointer;
    char mountPath[32]={0};
    if(strlen(filename)==0)
    {
        printf("Please input file name!\n");
        return FALSE;
    }

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    _appDemo_PVR_GetRecMountPath((_u8TimeshiftIdx!=0xFF),&mountPath[0]);
    if(! _appDemo_PVR_CheckUSB(mountPath))
    {
        printf("USB is not ready!\n");
        return FALSE;
    }
    sprintf(fullpathname,"%s/%s",mountPath,filename);
    filePointer = MsFS_Fopen(fullpathname, "r");
    if(filePointer == NULL)
    {
       printf("can't open file\n");

       return FALSE;
    }
    else
    {

       //MS_U32 u32VirtualAddr = MsOS_PA2KSEG1(u32PhysicalAddr)
       MsFS_Fread((void *)&u32Timestamp, sizeof(u32Timestamp), 1, filePointer);
       //printf("read %u bytes \u32Timestamp : %lu\n",sizeof(u32Timestamp),u32Timestamp);
       //printf("read %u bytes \nPTS : %lu\n",sizeof(capture_pts),capture_pts);
       MsFS_Fread((void *)&u32capturepts, sizeof(u32capturepts), 1, filePointer);
       MsFS_Fread((void *)&u32Width, sizeof(u32Width), 1, filePointer);
       MsFS_Fread((void *)&u32Height, sizeof(u32Height), 1, filePointer);
       MsFS_Fread(virtualAddr, 1,PVR_THUMBNAIL_CAPTURE_WIDTH * PVR_THUMBNAIL_CAPTURE_HEIGHT * 2, filePointer);
    }
    MsFS_Fflush(filePointer);
    MsFS_Fclose(filePointer);
    if(MApi_PVR_IsPlaybacking())
    {
        u32PhysicalAddr=MS_VA2PA((MS_U32)virtualAddr);
        _appDemo_PVR_Show_Capture_Video(u32PhysicalAddr);
        MApi_PVR_PlaybackJumpToThumbnail(_u8hPlayback,u32Timestamp, u32capturepts);
        MsOS_FreeMemory(virtualAddr,gs32NonCachedPoolID);
        return TRUE;
    }
    return FALSE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to Jump to the pts of Thumbnail
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_HideThumbnail \n
///
/// Sample Command:PVR_HideThumbnail
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_HideThumbnail(void)
{
    _appDemo_PVR_Remove_Capture_Video();
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to play from the Ath second to the Bth second
/// @param[in] ABBeginTime
/// @param[in] ABEndTime
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetABLoop \n
///
/// Sample Command:PVR_SetABLoop 2 10
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_PlaybackABLoop(MS_U32 *u32ABBeginTime,MS_U32 *u32ABEndTime)
{
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No Play.\n");
        return FALSE;
    }
    return MApi_PVR_PlaybackABLoop(_plyprogramInfo.FileName,*u32ABBeginTime,*u32ABEndTime);

}

//------------------------------------------------------------------------------
/// @brief The sample code to play from the Ath second to the Bth second
/// @param[in] ABBeginTime
/// @param[in] ABEndTime
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetABLoop \n
///
/// Sample Command:PVR_ReSetABLoop
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_PlaybackResetABLoop(void)
{
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No Play.\n");
        return FALSE;
    }
    return MApi_PVR_PlaybackABLoopReset();

}

//------------------------------------------------------------------------------
/// @brief The sample code to play and skip from the Ath second to the Bth second
/// @param[in] BeginTimeInSec
/// @param[in] EndTimeInSec
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetSkipTime \n
///
/// Sample Command:PVR_SetSkipTime 2 10
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_PlaybackAddSkipTime(MS_U32 *u32BeginTimeInSec,MS_U32 *u32EndTimeInSec)
{
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No Play.\n");
        return FALSE;
    }
    return MApi_PVR_SkipTableAdd(_plyprogramInfo.FileName,*u32BeginTimeInSec,*u32EndTimeInSec);

}


//------------------------------------------------------------------------------
/// @brief The sample code to play without skipping from the Ath second to the Bth second
/// @param[in] BeginTimeInSec
/// @param[in] EndTimeInSec
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RemoveSkipTime \n
///
/// Sample Command:PVR_RemoveSkipTime 2 10
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_PlaybackRemoveSkipTime(MS_U32 *u32BeginTimeInSec,MS_U32 *u32EndTimeInSec)
{
    MS_U32 u32SkipBeginTime=*u32BeginTimeInSec;
    MS_U32 u32Idx=0;
    MS_U32 u32SkipEndTime=*u32EndTimeInSec;

    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_SkipTableGetIndex(&u32SkipBeginTime,&u32Idx, &u32SkipEndTime))
    {
        printf("Cannot find such interval.\n");
        return FALSE;
    }
    if(!MApi_PVR_SkipTableRemove(_plyprogramInfo.FileName,u32Idx))
    {
        printf("Fail to remove!\n");
        return FALSE;
    }
    if(u32SkipEndTime>*u32EndTimeInSec)
    {
        printf("Still skip %lu to %lu!\n",*u32EndTimeInSec,u32SkipEndTime);
        MApi_PVR_SkipTableAdd(_plyprogramInfo.FileName,*u32EndTimeInSec,u32SkipEndTime);
    }
    if(*u32BeginTimeInSec>u32SkipBeginTime)
    {
        printf("Still skip %lu to %lu!\n",u32SkipBeginTime,*u32BeginTimeInSec);
        MApi_PVR_SkipTableAdd(_plyprogramInfo.FileName,u32SkipBeginTime,*u32BeginTimeInSec);
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to set retention time
/// @param[in] RetentionLimitTimeInSec
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetSkipTime \n
///
/// Sample Command:PVR_SetretentionTime 120
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_SetPlaybackRetentionLimit(const MS_U32 *u32RetentionLimitTimeInSec)
{
    MS_U32 u32RetentionLimitTime=*u32RetentionLimitTimeInSec;
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_SetPlaybackRetentionLimit(_plyprogramInfo.FileName,&u32RetentionLimitTime))
    {
        printf("Fail to set retention time!\n");
        return FALSE;
    }
    if(u32RetentionLimitTime!=*u32RetentionLimitTimeInSec)
    {
        printf("Set retention time to file end time!\n");
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to jump forward 30 sec and normal play
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlayJumpForward \n
///
/// Sample Command:PVR_PlayJumpForward
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_PlaybackJumpForward(void)
{
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_PlaybackJumpForward(_u8hPlayback))
    {
        printf("Fail to Jump Forward!\n");
        return FALSE;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to The sample code to jump backward 30 sec and normal play
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlayJumpBackward \n
///
/// Sample Command:PVR_PlayJumpBackward
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_PlaybackJumpBackward(void)
{
    if(!MApi_PVR_IsPlaybacking())
    {
        printf("No to Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_PlaybackJumpBackward(_u8hPlayback))
    {
        printf("Fail to Jump Forward!\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show current play/record time
/// @param[in] Type Record/Play
/// @param[in] Set TRUE/FALSE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetSkipTime \n
///
/// Sample Command:PVR_SetSkipTime 2 10
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_ShowTime(const MS_U32 *u32Type,const MS_U32 *u32Set)
{
    if(*u32Type==0)
    {
        return FALSE;
    }
    if(*u32Type==1)
    {
         _bShowRecordTime=(*u32Set==0)?FALSE:TRUE;
    }
    else if(*u32Type==2)
    {
        _bShowPlayTime=(*u32Set==0)?FALSE:TRUE;
    }
    else
    {
        printf("No such type!\n");
        return FALSE;
    }
    return TRUE;

}


//------------------------------------------------------------------------------
/// @brief The sample code to remove file
/// @param[in] filename
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RemoveFile \n
///
/// Sample Command:PVR_RemoveFile a.ts
//------------------------------------------------------------------------------
MS_BOOL appDemo_PVR_RemoveFile(char *fileName)
{

    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }
    if ((!fileName)||(strlen(fileName)<=0))
    {
        printf("No File Name\n");
        return FALSE;
    }
    if(!MApi_PVR_RemoveFile(fileName))
    {
        printf("Remove file fail!\n");
        return FALSE;
    }

    return TRUE;

}


//------------------------------------------------------------------------------
/// @brief The sample code to rename recorded file
/// @param[in] current file name
/// @param[in] new file name
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Rename \n
///
/// Sample Command:PVR_Rename 2.ts 10.ts
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_RenameFile(char *fileName,char *newFileName)
{
    if(!_bPvrInit)
    {
        printf("Not initialize PVR!\n");
        return FALSE;
    }

    if(MApi_PVR_IsPlaybacking())
    {
        printf("Cannot set Playback type during playing\n");
        return FALSE;
    }

    return MApi_PVR_RenameFileName(fileName,newFileName);

}

//------------------------------------------------------------------------------
/// @brief The sample code to get start time in timeshift mode
/// @param[in] void
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_GetTimeshiftRecStartTime \n
///
/// Sample Command:PVR_GetTimeshiftRecStartTime
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_GetTimeshiftRecStartTime(void)
{
    MS_U32 u32RecordStartTime=0;
    if(_u8TimeshiftIdx==0xFF)
    {
        printf("Not to start timeshift mode!\n");
        return FALSE;
    }

    if(MApi_PVR_GetRecordStartTime(_u8hRecord[_u8TimeshiftIdx],&u32RecordStartTime)==FALSE)
    {
        printf("The start time cannot be got!\n");
        return FALSE;
    }

    printf("Timeshift Mode Start Time: %8lu sec\n",u32RecordStartTime); //detect if data wrapper around
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to get start time in timeshift mode
/// @param[in] ScreenFrozen
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetTimeshiftRecScreenFrozen \n
///
/// Sample Command:PVR_SetTimeshiftRecScreenFrozen
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_SetTimeshiftRecScreenFrozen(const MS_BOOL *bFrozen)
{
    _bScreenFrozen=*bFrozen;
    printf("Screen Frozen : %s \n",(_bScreenFrozen)?"TRUE":"FALSE");
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to initialize pvr.
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SpeedCheck \n
///
/// Sample Command:PVR_SpeedCheck
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_CheckUSBSpeed(void)
{
    appDemoPvr_print("========= appDemo_PVR_CheckUSBSpeed =========\n");

    if(!_bPvrInit)
    {
        printf( "Not to init PVR!\n");
        return FALSE;
    }
    printf("Please Wait...\n");
    MApi_PVR_SpeedCheck();


    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to enable still image zapping.
/// @param[in] bEnable Enable flag of still image zapping
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_EnableStillImageZapping \n
///
/// Sample Command:PVR_EnableStillImageZapping 1
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_EnableStillImageZapping(MS_BOOL *bEnable)
{
    if(*bEnable)
    {
        _bEnStillImageZapping = TRUE;
    }
    else
    {
        _bEnStillImageZapping = FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to show PVR helper
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b HelpPVR \n
///
/// Sample Command:HelpPVR
//------------------------------------------------------------------------------


MS_BOOL appDemo_PVR_Help(void)
{
    printf ("*************************PVR function List*************************\n");
    printf ("PVR_Init [FSType] [MouthPath]\n");
    printf ("-----FSType : Filesystem Type\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_UNKNOWN = 0,\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_JFFS2 = 1,\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_VFAT = 2,\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_EXT2 = 3,\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_EXT3 = 4,\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_MSDOS = 5,\n");
    printf ("---------------: E_FILE_SYSTEM_TYPE_NTFS = 6,\n");
    printf ("-----MouthPath : USB Storage mounth path\n");
    printf ("PVR_SetIdx [index]\n"),
    printf ("-----index : Current recording idx\n");
    printf ("PVR_GetIdx\n");
    printf ("PVR_RecordStart [VideoPid] [AudioPid] [PCRPid] [VCodec] [ACodec] [FileName]\n");
    printf ("-----VideoPid : Video Pid of current live program\n");
    printf ("-----AudioPid : Audio Pid of current live program\n");
    printf ("-----PCRPid : PCR Pid of current live program\n");
    printf ("-----VCodec : Video Codec of current live program\n");
    printf ("---------------: MPEG2 = 0,\n");
    printf ("---------------: H264 = 1,\n");
    printf ("-----ACodec : Audio Codec of current live program\n");
    printf ("---------------: SiMPEG = 1,\n");
    printf ("---------------: SiAC3 = 2,\n");
    printf ("---------------: SiAC3P = 3,\n");
    printf ("---------------: SiAAC = 4,\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_RecordPause\n");
    printf ("PVR_RecordResume\n");
    printf ("PVR_RecordStop\n");
    printf ("PVR_PlaybackStart [VideoPid] [AudioPid] [PCRPid] [VCodec] [ACodec] [FileName]\n");;
    printf ("-----VideoPid : Video Pid of current live program\n");
    printf ("-----AudioPid : Audio Pid of current live program\n");
    printf ("-----PCRPid : PCR Pid of current live program\n");
    printf ("-----VCodec : Video Codec of current live program\n");
    printf ("---------------: MPEG2 = 0,\n");
    printf ("---------------: H264 = 1,\n");
    printf ("-----ACodec : Audio Codec of current live program\n");
    printf ("---------------: SiMPEG = 1,\n");
    printf ("---------------: SiAC3 = 2,\n");
    printf ("---------------: SiAC3P = 3,\n");
    printf ("---------------: SiAAC = 4,\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_PlaybackPause\n");
    printf ("PVR_PlaybackResume\n");
    printf ("PVR_PlaybackStop\n");
    printf ("PVR_PlaybackFF\n");
    printf ("PVR_PlaybackFB\n");
    printf ("PVR_PlaybackJumpToTime [Hours] [Minutes] [Seconds]\n");
    printf ("-----Hours : 0~23\n");
    printf ("-----Minutes : 0~59\n");
    printf ("-----Seconds : 0~59\n");
    printf ("PVR_StepInPlay\n");
    printf ("PVR_PlaybackWithNormalSpeed\n");
    printf ("PVR_TimeshiftRecordStart [VideoPid] [AudioPid] [PCRPid] [VCodec] [ACodec] [FileName]\n");
    printf ("-----VideoPid : Video Pid of current live program\n");
    printf ("-----AudioPid : Audio Pid of current live program\n");
    printf ("-----PCRPid : PCR Pid of current live program\n");
    printf ("-----VCodec : Video Codec of current live program\n");
    printf ("---------------: MPEG2 = 0\n");
    printf ("---------------: H264 = 1\n");
    printf ("-----ACodec : Audio Codec of current live program\n");
    printf ("---------------: SiMPEG = 1,\n");
    printf ("---------------: SiAC3 = 2,\n");
    printf ("---------------: SiAC3P = 3,\n");
    printf ("---------------: SiAAC = 4,\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_TimeshiftRecordStop\n");
    printf ("PVR_TimeshiftPlaybackStart\n");
    printf ("PVR_TimeshiftPlaybackStop\n");
    printf ("PVR_TimeshiftStop\n");
    printf ("PVR_SetABLoop [BeginTimeInSec] [EndTimeInSec]\n");
    printf ("PVR_RemoveSkipTime [BeginTimeInSec] [EndTimeInSec]\n");
    printf ("PVR_ShowTime [Type] [Set]\n");
    printf ("-----Type : Record/Play Time\n");
    printf ("---------------: Record Time =1,\n");
    printf ("---------------: Play Time =2,\n");
    printf ("-----Set : TRUE=1/FALSE=0\n");
    printf ("---------------: FALSE =0,\n");
    printf ("---------------: TRUE =1,\n");
    printf ("PVR_GetFileInfo [FileName]\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_CaptureThumbnail [FileName]\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_JumpToThumbnail [FileName]\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_PlayJumpForward\n");
    printf ("PVR_PlayJumpBackward\n");
    printf ("PVR_Rename [FileName] [NewFileName]\n");
    printf ("-----FileName : File Name\n");
    printf ("-----NewFileName : File Name\n");
    printf ("PVR_RemoveFile [FileName]\n");
    printf ("-----FileName : File Name\n");
    printf ("PVR_SetPlayType [Type]\n");
    printf ("-----Type : Player Type\n");
    printf ("---------------: Basic Player =0,\n");
    printf ("---------------: ES Player =1,\n");
    printf ("PVR_SetRecordType [Type]\n");
    printf ("-----Type : Player Type\n");
    printf ("---------------: Single Recorder =0,\n");
    printf ("---------------: Dual Recorder =1,\n");
    printf ("PVR_SetTimeshiftRecScreenFrozen [Set]\n");
    printf ("-----Set : TRUE=1/FALSE=0\n");
    printf ("---------------: FALSE =0,\n");
    printf ("---------------: TRUE =1,\n");
    printf ("PVR_EnableStillImageZapping [Set]\n");
    printf ("-----Set : TRUE=1/FALSE=0\n");
    printf ("---------------: FALSE =0,\n");
    printf ("---------------: TRUE =1,\n");
    printf ("PVR_SpeedCheck\n");
    printf ("**********************End of PVR function List**********************\n");
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set  timeshift record duration, need to set before timeshift record start
///           It is "not must option" for timeshift record setting.
///           Timeshift record duration is dominated by timeshift file size if timeshift file duration is not setting.
///           If both timeshift file duration and size is set, timeshift file size dominate to which condition first achieve.
/// @param[in] u32PlanRecTimeInMin
///         Time-Shift Record Time Duration, 10~240 Minutes , Unit: Minutes
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b appDemo_PVR_SetTimeshiftRecDuration \n
///
/// Sample Command:PVR_TimeshiftRecordDuration 1
//------------------------------------------------------------------------------

MS_BOOL appDemo_PVR_SetTimeshiftRecDuration(MS_U32 *u32PlanRecTimeInMin)
{

    if((MApi_PVR_IsTimeShift())&&(_u8TimeshiftIdx!=0xFF))
    {
        printf("Cannot set TimeShift duration during timeshift recording\n");
        return FALSE;
    }

    printf("TimeShift Rec Time Duration:%lu\n",*u32PlanRecTimeInMin);

    return MApi_PVR_SetTimeshiftFileDuration(*u32PlanRecTimeInMin * 60);

}


#ifdef PVR_DBG_LINUX
MS_BOOL appDemo_Test_Cmd(char *cmd)
{
    FILE * fp;
    char buffer[80];
    if((fp=popen(cmd,"r"))!=NULL)
    {
         while(fgets(buffer,sizeof(buffer),fp)!=NULL)
         {
             printf("%s",buffer);
         }
         pclose(fp);
    }
    return TRUE;
}


#endif

static void _appdemo_PVRDBG_Task(void)
{
   _appdemo_PVR_DBG_MSG();
}

static MS_BOOL _appdemo_PVR_DBGCreateTask(void)
{


    void *pStackbase = NULL;

    /* Create task for PVR DBG */
    if (pStackbase == NULL)
    {
       pStackbase = MsOS_AllocateMemory(TASK_STACK_SIZE, gs32NonCachedPoolID);
       printf("[%s][%d] \n",__FUNCTION__, __LINE__);
    }

    if (pStackbase == NULL)
    {
       printf("[%s][%d] \n",__FUNCTION__, __LINE__);
       return FALSE;
    }

    if (_s32DbgPVRTaskID == INVALID_TASK_ID)
    {
       _s32DbgPVRTaskID = MsOS_CreateTask((TaskEntry)_appdemo_PVRDBG_Task,
                                 0,
                                 E_TASK_PRI_MEDIUM, // E_TASK_PRI_MEDIUM,
                                 TRUE,
                                 pStackbase,
                                 TASK_STACK_SIZE,
                                 "_PVR_DBG");

    if (INVALID_TASK_ID == _s32DbgPVRTaskID)
    {
       printf("[%s][%d] Create PVR DBG Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
       return FALSE;
    }
    else
    {
       printf("[%s][%d] s32DbgPVRTaskID %ld \n",__FUNCTION__, __LINE__, _s32DbgPVRTaskID);
    }
    }
    return TRUE;
}

MS_BOOL appDemo_PVR_SetDBGLevel(MS_U8 *dbglevel , MS_U32 *intervelTime)
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("0:No DEBUG\n");
    printf("Bit0:Vdec\n");
    printf("Bit2:Audio\n");
    printf("Bit4:DMX Record     Bit5:DMX Play\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    _u8DBGLevel = *dbglevel;
    if(*intervelTime == 0 || *intervelTime >= 5000)
    {
       _u32DBGIntervel = 1000;
    }
    else
    {
       _u32DBGIntervel = *intervelTime;
    }
    printf("[DemoPVR]Now DBG LEVEL is %d Intervel:%8lu\n",_u8DBGLevel,_u32DBGIntervel);

    if(_u8DBGLevel & 0x4)
    {
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Dump_Dsp_Info, 0x15, (MS_U16)*intervelTime);
    }

    if(_u8DBGLevel > 0 && _s32DbgPVRTaskID == INVALID_TASK_ID)
    {
        appDemoPvr_print("_u8DBGLevel  : %d\n",_u8DBGLevel);
        if(!_appdemo_PVR_DBGCreateTask())
        {
            printf("create PVR DBG task fail!!!!!!!!!!! OH NO \n");
        }
    }
    else if(_u8DBGLevel == 0 && _s32DbgPVRTaskID != INVALID_TASK_ID)
    {
        appDemoPvr_print("close the pvr debug task\n");
        if(MsOS_DeleteTask(_s32DbgPVRTaskID))
        {
            _s32DbgPVRTaskID = INVALID_TASK_ID;
        }

    }
    else
    {
        printf("Skip the setting!!\n");
    }
    return TRUE;
}

static void _appdemo_PVR_DBG_MSG(void)
{


    MS_U32 tempsystime=0, cursystime=0;
    VDEC_EX_Status vdecStatus;
    MS_U32 u32tsEmptyCmdQSlot = 0, u32tsFileInTS = 0, u32TS=0;
    MS_U32 upstc=0, dwnstc=0;
    MS_U32 upspcr=0, dwnpcr=0;
    MS_U32 pu32Write=0, pu32Stamp=0;
    MS_U32 video_fifo = 0, audio_fifo = 0;

    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    while(1)
    {
        if(_u8DBGLevel!=0)
        {
            cursystime = MsOS_GetSystemTime();
            printf("systime :%8lu  diff:%8lu\n",cursystime,cursystime-tempsystime);
            tempsystime = cursystime;
            //video
            if(_u8DBGLevel & 0x1) //Main stream
            {
                memset(&vdecStatus, 0, sizeof(VDEC_EX_Status));
                MApi_VDEC_EX_GetStatus(stVDECStreamId,&vdecStatus);
                if(TRUE == vdecStatus.bInit)
                {
                    printf("==========================Video Stream==========================\n");
                    printf("VPTS:%8lX Diff:%8llX(Seq:%8lX I:%8lX 1stF:%8lX)\nAV(on:%8lX Start:%8lX Reach:%8lX)\n",
                        MApi_VDEC_EX_GetPTS(stVDECStreamId),MApi_VDEC_EX_GetVideoPtsStcDelta(stVDECStreamId),
                        (MS_U32)MApi_VDEC_EX_IsSeqChg(stVDECStreamId), (MS_U32)MApi_VDEC_EX_IsIFrameFound(stVDECStreamId), (MS_U32)MApi_VDEC_EX_IsFrameRdy(stVDECStreamId),
                        (MS_U32)MApi_VDEC_EX_IsAVSyncOn(stVDECStreamId), (MS_U32)MApi_VDEC_EX_IsStartSync(stVDECStreamId), (MS_U32)MApi_VDEC_EX_IsReachSync(stVDECStreamId));
                    printf("ES(R:%8lX W:%8lX) ( Disp:%8lX Dec:%8lX Err:%8lX Skip:%8lX Drop:%8lX)\n",MApi_VDEC_EX_GetESReadPtr(stVDECStreamId),  MApi_VDEC_EX_GetESWritePtr(stVDECStreamId), MApi_VDEC_EX_GetDispCnt(stVDECStreamId),
                        MApi_VDEC_EX_GetFrameCnt(stVDECStreamId), MApi_VDEC_EX_GetErrCnt(stVDECStreamId), MApi_VDEC_EX_GetSkipCnt(stVDECStreamId), MApi_VDEC_EX_GetDropCnt(stVDECStreamId));
                }
            }

            //audio
            if(_u8DBGLevel & 0x4) //Main Audio stream
            {
                printf("===========================ADEC Info============================\n");
                MApi_Audio_Monitor();
            }

            //dmx
            if(_u8DBGLevel & 0x10) //DMX record
            {
                printf("========================DMX Record Info=========================\n");
                MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_EGN0,&pu32Write);
                MApi_DMX_Pvr_Eng_GetRecordStamp(DMX_PVR_EGN0, &pu32Stamp);
                printf("Engine[0] Write:%8lX RecordStamp%8lu \n", pu32Write, pu32Stamp);
            }

            if(_u8DBGLevel & 0x20) //DMX play
            {
                printf("========================DMX Playback Info=======================\n");
                MApi_DMX_Filein_CMDQ_GetEmptyNum(&u32tsEmptyCmdQSlot);
                MApi_DMX_GetFileInTimeStamp(&u32tsFileInTS);
                MApi_DMX_Pvr_GetPlaybackStamp(&u32TS);
                printf("IDLE:%d EmptyNum:%8lX FileInTS:%8lu PlaybackTS:%8lu\n", MApi_DMX_Filein_IsIdle(), u32tsEmptyCmdQSlot, u32tsFileInTS, u32TS);
                MApi_DMX_Stc_Get(&upstc, &dwnstc);
                MApi_DMX_Pcr_Get(&upspcr, &dwnpcr);
                printf("TSP [STC:PCR] => %8lX:%8lX\n", dwnstc, dwnpcr);
                MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO, &video_fifo);
                MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO, &audio_fifo);
                printf("VIDEO State: %lu  AUDIO State: %lu\n", video_fifo, audio_fifo);
                printf("VIDEO LEVEL: %lu  AUDIO LEVEL: %lu\n", video_fifo&DMX_FIFO_STATUS_LV_MASK, audio_fifo&DMX_FIFO_STATUS_LV_MASK);
                printf("VIDEO EMPTY: %lu  AUDIO EMPTY: %lu\n", video_fifo&DMX_FIFO_STATUS_LV_EMPTY, audio_fifo&DMX_FIFO_STATUS_LV_EMPTY);

            }
            printf("--------------------------------------------------------------------------------------------------------\n");

        }
        MsOS_DelayTask(_u32DBGIntervel);

    }
}



#endif /* ENABLE_PVR_PACKAGE */

