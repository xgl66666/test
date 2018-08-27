/*****************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_AV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_av/tdal_av_video.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : AudioVideo TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES                                                                *
 ****************************************************************************/
/* Generic Headers */
#include <stdio.h>
#include "string.h" /* needed for memcpy() */

#include "MsCommon.h"

#include "tbox.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "tdal_av.h"
#include "tdal_av_p.h"
#include "tdal_mp_module_priv.h"
#include "MsMemory.h"
#include "drvMVOP.h"
#include "apiVDEC_EX.h"
#include "apiDMX.h"

#include "drvXC_IOPort.h"
#include "apiXC_EX.h"

#include "MsTypes2.h"
//#include "vdec/msAPI_Video.h" Evan
#include "drvAUDIO_if.h"
#include "drvTVEncoder.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "xc/msAPI_VE.h"
#include "xc/msAPI_XC_EX.h"
#include "apiXC.h"

#ifdef PACKAGE_PVR
#include "apiPVR.h"
#include "tdal_pvr.h"
#include "tdal_pvr_p.h"
#include "PVRPL_Video.h"
#endif

#ifdef ENABLE_MM_PACKAGE
//#include "msAPI_MM.h" Evan
#include "tdal_mp.h"
#endif

bool bGstreamerUglyHackReset = FALSE;
bool bResolutionAlreadyGet = FALSE;

/****************************************************************************
 *  DEFINES   *
 ****************************************************************************/
#define TDAL_AVi_VIDEO_EVENTS (E_VDEC_EX_EVENT_VIDEO_UNMUTE | E_VDEC_EX_EVENT_FIRST_FRAME | E_VDEC_EX_EVENT_SEQ_HDR_FOUND | E_VDEC_EX_EVENT_DISP_INFO_RDY | E_VDEC_EX_EVENT_USER_DATA_FOUND | E_VDEC_EX_EVENT_DISP_INFO_CHG)

#define kTDAL_AVi_VIDEO_DECODER eTDAL_AV_DECODER_VIDEO_1

#define AVSYNC_TIMEOUT_INTERVAL_H264         3000
#define AVSYNC_TIMEOUT_INTERVAL_OTHER       3000

volatile bool                   TDAL_AV_Started;
volatile bool                   TDAL_AV_Freeze;
volatile bool                   TDAL_AV_Playback_bIsFileIn = false;

#undef TDAL_AV_DBGTASK //vdec dbg log
//#define TDAL_AV_DBGTASK  1
//#define FAST_ZAPPER_ENABLE 1
/****************************************************************************
 *  MACROS   *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_AV);
/****************************************************************************
 *  TYPEDEFS   *
 ****************************************************************************/

typedef struct
{
  //tTDAL_AVi_VideoState   State;
  tTDAL_AV_VideoType   StreamType;
  tTDAL_AV_PictureType   PictureType;
  tTDAL_AV_VideoAspectRatio   AspectRatio;
  //tTDAL_AV_DisplayAspectRatioConversion AspectRatioConversion;
  //tTDAL_AV_AspectRatioTV   AspectRatioTv;
  tTDAL_AV_VideoResolution   VideoResolution;
  tTDAL_AV_VideoCapability   Capability;
  tTDAL_AV_CallbackProc_t   NeedDisplayCallback;
  tTDAL_AV_CallbackProc_t   UserDataCallback;
  tTDAL_AV_CallbackProc_t   AspectRatioChangeCallback;
  tTDAL_AV_CallbackProc_t   VideoSampleStoppedCallback;
  tTDAL_AV_CallbackProc_t   ResolutionChangeCallback;
  tTDAL_AV_CallbackProc_t   PicturePlayedCallback;
  tTDAL_AV_CallbackProc_t   PictureStoppedCallback;
  tTDAL_AV_CallbackProc_t   PesScrambledBitChangeCallback;
  tTDAL_AV_CallbackProc_t   FrameRateChangeCallback;
}tTDAL_AVi_VideoDecoderDescriptor;

typedef enum
{
    eTDAL_AVi_AvSync_Done,
    eTDAL_AVi_AvSync_Freerun,
    eTDAL_AVi_AvSync_Not_Done,
} tTDAL_AVi_AvSyncStatus;


/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE)   *
 ****************************************************************************/
 extern MSAPI_XC_DEVICE_ID G_stXC_DeviceId[2];
/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)   *
 ****************************************************************************/

LOCAL MS_U32                   VDES_AVSYNCTIME      = 450000;
LOCAL   tTDAL_AVi_VideoDecoderDescriptor   TDAL_AV_VideoDecoderDescriptor;
LOCAL VDEC_EX_DispInfo   TDAL_AVi_VideoDispInfo;

LOCAL uint8_t   TDAL_AV_CurrentAFD;

VDEC_StreamId stStreamId = {0};

LOCAL tTDAL_AV_DataMissingOnPidCallback DataMissingOnPidCallback = NULL;
LOCAL MS_BOOL g_Demo_IsIframedecoding = FALSE;
LOCAL MS_BOOL _bGetFirstFrame = FALSE;
static MS_BOOL _bVideo_PVRPlay = FALSE;
/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)   *
 ****************************************************************************/
LOCAL void p_TDAL_AV_Analyse_UserData(tTDAL_AV_Decoder decoder, uint8_t *UserData_Buffer, MS_U32 bufferSize);
LOCAL tTDAL_AV_Error p_TDAL_AV_Video_UnregisterEvents(void);
LOCAL tTDAL_AV_Error p_TDAL_AV_Video_RegisterEvents(void);
LOCAL void p_TDAL_AV_ExtractFlagList(MS_U32 events, MS_U32 eventArray[], MS_U32 eventArrayLength, MS_U32 * flagsFound);
LOCAL VDEC_StreamId* TDAL_AVi_VideoStartCB(MS_U32 VCodec, MS_BOOL bStart);
bool TDAL_AV_VideoStarted(void);
bool TDAL_AVm_GetDecoderStatus(VDEC_EX_DispInfo * pVidStatus);
void p_TDAL_AV_VideoEventCallback(MS_U32 eFlag, void *param);

