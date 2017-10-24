/** \file dmxtests.h
 *  \brief Declaration of functions for tests of the DMX layer
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

#ifndef _DMX_TESTS_H
#define _DMX_TESTS_H

/******************************************************
 *              Include                               *
 ******************************************************/

#include "tdal_dmx.h"

#include "TestManager.h"

/******************************************************
 *              Function declaration                  *
 ******************************************************/

/* functions for unitary tests */
void DMX_TestUnitary_InitTerm( void );
void DMX_TestUnitary_AllocateChanNoInit( void );
void DMX_TestUnitary_AllocateChanMax( void );
void DMX_TestUnitary_FreeChanBadId( void );
void DMX_TestUnitary_RegisterCallBackBadParam( void );
void DMX_TestUnitary_RegisterCallBackGoodParam( void );
void DMX_TestUnitary_SetChanPidGoodParam( void );
void DMX_TestUnitary_SetChanPidBadParam( void );
void DMX_TestUnitary_SetChanPidAlreadyUsed( void );
void DMX_TestUnitary_AllocateFilterBadChannel( void );
void DMX_TestUnitary_AllocateFilterMax( void );
void DMX_TestUnitary_SetFilterBadFilter( void );
void DMX_TestUnitary_SetFilterBadParam( void );
void DMX_TestUnitary_ControlChanBadChannel( void );
void DMX_TestUnitary_ControlChanBadParam( void );
void DMX_TestUnitary_ControlChanGoodParam( void );
void DMX_TestUnitary_ControlFilterBadFilter( void );
void DMX_TestUnitary_ControlFilterBadParam( void );
void DMX_TestUnitary_FreeUnusedFilter( void );
void DMX_TestUnitary_FreeFilterNominalCase( void );
void DMX_TestUnitary_FreeUnusedChan( void );


void DMX_TablesSectionsTest( void );
void DMX_StressTest( void );
void DMX_StressTest2( void );
void DMX_StressTestDSMCC( void );

void DMX_Scenario_SDTNegativeFiltering( void );
void DMX_Scenario_SDTFilterSet( void );
void DMX_Scenario_PesFilterSet( void );
void DMX_Scenario_PesTwoFilterSet( void );
void DMX_Scenario_SectionTwoFilterSet( void );
void DMX_Scenario_SDT_NIT_TDT_TOTFilterSet( void );


/******************************************************
 *              DMD test tree                         *
 ******************************************************/

extern tTestNode gTDAL_DMX_UnitaryTestSuite;
extern tTestNode gTDAL_DMX_TablesSectionsTest;
extern tTestNode gTDAL_DMX_ScenarioTests;
extern tTestNode gTDAL_DMX_StressTests;
extern tTestNode* gTDAL_DMXTestNodeList[];
extern tTestNode* gTDAL_DMXTestFunctionnalNodeList[];

/* To configure which Demod/Demux indexes to use in the dmx tests */
extern int gTDAL_DMX_TestFeIdx;
extern int gTDAL_DMX_TestDmxIdx;

extern tTDAL_DMX_DemuxId gTDAL_DMX_TestDmxIds[];

#endif /* _DMX_TESTS_H */
