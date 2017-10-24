/** \file analog_test_sce.c
 *  \brief Scenario Tests for the ANALOG layer of the TDAL API
 *  \author
 *  \date
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/
#include <string.h>
#include <stdio.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_dmd.h"
#include "tdal_panel.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_ana_aud.h"
#include "tdal_ana_vid.h"
#include "tdal_scart_input.h"
#include "tdal_ana_input.h"
#include "tdal_hdmi_in.h"
#include "tdal_vbi_slicer.h"
#include "tdal_speakers.h"
#include "tdal_connection_manager.h"

#include "testhelper.h"

#include "dmdtests.h"
#include "analogtests.h"

mTBOX_SET_MODULE(eTDAL_TEST_ANA);

/******************************************************
 *              ATV/DTV test tree                     *
 ******************************************************/

static tTestNode gTestAtvDtvTuneVideoAudio= {
	"TestAtvDtvTuneVideoAudio",
	"Play Video and Audio from an Analog Frequency then from a digital frequency",
	0,
	TestSce_AtvDtvTuneVideoAudio,
	NULL
};

static tTestNode gTestAtvDtvTuneSection= {
	"TestAtvDtvTuneSection",
	"Play Video and Audio from an Analog Frequency then filter section from a digital frequency",
	0,
	TestSce_AtvDtvTuneSection,
	NULL
};

tTestNode* gTDAL_ANALOG_AtvDtvTestNodeList[] = {
    &gTestAtvDtvTuneVideoAudio,
    &gTestAtvDtvTuneSection,
	NULL
};

tTestNode gTDAL_ANALOG_AtvDtvTestSuite = {
	"ATV/DTV Scenario Tests",
	"test ATV/DTV transitions",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTDAL_ANALOG_AtvDtvTestNodeList
};

extern int gChannel;
extern int gChannelAnalog;
extern bool bUseAnalogChannel;
extern struct TestManager_Channel_Analog gAnalogChannelList[];

static void TestSce_AtvDtvTuneVideoAudioSection(bool bDTV_AV, bool bDTV_Section);

static tTDAL_DMX_ChannelId TestSce_AtvDtvDMXSectionChId;
static tTDAL_DMX_FilterId  TestSce_AtvDtvDMXSectionFiltId;

#define SUB_PES

#if defined(SUB_PES)
static tTDAL_DMX_ChannelId TestSce_AtvDtvDMXSubPesChId;
static tTDAL_DMX_FilterId  TestSce_AtvDtvDMXSubPesFiltId;
#endif

static TKEL_sema_id        TestSce_AtvDtvDMXSectionSem = (TKEL_sema_id)NULL;

static void TestSce_AtvDtvDMXSectionAllocate( void );
static void TestSce_AtvDtvDMXSectionFree( void );
static void TestSce_AtvDtvDMXSectionEnableWait( void );
static void TestSce_AtvDtvDMXSectionDisable( void );
static void TestSce_AtvDtvDMXSectionNotify(uint8_t* buffer,
										   uint32_t length);
static void TestSce_AtvDtvDMXSectionNotifyCbk(tTDAL_DMX_ChannelId ChannelId,
											  tTDAL_DMX_FilterId FilterId,
											  uint32_t Filter_track,
											  uint32_t Length,
											  uint8_t *Buffer);

/******************************************************
 *              Scenario tests                        *
*******************************************************/
void TestSce_AtvDtvTuneVideoAudio(void )
{
	TestSce_AtvDtvTuneVideoAudioSection(TRUE, FALSE);
}

void TestSce_AtvDtvTuneSection(void )
{
	TestSce_AtvDtvTuneVideoAudioSection(TRUE, TRUE);
}

