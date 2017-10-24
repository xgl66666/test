/** \file DESC_test_u.c
 *  \brief Unitary Tests for the DESC layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_desc.h"

#include "TestManager.h"
#include "desctests.h"

#ifdef CIPLUSVALIDATOR
#include "glue4tests.h"
#else
#include "testhelper.h"
#endif

void TestUnitary_DescInitTerm(void);
void TestUnitary_DescOpenClose(void);
void TestUnitary_DVB_CSA(void);
void TestUnitary_AES_128_CBC(void);
void TestUnitary_DES_56_ECB(void);
void TestUnitary_PID(void);

/******************************************************
 * 				Defines								  *
 ******************************************************/

/******************************************************
 * 				Local variables						  *
 ******************************************************/

/******************************************************
 *              DESC test tree                       *
 ******************************************************/

static tTestNode gTestUInitTerm = {
	"TestUInitTerm",
	"test TDAL_DESC_Init() and TDAL_DESC_Term() functions",
	0, /* not interactive */
	TestUnitary_DescInitTerm,
	NULL
};

static tTestNode gTestUOpenClose = {
	"TestUOpenClose",
	"test TDAL_DESC_Open() and TDAL_DESC_Close() functions",
	0, /* not interactive */
	TestUnitary_DescOpenClose,
	NULL
};

static tTestNode gTestU_DVB_CSA = {
	"TestU_DVB_CSA",
	"Set DVB_CSA keys",
	0, /* not interactive */
	TestUnitary_DVB_CSA,
	NULL
};

static tTestNode gTestU_DES_56_ECB = {
	"TestU_DES_56_ECB",
	"Set DES_56_ECB keys",
	0, /* not interactive */
	TestUnitary_DES_56_ECB,
	NULL
};

static tTestNode gTestU_AES_128_CBC = {
	"TestU_AES_128_CBC",
	"Set AES_128_CBC keys",
	0, /* not interactive */
	TestUnitary_AES_128_CBC,
	NULL
};

static tTestNode gTestU_PID = {
	"TestU_PID",
	"PID set tests",
	0, /* not interactive */
	TestUnitary_PID,
	NULL
};

tTestNode* gTDAL_DESC_UnitaryTestSuite[] = {
	&gTestUInitTerm,
	&gTestUOpenClose,
	&gTestU_DVB_CSA,
	&gTestU_DES_56_ECB,
	&gTestU_AES_128_CBC,
	&gTestU_PID,
	NULL
};

/******************************************************
 * 				Unitary tests 						  *
 ******************************************************/

/*===================================================================
 *
 * TestUnitary_DescInitTerm
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *==================================================================*/
void TestUnitary_DescInitTerm()
{
    tTDAL_DESC_Capability Capability;
#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
    TDAL_DESC_Terminate(); /* if desc init is done in glue4test init*/
#else
    TestHelper_Initialize_TDAL_DMX();
#endif
    /* basic functions */
    TDAL_DESC_GetCapability(&Capability);
	mTBOX_PRINT((kTBOX_LF, "*****************************\n"));
	mTBOX_PRINT((kTBOX_LF, "Nb Descrambler %u\n", Capability.bNbDescrambler_Max));
	mTBOX_PRINT((kTBOX_LF, "    DVB_CSA %s\n", (Capability.isDVB_CSA_Supported == TRUE)?"Supported":"Not supported"));
	mTBOX_PRINT((kTBOX_LF, "    AES_128_CBC %s\n", (Capability.isAES_128_CBC_Supported == TRUE)?"Supported":"Not supported"));
	mTBOX_PRINT((kTBOX_LF, "    DES_56_ECB %s\n", (Capability.isDES_56_ECB_Supported == TRUE)?"Supported":"Not supported"));
	mTBOX_PRINT((kTBOX_LF, "*****************************\n"));

	/* Standard Init and Terminate */
	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );

	/* Twice Init and twice Terminate */
	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );
	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NOT_DONE, "TDAL_DESC_Init" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NOT_DONE, "TDAL_DESC_Terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_DMX();
#endif
}

