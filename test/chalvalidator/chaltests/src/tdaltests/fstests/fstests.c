/** \file fstests.c
 *  \brief FS Tests on TDAL API
 *  \author Arnaud Viala
 *  \date 2007, 20th of August
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_fs.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "fstests.h"

/*! \var tTestNode* gTDAL_FSTestNodeList[]
 *  \brief Node for FileSystem tests
 *
 *  This node references available tests for
 *  TDAL_FS tests. For the moment only some
 *  scenario tests are available
 */
tTestNode* gTDAL_FSTestNodeList[] = {
	&gTDAL_FS_InfoTestSuite,
	&gTDAL_FS_ScenarioTestSuite,
    &gTDAL_FS_InteractiveTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_FS_TestSuite
 * \brief Node for FileSystem test suite
 *
 *  This Node give useful info about gTDAL_FSTestNodeList to know
 *  available FileSystem tests
 */
tTestNode gTDAL_FS_TestSuite = {
    "Test_TDAL_FS",
    "test the filesystem module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_FSTestNodeList
};



