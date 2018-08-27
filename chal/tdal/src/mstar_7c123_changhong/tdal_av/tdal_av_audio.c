/*****************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_AV
 *
 * FILE NAME: URL:  $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_av/tdal_av_audio.c $
 *   Rev:  $Rev: 673 $
 *   Date: $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * Description : AudioVideo TDAL library implementation
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES   *
 ****************************************************************************/
/* Generic Headers */
#include <stdio.h>
#include "string.h" /* needed for memcpy() */

#include "MsCommon.h"

#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "tdal_av.h"
#include "tdal_av_p.h"
#include "MsMemory.h"
#include "apiAUDIO.h"
#include "apiDMX.h"
#include "apiHDMITx.h"

#if defined(USE_TDAL_MP)
#include "tdal_mp.h"
#endif

/****************************************************************************
 *  DEFINES   *
 ****************************************************************************/

#define kTDAL_AVi_AUDIO_DECODER eTDAL_AV_DECODER_AUDIO_1
#define kTDAL_AVi_AUDIO_SAMPLE_TASK_STACK_SIZE (2*4096)
#define kTDAL_AVi_AUDIO_MONITOR_TASK_STACK_SIZE (2*4096)

/****************************************************************************
 *  MACROS   *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_AV);


/****************************************************************************
 *  TYPEDEFS   *
 ****************************************************************************/

typedef struct
{
   //tTDAL_AVi_AudioState   AudioState;
   tTDAL_AV_SampleType   SampleType;
   tTDAL_AV_AudioType   AudioType;
   tTDAL_AV_AudioStereoMode   StereoMode;
   tTDAL_AV_AudioCapability   Capability;
   tTDAL_AV_CallbackProc_t   NewFrequencyCallback;
   tTDAL_AV_CallbackProc_t   SamplePlayedCallback;
   tTDAL_AV_CallbackProc_t   SampleStoppedCallback;
}tTDAL_AVi_AudioDecoderDescriptor;


/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)   *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE)   *
 ****************************************************************************/
/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)   *
 ****************************************************************************/

LOCAL MS_BOOL _bAudioInit = FALSE;
LOCAL tTDAL_AVi_AudioDecoderDescriptor   TDAL_AV_AudioDecoderDescriptor;

LOCAL uint8_t *  TDAL_AVi_AudioSampleData = 0;
LOCAL uint32_t   TDAL_AVi_AudioSampleSize = 0;
LOCAL uint16_t   TDAL_AVi_AudioSampleNbTimes = 0;

LOCAL uint32_t   TDAL_AVi_AudioSamplePointer = 0;
LOCAL uint16_t   TDAL_AVi_AudioSampleNbTimesLeft = 0;

TDAL_task_id          TDAL_AVi_AudioSampleTaskHandle = NULL;
uint8_t               TDAL_AVi_AudioSampleTaskStack[kTDAL_AVi_AUDIO_SAMPLE_TASK_STACK_SIZE];
LOCAL bool            TDAL_AVi_AudioSampleStop = false;
LOCAL TDAL_mutex_id   TDAL_AVi_AudioSampleMutex = NULL;

TDAL_task_id          TDAL_AVi_AudioMonitor = NULL;
uint8_t               TDAL_AVi_AudioMonitorStack[kTDAL_AVi_AUDIO_MONITOR_TASK_STACK_SIZE];
LOCAL bool            TDAL_AVi_AudioMonitorStop = false;
LOCAL TDAL_mutex_id   TDAL_AVi_AudioMutex = NULL;
LOCAL AUDIO_FS_TYPE _stCurSampleType = AUDIO_FS_NONE;
LOCAL HDMI_TX_OUTPUT_TYPE _stCurHDMIType = HDMI_OUT_PCM;


LOCAL tTDAL_AV_StreamType TDAL_AVi_AudioStreamType;

LOCAL tTDAL_AV_Error TDAL_AVi_StopRunningSample(void);
LOCAL bool _bAudioStarted = FALSE;
LOCAL bool _NeedUnmute = FALSE;
/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)   *
 ****************************************************************************/
