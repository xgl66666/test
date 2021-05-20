/** \file ts_test_sce.c
 *  \brief Scenario Tests for the TS layer of the TDAL API
 *  \author Arnaud Viala
 *  \date 26/02/07
 *  \version 1.0
 */

/* The test is meaningless if you don't use TDAL_TS */

/******************************************************
 *              Includes                              *
 ******************************************************/
#include <string.h>
#include <stdio.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_ts.h"

#include "testhelper.h"
#include "TestManager.h"
#include "tstests.h"

/* Needed TDAL modules */
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"
#include "tdal_avs.h"
#include "tdal_dmd.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif
#if defined(USE_TDAL_FS)
#include "tdal_fs.h"
#endif

/*! \include testhelper.h */


/******************************************************
 *              TS test tree                         *
 ******************************************************/
void    TestSce_Live( void );
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
void    TestSce_GammaLive_Tuner1( void );
void    TestSce_GammaLive_Tuner2( void );
void    TestSce_GammaLive_Tuner1WithZap( void );
void    TestSce_GammaLive_Tuner2WithZap( void );
void    TestSce_GammaLive_Tuner1WithEIT( void );
void    TestSce_GammaLive_Tuner2WithEIT( void );

#if defined(USE_TDAL_FS)
void    TestSce_Record_Tuner1( void );
void    TestSce_Record_Tuner2( void );
void    TestSce_Playback( void );
void    TestSce_PlaybackWithSpeedChange( void );
void    TestSce_GammaLiveRecord_Tuner1( void );
void    TestSce_GammaLiveRecord_Tuner2( void );
void	TestSce_GammaLiveRecord_2On1_Tuner1( void );
void    TestSce_Playback2( void );
#endif
#endif /* PACKAGE_PVR */

static tTestNode gTestScenario_Live = {
	"TestScenario_Live",
	"Live scenario",
	0,
	TestSce_Live,
	NULL
};

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
static tTestNode gTestScenario_GammaLive_Tuner1 = {
	"TestScenario_GammaLive_Tuner1",
	"GammaLive_Tuner1 scenario",
	0,
	TestSce_GammaLive_Tuner1,
	NULL
};

static tTestNode gTestScenario_GammaLive_Tuner2 = {
	"TestScenario_GammaLive_Tuner2",
	"GammaLive_Tuner2 scenario",
	0,
	TestSce_GammaLive_Tuner2,
	NULL
};

static tTestNode gTestScenario_GammaLive_Tuner1WithZap = {
	"TestScenario_GammaLive_Tuner1WithZap",
	"GammaLive_Tuner1WithZap scenario",
	0,
	TestSce_GammaLive_Tuner1WithZap,
	NULL
};

static tTestNode gTestScenario_GammaLive_Tuner2WithZap = {
	"TestScenario_GammaLive_Tuner2WithZap",
	"GammaLive_Tuner2WithZap scenario",
	0,
	TestSce_GammaLive_Tuner2WithZap,
	NULL
};

static tTestNode gTestScenario_GammaLive_Tuner1WithEIT = {
	"TestScenario_GammaLive_Tuner1WithEIT",
	"GammaLive_Tuner1WithEIT scenario",
	0,
	TestSce_GammaLive_Tuner1WithEIT,
	NULL
};

static tTestNode gTestScenario_GammaLive_Tuner2WithEIT = {
	"TestScenario_GammaLive_Tuner2WithEIT",
	"GammaLive_Tuner2WithEIT scenario",
	0,
	TestSce_GammaLive_Tuner2WithEIT,
	NULL
};

#if defined(USE_TDAL_FS)

static tTestNode gTestScenario_Record_Tuner1 = {
	"TestScenario_Record_Tuner1",
	"Record_Tuner1 scenario",
	0,
	TestSce_Record_Tuner1,
	NULL
};

static tTestNode gTestScenario_Record_Tuner2 = {
	"TestScenario_Record_Tuner2",
	"Record_Tuner2 scenario",
	0,
	TestSce_Record_Tuner2,
	NULL
};

static tTestNode gTestScenario_Playback = {
	"TestScenario_Playback",
	"Playback scenario",
	0,
	TestSce_Playback,
	NULL
};

static tTestNode gTestScenario_PlaybackWithSpeedChange = {
	"TestScenario_PlaybackWithSpeedChange",
	"Playback with speed change scenario",
	0,
	TestSce_PlaybackWithSpeedChange,
	NULL
};

static tTestNode gTestScenario_GammaLiveRecord_Tuner1 = {
	"TestScenario_GammaLiveRecord_Tuner1",
	"GammaLiveRecord_Tuner1 scenario",
	0,
	TestSce_GammaLiveRecord_Tuner1,
	NULL
};

static tTestNode gTestScenario_GammaLiveRecord_Tuner2 = {
	"TestScenario_GammaLiveRecord_Tuner2",
	"GammaLiveRecord_Tuner2 scenario",
	0,
	TestSce_GammaLiveRecord_Tuner2,
	NULL
};

static tTestNode gTestScenario_GammaLiveRecord_2On1_Tuner1 = {
	"TestScenario_GammaLiveRecord_2On1_Tuner1",
	"GammaLiveRecord 2 on Tuner1 scenario",
	0,
	TestSce_GammaLiveRecord_2On1_Tuner1,
	NULL
};

static tTestNode gTestScenario_Playback2 = {
	"TestScenario_Playback2",
	"Playback2 scenario",
	0,
	TestSce_Playback2,
	NULL
};

#endif

#endif /* PACKAGE_PVR */


tTestNode* gTSScenarioTestNodeList[] = {
	&gTestScenario_Live,
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
    &gTestScenario_GammaLive_Tuner1,
#if (NUMBER_OF_DMD==2)
    &gTestScenario_GammaLive_Tuner2,
#endif
	&gTestScenario_GammaLive_Tuner1WithZap,
#if (NUMBER_OF_DMD==2)
	&gTestScenario_GammaLive_Tuner2WithZap,
#endif
#if defined(USE_TDAL_FS)
    &gTestScenario_Record_Tuner1,
#if (NUMBER_OF_DMD==2)
    &gTestScenario_Record_Tuner2,
#endif
    &gTestScenario_Playback,
    &gTestScenario_PlaybackWithSpeedChange,
    &gTestScenario_GammaLiveRecord_Tuner1,
#if (NUMBER_OF_DMD==2)
    &gTestScenario_GammaLiveRecord_Tuner2,
#endif
#endif
	&gTestScenario_GammaLive_Tuner1WithEIT,
#if (NUMBER_OF_DMD==2)
	&gTestScenario_GammaLive_Tuner2WithEIT,
#endif
#if defined(USE_TDAL_FS)
	&gTestScenario_GammaLiveRecord_2On1_Tuner1,
    &gTestScenario_Playback2,
#endif
#endif /* PACKAGE_PVR */
	NULL
};

tTestNode gTDAL_TS_ScenarioTestSuite = {
	"TDAL_TS Scenario Tests",
	"test TDAL_TS scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTSScenarioTestNodeList
};




/******************************************************
 *              Defines                               *
 ******************************************************/
#define TDAL_TS_INTERNAL_BUFFER_SIZE    (8192/*+2048*/)
#define TDAL_TS_INTERNAL_PACKET_SIZE    188


/******************************************************
 *              Enums / Typedefs / Structures         *
 ******************************************************/
enum { /* MUST be mask values */
    eDMD  = 1,
    eDMX  = 2,
    eDESC = 4,
    eAV   = 8,
    eAVS  = 16,
    eDISP   = 32,
    eOUTPUT = 64,
    eHDMI   = 128
};

IMPORT int gChannel;

typedef struct
{
    uint32_t    packet_position;
    uint32_t    packet_time_ms;
} tTestSce_Timestamp;

/******************************************************
 *              Local variables (LOCAL)               *
 ******************************************************/
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
#if defined(USE_TDAL_FS)
LOCAL   tTDAL_FS_File   TS_FileRecord;
LOCAL   char *          TS_FileRecord_name = "/hdd/chaltests_ts_recfile";
LOCAL   tTDAL_FS_File   TS_FileRecord2;
LOCAL   char *          TS_FileRecord_name2 = "/hdd/chaltests_ts_recfile2";
LOCAL   uint32_t        TS_FileSize;
LOCAL   uint32_t        TS_FileSize_warn = 0x100000;

LOCAL   tTDAL_FS_File   TS_FileTimestamp = kTDAL_FS_INVALID_HANDLE;
LOCAL   char *          TS_FileTimestamp_name = "/hdd/chaltests_timestamp_file";

#endif
#endif

LOCAL bool            	  TS_DMX_PLAY_is_configured = false;
LOCAL tTDAL_DMX_ChannelId TS_DMX_PLAY_VideoChannelId;
LOCAL tTDAL_DMX_ChannelId TS_DMX_PLAY_AudioChannelId;

LOCAL uint32_t            TS_DMX_PLAY_VideoStreamId;
LOCAL uint32_t            TS_DMX_PLAY_AudioStreamId;

LOCAL tTDAL_DISP_LayerId  TS_AV_VideoLayer[4] = {(-1), (-1), (-1), (-1)};
LOCAL uint8_t             TS_AV_NbVideoLayer = 0;

LOCAL tTDAL_DMX_ChannelId TS_DMX_PLAY_PcrChannelId;
#if 0
LOCAL tTDAL_DMX_FilterId  TS_DMX_PLAY_VideoFilterId;
LOCAL tTDAL_DMX_FilterId  TS_DMX_PLAY_AudioFilterId;
LOCAL tTDAL_DMX_FilterId  TS_DMX_PLAY_PcrFilterId;
#endif

#define TDAL_TS_TIMESTAMP_TABLE_SIZE 100

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL   uint32_t        	TS_PacketTime_warn = 0;

LOCAL tTestSce_Timestamp  TS_TimestampTable[TDAL_TS_TIMESTAMP_TABLE_SIZE];
LOCAL uint16_t            TS_TimestampIndex = 0;
LOCAL tTestSce_Timestamp *pTS_TimestampPlayback;
LOCAL uint32_t            TS_TimestampPlaybackNbElt = 0;
#endif

LOCAL int                 TS_gChannel2 = 0;

/******************************************************
 *              Functions declarations                *
 ******************************************************/
void    TestSce_GammaLive( int tuner, int zap, bool EITFilterON );

void    pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( uint32_t mask );
void    pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( uint32_t mask );
void    pTestSce_HDMI_Callback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);
void 	pTestSce_Configure_AV(tTDAL_AV_VideoType videoType,tTDAL_AV_AudioType audioType);
void    pTestSce_Unconfigure_AV( void );

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL uint32_t pTestSce_Source_Timestamp_CalculatePacketToSeek(uint32_t packet_origin,
															   int32_t  jump_ms);
#endif

void	pTestSce_SetChannel(void)
{
	gChannel = 0; /* SD */
	gChannel = 6; /* HD */
}

void	pTestSce_SetChannel2(void)
{
	TS_gChannel2 = 0; /* SD */
	TS_gChannel2 = 5; /* HD */
}

void pTestSce_TS_DMX_PLAY_Unconfigure( void )
{
    tTDAL_DMX_Error dmx_err;
    if (!TS_DMX_PLAY_is_configured)
    {
            return;
    }

    /* free audio channel */
    dmx_err = TDAL_DMX_Free_Channel(TS_DMX_PLAY_AudioChannelId);
    TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx free audio channel");

    /* free video channel */
    dmx_err = TDAL_DMX_Free_Channel(TS_DMX_PLAY_VideoChannelId);
    TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx free video channel");

    /* free pcr channel */
    dmx_err = TDAL_DMX_Free_Channel(TS_DMX_PLAY_PcrChannelId);
    TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx free pcr channel");

    TS_DMX_PLAY_is_configured = false;
}