/**========================================================================**
 * Function   : TDAL_AV_VideoFreeze
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
#ifdef TDAL_AV_DBGTASK //vdec dbg log
#define kTDAL_AVi_VIDEO_DBGTASK_STACK_SIZE (2*4096)
TDAL_task_id          TDAL_AVi_VideoDBG = NULL;
uint8_t               TDAL_AVi_VideoDBGStack[kTDAL_AVi_VIDEO_DBGTASK_STACK_SIZE];
LOCAL bool            TDAL_AVi_VideoDBGrStop = FALSE;
void TDAL_AVi_VideoDBGTaskTaskFct(void)
{
    while (TDAL_AVi_VideoDBGrStop == FALSE)
    {
        uint32_t Frm=0,Err=0,Skip=0,Drop=0,Disp=0;
        uint32_t Wptr=0,Rptr=0;
        int32_t ddata=0;
        int32_t video_fifo0 = 0;
        uint32_t fileints=0,playbackts=0,vpts=0,u32stc=0,u32stc32=0,u32apts=0,u32vtd=0,u32atd=0, u32pcr32=0,u32pcr=0;
        uint64_t u64apts=0;
        VDEC_EX_Result bret=100;
        VDEC_EX_Status pStatus;
        MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO, &video_fifo0);    
        printf("stream id = %lu\n",stStreamId.u32Id);
        MApi_VDEC_EX_GetStatus(&stStreamId,&pStatus);
        printf("idle=%d init=%d stage=%d\n",pStatus.bIdle,pStatus.bInit,pStatus.eStage);

        vpts = MApi_VDEC_EX_GetPTS(&stStreamId);

        MApi_DMX_Stc_Eng_Get(0, &u32stc32, &u32stc);
        u32stc/=90;

        u64apts = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_33Bit_PTS);
        u32apts = (uint32_t)(u64apts & 0x00000000FFFFFFFF)/90;

        if (vpts > u32stc)
        {
            u32vtd = vpts - u32stc;
        }
        else
        {
            u32vtd = u32stc - vpts;
        }

        if (u32apts > u32stc)
        {
            u32atd = u32apts - u32stc;
        }
        else
        {
            u32atd = u32stc - u32apts;
        }

        MApi_DMX_Pcr_Eng_Get(0, &u32pcr32, &u32pcr);
        u32pcr/=90;
        printf("(VPTS:STC:PCR:APTS)=(%8lu:%8lu:%8lu:%8lu) (VTD:ATD)=(%8lu:%8lu)\n",vpts,u32stc,u32pcr,u32apts,u32vtd,u32atd);

        printf("(Frm,Err,Skip,Drop,Disp)=(%8lu,%8lu,%8lu,%8lu,%8lu)\n",MApi_VDEC_EX_GetFrameCnt(&stStreamId),MApi_VDEC_EX_GetErrCnt(&stStreamId),MApi_VDEC_EX_GetSkipCnt(&stStreamId),MApi_VDEC_EX_GetDropCnt(&stStreamId),MApi_VDEC_EX_GetDispCnt(&stStreamId));
        printf("(w,r)=(%8lX,%8lX)\n",MApi_VDEC_EX_GetESWritePtr(&stStreamId),MApi_VDEC_EX_GetESReadPtr(&stStreamId));
        Wptr = MApi_VDEC_EX_GetESWritePtr(&stStreamId);
        Rptr = MApi_VDEC_EX_GetESReadPtr(&stStreamId);
        ddata = (int32_t)Wptr - (int32_t)Rptr;
        if (ddata < 0)
            ddata+= VDEC_BIT_STREAM_LEN;
        printf("ddata=%ld\n",ddata);
        printf("VIDE1 State: 0x%lX\n", video_fifo0);
        printf("VIDE1 LEVEL: 0x%lX\n", (video_fifo0& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT);
        printf("VIDE1 EMPTY: 0x%lX\n", video_fifo0& DMX_FIFO_STATUS_LV_EMPTY);

        MApi_DMX_Filein_Eng_GetFileInTimeStamp(0,&fileints);
        MApi_DMX_Filein_Eng_GetPlaybackStamp(0,&playbackts);
        printf("FileInTS=%lu PlaybackTs=%lu\n",fileints,playbackts);
        printf("SyncOn:ReachSync %d:%d\n",MApi_VDEC_EX_IsAVSyncOn(&stStreamId), MApi_VDEC_EX_IsReachSync(&stStreamId));
        MsOS_DelayTask(500);
    }
    TDAL_AVi_VideoDBGrStop = FALSE;
    TDAL_AVi_VideoDBG = NULL;
}
#endif 
void* Demo_VDEC_GetStreamID(int eDevice)
{
    return (void *)&stStreamId;
}

tTDAL_AV_Error   TDAL_AV_VideoFreeze( tTDAL_AV_Decoder decoder )
{
	VDEC_EX_Result vdecResult;
	tTDAL_AV_Error error = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_AV_VideoFreeze");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	vdecResult = MApi_VDEC_EX_SetFreezeDisp(&stStreamId, TRUE);
	if (vdecResult != E_VDEC_EX_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_AV_VideoFreeze]  MApi_VDEC_SetFreezeDisp() returns %d\n", vdecResult));
		error = eTDAL_AV_ERROR;
	}
	
	//fix to enable freezed video layer output
	msAPI_XC_SetVideoMuteByMode(DISABLE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);
       TDAL_AV_Freeze = TRUE;
	mTBOX_RETURN(error);

}
tTDAL_AV_Error   TDAL_AV_VideoUnFreeze( tTDAL_AV_Decoder decoder )
{
	VDEC_EX_Result vdecResult;
	tTDAL_AV_Error error = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_AV_VideoFreeze");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	vdecResult = MApi_VDEC_EX_SetFreezeDisp(&stStreamId, FALSE);
	if (vdecResult != E_VDEC_EX_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_AV_VideoFreeze]  MApi_VDEC_SetFreezeDisp() returns %d\n", vdecResult));
		error = eTDAL_AV_ERROR;
	}

	//fix to enable freezed video layer output
	msAPI_XC_SetVideoMuteByMode(DISABLE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE);
       TDAL_AV_Freeze = FALSE;
	mTBOX_RETURN(error);

}
/**========================================================================**
 * Function   : TDAL_AV_VideoCapabilityGet
 *
 * Description  : return the video config
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_VideoCapabilityGet(tTDAL_AV_Decoder decoder, tTDAL_AV_VideoCapability *pstCapability)
{
	mTBOX_FCT_ENTER("TDAL_AV_VideoCapabilityGet");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	if (pstCapability == NULL)
	{
		mTBOX_RETURN(eTDAL_AV_BAD_PARAMETER_ERROR);
	}

	/* copy already initialized fields */
	memcpy(pstCapability, &TDAL_AV_VideoDecoderDescriptor.Capability,
			sizeof(tTDAL_AV_VideoCapability));

	mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : TDAL_AV_VideoPictureCaptureGet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_VideoPictureCaptureGet(tTDAL_AV_Decoder decoder,tTDAL_AV_PictureType pictureType,uint8_t** ppData,uint32_t *pSize)
{
return(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : TDAL_AV_VideoPictureCaptureRelease
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_VideoPictureCaptureRelease(tTDAL_AV_Decoder decoder,tTDAL_AV_PictureType pictureType,uint8_t* pData,uint32_t size)
{
return(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : TDAL_AV_VideoDestinationSet
 *
 * Description  : This function attaches a decoder to a video display layer
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_VideoDestinationSet(tTDAL_AV_Decoder decoder, uint32_t layerHandle)
{
return(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : TDAL_AV_VideoPictureStart
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_VideoPictureStart(tTDAL_AV_Decoder decoder,
   tTDAL_AV_PictureType pictureType,
   uint8_t* pData,uint32_t size,
   tTDAL_AV_VideoPictureParams *pstPictureParams)
{
    return(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : TDAL_AV_VideoPictureStop
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_VideoPictureStop(tTDAL_AV_Decoder decoder)
{
    return(eTDAL_AV_NO_ERROR);
}

/**======================================================================**
 * Function Name : TDAL_AV_VideoStatisticsTask
 *
 * Description   :
 *
 * Comment   :
 *
 **======================================================================**/
void   TDAL_AV_VIDEO_StatisticsTask( void * param )
{
    return;
}

bool TDAL_AV_Video_ResetFlow()
{
	MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE); //For playback
	MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
	MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);

	MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_MEM ,FALSE,TRUE,TRUE);

	//usleep(800 * 1000);

	MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_DEMOD ,FALSE,TRUE,TRUE);


	return true;

}

/**========================================================================**
 * Function   : p_TDAL_AV_Video_Start
 *
 * Scope   : private to module TDAL_AV
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error   p_TDAL_AV_Video_Start( tTDAL_AV_Decoder decoder , tTDAL_AV_StreamType StreamType, bool bIsInternal, bool bIsSlaveMode)
{

#if 1//ENABLE_MIU_1
        MS_U32 u32MiuInterval = MIU_INTERVAL;
#else
        MS_U32 u32MiuInterval = 0;
#endif
        tTDAL_AV_Error   Error = eTDAL_AV_NO_ERROR;
        VDEC_EX_Result bRes;
        MS_BOOL bRet;
    
        VDEC_EX_CodecType codecType;
        VDEC_EX_CodecType codecTypeCur;
        VDEC_EX_Status vdecStatus;
        VDEC_EX_InitParam initParam;
        VDEC_EX_Status stVedcStatus = {0};
        VDEC_EX_Slow_Sync Slow_Sync;

        memset(&Slow_Sync,0,sizeof(VDEC_EX_Slow_Sync));
        bool vdecInitalized = false;
        MS_U32 u32ErrorTolerance = 0;
        mTBOX_FCT_ENTER("p_TDAL_AV_Video_Start");
        if (TDAL_AV_Freeze)
        {
            TDAL_AV_VideoUnFreeze(decoder);
            mTBOX_RETURN(eTDAL_AV_NO_ERROR); 
        }
        if (TDAL_AV_Started == TRUE)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] AV is already running!\n" ));
            mTBOX_RETURN(eTDAL_AV_NO_ERROR);            
        }

        // MApi_VDEC_EX_SetDbgLevel(&stStreamId, E_VDEC_EX_DBG_LEVEL_TRACE);
    
        memset(&TDAL_AVi_VideoDispInfo, 0, sizeof(TDAL_AVi_VideoDispInfo));
        TDAL_AV_CurrentAFD   = 0;
    
        bRet = MApi_VDEC_EX_GetStatus(&stStreamId, &stVedcStatus);
    
#if 0
        //if(bGstreamerUglyHackReset == TRUE) //Fixes to recover from GStreamer playback
        {
    
            TDAL_AV_Video_ResetFlow();
    
            bGstreamerUglyHackReset = FALSE;
        }
#endif
    
    
        if (stVedcStatus.bInit == TRUE)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_Rst(-)\n" ));
            bRes = MApi_VDEC_EX_Rst(&stStreamId);
           // mTBOX_ASSERT(bRes == E_VDEC_OK);
            bRes = MApi_VDEC_EX_Exit(&stStreamId);
            //mTBOX_ASSERT(bRes == E_VDEC_OK);
            g_Demo_IsIframedecoding = FALSE;
        }

        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  Determine codec type\n" ));
        switch (StreamType.videoType)
        {
            case eTDAL_AV_VIDEO_TYPE_H264:
                codecType = E_VDEC_EX_CODEC_TYPE_H264;
                break;
            //case eTDAL_AV_VIDEO_TYPE_HEVC:
            //    codecType = E_VDEC_EX_CODEC_TYPE_HEVC;
            //break;
            case eTDAL_AV_VIDEO_TYPE_MPEG2:
            case eTDAL_AV_VIDEO_TYPE_MPEG1:
                codecType = E_VDEC_EX_CODEC_TYPE_MPEG2;
                break;
            case eTDAL_AV_VIDEO_TYPE_VC1:
                codecType = E_VDEC_EX_CODEC_TYPE_VC1_MAIN;
                break;
            case eTDAL_AV_VIDEO_TYPE_DIVX:
                codecType = E_VDEC_EX_CODEC_TYPE_DIVX412;
                break;
            default:
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_Start] Unsupported video codec %d\n", StreamType.videoType));
                codecType = E_VDEC_EX_CODEC_TYPE_NONE;
                break;
        }
        TDAL_AV_VideoDecoderDescriptor.StreamType = StreamType.videoType;
    
        if (MApi_VDEC_EX_GetFreeStream((VDEC_StreamId *)&stStreamId, sizeof(VDEC_StreamId), E_VDEC_EX_MAIN_STREAM, (VDEC_EX_CodecType)codecType) != E_VDEC_EX_OK)
        {
            Error = eTDAL_AV_ERROR;
        }
    
        if (Error == eTDAL_AV_NO_ERROR)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start] MApi_VDEC_GetActiveCodecType() and  MApi_VDEC_GetStatus()\n" ));
            memset(&vdecStatus, 0, sizeof(VDEC_EX_Status));
            codecTypeCur = MApi_VDEC_EX_GetActiveCodecType(&stStreamId);
            bRet = MApi_VDEC_EX_GetStatus(&stStreamId, &vdecStatus);
            if ((bRet == TRUE) && (TRUE == vdecStatus.bInit) && (codecTypeCur == codecType))
            {
                mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start] VDEC already correctly initalized\n" ));
                vdecInitalized = true;
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  Reinitialization required\n" ));
            }
        }  
    
        if (Error == eTDAL_AV_NO_ERROR && vdecInitalized == false)
        {
            memset(&initParam, 0, sizeof(VDEC_EX_InitParam));
            initParam.bDisableDropErrFrame = TRUE;
            initParam.eCodecType = codecType;
            /* TODO START: H264.IFrame */
            //Temp Solution for I-Frame decoder on H264
#if 0
            if(codec == E_DDI_VDEC_CODEC_TYPE_H264_IFRAME)
            {
                initParam.VideoInfo.eSrcMode = E_VDEC_SRC_MODE_FILE;
            }
#endif
            if (TDAL_AV_Playback_bIsFileIn)
            {
                initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
            }
            else
            {
                initParam.VideoInfo.eSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            }
    
            /* TODO END: H264.IFrame */
            //initParam.SysConfig.u32CodeBufAddr = VDEC_AEON_ADR; //MEM_ADR_BY_MIU(VDEC_AEON_ADR, VDEC_AEON_MEMORY_TYPE);
            initParam.SysConfig.u32CodeBufAddr = MEM_ADR_BY_MIU(VDEC_AEON_ADR, VDEC_AEON_MEMORY_TYPE);
            initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
            //initParam.SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR; //MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
            initParam.SysConfig.u32FrameBufAddr = MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE);
            initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
            //initParam.SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR; //MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
            initParam.SysConfig.u32BitstreamBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
            initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
            //initParam.SysConfig.u32DrvProcBufAddr = VDEC_BIT_STREAM_ADR; //MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
            initParam.SysConfig.u32DrvProcBufAddr = MEM_ADR_BY_MIU(VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_MEMORY_TYPE);
            initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;    
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_Init()\n" ));
#if 1
            bRes = MApi_VDEC_EX_Init(&stStreamId, &initParam);
            if (bRes != E_VDEC_EX_OK)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error initializing MApi_VDEC_Init, error = %d", bRes));
                Error = eTDAL_AV_ERROR;
            }
#endif
    
    #if (defined(FAST_ZAPPER_ENABLE) && (FAST_ZAPPER_ENABLE==1))
            MApi_VDEC_SetControl(E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xFF);
            MApi_VDEC_SetControl(E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT, 1);
    #endif
    
        }    
  
        if (Error == eTDAL_AV_NO_ERROR)
        {
    
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_AVSyncOn()\n" ));
            //change sync offset to 180ms, this value will influence lip-sync
            //If you want to change this value, please inform video and audio team at the same time
            bRes = MApi_VDEC_EX_AVSyncOn(&stStreamId, TRUE, 180, 0);
            if (bRes != E_VDEC_EX_OK)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_Start] MApi_VDEC_AVSyncOn fail %d\n", bRes));
                Error = eTDAL_AV_ERROR;
            }
        }    
   
        if (Error == eTDAL_AV_NO_ERROR)
        {
            MDrv_MVOP_Enable(DISABLE);
    
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_SetControl(VDEC_USER_CMD_FD_MASK_DELAY_COUNT)\n" ));
            //Set Field Delay count befor VDEC play
            bRes = MApi_VDEC_EX_SetControl(&stStreamId, E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 16);
            bRes = MApi_VDEC_EX_SetControl(&stStreamId,E_VDEC_EX_USER_CMD_FAST_DISPLAY, ENABLE);
            
            MApi_VDEC_EX_SetControl(&stStreamId,E_VDEC_EX_USER_CMD_SET_ERROR_TOLERANCE, &u32ErrorTolerance);
            MApi_VDEC_EX_SetControl(&stStreamId,E_VDEC_EX_USER_CMD_DROP_ERR_FRAME, TRUE);
            
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_SetControl(VDEC_USER_CMD_FAST_DISPLAY)\n" ));
            
            MApi_VDEC_EX_SetControl(&stStreamId, E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP, TRUE);
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_SetAVSyncFreerunThreshold\n" ));
            bRes = MApi_VDEC_EX_SetAVSyncFreerunThreshold(&stStreamId, 450000);
    
            if (bRes != E_VDEC_EX_OK)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_Start] MApi_VDEC_SetAVSyncFreerunThreshold fail %d\n", bRes));
                Error = eTDAL_AV_ERROR;
            }
    
        }    
    
        if (Error == eTDAL_AV_NO_ERROR)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  Enabling video events\n" ));
            Error = p_TDAL_AV_Video_RegisterEvents();
        }    
    
        if (Error == eTDAL_AV_NO_ERROR)
        {
            if (_bVideo_PVRPlay)
            {
                MApi_VDEC_SetControl(E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH, 0xFF);
            }
            else
            {
                Slow_Sync.u8RepeatPeriod = 5;
                Slow_Sync.u8DropPeriod = 0;
                MApi_VDEC_EX_SetControl(&stStreamId, E_VDEC_EX_USER_CMD_SET_SLOW_SYNC, (MS_U32)&Slow_Sync);
            }
            
            mTBOX_ASSERT(bRes == E_VDEC_EX_OK);
    
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  MApi_VDEC_EX_SetControl(VDEC_EX_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD, TRUE)\n" ));
            bRes = MApi_VDEC_EX_SetControl(&stStreamId, E_VDEC_EX_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD, TRUE);
            mTBOX_ASSERT(bRes == E_VDEC_EX_OK);
    
            // MDrv_MAD_Dvb_setDecCmd(MAD_PLAY);
            //~ MApi_VDEC_SetDbgLevel(E_VDEC_DBG_LEVEL_ERR);
    
            bRes = MApi_VDEC_EX_Play(&stStreamId);
            if (bRes != E_VDEC_EX_OK)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_Start] MApi_VDEC_Play fail %d\n", bRes));
                Error = eTDAL_AV_ERROR;
            }
            
