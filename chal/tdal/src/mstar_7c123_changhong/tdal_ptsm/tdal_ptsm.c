/******************************************************************************
 *                     COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PTSM
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ptsm/tdal_ptsm.c $
 *            $Rev: 414 $
 *            $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL Program Time Stamp Matching driver implementation
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "apiDMX.h"

#include "crules.h"
#include "tbox.h"
//#include "pltf_hardcfg.h"
#include "tdal_common.h"
#include "tdal_ptsm.h"

mTBOX_SET_MODULE(eTDAL_PTSM);

/********************************************************/
/*              Macros                                  */
/********************************************************/
#define   ms2stc(x)   (x*90)
#define   stc2ms(x)   (x/90)

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define   kTDAL_PTSM_MAX_STC_VALUE         0xffffffff
#define   kTDAL_PTSM_NB_MAX_CLIENT         3
#define   kTDAL_PTSM_CHECKING_PERIOD         200   /*   ms   */
#define   kTDAL_PTSM_CHECKING_PERIOD_DELTA   2   /*   ms   */

#define   kTDAL_PTSM_SCAN_TASK_NAME         "PTSMCHECKTASK"

/********************************************************/
/*              Typedefs                                */
/********************************************************/
typedef struct tTDAL_PTSM_Comparison_t
{
	tTDAL_PTSM_Handle handle;
	tTDAL_PTSM_ComparisonId comparisonId;
	tTDAL_PTSM_Pts ptsToSignal;
	struct tTDAL_PTSM_Comparison_t *next;
} tTDAL_PTSM_Comparison;

typedef struct
{
	bool handleUsed;
	tTDAL_PTSM_CallbackMatch callbackMatch;
	tTDAL_PTSM_CallbackError callbackError;
} tTDAL_PTSM_Request;

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/
//IMPORT   STCLKRV_Handle_t   SYS_CLKRVHndl;
//IMPORT   ST_DeviceName_t   SYS_ClkrvName;
//IMPORT   ST_DeviceName_t   SYS_EvtName;
/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/LOCAL bool TDAL_PTSM_initialized =
		false;
//LOCAL   STEVT_Handle_t   TDAL_PTSM_EVTHandle;
LOCAL tTDAL_PTSM_Request TDAL_PTSM_RunningRequest[kTDAL_PTSM_NB_MAX_CLIENT];
LOCAL tTDAL_PTSM_Comparison *TDAL_PTSM_LaunchedComparisonArray;
LOCAL TDAL_mutex_id TDAL_PTSM_Mutex;

LOCAL uint8_t TDAL_PTSMi_TaskStack[4096];
volatile LOCAL bool TDAL_PTSMi_TaskRun = FALSE;
volatile LOCAL bool TDAL_PTSMi_TaskFinished;

/*   variable   needed   for   check   task   */
TDAL_task_id      TDAL_PTSM_checkTaskID;
/********************************************************/
/*            Local   Functions   Declarations   (LOCAL)   */
/********************************************************/
LOCAL void TDAL_PTSMi_CheckingTask(
		void*);
LOCAL tTDAL_PTSM_ErrorCode TDAL_PTSMi_Diff_PTS_STC(tTDAL_PTSM_Pts*,
		tTDAL_PTSM_Delay*);
LOCAL tTDAL_PTSM_ErrorCode TDAL_PTSMi_Sort_PTS(tTDAL_PTSM_Handle handle,
		tTDAL_PTSM_ComparisonId comparisonId, tTDAL_PTSM_Pts *pPts);

/*LOCAL   void   TDAL_PTSMi_ClkrvEvtCallback(STEVT_CallReason_t      Reason,
 const   ST_DeviceName_t   RegistrantName,
 STEVT_EventConstant_t   Event,
 const   void            *EventData,
 const   void            *SubscriberData_p);*/

/********************************************************/
/*            Functions   Definitions   (LOCAL/GLOBAL)   */
/********************************************************/

