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
 *              DMD test tree                         *
 ******************************************************/

static tTestNode gTestScenarioTuneAnalogFreq= {
	"TestScenarioTuneAnalogFreq",
	"Tune on a analog Frequency",
	0,
	TestSce_TuneAnalogFreq,
	NULL
};

static tTestNode gTestScenarioTuneAnalogTwoFreq= {
	"TestScenarioTuneAnalogTwoFreq",
	"Send two tune command on analog Frequency ",
	0,
	TestSce_TuneAnalogTwoFreq,
	NULL
};

static tTestNode gTestScenarioTuneAnalogUnplugg= {
	"TestScenarioTuneAnalogUnplugg",
	"Tune on a analog Frequency and unplugg the cable",
	0,
	TestSce_TuneAnalogUnplugg,
	NULL
};

static tTestNode gTestScenarioTuneAnalogDigital= {
	"TestScenarioTuneAnalogDigital",
	"Tune on a analog Frequency and on a digital Frequency",
	0,
	TestSce_TuneAnalogDigital,
	NULL
};

static tTestNode gTestScenarioTuneVideoAudioAnalog= {
	"TestScenarioTuneVideoAudioAnalog",
	"Play Video and Audio from an Analog Frequency",
	0,
	TestSce_TuneVideoAudioAnalog,
	NULL
};

static tTestNode gTestScenarioAnaInputVideoAudioAnalog= {
	"TestScenarioAnaInputVideoAudioAnalog",
	"Play Video and Audio from an analog input",
	0,
	TestSce_AnaInputVideoAudioAnalog,
	NULL
};

static tTestNode gTestScenarioAnaScanning= {
	"gTestScenarioAnaScanning",
	"Make a scanning on analog signal",
	0,
	TestSce_ScanningAnalog,
	NULL
};


static tTestNode gTestScenarioHDMIInTest= {
	"TestScenarioHDMIInTest",
	"play video and audio from HDMI input",
	0,
	TestSce_HDMIInputVideoAudioPlay,
	NULL
};


static tTestNode gTestScenario2AnaInputVideoAudioAnalogPIP = {
	"TestScenarioAnaInputVideoAudioAnalogPIP",
	"play video and audio from two analog sources in PIP",
	0,
	TestSce_2AnaInputVideoAudioAnalogPIP,
	NULL
};

static tTestNode gTestScenarioTunerAnaInputVideoAudioAnalogPIP = {
	"TestScenarioTunerAnaInputVideoAudioAnalogPIP",
	"play video and audio from two analog sources (main is tuner) in PIP",
	0,
	TestSce_TunerAnaInputVideoAudioAnalogPIP,
	NULL
};


static tTestNode gTestScenario1AnaInput1HDMIVideoAudioAnalogPIP = {
	"TestScenario1AnaInput1HDMIVideoAudioAnalogPIP",
	"play video and audio from 1 analog sources 1 HDMI in PIP",
	0,
	TestSce_1AnaInput1HDMIVideoAudioAnalogPIP,
	NULL
};

static tTestNode gTestScenarioDigitalTunerInputVideoAudioAnalogPIP = {
	"TestScenario1DigitalTunerInputVideoAudioAnalogPIP",
	"play video and audio from digital tuner input and 1 analog source in PIP",
	0,
	TestSce_1TunerDigital1AnalogVideoAudioAnalogPIP,
	NULL
};


tTestNode* gANALOGScenarioTestNodeList[] = {
	&gTestScenarioTuneAnalogFreq,
    &gTestScenarioTuneAnalogTwoFreq,
    &gTestScenarioTuneAnalogUnplugg,
    &gTestScenarioTuneAnalogDigital,
    &gTestScenarioTuneVideoAudioAnalog,
    &gTestScenarioAnaInputVideoAudioAnalog,
    &gTestScenarioAnaScanning,
    &gTestScenarioHDMIInTest,
    &gTestScenario2AnaInputVideoAudioAnalogPIP,
	&gTestScenarioTunerAnaInputVideoAudioAnalogPIP,
	&gTestScenario1AnaInput1HDMIVideoAudioAnalogPIP,
	&gTestScenarioDigitalTunerInputVideoAudioAnalogPIP,
	NULL
};

tTestNode gTDAL_ANALOG_ScenarioTestSuite = {
	"TDAL_ANA Scenario Tests",
	"test TDAL_ANA scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gANALOGScenarioTestNodeList
};

extern int gChannel;
extern int gChannelAnalog;
extern bool bUseAnalogChannel;
extern struct TestManager_Channel_Analog gAnalogChannelList[];

void ANA_VID_CallbackProc( tTDAL_ANA_VID_Decoder eDecoder,
                           tTDAL_ANA_VID_Event   eEvent,
                           const void           *pEventData );
void ANA_AUD_CallbackProc( tTDAL_ANA_AUD_Decoder eDecoder,
                           tTDAL_ANA_AUD_Event   eEvent,
                           const void           *pEventData );

void VBI_SLICER_CallbackVPS( tTDAL_VBI_SLICER_ClientId  stClientId,
                             uint32_t                   ulInputHandle,
                             uint32_t                   ulVpsDataMsb,
                             uint32_t                   ulVpsDataLsb);

void VBI_SLICER_CallbackWST( tTDAL_VBI_SLICER_ClientId  stClientId,
                             uint32_t                   ulInputHandle,
                             uint16_t                   usSizeOfData,
                             const void                 *pData);

void VBI_SLICER_CallbackWSS( tTDAL_VBI_SLICER_ClientId  stClientId,
                             uint32_t                   ulInputHandle,
                             uint16_t                   usWssData);

void TestStartStopAnalogModule( unsigned char ucFeID,
                                bool Start);


void SCART_INPUT_CallbackFct
(
    tTDAL_SCART_INPUT_Id     eId,
    tTDAL_SCART_INPUT_Event  eEvent,
    const void              *pEventData
);

void TestSce_HDMIInput_Callback (tTDAL_HDMI_IN_SinkId     eId,
                                       tTDAL_HDMI_IN_Event      eEvent,
                                       const void               *pEventData);

void TestSce_HDMIInput_CallbackSub (tTDAL_HDMI_IN_SinkId     eId,
                                                 tTDAL_HDMI_IN_Event      eEvent,
                                                 const void               *pEventData);

bool AudioTrackSet = false;

/******************************************************
 *              Scenario tests                        *
*******************************************************/
void Generic_TuneAnalogFreq( int bWithOneTune, int bWithTwoTune, int bWithUnplugg, int bWithTuneDigital )
{
	unsigned char               ucFeID;
	tTDAL_DMD_NotifyStatus      DMD_NotifyCond;
    tTDAL_DMD_FECapability      stFECapability;

    TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_ATV);
    TestManager_AssertEqual(TDAL_DMD_GetFECapability( ucFeID, &stFECapability), eTDAL_DMD_NO_ERROR, "TDAL_DMD_GetFECapability succeed");
    if(stFECapability.eSourceSupport == eTDAL_DMD_DIGITAL)
    {
        TestManager_LogMessage("Tuner doesn't support the anolog\n");
        TestHelper_DMD_Term(&ucFeID);
        return;
    }
    else
    {
        if(bWithOneTune)
        {
            /*      Tuning on First Channel Analog     */
            gChannelAnalog = 0;
            bUseAnalogChannel = TRUE;
            TestHelper_DMD_Tune(&ucFeID);
        	/*TestHelper_DMD_WaitStatus(ucFeID); not needed, already done in tune */
            TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
            TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
            mTBOX_PRINT((kTBOX_LF, "Waiting 2 sec and check that no other notification is received\n"));
            TKEL_DelayTask(ms2tick(2000));
            TestHelper_DMD_GetInfo(&ucFeID);
        }

        if(bWithTwoTune)
        {
            /*      Tuning on First Channel Analog     */
            gChannelAnalog = 0;
            bUseAnalogChannel = TRUE;
            TestHelper_DMD_Tune(&ucFeID);
            gChannelAnalog = 1;
            /*      Tuning on Second Channel Analog     */        	
            TestHelper_DMD_Tune(&ucFeID);
        	/* TestHelper_DMD_WaitStatus(ucFeID); not needed, already done in tune */
        	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
        	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
            /* must be lock on the second frequency */
        	TestHelper_DMD_GetInfo(&ucFeID);
            mTBOX_PRINT((kTBOX_LF, "Frequency from array =%d\n", gAnalogChannelList[gChannelAnalog].TunFrequency));
            TestManager_AskForChecking( "The two frequencies(Get Info and Array must be similar)" );
        }

        if(bWithUnplugg)
        {
            /* unplugg the cable, event signal lost must be sent*/
            TestManager_AskForAction("Please unplug the antenna cable");
        	TestHelper_DMD_WaitStatus(ucFeID);
        	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
        	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_SIGNAL_LOST, "signal lost");
            /* plugg the cable, event signal lock must be sent*/
            TestManager_AskForAction("Please replug the antenna cable");
        	TestHelper_DMD_WaitStatus(ucFeID);
        	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
        	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
            /* unplugg the cable, event signal lost must be sent*/
            TestManager_AskForAction("Please unplug again the antenna cable");
        	TestHelper_DMD_WaitStatus(ucFeID);
        	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
        	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_SIGNAL_LOST,"signal lost");
            /* plugg the cable, event signal lock must be sent for the new frequency */
            TestManager_AskForAction("Please replug the antenna cable");
        	gChannelAnalog = 1;
            TestHelper_DMD_Tune(&ucFeID);
        	TestHelper_DMD_WaitStatus(ucFeID);
        	TestManager_AskForChecking("Press 1 when cable is replugged");
        	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
        	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
        	TestHelper_DMD_GetInfo(&ucFeID);
        }

        if(bWithTuneDigital)
        {
            if(stFECapability.eSourceSupport != eTDAL_DMD_HYBRID)
            {
                TestManager_LogMessage("Tuner doesn't support the Digital, test not available\n");
            }
            else
            {
                /* Tune an Digital Frequency */
                TestHelper_DMD_Term(&ucFeID);
                TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
                /*      Tuning on First Channel Digital     */
                gChannel = 0;
                bUseAnalogChannel = FALSE;
                TestHelper_DMD_Tune(&ucFeID);
            	TestHelper_DMD_WaitStatus(ucFeID);
                TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
                TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
                TestHelper_DMD_GetInfo(&ucFeID);

                /* Re-tune an analog freq */
                TestHelper_DMD_Term(&ucFeID);
                TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_ATV);
                /*      Tuning on First Channel Digital     */
                gChannelAnalog = 0;
                bUseAnalogChannel = TRUE;
                TestHelper_DMD_Tune(&ucFeID);
            	TestHelper_DMD_WaitStatus(ucFeID);
                TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
                TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
                TestHelper_DMD_GetInfo(&ucFeID);
            }
        }
    }

    bUseAnalogChannel = FALSE;
    gChannel = 0;
    TestHelper_DMD_Term(&ucFeID);
}