/*===================================================================
 *
 * TestUnitary_DescOpenClose
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *==================================================================*/
void TestUnitary_DescOpenClose()
{
    tTDAL_DESC_descrambler descHandle;
    int iCounter;

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
    TDAL_DESC_Terminate(); /* if desc init is done in glue4test init*/
#else
    TestHelper_Initialize_TDAL_DMX();
#endif

	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );

	/* Standard Open and Close */
	descHandle = TDAL_DESC_Open_Descrambler(TDAL_DESC_RESERVED);
	TestManager_AssertEqual(descHandle , 0, "TDAL_DESC_Open_Descrambler" );
	TestManager_AssertEqual( TDAL_DESC_Close_Descrambler(descHandle), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Close_Descrambler" );

	/* open all instances */
	for (iCounter = 0; iCounter < kTDAL_DESC_MAX_DESCRAMBLERS; iCounter++)
	{
    	descHandle = TDAL_DESC_Open_Descrambler(TDAL_DESC_RESERVED);
	    TestManager_AssertEqual(descHandle , iCounter, "TDAL_DESC_Open_Descrambler" );
	}

    /* close all instances */
	for (iCounter = 0; iCounter < kTDAL_DESC_MAX_DESCRAMBLERS; iCounter++)
	{
        TestManager_AssertEqual( TDAL_DESC_Close_Descrambler(iCounter), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Close_Descrambler" );
    }

	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_DMX();
#endif
}

/*===================================================================
 *
 * TestUnitary_DVB_CSA
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *==================================================================*/
 void TestUnitary_DVB_CSA()
{
    tTDAL_DESC_descrambler descHandle;
    const int8_t Key[8];

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
    TDAL_DESC_Terminate(); /* if desc init is done in glue4test init*/
#else
    TestHelper_Initialize_TDAL_DMX();
#endif

	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );
	descHandle = TDAL_DESC_Open_Descrambler(TDAL_DESC_RESERVED);
	TestManager_AssertEqual(descHandle , 0, "TDAL_DESC_Open_Descrambler" );

    /* must be OK */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_ODD_KEY,
                                                 sizeof(Key), Key),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_ODD_KEY" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_EVEN_KEY,
                                                 sizeof(Key), Key),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_EVEN_KEY" );

    /* must failed */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_GENERIC_KEY,
                                                 sizeof(Key), Key),
                            eTDAL_DESC_NOT_DONE, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_GENERIC_KEY" );

	TestManager_AssertEqual( TDAL_DESC_Close_Descrambler(descHandle), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Close_Descrambler" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_DMX();
#endif
}

/*===================================================================
 *
 * TestUnitary_AES_128_CBC
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *==================================================================*/
void TestUnitary_AES_128_CBC(void)
{
    tTDAL_DESC_descrambler descHandle;
    const int8_t Key128[32]; /* CCK[16]+CIV[16] */

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
    TDAL_DESC_Terminate(); /* if desc init is done in glue4test init*/
#else
    TestHelper_Initialize_TDAL_DMX();
#endif

	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );
	descHandle = TDAL_DESC_Open_Descrambler(TDAL_DESC_RESERVED);
	TestManager_AssertEqual(descHandle , 0, "TDAL_DESC_Open_Descrambler" );
	TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Type (descHandle, TDAL_DESC_AES_128_CBC),
	                        eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Type TDAL_DESC_AES_128_CBC" );

    /* must be OK */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_ODD_KEY,
                                                 sizeof(Key128), Key128),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_ODD_KEY" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_EVEN_KEY,
                                                 sizeof(Key128), Key128),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_EVEN_KEY" );

    /* must failed */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_GENERIC_KEY,
                                                 sizeof(Key128), Key128),
                            eTDAL_DESC_NOT_DONE, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_GENERIC_KEY" );

	TestManager_AssertEqual( TDAL_DESC_Close_Descrambler(descHandle), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Close_Descrambler" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_DMX();
#endif
}

