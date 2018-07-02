/** \file hdmicectests.c
 *  \brief HDMI_CEC Tests on TDAL API
 *  \author 
 *  \date 
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "tdal_hdmi_cec.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"

#include "hdmicectests.h"

tTestNode* gTDAL_HDMI_CECTestNodeList[] = {
	&gTDAL_HDMI_CEC_UnitaryTestSuite,
	&gTDAL_HDMI_CEC_ScenarioTestSuite,
	NULL
};

tTestNode gTDAL_HDMI_CEC_TestSuite = {
    "Test_TDAL_HDMI_CEC",
    "test the HDMI_CEC module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_HDMI_CECTestNodeList
};


