/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMX
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_dmx/tdal_dmx_channel.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DMX implementation
 *
 *****************************************************************************/

/********************************************************
   *   Includes                        *
   ********************************************************/
#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "tdal_dmx.h"
#include "tdal_dmx_priv.h"

#ifdef   PRODUCT_USE_CA
#include "tdal_desc.h"
#endif

#include "apiDMX.h"

#include "tbox.h"


/********************************************************
   *   Defines                        *
   ********************************************************/

#define kTDAL_DMX_SIGNAL_WAIT_BUFFER_TIMEOUT 1000

/*#define MULTIMEDIA_DISABLE_FILTERS */

/********************************************************
   *   Macros                        *
   ********************************************************/
mTBOX_SET_MODULE(eTDAL_DMX);


/********************************************************
   *   Local   File   Variables   (LOCAL)            *
   ********************************************************/
#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)
bool   gTDAL_DMX_AllocChannelTeletextOsd = FALSE;
#endif

LOCAL bool TDAL_DMXi_DisableSections = false;

void TDAL_DMXm_DisableFilters(bool disable)
{
#ifdef MULTIMEDIA_DISABLE_FILTERS

    int i = 0;

    printf("############## Disabling all filters, this could be dangerous #########\n");

    mLockAccess(TDAL_DMXi_pSectionTableAccess);

    for (i = 0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
    {
        if (disable)
        {
            if (TDAL_DMXi_pstFilter[i].bEnable)
            {
                MApi_DMX_Stop(TDAL_DMXi_pstFilter[i].FilterHandle);
                MApi_DMX_SectReset(TDAL_DMXi_pstFilter[i].FilterHandle);
                TDAL_DMXi_pstFilter[i].bMultimediaDisabled = true;
            }
        }
        else
        {
            if (TDAL_DMXi_pstFilter[i].bEnable && TDAL_DMXi_pstFilter[i].bMultimediaDisabled)
            {
                MApi_DMX_Start(TDAL_DMXi_pstFilter[i].FilterHandle);
            }
            TDAL_DMXi_pstFilter[i].bMultimediaDisabled = false;
        }
    }

    TDAL_DMXi_DisableSections = disable;

    mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

    if (disable)
    {
        MsOS_DelayTask(1000);
    }
#endif

}

/********************************************************
   *   Module   variables   (MODULE)               *
   ********************************************************/

void TDAL_DMXi_NotifySectCallback(MS_U8 u8DmxId, DMX_EVENT enEvent, MS_U32 NotifyParam)
{
    tTDAL_DMX_FilterId filterId = (tTDAL_DMX_FilterId) NotifyParam;
    MS_BOOL queueSuccess;

	if(enEvent == DMX_EVENT_DATA_READY)
    {
		mTBOX_TRACE((kTBOX_NIV_3,"[TDAL_DMXi_NotifySectCallback]   Section received\n"));
#if MSTAR_QUEUE
        queueSuccess = MsOS_SendToQueue(TDAL_DMXi_SectionQueue, &filterId, sizeof(tTDAL_DMX_FilterId), 0);
#else
        queueSuccess = TDAL_Enqueue(TDAL_DMXi_SectionQueue, &filterId);
#endif
        if (queueSuccess != TRUE)
        {
        	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMXi_NotifySectCallback]   Queue full, queue ID = %d\n", TDAL_DMXi_SectionQueue));
        }
    }
    else if(enEvent == DMX_EVENT_BUF_OVERFLOW)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_DMXi_NotifySectCallback]   DMX Buffer Overflow\n"));
    }
    else
    {
    	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_DMXi_NotifySectCallback]  Unknown event %d\n", enEvent));
    }
}


