/******************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS_SOURCE  -  TS_SOURCE driver
 *
 * FILE NAME:   TDAL_TS_SOURCE.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_source.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION - This driver is divided in 3 parts :
 *            - TDAL_TS_ROUTE  : Connect a TS source and a demux
 *            - TDAL_TS_SINK   : Inject a TS to a demux or to an external sink
 *            - TDAL_TS_SOURCE : Extract a TS from a demux or an external source
 *
 *****************************************************************************/
#define USE_TDAL_TS_SOURCE_SINK
#if   defined(USE_TDAL_TS_SOURCE_SINK)


/************************************************************************
   *   Includes
   ***********************************************************************/

#include "string.h"

#include "crules.h"
#include "tbox.h"

#include "tdal_common.h"

#include "tdal_ts.h"
#include "tdal_ts_buffer_p.h"
#include "tdal_ts_source_p.h"


/************************************************************************
   *   Defines
   ***********************************************************************/
#define   kTDAL_TSSOURCE_MONITORED_EVENT   STPTI_EVENT_BUFFER_OVERFLOW_EVT


/************************************************************************
   *   Macros
   ***********************************************************************/
mTBOX_SET_MODULE(eTDAL_TS_SOURCE);


/************************************************************************
   *   Typedefs
   ***********************************************************************/

/************************************************************************
   *   Global   Variables   (GLOBAL/IMPORT)
   ***********************************************************************/
/************************************************************************
   *   Local   Module   Variables   (MODULE)
   ***********************************************************************/


/************************************************************************
   *   Local   File   Variables   (LOCAL)
   ***********************************************************************/


