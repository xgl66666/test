/** \file output_test_sce.c
 *  \brief Scenario Tests for the output layer of the TDAL API
 *  \author WangYu
 *  \date 12/06/08
 *  \version 1.0
 */



/******************************************************
 *              Includes                              *
 ******************************************************/
#include <string.h>
#include <stdio.h>
#include "crules.h"

#include "tkel.h"
#include "tbox.h"
#include "tdal_disp.h"

#include "testhelper.h"
#include "TestManager.h"

#include "tdal_hdmi.h"

#include "tdal_disp.h"
#include "tdal_output.h"

#include "tdal_ts.h"
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_avs.h"
#include "tdal_dmd.h"
#include "avtests.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif

/*! \include testhelper.h */

#define OUTPUT_AUDIO_VOLUME_MAX		15

static int gTDAL_output_TestFeIdx=0;
static int gTDAL_output_TestDmxIdx=0;
/******************************************************
 *              Display test tree                         *
 ******************************************************/
extern void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);


void    TestSce_Output_DENC_Capability( void );
void    TestSce_Output_HD_Capability( void );
void    TestSce_Output_HDMI_Capability( void );
void    TestSce_Output_Audio_Analogic_Capability( void );
void    TestSce_Output_Audio_SPDIF_Capability( void );

/******************************************************
 *              Local variables (LOCAL)               *
 ******************************************************/
static tTDAL_OUTPUT_VideoHDDACFormat HDFormat[]={eTDAL_OUTPUT_VIDEO_HD_DAC_RGB,
											eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR};

static tTDAL_OUTPUT_VideoHDDACColorSpace HDColorSpace[]={eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601,
												eTDAL_OUTPUT_VIDEO_HD_DAC_ITU709,
												eTDAL_OUTPUT_VIDEO_HD_DAC_SMPTE240M};

static tTDAL_OUTPUT_VideoHDStandard HDStandard[]={ eTDAL_OUTPUT_VIDEO_HD_STD_640_480_P_60 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_5994,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_60,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_5994,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_60 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_483_P_5994,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_576_P_50  ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50   ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2398 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_24,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_25 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2997 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_30  ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_50  ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_5994 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_60   ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2398  ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_24    ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_25  ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2997 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_30   ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_50   ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_5994  ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_60    ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50    ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_5994 ,
										    eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_60    };


static tTDAL_OUTPUT_VideoHDMIFormat HDMIFormat[]={eTDAL_OUTPUT_VIDEO_HDMI_RGB888,
											eTDAL_OUTPUT_VIDEO_HDHMI_YCBCR422,
											eTDAL_OUTPUT_VIDEO_HDHMI_YCBCR444};



static tTestNode gTestScenario_Output_DENC_Capability = {
	"TestSce_Output_DENC_Capability",
	"Display output DENC capability",
	0,
	TestSce_Output_DENC_Capability,
	NULL
};


static tTestNode gTestScenario_Output_HD_Capability = {
	"TestSce_Output_HD_Capability",
	"Display HD_Capability",
	0,
	TestSce_Output_HD_Capability,
	NULL
};

static tTestNode gTestScenario_Output_HDMI_Capability = {
	"TestSce_Output_HDMI_Capability",
	"Layer Display Management",
	0,
	TestSce_Output_HDMI_Capability,
	NULL
};

static tTestNode gTestScenario_Output_Audio_Analogic_Capability = {
	"TestSce_Output_Audio_Analogic_Capability",
	"Display Layer_Supported_Capability",
	0,
	TestSce_Output_Audio_Analogic_Capability,
	NULL
};

static tTestNode gTestScenario_Output_Audio_SPDIF_Capability = {
	"TestSce_Output_Audio_SPDIF_Capability",
	"Display Layer_Supported_Capability",
	0,
	TestSce_Output_Audio_SPDIF_Capability,
	NULL
};


