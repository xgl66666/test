/** \file disptest.c
 *  \brief Display Tests on TDAL API
 *  \author WangYu
 *  \date 06/06/08
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_disp.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "tstests.h"
#include "disptests.h"

/*! \var tTestNode* gTDAL_DispTestNodeList[]
 *  \brief Node for display tests
 *
 *  This node references available tests for
 *  TDAL_disp tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_DispTestNodeList[] = {
	&gTDAL_Disp_ScenarioTestSuite,
	&gTDAL_Disp_UnitaryTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_Disp_TestSuite
 * \brief Node for display test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_DispTestNodeList to know
 *  available display tests
 */
tTestNode gTDAL_Disp_TestSuite = {
    "Test_TDAL_Disp",
    "test the display module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_DispTestNodeList
};


