/******************************************************************************
 *             COPYRIGHT 2004 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL$
 *        $Rev$
 *        $Date$
 *
 * PUBLIC
 *
 * DESCRIPTION : definition of the API of Thin Kernel Encapsulation Layer specific to Linux
 *
 *****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


#ifndef _TKEL_H_
#error "TKEL-ERROR: This file shall be included from tkel.h only!"
#endif

//#ifndef TKEL_PTHREADS
//#error "TKEL-ERROR: This file shall be included for PTHREADS only!"
//#endif


/*******************************************************/
/*         Includes                   */
/*******************************************************/

#include <pthread.h>

/*******************************************************/
/*         Defines                   */
/*******************************************************/

/* TKEL basic types */
#define TKEL_int8 signed char
#define TKEL_int16 signed short int
#define TKEL_int32 signed long int
#define TKEL_uint8 unsigned char
#define TKEL_uint16 unsigned short int
#define TKEL_uint32 unsigned long int
typedef unsigned char     TKEL_bool;

#if !defined(FALSE) &&   !defined(TRUE)
#define FALSE   0
#define TRUE   (!FALSE)
#endif

/* Queue specific definitions */
#define Q_GLOBAL              0x0001
#define Q_LOCAL              0x0002
#define Q_PRIOR              0x0004
#define Q_FIFO               0x0008
#define Q_LIMIT              0x0010
#define Q_NOLIMIT              0x0020
#define Q_PRIBUF              0x0040
#define Q_SYSBUF              0x0080

/* time management */
#define NB_MS_PER_TICK 10


/*******************************************************/
/*         Typedefs                   */
/*******************************************************/

/********************************************************************************
 * MEMORY TYPES
 ********************************************************************************/
/* TKEL memory size */
typedef TKEL_uint32 TKEL_msize;

/********************************************************************************
 * ISR TYPES
 ********************************************************************************/
/* TKEL frame */
typedef TKEL_int32 TKEL_frame;

/********************************************************************************
 * OTHER TYPES
 ********************************************************************************/
/* TKEL cause */
typedef TKEL_int16 TKEL_cause;

/********************************************************************************
 * TASK TYPES
 ********************************************************************************/

typedef TKEL_int16 TKEL_prio;     /* TKEL task priority */

typedef TKEL_uint32 TKEL_evts;     /* TKEL events */

typedef struct _TKEL_task        /* TKEL task descriptor */
{
   pthread_t task;
   TKEL_evts events;
   pthread_mutex_t evt_mutex;
   pthread_cond_t evt_cond;
   struct _TKEL_task * prev;
   struct _TKEL_task * next;
}TKEL_task;

typedef TKEL_task* TKEL_task_id;   /* TKEL task ID */

/********************************************************************************
 * SEMAPHORE TYPES
 ********************************************************************************/
/* TKEL semaphore ID */
typedef void * TKEL_sema_id;

/********************************************************************************
 * MUTEX TYPES
 ********************************************************************************/

typedef pthread_mutex_t * TKEL_mutex_id;   /* TKEL mutex ID */

/********************************************************************************
 * QUEUE TYPES
 ********************************************************************************/

typedef TKEL_uint32 TKEL_queue_id;   /* TKEL queue ID */

typedef TKEL_int16 TKEL_qdepth;     /* TKEL queue depth */

typedef TKEL_uint32 TKEL_qwidth;   /* TKEL queue width */

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

typedef TKEL_int32 TKEL_tck;/* TKEL tick width */

typedef TKEL_uint32 TKEL_timer_id;/* TKEL timer ID */

/* TKEL timer callback */
typedef void (*TKEL_timer_CB) ();


extern TKEL_uint32 TKEL_ticksPerSecond;
extern TKEL_uint32 TKEL_uSecondsPerTick;
extern TKEL_uint32 TKEL_CPUFrequency;

#if 0 //NOT_SO_OPTIMIZED_FOR_PC
/* Note: there is no need for ticks calculation on PC version */
#ifndef ms2tick
#define ms2tick(milliseconds) \
      (TKEL_tck)((((milliseconds) / 1000) * TKEL_ticksPerSecond) + \
      ((((milliseconds)%1000) * TKEL_ticksPerSecond) / 1000))
#endif

#ifndef tick2ms
#define tick2ms(ticks) \
      (((ticks) / TKEL_ticksPerSecond) * 1000 + \
       (((ticks) % TKEL_ticksPerSecond) * 1000 )/ TKEL_ticksPerSecond)
#endif

#else

#ifndef ms2tick
#define ms2tick(milliseconds)   ((TKEL_tck)(milliseconds))
#endif

#ifndef tick2ms
#define tick2ms(ticks)       (ticks)
#endif

#endif /* NOT_SO_OPTIMIZED_FOR_PC */

int TKEL_init(void);

#if defined(__cplusplus)
}
#endif

