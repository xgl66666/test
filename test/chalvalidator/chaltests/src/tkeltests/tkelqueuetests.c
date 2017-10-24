/** \file tkelqueuetests.c
 *  \brief Test queues API for TKEL layer
 *  \author Ronan MENEU
 *  \date 20/03/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "tkel.h"
#include "TestManager.h"

typedef void (*tFunctionTask) (void*);

const int gQUEUE_SIZE = 10;
const int gEVENT_ENQUEUE = 0x20;

TKEL_queue_id gQueueId;
TKEL_sema_id gSemQueueId;
TKEL_mutex_id gMutexId;

int gSlowProducer = 0;
int gConsumerUsesEvt = 0;
int gSlowConsumer = 0;

void TestCase_ExecuteTestEnqueueDequeue();
void TestCase_ExecuteTestEnqueueDequeueBetweenTask();
void TestCase_ExecuteTestQueueDeletionDuringDequeue();
void TestCase_ExecuteTestEnqueueFull();
void TestCase_ExecuteTestEnqueueDequeueTimeout();
void TestCase_ExecuteTestEnqueueTimeout();
void TestCase_ExecuteTestDequeueTimeout();
void TestCase_ExecuteTestEnqueueEvtNotification();
void TestCase_ExecuteTestEnqueueEvtNotificationSlowConsumer();

static tTestNode gTestEnqueueDequeue = {
    "TestQueue",
    "test the basic enqueue/dequeue operations",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueDequeue,
        NULL
};

static tTestNode gTestEnqueueDequeueBetweenTask = {
    "TestQueueBetweenTask",
    "test enqueue/dequeue between two tasks",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueDequeueBetweenTask,
        NULL
};

static tTestNode gTestQueueDeletionDuringDequeue = {
    "TestQueueDeletionDuringDequeue",
    "test a queue deletion while a tkeldequeue is waiting",
    0, /* not interactive */
    TestCase_ExecuteTestQueueDeletionDuringDequeue,
        NULL
};

static tTestNode gTestEnqueueDequeueTimeout = {
    "TestEnqueueDequeueTimeout",
    "test enqueue/dequeue with timeout",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueDequeueTimeout,
        NULL
};

static tTestNode gTestEnqueueTimeout = {
    "TestEnqueueTimeout",
    "test a enqueue timeout when queue is full",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueTimeout,
        NULL
};

static tTestNode gTestDequeueTimeout = {
    "TestDequeueTimeout",
    "test a dequeue timeout returning timeout",
    0, /* not interactive */
    TestCase_ExecuteTestDequeueTimeout,
        NULL
};

static tTestNode gTestEnqueueEvtNotification = {
    "TestEnqueueEvtNotification",
    "test the evt notification sent on a enqueue",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueEvtNotification,
        NULL
};

static tTestNode gTestEnqueueEvtNotificationSlowConsumer = {
    "TestEnqueueEvtNotificationSlowConsumer",
    "test the evt notification sent on a enqueue, with a slow consumer",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueEvtNotificationSlowConsumer,
        NULL
};


static tTestNode gTestQueueFull = {
    "TestQueueFull",
    "test the queue sending up to the limit",
    0, /* not interactive */
    TestCase_ExecuteTestEnqueueFull,
        NULL
};

tTestNode* gQueueTestNodeList[] = {
    &gTestEnqueueDequeue,
    &gTestEnqueueDequeueBetweenTask,
    &gTestQueueDeletionDuringDequeue,
    &gTestQueueFull,
    &gTestEnqueueDequeueTimeout,
    &gTestEnqueueTimeout,
    &gTestDequeueTimeout,
    &gTestEnqueueEvtNotification,
    &gTestEnqueueEvtNotificationSlowConsumer,
    NULL
};

tTestNode gTKELQueueTestSuite = {
    "TestQueue",
    "test the queue API",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gQueueTestNodeList
};

/*! \fn void TestCase_ExecuteTestEnqueueDequeue( void )
 *  \brief This test performs simple enqueue/dequeue
 *  \see TestMutex() for algorithm
 */
