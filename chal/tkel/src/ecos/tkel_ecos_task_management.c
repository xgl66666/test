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
 * DESCRIPTION : Task Management API of Thin Kernel Encapsulation Layer (T-KEL)
 *
 *****************************************************************************/


/********************************************************/
/*          Includes                      */
/********************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>

#include "tkel.h"
#include "tkel_ecos_private.h"


#ifndef _POSIX_THREAD_PRIORITY_SCHEDULING
#error "_POSIX_THREAD_PRIORITY_SCHEDULING not defined"
#endif


/********************************************************/
/*          Defines                     */
/********************************************************/
mTBOX_SET_MODULE(eTKEL_TASK);

#define NAIS_PTHREAD_STACK_MIN (8 * 1024) /*16 Ko*/

#define TKEL_PRIO_MIN   1
#define TKEL_PRIO_MAX   127

/********************************************************/
/*          Typedefs                      */
/********************************************************/
/* THREAD management */
typedef struct {
    pthread_t threadIdent;      /* Thread identifier */
    pthread_cond_t suspend_c;
}ITC_Thread_s;


/********************************************************/
/*          Local File Variables              */
/********************************************************/
static int mInitThread = 0;            /* 0 if not initialized */
static int mNbThread = 0;              /* Nb launched threads */
static ITC_Thread_s mThread[ITC_NB_TASK];   /* Threads Identifiers */

static pthread_mutex_t mSuspendTableLock;

static TKEL_task_id mTasksList = NULL;
static pthread_mutex_t mTasksListMutex; /* mutex for tasks list */


/********************************************************/
/*          Local Functions Declarations         */
/********************************************************/
static void AddTaskInList (TKEL_task_id tk_task);
static void RemoveTaskFromList(TKEL_task_id tk_task);
static int TKEL2PthreadPriority(int policy, TKEL_prio TKELPriority);
static TKEL_prio ptread2TKELPriority(int policy, int pthreadPriority);

/********************************************************/
/*          Functions Definitions (LOCAL)         */
/********************************************************/

/*=============================================================================
 *
 * Function : CheckTaskInList
 *
 * Parameters:
 *      tk_task : a pointer on the task structure to add in the list
 *
 * Description:
 *      Check a task definition structure in tasks list
 *
 * Returns:
 *      TRUE      task exists in list
 *      FALSE   task does not exist in list
 *
 *===========================================================================*/
bool CheckTaskInList(TKEL_task_id tk_task)
{
    TKEL_task_id elt = mTasksList;

    if (mInitThread == 0)
        return FALSE;

    pthread_mutex_lock(&mTasksListMutex);
    while ((elt != NULL) && (elt != tk_task))
        elt = elt->next;
    pthread_mutex_unlock(&mTasksListMutex);

    if(elt != NULL)
        return TRUE;
    else
        return FALSE;
}

/*=============================================================================
 *
 * Function : AddTaskInList
 *
 * Parameters:
 *      tk_task : a pointer on the task structure to add in the list
 *
 * Description:
 *      Add a task definition structure in tasks list
 *
 * Returns:
 *      TKEL_NO_ERR      no error
 *      TKEL_BAD_ARG   wrong argument value
 *      TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
void AddTaskInList(TKEL_task_id tk_task)
{
    TKEL_task_id elt = mTasksList;

    if (mInitThread == 0)
        return;

    pthread_mutex_lock(&mTasksListMutex);
    tk_task->next = NULL;

    if (elt == NULL) { /* empty list */
        tk_task->prev = NULL;
        mTasksList = tk_task;
        pthread_mutex_unlock(&mTasksListMutex);
        return;
    }

    while (elt->next != NULL)
        elt = elt->next;
    tk_task->prev = elt;
    elt->next = tk_task;
    pthread_mutex_unlock(&mTasksListMutex);

    mNbThread++;
}

