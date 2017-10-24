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
 * DESCRIPTION : Task Communication API of Thin Kernel Encapsulation Layer (T-KEL)
 *
 *****************************************************************************/


/********************************************************/
/*          Includes                      */
/********************************************************/
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
//#include <sys/resource.h>

#include "tkel.h"
#include "tkel_ecos_private.h"


/********************************************************/
/*          Defines                     */
/********************************************************/
mTBOX_SET_MODULE(eTKEL_COM);


/********************************************************/
/*          Typedefs                      */
/********************************************************/

/* QUEUE management */
typedef struct {
   void        *queue;       /* the queue */
   int         entry;        /* number of elements in the queue */
   int         head;         /* head of the queue */
   int         tail;         /* tail of the queue */
   int         count;        /* maximum number of messages in the queue */
   int         msgsize;
   TKEL_sema_id   sema;
   TKEL_task_id   task;
   TKEL_evts      event;
}TKEL_queue_t;


/********************************************************/
/*          Local Functions Declarations         */
/********************************************************/
static int AddFifo(int index, unsigned short len, unsigned char *msg);
static int GetFifo(int index, unsigned char *msg);


/********************************************************/
/*          Local File Variables              */
/********************************************************/
TKEL_task_id mTimerTask; /* Timers management task */
TKEL_sema_id mTimerSema; /* Timers management semaphore */

static pthread_mutex_t mQueueMutex;               /*mutex for queue */

static int mInitQueue = 0;             /* 0 if not initialized */
static int mNbQueue = 0;              /* Nb created queues */
static TKEL_queue_t mQueue[ITC_NB_QUEUES];   /* Queue management */


/********************************************************/
/*          Functions Definitions (LOCAL)         */
/********************************************************/

/*=============================================================================
 *
 * Function : AddFifo
 *
 * Parameters:
 *      FIFO index
 *      length of the message to save (bytes)
 *      message to save
 *
 * Description:
 *      Save a message in Fifo
 *
 * Returns:
 *      0 : OK
 *      -1 : error
 *
 *===========================================================================*/
static int AddFifo(int index, unsigned short len, unsigned char *msg)
{
	pthread_mutex_lock(&mQueueMutex);

	if (mQueue[index].entry >= mQueue[index].count)    /* full fifo : Error */
	{
		pthread_mutex_unlock(&mQueueMutex);
		mTBOX_TRACE((kTBOX_NIV_WARNING, "AddFifo : fifo is full\n"));
		return (-1);
	}

	if (TKEL_SignalSemaphore(mQueue[index].sema) != TKEL_NO_ERR)
	{
		pthread_mutex_unlock(&mQueueMutex);
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Semaphore error\n"));
		return (-1);
	}

	if (len > mQueue[index].msgsize)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "AddFifo : message to enqueue is too big and will be truncated\n"));
	}

	mQueue[index].entry++;   /* one more element   */

	/* element's copy in fifo */
	memcpy((unsigned char *) (mQueue[index].queue) + (mQueue[index].msgsize * mQueue[index].tail), msg, len);

	mQueue[index].tail++;   /* tail is updated   */
	if (mQueue[index].tail >= mQueue[index].count)
	{
		mQueue[index].tail = 0;
	}

	pthread_mutex_unlock(&mQueueMutex);

	return 0;   /* Success */
}

/*=============================================================================
 *
 * Function : GetFifo
 *
 * Parameters:
 *      FIFO index
 *      restored message
 *
 * Description:
 *      Restore a message from Fifo
 *
 * Returns:
 *      0 : OK
 *      -1 : error
 *
 *===========================================================================*/
static int GetFifo(int index, unsigned char *msg)
{
	pthread_mutex_lock(&mQueueMutex);

	if (mQueue[index].entry <= 0)   /* empty : Error */
	{
		pthread_mutex_unlock(&mQueueMutex);
		mTBOX_TRACE((kTBOX_NIV_WARNING, "GetFifo : fifo is empty\n"));
		return (-1);
	}

	mQueue[index].entry--;

	/* element's copy out fifo */
	memcpy(msg, (unsigned char *) (mQueue[index].queue) + (mQueue[index].msgsize * mQueue[index].head), mQueue[index].msgsize);

	mQueue[index].head++;   /* head is updated   */
	if (mQueue[index].head >= mQueue[index].count)
	{
		mQueue[index].head = 0;
	}

	pthread_mutex_unlock(&mQueueMutex);

	return 0;   /* Success */
}