tTestNode* gOutputScenarioTestNodeList[] = {
	&gTestScenario_Output_DENC_Capability,
       &gTestScenario_Output_HD_Capability,
       &gTestScenario_Output_HDMI_Capability,
	&gTestScenario_Output_Audio_Analogic_Capability,
	&gTestScenario_Output_Audio_SPDIF_Capability,
	NULL
};

tTestNode gTDAL_OUTPUT_ScenarioTestSuite = {
	"TDAL_Output Scenario Tests",
	"test TDAL_Output scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gOutputScenarioTestNodeList
};

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


void    TestSce_Output_DENC_Capability( void )
{
	tTDAL_OUTPUT_Capability pstCapability;
       tTDAL_AV_StreamType StreamType;
	tTDAL_OUTPUT_VideoDencCapability DencCapability;
	tTDAL_OUTPUT_VideoDencBCSParams Bcs;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );

	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );
       mTBOX_PRINT(( kTBOX_LF, "Output capabilities : %d VideoDenc, %d VideoHDDAC\n%d HDMI, %d AudioAna, %d AudioSPDIF, %d RFMod\n",
	   					         pstCapability.nbVideoDenc,pstCapability.nbVideoHDDAC, pstCapability.nbHDMI,
	   					         pstCapability.nbAudioAna,pstCapability.nbAudioSPDIF,pstCapability.nbRFMod));

	if(pstCapability.nbVideoDenc!=0)
	{
		     TestManager_AssertEqual( TDAL_OUTPUT_VideoDencCapabilityGet(eTDAL_OUTPUT_VIDEO_DENC_ID_0,&DencCapability),
									   eTDAL_OUTPUT_NO_ERROR,
									   "Output videoDENC Get capabilities" );
		    mTBOX_PRINT(( kTBOX_LF, "Output videoDENC capabilities[1:YES/0:NO] : FormatSupportedMask: %d, StandardSupportedMask: %d; IsColorBarSupported: %d; IsBrigthnessSupported:%d;\n IsContrastSupported:%d; IsSaturationSupported %d;\n",
						 DencCapability.FormatSupportedMask,DencCapability.StandardSupportedMask,DencCapability.IsColorBarSupported
						,DencCapability.IsBrigthnessSupported,DencCapability.IsContrastSupported,DencCapability.IsSaturationSupported));

			/*color bar check and set*/
		    if(DencCapability.IsColorBarSupported)
		    {
			    TestManager_AssertEqual( TDAL_OUTPUT_VideoDencColorBarControl(eTDAL_OUTPUT_VIDEO_DENC_ID_0,1),
				   						 eTDAL_OUTPUT_NO_ERROR,
				   					       "Output videodenc color bar enable" );
		    }
		    else
		    {
			     mTBOX_PRINT(( kTBOX_LF, "Output videoDENC color bar not supported"));

		     }



			/*init video*/
		    TestHelper_Initialize_TDAL_DMD();
		    TestManager_AssertEqual(TDAL_DMX_Init(),
									kTDAL_DMX_NO_ERROR,
									"dmx init");
#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Init(),
									eTDAL_DESC_ERROR,
									"desc init");
