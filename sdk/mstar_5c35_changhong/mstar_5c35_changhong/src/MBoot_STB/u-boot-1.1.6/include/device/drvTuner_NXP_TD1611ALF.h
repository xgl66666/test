////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _DRVTUNER_NXP_TD1611ALF_H_
#define _DRVTUNER_NXP_TD1611ALF_H_

#define INTERFACE extern

#define TN_FREQ_STEP                FREQ_STEP_62_5KHz//FREQ_STEP_62_5KHz//FREQ_STEP_50KHz//FREQ_STEP_31_25KHz

#define _TUNER_I2C_ID        0xC2

#define VHF_LOWMIN_FREQ        48250L    // KHz
#define VHF_LOWMAX_FREQ        158000L    // KHz
#define VHF_HIGHMIN_FREQ    160000L    // KHz
#define VHF_HIGHMAX_FREQ    442000L    // KHz
#define UHF_MIN_FREQ_UK        470000L//470000L    // KHz for UK
#define UHF_MIN_FREQ        442000L    // KHz
#define UHF_MAX_FREQ        863250L    // KHz

#define TUNER_IF_FREQ_KHz            38000L    // KHz
#define TUNER_L_PRIME_IF_FREQ_KHz    33950L    // KHz


#define TN_FREQ_SS_INVERSE_625          16   // 1/0.0625
#define TN_RSA_RSB_625                  0x06
#define TN_FREQ_SS_625                  62.5  // kHz

#define TN_FREQ_SS_50                 50  // kHz
#define TN_FREQ_SS_INVERSE_50         20   // 1/0.05
#define TN_RSA_RSB_50                  0


#define TN_LOW_BAND     0x51
#define TN_MID_BAND     0x52
#define TN_HIGH_BAND    0x54
#define TN_IF_I2C_ADDR      0x86
#define TN_PROG_I2C_ADDR    0xC2
//****************************************************************************
// Public functions.
//****************************************************************************

#define IF_OUT_LEVEL_112        (0<<3)
#define IF_OUT_LEVEL_109        (1<<3)
#define IF_OUT_LEVEL_106        (2<<3)
#define IF_OUT_LEVEL_103        (3<<3)
#define IF_OUT_LEVEL_100        (4<<3)
#define IF_OUT_LEVEL_97         (5<<3)
#define IF_OUT_LEVEL_94         (6<<3)
#define IF_OUT_LEVEL_Disable    (7<<3)


#define CHARGE_PUMP_70uA          0
#define CHARGE_PUMP_140uA         0x40
#define CHARGE_PUMP_350uA         0x80
#define CHARGE_PUMP_600uA         0xC0

#define P5_ADC_INPUT    (0<<4)
#define P5_TR_ON        (1<<4)

#define BAND_SWITCH_UHF           0x08
#define BAND_SWITCH_VHF_HIGH      0x02
#define BAND_SWITCH_VHF_LOW       0x01


INTERFACE void devTunerInit(void);
INTERFACE void devTunerSetFreqStep(FREQSTEP eFreqStep);
INTERFACE void devTunerSetPLLData(MS_U16 wPLLData, RFBAND eBand);

#undef INTERFACE
#endif
