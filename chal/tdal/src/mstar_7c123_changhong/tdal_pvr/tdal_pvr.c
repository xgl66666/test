/*
 * tdal_pvr.c
 *
 *  Created on: Mar 7, 2013
 *      Author: rtrk
 */
/********************************************************
   *   Includes                        *
********************************************************/
#include "crules.h"
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

#include "MsTypes.h"
#include "MsMemory.h"
#include "apiPVR.h"
#include "apiDMX.h"
#include "drvDSCMB.h"
#include "drvBDMA.h"
#include "MsFS.h"
#include "apiDAC.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiHDMITx.h"
#include "apiDMX.h"
#include "apiVDEC_EX.h"
#include "apiGOP.h"
#include "apiGFX.h"
#include "apiPNL.h"
#include "PVRPL_Record.h"

#include "tdal_common.h"
#include "tdal_dmx.h"
#include "tdal_av.h"
#include "tdal_disp.h"
#include "tbox.h"
#include "tdal_pvr.h"
#include "tdal_pvr_p.h"

/********************************************************
   *   Defines                        *
********************************************************/
#define MAX_PLAYBACK_NUM 2
#ifndef NARGA_CAVID
#define NARGA_CAVID    (0x02)
#endif

//#define ENABLE_CAPVR
/********************************************************
   *   Macros                        *
********************************************************/
mTBOX_SET_MODULE(eTDAL_PVR_EM);
/********************************************************
   *   Local   File   Variables   (LOCAL)            *
********************************************************/
APIPVR_PLAYBACK_TYPE eTDAL_PVR_PlayType = EN_APIPVR_PLAYBACK_TYPE_BASICPLAYER;
APIPVR_RECORD_TYPE   eTDAL_PVR_RecordType = EN_APIPVR_RECORD_TYPE_SINGLE;

LOCAL char          pxcMounthPath[20];
char                mountPath[72] = {0};

LOCAL bool          gTDAL_PVR_IsInitialized = FALSE;
LOCAL PVR_FSINFO    TDAL_PVR_recFSInfo[PVR_MAX_RECORDING_FILE];

LOCAL MS_U8         TDAL_PVR_u8hRecord[PVR_MAX_RECORDING_FILE]={APIPVR_INVALID_HANDLER};
LOCAL uint8_t       TDAL_PVRi_PCRFltId   = INVALID_FILTER_ID;
LOCAL uint8_t       TDAL_PVRi_VideoFltId = INVALID_FILTER_ID;
LOCAL uint8_t       TDAL_PVRi_AudioFltId = INVALID_FILTER_ID;
LOCAL bool          TDAL_PVRi_ScreenFrozen = TRUE;
LOCAL MS_S32        TDAL_PVRi_QueueID;
LOCAL MS_U8         TDAL_PVRi_TaskStack[0x4000];
LOCAL TDAL_task_id  TDAL_PVRi_TaskHandle;
LOCAL bool          TDAL_PVRi_EndTask = FALSE;
LOCAL bool          TDAL_PVRi_TaskFinished = TRUE;


LOCAL MS_U32        TDAL_PVRi_ProgIdx = 0;
LOCAL char          CurRecordedFileName[FILE_PATH_SIZE] = {0};

LOCAL PVRProgramInfo_t  TDAL_PVR_recprogramInfo[PVR_MAX_RECORDING_FILE];
LOCAL PVRProgramInfo_t  TDAL_PVR_plyprogramInfo;
LOCAL PVRProgramInfo_t  TDAL_PVR_livePromInfo;
LOCAL PVRProgramInfo_t  TDAL_PVR_bgrecPromInfo;
LOCAL uint8_t pvrbgRecHandle = 0;
LOCAL uint8_t progbgIdx = 0;
LOCAL tTDAL_DMX_ChannelId VideoChannel = 0xFF;
LOCAL tTDAL_DMX_ChannelId AudioChannel = 0xFF;
LOCAL tTDAL_DMX_ChannelId PCRChannel = 0xFF;

LOCAL tTDAL_PVR_Desc    TDAL_PVR_Desc[PVR_MAX_RECORDING_FILE];

LOCAL uint8_t           TDAL_PVRi_FreeSpaceThreshold = 0; //100% will be used

TDAL_mutex_id           TDAL_PVRi_Mutex = NULL;
uint64_t        u64FreeSpaceInKB = 0, u64TotalSpaceInKB = 0;
static const VDEC_StreamId _NullVidStreamID = {0};
static const AUDIO_DEC_ID  _NullAudStreamID = AU_DEC_INVALID;
extern VDEC_StreamId stStreamId;

//For Unplugging USB
static MS_BOOL _bUplugMsg=FALSE;
static char _moutPath[FILE_PATH_SIZE]={0};
static MS_BOOL _ReturnToLive = FALSE;
tTDAL_PVR_Rec_Filters PVRRecFilters[128];

LOCAL MS_U32 TDAL_PVR_playbackTime = 0;
LOCAL MS_U32 TDAL_PVR_previousChunksTime = 0;
MS_BOOL bIsSeamlessPlayback = FALSE;
MS_U32 _u32SeamlessStartTime = 0;
static MS_BOOL _bIsPlaySwitchChunk = FALSE;
/********************************************************
   *   Module   variables   (MODULE)               *
********************************************************/
typedef struct PVR_AV_ID_s
{
    void *vstreamID;
    void *astreamID;

}PVR_AV_ID_t;

typedef enum
{
    PVR_PATH_MAIN,
    PVR_PATH_SUB,
    PVR_PATH_INVALID,
} Ex_PVRPlaybackPath;


/********************************************************
   *   Functions   Definitions   (LOCAL/GLOBAL)        *
********************************************************/
//GLOBAL bool TDAL_MPi_ReleasePool(void);
IMPORT bool TDAL_AV_AudioStarted(void);
IMPORT bool TDAL_AV_VideoStarted(void);
bool TDAL_PVR_IsPlaybacking(void);
LOCAL APIPVR_CODEC_TYPE TDAL_PVRi_VDEC_TDAL2MS(uint32_t u32VCodec);
LOCAL tTDAL_AV_VideoType TDAL_PVRi_VDEC_MS2TDAL(uint32_t u32VCodec);
LOCAL APIPVR_AUD_CODEC_TYPE TDAL_PVRi_ADEC_TDAL2MS(uint32_t u32ACodec);
LOCAL tTDAL_AV_AudioType    TDAL_PVRi_ADEC_MS2TDAL(uint32_t tdal_ACodec);
LOCAL bool TDAL_PVRi_InsertFilter(PVRFilterInfo_t *pFilter, MS_U16 u16PID, MS_U32 u32DmxFltType);
LOCAL uint16_t TDAL_PVRi_FindPidInFilter(PVRFilterInfo_t *pFilter, MS_U32 u32DmxFltType);
LOCAL bool TDAL_PVRi_GetRecMountPath(bool bTimeshift, char *mountpath);
LOCAL void TDAL_PVRi_SetProgInfo(PVRProgramInfo_t *pProgInfo, void *pInfo);
LOCAL void TDAL_PVRi_ResetProgInfo(PVRProgramInfo_t *pProgInfo);
LOCAL void TDAL_PVRi_GetProgInfo(PVRProgramInfo_t *pProgInfo, char* fileName);
LOCAL void TDAL_PVRi_Task(MS_U32 argc, void *argv);
LOCAL void TDAL_PVRi_Timeshift_PlaybackStop(uint8_t pvrHandle);
LOCAL void TDAL_PVRi_Callback(PVREventInfo_t *event, tTDAL_PVR_Handle pvrHandle);
void TDAL_PVRi_EnableRouteForPlayback(int flow, int pInSrc, int pClkInv, int pExtSync, int pParal, tTDAL_ProgInfoElem *pProgInfoElem);
LOCAL void TDAL_PVRi_DisableRouteForPlayback(void);
LOCAL APIPVR_PLAYBACK_SPEED TDAL_PVRi_TrickModeConv(tTDAL_PVR_Speed enSpeed);
uint64_t TDAL_PVRi_GetSizeOfContent(tTDAL_PVR_Desc *pPVRDesc, char *FullRecordedFileName);
uint32_t TDAL_PVRi_GetDurationOfContent(tTDAL_PVR_Desc *pPVRDesc, char *FullRecordedFileName);
void TDAL_PVRi_GetDurationAndSizeContent(tTDAL_PVR_Desc *pPVRDesc, char *FullRecordedFileName);
LOCAL tTDAL_PVR_Speed TDAL_PVRi_TrickModeConvBack(APIPVR_PLAYBACK_SPEED enSpeed);
LOCAL tTDAL_PVR_Callback TDAL_PVR_Notification; 
LOCAL void TDAL_PVRi_ControlChannel_TDAL_DMX(bool enable);
void TDAL_PVRi_PlaybackPreviousChunk(tTDAL_PVR_Desc *pPVRDesc, uint16_t curFileIdx);

static DMX_FLOW_INPUT _eCurDMXInputSrc=DMX_FLOW_INPUT_DEMOD;
static DMX_TSIF _eCurDMXFlowSet[PVR_MAX_RECORDING_FILE]={DMX_TSIF_LIVE0};
static Ex_PVRPlaybackPath PVRPlaybackPath = PVR_PATH_INVALID;
static MS_BOOL _bScreenFrozen[MAX_PLAYBACK_NUM];

/********************************************************
   *   Demo Functions   Definitions   (LOCAL/GLOBAL)        *
********************************************************/
MS_BOOL Demo_PVR_USB_UnplugEvent(char *moutPath)
{
    PVREventInfo_t event_info;
    memset((void *)&event_info,0,sizeof(PVREventInfo_t));

    if(gTDAL_PVR_IsInitialized)
    {
        if(strstr(MApi_PVR_GetMouthPath(),moutPath)!=NULL)
        {
            event_info.pvrEvent = EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE;
            _bUplugMsg=TRUE;
            strcpy(_moutPath,moutPath);
            MsOS_SendToQueue(TDAL_PVRi_QueueID, (MS_U8 *)&event_info, sizeof(PVREventInfo_t), 1000);
            //printf("send to Queue <2> _u8TimeshiftIdx=%d\n",_u8TimeshiftIdx[_eCurPlaybackProgramPath]);
            return TRUE;
        }
    }
    return FALSE;
}

bool Demo_PVR_GetRecFlowSet(int* peDmxInput,int* pePlayback)
{

    *peDmxInput = _eCurDMXInputSrc ;
    *pePlayback = DMX_TSIF_LIVE0;

    printf("[%s][%d] DMX flow input = %d, DMX flow =%d\n",__FUNCTION__,__LINE__,*peDmxInput,*pePlayback);
    return TRUE;
}
bool Demo_PVR_GetRecEng(int *peDmxPvrEng)
{
    (*peDmxPvrEng) = 0;
    return TRUE;
}

LOCAL bool TDAL_PVRi_ShowProgramInfo(tTDAL_PVR_Handle pvrHandle)
{
    bool error = TRUE;
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    
    if (!gTDAL_PVR_IsInitialized)
    {
        error = FALSE;
        return error;
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = FALSE;
        return error;
    }
    
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }

    printf("bIsCaptured = %d\n",pPVRDesc->bIsCaptured);
    printf("bIsTimeshift = %d\n",pPVRDesc->bIsTimeshift);
    printf("bUsed = %d\n",pPVRDesc->bUsed);
    printf("eCurrentSpeed = %lu\n",pPVRDesc->eCurrentSpeed);
    printf("FilesCount = %d\n",pPVRDesc->FilesCount);
    printf("progIdx = %d\n",pPVRDesc->progIdx);
    printf("pvrPlyHandle = %d\n",pPVRDesc->pvrPlyHandle);
    printf("pvrRecHandle = %d\n",pPVRDesc->pvrRecHandle);
    printf("u32FullRecDuration = %lu\n",pPVRDesc->u32FullRecDuration);
    printf("u64FreeSpaceInKB = %llu\n",pPVRDesc->u64FreeSpaceInKB);
    printf("u64FullRecSizeInKB = %llu\n",pPVRDesc->u64FullRecSizeInKB);

    printf("ProgInfoElem.u16VideoPID = %ld\n",pPVRDesc->ProgInfoElem.u16VideoPID);
    printf("ProgInfoElem.u16AudioPID = %ld\n",pPVRDesc->ProgInfoElem.u16AudioPID);
    printf("ProgInfoElem.u16PCRPID = %ld\n",pPVRDesc->ProgInfoElem.u16PCRPID);
    printf("ProgInfoElem.u16SubsPID = %ld\n",pPVRDesc->ProgInfoElem.u16SubsPID);

    printf("u32VideoPid     : %d\n",TDAL_PVRi_FindPidInFilter(pPVRDesc->pPVRProgInfo->Filters, DMX_FILTER_TYPE_VIDEO));
    printf("u32PCRPid       : %d\n",TDAL_PVRi_FindPidInFilter(pPVRDesc->pPVRProgInfo->Filters, DMX_FILTER_TYPE_PCR));
    printf("u32AudioPid     : %lu\n",pPVRDesc->pPVRProgInfo->u16AudioPid);
    printf("u32PmtPid       : %lu\n",TDAL_PVRi_FindPidInFilter(pPVRDesc->pPVRProgInfo->Filters, DMX_FILTER_TYPE_SECTION));
    printf("enVCodec        : %lu\n",pPVRDesc->pPVRProgInfo->enVCodec);
    printf("u32ACodec       : %lu\n",pPVRDesc->pPVRProgInfo->u32ACodec);
    printf("u32LCN          : %lu\n",pPVRDesc->pPVRProgInfo->u32LCN);
    printf("bIsEncrypted    : %d\n",pPVRDesc->pPVRProgInfo->bIsEncrypted);
    printf("eEncryptionType : %d\n",pPVRDesc->pPVRProgInfo->enEncryptionType);
    printf("enServiceType  : %d\n",pPVRDesc->pPVRProgInfo->enServiceType);
    printf("u32StartTime    : %lu\n",pPVRDesc->pPVRProgInfo->u32StartTime);
    printf("u32Duration     : %lu\n",pPVRDesc->pPVRProgInfo->u32Duration);
    printf("u64FileLength    : %llu\n",pPVRDesc->pPVRProgInfo->u64FileLength);
    printf("bIsScrambled    : %d\n",pPVRDesc->pPVRProgInfo->bIsScrambled);
    printf("u8ProgIdx        : %d\n",pPVRDesc->pPVRProgInfo->u8ProgIdx);
    printf("u16CCPid        : %d\n",pPVRDesc->pPVRProgInfo->u16CCPid);
    printf("u8Age            : %d\n",pPVRDesc->pPVRProgInfo->u8Age);
    printf("u32LastPlayPositionInSec : %lu\n",pPVRDesc->pPVRProgInfo->u32LastPlayPositionInSec);
    printf("bLocked          : %d\n",pPVRDesc->pPVRProgInfo->bLocked);
    printf("bLive            : %d\n",pPVRDesc->pPVRProgInfo->bLive);
    
    for (i=0; i<pPVRDesc->pPVRProgInfo->u8AudioInfoNum; i++)
    {
        if (i==0)
            printf("=====Audio Info=====\n");
        printf("u16AudPID%d = %d u8AudType%d = %d\n", i, pPVRDesc->pPVRProgInfo->AudioInfo[i].u16AudPID, i, pPVRDesc->pPVRProgInfo->AudioInfo[i].u8AudType);
    }

    for (i=0; i<pPVRDesc->pPVRProgInfo->u8TTXNum; i++)
    {
        if (i==0)
            printf("=====TTX Info=====\n");
        printf("u16TTX_Pid%d = %d\n", i, pPVRDesc->pPVRProgInfo->TXTInfo[i].u16TTX_Pid);
        printf("u16TTX_PidFilter%d = %d\n", i, pPVRDesc->pPVRProgInfo->TXTInfo[i].u16TTX_PidFilter);
        printf("u8ComponentTag%d = %d\n", i, pPVRDesc->pPVRProgInfo->TXTInfo[i].u8ComponentTag);
        printf("u8lanuage_code%d = %d%d%d\n", i,
        pPVRDesc->pPVRProgInfo->TXTInfo[i].u8lanuage_code[0],
        pPVRDesc->pPVRProgInfo->TXTInfo[i].u8lanuage_code[1],
        pPVRDesc->pPVRProgInfo->TXTInfo[i].u8lanuage_code[2]);
        printf("u8TTXMagNum%d = %d\n", i, pPVRDesc->pPVRProgInfo->TXTInfo[i].u8TTXMagNum);
        printf("u8TTXPageNum%d = %d\n", i, pPVRDesc->pPVRProgInfo->TXTInfo[i].u8TTXPageNum);
        printf("u8TTXType%d = %d\n", i, pPVRDesc->pPVRProgInfo->TXTInfo[i].u8TTXType);
    }
    
    for (i=0; i<pPVRDesc->pPVRProgInfo->u8DVBSubtitleNum; i++)
    {
        if (i==0)
            printf("=====DVBSub Info=====\n");
        printf("u16ancillary_page_id%d = %d\n", i, pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u16ancillary_page_id);
        printf("u16composition_page_id%d = %d\n", i, pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u16composition_page_id);
        printf("u16Sub_Pid%d = %d\n", i, pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u16Sub_Pid);
        printf("u8lanuage_code%d = %d%d%d\n", i,
        pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8lanuage_code[0],
        pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8lanuage_code[1],
        pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8lanuage_code[2]);
        printf("u8ComponentTag%d = %d\n", i, pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8ComponentTag);
        printf("u8subtitling_type%d = %d\n", i, pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8subtitling_type);
        printf("u8Sub_PidFilter%d = %d\n", i, pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8Sub_PidFilter);
    }

    for (i=0; i<pPVRDesc->pPVRProgInfo->u8EBUSubtitleNum; i++)
    {
        if (i==0)
            printf("=====EBUSub Info=====\n");
        printf("u16TTX_Pid%d = %d\n", i, pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u16TTX_Pid);
        printf("u16TTX_PidFilter%d = %d\n", i, pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u16TTX_PidFilter);
        printf("u8ComponentTag%d = %d\n", i, pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8ComponentTag);
        printf("u8lanuage_code%d = %d%d%d\n", i,
        pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8lanuage_code[0],
        pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8lanuage_code[1],
        pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8lanuage_code[2]);
        printf("u8TTXMagNum%d = %d\n", i, pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8TTXMagNum);
        printf("u8TTXPageNum%d = %d\n", i, pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8TTXPageNum);
        printf("u8TTXType%d = %d\n", i, pPVRDesc->pPVRProgInfo->EBUSubtInfo[i].u8TTXType);
    }
}