#if 1 //K5
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
            MApi_VDEC_EX_SetControl(&stStreamId, E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 8);
            //MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);  //cause init problem on first run
#endif
    
        }
        
        if (Error == eTDAL_AV_NO_ERROR)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]          MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE)\n" ));
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
            TDAL_AV_Started = TRUE;
            TDAL_AV_Freeze = FALSE;
        }
    
        /* TO BE CHECKED: 
         * If we really need to call NeedDisplayCallback and ResolutionChangeCallback here.
         * These calls have been moved to p_TDAL_AV_Video_UseEvent callback
         * */
    
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_Start]  finished %s\n", Error == eTDAL_AV_NO_ERROR ? "SUCCESS" : "ERROR"));
        mTBOX_RETURN(Error);

}

/**========================================================================**
 * Function   : p_TDAL_AV_Video_Stop
 *
 * Scope   : private to module TDAL_AV
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/

tTDAL_AV_Error   p_TDAL_AV_Video_Stop( tTDAL_AV_Decoder decoder , bool bIsInternal, bool bIsSlaveMode)
{

	DMX_FILTER_STATUS dmxStatus;
	VDEC_EX_Result vdecResult;
	tTDAL_AV_Error error = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("p_TDAL_AV_Video_Stop");

    if (TDAL_AV_Started == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] AV is not running!\n" ));
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);
    }         

	dmxStatus = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
	mTBOX_ASSERT(dmxStatus == DMX_FILTER_STATUS_OK);

	vdecResult = MApi_VDEC_EX_Pause(&stStreamId);
    if (vdecResult != E_VDEC_EX_OK)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_Video_Stop]   MApi_VDEC_Pause returned error %d\n", vdecResult));
        TDAL_AV_Started = FALSE;
        error = eTDAL_AV_ERROR;
    }

    if (error == eTDAL_AV_NO_ERROR)
    {
    	error = p_TDAL_AV_Video_UnregisterEvents();
    }
    
	if (MApi_VDEC_EX_Exit(&stStreamId) != E_VDEC_EX_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_Video_Stop] MApi_VDEC_EX_Exit Not OK\n"));
	}
    
    TDAL_AV_Started = FALSE;
    TDAL_AV_Freeze = FALSE;
    _bGetFirstFrame = FALSE;

    if (!_bVideo_PVRPlay)
    {
#ifdef PACKAGE_PVR
        //TDAL_PVRi_BackGround_Record_Stop();
#endif
    }
	mTBOX_RETURN(error);

}

/**========================================================================*
 * Function   : p_TDAL_AV_Video_Init
 *
 * Scope   : private to module
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error   p_TDAL_AV_Video_Init( void )
{
	mTBOX_FCT_ENTER("p_TDAL_AV_Video_Init");

	TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.UserDataCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.AspectRatioChangeCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.PicturePlayedCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.PictureStoppedCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.ResolutionChangeCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.VideoSampleStoppedCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.PesScrambledBitChangeCallback = NULL;
	TDAL_AV_VideoDecoderDescriptor.FrameRateChangeCallback = NULL;

	/* save default video configuration */
	TDAL_AV_VideoDecoderDescriptor.PictureType = eTDAL_AV_PICTURE_MPEG;
	TDAL_AV_VideoDecoderDescriptor.VideoResolution.height = 576;
	TDAL_AV_VideoDecoderDescriptor.VideoResolution.width = 720;
	TDAL_AV_VideoDecoderDescriptor.AspectRatio = eTDAL_AV_ASPECT_RATIO_MPEG_4_3;