void TestCase_ExecuteTestEnqueueDequeue()
{
    int i;
    int aMsg;
    TKEL_int16 aMsgNb;
    TKEL_queue_id aQueueId;
    TestManager_AssertEqual( TKEL_CreateQueue( gQUEUE_SIZE, 4, &aQueueId), TKEL_NO_ERR, "create queue" );

    /* enqueue */
    for(i=0; i<gQUEUE_SIZE; i++ )
    {
        aMsg = i;
        TestManager_AssertEqual( TKEL_Enqueue(aQueueId,(void*)&aMsg), TKEL_NO_ERR, "enqueue" );
        TestManager_AssertEqual( TKEL_GetMsgNumber(aQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
        TestManager_AssertEqual( i, aMsgNb-1, "number of msg correct" );
    }

    TestManager_AssertEqual( TKEL_Enqueue(aQueueId,(void*)i), TKEL_NOT_DONE, "enqueue not done: queue full" );

    /* dequeue */
  for( i=gQUEUE_SIZE-1;i>=0; i-- )
    {
    TestManager_AssertEqual( TKEL_Dequeue(aQueueId,&aMsg), TKEL_NO_ERR, "dequeue" );
    TestManager_AssertEqual( TKEL_GetMsgNumber(aQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
    TestManager_AssertEqual( i, aMsgNb, "number of msg correct" );
    TestManager_AssertEqual( gQUEUE_SIZE-i-1, aMsg, "msg content correct" );
  }

    /*TestManager_AssertEqual( TKEL_Dequeue(gQueueId,&aMsg), TKEL_NOT_DONE, "enqueue not done: queue empty" );*/

    TestManager_AssertEqual( TKEL_DeleteQueue(aQueueId), TKEL_NO_ERR, "delete queue" );
}

/*! \fn void TaskProducer( void* arg )
 *  \brief This task enqueues msg.
 */
void TaskProducer(void * arg)
{
    int i;
    int aMsg;
    /* enqueueing */
    for(i=0; i<gQUEUE_SIZE; i++ )
    {
        aMsg = i;
        if( gConsumerUsesEvt )
        {
            TestManager_AssertEqual( TKEL_LockMutex(gMutexId), TKEL_NO_ERR, "lock mutex" );
        }

        TestManager_AssertEqual( TKEL_Enqueue(gQueueId,(void*)&aMsg), TKEL_NO_ERR, "enqueue" );

        if( gConsumerUsesEvt )
        {
            TestManager_AssertEqual( TKEL_UnlockMutex(gMutexId), TKEL_NO_ERR, "unlock mutex" );
        }

        if( gSlowProducer )
        {
            TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1000)), TKEL_NO_ERR, "delay task" );
        }
    }
}

/*! \fn void TaskConsumerWithDequeue( void* arg )
 *  \brief This task dequeues msg.
 */
void TaskConsumerWithDequeue(void * arg)
{
    int i=0;
    int aMsg;
    TKEL_int16 aMsgNb;
    do
    {
        TestManager_AssertEqual( TKEL_Dequeue(gQueueId,&aMsg), TKEL_NO_ERR, "dequeue" );
        TestManager_AssertEqual( i, aMsg, "msg content correct" );
        i++;
    }
    while(i < gQUEUE_SIZE);

    /* end of consuming: check all messages have been consumed, and post to semaphore */
    TestManager_AssertEqual( TKEL_GetMsgNumber(gQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
    TestManager_AssertEqual( aMsgNb, 0, "all messages consumed" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemQueueId), TKEL_NO_ERR, "post on semaphore" );
}

/*! \fn void TaskConsumerWithDequeueTimeout( void* arg )
 *  \brief This task dequeues msg.
 */
void TaskConsumerWithDequeueTimeout(void * arg)
{
    int i=0;
    int aMsg;
    TKEL_int16 aMsgNb;
    do
    {
        TKEL_err aErr = TKEL_DequeueTimeout(gQueueId,&aMsg,ms2tick(300));
        if( aErr == TKEL_TIMEOUT )
        {
            TestManager_LogMessage( "timeout occurs" );
        }
        else
        {
            TestManager_AssertEqual( aErr, TKEL_NO_ERR, "no error on dequeue" );
            TestManager_AssertEqual( i, aMsg, "msg content correct" );
            i++;
        }
    }
    while(i < gQUEUE_SIZE);

    /* end of consuming: check all messages have been consumed, and post to semaphore */
    TestManager_AssertEqual( TKEL_GetMsgNumber(gQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
    TestManager_AssertEqual( aMsgNb, 0, "all messages consumed" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemQueueId), TKEL_NO_ERR, "post on semaphore" );
}

/*! \fn void TaskConsumerWithEvtReception( void* arg )
 *  \brief This task dequeues msg on evt reception.
 */
void TaskConsumerWithEvtReception(void * arg)
{
    int i=0;
    int aMsg;
    TKEL_int16 aMsgNb;
    do
    {
        TKEL_evts aEvent;
        if( gSlowConsumer )
        {
            TestManager_AssertEqual( TKEL_DelayTask(ms2tick(100)), TKEL_NO_ERR, "delay 1/10s" );
        }
        TestManager_AssertEqual( TKEL_ReceiveEvent( 0, &aEvent), TKEL_NO_ERR, "evt received" );
        TestManager_AssertEqual( aEvent, gEVENT_ENQUEUE, "evt is the one expected" );
        TestManager_AssertEqual( TKEL_LockMutex(gMutexId), TKEL_NO_ERR, "lock mutex" );
        TestManager_AssertEqual( TKEL_GetMsgNumber(gQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
        while( aMsgNb )
        {
            TestManager_AssertEqual( TKEL_Dequeue(gQueueId,&aMsg), TKEL_NO_ERR, "dequeue" );
            TestManager_AssertEqual( i, aMsg, "msg content correct" );
            i++;
            TestManager_AssertEqual( TKEL_GetMsgNumber(gQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
        }
        TestManager_AssertEqual( TKEL_UnlockMutex(gMutexId), TKEL_NO_ERR, "unlock mutex" );
    }
    while(i < gQUEUE_SIZE);

    /* end of consuming: check all messages have been consumed, and post to semaphore */
    TestManager_AssertEqual( TKEL_GetMsgNumber(gQueueId,&aMsgNb), TKEL_NO_ERR, "get msg number" );
    TestManager_AssertEqual( aMsgNb, 0, "all messages consumed" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemQueueId), TKEL_NO_ERR, "post on semaphore" );
}

/*! \fn void TaskDequeueAndSignal( void* arg )
 *  \brief This task dequeues and signals.
 */
void TaskDequeueAndSignal(void * arg)
{
    int aMsg;
    TestManager_AssertGreater( TKEL_DequeueTimeout(gQueueId,&aMsg,ms2tick(4000)), TKEL_NO_ERR, "dequeue" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemQueueId), TKEL_NO_ERR, "post on semaphore" );
}

/*! \fn void TestProducerConsumer( void )
 *  \brief This test performs enqueue/dequeue between two tasks
 */
void TestProducerConsumer(tFunctionTask aTaskProducer, tFunctionTask aTaskConsumer)
{
    TKEL_task_id aTaskProdId, aTaskConsId;
    TestManager_AssertEqual( TKEL_CreateQueue( gQUEUE_SIZE, 4, &gQueueId), TKEL_NO_ERR, "create queue" );
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemQueueId), TKEL_NO_ERR, "create semaphore" );
    if( gConsumerUsesEvt )
    {
        TestManager_AssertEqual( TKEL_CreateMutex(&gMutexId), TKEL_NO_ERR, "create mutex" );
    }

  TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,aTaskConsumer,(void*) 0,&aTaskConsId), TKEL_NO_ERR, "create task consumer" );

    if( gConsumerUsesEvt )
    {
        TestManager_AssertEqual(
            TKEL_NotifyQueueEvent(gQueueId,aTaskConsId,gEVENT_ENQUEUE),  TKEL_NO_ERR, "notify queue evt to consumer" );
    }

  TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,aTaskProducer,(void*) 0,&aTaskProdId), TKEL_NO_ERR, "create task producer" );

	TestManager_AssertEqual(TKEL_DeleteTask(aTaskProdId), TKEL_NO_ERR, "delete task producer" );
	TestManager_AssertEqual(TKEL_DeleteTask(aTaskConsId), TKEL_NO_ERR, "delete task consumer" );

    if( gConsumerUsesEvt )
    {
        TestManager_AssertEqual( TKEL_DeleteMutex(gMutexId), TKEL_NO_ERR, "delete mutex" );
    }

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemQueueId), TKEL_NO_ERR, "wait for semaphore" );

  TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemQueueId), TKEL_NO_ERR, "delete semaphore" );
	
    TestManager_AssertEqual( TKEL_DeleteQueue(gQueueId), TKEL_NO_ERR, "delete queue" );


}