bool TDAL_AV_AudioStarted(void);
#ifdef PACKAGE_PVR
IMPORT bool TDAL_PVR_IsPlaybacking(void);
#endif

bool TDAL_AVm_GetAudioNeedUnmute(void)
{
    bool _needunmute = FALSE;
    TDAL_LockMutex(TDAL_AVi_AudioMutex);
    _needunmute = _NeedUnmute;
    TDAL_UnlockMutex(TDAL_AVi_AudioMutex);
    return _needunmute;
}

bool TDAL_AVm_SetAudioNeedUnmute(bool NeedUnmute)
{
    TDAL_LockMutex(TDAL_AVi_AudioMutex);
    _NeedUnmute = NeedUnmute;
    TDAL_UnlockMutex(TDAL_AVi_AudioMutex);
    return TRUE;
}
/**========================================================================**
 * Function   : TDAL_AV_AudioCapabilityGet
 *
 * Scope   : return the audio config
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioCapabilityGet(tTDAL_AV_Decoder decoder,tTDAL_AV_AudioCapability *pstCapability)
{
	mTBOX_FCT_ENTER("TDAL_AV_AudioCapabilityGet");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	if (pstCapability == NULL)
	{
		mTBOX_RETURN(eTDAL_AV_BAD_PARAMETER_ERROR);
	}

	/* copy already initialized fields */
	memcpy(pstCapability, &TDAL_AV_AudioDecoderDescriptor.Capability,
			sizeof(tTDAL_AV_AudioCapability));

	mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : TDAL_AV_AudioStereoModeGet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioStereoModeGet(tTDAL_AV_Decoder decoder,tTDAL_AV_AudioStereoMode *pstMode)
{
	tTDAL_AV_Error ErrorRet = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_AV_AudioStereoModeGet");

	*(pstMode) = eTDAL_AV_AUDIO_STEREO_MODE_STEREO;
	mTBOX_RETURN(ErrorRet);
}

