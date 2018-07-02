/** \file desctests.c
 *  \brief DESC Tests on TDAL API
 *  \author LO
 *  \date 04/11/08
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"

#include "TestManager.h"
#include "desctests.h"

/*! \var tTestNode* gTDAL_DESCTestNodeList[]
 *  \brief Node for DESC tests
 *
 *  This node references available tests for
 *  TDAL_DESC tests. For the moment only some
 *  scenario tests are available
 */

/*! \var tTestNode gTDAL_DESC_TestSuite
 * \brief Node for DESC test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_DESCTestNodeList to know
 *  available DESC tests
 */
tTestNode gTDAL_DESC_TestSuite = {
    "Test_TDAL_DESC",
    "test the DESC module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_DESC_UnitaryTestSuite
};


