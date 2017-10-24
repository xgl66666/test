/** \file testhelper.c
 *  \brief Global helper functions for CHAL tests
 *  \author Ronan MENEU
 *  \author Olivier DOLE
 *  \date 16/03/06
 *  \version 1.0
 *  \todo write documentation for DMX,AVS and TSIN helper functions
 *  Add new helper functions.
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include <string.h>     /* for memset() */

#include "crules.h"

#include "testhelper.h"
#include "TestManager.h"
#include "tester_prefs.h"
#include "fstests.h"

#include "tdal_av.h"
#include "tdal_kbd.h"
#ifdef USE_TDAL_FS
#include "tdal_fs.h"
#endif
#include "tdal_dmx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_hdmi.h"
#include "tdal_ts.h"
#include "tdal_avs.h"
#include "tdal_dmd.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif
#include "tdal_fla.h"

#include "tkel.h"
#include "tbox.h"

/*************************************************************************
 *  Defines                                                              *
 *************************************************************************/

/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                                        *
 ****************************************************************************/
/* To configure which Demod/Demux indexes to use in the av tests */
extern int gTDAL_AV_TestFeIdx;
extern int gTDAL_AV_TestDmxIdx;

extern void HDMICallback(tTDAL_HDMI_SourceId Id, tTDAL_HDMI_Event Event);

/*************************************************************************
 *  LOCAL variables                                                      *
 *************************************************************************/
mTBOX_SET_MODULE( eTDAL_TEST_COMMON );

/*! \var unsigned char DMD_FeID
 *  \brief Frontend identificator */
/*! \var bool DMD_lockedOnFreq
 *  \brief Boolean to know if DMD has locked a frequency */
/*! \var tTDAL_DMX_ChannelId DMX_VideoChannelId
 *  \brief Demux video channel identificator */
/*! \var tTDAL_DMX_ChannelId DMX_AudioChannelId
 *  \brief Demux audio channel identificator */
/*! \var tTDAL_DMX_ChannelId DMX_PcrChannelId
 *  \brief Demux PCR channel identificator */
/*! \var tTDAL_DMX_FilterId DMX_VideoFilterId
 *  \brief Identificator for the video filter created for a channel */
/*! \var tTDAL_DMX_FilterId DMX_AudioFilterId
 *  \brief Identificator for the audio filter created for a channel */
/*! \var tTDAL_DMX_FilterId DMX_PcrFilterId
 *  \brief Identificator for the PCR filter created for a channel */
/*! \var bool DMX_is_configured
 *  \brief Boolean to know if DMX is properly configured */
/*! \var tTDAL_TSIN_Handle TSIN_Handle
 *  \brief Handle for the TSIN */
unsigned char                   DMD_FeID[NUMBER_OF_DMD];
bool                            DMD_lockedOnFreq = false;
tTDAL_DMX_ChannelId             DMX_VideoChannelId;
tTDAL_DMX_ChannelId             DMX_AudioChannelId;
tTDAL_DMX_ChannelId             DMX_PcrChannelId;
unsigned int                    DMX_VideoPid=0x1FFF;
unsigned int                    DMX_AudioPid=0x1FFF;
unsigned int                    DMX_PcrPid=0x1FFF;
uint32_t                        DMX_VideoStreamId;
uint32_t                        DMX_AudioStreamId;
LOCAL   tTDAL_DMX_FilterId      DMX_VideoFilterId;
LOCAL   tTDAL_DMX_FilterId      DMX_AudioFilterId;
LOCAL   tTDAL_DMX_FilterId      DMX_PcrFilterId;
LOCAL   bool                    DMX_is_configured = false;

TKEL_sema_id            TDAL_DMD_TST_NotifySem[NUMBER_OF_DMD] =
{
    NULL,
#if (NUMBER_OF_DMD==2)
    NULL,
#endif
};
tTDAL_DMD_NotifyStatus   TDAL_DMD_TST_NotifyCond[NUMBER_OF_DMD] =
{
    eTDAL_DMD_LOCKED,
#if (NUMBER_OF_DMD==2)
    eTDAL_DMD_LOCKED,
#endif
};

TKEL_sema_id            TDAL_HDMI_TST_NotifySem = NULL;


/************************************************************************
 ** SERVICES INFO *******************************************************
 ************************************************************************/
