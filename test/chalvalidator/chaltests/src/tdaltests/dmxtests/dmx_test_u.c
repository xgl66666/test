/** \file dmx_test_u.c
 *  \brief Unitary Tests for the DMX layer of the TDAL API
 *  \author Mario Bensi
 *  \author Olivier DOLE
 *  \date 04/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_dmx.h"

#include "testhelper.h"
#include "dmxtests.h"
/******************************************************
 *              Defines                               *
 ******************************************************/

/******************************************************
 *              DMX test tree                         *
 ******************************************************/

static tTestNode gTestUInitTerminate = {
	"TestUInitTerminate",
	"test TDAL_DMX_Init() and TDAL_DMX_Terminate() functions",
	0, /* not interactive */
	DMX_TestUnitary_InitTerm,
	NULL
};

static tTestNode gTestUChanAllocNoInit = {
	"TestUChanAllocNoInit",
	"test the allocation of a channel with no previous initialization",
	0,
	DMX_TestUnitary_AllocateChanNoInit,
	NULL
};

static tTestNode gTestUChanAllocMax = {
    "TestUChanAllocMax",
    "Test the maximum channel allocation",
    0,
    DMX_TestUnitary_AllocateChanMax,
    NULL
};

static tTestNode gTestUFreeChanBadId = {
    "TestUFreeChanBadId",
    "Test of free channel with wrong channel id",
    0,
    DMX_TestUnitary_FreeChanBadId,
    NULL
};

static tTestNode gTestURegisterCallBackBadParam = {
    "TestURegisterCallBackBadParam",
    "Test of register callback with bad callback parameter",
    0,
    DMX_TestUnitary_RegisterCallBackBadParam,
    NULL
};

static tTestNode gTestURegisterCallBackGoodParam = {
    "TestURegisterCallBackGoodParam",
    "Test of register callback with good callback parameter",
    0,
    DMX_TestUnitary_RegisterCallBackGoodParam,
    NULL
};

static tTestNode gTestUSetChanPidGoodParam = {
    "TestUSetChanPidGoodParam",
    "Test of set channel PID with good parameter",
    0,
    DMX_TestUnitary_SetChanPidGoodParam,
    NULL
};

static tTestNode gTestUSetChanPidBadParam = {
    "TestUSetChanPidBadParam",
    "Test of set channel PID with bad parameter",
    0,
    DMX_TestUnitary_SetChanPidBadParam,
    NULL
};

#if 0 //Currently not used
static tTestNode gTestUSetChanPidAlreadyUsed = {
    "TestUSetChanPidAlreadyUsed",
    "Test of set channel PID with PID already used",
    0,
    DMX_TestUnitary_SetChanPidAlreadyUsed,
    NULL
};
#endif

static tTestNode gTestUAllocateFilterBadChannel = {
    "TestUAllocateFilterBadChannel",
    "Test of allocate filter for bad channel id",
    0,
    DMX_TestUnitary_AllocateFilterBadChannel,
    NULL
};

static tTestNode gTestUAllocateFilterMax = {
    "TestUAllocateFilterMax",
    "Test the maximum filter allocation",
    0,
    DMX_TestUnitary_AllocateFilterMax,
    NULL
};

static tTestNode gTestUSetFilterBadFilter = {
    "TestUSetFilterBadFilter",
    "Test of set filter with bad filter id",
    0,
    DMX_TestUnitary_SetFilterBadFilter,
    NULL
};

static tTestNode gTestUSetFilterBadParam = {
    "TestUSetFilterBadParam",
    "Test of set filter with bad parameter",
    0,
    DMX_TestUnitary_SetFilterBadParam,
    NULL
};

static tTestNode gTestUControlChanBadChannel = {
    "TestUControlChanBadChannel",
    "Test of control channel with bad channel id",
    0,
    DMX_TestUnitary_ControlChanBadChannel,
    NULL
};

static tTestNode gTestUControlChanBadParam = {
    "TestUControlChanBadParam",
    "Test of control channel with bad parameter",
    0,
    DMX_TestUnitary_ControlChanBadParam,
    NULL
};

static tTestNode gTestUControlChanGoodParam = {
    "TestUControlChanGoodParam",
    "Test of control channel with good parameter",
    0,
    DMX_TestUnitary_ControlChanGoodParam,
    NULL
};

