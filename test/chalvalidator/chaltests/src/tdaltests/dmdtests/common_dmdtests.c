/** \file dmdtests.c
 *  \brief Tests for the DMD layer of the TDAL API
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

/******************************************************
 *              Include                               *
 ******************************************************/

#include "crules.h"

#include "TestManager.h"
#include "dmdtests.h"

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

/*! \var tTestNode* gTDAL_DMDTestNodeList[] 
 *  \brief Root node for DMD tests
 *
 *  This node references the root node of each test.
 *  I mean the root node of Unitary DMD tests, the one
 *  of Scenario DMD tests and so on...
 */
tTestNode* gTDAL_DMDTestNodeList[] = {
	&gTDAL_DMD_UnitaryTestSuite,
	&gTDAL_DMD_ScenarioTestSuite,
	&gTDAL_DMD_PerformanceTestSuite,
	&gTDAL_DMD_ReliabilityTestSuite,
	&gTDAL_DMD_StressTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_DMD_TestSuite
 *  \brief Node for DMD test suite
 *
 *  This Node give useful info about dmd test suite.
 *  It also point to gTDAL_DMDTestNodeList to know 
 *  available DMD test categories
 */
tTestNode gTDAL_DMD_TestSuite = {
	"Test_TDAL_DMD",
	"test the dmd module",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTDAL_DMDTestNodeList
};

