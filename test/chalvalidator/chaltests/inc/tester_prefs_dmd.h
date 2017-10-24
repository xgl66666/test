#ifndef TESTER_PREFS_DMD_H
#define TESTER_PREFS_DMD_H

/** \file tester_prefs_dmd.h
 *  \brief Header file containing preferences for DMD
 *
 *  This file contains preferences for DMD. It notably defines
 *  a list of available channels.
 *
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

 #include "tdal_dmd.h"
 
/*! \struct t_testTune tester_prefs_dmd.h
 *  \brief this structure is used to set signal attributes
 *  \see TuneFakeChan and TuneChannel
 */
struct t_testTune {
    /*!  name of the channel */
    char *name;
    /*! frequency of the channel (Satellite in KHz else in Hz) */
    unsigned int frequency;
    /*! signal symbol rate in baud */
    unsigned int symbolRate;
    /*! signal convolution */
    tTDAL_DMD_Convolution convolution;
    /*! signal polarization */
    tTDAL_DMD_LNB_Polarization polarization;
    /*! signal modulation */
    tTDAL_DMD_Modulation modulation;
};

#if defined(MEDIUM_SAT)
#define NUMBER_CHANNEL  6
#elif defined(MEDIUM_TER)
#define NUMBER_CHANNEL  6
#elif defined(MEDIUM_CAB)
#define NUMBER_CHANNEL  1
#define CAB_TUNER_FREQ 466000000
#define CAB_TUNER_SYMB 6875000
#endif

/* DMD settings */

/*! \var t_testTune TuneChannel[NUMBER_CHANNEL]
 *  \brief Variable which stores channels valid and signal features
 *
 *  This variable is used to perform successive TDAL_DMD_Tune() on
 *  different channels.
 *
 *  \see TestPerf_Tune(), TestStress_GetInfoTune(), TestRel_LoopTune()
 *  and TestSce_TuneAll()
 */

static const struct t_testTune TuneChannel[NUMBER_CHANNEL] =
{
#if defined(MEDIUM_SAT)
	{"EinsExtra",       586000000, 0, eTDAL_DMD_FEC_AUTO, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
	   {"TP3 ASTRA",	12460000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
	   {"TP4 HOTBIRD",	  12380000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_VERTICAL, eTDAL_DMD_MOD_QPSK}, //OK
	   {"TP5 HOTBIRD",	  12597000, 27500000, eTDAL_DMD_FEC_5_6, eTDAL_DMD_POLAR_VERTICAL, eTDAL_DMD_MOD_QPSK}, //OK
	   {"TP7 HOTBIRD",	  3, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
	   {"TP9 ASTRA",   12460000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK

#elif defined(MEDIUM_TER)
    {"EinsExtra",   586000000, 22000000, eTDAL_DMD_FEC_5_6, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
    {"TP3 ASTRA",    12460000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
    {"TP4 HOTBIRD",    12380000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_VERTICAL, eTDAL_DMD_MOD_QPSK}, //OK
    {"TP5 HOTBIRD",    12597000, 27500000, eTDAL_DMD_FEC_5_6, eTDAL_DMD_POLAR_VERTICAL, eTDAL_DMD_MOD_QPSK}, //OK
    {"TP7 HOTBIRD",    1, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
    {"TP9 ASTRA",   12460000, 27500000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QPSK}, //OK
#elif defined(MEDIUM_CAB)
    {"player", 818000000, 6875000, eTDAL_DMD_FEC_3_4, eTDAL_DMD_POLAR_HORIZONTAL, eTDAL_DMD_MOD_QAM64},
#endif
};

#if defined(MEDIUM_SAT)

#define SWITCH_FREQ     22000
#define LOW_LNB_FREQ    9750
#define HIGH_LNB_FREQ   10600

/* Settings required by DMD to lock */
/*! \def CHANNEL_1
 *  \brief Set default channel as TuneChannel[0]
 *  \see TuneChannel
 */
#define CHANNEL_1       TuneChannel[0].name /* euroVsport */
#define FREQUENCY       TuneChannel[0].frequency
#define SYM_RATE        TuneChannel[0].symbolRate
#define POLARIZATION    TuneChannel[0].polarization
#define CONVOLUTION     TuneChannel[0].convolution
#define MODULATION      TuneChannel[0].modulation

#define CHANNEL_2       "rtl"

/*! \def CHANNEL_FAKE
 *  \brief Channel with improper signal features
 *  \see TuneFakeChan
 */
#define CHANNEL_FAKE    TuneFakeChan[0].name

#elif defined(MEDIUM_TER)

#define BANDWIDTH_1     6000000
#define BANDWIDTH_2     7000000
#define BANDWIDTH_3     8000000

#define SWITCH_FREQ     22000
#define LOW_LNB_FREQ    9750
#define HIGH_LNB_FREQ   10600

/* Settings required by DMD to lock */
#define CHANNEL_1       TuneChannel[0].name /* tf1 */
#define FREQUENCY       TuneChannel[0].frequency
#define SYM_RATE        TuneChannel[0].symbolRate
#define POLARIZATION    TuneChannel[0].polarization
#define CONVOLUTION     TuneChannel[0].convolution
#define MODULATION      TuneChannel[0].modulation

/* Settings required by DMD to lock */
#define CHANNEL_2		TuneChannel[1].name /* france 2 */
#define FREQUENCY2       TuneChannel[1].frequency
#define SYM_RATE2        TuneChannel[1].symbolRate
#define POLARIZATION2    TuneChannel[1].polarization
#define CONVOLUTION2     TuneChannel[1].convolution
#define MODULATION2      TuneChannel[1].modulation
/*#define CHANNEL_2       "canal+"*/

#define CHANNEL_FAKE    TuneFakeChan[0].name

#elif defined(MEDIUM_CAB)
#define CHANNEL_1       TuneChannel[0].name /* player */
#define FREQUENCY       TuneChannel[0].frequency
#define SYM_RATE        TuneChannel[0].symbolRate
#define POLARIZATION    TuneChannel[0].polarization
#define CONVOLUTION     TuneChannel[0].convolution
#define MODULATION      TuneChannel[0].modulation

#define CHANNEL_2       "player_2"

#define CHANNEL_FAKE    TuneFakeChan[0].name

#endif /* MEDIUM SAT/TER/CAB */

#endif // TESTER_PREFS_DMD_H