LOCAL void pTestSce_TS_DMX_PLAY_Configure
(
    int      iDmxIdx,
    int      iDmxIdxPcr,
    uint16_t pid_video,
    uint16_t pid_audio,
    uint16_t pid_pcr
)
{
    tTDAL_DMX_Error         dmx_err;
    tTDAL_DMX_ChannelType_t VideoType = {
		eTDAL_DMX_VIDEO_STREAM,
		eTDAL_DMX_DECODER,
		eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_ChannelType_t AudioType = {
		eTDAL_DMX_AUDIO_STREAM,
		eTDAL_DMX_DECODER,
		eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_ChannelType_t PcrType = {
		eTDAL_DMX_PCR_STREAM,
		eTDAL_DMX_DECODER,
		eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_DemuxId       eDmxIds[eTDAL_DMX_NB_DEMUX] = {
        eTDAL_DMX_DEMUX0,
        eTDAL_DMX_DEMUX1,
        eTDAL_DMX_DEMUX2,
    };
	tTDAL_AV_Error          av_err;

	mTBOX_PRINT((kTBOX_LF, "Demux AV on DMX[%d]-PCR on DMX[%d] (max=%d)\n",
				 iDmxIdx,
				 iDmxIdxPcr,
				 (eTDAL_DMX_NB_DEMUX-1)));

	TestManager_AssertGreater((double)eTDAL_DMX_NB_DEMUX,
                              (double)iDmxIdx,
                              "Demux index");
    if(iDmxIdx > (eTDAL_DMX_NB_DEMUX-1))
    {
        return;
    }

    TestManager_AssertGreater((double)eTDAL_DMX_NB_DEMUX,
                              (double)iDmxIdxPcr,
                              "Demux PCR index");
    if(iDmxIdxPcr > (eTDAL_DMX_NB_DEMUX-1))
    {
        return;
    }

	if (TS_DMX_PLAY_is_configured)
    {
        mTBOX_PRINT((kTBOX_LF, "Demux unconfigure\n"));
		pTestSce_TS_DMX_PLAY_Unconfigure();
    }

    /* setup video filtering */
    if (pid_video != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx],
											&VideoType,
											&TS_DMX_PLAY_VideoChannelId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate video channel" );
		dmx_err = TDAL_DMX_StreamHandleGet (TS_DMX_PLAY_VideoChannelId,
											&TS_DMX_PLAY_VideoStreamId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx stream handle get video" );

		av_err = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1,
										TS_DMX_PLAY_VideoStreamId );
        TestManager_AssertEqual(av_err,
								eTDAL_AV_NO_ERROR,
								"av input stream set video" );

        /* set pid on the video channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(TS_DMX_PLAY_VideoChannelId,
										   pid_video);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx set pid on video channel");
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = %d\n", pid_video));

#if 0
        /* allocate a filter on the video channel */
        dmx_err = TDAL_DMX_Allocate_Filter(TS_DMX_PLAY_VideoChannelId,
										   &TS_DMX_PLAY_VideoFilterId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate video filter");

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(TS_DMX_PLAY_VideoFilterId,
										  eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx control video filter" );
#endif
    }

    /* setup audio filtering */
	if (pid_audio != 0)
	{
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx],
											&AudioType,
											&TS_DMX_PLAY_AudioChannelId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate audio channel" );

		dmx_err = TDAL_DMX_StreamHandleGet (TS_DMX_PLAY_AudioChannelId,
											&TS_DMX_PLAY_AudioStreamId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx stream handle get audio" );

		av_err = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1,
										TS_DMX_PLAY_AudioStreamId );
        TestManager_AssertEqual(av_err,
								eTDAL_AV_NO_ERROR,
								"av input stream set audio" );
        /* set pid on the Audio channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(TS_DMX_PLAY_AudioChannelId,
										   pid_audio);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx set pid audio channel");
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = %d\n", pid_audio));

#if 0
        /* allocate a filter on the Audio channel */
        dmx_err = TDAL_DMX_Allocate_Filter(TS_DMX_PLAY_AudioChannelId,
										   &TS_DMX_PLAY_AudioFilterId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate audio filter" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(TS_DMX_PLAY_AudioFilterId,
										  eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx control enable audio filter");
#endif
  }

    /* setup pcr filtering */
    if (pid_pcr != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdxPcr],
											&PcrType, &TS_DMX_PLAY_PcrChannelId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate channel");

        /* set pid on the Pcr channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(TS_DMX_PLAY_PcrChannelId,
										   pid_pcr);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx set channel pcr pid");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = %d\n", pid_pcr));

#if 0
        /* allocate a filter on the Pcr channel */
        dmx_err = TDAL_DMX_Allocate_Filter(TS_DMX_PLAY_PcrChannelId,
										   &TS_DMX_PLAY_PcrFilterId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate pcr filter" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(TS_DMX_PLAY_PcrFilterId,
										  eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx enable dmx pcr filter" );
#endif
    }

    TS_DMX_PLAY_is_configured = true;
}

void pTestSce_TS_DMX_PLAY_Start( void )
{
    tTDAL_DMX_Error dmx_err;

	if (!TS_DMX_PLAY_is_configured)
    {
		return;
    }

	/* start the video channel */
	dmx_err = TDAL_DMX_Control_Channel(TS_DMX_PLAY_VideoChannelId,
									   eTDAL_DMX_CTRL_ENABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control video channel" );

	/* start the PCR channel */
	dmx_err = TDAL_DMX_Control_Channel(TS_DMX_PLAY_PcrChannelId,
									   eTDAL_DMX_CTRL_ENABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx enable dmx pcr channel");

	/* start the audio channel */
	dmx_err = TDAL_DMX_Control_Channel(TS_DMX_PLAY_AudioChannelId,
									   eTDAL_DMX_CTRL_ENABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control enable audio channel");

}

void pTestSce_TS_DMX_PLAY_Stop( void )
{
    tTDAL_DMX_Error dmx_err;

    if (!TS_DMX_PLAY_is_configured)
    {
		return;
    }

	/* start the video channel */
	dmx_err = TDAL_DMX_Control_Channel(TS_DMX_PLAY_VideoChannelId,
									   eTDAL_DMX_CTRL_DISABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control disable video channel");

	/* start the audio channel */
	dmx_err = TDAL_DMX_Control_Channel(TS_DMX_PLAY_AudioChannelId,
									   eTDAL_DMX_CTRL_DISABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control disable audio channel");

	/* start the PCR channel */
	dmx_err = TDAL_DMX_Control_Channel(TS_DMX_PLAY_PcrChannelId,
									   eTDAL_DMX_CTRL_DISABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control disable pcr channel");

}

static TKEL_sema_id pTestSce_dmxTestSectionSem = (TKEL_sema_id)NULL;
static TKEL_uint32  pTestSce_dmxTestSectionToWait = 1;
static TKEL_uint32  pTestSce_dmxTestSectionRecv   = 0;
static TKEL_mutex_id pTestSce_dmxTestProtect = (TKEL_mutex_id)NULL;

#define MAX_EIT_SVC  10
#define MAX_EIT_TBID 4

static uint16_t pTestSce_dmxTestEitSvcId[MAX_EIT_SVC];
static uint32_t pTestSce_dmxTestEitSectNumberMask[MAX_EIT_SVC][MAX_EIT_TBID][8];

static void pTestSce_dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
												 tTDAL_DMX_FilterId FilterId,
												 uint32_t Filter_track,
												 uint32_t Length,
												 uint8_t *Buffer)
{
	TKEL_err tkError;
	tTestHelper_SectionHeader Header;
	uint32_t IndexMask;
	uint32_t BitMask;
	uint8_t svc, svcFound=0;
	bool bFound = FALSE;
	uint32_t IndexTbId;

	pTestSce_dmxTestSectionRecv++;
	
	TKEL_LockMutex(pTestSce_dmxTestProtect);
	
	/*
	mTBOX_PRINT((kTBOX_LF,"SEC[%d/%d]->ch=%d flt=%d size=%d data[0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X]\r\n",
				 pTestSce_dmxTestSectionRecv,
				 pTestSce_dmxTestSectionToWait,
				 ChannelId,
				 FilterId,
				 Length,
				 Buffer[0],Buffer[1],Buffer[2],Buffer[3],
				 Buffer[4],Buffer[5],Buffer[6],Buffer[7]));
	*/

	/* EIT tableIds */
	if((Buffer[0] == 0x50) ||
	   (Buffer[0] == 0x51) ||
	   (Buffer[0] == 0x60) ||
	   (Buffer[0] == 0x61))
	{
		TestHelper_DVBHeaderExtract(Buffer,  &Header);

		/* search if service already acquired */
		for(svc = 0; svc < MAX_EIT_SVC; svc++)
		{
			if(pTestSce_dmxTestEitSvcId[svc] == Header.StreamId)
			{
				svcFound = svc;
				bFound = TRUE;
			}
		}

		if(bFound == FALSE)
		{
			/* search empty space */
			for(svc = 0; svc < MAX_EIT_SVC; svc++)
			{
				if(pTestSce_dmxTestEitSvcId[svc] == 0)
				{
					svcFound = svc;
					bFound = TRUE;
				}
			}
		}

		if(bFound == TRUE)
		{
			pTestSce_dmxTestEitSvcId[svcFound] = Header.StreamId;

			IndexMask = Header.SectionNumber / 32;
			BitMask   = Header.SectionNumber % 32;
			IndexTbId = ((Buffer[0] & 0x20) >> 4) + (Buffer[0] & 0x01);

			/*
			mTBOX_PRINT((kTBOX_LF,"SEC[%d/%d]->svc[%d]=%d SN=0x%x EIT[idx=0x%x mask=0x%x]\r\n",
						 pTestSce_dmxTestSectionRecv,
						 pTestSce_dmxTestSectionToWait,
						 svcFound,
						 pTestSce_dmxTestEitSvcId[svcFound],	
						 Header.SectionNumber,
						 IndexMask,
						 BitMask));
			*/
			pTestSce_dmxTestEitSectNumberMask[svcFound][IndexTbId][IndexMask] |= (1 << BitMask);
		}
	}
	else
	{
		TestHelper_DVBBufferDecode(Buffer, TRUE);
	}
	
	TKEL_UnlockMutex(pTestSce_dmxTestProtect);

	/*  free the TDAL buffer  */
	/* tkError = TKEL_Free(Buffer); */
	if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
	{
		mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
	}
	
	if(pTestSce_dmxTestSectionRecv >= pTestSce_dmxTestSectionToWait)
	{
		tkError = TKEL_SignalSemaphore(pTestSce_dmxTestSectionSem);
		if (tkError != TKEL_NO_ERR)
		{
			mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tkError ));
		}
	}
	
}

void pTestSce_DMX_SectionTwoFilterSet( int demux )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId0;
	tTDAL_DMX_FilterId       filterId0;
	tTDAL_DMX_ChannelId      channelId1;
	tTDAL_DMX_FilterId       filterId1;
	uint8_t HardValue[2] = {0x00, 0x02}; /* That's the PAT/SDT table ID */
	uint8_t HardMask[2]  = {0xFF, 0xFF};
	uint8_t HardMode[2]  = {0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelCallbackFct callback;
    tTDAL_DMX_DemuxId       eDmxIds[eTDAL_DMX_NB_DEMUX] = {
        eTDAL_DMX_DEMUX0,
        eTDAL_DMX_DEMUX1,
        eTDAL_DMX_DEMUX2,
    };
	
	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&pTestSce_dmxTestSectionSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	TestManager_AssertEqual( TKEL_CreateMutex(&pTestSce_dmxTestProtect),
							TKEL_NO_ERR,
							"TKEL CreateMutex" );
	
	type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( eDmxIds[demux],
														&type,
														&channelId0),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel0" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( eDmxIds[demux],
														&type,
														&channelId1),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel1" );
	callback = pTestSce_dmxTestGenericSectionNotify;
	pTestSce_dmxTestSectionToWait = 5;
	pTestSce_dmxTestSectionRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId0,
																 callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel0 CallBack" );
	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId1,
																 callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel1 CallBack" );
	/* Allocate a filters  */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId0,
													   &filterId0 ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter0" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId1,
													   &filterId1 ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter1" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId0, 0x00 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel0 PID" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId1, 210/*0x11*/ ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel0 PID" );
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId0,
												  filterId0,
												  0,
												  1,
												  &HardValue[0],
												  &HardMask[0],
												  &HardMode[0],
												  eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter0" );
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId1,
												  filterId1,
												  0,
												  1,
												  &HardValue[1],
												  &HardMask[1],
												  &HardMode[1],
												  eTDAL_DMX_CRC_IGNORE ),
						kTDAL_DMX_NO_ERROR, "DMX set filter1" );

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId0,
													   eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel0" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId1,
													   eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel1" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId0,
													  eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter0" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId1,
													  eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter1" );
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( pTestSce_dmxTestSectionSem,
														(TKEL_tck)ms2tick(pTestSce_dmxTestSectionToWait * 1000) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId0,
													   eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel0" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId1,
													   eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel1" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId0 ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel0" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId1 ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel1" );

	TestManager_AssertEqual( TKEL_DeleteSemaphore(pTestSce_dmxTestSectionSem),
							 TKEL_NO_ERR,
							 "TKEL DeleteSemaphore" );
	TestManager_AssertEqual( TKEL_DeleteMutex(pTestSce_dmxTestProtect),
							 TKEL_NO_ERR,
							 "TKEL DeleteMutex" );
}

void pTestSce_DMX_SectionEITFilterSet( int demux )
{
	int                     i;
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelIdEIT;
	tTDAL_DMX_FilterId      filterIdEIT[5];
	uint16_t                PidEIT = 0x12;
	uint8_t                 HardValueEIT[5] = {0x50, 0x51, 0x60, 0x61, 0x48}; /* table ID */
	uint8_t                 HardMaskEIT[5]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	uint8_t                 HardModeEIT[5]  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelId     channelIdTDT;
	tTDAL_DMX_FilterId      filterIdTDT;
	uint16_t                PidTDT = 0x14;
	uint8_t HardValueTDT[1] = {0x70}; /* table ID */
	uint8_t HardMaskTDT[1]  = {0xFF};
	uint8_t HardModeTDT[1]  = {0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelCallbackFct callback;
    tTDAL_DMX_DemuxId       eDmxIds[eTDAL_DMX_NB_DEMUX] = {
        eTDAL_DMX_DEMUX0,
        eTDAL_DMX_DEMUX1,
        eTDAL_DMX_DEMUX2,
    };
	uint8_t svc, tbid ;

	callback = pTestSce_dmxTestGenericSectionNotify;
	pTestSce_dmxTestSectionToWait = 5000;
	pTestSce_dmxTestSectionRecv   = 0;

	for(svc = 0; svc < MAX_EIT_SVC; svc++)
	{
		pTestSce_dmxTestEitSvcId[svc] = 0;
		for(tbid = 0; tbid < MAX_EIT_TBID; tbid++)
		{
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][0] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][1] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][2] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][3] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][4] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][5] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][6] = 0x00000000;
			pTestSce_dmxTestEitSectNumberMask[svc][tbid][7] = 0x00000000;
		}
	}

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&pTestSce_dmxTestSectionSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	TestManager_AssertEqual( TKEL_CreateMutex(&pTestSce_dmxTestProtect),
							TKEL_NO_ERR,
							"TKEL CreateMutex" );
	
	type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	/* EIT */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( eDmxIds[demux],
														&type,
														&channelIdEIT),
							 kTDAL_DMX_NO_ERROR,
							 "DMX allocate channel EIT" );

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelIdEIT,
																 callback ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX register Channel EIT CallBack" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelIdEIT, PidEIT ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Set Channel EIT PID" );
	/* Allocate a filters  */
	for(i = 0; i < 5; i++)
	{
		TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelIdEIT,
														   &filterIdEIT[i] ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX allocate filterEIT" );
		TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelIdEIT,
													  filterIdEIT[i],
													  0,
													  1,
													  &HardValueEIT[i],
													  &HardMaskEIT[i],
													  &HardModeEIT[i],
													  eTDAL_DMX_CRC_IGNORE ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX set filterEIT" );
	}

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdEIT,
													   eTDAL_DMX_CTRL_ENABLE ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel EIT" );
	for(i = 0; i < 5; i++)
	{
		TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdEIT[i],
														  eTDAL_DMX_CTRL_ENABLE ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX control filter EIT" );
	}

	/* TDT */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( eDmxIds[demux],
														&type,
														&channelIdTDT),
							 kTDAL_DMX_NO_ERROR,
							 "DMX allocate channel TDT" );
	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelIdTDT,
																 callback ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX register Channel TDT CallBack" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelIdTDT,
													   &filterIdTDT ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX allocate filter TDT" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelIdTDT, PidTDT ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Set ChannelTDT PID" );
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelIdTDT,
												  filterIdTDT,
												  0,
												  1,
												  &HardValueTDT[0],
												  &HardMaskTDT[0],
												  &HardModeTDT[0],
												  eTDAL_DMX_CRC_IGNORE ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX set filter TDT" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdTDT,
													   eTDAL_DMX_CTRL_ENABLE ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel TDT" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdTDT,
													  eTDAL_DMX_CTRL_ENABLE ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control filter TDT" );
	
	
	
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( pTestSce_dmxTestSectionSem,
														(TKEL_tck)ms2tick(40 /*pTestSce_dmxTestSectionToWait*/ * 1000) ),
							 TKEL_NO_ERR,
							 "TKEL Wait Semaphore" );
	

	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelIdEIT,
													   eTDAL_DMX_CTRL_DISABLE),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel EIT" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelIdEIT ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Free Channel EIT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelIdTDT,
													   eTDAL_DMX_CTRL_DISABLE),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel TDT" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelIdTDT ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Free Channel TDT" );
	TestManager_AssertEqual( TKEL_DeleteMutex(pTestSce_dmxTestProtect),
							 TKEL_NO_ERR,
							 "TKEL DeleteMutex" );

	for(svc = 0; svc < MAX_EIT_SVC; svc++)
	{
		if(pTestSce_dmxTestEitSvcId[svc] != 0)
		{
			mTBOX_PRINT((kTBOX_LF,"[ SVC ][EIT ]=[31    28      24      20      16      12       8       4       0]\r\n"));
			for(tbid = 0; tbid < MAX_EIT_TBID; tbid++)
			{
				mTBOX_PRINT((kTBOX_LF,"[%05d][0x%x]=[%08x%08x%08x%08x%08x%08x%08x%08x]\r\n",
							 pTestSce_dmxTestEitSvcId[svc],HardValueEIT[tbid],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][7],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][6],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][5],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][4],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][3],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][2],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][1],
							 pTestSce_dmxTestEitSectNumberMask[svc][tbid][0]));
			}
		}
	}


}

