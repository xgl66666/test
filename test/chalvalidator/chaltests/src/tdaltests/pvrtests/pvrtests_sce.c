/*
 * pvrtests_sce.c
 *
 *  Created on: 
 *      Author: rtrk
 */


#include "crules.h"

#include "tkel.h"
#include "tbox.h"

#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"
#include "tdal_ts.h"
#include "tdal_fs.h"
#include "tdal_pvr.h"
#include "tdal_desc.h"

#include "testhelper.h"
#include "TestManager.h"

#define MOUNT_PATH   "/root/sdx1"
#define PVRTEST_FILENAME "test" // test001.ts test002.ts ...

LOCAL void TestCase_BasicPVRRecordingTest(void);
LOCAL void TestCase_BasicPVRPlaybackTest(void);
LOCAL void TestCase_PVRPlaybackMultipleAudioTracksTest(void);
LOCAL void TestCase_PVRPlaybackToLIVETest(void);
LOCAL void TestCase_PVRTimeshiftRecordingTest(void);
LOCAL void PVRRecordingFile(bool withVideo, const char * fileName, int nChannel);
LOCAL void PVRPlaybackFile(bool otherAudioTrack, const char * fileName, int nChannel);
LOCAL void PVRTimeshiftRecording(bool withVideo, const char * fileName, int nChannel);
LOCAL void PVRCommonInit(void);
LOCAL void PVRLivePLAY(int nChannel);
LOCAL void PVRLiveSTOP();
LOCAL void PVRCommonTerm(void);
LOCAL void VideoEventsUnregister(void);
LOCAL void VideoEventsRegister(void);
LOCAL void VideoNeedDisplayCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData);
LOCAL void PVRCallback(int ePvrEventType, void *data);
LOCAL void AV_StartDecoders(tTDAL_AV_Decoder videoDec, tTDAL_AV_Decoder audioDec, tTDAL_AV_VideoType videoStream, tTDAL_AV_AudioType audioStream);
LOCAL void AV_StopDecoders(tTDAL_AV_Decoder videoDec, tTDAL_AV_Decoder audioDec);

extern struct TestManager_Channel gChannelList[];

static tTestNode gTestBasicPVRRecordingScenario = {
    "TestPVREmbedded_BasicRecording",
    "basic recording Live and saving to USB using embedded PVR",
    0, /* not interactive */
    TestCase_BasicPVRRecordingTest,
        NULL
};

static tTestNode gTestBasicPVRPlaybackScenario = {
        "TestPVREmbedded_BasicPlayback",
        "basic playback of recorded TS file from USB using embedded PVR",
        0, /* not interactive */
        TestCase_BasicPVRPlaybackTest,
            NULL
};

static tTestNode gTestPVRPlaybackMultipleAudioTracksScenario = {
        "TestPVREmbedded_PlaybackMultipleAudioTracks",
        "playback of recorded TS file with multiple audio tracks from USB using embedded PVR",
        0, /* not interactive */
        TestCase_PVRPlaybackMultipleAudioTracksTest,
            NULL
};

static tTestNode gTestPVRPlaybackToLIVEScenario = {
        "TestPVREmbedded_PlaybackTOLive",
        "play/stop recorded TS file and then go LIVE",
        0, /* not interactive */
        TestCase_PVRPlaybackToLIVETest,
            NULL
};

static tTestNode gTestPVRtimeshiftRecordingScenario = {
        "TestPVREmbedded_Timeshift_4XFwd",
        "timeshift recording TS file and timeshift play",
        0, /* not interactive */
        TestCase_PVRTimeshiftRecordingTest,
            NULL
};

tTestNode* gTDAL_PVR_ScenarioTestNodeList[] = {
	&gTestBasicPVRRecordingScenario,
	&gTestBasicPVRPlaybackScenario,
	&gTestPVRPlaybackMultipleAudioTracksScenario,
	&gTestPVRPlaybackToLIVEScenario,
	&gTestPVRtimeshiftRecordingScenario,
    NULL
};

/*! \var tTestNode gTDAL_PVR_TestSuite
 * \brief Node for PVR test suite
 *
 *  This Node give useful info about PVR test suite.
 *  It also point to gTDAL_MPTestNodeList to know
 *  available PVR tests
 */
