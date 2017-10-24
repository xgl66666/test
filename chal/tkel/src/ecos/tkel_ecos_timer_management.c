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
 * DESCRIPTION : Timer Management API of Thin Kernel Encapsulation Layer (T-KEL)
 *
 *****************************************************************************/


/********************************************************/
/*          Includes                      */
/********************************************************/
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include "tkel.h"
#include "tkel_ecos_private.h"


/********************************************************/
/*          Defines                     */
/********************************************************/
mTBOX_SET_MODULE(eTKEL_TIMER);

#define TIMER_SIGNAL_TYPE SIGALRM
#define TIMER_INTERVAL_TYPE ITIMER_REAL

#ifndef kTKEL_STOP
#define kTKEL_STOP      0
#endif /* kTKEL_STOP */

#ifndef kTKEL_RUNNING
#define kTKEL_RUNNING   (!kTKEL_STOP)
#endif /* kTKEL_RUNNING */

#define kTKEL_TIMER_TASK_PRIO 30
/********************************************************/
/*          Macros                        */
/********************************************************/
#ifdef __CYGWIN__
#define disable_interrupts()
#define enable_interrupts()
#else
#define disable_interrupts() sigemptyset(&mNewMask);\
        sigaddset(&mNewMask, TIMER_SIGNAL_TYPE); \
        sigprocmask(SIG_BLOCK, &mNewMask, &mOldMask)
#define enable_interrupts() sigprocmask(SIG_BLOCK, &mOldMask, 0)
#endif


/********************************************************/
/*          Typedefs                      */
/********************************************************/

/* Timers management */
typedef struct {
    TKEL_timer_id   timerID;
    TKEL_uint32     timeout;   /* clock ticks to wait for */
    TKEL_task_id    task_to_send_event;
    TKEL_evts       event_to_send;
    TKEL_timer_CB   callback;
    int inuse;
}Timer_s;


/********************************************************/
/*          Local Functions Declarations         */
/********************************************************/
static void StartTimer(Timer_s *t);
static void CancelItimer(void);
static void UpdateAllTimersBy(unsigned int time);
static Timer_s *ShortestTimer(void);
static void clk_isr(int sig);

static int setitimer(struct itimerval interval);
static int create_timer(void);

/********************************************************/
/*          Local File Variables              */
/********************************************************/
static Timer_s mTimer[ITC_NB_TIMERS];      /* timer queue */

static struct itimerval mTimerValue;

/* this holds the value of CurrentTime() */
static unsigned mTimerSetTimestamp;

Timer_s *mNextTimer;       /* timer we expect to run down next */

void TimerTask(void * arg);   /* Timers task */
extern TKEL_task_id mTimerTask;      /* Timers management task */
extern TKEL_sema_id mTimerSema;      /* Timers management semaphore */

void CallbackTask(void * arg);   /* Callback task (used when callback is associated to a timer */
TKEL_task_id mCallbackTask;      /* Callback management task */

/* used with the interrupt from the timer */
static sigset_t mNewMask;         /* signal mask */
static sigset_t mOldMask;         /* signal mask */

/* used when cancelling timers (if that works) */
struct itimerval mCancelTimer =
{
        {0, 0},
        {0, 0}
};

static TKEL_uint8 mTimerStatus = kTKEL_STOP;

static TKEL_sema_id mTimerMutex;      /* mutex for timer table */
static int mTimerInitialized = 0;      /* 0 if not intialized */


/********************************************************/
/*          Functions Definitions (LOCAL)         */
/********************************************************/
static void StartTimer(Timer_s *t)
{
    mNextTimer = t;
    if (!t)
        return;

    mTimerSetTimestamp = CurrentTime();

    mTimerValue.it_value.tv_sec = SEC(tick2ms(mNextTimer->timeout));
    mTimerValue.it_value.tv_usec = USEC(tick2ms(mNextTimer->timeout));
    /* TBD */
    if (-1 == setitimer(mTimerValue))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "setitimer error\n"));
    }
    return;
}

/* ======================================================================== */
static void CancelItimer(void)
{
    /* TBD */
    if (-1 == setitimer(mCancelTimer))
        return;

    UpdateAllTimersBy(CurrentTime() - mTimerSetTimestamp);
}

/* ======================================================================== */
/* clock interrupt handler */
/* this routine is executed when an alarm signal occurs */
/* ======================================================================== */
static void clk_isr(int sig)
{
    if (mNextTimer == 0)
        return;

    TKEL_SignalSemaphore(mTimerSema);
}