/*! \fn void TestSce_Live( void )
 *  \brief play LIVE stream
 */
void TestSce_Live(void)
{
    tTDAL_TS_Object                         route_source;
    tTDAL_TS_Object                         route_dest;
/*
	tTDAL_AV_Decoder                        tdalAvDecoderVideo;
	tTDAL_AV_StreamType                     tdalAvStreamTypeVideo;
	tTDAL_AV_Decoder                        tdalAvDecoderAudio;
	tTDAL_AV_StreamType                     tdalAvStreamTypeAudio;
	tTDAL_OUTPUT_AudioAnaCapability         stAudioAnaCapability;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAnaSpeakerAttenuation;
*/
    struct TestManager_Channel *            pChannel;


    pChannel = TestHelper_GetChannel();

	/* Initialize tdal modules */
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute init" );

    TestHelper_Configure_TDAL_AVS();

#ifdef USE_TDAL_FS
	TestHelper_Configure_TDAL_FS();
#endif

	/* Configure TDAL_DMD and TDAL_DMX */
	TestHelper_DMDTune( 0 );
	
	pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
								   0, /* PCR demux */
								   pChannel->mVideoPID,
								   pChannel->mAudioPID,
								   pChannel->mPCRPID);

	/* Connecting Routes */
	route_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
                                         eTDAL_TS_TYPE_TUNER ,
                                         0 );
	route_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
                                         eTDAL_TS_TYPE_DEMUX ,
                                         0 );
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_source , route_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route" );



	/* Demux play start filtering */
	pTestSce_TS_DMX_PLAY_Start();

	/* Audio/Video Start/Enable SetVolume */
	pTestSce_Configure_AV(pChannel->videoType,
						  pChannel->audioType);

    TestManager_AskForChecking("do you see audio and video playing");

	/* Audio/Video Stop */
	pTestSce_Unconfigure_AV();

	/* Demux play stop filtering */
	pTestSce_TS_DMX_PLAY_Stop();


	/* Unconfigure DMX */
	pTestSce_TS_DMX_PLAY_Unconfigure();


	/* Disconnecting Routes */
	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_source , route_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route" );







	/* Terminate TDAL modules */
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
}


#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)

/*! \fn void TestSce_GammaLive_Tuner1( void )
 *  \brief play Gamma-LIVE stream using Tuner1
 */
void TestSce_GammaLive_Tuner1(void)
{
    TestSce_GammaLive( 0 , 0, FALSE);
}


/*! \fn void TestSce_GammaLive_Tuner2( void )
 *  \brief play Gamma-LIVE stream using Tuner2
 */
void TestSce_GammaLive_Tuner2(void)
{
    TestSce_GammaLive( 1 , 0, FALSE);
}

/*! \fn void TestSce_GammaLive_Tuner1WithZap( void )
 *  \brief play Gamma-LIVE stream using Tuner1
 */
void TestSce_GammaLive_Tuner1WithZap(void)
{
    TestSce_GammaLive( 0 , 10, FALSE);
}


/*! \fn void TestSce_GammaLive_Tuner2WithZap( void )
 *  \brief play Gamma-LIVE stream using Tuner2
 */
void TestSce_GammaLive_Tuner2WithZap(void)
{
    TestSce_GammaLive( 1 , 10, FALSE);
}

/*! \fn void TestSce_GammaLive_Tuner1WithEIT( void )
 *  \brief play Gamma-LIVE stream using Tuner1
 */
void TestSce_GammaLive_Tuner1WithEIT(void)
{
    TestSce_GammaLive( 0 , 0, TRUE);
}


/*! \fn void TestSce_GammaLive_Tuner2WithEIT( void )
 *  \brief play Gamma-LIVE stream using Tuner2
 */
void TestSce_GammaLive_Tuner2WithEIT(void)
{
    TestSce_GammaLive( 1 , 0, TRUE);
}

/*! \fn void TestSce_GammaLive( int tuner , int zap)
 *  \brief play Gamma-LIVE stream using "tuner" parameter
 *  \ and zap
 */
void TestSce_GammaLive( int tuner , int zap, bool EITFilterON)
{
    tTDAL_TS_Object     route_A_source;
    tTDAL_TS_Object     route_B_source;
    tTDAL_TS_Object     route_A_dest;
    tTDAL_TS_Object     route_B_dest;

    tTDAL_TSSOURCE_Handle       tssource_handle;
    tTDAL_TSSOURCE_OpenParam    tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam  tssource_cfgprm;

    tTDAL_TSSINK_Handle         tssink_handle;
    tTDAL_TSSINK_OpenParam      tssink_oprm;
    tTDAL_TSSINK_StartParam     tssink_sprm;

    struct TestManager_Channel *    pChannel;

	int demux;

	if(EITFilterON == TRUE)
	{
		gChannel = 3;
	}
	else
	{
		pTestSce_SetChannel();
	}

    pChannel = TestHelper_GetChannel();
	

	/* Initialize tdal modules */
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);

	/* Initialize TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource init" );
	TestManager_AssertEqual(TDAL_TSSINK_Init(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink init" );
	
	TestHelper_Configure_TDAL_AVS();
	
	/* Configure TDAL_DMD and TDAL_DMX */
	TestHelper_DMDTune( tuner );
	
	demux = tuner + 1;

	pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
								   demux, /* PCR demux */
								   pChannel->mVideoPID,
								   pChannel->mAudioPID,
								   pChannel->mPCRPID);
	
	/* Open/Configure/Start TSSource and TSSink */
	tssource_oprm.type = eTDAL_TSSOURCE_TYPE_DEMUX;
	tssource_oprm.buffer_length = TDAL_TS_INTERNAL_BUFFER_SIZE;
	tssource_oprm.packet_length = TDAL_TS_INTERNAL_PACKET_SIZE;

	TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle , &tssource_oprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource open source" );

    tssink_oprm.type = eTDAL_TSSINK_TYPE_DEMUX;
	/*tssink_oprm.source = */
	TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_handle , &tssink_oprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink open sink" );
	
	/* Connecting Routes */
	route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
										   eTDAL_TS_TYPE_TUNER ,
										   tuner );
	route_B_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
										   eTDAL_TS_TYPE_MEMORY ,
										   0 );
	route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
										   eTDAL_TS_TYPE_DEMUX ,
										   demux );
	route_B_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
										   eTDAL_TS_TYPE_DEMUX ,
										   0 );
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_A_source , route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route A" );
	
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_B_source , route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route B" );
	
	pTestSce_DMX_SectionTwoFilterSet(demux);
	
	tssource_cfgprm.u.dmx.demux = route_A_dest;
	TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle , &tssource_cfgprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource config source" );
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add AUD pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add VID pid" );
	if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
		 ( pChannel->mPCRPID != pChannel->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource add PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_handle , tssource_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink set source" );

	TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource start" );
	
	/* These flags are used only in ST5100 architecture */
	tssink_sprm.tv_not_radio = TRUE;
	tssink_sprm.initial_packet_number = 0;
	tssink_sprm.realtime     = TRUE;
	
	
	/* Demux play start filtering */
	pTestSce_TS_DMX_PLAY_Start();

	/* Audio/Video Start/Enable SetVolume */
	pTestSce_Configure_AV(pChannel->videoType,
						  pChannel->audioType);
	
	TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_handle , &tssink_sprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink start" );

    TestManager_AskForChecking("do you see audio and video playing");

	if(EITFilterON == TRUE)
	{
		pTestSce_DMX_SectionEITFilterSet(demux);
	}

	/* Audio/Video Stop */
	pTestSce_Unconfigure_AV();

	/* Demux play stop filtering */
	pTestSce_TS_DMX_PLAY_Stop();
	
	/* Stop/Close TSSource and TSSink */
	TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink stop" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource remove VID pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource remove AUD pid" );
	if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
		 ( pChannel->mPCRPID != pChannel->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource remove PCR pid" );
	}
	/* Disconnecting Routes */
	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source , route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route A" );

	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_B_source , route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route B" );


	/********************** START A ZAPPING ************************/
	while(zap > 0)
	{
		TestManager_AskForChecking("press to zap");
		
		gChannel += 1;
		gChannel %= 2;
		
		pChannel = TestHelper_GetChannel();
		
		
		TestManager_AssertEqual( TDAL_TSROUTE_Connect(route_A_source,
													  route_A_dest),
								 eTDAL_TSROUTE_NO_ERROR ,
								 "ZAP:tsroute connect route A" );

		TestManager_AssertEqual( TDAL_TSROUTE_Connect(route_B_source,
													  route_B_dest ) ,
								 eTDAL_TSROUTE_NO_ERROR ,
								 "ZAP:tsroute connect route B" );

		/* Configure TDAL_DMD and TDAL_DMX */
		TestHelper_DMDTune( tuner );

		
		pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
									   demux, /* PCR demux */
									   pChannel->mVideoPID,
									   pChannel->mAudioPID,
									   pChannel->mPCRPID);


		tssource_cfgprm.u.dmx.demux = route_A_dest;
		TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle , &tssource_cfgprm ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource config source" );
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mAudioPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource add AUD pid" );
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mVideoPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource add VID pid" );
		if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
			 ( pChannel->mPCRPID != pChannel->mAudioPID ) )
		{
			TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mPCRPID ),
									 eTDAL_TSSOURCE_NO_ERROR,
									 "ZAP:tssource add PCR pid" );
		}

		TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_handle , tssource_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "ZAP:tssink set source" );

		TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource start" );

		/* These flags are used only in ST5100 architecture */
		tssink_sprm.tv_not_radio = TRUE;
		tssink_sprm.initial_packet_number = 0;
		tssink_sprm.realtime     = TRUE;

		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_handle , &tssink_sprm ),
								 eTDAL_TSSINK_NO_ERROR,
								 "ZAP:tssink start" );

		/* Demux play start filtering */
		pTestSce_TS_DMX_PLAY_Start();

		/* Audio/Video Start/Enable SetVolume */
		pTestSce_Configure_AV(pChannel->videoType,
							  pChannel->audioType);
		TestManager_AskForChecking("ZAP:do you see audio and video playing");

		pTestSce_DMX_SectionTwoFilterSet(demux);
		
		TestManager_AskForChecking("ZAP:do you see still audio and video playing");
		/* Audio/Video Stop */
		pTestSce_Unconfigure_AV();

		/* Demux play stop filtering */
		pTestSce_TS_DMX_PLAY_Stop();

		/* Stop/Close TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "ZAP:tssink stop" );

		TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource stop" );

		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mVideoPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource remove VID pid" );
		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mAudioPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "ZAP:tssource remove AUD pid" );
		if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
			 ( pChannel->mPCRPID != pChannel->mAudioPID ) )
		{
			TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mPCRPID ),
									 eTDAL_TSSOURCE_NO_ERROR,
									 "ZAP:tssource remove PCR pid" );
		}

		pTestSce_DMX_SectionTwoFilterSet(demux);
		
		/* Disconnecting Routes */
		TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source , route_A_dest ) ,
								 eTDAL_TSROUTE_NO_ERROR ,
								 "ZAP:tsroute disconnect route A" );

		TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_B_source , route_B_dest ) ,
								 eTDAL_TSROUTE_NO_ERROR ,
								 "ZAP:tsroute disconnect route B" );
		zap--;
	} /* zap > 0 */
	/********************** STOP A ZAPPING ************************/

	TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink close" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource close source" );
	/* Unconfigure DMX */
	pTestSce_TS_DMX_PLAY_Unconfigure();


	/* Terminate TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSSINK_Term(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink terminate" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource terminate" );
	TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate" );
	
	/* Terminate TDAL modules */
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT  | eHDMI);
}

#if defined(USE_TDAL_FS)
void    pTestSce_SinkConsumerCallback_FileWriter(
            tTDAL_TSSINK_Handle     handle,
            void *                  client_tag,
            uint8_t *               address_1,
            uint32_t                nb_packets_max_1,
            uint8_t *               address_2,
            uint32_t                nb_packets_max_2
        )
{
    uint32_t            copied = 0;
    uint32_t            copied2 = 0;
    tTDAL_TSSINK_Error  tssink_err;
	tTDAL_FS_Size       written;
	tTDAL_FS_Size       written2;
	tTDAL_FS_File       FileRecord;

	/*mTBOX_PRINT((kTBOX_LF, "File write s1=[%d pckts %d bytes] s2=[%d pckts %d bytes)\n",
				 nb_packets_max_1,
				 (nb_packets_max_1 * TDAL_TS_INTERNAL_PACKET_SIZE),
				 nb_packets_max_2,
				 (nb_packets_max_2 * TDAL_TS_INTERNAL_PACKET_SIZE)));*/

	FileRecord = (tTDAL_FS_File)client_tag;

    if ( address_1 != NULL )
    {
        copied = nb_packets_max_1 * TDAL_TS_INTERNAL_PACKET_SIZE;
        written = TDAL_FS_Write( FileRecord , address_1 , copied );

		if(written != copied)
		{
			mTBOX_PRINT((kTBOX_LF, "File write1 s1=%d bytes written %d bytes\n",
						 copied,
						 written));
		}

        TS_FileSize += copied;
    }

    if ( address_2 != NULL )
    {
        copied2 = nb_packets_max_2 * TDAL_TS_INTERNAL_PACKET_SIZE;
        written2 = TDAL_FS_Write( FileRecord , address_2 , copied2 );
		
		if(written2 != copied2)
		{
			mTBOX_PRINT((kTBOX_LF, "File write2 s1=%d bytes written %d bytes\n",
						 copied2,
						 written2));
		}

		TS_FileSize += copied2;
    }

    tssink_err = TDAL_TSSINK_NotifyConsumption( handle ,
                                                nb_packets_max_1 + nb_packets_max_2 ,
                                                1024 );  /* TODO #define it */
    if ( tssink_err != eTDAL_TSSINK_NO_ERROR )
    {
        /* TODO: traces ! */
    }

    if ( TS_FileSize >= TS_FileSize_warn )
    {
        TS_FileSize_warn = 0x100000 + ( TS_FileSize & 0xFFF00000 );
        mTBOX_PRINT((kTBOX_LF, "File written [0x%x] total size->%d MB ...\n",
					 FileRecord,
                     TS_FileSize >> 20));
    }

}

void TestSce_Record( int tuner )
{

    tTDAL_TS_Object     route_A_source;
    tTDAL_TS_Object     route_A_dest;

    tTDAL_TSSOURCE_Handle       tssource_handle;
    tTDAL_TSSOURCE_OpenParam    tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam  tssource_cfgprm;

    tTDAL_TSSINK_Handle         tssink_handle;
    tTDAL_TSSINK_OpenParam      tssink_oprm;

    struct TestManager_Channel *    pChannel;

	pTestSce_SetChannel();

    pChannel = TestHelper_GetChannel();


   /** Initialize tdal modules */
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI );

   /** Initialize TDAL_TS modules */
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute init" );
    TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
                            eTDAL_TSSOURCE_NO_ERROR,
                            "tssource init" );
    TestManager_AssertEqual(TDAL_TSSINK_Init(),
                            eTDAL_TSSINK_NO_ERROR,
                            "tssink init" );

	/* done in TestHelper_Configure_TDAL_FS helper function
    TestManager_AssertEqual(TDAL_FS_Init(),
                            eTDAL_FS_NO_ERROR,
                            "fs init" );*/


    TestHelper_Configure_TDAL_AVS();

	TestHelper_Configure_TDAL_FS();

