/** \file fla_test_sce.c
 *  \brief Scenario Tests for the FLA layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include <stdio.h>
#include <string.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_fla.h"

#include "TestManager.h"
#include "flatests.h"

#ifndef ERROR_WHEN_NO_RIGHT
#define ERROR_WHEN_NO_RIGHT	eTDAL_FLA_NOT_DONE
#endif

void TestSce_DescribeFlash(void);
void TestSce_EraseWriteRead(void);
void TestSce_WriteContiguousAddress(void);
void TestSce_VariousReading(void);
void TestSce_EraseAndCheck(void);
void TestSce_WriteProtection(void);
void TestSce_ReadProtection(void);
void TestSce_EraseWrite_ALL(void);

#define FLA_TEST_BLOCK 112  //last 1MB for 8M FLASH, org:32

/******************************************************
 *              FLA test tree                         *
 ******************************************************/


static tTestNode gTestScenarioDescribeFlash = {
	"TestScenarioDescribeFlash",
	"Describe flash in terms of block and addresses, and check for block contiguity",
	0,
	TestSce_DescribeFlash,
	NULL
};

static tTestNode gTestScenarioEraseWriteRead = {
	"TestScenarioEraseWriteRead",
	"Test a simple erase+write+read and compare result",
	0,
	TestSce_EraseWriteRead,
	NULL
};

static tTestNode gTestScenarioWriteContiguousAddress = {
	"TestScenarioWriteContiguousAddress",
	"Test some writing on contiguous addresses",
	0,
	TestSce_WriteContiguousAddress,
	NULL
};

static tTestNode gTestScenarioVariousReading = {
	"TestScenarioVariousReading",
	"Test some writing on contiguous addresses",
	0,
	TestSce_VariousReading,
	NULL
};

static tTestNode gTestScenarioEraseAndCheck = {
	"TestScenarioEraseAndCheck",
	"Erase block and check read value",
	0,
	TestSce_EraseAndCheck,
	NULL
};

static tTestNode gTestScenarioWriteProtection = {
	"TestScenarioWriteProtection",
	"test write protection",
	0,
	TestSce_WriteProtection,
	NULL
};

static tTestNode gTestScenarioReadProtection = {
	"TestScenarioReadProtection",
	"test read protection",
	0,
	TestSce_ReadProtection,
	NULL
};

static tTestNode gTestScenarioEraseWrite_ALL = {
	"TestScenarioEraseWriteAll",
	"test erase write all flash",
	0,
	TestSce_EraseWrite_ALL,
	NULL
};


tTestNode* gFLAScenarioTestNodeList[] = {
	&gTestScenarioDescribeFlash,
	&gTestScenarioEraseWriteRead,
	&gTestScenarioWriteContiguousAddress,
	&gTestScenarioVariousReading,
	&gTestScenarioEraseAndCheck,
	&gTestScenarioWriteProtection,
	&gTestScenarioReadProtection,
	&gTestScenarioEraseWrite_ALL,
	NULL
};

tTestNode gTDAL_FLA_ScenarioTestSuite = {
	"TDAL_FLA Scenario Tests",
	"test TDAL_FLA scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gFLAScenarioTestNodeList
};

/******************************************************
 *              Scenario tests                        *
 ******************************************************/

/*! \fn void TestSce_DescribeFlash( void )
 *  \brief describe flash in terms of block and addresses
 */
