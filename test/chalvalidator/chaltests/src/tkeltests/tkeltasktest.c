/** \file tkeltasktest.c
 *  \brief Test Task tests for TKEL layer
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

void dumpPthreadSchedParam(char*);
void setPthreadSchedParam(char* fctName, int priority);
void TestCase_ExecuteTestCreateTask(void);
void TestCase_ExecuteTestDeleteTask(void);
void TestCase_ExecuteTestSelfDeleteTask(void);
void TestCase_ExecuteTestPriority(void);
void TestCase_ExecuteTestGetCurrentTask(void);
void TestCase_ExecuteTestTaskSuspendResume(void);
void TestCase_ExecuteTestStressTask(void);

#define TEST_STACK_SIZE 1024
void* TaskStack[TEST_STACK_SIZE];
int gCodePass=0;
int gAskSuspend=0;
int gLoop=1;

TKEL_task_id gTaskToKill;
TKEL_sema_id gSemTaskId;

static tTestNode gTestCreateTask = {
    "TestCreateTask",
    "test the create task operation",
    0, /* not interactive */
    TestCase_ExecuteTestCreateTask,
        NULL
};

static tTestNode gTestDeleteTask= {
    "TestDeleteTask",
    "test that task deletion works on stopped task and not on working task",
    0, /* not interactive */
    TestCase_ExecuteTestDeleteTask,
        NULL
};

static tTestNode gTestSelfDeleteTask= {
    "TestSelfDeleteTask",
    "test that self task deletion works correctly",
    0, /* not interactive */
    TestCase_ExecuteTestSelfDeleteTask,
        NULL
};

static tTestNode gTestPriority= {
    "TestPriority",
    "test priority api",
    0, /* not interactive */
    TestCase_ExecuteTestPriority,
        NULL
};

static tTestNode gTestGetCurrentTask = {
    "TestGetCurrentTask",
    "test getCurrentTask",
    0, /* not interactive */
    TestCase_ExecuteTestGetCurrentTask,
        NULL
};

static tTestNode gTestTaskSuspendResume = {
    "TestSuspendResumeTask",
    "test that task can be suspended and resumed correctly",
    0, /* not interactive */
    TestCase_ExecuteTestTaskSuspendResume,
        NULL
};

static tTestNode gTestTaskStress= {
    "TestStressTask",
    "test that task creation/deletion works in stress conditions",
    0, /* not interactive */
    TestCase_ExecuteTestStressTask,
        NULL
};

tTestNode* gTaskNodeList[] = {
  &gTestCreateTask,
  &gTestDeleteTask,
    &gTestSelfDeleteTask,
    &gTestPriority,
    &gTestGetCurrentTask,
    &gTestTaskSuspendResume,
    &gTestTaskStress,
    NULL
};

tTestNode gTKELTaskTestSuite = {
    "TestTask",
    "test the task API",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTaskNodeList
};

/*! \fn void TaskDoIt( void * arg )
 *  \brief Task that check it's executed
 */
void TaskDoIt(void * arg)
{
    gCodePass++;
}

/*! \fn void TestCase_ExecuteTestCreateTask( void )
 *  \brief This test creates task with different parameters
 */
void TestCase_ExecuteTestCreateTask()
{
    TKEL_task_id aTaskId;
    gCodePass = 0;
    TestManager_AssertEqual( TKEL_CreateTask(1,0,0,
        TaskDoIt,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task with no stack" );
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1000)), TKEL_NO_ERR, "main waits for 1s" );
    TestManager_AssertEqual( gCodePass, 1, "task executed" );

    TestManager_AssertEqual( TKEL_CreateTask(1, "TaskDoIt", TEST_STACK_SIZE,
        TaskDoIt,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task with a stack" );
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1000)), TKEL_NO_ERR, "main waits for 1s" );
    TestManager_AssertEqual( gCodePass, 2, "task executed" );
}

/*! \fn void TaskTestLoopOfWork( void * arg )
 *  \brief Task that works and sleeps
 */
void TaskTestLoopOfWork(void * arg)
{
/*    dumpPthreadSchedParam("TaskTestLoopOfWork"); */
    while(gLoop)
    {
        gCodePass++;
        TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "wait 1/2 s" );
        if( gAskSuspend )
        {
            TestManager_AssertEqual( TKEL_SuspendTask(), TKEL_NO_ERR, "task suspend itself" );
        }
    }
}

/*! \fn void TestCase_ExecuteTestDeleteTask( void )
 *  \brief This test check that task deletion does not stop it and waits for its end
 */