void TestSce_TuneAnalogFreq( void )
{
    Generic_TuneAnalogFreq(1, 0, 0, 0);
}

void TestSce_TuneAnalogTwoFreq( void )
{
    Generic_TuneAnalogFreq(0, 1, 0, 0);
}

void TestSce_TuneAnalogUnplugg( void )
{
    Generic_TuneAnalogFreq(1, 0, 1, 0);
}

void TestSce_TuneAnalogDigital( void )
{
    Generic_TuneAnalogFreq(1, 0, 0, 1);
}

void TestSce_ScanningAnalog( void )
{
	unsigned char           ucFeID;
	tTDAL_DMD_ScanData      DMD_Scan;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
	uint16_t                FreqFound = 0;

    DMD_Scan.AnalogTer.eStandardSystem = eTDAL_DMD_ANALOG_STD_SECAM_L;
	DMD_Scan.AnalogTer.bEnableDriftMonitoring   = TRUE;
    DMD_Scan.AnalogTer.lManualDriftToApply      = 0;
	bUseAnalogChannel = TRUE;

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_ATV);

    TestManager_AssertEqual(TDAL_DMD_StartScan(ucFeID, &DMD_Scan, 474000000, 858000000), eTDAL_DMD_NO_ERROR, "TDAL_DMD_StartScan");

	TestHelper_DMD_WaitStatus(ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_FOUND, "signal locked");

	if(DMD_NotifyCond == eTDAL_DMD_FOUND)
	{
		FreqFound++;
		TestHelper_DMD_GetInfo(&ucFeID);
        TestStartStopAnalogModule(ucFeID, TRUE);
        TestManager_AskForChecking( "press a key to continue the scanning" );
	}
	while(DMD_NotifyCond == eTDAL_DMD_FOUND)
	{
	    TestManager_AssertEqual(TDAL_DMD_ContScan(ucFeID), eTDAL_DMD_NO_ERROR, "TDAL_DMD_ContScan");
		TestHelper_DMD_WaitStatus(ucFeID);
		TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
		
        if(DMD_NotifyCond == eTDAL_DMD_FOUND)
		{
			FreqFound++;
            mTBOX_PRINT((kTBOX_LF,"signal locked\n" ));
            TestStartStopAnalogModule(ucFeID, TRUE);
            TestManager_AskForChecking( "press a key to continue the scanning" );
		}	
	}
    TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_END_OF_RANGE, "End of scanning");

	mTBOX_PRINT((kTBOX_LF,"Frequencies found -> %d\n",FreqFound));

    TestManager_AssertEqual(TDAL_DMD_StopScan(ucFeID), eTDAL_DMD_NO_ERROR, "TDAL_DMD_StopScan");
    TestStartStopAnalogModule(ucFeID, FALSE);

    bUseAnalogChannel = FALSE;
	TestHelper_DMD_Term(&ucFeID);
}

void TestSce_TuneVideoAudioAnalog( void )
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
    tTDAL_VBI_SLICER_Capability             stDecoderCapability;
    tTDAL_VBI_SLICER_ClientId               stClientId;
	tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;

	AudioTrackSet = false;
    /*Get tuner capability*/
    TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_ATV);
    TestManager_AssertEqual(TDAL_DMD_GetFECapability( ucFeID, &stFECapability),
							eTDAL_DMD_NO_ERROR,
							"TDAL_DMD_GetFECapability succeed");

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
    if(stFECapability.eSourceSupport == eTDAL_DMD_DIGITAL)
    {
        TestManager_LogMessage("Tuner doesn't support the anolog\n");
        TestHelper_DMD_Term(&ucFeID);
        return;
    }
    else
    {
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_NONE;
		stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_SINGLE;
		TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
								TRUE,
								"ConnectionManager selectUseCase");

        /* Initialise modules */


		TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);

        TestManager_AssertEqual(TDAL_DMD_HandleGet(ucFeID, &DemodStreamHandle),
								eTDAL_DMD_NO_ERROR,
								"TDAL_DMD_HandleGet succeed");

		/* Initialisation for Analog Video*/
        TestHelper_Initialize_ANA_VID( &IndexVideoDecoder,
									   &stVideoDecoderCapability,
									   eTDAL_ANA_VID_CODING_CVBS  );
printf("IndexVideoDecoder %d\n",IndexVideoDecoder);
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

        /* Initialisation for Analog Audio*/
        TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

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

        /*Initialisation for vbi slicer */
        TestHelper_Initialize_VBI_SLICER( &stDecoderCapability );
        TestHelper_RegisterClient_VBI_SLICER(&stClientId, &stDecoderCapability);
        /*configure the input*/
        TestManager_AssertEqual(TDAL_VBI_SLICER_ConfigureClientInput(stClientId,
																	 DemodStreamHandle),
								eTDAL_VBI_SLICER_NO_ERROR,
								"TDAL_VBI_SLICER_ConfigureClientInput succeed");
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

        /*start the Vbi Slicer */
#if 0
        TestManager_AssertEqual(TDAL_VBI_SLICER_EnableInputSlicing(stClientId,DemodStreamHandle),
								eTDAL_VBI_SLICER_NO_ERROR,
								"TDAL_VBI_SLICER_EnableInputSlicing succeed");
#endif
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
								   "output audio SPDIF enable" );
		}

        TestManager_AskForChecking( "do you see video and audio playing ?" );
        TestManager_AskForChecking( "do you see some WSS/WST/VPS Data with the callbacks ?" );

		TestHelper_CheckPanelSettings();
		TestHelper_CheckSpeakersSettings();

#if 1
        TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
								eTDAL_ANA_VID_NO_ERROR,
								"TDAL_ANA_VID_Stop succeed");
        TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_Stop succeed");


        /*      Tuning on second Channel Analog     */
        gChannelAnalog = 1;
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
        TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder, &stVideoStartParams ),
								eTDAL_ANA_VID_NO_ERROR,
								"IndexVideoDecoder succeed");

        TestManager_AskForChecking( "do you see video and audio playing on second channel ?" );
#endif

        /* close all modules*/
        TestManager_AssertEqual(TDAL_VBI_SLICER_DisableInputSlicing(stClientId,DemodStreamHandle),
								eTDAL_VBI_SLICER_NO_ERROR,
								"TDAL_VBI_SLICER_DisableInputSlicing succeed");
        TestHelper_UnRegisterClient_VBI_SLICER(&stClientId, &stDecoderCapability);
        TestManager_AssertEqual(TDAL_VBI_SLICER_Term(),
								eTDAL_VBI_SLICER_NO_ERROR,
								"TDAL_VBI_SLICER_Term succeed");

        TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
								eTDAL_ANA_VID_NO_ERROR,
								"TDAL_ANA_VID_Stop succeed");
        TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_Stop succeed");
        TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);
        TestHelper_Terminate_ANA_VID(IndexVideoDecoder);

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