//	   TDAL_AV_VideoDecoderDescriptor.AspectRatioConversion   = eTDAL_AV_DISPLAY_AR_CONVERSION_IGNORE;
	TDAL_AV_VideoDecoderDescriptor.StreamType = eTDAL_AV_VIDEO_TYPE_MPEG2;

	TDAL_AV_VideoDecoderDescriptor.Capability.pictureTypeSupported =
			eTDAL_AV_PICTURE_MPEG | eTDAL_AV_PICTURE_H264;
	TDAL_AV_VideoDecoderDescriptor.Capability.videoTypeSupported =
			eTDAL_AV_VIDEO_TYPE_MPEG2 | eTDAL_AV_VIDEO_TYPE_MPEG1
					| eTDAL_AV_VIDEO_TYPE_H264;
	TDAL_AV_VideoDecoderDescriptor.Capability.isFreezeSupported = TRUE;
	TDAL_AV_VideoDecoderDescriptor.Capability.isPictureCaptureSupported = FALSE;
	TDAL_AV_VideoDecoderDescriptor.Capability.isSpeedSupported = FALSE;

	TDAL_AV_CurrentAFD   = 8;

	VDEC_StreamId lstStreamId = {0}, lstGetStreamId = {0};
	VDEC_EX_User_Cmd eUserCmd = E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;

	eUserCmd =  E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;

	if(MApi_VDEC_EX_GetControl(&lstStreamId,eUserCmd,(MS_U32 *)&lstGetStreamId)!= E_VDEC_EX_OK )
	{
	}
#ifdef TDAL_AV_DBGTASK
        TDAL_AVi_VideoDBG = TDAL_CreateTask(
             eTDAL_PRIORITY_HIGH,
             "VideoDBGTask",
             TDAL_AVi_VideoDBGStack,
             kTDAL_AVi_VIDEO_DBGTASK_STACK_SIZE,
             TDAL_AVi_VideoDBGTaskTaskFct,
             NULL);

        if (TDAL_AVi_VideoDBG == NULL)
        {
             mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Creating audio monitor task fail!\n"));
        }
#endif
	stStreamId = lstGetStreamId;
       TDAL_AV_Started = FALSE;
       TDAL_AV_Freeze = FALSE;
#ifdef PACKAGE_PVR
	PVRPL_Video_RegisterPlayCB((VDEC_CB_PLAY)TDAL_AVi_VideoStartCB);
#endif

	mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : p_TDAL_AV_Video_EventSubscribe
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error   p_TDAL_AV_Video_EventSubscribe(   tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, tTDAL_AV_CallbackProc_t notify_function)
{
   tTDAL_AV_CallbackProc_t *   pClbkStorage;

   mTBOX_FCT_ENTER("TDAL_AV_EventSubscribe");

   mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_EventSubscribe]  EventId = %d\n", event));

	switch (event)
	{
	case eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY:
		pClbkStorage = &TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_USER_DATA:
		pClbkStorage = &TDAL_AV_VideoDecoderDescriptor.UserDataCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE:
		pClbkStorage =
				&TDAL_AV_VideoDecoderDescriptor.AspectRatioChangeCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE:
		pClbkStorage = &TDAL_AV_VideoDecoderDescriptor.ResolutionChangeCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED:
		pClbkStorage = &TDAL_AV_VideoDecoderDescriptor.PicturePlayedCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED:
		pClbkStorage = &TDAL_AV_VideoDecoderDescriptor.PictureStoppedCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE:
		pClbkStorage =
				&TDAL_AV_VideoDecoderDescriptor.PesScrambledBitChangeCallback;
		break;

	case eTDAL_AV_EVENT_VIDEO_FRAME_RATE_CHANGE:
		pClbkStorage = &TDAL_AV_VideoDecoderDescriptor.FrameRateChangeCallback;
		break;

	default:
		/* ASSERTION here */
		mTBOX_ASSERT(FALSE);
		mTBOX_RETURN(eTDAL_AV_LIMITATION_ERROR);
	}

   *pClbkStorage = notify_function;

   mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}


