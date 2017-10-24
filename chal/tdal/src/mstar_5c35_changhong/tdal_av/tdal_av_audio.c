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

LOCAL tTDAL_AV_StreamType TDAL_AVi_AudioStreamType;
LOCAL bool                TDAL_AVi_RestartAudioAfterSampleFinished = false;

LOCAL tTDAL_AV_Error TDAL_AVi_StopRunningSample(void);

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)   *
 ****************************************************************************/

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

	printf("TDAL_AV_AudioStereoModeGet YP TODO %s %d\r\n", __FILE__, __LINE__);

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

	printf("TDAL_AV_AudioDestinationSet TODO %s %d\r\n", __FILE__, __LINE__);
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

LOCAL void TDAL_AVi_AudioSampleTaskFct(void *param)
{
    MS_U32 WrtAddr, WrtBytes;
    bool sampleFinishedFeeding = false;

    mTBOX_FCT_ENTER(TDAL_AVi_AudioSampleTaskFct);

    TDAL_LockMutex(TDAL_AVi_AudioSampleMutex);
    TDAL_UnlockMutex(TDAL_AVi_AudioSampleMutex);

    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
    mTBOX_TRACE((kTBOX_NIV_1, "MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);\n"));

    while (!TDAL_AVi_AudioSampleStop && !sampleFinishedFeeding)
    {
        if(MApi_AUDIO_CheckInputRequest(&WrtAddr, &WrtBytes))
        {
            mTBOX_TRACE((kTBOX_NIV_3, "TDAL_AVi_AudioSampleTaskFct: MApi_AUDIO_CheckInputRequest"));

            TDAL_LockMutex(TDAL_AVi_AudioSampleMutex);

            if (TDAL_AVi_AudioSamplePointer + WrtBytes <= TDAL_AVi_AudioSampleSize)
            {
                memcpy((void*)MS_PA2KSEG1(WrtAddr), (void*) (TDAL_AVi_AudioSampleData + TDAL_AVi_AudioSamplePointer), WrtBytes);
                TDAL_AVi_AudioSamplePointer += WrtBytes;
            }
            else
            {
                // clear the buffer first
                memset((void*)MS_PA2KSEG1(WrtAddr), 0, WrtBytes);

                if (WrtBytes <= TDAL_AVi_AudioSampleSize)
                {
                    // normal case
                    int feedsize = 0;

                    memcpy((void*)MS_PA2KSEG1(WrtBytes),(void*)(TDAL_AVi_AudioSampleData + TDAL_AVi_AudioSamplePointer), TDAL_AVi_AudioSampleSize - TDAL_AVi_AudioSamplePointer);
                    feedsize += (TDAL_AVi_AudioSampleSize - TDAL_AVi_AudioSamplePointer);

                    TDAL_AVi_AudioSampleNbTimesLeft--;
                    if (TDAL_AVi_AudioSampleNbTimesLeft > 0)
                    {
                        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_AVi_AudioSampleTaskFct: Number of times left to play sample = %d\n", TDAL_AVi_AudioSampleNbTimesLeft));
                        TDAL_AVi_AudioSamplePointer = 0;
                        memcpy((void*)MS_PA2KSEG1(WrtAddr + feedsize), (void*)(TDAL_AVi_AudioSampleData + TDAL_AVi_AudioSamplePointer), WrtBytes - feedsize);

                        TDAL_AVi_AudioSamplePointer = WrtBytes- feedsize;
                    }
                    else
                    {
                        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_AVi_AudioSampleTaskFct: Sample finished playing\n"));
                        sampleFinishedFeeding = true;
                    }
                }
            }

            TDAL_UnlockMutex(TDAL_AVi_AudioSampleMutex);

            MApi_AUDIO_SetInput();
            MsOS_DelayTask(10);
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_3, "TDAL_AVi_AudioSampleTaskFct: In loop...\n"));
            MsOS_DelayTask(20);
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

    MApi_AUDIO_StopDecode();

    if (TDAL_AVi_RestartAudioAfterSampleFinished)
    {
        p_TDAL_AV_Audio_Start(kTDAL_AVi_AUDIO_DECODER, TDAL_AVi_AudioStreamType, false);
        TDAL_AVi_RestartAudioAfterSampleFinished = false;
    }

    mTBOX_RETURN;
}

