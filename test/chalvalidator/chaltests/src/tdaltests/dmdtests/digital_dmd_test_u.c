/** \file dmd_test_u.c
 *  \brief Unitary Tests for the DMD layer of the TDAL API
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_dmd.h"

#include "testhelper.h"
#include "dmdtests.h"
#include "tester_prefs_dmd.h"



/******************************************************
 * 				Defines								  *
 ******************************************************/
mTBOX_SET_MODULE(eTDAL_TEST_DMD);

/******************************************************
 * 				Local variables						  *
 ******************************************************/

/******************************************************
 *              DMD test tree                         *
 ******************************************************/

static tTestNode gTestUInitTerm = {
	"TestUInitTerm",
	"test TDAL_DMD_Init() and TDAL_DMD_Term() functions",
	0, /* not interactive */
	TestUnitary_InitTerm,
	NULL
};

static tTestNode gTestUConfig = {
	"TestUConfig",
	"test TDAL_DMD_Config() function",
	0, /* not interactive */
	TestUnitary_Config,
	NULL
};

static tTestNode gTestUTune = {
	"TestUTune",
	"test TDAL_DMD_Tune() function",
	0, /* not interactive */
	TestUnitary_Tune,
	NULL
};

static tTestNode gTestUGetInfo = {
	"TestUGetInfo",
	"test TDAL_DMD_GetInfo function",
	0,
	TestUnitary_GetInfo,
	NULL
};

static tTestNode gTestUUnlock = {
	"TestUUnlock",
	"test TDAL_DMD_Unlock function",
	0,
	TestUnitary_Unlock,
	NULL
};

tTestNode* gDMDUnitaryTestNodeList[] = {
	&gTestUInitTerm,
	&gTestUConfig,
	&gTestUTune,
	&gTestUGetInfo,
	&gTestUUnlock,
	NULL
};

tTestNode gTDAL_DMD_UnitaryTestSuite = {
	"TDAL_DMD Unitary Tests",
	"test the TDAL_DMD function input parameters",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gDMDUnitaryTestNodeList
};

/******************************************************
 * 				Unitary tests 						  *
 ******************************************************/

/*! \fn void TestUnitary_InitTerm( void )
 *  \brief Test TDAL_DMD_Init() and TDAL_DMD_Terminate() functions
 *  \bug Known to return 2 errors due to ePio bug
 \verbatim
 Standard Init and Terminate
 Twice Init and Terminate
 Init with bad parameters
 Terminate with bad FE id
 \endverbatim
 */

void TestUnitary_InitTerm()
{
#if (NUMBER_OF_DMD==2)
	unsigned char ucFeID3;
#endif

	TestManager_LogMessage("1-1st DMD Init\n");
	/* Standard Init and Terminate */
   TestManager_AssertEqual( TDAL_DMD_Init(), \
							eTDAL_DMD_NO_ERROR, "DMD init" );	
	
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_0, \
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_NO_ERROR, "1st open instance" );
#if (NUMBER_OF_DMD==2)
	TestManager_LogMessage("1-2nd DMD Init\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_1, \
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_NO_ERROR, "2nd open instance " );
#endif
	TestManager_LogMessage("1-1st DMD close instance\n");
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(eTDAL_DMD_FE_0), eTDAL_DMD_NO_ERROR, "1st DMD close instance" );
#if (NUMBER_OF_DMD==2)
	TestManager_LogMessage("1-2nd DMD close instance\n");
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(eTDAL_DMD_FE_1), eTDAL_DMD_NO_ERROR, "2nd DMD close instance" );
#endif

	TestManager_LogMessage("DMD Terminate\n");
	TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate" );


    TestManager_AssertEqual( TDAL_DMD_Init(), \
							eTDAL_DMD_NO_ERROR, "DMD init" );	


	/* Twice or third Init and Terminate */
	TestManager_LogMessage("2-1st DMD Init\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_0, \
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_NO_ERROR, "2 - 1st DMD open instance" );
#if (NUMBER_OF_DMD==2)
	TestManager_LogMessage("2-2nd DMD Init\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_1, \
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_NO_ERROR, "2 - 2nd DMD open instance" );
	TestManager_LogMessage("2-3rd DMD Init\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_2, \
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_BAD_PARAMETER, "3rd DMD init" );
	TestManager_LogMessage("2-1st DMD Terminate\n");
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(eTDAL_DMD_FE_0), eTDAL_DMD_NO_ERROR, "1st DMD close instance" );
	TestManager_LogMessage("2-2nd DMD Terimnate\n");
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(eTDAL_DMD_FE_1), eTDAL_DMD_NO_ERROR, "2nd DMD close instance" );
#else
// we doesn't have more than 1 DMD
#if 0
	TestManager_LogMessage("2-2nd DMD Init !~!\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_1, \
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_BAD_PARAMETER, "2nd DMD open instance" );
	TestManager_LogMessage("2-1st DMD Terminate\n");
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(eTDAL_DMD_FE_0), eTDAL_DMD_NO_ERROR, "DMD terminate" );
#endif
#endif

	/* Init with bad parameters */
	TestManager_LogMessage("3-1st DMD Init bad FE Id\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_3,\
	                        pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_BAD_PARAMETER, "DMD open instance (null FE ID)" );
	TestManager_LogMessage("3-1st DMD Init bad type\n");
	TestManager_LogMessage("3-1st DMD Init null calback status\n");
	
	
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_0, \
							NULL, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_BAD_PARAMETER, "DMD open instance (Null callback status)" );
	TestManager_LogMessage("3-1st DMD Init null callback progress\n");
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_0, \
							pTestHelper_DMDStatusFct, \
							NULL), \
							eTDAL_DMD_BAD_PARAMETER, "DMD open instance (Null callback progress)" );
	
	TestManager_LogMessage("4-1st DMD Terminate bad FE Id\n");
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(15), eTDAL_DMD_BAD_DEMODULATOR, "DMD terminate (Unknown FE ID)" );

	TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate" );
}