/**========================================================================**
 * Function   : p_TDAL_AV_Video_EventUnsubscribe
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error   p_TDAL_AV_Video_EventUnsubscribe(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event)
{
   mTBOX_FCT_ENTER("TDAL_AV_EventUnsubscribe");

	switch (event)
	{
	case eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY:
		TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_USER_DATA:
		TDAL_AV_VideoDecoderDescriptor.UserDataCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE:
		TDAL_AV_VideoDecoderDescriptor.AspectRatioChangeCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE:
		TDAL_AV_VideoDecoderDescriptor.ResolutionChangeCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED:
		TDAL_AV_VideoDecoderDescriptor.PicturePlayedCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED:
		TDAL_AV_VideoDecoderDescriptor.PictureStoppedCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE:
		TDAL_AV_VideoDecoderDescriptor.PesScrambledBitChangeCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_VIDEO_FRAME_RATE_CHANGE:
		TDAL_AV_VideoDecoderDescriptor.FrameRateChangeCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	default:
		mTBOX_ASSERT(FALSE);
		mTBOX_RETURN(eTDAL_AV_LIMITATION_ERROR);
	}

   mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : p_TDAL_AV_Video_Term
 *
 * Scope   : private to module TDAL_AV
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error   p_TDAL_AV_Video_Term( void )
{
#ifdef    TDAL_AV_DBGTASK
    TDAL_AVi_VideoDBGrStop = TRUE;
#endif
	return eTDAL_AV_NO_ERROR;
}

/**========================================================================**
 * Function   : p_TDAL_AV_Video_UseEvent
 *
 * Scope   : private to module TDAL_AV
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
void p_TDAL_AV_Video_UseEvent(tTDAL_AV_Decoder   decoder, MS_U32 event)
{
        VDEC_EX_Result vdecResult;
        MS_U32 eventFlags[16];
        MS_U32 flagsFound;
        int i;
    
        mTBOX_FCT_ENTER("p_TDAL_AV_Video_UseEvent");
    
        mTBOX_TRACE((kTBOX_NIV_3, "[p_TDAL_AV_Video_UseEvent] Event received %d\n", event));
    
        p_TDAL_AV_ExtractFlagList(event, eventFlags, sizeof(eventFlags), &flagsFound);
    
        for (i = 0; i < flagsFound; ++i)
        {
            switch(eventFlags[i])
            {
                case E_VDEC_EX_EVENT_DISP_ONE:
                    break;
                case E_VDEC_EX_EVENT_DISP_REPEAT:
                    break;
                case E_VDEC_EX_EVENT_DISP_WITH_CC:
                    break;
                case E_VDEC_EX_EVENT_DEC_ONE:
                    break;
                case E_VDEC_EX_EVENT_DEC_I:
                    break;
                case E_VDEC_EX_EVENT_DEC_ERR:
                    break;
                case E_VDEC_EX_EVENT_DISP_INFO_RDY:
                    break;
                case E_VDEC_EX_EVENT_DISP_INFO_CHG:
                {
                   MApi_VDEC_EX_SetBlueScreen(&stStreamId, FALSE);    
                    VDEC_EX_DispInfo vdecDispInfo;
    
                    vdecResult = MApi_VDEC_EX_GetDispInfo(&stStreamId, &vdecDispInfo);
                    if (vdecResult == E_VDEC_EX_OK)
                    {
                        if (TDAL_AV_VideoDecoderDescriptor.AspectRatioChangeCallback != NULL)
                        {
                            if (vdecDispInfo.u8AspectRate != TDAL_AVi_VideoDispInfo.u8AspectRate)
                            {
                                tTDAL_AV_VideoAspectRatio tdalAspectRatio = -1;
                                switch (vdecDispInfo.u8AspectRate)
                                {
                                    case MVD_ASP_4TO3: tdalAspectRatio = eTDAL_AV_ASPECT_RATIO_MPEG_4_3; break;
                                    case MVD_ASP_221TO100: tdalAspectRatio = eTDAL_AV_ASPECT_RATIO_MPEG_221_1; break;
                                    default:
                                        tdalAspectRatio = eTDAL_AV_ASPECT_RATIO_MPEG_16_9;
                                        //mTBOX_ASSERT(FALSE);
                                        break;
                                }
    
                                if (tdalAspectRatio != -1)
                                {
                                    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_UseEvent] AspectRatioChangeCallback \n"));
                                #if 1
                                    TDAL_AV_VideoDecoderDescriptor.AspectRatioChangeCallback(decoder, eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE, &tdalAspectRatio);
                                #endif
                                }
                            }
                        }
 
                        if (TDAL_AV_VideoDecoderDescriptor.FrameRateChangeCallback != NULL)
                        {
                            if (vdecDispInfo.u32FrameRate != TDAL_AVi_VideoDispInfo.u32FrameRate)
                            {
                                tTDAL_AV_VideoFrameRate tdalFrameRate = (tTDAL_AV_VideoFrameRate) vdecDispInfo.u32FrameRate;
    
                                mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_UseEvent]   FrameRateChangeCallback, new frame rate %d\n", tdalFrameRate));
                            #if 1
                                TDAL_AV_VideoDecoderDescriptor.FrameRateChangeCallback(decoder, eTDAL_AV_EVENT_VIDEO_FRAME_RATE_CHANGE, &tdalFrameRate);
                            #endif
                            }
                        }
 
                        if (TDAL_AV_VideoDecoderDescriptor.ResolutionChangeCallback != NULL)
                        {
                            if ((vdecDispInfo.u16HorSize != TDAL_AVi_VideoDispInfo.u16HorSize) || (vdecDispInfo.u16VerSize != TDAL_AVi_VideoDispInfo.u16VerSize))
                            {
                                tTDAL_AV_VideoResolution tdalVideoResolution;
                                tdalVideoResolution.height = (uint16_t) vdecDispInfo.u16VerSize;
                                tdalVideoResolution.width = (uint16_t) vdecDispInfo.u16HorSize;
    
                                mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Video_UseEvent]    ResolutionChangeCallback, new resolution (%dx%d)\n",
                                        tdalVideoResolution.width, tdalVideoResolution.height));
                            #if 1
                                bResolutionAlreadyGet = TRUE;
                                TDAL_AV_VideoDecoderDescriptor.ResolutionChangeCallback(decoder, eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE, &tdalVideoResolution);
                            #endif
                            }
                        }
    
                        TDAL_AVi_VideoDispInfo = vdecDispInfo;
                    }
                    else
                    {
                        mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_Video_UseEvent]   MApi_VDEC_GetDispInfo returned %d\n", p_TDAL_AV_Video_UseEvent));
                    }

                    TDAL_LockMutex(TDAL_AVi_Mutex);
                    if (TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback != NULL && TDAL_AV_Started && _bGetFirstFrame)
                    {
                        TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback(kTDAL_AVi_VIDEO_DECODER, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY, NULL);
                    }
                    else
                    {
                        mTBOX_TRACE((kTBOX_NIV_1, "Skipping event NEED DISPLAY, TDAL_AV_Started = %d, NeedDisplayCallback = %p, _bGetFirstFrame=%d\n",
                                TDAL_AV_Started,
                                TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback,
                                _bGetFirstFrame
                        ));
                    }
                    if (!_bVideo_PVRPlay)
                    {
                    #ifdef PACKAGE_PVR
                        //TDAL_PVRi_BackGround_Record_Start();
                    #endif
                    }
                    TDAL_UnlockMutex(TDAL_AVi_Mutex);

                }
                    break;
                case E_VDEC_EX_EVENT_USER_DATA_FOUND:
                    if (TDAL_AV_VideoDecoderDescriptor.UserDataCallback != NULL)
                    {
                        VDEC_EX_CC_Info ccInfo;
                        memset(&ccInfo,0, sizeof(ccInfo));
                        ccInfo.u32Version = 0;
                        if (MApi_VDEC_EX_IsCCAvailable(&stStreamId))
                        {
                            if (E_VDEC_EX_OK == MApi_VDEC_EX_GetCCInfo(&stStreamId, (void*)&ccInfo, sizeof(VDEC_EX_CC_Info)))
                            {
                                p_TDAL_AV_Analyse_UserData(decoder, (uint8_t *) ccInfo.u32UserDataBuf, ccInfo.u32UserDataSize);
                            }
                            else
                            {
                                mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_Video_UseEvent]  MApi_VDEC_GetCCInfo returned error\n"));
                            }
                        }
                        else
                        {
                            mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_Video_UseEvent]  No userdata available\n"));
                        }
                    }
                    break;
                /*case E_VDEC_EX_EVENT_DISP_INFO_RDY:
                {
                    TDAL_LockMutex(TDAL_AVi_Mutex);
                    if (TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback != NULL && TDAL_AV_Started)
                    {
                        TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback(kTDAL_AVi_VIDEO_DECODER, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY, NULL);
                    }
                    else
                    {
                        mTBOX_TRACE((kTBOX_NIV_1, "Skipping event NEED DISPLAY, TDAL_AV_Started = %d, NeedDisplayCallback = %p\n",
                                TDAL_AV_Started,
                                TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback
                        ));
                    }
    
                    TDAL_UnlockMutex(TDAL_AVi_Mutex);
                }
                    break;*/
                case E_VDEC_EX_EVENT_FIRST_FRAME:
                    TDAL_LockMutex(TDAL_AVi_Mutex);
                    if (TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback != NULL && TDAL_AV_Started && !_bGetFirstFrame)
                        {
                            if (TDAL_AVm_GetDecoderStatus(NULL) == false)
                            {
                                mTBOX_TRACE((kTBOX_NIV_1, "Decoder not running\n"));
                                mTBOX_RETURN(eTDAL_AV_ERROR);
                            }
                       _bGetFirstFrame = TRUE;
                           TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback(kTDAL_AVi_VIDEO_DECODER, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY, NULL);
                        }
                        else
                        {
                           mTBOX_TRACE((kTBOX_NIV_1, "Skipping event NEED DISPLAY, TDAL_AV_Started = %d, NeedDisplayCallback = %p\n",
                                   TDAL_AV_Started,
                                   TDAL_AV_VideoDecoderDescriptor.NeedDisplayCallback
                           ));
                        }
                    TDAL_UnlockMutex(TDAL_AVi_Mutex);
                    break;
                case E_VDEC_EX_EVENT_PIC_FOUND:
                    break;
                case E_VDEC_EX_EVENT_VIDEO_UNMUTE:
                    break;
                case E_VDEC_EX_EVENT_SEQ_HDR_FOUND:
                    break;
                case E_VDEC_EX_EVENT_AFD_FOUND:
                    break;
                default:
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_Video_UseEvent]   Unknown event %d\n", eventFlags[i]));
            }
        }
    
        mTBOX_RETURN;

}

void p_TDAL_AV_VideoEventCallback(MS_U32 eFlag, void *param)
{
	tTDAL_AV_EvtMsg msg;
	MS_BOOL bRet;

	mTBOX_FCT_ENTER("p_TDAL_AV_VideoEventCallback");

	msg.Event = eFlag;
	msg.Exit = FALSE;
	msg.decoder = kTDAL_AVi_VIDEO_DECODER;

	mTBOX_TRACE((kTBOX_NIV_3, "[p_TDAL_AV_VideoEventCallback] Video event %d received\n", eFlag));

	bRet = MsOS_SendToQueue(TDAL_AV_EvtQueue_p, (MS_U8 *) &msg, sizeof(msg), 0);
	if (bRet != TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "[p_TDAL_AV_VideoEventCallback]  Could not send event %d to queue, queue full\n"));
	}

	mTBOX_RETURN;
}

tTDAL_AV_Error p_TDAL_AV_Video_RegisterEvents(void)
{
	tTDAL_AV_Error error = eTDAL_AV_NO_ERROR;
	VDEC_EX_EventCb eventCB = (VDEC_EX_EventCb) p_TDAL_AV_VideoEventCallback;
	VDEC_EX_Result bRes;

	mTBOX_FCT_ENTER("p_TDAL_AV_Video_RegisterEvents");

	bRes = MApi_VDEC_EX_SetEvent(&stStreamId, TDAL_AVi_VIDEO_EVENTS, eventCB, NULL);
	if (bRes != TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_RegisterEvents]   MApi_VDEC_SetEvent_EX    returned %d\n", bRes));
		error = eTDAL_AV_ERROR;
	}

	mTBOX_RETURN(error);

}

