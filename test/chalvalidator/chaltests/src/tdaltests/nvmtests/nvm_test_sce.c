/** \file nvm_test_sce.c
 *  \brief Scenario Tests for the NVM layer of the TDAL API
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
#include "tdal_nvm.h"

#define USE_FLASH_CFG

#ifdef USE_FLASH_CFG
#include "flash_cfg.h"
#else
typedef struct
{
	uint32_t addressOffset;
	uint32_t size;
}tFLASH_CFG_Location;
#endif

#include "TestManager.h"
#include "nvmtests.h"

#ifdef CIPLUSVALIDATOR
#include "glue4tests.h"
#else
#include "testhelper.h"
#endif

#ifndef ERROR_WHEN_NO_RIGHT
#define ERROR_WHEN_NO_RIGHT	eTDAL_NVM_NOT_DONE
#endif

#define CRC_SIZE 4;

void TestNvmSce_WriteRead(void);
void TestNvmSce_WriteContiguousAddress(void);
void TestNvmSce_VariousReading(void);
void TestNvmSce_FillFF(void);
void TestNvmSce_Fill00(void);

/******************************************************
 *              NVM test tree                         *
 ******************************************************/


static tTestNode gTestScenarioWriteRead = {
	"TestScenarioWriteRead",
	"Test a simple write+read and compare result",
	0,
	TestNvmSce_WriteRead,
	NULL
};

static tTestNode gTestScenarioWriteContiguousAddress = {
	"TestScenarioWriteContiguousAddress",
	"Test some writing on contiguous addresses",
	0,
	TestNvmSce_WriteContiguousAddress,
	NULL
};

static tTestNode gTestScenarioVariousReading = {
	"TestScenarioVariousReading",
	"Test some writing on contiguous addresses",
	0,
	TestNvmSce_VariousReading,
	NULL
};

static tTestNode gTestScenarioFillFF = {
	"TestScenarioFillFF",
	"Fill all NVM with 0xFF",
	0,
	TestNvmSce_FillFF,
	NULL
};

static tTestNode gTestScenarioFill00 = {
	"TestScenarioFill00",
	"Fill all NVM with 0x00",
	0,
	TestNvmSce_Fill00,
	NULL
};

tTestNode* gNVMScenarioTestNodeList[] = {
	&gTestScenarioWriteRead,
	&gTestScenarioWriteContiguousAddress,
	&gTestScenarioVariousReading,
	&gTestScenarioFillFF,
	&gTestScenarioFill00,
	NULL
};

tTestNode gTDAL_NVM_ScenarioTestSuite = {
	"TDAL_NVM Scenario Tests",
	"test TDAL_NVM scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gNVMScenarioTestNodeList
};

/******************************************************
 *              Scenario tests                        *
 ******************************************************/

/*! \fn void TestNvmSce_WriteRead( void )
 *  \brief a simple test of write and finally read
 */
void TestNvmSce_WriteRead()
{
	uint8_t aWriteBuffer[9] = { 5, 2, 7, 9, 3, 1, 7, 0, 6 };
	uint8_t aReadBuffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t *pBuffer;
	uint32_t i, error = 0;
	uint32_t availableSize;
    tFLASH_CFG_Location NVM_Location;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );
	TestManager_AssertEqual( TDAL_NVM_Write( 0, 9 , &aWriteBuffer[0]), 9, "write 9 bytes" );
	TestManager_AssertEqual( TDAL_NVM_Read( 0, 9, &aReadBuffer[0]), 9, "read 9 bytes" );
	TestManager_AssertTrue( memcmp( aWriteBuffer, aWriteBuffer, 9 ) == 0, "read bytes are the expected ones" );

    /* get nvm size */
#ifdef USE_FLASH_CFG
	FLASH_CFG_GetNvmLocation(&NVM_Location);
#else
	NVM_Location.addressOffset= 0x00000000;
	NVM_Location.size = 0x00004000;
