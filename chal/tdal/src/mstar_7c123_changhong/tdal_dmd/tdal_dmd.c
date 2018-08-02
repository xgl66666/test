/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMD
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_dmd/tdal_dmd.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DMD implementation
 *
 *****************************************************************************/

/*   Possible   Compilation   Flag :
 ===========================


 MEDIUM_SAT
 MEDIUM_CAB
 MEDIUM_TER

 BOARD_TER_DTT5118
 TUNER_SAT_288_NIM

 BOOTLOADER

 SELECT_DEVICE_AMBER
 BOARD_TER_DTT5118
 BOARD_TER_IDTV5105

 STTUNER_USE_SAT
 STTUNER_USE_CAB
 STTUNER_USE_TER
 STTUNER_USE_ATSC_VSB
 */

/********************************************************/
/*      Includes            */
/********************************************************/

#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tdal_common.h"

#include "tdal_dmd.h"
#include "tdal_dmd_priv.h"

#include "tbox.h"

#include "MsCommon.h"
#include "demod/drvDemod.h"
#include "drvTuner.h"
#include "apiDigiTuner.h"
//#include "drvIIC.h"

/********************************************************/
/*      Defines               */
/********************************************************/
#define TDAL_DMD_TASK_STACK         (4096 * 2)
#define CHSCAN_TASK_DELAY           30 // ms
/********************************************************/
/*      Macros               */
/********************************************************/

mTBOX_SET_MODULE(eTDAL_DMD);

/********************************************************/
/*      Typedefs            */
/********************************************************/

/********************************************************/
/*      Global   Variables   (GLOBAL/IMPORT)      */
/********************************************************/

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
LOCAL bool TDAL_DMDi_bFirstInitDone = false;
LOCAL bool TDAL_DMDi_bFirstTuneDone = false;
LOCAL tTDAL_DMD_Front_End TDAL_DMDi_pstFE[kTDAL_DMD_MAX_FRONT_END];
LOCAL tTDAL_DMD_TerSourceState TDAL_DMDi_SourceState =
		eTDAL_DMD_CFG_SOURCE_IS_DTV;
LOCAL   int TDAL_DMDi_ConvertElement   (char*   ElementName,
            int   value,
            bool   FromMSTAR);
LOCAL void TDAL_DMD_TuneTask(void *argv);
LOCAL tTDAL_DMD_Error TDAL_DMDi_TestTuneParameters(unsigned char ucFeID,
		tTDAL_DMD_TunData *psTunData, unsigned int uiFrequency);
LOCAL tTDAL_DMD_Error TDAL_DMDi_TestScanParameters(unsigned   char      ucFeID,
            tTDAL_DMD_ScanData      *psScanData);
LOCAL   tTDAL_DMD_Error TDAL_DMDi_TestConfigParameters   (tTDAL_DMD_Type   eType,
            tTDAL_DMD_ConfigData   *psCfgData);
LOCAL void TDAL_DMDi_NotifyStatus(tTDAL_DMD_FE ucFrontEnd, tTDAL_DMD_NotifyStatus status);

#ifdef   MEDIUM_CAB

LOCAL   tTDAL_DMD_Error   TDAL_DMDi_QAM_Config_Init(unsigned   char   ucFeID,
                     tTDAL_DMD_CabInitData   *psData);

/*LOCAL   tTDAL_DMD_Error   TDAL_DMDi_QAM_SetScanList(unsigned   char   ucFeID,
                     tTDAL_DMD_TunData   *pData);*/

LOCAL   bool      TDAL_DMDi_QAM_Tune   (unsigned   char   ucFeID,
                   tTDAL_DMD_TunData   *psTunData,
                   unsigned   int   TunFrequency);

LOCAL   bool      TDAL_DMDi_QAM_Scan(unsigned   char   ucFeID,
                   tTDAL_DMD_ScanData   *psScanData,
                   unsigned   int   uiStartFreq,
                   unsigned   int   uiEndFreq);
#endif



/**========================================================================**
 * Function    : TDAL_DMD_Init
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DMD_Error TDAL_DMD_Init(void)
{
	tTDAL_DMD_Error Error = eTDAL_DMD_NO_ERROR;
	uint8_t ucIndex;

	mTBOX_FCT_ENTER("TDAL_DMD_Init");
	/*   set   each   structure   of   front   end   id   */
	memset(&TDAL_DMDi_pstFE[0], 0x0, sizeof(tTDAL_DMD_Front_End));
	//memset(&_tunerParam, 0x00, sizeof(_tunerParam));

	/*   First   Call   Only   _   Init   the   Global   Front   End   management   structure*/
	if (TDAL_DMDi_bFirstInitDone == false)
	{
		/*   Reset   global   structure   table   */
		for (ucIndex = 0; ucIndex < kTDAL_DMD_MAX_FRONT_END; ucIndex++)
		{
			/*   Init   Front   End   Global   Variable   */
			TDAL_DMDi_pstFE[ucIndex].bFrontEndUsed = false;
			TDAL_DMDi_pstFE[ucIndex].pNotifyStatusFct = NULL;
			TDAL_DMDi_pstFE[ucIndex].pNotifyProgressFct = NULL;
			TDAL_DMDi_pstFE[ucIndex].FrontEndTask = NULL;
			TDAL_DMDi_pstFE[ucIndex].FrontEndMngtMsgQ = -1;
			TDAL_DMDi_pstFE[ucIndex].FrontEndTaskArgs.TaskFinished = FALSE;

			//TDAL_DMDi_pstFE[ucIndex].pstFrontEndMngtTaskID   = NULL;
			TDAL_DMDi_pstFE[ucIndex].pFrontEndMngtTaskStack = NULL;
			//TDAL_DMDi_pstFE[ucIndex].pstFrontEndMngtTaskDesc   = NULL;

			TDAL_DMDi_pstFE[ucIndex].bConfigInitDone = false;
			TDAL_DMDi_pstFE[ucIndex].bScanInProgress = false;
#ifdef   MEDIUM_SAT
			TDAL_DMDi_pstFE[ucIndex].bSatConfigOLBandDone = false;
			TDAL_DMDi_pstFE[ucIndex].eType = eTDAL_DMD_TYPE_QPSK;
			TDAL_DMDi_pstFE[ucIndex].eSourceSupport = eTDAL_DMD_DIGITAL;
#endif
			//strcpy(TDAL_DMDi_pstFE[ucIndex].STname, SYS_TunerName[ucIndex]);

#ifdef   MEDIUM_TER
			/*   specific   context   7109   ter   */
			TDAL_DMDi_pstFE[ucIndex].eType = eTDAL_DMD_TYPE_OFDM;
			TDAL_DMDi_pstFE[ucIndex].eSourceSupport = eTDAL_DMD_HYBRID;
#endif
#ifdef   MEDIUM_CAB
			TDAL_DMDi_pstFE[ucIndex].eType = eTDAL_DMD_TYPE_QAM;
			TDAL_DMDi_pstFE[ucIndex].eSourceSupport = eTDAL_DMD_DIGITAL;
#endif
		}

		MApi_Frontend_SetBroadcastType(0,/*DVBC*/1);

		if (MApi_DigiTuner_Init(0) == FALSE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Init driver failed.\n"));
			mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
		}
		MApi_DigiTuner_Loop_Through_On(0, TRUE);

		mTBOX_TRACE((kTBOX_NIV_1, "before MApi_DigiTuner_PowerOnOff \n"));
		if (MApi_DigiTuner_PowerOnOff(0, FALSE) == FALSE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Tuner power off failed.\n"));
			mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
		}

		TDAL_DMDi_bFirstInitDone = true;
	}

	mTBOX_RETURN(Error);
}

/**========================================================================**
 * Function    : TDAL_DMD_Terminate
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DMD_Error TDAL_DMD_Terminate(void)
{
	tTDAL_DMD_Error Error = eTDAL_DMD_NO_ERROR;
	uint8_t ucIndex;

	mTBOX_FCT_ENTER("TDAL_DMD_Terminate");

	for (ucIndex = 0; ucIndex < kTDAL_DMD_MAX_FRONT_END; ucIndex++)
	{
		if (TDAL_DMDi_pstFE[ucIndex].bFrontEndUsed == true)
		{
			mTBOX_TRACE((kTBOX_NIV_WARNING,"[TDAL_DMD_Terminate]   Closing front end   ucFeId = %d\n",ucIndex));
			TDAL_DMD_CloseFEInstance(ucIndex);
		}
	}

	TDAL_DMDi_bFirstInitDone = false;

	mTBOX_RETURN(Error);
}

/**========================================================================**
 * Function    : TDAL_DMD_GetCapability
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_DMD_Error TDAL_DMD_GetCapability(tTDAL_DMD_Capability *pstCapability)
{
	tTDAL_DMD_Error Error = eTDAL_DMD_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_DMD_GetCapability");

	if (pstCapability == NULL)
	{
		Error = eTDAL_DMD_BAD_PARAMETER;
	}

	if (Error == eTDAL_DMD_NO_ERROR)
	{
		pstCapability->uiFENumber = 1;
	}
	/*   only   one   tuner   */
	mTBOX_RETURN(Error);
}

/**========================================================================**
 *   Function  :   TDAL_DMD_GetFECapability
 *
 *   Description :
 *
 *   Side   effects :
 *
 *   Comment  :
 *
 **========================================================================**/
tTDAL_DMD_Error TDAL_DMD_GetFECapability(tTDAL_DMD_FE eFeID,
		tTDAL_DMD_FECapability *pstFECapability)