tTDAL_AV_Error p_TDAL_AV_Video_UnregisterEvents(void)
{
	tTDAL_AV_Error error = eTDAL_AV_NO_ERROR;
	VDEC_EX_Result bRes;

	mTBOX_FCT_ENTER("p_TDAL_AV_Video_RegisterEvents");

	bRes = MApi_VDEC_EX_UnsetEvent(&stStreamId, TDAL_AVi_VIDEO_EVENTS);
	if (bRes != TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_RegisterEvents]   MApi_VDEC_UnsetEvent_EX    returned %d\n", bRes));
		error = eTDAL_AV_ERROR;
	}

	mTBOX_RETURN(error);

}


#define   kp_TDAL_AV_USERDATA_AFD_ID_OFFSET   0
#define kp_TDAL_AV_USERDATA_AF_FLAG_ID_OFFSET   4
#define kp_TDAL_AV_USERDATA_AFD_ID   0x44544731
#define kp_TDAL_AV_USERDATA_ACTIVE_FORMAT_OFFSET   5

/***************************************************************
Name   : p_TDAL_AV_Analyse_UserData
Description :
Parameters  :
Assumptions :
Limitations :
Returns   :
****************************************************************/
LOCAL void p_TDAL_AV_Analyse_UserData(tTDAL_AV_Decoder decoder, uint8_t *UserData_Buffer, MS_U32 bufferSize)
{
   uint32_t   temp;
   uint32_t   AFD_ID;
   uint8_t   Active_format;

   mTBOX_FCT_ENTER("p_TDAL_AV_Analyse_UserData");

   if (kp_TDAL_AV_USERDATA_ACTIVE_FORMAT_OFFSET >= bufferSize)
   {
	   mTBOX_TRACE((kTBOX_NIV_WARNING, "No AFD data present in user data\n"));
	   mTBOX_RETURN;
   }

   mTBOX_ASSERT(kp_TDAL_AV_USERDATA_AFD_ID_OFFSET + 3 < bufferSize);
   temp   = (uint32_t) UserData_Buffer[kp_TDAL_AV_USERDATA_AFD_ID_OFFSET];
   AFD_ID   = (uint32_t) (temp << 24);
   temp   = (uint32_t) UserData_Buffer[kp_TDAL_AV_USERDATA_AFD_ID_OFFSET + 1];
   AFD_ID   = AFD_ID + (uint32_t) (temp << 16);
   temp   = (uint32_t) UserData_Buffer[kp_TDAL_AV_USERDATA_AFD_ID_OFFSET + 2];
   AFD_ID   = AFD_ID + (uint32_t) (temp << 8);
   temp   = (uint32_t) UserData_Buffer[kp_TDAL_AV_USERDATA_AFD_ID_OFFSET + 3];
   AFD_ID   = AFD_ID + (uint32_t) (temp);

   if (AFD_ID == kp_TDAL_AV_USERDATA_AFD_ID)
   {
	   if ((UserData_Buffer[kp_TDAL_AV_USERDATA_AF_FLAG_ID_OFFSET] & 0xC0) != 0x40)
	   {
		   Active_format = 0x08;
	   }
	   else
	   {
	/*   TBOX_Dump(UserData_Buffer,16);*/
		   Active_format = UserData_Buffer[kp_TDAL_AV_USERDATA_ACTIVE_FORMAT_OFFSET] & 0x0F;
	   }

	   if (Active_format != TDAL_AV_CurrentAFD)
	   {
		   tTDAL_AV_VideoUserData userData;

		   TDAL_AV_CurrentAFD = Active_format;
		   userData.AFD = TDAL_AV_CurrentAFD;

		   if(TDAL_AV_VideoDecoderDescriptor.UserDataCallback != NULL)
		   {
			   mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Analyse_UserData] generating eTDAL_AV_EVENT_VIDEO_USER_DATA event, AFF = %d\n", TDAL_AV_CurrentAFD));
			   TDAL_AV_VideoDecoderDescriptor.UserDataCallback(decoder, eTDAL_AV_EVENT_VIDEO_USER_DATA, (void*)&userData);
		   }
	   }
   }

   mTBOX_RETURN;
} /* End of p_TDAL_AV_Analyse_UserData */

LOCAL void p_TDAL_AV_ExtractFlagList(MS_U32 events, MS_U32 eventArray[], MS_U32 eventArrayLength, MS_U32 * flagsFound)
{
	MS_U32 i, flag, oldFlag;

	mTBOX_FCT_ENTER("p_TDAL_AV_ExtractFlagList");

	mTBOX_ASSERT(flagsFound != NULL);

	*flagsFound = 0;

	oldFlag = 0;

	for (i = 0; i < eventArrayLength; i++)
	{
		flag = 1l << i;
		if (flag < oldFlag)
		{
			break;
		}

		if ((flag & events) != 0)
		{
			eventArray[*flagsFound] = flag;
			(*flagsFound)++;
			mTBOX_TRACE((kTBOX_NIV_3, "[p_TDAL_AV_ExtractFlagList]  flag %d(%d) found in events %x(%d) \n", flag, i, events, events));
		}
		oldFlag = flag;
	}

	mTBOX_TRACE((kTBOX_NIV_3, "[p_TDAL_AV_ExtractFlagList] Number of flags found %d\n", *flagsFound));

	mTBOX_RETURN;
}

#define VIDEO_FRAMECOUNT_LOOP           300


LOCAL MS_BOOL TDAL_AVi_Video_GetVidInfo(VDEC_EX_DispInfo *pstVidStatus)
{
    VDEC_EX_DispInfo   info;
    VDEC_EX_Result     ret;

    mTBOX_FCT_ENTER("TDAL_AVi_Video_GetVidInfo");


#ifdef ENABLE_MM_PACKAGE
//    if((is_TDAL_MP_Initialized() == TRUE) )//&& (msAPI_MM_GetFileType() != MM_ES_MODE)
//    {
//        video_disp_info MM_DispInfo;
//        memset(&MM_DispInfo, 0x00, sizeof(video_disp_info));
//
//        //if(msAPI_MM_GetDispInfo(&MM_DispInfo) == TRUE)
//        if(MApi_MPlayer_Get_DispInfo(E_MPLAYER_MEDIA_MOVIE, (video_disp_info*)&MM_DispInfo) == TRUE)
//        {
//            pstVidStatus->u16CropLeft = MM_DispInfo.dCropLeft;
//            pstVidStatus->u16CropRight = MM_DispInfo.dCropRight;
//            pstVidStatus->u16CropTop = MM_DispInfo.dCropTop;
//            pstVidStatus->u16CropBottom = MM_DispInfo.dCropBottom;
//            pstVidStatus->u16SarWidth   = 0;
//            pstVidStatus->u16SarHeight = 0;
//            pstVidStatus->u16HorSize    = MM_DispInfo.dWidth;
//            pstVidStatus->u16VerSize    = MM_DispInfo.dHeight;
//            pstVidStatus->u32FrameRate  = MM_DispInfo.dFrameRate;
//            pstVidStatus->u8Interlace   = MM_DispInfo.dInterlace;
//        }
//        else
//            mTBOX_RETURN(FALSE);
//    }
//    else
#endif
    {
#if 0	//cause play problem
    if (MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2)
    {
        if (E_VDEC_EX_FAIL == MApi_VDEC_EX_IsWithValidStream(&stStreamId)) /*--- check valid mpeg ---*/
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Stream is not Valid!! \n"));
            mTBOX_RETURN(FALSE);
        }
    }
#endif

    memset(&info, 0, sizeof(VDEC_EX_DispInfo));
    ret = MApi_VDEC_EX_GetDispInfo(&stStreamId, &info);

    if (E_VDEC_EX_OK != ret)
    {
        mTBOX_RETURN(FALSE);
    }
    else
    {
        pstVidStatus->u16HorSize    = info.u16HorSize;
        pstVidStatus->u16VerSize    = info.u16VerSize;
        pstVidStatus->u32FrameRate  = info.u32FrameRate;
        pstVidStatus->u8AspectRate  = info.u8AspectRate;
        pstVidStatus->u8Interlace   = info.u8Interlace;
        pstVidStatus->u16SarWidth   = info.u16SarWidth;
        pstVidStatus->u16SarHeight  = info.u16SarHeight;
        pstVidStatus->u16CropRight  = info.u16CropRight;
        pstVidStatus->u16CropLeft   = info.u16CropLeft;
        pstVidStatus->u16CropBottom = info.u16CropBottom;
        pstVidStatus->u16CropTop    = info.u16CropTop;
        pstVidStatus->u32AspectWidth = info.u32AspectWidth;
        pstVidStatus->u32AspectHeight    = info.u32AspectHeight;

        mTBOX_TRACE((kTBOX_NIV_1, "HorSize = %u \n", pstVidStatus->u16HorSize));
        mTBOX_TRACE((kTBOX_NIV_1, "VerSize = %u \n", pstVidStatus->u16VerSize));
        mTBOX_TRACE((kTBOX_NIV_1, "FrameRate = %lu \n", pstVidStatus->u32FrameRate));
        mTBOX_TRACE((kTBOX_NIV_1, "AspectRate = %u \n", pstVidStatus->u8AspectRate));
        mTBOX_TRACE((kTBOX_NIV_1, "Interlace = %u \n", pstVidStatus->u8Interlace));
        mTBOX_TRACE((kTBOX_NIV_1, "SarWidth = %u \n", pstVidStatus->u16SarWidth));
        mTBOX_TRACE((kTBOX_NIV_1, "SarHeight = %u \n", pstVidStatus->u16SarHeight));
        mTBOX_TRACE((kTBOX_NIV_1, "AspectWidth = %u \n", pstVidStatus->u32AspectWidth));
        mTBOX_TRACE((kTBOX_NIV_1, "AspectHeight = %u \n", pstVidStatus->u32AspectHeight));

        if (MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2)
        {
            pstVidStatus->u8AFD         = info.u8AFD;
            pstVidStatus->u16Pitch      = info.u16Pitch;
            pstVidStatus->u16PTSInterval = info.u16PTSInterval;
            pstVidStatus->u8MPEG1       = info.u8MPEG1;
            pstVidStatus->u8PlayMode    = info.u8PlayMode;
            pstVidStatus->u8FrcMode     = info.u8FrcMode;

            if (pstVidStatus->u32FrameRate == 0||
                pstVidStatus->u16HorSize == 0 ||
                pstVidStatus->u16HorSize > 2000 ||
                pstVidStatus->u16VerSize == 0 ||
                pstVidStatus->u16VerSize > 1100)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "Video Info is not Valid!! \n"));
                mTBOX_RETURN(FALSE);
            }
            pstVidStatus->u8AFD = MApi_VDEC_EX_GetActiveFormat(&stStreamId);
            mTBOX_RETURN(TRUE);
        }
    }
    }
    mTBOX_RETURN(TRUE);
}



