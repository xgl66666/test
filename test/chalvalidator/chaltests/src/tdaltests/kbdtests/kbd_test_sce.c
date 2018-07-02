/** \file kbd_test_sce.c
 *  \brief Scenario Tests for the KBD layer of the TDAL API
 *  \author Arnaud Viala
 *  \date 26/02/07
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_kbd.h"

#include "TestManager.h"
#include "kbdtests.h"


/******************************************************
 *              Defines                               *
 ******************************************************/
#define kTDAL_KBD_TEST_DETECT_KEY_DURATION_SECONDS  20



void TestSce_DetectKeys(void);

/******************************************************
 *              KBD test tree                         *
 ******************************************************/


static tTestNode gTestScenarioDetectKeys = {
	"TestScenarioDetectKeys",
	"Detext keys event",
	0,
	TestSce_DetectKeys,
	NULL
};


tTestNode* gKBDScenarioTestNodeList[] = {
	&gTestScenarioDetectKeys,
	NULL
};

tTestNode gTDAL_KBD_ScenarioTestSuite = {
	"TDAL_KBD Scenario Tests",
	"test TDAL_KBD scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gKBDScenarioTestNodeList
};

/******************************************************
 *              Scenario tests                        *
 ******************************************************/

/*! \fn void p_TestUnitary_KeyboardCallback( keycode , keystatus )
 *  \brief The callback called on each key event
 */
uint32_t        p_TestSce_KeyboardNumberOfKeyEvent = 0;
void p_TestSce_KeyboardCallback(
    tTDAL_KBD_KeyCode KeyCode,
    tTDAL_KBD_KeyStatus KeyStatus)
{
    char    status=0;
    if ( KeyStatus == eTDAL_KBD_KEY_PRESSED )           status = 'P';
    else if ( KeyStatus == eTDAL_KBD_KEY_RELEASED )     status = 'R';
    else if ( KeyStatus == eTDAL_KBD_KEY_HELD_DOWN )    status = 'H';

    mTBOX_PRINT((kTBOX_LF,"KEY EVENT : Code : %02d=0x%02x - Status : %c \n", KeyCode, KeyCode, status));
    p_TestSce_KeyboardNumberOfKeyEvent++;
    return;
}


/*! \fn void TestSce_DetectKeys( void )
 *  \brief detect key events
 */
void TestSce_DetectKeys(void)
{
	tTDAL_KBD_ClientId  ClientId;

    /* Standard Init */
	TestManager_AssertEqual( TDAL_KBD_Init(), eTDAL_KBD_NO_ERROR, "TDAL_KBD_Init()" );


    /* Register first client */
    ClientId = TDAL_KBD_Register( p_TestSce_KeyboardCallback );
    TestManager_AssertEqual( ClientId, 0, "TDAL_KBD_Register( client )" );




#if 0
    mTBOX_PRINT((kTBOX_LF,"You have %d seconds to press some keys\n", kTDAL_KBD_TEST_DETECT_KEY_DURATION_SECONDS));
	TKEL_DelayTask( ms2tick(kTDAL_KBD_TEST_DETECT_KEY_DURATION_SECONDS*1000) );
#else
	TestManager_AskForChecking("press 1 to stop the test");
#endif
    //printf("p_TestSce_KeyboardNumberOfKeyEvent=%d\n", p_TestSce_KeyboardNumberOfKeyEvent);
	TestManager_AssertGreater( p_TestSce_KeyboardNumberOfKeyEvent , 0 , "Count number of key pressed" );




    /* Unregister first client */
	TestManager_AssertEqual( TDAL_KBD_UnRegister(ClientId), eTDAL_KBD_NO_ERROR, "TDAL_KBD_UnRegister( client )" );


	/* Standard Terminate */
	TestManager_AssertEqual( TDAL_KBD_Term(), eTDAL_KBD_NO_ERROR, "TDAL_KBD_Term()" );
}


