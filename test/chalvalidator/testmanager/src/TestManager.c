/******************************************************************************
 *                     COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TKEL
 *
 * FILE NAME: $URL$
 *            $Rev$
 *            $Date$
 *
 * DESCRIPTION : Miscellaneous API of Thin Kernel Encapsulation Layer (T-KEL)
 *
 *****************************************************************************/


/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "TestManager.h"
#include "tbox.h"
#include "tkel.h"

struct tTestManagerChronoImplementation
{
	TKEL_tck mStartTime;
	TKEL_tck mStopTime;
} ;

tTestManager gTestManager;

TESTMANAGER_err TestManager_AddOk(void);
TESTMANAGER_err TestManager_AddFailure(void);
TESTMANAGER_err TestManager_AskQuestion( char* aQuestionMessage );

/**
	* @brief Init this module
	*/
TESTMANAGER_err TestManager_Init()
{
  gTestManager.mInteractiveMode = 1; /* default value: interactive mode */
  gTestManager.mStopAtFirstErrorMode = 0 /* default value: don't stop at first error */;

  gTestManager.mTestResultVector = NULL;

	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This methods logs a string message.
	*/
TESTMANAGER_err TestManager_LogMessage( char* aMessage )
{
  mTBOX_PRINT((kTBOX_LF, "%s\n",aMessage));
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This method logs an error message, formatting it
	* to emphasize the error aspect.
	*/
TESTMANAGER_err TestManager_LogErrorMessage( char* aMessage )
{
  mTBOX_PRINT((kTBOX_LF, "ERROR: %s\n", aMessage));
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief Ask a closed question to user, for checking purpose
	*
	*  In interactive mode, user should enter answer on standard input,
	* after aMessage is displayed on standard output.
	* Increments Oks if Y, increments Failures if N.
	*  In non interactive mode, question will be displayed shorly, and
	* test goes on.
	*/
TESTMANAGER_err TestManager_AskForChecking( char* aQuestionMessage )
{
    return TestManager_AskForCheckingTimeout( aQuestionMessage , 1000 );
}

/**
	* @brief Ask a closed question to user, for checking purpose
	*
	*  In interactive mode, user should enter answer on standard input,
	* after aMessage is displayed on standard output.
	* Increments Oks if Y, increments Failures if N.
	*  In non interactive mode, question will be displayed shorly, and
	* test goes on after a timeout.
	*/
TESTMANAGER_err TestManager_AskForCheckingTimeout( char* aQuestionMessage , unsigned int timeout_ms )
{
	if( gTestManager.mInteractiveMode )
	{
		return TestManager_AskQuestion(aQuestionMessage);
	}
	else
	{
		mTBOX_PRINT((kTBOX_LF, "NOT INTERACTIVE MODE\n%s ?\n", aQuestionMessage ));
		mTBOX_PRINT((kTBOX_LF, "Waiting for %s milliseconds ...\n", timeout_ms ));
		TKEL_DelayTask(ms2tick(timeout_ms));
		mTBOX_PRINT((kTBOX_LF, "Test goes on\n" ));
		return TESTMANAGER_NO_ERR;
	}
}

/**
	* @brief Ask to user to realize an action
	*
	*  In interactive mode, user should realize action (for instance,
	* press a key on the	remote control, plug or unplug the cable,
	* press a button on the front panel, etc.).
	*  It should not be called in non interactive mode.
	*/
TESTMANAGER_err TestManager_AskForAction( char* aActionMessage )
{
	if( gTestManager.mInteractiveMode )
	{
		mTBOX_PRINT((kTBOX_LF, "USER ACTION REQUIRED: %s\n", aActionMessage ));
		return TESTMANAGER_NO_ERR;
	}
	else
	{
  	mTBOX_PRINT((kTBOX_LF, "SHOULD NOT BE CALLED\n" ));
  	return TESTMANAGER_NOT_DONE;
	}
}

/**
	* @brief This methods asserts a fact, and increments the corresponding
	* counter wheter the assert is true or false.
	*/
TESTMANAGER_err TestManager_AssertTrue( int aAssert, char* aMessage )
{
	if( aAssert )
	{
			TestManager_AddOk();
			mTBOX_PRINT((kTBOX_LF, "OK: %s\n", aMessage ));
	}
	else
	{
			TestManager_AddFailure();
			mTBOX_PRINT((kTBOX_LF, "ERROR. AssertTrue failed: %s\n", aMessage ));
	}
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This methods asserts equality between a and b.
	*/
TESTMANAGER_err TestManager_AssertEqual( long aLeftExpr, long aRightExpr, char* aMessage )
{
	if( aLeftExpr == aRightExpr )
	{
		TestManager_AddOk();
		mTBOX_PRINT((kTBOX_LF, "OK: %s\n", aMessage ));
	}
	else
	{
		TestManager_AddFailure();
		mTBOX_PRINT((kTBOX_LF,
			 "ERROR. AssertEqual fails: %d and %d are different.\n       '%s'\n", aLeftExpr, aRightExpr, aMessage ));
	}
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This methods asserts a greater than b.
	*/
TESTMANAGER_err TestManager_AssertGreater( double aLeftExpr, double aRightExpr, char* aMessage )
{
	if( aLeftExpr > aRightExpr )
	{
		TestManager_AddOk();
		mTBOX_PRINT((kTBOX_LF, "OK: %s\n", aMessage ));
	}
	else
	{
		TestManager_AddFailure();
		/* printf %f does not work on ST. So don't print values */
		mTBOX_PRINT((kTBOX_LF, "ERROR. AssertGreater fails: \n       '%s'\n", aMessage ));
	}
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This methods starts a chrono
	*/
TESTMANAGER_err TestManager_StartChrono( tTestManagerChrono* aChrono)
{
	TKEL_GetTick( &aChrono->mStartTime );
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This methods stops a chrono, and returns the elapsed ms
	*/
TESTMANAGER_err TestManager_StopChrono( tTestManagerChrono* aChrono, long* ms )
{
	TKEL_GetTick( &aChrono->mStopTime );
	*ms = tick2ms( aChrono->mStopTime - aChrono->mStartTime );
	return TESTMANAGER_NO_ERR;
}

/* ************* implementation ********** */
/**
 * @brief Increment ok count of manager and of current context
 */
TESTMANAGER_err TestManager_AddOk()
{
	if( gTestManager.mCurrentTestResult == NULL ) /* safe test: should not happen */
	{
		mTBOX_PRINT((kTBOX_LF, "ERROR NO TEST RESULT" ));
		return TESTMANAGER_NOT_DONE;
	}

  gTestManager.mCurrentTestResult->mTheOkTotal++;
	return TESTMANAGER_NO_ERR;
}

/**
 * @brief Increment failure count of manager and of current context
 */
TESTMANAGER_err TestManager_AddFailure()
{
	if( gTestManager.mCurrentTestResult == NULL ) /* safe test: should not happen */
	{
		mTBOX_PRINT((kTBOX_LF, "ERROR NO TEST RESULT" ));
		return TESTMANAGER_NOT_DONE;
	}

	gTestManager.mCurrentTestResult->mTheFailureTotal++;
	return TESTMANAGER_NO_ERR;
}

/**
	* @brief This methods asks user a question
	*/
TESTMANAGER_err TestManager_AskQuestion( char* aQuestionMessage )
{
#if 1
	char cAskResult[256];
	mTBOX_PRINT((kTBOX_LF, "%s ? (0=NO, 1=YES) : ", aQuestionMessage));
	mTBOX_GETSTRING(( cAskResult ));
	mTBOX_PRINT((kTBOX_LF,"%s\n", cAskResult));
	if ('1' != cAskResult[0])
#else
   char cAskResult;
	mTBOX_PRINT((kTBOX_LF, "%s ? (0=NO, 1=YES) : ", aQuestionMessage));
	mTBOX_GETCHAR(( &cAskResult ));
	mTBOX_PRINT((kTBOX_LF,"%c\n", cAskResult));
	if ('1' != cAskResult)
#endif
	{
		return TestManager_AddFailure();
	}
	else
	{
		return TestManager_AddOk();
	}
}

/**
	* @brief This methods asks user an integer with a txt prompt message
	* and a max size length to read.
	*/
int TestManager_GetNumber(char *txt, unsigned char size)
{
    int8_t  entry = 0;
    int8_t  end;
    int8_t  rg;
    int32_t result = 0;

    if(txt != NULL)
	{
        mTBOX_PRINT((kTBOX_LF, "\n%s", txt));
	}

    end = 0;
    rg  = 0;

    while(!end)
    {
        TKEL_DelayTask(ms2tick(100));
        mTBOX_GETCHAR(((char *)&entry));
		mTBOX_PRINT((kTBOX_LF, "%c", entry));

        if(entry >= '0' && entry <= '9')
        {
            entry -= '0';
        }
        else if(entry >= 'a' && entry <= 'f')
        {
            entry = entry - 'a' + 10;
        }
        else if(entry == 10 || entry == 13)
        {
            end = 1;
            continue;
        }
        else
		{
            continue;
		}

        result *= 10;
        result += (int32_t)entry;
        if(++rg == size)
            end = 1;


    }
	mTBOX_PRINT((kTBOX_LF, "\n"));

    return(result);
}




/**
	* @brief This methods asks user a character with a txt prompt message
	*/
unsigned char TestManager_GetChar(char *txt)
{
    int8_t          entry = 0;
    int8_t          rg;
    unsigned char   result;

    if(txt != NULL)
	{
        mTBOX_PRINT((kTBOX_LF, "\n%s", txt));
	}

    rg  = 0;

    TKEL_DelayTask(ms2tick(100));
    mTBOX_GETCHAR(((char *)&result));
    mTBOX_PRINT((kTBOX_LF, "%c", entry));
	mTBOX_PRINT((kTBOX_LF, "\n"));

    return(result);
}

