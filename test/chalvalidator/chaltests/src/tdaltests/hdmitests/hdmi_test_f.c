/** \file hdmi_test_f.c
 *  \Functional Tests for the HDMI layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 06/05/06
 *  \version 1.0
 */


#ifdef TDAL_HDMI_USE
/* TODO * 
* Callback Event for callback = tTDAL_HDMI_CallbackFct NotifyFct
* VS_Payload PAYLOAG_LENGTH, IEEE_REGISTER
* 
*/


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



static tTestNode gTestFInitTerm = {
"TestFInitTerm",
"Test TDAL_HDMI_Init() and TDAL_HDMI_Term() functions",
0, /* not interactive */
HDMI_TestFunctionalInitTerm,
NULL
};


static tTestNode gTestFStartStop = {
"TestFStartStop",
"Test TDAL_HDMI_Start() and TDAL_HDMI_Stop() functions",
0, /* not interactive */
HDMI_TestFunctionalStartStop,
NULL
};


static tTestNode gTestFEnableDisable = {
"TestFEnableDisable",
"Test TDAL_HDMI_Enable() and TDAL_HDMI_Disable() functions",
0, /* not interactive */
HDMI_TestFunctionalEnableDisable,
NULL
};

static tTestNode gTestFInitStatus = {
"TestFInitStatus",
"Test TDAL_HDMI_InitStatus() function",
0, /* not interactive */
HDMI_TestFunctionalInitStatus,
NULL
};



static tTestNode gTestFHotplugDetect = {
"TestFHotPlugDetect",
"Test Auto detection of plugged screen",
1, /* Interactive need a screen, or analyzer to be connected */
HDMI_TestFunctionalHotplugDetect,
NULL
};



static tTestNode gTestFEDIDTest = {
"TestFEDIDTest",
"Test EDID Data",
1, /* Interactive need a screen, or analyzer to be connected */
HDMI_TestFunctionalEDIDTest,
NULL
};


static tTestNode gTestFHDCPTest = {
"TestFHDCPTest",
"Test HDCP encryption",
1, /* Interactive need a screen, or analyzer to be connected */
HDMI_TestFunctionalHDCPTest,
NULL
};


static tTestNode gTestFInfoFrameSPD = {
"TestFInfoFrameSPD",
"Test of sending SPD frames",
1, /* Interactive need a screen, or analyzer to be connected */
HDMI_TestFunctionalInfoFrameSPD,
NULL
};

static tTestNode gTestFInfoFrameVS = {
"TestFInfoFrameVS",
"Test of sending VS frames",
1, /* Interactive need a screen, or analyzer to be connected */
HDMI_TestFunctionalInfoFrameVS,
NULL
};


static tTestNode gTestFAudioVideo = {
"TestFAudioVideo",
"Test of audio and video",
1, /* Interactive need a screen, or analyzer to be connected */
HDMI_TestFunctionalAudioVideo,
NULL
};




tTestNode* gHDMIFunctionalTestNodeList[] = {
&gTestFInitTerm,
&gTestFStartStop,
&gTestFEnableDisable,
&gTestFInitStatus,
&gTestFHotplugDetect,
&gTestFEDIDTest,
&gTestFHDCPTest,
&gTestFInfoFrameSPD,
&gTestFInfoFrameVS,
&gTestFAudioVideo,
NULL
};



tTestNode gTDAL_HDMI_FunctionalTestSuite = {
"TDAL_HDMI Functional Tests",
"Test the TDAL_HDMI functionalities",
0, /* not interactive */
NULL, /* no function, it's a test suite */
(void**)gHDMIFunctionalTestNodeList
};






/***************************************
 * Some statics variables              *
 ***************************************/
static  tTDAL_HDMI_SourceId sourceId = eTDAL_HDMI_SOURCE_ID_0;/* Default value */
static  tTDAL_HDMI_Capability stCapability;
static  tTDAL_HDMI_SinkStatus stStatus;
static  tTDAL_HDMI_EDIDData stEDIDData;
static  tTDAL_HDMI_EDIDRawBuffer stEDIDBuffer;
static  tTDAL_HDMI_InfoFrameSPD stSPD;
static  tTDAL_HDMI_InfoFrameVS stVS;
static  tTDAL_HDMI_HDCPParams params;
static  uint8_t VS_Payload[] = {0}; /* Buffer for VS payload */