static tTestNode gTestUControlFilterBadFilter = {
    "TestUControlFilterBadFilter",
    "Test of control filter with bad filter id",
    0,
    DMX_TestUnitary_ControlFilterBadFilter,
    NULL
};

static tTestNode gTestUControlFilterBadParam = {
    "TestUControlFilterBadParam",
    "Test of control filter with bad parameter",
    0,
    DMX_TestUnitary_ControlFilterBadParam,
    NULL
};

static tTestNode gTestUFreeUnusedFilter = {
    "TestUFreeUnusedFilter",
    "Test of free not used filter",
    0,
    DMX_TestUnitary_FreeUnusedFilter,
    NULL
};

static tTestNode gTestUFreeFilterNominalCase = {
    "TestUFreeFilterNominalCase",
    "Test of free filter in nominal case",
    0,
    DMX_TestUnitary_FreeFilterNominalCase,
    NULL
};

static tTestNode gTestUFreeUnusedChan = {
    "TestUFreeUnusedChan",
    "Test of free not used channel",
    0,
    DMX_TestUnitary_FreeUnusedChan,
    NULL
};


tTestNode* gDMXUnitaryTestNodeList[] = {
    &gTestUInitTerminate,
    &gTestUChanAllocNoInit,
    &gTestUChanAllocMax,
    &gTestUFreeChanBadId,
    &gTestURegisterCallBackBadParam,
    &gTestURegisterCallBackGoodParam,
    &gTestUSetChanPidGoodParam,
    &gTestUSetChanPidBadParam,
    /*&gTestUSetChanPidAlreadyUsed,*//* Avoid return since different channels can have the same pid */
    &gTestUAllocateFilterBadChannel,
    &gTestUAllocateFilterMax,
    &gTestUSetFilterBadFilter,
    &gTestUSetFilterBadParam,
    &gTestUControlChanBadChannel,
    &gTestUControlChanBadParam,
    &gTestUControlChanGoodParam,
    &gTestUControlFilterBadFilter,
    &gTestUControlFilterBadParam,
    &gTestUFreeUnusedFilter,
    &gTestUFreeFilterNominalCase,
    &gTestUFreeUnusedChan,
	NULL
};

tTestNode gTDAL_DMX_UnitaryTestSuite = {
	"TDAL_DMX Unitary Tests",
	"test the TDAL_DMX function input parameters",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDMXUnitaryTestNodeList
};


/******************************************************
 *             CallBack                               *
 *****************************************************/

static void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
                                            tTDAL_DMX_FilterId FilterId,
                                            uint32_t Filter_track,
                                            uint32_t Length,
                                            uint8_t *Buffer);

/******************************************************
 *              Unitary tests                         *
 ******************************************************/

/*! \fn void DMX_TestUnitary_InitTerm( void )
 *  \brief Test TDAL_DMX_Init() and TDAL_DMX_Terminate() functions
 *
 *  Test the following scenarii:
 *  Twice Init and one Terminate
 *  One Init and two Terminate
 *  Init and Terminate
 */
void DMX_TestUnitary_InitTerm()
{
	/* Twice Init and one Terminate */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init a second time" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

	/* One Init and two Terminate */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate a second time" );

	/* Init and Terminate */
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_AllocateChanNoInit( void )
 *  \brief Test DMX module with a channel allocatio without
 *  previous initialization
 */
void DMX_TestUnitary_AllocateChanNoInit()
{
	tTDAL_DMX_DemuxId		demuxId;
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId		channelId;
	
	demuxId = gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx];
	type.eChannelStream = eTDAL_DMX_PCR_STREAM;
	type.eChannelOutput = eTDAL_DMX_DECODER;
	type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;
			
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel(demuxId, &type, &channelId),
							kTDAL_DMX_ERROR_NOT_INITIALIZED, "DMX allocate channel" );
}

/*! \fn void DMX_TestUnitary_AllocateChanMax( void )
 *  \brief Test the maximum channel allocation
 *  \todo Creation of a accessor on maximum channel in tdal dmx to complete this test.
 */