/*===========================================================================
 *
 * TDAL_PTSM_Init
 *
 * Parameters:
 *
 * Description:
 *      initialize the TDAL PTSM module
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Init(void)
{
	int i;

	mTBOX_FCT_ENTER("TDAL_PTSM_Init");

	/*   test   if   TDAL   PTSM   has   already   been   initialized   */
	if (TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL   PTSM   already   initialized\n"));
	}
	else
	{
		for (i = 0; i < kTDAL_PTSM_NB_MAX_CLIENT; i++)
		{
			TDAL_PTSM_RunningRequest[i].handleUsed = false;
			TDAL_PTSM_RunningRequest[i].callbackMatch = NULL;
			TDAL_PTSM_RunningRequest[i].callbackError = NULL;
		}

		/*   init   the   array   of   launched   comparisons   */
		TDAL_PTSM_LaunchedComparisonArray = NULL;

		/*   create   a   mutex   to   protect   access   to   the   PTSM   structure   */
		TDAL_CreateMutex(&TDAL_PTSM_Mutex);

		TDAL_PTSMi_TaskRun = TRUE;
		TDAL_PTSMi_TaskFinished = FALSE;

		/*   ====================   */
		/*   create   the   PTSM   task   */
		/*   ====================   */
		TDAL_PTSM_checkTaskID = TDAL_CreateTask(eTDAL_PRIORITY_NORMAL,
				kTDAL_PTSM_SCAN_TASK_NAME,
				TDAL_PTSMi_TaskStack,
				sizeof(TDAL_PTSMi_TaskStack),
				TDAL_PTSMi_CheckingTask,
				NULL);


		if (TDAL_PTSM_checkTaskID == NULL)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "PTSM   init   failed   because   the   check   scan   task   could   not   start\n"));
			mTBOX_RETURN(eTDAL_PTSM_ERROR);
		}
	}

	TDAL_PTSM_initialized = true;

	mTBOX_RETURN(eTDAL_PTSM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_PTSM_Terminate
 *
 * Parameters:
 *
 * Description:
 *      terminate the TDAL PTSM module
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Terminate(void)
{
	int i;
	tTDAL_PTSM_Comparison *comparison;

	mTBOX_FCT_ENTER("TDAL_PTSM_Terminate");

	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL   PTSM   not   initialized\n"));
	}
	else
	{

		TDAL_LockMutex(TDAL_PTSM_Mutex);

		/*   free   all   the   client   structures   */
		for (i = 0; i < kTDAL_PTSM_NB_MAX_CLIENT; i++)
		{
			if ((TDAL_PTSM_RunningRequest[i].handleUsed == true)
					&& (TDAL_PTSM_RunningRequest[i].callbackError != NULL))
			{
				/*   launch   the   callback_error   to   inform   the   client   */
				TDAL_PTSM_RunningRequest[i].callbackError(
						eTDAL_PTSM_COMPARISON_STOPPED);
			}

			TDAL_PTSM_RunningRequest[i].handleUsed = false;
			TDAL_PTSM_RunningRequest[i].callbackMatch = NULL;
			TDAL_PTSM_RunningRequest[i].callbackError = NULL;
		}

		/*   free   the   array   of   launched   comparisons   */
		while (TDAL_PTSM_LaunchedComparisonArray != NULL)
		{
			comparison = TDAL_PTSM_LaunchedComparisonArray;
			TDAL_PTSM_LaunchedComparisonArray =
					TDAL_PTSM_LaunchedComparisonArray->next;
			TDAL_Free(comparison);
		}

        TDAL_UnlockMutex(TDAL_PTSM_Mutex);

		TDAL_PTSMi_TaskRun = FALSE;

		while (!TDAL_PTSMi_TaskFinished)
		{
			mTBOX_TRACE((kTBOX_NIV_1, "Waiting PTSM task to finish\n"));
			MsOS_DelayTask(100);
		}

		MsOS_DelayTask(100);

		TDAL_DeleteTask(TDAL_PTSM_checkTaskID);
		TDAL_DeleteMutex(TDAL_PTSM_Mutex);
	}

	TDAL_PTSM_initialized = false;

	mTBOX_RETURN(eTDAL_PTSM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_PTSM_GetCurrentStc
 *
 * Parameters:
 *      stc    pointer to a variable used to store the current stc
 *
 * Description:
 *      retreive the current stc
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_GetCurrentStc(tTDAL_PTSM_Pts* pStc)
{
	DMX_FILTER_STATUS dmxStatus;
	MS_U32 stc33, stc;

	mTBOX_FCT_ENTER("TDAL_PTSM_GetCurrentStc");

	/*   test   if   TDAL   PTSM   has   been   initialized   */
	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL   PTSM   not   initialized\n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	/*   test   input   parameter   */
	if (!pStc)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Null   parameter\n"));
		mTBOX_RETURN(eTDAL_PTSM_BAD_PARAMETER);
	}

	dmxStatus = MApi_DMX_Stc_Get(&stc33, &stc);

	if (dmxStatus != DMX_FILTER_STATUS_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_DMX_Stc_Get   error   %x\n", dmxStatus));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	pStc->clock_base_1msb = stc33;
	pStc->clock_base_32lsb = stc;

	mTBOX_RETURN(eTDAL_PTSM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_PTSM_Register
 *
 * Parameters:
 *      handle          pointer to a variable used to store the handle of the client
 *      callbackMatch   function called upon comparison match
 *      callbackError   function called upon error
 *
 * Description:
 *      register a client to Ptsm module
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Register(tTDAL_PTSM_Handle *pHandle,
		tTDAL_PTSM_CallbackMatch callbackMatch,
		tTDAL_PTSM_CallbackError callbackError)
{
	int i;
	tTDAL_PTSM_ErrorCode PTSM_Error = eTDAL_PTSM_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_PTSM_Register");

	/*   test   if   TDAL   PTSM   has   been   initialized   */
	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL   PTSM   not   initialized\n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	/*   test   input   parameter   */
	if ((!pHandle) || (!callbackMatch) || (!callbackError))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Null   parameter\n"));
		mTBOX_RETURN(eTDAL_PTSM_BAD_PARAMETER);
	}

	/*   protect   access   to   PTSM   structure   */
	TDAL_LockMutex(TDAL_PTSM_Mutex);

	for (i = 0; i < kTDAL_PTSM_NB_MAX_CLIENT; i++)
	{
		if (TDAL_PTSM_RunningRequest[i].handleUsed == false)
		{
			*pHandle = i;
			TDAL_PTSM_RunningRequest[i].handleUsed = true;
			TDAL_PTSM_RunningRequest[i].callbackMatch = callbackMatch;
			TDAL_PTSM_RunningRequest[i].callbackError = callbackError;
			break;
		}
	}

	TDAL_UnlockMutex(TDAL_PTSM_Mutex);

	if (i == kTDAL_PTSM_NB_MAX_CLIENT)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No   more   free   handle   for   PTSM\n"));
		PTSM_Error = eTDAL_PTSM_ERROR;
	}

	mTBOX_RETURN(PTSM_Error);
}

/*===========================================================================
 *
 * TDAL_PTSM_UnRegister
 *
 * Parameters:
 *      handle    handle of the client
 *
 * Description:
 *      unregister a client of Ptsm module
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_UnRegister(tTDAL_PTSM_Handle handle)
{
	tTDAL_PTSM_Comparison *comparison = NULL;
	tTDAL_PTSM_Comparison *previousComparison = NULL;

	mTBOX_FCT_ENTER("TDAL_PTSM_UnRegister");

	/*   test   if   TDAL   PTSM   has   been   initialized   */
	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL   PTSM   not   initialized\n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	if (handle >= kTDAL_PTSM_NB_MAX_CLIENT)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad   parameter\n"));
		mTBOX_RETURN(eTDAL_PTSM_BAD_PARAMETER);
	}

	TDAL_LockMutex(TDAL_PTSM_Mutex);

	if (TDAL_PTSM_RunningRequest[handle].handleUsed == false)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Handle   not   used   or   already   free\n"));
	}

	/*   launch   the   callback_error   to   inform   the   client   */
	if (TDAL_PTSM_RunningRequest[handle].callbackError != NULL)
	{
		TDAL_PTSM_RunningRequest[handle].callbackError(
				eTDAL_PTSM_COMPARISON_STOPPED);
	}

	/*   free   the   launched   comparisons   corresponding   to   the   current   handle   */
	comparison = TDAL_PTSM_LaunchedComparisonArray;
	while (comparison != NULL)
	{
		if (handle == comparison->handle)
		{
			/*   we   found   a   comparison   corresponding   to   the   current   handle   */
			if (comparison == TDAL_PTSM_LaunchedComparisonArray)
			{
				/*   item   to   delete   is   the   1rst   one   */
				TDAL_PTSM_LaunchedComparisonArray = comparison->next;
				TDAL_Free(comparison);
				comparison = TDAL_PTSM_LaunchedComparisonArray;
			}
			else
			{
				previousComparison->next = comparison->next;
				TDAL_Free(comparison);
				comparison = comparison->next;
			}
		}
		else
		{
			/*   check   following   comparison   */
			previousComparison = comparison;
			comparison = comparison->next;
		}
	}

	/*   update   the   handle   structure   */
	TDAL_PTSM_RunningRequest[handle].handleUsed = false;
	TDAL_PTSM_RunningRequest[handle].callbackMatch = NULL;
	TDAL_PTSM_RunningRequest[handle].callbackError = NULL;

	TDAL_UnlockMutex(TDAL_PTSM_Mutex);

	mTBOX_RETURN(eTDAL_PTSM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_PTSM_Diff_PTS_STC
 *
 * Parameters:
 *      pts    program time stamp
 *      delay  delay (in ms) until the stc reaches the pts given as parameter
 *
 * Description:
 *      calculate the delay until the stc reaches the pts given as parameter
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Diff_PTS_STC(tTDAL_PTSM_Pts *pPts,
		tTDAL_PTSM_Delay *pDelay)
{
	tTDAL_PTSM_ErrorCode PTSM_Error;

	mTBOX_FCT_ENTER("TDAL_PTSM_Diff_PTS_STC");

	/*   test   if   TDAL   PTSM   has   been   initialized   */
	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL   PTSM   not   initialized\n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	/*   test   input   parameter   */
	if (!pDelay || !pPts)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Null   parameter\n"));
		mTBOX_RETURN(eTDAL_PTSM_BAD_PARAMETER);
	}

	PTSM_Error = TDAL_PTSMi_Diff_PTS_STC(pPts, pDelay);

	mTBOX_RETURN(PTSM_Error);
}

