/** \file dmd_test_helper.c
 *  \brief Helper functions to write tests
 *  for the DMD layer of the TDAL API.
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 *  \warning Only use for DMD tests
 */

/******************************************************
 * 				Includes                              *
 ******************************************************/

#include "crules.h"
#include <string.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_dmd.h"
/*#include "tdal_common.h"*/

#include "testhelper.h"
#include "dmdtests.h"
#include "tester_prefs_dmd.h"

/******************************************************
 * 				Defines								  *
 ******************************************************/
mTBOX_SET_MODULE(eTDAL_TEST_DMD);

/******************************************************
 * 				Global variables				      *
 ******************************************************/
extern bool   bUseAnalogChannel;
extern int    gChannelAnalog;
extern struct TestManager_Channel_Analog gAnalogChannelList[];

extern int    gChannel;

/******************************************************
 * 				Local variables						  *
 ******************************************************/
/* DMD settings */
/*! \var static t_testTune TuneFakeChan[1]
 *  \brief Variable which stores a fake channel with invalid signal features
 *
 *  This variable is used to perform a TDAL_DMD_Tune() which must fail.
 *
 *  \see TestPerf_TuneFailed()
 */
t_testTune TuneFakeChan[1] =
{
#if defined(MEDIUM_SAT)
    {"fakechan", 10954000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}
#elif defined(MEDIUM_TER)
    {"fakechan", 556000000, 8000000, eTDAL_DMD_FEC_5_6, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_AUTO}
#elif defined(MEDIUM_CAB)
    {"fakechan", 500000000, 6875000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QAM64}
#endif
};


LOCAL TKEL_sema_id           TestHelper_DMD_NotifySem[NUMBER_OF_DMD]=
{
	NULL,
#if (NUMBER_OF_DMD==2)
	NULL,
#endif
};

LOCAL uint8_t                TestHelper_DMD_FeID[NUMBER_OF_DMD];

LOCAL tTDAL_DMD_NotifyStatus TestHelper_DMD_NotifyStatus[NUMBER_OF_DMD];
LOCAL int                    TestHelper_DMD_CurrentIdx = 0;

LOCAL void pTestHelper_DMD_StatusFct(uint8_t ucFeID, tTDAL_DMD_NotifyStatus Status);
LOCAL void pTestHelper_DMD_ProgressFct(uint8_t ucFeID, tTDAL_DMD_NotifyProgress Progress);

/******************************************************
 * 			test helper functions					  *
 ******************************************************/

/*! \fn void TestHelper_DMD_Config( unsigned char *ucFeID, int dmdIdx)
 *  \param[out] *ucFeID pointer to the Front End ID
 *  \param[in] dmdIdx index of the demod if several demod available
 *  \brief Helper function to intialize and configure the demod
 */
void TestHelper_DMD_Config(unsigned char *ucFeID, tTDAL_DMD_TerSourceState SourceState)
{
	tTDAL_DMD_ConfigData DMD_Config;
	tTDAL_DMD_Capability DMD_Capability;
	tTDAL_DMD_FECapability DMD_FECapability;
	tTDAL_DMD_FE             eFeId;
#if defined(MEDIUM_TER)
	uint32_t aBandWidthList[4] = {BANDWIDTH_1, BANDWIDTH_2, BANDWIDTH_3, 0}; /* Hz */
#endif
	int idx;

	TestManager_AssertGreater(NUMBER_OF_DMD, TestHelper_DMD_CurrentIdx, "Current Demod index");

	if(TestHelper_DMD_CurrentIdx < NUMBER_OF_DMD)
	{
		/* create semaphore for callback */
		if(TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx] == NULL)
		{
			TestManager_AssertEqual(TKEL_CreateSemaphore(0,
														 &TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx]),
									TKEL_NO_ERR,
									"DMD notify semaphore");
		}

		TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx] = eTDAL_DMD_SIGNAL_LOST;

        /* */
        TestManager_AssertEqual(TDAL_DMD_Init(),eTDAL_DMD_NO_ERROR, "DMD init" );
        TestManager_AssertEqual(TDAL_DMD_GetCapability(&DMD_Capability),eTDAL_DMD_NO_ERROR, "DMD Get capability" );
        mTBOX_TRACE((kTBOX_NIV_1, "nb Front end available %d\n",
						 DMD_Capability.uiFENumber));

		/* loop to get good FeID */

		/* the DMD_FeID is started to 0 */
		eFeId = eTDAL_DMD_FE_0;

		for(idx = 0; idx < (TestHelper_DMD_CurrentIdx+1) && idx < DMD_Capability.uiFENumber; idx++, eFeId++)
		{
            TestManager_AssertEqual(TDAL_DMD_GetFECapability(eFeId,
															 &DMD_FECapability),
									eTDAL_DMD_NO_ERROR,
									"DMD Get FE capability" );

            mTBOX_TRACE((kTBOX_NIV_1, "FEId %d type %d sourceSupport %d\n",eFeId,
						 DMD_FECapability.eType,
						 DMD_FECapability.eSourceSupport));

			TestManager_AssertEqual( TDAL_DMD_OpenFEInstance(eFeId,
                                                             (tTDAL_DMD_NotifyStatusFct)pTestHelper_DMD_StatusFct,
                                                             (tTDAL_DMD_NotifyProgressFct)pTestHelper_DMD_ProgressFct),
                                     eTDAL_DMD_NO_ERROR,
                                     "DMD Open FE Instance ");
			TestHelper_DMD_FeID[idx] = eFeId;
			*ucFeID = eFeId;
		}


