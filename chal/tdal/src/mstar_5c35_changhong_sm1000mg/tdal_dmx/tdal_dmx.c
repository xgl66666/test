/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMX
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_dmx/tdal_dmx.c $
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

#include "tbox.h"

#include "apiDMX.h"
#include "drvDSCMB.h"
#include "MsMemory.h"


/********************************************************
   *   Defines                        *
   ********************************************************/


/********************************************************
   *   Macros                        *
   ********************************************************/
mTBOX_SET_MODULE(eTDAL_DMX);



/*   global   variable   on   the   init   state   */
bool   TDAL_DMXi_initialized = FALSE;

/*   Mutex   of   channel   and   filter   */
TDAL_mutex_id   TDAL_DMXi_pSectionTableAccess;

/*   list   of   channels   for   all   pti   */
tTDAL_DMX_Channel_t   TDAL_DMXi_pstChannel[kTDAL_DMXi_MAX_NB_OF_CHANNELS];

/*   list   of   filters   for   all   pti   */
tTDAL_DMX_Filter_t   TDAL_DMXi_pstFilter[kTDAL_DMXi_MAX_NB_OF_FILTERS];

#if   defined(USE_TDAL_TS_SOURCE_SINK)
tTDAL_DMX_DemuxId   TDAL_DMXi_pstDemuxId[kTDAL_DMX_NB_DEMUX] = {eTDAL_DMX_DEMUX0, eTDAL_DMX_DEMUX1};
#else
tTDAL_DMX_DemuxId   TDAL_DMXi_pstDemuxId[kTDAL_DMX_NB_DEMUX] = {eTDAL_DMX_DEMUX0   };
#endif

TDAL_queue_id  TDAL_DMXi_SectionQueue = -1;

tTDAL_DMXi_SectionTask      TDAL_DMXi_SectionTaskArr[kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT];

MS_U8 u8TSPFirmware[] = {
    #include "fwTSP.dat"
};

MS_S32      TDAL_DMXi_sections_partition;
TDAL_mutex_id   TDAL_DMXi_sections_partition_mutex = NULL;

/*   memory   to   be   used   */
unsigned   char   TDAL_DMXi_sections_memory[kTDAL_DMX_SECTIONS_MEM_SIZE];



bool TDAL_DMXm_SetDmxFlow(void)
{
    mTBOX_FCT_ENTER("TDAL_DMXm_SetDmxFlow");

#if 1 // internal demod
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_DEMOD ,FALSE,TRUE,TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "switch to demod fail\n"));
        mTBOX_RETURN(false);
    }
#else // playcard
    MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_EXT_INPUT0 ,FALSE, TRUE, TRUE);
#endif

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PVR, DMX_FLOW_INPUT_DEMOD ,FALSE,TRUE,TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"switch to demod fail\n"));
        mTBOX_RETURN(false);
    }

    mTBOX_RETURN(true);

}