/********************************************************/
/*          Functions Definitions (GLOBAL)        */
/********************************************************/
TKEL_err TKEL_CreateQueue(TKEL_qdepth maxNumber,TKEL_qwidth msgSize,TKEL_queue_id *qid)
{
	int index;

	mTBOX_FCT_ENTER("TKEL_CreateQueue");

	if (!maxNumber || !msgSize || !qid)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect arguments\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	/* first time initialization */
	if (mInitQueue == 0)
	{
		mInitQueue = 1;

		pthread_mutex_init(&mQueueMutex, NULL);
		pthread_mutex_lock(&mQueueMutex);
		mNbQueue = 0;
		for (index = 0; index < ITC_NB_QUEUES; index++)
		{
			mQueue[index].queue = NULL;
			mQueue[index].entry = 0;
			mQueue[index].head = 0;
			mQueue[index].tail = 0;
			mQueue[index].count = 0;
			mQueue[index].msgsize = 0;
			mQueue[index].sema = NULL;
			mQueue[index].task = NULL;
			mQueue[index].event = 0x00;
		}
	}
	else
	{
		pthread_mutex_lock(&mQueueMutex);
	}

	for (index = 0; index < ITC_NB_QUEUES; index++)
	{

		if (mQueue[index].queue == NULL)
		{
			/* found a free slot */
			/* Create a FIFO */
			if (TKEL_Malloc(maxNumber * msgSize, &mQueue[index].queue) != TKEL_NO_ERR)
			{
				pthread_mutex_unlock(&mQueueMutex);
				printf("[%s %d]Tkel malloc failed:%d\n",__FUNCTION__,__LINE__,(maxNumber * msgSize));
				mTBOX_RETURN(TKEL_NOT_DONE);
			}

			/* create a semaphore */
			if (mQueue[index].queue)
			{
				if (TKEL_CreateSemaphore(0, &mQueue[index].sema) != TKEL_NO_ERR)
				{
					TKEL_Free(mQueue[index].queue);
					mQueue[index].queue = NULL;
				}
			}

			if (mQueue[index].queue)
			{
				mQueue[index].count = maxNumber;
				mQueue[index].head = 0;
				mQueue[index].tail = 0;
				mQueue[index].entry = 0;
				mQueue[index].msgsize = msgSize;
				mNbQueue++;
				*qid = index + 1;   /* queue identifier = index of mQueue[] + 1 (never 0) */
			}
			else
			{
				pthread_mutex_unlock(&mQueueMutex);
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error allocating queue\n"));
				mTBOX_RETURN(TKEL_NOT_DONE);
			}
			break;
		}
	}/* end for loop */

	pthread_mutex_unlock(&mQueueMutex);

	if (index >= ITC_NB_QUEUES)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Queue buffer full : cannot create another queue\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_DeleteQueue(TKEL_queue_id qid)
{
	TKEL_int32 ret = TKEL_NO_ERR;
	int index = qid - 1;

	mTBOX_FCT_ENTER("TKEL_DeleteQueue");

	if (index < 0 || index > ITC_NB_QUEUES)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect argument\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	pthread_mutex_lock(&mQueueMutex);

	if (mQueue[index].queue)
	{
		if (TKEL_DeleteSemaphore(mQueue[index].sema) != TKEL_NO_ERR)
		{
			mTBOX_TRACE((kTBOX_NIV_WARNING, "Cannot delete queue : the semaphore associated is waited by someone\n"));
			ret = TKEL_NOT_DONE;   /* the mutex associated to this queue is still kept by an Enqueue/Dequeue function */
		}

		else
		{
			TKEL_Free(mQueue[index].queue);   /* Delete the FIFO */
			mQueue[index].queue = 0;
			mQueue[index].head = 0;
			mQueue[index].tail = 0;
			mQueue[index].entry = 0;
			mQueue[index].count = 0;
			mQueue[index].msgsize = 0;
			mQueue[index].sema = NULL;
			mQueue[index].task = NULL;
			mQueue[index].event = 0x00;

			mNbQueue--;
			ret = TKEL_NO_ERR;
		}
	}

	else
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "This queue is already deleted\n"));
		ret = TKEL_NOT_DONE;
	}

	pthread_mutex_unlock(&mQueueMutex);
	mTBOX_RETURN(ret);
}

