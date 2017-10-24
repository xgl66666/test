/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslMS222_Advanced.h
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

#ifndef _TMBSL_MS222_ADVANCED_H
#define _TMBSL_MS222_ADVANCED_H

#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetSWVersion(
    ptmSWVersion_t  pSWVersion  /*  I: Receives SW Version */
);

tmErrorCode_t
tmbslMS222_GetSWSettingsVersion(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
);

tmErrorCode_t
tmbslMS222_SetGpio(
    tmUnitSelect_t          tUnit,           /* I: Unit number */
    UInt8                   bNumber,         /* I: Number of the GPIO to set */
    Bool                    bActive          /* I: GPIO enabled/disabled */
);

tmErrorCode_t
tmbslMS222_GetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t* pPowerState /* O: Power state */
);

tmErrorCode_t
tmbslMS222_GetStandardMode(
    tmUnitSelect_t              tUnit,          /*  I: Unit number */
    MS222StandardMode_t    *pStandardMode  /*  O: Standard mode of this device */
);

tmErrorCode_t
tmbslMS222_GetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32*         pRF     /*  O: RF frequency in hertz */
);

tmErrorCode_t
tmbslMS222_GetCF_Offset(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32*         puOffset    /* O: Center frequency offset in hertz */
);

tmErrorCode_t
tmbslMS222_GetAgcStatus(
    tmUnitSelect_t          tUnit,       /* I: Unit number */
    Bool*                   pbAgcLock,   /* O: AGC Lock status */
    Bool*                   pbAgcMax     /* O: AGC Max status */
);

tmErrorCode_t
tmbslMS222_SetPllManual(
    tmUnitSelect_t  tUnit,         /* I: Unit number */
    Bool            bOverridePLL   /* I: Determine if we need to put PLL in manual mode in SetRF */
);

tmErrorCode_t
tmbslMS222_GetPllManual(
    tmUnitSelect_t  tUnit,         /* I: Unit number */
    Bool*           pbOverridePLL  /* O: Determine if we need to put PLL in manual mode in SetRF */
);

tmErrorCode_t
tmbslMS222_GetIRQ(
    tmUnitSelect_t  tUnit  /* I: Unit number */,
    Bool*           pbIRQ  /* O: IRQ triggered */
);

tmErrorCode_t
tmbslMS222_WaitIRQ(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          timeOut,    /* I: timeOut for IRQ wait */
    UInt32          waitStep,   /* I: wait step */
    UInt8           irqStatus   /* I: IRQs to wait */
);

tmErrorCode_t
tmbslMS222_GetXtalCal_End(
    tmUnitSelect_t  tUnit           /* I: Unit number */,
    Bool*           pbXtalCal_End   /* O: XtalCal_End triggered */
);

tmErrorCode_t
tmbslMS222_SetRawRF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32          uRF     /* I: RF frequency in hertz */
);

tmErrorCode_t
tmbslMS222_GetAGCGainRead(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pAGC1Read,  /* O: AGC1 Read */
    UInt8*          pAGC2Read,  /* O: AGC2 Read */
    UInt8*          pAGC3Read   /* O: AGC3 Read */

);

tmErrorCode_t
tmbslMS222_GetRSSI(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt8*          puValue /*  O: Address of the variable to output item value */
);

tmErrorCode_t
tmbslMS222_SetLLPowerState(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    MS222PowerState_t    powerState  /* I: Power state of MS222 */
 );

tmErrorCode_t
tmbslMS222_GetLLPowerState(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    MS222PowerState_t*   pPowerState /* O: Power state of MS222 */
);

tmErrorCode_t
tmbslMS222_PLLcalc(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
	UInt32          LO /*I input LO in KHz */
	);

tmErrorCode_t
tmbslMS222_WriteRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data to write */
    UInt32          uWriteLen   /* I: Number of data to write */
);

tmErrorCode_t
tmbslMS222_ReadRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data to read */
    UInt32          uReadLen    /* I: Number of data to read */
);

tmErrorCode_t
tmbslMS222_SetRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data start address */
    UInt32          uDataLen,   /* I: Number of data */
    UInt8*          puData      /* I: Data to set */
);

tmErrorCode_t
tmbslMS222_GetRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data start address */
    UInt32          uDataLen,   /* I: Number of data */
    UInt8*          puData      /* I: Data to get */
);


tmErrorCode_t
tmbslMS222_Write(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const MS222_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8                       uData,      /* I: Data to write */
    tmbslFrontEndBusAccess_t    eBusAccess  /* I: Access to bus */
);

tmErrorCode_t
tmbslMS222_Read(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const MS222_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8*                      puData,     /* I: Data to read */
    tmbslFrontEndBusAccess_t    eBusAccess  /* I: Access to bus */
);

tmErrorCode_t
tmbslMS222_GetPLLState(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
);

tmErrorCode_t
tmbslMS222_SetXtal(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
	MS222XtalFreq_t eXtal   /*I: Xtal freq */
);

tmErrorCode_t
tmbslMS222_GetXtal(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
	MS222XtalFreq_t *peXtal /*O: Xtal freq */
);

tmErrorCode_t
tmbslMS222_AGCTOP23(
    tmUnitSelect_t              tUnit,
    Int8                        lDelta
);



#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_MS222_ADVANCED_H */

