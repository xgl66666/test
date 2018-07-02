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
#ifndef CIPLUSVALIDATOR

#include <string.h>     /* for memset() */

#include "crules.h"

#include "testhelper.h"
#include "TestManager.h"
#include "tester_prefs.h"

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
#include "tdal_ana_vid.h"
#include "tdal_scart_input.h"

#include "tdal_hdmi_in.h"
#include "tdal_panel.h"
#include "tdal_speakers.h"

#include "tkel.h"
#include "tbox.h"

#include "tdal_connection_manager.h"

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

extern void ANA_VID_CallbackProc( tTDAL_ANA_VID_Decoder eDecoder,
                                  tTDAL_ANA_VID_Event   eEvent,
                                  const void           *pEventData );

extern void ANA_VID_CallbackProcSub( tTDAL_ANA_VID_Decoder eDecoder,
                                  tTDAL_ANA_VID_Event   eEvent,
                                  const void           *pEventData );


extern void ANA_AUD_CallbackProc( tTDAL_ANA_AUD_Decoder eDecoder,
                                  tTDAL_ANA_AUD_Event   eEvent,
                                  const void           *pEventData );

extern void VBI_SLICER_CallbackVPS( tTDAL_VBI_SLICER_ClientId  stClientId,
                                    uint32_t                   ulInputHandle,
                                    uint32_t                   ulVpsDataMsb,
                                    uint32_t                   ulVpsDataLsb);

extern void VBI_SLICER_CallbackWST( tTDAL_VBI_SLICER_ClientId  stClientId,
                                    uint32_t                   ulInputHandle,
                                    uint16_t                   usSizeOfData,
                                    const void                 *pData);

extern void VBI_SLICER_CallbackWSS( tTDAL_VBI_SLICER_ClientId  stClientId,
                                    uint32_t                   ulInputHandle,
                                    uint16_t                   usWssData);