#if defined(MEDIUM_SAT)
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
        DMD_Config.Ter.ConfigType               = eTDAL_DMD_CFG_TER_SOURCE_TRANSITION;
        if(SourceState == eTDAL_DMD_CFG_SOURCE_IS_ATV)
            DMD_Config.Ter.uCfg.SourceTransition = eTDAL_DMD_CFG_SOURCE_IS_ATV;
        else
            DMD_Config.Ter.uCfg.SourceTransition = eTDAL_DMD_CFG_SOURCE_IS_DTV;
		TestManager_AssertEqual( TDAL_DMD_Config(*ucFeID, &DMD_Config), eTDAL_DMD_NO_ERROR, "dmd config SOURCE_TRANSITION" );
		
        DMD_Config.Ter.ConfigType = eTDAL_DMD_CFG_TER_INIT;
		DMD_Config.Ter.uCfg.Init.pBandWidthList = aBandWidthList;
#elif defined(MEDIUM_CAB)
		DMD_Config.Cab.ConfigType = eTDAL_DMD_CFG_CAB_INIT;
		DMD_Config.Cab.uCfg.Init.pSymbolRateList = NULL;
#endif
		TestManager_AssertEqual( TDAL_DMD_Config(*ucFeID, &DMD_Config), eTDAL_DMD_NO_ERROR, "dmd config INIT" );

	}
}

/*! \fn void TestHelper_DMD_TuneParam(char *channel, tTDAL_DMD_TunData *DMD_TuneParam)
 *  \brief Helper function to set tTDAL_DMD_TunData struct
 *
 *  This function searches for a valid channel.
 *  If no valid channel is found, check for the FAKE_CHANNEL
 *  Else set the default channel i.e. the first one present
 *  in tester_prefs_dmd.h file ( TuneChannel[0] = CHANNEL_1 )
 *
 *  \param[in] *channel name of the channel to tune. See  tester_prefs_dmd.h file
 *  for the list of available channels
 *  \param[out] *DMD_TuneParam pointer to the tTDAL_DMD_TunData struct
 */