/**========================================================================**
 * Function   : TDAL_AV_AudioDestinationSet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioDestinationSet(tTDAL_AV_Decoder eDecoder, uint32_t SpeakerHandle)
{
	tTDAL_AV_Error ErrorRet = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_AV_AudioDestinationSet");

	 //long long MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );

	mTBOX_RETURN(ErrorRet);
}


/**========================================================================**
 * Function   : TDAL_AV_AudioStereoModeSet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioStereoModeSet(tTDAL_AV_Decoder decoder, tTDAL_AV_AudioStereoMode mode)
{
	MS_BOOL ret;
	tTDAL_AV_Error returnValue;
	MS_U32 soundModeParam;

    mTBOX_FCT_ENTER(TDAL_AV_AudioStereoModeSet);

	returnValue = eTDAL_AV_NO_ERROR;

	switch(mode)
	{
	case eTDAL_AV_AUDIO_STEREO_MODE_MONO:
	    soundModeParam = 3;
	    break;
	case eTDAL_AV_AUDIO_STEREO_MODE_STEREO:
	    soundModeParam = 0;
	    break;
	case eTDAL_AV_AUDIO_STEREO_MODE_STEREO_DUAL_LEFT:
	    soundModeParam = 1;
	    break;
	case eTDAL_AV_AUDIO_STEREO_MODE_STEREO_DUAL_RIGHT:
	    soundModeParam = 2;
	    break;
	default:
	    returnValue = eTDAL_AV_LIMITATION_ERROR;
	    break;
	}

	if (returnValue == eTDAL_AV_NO_ERROR)
	{
	    ret = MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, soundModeParam, 0);
        if (!ret)
        {
            returnValue = eTDAL_AV_ERROR;
        }
	}

	mTBOX_RETURN(returnValue);
}


/**========================================================================**
 * Function   : TDAL_AV_Audio_BeepPlay
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioBeepPlay(tTDAL_AV_Decoder decoder,uint16_t frequency,uint16_t duration)
{
   return(eTDAL_AV_NO_ERROR);
}

LOCAL void TDAL_AVi_Audio_HDMI_TX_Status_Monitor()
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
        MApi_HDMITx_SetAudioOnOff(TRUE);
        MApi_HDMITx_SetAudioFrequency(hreq);
        MApi_HDMITx_SetAudioConfiguration(hreq, HDMITX_AUDIO_CH_2, htype);

        if (TDAL_AVm_GetAudioNeedUnmute() == TRUE)
        {
            TDAL_OUTPUTi_AudioAnaEnable(TRUE);
        }
    }

}

LOCAL void TDAL_AVi_AudioMonitorTaskFct(void * argv)
{
        while(TDAL_AVi_AudioMonitorStop == FALSE)
        {
            //dump debug message and monitor if DSP alive
            MApi_Audio_Monitor();

            if (_bAudioStarted==FALSE)
            {
                MsOS_DelayTask(10);
                continue;
            }
            //monitor if HDMI Tx status change
            TDAL_AVi_Audio_HDMI_TX_Status_Monitor();

            //monitor smple rate change
            MApi_Audio_SPDIF_Monitor();
            if (TDAL_AVm_GetAudioNeedUnmute() && TDAL_AV_VideoDecoding())
            {
                TDAL_OUTPUTi_AudioAnaEnable(TRUE);
            }
            MsOS_DelayTask(1);
        }
        TDAL_AVi_AudioMonitorStop = FALSE;
        TDAL_AVi_AudioMonitor = NULL;
}

LOCAL void TDAL_AVi_AudioSampleTaskFct(void *param)
{
    MS_U32 WrtAddr, WrtBytes;
    bool sampleFinishedFeeding = false;

    mTBOX_FCT_ENTER(TDAL_AVi_AudioSampleTaskFct);

    TDAL_LockMutex(TDAL_AVi_AudioSampleMutex);
    TDAL_UnlockMutex(TDAL_AVi_AudioSampleMutex);

    //new MM mode play command
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
    //can set av sync or free run
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
    AES_INFO es_info;
    MS_BOOL bRet = FALSE;
    MS_U32 u32RemainedSize = TDAL_AVi_AudioSampleSize;
    MS_U32 u32es_size = 0,_u32StartTime = MsOS_GetSystemTime();
    MS_U64 u64PTS = 0;
    MS_BOOL bSetPTS = TRUE;
    while (!TDAL_AVi_AudioSampleStop && !sampleFinishedFeeding)
    {
        bRet = MApi_AUDIO_MM2_checkAesInfo(AU_DEC_ID1, &es_info);
        WrtAddr = es_info.aes_write_addr;
        WrtBytes = es_info.aes_freeSpace;
        if (bRet)
        {
            mTBOX_TRACE((kTBOX_NIV_3,"[%s][%d]WrtAddr=%lx WrtBytes=%lx\n",__FUNCTION__,__LINE__,WrtAddr,WrtBytes));
            u32RemainedSize = TDAL_AVi_AudioSampleSize - TDAL_AVi_AudioSamplePointer;
            if (u32RemainedSize > 0)
            {
                if(WrtBytes>0)
                {
                    u32es_size = MIN(WrtBytes, u32RemainedSize);

                    memcpy((void*)MS_PA2KSEG1(MApi_AUDIO_GetDDRInfo(AU_DEC_ID1, E_AUD_MEMORY_BASE)+WrtAddr), (TDAL_AVi_AudioSampleData + TDAL_AVi_AudioSamplePointer), u32es_size);

                    TDAL_AVi_AudioSamplePointer += u32es_size;

                    //example of setting system time differemce (ticks) as PTS
                    u64PTS = (MsOS_GetSystemTime() - _u32StartTime) * 90;

                    //info DSP data is ready
                    MApi_AUDIO_MM2_inputAesFinished(AU_DEC_ID1, u32es_size, bSetPTS, u64PTS);
                }            
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_3,"Sample Play Finished!!\n"));
                sampleFinishedFeeding = TRUE;
                TDAL_AVi_AudioSampleStop = TRUE;
                break;
            }
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_3, "MApi_AUDIO_MM2_checkAesInfo Fail\n"));
            MsOS_DelayTask(1);
        }
    }

    while (!TDAL_AVi_AudioSampleStop && (MApi_AUDIO_CheckPlayDone() == FALSE))
    {
        MsOS_DelayTask(20);
        mTBOX_TRACE((kTBOX_NIV_3, "TDAL_AVi_AudioSampleTaskFct: Waiting to finish playing rest of the file...\n"));
    }

    if (!TDAL_AVi_AudioSampleStop)
    {
        if (TDAL_AV_AudioDecoderDescriptor.SamplePlayedCallback)
        {
            TDAL_AV_AudioDecoderDescriptor.SamplePlayedCallback(kTDAL_AVi_AUDIO_DECODER, eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED, NULL);
        }
    }
    else
    {
        if (TDAL_AV_AudioDecoderDescriptor.SampleStoppedCallback)
        {
            TDAL_AV_AudioDecoderDescriptor.SampleStoppedCallback(kTDAL_AVi_AUDIO_DECODER, eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED, NULL);
        }
    }

    TDAL_AVi_AudioSampleTaskHandle = NULL;

    mTBOX_RETURN;
}

LOCAL tTDAL_AV_Error TDAL_AVi_StopRunningSample()
{
    mTBOX_FCT_ENTER("TDAL_AVi_StopRunningSample");

    TDAL_AVi_AudioSampleStop = TRUE;

    while (TDAL_AVi_AudioSampleTaskHandle != NULL)
    {
        MsOS_DelayTask(10);
    }
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    mTBOX_RETURN(eTDAL_AV_NO_ERROR);

}

/**========================================================================**
 * Function   : TDAL_AV_AudioSampleStart
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioSampleStart( tTDAL_AV_Decoder   decoder,
   tTDAL_AV_SampleType   sampleType,
   uint8_t *   pData,
   uint32_t   size,
   uint16_t   nbTimes )
{
    tTDAL_AV_Error status = eTDAL_AV_NO_ERROR;
    En_DVB_decCmdType cmdType;

    mTBOX_FCT_ENTER("TDAL_AV_AudioSampleStart");

    if (pData == NULL || nbTimes == 0 || size == 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_AV_AudioSampleStart: Bad param\n"));
        mTBOX_RETURN(eTDAL_AV_BAD_PARAMETER_ERROR);
    }

    if (decoder != kTDAL_AVi_AUDIO_DECODER)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_AV_AudioSampleStart: Unsupported decoder %x\n", decoder));
        mTBOX_RETURN(eTDAL_AV_LIMITATION_ERROR);
    }

    if (sampleType != eTDAL_AV_SAMPLE_MP3 && sampleType != eTDAL_AV_SAMPLE_MPEG1 && sampleType != eTDAL_AV_SAMPLE_MPEG2)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unsupported sample type %d\n", sampleType ));
        mTBOX_RETURN(eTDAL_AV_LIMITATION_ERROR);
    }

    TDAL_AVi_StopRunningSample();
   
    TDAL_LockMutex(TDAL_AVi_AudioSampleMutex);

    if (status == eTDAL_AV_NO_ERROR)
    {
        TDAL_AVi_AudioSamplePointer = 0;
        TDAL_AVi_AudioSampleData = pData;
        TDAL_AVi_AudioSampleNbTimes = nbTimes;
        TDAL_AVi_AudioSampleSize = size;
        TDAL_AVi_AudioSampleStop = false;
        TDAL_AVi_AudioSampleNbTimesLeft = nbTimes;
    }

    mTBOX_TRACE((kTBOX_NIV_1, "Starting TDAL_AV_AudioSampleStart\n"));

    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    MApi_AUDIO_MM2_initAesInfo(AU_DEC_ID1);
    if (status == eTDAL_AV_NO_ERROR)
    {
        switch(sampleType)
        {
        case eTDAL_AV_SAMPLE_MP3:
            MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MP3);
            break;
        case eTDAL_AV_SAMPLE_MPEG1:
        case eTDAL_AV_SAMPLE_MPEG2:
            MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
            break;
        default:
            mTBOX_ASSERTm(FALSE, "Should not get here...\n");
            MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
        }

    }

    if (status == eTDAL_AV_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "Creating audio thread\n"));
        TDAL_AVi_AudioSampleTaskHandle = TDAL_CreateTask(
                eTDAL_PRIORITY_NORMAL,
                "AudioSampleTask",
                TDAL_AVi_AudioSampleTaskStack,
                kTDAL_AVi_AUDIO_SAMPLE_TASK_STACK_SIZE,
                TDAL_AVi_AudioSampleTaskFct,
                NULL);

        if (TDAL_AVi_AudioSampleTaskHandle == NULL)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not create audio thread\n"));
            status = eTDAL_AV_ERROR;
        }
    }

    TDAL_UnlockMutex(TDAL_AVi_AudioSampleMutex);

    mTBOX_RETURN(status);
}

/**========================================================================**
 * Function   : TDAL_AV_AudioSampleStop
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_AudioSampleStop(tTDAL_AV_Decoder decoder)
{
    mTBOX_FCT_ENTER("TDAL_AV_AudioSampleStop");

    TDAL_AVi_StopRunningSample();

    mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}

tTDAL_AV_Error   pTDAL_AV_Audio_Initialize(void)
{
    tTDAL_AV_Error   Error = eTDAL_AV_NO_ERROR;
    mTBOX_FCT_ENTER("pTDAL_AV_Audio_Initialize");

    AUDIO_INIT_INFO SystemInfo;
     AUDIO_OUT_INFO OutputInfo;
     AUDIO_PATH_INFO PathInfo;
    MS_U8 _NPcm_MuteFRMCNT = 10;
    MS_U8 _Pcm_MuteSMPCNT = 0;
    MS_U8 _NPcm_NullFRMCNT = 5;

     if (TRUE == _bAudioInit)
     {
         mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] Audio already initalized\n"));
         mTBOX_RETURN(Error);
     }
     

     
     MApi_AUDIO_WritePreInitTable();
     
     SystemInfo.tv_system=TV_PAL;
     SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
     SystemInfo.dtv_system=0;
     

     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetSystemInfo()\n"));
     MApi_AUDIO_SetSystemInfo(&SystemInfo);

/*
     OutputInfo.SpeakerOut=AUDIO_OUTPUT_MAIN_SPEAKER;
     OutputInfo.HpOut=AUDIO_OUTPUT_HP;
     OutputInfo.ScartOut=AUDIO_OUTPUT_SIFOUT;
     OutputInfo.MonitorOut=AUDIO_OUTPUT_LINEOUT;
*/
    OutputInfo.SpeakerOut = AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut = AUDIO_OUTPUT_HP;
    OutputInfo.ScartOut = AUDIO_OUTPUT_SCART1;
    OutputInfo.MonitorOut = AUDIO_NULL_OUTPUT;

     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetOutputInfo()\n"));
     MApi_AUDIO_SetOutputInfo(&OutputInfo);

