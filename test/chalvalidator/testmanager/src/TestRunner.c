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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crules.h"

#include "TestManager.h"
#include "tbox.h"
#ifdef CIPLUSVALIDATOR
#include "glue4tests.h"
#endif

#define VERSION_STRING "1.0.0 alpha"

extern void ClientTestSetup(void);
extern void TDAL_Init(void);

typedef enum
{
    eTEST_MANAGER_MODULE = 250
} tTestManager_ModuleTraceID;

int gWalkIndexStack[256];
tTestNode* gWalkTestNodeStack[256];
int gWalkTopStack = 0;
int gNodeCount = 0;
int gWithResult = 0;

/* Options */
int gOptionTestSuiteOnly = 0;
int gOptionOKOnly = 0;
int gOptionNotExecutedOnly = 0;
int gOptionErrorOnly = 0;

const char* sNodeNameToFind;
tTestNode* gNodeFound;

tTestNode* gRootTestNode = &gTestRoot;

typedef void (*WalkTreeCallBack)(tTestNode* aNode, int *pbKeepWalking);

void RecursWalkInTree( tTestNode* aTestRoot,
        WalkTreeCallBack aPrefixedCallback,
        WalkTreeCallBack aPostfixedCallback,
        int* abKeepWalking );

void DisplayTreeCallback(tTestNode* aTestNode, int* bKeepWalking);
void ExecuteTreeCallback(tTestNode* aTestNode, int* bKeepWalking);
void InitTestResultVector(void);
int GetNbSon( tTestNode* aCurrentNode );

/* some usefull accessors */
int IsTestCase(tTestNode* aN) { return aN->mTestNodeVector == NULL; }
int IsTestSuite(tTestNode* aN) { return aN->mTestNodeVector != NULL; }

int IsTestExecuted(tTestResult* aTestResult) { return ( aTestResult->mNbExecution != 0 ); }
int IsTestInError(tTestResult* aTestResult) { return ( aTestResult->mTheFailureTotal != 0); }
int IsTestOK(tTestResult* aTestResult) { return ( aTestResult->mTheFailureTotal == 0); }

/**
 * @brief This method walks in the node tree
 *   Is executes callback (if not null), in prefixed and postfixed position
 */
void WalkInTree( tTestNode* aTestRoot,
        WalkTreeCallBack aPrefixedCallback,
        WalkTreeCallBack aPostfixedCallback,
        int* pbKeepWalking )
{
    gWalkTopStack = 1;
    gWalkIndexStack[0] = 0;
    gWalkTestNodeStack[0] = aTestRoot;
    RecursWalkInTree( aTestRoot, aPrefixedCallback, aPostfixedCallback, pbKeepWalking );
}

/**
 * @brief This method walks recursively in the node tree, calling the callback
 * in a prefix way.
 */
void RecursWalkInTree( tTestNode* aTestRoot,
        WalkTreeCallBack aPrefixedCallback,
        WalkTreeCallBack aPostfixedCallback,
        int* abKeepWalking )
{
    int bKeepWalking = 1;

    /* execute prefixed call (depth first search) */
    if( aPrefixedCallback )
    {
        aPrefixedCallback( aTestRoot, &bKeepWalking );
    }

    if( bKeepWalking )
    {
        if( aTestRoot->mTestNodeVector )
        {
            int nIndex=0;
            tTestNode* aTestNode = aTestRoot->mTestNodeVector[nIndex];
            gWalkTopStack++;
            gWalkIndexStack[gWalkTopStack-1] = 0;
            gWalkTestNodeStack[gWalkTopStack-1] = aTestNode;
            while( aTestNode )
            {
                RecursWalkInTree( aTestNode, aPrefixedCallback, aPostfixedCallback, &bKeepWalking );
                if( !bKeepWalking )
                    break;
                nIndex++;
                aTestNode = aTestRoot->mTestNodeVector[nIndex];
                gWalkIndexStack[gWalkTopStack-1]++;
                gWalkTestNodeStack[gWalkTopStack-1] = aTestNode;
            }
            gWalkTopStack--;
        }
    }

    /* execute postfixed call (depth first search) */
    if( aPostfixedCallback )
    {
        aPostfixedCallback( aTestRoot, &bKeepWalking );
    }

    if( !bKeepWalking )
    {
        /* tell caller to stop walking */
        *abKeepWalking = 0;
    }
}

