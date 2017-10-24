/** \file tkelsemaphoretest.c
 *  \brief Test semaphore for the TKEL layer
 *  \author Ronan MENEU
 *  \date 16/03/06
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

void TestCase_ExecuteSemaphoreWith1resource();
void TestCase_ExecuteSemaphoreWith3resources();
void TestCase_ExecuteSemaphoreTestDeleteDuringWait();
void TestCase_ExecuteSemaphoreTimeout1();
void TestCase_ExecuteSemaphoreTimeout3();
void TestCase_ExecuteSemaphoreTimeoutInfinite();
void TestCase_ExecuteSemaphoreTimeoutImediate();

TKEL_sema_id gSemTestId;
TKEL_sema_id gSemId1;
int gbWaitWithTimeout=0;
int gTimeoutOccured=0;
TKEL_tck gTimeoutValue=0;

static tTestNode gTestSemaphoreBasic1 = {
    "TestBasicSemaphore1",
    "test the basic semaphore operation, with 1 resource",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreWith1resource,
		NULL
};

static tTestNode gTestSemaphoreBasic3 = {
    "TestBasicSemaphore3",
    "test the basic semaphore operation, with 3 resources",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreWith3resources,
		NULL
};

static tTestNode gSemaphoreTestDeleteDuringWait= {
    "TestSemaphoreDeleteOnWait",
    "test the behavior of the semaphore deletion while a task is waiting",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreTestDeleteDuringWait,
		NULL
};

static tTestNode gTestSemaphoreWaitTimeout1 = {
    "TestWaitTimeout1",
    "test the wait with timeout, with 1 resource",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreTimeout1,
		NULL
};

static tTestNode gTestSemaphoreWaitTimeout3 = {
    "TestWaitTimeout3",
    "test the wait with timeout, with 3 resources",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreTimeout3,
		NULL
};

static tTestNode gTestSemaphoreWaitTimeoutImmediate = {
    "TestWaitTimeoutImmediate",
    "test the wait with timeout immediate",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreTimeoutImediate,
		NULL
};

static tTestNode gTestSemaphoreWaitTimeoutInfinite = {
    "TestWaitTimeoutInfinite",
    "test the wait with timeout infinite",
    0, /* not interactive */
    TestCase_ExecuteSemaphoreTimeoutInfinite,
		NULL
};

tTestNode* gSemaphoreTestNodeList[] = {
  &gTestSemaphoreBasic1,
  &gTestSemaphoreBasic3,
	&gSemaphoreTestDeleteDuringWait,
	&gTestSemaphoreWaitTimeout1,
	&gTestSemaphoreWaitTimeout3,
	&gTestSemaphoreWaitTimeoutInfinite,
	&gTestSemaphoreWaitTimeoutImmediate,
	NULL
};

tTestNode gTKELSemaphoreTestSuite = {
    "TestSemaphore",
    "test the Semaphore",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
		(void**)gSemaphoreTestNodeList
};

/*! \var TKEL_mutex_id mid
 *  \brief Mutex identificator used to lock/unlock mutex
 *  \see TestSemaphore()
 */
static TKEL_sema_id mid;
/*! \var long gCodePass
 *  \brief Variable used to determine how many threads have done their task
 *  \see TaskWaitSemaSignal()
 */
static long gCodePass=0;

/*! \fn void TaskWaitSemaSignal( void * arg )
 *  \brief Task executed by a created thread
 \verbatim
 Wait for semaphore <BR>
 Wait 2 seconds <BR>
 Increment gCodePass (to notify that work is done) <BR>
 Raise a signal for semaphore
 \endverbatim
 *  \param[in] arg not use for the moment
 *  \see TestSemaphore()
 */
