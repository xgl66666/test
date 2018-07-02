/*
 * mptests_sce.c
 *
 *  Created on: 11 Feb 2013
 *      Author: mijovic
 */

#include <stdio.h>
#ifdef __cplusplus
#include <string.h>
#endif

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
#include "tdal_mp.h"

#include "testhelper.h"
#include "TestManager.h"

#define MOUNT_PATH   "/Mount2"
#define MPTEST_MP3_SAMPLE MOUNT_PATH "/test_sample_mp3.mp3"
#define MPTEST_WMA_SAMPLE MOUNT_PATH "/test_sample_wma.wma"
#define MPTEST_AAC_SAMPLE MOUNT_PATH "/test_sample_aac.aac"
#define MPTEST_WAV_SAMPLE MOUNT_PATH "/test_sample_wav.wav"
#define MPTEST_AVI_SAMPLE   MOUNT_PATH "/test_sample_avi.avi"
#define MPTEST_MP4_SAMPLE   MOUNT_PATH "/test_sample_mp4.mp4"
#define MPTEST_MPG_SAMPLE   MOUNT_PATH "/test_sample_mpg.mpg"
#define MPTEST_BMP_SAMPLE   MOUNT_PATH "/test_sample_bmp.bmp"
#define MPTEST_JPG_SAMPLE   MOUNT_PATH "/test_sample_jpg.jpg"
#define MPTEST_PNG_SAMPLE   MOUNT_PATH "/test_sample_png.png"
#define MPTEST_GIF_SAMPLE   MOUNT_PATH "/test_sample_gif.gif"
#define MPTEST_ANIM_GIF_SAMPLE   MOUNT_PATH "/test_sample_anim_gif.gif"

#define MP_TEST_DEFAULT_HEIGHT 576
#define MP_TEST_DEFAULT_WIDTH 720

mTBOX_SET_MODULE(eTDAL_TEST_MP);

enum 		eFileType 			{MP3,   WMA,   AAC,   WAV,   AVI,   MP4,   MPG,   BMP,   JPG,   PNG,   GIF};
LOCAL char* aMediaExtensions[] ={"mp3",	"wma", "aac", "wav", "avi", "mp4", "mpg", "bmp", "jpg", "png", "gif"};

LOCAL void pMP_TEST_MediaPlayerPlayAudioFile(enum eFileType type, char * fileName);
LOCAL void pMP_TEST_MediaPlayerPlayVideoFile(enum eFileType type, char * fileName);

LOCAL void TestCase_MediaPlayerTestMP3(void);
LOCAL void TestCase_MediaPlayerTestWMA(void);
LOCAL void TestCase_MediaPlayerTestAAC(void);
LOCAL void TestCase_MediaPlayerTestWAV(void);
LOCAL void TestCase_VideoMediaPlayerTestMP4(void);
LOCAL void TestCase_VideoMediaPlayerTestAVI(void);
LOCAL void TestCase_VideoMediaPlayerTestMPG(void);
LOCAL void TestCase_ImageMediaPlayerTestBMP(void);
LOCAL void TestCase_ImageMediaPlayerTestJPG(void);
LOCAL void TestCase_ImageMediaPlayerTestPNG(void);
LOCAL void TestCase_ImageMediaPlayerTestGIF(void);
LOCAL void TestCase_ImageMediaPlayerTestAnimGIF(void);
LOCAL void pMP_TEST_Init();
LOCAL void pMP_TEST_Term();


LOCAL tTestNode gAudioMediaPlayerScenarioMP3 = {
		"TestMediaPlayer_mp3",
		"start playing file from USB using media player",
		0, /* not interactive */
		TestCase_MediaPlayerTestMP3,
		NULL
};

LOCAL tTestNode gAudioMediaPlayerScenarioWMA = {
		"TestMediaPlayer_wma",
		"start playing file from USB using media player",
		0, /* not interactive */
		TestCase_MediaPlayerTestWMA,
		NULL
};

