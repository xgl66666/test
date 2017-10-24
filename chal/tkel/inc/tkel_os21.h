/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES              *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tkel/trunk/inc/arch/os21/tkel_os21.h $
 *        $Rev: 412 $
 *        $Date: 2011-09-23 12:33:05 +0200 (Fri, 23 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: header TKEL OS 21
 *
 *****************************************************************************/

#ifndef _TKEL_H_
#error "TKEL-ERROR: This file shall be included from tkel.h only!"
#endif

#ifndef TKEL_os21
#error "TKEL-ERROR: This file shall be included for OS21 only!"
#endif



#include <stddef.h>     /* for definition of NULL */
#include <os21.h>
#include <os21/st40.h>
#include <stdlib.h>     /* for use of abort function */
#include <stdio.h>
#include <string.h>

#ifndef _TKELOS21_H_
#define _TKELOS21_H_


#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************************
 * TKEL BASIC TYPES
 ********************************************************************************/
#define TKEL_int8      signed   char
#define TKEL_int16      signed   short int
#define TKEL_int32      signed   long int
#define TKEL_uint8      unsigned char
#define TKEL_uint16      unsigned short int
#define TKEL_uint32      unsigned long int
#define TKEL_uint64      unsigned long long

typedef unsigned char     TKEL_bool;

#if !defined(FALSE) &&   !defined(TRUE)
#define FALSE            0
#define TRUE            (!FALSE)
#endif


/********************************************************************************
 * MEMORY TYPES
 ********************************************************************************/
typedef TKEL_uint32       TKEL_msize;      /* TKEL memory size     */

/********************************************************************************
 * ISR TYPES
 ********************************************************************************/
typedef TKEL_int32        TKEL_frame;      /* TKEL frame      */

/********************************************************************************
 * OTHER TYPES
 ********************************************************************************/
typedef TKEL_int16        TKEL_cause;      /* TKEL cause of trouble   */

/********************************************************************************
 * TASK TYPES
 ********************************************************************************/
typedef TKEL_int16   TKEL_prio;     /* TKEL task priority              */
typedef TKEL_uint32   TKEL_evts;     /* TKEL events                  */

typedef struct _TKEL_task        /* TKEL task descriptor             */
{
   task_t        *task;       /* system task structure            */
   event_group_t     *events;     /* list of events the task received      */
   TKEL_evts       eventValue;   /* eventValue sent               */
   TKEL_bool       used;       /* indicates if the task descriptor is used */
   struct _TKEL_task *next;       /* points to next task              */
}TKEL_task;

typedef TKEL_task*   TKEL_task_id;   /* TKEL task ID                  */

/********************************************************************************
 * SEMAPHORE TYPES
 ********************************************************************************/
typedef semaphore_t       *TKEL_sema_id;   /* TKEL semaphore ID         */

/********************************************************************************
 * MUTEX TYPES
 ********************************************************************************/

typedef mutex_t     *TKEL_mutex_id;

/********************************************************************************
 * QUEUE TYPES
 ********************************************************************************/
typedef TKEL_int16   TKEL_queue_id;     /* TKEL queue ID           */
typedef TKEL_int16   TKEL_qdepth;     /* TKEL queue depth           */
typedef TKEL_uint32 TKEL_qwidth;     /* TKEL queue width           */

typedef struct _queueList_t
{
   message_queue_t     *pQueue;     /* pointer to OS20 queue structure     */
   TKEL_task_id     taskOwner;     /* pointer to the owner task ID      */
   TKEL_uint16       nbFreeMsg;     /* Number of unused message in queue   */
   TKEL_uint16       nbsentMsg;     /* number of sent message in queue     */
   TKEL_uint16       MsgSize;     /* size of one message         */
   TKEL_evts       notify;      /* event to send if message is sent     */
   void        *buffer;     /* buffer where OS20 manage the queue   */
}TKEL_queue_list;
#ifdef USE_TKEL_MEMORY_PARTITIONS
/********************************************************************************
 * MEMORY PARTITION TYPES
 ********************************************************************************/
typedef enum
{
   TKEL_MEMPART_FIXED_BUFFER_SIZE = 0x00464958,
   TKEL_MEMPART_VARIABLE_BUFFER_SIZE = 0x00564152
}TKEL_memPartType;

typedef struct _TKEL_fixedSizeMemPart
{
   TKEL_memPartType   type;
   TKEL_sema_id     lock;
   TKEL_uint32     memPartSize;
   void*         memPartAddress;
   TKEL_uint32     bufferSize;
   TKEL_uint8     *firstFreeBuffer;
   TKEL_uint8     *lastFreeBuffer;
}TKEL_fixedSizeMemPart;

typedef struct _TKEL_varSizeMemPart
{
   TKEL_memPartType   type;
   TKEL_sema_id     lock;
   TKEL_uint32     memPartSize;
   void*         memPartAddress;
   /* to be completed */
}TKEL_varSizeMemPart;

typedef struct _TKEL_memPart
{
   TKEL_memPartType   type;
   TKEL_sema_id     lock;
   TKEL_uint32     memPartSize;
   void*         memPartAddress;
}TKEL_memPart;

typedef TKEL_memPart* TKEL_memPart_id;     /* TKEL memory partition ID */

#endif /* USE_TKEL_MEMORY_PARTITIONS */

/********************************************************************************
 * TIMER TYPES
 ********************************************************************************/
typedef TKEL_uint32 TKEL_tck;      /* TKEL tick width     */
typedef enum
{
   TKEL_AVAILABLE,     /* timer has not been created             */
   TKEL_ALLOCATED,     /* timer has been created but not started      */
   TKEL_ACTIVE       /* timer has been created and is started      */
}TKEL_timer_state;

typedef void (*TKEL_timer_CB) ();     /* TKEL timer callback             */

typedef struct _TKEL_timer
{
   TKEL_tck      ticks;   /* number of ticks before event        */
   TKEL_task_id     task;     /* Task owner of timer             */
   TKEL_evts     event;   /* Event when timer ellapse         */
   TKEL_timer_state state;   /* State of timer               */
   TKEL_timer_CB   callback; /* callback to call when timeout expired     */
}TKEL_timer;

typedef TKEL_uint8 TKEL_timer_id;     /* TKEL timer ID             */

extern TKEL_uint32 TKEL_ticksPerSecond;
extern TKEL_uint32 TKEL_uSecondsPerTick;
extern TKEL_uint32 TKEL_CPUFrequency;

#define ms2tick(milliseconds) ((TKEL_tck)(milliseconds))
#define tick2ms(ticks) ((TKEL_uint32)(ticks))

/* time management */
#define NB_MS_PER_TICK 1

#ifdef __cplusplus
}
#endif

/* patch for DAL VFS test */
#define TKEL_IS_VALID_POINTER(p)  (((p) != NULL) && (((unsigned int)(p) & 0xC0000000) != 0))

#endif /*_TKELOS21_H_*/