/**
 * @brief This method is a callback for the WalkInTree function, to count node.
 */
void NodeCounterCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    gNodeCount++;
}

/**
 * @brief This method counts test nodes
 */
TESTMANAGER_err CountTestNodes()
{
    int bKeepWalking = 1;
    gNodeCount = 0;
    WalkInTree( &gTestRoot, NodeCounterCallback, NULL, &bKeepWalking );
    return TESTMANAGER_NO_ERR;
}

/**
 * @brief This method initializes a test result
 */
void InitTestResult( tTestResult* aTestResult, tTestNode* aTestNode )
{
    aTestResult->mTestNode = aTestNode;
    aTestResult->mTheOkTotal = 0;
    aTestResult->mTheFailureTotal = 0;
    aTestResult->mNbExecution = 0;
    aTestResult->mNbSonFullyExecuted = 0;
}

/**
 * @brief This method is a callback for the WalkInTree function, to found a given test node.
 */
void InitNodeTestResultCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    tTestResult* aCurrentTestResult = NULL;
    aCurrentTestResult = &gTestManager.mTestResultVector[gNodeCount];

    InitTestResult( aCurrentTestResult, aTestNode );

    gNodeCount++;
}

/**
 * @brief This method walks recursively into the tree, to setup
 *   test results.
 */
TESTMANAGER_err InitializeTestResultValues()
{
    int bKeepWalking = 1;
    gNodeCount = 0;
    WalkInTree( &gTestRoot, InitNodeTestResultCallback, NULL, &bKeepWalking );
    return TESTMANAGER_NO_ERR;
}

/**
 * @brief This method finds a test result for a given test node.
 * Algorithm is a simple linear search.
 */
TESTMANAGER_err FindTestResult( tTestNode* aTestNode, tTestResult** aTestResult )
{
    int i=0;
    tTestResult* aCurrentTestResult = NULL;

    for( i=0; i < gNodeCount; i++ )
    {
        aCurrentTestResult = &gTestManager.mTestResultVector[i];
        if( aCurrentTestResult->mTestNode == aTestNode )
        {
            *aTestResult = aCurrentTestResult;
            return TESTMANAGER_NO_ERR;
        }
    }
    return TESTMANAGER_NOT_DONE;
}

/**
 * @brief This method initializes a vector of test results
 */
void InitTestResultVector()
{
    /* First, get the total nuber of test nodes */
    CountTestNodes();

    /* then allocate a vector of test result */
    TKEL_Malloc( gNodeCount * sizeof(tTestResult), (void**)&gTestManager.mTestResultVector );

    InitializeTestResultValues();
}

/**
 * @brief This method returns 1 if node should be displayed.
 */
int IsNodeDisplayed( tTestNode* aTestNode, tTestResult* aTestResult )
{
    int bDisplay = 0;

    bDisplay = ( !gOptionTestSuiteOnly || IsTestSuite(aTestNode) );
    if( IsTestCase(aTestNode) )
    {
        bDisplay = bDisplay && ( !gOptionOKOnly ||
                (IsTestOK(aTestResult) && IsTestExecuted(aTestResult)) );
        bDisplay = bDisplay && ( !gOptionErrorOnly || IsTestInError(aTestResult) );
        bDisplay = bDisplay && ( !gOptionNotExecutedOnly || !IsTestExecuted(aTestResult) );
    }

    return bDisplay;
}

/**
 * @brief This method is the prefixed callback for the WalkInTree function, to display node tree.
 */
void DisplayTreeCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    char sNodeDescription[1024];
    char sTab[256];
    char sNodeNumber[1024];
    char sANumber[256];
    char sResultStatus[256];
    char sOkFailedStatus[256];
    int i = 0;
    int iN = 0;
    tTestResult* aCurrentTestResult = NULL;

    sNodeDescription[0] = 0;
    sNodeNumber[0] = 0;
    sANumber[0] = 0;

    *bKeepWalking = 1;
    while( i < gWalkTopStack )
    {
        sTab[i] = ' ';
        sprintf( sANumber, "%d", gWalkIndexStack[i]);
        strcpy( &sNodeNumber[iN], sANumber );
        i++;
        iN += strlen(sANumber);
        if( i < gWalkTopStack )
        {
            sNodeNumber[iN] = ' ';
            iN++;
        }
    }
    sTab[i] = 0;

    FindTestResult( aTestNode, &aCurrentTestResult );

    sResultStatus[0] = 0;
    if( gWithResult )
    {
        /* Build the OK/Failed status */
        if( aCurrentTestResult->mTheFailureTotal == 0 )
        {
            sprintf( sOkFailedStatus, "OK (%ld)", aCurrentTestResult->mTheOkTotal );
        }
        else
        {
            sprintf( sOkFailedStatus, "OK (%ld) FAILED (%ld)",
                    aCurrentTestResult->mTheOkTotal,
                    aCurrentTestResult->mTheFailureTotal );
        }

        if( IsTestSuite(aTestNode) )
        {
            /* node is a Test Suite */
            if( !IsTestExecuted(aCurrentTestResult) )
            {
                sprintf( sResultStatus, "NOT EXECUTED" );
            }
            else
            {
                int aPartiallyExecuted = (aCurrentTestResult->mNbSonFullyExecuted != GetNbSon(aTestNode) );

                sprintf( sResultStatus, "%s. %s",
                        aPartiallyExecuted ? "PARTIALLY EXECUTED" : "FULLY EXECUTED", sOkFailedStatus );
            }
        }
        else
        {
            /* Test Case */
            if( IsTestExecuted(aCurrentTestResult) )
            {
                sprintf( sResultStatus, "%s", sOkFailedStatus );
            }
            else
            {
                sprintf( sResultStatus, "NOT EXECUTED" );
            }
        }
    }

    if( IsNodeDisplayed(aTestNode, aCurrentTestResult) )
    {
        if( gWithResult )
        {
            mTBOX_PRINT((kTBOX_LF, "%s%s (%s) : %s\n", sTab, aTestNode->msName, sNodeNumber, sResultStatus ));
        }
        else
        {
            mTBOX_PRINT((kTBOX_LF, "%s%s (%s)\n", sTab, aTestNode->msName, sNodeNumber));
        }
    }
}

/**
 * @brief This method execute is a callback that execute a node.
 *   If function requires interactivity, and mode is not interactive,
 *   then function is not executed.
 *   If test fails, and mode "stopatfirsterror" is activated, then,
 *   it stops execution of next tests.
 */
void ExecuteTreeCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    *bKeepWalking = 1;
    if( aTestNode->mExecuteFunction )
    {
        if( !gTestManager.mInteractiveMode && aTestNode->mbIsFunctionalyInteractive )
        {
            mTBOX_PRINT((kTBOX_LF, "TESTCASE '%s' NOT EXECUTED (requires interactivity)\n", aTestNode->msName));
        }
        else
        {
            long aFailureTotal = 0;

            mTBOX_PRINT(( kTBOX_LF, "- - - Executing '%s' - - -\n", aTestNode->msName ));
            mTBOX_PRINT(( kTBOX_LF, "PURPOSE: '%s'\n", aTestNode->msPurpose ));

            /* find the corresponding test result, and set it to the test manager */
            gTestManager.mCurrentTestResult = NULL;
            FindTestResult( aTestNode, &gTestManager.mCurrentTestResult );

            if( gTestManager.mCurrentTestResult == NULL ) /* safe test: should not happen */
            {
                mTBOX_PRINT(( kTBOX_LF, "ERROR TEST RUNNER: NO TEST RESULT\n" ));
                return;
            }

            /* memorize current failure total */
            aFailureTotal = gTestManager.mCurrentTestResult->mTheFailureTotal;

            /* execute test*/
            aTestNode->mExecuteFunction();

            mTBOX_PRINT(( kTBOX_LF, "- - - Done - - -\n", aTestNode->msName ));
            mTBOX_PRINT(( kTBOX_LF, "\n" ));
            gTestManager.mCurrentTestResult->mNbExecution++;

            /* if test case has failed, and "stop at first error" mode is set, stops */
            if( gTestManager.mStopAtFirstErrorMode )
            {
                if( aFailureTotal != gTestManager.mCurrentTestResult->mTheFailureTotal )
                {
                    *bKeepWalking = 0; /* stop executing more tests */
                    mTBOX_PRINT(( kTBOX_LF, "TESTCASE '%s' has failed\n", aTestNode->msName ));
                }
            }
        }
    }
    else
    {
        ; /* do nothing: it's a test suite */
    }
}