void TestSce_DescribeFlash(void)
{
	char aString[1024];
	uint32_t aSizeOfBlock = 0;
	uint32_t aBlockAdress = 0;
	uint32_t aLastBlockAdress = 0;
	uint32_t aBlockNumber = 0;
	uint32_t aNumberOfBlocks = 0;
	int bIsContiguous = 1;

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );
	aNumberOfBlocks = TDAL_FLA_GetNumberOfBlocks();
	TestManager_AssertTrue( aNumberOfBlocks != 0, "Not 0 blocks" );

	sprintf(aString, "FLASH : %d Blocks", aNumberOfBlocks );
	TestManager_LogMessage( aString );
	for( aBlockNumber = 0; aBlockNumber < aNumberOfBlocks; aBlockNumber++ )
	{
		aBlockAdress = TDAL_FLA_GetBlockAddress( aBlockNumber );
		aSizeOfBlock = TDAL_FLA_GetBlockSize( aBlockNumber );

		if( aLastBlockAdress )
		{
			/* check that there is no overlap, nor hole */
			if( aLastBlockAdress+1 < aBlockAdress )
			{
				TestManager_LogMessage( "hole : blocks not contiguous" );
				bIsContiguous = 0;
			}
			else if( aLastBlockAdress+1 > aBlockAdress )
			{
				TestManager_AssertTrue( 0, "block overlap" );
			}
			else
			{
				; /* log nothing: normal case */
			}
		}
		aLastBlockAdress = aBlockAdress + aSizeOfBlock - 1;
		sprintf(aString, "Block %d : starts at 0x%x, size is %d", aBlockNumber, aBlockAdress, aSizeOfBlock );
		TestManager_LogMessage( aString );
	}

	if( !bIsContiguous )
	{
		TestManager_LogMessage( "some blocks are not contiguous" );
	}

	TestManager_AskForChecking( "do you agree with this flash description" );

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestSce_EraseWriteRead( void )
 *  \brief a simple test of erase+write and finally read
 */
void TestSce_EraseWriteRead()
{
	uint32_t aSizeOfBlock0 = 0;
	uint32_t aBlock0Adress;
	uint8_t aWriteBuffer[9] = { 5, 2, 7, 9, 3, 1, 7, 0, 6 };
	uint8_t aReadBuffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );
	aBlock0Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK);
	aSizeOfBlock0 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK);
	TestManager_AssertTrue( aSizeOfBlock0 != 0, "Block 0 has a size" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "erase block 0" );

	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 9, "write 9 bytes" );

	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "read 9 bytes" );

	TestManager_AssertTrue( memcmp( aReadBuffer, aWriteBuffer, 9 ) == 0, "read bytes are the exepected ones" );

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestSce_EraseWriteContiguousAddress( void )
 *  \brief a simple test of erase+write on some contiguous address
 */
void TestSce_WriteContiguousAddress(void)
{
	uint32_t aSizeOfBlock0 = 0;
	uint32_t aBlock0Adress;
	uint8_t aReadBuffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t i=0;

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );
	aBlock0Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK);
	aSizeOfBlock0 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK);
	TestManager_AssertTrue( aSizeOfBlock0 != 0, "Block 0 has a size" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "erase block 0" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "write 9 bytes" );

	/* write many times contiguously */
	for( i = 0; i < 9; i++ )
	{
		uint8_t aRead;
		TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "erase block 0" );
		TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress + i, &i, 1 ), 1, "write 1 byte" );
		TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress + i, &aRead, 1 ), 1, "read 1 byte" );
		TestManager_AssertEqual( aRead, i, "read byte is the expected one" );
	}

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestSce_VariousReading( void )
 *  \brief a test with a big write and many reading at different address
 */
