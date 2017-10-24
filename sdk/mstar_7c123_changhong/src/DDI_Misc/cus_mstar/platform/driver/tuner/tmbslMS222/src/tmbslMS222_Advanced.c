/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslMS222_Advanced.c
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

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmbslMS222.h"
#include "tmbslMS222_Advanced.h"

#include "tmbslMS222_Local.h"
#ifdef MS222_DVBC_SUPPORTED
#include "tmbslMS222_Config_DVBC.h"
#endif
#ifdef MS222_DVBT_SUPPORTED
#include "tmbslMS222_Config_DVBT.h"
#endif
#ifdef MS222_DTMB_SUPPORTED
#include "tmbslMS222_Config_DTMB.h"
#endif
#ifdef MS222_ISDBT_SUPPORTED
#include "tmbslMS222_Config_ISDBT.h"
#endif
#include "tmbslMS222_Config_Common.h"

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/
#if 0
static tmErrorCode_t
iMS222_LaunchRF_CAL(pMS222Object_t pObj);
#endif

static tmErrorCode_t
iMS222_SetRegMap(pMS222Object_t pObj, UInt8 uAddress, UInt32 uDataLen, UInt8* puData);

static tmErrorCode_t
iMS222_GetRegMap(pMS222Object_t pObj, UInt8 uAddress, UInt32 uDataLen, UInt8* puData);

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/