#ifndef CIPLUSVALIDATOR
struct TestManager_Channel gChannelList[] = {
#if defined (MEDIUM_SAT)
{
    "ASTRA",
    "EinsExtra",
    10747000,
    0,
    eTDAL_DMD_MOD_QPSK,
    eTDAL_DMD_SPECTRUM_AUTO,
    {
        22000000,
        eTDAL_DMD_MOD_QPSK,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_POLAR_HORIZONTAL
    },
    100,
    101,
    102,
    101,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
}, 

{
    "ASTRA",
    "TP3 ASTRA",
    12460000,
    27500000,
    eTDAL_DMD_MOD_QPSK,
    eTDAL_DMD_SPECTRUM_AUTO,
    {
        27500000,
        eTDAL_DMD_MOD_QPSK,
        eTDAL_DMD_FEC_3_4,
        eTDAL_DMD_POLAR_HORIZONTAL
    },
    44,
    3311,
    3312,
    3311,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{ NULL, NULL, 0, 0, 0, 0, {0, 0}, 0, 0, 0, 0,
	0,0,0,0,
 }

#elif defined (MEDIUM_CAB)
{
    "Player 1",      /* char *mSatellite : Name of the satelite */
    "dvb-t_818Mhz",  /* char *mChannelName :  Name of the channel */
    818000000,           /* uint32_t  TunFrequency */
    6875000,            /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_QAM64,        /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    { 6875000, eTDAL_DMD_MOD_QAM64, eTDAL_DMD_SPECTRUM_AUTO },    /* tTDAL_DMD_SpecificTunData    Specific */
    100,      /* uint16_t mPMTPID : PID to demux PMT */
    101,      /* uint16_t mVideoPID : PID to demux video */
    103,      /* uint16_t mAudioPID : PID to demux audio */
    101,      /* uint16_t mPCRPID PID to demux PCR */
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{
    "Player 2",
	"TS_1",
    810000000,          /* uint32_t  TunFrequency */
    6875000,            /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_QAM64,        /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    { 6875000, eTDAL_DMD_MOD_QAM64, eTDAL_DMD_SPECTRUM_AUTO },               /* tTDAL_DMD_SpecificTunData    Specific */
    100,       /* uint16_t mPMTPID : PID to demux PMT */
    201,      /* uint16_t mVideoPID : PID to demux video */
    203,      /* uint16_t mAudioPID : PID to demux audio */
    201,      /* uint16_t mPCRPID PID to demux PCR */
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{ NULL, NULL, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0 },
{ NULL, NULL, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0 },
{
    "Player 5",      /* char *mSatellite : Name of the satelite */
    "DSMCC",  /* char *mChannelName :  Name of the channel */
    794000000,           /* uint32_t  TunFrequency */
    6875000,            /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_QAM64,        /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    { 6875000, eTDAL_DMD_MOD_QAM64, eTDAL_DMD_SPECTRUM_AUTO },    /* tTDAL_DMD_SpecificTunData    Specific */
    100,      /* uint16_t mPMTPID : PID to demux PMT */
    101,      /* uint16_t mVideoPID : PID to demux video */
    103,      /* uint16_t mAudioPID : PID to demux audio */
    101,      /* uint16_t mPCRPID PID to demux PCR */
    eTDAL_AV_VIDEO_TYPE_MPEG2,
    eTDAL_AV_AUDIO_TYPE_MPEG,
    720,
    576
},
{
    "Player 3",
    "HD_1",
    802000000,          /* uint32_t  TunFrequency */
    6900000,            /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_QAM256,        /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    { 6900000, eTDAL_DMD_MOD_QAM256, eTDAL_DMD_SPECTRUM_AUTO },               /* tTDAL_DMD_SpecificTunData    Specific */
    0x1518,       /* uint16_t mPMTPID : PID to demux PMT */
    0x1519,      /* uint16_t mVideoPID : PID to demux video */
    0x151a,      /* uint16_t mAudioPID : PID to demux audio */
    0x1519,      /* uint16_t mPCRPID PID to demux PCR */
    eTDAL_AV_VIDEO_TYPE_H264,
    eTDAL_AV_AUDIO_TYPE_MPEG,
    1280,
    720
},
{ NULL, NULL, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0 },
{
    "Player 4",
    "HD_2",
    802000000,          /* uint32_t  TunFrequency */
    6900000,            /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_QAM256,        /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    { 6900000, eTDAL_DMD_MOD_QAM256, eTDAL_DMD_SPECTRUM_AUTO },               /* tTDAL_DMD_SpecificTunData    Specific */
    0x1522,       /* uint16_t mPMTPID : PID to demux PMT */
    0x1523,      /* uint16_t mVideoPID : PID to demux video */
    0x1524,      /* uint16_t mAudioPID : PID to demux audio */
    0x1523,      /* uint16_t mPCRPID PID to demux PCR */
    eTDAL_AV_VIDEO_TYPE_H264,
    eTDAL_AV_AUDIO_TYPE_MPEG,
    1280,
    720
},
{ NULL, NULL, 0, 0, 0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0 }
#elif defined (MEDIUM_TER)
{
#error Terestrial instead of cable is set!
    "ASTRA",
    "TP3 ASTRA",
    12460000,
    27500000,
    eTDAL_DMD_MOD_QPSK,
    eTDAL_DMD_SPECTRUM_AUTO,
    {
        eTDAL_DMD_FEC_3_4,
        eTDAL_DMD_POLAR_HORIZONTAL
    },
    400,
    3311,
    3312,
    3311,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	3,//eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{
	"ASTRA !~",
	"DW @@@@ EUROPE ~~",
	33333333,
	27500000,
	eTDAL_DMD_MOD_QPSK,
	eTDAL_DMD_SPECTRUM_AUTO,
	{
		eTDAL_DMD_FEC_3_4,
		eTDAL_DMD_POLAR_HORIZONTAL,
	},
	400,
	1000,
	1001,
	1000,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	3, //eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},

{ NULL, NULL, 0, 0, 0, 0, {0, 0}, 0, 0, 0, 0,
	0,0,0,0,
 }


#endif

};
#else  /*CIPLUSVALIDATOR*/
extern struct TestManager_Channel gChannelList[];
#endif /*CIPLUSVALIDATOR*/

int gChannel = 0;

/*************************************************************************
 *  Local functions                                                   *
 *************************************************************************/
void TestHelper_Initialize_TDAL_DMD_Config( tTDAL_DMD_ConfigData *DMD_Config );

#if defined(USE_TDAL_FS)
LOCAL char *pTestHelper_FsVolumeTypeToString(tTDAL_FS_VolumeType eType);
LOCAL char *pTestHelper_FsDeviceTypeToString(tTDAL_FS_DeviceType eType);
#endif /* USE_TDAL_FS */

/*************************************************************************
 *  Exported functions                                                   *
 *************************************************************************/

/** DMD **/
/*! \fn void TestHelper_Initialize_TDAL_DMD_Config( tTDAL_DMD_ConfigData *DMD_Config )
 *  \brief This function initializes a tTDAL_DMD_ConfigData structure
 *  \param[out] *DMD_Config structure to initialize
 */
/*! \fn void TestHelper_Initialize_TDAL_DMD_Tune( tTDAL_DMD_TunData *Tune_Config,
 *                                              uint32_t frequency,
 *                                              uint32_t symbol_rate,
 *                                              tTDAL_DMD_Convolution convolution,
 *                                              tTDAL_DMD_LNB_Polarization polarization )
 *  \brief This function initializes tTDAL_DMD_TunData structure with other input parameters
 *  \param[out] *Tune_Config structure to initialize
 *  \param[in] frequency frequency of the channel to lock
 *  \param[in] symbol_rate symbol rate of the signal
 *  \param[in] convolution convolution of the signal
 *  \param[in] polarization polarization of the signal
 */
/*! \fn void TestHelper_Initialize_TDAL_DMD( void )
 *  \brief Initialize and configure the demod
 *  \see TestHelper_Initialize_TDAL_DMD_Config() to fill configuration structure
 */
/*! \fn void TestHelper_DMDTuneSpecific( uint32_t freq, uint32_t symb,
 *                              tTDAL_DMD_Convolution conv,
 *                              tTDAL_DMD_LNB_Polarization  pola )
 *  \brief Tune a channel
 *  \param[in] freq frequency of the signal to lock
 *  \param[in] symb symbol rate of the signal
 *  \param[in] conv convolution of the signal
 *  \param[in] pola polarization of the signal
 *  \see TestHelper_Initialize_TDAL_DMD_Tune() to fill configuration structure for the signal to lock
 */

void TestHelper_Initialize_TDAL_DMD(void)
{
    int                     i = 0;
    tTDAL_DMD_Error         dmd_err;
    tTDAL_DMD_ConfigData    psCfgData;

    for(i = 0; i < NUMBER_OF_DMD; i++)
    {
        /* create semaphore for callback */
        if(TDAL_DMD_TST_NotifySem[i] == NULL)
        {
            TKEL_CreateSemaphore(0, &TDAL_DMD_TST_NotifySem[i]);
        }

         /* init DMD module */
        dmd_err = TDAL_DMD_Init();
        TestManager_AssertEqual( dmd_err, eTDAL_DMD_NO_ERROR, "dmd init" );
        dmd_err = TDAL_DMD_OpenFEInstance( eTDAL_DMD_FE_0,
                                 pTestHelper_DMDStatusFct,
                                 pTestHelper_DMDProgressFct );
		
        TestManager_AssertEqual( dmd_err, eTDAL_DMD_NO_ERROR, "TDAL_DMD_OpenFEInstance" );

		i = 0;

        /* configure DMD */
#if defined (MEDIUM_SAT)
        psCfgData.Sat.ConfigType = eTDAL_DMD_CFG_SAT_INIT;
        TestHelper_Initialize_TDAL_DMD_Config( &psCfgData );
        TestManager_AssertEqual( TDAL_DMD_Config(DMD_FeID[i], &psCfgData), eTDAL_DMD_NO_ERROR, "dmd config" );
        psCfgData.Sat.ConfigType = eTDAL_DMD_CFG_SAT_OL_BAND;
        TestHelper_Initialize_TDAL_DMD_Config( &psCfgData );
        TestManager_AssertEqual( TDAL_DMD_Config(DMD_FeID[i], &psCfgData), eTDAL_DMD_NO_ERROR, "dmd config" );
#else
        TestHelper_Initialize_TDAL_DMD_Config( &psCfgData );

        TestManager_AssertEqual( TDAL_DMD_Config(DMD_FeID[i], &psCfgData), eTDAL_DMD_NO_ERROR, "dmd config" );
#endif

        pTestHelper_DMDGetInfo(i);

    }
}

void TestHelper_Initialize_TDAL_DMD_Config( tTDAL_DMD_ConfigData *DMD_Config )
{
#if defined (MEDIUM_SAT)
    if (DMD_Config->Sat.ConfigType == eTDAL_DMD_CFG_SAT_INIT)
    {
        DMD_Config->Sat.uCfg.Init.SwitchFreq = SWITCH_FREQ;
        DMD_Config->Sat.uCfg.Init.Vertical = eTDAL_DMD_LNB_POWER_13V;
        DMD_Config->Sat.uCfg.Init.Horizontal = eTDAL_DMD_LNB_POWER_18V;
		uint32_t *pSymb_list = NULL;
		pSymb_list = (uint32_t *)TDAL_Malloc(sizeof(uint32_t )*2);
		pSymb_list[0] = 27500000;
		pSymb_list[1] = 1;
        DMD_Config->Sat.uCfg.Init.pSymbolRateList = pSymb_list;		
    }
    else if (DMD_Config->Sat.ConfigType == eTDAL_DMD_CFG_SAT_OL_BAND)
    {
        DMD_Config->Sat.uCfg.OlBand.BandType = eTDAL_DMD_BANDTYPE_KU; /* Europe */
        DMD_Config->Sat.uCfg.OlBand.LowLNBFreq = LOW_LNB_FREQ; /* Low OL */
        DMD_Config->Sat.uCfg.OlBand.HighLNBFreq = HIGH_LNB_FREQ; /* High OL */
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Sat.ConfigType %d not handled\n"));
    }
#elif defined (MEDIUM_CAB)
    DMD_Config->Cab.ConfigType = eTDAL_DMD_CFG_CAB_INIT;
    DMD_Config->Cab.uCfg.Init.pSymbolRateList = NULL;
#elif defined (MEDIUM_TER)
    static uint32_t aBandWidthList[4] = {BANDWIDTH_1, BANDWIDTH_2, BANDWIDTH_3, 0}; /* Hz */

    DMD_Config->Ter.ConfigType = eTDAL_DMD_CFG_TER_INIT;
    DMD_Config->Ter.uCfg.Init.pBandWidthList = aBandWidthList;
#endif
}

void TestHelper_Initialize_TDAL_DMD_Tune(tTDAL_DMD_TunData *Tune_Config, uint32_t *Frequency)
{
    struct TestManager_Channel* aChannel;
    aChannel = &gChannelList[gChannel];
#if defined (MEDIUM_SAT)
    mTBOX_PRINT((kTBOX_LF,"~~~~ tuning %s on %s\n", aChannel->mChannelName, aChannel->mSatellite ));
#else 
    mTBOX_PRINT((kTBOX_LF,"~~~~ tuning %s on freq %d\n", aChannel->mChannelName, aChannel->TunFrequency));
#endif

    *Frequency = aChannel->TunFrequency;
#if defined (MEDIUM_SAT)
    Tune_Config->Sat.SymbolRateBw = aChannel->Sat.SymbolRateBw;
    Tune_Config->Sat.Modulation = aChannel->Sat.Modulation;
    Tune_Config->Sat.SpecInv = aChannel->Sat.SpecInv;
    mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - Mod %x - Spectrum %x",
                *Frequency,
                Tune_Config->Sat.SymbolRateBw,
                Tune_Config->Sat.Modulation,
                Tune_Config->Sat.SpecInv));
    Tune_Config->Sat.Convolution = aChannel->Sat.Convolution;
    Tune_Config->Sat.Polarization = aChannel->Sat.Polarization;
    mTBOX_TRACE((kTBOX_NIV_1, " - FEC %x - Pola %x]\n",
                Tune_Config->Sat.Convolution,
                Tune_Config->Sat.Polarization));
#elif defined (MEDIUM_CAB)
    Tune_Config->Cab.SymbolRateBw = aChannel->Cab.SymbolRateBw;
    Tune_Config->Cab.Modulation = aChannel->Cab.Modulation;
    Tune_Config->Cab.SpecInv = aChannel->Cab.SpecInv;
    mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - Mod %x - Spectrum %x",
                *Frequency,
                Tune_Config->Cab.SymbolRateBw,
                Tune_Config->Cab.Modulation,
                Tune_Config->Cab.SpecInv));
    mTBOX_TRACE((kTBOX_NIV_1, "]\n"));
#elif defined (MEDIUM_TER)
    Tune_Config->Ter.SymbolRateBw = aChannel->Ter.SymbolRateBw;
    Tune_Config->Ter.Modulation = aChannel->Ter.Modulation;
    Tune_Config->Ter.SpecInv = aChannel->Ter.SpecInv;
    mTBOX_TRACE((kTBOX_NIV_1, "Try to lock on [Freq %d - SB %d - Mod %x - Spectrum %x",
                *Frequency,
                Tune_Config->Ter.SymbolRateBw,
                Tune_Config->Ter.Modulation,
                Tune_Config->Ter.SpecInv));
    Tune_Config->Ter.ConvolutionHP = aChannel->Ter.ConvolutionHP;
    Tune_Config->Ter.ConvolutionLP = aChannel->Ter.ConvolutionLP;
    Tune_Config->Ter.Mode = aChannel->Ter.Mode;
    Tune_Config->Ter.Guard = aChannel->Ter.Guard;
    Tune_Config->Ter.Hierarchy = aChannel->Ter.Hierarchy;
    Tune_Config->Ter.Priority = aChannel->Ter.Priority;
    Tune_Config->Ter.Offset = aChannel->Ter.Offset;
    mTBOX_TRACE((kTBOX_NIV_1, " - FEC/HP %x - FEC/LP %x - Mode %x - Guard %x - Hierarchy %x - Prio %x - Offset %x]\n",
                    Tune_Config->Ter.ConvolutionHP,
                    Tune_Config->Ter.ConvolutionLP,
                    Tune_Config->Ter.Mode,
                    Tune_Config->Ter.Guard,
                    Tune_Config->Ter.Hierarchy,
                    Tune_Config->Ter.Priority,
                    Tune_Config->Ter.Offset ));
#endif
    }