#endif
   	TBOX_Print(__LINE__,__FILE__,"NVM size=%d (0x%x)\n", NVM_Location.size, NVM_Location.addressOffset);
   	availableSize = NVM_Location.size - CRC_SIZE;

   	TestManager_AssertEqual( TKEL_Malloc( availableSize, (void**)&pBuffer), TKEL_NO_ERR, "allocate a buffer of max size");
    memset (pBuffer, 0xA5, availableSize);

	TestManager_AssertEqual( TDAL_NVM_Write( 0, availableSize , pBuffer), availableSize, "write all eeprom" );
    memset (pBuffer, 0x00, availableSize);
	TestManager_AssertEqual( TDAL_NVM_Read( 0, availableSize, pBuffer), availableSize, "read all bytes" );
	for( i = 0; i < availableSize; i++ )
	{
        if (pBuffer[i] != 0xA5) error++;
    }
	TestManager_AssertEqual( error, 0, "all bytes are correct" );

	TestManager_AssertEqual( TKEL_Free( pBuffer ), TKEL_NO_ERR, "free buffer" );

	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestNvmSce_WriteContiguousAddress( void )
 *  \brief a simple test of +write on some contiguous address
 */
void TestNvmSce_WriteContiguousAddress(void)
{
	uint8_t aReadBuffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t i = 0;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );

	TestManager_AssertEqual( TDAL_NVM_Write( 0, 9, &aReadBuffer[0] ), 9, "write 9 bytes" );

	/* write many times contiguously */
	for( i = 0; i < 9; i++ )
	{
		uint8_t aRead;
		TestManager_AssertEqual( TDAL_NVM_Write( 0 + i, 1, &i), 1, "write 1 byte" );
		TestManager_AssertEqual( TDAL_NVM_Read( 0 + i, 1, &aRead), 1, "read 1 byte" );
		TestManager_AssertEqual( aRead, i, "read byte is the expected one" );
	}

	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestNvmSce_VariousReading( void )
 *  \brief a test with a big write and many reading at different address
 */
void TestNvmSce_VariousReading(void)
{
	uint32_t aStep;
	uint8_t* apWriteBuffer = NULL;
	uint8_t* apReadBuffer = NULL;
	uint32_t i=0;
	uint32_t j=0;
	uint8_t aWrittenByte=0;
    tFLASH_CFG_Location NVM_Location;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );

#ifdef USE_FLASH_CFG
	FLASH_CFG_GetNvmLocation(&NVM_Location);
#else
	NVM_Location.addressOffset= 0x00000000;
	NVM_Location.size = 0x00004000;
#endif
   	TBOX_Print(__LINE__,__FILE__,"NVM size=%d (0x%x)\n", NVM_Location.size, NVM_Location.addressOffset);
	TestManager_AssertEqual( TKEL_Malloc( NVM_Location.size, (void**)&apWriteBuffer), TKEL_NO_ERR, "allocate a buffer of same size");
	TestManager_AssertEqual( TKEL_Malloc( NVM_Location.size, (void**)&apReadBuffer), TKEL_NO_ERR, "allocate a buffer of same size");

	if( apWriteBuffer == NULL || apReadBuffer == NULL )
	{
		return;
	}

	/* Initialize buffer */
	for( i=0; i<NVM_Location.size; i++ )
	{
		aWrittenByte++; /* it can overflow many times, it's not a problem */
		apWriteBuffer[i] = aWrittenByte;
	}

	TestManager_AssertEqual( TDAL_NVM_Write( 0, NVM_Location.size, apWriteBuffer ), NVM_Location.size,
		"write the whole block" );

	/* read many times */
	aStep = NVM_Location.size/23;
	for( j = 0; j < (NVM_Location.size - aStep); j += aStep )
	{
		/* init read buffer */
		/* by first, copying the original buffer */
		memcpy( apReadBuffer, apWriteBuffer, NVM_Location.size);
		/* then, reseting the part that will be read */
		memset( apReadBuffer+j, 0, aStep );

		TestManager_AssertEqual( TDAL_NVM_Read( 0 + j, aStep, &apReadBuffer[j]), aStep, "read some bytes" );
		TestManager_AssertTrue( memcmp(apWriteBuffer, apReadBuffer, NVM_Location.size) == 0, "read bytes are expected" );
	}

	TestManager_AssertEqual( TKEL_Free( apReadBuffer ), TKEL_NO_ERR, "free read buffer" );
	TestManager_AssertEqual( TKEL_Free( apWriteBuffer ), TKEL_NO_ERR, "free write buffer" );
	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestNvmSce_FillFF( void )
 *  \brief a test with a big write and many reading at different address
 */
