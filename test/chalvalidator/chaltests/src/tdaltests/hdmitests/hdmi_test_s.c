/** \file hdmi_test_s.c
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






tTestNode gTDAL_HDMI_StressTestSuite = {
  "TDAL_HDMI Stress Tests",
  "Test the TDAL_HDMI EDID data access",
  0, /* not interactive */
  HDMI_TestStressPlugUnplug, /* Only one function, it's NOT a test suite */
  NULL /* Not a test suite */
};

/*************************************************
 *                    Locales                    *
 *************************************************/
LOCAL bool AVIDisplayFormatForced = 0;




/******************************************************
 *              Unitary tests                         *
 ******************************************************/

/*! \fn void HDMI_TestStressPlugUnplug( void )
 *  \brief Test initialization and termination of HDMI driver
 */


void HDMI_TestStressPlugUnplug(void)
{

    /* Set sourceId to 0 by default */
    tTDAL_HDMI_SourceId sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Default value */
    uint8_t test;
    int i;

    tTDAL_HDMI_EDIDData stEDIDData;
    /*tTDAL_HDMI_EDIDRawBuffer stEDIDRawBuffer;*/
    tTDAL_HDMI_Capability stCapability;

    TestHelper_OpenComponentsForTDAL_HDMI();

    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI init" );
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback,AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(&stCapability), eTDAL_HDMI_NO_ERROR, "HDMI Get capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "HDMI capabilities : %d sources, EDID Parsing : %s\n",stCapability.NbSource, (stCapability.EDIDParsingCapable ? "available":"NOT available") ));


    /* StressTest_HDMI_01 - 000 : PlugUnplug tests */
    /* TODO : (STRESS) read several times the EDID data structure and check parsing */ 

    /* StressTest_HDMI_01 - 001 : Get EDID several times */
    /* TODO : (STRESS) read several times the EDID data structure and check parsing */ 
    /* For these tests, the plug/unplug test can be done several times, 
     * and the read of EDID structure can be used to check that sink 
     * detection is done correctly 
     */

    TestManager_AssertEqual( TDAL_HDMI_EDIDDataGet(sourceId, &stEDIDData), eTDAL_HDMI_NO_ERROR, "HDMI EDID Data Get" );
    
    test = 0x00;
    test |= stEDIDData.stEDIDBasic.ucHeader[0] | stEDIDData.stEDIDBasic.ucHeader[7];
    for(i = 1; i < 7; i++)
      test |= ~stEDIDData.stEDIDBasic.ucHeader[i];
    if (test == 0x00) 
      { 
        mTBOX_PRINT( ( kTBOX_LF, "EDID BASIC Data found : \n" ) );
        mTBOX_PRINT( ( kTBOX_LF, "Vendor Product Id : %s\n",stEDIDData.stEDIDBasic.stVendorProductIdent.cIDManufacturerName ) );
        mTBOX_PRINT( ( kTBOX_LF, "Version.Revision : %d.%d\n",stEDIDData.stEDIDBasic.ucVersion, stEDIDData.stEDIDBasic.ucRevision ) );
      }
      
    /* Don't forget to stop and terminate */
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}

#endif /* TDAL_HDMI_USE */