void TestSce_AnaInputVideoAudioAnalog( void )
{
    uint32_t                                AnaInputStreamHandle, LayerStreamHandle;
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
    tTDAL_VBI_SLICER_Capability             stVbiSLicerDecoderCapability;
    tTDAL_VBI_SLICER_ClientId               stClientId;
	tTDAL_ANA_VID_Coding                    eVidCoding;
	tTDAL_ANA_INPUT_Object                  VidMuxObj;
	tTDAL_ANA_INPUT_Object                  VidObj;
	tTDAL_ANA_INPUT_Object                  AudMuxObj;
    tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;


    TestHelper_Initialize_ANA_INPUT ( &VidObj,&VidMuxObj, &AudMuxObj, &eVidCoding );


	/* Ana Input initialisation */
    TestHelper_Initialize_SCART_INPUT ();



    /* Initialise modules */
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


	TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);

    /* Initialisation for Analog Video*/
    TestHelper_Initialize_ANA_VID ( &IndexVideoDecoder, &stVideoDecoderCapability,eVidCoding  );

	if(eVidCoding == eTDAL_ANA_VID_CODING_CVBS ||
	   eVidCoding == eTDAL_ANA_VID_CODING_YC ||
	   eVidCoding == eTDAL_ANA_VID_CODING_RGB_S)
	{
    	stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_SCART;
    	stUseCase.eSecondVideoInput = eTDAL_USE_CASE_NONE;
	    stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_SINGLE;
	}
	else  if(eVidCoding == eTDAL_ANA_VID_CODING_YPRPB)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_YPrPb;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_NONE;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_SINGLE;
		
	}
	else if(eVidCoding == eTDAL_ANA_VID_CODING_RGB_HV)
	{
		/* PC mode */
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_PC;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_NONE;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_SINGLE;
		
	}
    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(VidObj,VidMuxObj),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );

    /* Set the way for the video */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(VidMuxObj, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet video");

	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder,
														AnaInputStreamHandle),
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

    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoder,
														LayerStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet succeed");

    /* Initialisation for Analog Audio*/
    TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

    /* Set the way for the audio */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(AudMuxObj, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet audio");
	TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder,
														AnaInputStreamHandle),
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
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_DestinationSet FPD succeed");	

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

    /*Initialisation for vbi slicer */
    TestHelper_Initialize_VBI_SLICER( &stVbiSLicerDecoderCapability );

    TestHelper_RegisterClient_VBI_SLICER(&stClientId, &stVbiSLicerDecoderCapability);


    /*configure the input*/
    TestManager_AssertEqual(TDAL_VBI_SLICER_ConfigureClientInput(stClientId,
																 AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_ConfigureClientInput succeed");

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


    /*start audio and video Analog */
    stAudioStartParams.uiStdMask = stAudioDecoderCapability.uiSupportedStdMask;
    TestManager_AssertEqual(TDAL_ANA_AUD_Start(IndexAudioDecoder,
											   &stAudioStartParams),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Start succeed");

	stVideoStartParams.uiCodingMask  = eVidCoding;
	stVideoStartParams.uiStdMask     = stVideoDecoderCapability.uiSupportedStdMask;/*eTDAL_ANA_VID_STD_SECAM;*/
    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start succeed");

    /*start the Vbi Slicer */
    TestManager_AssertEqual(TDAL_VBI_SLICER_EnableInputSlicing(stClientId,AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_EnableInputSlicing succeed");

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
							   "output audio SPDIF enable" );
	}

    TestManager_AskForChecking( "do you see video and audio playing ?" );
    TestManager_AskForChecking( "do you see some WSS/WST/VPS Data with the callbacks ?" );

	/*TestHelper_CheckPanelSettings();
	TestHelper_CheckSpeakersSettings();*/
	
	if(eVidCoding == eTDAL_ANA_VID_CODING_RGB_HV)
	{
		TestHelper_CheckPCSettings();
	}	


	/*  check scart input mute outputs */
    TestManager_AskForChecking( "do you see video an audio on 1 scart output ?" );


	{
		tTDAL_SCART_INPUT_SlowBlankingSignal signal;
	TestManager_AssertEqual(TDAL_SCART_INPUT_GetSlowBlanking(eTDAL_SCART_INPUT_ID_0,&signal),
							eTDAL_SCART_INPUT_NO_ERROR,
							"scart input 1 get slow blanking " );

	printf("signal on scart 1 found %d \n",signal);
	}
	
	TestManager_AssertEqual(TDAL_SCART_INPUT_MuteOutputs(eTDAL_SCART_INPUT_ID_0,TRUE),
							eTDAL_SCART_INPUT_NO_ERROR,
							"scart input Mute outputs" );

    TestManager_AskForChecking( "is the video an audio mutes on the scart output ?" );

	TestManager_AssertEqual(TDAL_SCART_INPUT_MuteOutputs(eTDAL_SCART_INPUT_ID_0,FALSE),
							eTDAL_SCART_INPUT_NO_ERROR,
							"scart input Mute outputs" );
    TestManager_AskForChecking( "is the video an audio unmutes on the scart output ?" );


    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

	if(stOutputCap.nbAudioAna > 1)
	{
		/* we suppose that when more than one audio ana, the second is a headphone */
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio headphone disable" );
	}
	if(stOutputCap.nbAudioSPDIF > 0)
	{
	   TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
							   eTDAL_OUTPUT_NO_ERROR,
							   "output audio SPDIF disable" );
	}

    /* close all modules*/
    TestManager_AssertEqual(TDAL_VBI_SLICER_DisableInputSlicing(stClientId,AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_DisableInputSlicing succeed");
    TestHelper_UnRegisterClient_VBI_SLICER(&stClientId,
										   &stVbiSLicerDecoderCapability);
    TestManager_AssertEqual(TDAL_VBI_SLICER_Term(),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_Term succeed");

    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");
    TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Stop succeed");
    TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);
    TestHelper_Terminate_ANA_VID(IndexVideoDecoder);

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer disable" );
    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio disable" );
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
    TestManager_AssertEqual(TDAL_SCART_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_SCART_INPUT_Term succeed");

    TestManager_AssertEqual(TDAL_ANA_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_Term succeed");

}


void TestSce_2AnaInputVideoAudioAnalogPIP( void )
{
    uint32_t                                AnaInputStreamHandle, LayerStreamHandle, LayerStreamHandleSub;
    tTDAL_ANA_VID_Decoder                   IndexVideoDecoder,IndexVideoDecoderSub;
    tTDAL_ANA_AUD_Decoder                   IndexAudioDecoder;
    tTDAL_ANA_AUD_DecoderCapability         stAudioDecoderCapability;
    tTDAL_ANA_VID_DecoderCapability         stVideoDecoderCapability,stVideoDecoderCapabilitySub;
    tTDAL_ANA_AUD_StartParams               stAudioStartParams;
    tTDAL_ANA_VID_StartParams               stVideoStartParams,stVideoStartParamsSub;
    tTDAL_OUTPUT_VideoDigitalParams         stVideoDigitalParams;
	tTDAL_OUTPUT_VideoHDDACParams           stHDDACParams;
	tTDAL_OUTPUT_Capability                 stOutputCap;
    tTDAL_OUTPUT_AudioAnaCapability         stAudioCapability;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
#if 0
    tTDAL_VBI_SLICER_Capability             stVbiSLicerDecoderCapability;
    tTDAL_VBI_SLICER_ClientId               stClientId;
#endif
	tTDAL_ANA_VID_Coding                    eVidCoding,eVidCodingSub;
	tTDAL_ANA_INPUT_Object                  VidMuxObj,VidMuxObjSub;
	tTDAL_ANA_INPUT_Object                  VidObj,VidObjSub;
	tTDAL_ANA_INPUT_Object                  AudMuxObj;
    tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_ANA_VID_Event             		eIndexEvent;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;

	tTDAL_UseCaseInputType                  eUseCaseInputTemp;

mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the main source                  \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


    TestHelper_Initialize_ANA_INPUT ( &VidObj,&VidMuxObj, &AudMuxObj, &eVidCoding );

mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the main source end                 \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the secondary video source       \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


    TestHelper_Initialize_ANA_INPUT_SUB ( &VidObjSub,&VidMuxObjSub, &eVidCodingSub );


mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the secondary video source end   \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


	/* Ana Input initialisation */
    TestHelper_Initialize_SCART_INPUT ();



    /* Initialise modules */
    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_SPEAKERS_Init(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers init" );

    TestManager_AssertEqual(TDAL_PANEL_Init(),
							eTDAL_PANEL_NO_ERROR,
							"panel init" );


	TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);

    /* Initialisation for Analog Video*/
    TestHelper_Initialize_ANA_VID ( &IndexVideoDecoder, &stVideoDecoderCapability,eVidCoding  );
    TestHelper_Initialize_ANA_VID_SUB ( &IndexVideoDecoderSub, &stVideoDecoderCapabilitySub,eVidCodingSub  );
	
	if((eVidCoding == eTDAL_ANA_VID_CODING_CVBS ||
	   eVidCoding == eTDAL_ANA_VID_CODING_YC) &&
	    eVidCodingSub == eTDAL_ANA_VID_CODING_YPRPB)
	{
    	stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_SCART;
    	stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_YPrPb;
	    stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	}
	else if((eVidCodingSub == eTDAL_ANA_VID_CODING_CVBS ||
	          eVidCodingSub == eTDAL_ANA_VID_CODING_YC) &&
	          eVidCoding == eTDAL_ANA_VID_CODING_YPRPB)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_YPrPb;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_SCART;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	}

	else  if((eVidCoding == eTDAL_ANA_VID_CODING_CVBS ||
	          eVidCoding == eTDAL_ANA_VID_CODING_YC) &&
	          eVidCodingSub == eTDAL_ANA_VID_CODING_RGB_HV)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_SCART;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_PC;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
		
	}
	else if((eVidCodingSub == eTDAL_ANA_VID_CODING_CVBS ||
	          eVidCodingSub == eTDAL_ANA_VID_CODING_YC) &&
	          eVidCoding == eTDAL_ANA_VID_CODING_RGB_HV)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_PC;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_SCART;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	}

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(VidObj,VidMuxObj),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(VidObjSub,VidMuxObjSub),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );


    /* Set the way for the video */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(VidMuxObj, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet video");

	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder,
														AnaInputStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_InputStreamSet succeed");

    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(VidMuxObjSub, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet video sub");
	
	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoderSub,
														AnaInputStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_InputStreamSet sub succeed");


    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
													 (void*)&(LayerStreamHandle)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_1,
													 (void*)&(LayerStreamHandleSub)),
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



    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsGet succeed");

	stOutputWindow.Top = 0;
	stOutputWindow.Bottom = 576;
	stOutputWindow.Left= 0;
	stOutputWindow.Right = 720;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsSet succeed");


    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoder,
														LayerStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoderSub,
														LayerStreamHandleSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet succeed");


    /* Initialisation for Analog Audio*/
    TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

    /* Set the way for the audio */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(AudMuxObj, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet audio");

	TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder,
														AnaInputStreamHandle),
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
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_DestinationSet FPD succeed");	

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

    /*Initialisation for vbi slicer */
#if 0
    TestHelper_Initialize_VBI_SLICER( &stVbiSLicerDecoderCapability );

    TestHelper_RegisterClient_VBI_SLICER(&stClientId, &stVbiSLicerDecoderCapability);


    /*configure the input*/
    TestManager_AssertEqual(TDAL_VBI_SLICER_ConfigureClientInput(stClientId,
																 AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_ConfigureClientInput succeed");
#endif
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


    /*start audio and video Analog */
    stAudioStartParams.uiStdMask = stAudioDecoderCapability.uiSupportedStdMask;
    TestManager_AssertEqual(TDAL_ANA_AUD_Start(IndexAudioDecoder,
											   &stAudioStartParams),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Start succeed");

	stVideoStartParams.uiCodingMask  = eVidCoding;
	stVideoStartParams.uiStdMask     = stVideoDecoderCapability.uiSupportedStdMask;/*eTDAL_ANA_VID_STD_SECAM;*/

	stVideoStartParamsSub.uiCodingMask  = eVidCodingSub;
	stVideoStartParamsSub.uiStdMask     = stVideoDecoderCapabilitySub.uiSupportedStdMask;/*eTDAL_ANA_VID_STD_SECAM;*/


    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoderSub,
											   &stVideoStartParamsSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start sub succeed");


    /*start the Vbi Slicer */
#if 0
    TestManager_AssertEqual(TDAL_VBI_SLICER_EnableInputSlicing(AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_EnableInputSlicing succeed");
#endif

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
	/*-------------------------*/
	/* result                  */
	/*-------------------------*/

    TestManager_AskForChecking( "do you see 2 video and audio playing ?" );


    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoderSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");


	/* swap sources */
	 eUseCaseInputTemp = stUseCase.eFirstVideoInput;	
	 stUseCase.eFirstVideoInput = stUseCase.eSecondVideoInput;
     stUseCase.eSecondVideoInput = eUseCaseInputTemp;
	 stUseCase.eUseCaseWindow = eTDAL_USE_CASE_WINDOW_DUAL_PIP;

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	/* warning, inverse video layer */



    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoder,
														LayerStreamHandleSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet VID_0 succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoderSub,
														LayerStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet VID_1 succeed");


    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoderSub,
											   &stVideoStartParamsSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start sub succeed");

	/*-------------------------*/
	/* result                  */
	/*-------------------------*/

    TestManager_AskForChecking( "do you see 2 video and audio playing ?" );



	/*-------------------------*/
	/* terminate               */
	/*-------------------------*/



    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

	if(stOutputCap.nbAudioAna > 1)
	{
		/* we suppose that when more than one audio ana, the second is a headphone */
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio headphone disable" );
	}

    /* close all modules*/
#if 0
    TestManager_AssertEqual(TDAL_VBI_SLICER_DisableInputSlicing(AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_DisableInputSlicing succeed");
    TestHelper_UnRegisterClient_VBI_SLICER(&stClientId,
										   &stVbiSLicerDecoderCapability);
    TestManager_AssertEqual(TDAL_VBI_SLICER_Term(),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_Term succeed");
#endif

    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoderSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");
    TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Stop succeed");
    TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);

    for(eIndexEvent=eTDAL_ANA_VID_EVENT_TYPE_CHANGE; eIndexEvent<=eTDAL_ANA_VID_EVENT_NEED_DISPLAY; eIndexEvent++)
    {
        TestManager_AssertEqual( TDAL_ANA_VID_EventUnsubscribe(IndexVideoDecoderSub, eIndexEvent ), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_EventUnsubscribe succeed" );
    }

    TestHelper_Terminate_ANA_VID(IndexVideoDecoder);


    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_1),
							eTDAL_DISP_NO_ERROR,
							"2nd video disp layer disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st video disp layer disable" );
    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio disable" );
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
    TestManager_AssertEqual(TDAL_SCART_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_SCART_INPUT_Term succeed");

    TestManager_AssertEqual(TDAL_ANA_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_Term succeed");

}

void TestSce_TunerAnaInputVideoAudioAnalogPIP( void )
{
    unsigned char                           ucFeID;
	tTDAL_DMD_NotifyStatus                  DMD_NotifyCond;
    tTDAL_DMD_FECapability                  stFECapability;
    uint32_t                                DemodStreamHandle;
    uint32_t                                AnaInputStreamHandle, LayerStreamHandle, LayerStreamHandleSub;
    tTDAL_ANA_VID_Decoder                   IndexVideoDecoder,IndexVideoDecoderSub;
    tTDAL_ANA_AUD_Decoder                   IndexAudioDecoder;
    tTDAL_ANA_AUD_DecoderCapability         stAudioDecoderCapability;
    tTDAL_ANA_VID_DecoderCapability         stVideoDecoderCapability,stVideoDecoderCapabilitySub;
    tTDAL_ANA_AUD_StartParams               stAudioStartParams;
    tTDAL_ANA_VID_StartParams               stVideoStartParams,stVideoStartParamsSub;
    tTDAL_OUTPUT_VideoDigitalParams         stVideoDigitalParams;
	tTDAL_OUTPUT_VideoHDDACParams           stHDDACParams;
	tTDAL_OUTPUT_Capability                 stOutputCap;
    tTDAL_OUTPUT_AudioAnaCapability         stAudioCapability;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
#if 0
    tTDAL_VBI_SLICER_Capability             stVbiSLicerDecoderCapability;
    tTDAL_VBI_SLICER_ClientId               stClientId;
#endif
	tTDAL_ANA_VID_Coding                    eVidCodingSub;
	tTDAL_ANA_INPUT_Object                  VidMuxObjSub;
	tTDAL_ANA_INPUT_Object                  VidObjSub;
	tTDAL_ANA_INPUT_Object                  AudMuxObj;
    tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_ANA_VID_Event             		eIndexEvent;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;


    /*Get tuner capability*/
    TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_ATV);
    TestManager_AssertEqual(TDAL_DMD_GetFECapability( ucFeID, &stFECapability),
							eTDAL_DMD_NO_ERROR,
							"TDAL_DMD_GetFECapability succeed");
    if(stFECapability.eSourceSupport == eTDAL_DMD_DIGITAL)
    {
        TestManager_LogMessage("Tuner doesn't support the anolog\n");
        TestHelper_DMD_Term(&ucFeID);
        return;
    }

	mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
	mTBOX_PRINT((kTBOX_LF," selection of the secondary video source       \n"));
	mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


	TestHelper_Initialize_ANA_INPUT( &VidObjSub,&VidMuxObjSub,NULL,&eVidCodingSub );


	mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
	mTBOX_PRINT((kTBOX_LF," selection of the secondary video source end   \n"));
	mTBOX_PRINT((kTBOX_LF,"===============================================\n"));

		/* Ana Input initialisation */
	TestHelper_Initialize_SCART_INPUT ();

    /* Initialise modules */
    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_SPEAKERS_Init(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers init" );

    TestManager_AssertEqual(TDAL_PANEL_Init(),
							eTDAL_PANEL_NO_ERROR,
							"panel init" );


	TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);

    /* Initialisation for Analog Video*/
	TestManager_AssertEqual(TDAL_DMD_HandleGet(ucFeID, &DemodStreamHandle),
							eTDAL_DMD_NO_ERROR,
							"TDAL_DMD_HandleGet succeed");

	/* Initialisation for Analog Video*/
    TestHelper_Initialize_ANA_VID( &IndexVideoDecoder,
								   &stVideoDecoderCapability,
								   eTDAL_ANA_VID_CODING_CVBS  );
    TestHelper_Initialize_ANA_VID_SUB ( &IndexVideoDecoderSub, &stVideoDecoderCapabilitySub,eVidCodingSub  );
	
	if(eVidCodingSub == eTDAL_ANA_VID_CODING_YPRPB)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_TUNER;
    	stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_YPrPb;
	    stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	}
	else if((eVidCodingSub == eTDAL_ANA_VID_CODING_CVBS ||
			 eVidCodingSub == eTDAL_ANA_VID_CODING_YC))
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_SCART;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	}
	else  if(eVidCodingSub == eTDAL_ANA_VID_CODING_RGB_HV)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eSecondVideoInput = eTDAL_USE_CASE_PC;
    	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;		
	}

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(VidObjSub,VidMuxObjSub),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );


    /* Set the way for the video */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(VidMuxObjSub, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet video");

	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder,
														DemodStreamHandle ),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_InputStreamSet DEMOD succeed");

	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoderSub,
														AnaInputStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_InputStreamSet ANA INPUT succeed");


    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
													 (void*)&(LayerStreamHandle)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet VID_0 succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_1,
													 (void*)&(LayerStreamHandleSub)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet VID_1 succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsGet VID_0 succeed");

	stOutputWindow.Top = 0;
	stOutputWindow.Bottom = 1080;
	stOutputWindow.Left= 0;
	stOutputWindow.Right = 1920;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsSet VID_0 succeed");



    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsGet VID_1 succeed");

	stOutputWindow.Top = 0;
	stOutputWindow.Bottom = 576;
	stOutputWindow.Left= 0;
	stOutputWindow.Right = 720;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsSet VID_1 succeed");


    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoder,
														LayerStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet VID_0 succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoderSub,
														LayerStreamHandleSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet VID_1 succeed");


    /* Initialisation for Analog Audio*/
    TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

	TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder,
														DemodStreamHandle),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_InputStreamSet DEMOD succeed");	

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
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_DestinationSet FPD succeed");	

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

    /*Initialisation for vbi slicer */
#if 0
    TestHelper_Initialize_VBI_SLICER( &stVbiSLicerDecoderCapability );

    TestHelper_RegisterClient_VBI_SLICER(&stClientId, &stVbiSLicerDecoderCapability);


    /*configure the input*/
    TestManager_AssertEqual(TDAL_VBI_SLICER_ConfigureClientInput(stClientId,
																 AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_ConfigureClientInput succeed");
#endif
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
											   &stAudioStartParams),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Start succeed");

	stVideoStartParams.uiCodingMask  = eTDAL_ANA_VID_CODING_CVBS;
	stVideoStartParams.uiStdMask     = eTDAL_ANA_VID_STD_SECAM;

	stVideoStartParamsSub.uiCodingMask  = eVidCodingSub;
	stVideoStartParamsSub.uiStdMask     = stVideoDecoderCapabilitySub.uiSupportedStdMask;/*eTDAL_ANA_VID_STD_SECAM;*/


    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoderSub,
											   &stVideoStartParamsSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start sub succeed");


    /*start the Vbi Slicer */
#if 0
    TestManager_AssertEqual(TDAL_VBI_SLICER_EnableInputSlicing(AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_EnableInputSlicing succeed");
#endif

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

	/*-------------------------*/
	/* result                  */
	/*-------------------------*/

    TestManager_AskForChecking( "do you see 2 video and audio playing ?" );


    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoderSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");


	/* swap sources */
	 stUseCase.eFirstVideoInput = stUseCase.eSecondVideoInput;
     stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_TUNER;
	 stUseCase.eUseCaseWindow = eTDAL_USE_CASE_WINDOW_DUAL_PIP;

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	/* warning, inverse video layer */



    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoder,
														LayerStreamHandleSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet VID_0 succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet(IndexVideoDecoderSub,
														LayerStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_DestinationSet VID_1 succeed");


    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoderSub,
											   &stVideoStartParamsSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start sub succeed");

	/*-------------------------*/
	/* result                  */
	/*-------------------------*/

    TestManager_AskForChecking( "do you see 2 video and audio playing ?" );



	/*-------------------------*/
	/* terminate               */
	/*-------------------------*/



    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

	if(stOutputCap.nbAudioAna > 1)
	{
		/* we suppose that when more than one audio ana, the second is a headphone */
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio headphone disable" );
	}

    /* close all modules*/
#if 0
    TestManager_AssertEqual(TDAL_VBI_SLICER_DisableInputSlicing(AnaInputStreamHandle),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_DisableInputSlicing succeed");
    TestHelper_UnRegisterClient_VBI_SLICER(&stClientId,
										   &stVbiSLicerDecoderCapability);
    TestManager_AssertEqual(TDAL_VBI_SLICER_Term(),
							eTDAL_VBI_SLICER_NO_ERROR,
							"TDAL_VBI_SLICER_Term succeed");
#endif

    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoderSub),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");
    TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Stop succeed");
    TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);

    for(eIndexEvent=eTDAL_ANA_VID_EVENT_TYPE_CHANGE; eIndexEvent<=eTDAL_ANA_VID_EVENT_NEED_DISPLAY; eIndexEvent++)
    {
        TestManager_AssertEqual( TDAL_ANA_VID_EventUnsubscribe(IndexVideoDecoderSub, eIndexEvent ), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_EventUnsubscribe succeed" );
    }

    TestHelper_Terminate_ANA_VID(IndexVideoDecoder);


    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_1),
							eTDAL_DISP_NO_ERROR,
							"2nd video disp layer disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st video disp layer disable" );
    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio disable" );
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
    TestManager_AssertEqual(TDAL_SCART_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_SCART_INPUT_Term succeed");

    TestManager_AssertEqual(TDAL_ANA_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_Term succeed");

	bUseAnalogChannel = FALSE;
}

char *ANA_VID_EventToString(tTDAL_ANA_VID_Event Event)
{
    switch(Event)
    {
	case eTDAL_ANA_VID_EVENT_TYPE_CHANGE:
        return("TYPE_CHANGE");
        break;
	case eTDAL_ANA_VID_EVENT_TIMING_CHANGE:
        return("TIMING_CHANGE");
        break;
	case eTDAL_ANA_VID_EVENT_SIGNAL_LOST:
        return("SIGNAL_LOST");
        break;
	case eTDAL_ANA_VID_EVENT_NEED_DISPLAY:
        return("NEED_DISPLAY");
        break;
    default:
        return("UNKNOWN");
        break;
    }
}

void ANA_VID_CallbackProc( tTDAL_ANA_VID_Decoder eDecoder, tTDAL_ANA_VID_Event   eEvent, const void           *pEventData )
{
    mTBOX_TRACE((kTBOX_NIV_1, "ANA_VID_CallbackProc() Decoder[0x%x] Event[%s]\r\n",
				 eDecoder,
				 ANA_VID_EventToString(eEvent)));

	printf("ANA_VID_CallbackProc called \n");

	if(eEvent == eTDAL_ANA_VID_EVENT_NEED_DISPLAY)
	{
	    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st disp layer video enable" );
	}
	
}

void ANA_VID_CallbackProcSub( tTDAL_ANA_VID_Decoder eDecoder, tTDAL_ANA_VID_Event   eEvent, const void           *pEventData )
{
    mTBOX_TRACE((kTBOX_NIV_1, "ANA_VID_CallbackProcSub() Decoder[0x%x] Event[%s]\r\n",
				 eDecoder,
				 ANA_VID_EventToString(eEvent)));

	if(eEvent == eTDAL_ANA_VID_EVENT_NEED_DISPLAY)
	{
printf("ANA_VID_CallbackProcSub called \n");
	    TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_1),
							eTDAL_DISP_NO_ERROR,
							"2nd disp layer video enable" );
	}
	
}

char *ANA_AUD_EventToString(tTDAL_ANA_AUD_Event Event)
{
    switch(Event)
    {
	case eTDAL_ANA_AUD_EVENT_SIGNAL_CHANGED:
        return("SIGNAL_CHANGED");
        break;
	case eTDAL_ANA_AUD_EVENT_SIGNAL_LOST:
        return("SIGNAL_LOST");
        break;
	case eTDAL_ANA_AUD_EVENT_SIGNAL_SWITCH:
        return("SIGNAL_SWITCH");
        break;
    default:
        return("UNKNOWN");
        break;
    }
}

char *ANA_AUD_SoundStdToString(tTDAL_ANA_AUD_SoundStd SoundStd)
{
    switch(SoundStd)
    {
	case eTDAL_ANA_AUD_SOUND_STD_UNKNOWN:
        return("UNKOWN");
	case eTDAL_ANA_AUD_SOUND_STD_AM_MONO:
        return("AM_MONO");
	case eTDAL_ANA_AUD_SOUND_STD_FM_MONO:
        return("FM_MONO");
	case eTDAL_ANA_AUD_SOUND_STD_2CS:
        return("2CS");
	case eTDAL_ANA_AUD_SOUND_STD_AM_NICAM:
        return("AM_NICAM");
	case eTDAL_ANA_AUD_SOUND_STD_FM_NICAM:
        return("FM_NICAM");
    default:
        return("??");
    }
}

char *ANA_AUD_TrackToString(tTDAL_ANA_AUD_Track eTrack)
{
    switch(eTrack)
    {
	case eTDAL_ANA_AUD_TRACK_MONO:
        return("MONO");
	case eTDAL_ANA_AUD_TRACK_STEREO:
        return("STEREO");
	case eTDAL_ANA_AUD_TRACK_LANG_A:
        return("LANG_A");
	case eTDAL_ANA_AUD_TRACK_LANG_B:
        return("LANG_B");
	case eTDAL_ANA_AUD_TRACK_LANG_A_PLUS_B:
        return("LANG_A_PLUS_B");
    default:
        return("??");
    }
}

void ANA_AUD_CallbackProc
(
	tTDAL_ANA_AUD_Decoder eDecoder,
	tTDAL_ANA_AUD_Event   eEvent,
	const void           *pEventData
)
{
	tTDAL_ANA_AUD_Type *pstType = NULL;
	tTDAL_ANA_AUD_Track eTrack = eTDAL_ANA_AUD_SOUND_PROP_STEREO; /* by default */

    mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Event[%s]\r\n",
				 eDecoder,
				 ANA_AUD_EventToString(eEvent)));

	switch(eEvent)
	{
		case eTDAL_ANA_AUD_EVENT_SIGNAL_CHANGED:
		pstType = (tTDAL_ANA_AUD_Type *)pEventData;
		TestManager_AssertTrue((pstType != NULL), "Audio signal changed event data pointer");
		if(pstType != NULL)
		{
			mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Std[%s] Props[0x%x]\r\n",
						 eDecoder,
						 ANA_AUD_SoundStdToString(pstType->eStd),
						 pstType->uiPropMask));
			if(pstType->uiPropMask & eTDAL_ANA_AUD_SOUND_PROP_MONO)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Prop MONO\r\n",
							 eDecoder));
				eTrack = eTDAL_ANA_AUD_TRACK_MONO;
			}
			if(pstType->uiPropMask & eTDAL_ANA_AUD_SOUND_PROP_STEREO)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Prop STEREO\r\n",
							 eDecoder));
				eTrack = eTDAL_ANA_AUD_TRACK_STEREO;
			}
			if(pstType->uiPropMask & eTDAL_ANA_AUD_SOUND_PROP_DUAL)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Prop DUAL\r\n",
							 eDecoder));
				eTrack = eTDAL_ANA_AUD_TRACK_LANG_A_PLUS_B;
			}
			if(pstType->uiPropMask & eTDAL_ANA_AUD_SOUND_PROP_UNRELATED)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Prop UNRELATED\r\n",
							 eDecoder));
			}
			if(pstType->uiPropMask & eTDAL_ANA_AUD_SOUND_PROP_DATA)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "ANA_AUD_CallbackProc() Decoder[0x%x] Prop DATA\r\n",
							 eDecoder));
			}
				/*if(!AudioTrackSet)
				{
					TestManager_AssertEqual(TDAL_ANA_AUD_TrackSet(eDecoder,eTrack),
										eTDAL_ANA_AUD_NO_ERROR,
										"TDAL_ANA_AUD_TrackSet succeed");
					AudioTrackSet = true;

				}*/

		}
		break;
		case eTDAL_ANA_AUD_EVENT_SIGNAL_LOST:
			break;
		case eTDAL_ANA_AUD_EVENT_SIGNAL_SWITCH:
			break;
	}
}

