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
 * DESCRIPTION : Task Synchronization API of Thin Kernel Encapsulation Layer (T-KEL)
 *
 *****************************************************************************/


/********************************************************/
/*          Includes                      */
/********************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "MsOS.h"

#include "tkel.h"
#include "tkel_ecos_private.h"


/********************************************************/
/*          Defines                     */
/********************************************************/
mTBOX_SET_MODULE(eTKEL_SYNC);
#ifndef PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
#endif

static void MSLEEP(time_t ms)
{
    struct timespec timeout0, timeout1;
    struct timespec *t0 = &timeout0, *t1 = &timeout1;
    t0->tv_sec = (ms) / 1000;
    t0->tv_nsec = ((ms) % 1000) * 1000000;

    if (t0->tv_nsec >= 1000000000)
    {
        t0->tv_nsec -= 1000000000;
        t0->tv_sec++;
    }

    while (-1 == nanosleep(t0, t1)) {
        if (errno == EINTR)
            t0 = t1;
        else break;
    }
}

/********************************************************/
/*          Typedefs                      */
/********************************************************/

/* Critical section management */
typedef struct {
    TKEL_task_id taskID;
    TKEL_int16 LockTimes; /* number of times the task has gone in critical section */
    int policy;         /* previous task policy */
    TKEL_prio priority;   /* previous task priority */
}CriticalStatus_s;

/* Semaphore */
typedef struct {
    int      waitCounter;
    sem_t   posixSema;
}TKEL_sema_t;

/********************************************************/
/*          Local File Variables              */
/********************************************************/
static CriticalStatus_s mCriticalStatus =
{
        0,
        0,
        DEFAULT_POLICY,
        0
};
static pthread_mutex_t mCriticalMutex; /* mutex for critical section */
static int mInitCritical = 0;

/********************************************************/
/*          Functions Definitions (GLOBAL)        */
/********************************************************/