void TestHelper_DMD_TuneParam(char *channel, tTDAL_DMD_TunData *DMD_TuneParam, uint32_t *uiFrequency)
{
	int idx = 0;
	struct TestManager_Channel_Analog *pstChannelAnalog;

    if(bUseAnalogChannel)
    {
		pstChannelAnalog = &gAnalogChannelList[0];

		while(pstChannelAnalog->mChannelName != NULL)
		{
			if(strcmp(channel, pstChannelAnalog->mChannelName) == 0)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "tune analog channel: %s on freq=%d\n",
							 pstChannelAnalog->mChannelName,
							 pstChannelAnalog->TunFrequency));
				DMD_TuneParam->AnalogTer.eStandardSystem                   = pstChannelAnalog->AnalogTer.eStandardSystem;
				DMD_TuneParam->AnalogTer.SymbolRateBw                      = pstChannelAnalog->AnalogTer.SymbolRateBw;
				DMD_TuneParam->AnalogTer.bEnableDriftMonitoring            = pstChannelAnalog->AnalogTer.bEnableDriftMonitoring;
				DMD_TuneParam->AnalogTer.lManualDriftToApply               = pstChannelAnalog->AnalogTer.lManualDriftToApply;

				*uiFrequency = pstChannelAnalog->TunFrequency;

				break;
			}

			pstChannelAnalog++;
		}

		/* test if we have found a valid channel
		 * else set TuneParam to a default value */
		if( pstChannelAnalog->mChannelName == NULL )
		{
			mTBOX_TRACE((kTBOX_NIV_1, "No corresponding analog channel found\n Using default\n"));

			/* we suppose that idx 0 is always fill with a good analog channel
			 * that is the default channel */
			pstChannelAnalog = &gAnalogChannelList[0];

			DMD_TuneParam->AnalogTer.eStandardSystem                   = pstChannelAnalog->AnalogTer.eStandardSystem;
			DMD_TuneParam->AnalogTer.SymbolRateBw                      = pstChannelAnalog->AnalogTer.SymbolRateBw;
			DMD_TuneParam->AnalogTer.bEnableDriftMonitoring            = pstChannelAnalog->AnalogTer.bEnableDriftMonitoring;
			DMD_TuneParam->AnalogTer.lManualDriftToApply               = pstChannelAnalog->AnalogTer.lManualDriftToApply;
			
			*uiFrequency = pstChannelAnalog->TunFrequency;
		}

	}
	else
	{
		/* search for the channel in the known list
		 * the known list is in tester_prefs_dmd.h file */
		for(idx = 0; idx < NUMBER_CHANNEL; idx++)
		{
			if(strcmp(channel, TuneChannel[idx].name) == 0)
			{
				mTBOX_TRACE((kTBOX_NIV_1, "tune channel: %s on freq=%d\n",
							 TuneChannel[idx].name,
							 TuneChannel[idx].frequency));
				*uiFrequency = TuneChannel[idx].frequency;
#if defined(MEDIUM_SAT)
				DMD_TuneParam->Sat.SymbolRateBw = TuneChannel[idx].symbolRate;
				DMD_TuneParam->Sat.Modulation = TuneChannel[idx].modulation;
				DMD_TuneParam->Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
				DMD_TuneParam->Sat.Convolution = TuneChannel[idx].convolution;
				DMD_TuneParam->Sat.Polarization = TuneChannel[idx].polarization;
#elif defined(MEDIUM_TER)
				DMD_TuneParam->Ter.SymbolRateBw = TuneChannel[idx].symbolRate;
				DMD_TuneParam->Ter.Modulation = TuneChannel[idx].modulation;
				DMD_TuneParam->Ter.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
				DMD_TuneParam->Ter.ConvolutionHP = TuneChannel[idx].convolution;
				DMD_TuneParam->Ter.ConvolutionLP = TuneChannel[idx].convolution;
				DMD_TuneParam->Ter.Mode = eTDAL_DMD_MODE_AUTO;
				DMD_TuneParam->Ter.Guard = eTDAL_DMD_GUARD_AUTO;
				DMD_TuneParam->Ter.Hierarchy = eTDAL_DMD_HIER_AUTO;
				DMD_TuneParam->Ter.Priority = eTDAL_DMD_PRIORITY_STREAM_HIGH;
				DMD_TuneParam->Ter.Offset = eTDAL_DMD_OFFSET_AUTO;
#elif defined(MEDIUM_CAB)
				DMD_TuneParam->Cab.SymbolRateBw = TuneChannel[idx].symbolRate;
				DMD_TuneParam->Cab.Modulation = TuneChannel[idx].modulation;
				DMD_TuneParam->Cab.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
#endif
				break;
			}
		}

		/* test for the fake channel */
		if(strcmp(channel, TuneFakeChan[0].name) == 0)
		{
			*uiFrequency = TuneFakeChan[0].frequency;

#if defined(MEDIUM_SAT)
			DMD_TuneParam->Sat.SymbolRateBw = TuneFakeChan[0].symbolRate;
			DMD_TuneParam->Sat.Modulation = TuneFakeChan[0].modulation;
			DMD_TuneParam->Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
			DMD_TuneParam->Sat.Convolution = TuneFakeChan[0].convolution;
			DMD_TuneParam->Sat.Polarization = TuneFakeChan[0].polarization;
#elif defined(MEDIUM_TER)
			DMD_TuneParam->Ter.SymbolRateBw = TuneFakeChan[0].symbolRate;
			DMD_TuneParam->Ter.Modulation = TuneFakeChan[0].modulation;
			DMD_TuneParam->Ter.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
			DMD_TuneParam->Ter.ConvolutionHP = TuneFakeChan[0].convolution;
			DMD_TuneParam->Ter.ConvolutionLP = TuneFakeChan[0].convolution;
			DMD_TuneParam->Ter.Mode = eTDAL_DMD_MODE_AUTO;
			DMD_TuneParam->Ter.Guard = eTDAL_DMD_GUARD_AUTO;
			DMD_TuneParam->Ter.Hierarchy = eTDAL_DMD_HIER_AUTO;
			DMD_TuneParam->Ter.Priority = eTDAL_DMD_PRIORITY_STREAM_HIGH;
			DMD_TuneParam->Ter.Offset = eTDAL_DMD_OFFSET_AUTO;
#elif defined(MEDIUM_CAB)
				/* There are no specific cable tune parameters */
			DMD_TuneParam->Cab.SymbolRateBw = TuneFakeChan[0].symbolRate;
			DMD_TuneParam->Cab.Modulation = TuneFakeChan[0].modulation;
			DMD_TuneParam->Cab.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
#endif
		}

		/* test if we have found a valid channel
		 * else set TuneParam to a default value */
		if( (idx == NUMBER_CHANNEL) && (*uiFrequency != TuneFakeChan[0].frequency) )
		{
			mTBOX_TRACE((kTBOX_NIV_1, "No corresponding channel found\n Using default\n"));
			*uiFrequency = FREQUENCY;
#if defined(MEDIUM_SAT)
			DMD_TuneParam->Sat.SymbolRateBw = SYM_RATE;
			DMD_TuneParam->Sat.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
			DMD_TuneParam->Sat.Modulation = MODULATION;
			DMD_TuneParam->Sat.Convolution = CONVOLUTION;
			DMD_TuneParam->Sat.Polarization = POLARIZATION;
			mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - FEC %x - Pola %x]\n",
						uiFrequency,
						DMD_TuneParam->Sat.SymbolRateBw,
						DMD_TuneParam->Sat.Convolution,
						DMD_TuneParam->Sat.Polarization));
