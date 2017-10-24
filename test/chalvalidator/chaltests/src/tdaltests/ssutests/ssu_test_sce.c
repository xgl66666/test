/******************************************************
 *              Includes                              *
 ******************************************************/

#include <stdio.h>

#include "crules.h"
#include "tkel.h"
#include "tbox.h"
#include "tdal_fla.h"
#include "tdal_common.h"

#include "testhelper.h"
#include "ssutests.h"

/******************************************************
 *              SSU test tree                         *
 ******************************************************/

static tTestNode gTestScenarioTriggerSSU = {
        "TestScenarioTriggerSSU",
        "Simulate SSU trigger from middleware",
        0, /* not interactive */
		SSU_scenario_TriggerSSU,
        NULL
};

tTestNode* gTDAL_SSU_ScenarioTestNodeList[] = {
	&gTestScenarioTriggerSSU,
    NULL
};

tTestNode gTDAL_SSU_ScenarioTestSuite = {
    "TDAL_SSU Scenario Tests",
    "test the SSU module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_SSU_ScenarioTestNodeList
};

LOCAL void SSU_scenario_TriggerSSU(void)
{
	tTDAL_SSU_ControlParameters ssuCtrParam;
	int choice;

	ssuCtrParam.bInStandBy		= TestManager_GetNumber("STB is in StandBy (0 | 1) :", 1);
	ssuCtrParam.uiFreqency		= TestManager_GetNumber("Set frequency (6 digits):", 6);
	ssuCtrParam.uiSymbolRate	= TestManager_GetNumber("Set symbol rate (5 digits):", 5);
	ssuCtrParam.uiModulation	= TestManager_GetNumber("Set modulation type 0-6 (tTDAL_SSU_Modulation):", 1);
	ssuCtrParam.uiPid			= TestManager_GetNumber("Set PID (5 digits):", 5);
	ssuCtrParam.uiUpdateID		= TestManager_GetNumber("Set update ID (10 digits):", 10);

    if (TDAL_FLA_Init() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Init() failed!\n"));
        TestManager_AddFailure();
        return;
    }

    if (TDAL_FLA_Open() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Open() failed!\n"));
        TestManager_AddFailure();
        return;
    }

	TestManager_AssertEqual(TDAL_SSU_SetParameters(&ssuCtrParam),	eTDAL_SSU_NO_ERROR, "TDAL_SSU_SetParameters");

    if (TDAL_FLA_Close() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Close() failed!\n"));
        TestManager_AddFailure();
        return;
    }

    if (TDAL_FLA_Term() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Term() failed!\n"));
        TestManager_AddFailure();
        return;
    }

	mTBOX_PRINT((kTBOX_LF,"STB should reboot after which bootloader should initiate software update.\n"));

	choice = TestManager_GetNumber("Choose STB restart method 0-TDAL_Jump; 1-TDAL_SSU_Initiate; 2-TDAL_Reset(default)", 1);
	switch(choice)
	{
	case 0:
		TDAL_Jump(eTDAL_JUMP_BOOTLOADER);
		break;
	case 1:
		TestManager_AssertEqual( TDAL_SSU_Initiate(),	eTDAL_SSU_NO_ERROR, "TDAL_SSU_Initiate");
		break;
	case 2:
	default:
		TDAL_Reset();
	}
}