void TDAL_DMXi_SectionTask(void * argv)
{
	tTDAL_DMX_ChannelId channelId;
	tTDAL_DMX_FilterId filterId = kTDAL_DMX_ILLEGAL_FILTER;

	MS_BOOL bRes;
	MS_U32 actualSize, bytesReceived, remainingSize;
	static MS_U8 sectionBuffer[1024*8];//it is too big for heap
	DMX_FILTER_STATUS status;
	int id = (int) argv;

	mTBOX_FCT_ENTER_T("TDAL_DMXi_SectionTask   ");

	for (; TDAL_DMXi_SectionTaskArr[id].ShouldExit == FALSE;)
	{
		mTBOX_TRACE((kTBOX_NIV_3,"[%s %d]: taskId=%d waiting\n",__FUNCTION__,__LINE__,id));

		/*---------------------------------*/
		/*   Wait   for   data   on   specified   slot   */
		/*---------------------------------*/
#if MSTAR_QUEUE
		bRes = MsOS_RecvFromQueue(TDAL_DMXi_SectionQueue, &filterId, sizeof(filterId), &actualSize, kTDAL_DMX_SIGNAL_WAIT_BUFFER_TIMEOUT);
#else
		bRes = TDAL_Dequeue(TDAL_DMXi_SectionQueue, &filterId);
#endif
		if (bRes != TRUE)
		{
			continue;
		}
		else
		{
			/*we should delete all number of the tasks */
			if ((TDAL_DMXi_SectionTaskArr[id].ShouldExit == TRUE) || (filterId == -1))
			{
				mTBOX_TRACE((kTBOX_NIV_1, "[%s] exit by queue force\n",__FUNCTION__));
				break;
			}
		}

		channelId = TDAL_DMXi_pstFilter[filterId].AssociatedChannelId;

        if (TDAL_DMXi_pstFilter[filterId].bMultimediaDisabled)
        {
            MApi_DMX_Stop(TDAL_DMXi_pstFilter[filterId].FilterHandle);
            MApi_DMX_SectReset(TDAL_DMXi_pstFilter[filterId].FilterHandle);
        }

    	mTBOX_ASSERT(sizeof(filterId) == actualSize);
		mTBOX_TRACE((kTBOX_NIV_1,"[%s]:"
						"Received for filterId %d, channelId %d, pid %d\n",
						__FUNCTION__,
						filterId,
						channelId,
						TDAL_DMXi_pstChannel[channelId].ChannelPid));

		/*---------------------------------*/
		/*   Lock   Protection   Table   Semaphore   */
		/*---------------------------------*/
		mLockAccess(TDAL_DMXi_pSectionTableAccess);

		if(TDAL_DMXi_pstChannel[channelId].ChannelPid == 100)
		{

		}
		if ((bRes == TRUE) && (TDAL_DMXi_pstChannel[channelId].bWaitingForDeletion != TRUE))
		{
			/*-----------------------*/
			/*   Read   data   from   buffer   */
			/*-----------------------*/

			int max_count = 10;

			do
			{
				max_count--;
				memset(sectionBuffer,0,sizeof(sectionBuffer));
				status = MApi_DMX_CopyData(TDAL_DMXi_pstFilter[filterId].FilterHandle,sectionBuffer,sizeof(sectionBuffer),&bytesReceived,&remainingSize,NULL);
				if (status != DMX_FILTER_STATUS_OK)
				{
					mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not copy data, error returned %d. Resetting...\n", status));
					MApi_DMX_SectReset(TDAL_DMXi_pstFilter[filterId].FilterHandle);
					goto unlock_mutex;
				}

				if (TDAL_DMXi_DisableSections)
				{
				    printf("Should not get here\n");
				    MApi_DMX_Stop(TDAL_DMXi_pstFilter[filterId].FilterHandle);
				    MApi_DMX_SectReset(TDAL_DMXi_pstFilter[filterId].FilterHandle);
				    goto unlock_mutex;
				}

				mTBOX_TRACE((kTBOX_NIV_1, "Section received, bytes received %d, bytesRemaining %d\n", bytesReceived, remainingSize ));

				if (status == DMX_FILTER_STATUS_OK
						&& TDAL_DMXi_pstChannel[channelId].Callback
						&& TDAL_DMXi_pstChannel[channelId].bEnable
						&& TDAL_DMXi_pstFilter[filterId].bEnable)
				{
					void * callbackSectionBuffer = TDAL_DMXi_Section_Malloc(bytesReceived);

					if (callbackSectionBuffer == NULL)
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not allocate memory for the section buffer\n"));
						goto unlock_mutex;
					}

					if (callbackSectionBuffer != NULL)
					{
						memcpy(callbackSectionBuffer, sectionBuffer, bytesReceived);

						/*-----------------------------------*/
						/*   Unlock   Protection   Table   Semaphore   */
						/*-----------------------------------*/
						mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
						mTBOX_ASSERT(TDAL_DMXi_pstChannel[channelId].Callback != NULL);
						TDAL_DMXi_pstChannel[channelId].Callback(channelId,
								filterId,
								TDAL_DMXi_pstFilter[filterId].Tag,
								bytesReceived, callbackSectionBuffer);

						callbackSectionBuffer = NULL;

						/*-----------------------------------*/
						/*   Lock   Protection   Table   Semaphore   */
						/*-----------------------------------*/
						mLockAccess(TDAL_DMXi_pSectionTableAccess);

					}

				}

			}
			while (remainingSize > 0 && (max_count > 0));

		}

		/*-----------------------------------*/
		/*   Unlock   Protection   Table   Semaphore   */
		/*----------------------------------   */
unlock_mutex:
		mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
	}

	mTBOX_TRACE((kTBOX_NIV_WARNING, "[%s]:   (chId=%d)   End   of   Task\n",__FUNCTION__, channelId));

	TDAL_DMXi_SectionTaskArr[id].Finished = TRUE;

	mTBOX_RETURN;

}


