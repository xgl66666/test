/** \file avtests_sce.c
 *  \brief Scenario Tests for the A/V layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 16/03/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/

#include <stdio.h>
#include <string.h>

#include "crules.h"

#include "tkel.h"
#include "tbox.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"
#include "tdal_ts.h"

#include "tdal_avs.h"
#include "tdal_dmd.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif
#include "stills.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "avtests.h"
#include "gfxtests.h"
#include "avtests_resources.h"


IMPORT int gChannel;
#ifdef TDAL_HDMI_USE
void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);
#endif

void MediaPlayerPlayFile();

void TestCase_ExecuteTestVideo(void);
void TestCase_ExecuteTestAudioConfig(void);
void TestCase_ExecuteTestVideoConfig(void);
void TestCase_ExecuteTestVideoAndAudio(void);
void TestCase_ExecuteTestVideoAndAudioHD(void);
void TestCase_ExecuteTestAudioVolume(void);
void TestCase_ExecuteTestAudioVolumeHeadphone(void);
void TestCase_ExecuteTestAudioEnableDisable(void);
void TestCase_ExecuteTestAudioEnableDisableHeadphone(void);
void TestCase_ExecuteTestAudioFromMemory(void);
void TestCase_ExecuteTestStillPicture(void);
void TestCase_ExecuteTestStillPictureHD(void);
void TestCase_ExecuteTestVideoAndStill(void);
void TestCase_ExecuteTestBeep(void);
void TestCase_ExecuteVideoInWindow(void);
void TestCase_ExecuteVideoAspect(void);
void TestCase_ExecuteUnitTestVideoInWindow(void);
void TestCase_ExecuteTestVideoOptimizeWindowDest(void);
void TestCase_ExecuteTestSetBackgroundColorRGB(void);
void TestCase_ExecuteTestSetBackgroundColorYCrCb(void);
void TestCase_ExecuteVideoEvent(void);
void TestCase_ExecuteTestZappingAsComediaSW(void);
#ifndef CIPLUSVALIDATOR
void TestCase_ExecuteTestVideoWithOSD(void);
void TestCase_ExecuteTestVideoToGraphics(void);
#endif
LOCAL void VideoEventsRegister(void);
LOCAL void VideoEventsUnregister(void);
static tTestNode gTestVideoAlone = {
    "TestVideoAlone",
    "test the video operations, without sound",
    0, /* not interactive */
    TestCase_ExecuteTestVideo,
        NULL
};

static tTestNode gTestVideoInWindow = {
    "TestVideoInWindow",
    "test the windowset and get operations",
    0, /* not interactive */
    TestCase_ExecuteVideoInWindow,
        NULL
};

static tTestNode gTestVideoAndAudio = {
    "TestVideoAndAudio",
    "test the video and audio operations",
    0, /* not interactive */
    TestCase_ExecuteTestVideoAndAudio,
        NULL
};

static tTestNode gTestVideoAndAudioHD = {
    "TestVideoAndAudioHD",
    "test the video and audio operations in HD",
    0, /* not interactive */
    TestCase_ExecuteTestVideoAndAudioHD,
        NULL
};

static tTestNode gTestAudioVolume = {
    "TestAudioVolume",
    "test the audio volume operations",
    0, /* not interactive */
    TestCase_ExecuteTestAudioVolume,
        NULL
};

static tTestNode gTestAudioVolumeHeadphone = {
    "TestAudioVolumeHeadphone",
    "test the audio volume headphone operations",
    0, /* not interactive */
    TestCase_ExecuteTestAudioVolumeHeadphone,
        NULL
};

static tTestNode gTestAudioEnableDisable = {
    "TestAudioEnableDisable",
    "test the audio eanble/disable operations",
    0, /* not interactive */
    TestCase_ExecuteTestAudioEnableDisable,
        NULL
};

static tTestNode gTestAudioEnableDisableHeadphone = {
    "TestAudioEnableDisableHeadphone",
    "test the audio eanble/disable headphone operations",
    0, /* not interactive */
    TestCase_ExecuteTestAudioEnableDisableHeadphone,
        NULL
};

static tTestNode gTestAudioFromMemory = {
    "TestCase_ExecuteTestAudioFromMemory",
    "test the audio memory operations",
    0, /* not interactive */
    TestCase_ExecuteTestAudioFromMemory,
    NULL
};

static tTestNode gTestVideoAndStill = {
    "TestVideoAndStill",
    "test mixed video and still picture operations",
    0, /* not interactive */
    TestCase_ExecuteTestVideoAndStill,
        NULL
};

static tTestNode gTestStillPicture = {
    "TestStillPicture",
    "test the still picture display",
    0, /* not interactive */
    TestCase_ExecuteTestStillPicture,
        NULL
};

static tTestNode gTestStillPictureHD = {
    "TestStillPictureHD_720",
    "test the still picture HD 720 display",
    0, /* not interactive */
    TestCase_ExecuteTestStillPictureHD,
        NULL
};

static tTestNode gTestBeep = {
    "TestBeep",
    "test the beep",
    0, /* not interactive */
    TestCase_ExecuteTestBeep,
        NULL
};

static tTestNode gTestVideoAspect = {
    "TestVideoAspect",
    "test the video aspect (4/3, 16/9, etc.)",
    0, /* not interactive */
    TestCase_ExecuteVideoAspect,
        NULL
};

static tTestNode gTestVideoEvent = {
    "TestVideoEvent",
    "test the video events",
    0, /* not interactive */
    TestCase_ExecuteVideoEvent,
        NULL
};

static tTestNode gTestZappingAsComediaSW = {
    "TestZappingAsComediaSW",
    "execute a zapping as comedia SW",
    0, /* not interactive */
    TestCase_ExecuteTestZappingAsComediaSW,
        NULL
};

#ifndef CIPLUSVALIDATOR
static tTestNode gTestVideoWithOSD = {
    "TestVideoWithOSD",
    "test video with OSD",
    0, /* not interactive */
    TestCase_ExecuteTestVideoWithOSD,
        NULL
};
static tTestNode gTestVideoToGraphics = {
    "TestVideoWithAlignedOSD",
    "test video with Aligned OSD",
    0, /* not interactive */
    TestCase_ExecuteTestVideoToGraphics,
        NULL
};
#endif

/*! \var tTestNode* gTDAL_AVTestNodeList[]
 *  \brief Node for A/V tests
 *
 *  This node references available tests for
 *  TDAL_AV tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_AV_ScenarioTestNodeList[] = {
    &gTestStillPicture,
    &gTestStillPictureHD,
    &gTestVideoAlone,
    &gTestVideoAndAudio,
    &gTestVideoAndAudioHD,
    &gTestAudioVolume,
    &gTestAudioVolumeHeadphone,
    &gTestAudioEnableDisable,
    &gTestAudioEnableDisableHeadphone,
    &gTestAudioFromMemory,
    &gTestVideoAndStill,
    &gTestVideoInWindow,
    &gTestVideoAspect,
    &gTestBeep,
    &gTestVideoEvent,
    &gTestZappingAsComediaSW,
#ifndef CIPLUSVALIDATOR
    &gTestVideoWithOSD,
	&gTestVideoToGraphics,
#endif
    NULL
};

static int gCodePass=0;
tTDAL_AV_Event gExpectedEvent;

/*! \var tTestNode gTDAL_AV_TestSuite
 * \brief Node for A/V test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_AVTestNodeList to know
 *  available A/V tests
 */
tTestNode gTDAL_AV_ScenarioTestSuite = {
    "TDAL_AV Scenarios",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_AV_ScenarioTestNodeList
};

/*! \fn void GenericTestPlayVideo( int bWithSound, int bWithStill, int HDChannel )
 *  \brief This function implements various scenario of "playing video" tests.
 *  \param[in] bWithSound To test with (1) or without (0) sound
 *  \param[in] bWithStill To test with (1) or without (0) still image
 */
void GenericTestPlayVideo( int bWithSound, int bWithStill, int HDChannel)
{
    tTDAL_AV_Error      aRC;
    tTDAL_AV_StreamType StreamType;
    tTDAL_AV_VideoPictureParams stPictureParams;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
	tTDAL_AV_VideoCapability        stVideoCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;
	struct TestManager_Channel *pChannel;

	if(HDChannel)
	{
		gChannel = 5;
	}
	else
	{
		gChannel = 0;
	}
	pChannel = TestHelper_GetChannel();

    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),
							kTDAL_DMX_NO_ERROR,
							"desc init");
#endif

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

	TestManager_AssertEqual(TDAL_GFX_Init(),
								  eTDAL_DISP_NO_ERROR,
								  "fgx init");	

	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							  "output init");

