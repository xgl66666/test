/** \file dmdtests.h
 *  \brief Declaration of functions for tests of the DMD layer
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

#ifndef _DMD_TESTS_H
#define _DMD_TESTS_H

/******************************************************
 *              Include                               *
 ******************************************************/
#include "tdal_dmd.h"

#include "TestManager.h"

/******************************************************
 *              Defines                               *
 ******************************************************/

/******************************************************
 *              Function declaration                  *
 ******************************************************/

/* functions for unitary tests */
void TestUnitary_InitTerm( void );
void TestUnitary_Config( void );
void TestUnitary_Tune( void );
void TestUnitary_GetInfo( void );
void TestUnitary_Unlock( void );

/* functions for scenario tests */
void TestSce_TuneWait( void );
void TestSce_PlugUnplug( void );
void TestSce_PlugUnplug2( void );
void TestSce_TwiceTune( void );
void TestSce_TuneAll( void );
void TestSce_Scanning( void );

/* functions fo performance tests */
void TestPerf_Tune( void );
void TestPerf_TuneFailed( void );

/* functions for reliability tests */
void TestRel_LoopTune( void );

/* functions for stress tests */
void TestStress_GetInfoTune( void );

/* help functions for DMD tests */
void TestHelper_DMD_Config( unsigned char *ucFeID, tTDAL_DMD_TerSourceState SourceState);
void TestHelper_DMD_TuneParam(char *channel, tTDAL_DMD_TunData *DMD_TuneParam, uint32_t *uiFrequency);
void TestHelper_DMD_Tune( unsigned char *ucFeID );
void TestHelper_DMD_Tune_Chan( unsigned char *ucFeID, char *channel );
void TestHelper_DMD_GetInfo( unsigned char *ucFeID );
void TestHelper_DMD_Term( unsigned char *ucFeID );
void TestHelper_DMD_WaitStatus(unsigned char ucFeID);
void TestHelper_DMD_GetStatus(unsigned char ucFeID, tTDAL_DMD_NotifyStatus *peStatus);
void TestHelper_DMD_SetCurrentDMD(int index);

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

extern tTestNode gTDAL_DMD_UnitaryTestSuite;
extern tTestNode gTDAL_DMD_ScenarioTestSuite;
extern tTestNode gTDAL_DMD_PerformanceTestSuite;
extern tTestNode gTDAL_DMD_ReliabilityTestSuite;
extern tTestNode gTDAL_DMD_StressTestSuite;
extern tTestNode* gTDAL_DMDTestNodeList[];

#endif /* _DMD_TESTS_H */