/*! \fn void TestHelper_DMDTune(void)
 *  \brief Tune a specific channel (RTL on Astra)
 *  \see TestHelper_DMDTuneSpecific()
 */
void TestHelper_DMDTune(int iFeIdx)
{
    struct TestManager_Channel* aChannel;
    tTDAL_DMD_TunData sTunData;

	aChannel = &gChannelList[gChannel];
	mTBOX_TRACE((kTBOX_NIV_1,"tuning %s on %s\n",
				 aChannel->mChannelName,
				 aChannel->mSatellite));
	mTBOX_TRACE((kTBOX_NIV_1,"frequency %d BW=%d\n",
				 aChannel->TunFrequency,
#if defined (MEDIUM_SAT)
				 aChannel->Sat.SymbolRateBw
#elif defined (MEDIUM_CAB)
				 aChannel->Cab.SymbolRateBw
#elif defined (MEDIUM_TER)				 
				 aChannel->Ter.SymbolRateBw
#endif
	            ));

#if defined (MEDIUM_SAT)
    sTunData.Sat = aChannel->Sat;
#elif defined (MEDIUM_CAB)
    sTunData.Cab = aChannel->Cab;
#elif defined (MEDIUM_TER)
    sTunData.Ter = aChannel->Ter;
#endif		
		
	TestHelper_DMDTuneSpecific(iFeIdx, sTunData, aChannel->TunFrequency);
		
	/* update PID values*/
	DMX_VideoPid    = aChannel->mVideoPID;
	DMX_AudioPid    = aChannel->mAudioPID;
	DMX_PcrPid      = aChannel->mPCRPID;
	mTBOX_PRINT((kTBOX_LF,"video_pid(%d) audio_pid(%d) pcr_pid(%d)\n",DMX_VideoPid, DMX_AudioPid, DMX_PcrPid));
	/* */
}