//#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
//#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );


	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	//if(stOutputCap.nbHDMI > 0)
	{
printf("\t\tif(stOutputCap.nbHDMI > 0)\n");
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	// allow denc
	//if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0,
																 eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc STD set");

		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
    //TestHelper_Configure_TDAL_AVS();
	
	TestManager_AssertEqual(TDAL_AV_VideoCapabilityGet(eTDAL_AV_DECODER_VIDEO_1,
													   &stVideoCapability),
						   eTDAL_AV_NO_ERROR,
						   "av video Capability get");

	TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_DIGITAL);

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));

    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    StreamType.videoType = pChannel->videoType; /*eTDAL_AV_VIDEO_TYPE_MPEG2*/;
    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"1st av start video");
/**/
    stInputWindow.Left       = 0;
    stInputWindow.Top        = 0;
    stInputWindow.Right      = pChannel->Width ;
    stInputWindow.Bottom     = pChannel->Height;
	
	TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0, &uiWidth, &uiHeight),
							eTDAL_DISP_NO_ERROR,
							"disp blender dimensions get" );

    stOutputWindow.Left      = 0;
    stOutputWindow.Top       = 0;
    stOutputWindow.Right     = uiWidth;
    stOutputWindow.Bottom    = uiHeight;

    mTBOX_PRINT((kTBOX_LF,"TDAL_DISP_LayerIOWindowsSet: In[%d,%d,%d,%d]\n",
				 stInputWindow.Left,
				 stInputWindow.Top,
				 stInputWindow.Right,
				 stInputWindow.Bottom));
    mTBOX_PRINT((kTBOX_LF,"TDAL_DISP_LayerIOWindowsSet: Out[%d,%d,%d,%d]\n",
				 stOutputWindow.Left,
				 stOutputWindow.Top,
				 stOutputWindow.Right,
				 stOutputWindow.Bottom));
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp window size set" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video enable" );






    if ( bWithSound )
    {
		TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																  &stAudioCapability),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio capability get");
		stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax;
		stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax;
		stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax;
		stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax;
		stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax;
		stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax;
		TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																		  &stAudioAttenuation),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio volume set");

		TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

        StreamType.audioType = pChannel->audioType; /*eTDAL_AV_AUDIO_TYPE_MPEG;*/
        TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
											  StreamType),
								eTDAL_AV_NO_ERROR,
								"1st av start audio");
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio enable" );
		
		if(stOutputCap.nbAudioAna > 1)
		{
			/* we suppose that when more than one audio ana, the second is a headphone */
			TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_HEADPHONE,
																	  &stAudioCapability),
									eTDAL_OUTPUT_NO_ERROR,
									"output audio headphone capability get");
			stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax;
			stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax;
			stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax;
			stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax;
			stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax;
			stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax;
			TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_HEADPHONE,
																			  &stAudioAttenuation),
									eTDAL_OUTPUT_NO_ERROR,
									"output audio headphone volume set");
			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
									eTDAL_OUTPUT_NO_ERROR,
									"output audio headphone enable" );
		}
    }

    TestManager_AskForChecking( "do you see video playing" );

    if( bWithSound )
    {
        TestManager_AskForChecking( "do you hear sound" );
    }
    else
    {
        TestManager_AskForChecking( "do you confirm there is no sound" );
    }

    /* add a zapping */
    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video disable" );
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"1st av stop video");
    TestManager_AssertEqual(TKEL_DelayTask(ms2tick(500)),
							TKEL_NO_ERR,
							"1st wait for 500ms");
    if ( bWithSound )
    {
        StreamType.audioType = pChannel->audioType; /*eTDAL_AV_AUDIO_TYPE_MPEG;*/
        TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
								eTDAL_AV_NO_ERROR,
								"1st av stop audio");
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio disable" );
    }

	if(HDChannel)
	{
		gChannel = 7;
	}
	else
	{
		gChannel = 1;
	}
	pChannel = TestHelper_GetChannel();

    stInputWindow.Left       = 0;
    stInputWindow.Top        = 0;
    stInputWindow.Right      = pChannel->Width ;
    stInputWindow.Bottom     = pChannel->Height;
	
	TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0, &uiWidth, &uiHeight),
							eTDAL_DISP_NO_ERROR,
							"disp blender dimensions get" );

    stOutputWindow.Left      = 0;
    stOutputWindow.Top       = 0;
    stOutputWindow.Right     = uiWidth;
    stOutputWindow.Bottom    = uiHeight;

    mTBOX_PRINT((kTBOX_LF,"TDAL_DISP_LayerIOWindowsSet: In[%d,%d,%d,%d]\n",
				 stInputWindow.Left,
				 stInputWindow.Top,
				 stInputWindow.Right,
				 stInputWindow.Bottom));
    mTBOX_PRINT((kTBOX_LF,"TDAL_DISP_LayerIOWindowsSet: Out[%d,%d,%d,%d]\n",
				 stOutputWindow.Left,
				 stOutputWindow.Top,
				 stOutputWindow.Right,
				 stOutputWindow.Bottom));
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp window size set" );

	TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    StreamType.videoType = pChannel->videoType; /*eTDAL_AV_VIDEO_TYPE_MPEG2;*/
    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"2nd av start video");

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"2nd disp layer video enable" );

    if ( bWithSound )
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio disable");
        StreamType.audioType = pChannel->audioType ; /*eTDAL_AV_AUDIO_TYPE_MPEG;*/
        TestManager_AssertEqual( TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,StreamType),
								 eTDAL_AV_NO_ERROR,
								 "2nd av start audio");
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"2nd output audio enable" );
    }

	  TestManager_AskForChecking( "do you see video playing" );

    if( bWithSound )
    {
        TestManager_AskForChecking( "do you hear sound" );
    }
    else
    {
        TestManager_AskForChecking( "do you confirm there is no sound" );
    }

    if( bWithStill )
    {
        TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"2nd disp layer Disable" );
        TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
								eTDAL_AV_NO_ERROR,
								"2nd av stop video");
        TestManager_AssertEqual(TKEL_DelayTask(ms2tick(500)),
								TKEL_NO_ERR,
								"2nd wait for 500ms");

        TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"3rd disp layer enable" );
        aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
										eTDAL_AV_PICTURE_MPEG,
										(uint8_t *)gStill1,
										(uint32_t) sizeof(gStill1),
										&stPictureParams);
        TestManager_AssertEqual(aRC,
								eTDAL_AV_NO_ERROR,
								"1st av video picture play");

        TestManager_AskForChecking( "do you see still picture" );

        TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
								eTDAL_AV_NO_ERROR,
								"1st av video picture stop" );
        TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"3rd disp layer Disable" );
        TestManager_AssertEqual(TKEL_DelayTask(ms2tick(500)),
								TKEL_NO_ERR,
								"3rd wait for 500ms");
		TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);
        StreamType.videoType = pChannel->videoType; /* eTDAL_AV_VIDEO_TYPE_MPEG2; */
        TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
											  StreamType ),
								eTDAL_AV_NO_ERROR,
								"3rd av start video");
    }

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"4th disp layer Disable" );
	
	  TestManager_AskForChecking( "do you see a black screen" );
    
    if( bWithSound )
    {
        TestManager_AskForChecking( "do you hear sound" );
    }
    else
    {
        TestManager_AskForChecking( "do you confirm there is no sound" );
    }


    if( bWithSound )
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"2nd output audio disable" );
    }

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"4th disp layer enable" );

	if(stVideoCapability.isFreezeSupported == TRUE)
	{
		TestManager_AssertEqual(TDAL_AV_VideoFreeze(eTDAL_AV_DECODER_VIDEO_1),
								eTDAL_AV_NO_ERROR,
								"1st av freeze");

		TestManager_AskForChecking( "do you see frozen video" );
	}

    if( bWithStill )
    {
        TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"5th disp layer Disable" );
        TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
								eTDAL_AV_NO_ERROR,
								"3rd av stop video");
        TestManager_AssertEqual(TKEL_DelayTask(ms2tick(500)),
								TKEL_NO_ERR,
								"wait for 500ms");

        TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"5th disp layer enable" );
        aRC = TDAL_AV_VideoPictureStart( eTDAL_AV_DECODER_VIDEO_1,
										 eTDAL_AV_PICTURE_MPEG,
										 (uint8_t *)gStill1,
										 (uint32_t)sizeof(gStill1),
										 &stPictureParams);
        TestManager_AssertEqual(aRC,
								eTDAL_AV_NO_ERROR,
								"av video picture play");

        TestManager_AskForChecking( "do you see still picture" );

        TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
								eTDAL_AV_NO_ERROR,
								"2nd av video picture stop" );
        TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"6th disp layer Disable" );
        TestManager_AssertEqual(TKEL_DelayTask(ms2tick(500)),
								TKEL_NO_ERR,
								"wait for 500ms");
    }

    if( bWithSound )
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"3rd output audio enable" );
    }

	if(stVideoCapability.isFreezeSupported == TRUE)
	{
		/*TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);*/
		StreamType.videoType = pChannel->videoType; /* eTDAL_AV_VIDEO_TYPE_MPEG2; */
		TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
											  StreamType),
								eTDAL_AV_NO_ERROR,
								"4th av start video");
		TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"6th disp layer enable" );
		TestManager_AskForChecking( "do you see video playing" );
	
		TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"7th disp layer disable" );
	}

    if( bWithSound )
    {
        TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
								eTDAL_AV_NO_ERROR,
								"av stop audio");
    }
	
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
								eTDAL_AV_NO_ERROR,
								"av stop video");
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");

    /* terminate tdal modules */
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate");

    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#endif
