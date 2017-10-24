/** \file hdmitests.c
 *  \brief Tests for the HDMI layer of the TDAL API
 *  \author Pascal PIGNON
 *  \date 17/05/06
 *  \version 1.0
 */

#ifdef TDAL_HDMI_USE

/******************************************************
 *              Include                               *
 ******************************************************/

#include "crules.h"

#include "TestManager.h"
#include "hdmitests.h"

/******************************************************
 *              HDMI test tree                         *
 ******************************************************/

/*! \var tTestNode* gTDAL_HDMITestNodeList[] 
 *  \brief Root node for HDMI tests
 *
 *  This node references the root node of each test.
 *  I mean the root node of Unitary HDMI tests, the one
 *  of Scenario HDMI tests and so on...
 */
tTestNode* gTDAL_HDMITestNodeList[] = {
	&gTDAL_HDMI_UnitaryTestSuite,
	&gTDAL_HDMI_FunctionalTestSuite, /* Functional */
	&gTDAL_HDMI_StressTestSuite, /* Stress tests */
	&gTDAL_HDMI_ReliabilityTestSuite, /* Reliability tests */

	NULL
};



/*! \var tTestNode gTDAL_HDMI_TestSuite
 *  \brief Node for HDMI test suite
 *
 *  This Node give useful info about HDMI test suite.
 *  It also point to gTDAL_HDMITestNodeList to know 
 *  available HDMI test categories
 */
tTestNode gTDAL_HDMI_TestSuite = {
	"Test_TDAL_HDMI",
	"test the hdmi module",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTDAL_HDMITestNodeList
};

#endif /* TDAL_HDMI_USE */