#endif

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Init(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs init");
		        TestHelper_Configure_TDAL_AVS();
#endif


		    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute init");
		    TestManager_AssertEqual(TDAL_AV_Init(),
									eTDAL_AV_NO_ERROR,
									"av init");


		    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));

		    TestHelper_DMDTune(gTDAL_output_TestFeIdx);
		    TestHelper_Configure_TDAL_DMX(gTDAL_output_TestDmxIdx);

		    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;
		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start video");

		    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output video enable" );

		    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video enable" );

			/*Format check and set*/
		    if(DencCapability.FormatSupportedMask&eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencFormatSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc format set" );

    			    TestManager_AskForChecking( "Output videoDENC format set to: eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC, do you see the change?" );

		    }

		    if(DencCapability.FormatSupportedMask&eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencFormatSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc format set" );

    			    TestManager_AskForChecking( "Output videoDENC format set to: eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS, do you see the change?" );

		    }

		    if(DencCapability.FormatSupportedMask&eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencFormatSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc format set" );

    			    TestManager_AskForChecking( "Output videoDENC format set to: eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB, do you see the change?" );

		    }

		    if(DencCapability.FormatSupportedMask&eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencFormatSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc format set" );

    			    TestManager_AskForChecking( "Output videoDENC format set to: eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV, do you see the change?" );

		    }

			/*Standard check and set*/

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG, do you see the change?" );

		    }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_PAL_DK)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_DK),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_PAL_DK, do you see the change?" );

		    }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_PAL_I)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_I),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_PAL_I, do you see the change?" );

		    }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_PAL_M)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_M),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_PAL_M, do you see the change?" );

		    }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_PAL_N)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_PAL_N),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_PAL_N, do you see the change?" );

		    }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_SECAM)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_SECAM),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_SECAM, do you see the change?" );

		    }

		    if(DencCapability.StandardSupportedMask&eTDAL_OUTPUT_VIDEO_SD_STD_NTSC_M)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,eTDAL_OUTPUT_VIDEO_SD_STD_NTSC_M),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc standard set" );

    			    TestManager_AskForChecking( "Output videoDENC standard set to: eTDAL_OUTPUT_VIDEO_SD_STD_NTSC_M, do you see the change?" );

		    }

			/*BCS check and set*/

			/* Brigthness Max value */
		    Bcs.Brigthness=255;
		    Bcs.Contrast=100;
		    Bcs.Saturation=100;

		    if(DencCapability.IsBrigthnessSupported)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencBCSParamsSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,&Bcs),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc BCS set" );

    			    TestManager_AskForChecking( "Output videoDENC BCS set Brigthness to: 255, do you see the change?" );

		    }

		     else
		     {
			     mTBOX_PRINT(( kTBOX_LF, "Output videoDENC Brigthness set not supported"));

		     }

			/* Contrast Max value */
		    Bcs.Brigthness=100;
		    Bcs.Contrast=255;
		    Bcs.Saturation=100;

		    if(DencCapability.IsBrigthnessSupported)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencBCSParamsSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,&Bcs),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc BCS set" );

    			    TestManager_AskForChecking( "Output videoDENC BCS set Contrast to: 255, do you see the change?" );

		    }

		     else
		     {
			     mTBOX_PRINT(( kTBOX_LF, "Output videoDENC Contrast set not supported"));

		     }

			/* Saturation Max value */
		    Bcs.Brigthness=100;
		    Bcs.Contrast=100;
		    Bcs.Saturation=255;

		    if(DencCapability.IsBrigthnessSupported)
		    {
			    TestManager_AssertEqual(TDAL_OUTPUT_VideoDencBCSParamsSet
										(eTDAL_OUTPUT_VIDEO_DENC_ID_0,&Bcs),
										eTDAL_OUTPUT_NO_ERROR,
										"output videodenc BCS set" );

    			    TestManager_AskForChecking( "Output videoDENC BCS set Saturation to: 255, do you see the change?" );

		    }

		     else
		     {
			     mTBOX_PRINT(( kTBOX_LF, "Output videoDENC Saturation set not supported"));

		     }


		    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video disable" );

		    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output Disable" );


		    /* terminate tdal modules */
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
		    TestHelper_Terminate_TDAL_DMD();
		    TestHelper_Unconfigure_TDAL_DMX();
		    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
		                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));
		    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute terminate");

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Term(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs terminate");
#endif

#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Terminate(),
									eTDAL_DESC_ERROR,
									"desc terminate");
#endif
		    TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");

		}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "Output videoDENC not supported"));

	}

	    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								eTDAL_OUTPUT_NO_ERROR,
								"output terminate" );

	    TestManager_AssertEqual(TDAL_DISP_Term(),
								eTDAL_DISP_NO_ERROR,
								"disp terminate" );


}