{
	tTDAL_DMD_Error Error = eTDAL_DMD_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_DMD_GetCapability");

	if (eFeID != eTDAL_DMD_FE_0)
	{
		Error = eTDAL_DMD_BAD_PARAMETER;
	}

	if (Error == eTDAL_DMD_NO_ERROR)
	{
		pstFECapability->eType = TDAL_DMDi_pstFE[eFeID].eType;
		pstFECapability->eSourceSupport = TDAL_DMDi_pstFE[eFeID].eSourceSupport;
	}

	mTBOX_RETURN(Error);
}
/**========================================================================**
 * Function    : TDAL_DMD_APIRevisionGet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
const char *TDAL_DMD_APIRevisionGet(void)
{
	static const char API_Revision[] = "TDAL_DMD_v1.0";

	mTBOX_FCT_ENTER("TDAL_DMD_APIRevisionGet");

	mTBOX_RETURN((const char *) API_Revision);

}

/**========================================================================**
 * Function    : TDAL_DMD_PlatformRevisionGet
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
const char *TDAL_DMD_PlatformRevisionGet(void)
{
	static const char PLTF_Revision[] = "STFAEA16_7109";

	mTBOX_FCT_ENTER("TDAL_DMD_PlatformRevisionGet");

	mTBOX_RETURN((const char *) PLTF_Revision);
}

/******************************************************************************
 * Function Name : TDAL_DMD_OpenFEInstance
 *
 * Description   : This function initialises and opens the demod module & opens the TDAL
 *             front-end.
 *
 * Side effects  : Return eTDAL_DMD_NO_ERROR if DMD correctly initialised
 *
 * Comment       : Opens a front-end instance and return an ID used in
 *             others TDAL functions
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_OpenFEInstance(tTDAL_DMD_FE eFeID,
		tTDAL_DMD_NotifyStatusFct pNotifyStatusFct,
		tTDAL_DMD_NotifyProgressFct pNotifyProgressFct)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	MS_BOOL mutexLocked = FALSE;

	mTBOX_FCT_ENTER("TDAL_DMD_OpenFEInstance");
	mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_OpenFEInstance]   Opening FE = %d\n", eFeID));

	/*   Check   Primary   Error   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		if ((eFeID > eTDAL_DMD_FE_0) || (pNotifyStatusFct == NULL) || (pNotifyProgressFct == NULL))
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_OpenFEInstance]   Bad   parameter\n"));
			eError = eTDAL_DMD_BAD_PARAMETER;
		}
	}

	/*   check   front   end   availability   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		if (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_OpenFEInstance]   No   more   Front-End   available\n"));
			eError = eTDAL_DMD_BAD_PARAMETER;
		}
		else
		{
			TDAL_DMDi_pstFE[eFeID].bFrontEndUsed = true;
		}
	}

//#if (FRONTEND_DEMOD_TYPE == DEMOD_MSB122x)
//	if (eError == eTDAL_DMD_NO_ERROR && MDrv_Demod_SetTsSerial(!TS_PARALLEL_OUTPUT) == FALSE)
//	{
//		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_OpenFEInstance] [MDrv_Demod_SetTsSerial]   fail\n"));
//		eError = eTDAL_DMD_BAD_DEMODULATOR;
//	}
//#endif

	if (eError == eTDAL_DMD_NO_ERROR)
	{
		if (MApi_DigiTuner_PowerOnOff(0, TRUE) == FALSE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_OpenFEInstance]  MApi_DigiTuner_PowerOnOff fail\n"));
			eError = eTDAL_DMD_BAD_DEMODULATOR;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_OpenFEInstance]   MApi_DigiTuner_PowerOnOff ok \n"));
		}
	}

	/*   Get   ST   Tuner   Capability   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		/*   Get   Capabilities   from   STTUNER   to   retrieve   AGC   Max   and   Quality   Max
		 *   Values   used   in   TDAL_DMD_GetInfo()   function   */
		/* Need to set tuner capabilities */
//	      eStErrorCode = STTUNER_GetCapability(TDAL_DMDi_pstFE[eFeID].STname,
//	                   &stStCapability);
//	      if   (eStErrorCode != ST_NO_ERROR)
//	      {
//	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Init]   Error   returned   by   STTUNER_GetCapability(-)   %s\n",
//	            TBOX_GetDriverErrorText(eStErrorCode)));
//	      eError = eTDAL_DMD_BAD_DEMODULATOR;
//	      }
//	      else
//	      {
//
//	      TDAL_DMDi_pstFE[eFeID].ulValueAgcMax   = 6000;
	      TDAL_DMDi_pstFE[eFeID].ulValueQualityMax = 100;
//	      TDAL_DMDi_pstFE[eFeID].ulValueBerMax   = stStCapability.BerMax;
//
//	      mTBOX_TRACE((kTBOX_NIV_1,
//	        "[TDAL_DMD_Init]   ST   tuner   MAX   values :   SignalLevel = %d   _   SignalQuality = %d   _   BER = %d\n",
//	            TDAL_DMDi_pstFE[eFeID].ulValueAgcMax   ,
//	            TDAL_DMDi_pstFE[eFeID].ulValueQualityMax   ,
//	            TDAL_DMDi_pstFE[eFeID].ulValueBerMax   ));
//	      }
	}

	/*   Create   OS   ressources   associated   to   this   front   end   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		/*   Create   Front   End   Management   Mutex   */
		TDAL_CreateMutex(&TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

		/*   Lock   mutex   to   block   the   task   */
		mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);
		mutexLocked = TRUE;

		char queueName[50];
		sprintf(queueName, "DMD Queue %d", eFeID);
		TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ = MsOS_CreateQueue(NULL, //It is useless now, can pass NULL.
				NULL, // queue size (byte unit) : now fixed as 10 * u32MessageSize
				E_MSG_FIXED_SIZE,   // E_MSG_VAR_SIZE has not been supported yet
				sizeof(tTDAL_DMD_FrontEndMsgQStruct), // message size (byte unit) for E_MSG_FIXED_SIZE
				E_MSOS_FIFO,        // E_MSOS_FIFO suspended in FIFO order
				queueName);      // queue name

		if (TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ < 0)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMD_OpenFEInstance]:   Unable   to   create   MsgQueue   for   Front   End   mngt\n"));
			eError = eTDAL_DMD_BAD_DEMODULATOR;
		}
	}

	if (eError == eTDAL_DMD_NO_ERROR)
	{
		TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack = TDAL_Malloc(TDAL_DMD_TASK_STACK);

		if (TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack == NULL)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMD_OpenFEInstance] Could not allocated memory for DMD task stack\n"));
			eError = eTDAL_DMD_BAD_DEMODULATOR;
		}
	}

	/*   Check   if   init   succeed   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		TDAL_DMDi_pstFE[eFeID].pNotifyStatusFct = pNotifyStatusFct;
		TDAL_DMDi_pstFE[eFeID].pNotifyProgressFct = pNotifyProgressFct;
		TDAL_DMDi_pstFE[eFeID].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NONE;
		TDAL_DMDi_pstFE[eFeID].frontEnd = eFeID;

		/*   Set   the   tune   &   scan   function   in   accordance   with   the   type   of   front   end   */
		switch (TDAL_DMDi_pstFE[eFeID].eType)
		{
#ifdef   MEDIUM_SAT
		case eTDAL_DMD_TYPE_QPSK:
		TDAL_DMDi_pstFE[eFeID].pTuneFunction = TDAL_DMDi_QPSK_Tune;
		TDAL_DMDi_pstFE[eFeID].pScanFunction = TDAL_DMDi_QPSK_Scan;
		break;
#endif

#ifdef   MEDIUM_CAB
		case eTDAL_DMD_TYPE_QAM:
		TDAL_DMDi_pstFE[eFeID].pTuneFunction = TDAL_DMDi_QAM_Tune;
		TDAL_DMDi_pstFE[eFeID].pScanFunction = TDAL_DMDi_QAM_Scan;
		break;
#endif

#ifdef   MEDIUM_TER
		case eTDAL_DMD_TYPE_OFDM:
		TDAL_DMDi_pstFE[eFeID].pTuneFunction = (tTDAL_DMD_TuneFunction) TDAL_DMDi_OFDM_Tune;
		TDAL_DMDi_pstFE[eFeID].pScanFunction = (tTDAL_DMD_ScanFunction) TDAL_DMDi_OFDM_Scan;
		break;
#endif
		default:
			mTBOX_ASSERT(FALSE);
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMD_OpenFEInstance]  Code should not reach this\n"));
			eError = eTDAL_DMD_BAD_PARAMETER;
			break;
		}
	}

	if (eError == eTDAL_DMD_NO_ERROR)
	{
		char taskName[50];
		sprintf(taskName, "%s_%d", kTDAL_DMD_FRONT_END_TASK_NAME, eFeID);
		TDAL_DMDi_pstFE[eFeID].FrontEndTask = TDAL_CreateTask(
				eTDAL_PRIORITY_NORMAL,
				taskName,
				TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack,
				TDAL_DMD_TASK_STACK,
				TDAL_DMD_TuneTask, &(TDAL_DMDi_pstFE[eFeID].frontEnd));
		if (TDAL_DMDi_pstFE[eFeID].FrontEndTask == NULL)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMD_OpenFEInstance] Creating DMD task failed\n"));
			eError = eTDAL_DMD_BAD_DEMODULATOR;
		}
	}

	/* Stop everything because of error */
	if (eError == eTDAL_DMD_BAD_DEMODULATOR)
	{
		TDAL_DMDi_pstFE[eFeID].bFrontEndUsed = false;

		/*   Free   memory   and   OS   ressource   if   need   */

		if (TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ >= 0)
		{
			MsOS_DeleteQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ);
			TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ = -1;
		}

		if (TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack != NULL)
		{
			TDAL_Free(TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack);
			TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack = NULL;
		}

		//if (MDrv_Demod_Close() != TRUE)
		//{
		//	mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_OpenFEInstance]  Error   returned   by   MDrv_Demod_Close(-) \n"));
		//}
	}

	/*   UnLock   mutex   to   start   the   task   */
	if (mutexLocked)
	{
		mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);
	}

	mTBOX_RETURN(eError);

}