/**======================================================================**
 * Function Name : TDAL_DMX_Allocate_Channel
 *
 * Description   : This function allocates a channel. The output is the channel Id
 *             if succedded. Else the output is kTDAL_DMX_ILLEGAL_CHANNEL.
 *
 * Side effects  : allocation of a section or PES filtering channel
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct.
 *             kTDAL_DMX_ERROR_NOT_INITIALIZED if the module is not initialized.
 *             kTDAL_DMX_ERROR_TOO_MANY_REQ if there is no more channel available
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT if stream allocate failed
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS if a parameter is bad
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Allocate_Channel
(
   tTDAL_DMX_DemuxId      DemuxId,
   tTDAL_DMX_ChannelType_t   *   pType,
   tTDAL_DMX_ChannelId   *      pChannelId
)
{
	   tTDAL_DMX_ChannelId   channelId;
	   tTDAL_DMX_Error      errorCode = kTDAL_DMX_NO_ERROR;
	   uint32_t      requiredSize = 0;
#ifdef   __DEBUG_TRACE__
   char   *pChStreamStr;
   char   *pChOutStr;
   char   *pChMemStr;
#endif
//	   char        currentTaskName[kTDAL_DMX_MAX_TASK_NAME];
//	#if   !defined(USE_TDAL_TS_SOURCE_SINK)   /*   CLRV   is   in   baseline   mode   in   TS   sink   demux   injection   mode   */
//	   STCLKRV_SourceParams_t   clkrvSource;
//	#endif
//	#ifdef   __DEBUG_TRACE__
//	   char   *pChStreamStr;
//	   char   *pChOutStr;
//	   char   *pChMemStr;
//	#endif
//	   void   (*pGetBufferTask)(void   *);

	   mTBOX_FCT_ENTER("TDAL_DMX_Allocate_Channel");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Channel:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   if(pChannelId == NULL   ||   DemuxId   >=   kTDAL_DMX_NB_DEMUX   ||   pType == NULL)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Channel:   bad   parameter   \n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
	   }

	#ifdef   __DEBUG_TRACE__
	   switch(pType->eChannelStream)
	   {
	      case   eTDAL_DMX_VIDEO_STREAM:
	      pChStreamStr="VID";
	      break;
	   case   eTDAL_DMX_AUDIO_STREAM:
	      pChStreamStr="AUD";
	      break;
	   case   eTDAL_DMX_PCR_STREAM:
	      pChStreamStr="PCR";
	      break;
	   case   eTDAL_DMX_TELETEXT_STREAM:
	      pChStreamStr="TXT";
	      break;
	   case   eTDAL_DMX_SUBTITLE_STREAM:
	      pChStreamStr="SUBT";
	      break;
	   case   eTDAL_DMX_SECTION_STREAM:
	      pChStreamStr="SECT";
	      break;
	   case   eTDAL_DMX_TP_STREAM:
	      pChStreamStr="TP";
	      break;
	   default:
	      pChStreamStr="??";
	      break;
	   }

	   switch(pType->eChannelOutput)
	   {
	      case   eTDAL_DMX_DECODER:
	      pChOutStr = "DEC";
	      break;
	      case   eTDAL_DMX_MEMORY:
	      pChOutStr = "MEM";
	      break;
	      default:
	      pChOutStr = "??";
	      break;
	   }

	   switch(pType->eChannelMemorySize)
	   {
	      case   eTDAL_DMX_NO_MEMORY:
	      pChMemStr = "NO";
	      break;
	      case   eTDAL_DMX_LOW_MEMORY_SIZE:
	      pChMemStr = "LOW";
	      break;
	      case   eTDAL_DMX_MEDIUM_MEMORY_SIZE:
	      pChMemStr = "MED";
	      break;
	      case   eTDAL_DMX_HIGHT_MEMORY_SIZE:
	      pChMemStr = "HIGH";
	      break;
	      default:
	      pChMemStr = "??";
	      break;
	   }
	mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Channel:   stream=%s   out=%s   mem=%s\n",pChStreamStr,pChOutStr,pChMemStr));
	#endif

	   mLockAccess(TDAL_DMXi_pSectionTableAccess);

	   /*---------------------------*/
	   /*   Look   after   a   free   channel   */
	   /*---------------------------*/
	   channelId = TDAL_DMXi_LookForFreeChannel();
	   if   (channelId == kTDAL_DMX_ILLEGAL_CHANNEL)
	   {
	      mTBOX_TRACE((kTBOX_NIV_WARNING,"Allocate_Channel:   Unable   to   find   a   free   channel\n"));
	      errorCode = kTDAL_DMX_ERROR_TOO_MANY_REQ;
	   }

   /*----------------------------------------------*/
   /*   Error   Management                 */
   /*----------------------------------------------*/

   /*   If   channel   allocation   failed   */
   if   (errorCode   !=   kTDAL_DMX_NO_ERROR   )
   {
	  if(channelId   !=   kTDAL_DMX_ILLEGAL_CHANNEL)
	  {
	  /*   Try   to   free   all   ressources   already   created   */
			if (TDAL_DMXi_FreeChannelAndAssociatedRessources(channelId) !=   kTDAL_DMX_NO_ERROR   )
	  {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Channel:   "
			"Error   in   TDAL_DMXi_FreeChannelAndAssociatedRessources(%d)\n",
			channelId));
	  }
	  }
   }
   else
   {
	  /*   Channel   allocation   success   */
	  TDAL_DMXi_pstChannel[channelId].bUsed = TRUE;
	  TDAL_DMXi_pstChannel[channelId].bEnable = FALSE;
	  TDAL_DMXi_pstChannel[channelId].DemuxId = DemuxId;
	  TDAL_DMXi_pstChannel[channelId].ChannelStream = pType->eChannelStream;
	  TDAL_DMXi_pstChannel[channelId].ChannelOutput = pType->eChannelOutput;
	  TDAL_DMXi_pstChannel[channelId].ChannelMemorySize = pType->eChannelMemorySize;
	  *pChannelId = channelId;

	   /*-------------------------------------*/
	   /*   Check   valid   Stream(type)   and   Output   */
	   /*-------------------------------------*/
	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      if   (TDAL_DMXi_pstChannel[channelId].ChannelOutput == eTDAL_DMX_DECODER)
	      {
			  switch   (TDAL_DMXi_pstChannel[channelId].ChannelStream)
			  {
			  /*==========================*/
			  case   eTDAL_DMX_VIDEO_STREAM:
			  case   eTDAL_DMX_AUDIO_STREAM:
			  case   eTDAL_DMX_PCR_STREAM:
			  case   eTDAL_DMX_TELETEXT_STREAM:
			  case   eTDAL_DMX_SUBTITLE_STREAM:
				break;

			  /*==========================*/
			  default   :
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "Allocate_Channel:   bad   parameters\n"));
				errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
				break;
			  /*==========================*/
			  }
	      }
	      else   if   (TDAL_DMXi_pstChannel[channelId].ChannelOutput== eTDAL_DMX_MEMORY)
	      {
			  switch   (TDAL_DMXi_pstChannel[channelId].ChannelStream)
			  {
			  /*==========================*/
			  case   eTDAL_DMX_VIDEO_STREAM:
			  case   eTDAL_DMX_AUDIO_STREAM:
			  case   eTDAL_DMX_AUDIO_DESCRIPTION_STREAM:
				break;

			  /*==========================*/
			  case   eTDAL_DMX_PCR_STREAM:
				errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
				break;

			  /*==========================*/
			  case   eTDAL_DMX_TELETEXT_STREAM:
		#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)
			  {
				int   i;

				/*Check   the   Channel   already   allocated   by   VBI*/
				for   (i=0;   i<kTDAL_DMXi_MAX_NB_OF_CHANNELS;   i++)
				{
								if( (TDAL_DMXi_pstChannel[i].ChannelStream == eTDAL_DMX_TELETEXT_STREAM) && (TDAL_DMXi_pstChannel[i].bUsed))
					{
					*pChannelId = i;
					/*   Channel   Allocated   by   OSD*/
					gTDAL_DMX_AllocChannelTeletextOsd = TRUE;
					mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
					mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
					}
				}
			  }
		#endif   /*   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)   */
		#if   defined(TTXT_VBI_ON)
				requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   80;
		#else
				requiredSize = 3   *   1656;
		#endif
				break;

			  case   eTDAL_DMX_SUBTITLE_STREAM:
				if   (TDAL_DMXi_pstChannel[channelId].ChannelMemorySize == eTDAL_DMX_LOW_MEMORY_SIZE)
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   4;
				}
				else   if   (TDAL_DMXi_pstChannel[channelId].ChannelMemorySize == eTDAL_DMX_MEDIUM_MEMORY_SIZE)
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   4;
				}
				else
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   64;
				}
				break;

			  /*==========================*/
			  case   eTDAL_DMX_SECTION_STREAM:
				if   (TDAL_DMXi_pstChannel[channelId].ChannelMemorySize == eTDAL_DMX_LOW_MEMORY_SIZE)
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   4;
				}
				else   if   (TDAL_DMXi_pstChannel[channelId].ChannelMemorySize == eTDAL_DMX_MEDIUM_MEMORY_SIZE)
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   16;
				}
				else
				{
					/*   high   size   */
					requiredSize = kTDAL_DMX_FOUR_KILO_SIZE   *   16;
				}
		#ifdef   BOOTLOADER
					/*   stand   alone   */
				requiredSize = kTDAL_DMX_SECTIONS_MEM_SIZE;   /*   40   K   */
		#endif
				break;

			  /*==========================*/
			  case   eTDAL_DMX_TP_STREAM:
				if   (TDAL_DMXi_pstChannel[channelId].ChannelMemorySize == eTDAL_DMX_LOW_MEMORY_SIZE)
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   4;
				}
				else   if   (TDAL_DMXi_pstChannel[channelId].ChannelMemorySize == eTDAL_DMX_MEDIUM_MEMORY_SIZE)
				{
					requiredSize = kTDAL_DMX_ONE_KILO_SIZE   *   4;
				}
				else
				{
					requiredSize = kTDAL_DMX_FOUR_KILO_SIZE   *   4;
				}
				break;

			  /*==========================*/
			  default   :
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Channel:   bad   parameters\n"));
				errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
				break;
			  /*==========================*/
			  }
	      }
	      else
	      {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Allocate_Channel:   bad   parameters\n"));
	      errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
	      }
	   }