#elif defined(MEDIUM_TER)
			DMD_TuneParam->Ter.SymbolRateBw = SYM_RATE;
			DMD_TuneParam->Ter.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
			DMD_TuneParam->Ter.Modulation = MODULATION;
			DMD_TuneParam->Ter.ConvolutionHP = CONVOLUTION;
			DMD_TuneParam->Ter.ConvolutionLP = CONVOLUTION;
			DMD_TuneParam->Ter.Mode = eTDAL_DMD_MODE_AUTO;
			DMD_TuneParam->Ter.Guard = eTDAL_DMD_GUARD_AUTO;
			DMD_TuneParam->Ter.Hierarchy = eTDAL_DMD_HIER_AUTO;
			DMD_TuneParam->Ter.Priority = eTDAL_DMD_PRIORITY_STREAM_HIGH;
			DMD_TuneParam->Ter.Offset = eTDAL_DMD_OFFSET_AUTO;
			mTBOX_TRACE((kTBOX_NIV_1,"Try to lock on [Freq %d - SB %d - FEC %x]\n",
						uiFrequency,
						DMD_TuneParam->Ter.SymbolRateBw,
						DMD_TuneParam->Ter.ConvolutionHP));
#elif defined(MEDIUM_CAB)
				/* There are no specific cable tune parameters */
			DMD_TuneParam->Cab.SymbolRateBw = SYM_RATE;
			DMD_TuneParam->Cab.SpecInv = eTDAL_DMD_SPECTRUM_AUTO;
			DMD_TuneParam->Cab.Modulation = MODULATION;

			mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - Mod %x]\n",
						uiFrequency,
						DMD_TuneParam->Cab.SymbolRateBw,
						DMD_TuneParam->Cab.Modulation));
#endif
		}
	}
}

/*! \fn void TestHelper_DMD_Tune( unsigned char *ucFeID )
 *  \brief Helper function used to tune a channel
 *
 *  This function manages semaphore for callback
 *  and is only able to tune CHANNEL_1
 *
 *  \param[in] *ucFeID pointer to the Front End ID
 *  \brief Helper function used to tune a channel
 */
void TestHelper_DMD_Tune(unsigned char *ucFeID)
{
	tTDAL_DMD_TunData DMD_TuneParam;
    uint32_t          uiFrequency=0;

	if(bUseAnalogChannel == TRUE)
	{
		TestHelper_DMD_TuneParam(gAnalogChannelList[gChannelAnalog].mChannelName,
								 &DMD_TuneParam,
								 &uiFrequency);
	}
	else
	{
		TestHelper_DMD_TuneParam(TuneChannel[gChannel].name,
								 &DMD_TuneParam,
								 &uiFrequency);
	}

	TestManager_AssertEqual( TDAL_DMD_Tune(*ucFeID, &DMD_TuneParam, uiFrequency), eTDAL_DMD_NO_ERROR, "dmd tune" );

	TestHelper_DMD_WaitStatus(*ucFeID);
}