tTestNode gTDAL_PVR_ScenarioTestSuite = {
    "TDAL_PVR Scenarios",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_PVR_ScenarioTestNodeList
};

/*! \var tTestNode* gTDAL_MPTestNodeList[]
 *  \brief Node for M/P tests
 *
 *  This node references available tests for
 *  TDAL_MP tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_PVRTestNodeList[] = {
	&gTDAL_PVR_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_PVR_TestSuite
 * \brief Node for A/V test suite
 *
 *  This Node give useful info about media player test suite.
 *  It also point to gTDAL_MPTestNodeList to know
 *  available A/V tests
 */
tTestNode gTDAL_PVR_TestSuite = {
    "Test_TDAL_PVR",
    "test the PVR module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_PVRTestNodeList
};

volatile bool deviceMounted = false;
extern int gTDAL_AV_TestFeIdx;
extern int gTDAL_AV_TestDmxIdx;
extern int gChannel;
LOCAL bool bPVRCommonInitialized = false;

LOCAL void FS_DeviceCallback(tTDAL_FS_DeviceInfo *pDeviceInfo, tTDAL_FS_DeviceEvent DeviceEvent)
{
    uint8_t volumeCount;
    tTDAL_FS_VolumeInfo * volumes = NULL;

    if (DeviceEvent == eTDAL_FS_DEVICE_EVENT_INSERTED)
    {
        if (TDAL_FS_ListVolumes(pDeviceInfo->handle, &volumeCount, &volumes) == eTDAL_FS_NO_ERROR)
        {
            if (volumeCount > 0)
            {
                if (TDAL_FS_Mount(volumes[0].handle, MOUNT_PATH, 0)  == eTDAL_FS_NO_ERROR)
                {
                    printf("Device mounted\n");
                    deviceMounted = true;
                }
                else
                {
                    printf("Could not mount volume\n");
                }
            }
            else
            {
                printf("Volume array empty\n");
            }
        }
        else
        {
            printf("No volumes\n");
        }
    }
    else
    {

    }

    if (volumes)
    {
        TDAL_FS_FreeVolumesArray(volumes);
    }
}

uint8_t buffer[20*1024];

LOCAL void TestCase_BasicPVRRecordingTest()
{
    int nChannel;
    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }

    nChannel = 0;
    PVRLivePLAY(nChannel);
    PVRRecordingFile(false, PVRTEST_FILENAME, nChannel);
    TestManager_AskForChecking("Press '1' when you want change the channel");
    PVRLiveSTOP();
    
    TKEL_DelayTask(ms2tick(1000));
	
    nChannel++;
	PVRLivePLAY(nChannel);
	PVRRecordingFile(false, PVRTEST_FILENAME, nChannel);
	TestManager_AskForChecking("Press '1' when you want to stop the test");
	PVRLiveSTOP();
	
    /*      TDAL TERM for PVR       */
    if (bPVRCommonInitialized)
    {
        PVRCommonTerm();
        bPVRCommonInitialized = false;
    }  
}

LOCAL void TestCase_BasicPVRPlaybackTest()
{
    int nChannel;
    
    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }
    
    nChannel = 0;
    PVRPlaybackFile(false, PVRTEST_FILENAME, nChannel);
    
    nChannel++;
    PVRPlaybackFile(false, PVRTEST_FILENAME, nChannel);
    
    /*      TDAL TERM for PVR       */
    if (bPVRCommonInitialized)
    {
        PVRCommonTerm();
        bPVRCommonInitialized = false;
    }
}

LOCAL void TestCase_PVRPlaybackMultipleAudioTracksTest(void)
{
    int nChannel;

    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }

    nChannel = 0;
    PVRPlaybackFile(true, PVRTEST_FILENAME, nChannel);

    nChannel++;
    PVRPlaybackFile(true, PVRTEST_FILENAME, nChannel);

    /*      TDAL TERM for PVR       */
    if (bPVRCommonInitialized)
    {
        PVRCommonTerm();
        bPVRCommonInitialized = false;
    }
}