/*! \fn void TestUnitary_Config( void )
 *  \brief Test TDAL_DMD_Config() function
 */

void TestUnitary_Config()
{
	unsigned char 			ucFeID=0, wrongFeID;
	tTDAL_DMD_ConfigData 	DMD_Config;
#if defined(MEDIUM_TER)
	uint32_t aBandWidthList[4] = {BANDWIDTH_1, BANDWIDTH_2, BANDWIDTH_3, 0}; /* Hz */
#endif
	
	TestManager_AssertEqual( TDAL_DMD_Init(), \
							eTDAL_DMD_NO_ERROR, "DMD init" );	
	
	TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eTDAL_DMD_FE_0,
							pTestHelper_DMDStatusFct, \
							pTestHelper_DMDProgressFct), \
							eTDAL_DMD_NO_ERROR, "DMD init" );
	/* test TDAL_DMD_Config */
#if defined(MEDIUM_SAT)

//printf("\n\t MEDIUM_SAT \n\n");
	DMD_Config.Sat.ConfigType = eTDAL_DMD_CFG_SAT_INIT;
	DMD_Config.Sat.uCfg.Init.SwitchFreq = SWITCH_FREQ;
	DMD_Config.Sat.uCfg.Init.Vertical = eTDAL_DMD_LNB_POWER_13V;
	DMD_Config.Sat.uCfg.Init.Horizontal = eTDAL_DMD_LNB_POWER_18V;
	DMD_Config.Sat.uCfg.Init.pSymbolRateList = NULL;
	DMD_Config.Sat.ConfigType = eTDAL_DMD_CFG_SAT_OL_BAND;
	DMD_Config.Sat.uCfg.OlBand.BandType = eTDAL_DMD_BANDTYPE_KU; /* Europe */
	DMD_Config.Sat.uCfg.OlBand.LowLNBFreq = LOW_LNB_FREQ; /* Low OL */
	DMD_Config.Sat.uCfg.OlBand.HighLNBFreq = HIGH_LNB_FREQ; /* High OL */
#elif defined(MEDIUM_TER)
	DMD_Config.Ter.ConfigType = eTDAL_DMD_CFG_TER_INIT;
	DMD_Config.Ter.uCfg.Init.pBandWidthList = aBandWidthList;	
#elif defined(MEDIUM_CAB)
    DMD_Config.Cab.ConfigType = eTDAL_DMD_CFG_CAB_INIT;
    DMD_Config.Cab.uCfg.Init.pSymbolRateList = NULL;
#endif /* MEDIUM */

	wrongFeID = ucFeID^0xFF;

	//printf("\tt DMD_Config.Sat.ConfigTyp = %d \n\n", DMD_Config.Sat.ConfigType);
	

	TestManager_AssertEqual( TDAL_DMD_Config(wrongFeID, &DMD_Config), eTDAL_DMD_BAD_DEMODULATOR, "dmd config (Bad FE ID)" );
	TestManager_AssertEqual( TDAL_DMD_Config(eTDAL_DMD_FE_0, NULL), eTDAL_DMD_BAD_PARAMETER, "dmd config (Null config parameters)" );
	TestManager_AssertEqual( TDAL_DMD_Config(eTDAL_DMD_FE_0, &DMD_Config), eTDAL_DMD_NO_ERROR, "dmd config" );
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(eTDAL_DMD_FE_0), eTDAL_DMD_NO_ERROR, "DMD close instance" );
	
	TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate" );
}