LOCAL bool AVIDisplayFormatForced = 0;

bool bWaitHdmiEvent=false;


/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestUnitary_InitTerm( void )
 *  \brief Test initialization and termination of HDMI driver
 */

void HDMI_TestFunctionalInitTerm(void)
{

    int i;
    /* Set sourceId to 0 by default */

    /*  FuncTest_HDMI_01 - 000 : Init terminate Three times */

    for (i = 0; i < 3; i++)
    {
        TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI init" );
        TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );
    }


    /*  FuncTest_HDMI_01 - 001 : Init twice and then terminate */
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_ALREADY_INIT_ERROR, "HDMI second init" );
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );



    /* FuncTest_HDMI_01 - 002 : Get capabilities */
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );
    TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(&stCapability), eTDAL_HDMI_NO_ERROR, "HDMI Get capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "HDMI capabilities : %d sources, EDID Parsing : %s\n",stCapability.NbSource, (stCapability.EDIDParsingCapable ? "available":"NOT available") ));
    /* don't forget to terminate */
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

}



/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalStartStop( void )
 *  \Brief Test of functions start and stop of HDMI driver
 */

void HDMI_TestFunctionalStartStop(void)
{

    int i;
    /* Set sourceId to 0 by default */

    TestHelper_OpenComponentsForTDAL_HDMI();

    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );




    /* FuncTest_HDMI_02 - 000 : Start Stop three times */
    for (i = 0; i < 3; i++)
    {
        TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );

        TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    }


    /*   FuncTest_HDMI_02 - 001 : Init twice and then terminate */
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI first Start" );
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI second Start" );
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );


    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}



/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalEnableDisable( void )
 *  \Brief Test of functions start and stop of HDMI driver
 */

void HDMI_TestFunctionalEnableDisable(void)
{
    int i;
    /* Set sourceId to 0 by default */


    TestHelper_OpenComponentsForTDAL_HDMI();

    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );


    /*   FuncTest_HDMI_03 - 000 : Enable disable HDCP three times */
    for (i = 0; i < 3; i++)
    {
        TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_NO_ERROR, "HDCP Enable" );
        TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_NO_ERROR, "HDCP Disable" );
    }

    /*   FuncTest_HDMI_03 - 001 :  Call HDCP enable twice */
    TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_NO_ERROR, "First call to HDCP Enable" );
    TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_NO_ERROR, "Second call to HDCP Enable" );
    TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_NO_ERROR, "HDCP Disable" );

    /*   FuncTest_HDMI_03 - 002 :  Call HDCP disable without enabling HDCP */
    TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_NO_ERROR, "HDCP Disable" );

    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}




/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalInitStatus( void )
 *  \Brief Test of functions that get receiver status
 */

