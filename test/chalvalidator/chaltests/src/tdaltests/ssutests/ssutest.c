/******************************************************
 *              Include                               *
 ******************************************************/

#include "crules.h"

#include "TestManager.h"
#include "ssutests.h"

/******************************************************
 *              DMX test tree                         *
 ******************************************************/
tTestNode* gTDAL_SSUTestNodeList[] = {
    &gTDAL_SSU_ScenarioTestSuite,
    &gTDAL_SSU_UnitaryTestSuite,
    NULL
};

tTestNode gTDAL_SSU_TestSuite = {
    "Test_TDAL_SSU",
    "test the system software module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_SSUTestNodeList
};
