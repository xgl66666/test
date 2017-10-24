/** \file tkeleventtests.c
 *  \brief Test events API for TKEL layer
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
#include "tbox.h"
#include "TestManager.h"

TKEL_sema_id gSemEventId;
TKEL_sema_id gSemReceiverId;
TKEL_task_id gTaskEventReceiver;
TKEL_task_id gTaskEventSender;

static int gLoop = 1;
static int gNumberOfLoop = 20;
static int gSameEvtToSend = 1;

static int gSlowSender = 0;
static int gSlowReceiver = 1;

#define EVENT10 0x10

void TestCase_ExecuteTestEventReception(void);
void TestCase_ExecuteTestEventReceptionSlowSender(void);
void TestCase_ExecuteTestEventReceptionSlowReceiver(void);
void TestCase_ExecuteTestEventReceptionOnSameEvent(void);
void TestCase_ExecuteTestEventReceptionOnDifferentEvent(void);
void TestCase_ExecuteTestEventReceptionAfterSending(void);
void TestCase_ExecuteTestResetEvent(void);
void TestCase_ExecuteTestTimeoutTimeMeasure(void);
void TestCase_ExecuteTestSendEventDuringStartedReceptionWithTimeout(void);

void TestSenderReceiver(void);

static tTestNode gTestEventReception = {
    "TestEvtReception",
    "test the event reception",
    0, /* not interactive */
    TestCase_ExecuteTestEventReception,
        NULL
};

static tTestNode gTestEventReceptionSlowSender = {
    "TestEvtReceptionSlowSender",
    "test the event reception with slow sender",
    0, /* not interactive */
    TestCase_ExecuteTestEventReceptionSlowSender,
        NULL
};

static tTestNode gTestEventReceptionSlowReceiver = {
    "TestEvtReceptionSlowReceiver",
    "test the event reception with slow receiver",
    0, /* not interactive */
    TestCase_ExecuteTestEventReceptionSlowReceiver,
        NULL
};

static tTestNode gTestEventReceptionSameEvt = {
    "TestEvtReceptionSameEvt",
    "test the event reception of the same evt sent many times",
    0, /* not interactive */
    TestCase_ExecuteTestEventReceptionOnSameEvent,
        NULL
};

static tTestNode gTestEventReceptionDifferentEvt = {
    "TestEvtReceptionDifferentEvt",
    "test the event reception of some different evt",
    0, /* not interactive */
    TestCase_ExecuteTestEventReceptionOnDifferentEvent,
        NULL
};

static tTestNode gTestEventReceptionAfterSending = {
    "TestEvtReceptionAfterSending",
    "test the event reception when read comes after sending",
    0, /* not interactive */
    TestCase_ExecuteTestEventReceptionAfterSending,
        NULL
};

static tTestNode gTestResetEvent = {
    "TestResetEvent",
    "test the reset event",
    0, /* not interactive */
    TestCase_ExecuteTestResetEvent,
        NULL
};

static tTestNode gTestEventTimeoutTimeMeasure = {
    "TestEvtTimeoutTimeMeasure",
    "test the event timeout measure",
    0, /* not interactive */
    TestCase_ExecuteTestTimeoutTimeMeasure,
        NULL
};
static tTestNode gTestEventDuringReception = {
    "TestEvtDuringReception",
    "test that a event sent has immediate effect on the reception with timeout",
    0, /* not interactive */
    TestCase_ExecuteTestSendEventDuringStartedReceptionWithTimeout,
        NULL
};

tTestNode* gEventTestNodeList[] = {
    &gTestEventReception,
    &gTestEventReceptionSlowSender,
    &gTestEventReceptionSlowReceiver,
    &gTestEventReceptionSameEvt,
    &gTestEventReceptionDifferentEvt,
    &gTestEventReceptionAfterSending,
    &gTestResetEvent,
    &gTestEventTimeoutTimeMeasure,
    &gTestEventDuringReception,
    NULL
};

