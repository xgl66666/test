/** \file flatests.c
 *  \brief Flash Tests on TDAL API
 *  \author Ronan Meneu
 *  \date 21/04/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_fla.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "flatests.h"

/*! \var tTestNode* gTDAL_FLATestNodeList[]
 *  \brief Node for Flash tests
 *
 *  This node references available tests for 
 *  TDAL_FLA tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_FLATestNodeList[] = {
	&gTDAL_FLA_UnitaryTestSuite,
	&gTDAL_FLA_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_FLA_TestSuite
 * \brief Node for Flash test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_FLATestNodeList to know
 *  available flash tests
 */
tTestNode gTDAL_FLA_TestSuite = {
    "Test_TDAL_FLA",
    "test the flash module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_FLATestNodeList
};


