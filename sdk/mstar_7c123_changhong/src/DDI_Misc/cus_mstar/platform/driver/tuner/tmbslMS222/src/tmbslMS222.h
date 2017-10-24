/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslMS222.h
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                MS222_Driver_User_Guide.pdf
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_MS222_H
#define _TMBSL_MS222_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* MS222 Error Codes                                                       */
/*============================================================================*/

#define MS222_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
#define MS222_ERR_COMP                       (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define MS222_ERR_BAD_UNIT_NUMBER            (MS222_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define MS222_ERR_ERR_NO_INSTANCES           (MS222_ERR_BASE + TM_ERR_NO_INSTANCES)
#define MS222_ERR_NOT_INITIALIZED            (MS222_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define MS222_ERR_ALREADY_SETUP              (MS222_ERR_BASE + TM_ERR_ALREADY_SETUP)
#define MS222_ERR_INIT_FAILED                (MS222_ERR_BASE + TM_ERR_INIT_FAILED)
#define MS222_ERR_BAD_PARAMETER              (MS222_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define MS222_ERR_NOT_SUPPORTED              (MS222_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define MS222_ERR_NULL_CONTROLFUNC           (MS222_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
#define MS222_ERR_HW_FAILED                  (MS222_ERR_COMP + 0x0001)
#define MS222_ERR_NOT_READY                  (MS222_ERR_COMP + 0x0002)
#define MS222_ERR_BAD_VERSION                (MS222_ERR_COMP + 0x0003)
#define MS222_ERR_STD_NOT_SET                (MS222_ERR_COMP + 0x0004)
#define MS222_ERR_RF_NOT_SET                 (MS222_ERR_COMP + 0x0005)

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/
#define MS222_DVBT_SUPPORTED

//#define MS222_CONFIG_STD_FREQ_NUM 2
#define MS222_CONFIG_STD_FREQ_NUM 8

typedef enum _MS222Version_t {
    MS222_VersionES1 = 0,                                       /* ES1 */
    MS222_VersionES2,                                           /* ES2 */
    MS222_VersionES3,                                           /* ES3 */
    MS222_VersionES4,                                           /* ES4 */
    MS222_VersionMax
} MS222Version_t, *pMS222Version_t;

typedef struct _MS222_AdditionnalData_t
{
    tmUnitSelect_t tOtherUnit; /* tUnit of the instance containing the tUnit of the other path */
}MS222_AdditionnalData_t;

typedef enum _MS222PowerState_t {
    MS222_PowerNormalMode = 0,                                 /* Device normal mode */
    MS222_PowerStandbyWithXtalOn,                              /* Device standby mode with Xtal Output */
    MS222_PowerStandbyWithLtOnWithXtalOn,                      /* Device standby mode with LT on and with Xtal Output */
    MS222_PowerStandbyWithPllOnWithXtalOn,                     /* Device standby mode with PLL on and with Xtal Output */
    MS222_PowerStandbyWithLtOnWithPllOnWithXtalOn,             /* Device standby mode with LT on with PLL on and with Xtal Output */
    MS222_PowerStandbySmoothLTOnXtOutOn,
    MS222_PowerStandbySmoothLTOffXtOutOff,
    MS222_PowerStandbySmoothLTOffLNAOnXtOutOn,
    MS222_PowerMax
} MS222PowerState_t, *pMS222PowerState_t;

typedef enum  _MS222ModeInUse_t {
	MS222_Mode_Single = 0,
	MS222_Mode_Dual = 1,
	MS222_ModeMax = 3
} MS222ModeInUse_t, *pMS222ModeInUse_t;

typedef enum _MS222StandardMode_t {
    MS222_StandardMode_Unknown = 0,                  /* Unknown standard */
    MS222_DVBT_1_7MHz,                               /* Digital TV DVB-T/T2 6MHz */
    MS222_DVBT_6MHz,                                 /* Digital TV DVB-T/T2 6MHz */
    MS222_DVBT_7MHz,                                 /* Digital TV DVB-T/T2 7MHz */
    MS222_DVBT_8MHz,                                 /* Digital TV DVB-T/T2 8MHz */
    MS222_DVBT_10MHz,                                /* Digital TV DVB-T/T2 10MHz */
    MS222_StandardMode_Max
} MS222StandardMode_t, *pMS222StandardMode_t;

/* Register Bit-Field Definition */
typedef struct _MS222_BitField_t
{
    UInt8   Address;
    UInt8   PositionInBits;
    UInt8   WidthInBits;
    UInt8   Attributes;
}
MS222_BitField_t, *pMS222_BitField_t;

typedef enum _MS222XtalFreq_t {
	MS222_XtalFreq_Unknown,
	MS222_XtalFreq_16000000,
	MS222_XtalFreq_24000000,
	MS222_XtalFreq_25000000,
	MS222_XtalFreq_27000000,
	MS222_XtalFreq_30000000
} MS222XtalFreq_t, *pMS222XtalFreq_t;

typedef enum _MS222IF_Output_Level_t {
    MS222_IF_Output_Level_2Vpp_0_30dB = 0,           /* 2Vpp       0 - 30dB      */
    MS222_IF_Output_Level_1_25Vpp_min_4_26dB,        /* 1.25Vpp   -4 - 26dB      */
    MS222_IF_Output_Level_1Vpp_min_6_24dB,           /* 1Vpp      -6 - 24dB      */
    MS222_IF_Output_Level_0_8Vpp_min_8_22dB,         /* 0.8Vpp    -8 - 22dB      */
    MS222_IF_Output_Level_0_85Vpp_min_7_5_22_5dB,    /* 0.85Vpp   -7.5 - 22.5dB  */
    MS222_IF_Output_Level_0_7Vpp_min_9_21dB,         /* 0.7Vpp    -9 - 21dB      */
    MS222_IF_Output_Level_0_6Vpp_min_10_3_19_7dB,    /* 0.6Vpp    -10.3 - 19.7dB */
    MS222_IF_Output_Level_0_5Vpp_min_12_18dB,        /* 0.5Vpp    -12 - 18dB     */
    MS222_IF_Output_Level_Max
} MS222IF_Output_Level_t, *pMS222IF_Output_Level_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslMS222_Open(
    tmUnitSelect_t              tUnit,      /*  I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
);

tmErrorCode_t
tmbslMS222_HwInit(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

tmErrorCode_t
tmbslMS222_SetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t  powerState  /* I: Power state */
);

tmErrorCode_t
tmbslMS222_SetStandardMode(
    tmUnitSelect_t              tUnit,          /*  I: Unit number */
    MS222StandardMode_t    StandardMode    /*  I: Standard mode of this device */
);

tmErrorCode_t
tmbslMS222_SetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32          uRF     /*  I: RF frequency in hertz */
);

tmErrorCode_t
tmbslMS222_Close(
    tmUnitSelect_t  tUnit   /*  I: Unit number */
);

tmErrorCode_t
tmbslMS222_GetIF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
);

tmErrorCode_t
tmbslMS222_SetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    MS222IF_Output_Level_t  eIFLevel    /* I: IF Level */
);

tmErrorCode_t
tmbslMS222_GetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    MS222IF_Output_Level_t  *peIFLevel  /* I: IF Level */
);

tmErrorCode_t
tmbslMS222_GetPowerLevel(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          pPowerLevel /* O: Power Level in (x100) dBµVrms */
);

tmErrorCode_t
tmbslMS222_CheckHWVersion(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    pMS222Version_t pEsVersion  /*  o: ES version of the HW */
);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_MS222_H */