#if 1
    if(TDAL_FS_FileExist(TS_FileRecord_name) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TS_FileRecord_name ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs delete" );
	}
	else
	{
		mTBOX_PRINT((kTBOX_LF, "File %s does not exist\n",
					 TS_FileRecord_name));
	}

	TS_FileSize = 0;
    TS_FileSize_warn = 0x100000;
    TS_FileRecord = TDAL_FS_Open( TS_FileRecord_name ,
                                  kTDAL_FS_MODE_WRITE/* |
                                  kTDAL_FS_MODE_CREATE |
                                  kTDAL_FS_MODE_TRUNC*/ );
#else
	TS_FileRecord = 1;
#endif

    TestManager_AssertTrue( ( TS_FileRecord != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );

   /** Connecting Routes */
    route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
                                           eTDAL_TS_TYPE_TUNER ,
                                           tuner );
    route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
                                           eTDAL_TS_TYPE_DEMUX ,
                                           1 );

    TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_A_source , route_A_dest ) ,
                             eTDAL_TSROUTE_NO_ERROR ,
                             "tsroute connect route A" );


   /** Configure TDAL_DMD and TDAL_DMX */
    TestHelper_DMDTune( tuner );

   /** Open/Configure TSSource and TSSink */
    tssource_oprm.type = eTDAL_TSSOURCE_TYPE_DEMUX;
    tssource_oprm.buffer_length = TDAL_TS_INTERNAL_BUFFER_SIZE;
    tssource_oprm.packet_length = TDAL_TS_INTERNAL_PACKET_SIZE;
    TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle , &tssource_oprm ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource open source" );
    tssink_oprm.type = eTDAL_TSSINK_TYPE_EXTERN;
    tssink_oprm.notification_level = 1024; /* TODO #define it */
    TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_handle , &tssink_oprm ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink open sink" );

	pTestSce_DMX_SectionTwoFilterSet(1);

    tssource_cfgprm.u.dmx.demux = route_A_dest;
    TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle , &tssource_cfgprm ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource config source" );
	mTBOX_PRINT((kTBOX_LF, "recorded pid on audio = %d\n", pChannel->mAudioPID));
    TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mAudioPID ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource add AUD pid" );
	mTBOX_PRINT((kTBOX_LF, "recorded pid on video = %d\n", pChannel->mVideoPID));
    TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mVideoPID ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource add VID pid" );
    if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
         ( pChannel->mPCRPID != pChannel->mAudioPID ) )
    {
		mTBOX_PRINT((kTBOX_LF, "recorded pid on video = %d\n", pChannel->mPCRPID));
        TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , pChannel->mPCRPID ),
                                 eTDAL_TSSOURCE_NO_ERROR,
                                 "tssource add PCR pid" );
    }
    TestManager_AssertEqual( TDAL_TSSINK_SetConsumerCallback( tssink_handle ,
															  pTestSce_SinkConsumerCallback_FileWriter,
															  (void *)TS_FileRecord ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink set consumer cbk" );

    TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_handle , tssource_handle ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink set source" );
#if 0
	TestHelper_Configure_TDAL_FS();

    if(TDAL_FS_FileExist(TS_FileRecord_name) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TS_FileRecord_name ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs delete" );
	}


	TS_FileSize = 0;
    TS_FileSize_warn = 0x100000;
    TS_FileRecord = TDAL_FS_Open( TS_FileRecord_name ,
                                  kTDAL_FS_MODE_WRITE/* |
                                  kTDAL_FS_MODE_CREATE |
                                  kTDAL_FS_MODE_TRUNC*/ );

    TestManager_AssertTrue( ( TS_FileRecord != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );
#endif

	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{
		/** Start TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource start" );

		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_handle , NULL ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink start" );
		
		TestManager_AskForChecking("Press '1' when you want to stop recording");
#if 0
		while( TS_AbortRecord == FALSE )
		{
			/* TODO: gerer un timeout (60s) qui annule le test si ... */
			TKEL_DelayTask( ms2tick(500) );
		}
#endif
		/** Stop/Close TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink stop" );

        TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource stop" );
	}

	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mVideoPID ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource remove VID pid" );
    TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mAudioPID ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource remove AUD pid" );
    if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
         ( pChannel->mPCRPID != pChannel->mAudioPID ) )
    {
        TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle , pChannel->mPCRPID ),
                                 eTDAL_TSSOURCE_NO_ERROR,
                                 "tssource remove PCR pid" );
    }

    TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_handle ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink close" );
    TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource close source" );

	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{
		TestManager_AssertEqual( TDAL_FS_Close( TS_FileRecord ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs close" );
	}
#if 0
    {
        FILE * fd;

        fd = fopen( "/media/hdd/chaltest_dump_record", "w" );
        TestManager_AssertGreater( (uint32_t)fd , 0 , "opening dump file" );
        if ( fd != NULL )
        {
            mTBOX_PRINT((kTBOX_LF, "copying TS Data ...\n"));
            write( fd , TS_Buffer , TS_BufferOffset );
            mTBOX_PRINT((kTBOX_LF, "TS Data copied ...\n"));
            close( fd );
        }
    }
#endif

	/** Disconnecting Routes */
    TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source , route_A_dest ) ,
                             eTDAL_TSROUTE_NO_ERROR ,
                             "tsroute disconnect route A" );


	TestHelper_Unconfigure_TDAL_FS();

   /** Terminate TDAL_TS modules */
    TestManager_AssertEqual(TDAL_TSSINK_Term(),
                            eTDAL_TSSINK_NO_ERROR,
                            "tssink terminate" );
    TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
                            eTDAL_TSSOURCE_NO_ERROR,
                            "tssource terminate" );
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute terminate" );


   /** Terminate TDAL modules */
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT  | eHDMI);
}

void TestSce_Record_Tuner1(void)
{
    TestSce_Record( 0 );
}

void TestSce_Record_Tuner2(void)
{
    TestSce_Record( 1 );
}

/* declared above
LOCAL   tTDAL_FS_File   TS_FileRecord;
LOCAL   char *          TS_FileRecord_name = "/media/hdd/chaltests_ts_recfile";

LOCAL   uint32_t        TS_FileSize;
*/
LOCAL   bool            TS_FileReader_juststarted = TRUE;
LOCAL   TKEL_tck        TS_FileReader_tickPrev = 0;
LOCAL   TKEL_tck        TS_FileReader_tickCurr = 0;
void    pTestSce_SourceProducerCallback_FileReader(
            tTDAL_TSSOURCE_Handle   handle,
            uint8_t *               address_1,
            uint32_t                nb_packets_max_1,
            uint8_t *               address_2,
            uint32_t                nb_packets_max_2,
			void *                  p_private_data
        )
{
    uint32_t            copied = 0;
    uint32_t            copied2 = 0;
    uint32_t            read = 0;
    uint32_t            read2 = 0;
    tTDAL_TSSOURCE_Error  tssource_err;

	TKEL_GetTick(&TS_FileReader_tickCurr);

	if(TS_FileReader_tickPrev == 0)
	{
		TS_FileReader_tickPrev = TS_FileReader_tickCurr;
	}

	/*mTBOX_PRINT((kTBOX_LF, "%d-File read size->%d-%d\n",
				 (TS_FileReader_tickCurr - TS_FileReader_tickPrev),
				 nb_packets_max_1,
				 nb_packets_max_2));*/

	TS_FileReader_tickPrev = TS_FileReader_tickCurr;

	if ( address_1 != NULL )
    {
        if ( nb_packets_max_1 < 1024 /*1024*/ )
            copied = nb_packets_max_1 * TDAL_TS_INTERNAL_PACKET_SIZE;
        else
            copied = 1024 /*1024*/ * TDAL_TS_INTERNAL_PACKET_SIZE;
        read = TDAL_FS_Read( TS_FileRecord , address_1 , copied );
		if(read != copied)
		{
			mTBOX_PRINT((kTBOX_LF, "File Read1 s1=%d bytes read %d bytes\n",
						 copied,
						 read));
		}
        TS_FileSize += read;
    }

    #if 0
    if ( ( address_2 != NULL ) && ( read == copied ) )
    {
        copied2 = nb_packets_max_2 * TDAL_TS_INTERNAL_PACKET_SIZE;
        read2 = TDAL_FS_Read( TS_FileRecord , address_2 , copied2 );
        TS_FileSize += read2;
    }
    #endif

    if ( (read + read2) > 0 )
    {
        tssource_err = TDAL_TSSOURCE_NotifyProduction( handle ,
                                                       (read + read2)/188 ,
                                                       1024 );  /* TODO #define it */
        if ( tssource_err != eTDAL_TSSOURCE_NO_ERROR )
        {
            /* TODO: traces ! */
        }
    }
	else
	{
		#if 0
        tssource_err = TDAL_TSSOURCE_NotifyProduction( handle , 0, 1024 );
        if ( tssource_err != eTDAL_TSSOURCE_NO_ERROR )
        {
            /* TODO: traces ! */
        }
		#endif
	}

    /* This sleep should not be done here. It is necessary so the producer
     * won't produce too much data before the sink is started.
     * Such thing should be done inside TDAL_TSSOURCE module */
    if ( TS_FileReader_juststarted == TRUE )
    {
        TS_FileReader_juststarted = FALSE;
        TKEL_DelayTask( ms2tick(100/*500*/) );
    }

    if ( TS_FileSize >= TS_FileSize_warn )
    {
        TS_FileSize_warn = 0x100000 + ( TS_FileSize & 0xFFF00000 );
        mTBOX_PRINT((kTBOX_LF, "File read size->%d MB\n",
					 (TS_FileSize >> 20)));
    }

}

/****************************************************************
 * Function Name : pTestSce_Playback_CalculatePacketToSeek
 * Description :
 ******************************************************************/
LOCAL uint32_t pTestSce_Playback_CalculatePacketToSeek
(
	uint32_t PacketOrigin,
	int32_t  JumpInMs,
	uint32_t ByterateInBps
)
{
    uint32_t    packet_rate;
    int32_t     packet_to_jump;
    uint32_t    packet_to_seek = 0;

    /* divide the byterate by the packet_size is possible because
     * ByterateInBps >> 188 */
    packet_rate = ByterateInBps / 188;

    /* I think the maximum value of packet_rate that we can encounter is
     * around 50Mb/s -> 6,5MB/s -> 35000pkt/s
     * So JumpInMs/1000 should not exceed 53000 to not overflow the
     * int32_t packet_to_jump */
    if ( JumpInMs > 500000 || JumpInMs < -500000 )
    {
        packet_to_jump = ( JumpInMs / 1000 ) * packet_rate;
    }
    else
    {
        packet_to_jump = ( JumpInMs * (int32_t)packet_rate ) / 1000;
    }

    if ( ((int32_t)PacketOrigin) + packet_to_jump >= 0 )
    {
        packet_to_seek = (uint32_t)((int32_t)PacketOrigin + packet_to_jump);
    }

    mTBOX_PRINT((kTBOX_LF, "pTestSce_Playback_CalculatePacketToSeek:"
				 "orig=%d, jump=%d(ms), byterate=%d(bit/s) (pkt2jmp=%d) => seek=%d\n",
				 PacketOrigin,
				 JumpInMs,
				 ByterateInBps,
				 packet_to_jump,
				 packet_to_seek));

    return packet_to_seek;
}

/* callback function called to notify the external producer that a jump in soure stream is needed */
void pTestSce_Source_RequestJumpCallback_FileSeek
(
	tTDAL_TSSOURCE_Handle   handle,
	uint32_t     			packet_origin,
	int32_t      			jump_ms,
	uint32_t *   			p_new_packet_position,
	void *                  private_data
)
{
    tTDAL_FS_Error          tdalfs_err = 0;
    bool                    eof_reached = FALSE;
    uint32_t                packet_to_seek = 0;
    uint8_t                 search_result;
    tTDAL_TSSINK_Status     tssink_status;
    tTDAL_TSSINK_Error      tssink_err;
    bool                    use_timestamping_file = FALSE;
    tTDAL_FS_Size           tdalfs_size;
    tTDAL_TSSINK_Handle     tssink_inject_handle;    /* TSSINK injection */

	tssink_inject_handle = (tTDAL_TSSINK_Handle)private_data;

	if ( packet_origin == 0 && jump_ms == 0 )
	{
		packet_to_seek = 0;
	}
	else
	{
		/* check if packet position/time list available */
		if(pTS_TimestampPlayback != NULL)
		{
			packet_to_seek = pTestSce_Source_Timestamp_CalculatePacketToSeek(packet_origin,
																			 jump_ms);
		}
		else
		{
		tssink_err = TDAL_TSSINK_GetStatus( tssink_inject_handle ,
											&tssink_status );
		if ((tssink_err != eTDAL_TSSINK_NO_ERROR) ||
			(tssink_status.byterate_Bps == 0))
		{
                mTBOX_PRINT((kTBOX_LF,"RequestJumpCallback:Error with TDAL_TSSINK_GetStatus(). err=%d byterate=%d\n",
							 tssink_err,
							 tssink_status.byterate_Bps));
				packet_to_seek = packet_origin;
		}
		else
		{
			packet_to_seek = pTestSce_Playback_CalculatePacketToSeek(packet_origin ,
																	 jump_ms ,
																	 tssink_status.byterate_Bps );
		}
    }
    }
	
	/** Seek operation **/
	if ( eof_reached == FALSE )
    {
        tdalfs_size = packet_to_seek * 188;
        tdalfs_err = TDAL_FS_Seek( TS_FileRecord,
                                   tdalfs_size,
                                   eTDAL_FS_SEEK_ORIGIN_BEGIN );
        if (tdalfs_err != eTDAL_FS_NO_ERROR)
        {
            mTBOX_PRINT((kTBOX_LF,"RequestJumpCallback: ERROR Seek( pkt=%d ) failed (tdalfs_err=%d) \n",
						 packet_to_seek,
						 tdalfs_err));
        }
		else
		{
			mTBOX_PRINT((kTBOX_LF, "RequestJumpCallback:Seek(%d bytes/%d packets) rate(Bps)=%d\n",
						 tdalfs_size,
						 packet_to_seek,
						 tssink_status.byterate_Bps));
		}
    }

    *p_new_packet_position = packet_to_seek;

    TKEL_DelayTask( ms2tick(200) );

}