/******************************************************************************
 * Function Name : TDAL_DMD_CloseFEInstance
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_CloseFEInstance(tTDAL_DMD_FE eFeID)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	tTDAL_DMD_FrontEndMsgQStruct pstFrontEndMsg;
	MS_BOOL queueResult, bRet;

	mTBOX_FCT_ENTER("TDAL_DMD_CloseFEInstance");
	mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_CloseFEInstance]   Closing FE = %d\n", eFeID));

	/*   Check   Primary   Error   */
	if (eFeID > eTDAL_DMD_FE_0)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_CloseFEInstance]   Check   primary   error   failed   for   ucFeId = %d\n",
						eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
	}

	if (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_CloseFEInstance]   Check   primary   error   failed   for   ucFeId = %d\n",
						eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}

	/*   Send   a   end_of_task   message   to   frond   end   task   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{

		pstFrontEndMsg.eTypeOfEvent = eTDAL_DMD_FRONT_END_EVENT_TYPE_END_OF_TASK;

		queueResult = MsOS_SendToQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ, (MS_U8*)&pstFrontEndMsg, sizeof(pstFrontEndMsg), MSOS_WAIT_FOREVER);

		if (queueResult != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_CloseFEInstance]   Unable   to   send  eTDAL_DMD_FRONT_END_EVENT_TYPE_END_OF_TASK for   ucFeId = %d\n",
							eFeID));
			eError = eTDAL_DMD_DEVICE_BUSY;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_CloseFEInstance]   Sent   a   eTDAL_DMD_FRONT_END_EVENT_TYPE_END_OF_TASK   to   Front   End   Task   %d\n",
						eFeID));
		}
	}

	if (eError == eTDAL_DMD_NO_ERROR)
	{
		while (TDAL_DMDi_pstFE[eFeID].FrontEndTaskArgs.TaskFinished != TRUE)
		{
			MsOS_DelayTask(50);
		}
	}

	/* Delay to make sure thread has finished */
	MsOS_DelayTask(10);

	mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_CloseFEInstance]   All threads finished\n"));

	if (eError == eTDAL_DMD_NO_ERROR)

		mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_CloseFEInstance]   Deallocating resources \n"));
	{
		/*   Free   memory   and   OS   ressource   if   need   */
		if (TDAL_DMDi_pstFE[eFeID].FrontEndTask != NULL)
		{
			TDAL_DeleteTask(TDAL_DMDi_pstFE[eFeID].FrontEndTask);
			TDAL_DMDi_pstFE[eFeID].FrontEndTask = NULL;
		}

		if (TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ >= 0)
		{
			MsOS_DeleteQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ);
			TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ = -1;
		}

		if (TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack != NULL)
		{
			TDAL_Free(TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack);
			TDAL_DMDi_pstFE[eFeID].pFrontEndMngtTaskStack = NULL;
		}

		if (MApi_DigiTuner_PowerOnOff(0, FALSE) != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_CloseFEInstance]   Error   returned   by   MApi_DigiTuner_PowerOnOff(FALSE) \n"));
			eError = eTDAL_DMD_BAD_DEMODULATOR;
		}
	}

	/*   Init   front   end   structure   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		/*   Init   Front   End   Global   Variable   */
		TDAL_DMDi_pstFE[eFeID].bFrontEndUsed = false;
		TDAL_DMDi_pstFE[eFeID].pNotifyStatusFct = NULL;
		TDAL_DMDi_pstFE[eFeID].pNotifyProgressFct = NULL;

		TDAL_DMDi_pstFE[eFeID].bConfigInitDone = false;
		TDAL_DMDi_pstFE[eFeID].bScanInProgress = false;
		TDAL_DMDi_pstFE[eFeID].frontEnd = 0;
#ifdef   MEDIUM_SAT
		TDAL_DMDi_pstFE[eFeID].bSatConfigOLBandDone = false;
#endif
                TDAL_DMDi_bFirstTuneDone = false;
	}

	mTBOX_RETURN(eError);
}

/******************************************************************************
 * Function Name : TDAL_DMD_Config
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_Config(tTDAL_DMD_FE eFeID,
		tTDAL_DMD_ConfigData *psCfgData)
{
	tTDAL_DMD_FrontEndMsgQStruct * pstFrontEndMsg = NULL;
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_DMD_Config");

	/*   Check   FeID   */
	if ((eFeID > eTDAL_DMD_FE_0)
			|| (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Config]   Check   FeId   failed   for   eFeID = %d\n",
						eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
	}

	/*   Check   Primary   Error   */
	if ((psCfgData == NULL)
			|| (TDAL_DMDi_TestConfigParameters(TDAL_DMDi_pstFE[eFeID].eType,
					psCfgData) == eTDAL_DMD_BAD_PARAMETER))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Config]   Check   primary   error   failed   for   eFeID = %d\n",
						eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}

	/*   Lock   Front   End   Access   mutex   */
	mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	/*   Configuration   in   accordance   with   the   kind   of   front   end   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		switch (TDAL_DMDi_pstFE[eFeID].eType)
		{
#ifdef   MEDIUM_SAT
		/* TODO Need not to be implemented */
#endif

#ifdef   MEDIUM_CAB
		case eTDAL_DMD_TYPE_QAM:
		/*   Configuration   of   the   required   feature   */
		switch(psCfgData->Cab.ConfigType)
		{
			case eTDAL_DMD_CFG_CAB_INIT :
			TDAL_DMDi_pstFE[eFeID].stConfigData.uCfgType.eCabType = eTDAL_DMD_CFG_CAB_INIT;
			memcpy(&TDAL_DMDi_pstFE[eFeID].stConfigData.uCfg.stCabData.Init,
					&(psCfgData->Cab.uCfg.Init),
					sizeof(tTDAL_DMD_CabInitData));
			TDAL_DMDi_QAM_Config_Init(eFeID,&(psCfgData->Cab.uCfg.Init));
			break;

			case eTDAL_DMD_CFG_CAB_OTHER :
			break;
		}
		break;
#endif

#ifdef   MEDIUM_TER
		/* TODO Need not to be implemented */
#endif

		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Config]   Type   of   Front   End   not   managed   or   unknwon\n"));
			eError = eTDAL_DMD_BAD_PARAMETER;
			break;
		}
	}

	/*   Unlock   Front   End   Access   mutex   */
	mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	mTBOX_RETURN(eError);
}

/*****************************************************
 **FUNCTION  ::  PowOf10
 **ACTION   ::  Compute  10^n (where n is an integer)
 **PARAMS IN   ::   number -> n
 **PARAMS OUT::   NONE
 **RETURN   ::  10^n
 *****************************************************/
int PowOf10(int number)
{
	int i;
	int result = 1;

	for (i = 0; i < number; i++)
		result *= 10;

	return result;
}

/*****************************************************
 **FUNCTION  ::  GetPowOf10
 **ACTION   ::  Compute  x according to y with y=10^x
 **PARAMS IN   ::   number -> y
 **PARAMS OUT::   NONE
 **RETURN   ::   x
 *****************************************************/
long GetPowOf10(int number)
{
	int i = 0;

	while (PowOf10(i) < number)
		i++;

	return i;
}

long GetPowOf10_BER(int number)
{
	int i = 0;

	while (PowOf10(i) < number)
		i++;
	if ((i != 0) && (i > 0))
	{
		i = (10 - i) + 1;
	}

	return i;
}

