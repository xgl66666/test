/** \file kbdtests.c
 *  \brief KBD Tests on TDAL API
 *  \author Arnaud Viala
 *  \date 26/02/07
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_kbd.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "kbdtests.h"

/*! \var tTestNode* gTDAL_KBDTestNodeList[]
 *  \brief Node for Keyboard tests
 *
 *  This node references available tests for 
 *  TDAL_KBD tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_KBDTestNodeList[] = {
	&gTDAL_KBD_UnitaryTestSuite,
	&gTDAL_KBD_ScenarioTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_KBD_TestSuite
 * \brief Node for Keyboard test suite
 *
 *  This Node give useful info about audio/video test suite.
 *  It also point to gTDAL_KBDTestNodeList to know
 *  available keyboard tests
 */
tTestNode gTDAL_KBD_TestSuite = {
    "Test_TDAL_KBD",
    "test the keyboard module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_KBDTestNodeList
};


