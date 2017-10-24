/** \file stresstest.c
 *  \brief TDAL DMX TABLES SECTIONS TEST
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
#include "tdal_dmx.h"

#include "testhelper.h"
#include "dmxtests.h"
extern int gChannel;

/******************************************************
 *              Defines                               *
 ******************************************************/

static TKEL_sema_id dmxTestStressSectionSem = (TKEL_sema_id)NULL;
static TKEL_sema_id dmxTestStressSectionSemPriv = (TKEL_sema_id)NULL;

#define NB_FILTER 9

#define PAT_PID 0x00
#define NIT_PID 0x0010
#define BAT_PID 0x0011
#define SDT_PID 0x0011
#define EIT_PID 0x0012
#define TDT_PID 0x0014
#define TOT_PID 0x0014
#define DSMCC_PID 0x0044

#define PAT_FILTER 0x0
#define NIT_FILTER 0x40
#define BAT_FILTER 0x4A
#define SDTA_FILTER 0x42
#define SDTO_FILTER 0x46
#define EITA_FILTER 0x4E /* actual Ts, present/following event info */
#define EITO_FILTER 0x4F /* other Ts, present/following event info */
#define TDT_FILTER 0x70
#define TOT_FILTER 0x73

uint8_t EITAS_FILTER[16] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F}; /* actual Ts, event schedule info */
uint8_t EITOS_FILTER[16] = {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F}; /* other Ts, event schedule info */

#define kDMX_MAX_EIT_SVC  10
#define kDMX_MAX_EIT_TBID 4

static uint16_t dmxTestEitSvcId[kDMX_MAX_EIT_SVC];
static uint32_t dmxTestEitSectNumberMask[kDMX_MAX_EIT_SVC][kDMX_MAX_EIT_TBID][8];


static int dmxTestPesPacketToWait = 1;
static int dmxTestPesPacketRecv   = 0;
static int dmxTestSectionToWaitEIT = 1;
static int dmxTestSectionRecvEIT   = 0;
static int dmxTestSectionToWaitPriv = 1;
static int dmxTestSectionRecvPriv   = 0;

static TKEL_sema_id dmxTestPESSem     = (TKEL_sema_id)NULL;

static bool	dmxPesFilterThreadStop            = FALSE;
static bool dmxPesFilterThreadRunning         = FALSE;

static bool	dmxSectionFilterEITThreadStop     = FALSE;
static bool dmxSectionFilterEITThreadRunning  = FALSE;

static bool	dmxSectionFilterPrivThreadStop    = FALSE;
static bool dmxSectionFilterPrivThreadRunning = FALSE;

/******************************************************
 *             CallBack                               *
 *****************************************************/

static void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
                                            tTDAL_DMX_FilterId FilterId,
                                            uint32_t Filter_track,
                                            uint32_t Length,
                                            uint8_t *Buffer);

static void dmxTestGenericSectionNotifyEIT(tTDAL_DMX_ChannelId ChannelId,
										 tTDAL_DMX_FilterId FilterId,
										 uint32_t Filter_track,
										 uint32_t Length,
										 uint8_t *Buffer);

static void dmxTestGenericSectionNotifyPriv(tTDAL_DMX_ChannelId ChannelId,
											tTDAL_DMX_FilterId FilterId,
											uint32_t Filter_track,
											uint32_t Length,
											uint8_t *Buffer);

static void dmxTestGenericSectionNotifyDSMCC(tTDAL_DMX_ChannelId ChannelId,
											tTDAL_DMX_FilterId FilterId,
											uint32_t Filter_track,
											uint32_t Length,
											uint8_t *Buffer);

static void dmxTestSectionNotify(uint8_t* buffer, uint32_t length);

void ResquestFilter( tTDAL_DMX_ChannelId ChannelId,
              tTDAL_DMX_FilterId FilterId,
              uint32_t Filter_track,
              uint8_t size,
              uint8_t HardValue,
              uint8_t HardMask,
              uint8_t HardMode,
              uint8_t pid );

static void dmxTestSectionFilterEITThread(void *arg);
static void dmxTestSectionFilterPrivThread(void *arg);
static void dmxTestPesFilterThread(void *arg);
static void dmxTestPESNotify(tTDAL_DMX_ChannelId ChannelId,
							 tTDAL_DMX_FilterId  FilterId,
							 uint32_t            FilterTrack,
							 uint32_t            Length,
							 uint8_t*            Buffer);