void TestHelper_DMDTuneSpecific(int iFeIdx, tTDAL_DMD_TunData psTunData, uint32_t Frequency)
{
    TestManager_AssertEqual( TDAL_DMD_Tune(DMD_FeID[iFeIdx], &psTunData, Frequency), eTDAL_DMD_NO_ERROR, "dmd tune" );

	mTBOX_PRINT((kTBOX_LF,"Tune wait(%d)...\n",iFeIdx));

    TKEL_WaitSemaphore(TDAL_DMD_TST_NotifySem[iFeIdx]);

    pTestHelper_DMDGetInfo(iFeIdx);
}

/*! \fn void TestHelper_Terminate_TDAL_DMD( void )
 *  \brief Helper function to properly close the demod
 */
void TestHelper_Terminate_TDAL_DMD()
{
    int i;

    for(i = 0; i < NUMBER_OF_DMD; i ++)
    {
        if(TDAL_DMD_TST_NotifySem[i] != NULL)
        {
            TKEL_DeleteSemaphore(TDAL_DMD_TST_NotifySem[i]);
            TDAL_DMD_TST_NotifySem[i] = NULL;
        }
        TestManager_AssertEqual( TDAL_DMD_CloseFEInstance(DMD_FeID[i]), eTDAL_DMD_NO_ERROR, "dmd close instance" );
    }
    TestManager_AssertEqual( TDAL_DMD_Terminate(), eTDAL_DMD_NO_ERROR, "dmd terminate instance" );
}

/*! \fn void pTestHelper_DMDStatusFct( uint8_t ucFeID, tTDAL_DMD_NotifyStatus Status)
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
void pTestHelper_DMDStatusFct( tTDAL_DMD_FE eFeID, tTDAL_DMD_NotifyStatus Status)
{
    int i;

    /* search for FE index */
    for(i = 0; i < NUMBER_OF_DMD; i++)
    {
        if(eFeID == DMD_FeID[i])
        {
            if (Status == eTDAL_DMD_LOCKED)
            {
                TDAL_DMD_TST_NotifyCond[i] = eTDAL_DMD_LOCKED;
                mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_LOCKED)\r\n",
                             i));
            }
            else if (Status == eTDAL_DMD_LOCK_FAILED)
            {
                TDAL_DMD_TST_NotifyCond[i] = eTDAL_DMD_LOCK_FAILED;
                mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_LOCK_FAILED)\r\n"));
            }
            else if (Status == eTDAL_DMD_FOUND)
            {
                TDAL_DMD_TST_NotifyCond[i] = eTDAL_DMD_FOUND;
                mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_FOUND)\r\n"));
            }
            else if (Status == eTDAL_DMD_END_OF_RANGE)
            {
                TDAL_DMD_TST_NotifyCond[i] = eTDAL_DMD_END_OF_RANGE;
                mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_END_OF_RANGE)\r\n"));
            }
            else if (Status == eTDAL_DMD_SIGNAL_LOST)
            {
                TDAL_DMD_TST_NotifyCond[i] = eTDAL_DMD_SIGNAL_LOST;
                mTBOX_TRACE((kTBOX_NIV_1, "NotifyStatusFct(FE_%d-eTDAL_DMD_SIGNAL_LOST)\r\n"));
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR : NotifyStatusFct(FE_%d-Unknown Value !)\r\n"));
            }
            TKEL_SignalSemaphore(TDAL_DMD_TST_NotifySem[i]);
        }
    }
}

/*! \fn void pTestHelper_DMDProgressFct( uint8_t ucFeID, tTDAL_DMD_NotifyProgress Progress )
 *  \brief Callback progression for the demod
 *
 *  This function is rarely used and does a printf() of the 2nd parameter.
 *
 *  \param[in] ucFeID Frontend identificator
 *  \param[in] Progress Indicates the progression of a scan/tune
 */
void pTestHelper_DMDProgressFct( tTDAL_DMD_FE eFeID, tTDAL_DMD_NotifyProgress Progress)
{
        mTBOX_TRACE((kTBOX_NIV_1, "NotifyProgressFct() FE[0x%x] Progress[0x%x]\r\n", eFeID, Progress));
}

/*! \fn void pTestHelper_DMDGetInfo( void )
 *  \brief Retrieve and display information about the signal
 */
void pTestHelper_DMDGetInfo(int iFeIdx)
{
    tTDAL_DMD_Error error;
    tTDAL_DMD_Info sTDAL_DMD_TST_Info;

    mTBOX_FCT_ENTER("pTestHelper_DMDGetInfo");

    mTBOX_PRINT((kTBOX_LF,"------------- DMD Get Info Begin --------------\n"));

    if ((error = TDAL_DMD_GetInfo(DMD_FeID[iFeIdx], &sTDAL_DMD_TST_Info)) != eTDAL_DMD_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_DMD_GetInfo error (%d)\n", error));
        return;
    }

    mTBOX_PRINT((kTBOX_LF, "CarrierStatus = %s\n", \
                            (sTDAL_DMD_TST_Info.CarrierStatus == eTDAL_DMD_LOCKED)? "eTDAL_DMD_LOCKED" : \
                            (sTDAL_DMD_TST_Info.CarrierStatus == eTDAL_DMD_LOCK_FAILED)? "eTDAL_DMD_LOCK_FAILED" : \
                            (sTDAL_DMD_TST_Info.CarrierStatus == eTDAL_DMD_END_OF_RANGE)? "eTDAL_DMD_END_OF_RANGE" : \
                            "eTDAL_DMD_SIGNAL_LOST"));
    if(sTDAL_DMD_TST_Info.CarrierStatus == eTDAL_DMD_LOCKED)
    {
        mTBOX_PRINT((kTBOX_LF, "TunFrequency = %d KHz\n", sTDAL_DMD_TST_Info.TunFrequency));
        mTBOX_PRINT((kTBOX_LF, "SymbolRateBw = %d bauds\n", sTDAL_DMD_TST_Info.TunData.Ter.SymbolRateBw));
        mTBOX_PRINT((kTBOX_LF, "Modulation = %s\n", \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_BPSK)? "eTDAL_DMD_MOD_BPSK": \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QPSK)? "eTDAL_DMD_MOD_QPSK" : \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM4)? "eTDAL_DMD_MOD_QAM4" : \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_8PSK)? "eTDAL_DMD_MOD_8PSK" : \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM16)? "eTDAL_DMD_MOD_QAM16" : \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM32)? "eTDAL_DMD_MOD_QAM32" : \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM64)? "eTDAL_DMD_MOD_QAM64" : \
                                (sTDAL_DMD_TST_Info.TunData.Ter.Modulation == eTDAL_DMD_MOD_QAM128)? "eTDAL_DMD_MOD_QAM128" : \
                                "eTDAL_DMD_MOD_QAM256"));
        mTBOX_PRINT((kTBOX_LF, "SpecInv = %s\n", \
                                (sTDAL_DMD_TST_Info.TunData.Ter.SpecInv == eTDAL_DMD_SPECTRUM_NORMAL)? "eTDAL_DMD_SPECTRUM_NORMAL": \
                                (sTDAL_DMD_TST_Info.TunData.Ter.SpecInv == eTDAL_DMD_SPECTRUM_INVERTED)? "eTDAL_DMD_SPECTRUM_INVERTED": \
                                "eTDAL_DMD_SPECTRUM_AUTO"));