void VBI_SLICER_CallbackVPS( tTDAL_VBI_SLICER_ClientId  stClientId,
                             uint32_t                   ulInputHandle,
                             uint32_t                   ulVpsDataMsb,
                             uint32_t                   ulVpsDataLsb)
{
    mTBOX_TRACE((kTBOX_NIV_1, "VBI_SLICER_CallbackVPS() stClientId[0x%x] ulInputHandle[0x%x]\r\n", stClientId, ulInputHandle));
printf("==> VBI_SLICER_CallbackVPS \n");

    mTBOX_TRACE((kTBOX_NIV_1, "VBI_SLICER_CallbackVPS() ulVpsDataMsb[0x%x] ulVpsDataLsb[0x%x]\r\n", ulVpsDataMsb, ulVpsDataLsb));
}

void VBI_SLICER_CallbackWST( tTDAL_VBI_SLICER_ClientId  stClientId,
                             uint32_t                   ulInputHandle,
                             uint16_t                   usSizeOfData,
                             const void                 *pData)
{
#if 0
    uint16_t i;
#endif
    uint16_t* pReceiveData=NULL;

    mTBOX_TRACE((kTBOX_NIV_1, "VBI_SLICER_CallbackWST() stClientId[0x%x] ulInputHandle[0x%x]\r\n", stClientId ,	ulInputHandle));

    if(pData != NULL)
    {
        pReceiveData = (uint16_t*)pData;
#if 0
        mTBOX_TRACE((kTBOX_NIV_1, "Data : "));
        for(i=0;i<usSizeOfData;i++)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "[0x%x]", &pReceiveData[i]));
        }
        mTBOX_TRACE((kTBOX_NIV_1, "\r\n"));