/********************************************************
   *   TDAL Functions   Definitions   (LOCAL/GLOBAL)        *
********************************************************/

tTDAL_PVR_Error TDAL_PVR_Init(void)
{
    tTDAL_PVR_Error             error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Init");
    EN_APIPVR_FILE_SYSTEM_TYPE  eFSType = EN_APIPVR_FILE_SYSTEM_TYPE_VFAT;
    PVRFuncInfo_t               pvrFunc = {NULL};
    PVR_TSPInfo_t               pvrTspInfo = {0};
    DMX_FLOW_INPUT              DmxFlowInput;;
    int i = 0;
#if (DMX_INPUT == DMX_INPUT_DEMOD)
    DmxFlowInput = DMX_FLOW_INPUT_DEMOD;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT0)
    DmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT1)
    DmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
#else
    DmxFlowInput = DMX_FLOW_INPUT_DEMOD;
#endif
    _eCurDMXInputSrc = DmxFlowInput;
    if (gTDAL_PVR_IsInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "Already init PVR!\n"));
        
        mTBOX_RETURN(error);
    }

    TDAL_PVR_recFSInfo[0].recPath[0]='\0';
    TDAL_PVR_recFSInfo[1].recPath[0]='\0';

    /* TODO - consider API for FS*/
    MsFS_Init(FALSE);

    // [01] Init PVR  ==============================================//
    //pvrTspInfo.u16DmxFlowInput  = (MS_U16) DmxFlowInput;
    //pvrTspInfo.bClkInv          = FALSE;
    //pvrTspInfo.bExtSync         = TRUE;
    //pvrTspInfo.bParallel        = TRUE;
   
    pxcMounthPath[0] = '\0';
    _moutPath[0]='\0';
    if (!MApi_PVR_Init(pxcMounthPath, eFSType, PVR_MAX_RECORDING_FILE, pvrTspInfo))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_Init: fail to initialize PVR module\n"));
        mTBOX_RETURN(error);
    }
    if(!MApi_PVR_SetPlaybackPathNum(1))
    {
        printf("Fail to Set PlaybackPathNum!\n");
        return FALSE;
    }

    // [02] Mem Layout: The base address/length of physical memory in MApi_PVR_Mmap ==============//
    if (!TDAL_PVRm_BatchAllocateMem())
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_Init: fail to initialize PVR module\n"));
        mTBOX_RETURN(error);
    }
 
    // [03] Callback Function for PVR ==============//
    //pvrFunc.ALLOC       = TDAL_PVRm_MEMALLOC_FUNC;
    //pvrFunc.FREE        = TDAL_PVRm_MEMFREE_FUNC;
    //pvrFunc.PA2VA       = TDAL_PVRm_MEMPA2VA_FUNC;
    //pvrFunc.MemFlush    = TDAL_PVRm_MEMFLUSH_FUNC;
    //pvrFunc.KLADDER     = TDAL_PVRm_KLADDER_FUNC;
    //pvrFunc.TrickSetting = appDemo_PVR_SetTrickMode;  [PVR ERROR] Fail to set force motion(TRUE)!
    MApi_PVR_SetFuncInfo(pvrFunc);
    
    TDAL_CreateMutex(&TDAL_PVRi_Mutex);
    mTBOX_ASSERT(TDAL_PVRi_Mutex != NULL);

    // [04] Recorder and Player type ==============//
    MApi_PVR_SetRecordType(eTDAL_PVR_RecordType);
    MApi_PVR_SetPlaybackType(eTDAL_PVR_PlayType);
    

    // [05] Create Message queue to receive info ==============//
    MApi_PVR_SetMsgWaitMs(0);
    TDAL_PVRi_QueueID = MApi_PVR_GetMessageQueueID();

    TDAL_PVRi_EndTask = FALSE;
    TDAL_PVRi_TaskFinished = FALSE;

    TDAL_PVRi_TaskHandle = TDAL_CreateTask(eTDAL_PRIORITY_NORMAL,
            "PVR_Task",
            TDAL_PVRi_TaskStack,
            sizeof(TDAL_PVRi_TaskStack),
            TDAL_PVRi_Task,
            (void *) 0
    );
    
    if (-1 == TDAL_PVRi_TaskHandle)
    {
        printf("Create Task fail\n");
        goto PVRInit_Fail;
    }

    // [06] Init PVR relative info ==============//
    PVRPlaybackPath = PVR_PATH_MAIN;
    _bScreenFrozen[PVRPlaybackPath]=TRUE;
    memset(&TDAL_PVR_livePromInfo, 0, sizeof(PVRProgramInfo_t));
    memset(&TDAL_PVR_plyprogramInfo, 0, sizeof(PVRProgramInfo_t));
    memset(&TDAL_PVR_bgrecPromInfo, 0, sizeof(PVRProgramInfo_t));
    
    for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
    {
        TDAL_PVR_u8hRecord[i] = APIPVR_INVALID_HANDLER;
        memset(&TDAL_PVR_recprogramInfo[i], 0, sizeof(PVRProgramInfo_t));
    }

    //MDrv_DSCMB2_SetDefaultCAVid(0, NARGA_CAVID);
    //MDrv_AESDMA_SetDefaultCAVid(NARGA_CAVID);
    TDAL_PVRm_ResetPVRRecFilters();

    gTDAL_PVR_IsInitialized = TRUE;
    mTBOX_RETURN(error);
    PVRInit_Fail:
    TDAL_PVR_Term();
}

tTDAL_PVR_Error TDAL_PVR_RegisterCallback(tTDAL_PVR_Callback PVR_CB)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_RegisterCallback");
    
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        mTBOX_RETURN(error);
    }
    
    if (PVR_CB == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Callback function is NULL!\n"));
        mTBOX_RETURN(error);        
    }
    
    TDAL_PVR_Notification = PVR_CB;
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Term(void)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Term");
   PVREventInfo_t pvrEvent;
    int i = 0;

    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initalized!\n"));
        mTBOX_RETURN(error);
    }

    TDAL_PVRi_EndTask = TRUE;

    if (MApi_PVR_Destroy())
    {
        MS_BOOL result = TRUE;

        while(!TDAL_PVRi_TaskFinished)
        {
            MsOS_DelayTask(100);
        }

        TDAL_PVRi_QueueID = -1;
        TDAL_DeleteTask(TDAL_PVRi_TaskHandle);
        TDAL_PVRi_TaskHandle = -1;
        //*Init relative info*//

        memset(&TDAL_PVR_livePromInfo,0,sizeof(PVRProgramInfo_t));
        memset(&TDAL_PVR_plyprogramInfo,0,sizeof(PVRProgramInfo_t));
        for(i=0;i<PVR_MAX_RECORDING_FILE;i++)
        {
            TDAL_PVR_Desc[i].pvrRecHandle = APIPVR_INVALID_HANDLER;
            TDAL_PVR_Desc[i].pvrPlyHandle = APIPVR_INVALID_HANDLER;
            if(TDAL_PVR_Desc[i].pPVRProgInfo !=NULL)
            {
                memset(TDAL_PVR_Desc[i].pPVRProgInfo,0,sizeof(PVRProgramInfo_t));
            }
        }

        result = TDAL_PVRm_BatchFreeMem();
        mTBOX_ASSERT(result == TRUE);
        
        TDAL_DeleteMutex(TDAL_PVRi_Mutex);
        TDAL_PVRi_Mutex = NULL;
        
        gTDAL_PVR_IsInitialized = FALSE;
        mTBOX_RETURN(error);
    }
    
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Destroy PVR failed\n"));
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Open(tTDAL_PVR_Handle *pvrHandle, bool bIsTimeshift, const char* pPath)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Open");
    int i = 0;
    
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Path is %s\n",pPath));
    
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    
    if (pvrHandle == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);        
    }
    
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (TDAL_PVR_Desc[i].bUsed == FALSE)
        {
            *pvrHandle = (tTDAL_PVR_Handle)&TDAL_PVR_Desc[i];
            break;
        }
    }
    
    if ((pPath!=NULL) && (strlen(pPath)>0))
    {
        char *pResPath = pPath;
        if (pPath[0]=='/')
        {
            pResPath = &pPath[1];
        }
        
        if (bIsTimeshift)
        {
            strcpy(TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath, pResPath);
        }
        else
        {
            strcpy(TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath, pResPath);
        }
    }
    else
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Path is not correct\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);       
    }
    
    TDAL_PVR_Desc[i].pvrPlyHandle       = 0xFF;
    TDAL_PVR_Desc[i].pvrRecHandle       = 0xFF;
    TDAL_PVR_Desc[i].progIdx            = 0xFF;
    TDAL_PVR_Desc[i].bUsed              = TRUE;
    TDAL_PVR_Desc[i].bIsTimeshift       = bIsTimeshift;
    TDAL_PVR_Desc[i].pPVRProgInfo       = &TDAL_PVR_recprogramInfo[i];
    TDAL_PVRi_ResetProgInfo(TDAL_PVR_Desc[i].pPVRProgInfo);
    
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
   
}

tTDAL_PVR_Error TDAL_PVR_Close(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Close");
    tTDAL_PVR_Desc *pPVRDesc = NULL;
    int i = 0;

    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);        
    }
    
    if (pPVRDesc->bIsTimeshift)
        TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath[0] = '\0';
    else
        TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath[0] = '\0';
    
    memset(pPVRDesc, 0, sizeof(tTDAL_PVR_Desc));
    
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);    
}

tTDAL_PVR_Error TDAL_PVR_Record_Start(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Record_Start");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    DMX_FLOW_INPUT  DmxFlowInput = DMX_FLOW_INPUT_DEMOD;
    bool            bRet = FALSE;
    bool            bRecordAll = FALSE;
    int i = 0;
    PVR_AV_ID_t AVStreamID;

    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
#if 0
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
    if (TDAL_MPi_ReleasePool() == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not start record, MP memory partition still used!!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);
    }