/*! \fn void TestUnitary_Tune( void )
 *  \brief Test TDAL_DMD_Tune() function
 \verbatim
 Init and config
 Test TDAL_DMD_Tune() with different bad parameters
 Termintate DMD
 \endverbatim
 */

void TestUnitary_Tune()
{
	unsigned char 		ucFeID;
	tTDAL_DMD_TunData	DMD_TuneParam;
    uint32_t            uiFrequency=0;
		
	/* Init and config */
	TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	
	/* Test TDAL_DMD_Tune() with different bad parameters */
	TestManager_AssertEqual( TDAL_DMD_Tune(ucFeID+1, &DMD_TuneParam, uiFrequency), eTDAL_DMD_BAD_DEMODULATOR, "dmd tune (Bad FeID)" );
	TestManager_AssertEqual( TDAL_DMD_Tune(ucFeID, &DMD_TuneParam, uiFrequency), eTDAL_DMD_BAD_PARAMETER, "dmd tune (wrong params)" );
	uiFrequency = FREQUENCY;

#if defined(MEDIUM_SAT)
    DMD_TuneParam.Sat.SymbolRateBw = SYM_RATE;
    DMD_TuneParam.Sat.Modulation = MODULATION;
	DMD_TuneParam.Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
	DMD_TuneParam.Sat.Convolution = CONVOLUTION;
	DMD_TuneParam.Sat.Polarization = POLARIZATION;
	mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - FEC %x - Pola %x]\n",
				uiFrequency,
				DMD_TuneParam.Sat.SymbolRateBw,
				DMD_TuneParam.Sat.Convolution,
				DMD_TuneParam.Sat.Polarization));
#elif defined(MEDIUM_TER)

    DMD_TuneParam.Sat.SymbolRateBw = SYM_RATE;
    DMD_TuneParam.Sat.Modulation = MODULATION;
	DMD_TuneParam.Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
	DMD_TuneParam.Sat.Convolution = CONVOLUTION;
	DMD_TuneParam.Sat.Polarization = POLARIZATION;

	mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - FEC %x - Pola %x]\n",
				uiFrequency,
				DMD_TuneParam.Sat.SymbolRateBw,
				DMD_TuneParam.Sat.Convolution,
				DMD_TuneParam.Sat.Polarization));


/*    DMD_TuneParam.Ter.SymbolRateBw = SYM_RATE;
    DMD_TuneParam.Ter.Modulation = MODULATION;
	DMD_TuneParam.Ter.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
	DMD_TuneParam.Ter.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
	DMD_TuneParam.Ter.Modulation = eTDAL_DMD_MOD_AUTO;
	DMD_TuneParam.Ter.ConvolutionHP = CONVOLUTION;
	DMD_TuneParam.Ter.ConvolutionLP = CONVOLUTION;
	DMD_TuneParam.Ter.Mode = eTDAL_DMD_MODE_AUTO;
	DMD_TuneParam.Ter.Guard = eTDAL_DMD_GUARD_AUTO;
	DMD_TuneParam.Ter.Hierarchy = eTDAL_DMD_HIER_AUTO;
	DMD_TuneParam.Ter.Priority = eTDAL_DMD_PRIORITY_STREAM_HIGH;
	DMD_TuneParam.Ter.Offset = eTDAL_DMD_OFFSET_AUTO;
	mTBOX_TRACE((kTBOX_NIV_1,"Try to lock on [Freq %d - SB %d - FEC %x]\n",
				uiFrequency,
				DMD_TuneParam.Ter.SymbolRateBw,
				DMD_TuneParam.Ter.ConvolutionHP));
*/
#elif defined(MEDIUM_CAB)
			/* There are no specific cable tune parameters */
    DMD_TuneParam.Cab.SymbolRateBw = SYM_RATE;
    DMD_TuneParam.Cab.Modulation = MODULATION;
	
	DMD_TuneParam.Cab.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
    mTBOX_TRACE((kTBOX_NIV_1,"Try to lock on [Freq %d - SB %d]\n",
                uiFrequency,
                DMD_TuneParam.Cab.SymbolRateBw,
                DMD_TuneParam.Cab.Modulation));
#endif

#if 0
    DMD_TuneParam.Sat.SymbolRateBw = 22000000;
    DMD_TuneParam.Sat.Modulation = MODULATION;
	DMD_TuneParam.Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
	DMD_TuneParam.Sat.Convolution = 9;
	DMD_TuneParam.Sat.Polarization = POLARIZATION;
	mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - FEC %x - Pola %x]\n",
				uiFrequency,
				DMD_TuneParam.Sat.SymbolRateBw,
				DMD_TuneParam.Sat.Convolution,
				DMD_TuneParam.Sat.Polarization));


	TestManager_AssertEqual( TDAL_DMD_Tune(ucFeID, &DMD_TuneParam, 10735000), eTDAL_DMD_NO_ERROR, "dmd tune" );