/******************************************************
 *              DMX test tree                         *
 ******************************************************/


tTestNode gTDAL_DMX_StressTest = {
    "StressTest",
    "test the TDAL_DMX function input parameters",
    0, /* not interactive */
    DMX_StressTest,
    NULL, /* NO SON*/
};


static tTestNode gTDAL_DMX_StressTest2 = {
	"StressTest2",
	"Test filters section EIT/section Private/PES",
	0,
	DMX_StressTest2,
	NULL
};

static tTestNode gTDAL_DMX_StressTestDSMCC = {
	"StressTestDSMCC",
	"Test filters section EIT/section Private/PES",
	0,
	DMX_StressTestDSMCC,
	NULL
};

tTestNode* gTDAL_DMX_StressNodeList[] = {
	&gTDAL_DMX_StressTest,
	&gTDAL_DMX_StressTest2,
	&gTDAL_DMX_StressTestDSMCC,
	NULL
};

tTestNode gTDAL_DMX_StressTests = {
    "TDAL_DMX stress tests",
    "test the TDAL_DMX function in stress mode",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_DMX_StressNodeList
};

/*! \fn void DMX_StressTest( void )
 *  \brief test stress
 *
 */

void DMX_StressTest( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;
    tTDAL_DMX_FilterId       filterId[NB_FILTER + 32 ];
    uint32_t filter_track = 0;
    uint8_t size = 1;
    uint8_t pid[NB_FILTER] = { PAT_PID, NIT_PID, BAT_PID, SDT_PID, SDT_PID, EIT_PID, EIT_PID, TDT_PID, TOT_PID };
    uint8_t HardValue[NB_FILTER] = { PAT_FILTER, NIT_FILTER, BAT_FILTER, SDTA_FILTER, SDTO_FILTER, EITA_FILTER, EITO_FILTER, TDT_FILTER, TOT_FILTER };
    uint8_t HardMask[NB_FILTER] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t HardMode[NB_FILTER] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    tTDAL_DMX_ChannelCallbackFct callback;
    uint8_t cpt, i, nbf;

    tTestManagerChrono  chrono;
    long                elapsed_time = 0;

    TestManager_StartChrono(&chrono);


    TestHelper_Initialize_TDAL_DMD();
    TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

    TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestStressSectionSem),
                             TKEL_NO_ERR,
                             "TKEL CreateSemaphore" );

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
    type.eChannelOutput = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx], &type, &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    callback = dmxTestGenericSectionNotify;

    TestManager_AssertEqual(
            TDAL_DMX_Register_Channel_Callback( channelId, callback ),
            kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );


    nbf = 0;
    for( cpt=0; cpt < NB_FILTER; cpt++ )
    {
        TestManager_AssertEqual(
                TDAL_DMX_Allocate_Filter( channelId, &filterId[ nbf ] ),
                kTDAL_DMX_NO_ERROR,
                "DMX allocate filter" );

        filter_track = nbf;

        ResquestFilter( channelId, filterId[ nbf ], filter_track, size, HardValue[cpt],                    HardMask[cpt],HardMode[cpt], pid[cpt] );

	TestManager_AssertEqual( TDAL_DMX_Free_Filter( filterId[ nbf ] ),
							kTDAL_DMX_NO_ERROR, "DMX Free Filter" );

        if( HardValue[cpt] == EITO_FILTER )
        {
            for( i=0; i < 16; i++ )
            {
                TestManager_AssertEqual(
                TDAL_DMX_Allocate_Filter( channelId, &filterId[ nbf ] ),
                kTDAL_DMX_NO_ERROR,
                "DMX allocate filter" );

                ResquestFilter( channelId, filterId[ nbf ], filter_track, size,                            EITAS_FILTER[i], HardMask[cpt],HardMode[cpt],                       pid[cpt] );

                ResquestFilter( channelId, filterId[ nbf ], filter_track, size,                            EITOS_FILTER[i], HardMask[cpt],HardMode[cpt],                       pid[cpt] );
                nbf++;
            }
        }
        nbf++;
    }
    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

	TestHelper_Terminate_TDAL_DMD();
    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );

    TestManager_StopChrono(&chrono, &elapsed_time);

    mTBOX_PRINT((kTBOX_LF, "Time elapsed between the start and all pid filtering : %ld ms\n", elapsed_time));
}