/**
 * @brief This method is a callback for the WalkInTree function, to found a given test node.
 */
void NodeFinderCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    if( strcmp( aTestNode->msName, sNodeNameToFind ) == 0 )
    {
        *bKeepWalking = 0;
        gNodeFound = aTestNode;
    }
}

/**
 * @brief This method displays a test tree
 */
TESTMANAGER_err DisplayTestTree( tTestNode* aTestRoot, int bWithResult )
{
    int bKeepWalking = 1;
    char sTitleOption[1024];
    gWithResult = bWithResult;

    sTitleOption[0] = 0;
    if( gOptionTestSuiteOnly )
    {
        strcat( sTitleOption, "TestSuite");
    }

    if( gOptionOKOnly )
    {
        if( strlen(sTitleOption) ) strcat( sTitleOption, " and ");
        strcat(sTitleOption, "Test OK");
    }

    if( gOptionNotExecutedOnly )
    {
        if( strlen(sTitleOption) ) strcat( sTitleOption, " and ");
        strcat(sTitleOption, "Test not executed");
    }

    if( gOptionErrorOnly )
    {
        if( strlen(sTitleOption) ) strcat( sTitleOption, " and ");
        strcat(sTitleOption, "Test in error");
    }
    if( strlen(sTitleOption) ) strcat( sTitleOption, " only.");

    mTBOX_PRINT((kTBOX_LF, "Display from %s. %s\n", aTestRoot->msName, sTitleOption));

    WalkInTree( aTestRoot, DisplayTreeCallback, NULL, &bKeepWalking );
    return TESTMANAGER_NO_ERR;
}

/**
 * @brief AggregateInFatherTestSuite aggregates in father's test suite result,
 * after node itself has been computed.
 */
void AggregateInFatherTestSuite( tTestResult* aTestResult, tTestNode* aTestSuite )
{
    tTestResult* aTestSuiteResult=NULL;
    FindTestResult( aTestSuite, &aTestSuiteResult );

    aTestSuiteResult->mTheOkTotal += aTestResult->mTheOkTotal;
    aTestSuiteResult->mTheFailureTotal += aTestResult->mTheFailureTotal;
    if( aTestResult->mNbExecution )
    {
        aTestSuiteResult->mNbExecution = 1; /* only boolean meaning in a test suite */
    }

    /* If son is testcase, and has been executed, OR */
    /* If son is testsuite and has been fully executed */
    if( (IsTestCase(aTestResult->mTestNode) && aTestResult->mNbExecution ) ||
            (IsTestSuite(aTestResult->mTestNode) && (GetNbSon(aTestResult->mTestNode) == aTestResult->mNbSonFullyExecuted)) )
    {
        /* Then, consider that this testsuite has one more son executed */
        aTestSuiteResult->mNbSonFullyExecuted++;
    }

    if( IsTestCase(aTestSuite) )
    {
        mTBOX_PRINT((kTBOX_LF, "ERROR SHOULD BE A TESTSUITE\n" ));
    }
}

/**
 * @brief This method is the postfixed callback to aggregate test suite results.
 *  It aggregates to father.
 */
void PostfixedAggregateTestSuiteResultCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    if( gWalkTopStack > 1 )
    {
        tTestNode* aFather=NULL;
        tTestResult* aTestResult=NULL;
        FindTestResult( aTestNode, &aTestResult );

        aFather = gWalkTestNodeStack[ gWalkTopStack-2 ];
        AggregateInFatherTestSuite( aTestResult, aFather );
    }
    else
    {
        ; /* do nothing : it's the root itself */
    }
}

/**
 * @brief This method is the prefixed callback to aggregate test suite results.
 *  It resets test suite results
 */
void ResetTestSuiteResultCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    tTestResult* aTestResult=NULL;
    FindTestResult( aTestNode, &aTestResult );

    if( IsTestSuite(aTestNode) )
    {
        /* if it's a test suite, reset values */
        InitTestResult( aTestResult, aTestNode );
    }
}

/**
 * @brief This method is the prefixed callback to reset test results.
 */
void ResetTestResultCallback(tTestNode* aTestNode, int* bKeepWalking)
{
    tTestResult* aTestResult=NULL;
    FindTestResult( aTestNode, &aTestResult );
    InitTestResult( aTestResult, aTestNode );
}

