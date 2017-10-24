/** \file tkelmutexttests.c
 *  \brief Test mutex for TKEL layer
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

#include "tbox.h"
#include "tkel.h"
#include "TestManager.h"

typedef void (*tFunctionTask) (void*);

void TestCase_ExecuteBasicMutexOperation(void);
void TestCase_ExecuteMutexMultipleLockInSameThread(void);
void TestCase_ExecuteMutexTryLockSimple(void);
void TestCase_ExecuteMutexTryLockMultiple(void);
void TestCase_ExecuteMutexDetionOnLockedMutex(void);

static tTestNode gTestMutexBasic = {
    "TestBasicMutex",
    "test the basic mutex operation",
    0, /* not interactive */
    TestCase_ExecuteBasicMutexOperation,
		NULL
};

static tTestNode gTestMutexMultipleLockPerThread = {
    "TestMutexMultipleLockPerThread",
    "test the fact that a thread can lock a mutex multiple time (ref counting)",
    0, /* not interactive */
    TestCase_ExecuteMutexMultipleLockInSameThread,
		NULL
};

static tTestNode gTestMutexTryLockSimple = {
    "TestMutexTryLockSimple",
    "test the mutex trylock on a mutex, 1 time",
    0, /* not interactive */
    TestCase_ExecuteMutexTryLockSimple,
		NULL
};

static tTestNode gTestMutexTryLockMultiple = {
    "TestMutexTryLockMultiple",
    "test the mutex trylock on a mutex, with multiple times (ref counting)",
    0, /* not interactive */
    TestCase_ExecuteMutexTryLockMultiple,
		NULL
};

static tTestNode gTestMutexDeletionOnLockedMutex = {
    "TestMutexDeletionOnLockedMutex",
    "test the mutex deletion on a locked mutex",
    0, /* not interactive */
    TestCase_ExecuteMutexDetionOnLockedMutex,
		NULL
};

tTestNode* gMutexTestNodeList[] = {
  &gTestMutexBasic,
  &gTestMutexMultipleLockPerThread,
	&gTestMutexTryLockSimple,
	&gTestMutexTryLockMultiple,
	&gTestMutexDeletionOnLockedMutex,
	NULL
};

tTestNode gTKELMutexTestSuite = {
    "TestMutex",
    "test the mutex",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gMutexTestNodeList
};

/*! \var TKEL_mutex_id mid
 *  \brief Mutex identificator used to lock/unlock mutex
 *  \see TestMutex()
 */
static TKEL_mutex_id mid;
/*! \var long gCodePass
 *  \brief Variable used to determine how many threads have done their task
 *  \see TaskLockWorkUnlock()
 */
static long gCodePass=0;

/*! \fn void TaskLockWorkUnlock( void * arg )
 *  \brief Task executed by a created thread
 \verbatim
 Lock the mutex mid <BR>
 Sleep during 2 seconds <BR>
 Increment gCodePass (to signal that work is done) <BR>
 Unlock the mutex mid
 \endverbatim
 *  \param[in] arg see \a nNumberOfLockUnlock
 *  \see TestMutex()
 */
void TaskLockWorkUnlock(void * arg)
{
	int i;
	TKEL_err aErr;
	int n = (int)arg;
	for( i=0; i < n; i++ )
	{
		TestManager_AssertEqual( TKEL_LockMutex(mid), TKEL_NO_ERR, "lock mutex" );
	}

 	aErr = TKEL_DelayTask(ms2tick(2000)); /* TO DO: replace with a "log differing" */
	gCodePass++;

    if(gCodePass < 0xFF)
    {
        for( i=0; i < n; i++ )
        {
            TestManager_AssertEqual( TKEL_UnlockMutex(mid), TKEL_NO_ERR, "unlock mutex" );
        }
    }

	TestManager_AssertEqual( aErr, TKEL_NO_ERR, "unlock mutex" );
}

/*! \fn void TaskTryLockWorkUnlock( void * arg )
 *  \brief Task executed by a created thread
 \verbatim
 Try to lock the mutex mid, each 10/second <BR>
 Sleep during 2 seconds <BR>
 Increment gCodePass (to signal that work is done) <BR>
 Unlock the mutex mid
 \endverbatim
 *  \param[in] arg see \a nNumberOfLockUnlock
 *  \see TestMutex()
 */
void TaskTryLockWorkUnlock(void * arg)
{
	int i;
	int n = (int)arg;
	TKEL_err aErr = TKEL_NOT_DONE;

	while( aErr == TKEL_NOT_DONE )
	{
		TKEL_DelayTask(ms2tick(100));
		aErr = TKEL_TryLockMutex(mid);
	}

	TestManager_AssertEqual( aErr, TKEL_NO_ERR, "first trylock mutex" );

	/* mutex is now locked. lock other n-1 times if test asks it */
	for( i=1; i < n; i++ )
	{
		TestManager_AssertEqual( TKEL_TryLockMutex(mid), TKEL_NO_ERR, "other trylock mutex" );
	}

	/* Task now "working" */
	TKEL_DelayTask(ms2tick(1500));
	gCodePass++;

	/* Unlock mutex */
	for( i=0; i < n; i++ )
	{
		TestManager_AssertEqual( TKEL_UnlockMutex(mid), TKEL_NO_ERR, "unlock mutex" );
	}
}

/*! \fn void TestMutex( int nNumberOfLockUnlock )
 *  \brief General function to test TKEL mutex
 \verbatim
 Create a mutex identificator mid <BR>
 Create 3 TKEL threads which run TaskLockWorkUnlock()
 Active wait for termination as TKEL does not implement a task_join
 Check that work is done with gCodePass variable
 \endverbatim
 *  \param[in] nNumberOfLockUnlock Number of lock/unlock done by thread
 *  \see TaskLockWorkUnlock()
 */
