/** \file avtests_u.c
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
#include "tdal_ts.h"
#include "tdal_dmx.h"
#include "tdal_dmd.h"

#include "tdal_gfx.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"
#include "tdal_disp.h"

#include "testhelper.h"
#include "TestManager.h"
#include "avtests.h"
#include "avtests_resources.h"

void AVCallbackFuc(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData);
void TestCase_ExecuteUnitTestAVInitStart(void);
void TestCase_ExecuteUnitTestVideo(void);
void TestCase_ExecuteUnitTestAudio(void);
void TestCase_ExecuteUnitTestVideoPicture(void);

static tTestNode gUnitTestAVInitStart = {
    "UnitTestAVInitStart",
    "test the av init/term and start/stop",
    0, /* not interactive */
    TestCase_ExecuteUnitTestAVInitStart,
        NULL
};

static tTestNode gUnitTestVideo = {
    "UnitTestVideo",
    "test the video play",
    0, /* not interactive */
    TestCase_ExecuteUnitTestVideo,
        NULL
};

static tTestNode gUnitTestAudio = {
    "UnitTestAudio",
    "test the audio play",
    0, /* not interactive */
    TestCase_ExecuteUnitTestAudio,
        NULL
};

static tTestNode gUnitTestVideoPicture = {
    "UnitTestVideoPicture",
    "test the video picture play",
    0, /* not interactive */
    TestCase_ExecuteUnitTestVideoPicture,
        NULL
};


/*! \var tTestNode* gTDAL_AVTestNodeList[]
 *  \brief Node for A/V tests
 *
 *  This node references available tests for
 *  TDAL_AV tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_AVUnitaryTestNodeList[] = {
    &gUnitTestAVInitStart,
    &gUnitTestVideo,
    &gUnitTestAudio,
    &gUnitTestVideoPicture,
    NULL
};

tTestNode gTDAL_AV_UnitaryTestSuite = {
    "TDAL_AV Unitary Tests",
    "test the TDAL_AV function input parameters",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_AVUnitaryTestNodeList
};

/*! \fn void TestCase_ExecuteTestAudioConfig( void )
 *  \brief Test Audio config Get and set
 */

void TestCase_ExecuteUnitTestAVInitStart(void)
{
	tTDAL_AV_Capability pstCapability;
	tTDAL_AV_StreamType StreamType;

	/* Because of indicating now the input stream, the DMX module is needed */
	TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

       TestManager_AssertEqual(TDAL_AV_Init(),
							  eTDAL_AV_NO_ERROR,
							  "av init");

       TestManager_AssertEqual(TDAL_AV_CapabilityGet(&pstCapability),
							  eTDAL_AV_NO_ERROR,
							  "av Capability get");

       TestManager_AssertEqual(TDAL_AV_EventSubscribe
	   						  (eTDAL_AV_DECODER_VIDEO_1,
	   						  eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED,
	   						  AVCallbackFuc),
							  eTDAL_AV_NO_ERROR,
							  "av register video picture played");

       TestManager_AssertEqual(TDAL_AV_EventSubscribe
	   						  (eTDAL_AV_DECODER_AUDIO_1,
	   						  eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED,
	   						  AVCallbackFuc),
							  eTDAL_AV_NO_ERROR,
							  "av register audio sample played");

	StreamType.videoType=eTDAL_AV_VIDEO_TYPE_MPEG2;

	
	TestManager_AssertEqual(TDAL_GFX_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "fgx init");	
	
	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "output init");
					  
	TestManager_AssertEqual(TDAL_HDMI_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "hdmi init");

	TestManager_AssertEqual(TDAL_DISP_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "disp init");


    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
												   TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
						eTDAL_AV_NO_ERROR,
						"av input stream set video\n");
	
       TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
							  eTDAL_AV_NO_ERROR,
							  "av start video");

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
												   TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
						eTDAL_AV_NO_ERROR,
						"av input stream set audio\n");

        StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;
       TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,StreamType),
							  eTDAL_AV_NO_ERROR,
							  "av start audio");

       TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							  eTDAL_AV_NO_ERROR,
							  "av stop video");

       TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
							  eTDAL_AV_NO_ERROR,
							  "av stop audio");

       TestManager_AssertEqual(TDAL_AV_EventUnsubscribe
	   						  (eTDAL_AV_DECODER_VIDEO_1,
	   						  eTDAL_AV_EVENT_VIDEO_PICTURE_PLAYED),
							  eTDAL_AV_NO_ERROR,
							  "av unregister video picture played");

       TestManager_AssertEqual(TDAL_AV_EventUnsubscribe
	   						  (eTDAL_AV_DECODER_AUDIO_1,
	   						  eTDAL_AV_EVENT_AUDIO_SAMPLE_PLAYED),
							  eTDAL_AV_NO_ERROR,
							  "av unregister audio sample played");

       TestManager_AssertEqual(TDAL_AV_Terminate(),
							  eTDAL_AV_NO_ERROR,
							  "av term");

	   TestManager_AssertEqual(TDAL_DISP_Term(),
							  eTDAL_DISP_NO_ERROR,
							  "disp term");

	   TestManager_AssertEqual(TDAL_HDMI_Term(),
								 eTDAL_DISP_NO_ERROR,
								 "hdmi init");	
	   
	   TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								 eTDAL_DISP_NO_ERROR,
								 "output init");