/**
 * @brief This method aggregates test suite results
 */
TESTMANAGER_err AggregateTestSuiteResult( tTestNode* aTestRoot )
{
    int bKeepWalking = 1;
    WalkInTree( aTestRoot,
            ResetTestSuiteResultCallback,
            PostfixedAggregateTestSuiteResultCallback,
            &bKeepWalking );
    return TESTMANAGER_NO_ERR;
}

/**
 * @brief This method resets test results
 */
TESTMANAGER_err ResetTestResult( tTestNode* aTestRoot )
{
    int bKeepWalking = 1;

    mTBOX_PRINT((kTBOX_LF, "Reset Test Result '%s'\n", aTestRoot->msName ));

    WalkInTree( aTestRoot,
            ResetTestResultCallback,
            NULL,
            &bKeepWalking );

    /* After reset of some nodes, aggregate test suite result for the whole tree */
    AggregateTestSuiteResult( &gTestRoot );

    return TESTMANAGER_NO_ERR;
}

/**
 * @brief This method execute a test tree
 */
TESTMANAGER_err ExecuteTestTree( tTestNode* aTestRoot )
{
    int bKeepWalking = 1;
    tTestResult* aCurrentTestResult = NULL;

    mTBOX_PRINT((kTBOX_LF, "EXECUTE TEST '%s'\n", aTestRoot->msName ));

    WalkInTree( aTestRoot, ExecuteTreeCallback, NULL, &bKeepWalking );

    /* Display final result */
    FindTestResult( aTestRoot, &aCurrentTestResult );

    if( aCurrentTestResult == NULL ) /* safe test: should not happen */
    {
        mTBOX_PRINT((kTBOX_LF, "DISPLAY ERROR NO TEST RESULT"));
        return TESTMANAGER_NO_ERR;
    }

    /* After execution, aggregate test suite result for the whole tree */
    AggregateTestSuiteResult( &gTestRoot );

    mTBOX_PRINT((kTBOX_LF, "Number of OK: \t%d\n", aCurrentTestResult->mTheOkTotal ));
    mTBOX_PRINT((kTBOX_LF, "Number of Failure: \t%d\n", aCurrentTestResult->mTheFailureTotal ));

    return TESTMANAGER_NO_ERR;
}

/**
 * @brief This method returns number of son
 */