void TestMutex( int nNumberOfLockUnlock, tFunctionTask aTask )
{
  TKEL_task_id tid1,tid2,tid3;
	/*tTestManagerChrono aChrono;*/
	/*long aElapsedMs=0;*/

	gCodePass = 0;
	TestManager_AssertEqual( TKEL_CreateMutex(&mid), TKEL_NO_ERR, "create mutex" );

/*	TestManager_StartChrono(&aChrono);*/

  TestManager_AssertEqual( TKEL_CreateTask(1,0,0,aTask,(void*) nNumberOfLockUnlock,&tid1), TKEL_NO_ERR, "create task1" );
  TestManager_AssertEqual( TKEL_CreateTask(1,0,0,aTask,(void*) nNumberOfLockUnlock,&tid2), TKEL_NO_ERR, "create task2" );;
  TestManager_AssertEqual( TKEL_CreateTask(1,0,0,aTask,(void*) nNumberOfLockUnlock,&tid3), TKEL_NO_ERR, "create task3" );;

	/* TKEL_TaskJoin should be a better implementation, but it does not exist in TKEL */
	/* When task_join is used, we can use chrono and assert that elapsed time is greater than 6s*/
  TestManager_AssertEqual( TKEL_DelayTask(ms2tick(8000)), TKEL_NO_ERR, "delay" );

	/* 3 tasks work : all tasks must have worked completely */
	TestManager_AssertEqual( gCodePass, 3, "Assert that 3 threads have worked" );

	/* global elapsed time must exceed 3*2 = 6 seconds */
	/*TestManager_StopChrono(&aChrono, &aElapsedMs );
	TestManager_AssertGreater( aElapsedMs, 6000, "time elapsed greater than 6s" );*/

	TestManager_AssertEqual( TKEL_DeleteMutex(mid), TKEL_NO_ERR, "delete mutex" );

	TestManager_AssertEqual( TKEL_DeleteTask(tid1), TKEL_NO_ERR, "delete task1" );
	TestManager_AssertEqual( TKEL_DeleteTask(tid2), TKEL_NO_ERR, "delete task2" );
	TestManager_AssertEqual( TKEL_DeleteTask(tid3), TKEL_NO_ERR, "delete task3" );
}

/*! \fn void TestCase_ExecuteBasicMutexOperation( void )
 *  \brief This test performs basic mutex operation: create lock unlock delete
 *  with 1 lock/unlock per thread
 *  \see TestMutex() for algorithm
 */
void TestCase_ExecuteBasicMutexOperation()
{
  TestMutex( 1, TaskLockWorkUnlock );
}

/*! \fn void TestCase_ExecuteMutexMultipleLockInSameThread( void )
 *  \brief This test performs basic mutex operation: create lock unlock delete
 *  with 5 lock/unlock per thread
 *  \see TestMutex() for algorithm
 */
void TestCase_ExecuteMutexMultipleLockInSameThread()
{
  TestMutex( 5, TaskLockWorkUnlock );
}

/*! \fn void TestCase_ExecuteMutexTrySimple( void )
 *  \brief This test performs a try lock of a free mutex
 * other tasks try to lock every second. 1 lock only in the same thread.
 *  \see TestMutex() for algorithm
 */
void TestCase_ExecuteMutexTryLockSimple()
{
  TestMutex( 1, TaskTryLockWorkUnlock );
}

/*! \fn void TestCase_ExecuteMutexTrySimple( void )
 *  \brief This test performs a try lock of a free mutex
 * other tasks try to lock every second. multiple locks in the same thread.
 *  \see TestMutex() for algorithm
 */
void TestCase_ExecuteMutexTryLockMultiple()
{
  TestMutex( 5, TaskTryLockWorkUnlock );
}

/*! \fn void TestCase_ExecuteMutexDetionOnLockedMutex( void )
 *  \brief This test performs a mutex deletion when mutex is locked
 */
void TestCase_ExecuteMutexDetionOnLockedMutex()
{
	TKEL_task_id tid1;
    TKEL_err     retError;

	gCodePass = 0;
	TestManager_AssertEqual( TKEL_CreateMutex(&mid), TKEL_NO_ERR, "create mutex" );
	TestManager_AssertEqual( TKEL_CreateTask(1,0,0,TaskLockWorkUnlock,(void*) 1,&tid1), TKEL_NO_ERR, "create task" );
	TKEL_DelayTask(ms2tick(500));
    retError = TKEL_DeleteMutex(mid);
    if(retError == TKEL_NOT_DONE)
    {
        mTBOX_PRINT((kTBOX_LF, "delete mutex not done" ));
    	TestManager_AssertEqual( gCodePass, 0, "Assert that thread has not worked yet" );
    	TKEL_DelayTask(ms2tick(3000));
    	TestManager_AssertEqual( gCodePass, 1, "Assert that thread has worked" );
    	TestManager_AssertEqual( TKEL_DeleteMutex(mid), TKEL_NO_ERR, "delete mutex" );
    }
    else if(retError == TKEL_NO_ERR)
    {
        gCodePass = 0xFF;
        mTBOX_PRINT((kTBOX_LF,"WARNING : OS permit the deletion of locked mutex"));
    }
    else
    {
        gCodePass = 0xFF;
        mTBOX_PRINT((kTBOX_LF,"ERROR TKEL_DeleteMutex"));
    }
    TestManager_AssertEqual( TKEL_DeleteTask(tid1), TKEL_NO_ERR, "delete task" );
}
