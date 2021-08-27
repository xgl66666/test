/*****************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_AV
 *
 * FILE NAME: URL:  $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_av/tdal_av.c $
 *            Rev:  $Rev: 414 $
 *            Date: $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : AudioVideo TDAL library implementation.
 * Target : All
 * Author : Arnaud VIALA (arv).
 * Creation : 17/11/2004.
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES                                                                *
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




/****************************************************************************
 *  DEFINES                                                                 *
 ****************************************************************************/
#define kTDAL_AV_EVT_TASK_NAME         "tdalAVEvt"
#define kTDAL_AV_EVT_QUEUE_NAME      "tdalAVEvtQueue"

#define kTDAL_AV_EVT_TASK_STACK_SIZE  5120

/****************************************************************************
 *  MACROS                                                                  *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_AV);

/****************************************************************************
 *  TYPEDEFS                                                                *
 ****************************************************************************/

/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                                        *
 ****************************************************************************/

TDAL_mutex_id          TDAL_AVi_Mutex = NULL;

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE/IMPORT)                                  *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                                            *
 ****************************************************************************/
bool               TDAL_AV_AlreadyInitialized = FALSE;
TDAL_queue_id			   TDAL_AV_EvtQueue_p = -1;
MS_S32 			   TDAL_AV_EvtTaskID = -1;
LOCAL bool            TDAL_AV_EvtTaskStop;
LOCAL volatile bool            TDAL_AV_EvtTaskFinished;

LOCAL unsigned char     TDAL_AV_EvtTaskStack[kTDAL_AV_EVT_TASK_STACK_SIZE];

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                                    *
 ****************************************************************************/

LOCAL void p_TDAL_AV_EventsTask(MS_U32 argc, void * argv);

const char * TDAL_AV_APIRevisionGet(void )
{
    static const char API_Revision[] = "TDAL_AV_v1.4";
    return((const char *)API_Revision);
}

/**========================================================================**
 * Function     : TDAL_AV_PlatformRevisionGet
 *
 * Description  : Get revision of the Platform (implementation) video driver
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
const char *  TDAL_AV_PlatformRevisionGet(void )
{
    static const char PLTF_Revision[] = "st7109_v1.0";
    return((const char *)PLTF_Revision);
}


/**========================================================================**
 * Function     : TDAL_AV_Init
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error    TDAL_AV_Init( void )
{
	tTDAL_AV_Error error_v, error_a;

	mTBOX_FCT_ENTER("TDAL_AV_Init");

	if (TDAL_AV_AlreadyInitialized == TRUE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	/* Create Task and Queue for AVEvent */
#if 1
	TDAL_AV_EvtQueue_p = MsOS_CreateQueue(
		    NULL,         //It is useless now, can pass NULL.
            NULL,               // queue size (byte unit) : now fixed as 10 * u32MessageSize
            E_MSG_FIXED_SIZE,   // E_MSG_VAR_SIZE has not been supported yet
            sizeof(tTDAL_AV_EvtMsg),     // message size (byte unit) for E_MSG_FIXED_SIZE
            E_MSOS_FIFO,        // E_MSOS_FIFO suspended in FIFO order
            kTDAL_AV_EVT_QUEUE_NAME);      // queue name
#else
	TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(tTDAL_AV_EvtMsg), &TDAL_AV_EvtQueue_p);
#endif
	if (TDAL_AV_EvtQueue_p < 0)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING,"ERROR pointer TDAL_AV_EvtQueue_p = NULL \n"));
		mTBOX_RETURN(eTDAL_AV_ERROR);
	}

	TDAL_AV_EvtTaskStop = FALSE;

	TDAL_CreateMutex(&TDAL_AVi_Mutex);

	if (TDAL_AVi_Mutex == NULL)
	{
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not create AV mutex\n"));
        mTBOX_RETURN(eTDAL_AV_ERROR);
	}


	TDAL_AV_EvtTaskID = MsOS_CreateTask(
					       p_TDAL_AV_EventsTask,
						   0,
						   E_TASK_PRI_MEDIUM,
						   TRUE, /* Task auto start */
						   TDAL_AV_EvtTaskStack,
						   kTDAL_AV_EVT_TASK_STACK_SIZE,
						   kTDAL_AV_EVT_TASK_NAME);

	if (TDAL_AV_EvtTaskID < 0)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "task_init failed for AV Event\n"));
		mTBOX_RETURN(eTDAL_AV_ERROR);
	}

	error_v = p_TDAL_AV_Video_Init();
	error_a = p_TDAL_AV_Audio_Init();

	TDAL_AV_AlreadyInitialized = TRUE;

	/* the function returns eTDAL_AV_NO_ERROR in all case */
	mTBOX_RETURN(eTDAL_AV_NO_ERROR);
}