/*! \fn void TestHelper_DMD_Tune_Chan( unsigned char *ucFeID, char *channel )
 *  \brief Helper function to tune a specific channel
 *
 *  This function does not manage semaphore for callback
 *  but is able to tune any channel present in tester_prefs_dmd.h
 *  file ( TuneChannel[i] with 0 < i < NUMBER_CHANNEL )
 *
 *  \param[in] *ucFeID pointer to the Front End ID
 *  \param[in] *channel name of the channel to tune
 */
void TestHelper_DMD_Tune_Chan(unsigned char *ucFeID, char *channel)
{
	tTDAL_DMD_TunData DMD_TuneParam;
    uint32_t          uiFrequency=0;

	TestHelper_DMD_TuneParam(channel, &DMD_TuneParam, &uiFrequency);
	TestManager_AssertEqual( TDAL_DMD_Tune(*ucFeID, &DMD_TuneParam, uiFrequency), eTDAL_DMD_NO_ERROR, "dmd tune" );
	/* warning: this function does not manage semaphore because it
	 * was specifically implemented for TestSce_TwiceTune */
}

/*! \fn void TestHelper_DMD_GetInfo( unsigned char *ucFeID )
 *  \brief Helper function to retrieve information about current tuned channel
 *
 *  This function displays the following information: <BR>
 *  TunFrequency : frequency of the channel <BR>
 *  SymbolRateBw : symbol rate of the signal <BR>
 *  Modulation : modulation of the signal <BR>
 *  SpecInv : spectrum of the signal <BR>
 *  Convolution : convolution of the signal <BR>
 *  Polarization : polarization of the signal (Only for Satelite) <BR>
 *  CarrierStatus : to know if the channel is locked or not <BR>
 *  SignalQuality : Quality of the signal <BR>
 *  SignalBER : Binary Error Rate of the signal <BR>
 *  SignalLevel : Level of the signal <BR>
 *
 *  \param[in] *ucFeID pointer to the Front End ID
 */
void TestHelper_DMD_GetInfo(unsigned char *ucFeID)
{
	tTDAL_DMD_Info  DMD_Info;

	TestManager_AssertEqual( TDAL_DMD_GetInfo(*ucFeID, &DMD_Info), eTDAL_DMD_NO_ERROR, "dmd config" );
	mTBOX_PRINT((kTBOX_LF, "Info about locked frequency: \n"));
#if defined(MEDIUM_SAT)
	mTBOX_PRINT((kTBOX_LF, "        Medium = Satellite\n"));
	mTBOX_PRINT((kTBOX_LF, "        TunFrequency = %d KHz\n", DMD_Info.TunFrequency));

	if (DMD_Info.TunData.Modulation == eTDAL_DMD_MOD_QPSK)
	{
        mTBOX_PRINT((kTBOX_LF, "SymbolRateBw = %d bauds\n", DMD_Info.TunData.Sat.SymbolRateBw));
	    mTBOX_PRINT((kTBOX_LF, "Modulation = %s\n", \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_BPSK)? "eTDAL_DMD_MOD_BPSK": \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QPSK)? "eTDAL_DMD_MOD_QPSK" : \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QAM4)? "eTDAL_DMD_MOD_QAM4" : \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_8PSK)? "eTDAL_DMD_MOD_8PSK" : \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QAM16)? "eTDAL_DMD_MOD_QAM16" : \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QAM32)? "eTDAL_DMD_MOD_QAM32" : \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QAM64)? "eTDAL_DMD_MOD_QAM64" : \
				(DMD_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QAM128)? "eTDAL_DMD_MOD_QAM128" : \
				"eTDAL_DMD_MOD_QAM256"));
	    mTBOX_PRINT((kTBOX_LF, "SpecInv = %s\n", \
				(DMD_Info.TunData.Sat.SpecInv == eTDAL_DMD_SPECTRUM_NORMAL)? "eTDAL_DMD_SPECTRUM_NORMAL": \
				(DMD_Info.TunData.Sat.SpecInv == eTDAL_DMD_SPECTRUM_INVERTED)? "eTDAL_DMD_SPECTRUM_INVERTED": \
				"eTDAL_DMD_SPECTRUM_AUTO"));

		mTBOX_PRINT((kTBOX_LF, "Convolution = %s\n", \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_NONE)? "eTDAL_DMD_FEC_NONE": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_1_2)? "eTDAL_DMD_FEC_1_2": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_2_3)? "eTDAL_DMD_FEC_2_3": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_3_4)? "eTDAL_DMD_FEC_3_4": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_4_5)? "eTDAL_DMD_FEC_4_5": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_5_6)? "eTDAL_DMD_FEC_5_6": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_6_7)? "eTDAL_DMD_FEC_6_7": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_7_8)? "eTDAL_DMD_FEC_7_8": \
					(DMD_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_8_9)? "eTDAL_DMD_FEC_8_9": \
					"eTDAL_DMD_FEC_AUTO"));
		mTBOX_PRINT((kTBOX_LF, "Polarization = %s\n", \
					(DMD_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_HORIZONTAL)? "eTDAL_DMD_POLAR_HORIZONTAL" : \
					(DMD_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_VERTICAL)? "eTDAL_DMD_POLAR_VERTICAL" : \
					(DMD_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_CIRCULAR_LEFT)? "eTDAL_DMD_POLAR_CIRCULAR_LEFT" : \
					(DMD_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_CIRCULAR_RIGHT)? "eTDAL_DMD_POLAR_CIRCULAR_RIGHT" : \
					"eTDAL_DMD_POLAR_ALL"));
	}