/* ======================================================================== */
/* subtract time from all timers, enabling any that run out along the way */
/* ======================================================================== */
static void UpdateAllTimersBy(unsigned int time)
{
    Timer_s *t;
    /*TKEL_task_id mCallbackTask;*/

    for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->timeout != 0) {
            if (ms2tick(time) < t->timeout)
                t->timeout -= ms2tick(time);
            else
            {
                t->timeout = 0;
                /* if this has forced a timeout on */
                /* anyone else, enable it */
                if (t->inuse == 1)
                {
                    if (t->event_to_send != 0x00)
                        TKEL_SendEvent(t->task_to_send_event,t->event_to_send);
                    else if (t->callback)
                        /* if a callback has to be launched, we create a task for it, in order to let the TaskTimer follow its processing */
                        /*TKEL_CreateTask(1,0,0,CallbackTask,(void *)(t->callback),&mCallbackTask);*/
                        (t->callback)();
                }
            }
        }
    }
}

/* ======================================================================== */
static Timer_s *ShortestTimer(void)
{
    Timer_s *t, *s_t;      /* ShortestTimer */
    unsigned int old_timer = MANY_TENS_OF_MSECS;

    for (s_t = 0, t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->timeout>0 && t->timeout<old_timer)
        {
            old_timer = t->timeout;
            s_t = t;
        }
    }
    return (s_t);
}

/* ======================================================================== */
void TimerTask(void * arg)
{
    while (mTimerStatus == kTKEL_RUNNING)
    {
        TKEL_WaitSemaphore(mTimerSema);
        TKEL_LockMutex(mTimerMutex);
        UpdateAllTimersBy(CurrentTime() - mTimerSetTimestamp);
        /* start timer for next shortest guy if one exists */
        StartTimer(ShortestTimer());
        TKEL_UnlockMutex(mTimerMutex);
    }
}

/* local variables */
static timer_t timerid;
static struct sigevent sev;
static struct itimerspec its;
struct sigaction sa;

/*
 * create timer and set handler
 */
static int create_timer(void)
{
    disable_interrupts();
    // establish handler for timer
    sa.sa_flags = SA_NOCLDSTOP;
    sa.sa_handler = clk_isr;
    sigemptyset(&sa.sa_mask);
    if (sigaction(TIMER_SIGNAL_TYPE, &sa, NULL) == -1)
    {
        return -1;
    }
    // create timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMER_SIGNAL_TYPE;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
    {
        enable_interrupts();
        return -1;
    }
	enable_interrupts();
    return 0;
}

/*
 * set interval timer
 */
static int setitimer(struct itimerval interval)
{
    // set interval variable with one from arguments
    its.it_interval.tv_sec = interval.it_interval.tv_sec;
    its.it_interval.tv_nsec = interval.it_interval.tv_usec * 1000;
    its.it_value.tv_sec = interval.it_value.tv_sec;
    its.it_value.tv_nsec = interval.it_value.tv_usec * 1000;

    disable_interrupts();

    // timer set time
    if (timer_settime(timerid, 0, &its, NULL) == -1)
    {
        enable_interrupts();
        return -1;
    }

    enable_interrupts();
    return 0;
}

