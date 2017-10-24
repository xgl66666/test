/******************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS_SINK  -  TS_SINK driver
 *
 * FILE NAME:   TDAL_TS_SINK_P.H
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ts/tdal_ts_sink_p.h $
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

#ifndef   _TDAL_TSSINK_P_H_
#define   _TDAL_TSSINK_P_H_

/*******************************************************************************
   *   Includes
   ******************************************************************************/
#ifndef   _TDAL_TS_H_
#error   "tdal_ts.h   must   be   included   before   tdal_ts_sink_p.h"
#endif
#ifndef   _TDAL_TS_BUFFER_P_H_
#error   "tdal_ts_buffer_p.h   must   be   included   before   tdal_ts_sink_p.h"
#endif

#ifdef   __cplusplus
extern   "C"   {
#endif


/*******************************************************************************
   *   Defines
   ******************************************************************************/
#define   kPACKET_SIZE               188
#define   kTDAL_TSSINK_VIDEO_PID_MASK      0x0FFF

/*#define   kTDAL_TSSINK_RUNOUTWAITTIME      312   *//*   in   ticks   :   ~20ms   */
/*#define   kTDAL_TSSINK_PAUSEWAITTIME      156   *//*   in   ticks   :   ~10ms   */
/*#define   kTDAL_TSSINK_INTERBUFWAITTIME      10*/
/*#define   kTDAL_TSSINK_LOCKEDBYJUMPWAITTIME      20*/

#define   kTDAL_TSSINKi_NB_SOURCES        3
#define   kTDAL_TSSINKi_NB_DEMUX            3
#define   kTDAL_TSSINK_INJECTION_BUFFER_SIZE      0x37140   /*   =   1200*188   */   /*   0x37000   0x19000   100Kb   */
#define   kTDAL_TSSINK_UNDERFLOW_EVT_TIME_OUT      9375   /*   waiting   for   300Kb   before   time   out*/
#define   kTDAL_TSSINK_MAX_BACKWARD_JUMP      0xC0680
#define   kTDAL_TSSINK_PUSHPULL_THRESHOLD      192512
#define   kTDAL_TSSINK_AUDIO_PTIBUFFER_THRESHOLD   0xC800

#define   kTDAL_TSSINK_MAX_BIT_BUFFER_LEVEL      (276   *   1024)   /*   not   magic   number   computed   from   stvid   files
                                 to   avoid   OVER_FLOW   EVENT   */
#define   kTDAL_TSSINK_BIT_BUFFER_OVERFLOW_THRESHOLD   (96   *   1024)   /*   not   magic   number   computed   from   stvid   files   */

   /**   task   delay   if   we   have   no   ConsumerCallback   (in   ticks)   **/
#define   kTDAL_TSSINK_SLEEPTIME_NOCCB      1563

#if   TDAL_TSSINK_USE_STFDMA
#define   kTDAL_TSSINK_FDMA_ALIGNMENT        256   /*32*/
#define   kTDAL_TSSINKi_TOO_SMALL_FOR_DMA      1024
#endif

      /*   these   registers   are   valable   for   STi5100   */
#define   kTDAL_TSSINK_SWTS_DATA_REGISTER_ADDRESS      (TSMERGE_BASE_ADDRESS   +   0x10280)
#define   mTDAL_TSSINK_SWTS_DATA_REGISTER_VALUE      *((DU32   *)kTDAL_TSSINK_SWTS_DATA_REGISTER_ADDRESS)
#define   kTDAL_TSSINK_SWTS_CTRL_REGISTER_ADDRESS      (TSMERGE_BASE_ADDRESS   +   0x2E0)
#define   mTDAL_TSSINK_SWTS_CTRL_REGISTER_VALUE      *((DU32   *)kTDAL_TSSINK_SWTS_CTRL_REGISTER_ADDRESS)


/*******************************************************************************
   *   Typedefs
   ******************************************************************************/
enum   TDAL_TSSINK_InjectionState_e   {
   eTDAL_TSSINK_INJECTIONSTATE_OPENED,
   eTDAL_TSSINK_INJECTIONSTATE_STARTED,
   eTDAL_TSSINK_INJECTIONSTATE_STOPPED,
   eTDAL_TSSINK_INJECTIONSTATE_CLOSED
};
enum   TDAL_TSSINK_ThreadState_e   {
   eTDAL_TSSINK_THREADSTATE_CONSUMING, /*   the   thread   is   injecting   data   */
   eTDAL_TSSINK_THREADSTATE_ORDERSTOP, /*   we   want   the   thread   to   stop   injection   */
   eTDAL_TSSINK_THREADSTATE_STOPPED, /*   the   injection   is   stopped   */
   eTDAL_TSSINK_THREADSTATE_ORDEREXIT, /*   we   want   the   thread   to   exit   */
   eTDAL_TSSINK_THREADSTATE_EXITED      /*   the   thread   is   exited   */
};
enum   TDAL_TSSINK_OpenStep_e   {
   eTDAL_TSSINK_OPENSTEP_NOTHING,
   eTDAL_TSSINK_OPENSTEP_STRUCT_ALLOCATED,
   eTDAL_TSSINK_OPENSTEP_DMA_NODE_ALLOCATED,
   eTDAL_TSSINK_OPENSTEP_DMA_CHANNEL_LOCKED,
   eTDAL_TSSINK_OPENSTEP_SEMA_EXITRDV_CREATED,
   eTDAL_TSSINK_OPENSTEP_SEMA_START_CREATED,
   eTDAL_TSSINK_OPENSTEP_SEMA_STOPRDV_CREATED,
   eTDAL_TSSINK_OPENSTEP_SEMA_UNDERFLOW_CREATED,
   eTDAL_TSSINK_OPENSTEP_SEMA_CLIENT_CREATED,
   eTDAL_TSSINK_OPENSTEP_TASK_LAUNCHED,
   eTDAL_TSSINK_OPENSTEP_FINISHED
};
enum   TDAL_TSSINK_StartStep_e   {
   eTDAL_TSSINK_STARTSTEP_NOTHING,
   eTDAL_TSSINK_STARTSTEP_CONSUMER_RESERVED,
   eTDAL_TSSINK_STARTSTEP_EVT_SUBSCRIBED,
   eTDAL_TSSINK_STARTSTEP_FINISHED
};

/*   for   secure   access   to   registers   */
/*#pragma   ST_device(DU32)
typedef   volatile   unsigned   int   DU32;*/



/*******************************************************************************
   *   Structures
   ******************************************************************************/
struct   TaskControl_s   {
   task_t   *               threadid;
   enum   TDAL_TSSINK_ThreadState_e      threadstate;
   STOS_Semaphore_t   *            sema_exitrdv;
   STOS_Semaphore_t   *            sema_start;
   STOS_Semaphore_t   *            sema_stop;
};

/*   User   State   machine   */
/*   ------------------   */
typedef   enum
{
   eTDAL_TSSINK_USR_SLEEP=0,
   eTDAL_TSSINK_START,
   eTDAL_TSSINK_PAUSE,
   eTDAL_TSSINK_STOP
}   tTDAL_TSSINK_UsrState_t;

struct   tTDAL_TSSINK_Injection_s   {
   tTDAL_TSSINK_Type            type;
   enum   TDAL_TSSINK_InjectionState_e   state;

   tTDAL_TS_Object            source;

   enum   TDAL_TSSINK_OpenStep_e      open_step;
   enum   TDAL_TSSINK_StartStep_e      start_step;

   /*   info   about   buffers   */
   tTDAL_TSSOURCE_Handle        tssource_handle;
   struct   TDAL_TS_CircularBuffer_s   *   p_buffer;
   uint8_t                 consumer_index;
   uint8_t   *               stopped_consumer_address;   /*   address   of   consumer   when   it   is   stopped   */

   /*   info   about   thread   */
   struct   TaskControl_s        taskctrl;

   uint32_t            inject_counter;
   union   {
      struct   {
      #if   1   /*TDAL_TSSINK_STVIDFLOWCONTROL*/
      /*   used   to   asserve   the   injection   with   the   video   decoder   */
      bool            paused;
      STOS_Semaphore_t   *        sema_underflow;
      bool            first_underflow_received;
      #endif
      BOOL               PLAY_VideoJumpFlag;      /*   Is   TRUE   is   a   jump   has   been   notified      */
      BOOL               PLAY_VideoUnderflowFlag;      /*   Is   TRUE   is   an   underflow   has   been   notified   */
      BOOL               PLAY_VideoInjectionNotified;   /*   Is   TRUE   if   injection   has   been   notified      */
      U32               PLAY_VideoBitBufferFreeSize;   /*   Free   size   available   updated   by   video   event   */
      U32               PLAY_VideoBitRateValue;      /*   In   units   of   400   bits/second        */
      U32               PLAY_MinVideoBitRateValue;      /*   In   units   of   400   bits/second        */
      S32               PLAY_VideoRequiredTimeJump;   /*   Required   time   to   jump   in   ms        */
      S32               PLAY_VideoRequiredTimeJumpInMs;   /*   Required   time   to   jump   in   ms        */
      S32            PLAY_JumpIntInMs;
      U32               PLAY_VideoRequiredDuration;   /*   Required   time   to   inject   in   ms        */
      U32               PLAY_VideoBytesToComplete;   /*   Nb   bytes   to   complete   the   current   injection   */
      tTDAL_TSSINK_UsrState_t      PLAY_SwtsState;        /*   SWTS   DMA   State                 */
      U32               PLAY_BufferConsumerSize;      /*   TS   buffer   consumer   size            */
      BOOL            TS_FirstPacketReceived;
      BOOL            Discontinuity;
      BOOL            TransfertRelatedToPrevious;
      U32               TS_LatchTime;            /*   Global   system   time   of   the   TS   object   */
      U32               TS_PacketTimeInMs;            /*   Current   packet   time   in   ms      */
      U32               TS_PacketPosition;            /*   Current   packet   position        */
      S32               TS_PacketPreviousPosition;      /*   Last   packet   position        */
      S32               TS_FirstPacketPosition;      /*   First   packet   position        */
      U32            nbTimeWithoutUnderFlows;
      U32            NbPacketInjectedCounter;

      /*   info   about   SetPosition   Stuff   */
      signed   int        TimeToJumpInMs;
      S32               NbPacketsTojump;
      BOOL            JumpRequested;
      BOOL            JumpRelative;

      /*   info   about   injection   type   */
      BOOL            RadioInjection;

      /*   Quick   and   Durty   coding   to   know   wether   we   are   in   PUSH   or   PULL   injecting   mode   */
      U32            ConsProdDistance;

      #if   TDAL_TSSINK_USE_STFDMA
      STFDMA_Node_t   *        dma_node;
      #if   defined(ST_OSLINUX)
      STFDMA_Node_t   *        dma_node_k;   /*   Kernel   */
      #endif
      STFDMA_Node_t   *        dma_node_u;
      STFDMA_ChannelId_t      dma_channel;
      uint32_t            swts_identifier;
      #endif

      uint32_t            video_pid_ratio;
      uint32_t            audio_pid_ratio;
      uint32_t            syncbyte_offset;   /*   used   by   CalculatePidRatio   to   be   synchronized   even   if   packets   are   shifted.   */

      uint32_t            nb_picture_displayed_events;

      /*   "pvgs_..."   variables   mean   Previous   GetStatus   and   contains
        *   values   for   variables   from   the   previous   GetStatus   call.
        *   It   helps   determining   the   end_of_injection   status   */
      uint8_t   *            pvgs_consumer_address;
      uint32_t            pvgs_nb_picture_displayed_events;

      }   dmx;
      struct   {
      /*   info   about   the   external   consumer   */
      STOS_Semaphore_t   *        sema_client;
      uint32_t            client_request;
      tTDAL_TSSINK_ConsumerCallbackFct   consumer_callback;
      void   *            client_tag;
      }   mem;
   }   u;

   #if   ARV_TSBUFFERS_WITH_PROBES
   char   probe_name[16];

   mTBOX_PROBE_DECLARE(   probe_cons   , NULL   , 1024   );
   #endif
};


/*******************************************************************************
   *   Variables   Declarations   (IMPORT)
   ******************************************************************************/


/*******************************************************************************
   *   Functions   Declarations   (MODULE)
   ******************************************************************************/
      /*   Main   loop   of   the   injecting   thread   */
void        p_TDAL_TSSINK_ConsumingTask(   void   *   args   );
void        p_TDAL_TSSINK_InjectingLoop(
                        struct   tTDAL_TSSINK_Injection_s   *   p_inj   );
void        p_TDAL_TSSINK_ExternalConsumingLoop(
                        struct   tTDAL_TSSINK_Injection_s   *   p_inj   );
bool        p_TDAL_TSSINK_InjectFromCurrentBuffer_CPU(
                        struct   tTDAL_TSSINK_Injection_s   *   p_inj   );
#if   TDAL_TSSINK_USE_STFDMA
LOCAL   tTDAL_TSSINK_Error   p_TDAL_TSSINK_Play_InjectTS_FDMA(
                        struct   tTDAL_TSSINK_Injection_s   *   INJ,
                        uint8_t*   TS_Buffer,
                        U32   TS_Size,
                        BOOL   Synchronous);
#endif

LOCAL   void        p_TDAL_TSSINK_EvaluatePidsRatio(
                 struct   tTDAL_TSSINK_Injection_s   *   p_inj   ,
                 uint8_t   *               buffer   ,
                 uint32_t               length   );
LOCAL   void        p_TDAL_TSSINK_CalculatePidRatioInTSBuffer(
                 struct   tTDAL_TSSINK_Injection_s   *   p_inj   ,
                 uint8_t   *               buffer   ,
                 uint32_t               length   ,
                 uint16_t               pid_1   ,
                 uint32_t   *               pid_1_ratio   ,
                 uint16_t               pid_2   ,
                 uint32_t   *               pid_2_ratio   );


tTDAL_TSSINK_Error   p_TDAL_TSSINK_STVIDSubscribeEvents(   void   );
tTDAL_TSSINK_Error   p_TDAL_TSSINK_STVIDUnsubscribeEvents(   void   );
void        p_TDAL_TSSINK_STVIDEventsHandler(   uint32_t   reason,
                              uint32_t   event,
                              const   void   *   data   );

void        p_TDAL_TSSINK_CancelOpen(   struct   tTDAL_TSSINK_Injection_s   *   p_inj   );
void        p_TDAL_TSSINK_CancelStart(   struct   tTDAL_TSSINK_Injection_s   *   p_inj   );

tTDAL_TSSINK_Error   p_TDAL_TSSINK_NominalPlayFromTrickMode(struct   tTDAL_TSSINK_Injection_s   *   p_inj,
                            S32   JumpIntInMs,
                            U32   SizeToInject,
                            U32   SizeAvailable,
                            U32   ConsumerSize);

#ifdef   __cplusplus
}
#endif

#endif   /*   _TDAL_TSSINK_P_H_   */