LOCAL tTestNode gAudioMediaPlayerScenarioAAC = {
		"TestMediaPlayer_aac",
		"start playing file from USB using media player",
		0, /* not interactive */
		TestCase_MediaPlayerTestAAC,
		NULL
};

LOCAL tTestNode gAudioMediaPlayerScenarioWAV = {
        "TestMediaPlayer_wav",
        "start playing file from USB using media player",
        0, /* not interactive */
        TestCase_MediaPlayerTestWAV,
        NULL
};

LOCAL tTestNode gTestVideoMediaPlayerScenarioAVI = {
		"TestMediaPlayer_avi",
		"start playing video from USB using media player",
		0, /* not interactive */
		TestCase_VideoMediaPlayerTestAVI,
		NULL
};

LOCAL tTestNode gTestVideoMediaPlayerScenarioMP4 = {
		"TestMediaPlayer_mp4",
		"start playing video from USB using media player",
		0, /* not interactive */
		TestCase_VideoMediaPlayerTestMP4,
		NULL
};

LOCAL tTestNode gTestVideoMediaPlayerScenarioMPG = {
		"TestMediaPlayer_mpg",
		"start playing mpg video from USB using media player",
		0, /* not interactive */
		TestCase_VideoMediaPlayerTestMPG,
		NULL
};

LOCAL tTestNode gTestImageMediaPlayerScenarioBMP = {
		"TestMediaPlayer_Image_bmp",
		"show bmp image from USB using media player",
		0, /* not interactive */
		TestCase_ImageMediaPlayerTestBMP,
		NULL
};

LOCAL tTestNode gTestImageMediaPlayerScenarioJPG = {
		"TestMediaPlayer_Image_jpg",
		"show jpg image from USB using media player",
		0, /* not interactive */
		TestCase_ImageMediaPlayerTestJPG,
		NULL
};

LOCAL tTestNode gTestImageMediaPlayerScenarioPNG = {
		"TestMediaPlayer_Image_png",
		"show png image from USB using media player",
		0, /* not interactive */
		TestCase_ImageMediaPlayerTestPNG,
		NULL
};

LOCAL tTestNode gTestImageMediaPlayerScenarioGIF = {
        "TestMediaPlayer_Image_gif",
        "show png image from USB using media player",
        0, /* not interactive */
        TestCase_ImageMediaPlayerTestGIF,
        NULL
};

LOCAL tTestNode gTestImageMediaPlayerScenarioAnimGIF = {
        "TestMediaPlayer_Image_anim_gif",
        "show png image from USB using media player",
        0, /* not interactive */
        TestCase_ImageMediaPlayerTestAnimGIF,
        NULL
};

tTestNode* gTDAL_MP_ScenarioTestNodeList[] = {
		&gAudioMediaPlayerScenarioMP3,
		&gAudioMediaPlayerScenarioWMA,
		&gAudioMediaPlayerScenarioAAC,
		&gAudioMediaPlayerScenarioWAV,
		&gTestVideoMediaPlayerScenarioAVI,
		&gTestVideoMediaPlayerScenarioMP4,
		&gTestVideoMediaPlayerScenarioMPG,
		&gTestImageMediaPlayerScenarioBMP,
		&gTestImageMediaPlayerScenarioJPG,
		&gTestImageMediaPlayerScenarioPNG,
		&gTestImageMediaPlayerScenarioGIF,
		&gTestImageMediaPlayerScenarioAnimGIF,
		NULL
};

/*! \var tTestNode gTDAL_MP_TestSuite
 * \brief Node for MP test suite
 *
 *  This Node give useful info about media player test suite.
 *  It also point to gTDAL_MPTestNodeList to know
 *  available MP tests
 */
