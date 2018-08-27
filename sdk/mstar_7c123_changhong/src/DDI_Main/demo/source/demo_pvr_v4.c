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
/// @file   Demo_PVR.c
/// @author MStar Semiconductor Inc.
/// @brief  PVR Function Sample Demo
///
/// @name PVR Function Sample Command Usage
/// @verbatim
/// ----------------------------------------------------------------------------
/// Note: DigiTuner_SetFreq must be called to set tuner/demod before any operations
///=================================================================================================
#if ((DEMO_PVR_TEST == 1) && (DEMO_PVR_V4_TEST == 1))
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>

#include "MsCommon.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#if (DEMO_PVR_LINUX_MOUNT_NOTIFIER_TEST == 1)
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/prctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/netlink.h>
#endif

#include "apiDMX.h"
#include "apiPVR.h"
#include "apiAUDIO.h"
#include "MsMemory.h"
#include "demo_pvr.h"
#include "demo_av.h"
#include "demo_vdec.h"

#include "demo_dmx.h"
#include "demo_xc.h"

#include "MsFS.h"
//Drv
#include "drvSYS.h"
//MApi
#include "apiXC.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#endif

#include "Panel.h"
#include "demo_utility.h"
#if(DEMO_AUDIO_TEST == 1)
#include "demo_audio.h"
#endif

#include "apiGFX.h"
#include "apiGOP.h"
#include "apiPNL.h"

#include "drvDSCMB.h"

#if (DEMO_DSCMB_ADVANCED_TEST == 1)
#include "demo_keyladder.h"
#include "demo_dscmb_advanced.h"
#elif (DEMO_DSCMB_BASIC_TEST == 1)
#include "demo_dscmb_basic.h"
#endif

#if (DEMO_AESDMA_TEST == 1)
#include "demo_aesdma.h"
#endif

#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
#include "PVRPL_CA.h" // for DEMO_PVR_SUPPORT_DYNAMICKEY_TEST
#endif

#if((DEMO_XC_DIPMultiWin_TEST == 1) && (DEMO_VDEC_NDECODE_TEST == 1))
#define DEMO_PVR_VSYNC_MODULE_ENABLE 1
#else
#define DEMO_PVR_VSYNC_MODULE_ENABLE 0
#endif

#include "demo_zapping.h"
#include "drvDTC.h"

#define PVR_DBG 0
#if PVR_DBG
#define DemoPvr_print(fmt, args...)   { printf("[DemoPvr][%06d]\t", __LINE__); printf( fmt, ## args); }
#else
#define DemoPvr_print(fmt, args...)
#endif

#define PVR_EVENT_DBG 0

#if PVR_EVENT_DBG
#define DemoPvrEvent_print(fmt, args...)  { printf("[DemoPvr][%s][%06d]    ",__FUNCTION__, __LINE__); printf( fmt, ## args); }
#else
#define DemoPvrEvent_print(fmt, args...)
#endif

#define PVR_ERROR 1
#if PVR_ERROR
#define DemoPvr_Errorprint(fmt, args...)   { printf("[DemoPvr][ERROR][%s][%06d]\t",__FUNCTION__,__LINE__); printf( fmt, ## args); }
#else
#define DemoPvr_Errorprint(fmt, args...)
#endif

#if (DEMO_DMX_NEW_ARCHI_TEST == 1)
//Support DSCMB connect/dis-connect by DSCMB callback function , that is, DscmbConnect and DscmbDisConnect.
//Why use DSCMB callback function,
//For (DEMO_DMX_NEW_ARCHI_TEST == 1), Kaiser, Keltic, Keres...etc.
//The record pid fileter id is given from MApi_DMX_Pvr_Eng_Pid_Open(), not MApi_DMX_Pid()
// in this case , DSCMB connect/disconnect flow is  executed in pvr dscmb callback function(DscmbConnect/DscmbDisConnect)
#define DEMO_PVR_DSCMB_CALLBACK_MODE
#endif

#if (DEMO_DSCMB_ADVANCED_TEST == 1)
//(DEMO_DSCMB_ADVANCED_TEST == 1) means HWCA support(DMA Encrypt/Descrypt Key source from Key-ladder),
//instead of SWCA support (DMA Encrypt/Descrypt Key source from CPU(User) key)
#define DEMO_PVR_DSCMB_SUPPORT_KL
#endif

/* PVR_DOWNLOAD   */
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
#define PVR_DOWNLOAD_LEN                                       0x0000500040  /* 5.0 M*/
#else
#define PVR_DOWNLOAD_LEN                                       0x0000360000  /* 3.375 M*/
#endif
/* PVR_UPLOAD   */
#define PVR_UPLOAD_LEN                                         0x0000120000 /* 1.125 M*/

#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
#define PVR_SWDOWNLOAD_LEN                              0x0000360000         /* 3.375 M*/
#endif

#define PVR_MAX_RECORDING_FILE EN_APIPVR_FOUR_TUNER /*4*/

#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
#define MAX_PLAYBACK_NUM 2
#else
#define MAX_PLAYBACK_NUM 1
#endif

#define APIPVR_PLAYBACK_SPEED_1X  1000
#define PVR_FILE_SIZE_TO_STOP     (30*1024) /*30M space for USB reserver */
#define MIN_TIME_SHIFT_DISK_SPACE_IN_KB (512*1024)  /* 512MB */
#define INVALID_PID_ID 0x1FFF
#define INVALID_FILTER_ID                       0xFF
#define INVALID_RECORD_INDEX                       0xFF
#define PVR_IS_VALID_HANDLER(x) (x!=APIPVR_INVALID_HANDLER)



#define JumpToTimeMaxRange 429496  // 106Hr 2Min 48Sec
#define PVR_DSCMB_INFO_CW_SIZE 32

//Dwin buffer size set to panel width and height to capture the whole frame
#define PVR_THUMBNAIL_CAPTURE_WIDTH     (g_IPanel.Width()) /* 1920 */
#define PVR_THUMBNAIL_CAPTURE_HEIGHT   (g_IPanel.Height()) /*1080 */
#define PVR_THUMBNAIL_SURFACE_SIZE    (PVR_THUMBNAIL_CAPTURE_WIDTH*PVR_THUMBNAIL_CAPTURE_HEIGHT*2)
#define PVR_THUMBNAIL_DISPLAY_X     0
#define PVR_THUMBNAIL_DISPLAY_Y    0
#define PVR_THUMBNAIL_DISPLAY_WIDTH (PVR_THUMBNAIL_CAPTURE_WIDTH>>1)
#define PVR_THUMBNAIL_DISPLAY_HEIGHT (PVR_THUMBNAIL_CAPTURE_HEIGHT>>1)
#define PVR_THUMBNAIL_FRAME_BUFFER_FORMAT E_MS_FMT_ARGB4444


#define DEMO_PVR_CHECK_INIT()\
    if(!_bPvrInit)\
    {\
        DemoPvr_Errorprint("Not initialize PVR!\n");\
        return FALSE;\
    }\

#define DEMO_PVR_CHECK_REC_IDX(rec_idx, ret)\
    if(rec_idx >= _u8TotalRecorder)\
    {\
        DemoPvr_Errorprint("[%s][%d]Invalid Record Idx\n",__FILE__,__LINE__);\
        return ret;\
    }\

#define DEMO_PVR_CHECK_PB(pb, ret)\
    if(pb >= MAX_PLAYBACK_NUM)\
    {\
        DemoPvr_Errorprint("[%s][%d]Invalid Playback\n",__FILE__,__LINE__);\
        return ret;\
    }\


#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
#define check_sub(u32DeviceID,peDevice,C,D)\
    else if(peDevice == C) \
    {\
         u32DeviceID = D; \
    }
#else
#define check_sub(u32DeviceID,peDevice,C,D)
#endif

#define Demo_PVR_Check_AVPath(u32DeviceID, peDevice,A,B,C,D) \
    if (peDevice == A) \
    { \
        u32DeviceID = B; \
    } \
    check_sub(u32DeviceID, peDevice,C,D)\
    else \
    { \
        printf("[%s][%d]Error AVPATH(%d)!\n", __FUNCTION__, __LINE__, peDevice); \
        return FALSE; \
    }
#define DEMO_PVR_CHECK_MountPath(mountPath, ret)\
    mountPath = (char *)MApi_PVR_GetMouthPath();\
    if(mountPath == NULL)\
    {\
        printf("[%s][%d] mountPath == NULL\n",__FUNCTION__,__LINE__);\
        return ret;\
    }\

typedef enum
{
    EN_DEMO_PVR_PLAYBACK_PAUSE,
    EN_DEMO_PVR_PLAYBACK_RESUME,
    EN_DEMO_PVR_PLAYBACK_SLOWFORWARD,
    EN_DEMO_PVR_PLAYBACK_FASTFORWARD,
    EN_DEMO_PVR_PLAYBACK_FASTBACKWARD,
    EN_DEMO_PVR_PLAYBACK_SETSPEED,
} DEMO_PVR_PLAYBACK_STATE;


typedef enum
{
    PVR_PATH_MAIN,
    PVR_PATH_SUB,
    PVR_PATH_INVALID,
} Ex_PVRPlaybackPath;

typedef enum
{
    E_PVR_MEM_MALLOC_SUCCESS,
    E_PVR_MEM_MALLOC_FAIL,
    E_PVR_MEM_MALLOC_INVALID,
} EN_PVR_MEM_MALLOC;

typedef struct PVR_DualAV_LIVE_TYPE_s
{
    MS_BOOL    (*DualAV_Tuner_Config)(EN_AV_Device* peDevice, EN_DEMO_DMX_FLOW* pePlayback, EN_DEMO_DMX_FLOW_INPUT* peDmxInput, int* pClkInv, int* pExtSync, int* pParal);
    MS_BOOL    (*DualAV_TSP_SetPid)(EN_AV_Device* peDevice, MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
    MS_BOOL    (*DualAV_PlayAV)(EN_AV_Device* peDevice);
    MS_BOOL    (*DualAV_StopAV)(EN_AV_Device* peDevice);
    MS_BOOL    (*DualAV_StopAVFIFO)(EN_AV_Device* peDevice);
    MS_BOOL    (*DualAV_PlayRadio)(EN_AV_Device* peDevice);
    MS_BOOL    (*DualAV_StopRadio)(EN_AV_Device* peDevice);

} PVR_DualAV_LIVE_TYPE_t;

typedef struct PVR_DualAV_FILE_TYPE_s
{
    MS_BOOL    (*FileIn_FlowSet)(Ex_PVRPlaybackPath* peAVPATH, int* pClkInv, int* pExtSync, int* pParal);
    MS_BOOL    (*FileIn_SetPid)(Ex_PVRPlaybackPath* peAVPATH, MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
    MS_BOOL    (*FileIn_PlayAV)(Ex_PVRPlaybackPath* peAVPATH);
    MS_BOOL    (*FileIn_StopAV)(Ex_PVRPlaybackPath* peAVPATH);
} PVR_DualAV_FILE_TYPE_t;

typedef struct
{
    EN_GOP_DST_TYPE GopDest;
    MS_U8 GeWinId;
    MS_U8 u8FBId;
    MS_U16 GopBitmapWidth;
    MS_U16 GopBitmapHeight;
    MS_U16 GeDstFmt;
}GWinInfo;

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

typedef struct
{
    MS_BOOL bInited;
    MS_U8       u8VideoFilter;
    MS_U8       u8AudioFilter;
    EN_PCR_ENG  ePCREngID;
    MS_U16      u16VideoPID;
    MS_U16      u16AudioPID;
    MS_U16      u16PCRPID;
    VDEC_StreamId stVDECSteamID;
    EN_VDEC_DDI_CodecType eVideoCodec;
    EN_AUDIO_CODEC_TYPE   eAudioCodec;
    EN_DEMO_DMX_FLT_TYPE eVideoFIFO_ID;
    EN_DEMO_DMX_FLT_TYPE eAudioFIFO_ID;
    EN_DEMO_DMX_FLOW eFlow;
}PVR_AV_INFO;

typedef enum
{
    E_DEMO_PVR_DMX_OPEN_OPENFLT,
    E_DEMO_PVR_DMX_OPEN_SETPID,
    E_DEMO_PVR_DMX_OPEN_CONNECT_DSCMB,
}DEMO_PVR_DMX_OPEN_STATE;

typedef struct
{
    MS_BOOL bDSCMBInfoSet;
    MS_BOOL bConnectStatus;
    MS_U32 u32DscmbId;
    MS_U8 u8Type[PVR_DSCMB_INFO_CW_SIZE];
    MS_U8 u8StrOddCW[PVR_DSCMB_INFO_CW_SIZE];
    MS_U8 u8StrEvenCW[PVR_DSCMB_INFO_CW_SIZE];
} PVR_DSCMB_INFO;

typedef enum
{
    E_DEMO_PVR_DMAEncrypt_KSRC_CPU,
    E_DEMO_PVR_DMAEncrypt_KSRC_KL,
    E_DEMO_PVR_DMAEncrypt_KSRC_INVALID,
}DEMO_PVR_DMAEncrypt_KSRC;

static MS_BOOL b2ndEncryptionEnabled = TRUE;

static PVR_DSCMB_INFO stPVRRecordDSCMBInfo[PVR_MAX_RECORDING_FILE];
static PVR_DSCMB_INFO stPVRLiveDSCMBInfo[MAX_PLAYBACK_NUM];
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
static PVR_DSCMB_INFO stPVRBackRecordDSCMBInfo[PVR_MAX_RECORDING_FILE];
#endif
#endif
#if (DEMO_DSCMB_ADVANCED_TEST == 1)
static MS_U32 u32PVRDSCMBCAVid = 0xF;
#endif

/* PVR_DOWNLOAD   */
static void *            _pRec = NULL;
/* PVR_UPLOAD   */
static void *            _pPlay = NULL;
#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
static void *            _pSWRec = NULL;
#endif

static APIPVR_RECORD_TYPE _enRecordType= EN_APIPVR_RECORD_TYPE_SINGLE;
static MS_U8 _u8TotalRecorder = 1;

#define INVALID_TASK_ID -1

//For Debug Task
#if defined(MSOS_TYPE_ECOS)
#define PVR_DBG_TASK_STACK_SIZE 0x2000
#elif defined(MSOS_TYPE_LINUX)
#define PVR_DBG_TASK_STACK_SIZE 0 //use pthread default stack
#endif

static Task_Info _pvrDBGTask = {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, NULL, PVR_DBG_TASK_STACK_SIZE, "PVR_DBG_Task"};


#if defined(MSOS_TYPE_ECOS)
#define PVR_EVENT_TASK_STACK_SIZE 0x00004000
#elif defined(MSOS_TYPE_LINUX)
#define PVR_EVENT_TASK_STACK_SIZE 0 //use pthread default stack
#endif

// For performance test
static MS_BOOL bPVRPerformanceTest = FALSE;
static void _Demo_PVR_PerformanceTest(MS_U32 u32Time);
extern MS_BOOL MApi_PVR_GetReadWritePerformanceReport(MS_BOOL bIsPlayback, MS_U8 u8PathIdx, MS_U8 u8ReportType);
#define CPU_USAGE_START_MEASURE 30  // 30sec

static MS_BOOL bEnabled = FALSE;
static MS_U32 u32MeasurementPeriod = 30; //// default 30 minutes
#if defined(MSOS_TYPE_LINUX)
#define STAT_FILE_BUFFER_LEN 257
static FILE *fpStatFile = NULL;
static char stringBuffer[STAT_FILE_BUFFER_LEN];
typedef struct
{
    MS_U64 u64User;
    MS_U64 u64Nice;
    MS_U64 u64System;
    MS_U64 u64Idle;
    MS_U64 u64IOWait;
    MS_U64 u64Irq;
    MS_U64 u64SoftIrq;
    MS_U64 u64StealStolen;
    MS_U64 u64Guest;
} PVR_CPU_INFO;
static PVR_CPU_INFO StartCPUTime = {};
static PVR_CPU_INFO EndCPUTime = {};
#elif defined(MSOS_TYPE_ECOS)
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_private_misc.h>
extern bool GetUsageActive;
extern Task_Usage_Info TaskUsageInfo[CYGDBG_STACK_CHECK_TASK_TOTAL_NUM];
static cyg_alarm alarm_s;
// The count in Task_Usage_Info is size of U32, and the count is from CPU clock count.
// If there is only one thread running on 1GHz CPU,
// the count would overflow in 4G / 1G = 4sec
#define CPU_USAGE_MEASURE_INTERVAL 1000  // 1sec
#define CPU_USAGE_MEASURE_DURATION 20 * 60  // 20min
static MS_U64 TaskUsageSum[CYGDBG_STACK_CHECK_TASK_TOTAL_NUM];
static char *TaskNameArray[CYGDBG_STACK_CHECK_TASK_TOTAL_NUM];
static MS_U64 u64TotalSum = 0;
#endif

// For Sync Config
MS_BOOL bSyncConfigHasBeenSet = FALSE;
#if defined(MSOS_TYPE_LINUX)
static void _ExecuteCommand(char *cmd, char *buffer, MS_U32 u32Length)
{
    FILE *pipe = popen(cmd, "r");
    if(!pipe)
    {
        printf("popen failed!\n");
    }
    else
    {
        if(buffer)
        {
            fgets(buffer, u32Length, pipe);
            buffer[strcspn(buffer, "\n")] = 0;
        }
        pclose(pipe);
    }

}

#define MAX_PROC_BUFFER_SIZE 32
#define MAX_CMD_BUFFER_SIZE 64
char origDirtyBackgroundRatio[MAX_PROC_BUFFER_SIZE];
char origDirtyRatio[MAX_PROC_BUFFER_SIZE];
char origDirtyExpireCentisecs[MAX_PROC_BUFFER_SIZE];
char origDirtyWritebackCentisecs[MAX_PROC_BUFFER_SIZE];
#endif

#define PVR_EVENT_DESTROY_PROC_EXIT 0x00000001
#define PVR_DYNAMIC_KEY_DESTROY_PROC_EXIT 0x00000002
#define PVR_MOUNT_DESTROY_PROC_EXIT 0x00000003

static MS_BOOL _gbPvrEventExit = FALSE;
static MS_U8 u8PvrEventTaskStack[PVR_EVENT_TASK_STACK_SIZE];
static MS_S32 _gs32PvrEvtGrp = -1;
static Task_Info _pvrEventTask = {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, u8PvrEventTaskStack, PVR_EVENT_TASK_STACK_SIZE, "PVR_EVT_Task"};

#if (DEMO_PVR_LINUX_MOUNT_NOTIFIER_TEST == 1)
#define PVR_MOUNTNOTIFIER_TASK_STACK         0
#define DIR_PATH_SIZE 50
#define PVR_MOUNT_BUFFER_SIZE 1024*2
static MS_BOOL _gbPvrMntExit = FALSE;
static Task_Info _pvrMNTTask = {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, NULL, PVR_MOUNTNOTIFIER_TASK_STACK, "PVR_MNT_Task"};
static MS_S32 _gs32PvrMntGrp = -1;
#endif

//For Unplugging USB
static MS_BOOL _bUplugMsg=FALSE;
static char _moutPath[FILE_PATH_SIZE]={0};
// time
static MS_BOOL _bShowPlayTime=FALSE;
static MS_BOOL _bShowRecordTime=FALSE;
static MS_BOOL _bShowRecordStartTime=FALSE;
//For PVR loop
#define INVALID_EVENT_ID    -1
#define PVR_DBG_LOOP_EVENT               0x00000001
static MS_S32 s32_PVRLoopEvent_id = INVALID_EVENT_ID;
//For PVR
static MS_BOOL _bPvrInit= FALSE;
static MS_S32 _s32QueueID = -1;
// For merge stream
static EN_DEMO_DMX_FLT_SOURCEID _eCurDMXFltSourceID[PVR_MAX_RECORDING_FILE]={E_DMX_FLT_SOURCEID_0};
// For Slice
#define PVR_FILE_SIZE_MAX 0xFFFFFFFF
static MS_U64 _u64FileSliceSize = 0;
static MS_U64 _u64FileSizeInKB = PVR_FILE_SIZE_MAX;
static MS_U32 _u32Duration = 0;
static MS_BOOL _bBoundedLinearRecord = FALSE;
static MS_BOOL _bAppend = FALSE;

//For DynamicKey auto set key
#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
static MS_U32 _u8RecordIndex = 0;
#define PVR_DYNAMIC_KEY_MAX_INTERVAL 10000
static MS_U32 _u32DynamicKeyIntervel[PVR_MAX_RECORDING_FILE] = {1000,1000,1000,1000};
static MS_U8 _u8DynamicKeyLevel[PVR_MAX_RECORDING_FILE] = {0,0,0,0};

static MS_S32 _gs32PvrDynamicKeyGrp = -1;
#if defined(MSOS_TYPE_ECOS)
#define PVR_DYNAMIC_KEY_TASK_STACK_SIZE 0x2000
#elif defined(MSOS_TYPE_LINUX)
#define PVR_DYNAMIC_KEY_TASK_STACK_SIZE 0
//use pthread default stack
#endif
static MS_BOOL _gbPvrDynamicKeyExit[PVR_MAX_RECORDING_FILE] = {FALSE, FALSE, FALSE,FALSE};
static Task_Info _gThreadDynamicKeyProcess[PVR_MAX_RECORDING_FILE] =
{
    {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, NULL, PVR_DYNAMIC_KEY_TASK_STACK_SIZE, "dynamicKey1"},
    {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, NULL, PVR_DYNAMIC_KEY_TASK_STACK_SIZE, "dynamicKey2"},
    {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, NULL, PVR_DYNAMIC_KEY_TASK_STACK_SIZE, "dynamicKey3"},
    {INVALID_TASK_ID, E_TASK_PRI_MEDIUM, NULL, PVR_DYNAMIC_KEY_TASK_STACK_SIZE, "dynamicKey4"},
};
#endif
#if defined(MSOS_TYPE_ECOS)
static void _Demo_PollingThreadStatus(Task_Info stTaskInfo)
{

    TaskStatus eTaskStatus;
    while(1)    {
        if(!MsOS_GetTaskStatus(&stTaskInfo,&eTaskStatus))
        {
            printf("Error!! MsOS_GetTaskStatus() fail!! \n");
            break;
        }
        if(eTaskStatus == E_TASK_NOT_EXIST)
            break;
        else
        {
            printf("\33[32m[FUNC %s] [LINE %d] Wait for Task#%x %s termination \33[m \n",__FUNCTION__,__LINE__,stTaskInfo.iId,stTaskInfo.szName);
            MsOS_DelayTask(1);
        }
    }

}
#endif





static MS_U8 _u8CurRecProgramIdx=0;
static EN_DEMO_DMX_FLOW_INPUT _eCurDMXInputSrc=E_DMX_FLOW_INPUT_DEMOD0;
static EN_DEMO_DMX_FLOW _eCurDMXFlowSet[PVR_MAX_RECORDING_FILE]={E_DMX_FLOW_LIVE0};

static const VDEC_StreamId _NullVidStreamID = {0};
static const AUDIO_DEC_ID  _NullAudStreamID = AU_DEC_INVALID;

static VDEC_StreamId _VidStreamID[MAX_PLAYBACK_NUM];
static AUDIO_DEC_ID  _AudStreamID[MAX_PLAYBACK_NUM];

static MS_U8 _u8PcrDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8VidDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8AudDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_AUDIOINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8TtxDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_TTXINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8CcDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8EBUSubtDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_SUBTITLEINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8DVBSubtDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_SUBTITLEINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};

static Ex_PVRPlaybackPath _eCurPlaybackProgramPath = PVR_PATH_MAIN;
static MS_U8 _u8TotalPlaybackProgram=0;

static MS_U8 _u8TimeshiftRecordIdx[MAX_PLAYBACK_NUM]={INVALID_RECORD_INDEX};
static MS_U8 _u8TimeshiftPlaybackIdx[MAX_PLAYBACK_NUM]={PVR_PATH_INVALID};
static MS_U8 _u8hRecord[PVR_MAX_RECORDING_FILE]={APIPVR_INVALID_HANDLER};
static MS_U8 _u8hPlayback[MAX_PLAYBACK_NUM]={APIPVR_INVALID_HANDLER};
static PVRProgramInfo_t _recprogramInfo[PVR_MAX_RECORDING_FILE];
static PVRProgramInfo_t _plyprogramInfo[MAX_PLAYBACK_NUM];
static PVRProgramInfo_t _livePromInfo[MAX_PLAYBACK_NUM];
static APIPVR_PLAYBACK_TYPE _enPlayType= EN_APIPVR_PLAYBACK_TYPE_BASICPLAYER;

static MS_BOOL _bScreenFrozen[MAX_PLAYBACK_NUM];

static GWinInfo _gWinInfo={0};
static MS_BOOL bGOP_init = FALSE;

static PVR_AV_ID_t _AVStreamID[MAX_PLAYBACK_NUM];

static PVR_AV_INFO stFileAVInfo[MAX_PLAYBACK_NUM];
static ST_AV_INFO _stLiveInfo[MAX_PLAYBACK_NUM];

static MS_U8 _u8DBGLevel=0;
static MS_U32 _u32DBGIntervel=1000;
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
static MS_BOOL _bTimeshiftSeamless[MAX_PLAYBACK_NUM];
static MS_BOOL _bBackgroundRecord[PVR_MAX_RECORDING_FILE];
static MS_U8 _u8BackProgramIdx=0;
static PVRProgramInfo_t _backrecprogramInfo[PVR_MAX_RECORDING_FILE];
static MS_U8 _u8BackPcrDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8BackVidDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8BackAudDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_AUDIOINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8BackTtxDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_TTXINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8BackCcDmxId[PVR_MAX_RECORDING_FILE]={INVALID_FILTER_ID};
static MS_U8 _u8BackEBUSubtDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_SUBTITLEINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
static MS_U8 _u8BackDVBSubtDmxId[PVR_MAX_RECORDING_FILE][APIPVR_MAX_SUBTITLEINFO_NUM]={{INVALID_FILTER_ID},{INVALID_FILTER_ID}};
#endif

static MS_BOOL _bAutoScriptDbg = FALSE;
static MS_BOOL _bAutoScriptJumpToTime = FALSE;
static MS_U32 _u32PlaybackTime[MAX_PLAYBACK_NUM] = {0};
#define PVR_PLAYBACKTIME_INTERVAL 7200 // 2 hours is an arbitrary number that we use to judge if play time changes too fast
#define PVR_AUTO_SCRIPT_CMD 0x87


//--------------------------------------------------------------------------------------------------
// API prototype
//--------------------------------------------------------------------------------------------------
//record start
static MS_BOOL _Demo_PVR_SetRecordType(MS_U32 u32RecordType);
static MS_BOOL _Demo_PVR_FileIn_FlowSet(Ex_PVRPlaybackPath* peAVPATH, int* pClkInv, int* pExtSync, int* pParal);
static MS_BOOL _Demo_PVR_FileIn_SetPid(Ex_PVRPlaybackPath* peAVPATH, MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
static MS_BOOL _Demo_PVR_FileIn_PlayAV(Ex_PVRPlaybackPath* peAVPATH);
static MS_BOOL _Demo_PVR_FileIn_StopAV(Ex_PVRPlaybackPath* peAVPATH);
static MS_BOOL _Demo_PVR_LIVE_PlayAV(EN_AV_Device* peDevice);
static MS_BOOL _Demo_PVR_LIVE_StopAV(EN_AV_Device *peDevice);

static MS_BOOL _Demo_PVR_LIVE_PlayRadio(EN_AV_Device* peDevice);
static MS_BOOL _Demo_PVR_LIVE_StopRadio(EN_AV_Device *peDevice);

static MS_U32 _Demo_PVR_Get_ADECDDI_ADEC_Fmt_FromPromInfo(APIPVR_AUD_CODEC_TYPE u32ACodecPromInfo);
static MS_U32 _Demo_PVR_Get_VDECDDI_VDEC_Fmt_FromPromInfo(APIPVR_CODEC_TYPE u32VCodecPromInfo);

static MS_U16 _Demo_PVR_FindPidinFilter(PVRFilterInfo_t *pFilter,MS_U32 u32DmxFltType);
static MS_BOOL _Demo_PVR_GetFileInfo(char *FileName, PVRProgramInfo_t *promInfo);

static MS_U8 _Demo_PVR_Timeshift_GetPlaybackIdx(MS_U8 u8RecordIdx);
static MS_U8 _Demo_PVR_RecHandler2Idx(MS_U8 u8hRecord);
static MS_U8 _Demo_PVR_PlayHandler2Idx(MS_U8 u8hPlayback);
static void* _Demo_PVR_GetVidStreamID(Ex_PVRPlaybackPath *peAVPATH);
static void* _Demo_PVR_GetAudStreamID(Ex_PVRPlaybackPath *peAVPATH);

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
static MS_BOOL _Demo_PVR_Record_DSCMB_Create(PVR_DSCMB_INFO *stDSCMBInfo);
static MS_BOOL _Demo_PVR_Record_DSCMB_Destroy(PVR_DSCMB_INFO *stDSCMBInfo);
static MS_BOOL _Demo_PVR_Record_DSCMB_SetInfo(PVR_DSCMB_INFO *stDSCMBInfo);
static MS_BOOL _Demo_PVR_Record_DSCMB_Connect(PVR_DSCMB_INFO *stDSCMBInfo, MS_U16 u16PID, MS_U8 *pu8DmxFltId);
static MS_BOOL _Demo_PVR_Record_DSCMB_Disconnect(PVR_DSCMB_INFO *stDSCMBInfo, MS_U8 *pu8DmxFltId);
#endif
static MS_BOOL _Demo_PVR_DSCMB_GetLiveInfo(MS_U32 u32DeviceID, PVR_DSCMB_INFO *stDSCMBInfo);
static MS_BOOL _Demo_PVR_DSCMB_Live_Connect(MS_U32 *u32DeviceID, PVR_DSCMB_INFO *stDSCMBInfo);
static MS_BOOL _Demo_PVR_DSCMB_Live_Disconnect(MS_U32 *u32DeviceID, PVR_DSCMB_INFO *stDSCMBInfo);
static MS_BOOL _Demo_Dmx_GetLiveProgram(MS_U8 u8LivePath, MS_U32 *u32PCRPid,MS_U32 *u32VideoPid,MS_U32 *u32AudioPid,MS_U32 *u32VCodec,MS_U32 *u32ACodec,MS_U16 *pu16PtmPid,MS_U16 *pu16LCN);
static MS_BOOL _Demo_PVR_SetSyncConfig(MS_U32 *u32Value, char *DirtyBackgroundRatio, char *DirtyRatio, char *DirtyExpireCentisecs, char *DirtyWritebackCentisecs);

static void _Demo_PVRDBG_Task(void);
static EN_AV_Device _PVRPlaybackPath_Mapping_AV_Device(Ex_PVRPlaybackPath enPVRPlaybackPath);

const static PVR_DualAV_LIVE_TYPE_t _DualAVFunc = {Demo_AV_Tuner_Config,Demo_AV_TSP_SetPid,_Demo_PVR_LIVE_PlayAV,_Demo_PVR_LIVE_StopAV, Demo_AV_StopAVFIFO, _Demo_PVR_LIVE_PlayRadio, _Demo_PVR_LIVE_StopRadio};
// _DualFileFunc is only used by _Demo_PVR_ChangePMTEvent non-auto mode.
const static PVR_DualAV_FILE_TYPE_t _DualFileFunc = {_Demo_PVR_FileIn_FlowSet,_Demo_PVR_FileIn_SetPid,_Demo_PVR_FileIn_PlayAV,_Demo_PVR_FileIn_StopAV};
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
static void _Demo_PVR_DynamicKey_Task(void)
{
    MS_U32 u32Idx = _u8RecordIndex;
    MS_U32 u32Key = 0;
    MS_U32 u32Count = 0, u32IntervalSecond = 0;




    u32IntervalSecond = u32Count = _u32DynamicKeyIntervel[u32Idx]/1000;


    while(_gbPvrDynamicKeyExit[u32Idx] != TRUE )
    {
        if(_u8DynamicKeyLevel[u32Idx] != 0 )
        {
            if(TRUE == MApi_PVR_IsRecording(u32Idx) && u32Count  == u32IntervalSecond)
            {
                MApi_PVR_EX_SetEncryptKeyIdx(u32Idx, u32Key);
                u32Key = (u32Key + 1) % DYNAMIC_KEY_MAX;

            }
        }
        else
        {
            break;
        }

        MsOS_DelayTask(1000); // We check every second in case of blocking
        u32Count--;

        if(u32Count == 0)
        {

            u32Count = u32IntervalSecond;
        }

    }
    MsOS_SetEvent(_gs32PvrDynamicKeyGrp, PVR_DYNAMIC_KEY_DESTROY_PROC_EXIT);
    printf("_Demo_PVR_DynamicKey_Task terminated\n");
}

static MS_BOOL _Demo_PVR_DynamicKeyCreateTask(MS_U32 RecIdx)
{
    MS_S32 s32NonCachedPoolID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);

    /* Create task for PVR Dynamic Key */
    if(PVR_DYNAMIC_KEY_TASK_STACK_SIZE){
        if (_gThreadDynamicKeyProcess[RecIdx].pStack == NULL)
        {
            _gThreadDynamicKeyProcess[RecIdx].pStack = MsOS_AllocateMemory(PVR_DYNAMIC_KEY_TASK_STACK_SIZE,s32NonCachedPoolID);
        }

        if (_gThreadDynamicKeyProcess[RecIdx].pStack == NULL)
        {
           DemoPvr_Errorprint("[%s][%d] Allocate Memory for PVR Dynamic Key Task Failed  \n",__FUNCTION__,__LINE__);
           return FALSE;
        }
    }
    _gbPvrDynamicKeyExit[RecIdx] = FALSE;
    if (_gThreadDynamicKeyProcess[RecIdx].iId == INVALID_TASK_ID)
    {
       _gThreadDynamicKeyProcess[RecIdx].iId  = MsOS_CreateTask((TaskEntry)_Demo_PVR_DynamicKey_Task,
                                 0,
                                 _gThreadDynamicKeyProcess[RecIdx].ePriority, // E_TASK_PRI_MEDIUM,
                                 TRUE,
                                 _gThreadDynamicKeyProcess[RecIdx].pStack,
                                 _gThreadDynamicKeyProcess[RecIdx].u32StackSize,
                                 _gThreadDynamicKeyProcess[RecIdx].szName);

        if (INVALID_TASK_ID ==_gThreadDynamicKeyProcess[RecIdx].iId)
        {
           DemoPvr_Errorprint("[%s][%d] Create PVR Dynamic Key Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
           return FALSE;
        }
        else
        {
           DemoPvr_print("[%s][%d] s32DynamicKeyPVRTaskID %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,_gThreadDynamicKeyProcess[RecIdx].iId);
        }
    }
    return TRUE;
}

static MS_BOOL _Demo_PVR_SetDynamicKey(MS_U8 *idx, MS_U8 *cmd , MS_U32 *intervelTime)
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("0:No DynamicKey\n");
    printf("1:DynamicKey\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(RecIdx:CMD:Time)  (%d:%d:%"DTC_MS_U32_u")\n",*idx,*cmd,*intervelTime);
    DEMO_PVR_CHECK_INIT();

    MS_U8 _u8RecIdx = *idx;
    DEMO_PVR_CHECK_REC_IDX(_u8RecIdx, FALSE);

    _u8RecordIndex = _u8RecIdx;
    _u8DynamicKeyLevel[_u8RecIdx] = *cmd;
    if(*intervelTime == 0)
    {
       _u32DynamicKeyIntervel[_u8RecIdx] = 1000;
    }
    else
    {
       _u32DynamicKeyIntervel[_u8RecIdx] = *intervelTime;
    }

    if(_u8DynamicKeyLevel[_u8RecIdx] > 0 &&_gThreadDynamicKeyProcess[_u8RecIdx].iId == INVALID_TASK_ID)
    {

        DemoPvr_print("_u8DynamicKeyLevel  : %d\n",_u8DynamicKeyLevel[_u8RecIdx]);
        if(!_Demo_PVR_DynamicKeyCreateTask(_u8RecIdx))
        {
            DemoPvr_Errorprint("create PVR DBG task fail!!!!!!!!!!! OH NO \n");
        }
    }
    else if(_u8DynamicKeyLevel[_u8RecIdx] == 0 && _gThreadDynamicKeyProcess[_u8RecIdx].iId  != INVALID_TASK_ID)
    {
        DemoPvr_print("Close the PVR set dynamic key task\n");
        MS_U32  pu32RetrievedEventFlag;

        MS_S32 s32NonCachedPoolID;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
        _gbPvrDynamicKeyExit[_u8RecIdx] = TRUE;
        MsOS_WaitEvent(_gs32PvrDynamicKeyGrp, PVR_DYNAMIC_KEY_DESTROY_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#if defined(MSOS_TYPE_ECOS)
        _Demo_PollingThreadStatus(_gThreadDynamicKeyProcess[_u8RecIdx]);
#endif
        _gThreadDynamicKeyProcess[_u8RecIdx].iId = INVALID_TASK_ID;
        if (_gThreadDynamicKeyProcess[_u8RecIdx].pStack != NULL)
        {
            MsOS_FreeMemory(_gThreadDynamicKeyProcess[_u8RecIdx].pStack,s32NonCachedPoolID);
            _gThreadDynamicKeyProcess[_u8RecIdx].pStack = NULL;
        }
    }
    else
    {
        DemoPvr_print("Skip the setting!!\n");
    }
    return TRUE;
}
#endif

static MS_BOOL _Demo_PVR_DBGCreateTask(void)
{
    MS_S32 s32NonCachedPoolID = 0;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);

    // PVR loop event ID
    if (s32_PVRLoopEvent_id == INVALID_EVENT_ID)
    {
        DemoPvr_print("[DDI_PVR]Create event Group\n");
        s32_PVRLoopEvent_id = MsOS_CreateEventGroup("PVRloop_Event");
    }

    if (INVALID_TASK_ID == _pvrDBGTask.iId)
    {
        if(PVR_DBG_TASK_STACK_SIZE)
        {
            _pvrDBGTask.pStack = MsOS_AllocateMemory(PVR_DBG_TASK_STACK_SIZE,s32NonCachedPoolID);
            if (_pvrDBGTask.pStack == NULL)
            {
                printf("_Demo_PVR_DBGCreateTask stack malloc failed......\n");
                return FALSE;
            }
        }

        _pvrDBGTask.iId = MsOS_CreateTask((TaskEntry)_Demo_PVRDBG_Task,
                                 0,
                                 _pvrDBGTask.ePriority, // E_TASK_PRI_MEDIUM,
                                 TRUE,
                                 _pvrDBGTask.pStack,
                                 _pvrDBGTask.u32StackSize,
                                 _pvrDBGTask.szName);

        if (INVALID_TASK_ID == _pvrDBGTask.iId)
        {
           DemoPvr_Errorprint("[%s][%d] Create PVR DBG Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
           return FALSE;
        }
        else
        {
           DemoPvr_print("[%s][%d] _pvrDBGTask.iId %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,_pvrDBGTask.iId);
        }
    }
    return TRUE;
}

void* _Demo_PVR_MEMALLOC_FUNC(MS_U32 u32Size)
{
    MS_S32 s32NonCachedPoolID = 0;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
    return MsOS_AllocateMemory (u32Size, s32NonCachedPoolID);
}

static MS_BOOL _Demo_PVR_MEMFREE_FUNC(void **pBuf)
{
    if( *pBuf )
    {
        MS_S32 s32NonCachedPoolID = 0;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
        MS_BOOL bFreeMem = MsOS_FreeMemory(*pBuf, s32NonCachedPoolID);
        if( bFreeMem == TRUE)
        {
            *pBuf = NULL;
        }
        return bFreeMem;
    }
    return FALSE;
}

static int wcslen(const MS_U16 *pStr)
{
    int i = 0;

    while(pStr[i])
    {
        i++;
    }

    return i;
}

static MS_U16* wcscpy(MS_U16 *strDst, const MS_U16 *strSrc)
{
    int i;

    for(i = 0; strSrc[i]; i++)
    {
        strDst[i] = strSrc[i];
    }
    strDst[i] = '\0';
    return strDst;
}

static int wcscmp(const MS_U16 *pu16Str1, const MS_U16 *pu16Str2)
{
    int idx = 0;

    while(pu16Str1[idx] == pu16Str2[idx])
    {
        if(pu16Str1[idx] == 0)
            return 0;
        idx++;
    }

    return (int)(pu16Str1[idx] - pu16Str2[idx]);
}

static void wcscat(MS_U16 *pu16Dest, MS_U16 *pu16Src)
{
    memcpy(pu16Dest + wcslen(pu16Dest), pu16Src, (wcslen(pu16Src)+1) * 2);
}

static void wprintf(const MS_U16* pu16string, MS_BOOL bPrint, MS_BOOL bPrintNL)
{
    MS_U32  i = 0;
    MS_U8   *u8string = NULL;

    u8string = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U8)*FILE_PATH_SIZE*2);
    if (u8string == NULL)
    {
        printf("Memory Allocate Failed !!! \n");
        return ;
    }

    if (pu16string != NULL)
    {
        for (i = 0; i < wcslen(pu16string) && i < (FILE_PATH_SIZE*2); i++)  // U16 => U8
        {
            if (((pu16string[i] & 0xff00) == 0) && ((pu16string[i] & 0x00ff) >= 0x20) && ((pu16string[i] & 0x00ff) <= 0x80))
            {
                u8string[i] = pu16string[i] & 0x00ff;
            }
            else
            {
                u8string[i] = '?';
            }
        }

        u8string[i] = 0;

        if(bPrint)
        {
            printf("%s", u8string);
            if(bPrintNL)
            {
                printf("\n");
            }
        }
    }

    if (_Demo_PVR_MEMFREE_FUNC((void**)&u8string) == FALSE)
    {
        printf("Memory Free Failed !!! \n");
    }
}

static void _U8StringToU16String(MS_U8 *pu8Str, MS_U16 *pu16Str, MS_U8 u8Strlen)
{
    MS_U8 u8Index;

    if((pu8Str==NULL) || (pu16Str==NULL))
    {
        return;
    }

    for (u8Index=0; u8Index<u8Strlen; u8Index++)
    {
        pu16Str[u8Index] = pu8Str[u8Index];
    }
    pu16Str[u8Index] = 0;
}

static void _U16StringToU8String(MS_U16 *pu16Str, MS_U8 *pu8Str, MS_U8 u8Strlen)
{
    MS_U8 u8Index;

    if((pu16Str==NULL) || (pu8Str==NULL))
    {
        return;
    }

    for (u8Index=0; u8Index<u8Strlen; u8Index++)
    {
        pu8Str[u8Index] = pu16Str[u8Index];
    }
    pu8Str[u8Index] = 0;
}

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
MS_BOOL _Demo_PVR_RECORD_DSCMB_CONNECT_FUNC(MS_U8 u8DmxFltId, MS_U32 u32PID)
{
    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);

    if (_Demo_PVR_Record_DSCMB_Connect(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx], (MS_U16)u32PID, &u8DmxFltId) != TRUE)
    {
        DemoPvr_Errorprint("[%s][%d]DSCMB Connect Error with [PID:Flt][%"DTC_MS_U32_u":%u]\n",__FILE__,__LINE__,u32PID,u8DmxFltId);
        return FALSE;
    }
    return TRUE;
}

MS_BOOL _Demo_PVR_RECORD_DSCMB_DISCONNECT_FUNC(MS_U8 u8DmxFltId, MS_U32 u32PID)
{
    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);

    if (_Demo_PVR_Record_DSCMB_Disconnect(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx], &u8DmxFltId) != TRUE)
    {
        DemoPvr_Errorprint("[%s][%d]DSCMB Disconnect Error with [PID:Flt][%"DTC_MS_U32_u":%u]\n",__FILE__,__LINE__,u32PID,u8DmxFltId);
        return FALSE;
    }
    return TRUE;
}
#endif

static void _Demo_PVR_FreezeScreen(MS_BOOL bFrezon)
{
    void *PVRVDECStreamID = _Demo_PVR_GetVidStreamID(&_eCurPlaybackProgramPath);
    if (PVRVDECStreamID == NULL)
    {
        printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__);
        return;
    }
    MApi_VDEC_EX_SetFreezeDisp((VDEC_StreamId*)PVRVDECStreamID, bFrezon);
}

static MS_BOOL _Demo_PVR_FreeMem(int pvrId)
{
    switch(pvrId)
    {
        case PVR_ID_DOWNLOAD:
            DemoPvr_print("[%s][%d] _pRec=%p\n",__FUNCTION__,__LINE__,_pRec);
            if(_pRec != NULL)
            {
                DemoPvr_print("[%s][%d] --> try to free _pRec\n",__FUNCTION__,__LINE__);
                if(!_Demo_PVR_MEMFREE_FUNC(&_pRec))
                {
                    printf("_pRec Free Fail\n");
                    return FALSE;
                }
                _pRec = NULL;
            }
            break;
        case PVR_ID_UPLOAD:
            DemoPvr_print("[%s][%d] _pPlay=%p\n",__FUNCTION__,__LINE__,_pPlay);
            if(_pPlay != NULL)
            {
                DemoPvr_print("[%s][%d] --> try to free _pPlay\n",__FUNCTION__,__LINE__);
                if(!_Demo_PVR_MEMFREE_FUNC(&_pPlay))
                {
                    printf("_pPlay Free Fail\n");
                    return FALSE;
                }
                _pPlay = NULL;
            }
            break;
#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
        case  PVR_ID_SWBUFFER:
            DemoPvr_print("[%s][%d] _pSWRec=%p\n",__FUNCTION__,__LINE__,_pSWRec);
            if(_pSWRec != NULL)
            {
                DemoPvr_print("[%s][%d] --> try to free _pSWRec\n",__FUNCTION__,__LINE__);
                if(!_Demo_PVR_MEMFREE_FUNC(&_pSWRec))
                {
                    printf("_pSWRec Free Fail\n");
                    return FALSE;
                }
                _pSWRec = NULL;
            }
            break;
#endif /*DEMO_PVR_SUPPORT_SWPVR_TEST*/
        default:
            printf("[%s][%d] Should not be here. pvrId=%d\n",__FUNCTION__,__LINE__,pvrId);
            return FALSE;
    }
    return MApi_PVR_ResetMmap(pvrId);
}

static MS_BOOL _Demo_PVR_AllocateMem(int pvrId)
{
    MS_U32 u32PhyStartAdr=0;
    MS_U32 u32Size=0;

    switch(pvrId)
    {
        case PVR_ID_DOWNLOAD:
            u32Size = PVR_DOWNLOAD_LEN*_u8TotalRecorder;
            if(_pRec == NULL)
            {
                _pRec = _Demo_PVR_MEMALLOC_FUNC(u32Size);
                if(_pRec == NULL)
                {
                    printf("[%s][%d] _pRec HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pRec);

            DemoPvr_print("[%s][%d] PVR_ID_DOWNLOAD %"DTC_MS_U32_x" (%"DTC_MS_U32_x"+%"DTC_MS_U32_x"=%"DTC_MS_U32_x") \n",__FUNCTION__,__LINE__,(MS_U32)_pRec,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;
        case PVR_ID_UPLOAD:
            u32Size = PVR_UPLOAD_LEN * MAX_PLAYBACK_NUM;
            if(_pPlay == NULL)
            {
                _pPlay= _Demo_PVR_MEMALLOC_FUNC(u32Size);
                if(_pPlay == NULL)
                {
                    printf("[%s][%d] _pPlay HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pPlay);
            DemoPvr_print("[%s][%d] PVR_ID_UPLOAD %"DTC_MS_U32_x" (%"DTC_MS_U32_x"+%"DTC_MS_U32_x"=%"DTC_MS_U32_x") \n",__FUNCTION__,__LINE__,(MS_U32)_pPlay,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;
#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
        case  PVR_ID_SWBUFFER:
            if(_pSWRec == NULL)
            {
                _pSWRec = _Demo_PVR_MEMALLOC_FUNC(PVR_SWDOWNLOAD_LEN);
                if(_pSWRec == NULL)
                {
                    printf("[%s][%d] _pSWRec HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)_pSWRec);

            u32Size = PVR_SWDOWNLOAD_LEN;
            DemoPvr_print("[%s][%d] PVR_ID_SWBUFFER %"DTC_MS_U32_x" (%"DTC_MS_U32_x"+%"DTC_MS_U32_x"=%"DTC_MS_U32_x") \n",__FUNCTION__,__LINE__,_pSWRec,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size));
            break;
#endif /*DEMO_PVR_SUPPORT_SWPVR_TEST*/
        default:
            printf("[%s][%d] Should not be here. pvrId=%d\n",__FUNCTION__,__LINE__,pvrId);
            return FALSE;
    }

    if(MApi_PVR_Mmap(pvrId,u32PhyStartAdr,u32Size)==FALSE)
    {
        printf("[%s][%d] PVR Mmap failed\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return TRUE;
}

static  MS_BOOL  _Demo_PVR_CheckUSB(char *mountPath) // mount path: MS_U16 string
{
    int ret;
    DIR *dp;

    if((dp = MsFS_OpenDir((const char*)mountPath)) == NULL)
    {
        ret = MsFS_MkDir((const char*)mountPath, 0755);
        if(ret)
        {
            if(errno != EEXIST)
            {
                printf("[PVR ERROR] : fail to mkdir.\n");
                wprintf((MS_U16*)mountPath, TRUE, TRUE);
                return FALSE;
            }
            else
            {
                printf("[PVR] : mount path exists.\n");
                wprintf((MS_U16*)mountPath, TRUE, TRUE);
            }
        }
    }
    else
    {
        MsFS_CloseDir(dp);
    }
    return TRUE;
}

static MS_BOOL  _Demo_PVR_GetDiskSpace(char *mountPath,MS_U64* u64FreeSpaceInKB, MS_U64* u64TotalSpaceInKB)
{
    MApi_FsInfo fsInfo;

    if(mountPath==NULL)
    {
        printf("[%s][%d] mountPath==NULL\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        return FALSE;
    }
    DemoPvr_print("[%s][%d] mountPath=",__FUNCTION__,__LINE__);
    wprintf((MS_U16*)mountPath, PVR_DBG, TRUE);
    if(MApi_FS_Info((char *)mountPath, TRUE, &fsInfo)==TRUE)
    {
        *u64TotalSpaceInKB = (((MS_U64)fsInfo.u32ClusTotal* (MS_U64)fsInfo.u32ClusSize) / 1024);
        *u64FreeSpaceInKB = (((MS_U64)fsInfo.u32ClusFree* (MS_U64)fsInfo.u32ClusSize) / 1024);
        DemoPvr_print("*u64TotalSpaceInKB=%lld *u64FreeSpaceInKB=%lld\n",*u64TotalSpaceInKB,*u64FreeSpaceInKB);
        return TRUE;
    }
    return FALSE;
}

static  MS_BOOL _Demo_PVR_IsFileExist(char *m_mount_path, char *fileName)
{
    struct stat buf;
    char path[FILE_PATH_SIZE] = {0};
    //printf("m_mount_path %d , filename %d",strlen(m_mount_path),strlen(filename));
    //sprintf(path,"%s/%s",m_mount_path,filename);
    int size = sizeof(path);
    //sprintf(path,"%s/%s",m_mount_path,filename);
    int c = snprintf(path,size,"%s/%s.meta",m_mount_path,fileName);
    if( c > size || c < 0 )
     path[size-1] = '\0';

    char *bar = path;
    DemoPvr_print("path = %s length = %d\n",(char*)bar,strlen(bar));

    MS_U16 *pu16Path = NULL;

    pu16Path = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8 *)bar, pu16Path, strlen(bar));
    if( MsFS_Stat((const char*)pu16Path,&buf)<0)
    {
        DemoPvr_print("errno = %d\n",errno);
        _Demo_PVR_MEMFREE_FUNC((void**)&pu16Path);
        return FALSE;
    }

    _Demo_PVR_MEMFREE_FUNC((void**)&pu16Path);
    return TRUE;
}
static  void _Demo_PVR_Remove_Capture_Video(void)
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

static MS_BOOL _Demo_PVR_Show_Capture_Video(MS_U32 u32PhysicalAddr)
{
    //GE bitblt to copy DWin captured frame to GWin buffer to display
    //Gwin size maybe different to panel size; GE_Bitblt with scaling to change size to display
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;
    GFX_Point gfxPt0,gfxPt1;
    GFX_BufferInfo srcbuf;
    GFX_DrawRect bitbltInfo;

    _Demo_PVR_Remove_Capture_Video();

    MApi_GOP_GWIN_SwitchGOP(0);//use GOP 0
    MApi_GOP_GWIN_Set_STRETCHWIN(0, E_GOP_DST_OP0, 0, 0, _gWinInfo.GopBitmapWidth, _gWinInfo.GopBitmapHeight);
    _gWinInfo.u8FBId=MApi_GOP_GWIN_GetFreeFBID();
    MApi_GOP_GWIN_CreateFB(_gWinInfo.u8FBId, PVR_THUMBNAIL_DISPLAY_X, PVR_THUMBNAIL_DISPLAY_Y, _gWinInfo.GopBitmapWidth, _gWinInfo.GopBitmapHeight, PVR_THUMBNAIL_FRAME_BUFFER_FORMAT);


    _gWinInfo.GeWinId=MApi_GOP_GWIN_CreateWin_Assign_FB(0, _gWinInfo.u8FBId,PVR_THUMBNAIL_DISPLAY_X, PVR_THUMBNAIL_DISPLAY_Y);
    MApi_GOP_GWIN_SetBlending(0, FALSE, 0xff);
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);

    MApi_GOP_GWIN_MapFB2Win(_gWinInfo.u8FBId, _gWinInfo.GeWinId);
    DemoPvr_print("[%s][%d] _gWinInfo.u8FBId=%d  _gWinInfo.GeWinId=%d\n", __FUNCTION__, __LINE__,_gWinInfo.u8FBId, _gWinInfo.GeWinId);
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

static void _Demo_PVR_XC_Mute(Ex_PVRPlaybackPath* peAVPATH)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = 0;
    MS_U32 u32DisplayMute = ENABLE;
    EN_ZAPPING_TYPE eZappingType = E_ZAPPING_NORMAL;

    if(*peAVPATH == PVR_PATH_MAIN)
    {
        u32XCWindow = 0;
    }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    else if(*peAVPATH == PVR_PATH_SUB)
    {
        MS_BOOL bWindowEnable = FALSE;
        MS_U32 u32X = 0;
        MS_U32 u32Y = 0;
        MS_U32 u32Width = 0;
        MS_U32 u32Height = 0;
        u32XCWindow = 1;

        Demo_XC_EnableWindow(&u32XCDevice, &u32XCWindow, (MS_U32*)&bWindowEnable,
                             &u32X, &u32Y, &u32Width, &u32Height);
        printf("[%s][%d]Demo_XC_EnableWindow:%"DTC_MS_U32_d" %"DTC_MS_U32_d" %d\n",__FUNCTION__, __LINE__,u32XCDevice,u32XCWindow,FALSE);
    }
#endif
    else
    {
        printf("[%s][%d]Error AVPATH(%d)!\n", __FUNCTION__, __LINE__, *peAVPATH);
        return;
    }

    EN_AV_Device eDevice = _PVRPlaybackPath_Mapping_AV_Device(*peAVPATH);
    Demo_Zapping_GetZappingType(&eDevice,&eZappingType);
#if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
    if (((eZappingType == E_ZAPPING_XC_SEAMLESS) && (Demo_XC_SeamlessZapping_IsFreeze(&u32XCDevice, &u32XCWindow) == FALSE)) ||
        (eZappingType == E_ZAPPING_NORMAL))
#endif
    {
        Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute HDMI Main/Sub Window
        printf("[%s][%d]Demo_XC_SetVideoMute:%"DTC_MS_U32_d" %"DTC_MS_U32_d" %"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u32XCDevice,u32XCWindow,u32DisplayMute);

        if(*peAVPATH == PVR_PATH_MAIN)
        {
            u32XCDevice = 1;
            Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute); // Mute CVBS Main Window
            printf("[%s][%d]Demo_XC_SetVideoMute:%"DTC_MS_U32_d" %"DTC_MS_U32_d" %"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u32XCDevice,u32XCWindow,u32DisplayMute);
            Demo_VE_SetVideoMute(&u32DisplayMute);                        // Mute CVBS
            printf("[%s][%d]Demo_VE_SetVideoMute:%"DTC_MS_U32_d"\n",__FUNCTION__, __LINE__,u32DisplayMute);
        }
    }
}

static void _Demo_PVR_XC_Play(void)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = E_MSAPI_XC_MAX_WINDOW;
    MS_U32 u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_DTV;
    if (_eCurPlaybackProgramPath == PVR_PATH_MAIN)
    {
        printf("Main window of HD path (SC0) => Demo_XC_PlayVideo(...,E_DDI_XC_INPUT_SOURCE_DTV)\n");
        u32XCDevice = 0; //HD PATH: SC0
        u32XCWindow = E_MSAPI_XC_MAIN_WINDOW; // Main Window
        u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_DTV;
    }
    else if(_eCurPlaybackProgramPath == PVR_PATH_SUB)
    {
        printf("Sub window of HD path (SC0) => Demo_XC_PlayVideo(...,E_DDI_XC_INPUT_SOURCE_DTV2) \n");
        u32XCDevice = 0; //HD PATH: SC0
        u32XCWindow = E_MSAPI_XC_SUB_WINDOW; // Sub Window
        u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_DTV2;
    }

    Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc);

#if (DEMO_XC_DUALXC_TEST == 1)
    u32XCDevice = 1; //SD PATH: SC1
    u32XCWindow = E_MSAPI_XC_MAIN_WINDOW; // Main Window
    #if (DEMO_XC_DUALXC_IDENTICAL_DISPLAY_TEST == 1)
    u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_SCALER0_OP;
    extern MSAPI_XC_DEVICE_ID g_stXC_DeviceId[2];
    if(msAPI_XC_IsBlackVideoEnable_EX(&g_stXC_DeviceId[u32XCDevice], (E_MSAPI_XC_WINDOW)u32XCWindow) == TRUE)
    {//Always keep SC1 main window un-mute
        printf("Main window of SD path (SC1) => Demo_XC_PlayVideo(...,E_DDI_XC_INPUT_SOURCE_SCALER0_OP)\n");
        Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc);
    }
    #else
    if (_eCurPlaybackProgramPath == PVR_PATH_MAIN)
    {//SC1 main window follows SC0 main window. The look's the same.
        u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_DTV;
        printf("Main window of SD path (SC1) => Demo_XC_PlayVideo(...,E_DDI_XC_INPUT_SOURCE_DTV)\n");
        Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc);
    }
    #endif
#endif

    //Always keep VE un-mute
    if(MDrv_VE_IsBlackScreenEnabled() == TRUE)
        Demo_VE_PlayVideo();

    DemoPvr_print("########## XC Bluescreen DISABLE!!\n");
}

//------------------------------------------------------------------------------
/// @brief Set Demux flow for PVR playback
/// @param[in] eAVPATH, PVR Playback path, Main or Sub.
/// @param[in] pClkInv TSin options: clock phase inversion
/// @param[in] pExtSync TSin options: sync by external signal
/// @param[in] pParal TSin is parallel interface or serial interface
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note For PVR Playback to use
/// Command:
//------------------------------------------------------------------------------
static MS_BOOL _Demo_PVR_FileIn_FlowSet(Ex_PVRPlaybackPath* peAVPATH, int* pClkInv, int* pExtSync, int* pParal)
{
    MS_BOOL bClkInv, bExtSync, bParallel;
    EN_DEMO_DMX_FLOW eFlow;
    if ((pClkInv == NULL) || (pExtSync == NULL) || (pParal == NULL))
    {
        return FALSE;
    }

    if (*peAVPATH == PVR_PATH_MAIN)
    {
        eFlow = E_DMX_FLOW_FILE0;
    }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    else if(*peAVPATH == PVR_PATH_SUB)
    {
        eFlow = E_DMX_FLOW_FILE1;
    }
#endif
    else
    {
        printf("[%s][%d]Error AVPATH(%d)!\n", __FUNCTION__, __LINE__, *peAVPATH);
        return FALSE;
    }

    bClkInv = (0 == *pClkInv)? FALSE: TRUE;
    bExtSync = (0 == *pExtSync)? FALSE: TRUE;
    bParallel = (0 == *pParal)? FALSE: TRUE;

    stFileAVInfo[*peAVPATH].eFlow = eFlow;

    if (TRUE != Demo_DMX_FlowSet(eFlow, E_DMX_FLOW_INPUT_MEM, bClkInv, bExtSync, bParallel))
    {
        printf("[%s][%d] MApi_DMX_FlowSet(DMX_FLOW_INPUT_MEM) fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set video and audio pid to TSP for PVR fle mode
/// @param[in] eAVPATH, PVR Playback path, Main or Sub.
/// @param[in] VideoPid Video Pid
/// @param[in] AudioPid Audio Pid
/// @param[in] pu32VCodec Video codec type. 0 is for MPEG2. 1 is for H264. 2 is for AVS. 3 is for HEVC(H.265).
/// @param[in] pu32ACodec Audio Pid
///         0: NOT VALID
///         1: MPEG
///         2: AC3
///         3: AC3P
///         4: AAC
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b TV_TSP_SetPid \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_PVR_FileIn_SetPid(Ex_PVRPlaybackPath* peAVPATH, MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    EN_DEMO_DMX_FLOW eDMX_FLOW  = E_DMX_FLOW_FILE0;
    EN_VDEC_Device eVDEC_Source = E_VDEC_DEVICE_MAIN;
    MS_U32 u32ADECDevIdx = 0;
    MS_BOOL bRet = TRUE;

    Demo_PVR_Check_AVPath(stFileAVInfo[*peAVPATH].eVideoFIFO_ID,*peAVPATH,PVR_PATH_MAIN,E_DMX_FLT_TYPE_VID0,PVR_PATH_SUB,E_DMX_FLT_TYPE_VID1);
    Demo_PVR_Check_AVPath(eVDEC_Source,*peAVPATH,PVR_PATH_MAIN,E_VDEC_DEVICE_MAIN,PVR_PATH_SUB,E_VDEC_DEVICE_SUB);
    Demo_PVR_Check_AVPath(u32ADECDevIdx,*peAVPATH,PVR_PATH_MAIN,0,PVR_PATH_SUB,1);

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        stFileAVInfo[*peAVPATH].eAudioFIFO_ID = Demo_AV_GetAudioFIFO((EN_AV_Device *)peAVPATH);
    }
    else
    {
    if (*peAVPATH == PVR_PATH_SUB)
    {
        printf("[%s][%d]Error AVPATH(%d), not support multi audio path!\n", __FUNCTION__, __LINE__, *peAVPATH);
        return FALSE;
    }
        stFileAVInfo[*peAVPATH].eAudioFIFO_ID = E_DMX_FLT_TYPE_AUD0;
    }

    stFileAVInfo[*peAVPATH].u16VideoPID = (*pu32VideoPid) & 0x1FFF;
    stFileAVInfo[*peAVPATH].u16AudioPID = (*pu32AudioPid) & 0x1FFF;
    stFileAVInfo[*peAVPATH].u16PCRPID = (*pu32PCRPid) & 0x1FFF;
    stFileAVInfo[*peAVPATH].eVideoCodec = (*pu32VCodec) & 0xFF;
    stFileAVInfo[*peAVPATH].eAudioCodec = (*pu32ACodec) & 0xFF;

    printf("[%s][%d] AVPATH[%d] VideoPid[%u] AudioPid[%u] PCRPid[%u] VideoCodec[%u] AudioCodec[%u]\n", __FUNCTION__, __LINE__,*peAVPATH, stFileAVInfo[*peAVPATH].u16VideoPID, stFileAVInfo[*peAVPATH].u16AudioPID, stFileAVInfo[*peAVPATH].u16PCRPID, stFileAVInfo[*peAVPATH].eVideoCodec, stFileAVInfo[*peAVPATH].eAudioCodec);
    if (stFileAVInfo[*peAVPATH].eFlow == E_DMX_FLOW_FILE0)
    {
        eDMX_FLOW = E_DMX_FLOW_FILE0;
    }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    else if(stFileAVInfo[*peAVPATH].eFlow == E_DMX_FLOW_FILE1)
    {
        eDMX_FLOW = E_DMX_FLOW_FILE1;
    }
#endif
    else
    {
        printf("[%s][%d] Not support dmx file-in flowset(%d) \n",__FUNCTION__,__LINE__, stFileAVInfo[*peAVPATH].eFlow);
        return FALSE;
    }

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    if(_bTimeshiftSeamless[*peAVPATH] == FALSE)
#endif
    {
        printf("[%s][%d] Enable XC Mute\n",__FUNCTION__,__LINE__);
        _Demo_PVR_XC_Mute(peAVPATH);
    }

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stFileAVInfo[*peAVPATH].eVideoFIFO_ID,TRUE))
    {
        printf("[%s][%d] Demo_DMX_AVFifo_Reset fail\n", __FUNCTION__, __LINE__);
        goto _Demo_PVR_FileIn_SetPid_fail;
    }
    if (TRUE != Demo_DMX_AVFifo_Reset(stFileAVInfo[*peAVPATH].eAudioFIFO_ID,TRUE))
    {
        printf("[%s][%d]Demo_DMX_AVFifo_Reset fail\n", __FUNCTION__, __LINE__);
        goto _Demo_PVR_FileIn_SetPid_fail;
    }

    if (stFileAVInfo[*peAVPATH].u8VideoFilter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stFileAVInfo[*peAVPATH].u8VideoFilter);
        MApi_DMX_Close(stFileAVInfo[*peAVPATH].u8VideoFilter);
        stFileAVInfo[*peAVPATH].u8VideoFilter = INVALID_FILTER_ID;

        printf("[%s][%d] MApi_DMX_Stop :%d\n", __FUNCTION__, __LINE__,stFileAVInfo[*peAVPATH].u8VideoFilter);
    }
    if (stFileAVInfo[*peAVPATH].u8AudioFilter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stFileAVInfo[*peAVPATH].u8AudioFilter);
        MApi_DMX_Close(stFileAVInfo[*peAVPATH].u8AudioFilter);
        stFileAVInfo[*peAVPATH].u8AudioFilter = INVALID_FILTER_ID;

        printf("[%s][%d] MApi_DMX_Stop :%d\n", __FUNCTION__, __LINE__,stFileAVInfo[*peAVPATH].u8AudioFilter);
    }
    if (stFileAVInfo[*peAVPATH].ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stFileAVInfo[*peAVPATH].ePCREngID);
        Demo_DMX_PCR_FltClose(stFileAVInfo[*peAVPATH].ePCREngID);
        stFileAVInfo[*peAVPATH].ePCREngID = E_PCR_ENG_INVALID;

        printf("[%s][%d] Demo_DMX_PCR_FltStop :%d\n", __FUNCTION__, __LINE__,stFileAVInfo[*peAVPATH].ePCREngID);
    }

    ///[[ For Timeshift Seamless
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    if(_bTimeshiftSeamless[*peAVPATH] == FALSE)
#endif
    {
        printf("[%s][%d] Stop AVdec\n",__FUNCTION__,__LINE__);

        void *pVideoStreamId = _Demo_PVR_GetVidStreamID(peAVPATH);
        if (pVideoStreamId == NULL)
        {
            printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__);
            return FALSE;
        }
        stFileAVInfo[*peAVPATH].stVDECSteamID = *((VDEC_StreamId*)pVideoStreamId);
        Demo_VDEC_Stop(&eVDEC_Source,&(stFileAVInfo[*peAVPATH].stVDECSteamID));
        Demo_Audio_Stop(u32ADECDevIdx, AUDIO_APP_DTV);
    }

    DemoPvr_print("[%s][%d]DmxFlow: %8u VideoFltType: %8u VideoPid: %8u AudioFltType: %8u AudioPid: %8u PCRPid: %8u\n",__FUNCTION__, __LINE__, eDMX_FLOW, stFileAVInfo[*peAVPATH].eVideoFIFO_ID, stFileAVInfo[*peAVPATH].u16VideoPID, stFileAVInfo[*peAVPATH].eAudioFIFO_ID,stFileAVInfo[*peAVPATH].u16AudioPID, stFileAVInfo[*peAVPATH].u16PCRPID);
    //DMX Flt Open
    bRet  = Demo_DMX_FltOpen( eDMX_FLOW, stFileAVInfo[*peAVPATH].eVideoFIFO_ID, &stFileAVInfo[*peAVPATH].u8VideoFilter, stFileAVInfo[*peAVPATH].u16VideoPID);
    bRet &= Demo_DMX_FltOpen( eDMX_FLOW, stFileAVInfo[*peAVPATH].eAudioFIFO_ID, &stFileAVInfo[*peAVPATH].u8AudioFilter, stFileAVInfo[*peAVPATH].u16AudioPID);
    bRet &= Demo_DMX_PCR_FltOpen(eDMX_FLOW, stFileAVInfo[*peAVPATH].u16PCRPID, &stFileAVInfo[*peAVPATH].ePCREngID);

    if(!bRet)
    {
        printf("[%s][%d]Demo_DMX_Flt Open fail \n", __FUNCTION__, __LINE__);
        goto _Demo_PVR_FileIn_SetPid_fail;
    }

    //DMX Flt Start
    bRet  = (DMX_FILTER_STATUS_OK == MApi_DMX_Start(stFileAVInfo[*peAVPATH].u8VideoFilter));
    bRet &= (DMX_FILTER_STATUS_OK == MApi_DMX_Start(stFileAVInfo[*peAVPATH].u8AudioFilter));
    bRet &= Demo_DMX_PCR_FltStart(stFileAVInfo[*peAVPATH].ePCREngID);

    if(!bRet)
    {
        printf("[%s][%d]Demo_DMX_Flt Start fail \n", __FUNCTION__, __LINE__);
        goto _Demo_PVR_FileIn_SetPid_fail;
    }

    printf("[%s][%d] Device(%d) filters are ready\n", __FUNCTION__, __LINE__,*peAVPATH);
    return TRUE;

_Demo_PVR_FileIn_SetPid_fail:
    printf("[%s][%d] fail\n", __FUNCTION__, __LINE__);
    if (stFileAVInfo[*peAVPATH].u8VideoFilter!= INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stFileAVInfo[*peAVPATH].u8VideoFilter);
        MApi_DMX_Close(stFileAVInfo[*peAVPATH].u8VideoFilter);
        stFileAVInfo[*peAVPATH].u8VideoFilter = INVALID_FILTER_ID;
    }
    if (stFileAVInfo[*peAVPATH].u8AudioFilter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stFileAVInfo[*peAVPATH].u8AudioFilter);
        MApi_DMX_Close(stFileAVInfo[*peAVPATH].u8AudioFilter);
        stFileAVInfo[*peAVPATH].u8AudioFilter = INVALID_FILTER_ID;
    }

    if (stFileAVInfo[*peAVPATH].ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stFileAVInfo[*peAVPATH].ePCREngID);
        Demo_DMX_PCR_FltClose(stFileAVInfo[*peAVPATH].ePCREngID);
        stFileAVInfo[*peAVPATH].ePCREngID = E_PCR_ENG_INVALID;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief check the AV STC will change or not
/// @param[in] eAVPATH, PVR Playback path, Main or Sub.
/// @return the value is the PCR Engine ID.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_PVR_FileIn_Set_STC(Ex_PVRPlaybackPath peAVPATH,AUDIO_STC_SOURCE *eSetSTC)
{
    DEMO_PVR_CHECK_PB(peAVPATH, FALSE);

    if (stFileAVInfo[peAVPATH].ePCREngID == E_PCR_ENG0)
    {
        printf("[%s][%d]========= eAVPATH=%d, ePCREngID = E_TSP_0\n", __FUNCTION__, __LINE__, peAVPATH);
        *eSetSTC = E_TSP_0;
    }
    else if (stFileAVInfo[peAVPATH].ePCREngID == E_PCR_ENG1)
    {
        printf("[%s][%d]========= eAVPATH=%d, ePCREngID = E_TSP_1\n", __FUNCTION__, __LINE__, peAVPATH);
        *eSetSTC = E_TSP_1;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Play decoder
/// @param[in] peAVPATH, PVR Playback path, Main or Sub.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
static MS_BOOL _Demo_PVR_FileIn_PlayAV(Ex_PVRPlaybackPath* peAVPATH)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    AUDIO_STC_SOURCE eSetSTC = E_TSP_0;
    EN_VDEC_Device eVDECDeviceIdx = E_VDEC_DEVICE_MAIN;
    MS_U32 u32ADECDevIdx = 0;

    printf("========[%s]========\n",__FUNCTION__);
    printf("[%s][%d] eAVPATH : %d ,VDEC Stream ID Before Init 0x%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,*peAVPATH,stFileAVInfo[*peAVPATH].stVDECSteamID.u32Id);

    Demo_PVR_Check_AVPath(eVDECDeviceIdx,*peAVPATH,PVR_PATH_MAIN,E_VDEC_DEVICE_MAIN,PVR_PATH_SUB,E_VDEC_DEVICE_SUB);
    Demo_PVR_Check_AVPath(u32ADECDevIdx,*peAVPATH,PVR_PATH_MAIN,0,PVR_PATH_SUB,1);

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    ST_AV_INFO stAVInfo;
    if(_bTimeshiftSeamless[*peAVPATH] == TRUE)
    {
        if (Demo_AV_GetAVInfo((EN_AV_Device *)&eVDECDeviceIdx,E_AV_GetCmd_LiveInfo,&stAVInfo)  == FALSE)
        {
            printf("[%s][%d] Error!!!, Get Live info by Demo_AV_GetAVInfo failed\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
#endif

    if (TRUE != _Demo_PVR_FileIn_Set_STC(*peAVPATH, &eSetSTC))
    {
        printf("[%s][%d]_Demo_PVR_FileIn_Set_STC fail\n",__FUNCTION__, __LINE__);
        return FALSE;
    }
    printf("[%s][%d] Demo_AV_Set_STC : %d  \n",__FUNCTION__,__LINE__,eSetSTC);


#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    if(_bTimeshiftSeamless[*peAVPATH] == TRUE)
    {
        printf("[%s][%d] Diable XC Mute/MVOP and Restore Live Info.\n",__FUNCTION__,__LINE__);
        stFileAVInfo[*peAVPATH].stVDECSteamID = stAVInfo.stVideoParams.stVDECSteamID;
        stFileAVInfo[*peAVPATH].bInited = stAVInfo.bInited;
        DemoPvr_print("[%s][%d] eAVPATH : %d ,VDEC Stream ID Before Init 0x%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,*peAVPATH,stFileAVInfo[*peAVPATH].stVDECSteamID.u32Id);
    }
    else
#endif
    {
        _Demo_PVR_XC_Mute(peAVPATH);

        if (TRUE != Demo_VDEC_Init(&eVDECDeviceIdx,&(stFileAVInfo[*peAVPATH].stVDECSteamID),stFileAVInfo[*peAVPATH].eVideoCodec,E_VDEC_DDI_SRC_MODE_TS_FILE,eSetSTC)) // default mepg2
        {
            printf("[%s][%d]Demo_VDEC_Init fail\n",__FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            stFileAVInfo[*peAVPATH].bInited = TRUE;
            printf("[%s][%d] eAVPATH : %d ,Stream ID After Init  0x%"DTC_MS_U32_x"\n",__FUNCTION__, __LINE__,*peAVPATH,stFileAVInfo[*peAVPATH].stVDECSteamID.u32Id);
        }

        if (TRUE != Demo_VDEC_Play(&eVDECDeviceIdx,&(stFileAVInfo[*peAVPATH].stVDECSteamID),E_VDEC_SYNC_PCR_MODE))
        {
            printf("[%s][%d] Demo_VDEC_Play fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] Demo_VDEC_Play OK\n", __FUNCTION__, __LINE__);
        }
    }

    if (TRUE != Demo_DMX_AVFifo_Reset(stFileAVInfo[*peAVPATH].eVideoFIFO_ID,FALSE))
    {
        printf("[%s][%d]Demo DMX VideoFIFO reset fail\n", __FUNCTION__, __LINE__);
    }

    if(stFileAVInfo[*peAVPATH].u16AudioPID != INVALID_PID_ID)
    {
        if (Demo_Audio_Play(u32ADECDevIdx,AUDIO_APP_DTV,stFileAVInfo[*peAVPATH].eAudioCodec,&eSetSTC) == FALSE) // default mepg2
        {
            printf("[%s][%d] Demo_Audio_Play fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            printf("[%s][%d] Demo_Audio_Play ok\n", __FUNCTION__, __LINE__);
        }
        if (TRUE != Demo_DMX_AVFifo_Reset(stFileAVInfo[*peAVPATH].eAudioFIFO_ID,FALSE))
        {
            printf("[%s][%d]Demo DMX AudioFIFO reset fail\n", __FUNCTION__, __LINE__);
        }
        Demo_Audio_SetAbsoluteVolume(&u8Volume);
    }
    else
    {
        printf("[%s][%d] Invalid Audio PID\n", __FUNCTION__, __LINE__);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Stop decoder
/// @param[in] peAVPATH, PVR Playback path, Main or Sub.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_PVR_FileIn_StopAV(Ex_PVRPlaybackPath* peAVPATH)
{
    EN_VDEC_Device eVDECDeviceIdx = E_VDEC_DEVICE_MAIN;
    MS_U32 u32ADECDevIdx = 0;

    printf("=====[%s]=====\n",__FUNCTION__);

    Demo_PVR_Check_AVPath(eVDECDeviceIdx,*peAVPATH,PVR_PATH_MAIN,E_VDEC_DEVICE_MAIN,PVR_PATH_SUB,E_VDEC_DEVICE_SUB);
    Demo_PVR_Check_AVPath(u32ADECDevIdx,*peAVPATH,PVR_PATH_MAIN,0,PVR_PATH_SUB,1);

    _Demo_PVR_XC_Mute(peAVPATH);

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(stFileAVInfo[*peAVPATH].eVideoFIFO_ID,TRUE))
    {
        printf("[%s][%d]Demo DMX VideoFIFO reset fail\n", __FUNCTION__, __LINE__);
    }
    if (TRUE != Demo_DMX_AVFifo_Reset(stFileAVInfo[*peAVPATH].eAudioFIFO_ID,TRUE))
    {
        printf("[%s][%d]Demo DMX AudioFIFO reset fail\n", __FUNCTION__, __LINE__);
    }

    if (stFileAVInfo[*peAVPATH].u8VideoFilter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stFileAVInfo[*peAVPATH].u8VideoFilter);
        MApi_DMX_Close(stFileAVInfo[*peAVPATH].u8VideoFilter);
        stFileAVInfo[*peAVPATH].u8VideoFilter = INVALID_FILTER_ID;
    }
    if (stFileAVInfo[*peAVPATH].u8AudioFilter != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(stFileAVInfo[*peAVPATH].u8AudioFilter);
        MApi_DMX_Close(stFileAVInfo[*peAVPATH].u8AudioFilter);
        stFileAVInfo[*peAVPATH].u8AudioFilter = INVALID_FILTER_ID;
    }
    if (stFileAVInfo[*peAVPATH].ePCREngID != E_PCR_ENG_INVALID)
    {
        Demo_DMX_PCR_FltStop(stFileAVInfo[*peAVPATH].ePCREngID);
        Demo_DMX_PCR_FltClose(stFileAVInfo[*peAVPATH].ePCREngID);
        stFileAVInfo[*peAVPATH].ePCREngID = E_PCR_ENG_INVALID;
    }

    Demo_VDEC_Stop(&eVDECDeviceIdx,&(stFileAVInfo[*peAVPATH].stVDECSteamID));
    Demo_Audio_Stop(u32ADECDevIdx, AUDIO_APP_DTV);
    stFileAVInfo[*peAVPATH].bInited = FALSE;

    return TRUE;
}

static MS_BOOL _Demo_PVR_LIVE_PlayAV(EN_AV_Device* peDevice)
{
    if(b2ndEncryptionEnabled)
    {
        MS_U32 u32DeviceID;
        Demo_PVR_Check_AVPath(u32DeviceID,*peDevice,E_AV_DEVICE_MAIN,PVR_PATH_MAIN,E_AV_DEVICE_SUB,PVR_PATH_SUB);

        //LIVE DSCMB Connect
        if (_Demo_PVR_DSCMB_Live_Connect(&u32DeviceID, &stPVRLiveDSCMBInfo[*peDevice]) == TRUE)
        {
            DemoPvr_print("LIVE DSCMB Connect Success=> Device:(%"DTC_MS_U32_u") Type:(%s)  CW: Odd(%s) Even(%s)\n",u32DeviceID, stPVRLiveDSCMBInfo[u32DeviceID].u8Type,stPVRLiveDSCMBInfo[u32DeviceID].u8StrOddCW,stPVRLiveDSCMBInfo[u32DeviceID].u8StrEvenCW);
        }
    }

    #if(DEMO_PVR_VSYNC_MODULE_ENABLE == 1)
    // Just like DTV_PlayAV 0x20 (for the 1st path)
    *peDevice &= E_VDEC_EX_N_STREAM << 4;
    #endif

    return Demo_AV_PlayAV(peDevice);
}

static MS_BOOL _Demo_PVR_LIVE_StopAV(EN_AV_Device *peDevice)
{
    MS_BOOL bRet;
    if(b2ndEncryptionEnabled)
    {
        MS_U32 u32DeviceID;

        Demo_PVR_Check_AVPath(u32DeviceID,*peDevice,E_AV_DEVICE_MAIN,PVR_PATH_MAIN,E_AV_DEVICE_SUB,PVR_PATH_SUB);

        //SAVE LIVE DSCMB INFO and stop LIVE DSCMB
        if (_Demo_PVR_DSCMB_GetLiveInfo(u32DeviceID, &stPVRLiveDSCMBInfo[u32DeviceID]) == TRUE)
        {
            stPVRLiveDSCMBInfo[u32DeviceID].bConnectStatus = TRUE;
            if (_Demo_PVR_DSCMB_Live_Disconnect(&u32DeviceID, &stPVRLiveDSCMBInfo[u32DeviceID]) == TRUE)
            {
                DemoPvr_print("[%s][%d]Stop LIVE DSCMB Success=> Device:(%d) Type:(%s)  CW: Odd(%s) Even(%s)\n",__FUNCTION__,__LINE__,u32DeviceID, stPVRLiveDSCMBInfo[u32DeviceID].u8Type,stPVRLiveDSCMBInfo[u32DeviceID].u8StrOddCW,stPVRLiveDSCMBInfo[u32DeviceID].u8StrEvenCW);
            }
        }
    }

    bRet = Demo_AV_StopAV(peDevice);

    return bRet;
}

static MS_BOOL _Demo_PVR_LIVE_PlayRadio(EN_AV_Device* peDevice)
{
    if(b2ndEncryptionEnabled)
    {
        MS_U32 u32DeviceID;

        Demo_PVR_Check_AVPath(u32DeviceID,*peDevice,E_AV_DEVICE_MAIN,PVR_PATH_MAIN,E_AV_DEVICE_SUB,PVR_PATH_SUB);

        //LIVE DSCMB Connect
        if (_Demo_PVR_DSCMB_Live_Connect(&u32DeviceID, &stPVRLiveDSCMBInfo[*peDevice]) == TRUE)
        {
            DemoPvr_print("LIVE DSCMB Connect Success=> Device:(%"DTC_MS_U32_u") Type:(%s)  CW: Odd(%s) Even(%s)\n",u32DeviceID, stPVRLiveDSCMBInfo[u32DeviceID].u8Type,stPVRLiveDSCMBInfo[u32DeviceID].u8StrOddCW,stPVRLiveDSCMBInfo[u32DeviceID].u8StrEvenCW);
        }
    }

    return Demo_AV_PlayRadio(peDevice);
}
static MS_BOOL _Demo_PVR_LIVE_StopRadio(EN_AV_Device *peDevice)
{
    MS_BOOL bRet;
    if(b2ndEncryptionEnabled)
    {
        MS_U32 u32DeviceID;
        Demo_PVR_Check_AVPath(u32DeviceID,*peDevice,E_AV_DEVICE_MAIN,PVR_PATH_MAIN,E_AV_DEVICE_SUB,PVR_PATH_SUB);

        //SAVE LIVE DSCMB INFO and stop LIVE DSCMB
        if (_Demo_PVR_DSCMB_GetLiveInfo(u32DeviceID, &stPVRLiveDSCMBInfo[u32DeviceID]) == TRUE)
        {
            stPVRLiveDSCMBInfo[u32DeviceID].bConnectStatus = TRUE;
            if (_Demo_PVR_DSCMB_Live_Disconnect(&u32DeviceID, &stPVRLiveDSCMBInfo[u32DeviceID]) == TRUE)
            {
                DemoPvr_print("[%s][%d]Stop LIVE DSCMB Success=> Device:(%d) Type:(%s)  CW: Odd(%s) Even(%s)\n",__FUNCTION__,__LINE__,u32DeviceID, stPVRLiveDSCMBInfo[u32DeviceID].u8Type,stPVRLiveDSCMBInfo[u32DeviceID].u8StrOddCW,stPVRLiveDSCMBInfo[u32DeviceID].u8StrEvenCW);
            }
        }
    }

    bRet = Demo_AV_StopRadio(peDevice);

    return bRet;
}

static MS_BOOL _Demo_PVR_AV_Start(MS_BOOL bIsLiveStream, PVRProgramInfo_t *programInfo)
{
    MS_BOOL bRet=TRUE;
    MS_U32 u32VideoPid=_Demo_PVR_FindPidinFilter(programInfo->Filters, DMX_FILTER_TYPE_VIDEO);
    MS_U32 u32AudioPid=programInfo->u16AudioPid;
    MS_U32 u32PCRPid=_Demo_PVR_FindPidinFilter(programInfo->Filters, DMX_FILTER_TYPE_PCR);
    MS_U32 u32VCodec=0;
    MS_U32 u32ACodec=0;
    int clkInv = 0;
    int extSync = 1;
    int parallel = 1;
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;

    DemoPvr_print("[%s][%d]  bIsLiveStream=%d\n",__FUNCTION__,__LINE__,bIsLiveStream);
    DemoPvr_print("[%s][%d] u32VideoPid=%"DTC_MS_U32_u" u32AudioPid=%"DTC_MS_U32_u" u32PCRPid=%"DTC_MS_U32_u" \n", __FUNCTION__, __LINE__,u32VideoPid,u32AudioPid,u32PCRPid);
    Demo_PVR_Check_AVPath(eDevice,_eCurPlaybackProgramPath,PVR_PATH_MAIN,E_AV_DEVICE_MAIN,PVR_PATH_SUB,E_AV_DEVICE_SUB);

    u32VCodec = _Demo_PVR_Get_VDECDDI_VDEC_Fmt_FromPromInfo(programInfo->enVCodec);
    u32ACodec = _Demo_PVR_Get_ADECDDI_ADEC_Fmt_FromPromInfo((APIPVR_AUD_CODEC_TYPE)(programInfo->u32ACodec));

    do{
        if(bIsLiveStream)
        {
            if (FALSE == (bRet = _DualAVFunc.DualAV_Tuner_Config(&eDevice, &_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, &_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eDmxInput, &_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.DmxClkInv, &_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.DmxExtSync, &_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.DmxParallal)))
            {
                break;
            }

            #if(DEMO_PVR_VSYNC_MODULE_ENABLE == 1)
            // Always use PureMCU mode in PVR
            EN_AV_ShowFrame_Mode eMode = E_AV_PureMCU_MODE;
            MSAPI_XC_WINDOW_TYPE stDispWinInfo = {0};

            stDispWinInfo.x = _stLiveInfo[_eCurPlaybackProgramPath].stShowFrameInfo.u16X;
            stDispWinInfo.y = _stLiveInfo[_eCurPlaybackProgramPath].stShowFrameInfo.u16Y;
            stDispWinInfo.width = _stLiveInfo[_eCurPlaybackProgramPath].stShowFrameInfo.u16Width;
            stDispWinInfo.height = _stLiveInfo[_eCurPlaybackProgramPath].stShowFrameInfo.u16Height;

            printf("[%s][%d] using window size at path %u: width = %"DTC_MS_U32_u", height = %"DTC_MS_U32_u"\n",__FUNCTION__, __LINE__, eDevice, (MS_U32)stDispWinInfo.width, (MS_U32)stDispWinInfo.height);
            Demo_AV_SetShowFrameMode(&eDevice, (EN_AV_ShowFrame_Mode *)&eMode, (MS_U32 *)&eDevice, (MS_U16 *)&stDispWinInfo.x, (MS_U16 *)&stDispWinInfo.y, (MS_U16 *)&stDispWinInfo.width, (MS_U16 *)&stDispWinInfo.height, (MS_U16 *)&eDevice);
            #endif

            // For merge stream, when playback stop and return to live, modify source id to the original source id of live before playback start,
            // so that we can return to live successfully according to the stored program info before playback start.
            EN_DEMO_DMX_FLT_SOURCEID eDTVSrcID = _stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eDmxFltSourceID;
            EN_DEMO_DMX_FLT_SOURCEID eSrcID = Demo_DMX_GetSrcID(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow);
            Demo_DMX_SetSrcID(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, eDTVSrcID);

            bRet = _DualAVFunc.DualAV_TSP_SetPid(&eDevice, &u32VideoPid, &u32AudioPid, &u32PCRPid, &u32VCodec, &u32ACodec);

            // For merge stream, after return to live, recover source id
            Demo_DMX_SetSrcID(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, eSrcID);
        }
        else
        {
            if (FALSE == (bRet = _DualFileFunc.FileIn_FlowSet(&_eCurPlaybackProgramPath, &clkInv, &extSync, &parallel)))
            {
                break;
            }
            bRet = _DualFileFunc.FileIn_SetPid(&_eCurPlaybackProgramPath,&u32VideoPid, &u32AudioPid, &u32PCRPid, &u32VCodec, &u32ACodec);
        }
    }while(0);

    if (!bRet)
    {
        printf("[%s][%d] play av failed\n",__FUNCTION__,__LINE__);
        return bRet;
    }

    if (_eCurPlaybackProgramPath == PVR_PATH_MAIN)
    {
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_FF2X, 0, 0);
    }
    else if(_eCurPlaybackProgramPath == PVR_PATH_SUB)
    {
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC2_FF2X, 0, 0);//audio sub-decode not ready
    }

    printf("[%s][%d]  try to play av\n",__FUNCTION__,__LINE__);
    if(bIsLiveStream)
    {
        if(u32VideoPid == INVALID_PID_ID)
        {
            bRet = _DualAVFunc.DualAV_PlayRadio(&eDevice);
        }
        else
        {
            bRet = _DualAVFunc.DualAV_PlayAV(&eDevice);
        }
    }
    else
    {
        bRet = _DualFileFunc.FileIn_PlayAV(&_eCurPlaybackProgramPath);
    }

    if (!bRet)
    {
        if(bIsLiveStream)
        {
            if(u32VideoPid == INVALID_PID_ID)
            {
                _DualAVFunc.DualAV_StopRadio(&eDevice);
            }
            else
            {
                _DualAVFunc.DualAV_StopAV(&eDevice);
            }
        }
        else
        {
            _DualFileFunc.FileIn_StopAV(&_eCurPlaybackProgramPath);
        }
        printf("[%s][%d] play av failed\n",__FUNCTION__,__LINE__);
        return bRet;
    }

    return bRet;
}

static MS_BOOL _Demo_PVR_AV_Play(void)
{
    MS_U16 u16FrameTHreshold= 2;
    MS_U32 u32Loop=400;
    void *PVRVDECStreamID;
    EN_VDEC_Device eVDECDeviceIdx = E_VDEC_DEVICE_MAIN;

    Demo_PVR_Check_AVPath(eVDECDeviceIdx,_eCurPlaybackProgramPath,PVR_PATH_MAIN,E_VDEC_DEVICE_MAIN,PVR_PATH_SUB,E_VDEC_DEVICE_SUB);
    PVRVDECStreamID = _Demo_PVR_GetVidStreamID(&_eCurPlaybackProgramPath);
    if (PVRVDECStreamID == NULL)
    {
        printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__);
        return FALSE;
    }
    while(MApi_VDEC_EX_GetFrameCnt((VDEC_StreamId*)PVRVDECStreamID)<u16FrameTHreshold)
    {
        u32Loop--;
        if(u32Loop<=0)
        {
           printf("TimeOut MApi_VDEC_EX_GetFrameCnt=0x%"DTC_MS_U32_x"\n",MApi_VDEC_EX_GetFrameCnt((VDEC_StreamId*)PVRVDECStreamID));
           break;
        }

        MsOS_DelayTask(10);
    }

    //Check Video Decoder Status
    if( TRUE != Demo_VDEC_CheckDecoderStatus(&eVDECDeviceIdx))
    {
        printf("[%s][%d]Video Decoder[%d] Status is Not Ready!!\n Please Check Out the Code Flow!!\n", __FUNCTION__, __LINE__, _eCurPlaybackProgramPath);
        return FALSE;
    }
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    if(_bTimeshiftSeamless[_eCurPlaybackProgramPath] == FALSE)
#endif
    {
        printf("\n\n[%s][%d]Do XC/VE flow\n",__FUNCTION__,__LINE__);
        Demo_VDEC_IsAVSyncDone(&eVDECDeviceIdx);
        _Demo_PVR_XC_Play();
    }

    return TRUE;
}
static void _Demo_PVR_StopEvent(MS_BOOL bRecord,MS_U8 u8PVRIdx)
{
    DemoPvr_print("[%s][%d]bRecord=%d,pvrRecIdx=%d,_u8TimeshiftRecordIdx=%d\n",__FUNCTION__,__LINE__,bRecord,u8PVRIdx,_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);
    Ex_PVRPlaybackPath ePlaybackidx = PVR_PATH_INVALID;
    Ex_PVRPlaybackPath ePlayIdxSaved = PVR_PATH_INVALID;
    MS_U8 u8RecIdxSaved = 0xFF;


    if(bRecord)
    {
        DEMO_PVR_CHECK_REC_IDX(u8PVRIdx,);

        if(!MApi_PVR_IsRecording(_u8hRecord[u8PVRIdx]))
        {
            return ;
        }

        ePlaybackidx = (Ex_PVRPlaybackPath)_Demo_PVR_Timeshift_GetPlaybackIdx(u8PVRIdx);
        if (ePlaybackidx == PVR_PATH_INVALID)
        {
            u8RecIdxSaved = _u8CurRecProgramIdx;
            _u8CurRecProgramIdx = u8PVRIdx;
            Demo_PVR_Record_Stop();
            _u8CurRecProgramIdx = u8RecIdxSaved;
            printf("\nRecord Stop\n\n>>");
        }
        else
        {
            ePlayIdxSaved = _eCurPlaybackProgramPath;
            _eCurPlaybackProgramPath = ePlaybackidx;
            Demo_PVR_Timeshift_RecordStop();
            _eCurPlaybackProgramPath = ePlayIdxSaved;
            printf("\nTimeshift Record Stop\n\n>>");
        }
    }
    else
    {
        DEMO_PVR_CHECK_PB(u8PVRIdx,);

        if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[u8PVRIdx]))
        {
            printf("[%s][%d] not playing\n",__FUNCTION__,__LINE__);
            return ;
        }

        ePlayIdxSaved = _eCurPlaybackProgramPath;
        _eCurPlaybackProgramPath = (Ex_PVRPlaybackPath)u8PVRIdx;

        if(_u8TimeshiftPlaybackIdx[u8PVRIdx] != PVR_PATH_INVALID)
        {
            Demo_PVR_Timeshift_PlaybackStop();
            printf("\nTimeshift Playback Stop\n\n>>");

        }
        else
        {
            // FileIn_StopAV will do:
            // (1) xc mute: done by Video_Module::Exit
            // (2) ve mute: no need to do, always keep ve unmute
            // (3) reset av fifo: done by FileIn_Module::Exit, Video_Module::Exit, Audio_Module::Exit
            // (4) close all pid filters: done by PVR_Playback_Handler::Exit
            // (5) vdec exit: done by Video_Module::Exit
            // (6) audio stop decode: done by Audio_Module::Exit
            //
            // No need to call FileIn_StopAV here.
            Demo_PVR_PlaybackStop();
            printf("\nPlayback Stop\n\n>>");
        }
        _eCurPlaybackProgramPath = ePlayIdxSaved;
        _u32PlaybackTime[u8PVRIdx] = 0;
   }
}

//------------------------------------------------------------------------------
/// @brief The sample code to implement PMT change flow here.
/// the flow should contain at least following step.
/// a. DMX / AV / XC Initialization
/// b. PVR resume playback by MApi_PVR_PlaybackProgramChangeStart()
/// c. audio mute/demute control by previous speed state
/// d. XC/VE initial for start playback
//------------------------------------------------------------------------------
static void _Demo_PVR_ChangePMTEvent(Ex_PVRPlaybackPath eAVPATH, MS_BOOL bAutoMode)
{
    PVRDataInfo_t dummy = {0, 0, 0, 0};

    DemoPvr_print("[%s][%d]\n",__FUNCTION__,__LINE__);
    DEMO_PVR_CHECK_PB(eAVPATH,);

    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[eAVPATH]))
    {
        DemoPvr_print("[%s][%d] not playing\n",__FUNCTION__,__LINE__);
        return;
    }

    if(bAutoMode)
    {
        DemoPvr_print("[%s][%d]: Suspend !!!\n", __FUNCTION__, __LINE__);
        if(MApi_PVR_PlaybackProgramChangeOpen(_u8hPlayback[eAVPATH]) == FALSE)
        {
            DemoPvr_print("MApi_PVR_PlaybackProgramChangeOpen Failed!\n");
            return;
        }
    }
    else
    {
        MS_BOOL bRet = FALSE;
        Ex_PVRPlaybackPath ePlayIdxSaved = _eCurPlaybackProgramPath;

        _eCurPlaybackProgramPath = eAVPATH;

        bRet = _Demo_PVR_AV_Start(FALSE,&_plyprogramInfo[_eCurPlaybackProgramPath]);
        if(!bRet)
        {
            DemoPvr_print("Fail to Change Program. Swith to Live mode.\n");
            if(FALSE == _Demo_PVR_AV_Start(TRUE,&_livePromInfo[_eCurPlaybackProgramPath]))
            {
                DemoPvr_print("Switch to Live failed!\n");
            }
        }
        else
        {
            MApi_PVR_PlaybackProgramChangeStart(_u8hPlayback[eAVPATH], dummy);
            DemoPvr_print("Change Program success.\n");
        }
        _Demo_PVR_AV_Play();
        _eCurPlaybackProgramPath = ePlayIdxSaved;
    }
}

static void _Demo_PVR_Callback(PVREventInfo_t *pstEventInfo)
{
    MS_U64 u64FreeSpaceInKB, u64TotalSpaceInKB;

    MS_U32 u32LastPlaybackTime[MAX_PLAYBACK_NUM] = {0};
    MS_U32 u32RecordTime[PVR_MAX_RECORDING_FILE] ={0};
    MS_U8  u8PVRHandle = 0;
    MS_U8  u8PVRIdx = 0;
    MS_U32 u32RecordStartTime = 0;
    MS_U32 u32RecordDurationTime = 0;
    switch(pstEventInfo->pvrEvent)
    {
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        case EN_APIPVR_PATH_EVENT_NOTIFY_TIMESHIFT_SEAMLEASS_TIMEOUT:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            DemoPvrEvent_print("EN_APIPVR_PATH_EVENT_NOTIFY_TIMESHIFT_SEAMLEASS_TIMEOUT,_u8TimeshiftRecordIdx=%d u8PVRIdx=%d u8PVRHandle=%d\n",_u8TimeshiftRecordIdx[u8PVRIdx],u8PVRIdx,u8PVRHandle);

            DEMO_PVR_CHECK_PB(u8PVRIdx,);

            printf("[_Demo_PVR_Callback]NOTIFY_TIMESHIFT_SEAMLEASS_TIMEOUT curidx = %d\n",u8PVRHandle);
            break;
#endif
        case EN_APIPVR_EVENT_NOTIFY_FILE_END:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            DemoPvrEvent_print("EN_APIPVR_EVENT_NOTIFY_FILE_END,_u8TimeshiftRecordIdx=%d u8PVRIdx=%d u8PVRHandle=%d\n",_u8TimeshiftRecordIdx[u8PVRIdx],u8PVRIdx,u8PVRHandle);

            DEMO_PVR_CHECK_PB(u8PVRIdx,);

            if((MApi_PVR_EX_IsPlaybacking(u8PVRHandle))&&(MApi_PVR_PlaybackGetSpeed(u8PVRHandle)<=EN_APIPVR_PLAYBACK_SPEED_1XFB) )
            {
                printf("NOTIFY_FILE_END curidx = %d when speed is EN_APIPVR_PLAYBACK_SPEED_FB\n",_u8hPlayback[u8PVRIdx]);
                // When we hit file end in playback fastbackward case, we will not stop or start anymore.
                // Instead, we just let users to decide the next state by themselves.
            }
            else
            {
                _Demo_PVR_StopEvent(FALSE,u8PVRIdx);
            }
            break;
        case EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE:
            DemoPvrEvent_print("EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE\n");
            for(u8PVRIdx = 0;u8PVRIdx < PVR_MAX_RECORDING_FILE;u8PVRIdx++)
            {
                _Demo_PVR_StopEvent(TRUE,u8PVRIdx);
            }
            for(u8PVRIdx = 0;u8PVRIdx < MAX_PLAYBACK_NUM;u8PVRIdx++)
            {
                _Demo_PVR_StopEvent(FALSE,u8PVRIdx);
            }
            if(_bUplugMsg)
            {
                if(MsFS_Umount(_moutPath)==0)
                {
                    printf("Umount \"%s\" successfully!\n",_moutPath);
                }
                else
                {
                    printf("Umount \"%s\" failed!\n",_moutPath);
                }


                _bUplugMsg=FALSE;
            }
            break;
        case EN_APIPVR_EVENT_ERROR_READ_FILE_FAIL:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            DemoPvrEvent_print("[%s][%d]EN_APIPVR_EVENT_ERROR_READ_FILE_FAIL,%"DTC_MS_U32_u"\n",__FUNCTION__,__LINE__,pstEventInfo->u32Data[0]);

            DEMO_PVR_CHECK_PB(u8PVRIdx,);

            _Demo_PVR_StopEvent(FALSE,u8PVRIdx);
            char *mountPath;
            DEMO_PVR_CHECK_MountPath(mountPath, )
            if(_Demo_PVR_GetDiskSpace(mountPath,&u64FreeSpaceInKB, &u64TotalSpaceInKB))
            {
                printf("Free Disk Space:0x%"DTC_MS_U64_x" KB\n",u64FreeSpaceInKB);
            }
            break;

        case EN_APIPVR_EVENT_ERROR_INVALID_ADDR:
        case EN_APIPVR_EVENT_ERROR_WRITE_FILE_FAIL:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_RecHandler2Idx(u8PVRHandle);
            DemoPvrEvent_print("[%s][%d]EN_APIPVR_EVENT_ERROR_WRITE_FILE_FAIL,%"DTC_MS_U32_u"\n",__FUNCTION__,__LINE__,pstEventInfo->u32Data[0]);

            DEMO_PVR_CHECK_REC_IDX(u8PVRIdx,);

            _Demo_PVR_StopEvent(TRUE,u8PVRIdx);
            if(_Demo_PVR_GetDiskSpace((char *)MApi_PVR_GetMouthPath(),&u64FreeSpaceInKB, &u64TotalSpaceInKB))
            {
                printf("Free Disk Space:0x%"DTC_MS_U64_x" KB\n",u64FreeSpaceInKB);
            }
            break;

        case EN_APIPVR_EVENT_NOTIFY_RECORDED_TIME: //Second
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_RecHandler2Idx(u8PVRHandle);
            DEMO_PVR_CHECK_REC_IDX(u8PVRIdx,);

            u32RecordTime[u8PVRIdx] =pstEventInfo->u32Data[1];

            // Let record idx 0 trigger performance measure.
            if(bPVRPerformanceTest && (u8PVRIdx == 0))
            {
                _Demo_PVR_PerformanceTest(u32RecordTime[u8PVRIdx]);
            }

            if(_bShowRecordStartTime)
            {
                MApi_PVR_GetRecordStartTime(_u8hRecord[u8PVRIdx], &u32RecordStartTime);
                printf("RecordStartTime [%u][%02d:%02d:%02d] ",u8PVRIdx,(int)(u32RecordStartTime/3600),(int)((u32RecordStartTime%3600)/60),(int)((u32RecordStartTime%3600)%60));
                printf("Record Time [%u][%02d:%02d:%02d] ",u8PVRIdx,(int)(u32RecordTime[u8PVRIdx]/3600),(int)((u32RecordTime[u8PVRIdx]%3600)/60),(int)((u32RecordTime[u8PVRIdx]%3600)%60));
                u32RecordDurationTime = u32RecordTime[u8PVRIdx] - u32RecordStartTime;
                printf("Record Duration [%u][%02d:%02d:%02d]\n",u8PVRIdx,(int)(u32RecordDurationTime/3600),(int)((u32RecordDurationTime%3600)/60),(int)((u32RecordDurationTime%3600)%60));
            }
            else if(_bShowRecordTime)
            {
                printf("Record Time [%u][%02d:%02d:%02d] \n",u8PVRIdx,(int)(u32RecordTime[u8PVRIdx]/3600),(int)((u32RecordTime[u8PVRIdx]%3600)/60),(int)((u32RecordTime[u8PVRIdx]%3600)%60));
            }
            break;

        case EN_APIPVR_EVENT_NOTIFY_RECORDED_SIZE:  //KByte
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_RecHandler2Idx(u8PVRHandle);
            DEMO_PVR_CHECK_REC_IDX(u8PVRIdx,);

            DemoPvrEvent_print("[%s][%d]record idx = %d record size=%"DTC_MS_U32_u" KByte\n",__FUNCTION__,__LINE__,u8PVRIdx,pstEventInfo->u32Data[1]);
            break;

        case EN_APIPVR_EVENT_NOTIFY_RECORDED_NODATA:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_RecHandler2Idx(u8PVRHandle);
            DEMO_PVR_CHECK_REC_IDX(u8PVRIdx,);

            printf("[%s][%d]record idx = %d   record size=%"DTC_MS_U32_u" KByte, Record NOOOOOOOOOOO Data\n",__FUNCTION__,__LINE__,u8PVRIdx,pstEventInfo->u32Data[1]);
            break;

        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_TIME:
            DemoPvrEvent_print("[%s][%d]EN_APIPVR_EVENT_NOTIFY_PLAYBACK_TIME,%"DTC_MS_U32_u" sec\n",__FUNCTION__,__LINE__,pstEventInfo->u32Data[1]);
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            //printf("frame count: main:%d sub:%d",MApi_VDEC_EX_GetFrameCnt(_Demo_PVR_GetVidStreamID(0)),MApi_VDEC_EX_GetFrameCnt(_Demo_PVR_GetVidStreamID(1)));

            DEMO_PVR_CHECK_PB(u8PVRIdx,);

            u32LastPlaybackTime[u8PVRIdx] = _u32PlaybackTime[u8PVRIdx];
            _u32PlaybackTime[u8PVRIdx] = pstEventInfo->u32Data[1];


            if(_bShowPlayTime)
            {
                printf("Play Time [%u][%02d:%02d:%02d] \n",u8PVRIdx,(int)(_u32PlaybackTime[u8PVRIdx]/3600),(int)((_u32PlaybackTime[u8PVRIdx]%3600)/60),(int)((_u32PlaybackTime[u8PVRIdx]%3600)%60));
            }
            if(_bAutoScriptJumpToTime == FALSE && _bAutoScriptDbg == TRUE && u32LastPlaybackTime[u8PVRIdx] > 0)
            {
                if(u32LastPlaybackTime[u8PVRIdx] > _u32PlaybackTime[u8PVRIdx] && (u32LastPlaybackTime[u8PVRIdx] - _u32PlaybackTime[u8PVRIdx]) > PVR_PLAYBACKTIME_INTERVAL)
                {
                    printf("\33[31mTimestamp incorrect!!!!, u32LastPlaybackTime = %"DTC_MS_U32_u" u32PlaybackTime = %"DTC_MS_U32_u", u8PVRIdx = %u\33[m\n" ,u32LastPlaybackTime[u8PVRIdx], _u32PlaybackTime[u8PVRIdx], u8PVRIdx);
                }
                else if(u32LastPlaybackTime[u8PVRIdx] < _u32PlaybackTime[u8PVRIdx] && (_u32PlaybackTime[u8PVRIdx] - u32LastPlaybackTime[u8PVRIdx]) > PVR_PLAYBACKTIME_INTERVAL)
                {
                    printf("\33[31mTimestamp incorrect!!!!, u32LastPlaybackTime = %"DTC_MS_U32_u" u32PlaybackTime = %"DTC_MS_U32_u", u8PVRIdx = %u\33[m\n" ,u32LastPlaybackTime[u8PVRIdx], _u32PlaybackTime[u8PVRIdx], u8PVRIdx);
                }

            }
            else if(_bAutoScriptJumpToTime == TRUE && _bAutoScriptDbg == TRUE)
            {
                _bAutoScriptJumpToTime = FALSE;
            }
            break;

        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SIZE:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            DemoPvrEvent_print("[%s][%d]Playback idx = %d  Playback size=%"DTC_MS_U32_u" KByte\n",__FUNCTION__,__LINE__,u8PVRIdx,pstEventInfo->u32Data[1]);

            DEMO_PVR_CHECK_PB(u8PVRIdx,);
            break;

        case EN_APIPVR_EVENT_NOTIFY_FILE_BEGIN:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            DemoPvrEvent_print("EN_PVR_EVENT_NOTIFY_FILE_BEGIN \n");
            MApi_PVR_PlaybackSetSpeed(u8PVRHandle,APIPVR_PLAYBACK_SPEED_1X);
            DemoPvrEvent_print("SetSpeed APIPVR_PLAYBACK_SPEED_1X\n");
            break;

        //need implement pvr playback resume flow in event "EN_APIPVR_EVENT_NOTIFY_FILE_END_RESUME"
        //if MApi_PVR_PlaybackSetFileEndAutoResume(TRUE) is set
        case EN_APIPVR_EVENT_NOTIFY_FILE_END_RESUME:
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            MApi_PVR_PlaybackSetSpeed(u8PVRHandle,APIPVR_PLAYBACK_SPEED_1X);
            printf("EN_APIPVR_EVENT_NOTIFY_FILE_END_RESUME\n");
            break;

        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SUSPEND:
            //Dynamic PMT change flow for DMX / AV / XC Initialization and PVR resume playback
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            _Demo_PVR_ChangePMTEvent((Ex_PVRPlaybackPath)u8PVRIdx, pstEventInfo->u32Data[1]);
            printf("EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SUSPEND\n");
            break;

        //used for data waiting scheme to improve playback nearby file end in background(time-shift) record case
        //three event for data waiting scheme , that is,  data buffering start, data buffering done, data buffering timeout
        //playback maybe abort in waiting data flow if data buffering timeout happended!
        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_DATABUFFERING_STATUS:
            DemoPvrEvent_print("[%s][%d]EN_APIPVR_EVENT_NOTIFY_PLAYBACK_DATABUFFERING_STATUS(%"DTC_MS_U32_u") ",__FUNCTION__,__LINE__,pstEventInfo->u32Data[1]);
            u8PVRHandle = (MS_U8)pstEventInfo->u32Data[0];
            u8PVRIdx = _Demo_PVR_PlayHandler2Idx(u8PVRHandle);
            //printf("frame count: main:%d sub:%d",MApi_VDEC_EX_GetFrameCnt(_Demo_PVR_GetVidStreamID(0)),MApi_VDEC_EX_GetFrameCnt(_Demo_PVR_GetVidStreamID(1)));

            DEMO_PVR_CHECK_PB(u8PVRIdx,);

            if (pstEventInfo->u32Data[1] == EN_APIPVR_PLAYBACK_DATABUFFERING_START)
            {
                printf("Data Buffering Start\n");
            }
            else if (pstEventInfo->u32Data[1] == EN_APIPVR_PLAYBACK_DATABUFFERING_DONE)
            {
                printf("Data Buffering Done\n");
            }
            else if (pstEventInfo->u32Data[1] == EN_APIPVR_PLAYBACK_DATABUFFERING_TIMEOUT)
            {
                printf("Data Buffering Timeout\n");
            }
            break;

        case EN_APIPVR_EVENT_ERROR_SYNCBYTE_ERROR:
        case EN_APIPVR_EVENT_ERROR_OVERRUN:
        case EN_APIPVR_EVENT_NOTIFY_THUMBNAIL_AUTO_CAPTURE:
            break;
        default :
            DemoPvrEvent_print("default  enEvent=%d\n",pstEventInfo->pvrEvent);
            break;
    }
}

static void _Demo_PVR_Task(MS_U32 argc, void *argv)
{
    PVREventInfo_t stEventInfo;
    MS_U32 u32MessageSize ;

    memset((void *)&stEventInfo,0,sizeof(PVREventInfo_t));

    while(_gbPvrEventExit != TRUE)
    {
        if(TRUE==MsOS_RecvFromQueue(_s32QueueID, (MS_U8 *)&stEventInfo, sizeof(PVREventInfo_t), &u32MessageSize, MSOS_WAIT_FOREVER))
        {
            //printf("Recv stEventInfo %d,%d,%d\n",stEventInfo.pvrMode,stEventInfo.pvrEvent,stEventInfo.u32Data);
            _Demo_PVR_Callback(&stEventInfo);
        }

    }
    printf("_Demo_PVR_Task terminated successfully\n");
    MsOS_SetEvent(_gs32PvrEvtGrp, PVR_EVENT_DESTROY_PROC_EXIT);
}

static MS_BOOL _Demo_PVR_InsertFilter(PVRFilterInfo_t *pFilter, MS_U16 u16PID, MS_U32 u32DmxFltType,MS_U32 *pu32Idx)
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

    if(u16PID==INVALID_PID_ID)
    {
        return FALSE;
    }

    DemoPvr_print("[%s][%d] Insert u16PID=%u u32DmxFltType=%"DTC_MS_U32_u" \n", __FUNCTION__, __LINE__,u16PID,u32DmxFltType);
    pflt[u32Idx].u16PID = u16PID;
    pflt[u32Idx].u32DmxFltType = u32DmxFltType;
    *pu32Idx=u32Idx+1;

    return TRUE;
}

static MS_U16 _Demo_PVR_FindPidinFilter(PVRFilterInfo_t *pFilter,MS_U32 u32DmxFltType)
{
    int i=0;
    if(pFilter==NULL)
    {
        return INVALID_PID_ID;
    }
    for(i=0;i<APIPVR_MAX_PID_NUM;i++)
    {
        if((pFilter[i].u32DmxFltType == u32DmxFltType)&&(pFilter[i].u16PID!=0))
        {
            DemoPvr_print("[%s@%d] Find Pid : %u \n", __FUNCTION__, __LINE__,pFilter[i].u16PID);
            return pFilter[i].u16PID;
        }
    }
    return INVALID_PID_ID;
}

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
static MS_BOOL _Demo_PVR_Record_DSCMB_Create(PVR_DSCMB_INFO *stDSCMBInfo)
{
    if(stDSCMBInfo == NULL)
    {
        printf("[%s][%d]NULL stDSCMBInfo!\n",__FILE__,__LINE__);
        return FALSE;
    }
    if ((stDSCMBInfo->bDSCMBInfoSet == TRUE) && (stDSCMBInfo->bConnectStatus != TRUE))
    {
        if (stDSCMBInfo->u32DscmbId == 0)
        {
            if((stDSCMBInfo->u32DscmbId = Demo_DSCMB_Create_Resource()) == DRV_DSCMB_FLT_NULL)
            {
                printf("[%s][%d]Demo_DSCMB_Create_Resource() failed\n",__FILE__,__LINE__);
                stDSCMBInfo->u32DscmbId = 0;
                return FALSE;
            }
            printf("[%s][%d]Demo_DSCMB_Create_Resource() success\n",__FILE__,__LINE__);
        }
        else
        {
            printf("[%s][%d]DSCMB Create failed, DscmbId(%"DTC_MS_U32_u") already exist! \n",__FILE__,__LINE__,stDSCMBInfo->u32DscmbId);
            return FALSE;
        }
        stDSCMBInfo->bConnectStatus = TRUE;
    }
    return TRUE;
}

static MS_BOOL _Demo_PVR_Record_DSCMB_Destroy(PVR_DSCMB_INFO *stDSCMBInfo)
{
    if(stDSCMBInfo == NULL)
    {
        printf("[%s][%d]NULL stDSCMBInfo!\n",__FILE__,__LINE__);
        return FALSE;
    }

    if ((stDSCMBInfo->bConnectStatus == TRUE) && (stDSCMBInfo->u32DscmbId != 0))
    {
        if(Demo_DSCMB_Destroy_Resource(stDSCMBInfo->u32DscmbId) == FALSE)
        {
            printf("[%s][%d]Demo_DSCMB_Destroy_Resource() failed!\n",__FILE__,__LINE__);
            return FALSE;
        }
        stDSCMBInfo->u32DscmbId = 0;
        stDSCMBInfo->bConnectStatus = FALSE;
        printf("[%s][%d]Demo_DSCMB_Destroy_Resource() success\n",__FILE__,__LINE__);
        return TRUE;
    }
    printf("[%s][%d]No DSCMB resource to destroy!\n",__FILE__,__LINE__);
    return TRUE;
}

static MS_BOOL _Demo_PVR_Record_DSCMB_SetInfo(PVR_DSCMB_INFO *stDSCMBInfo)
{
    if(stDSCMBInfo == NULL)
    {
        printf("[%s][%d]NULL stDSCMBInfo!\n",__FILE__,__LINE__);
        return FALSE;
    }

    if (stDSCMBInfo->bConnectStatus == TRUE)
    {
        if(Demo_DSCMB_SetFltType(stDSCMBInfo->u32DscmbId, stDSCMBInfo->u8Type) == FALSE)
        {
            printf("[%s][%d]Demo_DSCMB_SetFltType() failed\n",__FILE__,__LINE__);
            return FALSE;
        }
        if(Demo_DSCMB_SetCW(stDSCMBInfo->u32DscmbId, stDSCMBInfo->u8StrOddCW, stDSCMBInfo->u8StrEvenCW) == FALSE)
        {
            printf("[%s][%d]Demo_DSCMB_SetCW() failed\n",__FILE__,__LINE__);
            return FALSE;
        }
    }
    return TRUE;
}

static MS_BOOL _Demo_PVR_Record_DSCMB_Connect(PVR_DSCMB_INFO *stDSCMBInfo, MS_U16 u16PID, MS_U8 *pu8DmxFltId)
{
    if (stDSCMBInfo->bConnectStatus == TRUE)
    {
        if(Demo_DSCMB_ConnectFltId(stDSCMBInfo->u32DscmbId, *pu8DmxFltId) == FALSE)
        {
            printf("[%s][%d]DSCMB connect [PID:Flt][%u:%u] failed!\n",__FILE__,__LINE__,u16PID, *pu8DmxFltId);
            return FALSE;
        }

        DemoPvr_print("[%s][%d]DSCMB connect [PID:Flt][%u:%u] success",__FILE__,__LINE__,u16PID, *pu8DmxFltId);
        return TRUE;
    }
    DemoPvr_print("[%s][%d]ByPass DSCMB Connect. DSCMB resource not create!\n",__FILE__,__LINE__);
    return TRUE;
}

static MS_BOOL _Demo_PVR_Record_DSCMB_Disconnect(PVR_DSCMB_INFO *stDSCMBInfo, MS_U8 *pu8DmxFltId)
{
    if (stDSCMBInfo->bConnectStatus == TRUE)
    {
        if(Demo_DSCMB_DisconnectFltId(stDSCMBInfo->u32DscmbId, *pu8DmxFltId) == FALSE)
        {
            printf("[%s][%d]DSCMB disconnect Flt[%u] failed!\n",__FILE__,__LINE__, *pu8DmxFltId);
            return FALSE;
        }
        DemoPvr_print("[%s][%d]DSCMB disconnect Flt[%u] success",__FILE__,__LINE__, *pu8DmxFltId);
        return TRUE;
    }
    DemoPvr_print("[%s][%d]ByPass DSCMB Disconnect. DSCMB resource not create!\n",__FILE__,__LINE__);
    return TRUE;
}
#endif

static MS_BOOL _Demo_PVR_DSCMB_GetLiveInfo(MS_U32 u32DeviceID, PVR_DSCMB_INFO *stDSCMBInfo)
{
    stDEMODSCMBINFO stLiveDSCMBInfo = {0};
    if (Demo_DSCMB_InfoGet(u32DeviceID,&stLiveDSCMBInfo) == TRUE)
    {
        if (stLiveDSCMBInfo.bConnectStatus == TRUE)
        {
            memset(stDSCMBInfo, 0, sizeof(PVR_DSCMB_INFO));
            if( (strlen((char *)stLiveDSCMBInfo.u8Type) > PVR_DSCMB_INFO_CW_SIZE) || (strlen((char *)stLiveDSCMBInfo.u8StrOddCW) > PVR_DSCMB_INFO_CW_SIZE) || (strlen((char *)stLiveDSCMBInfo.u8StrEvenCW) > PVR_DSCMB_INFO_CW_SIZE) )
            {
                printf("[%s][%d] DSCM CW Size is over %d\n",__FUNCTION__,__LINE__, PVR_DSCMB_INFO_CW_SIZE);
                return FALSE;
            }
            memcpy(stDSCMBInfo->u8Type, stLiveDSCMBInfo.u8Type, strlen((char *)stLiveDSCMBInfo.u8Type));
            memcpy(stDSCMBInfo->u8StrOddCW, stLiveDSCMBInfo.u8StrOddCW, strlen((char *)stLiveDSCMBInfo.u8StrOddCW));
            memcpy(stDSCMBInfo->u8StrEvenCW, stLiveDSCMBInfo.u8StrEvenCW, strlen((char *)stLiveDSCMBInfo.u8StrEvenCW));
            stDSCMBInfo->bDSCMBInfoSet = TRUE;
            DemoPvr_print("Get LIVE DSCMB Info => Device:(%d) Type:(%s)  CW: Odd(%s) Even(%s)\n",u32DeviceID, stDSCMBInfo->u8Type,stDSCMBInfo->u8StrOddCW,stDSCMBInfo->u8StrEvenCW);
            return TRUE;
        }
    }
    return FALSE;
}
static MS_BOOL _Demo_PVR_DSCMB_Live_Connect(MS_U32 *u32DeviceID, PVR_DSCMB_INFO *stDSCMBInfo)
{
    if ((stDSCMBInfo->bDSCMBInfoSet == TRUE) && (stDSCMBInfo->bConnectStatus != TRUE))
    {
        if (Demo_DSCMB_ConnectPID(u32DeviceID) == TRUE)
        {
            Demo_DSCMB_SetType(u32DeviceID,stDSCMBInfo->u8Type);
            Demo_DSCMB_CW(u32DeviceID,stDSCMBInfo->u8StrOddCW,stDSCMBInfo->u8StrEvenCW);
            stDSCMBInfo->bConnectStatus = TRUE;
            return TRUE;
        }
        printf("[%s][%d]DSCMB Live connect device(%"DTC_MS_U32_u")failed!\n",__FILE__,__LINE__, *u32DeviceID);
    }
    return FALSE;
}
static MS_BOOL _Demo_PVR_DSCMB_Live_Disconnect(MS_U32 *u32DeviceID, PVR_DSCMB_INFO *stDSCMBInfo)
{
    if (stDSCMBInfo->bConnectStatus == TRUE)
    {
        if (Demo_DSCMB_DisconnectPID(u32DeviceID) == TRUE)
        {
            stDSCMBInfo->bConnectStatus = FALSE;
            return TRUE;
        }
        printf("[%s][%d]DSCMB Live disconnect device(%"DTC_MS_U32_u")failed!\n",__FILE__,__LINE__, *u32DeviceID);
    }
    return FALSE;
}

static MS_BOOL _Demo_PVR_ResetProgramInfo(PVRProgramInfo_t *pPromInfo, MS_BOOL bOnlyResetPIDs)
{
    int i=0;
    DemoPvr_print("%s@%d :: %p\n",__FUNCTION__,__LINE__,pPromInfo);



    if (!bOnlyResetPIDs)
    {
        memset(pPromInfo,0,sizeof(PVRProgramInfo_t));
    }

    for(i=0;i<APIPVR_MAX_PID_NUM;i++)
    {
        pPromInfo->Filters[i].u16PID=INVALID_PID_ID;
    }

    for(i=0;i<APIPVR_MAX_TTXINFO_NUM;i++)
    {
        pPromInfo->TXTInfo[i].u16TTX_Pid=INVALID_PID_ID;
    }
    for(i=0;i<APIPVR_MAX_AUDIOINFO_NUM;i++)
    {
        pPromInfo->AudioInfo[i].u16AudPID=INVALID_PID_ID;
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        pPromInfo->DVBSubtInfo[i].u16Sub_Pid=INVALID_PID_ID;
    }
    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        pPromInfo->EBUSubtInfo[i].u16TTX_Pid=INVALID_PID_ID;
    }
    pPromInfo->u16CCPid=INVALID_PID_ID;

    return TRUE;
}

static MS_U32 _Demo_PVR_Get_PromInfo_ADEC_Fmt_FromPVRDDI(MS_U32 u32ACodec)
{
    switch (u32ACodec)
    {
        case E_DDI_PVR_AUDIO_CODEC_TYPE_MPEG:
            return EN_APIPVR_AUD_DVB_MPEG;
        case E_DDI_PVR_AUDIO_CODEC_TYPE_AC3:
            return EN_APIPVR_AUD_DVB_AC3;
        case E_DDI_PVR_AUDIO_CODEC_TYPE_AC3P:
            return EN_APIPVR_AUD_DVB_AC3P;
        case E_DDI_PVR_AUDIO_CODEC_TYPE_AAC:
            return EN_APIPVR_AUD_DVB_AAC;
        default:
            break;
    }
    return EN_APIPVR_AUD_DVB_NONE;
}

static APIPVR_AUD_CODEC_TYPE _Demo_PVR_Get_PromInfo_ADEC_Fmt_FromADECDDI(MS_U32 u32ACodec)
{
    switch (u32ACodec)
    {
        case DEMO_AUDIO_CODEC_MPEG:
            return EN_APIPVR_AUD_DVB_MPEG;
        case DEMO_AUDIO_CODEC_AC3:
            return EN_APIPVR_AUD_DVB_AC3;
        case DEMO_AUDIO_CODEC_AC3P:
            return EN_APIPVR_AUD_DVB_AC3P;
        case DEMO_AUDIO_CODEC_AAC:
            return EN_APIPVR_AUD_DVB_AAC;
        default:
            break;
    }
    return EN_APIPVR_AUD_DVB_NONE;
}

static MS_U32 _Demo_PVR_Get_ADECDDI_ADEC_Fmt_FromPromInfo(APIPVR_AUD_CODEC_TYPE u32ACodecPromInfo)
{
    switch (u32ACodecPromInfo)
    {
        case EN_APIPVR_AUD_DVB_MPEG:
            return DEMO_AUDIO_CODEC_MPEG;
        case EN_APIPVR_AUD_DVB_AC3:
            return DEMO_AUDIO_CODEC_AC3;
        case EN_APIPVR_AUD_DVB_AC3P:
            return DEMO_AUDIO_CODEC_AC3P;
        case EN_APIPVR_AUD_DVB_AAC:
            return DEMO_AUDIO_CODEC_AAC;
        default:
            break;
    }
    return DEMO_AUDIO_CODEC_NONE;
}

static APIPVR_CODEC_TYPE _Demo_PVR_Get_PromInfo_VDEC_Fmt_FromPVRDDI(MS_U32 u32VCodec)
{
    switch (u32VCodec)
    {
        case E_DDI_PVR_VIDEO_CODEC_TYPE_MPEG2:
            return EN_APIPVR_CODEC_TYPE_MPEG2;
        case E_DDI_PVR_VIDEO_CODEC_TYPE_H264:
            return EN_APIPVR_CODEC_TYPE_H264;
        case E_DDI_PVR_VIDEO_CODEC_TYPE_AVS:
            return EN_APIPVR_CODEC_TYPE_AVS;
        case E_DDI_PVR_VIDEO_CODEC_TYPE_HEVC:
            return EN_APIPVR_CODEC_TYPE_HEVC;
        default:
            break;
    }
    return EN_APIPVR_CODEC_TYPE_NONE;
}

static APIPVR_CODEC_TYPE _Demo_PVR_Get_PromInfo_VDEC_Fmt_FromVDECDDI(MS_U32 u32VCodec)
{
    switch (u32VCodec)
    {
        case E_VDEC_DDI_CODEC_TYPE_MPEG2:
            return EN_APIPVR_CODEC_TYPE_MPEG2;
        case E_VDEC_DDI_CODEC_TYPE_H264:
            return EN_APIPVR_CODEC_TYPE_H264;
        case E_VDEC_DDI_CODEC_TYPE_AVS:
            return EN_APIPVR_CODEC_TYPE_AVS;
        case E_VDEC_DDI_CODEC_TYPE_HEVC:
            return EN_APIPVR_CODEC_TYPE_HEVC;
        default:
            break;
    }
    return EN_APIPVR_CODEC_TYPE_NONE;
}

static MS_U32 _Demo_PVR_Get_VDECDDI_VDEC_Fmt_FromPromInfo(APIPVR_CODEC_TYPE u32VCodecPromInfo)
{
    switch (u32VCodecPromInfo)
    {
        case EN_APIPVR_CODEC_TYPE_MPEG2:
            return E_VDEC_DDI_CODEC_TYPE_MPEG2;
        case EN_APIPVR_CODEC_TYPE_H264:
            return E_VDEC_DDI_CODEC_TYPE_H264;
        case EN_APIPVR_CODEC_TYPE_AVS:
            return E_VDEC_DDI_CODEC_TYPE_AVS;
        case EN_APIPVR_CODEC_TYPE_HEVC:
            return E_VDEC_DDI_CODEC_TYPE_HEVC;
        default:
            break;
    }
    return E_VDEC_DDI_CODEC_TYPE_MPEG2;
}

static MS_BOOL _Demo_PVR_SetAudioStreamInfo(PVRProgramInfo_t *pPromInfo, MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32ACodec)
{
    st_Audio_PG stAudioStreamInfo;
    st_PG *pstPG = NULL;
    MS_U32 u32PgNum = 0;
    MS_U32 u32ProgIdx = 0;
    MS_U32 i = 0;

    //First ACodec
    pPromInfo->u8AudioInfoNum = 1;
    pPromInfo->AudioInfo[0].u16AudPID = (MS_U16)u32AudioPid;
    pPromInfo->AudioInfo[0].u8AudType = (MS_U8)u32ACodec;

    Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);
    for (i = 0; i < u32PgNum; i++)
    {
        if(((u32PCRPid == INVALID_PID_ID) || (u32PCRPid == (pstPG+i)->u32PidPcr)) && ((u32VideoPid == INVALID_PID_ID) || (u32VideoPid == (pstPG+i)->u32PidVideo)) && ((u32AudioPid == INVALID_PID_ID) || Demo_DMX_CheckAudioPgExist(pstPG+i, u32AudioPid)))
        {
            u32ProgIdx = i;
            break;
        }
    }

    if(u32ProgIdx > MAX_PG_IN_PAT_DDI)
    {
        printf("Program index(%"DTC_MS_U32_u") > maximum program number(%d) !\n", u32ProgIdx, MAX_PG_IN_PAT_DDI);
        return FALSE;
    }

    //Insert the other audio information to program information.
    for(i = 0; i < APIPVR_MAX_AUDIOINFO_NUM; i++)
    {
        if(FALSE == Demo_DMX_GetAudioStreamInfo(u32ProgIdx, i, &stAudioStreamInfo))
        {
            printf("[%s][%d] Get Audio Stream(%"DTC_MS_U32_u") info failed!!\n", __FUNCTION__, __LINE__, i);
            return FALSE;
        }

        if(stAudioStreamInfo.u32PidAudio == INVALID_PID_ID)
        {
            continue;
        }

        if(pPromInfo->AudioInfo[0].u16AudPID == stAudioStreamInfo.u32PidAudio)
        {
            pPromInfo->AudioInfo[0].u8ISOAudType = stAudioStreamInfo.u8Lang_audio_type;
            memcpy((void *)pPromInfo->AudioInfo[0].u8ISOLangInfo, (void *)stAudioStreamInfo.u8Lang, sizeof(pPromInfo->AudioInfo[0].u8ISOLangInfo));
        }
        else if(stAudioStreamInfo.u32PidAudio != 0)
        {
            pPromInfo->AudioInfo[pPromInfo->u8AudioInfoNum].u16AudPID = (MS_U16)stAudioStreamInfo.u32PidAudio;
            pPromInfo->AudioInfo[pPromInfo->u8AudioInfoNum].u8AudType = (MS_U8)stAudioStreamInfo.eAudioFmt;
            pPromInfo->AudioInfo[pPromInfo->u8AudioInfoNum].u8ISOAudType = stAudioStreamInfo.u8Lang_audio_type;
            memcpy((void *)pPromInfo->AudioInfo[pPromInfo->u8AudioInfoNum].u8ISOLangInfo, (void *)stAudioStreamInfo.u8Lang, sizeof(pPromInfo->AudioInfo[pPromInfo->u8AudioInfoNum].u8ISOLangInfo));
            DemoPvr_print("pPromInfo->AudioInfo[i].u16AudPID = %u\n",pPromInfo->AudioInfo[pPromInfo->u8AudioInfoNum].u16AudPID);
            (pPromInfo->u8AudioInfoNum)++;
        }
        else
        {
            break;
        }
    }

    return TRUE;
}

static MS_BOOL _Demo_PVR_SetProgramInfo(PVRProgramInfo_t *pPromInfo,MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32VCodec, MS_U32 u32ACodec,MS_U32 u32PmtPid, MS_U32 u32LCN, MS_BOOL bOnlyChangeProgramPIDs)
{
    MS_U32 u32Idx=0;

    if(pPromInfo==NULL)
    {
        printf( "Invalid PromInfo!\n");
        return FALSE;
    }
    _Demo_PVR_ResetProgramInfo(pPromInfo,bOnlyChangeProgramPIDs);

    if(u32VideoPid==INVALID_PID_ID)
    {
        pPromInfo->enServiceType = EN_APIPVR_SERVICETYPE_RADIO;
    }
    else
    {
        pPromInfo->enServiceType = EN_APIPVR_SERVICETYPE_DTV;
    }
    pPromInfo->u16AudioPid = u32AudioPid;

    pPromInfo->u32ACodec = u32ACodec;
    pPromInfo->enVCodec = (APIPVR_CODEC_TYPE)u32VCodec;

    _Demo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)u32VideoPid, (MS_U32)DMX_FILTER_TYPE_VIDEO,&u32Idx);
    _Demo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)u32PCRPid, (MS_U32)DMX_FILTER_TYPE_PCR,&u32Idx);
    _Demo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)u32PmtPid, (MS_U32)DMX_FILTER_TYPE_SECTION,&u32Idx);

    pPromInfo->u32LCN       = u32LCN;

#if 0
    //First ACodec
    pPromInfo->u8AudioInfoNum = 1;
    pPromInfo->AudioInfo[0].u16AudPID = (MS_U16)u32AudioPid;
    pPromInfo->AudioInfo[0].u8AudType = (MS_U16)u32ACodec;
#else
    _Demo_PVR_SetAudioStreamInfo(pPromInfo, u32PCRPid, u32VideoPid, u32AudioPid, u32ACodec);
#endif

#ifdef PVR_DBG
    DemoPvr_print("========= Start =========  %d\n",_u8CurRecProgramIdx);
    DemoPvr_print("[%d]::\n",_u8CurRecProgramIdx);
    DemoPvr_print("u32ACodec=%"DTC_MS_U32_u"\n",pPromInfo->u32ACodec);
    DemoPvr_print("u32AudioPid=%"DTC_MS_U32_d"\n",pPromInfo->u16AudioPid);
    DemoPvr_print("u32LCN=%"DTC_MS_U32_d"\n",pPromInfo->u32LCN);
    DemoPvr_print("u32VCodec=%d\n",pPromInfo->enVCodec);
    DemoPvr_print("========= End ========= %d\n",_u8CurRecProgramIdx);
#endif

    if(b2ndEncryptionEnabled)
    {
        pPromInfo->bIsEncrypted = TRUE;
        #if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
            pPromInfo->enEncryptionType     = EN_APIPVR_ENCRYPTION_DYNAMICKEY;
        #else
            #if defined(DEMO_PVR_DSCMB_SUPPORT_KL)
            pPromInfo->enEncryptionType     = EN_APIPVR_ENCRYPTION_SMARTCARD;
            #else
            pPromInfo->enEncryptionType     = EN_APIPVR_ENCRYPTION_USER;
            #endif // DEMO_PVR_DSCMB_SUPPORT_KL
        #endif // DEMO_PVR_SUPPORT_DYNAMICKEY_TEST
    }
    else
    {
        pPromInfo->bIsEncrypted = FALSE;
        pPromInfo->enEncryptionType     = EN_APIPVR_ENCRYPTION_NONE;
    }

    //Insert as last item;
    _Demo_PVR_InsertFilter(&pPromInfo->Filters[0], (MS_U16)0x00, (MS_U32)DMX_FILTER_TYPE_SECTION,&u32Idx);

    pPromInfo->u64FileSliceSize = _u64FileSliceSize;
    return TRUE;
}

static MS_BOOL _Demo_PVR_RecStart(MS_BOOL bTimeshift, MS_U32 u32FreeSizeInMB)
{
    MS_BOOL bRecordAll=FALSE;
    PVR_AV_ID_t AVStreamID;

    if(PVR_IS_VALID_HANDLER(_u8hRecord[_u8CurRecProgramIdx]))
    {
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        if(bTimeshift != TRUE)
        {
            printf("Already record in idx:%d!\n",_u8CurRecProgramIdx);     //Normal Record Case
            return FALSE;
        }
#else
        printf("Already record!\n");
        return FALSE;
#endif
    }

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
        if (_Demo_PVR_Record_DSCMB_Create(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx]) != TRUE)
        {
            printf("[%s][%d]DSCMB Create Error!\n",__FILE__,__LINE__);
            return FALSE;
        }
#endif

    if(bTimeshift)
    {
        MS_U64 u64FreeSpaceInKB=((MS_U64)u32FreeSizeInMB)<<10;
        MS_U64 u64FileSizeInKB = (_u64FileSizeInKB == PVR_FILE_SIZE_MAX)? u64FreeSpaceInKB : _u64FileSizeInKB;
        _AVStreamID[_eCurPlaybackProgramPath].vstreamID = _Demo_PVR_GetVidStreamID(&_eCurPlaybackProgramPath);
        _AVStreamID[_eCurPlaybackProgramPath].astreamID = _Demo_PVR_GetAudStreamID(&_eCurPlaybackProgramPath);
        if (_AVStreamID[_eCurPlaybackProgramPath].vstreamID == NULL)
        {
            printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__);
            goto _Demo_PVR_RecStart_fail;
        }
        MApi_PVR_EX_FreezeScreenInTimeshiftRecord(_eCurPlaybackProgramPath,EN_APIPVR_ACTION_TYPE_SET, &_bScreenFrozen[_eCurPlaybackProgramPath]);
        MApi_PVR_SetRecordFileSize(_u8CurRecProgramIdx, u64FileSizeInKB);//limited to 4G
        _u64FileSizeInKB = PVR_FILE_SIZE_MAX;
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        if(_bBackgroundRecord[_u8CurRecProgramIdx] == TRUE)
        {
            _bTimeshiftSeamless[_eCurPlaybackProgramPath] = TRUE;
            printf("\033[45;37m ### TimeshiftSeamless Mode ### \033[0m\n");
            printf("[%s][%d] Set _bTimeshiftSeamless (%d)\n",__FUNCTION__,__LINE__,_bTimeshiftSeamless[_eCurPlaybackProgramPath]);
        }
        else
#endif
        {
            printf("\033[45;37m ### Timeshift Mode ### \033[0m\n");
            if(PVR_IS_VALID_HANDLER(_u8hRecord[_u8CurRecProgramIdx]))
            {
                printf("Already Record\n");
                goto _Demo_PVR_RecStart_fail;
            }
        }

        if(FALSE == MApi_PVR_EX_TimeshiftRecordEngSet(&_u8hRecord[_u8CurRecProgramIdx],(void *)(&_AVStreamID[_eCurPlaybackProgramPath]),_u8CurRecProgramIdx,_eCurPlaybackProgramPath))
        {
            printf("Record eng set fail!\n");
            goto _Demo_PVR_RecStart_fail;
        }

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
        if (_Demo_PVR_Record_DSCMB_SetInfo(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx]) != TRUE)
        {
            printf("[%s][%d]DSCMB SetInfo Error!\n",__FILE__,__LINE__);
            goto _Demo_PVR_RecStart_fail;
        }
#endif
        if( FALSE == MApi_PVR_EX_TimeshiftRecordEngEnable(&_u8hRecord[_u8CurRecProgramIdx]))
        {
        #if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
            printf("[%s][%d]_u8BackProgramIdx:%u\n\n",__FUNCTION__,__LINE__,_u8BackProgramIdx);
            if(_bBackgroundRecord[_u8CurRecProgramIdx] == TRUE)
            {
                printf("\033[45;37m ### TimeshiftSeamless Start Failed ### \033[0m\n");
            }
        #endif
            Demo_PVR_Timeshift_RecordStop();
            goto _Demo_PVR_RecStart_fail;
        }

        DemoPvr_print("[%s][%d] timeshift record start [%d]=%d bRet=%d\n",__FUNCTION__,__LINE__,_u8CurRecProgramIdx,_u8hRecord[_u8CurRecProgramIdx],bRet);
    }
    else
    {
        if (PVR_IS_VALID_HANDLER(_u8hRecord[_u8CurRecProgramIdx]))
        {
            printf("Already Record!\n");
            goto _Demo_PVR_RecStart_fail;
        }
        AVStreamID.vstreamID = (void *)(&_NullVidStreamID);
        AVStreamID.astreamID = (void *)(&_NullAudStreamID);

        MS_U64 u64FileSizeInMB = (_u64FileSizeInKB == PVR_FILE_SIZE_MAX)? u32FreeSizeInMB : _u64FileSizeInKB/1024;

        if(_bBoundedLinearRecord)
            MApi_PVR_SetRecordFileSize(_u8CurRecProgramIdx, _u64FileSizeInKB);

        if(FALSE == MApi_PVR_EX_RecordEngSet(&_u8hRecord[_u8CurRecProgramIdx],(void *)(&AVStreamID),_u8CurRecProgramIdx, APIPVR_FILE_LINEAR, u64FileSizeInMB,_u32Duration))
        {
            printf("Record eng set fail!\n");
            goto _Demo_PVR_RecStart_fail;
        }

        _u64FileSizeInKB = PVR_FILE_SIZE_MAX;
        _u32Duration = 0;

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
        if (_Demo_PVR_Record_DSCMB_SetInfo(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx]) != TRUE)
        {
            printf("[%s][%d]DSCMB SetInfo Error!\n",__FILE__,__LINE__);
            goto _Demo_PVR_RecStart_fail;
        }
#endif
        if( FALSE == MApi_PVR_EX_RecordEngEnable(&_u8hRecord[_u8CurRecProgramIdx],bRecordAll))
        {
            Demo_PVR_Record_Stop();
            goto _Demo_PVR_RecStart_fail;
        }
        DemoPvr_print("[%s][%d] record start %d\n",__FUNCTION__,__LINE__,_u8hRecord[_u8CurRecProgramIdx]);
    }

#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
    MS_U8 cmd = 0x1;
    MS_U32 intervelTime = 10000;
    _Demo_PVR_SetDynamicKey(&_u8CurRecProgramIdx, &cmd, &intervelTime);
#endif
    return TRUE;

_Demo_PVR_RecStart_fail:
#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
   _Demo_PVR_Record_DSCMB_Destroy(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx]);
#endif
    return FALSE;
}

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
static MS_BOOL _Demo_PVR_Background_RecStart(void)
{
    MS_BOOL bRecordAll=FALSE;
    PVR_AV_ID_t AVStreamID;

    if(!PVR_IS_VALID_HANDLER(_u8hRecord[_u8CurRecProgramIdx]))
    {
        AVStreamID.vstreamID = (void *)(&_NullVidStreamID);
        AVStreamID.astreamID = (void *)(&_NullAudStreamID);
        DemoPvr_print("[%s][%d] record start %d\n",__FUNCTION__,__LINE__,_u8hRecord[_u8CurRecProgramIdx]);
        //Dscmb Flow

        if(FALSE == MApi_PVR_EX_BackRecordEngSet(&_u8hRecord[_u8CurRecProgramIdx],(void *)(&AVStreamID),_u8CurRecProgramIdx, APIPVR_FILE_CIRCULAR, 0,0))
        {
            printf("Back record eng set fail!\n");
            return FALSE;
        }

#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
        if (_Demo_PVR_Record_DSCMB_SetInfo(&stPVRBackRecordDSCMBInfo[_u8CurRecProgramIdx]) != TRUE)
        {
            printf("[%s][%d]DSCMB SetInfo Error!\n",__FILE__,__LINE__);
            goto _Demo_PVR_Background_RecStart_fail;
        }
#endif
        if(FALSE == MApi_PVR_EX_RecordEngEnable(&_u8hRecord[_u8CurRecProgramIdx],bRecordAll))
        {
            printf("Back record eng enable fail!\n");
            Demo_PVR_SeamlessBackRecordStop();
            goto _Demo_PVR_Background_RecStart_fail;
        }
    }
    else
    {
        printf("Already record in idx:%d \n",_u8hRecord[_u8CurRecProgramIdx]);
    }

    return TRUE;

_Demo_PVR_Background_RecStart_fail:
#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
   _Demo_PVR_Record_DSCMB_Destroy(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx]);
#endif
    return FALSE;
}

static MS_BOOL _Demo_PVR_BackgroundRecordStart(MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32PCRPid, MS_U32 u32VCodec, MS_U32 u32ACodec, MS_U16 u16PmtPid, MS_U16 u16LCN)
{
    MS_U32 u32TempVCodec = 0;
    MS_U32 u32TempACodec = 0;
    EN_DEMO_DMX_FLOW_INPUT eCurDMXInputSrc = _eCurDMXInputSrc;
    EN_DEMO_DMX_FLOW eCurDMXFlowSet = _eCurDMXFlowSet[_u8CurRecProgramIdx];

    u32TempVCodec = (MS_U32)_Demo_PVR_Get_PromInfo_VDEC_Fmt_FromPVRDDI(u32VCodec);
    u32TempACodec = (MS_U32)_Demo_PVR_Get_PromInfo_ADEC_Fmt_FromPVRDDI(u32ACodec);
    if(FALSE==_Demo_PVR_SetProgramInfo(&_backrecprogramInfo[_u8CurRecProgramIdx],u32PCRPid, u32VideoPid, u32AudioPid, u32TempVCodec, u32TempACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN,FALSE))
    {
        printf("[%s][%d]Can't set program info!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(b2ndEncryptionEnabled)
    {
        #if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
        _backrecprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_DYNAMICKEY;
        printf("Apply PVR CA mode(EN_APIPVR_ENCRYPTION_DYNAMICKEY)!\n");
        #else
            #if defined(DEMO_PVR_DSCMB_SUPPORT_KL)
            _backrecprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_SMARTCARD;
            printf("Apply PVR CA mode(EN_APIPVR_ENCRYPTION_SMARTCARD)!\n");
            #else // defined(DEMO_PVR_DSCMB_SUPPORT_KL)
            _backrecprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_USER;
            printf("Apply PVR CA mode(EN_APIPVR_ENCRYPTION_USER)!\n");
            //!!!Notice: Before release record clear stream lib or source code to customer, need follow formal release rule!!!//
            //!!!for details, please contact the HQ PVR module owner about the release flow!!!//
            //_recprogramInfo[_u8CurRecProgramIdx].enEncryptionType     = EN_APIPVR_ENCRYPTION_CLEAR;//record to clear from scrambled, no encryption anymore
            #endif // defined(DEMO_PVR_DSCMB_SUPPORT_KL)
        #endif // DEMO_PVR_SUPPORT_DYNAMICKEY_TEST
    }
    else
    {
        _backrecprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_NONE;
        printf("Not Apply PVR CA mode(EN_APIPVR_ENCRYPTION_NONE)!\n");
    }

    // [01] =============== Store meta data =========================================//
    if(!MApi_PVR_SetBackRecordMetaData(&_backrecprogramInfo[_u8CurRecProgramIdx],_u8CurRecProgramIdx,0,0,0,0))
    {
        printf("[%s][%d]Set MetaData Fail!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    // For merge stream, pass source id to MW
    if(!MApi_PVR_SetRecordSourceID(_u8CurRecProgramIdx, (MS_U8)_eCurDMXFltSourceID[_u8CurRecProgramIdx]))
    {
        printf("[ERROR] Can't set source id!\n");
        return FALSE;
    }

    // [02] ======== DMX Flowset and PID open =====================================//
    if(_u8CurRecProgramIdx < _u8TotalRecorder)
    {
        printf("@@@ _u8CurRecProgramIdx:[%u] eCurDMXInputSrc:[%u], _u32CurDMXFlow:[%u]\n",_u8CurRecProgramIdx,eCurDMXInputSrc,eCurDMXFlowSet);
    }
    else
    {
        printf(" \" over max program !!!! \"\n");
        return FALSE;
    }

    return _Demo_PVR_Background_RecStart();
}
#endif

/// Using this function to stop recording!!!!!!!!!!!!!
static MS_BOOL _Demo_PVR_RecStop(MS_BOOL bTimeshift, MS_U8 u8ProgramIdx)
{
    MS_BOOL bRet=FALSE;
    MS_U8 u8PlaybackIdx = 0;

    if(!PVR_IS_VALID_HANDLER(_u8hRecord[u8ProgramIdx]))
    {
        printf("[%s][%d] Already stop recording %d\n",__FUNCTION__,__LINE__,_u8hRecord[u8ProgramIdx]);
        return TRUE;
    }

#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
    // @NOTE DynamicKey Add
    MS_U8 cmd = 0x0;
    MS_U32 intervelTime = 10000;

    _Demo_PVR_SetDynamicKey(&_u8CurRecProgramIdx, &cmd, &intervelTime);
#endif

    if(bTimeshift)
    {
        _AVStreamID[_eCurPlaybackProgramPath].vstreamID = _Demo_PVR_GetVidStreamID(&_eCurPlaybackProgramPath);
        _AVStreamID[_eCurPlaybackProgramPath].astreamID = _Demo_PVR_GetAudStreamID(&_eCurPlaybackProgramPath);
        if (_AVStreamID[_eCurPlaybackProgramPath].vstreamID == NULL)
        {
            printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__);
            return FALSE;
        }

        u8PlaybackIdx =  _Demo_PVR_Timeshift_GetPlaybackIdx(u8ProgramIdx);
        if (u8PlaybackIdx != PVR_PATH_INVALID)
        {
            bRet=MApi_PVR_EX_TimeshiftRecordStop(&_u8hRecord[u8ProgramIdx],(void *)&_AVStreamID[_eCurPlaybackProgramPath],&_u8hPlayback[u8PlaybackIdx],u8ProgramIdx,_eCurPlaybackProgramPath);
        }
        DemoPvr_print("[%s][%d] timeshift record stop %d\n",__FUNCTION__,__LINE__,_u8hRecord[u8ProgramIdx]);
    }
    else
    {
        bRet=MApi_PVR_RecordStop(&_u8hRecord[u8ProgramIdx],u8ProgramIdx);
        DemoPvr_print("[%s][%d] record stop %d\n",__FUNCTION__,__LINE__,_u8hRecord[u8ProgramIdx]);
    }

    _Demo_PVR_ResetProgramInfo(&_recprogramInfo[u8ProgramIdx],FALSE);
#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
   if (_Demo_PVR_Record_DSCMB_Destroy(&stPVRRecordDSCMBInfo[u8ProgramIdx]) == FALSE)
    {
        printf("[%s][%d]DSCMB Destroy Error!\n",__FILE__,__LINE__);
        return FALSE;
    }
#endif

    return bRet;
}

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
MS_BOOL Demo_PVR_SeamlessBackRecordStop(void)
{
    DEMO_PVR_CHECK_INIT();
    MS_BOOL bRet = FALSE;
    _u8BackProgramIdx = _u8CurRecProgramIdx;
    DemoPvr_print("Demo_PVR_Record_Stop %d ::%s\n",_u8BackProgramIdx,_backrecprogramInfo[_u8BackProgramIdx].FileName);

    if(!PVR_IS_VALID_HANDLER(_u8hRecord[_u8BackProgramIdx]))
    {
        printf("[%s][%d] Already stop recording %d\n",__FUNCTION__,__LINE__,_u8hRecord[_u8CurRecProgramIdx]);
        return TRUE;
    }

    bRet=MApi_PVR_BackRecordStop(&_u8hRecord[_u8BackProgramIdx],_u8BackProgramIdx);
    if(bRet == FALSE)
    {
        printf("[%s][%d] Back ground record stop fail!%d\n",__FUNCTION__,__LINE__,_u8hRecord[_u8BackProgramIdx]);
        return FALSE;
    }

    _Demo_PVR_ResetProgramInfo(&_backrecprogramInfo[_u8BackProgramIdx],FALSE);
    DemoPvr_print("Demo_PVR_Record_Stop -->end \n");
#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)

   if (_Demo_PVR_Record_DSCMB_Destroy(&stPVRBackRecordDSCMBInfo[_u8BackProgramIdx]) == FALSE)
    {
        printf("[%s][%d]DSCMB Destroy Error!\n",__FILE__,__LINE__);
        return FALSE;
    }
#endif
    _bBackgroundRecord[_u8BackProgramIdx] = FALSE;

    return TRUE;
}
#endif

MS_BOOL _Demo_Dmx_GetCurProgram(EN_DEMO_DMX_FLOW u8DmxFlow,MS_U32 u32PCRPid,MS_U32 u32VideoPid,MS_U32 u32AudioPid,MS_U16 *pu16PtmPid,MS_U16 *pu16LCN)
{
    st_PG *pstPG = NULL;
    MS_U32 u32PgNum;
    MS_U32 i=0;
    MS_BOOL bRes = FALSE;
    //printf("[%s][%d]     u16PCRPid=0x%04x, u16VideoPid0x%04x, u16AudioPid0x%04x\n", __FUNCTION__, __LINE__, u16PCRPid, u16VideoPid, u16AudioPid);

    // For merge stream, before doing dmx scan, change source id to source id of current record idx
    EN_DEMO_DMX_FLT_SOURCEID eTempSrcID = Demo_DMX_GetSrcID(u8DmxFlow);
    Demo_DMX_SetSrcID(u8DmxFlow, _eCurDMXFltSourceID[_u8CurRecProgramIdx]);

    if(Demo_DMX_Scan(u8DmxFlow, FALSE)==TRUE)
    {
        Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);
        for (i= 0; i< u32PgNum; i++)
        {
            if( (u32PCRPid== INVALID_PID_ID||u32PCRPid == (pstPG+i)->u32PidPcr) && (u32VideoPid== INVALID_PID_ID||u32VideoPid == (pstPG+i)->u32PidVideo) && ( u32AudioPid== INVALID_PID_ID|| Demo_DMX_CheckAudioPgExist(pstPG+i, u32AudioPid)))
            {
/*
        printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidVideo);
        printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidAudio);
        printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidPcr);
        printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidPmt);
        printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32StreamTypeVideo);
        printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32StreamTypeAudio);
        printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32ProgNum);
        printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, ((pstPG+i)->b264)? "TRUE": "FALSE");
        printf("[%s][%d]     AudioFmt  = 0x%08x\n", __FUNCTION__, __LINE__, (pstPG+i)->eAudioFmt);
*/
                *pu16PtmPid = (MS_U16)(pstPG+i)->u32PidPmt;
                *pu16LCN = (MS_U16)(pstPG+i)->u32ProgNum;

                bRes = TRUE;
                break;
            }
        }
    }

    // For merge stream, recover source id after dmx scan
    Demo_DMX_SetSrcID(u8DmxFlow, eTempSrcID);

    return bRes;
}

MS_BOOL _Demo_Dmx_GetLiveProgram(MS_U8 u8LivePath, MS_U32 *u32PCRPid,MS_U32 *u32VideoPid,MS_U32 *u32AudioPid,MS_U32 *u32VCodec,MS_U32 *u32ACodec,MS_U16 *pu16PtmPid,MS_U16 *pu16LCN)
{
    st_PG *pstPG = NULL;
    EN_AV_Device eDevice = (EN_AV_Device)u8LivePath;
    MS_U32 u32PgNum;
    MS_U32 i=0;
    MS_BOOL bRes = FALSE;

    if (Demo_AV_GetAVInfo(&eDevice,E_AV_GetCmd_LiveInfo,&_stLiveInfo[_eCurPlaybackProgramPath])  == FALSE)
    {
        printf("[%s]Error!!!, Get Live info by Demo_AV_GetAVInfo failed\n",__FUNCTION__);
        return FALSE;
    }


    *u32AudioPid = (MS_U32)_stLiveInfo[_eCurPlaybackProgramPath].stAudioParams.u16PID;
    *u32VideoPid = (MS_U32)_stLiveInfo[_eCurPlaybackProgramPath].stVideoParams.u16PID;
    *u32PCRPid = (MS_U32)_stLiveInfo[_eCurPlaybackProgramPath].stPCRParams.u16PID;
    *u32VCodec = (MS_U32)_stLiveInfo[_eCurPlaybackProgramPath].stVideoParams.eVideoCodec;
    *u32ACodec = (MS_U32)_stLiveInfo[_eCurPlaybackProgramPath].stAudioParams.eAudioCodec;
    if (_stLiveInfo[_eCurPlaybackProgramPath].bInited == FALSE && *u32VideoPid != INVALID_PID_ID)// bInited is set to TRUE in Demo_AV_PlayAV, so for audio only case, we will not return false since it was not initialized in Demo_AV_PlayRadio
    {
        printf("[%s]Error!!!, Get Live PATH(%d) is not alive.\n",__FUNCTION__,u8LivePath);
        return FALSE;
    }

    printf("[%s][%d]Live Program[eFlow:%"DTC_MS_U32_u"]   u32PCRPid=0x%04"DTC_MS_U32_x", u32VideoPid0x%04"DTC_MS_U32_x", u32AudioPid0x%04"DTC_MS_U32_x" u32VCodec:%04"DTC_MS_U32_x" u32ACodec:%04"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, (MS_U32)_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, *u32PCRPid, *u32VideoPid, *u32AudioPid, *u32VCodec, *u32ACodec);

    // For merge stream, before doing dmx scan, change source id to source id of current DTV
    EN_DEMO_DMX_FLT_SOURCEID eTempDTVSrcID = _stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eDmxFltSourceID;
    EN_DEMO_DMX_FLT_SOURCEID eTempSrcID = Demo_DMX_GetSrcID(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow);
    Demo_DMX_SetSrcID(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, eTempDTVSrcID);

    if(Demo_DMX_Scan(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, FALSE)==TRUE)
    {
        Demo_DMX_GetProgramInfo(&pstPG, &u32PgNum);
        for (i= 0; i< u32PgNum; i++)
        {
            if( (*u32PCRPid== INVALID_PID_ID||*u32PCRPid== (pstPG+i)->u32PidPcr) && (*u32VideoPid== INVALID_PID_ID||*u32VideoPid== (pstPG+i)->u32PidVideo) && (*u32AudioPid== INVALID_PID_ID||Demo_DMX_CheckAudioPgExist(pstPG+i, *u32AudioPid)))
            {
                *pu16PtmPid = (MS_U16)(pstPG+i)->u32PidPmt;
                *pu16LCN = (MS_U16)(pstPG+i)->u32ProgNum;
                            /*
                            printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidVideo);
                            printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidAudio);
                            printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidPcr);
                            printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32PidPmt);
                            printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32StreamTypeVideo);
                            printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32StreamTypeAudio);
                            printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)(pstPG+i)->u32ProgNum);
                            printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, ((pstPG+i)->b264)? "TRUE": "FALSE");
                            printf("[%s][%d]     AudioFmt  = 0x%08x\n", __FUNCTION__, __LINE__, (pstPG+i)->eAudioFmt);
                            */
                bRes = TRUE;
                break;
            }
        }
    }

    // For merge stream, recover source id after dmx scan
    Demo_DMX_SetSrcID(_stLiveInfo[_eCurPlaybackProgramPath].stDMXflowParams.eFlow, eTempSrcID);

    // There is no live program now. It need to stop the DTV, otherwise the video decoder will initialize fail during playbacking.
    if(bRes == FALSE)
    {
        _DualAVFunc.DualAV_StopAV(&eDevice);
    }
    return bRes;
}

static MS_BOOL _Demo_PVR_StoreProgramInfo(EN_DEMO_DMX_FLOW eDmxPvrInput, MS_BOOL bTimeshift,MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32ACodec, MS_U32 u32VCodec , char *fileName)
{
    MS_U16 u16PmtPid=INVALID_PID_ID;
    MS_U16 u16LCN=0xFFFF;
    MS_U8 enRecIdx=_u8CurRecProgramIdx;
    MS_U32 u32TempACodec = 0;
    MS_U32 u32TempVCodec = 0;
    Ex_PVRPlaybackPath ePlaybackidx = PVR_PATH_INVALID;

    if(bTimeshift)
    {
        ePlaybackidx = (Ex_PVRPlaybackPath)_Demo_PVR_Timeshift_GetPlaybackIdx(enRecIdx);
        DEMO_PVR_CHECK_PB(ePlaybackidx, FALSE);
    }
    // [01] Get relative PID info ==============================================//

    if(_Demo_Dmx_GetCurProgram(eDmxPvrInput,u32PCRPid,u32VideoPid,u32AudioPid,&u16PmtPid,&u16LCN) == FALSE)
    {
        printf("Fail to get Live TV program PMT and PAT Info.\n");
        if(bTimeshift)
        {
            _livePromInfo[ePlaybackidx].bLive = FALSE;
        }
        return FALSE;
    }

    u32TempVCodec = (MS_U32)_Demo_PVR_Get_PromInfo_VDEC_Fmt_FromPVRDDI(u32VCodec);
    u32TempACodec = (MS_U32)_Demo_PVR_Get_PromInfo_ADEC_Fmt_FromPVRDDI(u32ACodec);


    if(bTimeshift)
    {
        if(FALSE==_Demo_PVR_SetProgramInfo(&_livePromInfo[ePlaybackidx],u32PCRPid, u32VideoPid, u32AudioPid, u32TempVCodec, u32TempACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN,FALSE))
        {
            printf("Can't store program info!\n");
            _livePromInfo[ePlaybackidx].bLive = FALSE;
            return FALSE;
        }
        _livePromInfo[ePlaybackidx].bLive = TRUE;

        if(b2ndEncryptionEnabled)
        {
            EN_AV_Device eDevice = E_AV_DEVICE_MAIN;
            Demo_PVR_Check_AVPath(eDevice, ePlaybackidx,PVR_PATH_MAIN,E_AV_DEVICE_MAIN,PVR_PATH_SUB,E_AV_DEVICE_SUB);
            //Get Timeshift Record DSCMB Info From Live Program
            if (_Demo_PVR_DSCMB_GetLiveInfo((MS_U32)eDevice, &stPVRRecordDSCMBInfo[eDevice]) == TRUE)
            {
                DemoPvr_print("Get Timeshift Record(%"DTC_MS_U32_u") DSCMB Info from LIVE => Device:(%"DTC_MS_U32_u") Type:(%s)  CW: Odd(%s) Even(%s)\n",(MS_U32)enRecIdx,(MS_U32)eDevice, stPVRRecordDSCMBInfo[eDevice].u8Type,stPVRRecordDSCMBInfo[eDevice].u8StrOddCW,stPVRRecordDSCMBInfo[eDevice].u8StrEvenCW);
            }
        }
    }

    printf("[Vcodec , Acodec](%"DTC_MS_U32_u",%"DTC_MS_U32_u")\n",u32TempVCodec, u32TempACodec);

    if(FALSE==_Demo_PVR_SetProgramInfo(&_recprogramInfo[_u8CurRecProgramIdx],u32PCRPid, u32VideoPid, u32AudioPid, u32TempVCodec, u32TempACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN,FALSE))
    {
        printf("Can't set program info!\n");
        return FALSE;
    }
    if(strlen(fileName)>FILE_PATH_SIZE)
    {
        printf("fileName size is over %d!\n", FILE_PATH_SIZE);
        return FALSE;
    }
    wcscpy((MS_U16*)_recprogramInfo[_u8CurRecProgramIdx].FileName, (MS_U16*)fileName);

    DemoPvr_print(" _u8CurRecProgramIdx=%d   fileName=",_u8CurRecProgramIdx);
    wprintf((MS_U16*)_recprogramInfo[_u8CurRecProgramIdx].FileName, TRUE, TRUE);

    if(b2ndEncryptionEnabled)
    {
        #if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
        _recprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_DYNAMICKEY;
        printf("Apply PVR CA mode(EN_APIPVR_ENCRYPTION_DYNAMICKEY)!\n");
        #else
            #if defined(DEMO_PVR_DSCMB_SUPPORT_KL)
            _recprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_SMARTCARD;
            printf("Apply PVR CA mode(EN_APIPVR_ENCRYPTION_SMARTCARD)!\n");
            #else//defined(DEMO_PVR_DSCMB_SUPPORT_KL)
            _recprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_USER;
            printf("Apply PVR CA mode(EN_APIPVR_ENCRYPTION_USER)!(%u)\n",_recprogramInfo[_u8CurRecProgramIdx].enEncryptionType);
            //!!!Notice: Before release record clear stream lib or source code to customer, need follow formal release rule!!!//
            //!!!for details, please contact the HQ PVR module owner about the release flow!!!//
            //_recprogramInfo[_u8CurRecProgramIdx].enEncryptionType     = EN_APIPVR_ENCRYPTION_CLEAR;//record to clear from scrambled, no encryption anymore
            #endif // defined(DEMO_PVR_DSCMB_SUPPORT_KL)
        #endif // DEMO_PVR_SUPPORT_DYNAMICKEY_TEST
    }
    else
    {
        _recprogramInfo[_u8CurRecProgramIdx].enEncryptionType = EN_APIPVR_ENCRYPTION_NONE;
        printf("Not Apply PVR CA mode(EN_APIPVR_ENCRYPTION_NONE)!\n");
    }

    // [02] Store meta data ==============================================//
    //{0,0,0,0} is the user KEY for EN_APIPVR_ENCRYPTION_USER Encryption mode
    if(_bAppend)
    {
        _recprogramInfo[_u8CurRecProgramIdx].bRecordAppend = TRUE;
        printf("[DemoPVR] Record Append! \n");
    }
    else
    {
        _recprogramInfo[_u8CurRecProgramIdx].bRecordAppend = FALSE;
    }
    if(!MApi_PVR_SetMetaData(&_recprogramInfo[_u8CurRecProgramIdx],enRecIdx,0,0,0,0))
    {
        printf("Can't set program info!\n");
        return FALSE;
    }

    // For merge stream, pass source id to MW
    if(!MApi_PVR_SetRecordSourceID(_u8CurRecProgramIdx, (MS_U8)_eCurDMXFltSourceID[_u8CurRecProgramIdx]))
    {
        DemoPvr_Errorprint("[ERROR] Can't set source id!\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_PVR_ChangeProgramPIDs(MS_BOOL bTimeshift, EN_DEMO_DMX_FLOW eDmxPvrInput, MS_U32 u32PCRPid, MS_U32 u32VideoPid, MS_U32 u32AudioPid, MS_U32 u32ACodec, MS_U32 u32VCodec)
{
    MS_U16 u16PmtPid=INVALID_PID_ID;
    MS_U16 u16LCN=0xFFFF;
    MS_U8 enRecIdx=_u8CurRecProgramIdx;
    MS_U32 u32TempACodec;
    MS_U32 u32TempVCodec;

    // Get information of new program (PMT pid , LCN)
    if(bTimeshift)
    {
        Ex_PVRPlaybackPath ePlaybackidx = (Ex_PVRPlaybackPath)_Demo_PVR_Timeshift_GetPlaybackIdx(enRecIdx);
        if (ePlaybackidx == PVR_PATH_INVALID)
        {
            DemoPvr_print("[%s][%d]Error AVPATH(%d, %d, %d)!\n", __FUNCTION__, __LINE__, ePlaybackidx, _eCurPlaybackProgramPath, enRecIdx);
            return FALSE;
        }

        if(_Demo_Dmx_GetCurProgram(eDmxPvrInput,u32PCRPid,u32VideoPid,u32AudioPid,&u16PmtPid,&u16LCN) == FALSE)
        {
            DemoPvr_print("fail to Demo_Dmx_GetCurProgram.\n");
            return FALSE;
        }

        u32TempVCodec = (MS_U32)_Demo_PVR_Get_PromInfo_VDEC_Fmt_FromVDECDDI(u32VCodec);
        u32TempACodec = (MS_U32)_Demo_PVR_Get_PromInfo_ADEC_Fmt_FromADECDDI(u32ACodec);
        if(FALSE==_Demo_PVR_SetProgramInfo(&_livePromInfo[ePlaybackidx],u32PCRPid, u32VideoPid, u32AudioPid, u32TempVCodec, u32TempACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN,TRUE))
        {
            DemoPvr_print("Can't store program info for live(%d)!\n",ePlaybackidx);
            _livePromInfo[ePlaybackidx].bLive = FALSE;
            return FALSE;
        }
        _livePromInfo[ePlaybackidx].bLive = TRUE;
    }
    else
    {
        if(_Demo_Dmx_GetCurProgram(eDmxPvrInput,u32PCRPid,u32VideoPid,u32AudioPid,&u16PmtPid,&u16LCN) == FALSE)
        {
            DemoPvr_print("fail to Demo_Dmx_GetCurProgram.\n");
            return FALSE;
        }

        u32TempVCodec = (MS_U32)_Demo_PVR_Get_PromInfo_VDEC_Fmt_FromPVRDDI(u32VCodec);
        u32TempACodec = (MS_U32)_Demo_PVR_Get_PromInfo_ADEC_Fmt_FromPVRDDI(u32ACodec);
    }

    // Store new program information to _recprogramInfo[]
    if(FALSE==_Demo_PVR_SetProgramInfo(&_recprogramInfo[enRecIdx],u32PCRPid, u32VideoPid, u32AudioPid, u32TempVCodec, u32TempACodec,(MS_U32)u16PmtPid,(MS_U32)u16LCN,TRUE))
    {
        DemoPvr_print("Can't set new program info for record(%d)!\n",enRecIdx);
        return FALSE;
    }

    DemoPvr_print(" _u8CurProgramIdx=%d   fileName=",enRecIdx);
    wprintf((MS_U16*)_recprogramInfo[enRecIdx].FileName, PVR_DBG, TRUE);

    // For merge stream, pass source id to MW
    if(!MApi_PVR_SetRecordSourceID(_u8CurRecProgramIdx, (MS_U8)_eCurDMXFltSourceID[_u8CurRecProgramIdx]))
    {
        DemoPvr_Errorprint("[ERROR] Can't set source id!\n");
        return FALSE;
    }

    // Update PIDs of new program (real) & record neccessary information of new program (*.pmt)
    if (!MApi_PVR_RecordProgramChange(&_u8hRecord[_u8CurRecProgramIdx],_u8CurRecProgramIdx,&_recprogramInfo[_u8CurRecProgramIdx]))
    {
        DemoPvr_print("MApi_PVR_RecordProgramChange Failed!\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_PVR_RecordStart(MS_BOOL bTimeshift, EN_DEMO_DMX_FLOW_INPUT eCurDMXInputSrc, EN_DEMO_DMX_FLOW eCurDMXFlowSet, MS_U32 u32FreeSizeInMB,MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char* fileName)
{
    //DemoPvr_print("(%ld,%ld,%ld,%ld,%ld)\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);
    if((*VideoPid==0)&&(*AudioPid==0)&&(*PCRPid==0))
    {
        DemoPvr_Errorprint("Invalid input!\n");
        return FALSE;
    }

    // [01] Check if  available ==============================================//
    DEMO_PVR_CHECK_INIT()
    if(_u8hRecord[_u8CurRecProgramIdx]!=APIPVR_INVALID_HANDLER)
    {
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        if(bTimeshift == FALSE)
#endif
        {
            DemoPvr_Errorprint("Already record in idx:%d!\n",_u8CurRecProgramIdx);     //Normal Record Case
            return FALSE;
        }
    }
    if(fileName==NULL)
    {
        DemoPvr_Errorprint( "Invalid fileName!\n");
        return FALSE;
    }

    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        DemoPvr_Errorprint("USB is not ready!\n");
        return FALSE;
    }


    // For merge stream
    // source id of current record idx must be equal to source id of current DTV in timeshift record,
    // if it's not, return false directly.

    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;
    ST_AV_DMX_INFO stAVDMXInfo;
    Demo_PVR_Check_AVPath(eDevice,_eCurPlaybackProgramPath,PVR_PATH_MAIN,E_AV_DEVICE_MAIN,PVR_PATH_SUB,E_AV_DEVICE_SUB);
    Demo_AV_GetAVInfo(&eDevice,E_AV_GetCmd_DMXFlowInfo,&stAVDMXInfo);

    if(bTimeshift && _eCurDMXFltSourceID[_u8CurRecProgramIdx] != stAVDMXInfo.eDmxFltSourceID)
    {
        DemoPvr_Errorprint("Current DTV source id != Record source id\n");
        return FALSE;
    }

    // [02] Store Current Program info ==============================================//
    MS_U16 *pu16FileName = NULL;

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8 *)fileName, pu16FileName, strlen(fileName));
    if(!_Demo_PVR_StoreProgramInfo(eCurDMXFlowSet, bTimeshift,*PCRPid, *VideoPid, *AudioPid, *pu32ACodec, *pu32VCodec , (char*)pu16FileName))
    {
        DemoPvr_Errorprint("Fail to store program info!\n");
        _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
        return FALSE;
    }
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);

    // [03] DMX Flowset and PID open ==============================================//
    if(_u8CurRecProgramIdx < _u8TotalRecorder)
    {
        DemoPvr_print("@@@ _u8CurRecProgramIdx:[%u] eCurDMXInputSrc:[%u], _u32CurDMXFlow:[%u]\n",_u8CurRecProgramIdx,eCurDMXInputSrc,eCurDMXFlowSet);
    }
    else
    {
        DemoPvr_Errorprint(" \" over max program !!!! \"\n");
        return FALSE;
    }

    return _Demo_PVR_RecStart(bTimeshift, u32FreeSizeInMB);
}

static MS_BOOL _Demo_PVR_GetFileInfo(char *FileName, PVRProgramInfo_t *promInfo)
{
    MS_U32 u32VideoPid=INVALID_PID_ID;
    MS_U32 u32AudioPid=INVALID_PID_ID;
    MS_U32 u32PCRPid=INVALID_PID_ID;
    char AudioFmtStr[][21]={"NOT_VALID","MPEG","AC3","AC3P","AAC"};
    char VideoFmtStr[][21]={"NOT_VALID","MPEG2","H264","AVS","HEVC"};

    DEMO_PVR_CHECK_INIT();

    if(promInfo==NULL)
    {
        printf("Invalid promInfo\n");
        return FALSE;
    }

    DemoPvr_print("memset program info\n");
    _Demo_PVR_ResetProgramInfo(promInfo,FALSE);

    if(MApi_PVR_GetProgramInfo(FileName,promInfo)==FALSE)
    {
        printf("GetFileInformation fail!\n");
        return FALSE;
    }
    u32VideoPid=_Demo_PVR_FindPidinFilter(promInfo->Filters, DMX_FILTER_TYPE_VIDEO);
    u32AudioPid=promInfo->u16AudioPid;
    u32PCRPid=_Demo_PVR_FindPidinFilter(promInfo->Filters, DMX_FILTER_TYPE_PCR);

    printf("video pid = 0x%"DTC_MS_U32_x"\n",u32VideoPid);
    printf("audio pid = 0x%"DTC_MS_U32_x"\n",u32AudioPid);
    printf("pcr   pid = 0x%"DTC_MS_U32_x"\n",u32PCRPid);
    printf("VideoFmt  = %s, VCodec = %"DTC_MS_U32_x"\n",VideoFmtStr[promInfo->enVCodec], (MS_U32)promInfo->enVCodec);
    printf("AudioFmt  = %s, ACodec = %"DTC_MS_U32_x"\n",AudioFmtStr[promInfo->u32ACodec], promInfo->u32ACodec);
    printf("\tduration  = %"DTC_MS_U32_u" sec\n",promInfo->u32Duration);
    printf("\tFile Len  = 0x%"DTC_MS_U64_x"  Bytes\n",promInfo->u64FileLength);
    printf("\tLast Time = %"DTC_MS_U32_u" sec\n",promInfo->u32LastPlayPositionInSec);
    printf("\tAge       = %u \n",promInfo->u8Age);
    printf("\tLock      = %u \n",promInfo->bLocked); // lock by user and it cannot be deleted even if disk is full
    printf("\n***************************\n\n");

    return TRUE;
}

static MS_U8 _Demo_PVR_Timeshift_GetPlaybackIdx(MS_U8 u8RecordIdx)
{
    MS_U8 playbackidx;
    for (playbackidx = 0;playbackidx < MAX_PLAYBACK_NUM;playbackidx++)
    {
        if (_u8TimeshiftRecordIdx[playbackidx] == u8RecordIdx)
        {
            DemoPvr_print("[%s] Get timeshift playbackidx:%u  from u8RecordIdx:%u\n",__FUNCTION__,playbackidx,u8RecordIdx);
            return playbackidx;
        }
    }
    //printf("[%s][%d]Error!!! No match playback index\n",__FUNCTION__, __LINE__);
    return PVR_PATH_INVALID;
}

static MS_BOOL _Demo_PVR_IsTimeshiftRecord(MS_U8 u8RecordIdx)
{
    Ex_PVRPlaybackPath ePlaybackidx = (Ex_PVRPlaybackPath)_Demo_PVR_Timeshift_GetPlaybackIdx(u8RecordIdx);
    if (ePlaybackidx != PVR_PATH_INVALID)
    {
        return TRUE;
    }
    return FALSE;
}

static MS_U8 _Demo_PVR_RecHandler2Idx(MS_U8 u8hRecord)
{
    MS_U8 recordidx;
    for (recordidx = 0;recordidx < _u8TotalRecorder;recordidx++)
    {
        if (_u8hRecord[recordidx] == u8hRecord)
        {
            DemoPvr_print("[%s] Get record idx:%u\n",__FUNCTION__,recordidx);
            return recordidx;
         }
    }
    printf("[%s][%d]Error!!! No match record handler\n",__FUNCTION__, __LINE__);
    return 0xFF;
}

static MS_U8 _Demo_PVR_PlayHandler2Idx(MS_U8 u8hPlayback)
{
    MS_U8 playbackidx;
    for (playbackidx = 0;playbackidx < MAX_PLAYBACK_NUM;playbackidx++)
    {
        if (_u8hPlayback[playbackidx] == u8hPlayback)
        {
            DemoPvr_print("[%s] Get playback idx:%u\n",__FUNCTION__,playbackidx);
            return playbackidx;
         }
    }
    printf("[%s][%d]Error!!! No match playback handler\n",__FUNCTION__, __LINE__);

    return PVR_PATH_INVALID;
}

void* _Demo_PVR_GetVidStreamID(Ex_PVRPlaybackPath *peAVPATH)
{
    void *vStreamID = (void *)&_NullVidStreamID;

    if(*peAVPATH == PVR_PATH_MAIN)
    {
        EN_VDEC_Device eDevice = E_VDEC_DEVICE_MAIN;
        vStreamID = Demo_VDEC_GetStreamID(&eDevice);
    }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    else if (*peAVPATH == PVR_PATH_SUB)
    {
        EN_VDEC_Device eDevice = E_VDEC_DEVICE_SUB;
        vStreamID = Demo_VDEC_GetStreamID(&eDevice);
    }
#endif
    else
    {
        printf("[%s][%d]Wrong AV PATH(%d)\n",__FUNCTION__, __LINE__,*peAVPATH);
        return NULL;
    }

    if(vStreamID == NULL)
    {
        DemoPvr_Errorprint("Get PlaybackPath %d StreamID return NULL\n", *peAVPATH);
        return NULL;
    }

    memcpy(&_VidStreamID[*peAVPATH],vStreamID,sizeof(VDEC_StreamId));

    return (void *)&_VidStreamID[*peAVPATH];
}

static void* _Demo_PVR_GetAudStreamID(Ex_PVRPlaybackPath *peAVPATH)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        ST_AUDIO_DEC_INFO stAudioInfo;
        MS_U32 u32ADECDevIdx = 0;

    if (*peAVPATH == PVR_PATH_MAIN)
    {
        u32ADECDevIdx = 0;
    }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    else if (*peAVPATH == PVR_PATH_SUB)
    {
        u32ADECDevIdx = 1;
    }
#endif
    else
    {
        printf("[%s][%d] Wrong AV PATH(%d) \n",__FUNCTION__,__LINE__,*peAVPATH);
        return (void *)&_NullAudStreamID;
    }

        Demo_Audio_GetDecInfo(u32ADECDevIdx, &stAudioInfo);
        memcpy(&_AudStreamID[*peAVPATH],&stAudioInfo.eDecID,sizeof(AUDIO_DEC_ID));

        return (void *)&_AudStreamID[*peAVPATH];
    }
    else
    {
        return (void *)&_NullAudStreamID;
    }
}

#if (DEMO_PVR_LINUX_MOUNT_NOTIFIER_TEST == 1)
static MS_BOOL _Demo_PVR_GetMountPath(char *moutPath)
{
    if( (MApi_PVR_GetMouthPath())&&(_bPvrInit))
    {
        _U16StringToU8String((MS_U16*)MApi_PVR_GetMouthPath(), (MS_U8*)moutPath, wcslen((MS_U16*)MApi_PVR_GetMouthPath()));
        printf("Get Mount PATH => [%s]\n",moutPath);
        return TRUE;
    }
    else
    {
         printf("Get Mount PATH failed!!!\n");
         return FALSE;
    }
}

static MS_BOOL _Demo_PVR_Device_Path_Check(char* device_path, const char* pSDA1)
{
    strncpy(device_path,"/dev", 4);
    device_path[4] = '\0';

    if((strlen(device_path) + strlen(pSDA1)) > DIR_PATH_SIZE)
    {
        return FALSE;
    }

    strncat(device_path,(pSDA1+9), strlen(pSDA1) - 9);
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// PVR USB mount notifier Task, which handle all the Signal from Linux kenel.
/// The Mount notifier task parser the signal and mount/unmount the USB device to PVR mount path.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _Demo_PVR_MountNotifier_Task(MS_U32 argc, void *argv)
{
    const MS_U16 buffersize = 16 * 1024;
    struct      sockaddr_nl nls;
    struct      pollfd pfd;
    char buf_cache[PVR_MOUNT_BUFFER_SIZE] = {0};
    MS_U16 ret;
    char dir_path[DIR_PATH_SIZE]={0};
    char device_path[DIR_PATH_SIZE]={0};
    ssize_t byte_recv=0;

    // string token
    const char * pADD = NULL;
    const char * pREMOVE = NULL;
    const char * pSDA1 = NULL;

    MS_U16 u16TimeoutInMS = 10;


    // Initial socket to receive usb event
    memset(&nls, 0, sizeof(nls));
    nls.nl_family = AF_NETLINK;
    nls.nl_pid = getpid();
    nls.nl_groups = -1;
    pfd.events = POLLIN;
    pfd.fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if(pfd.fd == -1)
    {
        printf("Not root\n");
        return;
    }

    setsockopt(pfd.fd, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    struct timeval tv;
    tv.tv_sec = 1;  /* 1 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(pfd.fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
    if(bind(pfd.fd, ((struct sockaddr*)&nls), sizeof(nls)))
    {
        close(pfd.fd);
        printf("bind failed\n");
        return;
    }

    //get USB mount path
    if(!_Demo_PVR_GetMountPath(dir_path))
    {
        close(pfd.fd);
        printf("Not initialize PVR!\n");
        return;
    }

    printf("!!! Create Moniter PVR USB mount/umont event Thread !!!\n");
    printf("PVR mount path: ");


    while(_gbPvrMntExit != TRUE)
    {
        char *err_str = strerror(errno);

        memset(buf_cache, 0 ,sizeof(buf_cache));
        byte_recv = recv(pfd.fd,buf_cache, sizeof(buf_cache), 0);
        if((byte_recv > 0) && (byte_recv < sizeof(buf_cache)))
        {
           *(buf_cache + byte_recv) = '\0';
        }
        else
        {
            continue;
        }

        pADD = strstr(buf_cache, "add@");
        if(pADD != NULL)
        {
            pSDA1 = strstr(buf_cache, "block/");
            if((pSDA1 != NULL) &&(pSDA1[9]=='/'))
            {


                if(!_Demo_PVR_Device_Path_Check(device_path, (pSDA1)))
                {
                    close(pfd.fd);
                    printf("Get Mount PATH failed!!!\n");
                    return;
                }
                printf("USB plugin : [%s] path:[%s] dir:[%s]\n",(pSDA1+9),device_path,dir_path);

                // It usually takes 10ms~20ms for /dev/sdx to get ready that we can mount successfully
                // We pick 10 ms as initial value and 100 ms as timeout according to experience
                while(u16TimeoutInMS < 100)
                {
                    MsOS_DelayTask(u16TimeoutInMS);
                    printf("Wait %d ms.....\n", u16TimeoutInMS);
                    u16TimeoutInMS += 10;

                    ret = MsFS_Mount(device_path,dir_path,"ntfs3g", 0, 0);
                    if(ret != 0)
                    {
                        printf("error [%d] : %s\n", errno, err_str);
                        ret = MsFS_Mount(device_path,dir_path,"vfat",0,0);
                    }
                    else
                    {
                        printf("mount %s --> %s success!! (NTFS)\n",device_path,dir_path);
                        break;
                    }

                    if(ret != 0)
                    {
                        printf("error [%d] : %s\n", errno, err_str);
                    }
                    else
                    {
                        printf("mount %s --> %s success!! (FAT)\n",device_path,dir_path);
                        break;
                    }
                }
            }
        }

        pREMOVE = strstr(buf_cache, "remove@");
        if(pREMOVE != NULL)
        {
            pSDA1 = strstr(buf_cache, "block/");
            if((pSDA1 != NULL) &&(pSDA1[9]=='/'))
            {
                if(!_Demo_PVR_Device_Path_Check(device_path, (pSDA1)))
                {
                    close(pfd.fd);
                    printf("Get Mount PATH failed!!!\n");
                    return;
                }
                printf("USB plugout : [%s] path:[%s] dir:[%s]\n",(pSDA1+9),device_path,dir_path);
                Demo_PVR_USB_UnplugEvent(dir_path);
            }
        }
    }
    close(pfd.fd);
    MsOS_SetEvent(_gs32PvrMntGrp, PVR_MOUNT_DESTROY_PROC_EXIT);
}

static MS_BOOL _Demo_PVR_MountNotifier_Create(void)
{
    MS_S32 s32NonCachedPoolID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);

    if(PVR_MOUNTNOTIFIER_TASK_STACK)
    {
        _pvrMNTTask.pStack = MsOS_AllocateMemory(PVR_MOUNTNOTIFIER_TASK_STACK,s32NonCachedPoolID);
        if (_pvrMNTTask.pStack == NULL)
        {
            printf("Allocate PVR MountNotifier TaskStack Failed......\n");
            return FALSE;
        }
    }

    _pvrMNTTask.iId = MsOS_CreateTask((TaskEntry)_Demo_PVR_MountNotifier_Task,
                                 0,
                                 _pvrMNTTask.ePriority, // E_TASK_PRI_MEDIUM,
                                 TRUE,
                                 _pvrMNTTask.pStack,
                                 _pvrMNTTask.u32StackSize,
                                 _pvrMNTTask.szName);

    if (_pvrMNTTask.iId < 0)
    {
        printf("[%s][%d] Create PVR MountNotifier Task Failed \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return TRUE;
}
#endif

static EN_PVR_MEM_MALLOC _appDemo_PVR_MemAlloc(void)
{
    if(!_Demo_PVR_AllocateMem(PVR_ID_DOWNLOAD))
        return E_PVR_MEM_MALLOC_FAIL;

    if(!_Demo_PVR_AllocateMem(PVR_ID_UPLOAD))
        return E_PVR_MEM_MALLOC_FAIL;

#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
    if(_enRecordType==EN_APIPVR_RECORD_TYPE_SWPVR)
        if(!_Demo_PVR_AllocateMem(PVR_ID_SWBUFFER))
        {
            printf("NOT enough memory resource for SWPVR record mode support!\n");
            return E_PVR_MEM_MALLOC_FAIL;
        }
#endif
    return E_PVR_MEM_MALLOC_SUCCESS;
}

static MS_BOOL _appDemo_PVR_MemFree(void)
{
    _Demo_PVR_FreeMem(PVR_ID_DOWNLOAD);
    _Demo_PVR_FreeMem(PVR_ID_UPLOAD);
#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
    _Demo_PVR_FreeMem(PVR_ID_SWBUFFER);
#endif
    return TRUE;
}

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief The sample code to initialize pvr.
/// @param[in] Path The path of the file
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note It spend most time to create a file at first time. Try to resolve the problem by create a file on Demo_PVR_Init.
/// Command: N/A
//------------------------------------------------------------------------------
static MS_BOOL _Demo_PVR_WriteFirstData(char *Path)
{
    MS_U32 u32_i;
    FILE *s32_fd;
    MS_U32 TEST_BLOCK_SIZE = 256;
    MS_BOOL usb_arr_init = FALSE;
    MS_U8 usb_arr_w[TEST_BLOCK_SIZE+1];

    if(usb_arr_init == FALSE)
    {
        for(u32_i=0; u32_i<TEST_BLOCK_SIZE; u32_i++)
        {
            usb_arr_w[u32_i] = (MS_U8)u32_i;
        }
            usb_arr_w[TEST_BLOCK_SIZE] = '\0';
        usb_arr_init = TRUE;
    }

    MS_U16 *pu16Path = NULL;

    pu16Path = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8 *)Path, pu16Path, strlen(Path));

    s32_fd = MsFS_Fopen((char*)pu16Path, "wb");

    if(s32_fd == 0)
    {
        printf("[USB Test] create file faild\n");
        _Demo_PVR_MEMFREE_FUNC((void**)&pu16Path);
        return FALSE;
    }
    MS_U32 u32WriteSize=MsFS_Fwrite((void *)usb_arr_w, 1,TEST_BLOCK_SIZE,s32_fd);
    MsFS_Fflush(s32_fd);
    printf("[%s][%d]Write first Data success!u32WriteSize(%"DTC_MS_U32_u")\n",__FUNCTION__,__LINE__,u32WriteSize);
    MsFS_Fclose(s32_fd);
    MsFS_Unlink((char*)pu16Path);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16Path);
    return TRUE;
}
#endif

static APIPVR_PLAYBACK_SPEED _Demo_PVR_GetSlowForward(APIPVR_PLAYBACK_SPEED enPlaybackSpeed)
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
        default:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            printf("[%s][%d] Undefined speed condition for SlowForward!!!, we set the speed as default (%d)\n",__FUNCTION__,__LINE__,enSpeed);
            break;
    }

    return enSpeed;
}
static APIPVR_PLAYBACK_SPEED _Demo_PVR_GetFastForward(APIPVR_PLAYBACK_SPEED enPlaybackSpeed)
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
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            break;
        default :
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            printf("[%s][%d] Undefined speed condition for FastForward!!!, we set the speed as default (%d)\n",__FUNCTION__,__LINE__,enSpeed);
            break;

    }


    return enSpeed;
}
static APIPVR_PLAYBACK_SPEED _Demo_PVR_GetFastBackward(APIPVR_PLAYBACK_SPEED enPlaybackSpeed)
{
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;
    switch(enPlaybackSpeed)
    {
        case EN_APIPVR_PLAYBACK_SPEED_32XFB:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
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
        default:
            enSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
            printf("[%s][%d] Undefined speed condition for FastBackward!!!, we set the speed as default (%d)\n",__FUNCTION__,__LINE__,enSpeed);
            break;

    }


    return enSpeed;
}


static MS_BOOL _Demo_PVR_PlaybackSetSpeed(DEMO_PVR_PLAYBACK_STATE enPlaybackSpeedType, MS_U32 u32Positive, MS_S32 s32Speed)
{
    MS_BOOL bRet = FALSE;

    DemoPvr_print("=========  _Demo_PVR_PlaybackSetSpeed =========\n");
    DEMO_PVR_CHECK_INIT();
    APIPVR_PLAYBACK_SPEED enPlaybackSpeed=MApi_PVR_PlaybackGetSpeed(_u8hPlayback[_eCurPlaybackProgramPath]);

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath])!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }

    switch(enPlaybackSpeedType)
    {
        case EN_DEMO_PVR_PLAYBACK_PAUSE:
            bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],EN_APIPVR_PLAYBACK_SPEED_0X);
            break;
        case EN_DEMO_PVR_PLAYBACK_RESUME:
            bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],EN_APIPVR_PLAYBACK_SPEED_1X);
            break;
        case EN_DEMO_PVR_PLAYBACK_SLOWFORWARD:
            enPlaybackSpeed=_Demo_PVR_GetSlowForward(enPlaybackSpeed);
            bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],enPlaybackSpeed);
            if(EN_APIPVR_PLAYBACK_SPEED_1X != enPlaybackSpeed)
            {
                printf("%d xSF Play \n",enPlaybackSpeed);
            }
            else
            {
                printf("Normal Play \n");
            }
            break;
        case EN_DEMO_PVR_PLAYBACK_FASTFORWARD:
            enPlaybackSpeed=_Demo_PVR_GetFastForward(enPlaybackSpeed);
            bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],enPlaybackSpeed);
            if(EN_APIPVR_PLAYBACK_SPEED_1X != enPlaybackSpeed)
            {
                printf("%d xFF Play \n",enPlaybackSpeed);
            }
            else
            {
                printf("Normal Play \n");
            }
            break;
        case EN_DEMO_PVR_PLAYBACK_FASTBACKWARD:
            enPlaybackSpeed=_Demo_PVR_GetFastBackward(enPlaybackSpeed);
            bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],enPlaybackSpeed);
            if(EN_APIPVR_PLAYBACK_SPEED_1X != enPlaybackSpeed)
            {
                printf("%d xFB Play \n",enPlaybackSpeed);
            }
            else
            {
                printf("Normal Play \n");
            }
            break;
        case EN_DEMO_PVR_PLAYBACK_SETSPEED:
            if(u32Positive == 0)
            {
                s32Speed *= -1;
            }
            bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],s32Speed);
            break;
        default:
            printf("Invalid Speed State %d!\n", enPlaybackSpeedType);
            break;

    }
    return bRet;
}

MS_BOOL _Demo_PVR_LiveInfo_Initialize(MS_U32 u32PlaybackPath)
{
    if(u32PlaybackPath >= MAX_PLAYBACK_NUM)
    {
        printf("Invalid playback path(%"DTC_MS_U32_u")\n",u32PlaybackPath);
    }

    _stLiveInfo[u32PlaybackPath].bInited = FALSE;

    _stLiveInfo[u32PlaybackPath].stVideoParams.u8Filter = INVALID_FILTER_ID;
    _stLiveInfo[u32PlaybackPath].stVideoParams.ePCREngID = E_PCR_ENG_INVALID;
    _stLiveInfo[u32PlaybackPath].stVideoParams.u16PID = INVALID_PID_ID;
    _stLiveInfo[u32PlaybackPath].stVideoParams.eFIFO_ID = E_DMX_FLT_TYPE_INVALID;
    _stLiveInfo[u32PlaybackPath].stVideoParams.stFilterType.eEngType = INVALID_FILEIN_TYPE;
    _stLiveInfo[u32PlaybackPath].stVideoParams.stFilterType.u8EngID = INVALID_FILEIN_ID;
    _stLiveInfo[u32PlaybackPath].stVideoParams.stFilterType.eEngFormat = E_AV_FileIn_Eng_TS;
    memset(&_stLiveInfo[u32PlaybackPath].stVideoParams.stVDECSteamID,0, sizeof(VDEC_StreamId));
    _stLiveInfo[u32PlaybackPath].stVideoParams.eVideoCodec = E_VDEC_DDI_CODEC_TYPE_MPEG2;
    _stLiveInfo[u32PlaybackPath].stVideoParams.eAudioCodec = DEMO_AUDIO_CODEC_MPEG;

    memcpy(&_stLiveInfo[u32PlaybackPath].stAudioParams, &_stLiveInfo[u32PlaybackPath].stVideoParams, sizeof(ST_AV_Params));
    memcpy(&_stLiveInfo[u32PlaybackPath].stAudioADParams, &_stLiveInfo[u32PlaybackPath].stVideoParams, sizeof(ST_AV_Params));
    memcpy(&_stLiveInfo[u32PlaybackPath].stPCRParams, &_stLiveInfo[u32PlaybackPath].stVideoParams, sizeof(ST_AV_Params));
    memset(&_stLiveInfo[u32PlaybackPath].stDMXflowParams, 0, sizeof(ST_AV_DMX_INFO));

    _stLiveInfo[u32PlaybackPath].eAVSYNC_Mode = E_AV_SYNC_PCR_MODE;
    memset(&_stLiveInfo[u32PlaybackPath].stShowFrameInfo, 0, sizeof(ST_AV_ShowFrameInfo));

    _stLiveInfo[u32PlaybackPath].bEnableAVSYNC_Task = FALSE;
    _stLiveInfo[u32PlaybackPath].eAVSYNC_Task_State = E_AV_AVSYNC_STATE_RESET;

    _stLiveInfo[u32PlaybackPath].bInitializeOrNot = FALSE;
    _stLiveInfo[u32PlaybackPath].eAVInitState = E_AV_AVINIT_STATE_RESET;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
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
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Init \n
///
/// Sample Command:PVR_Init 0 /mnt/tmp
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Init(MS_U32 *u32FSType, char *pMountPath)
{
    DemoPvr_print("========= Demo_PVR_Init =========\n");
    PVR_TSPInfo_t pvrTspInfo = {0};
    int i=0;
    PVR_AV_INFO stInitFileAVInfo = {FALSE,INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,INVALID_PID_ID,INVALID_PID_ID,{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,DEMO_AUDIO_CODEC_MPEG,E_DMX_FLT_TYPE_INVALID,E_DMX_FLT_TYPE_INVALID,E_DMX_FLOW_INVALID};
    EN_PVR_MEM_MALLOC EN_MEM_MALLOC;
    MS_U8 PlaybackPathNum = MAX_PLAYBACK_NUM;
    MS_U16 *pu16MountPath = NULL;


    // [00] check mount path  ==============================================//
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

    if(_bPvrInit)
    {
        printf( "Already init PVR!\n");
        return FALSE;
    }
    DemoPvr_print("========= PVR Init =========\n");

    MsFS_Init(TRUE);
    pu16MountPath = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8 *)pMountPath, pu16MountPath, strlen(pMountPath));
    if(! _Demo_PVR_CheckUSB((char*)pu16MountPath))
    {
        printf("No USB Disk Deteded!\n");
        _Demo_PVR_MEMFREE_FUNC((void**)&pu16MountPath);
        return FALSE;
    }

    DemoPvr_print("enFSType=%"DTC_MS_U32_u" pMountPath=%s\n",*u32FSType,pMountPath);

    // [01] Init PVR  ==============================================//
#if (DEMO_PVR_SUPPORT_SWPVR_TEST != 1)
    _Demo_PVR_SetRecordType(0);//single record
#endif

    if(!MApi_PVR_Init((char*)pu16MountPath,(EN_APIPVR_FILE_SYSTEM_TYPE)*u32FSType,(APIPVR_TUNER_NUM)_u8TotalRecorder,pvrTspInfo))
    {
       printf("Fail to Init PVR!\n");
       return FALSE;
    }
    if(!MApi_PVR_SetPlaybackPathNum(PlaybackPathNum))
    {
        printf("Fail to Set PlaybackPathNum!\n");
        return FALSE;
    }
    _bPvrInit = TRUE;
    // [02] Mem Layout: The base address/length of physical memory in MApi_PVR_Mmap ==============//
    EN_MEM_MALLOC = _appDemo_PVR_MemAlloc();
    if (EN_MEM_MALLOC == E_PVR_MEM_MALLOC_FAIL)
    {
        printf("PVR_MEM_MALLOC_FAIL!\n");
        Demo_PVR_Destroy();
        return FALSE;
    }
    DemoPvr_print("\n");

    // [03] Callback Function for PVR ==============//
    if(b2ndEncryptionEnabled)
    {
        //KL callback function only used for DMA ENCRYPTDECRYPT in non-callback mode,
        //In DMA ENCRYPTDECRYPT in callback mode, _Demo_PVR_KLADDER_FUNC is called in DMA ENCRYPTDECRYPT callback function.
        #if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
        PVRFuncInfo_t pvrFunc={};
        pvrFunc.DscmbConnect = _Demo_PVR_RECORD_DSCMB_CONNECT_FUNC;
        pvrFunc.DscmbDisConnect = _Demo_PVR_RECORD_DSCMB_DISCONNECT_FUNC;
        MApi_PVR_SetFuncInfo(pvrFunc);
        #endif//(DEMO_PVR_DSCMB_CALLBACK_MODE)
    }

    // [04] Recorder and Player type ==============//
    MApi_PVR_SetRecordType(_enRecordType);
    MApi_PVR_SetPlaybackType(_enPlayType);

#if (DEMO_PVR_SUPPORT_PLAYBACK_AUTORESUME_TEST == 1)
    MApi_PVR_PlaybackSetFileEndAutoResume(TRUE);
#else
    MApi_PVR_PlaybackSetFileEndAutoResume(FALSE);
#endif

    PVR_CfgParam stPvrCfgParam;

    stPvrCfgParam.bSet = TRUE;
    stPvrCfgParam.eCfgCmd = EN_APIPVR_CFG_AUTO_PB_PROGRAM_CHG;
    stPvrCfgParam.bAutoPlaybackProgramChg = TRUE;
    MApi_PVR_Configure(&stPvrCfgParam);

    // [05] Create Message queue to receive info ==============//
    MApi_PVR_SetMsgWaitMs(0);
    _s32QueueID = MApi_PVR_GetMessageQueueID();
    if(_pvrEventTask.iId==INVALID_TASK_ID)
    {
        _gbPvrEventExit = FALSE;
        if (INVALID_TASK_ID ==(_pvrEventTask.iId=MsOS_CreateTask(_Demo_PVR_Task,0,_pvrEventTask.ePriority,TRUE,_pvrEventTask.pStack,_pvrEventTask.u32StackSize,_pvrEventTask.szName)))
        {
            printf("Create Task fail\n");
            _appDemo_PVR_MemFree();
            Demo_PVR_Destroy();
            return FALSE;
        }

    }

    // [06] Init PVR relative info ==============//

    //*Init relative info*//
    _u8CurRecProgramIdx=0;
    _eCurDMXInputSrc=E_DMX_FLOW_INPUT_DEMOD0;
    _bUplugMsg=FALSE;
    _moutPath[0]='\0';

    _eCurPlaybackProgramPath=0;
    _u8TotalPlaybackProgram=0;
    for(i=0;i<MAX_PLAYBACK_NUM;i++)
    {
         _u8hPlayback[i]=APIPVR_INVALID_HANDLER;
         _bScreenFrozen[i]=TRUE;
         _u8TimeshiftRecordIdx[i]= INVALID_RECORD_INDEX;
         _u8TimeshiftPlaybackIdx[i] = PVR_PATH_INVALID;
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
         _bTimeshiftSeamless[i]=FALSE;
         _bBackgroundRecord[i]=FALSE;
         memset(&_u8BackPcrDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackPcrDmxId[i]));
         memset(&_u8BackVidDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackVidDmxId[i]));
         memset(&_u8BackAudDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackAudDmxId[i]));
         memset(&_u8BackTtxDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackTtxDmxId[i]));
         memset(&_u8BackPcrDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackPcrDmxId[i]));
         memset(&_u8BackCcDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackCcDmxId[i]));
         memset(&_u8BackEBUSubtDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackEBUSubtDmxId[i]));
         memset(&_u8BackDVBSubtDmxId[i],INVALID_FILTER_ID,sizeof(_u8BackDVBSubtDmxId[i]));
#endif
         memset(&_plyprogramInfo[i],0,sizeof(PVRProgramInfo_t));
         memset(&_livePromInfo[i],0,sizeof(PVRProgramInfo_t));
         memcpy(&stFileAVInfo[i],&stInitFileAVInfo,sizeof(PVR_AV_INFO));
         memcpy(&_AudStreamID[i],&_NullAudStreamID,sizeof(AUDIO_DEC_ID));
         memcpy(&_VidStreamID[i],&_NullVidStreamID,sizeof(VDEC_StreamId));
        if(b2ndEncryptionEnabled)
        {
            memset(&stPVRLiveDSCMBInfo[i],0,sizeof(PVR_DSCMB_INFO));
        }
        _Demo_PVR_LiveInfo_Initialize(i);
    }
    // [CHECK] the var init problem
    memset(&_u8PcrDmxId,INVALID_FILTER_ID,sizeof(_u8PcrDmxId));
    memset(&_u8VidDmxId,INVALID_FILTER_ID,sizeof(_u8VidDmxId));
    memset(&_u8AudDmxId,INVALID_FILTER_ID,sizeof(_u8AudDmxId));
    memset(&_u8TtxDmxId,INVALID_FILTER_ID,sizeof(_u8TtxDmxId));
    memset(&_u8CcDmxId,INVALID_FILTER_ID,sizeof(_u8CcDmxId));
    memset(&_u8EBUSubtDmxId,INVALID_FILTER_ID,sizeof(_u8EBUSubtDmxId));
    memset(&_u8DVBSubtDmxId,INVALID_FILTER_ID,sizeof(_u8DVBSubtDmxId));
    for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
    {
         _u8hRecord[i]=APIPVR_INVALID_HANDLER;
         _eCurDMXFlowSet[i]=E_DMX_FLOW_LIVE0;
         memset(&_recprogramInfo[i],0,sizeof(PVRProgramInfo_t));
        if(b2ndEncryptionEnabled)
        {
            memset(&stPVRRecordDSCMBInfo[i],0,sizeof(PVR_DSCMB_INFO));
        }
    }
    _gWinInfo.GopDest=E_GOP_DST_OP0;
    _gWinInfo.GeWinId=0xff;
    _gWinInfo.u8FBId=0xff;
    _gWinInfo.GeDstFmt=E_MS_FMT_ARGB4444;

    _gWinInfo.GopBitmapWidth=PVR_THUMBNAIL_DISPLAY_WIDTH;
    _gWinInfo.GopBitmapHeight=PVR_THUMBNAIL_DISPLAY_HEIGHT;

#if (DEMO_PVR_LINUX_MOUNT_NOTIFIER_TEST == 1)
    //create pvr USB notifier thread
    if(_pvrMNTTask.iId < 0)
    {
        _gbPvrMntExit = FALSE;
        _Demo_PVR_MountNotifier_Create();
    }
#endif

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
// [CHECK] Try to fixed the long write file time
    char Path[FILE_PATH_SIZE];
    strncpy(Path, pMountPath, strlen(pMountPath));
    Path[strlen(pMountPath)] = '\0';
    MS_U32 length = strlen(Path);
    Path[length] = '/';
    Path[length+1] = 'f';
    Path[length+2] = 'i';
    Path[length+3] = 'r';
    Path[length+4] = 's';
    Path[length+5] = 't';
    Path[length+6] = '\0';

    _Demo_PVR_WriteFirstData(Path);
#endif
    printf("Init PVR successfully  \n");
    return TRUE;


}

//------------------------------------------------------------------------------
/// @brief The sample code to inform pvr task an unplug event
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_USB_UnplugEvent \n
///
/// Sample Command:PVR_USB_UnplugEvent
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_USB_UnplugEvent(char *moutPath)
{
    MS_BOOL bRet = FALSE;
    PVREventInfo_t stEventInfo;
    memset((void *)&stEventInfo,0,sizeof(PVREventInfo_t));

    if(_bPvrInit && (NULL != moutPath))
    {
        stEventInfo.pvrEvent = EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE;
        _bUplugMsg=TRUE;
        if(strlen(moutPath)>FILE_PATH_SIZE)
        {
            printf("mountPath size is over %d!\n", FILE_PATH_SIZE);
            return bRet;
        }
        strncpy(_moutPath,moutPath, strlen(moutPath));
        _moutPath[strlen(moutPath)] = '\0';
        MsOS_SendToQueue(_s32QueueID, (MS_U8 *)&stEventInfo, sizeof(PVREventInfo_t), 1000);
        DemoPvr_print("send to Queue <2> _u8TimeshiftRecordIdx=%d\n",_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);
        bRet = TRUE;


    }
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to test USB speed.
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SpeedCheck \n
///
/// Sample Command:PVR_SpeedCheck
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_CheckUSBSpeed(void)
{
    DemoPvr_print("========= Demo_PVR_CheckUSBSpeed =========\n");
    DEMO_PVR_CHECK_INIT();
    printf("Please Wait...\n");

    MApi_PVR_SpeedCheck();
    return TRUE;
}

#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to test encrypt file.
/// @param[in] file name
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Demo_PVR_CheckEncryptFile \n
///
/// Sample Command:Demo_PVR_CheckEncryptFile
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_CheckEncryptFile(char *fileName)
{
    DemoPvr_print("========= Demo_PVR_CheckEncryptFile =========\n");
    DEMO_PVR_CHECK_INIT();
    printf("Please Wait...\n");

    MS_BOOL bRet = FALSE;
    MS_U16 *pu16FileName = NULL;

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8 *)fileName, pu16FileName, strlen(fileName));
    bRet = MApi_PVR_EncryptFileCheck((char *)pu16FileName);
    _Demo_PVR_MEMFREE_FUNC(&pu16FileName);
    return bRet;
}
#endif // DEMO_PVR_SUPPORT_DYNAMICKEY_TEST

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
MS_BOOL Demo_PVR_Destroy(void)
{

    DemoPvr_print("========= Demo_PVR_Destroy =========\n");
    int i=0;
    PVR_AV_INFO stInitFileAVInfo = {FALSE,INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_FILTER_ID,INVALID_PID_ID,INVALID_PID_ID,INVALID_PID_ID,{0,0},E_VDEC_DDI_CODEC_TYPE_MPEG2,DEMO_AUDIO_CODEC_MPEG,E_DMX_FLT_TYPE_INVALID,E_DMX_FLT_TYPE_INVALID,E_DMX_FLOW_INVALID};

    DEMO_PVR_CHECK_INIT();

    MS_U32  pu32RetrievedEventFlag;

    for(i=0 ; i < PVR_MAX_RECORDING_FILE ; i++)
    {
        if(MApi_PVR_IsRecording(i) == TRUE)
        {
            printf("There is a Record\n");
            return FALSE;
        }
    }

    for(i=0 ; i < MAX_PLAYBACK_NUM ; i++)
    {
        if(MApi_PVR_EX_IsPlaybacking(i) == TRUE)
        {
            printf("There is a Playback\n");
            return FALSE;
        }
    }

    // We want to terminate _Demo_PVR_Task before MApi_PVR_Destroy, because the queue will be destroyed in Fuchsia
    if(_pvrEventTask.iId!=INVALID_TASK_ID)
    {

        PVREventInfo_t stEventInfo;
        memset((void *)&stEventInfo,0,sizeof(PVREventInfo_t));
        stEventInfo.pvrEvent = EN_APIPVR_EVENT_NOTIFY_DESTROY_PVR;
        _gbPvrEventExit = TRUE;
        MS_BOOL bRet= MsOS_SendToQueue(_s32QueueID, (MS_U8 *)&stEventInfo, sizeof(PVREventInfo_t), 1000);
        if (bRet == FALSE)
        {
            printf("MsOS_SendToQueue FAIL!!\n");
        }
        else
        {
            DemoPvr_print("MsOS_SendToQueue SUCCESS\n");
        }
        MsOS_WaitEvent(_gs32PvrEvtGrp, PVR_EVENT_DESTROY_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);

#if defined(MSOS_TYPE_ECOS)
        //Polling the status of _Demo_PVR_Task to make sure it already returned
        _Demo_PollingThreadStatus(_pvrEventTask);
#endif
        _pvrEventTask.iId = INVALID_TASK_ID;

    }
#if (DEMO_PVR_LINUX_MOUNT_NOTIFIER_TEST == 1)
    MS_S32 s32NonCachedPoolID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
    _gbPvrMntExit = TRUE;
     MsOS_WaitEvent(_gs32PvrMntGrp, PVR_MOUNT_DESTROY_PROC_EXIT, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
    //free stack memory
    if (_pvrMNTTask.pStack != NULL)
    {
        MsOS_FreeMemory(_pvrMNTTask.pStack,s32NonCachedPoolID);
    }
    _pvrMNTTask.iId = -1;
    _pvrMNTTask.pStack = NULL;
#endif


    if(MApi_PVR_Destroy())
    {
        _bPvrInit= FALSE;
        _s32QueueID = -1;


        //*Init relative info*//
        _u8CurRecProgramIdx=0;
        _eCurDMXInputSrc=E_DMX_FLOW_INPUT_DEMOD0;
        _bUplugMsg=FALSE;
        _moutPath[0]='\0';

        _eCurPlaybackProgramPath=0;
        _u8TotalPlaybackProgram=0;
        for(i=0;i<MAX_PLAYBACK_NUM;i++)
        {
             _u8hPlayback[i]=APIPVR_INVALID_HANDLER;
             _bScreenFrozen[i]=TRUE;
             _u8TimeshiftRecordIdx[i]= INVALID_RECORD_INDEX;
             _u8TimeshiftPlaybackIdx[i] = PVR_PATH_INVALID;
             memset(&_plyprogramInfo[i],0,sizeof(PVRProgramInfo_t));
             memset(&_livePromInfo[i],0,sizeof(PVRProgramInfo_t));
             memcpy(&stFileAVInfo[i],&stInitFileAVInfo,sizeof(PVR_AV_INFO));
             memcpy(&_AudStreamID[i],&_NullAudStreamID,sizeof(AUDIO_DEC_ID));
             memcpy(&_VidStreamID[i],&_NullVidStreamID,sizeof(VDEC_StreamId));
            if(b2ndEncryptionEnabled)
            {
                memset(&stPVRLiveDSCMBInfo[i],0,sizeof(PVR_DSCMB_INFO));
            }
        }

        for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
        {
             _u8hRecord[i]=APIPVR_INVALID_HANDLER;
             _eCurDMXFlowSet[i]=E_DMX_FLOW_LIVE0;
             memset(&_recprogramInfo[i],0,sizeof(PVRProgramInfo_t));
            if(b2ndEncryptionEnabled)
            {
                memset(&stPVRRecordDSCMBInfo[i],0,sizeof(PVR_DSCMB_INFO));
            }
        }
        printf("Destroy PVR successfully\n");

        #if defined(MSOS_TYPE_LINUX)
        if(bSyncConfigHasBeenSet)
        {
            MS_U32 u32Value = 0;
            char recoverDirtyBackgroundRatio[MAX_PROC_BUFFER_SIZE] = {0};
            char recoverDirtyRatio[MAX_PROC_BUFFER_SIZE] = {0};
            char recoverDirtyExpireCentisecs[MAX_PROC_BUFFER_SIZE] = {0};
            char recoverDirtyWritebackCentisecs[MAX_PROC_BUFFER_SIZE]= {0};
            strncpy(recoverDirtyBackgroundRatio, origDirtyBackgroundRatio, sizeof(recoverDirtyBackgroundRatio)-1);
            strncpy(recoverDirtyRatio, origDirtyRatio, sizeof(recoverDirtyRatio)-1);
            strncpy(recoverDirtyExpireCentisecs, origDirtyExpireCentisecs, sizeof(recoverDirtyExpireCentisecs)-1);
            strncpy(recoverDirtyWritebackCentisecs, origDirtyWritebackCentisecs, sizeof(recoverDirtyWritebackCentisecs)-1);
            _Demo_PVR_SetSyncConfig(&u32Value, recoverDirtyBackgroundRatio, recoverDirtyRatio, recoverDirtyExpireCentisecs, recoverDirtyWritebackCentisecs);
        }
        #endif
        bSyncConfigHasBeenSet = FALSE;

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
/// @param[in] pu32VCodec Video Codec  of the current program  0 is for MPEG2. 1 is for H264. 2 is for AVS. 3 is for HEVC(H.265).
/// @param[in] pu32ACodec Audio Codec  of the current program
///         0: NOT VALID
///         1: MPEG
///         2: AC3
///         3: AC3P
///         4: AAC
/// @param[in] fileName  file name
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RecordStart \n
///
/// Sample Command:PVR_RecordStart
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Record_Start(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char* fileName)
{
    /* For detect USB space */
    MS_U64 u64FreeSpaceInKB, u64TotalSpaceInKB;
    MS_BOOL bRet = FALSE;
    DemoPvr_print("========= PVR Record Start =========\n");
    DemoPvr_print("(%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u")\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);

    DEMO_PVR_CHECK_INIT();
    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);

    MS_U8 *pu8MountPath = NULL;

    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)


    pu8MountPath = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U8)*FILE_PATH_SIZE*2);
    _U16StringToU8String((MS_U16*)mountPath, pu8MountPath, wcslen((MS_U16*)mountPath));
    if( _Demo_PVR_IsFileExist((char *)pu8MountPath,fileName) && (_bAppend == FALSE))
    {
        printf("%s exist.\n",fileName);
        _Demo_PVR_MEMFREE_FUNC((void**)&pu8MountPath);
        return FALSE;
    }

    DemoPvr_print("<2> _Demo_PVR_GetDiskSpace :%s\n",(char *)pu8MountPath);

    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(!_Demo_PVR_GetDiskSpace(mountPath,&u64FreeSpaceInKB,&u64TotalSpaceInKB))
    {
        printf("Disk Space can't be detected! \n");
        _Demo_PVR_MEMFREE_FUNC((void**)&pu8MountPath);
        return FALSE;
    }
    if(u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
    {
        printf("Disk Space is not enough! (less than %dMB)\n",PVR_FILE_SIZE_TO_STOP/1024);
        _Demo_PVR_MEMFREE_FUNC((void**)&pu8MountPath);
        return FALSE;
    }

    bRet = _Demo_PVR_RecordStart(FALSE, _eCurDMXInputSrc, _eCurDMXFlowSet[_u8CurRecProgramIdx], u64FreeSpaceInKB/1024, VideoPid, AudioPid, PCRPid, pu32VCodec, pu32ACodec, fileName);



    _Demo_PVR_MEMFREE_FUNC((void**)&pu8MountPath);
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change record program. Used for Dynamic PMT.
///           If there is a DSCMB connection and want to change key, need call Demo_PVR_Record_DSCMB_Change() first,
///           That is, Demo_PVR_Record_DSCMB_Change() -> Demo_PVR_Record_ChangeProgram()
/// @param[in] VideoPid Video PID of the current program
/// @param[in] AudioPid Audio PID of the current program
/// @param[in] PCRPid PCR PID of the current program
/// @param[in] pu32VCodec Video Codec  of the current program  0 is for MPEG2. 1 is for H264.2 is for AVS. 3 is for HEVC(H.265).
/// @param[in] pu32ACodec Audio Codec  of the current program
///         0: NOT VALID
///         1: MPEG
///         2: AC3
///         3: AC3P
///         4: AAC
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Record_ChangeProgram \n
///
/// Sample Command:PVR_Record_ChangeProgram
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Record_ChangeProgram(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    MS_BOOL bTimeshift = FALSE;
    DemoPvr_print("========= PVR Record ChangeProgram =========\n");
    DemoPvr_print("Change to (VPID,APID,PCR,VDEC,ADEC)(%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u")\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);

    DEMO_PVR_CHECK_INIT();
    if (_Demo_PVR_IsTimeshiftRecord(_u8CurRecProgramIdx) == TRUE)
    {
        bTimeshift = TRUE;
    }

    //[1]Change Record Program info
    if (!_Demo_PVR_ChangeProgramPIDs(bTimeshift,_eCurDMXFlowSet[_u8CurRecProgramIdx],*PCRPid,*VideoPid,*AudioPid,*pu32ACodec,*pu32VCodec))
    {
        printf("PVR ChangeProgramPIDs Failed!\n");
        return FALSE;
    }

    //[2]Reconfigure DSCMB Info about Type and CW if there is a  DSCMB connection
    //[Why] If disconnect all pidflt for old program, and then connect new program of pidflt for one DSCMB id,
    //"need reconfigure DSCMB Type and CW, or the setting before disconnect will be clear, and the record data from LIVE would be blocked into DSCMB module"
    //(Efang Chung said that is HW Limitation)
#if defined(DEMO_PVR_DSCMB_CALLBACK_MODE)
   if (_Demo_PVR_Record_DSCMB_SetInfo(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx]) != TRUE)
    {
        printf("[%s][%d]DSCMB SetInfo Error!\n",__FILE__,__LINE__);
        return FALSE;
    }
#endif

    return TRUE;
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
MS_BOOL Demo_PVR_Record_Stop(void)
{
    DEMO_PVR_CHECK_INIT();
    if (_Demo_PVR_IsTimeshiftRecord(_u8CurRecProgramIdx) == TRUE)
    {
        printf("Error!! Timeshift Mode! Current record index : %u\n", _u8CurRecProgramIdx);
        return FALSE;
    }

    DemoPvr_print("Demo_PVR_Record_Stop %d ::",_u8CurRecProgramIdx);
    wprintf((MS_U16*)_recprogramInfo[_u8CurRecProgramIdx].FileName, PVR_DBG, TRUE);
    _Demo_PVR_RecStop(FALSE, _u8CurRecProgramIdx);
    DemoPvr_print("Demo_PVR_Record_Stop -->end \n");
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
MS_BOOL Demo_PVR_Record_Pause(void)
{
    DemoPvr_print("=========  Demo_PVR_Record_Pause =========\n");
    DEMO_PVR_CHECK_INIT();

    return MApi_PVR_RecordPause(_u8hRecord[_u8CurRecProgramIdx]);
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
MS_BOOL Demo_PVR_Record_Resume(void)
{
    DemoPvr_print("=========  Demo_PVR_Record_Resume =========\n");
    DEMO_PVR_CHECK_INIT();

    return MApi_PVR_RecordResume(_u8hRecord[_u8CurRecProgramIdx]);
}

#if (DEMO_DSCMB_ADVANCED_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to set global CAVID
///The CAVID setting here is for KL/CIPHER DMA module used in PVR record
///1. For CIPHE key source from KL and set to Secrete key (eSrc = E_DEMO_KL_SRC_SECRET_1 ~E_DEMO_KL_SRC_SECRET_4),
///the CAVID in Secrete key need equal to CIPHER and KL
///2. If not setting here , default CAVID is 0x0F.
/// @param[in]  CAVid the CA Vendor ID for KL or CIPHER DMA,
/// it need to be match to the one by descrambler defined in demo_dscmb_advanced.c
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Record_DSCMB_SetCAVID \n
///
/// Sample Command:PVR_Record_DSCMB_SetCAVID
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Record_DSCMB_SetCAVID(MS_U32 *u32CAVid)
{
    if (u32CAVid == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32PVRDSCMBCAVid = *u32CAVid;
    DemoPvr_print("========= PVR Set DSCMB CAVID: %d =========\n",u32PVRDSCMBCAVid);
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code to Open a  Descrambler for record.
/// @param[in] u8Type a string represent the algorithm.
///                 please reference DSCMB_Type_Map table defined in demo_dscmb_basic.c or demo_dscmb_advanced.c
/// @param[in] u8StrOddCW a hex string represent a odd control word
/// @param[in] u8StrEvenCW a hex string represent a even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Record_DSCMB_Open \n
///
/// Sample Command:PVR_Record_DSCMB_Open
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Record_DSCMB_Open(MS_U8 *u8Type,MS_U8 *u8StrOddCW,MS_U8 *u8StrEvenCW)
{
    DemoPvr_print("========= PVR Record Open DSCMB: Type:(%s)  CW: Odd(%s) Even(%s)=========\n",u8Type,u8StrOddCW,u8StrEvenCW);
    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);

    if (stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].bDSCMBInfoSet == TRUE)
    {
        printf("[%s][%d]record dscmb Idx[%d] already opened!\n",__FUNCTION__,__LINE__,_u8CurRecProgramIdx);
        return FALSE;
    }
    memset(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx], 0, sizeof(PVR_DSCMB_INFO));
    if( (strlen((char *)u8Type) > PVR_DSCMB_INFO_CW_SIZE) || (strlen((char *)u8StrOddCW) > PVR_DSCMB_INFO_CW_SIZE) || (strlen((char *)u8StrEvenCW) > PVR_DSCMB_INFO_CW_SIZE) )
    {
        printf("[%s][%d] DSCM CW Size is over %d\n",__FUNCTION__,__LINE__, PVR_DSCMB_INFO_CW_SIZE);
        return FALSE;
    }
    memcpy(stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].u8Type, u8Type, strlen((char *)u8Type));
    memcpy(stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].u8StrOddCW, u8StrOddCW, strlen((char *)u8StrOddCW));
    memcpy(stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].u8StrEvenCW, u8StrEvenCW, strlen((char *)u8StrEvenCW));
    stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].bDSCMBInfoSet = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Close a  Descrambler for record.
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Record_DSCMB_Close \n
///
/// Sample Command:PVR_Record_DSCMB_Close
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Record_DSCMB_Close(void)
{
    DemoPvr_print("========= PVR Record Close DSCMB=========\n");
    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);

    if (stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].bDSCMBInfoSet == FALSE)
    {
        printf("[%s][%d]record dscmb Idx[%d] already closed or not exist!\n",__FUNCTION__,__LINE__,_u8CurRecProgramIdx);
        return FALSE;
    }
    memset(&stPVRRecordDSCMBInfo[_u8CurRecProgramIdx], 0, sizeof(PVR_DSCMB_INFO));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Change config for descrambler that is already opened and connect before in PVR record process.
///          Need used along with Demo_PVR_Record_ChangeProgram(), that is Demo_PVR_Record_DSCMB_Change() -> Demo_PVR_Record_ChangeProgram()
/// @param[in] u8Type a string represent the algorithm.
///                 please reference DSCMB_Type_Map table defined in demo_dscmb_basic.c or demo_dscmb_advanced.c
/// @param[in] u8StrOddCW a hex string represent a odd control word
/// @param[in] u8StrEvenCW a hex string represent a even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_Record_DSCMB_Open \n
///
/// Sample Command:PVR_Record_DSCMB_Change
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Record_DSCMB_Change(MS_U8 *u8Type,MS_U8 *u8StrOddCW,MS_U8 *u8StrEvenCW)
{
    DemoPvr_print("========= PVR Record Open DSCMB: Type:(%s)  CW: Odd(%s) Even(%s)=========\n",u8Type,u8StrOddCW,u8StrEvenCW);
    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);

    if ((stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].bDSCMBInfoSet == FALSE) || (stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].bConnectStatus == FALSE))
    {
        printf("[%s][%d]record dscmb Idx[%d] not opened and create resource!\n",__FUNCTION__,__LINE__,_u8CurRecProgramIdx);
        return FALSE;
    }
    if( (strlen((char *)u8Type) > PVR_DSCMB_INFO_CW_SIZE) || (strlen((char *)u8StrOddCW) > PVR_DSCMB_INFO_CW_SIZE) || (strlen((char *)u8StrEvenCW) > PVR_DSCMB_INFO_CW_SIZE) )
    {
        printf("[%s][%d] DSCM CW Size is over %d\n",__FUNCTION__,__LINE__, PVR_DSCMB_INFO_CW_SIZE);
        return FALSE;
    }
    memcpy(stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].u8Type, u8Type, strlen((char *)u8Type));
    memcpy(stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].u8StrOddCW, u8StrOddCW, strlen((char *)u8StrOddCW));
    memcpy(stPVRRecordDSCMBInfo[_u8CurRecProgramIdx].u8StrEvenCW, u8StrEvenCW, strlen((char *)u8StrEvenCW));

    return TRUE;
}

#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to set current ecvrypt key index
/// @param[in] encrypt key index number
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetEncryptKeyIdx \n
///
/// Sample Command:PVR_SetEncryptKeyIdx 6
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetEncryptKeyIdx(MS_U32 *u32KeyIdx)
{
    DemoPvr_print("=========  Demo_PVR_SetEncryptKeyIdx =========\n");
    DEMO_PVR_CHECK_INIT();

    //DynamicKey_print("[%d]Current encrypt key idx =%d\n",_u8CurRecProgramIdx,*u32KeyIdx);
    return MApi_PVR_EX_SetEncryptKeyIdx(_u8CurRecProgramIdx, *u32KeyIdx);
}
#endif // DEMO_PVR_SUPPORT_DYNAMICKEY_TEST

//------------------------------------------------------------------------------
/// @brief The sample code to record a live stream in timeshift mode
/// @param[in] VideoPid Video PID of the current live program
/// @param[in] AudioPid Audio PID of the current live program
/// @param[in] PCRPid PCR PID of the current live program
/// @param[in] pu32VCodec Video Codec  of the current live program  0 is for MPEG2. 1 is for H264. 2 is for AVS. 3 is for HEVC(H.265).
/// @param[in] pu32ACodec Audio Codec  of the current live program
///         0: NOT VALID
///         1: MPEG
///         2: AC3
///         3: AC3P
///         4: AAC
/// @param[in] fileName  file name
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_TimeshiftRecordStart \n
///
/// Sample Command:PVR_TimeshiftRecordStart
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_Timeshift_RecordStart(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char* fileName)
{
    DEMO_PVR_CHECK_INIT();

    MS_BOOL bRet=FALSE;
    MS_U64 u64FreeSpaceInKB, u64TotalSpaceInKB;
    DemoPvr_print("=========  Demo_PVR_Timeshift_RecordStart =========\n");
    DemoPvr_print("(%"DTC_MS_U32_d",%"DTC_MS_U32_d",%"DTC_MS_U32_d",%"DTC_MS_U32_d",%"DTC_MS_U32_d")\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);

    DEMO_PVR_CHECK_REC_IDX(_u8CurRecProgramIdx, FALSE);
    if(MApi_PVR_EX_IsTimeShift(_eCurPlaybackProgramPath))
    {
        printf("It is timeshift now.\n");
        return FALSE;
    }

    if(MApi_PVR_IsRecording(_u8hRecord[_u8CurRecProgramIdx]))
    {
        printf("Record now. Fail to start timeshift record.\n");
        return FALSE;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
         printf("Play now. Fail to start timeshift record.\n");
         return FALSE;
    }

    printf("Try to Remove timeshift file.\n");
    MS_U16 *pu16FileName = NULL;

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8*)fileName, pu16FileName, strlen(fileName));
    MApi_PVR_RemoveFile((char*)pu16FileName);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);


    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    DemoPvr_print("_moutPath=");
    wprintf((MS_U16*)mountPath, PVR_DBG, TRUE);
    if( _Demo_PVR_GetDiskSpace(mountPath,&u64FreeSpaceInKB, &u64TotalSpaceInKB))
    {
        printf("u64FreeSpaceInMB=0x%"DTC_MS_U64_x" \n",u64FreeSpaceInKB/1024);
        DemoPvr_print("=========  Demo_PVR_Record_Stop =========\n");
        if(u64FreeSpaceInKB < MIN_TIME_SHIFT_DISK_SPACE_IN_KB)
        {
            printf("Out of Disk Space. Timeshift need at least 512MB.\n");
            return FALSE;
        }
    }
    else
    {
        printf("No Free Disk Deteced!\n");
        return FALSE;
    }

    _u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]=_u8CurRecProgramIdx;
    printf("_u8TimeshiftRecordIdx = %u\n",_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);
    bRet = _Demo_PVR_RecordStart(TRUE, _eCurDMXInputSrc, _eCurDMXFlowSet[_u8CurRecProgramIdx], u64FreeSpaceInKB/1024, VideoPid, AudioPid, PCRPid, pu32VCodec, pu32ACodec, fileName);

    if(bRet)
    {
        printf("\n***** Timeshift Recorder *****\n");
        printf("Screen Frezon: %s\n",(_bScreenFrozen[_eCurPlaybackProgramPath])?"TRUE":"FALSE");
        printf("File Name: %s\n",fileName);
        printf("Program Info:\n");
        printf("\t Video PID: 0x%"DTC_MS_U32_x"\n",*VideoPid);
        printf("\t Audio PID: 0x%"DTC_MS_U32_x"\n",*AudioPid);
        printf("\t PCR   PID: 0x%"DTC_MS_U32_x"\n",*PCRPid);
        printf("\t Video Dec: %"DTC_MS_U32_u"\n",*pu32VCodec);
        printf("\t Audio Dec: %"DTC_MS_U32_u"\n",*pu32ACodec);
        printf("***************************\n");
        printf("\n");
        printf("Timeshift Record Successfully.\n");
    }
    else
    {
        _u8TimeshiftRecordIdx[_eCurPlaybackProgramPath] = INVALID_RECORD_INDEX;
    }


    DemoPvr_print("_u8TimeshiftIdx=%d\n",_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);

    return bRet;
}

MS_BOOL _Demo_PVR_Timeshift_RecordStop(MS_U8 u8RecordIdx)
{
    DEMO_PVR_CHECK_INIT();

    Ex_PVRPlaybackPath ePlaybackidx = (Ex_PVRPlaybackPath)_Demo_PVR_Timeshift_GetPlaybackIdx(u8RecordIdx);
    Ex_PVRPlaybackPath ePlayIdxSaved = PVR_PATH_INVALID;
    if (ePlaybackidx == PVR_PATH_INVALID)
    {
        printf("[%s][%d]Fail to get timeshift playbackidx: %u from _u8CurRecProgramIdx:%u _eCurPlaybackProgramPath:%u!\n", __FUNCTION__, __LINE__, ePlaybackidx, _u8CurRecProgramIdx, _eCurPlaybackProgramPath);
        return FALSE;
    }

    ePlayIdxSaved = _eCurPlaybackProgramPath;
    _eCurPlaybackProgramPath = ePlaybackidx;
    if(!_Demo_PVR_RecStop(TRUE, u8RecordIdx))
    {
        printf("Fail to stop recording u8RecordIdx = %u\n", u8RecordIdx);
        return FALSE;
    }
    _eCurPlaybackProgramPath = ePlayIdxSaved;
    _u8TimeshiftRecordIdx[ePlaybackidx]=INVALID_RECORD_INDEX;
    //check if there is timeshift playback
    if(_u8TimeshiftPlaybackIdx[ePlaybackidx] == PVR_PATH_INVALID)
    {
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        EN_AV_Device eDevice = E_AV_DEVICE_MAIN;
        // ===================== Get the device ID for vdec =============================

        Demo_PVR_Check_AVPath(eDevice,ePlaybackidx,PVR_PATH_MAIN,E_AV_DEVICE_MAIN,PVR_PATH_SUB,E_AV_DEVICE_SUB);

        if(_bTimeshiftSeamless[ePlaybackidx] == TRUE)
        {
            _bTimeshiftSeamless[ePlaybackidx] = FALSE;
            printf("[%s][%d] Set _bTimeshiftSeamless (%d)\n",__FUNCTION__,__LINE__,_bTimeshiftSeamless[ePlaybackidx]);
            printf("[%s][%d] prepare to switch to live stream and stop live vdec\n",__FUNCTION__,__LINE__);
            _DualAVFunc.DualAV_StopAV(&eDevice);
            if(_livePromInfo[ePlaybackidx].bLive)
            {
                printf("[%s][%d] switch to live stream!\n",__FUNCTION__, __LINE__);
                if (_Demo_PVR_AV_Start(TRUE, &_livePromInfo[ePlaybackidx]))
                {
                    _Demo_PVR_AV_Play();
                    printf("[%s][%d] switch to live stream success\n",__FUNCTION__, __LINE__);
                    _Demo_PVR_LiveInfo_Initialize(ePlaybackidx);
                }
                else
                {
                    printf("[%s][%d] \n[Demo_PVR_PlaybackStop] Return to Live[%u] failed!!!\n\n",__FUNCTION__, __LINE__,ePlaybackidx);
                }
            }
        }
#endif
    }
    _u64FileSizeInKB = PVR_FILE_SIZE_MAX;
    return TRUE;
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
MS_BOOL Demo_PVR_Timeshift_RecordStop(void)
{
    return _Demo_PVR_Timeshift_RecordStop(_u8CurRecProgramIdx);

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
MS_BOOL Demo_PVR_Timeshift_Stop(void)
{
    DemoPvr_print("=========  Demo_PVR_Timeshift_Stop =========\n");
    DEMO_PVR_CHECK_INIT();
    if(_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]==INVALID_RECORD_INDEX && _u8TimeshiftPlaybackIdx[_eCurPlaybackProgramPath] == PVR_PATH_INVALID)
    {
        printf("Already stop timeshift mode!\n");
        return FALSE;
    }

    Demo_PVR_Timeshift_PlaybackStop();
    _Demo_PVR_Timeshift_RecordStop(_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);

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
MS_BOOL Demo_PVR_Timeshift_PlaybackStart(void)
{
    DemoPvr_print("=========  Demo_PVR_Timeshift_PlaybackStart =========\n");
    PVRFilterInfo_t *Filters;
    MS_BOOL bRet = FALSE;
    MS_U32 u32VideoPid;
    MS_U32 u32AudioPid;
    MS_U32 u32PCRPid;
    MS_U32 u32VCodec;
    MS_U32 u32ACodec;
    MS_U32 u32PmtPid;
    MS_U32 u32LCN;
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;
    // ===================== Check PVR init =========================================
    DEMO_PVR_CHECK_INIT();
    // ===================== Get the device ID for vdec =============================

    Demo_PVR_Check_AVPath(eDevice,_eCurPlaybackProgramPath,PVR_PATH_MAIN,E_AV_DEVICE_MAIN,PVR_PATH_SUB,E_AV_DEVICE_SUB);

    // ===================== Check Machine ===========================================
    if(_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]==INVALID_RECORD_INDEX)
    {
        printf("Not to Start timeshiftrecording!\n");
        return FALSE;
    }

    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        DemoPvr_Errorprint("USB is not ready!\n");
        return FALSE;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        DemoPvr_print("Normal Play\n");
        bRet=MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],EN_APIPVR_PLAYBACK_SPEED_1X);
    }
    else
    {
        DemoPvr_print("Play FileName[%d] = ",_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);
        wprintf((MS_U16*)_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].FileName, PVR_DBG, TRUE);
        // ------ [1] Prepare playback info. (pids and codec) ------
        Filters= _recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].Filters;
        u32VideoPid=_Demo_PVR_FindPidinFilter(Filters, DMX_FILTER_TYPE_VIDEO);
        u32AudioPid=_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].u16AudioPid;
        u32PCRPid=_Demo_PVR_FindPidinFilter(Filters, DMX_FILTER_TYPE_PCR);
        u32VCodec=_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].enVCodec;
        u32ACodec=_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].u32ACodec;
        u32PmtPid=_Demo_PVR_FindPidinFilter(Filters, DMX_FILTER_TYPE_SECTION);
        u32LCN=_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].u32LCN;
        DemoPvr_print("(%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u")  %d\n",u32VideoPid,u32AudioPid,u32PCRPid,u32VCodec,u32ACodec,_recprogramInfo[_u8TimeshiftIdx[_eCurPlaybackProgramPath]].enVCodec);

        // ------ [2] Store playback info. to _plyprogramInfo structure ------
        if(_Demo_PVR_SetProgramInfo(&_plyprogramInfo[_eCurPlaybackProgramPath],u32PCRPid, u32VideoPid, u32AudioPid,  u32VCodec,  u32ACodec, u32PmtPid,  u32LCN,FALSE) == FALSE)
        {
            return FALSE;
        }

        wcscpy((MS_U16*)_plyprogramInfo[_eCurPlaybackProgramPath].FileName,(MS_U16*)_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].FileName);
        DemoPvr_print("Play FileName = ");
        wprintf((MS_U16*)_plyprogramInfo[_eCurPlaybackProgramPath].FileName, PVR_DBG, TRUE);
        // [3] ------  Get AV stream ID ------
        _AVStreamID[_eCurPlaybackProgramPath].vstreamID = _Demo_PVR_GetVidStreamID(&_eCurPlaybackProgramPath);
        _AVStreamID[_eCurPlaybackProgramPath].astreamID = _Demo_PVR_GetAudStreamID(&_eCurPlaybackProgramPath);
        if (_AVStreamID[_eCurPlaybackProgramPath].vstreamID == NULL)
        {
            printf("[%s][%d]Error vdec streamid!!\n",__FUNCTION__, __LINE__);
            return FALSE;
        }

        if(_livePromInfo[_eCurPlaybackProgramPath].bLive)
        {
            if (Demo_AV_GetAVInfo(&eDevice,E_AV_GetCmd_LiveInfo,&_stLiveInfo[_eCurPlaybackProgramPath])  == FALSE)
            {
                printf("[%s]Error!!!, Get Live info by Demo_AV_GetAVInfo failed\n",__FUNCTION__);
                return FALSE;
            }
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
            if(_bTimeshiftSeamless[_eCurPlaybackProgramPath] == TRUE)
            {
                printf("[%s][%d] Stop Live AVFIFO\n",__FUNCTION__,__LINE__);
                // For Timeshift Seamless Feature, the vdec do not need to stop.
                _DualAVFunc.DualAV_StopAVFIFO(&eDevice);
            }
            else
#endif
            {
                printf("[%s][%d] Stop Live AVdec & AVFIFO\n",__FUNCTION__,__LINE__);
                _DualAVFunc.DualAV_StopAV(&eDevice);
            }
        }

        do{
            MApi_PVR_EX_PlaybackABLoopReset(_u8hPlayback[_eCurPlaybackProgramPath]);

            bRet=MApi_PVR_EX_TimeshiftPlaybackStart(_eCurPlaybackProgramPath,&_u8hPlayback[_eCurPlaybackProgramPath],_eCurPlaybackProgramPath,(void *)&_AVStreamID[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,0,0);
        }while(0);
        if( bRet!=TRUE)
        {
            // If playback failed,
            // playbackstop & playbackclose would be called in PVR MW.
            // No need to call FileIn_StopAV.

            if(_livePromInfo[_eCurPlaybackProgramPath].bLive)
            {
                printf("timeshift play fail.\nPlay live stream...\n");
                if (_Demo_PVR_AV_Start(TRUE, &_livePromInfo[_eCurPlaybackProgramPath]))
                {
                    printf("switch to live stream seccess!\n");
                    _Demo_PVR_FreezeScreen(FALSE);
                    _Demo_PVR_AV_Play();
                    _Demo_PVR_LiveInfo_Initialize(_eCurPlaybackProgramPath);
                }
                else
                {
                    printf("\n[Demo_PVR_Timeshift_PlaybackStart] Return to Live[%u] failed!!!\n\n",_eCurPlaybackProgramPath);
                }
                return bRet;
            }
        }
        else
        {
            _u8TimeshiftPlaybackIdx[_eCurPlaybackProgramPath] = _eCurPlaybackProgramPath;
            _u8TotalPlaybackProgram++;
        }
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        if(_bTimeshiftSeamless[_eCurPlaybackProgramPath] == FALSE)
#endif
        {
            printf("[%s][%d] Freeze cmd & MVOP\n",__FUNCTION__,__LINE__);
            _Demo_PVR_FreezeScreen(FALSE);
        }
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
        else
        {
            printf("[%s][%d] Remove Freeze cmd & MVOP\n",__FUNCTION__,__LINE__);
            _bTimeshiftSeamless[_eCurPlaybackProgramPath] = FALSE;
            printf("[%s][%d] Set _bTimeshiftSeamless (%d)\n",__FUNCTION__,__LINE__,_bTimeshiftSeamless[_eCurPlaybackProgramPath]);
        }
#endif
    }

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
MS_BOOL Demo_PVR_Timeshift_PlaybackStop(void)
{
    MS_BOOL bRet=FALSE;
    DEMO_PVR_CHECK_INIT();

    if(_u8TimeshiftPlaybackIdx[_eCurPlaybackProgramPath]==PVR_PATH_INVALID)
    {
        printf("Already stop playing\n");
        return FALSE;
    }
    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No to play!\n");
        return FALSE;
    }

    bRet = MApi_PVR_EX_TimeshiftPlaybackStop(&_u8hPlayback[_eCurPlaybackProgramPath],_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath],_eCurPlaybackProgramPath);
    if(!bRet)
    {
        printf("Fail to Timeshift playback stop\n");
        return bRet;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath])==FALSE)
    {
        if(_livePromInfo[_eCurPlaybackProgramPath].bLive)
        {
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
            if(_bTimeshiftSeamless[_eCurPlaybackProgramPath] == TRUE)
            {
                _bTimeshiftSeamless[_eCurPlaybackProgramPath] = FALSE;
                printf("[%s][%d] Set _bTimeshiftSeamless (%d)\n",__FUNCTION__,__LINE__,_bTimeshiftSeamless[_eCurPlaybackProgramPath]);
            }
#endif
            if (_Demo_PVR_AV_Start(TRUE, &_livePromInfo[_eCurPlaybackProgramPath]))
            {
                printf("switch to live stream!\n");
                _Demo_PVR_AV_Play();
                _Demo_PVR_LiveInfo_Initialize(_eCurPlaybackProgramPath);
            }
            else
            {
                printf("\n[Demo_PVR_Timeshift_PlaybackStop] Return to Live[%u] failed!!!\n\n",_eCurPlaybackProgramPath);
            }
        }
    }


    if(bRet)
    {
        _u8TotalPlaybackProgram--;
        _u8TimeshiftPlaybackIdx[_eCurPlaybackProgramPath] = PVR_PATH_INVALID;
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to start a playback
/// @param[in] filename  the file name of a recored file
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackStart \n
///
/// Sample Command:PVR_PlaybackStart 101 102 101 0 1 28106_101_102_101_0_TEST_19921009_212744.ts 101 102 101 0 1
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_PlaybackStart(char *filename)
{
    MS_U32 VideoPid = 0;
    MS_U32 AudioPid = 0;
    MS_U32 PCRPid = 0;
    MS_U32 pu32VCodec = 0;
    MS_U32 pu32ACodec = 0;
    char *mountPath = NULL;

    DemoPvr_print("=========  Demo_PVR_PlaybackStart =========\n");
    DemoPvr_print("filename=%s\n",(filename)?filename:"NULL");
    MS_BOOL bRet=FALSE;
    MS_U16 u16PmtPid = 0;
    MS_U16 u16LCN = 0;
    EN_AV_Device eDevice = E_AV_DEVICE_MAIN;

    if ((!filename)||(strlen(filename)<=0))
    {
        printf("No Such File Name\n");
        return FALSE;
    }

    MS_U16 *pu16FileName = NULL;
    MS_U8 *pu8MountPath = NULL;

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);

    if(pu16FileName == NULL)
    {
        return FALSE;
    }

    _U8StringToU16String((MS_U8*)filename, pu16FileName, strlen(filename));
    if((_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath] != INVALID_RECORD_INDEX)
    && (0 == wcscmp(pu16FileName,(MS_U16*)_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].FileName)) )
    {
        printf("Error!! Timeshift Mode! %d\n", _u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);
        bRet = FALSE;
        goto END;
    }

    DEMO_PVR_CHECK_INIT();

    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)

    if(! _Demo_PVR_CheckUSB(mountPath))
    {
        printf("USB is not ready!\n");
        bRet = FALSE;
        goto END;
    }

    DemoPvr_print("[%s][%d]  _Demo_PVR_IsFileExist\n", __FUNCTION__, __LINE__);
    pu8MountPath = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U8)*FILE_PATH_SIZE*2);
    if(pu8MountPath == NULL)
    {
        printf("Alloc memory fail!!\n");
        bRet = FALSE;
        goto END;
    }

    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    _U16StringToU8String((MS_U16*)mountPath, pu8MountPath, wcslen((MS_U16*)mountPath));

    if(!_Demo_PVR_IsFileExist((char *)pu8MountPath, filename))
    {
        printf("%s did not exist.\n",filename);
        bRet = FALSE;
        goto END;
    }


    Demo_PVR_Check_AVPath(eDevice,_eCurPlaybackProgramPath,PVR_PATH_MAIN,E_AV_DEVICE_MAIN,PVR_PATH_SUB,E_AV_DEVICE_SUB);

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("Normal Play\n");
        bRet = MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],EN_APIPVR_PLAYBACK_SPEED_1X);
        goto END;
    }
    // Due to Normal Playback now,  set TimeshiftSeamless flag false to enter normal playback flow.
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    if(_bTimeshiftSeamless[_eCurPlaybackProgramPath] == TRUE)
    {
        _bTimeshiftSeamless[_eCurPlaybackProgramPath] = FALSE;
    }
#endif

    DemoPvr_print("[%s][%d] Demo_Dmx_GetCurProgram\n", __FUNCTION__, __LINE__);
    if(_Demo_Dmx_GetLiveProgram((MS_U8)eDevice, &PCRPid, &VideoPid, &AudioPid, &pu32VCodec, &pu32ACodec, &u16PmtPid,&u16LCN) == FALSE)
    {
        _livePromInfo[_eCurPlaybackProgramPath].bLive = FALSE;
        printf("fail to get Live TV Program PMT and PAT Info.\n");
    }
    else
    {
        pu32VCodec = (MS_U32)_Demo_PVR_Get_PromInfo_VDEC_Fmt_FromVDECDDI(pu32VCodec);
        pu32ACodec = (MS_U32)_Demo_PVR_Get_PromInfo_ADEC_Fmt_FromADECDDI(pu32ACodec);

        if(FALSE==_Demo_PVR_SetProgramInfo(&_livePromInfo[_eCurPlaybackProgramPath],(PCRPid), (VideoPid), (AudioPid), (pu32VCodec), (pu32ACodec),(MS_U32)u16PmtPid, (MS_U32)u16LCN,FALSE))
        {
            printf("Can't set program info!\n");
            bRet = FALSE;
            goto END;
        }
        _livePromInfo[_eCurPlaybackProgramPath].bLive = TRUE;
    }

    if(_livePromInfo[_eCurPlaybackProgramPath].bLive)
    {
        _DualAVFunc.DualAV_StopAV(&eDevice);
    }

    do{
        printf("Get Ply info  %s\n",filename);
        if(_Demo_PVR_GetFileInfo((char*)pu16FileName, &_plyprogramInfo[_eCurPlaybackProgramPath])==FALSE)
        {
            printf("Can't get program info\n");
            bRet = FALSE;
            break;
        }

        DemoPvr_print("[%s][%d] path= ", __FUNCTION__, __LINE__);
        wprintf((MS_U16*)_plyprogramInfo[_eCurPlaybackProgramPath].FileName, PVR_DBG, TRUE);
        MApi_PVR_EX_PlaybackABLoopReset(_u8hPlayback[_eCurPlaybackProgramPath]);

        bRet = MApi_PVR_EX_PlaybackStart(_eCurPlaybackProgramPath,&_u8hPlayback[_eCurPlaybackProgramPath],(void *)&_AVStreamID[_eCurPlaybackProgramPath],(char*)pu16FileName,0,0);
    }while(0);
    if(!bRet)
    {
        printf("[%s][%d] fail to playback start \n", __FUNCTION__, __LINE__);
        if(_livePromInfo[_eCurPlaybackProgramPath].bLive)
        {
            printf("switch to live \n");
            if (_Demo_PVR_AV_Start(TRUE, &_livePromInfo[_eCurPlaybackProgramPath]))
            {
                printf("switch to live stream seccess!\n");
                _Demo_PVR_AV_Play();
                _Demo_PVR_LiveInfo_Initialize(_eCurPlaybackProgramPath);
            }
            else
            {
                printf("\n[Demo_PVR_PlaybackStart] Return to Live[%u] failed!!!\n\n",_eCurPlaybackProgramPath);
            }
        }
    }
    else
    {
        _u8TotalPlaybackProgram++;
    }

END:
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu8MountPath);

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to trigger program playback change for Dynamic PMT.
///           Which will triger PVR suspend to prepared for PMT change.
///           And then "must" implement following PMT change flow,that is,_Demo_PVR_ChangePMTEvent(), in event "EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SUSPEND".
/// @param[in] VideoPid Video PID of the current live program
/// @param[in] AudioPid Audio PID of the current live program
/// @param[in] PCRPid PCR PID of the current live program
/// @param[in] pu32VCodec Video Codec  of the current live program  0 is for MPEG2.1 is for H264. 2 is for AVS. 3 is for HEVC(H.265).
/// @param[in] pu32ACodec Audio Codec  of the current live program
///         0: NOT VALID
///         1: MPEG
///         2: AC3
///         3: AC3P
///         4: AAC
/// @param[in] filename  the file name of a recored file
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackChangeProgram \n
///
/// Sample Command:PVR_PlaybackChangeProgram 101 102 101 0 1 28106_101_102_101_0_TEST_19921009_212744.ts
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_PlaybackChangeProgram(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid,const  MS_U32* pu32VCodec,const  MS_U32* pu32ACodec,char *filename)
{
    MS_U16 u16PmtPid;
    MS_U16 u16LCN;
    char u8FileName[32]={0};
    MS_U32 u32TempACodec;
    MS_U32 u32TempVCodec;
    DemoPvr_print("=========  Demo_PVR_PlaybackChangeProgram =========\n");
    DemoPvr_print("filename=%s\n",(filename)?filename:"NULL");
    DemoPvr_print("Change to (VPID,APID,PCR,VDEC,ADEC)(%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u",%"DTC_MS_U32_u")\n",*VideoPid,*AudioPid,*PCRPid,*pu32VCodec,*pu32ACodec);

    DEMO_PVR_CHECK_INIT();

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]) != TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }

    if ((!filename)||(strlen(filename)<=0))
    {
        printf("No Such File Name\n");
        return FALSE;
    }

    strncpy(u8FileName,filename, strlen(filename));
    u8FileName[strlen(filename)] = '\0';

    if(_Demo_Dmx_GetCurProgram(_eCurDMXFlowSet[_u8CurRecProgramIdx],*PCRPid,*VideoPid,*AudioPid,&u16PmtPid,&u16LCN) == FALSE)
    {
        DemoPvr_print("fail to Demo_Dmx_GetCurProgram.\n");
        return FALSE;
    }

    u32TempVCodec = (MS_U32)_Demo_PVR_Get_PromInfo_VDEC_Fmt_FromPVRDDI(*pu32VCodec);
    u32TempACodec = (MS_U32)_Demo_PVR_Get_PromInfo_ADEC_Fmt_FromPVRDDI(*pu32ACodec);

    if(FALSE==_Demo_PVR_SetProgramInfo(&_plyprogramInfo[_eCurPlaybackProgramPath],(*PCRPid), (*VideoPid), (*AudioPid), u32TempVCodec, u32TempACodec, (MS_U32)u16PmtPid, (MS_U32)u16LCN, FALSE))
    {
        printf("Can't set program info for play program!\n");
        return FALSE;
    }

#ifdef PVR_DBG
    DemoPvr_print("\n**************PMT Change*************\n\n");
    DemoPvr_print("\tvideo pid = 0x%u\n",_Demo_PVR_FindPidinFilter(_plyprogramInfo[_eCurPlaybackProgramPath].Filters, DMX_FILTER_TYPE_VIDEO));
    DemoPvr_print("\taudio pid = 0x%"DTC_MS_U32_u"\n",_plyprogramInfo[_eCurPlaybackProgramPath].u16AudioPid);
    DemoPvr_print("\tpcr   pid = 0x%u\n",_Demo_PVR_FindPidinFilter(_plyprogramInfo[_eCurPlaybackProgramPath].Filters, DMX_FILTER_TYPE_PCR));
    DemoPvr_print("\tVideoFmt       = %"DTC_MS_U32_u"\n",_plyprogramInfo[_eCurPlaybackProgramPath].enVCodec);
    DemoPvr_print("\tAudioFmt  = %"DTC_MS_U32_u"\n",_plyprogramInfo[_eCurPlaybackProgramPath].u32ACodec);
    DemoPvr_print("\n***************************\n\n");
#endif

    if(FALSE==MApi_PVR_PlaybackProgramChangeOpen(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("MApi_PVR_PlaybackProgramChangeOpen Failed!\n");
        return FALSE;
    }

    return TRUE;
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
MS_BOOL Demo_PVR_PlaybackStop(void)
{
    DEMO_PVR_CHECK_INIT();
    DEMO_PVR_CHECK_PB(_eCurPlaybackProgramPath, FALSE)
    if((_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath] != INVALID_RECORD_INDEX)
    && (0 == wcscmp((MS_U16*)_plyprogramInfo[_eCurPlaybackProgramPath].FileName,(MS_U16*)_recprogramInfo[_u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]].FileName)) )
    {
        printf("Error!! Timeshift Mode! %d\n", _u8TimeshiftRecordIdx[_eCurPlaybackProgramPath]);
        return FALSE;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath])==FALSE)
    {
        printf("Already stop playing\n");
        return FALSE;
    }

    if(!MApi_PVR_PlaybackStop(&_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("Fail to playback stop\n");
        return FALSE;
    }

    printf("prepare to switch to live stream\n");
    if(_livePromInfo[_eCurPlaybackProgramPath].bLive)
    {
        printf("switch to live stream!\n");
        if (_Demo_PVR_AV_Start(TRUE, &_livePromInfo[_eCurPlaybackProgramPath]))
        {
            _Demo_PVR_AV_Play();
            printf("switch to live stream success\n");
            _Demo_PVR_LiveInfo_Initialize(_eCurPlaybackProgramPath);
        }
        else
        {
            printf("\n[Demo_PVR_PlaybackStop] Return to Live[%u] failed!!!\n\n",_eCurPlaybackProgramPath);
        }
    }

    memset(&_plyprogramInfo[_eCurPlaybackProgramPath],0,sizeof(PVRProgramInfo_t));
    _u8TotalPlaybackProgram--;

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
MS_BOOL Demo_PVR_PlaybackPause(void)
{
    return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_PAUSE,0,0);
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
MS_BOOL Demo_PVR_PlaybackResume(void)
{
    return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_RESUME,0,0);
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
MS_BOOL Demo_PVR_PlaybackSlowForward(void)
{
    return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_SLOWFORWARD,0,0);
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
MS_BOOL Demo_PVR_PlaybackFastForward(void)
{
    return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_FASTFORWARD,0,0);
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
MS_BOOL Demo_PVR_PlaybackFastBackward(void)
{
    return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_FASTBACKWARD,0,0);
}
//------------------------------------------------------------------------------
/// @brief The sample code to playback set speed
/// @param[in] u32Positive 0: negative 1: positive
/// @param[in] s32Speed 0~64000, which means how many milliseconds original data could be played in 1 second
/// (< FBx1 :  -64000 <= s32Speed < -1000 , >= FFx2 : 2000<= s32Speed <=64000, Pause : 0, Resume : 1,
///     SFx1/32 : 32, SFx1/16: 64, SFx1/8 : 125, SFx1/4 : 250, SFx1/2 : 500)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_PlaybackSetSpeed  \n
///
/// Sample Command:PVR_PlaybackSetSpeed 1 2000
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_PlaybackSetSpeed(MS_U32 *u32Positive, MS_S32 *s32Speed)
{
    return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_SETSPEED,*u32Positive,*s32Speed);
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
MS_BOOL Demo_PVR_PlaybackJumpToTime(MS_U32 *u32Hour,MS_U32 *u32Minute,MS_U32 *u32Second)
{
    MS_U32 u32JumpToTimeInSeconds = (*u32Hour)*60*60+(*u32Minute)*60+(*u32Second);
    DemoPvr_print("Target Time: %2"DTC_MS_U32_d":%2"DTC_MS_U32_d":%2"DTC_MS_U32_d"\n",*u32Hour,*u32Minute,*u32Second);
    DemoPvr_print("Target Time: %"DTC_MS_U32_d" seconds\n",u32JumpToTimeInSeconds);
    MS_BOOL bret = FALSE;

    DEMO_PVR_CHECK_INIT();

    if((*u32Minute)>59)
    {
        printf("The value of a minute should be between 0~59.\n");
        return FALSE;
    }
    if((*u32Second)>59)
    {
        printf("The value of a second should be between 0~59.\n");
        return FALSE;
    }
    if(u32JumpToTimeInSeconds > JumpToTimeMaxRange)
    {
        printf("Time out of range.\n");
        return FALSE;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath])!=TRUE)
    {
        printf("Not to play!\n");
        return FALSE;
    }

    if(_bAutoScriptDbg == TRUE )
    {
        _bAutoScriptJumpToTime = TRUE;
    }
    bret = MApi_PVR_PlaybackJumpToTime(_u8hPlayback[_eCurPlaybackProgramPath],u32JumpToTimeInSeconds);

    return bret;
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
MS_BOOL Demo_PVR_GetFileInfo(char *FileName)
{
    PVRProgramInfo_t promInfo;

    DEMO_PVR_CHECK_INIT();

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

    if(_Demo_PVR_SetProgramInfo(&promInfo,INVALID_PID_ID, INVALID_PID_ID, INVALID_PID_ID, 0, 0,INVALID_PID_ID, 0, FALSE) == FALSE)
    {
        return FALSE;
    }

    MS_U16 *pu16FileName = NULL;

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8*)FileName, pu16FileName, strlen(FileName));
    if(_Demo_PVR_GetFileInfo((char*)pu16FileName, &promInfo)==FALSE)
    {
        printf("GetFileInformation fail!\n");
        return FALSE;
    }

    return TRUE;
}

MS_BOOL _Demo_PVR_SetRecordType(MS_U32 u32RecordType)
{
    MS_U32 u32EngNum = 0;

    if(_bPvrInit)
    {
        printf("Fail to set recordtype due to pvr already initialize!\n");
        return FALSE;
    }

    MApi_DMX_GetCap(DMX_CAP_PVR_ENG_NUM,&u32EngNum);
    if (u32EngNum > PVR_MAX_RECORDING_FILE)
    {
        u32EngNum = PVR_MAX_RECORDING_FILE;
    }

    switch(u32RecordType)
    {
        case 0:
            _enRecordType=EN_APIPVR_RECORD_TYPE_SINGLE;
            _u8TotalRecorder=u32EngNum;
            printf("Set \"Single Recorder\" Successfully[%d]\n",_u8TotalRecorder);
            break;
        case 1:
            printf("Dual Record has been deprecated, not support anymore.\n");
            _enRecordType=EN_APIPVR_RECORD_TYPE_SINGLE;
            _u8TotalRecorder=u32EngNum;
            return FALSE;
        case 2:
#if (DEMO_PVR_SUPPORT_SWPVR_TEST == 1)
            _enRecordType=EN_APIPVR_RECORD_TYPE_SWPVR;
            printf("Set \"SWPVR Recorder\" Successfully\n");
            _u8TotalRecorder=2;
#else
            printf("Set \"SWPVR Recorder\" Failed\n");
            printf("SWPVR not defined\n");
            return FALSE;
#endif
            break;
        default:
            printf("Fail to set due to invalid record type. (%"DTC_MS_U32_u")\n",u32RecordType);
            return FALSE;
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set  record type
/// @param[in] record Type
///         0: single record (support n record, n is define by HW PVR engine capability, max record number is PVR_MAX_RECORDING_FILE )
///         1: dual record (deprecated)
///         2: SWPVR record(support two record(two program) from  source of the same record engine by SW TSP Parser)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecordType \n
///
/// Sample Command:PVR_SetRecordType 1
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecordType(MS_U32 *u32RecordType)
{
    return _Demo_PVR_SetRecordType(*u32RecordType);
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
MS_BOOL Demo_PVR_SetRecIdx(MS_U32 *idx)
{
    DEMO_PVR_CHECK_INIT();
    DEMO_PVR_CHECK_REC_IDX(*idx, FALSE);

    _u8CurRecProgramIdx = (MS_U8) *idx;
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
MS_BOOL Demo_PVR_GetRecIdx(void)
{
    DEMO_PVR_CHECK_INIT();

    printf("Current idx =%d\n",_u8CurRecProgramIdx);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set src id for specific record idx
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecSrcId \n
///
/// Sample Command:PVR_SetRecSrcId 0 1
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecSrcID(MS_U32 *idx, EN_DEMO_DMX_FLT_SOURCEID *peDmxFltSourceID)
{
    DEMO_PVR_CHECK_INIT();

    if(*idx < PVR_MAX_RECORDING_FILE)
    {
        _eCurDMXFltSourceID[*idx] = *peDmxFltSourceID;
        printf("_eCurDMXFltSourceID[%"DTC_MS_U32_u"] = %d\n", *idx, _eCurDMXFltSourceID[*idx]);
        return TRUE;
    }
    else
    {
        printf("Invalid Rec Idx %"DTC_MS_U32_u"\n", *idx);
        return FALSE;
    }
}
//------------------------------------------------------------------------------
/// @brief The sample code to get current src id for specific record idx
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_GetRecSrcId \n
///
/// Sample Command:PVR_GetRecSrcId 0
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_GetRecSrcID(MS_U32 *idx)
{
    DEMO_PVR_CHECK_INIT();

    if(*idx < PVR_MAX_RECORDING_FILE)
    {
        printf("Current source id =%d\n",_eCurDMXFltSourceID[*idx]);
        return TRUE;
    }
    else
    {
        printf("Invalid Rec Idx %"DTC_MS_U32_u"\n", *idx);
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to Set current playback idx
/// @param[in] idx playback idx number  0: Main Path   1: Sub Path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Demo_PVR_SetPlaybackPath \n
///
/// Sample Command:Demo_PVR_SetPlaybackPath 0
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetPlaybackPath(MS_U32 *PlaybackIdx)
{
    DEMO_PVR_CHECK_INIT();
    DEMO_PVR_CHECK_PB(*PlaybackIdx, FALSE);

    if(*PlaybackIdx == 0)
    {
        _eCurPlaybackProgramPath = PVR_PATH_MAIN;
        printf("Set Playback Main Path!\n");
    }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    else if(*PlaybackIdx == 1)
    {
        _eCurPlaybackProgramPath = PVR_PATH_SUB;
        printf("Set Playback Sub Path!\n");
    }
#endif
    else
    {
        printf("[%s][%d]Set Error Playback Path(%d)!\n", __FUNCTION__, __LINE__, _eCurPlaybackProgramPath);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to get current playback Path
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Demo_PVR_GetPlaybackPath \n
///
/// Sample Command:Demo_PVR_GetPlaybackPath
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_GetPlaybackPath(void)
{
    DEMO_PVR_CHECK_INIT();
    if(_eCurPlaybackProgramPath == PVR_PATH_MAIN)
    {
        printf("Current Playback Path : Main Path\n");
    }
    else if(_eCurPlaybackProgramPath == PVR_PATH_SUB)
    {
        printf("Current Playback Path : Sub Path\n");
    }
    else
    {
        printf("[%s][%d]Error Playback Path(%d)!\n", __FUNCTION__, __LINE__, _eCurPlaybackProgramPath);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Set Record FlowSet from live source
/// @param[in] DMX input source (0:E_DMX_FLOW_INPUT_DEMOD0, 1:E_DMX_FLOW_INPUT_DEMOD1...etc)
/// @param[in] DMX playback flow (0:E_DMX_FLOW_LIVE0, 1:E_DMX_FLOW_LIVE1, 2:E_DMX_FLOW_LIVE2, 3:E_DMX_FLOW_LIVE3 others: error setting)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_RecFlowSet \n
/// Set record source from tuner source 0 to playback1 engine input
/// Sample Command: PVR_RecFlowSet 0 1
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_RecFlowSet(EN_DEMO_DMX_FLOW_INPUT* peDmxInput,EN_DEMO_DMX_FLOW* pePlayback)
{
    DEMO_PVR_CHECK_INIT();

    if( ((*peDmxInput) >= E_DMX_FLOW_INPUT_INVALID))
    {
         printf("Invalid EXTunerIdx! Only Tuner0~3 tuner\n");
         return FALSE;
    }
    if( ((*pePlayback) > E_DMX_FLOW_LIVE3))
    {
         printf("Invalid DMXFlow! Only TSIF0~3 TSIF\n");
         return FALSE;
    }

    _eCurDMXInputSrc = *peDmxInput;
    _eCurDMXFlowSet[_u8CurRecProgramIdx] = *pePlayback;

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
MS_BOOL Demo_PVR_PlaybackStepIn(void)
{
    DemoPvr_print("=========  Demo_PVR_PlaybackStepIn =========\n");

    MS_BOOL bRet = FALSE;

    DEMO_PVR_CHECK_INIT();
    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        DemoPvr_Errorprint("USB is not ready!\n");
        return FALSE;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath])!=TRUE)
    {
        printf("Not play!\n");
        return FALSE;
    }

    APIPVR_PLAYBACK_SPEED enPlaybackSpeed = MApi_PVR_PlaybackGetSpeed(_u8hPlayback[_eCurPlaybackProgramPath]);
    if(enPlaybackSpeed == EN_APIPVR_PLAYBACK_SPEED_0X || enPlaybackSpeed == EN_APIPVR_PLAYBACK_SPEED_STEP_IN)
    {
        if( (bRet=MApi_PVR_PlaybackStepIn(_u8hPlayback[_eCurPlaybackProgramPath]))!=TRUE)
        {
            printf("StepIn play failed!\n");
            return FALSE;
        }
    }
    else
    {
        return _Demo_PVR_PlaybackSetSpeed(EN_DEMO_PVR_PLAYBACK_PAUSE,0,0);
    }

    return bRet;
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
MS_BOOL Demo_PVR_PlaybackWithNormalSpeed(void)
{
    DemoPvr_print("=========  Demo_PVR_PlaybackWithNormalSpeed =========\n");
    MS_BOOL bRestart = FALSE;
    DEMO_PVR_CHECK_INIT();
    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        DemoPvr_Errorprint("USB is not ready!\n");
        return FALSE;
    }
    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath])!=TRUE)
    {
        printf("Not play!\n");
        return FALSE;
    }

    if(EN_APIPVR_PLAYBACK_SPEED_STEP_IN == MApi_PVR_PlaybackGetSpeed(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        bRestart = TRUE;
    }

    MApi_PVR_PlaybackSetSpeed(_u8hPlayback[_eCurPlaybackProgramPath],EN_APIPVR_PLAYBACK_SPEED_1X);

    //Fix me!
    if(bRestart)
    {
        _Demo_PVR_AV_Play();
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
MS_BOOL Demo_PVR_CaptureThumbnail(char *filename)
{
    DemoPvr_print("=========  Demo_PVR_CaptureThumbnail =========\n");
    MS_U32 u32capturepts=0;
    MS_PHY phyAddr;
    MS_U32 u32Timestamp;
    MS_U32 u32Width =PVR_THUMBNAIL_CAPTURE_WIDTH;
    MS_U32 u32Height =PVR_THUMBNAIL_CAPTURE_HEIGHT;
    MS_U32 u32VirtualAddr = 0;
    MS_U8 u8FBId =0xff;

    if(strlen(filename)==0)
    {
        printf("Please input file name!\n");
        return FALSE;
    }
    if(!bGOP_init)
    {
        printf("Not initialize GOP!\n");
        return FALSE;

    }
    DEMO_PVR_CHECK_INIT();
    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        DemoPvr_Errorprint("USB is not ready!\n");
        return FALSE;
    }

    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        u32Timestamp = MApi_PVR_GetPlaybackTime(_u8hPlayback[_eCurPlaybackProgramPath]);
    }
    else if(MApi_PVR_IsRecording(_u8hRecord[_u8CurRecProgramIdx]))
    {
        u32Timestamp = MApi_PVR_GetRecordTime(_u8hRecord[_u8CurRecProgramIdx],_u8CurRecProgramIdx);
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
    MApi_GOP_GWIN_GetFBAddr(u8FBId, &phyAddr);
    //printf("[%d]u8FBId=%d ,u32PhysicalAddr=0x%08lx\n",__LINE__,u8FBId,u32PhysicalAddr);

    if(_AVStreamID[_eCurPlaybackProgramPath].vstreamID ==NULL)
    {
        printf("streamID not initialize\n");
        return FALSE;
    }
    if(!MApi_PVR_EX_CaptureVideo((void *)&_AVStreamID[_eCurPlaybackProgramPath],phyAddr, u32Width, u32Height,&u32capturepts))
    {
        MApi_GOP_GWIN_DestroyFB(u8FBId);
        printf("Fail to capture video!\n");
        return FALSE;
    }

    //--------Save file--------
    MS_U16 *pu16FileName = NULL, *pu16FullPathName = NULL;
    MS_U16 u16Delimiter[] = {L'/', L'\0'};

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    pu16FullPathName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);

    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    wcscpy(pu16FullPathName, (MS_U16*)mountPath);
    wcscat(pu16FullPathName, u16Delimiter);
    _U8StringToU16String((MS_U8*)filename, pu16FileName, strlen(filename));
    wcscat(pu16FullPathName, pu16FileName);

    FILE *filePointer = MsFS_Fopen((char*)pu16FullPathName, "w");
    if(filePointer == NULL)
    {
       printf("can't open file\n");
       MApi_GOP_GWIN_DestroyFB(u8FBId);
       _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
       _Demo_PVR_MEMFREE_FUNC((void**)&pu16FullPathName);
       return FALSE;
    }
    else
    {
       u32VirtualAddr = MsOS_PA2KSEG1(phyAddr);
       MsFS_Fwrite((void *)&u32Timestamp, sizeof(u32Timestamp), 1, filePointer);
       MsFS_Fwrite((void *)&u32capturepts, sizeof(u32capturepts), 1, filePointer);
       MsFS_Fwrite((void *)&u32Width, sizeof(u32Width), 1, filePointer);
       MsFS_Fwrite((void *)&u32Height, sizeof(u32Height), 1, filePointer);
       MsFS_Fwrite((void *)u32VirtualAddr, 1,u32Width * u32Height * 2, filePointer);
    }
    MsFS_Fflush(filePointer);
    MsFS_Fclose(filePointer);
    //--------Show capture Video--------
    _Demo_PVR_Show_Capture_Video(phyAddr);
    MApi_GOP_GWIN_DestroyFB(u8FBId);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FullPathName);
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
MS_BOOL Demo_PVR_JumpToThumbnail(char *filename)
{
    DemoPvr_print("=========  Demo_PVR_CaptureThumbnail =========\n");
    MS_U32 u32capturepts=0;
    MS_U32 u32Timestamp;
    MS_S32 s32NonCachedPoolID = 0;
    void * virtualAddr=NULL;
    MS_U32 u32PhysicalAddr;
    MS_U32 u32Width =PVR_THUMBNAIL_CAPTURE_WIDTH;
    MS_U32 u32Height =PVR_THUMBNAIL_CAPTURE_HEIGHT;
    FILE *filePointer;
    MS_BOOL bRet = FALSE;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
    virtualAddr=MsOS_AllocateMemory(PVR_THUMBNAIL_SURFACE_SIZE, s32NonCachedPoolID);
    if(virtualAddr == NULL)
    {
       printf("[%s][%d] \n",__FUNCTION__,__LINE__);
       return FALSE;
    }
    if(strlen(filename)==0)
    {
        printf("Please input file name!\n");
        return FALSE;
    }

    DEMO_PVR_CHECK_INIT();
    char *mountPath = NULL;
    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    if(_Demo_PVR_CheckUSB(mountPath) == FALSE)
    {
        DemoPvr_Errorprint("USB is not ready!\n");
        return FALSE;
    }

    MS_U16 *pu16FileName = NULL, *pu16FullPathName = NULL;
    MS_U16 u16Delimiter[] = {L'/', L'\0'};

    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    pu16FullPathName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    if((pu16FileName == NULL)||(pu16FullPathName == NULL))
    {
        printf("Alloc memory fail\n");
        bRet = FALSE;
        goto END;
    }

    DEMO_PVR_CHECK_MountPath(mountPath, FALSE)
    wcscpy(pu16FullPathName, (MS_U16*)mountPath);
    wcscat(pu16FullPathName, u16Delimiter);
    _U8StringToU16String((MS_U8*)filename, pu16FileName, strlen(filename));
    wcscat(pu16FullPathName, pu16FileName);

    filePointer = MsFS_Fopen((char*)pu16FullPathName, "r");
    if(filePointer == NULL)
    {
       printf("can't open file\n");
       bRet = FALSE;
       goto END;
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
    if(MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        u32PhysicalAddr=MS_VA2PA((MS_U32)virtualAddr);
        _Demo_PVR_Show_Capture_Video(u32PhysicalAddr);
        MApi_PVR_PlaybackJumpToThumbnail(_u8hPlayback[_eCurPlaybackProgramPath],u32Timestamp, u32capturepts);
        MsOS_FreeMemory(virtualAddr,s32NonCachedPoolID);
        bRet = TRUE;
    }

END:
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FullPathName);
    return bRet;
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
MS_BOOL Demo_PVR_HideThumbnail(void)
{
    _Demo_PVR_Remove_Capture_Video();
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
MS_BOOL Demo_PVR_PlaybackABLoop(MS_U32 *u32ABBeginTime,MS_U32 *u32ABEndTime)
{
    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No Play.\n");
        return FALSE;
    }
    return MApi_PVR_EX_PlaybackABLoop(_u8hPlayback[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,*u32ABBeginTime,*u32ABEndTime);
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
MS_BOOL Demo_PVR_PlaybackResetABLoop(void)
{
    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No Play.\n");
        return FALSE;
    }
    return MApi_PVR_EX_PlaybackABLoopReset(_u8hPlayback[_eCurPlaybackProgramPath]);
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
MS_BOOL Demo_PVR_PlaybackAddSkipTime(MS_U32 *u32BeginTimeInSec,MS_U32 *u32EndTimeInSec)
{
    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No Play.\n");
        return FALSE;
    }
    return MApi_PVR_EX_SkipTableAdd(_u8hPlayback[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,*u32BeginTimeInSec,*u32EndTimeInSec);
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
MS_BOOL Demo_PVR_PlaybackRemoveSkipTime(MS_U32 *u32BeginTimeInSec,MS_U32 *u32EndTimeInSec)
{
    MS_U32 u32SkipBeginTime=*u32BeginTimeInSec;
    MS_U32 u32Idx=0;
    MS_U32 u32SkipEndTime=*u32EndTimeInSec;
    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_EX_SkipTableGetIndex(_u8hPlayback[_eCurPlaybackProgramPath],&u32SkipBeginTime,&u32Idx, &u32SkipEndTime))
    {
        printf("Cannot find such interval.\n");
        return FALSE;
    }
    if(!MApi_PVR_EX_SkipTableRemove(_u8hPlayback[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,u32Idx))
    {
        printf("Fail to remove!\n");
        return FALSE;
    }
    if(u32SkipEndTime>*u32EndTimeInSec)
    {
        printf("Still skip %"DTC_MS_U32_u" to %"DTC_MS_U32_u"!\n",*u32EndTimeInSec,u32SkipEndTime);
        MApi_PVR_EX_SkipTableAdd(_u8hPlayback[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,*u32EndTimeInSec,u32SkipEndTime);
    }
    if(*u32BeginTimeInSec>u32SkipBeginTime)
    {
        printf("Still skip %"DTC_MS_U32_u" to %"DTC_MS_U32_u"!\n",u32SkipBeginTime,*u32BeginTimeInSec);
        MApi_PVR_EX_SkipTableAdd(_u8hPlayback[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,u32SkipBeginTime,*u32BeginTimeInSec);
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
MS_BOOL Demo_PVR_SetPlaybackRetentionLimit(MS_U32 *u32RetentionLimitTimeInSec)
{
    MS_U32 u32RetentionLimitTime=*u32RetentionLimitTimeInSec;
    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_EX_SetPlaybackRetentionLimit(_u8hPlayback[_eCurPlaybackProgramPath],_plyprogramInfo[_eCurPlaybackProgramPath].FileName,&u32RetentionLimitTime))
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
MS_BOOL Demo_PVR_PlaybackJumpForward(void)
{
    DEMO_PVR_CHECK_INIT();

    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_PlaybackJumpForward(_u8hPlayback[_eCurPlaybackProgramPath]))
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
MS_BOOL Demo_PVR_PlaybackJumpBackward(void)
{
    DEMO_PVR_CHECK_INIT();

    if(!MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        printf("No to Play.\n");
        return FALSE;
    }
    if(!MApi_PVR_PlaybackJumpBackward(_u8hPlayback[_eCurPlaybackProgramPath]))
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
/// Command: \b PVR_ShowTime \n
///
/// Sample Command:PVR_ShowTime 2 10
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_ShowTime(MS_U32 *u32Type,MS_U32 *u32Set)
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
    else if(*u32Type==3)
    {
        _bShowRecordStartTime=(*u32Set==0)?FALSE:TRUE;
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
/// @sa/// @note
/// Command: \b PVR_RemoveFile \n
///
/// Sample Command:PVR_RemoveFile a.ts
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_RemoveFile(char *fileName)
{
    DEMO_PVR_CHECK_INIT();
    if ((!fileName)||(strlen(fileName)<=0))
    {
        printf("No File Name\n");
        return FALSE;
    }

    MS_U16 *pu16FileName = NULL;
    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    _U8StringToU16String((MS_U8*)fileName, pu16FileName, strlen(fileName));
    if(!MApi_PVR_RemoveFile((char*)pu16FileName))
    {
        printf("Remove file fail!\n");
        _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
        return FALSE;
    }

    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
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
MS_BOOL Demo_PVR_RenameFile(char *fileName,char *newFileName)
{
    MS_BOOL bRet = FALSE;
    MS_U16 *pu16FileName = NULL, *pu16NewFileName = NULL;
    int i = 0;
    DEMO_PVR_CHECK_INIT();


    for(i=0 ; i < MAX_PLAYBACK_NUM ; i++)
    {
        if(MApi_PVR_EX_IsPlaybacking(i) == TRUE)
        {
            printf("Cannot set Playback type during playing\n");
            return FALSE;
        }
    }



    pu16FileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);
    pu16NewFileName = _Demo_PVR_MEMALLOC_FUNC(sizeof(MS_U16)*FILE_PATH_SIZE);

    _U8StringToU16String((MS_U8*)fileName, pu16FileName, strlen(fileName));
    _U8StringToU16String((MS_U8*)newFileName, pu16NewFileName, strlen(newFileName));

    bRet = MApi_PVR_RenameFileName((char*)pu16FileName,(char*)pu16NewFileName);

    _Demo_PVR_MEMFREE_FUNC((void**)&pu16FileName);
    _Demo_PVR_MEMFREE_FUNC((void**)&pu16NewFileName);
    return bRet;
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
MS_BOOL Demo_PVR_SetTimeshiftRecScreenFrozen(const MS_BOOL *bFrozen)
{
    _bScreenFrozen[_eCurPlaybackProgramPath]=*bFrozen;
    printf("Screen Frozen : %s \n",(_bScreenFrozen[_eCurPlaybackProgramPath])?"TRUE":"FALSE");
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
/// Command: \b PVR_SetTimeshiftRecDuration \n
///
/// Sample Command:PVR_SetTimeshiftRecDuration 1
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetTimeshiftRecDuration(MS_U32 *u32PlanRecTimeInMin)
{
    if(_Demo_PVR_IsTimeshiftRecord(_u8CurRecProgramIdx) == TRUE)
    {
        printf("Cannot set TimeShift duration during timeshift recording\n");
        return FALSE;
    }

    printf("TimeShift Rec Time Duration:%"DTC_MS_U32_u"\n",*u32PlanRecTimeInMin);
    return MApi_PVR_SetTimeshiftFileDuration(*u32PlanRecTimeInMin * 60);
}

//------------------------------------------------------------------------------
/// @brief The sample code to set bounded linear record, need to set before record start
///           It is "must option" for bounded linear record setting.
/// @param[in] bBounded bounded linear record or not
///         *bBounded = 1 (bounded linear record)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetBoundedLinearRecord \n
///
/// Sample Command:PVR_SetBoundedLinearRecord 0
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetBoundedLinearRecord(const MS_BOOL *bBounded)
{
    printf("Bounded Linear Record: %d \n",*bBounded);
    PVR_CfgParam stPvrCfgParam = {};
    PVRBoundedLinearRecordInfo_t sBoundedLiearRecordInfo;
    sBoundedLiearRecordInfo.u8RecordIdx= _u8CurRecProgramIdx;
    sBoundedLiearRecordInfo.bBoundedLinearRecord = *bBounded;
    stPvrCfgParam.bSet = TRUE;
    stPvrCfgParam.eCfgCmd = EN_APIPVR_CFG_IS_BOUNDED_LINEAR_RECORD;
    stPvrCfgParam.pCfgPara = &sBoundedLiearRecordInfo;
    _bBoundedLinearRecord = *bBounded;

    return MApi_PVR_Configure(&stPvrCfgParam);
}

//------------------------------------------------------------------------------
/// @brief The sample code to set precision of timeshift duration , need to set before timeshift record start
///           It is "not must option" for timeshift record setting.
/// @param[in] *u32PrecisionInSec  Unit: Second
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecPrecision \n
///
/// Sample Command:PVR_SetRecPrecision 0
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecordPrecision(MS_U32 *u32PrecisionInSec)
{
    printf("[DemoPVR]Record Precision:%"DTC_MS_U32_u" sec\n",*u32PrecisionInSec);
    PVR_CfgParam stPvrCfgParam = {};

    stPvrCfgParam.bSet = TRUE;
    stPvrCfgParam.eCfgCmd = EN_APIPVR_CFG_RECORD_DURATION_PRECISION;
    stPvrCfgParam.pCfgPara = (MS_U32 *)u32PrecisionInSec;

    return MApi_PVR_Configure(&stPvrCfgParam);
}

//------------------------------------------------------------------------------
/// @brief The sample code to set recorded file size of timeshift record.
///           It is "not must option" for timeshift record setting.
/// @param[in] *u32FileSizeInMB  Unit: MB
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecFileSize or PVR_SetTimeshiftRecFileSize \n
///
/// Sample Command:PVR_SetRecFileSize 0 or PVR_SetTimeshiftRecFileSize 0
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecordFileSize(MS_U32 *u32FileSizeInMB)
{
    printf("[DemoPVR]  Rec File Size:%"DTC_MS_U32_u" MB\n", *u32FileSizeInMB);
    _u64FileSizeInKB = (MS_U64)(*u32FileSizeInMB) * 1024;
    if((*u32FileSizeInMB == 0) && MApi_PVR_IsRecording(_u8hRecord[_u8CurRecProgramIdx]))
    {
        return MApi_PVR_SetRecordFileSize(_u8CurRecProgramIdx, _u64FileSizeInKB);
    }
    return TRUE;
}



//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable record append mode.
/// @param[in] *u32Enable  TRUE/FALSE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecordAppend 0 or PVR_SetRecordAppend 1 \n
///
/// Sample Command:PVR_SetRecordAppend 0 or PVR_SetRecordAppend 1
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecordAppend(MS_U32 *u32Enable)
{
    DEMO_PVR_CHECK_INIT();
    printf("[DemoPVR] Record Append : %"DTC_MS_U32_u"\n", *u32Enable);
    if(*u32Enable == 1)
    {
        _bAppend = TRUE;
    }
    else
    {
        _bAppend = FALSE;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to set bounded linear duration, need to set before bounded linear record start
///           It is "not must option" for bounded linear record setting.
///           Bounded linear recording duration is dominated by file size if duration is not setting.
///           If both bounded linear recording file duration and size is set, bounded linear recording file size dominate to which condition first achieve.
/// @param[in] u32PlanRecTimeInMin
///         Bounded Linear Record Time Duration, 10~240 Minutes , Unit: Minutes
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecDuration \n
///
/// Sample Command:PVR_SetRecDuration 10
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecordDuration(MS_U32 *u32PlanRecTimeInMin)
{
    printf("[DemoPVR] Rec Time Duration:%"DTC_MS_U32_u" min\n",*u32PlanRecTimeInMin);
    _u32Duration = *u32PlanRecTimeInMin *60;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set slice record buffer size
///           It is "not must option" for timeshift record setting.
/// @param[in] *u32FileSliceInMB  Unit: MB
/// value of u32FileSliceInMB :max: 4 GB ; min: Record buffer size; default value :0
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetRecFileSlice  \n
///
/// Sample Command:PVR_SetRecFileSlice  10
//------------------------------------------------------------------------------
MS_BOOL Demo_PVR_SetRecordFileSlice(MS_U32 *u32FileSliceInMB)
{
    printf("[DemoPVR]TimeShift Rec File Slice:%"DTC_MS_U32_u" MB\n",*u32FileSliceInMB);
    _u64FileSliceSize = (MS_U64)(*u32FileSliceInMB) * 1024 * 1024;
    printf("[DemoPVR]_u64FileSliceSize File Slice:0x%"DTC_MS_U64_x"  B\n",_u64FileSliceSize);
    return TRUE;
}


MS_BOOL Demo_PVR_GetRecFlowSet(EN_DEMO_DMX_FLOW_INPUT* peDmxInput,EN_DEMO_DMX_FLOW* pePlayback)
{
    DEMO_PVR_CHECK_INIT();

    *peDmxInput = _eCurDMXInputSrc ;
    *pePlayback = _eCurDMXFlowSet[_u8CurRecProgramIdx];

    printf("[%s][%d][DemoPVR] DMX flow input = %d, DMX flow =%d\n",__FUNCTION__,__LINE__,*peDmxInput,*pePlayback);
    return TRUE;
}

MS_BOOL Demo_PVR_GetRecEng(EN_DEMO_DMX_PVR_ENG *peDmxPvrEng)
{
    DEMO_PVR_CHECK_INIT();
    //set DmxFlow
    switch((_u8CurRecProgramIdx%EN_APIPVR_FOUR_TUNER))
    {
        case 0:
            *peDmxPvrEng = E_DMX_PVR_EGN0;
            break;
        case 1:
            *peDmxPvrEng = E_DMX_PVR_EGN1;
            break;
        case 2:
            *peDmxPvrEng = E_DMX_PVR_EGN2;
            break;
        case 3:
            *peDmxPvrEng = E_DMX_PVR_EGN3;
            break;
        default:
            *peDmxPvrEng = E_DMX_PVR_ENG_INVALID;
            printf("[%s][%d]Error!!! the DMX PVR Eng not define(%d)\n",__FUNCTION__, __LINE__,_u8CurRecProgramIdx);
            return FALSE;
    }

    //printf("[%s][%d] _u8CurRecProgramIdx= %u, PvrEng=%ld\n",__FUNCTION__,__LINE__,_u8CurRecProgramIdx,*peDmxPvrEng);
    return TRUE;
}


#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
MS_BOOL Demo_PVR_SeamlessBackRecordStart(void)
{
    MS_U32 u32PCRPid = 0;
    MS_U32 u32VideoPid = 0;
    MS_U32 u32AudioPid = 0;
    MS_U32 u32VCodec = 0;
    MS_U32 u32ACodec = 0;
    MS_U16 u16PmtPid = 0;
    MS_U16 u16LCN = 0;
    MS_BOOL bRet = FALSE;
    // =====================Check if available =========================================
    DEMO_PVR_CHECK_INIT();

    if(_u8hRecord[_u8CurRecProgramIdx]!=APIPVR_INVALID_HANDLER)
    {
        printf("Already record in idx:%d!\n",_u8CurRecProgramIdx);
        return FALSE;
    }
    // =====================Get Live Pid Info. =========================================
    if(_Demo_Dmx_GetLiveProgram((MS_U8)_eCurPlaybackProgramPath, &u32PCRPid, &u32VideoPid, &u32AudioPid, &u32VCodec, &u32ACodec, &u16PmtPid,&u16LCN) == FALSE)
    {
        printf("[%s][%d]Get Live Pid Fail!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    // =====================Background Record Start ======================================
    bRet = _Demo_PVR_BackgroundRecordStart(u32VideoPid, u32AudioPid, u32PCRPid, u32VCodec, u32ACodec, u16PmtPid, u16LCN);
    if(bRet == TRUE)
    {
        printf("[%s][%d]Background Record Start!\n",__FUNCTION__,__LINE__);
    }
    else
    {
        printf("[%s][%d]Background Record Fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    _u8BackProgramIdx = _u8CurRecProgramIdx;
    _bBackgroundRecord[_u8BackProgramIdx] = TRUE;
    printf("\n\n[xxxxxx][%s][%d] _u8CurRecProgramIdx:%u\n",__FUNCTION__,__LINE__,_u8CurRecProgramIdx);

    return TRUE;
}
#endif

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
MS_BOOL Demo_PVR_Help(void)
{
    printf ("Plz refer to DDI test plan\n");
    return TRUE;
}


MS_BOOL Demo_PVR_SetConfigure(MS_U32 *pu32CfgCmd, MS_U32 *pu32Enable)
{
    if( (*pu32Enable != 0) && (*pu32Enable != 1) )
    {
        printf("Please use 1 or 0 to enable/disable Configure\n");
        return FALSE;
    }

    if((EN_APIPVR_PVR_CFG_CMD)(*pu32CfgCmd) == PVR_AUTO_SCRIPT_CMD)
    {
        if(*pu32Enable == 1)
        {
            printf("Auto Script Mode is on\n");
            _bAutoScriptDbg = TRUE;
        }
        else if(*pu32Enable == 0)
        {
            printf("Auto Script Mode is off\n");
            _bAutoScriptDbg = FALSE;
        }
        return TRUE;
    }
    else
    {
        PVR_CfgParam stPvrCfgParam;
        stPvrCfgParam.bSet = (MS_BOOL)(*pu32Enable);
        stPvrCfgParam.eCfgCmd = (EN_APIPVR_PVR_CFG_CMD)(*pu32CfgCmd);
        MApi_PVR_Configure(&stPvrCfgParam);
        return TRUE;
    }
}


MS_BOOL Demo_PVR_SetDBGLevel(PVR_DBG_LEVEL *dbglevel , MS_U32 *intervelTime)
{

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("0:No DEBUG\n");
    printf("Bit0:Main Vdec  Bit1:Sub Vdec\n");
    printf("Bit2:Main Audio Bit3:Sub Audio\n");
    printf("Bit4:DMX Record   Bit5:DMX Play\n");
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

    printf("[DemoPVR]Now DBG LEVEL is %d Intervel:%8"DTC_MS_U32_u"\n",_u8DBGLevel,_u32DBGIntervel);
    if(_u8DBGLevel & 0x4)
    {
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Dump_Dsp_Info, 0x15, (MS_U16)*intervelTime);
    }

    if(_u8DBGLevel > 0 && _pvrDBGTask.iId == INVALID_TASK_ID)
    {
        DemoPvr_print("_u8DBGLevel  : %d\n",_u8DBGLevel);
        if(!_Demo_PVR_DBGCreateTask())
        {
            printf("create PVR DBG task fail!!!!!!!!!!! OH NO \n");
        }
    }
    else if(_u8DBGLevel == 0 && _pvrDBGTask.iId != INVALID_TASK_ID)
    {
        DemoPvr_print("close the pvr debug task\n");
        MS_S32 s32NonCachedPoolID = 0;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);

        MS_U32 u32Events = 0;
        MsOS_WaitEvent(s32_PVRLoopEvent_id, PVR_DBG_LOOP_EVENT, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        printf("[DDI_PVR] Free stack !!!(%"DTC_MS_U32_u")\n",u32Events);
        MsOS_DeleteEventGroup(s32_PVRLoopEvent_id);
        s32_PVRLoopEvent_id = INVALID_EVENT_ID;
#if defined(MSOS_TYPE_ECOS)
        _Demo_PollingThreadStatus(_pvrDBGTask);
        //Polling the status of _pvrDBGTask to make sure it already returned

#endif
        _pvrDBGTask.iId = INVALID_TASK_ID;
        if (_pvrDBGTask.pStack != NULL)
        {
            MsOS_FreeMemory(_pvrDBGTask.pStack,s32NonCachedPoolID);
            _pvrDBGTask.pStack = NULL;
        }
    }
    else
    {
        printf("Skip the setting!!\n");
    }
    return TRUE;
}

static void _Demo_PVRDBG_Task(void)
{
    void *mainvStreamID;
    VDEC_EX_Status vdecMainStatus;
    MS_U32 tempsystime=0, cursystime=0;
    MS_U32 upstc0=0, dwnstc0=0;
    MS_U32 upspcr0=0, dwnpcr0=0;

    MS_PHY phyWrite=0;
    MS_U32 pu32Stamp=0, u32CmdCount=0, u32FileInTS=0, u32TS=0;
    AUDIO_STC_SOURCE stc0_id = E_TSP_0;
    MS_U32 video_fifo0 = 0;
    MS_U32 audio_fifo0 = 0;
    Ex_PVRPlaybackPath  eAVPATH = 0;
    MS_U32 u32stc32 = 0;
    MS_U32 u32stc = 0;
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
    void *subvStreamID;
    VDEC_EX_Status vdecSubStatus;
    MS_U32 upstc1=0, dwnstc1=0;
    MS_U32 upspcr1=0, dwnpcr1=0;
    AUDIO_STC_SOURCE stc1_id = E_TSP_1;
    MS_U32 video_fifo1 = 0;
    MS_U32 audio_fifo1 = 0;

#endif

    while(_u8DBGLevel != 0)
    {
       cursystime = MsOS_GetSystemTime();
       printf("systime :%8"DTC_MS_U32_u"  diff:%8"DTC_MS_U32_u"\n",cursystime,cursystime-tempsystime);
       tempsystime = cursystime;
       //video
        if(_u8DBGLevel & E_PVR_DBG_VDEC_MAIN) //Main stream
        {
            eAVPATH = PVR_PATH_MAIN;
            mainvStreamID = _Demo_PVR_GetVidStreamID(&eAVPATH);
            if (mainvStreamID != NULL)
            {
                memset(&vdecMainStatus, 0, sizeof(VDEC_EX_Status));
                MApi_VDEC_EX_GetStatus((VDEC_StreamId *)mainvStreamID,&vdecMainStatus);
                MApi_DMX_Stc_Eng_Get((DMX_PVR_ENG)_u8CurRecProgramIdx,&u32stc32, &u32stc);
                if(TRUE == vdecMainStatus.bInit)
                {
                    printf("=============Main Video Stream=================\n");
                    printf("VPTS:0x%8"DTC_MS_U32_x" Diff:%"DTC_MS_U32_d"(Seq:0x%8"DTC_MS_U32_x" I:0x%8"DTC_MS_U32_x" 1stF:0x%8"DTC_MS_U32_x") AV(on:0x%8"DTC_MS_U32_x" Start:0x%"DTC_MS_U32_x" Reach:0x%8"DTC_MS_U32_x")\n",
                        MApi_VDEC_EX_GetPTS(mainvStreamID),MApi_VDEC_EX_GetPTS(mainvStreamID)- u32stc / 90,
                        (MS_U32)MApi_VDEC_EX_IsSeqChg(mainvStreamID), (MS_U32)MApi_VDEC_EX_IsIFrameFound(mainvStreamID), (MS_U32)MApi_VDEC_EX_IsFrameRdy(mainvStreamID),
                        (MS_U32)MApi_VDEC_EX_IsAVSyncOn(mainvStreamID), (MS_U32)MApi_VDEC_EX_IsStartSync(mainvStreamID), (MS_U32)MApi_VDEC_EX_IsReachSync(mainvStreamID));
                    printf("ES(R:0x%8"DTC_MS_U32_x" W:0x%8"DTC_MS_U32_x") ( Disp:0x%8"DTC_MS_U32_x" Dec:0x%8"DTC_MS_U32_x" Err:0x%8"DTC_MS_U32_x" Skip:0x%8"DTC_MS_U32_x" Drop:0x%8"DTC_MS_U32_x")\n",MApi_VDEC_EX_GetESReadPtr(mainvStreamID)  ,  MApi_VDEC_EX_GetESWritePtr(mainvStreamID), MApi_VDEC_EX_GetDispCnt(mainvStreamID),
                        MApi_VDEC_EX_GetFrameCnt(mainvStreamID), MApi_VDEC_EX_GetErrCnt(mainvStreamID), MApi_VDEC_EX_GetSkipCnt(mainvStreamID), MApi_VDEC_EX_GetDropCnt(mainvStreamID));

                }
            }
            else
            {
                printf("[%s][%d]Error vdec streamid(main)!!\n",__FUNCTION__, __LINE__);
            }
         }
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
        if(_u8DBGLevel & E_PVR_DBG_VDEC_SUB) // Sub Stream
        {
            eAVPATH = PVR_PATH_SUB;
            subvStreamID = _Demo_PVR_GetVidStreamID(&eAVPATH);
            if (subvStreamID != NULL)
            {
                memset(&vdecSubStatus, 0, sizeof(VDEC_EX_Status));
                MApi_VDEC_EX_GetStatus((VDEC_StreamId *)subvStreamID,&vdecSubStatus);

                if(TRUE == vdecSubStatus.bInit)
                {
                    printf("=============Sub  Video Stream=================\n");
                    printf("VPTS:0x%8"DTC_MS_U32_x" Diff:0x%8llX(Seq:0x%8"DTC_MS_U32_x" I:0x%8"DTC_MS_U32_x" 1stF:0x%8"DTC_MS_U32_x") AV(on:0x%8"DTC_MS_U32_x" Start:0x%8"DTC_MS_U32_x" Reach:0x%8"DTC_MS_U32_x")\n",
                        MApi_VDEC_EX_GetPTS(subvStreamID),MApi_VDEC_EX_GetVideoPtsStcDelta(subvStreamID),
                        (MS_U32)MApi_VDEC_EX_IsSeqChg(subvStreamID), (MS_U32)MApi_VDEC_EX_IsIFrameFound(subvStreamID), (MS_U32)MApi_VDEC_EX_IsFrameRdy(subvStreamID),
                        (MS_U32)MApi_VDEC_EX_IsAVSyncOn(subvStreamID), (MS_U32)MApi_VDEC_EX_IsStartSync(subvStreamID), (MS_U32)MApi_VDEC_EX_IsReachSync(subvStreamID)
                    );
                    printf("ES(R:0x%8"DTC_MS_U32_x" W:0x%8"DTC_MS_U32_x") ( Disp:0x%8"DTC_MS_U32_x" Dec:0x%8"DTC_MS_U32_x" Err:0x%8"DTC_MS_U32_x" Skip:0x%8"DTC_MS_U32_x" Drop:0x%8"DTC_MS_U32_x")\n",MApi_VDEC_EX_GetESReadPtr(subvStreamID)  ,  MApi_VDEC_EX_GetESWritePtr(subvStreamID), MApi_VDEC_EX_GetDispCnt(subvStreamID),
                        MApi_VDEC_EX_GetFrameCnt(subvStreamID), MApi_VDEC_EX_GetErrCnt(subvStreamID), MApi_VDEC_EX_GetSkipCnt(subvStreamID), MApi_VDEC_EX_GetDropCnt(subvStreamID));
                }
            }
            else
            {
                printf("[%s][%d]Error vdec streamid(sub)!!\n",__FUNCTION__, __LINE__);
            }
        }
#endif
        //audio
        if(_u8DBGLevel & E_PVR_DBG_ADEC_MAIN) //Main Audio stream
        {
            printf("=============Main ADEC1 Info=================\n");
            printf("APTS_ms:%8"DTC_MS_U32_u" STCPTS_Diff_ms:%8"DTC_MS_U32_u" okfrmCnt:0x%8"DTC_MS_U32_x" errFrmCnt:0x%8"DTC_MS_U32_x" esBuf_currLevel:0x%8"DTC_MS_U32_x" pcmBuf_currLevel:0x%8"DTC_MS_U32_x"\n",
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS),
                (MS_U32)(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF)/90),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_okFrmCnt),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_errFrmCnt),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_currLevel),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_pcmBuf_currLevel)
            );
        }
#if 0
        if(0)//(_u8DBGLevel & 0x8) //Sub Audio stream
        {
            printf("=============Sub ADEC2 Info=================\n");
            printf("APTS_ms:%8lu STCPTS_Diff_ms:%8lu okfrmCnt:%8lX errFrmCnt:%8lX esBuf_currLevel:%8lX pcmBuf_currLevel:%8lX\n",
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_1ms_timeStamp),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo((Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF)/90),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_okFrmCnt),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_errFrmCnt),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_currLevel),
                (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_pcmBuf_currLevel)
             );
        }
#endif

        //dmx
        if(_u8DBGLevel & E_PVR_DBG_DMX_REC) //DMX record
        {
            printf("=============DMX Record Info=================\n");
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_WriteGet((DMX_PVR_ENG)_u8CurRecProgramIdx,&phyWrite))
            {
                printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__);

            }
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_GetRecordStamp((DMX_PVR_ENG)_u8CurRecProgramIdx,&pu32Stamp))
            {
                printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_GetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__);

            }
            printf("Engine Write:0x%8"DTC_MS_PHY_x" RecordStamp0x%8"DTC_MS_U32_x" \n",phyWrite,pu32Stamp);
        }

        if(_u8DBGLevel & E_PVR_DBG_DMX_PLAY) //DMX play
        {
            printf("=============DMX Playback Info=================\n");
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum((DMX_FILEIN_PATH)_eCurPlaybackProgramPath,&u32CmdCount))
            {
                printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_GetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__);

            }
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Filein_Eng_GetFileInTimeStamp((DMX_FILEIN_PATH)_eCurPlaybackProgramPath,&u32FileInTS))
            {
                printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_GetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__);

            }
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Filein_Eng_GetPlaybackStamp((DMX_FILEIN_PATH)_eCurPlaybackProgramPath,&u32TS))
            {
                printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_GetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__);

            }
            printf("IDLE:%d EmptyNum:0x%8"DTC_MS_U32_x" FileInTS:0x%8"DTC_MS_U32_x" PlaybackTS:0x%8"DTC_MS_U32_x"\n",MApi_DMX_Filein_Eng_IsIdle((DMX_FILEIN_PATH)_eCurPlaybackProgramPath*2),u32CmdCount,u32FileInTS,u32TS);

            if (TRUE != _Demo_PVR_FileIn_Set_STC(PVR_PATH_MAIN, &stc0_id))
            {
                printf("[%s][%d]_Demo_PVR_FileIn_Set_STC(PVR_PATH_MAIN) fail\n",__FUNCTION__, __LINE__);
            }
            if (stc0_id == E_TSP_0)
            {
                MApi_DMX_Stc_Eng_Get(0,&upstc0,&dwnstc0);
                MApi_DMX_Pcr_Eng_Get(0,&upspcr0,&dwnpcr0);
            }
            if (stc0_id == E_TSP_1)
            {
                MApi_DMX_Stc_Eng_Get(1,&upstc0,&dwnstc0);
                MApi_DMX_Pcr_Eng_Get(1,&upspcr0,&dwnpcr0);
            }
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO, &video_fifo0);
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO, &audio_fifo0);

            printf("TSP [STC0:PCR0] => %8"DTC_MS_U32_x":%8"DTC_MS_U32_x"\n",dwnstc0,dwnpcr0);
            printf("VIDEO State: 0x%"DTC_MS_U32_x"  AUDIO State: 0x%"DTC_MS_U32_x"\n", video_fifo0, audio_fifo0);
            printf("VIDEO LEVEL: 0x%"DTC_MS_U32_x"  AUDIO LEVEL: 0x%"DTC_MS_U32_x"\n",
                (MS_U32)((video_fifo0& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT), (MS_U32)((audio_fifo0& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT));
            printf("VIDEO EMPTY: %"DTC_MS_U32_x"  AUDIO EMPTY: %"DTC_MS_U32_x"\n",
                (MS_U32)(video_fifo0&DMX_FIFO_STATUS_LV_EMPTY), (MS_U32)(audio_fifo0&DMX_FIFO_STATUS_LV_EMPTY));
#if (DEMO_PVR_SUPPORT_DUALPLAYBACK_TEST == 1)
            if (TRUE != _Demo_PVR_FileIn_Set_STC(PVR_PATH_SUB, &stc1_id))
            {
                printf("[%s][%d]_Demo_PVR_FileIn_Set_STC(PVR_PATH_SUB) fail\n",__FUNCTION__, __LINE__);
            }
            if (stc1_id == E_TSP_0)
            {
                MApi_DMX_Stc_Eng_Get(0,&upstc1,&dwnstc1);
                MApi_DMX_Pcr_Eng_Get(0,&upspcr1,&dwnpcr1);
            }
            if (stc1_id == E_TSP_1)
            {
                MApi_DMX_Stc_Eng_Get(1,&upstc1,&dwnstc1);
                MApi_DMX_Pcr_Eng_Get(1,&upspcr1,&dwnpcr1);
            }
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO3D, &video_fifo1);
            MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO2, &audio_fifo1);

            printf("TSP [STC1:PCR1] => 0x%8"DTC_MS_U32_x":0x%8"DTC_MS_U32_x"\n",dwnstc1,dwnpcr1);
            printf("VIDE1 State: 0x%"DTC_MS_U32_x"  AUDI1 State: 0x%"DTC_MS_U32_x"\n", video_fifo1, audio_fifo1);
            printf("VIDE1 LEVEL: 0x%"DTC_MS_U32_x"  AUDI1 LEVEL: 0x%"DTC_MS_U32_x"\n",
                (MS_U32)((video_fifo1& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT), (MS_U32)((audio_fifo1& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT));
            printf("VIDE1 EMPTY: %"DTC_MS_U32_x"  AUDI1 EMPTY: %"DTC_MS_U32_x"\n",
                (MS_U32)(video_fifo1& DMX_FIFO_STATUS_LV_EMPTY), (MS_U32)(audio_fifo1&DMX_FIFO_STATUS_LV_EMPTY));
#endif
            //printf("FIFO:AV0 %8lu , %8lu  AV1 %8lu , %8lu",video_fifo0,audio_fifo0,video_fifo1,audio_fifo1);
        }
        printf("---------------------------------------------------------------------------------------------------------------------------------\n");

        MsOS_DelayTask(_u32DBGIntervel);

    }
    MsOS_SetEvent(s32_PVRLoopEvent_id, PVR_DBG_LOOP_EVENT);
    printf("[DDI_PVR] Leave loop\n");
}

static MS_BOOL _Demo_PVR_GetAudioTrackNum(MS_U32* const pu32AudioTrackNum)
{
    APIPVR_PARAM_t stParam;

    if(NULL == pu32AudioTrackNum)
    {
        DemoPvr_Errorprint("[%s][%d] u32AudioTrackNum is NULL!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Get the number of audio tracks.
    memset(&stParam, 0, sizeof(APIPVR_PARAM_t));
    stParam.enInfoType = EN_APIPVR_INFO_NB_AUDIO_TRACK;
    stParam.pParam = pu32AudioTrackNum;
    stParam.u32Paramsize = (MS_U32)sizeof(MS_U32); //Audio track number parameter size is MS_U32.
    if(FALSE == MApi_PVR_PlaybackGetTrackInfoByPara(_u8hPlayback[_eCurPlaybackProgramPath], &stParam, (MS_U32)1))
    {
        DemoPvr_Errorprint("[%s:%d] Get the number of audio track failed!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(*pu32AudioTrackNum <= 0)
    {
        DemoPvr_Errorprint("[%s:%d] The number of audio tracks is wrong(%"DTC_MS_U32_u")!!!\n", __FUNCTION__, __LINE__, *pu32AudioTrackNum);
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_PVR_GetAudioTrackInfo(APIPVRTrackInfo_t *astTrackInfo, const MS_U32 u32AudioTrackNum)
{
    APIPVR_PARAM_t *astParam = NULL;
    MS_U32 u32iCount = 0;

    if(NULL == astTrackInfo)
    {
        DemoPvr_Errorprint("[%s][%d] astTrackInfo is NULL!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    astParam = _Demo_PVR_MEMALLOC_FUNC(sizeof(APIPVR_PARAM_t)*u32AudioTrackNum);
    if(NULL == astParam)
    {
        DemoPvr_Errorprint("[%s][%d] stParam allocate memory failed!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    memset(astParam, 0, sizeof(APIPVR_PARAM_t)*u32AudioTrackNum);
    for(u32iCount = 0; u32iCount < u32AudioTrackNum; u32iCount++)
    {
        astParam[u32iCount].enInfoType = EN_APIPVR_INFO_AUDIO_TRACK_INFO;
        astParam[u32iCount].pParam = &astTrackInfo[u32iCount];
        astParam[u32iCount].u32Paramsize = (MS_U32)sizeof(APIPVRTrackInfo_t);
    }

    if(FALSE == MApi_PVR_PlaybackGetTrackInfoByPara(_u8hPlayback[_eCurPlaybackProgramPath], astParam, u32AudioTrackNum))
    {
        DemoPvr_Errorprint("[%s:%d] MApi_PVR_PlaybackGetTrackInfoByPara() failed!!!\n", __FUNCTION__, __LINE__);
        _Demo_PVR_MEMFREE_FUNC((void**)&astParam);
        return FALSE;
    }

    _Demo_PVR_MEMFREE_FUNC((void**)&astParam);

    return TRUE;
}

MS_BOOL Demo_PVR_ShowAudioTrackInfo(void)
{
    APIPVRTrackInfo_t *astTrackInfo = NULL;
    MS_U32 u32AudioTrackNum = 0;
    MS_U32 u32iCount = 0;
    DemoPvr_print("=========  Demo_PVR_SetAudioTrackByIndex =========\n");

    DEMO_PVR_CHECK_INIT();
    if(TRUE != MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        DemoPvr_Errorprint("Not to play!\n");
        return FALSE;
    }

    //Get the number of audio tracks.
    if(TRUE != _Demo_PVR_GetAudioTrackNum(&u32AudioTrackNum))
    {
        DemoPvr_Errorprint("[%s:%d] Get the number of audio tracks failed!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Get track information of audio tracks.
    astTrackInfo = _Demo_PVR_MEMALLOC_FUNC(sizeof(APIPVRTrackInfo_t)*u32AudioTrackNum);
    if(NULL == astTrackInfo)
    {
        DemoPvr_Errorprint("[%s][%d] astTrackInfo allocate memory failed!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    memset(astTrackInfo, 0, sizeof(APIPVRTrackInfo_t)*u32AudioTrackNum);
    if(FALSE == _Demo_PVR_GetAudioTrackInfo(astTrackInfo, u32AudioTrackNum))
    {
        DemoPvr_Errorprint("[%s:%d] Get the information of audio tracks failed!!!\n", __FUNCTION__, __LINE__);
        _Demo_PVR_MEMFREE_FUNC((void**)&astTrackInfo);
        return FALSE;
    }

    for(u32iCount = 0; u32iCount < u32AudioTrackNum; u32iCount++)
    {
        printf("Audio track [%"DTC_MS_U32_u"], Codec = 0x%X\n", u32iCount, (astTrackInfo[u32iCount].enAVCodecType - EN_APIPVR_AV_CODEC_AUD_NONE));
    }

    _Demo_PVR_MEMFREE_FUNC((void**)&astTrackInfo);

    return TRUE;
}

MS_BOOL Demo_PVR_SetAudioTrackByIndex(MS_U32 *u32AudIdx)
{
    APIPVRTrackInfo_t *astTrackInfo = NULL;
    MS_U32 u32AudioTrackNum = 0;
    DemoPvr_print("=========  Demo_PVR_SetAudioTrackByIndex =========\n");

    DEMO_PVR_CHECK_INIT();
    if(TRUE != MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        DemoPvr_Errorprint("Not to play!\n");
        return FALSE;
    }

    //Get the number of audio tracks.
    if(TRUE != _Demo_PVR_GetAudioTrackNum(&u32AudioTrackNum))
    {
        DemoPvr_Errorprint("[%s:%d] Get the number of audio tracks failed!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(*u32AudIdx > u32AudioTrackNum)
    {
        DemoPvr_Errorprint("[%s:%d] Audio index(%"DTC_MS_U32_u") > Audio track number(%"DTC_MS_U32_u")!!!\n", __FUNCTION__, __LINE__, *u32AudIdx, u32AudioTrackNum);
        return FALSE;
    }

    //Get track information of audio tracks.
    astTrackInfo = _Demo_PVR_MEMALLOC_FUNC(sizeof(APIPVRTrackInfo_t)*u32AudioTrackNum);
    if(NULL == astTrackInfo)
    {
        DemoPvr_Errorprint("[%s][%d] astTrackInfo allocate memory failed!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    memset(astTrackInfo, 0, sizeof(APIPVRTrackInfo_t)*u32AudioTrackNum);
    if(FALSE == _Demo_PVR_GetAudioTrackInfo(astTrackInfo, u32AudioTrackNum))
    {
        DemoPvr_Errorprint("[%s:%d] Get the information of audio tracks failed!!!\n", __FUNCTION__, __LINE__);
        _Demo_PVR_MEMFREE_FUNC((void**)&astTrackInfo);
        return FALSE;
    }

    //Change audio track by index.
    if(FALSE == MApi_PVR_PlaybackTrackChange(_u8hPlayback[_eCurPlaybackProgramPath], &astTrackInfo[*u32AudIdx], (MS_U32)1))
    {
        DemoPvr_Errorprint("[%s:%d] MApi_PVR_PlaybackTrackChange() failed!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    _Demo_PVR_MEMFREE_FUNC((void**)&astTrackInfo);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change display window size and position.
/// @param[in] Width of Display Window
/// @param[in] Height of Display Window
/// @param[in] X Position of Display Window
/// @param[in] Y Position of Display Window
/// @param[in] Defaut size of video
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_ChangeDisplayWindowSize \n
///
/// Sample Command:PVR_ChangeDisplayWindowSize 100 100 50 20 0
//------------------------------------------------------------------------------
#if (DEMO_PVR_SUPPORT_CHANGE_DISPLAYWIN_TEST == 1)
MS_BOOL Demo_PVR_ChangeDisplayWindowSize(MS_U32 *u32Width,MS_U32 *u32Height,MS_U32 *u32XPos,MS_U32 *u32YPos,MS_BOOL *bDef)
{
    DemoPvr_print("=========  Demo_PVR_ChangeDisplayWindowSize =========\n");
    DEMO_PVR_CHECK_INIT();

    if(TRUE != MApi_PVR_EX_IsPlaybacking(_u8hPlayback[_eCurPlaybackProgramPath]))
    {
        DemoPvr_Errorprint("Not to play!\n");
        return FALSE;
    }

    APIPVRDisplayWinInfo_t stDispWinInfo;
    stDispWinInfo.u32Height  = *u32Height;
    stDispWinInfo.u32Width   = *u32Width;
    stDispWinInfo.u32X_Pos   = *u32XPos;
    stDispWinInfo.u32Y_Pos   = *u32YPos;
    stDispWinInfo.bDef = *bDef;
    return MApi_PVR_DispPlayWindowChange(_u8hPlayback[_eCurPlaybackProgramPath],stDispWinInfo);
}
#endif

#if defined(MSOS_TYPE_ECOS)
static void _Demo_PVR_eCos_cpu_usage(cyg_handle_t alarm,cyg_addrword_t data)
{
    MS_U8 u8Index = 0;

    // Accumulate each thread's count into a MS_U64 array,
    // and accumulate total count.
    for(u8Index = 0;u8Index < CYGDBG_STACK_CHECK_TASK_TOTAL_NUM;u8Index++)
    {
        if(TaskUsageInfo[u8Index].count !=0)
        {
            if(TaskNameArray[u8Index] == NULL)
            {
                TaskNameArray[u8Index] = TaskUsageInfo[u8Index].Name;
            }
            TaskUsageSum[u8Index] += TaskUsageInfo[u8Index].count;
            u64TotalSum += TaskUsageInfo[u8Index].count;
        }
    }

    *((MS_U64 *) data) += CPU_USAGE_MEASURE_INTERVAL;

    // Timeout
    // Disable alarm and report result.
    if(*((MS_U64 *) data) >= CPU_USAGE_MEASURE_DURATION * 1000)
    {
        // Set GetUsageActive set to 0, so kernel would stop measuring cpu usage.
        GetUsageActive = 0;
        cyg_alarm_disable(alarm);
        cyg_alarm_delete(alarm);
        bEnabled = FALSE;

        printf("\e[1m\e[32m***  MeasureResult  ***\e[0m\n");
        for(u8Index = 0;u8Index < CYGDBG_STACK_CHECK_TASK_TOTAL_NUM;u8Index++)
        {
            if(TaskUsageSum[u8Index] !=0)
            {
                printf("%s : %"DTC_MS_U64_u".%"DTC_MS_U64_u"%%\n", TaskNameArray[u8Index], TaskUsageSum[u8Index] * 100 / u64TotalSum, TaskUsageSum[u8Index] * 100 % u64TotalSum * 100 / u64TotalSum);
            }
        }
        printf("\e[1m\e[32m*** *** *** *** *** ***\e[0m\n");
    }

    // The counter in TaskUsageInfo is U32.
    // Reset task usage info to prevent it from overflow.
    memset(TaskUsageInfo, 0, CYGDBG_STACK_CHECK_TASK_TOTAL_NUM * sizeof(Task_Usage_Info));

}
#elif defined(MSOS_TYPE_LINUX)
static void _Demo_PVR_Linux_cpu_usage(PVR_CPU_INFO *pInfo)
{
    MS_U32 u32Pos = 0;
    int c = 0;
    memset(stringBuffer, 0, STAT_FILE_BUFFER_LEN);

    do // read one line
    {
        c = fgetc(fpStatFile);
        if(u32Pos == STAT_FILE_BUFFER_LEN - 1)
        {
            break;
        }
        if(c != EOF)
        {
            stringBuffer[u32Pos++] = (char) c;
        }
    }while(c != EOF && c != '\n');

    stringBuffer[u32Pos] = '\0';

    // handle line
    if(strstr(stringBuffer, "cpu"))
    {
        sscanf(stringBuffer, "cpu %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u" %"DTC_MS_U64_u"", &pInfo->u64User, &pInfo->u64Nice, &pInfo->u64System, &pInfo->u64Idle, &pInfo->u64IOWait, &pInfo->u64Irq, &pInfo->u64SoftIrq, &pInfo->u64StealStolen, &pInfo->u64Guest);
        //printf("user %"DTC_MS_U64_u" nice %"DTC_MS_U64_u" system %"DTC_MS_U64_u" idle %"DTC_MS_U64_u" iowait %"DTC_MS_U64_u" irq %"DTC_MS_U64_u" softirq %"DTC_MS_U64_u" stealstolen %"DTC_MS_U64_u" guest %"DTC_MS_U64_u"\n", pInfo->u64User, pInfo->u64Nice, pInfo->u64System, pInfo->u64Idle, pInfo->u64IOWait, pInfo->u64Irq, pInfo->u64SoftIrq, pInfo->u64StealStolen, pInfo->u64Guest);
    }
    else
    {
        printf("[Error] scan stat file error!\n");
    }

    fseek(fpStatFile, 0, SEEK_SET);
    fflush(fpStatFile);
}
#endif

static void _Demo_PVR_PerformanceTest(MS_U32 u32Time)
{
    if(u32Time < CPU_USAGE_START_MEASURE)
    {
        return;
    }
    // Once overrun can be 100% detected, remove this part,
    // and call MApi_PVR_GetReadWritePerformanceReport 0x02 at the end of measurement.
    if((u32Time < u32MeasurementPeriod*60 + CPU_USAGE_START_MEASURE) && !bEnabled)
    {
        MS_U8 u8Index = 0;
        for(u8Index = 0;u8Index < _u8TotalRecorder;u8Index++)
        {
            printf("MApi_PVR_GetReadWritePerformanceReport u8Index = %u\n", u8Index);
            MApi_PVR_GetReadWritePerformanceReport(0, u8Index, 0x82);
        }
    }

    #if defined(MSOS_TYPE_LINUX)
    if((u32Time < u32MeasurementPeriod*60 + CPU_USAGE_START_MEASURE) && !bEnabled)
    {
        if(fpStatFile == NULL)
        {
            fpStatFile = fopen("/proc/stat", "r");
        }
        if(fpStatFile == NULL)
        {
            printf("[PVR] open Error!(Errno:%d) %s \n", errno, strerror(errno));
            printf("[Error] open /proc/stat fail!\n");
        }
        else
        {
            printf("\e[1m\e[32m***  Measure Start  ***\e[0m\n");
            memset(&StartCPUTime, 0, sizeof(PVR_CPU_INFO));
            memset(&EndCPUTime, 0, sizeof(PVR_CPU_INFO));
            bEnabled = TRUE;
            _Demo_PVR_Linux_cpu_usage(&StartCPUTime);
        }
    }
    if((u32Time >= u32MeasurementPeriod*60 + CPU_USAGE_START_MEASURE) && bEnabled && fpStatFile)
    {
        _Demo_PVR_Linux_cpu_usage(&EndCPUTime);
        PVR_CPU_INFO DiffCPUTime = {};
        DiffCPUTime.u64User = EndCPUTime.u64User - StartCPUTime.u64User;
        DiffCPUTime.u64Nice = EndCPUTime.u64Nice - StartCPUTime.u64Nice;
        DiffCPUTime.u64System = EndCPUTime.u64System - StartCPUTime.u64System;
        DiffCPUTime.u64Idle = EndCPUTime.u64Idle - StartCPUTime.u64Idle;
        DiffCPUTime.u64IOWait= EndCPUTime.u64IOWait - StartCPUTime.u64IOWait;
        DiffCPUTime.u64Irq = EndCPUTime.u64Irq - StartCPUTime.u64Irq;
        DiffCPUTime.u64SoftIrq= EndCPUTime.u64SoftIrq - StartCPUTime.u64SoftIrq;
        DiffCPUTime.u64StealStolen= EndCPUTime.u64StealStolen - StartCPUTime.u64StealStolen;
        DiffCPUTime.u64Guest= EndCPUTime.u64Guest - StartCPUTime.u64Guest;
        //printf("diff user %"DTC_MS_U64_u" nice %"DTC_MS_U64_u" system %"DTC_MS_U64_u" idle %"DTC_MS_U64_u" iowait %"DTC_MS_U64_u" irq %"DTC_MS_U64_u" softirq %"DTC_MS_U64_u" stealstolen %"DTC_MS_U64_u" guest %"DTC_MS_U64_u"\n", DiffCPUTime.u64User, DiffCPUTime.u64Nice, DiffCPUTime.u64System, DiffCPUTime.u64Idle, DiffCPUTime.u64IOWait, DiffCPUTime.u64Irq, DiffCPUTime.u64SoftIrq, DiffCPUTime.u64StealStolen, DiffCPUTime.u64Guest);
        MS_U64 u64TotalTime = DiffCPUTime.u64User + DiffCPUTime.u64Nice + DiffCPUTime.u64System + DiffCPUTime.u64Idle + DiffCPUTime.u64IOWait + DiffCPUTime.u64Irq + DiffCPUTime.u64SoftIrq + DiffCPUTime.u64StealStolen + DiffCPUTime.u64Guest;
        MS_U64 u64NonIdleTime = u64TotalTime - DiffCPUTime.u64Idle;

        printf("\e[1m\e[32m***  MeasureResult  ***\e[0m\n");

        printf("Total : %"DTC_MS_U64_u".%"DTC_MS_U64_u"%%\n", u64NonIdleTime * 100 / u64TotalTime, u64NonIdleTime * 100 % u64TotalTime * 100 / u64TotalTime);
        printf("User : %"DTC_MS_U64_u".%"DTC_MS_U64_u"%%\n", DiffCPUTime.u64User * 100 / u64TotalTime, DiffCPUTime.u64User * 100 % u64TotalTime * 100 / u64TotalTime);
        printf("System : %"DTC_MS_U64_u".%"DTC_MS_U64_u"%%\n", DiffCPUTime.u64System * 100 / u64TotalTime, DiffCPUTime.u64System * 100 % u64TotalTime * 100 / u64TotalTime);
        printf("IOWait : %"DTC_MS_U64_u".%"DTC_MS_U64_u"%%\n", DiffCPUTime.u64IOWait * 100 / u64TotalTime, DiffCPUTime.u64IOWait * 100 % u64TotalTime * 100 / u64TotalTime);
        printf("Idle : %"DTC_MS_U64_u".%"DTC_MS_U64_u"%%\n", DiffCPUTime.u64Idle * 100 / u64TotalTime, DiffCPUTime.u64Idle * 100 % u64TotalTime * 100 / u64TotalTime);

        printf("\e[1m\e[32m*** *** *** *** *** ***\e[0m\n");

        if(fpStatFile != NULL)
        {
            fclose(fpStatFile);
            fpStatFile = NULL;
        }
        bEnabled = FALSE;
    }
    #elif defined(MSOS_TYPE_ECOS)
    if((u32Time < u32MeasurementPeriod*60 + CPU_USAGE_START_MEASURE) && !bEnabled)
    {
        MS_U32 u32Index = 0;
        for(u32Index = 0;u32Index < CYGDBG_STACK_CHECK_TASK_TOTAL_NUM;u32Index++)
        {
            TaskNameArray[u32Index] = NULL;
            TaskUsageSum[u32Index] = 0;
        }
        u64TotalSum = 0;

        MS_U64 u64MeasureDuration = 0;
        cyg_handle_t counter;
        cyg_clock_to_counter(cyg_real_time_clock(),&counter);
        cyg_handle_t alarmH;
        // Create alarm.
        // _Demo_PVR_eCos_cpu_usage is the alarm function.
        // u64MeasureDuration is passed to alarm function and used as a timer to indicate the ending of measurement.
        cyg_alarm_create(counter, _Demo_PVR_eCos_cpu_usage, (cyg_addrword_t)&u64MeasureDuration, &alarmH, &alarm_s);

        // The first time _Demo_PVR_eCos_cpu_usage being executed would be cyg_current_time() + CPU_USAGE_MEASURE_INTERVAL,
        // and would be continually executed in every CPU_USAGE_MEASURE_INTERVAL ms.
        cyg_alarm_initialize(alarmH, cyg_current_time() + CPU_USAGE_MEASURE_INTERVAL, CPU_USAGE_MEASURE_INTERVAL);
        cyg_alarm_enable(alarmH);
        // When GetUsageActive set to 1, kernel would start measuring cpu usage and record results in TaskUsageInfo.
        // Please see src/sched/sche.cxx in ecospro
        GetUsageActive = 1;
        printf("\e[1m\e[32m***  Measure Start  ***\e[0m\n");
        bEnabled = TRUE;
    }
    #endif
}
//------------------------------------------------------------------------------
/// @brief The sample code to record a live stream in timeshift mode
/// @param[in] Enable/Disable the performance test
/// @param[in] period to measure the performance
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b PVR_SetPerformanceTest \n
///
/// Sample Command:PVR_SetPerformanceTest 1 30

MS_BOOL Demo_PVR_SetPerformanceTest(MS_U32 *u32Enable, MS_U32 *u32MeasurementTime)
{
    if(*u32Enable == 1)
    {
        bPVRPerformanceTest = TRUE;
    }
    else
    {
        bPVRPerformanceTest = FALSE;
    }
    if(*u32MeasurementTime > 0)
    {
        u32MeasurementPeriod = *u32MeasurementTime;
    }
    return TRUE;
}

static MS_BOOL _Demo_PVR_SetSyncConfig(MS_U32 *u32Value, char *DirtyBackgroundRatio, char *DirtyRatio, char *DirtyExpireCentisecs, char *DirtyWritebackCentisecs)
{
    PVR_CfgParam stPvrCfgParam;
    stPvrCfgParam.bSet = TRUE;
    stPvrCfgParam.eCfgCmd = EN_APIPVR_CFG_FILE_SYNC_CONFIG;
    stPvrCfgParam.pCfgPara = u32Value;
    if(MApi_PVR_Configure(&stPvrCfgParam) == FALSE)
    {
        return FALSE;
    }

    #if defined(MSOS_TYPE_LINUX)
    // Default values:
    // vm.dirty_background_bytes = 0
    // vm.dirty_background_ratio = 10
    // vm.dirty_bytes = 0
    // vm.dirty_expire_centisecs = 3000
    // vm.dirty_ratio = 20
    // vm.dirty_writeback_centisecs = 500

    printf("input paramaters===\nDirtyBackgroundRatio = %s\nDirtyRatio = %s\nDirtyExpireCentisecs = %s\nDirtyWritebackCentisecs = %s\n===\n", DirtyBackgroundRatio, DirtyRatio, DirtyExpireCentisecs, DirtyWritebackCentisecs);

    char tempDirtyBackgroundRatio[MAX_PROC_BUFFER_SIZE] = {0};
    char tempDirtyRatio[MAX_PROC_BUFFER_SIZE] = {0};
    char tempDirtyExpireCentisecs[MAX_PROC_BUFFER_SIZE] = {0};
    char tempDirtyWritebackCentisecs[MAX_PROC_BUFFER_SIZE] = {0};

    // (1) dirty_background_ratio
    char getCmd[MAX_CMD_BUFFER_SIZE] = {0};
    strncpy(getCmd, "cat /proc/sys/vm/dirty_background_ratio", sizeof(getCmd)-1);

    char setCmd[MAX_CMD_BUFFER_SIZE] = {0};
    snprintf(setCmd, sizeof(setCmd)-1, "echo %s > /proc/sys/vm/dirty_background_ratio\n", DirtyBackgroundRatio);

    _ExecuteCommand(getCmd, origDirtyBackgroundRatio, MAX_PROC_BUFFER_SIZE);
    printf("orig dirty_background_ratio = %s\n", origDirtyBackgroundRatio);
    _ExecuteCommand(setCmd, NULL, 0);
    _ExecuteCommand(getCmd, tempDirtyBackgroundRatio, MAX_PROC_BUFFER_SIZE);
    printf("new dirty_background_ratio = %s\n", tempDirtyBackgroundRatio);

    // (2) dirty_ratio
    strncpy(getCmd, "cat /proc/sys/vm/dirty_ratio", sizeof(getCmd)-1);
    snprintf(setCmd, sizeof(setCmd)-1, "echo %s > /proc/sys/vm/dirty_ratio\n", DirtyRatio);

    _ExecuteCommand(getCmd, origDirtyRatio, MAX_PROC_BUFFER_SIZE);
    printf("orig dirty_ratio = %s\n", origDirtyRatio);
    _ExecuteCommand(setCmd, NULL, 0);
    _ExecuteCommand(getCmd, tempDirtyRatio, MAX_PROC_BUFFER_SIZE);
    printf("new dirty_ratio = %s\n", tempDirtyRatio);

    // (3) dirty_expire_centisecs
    strncpy(getCmd, "cat /proc/sys/vm/dirty_expire_centisecs", sizeof(getCmd)-1);
    snprintf(setCmd, sizeof(setCmd)-1, "echo %s > /proc/sys/vm/dirty_expire_centisecs\n", DirtyExpireCentisecs);

    _ExecuteCommand(getCmd, origDirtyExpireCentisecs, MAX_PROC_BUFFER_SIZE);
    printf("orig dirty_expire_centisecs = %s\n", origDirtyExpireCentisecs);
    _ExecuteCommand(setCmd, NULL, 0);
    _ExecuteCommand(getCmd, tempDirtyExpireCentisecs, MAX_PROC_BUFFER_SIZE);
    printf("new dirty_expire_centisecs = %s\n", tempDirtyExpireCentisecs);

    // (4) dirty_writeback_centisecs
    strncpy(getCmd, "cat /proc/sys/vm/dirty_writeback_centisecs",  sizeof(getCmd)-1);
    snprintf(setCmd, sizeof(setCmd)-1, "echo %s > /proc/sys/vm/dirty_writeback_centisecs\n", DirtyWritebackCentisecs);

    _ExecuteCommand(getCmd, origDirtyWritebackCentisecs, MAX_PROC_BUFFER_SIZE);
    printf("orig dirty_writeback_centisecs = %s\n", origDirtyWritebackCentisecs);
    _ExecuteCommand(setCmd, NULL, 0);
    _ExecuteCommand(getCmd, tempDirtyWritebackCentisecs, MAX_PROC_BUFFER_SIZE);
    printf("new dirty_writeback_centisecs = %s\n", tempDirtyWritebackCentisecs);

    #endif

    return TRUE;
}

static EN_AV_Device _PVRPlaybackPath_Mapping_AV_Device(Ex_PVRPlaybackPath enPVRPlaybackPath)
{
    switch(enPVRPlaybackPath)
    {
        case PVR_PATH_MAIN:
            return E_AV_DEVICE_MAIN;
        case PVR_PATH_SUB:
            return  E_AV_DEVICE_SUB;
        default:
            printf("Wrong PVR Playback Path%d\n", enPVRPlaybackPath);
            return E_AV_DEVICE_MAIN;
    }
}

// For eCos:
// u32Value > 0: adjust the synchronization interval.
// u32Value = 0: old code flow. synchronize after every write.
// DirtyBackgroundRatio, DirtyRatio, DirtyExpireCentisecs and DirtyWritebackCentisecs are unused.
//
// For Linux:
// u32Value > 0: disable the synchronization in PVR MW.
// u32Value = 0: enable the synchronization in PVR MW.
// If u32Value > 0,
// users can tune DirtyBackgroundRatio, DirtyRatio, DirtyExpireCentisecs and DirtyWritebackCentisecs values.
// This demo cmd would set the users-tuned values to procfs, and recover them to default values when pvr exit.
MS_BOOL Demo_PVR_SetSyncConfig(MS_U32 *u32Value, char *DirtyBackgroundRatio, char *DirtyRatio, char *DirtyExpireCentisecs, char *DirtyWritebackCentisecs)
{
    if(bSyncConfigHasBeenSet)
    {
        printf("Cannot set sync config more than once!\n");
        return FALSE;
    }
    if(_Demo_PVR_SetSyncConfig(u32Value, DirtyBackgroundRatio, DirtyRatio, DirtyExpireCentisecs, DirtyWritebackCentisecs) == FALSE)
    {
        printf("Set sync config fail!\n");
        return FALSE;
    }
    bSyncConfigHasBeenSet = TRUE;

    return TRUE;
}

MS_BOOL Demo_PVR_Disable2ndEncryption(MS_U32 *u32Disable)
{
    if(_bPvrInit)
    {
        printf("Cannot Disable/Enable 2nd Encryption after PVR init!\n");
        return FALSE;
    }
    else
    {
        if(*u32Disable == 1)
        {
            b2ndEncryptionEnabled = FALSE;
            printf("2nd Encryption Disabled!\n");
        }
        else if(*u32Disable == 0)
        {
            b2ndEncryptionEnabled = TRUE;
            printf("2nd Encryption Enabled!\n");
        }
        else
        {
            printf("Unknown 2nd Encryption Config!\n");
        }
        return TRUE;
    }
}

#endif /* DEMO_PVR_TEST */