#if   !defined(TS_RELABILITY_USE_LEVEL_QUALITY_BER)
tTDAL_DMD_Error TDAL_DMD_GetTSReliability(tTDAL_DMD_FE eFeID,
		tTDAL_DMD_Info *psInfo, uint8_t *pTSReliability, uint32_t *pQuo,
		uint32_t *pRem, int32_t *pExp)
{
	tTDAL_DMD_Error eResult = eTDAL_DMD_NO_ERROR;
	uint32_t div, exp1;
	uint32_t qualityFactor = 0;
	uint8_t qualityToUse;
	uint32_t berToUse;

	mTBOX_FCT_ENTER("TDAL_DMD_GetTSReliability");

	if ((eFeID > eTDAL_DMD_FE_0)
			|| (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true))
	{

		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_GetInfo]   Check   primary   error   failed   for   ucFeId = %d\n",
						eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
	}

	if ((psInfo != NULL) && (pTSReliability != NULL) && (pQuo != NULL)
			&& (pRem != NULL) && (pExp != NULL))
	{

		MS_U16 snr = 0;
		float pfltBER = 0.0;
		MS_S16 ps16PWR = 0;

		/*   Lock   Front   End   Access   mutex   */
		mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

		//SNR:
		MApi_DigiTuner_GetSNR(0, &snr);
		//BER
		MApi_DigiTuner_GetBER(0, &pfltBER);
		//Signal level -> Get tuner Signal Power Strength ??
		MApi_DigiTuner_GetPWR(0, &ps16PWR);

		/*   Lock   Front   End   Access   mutex   */
		mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

		*pQuo = 0;
		*pRem = 0;
		*pExp = 0;

		if ((snr != 0))
		{
			qualityToUse = (snr * 100)
					/ TDAL_DMDi_pstFE[eFeID].ulValueQualityMax;
			berToUse = (uint32_t) (pfltBER + 0.5);
		}
		else
		{
			/*   Use   the   previous   information   */
			qualityToUse = psInfo->SignalQuality;
			berToUse = psInfo->SignalBER;
		}

		if (berToUse == 0)
		{
			*pTSReliability = qualityToUse;
			mTBOX_TRACE((kTBOX_NIV_2,"[TDAL_DMD_GetTSReliability]   BER   0   reliability = quality = %d\n", qualityToUse));
		}
		else
		{
			*pExp = (int32_t) -(GetPowOf10_BER(berToUse));
			exp1 = (uint32_t) GetPowOf10(berToUse) - 1;
			div = PowOf10(exp1);
			*pQuo = berToUse / div;
			*pRem = berToUse % div;
			div /= 100; /*   keep   only   two   numbers   after   decimal   point   */
			if (div != 0) /*use   to   avoid   crashing   of   something/0   division   in   toolset   ST40R4.0.1   */
			{
				*pRem /= div;
			}

			if (*pExp > -3)
			{
				qualityFactor = 80;
			}
			else if (*pExp == -3)
			{
				qualityFactor = 40;
			}
			else if (*pExp == -4)
			{
				qualityFactor = 30;
			}
			else if (*pExp == -5)
			{
				qualityFactor = 20;
			}
			else if (*pExp < -5)
			{
				qualityFactor = 10;
			}

			if (qualityToUse > qualityFactor)
			{
				*pTSReliability = qualityToUse - qualityFactor;
			}
			else
			{
				*pTSReliability = 0;
			}
			mTBOX_TRACE((kTBOX_NIV_2,"[TDAL_DMD_GetTSReliability]   freq   %d   Quality   %d   BER   %d   ->   %d.%dx10**-%d   reliability   %d\n", psInfo->TunFrequency, qualityToUse, berToUse, *pQuo,*pRem,*pExp, *pTSReliability));
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING,"[TDAL_DMD_GetTSReliability]   Check   primary   error   failed   \n"));
		eResult = eTDAL_DMD_BAD_PARAMETER;
	}
	mTBOX_RETURN(eResult);
}

#else   /*#if   !defined(TS_RELABILITY_USE_LEVEL_QUALITY_BER)*/

tTDAL_DMD_Error TDAL_DMD_GetTSReliability(
		tTDAL_DMD_FE eFeID,
		tTDAL_DMD_Info *psInfo,
		int32_t *pTSReliability,
		uint32_t *pQuo,
		uint32_t *pRem,
		int32_t *pExp)
{
	return eTDAL_DMD_NO_ERROR;
}
#endif   /*#if   !defined(TS_RELABILITY_USE_LEVEL_QUALITY_BER)*/
/******************************************************************************
 * Function Name : TDAL_DMD_GetInfo
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_GetInfo(tTDAL_DMD_FE eFeID, tTDAL_DMD_Info *psInfo)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	MS_FE_CARRIER_PARAM pParam;

	mTBOX_FCT_ENTER("TDAL_DMD_GetInfo");

	/*   Check   FeID   */
	if ((eFeID > eTDAL_DMD_FE_0)
			|| (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_GetInfo]   Check   primary   error   failed   for   ucFeId = %d\n",
						eFeID));
		eError = eTDAL_DMD_BAD_DEMODULATOR;
	}

	if (eError == eTDAL_DMD_NO_ERROR && psInfo == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_GetInfo]   tTDAL_DMD_Info * is NULL   ucFeId = %d\n",
						eFeID));
		eError = eTDAL_DMD_BAD_PARAMETER;
	}

	if (eError == eTDAL_DMD_NO_ERROR)
	{
		/*   Lock   Front   End   Access   mutex   */
		mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

		if (FALSE == MApi_DigiTuner_GetTPSInfo(0, &pParam))
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_DigiTuner_GetTPSInfo failed.\n"));
		}

		psInfo->TunFrequency = pParam.u32Frequency * 1000;
		//psInfo->TunData.Cab.Modulation=pParam->CabParam.eConstellation;
		switch (pParam.CabParam.eConstellation)
		{
		case CAB_QAM16:
			psInfo->TunData.Cab.Modulation = eTDAL_DMD_MOD_QAM16;
			break;
		case CAB_QAM32:
			psInfo->TunData.Cab.Modulation = eTDAL_DMD_MOD_QAM32;
			break;
		case CAB_QAM64:
			psInfo->TunData.Cab.Modulation = eTDAL_DMD_MOD_QAM64;
			break;
		case CAB_QAM128:
			psInfo->TunData.Cab.Modulation = eTDAL_DMD_MOD_QAM128;
			break;
		case CAB_QAM256:
			psInfo->TunData.Cab.Modulation = eTDAL_DMD_MOD_QAM256;
			break;
		case CAB_QAMAUTO:
			psInfo->TunData.Cab.Modulation = eTDAL_DMD_MOD_AUTO;
			break;
		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Setting psInfo->TunData.Cab.Modulation failed, constellation = %d.\n", pParam.CabParam.eConstellation));
			break;
		}

		switch (pParam.CabParam.eIQMode)
		{
		case CAB_IQ_NORMAL:
			psInfo->TunData.Cab.SpecInv = eTDAL_DMD_SPECTRUM_NORMAL;
			break;
		case CAB_IQ_INVERT:
			psInfo->TunData.Cab.SpecInv = eTDAL_DMD_SPECTRUM_INVERTED;
			break;
		default:
			psInfo->TunData.Cab.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
			mTBOX_TRACE((kTBOX_NIV_WARNING, "Setting psInfo->TunData.Cab.SpecInv unknown.\n"));
			break;
		}
		psInfo->TunData.Cab.SymbolRateBw = pParam.CabParam.u16SymbolRate * 1000;

		//pParam->CabParam.u32FreqOffset;
		//pParam->CabParam.u8TapAssign;
		//pParam->CabParam.u8TuneFreqOffset;

                EN_LOCK_STATUS eDemodstatus = E_DEMOD_CHECKING;
    MApi_DigiTuner_GetLock(0, &eDemodstatus);
		if (E_DEMOD_LOCK != eDemodstatus)
		{
			mTBOX_TRACE((kTBOX_NIV_WARNING, "MApi_DigiTuner_GetLock() returned FALSE.\n"));
			psInfo->CarrierStatus = eTDAL_DMD_LOCK_FAILED;

			psInfo->SignalQuality = 0;
			psInfo->SignalBER     = 0;
			psInfo->SignalLevel   = 0;
		}
		else
		{
			psInfo->CarrierStatus = eTDAL_DMD_LOCKED;

			//SNR:
			MS_U16 snr = 0;
			MApi_DigiTuner_GetSNR(0, &snr);
			psInfo->SignalQuality = snr;

			//BER
			float pfltBER = 0.0;
			MApi_DigiTuner_GetBER(0, &pfltBER);
			psInfo->SignalBER = (uint32_t) (pfltBER + 0.5);

			//Signal level -> Get tuner Signal Power Strength ??
			MS_S16 ps16PWR = 0;
			if (FALSE == MApi_DigiTuner_GetPWR(0, &ps16PWR))
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_Demod_GetPWR failed\n"));
				mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
			}
			psInfo->SignalLevel = (uint8_t) ps16PWR;
		}

		/*   Lock   Front   End   Access   mutex   */
		mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);
	}

	mTBOX_RETURN(eError);
}