#ifdef   PRODUCT_USE_CA
    /* Notify the descrambler layer that the channel changed pid */
    TDAL_DESC_DemuxChannelCallback(channelId, pType->eChannelStream);
#endif
     TDAL_DMXi_pstChannel[channelId].BufferSize = requiredSize;

	  mTBOX_TRACE((kTBOX_NIV_1,"Allocate_Channel:   %s   [ChannelId = %d]\n",
			errorCode == kTDAL_DMX_NO_ERROR ? "SUCCESS" : "ERROR",
			channelId));
   }

   /*   ---------------------------------   */
   /*   Unlock   Protection   Table   Semaphore   */
   /*   ---------------------------------   */
   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
   mTBOX_RETURN(errorCode);

}


/**======================================================================**
 * Function Name : TDAL_DMX_Register_Channel_Callback
 *
 * Description   : this function register a callback on a channel
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if Id does not exist
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS if callback is null
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *
 * Comment       : ! no need to stop filters on the channel.
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Register_Channel_Callback(   tTDAL_DMX_ChannelId   ChannelId,
                            tTDAL_DMX_ChannelCallbackFct   pCallback   )
{
	   mTBOX_FCT_ENTER("TDAL_DMX_Register_Channel_Callback");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
	            "Register_Channel_Callback:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   /*-----------------------------------------*/
	   /*   check   channelId   argument   and   allocation   */
	   /*-----------------------------------------*/
	   if   ((ChannelId   <   0   ||   ChannelId   >=   kTDAL_DMXi_MAX_NB_OF_CHANNELS)   ||
	      (TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE   ))
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Register_Channel_Callback:   "
	            "ChannelId   not   available   or   not   allocated\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_INVALID_CHANNEL_ID);
	   }

	   /*-----------------------------------------*/
	   /*   check   the   callback            */
	   /*-----------------------------------------*/

	   if   (pCallback == NULL)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Register_Channel_Callback:   "
	            "Callback   is   NULL\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
	   }

	   /*--------------------------------------------*/
	   /*   save   the   callback   in   the   channel   structure   */
	   /*--------------------------------------------*/
	   mLockAccess(TDAL_DMXi_pSectionTableAccess);
	   TDAL_DMXi_pstChannel[ChannelId].Callback = pCallback;
	   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
	   mTBOX_TRACE((kTBOX_NIV_1,   "Register_Channel_Callback:   SUCCEED   \n"));

	   mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
}



