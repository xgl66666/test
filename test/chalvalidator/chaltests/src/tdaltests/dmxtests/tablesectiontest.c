/** \file tablesectiontest.c
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
/******************************************************
 *              Defines                               *
 ******************************************************/

static TKEL_sema_id dmxTestTableSectionSem = (TKEL_sema_id)NULL;

/******************************************************
 *             CallBack                               *
 *****************************************************/

static void dmxTestGenericSectionNotify(tTDAL_DMX_ChannelId ChannelId,
                                            tTDAL_DMX_FilterId FilterId,
                                            uint32_t Filter_track,
                                            uint32_t Length,
                                            uint8_t *Buffer);

static void dmxTestSectionNotify(uint8_t* buffer, uint32_t length);


/******************************************************
 *              DMX test tree                         *
 ******************************************************/


tTestNode gTDAL_DMX_TablesSectionsTest = {
    "TDAL_DMX tables sections test",
    "test the TDAL_DMX function input parameters",
    0, /* not interactive */
    DMX_TablesSectionsTest,
    NULL, /* NO SON*/
};


void DMX_TablesSectionsTest( void )
{
    tTDAL_DMX_ChannelType_t type;
    tTDAL_DMX_ChannelId      channelId;
    tTDAL_DMX_FilterId       filterId;
    uint32_t filter_track = 0;
    uint8_t size = 1;
    uint8_t HardValue[1] = {0x42};
    uint8_t HardMask[1] = {0xFF};
    uint8_t HardMode[1] = {0xFF};
    tTDAL_DMX_ChannelCallbackFct callback;

    TestHelper_Initialize_TDAL_DMD();
    TestHelper_DMDTune(gTDAL_DMX_TestFeIdx);
    TestManager_AssertEqual( TDAL_TSROUTE_Init(), eTDAL_TSROUTE_NO_ERROR, "tsroute init" );
	TestHelper_Configure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
									  TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));


    TestManager_AssertEqual( TKEL_CreateSemaphore(0,&dmxTestTableSectionSem),
                             TKEL_NO_ERR,
                             "TKEL CreateSemaphore" );

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

    TestManager_AssertEqual( TDAL_DMX_Allocate_Filter( channelId, &filterId ),
                             kTDAL_DMX_NO_ERROR,
                             "DMX allocate filter" );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Channel_PID( channelId, 0x0011 ),
        kTDAL_DMX_NO_ERROR,
        "DMX Set Channel PID" );

    TestManager_AssertEqual(
        TDAL_DMX_Set_Filter( channelId,
                             filterId,
                             filter_track,
                             size,
                             &HardValue[0],
                             &HardMask[0],
                             &HardMode[0],
                             eTDAL_DMX_CRC_IGNORE ),
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

	//mz
	//TestManager_AssertEqual( TKEL_SignalSemaphore(dmxTestTableSectionSem),
	//						TKEL_NO_ERR,
	//						"TKEL Signal Semaphore" );
	//end

    TestManager_AssertEqual( TKEL_WaitSemaphore( dmxTestTableSectionSem ),
                             TKEL_NO_ERR,
                             "TKEL Wait Semaphore" );

    TestManager_AssertEqual(
            TDAL_DMX_Control_Channel (channelId,eTDAL_DMX_CTRL_DISABLE),
            kTDAL_DMX_NO_ERROR, "DMX control channel" );


    TestManager_AssertEqual( TDAL_DMX_Free_Channel( channelId ),
                             kTDAL_DMX_NO_ERROR, "DMX Free Channel" );

	TestHelper_Terminate_TDAL_DMD();
    TestManager_AssertEqual( TDAL_DMX_Terminate(), kTDAL_DMX_NO_ERROR, "DMX terminate" );
    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_DMX_TestFeIdx),
										TEST_HELPER_TSROUTE_DEMUX(gTDAL_DMX_TestDmxIdx));
    TestManager_AssertEqual( TDAL_TSROUTE_Term(), eTDAL_TSROUTE_NO_ERROR, "tsroute terminate" );
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
    TKEL_err tkError;

    mTBOX_PRINT((kTBOX_LF,"Section received on filter %d, Section size %d, Buffer address %8x \n",FilterId,Length,Buffer ));

    /*  check the section received and print info  */
    dmxTestSectionNotify(Buffer,Length);

    /*  free the TDAL buffer  */
    /* tkError = TKEL_Free(Buffer); */
    if (TDAL_DMX_Free_Buffer(Buffer) != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to free the TDAL buffer \n" ));
    }

    tkError = TKEL_SignalSemaphore(dmxTestTableSectionSem);
    if (tkError != TKEL_NO_ERR)
    {
        mTBOX_PRINT((kTBOX_LF,"error to release the semaphore 0x%x\n",tkError ));
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
        for (i=0; i<10; i++)
        {
                sprintf(&outStr[strlen(outStr)], " 0x%02x", buffer[i]);
        }

        mTBOX_PRINT((kTBOX_LF,"sn-> %s\r\n",outStr ));

}