#endif
#endif
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }   

    TDAL_PVRi_GetRecMountPath(pPVRDesc->bIsTimeshift, &mountPath[0]);
    if (!TDAL_PVRm_GetDiskSpace(mountPath, &u64FreeSpaceInKB, &u64TotalSpaceInKB))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Disk Space can't be detected!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error); 
    }
    
    if (u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        TDAL_PVR_Notification(eTDAL_PVR_NO_DISK_SPACE, NULL);
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Disk Space is not enough! (less than %dMB)\n",PVR_FILE_SIZE_TO_STOP/1024));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    pPVRDesc->u64FreeSpaceInKB = u64FreeSpaceInKB;
    pPVRDesc->u32FullRecDuration = 0;

    if (pPVRDesc->FilesCount > 0)
    {
        /* we are continuing broken recording
         * we need to check how many chunks has been recorded
         * to determine next chunk name and total number of chunks */
        pPVRDesc->FilesCount = 0;
        while(TDAL_PVRm_IsFileExist(MApi_PVR_GetMouthPath(), pPVRDesc->pPVRProgInfo->FileName))
        {
            pPVRDesc->FilesCount++;
            sprintf(strchr(pPVRDesc->pPVRProgInfo->FileName, '.')-3, "%03d.trp", pPVRDesc->FilesCount);
        }
    }

    if (bIsSeamlessPlayback)
    {
        //TDAL_PVRi_BackGround_Record_Stop();
    }
    mTBOX_TRACE((kTBOX_NIV_5, "TDAL_PVR_Record_Start: Record file %s\n", pPVRDesc->pPVRProgInfo->FileName));
    /* Set metadata in order to save settings for playback */
    if (!MApi_PVR_SetMetaData(pPVRDesc->pPVRProgInfo, pPVRDesc->progIdx, 0, 0, 0, 0))
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_SetMetaData: failure!!!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        //TDAL_PVRi_BackGround_Record_Start();
        mTBOX_RETURN(error);
    }
    AVStreamID.vstreamID = (void *)(&_NullVidStreamID);
    AVStreamID.astreamID = (void *)(&_NullAudStreamID);    
    bRet = MApi_PVR_EX_RecordEngSet(&pPVRDesc->pvrRecHandle,(void *)(&AVStreamID), pPVRDesc->progIdx, APIPVR_FILE_LINEAR, u64FreeSpaceInKB/1024, FALSE);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_RecordEngSet: failure!!!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);        
        //TDAL_PVRi_BackGround_Record_Start();
        mTBOX_RETURN(error);
    }
    bRet = MApi_PVR_EX_RecordEngEnable(&pPVRDesc->pvrRecHandle,bRecordAll);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_RecordEngEnable: failure!!!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);        
        //TDAL_PVRi_BackGround_Record_Start();
        mTBOX_RETURN(error);
    }    
    pPVRDesc->FilesCount++;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Record_Stop(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Record_Stop");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    bool bRet = FALSE;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);        
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrRecHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_IsRecording(pPVRDesc->pvrRecHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_IsRecording: no recordings to stop\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    pPVRDesc->pPVRProgInfo->u32Duration  = MApi_PVR_GetRecordTime(pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);
    pPVRDesc->pPVRProgInfo->u32StartTime = 0;

    bRet = MApi_PVR_RecordStop(&pPVRDesc->pvrRecHandle,pPVRDesc->progIdx);
    
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_RecordStop: failed to stop\n"));
    }


    //patch to notify FS that recording has changed disk space
     pTDAL_PVR_NotifyRecordedContent(TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath,
               (uint32_t) (pPVRDesc->u64FullRecSizeInKB + pPVRDesc->pPVRProgInfo->u64FileLength));
    TDAL_PVRm_ResetPVRRecFilters();

    /* TODO -  program info reset?*/
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    //TDAL_PVRi_BackGround_Record_Start();
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Record_Pause(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Record_Pause");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    bool            bRet = FALSE;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);        
    }
    
    if(!PVR_IS_VALID_HANDLER(pPVRDesc->pvrRecHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    bRet = MApi_PVR_RecordPause(pPVRDesc->progIdx);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_RecordPause: failed to pause current recording!\n"));    
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Record_Resume(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Record_Resume");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    bool            bRet = FALSE;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);        
    }
    
    if(!PVR_IS_VALID_HANDLER(pPVRDesc->pvrRecHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    bRet = MApi_PVR_RecordResume(pPVRDesc->progIdx);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_RecordPause: failed to pause current recording!\n"));    
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStart(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Timeshift_RecordStart");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    DMX_FLOW_INPUT  DmxFlowInput = DMX_FLOW_INPUT_DEMOD;
    bool            bRet = FALSE;
    bool            bRecordAll = FALSE;
    char            FileName[32] = {0};
    int i = 0;
    PVR_AV_ID_t AVStreamID;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
#if 0
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
    if (TDAL_MPi_ReleasePool() == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not start record, MP memory partition still used!!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);
    }
#endif
#endif
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift==FALSE)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (bIsSeamlessPlayback)
    {
        pPVRDesc->pvrRecHandle = pvrbgRecHandle;
        pPVRDesc->progIdx = progbgIdx;
        _u32SeamlessStartTime = MsOS_GetSystemTime() - _u32SeamlessStartTime;
        _u32SeamlessStartTime/=1000;
    }
    
    if ((pPVRDesc->progIdx!=0xFF) && MApi_PVR_IsRecording(pPVRDesc->progIdx))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Can not start timeshift recording\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    // [02] Should not play now ==============================================//
    if(MApi_PVR_IsPlaybacking())
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Can not start timeshift recording. Playback is going on\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    // [03] Remove timeshift file ==============================================//
    if (TDAL_PVRm_IsFileExist(MApi_PVR_GetMouthPath(), pPVRDesc->pPVRProgInfo->FileName))
    {
        bRet = MApi_PVR_RemoveFile(pPVRDesc->pPVRProgInfo->FileName);
        if (bRet == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Can not remove previous timeshift file!\n"));
            TDAL_UnlockMutex(TDAL_PVRi_Mutex);
            mTBOX_RETURN(error);
        }
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_3, "Timeshift file does not exists\n"));
    }
    
    TDAL_PVRi_GetRecMountPath(pPVRDesc->bIsTimeshift, &mountPath[0]);
    if (!TDAL_PVRm_GetDiskSpace(mountPath, &u64FreeSpaceInKB, &u64TotalSpaceInKB))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Disk Space can't be detected!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if(u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Disk Space is not enough! (less than %dMB)\n",PVR_FILE_SIZE_TO_STOP/1024));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    /* Set metadata in order to save settings for playback */
    bRet = MApi_PVR_SetMetaData(pPVRDesc->pPVRProgInfo, pPVRDesc->progIdx, 0, 0, 0, 0);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_SetMetaData: failure!!!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }    
    /* Timeshift setting for Live buffer */

    MApi_PVR_EX_FreezeScreenInTimeshiftRecord(PVRPlaybackPath,EN_APIPVR_ACTION_TYPE_SET, &_bScreenFrozen[PVRPlaybackPath]);
    MApi_PVR_SetTimeshiftFileSize(u64FreeSpaceInKB*0.98);//left at least 2% free space for PVR middleware
    /* TODO */
    
    AVStreamID.vstreamID = (void *)(&_NullVidStreamID);
    AVStreamID.astreamID = (void *)(&_NullAudStreamID); 

    bRet = MApi_PVR_EX_TimeshiftRecordEngSet(&pPVRDesc->pvrRecHandle,(void *)(&AVStreamID),pPVRDesc->progIdx,PVRPlaybackPath);

    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_TimeshiftRecordEngSet: failure!!!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    bRet = MApi_PVR_EX_TimeshiftRecordEngEnable(&pPVRDesc->pvrRecHandle);

    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_TimeshiftRecordEngEnable: failure!!!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }    

    pPVRDesc->FilesCount = 1;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStop(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Timeshift_RecordStop");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    uint8_t filename[128] = {0};
    int i = 0;
    bool bRet = FALSE;
    struct stat buf;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift==FALSE)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrRecHandle))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not valid pvrRecHandle - progIdx\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_IsRecording(pPVRDesc->pvrRecHandle))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Already stop recording\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    pPVRDesc->pPVRProgInfo->u32Duration  = MApi_PVR_GetRecordTime(pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);
    MApi_PVR_GetRecordStartTime(pPVRDesc->pvrRecHandle, &pPVRDesc->pPVRProgInfo->u32StartTime);

    bRet = MApi_PVR_TimeshiftRecordStop(&pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);   
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_TimeshiftRecordStop: failure!!!\n"));
    }

    /* FIXME: Ugly patch for filename since it does beign with '/' */
    filename[0]='/';
    strncpy(&filename[1], pPVRDesc->pPVRProgInfo->FileName, sizeof(filename));

    if( MsFS_Stat(&filename, &buf)<0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "errno = %d\n", errno));
    }


   //patch to notify FS that recording has changed disk space
    pTDAL_PVR_NotifyRecordedContent(TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath,
                    (uint32_t) (buf.st_size >> 10));
    TDAL_PVRm_ResetPVRRecFilters();

    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    /* TODO - audio mute?*/
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Timeshift_Stop(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Timeshift_Stop");
    
 
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Timeshift_PlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Timeshift_PlaybackStart");

    bool bRet = FALSE;
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    PVRDataInfo_t dataInfo;
    char PathName[32]={0};
    PVR_AV_ID_t AVStreamID;
    PVRFilterInfo_t *Filters = NULL;
    
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (bIsSeamlessPlayback)
    {
        TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
        MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);
        _ReturnToLive = TRUE;        
    }
    else
    {
        if (TDAL_AV_VideoStarted())
        {
            TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);
            _ReturnToLive = TRUE;
        }
        if (TDAL_AV_AudioStarted())
        {
            TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
            _ReturnToLive = TRUE;
        }
    }

    if (_ReturnToLive == TRUE)
    {
        TDAL_DMXi_GetLiveChannelID(&VideoChannel,&AudioChannel,&PCRChannel);
        TDAL_PVRi_ControlChannel_TDAL_DMX(FALSE);
    }
#if 0
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
    if (TDAL_MPi_ReleasePool() == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not start record, MP memory partition still used!!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);
    }
#endif
#endif
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift==FALSE)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    // [02] change to normal speed when playing  ==============================================//
    if (MApi_PVR_IsPlaybacking())
    {
        /* TODO - audio muting*/
        /* TODO - check this handle, this handle could be wrong one, very likely */
        bRet = MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle,EN_APIPVR_PLAYBACK_SPEED_1X);
        if (bRet)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is playback, already\n"));
            TDAL_UnlockMutex(TDAL_PVRi_Mutex);
            mTBOX_RETURN(error);
        }
        else
        {
            error = eTDAL_PVR_ERROR_NOT_DONE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is playback, already and something went wrong\n"));
            TDAL_UnlockMutex(TDAL_PVRi_Mutex);
            mTBOX_RETURN(error); 
        }
    }

    // [03]  ==============================================//
    memcpy(&TDAL_PVR_plyprogramInfo, pPVRDesc->pPVRProgInfo, sizeof(PVRProgramInfo_t));

    // [04]   ==============================================//
    AVStreamID.vstreamID = (void *)(&_NullVidStreamID);
    AVStreamID.astreamID = (void *)(&_NullAudStreamID);
    
    MApi_PVR_EX_PlaybackABLoopReset(pPVRDesc->pvrPlyHandle);
#ifdef PRODUCT_USE_CA
    tTDAL_PVR_Rec_Filters *rec_filter = NULL;
    uint32_t descId;
    
    if(rec_filter)
    {
        for (i = 0 ; i < 128 ; i++)
        {
            if (PVRRecFilters[i].u8FilterID != INVALID_FILTER_ID && PVRRecFilters[i].u16PID != INVALID_PID)
            {
                descId = TDAL_DESC_GetDescramblerID(PVRRecFilters[i].u16PID);
                if(descId != 0xFFFFFFFF)
                {
                    MDrv_DSCMB2_FltConnectFltId(0,descId,PVRRecFilters[i].u8FilterID);
                }
            }
        }
    }
#endif
    bRet = MApi_PVR_EX_TimeshiftPlaybackStart(PVRPlaybackPath,&pPVRDesc->pvrPlyHandle,PVRPlaybackPath,(void *)(&AVStreamID),TDAL_PVR_plyprogramInfo.FileName, u32PlaybackTimeInSec, 0);
    if (bRet == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_TimeshiftPlaybackStart failed\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    /*TODO - screen freezing?*/
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_Timeshift_PlaybackStop(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_Timeshift_PlaybackStop");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    bool bRet = FALSE;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift==FALSE)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not valid pvrPlyHandle\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (MApi_PVR_IsPlaybacking() == FALSE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Already stop playing\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (!MApi_PVR_PlaybackStop(&pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_IN_DRIVER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackStop failed to execute!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    while (MApi_PVR_IsPlaybacking())
    {
        MsOS_DelayTask(10);
    }
    /* TODO - so this should be done in the MW */

    memset(&TDAL_PVR_plyprogramInfo, 0, sizeof(PVRProgramInfo_t));      
    if (_ReturnToLive == TRUE)
    {
        tTDAL_AV_StreamType StreamType;
        TDAL_PVRi_ControlChannel_TDAL_DMX(TRUE);
        
        StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
        TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType);

        StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;
        TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,StreamType);
        _ReturnToLive = FALSE;
    }

    TDAL_PVR_playbackTime = 0;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

bool TDAL_PVR_IsRecording(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_IsRecording");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    bool bRet = FALSE;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }
    bRet = MApi_PVR_IsRecording(pPVRDesc->pvrRecHandle);
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    return bRet;
}

bool TDAL_PVR_IsPlaybacking(void)
{
    bool bRet = FALSE;
    bRet = MApi_PVR_IsPlaybacking();
    return bRet;
}
tTDAL_PVR_Error TDAL_PVR_PlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackStart");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    MS_BOOL bRet = FALSE;
    PVR_AV_ID_t AVStreamID;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (TDAL_AV_VideoStarted())
    {
        TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);
        _ReturnToLive = TRUE;
    }
    if (TDAL_AV_AudioStarted())
    {
        TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
        _ReturnToLive = TRUE;
    }

    if (_ReturnToLive == TRUE)
    {
        TDAL_DMXi_GetLiveChannelID(&VideoChannel,&AudioChannel,&PCRChannel);
        TDAL_PVRi_ControlChannel_TDAL_DMX(FALSE);
    }
#if 0
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
    if (TDAL_MPi_ReleasePool() == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not start record, MP memory partition still used!!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);
    }