/*! \fn void TestCase_ExecuteTestEnqueueDequeueBetweenTask( void )
 *  \brief This test performs simple enqueue/dequeue between two task
 */
void TestCase_ExecuteTestEnqueueDequeueBetweenTask()
{
    gSlowProducer = 0;
    gConsumerUsesEvt = 0;
    gSlowConsumer = 0;
    TestProducerConsumer(TaskProducer, TaskConsumerWithDequeue);
}

/*! \fn void TestCase_ExecuteTestQueueDeletionDuringDequeue( void )
 *  \brief This test checks behaviour of queue deletion during a dequeue
 *  \bug TKEL_DeleteQueue causes a segfault if there is a concurrent dequeue.
 */
void TestCase_ExecuteTestQueueDeletionDuringDequeue()
{

	/* TestManager_AssertTrue( 0, "BUG (?) : queue deletion during dequeue causes segfault" ); */
#if 0
    TKEL_task_id aTask;

    TestManager_AssertEqual( TKEL_CreateQueue( gQUEUE_SIZE, 4, &gQueueId), TKEL_NO_ERR, "create queue" );
  	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemQueueId), TKEL_NO_ERR, "create semaphore" );

  TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,TaskDequeueAndSignal,(void*) 0,&aTask), TKEL_NO_ERR, "create task" );

    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(2000)), TKEL_NO_ERR, "task delay" );
    TestManager_AssertEqual( TKEL_DeleteQueue(gQueueId), TKEL_NOT_DONE, "delete queue" );

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemQueueId), TKEL_NO_ERR, "wait for semaphore" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemQueueId), TKEL_NO_ERR, "delete semaphore" );
#endif
}