#endif		
        TDAL_VBI_SLICER_FreeWstPacket((void*)pData);
    }
}


uint32_t ulNbWssPacketReceived = 0;


void VBI_SLICER_CallbackWSS( tTDAL_VBI_SLICER_ClientId  stClientId,
                             uint32_t                   ulInputHandle,
                             uint16_t                   usWssData)
{
    if(ulNbWssPacketReceived <= 10)
    {
        ulNbWssPacketReceived++;
        mTBOX_TRACE((kTBOX_NIV_1, "VBI_SLICER_CallbackWSS() (%d/10 then no output) stClientId[0x%x] ulInputHandle[0x%x]\r\n", ulNbWssPacketReceived, stClientId ,	ulInputHandle));
    mTBOX_TRACE((kTBOX_NIV_1, "VBI_SLICER_CallbackWSS() usWssData[0x%x] \r\n", usWssData));
}
}

void TestStartStopAnalogModule( unsigned char ucFeID, bool Start)
{
    static uint32_t                         DemodStreamHandle, LayerStreamHandle;
    static tTDAL_ANA_VID_Decoder            IndexVideoDecoder;
    static tTDAL_ANA_AUD_Decoder            IndexAudioDecoder;
    tTDAL_ANA_AUD_DecoderCapability         stAudioDecoderCapability;
    tTDAL_ANA_VID_DecoderCapability         stVideoDecoderCapability;
    tTDAL_ANA_AUD_StartParams               stAudioStartParams;
    tTDAL_ANA_VID_StartParams               stVideoStartParams;
    static tTDAL_VBI_SLICER_Capability      stDecoderCapability;
    static tTDAL_VBI_SLICER_ClientId        stClientId;

    if(Start)
    {
        /* Initialise modules */
        TestManager_AssertEqual(TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "disp init" );
        TestManager_AssertEqual(TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init" );

        TestManager_AssertEqual(TDAL_DMD_HandleGet(ucFeID, &DemodStreamHandle), eTDAL_DMD_NO_ERROR, "TDAL_DMD_HandleGet succeed");

        /* Initialisation for Analog Video*/
/* @@@PPO */
        TestHelper_Initialize_ANA_VID( &IndexVideoDecoder, &stVideoDecoderCapability, eTDAL_ANA_VID_CODING_CVBS );

        /* Set the way for the video */
        TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder, DemodStreamHandle ), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_InputStreamSet succeed");

        TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0, (void*)&(LayerStreamHandle)), eTDAL_DISP_NO_ERROR, "TDAL_DISP_LayerHandleGet succeed");

        TestManager_AssertEqual(TDAL_ANA_VID_DestinationSet( IndexVideoDecoder, LayerStreamHandle), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_DestinationSet succeed");

        /* Initialisation for Analog Audio*/
        TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

        /* Set the way for the audio */
        TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder, DemodStreamHandle ), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_InputStreamSet succeed");

        /*Initialisation for vbi slicer */
        TestHelper_Initialize_VBI_SLICER( &stDecoderCapability );
        TestHelper_RegisterClient_VBI_SLICER(&stClientId, &stDecoderCapability);
        /*configure the input*/
        TestManager_AssertEqual(TDAL_VBI_SLICER_ConfigureClientInput(stClientId, DemodStreamHandle), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_ConfigureClientInput succeed");

        /*start audio and video Analog */
        stAudioStartParams.uiStdMask = stAudioDecoderCapability.uiSupportedStdMask;
        TestManager_AssertEqual(TDAL_ANA_AUD_Start(IndexAudioDecoder, &stAudioStartParams ), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_Start succeed");

        stVideoStartParams.uiCodingMask  = stVideoDecoderCapability.uiSupportedCodingMask;
        stVideoStartParams.uiStdMask     = stVideoDecoderCapability.uiSupportedStdMask;
        TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder, &stVideoStartParams ), eTDAL_ANA_VID_NO_ERROR, "IndexVideoDecoder succeed");

        /*start the Vbi Slicer */
        ulNbWssPacketReceived = 0;
        TestManager_AssertEqual(TDAL_VBI_SLICER_EnableInputSlicing(stClientId,DemodStreamHandle), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_EnableInputSlicing succeed");
    }
    else
    {
        TestManager_AssertEqual(TDAL_VBI_SLICER_DisableInputSlicing(stClientId,DemodStreamHandle), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_DisableInputSlicing succeed");
        TestHelper_UnRegisterClient_VBI_SLICER(&stClientId, &stDecoderCapability);
        TestManager_AssertEqual(TDAL_VBI_SLICER_Term(), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_Term succeed");

        TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_Stop succeed");
        TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexVideoDecoder), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_Stop succeed");
        TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);
        TestHelper_Terminate_ANA_VID(IndexVideoDecoder);

        TestManager_AssertEqual(TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "disp terminate" );
        TestManager_AssertEqual(TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output terminate" );
    }
}