tTestNode gTKELEventTestSuite = {
    "TestEvent",
    "test the event API",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gEventTestNodeList
};

/*! \fn void TestCase_ExecutetestEventReception( void )
 *  \brief This test performs a simple sender/receiver simulation
 */
void TestCase_ExecuteTestEventReception()
{
    gSlowSender = 0;
    gSlowReceiver = 0;
    TestSenderReceiver();
}

/*! \fn void TestCase_ExecuteTestEventReceptionSlowSender( void )
 *  \brief This test performs a simple sender/receiver simulation
 *  with a slow sender.
 */
void TestCase_ExecuteTestEventReceptionSlowSender()
{
    gSlowSender = 1;
    gSlowReceiver = 0;
    TestSenderReceiver();
}


/*! \fn void TestCase_ExecuteTestEventReceptionSlowSender( void )
 *  \brief This test performs a simple sender/receiver simulation
 *  with a slow receiver.
 */
void TestCase_ExecuteTestEventReceptionSlowReceiver()
{
    gSlowSender = 0;
    gSlowReceiver = 1;
    TestSenderReceiver();
}

/*! \fn void TaskSendManyTimesAndReceive( void* arg )
 *  \brief This task sends many times the same event and receives
 */
void TaskSendManyTimesAndReceive( void* arg )
{
    TKEL_task_id aTaskId;
    TKEL_evts aEvent;
    int i;
    TestManager_AssertEqual( TKEL_GetCurrentTask( &aTaskId ), TKEL_NO_ERR, "get current task" );
    
    for( i=1; i<32; i++ )
    {
        TKEL_evts aSentEvent;
        
        if( gSameEvtToSend )
        {
            TestManager_AssertEqual( TKEL_SendEvent( aTaskId, EVENT10 ), TKEL_NO_ERR, "send task" );
        }
        else
        {
            aSentEvent = i;
            TestManager_AssertEqual( TKEL_SendEvent( aTaskId, aSentEvent ), TKEL_NO_ERR, "send task" );
        }
    }
    
    TestManager_AssertEqual( TKEL_ReceiveEvent( 0, &aEvent ), TKEL_NO_ERR, "receive event" );
    if( gSameEvtToSend )
    {
        TestManager_AssertEqual( aEvent, EVENT10, "expected event" );
    }
    else
    {
        /* expected event is an addition of the 32 first numbers, in terms of bit */
        TestManager_AssertEqual( aEvent, 0x1f, "expected event addition of the sent events" );
    }

    TestManager_AssertEqual( TKEL_ReceiveEvent( ms2tick(100), &aEvent ), TKEL_TIMEOUT, "receive event has timed out" );
    
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemEventId), TKEL_NO_ERR, "post on semaphore" );
}

/*! \fn void TestSendManyTimesAndReceive( void )
 *  \brief This test checks that many times the same event send makes one event received.
 */
void TestSendManyTimesAndReceive()
{
    TKEL_task_id gTaskSendManyTimesAndReceive;
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemEventId), TKEL_NO_ERR, "create semaphore" );
    
    TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,
            TaskSendManyTimesAndReceive,(void*) 0,&gTaskSendManyTimesAndReceive), TKEL_NO_ERR, "create task" );

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemEventId), TKEL_NO_ERR, "wait for receiver semaphore" );
  TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemEventId), TKEL_NO_ERR, "delete semaphore" );
    
    /* the following line should be replaced by TKEL_Join. The delay is there to avoid 
    an error on the deletetask, when task has not completly finished. */
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "wait for 1/2s" );
    TestManager_AssertEqual( TKEL_DeleteTask(gTaskSendManyTimesAndReceive), TKEL_NO_ERR, "delete task" );
}

/*! \fn void TestCase_ExecuteTestEventReceptionOnSameEvent( void )
 *  \brief This test checks that many times the same event send makes one event received.
 */