#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif

#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");
	gChannel = 0;
}

/*! \fn void TestCase_ExecuteTestVideo( void )
 *  \brief Execute a video test without sound and without still image
 *  \see GenericTestPlayVideo()
 */
void TestCase_ExecuteTestVideo()
{
    GenericTestPlayVideo( 0, 0, 0 );
}

/*! \fn void TestCase_ExecuteTestVideoAndAudio( void )
 *  \brief Execute a video test with sound and without still image
 *  \see GenericTestPlayVideo()
 */
void TestCase_ExecuteTestVideoAndAudio()
{
    GenericTestPlayVideo( 1, 0, 0 );
}

/*! \fn void TestCase_ExecuteTestVideoAndAudioHD( void )
 *  \brief Execute a video test with sound and without still image
 *  \see GenericTestPlayVideo()
 */
void TestCase_ExecuteTestVideoAndAudioHD()
{
    GenericTestPlayVideo( 1, 0, 1 );
}

/*! \fn void TestCase_ExecuteTestAudioVolume( void )
 *  \brief Execute a video test with sound and without still image
 *  \see GenericTestPlayVideo()
 */
void GenericTestAudioVolume(int headphone)
{
    uint32_t Volume;
    char    AskResult;
    tTDAL_AV_StreamType StreamType;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stCapability;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
    uint32_t VolumeMax;
	tTDAL_OUTPUT_OutputId         eAudOutput = eTDAL_OUTPUT_AUDIO_ANA_ID_0; /* default */

    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),
							kTDAL_DMX_NO_ERROR,
							"desc init");
#endif

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif
		TestManager_AssertEqual(TDAL_GFX_Init(),
									  eTDAL_DISP_NO_ERROR,
									  "fgx init");	
	
		TestManager_AssertEqual(TDAL_OUTPUT_Init(),
								  eTDAL_OUTPUT_NO_ERROR,
								  "output init");
	
	//#ifdef TDAL_HDMI_USE
		TestManager_AssertEqual(TDAL_HDMI_Init(),
								eTDAL_HDMI_NO_ERROR,
								"hdmi init" );
	//#endif
	
		TestManager_AssertEqual(TDAL_DISP_Init(),
								eTDAL_DISP_NO_ERROR,
								"disp init" );

	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(headphone)
	{
		if(stOutputCap.nbAudioAna > 1 )
		{
			/* we suppose that when more than one audio ana, the second is a headphone */
			eAudOutput = eTDAL_OUTPUT_AUDIO_HEADPHONE;
		}
		else
		{
			mTBOX_PRINT((kTBOX_LF,"!!!!!! No headphone output available !!!!!!\n"));

			/* terminate tdal modules */
			TestHelper_Terminate_TDAL_DMD();
			TestManager_AssertEqual(TDAL_DISP_Term(),
									eTDAL_AV_NO_ERROR,
									"disp terminate" );

#ifdef TDAL_HDMI_USE
			TestManager_AssertEqual(TDAL_HDMI_Term(),
									eTDAL_HDMI_NO_ERROR,
									"hdmi term" );
#endif

			TestManager_AssertEqual(TDAL_OUTPUT_Term(),
									eTDAL_AV_NO_ERROR,
									"output terminate" );

#ifdef AVS_STB
			TestManager_AssertEqual(TDAL_AVS_Term(),
									eTDAL_AVS_NO_ERROR,
									"avs terminate");
#endif

#ifdef PRODUCT_USE_CA
			TestManager_AssertEqual(TDAL_DESC_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"desc terminate");
#endif
			TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");
			return;		
		}
	}

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );

		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output HDMI video enable" );

	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init" );
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
	VideoEventsRegister();
	TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_DIGITAL);

    TestHelper_Configure_TDAL_AVS();
    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);

    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");


    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start video");
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output video enable" );
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer enable" );

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start audio");
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eAudOutput),
							eTDAL_OUTPUT_NO_ERROR,
							"output video enable" );
    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eAudOutput,
															  &stCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");
	stAttenuation.FrontRLAttenuation     = stCapability.AttenuationMax;
	stAttenuation.FrontCenterAttenuation = stCapability.AttenuationMax;
	stAttenuation.SurroundRLAttenuation  = stCapability.AttenuationMax;
	stAttenuation.LFEAttenuation         = stCapability.AttenuationMax;
	stAttenuation.RearRLAttenuation      = stCapability.AttenuationMax;
	stAttenuation.RearCenterAttenuation  = stCapability.AttenuationMax;
    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eAudOutput,
																	  &stAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");

    TestManager_AskForChecking("do you see video playing");
    TestManager_AskForChecking("do you hear sound");

    /* set audio volume */
    for (Volume = stCapability.AttenuationMax; Volume > 0; Volume-=1)
    {
		stAttenuation.FrontRLAttenuation     = Volume;
		stAttenuation.FrontCenterAttenuation = Volume;
		stAttenuation.SurroundRLAttenuation  = Volume;
		stAttenuation.LFEAttenuation         = Volume;
		stAttenuation.RearRLAttenuation      = Volume;
		stAttenuation.RearCenterAttenuation  = Volume;
        TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eAudOutput,
																		  &stAttenuation),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio volume set");
        mTBOX_PRINT((kTBOX_LF,"Current volume is %d. Press ENTER", Volume));
        mTBOX_GETCHAR(( &AskResult ));
        mTBOX_PRINT((kTBOX_LF,"\r\n"));
    }

    TestManager_AskForChecking("Did the volume decrease");

    /* set audio volume */
    if (stCapability.AttenuationMax % 2)
        VolumeMax = stCapability.AttenuationMax + 2;
    else
        VolumeMax = stCapability.AttenuationMax + 1;

    for (Volume = 0; Volume < VolumeMax; Volume+=2)
    {
		stAttenuation.FrontRLAttenuation     = Volume;
		stAttenuation.FrontCenterAttenuation = Volume;
		stAttenuation.SurroundRLAttenuation  = Volume;
		stAttenuation.LFEAttenuation         = Volume;
		stAttenuation.RearRLAttenuation      = Volume;
		stAttenuation.RearCenterAttenuation  = Volume;
        TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eAudOutput,
																		  &stAttenuation),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio volume set");
        mTBOX_PRINT((kTBOX_LF,"Current volume is %d. Press ENTER", Volume));
        mTBOX_GETCHAR(( &AskResult ));
        mTBOX_PRINT((kTBOX_LF,"\r\n"));
    }
    TestManager_AskForChecking("Did the volume increase");

    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output video disable" );
    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer disable" );

    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av stop video");
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av stop audio");
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate" );

    /* terminate tdal modules */
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate" );
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_AV_NO_ERROR,
							"disp terminate" );

#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi term" );
#endif

    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_AV_NO_ERROR,
							"output terminate" );

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif

#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");
}

/*! \fn void TestCase_ExecuteTestAudioVolume( void )
 *  \brief Execute a video test with sound and without still image
 *  \see
 */
void TestCase_ExecuteTestAudioVolume()
{
	GenericTestAudioVolume(0);
}

/*! \fn void TestCase_ExecuteTestAudioVolumeHeadphone( void )
 *  \brief Execute a video test with sound and without still image
 *  \see
 */
void TestCase_ExecuteTestAudioVolumeHeadphone()
{
	GenericTestAudioVolume(1);
}

/*-------------------------------------------------------------------------
 * Function : ChalTest_ReadFileToBuffer
 * Input    : char *Filename, unsigned char **Buffer_pp, long int *BufferSize_p
 * Output   :
 * Return   : True if error
 * ----------------------------------------------------------------------*/