tTDAL_DMX_Error   TDAL_DMX_Init(   void   )
{
	int i;
	DMX_FILTER_STATUS bRet;
    MS_BOOL bfRet;

	mTBOX_FCT_ENTER("TDAL_DMX_Init");

	   /*--------------------------------------------*/
	   /*   check   if   the   module   is   already   initialized   */
	   /*--------------------------------------------*/
   if   (TDAL_DMXi_initialized)
   {
	  mTBOX_TRACE((kTBOX_NIV_1,"Init:   module   already   initialized\n"));
	  mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
   }

	bRet = MApi_DMX_CMD_Run( 0xF0010001 , 1, 0 , NULL );
	mTBOX_ASSERT(bRet == DMX_FILTER_STATUS_OK);

	bRet = MApi_DMX_Init();
	mTBOX_ASSERT(bRet == DMX_FILTER_STATUS_OK);
	{   // set VQ for TSP, set FW/VQ for TSP2
		DMX_TSPParam    param;
		memset(&param,0,sizeof(param));
		memcpy((void*)MS_PA2KSEG1((MS_U32)TSP_FW_BUF_ADR/*u8TSPFirmware*/) , u8TSPFirmware, sizeof(u8TSPFirmware));
		bRet = MApi_DMX_SetFW(((MS_PHYADDR)TSP_FW_BUF_ADR/*u8TSPFirmware*/), sizeof(u8TSPFirmware));
        if (DMX_FILTER_STATUS_OK != bRet)
        {
            printf("\n""\033[1;31m""[FUN][%s()@%04d] MApi_DMX_SetFW fail:%x   ""\033[m""\n", __FUNCTION__, __LINE__, bRet);
        }
		param.phyFWAddr = (MS_PHYADDR) TSP_FW_BUF_ADR;
		param.u32FWSize = sizeof(u8TSPFirmware);
		param.phyVQAddr = (MS_PHYADDR)(TSP_VQ_BUF_ADR);
		param.u32VQSize = TSP_FW_BUF_LEN;
		mTBOX_TRACE((kTBOX_NIV_1, "FWAddr = %08lX , Size = %08lX , VQAddr = %08lX , VQSize = %08lX \n",param.phyFWAddr,param.u32FWSize,param.phyVQAddr,param.u32VQSize));
		if (MApi_DMX_TSPInit(&param) != DMX_FILTER_STATUS_OK )
		{
            printf("\n""\033[1;31m""[FUN][%s()@%04d] MApi_DMX_TSPInit fail""\033[m""\n", __FUNCTION__, __LINE__);
		   return kTDAL_DMX_ERROR_MUTEX ;
		}
	}
	bRet = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    if (DMX_FILTER_STATUS_OK != bRet)
    {
        printf("\n""\033[1;31m""[FUN][%s()@%04d]  MApi_DMX_AVFifo_Reset fail:%x  ""\033[m""\n", __FUNCTION__, __LINE__, bRet);    
    }
	bRet = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    if (DMX_FILTER_STATUS_OK != bRet)
    {
        printf("\n""\033[1;31m""[FUN][%s()@%04d]  MApi_DMX_AVFifo_Reset fail:%x  ""\033[m""\n", __FUNCTION__, __LINE__, bRet);    
    }
	bRet = MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);
    if (DMX_FILTER_STATUS_OK != bRet)
    {
        printf("\n""\033[1;31m""[FUN][%s()@%04d]  MApi_DMX_AVFifo_Reset fail:%x  ""\033[m""\n", __FUNCTION__, __LINE__, bRet);    
    }
	bfRet = MDrv_DSCMB_Init();
    if ( TRUE != bfRet)
    {
        printf("\n""\033[1;31m""[FUN][%s()@%04d] MDrv_DSCMB_Init fail:%d   ""\033[m""\n", __FUNCTION__, __LINE__, bfRet);       
    }
    MDrv_DSCMB2_SetDefaultCAVid(0,0x02);
    MDrv_AESDMA_SetDefaultCAVid(0x02);
	if (TDAL_DMXm_SetDmxFlow() == false)
	{
	    mTBOX_RETURN(kTDAL_DMX_ERROR_MUTEX);
	}
#if 1 // new flow carson
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_AUDIO_PKT, FALSE);
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_VIDEO_PKT, FALSE);
#endif

	TDAL_CreateMutex(&TDAL_DMXi_pSectionTableAccess);
	if   (TDAL_DMXi_pSectionTableAccess == NULL)
	{
	   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Init:   Mutex   'SectionTableAccess'   failed\n"));
	   mTBOX_RETURN(kTDAL_DMX_ERROR_MUTEX);
	}

	TDAL_CreateMutex(&TDAL_DMXi_sections_partition_mutex);
	mTBOX_ASSERT(TDAL_DMXi_sections_partition_mutex != NULL);

	TDAL_DMXi_sections_partition = MsOS_CreateMemoryPool(
			kTDAL_DMX_SECTIONS_MEM_SIZE,
			0x0, /* Min allocation */
			(void*)&TDAL_DMXi_sections_memory,
			E_MSOS_FIFO,
			"DMX_Pool");

	if (TDAL_DMXi_sections_partition < 0)
	{
		  mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Init:   Demux partition initialization failed\n"));
		  mTBOX_RETURN(kTDAL_DMX_ERROR_MUTEX);
	}
		   /*---------------------------*/
		   /*   Initialize   channel   tables   */
		   /*---------------------------*/
		   for   (i=0; i<kTDAL_DMXi_MAX_NB_OF_CHANNELS   ; i++)
		   {
		      TDAL_DMXi_pstChannel[i].DemuxId   = -1;
		      TDAL_DMXi_pstChannel[i].ChannelId = (tTDAL_DMX_ChannelId)   i;
		      TDAL_DMXi_pstChannel[i].bUsed   = FALSE;
		      TDAL_DMXi_pstChannel[i].bEnable   = FALSE;
		      TDAL_DMXi_pstChannel[i].Callback   = NULL;
		      TDAL_DMXi_pstChannel[i].ChannelPid = kTDAL_DMX_ILLEGAL_PID;
		      TDAL_DMXi_pstChannel[i].DemuxHandle = 0;
		      TDAL_DMXi_pstChannel[i].StreamHandle = 0;
		   }

		   /*---------------------------*/
		   /*   Initialize   filter   tables   */
		   /*---------------------------*/
	for (i=0; i < kTDAL_DMXi_MAX_NB_OF_FILTERS; i++)
		   {
		      TDAL_DMXi_pstFilter[i].FilterId       = (tTDAL_DMX_FilterId)i;
		      TDAL_DMXi_pstFilter[i].AssociatedChannelId = kTDAL_DMX_ILLEGAL_CHANNEL;
		      TDAL_DMXi_pstFilter[i].bUsed       = FALSE;
		      TDAL_DMXi_pstFilter[i].bEnable       = FALSE;
		      TDAL_DMXi_pstFilter[i].bWaitingEnable   = FALSE;
		      TDAL_DMXi_pstFilter[i].bSet       = FALSE;
		      TDAL_DMXi_pstFilter[i].Tag          = 0;
		      TDAL_DMXi_pstFilter[i].FilterHandle   = 0;
		      TDAL_DMXi_pstFilter[i].Filter       = NULL;
		      TDAL_DMXi_pstFilter[i].PositiveMask       = NULL;
		      TDAL_DMXi_pstFilter[i].NegativeMask       = NULL;
		   }
