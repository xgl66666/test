/** \file tstests.c
 *  \brief TS Tests on TDAL API
 *  \author Arnaud Viala
 *  \date 02/03/07
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_ts.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "tstests.h"

/*! \var tTestNode* gTDAL_TSTestNodeList[]
 *  \brief Node for Keyboard tests
 *
 *  This node references available tests for
 *  TDAL_TS tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_TSTestNodeList[] = {
	&gTDAL_TS_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_TS_TestSuite
 * \brief Node for Keyboard test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_TSTestNodeList to know
 *  available keyboard tests
 */
tTestNode gTDAL_TS_TestSuite = {
    "Test_TDAL_TS",
    "test the keyboard module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_TSTestNodeList
};



/* To configure which Demod/Demux indexes to use in the TS tests */
int gTDAL_TS_TestFeIdx  = 0;
int gTDAL_TS_TestDmxIdx = 0;