void    TestSce_Output_HD_Capability( void )
{
	tTDAL_OUTPUT_Capability pstCapability;
       tTDAL_AV_StreamType StreamType;
	tTDAL_OUTPUT_VideoHDDACCapability HDCapability;
	tTDAL_OUTPUT_VideoHDDACParams HDParams;
	int i,j,k;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
#ifdef TDAL_HDMI_USE
    TestManager_AssertEqual(TDAL_HDMI_Init(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi init" );
#endif


	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );
       mTBOX_PRINT(( kTBOX_LF, "Output capabilities : %d VideoDenc, %d VideoHDDAC\n %d HDMI, %d AudioAna, %d AudioSPDIF, %d RFMod\n",
	   					         pstCapability.nbVideoDenc,pstCapability.nbVideoHDDAC, pstCapability.nbHDMI,
	   					         pstCapability.nbAudioAna,pstCapability.nbAudioSPDIF,pstCapability.nbRFMod));

	if(pstCapability.nbVideoHDDAC!=0)
	{
        TestManager_AssertEqual( TDAL_OUTPUT_VideoHDDACCapabilityGet(eTDAL_OUTPUT_VIDEO_DENC_ID_0,&HDCapability),
									   eTDAL_OUTPUT_NO_ERROR,
									   "Output videoHD Get capabilities" );

        mTBOX_PRINT(( kTBOX_LF, "Output videoHD capabilities[1:YES/0:NO] : FormatSupportedMask: %d, StandardSupportedMask: %d; ColorSpaceSupportedMask: %d\n",
						 HDCapability.FormatSupportedMask,HDCapability.StandardSupportedMask,HDCapability.ColorSpaceSupportedMask));

			/*init video*/
		    TestHelper_Initialize_TDAL_DMD();
		    TestManager_AssertEqual(TDAL_DMX_Init(),
									kTDAL_DMX_NO_ERROR,
									"dmx init");
#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Init(),
									eTDAL_DESC_ERROR,
									"desc init");
#endif

#ifdef AVS_STB
	            TestManager_AssertEqual(TDAL_AVS_Init(),
	                                    eTDAL_AVS_NO_ERROR,
	                                    "avs init");
		     TestHelper_Configure_TDAL_AVS();
#endif

		HDParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		HDParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		HDParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		HDParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&HDParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");

#ifdef TDAL_HDMI_USE
		if(pstCapability.nbHDMI > 0)
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

		    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute init");
		    TestManager_AssertEqual(TDAL_AV_Init(),
									eTDAL_AV_NO_ERROR,
									"av init");

		    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));

		    TestHelper_DMDTune(gTDAL_output_TestFeIdx);
		    TestHelper_Configure_TDAL_DMX(gTDAL_output_TestDmxIdx);

		    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

		    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start video");
		if(pstCapability.nbVideoDenc > 0)
		{
			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output denc video enable" );
		}

		    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video enable" );


			HDParams.EmbeddedSyncOn=TRUE;

			/* set all the params supported */
			/* check the format */
		    for(i=0;i<2;i++)
		    {
				if( HDCapability.FormatSupportedMask & HDFormat[i])
				{/* check the format */
					for(j=0;j<3;j++)
					{
						if( HDCapability.ColorSpaceSupportedMask & HDColorSpace[j])
						{/* check the color space */
							for(k=0;k<27;k++)
								{
									if( HDCapability.StandardSupportedMask & HDStandard[k])
									{/* check the standard */
										HDParams.ColorSpace=HDColorSpace[j];
										HDParams.Format=HDFormat[i];
										HDParams.Standard=HDStandard[k];
									       TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet
																(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,&HDParams),
																eTDAL_OUTPUT_NO_ERROR,
																"output videoHD params set" );

										 mTBOX_PRINT(( kTBOX_LF, "Output videoHD params set to: Format= 0x%x,  Color space= 0x%x, standard= 0x%x\n",
										 						  HDParams.Format,HDParams.ColorSpace,HDParams.Standard));


						    			        TestManager_AskForChecking( "Output videoHD params set , do you see the change?" );

									}

								}
						}
					}
				}
		    }



		    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video disable" );

		    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output Disable" );


		    /* terminate tdal modules */
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
		    TestHelper_Terminate_TDAL_DMD();
		    TestHelper_Unconfigure_TDAL_DMX();
		    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));
		    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute terminate");

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Term(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs terminate");
#endif

#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Terminate(),
									eTDAL_DESC_ERROR,
									"desc terminate");
