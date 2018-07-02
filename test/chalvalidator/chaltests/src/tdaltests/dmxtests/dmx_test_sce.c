/** \file dmx_test_sce.c
 *  \brief TDAL DMX SCENARIO TESTS
 *  \author Mario Bensi
 *  \author Olivier DOLE
 *  \date 04/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include <stdio.h>
#include <string.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_ts.h"
#include "tdal_dmx.h"

#include "testhelper.h"
#include "dmxtests.h"

void DMX_Scenario_Bug_NegativeFiltering();
void DMX_Scenario_SDTNegativeFiltering_TemporarilyInactive();
void DMX_Scenario_PMTNegativeFiltering();

IMPORT int gChannel;

/******************************************************
 *              Defines                               *
 ******************************************************/

/* NOTE: these values correspond to "TF1" service on french DVB-T network */
#define kDMX_TEST_PES_TYPE1  eTDAL_DMX_TELETEXT_STREAM
#define kDMX_TEST_PES_PID1   102 /*212*/ //140 //in dvb-t_818Mhz.ts
#define kDMX_TEST_PES_TYPE2  eTDAL_DMX_SUBTITLE_STREAM
#define kDMX_TEST_PES_PID2   111 /*320*/ //150

static TKEL_sema_id dmxTestSectionSem = (TKEL_sema_id)NULL;
static TKEL_sema_id dmxTestPESSem     = (TKEL_sema_id)NULL;

static int dmxTestPesPacketToWait = 1;
static int dmxTestPesPacketRecv   = 0;
static int dmxTestSectionToWait = 1;
static int dmxTestSectionRecv   = 0;

static tTestHelper_SectionHeader  dmxTestSectHeader[2];

/******************************************************
 *             CallBack                               *
 *****************************************************/

static void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
										tTDAL_DMX_FilterId FilterId,
										uint32_t Filter_track,
										uint32_t Length,
										uint8_t *Buffer);

static void dmxTestSectionNotify(uint8_t* buffer, uint32_t length);

static void dmxTestPESNotify(tTDAL_DMX_ChannelId ChannelId,
							 tTDAL_DMX_FilterId  FilterId,
							 uint32_t            FilterTrack,
							 uint32_t            Length,
							 uint8_t*            Buffer);

/******************************************************
 *              DMX test tree                         *
 ******************************************************/

#if 0 //Currently not used
static tTestNode gTestScenarioSDTNegativeFiltering = {
	"TestScenarioSDTNegativeFiltering",
	"Test to filter SDT with negative filtering parameters",
	0,
	/*DMX_Scenario_SDTNegativeFiltering_TemporarilyInactive ,*/
	DMX_Scenario_SDTNegativeFiltering,
	NULL
};


static tTestNode gTestScenarioPMTNegativeFiltering = {
	"TestScenarioPMTNegativeFiltering",
	"Test to filter PMT with negative filtering parameters",
	0,
    DMX_Scenario_PMTNegativeFiltering,
	NULL
};
#endif

static tTestNode gTestScenarioSDTFilterSet = {
	"TestScenarioSDTFilterSet",
	"Test to filter SDT then change filter settings without a free filter",
	0,
	DMX_Scenario_SDTFilterSet,
	NULL
};

static tTestNode gTestScenarioBugBlockedOnNegativeFiltering = {
	"TestScenarioBugBlockedOnNegativeFiltering",
	"Test scenario that demonstrates a bug on negative filtering",
	0,
	DMX_Scenario_Bug_NegativeFiltering,
	NULL
};

static tTestNode gTestScenarioPesFilterSet = {
	"TestScenarioPesFilterSet",
	"Test to filter PES packet",
	0,
	DMX_Scenario_PesFilterSet,
	NULL
};

static tTestNode gTestScenarioPesTwoFilterSet = {
	"TestScenarioPesTwoFilterSet",
	"Test 2 filters PES packet",
	0,
	DMX_Scenario_PesTwoFilterSet,
	NULL
};

static tTestNode gTestScenarioSectionTwoFilterSet = {
	"TestScenarioSectionTwoFilterSet",
	"Test 2 filters section",
	0,
	DMX_Scenario_SectionTwoFilterSet,
	NULL
};

static tTestNode gTestScenarioSDT_NIT_TDT_TOTFilterSet = {
	"TestScenarioTDT_TOTFilterSet",
	"Test to filter SDT, NIT, TDT and TOT DVB tables",
	0,
	DMX_Scenario_SDT_NIT_TDT_TOTFilterSet,
	NULL
};