#if MSTAR_QUEUE
           TDAL_DMXi_SectionQueue = MsOS_CreateQueue(
                          NULL,         //It is useless now, can pass NULL.
                          NULL,               // queue size (byte unit) : now fixed as 10 * u32MessageSize
                          E_MSG_FIXED_SIZE,   // E_MSG_VAR_SIZE has not been supported yet
                          sizeof(tTDAL_DMX_FilterId),     // message size (byte unit) for E_MSG_FIXED_SIZE
                          E_MSOS_FIFO,        // E_MSOS_FIFO suspended in FIFO order
                          "DMX Queue");      // queue name
#else
		   TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(tTDAL_DMX_FilterId), &TDAL_DMXi_SectionQueue);
#endif
           if (TDAL_DMXi_SectionQueue < 0)
          {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Init:   Demux section queue initialization failed\n"));
                mTBOX_RETURN(kTDAL_DMX_ERROR_MUTEX);
          }

		   for (i = 0; i < kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT; i++)
		   {
			   char threadName[30];

			   mTBOX_TRACE((kTBOX_NIV_1, "Creating DMX task %d\n", i));

			   sprintf(threadName, "DMX Thread %d", i);

			   TDAL_DMXi_SectionTaskArr[i].ShouldExit = FALSE;
			   TDAL_DMXi_SectionTaskArr[i].Finished = FALSE;

			   TDAL_DMXi_SectionTaskArr[i].TaskHandle = TDAL_CreateTask(eTDAL_PRIORITY_NORMAL,
					    threadName,
					    TDAL_DMXi_SectionTaskArr[i].TaskStack,
					    sizeof(TDAL_DMXi_SectionTaskArr[i].TaskStack),
					    TDAL_DMXi_SectionTask,
				        (void *) i
				        );

			   if (TDAL_DMXi_SectionTaskArr[i].TaskHandle < 0)
			   {
				   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not create section receive task %d", i));
				   mTBOX_RETURN(kTDAL_DMX_ERROR_MUTEX);
			   }
		   }

	   /*---------------------------------------------------*/
	   /*   the   module   is   initialized.   Update   global   variable   */
	   /*---------------------------------------------------*/
	   TDAL_DMXi_initialized = TRUE;

	   mTBOX_TRACE((kTBOX_NIV_1,"Init:   SUCCEED\n"));
	   mTBOX_RETURN(kTDAL_DMX_NO_ERROR);

}