LOCAL tTDAL_AV_Error TDAL_AVi_StopRunningSample()
{
    mTBOX_FCT_ENTER("TDAL_AVi_StopRunningSample");

    TDAL_AVi_AudioSampleStop = true;

    if (TDAL_AVi_AudioSampleTaskHandle)
    {
        TDAL_DeleteTask(TDAL_AVi_AudioSampleTaskHandle);
        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_AVi_StopRunningSample: TDAL_DeleteTask(TDAL_AVi_AudioSampleTaskHandle);\n"));
        TDAL_AVi_AudioSampleTaskHandle = NULL;
    }

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
   
    cmdType = MApi_AUDIO_GetDecStatus();
    if (cmdType != MSAPI_AUD_DVB_DECCMD_STOP)
    {
        TDAL_AVi_RestartAudioAfterSampleFinished = true;
        p_TDAL_AV_Audio_Stop(kTDAL_AVi_AUDIO_DECODER);
    }
    
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

        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);

        MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);

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

     if (TRUE == _bAudioInit)
     {
         mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] Audio already initalized\n"));
    	 mTBOX_RETURN(Error);
     }

     SystemInfo.tv_system=TV_PAL;
     SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
     SystemInfo.dtv_system=0;

     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetSystemInfo()\n"));
     MApi_AUDIO_SetSystemInfo(&SystemInfo);

     OutputInfo.SpeakerOut=AUDIO_OUTPUT_MAIN_SPEAKER;
     OutputInfo.HpOut=AUDIO_OUTPUT_HP;
     OutputInfo.ScartOut=AUDIO_OUTPUT_SIFOUT;
     OutputInfo.MonitorOut=AUDIO_OUTPUT_LINEOUT;
     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetOutputInfo()\n"));
     MApi_AUDIO_SetOutputInfo(&OutputInfo);

     PathInfo.SpeakerOut = AUDIO_PATH_MAIN;
     PathInfo.HpOut = 0;
     PathInfo.MonitorOut = AUDIO_PATH_1;
     PathInfo.SpdifOut = AUDIO_PATH_2;
     PathInfo.ScartOut = AUDIO_PATH_1;
     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetPathInfo()\n"));
     MApi_AUDIO_SetPathInfo(&PathInfo);

     //MW_AUD_GetBinAddress();//expand to next two functions:
     MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
     MDrv_AUDIO_SetDspBaseAddr(DSP_SE,  0, MAD_SE_BUF_ADR);
     mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_Initialize()\n"));
     MApi_AUDIO_Initialize();
     //MApi_AUDIO_EnableTone(TRUE);
     if (FALSE == MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0) )
     {
         mTBOX_TRACE((kTBOX_NIV_1, "[pTDAL_AV_Audio_Initialize] MApi_AUDIO_SetAC3PInfo() fail\n"));
     }
#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif

     mTBOX_TRACE((kTBOX_NIV_1, "Audio init\n"));

     _bAudioInit = TRUE;

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
    MS_BOOL bfRet = FALSE;
    DMX_FILTER_STATUS eDMXRet = DMX_FILTER_STATUS_ERROR;

    En_DVB_decSystemType AudioCodecType = MSAPI_AUD_DVB_INVALID;

    mTBOX_FCT_ENTER("p_TDAL_AV_Audio_Start");

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Start] Audio start, stream type = %d\n", StreamType.audioType));

    Error = pTDAL_AV_Audio_Initialize();

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
		 bfRet = MApi_AUDIO_SetSystem(AudioCodecType);
		 if ( FALSE == bfRet)
		 {
             mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Audio_Start] MApi_AUDIO_SetSystem fail\n"));
		 }
		 MApi_AUDIO_SetCommand((En_DVB_decCmdType)MSAPI_AUD_PLAY);
    }

    if (Error == eTDAL_AV_NO_ERROR)
    {
    	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Start]    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE)\n"));
    	eDMXRet = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
        if ( DMX_FILTER_STATUS_ERROR == eDMXRet)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[p_TDAL_AV_Audio_Start] MApi_DMX_AVFifo_Reset audio fail\n"));
        }
    	if (TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback != NULL)
    	{
    		tTDAL_AV_AudioFrequency audioFreq = (tTDAL_AV_AudioFrequency) MApi_AUDIO_GetSampleRate();

    		TDAL_AV_AudioDecoderDescriptor.NewFrequencyCallback(kTDAL_AVi_AUDIO_DECODER, eTDAL_AV_EVENT_AUDIO_NEW_FREQUENCY, &audioFreq);
    	}
    }

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

	TDAL_AVi_StopRunningSample();
	
	dmxStatus = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
	mTBOX_ASSERT(dmxStatus == DMX_FILTER_STATUS_OK);

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_AV_Audio_Stop]   Stop audio\n"));
    MApi_AUDIO_SetCommand( (En_DVB_decCmdType)MSAPI_AUD_STOP );

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
	mTBOX_ASSERT(TDAL_CreateMutex != NULL);

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