void ResquestFilter( tTDAL_DMX_ChannelId channelId,
              tTDAL_DMX_FilterId filterId,
              uint32_t filter_track,
              uint8_t size,
              uint8_t hardValue,
              uint8_t hardMask,
              uint8_t hardMode,
              uint8_t pid )
{

    tTestManagerChrono  chrono;
    long                elapsed_time = 0;

    TestManager_StartChrono(&chrono);

    TestManager_AssertEqual(
        TDAL_DMX_Set_Channel_PID( channelId, pid ),
        kTDAL_DMX_NO_ERROR,
        "DMX Set Channel PID" );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Filter( channelId,
                             filterId,
                             filter_track,
                             size,
                             &hardValue,
                             &hardMask,
                             &hardMode,eTDAL_DMX_CRC_IGNORE ),
                             kTDAL_DMX_NO_ERROR,
                             "DMX set filter" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
            kTDAL_DMX_NO_ERROR,
            "DMX control channel" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
            kTDAL_DMX_NO_ERROR,
            "DMX control filter" );


#if 1
	mTBOX_PRINT((kTBOX_LF,"Wait 50s...\r\n"));
	TestManager_AssertEqual( TKEL_WaitSemaphoreTimeout( dmxTestStressSectionSem, ms2tick(50000) ),
							TKEL_NO_ERR,
							 "TKEL Wait Semaphore" );
#else
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestStressSectionSem ),
							TKEL_NO_ERR,
							 "TKEL Wait Semaphore" );
#endif

    TestManager_AssertEqual(
            TDAL_DMX_Control_Channel (channelId,eTDAL_DMX_CTRL_DISABLE),
            kTDAL_DMX_NO_ERROR, "DMX control channel" );

    TestManager_StopChrono(&chrono, &elapsed_time);

    mTBOX_PRINT((kTBOX_LF, "Time elapsed between pid filtering and notification: %ld ms\n", elapsed_time));
}






/*! \fn void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId, tTDAL_DMX_FilterId FilterId, uint32_t Filter_track, uint32_t Length, uint8_t *Buffer)
 *  \brief test Callback
 *
 */

static void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
                                            tTDAL_DMX_FilterId FilterId,
                                            uint32_t Filter_track,
                                            uint32_t Length,
                                            uint8_t *Buffer)
{
    TKEL_err        tkError;

    mTBOX_PRINT((kTBOX_LF,"Section received on filter %d \n",FilterId ));

    /*  check the section received and print info  */
    dmxTestSectionNotify(Buffer,Length);

    /*  free the TDAL buffer  */
    if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
    }

    tkError = TKEL_SignalSemaphore(dmxTestStressSectionSem);
    if (tkError != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tkError ));
    }

}

/*! \fn void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId, tTDAL_DMX_FilterId FilterId, uint32_t Filter_track, uint32_t Length, uint8_t *Buffer)
 *  \brief test Callback
 *
 */

static void dmxTestGenericSectionNotifyPriv(tTDAL_DMX_ChannelId ChannelId,
                                            tTDAL_DMX_FilterId FilterId,
                                            uint32_t Filter_track,
                                            uint32_t Length,
                                            uint8_t *Buffer)
{
	dmxTestSectionRecvPriv++;
	
	if (dmxTestSectionRecvPriv % 10 == 0)
	{
		printf("Private cpt : %d \r\n",dmxTestSectionRecvPriv);
	}

/*	mTBOX_PRINT((kTBOX_LF,"Priv SEC[%d/%d]->ch=%d flt=%d track=%d size=%d data[0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X]\r\n",
			 dmxTestSectionRecvPriv,
			 dmxTestSectionToWaitPriv,
			 ChannelId,
			 FilterId,
			 Filter_track,
			 Length,
			 Buffer[0],Buffer[1],Buffer[2],Buffer[3],
			 Buffer[4],Buffer[5],Buffer[6],Buffer[7]));
*/
    /*  free the TDAL buffer  */
    if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
    }
}