/*===========================================================================
 *
 * TDAL_PTSMi_Diff_PTS_STC
 *
 * Parameters:
 *      pts    program time stamp
 *      delay  delay (in ms) until the stc reaches the pts given as parameter
 *
 * Description:
 *      calculate the delay until the stc reaches the pts given as parameter
 *      (used as internal because we do not test parameters)
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
LOCAL tTDAL_PTSM_ErrorCode TDAL_PTSMi_Diff_PTS_STC(
		tTDAL_PTSM_Pts *pPts, tTDAL_PTSM_Delay *pDelay)
{
	uint32_t stc_45khz, pts_45khz;
	uint32_t uDelay;
	DMX_FILTER_STATUS dmxStatus;
	MS_U32 stc33, stc;

	mTBOX_FCT_ENTER("TDAL_PTSMi_Diff_PTS_STC");

	dmxStatus = MApi_DMX_Stc_Get(&stc33, &stc);
	if (dmxStatus != DMX_FILTER_STATUS_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_DMX_Stc_Get   error \n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	/*   convert   PTS   and   STC   to   unit   of   45kHz   in   order   to   do   calculation   on   32bits   */
	stc_45khz = (uint32_t) ((stc33 << 31) | (stc >> 1));
	pts_45khz = (uint32_t) ((pPts->clock_base_1msb << 31)
			| (pPts->clock_base_32lsb >> 1));

	/*   test   where   is   the   now   (i.e.   stc)   compare   to   the   maxvalue/2   */
	if (stc_45khz < kTDAL_PTSM_MAX_STC_VALUE / 2)
	{
		/*   test   if   PTS   is   in   the   future   (easier   than   testing   the   past)   */
		if ((pts_45khz > stc_45khz)
				&& (pts_45khz < (stc_45khz + kTDAL_PTSM_MAX_STC_VALUE / 2)))
		{
			/*   positive   delay   */
			uDelay = pts_45khz - stc_45khz;
			*pDelay = uDelay;
		}
		else
		{
			/*   negative   delay   */
			if (pts_45khz < stc_45khz)
			{
				uDelay = stc_45khz - pts_45khz;
			}
			else
			{
				uDelay = kTDAL_PTSM_MAX_STC_VALUE - (pts_45khz - stc_45khz);
			}

			*pDelay = -uDelay;
		}
	}
	else
	{
		/*   test   if   PTS   is   in   the   past   (easier   than   testing   the   future)   */
		if ((pts_45khz > stc_45khz - kTDAL_PTSM_MAX_STC_VALUE / 2)
				&& (pts_45khz < stc_45khz))
		{
			/*   negative   delay   */
			uDelay = stc_45khz - pts_45khz;
			*pDelay = -uDelay;
		}
		else
		{
			/*   positive   delay   */
			if (pts_45khz < stc_45khz)
			{
				uDelay = kTDAL_PTSM_MAX_STC_VALUE - (stc_45khz - pts_45khz);
			}
			else
			{
				uDelay = pts_45khz - stc_45khz;
			}

			*pDelay = uDelay;
		}
	}

	*pDelay = stc2ms(((*pDelay)<<1));

	mTBOX_TRACE((kTBOX_NIV_1, "Delay = %d\n", *pDelay));
	mTBOX_RETURN(eTDAL_PTSM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_PTSMi_Sort_PTS
 *
 * Parameters:
 *      pts1, pts2    program time stamp
 *
 * Description:
 *      sort the pts given in the array of the launched comparisons.
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
LOCAL tTDAL_PTSM_ErrorCode TDAL_PTSMi_Sort_PTS(
		tTDAL_PTSM_Handle handle, tTDAL_PTSM_ComparisonId comparisonId,
		tTDAL_PTSM_Pts *pPts)
{
	tTDAL_PTSM_Comparison *comparison = NULL;
	tTDAL_PTSM_Comparison *previousComparison = NULL;
	tTDAL_PTSM_Comparison *comparisonToSort;

	mTBOX_FCT_ENTER("TDAL_PTSMi_Sort_PTS");

	/*   check   if   the   comparison   Id   already   exist   :   if   yes, delete   it   */
	comparison = TDAL_PTSM_LaunchedComparisonArray;
	while (comparison != NULL)
	{
		if ((handle == comparison->handle)
				&& (comparisonId == comparison->comparisonId))
		{
			/*   Id   for   comparison   found   :   free   the   structure   */
			if (comparison == TDAL_PTSM_LaunchedComparisonArray)
				TDAL_PTSM_LaunchedComparisonArray = comparison->next; /*   item   to   delete   is   the   1rst   one   */
			else
				previousComparison->next = comparison->next;

			TDAL_Free(comparison);
			mTBOX_TRACE((kTBOX_NIV_WARNING, "Comparison   Id   (%d)   was   already   associated   to   handle   (%d)\n", comparisonId, handle));
			break;
		}
		previousComparison = comparison;
		comparison = comparison->next;
	}

	/*   allocate   the   item   to   sort   */
	comparisonToSort = TDAL_Malloc(sizeof(tTDAL_PTSM_Comparison));
	comparisonToSort->ptsToSignal.clock_base_32lsb = pPts->clock_base_32lsb;
	comparisonToSort->ptsToSignal.clock_base_1msb = pPts->clock_base_1msb;
	comparisonToSort->handle = handle;
	comparisonToSort->comparisonId = comparisonId;
	comparisonToSort->next = NULL;

	/*   check   the   array   of   registered   PTS   and   sort   the   PTS   given   */
	comparison = TDAL_PTSM_LaunchedComparisonArray;
	while (comparison != NULL)
	{
		if ((pPts->clock_base_1msb < comparison->ptsToSignal.clock_base_1msb)
				|| ((pPts->clock_base_1msb
						== comparison->ptsToSignal.clock_base_1msb)
						&& (pPts->clock_base_32lsb
								<= comparison->ptsToSignal.clock_base_32lsb)))
		{
			/*   pPts   will   fall   earlier   than   the   Pts   sorted   in   the   array   */
			if (comparison == TDAL_PTSM_LaunchedComparisonArray)
			{
				/*   item   must   be   sorted   at   the   first   position   */
				comparisonToSort->next = TDAL_PTSM_LaunchedComparisonArray;
				TDAL_PTSM_LaunchedComparisonArray = comparisonToSort;
			}
			else
			{
				previousComparison->next = comparisonToSort;
				comparisonToSort->next = comparison;
			}
			break;
		}
		else
		{
			/*   pPts   will   fall   later   :   continue   to   scroll   the   array   */
			previousComparison = comparison;
			comparison = comparison->next;
		}
	}

	if (TDAL_PTSM_LaunchedComparisonArray == NULL)
	{
		/*   the   array   is   empty   */
		TDAL_PTSM_LaunchedComparisonArray = comparisonToSort;
	}
	else if (comparison == NULL)
	{
		/*   the   item   must   be   the   last   one   */
		previousComparison->next = comparisonToSort;
	}

	mTBOX_RETURN(eTDAL_PTSM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_PTSM_Start
 *
 * Parameters:
 *      handle        handle of the client
 *      comparisonId  id of the comparison
 *      pts           program time stamp to notify
 *      delay         delay (in ms)
 *
 * Description:
 *      this function is used to start a comparison for a client referenced by
 *      handle. The notification will be sent 'delay' milliseconds before match.
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Start(tTDAL_PTSM_Handle handle,
		tTDAL_PTSM_ComparisonId comparisonId, tTDAL_PTSM_Pts *pPts,
		tTDAL_PTSM_Delay delay)
{
	tTDAL_PTSM_Pts ptsToRegister;
	tTDAL_PTSM_ErrorCode PTSM_Error;

	mTBOX_FCT_ENTER("TDAL_PTSM_Start");
	mTBOX_TRACE((kTBOX_NIV_1, "TDAL_PTSM_Start   :   handle   (%d) - comparisonId   (%d)\n" ,handle ,comparisonId));

	/*   test   if   TDAL   PTSM   has   been   initialized   */
	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL   PTSM   not   initialized\n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	/*   test   input   parameter   */
	if ((!pPts) || (handle >= kTDAL_PTSM_NB_MAX_CLIENT))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad   parameter\n"));
		mTBOX_RETURN(eTDAL_PTSM_BAD_PARAMETER);
	}

	/*   process   the   delay   */
	ptsToRegister.clock_base_1msb = pPts->clock_base_1msb;
	ptsToRegister.clock_base_32lsb = pPts->clock_base_32lsb;
	if (delay >= 0)
	{
		/*   the   notification   must   be   sent   'delay'   milliseconds   before   the   real   match   */
		if (ptsToRegister.clock_base_32lsb < (uint32_t) ms2stc(delay))
		{
			/*   wrap   around   */
			ptsToRegister.clock_base_1msb = 1 - ptsToRegister.clock_base_1msb;
		}

		ptsToRegister.clock_base_32lsb -= ms2stc(delay);
	}
	else
	{
		/*   the   notification   must   be   sent   'delay'   milliseconds   after   the   real   match   */
		if ((uint32_t) (0xFFFFFFFF - ptsToRegister.clock_base_32lsb)
				< (uint32_t) ms2stc(-delay))
		{
			/*   wrap   around   */
			ptsToRegister.clock_base_1msb = 1 - ptsToRegister.clock_base_1msb;
		}

		ptsToRegister.clock_base_32lsb += ms2stc(-delay);
	}

	/*   protect   access   to   PTSM   structure   */
	TDAL_LockMutex(TDAL_PTSM_Mutex);

	if (TDAL_PTSM_RunningRequest[handle].handleUsed == false)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Handle   given   has   not   been   registered\n"));
		PTSM_Error = eTDAL_PTSM_BAD_PARAMETER;
	}
	else
	{
		/*   register   the   requested   pts   */
		TDAL_PTSMi_Sort_PTS(handle, comparisonId, &ptsToRegister);
		PTSM_Error = eTDAL_PTSM_NO_ERROR;
	}

	TDAL_UnlockMutex(TDAL_PTSM_Mutex);

	mTBOX_RETURN(PTSM_Error);
}

/*===========================================================================
 *
 * TDAL_PTSM_Stop
 *
 * Parameters:
 *      handle        handle of the client
 *      comparisonId  id of the comparison
 *
 * Description:
 *      this function is used to stop a comparison.
 *
 * Returns:
 *      eTDAL_PTSM_NO_ERROR       no error
 *      eTDAL_PTSM_BAD_PARAMETER  wrong argument value
 *      eTDAL_PTSM_ERROR          command not done
 *
 *===========================================================================*/
tTDAL_PTSM_ErrorCode TDAL_PTSM_Stop(tTDAL_PTSM_Handle handle,
		tTDAL_PTSM_ComparisonId comparisonId)
{
	bool foundId;
	tTDAL_PTSM_Comparison *comparison = NULL;
	tTDAL_PTSM_Comparison *previousComparison = NULL;
	tTDAL_PTSM_CallbackError callbackErrorToCall = NULL;
	tTDAL_PTSM_ErrorCode PTSM_Error;

	mTBOX_FCT_ENTER("TDAL_PTSM_Stop"); mTBOX_TRACE((kTBOX_NIV_1, "TDAL_PTSM_Stop   :   handle   (%d) - comparisonId   (%d)\n" ,handle ,comparisonId));

	/*   test   if   TDAL   PTSM   has   been   initialized   */
	if (!TDAL_PTSM_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL   PTSM   not   initialized\n"));
		mTBOX_RETURN(eTDAL_PTSM_ERROR);
	}

	/*   test   input   parameter   */
	if (handle >= kTDAL_PTSM_NB_MAX_CLIENT)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad   parameter\n"));
		mTBOX_RETURN(eTDAL_PTSM_BAD_PARAMETER);
	}

	/*   protect   access   to   PTSM   structure   */
	TDAL_LockMutex(TDAL_PTSM_Mutex);

	/*   check   handle   */
	if (TDAL_PTSM_RunningRequest[handle].handleUsed == false)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Handle   given   (%d)   has   not   been   registered\n", handle));
		PTSM_Error = eTDAL_PTSM_BAD_PARAMETER;
	}
	else
	{
		PTSM_Error = eTDAL_PTSM_NO_ERROR;

		/*   keep   a   pointer   on   the   callback   before   unprotecting   the   PTSM   structure   */
		callbackErrorToCall = TDAL_PTSM_RunningRequest[handle].callbackError;

		/*   free   the   ressources   allocated   for   the   comparison   Id   */
		comparison = TDAL_PTSM_LaunchedComparisonArray;
		foundId = false;

		while (comparison != NULL)
		{
			if ((handle == comparison->handle)
					&& (comparisonId == comparison->comparisonId))
			{
				/*   Id   for   comparison   found   :   free   the   structure   */
				if (comparison == TDAL_PTSM_LaunchedComparisonArray)
					TDAL_PTSM_LaunchedComparisonArray = comparison->next; /*   item   to   delete   is   the   1rst   one   */
				else
					previousComparison->next = comparison->next;

				TDAL_Free(comparison);
				foundId = true;
				break;
			}
			previousComparison = comparison;
			comparison = comparison->next;
		}
		if (foundId == false)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Comparison   Id   (%d)   not   associated   to   handle   given   (%d)\n", comparisonId, handle));
			PTSM_Error = eTDAL_PTSM_BAD_PARAMETER;
		}
	}

	TDAL_UnlockMutex(TDAL_PTSM_Mutex);

	/*   launch   the   PTSM   callback   */
	if (PTSM_Error == eTDAL_PTSM_NO_ERROR)
	{
		/*   check   the   associated   callback   */
		if (callbackErrorToCall == NULL)
		{
			mTBOX_TRACE((kTBOX_NIV_WARNING, "No   PTSM   callback_error   for   handle   %d   !!!\n", handle));
			PTSM_Error = eTDAL_PTSM_ERROR;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1, "Launch   PTSM   callback_error\n"));
			callbackErrorToCall(eTDAL_PTSM_COMPARISON_STOPPED);
		}
	}

	mTBOX_RETURN(PTSM_Error);
}