void DMX_TestUnitary_AllocateChanMax()
{
    uint8_t                  nbDemuxId;
    uint8_t                  cpt = 0;
    tTDAL_DMX_DemuxId       *demuxId;
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId     channelId[100];
    tTDAL_DMX_Error         error = kTDAL_DMX_NO_ERROR;

    uint8_t i;

    for( i=0; i<100; i++ )
    {
        channelId[i] = 0;
    }

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    TestManager_AssertEqual(
            TDAL_DMX_GetDemuxId( &nbDemuxId, &demuxId ),
            kTDAL_DMX_NO_ERROR,
            "DMX GetDemuxId" );

    mTBOX_PRINT( ( kTBOX_LF, "nbr demux : %d \n", nbDemuxId ) );

    type.eChannelStream = eTDAL_DMX_PCR_STREAM;
    type.eChannelOutput = eTDAL_DMX_DECODER;
    type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;

    while( error == kTDAL_DMX_NO_ERROR )
    {
        error = TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
										   &type,
										   &channelId[cpt] );
        mTBOX_PRINT( ( kTBOX_LF, "cpt : %d channel id = %d\n", cpt, channelId[cpt] ) );
		if(error == kTDAL_DMX_NO_ERROR)
		{
			cpt++;
		}
    }

    TestManager_AssertEqual(TDAL_DMX_Allocate_Channel(gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
													  &type,
													  &channelId[cpt]),
							kTDAL_DMX_ERROR_TOO_MANY_REQ,
							"DMX allocate channel error");

	/* decrement before freeing because last allocation is invalid */
	cpt--;

    while( cpt > 0 )
    {
        if( channelId[cpt] != 0 )
        {
            TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId[cpt] ),
                                   kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
        }
        cpt--;
    }

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_FreeChanBadId( void )
 *  \brief Test the desallocation of a channel with a wrong ID
 *
 *  To be more precise the test allocates a channel and then free it
 *  with a bad ID. So TDAL_DMX_Free_Channel() must fail. To finish the
 *  test the channel is free with the correct ID as parameter.
 */

void DMX_TestUnitary_FreeChanBadId( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelId;

	type.eChannelStream = eTDAL_DMX_PCR_STREAM;
	type.eChannelOutput = eTDAL_DMX_DECODER;
	type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;
	
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel(gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
													   &type,
													   &channelId),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( -1 ),
							kTDAL_DMX_ERROR_INVALID_CHANNEL_ID, "DMX Free Channel with wrong ID" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel with correct ID" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_RegisterCallBackBadParam( void )
 *  \brief Test of register callback with bad callback parameter
 *
 *  Test the following scenarii:
 *  Register Channel CallBack with a bad ChannelId
 *  Register Channel CallBack with a bad CallBack Parameter
 */

void DMX_TestUnitary_RegisterCallBackBadParam( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId = -1;
    tTDAL_DMX_ChannelCallbackFct callback;


    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );


    type.eChannelStream = eTDAL_DMX_PCR_STREAM;
    type.eChannelOutput = eTDAL_DMX_DECODER;
    type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;


    callback = dmxTestGenericSectionNotify;

    TestManager_AssertEqual(
            TDAL_DMX_Register_Channel_Callback( channelId, callback ),
            kTDAL_DMX_ERROR_INVALID_CHANNEL_ID, "DMX register Channel CallBack" );


    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    TestManager_AssertEqual(
            TDAL_DMX_Register_Channel_Callback( channelId, 0 ),
            kTDAL_DMX_ERROR_BAD_PARAMETERS, "DMX register Channel CallBack" );


    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_RegisterCallBackGoodParam( void )
 *  \brief Test of register callback with good callback parameter
 *
 */

void DMX_TestUnitary_RegisterCallBackGoodParam( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;
    tTDAL_DMX_ChannelCallbackFct callback;


    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_PCR_STREAM;
    type.eChannelOutput = eTDAL_DMX_DECODER;
    type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );


    callback = dmxTestGenericSectionNotify;

    TestManager_AssertEqual(
            TDAL_DMX_Register_Channel_Callback( channelId, callback ),
            kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );

    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_SetChanPidGoodParam( void )
 *  \brief Test TDAL_DMX_Set_Channel_PID() function with good parameters
 */

void DMX_TestUnitary_SetChanPidGoodParam( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelId;
	
	type.eChannelStream = eTDAL_DMX_PCR_STREAM;
	type.eChannelOutput = eTDAL_DMX_DECODER;
	type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;

	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	/* Test TDAL_DMX_Set_Channel PID */
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId, 0),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	/* Terminate DMX */
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
	

}

/*! \fn void DMX_TestUnitary_SetChanPidBadParam( void )
 *  \brief Test TDAL_DMX_Set_Channel_PID() with a wrong channel ID
 */

