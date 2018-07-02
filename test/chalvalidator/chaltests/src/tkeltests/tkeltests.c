/** \file tkeltests.c
 *  \brief Tests for TKEL layer
 *  \author Ronan MENEU
 *  \date 16/03/06
 *  \version 1.0
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"

#include "tkel.h"
#include "TestManager.h"

#include "tkeltests.h"

/* TKEL Test suite */
/* --------------- */

/*! \var tTestNode* gTestNodeList[]
 *  \brief Root node for TKEL tests
 */
tTestNode* gTestNodeList[] = {
    &gTKELTaskTestSuite,
    &gTKELMutexTestSuite,
    &gTKELSemaphoreTestSuite,
    &gTKELQueueTestSuite,
#ifndef CIPLUSVALIDATOR
    &gTKELMacroTestSuite,
    &gTKELEventTestSuite,
    &gTKELTimerTestSuite,
#endif
  NULL
};

/*! \var tTestNode gTKELTestSuite
 *  \brief Node for TKEL test suite
 */
tTestNode gTKELTestSuite = {
    "TestTKEL",
    "test the tkel implementation",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
	(void**)gTestNodeList
};
