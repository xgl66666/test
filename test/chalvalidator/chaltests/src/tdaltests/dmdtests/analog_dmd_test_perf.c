/** \file dmd_test_perf.c
 *  \brief Performance Tests for the DMD layer of the TDAL API
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
extern t_testTune TuneFakeChan;

static tTestNode gTestPerformanceTune = {
	"TestPerformanceTune",
	"Compute time between Tune and notification",
	0,
	TestPerf_Tune,
	NULL
};

static tTestNode gTestPerformanceTuneFake = {
	"TestPerformanceTuneFake",
	"Compute time between Tune and LOCK_FAILED notification",
	0,
	TestPerf_TuneFailed,
	NULL
};

tTestNode* gDMDPerformanceTestNodeList[] = {
	&gTestPerformanceTune,
	&gTestPerformanceTuneFake,
	NULL
};

tTestNode gTDAL_DMD_PerformanceTestSuite = {
	"TDAL_DMD Performance Tests",
	"test the TDAL_DMD performance",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDMDPerformanceTestNodeList
};

/******************************************************
 *              Scenario tests                        *
 ******************************************************/

/*! \fn void TestPerf_Tune( void )
 *  \brief Compute time between a valid tuned channel and its LOCKED notification
 \verbatim
 Create semaphore for callback
 Start chrono
 Tune a channel
 Stop chrono & display results
 \endverbatim
 */

void TestPerf_Tune()
{
	unsigned char 		ucFeID;
	tTDAL_DMD_TunData 	DMD_TuneParam;
	tTestManagerChrono	chrono;
	long				elapsed_time = 0;
	tTDAL_DMD_NotifyStatus DMD_NotifyCond;
    uint32_t            uiFrequency=0;

#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);

	TestHelper_DMD_TuneParam(CHANNEL_1, &DMD_TuneParam, &uiFrequency);
	TestManager_StartChrono(&chrono);
	
	TestManager_AssertEqual( TDAL_DMD_Tune(ucFeID, &DMD_TuneParam, uiFrequency), eTDAL_DMD_NO_ERROR, "dmd tune" );

	TestHelper_DMD_WaitStatus(ucFeID);
	
	TestManager_StopChrono(&chrono, &elapsed_time);

	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);

	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	mTBOX_PRINT((kTBOX_LF, "Time elapsed between tune and notification: %ld ms\n", elapsed_time));

	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn void TestPerf_TuneFailed( void )
 *  \brief Compute time between a fake tuned channel and its FAILED notification
 \verbatim
 Create semaphore for callback
 Start chrono
 Tune a fake channel
 Stop chrono & display results
 \endverbatim
 */

void TestPerf_TuneFailed()
{
	unsigned char       ucFeID;
	tTDAL_DMD_TunData   DMD_TuneParam;
	tTestManagerChrono  chrono;
	long                elapsed_time = 0;
	tTDAL_DMD_NotifyStatus DMD_NotifyCond;
    uint32_t            uiFrequency=0;

#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif
	
	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	
	TestHelper_DMD_TuneParam(CHANNEL_1, &DMD_TuneParam, &uiFrequency);
	TestManager_StartChrono(&chrono);
	
	TestManager_AssertEqual( TDAL_DMD_Tune(ucFeID, &DMD_TuneParam, uiFrequency), eTDAL_DMD_NO_ERROR, "dmd tune" );
	
	TestHelper_DMD_WaitStatus(ucFeID);
	
	TestManager_StopChrono(&chrono, &elapsed_time);
	
	TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
	TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCK_FAILED, "no signal");

	mTBOX_PRINT((kTBOX_LF, "Time elapsed between tune and notification: %ld ms\n", elapsed_time));
	
	TestHelper_DMD_Term(&ucFeID);
}