#if 0
LOCAL MVOP_InputSel TDAL_AVi_Video_GetMVOPInputSel(void)
{
//    MVOP_InputSel tMvopInputSel;

    mTBOX_FCT_ENTER("TDAL_AVi_Video_GetMVOPInputSel");
#if 0
    if(g_Demo_IsIframedecoding == TRUE)
    {
        tMvopInputSel = MVOP_INPUT_DRAM;
    }
    else if ((MApi_VDEC_GetActiveCodecType() == E_VDEC_CODEC_TYPE_MPEG2) ||
        (MApi_VDEC_GetActiveCodecType() == E_VDEC_CODEC_TYPE_VC1_ADV) ||
        (MApi_VDEC_GetActiveCodecType() == E_VDEC_CODEC_TYPE_MPEG4))
    {
        tMvopInputSel = MVOP_INPUT_MVD;
        mTBOX_TRACE((kTBOX_NIV_1, "MVOP_INPUT_MVD \n"));
    }
    else if(MApi_VDEC_GetActiveCodecType() == E_VDEC_CODEC_TYPE_H264)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        mTBOX_TRACE((kTBOX_NIV_1, "E_VDEC_CODEC_TYPE_H264 \n"));
    }
    else if(MApi_VDEC_GetActiveCodecType() == E_VDEC_CODEC_TYPE_AVS)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        mTBOX_TRACE((kTBOX_NIV_1, "E_VDEC_CODEC_TYPE_AVS \n"));
    }
#endif
    mTBOX_RETURN(0);
    //mTBOX_RETURN(tMvopInputSel);
}
#endif

LOCAL MVOP_InputSel TDAL_AVi_Video_GetMVOPInputSel(void)
{
    MVOP_InputSel tMvopInputSel;

    mTBOX_FCT_ENTER("TDAL_AVi_Video_GetMVOPInputSel");

    if(g_Demo_IsIframedecoding == TRUE)
    {
        tMvopInputSel = MVOP_INPUT_DRAM;
    }
    else if ((MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2) ||
        (MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_VC1_ADV) ||
        (MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG4))
    {
        tMvopInputSel = MVOP_INPUT_MVD;
        mTBOX_TRACE((kTBOX_NIV_1, "MVOP_INPUT_MVD \n"));
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_H264)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        mTBOX_TRACE((kTBOX_NIV_1, "E_VDEC_CODEC_TYPE_H264 \n"));
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_H264)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        mTBOX_TRACE((kTBOX_NIV_1, "E_VDEC_CODEC_TYPE_H264 \n"));
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_HEVC)
    {
        tMvopInputSel = MVOP_INPUT_EVD;
        mTBOX_TRACE((kTBOX_NIV_1, "E_VDEC_EX_CODEC_TYPE_HEVC \n"));
    }
    else if(MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_AVS)
    {
        tMvopInputSel = MVOP_INPUT_H264;
        mTBOX_TRACE((kTBOX_NIV_1, "E_VDEC_CODEC_TYPE_AVS \n"));
    }

    mTBOX_RETURN(tMvopInputSel);
}

LOCAL MS_U32 TDAL_AVi_Video_TimeDiffFromNow(MS_U32 u32Time)
{
    MS_U32 u32Now;

    mTBOX_FCT_ENTER("TDAL_AVi_Video_TimeDiffFromNow");

	u32Now = MsOS_GetSystemTime();
    if(u32Now >= u32Time)
    {
        mTBOX_RETURN(u32Now - u32Time);
    }
    else
    {
        mTBOX_RETURN(0xFFFFFFFF - u32Time + u32Now);
    }
}

LOCAL tTDAL_AVi_AvSyncStatus TDAL_AVi_Video_IsSyncDone()
{
    VDEC_EX_Result enRst = E_VDEC_EX_FAIL;

    mTBOX_FCT_ENTER("TDAL_AVi_Video_IsSyncDone");

    if (!MApi_VDEC_EX_IsAVSyncOn(&stStreamId))
    {
        mTBOX_TRACE((kTBOX_NIV_1, "MApi_VDEC_IsAVSyncOn Not ON\n"));
        mTBOX_RETURN(eTDAL_AVi_AvSync_Not_Done);
    }
#if 0 //vladimir.davydov: this check is disabled because it always returns true on K5
    if (MApi_VDEC_EX_IsFreerun(&stStreamId))
    {
        mTBOX_TRACE((kTBOX_NIV_1, "MApi_VDEC_IsFreerun() IS ON\n"));
        mTBOX_RETURN(eTDAL_AVi_AvSync_Freerun);
    }
#endif

    enRst = MApi_VDEC_EX_IsReachSync(&stStreamId);
    if(enRst == E_VDEC_EX_OK)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "MApi_VDEC_IsReachSync OK!\n"));
        mTBOX_RETURN(eTDAL_AVi_AvSync_Done);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1, "MApi_VDEC_IsReachSync Not OK, return %u\n", enRst));
        //MApi_VDEC_SetDbgLevel(E_VDEC_DBG_LEVEL_DBG);
        //MApi_VDEC_DbgDumpStatus();
        mTBOX_RETURN(eTDAL_AVi_AvSync_Not_Done);
    }
}

bool TDAL_AVm_GetDecoderStatus(VDEC_EX_DispInfo * pVidStatus)
{
    VDEC_EX_DispInfo stVidStatus;
    MSAPI_XC_VDEC_DispInfo stMSAPIDispInfo;
    MS_U8 u8MvopMiuSel = 0;
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    static MS_U32 u32SyncStarTime, u32SyncTimeDiff;
    MS_U32 u32Loop = VIDEO_FRAMECOUNT_LOOP;
    MS_BOOL bSyncFlag = FALSE;
    MVOP_InputSel tMvopInputSel;
    bool decoderRunning = FALSE;

    mTBOX_FCT_ENTER("TDAL_AVi_GetDecoderStatus");

    if (TDAL_AV_Started == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] AV is not running!\n" ));
        return decoderRunning;            
    }
    /////////////////////
    //check frame count//
    /////////////////////
    u32StartTime = MsOS_GetSystemTime();

#if 1	//cause init problem
    u32Loop = VIDEO_FRAMECOUNT_LOOP;

    //check frame ready
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;

    
    MS_S32 ddata;
    VDEC_EX_Status stVedcStatus = {0};
    MS_U32 upstc0=0, dwnstc0=0;
    MS_U32 upspcr0=0, dwnpcr0=0;
    MS_U32 video_fifo0 = 0;
    MS_U32 audio_fifo0 = 0;    

    do
    {
        u32Loop--;
        if(u32Loop==0)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "get frame count timeout!! \n"));
            //mTBOX_RETURN(false);
            break;
        }

        MsOS_DelayTask(10);
        ret = MApi_VDEC_EX_CheckDispInfoRdy(&stStreamId);

    }while(ret != E_VDEC_EX_OK);
    //MApi_XC_WaitInputVSync(2, 200, MAIN_WINDOW);
    
    MApi_VDEC_EX_SetBlueScreen(&stStreamId, FALSE);

    u32EndTime = MsOS_GetSystemTime();
    mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] DIFF TIME to Disp Rdy = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime));