/* callback function called to notify client application of the TS timestamping */
void pTestSce_Source_TimestampingCallback
(
    tTDAL_TSSOURCE_Handle   handle,
    void *                  client_tag,
    uint32_t                packet_position,
    uint32_t                packet_time_ms
)
{
	tTDAL_FS_Size copy;
	tTDAL_FS_Size written;

	if(TS_TimestampIndex < TDAL_TS_TIMESTAMP_TABLE_SIZE)
	{
		TS_TimestampTable[TS_TimestampIndex].packet_position = packet_position;
		TS_TimestampTable[TS_TimestampIndex].packet_time_ms  = packet_time_ms;

		TS_TimestampIndex++;
	}

#if defined(USE_TDAL_FS)
	if((TS_FileTimestamp != kTDAL_FS_INVALID_HANDLE) &&
	   (TS_TimestampIndex >= TDAL_TS_TIMESTAMP_TABLE_SIZE))
	{
		TS_TimestampIndex = 0;

		copy = (TDAL_TS_TIMESTAMP_TABLE_SIZE * sizeof(tTestSce_Timestamp));

		written = TDAL_FS_Write(TS_FileTimestamp,
								&TS_TimestampTable[0],
								copy);
		if(written != copy)
		{
			mTBOX_PRINT((kTBOX_LF, "File timestamp write s=%d bytes written %d bytes\n",
						 copy,
						 written));
		}
		else
		{
			mTBOX_PRINT((kTBOX_LF, "File timestamp write s=%d bytes written %d bytes\n",
						 copy,
						 written));
		}
	}
#endif

    if ( (packet_time_ms / 1000) >= TS_PacketTime_warn )
    {
        TS_PacketTime_warn += 1;
		
		/*mTBOX_PRINT((kTBOX_LF,"TimestampingCallback:packet[pos=%d, time=%d ms]\n",
					 packet_position,
					 packet_time_ms));*/
    }
}

void pTestSce_Source_Timestamp_Configure(tTDAL_TSSOURCE_Handle tssource_handle)
{
	TS_PacketTime_warn = 0;

#if defined(USE_TDAL_FS)
    if(TDAL_FS_FileExist(TS_FileTimestamp_name) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TS_FileTimestamp_name ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs timestamp delete" );
	}

    TS_FileTimestamp = TDAL_FS_Open( TS_FileTimestamp_name ,
									 kTDAL_FS_MODE_WRITE/* |
									 kTDAL_FS_MODE_CREATE |
									 kTDAL_FS_MODE_TRUNC*/ );

    TestManager_AssertTrue( ( TS_FileTimestamp != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs timestamp file open" );

#endif

	if(TS_FileTimestamp != kTDAL_FS_INVALID_HANDLE)
	{
		TS_TimestampIndex = 0;

		TestManager_AssertEqual( TDAL_TSSOURCE_SetTimestampingCallback(tssource_handle,
																	   pTestSce_Source_TimestampingCallback,
																	   (void *)NULL),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource set timestamping callback" );
	}
}

void pTestSce_Source_Timestamp_Unconfigure(tTDAL_TSSOURCE_Handle tssource_handle)
{
	tTDAL_FS_Size copy;
	tTDAL_FS_Size written;

	TS_PacketTime_warn = 0;
	TestManager_AssertEqual( TDAL_TSSOURCE_SetTimestampingCallback(tssource_handle,
																   NULL,
																   (void *)NULL),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource reset timestamping callback" );
	
#if defined(USE_TDAL_FS)
	if(TS_FileTimestamp != kTDAL_FS_INVALID_HANDLE)
	{
		/* save current timestamp table in file */
		copy = (TS_TimestampIndex * sizeof(tTestSce_Timestamp));

		written = TDAL_FS_Write(TS_FileTimestamp,
								&TS_TimestampTable[0],
								copy);
		if(written != copy)
		{
			mTBOX_PRINT((kTBOX_LF, "File timestamp write s=%d bytes written %d bytes\n",
						 copy,
						 written));
		}
		else
		{
			mTBOX_PRINT((kTBOX_LF, "File timestamp write s=%d bytes written %d bytes\n",
						 copy,
						 written));
		}

		TestManager_AssertEqual( TDAL_FS_Close( TS_FileTimestamp ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs timestamp file close" );
	}
#endif
}

void pTestSce_Source_Timestamp_Get(void)
{
	tTestSce_Timestamp *pTS_Timestamp;
	uint32_t            index;
	TKEL_err            eTkelErr;
	tTDAL_FS_Size       read;

#if defined(USE_TDAL_FS)
	tTDAL_FS_Stat Stat;

    if(TDAL_FS_FileExist(TS_FileTimestamp_name) == eTDAL_FS_NO_ERROR)
	{
		TS_FileTimestamp = TDAL_FS_Open( TS_FileTimestamp_name ,
										 kTDAL_FS_MODE_READ );
	}

    TestManager_AssertTrue( ( TS_FileTimestamp != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs timestamp file open for read " );

	if(TS_FileTimestamp != kTDAL_FS_INVALID_HANDLE)
	{
		TestManager_AssertEqual( TDAL_FS_Stat( TS_FileTimestamp, &Stat ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs timestamp file close" );
		
		mTBOX_PRINT((kTBOX_LF, "File timestamp stat size=%d -> nbElement=%d\n",
					 Stat.Size,
					 Stat.Size / sizeof(tTestSce_Timestamp)));
		if(Stat.Size > 0)
		{
			/* allocate timestamp playback structure */
			eTkelErr = TKEL_Malloc(Stat.Size,
								   (void*)&pTS_TimestampPlayback);
			mTBOX_ASSERT(eTkelErr == TKEL_NO_ERR);

			if(pTS_TimestampPlayback != NULL)
			{
				read = TDAL_FS_Read( TS_FileTimestamp , pTS_TimestampPlayback , Stat.Size );
				if(read != Stat.Size)
				{
					mTBOX_PRINT((kTBOX_LF, "File Read timestamp s=%d bytes read %d bytes\n",
								 Stat.Size,
								 read));
				}

				TS_TimestampPlaybackNbElt = Stat.Size / sizeof(tTestSce_Timestamp);
				pTS_Timestamp             = pTS_TimestampPlayback;

				mTBOX_PRINT((kTBOX_LF,"TimestampingGet:0-packet[pos=%d, time=%d ms]\n",
							 pTS_Timestamp->packet_position,
							 pTS_Timestamp->packet_time_ms));
				
#if 0
				pTS_Timestamp++;
				for(index = 1; (index < TS_TimestampPlaybackNbElt); index++)
				{

					mTBOX_PRINT((kTBOX_LF,"TimestampingGet:%d-packet[pos=%d, time=%d ms] diff[pos=%d, time=%d ms]\n",
								 index,
								 pTS_Timestamp->packet_position,
								 pTS_Timestamp->packet_time_ms,
								 (pTS_Timestamp->packet_position - (pTS_Timestamp-1)->packet_position),
								 (pTS_Timestamp->packet_time_ms - (pTS_Timestamp-1)->packet_time_ms)));
					
					pTS_Timestamp++;
				}
#else
				pTS_Timestamp = &pTS_TimestampPlayback[(TS_TimestampPlaybackNbElt - 1)];
				mTBOX_PRINT((kTBOX_LF,"TimestampingGet:%d-packet[pos=%d, time=%d ms]\n",
							 (TS_TimestampPlaybackNbElt - 1),
							 pTS_Timestamp->packet_position,
							 pTS_Timestamp->packet_time_ms));
#endif
			}
			
		}
		
		TestManager_AssertEqual( TDAL_FS_Close( TS_FileTimestamp ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs timestamp file close" );
	}
#endif
}

void pTestSce_Source_Timestamp_Release(void)
{
	TKEL_err eTkelErr;
	
	if(pTS_TimestampPlayback != NULL)
	{
		/* free timestamp playback structure */
		eTkelErr = TKEL_Free((void*)pTS_TimestampPlayback);
		mTBOX_ASSERT(eTkelErr == TKEL_NO_ERR);
	}
}

/****************************************************************
 * Function Name : pTestSce_Source_Timestamp_CalculatePacketToSeek
 * Description :
 ******************************************************************/
LOCAL uint32_t pTestSce_Source_Timestamp_CalculatePacketToSeek
(
	uint32_t packet_origin,
	int32_t  jump_ms
)
{
	tTestSce_Timestamp     *pTS_Timestamp;
	tTestSce_Timestamp     *pTS_TimestampPrev;
	uint32_t                index;
	uint32_t				time_diff_us;
	uint32_t				pckt_time_us;
	uint32_t				time_ms;
	int32_t					time_jump_ms;
	uint32_t                packet_jump_pos;
    uint32_t                packet_to_seek = 0;
	
	pTS_Timestamp     = pTS_TimestampPlayback;
	
	/* search packet positions near packet_origin */
	for(index = 0; index < TS_TimestampPlaybackNbElt; index++)
	{
		if(packet_origin <= pTS_Timestamp->packet_position)
		{
			if(index > 1)
			{
				pTS_TimestampPrev = pTS_Timestamp - 1;

				/* keep this packet index */
				mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:%d-orig=%d pos=%d-%d time=%d-%d(ms) jump=%d(ms)\n",
							 index,
							 packet_origin,
							 pTS_TimestampPrev->packet_position,
							 pTS_Timestamp->packet_position,
							 pTS_TimestampPrev->packet_time_ms,
							 pTS_Timestamp->packet_time_ms,
							 jump_ms));

				time_diff_us = (pTS_Timestamp->packet_time_ms - pTS_TimestampPrev->packet_time_ms) * 1000;
				pckt_time_us = time_diff_us / (pTS_Timestamp->packet_position - pTS_TimestampPrev->packet_position);

				time_ms = pTS_TimestampPrev->packet_time_ms +
					(((packet_origin - pTS_TimestampPrev->packet_position) * pckt_time_us) / 1000);

				/* absolute time to jump is time of packet_orign position + jump_ms */
				time_jump_ms = time_ms + jump_ms;

				mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:%d-tdiff=%d(us) pck_time=%d(us)->%d(ms) jump=%d(ms)\n",
							 index,
							 time_diff_us,
							 pckt_time_us,
							 time_ms,
							 time_jump_ms));
				break;
			}
			else if(index == 0)
			{
				mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:%d-orig=%d pos=%d time=%d(ms) jump=%d(ms)\n",
							 index,
							 packet_origin,
							 pTS_Timestamp->packet_position,
							 pTS_Timestamp->packet_time_ms,
							 jump_ms));
				break;
			}
		}
		
		pTS_Timestamp++;
	}
	
	/* check if time to jump is outside the list */
	if(time_jump_ms < 0)
	{
		/* go to the end of packet position list minus 1024 packets */
		packet_to_seek = pTS_TimestampPlayback[TS_TimestampPlaybackNbElt-1].packet_position - (1024*24);
		mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:start of list->seek=%d(packet)end=%d(packet)\n",
					 packet_to_seek,
					 pTS_TimestampPlayback[TS_TimestampPlaybackNbElt-1].packet_position));
	}
	else if(time_jump_ms > pTS_TimestampPlayback[TS_TimestampPlaybackNbElt-1].packet_time_ms)
	{
		packet_to_seek = pTS_TimestampPlayback[TS_TimestampPlaybackNbElt-1].packet_position;
		
		mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:end of list-> seek=%d(packet)\n",
					 packet_to_seek));
	}
	else
	{
		pTS_Timestamp     = pTS_TimestampPlayback;
		
		/* search packets near the time to jump */
		for(index = 0; index < TS_TimestampPlaybackNbElt; index++)
		{
			if(time_jump_ms <= pTS_Timestamp->packet_time_ms)
			{
				pTS_TimestampPrev = pTS_Timestamp - 1;

				/* keep this packet index */
				mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:%d-pos=%d-%d time=%d-%d(ms) jump=%d(ms)\n",
							 index,
							 pTS_TimestampPrev->packet_position,
							 pTS_Timestamp->packet_position,
							 pTS_TimestampPrev->packet_time_ms,
							 pTS_Timestamp->packet_time_ms,
							 time_jump_ms));

				time_diff_us = (pTS_Timestamp->packet_time_ms - pTS_TimestampPrev->packet_time_ms) * 1000;
				pckt_time_us = time_diff_us / (pTS_Timestamp->packet_position - pTS_TimestampPrev->packet_position);

				packet_jump_pos = ((time_jump_ms - pTS_TimestampPrev->packet_time_ms) * 1000) / pckt_time_us;

				/* absolute packet calculation */
				packet_to_seek  = pTS_TimestampPrev->packet_position + packet_jump_pos;

				mTBOX_PRINT((kTBOX_LF,"Timestamp_CalculatePacketToSeek:%d-tdiff=%d(us) pck_time=%d(us)->jump=%d(packet) seek=%d(packet)\n",
							 index,
							 time_diff_us,
							 pckt_time_us,
							 packet_jump_pos,
							 packet_to_seek));
				break;
			}

			pTS_Timestamp++;
		}
	}

	return(packet_to_seek);
}

