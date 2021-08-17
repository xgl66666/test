/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES              *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/sources/hal/tkel/trunk/inc/tkel.h $
 *        $Rev: 61434 $
 *        $Date: 2011-01-14 16:28:57 +0100 (ven., 14 janv. 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: definition of the API of Thin Kernel Encapsulation Layer
 *
 *   LIPPA - December 2010 
 *         To implemente the mapping SmarDTV DAL OS -> TKEL, add 3 
 *         management task functions: TKEL_CreateTask_UnlimitStack,
 *         TKEL_ExitTask and TKEL_KillTask.
 *
 *****************************************************************************/

#ifndef _TKEL_H_
#define _TKEL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(TKEL_windows)||defined(TKEL_linux)
   #include "tkel_pc.h"
#elif defined(TKEL_os21)
   #include "tkel_os21.h"
#elif defined(TKEL_ecos)
   #include "tkel_ecos.h"
#elif defined(TKEL_stub)
   #include "tkel_stub.h"
#else
   #error "TKEL-ERROR: Undefined Operating System!"
#endif

#if defined(ST_OSLINUX)
#if defined(MODULE)
#undef MODULE
#endif
#define MODULE
#endif


/*=============================== Public types ===============================*/

/* TKEL error codes */
typedef enum
{
   TKEL_NO_ERR,   /* No error. */
   TKEL_TIMEOUT,   /* A timeout occurred. */
   TKEL_BAD_ARG,   /* Wrong argument value(s). */
   TKEL_NOT_DONE   /* Command not done. */
}TKEL_err;


/* TKEL execution status */
typedef enum
{
   TKEL_NORMAL,   /* Normal execution. */
   TKEL_CRITICAL   /* Critical region. */
}TKEL_status;


typedef struct _TKEL_ExceptionReg
{
    char name[6]; // string which can be used alongside value
    int value;
}tTKEL_ExceptionReg;

typedef struct _TKEL_ExceptionInfo
{
    char savedRegsCount;
    tTKEL_ExceptionReg savedRegs[40];
}tTKEL_ExceptionInfo;

/*============================= Public functions =============================*/

/* === Task management === */
TKEL_err TKEL_Init(void);
TKEL_err TKEL_Term(void);

TKEL_err TKEL_CreateTask(   TKEL_prio priority,      /* Creates and starts a task. */
        char *taskName,
        TKEL_uint16 stackSize,
        void (*entry) (void*),
        void *arg,
        TKEL_task_id *taskID);
TKEL_err TKEL_CreateTask_UnlimitStack (   TKEL_prio priority,      /* Idem CreateTask but with an unlimited stack size. */
        char *taskName,
        TKEL_msize stackSize,
        void (*entry) (void*),
        void *arg,
        TKEL_task_id *taskID);

TKEL_err TKEL_ExitTask(TKEL_task_id taskID,         /* Exit current task (required by some OS at the end of main task) */
              TKEL_int32 exitStatus);
TKEL_err TKEL_KillTask(TKEL_task_id taskID);        /* Kill a task (required by some OS   before deletion of task) */
TKEL_err TKEL_DeleteTask(TKEL_task_id taskID);       /* Deletes a task. */

TKEL_err TKEL_GetTaskPriority(TKEL_task_id taskID,     /* Gets a task's priority. */
         TKEL_prio *priority);

TKEL_err TKEL_SetTaskPriority(TKEL_task_id taskID,     /* Changes a task's priority. */
         TKEL_prio priority);

TKEL_err TKEL_GetCurrentTask(TKEL_task_id *taskID);     /* Get the ID of the current running task. */

TKEL_err TKEL_SuspendTask(void);              /* Suspends the current task. */

TKEL_err TKEL_ResumeTask(TKEL_task_id taskID);       /* Resumes a task. */

TKEL_err TKEL_DelayTask(TKEL_tck ticks);         /* Suspends the current running task for a period of time. */


/* === Task synchronization === */

/* Semaphores */

TKEL_err TKEL_CreateSemaphore(TKEL_int16 count,         /* Creates a semaphore. */
         TKEL_sema_id *semaphoreID);

TKEL_err TKEL_DeleteSemaphore(TKEL_sema_id semaphoreID);   /* Deletes a semaphore. */

TKEL_err TKEL_SignalSemaphore(TKEL_sema_id semaphoreID);   /* Releases a semaphore token. */

TKEL_err TKEL_WaitSemaphore(TKEL_sema_id semaphoreID);     /* Acquires a semaphore token. */

TKEL_err TKEL_WaitSemaphoreTimeout(TKEL_sema_id semaphoreID, TKEL_tck ticks);     /* Acquires a semaphore token. */

/* Critical sections */

TKEL_err TKEL_Critical(TKEL_status *status);     /* Begins a critical region. */

TKEL_err TKEL_Normal(TKEL_status status);        /* Ends a critical region. */

/* Events */

TKEL_err TKEL_SendEvent(TKEL_task_id taskID,   /* Sends events to a target task. */
        TKEL_evts events);

TKEL_err TKEL_SendITEvent(TKEL_task_id taskID,   /* Sends events from an IT to a target task. */
        TKEL_evts events);

TKEL_err TKEL_ReceiveEvent(TKEL_tck ticks,     /* Receives events */
         TKEL_evts *events);

TKEL_err TKEL_ResetEvent(void);          /* Resets the pending events. */

/* Mutexes */

TKEL_err TKEL_CreateMutex(TKEL_mutex_id * MutexID);

TKEL_err TKEL_DeleteMutex(TKEL_mutex_id MutexID);

TKEL_err TKEL_LockMutex(TKEL_mutex_id MutexID);

TKEL_err TKEL_UnlockMutex(TKEL_mutex_id MutexID);

TKEL_err TKEL_TryLockMutex(TKEL_mutex_id MutexID);

/* === Task communications === */

TKEL_err TKEL_CreateQueue(TKEL_qdepth maxNumber,     /* Creates a message queue. */
        TKEL_qwidth msgSize,
        TKEL_queue_id *queueID);

TKEL_err TKEL_CreateQueueTimeout(TKEL_qdepth maxNumber,     /* Creates a message queue with timeout. */
        TKEL_qwidth msgSize,
        TKEL_queue_id *queueID);

TKEL_err TKEL_DeleteQueue(TKEL_queue_id queueID);     /* Deletes a message queue. */

TKEL_err TKEL_Enqueue(TKEL_queue_id queueID,       /* Posts a message to a queue. */
        void *message);

TKEL_err TKEL_Dequeue(TKEL_queue_id queueID,       /* Requests a message from a queue. */
        void *message);

TKEL_err TKEL_EnqueueTimeout(TKEL_queue_id queueID,       /* Posts a message to a queue. */
        void *message,
        TKEL_tck ticks);

TKEL_err TKEL_DequeueTimeout(TKEL_queue_id queueID,       /* Requests a message from a queue. */
        void *message,
        TKEL_tck ticks);

TKEL_err TKEL_GetMsgNumber(TKEL_queue_id queueID,     /* Gets the number of message in a queue. */
         TKEL_int16 *msgNumber);

TKEL_err TKEL_NotifyQueueEvent(TKEL_queue_id queueID,
            TKEL_task_id taskID,
            TKEL_evts events);

/* === Memory management === */

void* TKEL_GetMemoryPartition(void);

#ifdef USE_TKEL_MEMORY_PARTITIONS
/* === Memory partitions management === */

TKEL_err TKEL_CreateFixedSizeMemPart(      /* Creates a memory partition. */
         TKEL_uint32     bufferSize,
         TKEL_uint32     memPartSize,
         void*         memPartAddress,
         TKEL_memPart_id   *memPartID);

TKEL_err TKEL_DeleteMemPart(
         TKEL_memPart_id   memPartID);   /* Deletes a memory partition. */

TKEL_err TKEL_MemPartAlloc(            /* Allocates a memory buffer. */
         TKEL_memPart_id   memPartID,
         TKEL_msize     memorySize,
         void        **buffer);

TKEL_err TKEL_MemPartFree(            /* Releases a memory buffer. */
         TKEL_memPart_id   memPartID,
         void        *buffer);

TKEL_err TKEL_MemPartCheckBuffer(         /* Check a memory buffer. */
         TKEL_memPart_id   memPartID,
         void        *buffer);

#endif /* USE_TKEL_MEMORY_PARTITIONS */

/* === Timer management === */

TKEL_err TKEL_CreateTimer(TKEL_timer_id *timerID);     /* Creates a timer. */

TKEL_err TKEL_DeleteTimer(TKEL_timer_id timerID);     /* Deletes a timer. */

TKEL_err TKEL_StartTimer(   TKEL_timer_id timerID,     /* Starts a timer. */
        TKEL_tck ticks,
        TKEL_evts events);

TKEL_err TKEL_StartTimerWithCB( TKEL_timer_id timerID,   /* Starts a timer with callback on timeout */
         TKEL_tck ticks, TKEL_timer_CB callback);

TKEL_err TKEL_StopTimer(TKEL_timer_id timerID);      /* Stops a timer. */

/*----------------------------------------------------------*/
/* This function must be called after TKEL_StartTimer     */
/* else the TaskId will be replaced by the TKEL_StartTimer   */
/* caller task Id                        */
/*----------------------------------------------------------*/

TKEL_err TKEL_SetTimerOwnerTask(TKEL_timer_id timerID,
                   TKEL_task_id taskID);   /* Set the Owner of the timer. */


/* === ISR Management === */

TKEL_frame TKEL_EnterIT(TKEL_frame frame);       /* Enter an Interrupt Service Routine. */

TKEL_frame TKEL_ExitIT(TKEL_frame frame);        /* Exit an Interrupt Service Routine. */


/* === Miscellaneous === */

void TKEL_Abort(TKEL_cause cause);       /* Aborts and enters fatal error handling mode. */

TKEL_err TKEL_SetExceptionHandler(void (*function)(void), tTKEL_ExceptionInfo** exceptionInfoLocation); /* Sets function to be called in exception handling, used by some OS */

TKEL_err TKEL_GetTick(TKEL_tck *pTick);

#ifdef __DEBUG_MEM_LEAK__ /* Add utitily for memory leak debbuging */
#define kTM_OPE_ALL   0
#define kTM_OPE_NEW   1
#define kTM_OPE_FREE   2
#define kTM_OPE_DEL   3
#define kTM_OPE_NB     4

#define TKEL_Malloc(_s,_b) TKEL_DebugMalloc(__FILE__,__LINE__,(_s),(_b))
#define TKEL_Free(_b)     TKEL_DebugFree(__FILE__,__LINE__,(_b))
#define _Malloc(_s)     TKEL_DebugNatifMalloc(__FILE__,__LINE__,(_s))
#define _Free(_p)       TKEL_DebugNatifFree(__FILE__,__LINE__,(_p))
#define _New(_p,_t)     _p = new _t; TKEL_TraceMemory(__FILE__,__LINE__,_p,0,kTM_OPE_NEW)
#define _Delete(_p)     TKEL_TraceMemory(__FILE__,__LINE__,(_p),0,kTM_OPE_DEL); delete _p
#define _DelArr(_p)     TKEL_TraceMemory(__FILE__,__LINE__,(_p),0,kTM_OPE_DEL); delete[] _p

/* Allocates a memory buffer : basic malloc and malloc with debug */
TKEL_err TKEL_BasicMalloc     (TKEL_msize memorySize, void **ppBuffer);
TKEL_err TKEL_DebugMalloc     (const char *zFile, int iLine,
                   TKEL_msize memorySize, void **ppBuffer);
void   *TKEL_DebugNatifMalloc (const char *zFile, int iLine,
                   size_t iSize);

/* Releases a memory buffer : basic free and free with debug */
TKEL_err TKEL_BasicFree     (void *pBuffer);
TKEL_err TKEL_DebugFree     (const char *zFile, int iLine, void *pBuffer);
void     TKEL_DebugNatifFree   (const char *zFile, int iLine, void *pBuffer);

/* Trace all malloc and free */
void TKEL_TraceMemory     (const char *zFile, int iLine, void *Ptr, int iSize,
            int iOpe);
void TKEL_StartTraceMemory ();
void TKEL_StopTraceMemory   ();

#else
/* Allocates a memory buffer. */
TKEL_err TKEL_Malloc (TKEL_msize memorySize, void **buffer);

/* Releases a memory buffer. */
TKEL_err TKEL_Free   (void *buffer);

#define _Malloc     malloc
#define _Free     free
#define _New(_p,_t) _p = new _t
#define _Delete(_p) delete _p
#define _DelArr(_p) delete[] _p

#endif


#ifndef ms2tick
#define ms2tick(milliseconds)   (TKEL_tck) ((milliseconds) / NB_MS_PER_TICK)
#endif   /* #ifndef ms */

#ifndef tick2ms
#define tick2ms(ticks)       (NB_MS_PER_TICK * (ticks))
#endif

#define kTKEL_TIMEOUT_INFINITE   0
#define kTKEL_TIMEOUT_IMMEDIATE 1


/* Module trace identifiers */
typedef enum
{
   eTKEL_COM = 150,   /* task communication (message queues) */
   eTKEL_MEMORY,     /* memory managed by TKEL */
   eTKEL_TASK,      /* task management (create, delay, suspend,...) */
   eTKEL_TIMER,     /* timer mangement */
   eTKEL_SYNC,      /* task synchronisation (semaphore, mutex,...) */
   eTKEL_MISC       /* miscellaneous (get tick, initialisation,...) */

}tTKEL_ModuleTraceID;


#if defined(__cplusplus)
}
#endif

#endif /* _TKEL_H_ */
