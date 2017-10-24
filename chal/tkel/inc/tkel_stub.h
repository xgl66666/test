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

#ifndef TKEL_stub
#error "TKEL-ERROR: This file shall be included for UCOS only!"
#endif
#include <stddef.h>
#include <stdlib.h>     /* for use of abort function */
#include <stdio.h>
#include <string.h>
/*******************************************************/
/*         Includes                   */
/*******************************************************/

#define TKEL_ticksPerSecond 1
//##############################################################
#ifndef _TKELSTUB_H_
#define _TKELSTUB_H_
/*******************************************************/
/*         Defines                   */
/*******************************************************/
/* TKEL basic types */
#define TKEL_int8      signed   char
#define TKEL_int16      signed   short int
#define TKEL_int32      signed   long int
#define TKEL_uint8      unsigned char
#define TKEL_uint16      unsigned short int
#define TKEL_uint32      unsigned long int
#define TKEL_uint64      unsigned long long

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
#define NB_MS_PER_TICK (1000/TKEL_ticksPerSecond)

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

typedef struct _TKEL_task        /* TKEL task descriptor             */
{
    TKEL_evts       eventValue;   /* eventValue sent               */
   TKEL_bool       used;       /* indicates if the task descriptor is used */
   struct _TKEL_task *next;       /* points to next task              */
}TKEL_task;

typedef TKEL_task*   TKEL_task_id;   /* TKEL task ID                  */

/********************************************************************************
 * SEMAPHORE TYPES
 ********************************************************************************/
/* TKEL semaphore ID */
typedef unsigned int TKEL_sema_id;

/********************************************************************************
 * MUTEX TYPES
 ********************************************************************************/
typedef  struct _TKEL_mutex_t
{

}TKEL_mutex_t;   

typedef TKEL_mutex_t* TKEL_mutex_id;
/********************************************************************************
 * QUEUE TYPES
 ********************************************************************************/

typedef TKEL_uint32 TKEL_queue_id;   /* TKEL queue ID */

typedef TKEL_int16 TKEL_qdepth;     /* TKEL queue depth */

typedef TKEL_uint32 TKEL_qwidth;   /* TKEL queue width */

#ifndef USE_TKEL_MEMORY_PARTITIONS
#define USE_TKEL_MEMORY_PARTITIONS
#endif

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
   TKEL_uint32 size;      /**< size of heap */
   TKEL_uint32 buf_size;  /**< size of each entry */
   void *heap;            /**< heap handle, which will be bcm_heap_t * */
}TKEL_fixedSizeMemPart;

typedef struct _TKEL_varSizeMemPart
{
   /* to be completed */
}TKEL_varSizeMemPart;

typedef struct _TKEL_memPart
{
   TKEL_memPartType type;
   union {
      TKEL_fixedSizeMemPart fixed;
      TKEL_varSizeMemPart var;
   } part;
}TKEL_memPart;

typedef TKEL_memPart* TKEL_memPart_id;     /* TKEL memory partition ID */

#endif /* USE_TKEL_MEMORY_PARTITIONS */

/********************************************************************************
 * TIMER TYPES
 ********************************************************************************/

typedef TKEL_int32 TKEL_tck;/* TKEL tick width */

typedef TKEL_uint32 TKEL_timer_id;/* TKEL timer ID */

/* TKEL timer callback */
typedef void (*TKEL_timer_CB)(void);


extern TKEL_uint32 TKEL_uSecondsPerTick;
extern TKEL_uint32 TKEL_CPUFrequency;

int TKEL_init(void);

#if defined(__cplusplus)
}
#endif

#endif /*_TKELUCOS_H_*/
