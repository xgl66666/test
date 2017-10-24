/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL$
 *         $Rev$
 *         $Date$
 *
 * PRIVATE
 *
 * DESCRIPTION : private TKEL definitions
 *
 *****************************************************************************/

#if defined(__cplusplus) 
extern "C" {
#endif


/*******************************************************/
/*          Includes                      */
/*******************************************************/

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <assert.h>

#include "tbox.h"
#include "crules.h"

/*******************************************************/
/*          Defines                      */
/*******************************************************/

#define ITC_NB_QUEUES      200 /* queues number */
#define ITC_NB_TIMERS      200 /* timers number */
#define ITC_NB_TASK       200 /* number of threads */
#define ITC_NB_LOCKS      200 /* maximum of simultaneous mutex (LOCK) */
#define ITC_NB_SYNC       200 /* maximum of simultaneous event (SYNC) */
#define ITC_NB_SEMA       200 /* maximum of simultaneous semaphore (SEMA) */

/* thread policies : SCHED_RR - SCHED_FIFO - SCHED_OTHER   */
#ifdef __TKEL_THREAD_PRIO_ENABLE__
#define DEFAULT_POLICY      SCHED_RR
#else
#define DEFAULT_POLICY      SCHED_OTHER
#endif
#define CRITICAL_POLICY      SCHED_FIFO

/* The following definitions support conversions between the basic unit of   */
/* time in the nip (which is tens-of-milliseconds) and time in all the 4.2BSD */
/* system calls (which is seconds and microseconds).                  */
#define USECS_PER_MSEC   1000
#define MSECS_PER_SEC   1000
#define USECS_PER_SEC   1000000
#define NSECS_PER_MSEC  1000000
#define NSECS_PER_SEC   1000000000
#define NSECS_PER_TICK   (NB_MS_PER_TICK * 1000000)

#define MANY_TENS_OF_MSECS (~0) /* unsigned, please */

/* useful for converting from 4.2BSD to nip units */
#define USECS_PER_TEN_MSECS (USECS_PER_MSEC*10)
#define TENS_OF_MSECS_PER_SEC (MSECS_PER_SEC/10)
#define NSECS_PER_TEN_MSECS (NSECS_PER_MSEC*10)

/* useful for converting from nip units to 4.2 units */
#define SEC(x)   (x/MSECS_PER_SEC)
#define USEC(x) ((x*USECS_PER_MSEC)%USECS_PER_SEC)


/*******************************************************/
/*          Functions Declarations            */
/*******************************************************/
bool CheckTaskInList (TKEL_task_id tk_task);
TKEL_task_id GetTaskFromList (pthread_t task);
unsigned int CurrentTime(void);
TKEL_err TKEL_MemInit(void);


#if defined(__cplusplus) 
}
#endif

