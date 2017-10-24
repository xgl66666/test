/** \file hdmi_test_u.c
 *  \Reliability Tests for the HDMI layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 17/05/06
 *  \version 1.0
 */

#ifdef TDAL_HDMI_USE

/******************************************************
 *              Includes                              *
 ******************************************************/

#include <stdio.h>

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


tTestNode gTDAL_HDMI_ReliabilityTestSuite = {
  "TDAL_HDMI Reliability Tests",
  "Test the reliability of TDAL_HDMI functions",
  0, /* not interactive */
  HDMI_TestReliabilityRandom, /* Only one function, it's NOT a test suite */
  NULL /* Not a test suite */
};

/******************************************************
 *              Locales                               *
 ******************************************************/

LOCAL bool AVIDisplayFormatForced = 0;


/******************************************************
 *              Unitary tests                         *
 ******************************************************/

/*! \fn void HDMI_TestReliability_Random( void )
 *  \Random test of usual TDAL_HDMI functions
 */

     
void HDMI_TestReliabilityRandom(void)
{
    /* Set sourceId to 0 by default */
    tTDAL_HDMI_SourceId sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Default value */
    tTDAL_HDMI_EDIDData stEDIDData;
    tTDAL_HDMI_EDIDRawBuffer stEDIDBuffer;
    tTDAL_HDMI_InfoFrameSPD stSPD;
    tTDAL_HDMI_InfoFrameVS stVS;
    tTDAL_HDMI_HDCPParams params;
    tTDAL_HDMI_Capability stCapability;
    uint8_t VS_Payload[] = {0};
    uint8_t test;
    int i;  

    TestHelper_OpenComponentsForTDAL_HDMI();

    /* HDMI Init and start  */
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI init" );
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback,AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(&stCapability), eTDAL_HDMI_NO_ERROR, "HDMI Get capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "HDMI capabilities : %d sources, EDID Parsing : %s\n",stCapability.NbSource, (stCapability.EDIDParsingCapable ? "available":"NOT available") ));
        
    /* Init all structures */
    /* Infoframe SPD */
    stSPD.uiVersion = SPD_VERSION;
    sprintf(stSPD.cVendorName,"%s",VENDOR_NAME);
    sprintf(stSPD.cProductDescription,"%s",DESCRIPTION);
    stSPD.eSrcDeviceInfo = SRC_DEVICE;
    /* HDCP */
    params.uiIV[0] = DUMMY_IV_0;
    params.uiIV[1] = DUMMY_IV_1;
    params.uiKSV[0] = DUMMY_KSV_0;
    params.uiKSV[1] = DUMMY_KSV_1;
    for (i = 0; i < 80; i++)
      params.uiDeviceKeys[i] = DUMMY_DEVICE_KEY; /* All keys are init'd to dummy values */

    /* Infoframe VS */
    stVS.uiVersion = VS_VERSION;
    stVS.ucIEEERegistrationId[0] = IEEE_REGISTER_0;
    stVS.ucIEEERegistrationId[1] = IEEE_REGISTER_1;
    stVS.ucIEEERegistrationId[2] = IEEE_REGISTER_2;
    stVS.uiSpecificPayloadLength = PAYLOAD_LENGTH;
    stVS.pucSpecificPayload = VS_Payload;


    /* The following code is a sample code that must be inserted in a loop */
    /* This test depends of TDAL_HDMI/driver code, and need it to be terminated */
    TestManager_AssertEqual( TDAL_HDMI_InfoFrameVSSet(sourceId, &stVS), eTDAL_HDMI_NO_ERROR, "HDMI InfoFrameVSSet" );

    TestManager_AssertEqual( TDAL_HDMI_HDCPParamSet(sourceId, &params), eTDAL_HDMI_NO_ERROR, "HDMI HDCP ParamSet" );

    TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI HDCP enable" );

    TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI HDCP disable" );

    TestManager_AssertEqual( TDAL_HDMI_InfoFrameSPDSet(sourceId, &stSPD), eTDAL_HDMI_NO_ERROR, "HDMI Infoframe Set SPD " );
    
    TestManager_AssertEqual( TDAL_HDMI_EDIDRawBufferGet(sourceId, &stEDIDBuffer), eTDAL_HDMI_NO_ERROR, "EDID Raw Data" );

    
    TestManager_AssertEqual( TDAL_HDMI_EDIDDataGet(sourceId, &stEDIDData), eTDAL_HDMI_NO_ERROR, "HDMI EDID Data Get" );
    
    test = 0x00;
    test |= stEDIDData.stEDIDBasic.ucHeader[0] | stEDIDData.stEDIDBasic.ucHeader[7];
    for(i = 1; i < 7; i++)
    {
        test |= ~stEDIDData.stEDIDBasic.ucHeader[i];
    }
    if (test == 0x00) 
      { 
        mTBOX_PRINT( ( kTBOX_LF, "EDID BASIC Data found : \n" ) );
        mTBOX_PRINT( ( kTBOX_LF, "Vendor Product Id : %s\n",stEDIDData.stEDIDBasic.stVendorProductIdent.cIDManufacturerName ) );
        mTBOX_PRINT( ( kTBOX_LF, "Version.Revision : %d.%d\n",stEDIDData.stEDIDBasic.ucVersion, stEDIDData.stEDIDBasic.ucRevision ) );
      }
    /* ... And so on */     

    /* Stop and term */
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();
}

#endif /* TDAL_HDMI_USE */