void DMX_TestUnitary_SetChanPidBadParam( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelId;
	
	type.eChannelStream = eTDAL_DMX_PCR_STREAM;
	type.eChannelOutput = eTDAL_DMX_DECODER;
	type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;
	
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId ),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	/* Test TDAL_DMX_Set_Channel PID with bad channel and Bad PID */
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId+1, 0 ),
							kTDAL_DMX_ERROR_INVALID_CHANNEL_ID, "DMX Set Channel PID (wrong channel ID)" );
	/* Terminate DMX */
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_SetChanPidAlreadyUsed( void )
 *  \brief Test TDAL_DMX_Set_Channel_PID() with a PID already used
 *
 */

void DMX_TestUnitary_SetChanPidAlreadyUsed( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId     channelId[2];
	uint16_t				pid = 0x100;
	
	type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;
	
	
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId[0] ),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	TestManager_AssertEqual( TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
														&type,
														&channelId[1] ),
							kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	/* Test TDAL_DMX_Set_Channel PID with a PID already used */
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId[0], pid ),
							kTDAL_DMX_NO_ERROR, "DMX Set Channel PID" );
	TestManager_AssertEqual( TDAL_DMX_Set_Channel_PID( channelId[1], pid ),
							kTDAL_DMX_ERROR_PID_IN_USE, "DMX Set Channel PID with already used PID" );
	/* Terminate DMX */
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId[0] ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId[1] ),
							kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_AllocateFilterBadChannel( void )
 *  \brief Test of allocate filter for bad channel id
 *
 */