void TestSce_VariousReading(void)
{
	uint32_t aSizeOfBlock0 = 0;
	uint32_t aBlock0Adress;
	uint32_t aStep;
	uint8_t* apWriteBuffer = NULL;
	uint8_t* apReadBuffer = NULL;
	uint32_t i=0;
	uint32_t j=0;
	uint8_t aWrittenByte=0;

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );
	aBlock0Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK);
	aSizeOfBlock0 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK);
	TestManager_AssertTrue( aSizeOfBlock0 != 0, "Block 0 has a size" );

	TestManager_AssertEqual( TKEL_Malloc( aSizeOfBlock0, (void**)&apWriteBuffer ), TKEL_NO_ERR,
	 "allocate a buffer of same size" );
	TestManager_AssertEqual( TKEL_Malloc( aSizeOfBlock0, (void**)&apReadBuffer ), TKEL_NO_ERR,
	 "allocate a buffer of same size" );
	if( apWriteBuffer == NULL || apReadBuffer == NULL )
	{
		return;
	}

	/* Initialize buffer */
	for( i=0; i<aSizeOfBlock0; i++ )
	{
		aWrittenByte++; /* it can overflow many times, it's not a problem */
		apWriteBuffer[i] = aWrittenByte;
	}

	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "erase block 0" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, apWriteBuffer, aSizeOfBlock0 ), aSizeOfBlock0,
		"write the whole block" );

	/* read many times */
	aStep = aSizeOfBlock0/23;
	for( j = 0; j < (aSizeOfBlock0 - aStep); j += aStep )
	{
		/* init read buffer */
		/* by first, copying the original buffer */
		memcpy( apReadBuffer, apWriteBuffer, aSizeOfBlock0);
		/* then, reseting the part that will be read */
		memset( apReadBuffer+j, 0, aStep );

		TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress + j, &apReadBuffer[j], aStep ), aStep, "read some bytes" );
		TestManager_AssertTrue( memcmp(apWriteBuffer, apReadBuffer, aSizeOfBlock0) == 0, "read bytes are expected" );
	}

	TestManager_AssertEqual( TKEL_Free( apReadBuffer ), TKEL_NO_ERR, "free read buffer" );
	TestManager_AssertEqual( TKEL_Free( apWriteBuffer ), TKEL_NO_ERR, "free write buffer" );
	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestSce_EraseAndCheck( void )
 *  \brief a test with full erase, that checks that read value is 0xff
 */
void TestSce_EraseAndCheck(void)
{
	uint32_t aSizeOfBlock = 0;
	uint32_t aBlockAdress = 0;
	uint32_t aBlockNumber = 0;
	uint32_t aNumberOfBlocks = 0;

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );
	aNumberOfBlocks = TDAL_FLA_GetNumberOfBlocks();
	TestManager_AssertTrue( aNumberOfBlocks != 0, "Not 0 blocks" );
	for( aBlockNumber = FLA_TEST_BLOCK; aBlockNumber < aNumberOfBlocks; aBlockNumber++ )
	{
		uint8_t* apBuffer = NULL;
		uint32_t aIndex;
		aBlockAdress = TDAL_FLA_GetBlockAddress( aBlockNumber );
		aSizeOfBlock = TDAL_FLA_GetBlockSize( aBlockNumber );
		TestManager_AssertTrue( aSizeOfBlock != 0, "Block has a size" );
		TestManager_AssertEqual( TDAL_FLA_Erase( aBlockAdress, aSizeOfBlock ), eTDAL_FLA_NO_ERROR, "erase block" );
		TestManager_AssertEqual( TKEL_Malloc( aSizeOfBlock, (void**)&apBuffer ), TKEL_NO_ERR, "alloc buffer" );
		TestManager_AssertEqual( TDAL_FLA_Read( aBlockAdress, &apBuffer[0], aSizeOfBlock ), aSizeOfBlock,"read all block" );
		/* check read value */
		for( aIndex = 0; aIndex < aSizeOfBlock; aIndex++ )
		{
			if( apBuffer[ aIndex ] != 0xff )
			{
				break;
			}
		}
		TestManager_AssertTrue( aIndex == aSizeOfBlock, "buffer has value 0xff after erase" );
		TestManager_AssertEqual( TKEL_Free( apBuffer ), TKEL_NO_ERR, "free buffer" );
	}

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

/*! \fn void TestSce_WriteProtection( void )
 *  \brief a test about write protection, set and then unset
 */
