/** \file nvmtests.c
 *  \brief NVM Tests on TDAL API
 *  \author Ronan Meneu
 *  \date 21/04/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_nvm.h"

#include "TestManager.h"
#include "nvmtests.h"

/*! \var tTestNode* gTDAL_NVMTestNodeList[]
 *  \brief Node for NVM tests
 *
 *  This node references available tests for
 *  TDAL_NVM tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_NVMTestNodeList[] = {
	&gTDAL_NVM_UnitaryTestSuite,
	&gTDAL_NVM_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_NVM_TestSuite
 * \brief Node for NVM test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_NVMTestNodeList to know
 *  available NVM tests
 */
tTestNode gTDAL_NVM_TestSuite = {
    "Test_TDAL_NVM",
    "test the NVM module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_NVMTestNodeList
};