/*=============================================================================
 *
 * Function : RemoveTaskFromList
 *
 * Parameters:
 *      tk_task : the task to remove from the tasks list
 *
 * Description:
 *      Remove a task structure from tasks list
 *
 * Returns:
 *      TKEL_NO_ERR      no error
 *      TKEL_BAD_ARG   wrong argument value
 *      TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
void RemoveTaskFromList(TKEL_task_id tk_task)
{
    if (mInitThread == 0)
        return;

    pthread_mutex_lock(&mTasksListMutex);
    if (tk_task->prev)
        tk_task->prev->next = tk_task->next;
    else /* list head */
        mTasksList = tk_task->next;
    if (tk_task->next)
        tk_task->next->prev = tk_task->prev;
    pthread_mutex_unlock(&mTasksListMutex);

    if (mNbThread > 0)
        mNbThread--;
}


/********************************************************/
/*          Functions Definitions (GLOBAL)        */
/********************************************************/
TKEL_err TKEL_CreateTask(TKEL_prio priority,
        char *taskName,
        TKEL_uint16 stackSize,
        void (*entry) (void*),
        void *arg,
        TKEL_task_id *taskID)
{
    pthread_attr_t attr;
    struct sched_param param;
    int i;
    int ret;

    mTBOX_FCT_ENTER("TKEL_CreateTask");

    if ((taskID == NULL) || (entry == NULL) || (priority < TKEL_PRIO_MIN || priority > TKEL_PRIO_MAX))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Incorrect arguments\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

	memset(&attr,0,sizeof(pthread_attr_t));
	memset(&param,0,sizeof(struct sched_param));

    /* If this is the first time to call this function */
    if (mInitThread == 0)
	{
        mInitThread = 1;
        pthread_mutex_init(&mTasksListMutex, NULL);
        pthread_mutex_init(&mSuspendTableLock, NULL);

        pthread_mutex_lock(&mSuspendTableLock);
        for (i = 0; i < ITC_NB_TASK; i++)
		{
            mThread[i].threadIdent = 0;      /* Thread identifier */
            pthread_cond_init(&mThread[i].suspend_c, NULL);
        }
        pthread_mutex_unlock(&mSuspendTableLock);
    }

    if (TKEL_Malloc(sizeof(TKEL_task), (void**)taskID) == TKEL_NOT_DONE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]TKEL_MALLOC FAIL\n",__FUNCTION__,__LINE__));
		return TKEL_NOT_DONE;
	}
	else
	{
		memset(*taskID,0,sizeof(TKEL_task));
	}

    (*taskID)->events = 0x0000;
    if (pthread_mutex_init(&((*taskID)->evt_mutex),NULL) != 0)
	{
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init thread mutex\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    if (pthread_cond_init(&((*taskID)->evt_cond),NULL) != 0)
	{
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init thread cond\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    if (pthread_attr_init(&attr) != 0)
	{
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init thread attributes\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    if(stackSize<NAIS_PTHREAD_STACK_MIN)
    {
        stackSize = NAIS_PTHREAD_STACK_MIN;
    }

    if (pthread_attr_setstacksize( &attr, stackSize ) != 0)
    {
        pthread_attr_destroy(&attr);
        _Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init thread stack\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0)
	{
        pthread_attr_destroy(&attr);
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init thread state\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    AddTaskInList((*taskID));

	if (pthread_attr_setschedpolicy(&attr, DEFAULT_POLICY) != 0)
	{
        RemoveTaskFromList(*taskID);
        pthread_attr_destroy(&attr);
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error init thread policy\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
	}

    if (pthread_attr_getschedparam(&attr, &param) != 0)
	{
        RemoveTaskFromList(*taskID);
        pthread_attr_destroy(&attr);
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error get thread parameters (priority)\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    param.sched_priority = TKEL2PthreadPriority(DEFAULT_POLICY, priority);
    mTBOX_TRACE((kTBOX_NIV_1,"Create Thread with priority %d (=%d)\n",priority,param.sched_priority));

    if (pthread_attr_setschedparam(&attr, &param) != 0)
    {
        RemoveTaskFromList(*taskID);
        pthread_attr_destroy(&attr);
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error set thread parmeters (priority)\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
    {
        RemoveTaskFromList(*taskID);
        pthread_attr_destroy(&attr);
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error force thread inherit\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    pthread_mutex_lock(&mTasksListMutex);
    /* FIXME: gdb crashes when user is not root and cannot assign priorities freely, so temporary hack */
    if ((ret = pthread_create(&((*taskID)->task), &attr, (void* (*) (void *))entry, arg)) != 0)
    {
        pthread_mutex_unlock(&mTasksListMutex);
        RemoveTaskFromList(*taskID);
        pthread_attr_destroy(&attr);
        TKEL_Free(*taskID);
        *taskID = NULL;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error pthread_create err=%s\n", strerror(ret)));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1, "pthread_create err=%s\n", strerror(ret)));
    }
    pthread_mutex_unlock(&mTasksListMutex);

    mNbThread++;

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_DeleteTask(TKEL_task_id _taskID)
{
    TKEL_task_id taskID = _taskID;
    TKEL_task_id callerTaskID;

    mTBOX_FCT_ENTER("TKEL_DeleteTask");

    TKEL_GetCurrentTask(&callerTaskID);

    /* if no id, get current running task id */
    if (!_taskID)
    {
        TKEL_GetCurrentTask(&taskID);
    }

    if(CheckTaskInList(taskID) != TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Task does not exist in list\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (taskID == callerTaskID)
    {
        /* caller suicides itself, call exit   */
        RemoveTaskFromList(taskID);
        if ((pthread_mutex_destroy(&(taskID->evt_mutex)) != 0) || (pthread_cond_destroy(&(taskID->evt_cond)) != 0))
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error destroy thread mutex\n"));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }

        TKEL_Free(taskID);
        pthread_detach(pthread_self()); /* Tasks are created with attached attribut, so before auto kill
         * it is mandatory to change attribut to detach to release
         * thread resources allocated */
        pthread_exit(0);
    }
    else
    {
        /* caller deletes another task, wait for its end */
        if (pthread_join(taskID->task, NULL) != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error thread join\n"));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }

        if(CheckTaskInList(taskID) == TRUE)
        {
            RemoveTaskFromList(taskID);
            TKEL_Free(taskID);
        }
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/*=============================================================================
 *
 * Function : GetTaskFromList
 *
 * Parameters:
 *      task : the thread of the task to find in the tasks list
 *
 * Description:
 *      Get task description from tasks list
 *
 * Returns:
 *      TKEL_task_id   a pointer on the task definition structure
 *
 *===========================================================================*/
TKEL_task_id GetTaskFromList(pthread_t task)
{
    TKEL_task_id elt = mTasksList;

    mTBOX_FCT_ENTER("GetTaskFromList");

    if (mInitThread == 0)
        return NULL;

    pthread_mutex_lock(&mTasksListMutex);
    while (elt) {
        if (elt->task == task)
            break;
        elt = elt->next;
    }
    pthread_mutex_unlock(&mTasksListMutex);
    return elt;
}

/* ======================================================================== */
TKEL_err TKEL_GetTaskPriority(TKEL_task_id _taskID,TKEL_prio *priority)
{
    struct sched_param param;
    TKEL_task_id taskID = _taskID;
    int policy;

    mTBOX_FCT_ENTER("TKEL_GetTaskPriority");
    *priority = 1;

    if (!_taskID) TKEL_GetCurrentTask(&taskID);
    if(CheckTaskInList(taskID)!=TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Task does not exist in list\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!taskID)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the task\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else if (!(taskID->task))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the task\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    else if (pthread_getschedparam(taskID->task,&policy,&param) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the right priority value\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else
    {
        *priority = ptread2TKELPriority(policy, param.sched_priority);
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_SetTaskPriority(TKEL_task_id _taskID,TKEL_prio priority)
{
    struct sched_param param;
    TKEL_task_id taskID = _taskID;
    int policy;
    int returnValue;

    mTBOX_FCT_ENTER("TKEL_SetTaskPriority");

    if ((priority < TKEL_PRIO_MIN || priority > TKEL_PRIO_MAX))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Incorrect arguments\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!_taskID)
    {
        TKEL_GetCurrentTask(&taskID);
    }

    if(CheckTaskInList(taskID)!=TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Task does not exist in list\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!taskID)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the task\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else if (!(taskID->task)) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the task\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    else if ((returnValue = pthread_getschedparam(taskID->task,&policy,&param)) != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find back the current right priority value (%s)\n", strerror(returnValue)));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }
    else
    {
        param.sched_priority = TKEL2PthreadPriority(policy, priority);

        if (pthread_setschedparam(taskID->task,policy,&param) != 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot set the right priority value\n"));
            mTBOX_RETURN(TKEL_NOT_DONE);
        }
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_GetCurrentTask(TKEL_task_id *taskID)
{
    mTBOX_FCT_ENTER("TKEL_GetCurrentTask");
    pthread_t threadId = pthread_self();
    (*taskID) = GetTaskFromList(threadId);

    if ((*taskID)) {
        mTBOX_RETURN(TKEL_NO_ERR);
    }
    else {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Do not find the current task\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
}

/* ======================================================================== */
TKEL_err TKEL_SuspendTask()
{
    int i;
    pthread_t threadId = pthread_self();
    TKEL_err ret = TKEL_NO_ERR;

    mTBOX_FCT_ENTER("TKEL_SuspendTask");

    if ((!mNbThread) || (!mInitThread)) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No task to suspend\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    pthread_mutex_lock(&mSuspendTableLock);

    for (i = 0; i < ITC_NB_TASK; i++) {
        if (mThread[i].threadIdent == 0) {
            /* found empty slot */
            mThread[i].threadIdent = threadId;
            break;
        }
    }
    if (i != ITC_NB_TASK) {
        pthread_cond_wait(&mThread[i].suspend_c, &mSuspendTableLock);
        ret = TKEL_NO_ERR;
    }
    else {   /* There was not empty slot */
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Impossible to suspend another task\n"));
        ret = TKEL_NOT_DONE;
    }

    pthread_mutex_unlock(&mSuspendTableLock);
    mTBOX_RETURN(ret);
}

/* ======================================================================== */
TKEL_err TKEL_ResumeTask(TKEL_task_id taskID)
{
    int i;
    TKEL_err ret;
    pthread_t threadId;

    mTBOX_FCT_ENTER("TKEL_ResumeTask");

    if ((!mNbThread) || (!mInitThread)) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No task to resume\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    if (!taskID) {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!GetTaskFromList(taskID->task)) {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot find the task\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    threadId = taskID->task;

    pthread_mutex_lock(&mSuspendTableLock);

    /* search the specified thread */
    /* Is it risky to go through the whole table event if it is not full? */
    for (i = 0; i < ITC_NB_TASK; i++) {
        if (mThread[i].threadIdent == threadId) {
            /* found it */
            pthread_cond_broadcast(&mThread[i].suspend_c);
            mThread[i].threadIdent = 0;
            break;
        }
    }

    if (i != ITC_NB_TASK)
        ret = TKEL_NO_ERR;
    else {   /* The thread was not suspended before */
        mTBOX_TRACE((kTBOX_NIV_WARNING, "The task was not suspended\n"));
        ret = TKEL_NOT_DONE;
    }

    pthread_mutex_unlock(&mSuspendTableLock);
    mTBOX_RETURN(ret);
}

/* ======================================================================== */
TKEL_err TKEL_DelayTask(TKEL_tck ticks)
{
    unsigned int timeout = tick2ms(ticks);
    struct timespec timeout0;
    struct timespec timeout1;
    struct timespec* tmp;
    struct timespec* t0 = &timeout0;
    struct timespec* t1 = &timeout1;

    mTBOX_FCT_ENTER("TKEL_DelayTask");

    /* usleep can't be used for duration < 1sec - POSIX.1 -2001 declares this function has obsolete, use nanosleep instead */
    t0->tv_sec = (timeout) / 1000; /* Seconds */
    t0->tv_nsec = (timeout % 1000) * 1000000; /* nanoseconds */
    if (t0->tv_nsec >= 1000000000)
    {
        t0->tv_nsec -= 1000000000;
        t0->tv_sec++;
    }

    while (-1 == nanosleep(t0, t1))
    {
        if (errno == EINTR)
        {
            tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
        else
        {
            mTBOX_RETURN(TKEL_NOT_DONE);
        }
    }

    mTBOX_RETURN(TKEL_NO_ERR);
}

void dumpPthreadSchedParam(char* fctName)
{
    pthread_t self;
    struct sched_param param;
    int policy;

#ifdef __TKEL_THREAD_PRIO_ENABLE__
    self = pthread_self();
    if (pthread_getschedparam(self, &policy, &param) == 0)
    {
        TBOX_Print(kTBOX_LF,
                "\n%s\npolicy   %s\nparam.__sched_priority %d\n",
                fctName,
                (policy == SCHED_FIFO ? "FIFO":
                        (policy == SCHED_RR ? "RR" :
                                (policy == SCHED_OTHER ? "OTHER":
                                        "unknown"))),
                                        param.sched_priority);
        sleep(1);
    }
#endif
}

void setPthreadSchedParam(char* fctName, TKEL_prio priority)
{
    pthread_t self;
    struct sched_param param;
    int policy;

#ifdef __TKEL_THREAD_PRIO_ENABLE__
    self = pthread_self();
    policy = SCHED_FIFO;
    param.sched_priority = TKEL2PthreadPriority(policy, priority);
    if (pthread_setschedparam(self, policy, &param) == 0)
    {
        dumpPthreadSchedParam(fctName);
    }
#endif
}

static int TKEL2PthreadPriority(int policy, TKEL_prio TKELPriority)
{
    int pthreadPriority = 0;
    int priority_max;
    int priority_min;

#if defined(__TKEL_THREAD_PRIO_ENABLE__) && defined (_POSIX_THREAD_PRIORITY_SCHEDULING)
    priority_max = sched_get_priority_max(policy);
    priority_min = sched_get_priority_min(policy);

    pthreadPriority = TKELPriority * (((float)(priority_max - priority_min + 1)/(float)(TKEL_PRIO_MAX - TKEL_PRIO_MIN))) + 1;
    if (pthreadPriority >= priority_max)
        pthreadPriority = priority_max;
    else if (pthreadPriority <= priority_min)
        pthreadPriority = priority_min;

#else
    pthreadPriority = 0;
#endif

    return(pthreadPriority);
}

static TKEL_prio ptread2TKELPriority(int policy, int pthreadPriority)
{
    TKEL_prio TKELPriority = 1;
    int priority_max;
    int priority_min;

#if defined(__TKEL_THREAD_PRIO_ENABLE__) && defined (_POSIX_THREAD_PRIORITY_SCHEDULING)
    priority_max = sched_get_priority_max(policy);
    priority_min = sched_get_priority_min(policy);

    TKELPriority = pthreadPriority * (((float)(TKEL_PRIO_MAX - TKEL_PRIO_MIN)/(float)(priority_max - priority_min + 1)));
    if (TKELPriority >= TKEL_PRIO_MAX)
        TKELPriority = TKEL_PRIO_MAX;
    else if (TKELPriority <= TKEL_PRIO_MIN)
        TKELPriority = TKEL_PRIO_MIN;
#endif

    return(TKELPriority);
}