#endif
		    TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");

		}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "Output videoHD not supported\n"));

	}

	    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								eTDAL_OUTPUT_NO_ERROR,
								"output terminate" );

	    TestManager_AssertEqual(TDAL_DISP_Term(),
								eTDAL_DISP_NO_ERROR,
								"disp terminate" );

}


void    TestSce_Output_HDMI_Capability( void )
{
	tTDAL_OUTPUT_Capability pstCapability;
       tTDAL_AV_StreamType StreamType;
	tTDAL_OUTPUT_HDMICapability HDMICapability;
	tTDAL_OUTPUT_HDMIParams HDMIParams;
	tTDAL_OUTPUT_VideoHDDACParams HDParams;

	int i,j;  

	    TestManager_AssertEqual(TDAL_DISP_Init(),
								eTDAL_DISP_NO_ERROR,
								"disp init" );

	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
#ifdef TDAL_HDMI_USE
	TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI init" );
#endif
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );
       mTBOX_PRINT(( kTBOX_LF, "Output capabilities : %d VideoDenc, %d VideoHDDAC\n%d HDMI, %d AudioAna, %d AudioSPDIF, %d RFMod\n",
	   					         pstCapability.nbVideoDenc,pstCapability.nbVideoHDDAC, pstCapability.nbHDMI,
	   					         pstCapability.nbAudioAna,pstCapability.nbAudioSPDIF,pstCapability.nbRFMod));

	if(pstCapability.nbHDMI!=0)
	{
			/*init video*/
		    TestHelper_Initialize_TDAL_DMD();
		    TestManager_AssertEqual(TDAL_DMX_Init(),
									kTDAL_DMX_NO_ERROR,
									"dmx init");
#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Init(),
									eTDAL_DESC_ERROR,
									"desc init");
#endif

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Init(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs init");
		        TestHelper_Configure_TDAL_AVS();
#endif


		    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute init");
		    TestManager_AssertEqual(TDAL_AV_Init(),
									eTDAL_AV_NO_ERROR,
									"av init");

	if(pstCapability.nbVideoHDDAC!=0)
	{
		HDParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		HDParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		HDParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		HDParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&HDParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}
		if(pstCapability.nbVideoDenc > 0)
		{
			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output denc video enable" );
		}

#ifdef TDAL_HDMI_USE
		if(pstCapability.nbHDMI > 0)
		{

			TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
													HDMICallback,
													FALSE),
									eTDAL_HDMI_NO_ERROR,
									"hdmi start" );

			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output HDMI video enable" );

            TKEL_DelayTask(ms2tick(3000));

            TestManager_AssertEqual( TDAL_OUTPUT_HDMICapabilityGet(eTDAL_OUTPUT_HDMI_ID_0,&HDMICapability),
                                      eTDAL_OUTPUT_NO_ERROR,
                                      "Output videoHDMI Get capabilities" );
           mTBOX_PRINT(( kTBOX_LF, "Output videoHDMI capabilities[1:YES/0:NO] : FormatSupportedMask: %d, StandardSupportedMask: %d\n",
                        HDMICapability.FormatSupportedMask,HDMICapability.StandardSupportedMask));

		}
