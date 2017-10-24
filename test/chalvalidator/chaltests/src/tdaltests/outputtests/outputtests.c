/** \file avtests.c
 *  \brief Scenario Tests for the output layer of the TDAL API
 *  \date 11/07/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_output.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "outputtests.h"

/*! \var tTestNode* gTDAL_OUTPUTTestNodeList[]
 *  \brief Node for DISP tests
 *
 *  This node references available tests for
 *  TDAL_OUTPUT tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_OUTPUTTestNodeList[] = {
	&gTDAL_OUTPUT_UnitaryTestSuite,
	&gTDAL_OUTPUT_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_OUTPUT_TestSuite
 * \brief Node for OUTPUT test suite
 *
 *  This Node give useful info about OUTPUT test suite.
 *  It also point to gTDAL_OUTPUTTestNodeList to know
 *  available DISP tests
 */
tTestNode gTDAL_OUTPUT_TestSuite = {
    "Test_TDAL_OUTPUT",
    "test the OUTPUT module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_OUTPUTTestNodeList
};