#endif	
	TestManager_AssertEqual( TDAL_DMD_Tune(ucFeID, &DMD_TuneParam, uiFrequency), eTDAL_DMD_NO_ERROR, "dmd tune" );

	TestHelper_DMD_WaitStatus(ucFeID);

	/* termintate DMD */
	TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(ucFeID), eTDAL_DMD_NO_ERROR, "DMD close instance" );
	TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate" );
}

/*! \fn void TestUnitary_GetInfo( void )
 *  \brief Test TDAL_DMD_GetInfo() function
 \verbatim
 Configure the demod and tune a frequency
 Test TDAL_DMD_GetInfo() with a bad FeId and also a NULL second parameter
 Termintate DMD
 \endverbatim
 */

void TestUnitary_GetInfo()
{
	 unsigned char       	ucFeID;
	 tTDAL_DMD_Info			DMD_Info;
	 
	 /* Configure the demod and tune a frequency */

	 TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	 TestHelper_DMD_Tune(&ucFeID);
	 /* Test TDAL_DMD_GetInfo() with a bad FeId and also a NULL second parameter */
	 TestManager_AssertEqual( TDAL_DMD_GetInfo(ucFeID+1, &DMD_Info), eTDAL_DMD_BAD_DEMODULATOR, "dmd info (wrong FeID)" );
	 TestManager_AssertEqual( TDAL_DMD_GetInfo(ucFeID, NULL), eTDAL_DMD_BAD_PARAMETER, "dmd info (Null param)" );
	 TestManager_AssertEqual( TDAL_DMD_GetInfo(ucFeID, &DMD_Info), eTDAL_DMD_NO_ERROR, "dmd info~" );

mTBOX_TRACE((kTBOX_NIV_1,"DMD_Info->TunFrequency = %d\n" , DMD_Info.TunFrequency));
mTBOX_TRACE((kTBOX_NIV_1,"DMD_Info->CarrierStatus = %d\n" , DMD_Info.CarrierStatus));
mTBOX_TRACE((kTBOX_NIV_1,"DMD_Info->eSourceState = %d\n" , DMD_Info.eSourceState));
mTBOX_TRACE((kTBOX_NIV_1,"DMD_Info->SignalBER = %d\n" , DMD_Info.SignalBER));
mTBOX_TRACE((kTBOX_NIV_1,"DMD_Info->SignalQuality = %d\n" , DMD_Info.SignalQuality));
mTBOX_TRACE((kTBOX_NIV_1,"DMD_Info->SymbolRateBw = %d\n" , DMD_Info.TunData.Sat.SymbolRateBw));



	 /* termintate DMD */
	 TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(ucFeID), eTDAL_DMD_NO_ERROR, "DMD terminate" );	 	
	 TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate" );	 	
}

/*! \fn void TestUnitary_Unlock( void )
 *  \brief Test TDAL_DMD_Unlock() function
 \verbatim
 Configure the demod
 Test TDAL_DMD_Unlock() with a wrong FeID and try to unlock when nothing is locked
 Tune a channel and check that TDAL_DMD_Unlock() do its work
 Termintate DMD
 \endverbatim
 */

void TestUnitary_Unlock()
{
	unsigned char 			ucFeID;

	/* Configure the demod */
	 TestHelper_DMD_Config(&ucFeID, eTDAL_DMD_CFG_SOURCE_IS_DTV);
	
	 /* Test TDAL_DMD_Unlock() with a wrong FeID and try to unlock when nothing is locked */
	 TestManager_AssertEqual( TDAL_DMD_Unlock(ucFeID+1), eTDAL_DMD_BAD_DEMODULATOR, "dmd unlock (wrong FeID)" );
	 TestManager_AssertEqual( TDAL_DMD_Unlock(ucFeID), eTDAL_DMD_NOT_LOCKED, "dmd unlock (no tune)" );

	 /* Tune a channel and check that TDAL_DMD_Unlock() do its work */
	 TestHelper_DMD_Tune(&ucFeID);
	 TestManager_AssertEqual( TDAL_DMD_Unlock(ucFeID), eTDAL_DMD_NO_ERROR, "dmd unlock" );
	
	 /* termintate DMD */
	 TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(ucFeID), eTDAL_DMD_NO_ERROR, "DMD close instance" );
	 TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate" );
}

