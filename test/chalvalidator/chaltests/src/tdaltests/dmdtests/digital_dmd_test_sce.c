/** \file dmd_test_sce.c
 *  \brief Scenario Tests for the DMD layer of the TDAL API
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_dmd.h"

#include "testhelper.h"
#include "dmdtests.h"
#include "tester_prefs_dmd.h"

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

static tTestNode gTestScenarioTuneWait = {
	"TestScenarioTuneWait",
	"Tune & check a locked notification is received. Wait 2 sec to check that no other notification is received",
	0,
	TestSce_TuneWait,
	NULL
};

#if 0 // Currently not used
static tTestNode gTestScenarioPlugUnplug = {
	"TestScenarioPlugUnplug",
	"Plug and Unplug Antenna Cable",
	1, /* interactive */
	TestSce_PlugUnplug,
	NULL
};

static tTestNode gTestScenarioPlugUnplug2 = {
	"TestScenarioPlugUnplug2",
	"Plug and Unplug Antenna Cable",
	1, /* interactive */
	TestSce_PlugUnplug2,
	NULL
};
#endif

static tTestNode gTestScenarioTwiceTune = {
	"TestScenarioTwiceTune",
	"Tune twice without waiting for notification",
	0,
	TestSce_TwiceTune,
	NULL
};

static tTestNode gTestScenarioTuneAll = {
	"TestScenarioTuneAll",
	"Tune all channel present in tester_prefs_dmd.h file",
	0,
	TestSce_TuneAll,
	NULL
};

static tTestNode gTestScenarioScan= {
	"TestScenarioScan",
	"Scanning scenario test",
	0,
	TestSce_Scanning,
	NULL
};

tTestNode* gDMDScenarioTestNodeList[] = {
	&gTestScenarioTuneWait,
	/*&gTestScenarioPlugUnplug,*/
	/*&gTestScenarioPlugUnplug2,*/
	&gTestScenarioTwiceTune,
	&gTestScenarioTuneAll,
	&gTestScenarioScan,
	NULL
};

tTestNode gTDAL_DMD_ScenarioTestSuite = {
	"TDAL_DMD Scenario Tests",
	"test TDAL_DMD scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDMDScenarioTestNodeList
};

/******************************************************
 *              Scenario tests                        *
 ******************************************************/

/*! \fn void TestSce_TuneWait( void )
 *  \brief Tune & check a locked notification is received. Wait 2 sec to check that no other notification is received
 \verbatim
 Configure demod and Tune a channel
 Wait 2 sec and check that no other notification is received
 \endverbatim
 */