/************************************************************************
   *   Exported   Functions   Definitions   (GLOBAL)
   ***********************************************************************/


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSOURCE_Init
 *
 * Description :   Initializes the TDAL_TSSOURCE driver
 *
 * Side effects :
 *
 * Comment :
   ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Init(   void   )
{
   return   eTDAL_TSSOURCE_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSOURCE_Term
 *
 * Description :   Terminates the TDAL_TSSOURCE driver
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Term(   void   )
{
   return   eTDAL_TSSOURCE_NO_ERROR;
}


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSOURCE_Open
 *
 * Description :   Initialize a new extraction
 *
 * Side effects :  It allocates an Extraction_s structure and all stuff around
 *             If an error occurs, it restores the previous state.
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Open(   tTDAL_TSSOURCE_Handle   *   p_handle,
                            tTDAL_TSSOURCE_OpenParam   *   p_oprm   )
{
    return   eTDAL_TSSOURCE_NO_ERROR;
}



/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSOURCE_Config
 *
 * Description :   Configure an extraction
 *
 * Side effects :  It STPTI_Open, STPTI_BufferAllocate.
 *             If an error occurs in these functions, it restores
 *             the previous state.
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Config(   tTDAL_TSSOURCE_Handle      handle,
                              tTDAL_TSSOURCE_ConfigParam   *   p_cfgprm   )
{
    return   eTDAL_TSSOURCE_NO_ERROR;
}

/**----------------------------------------------------------------------
 * Function Name :   TDAL_TSSOURCE_Close
 *
 * Description :   Closes an extraction.
 *
 * Side effects :   It calls TDAL_TSSOURCE_Stop, TDAL_TSSOURCE_RemovePid,
 *        STPTI_BufferDeallocate, STPTI_Close and TDAL_Free.
 *        If an error occurs in these functions, it continues
 *        the execution and returns an error code.
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Close(   tTDAL_TSSOURCE_Handle   handle   )
{
    return eTDAL_TSSOURCE_NO_ERROR;
}




/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSOURCE_AddPid
 *
 * Description :   Adds a PID to an extraction
 *
 * Side effects :   It modifies '*p_extr' structure and calls
 *            STPTI_SlotAllocate, STPTI_SloSetPid and
 *            STPTI_SlotLinkToBuffer. If an error occurs in these
 *            functions, it restores the previous state.
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_AddPid(   tTDAL_TSSOURCE_Handle   handle,
                        tTDAL_TSSOURCE_Pid      pid   )
{
    return eTDAL_TSSOURCE_NO_ERROR;
}

/**----------------------------------------------------------------------
   *   Function   Name   :   TDAL_TSSOURCE_RemovePid
   *
   *   Description   :   Removes   a   PID   from   the   extraction
   *
   *   Side   effects   :   It   modifies   '*p_extr'   structure   and   calls
   *      STPTI_SlotUnLink, STPTI_SlotClearPid   and   STPTI_
   *      STPTI_SlotDeAllocate.   If   an   error   occurs   in   these
   *      functions, it   continues   the   execution   and   returns
   *      an   error   code.
   *
   *   Comment   :
   ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_RemovePid(   tTDAL_TSSOURCE_Handle   handle,
                            tTDAL_TSSOURCE_Pid      pid   )
{
    return eTDAL_TSSOURCE_NO_ERROR;

}


/**----------------------------------------------------------------------
 * Function Name : TDAL_TSSOURCE_Start
 *
 * Description :   Starts producing
 *
 * Side effects :  This function spawns an extracting thread.
 *             It calls STPTI_SlotLinkToBuffer,
 *             semaphore_create_fifo and task_create. If an
 *             error occurs in OS functions, it tries to restore
 *             the previous state. Slot-buffer linking failure is
 *             just warned by mTBOX_Trace.
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Start(   tTDAL_TSSOURCE_Handle   handle   )
{
    return eTDAL_TSSOURCE_NO_ERROR;
}


/**----------------------------------------------------------------------
   *   Function   Name   :   TDAL_TSSOURCE_Stop
   *
   *   Description   :   Stop   the   extraction.
   *
   *   Side   effects   :   It   stops   the   extracting   thread   and   unlink   slots   and
   *      buffer.   It   calls   semaphore_wait, semaphore_delete   and
   *      STPTI_SlotUnLink.   If   an   error   occurs, it   tries   to   continue
   *      and   returns   an   error   code.
   *
   *   Comment   :
   ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Stop(   tTDAL_TSSOURCE_Handle   handle   )
{
    return eTDAL_TSSOURCE_NO_ERROR;
}


/**----------------------------------------------------------------------
   *   Function   Name   :   TDAL_TSSOURCE_SetTimestampingCallback
   *
   *   Description   :   Set   the   Timestamping   consumer   call-back
   *
   *   Side   effects   :
   *
   *   Comment   :
   ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_SetTimestampingCallback(
                 tTDAL_TSSOURCE_Handle            handle,
                 tTDAL_TSSOURCE_TimestampingCallbackFct   timestamping_callback,
                 void   *                   client_tag
)   {
    return eTDAL_TSSOURCE_NO_ERROR;

}




/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSOURCE_NotifyProduction
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_NotifyProduction(
                   tTDAL_TSSOURCE_Handle   handle,
                   uint32_t        produced_packets,
                   uint32_t        notification_level
)   {
    return eTDAL_TSSOURCE_NO_ERROR;
}


/**----------------------------------------------------------------------
   *   Function   Name   :   p_TDAL_TSSOURCE_Pause
   *
   *   Description   :   Pause   TS   source
   *
   *   Side   effects   :
   *
   *   Comment   :
   ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Pause(
                 tTDAL_TSSOURCE_Handle      handle,
                 bool               reinit_pointer
                 )
{
    return eTDAL_TSSOURCE_NO_ERROR;
}




/**----------------------------------------------------------------------
 * Function Name : p_TDAL_TSSOURCE_Resume
 *
 * Description :   Resume TS source
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   tTDAL_TSSOURCE_Error   TDAL_TSSOURCE_Resume(
                 tTDAL_TSSOURCE_Handle      handle
                 )
{
   return eTDAL_TSSOURCE_NO_ERROR;
}


/* Comment :
 ---------------------------------------------------------------------**/
#if   0//PATCH_FOR_BUGGED_STPTI_SIGNAL
LOCAL   void   p_TDAL_TSSOURCE_STPTI_UnregisterEvents(   uint8_t   pti_name_index   )
{
   ST_ErrorCode_t            st_err;
   uint8_t               local_nb_subs;

   if   (stpti_evt_handle == (STEVT_Handle_t)NULL)
   {
      return;
   }

   /*   test   if   event(s)   has   several   subscribers   */
   TDAL_LockMutex(   stpti_evt_mutex   );
   local_nb_subs = stpti_evt_nb_subscribers[pti_name_index];
   stpti_evt_nb_subscribers[pti_name_index]--;
   if   (local_nb_subs   >   1)
   {
      TDAL_UnlockMutex(   stpti_evt_mutex   );
      return;
   }
   TDAL_UnlockMutex(   stpti_evt_mutex   );

   /*   else   */
   st_err = STEVT_UnsubscribeDeviceEvent(   stpti_evt_handle,
                        SYS_PtiName[pti_name_index],
                        kTDAL_TSSOURCE_MONITORED_EVENT   );
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "Event   [0x%x]   unsubscribing   failed   :   st_err=0x%x\r\n",
            kTDAL_TSSOURCE_MONITORED_EVENT   , st_err));
   }

   st_err = STPTI_DisableErrorEvent(   SYS_PtiName[pti_name_index]   ,
                     kTDAL_TSSOURCE_MONITORED_EVENT   );
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "Event   [0x%x]   not   successfully   disabled   for   \"%s\"   (st_err=0x%x)\r\n",
            kTDAL_TSSOURCE_MONITORED_EVENT,
            SYS_PtiName[pti_name_index],
            st_err   ));
   }