#endif

    u32StartTime = MsOS_GetSystemTime();
    /////////////////////////
    //get video information//
    /////////////////////////
    if(TRUE != TDAL_AVi_Video_GetVidInfo(&stVidStatus))
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, " Video Get Info Failed!! \n"));
        mTBOX_RETURN(false);
    }
    else
    {
    	if (pVidStatus != NULL)
    	{
    		memcpy(pVidStatus, &stVidStatus, sizeof(stVidStatus));
    	}
    }

    /////////////////////////////////////
    //Set VDEC information and Set MVOP//
    ////////////////////////////////////
    tMvopInputSel = TDAL_AVi_Video_GetMVOPInputSel();
    
    stMSAPIDispInfo.u16HorSize          = stVidStatus.u16HorSize;
    stMSAPIDispInfo.u16VerSize          = stVidStatus.u16VerSize;
    stMSAPIDispInfo.u32FrameRate        = stVidStatus.u32FrameRate;
    stMSAPIDispInfo.u8Interlace         = stVidStatus.u8Interlace;
    stMSAPIDispInfo.u8AFD               = stVidStatus.u8AFD;
    stMSAPIDispInfo.u16SarWidth         = stVidStatus.u16SarWidth;
    stMSAPIDispInfo.u16SarHeight        = stVidStatus.u16SarHeight;
    stMSAPIDispInfo.u16CropRight        = stVidStatus.u16CropRight;
    stMSAPIDispInfo.u16CropLeft         = stVidStatus.u16CropLeft;
    stMSAPIDispInfo.u16CropBottom       = stVidStatus.u16CropBottom;
    stMSAPIDispInfo.u16CropTop          = stVidStatus.u16CropTop;
    stMSAPIDispInfo.u16Pitch            = stVidStatus.u16Pitch;
    stMSAPIDispInfo.u16PTSInterval      = stVidStatus.u16PTSInterval;
    stMSAPIDispInfo.u8MPEG1             = stVidStatus.u8MPEG1;
    stMSAPIDispInfo.u8PlayMode          = stVidStatus.u8PlayMode;
    stMSAPIDispInfo.u8FrcMode           = stVidStatus.u8FrcMode;
    stMSAPIDispInfo.u8AspectRate        = stVidStatus.u8AspectRate;
    stMSAPIDispInfo.bWithChroma         = stVidStatus.bWithChroma;
    stMSAPIDispInfo.bColorInXVYCC       = stVidStatus.bColorInXVYCC;
    stMSAPIDispInfo.u32DynScalingAddr   = stVidStatus.u32DynScalingAddr;
    stMSAPIDispInfo.u32DynScalingSize   = stVidStatus.u32DynScalingSize;
    stMSAPIDispInfo.u8DynScalingDepth   = stVidStatus.u8DynScalingDepth;
    stMSAPIDispInfo.bEnableMIUSel       = stVidStatus.bEnableMIUSel;
    stMSAPIDispInfo.u32AspectWidth      = stVidStatus.u32AspectWidth;
    stMSAPIDispInfo.u32AspectHeight     = stVidStatus.u32AspectHeight;

    if (msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stMSAPIDispInfo) != E_MSAPI_XC_OK)
    {
        return FALSE;
	}

    u8MvopMiuSel = VDEC_FRAME_BUF_MEMORY_TYPE & BIT(0);

    if (msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, u8MvopMiuSel) != TRUE)
    {
        return FALSE;
    }
    
    
    if(TRUE != msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, tMvopInputSel))
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Video Set Output Config Failed!! \n"));
        mTBOX_RETURN(false);
    }

    u32EndTime = MsOS_GetSystemTime();
    mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] DIFF TIME to Set Mvop = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime));
   ////////////////////////////////////////////////////////////
    //Check if information for display setting is ready or not//
    //And disable VDEC blue screen                            //
    ////////////////////////////////////////////////////////////
#if 1
    u32StartTime = MsOS_GetSystemTime();

    do
    {
        u32Loop--;
        if(u32Loop==0)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "get frame rdy timeout!! \n"));
            //mTBOX_RETURN(false);
            break;
        }

        MsOS_DelayTask(10);
        ret = MApi_VDEC_EX_IsFrameRdy(&stStreamId);
    }while(ret != E_VDEC_EX_OK);

    u32EndTime = MsOS_GetSystemTime();
    mTBOX_TRACE((kTBOX_NIV_WARNING, "[%s][%d] DIFF TIME to check frame rdy = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime));
#endif
    if (_bVideo_PVRPlay)
    {
        //////////////////////
        //check AV sync done//
        //////////////////////
        u32SyncStarTime = MsOS_GetSystemTime();
        decoderRunning = FALSE;

        int async_timeout;
        if(MApi_VDEC_EX_GetActiveCodecType(&stStreamId) == E_VDEC_EX_CODEC_TYPE_H264)
        {
            async_timeout = AVSYNC_TIMEOUT_INTERVAL_H264;
        } else {
            async_timeout = AVSYNC_TIMEOUT_INTERVAL_OTHER;
        }

        tTDAL_AVi_AvSyncStatus avSyncStatus;

        while(bSyncFlag != TRUE)
        {
            u32SyncTimeDiff = TDAL_AVi_Video_TimeDiffFromNow(u32SyncStarTime);
            mTBOX_TRACE((kTBOX_NIV_3, "SyncTimeDiff = %lu \n", u32SyncTimeDiff));
            avSyncStatus = TDAL_AVi_Video_IsSyncDone();
            if(avSyncStatus == eTDAL_AVi_AvSync_Done)
            {
               mTBOX_TRACE((kTBOX_NIV_1, "*****************Sync Done %ld ms\n", u32SyncTimeDiff));
               bSyncFlag = TRUE;
               decoderRunning = TRUE;
            }
            else if (u32SyncTimeDiff > async_timeout) // set timeout
            {
                bSyncFlag = TRUE;
                if (avSyncStatus == eTDAL_AVi_AvSync_Freerun)
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "*****************Sync In Freerun %ld ms\n", u32SyncTimeDiff));
                    decoderRunning = TRUE;
                }
                else
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "*****************Sync Timeout %ld ms\n", u32SyncTimeDiff));
                    decoderRunning = TRUE;
                }
            }
            MsOS_DelayTask(10);
        }
    
        u32EndTime = MsOS_GetSystemTime();
        mTBOX_TRACE((kTBOX_NIV_1, "SUCESS [%s][%d] DIFF TIME to wait AV-Sync = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32SyncStarTime));
    }
    else
    {
        decoderRunning = TRUE;
    }

    return decoderRunning;
}

bool TDAL_AV_VideoStarted(void)
{
    return TDAL_AV_Started;
}

bool TDAL_AV_VideoDecoding(void)
{
    return _bGetFirstFrame;
}
tTDAL_AV_VideoType TDAL_AV_VideoGetStreamType(void)
{
    return TDAL_AV_VideoDecoderDescriptor.StreamType;
}
/********************************************************
   *   Demo Functions   Definitions   (LOCAL/CALLBACK)        *
********************************************************/
LOCAL VDEC_StreamId* TDAL_AVi_VideoStartCB(MS_U32 VCodec, MS_BOOL bStart)
{
#ifdef PACKAGE_PVR
    if (bStart)
    {
        tTDAL_AV_StreamType StreamType;
        
        _bVideo_PVRPlay = TRUE;
        if (TDAL_PVRi_PlaybackSwitchChunk() || TDAL_PVRi_SeamlessPlayback())
        {
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, FALSE);
        }
        else
        {
            switch (VCodec)
            {
                case E_VDEC_EX_CODEC_TYPE_H264:
                    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_H264;
                    break;
                case E_VDEC_EX_CODEC_TYPE_MPEG2:
                    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
                    break;
                case E_VDEC_EX_CODEC_TYPE_VC1_MAIN:
                    StreamType.videoType =  eTDAL_AV_VIDEO_TYPE_VC1;
                    break;
                case E_VDEC_EX_CODEC_TYPE_DIVX412:
                    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_DIVX;
                    break;
                default:
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Video_Start] Unsupported video codec %d\n", StreamType.videoType));
                    StreamType.videoType = 0x000000000;
                    break;
            }
            p_TDAL_AV_Video_Start(eTDAL_AV_DECODER_VIDEO_1, StreamType, FALSE, FALSE);
        }
    }
    else
    {
        if (TDAL_PVRi_PlaybackSwitchChunk() || TDAL_PVRi_SeamlessPlayback())
        {
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
            MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);          
        }
        else
        {
            p_TDAL_AV_Video_Stop(eTDAL_AV_DECODER_VIDEO_1, FALSE, FALSE);
        }
        _bVideo_PVRPlay = FALSE;
    }
#endif
    return &stStreamId;
}