/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetSWVersion:                                   */
/*                                                                            */
/* DESCRIPTION: Gets the versions of the driver.                              */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetSWVersion(
    ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
)
{
    pSWVersion->compatibilityNr = MS222_COMP_NUM;
    pSWVersion->majorVersionNr  = MS222_MAJOR_VER;
    pSWVersion->minorVersionNr  = MS222_MINOR_VER;

    return TM_OK;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetSWSettingsVersion:                           */
/*                                                                            */
/* DESCRIPTION: Returns the version of the driver settings.                   */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetSWSettingsVersion(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
)
{
    pSWSettingsVersion->customerNr      = MS222_SETTINGS_CUSTOMER_NUM;
    pSWSettingsVersion->projectNr       = MS222_SETTINGS_PROJECT_NUM;
    pSWSettingsVersion->majorVersionNr  = MS222_SETTINGS_MAJOR_VER;
    pSWSettingsVersion->minorVersionNr  = MS222_SETTINGS_MINOR_VER;

    return TM_OK;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetPowerState:                                  */
/*                                                                            */
/* DESCRIPTION: Gets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t* pPowerState /* O: Power state */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetPowerState(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(pPowerState == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pPowerState = pObj->curPowerState;
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_SetGpio                                         */
/*                                                                            */
/* DESCRIPTION: Sets the GPIOs.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_SetGpio(
    tmUnitSelect_t          tUnit,           /* I: Unit number */
    UInt8                   bNumber,         /* I: Number of the GPIO to set */
    Bool                    bActive          /* I: GPIO enabled/disabled */
)
{
    pMS222Object_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetGpio(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        switch (bNumber)
        {
            case 1:
                /* Set GPIO n�1 */
                err = iMS222_Write(pObj, &gMS222_Reg_Gpio_ctrl_byte__GPIO1, (bActive==True)?1:0, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                break;

            case 2:
                /* Set GPIO n�2 */
                err = iMS222_Write(pObj, &gMS222_Reg_Gpio_ctrl_byte__GPIO2, (bActive==True)?1:0, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                break;

            default:
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslMS222_SetGpio(0x%08X) invalid GPIO number.", tUnit));
                break;
        }
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetCF_Offset:                                   */
/*                                                                            */
/* DESCRIPTION: Gets CF Offset.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetCF_Offset(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32*         puOffset    /* O: Center frequency offset in hertz */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetCF_Offset(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(   pObj->StandardMode<=MS222_StandardMode_Unknown
        || pObj->StandardMode>=MS222_StandardMode_Max
        || pObj->pStandard == Null
        || puOffset == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *puOffset = pObj->pStandard->CF_Offset;
    }

    _MUTEX_RELEASE(MS222)

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslMS222_SetPllManual:                                   */
/*                                                                            */
/* DESCRIPTION: Sets bOverridePLL flag.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_SetPllManual(
    tmUnitSelect_t  tUnit,         /* I: Unit number */
    Bool            bOverridePLL   /* I: Determine if we need to put PLL in manual mode in SetRF */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetPllManual(0x%08X)", tUnit);

    pObj->bOverridePLL = bOverridePLL;

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetPllManual:                                   */
/*                                                                            */
/* DESCRIPTION: Gets bOverridePLL flag.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetPllManual(
    tmUnitSelect_t  tUnit,         /* I: Unit number */
    Bool*           pbOverridePLL  /* O: Determine if we need to put PLL in manual mode in SetRF */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetPllManual(0x%08X)", tUnit);

    if(pbOverridePLL == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pbOverridePLL = pObj->bOverridePLL;
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetIRQ:                                         */
/*                                                                            */
/* DESCRIPTION: Gets IRQ status.                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetIRQ(
    tmUnitSelect_t  tUnit   /* I: Unit number */,
    Bool*           pbIRQ   /* O: IRQ triggered */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uValue = 0;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetIRQ(0x%08X)", tUnit);

    if(pbIRQ == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pbIRQ = 0;

        err = iMS222_Read(pObj, &gMS222_Reg_IRQ_status__IRQ_status, &uValue, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            *pbIRQ = uValue;
        }
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_WaitIRQ:                                        */
/*                                                                            */
/* DESCRIPTION: Waits for the IRQ to raise.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_WaitIRQ(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          timeOut,    /* I: timeOut for IRQ wait */
    UInt32          waitStep,   /* I: wait step */
    UInt8           irqStatus   /* I: IRQs to wait */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_WaitIRQ(0x%08X)", tUnit);

    err = iMS222_WaitIRQ(pObj, timeOut, waitStep, irqStatus);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_WaitIRQ(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetXtalCal_End:                                 */
/*                                                                            */
/* DESCRIPTION: Gets XtalCal_End status.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetXtalCal_End(
    tmUnitSelect_t  tUnit           /* I: Unit number */,
    Bool*           pbXtalCal_End   /* O: XtalCal_End triggered */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uValue = 0;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetXtalCal_End(0x%08X)", tUnit);

    if(pbXtalCal_End == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pbXtalCal_End = 0;

        err = iMS222_Read(pObj, &gMS222_Reg_IRQ_status__XtalCal_End, &uValue, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            *pbXtalCal_End = uValue;
        }
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetPLLState:                                   */
/*                                                                            */
/* DESCRIPTION: Gets PLL Lock Status.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetPLLState(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmbslFrontEndState_t*   pPLLState /* O: PLL Lock status */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uValue = 0, uValueLO = 0;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetPLLState(0x%08X)", tUnit);

    if( pPLLState == Null )
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        err = iMS222_Read(pObj, &gMS222_Reg_Power_state_byte_1__LO_Lock, &uValueLO, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            err = iMS222_Read(pObj, &gMS222_Reg_IRQ_status__IRQ_status, &uValue, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            uValue = uValue & uValueLO;

            *pPLLState =  (uValue)? tmbslFrontEndStateLocked : tmbslFrontEndStateNotLocked;
        }
        else
        {
            *pPLLState = tmbslFrontEndStateUnknown;
        }
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetRF:                                          */
/*                                                                            */
/* DESCRIPTION: Gets tuned RF.                                                */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetRF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puRF    /* O: RF frequency in hertz */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetRF(0x%08X)", tUnit);

    if(puRF == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get RF */
        *puRF = pObj->uRF;
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetStandardMode                                 */
/*                                                                            */
/* DESCRIPTION: Gets the standard mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetStandardMode(
    tmUnitSelect_t          tUnit,          /* I: Unit number */
    MS222StandardMode_t  *pStandardMode  /* O: Standard mode of this device */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetStandardMode(0x%08X)", tUnit);

    if(pStandardMode == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get standard mode */
        *pStandardMode = pObj->StandardMode;
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_SetRawRF:                                       */
/*                                                                            */
/* DESCRIPTION: Tunes to a RF (Without CF_Offset correction).                 */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_SetRawRF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32          uRF     /* I: RF frequency in hertz */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetRawRF(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(   pObj->StandardMode<=MS222_StandardMode_Unknown
        || pObj->StandardMode>=MS222_StandardMode_Max
        || pObj->pStandard == Null)
    {
        err = MS222_ERR_STD_NOT_SET;
    }

    if(err == TM_OK)
    {
        /* Check if Hw is ready to operate */
        err = iMS222_CheckHwState(pObj, MS222_HwStateCaller_SetRawRF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_CheckHwState(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        pObj->uRF = uRF;
        pObj->uProgRF = pObj->uRF;

        err = iMS222_SetRF(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_SetRF(0x%08X) failed.", tUnit));
    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetAGCGainRead                                  */
/*                                                                            */
/* DESCRIPTION: Reads AGC1, AGC2, AGC4, AGC5.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetAGCGainRead(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pAGC1Read,  /* O: AGC1 Read */
    UInt8*          pAGC2Read,  /* O: AGC2 Read */
    UInt8*          pAGC3Read  /* O: AGC3 Read */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetAGCGainRead(0x%08X)", tUnit);

        if(pAGC1Read != Null)
        {
            err = iMS222_Read(pObj, &gMS222_Reg_AGC1_LT_Gain_status__AGC1_gain_read, pAGC1Read, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK && pAGC2Read != Null)
        {
            err = iMS222_Read(pObj, &gMS222_Reg_AGC2_Gain_status__AGC2_gain_read, pAGC2Read, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK && pAGC3Read != Null)
        {
            err = iMS222_Read(pObj, &gMS222_Reg_AGC3_gain_status__AGC3_Gain_Read, pAGC2Read, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));
        }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetRSSI:                                        */
/*                                                                            */
/* DESCRIPTION: Gets RSSI.                                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetRSSI(
     tmUnitSelect_t tUnit,  /* I: Unit number */
     UInt8*         puValue /* O: Address of the variable to output item value */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetRSSI(0x%08X)", tUnit);

    if (puValue == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Check if Hw is ready to operate */
        err = iMS222_CheckHwState(pObj, MS222_HwStateCaller_GetRSSI);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_CheckHwState(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iMS222_GetRSSI(pObj, puValue);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_GetRSSI(0x%08X) failed.", tUnit));
    }

    _MUTEX_RELEASE(MS222)

    return err;
}



/*============================================================================*/
/* FUNCTION:    tmbslMS222_SetLLPowerState                                */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_SetLLPowerState(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    MS222PowerState_t    powerState  /* I: Power state of MS222 */
 )
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetLLPowerState(0x%08X)", tUnit);

    pObj->curPowerState = tmPowerMax;

    err = iMS222_SetLLPowerState(pObj, powerState);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_SetLLPowerState(0x%08X, %d) failed.", tUnit, (int)powerState));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetLLPowerState                                */
/*                                                                            */
/* DESCRIPTION: Gets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetLLPowerState(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    MS222PowerState_t*   pPowerState /* O: Power state of MS222 */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uValue = 0;


    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetLLPowerState(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(pPowerState == Null)
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        err = iMS222_Read(pObj, &gMS222_Reg_Power_state_byte_2__Power_State_Mode, &uValue, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
		switch (uValue)
		{
			case MS222_power_state_mode_NORMAL_MODE_VALUE_0:
				*pPowerState = MS222_PowerNormalMode;
				break;
			case MS222_power_state_mode_STDBY_PLUS16M_VALUE_1:
				*pPowerState = MS222_PowerStandbyWithXtalOn;
				break;
			case MS222_power_state_mode_STDBY_PLUS16M_PLUS_LT_VALUE_2:
				*pPowerState = MS222_PowerStandbyWithLtOnWithXtalOn;
				break;
			case MS222_power_state_mode_STDBY_PLUS16M_PLUS_PLL_VALUE_3:
				*pPowerState = MS222_PowerStandbyWithPllOnWithXtalOn;
				break;
			case MS222_power_state_mode_STDBY_PLUS_16M_PLUS_LT_PLUS_PLL_VALUE_4:
				*pPowerState = MS222_PowerStandbyWithLtOnWithPllOnWithXtalOn;
				break;
			default :
				err = MS222_ERR_NOT_SUPPORTED;
				break;
		}

    }

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_PLLcalc                                         */
/*                                                                            */
/* DESCRIPTION: simulate Hardware PLL calculation                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_PLLcalc(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
	UInt32          LO /*I input LO in KHz */
	)
{
	pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
	UInt32 N_int, K_int;
    UInt8 LOPostDiv;    /* Absolute value */
    UInt8 Prescaler;  /* Absolute value  */

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetPowerSavingMode(0x%08X)", tUnit);

	err = iMS222_CalculatePostDivAndPrescalerBetterMargin( LO, False, &LOPostDiv, &Prescaler);

    if(err == TM_OK)
    {
		err = iMS222_CalculateNIntKInt(LO, LOPostDiv, Prescaler, &N_int, &K_int, pObj->eXtalFreq-1);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_CalculateNIntKInt(0x%08X) failed.", pObj->tUnitW));
	}

    /* Decode PostDiv */
    switch(LOPostDiv)
    {
        case 1:
            LOPostDiv = 1;
            break;
        case 2:
            LOPostDiv = 2;
            break;
        case 4:
            LOPostDiv = 3;
            break;
        case 8:
            LOPostDiv = 4;
            break;
        case 16:
            LOPostDiv = 5;
            break;
        default:
            err = MS222_ERR_BAD_PARAMETER;
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslMS222_PLLcalc *PostDiv value is wrong.", pObj->tUnitW));
            break;
   }

    if(err == TM_OK)
    {
        err = iMS222_Write(pObj, &gMS222_Reg_Main_Post_Divider_byte__LOPostDiv, LOPostDiv, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iMS222_Write(pObj, &gMS222_Reg_Main_Post_Divider_byte__LOPresc, Prescaler, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iMS222_Write(pObj, &gMS222_Reg_Sigma_delta_byte_4__LO_Frac_0, (UInt8)(K_int & 0xFF), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iMS222_Write(pObj, &gMS222_Reg_Sigma_delta_byte_3__LO_Frac_1, (UInt8)((K_int >> 8) & 0xFF), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iMS222_Write(pObj, &gMS222_Reg_Sigma_delta_byte_2__LO_Frac_2, (UInt8)((K_int >> 16) & 0xFF), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iMS222_Write(pObj, &gMS222_Reg_Sigma_delta_byte_1__LO_Int, (UInt8)(N_int & 0xFF), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    _MUTEX_RELEASE(MS222)

    return err;

}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_SetXtal                                        */
/*                                                                            */
/* DESCRIPTION: sets the Xtal                                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_SetXtal(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
	MS222XtalFreq_t eXtal   /*I: Xtal freq */
	)
{
	pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uVal;
    UInt8 uVal2;
    UInt32          uIF = 0;
    UInt8           ProgIF = 0;
    UInt8           LOPostdiv = 0;
    UInt8           LOPrescaler = 0;
    UInt32          FVCO = 0;
    UInt32          uXtalFreqId;
    UInt32          i;
    UInt8           uRDiv;
    UInt8           uNDiv;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetXtal(0x%08X)", tUnit);

    pObj->eXtalFreq = eXtal;
    uXtalFreqId = pObj->eXtalFreq-1;

    /* Xt_Freq_2 */
    uVal = (RDivNDivTable[pObj->eXtalFreq-1].uXtalFreq/1000) & 0xFF;
    uVal2 = ((RDivNDivTable[pObj->eXtalFreq-1].uXtalFreq/1000) >> 8) & 0x7F;
    err = iMS222_Write(pObj, &gMS222_Reg_Xtal_Flex_byte_1__XT_Freq_2, uVal2, Bus_RW);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* Xt_Freq_1 */
        err = iMS222_Write(pObj, &gMS222_Reg_Xtal_Flex_byte_2__XT_Freq_1, uVal, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Read PostDiv et Prescaler */
    err = iMS222_Read(pObj, &gMS222_Reg_Main_Post_Divider_byte, &LOPostdiv, Bus_RW);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* PostDiv */
        err = iMS222_Read(pObj, &gMS222_Reg_Main_Post_Divider_byte__LOPostDiv, &LOPostdiv, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            /* Prescaler */
            err = iMS222_Read(pObj, &gMS222_Reg_Main_Post_Divider_byte__LOPresc, &LOPrescaler, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* IF */
            err = iMS222_Read(pObj, &gMS222_Reg_IF_Frequency_byte__IF_Freq, &ProgIF, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* Decode IF */
            uIF = ProgIF*50000;

            /* Decode PostDiv */
            switch(LOPostdiv)
            {
            case 1:
                LOPostdiv = 1;
                break;
            case 2:
                LOPostdiv = 2;
                break;
            case 3:
                LOPostdiv = 4;
                break;
            case 4:
                LOPostdiv = 8;
                break;
            case 5:
                LOPostdiv = 16;
                break;
            default:
                err = MS222_ERR_BAD_PARAMETER;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_OverrideICP(0x%08X) LOPostDiv value is wrong.", pObj->tUnitW));
                break;
            }
        }
        if(err == TM_OK)
        {
            /* Calculate FVCO in MHz*/
            FVCO = LOPostdiv * LOPrescaler * ((pObj->uRF + uIF) / 1000);

            /* Set correct ICP */


            for (i = 0 ; i < RDivNDivTable[uXtalFreqId].uNbMode ; i++)
            {
                if (FVCO >= RDivNDivTable[uXtalFreqId].psVCOModes[i].uVCOFreq)
                {
                    uRDiv = RDivNDivTable[uXtalFreqId].psVCOModes[i].uRDiv;
                    uNDiv = RDivNDivTable[uXtalFreqId].psVCOModes[i].uNDiv;
                }
            }

            if(err == TM_OK)
            {
                /* N_Div */
                err = iMS222_Write(pObj, &gMS222_Reg_Xtal_Flex_byte_3__N_Div, uNDiv, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                /* R_Div */
                err = iMS222_Write(pObj, &gMS222_Reg_Xtal_Flex_byte_3__R_Div, uRDiv, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
    }

	if(err == TM_OK)
    {
        /* Force_Icp_256us */
        err = iMS222_Write(pObj, &gMS222_Reg_Xtal_Flex_byte_4__Force_Icp_256us_1, (UInt8)(RDivNDivTable[pObj->eXtalFreq-1].uForceIcp256us & 0x00FF), Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));

		if(err == TM_OK)
		{
			err = iMS222_Write(pObj, &gMS222_Reg_Xtal_Flex_byte_3__Force_Icp_256us_2, (UInt8)((RDivNDivTable[pObj->eXtalFreq-1].uForceIcp256us & 0x1F00) >> 8), Bus_RW);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
		}
    }

	if(err == TM_OK)
    {
        /* RC_Delay_Ck_Length */
        err = iMS222_Write(pObj, &gMS222_Reg_Xtal_flex_byte_5__RC_Delay_Ck_Length, (UInt8)RDivNDivTable[pObj->eXtalFreq-1].uRcDelayLength, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }



    _MUTEX_RELEASE(MS222)

    return err;

}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetXtal                                        */
/*                                                                            */
/* DESCRIPTION: gets the Xtal                                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetXtal(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
	MS222XtalFreq_t *peXtal /*O: Xtal freq */
	)
{
	pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uVal;
    UInt8 uVal2;
    UInt32 uFreq;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetXtal(0x%08X)", tUnit);

    /* Xt_Freq_2 */
    err = iMS222_Read(pObj, &gMS222_Reg_Xtal_Flex_byte_1__XT_Freq_2, &uVal2, Bus_RW);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* Xt_Freq_1 */
        err = iMS222_Read(pObj, &gMS222_Reg_Xtal_Flex_byte_2__XT_Freq_1, &uVal, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if (err == TM_OK)
    {
        uFreq = (uVal + (uVal2 << 8))*1000;

        switch (uFreq)
        {
        case 16000000:
            *peXtal = MS222_XtalFreq_16000000;
            break;
        case 24000000:
            *peXtal = MS222_XtalFreq_24000000;
            break;
        case 25000000:
            *peXtal = MS222_XtalFreq_25000000;
            break;
        case 27000000:
            *peXtal = MS222_XtalFreq_27000000;
            break;
        case 30000000:
            *peXtal = MS222_XtalFreq_30000000;
            break;
        default:
            *peXtal = MS222_XtalFreq_Unknown;
            break;
        }
    }

    _MUTEX_RELEASE(MS222)

    return err;

}


/*============================================================================*/
/* FUNCTION:    tmbslMS222_WriteRegMap                                     */
/*                                                                            */
/* DESCRIPTION: Writes driver RegMap cached data to MS222 hardware.        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_WriteRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data to write */
    UInt32          uWriteLen   /* I: Number of data to write */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_WriteRegMap(0x%08X)", tUnit);

    err = iMS222_WriteRegMap(pObj, uAddress, uWriteLen);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_WriteRegMap(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_ReadRegMap                                      */
/*                                                                            */
/* DESCRIPTION: Reads driver RegMap cached data from MS222 hardware.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_ReadRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data to read */
    UInt32          uReadLen    /* I: Number of data to read */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_ReadRegMap(0x%08X)", tUnit);

    err = iMS222_ReadRegMap(pObj, uAddress, uReadLen);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_ReadRegMap(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_SetRegMap                                       */
/*                                                                            */
/* DESCRIPTION: Sets driver RegMap cached data.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_SetRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data start address */
    UInt32          uDataLen,   /* I: Number of data */
    UInt8*          puData      /* I: Data to set */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_SetRegMap(0x%08X)", tUnit);

    err = iMS222_SetRegMap(pObj, uAddress, uDataLen, puData);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_SetRegMap(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_GetRegMap                                       */
/*                                                                            */
/* DESCRIPTION: Gets driver RegMap cached data.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_GetRegMap(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uAddress,   /* I: Data start address */
    UInt32          uDataLen,   /* I: Number of data */
    UInt8*          puData      /* I: Data to get */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_GetRegMap(0x%08X)", tUnit);

    err = iMS222_GetRegMap(pObj, uAddress, uDataLen, puData);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_GetRegMap(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_Write                                           */
/*                                                                            */
/* DESCRIPTION: Writes in MS222 hardware                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_Write(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const MS222_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8                       uData,      /* I: Data to write */
    tmbslFrontEndBusAccess_t                eBusAccess  /* I: Access to bus */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_Write(0x%08X)", tUnit);

    err = iMS222_Write(pObj, pBitField, uData, eBusAccess);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslMS222_Read                                            */
/*                                                                            */
/* DESCRIPTION: Reads in MS222 hardware                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslMS222_Read(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const MS222_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8*                      puData,     /* I: Data to read */
    tmbslFrontEndBusAccess_t                eBusAccess  /* I: Access to bus */
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslMS222_Read(0x%08X)", tUnit);

    err = iMS222_Read(pObj, pBitField, puData, eBusAccess);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(MS222)

    return err;
}

tmErrorCode_t
tmbslMS222_AGCTOP23(
    tmUnitSelect_t              tUnit,
    Int8 lDelta
)
{
    pMS222Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uAGC3Gain;
    Int8 AGC3_Gain_Read[8] = {-6,-3,0,3,6,9,12,15};
    UInt32 i=0;
    UInt8 uValue1 =0;
    UInt8 uValue2 =0;

    /* Get a driver instance */
    err = iMS222_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(MS222)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslAGCTOP23(0x%08X)", tUnit);

    /* get AGC3 gain */
    err = iMS222_Read(pObj, &gMS222_Reg_AGC3_gain_status__AGC3_Gain_Read, &uAGC3Gain, Bus_RW);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Read(0x%08X) failed.", pObj->tUnitW));


    if (pObj->eChipVersion == MS222_VersionES1)
    {
        /* find the base TOP */
        while ((pObj->uProgRF >= pObj->pStandard->AGC2_TOP_ES1[i].uFreq) && (i<MS222_CONFIG_STD_FREQ_NUM))
        {
            i++;
        }
        i--;
        if (pObj->pStandard->AGC2_TOP_ES1[i].uFreq == -1)
        {
            i--;
        }
        uValue1 = pObj->pStandard->AGC2_TOP_ES1[i].uTOPDn;
        uValue2 = pObj->pStandard->AGC2_TOP_ES1[i].uTOPUp;

        if (err == TM_OK)
        {
            if (AGC3_Gain_Read[uAGC3Gain] == 15)
            {
                /* AGC2_TOP_DO */
                err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_2__AGC2_TOP_DO, 107-uValue1-lDelta, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* AGC2_TOP_UP */
                    err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_3__AGC2_TOP_UP, 107-uValue2-lDelta, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
            else if (AGC3_Gain_Read[uAGC3Gain] <= 9)
            {
                /* AGC2_TOP_DO */
                err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_2__AGC2_TOP_DO, 107-uValue1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* AGC2_TOP_UP */
                    err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_3__AGC2_TOP_UP, 107-uValue2, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
        }
    }
    else if ( pObj->eChipVersion == MS222_VersionES2 )
    {
        /* find the base TOP */
        while ((pObj->uProgRF >= pObj->pStandard->AGC2_TOP_ES2[i].uFreq) && (i<MS222_CONFIG_STD_FREQ_NUM))
        {
            i++;
        }
        i--;
        if (pObj->pStandard->AGC2_TOP_ES2[i].uFreq == -1)
        {
            i--;
        }
        uValue1 = pObj->pStandard->AGC2_TOP_ES2[i].uTOPDn;
        uValue2 = pObj->pStandard->AGC2_TOP_ES2[i].uTOPUp;

        if (err == TM_OK)
        {
            if (AGC3_Gain_Read[uAGC3Gain] == 15)
            {
                /* AGC2_TOP_DO */
                err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_2__AGC2_TOP_DO, (107-uValue1)*2-lDelta, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* AGC2_TOP_UP */
                    err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_3__AGC2_TOP_UP, (107-uValue2)*2-lDelta, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
            else if (AGC3_Gain_Read[uAGC3Gain] <= 9)
            {
                /* AGC2_TOP_DO */
                err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_2__AGC2_TOP_DO, (107-uValue1)*2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* AGC2_TOP_UP */
                    err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_3__AGC2_TOP_UP, (107-uValue2)*2, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
        }
    }
    else
    {
        /* find the base TOP */
        while ((pObj->uProgRF >= pObj->pStandard->AGC2_TOP_ES3[i].uFreq) && (i<MS222_CONFIG_STD_FREQ_NUM))
        {
            i++;
        }
        i--;
        if (pObj->pStandard->AGC2_TOP_ES3[i].uFreq == -1)
        {
            i--;
        }
        uValue1 = pObj->pStandard->AGC2_TOP_ES3[i].uTOPDn;
        uValue2 = pObj->pStandard->AGC2_TOP_ES3[i].uTOPUp;

        if (err == TM_OK)
        {
            if (AGC3_Gain_Read[uAGC3Gain] == 15)
            {
                /* AGC2_TOP_DO */
                err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_2__AGC2_TOP_DO, (107-uValue1)*2-lDelta, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* AGC2_TOP_UP */
                    err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_3__AGC2_TOP_UP, (107-uValue2)*2-lDelta, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
            else if (AGC3_Gain_Read[uAGC3Gain] <= 9)
            {
                /* AGC2_TOP_DO */
                err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_2__AGC2_TOP_DO, (107-uValue1)*2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* AGC2_TOP_UP */
                    err = iMS222_Write(pObj, &gMS222_Reg_AGC2_byte_3__AGC2_TOP_UP, (107-uValue2)*2, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
        }
    }



    _MUTEX_RELEASE(MS222)

    return err;
}

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/
#if 0
/*============================================================================*/
/* FUNCTION:    iMS222_LaunchRF_CAL                                        */
/*                                                                            */
/* DESCRIPTION: Launches RF CAL.                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iMS222_LaunchRF_CAL(
    pMS222Object_t   pObj
)
{
    tmErrorCode_t   err  = TM_OK;

    /* Set IRQ_clear */
    err = iMS222_Write(pObj, &gMS222_Reg_IRQ_clear, MS222_IRQ_Global|MS222_IRQ_SetRF, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* Set state machine and Launch it */
        err = iMS222_SetMSM(pObj, MS222_MSM_SetRF, True);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_SetMSM(0x%08X, MS222_MSM_SetRF) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            err = iMS222_WaitIRQ(pObj, 50, 5, MS222_IRQ_SetRF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iMS222_WaitIRQ(0x%08X, 50, 5, MS222_IRQ_SetRF) failed.", pObj->tUnitW));
        }
    }

    return err;
}
#endif
/*============================================================================*/
/* FUNCTION:    iMS222_SetRegMap                                           */
/*                                                                            */
/* DESCRIPTION: Sets driver RegMap cached data.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iMS222_SetRegMap(
    pMS222Object_t   pObj,       /* I: Driver object */
    UInt8               uAddress,   /* I: Data start address */
    UInt32              uDataLen,   /* I: Number of data */
    UInt8*              puData      /* I: Data to set */
)
{
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;
    UInt32          count = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iMS222_SetRegMap(0x%08X)", pObj->tUnitW);

    if( puData != Null &&
        uAddress < MS222_REG_MAP_NB_BYTES &&
        (uAddress + uDataLen) <= MS222_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        for(count = 0; count<uDataLen; count++)
        {
            pRegData[count] = puData[count];
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iMS222_GetRegMap                                           */
/*                                                                            */
/* DESCRIPTION: Gets driver RegMap cached data.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iMS222_GetRegMap(
    pMS222Object_t   pObj,       /* I: Driver object */
    UInt8               uAddress,   /* I: Data start address */
    UInt32              uDataLen,   /* I: Number of data */
    UInt8*              puData      /* I: Data to get */
)
{
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;
    UInt32          count = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iMS222_GetRegMap(0x%08X)", pObj->tUnitW);

    if( puData != Null &&
        uAddress < MS222_REG_MAP_NB_BYTES &&
        (uAddress + uDataLen) <= MS222_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = MS222_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        for(count = 0; count<uDataLen; count++)
        {
            puData[count] = pRegData[count];
        }
    }

    return err;
}