#elif defined(MEDIUM_TER)
    if(bUseAnalogChannel)
    {
		mTBOX_PRINT((kTBOX_LF, "        Medium = Terrestrial Analog\n"));
		mTBOX_PRINT((kTBOX_LF, "        TunFrequency = %d Hz\n", DMD_Info.TunFrequency));

		mTBOX_PRINT((kTBOX_LF, "        CurrentStandard = %s\n", \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_PAL_BG)? "eTDAL_DMD_ANALOG_STD_PAL_BG": \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_PAL_I)? "eTDAL_DMD_ANALOG_STD_PAL_I": \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_PAL_DK)? "eTDAL_DMD_ANALOG_STD_PAL_DK": \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_SECAM_L)? "eTDAL_DMD_ANALOG_STD_SECAM_L": \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_SECAM_L_prime)? "eTDAL_DMD_ANALOG_STD_SECAM_L_prime": \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_SECAM_BG)? "eTDAL_DMD_ANALOG_STD_SECAM_BG": \
					 (DMD_Info.TunData.AnalogTer.eStandardSystem == eTDAL_DMD_ANALOG_STD_SECAM_DK)? "eTDAL_DMD_ANALOG_STD_SECAM_DK": \
					 "??? error"));
        mTBOX_PRINT((kTBOX_LF, "EnableDrift = %d n", DMD_Info.TunData.AnalogTer.bEnableDriftMonitoring));
    }
    else
    {
		 mTBOX_PRINT((kTBOX_LF, "        Medium = Terrestrial Digital\n"));
		 mTBOX_PRINT((kTBOX_LF, "        TunFrequency = %d Hz\n", DMD_Info.TunFrequency));

    	 mTBOX_PRINT((kTBOX_LF, "SymbolRateBw = %d bauds\n", DMD_Info.TunData.Ter.SymbolRateBw));
    	 mTBOX_PRINT((kTBOX_LF, "Modulation = %s\n", \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_BPSK)? "eTDAL_DMD_MOD_BPSK": \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QPSK)? "eTDAL_DMD_MOD_QPSK" : \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM4)? "eTDAL_DMD_MOD_QAM4" : \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_8PSK)? "eTDAL_DMD_MOD_8PSK" : \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM16)? "eTDAL_DMD_MOD_QAM16" : \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM32)? "eTDAL_DMD_MOD_QAM32" : \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM64)? "eTDAL_DMD_MOD_QAM64" : \
    				(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM128)? "eTDAL_DMD_MOD_QAM128" : \
					(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM256)? "eTDAL_DMD_MOD_QAM256" : \
					(DMD_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_AUTO)? "eTDAL_DMD_MOD_AUTO" : \
    				"???"));
    	 mTBOX_PRINT((kTBOX_LF, "SpecInv = %s\n", \
    				(DMD_Info.TunData.Ter.SpecInv == eTDAL_DMD_SPECTRUM_NORMAL)? "eTDAL_DMD_SPECTRUM_NORMAL": \
    				(DMD_Info.TunData.Ter.SpecInv == eTDAL_DMD_SPECTRUM_INVERTED)? "eTDAL_DMD_SPECTRUM_INVERTED": \
					(DMD_Info.TunData.Ter.SpecInv == eTDAL_DMD_SPECTRUM_AUTO)? "eTDAL_DMD_SPECTRUM_AUTO": \					
    				"???"));
    	 mTBOX_PRINT((kTBOX_LF, "Convolution = %s\n", \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_NONE)? "eTDAL_DMD_FEC_NONE": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_1_2)? "eTDAL_DMD_FEC_1_2": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_2_3)? "eTDAL_DMD_FEC_2_3": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_3_4)? "eTDAL_DMD_FEC_3_4": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_4_5)? "eTDAL_DMD_FEC_4_5": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_5_6)? "eTDAL_DMD_FEC_5_6": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_6_7)? "eTDAL_DMD_FEC_6_7": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_7_8)? "eTDAL_DMD_FEC_7_8": \
    				 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_8_9)? "eTDAL_DMD_FEC_8_9": \
					 (DMD_Info.TunData.Ter.ConvolutionHP == eTDAL_DMD_FEC_AUTO)? "eTDAL_DMD_FEC_AUTO": \
    				 "???"));
    }