void TestSce_WriteProtection(void)
{
	uint32_t aSizeOfBlock0 = 0;
	uint32_t aSizeOfBlock1 = 0;
	uint32_t aSizeOfBlock2 = 0;
	uint32_t aBlock0Adress;
	uint32_t aBlock1Adress;
	uint32_t aBlock2Adress;
	uint8_t aWriteBuffer[9] = { 5, 2, 7, 9, 3, 1, 7, 0, 6 };
	uint8_t aWriteBuffer1[9] = { 0, 255, 0, 255, 0, 255, 0, 255, 0 };
	uint8_t aReadBuffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	tTDAL_FLA_Partition aPartition[2];

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "1-FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "2-FLA open" );

	aBlock0Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK);
	aSizeOfBlock0 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK);
	TestManager_AssertTrue( aSizeOfBlock0 != 0, "3-Block 0 has a size" );

	aBlock1Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK+1);
	aSizeOfBlock1 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK+1);
	TestManager_AssertTrue( aSizeOfBlock1 != 0, "4-Block 1 has a size" );

	aBlock2Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK+2);
	aSizeOfBlock2 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK+2);
	TestManager_AssertTrue( aSizeOfBlock1 != 0, "5-Block 2 has a size" );

	/* without any partition, writing is allowed */
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "6-erase block 0" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 9, "7-write 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "8-read 9 bytes" );

	/* with a partition that includes this start address, writing is allowed */
	aPartition[0].StartAddress = aBlock0Adress;
	aPartition[0].EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;

	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, aPartition ), eTDAL_FLA_NO_ERROR, "9-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "10-erase block 0" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 9, "11-write 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "12-read 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer1[0], 9 ), 9, "13-write without erase allowed" );

	/* remove writing rights */
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, aPartition ), eTDAL_FLA_NO_ERROR, "14-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NOT_DONE, "15-erase block 0 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 0, "16-write not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "17-read 9 bytes" );

    /* reset first block to be able to write */
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, aPartition ), eTDAL_FLA_NO_ERROR, "18-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "19-erase block 0" );

	/* with a partition that does not include this start address, writing is NOT allowed */
	aPartition[0].StartAddress = aBlock0Adress + 1;
	aPartition[0].EndAddress = aBlock0Adress + aSizeOfBlock0 - 1;
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
	aPartition[1].StartAddress = aBlock1Adress;
	aPartition[1].EndAddress = aBlock1Adress + aSizeOfBlock1;
	aPartition[1].Rights = TDAL_FLA_READ_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 2, aPartition), eTDAL_FLA_NO_ERROR, "20-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NOT_DONE, "21-erase block 0 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock1Adress, aSizeOfBlock1 ), eTDAL_FLA_NOT_DONE, "22-erase block 1 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 0 ), 0, "23-write not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 0 ), 0, "24-read not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress + 1, &aWriteBuffer[0], 9 ), 9, "25-write 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress + 1, &aReadBuffer[0], 9 ), 9, "25-read 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress + aSizeOfBlock0 - 5, &aWriteBuffer[0], 9 ), 0, "27-write both partitions not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress + aSizeOfBlock0 - 5, &aReadBuffer[0], 9 ), 0, "28-read both partitions not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress + aSizeOfBlock0 - 20, &aWriteBuffer[0], 9 ), 9, "29-write 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress + aSizeOfBlock0 - 20, &aReadBuffer[0], 9 ), 9, "30-read 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock1Adress, &aWriteBuffer[0], 9 ), 0, "31-write 2nd partition not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock1Adress, &aReadBuffer[0], 9 ), 9, "32-read 2nd partitions allowed" );

	/* with a partition that does not include this start address, writing is NOT allowed */
	aPartition[0].StartAddress = aBlock0Adress;
	aPartition[0].EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
	aPartition[1].StartAddress = aBlock1Adress;
	aPartition[1].EndAddress = aBlock1Adress + aSizeOfBlock1;
	aPartition[1].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 2, aPartition), eTDAL_FLA_NO_ERROR, "33-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "34-erase block 0" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock1Adress, aSizeOfBlock1 ), eTDAL_FLA_NO_ERROR, "35-erase block 1" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock2Adress, aSizeOfBlock2 ), ERROR_WHEN_NO_RIGHT, "36-erase block 2 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 9, "37-write 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "38-read 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress + aSizeOfBlock0 - 5, &aWriteBuffer[0], 9 ), 0, "39-write both partitions not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress + aSizeOfBlock0 - 5, &aReadBuffer[0], 9 ), 0, "40-read both partitions not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock1Adress, &aWriteBuffer[0], 9 ), 9, "41-write 9 bytes in block 1" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock1Adress, &aReadBuffer[0], 9 ), 9, "42-read 9 bytes in block 1" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock1Adress + aSizeOfBlock1 - 5, &aWriteBuffer[0], 9 ), 0, "43-write after block 1 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock1Adress + aSizeOfBlock1 - 5, &aReadBuffer[0], 9 ), 0, "44-read after block 1 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock2Adress, &aWriteBuffer[0], 9 ), 0, "45-write in block 2 not allowed" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock2Adress, &aReadBuffer[0], 9 ), 0, "46-read in block 2 not allowed" );

	/* First allow writing, and erase */
	aPartition[0].StartAddress = aBlock0Adress;
	aPartition[0].EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, aPartition), eTDAL_FLA_NO_ERROR, "47-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "48-erase block 0" );
	/* Then forbid writing, and try to write */
	aPartition[0].StartAddress = aBlock0Adress + 1;
	aPartition[0].EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition[0].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, aPartition), eTDAL_FLA_NO_ERROR, "49-set partition" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 0, "50-write not allowed" );

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "51-FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "52-FLA terminate" );
}