LOCAL void TestCase_PVRPlaybackToLIVETest()
{
    int nChannel;
    
    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }
    
    nChannel = 0;
    PVRPlaybackFile(false, PVRTEST_FILENAME, nChannel);
    TKEL_DelayTask(ms2tick(1000));
    nChannel++;
    PVRLivePLAY(nChannel);
    
    TestManager_AskForChecking("Do you see video playing?");

    TestManager_AskForChecking("Do you hear sound?");
    
    PVRLiveSTOP();
    /*      TDAL TERM for PVR       */
    if (bPVRCommonInitialized)
    {
        PVRCommonTerm();
        bPVRCommonInitialized = false;
    }
}

/*
 * Really small test to pvr timeshift
 */
LOCAL void TestCase_PVRTimeshiftRecordingTest(void)
{
    int nChannel;
    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }

    nChannel = 0;
    PVRLivePLAY(nChannel);
    PVRTimeshiftRecording(false, PVRTEST_FILENAME, nChannel);
    TestManager_AskForChecking("Press '1' when you want change the channel");
    PVRLiveSTOP();       
    /*      TDAL TERM for PVR       */
    if (bPVRCommonInitialized)
    {
        PVRCommonTerm();
        bPVRCommonInitialized = false;
    }
}

/*
 * Really small test to see if mp3 is working properly
 */
LOCAL void  PVRRecordingFile(bool withVideo, const char * fileName, int nChannel)
{
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;
    void * ptr;
    long int size;
    tTDAL_PVR_Handle    pvrHandle = 0;
    bool                bIsTimeshift = false;
    char    FileName[64] = {0};

    tTDAL_AV_Error tavErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_PVR_ComplexProgInfo compProgInfo;
    tTDAL_PVR_AudioInfo audioInfo[2] = {{"ven", 1, 132, 1}, {"eng", 1, 137, 1}};  

    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }

    sprintf(FileName, "%s%03d.ts", fileName, nChannel);
    compProgInfo.stPVRBasicProgInfo.u32AudioPid   = gChannelList[gChannel].mAudioPID;
    compProgInfo.stPVRBasicProgInfo.u32VideoPid   = gChannelList[gChannel].mVideoPID;
    compProgInfo.stPVRBasicProgInfo.u32PCRPid     = gChannelList[gChannel].mPCRPID;
    compProgInfo.stPVRBasicProgInfo.u32PmtPid     = gChannelList[gChannel].mPMTPID;
    compProgInfo.stPVRBasicProgInfo.u32ACodec     = 1;
    compProgInfo.stPVRBasicProgInfo.u32VCodec     = 0;
    compProgInfo.stPVRBasicProgInfo.u32LCN        = gChannelList[gChannel].mVideoPID; /* Should be checked - for now it is not important */
    compProgInfo.stPVRBasicProgInfo.fileName      = FileName;
    compProgInfo.stPVRBasicProgInfo.nAudioInfoCount   = 2;
    memcpy(compProgInfo.stPVRBasicProgInfo.pAudioInfo, audioInfo, sizeof(tTDAL_PVR_AudioInfo)*2);
    compProgInfo.stPVRBasicProgInfo.nSubsInfoCount    = 0;

    if (deviceMounted == false)
    {
        TestManager_AssertEqual(TDAL_FS_RegisterDeviceCallback(FS_DeviceCallback),
                eTDAL_FS_NO_ERROR,
                "fs_register device callback");

        /* TODO Should wait here until device plugged event received */

        while (deviceMounted == false)
        {
            TKEL_DelayTask(ms2tick(1000));
            printf("Waiting until device mounted\n");
        }
    }
    //fileTest(fileName);

    TestManager_AssertEqual(TDAL_PVR_Init(),
                        eTDAL_PVR_NO_ERROR,
                        "pvr init");

    TestManager_AssertEqual(TDAL_PVR_Open(&pvrHandle, bIsTimeshift, MOUNT_PATH),
                        eTDAL_PVR_NO_ERROR,
                        "pvr open" );
    
    TestManager_AssertEqual(TDAL_PVR_SetProgramInfo(pvrHandle, &compProgInfo.stPVRBasicProgInfo),
            eTDAL_PVR_NO_ERROR,
            "pvr set program info" );
    
	TestManager_AssertEqual(TDAL_PVR_Record_Start(pvrHandle),
	        eTDAL_PVR_NO_ERROR,
	                    "pvr start" );

	TestManager_AskForChecking("Press '1' when you want to stop recording");
	

    TestManager_AssertEqual(TDAL_PVR_Record_Stop(pvrHandle),
            eTDAL_PVR_NO_ERROR,
                        "pvr stop");
    
    TDAL_PVR_GetProgramInfo(pvrHandle, &compProgInfo);
    printf("PVR RECORDING DURATION = %d\n", compProgInfo.u32duration);
    
    TestManager_AssertEqual(TDAL_PVR_Close(pvrHandle),
                eTDAL_PVR_NO_ERROR,
                            "pvr close");
}