/**======================================================================**
 * Function Name : TDAL_DMX_Set_Channel_PID
 *
 * Description   : this function set a PID on a channel
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if bad Id
 *             kTDAL_DMX_ERROR_MUTEX for mutex error
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS for bad PID
 *    not used -> kTDAL_DMX_ERROR_PID_IN_USE if PID is already used
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Set_Channel_PID(   tTDAL_DMX_ChannelId   ChannelId   ,
                        uint16_t   Pid   )
{
	   tTDAL_DMX_Error   errorCode = kTDAL_DMX_NO_ERROR;
	   int   i;

	   mTBOX_FCT_ENTER("TDAL_DMX_Set_Channel_PID");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   /*-----------------------------*/
	   /*   Check   channel   Id      */
	   /*-----------------------------*/
	if ((ChannelId < 0) ||  (ChannelId >= kTDAL_DMXi_MAX_NB_OF_CHANNELS))
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   Argument   ChannelId   not   available\n"));
	      errorCode = kTDAL_DMX_ERROR_INVALID_CHANNEL_ID;
	   }

	   /*-----------------------------------*/
	   /*   check   if   the   channel   is   allocated   */
	   /*-----------------------------------*/
	if ((errorCode == kTDAL_DMX_NO_ERROR) && (TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE))
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   Channel   is   not   allocated\n"));
	      errorCode = kTDAL_DMX_ERROR_INVALID_CHANNEL_ID;
	   }

	   /*-----------------------------------*/
	   /*   check   PID   value            */
	   /*-----------------------------------*/
	if ((errorCode == kTDAL_DMX_NO_ERROR) && (Pid > 0x1FFF))
	   {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   Argument   PID(0x%x)   not   valid\n",Pid));
	      errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
	   }

	   mTBOX_TRACE((kTBOX_NIV_1,"Set_Channel_PID:   Good   parameters\n"));

	   mLockAccess(TDAL_DMXi_pSectionTableAccess);

	   if   (TDAL_DMXi_pstChannel[ChannelId].ChannelStream   !=   eTDAL_DMX_PCR_STREAM)
	{   
		/*---------------------------------------------------------*/
	      /*   check   if   the   PID   is   not   already   used   in   other   channels   */
	      /*   except   for   PCR            ^--   (on   same   demux)   */
	      /*---------------------------------------------------------*/
	      mTBOX_TRACE((kTBOX_NIV_1,"Set_Channel_PID:   check   PID   %d   already   used\n",   Pid));

		for   (i=0;(i < kTDAL_DMXi_MAX_NB_OF_CHANNELS) && (errorCode == kTDAL_DMX_NO_ERROR);i++)
		{
			if ((Pid   !=   0x1FFF)/*   illegal   pid   */
				&& (TDAL_DMXi_pstChannel[i].ChannelPid == Pid)
				&& (TDAL_DMXi_pstChannel[i].DemuxId == TDAL_DMXi_pstChannel[ChannelId].DemuxId)
				&& (TDAL_DMXi_pstChannel[i].ChannelId != ChannelId)
				&& (TDAL_DMXi_pstChannel[i].bUsed == 1)
				&& (TDAL_DMXi_pstChannel[i].ChannelStream != eTDAL_DMX_PCR_STREAM))
	      {
	        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   PID   %d   is   already   used   on   an   other   channel\n",   Pid));
	        errorCode = kTDAL_DMX_ERROR_PID_IN_USE;
	#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)
	        /*   check   if   PID   already   set   by   VBI*/
				if((TDAL_DMXi_pstChannel[i].ChannelStream == eTDAL_DMX_TELETEXT_STREAM)
					&& (TDAL_DMXi_pstChannel[i].bEnable))
	        {
	            /*do   nothing*/
	            mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
	        }
	#endif
	      }
	      }
	   }

	   /*   Moved   -   EDO   */
	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	#ifdef   PRODUCT_USE_CA
	/*
	      STOS_SemaphoreWait(TDAL_DESC_synchronisation_lock);
	      TDAL_DESC_pstDesc[ChannelId].ChannelPid = Pid;
	      STOS_SemaphoreSignal(TDAL_DESC_synchronisation_lock);
	*/
	      /* Notify the descrambler layer that the channel changed pid */
	      TDAL_DESC_DemuxPidCallback( ChannelId, Pid );
	#endif
	      TDAL_DMXi_pstChannel[ChannelId].ChannelPid = Pid;

	      if(TDAL_DMXi_pstChannel[ChannelId].bEnable == TRUE)
	      {
	      /*   Force   a   enable/disable   sequence   to   update   the   PID   in   demux   HW   */
	      errorCode = TDAL_DMX_Control_Channel(ChannelId,   eTDAL_DMX_CTRL_DISABLE);
	      if(errorCode   !=   kTDAL_DMX_NO_ERROR)
	      {
	        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   Control_Channel(DISABLE)   FAILED)\n"));
	      }
	      else
	      {
	        errorCode = TDAL_DMX_Control_Channel(ChannelId,   eTDAL_DMX_CTRL_ENABLE);
	        if(errorCode   !=   kTDAL_DMX_NO_ERROR)
	        {
	            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Set_Channel_PID:   Control_Channel(ENABLE)   FAILED)\n"));
	        }
	      }
	      }
	   }

	   if   (errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      /*-----------------------------------------------*/
	      /*   Channel   PID   set   success,   update   channel   table   */
	      /*-----------------------------------------------*/
		mTBOX_TRACE((kTBOX_NIV_1,"Set_Channel_PID:   " "SUCCEED   [PID = 0x%x]   on   [channel   %d]\n",Pid,ChannelId));
	   }

	   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

	   mTBOX_RETURN(errorCode);
}