#endif
#endif

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }    
    
    if (pPVRDesc==NULL || pPVRDesc->bIsTimeshift!=FALSE)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (MApi_PVR_IsPlaybacking())
    {
        /* TODO - audio muting*/
        /* TODO - check this handle, this handle could be wrong one, very likely */
        bRet = MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle,EN_APIPVR_PLAYBACK_SPEED_1X);
        if (bRet)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is playback, already\n"));
            TDAL_UnlockMutex(TDAL_PVRi_Mutex);
            mTBOX_RETURN(error);
        }
        else
        {
            error = eTDAL_PVR_ERROR_NOT_DONE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is playback, already and something went wrong\n"));
            TDAL_UnlockMutex(TDAL_PVRi_Mutex);
            mTBOX_RETURN(error); 
        }
    }
    memcpy(&TDAL_PVR_plyprogramInfo, pPVRDesc->pPVRProgInfo, sizeof(PVRProgramInfo_t));

    pPVRDesc->eCurrentSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
        
    bRet = MApi_PVR_EX_PlaybackABLoopReset(pPVRDesc->pvrPlyHandle);
    if (!bRet)
    {
        //error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_PlaybackABLoopReset: failed to reset AB loop\n"));
        //mTBOX_RETURN(error);
    }
    
    AVStreamID.vstreamID = Demo_VDEC_GetStreamID(0);
    AVStreamID.astreamID = (void *)(&_NullAudStreamID);
    

    bRet = MApi_PVR_EX_PlaybackStart(PVRPlaybackPath,&pPVRDesc->pvrPlyHandle,(void *)(&AVStreamID),TDAL_PVR_plyprogramInfo.FileName, u32PlaybackTimeInSec, 0);
    if(!bRet)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_PlaybackStart: failed to start playback\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackStop(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackStop");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    } 

    if (MApi_PVR_IsPlaybacking() == FALSE)
    {
        tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
        printf("Already stop playing\n");
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    /* This serves to block the PVR playback stopping action
     * if file suddenly disappears(USB ejected...)
     */ 
    TDAL_PVRi_GetRecMountPath(FALSE, &mountPath[0]);
    if (TDAL_PVRm_CheckUSB(mountPath) == FALSE)
    {
        tTDAL_PVR_Error error = eTDAL_PVR_ERROR_NOT_DONE;
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_PlaybackStop(&pPVRDesc->pvrPlyHandle))
    {
        tTDAL_PVR_Error error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackStop: failed to stop playback\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (_ReturnToLive == TRUE)
    {
        tTDAL_AV_StreamType StreamType;
        TDAL_PVRi_ControlChannel_TDAL_DMX(TRUE);
        
        StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
        TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType);

        StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;
        TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,StreamType);
        _ReturnToLive = FALSE;
    }

    TDAL_PVR_playbackTime = 0;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackPause(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackPause");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    bool bRet = TRUE;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }
    
    if (MApi_PVR_IsPlaybacking() != TRUE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not in playback state!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    bRet = MApi_PVR_PlaybackPause(pPVRDesc->pvrPlyHandle);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackPause: failed to pause!\n"));
    }
    else
    {
        pPVRDesc->eCurrentSpeed = EN_APIPVR_PLAYBACK_SPEED_0X;
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackResume(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackResume");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    bool bRet = FALSE;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }
    
    if (MApi_PVR_IsPlaybacking() != TRUE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not in playback state\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle) == EN_APIPVR_PLAYBACK_SPEED_0X)
    {
        /* TODO */
        //_appDemo_PVR_AudioMute(FALSE, TRUE);
        bRet = MApi_PVR_PlaybackResume(pPVRDesc->pvrPlyHandle);
        if (bRet == FALSE)
        {
            error = eTDAL_PVR_ERROR_NOT_DONE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackResume: failed!\n"));
            TDAL_UnlockMutex(TDAL_PVRi_Mutex);
            mTBOX_RETURN(error);            
        }
        pPVRDesc->eCurrentSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    else if (MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle) == EN_APIPVR_PLAYBACK_SPEED_STEP_IN)
    {
        /* TODO - audio mute?*/
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    bRet = MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle, EN_APIPVR_PLAYBACK_SPEED_1X);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackSetSpeed: failed!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);            
    }
    else
    {
        pPVRDesc->eCurrentSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
    }

    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackGetSpeed(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed* pSpeed)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackResume");
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    APIPVR_PLAYBACK_SPEED speed;
    bool bRet = FALSE;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }

    if (MApi_PVR_IsPlaybacking() != TRUE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not in playback state\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    speed = MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle);

    *pSpeed = TDAL_PVRi_TrickModeConvBack(speed);
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackFastForward(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed enTrick)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackFastForward");
    bool bRet = FALSE;
    APIPVR_PLAYBACK_SPEED enSpeed = EN_APIPVR_PLAYBACK_SPEED_INVALID;
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }
        
    if (MApi_PVR_IsPlaybacking() != TRUE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not in playback state\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    enSpeed = TDAL_PVRi_TrickModeConv(enTrick);
    if (enSpeed == EN_APIPVR_PLAYBACK_SPEED_INVALID)
    {
        enSpeed = MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle);
    }

    bRet = MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle, enSpeed);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackSetSpeed: failed!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);            
    }
    else
    {
        pPVRDesc->eCurrentSpeed = enSpeed;
    }
    
    if(EN_APIPVR_PLAYBACK_SPEED_1X != MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle))
    {
        /* TODO - mute/unmute due to playback speed */
    }
    else
    {
        /* TODO - mute/unmute due to playback speed */
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackFastBackward(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed enTrick)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackFastBackward");
    APIPVR_PLAYBACK_SPEED enSpeed=EN_APIPVR_PLAYBACK_SPEED_INVALID;
    bool bRet = FALSE;
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }
        
    if (MApi_PVR_IsPlaybacking()!=TRUE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not in playback state\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    enSpeed = TDAL_PVRi_TrickModeConv(enTrick);
    if (enSpeed == EN_APIPVR_PLAYBACK_SPEED_INVALID)
    {
        enSpeed = MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle);
    }
    
    bRet = MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle, enSpeed);
    if (bRet == FALSE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackSetSpeed: failed!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);            
    }
    else
        pPVRDesc->eCurrentSpeed = enSpeed;
    
    if (EN_APIPVR_PLAYBACK_SPEED_1X != MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle))
    {
        /* TODO - mute/unmute due to playback speed */
    }
    else
    {
        /* TODO - mute/unmute due to playback speed */
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackJumpToTime(tTDAL_PVR_Handle pvrHandle, int32_t *u32Hour, uint32_t *u32Minute, uint32_t *u32Second)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackJumpToTime");
    MS_U32 u32JumpToTimeInSeconds = (*u32Hour)*60*60+(*u32Minute)*60+(*u32Second);
    tTDAL_PVR_Desc  *pPVRDesc = NULL;
    bool bRet = FALSE;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }

    if (pPVRDesc == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVRDesc is corrupted!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);        
    }
        
    if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (((*u32Hour)>23) || ((*u32Hour)<0))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "invalide value for hours\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (((*u32Minute)>59) || ((*u32Minute)<0))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "invalide value for minutes\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (((*u32Second)>59) || ((*u32Second)<0))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "invalide value for minutes\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (MApi_PVR_IsPlaybacking() != TRUE)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not in playback state\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    bRet = MApi_PVR_PlaybackJumpToTime(pPVRDesc->pvrPlyHandle, u32JumpToTimeInSeconds);
    if (bRet)
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackJumpToTime: failed to jump to time!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_GetFileInfo(char *FileName)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_GetFileInfo");
    
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_SetProgramInfo(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_BasicProgInfo *pBasicProgInfo)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVRm_SetProgramInfo");   
    tTDAL_PVR_Desc *pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
    MS_U8 enIdx = APIPVR_FILE_LINEAR;
    int i = 0;
    
    if (pPVRDesc == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR handle is NULL\n"));
        mTBOX_RETURN(error);
    }
    
    if (pBasicProgInfo==NULL || pBasicProgInfo->fileName==NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pBasicProgInfo is NULL\n"));
        mTBOX_RETURN(error);
    }
    
    enIdx = APIPVR_FILE_LINEAR;
    
    TDAL_PVRi_ResetProgInfo(pPVRDesc->pPVRProgInfo);
    if (pPVRDesc->bIsTimeshift)
    {
        TDAL_PVRi_ResetProgInfo(&TDAL_PVR_livePromInfo);
        TDAL_PVRi_SetProgInfo(&TDAL_PVR_livePromInfo, (void*)pBasicProgInfo);
        if (eTDAL_PVR_RecordType == EN_APIPVR_RECORD_TYPE_DUAL)
        {
            enIdx = APIPVR_FILE_CIRCULAR;
        }
    }

    TDAL_PVRi_SetProgInfo(pPVRDesc->pPVRProgInfo, (void*)pBasicProgInfo);
    /* Starting demultiplexing in PVR need this info, so in the future
     * this should be moved in higher layer of SW. Moreover there is slight 
     * redundancy so this part currently need some TODO, too.
     */
    pPVRDesc->ProgInfoElem.u16AudioPID = (uint16_t) pBasicProgInfo->u32AudioPid;
    pPVRDesc->ProgInfoElem.u16VideoPID = (uint16_t) pBasicProgInfo->u32VideoPid;
    pPVRDesc->ProgInfoElem.u16PCRPID   = (uint16_t) pBasicProgInfo->u32PCRPid;
    pPVRDesc->FilesCount               = (uint16_t) pBasicProgInfo->filesCount;
    
    if (pPVRDesc->bIsTimeshift)
    {
        if (strlen(TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath)>0)
        {
            sprintf(pPVRDesc->pPVRProgInfo->FileName, "%s/%s", TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath, pBasicProgInfo->fileName);
        }
    }
    else
    {
        if (strlen(TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath)>0)
        {
            sprintf(pPVRDesc->pPVRProgInfo->FileName, "%s/%s", TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath, pBasicProgInfo->fileName);
        }
        mTBOX_TRACE((kTBOX_NIV_3, "Full file name for recording: %s\n", pPVRDesc->pPVRProgInfo->FileName));
    }
    
    // [02] Store meta data ==============================================//
    pPVRDesc->progIdx = enIdx;
   
    pPVRDesc->FilesCount = pBasicProgInfo->filesCount;

    pPVRDesc->pPVRProgInfo->bIsEncrypted = pBasicProgInfo->bIsEncrypted;
    switch (pBasicProgInfo->eEncryptionType)
    {
        case eTDAL_PVR_ENCR_NONE:
            pPVRDesc->pPVRProgInfo->enEncryptionType = EN_APIPVR_ENCRYPTION_NONE;
            break;
        case eTDAL_PVR_ENCR_DEFAULT:
            pPVRDesc->pPVRProgInfo->enEncryptionType = EN_APIPVR_ENCRYPTION_DEFAULT;
            break;
        case eTDAL_PVR_ENCR_USER:
            pPVRDesc->pPVRProgInfo->enEncryptionType = EN_APIPVR_ENCRYPTION_USER;
            break;            
        case eTDAL_PVR_ENCR_CIPLUS:
            pPVRDesc->pPVRProgInfo->enEncryptionType = EN_APIPVR_ENCRYPTION_CIPLUS;
            break;            
        case eTDAL_PVR_ENCR_SMARTCARD:
            pPVRDesc->pPVRProgInfo->enEncryptionType = EN_APIPVR_ENCRYPTION_SMARTCARD;
            break;
        default:
            printf("Wrong EncryptionType : %d\n Reset to NONE",pBasicProgInfo->eEncryptionType);
            pPVRDesc->pPVRProgInfo->bIsEncrypted = FALSE;
            pPVRDesc->pPVRProgInfo->enEncryptionType = EN_APIPVR_ENCRYPTION_NONE;
            break;
    }
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_GetProgramInfo(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_ComplexProgInfo *pPVRComplexInfo)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVRm_SetProgramInfo");
    tTDAL_PVR_Desc *pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
    
    if (pPVRDesc == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR handle is NULL\n"));
        mTBOX_RETURN(error);
    }

    if (pPVRComplexInfo==NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVRComplexInfo pointer is NULL\n"));
        mTBOX_RETURN(error);
    }
        
    if (pPVRDesc->pPVRProgInfo == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVRComplexInfo pointer is NULL\n"));
        mTBOX_RETURN(error);        
    }
    
    /* Getting information about recorded content from embedded metadata file*/
    if (pPVRDesc->bIsTimeshift)
    {
        if (strlen(TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath)>0)
        {
            sprintf(CurRecordedFileName, "%s/%s", TDAL_PVR_recFSInfo[PVR_TIMESHIFT_PATH_IDX].recPath, pPVRComplexInfo->stPVRBasicProgInfo.fileName);
        }
    }
    else
    {
        if (strlen(TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath)>0)
        {
            sprintf(CurRecordedFileName, "%s/%s", TDAL_PVR_recFSInfo[PVR_NORMALREC_PATH_IDX].recPath, pPVRComplexInfo->stPVRBasicProgInfo.fileName);
        }
        mTBOX_TRACE((kTBOX_NIV_3, "Full file name for recording: %s\n", pPVRDesc->pPVRProgInfo->FileName));
    }
    
    TDAL_PVRi_ResetProgInfo(pPVRDesc->pPVRProgInfo);
    if (MApi_PVR_GetProgramInfo(CurRecordedFileName, pPVRDesc->pPVRProgInfo) == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_GetProgramInfo: failure\n"));
    }

    /* Gets basic info from recorded file*/
    pPVRComplexInfo->stPVRBasicProgInfo.u32AudioPid = pPVRDesc->pPVRProgInfo->u16AudioPid;
    pPVRComplexInfo->stPVRBasicProgInfo.u32VideoPid = TDAL_PVRi_FindPidInFilter(pPVRDesc->pPVRProgInfo->Filters, DMX_FILTER_TYPE_VIDEO);
    pPVRComplexInfo->stPVRBasicProgInfo.u32PCRPid   = TDAL_PVRi_FindPidInFilter(pPVRDesc->pPVRProgInfo->Filters, DMX_FILTER_TYPE_PCR);
    pPVRComplexInfo->stPVRBasicProgInfo.u32PmtPid   = TDAL_PVRi_FindPidInFilter(pPVRDesc->pPVRProgInfo->Filters, DMX_FILTER_TYPE_SECTION);
    //First ACodec
    pPVRComplexInfo->stPVRBasicProgInfo.nAudioInfoCount = pPVRDesc->pPVRProgInfo->u8AudioInfoNum;
    //pProgInfo->AudioInfo[0].u16AudPID = (MS_U16)pBasicInfo->u32AudioPid;
    //pProgInfo->AudioInfo[0].u8AudType = (MS_U16)pBasicInfo->u32ACodec;
    
    int i = 0;
    /* Multiple audio tracks are enabled if they exist */

    for (i=0; i<pPVRDesc->pPVRProgInfo->u8AudioInfoNum; i++)
    {
        pPVRComplexInfo->stPVRBasicProgInfo.pAudioInfo[i].u16AudioPID = pPVRDesc->pPVRProgInfo->AudioInfo[i].u16AudPID; 
        pPVRComplexInfo->stPVRBasicProgInfo.pAudioInfo[i].u8AudioType = TDAL_PVRi_ADEC_MS2TDAL(pPVRDesc->pPVRProgInfo->AudioInfo[i].u8AudType); 
    }
    
    /* Multiple subtitles are enabled if they exist */
    pPVRComplexInfo->stPVRBasicProgInfo.nSubsInfoCount = pPVRDesc->pPVRProgInfo->u8EBUSubtitleNum;

    for (i=0; i<pPVRDesc->pPVRProgInfo->u8EBUSubtitleNum; i++)
    {
        pPVRComplexInfo->stPVRBasicProgInfo.pSubsInfo[i].u16Sub_Pid = pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u16Sub_Pid; 
        pPVRComplexInfo->stPVRBasicProgInfo.pSubsInfo[i].u8ComponentTag = pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8ComponentTag;
        pPVRComplexInfo->stPVRBasicProgInfo.pSubsInfo[i].u8subtitling_type = pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8subtitling_type;
        pPVRComplexInfo->stPVRBasicProgInfo.pSubsInfo[i].u8lanuage_code[0] = pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8lanuage_code[0];
        pPVRComplexInfo->stPVRBasicProgInfo.pSubsInfo[i].u8lanuage_code[1] = pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8lanuage_code[1];
        pPVRComplexInfo->stPVRBasicProgInfo.pSubsInfo[i].u8lanuage_code[2] = pPVRDesc->pPVRProgInfo->DVBSubtInfo[i].u8lanuage_code[2];
    }
    
    pPVRComplexInfo->stPVRBasicProgInfo.u32ACodec   = TDAL_PVRi_ADEC_MS2TDAL(pPVRDesc->pPVRProgInfo->u32ACodec);
    pPVRComplexInfo->stPVRBasicProgInfo.u32VCodec   = TDAL_PVRi_VDEC_MS2TDAL((MS_U32)pPVRDesc->pPVRProgInfo->enVCodec);
    pPVRComplexInfo->stPVRBasicProgInfo.u32LCN      = pPVRDesc->pPVRProgInfo->u32LCN;
    /* Gets additional info from recorded file*/
    if (pPVRDesc->FilesCount > 0)
    {
        TDAL_PVRi_GetDurationAndSizeContent(pPVRDesc, CurRecordedFileName);
        pPVRComplexInfo->stPVRBasicProgInfo.filesCount  = pPVRDesc->FilesCount;
        pPVRComplexInfo->u32StartTime                   = pPVRDesc->pPVRProgInfo->u32StartTime;
        pPVRComplexInfo->u32duration                    = pPVRDesc->u32FullRecDuration;
        pPVRComplexInfo->u64FileLength                  = pPVRDesc->u64FullRecSizeInKB;
        pPVRComplexInfo->u32LastPlayPositionInSec       = pPVRDesc->pPVRProgInfo->u32LastPlayPositionInSec;
        pPVRComplexInfo->u8Age                          = pPVRDesc->pPVRProgInfo->u8Age; 
        pPVRComplexInfo->bLocked                        = pPVRDesc->pPVRProgInfo->bLocked; // lock by user and it cannot be deleted even if disk is full
    }
    pPVRComplexInfo->stPVRBasicProgInfo.bIsEncrypted = pPVRDesc->pPVRProgInfo->bIsEncrypted;
    pPVRComplexInfo->stPVRBasicProgInfo.eEncryptionType = pPVRDesc->pPVRProgInfo->enEncryptionType;

    printf("PlaybackInfo \n");
    printf("u32VideoPid     : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32VideoPid);
    printf("u32PCRPid       : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32PCRPid);
    printf("u32AudioPid     : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32AudioPid);
    printf("u32PmtPid       : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32PmtPid);
    printf("u32VCodec       : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32VCodec);
    printf("u32ACodec       : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32ACodec);
    printf("u32LCN          : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32LCN);
    printf("u32LCN          : %lu\n",pPVRComplexInfo->stPVRBasicProgInfo.u32LCN);
    printf("bIsEncrypted    : %d\n",pPVRComplexInfo->stPVRBasicProgInfo.bIsEncrypted);
    printf("eEncryptionType : %d\n",pPVRComplexInfo->stPVRBasicProgInfo.eEncryptionType);
  
    memset(CurRecordedFileName,0, sizeof(CurRecordedFileName));
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_SetPlaybackRetentionLimit(tTDAL_PVR_Handle pvrHandle, const uint32_t *u32RetentionLimitTimeInSec)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_SetPlaybackRetentionLimit");
    tTDAL_PVR_Desc *pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
    MS_U32 u32RetentionLimitTime = *u32RetentionLimitTimeInSec;
    int i = 0;
    
    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }
    
    if (PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrRecHandle is corrupted\n"));
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_IsPlaybacking())
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "need to be in playback state\n"));
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_SetPlaybackRetentionLimit(TDAL_PVR_plyprogramInfo.FileName, &u32RetentionLimitTime))
    {
        printf("Fail to set retention time!\n");
        return FALSE;
    }
    
    if (u32RetentionLimitTime != *u32RetentionLimitTimeInSec)
    {
        printf("Set retention time to file end time!\n");
    }
    
    mTBOX_RETURN(error);
}

/* jump forward 30 sec and normal play */
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpForward(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackJumpForward");
    tTDAL_PVR_Desc *pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }

    if (PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_IsPlaybacking())
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_PlaybackJumpForward(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackJumpForward failed to jump\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

tTDAL_PVR_Error TDAL_PVR_PlaybackJumpBackward(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_PlaybackJumpBackward");
    tTDAL_PVR_Desc *pPVRDesc = NULL;
    int i = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    if (!gTDAL_PVR_IsInitialized)
    {
        error = eTDAL_PVR_ERROR_NOT_INIT;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PVR is not initialized!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    if (pvrHandle == (tTDAL_PVR_Handle)NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrHandle is NULL!\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }

    for (i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (pvrHandle==(tTDAL_PVR_Handle)&TDAL_PVR_Desc[i] && TDAL_PVR_Desc[i].bUsed==TRUE)
        {
            pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
            break;
        }
    }

    if (PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_IsPlaybacking())
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pvrPlyHandle is corrupted\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    
    if (!MApi_PVR_PlaybackJumpBackward(pPVRDesc->pvrPlyHandle))
    {
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackJumpBackward failed to jump\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        mTBOX_RETURN(error);
    }
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    mTBOX_RETURN(error);
}

MS_BOOL  TDAL_PVRm_GetRecMountPath(MS_BOOL bTimeshift, char *mountpath)
{
    int len = 0;
    char *path = mountpath;
    PVR_REC_PATH_IDX subPathIdx = PVR_NORMALREC_PATH_IDX;
    if(bTimeshift)
    {
        subPathIdx = PVR_TIMESHIFT_PATH_IDX;
    }

    if(strlen(TDAL_PVR_recFSInfo[subPathIdx].recPath)==0)
    {
        len = strlen(MApi_PVR_GetMouthPath());
        memcpy(path,MApi_PVR_GetMouthPath(),len);
    }
    else
    {
        sprintf(path,"%s/%s",MApi_PVR_GetMouthPath(),TDAL_PVR_recFSInfo[subPathIdx].recPath);
    }

    return TRUE;
}

LOCAL void TDAL_PVRi_GetProgInfo(PVRProgramInfo_t *pProgInfo, char* fileName)
{
    if (pProgInfo == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_GetProgInfo: failure \n"));
        return;
    }    
    
    TDAL_PVRi_ResetProgInfo(pProgInfo);

    if (MApi_PVR_GetProgramInfo(fileName, pProgInfo) == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_GetProgramInfo: failure\n"));
    }    
}

LOCAL void TDAL_PVRi_SetProgInfo(PVRProgramInfo_t *pProgInfo, void *pInfo)
{
    tTDAL_PVR_BasicProgInfo *pBasicInfo = (tTDAL_PVR_BasicProgInfo *)pInfo;
    int i = 0;
    if (pProgInfo==NULL || pInfo==NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRm_SetProgInfo: failure\n"));
        return;
    }
    
    if (pBasicInfo->u32VideoPid == INVALID_PID)
    {
        pProgInfo->enServiceType = EN_APIPVR_SERVICETYPE_RADIO;
    }
    else
    {
        pProgInfo->enServiceType = EN_APIPVR_SERVICETYPE_DTV;
    }
    
    pProgInfo->u32ACodec    = (MS_U32)TDAL_PVRi_ADEC_TDAL2MS((tTDAL_AV_AudioType)pBasicInfo->u32ACodec);
    pProgInfo->u16AudioPid  = (MS_U16)pBasicInfo->u32AudioPid;
    pProgInfo->enVCodec     = TDAL_PVRi_VDEC_TDAL2MS(pBasicInfo->u32VCodec);

    TDAL_PVRi_InsertFilter(&pProgInfo->Filters[0], (MS_U16)pBasicInfo->u32VideoPid, (MS_U32)DMX_FILTER_TYPE_VIDEO);
    TDAL_PVRi_InsertFilter(&pProgInfo->Filters[0], (MS_U16)pBasicInfo->u32PCRPid, (MS_U32)DMX_FILTER_TYPE_PCR);
    TDAL_PVRi_InsertFilter(&pProgInfo->Filters[0], (MS_U16)pBasicInfo->u32PmtPid, (MS_U32)DMX_FILTER_TYPE_SECTION);

    pProgInfo->u32LCN          = pBasicInfo->u32LCN;
    pProgInfo->u32StartTime    = MsOS_GetSystemTime();

    //First ACodec
    pProgInfo->u8AudioInfoNum           = pBasicInfo->nAudioInfoCount;
    pProgInfo->AudioInfo[0].u16AudPID   = (MS_U16)pBasicInfo->u32AudioPid;
    pProgInfo->AudioInfo[0].u8AudType   = (MS_U16)TDAL_PVRi_ADEC_TDAL2MS((tTDAL_AV_AudioType)pBasicInfo->u32ACodec);
    for(i=1; i<pBasicInfo->nAudioInfoCount; i++)
    {
        pProgInfo->AudioInfo[i].u16AudPID = (MS_U16)pBasicInfo->pAudioInfo[i].u16AudioPID;
        pProgInfo->AudioInfo[i].u8AudType = (MS_U16)TDAL_PVRi_ADEC_TDAL2MS((tTDAL_AV_AudioType)pBasicInfo->pAudioInfo[i].u8AudioType);
    }
    
    pProgInfo->u8EBUSubtitleNum = pBasicInfo->nSubsInfoCount;
    for(i=0; i<pBasicInfo->nSubsInfoCount; i++)
    {
        pProgInfo->DVBSubtInfo[i].u16Sub_Pid        = pBasicInfo->pSubsInfo[i].u16Sub_Pid;
        pProgInfo->DVBSubtInfo[i].u8ComponentTag    = pBasicInfo->pSubsInfo[i].u8ComponentTag;
        pProgInfo->DVBSubtInfo[i].u8subtitling_type = pBasicInfo->pSubsInfo[i].u8subtitling_type;
        pProgInfo->DVBSubtInfo[i].u8lanuage_code[0] = pBasicInfo->pSubsInfo[i].u8lanuage_code[0];
        pProgInfo->DVBSubtInfo[i].u8lanuage_code[1] = pBasicInfo->pSubsInfo[i].u8lanuage_code[1];
        pProgInfo->DVBSubtInfo[i].u8lanuage_code[2] = pBasicInfo->pSubsInfo[i].u8lanuage_code[2];
    }

    pProgInfo->bIsEncrypted        	= pBasicInfo->bIsEncrypted;
    pProgInfo->enEncryptionType     = TDAL_PVRm_ConvEncrType((uint8_t)pBasicInfo->eEncryptionType);
    TDAL_PVRm_SetCryptKey(pBasicInfo->u8CryptKey);

    //Insert as last item;
    TDAL_PVRi_InsertFilter(&pProgInfo->Filters[0], (MS_U16)0x00, (MS_U32)DMX_FILTER_TYPE_SECTION);
    TDAL_PVRi_ProgIdx = 0;
}

LOCAL void TDAL_PVRi_ResetProgInfo(PVRProgramInfo_t *pProgInfo)
{
    int i=0;

    memset(pProgInfo,0,sizeof(PVRProgramInfo_t));

    for(i=0;i<APIPVR_MAX_PID_NUM;i++)
    {
        pProgInfo->Filters[i].u16PID=INVALID_PID;
    }

    for(i=0;i<APIPVR_MAX_TTXINFO_NUM;i++)
    {
        pProgInfo->TXTInfo[i].u16TTX_Pid=INVALID_PID;
    }

    for(i=0;i<APIPVR_MAX_AUDIOINFO_NUM;i++)
    {
        pProgInfo->AudioInfo[i].u16AudPID=INVALID_PID;
        pProgInfo->AudioInfo[i].u8AudType = 0xFF;
        pProgInfo->AudioInfo[i].u8ISOLangInfo[0] = 0;
        pProgInfo->AudioInfo[i].u8ISOLangInfo[1] = 0;
        pProgInfo->AudioInfo[i].u8ISOLangInfo[2] = 0;
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        pProgInfo->DVBSubtInfo[i].u16Sub_Pid=INVALID_PID;
    }

    for(i=0;i<APIPVR_MAX_SUBTITLEINFO_NUM;i++)
    {
        pProgInfo->EBUSubtInfo[i].u16TTX_Pid=INVALID_PID;
    }
    pProgInfo->u16CCPid=INVALID_PID;
}

LOCAL uint32_t TDAL_PVRi_ADEC_TDAL2MS(tTDAL_AV_AudioType tdal_ACodec)
{
    switch (tdal_ACodec)
    {
        case eTDAL_AV_AUDIO_TYPE_MPEG:
            return EN_APIPVR_AUD_DVB_MPEG;
        case eTDAL_AV_AUDIO_TYPE_AC3:
            return EN_APIPVR_AUD_DVB_AC3;
        case eTDAL_AV_AUDIO_TYPE_HE_AAC:
            return EN_APIPVR_AUD_DVB_AC3P;
        case eTDAL_AV_AUDIO_TYPE_MPEG_AAC:
            return EN_APIPVR_AUD_DVB_AAC;
        default:
            break;
    }
    return EN_APIPVR_AUD_DVB_NONE;
}

LOCAL tTDAL_AV_AudioType TDAL_PVRi_ADEC_MS2TDAL(uint32_t tdal_ACodec)
{
    switch (tdal_ACodec)
    {
        case EN_APIPVR_AUD_DVB_MPEG:
            return eTDAL_AV_AUDIO_TYPE_MPEG;
        case EN_APIPVR_AUD_DVB_AC3:
            return eTDAL_AV_AUDIO_TYPE_AC3;
        case EN_APIPVR_AUD_DVB_AC3P:
            return eTDAL_AV_AUDIO_TYPE_HE_AAC;
        case EN_APIPVR_AUD_DVB_AAC:
            return eTDAL_AV_AUDIO_TYPE_MPEG_AAC;
        default:
            break;
    }
    return eTDAL_AV_AUDIO_TYPE_MPEG;
}


LOCAL APIPVR_CODEC_TYPE TDAL_PVRi_VDEC_TDAL2MS(uint32_t u32VCodec)
{
    switch (u32VCodec)
    {
        case eTDAL_AV_VIDEO_TYPE_MPEG2:
            return EN_APIPVR_CODEC_TYPE_MPEG2;
        case eTDAL_AV_VIDEO_TYPE_H264:
            return EN_APIPVR_CODEC_TYPE_H264;
        default:
            break;
    }
    return EN_APIPVR_CODEC_TYPE_NONE;
}

LOCAL tTDAL_AV_VideoType TDAL_PVRi_VDEC_MS2TDAL(uint32_t u32VCodec)
{
    switch (u32VCodec)
    {
        case EN_APIPVR_CODEC_TYPE_MPEG2:
            return eTDAL_AV_VIDEO_TYPE_MPEG2;
        case EN_APIPVR_CODEC_TYPE_H264:
            return eTDAL_AV_VIDEO_TYPE_H264;
        default:
            break;
    }
    return eTDAL_AV_VIDEO_TYPE_MPEG2;
}

LOCAL bool TDAL_PVRi_InsertFilter(PVRFilterInfo_t *pFilter, MS_U16 u16PID, MS_U32 u32DmxFltType)
{
    PVRFilterInfo_t *pflt= (PVRFilterInfo_t *)(pFilter);
    
    if (pflt == NULL)
    {
        return FALSE;
    }

    if (TDAL_PVRi_ProgIdx > APIPVR_MAX_PID_NUM)
    {
        return FALSE;
    }

    if (u16PID == INVALID_PID)
    {
        return FALSE;
    }

    pflt[TDAL_PVRi_ProgIdx].u16PID         = u16PID;
    pflt[TDAL_PVRi_ProgIdx].u32DmxFltType  = u32DmxFltType;
    TDAL_PVRi_ProgIdx++;

    return TRUE;
}

LOCAL uint16_t TDAL_PVRi_FindPidInFilter(PVRFilterInfo_t *pFilter, MS_U32 u32DmxFltType)
{
    int i=0;
    if(pFilter==NULL)
    {
        return INVALID_PID;
    }
    
    for(i=0; i<APIPVR_MAX_PID_NUM; i++)
    {
        if((pFilter[i].u32DmxFltType==u32DmxFltType) && (pFilter[i].u16PID!=0))
        {
            
            return pFilter[i].u16PID;
        }
    }
    
    return INVALID_PID;
}

LOCAL bool TDAL_PVRi_GetRecMountPath(bool bTimeshift, char *mountpath)
{
    int len = 0;
    char *path = mountpath;
    PVR_REC_PATH_IDX subPathIdx = PVR_NORMALREC_PATH_IDX;
    
    if(bTimeshift)
    {
        subPathIdx = PVR_TIMESHIFT_PATH_IDX;
    }

    if(strlen(TDAL_PVR_recFSInfo[subPathIdx].recPath)==0)
    {
        len = strlen(MApi_PVR_GetMouthPath());
        memcpy(path, MApi_PVR_GetMouthPath(),len);
    }
    else
    {
        sprintf(path,"%s/%s",MApi_PVR_GetMouthPath(),TDAL_PVR_recFSInfo[subPathIdx].recPath);
    }

    return TRUE;
}

LOCAL void TDAL_PVRi_Task(MS_U32 argc, void *argv)
{
    PVREventInfo_t pvrEvent;
    MS_U32 u32MessageSize ;
    memset((void *)&pvrEvent,0,sizeof(PVREventInfo_t));
    tTDAL_PVR_Handle pvrHandle = NULL;

    while(!TDAL_PVRi_EndTask)
    {
        if(TRUE==MsOS_RecvFromQueue(TDAL_PVRi_QueueID, (MS_U8 *)&pvrEvent, sizeof(PVREventInfo_t), &u32MessageSize, MSOS_WAIT_FOREVER))
        {
            /*TODO */
            int i = 0;
            for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
            {
                if (TDAL_PVR_Desc[i].bUsed==TRUE && TDAL_PVR_Desc[i].progIdx==pvrEvent.u32Data[0])
                {
                    pvrHandle = (tTDAL_PVR_Handle)&TDAL_PVR_Desc[i];
                    TDAL_PVRi_Callback(&pvrEvent, pvrHandle);
                    break;
                }
            }
        }
    }

    TDAL_PVRi_TaskFinished = TRUE;

}

LOCAL void TDAL_PVRi_Callback(PVREventInfo_t *event, tTDAL_PVR_Handle pvrHandle)
{
    int idx = 0;
    static MS_U32 u32PlaybackTime = 0;
    static MS_U32 u32PlaybackSize = 0;
    static MS_U32 u32RecordSize[APIPVR_MAX_PROGRAM_NUM] = {0,0};
    tTDAL_PVR_Desc  *pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
    static int recChunkCount = 0;
    static int curFileIdx = 0;
    static MS_U32 currentTime = 0;
    static MS_BOOL bRecThreshold = FALSE;
    
    switch(event->pvrEvent)
    {
        case EN_APIPVR_EVENT_NOTIFY_FILE_END:
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            if (!PVR_IS_VALID_HANDLER(pPVRDesc->pvrPlyHandle))
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "pPVRDesc->pvrPlyHandle is invalid!\n"));
            }
            
            if (MApi_PVR_IsPlaybacking() && 
                    (MApi_PVR_PlaybackGetSpeed(pPVRDesc->pvrPlyHandle)<=EN_APIPVR_PLAYBACK_SPEED_1XFB))
            {
                if (MApi_PVR_IsTimeShift())
                {
                    MApi_PVR_TimeshiftPlaybackStop(&pPVRDesc->pvrPlyHandle, pPVRDesc->progIdx);
                    /* TODO */
                    MApi_PVR_TimeshiftPlaybackStart(&pPVRDesc->pvrPlyHandle, pPVRDesc->pPVRProgInfo->FileName, 0, 0);
                }
                else
                {
                    MApi_PVR_PlaybackStop(&pPVRDesc->pvrPlyHandle);
                    /* TODO */
                }
            }
            else
            {
                if (pPVRDesc->FilesCount>1 && ++curFileIdx<pPVRDesc->FilesCount)
                {
                    TDAL_PVR_previousChunksTime = TDAL_PVR_playbackTime;
                    TDAL_PVRi_PlaybackNextChunk(pPVRDesc, curFileIdx);
                }
                else
                {
                    TDAL_PVR_previousChunksTime = 0;
                    /* Reset file chunks counter */
                    curFileIdx = 0;
                    /* This is done due to request of need to display black screen
                     * if usb disk is suddenly ejected and stopping is done here
                     * in lower layer because of some malfunctioning if stopping is left
                     * to upper layer of SW.
                     */
                    MApi_PVR_PlaybackStop(&pPVRDesc->pvrPlyHandle);
                    if (TDAL_PVR_Notification != NULL)
                    {
                        TDAL_PVR_Notification(eTDAL_PVR_END_FILE, pvrHandle);
                    }
                    TDAL_PVR_playbackTime = 0;
                }
            }
            break;
        }
        case EN_APIPVR_EVENT_ERROR_NO_DISK_SPACE:
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            if (TDAL_PVR_Notification != NULL)
                TDAL_PVR_Notification(eTDAL_PVR_NO_DISK_SPACE, pvrHandle);
            break;
        }
        case EN_APIPVR_EVENT_ERROR_READ_FILE_FAIL:
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            if (TDAL_PVR_Notification != NULL)
                TDAL_PVR_Notification(eTDAL_PVR_READ_FILE_FAIL, pvrHandle);
            break;
        }
        case EN_APIPVR_EVENT_ERROR_INVALID_ADDR:
        case EN_APIPVR_EVENT_ERROR_WRITE_FILE_FAIL:
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));

            if((TDAL_PVRm_GetDiskSpace(mountPath,&u64FreeSpaceInKB, &u64TotalSpaceInKB) == TRUE) &&
               (u64FreeSpaceInKB == 0))
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not enough disk space!\n"));
                if (TDAL_PVR_Notification != NULL)
                    TDAL_PVR_Notification(eTDAL_PVR_NO_DISK_SPACE, pvrHandle);
            }
            else
            {
                MApi_PVR_RecordStop(&pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Write to disk failed!\n"));
                if (TDAL_PVR_Notification != NULL)
                    TDAL_PVR_Notification(eTDAL_PVR_WRITE_FILE_FAIL, pvrHandle);
            }
            break;
        }
        case EN_APIPVR_EVENT_NOTIFY_RECORDED_TIME: //Second
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            if (event->u32Data[0] >= MAX_PROGRAM_NUM)
            {
                printf("Invalid Program Indx  (%lu)\n",event->u32Data[0]);
                break;
            }
            
            if (pPVRDesc == NULL)
            {
                /* something bad happened*/
                return;                
            }
            
            pPVRDesc->pPVRProgInfo->u32Duration = MApi_PVR_GetRecordTime(pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);
            if (!pPVRDesc->bIsTimeshift &&
                 pPVRDesc->bIsCaptured==FALSE &&
                 pPVRDesc->pPVRProgInfo->u32Duration>PVR_THUMBNAIL_CAPTURE_TIME)
            {
                TDAL_PVRi_CaptureWin(DWIN_SRC_OP, PVR_THUMBNAIL_CAPTURE_WIDTH, PVR_THUMBNAIL_CAPTURE_HEIGHT, pPVRDesc->pPVRProgInfo->FileName);
                pPVRDesc->bIsCaptured = TRUE;
            }
            break;
        }
        case EN_APIPVR_EVENT_NOTIFY_RECORDED_SIZE:  //KByte
        {   
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            if(event->u32Data[0]>=MAX_PROGRAM_NUM)
            {
                printf("Invalid Program Indx  (%lu)\n",event->u32Data[0]);
                break;
            }

            if (bRecThreshold)
            {
            	TDAL_PVRm_GetDiskSpace(mountPath,&u64FreeSpaceInKB, &u64TotalSpaceInKB);
                pPVRDesc->u64FreeSpaceInKB = u64FreeSpaceInKB - event->u32Data[1];
                u64FreeSpaceInKB = pPVRDesc->u64FreeSpaceInKB;
            }
            else
            {
               u64FreeSpaceInKB = pPVRDesc->u64FreeSpaceInKB - event->u32Data[1] + pPVRDesc->pPVRProgInfo->u64FileLength;
               pPVRDesc->u64FreeSpaceInKB = u64FreeSpaceInKB;
            }

            u32RecordSize[event->u32Data[0]] = event->u32Data[1];
            pPVRDesc->pPVRProgInfo->u64FileLength = event->u32Data[1];

            if (u64FreeSpaceInKB*100 <= (u64TotalSpaceInKB*TDAL_PVRi_FreeSpaceThreshold))
            {
            	bRecThreshold = TRUE;
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Record threshold reached (%lldKB of %lldKB)\n", u64FreeSpaceInKB, u64TotalSpaceInKB));

                if (TDAL_PVR_Notification != NULL)
                    TDAL_PVR_Notification(eTDAL_PVR_FILE_THRESHOLD, pvrHandle);
            }
            else
            {
            	bRecThreshold = FALSE;
            }

            if (u64FreeSpaceInKB < PVR_FILE_SIZE_TO_STOP)
            {

                if (TDAL_PVR_Notification)
                    TDAL_PVR_Notification(eTDAL_PVR_NO_DISK_SPACE, NULL);
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Disk Space is not enough!\n"));
                return;
            }

            if ((pPVRDesc->bIsTimeshift == FALSE) && (u32RecordSize[event->u32Data[0]] > PVR_FILE_CHUCK_MAX_SIZE))
            {
                /* In order to suppress retarded notifications from
                 * PVR driver in case of recording split in chunks 
                 */
                MS_U32 tmpTime = MsOS_GetSystemTime();
                if (tmpTime<currentTime || (tmpTime - currentTime)<3000)
                {
                    currentTime = MsOS_GetSystemTime();
                    return;
                }
                
                if (!TDAL_PVRi_RecordNextChunk(pPVRDesc))
                {
                    if (TDAL_PVR_Notification)
                        TDAL_PVR_Notification(eTDAL_PVR_RECORDING_ERROR, NULL);
                    return;
                }
                currentTime = MsOS_GetSystemTime();
            }
            break;
        }
        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_TIME:
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            u32PlaybackTime = event->u32Data[1];
            MS_U32 u32ErrFrame=MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_errFrmCnt);
            MS_U32 u32JumpToTimeInSeconds=u32PlaybackTime+3;
            /* TODO - show time */

            if (pPVRDesc == NULL)
            {
                /* something bad happened */
                return;
            }

            if (pPVRDesc->FilesCount > 1)
            {
                TDAL_PVR_playbackTime = TDAL_PVR_previousChunksTime;
                TDAL_PVR_playbackTime += MApi_PVR_GetPlaybackTimeInSec(pPVRDesc->pvrPlyHandle);
            }
            else
            {
                TDAL_PVR_playbackTime = MApi_PVR_GetPlaybackTimeInSec(pPVRDesc->pvrPlyHandle);
            }
            break;
        }
        case EN_APIPVR_EVENT_NOTIFY_PLAYBACK_SIZE:
        {
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            //printf("Playback size = %d\n", event->u32Data[1]);
            break;
        }
        case EN_APIPVR_EVENT_NOTIFY_FILE_BEGIN:
        {
            /* fix for rewinding back previous chunk,
             * mstar is sending this callback 2 times always
             * so skipping second call to prevent mw and app being notified that we are at the beginning of the file */
            static case_begin_count = 0;
            case_begin_count++;
            if(case_begin_count == 2)
            {
                case_begin_count = 0;
                break;
            }
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            if (pPVRDesc->FilesCount>1 && --curFileIdx>=0)
            {
                TDAL_PVRi_PlaybackPreviousChunk(pPVRDesc, curFileIdx);
                if (TDAL_PVR_previousChunksTime - pPVRDesc->pPVRProgInfo->u32Duration > 0)
                {
                    TDAL_PVR_previousChunksTime -= pPVRDesc->pPVRProgInfo->u32Duration;
                }
                else
                {
                    TDAL_PVR_previousChunksTime = 0;
                }
            }
            else
            {
                /* Reset file chunks counter */
                curFileIdx = 0;

                MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle, APIPVR_PLAYBACK_SPEED_1X);
                if (TDAL_PVR_Notification != NULL)
                    TDAL_PVR_Notification(eTDAL_PVR_BEGIN_FILE, pvrHandle);
            }
            break;
        }
        case EN_APIPVR_EVENT_ERROR_SYNCBYTE_ERROR:
        case EN_APIPVR_EVENT_ERROR_OVERRUN:
        case EN_APIPVR_EVENT_NOTIFY_THUMBNAIL_AUTO_CAPTURE:
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            break;
        default :
            mTBOX_TRACE((kTBOX_NIV_3, "Event occurred  - %d\n", event->pvrEvent));
            break;
    }
}