/*! \fn void TestCase_ExecuteTestEnqueueFull( void )
 *  \brief This test enqueue up to full
 */
void TestCase_ExecuteTestEnqueueFull()
{
    int i;
    int aMsg;
    TestManager_AssertEqual( TKEL_CreateQueue( gQUEUE_SIZE, 4, &gQueueId), TKEL_NO_ERR, "create queue" );

    /* enqueue */
    for(i=0; i<gQUEUE_SIZE; i++ )
    {
        aMsg = i;
        TestManager_AssertEqual( TKEL_Enqueue(gQueueId,(void*)&aMsg), TKEL_NO_ERR, "enqueue" );
    }
    TestManager_AssertEqual( TKEL_Enqueue(gQueueId,(void*)i), TKEL_NOT_DONE, "enqueue not done: queue full" );

    TestManager_AssertEqual( TKEL_DeleteQueue(gQueueId), TKEL_NO_ERR, "delete queue" );
}

/*! \fn void TestCase_ExecuteTestEnqueueFull( void )
 *  \brief This test simulates a producer/consumer queue,
 *  with a slow producer, and many timeouts on consumer
 */
void TestCase_ExecuteTestEnqueueDequeueTimeout()
{
    gSlowProducer = 1;
    gConsumerUsesEvt = 0;
    gSlowConsumer = 0;
    TestProducerConsumer(TaskProducer, TaskConsumerWithDequeueTimeout);
}

/*! \fn void TestCase_ExecuteTestEnqueueTimeout( void )
 *  \brief This test enqueus up to limit and check timeout occurs
 */
void TestCase_ExecuteTestEnqueueTimeout()
{
    int i;
    int aMsg;
    const int gQUEUE_SIZE = 2;
    TKEL_queue_id gQueueId;
    TestManager_AssertEqual( TKEL_CreateQueue( gQUEUE_SIZE, 4, &gQueueId), TKEL_NO_ERR, "create queue" );

    /* enqueue */
    for(i=0; i<gQUEUE_SIZE; i++ )
    {
        aMsg = i;
        TestManager_AssertEqual( TKEL_Enqueue(gQueueId,(void*)&aMsg), TKEL_NO_ERR, "enqueue" );
    }

    TestManager_AssertEqual( TKEL_Enqueue(gQueueId,(void*)i), TKEL_NOT_DONE, "enqueue not done: queue full" );

    TestManager_AssertEqual(
        TKEL_EnqueueTimeout(gQueueId,(void*)i,ms2tick(1000)), TKEL_TIMEOUT, "enqueue timeout: queue full" );

    TestManager_AssertEqual( TKEL_DeleteQueue(gQueueId), TKEL_NO_ERR, "delete queue" );
}

/*! \fn void TestCase_ExecuteTestDequeueTimeout( void )
 *  \brief This test check dequeues timout occurs on a empty queue.
 */
void TestCase_ExecuteTestDequeueTimeout()
{
    int aMsg;
    TestManager_AssertEqual( TKEL_CreateQueue( gQUEUE_SIZE, 4, &gQueueId), TKEL_NO_ERR, "create queue" );
    TestManager_AssertEqual( TKEL_DequeueTimeout(gQueueId, &aMsg, ms2tick(500)), TKEL_TIMEOUT, "dequeue timeout : empty queue" );
    TestManager_AssertEqual( TKEL_DeleteQueue(gQueueId), TKEL_NO_ERR, "delete queue" );
}

/*! \fn void TestCase_ExecuteTestEnqueueEvtNotification( void )
 *  \brief This test performs a consumer/producer algorithm,
 *  where consumer is awaken by the event.
 */
void TestCase_ExecuteTestEnqueueEvtNotification()
{
    gSlowProducer = 0;
    gConsumerUsesEvt = 1;
    gSlowConsumer = 0;
    TestProducerConsumer(TaskProducer, TaskConsumerWithEvtReception);
}

/*! \fn void TestCase_ExecuteTestEnqueueEvtNotificationSlowConsumer( void )
 *  \brief This test performs a consumer/producer algorithm,
 *  where consumer is awaken by the event, with many evts for one reception
 */
void TestCase_ExecuteTestEnqueueEvtNotificationSlowConsumer()
{
    gSlowProducer = 0;
    gConsumerUsesEvt = 1;
    gSlowConsumer = 1;
    TestProducerConsumer(TaskProducer, TaskConsumerWithEvtReception);
}