/*
     PathInfo.SpeakerOut = AUDIO_PATH_MAIN;
     PathInfo.HpOut = 0;
     PathInfo.MonitorOut = AUDIO_PATH_1;
     PathInfo.SpdifOut = AUDIO_PATH_2;
     PathInfo.ScartOut = AUDIO_PATH_1;
     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetPathInfo()\n"));
     MApi_AUDIO_SetPathInfo(&PathInfo);
*/
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

     //MW_AUD_GetBinAddress();//expand to next two functions:
#if 0 //not used for K5
     MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
     MDrv_AUDIO_SetDspBaseAddr(DSP_SE,  0, MAD_SE_BUFFER_ADR);
#endif
     MDrv_AUDIO_SetDspBaseAddr(DSP_ADV,  0, MAD_ADV_BUF_ADR);
     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_Initialize()\n"));
     MApi_AUDIO_Initialize();

     MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_DTV_IN);

     MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);
     MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);

#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif

     mTBOX_TRACE((kTBOX_NIV_1, "Creating audio monitor task\n"));
     TDAL_AVi_AudioMonitor = TDAL_CreateTask(
             eTDAL_PRIORITY_HIGH,
             "AudioMonitorTask",
             TDAL_AVi_AudioMonitorStack,
             kTDAL_AVi_AUDIO_MONITOR_TASK_STACK_SIZE,
             TDAL_AVi_AudioMonitorTaskFct,
             NULL);

     if (TDAL_AVi_AudioMonitor == NULL)
     {
         mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Creating audio monitor task fail!\n"));
         Error = eTDAL_AV_ERROR;
     }

     mTBOX_TRACE((kTBOX_NIV_1, "Audio init\n"));

     _bAudioInit = TRUE;
     _NeedUnmute = FALSE;
     
     MDrv_AUDIO_WriteReg(0x2DD0, _NPcm_MuteFRMCNT);
     MDrv_AUDIO_WriteReg(0x2DD2, _Pcm_MuteSMPCNT);
     MDrv_AUDIO_WriteReg(0x2DD4, _NPcm_NullFRMCNT);
     
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, TRUE);
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, TRUE);
    MApi_AUDIO_SPDIF_SetMute(TRUE);
    MApi_AUDIO_HDMI_Tx_SetMute(TRUE);

    mTBOX_RETURN(Error);

}