void TestCase_ExecuteTestDeleteTask()
{
    TKEL_task_id aTaskId;
    int aPrevCodePass;

    gCodePass = 0;
    gAskSuspend = 0;
    gLoop = 1;

/*    setPthreadSchedParam("TestCase_ExecuteTestDeleteTask", 1); */
    TestManager_AssertEqual( TKEL_CreateTask(1,0,0,
        TaskTestLoopOfWork,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task" );

    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1850)), TKEL_NO_ERR, "main wait less than 2 s" );
    TestManager_AssertTrue( gCodePass >= 3, "more than 3 steps executed" );

    /* TestManager_AssertEqual( TKEL_DeleteTask(aTaskId), TKEL_NOT_DONE, "delete task don't work on working task" ); */
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(2000)), TKEL_NO_ERR, "wait 2 s" );
    TestManager_AssertTrue( gCodePass >= 6, "more than 6 steps executed" );
    gLoop = 0;
    aPrevCodePass = gCodePass;
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1000)), TKEL_NO_ERR, "wait 1 s" );
    TestManager_AssertEqual( gCodePass, aPrevCodePass , "no other steps executed" );
    TestManager_AssertEqual( TKEL_DeleteTask(aTaskId), TKEL_NO_ERR, "delete task works on completed task" );
    gLoop = 1;
}

/*! \fn void TaskSelfDelete( void * arg )
 *  \brief Task that deletes itself returns not done
 */
void TaskSelfDelete(void * arg)
{
    TKEL_task_id aSelfTask;
    TestManager_AssertEqual( TKEL_GetCurrentTask(&aSelfTask), TKEL_NO_ERR, "get current task" );
    TestManager_AssertEqual( TKEL_DeleteTask(aSelfTask), TKEL_NO_ERR, "delete task" );
    gCodePass++;
}

/*! \fn void TestCase_ExecuteTestSelfDeleteTask( void )
 *  \brief This test check that task can self-delete correctly.
 */
void TestCase_ExecuteTestSelfDeleteTask()
{
    TKEL_task_id aTaskId;
    gCodePass = 0;
    TestManager_AssertEqual( TKEL_CreateTask(1,0,0,
        TaskSelfDelete,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task" );

    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(3000)), TKEL_NO_ERR, "main wait 3 s" );
    TestManager_AssertEqual( gCodePass, 0, "0 step executed" );
}

/*! \fn void TestCase_ExecuteTestPriority( void )
 *  \brief This test performs set and get priority on task
 *  \verbatim
 *  note that the getPriority value could be different from the set value
 *  on OS having less than 256 priority value. This is why we have two
 *  sequences of set/get, to manage the first rounding.
 *  \endverbatim
 *  \bug on 5105, this test fails. Probably a bug in the priority conversion
 *  between tkel priorities and os20 priorities.
 */