void TDAL_PVRi_EnableRouteForPlayback(int flow, int InSrc, int ClkInv, int ExtSync, int Paral, tTDAL_ProgInfoElem *pProgInfoElem)
{
    DMX_FLOW dmxFlow;
    DMX_FLOW_INPUT inSrc;
    MS_BOOL bClkInv, bExtSync, bParallel;

    switch (flow)
    {
        case 0:
            dmxFlow = DMX_FLOW_PLAYBACK;
            break;
        case 1:
            dmxFlow = DMX_FLOW_PVR;
            break;
        case 2:
            dmxFlow = DMX_FLOW_PVR1;
            break;
        default:

            printf("[%s][%d] unsupport flow %d\n", __FUNCTION__, __LINE__, flow);
            break;
    }
    switch (InSrc)
    {
        case 0:
            inSrc = DMX_FLOW_INPUT_DEMOD;
            break;
        case 1:
            inSrc = DMX_FLOW_INPUT_MEM;
            break;
        case 2:
            inSrc = DMX_FLOW_INPUT_EXT_INPUT0;
            break;
        case 3:
            inSrc = DMX_FLOW_INPUT_EXT_INPUT1;
            break;
#ifdef TS2_IF_SUPPORT
        case 4:
            inSrc = DMX_FLOW_INPUT_EXT_INPUT2;
            break;
#endif
        default:
            printf("[%s][%d] unsupport input source  %d\n", __FUNCTION__, __LINE__, InSrc);
            break;
    }
    bClkInv = (0 == ClkInv)? FALSE: TRUE;
    bExtSync = (0 == ExtSync)? FALSE: TRUE;
    bParallel = (0 == Paral)? FALSE: TRUE;
    if (DMX_FILTER_STATUS_OK != MApi_DMX_FlowSet(dmxFlow, inSrc, bClkInv, bExtSync, bParallel))
    {
        printf("[%s][%d] MApi_DMX_FlowSet fail\n", __FUNCTION__, __LINE__);
    }
#ifdef ENABLE_CI_PACKAGE
    MS_BOOL st;
#endif

    MApi_VDEC_EX_Rst((VDEC_StreamId *)&stStreamId);
    MApi_VDEC_EX_Exit((VDEC_StreamId *)&stStreamId);
    /* TODO - check if we need this setting for VDEC*/
    TDAL_AV_Playback_bIsFileIn = TRUE;
    if (TDAL_PVRi_PCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(TDAL_PVRi_PCRFltId);
        MApi_DMX_Close(TDAL_PVRi_PCRFltId);
        TDAL_PVRi_PCRFltId = INVALID_FILTER_ID;
    }

    if (TDAL_PVRi_VideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(TDAL_PVRi_VideoFltId);
        MApi_DMX_Close(TDAL_PVRi_VideoFltId);
        TDAL_PVRi_VideoFltId = INVALID_FILTER_ID;
    }

    if (TDAL_PVRi_AudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(TDAL_PVRi_AudioFltId);
        MApi_DMX_Close(TDAL_PVRi_AudioFltId);
        TDAL_PVRi_AudioFltId = INVALID_FILTER_ID;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_PCR | DMX_FILTER_SOURCE_TYPE_FILE, &TDAL_PVRi_PCRFltId))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Open failed\n"));
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(TDAL_PVRi_PCRFltId, &pProgInfoElem->u16PCRPID, TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Pid failed to set PCR_PID\n"));
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(TDAL_PVRi_PCRFltId))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Start failed\n"));
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_VIDEO | DMX_FILTER_SOURCE_TYPE_FILE, &TDAL_PVRi_VideoFltId))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Open failed\n"));
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(TDAL_PVRi_VideoFltId,&pProgInfoElem->u16VideoPID, TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Pid failed to set VPID\n"));
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(TDAL_PVRi_VideoFltId))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Start failed\n"));
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_AUDIO | DMX_FILTER_SOURCE_TYPE_FILE, &TDAL_PVRi_AudioFltId))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Open failed\n"));
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(TDAL_PVRi_AudioFltId, (MS_U16*)&pProgInfoElem->u16AudioPID, TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Pid failed to set APID\n"));
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(TDAL_PVRi_AudioFltId))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_SetRouteForPlayback: MApi_DMX_Start failed\n"));
    }