#if   defined(TSSOURCE_STPTI_INDEX_USE)
#if   1   /*   GS   debug   _test   STPTI_EVENT_INDEX_MATCH_EVT   */
   st_err = STEVT_UnsubscribeDeviceEvent(   stpti_evt_handle,
                        SYS_PtiName[pti_name_index],
                        STPTI_EVENT_INDEX_MATCH_EVT   );
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "Event   [0x%x]   unsubscribing   failed   :   st_err=0x%x\r\n",
            STPTI_EVENT_INDEX_MATCH_EVT   , st_err));
   }

   st_err = STPTI_DisableErrorEvent(   SYS_PtiName[pti_name_index]   ,
                     STPTI_EVENT_INDEX_MATCH_EVT   );
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "Event   [0x%x]   not   successfully   disabled   for   \"%s\"   (st_err=0x%x)\r\n",
            STPTI_EVENT_INDEX_MATCH_EVT,
            SYS_PtiName[pti_name_index],
            st_err   ));
   }
#endif
#endif
}
#endif


/**----------------------------------------------------------------------
 * Function Name : p_TDAL_TSSOURCE_STPTI_EventCallback
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
#if   PATCH_FOR_BUGGED_STPTI_SIGNAL

LOCAL   void   p_TDAL_TSSOURCE_STPTI_EventCallback(   STEVT_CallReason_t   Reason,
                            const   ST_DeviceName_t   RegistrantName,
                            STEVT_EventConstant_t   Event,
                            const   void   *EventData,
                            const   void   *SubscriberData_p   )
{
   uint8_t            ui_loop;
   uint8_t            pti_name_index;
   uint32_t        pti_buffer_len;
   ST_ErrorCode_t      st_err;
   struct   Extraction_s   *   p_faulty_extr = NULL;
   bool            first_notification = FALSE;
   STPTI_EventData_t   *      pst_eventdata;

   pti_name_index = (uint8_t)(uint32_t)SubscriberData_p;

   TDAL_LockMutex(   TDAL_TSSOURCE_Start_Stop   );

   pst_eventdata = (STPTI_EventData_t   *)   EventData;
   if   (   Event == STPTI_EVENT_BUFFER_OVERFLOW_EVT   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:   BUFFER_OVERFLOW   event   from   %s\n",
            RegistrantName));
      {
      TDAL_LockMutex(   stpti_evt_mutex   );
      for(   ui_loop=0   ; ui_loop<stpti_evt_extractions_count   ; ui_loop++   )
      {
        if   (   stpti_evt_extractions_array[ui_loop]->u.dmx.demuxname_index == pti_name_index   )
        {
            /*   This   extraction   is   on   the   faulty   demux   */
            STPTI_BufferTestForData(   stpti_evt_extractions_array[ui_loop]->u.dmx.demuxbuffer   ,
                        &pti_buffer_len   );
            if   (   (   kTDAL_TSSOURCE_PTIBUFFERSIZE   -   pti_buffer_len)   <   10*188   )
            {
            /*   This   is   the   faulty   buffer   !   */
            p_faulty_extr = stpti_evt_extractions_array[ui_loop];
               break;
            }
        }
      }

      if   (   p_faulty_extr != NULL   )
      {
        if   (   p_faulty_extr->u.dmx.buffer_overflow_notified == FALSE   )
        {
            if   (   p_faulty_extr->u.dmx.event_counter   >   5   )
            {
            first_notification = TRUE;
            p_faulty_extr->u.dmx.buffer_overflow_notified = TRUE;
            p_faulty_extr->u.dmx.notification_handled = FALSE;
            p_faulty_extr->u.dmx.last_notification_time = time_now();
            }
            else
            {
            p_faulty_extr->u.dmx.event_counter++;
            }
        }
      }
      TDAL_UnlockMutex(   stpti_evt_mutex   );
      if   (   first_notification == TRUE   )
      {
      STMERGE_Reset();

        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
            "signal   abort   ...\n"));
        st_err = STPTI_SignalAbort(   p_faulty_extr->u.dmx.demuxsignal   );
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
            "signal   abort   ...   st_err=0x%x\n",st_err));

      }
      if   (   p_faulty_extr == NULL   )
      {
        mTBOX_TRACE((kTBOX_NIV_3, "STPTI_EventCallback:"
            "why   did   we   receive   Overflow   event   and   p_faulty_extr   is   NULL   ?\n"   ));
      }
      else   if   (   p_faulty_extr->u.dmx.notification_handled == TRUE   )
      {
        if   (   time_minus(   time_now()   , p_faulty_extr->u.dmx.last_notification_time   )   >   (clock_t)(3*ST_GetClocksPerSecond())   )
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
               "say   that   it   is   handled   ...\n"   ));
            p_faulty_extr->u.dmx.buffer_overflow_notified = FALSE;
            p_faulty_extr->u.dmx.event_counter = 0;
            p_faulty_extr->u.dmx.notification_handled = FALSE;
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
               "handled   but   too   early   !\n"   ));
        }
      }
      }

   }


   if   (   Event == STPTI_EVENT_CC_ERROR_EVT   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
            "CC_ERROR   event   from   %s\n",
            RegistrantName));
   }