/*! \fn void TestSce_ReadProtection( void )
 *  \brief a test about read protection, set and then unset
 */
void TestSce_ReadProtection(void)
{
	uint32_t aSizeOfBlock0 = 0;
	uint32_t aBlock0Adress;
	uint8_t aWriteBuffer[9] = { 5, 2, 7, 9, 3, 1, 7, 0, 6 };
	uint8_t aReadBuffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	tTDAL_FLA_Partition aPartition;

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );
	aBlock0Adress = TDAL_FLA_GetBlockAddress(FLA_TEST_BLOCK);
	aSizeOfBlock0 = TDAL_FLA_GetBlockSize(FLA_TEST_BLOCK);
	TestManager_AssertTrue( aSizeOfBlock0 != 0, "Block 0 has a size" );

	/* write something first */
	TestManager_AssertEqual( TDAL_FLA_Erase( aBlock0Adress, aSizeOfBlock0 ), eTDAL_FLA_NO_ERROR, "erase block 0" );
	TestManager_AssertEqual( TDAL_FLA_Write( aBlock0Adress, &aWriteBuffer[0], 9 ), 9, "write 9 bytes" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "read 9 bytes" );

	/* with a partition that includes this start address, reading is allowed */
	aPartition.StartAddress = aBlock0Adress;
	aPartition.EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition.Rights = TDAL_FLA_READ_ENABLE;

	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, &aPartition ), eTDAL_FLA_NO_ERROR, "set partition" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "read 9 bytes" );

	/* remove read rights */
	aPartition.Rights = 0;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, &aPartition ), eTDAL_FLA_NO_ERROR, "set partition" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 0, "read not allowed" );

	/* with a partition that does not include this start address, reading is NOT allowed */
	aPartition.StartAddress = aBlock0Adress + 1;
	aPartition.EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition.Rights = TDAL_FLA_READ_ENABLE;
	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, &aPartition), eTDAL_FLA_NO_ERROR, "set partition" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 0, "read not allowed" );

	/* reallow reading */
	aPartition.StartAddress = aBlock0Adress;
	aPartition.EndAddress = aBlock0Adress + aSizeOfBlock0;
	aPartition.Rights = TDAL_FLA_READ_ENABLE;

	TestManager_AssertEqual( TDAL_FLA_SetPartition( 1, &aPartition ), eTDAL_FLA_NO_ERROR, "set partition" );
	TestManager_AssertEqual( TDAL_FLA_Read( aBlock0Adress, &aReadBuffer[0], 9 ), 9, "read 9 bytes" );

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );
}