/**======================================================================**
 * Function Name : TDAL_DMX_Control_Channel
 *
 * Description   : this function is used to control a channel
 *              ( enabling, stopping, resetting)
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if no channel is invalid
 *                                or channel not used or bad PID
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS for bad argument ctrl
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *             kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL if channel is not used
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Control_Channel(   tTDAL_DMX_ChannelId   ChannelId,
                        tTDAL_DMX_Ctrl      Ctrl)
{
	   tTDAL_DMX_Error      errorCode = kTDAL_DMX_NO_ERROR;
	   int            i;

	   mTBOX_FCT_ENTER("TDAL_DMX_Control_Channel");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
	            "Control_Channel:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   /*-------------------------------------*/
	   /*   Check   channel            */
	   /*-------------------------------------*/
	   if   (ChannelId   <   0   ||   ChannelId   >=   kTDAL_DMXi_MAX_NB_OF_CHANNELS)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
	            "ChannelId   not   valid\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_INVALID_CHANNEL_ID);
	   }

	   /*-------------------------------------*/
	   /*   check   channel   state            */
	   /*-------------------------------------*/
	   if   (TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
	            "channel   not   used\n"));

	      mTBOX_RETURN(kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL);
	   }

	   /*-------------------------------------*/
	   /*   check   argument            */
	   /*-------------------------------------*/
	   if   ((Ctrl   !=   eTDAL_DMX_CTRL_RESET)   &&
	      (Ctrl   !=   eTDAL_DMX_CTRL_ENABLE)   &&
	      (Ctrl   !=   eTDAL_DMX_CTRL_DISABLE))
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
	            "Argument   Ctrl   is   an   unknown   value   [%d]\n",
	            Ctrl));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_BAD_PARAMETERS);
	   }
	   else
	   {
	      mTBOX_TRACE((kTBOX_NIV_1,   "Control_Channel:   "
	            "ChId=%d   to   be   control   %s\n",
	            ChannelId,
	            (Ctrl == eTDAL_DMX_CTRL_ENABLE)   ?   "ENABLE"   :   ((Ctrl == eTDAL_DMX_CTRL_DISABLE)   ?   "DISABLE"   :   "RESET")));
	   }
	#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)
	   /*   In   this   case,   the   driver   had   already   enable   the   channel   for   the   VBI   */
	   /*   So,   the   driver   can't   disable   the   channel   for   the   OSD   (VBI   is   the   first)*/
	   if(   (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_TELETEXT_STREAM)   &&
	      (gTDAL_DMX_AllocChannelTeletextOsd)   &&
	      (Ctrl == eTDAL_DMX_CTRL_DISABLE)   )
	   {
	      mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
	   }
	#endif
	   /*-------------------------------------*/
	   /*   Lock   Protection   mutex        */
	   /*-------------------------------------*/
	   mLockAccess(TDAL_DMXi_pSectionTableAccess);

	   /*-------------------------------------*/
	   /*   Control   Channel            */
	   /*-------------------------------------*/

	   switch   (Ctrl)
	   {
	      case   eTDAL_DMX_CTRL_ENABLE:
	      if   ((TDAL_DMXi_pstChannel[ChannelId].bEnable == FALSE)   &&
	        (TDAL_DMXi_pstChannel[ChannelId].ChannelPid   !=   kTDAL_DMX_ILLEGAL_PID))
	      {

	        	TDAL_DMXi_pstChannel[ChannelId].bEnable = TRUE;

	            /*   Find   all   filters   to   be   controlled   */
				for (i = 0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
				{
					if ((TDAL_DMXi_pstFilter[i].AssociatedChannelId == ChannelId)
							&& (TDAL_DMXi_pstFilter[i].bEnable == FALSE)
							&& (TDAL_DMXi_pstFilter[i].bWaitingEnable == TRUE))
					{
						TDAL_DMXi_Control_Filter(i, eTDAL_DMX_CTRL_ENABLE);
						TDAL_DMXi_pstFilter[i].bEnable = TRUE;
					}
				}
//
//	            /*   Enable   the   list   of   filter   */
//	            STErrorCode = STPTI_ModifyGlobalFilterState(listOfFilters,
//	                                    0,
//	                                    listOfFilters,
//	                                    nbOfFiltersToControl);
//	            if(STErrorCode   !=   ST_NO_ERROR)
//	            {
//	               mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
//	                        "STPTI_ModifyGlobalFilterState(-)   return   %s\n",
//	                        TBOX_GetDriverErrorText(STErrorCode)));
//
//	            errorCode = kTDAL_DMX_ERROR_BAD_PARAMETERS;
//	            }
//	            else
//	            {
//	               mTBOX_TRACE((kTBOX_NIV_1,"Control_Channel:   "
//	                        "STPTI_ModifyGlobalFilterState(-)   return   %s\n",
//	                        TBOX_GetDriverErrorText(STErrorCode)));
//	            }
	        }

	        if(   errorCode == kTDAL_DMX_NO_ERROR   )
	        {
	            /*   GS   :   put   in   comments   to   remove   warnings   */
	            /*switch(   TDAL_DMXi_pstChannel[ChannelId].ChannelStream   )
	            {
	            case   eTDAL_DMX_VIDEO_STREAM:
	            case   eTDAL_DMX_AUDIO_STREAM:
	            {
	               break;
	            }
	            }*/

	            if(errorCode == kTDAL_DMX_NO_ERROR)
	            {


	#if   defined(__DEBUG_TRACE__)   &&   defined(TDAL_DMX_DBG_TRC_PCK_CNT)
	            TDAL_DMXi_DebugPacketCount(ChannelId);
	#endif
	            }
	      }
	      break;

	      case   eTDAL_DMX_CTRL_DISABLE:
	      if   ((TDAL_DMXi_pstChannel[ChannelId].bEnable == TRUE)   &&
	        (TDAL_DMXi_pstChannel[ChannelId].ChannelPid   !=   kTDAL_DMX_ILLEGAL_PID))
	        {
	        	/*   Found   all   filters   to   be   controlled   */
				for (i = 0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
				{
					if ((TDAL_DMXi_pstFilter[i].AssociatedChannelId == ChannelId)
							&& (TDAL_DMXi_pstFilter[i].bEnable == TRUE))
					{
						TDAL_DMXi_Control_Filter(i, eTDAL_DMX_CTRL_DISABLE);
						TDAL_DMXi_pstFilter[i].bEnable = FALSE;
						TDAL_DMXi_pstFilter[i].bWaitingEnable = TRUE;
					}
				}

				TDAL_DMXi_pstChannel[ChannelId].bEnable = FALSE;

//	        if   (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_PCR_STREAM)
//	        {
//	            STErrorCode = STCLKRV_InvDecodeClk(SYS_CLKRVHndl);
//	            if   (STErrorCode   !=   ST_NO_ERROR)
//	            {
//	               mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "Control_Channel:   STCLKRV_InvDecodeClk()   return   %s\r\n",   TBOX_GetDriverErrorText(STErrorCode)));
//	               errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
//	            }
//	            else
//	            {
//	               mTBOX_TRACE((kTBOX_NIV_1,   "Control_Channel:   STCLKRV_InvDecodeClk()   return   %s\r\n",   TBOX_GetDriverErrorText(STErrorCode)));
//	            }
//	        }

	        if(errorCode == kTDAL_DMX_NO_ERROR)
	        {
	/*            TDAL_DMXi_pstChannel[ChannelId].ChannelPid = kTDAL_DMX_ILLEGAL_PID;   *//*Illegal   PID*/
//	            STErrorCode = STPTI_SlotClearPid(TDAL_DMXi_pstChannel[ChannelId].SlotHandle);
//	            if(STErrorCode   !=   ST_NO_ERROR)
//	            {
//	            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
//	                     "STPTI_SlotClearPid(s=0x%x,p=0x%x)   return   %s\n",
//	                     TDAL_DMXi_pstChannel[ChannelId].SlotHandle,
//	                     TBOX_GetDriverErrorText(STErrorCode)));
//	            errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
//	            }
//	            else
//	            {
//	            mTBOX_TRACE((kTBOX_NIV_1,"Control_Channel:   "
//	                     "STPTI_SlotClearPid(s=0x%x,p=0x%x)   returns   %s\n",
//	                     TDAL_DMXi_pstChannel[ChannelId].SlotHandle,
//	                     TDAL_DMXi_pstChannel[ChannelId].ChannelPid,
//	                     TBOX_GetDriverErrorText(STErrorCode)));
//	            }
	        }

	        if(   errorCode == kTDAL_DMX_NO_ERROR   )
	        {
	            switch(   TDAL_DMXi_pstChannel[ChannelId].ChannelStream   )
	            {
	            case   eTDAL_DMX_VIDEO_STREAM:
	               break;

	            case   eTDAL_DMX_AUDIO_STREAM:
//	               if(TDAL_DMXi_pstChannel[ChannelId].BufferHandle   !=   0)
//	               {
//	                 STErrorCode = STPTI_BufferFlush(TDAL_DMXi_pstChannel[ChannelId].BufferHandle);
//	                 if   (STErrorCode   !=   ST_NO_ERROR)
//	                 {
//	                   mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "Control_Channel:   STPTI_BufferFlush()   return   %s\r\n",   TBOX_GetDriverErrorText(STErrorCode)));
//	                   errorCode = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
//	                 }
//	                 else
//	                 {
//	                   mTBOX_TRACE((kTBOX_NIV_1,   "Control_Channel:   STPTI_BufferFlush()   return   %s\r\n",   TBOX_GetDriverErrorText(STErrorCode)));
//	                 }
//	               }
	               break;

	            default:
	               break;
	            }   /*   switch   */

	        }
	      }
	      break;

	      case   eTDAL_DMX_CTRL_RESET:
	      break;

	      default:
	      break;
	   }

	   /*--------------------------------*/
	   /*   flush   the   buffer   in   any   case   */
	   /*--------------------------------*/
	   if   ((errorCode == kTDAL_DMX_NO_ERROR)   &&
	      (TDAL_DMXi_pstChannel[ChannelId].ChannelOutput == eTDAL_DMX_MEMORY)   &&
	      (   (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_SECTION_STREAM)   ||
	      (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_SUBTITLE_STREAM)||
	      (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_TELETEXT_STREAM)   )   )
	   {
//	      STErrorCode = STPTI_BufferFlush(TDAL_DMXi_pstChannel[ChannelId].BufferHandle);
//	      if(STErrorCode   !=   ST_NO_ERROR)
//	      {
//	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Control_Channel:   "
//	               "STPTI_BufferFlush   %d   failed   return   %s\n",
//	            TDAL_DMXi_pstChannel[ChannelId].ChannelStream,
//	            TBOX_GetDriverErrorText(STErrorCode)));
//	      }
	   }
	   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);

	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      mTBOX_TRACE((kTBOX_NIV_1,"Control_Channel:   SUCCEED   [ChannelId=%d]\n",ChannelId));
	   }

	   mTBOX_RETURN(errorCode);
}