void TestCase_ExecuteTestEventReceptionOnSameEvent()
{
    gSameEvtToSend = 1;
    TestSendManyTimesAndReceive();
}

/*! \fn void TestCase_ExecuteTestEventReceptionOnSameEvent( void )
 *  \brief This test checks that many times the same event send makes one event received.
 */
void TestCase_ExecuteTestEventReceptionOnDifferentEvent()
{
    gSameEvtToSend = 0;
    TestSendManyTimesAndReceive();
}

/*! \fn void TaskEventSender( void* arg )
 *  \brief This task send one event
 */
void TaskEventSendOneEvent(void * arg)
{
    TestManager_AssertEqual( TKEL_SendEvent(gTaskEventReceiver,EVENT10), TKEL_NO_ERR, "send event" );
}

/*! \fn void TaskEventReceiveInTwoSeconds( void* arg )
 *  \brief This task send events
 */
void TaskEventReceiveInTwoSeconds(void * arg)
{
    TKEL_evts aEvent;
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(2000)), TKEL_NO_ERR, "wait for 2s" );
    TestManager_AssertEqual( TKEL_ReceiveEvent(0, &aEvent), TKEL_NO_ERR, "receive event" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemEventId), TKEL_NO_ERR, "signal reception" );
}

/*! \fn void TestSenderReceiver( void )
 *  \brief This test checks that a receive works after one sending
 */
void TestCase_ExecuteTestEventReceptionAfterSending()
{
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemEventId), TKEL_NO_ERR, "create semaphore" );
    
    TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,TaskEventReceiveInTwoSeconds,
            (void*) 0,&gTaskEventReceiver), TKEL_NO_ERR, "create task receiver" );
    
    TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,TaskEventSendOneEvent,(void*) 0,&gTaskEventSender), TKEL_NO_ERR, "create task sender" );
    
	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemEventId), TKEL_NO_ERR, "wait for receiver semaphore" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemEventId), TKEL_NO_ERR, "delete semaphore" );

    /* the following line should be replaced by TKEL_Join. The delay is there to avoid 
    an error on the deletetask, when task has not completly finished. */
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "wait for 1/2s" );
    TestManager_AssertEqual( TKEL_DeleteTask(gTaskEventSender), TKEL_NO_ERR, "delete sender task " );
    TestManager_AssertEqual( TKEL_DeleteTask(gTaskEventReceiver), TKEL_NO_ERR, "delete receiver task " );
}

/*! \fn void TaskResetAndReceive( void* arg )
 *  \brief This task send one event, then receive, reset and receive again
 */
void TaskResetAndReceive(void * arg)
{
    TKEL_task_id aTaskId;
    TKEL_evts aEvent;
    TestManager_AssertEqual( TKEL_GetCurrentTask( &aTaskId ), TKEL_NO_ERR, "get current task" );
    
    TestManager_AssertEqual( TKEL_SendEvent(aTaskId,EVENT10), TKEL_NO_ERR, "send event" );
    TestManager_AssertEqual( TKEL_ReceiveEvent(100, &aEvent), TKEL_NO_ERR, "receive event" );
    TestManager_AssertEqual( aEvent, EVENT10, "expected event" );
    
    TestManager_AssertEqual( TKEL_SendEvent(aTaskId,EVENT10), TKEL_NO_ERR, "send event" );
    TestManager_AssertEqual( TKEL_ResetEvent(), TKEL_NO_ERR, "reset event" );
    TestManager_AssertEqual( TKEL_ReceiveEvent(100, &aEvent), TKEL_TIMEOUT, "receive timedout" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemEventId), TKEL_NO_ERR, "signal reception" );
}

/*! \fn void TaskResetAndReceive( void* arg )
 *  \brief This test checks that after a reset, no more event is received
 */