tTestNode* gTDAL_DMXTestFunctionnalNodeList[] = {
	//&gTestScenarioSDTNegativeFiltering,
	//&gTestScenarioPMTNegativeFiltering,
	&gTestScenarioSDTFilterSet,
	&gTestScenarioBugBlockedOnNegativeFiltering,
	&gTestScenarioPesFilterSet,
	&gTestScenarioPesTwoFilterSet,
	&gTestScenarioSectionTwoFilterSet,
	&gTestScenarioSDT_NIT_TDT_TOTFilterSet,
	NULL
};

tTestNode gTDAL_DMX_ScenarioTests = {
    "TDAL_DMX scenario tests",
    "test the TDAL_DMX function input parameters",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_DMXTestFunctionnalNodeList
};

/*! \fn void DMX_Scenario_SDTNegativeFiltering( void )
 *  \brief Do negative filtering on Service Description Table (SDT)
 *
 *  This test locks a signal and configures demux.
 *  It sets a negative filter to retrieve SDTs i.e.
 *  SDTs describing the actual TS and SDTs refering to another TS.
 *  It also sets a filter for version_number field in SDT.
 *  To be sure that the 2 SDT types and different version_number
 *  fields are retrieved, a filtering of 10 SDTs is done.
 */
void DMX_Scenario_SDTNegativeFiltering( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	uint8_t HardValue[4] = {0x42, 0x00, 0x00, 0x00}; /* That's the SDT table ID for actuel TS */
	uint8_t HardMask[4]  = {0xFF, 0x00, 0x00, 0x00};
	uint8_t HardMode[4]  = {0xFF, 0x00, 0x00, 0x00};
	tTDAL_DMX_ChannelCallbackFct callback;
	uint16_t Pid = 0x0011;
    uint8_t  VersionNumber;

	/*gChannel = 4; *//* FLOREZ player */
	/*gChannel = 0; *//* Live TF1 */
	gChannel = 1; /* Live France2 */

	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );

	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestSectionSem),
			TKEL_NO_ERR,
			"TKEL CreateSemaphore" );
	
	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	TestManager_AssertEqual(TDAL_DMX_Allocate_Channel(gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
													  &type,
													  &channelId),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate channel");
	
	callback = dmxTestGenericSectionNotify;
	dmxTestSectionToWait = 1;
	dmxTestSectionRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for SDT and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, Pid ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	
	/* Set a filter of depth 0 to filter SDT (tbId 0x42)
	 * Set a filter of depth 4 to filter version number on SDT */
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId, 0, 4,
							&HardValue[0], &HardMask[0], &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),	
							kTDAL_DMX_NO_ERROR, "DMX control filter" );
					
	
	/* reset section header notification data */
	dmxTestSectHeader[0].TableId = 0x00;

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,	"DMX control channel" );
	
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestSectionSem ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	mTBOX_PRINT((kTBOX_LF,"tbid=0x%x vers=%d to negative filter\r\n",
				 dmxTestSectHeader[0].TableId,
				 dmxTestSectHeader[0].VersionNumber));

	/* save current table version number */
	VersionNumber = dmxTestSectHeader[0].VersionNumber;

	/* Change filter settings to get negative version number */
	HardValue[3] = dmxTestSectHeader[0].VersionNumber << 1;
	HardMask[3]  = 0x3E;
	HardMode[3]  = 0xC1;
			
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId, 0, 4,
							&HardValue[0], &HardMask[0], &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );


	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,	"DMX control channel" );