tTestNode gTDAL_MP_ScenarioTestSuite = {
		"TDAL_MP Scenarios",
		"test the mp module",
		0, /* not interactive */
		NULL, /* no function, it's a test suite */
		(void**)gTDAL_MP_ScenarioTestNodeList
};

/*! \var tTestNode* gTDAL_MPTestNodeList[]
 *  \brief Node for MP tests
 *
 *  This node references available tests for
 *  TDAL_MP tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_MPTestNodeList[] = {
		&gTDAL_MP_ScenarioTestSuite,
		NULL
};

/*! \var tTestNode gTDAL_MP_TestSuite
 * \brief Node for A/V test suite
 *
 *  This Node give useful info about media player test suite.
 *  It also point to gTDAL_MPTestNodeList to know
 *  available mp tests
 */
tTestNode gTDAL_MP_TestSuite = {
		"Test_TDAL_MP",
		"test the mp module",
		0, /* not interactive */
		NULL, /* no function, it's a test suite */
		(void**)gTDAL_MPTestNodeList
};


LOCAL bool deviceMounted = false;


LOCAL void FS_DeviceCallback(       tTDAL_FS_DeviceInfo *   pDeviceInfo ,
		tTDAL_FS_DeviceEvent    DeviceEvent )
{
	uint8_t volumeCount;
	tTDAL_FS_VolumeInfo * volumes = NULL;

	mTBOX_FCT_ENTER("FS_DeviceCallback");

	if (DeviceEvent == eTDAL_FS_DEVICE_EVENT_INSERTED)
	{

		if (TDAL_FS_ListVolumes(pDeviceInfo->handle, &volumeCount, &volumes) == eTDAL_FS_NO_ERROR)
		{
			if (volumeCount > 0)
			{
				if (TDAL_FS_Mount(volumes[0].handle, MOUNT_PATH, 0)  == eTDAL_FS_NO_ERROR)
				{
					mTBOX_TRACE((kTBOX_NIV_1, "Device mounted"));
					deviceMounted = true;
				}
				else
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not mount volume"));
				}
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Volume array empty"));
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No volumes"));
		}
	}
	else
	{
		if (TDAL_FS_ListVolumes(pDeviceInfo->handle, &volumeCount, &volumes) == eTDAL_FS_NO_ERROR)
		{
			if (volumeCount > 0)
			{
				if (TDAL_FS_Umount(MOUNT_PATH)  == eTDAL_FS_NO_ERROR)
				{
					mTBOX_TRACE((kTBOX_NIV_1, "Device unmounted"));
					deviceMounted = false;
				}
				else
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not unmount volume"));
				}
			}
			else
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Volume array empty"));
			}

		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No volumes"));
		}
	}

	if (volumes)
	{
		TDAL_FS_FreeVolumesArray(volumes);
	}

	mTBOX_RETURN;
}

LOCAL tTDAL_FS_File filePtr;


uint32_t mptest_InputDataRead(tTDAL_MP_Handle Handle, uint8_t *pucBuffer, uint32_t ulSize)
{
	uint32_t bytesRead;

	mTBOX_FCT_ENTER("mptest_InputDataRead");

	if (filePtr == kTDAL_FS_INVALID_HANDLE)
	{
		mTBOX_RETURN(-1);
	}

	bytesRead = TDAL_FS_Read(filePtr, pucBuffer, ulSize);

	mTBOX_RETURN(bytesRead);
}

uint32_t mptest_InputDataSeek(tTDAL_MP_Handle Handle, int32_t llOffset, tTDAL_MP_SeekMode eMode)
{
	tTDAL_FS_Seek_Origin whence = eTDAL_FS_SEEK_ORIGIN_BEGIN;

	mTBOX_FCT_ENTER("mptest_InputDataSeek");

	if (filePtr == kTDAL_FS_INVALID_HANDLE)
	{
		mTBOX_RETURN(-1);
	}

	switch(eMode)
	{
	case eTDAL_MP_SEEK_MODE_SET:
		whence = eTDAL_FS_SEEK_ORIGIN_BEGIN;
		break;
	case eTDAL_MP_SEEK_MODE_CUR:
		whence = eTDAL_FS_SEEK_ORIGIN_CURRENT;
		break;
	case eTDAL_MP_SEEK_MODE_END:
		whence = eTDAL_FS_SEEK_ORIGIN_END;
		break;
	default:
		TestManager_AssertTrue(false, "mptest_InputDataSeek wrong param");
		break;
	}

	mTBOX_RETURN( TDAL_FS_Seek(filePtr, llOffset, whence) );
}

