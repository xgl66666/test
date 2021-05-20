/** \file fla_test_u.c
 *  \brief Unitary Tests for the FLA layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_fla.h"

#include "TestManager.h"
#include "flatests.h"
#include "testhelper.h"
/*#include "tdal_common.h"*/

void TestUnitary_FlashInitTerm(void);
void TestUnitary_FlashReadBadAddress(void);
void TestUnitary_FlashWriteBadAddress(void);
void TestUnitary_FlashEraseBadBlock(void);
void TestUnitary_TestSetPartition(void);

/******************************************************
 * 				Defines								  *
 ******************************************************/

/******************************************************
 * 				Local variables						  *
 ******************************************************/

/******************************************************
 *              Flash test tree                       *
 ******************************************************/

static tTestNode gTestUInitTerm = {
	"TestUInitTerm",
	"test TDAL_FLA_Init() and TDAL_FLA_Term() functions",
	0, /* not interactive */
	TestUnitary_FlashInitTerm,
	NULL
};

static tTestNode gTestFlashReadBadAddress = {
	"TestFlashReadBadAddress",
	"test read with bad address",
	0, /* not interactive */
	TestUnitary_FlashReadBadAddress,
	NULL
};

static tTestNode gTestFlashWriteBadAddress = {
	"TestFlashWriteBadAddress",
	"test write with bad address",
	0, /* not interactive */
	TestUnitary_FlashWriteBadAddress,
	NULL
};

static tTestNode gTestFlashEraseBadBlock = {
	"TestFlashEraseBadBlock",
	"test flash erase with a unaligned block address or size",
	0, /* not interactive */
	TestUnitary_FlashEraseBadBlock,
	NULL
};

static tTestNode gTestFlashSetPartition = {
	"TestFlashSetPartition",
	"test flash set partition",
	0, /* not interactive */
	TestUnitary_TestSetPartition,
	NULL
};

tTestNode* gFLAUnitaryTestNodeList[] = {
	&gTestUInitTerm,
	&gTestFlashReadBadAddress,
	&gTestFlashWriteBadAddress,
	&gTestFlashEraseBadBlock,
	&gTestFlashSetPartition,
	NULL
};

tTestNode gTDAL_FLA_UnitaryTestSuite = {
	"TDAL_FLA Unitary Tests",
	"test the TDAL_FLA function input parameters",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gFLAUnitaryTestNodeList
};

/******************************************************
 * 				Unitary tests 						  *
 ******************************************************/

/*! \fn void TestUnitary_FlashInitTerm( void )
 *  \brief Test TDAL_FLA_Init() and TDAL_FLA_Terminate() functions
 \verbatim
 Standard Init and Terminate
 Twice Init and Terminate
 \endverbatim
 */

void TestUnitary_FlashInitTerm()
{
	/* Standard Init and Terminate */
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );

	/* Twice Init and Terminate */
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "1st FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "2nd FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestUnitary_FlashReadBadAddress( void )
 *  \brief Test that TDAL_FLA_Read() returns bad parameter
 */
void TestUnitary_FlashReadBadAddress(void)
{
	uint8_t	Buffer[1024];
	uint32_t aBadAddress = 0xffffffff;
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBadAddress, &Buffer[0], 12), 0, "Read on bad address returns bad param" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestUnitary_FlashWriteBadAddress( void )
 *  \brief Test that TDAL_FLA_Write() returns bad parameter
 */
void TestUnitary_FlashWriteBadAddress(void)
{
	uint8_t	Buffer[1024];
	uint32_t aBadAddress = 0xffffffff;
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBadAddress, &Buffer[0], 12), 0,
		"Write on bad address returns bad param" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestUnitary_FlashEraseBadBlock( void )
 *  \brief Test that TestUnitary_FlashEraseBadBlock() returns bad parameter with bad address or size
 */
void TestUnitary_FlashEraseBadBlock(void)
{
	uint32_t aSizeOfBlock0 = 0;
	uint32_t aBlock0Adress;
	uint32_t aBadAddress;
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	aBlock0Adress = TDAL_FLA_GetBlockAddress(0);
	aBadAddress = aBlock0Adress + 1; /* block size is not 1! */
	aSizeOfBlock0 = TDAL_FLA_GetBlockSize(0);
	TestManager_AssertTrue( aSizeOfBlock0 != 0, "Block 0 has a size" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBadAddress, aSizeOfBlock0 ), eTDAL_FLA_BAD_PARAMETER,
		"Erase block on bad address returns bad param" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0-1 ), eTDAL_FLA_BAD_PARAMETER,
		"Erase block on good address with bad size returns bad param" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestUnitary_TestSetPartition( void )
 *  \brief Test that TDAL_FLA_SetPartition returns bad parameter.
 */
void TestUnitary_TestSetPartition(void)
{
	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 12, NULL ), eTDAL_FLA_BAD_PARAMETER,
		"Set partition with null pointer" );

	TestManager_AssertEqual( TDAL_FLA_SetPartition( 0, NULL ), eTDAL_FLA_BAD_PARAMETER,
		"Set partition with null size" );

	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}