#ifdef ENABLE_CI_PACKAGE

    st = msAPP_CI_setCCFID(TDAL_PVRi_VideoFltId, TRUE);
    printf("[CI+] Set FID: %d,%d.\r\n", st, TDAL_PVRi_VideoFltId);
    st = msAPP_CI_setCCFID(TDAL_PVRi_AudioFltId, TRUE);
    printf("[CI+] Set FID: %d,%d.\r\n", st, TDAL_PVRi_AudioFltId);
    st = msAPP_CI_setCCFID(TDAL_PVRi_PCRFltId, TRUE);
    printf("[CI+] Set FID: %d,%d.\r\n", st, TDAL_PVRi_PCRFltId);
#endif
}

void TDAL_PVRi_DisableRouteForPlayback(void)
{
    DMX_FLOW dmxFlow = DMX_FLOW_PLAYBACK;
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
    if (TDAL_PVRi_PCRFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(TDAL_PVRi_PCRFltId);
        MApi_DMX_Close(TDAL_PVRi_PCRFltId);
        TDAL_PVRi_PCRFltId = INVALID_FILTER_ID;
    }

    if (TDAL_PVRi_VideoFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(TDAL_PVRi_VideoFltId);
        MApi_DMX_Close(TDAL_PVRi_VideoFltId);
        TDAL_PVRi_VideoFltId = INVALID_FILTER_ID;
    }

    if (TDAL_PVRi_AudioFltId != INVALID_FILTER_ID)
    {
        MApi_DMX_Stop(TDAL_PVRi_AudioFltId);
        MApi_DMX_Close(TDAL_PVRi_AudioFltId);
        TDAL_PVRi_AudioFltId = INVALID_FILTER_ID;
    }
    
    if (DMX_FILTER_STATUS_OK != MApi_DMX_FlowSet(dmxFlow, inSrc, clkInv, extSync, parallel))
    {
        printf("[%s][%d] MApi_DMX_FlowSet fail\n", __FUNCTION__, __LINE__);
    }
}