/**======================================================================**
 * Function Name : TDAL_DMX_Free_Channel
 *
 * Description   : this function is used to free a channel
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID if ChannelId is invalid
 *             kTDAL_DMX_ERROR_MUTEX for mutex issue
 *             kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL if channel is not used
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT if not able to stop a filter,
 *                             to deallocate a filter or the stream
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Free_Channel(   tTDAL_DMX_ChannelId   ChannelId   )
{
	   tTDAL_DMX_Error   errorCode = kTDAL_DMX_NO_ERROR;

	   mTBOX_FCT_ENTER("TDAL_DMX_Free_Channel");

	   /*------------------------------------*/
	   /*   check   if   the   module   is   initialized   */
	   /*------------------------------------*/
	   if   (!TDAL_DMXi_initialized)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
	            "Free_Channel:   TDAL_DMX   module   has   not   been   initialized\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_NOT_INITIALIZED);
	   }

	   /*-------------------------------------*/
	   /*   Check   channel   Id            */
	   /*-------------------------------------*/
	   if   (ChannelId   <   0   ||   ChannelId   >=   kTDAL_DMXi_MAX_NB_OF_CHANNELS)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Free_Channel:   "
	            "Argument   ChannelId   not   valid\n"));

	      errorCode = kTDAL_DMX_ERROR_INVALID_CHANNEL_ID;
	   }
	   else
	   {
	      mTBOX_TRACE((kTBOX_NIV_1,"Free_Channel:   ChannelId=%d\n",   ChannelId));
	   }

	   /*-------------------------------------*/
	   /*   check   channel   state            */
	   /*-------------------------------------*/
	   if   (errorCode == kTDAL_DMX_NO_ERROR   &&   TDAL_DMXi_pstChannel[ChannelId].bUsed == FALSE)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Free_Channel:   "
	            "channel   (%d)   not   used\n",
	            ChannelId));
	      errorCode = kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL;
	   }
	#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)
	   /*Never   free   the   channel   if   the   VBI   is   on   */
	   if(   (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_TELETEXT_STREAM)   &&
	      (gTDAL_DMX_AllocChannelTeletextOsd)   )
	   {
	      gTDAL_DMX_AllocChannelTeletextOsd = FALSE;
	      mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
	   }
	#endif