void HDMI_TestFunctionalInitStatus(void)
{

    int i;
    sourceId = eTDAL_HDMI_SOURCE_ID_0;

    /* TDAL_HDMIInit() not done for this test !!!*/


    TestHelper_OpenComponentsForTDAL_HDMI();

    /*   FuncTest_HDMI_04 - 000 :  */
    TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(&stCapability), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI Get capabilities without init" );
    /*   FuncTest_HDMI_04 - 001 :  */
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI Start without init" );
    /*   FuncTest_HDMI_04 - 002 :  */
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI Stop without init" );
    /*   FuncTest_HDMI_04 - 003 :  */
    TestManager_AssertEqual( TDAL_HDMI_StatusGet(sourceId, &stStatus), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI status get without init" );
    /*   FuncTest_HDMI_04 - 005 :  */
    TestManager_AssertEqual( TDAL_HDMI_EDIDDataGet(sourceId, &stEDIDData), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI EDIDData get without init" );
    /*   FuncTest_HDMI_04 - 006 :  */
    TestManager_AssertEqual( TDAL_HDMI_EDIDRawBufferGet(sourceId, &stEDIDBuffer), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI EDIDData get without init" );
    /*   FuncTest_HDMI_04 - 007 :  */
    stSPD.uiVersion = SPD_VERSION;
    sprintf(stSPD.cVendorName,"%s",VENDOR_NAME);
    sprintf(stSPD.cProductDescription,"%s",DESCRIPTION);
    stSPD.eSrcDeviceInfo = SRC_DEVICE;

    TestManager_AssertEqual( TDAL_HDMI_InfoFrameSPDSet(sourceId, &stSPD), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI SPD info frame set without init" );
    /*   FuncTest_HDMI_04 - 008 :  */
    stVS.uiVersion = VS_VERSION;
    stVS.ucIEEERegistrationId[0] = IEEE_REGISTER_0;
    stVS.ucIEEERegistrationId[1] = IEEE_REGISTER_1;
    stVS.ucIEEERegistrationId[2] = IEEE_REGISTER_2;
    stVS.uiSpecificPayloadLength = PAYLOAD_LENGTH;
    stVS.pucSpecificPayload = VS_Payload;
    TestManager_AssertEqual( TDAL_HDMI_InfoFrameVSSet(sourceId, &stVS), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI VS info frame set without init" );
    /*   FuncTest_HDMI_04 - 009 :  */
    TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI HDCP enable without init" );
    /*   FuncTest_HDMI_04 - 010 :  */
    TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI HDCP disable without init" );
    /*   FuncTest_HDMI_04 - 011 :  */
    params.uiIV[0] = DUMMY_IV_0;
    params.uiIV[1] = DUMMY_IV_1;
    params.uiKSV[0] = DUMMY_KSV_0;
    params.uiKSV[1] = DUMMY_KSV_1;
    for (i = 0; i < 80; i++)
        params.uiDeviceKeys[i] = DUMMY_DEVICE_KEY; /* All keys are init'd to dummy values */
    TestManager_AssertEqual( TDAL_HDMI_HDCPParamSet(sourceId, &params), eTDAL_HDMI_NOT_INIT_ERROR, "HDMI HDCP param set without init" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}


/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalHotplugDetect( void )
 *  \Brief Test of function that detect the presence of a display terminal
 */


void HDMI_TestFunctionalHotplugDetect(void)
{
    tTDAL_HDMI_SinkStatus stStatus;   
    tTDAL_HDMI_Error Error = eTDAL_HDMI_NO_ERROR;
    TestHelper_OpenComponentsForTDAL_HDMI();
    /* FuncTest_HDMI_05-000 : Test of screen detection */
    TestHelper_HDMI_Init();
    /*Start HDMI */
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );

    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    bWaitHdmiEvent = true;
    TKEL_WaitSemaphoreTimeout(TDAL_HDMI_TST_NotifySem, ms2tick(10000) );
    bWaitHdmiEvent = false;


    Error = TDAL_HDMI_StatusGet(sourceId, &stStatus);
    TestManager_AssertEqual( Error , eTDAL_HDMI_NO_ERROR, "Get hdmi status" );
    TestManager_AssertEqual( (stStatus.EventState == eTDAL_HDMI_EVENT_NO_ENCRYPTION || stStatus.EventState == eTDAL_HDMI_EVENT_RECEIVER_CONNECTED ) , TRUE, "Test connected mode" );

    if (Error == eTDAL_HDMI_NO_ERROR && (stStatus.EventState == eTDAL_HDMI_EVENT_NO_ENCRYPTION || stStatus.EventState == eTDAL_HDMI_EVENT_RECEIVER_CONNECTED ))
    {

        mTBOX_PRINT( ( kTBOX_LF, "Disconnect the video terminal on HDMI output\n" ) );
        bWaitHdmiEvent = true;
        TKEL_WaitSemaphoreTimeout(TDAL_HDMI_TST_NotifySem, ms2tick(10000) );
        bWaitHdmiEvent = false;

        Error = TDAL_HDMI_StatusGet(sourceId, &stStatus);
        TestManager_AssertEqual( Error , eTDAL_HDMI_NO_ERROR, "Get hdmi status" );
        TestManager_AssertEqual(stStatus.EventState == eTDAL_HDMI_EVENT_NO_RECEIVER , TRUE, "Test no receiver mode");
    }

    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    TestHelper_HDMI_Term();
    TestHelper_CloseComponentsForTDAL_HDMI();
}


/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalEDIDTest( void )
 *  \Brief Test of functions EDID part
 */

void HDMI_TestFunctionalEDIDTest(void)
{
    tTDAL_HDMI_Error Error = eTDAL_HDMI_NO_ERROR;
    int i;
    uint8_t test;
    char *EventTimingName[]= { "eTDAL_HDMI_ESTABLISHED_TIMING_UNKNOWN",
        /* TIMING I */
        "eTDAL_HDMI_ESTABLISHED_TIMING_720_400_70",   
        "eTDAL_HDMI_ESTABLISHED_TIMING_720_400_88",   
        "eTDAL_HDMI_ESTABLISHED_TIMING_640_480_60", 
        "eTDAL_HDMI_ESTABLISHED_TIMING_640_480_67",
        "eTDAL_HDMI_ESTABLISHED_TIMING_640_480_72",
        "eTDAL_HDMI_ESTABLISHED_TIMING_640_480_75",
        "eTDAL_HDMI_ESTABLISHED_TIMING_800_600_56",
        "veTDAL_HDMI_ESTABLISHED_TIMING_800_600_60",
        /* TIMING II */
        "eTDAL_HDMI_ESTABLISHED_TIMING_800_600_72",
        "eTDAL_HDMI_ESTABLISHED_TIMING_800_600_75",
        "eTDAL_HDMI_ESTABLISHED_TIMING_832_624_75",
        "eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_87",
        "eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_60",
        "eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_70",
        "eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_75",
        "eTDAL_HDMI_ESTABLISHED_TIMING_1280_1024_75"};

    TestHelper_OpenComponentsForTDAL_HDMI();

/* FuncTest_HDMI_06-000 : EDID Raw Buffer*/
    TestHelper_HDMI_Init();
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    /*wait 10sec to connect the HDMI*/
    bWaitHdmiEvent = true;
    TKEL_WaitSemaphoreTimeout(TDAL_HDMI_TST_NotifySem, ms2tick(10000) );
    bWaitHdmiEvent = false;

    /*wait the EDID*/
    TKEL_DelayTask(ms2tick(3000));

    TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(&stCapability), eTDAL_HDMI_NO_ERROR, "HDMI Get capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "HDMI capabilities : %d sources, EDID Parsing : %s\n", stCapability.NbSource, (stCapability.EDIDParsingCapable ? "available":"NOT available") ));


    TestManager_AssertEqual( TDAL_HDMI_EDIDRawBufferGet(sourceId, &stEDIDBuffer), eTDAL_HDMI_NO_ERROR, "EDID Raw Data" );
    /* TODO : check parsing here need TDAL_HDMI to be available */  


/* FuncTest_HDMI_06-001 : EDID Data get */
    /* TODO Parsing according to TDAL_HDMI/driver functions */ 
    Error = TDAL_HDMI_EDIDDataGet(sourceId, &stEDIDData);
    TestManager_AssertEqual( Error , eTDAL_HDMI_NO_ERROR, "EDID Data Get" );

    if (Error == eTDAL_HDMI_NO_ERROR)
    {
        /*the E-DID standard (00h, FFh, FFh, FFh, FFh, FFh, FFh, 00h)*/
        test = 0x00;
        test |= stEDIDData.stEDIDBasic.ucHeader[0] | stEDIDData.stEDIDBasic.ucHeader[7];
        for (i = 1; i < 7; i++)
        {
            test |= ~stEDIDData.stEDIDBasic.ucHeader[i];
        }
        if (test == 0x00)
        {
            mTBOX_PRINT( ( kTBOX_LF, "\nheaderEDID BASIC Data found \n" ) );

            mTBOX_PRINT( ( kTBOX_LF, "\nVendor Product \n" ) );
            mTBOX_PRINT( ( kTBOX_LF, "Product Name : %3s\n", stEDIDData.stEDIDBasic.stVendorProductIdent.cIDManufacturerName ) );
            mTBOX_PRINT( ( kTBOX_LF, "Product code : %d\n", stEDIDData.stEDIDBasic.stVendorProductIdent.uiIDProductCode ) );
            mTBOX_PRINT( ( kTBOX_LF, "Optional product serial number : %d\n", stEDIDData.stEDIDBasic.stVendorProductIdent.uiIDSerialNumber ) );
            mTBOX_PRINT( ( kTBOX_LF, "Week:%d, Year:%d\n", stEDIDData.stEDIDBasic.stVendorProductIdent.uiWeekOfManufacture, stEDIDData.stEDIDBasic.stVendorProductIdent.uiYearOfManufacture ) );

            mTBOX_PRINT( ( kTBOX_LF, "\nVersion.Revision : %d.%d\n", stEDIDData.stEDIDBasic.ucVersion, stEDIDData.stEDIDBasic.ucRevision ) );

            mTBOX_PRINT( ( kTBOX_LF, "\nResolution\n") );
            mTBOX_PRINT( ( kTBOX_LF, "Timing 1 :  %s\n", EventTimingName[stEDIDData.stEDIDBasic.stEstablishedTimings.eTiming1] ) );
            mTBOX_PRINT( ( kTBOX_LF, "Timing 2 :  %s\n", EventTimingName[stEDIDData.stEDIDBasic.stEstablishedTimings.eTiming2] ) );        
        }
        else
        {
            mTBOX_PRINT( ( kTBOX_LF, " !!!! EDID BASIC Data Not found !!!!\n" ) );
        }
    }
    /* Don't forget to stop and terminate */
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    TestHelper_HDMI_Term();

    TestHelper_CloseComponentsForTDAL_HDMI();

}



/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalHDCPTest( void )
 *  \Brief Test of functions HDCPEnable and HDCPDisable
 */

void HDMI_TestFunctionalHDCPTest(void)
{
    int i;

    TestHelper_OpenComponentsForTDAL_HDMI();

    /* FuncTest_HDMI_07-000 to 07-002: HDCP Raw Buffer*/

    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    TKEL_DelayTask(ms2tick(5000)); /* Wait 5 sec ... TODO : replace by interactivity signal/callback */ 

    params.uiIV[0] = DUMMY_IV_0;
    params.uiIV[1] = DUMMY_IV_1;
    params.uiKSV[0] = DUMMY_KSV_0;
    params.uiKSV[1] = DUMMY_KSV_1;
    for (i = 0; i < 80; i++)
        params.uiDeviceKeys[i] = DUMMY_DEVICE_KEY; /* All keys are init'd to dummy values */
    TestManager_AssertEqual( TDAL_HDMI_HDCPParamSet(sourceId, &params), eTDAL_HDMI_NO_ERROR, "HDMI HDCP ParamSet" );

    TestManager_AssertEqual( TDAL_HDMI_HDCPEnable(sourceId), eTDAL_HDMI_NO_ERROR, "HDCP Enable" );

    TKEL_DelayTask(ms2tick(5000)); /* Wait 5 sec ... TODO : replace by interactivity signal/callback */ 

    TestManager_AssertEqual( TDAL_HDMI_HDCPDisable(sourceId), eTDAL_HDMI_NO_ERROR, "HDCP Disable" );

    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );


    TestHelper_CloseComponentsForTDAL_HDMI();
}




/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalInfoFrameSPD( void )
 *  \Brief Test of functions that send SPD info frames
 */

void HDMI_TestFunctionalInfoFrameSPD(void)
{

    TestHelper_OpenComponentsForTDAL_HDMI();

/* FuncTest_HDMI_08-000 */

	TestHelper_HDMI_Init();
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    bWaitHdmiEvent = true;
    TKEL_WaitSemaphoreTimeout(TDAL_HDMI_TST_NotifySem, ms2tick(10000) );
    bWaitHdmiEvent = false;

    TKEL_DelayTask(ms2tick(1000));

    stSPD.uiVersion = SPD_VERSION;
    sprintf(stSPD.cVendorName,"%s",VENDOR_NAME);
    sprintf(stSPD.cProductDescription,"%s",DESCRIPTION);

    mTBOX_PRINT( ( kTBOX_LF, "VendorName %s\n", stSPD.cVendorName ) );
    mTBOX_PRINT( ( kTBOX_LF, "ProductDescription %s\n", stSPD.cProductDescription ) );

    stSPD.eSrcDeviceInfo = SRC_DEVICE;
    TestManager_AssertEqual( TDAL_HDMI_InfoFrameSPDSet(sourceId, &stSPD), eTDAL_HDMI_NO_ERROR, "HDMI Infoframe Set SPD" );

    mTBOX_PRINT( ( kTBOX_LF, "wait 5 sec\n" ) );
    TKEL_DelayTask(ms2tick(5000));

    /* Don't forget to stop and terminate */
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}
/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalInfoFrameVS( void )
 *  \Brief Test of functions that send VS info frames
 */

void HDMI_TestFunctionalInfoFrameVS(void)
{

    TestHelper_OpenComponentsForTDAL_HDMI();

/* FuncTest_HDMI_08-001 */

    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    TKEL_DelayTask(ms2tick(5000)); /* Wait 5 sec ... TODO : replace by interactivity signal/callback */ 

    stVS.uiVersion = VS_VERSION;

    stVS.ucIEEERegistrationId[0] = IEEE_REGISTER_0;
    stVS.ucIEEERegistrationId[1] = IEEE_REGISTER_1;
    stVS.ucIEEERegistrationId[2] = IEEE_REGISTER_2;
    stVS.uiSpecificPayloadLength = PAYLOAD_LENGTH;
    stVS.pucSpecificPayload = VS_Payload;
    TestManager_AssertEqual( TDAL_HDMI_InfoFrameVSSet(sourceId, &stVS), eTDAL_HDMI_NO_ERROR, "HDMI InfoFrameVSSet" );

    /* Don't forget to stop and terminate */
    TestManager_AssertEqual( TDAL_HDMI_Stop(sourceId), eTDAL_HDMI_NO_ERROR, "HDMI Stop" );
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}


/******************************************************
 *              Functional tests                       *
 ******************************************************/

/*! \fn void HDMI_TestFunctionalAudioVideo( void )
 *  \Function that test audio and video result on an AV terminal/analyzer
 */

void HDMI_TestFunctionalAudioVideo(void)
{

    TestHelper_OpenComponentsForTDAL_HDMI();

    /* FuncTest_HDMI_09-000 : Audio video test */
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );
    TestManager_AssertEqual( TDAL_HDMI_Start(sourceId, DummyCallback, AVIDisplayFormatForced), eTDAL_HDMI_NO_ERROR, "HDMI Start" );
    mTBOX_PRINT( ( kTBOX_LF, "Connect a video terminal on HDMI output\n" ) );
    mTBOX_PRINT( ( kTBOX_LF, "Callback should be called\n" ) );

    TKEL_DelayTask(ms2tick(5000)); /* Wait 5 sec ... TODO : replace by interactivity signal/callback */ 

    TestManager_AssertEqual( TDAL_HDMI_CapabilityGet(&stCapability), eTDAL_HDMI_NO_ERROR, "HDMI Get capabilities" );
    mTBOX_PRINT(( kTBOX_LF, "HDMI capabilities : %d sources, EDID Parsing : %s\n",stCapability.NbSource, (stCapability.EDIDParsingCapable ? "available":"NOT available")));

    /* TODO Display video */
    /* */

    /* don't forget to terminate */
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );

    TestHelper_CloseComponentsForTDAL_HDMI();

}

#endif /* TDAL_HDMI_USE */