LOCAL APIPVR_PLAYBACK_SPEED TDAL_PVRi_TrickModeConv(tTDAL_PVR_Speed enSpeed)
{
    APIPVR_PLAYBACK_SPEED msSpeed = EN_APIPVR_PLAYBACK_SPEED_INVALID; 
    switch (enSpeed)
    {
    case eTDAL_PVR_SPEED_32X_BACKWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_32XFB;
        break;
    case eTDAL_PVR_SPEED_16X_BACKWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_16XFB;
        break;
    case eTDAL_PVR_SPEED_8X_BACKWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_8XFB;
        break;
    case eTDAL_PVR_SPEED_4X_BACKWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_4XFB;
        break;
    case eTDAL_PVR_SPEED_2X_BACKWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_2XFB;
        break;
    case eTDAL_PVR_SPEED_1X_BACKWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_1XFB;
        break;
    case eTDAL_PVR_SPEED_0X:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_0X;
        break;
    case eTDAL_PVR_SPEED_FREEZE:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_STEP_IN;
        break;
    case eTDAL_PVR_SPEED_0X25_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_4X;
        break;
    case eTDAL_PVR_SPEED_0X5_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_FF_1_2X;
        break;
    case eTDAL_PVR_SPEED_1X_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_1X;
        break;
    case eTDAL_PVR_SPEED_2X_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_2XFF;
        break;
    case eTDAL_PVR_SPEED_4X_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_4XFF;
        break;
    case eTDAL_PVR_SPEED_8X_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_8XFF;
        break;
    case eTDAL_PVR_SPEED_16X_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_16XFF;
        break;
    case eTDAL_PVR_SPEED_32X_FORWARD:
        msSpeed = EN_APIPVR_PLAYBACK_SPEED_32XFF;
        break;
    case eTDAL_PVR_SPEED_64X_FORWARD:
    case eTDAL_PVR_SPEED_64X_BACKWARD:
    case eTDAL_PVR_SPEED_0X5_BACKWARD:
    case eTDAL_PVR_SPEED_0X25_BACKWARD:
    default:
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not supported trick mode!\n"));
        break;
    }
    
    return msSpeed;
}


LOCAL tTDAL_PVR_Speed TDAL_PVRi_TrickModeConvBack(APIPVR_PLAYBACK_SPEED enSpeed)
{
	tTDAL_PVR_Speed msSpeed = eTDAL_PVR_SPEED_0X;

    switch (enSpeed)
    {
		case EN_APIPVR_PLAYBACK_SPEED_32XFB:
			msSpeed = eTDAL_PVR_SPEED_32X_BACKWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_16XFB:
			msSpeed = eTDAL_PVR_SPEED_16X_BACKWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_8XFB:
			msSpeed = eTDAL_PVR_SPEED_8X_BACKWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_4XFB:
			msSpeed = eTDAL_PVR_SPEED_4X_BACKWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_2XFB:
			msSpeed = eTDAL_PVR_SPEED_2X_BACKWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_1XFB:
			msSpeed = eTDAL_PVR_SPEED_1X_BACKWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_0X:
			msSpeed = eTDAL_PVR_SPEED_0X;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_STEP_IN:
			msSpeed = eTDAL_PVR_SPEED_FREEZE;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_FF_1_4X:
			msSpeed = eTDAL_PVR_SPEED_0X25_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_FF_1_2X:
			msSpeed = eTDAL_PVR_SPEED_0X5_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_1X:
			msSpeed = eTDAL_PVR_SPEED_1X_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_2XFF:
			msSpeed = eTDAL_PVR_SPEED_2X_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_4XFF:
			msSpeed = eTDAL_PVR_SPEED_4X_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_8XFF:
			msSpeed = eTDAL_PVR_SPEED_8X_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_16XFF:
			msSpeed = eTDAL_PVR_SPEED_16X_FORWARD;
			break;
		case EN_APIPVR_PLAYBACK_SPEED_32XFF:
			msSpeed = eTDAL_PVR_SPEED_32X_FORWARD;
			break;
		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not supported trick mode!\n"));
			break;
    }

    return msSpeed;
}

void TDAL_PVR_SetFreeSpaceThreshold(uint8_t percent)
{
    TDAL_PVRi_FreeSpaceThreshold = percent % 100;
}

uint8_t TDAL_PVR_GetFreeSpaceThreshold()
{
    return TDAL_PVRi_FreeSpaceThreshold;
}

uint32_t TDAL_PVR_GetPlaybackTime()
{
    return TDAL_PVR_playbackTime;
}

void TDAL_PVRi_PlaybackNextChunk(tTDAL_PVR_Desc *pPVRDesc, uint16_t curFileIdx)
{
    char nextFileChunk[128] = {0};
    char extension[5] = {0};
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    _bIsPlaySwitchChunk = TRUE;
    if (!MApi_PVR_PlaybackStop(&pPVRDesc->pvrPlyHandle))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackStop: failed to stop playback in order to take next file chunk for playback\n"));
    }
    uint8_t length = strlen(pPVRDesc->pPVRProgInfo->FileName);

    strcpy(extension, strchr(pPVRDesc->pPVRProgInfo->FileName, '.'));
    strncpy(nextFileChunk, pPVRDesc->pPVRProgInfo->FileName, length);
    sprintf(strchr(nextFileChunk, '.')-3, "%03d%s", curFileIdx, extension);
    
    TDAL_PVRi_GetProgInfo(pPVRDesc->pPVRProgInfo, nextFileChunk);
    //MApi_PVR_PlaybackABLoopReset();
    if (!MApi_PVR_PlaybackStart(&pPVRDesc->pvrPlyHandle, nextFileChunk, 0,0))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackStart: failed to start playback next file chunk\n"));
    }

    if (!MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle, pPVRDesc->eCurrentSpeed))
    {
        printf("MApi_PVR_PlaybackSetSpeed: failed to set for playback\n");
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackSetSpeed: failed to set for playback\n"));
    }
    _bIsPlaySwitchChunk = FALSE;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
}

void TDAL_PVRi_PlaybackPreviousChunk(tTDAL_PVR_Desc *pPVRDesc, uint16_t curFileIdx)
{
    char nextFileChunk[128] = {0};
    char extension[5] = {0};
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    _bIsPlaySwitchChunk = TRUE;
    if (!MApi_PVR_PlaybackStop(&pPVRDesc->pvrPlyHandle))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackStop: failed to stop playback in order to take next file chunk for playback\n"));
    }
    uint8_t length = strlen(pPVRDesc->pPVRProgInfo->FileName);

    strcpy(extension, strchr(pPVRDesc->pPVRProgInfo->FileName, '.'));
    strncpy(nextFileChunk, pPVRDesc->pPVRProgInfo->FileName, length);
    sprintf(strchr(nextFileChunk, '.')-3, "%03d%s", curFileIdx, extension);

    TDAL_PVRi_GetProgInfo(pPVRDesc->pPVRProgInfo, nextFileChunk);
    //MApi_PVR_PlaybackABLoopReset();
    if (!MApi_PVR_PlaybackStart(&pPVRDesc->pvrPlyHandle, nextFileChunk, pPVRDesc->pPVRProgInfo->u32Duration-5,0))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackStart: failed to start playback next file chunk\n"));
    }

    if (!MApi_PVR_PlaybackSetSpeed(pPVRDesc->pvrPlyHandle, pPVRDesc->eCurrentSpeed))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_PlaybackSetSpeed: failed to set for playback\n"));
    }
    _bIsPlaySwitchChunk = FALSE;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
}

bool TDAL_PVRi_RecordNextChunk(tTDAL_PVR_Desc *pPVRDesc)
{
    char nextFileChunk[128] = {0};
    char extension[5] = {0};
    uint16_t curFileIdx = pPVRDesc->FilesCount;
    uint8_t length = 0;
    TDAL_LockMutex(TDAL_PVRi_Mutex);
    pPVRDesc->u32FullRecDuration += MApi_PVR_GetRecordTime(pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);
    pPVRDesc->u64FullRecSizeInKB += pPVRDesc->pPVRProgInfo->u64FileLength;
    
    if (!MApi_PVR_RecordStop(&pPVRDesc->pvrRecHandle, pPVRDesc->progIdx))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_RecordStop: failed to stop recordin in order to make next file chunk\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        return FALSE;
    }    

    length = strlen(pPVRDesc->pPVRProgInfo->FileName);
    strcpy(extension, strchr(pPVRDesc->pPVRProgInfo->FileName, '.'));
    sprintf(strchr(pPVRDesc->pPVRProgInfo->FileName, '.')-3, "%03d%s", curFileIdx, extension);

    /* Set metadata in order to save settings for playback */
    if (!MApi_PVR_SetMetaData(pPVRDesc->pPVRProgInfo, pPVRDesc->progIdx, 0, 0, 0, 0))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_SetMetaData: failed to set program info for next file chunk\n"));
    }
    
    if (!MApi_PVR_RecordStart(&pPVRDesc->pvrRecHandle, pPVRDesc->progIdx, APIPVR_FILE_LINEAR, pPVRDesc->u64FreeSpaceInKB>>10, FALSE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_RecordStart: failed to start recording next file chunk\n"));
        TDAL_UnlockMutex(TDAL_PVRi_Mutex);
        return FALSE;
    }
    pPVRDesc->FilesCount = curFileIdx + 1;
    TDAL_UnlockMutex(TDAL_PVRi_Mutex);
    return TRUE;
}

void TDAL_PVRi_CaptureWin(uint8_t source, uint32_t width, uint32_t height, char *filename)
{
    GOP_DwinProperty       dwinProperty;
    MS_PHYADDR             phyDWinBufAddr;
    //Dwin buffer size set to panel width and height to capture the whole frame
    MS_U16                 u16DWinBufWidth = 0;
    MS_U16                 u16DWinBufHeight = 0;
    MS_U16                 u16DWinBufX = 0;
    MS_U16                 u16DWinBufY = 0;
    GOP_GwinFBAttr         bufAttr;
    EN_GOP_DWIN_SRC_SEL    eSource = (EN_GOP_DWIN_SRC_SEL)source;
    //Gwin size maybe different to panel size; GE_Bitblt with scaling to change size to display
    MS_U32                 u32Flag = GFXDRAW_FLAG_SCALE;
    GFX_BufferInfo         srcbufInfo;
    GFX_BufferInfo         dstbufInfo;
    GFX_DrawRect           drawrectInfo;

    MApi_GOP_DWIN_SetSourceSel(eSource);

    if (eSource == DWIN_SRC_OP)
    {
        //Source can only from Scaler OP
        //OP scan type is progressive
        tTDAL_DISP_Error error;
        tTDAL_DISP_LayerWindow pstInputWindow;
        tTDAL_DISP_LayerWindow pstOutputWindow;
        error = TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0, &pstInputWindow, &pstOutputWindow);
        if(error == eTDAL_DISP_NO_ERROR)
        {
            u16DWinBufWidth = pstOutputWindow.Right - pstOutputWindow.Left + 1;;
            u16DWinBufHeight = pstOutputWindow.Bottom - pstOutputWindow.Top + 1;
            u16DWinBufX = pstOutputWindow.Left;
            u16DWinBufY = pstOutputWindow.Top;
			//Thumbnail could not be acquired if input video Width is lower than 720
            if(u16DWinBufWidth < 720)
            {
                u16DWinBufX = 0;
                u16DWinBufY = 0;
                u16DWinBufWidth = 720;
                u16DWinBufHeight = 576;
            }
        }
        else
        {
            u16DWinBufX = 0;
            u16DWinBufY = 0;
            u16DWinBufWidth = 720;
            u16DWinBufHeight = 576;
        }
    }
    else if (eSource == DWIN_SRC_IP)
    {
     //Source can only from Scaler IP
     //Is the DWIN source is IP, we need to know the video size and interlace or progressive
     VDEC_EX_DispInfo   info;
     VDEC_EX_Result     ret;

     memset(&info, 0, sizeof(VDEC_EX_DispInfo));
     ret = MApi_VDEC_EX_GetDispInfo((VDEC_StreamId *)&stStreamId, &info);

     if (E_VDEC_EX_OK != ret)
     {
         /* TODO */
     }
     else
     {
         u16DWinBufWidth = info.u16HorSize;
         u16DWinBufHeight = info.u16VerSize;
     }
     //Interlaced video, 'dwinProperty.u16h' is set to 1/2 of video height
     //If the scan type is DWIN_SCAN_MODE_PROGRESSIVE, the dwin buffer size should be also set to 1/2
     //If the scan type is DWIN_SCAN_MODE_extern, the dwin buffer size should not change
     if (info.u8Interlace == 1)
     {
         u16DWinBufHeight = u16DWinBufHeight>>1;
     }
    }
    else
    {
     printf("This source number is not supported now.\n");
    }
    //(1)SCAN type: DWIN_SCAN_MODE_PROGRESSIVE
    // - Contiguous captured/Auto stop captured: In each vsync, capture the top or bottom field
    //(2)SCAN type: DWIN_SCAN_MODE_extern
    // - Contiguous captured: contiguously captured top and bottom field
    // - Auto stop captured: Hardware will auto stop once the bottom field is captured.    
    MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
    //Data format only YUV; Set data format to YUV422
    MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);

    MS_U8 captureFBId = 0xff;
    captureFBId = MApi_GOP_GWIN_GetFreeFBID();
    if (captureFBId == 0xff)
    {
     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is no free frame buffer to capture video"));
     return;
    }

    if (MApi_GOP_GWIN_CreateFB(captureFBId, 0, 0, u16DWinBufWidth, u16DWinBufHeight, E_MS_FMT_YUV422) != GOP_API_SUCCESS)
    {
     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failed to create frame buffer for capture!"));
     return;
    }

    MApi_GOP_GWIN_GetFBInfo(captureFBId, &bufAttr);
    MApi_GFX_ClearFrameBuffer(bufAttr.addr, bufAttr.size, 0x00);
    MApi_GFX_FlushQueue();

    phyDWinBufAddr = bufAttr.addr;

    // Set DWin property
    dwinProperty.u16x          = u16DWinBufX;
    dwinProperty.u16y          = u16DWinBufY;
    dwinProperty.u16fbw        = u16DWinBufWidth;
    dwinProperty.u16w          = u16DWinBufWidth;
    dwinProperty.u16h          = u16DWinBufHeight;
    dwinProperty.u32fbaddr0    = phyDWinBufAddr;
    dwinProperty.u32fbaddr1    = phyDWinBufAddr + bufAttr.size;

    MApi_GOP_DWIN_SetWinProperty(&dwinProperty);

    //Enable Dwin to capture each frame if CaptureOneFrame is 0
    MApi_GOP_DWIN_Enable(TRUE);
    MApi_GOP_DWIN_CaptureOneFrame();

    MS_U8 dstFBId = 0xff;
    dstFBId = MApi_GOP_GWIN_GetFreeFBID();
    if (dstFBId == 0xff)
    {
     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is no free frame buffer to capture video"));
     return;
    }

    if (MApi_GOP_GWIN_CreateFB(dstFBId, 0, 0, (uint16_t) width, (uint16_t) height, E_MS_FMT_ARGB8888) != GOP_API_SUCCESS)
    {
     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failed to create frame buffer for capture!"));
     return;
    }
    MApi_GOP_GWIN_GetFBInfo(dstFBId, &bufAttr);
    MApi_GFX_ClearFrameBuffer(bufAttr.addr, bufAttr.size, 0x00);
    MApi_GFX_FlushQueue();
    //set dst buffer

    // Bliting the captured bitmap to DRAM of GE
    srcbufInfo.u32ColorFmt = GFX_FMT_YUV422;
    srcbufInfo.u32Addr     = phyDWinBufAddr;
    srcbufInfo.u32Width    = u16DWinBufWidth;
    srcbufInfo.u32Height   = u16DWinBufHeight;
    srcbufInfo.u32Pitch    = u16DWinBufWidth<<1;
    MApi_GFX_SetSrcBufferInfo(&srcbufInfo, 0);

    dstbufInfo.u32ColorFmt = bufAttr.fbFmt;
    dstbufInfo.u32Addr     = bufAttr.addr;
    dstbufInfo.u32Width    = (uint16_t) width;
    dstbufInfo.u32Height   = (uint16_t) height;
    dstbufInfo.u32Pitch    = bufAttr.pitch;     
    MApi_GFX_SetDstBufferInfo(&dstbufInfo, 0);

    drawrectInfo.srcblk.x      = 0;
    drawrectInfo.srcblk.y      = 0;
    drawrectInfo.srcblk.width  = u16DWinBufWidth;
    drawrectInfo.srcblk.height = u16DWinBufHeight;

    drawrectInfo.dstblk.x      = 0;
    drawrectInfo.dstblk.y      = 0;
    drawrectInfo.dstblk.width  = (uint16_t) width;
    drawrectInfo.dstblk.height = (uint16_t) height;

    MApi_GFX_BitBlt(&drawrectInfo, u32Flag);
    MApi_GFX_FlushQueue();
    MApi_GOP_GWIN_DeleteFB(captureFBId);

    char screenNamePath[128] = {0};
    screenNamePath[0] = '/';
    strncpy(&screenNamePath[1], filename, strlen(filename));
    strncpy(strchr(screenNamePath, '.'), ".tmb", 4);