/**========================================================================**
 * Function     : TDAL_AV_Terminate
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error    TDAL_AV_Terminate( void )
{
	tTDAL_AV_Error error_v, error_a;
	tTDAL_AV_EvtMsg pEvtMsg;
	MS_BOOL bRet;

	mTBOX_FCT_ENTER("TDAL_AV_Terminate");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	TDAL_LockMutex(TDAL_AVi_Mutex);

	error_v = p_TDAL_AV_Video_Term();
	error_a = p_TDAL_AV_Audio_Term();

	/* send exit message to event task */
	TDAL_AV_EvtTaskStop = TRUE;
	TDAL_AV_EvtTaskFinished = FALSE;
	pEvtMsg.Exit = TRUE;
	bRet = MsOS_SendToQueue(TDAL_AV_EvtQueue_p, (MS_U8 *) &pEvtMsg, sizeof(pEvtMsg), MSOS_WAIT_FOREVER);
	//bRet = TDAL_Enqueue(TDAL_AV_EvtQueue_p, &pEvtMsg);
	if (bRet != TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_AV_Terminate] Could not send end of task message\n"));
		mTBOX_RETURN(eTDAL_AV_ERROR);
	}

	mTBOX_TRACE((kTBOX_NIV_1,"AV_Audio_EventsUnregister: Waiting end of event task \n"));
	TDAL_UnlockMutex(TDAL_AVi_Mutex);

	while (TDAL_AV_EvtTaskFinished != TRUE)
	{
		MsOS_DelayTask(100);
		mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_AV_Terminate]  Waiting AV evt thread to finish\n"));
	}
#if 0 // don't suggest use MsOS_DeleteTask, you can just free task by return from task entry
	bRet = MsOS_DeleteTask(TDAL_AV_EvtTaskID);
	if (bRet != TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "AV_Audio_EventUnregister: Delete of event task failed\n"));
		mTBOX_RETURN(eTDAL_AV_ERROR);
	}
#endif	
	TDAL_AV_EvtTaskID = -1;

	bRet = MsOS_DeleteQueue(TDAL_AV_EvtQueue_p);
	//bRet = TDAL_DeleteQueue(TDAL_AV_EvtQueue_p);
	mTBOX_ASSERT(bRet == TRUE);
	TDAL_AV_EvtQueue_p = -1;

	TDAL_AV_AlreadyInitialized = FALSE;



	TDAL_DeleteMutex(TDAL_AVi_Mutex);
	TDAL_AVi_Mutex = NULL;



	/* the function returns eTDAL_AV_NO_ERROR in all case */
	mTBOX_RETURN(eTDAL_AV_NO_ERROR);

}

/******************************************************************************
 * Function Name    : TDAL_AV_CapabilityGet
 * Description      : Describe the module
 * Side effects     :
 * Comment          :

 * Inputs           :
 * Outputs          :
 *****************************************************************************/
tTDAL_AV_Error TDAL_AV_CapabilityGet(tTDAL_AV_Capability *pstCapability)
{
    mTBOX_FCT_ENTER("TDAL_AV_CapabilityGet");

    if(pstCapability != (tTDAL_AV_Capability *) NULL)
    {
        pstCapability->decoderSupported = eTDAL_AV_DECODER_VIDEO_1 | eTDAL_AV_DECODER_AUDIO_1;
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);
    }
    else
    {
        mTBOX_RETURN(eTDAL_AV_BAD_PARAMETER_ERROR);
    }
}