#elif defined(MEDIUM_CAB)
	mTBOX_PRINT((kTBOX_LF, "        Medium = Cable\n"));
	mTBOX_PRINT((kTBOX_LF, "        TunFrequency = %d KHz\n", DMD_Info.TunFrequency));

	 mTBOX_PRINT((kTBOX_LF, "SymbolRateBw = %d bauds\n", DMD_Info.TunData.Cab.SymbolRateBw));
	 mTBOX_PRINT((kTBOX_LF, "Modulation = %s\n", \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_BPSK)? "eTDAL_DMD_MOD_BPSK": \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_QPSK)? "eTDAL_DMD_MOD_QPSK" : \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_QAM4)? "eTDAL_DMD_MOD_QAM4" : \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_8PSK)? "eTDAL_DMD_MOD_8PSK" : \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_QAM16)? "eTDAL_DMD_MOD_QAM16" : \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_QAM32)? "eTDAL_DMD_MOD_QAM32" : \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_QAM64)? "eTDAL_DMD_MOD_QAM64" : \
				(DMD_Info.TunData.Cab.Modulation == eTDAL_DMD_MOD_QAM128)? "eTDAL_DMD_MOD_QAM128" : \
				"eTDAL_DMD_MOD_QAM256"));
	 mTBOX_PRINT((kTBOX_LF, "SpecInv = %s\n", \
				(DMD_Info.TunData.Cab.SpecInv == eTDAL_DMD_SPECTRUM_NORMAL)? "eTDAL_DMD_SPECTRUM_NORMAL": \
				(DMD_Info.TunData.Cab.SpecInv == eTDAL_DMD_SPECTRUM_INVERTED)? "eTDAL_DMD_SPECTRUM_INVERTED": \
				"eTDAL_DMD_SPECTRUM_AUTO"));
#endif
	mTBOX_PRINT((kTBOX_LF, "CarrierStatus = %s\n", \
				(DMD_Info.CarrierStatus == eTDAL_DMD_LOCKED)? "eTDAL_DMD_LOCKED" : \
				(DMD_Info.CarrierStatus == eTDAL_DMD_LOCK_FAILED)? "eTDAL_DMD_LOCK_FAILED" : \
				(DMD_Info.CarrierStatus ==  eTDAL_DMD_FOUND)? " eTDAL_DMD_FOUND" : \
				(DMD_Info.CarrierStatus == eTDAL_DMD_END_OF_RANGE)? "eTDAL_DMD_END_OF_RANGE" : \
				 (DMD_Info.CarrierStatus == eTDAL_DMD_SIGNAL_LOST)? "eTDAL_DMD_SIGNAL_LOST" : \
				"???"));
	mTBOX_PRINT((kTBOX_LF, "SignalQuality = %d\n", DMD_Info.SignalQuality));
	mTBOX_PRINT((kTBOX_LF, "SignalBER = %d\n", DMD_Info.SignalBER));
	mTBOX_PRINT((kTBOX_LF, "SignalLevel = %d\n", DMD_Info.SignalLevel));
}

/*! \fn void TestHelper_DMD_Term( unsigned char *ucFeID )
 *  \brief Helper function to properly terminate a DMD test
 *
 *  This function checks wether a semaphore was used or not
 *  and eventually desallocate it
 *
 *  \param[in] *ucFeID pointer to the Front End ID
 */