static void dmxTestGenericSectionNotifyDSMCC(tTDAL_DMX_ChannelId ChannelId,
                                            tTDAL_DMX_FilterId FilterId,
                                            uint32_t Filter_track,
                                            uint32_t Length,
                                            uint8_t *Buffer)
{
    TKEL_err tkError;
    static int lastModuleID = -1;
    static int lastBlockNb = -1;

    int moduleId = (int)(Buffer[20] << 8 | Buffer[21]);
    int BlockNb  = (int)(Buffer[24] << 8 | Buffer[25]);

    /* test module */
    if (lastModuleID != -1)
    {
        if ((moduleId != lastModuleID + 1) && (lastModuleID != 16 && moduleId != 1))
        {
            mTBOX_PRINT((kTBOX_LF,"ERROR module: %u %u\n", lastModuleID, moduleId));
            TestManager_AssertEqual(1, 0, "Bad Module number: Section missing?");
        }
    }
    lastModuleID = moduleId;

    /* test block */
    if (lastBlockNb != -1)
    {
        if ((BlockNb != (lastBlockNb + 1)%17) && (BlockNb != lastBlockNb))
        {
            mTBOX_PRINT((kTBOX_LF, "ERROR block: %u %u\n", lastBlockNb, BlockNb));
            TestManager_AssertEqual(1, 0, "Bad Block number: Section missing?");
        }
    }
    lastBlockNb = BlockNb;

    /* check data */
    if (Length == 4096)
    {
        int Counter;
        int TestResult = 0;

        if (BlockNb != 0)
        {
            for (Counter = 0; Counter < 4066; Counter++)
            {
                /* all data are identical */
                if (Buffer[Counter+26] != Buffer[26])
                {
                    TestResult++;
                }
            }
        }
        else
        {
            for (Counter = 0; Counter < 4052; Counter++)
            {
                /* all data are identical - first byte modified by genius */
                if (Buffer[Counter+40] != Buffer[40])
                {
                    TestResult++;
                }
            }
        }

    	dmxTestSectionRecvPriv++;
        TestManager_AssertEqual(TestResult, 0, "Section Pattern Verified");

       	if (TestResult != 0)
       	{
        	mTBOX_PRINT((kTBOX_LF,"*** ERROR *** DSMCC[%d/%d]->size=%04d ModuleId=%04d BlockNb=%04d -> Nb error:%u\r\n",
			    dmxTestSectionRecvPriv,
			    dmxTestSectionToWaitPriv,
			    Length, moduleId, BlockNb,
			    TestResult));
        }
        else
       	{
        	/*mTBOX_PRINT((kTBOX_LF,"DSMCC[%d/%d]->size=%04d ModuleId=%04d BlockNb=%04d->Data OK\r\n",
			    dmxTestSectionRecvPriv,
			    dmxTestSectionToWaitPriv,
			    Length, moduleId, BlockNb));*/
        }
    }

    /*  free the TDAL buffer  */
    if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
    }

	if(dmxTestSectionRecvPriv >= dmxTestSectionToWaitPriv)
	{
		tkError = TKEL_SignalSemaphore(dmxTestStressSectionSem);
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
        if (buffer[0] == 0x4A)
        {
                strcat(outStr, "BAT |");
        }
        if (buffer[0] == 0x4E)
        {
                strcat(outStr, "EITa |");
        }
        if (buffer[0] == 0x4F)
        {
                strcat(outStr, "EITo |");
        }
        if( (buffer[0] >= 0x50) && (buffer[0] <= 0x5F) )
        {
                strcat(outStr, "EITaS |");
        }
        if( (buffer[0] >= 0x60) && (buffer[0] <= 0x6F) )
        {
                strcat(outStr, "EIToS |");
        }
        for (i=0; i<10; i++)
        {
                sprintf(&outStr[strlen(outStr)], " 0x%02x", buffer[i]);
        }


        /* mTBOX_PRINT((kTBOX_LF,"sn-> %s\r\n",outStr )); */

}

void DMX_StressTest2(void)
{
    TKEL_err        tk_err;
    TKEL_task_id    tk_section_EIT_task;
    TKEL_task_id    tk_section_priv_task;
    TKEL_task_id    tk_pes_task;


/* use stream BBC_270905.TS */	
	/* Lock a signal */
	TestHelper_Initialize_TDAL_DMD();
	TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

#if 1
	/* Add a EIT Section task filtering process*/
	dmxSectionFilterEITThreadStop    = FALSE;
	dmxSectionFilterEITThreadRunning = TRUE;

	tk_err = TKEL_CreateTask(32,
							 "dmxTestSectionFilterEIT",
							 4096,
							 dmxTestSectionFilterEITThread,
							 NULL,
							 &tk_section_EIT_task );

	TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) ,
							"EIT filter thread" );
	