void SCART_INPUT_CallbackFct
(
    tTDAL_SCART_INPUT_Id     eId,
    tTDAL_SCART_INPUT_Event  eEvent,
    const void              *pEventData
)
{
	tTDAL_SCART_INPUT_SlowBlankingSignal eSLBSignal;
	tTDAL_SCART_INPUT_FastBlankingSignal eFBSignal;

	switch (eEvent)
	{
		case eTDAL_SCART_INPUT_EVENT_SLOW_BLANKING_CHANGE :
            mTBOX_TRACE((kTBOX_NIV_1, "SCART_INPUT_CallbackFct() eID[%d] SLB change \r\n", eId ));
			TDAL_SCART_INPUT_GetSlowBlanking(eId, &eSLBSignal);
			mTBOX_TRACE((kTBOX_NIV_1, "SCART_INPUT_CallbackFct() eID[%d] SLB change new value %d  \r\n", eId, eSLBSignal));				
			break;
		case eTDAL_SCART_INPUT_EVENT_FAST_BLANKING_CHANGE :			
            mTBOX_TRACE((kTBOX_NIV_1, "SCART_INPUT_CallbackFct() eID[%d] FB change \r\n", eId ));
			TDAL_SCART_INPUT_GetFastBlanking(eId, &eFBSignal);
			mTBOX_TRACE((kTBOX_NIV_1, "SCART_INPUT_CallbackFct() eID[%d] SLB change new value %d  \r\n", eId, eFBSignal));				
			break;
		default:
			break;
	}

}


void TestSce_HDMIInputVideoAudioPlay( void )
{
    tTDAL_OUTPUT_Capability                 stOutputCap;
    tTDAL_OUTPUT_AudioAnaCapability         stAudioCapability;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;
	tTDAL_HDMI_IN_SinkId 					eId;
	uint32_t 							    LayerHandle;



    /* Initialise modules */
    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_SPEAKERS_Init(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers init" );
    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"audio/Video init" );



	TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow);

	stOutputWindow.Bottom = 1080;
	stOutputWindow.Top = 0;
	stOutputWindow.Left = 0;
	stOutputWindow.Right = 1920;

	TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow);



	/* Ana Input initialisation */
    TestHelper_Initialize_HDMI_IN_INPUT (&eId);
    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_NEED_DISPLAY,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );


    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_STATE_CHANGE,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_INFOFRAME_AVI,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );


    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HMDI_IN_EVENT_INFOFRAME_AUDIO,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_INFOFRAME_SPD,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_INFOFRAME_VS,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_GAMUT_METADATA,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );


    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_GENERAL_CONTROL,
                             TestSce_HDMIInput_Callback),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );


	TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0, (void *)&LayerHandle);
	TDAL_HDMI_IN_VideoDestinationSet(eId,LayerHandle);	


    stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_HDMI;
	stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_HDMI;
    stUseCase.eSecondVideoInput = eTDAL_USE_CASE_NONE;
	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_SINGLE;

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TestManager_AssertEqual( TDAL_HDMI_IN_Start(eId),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_Start succeed" );

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
		
		TestManager_AssertEqual(TDAL_AV_AudioDestinationSet(eTDAL_AV_DECODER_AUDIO_1,
															SpeakerHandle),
								eTDAL_AV_NO_ERROR,
								"TDAL_AV_AudioDestinationSet FPD succeed");	
	}

	if(stSpeakersCap.nbSpeakers > 1)
	{
		/* 2nd speaker Id is HP */
		TestManager_AssertEqual(TDAL_SPEAKERS_HandleGet(eTDAL_SPEAKERS_HP_ID_0,
														&SpeakerHandle),
								eTDAL_SPEAKERS_NO_ERROR,
								"HP speaker handle get");

		TestManager_AssertEqual(TDAL_AV_AudioDestinationSet(eTDAL_AV_DECODER_AUDIO_1,
															SpeakerHandle),
								eTDAL_AV_NO_ERROR,
								"TDAL_AV_AudioDestinationSet HP succeed");		
	}


    memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");



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
							   "output audio SPDIF enable" );
	}

    TestManager_AskForChecking( "do you see video and audio playing ?" );
    TestManager_AskForChecking( "do you see some HDMI IN callback calls?" );


    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

	if(stOutputCap.nbAudioAna > 1)
	{
		/* we suppose that when more than one audio ana, the second is a headphone */
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio headphone disable" );
	}
	
	if(stOutputCap.nbAudioSPDIF > 0)
	{
	   TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_SPDIF_ID_0),
							   eTDAL_OUTPUT_NO_ERROR,
							   "output audio SPDIF disable" );
	}

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"disp layer disable" );

	TestManager_AssertEqual(TDAL_SPEAKERS_Term(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers terminate" );
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"Audio/Video terminate" );
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );
    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );

	
	TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_NEED_DISPLAY),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );
    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_STATE_CHANGE),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_INFOFRAME_AVI),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );


    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HMDI_IN_EVENT_INFOFRAME_AUDIO),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_INFOFRAME_SPD),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_INFOFRAME_VS),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_GAMUT_METADATA),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );

    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_GENERAL_CONTROL),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );

	TestManager_AssertEqual( TDAL_HDMI_IN_Stop(eId),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_Stop succeed" );
    TestManager_AssertEqual(TDAL_HDMI_IN_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_HDMI_IN_Term succeed");
}

void TestSce_HDMIInput_Callback (tTDAL_HDMI_IN_SinkId     eId,
                                                 tTDAL_HDMI_IN_Event      eEvent,
                                                 const void               *pEventData)
{

	tTDAL_HDMI_IN_SourceStatus 		*pstState;
	tTDAL_HDMI_InfoFrameAVI 		stAVIInfo;
	tTDAL_HDMI_IN_InfoFrameAudio 	stAudioInfo;
	tTDAL_HDMI_InfoFrameSPD  		stSPD;
	tTDAL_HDMI_InfoFrameVS   		stVS;
	tTDAL_HDMI_IN_GamutBoundaryDesc stDesc;
	tTDAL_HDMI_IN_GeneralControl 	stControl;
	tTDAL_AV_StreamType             StreamType;
/*mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF,"TestSce_HDMIInput_Callback called for sinkid %d\n",eId));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));*/

	switch(eEvent)
	{
	case eTDAL_HDMI_IN_EVENT_NEED_DISPLAY:
		mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_NEED_DISPLAY \r\n", eId ));
		TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
		/* GS : test purpose of tdal_av audio */
        StreamType.audioType = eTDAL_AV_AUDIO_TYPE_HE_AAC;
        TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1,
											  StreamType),
								eTDAL_AV_NO_ERROR,
								"1st av start audio");

		break;

	case eTDAL_HDMI_IN_EVENT_STATE_CHANGE:
		mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_STATE_CHANGE \r\n", eId ));
		if(pEventData != NULL)
		{
			pstState = ((tTDAL_HDMI_IN_SourceStatus *)(pEventData));
    	   mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback() eTDAL_HDMI_IN_EVENT_STATE_CHANGE state[%s] source[%s]\r\n",
						((pstState->eState == eTDAL_HDMI_IN_STATE_NO_SOURCE) ? "NO_SRC":\
			             (pstState->eState == eTDAL_HDMI_IN_STATE_SOURCE_CONNECTED) ? "SRC_CONNECTED":\
						 (pstState->eState == eTDAL_HDMI_IN_STATE_SOURCE_NOT_HDCP) ? "SRC_NO_HDCP":\
						 (pstState->eState == eTDAL_HDMI_IN_STATE_AUTHENTICATION_IN_PROGRESS) ? "AUTH_IN_PROG":\
						 (pstState->eState == eTDAL_HDMI_IN_STATE_AUTHENTICATION_SUCCEED) ? "AUTH_SUCCEED":\
						 (pstState->eState == eTDAL_HDMI_IN_STATE_AUTHENTICATION_FAILED) ? "AUTH_FAILED":\
						 (pstState->eState == eTDAL_HDMI_IN_STATE_NO_ENCRYPTION) ? "NO_ENCRYPT": "??"),
						((pstState->eSourceDeviceType == eTDAL_HDMI_IN_SOURCE_TYPE_HDMI) ? "HDMI":\
						 (pstState->eSourceDeviceType == eTDAL_HDMI_IN_SOURCE_TYPE_DVI) ? "DVI": "??")));
 		}
		break;

	case eTDAL_HDMI_IN_EVENT_INFOFRAME_AVI:
    	mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_INFOFRAME_AVI \r\n", eId ));
		/* check the fonctions to get data doesn't crash */
		TDAL_HDMI_IN_InfoFrameAVIGet(eId, &stAVIInfo);
		break;

    case eTDAL_HMDI_IN_EVENT_INFOFRAME_AUDIO:
		mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HMDI_IN_EVENT_INFOFRAME_AUDIO \r\n", eId ));
		TDAL_HDMI_IN_InfoFrameAudioGet(eId, &stAudioInfo);
		break;

	case eTDAL_HDMI_IN_EVENT_INFOFRAME_SPD:
    	mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_INFOFRAME_SPD \r\n", eId ));
		TDAL_HDMI_IN_InfoFrameSPDGet(eId, &stSPD);
		break;
	case eTDAL_HDMI_IN_EVENT_INFOFRAME_VS:
		mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_INFOFRAME_VS \r\n", eId ));
		TDAL_HDMI_IN_InfoFrameVSGet(eId, &stVS);
		break;
	
	case eTDAL_HDMI_IN_EVENT_GAMUT_METADATA:
    	mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_GAMUT_METADATA \r\n", eId ));
		TDAL_HDMI_IN_GamutMetadataGet(eId,&stDesc);
		break;
	
	case eTDAL_HDMI_IN_EVENT_GENERAL_CONTROL:
    	mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] eTDAL_HDMI_IN_EVENT_GENERAL_CONTROL \r\n", eId ));
		TDAL_HDMI_IN_GeneralControlGet(eId,&stControl);
		break;
	
	default:
		mTBOX_TRACE((kTBOX_NIV_2, "TestSce_HDMIInput_Callback()"
					 "eID[%d] event[%d]\r\n", eId, eEvent ));
		break;			
	}

