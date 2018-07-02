/** \file dmd_test_stress.c
 *  \brief Stress Tests for the DMD layer of the TDAL API.
 *
 *  To be precise these tests are more concurrency call tests
 *  rather than stress tests. I mean it tests the possibility
 *  to call a function during the excution of another one.
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

static tTestNode gTestStressGetInfoTune= {
	"TestStressGetInfoTune",
	"Call DMD_GetInfo at regular interval in a separate task while tuning",
	0,
	TestStress_GetInfoTune,
	NULL
};

tTestNode* gDMDStressTestNodeList[] = {
	&gTestStressGetInfoTune,
	NULL
};

tTestNode gTDAL_DMD_StressTestSuite = {
	"TDAL_DMD Stress Tests",
	"test TDAL_DMD concurrency calls",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDMDStressTestNodeList
};

/******************************************************
 *              Reliability tests                     *
 ******************************************************/

LOCAL void GetInfoTask( void* arg );
/*! \var static int gindex
 *  \brief declare gindex as static not to do an infinite loop in GetInfoTask */
static int gindex = 0;

static unsigned char 	ucFeID;

/*! \fn void TestStress_GetInfoTune( void )
 *  \brief Call DMD_GetInfo at regular interval in a separate task while tuning
 \verbatim
 Configure demod
 Create a task to retrieve DMDInfo at regular interval
 Do a loop to tune all available frequency define in tester_prefs_dmd.h file
 \endverbatim
 */

void TestStress_GetInfoTune()
{
	unsigned char 	ucFeID;
	TKEL_task_id	InfoTaskId;
	tTDAL_DMD_NotifyStatus  DMD_NotifyCond;
#if (NUMBER_OF_DMD==2)
	int32_t                 dmdIdx;

	dmdIdx = TestManager_GetNumber("Enter DMD index (0 or 1):", 1);
	TestHelper_DMD_SetCurrentDMD(dmdIdx);
#endif

	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);



	TestManager_AssertEqual(TKEL_CreateTask(30, NULL, 0, GetInfoTask, (void*) &ucFeID,  &InfoTaskId), TKEL_NO_ERR, "Create GetInfo task");


	for(gindex = 0; gindex < NUMBER_CHANNEL; gindex++)
	{
		TestHelper_DMD_Tune_Chan(&ucFeID, TuneChannel[gindex].name);
		TestHelper_DMD_WaitStatus(ucFeID);
		TestHelper_DMD_GetStatus(ucFeID, &DMD_NotifyCond);
		TestManager_AssertEqual(DMD_NotifyCond, eTDAL_DMD_LOCKED, "signal locked");
	}
	
	TKEL_DelayTask( ms2tick(2000) );

	TestHelper_DMD_Term(&ucFeID);
}

/*! \fn LOCAL void GetInfoTask( void* arg )
 *  \brief function which retrieve at regular interval information about demod
 \verbatim
 Retrieve information about demod each 150ms (approximatively)
 Delete the task once TDAL_DMD_Tune() has reached the last channel to lock
 \endverbatim
 *  \param[in] arg is in fact a pointer to the Front End ID (useful for TDAL_DMD_GetInfo())
 */

LOCAL void GetInfoTask( void* arg )
{
	int delay = 300;
/*	unsigned char FeID = (int) arg;*/
	

	while(gindex < NUMBER_CHANNEL)
	{
		TKEL_DelayTask( ms2tick(delay) );
		/*TestHelper_DMD_GetInfo( &FeID );*/
		//printf("GetInfoTask : call TestHelper_DMD_GetInfo for Id %d \r\n",ucFeID);
		TestHelper_DMD_GetInfo( &ucFeID );
	}
	TestManager_AssertEqual(TKEL_DeleteTask(NULL), TKEL_NO_ERR, "Delete GetInfo task");
}
