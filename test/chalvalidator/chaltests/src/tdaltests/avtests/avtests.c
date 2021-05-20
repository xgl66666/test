/** \file avtests.c
 *  \brief Scenario Tests for the A/V layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 16/03/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_av.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "avtests.h"

/*! \var tTestNode* gTDAL_AVTestNodeList[]
 *  \brief Node for A/V tests
 *
 *  This node references available tests for
 *  TDAL_AV tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_AVTestNodeList[] = {
	&gTDAL_AV_UnitaryTestSuite,
	&gTDAL_AV_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_AV_TestSuite
 * \brief Node for A/V test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_AVTestNodeList to know
 *  available A/V tests
 */
tTestNode gTDAL_AV_TestSuite = {
    "Test_TDAL_AV",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_AVTestNodeList
};


/* To configure which Demod/Demux indexes to use in the av tests */
int gTDAL_AV_TestFeIdx  = 0;
int gTDAL_AV_TestDmxIdx = 0;