uint32_t mptest_InputDataLength(tTDAL_MP_Handle Handle, int32_t llOffset, tTDAL_MP_SeekMode eMode)
{
	tTDAL_FS_Size position = 0, length = 0;
	tTDAL_FS_Error err;

	mTBOX_FCT_ENTER("mptest_InputDataLength");

	if (filePtr == kTDAL_FS_INVALID_HANDLE)
	{
		mTBOX_RETURN(0);
	}

	TDAL_FS_Tell(filePtr, &position);

	TDAL_FS_Seek(filePtr, 0, eTDAL_FS_SEEK_ORIGIN_END);

	TDAL_FS_Tell(filePtr, &length);

	TDAL_FS_Seek(filePtr, position, eTDAL_FS_SEEK_ORIGIN_BEGIN);

	mTBOX_RETURN(length);
}



LOCAL void  pMP_TEST_MediaPlayerShowImage(enum eFileType type, char * fileName)
{
	tTDAL_MP_Handle mpHandle;
	tTDAL_MP_InitParams initParams;
	tTDAL_MP_OpenParams openParams;
	tTDAL_MP_StartParams startParams;
	tTDAL_DISP_LayerWindow  stInputWindow;
	tTDAL_DISP_LayerWindow  stOutputWindow;
	uint32_t                uiWidth;
	uint32_t                uiHeight;

	mTBOX_FCT_ENTER("pMP_Test_MediaPlayerShowImage");

	pMP_TEST_Init();

    if(deviceMounted == false)
    {
        TestManager_LogMessage("Please insert USB");
    }

	while (deviceMounted == false)
	{
		TKEL_DelayTask(ms2tick(1000));
		 mTBOX_TRACE((kTBOX_NIV_WARNING,"Device is not mounted. Insert device to continue"));
	}

	TestManager_AssertEqual(TDAL_MP_Init(&initParams),
			eTDAL_MP_NO_ERROR,
			"mp init");

	filePtr = TDAL_FS_Open(fileName, kTDAL_FS_MODE_READ);

	TestManager_AssertTrue(filePtr != kTDAL_FS_INVALID_HANDLE, "tdal_fs_open");

    stInputWindow.Left       = 0;
    stInputWindow.Top        = 0;
    stInputWindow.Right      = 720;
    stInputWindow.Bottom     = 576;

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

    memset(&openParams, 0, sizeof(openParams));
    openParams.InputDataRead = mptest_InputDataRead;
    openParams.InputDataSeek = mptest_InputDataSeek;
    openParams.InputDataLength = mptest_InputDataLength;
    TestManager_AssertEqual(TKEL_Malloc(4*sizeof(char), &openParams.fileExtension),
            TKEL_NO_ERR,
            "tkel malloc");
    strcpy(openParams.fileExtension, aMediaExtensions[type]);

    TestManager_AssertEqual(TDAL_MP_Open(&openParams, &mpHandle),
            eTDAL_MP_NO_ERROR,
            "mp open");

	tTDAL_MP_MetaDataPict imgMetadata;
	tTDAL_MP_Error metadataError;

	metadataError = TDAL_MP_PictureGetMetadata(mpHandle, &imgMetadata);
	
	TestManager_AssertEqual(metadataError, eTDAL_MP_NO_ERROR, "TDAL_MP_PictureGetMetadata");
	
	if(metadataError != eTDAL_MP_NO_ERROR)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not get metadata!"));
		pMP_TEST_Term();
		mTBOX_RETURN;	
	}

	if(!imgMetadata.bIsSupported)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING,"This media file is not supported!"));
		pMP_TEST_Term();
		mTBOX_RETURN;
	}

	startParams.iSpeed = 100;
	startParams.uiTime = 0;


	TestManager_AssertEqual(TDAL_MP_Start(mpHandle, &startParams),
			eTDAL_MP_NO_ERROR,
			"mp start" );

	TestManager_AskForChecking("Do you see image?");

	TestManager_AssertEqual(TDAL_MP_Stop(mpHandle),
			eTDAL_MP_NO_ERROR,
			"mp stop");

	pMP_TEST_Term();

	mTBOX_RETURN;
}