/******************************************************************************
 * Function Name : TDAL_DMD_Tune
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_Tune(tTDAL_DMD_FE eFeID, tTDAL_DMD_TunData *psTunData,
		uint32_t uiFrequency)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	tTDAL_DMD_FrontEndMsgQStruct pstFrontEndMsg;

	mTBOX_FCT_ENTER("TDAL_DMD_Tune");

	if (TDAL_DMDi_SourceState == eTDAL_DMD_CFG_SOURCE_IS_ATV)
	{
		mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMD_Tune] eFeID = %d  analog   freq=%u   _   drift   enable=%s   _   drift=%d\n",
						eFeID,
						uiFrequency,
						(psTunData->AnalogTer.bEnableDriftMonitoring == true) ? "true" : "false",
						psTunData->AnalogTer.lManualDriftToApply));
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMD_Tune] eFeID = %d   digital   freq=%u   _   BW%d\n",
						eFeID,
						uiFrequency,
						psTunData->Cab.SymbolRateBw));
	}

	/*   Check   FeID   */
	if ((eFeID > eTDAL_DMD_FE_0) || (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Tune]   Check   FeId   failed   for   ucFeId = %d\n",eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
	}

	/*   Check   Primary   Error   */
	if ((psTunData == NULL) || (TDAL_DMDi_pstFE[eFeID].bConfigInitDone != true)
		|| (TDAL_DMDi_TestTuneParameters(eFeID, psTunData, uiFrequency) == eTDAL_DMD_BAD_PARAMETER))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Tune]   Check   primary   error   failed   for   ucFeId = %d\n",eFeID));
			mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
		}

	memset(&pstFrontEndMsg, 0, sizeof(tTDAL_DMD_FrontEndMsgQStruct));

	/*   Send   the   command   to   the   management   task   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{

		pstFrontEndMsg.eTypeOfEvent = eTDAL_DMD_FRONT_END_EVENT_TYPE_TUNE;

		memcpy(&pstFrontEndMsg.uDataType.stTunData, psTunData,sizeof(tTDAL_DMD_TunData));
		pstFrontEndMsg.uiFrequency = uiFrequency;

		mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_Tune]   Send   a   eTDAL_DMD_FRONT_END_EVENT_TYPE_TUNE   to   Front   End   Task   %d   _   freq = %ld   Hz   _   BW = %d\n",
						eFeID,
						pstFrontEndMsg.uiFrequency,
						pstFrontEndMsg.uDataType.stTunData.Cab.SymbolRateBw));

		MS_BOOL queueResult = MsOS_SendToQueue(
				TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ,(MS_U8*) &pstFrontEndMsg,
				sizeof(tTDAL_DMD_FrontEndMsgQStruct), 0);

		if (queueResult != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_TUNE] Error queuing tune request"));
			eError = eTDAL_DMD_DEVICE_BUSY;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_TUNE] Tune request queued"));
		}
                TDAL_DMDi_bFirstTuneDone = true;
	}

	mTBOX_RETURN(eError);
}

/******************************************************************************
 * Function Name : TDAL_DMD_Unlock
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_Unlock(tTDAL_DMD_FE eFeID)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	MS_BOOL retLocked, queueResult;

#if   1
	tTDAL_DMD_FrontEndMsgQStruct pstFrontEndMsg;
#endif
	mTBOX_FCT_ENTER("TDAL_DMD_Unlock");

	/*   Check   FeID   */
	if ((eFeID > eTDAL_DMD_FE_0) || (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_Unlock]   Check   primary   error   failed   for   ucFeId = %d\n",eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_DEMODULATOR);
	}

	/*   Lock   Front   End   Access   mutex   */
	mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

        EN_LOCK_STATUS eDemodstatus = E_DEMOD_CHECKING;
	retLocked = MApi_DigiTuner_GetLock((MS_U8)eFeID, &eDemodstatus);

    // why need check eTdalDmdActionInProgress??
    // common to avoid to showing Assertion failed (ErrCodeDMD == eDMD_NO_ERROR) in FILE cabinstall_dmd.c
	if ((eDemodstatus != E_DEMOD_LOCK && retLocked) /*|| TDAL_DMDi_pstFE[eFeID].eTdalDmdActionInProgress == eTDAL_DMD_FRONT_END_ACTION_NONE*/)
	{
		eError = eTDAL_DMD_NOT_LOCKED;
	}
        else if((TDAL_DMDi_bFirstTuneDone == false) && (eError == E_DEMOD_LOCK))
        {        
                eError = eTDAL_DMD_NOT_LOCKED;
        }        


#if   1
	/*   Send   the   command   to   the   management   task   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		pstFrontEndMsg.eTypeOfEvent = eTDAL_DMD_FRONT_END_EVENT_TYPE_UNLOCK;

		mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_Unlock]   Send   a   eTDAL_DMD_FRONT_END_EVENT_TYPE_UNLOCK   to   Front   End   Task   %d\n",eFeID));

		queueResult = MsOS_SendToQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ,(MS_U8*)&pstFrontEndMsg, sizeof(tTDAL_DMD_FrontEndMsgQStruct), 0);
		if (queueResult != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not queue unlock request"));
			eError = eTDAL_DMD_DEVICE_BUSY;
		}
	}

#endif
	/*   Unlock   Front   End   Access   mutex   */
	mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	mTBOX_RETURN(eError);
}

/******************************************************************************
 * Function Name : TDAL_DMD_StartScan
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_StartScan(tTDAL_DMD_FE eFeID,
		tTDAL_DMD_ScanData *psScanData, unsigned int uiStartFreq,
		unsigned int uiEndFreq)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	tTDAL_DMD_FrontEndMsgQStruct pstFrontEndMsg;
	MS_BOOL bRet;

	mTBOX_FCT_ENTER("TDAL_DMD_StartScan");

	/*   Check   Primary   Error   */
	if ((eFeID > eTDAL_DMD_FE_0) || (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true) || (psScanData == NULL)
		|| (TDAL_DMDi_pstFE[eFeID].bConfigInitDone != true)
		|| ((TDAL_DMDi_TestScanParameters(eFeID, psScanData) == eTDAL_DMD_BAD_PARAMETER)))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_StartScan]   Check   primary   error   failed   for   ucFeId = %d\n",eFeID));
			mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
		}

	/*   Lock   Front   End   Access   mutex   */
	mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	/*   If   scan   already   in   progress, stop   it   first   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		if (TDAL_DMDi_pstFE[eFeID].bScanInProgress == true)
		{
			eError = TDAL_DMD_StopScan(eFeID);
		}
	}

	/*   Careful, Unlock   Tune   is   mandatory   in   this   case:
	 If   a   tune   is   performed   and   signal   is   unlocked
	 STUNER_SetScanList   retuen   an   error
	 It   needs   to   be   explicitely   UNLOCKED
	 !!!!!!!!   */
	// TODO Ivica Unlock tuner
	//STTUNER_Unlock(TDAL_DMDi_pstFE[eFeID].SThandle);
	/*   Send   the   command   to   the   management   task   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		pstFrontEndMsg.eTypeOfEvent = eTDAL_DMD_FRONT_END_EVENT_TYPE_SCAN;
		memcpy(&pstFrontEndMsg.uDataType.stCompleteScanData.stScanData,psScanData, sizeof(tTDAL_DMD_ScanData));

		pstFrontEndMsg.uDataType.stCompleteScanData.uiStartFreq = uiStartFreq;
		pstFrontEndMsg.uDataType.stCompleteScanData.uiEndFreq = uiEndFreq;

		bRet = MsOS_SendToQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ,(MS_U8*)&pstFrontEndMsg, sizeof(pstFrontEndMsg), 0);
		if (bRet != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_StartScan]   Unable   to   queue   a   new   message   for   eFeID = %d\n",eFeID));
			eError = eTDAL_DMD_DEVICE_BUSY;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_StartScan]   Send   a   eTDAL_DMD_FRONT_END_EVENT_TYPE_SCAN   to   Front   End   Task   %d   _   from   freq   %ld   Hz   to   %ld   Hz   \n",
							eFeID,
							uiStartFreq,
							uiEndFreq));
			TDAL_DMDi_pstFE[eFeID].bScanInProgress = true;
		}

	}

	/*   Unlock   Front   End   Access   mutex   */
	mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	mTBOX_RETURN(eError);
}

/******************************************************************************
 * Function Name : TDAL_DMD_ContScan
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_ContScan(tTDAL_DMD_FE eFeID)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	tTDAL_DMD_FrontEndMsgQStruct pstFrontEndMsg;
	MS_BOOL bRet;

	mTBOX_FCT_ENTER("TDAL_DMD_ContScan");

	/*   Check   Primary   Error   */
	if ((eFeID > eTDAL_DMD_FE_0) || (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true) || (TDAL_DMDi_pstFE[eFeID].bScanInProgress != true))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_ContScan]   Check   primary   error   failed   for   ucFeId = %d\n",eFeID));
			mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
		}

	/*   Lock   Front   End   Access   mutex   */
	mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	/*   Send   the   command   to   the   management   task   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		pstFrontEndMsg.eTypeOfEvent = eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN;

		bRet = MsOS_SendToQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ,(MS_U8*)&pstFrontEndMsg, sizeof(pstFrontEndMsg), 0);
		if (bRet != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_ContScan]   Unable   to   queue   eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN   for   eFeID = %d\n",eFeID));
			eError = eTDAL_DMD_DEVICE_BUSY;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_ContScan]   Send   a   eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN   to   Front   End   Task   %d\n",eFeID));
		}
	}

	/*   Unlock   Front   End   Access   mutex   */
	mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	mTBOX_RETURN(eError);
}

/******************************************************************************
 * Function Name : TDAL_DMD_StopScan
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_DMD_Error TDAL_DMD_StopScan(tTDAL_DMD_FE eFeID)
{
	tTDAL_DMD_Error eError = eTDAL_DMD_NO_ERROR;
	tTDAL_DMD_FrontEndMsgQStruct pstFrontEndMsg;
	MS_BOOL bRet;

	mTBOX_FCT_ENTER("TDAL_DMD_StopScan");

	/*   Check   Primary   Error   */
	if ((eFeID > eTDAL_DMD_FE_0) || (TDAL_DMDi_pstFE[eFeID].bFrontEndUsed != true))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_StopScan]   Check   primary   error   failed   for   ucFeId = %d\n",eFeID));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}

	/*   Lock   Front   End   Access   mutex   */
	mLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	/*   Send   the   command   to   the   management   task   */
	if (eError == eTDAL_DMD_NO_ERROR)
	{
		pstFrontEndMsg.eTypeOfEvent = eTDAL_DMD_FRONT_END_EVENT_TYPE_STOP_SCAN;

		bRet = MsOS_SendToQueue(TDAL_DMDi_pstFE[eFeID].FrontEndMngtMsgQ,(MS_U8*)&pstFrontEndMsg, sizeof(pstFrontEndMsg), 0);
		if (bRet != TRUE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMD_StopScan]   Unable   to   queue   eTDAL_DMD_FRONT_END_EVENT_TYPE_STOP_SCAN   for   eFeID = %d\n",eFeID));
			eError = eTDAL_DMD_DEVICE_BUSY;
		}
		else
		{
		    mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMD_StopScan]   Send   a   eTDAL_DMD_FRONT_END_EVENT_TYPE_STOP_SCAN   to   Front   End   Task   %d\n",eFeID));
			TDAL_DMDi_pstFE[eFeID].bScanInProgress = false;
		}
	}

	/*   Unlock   Front   End   Access   mutex   */
	mUnLockAccess(TDAL_DMDi_pstFE[eFeID].pFrontEndAccess);

	mTBOX_RETURN(eError);
}

