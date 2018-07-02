/** \file tkeltimertests.c
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
#include "TestManager.h"

typedef void (*tFunctionTask) (void*);

static TKEL_sema_id gSemTimerId;
static TKEL_timer_id gTimerId;
static TKEL_task_id gTaskId1, gTaskId2;
static int gLoopTask2 = 0;
static int gCheckerTimerCB = 0;

void TestCase_ExecuteTestStartTimer();
void TestCase_ExecuteTestRepeatedStartTimer();
void TestCase_ExecuteTestStartStopTimer();
void TestCase_ExecuteTestStartTimerAlreadyStarted();
void TestCase_ExecuteTestDeleteTimerOnStartedTimer();
void TestCase_ExecuteTestSetTimerOwnerTask();
void TestCase_ExecuteTestStopTimerOnInit();
void TestCase_ExecuteTestDoubleStopTimer();
void TestCase_ExecuteTestTimerUnitTest();

static tTestNode gTestStartTimer = {
    "TestStartTimer",
    "test the start timer operation",
    0, /* not interactive */
    TestCase_ExecuteTestStartTimer,
		NULL
};

static tTestNode gTestRepeatedStartTimerOnEvtReception = {
    "TestRepeatedStartTimerOnEvtReception",
    "test repeated start timer on timer evt reception",
    0, /* not interactive */
    TestCase_ExecuteTestRepeatedStartTimer,
		NULL
};

static tTestNode gTestStartStopTimer = {
    "TestStartStopTimer",
    "test the stop following an immediate start",
    0, /* not interactive */
    TestCase_ExecuteTestStartStopTimer,
		NULL
};

static tTestNode gTestStartTimerAlreadyStarted = {
    "TestStartTimerAlreadyStarted",
    "test start timer on a started timer",
    0, /* not interactive */
    TestCase_ExecuteTestStartTimerAlreadyStarted,
		NULL
};

static tTestNode gTestStopTimerOnInit = {
    "TestStopTimerOnInit",
    "test stoptimer after a create",
    0, /* not interactive */
    TestCase_ExecuteTestStopTimerOnInit,
		NULL
};

static tTestNode gTestDoubleStopTimer = {
    "TestDoubleStopTimer",
    "test stop timer on a stopped timer",
    0, /* not interactive */
    TestCase_ExecuteTestDoubleStopTimer,
		NULL
};

static tTestNode gTestDeleteTimerOnStartedTimer = {
    "TestDeleteTimerOnStartedTimer",
    "test a deletetimer on a started timer",
    0, /* not interactive */
    TestCase_ExecuteTestDeleteTimerOnStartedTimer,
		NULL
};

static tTestNode gTestSetTimerOwnerTask = {
    "TestSetTimerOwnerTask",
    "test a SetTimerOwnerTask and check good evt reception",
    0, /* not interactive */
    TestCase_ExecuteTestSetTimerOwnerTask,
		NULL
};


static tTestNode gTestTimerUnitTest = {
    "TimerUnitTest",
    "test a basics",
    0, /* not interactive */
    TestCase_ExecuteTestTimerUnitTest,
		NULL
};

tTestNode* gTimerTestNodeList[] = {
	&gTestStartTimer,
	&gTestRepeatedStartTimerOnEvtReception,
	&gTestStartStopTimer,
	&gTestStartTimerAlreadyStarted,
	&gTestStopTimerOnInit,
	&gTestDoubleStopTimer,
	&gTestDeleteTimerOnStartedTimer,
	&gTestSetTimerOwnerTask,
	&gTestTimerUnitTest,
	NULL
};

tTestNode gTKELTimerTestSuite = {
    "TestTimer",
    "test the timer API",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTimerTestNodeList
};


/*! \fn void TaskStartTimer( void )
 *  \brief This test start a simple timer
 */
void TaskStartTimer(void* arg)
{
  TKEL_timer_id tmid;
	TKEL_evts aEvent;
  TestManager_AssertEqual( TKEL_CreateTimer(&tmid), TKEL_NO_ERR, "create timer" );
  TestManager_AssertEqual( TKEL_StartTimer(tmid, ms2tick(10000), 0x01), TKEL_NO_ERR, "start timer (10000 ms, 0x01)" );
  TestManager_AssertEqual( TKEL_ReceiveEvent(0, &aEvent), TKEL_NO_ERR, "receive timer evt" );
  TestManager_AssertEqual( aEvent, 0x01, "evt is the expected one" );
  TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
  TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NO_ERR, "delete timer" );
}

