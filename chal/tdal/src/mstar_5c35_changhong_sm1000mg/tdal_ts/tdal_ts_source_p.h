/******************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS_SOURCE  -  TS_SOURCE driver
 *
 * FILE NAME:   TDAL_TS_SOURCE_P.H
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_source_p.h $
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

#ifndef   _TDAL_TS_SOURCE_P_H_
#define   _TDAL_TS_SOURCE_P_H_

/*******************************************************************************
   *   Includes
   ******************************************************************************/

#ifdef   __cplusplus
extern   "C"   {
#endif

#ifndef   _TDAL_TS_H_
#error   "tdal_ts.h   must   be   included   before   tdal_ts_source_p.h"
#endif
#ifndef   _TDAL_TS_BUFFER_P_H_
#error   "tdal_ts_buffer_p.h   must   be   included   before   tdal_ts_source_p.h"
#endif

/*******************************************************************************
   *   Defines
   ******************************************************************************/
#define   TDAL_TSSOURCE_OLD_STYLE_EXTRACTION        1

#define   kTDAL_TSSOURCE_MAX_NB_STREAMS      16   /*   Some   Streams   needs   a   great   number   of   pids   */

#define   kTDAL_TSSOURCE_INVALID_PID      (tTDAL_TSSOURCE_Pid)0xffff

#if   TDAL_TSSOURCE_OLD_STYLE_EXTRACTION
#define   kTDAL_TSSOURCE_PTIBUFFERSIZE      18*188*32   /*4*256*188*/
#else
#define   kTDAL_TSSOURCE_PTIBUFFERSIZE      4*256*188
#endif
#define   kTDAL_TSSOURCE_PTIMULTIPACKET      /*32*/   256
#if   defined(ST_OSLINUX)
#define   kTDAL_TSSOURCE_PTITRANSFERTMODE   STPTI_COPY_TRANSFER_BY_MEMCPY
#else
#define   kTDAL_TSSOURCE_PTITRANSFERTMODE   STPTI_COPY_TRANSFER_BY_MEMCPY   /*STPTI_COPY_TRANSFER_BY_FDMA*/
#endif
   /**   task   delay   if   we   have   no   ProducerCallback   (in   ticks)   **/
#define   kTDAL_TSSOURCE_SLEEPTIME_NOPCB      1563   /*   100   ms   */
#define   kTDAL_TSSOURCE_SLEEPTIME_WAITDATA   5   /*   5   ms   */
#define   kTDAL_TSSOURCE_SLEEPTIME_PAUSE      150   /*   10   ms   */

#define   kTDAL_TSSOURCE_FDMA_ALIGNMENT        32

#define   kTDAL_TSSOURCE_CIRC_BUFFER_ALIGNMENT   256   /*32*/

/*#if   defined(STPTI_INDEX_SUPPORT)
#define   TSSOURCE_STPTI_INDEX_USE
#endif*/

/*******************************************************************************
   *   Typedefs
   ******************************************************************************/
enum   TDAL_TSSOURCE_ExtractionState   {
   eTDAL_TSSOURCE_EXTRACTIONSTATE_OPENED,
   eTDAL_TSSOURCE_EXTRACTIONSTATE_STARTED,
   eTDAL_TSSOURCE_EXTRACTIONSTATE_STOPPED,
   eTDAL_TSSOURCE_EXTRACTIONSTATE_CLOSED
};
enum   TDAL_TSSOURCE_ThreadState_e   {
   eTDAL_TSSOURCE_THREADSTATE_PRODUCING, /*   the   thread   is   extracting   data   */
   eTDAL_TSSOURCE_THREADSTATE_ORDERSTOP,   /*   we   want   the   thread   to   stop   extraction   */
   eTDAL_TSSOURCE_THREADSTATE_STOPPED,   /*   the   extraction   is   stopped   */
   eTDAL_TSSOURCE_THREADSTATE_ORDEREXIT,   /*   we   want   the   thread   to   exit   */
   eTDAL_TSSOURCE_THREADSTATE_EXITED      /*   the   thread   is   exited   */
};
enum   TDAL_TSSOURCE_OpenStep_e   {
   eTDAL_TSSOURCE_OPENSTEP_NOTHING,
   eTDAL_TSSOURCE_OPENSTEP_STRUCT_ALLOCATED,
   eTDAL_TSSOURCE_OPENSTEP_BUFFER_ALLOCATED,
   eTDAL_TSSOURCE_OPENSTEP_SEMA_EXITRDV_CREATED,
   eTDAL_TSSOURCE_OPENSTEP_SEMA_START_CREATED,
   eTDAL_TSSOURCE_OPENSTEP_SEMA_STOPRDV_CREATED,
   eTDAL_TSSOURCE_OPENSTEP_SEMA_CLIENT_CREATED,
   eTDAL_TSSOURCE_OPENSTEP_SEMA_PAUSE_CREATED,
   eTDAL_TSSOURCE_OPENSTEP_TASK_LAUNCHED,
   eTDAL_TSSOURCE_OPENSTEP_FINISHED
};
enum   TDAL_TSSOURCE_ConfigStep_e   {
   eTDAL_TSSOURCE_CONFIGSTEP_NOTHING,
   eTDAL_TSSOURCE_CONFIGSTEP_STPTI_HANDLE_OPENED,
   eTDAL_TSSOURCE_CONFIGSTEP_STPTI_BUFFER_ALLOCATED,
   eTDAL_TSSOURCE_CONFIGSTEP_STPTI_SIGNAL_ALLOCATED,
   eTDAL_TSSOURCE_CONFIGSTEP_STPTI_SIGNAL_ASSOCIATED,
   eTDAL_TSSOURCE_CONFIGSTEP_FINISHED
};
enum   TDAL_TSSOURCE_AddPidStep_e   {
   eTDAL_TSSOURCE_ADDPIDSTEP_NOTHING,
   eTDAL_TSSOURCE_ADDPIDSTEP_SLOT_ALLOCATED,
   eTDAL_TSSOURCE_ADDPIDSTEP_SLOT_SET,
   eTDAL_TSSOURCE_ADDPIDSTEP_SLOT_LINKED,
   eTDAL_TSSOURCE_ADDPIDSTEP_FINISHED
};
enum   TDAL_TSSOURCE_StartStep_e   {
   eTDAL_TSSOURCE_STARTSTEP_NOTHING,
   eTDAL_TSSOURCE_STARTSTEP_SLOTS_LINKED,
   eTDAL_TSSOURCE_STARTSTEP_EVENTS_REGISTERED,
   eTDAL_TSSOURCE_STARTSTEP_FINISHED
};


/*******************************************************************************
   *   Structures
   ******************************************************************************/

struct   Stream_s   {
   tTDAL_TSSOURCE_Pid   pid;
#if   defined(TSSOURCE_STPTI_INDEX_USE)
#endif
};


struct   TaskControl_s   {
   //task_t   *               threadid;
   enum   TDAL_TSSOURCE_ThreadState_e   threadstate;
};


struct   Extraction_s   {
   tTDAL_TSSOURCE_Type            type;

   enum   TDAL_TSSOURCE_ExtractionState   state;
   bool                 configured;

   enum   TDAL_TSSOURCE_OpenStep_e      open_step;
   enum   TDAL_TSSOURCE_ConfigStep_e      config_step;
   enum   TDAL_TSSOURCE_AddPidStep_e      addpid_step;
   enum   TDAL_TSSOURCE_StartStep_e      start_step;

   /*   buffer   attached   to   the   TSSOURCE   */
   struct   TDAL_TS_CircularBuffer_s      buffer;
   uint32_t               packet_length;

   /*   info   about   thread   */
   struct   TaskControl_s        taskctrl;

   union   {
      struct   dmx   {
      /*   info   on   demux   */
      tTDAL_TS_Object        demux;
      uint8_t            demuxname_index;   /*   index   of   PTI's   name   */

      
      uint32_t              TS_BufferPtr;
      uint32_t              TS_BufferBase;
      uint32_t              TS_BufferSize;
      uint32_t              TS_BufferProducer;
      uint32_t              TS_BufferPreviousProducer;
      uint32_t              TS_BufferConsumer;
      bool            BufferFlushed;
      
      bool            outOfTheLoop;

      #if   PATCH_FOR_BUGGED_STPTI_SIGNAL
      bool            buffer_overflow_notified;
      clock_t            last_notification_time;
      bool            notification_handled;
      uint32_t            event_counter;
      #endif

      /*   info   about   streams   (pti/slot)   */
      struct   Stream_s        streams[kTDAL_TSSOURCE_MAX_NB_STREAMS];
      uint8_t            streams_count;

      /*   timestamping   callback   data   */
      tTDAL_TSSOURCE_TimestampingCallbackFct   timestamping_callback;
      void   *                   timestamping_client_tag;
#if   !TDAL_TSSOURCE_OLD_STYLE_EXTRACTION
      STFDMA_Node_t   *        dma_node;
      STFDMA_Node_t   *        dma_node_u;
      STFDMA_ChannelId_t      dma_channel;
#endif
      }   dmx;
      struct   {
      /*   info   about   the   external   producer   */
      uint32_t            client_request;
      tTDAL_TSSOURCE_ProducerCallbackFct   producer_callback;
      tTDAL_TSSOURCE_RequestJumpCallbackFct   request_jump_callback;
      void   *                 private_data;

      bool            paused;
      }   mem;
   }   u;

   #if   ARV_TSBUFFERS_WITH_PROBES
   char   probe_name[16];

   mTBOX_PROBE_DECLARE(   probe_prod   , NULL   , 1024   );
   #endif
};

/*******************************************************************************
   *   Variables   Declarations   (IMPORT)
   ******************************************************************************/

/*******************************************************************************
   *   Functions   Declarations   (MODULE)
   ******************************************************************************/
      /*   Main   loop   of   the   extracting   thread   */
void   p_TDAL_TSSOURCE_ProducingTask(   void   *   args   );
void   p_TDAL_TSSOURCE_ExtractingLoop(   struct   Extraction_s   *   p_extr   );
void   p_TDAL_TSSOURCE_ExternalProducingLoop(   struct   Extraction_s   *   p_extr   );

uint8_t   p_TDAL_TSSOURCE_GetPTINameIndex(   tTDAL_TS_Object   );
void   p_TDAL_TSSOURCE_PrintBuffersState(   struct   Extraction_s   *   extr   );

void   p_TDAL_TSSOURCE_CancelOpen(   struct   Extraction_s   *   p_extr   );
void   p_TDAL_TSSOURCE_CancelConfig(   struct   Extraction_s   *   p_extr   );
void   p_TDAL_TSSOURCE_CancelAddPid(   struct   Extraction_s   *   p_extr   , uint8_t   index   );
void   p_TDAL_TSSOURCE_CancelStart(   struct   Extraction_s   *   p_extr   );
bool   p_TDAL_TSSOURCE_AllocateCircularBuffer(   struct   Extraction_s   *   p_extr   , uint32_t   length);
#ifdef   __cplusplus
}
#endif

#endif   /*   _TDAL_TSSOURCE_P_H_   */