/**========================================================================**
 * Function     : TDAL_AV_Start
 *
 * Description  : Start audio or video decoding
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_Start(tTDAL_AV_Decoder decoder,tTDAL_AV_StreamType StreamType)
{
    tTDAL_AV_Error ErrorCode = eTDAL_AV_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_AV_Start");

    if (TDAL_AV_AlreadyInitialized == FALSE) {
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);
    }

    TDAL_LockMutex(TDAL_AVi_Mutex);

    /* start selected decoder */
    if(decoder == eTDAL_AV_DECODER_VIDEO_1)
    {
#if defined(USE_TDAL_TS_SOURCE_SINK)
        TDAL_AVm_UserSpeedApply(decoder);
#endif /* defined(USE_TDAL_TS_SOURCE_SINK) */
        ErrorCode = p_TDAL_AV_Video_Start(decoder, StreamType, FALSE, FALSE);
    }
    else if(decoder == eTDAL_AV_DECODER_AUDIO_1)
    {
        ErrorCode = p_TDAL_AV_Audio_Start(decoder, StreamType, true);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Start:decoder[0x%x] not handled\n", decoder));
        /*mTBOX_ASSERT(0);*/
        ErrorCode = eTDAL_AV_ERROR;
    }

    TDAL_UnlockMutex(TDAL_AVi_Mutex);

    mTBOX_RETURN(ErrorCode);
}


/**========================================================================**
 * Function     : TDAL_AV_Stop
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error    TDAL_AV_Stop( tTDAL_AV_Decoder decoder )
{
    tTDAL_AV_Error ErrorCode = eTDAL_AV_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_AV_Stop");

    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_AV_Stop] decoder = %d\n", decoder));

    if (TDAL_AV_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_AV_NO_ERROR);
    }

    TDAL_LockMutex(TDAL_AVi_Mutex);

    /* stop selected decoder*/
    if (decoder == eTDAL_AV_DECODER_VIDEO_1)
    {
        ErrorCode = p_TDAL_AV_Video_Stop(decoder, FALSE, FALSE);
    }
    else if (decoder == eTDAL_AV_DECODER_AUDIO_1)
    {
        ErrorCode = p_TDAL_AV_Audio_Stop(decoder);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Stop:decoder[0x%x] not handled\n", decoder));
        /*mTBOX_ASSERT(0);*/
        ErrorCode = eTDAL_AV_ERROR;
    }

    TDAL_UnlockMutex(TDAL_AVi_Mutex);

   mTBOX_RETURN(ErrorCode);
}

/**========================================================================**
 * Function     : TDAL_AV_EventSubscribe
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_EventSubscribe(tTDAL_AV_Decoder decoder,
                             tTDAL_AV_Event event,
                             tTDAL_AV_CallbackProc_t notifyFunction)
{
	tTDAL_AV_Error error;

	mTBOX_FCT_ENTER("TDAL_AV_EventSubscribe");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	TDAL_LockMutex(TDAL_AVi_Mutex);

	if ((decoder == eTDAL_AV_DECODER_VIDEO_1))
	{
		error = p_TDAL_AV_Video_EventSubscribe(decoder, event, notifyFunction);
	}
	else if ((decoder == eTDAL_AV_DECODER_AUDIO_1))
	{
		error = p_TDAL_AV_Audio_EventSubscribe(decoder, event, notifyFunction);
	}
	else
	{
		mTBOX_ASSERT(0);
		error = eTDAL_AV_ERROR;
	}

	TDAL_UnlockMutex(TDAL_AVi_Mutex);

	mTBOX_RETURN(error);
}

/**========================================================================**
 * Function     : TDAL_AV_EventUnsubscribe
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_EventUnsubscribe(tTDAL_AV_Decoder decoder,tTDAL_AV_Event event)
{
	tTDAL_AV_Error error;

	mTBOX_FCT_ENTER("TDAL_AV_EventUnSubscribe");

	if (TDAL_AV_AlreadyInitialized == FALSE)
	{
		mTBOX_RETURN(eTDAL_AV_NO_ERROR);
	}

	TDAL_LockMutex(TDAL_AVi_Mutex);

	if ((decoder == eTDAL_AV_DECODER_VIDEO_1))
	{
		error = p_TDAL_AV_Video_EventUnsubscribe(decoder, event);
	}
	else if ((decoder == eTDAL_AV_DECODER_AUDIO_1))
	{
		error = p_TDAL_AV_Audio_EventUnsubscribe(decoder, event);
	}
	else
	{
		mTBOX_ASSERT(0);
		error = eTDAL_AV_ERROR;
	}

	TDAL_UnlockMutex(TDAL_AVi_Mutex);

	mTBOX_RETURN(error);
}

/**========================================================================**
 * Function     : TDAL_AV_InputStreamSet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_InputStreamSet(tTDAL_AV_Decoder decoder,uint32_t streamHandle)
{
return eTDAL_AV_NO_ERROR;
}

/**========================================================================**
 * Function     : TDAL_AV_SetSynchroMode
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_AV_Error TDAL_AV_SynchroModeSet(tTDAL_AV_Decoder videoDecoder,tTDAL_AV_Decoder audioDecoder,tTDAL_AV_SynchroMode mode)
{
return eTDAL_AV_NO_ERROR;
}

tTDAL_AV_Error TDAL_AV_SpeedSet(tTDAL_AV_Decoder decoder,int32_t speed)
{
return eTDAL_AV_NO_ERROR;
}

#if defined(USE_TDAL_TS_SOURCE_SINK)
tTDAL_AV_Error TDAL_AVm_UserSpeedApply(tTDAL_AV_Decoder decoder)
{
return eTDAL_AV_NO_ERROR;
}
#endif /* defined(USE_TDAL_TS_SOURCE_SINK) */