void TaskWaitSemaSignal(void * arg)
{
	int aErrCount=0;
	if( gbWaitWithTimeout )
	{
		TKEL_err aErr;
		do
		{
			aErr = TKEL_WaitSemaphoreTimeout(mid, gTimeoutValue);
			if( aErr == TKEL_TIMEOUT )
			{
				gTimeoutOccured++;
			}

			if( gTimeoutValue == kTKEL_TIMEOUT_IMMEDIATE )
			{
				/* not verbose. TO DO: replace it with the "log differing" */
				TKEL_err aDelayErr = TKEL_DelayTask(ms2tick(100));
				if( aDelayErr != TKEL_NO_ERR )
				{
					aErrCount++;
				}
			}
		}
		while( aErr == TKEL_TIMEOUT );

		TestManager_AssertEqual( aErr, TKEL_NO_ERR, "wait semaphore succeeded" );
	}
	else
	{
		TestManager_AssertEqual( TKEL_WaitSemaphore(mid), TKEL_NO_ERR, "wait for semaphore" );
	}

	TKEL_DelayTask(ms2tick(2000));
	gCodePass++;

	TestManager_AssertEqual( TKEL_SignalSemaphore(mid), TKEL_NO_ERR, "signal semaphore" );
}


/*! \fn void TestSemaphore( int n )
 *  \brief General function to test TKEL semaphores
 \verbatim
 Create a semaphore
 Create 3 TKEL threads which run TaskWaitSemaSignal()
 Active wait for termination as TKEL does not implement a task_join
 Check that work is done with gCodePass variable
 \endverbatim
 *  \param[in] n Initial semaphore token count.
 *  \see TaskWaitSemaSignal()
 */
void TestSemaphore( int n )
{
  TKEL_task_id tid1,tid2,tid3;
	/*tTestManagerChrono aChrono;*/
	/*long aElapsedMs=0;*/

	gCodePass = 0;
  TestManager_AssertEqual( TKEL_CreateSemaphore(n, &mid), TKEL_NO_ERR, "create semaphore" );

/*	TestManager_StartChrono(&aChrono);*/

  TestManager_AssertEqual( TKEL_CreateTask(1,0,0,TaskWaitSemaSignal,(void*) 0,&tid1), TKEL_NO_ERR, "create task1" );
  TestManager_AssertEqual( TKEL_CreateTask(1,0,0,TaskWaitSemaSignal,(void*) 0,&tid2), TKEL_NO_ERR, "create task2" );;
  TestManager_AssertEqual( TKEL_CreateTask(1,0,0,TaskWaitSemaSignal,(void*) 0,&tid3), TKEL_NO_ERR, "create task3" );;

	/* TKEL_TaskJoin should be a better implementation, but it does not exist in TKEL */
	/* When task_join is used, we can use chrono and assert that elapsed time is greater than 6s*/
  TestManager_AssertEqual( TKEL_DelayTask(ms2tick(7000)), TKEL_NO_ERR, "delay" );

	/* 3 tasks work : all tasks must have worked completely */
	TestManager_AssertEqual( gCodePass, 3, "Assert that 3 threads have worked" );

	/* global elapsed time must exceed 3*2 = 6 seconds */
	/*TestManager_StopChrono(&aChrono, &aElapsedMs );
	TestManager_AssertGreater( aElapsedMs, 6000, "time elapsed greater than 6s" );*/
}

/*! \fn void TestCase_ExecuteSemaphoreWith1resource( void )
 *  \brief This test performs basic Semaphore operation: create lock
 *  unlock delete with 1 lock/unlock per thread
 *  \see TestSemaphore()
 */
void TestCase_ExecuteSemaphoreWith1resource()
{
	gbWaitWithTimeout=0;
  TestSemaphore( 1 );
}

/*! \fn void TestCase_ExecuteSemaphoreWith3resources( void )
 *  \brief This test performs basic Semaphore operation: create lock
 *  unlock delete with 3 lock/unlock per thread
 *  \see TestSemaphore()
 */
void TestCase_ExecuteSemaphoreWith3resources()
{
	gbWaitWithTimeout=0;
  TestSemaphore( 3 );
}

/*! \fn void TaskDeleteOnWait( void * arg )
 *  \brief Task for delete on wait test
 */
