#ifndef _SSUTESTS_H_
#define _SSUTESTS_H_

/******************************************************
 *              Include                               *
 ******************************************************/

#include "tdal_ssu.h"

#include "TestManager.h"

/******************************************************
 *              Function declaration                  *
 ******************************************************/

/* functions for unitary tests */
void SSU_TestUnitary_GetPlatformInfo(void);
void SSU_TestUnitary_GetStatusInfo(void);

/* functions for scenario tests */
void SSU_scenario_TriggerSSU(void);

/******************************************************
 *              SSU test tree                         *
 ******************************************************/

extern tTestNode gTDAL_SSU_ScenarioTestSuite;
extern tTestNode gTDAL_SSU_UnitaryTestSuite;
extern tTestNode gTDAL_SSU_TestSuite;

#endif /* _SSUTESTS_H_ */