/*! \fn void TestTimer( void )
 *  \brief This test start a simple timer
 */
void TestTimer( tFunctionTask aTask )
{
	TKEL_task_id aTaskId;
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemTimerId), TKEL_NO_ERR, "create semaphore" );

	TestManager_AssertEqual(
		TKEL_CreateTask(1,0,0,
			aTask,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task" );

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemTimerId), TKEL_NO_ERR, "wait semaphore" );

/*	TestManager_AssertEqual(
		TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "delay 1/2s" ); TO DO: replace with TKEL_Join */
	TestManager_AssertEqual( TKEL_DeleteTask(aTaskId), TKEL_NO_ERR, "delete task" );
}

/*! \fn void DoNothing( void )
 *  \brief This task does nothing
 */
void DoNothing() { }

void DoSomething() { gCheckerTimerCB = 1; }

/*! \fn void TaskUnitTimer( void )
 *  \brief This task test unitely
 */
void TaskUnitTimer(void * arg)
{
	TestManager_AssertEqual( TKEL_CreateTimer(&gTimerId), TKEL_NO_ERR, "create timer" );

	TestManager_AssertEqual(
		TKEL_StartTimer(gTimerId, 0, 0x10), TKEL_BAD_ARG, "bad arg: timer with null time" );

	TestManager_AssertEqual(
		TKEL_StartTimer(gTimerId, ms2tick(1000), 0x0), TKEL_BAD_ARG, "bad arg: timer with null event" );

	TestManager_AssertEqual(
		TKEL_StartTimerWithCB(gTimerId, 0, DoNothing), TKEL_BAD_ARG, "bad arg: timer with null time (callback)" );

	TestManager_AssertEqual(
		TKEL_StartTimerWithCB(gTimerId, ms2tick(1000), NULL), TKEL_BAD_ARG, "bad arg: timer with null calback)" );

	gCheckerTimerCB = 0;

	TestManager_AssertEqual(
		TKEL_StartTimerWithCB(gTimerId, ms2tick(1000), DoSomething), TKEL_NO_ERR,  "good call, timer with callback 1s)" );

	TestManager_AssertEqual(TKEL_DelayTask(ms2tick(1000)), TKEL_NO_ERR, "wait 1s)");

	TestManager_AssertEqual(gCheckerTimerCB, 1, "callback was called!");

	TestManager_AssertEqual( TKEL_DeleteTimer(gTimerId), TKEL_NO_ERR, "delete timer" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestTimerUnitTest( void )
 *  \brief This test tests bad parameters on timer api
 */
void TestCase_ExecuteTestTimerUnitTest()
{
	TestTimer( TaskUnitTimer );
}

/*! \fn void TestCase_ExecuteTestStartTimer( void )
 *  \brief This test start a simple timer
 */
void TestCase_ExecuteTestStartTimer()
{
	TestTimer( TaskStartTimer );
}

/*! \fn void TaskRestartTimerOnReception( void )
 *  \brief This test restart timer on reception
 */
void TaskRestartTimerOnReception(void* arg )
{
	TKEL_evts aEvent;
	int nNb = 0;
	TestManager_AssertEqual( TKEL_CreateTimer(&gTimerId), TKEL_NO_ERR, "create timer" );
	TestManager_AssertEqual(
		TKEL_StartTimer(gTimerId, ms2tick(500), 0x01), TKEL_NO_ERR, "start timer" );

	while( nNb < 10 )
	{
		TestManager_AssertEqual( TKEL_ReceiveEvent(0, &aEvent), TKEL_NO_ERR, "receive timer evt" );
		TestManager_AssertEqual( aEvent, 0x01, "evt is the expected one" );

		TestManager_AssertEqual(
			TKEL_StartTimer(gTimerId, ms2tick(100), 0x01), TKEL_NO_ERR, "restart timer (100 ms, 0x01)" );
		nNb++;
	}

	TestManager_AssertEqual( TKEL_ReceiveEvent(0, &aEvent), TKEL_NO_ERR, "receive timer evt" );
	TestManager_AssertEqual( aEvent, 0x01, "evt is the expected one" );
	TestManager_AssertEqual( TKEL_DeleteTimer(gTimerId), TKEL_NO_ERR, "delete timer" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestRepeatedStartTimer( void )
 *  \brief This test start a timer many times
 */
void TestCase_ExecuteTestRepeatedStartTimer()
{
	TestTimer( TaskRestartTimerOnReception );
}

/*! \fn void TaskStartTimerAndStop( void )
 *  \brief This test start a simple timer and stop immediatly
 */
void TaskStartTimerAndStop(void* arg)
{
  TKEL_timer_id tmid;
	TKEL_evts aEvent;
  TestManager_AssertEqual( TKEL_CreateTimer(&tmid), TKEL_NO_ERR, "create timer" );
  TestManager_AssertEqual( TKEL_StartTimer(tmid, ms2tick(1000), 0x01), TKEL_NO_ERR, "start timer (100 ms, 0x01)" );
  TestManager_AssertEqual( TKEL_StopTimer(tmid), TKEL_NO_ERR, "stop timer" );
  TestManager_AssertEqual( TKEL_ReceiveEvent(100, &aEvent), TKEL_TIMEOUT, "receive timed out" );
  TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NO_ERR, "delete timer" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestStartStopTimer( void )
 *  \brief This test start and then stop before timer occurs
 */
void TestCase_ExecuteTestStartStopTimer()
{
	TestTimer( TaskStartTimerAndStop );
}

/*! \fn void TaskDoubleStartTimer( void )
 *  \brief This test start a simple timer twice
 */
void TaskDoubleStartTimer(void* arg)
{
	TKEL_timer_id tmid;
	TestManager_AssertEqual( TKEL_CreateTimer(&tmid), TKEL_NO_ERR, "create timer" );
	TestManager_AssertEqual( TKEL_StartTimer(tmid, ms2tick(1000), 0x01), TKEL_NO_ERR, "start timer (100 ms, 0x01)" );
	TestManager_AssertEqual( TKEL_StartTimer(tmid, ms2tick(1000), 0x01), TKEL_NOT_DONE, "start timer not done" );
	TestManager_AssertEqual( TKEL_StopTimer(tmid), TKEL_NO_ERR, "stop timer" );
	TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NO_ERR, "delete timer" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestRepeatedStartTimer( void )
 *  \brief This test start a timer many times
 */
void TestCase_ExecuteTestStartTimerAlreadyStarted()
{
	TestTimer( TaskDoubleStartTimer );
}

/*! \fn void TaskStopOnInit( void )
 *  \brief This test stops a timer after a create.
 */
void TaskStopOnInit(void* arg)
{
  TKEL_timer_id tmid;
  TestManager_AssertEqual( TKEL_CreateTimer(&tmid), TKEL_NO_ERR, "create timer" );
  TestManager_AssertEqual( TKEL_StopTimer(tmid), TKEL_NOT_DONE, "stop timer not done" );
  TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NO_ERR, "delete timer" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestStopTimerAlreadyStopped( void )
 *  \brief This test a stop on a stopped timer
 */
void TestCase_ExecuteTestStopTimerOnInit()
{
	TestTimer( TaskStopOnInit );
}

/*! \fn void TaskDoubleStop( void )
 *  \brief This task stops a timer twice after a start.
 */
void TaskDoubleStop(void* arg)
{
	TKEL_timer_id tmid;
	TestManager_AssertEqual( TKEL_CreateTimer(&tmid), TKEL_NO_ERR, "create timer" );
	TestManager_AssertEqual( TKEL_StartTimer(tmid, ms2tick(1000), 0x01), TKEL_NO_ERR, "stop timer" );
	TestManager_AssertEqual( TKEL_StopTimer(tmid), TKEL_NO_ERR, "stop timer" );
	TestManager_AssertEqual( TKEL_StopTimer(tmid), TKEL_NOT_DONE, "stop timer a second time not done" );
	TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NO_ERR, "delete timer" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestDoubleStop( void )
 *  \brief This test stops a timer twice after a start.
 */
void TestCase_ExecuteTestDoubleStopTimer()
{
	TestTimer( TaskDoubleStop );
}

/*! \fn void TaskDeleteTimerAfterStart( void )
 *  \brief This task deletes timer after start
 */
void TaskDeleteTimerAfterStart(void* arg)
{
	TKEL_timer_id tmid;
	TestManager_AssertEqual( TKEL_CreateTimer(&tmid), TKEL_NO_ERR, "create timer" );
	TestManager_AssertEqual( TKEL_StartTimer(tmid, ms2tick(1000), 0x01), TKEL_NO_ERR, "stop timer" );
	TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NOT_DONE, "delete timer not done" );
	TestManager_AssertEqual( TKEL_StopTimer(tmid), TKEL_NO_ERR, "stop timer" );
	TestManager_AssertEqual( TKEL_DeleteTimer(tmid), TKEL_NO_ERR, "delete timer" );

	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestDoubleStop( void )
 *  \brief This test stops a timer twice after a start.
 */
void TestCase_ExecuteTestDeleteTimerOnStartedTimer()
{
	TestTimer( TaskDeleteTimerAfterStart );
}

/*! \fn void Task1( void )
 *  \brief This task starts timer, and changes reception to task 2
 */
void Task1(void* arg)
{
	int nNb = 0;
  TestManager_AssertEqual( TKEL_StartTimer(gTimerId, ms2tick(1000), 0x01), TKEL_NO_ERR, "start timer" );
  TestManager_AssertEqual( TKEL_SetTimerOwnerTask(gTimerId, gTaskId2), TKEL_NO_ERR, "change to task2" );

	while( nNb < 10 )
	{
		TKEL_evts aEvent;
		TestManager_AssertEqual( TKEL_ReceiveEvent(0, &aEvent), TKEL_NO_ERR, "receive event in task 1" );
		/* commented to minimize log : need to be reactive when "log differed"
		TestManager_AssertEqual( aEvent, 0x01, "expected timer event" );*/
		TestManager_AssertEqual( TKEL_StartTimer(gTimerId, ms2tick(1000), 0x01), TKEL_NO_ERR, "start timer" );
		TestManager_AssertEqual( TKEL_SetTimerOwnerTask(gTimerId, gTaskId2), TKEL_NO_ERR, "change to task2" );
		nNb++;
	}

	gLoopTask2 = 0;
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTimerId), TKEL_NO_ERR, "wait semaphore" );
}

/*! \fn void Task2( void )
 *  \brief This task restarts timer on reception and changes reception to task 1
 */
void Task2(void* arg)
{
	TKEL_evts aEvent;
	while( gLoopTask2 )
	{
		TestManager_AssertEqual( TKEL_ReceiveEvent(ms2tick(3000), &aEvent), TKEL_NO_ERR, "receive event in task 2" );
		/* commented to minimize log : need to be reactive when "log differed"
		TestManager_AssertEqual( aEvent, 0x01, "expected timer event" );*/
		if( !gLoopTask2 )
		{
			break;
		}
		TestManager_AssertEqual( TKEL_StartTimer(gTimerId, ms2tick(1000), 0x01), TKEL_NO_ERR, "start timer" );
		TestManager_AssertEqual( TKEL_SetTimerOwnerTask(gTimerId, gTaskId1), TKEL_NO_ERR, "change to task1" );
	}
}

/*! \fn void TestCase_ExecuteTestSetTimerOwnerTask( void )
 *  \brief This test validates the changes of task reception.
 */
void TestCase_ExecuteTestSetTimerOwnerTask()
{
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemTimerId), TKEL_NO_ERR, "create semaphore" );
	TestManager_AssertEqual( TKEL_CreateTimer(&gTimerId), TKEL_NO_ERR, "create timer" );

	TestManager_AssertEqual(
		TKEL_CreateTask(1,0,0,
			Task1,(void*) 0,&gTaskId1), TKEL_NO_ERR, "create task 1" );
	gLoopTask2 = 1;
	TestManager_AssertEqual(
		TKEL_CreateTask(1,0,0,
			Task2,(void*) 0,&gTaskId2), TKEL_NO_ERR, "create task 2" );

	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemTimerId), TKEL_NO_ERR, "wait semaphore" );

	TestManager_AssertEqual(
		TKEL_DelayTask(ms2tick(3500)), TKEL_NO_ERR, "delay 3s1/2" ); /* TO DO: replace with TKEL_Join */
	TestManager_AssertEqual( TKEL_DeleteTask(gTaskId1), TKEL_NO_ERR, "delete task1" );
	TestManager_AssertEqual( TKEL_DeleteTask(gTaskId2), TKEL_NO_ERR, "delete task2" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemTimerId), TKEL_NO_ERR, "delete semaphore" );
	TestManager_AssertEqual( TKEL_DeleteTimer(gTimerId), TKEL_NO_ERR, "delete timer" );
}
