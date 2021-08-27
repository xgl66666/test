//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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
#include "Board.h"
#if(FRONTEND_TUNER_TYPE == TUNER_TDA18260)

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
#include "tmddTDA18250.h"

#include "tmbslTDA18250.h"
#include <tmbslTDA18250InstanceCustom.h>
#include "tmbslTDA18250local.h"
#include "tmbslTDA18250Instance.h"
#include "tmbslTDA18250AdvanceCustom.h"


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
            err = tmddTDA18250_SetAGC1_gain_Force(tUnit, tmddTDA18250_AGC_Mode_forced);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetAGC1_gain_Force(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gain(tUnit, (UInt8)agc1Gain);
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

        /* AGC1 free */
        err = tmddTDA18250_SetAGC1_gain_Force(tUnit, tmddTDA18250_AGC_Mode_Free);
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

#endif