void TestSce_PlaybackChannel( int channel , char *p_file)
{
    int                         demux = 0;

	tTDAL_TS_Object             route_A_source;
    tTDAL_TS_Object             route_A_dest;

    tTDAL_TSSOURCE_Handle       tssource_handle;
    tTDAL_TSSOURCE_OpenParam    tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam  tssource_cfgprm;

    tTDAL_TSSINK_Handle         tssink_handle;
    tTDAL_TSSINK_OpenParam      tssink_oprm;
    tTDAL_TSSINK_StartParam     tssink_sprm;

    struct TestManager_Channel *    pChannel;

	TS_FileReader_tickPrev = 0;

    pChannel = TestHelper_GetChannelIdx(channel);


   /** Initialize tdal modules */
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);

   /** Initialize TDAL_TS modules */
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute init" );
    TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
                            eTDAL_TSSOURCE_NO_ERROR,
                            "tssource init" );
    TestManager_AssertEqual(TDAL_TSSINK_Init(),
                            eTDAL_TSSINK_NO_ERROR,
                            "tssink init" );

	/* done in TestHelper_Configure_TDAL_FS helper function
    TestManager_AssertEqual(TDAL_FS_Init(),
                            eTDAL_FS_NO_ERROR,
                            "fs init" );*/


    TestHelper_Configure_TDAL_AVS();


   /** Connecting Routes */
    route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
                                           eTDAL_TS_TYPE_MEMORY ,
                                           0 );
    route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
                                           eTDAL_TS_TYPE_DEMUX ,
                                           demux );

    TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_A_source , route_A_dest ) ,
                             eTDAL_TSROUTE_NO_ERROR ,
                             "tsroute connect route A" );


	pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
								   0, /* PCR demux */
								   pChannel->mVideoPID,
								   pChannel->mAudioPID,
								   pChannel->mPCRPID);


   /** Open/Configure TSSource and TSSink */
    tssource_oprm.type = eTDAL_TSSOURCE_TYPE_MEMORY;
    tssource_oprm.buffer_length = TDAL_TS_INTERNAL_BUFFER_SIZE;
    tssource_oprm.packet_length = TDAL_TS_INTERNAL_PACKET_SIZE;
    TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle , &tssource_oprm ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource open source" );

    tssink_oprm.type = eTDAL_TSSINK_TYPE_DEMUX;
    TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_handle , &tssink_oprm ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink open sink" );

    tssource_cfgprm.u.mem.producer_callback = pTestSce_SourceProducerCallback_FileReader;
	tssource_cfgprm.u.mem.request_jump_callback = pTestSce_Source_RequestJumpCallback_FileSeek;
	tssource_cfgprm.u.mem.private_data = tssink_handle;
    TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle , &tssource_cfgprm ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource config source" );

    TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_handle , tssource_handle ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink set source" );

	TestHelper_Configure_TDAL_FS();

    TS_FileSize = 0;
    TS_FileSize_warn = 0x100000;
    TS_FileReader_juststarted = TRUE;
    TS_FileRecord = TDAL_FS_Open( p_file,
                                  kTDAL_FS_MODE_READ );

    TestManager_AssertTrue( ( TS_FileRecord != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );


	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{
        /** Start TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource start" );
		
		/* These flags are used only in ST5100 architecture */
        tssink_sprm.tv_not_radio = TRUE;
		tssink_sprm.initial_packet_number = 0;
		tssink_sprm.realtime     = FALSE;
		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_handle , &tssink_sprm ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink start" );

		/* Demux play start filtering */
		pTestSce_TS_DMX_PLAY_Start();

		/* Audio/Video Start/Enable SetVolume */
		pTestSce_Configure_AV(pChannel->videoType,
							  pChannel->audioType);
	
#if 0
		TestManager_LogMessage("Playback should start. You can press some key :");
		TestManager_LogMessage(" O: Test OK, stop it         1: play 1X speed");
		TestManager_LogMessage(" K: Test OK, stop it         2: play 2X speed");
		
		TKEL_DelayTask( ms2tick(10000) );
	
		TestManager_AssertEqual( TDAL_AV_SpeedSet( eTDAL_AV_DECODING_VIDEO ,
												   200 ),
								 eTDAL_AV_NO_ERROR,
								 "av setspeed( 200 )");
#endif
		TestManager_AskForChecking("Press '1' when you want to stop playback");
	
#if 0
		char cAskResult = '1';
		while( ( cAskResult >= '0' ) && ( cAskResult >= '9' ) )
		{
			mTBOX_PRINT((kTBOX_LF, "O/K/1/2 ? : "));
			mTBOX_GETCHAR(( &cAskResult ));
			mTBOX_PRINT((kTBOX_LF,"%c\n", cAskResult));
	
			if ( cAskResult == '2' )
			{
				TestManager_AssertEqual( TDAL_AV_SpeedSet( eTDAL_AV_DECODING_VIDEO ,
														   200 ),
										 eTDAL_AV_NO_ERROR,
										 "av start audio");
			}
			/* TODO: gerer un timeout (60s) qui annule le test si ... */
		}
		TestManager_AssertEqual( cAskResult,
								 'O',
								 "playback is visible" );
#endif
	
		/* Audio/Video Stop */
		pTestSce_Unconfigure_AV();
		
		/* Demux play stop filtering */
		pTestSce_TS_DMX_PLAY_Stop();
	
		/** Stop/Close TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink stop" );
	
		TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource stop" );
		
		TestManager_AssertEqual( TDAL_FS_Close( TS_FileRecord ) ,
                             eTDAL_FS_NO_ERROR ,
                             "fs close" );
	}
	else
	{
		TestManager_LogMessage("!! Before playback could start, a record mud be done !!\n");
	}

    TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_handle ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink close" );

    TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource close source" );

	/* Unconfigure DMX */
	pTestSce_TS_DMX_PLAY_Unconfigure();
	
    #if 0
    {
        FILE * fd;

        fd = fopen( "/media/hdd/chaltest_dump_record", "w" );
        TestManager_AssertGreater( (uint32_t)fd , 0 , "opening dump file" );
        if ( fd != NULL )
        {
            mTBOX_PRINT((kTBOX_LF, "copying TS Data ...\n"));
            write( fd , TS_Buffer , TS_BufferOffset );
            mTBOX_PRINT((kTBOX_LF, "TS Data copied ...\n"));
            close( fd );
        }
    }
    #endif



	
   /** Disconnecting Routes */
    TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source , route_A_dest ) ,
                             eTDAL_TSROUTE_NO_ERROR ,
                             "tsroute disconnect route A" );

	TestHelper_Unconfigure_TDAL_FS();

    /*TestManager_AssertEqual(TDAL_FS_Term(),
                            eTDAL_FS_NO_ERROR,
                            "fs terminate" );*/

   /** Terminate TDAL_TS modules */
    TestManager_AssertEqual(TDAL_TSSINK_Term(),
                            eTDAL_TSSINK_NO_ERROR,
                            "tssink terminate" );
    TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
                            eTDAL_TSSOURCE_NO_ERROR,
                            "tssource terminate" );
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute terminate" );


   /** Terminate TDAL modules */
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
}

void TestSce_Playback( void )
{
	pTestSce_SetChannel();

	TestSce_PlaybackChannel(gChannel, TS_FileRecord_name);
}

void TestSce_Playback2( void )
{
	pTestSce_SetChannel2();

	TestSce_PlaybackChannel(TS_gChannel2, TS_FileRecord_name2);
}

void TestSce_PlaybackWithSpeedChange( void )
{
    int                         demux = 0;

	tTDAL_TS_Object             route_A_source;
    tTDAL_TS_Object             route_A_dest;

    tTDAL_TSSOURCE_Handle       tssource_handle;
    tTDAL_TSSOURCE_OpenParam    tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam  tssource_cfgprm;

    tTDAL_TSSINK_Handle         tssink_handle;
    tTDAL_TSSINK_OpenParam      tssink_oprm;
    tTDAL_TSSINK_StartParam     tssink_sprm;

    struct TestManager_Channel *    pChannel;
	char cAskResult;
	int32_t speed = 100;

	TS_FileReader_tickPrev = 0;

	pTestSce_SetChannel();

    pChannel = TestHelper_GetChannel();


   /** Initialize tdal modules */
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);

   /** Initialize TDAL_TS modules */
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute init" );
    TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
                            eTDAL_TSSOURCE_NO_ERROR,
                            "tssource init" );
    TestManager_AssertEqual(TDAL_TSSINK_Init(),
                            eTDAL_TSSINK_NO_ERROR,
                            "tssink init" );

    TestHelper_Configure_TDAL_AVS();

   /** Connecting Routes */
    route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
                                           eTDAL_TS_TYPE_MEMORY ,
                                           0 );
    route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
                                           eTDAL_TS_TYPE_DEMUX ,
                                           demux );

    TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_A_source , route_A_dest ) ,
                             eTDAL_TSROUTE_NO_ERROR ,
                             "tsroute connect route A" );

	pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
								   0, /* PCR demux */
								   pChannel->mVideoPID,
								   pChannel->mAudioPID,
								   pChannel->mPCRPID);

   /** Open/Configure TSSource and TSSink */
    tssource_oprm.type = eTDAL_TSSOURCE_TYPE_MEMORY;
    tssource_oprm.buffer_length = TDAL_TS_INTERNAL_BUFFER_SIZE;
    tssource_oprm.packet_length = TDAL_TS_INTERNAL_PACKET_SIZE;
    TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle , &tssource_oprm ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource open source" );

    tssink_oprm.type = eTDAL_TSSINK_TYPE_DEMUX;
    TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_handle , &tssink_oprm ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink open sink" );

    tssource_cfgprm.u.mem.producer_callback = pTestSce_SourceProducerCallback_FileReader;
	tssource_cfgprm.u.mem.request_jump_callback = pTestSce_Source_RequestJumpCallback_FileSeek;
	tssource_cfgprm.u.mem.private_data = tssink_handle;
    TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle , &tssource_cfgprm ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource config source" );

    TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_handle , tssource_handle ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink set source" );

	TestHelper_Configure_TDAL_FS();

    TS_FileSize = 0;
    TS_FileSize_warn = 0x100000;
    TS_FileReader_juststarted = TRUE;
    TS_FileRecord = TDAL_FS_Open( TS_FileRecord_name ,
                                  kTDAL_FS_MODE_READ );

    TestManager_AssertTrue( ( TS_FileRecord != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );

	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{
		pTestSce_Source_Timestamp_Get();

        /** Start TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource start" );
		
		/* These flags are used only in ST5100 architecture */
        tssink_sprm.tv_not_radio = TRUE;
		tssink_sprm.initial_packet_number = 0;
		tssink_sprm.realtime     = FALSE;
		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_handle , &tssink_sprm ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink start" );

		/* Demux play start filtering */
		pTestSce_TS_DMX_PLAY_Start();

		/* Audio/Video Start/Enable SetVolume */
		pTestSce_Configure_AV(pChannel->videoType,
							  pChannel->audioType);

		cAskResult = 'd';
		while( ( cAskResult >= 'a' ) && ( cAskResult <= 's' ) )
		{
			mTBOX_PRINT((kTBOX_LF, "Choose the following speeds:\n"));
			mTBOX_PRINT((kTBOX_LF, "a -> 0x Freeze\n"));
			mTBOX_PRINT((kTBOX_LF, "b -> 0x25 Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "c -> 0x50 Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "d -> 1x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "e -> 2x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "f -> 4x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "g -> 8x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "h -> 16x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "i -> 32x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "j -> 64x Forward\n"));
			mTBOX_PRINT((kTBOX_LF, "k -> 0x25 Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "l -> 0x50 Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "m -> 1x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "n -> 2x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "o -> 4x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "p -> 8x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "q -> 16x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "r -> 32x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "s -> 64x Backward\n"));
			mTBOX_PRINT((kTBOX_LF, "t -> stop the playback\n"));
			mTBOX_GETCHAR(( &cAskResult ));
			mTBOX_PRINT((kTBOX_LF,"%c\n", cAskResult));
			switch(cAskResult)	
			{
			case 'a': speed =    0; break;
			case 'b': speed =   25; break;
			case 'c': speed =   50; break;
			case 'd': speed =  100; break;
			case 'e': speed =  200; break;
			case 'f': speed =  400; break;
			case 'g': speed =  800; break;
			case 'h': speed =  1600; break;
			case 'i': speed =  3200; break;
			case 'j': speed =  6400; break;
			case 'k': speed =   -25; break;
			case 'l': speed =   -50; break;
			case 'm': speed =  -100; break;
			case 'n': speed =  -200; break;
			case 'o': speed =  -400; break;
			case 'p': speed =  -800; break;
			case 'q': speed = -1600; break;
			case 'r': speed = -3200; break;
			case 's': speed = -6400; break;
			default:  speed =   100; break;
			}

			TestManager_AssertEqual( TDAL_AV_SpeedSet(eTDAL_AV_DECODER_VIDEO_1,
													  speed),
									 eTDAL_AV_NO_ERROR,
									 "av speed set" );

		}
	
		pTestSce_Source_Timestamp_Release();

		/* Audio/Video Stop */
		pTestSce_Unconfigure_AV();
		
		/* Demux play stop filtering */
		pTestSce_TS_DMX_PLAY_Stop();
	
		/** Stop/Close TSSource and TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink stop" );
	
		TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource stop" );
		
		TestManager_AssertEqual( TDAL_FS_Close( TS_FileRecord ) ,
                             eTDAL_FS_NO_ERROR ,
                             "fs close" );
	}
	else
	{
		TestManager_LogMessage("!! Before playback could start, a record mud be done !!\n");
	}

    TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_handle ),
                             eTDAL_TSSINK_NO_ERROR,
                             "tssink close" );

    TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle ),
                             eTDAL_TSSOURCE_NO_ERROR,
                             "tssource close source" );

	/* Unconfigure DMX */
	pTestSce_TS_DMX_PLAY_Unconfigure();
	
   /** Disconnecting Routes */
    TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source , route_A_dest ) ,
                             eTDAL_TSROUTE_NO_ERROR ,
                             "tsroute disconnect route A" );

	TestHelper_Unconfigure_TDAL_FS();

   /** Terminate TDAL_TS modules */
    TestManager_AssertEqual(TDAL_TSSINK_Term(),
                            eTDAL_TSSINK_NO_ERROR,
                            "tssink terminate" );
    TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
                            eTDAL_TSSOURCE_NO_ERROR,
                            "tssource terminate" );
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
                            eTDAL_TSROUTE_NO_ERROR,
                            "tsroute terminate" );

   /** Terminate TDAL modules */
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
}

/*! \fn void TestSce_GammaLiveRecord( int tuner )
 *  \brief play Gamma-LIVE stream and start a record using "tuner" parameter
 */
void TestSce_GammaLiveRecord( int tuner )
{
    tTDAL_TS_Object     route_A_source;
    tTDAL_TS_Object     route_B_source;
    tTDAL_TS_Object     route_A_dest;
    tTDAL_TS_Object     route_B_dest;

    tTDAL_TSSOURCE_Handle       tssource_handle;
    tTDAL_TSSOURCE_OpenParam    tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam  tssource_cfgprm;

    tTDAL_TSSINK_Handle         tssink_dmx_handle;
    tTDAL_TSSINK_OpenParam      tssink_dmx_oprm;
    tTDAL_TSSINK_StartParam     tssink_dmx_sprm;

	tTDAL_TSSINK_Handle         tssink_ext_handle;
	tTDAL_TSSINK_OpenParam      tssink_ext_oprm;

    struct TestManager_Channel *    pChannel;

	pTestSce_SetChannel();

    pChannel = TestHelper_GetChannel();
	

	/* Initialize tdal modules */
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);

	/* Initialize TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource init" );
	TestManager_AssertEqual(TDAL_TSSINK_Init(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink init" );
	
	TestHelper_Configure_TDAL_AVS();
	
	TestHelper_Configure_TDAL_FS();


	/* Configure TDAL_DMD and TDAL_DMX */
	TestHelper_DMDTune( tuner );
	
	pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
								   1, /* PCR demux */
								   pChannel->mVideoPID,
								   pChannel->mAudioPID,
								   pChannel->mPCRPID);
	
	/* Connecting Routes */
	route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
										   eTDAL_TS_TYPE_TUNER ,
										   tuner );
	route_B_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
										   eTDAL_TS_TYPE_MEMORY ,
										   0 );
	route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
										   eTDAL_TS_TYPE_DEMUX ,
										   1 );
	route_B_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
										   eTDAL_TS_TYPE_DEMUX ,
										   0 );
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_A_source ,
												   route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route A" );
	
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_B_source ,
												   route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route B" );
	
	/* Open/Configure/Start TSSource and TSSink */
	tssource_oprm.type          = eTDAL_TSSOURCE_TYPE_DEMUX;
	tssource_oprm.buffer_length = TDAL_TS_INTERNAL_BUFFER_SIZE;
	tssource_oprm.packet_length = TDAL_TS_INTERNAL_PACKET_SIZE;

	TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle ,
												 &tssource_oprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource open source" );

    tssink_dmx_oprm.type = eTDAL_TSSINK_TYPE_DEMUX;
	/*tssink_oprm.source = */
	TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_dmx_handle ,
											   &tssink_dmx_oprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx open sink" );
	mTBOX_PRINT((kTBOX_LF, "tssink dmx handle = 0x%x\n", tssink_dmx_handle));
	
	pTestSce_DMX_SectionTwoFilterSet(1);
	
	tssource_cfgprm.u.dmx.demux = route_A_dest;
	TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle ,
												   &tssource_cfgprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource config source" );
	