/* ======================================================================== */
/* === Semaphores === */
/* ======================================================================== */
TKEL_err TKEL_CreateSemaphore(TKEL_int16 count,TKEL_sema_id *semaphoreID)
{
    TKEL_err   res = TKEL_NO_ERR;
    TKEL_sema_t *tkelSema = NULL;

    mTBOX_FCT_ENTER("TKEL_CreateSemaphore");

    /* Allocation */
	if (TKEL_Malloc(sizeof(TKEL_sema_t), (void**)&tkelSema) != TKEL_NO_ERR)
	{
		printf("[%s %d]Tkel malloc size %d failed\n",__FUNCTION__,__LINE__,sizeof(TKEL_sema_t));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

    *semaphoreID = (TKEL_sema_id)tkelSema;

    if (*semaphoreID == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Semaphoresa allocation failed\n"));
        res = TKEL_NOT_DONE;
    }
    else
    {
        /* Semaphore initialization */
        if (sem_init(&(tkelSema->posixSema), 0, count) != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init semaphore (%s)\n", strerror(errno)));
            TKEL_Free(tkelSema);
            res = TKEL_NOT_DONE;
        }

        tkelSema->waitCounter = 0;
    }

    mTBOX_RETURN(res);
}

/* ======================================================================== */
TKEL_err TKEL_DeleteSemaphore(TKEL_sema_id semaphoreID)
{
    TKEL_sema_t *tkelSema = (TKEL_sema_t *)semaphoreID;

    mTBOX_FCT_ENTER("TKEL_DeleteSemaphore");

    if(NULL == tkelSema)
    {
        return TKEL_BAD_ARG;
    }
    if (tkelSema->waitCounter > 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Tasks are blocked on the semaphore\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else
    {
        if (sem_destroy(&(tkelSema->posixSema)) != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot destroy the semaphore (%s)\n", strerror(errno)));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }

        /* Cleanup */
        TKEL_Free(tkelSema);
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_SignalSemaphore(TKEL_sema_id semaphoreID)
{
    TKEL_sema_t *tkelSema = (TKEL_sema_t *)semaphoreID;

    mTBOX_FCT_ENTER("TKEL_SignalSemaphore");

    if(NULL == tkelSema)
    {
        return TKEL_BAD_ARG;
    }
    if (sem_post(&(tkelSema->posixSema)) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable to signal the semaphore (%s)\n", strerror(errno)));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_WaitSemaphore(TKEL_sema_id semaphoreID)
{
    int       returnValue;
    TKEL_sema_t *tkelSema = (TKEL_sema_t *)semaphoreID;

    mTBOX_FCT_ENTER("TKEL_WaitSemaphore");

    if(NULL == tkelSema)
    {
        return TKEL_BAD_ARG;
    }
    tkelSema->waitCounter++;
    while ((returnValue = sem_wait(&(tkelSema->posixSema))) == -1 && errno == EINTR)
        continue;      /* Restart when interrupted by handler */
    if (returnValue != 0)
    {
        tkelSema->waitCounter--;

        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable to wait for the semaphore (%s)\n", strerror(errno)));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    tkelSema->waitCounter--;

    mTBOX_RETURN(TKEL_NO_ERR);
}

/*=============================================================================
 *
 * Function : TKEL_WaitSemaphoreTimeout
 *
 * Parameters:
 *   semaphoreID   semaphore id
 *   ticks      Timeout in ticks
 *
 * Description:
 *      Wait a semaphore token for a period of time.
 *      If value of ticks is 0, function waits indefinitely.
 *
 * Returns:
 *   TKEL_NO_ERR      no error
 *   TKEL_BAD_ARG   wrong argument value
 *   TKEL_TIMEOUT   the delay is down
 *
 * Warning:
 *   attention : comportement inconnu si timeout < 10
 *   de plus, ce n'est pas tres precis ...
 *
 *===========================================================================*/
#if defined(ST_OSLINUX) || defined(MARVELL_OSLINUX)
#define _XOPEN_SOURCE 601
#endif
TKEL_err TKEL_WaitSemaphoreTimeout(TKEL_sema_id semaphoreID, TKEL_tck ticks)
{
#if !defined(_XOPEN_SOURCE) || (_XOPEN_SOURCE < 600)
//printf("->[m] TKEL_WaitSemaphoreTimeout run 1 \r\n");//mz
    unsigned int timeout = 0;
    unsigned int timestart = 0;
    unsigned int timeend = 0;
    int sem_res;

    TKEL_sema_t *tkelSema = (TKEL_sema_t *)semaphoreID;

    mTBOX_FCT_ENTER("TKEL_WaitSemaphoreTimeout");

    sem_res = sem_trywait(&(tkelSema->posixSema));
    if (ticks != kTKEL_TIMEOUT_IMMEDIATE)
    {
        timestart = CurrentTime();
        /* TBD - case when CurrentTime is in proximity of overflow of clock counting*/
        if (ticks == kTKEL_TIMEOUT_INFINITE)
        {
            timeout = 0xFFFFFFFF;
            timeend = 0xFFFFFFFF;
        }
        else
        {
            timeout = tick2ms(ticks);
            timeend = timestart + timeout;
        }

        while( sem_res != 0 )
        {
            /* TBD */
            MSLEEP(1);
            if (timeend < (CurrentTime()))
            {
                break;
            }

            sem_res = sem_trywait(&(tkelSema->posixSema));
        }
    }

    if (sem_res == 0)
    {
        mTBOX_RETURN(TKEL_NO_ERR);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]Semaphore timeout has expired,ticks=%d,timeout=%d,timestart=%d,timeend=%d\n",__FUNCTION__,__LINE__,ticks,timeout,timestart,timeend));
        mTBOX_RETURN(TKEL_TIMEOUT);
    }
#else
    TKEL_sema_t      *tkelSema = (TKEL_sema_t *)semaphoreID;
    unsigned int   timeout;
    struct timespec ts;
    struct timeval   now;
    int          errno;
    int          sem_result;
    TKEL_err      result;

    mTBOX_FCT_ENTER("TKEL_WaitSemaphoreTimeout");

    if(NULL == tkelSema)
    {
        return TKEL_BAD_ARG;
    }
    tkelSema->waitCounter++;
    if (kTKEL_TIMEOUT_INFINITE == ticks)
    {
        while ((sem_result = sem_wait(&(tkelSema->posixSema))) == -1 && errno == EINTR)
            continue;      /* Restart when interrupted by handler */
    }
    else
    {
        gettimeofday(&now, 0);

        if (kTKEL_TIMEOUT_IMMEDIATE == ticks)
        {
            ts.tv_sec = now.tv_sec;
            ts.tv_nsec = now.tv_usec * USECS_PER_MSEC;
        }
        else
        {
            timeout = tick2ms(ticks);
            ts.tv_sec = now.tv_sec + timeout / 1000;
            ts.tv_nsec = now.tv_usec * USECS_PER_MSEC
                    + (timeout % 1000) * 1000000;
            if (ts.tv_nsec >= 1000000000)
            {
                ts.tv_nsec -= 1000000000;
                ts.tv_sec++;
            }
        }

        while ((sem_result = sem_timedwait(&(tkelSema->posixSema), &ts)) == -1 && errno == EINTR)
            continue;      /* Restart when interrupted by handler */
    }
    tkelSema->waitCounter--;

    if (sem_result == 0)
    {
        result = TKEL_NO_ERR;
    }
    else
    {
        if (errno == ETIMEDOUT)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Semaphore timeout has expired\n"));
            result = TKEL_TIMEOUT;
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Unable to wait for the semaphore (%s)\n", strerror(errno)));
            result = TKEL_NOT_DONE;
        }
    }

    mTBOX_RETURN(result);
#endif
}

/* ======================================================================== */
/* === Critical sections === */
/* ======================================================================== */
TKEL_err TKEL_Critical(TKEL_status *status)
{
    TKEL_task_id taskID;
    int policy;
    struct sched_param param;

    mTBOX_FCT_ENTER("TKEL_Critical");

    if (mInitCritical == 0) {
        mInitCritical = 1;
        *status = TKEL_NORMAL;
        pthread_mutex_init(&mCriticalMutex, NULL);
    }

    if (pthread_mutex_lock(&mCriticalMutex) != TKEL_NO_ERR) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot take the mutex for critical section\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    TKEL_GetCurrentTask(&taskID);

    if ( (mCriticalStatus.LockTimes != 0) && (mCriticalStatus.taskID != taskID) ) {
        pthread_mutex_unlock(&mCriticalMutex);
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Another task is in critical section\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    /* It is not the 1rst time that the task locks the section */
    if (mCriticalStatus.LockTimes != 0) {
        mCriticalStatus.LockTimes++;
        *status = TKEL_CRITICAL;
        mTBOX_RETURN(TKEL_NO_ERR);
    }
    else
    {
        if (pthread_getschedparam(taskID->task,&policy,&param) != 0) {
            pthread_mutex_unlock(&mCriticalMutex);
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the task status\n"));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }

        /* register the status of the task before the critical section */
        mCriticalStatus.policy = policy;
        mCriticalStatus.priority = param.sched_priority;

        policy = CRITICAL_POLICY;
        param.sched_priority = sched_get_priority_max(CRITICAL_POLICY);

        /* with the new parameters, the task will be priority */
        if (pthread_setschedparam(taskID->task,policy,&param) != 0) {
            pthread_mutex_unlock(&mCriticalMutex);
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot change the task status\n"));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }

        mCriticalStatus.taskID = taskID;
        mCriticalStatus.LockTimes++;

        *status = TKEL_CRITICAL;
        mTBOX_RETURN(TKEL_NO_ERR);
    }
}

/* ======================================================================== */
TKEL_err TKEL_Normal(TKEL_status status)
{
    TKEL_task_id taskID;
    int policy;
    struct sched_param param;
    TKEL_err ret = TKEL_NO_ERR;

    mTBOX_FCT_ENTER("TKEL_Normal");

    TKEL_GetCurrentTask(&taskID);

    if ( (mCriticalStatus.LockTimes <= 0) || (mCriticalStatus.taskID != taskID) ) {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "The task is already in normal mode\n"));
        ret = TKEL_NOT_DONE;
    }

    /* The task has finished to unlock as many times as it locks the critical section */
    else if (mCriticalStatus.LockTimes == 0) {
        policy = mCriticalStatus.policy;
        param.sched_priority = mCriticalStatus.priority;

        /* the task gets back its previous parameters */
        if (pthread_setschedparam(taskID->task,policy,&param) != 0) {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot set back the task status\n"));
            ret = TKEL_NOT_DONE;
        }
    }

    mCriticalStatus.LockTimes--;
    pthread_mutex_unlock(&mCriticalMutex);

    mTBOX_RETURN(ret);
}

/* ======================================================================== */
/* === Events === */
/* ======================================================================== */
TKEL_err TKEL_SendEvent(TKEL_task_id taskID,TKEL_evts events)
{
    int retVal = TKEL_NO_ERR;

    mTBOX_FCT_ENTER("TKEL_SendEvent");

    if (!taskID) {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    else if(CheckTaskInList(taskID)!=TRUE) {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Task does not exist in list\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!GetTaskFromList(taskID->task)) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the task\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    pthread_mutex_lock(&(taskID->evt_mutex));
    taskID->events |= events;

    if (taskID->events != 0x00)
        pthread_cond_broadcast(&(taskID->evt_cond));
    else {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No valid event to send\n"));
        retVal = TKEL_NOT_DONE;
    }

    pthread_mutex_unlock(&(taskID->evt_mutex));

    mTBOX_RETURN(retVal);
}

/* ======================================================================== */
TKEL_err TKEL_SendITEvent(TKEL_task_id taskID,TKEL_evts events)
{
    mTBOX_FCT_ENTER("TKEL_SendITEvent");

    mTBOX_RETURN(TKEL_NOT_DONE);
}

/* ======================================================================== */
TKEL_err TKEL_ReceiveEvent(TKEL_tck ticks, TKEL_evts *events)
{
    TKEL_task_id taskID = GetTaskFromList(pthread_self());
    unsigned int timeout = tick2ms(ticks);
    struct timespec ts;
    unsigned int now;
    int retVal;

    mTBOX_FCT_ENTER("TKEL_ReceiveEvent");

    if (!events) {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!taskID) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the current task\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    pthread_mutex_lock(&(taskID->evt_mutex));
    if (taskID->events != 0x00) {
        *events = taskID->events;
        taskID->events = 0x00;
        pthread_mutex_unlock(&(taskID->evt_mutex));
        mTBOX_RETURN(TKEL_NO_ERR);
    }

    if (ticks > 0) {
        // get current time in tens of miliseconds
    	now = CurrentTime();

        ts.tv_sec = (now + timeout) / MSECS_PER_SEC;
        ts.tv_nsec = ((now + timeout) % MSECS_PER_SEC) * NSECS_PER_MSEC;

        if (ts.tv_nsec >= 1000000000) {
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }
    }

    if (ticks > 0)
        retVal = pthread_cond_timedwait(&(taskID->evt_cond),&(taskID->evt_mutex),&ts);
    else
        retVal = pthread_cond_wait(&(taskID->evt_cond),&(taskID->evt_mutex));

    *events = taskID->events;
    taskID->events = 0x00;
    pthread_mutex_unlock(&(taskID->evt_mutex));

    if (retVal == 0) {
        mTBOX_RETURN(TKEL_NO_ERR);
    }

    else if (retVal == ETIMEDOUT) {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Timeout has been reached\n"));
        mTBOX_RETURN(TKEL_TIMEOUT);
    }

    else {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Wait process failed\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
}

/* ======================================================================== */
TKEL_err TKEL_ResetEvent()
{
    TKEL_task_id taskID = GetTaskFromList(pthread_self());

    mTBOX_FCT_ENTER("TKEL_ResetEvent");

    pthread_mutex_lock(&(taskID->evt_mutex));
    taskID->events = 0x00;
    pthread_mutex_unlock(&(taskID->evt_mutex));

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
/* === Mutexes === */
/* ======================================================================== */
TKEL_err TKEL_CreateMutex(TKEL_mutex_id *MutexID)
{
    TKEL_err         res = TKEL_NO_ERR;
    pthread_mutexattr_t attr;
    pthread_mutex_t *pMutexId = NULL;

    mTBOX_FCT_ENTER("TKEL_CreateMutex");

    if (!MutexID)
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    /* Allocation */
	if (TKEL_Malloc(sizeof(TKEL_mutex_t), (void**)MutexID) != TKEL_NO_ERR)
	{
		printf("[%s %d]Tkel malloc size %d failed\n",__FUNCTION__,__LINE__,sizeof(TKEL_mutex_t));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

    if (*MutexID == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Mutex allocation failed\n"));
        res = TKEL_NOT_DONE;
    }
    else
    {
        pMutexId = &(*MutexID)->mutex;
        /* Mutex initialization */
        pthread_mutexattr_init(&attr);

        /* ECOS does not support this function*/
        //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        if (pthread_mutex_init(pMutexId, &attr) != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init mutex\n"));
            res = TKEL_NOT_DONE;
        }
        if (res == TKEL_NO_ERR)
        {
            (*MutexID)->cnt = 0;
            (*MutexID)->owner = 0;
        }
    }

    mTBOX_RETURN(res);
}

/* ======================================================================== */
TKEL_err TKEL_DeleteMutex(TKEL_mutex_id MutexID)
{
    pthread_mutex_t *pMutexId = &MutexID->mutex;
    mTBOX_FCT_ENTER("TKEL_DeleteMutex");

    if (!MutexID)
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    /* Destroy mutex */
    if (pthread_mutex_destroy(pMutexId) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot destroy the mutex\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else
    {
        /* Cleanup */
        TKEL_Free(MutexID);
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_LockMutex(TKEL_mutex_id MutexID)
{
    pthread_mutex_t *pMutexId = NULL;
    int err = -1;

    mTBOX_FCT_ENTER("TKEL_LockMutex");

    if (!MutexID)
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    pMutexId = &MutexID->mutex;

    err = pthread_mutex_lock(pMutexId);

    if (err!=0 && err!=EDEADLK)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot lock the mutex\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else if (err == 0)
    {
        MutexID->owner = (pthread_t)MsOS_InfoTaskID();
    }

    MutexID->cnt++;

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_UnlockMutex(TKEL_mutex_id MutexID)
{
    pthread_mutex_t *pMutexId = NULL;
    int pthread_id = MsOS_InfoTaskID();

    mTBOX_FCT_ENTER("TKEL_UnlockMutex");

    if (!MutexID)
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    pMutexId = &MutexID->mutex;

    if (pthread_id==MutexID->owner && --MutexID->cnt==0)
    {
        if (pthread_mutex_unlock(pMutexId) != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot unlock the mutex\n"));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_TryLockMutex(TKEL_mutex_id MutexID)
{
    pthread_mutex_t *pMutexId = NULL;
    int err = -1;

    mTBOX_FCT_ENTER("TKEL_TryLockMutex");

    if (!MutexID)
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    pMutexId = &MutexID->mutex;

    err = pthread_mutex_trylock(pMutexId);

    if (err!=0 && err!=EDEADLK)
    {

        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot lock the mutex\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else if (err == 0)
    {
        MutexID->owner = (pthread_t)MsOS_InfoTaskID();

    }
    MutexID->cnt++;

    mTBOX_RETURN(TKEL_NO_ERR);
}