/*===========================================================================
 *
 * TDAL_PTSMi_CheckingTask
 *
 * Parameters:
 *      none
 *
 * Description:
 *      this task checks if current time matches with a previous request
 *
 * Returns:
 *      none
 *
 *===========================================================================*/
LOCAL void TDAL_PTSMi_CheckingTask(
		void *arg)
{
	bool analyseNextItem;
	tTDAL_PTSM_Delay Delay;
	tTDAL_PTSM_Comparison *comparison;
	MS_U32 periodToWait;
#if   0
	tTDAL_PTSM_Pts LastPtsSignaled;
#endif

	mTBOX_FCT_ENTER_T("TDAL_PTSMi_CheckingTask");

	TDAL_PTSMi_TaskFinished = FALSE;

/*	periodToWait = ST_GetClocksPerSecondLow();
	periodToWait *= (kTDAL_PTSM_CHECKING_PERIOD
			- kTDAL_PTSM_CHECKING_PERIOD_DELTA);
	periodToWait /= 1000;*/
	periodToWait = (kTDAL_PTSM_CHECKING_PERIOD
			- kTDAL_PTSM_CHECKING_PERIOD_DELTA);
#if   0
	LastPtsSignaled.clock_base_32lsb = 0;
	LastPtsSignaled.clock_base_1msb = 0;
#endif

	while (TDAL_PTSMi_TaskRun)
	{
		/*   wait   for   a   period   (delta   is   the   tolerance   granted   for   time   calculation)   */
		//STOS_TaskDelay(periodToWait);
		MsOS_DelayTask(periodToWait);

		/*   protect   access   to   PTSM   structure   */
		TDAL_LockMutex(TDAL_PTSM_Mutex);

		/*   check   all   comparison   Id   requests   :   the   1rst   to   fall   is   the   1rst   in   "TDAL_PTSM_LaunchedComparisonArray"   */
		analyseNextItem = true;

		/*   ============================================================   */
		/*   Analyse   the   1rst   item   from   the   array   of   launched   comparisons   */
		/*   ============================================================   */
		while ((TDAL_PTSM_LaunchedComparisonArray != NULL)
				&& (analyseNextItem == true))
		{
			if (TDAL_PTSMi_Diff_PTS_STC(
					&TDAL_PTSM_LaunchedComparisonArray->ptsToSignal, &Delay)
					== eTDAL_PTSM_NO_ERROR)
			{
				/*   =========================   */
				/*   check   the   time   difference   */
				/*   =========================   */
				if (Delay > (kTDAL_PTSM_CHECKING_PERIOD / 2))
				{
					/*   remaining   delay   is   too   long   in   the   future   */
					analyseNextItem = false;
				}
				else
				{
					/*   time   difference   is   good   */
					mTBOX_TRACE((kTBOX_NIV_1, "Found   a   time   request   item   which   match\n"));
					analyseNextItem = true;

					/*   check   handle   */
					if ((TDAL_PTSM_LaunchedComparisonArray->handle
							>= kTDAL_PTSM_NB_MAX_CLIENT)
							|| (TDAL_PTSM_RunningRequest[TDAL_PTSM_LaunchedComparisonArray->handle].handleUsed
									== false))
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Handle   no   more   used\n"));
					}
					else if (TDAL_PTSM_RunningRequest[TDAL_PTSM_LaunchedComparisonArray->handle].callbackMatch
							!= NULL)
					{
#if   0
						/*   the   pts   has   to   be   longer   than   the   last   signaled   */
						if ( (LastPtsSignaled.clock_base_1msb < TDAL_PTSM_LaunchedComparisonArray->ptsToSignal.clock_base_1msb) ||
								( (LastPtsSignaled.clock_base_1msb == TDAL_PTSM_LaunchedComparisonArray->ptsToSignal.clock_base_1msb) &&
										(LastPtsSignaled.clock_base_32lsb <= TDAL_PTSM_LaunchedComparisonArray->ptsToSignal.clock_base_32lsb) ) )
						{
							/*   update   the   variable   */
							LastPtsSignaled.clock_base_32lsb = TDAL_PTSM_LaunchedComparisonArray->ptsToSignal.clock_base_32lsb;
							LastPtsSignaled.clock_base_1msb = TDAL_PTSM_LaunchedComparisonArray->ptsToSignal.clock_base_1msb;

							/*   launch   the   PTSM   associated   callback   */
							TDAL_PTSM_RunningRequest[TDAL_PTSM_LaunchedComparisonArray->handle].callbackMatch( TDAL_PTSM_LaunchedComparisonArray->comparisonId );
						}
						else
						{
							/*   the   pts   is   ignored   :   it   should   has   been   displayed   before   */
							mTBOX_TRACE((kTBOX_NIV_WARNING, "One   of   the   PTS   is   ignored   (because   processed   too   late)\n"));
						}
#else
						/*   launch   the   PTSM   associated   callback   */
						TDAL_PTSM_RunningRequest[TDAL_PTSM_LaunchedComparisonArray->handle].callbackMatch(
								TDAL_PTSM_LaunchedComparisonArray->comparisonId);
#endif
					}
				}
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "An   error   occured   while   checking   PTS   in   the   task\n"));

				/*   Error   while   checking   the   PTS   :   remove   the   corrupted   data   and   check   following   item   */
				analyseNextItem = true;
			}

			/*   ==============================================   */
			/*   delete   the   first   item   if   it   has   been   processed   */
			/*   ==============================================   */
			if (analyseNextItem == true)
			{
				/*   free   the   ressources   allocated   for   the   comparison   Id   */
				comparison = TDAL_PTSM_LaunchedComparisonArray;
				TDAL_PTSM_LaunchedComparisonArray =
						TDAL_PTSM_LaunchedComparisonArray->next;
				TDAL_Free(comparison);
			}
		} /*   end   while   ((TDAL_PTSM_LaunchedComparisonArray   !=   NULL)...   */

		TDAL_UnlockMutex(TDAL_PTSM_Mutex);
	} /*   end   while   (1)   */

	mTBOX_TRACE((kTBOX_NIV_WARNING, "Exit   from   PTSM   task\n"));

	TDAL_PTSMi_TaskFinished = TRUE;

	mTBOX_RETURN;
}

