/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250AdvanceCustom.c
 *
 *                3
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
#include "../../inc/tmNxTypes.h"
#include "../../inc/tmCompId.h"
#include "../../inc/tmFrontEnd.h"
#include "../../inc/tmbslFrontEndTypes.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "../../tmddTDA18250/inc/tmddTDA18250.h"

#include "../inc/tmbslTDA18250.h"
#include "../inc/tmbslTDA18250InstanceCustom.h"
#include "tmbslTDA18250local.h"
#include "tmbslTDA18250Instance.h"
#include "../inc/tmbslTDA18250AdvanceCustom.h"


static tmErrorCode_t
TDA18250IRCALDigitalMaster_InitRestore
(
    tmUnitSelect_t     tUnit,       /* I: Unit number */
    tmTDA18250_Step_t  uIRStep      /* I: IRCAL Step  */
);

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250DigitalMasterReset:                              */
/*                                                                            */
/* DESCRIPTION: Resets the TDA18250 Digital Master.                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250DigitalMasterReset
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Slave Type not supported */
        if ( pObj->deviceType > tmTDA18250_DeviceTypeMasterWithoutXTout )
        {
            err = TDA18250_ERR_NOT_SUPPORTED;
        }
    }

    if (err == TM_OK)
    {
        /* Switch smooth time constant setting to ensure smooth opening */
        err = tmddTDA18250_SetRFSW_T_CST(tUnit, pObj->Switch_TimeCst);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetRFSW_T_CST(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err = TDA18250SetSwitchOpen(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetSwitchOpen(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /***** Write all initialization bytes except AGC registers *****/
            err = tmddTDA18250Reset(tUnit, pObj->clock_mode_auto, tmddTDA18250_WithoutResetAGC);
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250ResetXtoutMTO(tUnit, (tmddTDA18250DeviceType_t)pObj->deviceType);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250ResetXtoutMTO(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetResetMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetResetMSM(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }

        /***** Image Rejection Calibration *****/
        if (err == TM_OK)
        {
            /* stop AGC1 loop */
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250IRCALDigitalMaster_InitRestore(tUnit, tmTDA18250_INIT);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250IRCAL_Init(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250IRCalibration(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250IRCalibration(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250IRCALDigitalMaster_InitRestore(tUnit, tmTDA18250_RESTORE);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250IRCAL_Init(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* stop AGC1 loop */
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        /***** Write default LO *****/
        if (err == TM_OK)
        {
            tmddTDA18250SetLOFreq(tUnit, TMBSL_TDA18250_DEFAULT_LO);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLOFreq(0x%08X) failed.", tUnit));
            pObj->uRF = TMBSL_TDA18250_DEFAULT_LO - pObj->Std_Array[pObj->StandardMode].IF ;
        }

        /***** Write default smooth offset between RF and Loop throught *****/
        if (err == TM_OK)
        {
            tmddTDA18250SetLT_smooth_offset(tUnit, 0);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLT_smooth_offset(0x%08X) failed.", tUnit));
        }

        /* Switch smooth time constant setting because in direct mode in register initialization */
        if (err == TM_OK)
        {
            err = tmddTDA18250_SetRFSW_T_CST(tUnit, pObj->Switch_TimeCst);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetRFSW_T_CST(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err = TDA18250SetSwitchClose(tUnit, tmTDA18250_IND_ANLG_Band_7);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetSwitchClose(0x%08X) failed.", tUnit));
        }

        /* default band is ANALOG 7MHz */
        if (err == TM_OK)
        {
            pObj->BandIndex = tmTDA18250_IND_ANLG_Band_7;
        }

        if (err == TM_OK)
        {
            err= tmddTDA18250SetHP_MTO_T_CST(tUnit, pObj->MTO_TimeCst);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetHP_MTO_T_CST(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err= tmddTDA18250Setnotch_select(tUnit, 01);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setnotch_select(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250IRCALDigitalMaster_InitRestore:                       */
/*                                                                            */
/* DESCRIPTION: IC Init before / Restore after calibration.                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       only for digital master reset                                 */
/*============================================================================*/
tmErrorCode_t
TDA18250IRCALDigitalMaster_InitRestore
(
    tmUnitSelect_t     tUnit,       /* I: Unit number */
    tmTDA18250_Step_t  uIRStep      /* I: IRCAL Step  */
)
{
    ptmTDA18250Object_t             pObj = Null;
    tmErrorCode_t                   err = TM_OK;
    UInt8                           uPower_Down_byte_2;
    UInt8                           uAGC2b_byte;
    tmddTDA18250_SW_RFcal_modeOFF_t uSwitch = tmddTDA18250_RFcal_modeOFF_Max;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        if (uIRStep == tmTDA18250_INIT)
        {
            uSwitch            = tmddTDA18250_RFcal_modeOFF_OPEN;
            uPower_Down_byte_2 = 0xAF;
            uAGC2b_byte        = 0x8F;
        }
        if (uIRStep == tmTDA18250_RESTORE)
        {
            uSwitch            = tmddTDA18250_RFcal_modeOFF_CLOSE;
            uPower_Down_byte_2 = 0x63;
            uAGC2b_byte        = 0x9F;
        }

        if (err == TM_OK)
        {
            /* Switch off/on the entire filter switch */
            err = tmddTDA18250SetRFcal_modeOFF(tUnit, uSwitch);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFcal_modeOFF(0x%08X) failed.", tUnit));
        }
        /* remark: AGC1b & AGC2a power down not activated after registers initialization */

        if (err == TM_OK)
        {
            err = tmddTDA18250Write(tUnit, 0x18, 1, &uPower_Down_byte_2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Write(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Write(tUnit, 0x1C, 1, &uAGC2b_byte);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Write(0x%08X) failed.", tUnit));
        }

    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC1:                                         */
/*                                                                            */
/* DESCRIPTION: Sets the AGC1 gain.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC1
(
    tmUnitSelect_t           tUnit,     /* I: Unit number */
    tmddTDA18250_AGC1_Gain_t agc1Gain   /* I: AGC1 gain   */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* stop AGC1 loop */
        err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            /* AGC1 frozen */
            err = tmddTDA18250_SetAGC1_gain_Force(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetAGC1_gain_Force(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gain(tUnit, agc1Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gain(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetAGC1:                                         */
/*                                                                            */
/* DESCRIPTION: Gets the AGC1 gain.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetAGC1
(
    tmUnitSelect_t            tUnit,     /* I: Unit number */
    tmddTDA18250_AGC1_Gain_t* pAgc1Gain  /* O: AGC1 gain   */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = tmddTDA18250GetAGC1_Gain(tUnit, (UInt8*)pAgc1Gain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC1_Gain(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC2b_Gain:                                   */
/*                                                                            */
/* DESCRIPTION: Sets the AGC2b gain.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       decimal value=(Gain + 12dB)/0.2, decimal value: [0..142]      */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,     /* I: Unit number    */
    UInt8           agc2Gain   /* I: AGC2 gain (dB) */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uRdAgc2Gain;
    UInt8               AGC2b = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Get and Set back the same gain to refresh internal Tuner memory */
        err = tmddTDA18250GetAGC2b_Gain(tUnit, &uRdAgc2Gain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC2b_Gain(tUnit, uRdAgc2Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            /* AGC2 frozen */
            err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            AGC2b = 5*(agc2Gain + 12);
            err = tmddTDA18250SetAGC2b_Gain(tUnit, (UInt8)AGC2b);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetAGC2b_Gain:                                   */
/*                                                                            */
/* DESCRIPTION: Gets the AGC2b gain.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Gain =(decimal value)*0.2-12 dB, decimal value: [0..142]      */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,     /* I: Unit number    */
    UInt8*          pAgc2Gain  /* O: AGC2 gain (dB) */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               RdAgc2Gain = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = tmddTDA18250GetAGC2b_Gain(tUnit, &RdAgc2Gain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));
    }
    if (err == TM_OK)
    {
        *pAgc2Gain = (RdAgc2Gain / 5) - 12;

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC2b_Raw:                                    */
/*                                                                            */
/* DESCRIPTION: Sets the AGC2b raw value                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC2b_Raw
(
    tmUnitSelect_t  tUnit,     /* I: Unit number    */
    UInt8           Agc2Raw    /* I: AGC2 gain (dB) */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uRdAgc2Gain;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {

        /* Get and Set back the same gain to refresh internal Tuner memory */
        err = tmddTDA18250GetAGC2b_Gain(tUnit, &uRdAgc2Gain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC2b_Gain(tUnit, uRdAgc2Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            /* AGC2 frozen */
            err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
             err = tmddTDA18250SetAGC2b_Gain(tUnit, Agc2Raw);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
         }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetAGC2b_Raw:                                    */
/*                                                                            */
/* DESCRIPTION: Gets the AGC2b raw value                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetAGC2b_Raw
(
    tmUnitSelect_t  tUnit,     /* I: Unit number    */
    UInt8           *pAgc2Raw    /* I: AGC2 gain (dB) */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {

        err = tmddTDA18250GetAGC2b_Gain(tUnit, pAgc2Raw);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC1Freeze:                                   */
/*                                                                            */
/* DESCRIPTION: Freezes the AGC1 gain                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC1Freeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* stop AGC1 loop */
        err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC1Unfreeze:                                 */
/*                                                                            */
/* DESCRIPTION: Unfreezes the AGC1 gain                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC1Unfreeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {

        /* AGC1 frozen */
        err = tmddTDA18250_SetAGC1_gain_Force(tUnit, tmddTDA18250_OFF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetAGC1_gain_Force(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
        /* stop AGC1 loop */
        err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC2Freeze:                                   */
/*                                                                            */
/* DESCRIPTION: Freezes the AGC2b gain                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC2Freeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uAgc2Gain;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Get and Set back the same gain to freeze AGC2 gain */
        err = tmddTDA18250GetAGC2b_Gain(tUnit, &uAgc2Gain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC2b_Gain(tUnit, uAgc2Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            /* AGC2 frozen */
            err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetAGC2Unfreeze:                                 */
/*                                                                            */
/* DESCRIPTION: Unfreezes the AGC2b gain                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetAGC2Unfreeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* AGC2 frozen */
        err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_OFF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetLoFreq:                                       */
/*                                                                            */
/* DESCRIPTION: Sets the the MAIN fractional PLL settings.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       RF (in Hz) as parameter                                       */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetLoFreq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number      */
    UInt32          uRF         /* I: Frequency in Hz  */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt32              uLO = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        uLO = uRF + pObj->Std_Array[pObj->StandardMode].IF;
        err = tmddTDA18250SetLOFreq(tUnit, uLO);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLOFreq(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetVCO:                                          */
/*                                                                            */
/* DESCRIPTION: Gets VCO in KHz                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetVCO
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          puVCO       /* O: VCO in KHz  */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uPrescTemp = 0;
    UInt8               uPostDivTemp = 0;
    UInt32              uLO = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        uLO = pObj->uRF + pObj->Std_Array[pObj->StandardMode].IF;

        /* Retrieve Prescaler and PostDiv */
        err = tmddTDA18250GetMainPLL(tUnit, uLO, &uPrescTemp, &uPostDivTemp);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetMainPLL(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            /* return VCO (in kHz) */
            *puVCO = (uLO/1000) * uPrescTemp * uPostDivTemp;
        }
        else
        {
            *puVCO = 0;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


