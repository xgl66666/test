/** \file hdmitests.h
 *  \brief Declaration of functions for tests of the HDMI layer
 *  \author Pascal PIGNON
 *  \date 17/05/06
 *  \version 1.0
 */

#ifndef _HDMI_TESTS_H
#define _HDMI_TESTS_H

/******************************************************
 *              Include                               *
 ******************************************************/

#include "tdal_hdmi.h"

#include "TestManager.h"

/******************************************************
 *              Some defines                          *
 ******************************************************/


 /* Specific SPD data */
#define SPD_VERSION 			0x00 /* Dummy version */
#define VENDOR_NAME  "DUMMY" /* Dummy vendor name for SPD info frame */
#define DESCRIPTION  "Dummy for tests" /* Dummy vendor name for SPD info frame */
#define SRC_DEVICE 	eTDAL_HDMI_SRC_DEVICE_INFO_DIG_STB /* Dummy vendor name for SPD info frame */
 
/* Specific VS data */
#define VS_VERSION 			0x00 /* Dummy version */
/* Dummy OUI */
#define IEEE_REGISTER_0 0x00
#define IEEE_REGISTER_1 0x00
#define IEEE_REGISTER_2 0x00

#define PAYLOAD_LENGTH  0x00

 
 /* Specific HDCP data */
#define DUMMY_IV_0  0 
#define DUMMY_IV_1  0
#define DUMMY_KSV_0 0 
#define DUMMY_KSV_1 0
#define DUMMY_DEVICE_KEY 0






/******************************************************
 *              Function declaration                  *
 ******************************************************/

void HDMI_TestUnitary_BadParam(void);

void DummyCallback(tTDAL_HDMI_SourceId sourceId, tTDAL_HDMI_Event Event);
void HDMI_TestFunctionalInitTerm(void);
void HDMI_TestFunctionalStartStop(void);
void HDMI_TestFunctionalEnableDisable(void);
void HDMI_TestFunctionalInitStatus(void);
void HDMI_TestFunctionalHotplugDetect(void);
void HDMI_TestFunctionalEDIDTest(void);
void HDMI_TestFunctionalHDCPTest(void);
void HDMI_TestFunctionalInfoFrameSPD(void);
void HDMI_TestFunctionalInfoFrameVS(void);
void HDMI_TestFunctionalAudioVideo(void);

void HDMI_TestStressPlugUnplug(void);

void HDMI_TestReliabilityRandom(void);

/*useful function */
void TestHelper_HDMI_Term( void );
void TestHelper_HDMI_Init( void );

/******************************************************
 *              HDMI test tree                        *
 ******************************************************/


extern tTestNode* gHDMIFunctionalTestNodeList[];
extern tTestNode* gTDAL_HDMITestNodeList[];

extern tTestNode gTDAL_HDMI_TestSuite;
extern  tTestNode gTDAL_HDMI_UnitaryTestSuite;
extern  tTestNode gTDAL_HDMI_FunctionalTestSuite; /* Functional */
extern  tTestNode gTDAL_HDMI_StressTestSuite; /* Stress tests */
extern  tTestNode gTDAL_HDMI_ReliabilityTestSuite; /* Reliability tests */
         

#endif /* _HDMI_TESTS_H */