#endif
#if 1
	/* Add a Priv Section task filtering process*/
	dmxSectionFilterPrivThreadStop    = FALSE;
	dmxSectionFilterPrivThreadRunning = TRUE;

	tk_err = TKEL_CreateTask(32,
							 "dmxTestSectionFilterPriv",
							 4096,
							 dmxTestSectionFilterPrivThread,
							 NULL,
							 &tk_section_priv_task );

	TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) ,
							"Private filter thread" );
#endif
#if 1
	/* Add a PES task filtering process*/
	dmxPesFilterThreadStop    = FALSE;
	dmxPesFilterThreadRunning = TRUE;

	tk_err = TKEL_CreateTask(32,
							 "dmxTestPesFilterThread",
							 4096,
							 dmxTestPesFilterThread,
							 NULL,
							 &tk_pes_task );

	TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) ,
							"PES filter thread" );
#endif


TKEL_DelayTask( ms2tick(4*60*1000) );

tk_err = TKEL_SignalSemaphore(dmxTestStressSectionSem);
if (tk_err != TKEL_NO_ERR)
{
		mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tk_err ));
}

tk_err = TKEL_SignalSemaphore(dmxTestStressSectionSemPriv);
if (tk_err != TKEL_NO_ERR)
{
		mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tk_err ));
}

tk_err = TKEL_SignalSemaphore(dmxTestPESSem);
if (tk_err != TKEL_NO_ERR)
{
		mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tk_err ));
}

TKEL_DelayTask( ms2tick(2000) );


    while(( dmxPesFilterThreadRunning == TRUE ) &&
		  ( dmxSectionFilterEITThreadRunning == TRUE ) &&
		  ( dmxSectionFilterPrivThreadRunning == TRUE ))
		
    {
        TKEL_DelayTask( ms2tick(1000) );
    }



    tk_err = TKEL_DeleteTask( tk_section_EIT_task );
	TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) , "EIT filter thread delete" );

	TKEL_DeleteTask( tk_section_priv_task );
	TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) , "Private filter thread delete" );

	TKEL_DeleteTask( tk_pes_task );
	TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) , "PES filter thread delete" );

	TestManager_AssertEqual(dmxTestSectionToWaitEIT ,
							dmxTestSectionRecvEIT,
							"EIT filter counter" );
	TestManager_AssertEqual(dmxTestSectionToWaitPriv ,
							dmxTestSectionRecvPriv,
							"Private filter counter" );
	TestManager_AssertEqual(dmxTestPesPacketToWait ,
							dmxTestPesPacketRecv,
							"PES filter counter" );

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
	TestHelper_Terminate_TDAL_DMD();
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

static void dmxTestSectionFilterPriv( void )
{
	int                     i;
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelId;
	tTDAL_DMX_FilterId      filterId[2];

	uint16_t                PidPriv = 0x28a;
	uint8_t                 HardValue[2] = {0x3B, 0x3C}; /* table ID */
	
	uint8_t                 HardMask[2]  = {0x00, 0xFF};
	uint8_t                 HardMode[2]  = {0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelCallbackFct callback;

	callback = dmxTestGenericSectionNotifyPriv;
	dmxTestSectionToWaitPriv = 9413;
	dmxTestSectionRecvPriv   = 0;


	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestStressSectionSemPriv),
			TKEL_NO_ERR,
			"TKEL CreateSemaphore" );
	
	type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
	
	/* Private section */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId),
							 kTDAL_DMX_NO_ERROR,
							 "DMX allocate channel" );

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId,
																 callback ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX register Channel EIT CallBack" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, PidPriv ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Set Channel EIT PID" );
	/* Allocate a filters  */
	for(i = 0; i < 1; i++)
	{
		TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId,
														   &filterId[i] ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX allocate filter" );
		TestManager_AssertEqual( TDAL_DMX_Set_Filter( channelId,
													  filterId[i],
													  0,
													  1,
													  &HardValue[i],
													  &HardMask[i],
													  &HardMode[i],eTDAL_DMX_CRC_IGNORE ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX set filter" );
	}

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId,
													   eTDAL_DMX_CTRL_ENABLE ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel" );
	for(i = 0; i < 1; i++)
	{
		TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId[i],
														  eTDAL_DMX_CTRL_ENABLE ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX control filter" );
	}
	
	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestStressSectionSemPriv),
							 TKEL_NO_ERR,
							 "TKEL Wait Semaphore" );


	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId,
													   eTDAL_DMX_CTRL_DISABLE),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Free Channel" );

	dmxSectionFilterPrivThreadStop = TRUE;

}