void TestHelper_DMD_Term(unsigned char *ucFeID)
{
	int idx;

	/* termintate DMD */
	/* loop to get good FeID */
	for(idx = 0; idx < (TestHelper_DMD_CurrentIdx+1); idx++)
	{
		TestManager_AssertEqual(TDAL_DMD_CloseFEInstance(TestHelper_DMD_FeID[idx]), eTDAL_DMD_NO_ERROR, "DMD close Fe instance");
	}

	TestManager_AssertEqual(TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "DMD terminate");

	if(TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx] != NULL)
	{
		TKEL_DeleteSemaphore(TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx]);
		TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx] = NULL;
	}
}

/*! \fn void TestHelper_DMD_WaitStatus( unsigned char ucFeID )
 *  \brief Helper function to properly wait a DMD notification
 *
 *  This function waits a semaphore
 *
 *  \param[in] ucFeID It is the Front End ID
 */
void TestHelper_DMD_WaitStatus(unsigned char ucFeID)
{
	TKEL_WaitSemaphore(TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx]);
}

void TestHelper_DMD_GetStatus(unsigned char ucFeID, tTDAL_DMD_NotifyStatus *peStatus)
{
	if(peStatus != (tTDAL_DMD_NotifyStatus *)NULL)
	{
		*peStatus = TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx];
	}
}

void TestHelper_DMD_SetCurrentDMD(int idx)
{
	if((idx >=0) && (idx < NUMBER_OF_DMD))
	{
		TestHelper_DMD_CurrentIdx = idx;
	}
}

/*! \fn void pTestHelper_DMD_StatusFct( uint8_t ucFeID, tTDAL_DMD_NotifyStatus Status)
 *  \brief Callback status for the demod
 *
 *  This function returns useful information about a channel/signal status. For
 *  example it is locked or the signal has been lost.
 *  Before exiting the function a semaphore is raised in order to loose as less time
 *  as possible
 *
 \code
 ... // Init and configure demod
 TestHelper_DMDTune(); // Tune RTL
 TKEL_WaitSemaphore(TDAL_DMD_TST_NotifySem); // Semaphore has been raised so Tune() is finished
 TestManager_AssertEqual(TDAL_DMD_TST_NotifyCond, eTDAL_DMD_LOCKED, "signal locked"); // check that signal is locked
 \endcode
 *  \param[in] ucFeID Frontend identificator
 *  \param[in] Status Status of the demod
 */
LOCAL void pTestHelper_DMD_StatusFct( uint8_t ucFeID, tTDAL_DMD_NotifyStatus Status)
{
	if (Status == eTDAL_DMD_LOCKED)
	{
		TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx] = eTDAL_DMD_LOCKED;
		mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_LOCKED)\r\n",
					 ucFeID));
	}
	else if (Status == eTDAL_DMD_LOCK_FAILED)
	{
		TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx] = eTDAL_DMD_LOCK_FAILED;
		mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_LOCK_FAILED)\r\n",
					 ucFeID));
	}
	else if (Status == eTDAL_DMD_FOUND)
	{
		TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx] = eTDAL_DMD_FOUND;
		mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_FOUND)\r\n",
					 ucFeID));
	}
	else if (Status == eTDAL_DMD_END_OF_RANGE)
	{
		TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx] = eTDAL_DMD_END_OF_RANGE;
		mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_END_OF_RANGE)\r\n",
					 ucFeID));
	}
	else if (Status == eTDAL_DMD_SIGNAL_LOST)
	{
		TestHelper_DMD_NotifyStatus[TestHelper_DMD_CurrentIdx] = eTDAL_DMD_SIGNAL_LOST;
		mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_SIGNAL_LOST)\r\n",
					 ucFeID));
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR : NotifyStatusFct(FE_%d-Unknown Value !)\r\n",
					 ucFeID));
	}

	/* signal only if current demod used */
	if(TestHelper_DMD_FeID[TestHelper_DMD_CurrentIdx] == ucFeID)
	{
		TKEL_SignalSemaphore(TestHelper_DMD_NotifySem[TestHelper_DMD_CurrentIdx]);
	}
}

/*! \fn void pTestHelper_DMD_ProgressFct( uint8_t ucFeID, tTDAL_DMD_NotifyProgress Progress )
 *  \brief Callback progression for the demod
 *
 *  This function is rarely used and does a printf() of the 2nd parameter.
 *
 *  \param[in] ucFeID Frontend identificator
 *  \param[in] Progress Indicates the progression of a scan/tune
 */
LOCAL void pTestHelper_DMD_ProgressFct( uint8_t ucFeID, tTDAL_DMD_NotifyProgress Progress)
{
	mTBOX_TRACE((kTBOX_NIV_1, "NotifyProgressFct() FE[0x%x] Progress[0x%x]\r\n",
				 ucFeID,
				 Progress));
}