/********************************************************/
/*          Functions Definitions (GLOBAL)        */
/********************************************************/
TKEL_err TKEL_CreateTimer(TKEL_timer_id *timerID)
{
    Timer_s *free_timer = 0;   /* pointer to unused timer entry */
    Timer_s *t = 0;

    mTBOX_FCT_ENTER("TKEL_CreateTimer");

    if (!timerID)
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (mTimerInitialized == 0)
    {
		mTimerInitialized = 1;
		TKEL_CreateMutex((TKEL_mutex_id *)&mTimerMutex);

        if (mTimerStatus == kTKEL_STOP)
        {
            mTimerStatus = kTKEL_RUNNING;

            /* TBD */
            /* disable constant interval timing */
            /*timerclear(&mTimerValue.it_interval);*/

            if ( -1 == create_timer())
            {
                mTBOX_RETURN(TKEL_NOT_DONE);
            }

            TKEL_LockMutex(mTimerMutex);

            for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++) {
                t->timerID = (TKEL_timer_id) t;
                t->timeout = 0;
                t->task_to_send_event = 0;
                t->event_to_send = 0x00;
                t->callback = NULL;
                t->inuse = 0;
            }
            TKEL_UnlockMutex(mTimerMutex);

            TKEL_CreateSemaphore(0,&mTimerSema);
            TKEL_CreateTask(kTKEL_TIMER_TASK_PRIO,0,0,TimerTask,0,&mTimerTask);

            // signal(TIMER_SIGNAL_TYPE, clk_isr);
        }
    }

    disable_interrupts();

    TKEL_LockMutex(mTimerMutex);

    for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->inuse == 0)
        {
            free_timer = t;
            break;
        }
    }
    if (!free_timer)
    {
        TKEL_UnlockMutex(mTimerMutex);
        enable_interrupts();
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Cannot create more timers\n"));
        mTBOX_RETURN(TKEL_NOT_DONE);
    }

    free_timer->task_to_send_event = 0;
    free_timer->event_to_send = 0x00;
    free_timer->callback = NULL;
    free_timer->inuse = 1;
    *timerID = free_timer->timerID;

    TKEL_UnlockMutex(mTimerMutex);

    enable_interrupts();
    mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_DeleteTimer(TKEL_timer_id timerID)
{
    Timer_s   *t;
    TKEL_err   error;

    mTBOX_FCT_ENTER("TKEL_DeleteTimer");

    TKEL_LockMutex(mTimerMutex);

    for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->timerID == timerID)
        {
            if ((t->inuse == 0) || (t->timeout > 0))
            {
                /* timer is not used or still running */
                error = TKEL_NOT_DONE;
            }
            else
            {
                t->inuse = 0;
                error = TKEL_NO_ERR;
            }
            TKEL_UnlockMutex(mTimerMutex);
            mTBOX_RETURN(error);
        }
    }

    TKEL_UnlockMutex(mTimerMutex);
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Timer to delete not found\n"));
    mTBOX_RETURN(TKEL_BAD_ARG);
}

/* ======================================================================== */
TKEL_err TKEL_StartTimer(TKEL_timer_id timerID,TKEL_tck ticks,TKEL_evts events)
{
    Timer_s *t;
    bool startitimer = FALSE;

    mTBOX_FCT_ENTER("TKEL_StartTimer");

    if (!timerID || ticks==0 || events==0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad arguments to start the timer\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    TKEL_LockMutex(mTimerMutex);
    for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->timerID == timerID && t->inuse == 1)
        {
            if (t->timeout > 0) /* Already started */
                break;

            if (!mNextTimer)
            {
                startitimer = TRUE;
            }
            else if ((ticks + ms2tick(CurrentTime())) < (mNextTimer->timeout + ms2tick(mTimerSetTimestamp)))
            {
                CancelItimer();
                startitimer = TRUE;
            }
            else
            {
                UpdateAllTimersBy(CurrentTime() - mTimerSetTimestamp);
                mTimerSetTimestamp = CurrentTime();
            }

            t->task_to_send_event = GetTaskFromList(pthread_self());
            t->event_to_send = events;
            t->callback = NULL;
            t->timeout = ticks;
            if (startitimer)
            {
                StartTimer(t);
            }
            TKEL_UnlockMutex(mTimerMutex);

            mTBOX_RETURN(TKEL_NO_ERR);
        }
    }

    TKEL_UnlockMutex(mTimerMutex);
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Timer to start not found or already started\n"));
    mTBOX_RETURN(TKEL_NOT_DONE);
}

/* ======================================================================== */
TKEL_err TKEL_StopTimer(TKEL_timer_id timerID)
{
    Timer_s   *t;
    TKEL_err   error;

    mTBOX_FCT_ENTER("TKEL_StopTimer");

    TKEL_LockMutex(mTimerMutex);

    for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->timerID == timerID && t->inuse == 1)
        {
            if (t->timeout == 0)
            {
                /* Already stopped */
                error = TKEL_NOT_DONE;
            }
            else
            {
                t->timeout = 0;
                t->task_to_send_event = 0;
                t->event_to_send = 0x00;
                t->callback = NULL;

                /* check if we were waiting on this one */
                if (t == mNextTimer)
                {
                    CancelItimer();
                    StartTimer(ShortestTimer());
                }
                error = TKEL_NO_ERR;
            }
            TKEL_UnlockMutex(mTimerMutex);
            mTBOX_RETURN(error);
        }
    }

    TKEL_UnlockMutex(mTimerMutex);
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Timer to stop not found or already stopped\n"));
    mTBOX_RETURN(TKEL_BAD_ARG);
}