/*
 * Really small test to see if PVR playback is working properly
 */
LOCAL void  PVRPlaybackFile(bool otherAudioTrack, const char * fileName, int nChannel)
{
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;
    void * ptr;
    long int size;
    tTDAL_PVR_Handle    pvrHandle = 0;
    bool                bIsTimeshift = false;
    char    FileName[64] = {0};

    tTDAL_AV_Error tavErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_PVR_BasicProgInfo basicProgInfo;
    tTDAL_PVR_ComplexProgInfo complexProgInfo;

    /* ************************************************** */
    /*                      Configuration of  TDAL modules                      */
    /* ************************************************** */
    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }
    
    if (deviceMounted == false)
    {
        TestManager_AssertEqual(TDAL_FS_RegisterDeviceCallback(FS_DeviceCallback),
                eTDAL_FS_NO_ERROR,
                "fs_register device callback");

        /* TODO Should wait here until device plugged event received */

        while (deviceMounted == false)
        {
            TKEL_DelayTask(ms2tick(1000));
            printf("Waiting until device mounted\n");
        }
    }

    TestManager_AssertEqual(TDAL_PVR_Init(),
                        eTDAL_PVR_NO_ERROR,
                        "pvr init");

    TestManager_AssertEqual(TDAL_PVR_Open(&pvrHandle, bIsTimeshift, MOUNT_PATH),
                        eTDAL_PVR_NO_ERROR,
                        "pvr open" );
    sprintf(FileName, "%s%03d.ts", fileName, ((otherAudioTrack) ? 0: nChannel));
    complexProgInfo.stPVRBasicProgInfo.fileName = FileName;
    TDAL_PVR_GetProgramInfo(pvrHandle, &complexProgInfo);
    
    basicProgInfo.u32AudioPid       = complexProgInfo.stPVRBasicProgInfo.pAudioInfo[((otherAudioTrack) ? nChannel : 0)].u16AudioPID;
    basicProgInfo.u32VideoPid       = complexProgInfo.stPVRBasicProgInfo.u32VideoPid;
    basicProgInfo.u32PCRPid         = complexProgInfo.stPVRBasicProgInfo.u32PCRPid;
    basicProgInfo.u32PmtPid         = complexProgInfo.stPVRBasicProgInfo.u32PmtPid;
    basicProgInfo.u32ACodec         = complexProgInfo.stPVRBasicProgInfo.pAudioInfo[((otherAudioTrack) ? nChannel : 0)].u8AudioType;
    basicProgInfo.u32VCodec         = 0;
    basicProgInfo.u32LCN            = gChannelList[gChannel].mVideoPID; /* Should be checked - for now it is not important */
    basicProgInfo.fileName          = FileName;
    basicProgInfo.nAudioInfoCount   = 1;
    basicProgInfo.nSubsInfoCount    = 0;
    
    TestManager_AssertEqual(TDAL_PVR_SetProgramInfo(pvrHandle, &basicProgInfo),
            eTDAL_PVR_NO_ERROR,
            "pvr set program info" );
    
    uint32_t u32TimeinSecs = 0;
    TestManager_AssertEqual(TDAL_PVR_PlaybackStart(pvrHandle, u32TimeinSecs),
            eTDAL_PVR_NO_ERROR,
            "pvr playback start" );
    
    /* ************************************************** */
    /*              Starting AV decoders                  */
    /* ************************************************** */       
    AV_StartDecoders(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_DECODER_AUDIO_1,
            eTDAL_AV_VIDEO_TYPE_MPEG2, eTDAL_AV_AUDIO_TYPE_MPEG);
    
    TestManager_AskForChecking("Do you see video playing?");
    
    TestManager_AssertEqual(TDAL_PVR_PlaybackStop(pvrHandle),
                eTDAL_PVR_NO_ERROR,
                "pvr playback stop" );

    TestManager_AssertEqual(TDAL_PVR_Close(pvrHandle),
                    eTDAL_PVR_NO_ERROR,
                                "pvr close");
    
    /* Stopping AV decoders, disabling display and audio output*/
    AV_StopDecoders(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_DECODER_AUDIO_1);
}