/*mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF,"TestSce_HDMIInput_Callback called end          \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));*/


	return;
}

void TestSce_HDMIInput_CallbackSub (tTDAL_HDMI_IN_SinkId     eId,
                                            tTDAL_HDMI_IN_Event      eEvent,
                                            const void               *pEventData)
{
	tTDAL_HDMI_IN_SourceStatus 			*pstStatus = NULL;

	switch(eEvent)
	{
		case eTDAL_HDMI_IN_EVENT_NEED_DISPLAY:
		TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_1);
		break;			
		case eTDAL_HDMI_IN_EVENT_STATE_CHANGE:
    	mTBOX_TRACE((kTBOX_NIV_1, "TestSce_HDMIInput_Callback() eID[%d] eTDAL_HDMI_IN_EVENT_STATE_CHANGE \r\n", eId ));
		if(pEventData != NULL)
		{
			pstStatus = ((tTDAL_HDMI_IN_SourceStatus *)(pEventData));
    	   mTBOX_TRACE((kTBOX_NIV_1, "TestSce_HDMIInput_Callback() eTDAL_HDMI_IN_EVENT_STATE_CHANGE state %d source type %d \r\n",pstStatus->eState,pstStatus->eSourceDeviceType));
 		}
		break;
		default:
		break;
	}	

}

void TestSce_1AnaInput1HDMIVideoAudioAnalogPIP(void)
{
    tTDAL_ANA_AUD_DecoderCapability         stAudioDecoderCapability;
    tTDAL_ANA_VID_DecoderCapability         stVideoDecoderCapability;
    tTDAL_OUTPUT_Capability                 stOutputCap;
    tTDAL_OUTPUT_AudioAnaCapability         stAudioCapability;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;
	tTDAL_HDMI_IN_SinkId 					eId;
    uint32_t                                AnaInputStreamHandle, LayerStreamHandle,SubLayerStreamHandle;
    tTDAL_ANA_VID_Decoder                   IndexVideoDecoder;
    tTDAL_ANA_AUD_Decoder                   IndexAudioDecoder;
    tTDAL_ANA_AUD_StartParams               stAudioStartParams;
    tTDAL_ANA_VID_StartParams               stVideoStartParams;
	tTDAL_ANA_VID_Coding                    eVidCoding;
	tTDAL_ANA_INPUT_Object                  VidMuxObj;
	tTDAL_ANA_INPUT_Object                  VidObj;
	tTDAL_ANA_INPUT_Object                  AudMuxObj;

    /* Initialise modules */
    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_SPEAKERS_Init(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers init" );

    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_DISP_NO_ERROR,
							"av init" );

    TestManager_AssertEqual(TDAL_DMX_Init(),
							eTDAL_DISP_NO_ERROR,
							"av init" );



	/* Ana Input initialisation */

mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the main HDMI video source       \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


    TestHelper_Initialize_ANA_INPUT ( &VidObj,&VidMuxObj, &AudMuxObj, &eVidCoding );
    /* Initialisation for Analog Video*/
    TestHelper_Initialize_ANA_VID ( &IndexVideoDecoder, &stVideoDecoderCapability,eVidCoding  );

	TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);



	TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow);

	stOutputWindow.Bottom = 1080;
	stOutputWindow.Top = 0;
	stOutputWindow.Left = 0;
	stOutputWindow.Right = 1920;

	TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow);

mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the main HDMI video source end   \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the secondary video source       \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));


    TestHelper_Initialize_HDMI_IN_INPUT (&eId);


