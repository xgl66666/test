/** \file gfxtests.c
 *  \brief Tests for the GFX layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 *              Include                               *
 ******************************************************/

#include "crules.h"

#include "TestManager.h"
#include "gfxtests.h"

/******************************************************
 *              GFX test tree                         *
 ******************************************************/

/*! \var tTestNode* gTDAL_GFXTestNodeList[] 
 *  \brief Root node for GFX tests
 *
 *  This node references the root node of each test.
 *  I mean the root node of Unitary GFX tests, the one
 *  of Scenario GFX tests and so on...
 */
tTestNode* gTDAL_GFXTestNodeList[] = {
	&gTDAL_GFX_UnitaryTestSuite,
	&gTDAL_GFX_ScenarioTestSuite, 
	&gTDAL_GFX_PerformanceTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_GFX_TestSuite
 *  \brief Node for GFX test suite
 *
 *  This Node give useful info about graphics test suite.
 *  It also point to gTDAL_GFXTestNodeList to know 
 *  available GFX test categories
 */
tTestNode gTDAL_GFX_TestSuite = {
	"Test_TDAL_GFX",
	"test the gfx module",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTDAL_GFXTestNodeList
};