/*
 * Basic timeshift recording/playback test.
 */
LOCAL void  PVRTimeshiftRecording(bool withVideo, const char * fileName, int nChannel)
{
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;
    void * ptr;
    long int size;
    tTDAL_PVR_Handle    pvrHandle = 0;
    bool                bIsTimeshift = true;
    char    FileName[64] = {0};
    char    trick = '0';

    tTDAL_AV_Error tavErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_PVR_ComplexProgInfo compProgInfo;

    /*      TDAL INIT for PVR       */
    if (!bPVRCommonInitialized)
    {
        PVRCommonInit();
        bPVRCommonInitialized = true;
    }

    sprintf(FileName, "%s%03d.ts", fileName, nChannel);
    compProgInfo.stPVRBasicProgInfo.u32AudioPid   = gChannelList[gChannel].mAudioPID;
    compProgInfo.stPVRBasicProgInfo.u32VideoPid   = gChannelList[gChannel].mVideoPID;
    compProgInfo.stPVRBasicProgInfo.u32PCRPid     = gChannelList[gChannel].mPCRPID;
    compProgInfo.stPVRBasicProgInfo.u32PmtPid     = gChannelList[gChannel].mPMTPID;
    compProgInfo.stPVRBasicProgInfo.u32ACodec     = 1;
    compProgInfo.stPVRBasicProgInfo.u32VCodec     = 0;
    compProgInfo.stPVRBasicProgInfo.u32LCN        = gChannelList[gChannel].mVideoPID; /* Should be checked - for now it is not important */
    compProgInfo.stPVRBasicProgInfo.fileName      = FileName;

    if (deviceMounted == false)
    {
        TestManager_AssertEqual(TDAL_FS_RegisterDeviceCallback(FS_DeviceCallback),
                eTDAL_FS_NO_ERROR,
                "fs_register device callback");

        /* TODO Should wait here until device plugged event received */

        while (deviceMounted == false)
        {
            TKEL_DelayTask(ms2tick(1000));
            printf("Waiting until device mounted\n");
        }
    }
    //fileTest(fileName);

    TestManager_AssertEqual(TDAL_PVR_Init(),
                        eTDAL_PVR_NO_ERROR,
                        "pvr init");
    
    TestManager_AssertEqual(TDAL_PVR_RegisterCallback(PVRCallback),
                            eTDAL_PVR_NO_ERROR,
                            "pvr register callback");    

    TestManager_AssertEqual(TDAL_PVR_Open(&pvrHandle, bIsTimeshift, MOUNT_PATH),
                        eTDAL_PVR_NO_ERROR,
                        "pvr open" );
    
    TestManager_AssertEqual(TDAL_PVR_SetProgramInfo(pvrHandle, &compProgInfo.stPVRBasicProgInfo),
            eTDAL_PVR_NO_ERROR,
            "pvr set program info" );
    
    TestManager_AssertEqual(TDAL_PVR_Timeshift_RecordStart(pvrHandle),
            eTDAL_PVR_NO_ERROR,
                        "pvr timeshift recording start" );
    /*      Disable Video Layer      */
    tdispErr = TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0);
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp layer video disable");

    TestManager_AskForChecking("Press '1' when you want to play recorded content");
    
    TestManager_AssertEqual(TDAL_PVR_Timeshift_PlaybackStart(pvrHandle, 0),
            eTDAL_PVR_NO_ERROR,
                        "pvr timeshift playback start" );
    
    /* ************************************************** */
    /*              Starting AV decoders                  */
    /* ************************************************** */       
    AV_StartDecoders(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_DECODER_AUDIO_1,
            eTDAL_AV_VIDEO_TYPE_MPEG2, eTDAL_AV_AUDIO_TYPE_MPEG);      

    TKEL_DelayTask(ms2tick(10000));
    TDAL_PVR_PlaybackFastForward(pvrHandle, eTDAL_PVR_SPEED_4X_FORWARD);

    TestManager_AskForChecking("Press '1' when you want to stop playback");
    TestManager_AssertEqual(TDAL_PVR_Timeshift_PlaybackStop(pvrHandle),
            eTDAL_PVR_NO_ERROR,
                        "pvr playback timeshift stop");

    TestManager_AskForChecking("Press '1' when you want to stop timeshift recording");
    TestManager_AssertEqual(TDAL_PVR_Timeshift_RecordStop(pvrHandle),
            eTDAL_PVR_NO_ERROR,
                        "pvr recording timeshift stop");
    
    TestManager_AssertEqual(TDAL_PVR_Close(pvrHandle),
                eTDAL_PVR_NO_ERROR,
                            "pvr close");
}