void TestSce_EraseWrite_ALL(void)
{
    /* Calculate the number of blocks required to store the ssw in flash */
    uint32_t totalBlockSize = 0;
    uint32_t blockNumber = 0;
    uint32_t NbError = 0;
    uint32_t cpt_block;
    uint8_t *pBufferTmp;

	TestManager_AssertEqual( TDAL_FLA_Init(), eTDAL_FLA_NO_ERROR, "FLA init" );
	TestManager_AssertEqual( TDAL_FLA_Open(), eTDAL_FLA_NO_ERROR, "FLA open" );

    do
    {
        totalBlockSize += TDAL_FLA_GetBlockSize(blockNumber);
        blockNumber++;
    } while (totalBlockSize < 0x800000);

    /* erase all sectors */
    for (cpt_block = FLA_TEST_BLOCK; cpt_block < blockNumber; cpt_block++)
    {
        printf("0x%08X %u\n", TDAL_FLA_GetBlockAddress(cpt_block), TDAL_FLA_GetBlockSize(cpt_block));
        TestManager_AssertEqual( TDAL_FLA_Erase(TDAL_FLA_GetBlockAddress(cpt_block), TDAL_FLA_GetBlockSize(cpt_block)), eTDAL_FLA_NO_ERROR, "erase block" );
    }

    for (cpt_block = FLA_TEST_BLOCK; cpt_block < blockNumber; cpt_block++)
    {
		/* read buffer */
		TestManager_AssertEqual( TKEL_Malloc(TDAL_FLA_GetBlockSize(cpt_block), (void**)&pBufferTmp),TKEL_NO_ERR,"alloc buffer" );

		if((pBufferTmp != (uint8_t*)NULL))
		{
			memset(pBufferTmp, 0, TDAL_FLA_GetBlockSize(cpt_block));
			printf("0x%08X %u\n", TDAL_FLA_GetBlockAddress(cpt_block), TDAL_FLA_GetBlockSize(cpt_block));
			TestManager_AssertEqual( TDAL_FLA_Write( TDAL_FLA_GetBlockAddress(cpt_block), pBufferTmp, TDAL_FLA_GetBlockSize(cpt_block)), TDAL_FLA_GetBlockSize(cpt_block), "write bytes" );
			TestManager_AssertEqual( TKEL_Free( pBufferTmp ), TKEL_NO_ERR, "free buffer" );
		}
    }

    for (cpt_block = FLA_TEST_BLOCK; cpt_block < blockNumber; cpt_block++)
    {
        uint32_t uByteCounter;

        printf("0x%08X %u\n", TDAL_FLA_GetBlockAddress(cpt_block), TDAL_FLA_GetBlockSize(cpt_block));

		/* read buffer */
		TestManager_AssertEqual( TKEL_Malloc(TDAL_FLA_GetBlockSize(cpt_block), (void**)&pBufferTmp),TKEL_NO_ERR,"alloc buffer" );

		if((pBufferTmp != (uint8_t*)NULL))
		{
			memset(pBufferTmp, 0, TDAL_FLA_GetBlockSize(cpt_block));

			TestManager_AssertEqual( TDAL_FLA_Read( TDAL_FLA_GetBlockAddress(cpt_block), pBufferTmp, TDAL_FLA_GetBlockSize(cpt_block)), TDAL_FLA_GetBlockSize(cpt_block), "read bytes" );
			for (uByteCounter = 0; uByteCounter < TDAL_FLA_GetBlockSize(cpt_block); uByteCounter++)
			{
				if (pBufferTmp[uByteCounter] != 0)
				{
					NbError++;
				}
			}

			TestManager_AssertEqual( TKEL_Free( pBufferTmp ), TKEL_NO_ERR, "free buffer" );
			pBufferTmp = NULL;

		}
    }
	
	TestManager_AssertEqual( NbError, 0, "All bytes set to 0" );

	TestManager_AssertEqual( TDAL_FLA_Close(), eTDAL_FLA_NO_ERROR, "FLA close" );
	TestManager_AssertEqual( TDAL_FLA_Term(), eTDAL_FLA_NO_ERROR, "FLA terminate" );

}