static void dmxTestSectionFilterEIT( void )
{
	int                     i;
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelIdEIT;
	tTDAL_DMX_FilterId      filterIdEIT[4];
	uint16_t                PidEIT = 0x12;
	uint8_t                 HardValueEIT[4] = {0x50, 0x60, 0x51, 0x61}; /* table ID */
	uint8_t                 HardMaskEIT[4]  = {0x00, 0x60, 0xFF, 0xFF};
	uint8_t                 HardModeEIT[4]  = {0xFF, 0xFF, 0xFF, 0xFF}; /* Set to 0 for negative filtering */
	tTDAL_DMX_ChannelCallbackFct callback;
	uint8_t svc, tbid ;

	callback = dmxTestGenericSectionNotifyEIT;
	dmxTestSectionToWaitEIT = 16736;
	dmxTestSectionRecvEIT   = 0;


	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestStressSectionSem),
			TKEL_NO_ERR,
			"TKEL CreateSemaphore" );
	
	for(svc = 0; svc < kDMX_MAX_EIT_SVC; svc++)
	{
		dmxTestEitSvcId[svc] = 0;
		for(tbid = 0; tbid < kDMX_MAX_EIT_TBID; tbid++)
		{
			dmxTestEitSectNumberMask[svc][tbid][0] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][1] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][2] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][3] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][4] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][5] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][6] = 0x00000000;
			dmxTestEitSectNumberMask[svc][tbid][7] = 0x00000000;
		}
	}

	type.eChannelStream     = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	/* EIT */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
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
	for(i = 0; i < 1; i++)
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
													  &HardModeEIT[i],eTDAL_DMX_CRC_IGNORE ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX set filterEIT" );
	}

	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelIdEIT,
													   eTDAL_DMX_CTRL_ENABLE ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel EIT" );
	for(i = 0; i < 1; i++)
	{
		TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterIdEIT[i],
														  eTDAL_DMX_CTRL_ENABLE ),
								 kTDAL_DMX_NO_ERROR,
								 "DMX control filter EIT" );
	}

	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestStressSectionSem),
							 TKEL_NO_ERR,
							 "TKEL Wait Semaphore" );


	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelIdEIT,
													   eTDAL_DMX_CTRL_DISABLE),
							 kTDAL_DMX_NO_ERROR,
							 "DMX control channel EIT" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelIdEIT ),
							 kTDAL_DMX_NO_ERROR,
							 "DMX Free Channel EIT" );
	dmxSectionFilterEITThreadStop  = TRUE;

#if 1
	for(svc = 0; svc < kDMX_MAX_EIT_SVC; svc++)
	{
		if(dmxTestEitSvcId[svc] != 0)
		{
			mTBOX_PRINT((kTBOX_LF,"[ SVC ][EIT ]=[31    28      24      20      16      12       8       4       0]\r\n"));
			for(tbid = 0; tbid < kDMX_MAX_EIT_TBID; tbid++)
			{
				mTBOX_PRINT((kTBOX_LF,"[%05d][0x%x]=[%08x%08x%08x%08x%08x%08x%08x%08x]\r\n",
							 dmxTestEitSvcId[svc],HardValueEIT[tbid],
							 dmxTestEitSectNumberMask[svc][tbid][7],
							 dmxTestEitSectNumberMask[svc][tbid][6],
							 dmxTestEitSectNumberMask[svc][tbid][5],
							 dmxTestEitSectNumberMask[svc][tbid][4],
							 dmxTestEitSectNumberMask[svc][tbid][3],
							 dmxTestEitSectNumberMask[svc][tbid][2],
							 dmxTestEitSectNumberMask[svc][tbid][1],
							 dmxTestEitSectNumberMask[svc][tbid][0]));
			}

		}
	}
#endif
}

