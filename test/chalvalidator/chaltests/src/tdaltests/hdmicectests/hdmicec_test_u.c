/** \file hdmicec_test_u.c
 *  \brief Unitary Tests for the HDMI_CEC layer of the TDAL API
 *  \author 
 *  \date 
 *  \version 1.0
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_hdmi_cec.h"

#include "TestManager.h"
#include "testhelper.h"
#include "tdal_common.h"

#include "hdmicectests.h"
void TestUnitary_HdmiCecInitTerm(void);
void TestUnitary_HdmiCecSubscribe(void);

/******************************************************
 * 				Defines								  *
 ******************************************************/

/******************************************************
 * 				Local variables						  *
 ******************************************************/

/******************************************************
 *              hdmicec test tree                    *
 ******************************************************/

static tTestNode gTestUInitTerm = {
	"TestUInitTerm",
	"test TDAL_HDMI_CEC_Init() and TDAL_HDMI_CEC_Term() functions",
	0, /* not interactive */
	TestUnitary_HdmiCecInitTerm,
	NULL
};

static tTestNode gTestHdmiCecSubscribe = {
	"TestHdmiCecSubscribe",
	"test register and unregister functions",
	0, /* not interactive */
	TestUnitary_HdmiCecSubscribe,
	NULL
};

tTestNode* gHDMI_CECUnitaryTestNodeList[] = {
	&gTestUInitTerm,
	&gTestHdmiCecSubscribe,
	NULL
};

tTestNode gTDAL_HDMI_CEC_UnitaryTestSuite = {
	"TDAL_HDMI_CEC Unitary Tests",
	"test the TDAL_HDMI_CEC function input parameters",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gHDMI_CECUnitaryTestNodeList
};

/******************************************************
 * 				Unitary tests 						  *
 ******************************************************/



void TestUnitary_HdmiCecInitTerm()
{
}

void TestUnitary_HdmiCecSubscribe(void)
{
}