bool ChalTest_ReadFileToBuffer( char *Filename, unsigned char **Buffer_pp, long int *BufferSize_p )
{

    long int size;
    TKEL_err tkel_Err;
    /*arv: struct stat file_descriptor;*/
    tTBOX_File file;

    file = TBOX_FileOpen(Filename,eTBOX_READ);

    if (!file)
    {
        mTBOX_PRINT((kTBOX_LF,"Injector : unable to open filename [%s]\n",
					 Filename));
        return FALSE;
    }
    else
    {
    	*BufferSize_p = (long int)TBOX_FileGetSize( file );
    	if(*BufferSize_p > 0)
    	{
    		tkel_Err = TKEL_Malloc((TKEL_msize)*BufferSize_p, (void **)Buffer_pp);
    		if ( tkel_Err != TKEL_NO_ERR )
    		{
    			mTBOX_PRINT((kTBOX_LF,"unable to allocate memory\n"));
    			TBOX_FileClose(file);
    			return FALSE;
    		}
    		mTBOX_PRINT((kTBOX_LF, "loading <%s>.. %d bytes\n", Filename, *BufferSize_p));

    		size = TBOX_FileRead( file , *Buffer_pp , (size_t) *BufferSize_p );

    		if (size != *BufferSize_p)
    		{
    			mTBOX_PRINT((kTBOX_LF,"Error: Read %ld bytes, expected %ld\n", size, *BufferSize_p));
    			TBOX_FileClose( file);
    			return FALSE;
    		}
    		TBOX_FileClose(file);
    	}
    } /* end of file opened */

    return TRUE;

} /* end ChalTest_ReadFileToBuffer */

/*! \fn void TestCase_ExecuteTestAudioFromMemory( void )
 *  \brief Execute a Audio test with sound from memory
 *  \
 */
void TestCase_ExecuteTestAudioFromMemory()
{
    uint8_t                 *Buffer;
    long int                BufferSize;
    tTDAL_AV_SampleType     sampleType;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;


#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_DMX_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"dmx init" );

	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
															  &stAudioCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");
	stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax;
	stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax;
	stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax;
	stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax;
	stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax;
	stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax;
	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																	  &stAudioAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio enable" );
    TestHelper_Configure_TDAL_AVS();
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    Buffer = asample_mpg;
    BufferSize = asample_mpg_len;

    sampleType = eTDAL_AV_SAMPLE_MPEG1;
    TestManager_AssertEqual(TDAL_AV_AudioSampleStart(eTDAL_AV_DECODER_AUDIO_1,
                                                     sampleType,
                                                     Buffer,
                                                     (uint32_t)BufferSize,
                                                     1),
							eTDAL_AV_NO_ERROR,
							"av sample audio start ");

    TestManager_AssertEqual(TKEL_DelayTask(ms2tick(5000)),
							TKEL_NO_ERR,
							"wait for 5s");
    TestManager_AskForChecking( "do you hear mpeg sound");

    TestManager_AssertEqual(TDAL_AV_AudioSampleStop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av sample audio stop ");

    /* play a mp3 file */

    Buffer = sample_1_mp3;
    BufferSize = sample_1_mp3_len;

    sampleType = eTDAL_AV_SAMPLE_MP3;
    TestManager_AssertEqual(TDAL_AV_AudioSampleStart(eTDAL_AV_DECODER_AUDIO_1,
                                                     sampleType,
                                                     Buffer,
                                                     (uint32_t)BufferSize,
                                                     1),
							eTDAL_AV_NO_ERROR,
							"av sample audio start ");
    TestManager_AssertEqual(TKEL_DelayTask(ms2tick(10000)),
							TKEL_NO_ERR,
							"wait for 10s");
    TestManager_AskForChecking( "do you hear mp3 sound");
    TestManager_AssertEqual(TDAL_AV_AudioSampleStop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av sample audio stop ");

    /* re play mp3 file' */

    Buffer = sample_2_mp3;
    BufferSize = sample_2_mp3_len;

    sampleType = eTDAL_AV_SAMPLE_MP3;
    TestManager_AssertEqual(TDAL_AV_AudioSampleStart(eTDAL_AV_DECODER_AUDIO_1,
                                                     sampleType,
                                                     Buffer,
                                                     (uint32_t)BufferSize,
                                                     2), eTDAL_AV_NO_ERROR, "av sample audio start ");
    TestManager_AssertEqual(TKEL_DelayTask(ms2tick(10000)),
							TKEL_NO_ERR,
							"wait for 10s");
    TestManager_AskForChecking( "do you hear mp3 sound 2times");
    TestManager_AssertEqual(TDAL_AV_AudioSampleStop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av sample audio stop ");

    TestHelper_Unconfigure_TDAL_DMX();
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate" );

    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_AV_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_AV_NO_ERROR,
							"output terminate" );
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							eTDAL_AV_NO_ERROR,
							"dmx terminate" );

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif


}

void GenericTestAudioEnableDisable(int headphone)
{
    tTDAL_AV_StreamType StreamType;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stCapability;
    tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
    tTDAL_OUTPUT_Capability       stOutputCap;
	tTDAL_OUTPUT_OutputId         eAudOutput = eTDAL_OUTPUT_AUDIO_ANA_ID_0; /* default */

    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),
							kTDAL_DMX_NO_ERROR,
							"desc init");
#endif

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
							
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif

	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");
	if(headphone)
	{
		if(stOutputCap.nbAudioAna > 1 )
		{
			/* we suppose that when more than one audio ana, the second is a headphone */
			eAudOutput = eTDAL_OUTPUT_AUDIO_HEADPHONE;
		}
		else
		{
			mTBOX_PRINT((kTBOX_LF,"!!!!!! No headphone output available !!!!!!\n"));

			/* terminate tdal modules */
			TestHelper_Terminate_TDAL_DMD();
			TestManager_AssertEqual(TDAL_DISP_Term(),
									eTDAL_AV_NO_ERROR,
									"disp terminate" );

#ifdef TDAL_HDMI_USE
			TestManager_AssertEqual(TDAL_HDMI_Term(),
									eTDAL_HDMI_NO_ERROR,
									"hdmi term" );
#endif

			TestManager_AssertEqual(TDAL_OUTPUT_Term(),
									eTDAL_AV_NO_ERROR,
									"output terminate" );

#ifdef AVS_STB
			TestManager_AssertEqual(TDAL_AVS_Term(),
									eTDAL_AVS_NO_ERROR,
									"avs terminate");
#endif

#ifdef PRODUCT_USE_CA
			TestManager_AssertEqual(TDAL_DESC_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"desc terminate");
#endif
			TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");
			return;		
		}
	}

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init" );
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
	TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_DIGITAL);

    TestHelper_Configure_TDAL_AVS();
    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);

    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start video");
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer enable" );

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start audio");
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eAudOutput),
							eTDAL_OUTPUT_NO_ERROR,
							"output video enable" );
    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eAudOutput,
															  &stCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");
	stAttenuation.FrontRLAttenuation     = stCapability.AttenuationMax;
	stAttenuation.FrontCenterAttenuation = stCapability.AttenuationMax;
	stAttenuation.SurroundRLAttenuation  = stCapability.AttenuationMax;
	stAttenuation.LFEAttenuation         = stCapability.AttenuationMax;
	stAttenuation.RearRLAttenuation      = stCapability.AttenuationMax;
	stAttenuation.RearCenterAttenuation  = stCapability.AttenuationMax;
    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eAudOutput,
																	  &stAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");
    TestManager_AskForChecking("do you see video playing");
    TestManager_AskForChecking("do you hear sound");

    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eAudOutput),
                            eTDAL_OUTPUT_NO_ERROR,
                            "output audio disable" );
    TestManager_AskForChecking("Is the audio disabled");

    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eAudOutput),
                            eTDAL_OUTPUT_NO_ERROR,
                            "output audio enable" );
    TestManager_AskForChecking("Is the audio enabled");


    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer enable" );

    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eAudOutput),
                            eTDAL_OUTPUT_NO_ERROR,
                            "output audio disable" );


    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av stop video");
    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av stop audio");
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate" );

    /* terminate tdal modules */
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate" );
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_AV_NO_ERROR,
							"disp terminate" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi term" );
#endif

    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_AV_NO_ERROR,
							"output terminate" );

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif

#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");
}

/*! \fn void TestCase_ExecuteTestAudioEnableDisable( void )
 *  \brief Execute a video test with sound and without still image
 *  \see GenericTestPlayVideo()
 */
void TestCase_ExecuteTestAudioEnableDisable()
{
	GenericTestAudioEnableDisable(0);
}

/*! \fn void TestCase_ExecuteTestAudioEnableDisableHeadphone( void )
 *  \brief Execute a video test with sound and without still image
 *  \see GenericTestPlayVideo()
 */
void TestCase_ExecuteTestAudioEnableDisableHeadphone()
{
	GenericTestAudioEnableDisable(1);
}

/*! \fn void TestCase_ExecuteTestVideoAndStill( void )
 *  \brief Execute a video test without sound and with still image
 *  \see GenericTestPlayVideo()
 */
void TestCase_ExecuteTestVideoAndStill()
{
    GenericTestPlayVideo( 0, 1, 0 );
}