#if defined (TDAL_PVR_POSIX)
    int file = open(screenNamePath, O_CREAT | O_WRONLY);
    if(file < 0)
    {
     mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_PVRi_CaptureWin: Creating file %s failed.\n", screenNamePath));
     MApi_GOP_GWIN_DestroyFB(dstFBId);
    }
    else
    {
     write(file, (void *)&width, sizeof(width));
     write(file, (void *)&height, sizeof(height));
     write(file, (void *)MsOS_PA2KSEG1(bufAttr.addr), bufAttr.pitch*bufAttr.height);
     close(file);
    }
#else
    FILE *filePointer = MsFS_Fopen(screenNamePath, "w");
    if(filePointer == NULL)
    {
     printf("can't open file\n");
     MApi_GOP_GWIN_DestroyFB(dstFBId);
    }
    else
    {
     //MsFS_Fwrite((void *)&u32Timestamp, sizeof(u32Timestamp), 1, filePointer);
     //MsFS_Fwrite((void *)&u32capturepts, sizeof(u32capturepts), 1, filePointer);
     MsFS_Fwrite((void *)&width, sizeof(width), 1, filePointer);
     MsFS_Fwrite((void *)&height, sizeof(height), 1, filePointer);
     MsFS_Fwrite((void *)MsOS_PA2KSEG1(bufAttr.addr), 1, bufAttr.pitch*bufAttr.height, filePointer);
    }
    MsFS_Fflush(filePointer);
    MsFS_Fclose(filePointer);
#endif
    //Before leaving this function, FB has been created must delete
    MApi_GOP_GWIN_DeleteFB( dstFBId );

}

uint32_t TDAL_PVR_Record_GetDuration(tTDAL_PVR_Handle pvrHandle)
{
    tTDAL_PVR_Desc  *pPVRDesc = (tTDAL_PVR_Desc *)pvrHandle;
    uint32_t u32duration = 0;
    
    if (pPVRDesc == NULL)
    {
        return 0;
    }

    u32duration = pPVRDesc->u32FullRecDuration + MApi_PVR_GetRecordTime(pPVRDesc->pvrRecHandle, pPVRDesc->progIdx);

    if (bIsSeamlessPlayback)
    {
        if (_u32SeamlessStartTime > u32duration)
        {
            _u32SeamlessStartTime = u32duration;
        }
        u32duration -= _u32SeamlessStartTime;
    }
    
    return u32duration;
}

void TDAL_PVRi_GetDurationAndSizeContent(tTDAL_PVR_Desc *pPVRDesc, char *FullRecordedFileName)
{
    if (pPVRDesc==NULL || FullRecordedFileName==NULL)
    {
        return;
    }
    int i = 0;
    uint32_t duration = 0;
    uint64_t size = 0;
    char *ext = NULL;
    ext = strchr(FullRecordedFileName,'.');
    
    /* Getting information about recorded content from embedded metadata file*/
    for(i = 0; i < pPVRDesc->FilesCount; i++)
    {
        sprintf(strchr(FullRecordedFileName,'.')-3,"%03d%s", i, ext);
        TDAL_PVRi_ResetProgInfo(pPVRDesc->pPVRProgInfo);
        if (MApi_PVR_GetProgramInfo(FullRecordedFileName, pPVRDesc->pPVRProgInfo) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_GetProgramInfo: failure\n"));
        }
        duration    += pPVRDesc->pPVRProgInfo->u32Duration;
        size        += pPVRDesc->pPVRProgInfo->u64FileLength;
    }
    pPVRDesc->u32FullRecDuration = duration;
    pPVRDesc->u64FullRecSizeInKB = size;
}

uint32_t TDAL_PVRi_GetDurationOfContent(tTDAL_PVR_Desc *pPVRDesc, char *FullRecordedFileName)
{
    if (pPVRDesc==NULL || FullRecordedFileName==NULL)
    {
        return 0;
    }
    int i = 0;
    uint32_t duration = 0;
    char *ext = NULL;
    ext = strchr(FullRecordedFileName,'.');
    
    /* Getting information about recorded content from embedded metadata file*/
    for(i = 0; i < pPVRDesc->FilesCount; i++)
    {
        sprintf(strchr(FullRecordedFileName,'.')-3,"%03d%s", i, ext);
        TDAL_PVRi_ResetProgInfo(pPVRDesc->pPVRProgInfo);
        if (MApi_PVR_GetProgramInfo(FullRecordedFileName, pPVRDesc->pPVRProgInfo) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_GetProgramInfo: failure\n"));
        }
        duration += pPVRDesc->pPVRProgInfo->u32Duration;
    }
    pPVRDesc->u32FullRecDuration = duration;
    
    return duration;
}

uint64_t TDAL_PVRi_GetSizeOfContent(tTDAL_PVR_Desc *pPVRDesc, char *FullRecordedFileName)
{
    if (pPVRDesc==NULL || FullRecordedFileName==NULL)
    {
        return 0;
    }
    int i = 0;
    uint64_t size = 0;
    char *ext = NULL;
    ext = strchr(FullRecordedFileName,'.');
    /* Getting information about recorded content from embedded metadata file*/
    for(i = 0; i < pPVRDesc->FilesCount; i++)
    {
        sprintf(strchr(FullRecordedFileName,'.')-3,"%03d%s", i, ext);
        TDAL_PVRi_ResetProgInfo(pPVRDesc->pPVRProgInfo);
        if (MApi_PVR_GetProgramInfo(FullRecordedFileName, pPVRDesc->pPVRProgInfo) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_GetProgramInfo: failure\n"));
        }
        size += pPVRDesc->pPVRProgInfo->u64FileLength;
    }
    pPVRDesc->u64FullRecSizeInKB = size;
    
    return size;
}


void TDAL_PVRm_UsbRemoved()
{
    tTDAL_PVR_Handle pvrHandle = NULL;
    /*TODO */
    int i = 0;
    for(i=0; i<PVR_MAX_RECORDING_FILE; i++)
    {
        if (TDAL_PVR_Desc[i].bUsed==TRUE && TDAL_PVR_Desc[i].bIsTimeshift==FALSE && TDAL_PVR_Desc[i].pvrPlyHandle!=0xFF)
        {
            pvrHandle = (tTDAL_PVR_Handle)&TDAL_PVR_Desc[i];
            if (TDAL_PVR_Notification != NULL)
                TDAL_PVR_Notification(eTDAL_PVR_READ_FILE_FAIL, pvrHandle);
            break;
        }
    }
}

LOCAL void TDAL_PVRi_ControlChannel_TDAL_DMX(bool enable)
{
    tTDAL_DMX_Error dmx_err;
    MS_U16 videopid = 0;
    MS_U16 pcrpid = 0;
    MS_U16 audiopid = 0;
    
    videopid = TDAL_PVRi_FindPidInFilter(TDAL_PVR_livePromInfo.Filters, DMX_FILTER_TYPE_VIDEO);
    pcrpid = TDAL_PVRi_FindPidInFilter(TDAL_PVR_livePromInfo.Filters, DMX_FILTER_TYPE_PCR);
    audiopid = TDAL_PVR_livePromInfo.u16AudioPid;

    printf("[%s][%d] VideoPid=%d AudioPid=%d PCRPid =%d\n",__FUNCTION__,__LINE__,videopid,audiopid,pcrpid);
    
    if(enable == TRUE)
    {
        dmx_err = TDAL_DMX_Set_Channel_PID(VideoChannel, videopid);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on video channel");
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = %d\n", videopid));

        dmx_err = TDAL_DMX_Control_Channel(VideoChannel, eTDAL_DMX_CTRL_ENABLE);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control video channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(AudioChannel, audiopid);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on audio channel");
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = %d\n", audiopid));

        dmx_err = TDAL_DMX_Control_Channel(AudioChannel, eTDAL_DMX_CTRL_ENABLE);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control audio channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(PCRChannel, pcrpid);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on pcr channel");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = %d\n", pcrpid));

        dmx_err = TDAL_DMX_Control_Channel(PCRChannel, eTDAL_DMX_CTRL_ENABLE);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control pcr channel");

    }
    else
    {
        dmx_err = TDAL_DMX_Control_Channel(VideoChannel, eTDAL_DMX_CTRL_DISABLE);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control video channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(VideoChannel, 0x1FFF);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on video channel");
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = 0x1FFF\n"));

        dmx_err = TDAL_DMX_Control_Channel(AudioChannel, eTDAL_DMX_CTRL_DISABLE);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control audio channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(AudioChannel, 0x1FFF);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on audio channel");
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = 0x1FFF\n"));

        dmx_err = TDAL_DMX_Control_Channel(PCRChannel, eTDAL_DMX_CTRL_DISABLE);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control pcr channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(PCRChannel, 0x1FFF);
        //TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on pcr channel");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = 0x1FFF\n"));
    }    
}
bool TDAL_PVRm_ResetPVRRecFilters(void)
{
    int i = 0;
    for (i = 0 ; i < 128 ; i++)
    {
        PVRRecFilters[i].u8FilterID = INVALID_FILTER_ID;
        PVRRecFilters[i].u16PID = INVALID_PID;
    }
    return TRUE;
}

tTDAL_PVR_Rec_Filters *TDAL_PVRm_GetPVRRecFilters(void)
{
    int i = 0;
    PVRPL_Rec_Filters *filters = NULL;
    
    filters = PVRPL_Record_GetFilters();

    for (i = 0 ; i < 128 ; i++)
    {
        if (filters[i].u8FilterID == INVALID_FILTER_ID 
            && filters[i].u16PID == INVALID_PID)
        {
            break;
        }
        PVRRecFilters[i].u8FilterID = filters[i].u8FilterID;
        PVRRecFilters[i].u16PID = filters[i].u16PID;
    }
    return PVRRecFilters;
}
tTDAL_PVR_Error TDAL_PVRi_SetBGRecProgramInfo(void)
{
    int32_t VideoPid = 0,PCRPid = 0,AudioPid = 0;

    TDAL_DMXi_GetLiveChannelPID(&VideoPid, &AudioPid, &PCRPid);
    TDAL_PVRi_InsertFilter(&TDAL_PVR_bgrecPromInfo.Filters[0], (MS_U16)VideoPid, (MS_U32)DMX_FILTER_TYPE_VIDEO);
    TDAL_PVRi_InsertFilter(&TDAL_PVR_bgrecPromInfo.Filters[0], (MS_U16)PCRPid, (MS_U32)DMX_FILTER_TYPE_PCR);
    TDAL_PVRi_InsertFilter(&TDAL_PVR_bgrecPromInfo.Filters[0], (MS_U16)0x00, (MS_U32)DMX_FILTER_TYPE_SECTION);

    TDAL_PVR_bgrecPromInfo.enVCodec = TDAL_PVRi_VDEC_TDAL2MS(TDAL_AV_VideoGetStreamType());
    TDAL_PVR_bgrecPromInfo.u32ACodec = TDAL_PVRi_ADEC_TDAL2MS(TDAL_AV_Audio_StreamType());
    TDAL_PVR_bgrecPromInfo.u16AudioPid = AudioPid;
    TDAL_PVR_bgrecPromInfo.u16CCPid = 0x1FFF;
    TDAL_PVR_bgrecPromInfo.u32LCN = 1;
    TDAL_PVR_bgrecPromInfo.enServiceType = EN_APIPVR_SERVICETYPE_DTV;
    TDAL_PVR_bgrecPromInfo.bIsEncrypted = TRUE;
    TDAL_PVR_bgrecPromInfo.enEncryptionType = EN_APIPVR_ENCRYPTION_SMARTCARD;
    TDAL_PVRi_ProgIdx = 0;
}
tTDAL_PVR_Error TDAL_PVRi_BackGround_Record_Start(void)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    PVR_AV_ID_t AVStreamID;
    
    mTBOX_FCT_ENTER("TDAL_PVRi_BackGround_Record_Start");
    
    if (bIsSeamlessPlayback)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "BackGround Recordingl!!\n"));
        //TDAL_PVRi_BackGround_Record_Stop();
    }
    
    TDAL_PVRi_SetBGRecProgramInfo();

    if(!MApi_PVR_SetBackRecordMetaData(&TDAL_PVR_bgrecPromInfo, progbgIdx,0,0,0,0))
    {        
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Set MetaData Fail!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);
    }
    
    AVStreamID.vstreamID = (void *)(&_NullVidStreamID);
    AVStreamID.astreamID = (void *)(&_NullAudStreamID);
    
    if (!MApi_PVR_EX_BackRecordEngSet(&pvrbgRecHandle, (void *)(&AVStreamID), progbgIdx, APIPVR_FILE_CIRCULAR, 0,0))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_BackRecordEngSet Fail!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);        
    }
    
    if (!MApi_PVR_EX_RecordEngEnable(&pvrbgRecHandle, FALSE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_EX_RecordEngEnable Fail!!\n"));
        error = eTDAL_PVR_ERROR_NOT_DONE;
        mTBOX_RETURN(error);       
    }

    bIsSeamlessPlayback = TRUE;
    _u32SeamlessStartTime = MsOS_GetSystemTime();
    mTBOX_RETURN(error); 
}
tTDAL_PVR_Error TDAL_PVRi_BackGround_Record_Stop(void)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_PVRi_BackGround_Record_Stop");

    if (!MApi_PVR_BackRecordStop(&pvrbgRecHandle, progbgIdx))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_PVR_BackRecordStop Fail!!\n"));
        mTBOX_RETURN(error);
    }
    bIsSeamlessPlayback = FALSE;
    _u32SeamlessStartTime = 0;
    memset(&TDAL_PVR_bgrecPromInfo, 0, sizeof(PVRProgramInfo_t));
    mTBOX_RETURN(error); 
}
MS_BOOL TDAL_PVRi_PlaybackSwitchChunk(void)
{
    return _bIsPlaySwitchChunk;
}
MS_BOOL TDAL_PVRi_SeamlessPlayback(void)
{
    return bIsSeamlessPlayback;
}