/**======================================================================**
 * Function Name : TDAL_DMX_Terminate
 *
 * Description   : this function deinitialises the demux filter module
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if initialised successfully
 *             kTDAL_DMX_ERROR_MUTEX if mutex can't be deleted
 *             kTDAL_DMX_ERROR_UNABLE_TO_GRANT otherwise
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_Terminate(   void   )
{
	int i;
	MS_BOOL bRet;
	tTDAL_DMX_Error error = kTDAL_DMX_NO_ERROR;
	DMX_FILTER_STATUS dmxStatus;

	mTBOX_FCT_ENTER("TDAL_DMX_Terminate");

	/*---------------------------------------*/
	/*   check   if   the   module   is   already   closed   */
	/*---------------------------------------*/

	if (!TDAL_DMXi_initialized)
	{
		mTBOX_TRACE((kTBOX_NIV_1,"Terminate:   TDAL_DMX   module   already   closed\n"));
		mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
	}

	mTBOX_TRACE((kTBOX_NIV_1,"Terminate:   begins\n"));

	/*-------------------------------------------------*/
	/*   Stopping threads                              */
	/*-------------------------------------------------*/

	for (i = 0; i < kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT; i++)
	{
		TDAL_DMXi_SectionTaskArr[i].Finished = FALSE;
		TDAL_DMXi_SectionTaskArr[i].ShouldExit = TRUE;
	}

	for (i = 0; i < kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT; i++)
	{
		mTBOX_TRACE((kTBOX_NIV_1, "Waiting threads to finish\n"));
		if (TDAL_DMXi_SectionTaskArr[i].Finished != TRUE)
		{
			MsOS_DelayTask(100);
		}
	}

	/*   Lock   Protection   Table   Semaphore   */
	mLockAccess(TDAL_DMXi_pSectionTableAccess);

	/*-------------------------------------------------*/
	/*   deallocate   channels   and   filters   associated   used   */
	/*-------------------------------------------------*/
	if (error == kTDAL_DMX_NO_ERROR)
	{
		for (i = 0; i < kTDAL_DMXi_MAX_NB_OF_CHANNELS; i++)
		{
			//mTBOX_TRACE((kTBOX_NIV_1, "TDAL_DMXi_pstChannel[%d].bUsed=%d\n",i,TDAL_DMXi_pstChannel[i].bUsed));
			if (TDAL_DMXi_pstChannel[i].bUsed == TRUE)
			{
				/*   Free   it   */
				if (TDAL_DMXi_FreeChannelAndAssociatedRessources(TDAL_DMXi_pstChannel[i].ChannelId) != kTDAL_DMX_NO_ERROR)
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Terminate:   Unable   to   free   channel   %d   and   ressource\n", i));
					error = kTDAL_DMX_ERROR_UNABLE_TO_GRANT;
				}
			}
		}
	}

	for (i = 0; i < kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT; i++)
	{
		MS_BOOL queueSuccess;
		tTDAL_DMX_FilterId filterId = (tTDAL_DMX_FilterId)-1;
		
		TDAL_DMXi_SectionTaskArr[i].ShouldExit = TRUE;
		
#if MSTAR_QUEUE
            queueSuccess = MsOS_SendToQueue(TDAL_DMXi_SectionQueue, &filterId, sizeof(tTDAL_DMX_FilterId), 0);
#else
		queueSuccess = TDAL_Enqueue(TDAL_DMXi_SectionQueue, &filterId);
#endif
	}


	for (i = 0; i < kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT; i++)
	{
		TDAL_DeleteTask(TDAL_DMXi_SectionTaskArr[i].TaskHandle);
		TDAL_DMXi_SectionTaskArr[i].TaskHandle = NULL;
	}

#if MSTAR_QUEUE	
    MsOS_DeleteQueue(TDAL_DMXi_SectionQueue);
#else
	TDAL_DeleteQueue(TDAL_DMXi_SectionQueue);
#endif
	TDAL_DMXi_SectionQueue = -1;

	/*----------------------------------------------*/
	/*   Delete   memory   partition            */
	/*----------------------------------------------*/

	bRet = MsOS_DeleteMemoryPool(TDAL_DMXi_sections_partition);
	mTBOX_ASSERT(bRet == TRUE);

	dmxStatus = MApi_DMX_Exit();
	mTBOX_ASSERT(dmxStatus == DMX_FILTER_STATUS_OK);

	/*-----------------------------------------------------*/
	/*   Protection   table   semaphore   deletion            */
	/*-----------------------------------------------------*/
	mUnLockAccess(TDAL_DMXi_pSectionTableAccess);
	
	if (TDAL_DMXi_pSectionTableAccess != NULL)
	{
	TDAL_DeleteMutex(TDAL_DMXi_pSectionTableAccess);
		TDAL_DMXi_pSectionTableAccess = NULL;
	}

	/*----------------------------------------------*/
	/*   the   module   is   closed.   Update   global   variable   */
	/*----------------------------------------------*/
	TDAL_DMXi_initialized = FALSE;

	mTBOX_TRACE((kTBOX_NIV_1,"Terminate:   End   SUCCEED\n"));

	mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
}