LOCAL void TestCase_MediaPlayerTestMP3()
{
	pMP_TEST_MediaPlayerPlayAudioFile(MP3, MPTEST_MP3_SAMPLE);
}

LOCAL void TestCase_MediaPlayerTestWMA()
{
	pMP_TEST_MediaPlayerPlayAudioFile(WMA, MPTEST_WMA_SAMPLE);
}

LOCAL void TestCase_MediaPlayerTestAAC()
{
	pMP_TEST_MediaPlayerPlayAudioFile(AAC, MPTEST_AAC_SAMPLE);
}

LOCAL void TestCase_MediaPlayerTestWAV()
{
    pMP_TEST_MediaPlayerPlayAudioFile(WAV, MPTEST_WAV_SAMPLE);
}

LOCAL void TestCase_VideoMediaPlayerTestAVI()
{
	pMP_TEST_MediaPlayerPlayVideoFile(AVI, MPTEST_AVI_SAMPLE);
}

LOCAL void TestCase_VideoMediaPlayerTestMPG()
{
	pMP_TEST_MediaPlayerPlayVideoFile(MPG, MPTEST_MPG_SAMPLE);
}

LOCAL void TestCase_VideoMediaPlayerTestMP4()
{
	pMP_TEST_MediaPlayerPlayVideoFile(MP4, MPTEST_MP4_SAMPLE);
}

LOCAL void TestCase_ImageMediaPlayerTestBMP()
{
	pMP_TEST_MediaPlayerShowImage(BMP, MPTEST_BMP_SAMPLE);
}

LOCAL void TestCase_ImageMediaPlayerTestJPG()
{
	pMP_TEST_MediaPlayerShowImage(JPG, MPTEST_JPG_SAMPLE);
}

LOCAL void TestCase_ImageMediaPlayerTestPNG()
{
	pMP_TEST_MediaPlayerShowImage(PNG, MPTEST_PNG_SAMPLE);
}

LOCAL void TestCase_ImageMediaPlayerTestGIF()
{
    pMP_TEST_MediaPlayerShowImage(GIF, MPTEST_GIF_SAMPLE);
}

LOCAL void TestCase_ImageMediaPlayerTestAnimGIF()
{
    pMP_TEST_MediaPlayerShowImage(GIF, MPTEST_ANIM_GIF_SAMPLE);
}

