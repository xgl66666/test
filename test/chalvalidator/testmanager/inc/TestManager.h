/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: TestManager
 *
 * FILE NAME: $URL: $
 *            $Rev: $
 *            $Date: $
 *
 * PUBLIC
 *
 * DESCRIPTION: definition of the API of TestManager
 *
 *****************************************************************************/

#ifndef _TEST_MANAGER_H
#define _TEST_MANAGER_H

#include "tkel.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*=============================== Public types ===============================*/
/* TKEL error codes */
typedef enum
{
    TESTMANAGER_NO_ERR,    /* No error. */
    TESTMANAGER_NOT_DONE,  /* Command not done. */
    TESTMANAGER_BAD_ARG    /* Bad argument. */
} TESTMANAGER_err;

typedef void (*tTestCase_Execute)(void);

/* The TestCase */
typedef struct
{
    char* msName;
    char* msPurpose;
    int   mbIsFunctionalyInteractive;
    tTestCase_Execute mExecuteFunction;
		void** mTestNodeVector;  /* null terminated list of test node */
} tTestNode;

/* The TestResult */
typedef struct
{
		tTestNode* mTestNode;
    long mTheOkTotal;
    long mTheFailureTotal;
		long mNbExecution;   /* total count of execution for a testcase */
		long mNbSonFullyExecuted; /* number of fully executed sons for a given test suite */
} tTestResult;

/* The TestManager */
typedef struct
{
    int mInteractiveMode;
    int mStopAtFirstErrorMode;
		tTestResult* mTestResultVector;
		tTestResult* mCurrentTestResult;
} tTestManager;

typedef struct
{
	TKEL_tck mStartTime;
	TKEL_tck mStopTime;
} tTestManagerChrono;

extern tTestNode gTestRoot;
extern tTestManager gTestManager;

/*============================= Public functions =============================*/

/**
	* @brief Init this module
	*/
TESTMANAGER_err TestManager_Init(void);

/**
	* @brief This methods logs a string message.
	*/
TESTMANAGER_err TestManager_LogMessage( char* aMessage );

/**
	* @brief This method logs an error message, formatting it
	* to emphasize the error aspect.
	*/
TESTMANAGER_err TestManager_LogErrorMessage( char* aMessage );

/**
	* @brief Ask a closed question to user, for checking purpose
	*
	*  In interactive mode, user should enter answer on standard input,
	* after aMessage is displayed on standard output.
	* Increments Oks if Y, increments Failures if N.
	*  In non interactive mode, question will be displayed shorly, and
	* test goes on.
	*/
TESTMANAGER_err TestManager_AskForChecking( char* aQuestionMessage );

/**
	* @brief Ask a closed question to user, for checking purpose
	*
	*  In interactive mode, user should enter answer on standard input,
	* after aMessage is displayed on standard output.
	* Increments Oks if Y, increments Failures if N.
	*  In non interactive mode, question will be displayed shorly, and
	* test goes on after a timeout.
	*/
TESTMANAGER_err TestManager_AskForCheckingTimeout( char* aQuestionMessage , unsigned int timeout_ms );

/**
	* @brief Ask to user to realize an action
	*
	*  In interactive mode, user should realize action (for instance,
	* press a key on the	remote control, plug or unplug the cable,
	* press a button on the front panel, etc.).
	*  It should not be called in non interactive mode.
	*/
TESTMANAGER_err TestManager_AskForAction( char* aActionMessage );

/**
	* @brief This methods asks user an integer with a txt prompt message
	* and a max size length to read.
	*/
int TestManager_GetNumber(char *txt, unsigned char size);


/**
	* @brief This methods asks user a character with a txt prompt message.
	*/
unsigned char TestManager_GetChar(char *txt);


/**
	* @brief This methods asserts a fact, and increments the corresponding
	* counter wheter the assert is true or false.
	*/
TESTMANAGER_err TestManager_AssertTrue( int aAssert, char* aMessage );

/**
	* @brief This methods asserts equality between a and b.
	*/
TESTMANAGER_err TestManager_AssertEqual( long aLeftExpr, long aRightExpr, char* aMessage );

/**
	* @brief This methods asserts a being greater than b.
	*/
TESTMANAGER_err TestManager_AssertGreater( double aLeftExpr, double aRightExpr, char* aMessage );

/**
	* @brief This methods starts a chrono
	*/
TESTMANAGER_err TestManager_StartChrono( tTestManagerChrono* a);

/**
	* @brief This methods stops a chrono, and returns the elapsed ms
	*/
TESTMANAGER_err TestManager_StopChrono( tTestManagerChrono*, long* ms );

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _TEST_MANAGER_H */