LOCAL void PVRLivePLAY(int nChannel)
{
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;
    tTDAL_AV_Error          tavErr;
    tTDAL_AV_StreamType     StreamType;
    tTDAL_DISP_Error        tdispErr;
    tTDAL_OUTPUT_Error      toutErr;

    /*      Tuning on nChannel Channel      */
    gChannel = nChannel;
    TestHelper_DMDTune(gTDAL_AV_TestFeIdx);
    
    /*      TDAL DMX      */
    TestHelper_Configure_TDAL_DMX(gTDAL_AV_TestDmxIdx);
    
    /* ************************************************** */
    /*              Starting AV decoders                  */
    /* ************************************************** */       
    AV_StartDecoders(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_DECODER_AUDIO_1,
            eTDAL_AV_VIDEO_TYPE_MPEG2, eTDAL_AV_AUDIO_TYPE_MPEG);
}

LOCAL void PVRLiveSTOP()
{
    tTDAL_AV_Error          tavErr;
    tTDAL_AV_StreamType     StreamType;
    tTDAL_DISP_Error        tdispErr;
    tTDAL_OUTPUT_Error      toutErr;

    /* Stopping AV decoders, disabling display and output*/
    AV_StopDecoders(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_DECODER_AUDIO_1);

    /* Disable all the channel (Video, Audio, PCR) */
    TestHelper_ControlChannel_TDAL_DMX(FALSE);
    /* ************************************************** */
    /*  Termination of all needed TDAL modules            */
    /* ************************************************** */

    //VideoEventsUnregister();
    //AudioEventsUnregister();     
}

void PVRCommonInit(void)
{
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;

    tTDAL_AV_Error tavErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_PVR_BasicProgInfo basicProgInfo;
#ifdef PRODUCT_USE_CA
    tTDAL_DESC_Error tdescErr;
#endif

    /* ************************************************** */
    /*                Initialization of all needed TDAL modules               */
    /* ************************************************** */

    TestHelper_Initialize_TDAL_AV();

    TestManager_AssertEqual(TDAL_FS_Init(),
                            eTDAL_FS_NO_ERROR,
                            "fs_init");
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
    
    VideoEventsRegister();
}

LOCAL void PVRCommonTerm(void)
{
    tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
    tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
    tTDAL_DISP_LayerWindow  stInputWindow;
    tTDAL_DISP_LayerWindow  stOutputWindow;
    uint32_t                uiWidth;
    uint32_t                uiHeight;

    tTDAL_AV_Error tavErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_PVR_BasicProgInfo basicProgInfo;

    TestHelper_Terminate_TDAL_AV();
}

LOCAL void VideoNeedDisplayCallback(tTDAL_AV_Decoder decoder, tTDAL_AV_Event event, const void *pEventData)
{
    tTDAL_DISP_Error tdispErr;

    /*      Enable Video Layer      */
    tdispErr = TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp layer video enable");
}