/*! \fn void TestCase_ExecuteTestStillPicture( void )
 *  \brief This function tests stills picture.
 */
void TestCase_ExecuteTestStillPicture()
{
    tTDAL_AV_Error aRC;
#if defined(TDAL_MEDIA_VIEWER)
    bool                    AskResult;
    char                    filename[128];
    uint8_t                 *Jpeg_Buffer;
    long int                Jpeg_BufferSize;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Color         PaletteColor;
#endif
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256];
    tTDAL_AV_VideoPictureParams stPictureParams;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;

    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif
	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    TestHelper_Configure_TDAL_AVS();
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
#if defined(TDAL_MEDIA_VIEWER)
    /*show a rgb picture */
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output enable" );
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp enable" );

    TestManager_AssertEqual(TDAL_GFX_Init(),
							eTDAL_GFX_NO_ERROR,
							"gfx init" );

    strncpy(filename, TDAL_AV_TESTDATA_PATH "troll_400x534.raw", sizeof(filename) );
    AskResult =  ChalTest_ReadFileToBuffer(filename,
										   &Jpeg_Buffer,
										   &Jpeg_BufferSize );
    TestManager_AssertEqual( AskResult, TRUE, "ChalTest_ReadFileToBuffer ");

    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 400;
    stInputWindow.Bottom = 534;

    stOutputWindow.Left   = 100;
    stOutputWindow.Top    = 50;
    stOutputWindow.Right  = stInputWindow.Right + stOutputWindow.Left -1;
    stOutputWindow.Bottom = stInputWindow.Bottom + stOutputWindow.Top -1;
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set IOWindows" );

    stPictureParams.notCompressWidth            = stInputWindow.Right;
    stPictureParams.notCompressHeight           = stInputWindow.Bottom;
    stPictureParams.notCompressDataBufferPosX   = stInputWindow.Left;
    stPictureParams.notCompressDataBufferPosY   = stInputWindow.Top;
    stPictureParams.notCompressDataBufferWidth  = stPictureParams.notCompressWidth;
    stPictureParams.notCompressDataBufferHeight = stPictureParams.notCompressWidth;

    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_RGB,
									(uint8_t *)Jpeg_Buffer,
									(uint32_t)Jpeg_BufferSize,
									&stPictureParams );
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );
    TestManager_AskForChecking( "do you see the RGB picture " );

    TestManager_AssertEqual(TDAL_GFX_CapabilitiesGet(&Capabilities),
							eTDAL_GFX_NO_ERROR,
							"GFX get capabilities" );

    /* Set up the palette */
    TestHelper_GFX_FillPalette(&Palette);

    /* Set up default params & create a GFX region */
    Params.offset.x    = 200;
    Params.offset.y    = 200;
    Params.size.width  = 400;
    Params.size.height = 300;
    Params.refSize.width  = 720;
    Params.refSize.height = 576;
    Params.colorType   = Palette.colorType;
    Params.alpha       = 0;
    TestManager_AssertEqual(TDAL_GFX_RegionCreate(&GfxHandle,
												  &Params),
							eTDAL_GFX_NO_ERROR,
							"Create a GFX region" );
    TestManager_AssertEqual(TDAL_GFX_RegionShow(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Show region" );

    TestManager_AssertEqual(TDAL_GFX_RegionPaletteSet(GfxHandle,
													  &Palette),
							eTDAL_GFX_NO_ERROR,
							"Set the palette" );

    /* Draw 5 valid rectangles */
    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    TestManager_AskForChecking("do you see rectangle" );
    TestManager_AssertEqual(TDAL_GFX_RegionHide(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Hide region" );

    stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = 719;
    stOutputWindow.Bottom = 575;
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set IOWindows" );

    TestManager_AskForChecking("do you see the RGB picture in full screen " );

    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av video picture stop" );

    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 719;
    stInputWindow.Bottom = 575;
    stOutputWindow       = stInputWindow;
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set IOWindows" );

	TKEL_Free(Jpeg_Buffer);
#endif

    if(stOutputCap.nbVideoHDDAC > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
    }

#ifdef TDAL_HDMI_USE
    if(stOutputCap.nbHDMI > 0)
    {
        TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
    }
#endif

    if(stOutputCap.nbVideoDenc > 0)
    {
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output denc video enable" );
    }

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp enable" );

    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_MPEG,
									(uint8_t *)gStill1,
									(uint32_t)sizeof(gStill1),
									&stPictureParams);
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );
    TestManager_AskForChecking("do you see still picture" );
    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av video picture stop" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp Disable" );
    TestManager_AskForChecking("do you see black screen" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp Enable" );
    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_MPEG,
									(uint8_t *)gStill2,
									(uint32_t)sizeof(gStill2),
									&stPictureParams );
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );
    TestManager_AskForChecking("do you see another still picture");
    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av video picture stop" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp Enable" );
    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_MPEG,
									(uint8_t *)gStill1,
									(uint32_t)sizeof(gStill1),
									&stPictureParams );
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );
    TestManager_AskForChecking("do you see the first still picture again" );
    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av video picture stop" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp Disable" );

    if(stOutputCap.nbVideoDenc > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output Disable" );
    }

    if(stOutputCap.nbVideoHDDAC > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac disable");
    }

#ifdef TDAL_HDMI_USE
    if(stOutputCap.nbHDMI > 0)
    {
        TestManager_AssertEqual(TDAL_HDMI_Stop(eTDAL_HDMI_SOURCE_ID_0),
								eTDAL_HDMI_NO_ERROR,
								"hdmi stop" );
    }
#endif
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_AV_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_AV_NO_ERROR,
							"output terminate" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#endif
#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif

}

/*! \fn void TestCase_ExecuteTestStillPictureHD( void )
 *  \brief This function tests stills picture.
 */
void TestCase_ExecuteTestStillPictureHD()
{
    tTDAL_AV_Error aRC;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256];
    tTDAL_AV_VideoPictureParams stPictureParams;
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;

    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif
	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    TestHelper_Configure_TDAL_AVS();
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");

    VideoEventsRegister();
    if(stOutputCap.nbVideoHDDAC > 0)
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
    }

#ifdef TDAL_HDMI_USE
    if(stOutputCap.nbHDMI > 0)
    {
        TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
    }
#endif

    if(stOutputCap.nbVideoDenc > 0)
    {
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output denc video enable" );
    }

    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 1279;
    stInputWindow.Bottom = 719;
    stOutputWindow.Left   = 0;
    stOutputWindow.Top     = 0;
    stOutputWindow.Right  = 1919;
    stOutputWindow.Bottom = 1079;
    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"disp set IOWindows" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp enable" );

    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_MPEG,
									(uint8_t *)gStill3,
									(uint32_t)sizeof(gStill3),
									&stPictureParams);
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );

    TestManager_AskForChecking("do you see still picture" );

    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av video picture stop" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp Disable" );
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_AV_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_AV_NO_ERROR,
							"output terminate" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#endif
#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
#endif

}

/*! \fn void TestCase_ExecuteTestBeep( void )
 *  \brief Execute a beep test
 */
void TestCase_ExecuteTestBeep()
{
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    TestManager_AskForChecking("do you hear beep" );

    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");
}

/*! \fn void TestCase_ExecuteVideoInWindow( void )
 *  \brief This function tests windowset and get methods
 */