static void TestSce_AtvDtvTuneVideoAudioSection(bool bDTV_AV, bool bDTV_Section)
{
    unsigned char                           ucFeID;
	tTDAL_DMD_NotifyStatus                  DMD_NotifyCond;
    tTDAL_DMD_FECapability                  stFECapability;
    uint32_t                                DemodStreamHandle, LayerStreamHandle;
    tTDAL_ANA_VID_Decoder                   IndexVideoDecoder;
    tTDAL_ANA_AUD_Decoder                   IndexAudioDecoder;
    tTDAL_ANA_AUD_DecoderCapability         stAudioDecoderCapability;
    tTDAL_ANA_VID_DecoderCapability         stVideoDecoderCapability;
    tTDAL_ANA_AUD_StartParams               stAudioStartParams;
    tTDAL_ANA_VID_StartParams               stVideoStartParams;
    tTDAL_OUTPUT_VideoDigitalParams         stVideoDigitalParams;
	tTDAL_OUTPUT_VideoHDDACParams           stHDDACParams;
	tTDAL_OUTPUT_Capability                 stOutputCap;
    tTDAL_OUTPUT_AudioAnaCapability         stAudioCapability;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;
	struct TestManager_Channel             *pChannel;
	tTDAL_DMD_ConfigData                    DMD_Config;
    tTDAL_AV_StreamType                     StreamType;
    uint32_t                                uiWidth;
    uint32_t                                uiHeight;
	uint32_t                			    uiNbChange = 0;

#if 0
    tTDAL_DMX_ChannelType_t      type;
	tTDAL_DMX_ChannelId TestSce_AtvDtvDMXSectionTdtChId;
#endif

    TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
	TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
	TestManager_AssertEqual(TDAL_PANEL_Init(),
							eTDAL_PANEL_NO_ERROR,
							"panel init" );
	TestManager_AssertEqual(TDAL_SPEAKERS_Init(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speaker init" );
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
	/* Initialisation for Analog Video*/
	TestHelper_Initialize_ANA_VID( &IndexVideoDecoder,
								   &stVideoDecoderCapability,
								   eTDAL_ANA_VID_CODING_CVBS  );
	/* Initialisation for Analog Audio*/
	TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );
    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(0),
                                      TEST_HELPER_TSROUTE_DEMUX(0));

	if(bDTV_AV)
	{
		gChannel = 7;
		pChannel = TestHelper_GetChannel();
		/*      TDAL DMX      */
		TestHelper_Configure_TDAL_DMX(0);
		/* Disable all the channel (Video, Audio, PCR) */
		TestHelper_ControlChannel_TDAL_DMX(FALSE);
	}

	if(bDTV_Section)
	{
		TestSce_AtvDtvDMXSectionAllocate();
	}
	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_ATV);

    /*Get tuner capability*/
    TestManager_AssertEqual(TDAL_DMD_GetFECapability( ucFeID, &stFECapability),
							eTDAL_DMD_NO_ERROR,
							"TDAL_DMD_GetFECapability succeed");

    if(stFECapability.eSourceSupport == eTDAL_DMD_DIGITAL)
    {
        TestManager_LogMessage("Tuner doesn't support the anolog\n");
        TestHelper_DMD_Term(&ucFeID);
        return;
    }
    else
    {
		TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);

do
{
		/* swicth to DTV mode */
		TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_DIGITAL);

		bUseAnalogChannel = FALSE;

		/*TKEL_DelayTask(ms2tick(20));*/

		DMD_Config.Ter.ConfigType            = eTDAL_DMD_CFG_TER_SOURCE_TRANSITION;
		DMD_Config.Ter.uCfg.SourceTransition = eTDAL_DMD_CFG_SOURCE_IS_DTV;
		TestManager_AssertEqual( TDAL_DMD_Config(ucFeID, &DMD_Config),
								 eTDAL_DMD_NO_ERROR,
								 "dmd config DTV SOURCE_TRANSITION" );



		TestHelper_DMD_Tune(&ucFeID);

		if(bDTV_AV)
		{
			/* Enable all the channel (Video, Audio, PCR) */
			TestHelper_ControlChannel_TDAL_DMX(TRUE);

#if defined(SUB_PES)

			TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( TestSce_AtvDtvDMXSubPesFiltId,
															   0x2e4 ), /* ARTE HD subt pes pid */
									kTDAL_DMX_NO_ERROR, "DMX Set PES Channel PID" );