static void dmxTestGenericSectionNotifyEIT(tTDAL_DMX_ChannelId ChannelId,
										 tTDAL_DMX_FilterId FilterId,
										 uint32_t Filter_track,
										 uint32_t Length,
										 uint8_t *Buffer)
{
	tTestHelper_SectionHeader Header;
	uint32_t IndexMask;
	uint32_t BitMask;
	uint8_t svc, svcFound=0;
	bool bFound = FALSE;
	uint32_t IndexTbId;
	
	dmxTestSectionRecvEIT++;

	if (dmxTestSectionRecvEIT % 10 == 0)
	{	
		printf("EIT cpt : %d \r\n",dmxTestSectionRecvEIT);
	}

	/* EIT tableIds */
	if((Buffer[0] == 0x50) ||
	   (Buffer[0] == 0x51) ||
	   (Buffer[0] == 0x60) ||
	   (Buffer[0] == 0x61))
	{
		TestHelper_DVBHeaderExtract(Buffer,  &Header);

		/* search if service already acquired */
		for(svc = 0; svc < kDMX_MAX_EIT_SVC; svc++)
		{
			if(dmxTestEitSvcId[svc] == Header.StreamId)
			{
				svcFound = svc;
				bFound = TRUE;
			}
		}

		if(bFound == FALSE)
		{
			/* search empty space */
			for(svc = 0; svc < kDMX_MAX_EIT_SVC; svc++)
			{
				if(dmxTestEitSvcId[svc] == 0)
				{
					svcFound = svc;
					bFound = TRUE;
				}
			}
		}

		if(bFound == TRUE)
		{
			dmxTestEitSvcId[svcFound] = Header.StreamId;

			IndexMask = Header.SectionNumber / 32;
			BitMask   = Header.SectionNumber % 32;
			IndexTbId = ((Buffer[0] & 0x20) >> 4) + (Buffer[0] & 0x01);

			
/*			mTBOX_PRINT((kTBOX_LF,"EIT SEC[%d/%d]->svc[%d]=%d SN=0x%x EIT[idx=0x%x mask=0x%x]\r\n",
						 dmxTestSectionRecvEIT,
						 dmxTestSectionToWaitEIT,
						 svcFound,
						 dmxTestEitSvcId[svcFound],	
						 Header.SectionNumber,
						 IndexMask,
						 BitMask));
*/			
			dmxTestEitSectNumberMask[svcFound][IndexTbId][IndexMask] |= (1 << BitMask);
		}
	}
	else
	{
/*		mTBOX_PRINT((kTBOX_LF,"SEC[%d/%d]->ch=%d flt=%d track=%d size=%d data[0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X]\r\n",
					 dmxTestSectionRecvEIT,
					 dmxTestSectionToWaitEIT,
					 ChannelId,
					 FilterId,
					 Filter_track,
					 Length,
					 Buffer[0],Buffer[1],Buffer[2],Buffer[3],
					 Buffer[4],Buffer[5],Buffer[6],Buffer[7]));
*/
		/*  check the section received and print info  */
		dmxTestSectionNotify(Buffer,Length);
	}

	/*  free the TDAL buffer  */
	if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
	{
		mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
	}
}


/*! \fn void dmxTestPesFilterSet( void )
 *  \brief check PES packet filtering
 *
 *  This test locks a signal and configures demux.
 *  It sets a filter to retrieve PES packet.
 */