//	   if   (TDAL_DMXi_pstChannel[ChannelId].ChannelStream == eTDAL_DMX_VIDEO_STREAM)
//	   {
//	      STPTI_DisableScramblingEvents(TDAL_DMXi_pstChannel[ChannelId].SlotHandle);
//	   }

	#ifdef   __DEBUG_TRACE__
	{
	   char   *pChStreamStr;
	   char   *pChOutStr;

	   switch(TDAL_DMXi_pstChannel[ChannelId].ChannelStream)
	   {
	      case   eTDAL_DMX_VIDEO_STREAM:
	      pChStreamStr="VID";
	      break;
	      case   eTDAL_DMX_AUDIO_STREAM:
	      pChStreamStr="AUD";
	      break;
	      case   eTDAL_DMX_PCR_STREAM:
	      pChStreamStr="PCR";
	      break;
	      case   eTDAL_DMX_TELETEXT_STREAM:
	      pChStreamStr="TXT";
	      break;
	      case   eTDAL_DMX_SUBTITLE_STREAM:
	      pChStreamStr="SUBT";
	      break;
	      case   eTDAL_DMX_SECTION_STREAM:
	      pChStreamStr="SECT";
	      break;
	      case   eTDAL_DMX_TP_STREAM:
	      pChStreamStr="TP";
	      break;
	      default:
	      pChStreamStr="??";
	      break;
	   }

	   switch(TDAL_DMXi_pstChannel[ChannelId].ChannelOutput)
	   {
	      case   eTDAL_DMX_DECODER:
	      pChOutStr = "DEC";
	      break;
	      case   eTDAL_DMX_MEMORY:
	      pChOutStr = "MEM";
	      break;
	      default:
	      pChOutStr = "??";
	      break;
	   }

	   mTBOX_TRACE((kTBOX_NIV_1,"Free_Channel:   stream=%s   out=%s\n",
	            pChStreamStr,
	            pChOutStr));
	}
	#endif

	   /*   disable   the   channel   if   enabled   before   freeing   associted   resources   */
	   if   ((errorCode == kTDAL_DMX_NO_ERROR)   &&
	      (TDAL_DMXi_pstChannel[ChannelId].bEnable == TRUE))
	   {
	      errorCode = TDAL_DMX_Control_Channel(ChannelId,   eTDAL_DMX_CTRL_DISABLE);
	   }
	   /*-------------------------------------*/
	   /*   Lock   Protection   mutex        */
	   /*-------------------------------------*/
	   mLockAccess(TDAL_DMXi_pSectionTableAccess);

	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      errorCode = TDAL_DMXi_FreeChannelAndAssociatedRessources(ChannelId);
	   }

	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      TDAL_DMXi_pstChannel[ChannelId].bEnable = FALSE;
	      TDAL_DMXi_pstChannel[ChannelId].bUsed   = FALSE;
	      TDAL_DMXi_pstChannel[ChannelId].ChannelPid = kTDAL_DMX_ILLEGAL_PID;

	      mTBOX_TRACE((kTBOX_NIV_1,"Free_Channel:   SUCCEED   [ChannellId=%d]\n",ChannelId));
	   }

	   mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
	   mTBOX_RETURN(errorCode);

}



/**======================================================================**
 * Function Name : TDAL_DMXi_LookForFreeChannel
 *
 * Description   : this function return a free channel
 *
 * Return       : channelId if found a free channel
 *             kTDAL_DMX_ILLEGAL_CHANNEL otherwise
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_ChannelId   TDAL_DMXi_LookForFreeChannel(   void   )
{
	   int   i;
	   tTDAL_DMX_ChannelId   channelIdFound = kTDAL_DMX_ILLEGAL_CHANNEL;

	   for   (i=0;   i< kTDAL_DMXi_MAX_NB_OF_CHANNELS;   i++)
	   {
	      if   ((TDAL_DMXi_pstChannel[i].bUsed == FALSE)   &&
	      (TDAL_DMXi_pstChannel[i].bWaitingForDeletion == FALSE))
	      {
			  channelIdFound = i;
			  break;
	      }
	   }

	   return   (channelIdFound);
}



/**======================================================================**
 * Function Name : TDAL_DMXi_FreeChannelAndAssociatedRessources
 *
 * Description   : free channels resoources and send a message to the deletion task
 *
 * Comment       : the deletion task free real hardware ressources linked to the channel
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMXi_FreeChannelAndAssociatedRessources(
               tTDAL_DMX_ChannelId   channelId
)   
{
	   tTDAL_DMX_Error      errorCode = kTDAL_DMX_NO_ERROR;
	   int            i;

	   mTBOX_FCT_ENTER("TDAL_DMXi_FreeChannelAndAssociatedRessources");

//	   if   ((TDAL_DMXi_pstChannel[channelId].ChannelOutput == eTDAL_DMX_MEMORY)   &&
//	      ((TDAL_DMXi_pstChannel[channelId].ChannelStream == eTDAL_DMX_SECTION_STREAM)   ||
//	      (TDAL_DMXi_pstChannel[channelId].ChannelStream == eTDAL_DMX_SUBTITLE_STREAM)||
//	      (TDAL_DMXi_pstChannel[channelId].ChannelStream == eTDAL_DMX_TELETEXT_STREAM)))
	   {   /*   --------------------------------------------------------   */
	      /*   Free   all   filters   and   ressources   of   the   specified   channel   */
	      /*   --------------------------------------------------------   */
	for (i=0;(i < kTDAL_DMXi_MAX_NB_OF_FILTERS) && (errorCode == kTDAL_DMX_NO_ERROR);i++)
	      {
			  if   (TDAL_DMXi_pstFilter[i].AssociatedChannelId == channelId)
			  {
			mTBOX_TRACE((kTBOX_NIV_1,"FreeChannelAndAssociatedRessources:" "Call FreeFilterAndAssociatedRessources(%d)\n",i));
				errorCode = TDAL_DMXi_FreeFilterAndAssociatedRessources(i);
			  }
	      }


	}   
	/*   end   if   SECTION   stream   and   MEMORY   output   */
//	   else   if((TDAL_DMXi_pstChannel[channelId].ChannelOutput == eTDAL_DMX_DECODER)   ||
//	      ((TDAL_DMXi_pstChannel[channelId].ChannelOutput == eTDAL_DMX_MEMORY)   &&
//	        (TDAL_DMXi_pstChannel[channelId].ChannelStream   !=   eTDAL_DMX_SECTION_STREAM)))
//	   {
//
//	   }   /*   end   if   DECODER   output   */

	   /*-----------------------------------------------------------------*/
	   /*   RAZ   some   channel   fields,   the   other   will   be   RAZ   in   deletion   task   */
	   /*-----------------------------------------------------------------*/
	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
	      TDAL_DMXi_pstChannel[channelId].bUsed   = FALSE;
	      TDAL_DMXi_pstChannel[channelId].Callback = NULL;
	      TDAL_DMXi_pstChannel[channelId].bEnable = FALSE;
	      TDAL_DMXi_pstChannel[channelId].ChannelPid = kTDAL_DMX_ILLEGAL_PID;
	   }

	   if(errorCode == kTDAL_DMX_NO_ERROR)
	   {
		mTBOX_TRACE((kTBOX_NIV_1,"FreeChannelAndAssociatedRessources:   SUCCEED   chId=%d\n",channelId));
	}

	mTBOX_RETURN(errorCode);

}