/****************************************************************************
 Name         : TDAL_DMXi_ClkrvEvtCallback()

 Description  : Callback function for STCLKRV events

 Parameters   : STEVT_CallReason_t Reason
 ST_DeviceName_t RegistrantName : Device event instance ident
 STEVT_EventConstant_t Event : Event ident
 void *EventData
 void *SubscriberData_p

 Return Value : None
 ****************************************************************************/
// LOCAL void TDAL_PTSMi_ClkrvEvtCallback(
//		STEVT_CallReason_t Reason, const ST_DeviceName_t RegistrantName,
//		STEVT_EventConstant_t Event, const void *EventData,
//		const void *SubscriberData_p)
//{
//	int i;
//
//	/*   This   callback   is   called   from   a   STCLKRV   internal   task   context   */
//
//	/*   Only   one   instance, so   should   be   true   alays...   */
//	if (strcmp(RegistrantName, SYS_ClkrvName) == 0)
//	{
//		/*   protect   access   to   PTSM   structure   */
//		TDAL_LockMutex(TDAL_PTSM_Mutex);
//
//		/*   free   all   the   client   structures   */
//		for (i = 0; i < kTDAL_PTSM_NB_MAX_CLIENT; i++)
//		{
//			if ((TDAL_PTSM_RunningRequest[i].handleUsed == true)
//					&& (TDAL_PTSM_RunningRequest[i].callbackError != NULL))
//			{
//				switch (Event)
//				{
//				case STCLKRV_PCR_VALID_EVT:
//					break;
//				case STCLKRV_PCR_DISCONTINUITY_EVT:
//					/*   launch   the   callback_error   to   inform   the   client   */
//					TDAL_PTSM_RunningRequest[i].callbackError(
//							eTDAL_PTSM_PCR_DISCONTINUITY);
//					break;
//				case STCLKRV_PCR_GLITCH_EVT:
//					break;
//				default:
//					break;
//				}
//			}
//		}
//		/*   unprotect   access   to   PTSM   structure   */
//		TDAL_UnlockMutex(TDAL_PTSM_Mutex);
//	}
//
//}