#if   defined(TSSOURCE_STPTI_INDEX_USE)
   if   (   Event == STPTI_EVENT_INDEX_MATCH_EVT   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
        "INDEX_MATCH_EVT   event   from   %s   on   slot@%x   buff@%x   cnt=%d   rec_cnt=%d   nbSC=%d\n",
        RegistrantName,
        pst_eventdata->SlotHandle,
        pst_eventdata->u.IndexEventData.BufferHandle,
        pst_eventdata->u.IndexEventData.PacketCount,
        pst_eventdata->u.IndexEventData.RecordBufferPacketCount,
        pst_eventdata->u.IndexEventData.NumberStartCodesDetected));
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "STPTI_EventCallback:"
        "INDEX_MATCH_EVT   event   from   %s   SC=0x%x   offSC=%d   auxData=0x%x\n",
        RegistrantName,
        pst_eventdata->u.IndexEventData.MPEGStartCode[0].MPEGStartCodeValue,
        pst_eventdata->u.IndexEventData.MPEGStartCode[0].MPEGStartCodeOffsetInToTSPacket,
        pst_eventdata->u.IndexEventData.MPEGStartCode[0].AuxillaryData));
   }
#endif

   STPTI_EnableErrorEvent(   SYS_PtiName[pti_name_index]   , Event   );

      TDAL_UnlockMutex(   TDAL_TSSOURCE_Start_Stop   );
}
#endif


/**----------------------------------------------------------------------
   *   Function   Name   :   p_TDAL_TSSOURCE_STPTI_EventCallback
   *
   *   Description   :
   *
   *   Side   effects   :
   *
   *   Comment   :
   ---------------------------------------------------------------------**/
bool   m_TDAL_TSSOURCE_GetPointerToCircularBuffer(
               tTDAL_TSSOURCE_Handle        handle,
               struct   TDAL_TS_CircularBuffer_s   **   pp_buffer)
{
   struct   Extraction_s   *   p_extr = (struct   Extraction_s   *)   handle;

   mTBOX_FCT_ENTER("m_TDAL_TSSOURCE_GetPointerToCircularBuffer()");

//   /****   SEVERAL   CHECKS   ****/
//   if   (   p_extr == NULL   )
//   {
//      mTBOX_RETURN(   false   );
//   }
//   if   ((   p_extr->state != eTDAL_TSSOURCE_EXTRACTIONSTATE_OPENED   )   &&
//      (   p_extr->state != eTDAL_TSSOURCE_EXTRACTIONSTATE_STARTED   )   &&
//      (   p_extr->state != eTDAL_TSSOURCE_EXTRACTIONSTATE_STOPPED   ))
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//            "m_TDAL_TSSOURCE_GetPointerToCircularBuffer:   TS   Source   is   not   opened   (buffer   not   allocated)\n"));
//      mTBOX_RETURN(   false   );
//   }
//
//   /****   MAIN   OPERATIONS   ****/
//   *pp_buffer = &p_extr->buffer;

   mTBOX_RETURN(   true   );
}