#if defined(MEDIUM_SAT)
        if (sTDAL_DMD_TST_Info.TunData.Sat.Modulation == eTDAL_DMD_MOD_QPSK)
        {
            mTBOX_PRINT((kTBOX_LF, "Convolution = %s\n", \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_NONE)? "eTDAL_DMD_FEC_NONE": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_1_2)? "eTDAL_DMD_FEC_1_2": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_2_3)? "eTDAL_DMD_FEC_2_3": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_3_4)? "eTDAL_DMD_FEC_3_4": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_4_5)? "eTDAL_DMD_FEC_4_5": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_5_6)? "eTDAL_DMD_FEC_5_6": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_6_7)? "eTDAL_DMD_FEC_6_7": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_7_8)? "eTDAL_DMD_FEC_7_8": \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Convolution == eTDAL_DMD_FEC_8_9)? "eTDAL_DMD_FEC_8_9": \
                                    "eTDAL_DMD_FEC_AUTO"));
            mTBOX_PRINT((kTBOX_LF, "Polarization = %s\n", \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_HORIZONTAL)? "eTDAL_DMD_POLAR_HORIZONTAL" : \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_VERTICAL)? "eTDAL_DMD_POLAR_VERTICAL" : \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_CIRCULAR_LEFT)? "eTDAL_DMD_POLAR_CIRCULAR_LEFT" : \
                                    (sTDAL_DMD_TST_Info.TunData.Sat.Polarization == eTDAL_DMD_POLAR_CIRCULAR_RIGHT)? "eTDAL_DMD_POLAR_CIRCULAR_RIGHT" : \
                                    "eTDAL_DMD_POLAR_ALL"));
        }
#endif
        mTBOX_PRINT((kTBOX_LF, "SignalQuality = %d\n", sTDAL_DMD_TST_Info.SignalQuality));
        mTBOX_PRINT((kTBOX_LF, "SignalBER = %d\n", sTDAL_DMD_TST_Info.SignalBER));
        mTBOX_PRINT((kTBOX_LF, "SignalLevel = %d\n", sTDAL_DMD_TST_Info.SignalLevel));
    }

    mTBOX_PRINT((kTBOX_LF,"------------- DMD Get Info End --------------\n"));
}

/* ------------- DMX -------------- */

void TestHelper_Configure_TDAL_DMX2
(
    int      iDmxIdx,
    uint16_t pid_video,
    uint16_t pid_audio,
    uint16_t pid_pcr
)
{
    tTDAL_DMX_Error             dmx_err;
    tTDAL_AV_Error              av_err;
    tTDAL_DMX_ChannelType_t     VideoType = {eTDAL_DMX_VIDEO_STREAM ,eTDAL_DMX_DECODER,eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_ChannelType_t     AudioType = {eTDAL_DMX_AUDIO_STREAM ,eTDAL_DMX_DECODER,eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_ChannelType_t     PcrType = {eTDAL_DMX_PCR_STREAM ,eTDAL_DMX_DECODER,eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_DemuxId eDmxIds[eTDAL_DMX_NB_DEMUX] = {
        eTDAL_DMX_DEMUX0,
        eTDAL_DMX_DEMUX1,
        eTDAL_DMX_DEMUX2,
    };

    TestManager_AssertGreater((double)(eTDAL_DMX_NB_DEMUX-1),
                              (double)iDmxIdx,
                              "Demux index");

	mTBOX_PRINT((kTBOX_LF, "before if(iDmxIdx = '%d' > (eTDAL_DMX_NB_DEMUX-1)\n", iDmxIdx));
    if(iDmxIdx > (eTDAL_DMX_NB_DEMUX-1))
    {
		mTBOX_PRINT((kTBOX_LF, "if(iDmxIdx > (eTDAL_DMX_NB_DEMUX-1)\n"));
        return;
    }

	mTBOX_PRINT((kTBOX_LF, "before  if (DMX_is_configured)\n"));
    if (DMX_is_configured)
    {
		mTBOX_PRINT((kTBOX_LF, "TestHelper_Unconfigure_TDAL_DMX()\n"));
         TestHelper_Unconfigure_TDAL_DMX();
    }

	mTBOX_PRINT((kTBOX_LF, " if (pid_video != 0)\n"));
    /* setup video filtering */
    if (pid_video != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx] ,&VideoType, &DMX_VideoChannelId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate video channel" );

		dmx_err = TDAL_DMX_StreamHandleGet (DMX_VideoChannelId, &DMX_VideoStreamId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx stream handle get video" );

        av_err = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1, DMX_VideoStreamId );
        TestManager_AssertEqual( av_err, eTDAL_AV_NO_ERROR, "av input stream set video" );

        /* set pid on the video channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_VideoChannelId, pid_video);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on video channel" );
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = %d\n", pid_video));

        /* allocate a filter on the video channel */
        dmx_err = TDAL_DMX_Allocate_Filter(DMX_VideoChannelId, &DMX_VideoFilterId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate video filter" );

        /* start the channel */
        dmx_err = TDAL_DMX_Control_Channel(DMX_VideoChannelId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx control video channel" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(DMX_VideoFilterId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx control video filter" );
    }

    /* setup audio filtering */
  if (pid_audio != 0)
  {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx] ,&AudioType, &DMX_AudioChannelId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate audio channel" );

		dmx_err = TDAL_DMX_StreamHandleGet (DMX_AudioChannelId, &DMX_AudioStreamId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx stream handle get audio" );

		av_err = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1, DMX_AudioStreamId );
        TestManager_AssertEqual( av_err, eTDAL_AV_NO_ERROR, "av input stream set audio" );

        /* set pid on the Audio channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_AudioChannelId, pid_audio);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid audio channel" );
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = %d\n", pid_audio));

        /* allocate a filter on the Audio channel */
        dmx_err = TDAL_DMX_Allocate_Filter(DMX_AudioChannelId, &DMX_AudioFilterId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate audio filter" );

        /* start the channel */
        dmx_err = TDAL_DMX_Control_Channel(DMX_AudioChannelId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx control enable audio channel" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(DMX_AudioFilterId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx control enable audio filter" );
  }

    /* setup pcr filtering */
    if (pid_pcr != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx] ,&PcrType, &DMX_PcrChannelId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate channel" );

        /* set pid on the Pcr channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_PcrChannelId, pid_pcr);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx set channel pcr pid");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = %d\n", pid_pcr));

        /* allocate a filter on the Pcr channel */
        dmx_err = TDAL_DMX_Allocate_Filter(DMX_PcrChannelId, &DMX_PcrFilterId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate pcr filter" );

        /* start the channel */
        dmx_err = TDAL_DMX_Control_Channel(DMX_PcrChannelId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx enable dmx pcr channel" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(DMX_PcrFilterId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx enable dmx pcr filter" );
    }
    DMX_is_configured = true;
}

void TestHelper_ControlChannel_TDAL_DMX( int iEnbleChannel )
{
    tTDAL_DMX_Error dmx_err;

	mTBOX_PRINT((kTBOX_LF, "Enter Demux control (%d)\n", DMX_is_configured));

	if (!DMX_is_configured)
    {
            return;
    }

    if(iEnbleChannel == TRUE)
    {
        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_VideoChannelId, DMX_VideoPid);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on video channel");
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = %d\n", DMX_VideoPid));

        dmx_err = TDAL_DMX_Control_Channel(DMX_VideoChannelId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control video channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_AudioChannelId, DMX_AudioPid);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on audio channel");
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = %d\n", DMX_AudioPid));

        dmx_err = TDAL_DMX_Control_Channel(DMX_AudioChannelId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control audio channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_PcrChannelId, DMX_PcrPid);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on pcr channel");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = %d\n", DMX_PcrPid));

        dmx_err = TDAL_DMX_Control_Channel(DMX_PcrChannelId, eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control pcr channel");

    }
    else
    {
        dmx_err = TDAL_DMX_Control_Channel(DMX_VideoChannelId, eTDAL_DMX_CTRL_DISABLE);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control video channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_VideoChannelId, 0x1FFF);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on video channel");
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = 0x1FFF\n"));

        dmx_err = TDAL_DMX_Control_Channel(DMX_AudioChannelId, eTDAL_DMX_CTRL_DISABLE);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control audio channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_AudioChannelId, 0x1FFF);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on audio channel");
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = 0x1FFF\n"));

        dmx_err = TDAL_DMX_Control_Channel(DMX_PcrChannelId, eTDAL_DMX_CTRL_DISABLE);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx control pcr channel");

        dmx_err = TDAL_DMX_Set_Channel_PID(DMX_PcrChannelId, 0x1FFF);
        TestManager_AssertEqual(dmx_err, kTDAL_DMX_NO_ERROR, "dmx set pid on pcr channel");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = 0x1FFF\n"));
    }
}