void TestCase_ExecuteVideoInWindow()
{
    tTDAL_DISP_Error aRC;
    tTDAL_AV_StreamType     StreamType;
    tTDAL_DISP_LayerWindow  InputWindow;
    tTDAL_DISP_LayerWindow  OutputWindow;
    tTDAL_DISP_LayerWindow  RefInputWindow;
    tTDAL_DISP_LayerWindow  RefOutputWindow;
    tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
    tTDAL_OUTPUT_Capability       stOutputCap;

    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif
	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
    TestHelper_Configure_TDAL_AVS();
    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);

    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start video");
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer enable" );
    /* add audio playback */
    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start audio");
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output video enable" );
    /* add audio playback */
    TestManager_AskForChecking("do you see video" );

    /* error cases */
    aRC = TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
									  &RefInputWindow,
									  &RefOutputWindow);
    aRC = TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
									  &InputWindow,
									  &OutputWindow);
    TestManager_AssertEqual(aRC,
							eTDAL_DISP_NO_ERROR,
							"disp window size get");

    /* good cases */
    OutputWindow.Left       = RefOutputWindow.Left;
    OutputWindow.Top        = RefOutputWindow.Top;
    OutputWindow.Right      = RefOutputWindow.Left + (RefOutputWindow.Right / 2);
    OutputWindow.Bottom     = RefOutputWindow.Top + (RefOutputWindow.Bottom / 2);

    mTBOX_PRINT((kTBOX_LF,"TDAL_DISP_LayerIOWindowsSet: (OutRef [%d,%d] [%d,%d]) Out [%d,%d] [%d,%d]\n", RefOutputWindow.Left, RefOutputWindow.Top, RefOutputWindow.Right, RefOutputWindow.Bottom, OutputWindow.Left, OutputWindow.Top, OutputWindow.Right, OutputWindow.Bottom));
    aRC = TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0, &InputWindow, &OutputWindow);
    TestManager_AssertEqual(aRC, eTDAL_DISP_NO_ERROR, "disp window size set" );
    TestManager_AskForChecking( "do you see video in the top/left corner (1/4 PAL size)" );

    aRC = TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0, &InputWindow, &OutputWindow);
    TestManager_AssertEqual(aRC, eTDAL_DISP_NO_ERROR, "disp window size get" );
    TestManager_AssertEqual(memcmp(&OutputWindow,
								   &RefOutputWindow,
								   sizeof(tTDAL_DISP_LayerWindow) == 0),
							eTDAL_DISP_NO_ERROR,
							"disp window size get");

    OutputWindow.Left       = RefOutputWindow.Left + (RefOutputWindow.Right / 2);
    OutputWindow.Top        = RefOutputWindow.Top;
    OutputWindow.Right      = RefOutputWindow.Right;
    OutputWindow.Bottom     = RefOutputWindow.Top + (RefOutputWindow.Bottom / 2);
    aRC = TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0, &InputWindow, &OutputWindow);
    TestManager_AssertEqual(aRC, eTDAL_DISP_NO_ERROR, "disp window size set");
    TestManager_AskForChecking("do you see video in the top/right corner (1/4 PAL size)" );

    OutputWindow.Left       = RefOutputWindow.Left;
    OutputWindow.Top        = RefOutputWindow.Top + (RefOutputWindow.Bottom / 2);
    OutputWindow.Right      = RefOutputWindow.Left + (RefOutputWindow.Right / 2);
    OutputWindow.Bottom     = RefOutputWindow.Bottom;
    aRC = TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0, &InputWindow, &OutputWindow);
    TestManager_AssertEqual(aRC, eTDAL_DISP_NO_ERROR, "disp window size set" );
    TestManager_AskForChecking("do you see video in the bottom/left corner (1/4 PAL size)" );

    OutputWindow.Left       = RefOutputWindow.Left + (RefOutputWindow.Right / 2);
    OutputWindow.Top        = RefOutputWindow.Top + (RefOutputWindow.Bottom / 2);
    OutputWindow.Right      = RefOutputWindow.Right;
    OutputWindow.Bottom     = RefOutputWindow.Bottom;
    aRC = TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0, &InputWindow, &OutputWindow);
    TestManager_AssertEqual(aRC, eTDAL_DISP_NO_ERROR, "disp window size set" );
    TestManager_AskForChecking("do you see video in the bottom/right corner (1/4 PAL size)" );

    OutputWindow.Left       = RefOutputWindow.Left + (RefOutputWindow.Right / 4);
    OutputWindow.Top        = RefOutputWindow.Top + (RefOutputWindow.Bottom / 4);
    OutputWindow.Right      = RefOutputWindow.Left + (3 * RefOutputWindow.Right / 4);
    OutputWindow.Bottom     = RefOutputWindow.Top + (3 * RefOutputWindow.Bottom / 4);
    aRC = TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0, &InputWindow, &OutputWindow);
    TestManager_AssertEqual(aRC, eTDAL_DISP_NO_ERROR, "disp window size set" );
    TestManager_AskForChecking("do you see video in the middle of the screen (1/4 PAL size)" );

    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
                            eTDAL_AV_NO_ERROR,
                            "av stop audio");

    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
                                eTDAL_AV_NO_ERROR,
                                "av stop video");

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0), eTDAL_DISP_NO_ERROR, "disp layer disable" );
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");

    /* terminate tdal modules */
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),eTDAL_TSROUTE_NO_ERROR,"tsroute terminate");
    TestManager_AssertEqual(TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),eTDAL_OUTPUT_NO_ERROR, "output terminate" );

#ifdef TDAL_HDMI_USE
    if(stOutputCap.nbHDMI > 0)
    {
        TestManager_AssertEqual(TDAL_HDMI_Stop(eTDAL_HDMI_SOURCE_ID_0),
                                eTDAL_HDMI_NO_ERROR,
                                "hdmi stop" );
    }

    TestManager_AssertEqual(TDAL_HDMI_Term(),
                                eTDAL_HDMI_NO_ERROR,
                                "hdmi term" );
#endif

#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Term(), eTDAL_AVS_NO_ERROR, "avs terminate");
#endif

    TestManager_AssertEqual(TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "dmx terminate");
}

/*! \fn void TestCase_ExecuteVideoAspect( void )
 *  \brief This function tests changes of aspect ratio.
 */
void TestCase_ExecuteVideoAspect()
{
}

#ifndef CIPLUSVALIDATOR
void TestCase_ExecuteTestVideoWithOSD(void)
{
    tTDAL_AV_StreamType StreamType;
    tTDAL_AV_VideoPictureParams stPictureParams;
    tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
    tTDAL_OUTPUT_Capability       stOutputCap;
    tTDAL_GFX_Capabilities  Capabilities;
    tTDAL_GFX_RegionHandle  GfxHandle;
    uint32_t     LayerHandle;
    tTDAL_GFX_RegionConfig  Params;
    tTDAL_GFX_Color         PaletteColor;
    tTDAL_GFX_Palette       Palette;
    tTDAL_GFX_Color         PaletteColorTab[256];
    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];
	uint32_t Width, Height;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;



    /* Initialize tdal modules */
    TestHelper_Initialize_TDAL_DMD();
    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Init(),
							kTDAL_DMX_NO_ERROR,
							"desc init");
#endif

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif
    TestManager_AssertEqual(TDAL_GFX_Init(),
							eTDAL_GFX_NO_ERROR,
							"gfx init" );

	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );

		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");

	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(), eTDAL_AVS_NO_ERROR, "avs init");
#endif

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));

	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
															  &stAudioCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");
	stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax;
	stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax;
	stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax;
	stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax;
	stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax;
	stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax;
	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																	  &stAudioAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");

    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							" av start audio");
    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							" output audio enable" );

    TestManager_AskForChecking( "do you hear sound" );

     StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

	    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"av start video");

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							" disp layer video enable" );

    TestManager_AskForChecking( "do you see video playing" );


    TestManager_AssertEqual(TDAL_GFX_CapabilitiesGet(&Capabilities),
							eTDAL_GFX_NO_ERROR,
							"GFX get capabilities" );


    /* Set up the palette */
    TestHelper_GFX_FillPalette(&Palette);

    TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0,
														   &Width,
														   &Height),
							eTDAL_DISP_NO_ERROR,
							"DISP get blender dimensions" );

	stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = Width-1;
    stInputWindow.Bottom  = Height-1;
	stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width-1;
    stOutputWindow.Bottom = Height-1;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"GFX set image IOWindows" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp GFX layer enable" );


    /* Set up default params & create a GFX region */
    Params.offset.x    = 200;
    Params.offset.y    = 200;
    Params.size.width  = 400;
    Params.size.height = 300;
    Params.refSize.width  = 720;
    Params.refSize.height = 576;
    Params.colorType   = Palette.colorType;
    Params.alpha       = 0xff;
    TestManager_AssertEqual(TDAL_GFX_RegionCreate(&GfxHandle,
												  &Params),
							eTDAL_GFX_NO_ERROR,
							"Create a GFX region" );

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0, &LayerHandle),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet" );

    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
												  		    LayerHandle),
							eTDAL_GFX_NO_ERROR,
							"TDAL_GFX_RegionDestinationSet" );

    TestManager_AssertEqual(TDAL_GFX_RegionShow(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Show region" );

    TestManager_AssertEqual(TDAL_GFX_RegionPaletteSet(GfxHandle,
													  &Palette),
							eTDAL_GFX_NO_ERROR,
							"Set the palette" );

    /* Draw valid rectangle that fill the region */
    Params.offset.x    = 0;
    Params.offset.y    = 0;
    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    TestManager_AskForChecking("do you see rectangle" );
    TestManager_AskForChecking("video is still OK?" );
    TestManager_AssertEqual(TDAL_GFX_RegionHide(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Hide region" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp video layer disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp GFX layer disable" );


    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							" output audio disable" );


    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av stop video");

    TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
						eTDAL_AV_NO_ERROR,
						"av stop audio");



    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");

    /* terminate tdal modules */
    TestHelper_Terminate_TDAL_DMD();
    TestHelper_Unconfigure_TDAL_DMX();
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate");

    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#endif
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");
}

