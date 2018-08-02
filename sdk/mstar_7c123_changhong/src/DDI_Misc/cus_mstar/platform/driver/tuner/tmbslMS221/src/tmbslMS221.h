/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslMS221.h
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                MS221_Driver_User_Guide.pdf
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_MS221_H
#define _TMBSL_MS221_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* MS221 Error Codes                                                       */
/*============================================================================*/

#define MS221_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
#define MS221_ERR_COMP                       (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define MS221_ERR_BAD_UNIT_NUMBER            (MS221_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define MS221_ERR_ERR_NO_INSTANCES           (MS221_ERR_BASE + TM_ERR_NO_INSTANCES)
#define MS221_ERR_NOT_INITIALIZED            (MS221_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define MS221_ERR_ALREADY_SETUP              (MS221_ERR_BASE + TM_ERR_ALREADY_SETUP)
#define MS221_ERR_INIT_FAILED                (MS221_ERR_BASE + TM_ERR_INIT_FAILED)
#define MS221_ERR_BAD_PARAMETER              (MS221_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define MS221_ERR_NOT_SUPPORTED              (MS221_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define MS221_ERR_NULL_CONTROLFUNC           (MS221_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
#define MS221_ERR_HW_FAILED                  (MS221_ERR_COMP + 0x0001)
#define MS221_ERR_NOT_READY                  (MS221_ERR_COMP + 0x0002)
#define MS221_ERR_BAD_VERSION                (MS221_ERR_COMP + 0x0003)
#define MS221_ERR_STD_NOT_SET                (MS221_ERR_COMP + 0x0004)
#define MS221_ERR_RF_NOT_SET                 (MS221_ERR_COMP + 0x0005)

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/
#define MS221_DVBC_SUPPORTED

#define MS221_CONFIG_STD_FREQ_NUM 8

typedef enum _MS221Version_t {
    MS221_VersionES1 = 0,                                       /* ES1 */
    MS221_VersionES2,                                           /* ES2 */
    MS221_VersionES3,                                           /* ES3 */
    MS221_VersionES4,                                           /* ES4 */
    MS221_VersionMax
} MS221Version_t, *pMS221Version_t;

typedef struct _MS221_AdditionnalData_t
{
    tmUnitSelect_t tOtherUnit; /* tUnit of the instance containing the tUnit of the other path */
}MS221_AdditionnalData_t;

typedef enum _MS221PowerState_t {
    MS221_PowerNormalMode = 0,                                 /* Device normal mode */
    MS221_PowerStandbyWithXtalOn,                              /* Device standby mode with Xtal Output */
    MS221_PowerStandbyWithLtOnWithXtalOn,                      /* Device standby mode with LT on and with Xtal Output */
    MS221_PowerStandbyWithPllOnWithXtalOn,                     /* Device standby mode with PLL on and with Xtal Output */
    MS221_PowerStandbyWithLtOnWithPllOnWithXtalOn,             /* Device standby mode with LT on with PLL on and with Xtal Output */
    MS221_PowerStandbySmoothLTOnXtOutOn,
    MS221_PowerStandbySmoothLTOffXtOutOff,
    MS221_PowerStandbySmoothLTOffLNAOnXtOutOn,
    MS221_PowerMax
} MS221PowerState_t, *pMS221PowerState_t;

typedef enum  _MS221ModeInUse_t {
	MS221_Mode_Single = 0,
	MS221_Mode_Dual = 1,
	MS221_ModeMax = 3
} MS221ModeInUse_t, *pMS221ModeInUse_t;

typedef enum _MS221StandardMode_t {
    MS221_StandardMode_Unknown = 0,                  /* Unknown standard */
    MS221_QAM_6MHz,                                  /* Digital TV QAM 6MHz */
    MS221_QAM_8MHz,                                  /* Digital TV QAM 8MHz */
    MS221_StandardMode_Max
} MS221StandardMode_t, *pMS221StandardMode_t;

/* Register Bit-Field Definition */
typedef struct _MS221_BitField_t
{
    UInt8   Address;
    UInt8   PositionInBits;
    UInt8   WidthInBits;
    UInt8   Attributes;
}
MS221_BitField_t, *pMS221_BitField_t;

typedef enum _MS221XtalFreq_t {
	MS221_XtalFreq_Unknown,
	MS221_XtalFreq_16000000,
	MS221_XtalFreq_24000000,
	MS221_XtalFreq_25000000,
	MS221_XtalFreq_27000000,
	MS221_XtalFreq_30000000
} MS221XtalFreq_t, *pMS221XtalFreq_t;

typedef enum _MS221IF_Output_Level_t {
    MS221_IF_Output_Level_2Vpp_0_30dB = 0,           /* 2Vpp       0 - 30dB      */
    MS221_IF_Output_Level_1_25Vpp_min_4_26dB,        /* 1.25Vpp   -4 - 26dB      */
    MS221_IF_Output_Level_1Vpp_min_6_24dB,           /* 1Vpp      -6 - 24dB      */
    MS221_IF_Output_Level_0_8Vpp_min_8_22dB,         /* 0.8Vpp    -8 - 22dB      */
    MS221_IF_Output_Level_0_85Vpp_min_7_5_22_5dB,    /* 0.85Vpp   -7.5 - 22.5dB  */
    MS221_IF_Output_Level_0_7Vpp_min_9_21dB,         /* 0.7Vpp    -9 - 21dB      */
    MS221_IF_Output_Level_0_6Vpp_min_10_3_19_7dB,    /* 0.6Vpp    -10.3 - 19.7dB */
    MS221_IF_Output_Level_0_5Vpp_min_12_18dB,        /* 0.5Vpp    -12 - 18dB     */
    MS221_IF_Output_Level_Max
} MS221IF_Output_Level_t, *pMS221IF_Output_Level_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslMS221_Open(
    tmUnitSelect_t              tUnit,      /*  I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
);

tmErrorCode_t
tmbslMS221_HwInit(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

tmErrorCode_t
tmbslMS221_SetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t  powerState  /* I: Power state */
);

tmErrorCode_t
tmbslMS221_SetStandardMode(
    tmUnitSelect_t              tUnit,          /*  I: Unit number */
    MS221StandardMode_t    StandardMode    /*  I: Standard mode of this device */
);

tmErrorCode_t
tmbslMS221_SetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32          uRF     /*  I: RF frequency in hertz */
);

tmErrorCode_t
tmbslMS221_Close(
    tmUnitSelect_t  tUnit   /*  I: Unit number */
);

tmErrorCode_t
tmbslMS221_GetIF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
);

tmErrorCode_t
tmbslMS221_SetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    MS221IF_Output_Level_t  eIFLevel    /* I: IF Level */
);

tmErrorCode_t
tmbslMS221_GetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    MS221IF_Output_Level_t  *peIFLevel  /* I: IF Level */
);

tmErrorCode_t
tmbslMS221_GetPowerLevel(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          pPowerLevel /* O: Power Level in (x100) dBµVrms */
);

tmErrorCode_t
tmbslMS221_CheckHWVersion(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    pMS221Version_t pEsVersion  /*  o: ES version of the HW */
);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_MS221_H */