#if 1
	mTBOX_PRINT((kTBOX_LF,"Wait 50s...\r\n"));
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestSectionSem, ms2tick(5000/*0*/) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#else
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestSectionSem ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#endif
		
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );

	mTBOX_PRINT((kTBOX_LF,"tbid=0x%x version[old=%d, new=%d]\r\n",
				 dmxTestSectHeader[0].TableId,
				 VersionNumber,
				 dmxTestSectHeader[0].VersionNumber));
			
	TestManager_AssertTrue((VersionNumber != dmxTestSectHeader[0].VersionNumber),
							"SDT version number");

	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
	
	gChannel = 0;
}
void DMX_Scenario_PMTNegativeFiltering( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	uint8_t HardValue[4] = {0x02, 0x00, 0x00, 0x00}; 
	uint8_t HardMask[4]  = {0xFF, 0x00, 0x00, 0x00};
	uint8_t HardMode[4]  = {0xFF, 0x00, 0x00, 0x00};
	tTDAL_DMX_ChannelCallbackFct callback;
	uint16_t Pid = 0x07D0;
    uint8_t  VersionNumber;

	/*gChannel = 4; *//* FLOREZ player */
	/*gChannel = 0; *//* Live TF1 */
	gChannel = 1; 

	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );

	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestSectionSem),
			TKEL_NO_ERR,
			"TKEL CreateSemaphore" );
	
	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	TestManager_AssertEqual(TDAL_DMX_Allocate_Channel(gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
													  &type,
													  &channelId),
							kTDAL_DMX_NO_ERROR,
							"DMX allocate channel");
	
	callback = dmxTestGenericSectionNotify;
	dmxTestSectionToWait = 1;
	dmxTestSectionRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for SDT and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, Pid ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	

	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId, 0, 4,
							&HardValue[0], &HardMask[0], &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),	
							kTDAL_DMX_NO_ERROR, "DMX control filter" );
					
	
	/* reset section header notification data */
	dmxTestSectHeader[0].TableId = 0x00;

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,	"DMX control channel" );
	
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestSectionSem ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	mTBOX_PRINT((kTBOX_LF,"tbid=0x%x vers=%d to negative filter\r\n",
				 dmxTestSectHeader[0].TableId,
				 dmxTestSectHeader[0].VersionNumber));

	/* save current table version number */
	VersionNumber = dmxTestSectHeader[0].VersionNumber;

	/* Change filter settings to get negative version number */
	HardValue[3] = dmxTestSectHeader[0].VersionNumber << 1;
	HardMask[3]  = 0x3E;
	HardMode[3]  = 0xC1;
			
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId, 0, 4,
							&HardValue[0], &HardMask[0], &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );


	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR,	"DMX control channel" );
#if 0
	mTBOX_PRINT((kTBOX_LF,"Wait 50s...\r\n"));
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestSectionSem, ms2tick(50000) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#else
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestSectionSem ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#endif
		
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );

	mTBOX_PRINT((kTBOX_LF,"tbid=0x%x version[old=%d, new=%d]\r\n",
				 dmxTestSectHeader[0].TableId,
				 VersionNumber,
				 dmxTestSectHeader[0].VersionNumber));
			
	TestManager_AssertTrue((VersionNumber != dmxTestSectHeader[0].VersionNumber),
							"PMT version number");

	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
	
	gChannel = 0;
}

void DMX_Scenario_SDTFilterSet( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	uint8_t HardValue[2] = {0x42, 0x46}; /* That's the SDT table ID for actuel TS */
	uint8_t HardMask[2] = {0xFF, 0xFF};
	uint8_t HardMode[2] = {0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelCallbackFct callback;
	/*gChannel = 4; *//* FLOREZ player */

	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestSectionSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	
	
	type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

	callback = dmxTestGenericSectionNotify;
	dmxTestSectionToWait = 100;
	dmxTestSectionRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for SDT and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, 0x0011 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId, 0, 1,
							&HardValue[0], &HardMask[0], &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter" );

	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestSectionSem, ms2tick(10000 * dmxTestSectionToWait) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );

	dmxTestSectionToWait = 1;
	dmxTestSectionRecv   = 0;
	
#if 1
	/* sdt other acquisition  ==> may be failed accoding to the stream */
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId, 1, 1,
							&HardValue[1], &HardMask[1], &HardMode[1],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter" );
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestSectionSem, ms2tick(10000 * dmxTestSectionToWait) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#endif	
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId,eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

void DMX_Scenario_SectionTwoFilterSet( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId0;
	tTDAL_DMX_FilterId       filterId0;
	tTDAL_DMX_ChannelId      channelId1;
	tTDAL_DMX_FilterId       filterId1;
	uint8_t HardValue[2] = {0x00, 0x02}; /* That's the PAT/PMT table ID */
	uint8_t HardMask[2]  = {0xFF, 0xFF};
	uint8_t HardMode[2]  = {0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelCallbackFct callback;
	
	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestSectionSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	
	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId0),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel0" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId1),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel1" );

	callback = dmxTestGenericSectionNotify;
	dmxTestSectionToWait = 100;
	dmxTestSectionRecv   = 0;

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
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId1, 100 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel0 PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId0,
												  filterId0,
												  0,
												  1,
												  &HardValue[0],
												  &HardMask[0],
												  &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter0" );

	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId1,
												  filterId1,
												  0,
												  1,
												  &HardValue[1],
												  &HardMask[1],
												  &HardMode[1],eTDAL_DMX_CRC_IGNORE ),
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

	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestSectionSem ),
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
	
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

