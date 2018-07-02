/** \file dmd_test_rel.c
 *  \brief Reliability Tests for the DMD layer of the TDAL API
 *
 *  In this first release, it only tests how reliable is TDAL_DMD_Tune()
 *  function. So we intensively call it with different channel, and it must
 *  lock the channel at each call.
 *
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

static tTestNode gTestReliabilityLoopTune = {
	"TestReliabilityLoopTune",
	"Do a loop which tunes freq and check that each time a lock notification is received",
	0,
	TestRel_LoopTune,
	NULL
};

tTestNode* gDMDReliabilityTestNodeList[] = {
	&gTestReliabilityLoopTune,
	NULL
};

tTestNode gTDAL_DMD_ReliabilityTestSuite = {
	"TDAL_DMD Reliability Tests",
	"test TDAL_DMD reliability",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDMDReliabilityTestNodeList
};

/******************************************************
 *              Reliability tests                     *
 ******************************************************/

/*! \fn void TestRel_LoopTune( void )
 *  \brief Do a loop which tunes freq and check that each time
 *  a lock notification is received
 */

void TestRel_LoopTune()
{
	unsigned char ucFeID;
	int index = 0, i = 0;
	tTDAL_DMD_NotifyStatus DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif
	
	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	
	for (i = 0; i < 20; i++)
	{
		for(index = 0; index < NUMBER_CHANNEL; index++)
		{
			TestHelper_DMD_Tune_Chan(&ucFeID, TuneChannel[index].name);
			TestHelper_DMD_WaitStatus(ucFeID);
			TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
			TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
		}
		/* initialize index for next loop */
		index = 0;
	}

	TestHelper_DMD_Term(&ucFeID);
}