/**========================================================================**
 * Function   : p_TDAL_AV_Audio_Start
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
tTDAL_AV_Error   p_TDAL_AV_Audio_Start( tTDAL_AV_Decoder decoder, tTDAL_AV_StreamType StreamType, bool stopAudioThread )
{
     tTDAL_AV_Error   Error = eTDAL_AV_NO_ERROR;

    En_DVB_decSystemType AudioCodecType = MSAPI_AUD_DVB_INVALID;

    mTBOX_FCT_ENTER("p_TDAL_AV_Audio_Start");

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Start] Audio start, stream type = %d\n", StreamType.audioType));

	fflush(stdout);

    Error = pTDAL_AV_Audio_Initialize();

    if (_bAudioStarted == TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] Audio is already running!\n" ));
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);        
    }
#ifdef PACKAGE_PVR
    if (TDAL_PVR_IsPlaybacking())
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] PVR is Playbacking!\n" ));
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);
    } 
#endif

    if (stopAudioThread)
    {
        TDAL_AVi_StopRunningSample();
    }

    switch(StreamType.audioType)
    {
    case eTDAL_AV_AUDIO_TYPE_AC3:
    	AudioCodecType = MSAPI_AUD_DVB_AC3;
    	break;
    case eTDAL_AV_AUDIO_TYPE_MPEG:
    	AudioCodecType = MSAPI_AUD_DVB_MPEG;
    	break;
    case eTDAL_AV_AUDIO_TYPE_HE_AAC:
    case eTDAL_AV_AUDIO_TYPE_MPEG_AAC:
    	AudioCodecType = MSAPI_AUD_DVB_AAC;
    	break;
    case eTDAL_AV_AUDIO_TYPE_EAC3:
    	AudioCodecType = MSAPI_AUD_DVB_AC3P;
    	break;
    default:
        AudioCodecType = MSAPI_AUD_DVB_NONE;
    	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Audio_Start] Unsupported audio codec %d\n", StreamType.audioType));
    	break;
    }

    TDAL_AVi_AudioStreamType = StreamType;

    if (Error == eTDAL_AV_NO_ERROR)
    {
    	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Start] Audio play\n"));
    	//Set MAD
		 //MDrv_AUDIO_SetNormalPath(AUDIO_PATH_MAIN_SPEAKER, AUDIO_SOURCE_DTV, AUDIO_OUTPUT_MAIN_SPEAKER);
		 MApi_AUDIO_SetSystem(AudioCodecType);
		 //MApi_AUDIO_SetCommand((En_DVB_decCmdType)MSAPI_AUD_PLAY);
		MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);
    }

    if (Error == eTDAL_AV_NO_ERROR)
    {
    	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Start]    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE)\n"));
    	MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);

    	if (TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback != NULL)
    	{
    		tTDAL_AV_AudioFrequency audioFreq = (tTDAL_AV_AudioFrequency) MApi_AUDIO_GetSampleRate();

    		TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback(kTDAL_AVi_AUDIO_DECODER, eTDAL_AV_EVENT_AUDIO_NEW_FREQUENCY, &audioFreq);
    	}
    }
     //Unoomment it for AC3 HDMI support
     //MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_NONPCM);
    _bAudioStarted = TRUE;
    mTBOX_RETURN(Error);
}

/**========================================================================**
 * Function   : p_TDAL_AV_Audio_Stop
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
tTDAL_AV_Error   p_TDAL_AV_Audio_Stop( tTDAL_AV_Decoder decoder )
{
	DMX_FILTER_STATUS dmxStatus;
	tTDAL_AV_Error error = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("p_TDAL_AV_Audio_Stop");

	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Stop]   MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE)\n"));

    if (_bAudioStarted == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] Audio is not running!\n" ));
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);        
    }
#ifdef PACKAGE_PVR
    if (TDAL_PVR_IsPlaybacking())
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Start] PVR is Playbacking!\n" ));
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);
    } 
#endif
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, TRUE);
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, TRUE);
    MApi_AUDIO_SPDIF_SetMute(TRUE);
    MApi_AUDIO_HDMI_Tx_SetMute(TRUE);

	TDAL_AVi_StopRunningSample();
	
	dmxStatus = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
	mTBOX_ASSERT(dmxStatus == DMX_FILTER_STATUS_OK);

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Stop]   Stop audio\n"));
    //MApi_AUDIO_SetCommand( (En_DVB_decCmdType)MSAPI_AUD_STOP ); Evan
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    _bAudioStarted = FALSE;
    _NeedUnmute = FALSE;
    mTBOX_RETURN(error);
}

/**========================================================================**
 * Function   : p_TDAL_AV_Audio_EventSubscribe
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment   :
 *
 **========================================================================**/