/* ======================================================================== */
TKEL_err TKEL_Enqueue(TKEL_queue_id qid,void *msg)
{
	int index = qid - 1;

	mTBOX_FCT_ENTER("TKEL_Enqueue");

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	if ((index < 0) || (index >= ITC_NB_QUEUES) || (!msg))
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect arguments\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	else if (!mQueue[index].queue)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Queue does not exist\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	if (AddFifo(index, mQueue[index].msgsize,(unsigned char *) msg))
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Fifo is full\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	if (mQueue[index].event != 0x00)
	{
		TKEL_SendEvent(mQueue[index].task, mQueue[index].event);
	}

	mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_Dequeue(TKEL_queue_id qid,void *msg_buf)
{
	int      index = qid - 1;

	mTBOX_FCT_ENTER("TKEL_Dequeue");

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	if ((index < 0) || (index >= ITC_NB_QUEUES))
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect argument\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	else if (!mQueue[index].queue)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Queue does not exist\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	if (TKEL_WaitSemaphore(mQueue[index].sema) == TKEL_NO_ERR)
	{
		if (!GetFifo(index, (unsigned char *) msg_buf))
		{
			mTBOX_RETURN(TKEL_NO_ERR);
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error: FIFO is inconsistent with semaphore\n"));
			mTBOX_RETURN(TKEL_NOT_DONE);
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Semaphore error\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}
}

/* ======================================================================== */
TKEL_err TKEL_GetMsgNumber(TKEL_queue_id qid,TKEL_int16 *msgNumber)
{
	int index = qid - 1;

	mTBOX_FCT_ENTER("TKEL_GetMsgNumber");

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	if ((index < 0) || (index >= ITC_NB_QUEUES) || !msgNumber)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect arguments\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	else if (!mQueue[index].queue)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Queue does not exist\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	pthread_mutex_lock(&mQueueMutex);
	*msgNumber = mQueue[index].entry;
	pthread_mutex_unlock(&mQueueMutex);

	mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_NotifyQueueEvent(TKEL_queue_id queueID,TKEL_task_id taskID,TKEL_evts events)
{
	int index = queueID - 1;

	mTBOX_FCT_ENTER("TKEL_NotifyQueueEvent");

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	if (!taskID)
	{
		TKEL_GetCurrentTask(&taskID);
	}
	else if(CheckTaskInList(taskID)!=TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Task does not exist in list\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	if ((index < 0) || (index >= ITC_NB_QUEUES) || !taskID || !events)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect arguments\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	else if ((!GetTaskFromList(taskID->task)) || (!mQueue[index].queue))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Either queue or task does not exist\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	pthread_mutex_lock(&mQueueMutex);
	mQueue[index].task = taskID;
	mQueue[index].event = events;
	pthread_mutex_unlock(&mQueueMutex);

	mTBOX_RETURN(TKEL_NO_ERR);
}

/* ======================================================================== */
TKEL_err TKEL_CreateQueueTimeout(TKEL_qdepth maxNumber,TKEL_qwidth msgSize,TKEL_queue_id *queueID)
{
	return TKEL_CreateQueue(maxNumber, msgSize, queueID);
}

/* ======================================================================== */
TKEL_err TKEL_EnqueueTimeout(TKEL_queue_id queueID,void *message,TKEL_tck ticks)
{
	int         index = queueID - 1;
	unsigned int   timeout;
	unsigned int   delay = 50;          /* 50 ms */

	mTBOX_FCT_ENTER("TKEL_EnqueueTimeout");

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	if ((index < 0) || (index >= ITC_NB_QUEUES) || (ticks < 0) || !message)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect arguments\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	if (!mQueue[index].queue)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Queue does not exist\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	/* compute the timeout */
	switch(ticks)
	{
		case kTKEL_TIMEOUT_IMMEDIATE:
			timeout = 0;
			break;

		case kTKEL_TIMEOUT_INFINITE:
			timeout = (unsigned int) -1;
			break;

		default:
			timeout = tick2ms(ticks);
			break;
	}

	do
	{
		/* try to enquee the message */
		if (!AddFifo(index, mQueue[index].msgsize, (unsigned char *) message))
		{
			if (mQueue[index].event != 0x00)
			{
				TKEL_SendEvent(mQueue[index].task, mQueue[index].event);
			}

			mTBOX_RETURN(TKEL_NO_ERR);
		}

		if (timeout > 0)
		{
			/* timeout has not occured yet */

			if (delay > timeout)
			{
				/* this is the last time we wait */
				delay = timeout;
			}

			TKEL_DelayTask(ms2tick(delay));
			timeout -= delay;
		}
		else
		{
			/* timeout has occured */

			mTBOX_TRACE((kTBOX_NIV_WARNING, "Enqueue timeout has been reached\n"));
			mTBOX_RETURN(TKEL_TIMEOUT);
		}
	}while (1);
}

/* ======================================================================== */
TKEL_err TKEL_DequeueTimeout(TKEL_queue_id queueID,void *message,TKEL_tck ticks)
{
	TKEL_err   ret;
	int      index = queueID - 1;

	mTBOX_FCT_ENTER("TKEL_DequeueTimeout");

	if (!mNbQueue)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "There is no queue created\n"));
		mTBOX_RETURN(TKEL_NOT_DONE);
	}

	/* check input parameters */
	if ((index < 0) || (index >= ITC_NB_QUEUES) || (ticks < 0))
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Incorrect arguments\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	/* is queue created ? */
	if (!mQueue[index].queue)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Queue does not exist\n"));
		mTBOX_RETURN(TKEL_BAD_ARG);
	}

	if (kTKEL_TIMEOUT_INFINITE == ticks)
	{
		mTBOX_RETURN(TKEL_Dequeue (queueID, message));
	}

	ret = TKEL_WaitSemaphoreTimeout(mQueue[index].sema, ticks);

	switch (ret)
	{
		case TKEL_NO_ERR:
			if (!GetFifo(index, (unsigned char *) message))
			{
				mTBOX_RETURN(TKEL_NO_ERR);
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error: FIFO is inconsistent with semaphore\n"));
				mTBOX_RETURN(TKEL_NOT_DONE);
			}
			break;

		case TKEL_TIMEOUT:
			mTBOX_TRACE((kTBOX_NIV_1, "Dequeue timeout has been reached\n"));
			mTBOX_RETURN(TKEL_TIMEOUT);
			break;

		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Semaphore error\n"));
			mTBOX_RETURN(TKEL_NOT_DONE);
			break;
	}
}