#endif

		    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));

		    TestHelper_DMDTune(gTDAL_output_TestFeIdx);
		    TestHelper_Configure_TDAL_DMX(gTDAL_output_TestDmxIdx);

		    StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

		    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start video");

		    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video enable" );


            /* LO : modified for compil */
			HDMIParams.AudioMode=eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_AC3;

			/* set all the params supported */
			/* check the format */
		    for(i=0;i<2;i++)
		    {
				if( HDMICapability.FormatSupportedMask & HDMIFormat[i])
				{/* check the format */
					for(j=0;j<27;j++)
					{
						if( HDMICapability.StandardSupportedMask & HDStandard[j])
						{/* check the standard */

							HDMIParams.VideoFormat=HDMIFormat[i];
							HDMIParams.VideoStandard=HDStandard[j];

						       TestManager_AssertEqual(TDAL_OUTPUT_HDMIParamsSet(eTDAL_OUTPUT_HDMI_ID_0,&HDMIParams),
                                                       eTDAL_OUTPUT_NO_ERROR,
                                                       "output videoHDMI params set" );

							 mTBOX_PRINT(( kTBOX_LF, "Output videoHDMI params set to: Format= 0x%x,  standard= 0x%x\n",
							 						  HDMIParams.VideoFormat,HDMIParams.VideoStandard));


			    			        TestManager_AskForChecking( "Output videoHDMI params set , do you see the change?" );

						}
					}
				}
		    }



		    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video disable" );



		    /* terminate tdal modules */
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
		    TestHelper_Terminate_TDAL_DMD();
		    TestHelper_Unconfigure_TDAL_DMX();
		    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));
		    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute terminate");


#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Term(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs terminate");
#endif

#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Terminate(),
									eTDAL_DESC_ERROR,
									"desc terminate");
#endif
		    TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");

		}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "Output videoHDMI not supported"));

	}

	    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								eTDAL_OUTPUT_NO_ERROR,
								"output terminate" );
#ifdef TDAL_HDMI_USE	
		    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI term" );
#endif

	    TestManager_AssertEqual(TDAL_DISP_Term(),
								eTDAL_DISP_NO_ERROR,
								"disp terminate" );

}


void    TestSce_Output_Audio_Analogic_Capability( void )
{
	tTDAL_OUTPUT_Capability pstCapability;
       tTDAL_AV_StreamType StreamType;
	tTDAL_OUTPUT_AudioAnaCapability AudioAnaCapability;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation pstAttenuation;
	tTDAL_OUTPUT_VideoHDDACParams HDParams;

       int i;
	pstAttenuation.FrontCenterAttenuation=0;
	pstAttenuation.FrontRLAttenuation=0;
	pstAttenuation.LFEAttenuation=0;
	pstAttenuation.MixLevel=0;
	pstAttenuation.RearCenterAttenuation=0;
	pstAttenuation.RearRLAttenuation=0;
	pstAttenuation.SurroundRLAttenuation=0;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );


	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );


       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );
       mTBOX_PRINT(( kTBOX_LF, "Output capabilities : %d VideoDenc, %d VideoHDDAC\n %d HDMI, %d AudioAna, %d AudioSPDIF, %d RFMod\n",
	   					         pstCapability.nbVideoDenc,pstCapability.nbVideoHDDAC, pstCapability.nbHDMI,
	   					         pstCapability.nbAudioAna,pstCapability.nbAudioSPDIF,pstCapability.nbRFMod));


	if(pstCapability.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

	if(pstCapability.nbAudioAna!=0)
	{
		     TestManager_AssertEqual( TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,&AudioAnaCapability),
									   eTDAL_OUTPUT_NO_ERROR,
									   "Output AudioAna Get capabilities" );
		    mTBOX_PRINT(( kTBOX_LF, "Output audioAna capabilities Get\n"));

			/*init audio*/
		    TestHelper_Initialize_TDAL_DMD();
		    TestManager_AssertEqual(TDAL_DMX_Init(),
									kTDAL_DMX_NO_ERROR,
									"dmx init");
#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Init(),
									eTDAL_DESC_ERROR,
									"desc init");