tTDAL_AV_Error   p_TDAL_AV_Audio_EventSubscribe
(
   tTDAL_AV_Decoder decoder,
   tTDAL_AV_Event event,
   tTDAL_AV_CallbackProc_t notify_function
)
{
	tTDAL_AV_CallbackProc_t * pClbkStorage;

	mTBOX_FCT_ENTER("p_TDAL_AV_Audio_EventSubscribe");

	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_EventSubscribe]  Event id %d\n", event));

	switch (event)
	{
	case eTDAL_AV_EVENT_AUDIO_NEW_FREQUENCY:
		pClbkStorage = &TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback;
		break;

	case eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED:
		pClbkStorage = &TDAL_AV_AudioDecoderDescriptor.SamplePlayedCallback;
		break;

	case eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED:
		pClbkStorage = &TDAL_AV_AudioDecoderDescriptor.SampleStoppedCallback;
		break;

	default:
		/* ASSERTION here */
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
tTDAL_AV_Error   p_TDAL_AV_Audio_EventUnsubscribe
(
   tTDAL_AV_Decoder decoder,
   tTDAL_AV_Event event
)
{
	mTBOX_FCT_ENTER("p_TDAL_AV_Audio_EventUnsubscribe");

	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_EventUnsubscribe]  Event id %d\n", event));

	switch (event)
	{
	case eTDAL_AV_EVENT_AUDIO_NEW_FREQUENCY:
		TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED:
		TDAL_AV_AudioDecoderDescriptor.SamplePlayedCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	case eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED:
		TDAL_AV_AudioDecoderDescriptor.SampleStoppedCallback =
				(tTDAL_AV_CallbackProc_t) NULL;
		break;

	default:
		/* ASSERTION here */
		mTBOX_RETURN(eTDAL_AV_LIMITATION_ERROR);
	}

	mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}