extern void SCART_INPUT_CallbackFct
(
    tTDAL_SCART_INPUT_Id     eId,
    tTDAL_SCART_INPUT_Event  eEvent,
    const void              *pEventData
);



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
struct TestManager_Channel gChannelList[] = {
#if defined (MEDIUM_SAT)
{
    "ASTRA",
    "EinsExtra",
    10735000,
    22000000,
    eTDAL_DMD_MOD_QPSK,
    eTDAL_DMD_SPECTRUM_AUTO,
    {
        eTDAL_DMD_FEC_5_6,
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
    "Player 1",     /* char *mSatellite : Name of the satelite */
    "Channel 1",    /* char *mChannelName :  Name of the channel */
    618000000,          /* uint32_t  TunFrequency */
    6875000,            /* uint32_t  SymbolRateBw */
    eTDAL_DMD_MOD_QAM64,        /* tTDAL_DMD_Modulation         Modulation */
    eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
    0,               /* tTDAL_DMD_SpecificTunData    Specific */
    0,      /* uint16_t mPMTPID : PID to demux PMT */
    47,     /* uint16_t mVideoPID : PID to demux video */
    46,     /* uint16_t mAudioPID : PID to demux audio */
    47,     /* uint16_t mPCRPID PID to demux PCR */
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{
    "Player 2",
    "Channel 1",
    794000000,
    6875000,
    eTDAL_DMD_MOD_QAM64,
    eTDAL_DMD_SPECTRUM_AUTO,
    0,
    0,
    120,
    130,
    120,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{ NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,0,0,0,
}
#elif defined (MEDIUM_TER)
{ /* 0 */
    "LIVE",
    "TF1",
	562000000,          /* uint32_t  TunFrequency */
    {
        8000000,            /* uint32_t  SymbolRateBw */
        eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
        eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    0,
	120,
	130,
	120,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},

{/* 1 */
    "LIVE",
    "FRANCE 2",
    586000000,          /* uint32_t  TunFrequency */
    {
        8000000,            /* uint32_t  SymbolRateBw */
        eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
        eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    0,
    120,
    130,
    120,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{/* 2 */
    "LIVE",
    "ITELE",
    674000000,          /* uint32_t  TunFrequency */
    {
        8000000,            /* uint32_t  SymbolRateBw */
        eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
        eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    0,
    163,
    92,
    163,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{/* 3 */
    "LIVE",
    "W9",
    626000000,          /* uint32_t  TunFrequency */
    {
        8000000,            /* uint32_t  SymbolRateBw */
        eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
        eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    0,
    220,
    230,
    220,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{ /* 4 */
    "FALSTER",
    "DTG FP 1",
    490000000,          /* uint32_t  TunFrequency */
    {
        8000000,                    /* uint32_t  SymbolRateBw */
        eTDAL_DMD_MOD_AUTO,         /* tTDAL_DMD_Modulation         Modulation */
        eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    0,
    258,
    0,
    268,
	eTDAL_AV_VIDEO_TYPE_MPEG2,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	720,
	576
},
{/* 5 */
    "LIVE R5",
    "TF1 HD",
    602000000,          /* uint32_t  TunFrequency */
    {
		8000000,            /* uint32_t  SymbolRateBw */
		eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
		eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_8,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    110,
    120,
    130,
    120,
	eTDAL_AV_VIDEO_TYPE_H264,
	eTDAL_AV_AUDIO_TYPE_HE_AAC,
	1440,
	1080
},
{/* 6 */
    "LIVE R5",
    "FRANCE2 HD",
    602001000,          /* uint32_t  TunFrequency */
    {
		8000000,            /* uint32_t  SymbolRateBw */
		eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
		eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_8,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    210,
    220,
    230,
    220,
	eTDAL_AV_VIDEO_TYPE_H264,
	eTDAL_AV_AUDIO_TYPE_HE_AAC,
	1440,
	1080
},
{/* 7 */
    "LIVE R4",
    "ARTE HD",
    626000000,          /* uint32_t  TunFrequency */
    {
		8000000,            /* uint32_t  SymbolRateBw */
		eTDAL_DMD_MOD_AUTO,        /* tTDAL_DMD_Modulation         Modulation */
		eTDAL_DMD_SPECTRUM_AUTO,    /* tTDAL_DMD_Spectrum           SpecInv */
        eTDAL_DMD_MODE_8K,
        eTDAL_DMD_GUARD_1_32,
        eTDAL_DMD_HIER_AUTO,
        eTDAL_DMD_PRIORITY_STREAM_HIGH,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_FEC_AUTO,
        eTDAL_DMD_OFFSET_AUTO,
    },
    710,
    720,
    730,
    720,
	eTDAL_AV_VIDEO_TYPE_H264,
	eTDAL_AV_AUDIO_TYPE_MPEG,
	1440,
	1080
},
{ NULL, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0,
	0,0,0,0,
 }

#endif

};

struct TestManager_Channel_Analog gAnalogChannelList[] = {
{
    "LIVE ANALOG",
    "TF1",
    615310000,          /* uint32_t  TunFrequency */

    {
		8000000,
        eTDAL_DMD_ANALOG_STD_SECAM_L,
        TRUE,
        0,
    },
},
{
    "LIVE ANALOG",
    "FR2",
    575031000,          /* uint32_t  TunFrequency */
    {
		8000000,
        eTDAL_DMD_ANALOG_STD_SECAM_L,
        TRUE,
        0,
    },
},
{
    NULL, NULL, 0, {0, 0, 0, 0}
}
};

/* In analog input list,
 * the video inputs must be declared first within the SCART inputs must be declared first.
 * then the audio inputs are declared.
 * The index starts from 0 for each input type (CVBS, YC, ...)
 */
struct TestManager_Input_Analog gAnalogInputList[] = {
{"SCART1 CVBS", eANALOG_INPUT_TYPE_VIDEO_CVBS, 0,0},
{"SCART1 YC",   eANALOG_INPUT_TYPE_VIDEO_YC,   0,0},
{"SCART1 RGB",  eANALOG_INPUT_TYPE_VIDEO_RGB_S,0,0},
{"SCART2 CVBS", eANALOG_INPUT_TYPE_VIDEO_CVBS, 1,0},
{"SCART2 YC",   eANALOG_INPUT_TYPE_VIDEO_YC,   1,0},
{"ANA1 CVBS",   eANALOG_INPUT_TYPE_VIDEO_CVBS, 2,0},
{"ANA2 YC",     eANALOG_INPUT_TYPE_VIDEO_YC,   2,0},
{"ANA3 YPBPR",  eANALOG_INPUT_TYPE_VIDEO_YPBPR,1,1},
{"ANA3 VGA"  ,  eANALOG_INPUT_TYPE_VIDEO_VGA  ,2,1},
{"SCART1 AUDIO",eANALOG_INPUT_TYPE_AUDIO,      0,0},
{"SCART2 AUDIO",eANALOG_INPUT_TYPE_AUDIO,      1,0},
{"ANA1 AUDIO ", eANALOG_INPUT_TYPE_AUDIO,      2,0},
{"ANA2 AUDIO ", eANALOG_INPUT_TYPE_AUDIO,      3,0},
{"ANA3 AUDIO ", eANALOG_INPUT_TYPE_AUDIO,      4,0},
{NULL, 0, 0,}
};

int gChannel = 0;
int gChannelAnalog =0;
int gInputAnalog =0;
bool bUseAnalogChannel = false;
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
    int                     i;
    tTDAL_DMD_Error         dmd_err;
    tTDAL_DMD_ConfigData    psCfgData;

    for(i = 0; i < NUMBER_OF_DMD; i++)
    {
        /* create semaphore for callback */
        if(TDAL_DMD_TST_NotifySem[i] == NULL)
        {
            TKEL_CreateSemaphore(0, &TDAL_DMD_TST_NotifySem[i]);
        }

        dmd_err = TDAL_DMD_Init();
        /* init DMD module */
        dmd_err = TDAL_DMD_OpenFEInstance( eTDAL_DMD_FE_0,
                                 pTestHelper_DMDStatusFct,
                                 pTestHelper_DMDProgressFct );
        TestManager_AssertEqual( dmd_err, eTDAL_DMD_NO_ERROR, "dmd init" );

        /* configure DMD */
#if defined (MEDIUM_SAT)
        psCfgData.Sat.ConfigType = eTDAL_DMD_CFG_SAT_INIT;
        TestHelper_Initialize_TDAL_DMD_Config( &psCfgData );
        TestManager_AssertEqual( TDAL_DMD_Config(DMD_FeID[i], &psCfgData), eTDAL_DMD_NO_ERROR, "dmd config" );
        psCfgData.Sat.ConfigType = eTDAL_DMD_CFG_SAT_OL_BAND;
        TestHelper_Initialize_TDAL_DMD_Config( &psCfgData );
        TestManager_AssertEqual( TDAL_DMD_Config(DMD_FeID[i], &psCfgData), eTDAL_DMD_NO_ERROR, "dmd config" );
#else
        psCfgData.Ter.ConfigType            = eTDAL_DMD_CFG_TER_SOURCE_TRANSITION;
		psCfgData.Ter.uCfg.SourceTransition = eTDAL_DMD_CFG_SOURCE_IS_DTV;
		TestManager_AssertEqual( TDAL_DMD_Config(DMD_FeID[i], &psCfgData), eTDAL_DMD_NO_ERROR, "dmd config SOURCE_TRANSITION" );

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
        DMD_Config->Sat.uCfg.Init.pSymbolRateList = NULL;
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
    mTBOX_TRACE((kTBOX_NIV_1, "tuning %s on %s\n", aChannel->mChannelName, aChannel->mSatellite ));

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
    struct TestManager_Channel*         aChannel=NULL;
    struct TestManager_Channel_Analog*  aAnalogChannel=NULL;
    tTDAL_DMD_TunData                   sTunData;

    if(bUseAnalogChannel)
	{
        aAnalogChannel = &gAnalogChannelList[gChannelAnalog];
		mTBOX_TRACE((kTBOX_NIV_1,"analog tuning %s on %s\n",
					 aAnalogChannel->mChannelName,
					 aAnalogChannel->mSatellite));
		mTBOX_TRACE((kTBOX_NIV_1,"analog frequence %d std=0x%x\n",
					 aAnalogChannel->TunFrequency,
					 aAnalogChannel->AnalogTer.eStandardSystem));
        sTunData.AnalogTer = aAnalogChannel->AnalogTer;
		
		TestHelper_DMDTuneSpecific(iFeIdx, sTunData, aAnalogChannel->TunFrequency);
	}
    else
	{
        aChannel = &gChannelList[gChannel];
		mTBOX_TRACE((kTBOX_NIV_1,"tuning %s on %s\n",
					 aChannel->mChannelName,
					 aChannel->mSatellite));
		mTBOX_TRACE((kTBOX_NIV_1,"frequence %d \n",
					 aChannel->TunFrequency));
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
		/* */
	}

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
    if(iDmxIdx > (eTDAL_DMX_NB_DEMUX-1))
    {
        return;
    }

    if (DMX_is_configured)
    {
            TestHelper_Unconfigure_TDAL_DMX();
    }

    /* setup video filtering */
    if (pid_video != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx] ,&VideoType, &DMX_VideoChannelId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx allocate video channel" );

		dmx_err = TDAL_DMX_StreamHandleGet (DMX_VideoChannelId, &DMX_VideoStreamId);
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "dmx stream handle get video" );

		dmx_err = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_VIDEO_1, DMX_VideoStreamId );
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "av input stream set video" );

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

		dmx_err = TDAL_AV_InputStreamSet(eTDAL_AV_DECODER_AUDIO_1, DMX_AudioStreamId );
        TestManager_AssertEqual( dmx_err, kTDAL_DMX_NO_ERROR, "av input stream set audio" );

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
	/* update PID values*/
	DMX_VideoPid    = aChannel->mVideoPID;
	DMX_AudioPid    = aChannel->mAudioPID;
	DMX_PcrPid      = aChannel->mPCRPID;
	/* */

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

struct TestManager_Input_Analog *    TestHelper_GetAnalogInput( void )
{
    struct TestManager_Input_Analog* aInput;
    aInput = &gAnalogInputList[ gInputAnalog ];
    return aInput;
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

#ifdef TDAL_HDMI_USE
    /*      TDAL_HDMI       */
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
    tTDAL_FS_Error          tdal_fs_err;
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

    TestManager_AssertEqual(TDAL_FS_Mount(volume_handle,
                                          "/hdd",
                                          kTDAL_FS_MODE_RDWR),
                            eTDAL_FS_NO_ERROR,
                            "FS mount");
}


void TestHelper_Unconfigure_TDAL_FS( void )
{

    TestManager_AssertEqual(TDAL_FS_Umount("/hdd"),
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

void TestHelper_Initialize_ANA_VID ( tTDAL_ANA_VID_Decoder* pIndexDecoder, tTDAL_ANA_VID_DecoderCapability* pstDecoderCapability, tTDAL_ANA_VID_Coding eVideoCoding  )
{
    tTDAL_ANA_VID_Capability        stCapability;
    tTDAL_ANA_VID_Event             eIndexEvent;
    tTDAL_ANA_VID_Decoder           eIndexDecoder;
    uint32_t                        eIndexStdMask;
    uint32_t                        eIndexCodingMask;
	uint8_t                         found = 0;
	uint8_t 						i;
	uint8_t 						nbDec;

    TestManager_AssertEqual( TDAL_ANA_VID_Init(), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_Init succeed" );

    /* Get a analog video decoder*/
    TestManager_AssertEqual( TDAL_ANA_VID_CapabilityGet(&stCapability), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_CapabilityGet succeed" );

	nbDec = 4;


    for(i=0; i<=nbDec && !found; i++ )
    {

		eIndexDecoder = 1 << i;
		
        if(stCapability.uiDecoderSupported & eIndexDecoder)
        {

		    /* Get capability of the video decoder*/
		    TestManager_AssertEqual( TDAL_ANA_VID_DecoderCapabilityGet(eIndexDecoder, pstDecoderCapability), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_DecoderCapabilityGet succeed" );

		    /* Which standart is supported ? */
		    for(eIndexStdMask=eTDAL_ANA_VID_STD_UNKNOWN; eIndexStdMask<=eTDAL_ANA_VID_STD_PC; eIndexStdMask++)
		    {
		        if(pstDecoderCapability->uiSupportedStdMask && eIndexStdMask)
		        {
		            mTBOX_PRINT((kTBOX_LF,"Analog Video Standart [%x] supported\n", eIndexStdMask));
		        }
		    }


					if(eVideoCoding & pstDecoderCapability->uiSupportedCodingMask)
					{
						found = 1;

					    /* Register Event */
					    for(eIndexEvent=eTDAL_ANA_VID_EVENT_TYPE_CHANGE; eIndexEvent<=eTDAL_ANA_VID_EVENT_NEED_DISPLAY; eIndexEvent++)
					    {
					        TestManager_AssertEqual( TDAL_ANA_VID_EventSubscribe(eIndexDecoder, eIndexEvent, (tTDAL_ANA_VID_CallbackProc)ANA_VID_CallbackProc), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_EventSubscribe succeed" );
					    }
					}
					else
					{
/* printf("video decoder not match %d video coding %x cap %x  \n",eIndexDecoder,
				eVideoCoding,
				pstDecoderCapability->uiSupportedCodingMask
				); */

					}
         }
    }

    /* save decoder used*/
	if(found)
	{
		*pIndexDecoder = eIndexDecoder;
	}
}


void TestHelper_Initialize_ANA_VID_SUB ( tTDAL_ANA_VID_Decoder* pIndexDecoder, tTDAL_ANA_VID_DecoderCapability* pstDecoderCapability, tTDAL_ANA_VID_Coding eVideoCoding  )
{
    tTDAL_ANA_VID_Capability        stCapability;
    tTDAL_ANA_VID_Event             eIndexEvent;
    tTDAL_ANA_VID_Decoder           eIndexDecoder;
    uint32_t                        eIndexStdMask;
    uint32_t                        eIndexCodingMask;
	uint8_t                         found = 0;
	uint8_t 						i;
	uint8_t 						nbDec;


    /* Get a analog video decoder*/
    TestManager_AssertEqual( TDAL_ANA_VID_CapabilityGet(&stCapability), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_CapabilityGet succeed" );

	nbDec = 4;


    for(i=0; i<=nbDec && !found; i++ )
    {

		eIndexDecoder = 1 << i;
		
        if(stCapability.uiDecoderSupported & eIndexDecoder)
        {

		    /* Get capability of the video decoder*/
		    TestManager_AssertEqual( TDAL_ANA_VID_DecoderCapabilityGet(eIndexDecoder, pstDecoderCapability), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_DecoderCapabilityGet succeed" );

		    /* Which standart is supported ? */
		    for(eIndexStdMask=eTDAL_ANA_VID_STD_UNKNOWN; eIndexStdMask<=eTDAL_ANA_VID_STD_PC; eIndexStdMask++)
		    {
		        if(pstDecoderCapability->uiSupportedStdMask && eIndexStdMask)
		        {
		            mTBOX_PRINT((kTBOX_LF,"Analog Video Standart [%x] supported\n", eIndexStdMask));
		        }
		    }


					if(eVideoCoding & pstDecoderCapability->uiSupportedCodingMask)
					{
						found = 1;

					    /* Register Event */
					    for(eIndexEvent=eTDAL_ANA_VID_EVENT_TYPE_CHANGE; eIndexEvent<=eTDAL_ANA_VID_EVENT_NEED_DISPLAY; eIndexEvent++)
					    {
					        TestManager_AssertEqual( TDAL_ANA_VID_EventSubscribe(eIndexDecoder, eIndexEvent, (tTDAL_ANA_VID_CallbackProc)ANA_VID_CallbackProcSub), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_VID_EventSubscribe succeed" );
					    }
					}
					else
					{

					}
         }
    }

    /* save decoder used*/
	if(found)
	{
		*pIndexDecoder = eIndexDecoder;
	}
}


void TestHelper_Initialize_ANA_AUD ( tTDAL_ANA_AUD_Decoder* IndexDecoder, tTDAL_ANA_AUD_DecoderCapability* pstDecoderCapability  )
{
    tTDAL_ANA_AUD_Capability        stCapability;
    tTDAL_ANA_AUD_Event             eIndexEvent;
    tTDAL_ANA_AUD_Decoder           eIndexDecoder;
    uint32_t                        eIndexSoundStdMask;

    TestManager_AssertEqual( TDAL_ANA_AUD_Init(), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_Init succeed" );

    /* Get a analog audio decoder*/
    TestManager_AssertEqual( TDAL_ANA_AUD_CapabilityGet(&stCapability), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_CapabilityGet succeed" );
    for(eIndexDecoder=eTDAL_ANA_AUD_DECODER_1; eIndexDecoder<=eTDAL_ANA_AUD_DECODER_4; eIndexDecoder++)
    {
        if(stCapability.uiDecoderSupported && eIndexDecoder)
        {
            break;
        }
    }

    /* Get capability of the audio decoder*/
    TestManager_AssertEqual( TDAL_ANA_AUD_DecoderCapabilityGet(eIndexDecoder, pstDecoderCapability), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_DecoderCapabilityGet succeed" );

    /* Which standart is supported ? */
    for(eIndexSoundStdMask=eTDAL_ANA_AUD_SOUND_STD_UNKNOWN; eIndexSoundStdMask<=eTDAL_ANA_AUD_SOUND_STD_FM_NICAM; eIndexSoundStdMask++)
    {
        if(pstDecoderCapability->uiSupportedStdMask && eIndexSoundStdMask)
        {
            mTBOX_PRINT((kTBOX_LF,"Analog Audio Standart [%x] supported\n", eIndexSoundStdMask));
        }
    }

    /* Register Event */
    for(eIndexEvent=eTDAL_ANA_AUD_EVENT_SIGNAL_CHANGED; eIndexEvent<=eTDAL_ANA_AUD_EVENT_SIGNAL_SWITCH; eIndexEvent++)
    {
        TestManager_AssertEqual( TDAL_ANA_AUD_EventSubscribe(eIndexDecoder, eIndexEvent, (tTDAL_ANA_AUD_CallbackProc)ANA_AUD_CallbackProc), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_EventSubscribe succeed" );
    }

    /* save decoder used*/
    *IndexDecoder = eIndexDecoder;
}

void TestHelper_Initialize_VBI_SLICER ( tTDAL_VBI_SLICER_Capability* pstDecoderCapability  )
{
    tTDAL_VBI_SLICER_Standard       eIndexStd;
    tTDAL_VBI_SLICER_DataType       eIndexData;

    TestManager_AssertEqual( TDAL_VBI_SLICER_Init(), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_Init succeed" );

    /* Get capability of the vbi slicer*/
    TestManager_AssertEqual( TDAL_VBI_SLICER_CapabilityGet(pstDecoderCapability), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_CapabilityGet succeed" );
    for(eIndexStd=eTDAL_VBI_SLICER_STANDARD_PAL; eIndexStd<=eTDAL_VBI_SLICER_STANDARD_NTSC; eIndexStd++)
    {
        if(pstDecoderCapability->StandardSupportedBitmask & eIndexStd)
        {
            mTBOX_PRINT((kTBOX_LF,"VBI SLICER Standart [%x] supported\n", eIndexStd));
        }
    }
    for(eIndexData=eTDAL_VBI_SLICER_DATA_VPS; eIndexData<=eTDAL_VBI_SLICER_DATA_WSS; eIndexData++)
    {
        if(pstDecoderCapability->StandardSupportedBitmask & eIndexData)
        {
            mTBOX_PRINT((kTBOX_LF,"VBI SLICER DataType [%x] supported\n", eIndexData));
        }
    }
}

	
void TestHelper_Initialize_ANA_INPUT ( tTDAL_ANA_INPUT_Object *p_VidObject,
                                       tTDAL_ANA_INPUT_Object *p_VidMuxObj,
									   tTDAL_ANA_INPUT_Object *p_AudMuxObj,
									   tTDAL_ANA_VID_Coding   *pstVidCoding  )
{
	tTDAL_ANA_INPUT_Object object;
	tTDAL_ANA_INPUT_Object muxobject;

    TestManager_AssertEqual( TDAL_ANA_INPUT_Init(),
							 eTDAL_ANA_INPUT_NO_ERROR,
							 "TDAL_ANA_INPUT_Init succeed" );

	/* ask for video input object and its video coding */
	TestHelper_AskForInput_ANA_INPUT(&object, TRUE, FALSE, pstVidCoding, &muxobject);

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(object, muxobject),
							 eTDAL_ANA_INPUT_NO_ERROR,
							 "TDAL_ANA_INPUT_VideoInputSelect succeed" );

	*p_VidMuxObj = muxobject;
	*p_VidObject = object;

	if(p_AudMuxObj != NULL)
	{
		/* ask for audio input */
		TestHelper_AskForInput_ANA_INPUT(&object, FALSE, TRUE, NULL, &muxobject);

		TestManager_AssertEqual( TDAL_ANA_INPUT_AudioInputSelect(object, muxobject),
								 eTDAL_ANA_INPUT_NO_ERROR,
								 "TDAL_ANA_INPUT_AudioInputSelect succeed" );

		*p_AudMuxObj = muxobject;
	}
}

void TestHelper_Initialize_ANA_INPUT_SUB ( tTDAL_ANA_INPUT_Object *p_VidObject,
                                       tTDAL_ANA_INPUT_Object *p_VidMuxObj,
									   tTDAL_ANA_VID_Coding   *pstVidCoding  )
{
	tTDAL_ANA_INPUT_Object object;
	tTDAL_ANA_INPUT_Object muxobject;

	/* ask for video input object and its video coding */
	TestHelper_AskForInput_ANA_INPUT(&object, TRUE, FALSE, pstVidCoding,&muxobject);

	TestManager_AssertEqual( TDAL_ANA_INPUT_VideoInputSelect(object,
															 muxobject),
									 eTDAL_ANA_INPUT_NO_ERROR,
									 "TDAL_ANA_INPUT_VideoInputSelect succeed" );
	*p_VidMuxObj = muxobject;
	*p_VidObject = object;
}




void TestHelper_Initialize_SCART_INPUT (  )
{
	uint8_t i;
	tTDAL_SCART_INPUT_Capability stCapability;
	tTDAL_SCART_INPUT_Id eId = eTDAL_SCART_INPUT_ID_0;
	tTDAL_SCART_INPUT_ScartCapability stScartCapability;
	
    TestManager_AssertEqual( TDAL_SCART_INPUT_Init(), eTDAL_ANA_INPUT_NO_ERROR, "TDAL_ANA_INPUT_Init succeed" );

    /* Get capability of the vbi slicer*/
	TestManager_AssertEqual( TDAL_SCART_INPUT_CapabilityGet(&stCapability),
                             eTDAL_SCART_INPUT_NO_ERROR,
							 "TDAL_SCART_INPUT_CapabilityGet succeed" );	

	for (i = 0; i < stCapability.nbInput; i++)
	{
		eId  = eTDAL_SCART_INPUT_ID_0 + i;
        TestManager_AssertEqual( TDAL_SCART_INPUT_GetScartCapability(eTDAL_SCART_INPUT_ID_0,&stScartCapability),
							     eTDAL_SCART_INPUT_NO_ERROR,
							     "TDAL_SCART_INPUT_GetScartCapability for scart succeed" );

	    mTBOX_PRINT((kTBOX_LF,"SCART_INPUT Capability : scart Id %d [ScartId=%d]; [output capable=%d] [RGB capable=%d] [YC capable=%d]\n",
                 eId,
		         stScartCapability.bOutputCapable,
				 stScartCapability.bRGBCapable,
				 stScartCapability.bYCCapable));

		TestManager_AssertEqual( TDAL_SCART_INPUT_SubscribeEvent(eId,eTDAL_SCART_INPUT_EVENT_SLOW_BLANKING_CHANGE,SCART_INPUT_CallbackFct ),
			                     eTDAL_SCART_INPUT_NO_ERROR,
                                 "TDAL_SCART_INPUT_SubscribeEvent for scart SLB succeed");			


		TestManager_AssertEqual( TDAL_SCART_INPUT_SubscribeEvent(eId,eTDAL_SCART_INPUT_EVENT_FAST_BLANKING_CHANGE, SCART_INPUT_CallbackFct),
			                     eTDAL_SCART_INPUT_NO_ERROR,
                                 "TDAL_SCART_INPUT_SubscribeEvent for scart FB succeed");

		TestManager_AssertEqual( TDAL_SCART_INPUT_Start(eId),
			                     eTDAL_SCART_INPUT_NO_ERROR,
                                 "TDAL_SCART_INPUT_Start for scart succeed");
			
	}		



}



void TestHelper_AskForInput_ANA_INPUT( tTDAL_ANA_INPUT_Object * p_obj,
									   bool bVideo,
									   bool bAudio,
									   tTDAL_ANA_VID_Coding *p_coding,
									   tTDAL_ANA_INPUT_Object * p_muxobj)
{
	uint32_t               i = 0;
	uint32_t               input_index = 0;
	tTDAL_ANA_INPUT_Object object;
	tTDAL_ANA_VID_Coding   coding;
	tTDAL_ANA_INPUT_Capability stInputCapability;
    int IndexMux, IndexInput, IndexMuxFound;
    int nbMuxFound = 0;
	tTDAL_ANA_INPUT_MuxCapability stInputMuxCapability;
    char command = 'n';

	while(gAnalogInputList[i].mInputName != NULL)
	{
		if(((bVideo == TRUE) && ((gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_VIDEO_CVBS) ||
								(gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_VIDEO_YC) ||
								(gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_VIDEO_RGB_S) ||
								(gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_VIDEO_RGB_SOG) ||
								(gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_VIDEO_YPBPR) ||
								(gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_VIDEO_VGA))) ||
		   ((bAudio == TRUE) && (gAnalogInputList[i].eType == eANALOG_INPUT_TYPE_AUDIO)))
		{
			mTBOX_PRINT((kTBOX_LF,"Analog Input[%2d]-%s\n",
						 i,
						 gAnalogInputList[i].mInputName));
		}
		i++;
	};

	if ( i > 0 )
	{
		do
		{
			input_index = TestManager_GetNumber("Select an input (2 digits):", 2);
		}
		while(input_index > i);
	}

	mTBOX_PRINT((kTBOX_LF,"Analog Input[%2d]-%s chosen\n",
				 input_index,
				 gAnalogInputList[input_index].mInputName));
	
	switch(gAnalogInputList[input_index].eType)
	{
	case eANALOG_INPUT_TYPE_VIDEO_CVBS:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_VIDEO,eTDAL_ANA_INPUT_TYPE_CVBS, gAnalogInputList[input_index].Index);
		coding = eTDAL_ANA_VID_CODING_CVBS;		
		break;
	case eANALOG_INPUT_TYPE_VIDEO_YC:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_VIDEO,eTDAL_ANA_INPUT_TYPE_YC, gAnalogInputList[input_index].Index);
		coding = eTDAL_ANA_VID_CODING_YC;
		break;
	case eANALOG_INPUT_TYPE_VIDEO_RGB_S:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_VIDEO,eTDAL_ANA_INPUT_TYPE_COMP, gAnalogInputList[input_index].Index);
		coding = eTDAL_ANA_VID_CODING_RGB_S;
		break;
	case eANALOG_INPUT_TYPE_VIDEO_RGB_SOG:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_VIDEO,eTDAL_ANA_INPUT_TYPE_COMP, gAnalogInputList[input_index].Index);
		coding = eTDAL_ANA_VID_CODING_RGB_SOG;
		break;
	case eANALOG_INPUT_TYPE_VIDEO_YPBPR:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_VIDEO,eTDAL_ANA_INPUT_TYPE_COMP, gAnalogInputList[input_index].Index);
printf("object YPRPB %x \n",object);
		coding = eTDAL_ANA_VID_CODING_YPRPB;
		break;
	case eANALOG_INPUT_TYPE_VIDEO_VGA:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_VIDEO,eTDAL_ANA_INPUT_TYPE_COMP, gAnalogInputList[input_index].Index);
		coding = eTDAL_ANA_VID_CODING_RGB_HV;
		break;
	case eANALOG_INPUT_TYPE_AUDIO:
		object = mTDAL_ANA_INPUT_GetObjectId(eTDAL_ANA_INPUT_CLASS_AUDIO,eTDAL_ANA_INPUT_TYPE_LR, gAnalogInputList[input_index].Index);
		coding = eTDAL_ANA_VID_CODING_UNKNOWN;
		break;
	}
	mTBOX_PRINT((kTBOX_LF,"Analog Input[%2d]-%s -> object=0x%x\n",
				 input_index,
				 gAnalogInputList[input_index].mInputName,
				 object));

	*p_obj = object;
	
	if(p_coding != NULL) /* pointer available only for video input */
	{
		*p_coding = coding;
	}

	/* get mux index matching input object */
    /*-------------------------------------*/

	/* 1st- Get capability of the ana input module */
    TestManager_AssertEqual( TDAL_ANA_INPUT_CapabilityGet(&stInputCapability),
							 eTDAL_ANA_INPUT_NO_ERROR,
							 "TDAL_ANA_INPUT_CapabilityGet succeed" );
    mTBOX_PRINT((kTBOX_LF,"ANA_INPUT Capability : nbMux=%d\n",
				 stInputCapability.nbMux));

    if(stInputCapability.nbMux == 0)
    {
        mTBOX_PRINT((kTBOX_LF,"ANA_INPUT has no Mux => ERROR \n"));
        return;
    }

    for(IndexMux=0; IndexMux<stInputCapability.nbMux; IndexMux++)
    {
		/* Get capability of each ana input mux */
		TestManager_AssertEqual( TDAL_ANA_INPUT_MuxCapabilityGet(stInputCapability.MuxArray[IndexMux],
																 &stInputMuxCapability),
								 eTDAL_ANA_INPUT_NO_ERROR,
								 "TDAL_ANA_INPUT_MuxCapabilityGet succeed" );

		if(stInputMuxCapability.nbInput == 0)
		{
			mTBOX_PRINT((kTBOX_LF,"ANA_INPUT mux[%x]has no input => ERROR \n",
						 stInputCapability.MuxArray[IndexMux]));
			return;
		}
		
		mTBOX_PRINT((kTBOX_LF,"ANA_INPUT Capability : mux=%x [Class=%x,Type=%x,Instance=%x]\n",
					 stInputCapability.MuxArray[IndexMux],
                     mTDAL_ANA_INPUT_GetObjectClass(stInputCapability.MuxArray[IndexMux]),
                     mTDAL_ANA_INPUT_GetObjectType(stInputCapability.MuxArray[IndexMux]),
                     mTDAL_ANA_INPUT_GetObjectInstance(stInputCapability.MuxArray[IndexMux]) ));
		
		/* print mux capabilities */
		for(IndexInput=0; IndexInput<stInputMuxCapability.nbInput; IndexInput++)
		{
			mTBOX_PRINT((kTBOX_LF,"ANA_INPUT Capability : %d- object=%x [Class=%x,Type=%x,Instance=%x]\n",
						 IndexInput,
						 stInputMuxCapability.InputArray[IndexInput],
						 mTDAL_ANA_INPUT_GetObjectClass(stInputMuxCapability.InputArray[IndexInput]),
						 mTDAL_ANA_INPUT_GetObjectType(stInputMuxCapability.InputArray[IndexInput]),
						 mTDAL_ANA_INPUT_GetObjectInstance(stInputMuxCapability.InputArray[IndexInput]) ));

			if(stInputMuxCapability.InputArray[IndexInput] == object)
			{
				mTBOX_PRINT((kTBOX_LF,"ANA_INPUT Capability : input[%d-object=%x-Class=%x,Type=%x,Instance=%x] found on mux[%d-object=%x-Class=%x,Type=%x,Instance=%x]\n",
							 IndexInput,
							 stInputMuxCapability.InputArray[IndexInput],
							 mTDAL_ANA_INPUT_GetObjectClass(stInputMuxCapability.InputArray[IndexInput]),
							 mTDAL_ANA_INPUT_GetObjectType(stInputMuxCapability.InputArray[IndexInput]),
							 mTDAL_ANA_INPUT_GetObjectInstance(stInputMuxCapability.InputArray[IndexInput]),
							 IndexMux,
							 stInputCapability.MuxArray[IndexMux],
							 mTDAL_ANA_INPUT_GetObjectClass(stInputCapability.MuxArray[IndexMux]),
							 mTDAL_ANA_INPUT_GetObjectType(stInputCapability.MuxArray[IndexMux]),
							 mTDAL_ANA_INPUT_GetObjectInstance(stInputCapability.MuxArray[IndexMux])));
				nbMuxFound++;				
				
				if(nbMuxFound > 1)
				{
					mTBOX_PRINT((kTBOX_LF,"ANA_INPUT : input[object=%x-Class=%x,Type=%x,Instance=%x] found also on %d mux\n",
								 object,
								 mTDAL_ANA_INPUT_GetObjectClass(object),
								 mTDAL_ANA_INPUT_GetObjectType(object),
								 mTDAL_ANA_INPUT_GetObjectInstance(object),
								 nbMuxFound));
					/* when another mux match the input object, ask the user to tell to choose it or not */
					command = TestManager_GetChar( "Do you choose this mux ? [y/n] (n means keep the previous mux found)" );
					if((command == 'y') || (command == 'Y'))
					{
						IndexMuxFound = IndexMux; /* the index mux matching found is saved */
					}
				}
				else
				{
					IndexMuxFound = IndexMux; /* the 1st index mux matching found is saved */
				}
			}
		}
    }

	*p_muxobj = stInputCapability.MuxArray[IndexMuxFound];
}


void TestHelper_RegisterClient_VBI_SLICER(tTDAL_VBI_SLICER_ClientId *pstClientId, tTDAL_VBI_SLICER_Capability* pstDecoderCapability)
{
    /*register a client*/
    TestManager_AssertEqual( TDAL_VBI_SLICER_ClientRegister(pstClientId),
							 eTDAL_VBI_SLICER_NO_ERROR,
							 "TDAL_VBI_SLICER_ClientRegister succeed" );
    /*register the callback*/
    if(pstDecoderCapability->DataTypeSupportedBitmask & eTDAL_VBI_SLICER_DATA_VPS)
    {
        TestManager_AssertEqual( TDAL_VBI_SLICER_ClientRegisterVPSCallback(*pstClientId,
																		   VBI_SLICER_CallbackVPS),
								 eTDAL_VBI_SLICER_NO_ERROR,
								 "TDAL_VBI_SLICER_ClientRegisterVPSCallback succeed" );
    }
    if(pstDecoderCapability->DataTypeSupportedBitmask & eTDAL_VBI_SLICER_DATA_WST)
    {
        TestManager_AssertEqual(TDAL_VBI_SLICER_ClientRegisterWSTCallback(*pstClientId,
																		  VBI_SLICER_CallbackWST),
								eTDAL_VBI_SLICER_NO_ERROR,
								"TDAL_VBI_SLICER_ClientRegisterWSSCallback succeed" );
    }
    if(pstDecoderCapability->DataTypeSupportedBitmask & eTDAL_VBI_SLICER_DATA_WSS)
    {
        TestManager_AssertEqual( TDAL_VBI_SLICER_ClientRegisterWSSCallback(*pstClientId,
																		   VBI_SLICER_CallbackWSS),
								 eTDAL_VBI_SLICER_NO_ERROR,
								 "TDAL_VBI_SLICER_ClientRegisterWSSCallback succeed" );
    }
}

void TestHelper_Initialize_HDMI_IN_INPUT (tTDAL_HDMI_IN_SinkId *peId)
{
	tTDAL_HDMI_IN_Capability stInputCapability;
	uint8_t i;
	tTDAL_HDMI_IN_SinkId eId;
	int input_index;
	
    TestManager_AssertEqual( TDAL_HDMI_IN_Init(), eTDAL_HDMI_IN_NO_ERROR, "TDAL_HDMI_IN_Init succeed" );

    /* Get capability of the vbi slicer*/
    TestManager_AssertEqual( TDAL_HDMI_IN_CapabilityGet(&stInputCapability),
							 eTDAL_HDMI_IN_NO_ERROR,
							 "TDAL_HDMI_IN_CapabilityGet succeed" );

    mTBOX_PRINT((kTBOX_LF,"HDMI_INPUT Capability : [nbObject=%d], gamut %d general control %d \n",
				 stInputCapability.ucNbSink,
				 stInputCapability.bGamutMetadataCapable,
				 stInputCapability.bGeneralControlPacketCapable));

    if((stInputCapability.ucNbSink == 0))
    {
        mTBOX_PRINT((kTBOX_LF,"HDMI_IN_INPUT has no Input => ERROR \n"));
        return;
    }

	for (i = 0; i < stInputCapability.ucNbSink; i++)
	{
		mTBOX_PRINT((kTBOX_LF,"%d : HDMI Input[%2d]\n",i,i + 1));
	}

	if ( i > 0 )
	{
		do
		{
			input_index = TestManager_GetNumber("Select an HDMI input (1 digits):", 1);
		}
		while(input_index > i);
	}

	eId = input_index;

	mTBOX_PRINT((kTBOX_LF,"HDMI Input[%2d]- chosen\n",eId ));

	*peId = eId;

}




void TestHelper_UnRegisterClient_VBI_SLICER(tTDAL_VBI_SLICER_ClientId *pstClientId, tTDAL_VBI_SLICER_Capability* pstDecoderCapability)
{
    /*unregister a client*/
    /*unregister the callback*/
    if(pstDecoderCapability->DataTypeSupportedBitmask & eTDAL_VBI_SLICER_DATA_VPS)
    {
        TestManager_AssertEqual( TDAL_VBI_SLICER_ClientUnregisterVPSCallback(*pstClientId), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_ClientRegisterVPSCallback succeed" );
    }
    if(pstDecoderCapability->DataTypeSupportedBitmask & eTDAL_VBI_SLICER_DATA_WST)
    {
        TestManager_AssertEqual( TDAL_VBI_SLICER_ClientUnregisterWSTCallback(*pstClientId), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_ClientUnRegisterWSTCallback succeed" );
    }
    if(pstDecoderCapability->DataTypeSupportedBitmask & eTDAL_VBI_SLICER_DATA_WSS)
    {
        TestManager_AssertEqual( TDAL_VBI_SLICER_ClientUnregisterWSSCallback(*pstClientId), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_ClientUnRegisterWSSCallback succeed" );
    }

    TestManager_AssertEqual( TDAL_VBI_SLICER_ClientUnregister(*pstClientId), eTDAL_VBI_SLICER_NO_ERROR, "TDAL_VBI_SLICER_ClientUnRegister succeed" );

}

void TestHelper_Terminate_ANA_AUD(tTDAL_ANA_AUD_Decoder IndexDecoder)
{
    tTDAL_ANA_AUD_Event             eIndexEvent;

    /* Un Register Event */
    for(eIndexEvent=eTDAL_ANA_AUD_EVENT_SIGNAL_CHANGED; eIndexEvent<=eTDAL_ANA_AUD_EVENT_SIGNAL_SWITCH; eIndexEvent++)
    {
        TestManager_AssertEqual( TDAL_ANA_AUD_EventUnsubscribe(IndexDecoder, eIndexEvent ), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_EventUnsubscribe succeed" );
    }

    TestManager_AssertEqual( TDAL_ANA_AUD_Term(), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_AUD_Term" );
}

void TestHelper_Terminate_ANA_VID(tTDAL_ANA_VID_Decoder IndexDecoder)
{
    tTDAL_ANA_VID_Event             eIndexEvent;

    /* Un Register Event */
    for(eIndexEvent=eTDAL_ANA_VID_EVENT_TYPE_CHANGE; eIndexEvent<=eTDAL_ANA_VID_EVENT_NEED_DISPLAY; eIndexEvent++)
    {
        TestManager_AssertEqual( TDAL_ANA_VID_EventUnsubscribe(IndexDecoder, eIndexEvent ), eTDAL_ANA_VID_NO_ERROR, "TDAL_ANA_AUD_EventUnsubscribe succeed" );
    }

    TestManager_AssertEqual( TDAL_ANA_VID_Term(), eTDAL_ANA_AUD_NO_ERROR, "TDAL_ANA_VID_Term" );

}


void TestHelper_CheckPanelSettings(void)
{
	uint8_t brightness;
	uint8_t contrast;
	uint8_t sharpness;
	uint8_t saturation;
	uint8_t backlight;
	bool  bBackLightControl;

    TestManager_AssertEqual( TDAL_PANEL_GetBrightness(&brightness),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_GetBrightness  succeed" );


    TestManager_AssertEqual( TDAL_PANEL_GetContrast(&contrast),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_GetContrast  succeed" );

    TestManager_AssertEqual( TDAL_PANEL_GetSaturation(&saturation),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_GetSaturation  succeed" );


    TestManager_AssertEqual( TDAL_PANEL_GetSharpness(&sharpness),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_GetSharpness  succeed" );

    TestManager_AssertEqual( TDAL_PANEL_GetControlBacklight(&bBackLightControl,&backlight),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_GetControlBacklight  succeed" );


	mTBOX_PRINT((kTBOX_LF,"brightness %d contrast %d saturation %d sharpness %d backLight %d \n",
				brightness,contrast,saturation,sharpness,backlight));


	/* check brightness */

    TestManager_AssertEqual( TDAL_PANEL_SetBrightness(90),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetBrightness 90% succeed" );

    TestManager_AskForChecking( "do you see change on brightness ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetBrightness(10),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetBrightness 10% succeed" );

    TestManager_AskForChecking( "do you see change on brightness ?" );

    TestManager_AssertEqual( TDAL_PANEL_SetBrightness(brightness),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetBrightness   succeed" );

    TestManager_AskForChecking( "do you see change on brightness ?" );



    TestManager_AssertEqual( TDAL_PANEL_SetContrast(90),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetContrast 90% succeed" );

    TestManager_AskForChecking( "do you see change on contrast ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetContrast(10),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetBrightness 10% succeed" );

    TestManager_AskForChecking( "do you see change on contrast ?" );

    TestManager_AssertEqual( TDAL_PANEL_SetContrast(contrast),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetBrightness  succeed" );

    TestManager_AskForChecking( "do you see change on contrast ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetSaturation(90),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetSaturation 90% succeed" );

    TestManager_AskForChecking( "do you see change on saturation ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetSaturation(10),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetSaturation 10% succeed" );

    TestManager_AskForChecking( "do you see change on saturation ?" );

    TestManager_AssertEqual( TDAL_PANEL_SetSaturation(saturation),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetSaturation  succeed" );

    TestManager_AskForChecking( "do you see change on saturation ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetSharpness(90),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetSharpness 90% succeed" );

    TestManager_AskForChecking( "do you see change on sharpness ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetSharpness(10),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetSharpness 10% succeed" );

    TestManager_AskForChecking( "do you see change on sharpness ?" );

    TestManager_AssertEqual( TDAL_PANEL_SetSharpness(sharpness),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetSharpness  succeed" );

    TestManager_AskForChecking( "do you see change on sharpness ?" );

    TestManager_AssertEqual( TDAL_PANEL_SetControlBacklight(TRUE,90),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetControlBacklight 90% succeed" );

    TestManager_AskForChecking( "do you see change on backlight ?" );


    TestManager_AssertEqual( TDAL_PANEL_SetControlBacklight(TRUE,10),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetControlBacklight 10% succeed" );

    TestManager_AskForChecking( "do you see change on backlight ?" );

    TestManager_AssertEqual( TDAL_PANEL_SetControlBacklight(TRUE,backlight),
							     eTDAL_PANEL_NO_ERROR,
							     "TDAL_PANEL_SetControlBacklight  succeed" );

    TestManager_AskForChecking( "do you see change on backlight ?" );



}
	
void TestHelper_CheckSpeakersSettings(void)
{
	uint8_t freqArray[kTDAL_SPEAKERS_MAX_EQUALIZER_FREQUENCY];
	uint8_t value;
    TestManager_AssertEqual( TDAL_SPEAKERS_GetEqualizer(eTDAL_SPEAKERS_FPD_ID_0,kTDAL_SPEAKERS_MAX_EQUALIZER_FREQUENCY,(uint8_t *) freqArray),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_PANEL_GetBrightness  succeed" );

	value = freqArray[0];
	freqArray[0] = 10;
	
    TestManager_AssertEqual( TDAL_SPEAKERS_SetEqualizer(eTDAL_SPEAKERS_FPD_ID_0,freqArray),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetEqualizer  succeed" );

    TestManager_AskForChecking( "do you hear change on first frequency 10% ?" );
	freqArray[0] = 90;

    TestManager_AssertEqual( TDAL_SPEAKERS_SetEqualizer(eTDAL_SPEAKERS_FPD_ID_0,freqArray),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetEqualizer  succeed" );

    TestManager_AskForChecking( "do you hear change on first frequency 90% ?" );
	freqArray[0] = value;
	value = freqArray[1];
	freqArray[1] = 10;
	
    TestManager_AssertEqual( TDAL_SPEAKERS_SetEqualizer(eTDAL_SPEAKERS_FPD_ID_0,freqArray),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetEqualizer  succeed" );

    TestManager_AskForChecking( "do you hear change on 2nd frequency 10% ?" );

	freqArray[1] = 90;

    TestManager_AssertEqual( TDAL_SPEAKERS_SetEqualizer(eTDAL_SPEAKERS_FPD_ID_0,freqArray),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetEqualizer  succeed" );

    TestManager_AskForChecking( "do you hear change on 2nd frequency 90% ?" );

	freqArray[1] = value;

    TestManager_AssertEqual( TDAL_SPEAKERS_SetEqualizer(eTDAL_SPEAKERS_FPD_ID_0,freqArray),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetEqualizer  succeed" );

    TestManager_AskForChecking( "do you hear change on 2nd frequency init state ?" );

    TestManager_AssertEqual( TDAL_SPEAKERS_GetBalance(eTDAL_SPEAKERS_FPD_ID_0,&value),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetBalance  succeed" );

	mTBOX_PRINT((kTBOX_LF,"current balance get %d \n",	value));

    TestManager_AssertEqual( TDAL_SPEAKERS_SetBalance(eTDAL_SPEAKERS_FPD_ID_0,20),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetBalance  succeed" );

    TestManager_AskForChecking( "do you hear sound on the left ?" );


    TestManager_AssertEqual( TDAL_SPEAKERS_SetBalance(eTDAL_SPEAKERS_FPD_ID_0,80),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetBalance  succeed" );

    TestManager_AskForChecking( "do you hear sound on the right ?" );


    TestManager_AssertEqual( TDAL_SPEAKERS_SetBalance(eTDAL_SPEAKERS_FPD_ID_0,50),
							     eTDAL_SPEAKERS_NO_ERROR,
							     "TDAL_SPEAKERS_SetBalance  succeed" );

    TestManager_AskForChecking( "do you hear sound on the middle ?" );


	
}


void TestHelper_CheckPCSettings(void)
{
	tTDAL_ANA_VID_PC_FormatInformation stPCInformation;
	uint16_t clockValue,phaseValue;
	tTDAL_ANA_VID_Decoder eDecoder = eTDAL_ANA_VID_DECODER_1;

	TestManager_AssertEqual( TDAL_ANA_VID_GetPcFormatInfo(eDecoder,
														  &stPCInformation),
							     eTDAL_ANA_VID_NO_ERROR,
							     "TDAL_ANA_VID_GetPcFormatInfo  succeed" );

	mTBOX_PRINT((kTBOX_LF,"phase %d clock %d \n",
				stPCInformation.usPhase,stPCInformation.usPixelClock));

	clockValue = stPCInformation.usPixelClock;
 	phaseValue = stPCInformation.usPhase;

	stPCInformation.usPixelClock = 10;
	
	TestManager_AssertEqual( TDAL_ANA_VID_SetPcFormatInfo(eDecoder,
														  &stPCInformation),
							     eTDAL_ANA_VID_NO_ERROR,
							     "TDAL_ANA_VID_SetPcFormatInfo  succeed" );

    TestManager_AskForChecking( "do you see change on pixel clock ?" );

	stPCInformation.usPixelClock = 90;
	
	TestManager_AssertEqual( TDAL_ANA_VID_SetPcFormatInfo(eDecoder,
														  &stPCInformation),
							     eTDAL_ANA_VID_NO_ERROR,
							     "TDAL_ANA_VID_SetPcFormatInfo  succeed" );

    TestManager_AskForChecking( "do you see change on pixel clock ?" );

	stPCInformation.usPixelClock = clockValue;
	stPCInformation.usPhase = 10;

	TestManager_AssertEqual( TDAL_ANA_VID_SetPcFormatInfo(eDecoder,
														  &stPCInformation),
							     eTDAL_ANA_VID_NO_ERROR,
							     "TDAL_ANA_VID_SetPcFormatInfo  succeed" );

    TestManager_AskForChecking( "do you see change on phase  ?" );

	stPCInformation.usPhase = 90;

	TestManager_AssertEqual( TDAL_ANA_VID_SetPcFormatInfo(eDecoder,
														  &stPCInformation),
							     eTDAL_ANA_VID_NO_ERROR,
							     "TDAL_ANA_VID_SetPcFormatInfo  succeed" );

    TestManager_AskForChecking( "do you see change on phase  ?" );

	stPCInformation.usPhase = phaseValue;

	TestManager_AssertEqual( TDAL_ANA_VID_SetPcFormatInfo(eDecoder,
														  &stPCInformation),
							     eTDAL_ANA_VID_NO_ERROR,
							     "TDAL_ANA_VID_SetPcFormatInfo  succeed" );

    TestManager_AskForChecking( "do you see change on phase  ?" );



}

void TestHelper_Configure_TDAL_ConnectionManager(tTestManager_ConnectionManagerType type)
{
	tTDAL_UseCaseInfo stUseCase;
	
	if(type == eCONNECTION_MANAGER_TYPE_DIGITAL)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_DIG_TUNER;
		stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_DIG_TUNER;
	}
	else if(type == eCONNECTION_MANAGER_TYPE_ANALOG_TUNER)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_TUNER;
		stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_TUNER;
	}
	else if(type == eCONNECTION_MANAGER_TYPE_ANALOG_SCART)
	{
    	stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_SCART;
	}
	else if(type == eCONNECTION_MANAGER_TYPE_ANALOG_YPRPB)
	{
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_ANA_YPrPb;		
	}
	else if(type == eCONNECTION_MANAGER_TYPE_ANALOG_PC)
	{
		/* PC mode */
		stUseCase.eAudioInput       = eTDAL_USE_CASE_ANA_SCART;
	    stUseCase.eFirstVideoInput  = eTDAL_USE_CASE_PC;		
	}
	
	stUseCase.eSecondVideoInput = eTDAL_USE_CASE_NONE;
	stUseCase.eUseCaseWindow    = eTDAL_USE_CASE_WINDOW_SINGLE;
	
	TestManager_AssertEqual(TDAL_ConnectionManager_SelectUseCase(&stUseCase),
							TRUE,
							"ConnectionManager selectUseCase");
}

#endif /*#ifndef CIPLUSVALIDATOR*/
