/** \file NVM_test_u.c
 *  \brief Unitary Tests for the NVM layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_nvm.h"

#include "TestManager.h"
#include "nvmtests.h"
/*#include "tdal_common.h"*/

#ifdef CIPLUSVALIDATOR
#include "glue4tests.h"
#else
#include "testhelper.h"
#endif

void TestUnitary_NvmInitTerm(void);
void TestUnitary_NvmOpenClose(void);
void TestUnitary_NvmReadBadAddress(void);
void TestUnitary_NvmWriteBadAddress(void);

/******************************************************
 * 				Defines								  *
 ******************************************************/

/******************************************************
 * 				Local variables						  *
 ******************************************************/

/******************************************************
 *              NVM test tree                       *
 ******************************************************/

static tTestNode gTestUInitTerm = {
	"TestUInitTerm",
	"test TDAL_NVM_Init() and TDAL_NVM_Term() functions",
	0, /* not interactive */
	TestUnitary_NvmInitTerm,
	NULL
};

static tTestNode gTestUOpenClose = {
	"TestUOpenClose",
	"test TDAL_NVM_Open() and TDAL_NVM_Close() functions",
	0, /* not interactive */
	TestUnitary_NvmOpenClose,
	NULL
};

static tTestNode gTestNvmReadBadAddress = {
	"TestNvmReadBadAddress",
	"test read with bad address",
	0, /* not interactive */
	TestUnitary_NvmReadBadAddress,
	NULL
};

static tTestNode gTestNvmWriteBadAddress = {
	"TestNvmWriteBadAddress",
	"test write with bad address",
	0, /* not interactive */
	TestUnitary_NvmWriteBadAddress,
	NULL
};

tTestNode* gNVMUnitaryTestNodeList[] = {
	&gTestUInitTerm,
	&gTestUOpenClose,
	&gTestNvmReadBadAddress,
	&gTestNvmWriteBadAddress,
	NULL
};

tTestNode gTDAL_NVM_UnitaryTestSuite = {
	"TDAL_NVM Unitary Tests",
	"test the TDAL_NVM function input parameters",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gNVMUnitaryTestNodeList
};

/******************************************************
 * 				Unitary tests 						  *
 ******************************************************/

/*! \fn void TestUnitary_NvmInitTerm( void )
 *  \brief Test TDAL_NVM_Init() and TDAL_NVM_Terminate() functions
 \verbatim
 Standard Init and Terminate
 Twice Init and Terminate
 \endverbatim
 */

void TestUnitary_NvmInitTerm()
{
#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif
	/* Standard Init and Terminate */
	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

	/* Twice Init and twice Terminate */
	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "1st NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NOT_DONE, "2nd NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NOT_DONE, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestUnitary_NvmOpenClose( void )
 *  \brief Test TDAL_NVM_Open() and TDAL_NVM_Close() functions
 \verbatim
 Standard Open and Close
 Twice Open and Close
 Open and Term and re-open
 \endverbatim
 */

void TestUnitary_NvmOpenClose()
{
#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );

	/* Standard Open and Close */
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );
	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );

	/* Twice Open and twice Close */
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NOT_DONE, "NVM open" );
	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );
	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NOT_DONE, "NVM close" );

	/* Open and Term and re-open */
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );
	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );

	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestUnitary_NvmReadBadAddress( void )
 *  \brief Test that TDAL_NVM_Read() returns bad parameter
 */
void TestUnitary_NvmReadBadAddress(void)
{
	uint8_t	Buffer[1024];
	uint32_t aBadAddress = 0xffffffff;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );
	TestManager_AssertEqual( TDAL_NVM_Read( aBadAddress, 12, &Buffer[0]), 0, "Read on bad address returns bad param" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestUnitary_NvmWriteBadAddress( void )
 *  \brief Test that TDAL_NVM_Write() returns bad parameter
 */
void TestUnitary_NvmWriteBadAddress(void)
{
	uint8_t	Buffer[1024];
	uint32_t aBadAddress = 0xffffffff;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Write( aBadAddress, 12, &Buffer[0]), 0,
		"Write on bad address returns bad param" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}
