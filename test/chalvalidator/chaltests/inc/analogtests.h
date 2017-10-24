/** \file analogtests.h
 *  \brief Declaration of functions for tests of the DMD layer
 *  \author
 *  \date
 *  \version 1.0
 */

#ifndef _ANALOG_TESTS_H
#define _ANALOG_TESTS_H

/******************************************************
 *              Include                               *
 ******************************************************/

#include "tdal_dmd.h"
#ifdef USE_TDAL_ANALOG
#include "tdal_ana_aud.h"
#include "tdal_ana_vid.h"
#include "tdal_ana_input.h"
#endif

#include "TestManager.h"

/******************************************************
 *              Defines                               *
 ******************************************************/

/******************************************************
 *              Function declaration                  *
 ******************************************************/
/* functions for scenario tests */
void TestSce_TuneAnalogFreq( void );
void TestSce_TuneAnalogTwoFreq( void );
void TestSce_TuneAnalogUnplugg( void );
void TestSce_TuneAnalogDigital( void );
void TestSce_TuneVideoAudioAnalog( void );
void TestSce_AnaInputVideoAudioAnalog( void );
void TestSce_ScanningAnalog( void );


void TestSce_HDMIInputVideoAudioPlay(void);
void TestSce_2AnaInputVideoAudioAnalogPIP(void);
void TestSce_TunerAnaInputVideoAudioAnalogPIP(void);
void TestSce_1AnaInput1HDMIVideoAudioAnalogPIP(void);
void TestSce_1TunerDigital1AnalogVideoAudioAnalogPIP(void);

void TestSce_AtvDtvTuneVideoAudio(void);
void TestSce_AtvDtvTuneSection(void);

extern tTestNode gTDAL_ANALOG_TestSuite;
extern tTestNode* gTDAL_AnalogTestNodeList[];
extern tTestNode gTDAL_ANALOG_ScenarioTestSuite;

extern tTestNode gTDAL_ANALOG_AtvDtvTestSuite;


#endif /* _ANALOG_TESTS_H */