mTBOX_PRINT((kTBOX_LF,"===============================================\n"));
mTBOX_PRINT((kTBOX_LF," selection of the secondary video source end     \n"));
mTBOX_PRINT((kTBOX_LF,"===============================================\n"));




    TestManager_AssertEqual( TDAL_HDMI_IN_EventSubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_NEED_DISPLAY,
                             TestSce_HDMIInput_CallbackSub),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventSubscribe succeed" );


	TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_1, &LayerStreamHandle);
	TDAL_HDMI_IN_VideoDestinationSet(eId,LayerStreamHandle);	

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsGet succeed");

	stOutputWindow.Top = 0;
	stOutputWindow.Bottom = 720;
	stOutputWindow.Left= 0;
	stOutputWindow.Right = 576;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsSet succeed");



	if(eVidCoding == eTDAL_ANA_VID_CODING_CVBS ||
	   eVidCoding == eTDAL_ANA_VID_CODING_YC)
	{
    	stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_SCART;
	}
	else  if(eVidCoding == eTDAL_ANA_VID_CODING_YPRPB)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_YPrPb;
		
	}
	else if(eVidCoding == eTDAL_ANA_VID_CODING_RGB_HV)
	{
		/* PC mode */
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_PC;
		
	}

    stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_HDMI;
	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(VidObj,VidMuxObj),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );

	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder,
														AnaInputStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_InputStreamSet succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
													 (void*)&(LayerStreamHandle)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_1,
													 (void*)&(SubLayerStreamHandle)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet succeed");


	TDAL_ANA_VID_DestinationSet(IndexVideoDecoder, LayerStreamHandle);

	TDAL_HDMI_IN_VideoDestinationSet(eId, SubLayerStreamHandle);


	TestManager_AssertEqual( TDAL_HDMI_IN_Start(eId),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_Start succeed" );

    /* Initialisation for Analog Audio*/
    TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

    /* Set the way for the audio */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(AudMuxObj, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet audio");

	TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder,
														AnaInputStreamHandle),
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
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_DestinationSet FPD succeed");	

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


    /*start audio and video Analog */
    stAudioStartParams.uiStdMask = stAudioDecoderCapability.uiSupportedStdMask;
    TestManager_AssertEqual(TDAL_ANA_AUD_Start(IndexAudioDecoder,
											   &stAudioStartParams),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Start succeed");

	stVideoStartParams.uiCodingMask  = eVidCoding;
	stVideoStartParams.uiStdMask     = stVideoDecoderCapability.uiSupportedStdMask;/*eTDAL_ANA_VID_STD_SECAM;*/

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");


	/*--------------------------*/
	/* wait for result          */
	/*--------------------------*/

    TestManager_AskForChecking( "do you see analog input video in main and hdmi video in pip ?" );


    /*-------------------------*/
	/* swap source             */
	/*-------------------------*/
    TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Stop succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");


	TestManager_AssertEqual( TDAL_HDMI_IN_Stop(eId),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_Start succeed" );


	 stUseCase.eSecondVideoInput = stUseCase.eFirstVideoInput;
     stUseCase.eFirstVideoInput = eTDAL_USE_CASE_ANA_HDMI;
	 stUseCase.eUseCaseWindow = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	 stUseCase.eAudioInput = eTDAL_USE_CASE_ANA_HDMI;

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TDAL_ANA_VID_DestinationSet(IndexVideoDecoder, SubLayerStreamHandle);

	TDAL_HDMI_IN_VideoDestinationSet(eId, LayerStreamHandle);

	TestManager_AssertEqual( TDAL_HDMI_IN_Start(eId),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_Start succeed" );

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");
	/*  wait for result */
    TestManager_AskForChecking( "do you see analog input video in main and hdmi video in pip ?" );



    /*-------------------------*/
	/* terminate test          */
	/*-------------------------*/



    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

	if(stOutputCap.nbAudioAna > 1)
	{
		/* we suppose that when more than one audio ana, the second is a headphone */
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio headphone disable" );
	}

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");

	
    TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);

    TestHelper_Terminate_ANA_VID(IndexVideoDecoder);


    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_1),
							eTDAL_DISP_NO_ERROR,
							"2nd video disp layer disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st video disp layer disable" );
	
    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio disable" );

	TestManager_AssertEqual( TDAL_HDMI_IN_Stop(eId),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_Start succeed" );


    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );


    TestManager_AssertEqual(TDAL_SPEAKERS_Term(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers terminate" );
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );


    TestManager_AssertEqual( TDAL_HDMI_IN_EventUnsubscribe(eId,
                             eTDAL_HDMI_IN_EVENT_NEED_DISPLAY),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_EventUnsubscribe succeed" );



    TestManager_AssertEqual(TDAL_HDMI_IN_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_HDMI_IN_Term succeed");

    TestManager_AssertEqual(TDAL_ANA_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_Term succeed");


}


extern int gChannel;
void TestSce_1TunerDigital1AnalogVideoAudioAnalogPIP(void)
{
    tTDAL_ANA_AUD_DecoderCapability         stAudioDecoderCapability;
    tTDAL_ANA_VID_DecoderCapability         stVideoDecoderCapability;
    tTDAL_OUTPUT_Capability                 stOutputCap;
    tTDAL_OUTPUT_AudioAnaCapability         stAudioCapability;
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_UseCaseInfo                       stUseCase;
	tTDAL_SPEAKERS_Capability               stSpeakersCap;
    uint32_t                                SpeakerHandle;
    tTDAL_DISP_LayerWindow     				stInputWindow;
	tTDAL_DISP_LayerWindow     				stOutputWindow;
	tTDAL_HDMI_IN_SinkId 					eId;
    uint32_t                                AnaInputStreamHandle, LayerStreamHandle,SubLayerStreamHandle;
    tTDAL_ANA_VID_Decoder                   IndexVideoDecoder;
    tTDAL_ANA_AUD_Decoder                   IndexAudioDecoder;
    tTDAL_ANA_AUD_StartParams               stAudioStartParams;
    tTDAL_ANA_VID_StartParams               stVideoStartParams;
	tTDAL_ANA_VID_Coding                    eVidCoding;
	tTDAL_ANA_INPUT_Object                  VidMuxObj;
	tTDAL_ANA_INPUT_Object                  VidObj;
	tTDAL_ANA_INPUT_Object                  AudMuxObj;
	struct TestManager_Channel             *pChannel;
    unsigned char                           ucFeID;
	tTDAL_DMD_ConfigData                    DMD_Config;
    tTDAL_AV_StreamType                     StreamType;


    /* Initialise modules */
    TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init" );
    TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init" );
    TestManager_AssertEqual(TDAL_SPEAKERS_Init(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers init" );

    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");

    TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_DISP_NO_ERROR,
							"av init" );

    TestManager_AssertEqual(TDAL_DMX_Init(),
							eTDAL_DISP_NO_ERROR,
							"av init" );

    TestHelper_Initialize_ANA_INPUT ( &VidObj,&VidMuxObj, &AudMuxObj, &eVidCoding );
    /* Initialisation for Analog Video*/
    TestHelper_Initialize_ANA_VID ( &IndexVideoDecoder, &stVideoDecoderCapability,eVidCoding  );

    TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(0),
                                      TEST_HELPER_TSROUTE_DEMUX(0));

	gChannel = 7;
	pChannel = TestHelper_GetChannel();
	/*      TDAL DMX      */
	TestHelper_Configure_TDAL_DMX(0); 
	/* Disable all the channel (Video, Audio, PCR) */
	TestHelper_ControlChannel_TDAL_DMX(FALSE);

    TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);

	DMD_Config.Ter.ConfigType            = eTDAL_DMD_CFG_TER_SOURCE_TRANSITION;
	DMD_Config.Ter.uCfg.SourceTransition = eTDAL_DMD_CFG_SOURCE_IS_DTV;
	TestManager_AssertEqual( TDAL_DMD_Config(ucFeID, &DMD_Config),
							 eTDAL_DMD_NO_ERROR,
							 "dmd config DTV SOURCE_TRANSITION" );



	TDAL_DISP_BlenderDimensionsSet(eTDAL_DISP_BLENDER_ID_0,1920,1080);



	TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow);

	stOutputWindow.Bottom = 1080;
	stOutputWindow.Top = 0;
	stOutputWindow.Left = 0;
	stOutputWindow.Right = 1920;

	TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,&stInputWindow,&stOutputWindow);

	TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_1, &LayerStreamHandle);


    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsGet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsGet succeed");

	stOutputWindow.Top = 0;
	stOutputWindow.Bottom = 720;
	stOutputWindow.Left= 0;
	stOutputWindow.Right = 576;

    TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_1,&stInputWindow,&stOutputWindow),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerIOWindowsSet succeed");


    stUseCase.eFirstVideoInput = eTDAL_USE_CASE_DIG_TUNER;
	stUseCase.eAudioInput = eTDAL_USE_CASE_DIG_TUNER;
	stUseCase.eSecondVideoInput = eTDAL_USE_CASE_ANA_YPrPb;
	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_DUAL_PIP;

	TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
	TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_1);

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");


	TestHelper_DMD_Tune(&ucFeID);



	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(VidObj,VidMuxObj),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );

	TestManager_AssertEqual(TDAL_ANA_VID_InputStreamSet(IndexVideoDecoder,
														AnaInputStreamHandle),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_InputStreamSet succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_0,
													 (void*)&(LayerStreamHandle)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet succeed");

    TestManager_AssertEqual(TDAL_DISP_LayerHandleGet(eTDAL_DISP_LAYER_VIDEO_ID_1,
													 (void*)&(SubLayerStreamHandle)),
							eTDAL_DISP_NO_ERROR,
							"TDAL_DISP_LayerHandleGet succeed");


	TDAL_ANA_VID_DestinationSet(IndexVideoDecoder, SubLayerStreamHandle);


    /* Initialisation for Analog Audio*/
    TestHelper_Initialize_ANA_AUD( &IndexAudioDecoder, &stAudioDecoderCapability  );

    /* Set the way for the audio */
    TestManager_AssertEqual(TDAL_ANA_INPUT_HandleGet(AudMuxObj, (void*)&(AnaInputStreamHandle)),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_HandleGet audio");

	TestManager_AssertEqual(TDAL_ANA_AUD_InputStreamSet(IndexAudioDecoder,
														AnaInputStreamHandle),
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
								eTDAL_ANA_AUD_NO_ERROR,
								"TDAL_ANA_AUD_DestinationSet FPD succeed");	

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



    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
															  &stAudioCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");
    stAudioAttenuation.FrontRLAttenuation     = stAudioCapability.AttenuationMax/8;
    stAudioAttenuation.FrontCenterAttenuation = stAudioCapability.AttenuationMax/8;
    stAudioAttenuation.SurroundRLAttenuation  = stAudioCapability.AttenuationMax/8;
    stAudioAttenuation.LFEAttenuation         = stAudioCapability.AttenuationMax/8;
    stAudioAttenuation.RearRLAttenuation      = stAudioCapability.AttenuationMax/8;
    stAudioAttenuation.RearCenterAttenuation  = stAudioCapability.AttenuationMax/8;
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

	stVideoStartParams.uiCodingMask  = eVidCoding;
	stVideoStartParams.uiStdMask     = stVideoDecoderCapability.uiSupportedStdMask;

    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");
    /*start audio and video Analog */
    stAudioStartParams.uiStdMask = stAudioDecoderCapability.uiSupportedStdMask;
    TestManager_AssertEqual(TDAL_ANA_AUD_Start(IndexAudioDecoder,
											   &stAudioStartParams),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Start succeed");
 

	/* digital tuner part */

	TestHelper_ControlChannel_TDAL_DMX(TRUE);

	TDAL_AV_VideoDestinationSet(eTDAL_AV_DECODER_VIDEO_1, LayerStreamHandle);

	TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
													TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

	StreamType.videoType = pChannel->videoType; /*eTDAL_AV_VIDEO_TYPE_MPEG2*/;
	TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"1st av start video");

	/* TKEL_DelayTask(ms2tick(5000)); */

	/*--------------------------*/
	/* wait for result          */
	/*--------------------------*/

    TestManager_AskForChecking( "do you see analog input video in main and digital tuner video in pip ?" );


    /*-------------------------*/
	/* swap source             */
	/*-------------------------*/

    TestManager_AssertEqual(TDAL_ANA_AUD_Stop(IndexAudioDecoder),
							eTDAL_ANA_AUD_NO_ERROR,
							"TDAL_ANA_AUD_Stop succeed");

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");

	TestManager_AssertEqual(TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1),
							eTDAL_AV_NO_ERROR,
							"1st av start video");

	TestHelper_ControlChannel_TDAL_DMX(FALSE);

	 stUseCase.eSecondVideoInput = stUseCase.eFirstVideoInput;
     stUseCase.eFirstVideoInput = eTDAL_USE_CASE_ANA_YPrPb;
	 stUseCase.eUseCaseWindow = eTDAL_USE_CASE_WINDOW_DUAL_PIP;
	 stUseCase.eAudioInput = eTDAL_USE_CASE_ANA_SCART;

    TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
                            TRUE,
                            "ConnectionManager selectUseCase");

	TDAL_ANA_VID_DestinationSet(IndexVideoDecoder, LayerStreamHandle);

	TestHelper_ControlChannel_TDAL_DMX(TRUE);

	TDAL_AV_VideoDestinationSet(eTDAL_AV_DECODER_VIDEO_1, SubLayerStreamHandle);

	TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
													TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
							eTDAL_AV_NO_ERROR,
							"TDAL AV input stream set\n");

	StreamType.videoType = pChannel->videoType; /*eTDAL_AV_VIDEO_TYPE_MPEG2*/;
	TestManager_AssertEqual(TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1,
										  StreamType),
							eTDAL_AV_NO_ERROR,
							"1st av start video");
    TestManager_AssertEqual(TDAL_ANA_VID_Start(IndexVideoDecoder,
											   &stVideoStartParams),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Start main succeed");

	/*  wait for result */
    TestManager_AskForChecking( "do you see analog input video in main and hdmi video in pip ?" );



    /*-------------------------*/
	/* terminate test          */
	/*-------------------------*/



    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"1st output audio disable" );

	if(stOutputCap.nbAudioAna > 1)
	{
		/* we suppose that when more than one audio ana, the second is a headphone */
		TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_HEADPHONE),
								eTDAL_OUTPUT_NO_ERROR,
								"1st output audio headphone disable" );
	}

    TestManager_AssertEqual(TDAL_ANA_VID_Stop(IndexVideoDecoder),
							eTDAL_ANA_VID_NO_ERROR,
							"TDAL_ANA_VID_Stop succeed");

	
    TestHelper_Terminate_ANA_AUD(IndexAudioDecoder);

    TestHelper_Terminate_ANA_VID(IndexVideoDecoder);


    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_1),
							eTDAL_DISP_NO_ERROR,
							"2nd video disp layer disable" );

    TestManager_AssertEqual(TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							eTDAL_DISP_NO_ERROR,
							"1st video disp layer disable" );
	
    TestManager_AssertEqual(TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio disable" );


    TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate" );


    TestManager_AssertEqual(TDAL_SPEAKERS_Term(),
							eTDAL_SPEAKERS_NO_ERROR,
							"speakers terminate" );
    TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate" );

    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_DISP_NO_ERROR,
							"av terminate" );


 
    TestManager_AssertEqual(TDAL_ANA_INPUT_Term(),
							eTDAL_ANA_INPUT_NO_ERROR,
							"TDAL_ANA_INPUT_Term succeed");





}


