/* disable this to run all tests
	   TestManager_AssertEqual(TDAL_GFX_Terminate(),
								 eTDAL_DISP_NO_ERROR,
								 "fgx init");  
*/

	   /* Because of indicating now the input stream, the DMX module is needed */
	   TestHelper_Unconfigure_TDAL_DMX();
	   TestManager_AssertEqual(TDAL_DMX_Terminate(),
							   kTDAL_DMX_NO_ERROR,
							   "dmx terminate");

}

void TestCase_ExecuteUnitTestVideo(void)
{
	tTDAL_AV_VideoCapability pstCapability;
	tTDAL_AV_StreamType StreamType;
	
	/* Because of indicating now the input stream, the DMX module is needed */
	TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);

       TestManager_AssertEqual(TDAL_AV_Init(),
							  eTDAL_AV_NO_ERROR,
							  "av init");

       TestManager_AssertEqual(TDAL_AV_VideoCapabilityGet(eTDAL_AV_DECODER_VIDEO_1,&pstCapability),
							  eTDAL_AV_NO_ERROR,
							  "av video Capability get");

       TestManager_AssertEqual(TDAL_AV_VideoDestinationSet(eTDAL_AV_DECODER_VIDEO_1,0),
							  eTDAL_AV_NO_ERROR,
							  "av VideoDestinationSet");

	TestManager_AssertEqual(TDAL_GFX_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "fgx init");	
	
	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "output init");
					  
	TestManager_AssertEqual(TDAL_HDMI_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "hdmi init");

	TestManager_AssertEqual(TDAL_DISP_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "disp init");
	
	StreamType.videoType=eTDAL_AV_VIDEO_TYPE_MPEG2;

    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
												   TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
						eTDAL_AV_NO_ERROR,
						"av input stream set video\n");
	
       TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
							  eTDAL_AV_NO_ERROR,
							  "av start video");

       TestManager_AssertEqual(TDAL_AV_VideoFreeze(eTDAL_AV_DECODER_VIDEO_1),
							  eTDAL_AV_NO_ERROR,
							  "av video freeze");

       TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							  eTDAL_AV_NO_ERROR,
							  "av stop video");
	
       TestManager_AssertEqual(TDAL_AV_Terminate(),
							  eTDAL_AV_NO_ERROR,
							  "av term");
	   TestManager_AssertEqual(TDAL_DISP_Term(),
							  eTDAL_DISP_NO_ERROR,
							  "disp term");

	   TestManager_AssertEqual(TDAL_HDMI_Term(),
								 eTDAL_DISP_NO_ERROR,
								 "hdmi init");	
	   
	   TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								 eTDAL_DISP_NO_ERROR,
								 "output init");
/* disable this to run all tests
	   TestManager_AssertEqual(TDAL_GFX_Terminate(),
								 eTDAL_DISP_NO_ERROR,
								 "fgx init");  
*/
	   /* Because of indicating now the input stream, the DMX module is needed */
	   TestHelper_Unconfigure_TDAL_DMX();
	   TestManager_AssertEqual(TDAL_DMX_Terminate(),
							   kTDAL_DMX_NO_ERROR,
							   "dmx terminate");

}