#endif

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Init(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs init");
		        TestHelper_Configure_TDAL_AVS();
#endif

	if(pstCapability.nbVideoHDDAC!=0)
	{
		HDParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		HDParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		HDParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		HDParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&HDParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

		    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute init");
		    TestManager_AssertEqual(TDAL_AV_Init(),
									eTDAL_AV_NO_ERROR,
									"av init");

		    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));
            VideoEventsRegister();

		    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
		    TestHelper_Configure_TDAL_DMX(gTDAL_output_TestDmxIdx);

		    StreamType.audioType= eTDAL_AV_AUDIO_TYPE_MPEG;

		    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start audio");

		    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output audio enable" );

            AudioEventsRegister();

		    StreamType.videoType= eTDAL_AV_VIDEO_TYPE_MPEG2;

		    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start video");

		    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video enable" );


			/* set all the params supported */
		    for(i=0;i<=5;i++)
		    {
				if(AudioAnaCapability.IsFrontCenterPresenceSupported&&AudioAnaCapability.IsFrontCenterAttenuationSupported)
				{
					pstAttenuation.FrontCenterAttenuation=OUTPUT_AUDIO_VOLUME_MAX-i*3;
				}

				if(AudioAnaCapability.IsFrontRLPresenceSupported&&AudioAnaCapability.IsFrontRLAttenuationSupported)
				{
					pstAttenuation.FrontRLAttenuation=OUTPUT_AUDIO_VOLUME_MAX-i*3;
				}

				if(AudioAnaCapability.IsSurroundRLPresenceSupported&&AudioAnaCapability.IsSourroundRLAttenuationSupported)
				{
					pstAttenuation.SurroundRLAttenuation=OUTPUT_AUDIO_VOLUME_MAX-i*3;
				}

				if(AudioAnaCapability.IsRearCenterPresenceSupported&&AudioAnaCapability.IsRearCenterAttenuationSupported)
				{
					pstAttenuation.RearCenterAttenuation=OUTPUT_AUDIO_VOLUME_MAX-i*3;
				}

				if(AudioAnaCapability.IsRearRLPresenceSupported&&AudioAnaCapability.IsRearRLAttenuationSupported)
				{
					pstAttenuation.RearRLAttenuation=OUTPUT_AUDIO_VOLUME_MAX-i*3;
				}

				if(AudioAnaCapability.IsMixingSupported)
				{
					pstAttenuation.MixLevel=100-i*20;/* 100: max mixlevel */
				}

			       TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet
										(eTDAL_OUTPUT_AUDIO_ANA_ID_0,&pstAttenuation),
										eTDAL_OUTPUT_NO_ERROR,
										"output Audioana attenuation set" );

				 mTBOX_PRINT(( kTBOX_LF, "Output audioAna attenuation set to(max = 15): %d\n",
				 						  OUTPUT_AUDIO_VOLUME_MAX-i*3));


    			        TestManager_AskForChecking( "Output audioAna attenuation set , do you hear the change?" );

		    }




		    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output Disable" );


		    /* terminate tdal modules */
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
		    TestHelper_Terminate_TDAL_DMD();
		    TestHelper_Unconfigure_TDAL_DMX();
		    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));
		    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute terminate");

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Term(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs terminate");
#endif

#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Terminate(),
									eTDAL_DESC_ERROR,
									"desc terminate");
#endif
		    TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");

		}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "Output videoHDMI not supported"));

	}

	    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								eTDAL_OUTPUT_NO_ERROR,
								"output terminate" );

	    TestManager_AssertEqual(TDAL_DISP_Term(),
								eTDAL_DISP_NO_ERROR,
								"disp terminate" );

    VideoEventsUnregister();
    AudioEventsUnregister();

}