#endif


			TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
															TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
									eTDAL_AV_NO_ERROR,
									"TDAL AV input stream set\n");

			StreamType.videoType = pChannel->videoType; /*eTDAL_AV_VIDEO_TYPE_MPEG2*/;
			TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
												  StreamType),
									eTDAL_AV_NO_ERROR,
									"1st av start video");

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

#if 0
			/* check allocate TDT */

		    type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
		    type.eChannelOutput     = eTDAL_DMX_MEMORY;
		    type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;

printf("====================================\n");
printf("ALLOCATE PAT    \n");
printf("====================================\n");

		    TestManager_AssertEqual(TDAL_DMX_Allocate_Channel( eTDAL_DMX_DEMUX0,
												   &type,
												   &TestSce_AtvDtvDMXSectionTdtChId ),
						kTDAL_DMX_NO_ERROR,
						"DMX allocate channel TDT" );


            TestManager_AssertEqual(TDAL_DMX_Set_Channel_PID( TestSce_AtvDtvDMXSectionTdtChId,
													  0x0000 ),
							kTDAL_DMX_NO_ERROR,
							"DMX Set Channel PID" );
#endif
			


			/* audio */
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
									"2nd output audio enable" );

			if(stOutputCap.nbAudioAna > 1)
			{
				TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
										eTDAL_OUTPUT_NO_ERROR,
										"2nd output audio headphone enable" );
			}
		
			if(stOutputCap.nbAudioSPDIF > 0)
			{
			TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
									   eTDAL_OUTPUT_NO_ERROR,
									   "2nd output audio SPDIF enable" );
			}
		}

		if(bDTV_Section)
		{
			 TestSce_AtvDtvDMXSectionEnableWait();
		}

		if(bDTV_AV)
		{
			TKEL_DelayTask(ms2tick(5000));
			printf("!!!!!!!!!!!!!!!!!!!!!!! DIGITAL uiNbChange %d \n",uiNbChange);
			/* TestManager_AskForChecking( "do you see DIGITAL video and audio playing ?" ); */

#if 0
			TDAL_DMX_Free_Channel(TestSce_AtvDtvDMXSectionTdtChId);
#endif
			TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
									eTDAL_DISP_NO_ERROR,
									"disp layer video disable" );
			TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
									eTDAL_AV_NO_ERROR,
									"av stop video");
			TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output audio disable" );
			if(stOutputCap.nbAudioSPDIF > 0)
			{
			   TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
									   eTDAL_OUTPUT_NO_ERROR,
									   "output audio SPDIF disable" );
			}
			if(stOutputCap.nbAudioAna > 1)
			{
				/* we suppose that when more than one audio ana, the second is a headphone */
				TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
										eTDAL_OUTPUT_NO_ERROR,
										"output audio headphone disable" );
			}
			TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1),
									eTDAL_AV_NO_ERROR,
									"av stop audio");
		}

		if(bDTV_Section)
		{
			TestSce_AtvDtvDMXSectionDisable();
		}

		DMD_Config.Ter.ConfigType            = eTDAL_DMD_CFG_TER_SOURCE_TRANSITION;
		DMD_Config.Ter.uCfg.SourceTransition = eTDAL_DMD_CFG_SOURCE_IS_ATV;
		TestManager_AssertEqual( TDAL_DMD_Config(ucFeID, &DMD_Config),
								 eTDAL_DMD_NO_ERROR,
								 "dmd config ATV SOURCE_TRANSITION" );
		TestManager_AssertEqual(TDAL_DMD_HandleGet(ucFeID, &DemodStreamHandle),
								eTDAL_DMD_NO_ERROR,
								"TDAL_DMD_HandleGet succeed");
		
		/* Set the way for the video */
        TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder, DemodStreamHandle ),
								eTDAL_ANA_VID_NO_ERROR,
								"TDAL_ANA_VID_InputStreamSet succeed");

        TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
														 (void*)&(LayerStreamHandle)),
								eTDAL_DISP_NO_ERROR,
								"TDAL_DISP_LayerHandleGet succeed");

		TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
								eTDAL_DISP_NO_ERROR,
								"TDAL_DISP_LayerIOWindowsGet succeed");

		stOutputWindow.Top = 0;
		stOutputWindow.Bottom = 1080;
		stOutputWindow.Left= 0;
		stOutputWindow.Right = 1920;

		TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
								eTDAL_DISP_NO_ERROR,
								"TDAL_DISP_LayerIOWindowsSet succeed");

        TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet( IndexVideoDecoder,
															 LayerStreamHandle),
								eTDAL_ANA_VID_NO_ERROR,
								"TDAL_ANA_VID_DestinationSet succeed");

        /* Set the way for the audio */
        TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder,
															DemodStreamHandle ),
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_InputStreamSet succeed");

		memset(&stSpeakersCap, 0, sizeof(tTDAL_SPEAKERS_Capability));
		TestManager_AssertEqual(TDAL_SPEAKERS_CapabilityGet(&stSpeakersCap),
								eTDAL_SPEAKERS_NO_ERROR,
								"speakers caps");
		if(stSpeakersCap.nbSpeakers > 0)
		{
			/* 1st speaker Id is FPD */
			TestManager_AssertEqual(TDAL_SPEAKERS_HandleGet(eTDAL_SPEAKERS_FPD_ID_0,
															&SpeakerHandle),
									eTDAL_SPEAKERS_NO_ERROR,
									"FPD speaker handle get");
			TestManager_AssertEqual(TDAL_ANA_AUD_DestinationSet(IndexAudioDecoder,
																SpeakerHandle),
									eTDAL_ANA_AUD_NO_ERROR,"TDAL_ANA_AUD_DestinationSet FPD succeed");	
		
		}
		
		if(stSpeakersCap.nbSpeakers > 1)
		{
			/* 2nd speaker Id is HP */
			TestManager_AssertEqual(TDAL_SPEAKERS_HandleGet(eTDAL_SPEAKERS_HP_ID_0,
															&SpeakerHandle),
									eTDAL_SPEAKERS_NO_ERROR,
									"HP speaker handle get");
			TestManager_AssertEqual(TDAL_ANA_AUD_DestinationSet(IndexAudioDecoder,
																SpeakerHandle),
									eTDAL_ANA_AUD_NO_ERROR,
									"TDAL_ANA_AUD_DestinationSet HP succeed");	
		}

		TestManager_AssertEqual(TDAL_ANA_AUD_TrackSet(IndexAudioDecoder,
													  eTDAL_ANA_AUD_TRACK_STEREO),
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_TrackSet succeed");

        memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
        TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
								eTDAL_OUTPUT_NO_ERROR,
								"output caps");
    	if(stOutputCap.nbVideoDigital > 0)
    	{
    		stVideoDigitalParams.VideoFormat     = eTDAL_OUTPUT_VIDEO_DIGITAL_RGB888;
    		stVideoDigitalParams.VideoStandard   = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
    		TestManager_AssertEqual(TDAL_OUTPUT_VideoDigitalParamsSet(eTDAL_OUTPUT_VIDEO_DIGITAL_ID_0,
																	  &stVideoDigitalParams),
									eTDAL_OUTPUT_NO_ERROR,
									"output video digital params set");
    		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DIGITAL_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output video digital enable");
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
		/* swicth to ATV mode */
		TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_ANALOG_TUNER);


        /*      Tuning on First Channel Analog     */
        gChannelAnalog = 0;
		bUseAnalogChannel = TRUE;
        TestHelper_DMD_Tune(&ucFeID);
        TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
        TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");

        /* Now Tuner "locked", then wait for signal presence !! */

        /*start audio and video Analog */
        stAudioStartParams.uiStdMask = stAudioDecoderCapability.uiSupportedStdMask;
        TestManager_AssertEqual(TDAL_ANA_AUD_Start(IndexAudioDecoder,
												   &stAudioStartParams ),
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_Start succeed");

        stVideoStartParams.uiCodingMask  = eTDAL_ANA_VID_CODING_CVBS;/*stVideoDecoderCapability.uiSupportedCodingMask;*/
        stVideoStartParams.uiStdMask     = eTDAL_ANA_VID_STD_SECAM;/*stVideoDecoderCapability.uiSupportedStdMask;*/
        TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
												   &stVideoStartParams ),
								eTDAL_ANA_VID_NO_ERROR,
								"IndexVideoDecoder succeed");

        TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"1st disp layer video enable" );

		TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																  &stAudioCapability),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio capability get");
		stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax/4;
		stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax/4;
		stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax/4;
		stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax/4;
		stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax/4;
		stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax/4;
		TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																		  &stAudioAttenuation),
								eTDAL_OUTPUT_NO_ERROR,
								"output audio volume set");
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
									"1st output audio headphone enable" );
		}
		
		if(stOutputCap.nbAudioSPDIF > 0)
		{
		   TestManager_AssertEqual(TDAL_OUTPUT_AudioSPDIFModeSet(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0,
																 eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED),
								   eTDAL_OUTPUT_NO_ERROR,
								   "output audio SPDIF audio mode set to OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED" );
		   TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
								   eTDAL_OUTPUT_NO_ERROR,
								   "1st output audio SPDIF enable" );
		}
		 TKEL_DelayTask(ms2tick(5000));
			printf("!!!!!!!!!!!!!!!!!!!!!!! ANALOG uiNbChange %d \n",uiNbChange);

        /* TestManager_AskForChecking( "do you see ANALOG video and audio playing ?" ); */

        TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
								eTDAL_ANA_VID_NO_ERROR,
								"TDAL_ANA_VID_Stop succeed");
        TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_Stop succeed");

        TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
								eTDAL_DISP_NO_ERROR,
								"disp layer disable" );
		if(stOutputCap.nbVideoDigital > 0)
		{
			TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DIGITAL_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output video digital disable");
		}
		if(stOutputCap.nbVideoHDDAC > 0)
		{
			TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
									eTDAL_OUTPUT_NO_ERROR,
									"output hd dac disable");
		}
		if(stOutputCap.nbAudioSPDIF > 0)
		{
		   TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
								   eTDAL_OUTPUT_NO_ERROR,
								   "output audio SPDIF disable" );
		}
		if(stOutputCap.nbAudioAna > 1)
		{
			/* we suppose that when more than one audio ana, the second is a headphone */
			TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
									eTDAL_OUTPUT_NO_ERROR,
									"output audio headphone disable" );
		}
		uiNbChange++;
}
while(uiNbChange < 5);
		/* Terminate test */
		
		/* swicth to DTV mode */
		TestHelper_Configure_TDAL_ConnectionManager(eCONNECTION_MANAGER_TYPE_DIGITAL);
		
		if(bDTV_Section)
		{
			TestSce_AtvDtvDMXSectionFree();
		}

		TestHelper_Unconfigure_TDAL_DMX();
		TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(0),
											TEST_HELPER_TSROUTE_DEMUX(0));
		TestManager_AssertEqual(TDAL_TSROUTE_Term(),
								eTDAL_TSROUTE_NO_ERROR,
								"tsroute terminate");

		TestManager_AssertEqual(TDAL_AV_Terminate(),
								eTDAL_AV_NO_ERROR,
								"av terminate");

        TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);
        TestHelper_Terminate_ANA_VID(IndexVideoDecoder);
		
		TestManager_AssertEqual(TDAL_SPEAKERS_Term(),
								eTDAL_SPEAKERS_NO_ERROR,
								"speakers terminate" );
		TestManager_AssertEqual(TDAL_PANEL_Term(),
								eTDAL_PANEL_NO_ERROR,
								"panel terminate" );
        TestManager_AssertEqual(TDAL_DISP_Term(),
								eTDAL_DISP_NO_ERROR,
								"disp terminate" );
        TestManager_AssertEqual(TDAL_OUTPUT_Term(),
								eTDAL_OUTPUT_NO_ERROR,
								"output terminate" );

        TestHelper_Terminate_TDAL_DMD();
		
		bUseAnalogChannel = FALSE;
    }
}


