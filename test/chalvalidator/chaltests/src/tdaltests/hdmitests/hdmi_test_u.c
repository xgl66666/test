/** \file hdmi_test_u.c
 *  \brief Unitary Tests for the HDMI layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 06/05/06
 *  \version 1.0
 */

#ifdef TDAL_HDMI_USE

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_hdmi.h"

#include "testhelper.h"
#include "hdmitests.h"


/******************************************************
 *              Defines                               *
 ******************************************************/

/******************************************************
 *              HDMI test tree                         *
 ******************************************************/






tTestNode gTDAL_HDMI_UnitaryTestSuite = {
  "TDAL_HDMI Unitary Tests",
  "test the TDAL_HDMI function input parameters",
  0, /* not interactive */
  HDMI_TestUnitary_BadParam, /* Only one function, it's NOT a test suite */
  NULL /* Not a test suite */
};


/******************************************************
 *                    Locales                         *
 ******************************************************/
LOCAL bool AVIDisplayFormatForced = 0;



/******************************************************
 *              Unitary tests                         *
 ******************************************************/

/*! \fn void GFX_TestUnitary_InitTerm( void )
 *  \brief Test initialization and termination of HDMI driver
 */


void HDMI_TestUnitary_BadParam()
{

	/* Set sourceId to 0 by default */
	tTDAL_HDMI_SourceId sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Default value */
	tTDAL_HDMI_SinkStatus sStatus;
	tTDAL_HDMI_EDIDData stEDIDData;
	tTDAL_HDMI_EDIDRawBuffer stEDIDRawBuffer;
	tTDAL_HDMI_InfoFrameSPD stSPD;
	tTDAL_HDMI_InfoFrameVS stVS;
	tTDAL_HDMI_HDCPParams Params;
	

   TestHelper_OpenComponentsForTDAL_HDMI();

	/* HDMI Init used for next tests */
	TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI init" );


	/* UnitTest_HDMI_01 - 000 : HDMI Start */
	TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, NULL,AVIDisplayFormatForced), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI start with null callback" );
	/* UnitTest_HDMI_01 - 001 : */
	sourceId = 0xff; /* Bad Id */
	TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback,AVIDisplayFormatForced), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI Start with bad SourceId" );
	
	/* UnitTest_HDMI_01 - 002 : HDMI Stop*/
	sourceId = eTDAL_HDMI_SOURCE_ID_0;
	TDAL_HDMI_Start(sourceId, DummyCallback,AVIDisplayFormatForced);
	sourceId = 0xff; /* Bad Id */
	TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI stop with bad source Id" );
	TDAL_HDMI_Stop(eTDAL_HDMI_SOURCE_ID_0);

	/* UnitTest_HDMI_01 - 003 : HDMI Capability get */
	TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI Get capabilities with NULL pointer" );


	/* UnitTest_HDMI_01 - 004 : HDMI Status get */
	sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_StatusGet(sourceId, &sStatus), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI StatusGet with bad Source Id" );

	sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Set a correct sourceId */
	TestManager_AssertEqual( TDAL_HDMI_StatusGet(sourceId, NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI StatusGet with NULL pointer for SinkStatus" );
	TDAL_HDMI_Start(sourceId, NULL,AVIDisplayFormatForced);

	/* UnitTest_HDMI_01 - 005 : HDMI EDID DataGet */

	sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_EDIDDataGet(sourceId, &stEDIDData), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI EDIDDataGet with NULL pointer for SinkStatus" );
	sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Set a correct sourceId */
	TestManager_AssertEqual( TDAL_HDMI_EDIDDataGet(sourceId, NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI EDIDDataGet with NULL pointer for EDIDData" );

	
	/* UnitTest_HDMI_01 - 006 : HDMI RawBufferGet */
	sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_EDIDRawBufferGet(sourceId, &stEDIDRawBuffer), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI EDIDRawBufferGet with bad source Id" );
	sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Set a correct sourceId */
	TestManager_AssertEqual( TDAL_HDMI_EDIDRawBufferGet(sourceId, NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI EDIDRawBufferGet with NULL pointer for raw buffer" );


	/* UnitTest_HDMI_01 - 007 : HDMI Info frame SPD set */
	sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_InfoFrameSPDSet(sourceId, &stSPD), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI InfoFrameSPDSet with bad source Id" );
	sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Set a correct sourceId */
	TestManager_AssertEqual( TDAL_HDMI_InfoFrameSPDSet(sourceId, NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI InfoFrameSPDSet with NULL pointer for SPD buffer" );



	/* UnitTest_HDMI_01 - 008 : HDMI Info frame Vss Set */
	sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_InfoFrameVSSet(sourceId, &stVS), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI InfoFrameVSSet with bad source Id" );
	sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Set a correct sourceId */
	TestManager_AssertEqual( TDAL_HDMI_InfoFrameVSSet(sourceId, NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI InfoFrameVSSet with NULL pointer for VS buffer" );
	
	/* UnitTest_HDMI_01 - 009 : HDMI HDCP enable/disable */
    sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI HDCPEnable with bad source Id" );
	/* UnitTest_HDMI_01 - 010 : HDMI HDCP enable/disable */
	TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI HDCPDisable with bad source Id" );
	
	/* UnitTest_HDMI_01 - 011 : HDCP ParamsSet */
	sourceId = 0xff; /* Bad SourceId */
	TestManager_AssertEqual( TDAL_HDMI_HDCPParamSet(sourceId, &Params), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI HDCPParamsSet with bad source Id" );
	sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Set a correct sourceId */
    TestManager_AssertEqual( TDAL_HDMI_HDCPParamSet(sourceId, NULL), eTDAL_HDMI_BAD_PARAMETER_ERROR, "HDMI HDCPParamsSet with NULL pointer for VS buffer" );


	TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI term" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}

#endif /* TDAL_HDMI_USE */