/******************************************************************************
 *
 *            INTERNAL   MODULE   FUNCTIONS
 *
 *******************************************************************************/

LOCAL MS_BOOL TDAL_DMDi_ReceiveFromQueue(
		tTDAL_DMD_FE ucFrontEnd,
		tTDAL_DMD_FrontEndMsgQStruct * msg,
		MS_U32 timeout)
{
	MS_U32 actualSize;
	MS_BOOL result;

	//mTBOX_FCT_ENTER("TDAL_DMDi_ReceiveFromQueue");

	result =  MsOS_RecvFromQueue(TDAL_DMDi_pstFE[ucFrontEnd].FrontEndMngtMsgQ, (MS_U8 *) msg, sizeof(tTDAL_DMD_FrontEndMsgQStruct), &actualSize, timeout);

	mTBOX_ASSERT(result == TRUE ?  sizeof(tTDAL_DMD_FrontEndMsgQStruct) == actualSize : TRUE);

	//mTBOX_RETURN(result);
	return result;
}

LOCAL void TDAL_DMDi_NotifyStatus(tTDAL_DMD_FE ucFrontEnd, tTDAL_DMD_NotifyStatus status)
{
    tTDAL_DMD_NotifyStatusFct statusFct = TDAL_DMDi_pstFE[ucFrontEnd].pNotifyStatusFct;

    mUnLockAccess(TDAL_DMDi_pstFE[ucFrontEnd].pFrontEndAccess);

    statusFct(ucFrontEnd, status);

    mLockAccess(TDAL_DMDi_pstFE[ucFrontEnd].pFrontEndAccess);
}

/******************************************************************************
 * Function Name : TDAL_DMDi_FrontEndManagementTask
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
LOCAL void TDAL_DMD_TuneTask(
		void *argv)
{
	/*   Variable   used   for   front   end   stack   management   */
	bool bExitTask = false;
	bool bError = false;
	tTDAL_DMD_FE ucFrontEnd = *((tTDAL_DMD_FE *)argv);
	tTDAL_DMD_FrontEndMsgQStruct MsgReceived;

	MS_U32 actualSize;
	MS_BOOL bRet;

	unsigned int uiTunFrequencyInProgress = 0;
	unsigned int uiScanFrequencyInProgress = 0;
	tTDAL_DMD_TunData stTunDataInProgress;
	tTDAL_DMDi_CompleteScanData scanData;

	/*   Variable   used   for   SCAN   management   */
	MS_BOOL dontBlock = FALSE;
	MS_U32 u32LockTimeMax = 0;
	MS_U32 u32LockTimeStart = 0;

#ifdef DEBUG_SIGNAL_QUALITY
	MS_U16 u16SNR = 0;
    float fltBER = 0;
#endif

	mTBOX_FCT_ENTER("TDAL_DMDi_FrontEndManagementTask");

	mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMDi_FrontEndManagementTask]   Front   End   %d   Management   Task   Started\n",
					ucFrontEnd));

	/*   Lock/Unlock   mutex   to   wait   after   the   end   of   intialisation   */
	mLockAccess(TDAL_DMDi_pstFE[ucFrontEnd].pFrontEndAccess);
	mUnLockAccess(TDAL_DMDi_pstFE[ucFrontEnd].pFrontEndAccess);

	while (bExitTask == false)
	{
		bRet = TDAL_DMDi_ReceiveFromQueue(ucFrontEnd, &MsgReceived, dontBlock == TRUE ? 0 : 50);
		dontBlock = FALSE;

		mLockAccess(TDAL_DMDi_pstFE[ucFrontEnd].pFrontEndAccess);

		if (bRet == TRUE)
		{
			switch (MsgReceived.eTypeOfEvent)
			{
			case eTDAL_DMD_FRONT_END_EVENT_TYPE_END_OF_TASK:
				mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_DMDi_FrontEndManagementTask] eTDAL_DMD_FRONT_END_EVENT_TYPE_END_OF_TASK message received\n", MsgReceived.eTypeOfEvent));
				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_END_OF_TASK;
				bExitTask = true;
				break;
			case eTDAL_DMD_FRONT_END_EVENT_TYPE_TUNE:
				/*   Saved   the   configuration   */
				memcpy(&stTunDataInProgress,
						&MsgReceived.uDataType.stTunData,
						sizeof(tTDAL_DMD_TunData));
				uiTunFrequencyInProgress = MsgReceived.uiFrequency;
				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_REQUEST_TUNING;
				break;
			case eTDAL_DMD_FRONT_END_EVENT_TYPE_UNLOCK:
			case eTDAL_DMD_FRONT_END_EVENT_TYPE_STOP_SCAN:
				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NONE;
				break;
			case eTDAL_DMD_FRONT_END_EVENT_TYPE_SCAN:
				memcpy(&scanData,
						&MsgReceived.uDataType.stCompleteScanData,
						sizeof(tTDAL_DMDi_CompleteScanData));
				uiScanFrequencyInProgress = scanData.uiStartFreq;
				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_REQUEST;
				break;
			case eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN:
				if (TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress == eTDAL_DMD_FRONT_END_ACTION_SCANNING_WAITING_NEXT)
				{
					TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_NEXT_FREQ;
				}
				else
				{
					mTBOX_TRACE((kTBOX_NIV_WARNING, "Unexpected message eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN\n"));
				}
				break;
			default:
				mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_DMDi_FrontEndManagementTask] Message type %d is not supported\n", MsgReceived.eTypeOfEvent));
				break;
			}

		}

                EN_LOCK_STATUS eDemodstatus = E_DEMOD_CHECKING;
                
		switch(TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress)
		{
		case eTDAL_DMD_FRONT_END_ACTION_NONE:
			/* do nothing */
			break;
		case eTDAL_DMD_FRONT_END_ACTION_SCANNING_WAITING_NEXT:
			/* waiting for CONTINUE SCAN message to move on */
			break;
		case eTDAL_DMD_FRONT_END_ACTION_END_OF_TASK:
			bExitTask = true;
			dontBlock = TRUE;
			break;
		case eTDAL_DMD_FRONT_END_ACTION_REQUEST_TUNING:
			/*   If   last   action   is   a   tune   on   the   same   frequency   */
			bError = TDAL_DMDi_pstFE[ucFrontEnd].pTuneFunction(
					ucFrontEnd, &stTunDataInProgress,
					uiTunFrequencyInProgress);

			if (bError == true)
			{
				/*   Notify   LOCK_FAILED   */
				mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMDi_FrontEndManagementTask]   Notify   Status   1   eTDAL_DMD_LOCK_FAILED\n"));
				TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_LOCK_FAILED);

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Tune request sent\n"));

			    bRet = MApi_DigiTuner_GetMaxLockTime(0, FE_TUNE_MANUAL, &u32LockTimeMax);
			    mTBOX_ASSERT(bRet == TRUE);
			    u32LockTimeStart = MsOS_GetSystemTime();

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_TUNNING_IN_PROGRESS;
			}

			break;
		case eTDAL_DMD_FRONT_END_ACTION_SCANNING_REQUEST:
			/*   If   last   action   is   a   tune   on   the   same   frequency   */
			mTBOX_ASSERTm(sizeof(tTDAL_DMD_TunData ) == sizeof(tTDAL_DMD_ScanData), "These two types should be same in size and have same fields\n");
			bError = TDAL_DMDi_pstFE[ucFrontEnd].pTuneFunction(
					ucFrontEnd, &scanData.stScanData,
					uiScanFrequencyInProgress);

			if (bError == true)
			{
				/*   Notify   LOCK_FAILED   */
				mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_DMDi_FrontEndManagementTask] Could not lock, going to next frequency\n"));

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_NEXT_FREQ;
				dontBlock = TRUE;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Tune request sent\n"));

			    bRet = MApi_DigiTuner_GetMaxLockTime(0, FE_TUNE_MANUAL, &u32LockTimeMax);
			    mTBOX_ASSERT(bRet == TRUE);
			    u32LockTimeStart = MsOS_GetSystemTime();

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_IN_PROGRESS;
			}

			break;
		case eTDAL_DMD_FRONT_END_ACTION_TUNNING_IN_PROGRESS:
			MApi_DigiTuner_GetLock(0, &eDemodstatus);
			if (E_DEMOD_LOCK == eDemodstatus)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Tuner locked\n"));
				TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_LOCKED);

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_TUNED;
			}
			else
			{
				if (MsOS_GetSystemTime() - u32LockTimeStart > u32LockTimeMax )
				{
					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Could not lock to a frequency\n"));
					TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_LOCK_FAILED);

					TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED;
				}
				else
				{
					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Could not lock to a frequency\n"));
				}
			}
			break;
		case eTDAL_DMD_FRONT_END_ACTION_SCANNING_IN_PROGRESS:
			MApi_DigiTuner_GetLock(0, &eDemodstatus);
			if (E_DEMOD_LOCK == eDemodstatus)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Tuner locked\n"));
				TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_FOUND);

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_WAITING_NEXT;
			}
			else
			{
				if (MsOS_GetSystemTime() - u32LockTimeStart > u32LockTimeMax )
				{
					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Could not lock to a frequency. Switching to next freq\n"));

					TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_NEXT_FREQ;
				}
				else
				{
					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Waiting to lock to frequency\n"));
				}
			}
			break;
		case eTDAL_DMD_FRONT_END_ACTION_TUNED:
			MApi_DigiTuner_GetLock(0, &eDemodstatus);
			if (E_DEMOD_LOCK != eDemodstatus)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "Signal lost, sending SIGNAL LOST\n"));
				TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_SIGNAL_LOST);

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED;
			}
			break;
		case eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED:
			/*   If   last   action   is   a   tune   on   the   same   frequency   */
			bError = TDAL_DMDi_pstFE[ucFrontEnd].pTuneFunction(
					ucFrontEnd, &stTunDataInProgress,
					uiTunFrequencyInProgress);

			if (bError == true)
			{
				/*   Notify   LOCK_FAILED   */
				mTBOX_TRACE((kTBOX_NIV_1,"TuneFunction returned error, trying again later\n"));
				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED;
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Tune request sent\n"));

			    bRet = MApi_DigiTuner_GetMaxLockTime(0, FE_TUNE_MANUAL, &u32LockTimeMax);
			    mTBOX_ASSERT(bRet == TRUE);
			    u32LockTimeStart = MsOS_GetSystemTime();

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SILENT_TUNE;
			}
			break;
		case eTDAL_DMD_FRONT_END_ACTION_SILENT_TUNE:
			MApi_DigiTuner_GetLock(0, &eDemodstatus);
			if (E_DEMOD_LOCK == eDemodstatus)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Tuner locked\n"));
				TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_LOCKED);

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_TUNED;
			}
			else
			{
				if (MsOS_GetSystemTime() - u32LockTimeStart > u32LockTimeMax)
				{
					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Could not lock to a frequency\n"));
					TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED;
				}
				else
				{
					// Do nothing, wait
				#ifdef DEBUG_SIGNAL_QUALITY
			        MApi_DigiTuner_GetSNR(0, &u16SNR);
			        MApi_DigiTuner_GetBER(0, &fltBER);

					mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask]  Signal quality %d %f\n",
			            u16SNR, fltBER));

				#endif
				}
			}
			break;
		case eTDAL_DMD_FRONT_END_ACTION_SCANNING_NEXT_FREQ:
			uiScanFrequencyInProgress += 8000000;
			if (uiScanFrequencyInProgress > scanData.uiEndFreq)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "End of scan\n"));
				TDAL_DMDi_NotifyStatus(ucFrontEnd, eTDAL_DMD_END_OF_RANGE);

				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_NONE;
			}
			else
			{
				TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress = eTDAL_DMD_FRONT_END_ACTION_SCANNING_REQUEST;
			}
			break;
		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMDi_FrontEndManagementTask]  Unknown state %d", TDAL_DMDi_pstFE[ucFrontEnd].eTdalDmdActionInProgress));
			mTBOX_ASSERT(FALSE);

			break;
		}

		mUnLockAccess(TDAL_DMDi_pstFE[ucFrontEnd].pFrontEndAccess);

	}

	mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_DMDi_FrontEndManagementTask] Exit management thread\n"));

	TDAL_DMDi_pstFE[ucFrontEnd].FrontEndTaskArgs.TaskFinished = TRUE;
}

