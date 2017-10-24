/** \file analogtests.c
 *  \brief Tests for the Analog layer of the TDAL API
 *  \author
 *  \date
 *  \version 1.0
 */

/******************************************************
 *              Include                               *
 ******************************************************/

#include "crules.h"

#include "TestManager.h"
#include "analogtests.h"

/******************************************************
 *              ANALOG test tree                         *
 ******************************************************/

/*! \var tTestNode* gTDAL_AnalogTestNodeList[]
 *  \brief Root node for ANALOG tests
 *
 *  This node references the root node of each test.
 *  I mean the root node of Unitary ANALOG tests, the one
 *  of Scenario ANA tests and so on...
 */
tTestNode* gTDAL_AnalogTestNodeList[] = {
	&gTDAL_ANALOG_ScenarioTestSuite,
	&gTDAL_ANALOG_AtvDtvTestSuite,
	NULL
};

/*! \var tTestNode gTDAL_ANA_TestSuite
 *  \brief Node for ANA test suite
 *
 *  This Node give useful info about ana test suite.
 *  It also point to gTDAL_AnalogTestNodeList to know
 *  available ANALOG test categories
 */
tTestNode gTDAL_ANALOG_TestSuite = {
	"Test_TDAL_ANALOG",
	"test all the analog modules",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTDAL_AnalogTestNodeList
};