static void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
										tTDAL_DMX_FilterId FilterId,
										uint32_t Filter_track,										
										uint32_t Length,
										uint8_t *Buffer)
{
	TKEL_err tkError;

	dmxTestSectionRecv++;
	
/*	mTBOX_PRINT((kTBOX_LF,"SEC[%d/%d]->ch=%d flt=%d track=%d size=%d data[0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X]\r\n",
				 dmxTestSectionRecv,
				 dmxTestSectionToWait,
				 ChannelId,
				 FilterId,
				 Filter_track,
				 Length,
				 Buffer[0],Buffer[1],Buffer[2],Buffer[3],
				 Buffer[4],Buffer[5],Buffer[6],Buffer[7]));
*/	
	if(Filter_track < 2)
	{
		TestHelper_DVBHeaderExtract(Buffer,  &dmxTestSectHeader[Filter_track]);
		
		mTBOX_PRINT((kTBOX_LF,"SEC[%d/%d]->ch=%d flt=%d track=%d tbid=0x%x vers=%d\r\n",
					 dmxTestSectionRecv,
					 dmxTestSectionToWait,
					 ChannelId,
					 FilterId,
					 Filter_track,
					 dmxTestSectHeader[Filter_track].TableId,
					 dmxTestSectHeader[Filter_track].VersionNumber));

	}


	/*  check the section received and print info  */
	dmxTestSectionNotify(Buffer,Length);
	
	/*  free the TDAL buffer  */
	/* tkError = TKEL_Free(Buffer); */
	if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
	{
		mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
	}
	
	if(dmxTestSectionRecv >= dmxTestSectionToWait)
	{
		tkError = TKEL_SignalSemaphore(dmxTestSectionSem);
		if (tkError != TKEL_NO_ERR)
		{
			mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tkError ));
		}
	}
	
}

static void dmxTestSectionNotify(uint8_t* buffer, uint32_t length)
{
	int i;
	char outStr[100];
	
	outStr[0] = '\0';
	
	if (buffer[0] == 0x00)
	{
		strcat(outStr,"PAT |" );
	}
	if (buffer[0] == 0x02)
	{
		strcat(outStr,"PMT |" );
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
	
	TestHelper_DVBBufferDecode(buffer, TRUE);

}

static void dmxTestPESNotify
(
    tTDAL_DMX_ChannelId ChannelId,
    tTDAL_DMX_FilterId  FilterId,
    uint32_t            FilterTrack,
    uint32_t            Length,
    uint8_t*            Buffer
)
{
	TKEL_err tkError;
	int      iLength;
	bool     bErr = TRUE;

	dmxTestPesPacketRecv++;
	
/*	mTBOX_PRINT((kTBOX_LF,"PES[%d/%d]->ch=%d flt=%d size=%d data[0x%X,0x%X,0x%X,0x%X,0x%X,0x%X]\r\n",
				 dmxTestPesPacketRecv,
				 dmxTestPesPacketToWait,
				 ChannelId,
				 FilterId,
				 Length,
				 Buffer[0],Buffer[1],Buffer[2],Buffer[3],Buffer[4],Buffer[5]));	
*/	
	/* PES packet: check start code prefix */
	if((Buffer[0] == 0x00) &&
	   (Buffer[1] == 0x00) &&
	   (Buffer[2] == 0x01))
	{
		/*mTBOX_PRINT((kTBOX_LF,"PES start code prefix OK\n"));*/
	}
	else
	{
		mTBOX_PRINT((kTBOX_LF,"PES start code prefix error[0x%X%X%X]\n",
					 Buffer[0],Buffer[1],Buffer[2]));
		bErr = FALSE;
	}

	/* PES packet: stream_id */
	mTBOX_PRINT((kTBOX_LF,"PES[%d/%d]->stream_id[0x%x]\n",
				 dmxTestPesPacketRecv,
				 dmxTestPesPacketToWait,
				 Buffer[3]));
	
	/* PES packet: length */
	iLength  = 0;
	iLength  = (Buffer[4] << 8) & 0xFF00;
	iLength += Buffer[5] & 0xFF;

/*	mTBOX_PRINT((kTBOX_LF,"PES length[%d]\n", iLength));*/


	if (TDAL_DMX_Free_Buffer(Buffer) != kTDAL_DMX_NO_ERROR)
	{
		mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
		bErr = FALSE;
	}
	
	if(dmxTestPesPacketRecv >= dmxTestPesPacketToWait)
	{
		tkError = TKEL_SignalSemaphore(dmxTestPESSem);
		if (tkError != TKEL_NO_ERR)
		{
			mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tkError ));
			bErr = FALSE;
		}
	}
	