void    TestSce_Output_Audio_SPDIF_Capability( void )
{
	tTDAL_OUTPUT_Capability pstCapability;
       tTDAL_AV_StreamType StreamType;
	 tTDAL_OUTPUT_AudioDigitalMode Audiomode;
	tTDAL_OUTPUT_VideoHDDACParams HDParams;

    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );

	TestManager_AssertEqual( TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "Output init" );
       TestManager_AssertEqual( TDAL_OUTPUT_CapabilityGet(&pstCapability),
	   						   eTDAL_OUTPUT_NO_ERROR,
	   						   "Output Get capabilities" );
       mTBOX_PRINT(( kTBOX_LF, "Output capabilities : %d VideoDenc, %d VideoHDDAC\n%d HDMI, %d AudioAna, %d AudioSPDIF, %d RFMod\n",
	   					         pstCapability.nbVideoDenc,pstCapability.nbVideoHDDAC, pstCapability.nbHDMI,
	   					         pstCapability.nbAudioAna,pstCapability.nbAudioSPDIF,pstCapability.nbRFMod));

	if(pstCapability.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

	if(pstCapability.nbAudioSPDIF!=0)
	{
			/*init audio*/
		    TestHelper_Initialize_TDAL_DMD();
		    TestManager_AssertEqual(TDAL_DMX_Init(),
									kTDAL_DMX_NO_ERROR,
									"dmx init");
#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Init(),
									eTDAL_DESC_ERROR,
									"desc init");
#endif

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Init(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs init");
		        TestHelper_Configure_TDAL_AVS();
#endif

	if(pstCapability.nbVideoHDDAC!=0)
	{
		HDParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		HDParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		HDParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		HDParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&HDParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}


		    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute init");
		    TestManager_AssertEqual(TDAL_AV_Init(),
									eTDAL_AV_NO_ERROR,
									"av init");

		    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                      TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));

		    TestHelper_DMDTune(gTDAL_output_TestFeIdx);
		    TestHelper_Configure_TDAL_DMX(gTDAL_output_TestDmxIdx);

		    StreamType.audioType= eTDAL_AV_AUDIO_TYPE_MPEG;

		    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start video");

		    TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output video enable" );

		    StreamType.videoType= eTDAL_AV_VIDEO_TYPE_MPEG2;

		    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										  			TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

		    TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,StreamType),
												  eTDAL_AV_NO_ERROR,
											  	  "av start video");

		    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									" disp layer video enable" );

		    Audiomode=eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED_AC3;
   	           TestManager_AssertEqual(TDAL_OUTPUT_AudioSPDIFModeSet
								(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0,Audiomode),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio SPDIF audio mode set to OUTPUT_AUDIO_DIGITAL_MODE_COMPRESSED" );


		    Audiomode=eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED;
   	           TestManager_AssertEqual(TDAL_OUTPUT_AudioSPDIFModeSet
								(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0,Audiomode),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio SPDIF audio mode set to OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED" );


		    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output Disable" );


		    /* terminate tdal modules */
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1), eTDAL_AV_NO_ERROR,"av video audio");
            TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av terminate");
		    TestHelper_Terminate_TDAL_DMD();
		    TestHelper_Unconfigure_TDAL_DMX();
		    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_output_TestFeIdx),
		                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_output_TestDmxIdx));
		    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
									eTDAL_TSROUTE_NO_ERROR,
									"tsroute terminate");

#ifdef AVS_STB
            TestManager_AssertEqual(TDAL_AVS_Term(),
                                    eTDAL_AVS_NO_ERROR,
                                    "avs terminate");
#endif

#ifdef PRODUCT_USE_CA
		    TestManager_AssertEqual(TDAL_DESC_Terminate(),
									eTDAL_DESC_ERROR,
									"desc terminate");
#endif
		    TestManager_AssertEqual(TDAL_DMX_Terminate(),
									kTDAL_DMX_NO_ERROR,
									"dmx terminate");

		}

	else
	{
	     mTBOX_PRINT(( kTBOX_LF, "Output audio SPDIF not supported"));

	}

	    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								eTDAL_OUTPUT_NO_ERROR,
								"output terminate" );

	    TestManager_AssertEqual(TDAL_DISP_Term(),
								eTDAL_DISP_NO_ERROR,
								"disp terminate" );

}