void TestCase_ExecuteTestVideoToGraphics(void)
{
    tTDAL_AV_VideoPictureParams 	stPictureParams;
    tTDAL_OUTPUT_VideoHDDACParams 	stHDDACParams;
    tTDAL_OUTPUT_Capability       	stOutputCap;
    tTDAL_GFX_Capabilities  		Capabilities;
    tTDAL_GFX_RegionHandle  		GfxHandle;
    uint32_t     					LayerHandle;
    tTDAL_GFX_RegionConfig  		Params;
    tTDAL_GFX_Color        			PaletteColor;
    tTDAL_GFX_Palette       		Palette;
    tTDAL_GFX_Color         		PaletteColorTab[256];
	uint32_t 						Width, Height;
    tTDAL_DISP_LayerWindow  		stInputWindow;
    tTDAL_DISP_LayerWindow  		stOutputWindow;
	tTDAL_AV_Error      			aRC;
	tTDAL_DISP_BlenderWindow		stBlenderWindow;

    Palette.nbColors = 256;
    Palette.pColor   = &PaletteColorTab[0];


    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;


    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif

    TestManager_AssertEqual(TDAL_GFX_Init(),
							eTDAL_GFX_NO_ERROR,
							"gfx init" );

	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );

		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");

	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
    VideoEventsRegister();
#ifdef AVS_STB
    TestManager_AssertEqual(TDAL_AVS_Init(), eTDAL_AVS_NO_ERROR, "avs init");
#endif


    TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0,
														   &Width,
														   &Height),
							eTDAL_DISP_NO_ERROR,
							"DISP get blender dimensions" );

	stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = 719;
    stInputWindow.Bottom  = 575;
	stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width-1;
    stOutputWindow.Bottom = Height-1;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"Video set image IOWindows" );


    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							" disp layer video enable" );

    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_MPEG,
									(uint8_t *)gStill4,
									(uint32_t)sizeof(gStill4),
									&stPictureParams);
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );
    TestManager_AskForChecking("do you see still picture" );


    TestManager_AssertEqual(TDAL_GFX_CapabilitiesGet(&Capabilities),
							eTDAL_GFX_NO_ERROR,
							"GFX get capabilities" );


    /* Set up the palette */
    TestHelper_GFX_FillPalette(&Palette);


	stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = 719;
    stInputWindow.Bottom  = 575;
	stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = Width - 1;
    stOutputWindow.Bottom = Height - 1;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_GFX_ID_0,
														&stInputWindow,
														&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"GFX set image IOWindows" );

    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_GFX_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp GFX layer enable" );


    /* Set up default params & create a GFX region */
    Params.offset.x    = 0;
    Params.offset.y    = 0;
    Params.size.width  = 720;
    Params.size.height = 576;
    Params.refSize.width  = 720;
    Params.refSize.height = 576;
    Params.colorType   = Palette.colorType;
    Params.alpha       = 0xff;
    TestManager_AssertEqual(TDAL_GFX_RegionCreate(&GfxHandle,
												  &Params),
							eTDAL_GFX_NO_ERROR,
							"Create a GFX region" );

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_GFX_ID_0, &LayerHandle),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet" );

    TestManager_AssertEqual(TDAL_GFX_RegionDestinationSet(GfxHandle,
												  		    LayerHandle),
							eTDAL_GFX_NO_ERROR,
							"TDAL_GFX_RegionDestinationSet" );

    TestManager_AssertEqual(TDAL_GFX_RegionShow(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Show region" );

    TestManager_AssertEqual(TDAL_GFX_RegionPaletteSet(GfxHandle,
													  &Palette),
							eTDAL_GFX_NO_ERROR,
							"Set the palette" );

    /* Draw valid rectangle that fill the region */
    Params.offset.x     = 67;
    Params.offset.y    	= 40;
    Params.size.width   = 6;
    Params.size.height  = 60;
    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    Params.offset.x     = 40;
    Params.offset.y    	= 67;
    Params.size.width   = 60;
    Params.size.height  = 6;
    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    /* Draw valid rectangle that fill the region */
    Params.offset.x     = 647;
    Params.offset.y    	= 476;
    Params.size.width   = 6;
    Params.size.height  = 60;
    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    Params.offset.x     = 620;
    Params.offset.y    	= 503;
    Params.size.width   = 60;
    Params.size.height  = 6;
    PaletteColor.paletteEntry = 8;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );


    TestManager_AskForChecking("are video and OSD aligned?" );
	
    stBlenderWindow.sLeft = -120;
    stBlenderWindow.sTop = 0;
    stBlenderWindow.uiRight = 2039;
    stBlenderWindow.uiBottom = 1079;

 	TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsOutputSet(eTDAL_DISP_BLENDER_ID_0, &stBlenderWindow),
							eTDAL_DISP_NO_ERROR,
							"Resize Blender" );

	TestManager_AskForChecking("are video and OSD aligned?" );


    stBlenderWindow.sLeft = 0;
    stBlenderWindow.sTop = 0;
    stBlenderWindow.uiRight = Width - 1;
    stBlenderWindow.uiBottom = Height - 1;

 	TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsOutputSet(eTDAL_DISP_BLENDER_ID_0, &stBlenderWindow),
							eTDAL_DISP_NO_ERROR,
							"Resize Blender" );

	stInputWindow.Left    = 0;
    stInputWindow.Top     = 0;
    stInputWindow.Right   = 719;
    stInputWindow.Bottom  = 575;
	stOutputWindow.Left   = 0;
    stOutputWindow.Top    = 0;
    stOutputWindow.Right  = 959;
    stOutputWindow.Bottom = 539;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														&stInputWindow,
														&stOutputWindow),
														eTDAL_DISP_NO_ERROR,
														"Video set image IOWindows" );

	TestManager_AskForChecking("Is video quarter screen ?" );

    /* Draw valid rectangle that fill the region */
    Params.offset.x     = 33;
    Params.offset.y    	= 20;
    Params.size.width   = 4;
    Params.size.height  = 30;
    PaletteColor.paletteEntry = 4;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    Params.offset.x     = 20;
    Params.offset.y    	= 33;
    Params.size.width   = 30;
    Params.size.height  = 4;
    PaletteColor.paletteEntry = 4;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    /* Draw valid rectangle that fill the region */
    Params.offset.x     = 323;
    Params.offset.y    	= 238;
    Params.size.width   = 4;
    Params.size.height  = 30;
    PaletteColor.paletteEntry = 4;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );

    Params.offset.x     = 310;
    Params.offset.y    	= 250;
    Params.size.width   = 30;
    Params.size.height  = 4;
    PaletteColor.paletteEntry = 4;
    TestManager_AssertEqual(TDAL_GFX_RectangleDraw(GfxHandle,
												   Params.offset,
												   Params.size,
												   Params.colorType,
												   PaletteColor),
							eTDAL_GFX_NO_ERROR,
							"Draw a rectangle" );


    TestManager_AskForChecking("are video and NEW OSD cross aligned?" );

    TestManager_AssertEqual(TDAL_GFX_RegionHide(GfxHandle),
							eTDAL_GFX_NO_ERROR,
							"Hide region" );


    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp video layer disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_GFX_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp GFX layer disable" );


    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							" output audio disable" );


    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av stop video");
    VideoEventsUnregister();
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate");

    /* terminate tdal modules */

    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate" );
#endif
#ifdef PRODUCT_USE_CA
    TestManager_AssertEqual(TDAL_DESC_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"desc terminate");
#endif
    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");
}
#endif /* #ifndef CIPLUSVALIDATOR */

/*! \fn void AVCallback(const void *EventData)
 *  \brief This function tests some events on callback
 */
void AVCallback(const void *EventData)
{
    gCodePass++;
    /* event data is void*, how can i test received event ?
    TestManager_AssertEqual( gExpectedEvent, *EventData, "Event is the one expected" );*/
}

/*! \fn void SubscribeAndCheckEvents( void )
 *  \brief This function tests event subscription and event checking
 */
void SubscribeAndCheckEvents(tTDAL_AV_Event aEvent, const char* sEventName)
{
}

/*! \fn void TestCase_ExecuteVideoAspect( void )
 *  \brief This function tests changes of aspect ratio.
 */