LOCAL void pMP_TEST_MediaPlayerPlayAudioFile(enum eFileType type, char * fileName)
{
	tTDAL_MP_Handle mpHandle;
	tTDAL_MP_InitParams initParams;
	tTDAL_MP_OpenParams openParams;
	tTDAL_MP_StartParams startParams;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
	tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
	tTDAL_DISP_LayerWindow  stInputWindow;
	tTDAL_DISP_LayerWindow  stOutputWindow;
	uint32_t                uiWidth;
	uint32_t                uiHeight;

	mTBOX_FCT_ENTER(("pMP_TEST_MediaPlayerPlayAudioFile"));

	pMP_TEST_Init();

	if(deviceMounted == false)
	{
	    TestManager_LogMessage("Please insert USB");
	}

	while (deviceMounted == false)
	{
		TKEL_DelayTask(ms2tick(1000));
		 mTBOX_TRACE((kTBOX_NIV_WARNING, "Device is not mounted. Insert device to continue"));
	}


	filePtr = TDAL_FS_Open(fileName, kTDAL_FS_MODE_READ);

	TestManager_AssertTrue(filePtr != kTDAL_FS_INVALID_HANDLE, "tdal_fs_open");

	memset(&openParams, 0, sizeof(openParams));
	openParams.InputDataRead = mptest_InputDataRead;
	openParams.InputDataSeek = mptest_InputDataSeek;
	openParams.InputDataLength = mptest_InputDataLength;

	TestManager_AssertEqual(TKEL_Malloc(4*sizeof(char), &openParams.fileExtension),
			TKEL_NO_ERR,
			"tkel malloc");

	strcpy(openParams.fileExtension, aMediaExtensions[type]);

	TestManager_AssertEqual(TDAL_MP_Open(&openParams, &mpHandle),
			eTDAL_MP_NO_ERROR,
			"mp open");

	tTDAL_MP_MetaDataAudio audioMetaData;
	tTDAL_MP_Error metadataError;

	metadataError = TDAL_MP_AudioGetMetadata(mpHandle, &audioMetaData);
	TestManager_AssertEqual(metadataError, eTDAL_MP_NO_ERROR, "TDAL_MP_AudioGetMetadata");

	if(metadataError != eTDAL_MP_NO_ERROR)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not get metadata!"));
		pMP_TEST_Term();
		mTBOX_RETURN;	
	}
	
	if(!audioMetaData.bIsSupported)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "This media file is not supported!"));
		pMP_TEST_Term();
		mTBOX_RETURN;
	}

	startParams.iSpeed = 100;
	startParams.uiTime = 0;


	TestManager_AssertEqual(TDAL_MP_Start(mpHandle, &startParams),
			eTDAL_MP_NO_ERROR,
			"mp start" );

	TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
			eTDAL_OUTPUT_NO_ERROR,
			"1st output audio enable" );

	TestManager_AskForChecking("Is audio playback started?");


	int32_t speed = 100;
	while(speed <= 800)
	{
		TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, speed),
			eTDAL_MP_NO_ERROR,
			"mp speed set");
		speed = speed * 2;
		TKEL_DelayTask(1100);
	}

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 100),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TestManager_AskForChecking("Is audio playback fast-forwarded?");

	speed = -200;
	while(speed >= -800)
	{
		TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, speed),
			eTDAL_MP_NO_ERROR,
			"mp speed set");
		speed = speed * 2;
		TKEL_DelayTask(1000);
	}

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 100),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TKEL_DelayTask(1000);

	TestManager_AskForChecking("Is audio playback backwarded?");

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 0),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TKEL_DelayTask(1000);

	TestManager_AskForChecking("Is playback paused?");

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 100),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TKEL_DelayTask(1000);

	TestManager_AssertEqual(TDAL_MP_Stop(mpHandle),
			eTDAL_MP_NO_ERROR,
			"mp stop");

	TestManager_AssertEqual(TDAL_MP_Close(mpHandle),
			eTDAL_MP_NO_ERROR,
			"mp stop");

	TestManager_AssertEqual(TDAL_FS_Close(filePtr),
			eTDAL_FS_NO_ERROR,
			"fs close");

	TestManager_AskForChecking("Is playback stopped?");

	pMP_TEST_Term();

	mTBOX_RETURN;
}

