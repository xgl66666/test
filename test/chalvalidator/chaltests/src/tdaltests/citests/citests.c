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
#include "citests.h"

/*! \var tTestNode* gTDAL_AVTestNodeList[]
 *  \brief Node for A/V tests
 *
 *  This node references available tests for
 *  TDAL_AV tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_CITestNodeList[] = {
	&gTDAL_CI_ScenarioTestSuite,
	&gTDAL_CI_UnitaryTestSuite,	
	NULL
};


tTestNode gTDAL_CI_TestSuite = {
    "Test_TDAL_CI",
    "test the ci module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_CITestNodeList
};


/* To configure which Demod/Demux indexes to use in the av tests */
int gTDAL_CI_TestFeIdx  = 0;
int gTDAL_CI_TestDmxIdx = 0;