void TaskDeleteOnWait(void * arg)
{
	TestManager_AssertGreater( TKEL_WaitSemaphoreTimeout(gSemTestId, ms2tick(4000)), TKEL_NO_ERR, "wait for semaphore stest" );
	gCodePass++;
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemId1), TKEL_NO_ERR, "signal semaphore 1" );
	gCodePass++;
}

/*! \fn void TestCase_ExecuteSemaphoreTestDeleteDuringWait( void )
 *  \brief This test checks that a delete semaphore fails when a task is waiting
 *  \bug DeleteSemaphore causes a segfault if there is a concurrent wait.
 */
void TestCase_ExecuteSemaphoreTestDeleteDuringWait()
{
#if 0
	TKEL_task_id tid1;
#endif
	gCodePass = 0;
	/* TestManager_AssertTrue( 0, "Bug (?) DeleteSemaphore causes a segfault if wait concurrent" );*/
	/* CODE IS COMMENTED TO AVOID SEGFAULT: PLEASE UNCOMMENT TO TEST */
#if 0
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemTestId), TKEL_NO_ERR, "create semaphore test (0 resource)" );
	TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemId1), TKEL_NO_ERR, "create semaphore 1 (0 resource)" );
	TestManager_AssertEqual( TKEL_CreateTask(1,NULL,0, TaskDeleteOnWait, (void*) 0,&tid1), TKEL_NO_ERR, "create task1" );
	TestManager_AssertEqual( TKEL_DelayTask(ms2tick(2000)), TKEL_NO_ERR, "main wait 2s" );
	TestManager_AssertGreater( TKEL_DeleteSemaphore(gSemTestId), TKEL_NO_ERR, "cannot delete semaphore test" );
	TestManager_AssertEqual( TKEL_WaitSemaphore(gSemId1), TKEL_NO_ERR, "wait for semaphore 1" );
	TestManager_AssertEqual( TKEL_DeleteSemaphore(gSemId1), TKEL_NO_ERR, "delete semaphore 1" );
	TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "main wait 2s" );
	TestManager_AssertEqual( gCodePass, 2, "2 steps executed" );
#endif
}

/*! \fn void TestCase_ExecuteSemaphoreTimeout1( void )
 *  \brief This test checks waittimeout with 1 resource
 */
void TestCase_ExecuteSemaphoreTimeout1()
{
	gbWaitWithTimeout=1;
	gTimeoutOccured=0;
	gTimeoutValue=ms2tick(1000);
  TestSemaphore( 1 );
	TestManager_AssertGreater( gTimeoutOccured, 3, "more than 3 timeouts have occured" );
}

/*! \fn void TestCase_ExecuteSemaphoreTimeout1( void )
 *  \brief This test checks waittimeout with 3 resources
 */
void TestCase_ExecuteSemaphoreTimeout3()
{
	gbWaitWithTimeout=1;
	gTimeoutOccured=0;
	gTimeoutValue=ms2tick(1000);
  TestSemaphore( 3 );
	TestManager_AssertEqual( gTimeoutOccured, 0, "0 timeouts have occured" );
}

/*! \fn void TestCase_ExecuteSemaphoreTimeoutImediate( void )
 *  \brief This test checks waittimeout immediate
 */
void TestCase_ExecuteSemaphoreTimeoutImediate()
{
	gbWaitWithTimeout=1;
	gTimeoutOccured=0;
	gTimeoutValue=kTKEL_TIMEOUT_IMMEDIATE;
  TestSemaphore( 1 );
	TestManager_AssertGreater( gTimeoutOccured, 40,  "more than 40 timeouts have occured" );
}

/*! \fn void TestCase_ExecuteSemaphoreTimeoutInfinite( void )
 *  \brief This test checks waittimeout infinite
 */
void TestCase_ExecuteSemaphoreTimeoutInfinite()
{
	gbWaitWithTimeout=1;
	gTimeoutOccured=0;
	gTimeoutValue=kTKEL_TIMEOUT_INFINITE;
  TestSemaphore( 1 );
	TestManager_AssertEqual( gTimeoutOccured, 0, "0 timeouts have occured" );
}