void TestCase_ExecuteTestResetEvent()
{
    TKEL_task_id aTask;
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemEventId), TKEL_NO_ERR, "create semaphore" );
    
    TestManager_AssertEqual(
        TKEL_CreateTask(1,0,0,TaskResetAndReceive,
            (void*) 0,&aTask), TKEL_NO_ERR, "create task receiver" );

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemEventId), TKEL_NO_ERR, "wait for receiver semaphore" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemEventId), TKEL_NO_ERR, "delete semaphore" );

    /* the following line should be replaced by TKEL_Join. The delay is there to avoid 
    an error on the deletetask, when task has not completly finished. */
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "wait for 1/2s" );
    TestManager_AssertEqual( TKEL_DeleteTask(aTask), TKEL_NO_ERR, "delete task" );
}

/*! \fn void TaskEventSender( void* arg )
 *  \brief This task send events
 */
void TaskEventSender(void * arg)
{
    int i;
    for(i=0; i<gNumberOfLoop; i++ )
    {
        if( gSlowSender )
        {
            TestManager_AssertEqual( TKEL_DelayTask( ms2tick(100) ), TKEL_NO_ERR, "delay 1/10s" );
        }
        
        TestManager_AssertEqual( TKEL_SendEvent(gTaskEventReceiver,EVENT10), TKEL_NO_ERR, "send event" );
    }
    
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemEventId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TaskEventReceiver( void* arg )
 *  \brief This task receives events
 */
void TaskEventReceiver(void * arg)
{
    TKEL_evts aEvent;
    while(gLoop)
    {
        if( gSlowReceiver )
        {
            TestManager_AssertEqual( TKEL_DelayTask( ms2tick(100) ), TKEL_NO_ERR, "delay 1/10s" );
        }
        
        TestManager_AssertEqual( TKEL_ReceiveEvent(0, &aEvent), TKEL_NO_ERR, "receive event" );
        TestManager_AssertEqual( aEvent, EVENT10, "expected event" );
    }
    TestManager_AssertEqual( TKEL_SignalSemaphore(gSemReceiverId), TKEL_NO_ERR, "signal receiver semaphore" );
}

/*! \fn void TestSenderReceiver( void )
 *  \brief This test performs a event sender/receiver simulation
 */
void TestSenderReceiver()
{
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemEventId), TKEL_NO_ERR, "create semaphore" );
    TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemReceiverId), TKEL_NO_ERR, "create semaphore" );

    gLoop = 1;
    TestManager_AssertEqual(TKEL_CreateTask(1,0,0,TaskEventReceiver,(void*)0,&gTaskEventReceiver),TKEL_NO_ERR,"create task receiver");
    TestManager_AssertEqual(TKEL_CreateTask(1,0,0,TaskEventSender,(void*)0,&gTaskEventSender),TKEL_NO_ERR,"create task sender");

    /* wait for producer's end */
	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemEventId), TKEL_NO_ERR, "wait for semaphore" );
    
    /* now, stop receiver by sending a last event */
    gLoop = 0;
    TestManager_AssertEqual( TKEL_SendEvent(gTaskEventReceiver,EVENT10), TKEL_NO_ERR, "send last event" );
    TestManager_AssertEqual( TKEL_WaitSemaphore(gSemReceiverId), TKEL_NO_ERR, "wait for receiver semaphore" );
    
  TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemReceiverId), TKEL_NO_ERR, "delete semaphore" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemEventId), TKEL_NO_ERR, "delete semaphore" );
	
    /* the following line should be replaced by TKEL_Join. The delay is there to avoid 
	an error on the deletetask, when task has not completly finished. */
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "wait for 1/2s" );
    TestManager_AssertEqual( TKEL_DeleteTask(gTaskEventSender), TKEL_NO_ERR, "delete sender task" );
    TestManager_AssertEqual( TKEL_DeleteTask(gTaskEventReceiver), TKEL_NO_ERR, "delete receiver task" );

}

/*! \fn void TaskMeasureTimeout( void )
 *  \brief This test measures timeout
 */