void TestHelper_Configure_TDAL_DMX(int iDmxIdx)
{
    struct TestManager_Channel* aChannel;
    aChannel = &gChannelList[ gChannel ];

    TestHelper_Configure_TDAL_DMX2(iDmxIdx,
                                   aChannel->mVideoPID,
                                   aChannel->mAudioPID,
                                   aChannel->mPCRPID );
}

struct TestManager_Channel *    TestHelper_GetChannel( void )
{
    struct TestManager_Channel* aChannel;
    aChannel = &gChannelList[ gChannel ];
    return aChannel;
}

struct TestManager_Channel *    TestHelper_GetChannelIdx( int index )
{
    struct TestManager_Channel* aChannel;
    aChannel = &gChannelList[ index ];
    return aChannel;
}

void TestHelper_Unconfigure_TDAL_DMX( void )
{
    tTDAL_DMX_Error dmx_err;

	mTBOX_PRINT((kTBOX_LF, "Enter Demux unconfigure (%d)\n", DMX_is_configured));

	if (!DMX_is_configured)
    {
            return;
    }

	/* free audio channel */
    dmx_err = TDAL_DMX_Free_Channel(DMX_AudioChannelId);
    TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx free audio channel" );

	/* free video channel */
    dmx_err = TDAL_DMX_Free_Channel(DMX_VideoChannelId);
    TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx free video channel" );

	/* free pcr channel */
    dmx_err = TDAL_DMX_Free_Channel(DMX_PcrChannelId);
    TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx pcr video channel" );

    DMX_is_configured = false;
}

/* ---------------  AVS  ------------------ */
void TestHelper_Configure_TDAL_AVS( void )
{
#ifdef AVS_STB
    tTDAL_AVS_Error avs_err;
    avs_err = TDAL_AVS_SelectInput( eTDAL_AVS_VIDEO , eTDAL_AVS_OUT_TV , eTDAL_AVS_IN_ENCODER );
    TestManager_AssertEqual( avs_err, eTDAL_AVS_NO_ERROR, "avs select video input" );

    avs_err = TDAL_AVS_SelectInput( eTDAL_AVS_AUDIO , eTDAL_AVS_OUT_TV , eTDAL_AVS_IN_ENCODER );
    TestManager_AssertEqual( avs_err, eTDAL_AVS_NO_ERROR, "avs select audio input" );

    avs_err = TDAL_AVS_SelectSignalType(eTDAL_AVS_VIDEO , eTDAL_AVS_OUT_TV , eTDAL_AVS_CVBS );
    TestManager_AssertEqual( avs_err, eTDAL_AVS_NO_ERROR, "avs select signal type video" );

    avs_err = TDAL_AVS_SelectSignalType(eTDAL_AVS_AUDIO , eTDAL_AVS_OUT_TV , eTDAL_AVS_STEREO );
    TestManager_AssertEqual( avs_err, eTDAL_AVS_NO_ERROR, "avs select signal type audio" );

    avs_err = TDAL_AVS_SelectLevel( eTDAL_AVS_VIDEO , eTDAL_AVS_OUT_TV , 6 );
    TestManager_AssertEqual( avs_err, eTDAL_AVS_NO_ERROR, "avs select video level 6" );

    avs_err = TDAL_AVS_Mute( eTDAL_AVS_AUDIO , eTDAL_AVS_OUT_TV , eTDAL_AVS_MUTE_OFF );
    TestManager_AssertEqual( avs_err, eTDAL_AVS_NO_ERROR, "avs mute off" );

#endif
}

#ifndef CIPLUSVALIDATOR
/* ---------------  AV  ------------------ */
void TestHelper_Initialize_TDAL_AV( void )
{
#ifdef AVS_STB
    tTDAL_AVS_Error tavsErr;
#endif
    tTDAL_AV_Error tavErr;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_DMX_Error tdmxErr;
    tTDAL_HDMI_Error thdmiErr;
    tTDAL_OUTPUT_VideoHDDACParams stHDDACParams;
	tTDAL_OUTPUT_Capability       stOutputCap;
#ifdef PRODUCT_USE_CA
    tTDAL_DESC_Error tdescErr;
#endif

#ifdef AVS_STB
    /*      TDAL AVS      */
    tavsErr = TDAL_AVS_Init();
    TestManager_AssertEqual(tavsErr, eTDAL_AVS_NO_ERROR, "avs init");
#endif

    /*      TDAL AV      */
    tavErr = TDAL_AV_Init();
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av init");

    /*      TDAL DISP      */
    tdispErr = TDAL_DISP_Init();
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp init");

    /*      TDAL OUTPUT      */
    toutErr = TDAL_OUTPUT_Init();
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output init");

    /*      TDAL_HDMI       */
#ifdef TDAL_HDMI_USE
    thdmiErr = TDAL_HDMI_Init();
    TestManager_AssertEqual(thdmiErr, eTDAL_HDMI_NO_ERROR, "hdmi init" );
#endif
	memset(&stOutputCap, 0, sizeof(tTDAL_OUTPUT_Capability));
    TestManager_AssertEqual(TDAL_OUTPUT_CapabilityGet(&stOutputCap),
							eTDAL_OUTPUT_NO_ERROR,
							"output caps");

	if(stOutputCap.nbVideoHDDAC > 0)
	{
		stHDDACParams.ColorSpace     = eTDAL_OUTPUT_VIDEO_HD_DAC_ITU601;
		stHDDACParams.Format         = eTDAL_OUTPUT_VIDEO_HD_DAC_YCBCR;
		stHDDACParams.Standard       = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
		stHDDACParams.EmbeddedSyncOn = FALSE;
		TestManager_AssertEqual(TDAL_OUTPUT_VideoHDDACParamsSet(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0,
																&stHDDACParams),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac params set");
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output hd dac enable");
	}

#ifdef TDAL_HDMI_USE
	if(stOutputCap.nbHDMI > 0)
	{
		TestManager_AssertEqual(TDAL_HDMI_Start(eTDAL_HDMI_SOURCE_ID_0,
												HDMICallback,
												FALSE),
								eTDAL_HDMI_NO_ERROR,
								"hdmi start" );
	}
#endif

	if(stOutputCap.nbVideoDenc > 0)
	{
		TestManager_AssertEqual(TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0),
								eTDAL_OUTPUT_NO_ERROR,
								"output denc video enable" );
	}

    /*      TDAL DMX      */
    tdmxErr = TDAL_DMX_Init();
    TestManager_AssertEqual(tdmxErr, kTDAL_DMX_NO_ERROR, "dmx init");

#ifdef PRODUCT_USE_CA
    /*      TDAL DESC      */
    tdescErr = TDAL_DESC_Init();
    TestManager_AssertEqual(tdescErr, eTDAL_DESC_NO_ERROR, "desc init");
#endif

    /*      TDAL TSROUTE      */
    TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init");

    /*      TDAL DMD      */
    TestHelper_Initialize_TDAL_DMD();
}