/*	TestManager_AssertTrue( bErr, "PES packet notify" );*/

}

/*! \fn void DMX_Scenario_SDTNegativeFiltering_TemporarilyInactive( void )
 *  \brief temporary useless function to avoid a hang
 *  TO DO : remove this function when bug is fixed
 */
void DMX_Scenario_SDTNegativeFiltering_TemporarilyInactive()
{
	TestManager_AssertTrue( 0 , "Test is not executed because it hangs if some other dmx test have been executed" );
}

/*! \fn void DMX_Scenario_Bug_NegativeFiltering( void )
 *  \brief demontrates a bug when 2 init-term are done before the negative filtering test: it blocks
 *  \bug there is a bug when 2 init-term are done before the negative filtering test: it blocks
 */
void DMX_Scenario_Bug_NegativeFiltering()
{
	/*TestManager_AssertTrue( 0 , "This test hangs systematically. Uncomment to see result" );
	 UNCOMMENT TO TEST EXECUTE THIS TEST CASE */
	
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
	
    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

	DMX_Scenario_SDTNegativeFiltering();
}

/*! \fn void DMX_Scenario_PesFilterSet( void )
 *  \brief check PES packet filtering
 *
 *  This test locks a signal and configures demux.
 *  It sets a filter to retrieve PES packet.
 */
void DMX_Scenario_PesFilterSet( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	tTDAL_DMX_ChannelCallbackFct callback;

	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	

	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestPESSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	
	
	/* get subtitle pes */
	type.eChannelStream     = kDMX_TEST_PES_TYPE2;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

	callback = dmxTestPESNotify;

	dmxTestPesPacketToWait = 100;
	dmxTestPesPacketRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for PES and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, kDMX_TEST_PES_PID1 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
								kTDAL_DMX_NO_ERROR, "DMX control filter" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
								kTDAL_DMX_NO_ERROR, "DMX control filter" );
	
	mTBOX_PRINT((kTBOX_LF, "Waiting for PES packet(s)...\n"));

#if 0
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestPESSem, ms2tick(6000/*0*/) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#else
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestPESSem),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#endif

#if 0 /* add a second PES reception sequence */
	dmxTestPesPacketToWait = 100;
	dmxTestPesPacketRecv   = 0;
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId,eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );

#if 0
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestPESSem, ms2tick(6000/*0*/) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#else
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestPESSem),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#endif
#endif

	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_DISABLE ),
								kTDAL_DMX_NO_ERROR, "DMX control filter" );
	
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

}


/*! \fn void DMX_Scenario_PesTwoFilterSet( void )
 *  \brief check PES packet filtering on two pids
 *
 *  This test locks a signal and configures demux.
 *  It sets two filters to retrieve PES packet.
 */
void DMX_Scenario_PesTwoFilterSet( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId1;
	tTDAL_DMX_FilterId       filterId1;
	tTDAL_DMX_ChannelId      channelId2;
	tTDAL_DMX_FilterId       filterId2;
	tTDAL_DMX_ChannelCallbackFct callback;
	
	/* Lock a signal */

	TestHelper_Initialize_TDAL_DMD();
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestPESSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	
	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	/* try to get teletext pes */
	type.eChannelStream     = kDMX_TEST_PES_TYPE1;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId1),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel1" );

	/* try to get subtitle pes */
	type.eChannelStream     = kDMX_TEST_PES_TYPE2;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId2),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel2" );
	callback = dmxTestPESNotify;

	dmxTestPesPacketToWait = 50;
	dmxTestPesPacketRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId1, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel1 CallBack" );
	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId2, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel2 CallBack" );
	/* Allocate a filter for PES and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId1, &filterId1 ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter1" );
	/* Allocate a filter for PES and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId2, &filterId2 ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter2" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId1, kDMX_TEST_PES_PID1),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel1 PID" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId2, kDMX_TEST_PES_PID2 ),
						kTDAL_DMX_NO_ERROR, "DMX Set Channel2 PID" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId1, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel1" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId2, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel2" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId1, eTDAL_DMX_CTRL_ENABLE ),
								kTDAL_DMX_NO_ERROR, "DMX control filter1" );
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId2, eTDAL_DMX_CTRL_ENABLE ),
								kTDAL_DMX_NO_ERROR, "DMX control filter2" );
	
	mTBOX_PRINT((kTBOX_LF, "Waiting for PES packet(s)...\n"));

#if 0
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestPESSem, ms2tick(6000/*0*/) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#else
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestPESSem),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
#endif
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId1,eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel1" );
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId2,eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel2" );
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId1 ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel1" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId2 ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel2" );
	
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

