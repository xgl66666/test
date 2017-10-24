/** \file avtests.h
 *  \brief Declaration of test suites for av tests
 *  \author Ronan Meneu
 *  \date 23/03/06
 *  \version 1.0
 */

#ifndef _AVTESTS_H_
#define _AVTESTS_H_

/******************************************************
 *              AV test tree                         *
 ******************************************************/
#define TDAL_AV_TESTDATA_PATH "../../test/chalvalidator/chaltests/data/"

extern tTestNode gTDAL_AV_UnitaryTestSuite;
extern tTestNode gTDAL_AV_ScenarioTestSuite;
extern tTestNode gTDAL_AV_TestSuite;

extern int gTDAL_AV_TestFeIdx;
extern int gTDAL_AV_TestDmxIdx;

#endif /* _AVTESTS_H_ */