#if 1 /* GS: debug purpose */
	mTBOX_PRINT((kTBOX_LF, "recorded pid on PMT = %d\n", pChannel->mPMTPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle ,
												   pChannel->mPMTPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add PMT pid" );
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid on PAT = %d\n", 0));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle , 0 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add PAT pid" );
#endif
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid on audio = %d\n", pChannel->mAudioPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle ,
												   pChannel->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add AUD pid" );
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid on video = %d\n", pChannel->mVideoPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle ,
												   pChannel->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add VID pid" );
	if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
		 ( pChannel->mPCRPID != pChannel->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle ,
													   pChannel->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource add PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_dmx_handle ,
													  tssource_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx set source" );

	TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource start" );
	
	/* These flags are used only in ST5100 architecture */
	tssink_dmx_sprm.tv_not_radio          = TRUE;
	tssink_dmx_sprm.initial_packet_number = 0;
	tssink_dmx_sprm.realtime              = TRUE;
	
	/* Demux play start filtering */
	pTestSce_TS_DMX_PLAY_Start();

	/* Audio/Video Start/Enable SetVolume */
	pTestSce_Configure_AV(pChannel->videoType,
						  pChannel->audioType);

	TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_dmx_handle ,
												&tssink_dmx_sprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx start" );
	
	TestManager_AskForChecking("do you see audio and video playing");

    if(TDAL_FS_FileExist(TS_FileRecord_name) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TS_FileRecord_name ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs delete" );
	}

	TS_FileSize = 0;
    TS_FileSize_warn = 0x100000;
    TS_FileRecord = TDAL_FS_Open( TS_FileRecord_name ,
                                  kTDAL_FS_MODE_WRITE/* |
                                  kTDAL_FS_MODE_CREATE |
                                  kTDAL_FS_MODE_TRUNC*/ );

    TestManager_AssertTrue( ( TS_FileRecord != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );


	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{

		pTestSce_Source_Timestamp_Configure(tssource_handle);

		tssink_ext_oprm.type = eTDAL_TSSINK_TYPE_EXTERN;
		tssink_ext_oprm.notification_level = 1024; /* TODO #define it */
		TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_ext_handle ,
												   &tssink_ext_oprm ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern open sink" );
		mTBOX_PRINT((kTBOX_LF, "tssink extern  handle = 0x%x\n", tssink_ext_handle));
		
		TestManager_AssertEqual( TDAL_TSSINK_SetConsumerCallback( tssink_ext_handle ,
																  pTestSce_SinkConsumerCallback_FileWriter,
																  (void *)TS_FileRecord ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern set source" );

		TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_ext_handle ,
														  tssource_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern set source" );
		
		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_ext_handle , NULL ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern start" );

		TestManager_AskForChecking("Press '1' when you want to stop recording");
		
		pTestSce_Source_Timestamp_Unconfigure(tssource_handle);
		
		/** Stop/Close extern TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_ext_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern stop" );
	
		TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_ext_handle ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern close" );
		
		TestManager_AssertEqual( TDAL_FS_Close( TS_FileRecord ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs close" );
	}
	else
	{
		TestManager_LogMessage("!! No file created ==>NO RECORD !!\n");
	}

	TestHelper_Unconfigure_TDAL_FS();
	
    TestManager_AskForChecking("do you see audio and video still playing");
	
	/* Audio/Video Stop */
	pTestSce_Unconfigure_AV();

	/* Demux play stop filtering */
	pTestSce_TS_DMX_PLAY_Stop();
	
	/* Stop/Close TSSource and TSSink */
	TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_dmx_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle ,
													  pChannel->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource remove VID pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle ,
													  pChannel->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource remove AUD pid" );
	if ( ( pChannel->mPCRPID != pChannel->mVideoPID ) &&
		 ( pChannel->mPCRPID != pChannel->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle ,
														  pChannel->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource remove PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_dmx_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx close" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource close source" );
	/* Unconfigure DMX */
	pTestSce_TS_DMX_PLAY_Unconfigure();

	/* Disconnecting Routes */
	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source ,
													  route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route A" );

	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_B_source ,
													  route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route B" );

	/* Terminate TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSSINK_Term(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink terminate" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource terminate" );
	TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate" );
	
	/* Terminate TDAL modules */
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
}

/*! \fn void TestSce_GammaLiveRecord_2On1_Tuner( )
 *  \brief play Gamma-LIVE stream and start 2 records on 1 tuner
 */
void TestSce_GammaLiveRecord_2On1_Tuner(int tuner)
{
    tTDAL_TS_Object     route_A_source;
    tTDAL_TS_Object     route_B_source;
    tTDAL_TS_Object     route_A_dest;
    tTDAL_TS_Object     route_B_dest;

    tTDAL_TSSOURCE_Handle       tssource_handle1;
    tTDAL_TSSOURCE_Handle       tssource_handle2;
    tTDAL_TSSOURCE_OpenParam    tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam  tssource_cfgprm;

    tTDAL_TSSINK_Handle         tssink_dmx_handle;
    tTDAL_TSSINK_OpenParam      tssink_dmx_oprm;
    tTDAL_TSSINK_StartParam     tssink_dmx_sprm;

	tTDAL_TSSINK_Handle         tssink_ext_handle1;
	tTDAL_TSSINK_Handle         tssink_ext_handle2;
	tTDAL_TSSINK_OpenParam      tssink_ext_oprm;

    struct TestManager_Channel *    pChannel1;
    struct TestManager_Channel *    pChannel2;

	pTestSce_SetChannel();
	pTestSce_SetChannel2();

    pChannel1 = TestHelper_GetChannel();
	pChannel2 = TestHelper_GetChannelIdx(TS_gChannel2);

	/* Initialize tdal modules */
#if defined(CHALV2_ON)
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
#else
	pTestSce_Initialize_DMD_DMX_DESC_AVS_DENC_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDENC );
#endif

	/* Initialize TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource init" );
	TestManager_AssertEqual(TDAL_TSSINK_Init(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink init" );
	
	TestHelper_Configure_TDAL_AVS();
	
	TestHelper_Configure_TDAL_FS();


	/* Configure TDAL_DMD and TDAL_DMX */
	TestHelper_DMDTune( tuner );
	
	pTestSce_TS_DMX_PLAY_Configure(0, /* AV demux */
								   1, /* PCR demux */
								   pChannel1->mVideoPID,
								   pChannel1->mAudioPID,
								   pChannel1->mPCRPID);
	
	/* Connecting Routes */
	route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
										   eTDAL_TS_TYPE_TUNER ,
										   tuner );
	route_B_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
										   eTDAL_TS_TYPE_MEMORY ,
										   0 );
	route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
										   eTDAL_TS_TYPE_DEMUX ,
										   1 );
	route_B_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
										   eTDAL_TS_TYPE_DEMUX ,
										   0 );
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_A_source ,
												   route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route A" );
	
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( route_B_source ,
												   route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route B" );
	
	/* Open/Configure/Start TSSource and TSSink */
	tssource_oprm.type          = eTDAL_TSSOURCE_TYPE_DEMUX;
	tssource_oprm.buffer_length = TDAL_TS_INTERNAL_BUFFER_SIZE;
	tssource_oprm.packet_length = TDAL_TS_INTERNAL_PACKET_SIZE;

	TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle1 ,
												 &tssource_oprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 open source" );

	TestManager_AssertEqual( TDAL_TSSOURCE_Open( &tssource_handle2 ,
												 &tssource_oprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 open source" );

	tssink_dmx_oprm.type = eTDAL_TSSINK_TYPE_DEMUX;
	/*tssink_oprm.source = */
	TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_dmx_handle ,
											   &tssink_dmx_oprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx open sink" );
	mTBOX_PRINT((kTBOX_LF, "tssink dmx handle = 0x%x\n", tssink_dmx_handle));
	
	pTestSce_DMX_SectionTwoFilterSet(1);
	
	tssource_cfgprm.u.dmx.demux = route_A_dest;
	TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle1 ,
												   &tssource_cfgprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 config source" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Config( tssource_handle2 ,
												   &tssource_cfgprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 config source" );
	

#if 1 /* GS: debug purpose */
	mTBOX_PRINT((kTBOX_LF, "recorded pid1 on PMT = %d\n", pChannel1->mPMTPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle1 ,
												   pChannel1->mPMTPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 add PMT pid" );
	mTBOX_PRINT((kTBOX_LF, "recorded pid2 on PMT = %d\n", pChannel2->mPMTPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle2 ,
												   pChannel2->mPMTPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 add PMT pid" );
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid on PAT = %d\n", 0));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle1 , 0 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 add PAT pid" );
#endif
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid1 on audio = %d\n", pChannel1->mAudioPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle1 ,
												   pChannel1->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 add AUD pid" );
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid1 on video = %d\n", pChannel1->mVideoPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle1 ,
												   pChannel1->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 add VID pid" );
	if ( ( pChannel1->mPCRPID != pChannel1->mVideoPID ) &&
		 ( pChannel1->mPCRPID != pChannel1->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle1 ,
													   pChannel1->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource1 add PCR pid" );
	}
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid2 on audio = %d\n", pChannel2->mAudioPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle2 ,
												   pChannel2->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 add AUD pid" );
	
	mTBOX_PRINT((kTBOX_LF, "recorded pid2 on video = %d\n", pChannel2->mVideoPID));
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle2 ,
												   pChannel2->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 add VID pid" );
	if ( ( pChannel2->mPCRPID != pChannel2->mVideoPID ) &&
		 ( pChannel2->mPCRPID != pChannel2->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( tssource_handle2 ,
													   pChannel2->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource2 add PCR pid" );
	}
	TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_dmx_handle ,
													  tssource_handle1 ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx set source1" );

	TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle1 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 start" );
	
	/* These flags are used only in ST5100 architecture */
	tssink_dmx_sprm.tv_not_radio          = TRUE;
	tssink_dmx_sprm.initial_packet_number = 0;
	tssink_dmx_sprm.realtime              = TRUE;
	
	/* Demux play start filtering */
	pTestSce_TS_DMX_PLAY_Start();

	/* Audio/Video Start/Enable SetVolume */
#if defined(CHALV2_ON)
	pTestSce_Configure_AV(pChannel1->videoType,
						  pChannel1->audioType);
#else
	pTestSce_Configure_AV();
#endif

	TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_dmx_handle ,
												&tssink_dmx_sprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx start" );
	
	TestManager_AskForChecking("do you see audio and video playing");

    if(TDAL_FS_FileExist(TS_FileRecord_name) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TS_FileRecord_name ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs delete" );
	}

	TS_FileSize = 0;
    TS_FileSize_warn = 0x100000;
    TS_FileRecord = TDAL_FS_Open( TS_FileRecord_name ,
                                  kTDAL_FS_MODE_WRITE/* |
                                  kTDAL_FS_MODE_CREATE |
                                  kTDAL_FS_MODE_TRUNC*/ );

    TestManager_AssertTrue( ( TS_FileRecord != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file 1 open" );


	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{
		mTBOX_PRINT((kTBOX_LF, "File 1  handle = 0x%x\n", TS_FileRecord));

		pTestSce_Source_Timestamp_Configure(tssource_handle1);

		tssink_ext_oprm.type = eTDAL_TSSINK_TYPE_EXTERN;
		tssink_ext_oprm.notification_level = 1024; /* TODO #define it */
		TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_ext_handle1 ,
												   &tssink_ext_oprm ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern1 open sink" );
		mTBOX_PRINT((kTBOX_LF, "tssink extern1  handle = 0x%x\n", tssink_ext_handle1));
		
		TestManager_AssertEqual( TDAL_TSSINK_SetConsumerCallback( tssink_ext_handle1 ,
																  pTestSce_SinkConsumerCallback_FileWriter,
																  (void *)TS_FileRecord ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern1 set consumer callback" );

		TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_ext_handle1 ,
														  tssource_handle1 ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern1 set source1" );
		
		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_ext_handle1 , NULL ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern1 start" );

		
	}
	else
	{
		TestManager_LogMessage("!! No file created ==>NO RECORD 1 !!\n");
	}

    if(TDAL_FS_FileExist(TS_FileRecord_name2) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TS_FileRecord_name2 ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs 2 delete" );
	}

    TS_FileRecord2 = TDAL_FS_Open( TS_FileRecord_name2 ,
								   kTDAL_FS_MODE_WRITE/* |
								   kTDAL_FS_MODE_CREATE |
								   kTDAL_FS_MODE_TRUNC*/ );

	TestManager_AssertTrue( ( TS_FileRecord2 != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file 2 open" );

	if(TS_FileRecord2 != kTDAL_FS_INVALID_HANDLE)
	{
		mTBOX_PRINT((kTBOX_LF, "File 2  handle = 0x%x\n", TS_FileRecord2));

		tssink_ext_oprm.type = eTDAL_TSSINK_TYPE_EXTERN;
		tssink_ext_oprm.notification_level = 1024; /* TODO #define it */
		TestManager_AssertEqual( TDAL_TSSINK_Open( &tssink_ext_handle2 ,
												   &tssink_ext_oprm ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern2 open sink" );
		mTBOX_PRINT((kTBOX_LF, "tssink extern2  handle = 0x%x\n", tssink_ext_handle2));
		
		TestManager_AssertEqual( TDAL_TSSINK_SetConsumerCallback( tssink_ext_handle2 ,
																  pTestSce_SinkConsumerCallback_FileWriter,
																  (void *)TS_FileRecord2 ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern2 set consumer callback" );

		TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( tssink_ext_handle2 ,
														  tssource_handle2 ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern2 set source2" );
		
		TestManager_AssertEqual( TDAL_TSSOURCE_Start( tssource_handle2 ),
						 eTDAL_TSSOURCE_NO_ERROR,
						 "tssource2 start" );

		TestManager_AssertEqual( TDAL_TSSINK_Start( tssink_ext_handle2 , NULL ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern2 start" );
	}
	else
	{
		TestManager_LogMessage("!! No file created ==>NO RECORD 2!!\n");
	}

	TestManager_AskForChecking("Press '1' when you want to stop recording");

	if(TS_FileRecord != kTDAL_FS_INVALID_HANDLE)
	{
		pTestSce_Source_Timestamp_Unconfigure(tssource_handle1);

		/** Stop/Close extern TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_ext_handle1 ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern1 stop" );

		TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_ext_handle1 ),
								 eTDAL_TSSINK_NO_ERROR,
								"tssink extern1 close" );

		TestManager_AssertEqual( TDAL_FS_Close( TS_FileRecord ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs 1 close" );
	}
	
	if(TS_FileRecord2 != kTDAL_FS_INVALID_HANDLE)
	{		
		/** Stop/Close extern TSSink */
		TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_ext_handle2 ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern2 stop" );
	
		TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_ext_handle2 ),
								 eTDAL_TSSINK_NO_ERROR,
								 "tssink extern2 close" );
		
		TestManager_AssertEqual( TDAL_FS_Close( TS_FileRecord2 ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs 2 close" );
	}
	
	TestHelper_Unconfigure_TDAL_FS();
	
    TestManager_AskForChecking("do you see audio and video still playing");
	
	/* Audio/Video Stop */
	pTestSce_Unconfigure_AV();

	/* Demux play stop filtering */
	pTestSce_TS_DMX_PLAY_Stop();
	
	/* Stop/Close TSSource and TSSink */
	TestManager_AssertEqual( TDAL_TSSINK_Stop( tssink_dmx_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle1 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle1 ,
													  pChannel1->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 remove VID pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle1 ,
													  pChannel1->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 remove AUD pid" );
	if ( ( pChannel1->mPCRPID != pChannel1->mVideoPID ) &&
		 ( pChannel1->mPCRPID != pChannel1->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle1 ,
														  pChannel1->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource1 remove PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSOURCE_Stop( tssource_handle2 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle2 ,
													  pChannel2->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 remove VID pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle2 ,
													  pChannel2->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 remove AUD pid" );
	if ( ( pChannel2->mPCRPID != pChannel2->mVideoPID ) &&
		 ( pChannel2->mPCRPID != pChannel2->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( tssource_handle2 ,
														  pChannel2->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource2 remove PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSINK_Close( tssink_dmx_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink dmx close" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle1 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource1 close source" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Close( tssource_handle2 ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource2 close source" );
	/* Unconfigure DMX */
	pTestSce_TS_DMX_PLAY_Unconfigure();

	/* Disconnecting Routes */
	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_A_source ,
													  route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route A" );

	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( route_B_source ,
													  route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route B" );

	/* Terminate TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSSINK_Term(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink terminate" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource terminate" );
	TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate" );
	
	/* Terminate TDAL modules */
#if defined(CHALV2_ON)
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT | eHDMI);
#else
	pTestSce_Terminate_DMD_DMX_DESC_AVS_DENC_AV( eDMD | eDMX | eDESC | eAV | eAVS | eDENC );
#endif
}

/*! \fn void TestSce_GammaLiveRecord_Tuner1( void )
 *  \brief play Gamma-LIVE stream and start a record using Tuner1
 */
void TestSce_GammaLiveRecord_Tuner1(void)
{
    TestSce_GammaLiveRecord( 0 );
}


/*! \fn void TestSce_GammaLiveRecord_Tuner2( void )
 *  \brief play Gamma-LIVE stream and start a record using Tuner2
 */
void TestSce_GammaLiveRecord_Tuner2(void)
{
    TestSce_GammaLiveRecord( 1 );
}

/*! \fn void TestSce_GammaLiveRecord_2On1_Tuner1( void )
 *  \brief play Gamma-LIVE stream and start 2 records using Tuner1
 */
void TestSce_GammaLiveRecord_2On1_Tuner1(void)
{
    TestSce_GammaLiveRecord_2On1_Tuner( 0 );
}

#endif /* USE_TDAL_FS */

#endif /* PACKAGE_PVR */

void    pTestSce_Initialize_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( uint32_t mask )
{
	tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
	tTDAL_DISP_Capability         stDispCap;
	tTDAL_DISP_BlenderCapability  stBlenderCap;
	uint8_t                       blenderIdx;
	uint8_t                       layerIdx;
	tTDAL_DISP_BlenderId          blenderId[4] =
	{
		eTDAL_DISP_BLENDER_ID_0, /* Blender identifier 0 */
		eTDAL_DISP_BLENDER_ID_1, /* Blender identifier 1 */
		eTDAL_DISP_BLENDER_ID_2, /* Blender identifier 2 */
		eTDAL_DISP_BLENDER_ID_3  /* Blender identifier 3 */
	};
    tTDAL_DISP_BlenderZOrderPositions *pstTdalDispZOrderListPos;
	TKEL_err                           eTkelErr;

  if ( (mask & eDMD ) != 0 )
  {
	mTBOX_PRINT((kTBOX_LF, "Init DMD\n"));

    TestHelper_Initialize_TDAL_DMD();
  }

  if ( (mask & eDMX ) != 0 )
  {
    mTBOX_PRINT((kTBOX_LF, "Init DMX\n"));

	TestManager_AssertEqual(TDAL_DMX_Init(),
							kTDAL_DMX_NO_ERROR,
							"dmx init");
  }

  if ( (mask & eDESC ) != 0 )
  {
  #ifdef PRODUCT_USE_CA
    mTBOX_PRINT((kTBOX_LF, "Init DESC\n"));

	TestManager_AssertEqual(TDAL_DESC_Init(),
							kTDAL_DMX_NO_ERROR,
							"desc init");
  #endif
  }

#ifdef AVS_STB
  if ( (mask & eAVS ) != 0 )
  {
	mTBOX_PRINT((kTBOX_LF, "Init AVS\n"));

	TestManager_AssertEqual(TDAL_AVS_Init(),
							eTDAL_AVS_NO_ERROR,
							"avs init");
  }
#endif

  if ( (mask & eDISP ) != 0 )
  {
	mTBOX_PRINT((kTBOX_LF, "Init DISP\n"));

	TestManager_AssertEqual(TDAL_DISP_Init(),
							eTDAL_DISP_NO_ERROR,
							"disp init");

	TestManager_AssertEqual(TDAL_DISP_CapabilityGet(&stDispCap),
							eTDAL_DISP_NO_ERROR,
							"disp caps");
	for(blenderIdx = 0; blenderIdx < stDispCap.nbBlender; blenderIdx++)
	{
		TestManager_AssertEqual(TDAL_DISP_BlenderCapabilityGet(blenderId[blenderIdx],
															   &stBlenderCap),
								eTDAL_DISP_NO_ERROR,
								"disp blender caps");
		/* allocate ZOrder possible positions structure */
		eTkelErr = TKEL_Malloc(stBlenderCap.nbLayer * sizeof(tTDAL_DISP_BlenderZOrderPositions),
							   (void*)&pstTdalDispZOrderListPos);
		mTBOX_ASSERT(eTkelErr == TKEL_NO_ERR);
		
		if(pstTdalDispZOrderListPos != NULL)
		{
			TestManager_AssertEqual(TDAL_DISP_BlenderZOrderPositionsGet(blenderId[blenderIdx],
																		pstTdalDispZOrderListPos),
									eTDAL_DISP_NO_ERROR,
									"disp blender zorder positions get");
			
			for(layerIdx = 0; layerIdx < stBlenderCap.nbLayer; layerIdx++)
			{
				/* search for video layer Id presence */
				if((pstTdalDispZOrderListPos[layerIdx].LayerId == eTDAL_DISP_LAYER_VIDEO_ID_0) ||
				   (pstTdalDispZOrderListPos[layerIdx].LayerId == eTDAL_DISP_LAYER_VIDEO_ID_1))
				{
					TS_AV_VideoLayer[TS_AV_NbVideoLayer] = pstTdalDispZOrderListPos[layerIdx].LayerId;
					TS_AV_NbVideoLayer++;
				}
			}
			
			/* free ZOrder possible positions structure */
			eTkelErr = TKEL_Free((void*)pstTdalDispZOrderListPos);
			mTBOX_ASSERT(eTkelErr == TKEL_NO_ERR);
		}


	}
  }

  if ( (mask & eOUTPUT ) != 0 )
  {
    mTBOX_PRINT((kTBOX_LF, "Init OUTPUT\n"));

	TestManager_AssertEqual(TDAL_OUTPUT_Init(),
							eTDAL_OUTPUT_NO_ERROR,
							"output init");
	
	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
	
	TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
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

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_VideoDencStandardSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0,
																 eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc STD set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc enable");
	}
  }

  #ifdef USE_TDAL_HDMI
  if ( (mask & eHDMI ) != 0 )
  {
	  mTBOX_PRINT((kTBOX_LF, "Init HDMI\n"));
	
	  TestManager_AssertEqual(TDAL_HDMI_Init(),
							  eTDAL_HDMI_NO_ERROR,
							  "hdmi init" );
	  if(stOutputCap.nbHDMI > 0)
	  {
		  TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												  pTestSce_HDMI_Callback,
												  FALSE),
								  eTDAL_HDMI_NO_ERROR,
								  "hdmi start" );
	  }
  }
  #endif /* #ifdef USE_TDAL_HDMI */
	
  if ( (mask & eAV ) != 0 )
  {
    mTBOX_PRINT((kTBOX_LF, "Init AV\n"));
	
	TestManager_AssertEqual(TDAL_AV_Init(),
							eTDAL_AV_NO_ERROR,
							"av init");
  }
}

void    pTestSce_Terminate_DMD_DMX_DESC_AVS_DISP_OUTPUT_AV( uint32_t mask )
{

  if ( (mask & eAV ) != 0 )
  {
    TestManager_AssertEqual(TDAL_AV_Terminate(),
							eTDAL_AV_NO_ERROR,
							"av terminate" );
  }
  mTBOX_PRINT((kTBOX_LF, "Term AV done\n"));
  if ( (mask & eDISP ) != 0 )
  {
	TestManager_AssertEqual(TDAL_DISP_Term(),
							eTDAL_DISP_NO_ERROR,
							"disp terminate");
	TS_AV_NbVideoLayer    = 0;
	TS_AV_VideoLayer[0] = (-1);
	TS_AV_VideoLayer[1] = (-1);
	TS_AV_VideoLayer[2] = (-1);
	TS_AV_VideoLayer[3] = (-1);
  }
  mTBOX_PRINT((kTBOX_LF, "Term DISP done\n"));

  if ( (mask & eOUTPUT ) != 0 )
  {
	TestManager_AssertEqual(TDAL_OUTPUT_Term(),
							eTDAL_OUTPUT_NO_ERROR,
							"output terminate");
  }
  mTBOX_PRINT((kTBOX_LF, "Term OUTPUT done\n"));

  if ( (mask & eHDMI ) != 0 )
  {
	TestManager_AssertEqual(TDAL_HDMI_Term(),
							eTDAL_HDMI_NO_ERROR,
							"hdmi terminate");
  }
  mTBOX_PRINT((kTBOX_LF, "Term HDMI done\n"));

#ifdef AVS_STB
  if ( (mask & eAVS ) != 0 )
  {
	TestManager_AssertEqual(TDAL_AVS_Term(),
							eTDAL_AVS_NO_ERROR,
							"avs terminate");
  }
  mTBOX_PRINT((kTBOX_LF, "Term AVS done\n"));
#endif

  if ( (mask & eDESC ) != 0 )
  {
  #ifdef PRODUCT_USE_CA
	TestManager_AssertEqual(TDAL_DESC_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"desc terminate");
	mTBOX_PRINT((kTBOX_LF, "Term DESC done\n"));
  #endif
  }

  if ( (mask & eDMX ) != 0 )
  {
	TestManager_AssertEqual(TDAL_DMX_Terminate(),
							kTDAL_DMX_NO_ERROR,
							"dmx terminate");
  }
  mTBOX_PRINT((kTBOX_LF, "Term DMX done\n"));


  if ( (mask & eDMD ) != 0 )
  {
    TestHelper_Terminate_TDAL_DMD();
  }
  mTBOX_PRINT((kTBOX_LF, "Term DMD done\n"));
}

void pTestSce_Configure_AV
(
	tTDAL_AV_VideoType videoType,
    tTDAL_AV_AudioType audioType
)
{
	tTDAL_AV_Decoder                        tdalAvDecoderVideo;
	tTDAL_AV_StreamType                     tdalAvStreamTypeVideo;
	tTDAL_AV_Decoder                        tdalAvDecoderAudio;
	tTDAL_AV_StreamType                     tdalAvStreamTypeAudio;
	tTDAL_OUTPUT_AudioAnaCapability         stAudioAnaCapability;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAnaSpeakerAttenuation;
	uint8_t                                 layerIdx;

	tdalAvDecoderVideo              = eTDAL_AV_DECODER_VIDEO_1;
	tdalAvStreamTypeVideo.videoType = videoType;

	TestManager_AssertEqual( TDAL_AV_SpeedSet(tdalAvDecoderVideo,
											  100),
							 eTDAL_AV_NO_ERROR,
							 "av speed set 100" );

    TestManager_AssertEqual(TDAL_AV_Start(tdalAvDecoderVideo,
										  tdalAvStreamTypeVideo),
							eTDAL_AV_NO_ERROR,
							"av start video");

	for(layerIdx = 0; layerIdx < TS_AV_NbVideoLayer; layerIdx++)
	{
		TestManager_AssertEqual( TDAL_DISP_LayerEnable(TS_AV_VideoLayer[layerIdx]),
								 eTDAL_DISP_NO_ERROR,
								 "disp layer enable" );
	}

	tdalAvDecoderAudio              = eTDAL_AV_DECODER_AUDIO_1;
	tdalAvStreamTypeAudio.audioType = audioType;
	
	TestManager_AssertEqual(TDAL_AV_Start(tdalAvDecoderAudio,
										  tdalAvStreamTypeAudio),
							eTDAL_AV_NO_ERROR,
							"av start audio");
    TestManager_AssertEqual( TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							 eTDAL_OUTPUT_NO_ERROR,
							 "output audio enable" );
    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
															  &stAudioAnaCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");

	stAudioAnaSpeakerAttenuation.FrontRLAttenuation     = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.FrontCenterAttenuation = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.SurroundRLAttenuation  = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.LFEAttenuation         = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.RearRLAttenuation      = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.RearCenterAttenuation  = stAudioAnaCapability.AttenuationMax;

	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																	  &stAudioAnaSpeakerAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");
}

void    pTestSce_Unconfigure_AV( void )
{
	tTDAL_AV_Decoder tdalAvDecoderVideo;
	tTDAL_AV_Decoder tdalAvDecoderAudio;
	uint8_t layerIdx;

	tdalAvDecoderVideo = eTDAL_AV_DECODER_VIDEO_1;
	tdalAvDecoderAudio = eTDAL_AV_DECODER_AUDIO_1;

	for(layerIdx = 0; layerIdx < TS_AV_NbVideoLayer; layerIdx++)
	{
		TestManager_AssertEqual( TDAL_DISP_LayerDisable(TS_AV_VideoLayer[layerIdx]),
								 eTDAL_DISP_NO_ERROR,
								 "disp layer disable" );
	}
    TestManager_AssertEqual(TDAL_AV_Stop(tdalAvDecoderVideo),
							eTDAL_AV_NO_ERROR,
							"av stop video");
    TestManager_AssertEqual(TDAL_AV_Stop(tdalAvDecoderAudio),
							eTDAL_AV_NO_ERROR,
							"av stop audio");
}
#ifdef USE_TDAL_HDMI
void pTestSce_HDMI_Callback
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

	stHdmiAVI.activeAspectRatio 		= 0x8; /*1000 -> same as picture*/
	stHdmiAVI.pictureScaling 			= eTDAL_HDMI_PICTURE_NON_UNIFORM_SCALING;
	stHdmiAVI.PixelRepetition			= 0;
	stHdmiAVI.VideoFormatIDCode			= 0;
	stHdmiAVI.LineNEndofTopLower        = 0;
	stHdmiAVI.LineNEndofTopUpper		= 0;
	stHdmiAVI.LineNStartofBotLower		= 0;
	stHdmiAVI.LineNStartofBotUpper		= 0;
	stHdmiAVI.PixelNEndofLeftLower		= 0;
	stHdmiAVI.PixelNEndofLeftUpper		= 0;
	stHdmiAVI.PixelNStartofRightLower	= 0;
	stHdmiAVI.PixelNStartofRightUpper	= 0;
	
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


