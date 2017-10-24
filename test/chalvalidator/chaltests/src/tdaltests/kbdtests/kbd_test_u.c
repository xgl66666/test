/** \file kbd_test_u.c
 *  \brief Unitary Tests for the KBD layer of the TDAL API
 *  \author Arnaud Viala
 *  \date 26/02/07
 *  \version 1.0
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_kbd.h"

#include "TestManager.h"
#include "testhelper.h"
#include "tdal_common.h"

void TestUnitary_KeyboardInitTerm(void);
void TestUnitary_KeyboardRegistration(void);

/******************************************************
 * 				Defines								  *
 ******************************************************/

/******************************************************
 * 				Local variables						  *
 ******************************************************/

/******************************************************
 *              keyboard test tree                    *
 ******************************************************/

static tTestNode gTestUInitTerm = {
	"TestUInitTerm",
	"test TDAL_KBD_Init() and TDAL_KBD_Term() functions",
	0, /* not interactive */
	TestUnitary_KeyboardInitTerm,
	NULL
};

static tTestNode gTestKeyboardRegistration = {
	"TestKeyboardRegistration",
	"test register and unregister functions",
	0, /* not interactive */
	TestUnitary_KeyboardRegistration,
	NULL
};

tTestNode* gKBDUnitaryTestNodeList[] = {
	&gTestUInitTerm,
	&gTestKeyboardRegistration,
	NULL
};

tTestNode gTDAL_KBD_UnitaryTestSuite = {
	"TDAL_KBD Unitary Tests",
	"test the TDAL_KBD function input parameters",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gKBDUnitaryTestNodeList
};

/******************************************************
 * 				Unitary tests 						  *
 ******************************************************/

/*! \fn void TestUnitary_KeyboardInitTerm( void )
 *  \brief Test TDAL_KBD_Init() and TDAL_KBD_Terminate() functions
 \verbatim
 Standard Init and Terminate
 Twice Init and Terminate
 \endverbatim
 */

void TestUnitary_KeyboardInitTerm()
{
	/* Standard Init and Terminate */
	TestManager_AssertEqual( TDAL_KBD_Init(), eTDAL_KBD_NO_ERROR, "KBD init" );
	TestManager_AssertEqual( TDAL_KBD_Term(), eTDAL_KBD_NO_ERROR, "KBD terminate" );

	/* Twice Init and Terminate */
	TestManager_AssertEqual( TDAL_KBD_Init(), eTDAL_KBD_NO_ERROR, "1st KBD init" );
	TestManager_AssertEqual( TDAL_KBD_Init(), eTDAL_KBD_NO_ERROR, "2nd KBD init" );
	TestManager_AssertEqual( TDAL_KBD_Term(), eTDAL_KBD_NO_ERROR, "1st KBD terminate" );
	TestManager_AssertEqual( TDAL_KBD_Term(), eTDAL_KBD_NO_ERROR, "2nd KBD terminate" );
}


/*! \fn void p_TestUnitary_KeyboardCallback( keycode , keystatus )
 *  \brief The callback called on each key event
 */
void p_TestUnitary_KeyboardCallback(
    tTDAL_KBD_KeyCode KeyCode,
    tTDAL_KBD_KeyStatus KeyStatus)
{
	return;
}


/*! \fn void TestUnitary_KeyboardRegistration( void )
 *  \brief Test TDAL_KBD_Register() and TDAL_KBD_UnRegister() functions
 */
void TestUnitary_KeyboardRegistration(void)
{
	tTDAL_KBD_ClientId  ClientId1;
    tTDAL_KBD_ClientId  ClientId2;

    /* Standard Init */
	TestManager_AssertEqual( TDAL_KBD_Init(), eTDAL_KBD_NO_ERROR, "TDAL_KBD_Init()" );


    /* Register first client */
    ClientId1 = TDAL_KBD_Register( p_TestUnitary_KeyboardCallback );
    TestManager_AssertEqual( ClientId1, 0, "TDAL_KBD_Register( 1st client )" );

    /* Register 2nd client */
    ClientId2 = TDAL_KBD_Register( p_TestUnitary_KeyboardCallback );
    TestManager_AssertEqual( ClientId2, kTDAL_KBD_NO_ID, "TDAL_KBD_Register( 2nd client )" );


    /* Unregister first client */
	TestManager_AssertEqual( TDAL_KBD_UnRegister(ClientId1), eTDAL_KBD_NO_ERROR, "TDAL_KBD_UnRegister( 1st client )" );

    /* Unregister 2nd client */
	TestManager_AssertEqual( TDAL_KBD_UnRegister(ClientId1), eTDAL_KBD_ERROR, "TDAL_KBD_UnRegister( 2nd client )" );


	/* Standard Terminate */
	TestManager_AssertEqual( TDAL_KBD_Term(), eTDAL_KBD_NO_ERROR, "TDAL_KBD_Term()" );
}