LOCAL void  pMP_TEST_MediaPlayerPlayVideoFile(enum eFileType type, char * fileName)
{
	tTDAL_MP_Handle mpHandle;
	tTDAL_MP_InitParams initParams;
	tTDAL_MP_OpenParams openParams;
	tTDAL_MP_StartParams startParams;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
	tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
	tTDAL_DISP_LayerWindow  stInputWindow;
	tTDAL_DISP_LayerWindow  stOutputWindow;
	uint32_t                uiWidth;
	uint32_t                uiHeight;


	mTBOX_FCT_ENTER("pMP_TEST_MediaPlayerPlayVideoFile");

	pMP_TEST_Init();

    if(deviceMounted == false)
    {
        TestManager_LogMessage("Please insert USB");
    }

	while (deviceMounted == false)
	{
		TKEL_DelayTask(ms2tick(1000));
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Device is not mounted. Insert device to continue"));
	}


	filePtr = TDAL_FS_Open(fileName, kTDAL_FS_MODE_READ);

	TestManager_AssertTrue(filePtr != kTDAL_FS_INVALID_HANDLE, "tdal_fs_open");

    memset(&openParams, 0, sizeof(openParams));
    openParams.InputDataRead = mptest_InputDataRead;
    openParams.InputDataSeek = mptest_InputDataSeek;
    openParams.InputDataLength = mptest_InputDataLength;
    TestManager_AssertEqual(TKEL_Malloc(4*sizeof(char), &openParams.fileExtension),
            TKEL_NO_ERR,
            "tkel malloc");
    strcpy(openParams.fileExtension, aMediaExtensions[type]);

	TestManager_AssertEqual(TDAL_MP_Open(&openParams, &mpHandle),
	            eTDAL_MP_NO_ERROR,
	            "mp open");

	tTDAL_MP_MetaDataAv avMetadata;
	tTDAL_MP_Error metadataError;
	
	metadataError = TDAL_MP_AVGetMetadata(mpHandle, &avMetadata);
	TestManager_AssertEqual( metadataError, eTDAL_MP_NO_ERROR, "TDAL_MP_AVGetMetadata");

	if(metadataError != eTDAL_MP_NO_ERROR)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Could not get metadata!"));
		pMP_TEST_Term();
		mTBOX_RETURN;	
	}
	
	if(!avMetadata.Audio.bIsSupported || !avMetadata.Video.bIsSupported)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "This media file is not supported!\n"));
		pMP_TEST_Term();
		mTBOX_RETURN;
	}

	startParams.iSpeed = 100;
	startParams.uiTime = 0;


	TestManager_AssertEqual(TDAL_MP_Start(mpHandle, &startParams),
			eTDAL_MP_NO_ERROR,
			"mp start" );

	TestManager_AskForChecking("Is video playback started?");

	int32_t speed = 100;
	while(speed <= 800)
	{
		TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, speed),
			eTDAL_MP_NO_ERROR,
			"mp speed set");
		speed = speed * 2;
		TKEL_DelayTask(1100);
	}

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 100),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TestManager_AskForChecking("Is video playback fast-forwarded?");

	speed = -200;
	while(speed >= -800)
	{
		TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, speed),
			eTDAL_MP_NO_ERROR,
			"mp speed set");
		speed = speed * 2;
		TKEL_DelayTask(1000);
	}

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 100),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TKEL_DelayTask(1000);

	TestManager_AskForChecking("Is video playback backwarded?");

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 0),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TestManager_AskForChecking("Is playback paused?");

	TestManager_AssertEqual(TDAL_MP_SpeedSet(mpHandle, 100),
			eTDAL_MP_NO_ERROR,
			"mp speed set");

	TKEL_DelayTask(1000);

	TestManager_AssertEqual(TDAL_MP_Stop(mpHandle),
			eTDAL_MP_NO_ERROR,
			"mp stop");

	TestManager_AssertEqual(TDAL_MP_Close(mpHandle),
			eTDAL_MP_NO_ERROR,
			"mp stop");

	TestManager_AssertEqual(TDAL_FS_Close(filePtr),
			eTDAL_FS_NO_ERROR,
			"fs close");


	TestManager_AskForChecking("Is playback stopped?");

	pMP_TEST_Term();

	mTBOX_RETURN;
}