/* ---------------  AV  ------------------ */
void TestHelper_Terminate_TDAL_AV( void )
{
#ifdef AVS_STB
    tTDAL_AVS_Error tavsErr;
#endif
    tTDAL_AV_Error tavErr;
    tTDAL_DISP_Error tdispErr;
    tTDAL_OUTPUT_Error toutErr;
    tTDAL_HDMI_Error thdmiErr;
    tTDAL_DMX_Error tdmxErr;
#ifdef PRODUCT_USE_CA
    tTDAL_DESC_Error tdescErr;
#endif

    /*      TDAL DMD      */
    TestHelper_Terminate_TDAL_DMD();

    TestHelper_Unconfigure_TDAL_TSROUTE(TEST_HELPER_TSROUTE_TUNER(gTDAL_AV_TestFeIdx),
                                        TEST_HELPER_TSROUTE_DEMUX(gTDAL_AV_TestDmxIdx));
    TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate");

#ifdef PRODUCT_USE_CA
    /*      TDAL DESC      */
    tdescErr = TDAL_DESC_Terminate();
    TestManager_AssertEqual(tdescErr, eTDAL_DESC_NO_ERROR, "desc terminate");
#endif

    /*      TDAL DMX      */
    TestHelper_Unconfigure_TDAL_DMX();
    tdmxErr = TDAL_DMX_Terminate();
    TestManager_AssertEqual(tdmxErr, kTDAL_DMX_NO_ERROR, "dmx terminate");

    /*      TDAL HDMI      */
#ifdef TDAL_HDMI_USE	
    thdmiErr = TDAL_HDMI_Term();
    TestManager_AssertEqual(thdmiErr, eTDAL_HDMI_NO_ERROR, "hdmi terminate");
#endif
    /*      TDAL OUTPUT      */
    toutErr = TDAL_OUTPUT_Term();
    TestManager_AssertEqual(toutErr, eTDAL_OUTPUT_NO_ERROR, "output terminate");

    /*      TDAL DISP      */
    tdispErr = TDAL_DISP_Term();
    TestManager_AssertEqual(tdispErr, eTDAL_DISP_NO_ERROR, "disp terminate");

    /*      TDAL AV      */
    tavErr = TDAL_AV_Terminate();
    TestManager_AssertEqual(tavErr, eTDAL_AV_NO_ERROR, "av terminate");

#ifdef AVS_STB
    /*      TDAL AVS      */
    tavsErr = TDAL_AVS_Term();
    TestManager_AssertEqual(tavsErr, eTDAL_AVS_NO_ERROR, "avs terminate");
#endif
}
#endif /* #ifndef CIPLUSVALIDATOR */

/* ----------------- TSROUTE ------------- */
void TestHelper_Configure_TDAL_TSROUTE( tTDAL_TS_Object source, tTDAL_TS_Object demux )
{
	mTBOX_PRINT((kTBOX_LF, "TSROUTE_Conect(src=0x%x,dmx=0x%x)\n",
				 source,
				 demux));
    TestManager_AssertEqual( TDAL_TSROUTE_Connect( source , demux ), eTDAL_TSROUTE_NO_ERROR, "tsroute connect" );
}

void TestHelper_Unconfigure_TDAL_TSROUTE( tTDAL_TS_Object source, tTDAL_TS_Object demux )
{
    TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( source , demux ), eTDAL_TSROUTE_NO_ERROR, "tsroute discconnect" );
}

#if defined(USE_TDAL_FS)

LOCAL uint8_t pTestHelper_FsDeviceCount = 0;

LOCAL void pTestHelper_FsDeviceCallback
(
    tTDAL_FS_DeviceInfo *pDeviceInfo,
    tTDAL_FS_DeviceEvent DeviceEvent
)
{
	if(DeviceEvent == eTDAL_FS_DEVICE_EVENT_INSERTED)
	{
        mTBOX_TRACE(( kTBOX_NIV_1,"INSERTION Device[%2d]=handle=%8x,name=%s,size=%d*%d,type=%s\n",
                      pTestHelper_FsDeviceCount,
                      pDeviceInfo->handle,
                      pDeviceInfo->name,
                      pDeviceInfo->block_count,
                      pDeviceInfo->block_size,
                      pTestHelper_FsDeviceTypeToString(pDeviceInfo->type)));
        pTestHelper_FsDeviceCount++;
    }
}