int GetNbSon( tTestNode* aCurrentNode )
{
    if( IsTestSuite(aCurrentNode) )
    {
        int nIndex = 0;
        while( aCurrentNode->mTestNodeVector[nIndex] )
        {
            nIndex++;
        }
        return nIndex;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief This method access a given son of a test node.
 *  It checks first that index is correct.
 */
tTestNode* GetSon( tTestNode* aCurrentNode, int aNodeIndex )
{
    if( IsTestSuite(aCurrentNode) )
    {
        int nNbSon = GetNbSon( aCurrentNode );
        if( aNodeIndex >= 0 && aNodeIndex < nNbSon )
        {
            return aCurrentNode->mTestNodeVector[aNodeIndex];
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief This method finds test node from a series of number. for instance, 0 0 0
 */
int FindNodeFromNumber( const char* aInput, tTestNode** apTestNode )
{
    tTestNode* aCurrentNode = gRootTestNode;
    char* sNextSpace;
    if( aInput && aInput[0] != '0' )
    {
        aCurrentNode = NULL;
    }
    else
    {
        sNextSpace = strchr( aInput+1, ' ' );
        while( sNextSpace )
        {
            int aNodeIndex = atoi( sNextSpace+1 );
            aCurrentNode = GetSon( aCurrentNode, aNodeIndex );
            if( !aCurrentNode )
            {
                break;
            }
            sNextSpace = strchr( sNextSpace+1, ' ' );
        }
    }

    if( aCurrentNode )
    {
        *apTestNode = aCurrentNode;
        return 1;
    }
    else
    {
        *apTestNode = NULL;
        return 0;
    }
}

/**
 * @brief This method look for a node, from its name.
 */
int FindNode( const char* aCommand, tTestNode** aNode )
{
    int bKeepWalking = 1;
    *aNode = NULL;

    /* set the name to find */
    sNodeNameToFind = aCommand;
    gNodeFound = NULL;

    /* walk on tree to find it */
    WalkInTree( gRootTestNode, NodeFinderCallback, NULL, &bKeepWalking );

    if( !bKeepWalking )
    {
        /* a test node has been found */
        *aNode = gNodeFound;
        return 1;
    }

    return 0;
}

/**
 * @brief This method returns the number of argument in the input/
 *  arguments are separated with space.
 */
int NbArg(const char* aInput)
{
    const char* aSpace;
    if( strlen(aInput) == 0 )
    {
        return 0;
    }
    else
    {
        int n = 1;
        aSpace = strchr(aInput, ' ');
        while( aSpace )
        {
            n++;
            aSpace++;
            aSpace = strchr(aSpace, ' ');
        }
        return n;
    }
}

/**
 * @brief Set interactive mode according to the command.
 */
TESTMANAGER_err SetInteractiveMode( const char* aInput )
{
    char* sArg1;
    if( NbArg(aInput) == 2 )
    {
        sArg1 = strchr( aInput, ' ' );
        if( sArg1 )
        {
            sArg1++;
            if( (strncmp( sArg1, "on", 2 ) == 0) ||
                    (strncmp( sArg1, "1", 1 ) == 0) )
            {
                gTestManager.mInteractiveMode = 1;
            }
            else if( (strncmp( sArg1, "off", 3 ) == 0) ||
                    (strncmp( sArg1, "0", 1 ) == 0) )
            {
                gTestManager.mInteractiveMode = 0;
            }
        }
    }

    mTBOX_PRINT((kTBOX_LF, "Interactive mode is %s\n", (gTestManager.mInteractiveMode ? "on" : "off" ) ));
    return TESTMANAGER_NO_ERR;
}

/**
 * @brief Set StopAtFirstError mode, according to the command.
 */
TESTMANAGER_err SetStopAtFirstErrorMode( const char* aInput )
{
    char* sArg1;
    if( NbArg(aInput) == 2 )
    {
        sArg1 = strchr( aInput, ' ' );
        if( sArg1 )
        {
            sArg1++;
            if( (strncmp( sArg1, "on", 2 ) == 0) ||
                    (strncmp( sArg1, "1", 1 ) == 0) )
            {
                gTestManager.mStopAtFirstErrorMode = 1;
            }
            else if( (strncmp( sArg1, "off", 3 ) == 0) ||
                    (strncmp( sArg1, "0", 1 ) == 0) )
            {
                gTestManager.mStopAtFirstErrorMode = 0;
            }
        }
    }

    mTBOX_PRINT((kTBOX_LF, "Stop at first error mode is %s\n", (gTestManager.mStopAtFirstErrorMode ? "on" : "off" ) ));
    return TESTMANAGER_NO_ERR;
}

/**
 * @brief This method tests if first argument of the command line (aInput) is equal to s
 */
int FirstArgEquals( const char* aInput, const char* s )
{
    char* space = strchr( aInput, ' ' );
    if( space )
    {
        return (strncmp( aInput, s, strlen(s) ) == 0);
    }
    else
    {
        return (strcmp( aInput, s) == 0);
    }
}

/**
 * @brief This method reset options,
 */
void ResetOptions()
{
    gOptionTestSuiteOnly = 0;
    gOptionOKOnly = 0;
    gOptionErrorOnly = 0;
    gOptionNotExecutedOnly = 0;
}

/**
 * @brief This method set display options,
 * returning 1 if options valid.
 */
int SetOptions( const char *aSecondArg )
{
    const char* aStart = aSecondArg;
    while( aStart[0] != 0 && aStart[0] != ' ' )
    {
        switch( aStart[0] )
        {
        case '1':
            gOptionTestSuiteOnly = 1;
            break;
        case '2':
            gOptionOKOnly = 1;
            break;
        case '3':
            gOptionErrorOnly = 1;
            break;
        case '4':
            gOptionNotExecutedOnly = 1;
            break;
        default:
            ResetOptions();
            return 0;
        }
        aStart++;
    }

    return 1;
}

/**
 * @brief This method returns 1 if there is a second argument,
 * and returns it via the aSecondArg parameter
 */
int SecondArg( const char* aInput, char** aSecondArg )
{
    char* secondarg = strchr( aInput, ' ' );
    if( secondarg )
    {
        secondarg++;
        *aSecondArg = secondarg;
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Remove white spaces at end of string
 */
void stripend( char* aInput )
{
    int i = strlen(aInput);
    for( ; (i > 0) && (aInput[i-1] == ' '); i-- )
    {
        aInput[i-1] = 0; /* replace white space with 0 */
    }
}

/**
 * @brief This method interprets the command typed by the user
 */
int TestManager_InterpretCommand( const char* aInput )
{
    tTestNode* aTestNode=NULL;
    int bContinue=1;
    if( FirstArgEquals( aInput, "l" ) || FirstArgEquals( aInput, "list" ) || FirstArgEquals( aInput, "1" ) )
    {
        char* aSecondArg;
        tTestNode* aDisplayTestNode = gRootTestNode; /* default value */
        ResetOptions();
        if( SecondArg( aInput, &aSecondArg ) )
        {
            if( FindNode( aSecondArg, &aTestNode ) ||
                    FindNodeFromNumber( aSecondArg, &aTestNode) )
            {
                aDisplayTestNode = aTestNode;
            }
            else if( SetOptions( aSecondArg ) )
            {
                if( SecondArg( aSecondArg, &aSecondArg ) )
                {
                    if( FindNode( aSecondArg, &aTestNode ) ||
                            FindNodeFromNumber( aSecondArg, &aTestNode) )
                    {
                        aDisplayTestNode = aTestNode;
                    }
                }
            }
            else
            {
                mTBOX_PRINT((kTBOX_LF, "unknown test node or option\n" ));
            }
        }

        DisplayTestTree( aDisplayTestNode, 0 );
    }
    else if( FindNode( aInput, &aTestNode ) )
    {
        ExecuteTestTree( aTestNode );
    }
    else if( FirstArgEquals( aInput, "0" ) )
    {
        if( FindNodeFromNumber(aInput, &aTestNode) )
        {
            ExecuteTestTree( aTestNode );
        }
        else
        {
            mTBOX_PRINT((kTBOX_LF, "unknown test node\n" ));
        }
    }
    else if( FirstArgEquals( aInput, "interactive" ) ||
            FirstArgEquals( aInput, "i" ) ||
            FirstArgEquals( aInput, "2" ) )
    {
        SetInteractiveMode( aInput );
    }
    else if( FirstArgEquals( aInput, "cd" ) ||
            FirstArgEquals( aInput, "c" ) ||
            FirstArgEquals( aInput, "6" ) )
    {
        char* aSecondArg;
        if( SecondArg( aInput, &aSecondArg ) )
        {
            if( FindNode( aSecondArg, &aTestNode ) ||
                    FindNodeFromNumber( aSecondArg, &aTestNode) )
            {
                gRootTestNode = aTestNode;
                mTBOX_PRINT((kTBOX_LF, "test node '%s'\n", gRootTestNode->msName ));
            }
            else
            {
                mTBOX_PRINT((kTBOX_LF, "unknown test node\n" ));
            }
        }
        else
        {
            mTBOX_PRINT((kTBOX_LF, "back to root\n" ));
            gRootTestNode = &gTestRoot; /* default value */
        }
    }
    else if( FirstArgEquals( aInput, "stopatfirsterror") ||
            FirstArgEquals( aInput, "s" ) ||
            FirstArgEquals( aInput, "3" ) )
    {
        SetStopAtFirstErrorMode( aInput );
    }
    else if( FirstArgEquals( aInput, "reset" ) ||
            FirstArgEquals( aInput, "z" ) ||
            FirstArgEquals( aInput, "5" ) )
    {
        char* aSecondArg;
        aTestNode = gRootTestNode; /* default value */
        if( SecondArg( aInput, &aSecondArg ) )
        {
            if( FindNode( aSecondArg, &aTestNode ) ||
                    FindNodeFromNumber( aSecondArg, &aTestNode) )
            {
                ResetTestResult( aTestNode );
            }
            else
            {
                mTBOX_PRINT((kTBOX_LF, "unknown test node ppp\n" ));
            }
        }
    }
    else if( FirstArgEquals( aInput, "result" ) ||
            FirstArgEquals( aInput, "r" ) ||
            FirstArgEquals( aInput, "4" ) )
    {
        char* aSecondArg;
        tTestNode* aResultTestNode = gRootTestNode; /* default value */
        ResetOptions();
        if( SecondArg( aInput, &aSecondArg ) )
        {
            if( FindNode( aSecondArg, &aTestNode ) ||
                    FindNodeFromNumber( aSecondArg, &aTestNode) )
            {
                aResultTestNode = aTestNode;
            }
            else if( SetOptions( aSecondArg ) )
            {
                if( SecondArg( aSecondArg, &aSecondArg ) )
                {
                    if( FindNode( aSecondArg, &aTestNode ) ||
                            FindNodeFromNumber( aSecondArg, &aTestNode) )
                    {
                        aResultTestNode = aTestNode;
                    }
                }
            }
            else
            {
                mTBOX_PRINT((kTBOX_LF, "unknown test node or option\n" ));
            }
        }

        DisplayTestTree( aResultTestNode, 1 );
    }
    else if( (strcmp( aInput, "help" ) == 0) || ( strcmp( aInput, "?" ) == 0 ) || FirstArgEquals( aInput, "7" ) )
    {
        mTBOX_PRINT((kTBOX_LF, "\tNODE : execute node (name or number)\n"));
        mTBOX_PRINT((kTBOX_LF, "\tlist|l|1 [OPTIONS] [NODE] : list tests, from given node or root.\n"));
        mTBOX_PRINT((kTBOX_LF, "\tinteractive|i|2 [on|off|0|1] : set interactive mode on or off\n"));
        mTBOX_PRINT((kTBOX_LF, "\tstopatfirsterror|s|3 [on|off|0|1] : set stopatfirsterror mode on or off\n"));
        mTBOX_PRINT((kTBOX_LF, "\tresult|r|4 [OPTIONS] [NODE]: display result, from given node or root\n"));
        mTBOX_PRINT((kTBOX_LF, "\treset|z|5 [NODE]: reset result, of a given node or root\n"));
        mTBOX_PRINT((kTBOX_LF, "\tcd|c|6 [NODE]: change root to another node\n"));
        mTBOX_PRINT((kTBOX_LF, "\thelp|?|7 : display this help\n"));
        mTBOX_PRINT((kTBOX_LF, "\tquit|8 : quit testrunner\n"));
        mTBOX_PRINT((kTBOX_LF, "\t\n"));
        mTBOX_PRINT((kTBOX_LF, "\tOPTIONS: 1=TestSuite 2=OK 3=Err 4=NotExec\n"));
        mTBOX_PRINT((kTBOX_LF, "\n"));
    }
    else if( (strcmp( aInput, "quit" ) == 0) || FirstArgEquals( aInput, "8" ) )
    {
        bContinue = 0;
    }
    else
    {
        mTBOX_PRINT((kTBOX_LF, "Unknown command: '%s'\n", aInput));
    }
    return bContinue;
}

/**
 * @brief The main: a simple loop to catch command, and execute them.
 */
GLOBAL void * chalvalidator_main(void *argv)
{
    char pStrIn[1024];


#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    /**--------------------------**
     *     Client test setup      *
     **--------------------------**/
    ClientTestSetup();
    /**--------------------------**
     *     Init TDAL drivers      *
     * ( Init low level drivers ) *
     **--------------------------**/
    TDAL_Init();
#endif

    /**--------------------------**
     *     Init TKEL drivers      *
     **--------------------------**/
    TKEL_Init();

    /**--------------------------**
     *     Init test manager      *
     **--------------------------**/
    TestManager_Init();
    InitTestResultVector();
    mTBOX_PRINT((kTBOX_LF, "Test Runner\n"));
    mTBOX_PRINT((kTBOX_LF, "Version %s\n\n", VERSION_STRING));
    while(1)
    {
        mTBOX_PRINT((kTBOX_LF,"CMD>"));
        pStrIn[0] = 0; /* \bug This line is a workaround to avoid a mTBOX_GETSTRING bug : \
        when empty string, the 0 is not set at the end. */
#if 0
        sprintf(pStrIn, "0 1 3 1 3");
#else
        mTBOX_GETSTRING((pStrIn));
#endif
        stripend(pStrIn);
        if((pStrIn[0] == 0x00)||(pStrIn[0] == 0x20))
            continue;
        if( !TestManager_InterpretCommand(pStrIn) )
            break;
    }
	
	return NULL;
}