/**----------------------------------------------------------------------
 * Function Name : m_TDAL_TSSOURCE_RestartWithRequestedPacketNb
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
void   m_TDAL_TSSOURCE_RestartWithRequestedPacketNb(
               tTDAL_TSSOURCE_Handle   handle,
            uint32_t        packet_origin,
            int32_t        jump_ms,
            uint32_t   *      p_new_packet_position   )
{
   tTDAL_TSSOURCE_Error   tdal_tssource_err;
   struct   Extraction_s   *p_extr = (struct   Extraction_s   *)   handle;
   bool            no_error = TRUE;
   mTBOX_FCT_ENTER("m_TDAL_TSSOURCE_RestartWithRequestedPacketNb()");
//
//   /****   SEVERAL   CHECKS   ****/
//   if   (   p_extr == NULL   )
//   {
//      no_error = FALSE;
//   }
//   if   (   p_extr->state != eTDAL_TSSOURCE_EXTRACTIONSTATE_STARTED   )
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//            "RestartWithRequestedPacketNb:   TS   Source   is   not   started\n"));
//      no_error = FALSE;
//   }
//   if   (   p_extr->type != eTDAL_TSSOURCE_TYPE_MEMORY   )
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
//            "RestartWithRequestedPacketNb:   TS   Source   is   not   of   type   MEMORY\n"));
//      no_error = FALSE;
//   }
//
//   /****   MAIN   OPERATIONS   ****/
//   if   (   no_error == TRUE   )
//   {
//   if   (   p_extr->u.mem.request_jump_callback != NULL   )
//   {
//      mTBOX_TRACE((kTBOX_NIV_1,"RestartWithRequestedPacketNb:Pause-Jump-Resume\n"));
//
//      /**   PAUSE   Source   Main   Loop   **/
//      tdal_tssource_err = TDAL_TSSOURCE_Pause(   handle   , TRUE   /*   reinit   producer_pointer   to   0   */   );
//      if   (   tdal_tssource_err != eTDAL_TSSOURCE_NO_ERROR   )
//      {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"RestartWithRequestedPacketNb:   "
//            "[ERROR]   TDAL_TSSOURCE_Pause   failed   :   tdal_tssource_err=%d\n",
//            tdal_tssource_err));
//      }
//
//
//      p_extr->u.mem.request_jump_callback(p_extr,
//                   packet_origin,
//                        jump_ms,
//                        p_new_packet_position,
//                   p_extr->u.mem.private_data   );
//
//   
//      /*   RESUME   Source   Main   Loop   */
//      tdal_tssource_err = TDAL_TSSOURCE_Resume(   handle   );
//      if   (   tdal_tssource_err != eTDAL_TSSOURCE_NO_ERROR   )
//      {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"RestartWithRequestedPacketNb:   "
//            "[ERROR]   TDAL_TSSOURCE_Resume   failed   :   tdal_tssource_err=%d\n",
//            tdal_tssource_err));
//      }
//   }
//   }
//   else
//   {
//      *p_new_packet_position = packet_origin;
//   }
//

   mTBOX_RETURN;
}
/**----------------------------------------------------------------------
 * Function Name :  TDAL_TSSOURCE_SetProducerCallback
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
#if   ARV_TSBUFFERS_WITH_PROBES
GLOBAL   void   _TDAL_TSSOURCE_DumpProbeResults(   tTDAL_TSSOURCE_Handle   handle   )
{
   struct   Extraction_s   *   p_extr = (struct   Extraction_s   *)   handle;
   char            filename[32];
   sprintf(filename   , "C:\\PROBES\\%s.dat"   , p_extr->probe_name   );
   mTBOX_PROBE_DUMP((   p_extr->probe_prod   , filename   ));
}
#endif

/**----------------------------------------------------------------------
 * Function Name :  _TDAL_TSSOURCE_GetPointer
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   uint8_t   *   _TDAL_TSSOURCE_GetPointer(   tTDAL_TSSOURCE_Handle   handle   )
{
   struct   Extraction_s   *   p_extr = (struct   Extraction_s   *)   handle;
   uint8_t   *        ret;

   ret = p_extr->buffer.producer;

   return   ret;
}


/**----------------------------------------------------------------------
 * Function Name :  _TDAL_TSSOURCE_GetPointer
 *
 * Description :
 *
 * Side effects :
 *
 * Comment :
 ---------------------------------------------------------------------**/
GLOBAL   void   _TDAL_TSSOURCE_GetBuffer(   tTDAL_TSSOURCE_Handle   handle   , uint8_t   **pp_buff, uint32_t   *p_length   )
{
   struct   Extraction_s   *   p_extr = (struct   Extraction_s   *)   handle;

   *pp_buff = p_extr->buffer.address;
   *p_length = p_extr->buffer.length;

   return;
}

#endif