void TestSce_TuneWait()
{
	unsigned char 			ucFeID;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	TestHelper_DMD_Tune(&ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	mTBOX_PRINT((kTBOX_LF, "Waiting 2 sec and check that no other notification is received\n"));
	TKEL_DelayTask(ms2tick(2000));
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn void TestSce_PlugUnplug( void )
 *  \brief Plug and Unplug Antenna Cable
 \verbatim
 Configure demod and Tune a channel
 Ask the user to unplug the cable and then replug it and check that expected notifications are received
 \endverbatim
 *  \warning this test is interactive
 */

void TestSce_PlugUnplug()
{
	unsigned char 			ucFeID;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	TestHelper_DMD_Tune(&ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	TestManager_AskForAction("Please unplug the antenna cable");
	TestHelper_DMD_WaitStatus(ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_SIGNAL_LOST, "signal lost");
	TestManager_AskForAction("Please replug the antenna cable");
	TestHelper_DMD_WaitStatus(ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn void TestSce_PlugUnplug2( void )
 *  \brief Plug and Unplug Antenna Cable and tune another channel between the plug operations
 \verbatim
 Configure demod and Tune a channel.
 Ask the user to unplug the cable, tune another channel.
 Ask the user to replug the cable and check that the new channel is locked.
 \endverbatim
 * \warning this test is interactive
 */

void TestSce_PlugUnplug2()
{
	unsigned char ucFeID;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	TestHelper_DMD_Tune(&ucFeID); /* Tune Channel 1 by default */
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	TestManager_AskForAction("Please unplug the antenna cable");
	TestHelper_DMD_WaitStatus(ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_SIGNAL_LOST,"signal lost");
	TestManager_AskForAction("Please replug the antenna cable");
	TestHelper_DMD_Tune_Chan(&ucFeID, CHANNEL_2);
	TestHelper_DMD_WaitStatus(ucFeID);
	TestManager_AskForChecking("Press 1 when cable is replugged");
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	TestHelper_DMD_GetInfo(&ucFeID);
	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn void TestSce_TwiceTune( void )
 *  \brief Tune twice without waiting for notification
 *  \bug this test is known to fail time to time cause it may receive a
 *  SIGNAL_LOST notification from the first channel
 */

void TestSce_TwiceTune()
{
	unsigned char	ucFeID;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	TestHelper_DMD_Tune_Chan(&ucFeID, CHANNEL_1);
	TestHelper_DMD_Tune_Chan(&ucFeID, CHANNEL_2);
	TestHelper_DMD_WaitStatus(ucFeID);
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	TestHelper_DMD_GetInfo(&ucFeID);
	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn void TestSce_TuneAll( void )
 *  \brief Tune all channel present in tester_prefs_dmd.h file
 */

void TestSce_TuneAll()
{
	unsigned char ucFeID;
	int index = 0;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	for(index = 0; index < NUMBER_CHANNEL; index++)
	{
		TestHelper_DMD_Tune_Chan(&ucFeID, TuneChannel[index].name);
		TestHelper_DMD_WaitStatus(ucFeID);
		TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
		TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	}
	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn void TestSce_Scanning( void )
 *  \scan the band
 */

 
 
void TestSce_Scanning()
{
	unsigned char ucFeID;
	tTDAL_DMD_ScanData  DMD_Scan;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
	uint16_t  FreqFound = 0;

	DMD_Scan.Sat.SymbolRateBw = SYM_RATE;
	DMD_Scan.Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
	DMD_Scan.Sat.Modulation = MODULATION;
	DMD_Scan.Sat.Convolution = CONVOLUTION;
	DMD_Scan.Sat.Polarization = POLARIZATION;	
	
	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);

#ifdef MEDIUM_TER
    TestManager_AssertEqual(TDAL_DMD_StartScan(ucFeID, &DMD_Scan,474000000,858000000),
                            eTDAL_DMD_NO_ERROR,
                            "TDAL_DMD_StartScan");
#endif
#ifdef MEDIUM_SAT
    TestManager_AssertEqual(TDAL_DMD_StartScan(ucFeID, &DMD_Scan,11600000,12750000),
                            eTDAL_DMD_NO_ERROR,
                            "TDAL_DMD_StartScan");
#endif
#ifdef MEDIUM_CAB
    TestManager_AssertEqual(TDAL_DMD_StartScan(ucFeID, &DMD_Scan,474000000,858000000),
                            eTDAL_DMD_NO_ERROR,
                            "TDAL_DMD_StartScan");
#endif
    TestHelper_DMD_WaitStatus(ucFeID);
    TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_FOUND, "signal locked");

	if(DMD_NotifyCond == eTDAL_DMD_FOUND)
	{
		FreqFound++;
		TestHelper_DMD_GetInfo(&ucFeID);
	
	}
	while(DMD_NotifyCond == eTDAL_DMD_FOUND)
	{
	    TestManager_AssertEqual(TDAL_DMD_ContScan(ucFeID),
                                eTDAL_DMD_NO_ERROR,
                                "TDAL_DMD_ContScan");
		TestHelper_DMD_WaitStatus(ucFeID);
		TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
		
        if(DMD_NotifyCond == eTDAL_DMD_FOUND)
		{
			FreqFound++;
			TestHelper_DMD_GetInfo(&ucFeID);
            mTBOX_PRINT((kTBOX_LF,"signal locked\n" ));
		}	
	}
    TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_END_OF_RANGE, "End of scanning");

	mTBOX_PRINT((kTBOX_LF,"Frequencies found -> %d\n",FreqFound));

    TestManager_AssertEqual(TDAL_DMD_StopScan(ucFeID),
                            eTDAL_DMD_NO_ERROR,
                            "TDAL_DMD_StopScan");

	TestHelper_DMD_Term(&ucFeID);
	
	
}