void TestCase_ExecuteUnitTestVideoPicture(void)
{
    tTDAL_AV_Error aRC;	
    uint8_t                 *Jpeg_Buffer, *pdata;
    long int                Jpeg_BufferSize;
    uint32_t	            psize;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    tTDAL_AV_VideoPictureParams stPictureParams;

    stPictureParams.notCompressWidth            = 0;
    stPictureParams.notCompressHeight           = 0;
    stPictureParams.notCompressDataBufferPosX   = 0;
    stPictureParams.notCompressDataBufferPosY   = 0;
    stPictureParams.notCompressDataBufferWidth  = 0;
    stPictureParams.notCompressDataBufferHeight = 0;

       TestManager_AssertEqual(TDAL_AV_Init(),
							  eTDAL_AV_NO_ERROR,
							  "av init");

	TestManager_AssertEqual(TDAL_GFX_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "fgx init");	
	
	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "output init");
					  
	TestManager_AssertEqual(TDAL_HDMI_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "hdmi init");

	TestManager_AssertEqual(TDAL_DISP_Init(),
							  eTDAL_DISP_NO_ERROR,
							  "disp init");

	Jpeg_Buffer = bgradio_mpg;
	Jpeg_BufferSize = bgradio_mpg_len;


    aRC = TDAL_AV_VideoPictureStart(eTDAL_AV_DECODER_VIDEO_1,
									eTDAL_AV_PICTURE_RGB,
									(uint8_t *)Jpeg_Buffer,
									(uint32_t)Jpeg_BufferSize,
									&stPictureParams );
    TestManager_AssertEqual(aRC,
							eTDAL_AV_NO_ERROR,
							"av video picture start" );


    stInputWindow.Left   = 0;
    stInputWindow.Top    = 0;
    stInputWindow.Right  = 400;
    stInputWindow.Bottom = 534;

    stOutputWindow.Left   = 100;
    stOutputWindow.Top    = 50;
    stOutputWindow.Right  = stInputWindow.Right + stOutputWindow.Left;
    stOutputWindow.Bottom = stInputWindow.Bottom + stOutputWindow.Top;
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

    TestManager_AssertEqual(TDAL_AV_VideoPictureStop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"av video picture stop" );

       TestManager_AssertEqual(TDAL_AV_VideoPictureCaptureGet
	   						  (eTDAL_AV_DECODER_VIDEO_1,
	   						  eTDAL_AV_PICTURE_MPEG,
	   						  &pdata,&psize),
							  eTDAL_AV_NO_ERROR,
							  "av VideoPictureCaptureGet");

       TestManager_AssertEqual(TDAL_AV_VideoPictureCaptureRelease
	   						  (eTDAL_AV_DECODER_VIDEO_1,
	   						  eTDAL_AV_PICTURE_MPEG,
	   						  pdata,psize),   						
							  eTDAL_AV_NO_ERROR,
							  "av VideoPictureCaptureRelease");

	   TestManager_AssertEqual(TDAL_DISP_Term(),
							  eTDAL_DISP_NO_ERROR,
							  "disp term");

	   TestManager_AssertEqual(TDAL_HDMI_Term(),
								 eTDAL_DISP_NO_ERROR,
								 "hdmi init");	
	   
	   TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								 eTDAL_DISP_NO_ERROR,
								 "output init");
/* disable this to run all tests
	   TestManager_AssertEqual(TDAL_GFX_Terminate(),
								 eTDAL_DISP_NO_ERROR,
								 "fgx init");  
*/
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							  eTDAL_AV_NO_ERROR,
							  "av term");
	
}

void TestCase_ExecuteUnitTestAudio(void)
{
	tTDAL_AV_AudioCapability pstCapability;
    uint8_t                 *Buffer;
    long int                BufferSize;
    tTDAL_AV_SampleType     sampleType;
    
    tTDAL_DMX_ChannelId AudioChannelId;
    uint32_t AudioStreamHandle;
    tTDAL_DMX_ChannelType_t AudioChannelType = {eTDAL_DMX_AUDIO_STREAM ,
                                                eTDAL_DMX_DECODER,
                                                eTDAL_DMX_NO_MEMORY};
	

       TestManager_AssertEqual(TDAL_AV_Init(),
							  eTDAL_AV_NO_ERROR,
							  "av init");

	TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
							
							
       TestManager_AssertEqual(TDAL_AV_AudioCapabilityGet(eTDAL_AV_DECODER_AUDIO_1,&pstCapability),
							  eTDAL_AV_NO_ERROR,
							  "av audio Capability get");

    Buffer = asample_mpg;
    BufferSize = asample_mpg_len;

	 TestManager_AssertEqual(TDAL_DMX_Allocate_Channel(0,&AudioChannelType,&AudioChannelId),
	 						kTDAL_DMX_NO_ERROR,
							"dmx allocate channel");
							
 	TestManager_AssertEqual(TDAL_DMX_StreamHandleGet (AudioChannelId,&AudioStreamHandle),
 								kTDAL_DMX_NO_ERROR,
							"dmx get stream handle");
							
	TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1, AudioStreamHandle),
							kTDAL_DMX_NO_ERROR,
							"dmx set stream handle");;

    sampleType = eTDAL_AV_SAMPLE_MPEG1;
    TestManager_AssertEqual(TDAL_AV_AudioSampleStart(eTDAL_AV_DECODER_AUDIO_1,
                                                     sampleType,
                                                     Buffer,
                                                     (uint32_t)BufferSize,
                                                     1),
							eTDAL_AV_NO_ERROR,
							"av sample audio start ");

    TestManager_AssertEqual(TDAL_AV_AudioSampleStop(eTDAL_AV_DECODER_AUDIO_1),
							eTDAL_AV_NO_ERROR,
							"av sample audio stop ");

    TestManager_AssertEqual(TDAL_AV_Terminate(),
							  eTDAL_AV_NO_ERROR,
							  "av term");

    TestManager_AssertEqual(TDAL_DMX_Terminate(),
							   kTDAL_DMX_NO_ERROR,
							   "dmx terminate");
	
}

void AVCallbackFuc(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{

return;
}