void TestCase_ExecuteTestPriority()
{
    TKEL_prio aPrio;
    TKEL_task_id aTaskId;

    gCodePass = 0;
    gAskSuspend = 0;
    gLoop = 1;

    TestManager_AssertEqual( TKEL_CreateTask(1,0,0,
        TaskTestLoopOfWork,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task" );

    TestManager_AssertEqual( TKEL_SetTaskPriority(aTaskId, 0), TKEL_BAD_ARG, "0 is not a task priority" );
    TestManager_AssertEqual( TKEL_SetTaskPriority(aTaskId, 128), TKEL_BAD_ARG, "128 is too high for task priority" );


    /*--------------------------------------------------------------------*/
    /* test OK only if the number priority levels of the OS under test is */
    /* higher than 128                                                    */
    /* else the test can't be OK                                          */
    /*--------------------------------------------------------------------*/
    for( aPrio = 1; aPrio < 127; aPrio++ )
    {
        TKEL_prio aGetPrio1;
#if 0
        TKEL_prio aGetPrio2;
#endif
        TestManager_AssertEqual( TKEL_SetTaskPriority(aTaskId, aPrio), TKEL_NO_ERR, "set task priority" );
        TestManager_AssertEqual( TKEL_GetTaskPriority(aTaskId, &aGetPrio1), TKEL_NO_ERR, "get task priority" );
#if 0
        if(aGetPrio1 != 0 && aGetPrio1 != 128)
        {
            TestManager_AssertEqual( TKEL_SetTaskPriority(aTaskId, aGetPrio1), TKEL_NO_ERR, "set task priority" );
            TestManager_AssertEqual( TKEL_GetTaskPriority(aTaskId, &aGetPrio2), TKEL_NO_ERR, "get task priority" );

            TestManager_AssertEqual( aGetPrio1, aGetPrio2, "priorities are equal" );
        }
#endif

    }
    gLoop = 0;
    TestManager_AssertEqual( TKEL_DeleteTask( aTaskId ), TKEL_NO_ERR, "delete task" );
    gLoop = 1;
}

/*! \fn void TaskTryLockWorkUnlock( void * arg )
 *  \brief Task executed by a created thread
 */
void TaskTestGetCurrentTask(void * arg)
{
    TestManager_AssertEqual( TKEL_GetCurrentTask(&gTaskToKill), TKEL_NO_ERR, "get current task" );
	TestManager_AssertEqual( TKEL_SignalSemaphore(gSemTaskId), TKEL_NO_ERR, "signal semaphore" );
}

/*! \fn void TestCase_ExecuteTestGetCurrentTask( void )
 *  \brief This test creates a task, that gives its id to the main process, to be killed.
 */
void TestCase_ExecuteTestGetCurrentTask()
{
    TKEL_task_id aTaskId;

    TestManager_AssertEqual( TKEL_CreateSemaphore(0, &gSemTaskId), TKEL_NO_ERR, "create semaphore with 0 resource" );
    TestManager_AssertEqual( TKEL_CreateTask(1,0,0,
                             TaskTestGetCurrentTask,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task" );
    TestManager_AssertEqual( TKEL_WaitSemaphore(gSemTaskId), TKEL_NO_ERR, "wait for semaphore" );
    TestManager_AssertEqual( TKEL_DeleteTask( gTaskToKill ), TKEL_NO_ERR, "delete task" );
    TestManager_AssertEqual( TKEL_DeleteSemaphore( gSemTaskId), TKEL_NO_ERR, "delete semaphore" );
}

/*! \fn void TestCase_ExecuteTestTaskDelay( void )
 *  \brief This test tests task delay
 */
void TestCase_ExecuteTestTaskDelay()
{
/*
Call TKEL_CreateTask. This task asks for a user start, saying sleep of 30s. Then calls TKEL_DelayTask(30000). This task asks user is 30s have elapsed.
Call TKEL_Join*/
}

/*! \fn void TestCase_ExecuteTestTaskSuspendResume( void )
 *  \brief This test tests task suspend and resume
 */
void TestCase_ExecuteTestTaskSuspendResume()
{
    TKEL_task_id aTaskId;
    int gPrevCodePass;

    gCodePass = 0;
    gAskSuspend = 0;
    gLoop = 1;

    TestManager_AssertEqual( TKEL_CreateTask(1,0,0,
        TaskTestLoopOfWork,(void*) 0,&aTaskId), TKEL_NO_ERR, "create task" );

    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1850)), TKEL_NO_ERR, "main wait less than 2 s" );
    TestManager_AssertTrue( gCodePass >= 3, "more than 3 steps executed" );

    gAskSuspend = 1;
    gPrevCodePass = gCodePass;
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(2000)), TKEL_NO_ERR, "wait 2 s" );
    TestManager_AssertEqual( gCodePass, gPrevCodePass, "no more steps executed" );

    gAskSuspend = 0;
    TestManager_AssertEqual( TKEL_ResumeTask(aTaskId), TKEL_NO_ERR, "resume task" );
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(1850)), TKEL_NO_ERR, "main wait less than 2 s" );
    TestManager_AssertTrue( gCodePass >= 6, "more than 6 steps executed" );

    gLoop = 0;
    TestManager_AssertEqual( TKEL_DelayTask(ms2tick(500)), TKEL_NO_ERR, "main wait 1/2 s" );
    TestManager_AssertEqual( TKEL_DeleteTask(aTaskId), TKEL_NO_ERR, "delete task" );
    gLoop = 1;
}


/*! \fn void TaskTestLoopOfWork( void * arg )
 *  \brief Task that works and sleeps
 */
void TaskTestLoopOfWorkStress(void * arg)
{
    char buffer[40];

/*    dumpPthreadSchedParam("TaskTestLoopOfWorkStress"); */
    TKEL_DelayTask(ms2tick(100));
    if(gCodePass%100==0)
    {
	    sprintf(buffer,"task %d delete itself ...", gCodePass);
	    TestManager_LogMessage(buffer);
    }
    TKEL_DeleteTask(NULL);
}

/*! \fn void TestCase_ExecuteTestStressTask( void )
 *  \brief This test check that task creation/deletion works in stress conditions
 */
void TestCase_ExecuteTestStressTask()
{
    TKEL_task_id aTaskId;
    TKEL_err err; 
    int i;
    char buffer[40];

    gCodePass = 0;
    gAskSuspend = 0;

    for ( i=0 ; i<=3000 ; i++)
    {
/*	    setPthreadSchedParam("TestCase_ExecuteTestStressTask", 1); */
	    err = TKEL_CreateTask(1,0,0,TaskTestLoopOfWorkStress,(void*) 0,&aTaskId);
	    if (err != TKEL_NO_ERR)
	    {
		    sprintf(buffer,"create task %d", gCodePass);
		    TestManager_AssertEqual( err, TKEL_NO_ERR, buffer );
		    return;
	    }
	    else
	    {
		    if(gCodePass%100==0)
		    {
			    sprintf(buffer,"create task %d", gCodePass);
			    TestManager_LogMessage(buffer);
		    }
		    TKEL_DelayTask(ms2tick(200));
	    }
	    gCodePass ++;
    }
}