/**========================================================================**
 * Function   : p_TDAL_AV_Audio_Init
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
tTDAL_AV_Error   p_TDAL_AV_Audio_Init( void )
{
	tTDAL_AV_Error err = eTDAL_AV_NO_ERROR;

	mTBOX_FCT_ENTER("p_TDAL_AV_Audio_Init");

	TDAL_CreateMutex(&TDAL_AVi_AudioSampleMutex);
        TDAL_CreateMutex(&TDAL_AVi_AudioMutex);

	mTBOX_ASSERT(TDAL_AVi_AudioSampleMutex != NULL);
	mTBOX_ASSERT(TDAL_AVi_AudioMutex != NULL);

	TDAL_AV_AudioDecoderDescriptor.AudioType  = eTDAL_AV_AUDIO_TYPE_MPEG;
    TDAL_AV_AudioDecoderDescriptor.SampleType = eTDAL_AV_SAMPLE_MPEG1;
    TDAL_AV_AudioDecoderDescriptor.StereoMode = eTDAL_AV_AUDIO_STEREO_MODE_STEREO;

    TDAL_AV_AudioDecoderDescriptor.Capability.isBeepSupported   = FALSE;
    TDAL_AV_AudioDecoderDescriptor.Capability.isSpeedSupported   = FALSE;
    TDAL_AV_AudioDecoderDescriptor.Capability.sampleTypeSupported   = eTDAL_AV_SAMPLE_MPEG1 | eTDAL_AV_SAMPLE_PCM | eTDAL_AV_SAMPLE_MPEG2 | eTDAL_AV_SAMPLE_AC3 | eTDAL_AV_SAMPLE_MP3 | eTDAL_AV_SAMPLE_WAVE;
    TDAL_AV_AudioDecoderDescriptor.Capability.speedBackwardMax   = FALSE;
    TDAL_AV_AudioDecoderDescriptor.Capability.speedForwardMax   = FALSE;
    //TDAL_AV_AudioDecoderDescriptor.AudioState   = eTDAL_AVi_AUDIO_STATE_STOPPED;
    TDAL_AV_AudioDecoderDescriptor.SampleType   = eTDAL_AV_SAMPLE_NONE;

    TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback = NULL;
    TDAL_AV_AudioDecoderDescriptor.SamplePlayedCallback = NULL;
    TDAL_AV_AudioDecoderDescriptor.SampleStoppedCallback = NULL;

    err = pTDAL_AV_Audio_Initialize();

	mTBOX_RETURN(err);
}


/**========================================================================**
 * Function   : p_TDAL_AV_Audio_Term
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
tTDAL_AV_Error   p_TDAL_AV_Audio_Term( void )
{
    TDAL_AVi_StopRunningSample();

    TDAL_DeleteMutex(TDAL_AVi_AudioSampleMutex);
    TDAL_AVi_AudioSampleMutex = NULL;

    TDAL_DeleteMutex(TDAL_AVi_AudioMutex);
    TDAL_AVi_AudioMutex = NULL;
    
    TDAL_AVi_AudioMonitorStop = TRUE;
    _bAudioInit = FALSE;
    return eTDAL_AV_NO_ERROR;
}


/**========================================================================**
 * Function   : p_TDAL_AV_Audio_UseEvent
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
void p_TDAL_AV_Audio_UseEvent(tTDAL_AV_Decoder  decoder, MS_U32 event)
{
}

bool TDAL_AV_AudioStarted(void)
{
    return _bAudioStarted;
}

tTDAL_AV_AudioType TDAL_AV_Audio_StreamType( void )
{
    return TDAL_AVi_AudioStreamType.audioType;
}