LOCAL tTDAL_DMD_Error TDAL_DMDi_TestTuneParameters(unsigned char ucFeID,
		tTDAL_DMD_TunData *psTunData, unsigned int uiFrequency)
{
	mTBOX_FCT_ENTER("TDAL_DMDi_TestTuneParameters");

	if (psTunData == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"TDAL_DMDi_TestTuneParameters(ERROR - Line   %d)\n",
						__LINE__));
		mTBOX_RETURN (eTDAL_DMD_BAD_PARAMETER);
	}

	switch (TDAL_DMDi_pstFE[ucFeID].eType)
	{
#ifdef   MEDIUM_SAT
	case eTDAL_DMD_TYPE_QPSK :
	break;
#endif

#ifdef   MEDIUM_CAB
	case eTDAL_DMD_TYPE_QAM :
	if (DMD_ConvertModulationTdal2ST(psTunData->Cab.Modulation) == kTDAL_DMD_BAD_ELEMENT_CONVERSION)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMDi_TestConfigParameters]   Bad   parameter   for   CAB   front   end\n"));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}
	break;
#endif

#ifdef   MEDIUM_TER
	case eTDAL_DMD_TYPE_OFDM :
	break;
#endif

	default:
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"TDAL_DMDi_TestConfigParameters(ERROR - Line   %d)\n",
						__LINE__));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}

	mTBOX_RETURN(eTDAL_DMD_NO_ERROR);
}

LOCAL tTDAL_DMD_Error
TDAL_DMDi_TestScanParameters(unsigned   char      ucFeID,
            tTDAL_DMD_ScanData      *psScanData)
{
	mTBOX_FCT_ENTER("TDAL_DMDi_TestScanParameters");
	mTBOX_RETURN(eTDAL_DMD_NO_ERROR);
}

LOCAL tTDAL_DMD_Error TDAL_DMDi_TestConfigParameters(tTDAL_DMD_Type eType,
		tTDAL_DMD_ConfigData *psCfgData)
{
	unsigned char NbSrBw;

	mTBOX_FCT_ENTER("TDAL_DMDi_TestConfigParameters");

	if (psCfgData == (tTDAL_DMD_ConfigData *) NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"TDAL_DMDi_TestConfigParameters(ERROR - Line   %d)\n",
						__LINE__));
		mTBOX_RETURN (eTDAL_DMD_BAD_PARAMETER);
	}

	switch (eType)
	{
#ifdef   MEDIUM_SAT
	case eTDAL_DMD_TYPE_QPSK :
	break; /***   case   eTDAL_DMD_TYPE_QPSK   ***/
#endif

#ifdef   MEDIUM_CAB
	case eTDAL_DMD_TYPE_QAM :
	if ((psCfgData->Cab.ConfigType < eTDAL_DMD_CFG_CAB_INIT) ||
			(psCfgData->Cab.ConfigType > eTDAL_DMD_CFG_CAB_OTHER))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"TDAL_DMDi_TestConfigParameters(ERROR - Line   %d)\n",
						__LINE__));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}

	if (psCfgData->Cab.ConfigType == eTDAL_DMD_CFG_CAB_INIT)
	{
		NbSrBw = 0;

		if (psCfgData->Cab.uCfg.Init.pSymbolRateList != NULL)
		{
			/*   Check   all   symbol   rate   */
			while (psCfgData->Cab.uCfg.Init.pSymbolRateList[NbSrBw] != 0)
			{
				if ((psCfgData->Cab.uCfg.Init.pSymbolRateList[NbSrBw] < kTDAL_DMD_CFG_FS_BW_MIN_CAB) ||
						(psCfgData->Cab.uCfg.Init.pSymbolRateList[NbSrBw] > kTDAL_DMD_CFG_FS_BW_MAX_CAB))
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,
									"TDAL_DMDi_TestConfigParameters(ERROR - Line   %d)\n",
									__LINE__));
					mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
				}
				NbSrBw++;
			}
		}
	}
	break;
#endif

#ifdef   MEDIUM_TER
	case eTDAL_DMD_TYPE_OFDM :
	break;
#endif

	default:
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,
						"TDAL_DMDi_TestConfigParameters(ERROR - Line   %d)\n",
						__LINE__));
		mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
	}

	mTBOX_RETURN(eTDAL_DMD_NO_ERROR);
}


#ifdef   MEDIUM_CAB
/******************************************************************************
* Function Name : TDAL_DMDi_QAM_Config_Init
*
* Description   :
*
* Side effects  :
*
* Comment       :
*
*****************************************************************************/
LOCAL   tTDAL_DMD_Error
TDAL_DMDi_QAM_Config_Init   (unsigned   char   ucFeID,
            tTDAL_DMD_CabInitData   *psData)
{
//   unsigned   char   i;
//   ST_ErrorCode_t      eStErrorCode;
//   STTUNER_BandList_t   stbandList;
//   STTUNER_Band_t      stband;
//
   mTBOX_FCT_ENTER("TDAL_DMDi_QAM_Config_Init");
//
//   i = 0;
//
//   if   (psData->pSymbolRateList != NULL)
//   {
//      while   ((i < kTDAL_DMD_MAX_SB)   &&
//      (psData->pSymbolRateList[i] != 0))
//      {
//      TDAL_DMDi_pstFE[ucFeID].pulCabSymbolRates[i] = psData->pSymbolRateList[i];
//      i++;
//      TDAL_DMDi_pstFE[ucFeID].ucCabNbSymbRates++;
//      }
//   }
//
//   TDAL_DMDi_pstFE[ucFeID].ucCabNbSymbRates = i;
//
//   while   (i < kTDAL_DMD_MAX_SB)
//   {
//      TDAL_DMDi_pstFE[ucFeID].pulCabSymbolRates[i] = 0;
//      i++;
//   }
//
//   /*   Set   Band   List   */
//      stbandList.NumElements = 1;
//   stbandList.BandList = &stband;
//
//   memset(&stband, 0, sizeof(STTUNER_Band_t));
//   stband.BandStart = kTDAL_DMD_CFG_FREQ_TUNER_MIN_CAB;
//   stband.BandEnd = kTDAL_DMD_CFG_FREQ_TUNER_MAX_CAB;
//
//      eStErrorCode = STTUNER_SetBandList(TDAL_DMDi_pstFE[ucFeID].SThandle, &stbandList);
//
//      if   (eStErrorCode != ST_NO_ERROR)
//      {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//            "Error   returned   by   STTUNER_SetBandList()   %s\n",
//            TBOX_GetDriverErrorText(eStErrorCode)));
//      mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
//      }
//
//
   TDAL_DMDi_pstFE[ucFeID].bConfigInitDone = true;

   mTBOX_RETURN(/*TDAL_DMDi_QAM_Config_SetBandList(ucFeID)*/eTDAL_DMD_NO_ERROR);
}