/******************************************************************************
   *   Function   Name :   TDAL_DMX_StreamHandleGet
   *
   *   Description :   this   function   is   used   to   get   The   streamId   of   a   channel   channel
   *
   *
   *   Side   effects :
   *      return   kTDAL_DMX_NO_ERROR   if   all   is   correct
   *            kSFILTER_ERROR_INVALID_CHANNEL_ID   if   no   channel   is   invalid
   *            or   channel   not   used   or   bad   PID
   *            kTDAL_DMX_ERROR_BAD_PARAMETERS   for   bad   argument   ctrl
   *            kTDAL_DMX_ERROR_MUTEX   for   mutex   issue
   *            kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL   if   channel   is   not   used
   *
   *   Comment   :
   *
   *****************************************************************************/
tTDAL_DMX_Error   TDAL_DMX_StreamHandleGet   (tTDAL_DMX_ChannelId   ChannelId, uint32_t*   pStreamHandle)
{

	   mTBOX_FCT_ENTER("TDAL_DMX_StreamHandleGet");

	   /*   Check   channel   */
	   if   (ChannelId < 0 || ChannelId >= kTDAL_DMXi_MAX_NB_OF_CHANNELS)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DMX_Control_Channel :   ChannelId   not   valid\n"));
	      mTBOX_RETURN(kTDAL_DMX_ERROR_INVALID_CHANNEL_ID);
	   }

	   *pStreamHandle = (uint32_t   )   TDAL_DMXi_pstChannel[ChannelId].StreamHandle;

	   mTBOX_RETURN(kTDAL_DMX_NO_ERROR);
}

/**======================================================================**
 * Function Name : TDAL_DMX_GetDemuxId
 *
 * Description   : this function is used to give te list of demuxId
 *             which must be used by client
 *
 * Side effects  :
 *
 * Return       : kTDAL_DMX_NO_ERROR if all is correct
 *             kTDAL_DMX_ERROR_BAD_PARAMETERS
 *
 * Comment       :
 *
 **======================================================================**/
tTDAL_DMX_Error   TDAL_DMX_GetDemuxId(   uint8_t   *        nbDemuxId,
                     tTDAL_DMX_DemuxId   **   demuxId)
{
	   tTDAL_DMX_Error   Error = kTDAL_DMX_NO_ERROR;
	   mTBOX_FCT_ENTER("TDAL_DMX_GetDemuxId");

	   if(nbDemuxId == NULL || demuxId == NULL)
	   {
	      Error = kTDAL_DMX_ERROR_BAD_PARAMETERS;
	   }

	   if(Error == kTDAL_DMX_NO_ERROR)
	   {
	      *nbDemuxId = kTDAL_DMX_NB_DEMUX;
	      *demuxId = TDAL_DMXi_pstDemuxId;
	   }

	   mTBOX_RETURN(Error);
}


/**======================================================================**
 * Function Name : TDAL_DMXi_Section_Malloc
 *
 * Description   : Allocate 'memorySize' bytes in the partition reserved
 *             for sections
 *
 * Return       : a pointer to the allocated memory
 *
 * Comment       :
 *
 **======================================================================**/
void*   TDAL_DMXi_Section_Malloc(   unsigned   long   memorySize   )
{
	   void   *ptr = NULL;

	   if   (memorySize == 0)
	   {
	      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Malloc:   Failed, memorySize = 0\r\n"));
	      return   NULL;
	   }

	   mLockAccess(TDAL_DMXi_sections_partition_mutex);
	   ptr = MsOS_AllocateMemory((MS_U32) memorySize, TDAL_DMXi_sections_partition);
	   mUnLockAccess(TDAL_DMXi_sections_partition_mutex);

	   return   ptr;
}


/**======================================================================**
 * Function Name : TDAL_DMXi_Section_Free
 *
 * Description   : Free a memory zone allocated with TDAL_DMXi_Section_Malloc
 *
 * Return       : void
 *
 * Comment       :
 *
 **======================================================================**/
void   TDAL_DMXi_Section_Free(   void   *   buffer   )
{
   if   (buffer == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Free:   Failed, buffer = NULL   \n"));
      return;
   }

   mLockAccess(TDAL_DMXi_sections_partition_mutex);
   MsOS_FreeMemory(buffer,TDAL_DMXi_sections_partition);
   mUnLockAccess(TDAL_DMXi_sections_partition_mutex);

   return;
}


#if   defined(__DEBUG_TRACE__) && defined(TDAL_DMX_DBG_TRC_PCK_CNT)
void   TDAL_DMXi_DebugPacketCount   (   tTDAL_DMX_ChannelId   ChannelId)
{
}
#endif   /*   __DEBUG_TRACE__   */