LOCAL void pMP_TEST_Init()
{
	tTDAL_MP_InitParams initParams;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAttenuation;
	tTDAL_OUTPUT_AudioAnaCapability stAudioCapability;
	tTDAL_DISP_LayerWindow  stInputWindow;
	tTDAL_DISP_LayerWindow  stOutputWindow;
	uint32_t                uiWidth;
	uint32_t                uiHeight;


	TestManager_AssertEqual(TDAL_GFX_Init(),
			eTDAL_DISP_NO_ERROR,
			"fgx init");


	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
			eTDAL_DISP_NO_ERROR,
			"output init");

#ifdef TDAL_HDMI_USE
	TestManager_AssertEqual(TDAL_HDMI_Init(),
			eTDAL_HDMI_NO_ERROR,
			"hdmi init" );
#endif

	TestManager_AssertEqual(TDAL_DISP_Init(),
			eTDAL_DISP_NO_ERROR,
			"disp init" );


	TestManager_AssertEqual(TDAL_FS_Init(),
			eTDAL_FS_NO_ERROR,
			"fs_init");

	TestManager_AssertEqual(TDAL_AV_Init(), eTDAL_AV_NO_ERROR, "av init");

	TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0,
			eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
			eTDAL_OUTPUT_NO_ERROR,
			"output denc STD set");

	TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
			eTDAL_OUTPUT_NO_ERROR,
			"output denc video enable" );

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
			"1st output audio enable" );

	stInputWindow.Left       = 0;
	stInputWindow.Top        = 0;
	stInputWindow.Right      = 720;
	stInputWindow.Bottom     = 576;

	TestManager_AssertEqual(TDAL_DISP_BlenderDimensionsGet(eTDAL_DISP_BLENDER_ID_0, &uiWidth, &uiHeight),
			eTDAL_DISP_NO_ERROR,
			"disp blender dimensions get" );

	stOutputWindow.Left      = 0;
	stOutputWindow.Top       = 0;
	stOutputWindow.Right     = uiWidth;
	stOutputWindow.Bottom    = uiHeight;

	TestManager_AssertEqual(TDAL_DISP_LayerIOWindowsSet(eTDAL_DISP_LAYER_VIDEO_ID_0,
			&stInputWindow,
			&stOutputWindow),
			eTDAL_DISP_NO_ERROR,
			"disp window size set" );

	TestManager_AssertEqual(TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
			eTDAL_DISP_NO_ERROR,
			"1st disp layer video enable" );

	TestManager_AssertEqual(TDAL_MP_Init(&initParams),
			eTDAL_MP_NO_ERROR,
			"mp init");

	TestManager_AssertEqual(TDAL_FS_RegisterDeviceCallback(FS_DeviceCallback),
			eTDAL_FS_NO_ERROR,
			"fs_register device callback");
}

LOCAL void pMP_TEST_Term()
{

	TestManager_AssertEqual(TDAL_MP_Terminate(),
			eTDAL_MP_NO_ERROR,
			"mp term");

	TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av term");

	TestManager_AssertEqual(TDAL_FS_Term(),
			eTDAL_FS_NO_ERROR,
			"fs_term");

	TestManager_AssertEqual(TDAL_DISP_Term(),
			eTDAL_DISP_NO_ERROR,
			"disp term" );

#ifdef TDAL_HDMI_USE
	TestManager_AssertEqual(TDAL_HDMI_Term(),
			eTDAL_HDMI_NO_ERROR,
			"hdmi term" );
#endif
	TestManager_AssertEqual(TDAL_OUTPUT_Term(),
			eTDAL_DISP_NO_ERROR,
			"output term");

	TestManager_AssertEqual(TDAL_GFX_Terminate(),
			eTDAL_DISP_NO_ERROR,
			"fgx init");
}