/* ======================================================================== */
/* return time in tens of milliseconds */
/* ======================================================================== */
unsigned int CurrentTime(void)
{
    struct timespec ts;
    (void) clock_gettime(CLOCK_REALTIME, &ts);
    /* ignore overflow */
    return ts.tv_sec * MSECS_PER_SEC + ts.tv_nsec/NSECS_PER_MSEC;
}

/*=============================================================================
 *
 * TKEL_StartTimerWithCB
 *
 * Parameters:
 *      timerID      timer id
 *      ticks      timer interval in ticks
 *      callback   callback to call when timeout
 *
 * Description:
 *      Starts a timer. Callback will be call when timeout occurs
 *      The function does not restart an already started timer, if
 *      timer is running, function returns TKEL_NOT_DONE.
 *
 * Returns:
 *      TKEL_NO_ERR      no error
 *      TKEL_BAD_ARG   wrong argument value
 *      TKEL_NOT_DONE   command not done
 *
 *===========================================================================*/
TKEL_err TKEL_StartTimerWithCB(TKEL_timer_id timerID,TKEL_tck ticks,TKEL_timer_CB callback)
{
    Timer_s *t;
    bool startitimer = FALSE;

    mTBOX_FCT_ENTER("TKEL_StartTimerWithCB");

    if (!timerID || ticks==0 || !callback)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad arguments to start the timer\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    TKEL_LockMutex(mTimerMutex);
    for (t = mTimer; t < &mTimer[ITC_NB_TIMERS]; t++)
    {
        if (t->timerID == timerID && t->inuse == 1)
        {
            if (t->timeout > 0) /* Already started */
                break;

            if (!mNextTimer)
            {
                startitimer = TRUE;
            }
            else if ((ticks + ms2tick(CurrentTime())) < (mNextTimer->timeout + ms2tick(mTimerSetTimestamp)))
            {
                CancelItimer();
                startitimer = TRUE;
            }
            else
            {
                UpdateAllTimersBy(CurrentTime() - mTimerSetTimestamp);
                mTimerSetTimestamp = CurrentTime();
            }

            t->task_to_send_event = GetTaskFromList(pthread_self());
            t->event_to_send = 0x00;
            t->timeout = ticks;
            t->callback = callback;
            if (startitimer)
            {
                StartTimer(t);
            }
            TKEL_UnlockMutex(mTimerMutex);

            mTBOX_RETURN(TKEL_NO_ERR);
        }
    }

    TKEL_UnlockMutex(mTimerMutex);
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Timer already started\n"));
    mTBOX_RETURN(TKEL_NOT_DONE);
}

/*=============================================================================
 *
 * TKEL_SetTimerOwnerTask
 *
 * Parameters:
 *      timerID      timer id
 *      taskID      ID of the task to notify when the timer expired
 *
 * Description:
 *      set the task field of the timer identified by TimerID
 *      If the timer expired, the owner received the event
 *      This function must be called after TKEL_StartTimer
 *      else the TaskId will be replaced by the TKEL_StartTimer
 *      caller task Id
 *
 * Returns:
 *      TKEL_NO_ERR      no error
 *      TKEL_BAD_ARG   wrong argument value
 *
 *===========================================================================*/
TKEL_err TKEL_SetTimerOwnerTask(TKEL_timer_id timerID,TKEL_task_id taskID)
{
    mTBOX_FCT_ENTER("TKEL_SetTimerOwnerTask");

    /* check input parameters */
    if (timerID >= (TKEL_timer_id)(&mTimer[ITC_NB_TIMERS]))
    {
        mTBOX_RETURN(TKEL_BAD_ARG);
    }

    if (!taskID)
        TKEL_GetCurrentTask(&taskID);
    else if(CheckTaskInList(taskID)!=TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Task does not exist in list\n"));
        mTBOX_RETURN(TKEL_BAD_ARG);
    }
    /* lock the timer list */
    TKEL_LockMutex(mTimerMutex);

    ((Timer_s*)(timerID))->task_to_send_event = taskID;

    /* unlock the timer list */
    TKEL_UnlockMutex(mTimerMutex);

    mTBOX_RETURN(TKEL_NO_ERR);
}