void DMX_Scenario_SDT_NIT_TDT_TOTFilterSet( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId[2];
	uint8_t HardValue[2][3] = {{0x70, 0x00, 0xa8}, {0x73, 0x00, 0xa8}}; /* That's the TOT/TDT table ID for actuel TS */
	uint8_t HardMask[2][3]  = {{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}};
	uint8_t HardMode[2][3]  = {{0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}}; /* Set to 0 for negative filtering */
	uint8_t HardValueSDT[3] = {0x42, 0x00, 0xa8}; /* That's the SDT table ID for actuel TS */
	uint8_t HardMaskSDT[3]  = {0xFF, 0x00, 0x00};
	uint8_t HardModeSDT[3]  = {0xFF, 0xFF, 0xFF}; /* Set to 0 for negative filtering */
	uint8_t HardValueNIT[3] = {0x40, 0x00, 0xa8}; /* That's the NIT table ID for actuel TS */
	uint8_t HardMaskNIT[3]  = {0xFF, 0x00, 0x00};
	uint8_t HardModeNIT[3]  = {0xFF, 0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelId      channelIdSDT;
	tTDAL_DMX_FilterId       filterIdSDT;
	tTDAL_DMX_ChannelId      channelIdNIT;
	tTDAL_DMX_FilterId       filterIdNIT;
	tTDAL_DMX_ChannelCallbackFct callback;
	
	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestSectionSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	
	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

	callback = dmxTestGenericSectionNotify;
	dmxTestSectionToWait = 100;
	dmxTestSectionRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for TDT  */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId[0] ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	/* Allocate a filter for T0T  */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId[1] ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	mTBOX_PRINT((kTBOX_LF,"TDT/TOT ch=%d fl=%d on tbid=0x%x and fl=%d on tbid=0x%x\r\n",
				 channelId,
				 filterId[0],
				 HardValue[0][0],
				 filterId[1],
				 HardValue[1][0]));

	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, 0x14 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId[0], 0, 3,
							&HardValue[0][0], &HardMask[0][0], &HardMode[0][0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );

	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId, filterId[1], 1, 3,
							&HardValue[1][0], &HardMask[1][0], &HardMode[1][0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter" );
#if 1
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelIdSDT),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel SDT" );

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelIdSDT, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for SDT */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelIdSDT, &filterIdSDT ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter SDT" );
		
	mTBOX_PRINT((kTBOX_LF,"SDT ch=%d fl=%d on tbid=0x%x\r\n",
			 channelIdSDT,
			 filterIdSDT,
			 HardValueSDT[0]));

	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelIdSDT, 0x11 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID SDT" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelIdSDT, filterIdSDT, 0, 3,
							&HardValueSDT[0], &HardMaskSDT[0], &HardModeSDT[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter SDT" );

	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdSDT, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter SDT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdSDT, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel SDT" );

#endif
#if 1
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelIdNIT),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel NIT" );

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelIdNIT, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for SDT */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelIdNIT, &filterIdNIT ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter NIT" );
		
	mTBOX_PRINT((kTBOX_LF,"NIT ch=%d fl=%d on tbid=0x%x\r\n",
			 channelIdNIT,
			 filterIdNIT,
			 HardValueNIT[0]));

	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelIdNIT, 0x10 ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID NIT" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelIdNIT, filterIdNIT, 0, 3,
							&HardValueNIT[0], &HardMaskNIT[0], &HardModeNIT[0],eTDAL_DMX_CRC_IGNORE ),
							kTDAL_DMX_NO_ERROR, "DMX set filter NIT" );

	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdNIT, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter NIT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdNIT, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel NIT" );

	TKEL_DelayTask(ms2tick(5000));

/*
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdNIT, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter NIT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdNIT, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel NIT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdSDT, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter SDT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdSDT, eTDAL_DMX_CTRL_DISABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel SDT" );

	TKEL_DelayTask(ms2tick(5000));
*/
#endif

	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId[1], eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter TOT" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	/* TDT other acquisition  ==> may be failed accoding to the stream */
	
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId[0], eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control filter TDT" );
	
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestSectionSem, ms2tick(10000 * dmxTestSectionToWait) ),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId,eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