/*===================================================================
 *
 * TestUnitary_DES_56_ECB
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *==================================================================*/
void TestUnitary_DES_56_ECB(void)
{
    tTDAL_DESC_descrambler descHandle;
    const int8_t Key64[8];

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
    TDAL_DESC_Terminate(); /* if desc init is done in glue4test init*/
#else
    TestHelper_Initialize_TDAL_DMX();
#endif

	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );
	descHandle = TDAL_DESC_Open_Descrambler(TDAL_DESC_RESERVED);
	TestManager_AssertEqual(descHandle , 0, "TDAL_DESC_Open_Descrambler" );
	TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Type (descHandle, TDAL_DESC_DES_56_ECB),
	                        eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Type TDAL_DESC_DES_56_ECB" );

    /* must be OK */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_ODD_KEY,
                                                 sizeof(Key64), Key64),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_ODD_KEY" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_EVEN_KEY,
                                                 sizeof(Key64), Key64),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_EVEN_KEY" );

    /* must failed */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Key  (descHandle,
                                                 TDAL_DESC_GENERIC_KEY,
                                                 sizeof(Key64), Key64),
                            eTDAL_DESC_NOT_DONE, "TDAL_DESC_Set_Descrambler_Key TDAL_DESC_GENERIC_KEY" );

	TestManager_AssertEqual( TDAL_DESC_Close_Descrambler(descHandle), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Close_Descrambler" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_DMX();
#endif
}

/*===================================================================
 *
 * TestUnitary_PID
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *==================================================================*/
void TestUnitary_PID(void)
{
    tTDAL_DESC_descrambler  descHandle;
    const int8_t            Key128_1[32]; /* CCK[16]+CIV[16] */
    const int8_t            Key128_2[32]; /* CCK[16]+CIV[16] */

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Init();
    TDAL_DESC_Terminate(); /* if desc init is done in glue4test init*/
#else
    TestHelper_Initialize_TDAL_DMX();
#endif

    /* DESC */
	TestManager_AssertEqual( TDAL_DESC_Init(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Init" );
	descHandle = TDAL_DESC_Open_Descrambler(TDAL_DESC_RESERVED);
	TestManager_AssertEqual(descHandle , 0, "TDAL_DESC_Open_Descrambler" );
	TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Type (descHandle, TDAL_DESC_AES_128_CBC),
	                        eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Type TDAL_DESC_AES_128_CBC" );

    /* set keys */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Keys  (descHandle,
                                                 sizeof(Key128_1), Key128_1,
                                                 sizeof(Key128_1), Key128_1),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Keys" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Keys  (descHandle,
                                                 sizeof(Key128_2), Key128_2,
                                                 sizeof(Key128_2), Key128_2),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Keys" );

    /* set not authorized PID */
    TestManager_AssertEqual(TDAL_DESC_Stream_Authorisation (0x1789, FALSE),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Stream_Authorisation FALSE" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Pid  (descHandle, 0x1789),
                            eTDAL_DESC_NOT_DONE, "TDAL_DESC_Set_Descrambler_Pid NOT authorize" );

    /* set authorize PID */
    TestManager_AssertEqual(TDAL_DESC_Stream_Authorisation (0x1789, TRUE),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Stream_Authorisation TRUE" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Pid  (descHandle, 0x1789),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Pid authorize (but not set in DEMUX!)" );

    /* set PID in Demux */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Pid  (descHandle, kTDAL_DESC_ILLEGAL_PID),
                            eTDAL_DESC_ERROR, "TDAL_DESC_Set_Descrambler_Pid kTDAL_DESC_ILLEGAL_PID" );

    /* set keys */
    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Keys  (descHandle,
                                                 sizeof(Key128_1), Key128_1,
                                                 sizeof(Key128_1), Key128_1),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Keys" );

    TestManager_AssertEqual(TDAL_DESC_Set_Descrambler_Keys  (descHandle,
                                                 sizeof(Key128_2), Key128_2,
                                                 sizeof(Key128_2), Key128_2),
                            eTDAL_DESC_NO_ERROR, "TDAL_DESC_Set_Descrambler_Keys" );

	TestManager_AssertEqual( TDAL_DESC_Close_Descrambler(descHandle), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Close_Descrambler" );
	TestManager_AssertEqual( TDAL_DESC_Terminate(), eTDAL_DESC_NO_ERROR, "TDAL_DESC_Terminate" );

#ifdef CIPLUSVALIDATOR
    GLUE4TESTS_Terminate();
#else
    TestHelper_Terminate_TDAL_DMX();
#endif
}