static void dmxTestPesFilterSet( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	tTDAL_DMX_ChannelCallbackFct callback;
	/* parameters BBC1 service of BBC_270905.TS stream file */
	uint16_t                PidPes = 0x25B;

	/* Create a semaphore for the callback */
	TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestPESSem),
							TKEL_NO_ERR,
							"TKEL CreateSemaphore" );
	
	/* Do DMX stuff */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	/* get subtitle pes */
	type.eChannelStream     = eTDAL_DMX_SUBTITLE_STREAM;
	type.eChannelOutput     = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

	callback = dmxTestPESNotify;

	dmxTestPesPacketToWait = 199; /*400;*/
	dmxTestPesPacketRecv   = 0;

	TestManager_AssertEqual( TDAL_DMX_Register_Channel_Callback( channelId, callback ),
							kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	
	/* Allocate a filter for PES and another for version_number */
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, PidPes  ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
								kTDAL_DMX_NO_ERROR, "DMX control filter" );
	
	mTBOX_PRINT((kTBOX_LF, "Waiting for PES packet(s)...\n"));

	TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestPESSem),
							TKEL_NO_ERR, "TKEL Wait Semaphore" );

	TestManager_AssertEqual( TDAL_DMX_Control_Channel (channelId,eTDAL_DMX_CTRL_DISABLE),
							kTDAL_DMX_NO_ERROR, "DMX control channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );	

	dmxPesFilterThreadStop = TRUE;
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
	int      iLength;
	bool     bErr = TRUE;

	dmxTestPesPacketRecv++;
	
	printf("PES cpt : %d \r\n",dmxTestPesPacketRecv);
	
	/* PES packet: check start code prefix */
	if((Buffer[0] == 0x00) &&
	   (Buffer[1] == 0x00) &&
	   (Buffer[2] == 0x01))
	{
		/*mTBOX_PRINT((kTBOX_LF,"PES start code prefix OK\n"));*/
		bErr = TRUE;
	}
	else
	{
		mTBOX_PRINT((kTBOX_LF,"PES start code prefix error[0x%X%X%X]\n",
					 Buffer[0],Buffer[1],Buffer[2]));
		bErr = FALSE;
	}

	/* PES packet: length */
	iLength  = 0;
	iLength  = (Buffer[4] << 8) & 0xFF00;
	iLength += Buffer[5] & 0xFF;

/*	mTBOX_PRINT((kTBOX_LF,"PES[%d/%d]->ch=%d flt=%d size=%d data[0x%X,0x%X,0x%X,0x%X,0x%X,0x%X] stream_id[0x%x] PES length[%d]\r\n",
				 dmxTestPesPacketRecv,
				 dmxTestPesPacketToWait,
				 ChannelId,
				 FilterId,
				 Length,
				 Buffer[0],Buffer[1],Buffer[2],Buffer[3],Buffer[4],Buffer[5],
				 Buffer[3],
				 iLength));	
*/

	if (TDAL_DMX_Free_Buffer(Buffer) != kTDAL_DMX_NO_ERROR)
	{
		mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
		bErr = FALSE;
	}
}

static void dmxTestSectionFilterEITThread( void * arg )
{
    while( dmxSectionFilterEITThreadStop == FALSE )
    {
		dmxTestSectionFilterEIT();
	}

	dmxSectionFilterEITThreadRunning = FALSE;
}

static void dmxTestPesFilterThread( void * arg )
{
    while( dmxPesFilterThreadStop == FALSE )
    {
		dmxTestPesFilterSet();
	}

	dmxPesFilterThreadRunning = FALSE;
}

static void dmxTestSectionFilterPrivThread( void * arg )
{
    while( dmxSectionFilterPrivThreadStop == FALSE )
    {
		dmxTestSectionFilterPriv();
	}

	dmxSectionFilterPrivThreadRunning = FALSE;
}

void DMX_StressTestDSMCC( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId channelId;
    tTDAL_DMX_FilterId filterId;
    uint8_t ucFilter = 0x3C;    /* only DDB */
    uint8_t ucMask   = 0xFF;
    uint8_t ucMode   = 0xFF;    /* positif mode */

    tTestManagerChrono  chrono;
    long  elapsed_time = 0;

    TestManager_StartChrono(&chrono);

    /* use player with stream multiplexed by genius */
    /* bin file is in Y:\test\chal\chaltests\data\DDBoutput.bin */
    gChannel = 4;

    TestHelper_Initialize_TDAL_DMD();
    TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

    TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestStressSectionSem),
                             TKEL_NO_ERR,
                             "TKEL CreateSemaphore" );

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    /* same as mhg_si_api.c */
    type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
    type.eChannelOutput = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_HIGHT_MEMORY_SIZE;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx], &type, &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    TestManager_AssertEqual(
            TDAL_DMX_Register_Channel_Callback( channelId, dmxTestGenericSectionNotifyDSMCC ),
            kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );
	dmxTestSectionToWaitPriv = 256;
	dmxTestSectionRecvPriv   = 0;

    TestManager_AssertEqual(
            TDAL_DMX_Allocate_Filter( channelId, &filterId ),
            kTDAL_DMX_NO_ERROR,
            "DMX allocate filter" );

    ResquestFilter( channelId, filterId, 0/*filter_track*/, 1/*size*/, ucFilter, ucMask, ucMode, DSMCC_PID);

    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

	TestHelper_Terminate_TDAL_DMD();
    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );

    TestManager_StopChrono(&chrono, &elapsed_time);

    mTBOX_PRINT((kTBOX_LF, "Time elapsed between the start and all pid filtering : %ld ms\n", elapsed_time));
}