LOCAL void VideoEventsRegister(void)
{
    tTDAL_AV_Error tavErr;

    /* register the video callback needed for the display */
    tavErr = TDAL_AV_EventSubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY, VideoNeedDisplayCallback);
    
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av register video need display");
 
}

LOCAL void VideoEventsUnregister(void)
{
    tTDAL_AV_Error tavErr;

    /* unregister the video callback needed for the display */
    tavErr = TDAL_AV_EventUnsubscribe(eTDAL_AV_DECODER_VIDEO_1, eTDAL_AV_EVENT_VIDEO_NEED_DISPLAY);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av unregister video need display");

}

LOCAL void PVRCallback(int pvrEventType, void *data)
{
    tTDAL_PVR_Event event = (tTDAL_PVR_Event)pvrEventType;
    tTDAL_PVR_Handle pvrHandle = NULL;
    switch (event)
    {
    case eTDAL_PVR_END_FILE:
        pvrHandle = data;
        
        if (pvrHandle == NULL)
        {
            return;
        }
        
        if (TDAL_PVR_IsRecording(pvrHandle))
        {
            tTDAL_AV_StreamType StreamType;
            TDAL_PVR_Timeshift_PlaybackStop(pvrHandle);
            /*      Disable Video Layer      */
            TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0);

            /*      Stop Video Decoder      */
            TDAL_AV_Stop(eTDAL_AV_DECODER_VIDEO_1);

            /*      Disable Audio Output      */
            TDAL_OUTPUT_Disable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);

            /*      Stop Audio Decoder      */
            TDAL_AV_Stop(eTDAL_AV_DECODER_AUDIO_1);
            /*      Start Video Decoder      */
            StreamType.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

            TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
                                                        TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
                                eTDAL_AV_NO_ERROR,
                                "TDAL AV input stream set\n");

            TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1, StreamType);
            /*      Start Audio Decoder      */
            StreamType.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;

            TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
                                                        TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
                                eTDAL_AV_NO_ERROR,
                                "TDAL AV input stream set\n");

            TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1, StreamType);

            /*      Enable Audio Output      */
            TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);
        }
        else
        {
            TDAL_PVR_PlaybackStop(pvrHandle);
        }
        break;
    case eTDAL_PVR_BEGIN_FILE:
        
        break;
    case eTDAL_PVR_OPEN_FILE_FAIL:
        
        break;
    case eTDAL_PVR_READ_FILE_FAIL:
        
        break;
    case eTDAL_PVR_WRITE_FILE_FAIL:
        
        break;
    case eTDAL_PVR_NO_DISK_SPACE:
        
        break;
    }
}

LOCAL void AV_StartDecoders(tTDAL_AV_Decoder videoDec, tTDAL_AV_Decoder audioDec, tTDAL_AV_VideoType videoStream, tTDAL_AV_AudioType audioStream)
{
    tTDAL_AV_Error tavErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_AV_StreamType StreamType;
    
    StreamType.videoType = videoStream;
    /* ************************************************** */
    /*                                  Video Starting    */
    /* ************************************************** */
    /*      Start Video Decoder      */
    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
                                                TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_VIDEO_1)),
                        eTDAL_AV_NO_ERROR,
                        "TDAL AV input stream set\n");

    tavErr = TDAL_AV_Start(eTDAL_AV_DECODER_VIDEO_1, StreamType);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av start video");   
    
    /* ************************************************** */
    /*                                  Audio Starting                                   */
    /* ************************************************** */

    /*      Start Audio Decoder      */
    TestManager_AssertEqual(TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
                                                TestHelper_DMXGetStreamHandle(eTDAL_AV_DECODER_AUDIO_1)),
                        eTDAL_AV_NO_ERROR,
                        "TDAL AV input stream set\n");

    StreamType.audioType = audioStream;
    tavErr = TDAL_AV_Start(eTDAL_AV_DECODER_AUDIO_1, StreamType);
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av start audio");

    /*      Enable Audio Output      */
    toutErr = TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0);
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output audio enable");    
}

LOCAL void AV_StopDecoders(tTDAL_AV_Decoder videoDec, tTDAL_AV_Decoder audioDec)
{
    tTDAL_AV_Error tavErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_AV_StreamType StreamType;
    tTDAL_DISP_Error tdispErr;
    
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
}
