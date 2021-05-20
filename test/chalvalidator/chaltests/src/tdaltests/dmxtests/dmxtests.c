/** \file dmxtests.c
 *  \brief Tests for the DMX layer of the TDAL API
 *  \author Olivier DOLE
 *  \date 04/04/06
 *  \version 1.0
 */

/******************************************************
 *              Include                               *
 ******************************************************/

#include "crules.h"

#include "TestManager.h"
#include "dmxtests.h"

/******************************************************
 *              DMX test tree                         *
 ******************************************************/

/*! \var tTestNode* gTDAL_DMXTestNodeList[]
 *  \brief Root node for DMX tests
 *
 *  This node references the root node of each test.
 *  I mean the root node of Unitary DMX tests, the one
 *  of Scenario DMD tests and so on...
 */
tTestNode* gTDAL_DMXTestNodeList[] = {
	&gTDAL_DMX_UnitaryTestSuite,
    &gTDAL_DMX_TablesSectionsTest,
    &gTDAL_DMX_ScenarioTests,
    &gTDAL_DMX_StressTests,
	NULL
};

/*! \var tTestNode gTDAL_DMX_TestSuite
 *  \brief Node for DMX test suite
 *
 *  This Node give useful info about dmx test suite.
 *  It also point to gTDAL_DMXTestNodeList to know
 *  available DMX test categories
 */
tTestNode gTDAL_DMX_TestSuite = {
	"Test_TDAL_DMX",
	"test the dmx module",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gTDAL_DMXTestNodeList
};

/* To configure which Demod/Demux indexes to use in the dmx tests */
int gTDAL_DMX_TestFeIdx  = 0;
int gTDAL_DMX_TestDmxIdx = 0;

tTDAL_DMX_DemuxId gTDAL_DMX_TestDmxIds[eTDAL_DMX_NB_DEMUX] =
{
	eTDAL_DMX_DEMUX0,
	eTDAL_DMX_DEMUX1,
	eTDAL_DMX_DEMUX2
};