void TaskMeasureTimeout(void * arg)
{
    TKEL_evts aEvent;
    tTestManagerChrono aChrono;
    long aElapsedMs=0;
    const int aErrorLimit = 200;
    TestManager_StartChrono(&aChrono);
    TestManager_AssertEqual( TKEL_ReceiveEvent(ms2tick(1000),&aEvent), TKEL_TIMEOUT, "TIMEOUT 1s OK" );
    TestManager_StopChrono(&aChrono, &aElapsedMs);
    TestManager_AssertGreater( aErrorLimit, aElapsedMs-1000.0, "time elapsed is near 1s" );

    TestManager_StartChrono(&aChrono);
    TestManager_AssertEqual( TKEL_ReceiveEvent(ms2tick(2000),&aEvent), TKEL_TIMEOUT, "TIMEOUT 2s OK" );
    TestManager_StopChrono(&aChrono, &aElapsedMs);
    TestManager_AssertGreater( aErrorLimit, aElapsedMs - 2000.0, "time elapsed is near 2s" );
    
    TestManager_StartChrono(&aChrono);
    TestManager_AssertEqual( TKEL_ReceiveEvent(ms2tick(3000),&aEvent), TKEL_TIMEOUT, "TIMEOUT 3s OK" );
    TestManager_StopChrono(&aChrono, &aElapsedMs);
    TestManager_AssertGreater( aErrorLimit, aElapsedMs - 3000.0, "time elapsed is near 3s" );
    
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemEventId), TKEL_NO_ERR, "signal receiver semaphore" );
}

/*! \fn void TestCase_ExecuteTestTimeoutTimeMeasure( void )
 *  \brief This test performs a event sender/receiver simulation
 */
void TestCase_ExecuteTestTimeoutTimeMeasure()
{
    TKEL_task_id aTaskId;
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemEventId), TKEL_NO_ERR, "create semaphore" );
    
    TestManager_AssertEqual(TKEL_CreateTask(1,0,0,TaskMeasureTimeout,(void*)0,&aTaskId), TKEL_NO_ERR, "create task" );

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemEventId), TKEL_NO_ERR, "wait for semaphore" );
    
	TestManager_AssertEqual(TKEL_DeleteTask(aTaskId), TKEL_NO_ERR, "delete task" );
  TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemEventId), TKEL_NO_ERR, "delete semaphore" );
}

/*! \fn void TaskMeasureTimeout( void )
 *  \brief This test receive event with timeout
 */
void TaskReceiveWithTimeout(void * arg)
{
    TKEL_evts aEvent;
    TestManager_AssertEqual( TKEL_ReceiveEvent(ms2tick(10000),&aEvent), TKEL_NO_ERR, "receive event" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemEventId), TKEL_NO_ERR, "signal receiver semaphore" );
}

/*! \fn void TestCase_ExecuteTestSendEventDuringStartedReceptionWithTimeout( void )
 *  \brief This test starts a receive event with 10s timeout. Main task sends event in 5s.
 */
void TestCase_ExecuteTestSendEventDuringStartedReceptionWithTimeout()
{
    TKEL_task_id aTaskId;
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemEventId), TKEL_NO_ERR, "create semaphore" );
    
    TestManager_AssertEqual(TKEL_CreateTask(1,0,0,TaskReceiveWithTimeout,(void*)0,&aTaskId), TKEL_NO_ERR, "create task" );
    
    TestManager_AssertEqual( TKEL_DelayTask( ms2tick(5000) ), TKEL_NO_ERR, "wait for 5s" );
    TestManager_AssertEqual( TKEL_SendEvent( aTaskId, EVENT10), TKEL_NO_ERR, "send event" );
    
	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemEventId), TKEL_NO_ERR, "wait for semaphore" );

	TestManager_AssertEqual(TKEL_DeleteTask(aTaskId), TKEL_NO_ERR, "delete task" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemEventId), TKEL_NO_ERR, "delete semaphore" );
}