/******************************************************************************
* Function Name : TDAL_DMDi_QAM_Tune
*
* Description   :
*
* Side effects  :
*
* Comment       :
*
*****************************************************************************/
LOCAL   bool
TDAL_DMDi_QAM_Tune   (unsigned   char   ucFeID,
        tTDAL_DMD_TunData   *psTunData,
        unsigned   int   TunFrequency)
{
    bool      bError = false;
	MS_FE_CARRIER_PARAM param;

    mTBOX_FCT_ENTER("TDAL_DMDi_QAM_Tune");

	param.u32Frequency = TunFrequency / 1000;

	param.CabParam.u16SymbolRate = (MS_U16) (psTunData->Cab.SymbolRateBw / 1000);
	//param.CabParam.eConstellation = tunData->Cab.Modulation;
	switch (psTunData->Cab.Modulation)
	{
	case eTDAL_DMD_MOD_QAM16:
		param.CabParam.eConstellation = CAB_QAM16;
		break;
	case eTDAL_DMD_MOD_QAM32:
		param.CabParam.eConstellation = CAB_QAM32;
		break;
	case eTDAL_DMD_MOD_QAM64:
		param.CabParam.eConstellation = CAB_QAM64;
		break;
	case eTDAL_DMD_MOD_QAM128:
		param.CabParam.eConstellation = CAB_QAM128;
		break;
	case eTDAL_DMD_MOD_QAM256:
		param.CabParam.eConstellation = CAB_QAM256;
		break;
	case eTDAL_DMD_MOD_AUTO:
		param.CabParam.eConstellation = CAB_QAMAUTO;
		break;
	default:
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Setting param.CabParam.eConstellation failed.\n"));
		bError = true;
		mTBOX_RETURN(bError);
		break;
	}
        #if 1//tuner_debug
        param.CabParam.eIQMode = CAB_IQ_NORMAL;
        param.CabParam.eBandWidth = CAB_BW_8M;
        #endif
	bError = MApi_DigiTuner_Tune2RfCh(0, &param, FE_TUNE_MANUAL) == TRUE ? false : true;

	mTBOX_TRACE((kTBOX_NIV_1, "Tune to (Freq) = (%ld), (Symbol rate) = (%d) Try Lock\n",
			param.u32Frequency,
			param.CabParam.u16SymbolRate));

   mTBOX_RETURN(bError);
}


/******************************************************************************
* Function Name : TDAL_DMDi_QAM_SetScanList
*
* Description   :
*
* Side effects  :
*
* Comment       :
*
*****************************************************************************/
//LOCAL   tTDAL_DMD_Error
//TDAL_DMDi_QAM_SetScanList(unsigned   char   ucFeID,
//            tTDAL_DMD_TunData   *pData)
//{
//   tTDAL_DMD_Error      rc = eTDAL_DMD_NO_ERROR;
//   ST_ErrorCode_t      eStErrorCode;
//   STTUNER_Modulation_t   Modulation;
//   /*STTUNER_Spectrum_t   Spectrum;*/
//   STTUNER_ScanList_t   scanList;
//   STTUNER_Scan_t      scan;
//
//   mTBOX_FCT_ENTER("TDAL_DMDi_QAM_SetScanList");
//
//   if   (pData == NULL)
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"SetScanList   with   NULL   data\n"));
//      mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
//   }
//
//   Modulation = DMD_ConvertModulationTdal2ST(pData->Cab.Modulation);
//   if   (Modulation == kTDAL_DMD_BAD_ELEMENT_CONVERSION)
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//        "SetScanList   with   BAD   modulation   (%d)\n",
//        pData->Cab.Modulation));
//      mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
//   }
//
//   /*Spectrum = DMD_ConvertSpectrumTdal2ST(pData->Cab.SpecInv);
//   if   (Spectrum == kTDAL_DMD_BAD_ELEMENT_CONVERSION)
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//        "SetScanList   with   BAD   spectrum   (%d)\n",
//        pData->Cab.SpecInv));
//      mTBOX_RETURN(eTDAL_DMD_BAD_PARAMETER);
//   }*/
//
//   /*   Setting   */
//   memset(&scan, 0, sizeof(STTUNER_Scan_t));
//   scan.Modulation   = Modulation;
//   scan.Band       = 0;
//   scan.AGC       = kTDAL_DMD_ST_DEFAULT_AGC;
//   scan.ResidualOffset = kTDAL_DMD_ST_DEFAULT_RESIDUAL_OFF;
//   scan.J83       = STTUNER_J83_ALL;
//   scan.SpectrumIs   = STTUNER_INVERSION_AUTO;
//   scan.Spectrum   = STTUNER_INVERSION_AUTO;
//   scan.SymbolRate   = pData->Cab.SymbolRateBw;
//
//   scanList.NumElements = 1;
//   scanList.ScanList = &scan;
//
//   eStErrorCode = STTUNER_SetScanList(TDAL_DMDi_pstFE[ucFeID].SThandle,&scanList);
//
//   if   (eStErrorCode != ST_NO_ERROR)
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//        "Error   returned   by   STTUNER_SetScanList()   %s\n",
//        TBOX_GetDriverErrorText(eStErrorCode)));
//      rc = eTDAL_DMD_BAD_PARAMETER;
//   }
//
//   mTBOX_RETURN(rc);
//}

/******************************************************************************
* Function Name : TDAL_DMDi_QAM_Scan
*
* Description   :
*
* Side effects  :
*
* Comment       :
*
*****************************************************************************/
LOCAL   bool
TDAL_DMDi_QAM_Scan(unsigned   char   ucFeID,
        tTDAL_DMD_ScanData   *psScanData,
        unsigned   int   uiStartFreq,
        unsigned   int   uiEndFreq)
{
   bool      bResult = false;
//   ST_ErrorCode_t   eStErrorCode;
//   unsigned   int      FreqStep;
//
   mTBOX_FCT_ENTER("TDAL_DMDi_Scan_QAM");
//
//   /*   Set   scan   list   */
//   if   (TDAL_DMDi_QAM_SetScanList(ucFeID,psScanData) != eTDAL_DMD_NO_ERROR)
//   {
//      bResult = true;
//   }
//
//   /*   Scanning   */
//   if   (bResult == false)
//   {
//      if   (uiStartFreq == uiEndFreq)
//      {
//      FreqStep = 0;
//      }
//      else
//      {
//      FreqStep = psScanData->Cab.SymbolRateBw;
//      }
//
//      eStErrorCode = STTUNER_Scan(TDAL_DMDi_pstFE[ucFeID].SThandle,
//               (unsigned   int)uiStartFreq,
//               (unsigned   int)uiEndFreq,
//               FreqStep,
//               kTDAL_DMD_DEFAULT_TIMEOUT);
//
//      if   (eStErrorCode != ST_NO_ERROR)
//      {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//            "Error   returned   by   STTUNER_Scan()   %s\n",
//            TBOX_GetDriverErrorText(eStErrorCode)));
//      bResult = true;
//      }
//      else
//      {
//      mTBOX_TRACE((kTBOX_NIV_1,
//            "STTUNER_Scan(s=%d,e=%d,step=%d)   %s\n",
//            uiStartFreq,uiEndFreq,FreqStep,TBOX_GetDriverErrorText(eStErrorCode)));
//      }
//   }

   mTBOX_RETURN(bResult);
}

#endif

LOCAL   int
TDAL_DMDi_ConvertElement   (char*   ElementName,
            int   value,
            bool   FromMSTAR)
{
   int   i;
   int   size = 0;
   int   returnedValue = 0;

   tTDAL_DMD_TableElement   *TablePointer;

   if   (strcmp   (ElementName, "Modulation") == 0)
   {
      TablePointer = TDAL_DMDi_ModulationTable;
      size = (int)(sizeof(TDAL_DMDi_ModulationTable)/sizeof(tTDAL_DMD_TableElement));
   }
   else
   {
      /*   No   trace   for   this   function   */
      mTBOX_TRACE   ((kTBOX_NIV_CRITICAL, "Element   type   not   recognized :   %s\n", ElementName));
      return(kTDAL_DMD_BAD_ELEMENT_CONVERSION);
   }

   /*   Read   the   right   array   until   we   find   the   input   value   given   */
   for   (i=0; i<size; i++)
   {
      if   ((FromMSTAR == true)   &&   (TablePointer[i].ST_value == value))
      {
		  returnedValue = TablePointer[i].tdal_value;
		  break;
      }
      else   if   ((FromMSTAR == false)   &&   (TablePointer[i].tdal_value == value))
      {
		  returnedValue = TablePointer[i].ST_value;
		  break;
      }
   }
   if   (i >= size)   /*   element   not   found   */
   {
      returnedValue = kTDAL_DMD_BAD_ELEMENT_CONVERSION;
      /*   No   trace   for   this   function   */
      mTBOX_TRACE   ((kTBOX_NIV_CRITICAL, "%s   conversion   error :   %s = %d   (%s   value)\n", ElementName, ElementName, value, (FromMSTAR == true)?   "MSTAR":"TDAL_API"));

   }

   return(returnedValue);
}