void TestNvmSce_FillFF()
{
	uint8_t *pBuffer;
	uint32_t i, error = 0;
    uint32_t availableSize;
    tFLASH_CFG_Location NVM_Location;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );

    /* get nvm size */
#ifdef USE_FLASH_CFG
	FLASH_CFG_GetNvmLocation(&NVM_Location);
#else
	NVM_Location.addressOffset= 0x00000000;
	NVM_Location.size = 0x00004000;
#endif
   	TBOX_Print(__LINE__,__FILE__,"NVM size=%d (0x%x)\n", NVM_Location.size, NVM_Location.addressOffset);
   	availableSize = NVM_Location.size - CRC_SIZE;
   	TestManager_AssertEqual( TKEL_Malloc( availableSize, (void**)&pBuffer), TKEL_NO_ERR, "allocate a buffer of max size");
    memset (pBuffer, 0xFF, availableSize);

	TestManager_AssertEqual( TDAL_NVM_Write( 0, availableSize , pBuffer), availableSize, "write all eeprom" );
    memset (pBuffer, 0x00, availableSize);
	TestManager_AssertEqual( TDAL_NVM_Read( 0, availableSize, pBuffer), availableSize, "read all bytes" );
	for( i = 0; i < availableSize; i++ )
	{
        if (pBuffer[i] != 0xFF) error++;
    }
	TestManager_AssertEqual( error, 0, "all bytes are correct" );

	TestManager_AssertEqual( TKEL_Free( pBuffer ), TKEL_NO_ERR, "free buffer" );

	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}

/*! \fn void TestNvmSce_Fill00( void )
 *  \brief a test with a big write and many reading at different address
 */
void TestNvmSce_Fill00()
{
	uint8_t *pBuffer;
	uint32_t i, error = 0;
    tFLASH_CFG_Location NVM_Location;
    uint32_t availableSize;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
#else
    TestHelper_Initialize_TDAL_FLA();
#endif

	TestManager_AssertEqual( TDAL_NVM_Init(), eTDAL_NVM_NO_ERROR, "NVM init" );
	TestManager_AssertEqual( TDAL_NVM_Open(), eTDAL_NVM_NO_ERROR, "NVM open" );

    /* get nvm size */
#ifdef USE_FLASH_CFG
	FLASH_CFG_GetNvmLocation(&NVM_Location);
#else
	NVM_Location.addressOffset= 0x00000000;
	NVM_Location.size = 0x00004000;
#endif
   	TBOX_Print(__LINE__,__FILE__,"NVM size=%d (0x%x)\n", NVM_Location.size, NVM_Location.addressOffset);
   	availableSize = NVM_Location.size - CRC_SIZE;
   	TestManager_AssertEqual( TKEL_Malloc( availableSize, (void**)&pBuffer), TKEL_NO_ERR, "allocate a buffer of max size");
    memset (pBuffer, 0x00, availableSize);

	TestManager_AssertEqual( TDAL_NVM_Write( 0, availableSize , pBuffer), availableSize, "write all eeprom" );
    memset (pBuffer, 0xFF, availableSize);
	TestManager_AssertEqual( TDAL_NVM_Read( 0, availableSize, pBuffer), availableSize, "read all bytes" );
	for( i = 0; i < availableSize; i++ )
	{
        if (pBuffer[i] != 0x00) error++;
    }
	TestManager_AssertEqual( error, 0, "all bytes are correct" );

	TestManager_AssertEqual( TKEL_Free( pBuffer ), TKEL_NO_ERR, "free buffer" );

	TestManager_AssertEqual( TDAL_NVM_Close(), eTDAL_NVM_NO_ERROR, "NVM close" );
	TestManager_AssertEqual( TDAL_NVM_Term(), eTDAL_NVM_NO_ERROR, "NVM terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_FLA();
#endif
}