static void TestSce_AtvDtvDMXSectionAllocate( void )
{
    tTDAL_DMX_ChannelType_t      type;
    uint32_t                     filter_track = 0;
    uint8_t                      size = 1;
    uint8_t                      HardValue[1] = {0x42};
    uint8_t                      HardMask[1] = {0xFF};
    uint8_t                      HardMode[1] = {0xFF};
    tTDAL_DMX_ChannelCallbackFct callback;

    TestManager_AssertEqual( TKEL_CreateSemaphore(0,&TestSce_AtvDtvDMXSectionSem),
                             TKEL_NO_ERR,
                             "TKEL CreateSemaphore" );

    type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
    type.eChannelOutput     = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;

    TestManager_AssertEqual(TDAL_DMX_Allocate_Channel( eTDAL_DMX_DEMUX0,
													   &type,
													   &TestSce_AtvDtvDMXSectionChId ),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate channel" );

    callback = TestSce_AtvDtvDMXSectionNotifyCbk;

    TestManager_AssertEqual(TDAL_DMX_Register_Channel_Callback( TestSce_AtvDtvDMXSectionChId,
																callback ),
							kTDAL_DMX_NO_ERROR,
							"DMX register Channel CallBack" );
    TestManager_AssertEqual(TDAL_DMX_Allocate_Filter( TestSce_AtvDtvDMXSectionChId,
													  &TestSce_AtvDtvDMXSectionFiltId ),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate filter" );
    TestManager_AssertEqual(TDAL_DMX_Set_Channel_PID( TestSce_AtvDtvDMXSectionChId,
													  0x0011 ),
							kTDAL_DMX_NO_ERROR,
							"DMX Set Channel PID" );

    TestManager_AssertEqual(TDAL_DMX_Set_Filter( TestSce_AtvDtvDMXSectionChId,
												 TestSce_AtvDtvDMXSectionFiltId,
												 filter_track,
												 size,
												 &HardValue[0],
												 &HardMask[0],
												 &HardMode[0] ),
							kTDAL_DMX_NO_ERROR,
							"DMX set filter" );


	/*  allocate PES */
    type.eChannelStream     = eTDAL_DMX_SUBTITLE_STREAM;
    type.eChannelOutput     = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;

#if defined(SUB_PES)
printf(" allocate channel PES SUB \n");

    TestManager_AssertEqual(TDAL_DMX_Allocate_Channel( eTDAL_DMX_DEMUX0,
													   &type,
													   &TestSce_AtvDtvDMXSubPesChId ),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate PES channel" );

printf(" allocate filter PES SUB \n");

    TestManager_AssertEqual(TDAL_DMX_Allocate_Filter( TestSce_AtvDtvDMXSubPesChId,
													  &TestSce_AtvDtvDMXSubPesFiltId ),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate PES filter" );

	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( TestSce_AtvDtvDMXSubPesFiltId,
													   0x2e4 ), /* ARTE HD subt pes pid */
							kTDAL_DMX_NO_ERROR, "DMX Set PES Channel PID" );

printf(" start filter PES SUB \n");

    TestManager_AssertEqual(TDAL_DMX_Control_Filter( TestSce_AtvDtvDMXSubPesFiltId,
													  eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate PES filter" );


#endif
	
}
	
static void TestSce_AtvDtvDMXSectionEnableWait( void )
{
    TestManager_AssertEqual(TDAL_DMX_Control_Channel( TestSce_AtvDtvDMXSectionChId,
													  eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,
							"DMX control channel" );

    TestManager_AssertEqual(TDAL_DMX_Control_Filter( TestSce_AtvDtvDMXSectionFiltId,
													 eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,
							"DMX control filter" );
#if defined(SUB_PES)
    TestManager_AssertEqual(TDAL_DMX_Control_Channel( TestSce_AtvDtvDMXSubPesChId,
													  eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,
							"DMX control PES channel" );
#endif	

    TestManager_AssertEqual( TKEL_WaitSemaphore( TestSce_AtvDtvDMXSectionSem ),
                             TKEL_NO_ERR,
                             "TKEL Wait Semaphore" );
}

static void TestSce_AtvDtvDMXSectionDisable( void )
{
	TestManager_AssertEqual(TDAL_DMX_Control_Channel (TestSce_AtvDtvDMXSectionChId,
													  eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR,
							"DMX control channel" );
#if defined(SUB_PES)
    TestManager_AssertEqual(TDAL_DMX_Control_Channel( TestSce_AtvDtvDMXSubPesChId,
													  eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR,
							"DMX control PES channel" );
#endif
}

static void TestSce_AtvDtvDMXSectionFree( void )
{
    TestManager_AssertEqual(TDAL_DMX_Free_Channel( TestSce_AtvDtvDMXSectionChId ),
							kTDAL_DMX_NO_ERROR,
							"DMX Free Channel" );
#if defined(SUB_PES)
    TestManager_AssertEqual(TDAL_DMX_Free_Channel( TestSce_AtvDtvDMXSubPesChId ),
							kTDAL_DMX_NO_ERROR,
							"DMX control PES channel" );
#endif
}

static void TestSce_AtvDtvDMXSectionNotify(uint8_t* buffer, uint32_t length)
{
        int i;
        char outStr[100];

        outStr[0] = '\0';

        if (buffer[0] == 0x00)
        {
                strcat(outStr,"PAT |" );
        }
        if (buffer[0] == 0x70)
        {
                strcat(outStr,"TDT |" );
        }
        if (buffer[0] == 0x73)
        {
                strcat(outStr, "TOT |" );
        }
        if (buffer[0] == 0x40)
        {
                strcat(outStr, "NITa |");
        }
        if (buffer[0] == 0x41)
        {
                strcat(outStr, "NITo |");
        }
        if (buffer[0] == 0x42)
        {
                strcat(outStr, "SDTa |");
        }
        if (buffer[0] == 0x46)
        {
                strcat(outStr, "SDTo |");
        }
        for (i=0; i<10; i++)
        {
                sprintf(&outStr[strlen(outStr)], " 0x%02x", buffer[i]);
        }

        mTBOX_PRINT((kTBOX_LF,"sn-> %s\r\n",outStr ));

}

static void TestSce_AtvDtvDMXSectionNotifyCbk(tTDAL_DMX_ChannelId ChannelId,
											  tTDAL_DMX_FilterId FilterId,
											  uint32_t Filter_track,
											  uint32_t Length,
											  uint8_t *Buffer)
{
    TKEL_err tkError;

    mTBOX_PRINT((kTBOX_LF,"Section received on filter %d, Section size %d, Buffer address %8x \n",FilterId,Length,Buffer ));

    /*  check the section received and print info  */
    TestSce_AtvDtvDMXSectionNotify(Buffer,Length);

    /*  free the TDAL buffer  */
    /* tkError = TKEL_Free(Buffer); */
    if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
    }

    tkError = TKEL_SignalSemaphore(TestSce_AtvDtvDMXSectionSem);
    if (tkError != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tkError ));
    }

}