void TestCase_ExecuteVideoEvent(void)
{
}
#ifdef TDAL_HDMI_USE
void HDMICallback
(
	tTDAL_HDMI_SourceId Id,
	tTDAL_HDMI_Event    Event
)
{
	tTDAL_OUTPUT_HDMIParams     stHdmiParams;
	tTDAL_HDMI_InfoFrameAVI     stHdmiAVI;

	stHdmiParams.AudioMode     = eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED;
	stHdmiParams.VideoFormat   = eTDAL_OUTPUT_VIDEO_HDMI_RGB888;
	stHdmiParams.VideoStandard = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;

	stHdmiAVI.uiactiveAspectRatio 		= 0x8; /*1000 -> same as picture*/
	stHdmiAVI.epictureScaling 			= eTDAL_HDMI_PICTURE_NON_UNIFORM_SCALING;
	stHdmiAVI.uiPixelRepetition			= 0;
	stHdmiAVI.uiVideoFormatIDCode			= 0;
	stHdmiAVI.uiLineNEndofTopLower        = 0;
	stHdmiAVI.uiLineNEndofTopUpper		= 0;
	stHdmiAVI.uiLineNStartofBotLower		= 0;
	stHdmiAVI.uiLineNStartofBotUpper		= 0;
	stHdmiAVI.uiPixelNEndofLeftLower		= 0;
	stHdmiAVI.uiPixelNEndofLeftUpper		= 0;
	stHdmiAVI.uiPixelNStartofRightLower	= 0;
	stHdmiAVI.uiPixelNStartofRightUpper	= 0;

	if (Event == eTDAL_HDMI_EVENT_RECEIVER_CONNECTED)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_HDMIParamsSet(eTDAL_OUTPUT_HDMI_ID_0,
														  &stHdmiParams),
								eTDAL_OUTPUT_NO_ERROR,
								"cbk:output hdmi params");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
								eTDAL_HDMI_NO_ERROR,
								"cbk:output hdmi enable");
		TestManager_AssertEqual(TDAL_HDMI_InfoFrameAVISet(Id, &stHdmiAVI),
								eTDAL_HDMI_NO_ERROR,
								"cbk:hdmi AVI info frame");

	}
	if (Event == eTDAL_HDMI_EVENT_NO_RECEIVER)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_HDMI_ID_0),
								eTDAL_HDMI_NO_ERROR,
								"cbk:output hdmi disable");
	}
}
#endif
LOCAL void MediaVideoCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    switch(event)
    {
        case eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED:
        case eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED:
            break;

        default:
            mTBOX_PRINT((kTBOX_LF, "VideoCallback: unknown event (%d)\n", event));
            break;
    }
    return;
}

LOCAL void VideoNeedDisplayCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    tTDAL_DISP_Error tdispErr;

    /*      Enable Video Layer      */
    tdispErr = TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp layer video enable");
}

LOCAL void VideoUserDataCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void VideoAspectRatioMpegCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void VideoResolutionCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void ScrambledStatusCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
}

LOCAL void MediaAudioCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    switch(event)
    {
        case eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED:
        case eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED:
            break;

        default:
            mTBOX_PRINT((kTBOX_LF, "AudioCallback: unknown event (%d)\n", event));
            break;
    }
    return;
}

LOCAL void VideoEventsRegister(void)
{
    tTDAL_AV_Error tavErr;

    /* register the video callback needed for the picture start */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED, MediaVideoCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video picture played");

    /* register the video callback needed for the picture stop */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED, MediaVideoCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video picture stopped");

    /* register the video callback needed for the display */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY, VideoNeedDisplayCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video need display");

    /* register the video user data */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_USER_DATA, VideoUserDataCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video user data");

    /* register the video aspect ratio change */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE, VideoAspectRatioMpegCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video ar change");

    /* register the video resolution change */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE, VideoResolutionCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video ar change");

    /* register the video user data */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE, ScrambledStatusCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video scramble change");
}

LOCAL void AudioEventsRegister(void)
{
    tTDAL_AV_Error tavErr;

    /* register the audio callback needed for the sample start */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED, MediaAudioCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register audio sample played");

                /* register the audio callback needed for the sample stop */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED, MediaAudioCallback);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register audio sample stopped");
}

LOCAL void VideoEventsUnregister(void)
{
    tTDAL_AV_Error tavErr;

    /* unregister the video callback needed for the picture start */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video picture played");

    /* unregister the video callback needed for the picture stop */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_PICTURE_STOPPED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video picture stopped");

    /* unregister the video callback needed for the display */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video need display");

    /* unregister the video user data */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_USER_DATA);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video user data");

    /* unregister the video aspect ratio change */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_ASPECT_RATIO_CHANGE);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video ar change");

    /* unregister the video resolution change */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_RESOLUTION_CHANGE);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video ar change");

    /* unregister the video user data */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_TRP_SCRAMBLED_CHANGE);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video scramble change");
}

LOCAL void AudioEventsUnregister(void)
{
    tTDAL_AV_Error tavErr;

    /* unregister the audio callback needed for the sample start */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister audio sample played");

    /* unregister the audio callback needed for the sample stop */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_AUDIO_1, eTDAL_AV_EVENT_AUDIO_SAMPLE_STOPPED);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister audio sample stopped");
}

void TestCase_ExecuteTestZappingAsComediaSW()
{
    tTDAL_AV_Error tavErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
#ifdef PRODUCT_USE_CA
    tTDAL_DESC_Error tdescErr;
#endif
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;

    /* ************************************************** */
    /*                Initialization of all needed TDAL modules               */
    /* ************************************************** */

    TestHelper_Initialize_TDAL_AV();


    /* ************************************************** */
    /*                      Configuration of  TDAL modules                      */
    /* ************************************************** */

    /*      TDAL AVS      */
    TestHelper_Configure_TDAL_AVS();

    /*      TDAL TSROUTE      */
    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));

	TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_DIGITAL);

    /*      Audio volume to max      */
	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
															  &stAudioCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");
	stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax;
	stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax;
	stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax;
	stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax;
	stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax;
	stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax;
	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																	  &stAudioAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");

    /*      Tuning on First Channel      */
    gChannel = 0;
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);

    /*      TDAL DMX      */
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);


    /* ************************************************** */
    /*                                  Video Starting                                   */
    /* ************************************************** */

    /*      Start Video Decoder      */
    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
									  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
						eTDAL_AV_NO_ERROR,
						"TDAL AV input stream set\n");


    tavErr = TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1, StreamType);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av start video");

    /*      Enable Video Output      */
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video enable" );

    VideoEventsRegister();


    /* ************************************************** */
    /*                                  Audio Starting                                   */
    /* ************************************************** */

    /*      Start Audio Decoder      */
    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
									  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
						eTDAL_AV_NO_ERROR,
						"TDAL AV input stream set\n");

    tavErr = TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1, StreamType);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av start audio");

    /*      Enable Audio Output      */
    toutErr = TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output audio enable");

    AudioEventsRegister();


    /* ************************************************** */
    /*                                   First Channel                                    */
    /* ************************************************** */

    TestManager_AskForChecking("Do you see video playing?");

    TestManager_AskForChecking("Do you hear sound?");

    /*      Disable Video Layer      */
    tdispErr = TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0);
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp layer video disable");

    /*      Stop Video Decoder      */
    tavErr = TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av stop video");

    /*      Disable Audio Output      */
    toutErr = TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output audio disable");

    /*      Stop Audio Decoder      */
    tavErr = TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av stop audio");

    /* Disable all the channel (Video, Audio, PCR) */
    TestHelper_ControlChannel_TDAL_DMX(FALSE);

    gChannel = 1;
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);

    /* Enable all the channel (Video, Audio, PCR) */
    TestHelper_ControlChannel_TDAL_DMX(TRUE);
    /* ************************************************** */
    /*                                  Video Starting                                   */
    /* ************************************************** */

    /*      Start Video Decoder      */
    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
									  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
						eTDAL_AV_NO_ERROR,
						"TDAL AV input stream set\n");

    tavErr = TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1, StreamType);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av start video");

    /*      Enable Video Output      */
    /*      Enable Video Output      */
    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video enable" );


    /* ************************************************** */
    /*                                  Audio Starting                                   */
    /* ************************************************** */

    /*      Start Audio Decoder      */
    StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
									  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
						eTDAL_AV_NO_ERROR,
						"TDAL AV input stream set\n");

    tavErr = TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1, StreamType);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av start audio");

    /*      Enable Audio Output      */
    toutErr = TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output audio enable");


    /* ************************************************** */
    /*                                 Second Channel                                 */
    /* ************************************************** */

    TestManager_AskForChecking("Do you see video playing?");

    TestManager_AskForChecking("Do you hear sound?");


    /*      Disable Video Layer      */
    tdispErr = TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0);
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp layer video disable");

    /*      Stop Video Decoder      */
    tavErr = TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av stop video");

    /*      Disable Audio Output      */
    toutErr = TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output audio disable");

    /*      Stop Audio Decoder      */
    tavErr = TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av stop audio");

    /* Disable all the channel (Video, Audio, PCR) */
    TestHelper_ControlChannel_TDAL_DMX(FALSE);
    /* ************************************************** */
    /*                Termination of all needed TDAL modules               */
    /* ************************************************** */

    VideoEventsUnregister();
    AudioEventsUnregister();

    TestHelper_Terminate_TDAL_AV();
}

