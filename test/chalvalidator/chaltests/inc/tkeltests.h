/** \file tkeltests.h
 *  \brief Header for TKEL tests
 *
 *  This file contains declarations of the root nodes
 *  for the different available tests. It of course also
 *  declares specific nodes for TKEL tests.
 *  
 *  \author Ronan MENEU
 *  \date 14/03/06
 *  \version 1.0
 */

#ifndef _TKEL_TESTS_H
#define _TKEL_TESTS_H

#include "TestManager.h"
/*! \var gTDAL_DMD_TestSuite
 *  \brief root node for DMD test suite
 *  \see dmdtests.c */
/*! \var gTDAL_DMX_TestSuite
 *  \brief root node for DMX test suite
 *  \see dmxtests.c */
/*!	\var gTDAL_AV_TestSuite
 *  \brief root node for AV test suite
 *  \see avtests.c */
/*!	\var gTKELTestSuite
 *  \brief root node for TKEL test suite
 *  \see tkeltests.c */
/*! \var gTKELMutexTestSuite
 *  \brief node for TKEL mutex tests
 *  \see tkelmutexttests.c */
/*! \var gTKELSemaphoreTestSuite
 *  \brief node for TKEL semaphore tests
 *  \see tkelsemaphoretest.c */
/*! \var gTKELMacroTestSuite
 *  \brief node for TKEL macro tests
 *  \see tkelmacro.c
 */
extern tTestNode gTKELTaskTestSuite;
extern tTestNode gTKELMutexTestSuite;
extern tTestNode gTKELSemaphoreTestSuite;
extern tTestNode gTKELQueueTestSuite;
extern tTestNode gTKELMacroTestSuite;
extern tTestNode gTKELEventTestSuite;
extern tTestNode gTKELTestSuite;
extern tTestNode gTKELTimerTestSuite;

extern tTestNode gTDAL_DMX_TestSuite;
extern tTestNode gTDAL_DMD_TestSuite;
extern tTestNode gTDAL_GFX_TestSuite;
#endif /* #ifndef _TKEL_TESTS_H */