void DMX_TestUnitary_AllocateFilterBadChannel( void )
{
    tTDAL_DMX_ChannelId      channelId=0;
    tTDAL_DMX_FilterId       filterId;

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    TestManager_AssertEqual( TDAL_DMX_Allocate_Filter (channelId, &filterId),
                             kTDAL_DMX_ERROR_INVALID_CHANNEL_ID,
                             "DMX allocate channel" );
    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_AllocateFilterMax( void )
 *  \brief Test the maximum filter allocation
 *  \todo Creation of a accessor on maximum filter in tdal dmx to complete this test.
 */

void DMX_TestUnitary_AllocateFilterMax( void )
{
    uint8_t                      cpt = 0;
    tTDAL_DMX_ChannelType_t      type;
    tTDAL_DMX_ChannelId          channelId;
    tTDAL_DMX_ChannelCallbackFct callback;
    tTDAL_DMX_Error              error = kTDAL_DMX_NO_ERROR;
    tTDAL_DMX_FilterId           filterId[100];

    uint8_t i;

    for( i=0; i<100; i++ )
    {
        filterId[i] = 0;
    }


    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
    type.eChannelOutput = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );


    callback = dmxTestGenericSectionNotify;

    TestManager_AssertEqual(
            TDAL_DMX_Register_Channel_Callback( channelId, callback ),
            kTDAL_DMX_NO_ERROR, "DMX register Channel CallBack" );

    while ( error == kTDAL_DMX_NO_ERROR )
    {
        error = TDAL_DMX_Allocate_Filter( channelId, &filterId[cpt] );
        if ( error == kTDAL_DMX_NO_ERROR )
            cpt++;
    }

    mTBOX_PRINT( ( kTBOX_LF, "maximum allocation filter = %d \n", cpt ) );

	/* decrement before freeing because last allocation is invalid */
	cpt--;

    while ( cpt > 0 )
    {
        if ( filterId[cpt] != 0 )
        {
            TestManager_AssertEqual(
                    TDAL_DMX_Free_Filter( filterId[cpt] ),
                    kTDAL_DMX_NO_ERROR, "DMX Free filter" );
        }
        cpt--;
    }

    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_SetFilterBadParam( void )
 *  \brief Test TDAL_DMX_Set_Filter() function with various
 *  bad parameters
 */

void DMX_TestUnitary_SetFilterBadFilter( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;
    uint32_t filter_track = 0;
    uint8_t size = 8;
    uint8_t HardValue[8] = {0x81,0xAA,0x55,0x00,0x00,0x00,0x00,0x80};
    uint8_t HardMask[8] = {0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
    uint8_t HardMode[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
    type.eChannelOutput = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    mTBOX_PRINT( ( kTBOX_LF, "Channel id = %d \n", channelId ) );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Channel_PID( channelId,0x100 ),
        kTDAL_DMX_NO_ERROR,
        "DMX Set Channel PID" );


    TestManager_AssertEqual(
        TDAL_DMX_Set_Filter( channelId,
                             -1,
                             filter_track,
                             size,
                             &HardValue[0],
                             &HardMask[0],
                             &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
                             kTDAL_DMX_ERROR_INVALID_FILTER_ID,
                             "DMX set filter" );

    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_SetFilterGoodParam( void )
 *  \brief
 *
 */

void DMX_TestUnitary_SetFilterBadParam( void )
{	
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;
    tTDAL_DMX_FilterId       filterId;
    uint32_t filter_track = 0;
    uint8_t HardValue[8] = {0x81,0xAA,0x55,0x00,0x00,0x00,0x00,0x80};
    uint8_t HardMask[8] = {0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
    uint8_t HardMode[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
    type.eChannelOutput = eTDAL_DMX_MEMORY;
    type.eChannelMemorySize = eTDAL_DMX_MEDIUM_MEMORY_SIZE;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    mTBOX_PRINT( ( kTBOX_LF, "Channel id = %d \n", channelId ) );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Channel_PID( channelId,0x100 ),
        kTDAL_DMX_NO_ERROR,
        "DMX Set Channel PID" );

    TestManager_AssertEqual( TDAL_DMX_Allocate_Filter (channelId, &filterId),
                             kTDAL_DMX_NO_ERROR,
                             "DMX allocate channel" );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Filter( channelId,
                             filterId,
                             filter_track,
                             17,
                             &HardValue[0],
                             &HardMask[0],
                             &HardMode[0],eTDAL_DMX_CRC_IGNORE ),
                             kTDAL_DMX_ERROR_BAD_PARAMETERS,
                             "DMX set filter" );

    TestManager_AssertEqual(
                    TDAL_DMX_Free_Filter( filterId ),
                    kTDAL_DMX_NO_ERROR, "DMX Free filter" );

    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                    kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_ControlChanBadChannel( void )
 *  \brief Test of control channel with bad channel id
 *  \bug return a error kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER when the channel id is wrong
 */

void DMX_TestUnitary_ControlChanBadChannel( void )
{
    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Channel( -1, eTDAL_DMX_CTRL_ENABLE ),
            kTDAL_DMX_ERROR_INVALID_CHANNEL_ID,
            "DMX control channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_ControlChanBadParam( void )
 *  \brief Test of control channel with bad parameter
 *
 */

void DMX_TestUnitary_ControlChanBadParam( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;


    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_PCR_STREAM;
    type.eChannelOutput = eTDAL_DMX_DECODER;
    type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Channel (channelId, -1),
            kTDAL_DMX_ERROR_BAD_PARAMETERS,
            "DMX control channel" );


    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_ControlChanGoodParam( void )
 *  \brief Test of control channel with good parameter
 *
 */

void DMX_TestUnitary_ControlChanGoodParam( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;

    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_PCR_STREAM;
    type.eChannelOutput = eTDAL_DMX_DECODER;
    type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Channel_PID( channelId,0x100 ),
        kTDAL_DMX_NO_ERROR,
        "DMX Set Channel PID" );

	TestManager_AssertEqual(
            TDAL_DMX_Control_Channel (channelId, eTDAL_DMX_CTRL_ENABLE),
            kTDAL_DMX_NO_ERROR,
            "DMX control channel" );


    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
}

/*! \fn void DMX_TestUnitary_ControlFilterBadFilter( void )
 *  \brief Test of set filter with bad filter id
 *
 */

void DMX_TestUnitary_ControlFilterBadFilter( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
	
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	type.eChannelStream = eTDAL_DMX_SECTION_STREAM;
	type.eChannelOutput = eTDAL_DMX_MEMORY;
	type.eChannelMemorySize = eTDAL_DMX_LOW_MEMORY_SIZE;
	
	TestManager_AssertEqual(
			TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
									   &type,
									   &channelId ),
			kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	
	TestManager_AssertEqual(
			TDAL_DMX_Set_Channel_PID( channelId, 0x100 ),
			kTDAL_DMX_NO_ERROR,
			"DMX Set Channel PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
			kTDAL_DMX_NO_ERROR,
			"DMX allocate filter" );
	
	TestManager_AssertEqual(
			TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
			kTDAL_DMX_NO_ERROR,
			"DMX control channel" );
	
	TestManager_AssertEqual(
			TDAL_DMX_Control_Filter( -1, eTDAL_DMX_CTRL_ENABLE ),
			kTDAL_DMX_ERROR_INVALID_FILTER_ID,
			"DMX control filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
			kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
}

/*! \fn void DMX_TestUnitary_ControlFilterBadParam( void )
 *  \brief
 *
 */

void DMX_TestUnitary_ControlFilterBadParam( void )
{
	tTDAL_DMX_ChannelType_t type;
	tTDAL_DMX_ChannelId      channelId;
	tTDAL_DMX_FilterId       filterId;
	
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
	
	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );
	
	type.eChannelStream = eTDAL_DMX_PCR_STREAM;
	type.eChannelOutput = eTDAL_DMX_DECODER;
	type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;
	
	TestManager_AssertEqual(
			TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
									   &type,
									   &channelId ),
			kTDAL_DMX_NO_ERROR, "DMX allocate channel" );
	
	TestManager_AssertEqual(
			TDAL_DMX_Set_Channel_PID( channelId, 0x100 ),
			kTDAL_DMX_NO_ERROR,
			"DMX Set Channel PID" );
	
	TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
			kTDAL_DMX_NO_ERROR,
			"DMX allocate filter" );
	
	TestManager_AssertEqual(
			TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
			kTDAL_DMX_NO_ERROR,
			"DMX control channel" );
	
	TestManager_AssertEqual(
			TDAL_DMX_Control_Filter( filterId , 3 ),
			kTDAL_DMX_ERROR_BAD_PARAMETERS,
			"DMX control filter" );
	
	TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
			kTDAL_DMX_NO_ERROR, "DMX Free Channel" );
	
	TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
}

/*! \fn void DMX_TestUnitary_FreeUnusedFilter( void )
 *  \brief Test of free not used filter
 *
 */

void DMX_TestUnitary_FreeUnusedFilter( void )
{
    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    TestManager_AssertEqual(
                    TDAL_DMX_Free_Filter( 0 ),
                    kTDAL_DMX_ERROR_CANT_CONTROL_FREE_FILTER, "DMX Free filter" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
}

/*! \fn void DMX_TestUnitary_FreeFilterNominalCase( void )
 *  \brief test of free filter in nominal case
 *
 */

void DMX_TestUnitary_FreeFilterNominalCase( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;
    tTDAL_DMX_FilterId       filterId;

    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));

	TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    type.eChannelStream = eTDAL_DMX_PCR_STREAM;
    type.eChannelOutput = eTDAL_DMX_DECODER;
    type.eChannelMemorySize = eTDAL_DMX_NO_MEMORY;

    TestManager_AssertEqual(
        TDAL_DMX_Allocate_Channel( gTDAL_DMX_TestDmxIds[gTDAL_DMX_TestDmxIdx],
								   &type,
								   &channelId ),
        kTDAL_DMX_NO_ERROR, "DMX allocate channel" );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Channel_PID( channelId, 0x100 ),
        kTDAL_DMX_NO_ERROR,
        "DMX Set Channel PID" );

    TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
                             kTDAL_DMX_NO_ERROR,
                             "DMX allocate filter" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Channel( channelId, eTDAL_DMX_CTRL_ENABLE ),
            kTDAL_DMX_NO_ERROR,
            "DMX control channel" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Filter( filterId, eTDAL_DMX_CTRL_ENABLE ),
            kTDAL_DMX_NO_ERROR,
            "DMX control filter" );

    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
}

/*! \fn void DMX_TestUnitary_FreeUnusedChan( void )
 *  \brief Test of free not used channel
 *
 */

void DMX_TestUnitary_FreeUnusedChan( void )
{
    TestManager_AssertEqual( TDAL_DMX_Init(), kTDAL_DMX_NO_ERROR, "DMX init" );

    TestManager_AssertEqual(
                    TDAL_DMX_Free_Channel( 0 ),
                    kTDAL_DMX_ERROR_CANT_CONTROL_FREE_CHANNEL, "DMX Free Channel" );
    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
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
    mTBOX_PRINT((kTBOX_LF,"Section received on filter %d, Section size %d, Buffer address %8x \n\n",
				 FilterId,
				 Length,
				 Buffer ));

}