void TestHelper_AskForVolume_TDAL_FS( tTDAL_FS_VolumeHandle * p_handle )
{
    static bool             bFirstCall = FALSE;
    uint8_t                 dev_index;
    uint8_t                 vol_index;

    uint8_t                 devices_count;
    tTDAL_FS_DeviceInfo *   p_devices_array;
    uint8_t                 volumes_count;
    tTDAL_FS_VolumeInfo *   p_volumes_array;

    tTDAL_FS_DeviceHandle   device_to_be_formatted = 0xFFFFFFFF;

    if(bFirstCall == FALSE)
    {
        TestManager_AssertEqual(TDAL_FS_Init(),
                                eTDAL_FS_NO_ERROR,
                                "fs init" );
        bFirstCall = TRUE;

        TestManager_AssertEqual(TDAL_FS_RegisterDeviceCallback(pTestHelper_FsDeviceCallback),
                                eTDAL_FS_NO_ERROR,
                                "fs register callback");

        mTBOX_PRINT((kTBOX_LF,"Wait for FS device notification...\n"));
        while(pTestHelper_FsDeviceCount == 0)
        {
            TKEL_DelayTask(ms2tick(100));
        };

    }

    TestManager_AssertEqual(TDAL_FS_ListDevices(&devices_count,
                                                &p_devices_array),
                            eTDAL_FS_NO_ERROR,
                            "FS list devices");

    mTBOX_PRINT((kTBOX_LF,"Listed %d devices\n", devices_count));

    if ( devices_count != 0 )
    {
        dev_index = 0;
        if ( devices_count != 1 )
        {
            for( dev_index=0 ; dev_index<devices_count ; dev_index++ )
            {
                mTBOX_PRINT(( kTBOX_LF,"Device[%2d] = { %8x , %s , %d*blocks , %d , %s}\n",
                              dev_index ,
                              p_devices_array[dev_index].handle,
                              p_devices_array[dev_index].name,
                              p_devices_array[dev_index].block_count,
                              p_devices_array[dev_index].block_size,
                              pTestHelper_FsDeviceTypeToString(p_devices_array[dev_index].type)));
            }
            dev_index = TestManager_GetNumber("Select a device :", 1);
        }

        TestManager_AssertEqual(TDAL_FS_ListVolumes(p_devices_array[dev_index].handle,
                                                    &volumes_count,
                                                    &p_volumes_array),
                                eTDAL_FS_NO_ERROR,
                                "FS list volumes");
        if (volumes_count == 0)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ListVolumes() on device \"%s\" succeed => No volume found\n",
                       p_devices_array[dev_index].name));
            if (device_to_be_formatted == 0xFFFFFFFF)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Device \"%s\" would be formatted\n",
                           p_devices_array[dev_index].name));
                device_to_be_formatted = p_devices_array[dev_index].handle;
            }
        }
        else
        {
            mTBOX_PRINT((kTBOX_LF,"There is %d volumes(s) on device \"%s\"\n",
                       volumes_count, p_devices_array[dev_index].name));
            mTBOX_PRINT((kTBOX_LF,"Volumes[XX] = {  handle  |                 name | nb of blocks | type }\n"));

            vol_index = 0;
			for( vol_index=0 ; (vol_index<volumes_count); vol_index++ )
			{
				mTBOX_PRINT((kTBOX_LF,"Volumes[%2d] = { %8x | %20s |  %10d  | %s }\n",
						  vol_index,
						  p_volumes_array[vol_index].handle,
						  p_volumes_array[vol_index].name,
						  p_volumes_array[vol_index].size,
							  pTestHelper_FsVolumeTypeToString(p_volumes_array[vol_index].type)
							  ));
			}
            vol_index = 0;
            if ( volumes_count != 1 )
            {
                vol_index = TestManager_GetNumber("Select a volume :", 1);
            }

            *p_handle = p_volumes_array[vol_index].handle;


            TestManager_AssertEqual(TDAL_FS_FreeVolumesArray(p_volumes_array),
                                    eTDAL_FS_NO_ERROR,
                                    "FS free volumes");

        }
    }

    if ( devices_count != 0 )
    {
        TestManager_AssertEqual(TDAL_FS_FreeDevicesArray( p_devices_array ),
                                eTDAL_FS_NO_ERROR,
                                "FS free devices");

    }
}


void TestHelper_Configure_TDAL_FS( void )
{
    tTDAL_FS_VolumeHandle   volume_handle;

    TestHelper_AskForVolume_TDAL_FS( &volume_handle );

    TestManager_AssertEqual(TDAL_FS_Mount(volume_handle,FS_HDD_LOCATION,kTDAL_FS_MODE_RDWR),eTDAL_FS_NO_ERROR,"FS mount");
}


void TestHelper_Unconfigure_TDAL_FS( void )
{

    TestManager_AssertEqual(TDAL_FS_Umount(FS_HDD_LOCATION),
											eTDAL_FS_NO_ERROR,
											"FS unmount");
}


LOCAL char * pTestHelper_FsVolumeTypeToString
(
    tTDAL_FS_VolumeType eType
)
{
	switch(eType)
	{
        case eTDAL_FS_VOLUME_TYPE_E2FS:         return("E2FS");
        case eTDAL_FS_VOLUME_TYPE_FAT12:        return("FAT12");
        case eTDAL_FS_VOLUME_TYPE_FAT16:        return("FAT16");
        case eTDAL_FS_VOLUME_TYPE_FAT32:        return("FAT32");
        case eTDAL_FS_VOLUME_TYPE_FAT32_LBA:    return("FAT32_LBA");
        case eTDAL_FS_VOLUME_TYPE_NTFS:         return("NTFS");
        case eTDAL_FS_VOLUME_TYPE_E3FS:         return("E3FS");
        case eTDAL_FS_VOLUME_TYPE_ISO9660:      return("ISO9660");
        case eTDAL_FS_VOLUME_TYPE_SMBFS:        return("SMBFS");
        case eTDAL_FS_VOLUME_TYPE_UNKNOWN:
        default:                                return("unknown");
	}
}

LOCAL char * pTestHelper_FsDeviceTypeToString
(
    tTDAL_FS_DeviceType eType
)
{
    switch(eType)
    {
    case eTDAL_FS_DEVICETYPE_FIXED_STORAGE:
        return("FIXED_STORAGE");
        break;
    case eTDAL_FS_DEVICETYPE_REMOVABLE_STORAGE:
        return("REMOVABLE_STORAGE");
        break;
    case eTDAL_FS_DEVICETYPE_UNKNOWN:
    default:
        return("UNKNOWN");
        break;
    }
}

#endif /* USE_TDAL_FS */

uint32_t TestHelper_DMXGetStreamHandle(tTDAL_AV_Decoder decoder)
{
    tTDAL_DMX_Error             dmx_err;
	uint32_t                        VideoStreamHandle;
	uint32_t                        AudioStreamHandle;

	if(decoder==eTDAL_AV_DECODER_VIDEO_1)
	{
		dmx_err = TDAL_DMX_StreamHandleGet (DMX_VideoChannelId, &VideoStreamHandle);
		TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx stream handle get video" );

		return VideoStreamHandle;
	}

	else if(decoder==eTDAL_AV_DECODER_AUDIO_1)
	{
        dmx_err = TDAL_DMX_StreamHandleGet (DMX_AudioChannelId, &AudioStreamHandle);
		TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx stream handle get audio" );

		return AudioStreamHandle;
	}

	return 0;
}

void TestHelper_Initialize_TDAL_FLA(void)
{
    TDAL_FLA_Init();
    TDAL_FLA_Open();
}

void TestHelper_Terminate_TDAL_FLA(void)
{
    TDAL_FLA_Close();
    TDAL_FLA_Term();
}

void TestHelper_Initialize_TDAL_DMX(void)
{
    TDAL_DMX_Init();
}

void TestHelper_Terminate_TDAL_DMX(void)
{
    TDAL_DMX_Terminate();
}

void TestHelper_OpenComponentsForTDAL_HDMI(){

    /*      TDAL AV      */
    TestManager_AssertEqual(TDAL_AV_Init(), eTDAL_AV_NO_ERROR, "av init");

    /*      TDAL DISP      */
    TestManager_AssertEqual(TDAL_DISP_Init(), eTDAL_DISP_NO_ERROR, "disp init");

    /*      TDAL OUTPUT      */
    TestManager_AssertEqual(TDAL_OUTPUT_Init(), eTDAL_OUTPUT_NO_ERROR, "output init");

}

void TestHelper_CloseComponentsForTDAL_HDMI(){

    /*      TDAL AV      */
    TestManager_AssertEqual(TDAL_AV_Terminate(), eTDAL_AV_NO_ERROR, "av init");

    /*      TDAL DISP      */
    TestManager_AssertEqual(TDAL_DISP_Term(), eTDAL_DISP_NO_ERROR, "disp init");

    /*      TDAL OUTPUT      */
    TestManager_AssertEqual(TDAL_OUTPUT_Term(), eTDAL_OUTPUT_NO_ERROR, "output init");

}

void TestHelper_Configure_TDAL_ConnectionManager(tTestManager_ConnectionManagerType type)
{
}
