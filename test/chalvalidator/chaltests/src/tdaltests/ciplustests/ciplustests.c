/** \file avtests.c
 *  \brief Scenario Tests for the A/V layer of the TDAL API
 *  \author Ronan Meneu
 *  \date 16/03/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "TestManager.h"
#include "ciplustests.h"

tTestNode* gTDAL_CIPLUSTestNodeList[] = {
	&gTDAL_CIPLUSauto,
	&gTDAL_CIPLUSstepbystep,
	NULL
};

tTestNode gTDAL_CIPLUS_TestSuite = {
    "Test_TDAL_CI(+)",
    "",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTDAL_CIPLUSTestNodeList
};