#if defined(USE_TDAL_TS_SOURCE_SINK)
/***************************************************************************
 * Name        : TDAL_AVm_StcOffsetSet
 * Description : Sets the offset to be added in base value of returned STC
 * Returns     : TRUE if success. FALSE otherwise.
 **************************************************************************/
bool TDAL_AVm_StcOffsetSet(S32 offset)
{
    bool           bRet = TRUE;
    return (bRet);
}

/***************************************************************************
 * Name        : TDAL_AVm_StcGet
 * Description : returns the given STC baseline value plus the time elapsed
 *               since the last update.
 * Returns     : TRUE if success. FALSE otherwise.
 **************************************************************************/
bool TDAL_AVm_StcGet(STCLKRV_ExtendedSTC_t *pStc)
{
    bool           bRet = TRUE;
    return (bRet);
}

/***************************************************************************
 * Name        : TDAL_AVm_StcBaselineSet
 * Description : Loads given STC baseline value to UserBaseline storage area.
 * Returns     : TRUE if success. FALSE otherwise.
 **************************************************************************/
bool TDAL_AVm_StcBaselineSet(STCLKRV_ExtendedSTC_t *pStc)
{
    bool           bRet = TRUE;
    return (bRet);
}

#endif /* defined(USE_TDAL_TS_SOURCE_SINK) */

LOCAL void p_TDAL_AV_EventsTask(MS_U32 argc, void * argv)
{
	tTDAL_AV_EvtMsg pEvtMsg;
	MS_U32 actualSize;
	MS_BOOL bRet;

	mTBOX_FCT_ENTER_T("TDAL_AV_EvtTask");

	while (TDAL_AV_EvtTaskStop == FALSE)
	{
		bRet = MsOS_RecvFromQueue(TDAL_AV_EvtQueue_p, (MS_U8 *) &pEvtMsg, sizeof(pEvtMsg), &actualSize, MSOS_WAIT_FOREVER);
		//bRet = TDAL_Dequeue(TDAL_AV_EvtQueue_p, &pEvtMsg);
		mTBOX_ASSERT(bRet == TRUE ? sizeof(pEvtMsg) == actualSize: FALSE);

		if (pEvtMsg.Exit == FALSE)
		{
			if (pEvtMsg.decoder < eTDAL_AV_DECODER_AUDIO_1)
			{
				p_TDAL_AV_Video_UseEvent(pEvtMsg.decoder, pEvtMsg.Event);
			}
			else
			{
				p_TDAL_AV_Audio_UseEvent(pEvtMsg.decoder, pEvtMsg.Event);
			}

		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,">>>: Task Exit\n"));
			break;;
		}

	}

	TDAL_AV_EvtTaskFinished = TRUE;

	mTBOX_RETURN;
}
