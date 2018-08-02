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
//-----------------------------------------------------------------------------
// $Header:
// (C) Copyright Nutune, All rights reserved
//
// This source code and any compilation or derivative thereof is the sole
// property of Nutune and is provided pursuant to a Software
// License Agreement.  This code is the proprietary information of Nutune
// and is confidential in nature.  Its use and dissemination by
// any party other than Nutune is strictly limited by the
// confidential information provisions of the Agreement referenced above.
//-----------------------------------------------------------------------------
// FILE NAME:    tmbslNT220x.c
//
// %version: 33 %
//
// DESCRIPTION:  Function for the Hybrid silicon tuner NT220x
//
// DOCUMENT REF:
//
// NOTES:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// Standard include files:
//-----------------------------------------------------------------------------
//
#include "MsCommon.h"
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

#include "tmddNT220x.h"

#ifdef TMBSL_NT220HN
 #include "tmbslNT220HN.h"
#else /* TMBSL_NT220HN */
 #include "tmbslNT220DN.h"
#endif /* TMBSL_NT220HN */

//-----------------------------------------------------------------------------
// Project include files:
//-----------------------------------------------------------------------------
//
#include "tmbslNT220xlocal.h"
#include "tmbslNT220xInstance.h"

//-----------------------------------------------------------------------------
// Types and defines:
//-----------------------------------------------------------------------------
//
static tmErrorCode_t NT220xInit(tmUnitSelect_t tUnit);
static tmErrorCode_t NT220xWait(ptmNT220xObject_t pObj, UInt32 Time);
static tmErrorCode_t NT220xWaitXtalCal_End( ptmNT220xObject_t pObj, UInt32 timeOut, UInt32 waitStep);

//-----------------------------------------------------------------------------
// Global data:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// Exported functions:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslTDA18211Init:
//
// DESCRIPTION: create an instance of a NT220x Tuner
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TM_OK
//
// NOTES:
//-----------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xInit
(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if (psSrvFunc == Null)
    {
        err = NT220x_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        //----------------------
        // initialize the Object
        //----------------------
        // pObj initialization
        err = NT220xGetInstance(tUnit, &pObj);
    }

    /* check driver state */
    if (err == TM_OK || err == NT220x_ERR_NOT_INITIALIZED)
    {
        if (pObj != Null && pObj->init == True)
        {
            err = NT220x_ERR_NOT_INITIALIZED;
        }
        else
        {
            /* initialize the Object */
            if (pObj == Null)
            {
                err = NT220xAllocInstance(tUnit, &pObj);
                if (err != TM_OK || pObj == Null)
                {
                    err = NT220x_ERR_NOT_INITIALIZED;
                }
            }

            if (err == TM_OK)
            {
                // initialize the Object by default values
                pObj->sRWFunc = psSrvFunc->sIo;
                pObj->sTime = psSrvFunc->sTime;
                pObj->sDebug = psSrvFunc->sDebug;

                if(  psSrvFunc->sMutex.Init != Null
                    && psSrvFunc->sMutex.DeInit != Null
                    && psSrvFunc->sMutex.Acquire != Null
                    && psSrvFunc->sMutex.Release != Null)
                {
                    pObj->sMutex = psSrvFunc->sMutex;

                    err = pObj->sMutex.Init(&pObj->pMutex);
                }

                pObj->init = True;
                err = TM_OK;

                err = tmddNT220xInit(tUnit, psSrvFunc);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xInit(0x%08X) failed.", pObj->tUnit));
            }
        }
    }

    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xDeInit:
//
// DESCRIPTION: destroy an instance of a NT220x Tuner
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-----------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xDeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xDeInit(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xDeInit(0x%08X) failed.", pObj->tUnit));

        (void)NT220xMutexRelease(pObj);

        if(pObj->sMutex.DeInit != Null)
        {
            if(pObj->pMutex != Null)
            {
                err = pObj->sMutex.DeInit(pObj->pMutex);
            }

            pObj->sMutex.Init = Null;
            pObj->sMutex.DeInit = Null;
            pObj->sMutex.Acquire = Null;
            pObj->sMutex.Release = Null;

            pObj->pMutex = Null;
        }
    }

    err = NT220xDeAllocInstance(tUnit);

    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xGetSWVersion:
//
// DESCRIPTION: Return the version of this device
//
// RETURN:      TM_OK
//
// NOTES:       Values defined in the tmNT220xlocal.h file
//-----------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xGetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
)
{
    tmErrorCode_t   err = NT220x_ERR_NOT_INITIALIZED;

    err = tmddNT220xGetSWVersion(pSWVersion);

    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xCheckHWVersion:
//
// DESCRIPTION: Check HW version
//
// RETURN:      TM_OK if no error
//
// NOTES:       Values defined in the tmNT220xlocal.h file
//-----------------------------------------------------------------------------
tmErrorCode_t
tmbslNT220xCheckHWVersion
(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = NT220x_ERR_NOT_INITIALIZED;
    UInt16              uIdentity = 0;
    UInt8               majorRevision = 0;

    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xGetIdentity(tUnit, &uIdentity);

        if(err == TM_OK)
        {
            if(uIdentity == 18272 || uIdentity == 18212)
            {
                /* NT220HN/12 found. Check Major Revision*/
                err = tmddNT220xGetMajorRevision(tUnit, &majorRevision);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetMajorRevision(0x%08X) failed.", tUnit));

                if(err == TM_OK && majorRevision != 1)
                {
                    /* Only NT220HN/12 ES2 are supported */
                    err = NT220x_ERR_BAD_VERSION;
                }
            }
            else
            {
                err = NT220x_ERR_BAD_VERSION;
            }
        }

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xSetPowerState:
//
// DESCRIPTION: Set the power state of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xSetPowerState
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmNT220xPowerState_t  powerState  /* I: Power state of this device */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        if(powerState > pObj->minPowerState)
        {
            powerState = pObj->minPowerState;
        }

        // Call tmddNT220xSetPowerState
        err = tmddNT220xSetPowerState(tUnit, powerState);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetPowerState(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            // set power state
            pObj->curPowerState = powerState;
        }

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xGetPowerState:
//
// DESCRIPTION: Get the power state of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xGetPowerState
(
    tmUnitSelect_t          tUnit,          /* I: Unit number */
    tmNT220xPowerState_t  *pPowerState    /* O: Power state of this device */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if(pPowerState == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        // pObj initialization
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // get power state
        *pPowerState = pObj->curPowerState;

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xSetStandardMode:
//
// DESCRIPTION: Set the standard mode of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xSetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number */
    tmNT220xStandardMode_t    StandardMode    /* I: Standard mode of this device */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // store standard mode
        pObj->StandardMode = StandardMode;

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xGetStandardMode:
//
// DESCRIPTION: Get the standard mode of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xGetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number */
    tmNT220xStandardMode_t    *pStandardMode  /* O: Standard mode of this device */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if(pStandardMode == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        // pObj initialization
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        /* Get standard mode */
        *pStandardMode = pObj->StandardMode;

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xSetRf:
//
// DESCRIPTION: Calculate i2c I2CMap & write in NT220x
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              NT220x_ERR_BAD_PARAMETER
//              TMBSL_ERR_IIC_ERR
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xSetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32          uRF     /* I: RF frequency in hertz */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;
    Bool                bIRQWait = True;
    UInt8   ratioL, ratioH;
    UInt32  DeltaL, DeltaH;
	UInt8 uAGC1_loop_off;
	UInt8 uRF_Filter_Gv = 0;

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
	{
		err = tmddNT220xGetIRQWait(tUnit, &bIRQWait);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetIRQWait(0x%08X) failed.", tUnit));

		pObj->uRF = uRF;

        if(err == TM_OK)
        {
            /* Set LPF */
            err = tmddNT220xSetLP_FC(tUnit, pObj->Std_Array[pObj->StandardMode].LPF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetLP_FC(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set LPF Offset */
            err = tmddNT220xSetLP_FC_Offset(tUnit, pObj->Std_Array[pObj->StandardMode].LPF_Offset);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetLP_FC_Offset(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set IF Gain */
            err = tmddNT220xSetIF_Level(tUnit, pObj->Std_Array[pObj->StandardMode].IF_Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIF_Level(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set IF Notch */
            err = tmddNT220xSetIF_ATSC_Notch(tUnit, pObj->Std_Array[pObj->StandardMode].IF_Notch);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIF_ATSC_Notch(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Enable/disable HPF */
            if ( pObj->Std_Array[pObj->StandardMode].IF_HPF == tmNT220x_IF_HPF_Disabled )
            {
                err = tmddNT220xSetHi_Pass(tUnit, 0);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetHi_Pass(0x%08X, 0) failed.", tUnit));
            }
            else
            {
                err = tmddNT220xSetHi_Pass(tUnit, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetHi_Pass(0x%08X, 1) failed.", tUnit));

                if(err == TM_OK)
                {
                    /* Set IF HPF */
                    err = tmddNT220xSetIF_HP_Fc(tUnit, (UInt8)(pObj->Std_Array[pObj->StandardMode].IF_HPF - 1));
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIF_HP_Fc(0x%08X) failed.", tUnit));
                }
            }
        }

        if(err == TM_OK)
        {
            /* Set DC Notch */
            err = tmddNT220xSetIF_Notch(tUnit, pObj->Std_Array[pObj->StandardMode].DC_Notch);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIF_Notch(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC1 LNA Top */
            err = tmddNT220xSetAGC1_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC1_LNA_TOP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGC1_TOP(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC2 RF Top */
            err = tmddNT220xSetAGC2_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC2_RF_Attenuator_TOP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGC2_TOP(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC3 RF AGC Top */
            if (pObj->uRF < tmNT220x_AGC3_RF_AGC_TOP_FREQ_LIM)
            {
                err = tmddNT220xSetRFAGC_Top(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_RF_AGC_TOP_Low_band);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetRFAGC_Top(0x%08X) failed.", tUnit));
            }
            else
            {
                err = tmddNT220xSetRFAGC_Top(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_RF_AGC_TOP_High_band);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetRFAGC_Top(0x%08X) failed.", tUnit));
            }
        }

        if(err == TM_OK)
        {
            /* Set AGC4 IR Mixer Top */
            err = tmddNT220xSetIR_Mixer_Top(tUnit, pObj->Std_Array[pObj->StandardMode].AGC4_IR_Mixer_TOP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIR_Mixer_Top(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC5 IF AGC Top */
            err = tmddNT220xSetAGC5_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC5_IF_AGC_TOP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGC5_TOP(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC3 Adapt */
            err = tmddNT220xSetPD_RFAGC_Adapt(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_Adapt);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetPD_RFAGC_Adapt(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC3 Adapt TOP */
            err = tmddNT220xSetRFAGC_Adapt_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_Adapt_TOP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetRFAGC_Adapt_TOP(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC5 Atten 3dB */
            err = tmddNT220xSetRF_Atten_3dB(tUnit, pObj->Std_Array[pObj->StandardMode].AGC5_Atten_3dB);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetRF_Atten_3dB(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGC5 Detector HPF */
            err = tmddNT220xSetAGC5_Ana(tUnit, pObj->Std_Array[pObj->StandardMode].AGC5_Detector_HPF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGC5_Ana(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set AGCK Mode */
            err = tmddNT220xSetAGCK_Mode(tUnit, pObj->Std_Array[pObj->StandardMode].GSK&0x03);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGCK_Mode(0x%08X) failed.", tUnit));

            err = tmddNT220xSetAGCK_Step(tUnit, (pObj->Std_Array[pObj->StandardMode].GSK&0x0C)>>2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGCK_Step(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set H3H5 VHF Filter 6 */
            err = tmddNT220xSetPSM_StoB(tUnit, pObj->Std_Array[pObj->StandardMode].H3H5_VHF_Filter6);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetPSM_StoB(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            /* Set IF */
            err = tmddNT220xSetIF_Freq(tUnit, pObj->Std_Array[pObj->StandardMode].IF - pObj->Std_Array[pObj->StandardMode].CF_Offset);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIF_Freq(0x%08X) failed.", tUnit));
        }
		if (pObj->Std_Array[pObj->StandardMode].LTO_STO_immune && pObj->Master )
		{
			/* save RF_Filter_Gv current value */
			if(err == TM_OK)
			{
				err = tmddNT220xGetAGC2_Gain_Read (tUnit, &uRF_Filter_Gv);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetRF_Filter_Gv(0x%08X) failed.", tUnit));
			}
			if(err == TM_OK)
			{
				err = tmddNT220xSetRF_Filter_Gv(tUnit, uRF_Filter_Gv );
			}
			if(err == TM_OK)
			{
				err = tmddNT220xSetForce_AGC2_gain(tUnit, 0x1);
			}
			/* smooth RF_Filter_Gv to min value */
			if((err == TM_OK)&&(uRF_Filter_Gv != 0))
			{
				do
				{
					err = tmddNT220xSetRF_Filter_Gv(tUnit, uRF_Filter_Gv -1 );
					tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetRF_Filter_Gv(0x%08X) failed.", tUnit));
					if(err == TM_OK)
					{
						err = NT220xWait(pObj, 10);
						tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetRF_Filter_Cap(0x%08X) failed.", tUnit));
					}
					uRF_Filter_Gv = uRF_Filter_Gv -1 ;
				}
				while ( uRF_Filter_Gv > 0);
			}
			if(err == TM_OK)
			{
				err = tmddNT220xSetRF_Atten_3dB (tUnit, 0x01);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetRF_Filter_Gv(0x%08X) failed.", tUnit));
			}
		}
        if(err == TM_OK)
        {
            /* Set RF */
            err = tmddNT220xSetRF_Freq(tUnit, uRF + pObj->Std_Array[pObj->StandardMode].CF_Offset);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetRF_Freq(0x%08X) failed.", tUnit));

        }

		if (pObj->Std_Array[pObj->StandardMode].LTO_STO_immune && pObj->Master )
		{

			if(err == TM_OK)
			{
				err = tmddNT220xSetRF_Atten_3dB (tUnit, 0x00);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetRF_Filter_Gv(0x%08X) failed.", tUnit));
			}
			err = NT220xWait(pObj, 50);
			if(err == TM_OK)
			{
				tmddNT220xSetForce_AGC2_gain(tUnit, 0x0);
			}
		}

        if(err == TM_OK)
        {
            /*  Spurious reduction begin */
            ratioL = (UInt8)(uRF / 16000000);
            ratioH = (UInt8)(uRF / 16000000) + 1;
            DeltaL = (uRF - (ratioL * 16000000));
            DeltaH = ((ratioH * 16000000) - uRF);

            if (uRF < 72000000 )
            {
                /* Set sigma delta clock*/
                err = tmddNT220xSetDigital_Clock_Mode(tUnit, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, sigma delta clock) failed.", tUnit));
            }
            else if (uRF < 104000000 )
            {
                /* Set 16 Mhz Xtal clock */
                err = tmddNT220xSetDigital_Clock_Mode(tUnit, 0);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, 16 Mhz xtal clock) failed.", tUnit));
            }
            else if (uRF <= 120000000 )
            {
                /* Set sigma delta clock*/
                err = tmddNT220xSetDigital_Clock_Mode(tUnit, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, sigma delta clock) failed.", tUnit));
            }
            else  /* RF above 120 MHz */
            {
                if  (DeltaL <= DeltaH )
                {
                    if (ratioL & 0x000001 )  /* ratioL odd */
                    {
                        /* Set 16 Mhz Xtal clock */
                        err = tmddNT220xSetDigital_Clock_Mode(tUnit, 0);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, 16 Mhz xtal clock) failed.", tUnit));
                    }
                    else /* ratioL even */
                    {
                        /* Set sigma delta clock*/
                        err = tmddNT220xSetDigital_Clock_Mode(tUnit, 1);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, sigma delta clock) failed.", tUnit));
                    }

                }
                else  /* (DeltaL > DeltaH ) */
                {
                    if (ratioL & 0x000001 )  /*(ratioL odd)*/
                    {
                        /* Set sigma delta clock*/
                        err = tmddNT220xSetDigital_Clock_Mode(tUnit, 1);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, sigma delta clock) failed.", tUnit));
                    }
                    else
                    {
                        /* Set 16 Mhz Xtal clock */
                        err = tmddNT220xSetDigital_Clock_Mode(tUnit, 0);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetDigital_Clock_Mode(0x%08X, 16 Mhz xtal clock) failed.", tUnit));
                    }
                }
            }
		}
        /*  Spurious reduction end */

		if(err == TM_OK)
		{
			if ( pObj->Std_Array[pObj->StandardMode].AGC1_Freeze == True )
			{
				err  =  tmddNT220xGetAGC1_loop_off(tUnit, &uAGC1_loop_off) ;

				if (uAGC1_loop_off == 0) // first AGC1 freeze
				{
					err  =  tmddNT220xSetAGC1_loop_off(tUnit, 0x1 ) ;
					if(err == TM_OK)
					{
						err = tmddNT220xSetForce_AGC1_gain(tUnit, 0x1);
					}
				}
				if(err == TM_OK)
				{
					err = tmddNT220xAGC1_Adapt (tUnit); /* Adapt AGC1gain from Last SetRF */
				}
			}
			else
			{
				err = tmddNT220xSetForce_AGC1_gain(tUnit, 0x0);
				if(err == TM_OK)
				{
					err  =  tmddNT220xSetAGC1_loop_off(tUnit, 0x0 ) ;
				}
			}
		}

        (void)NT220xMutexRelease(pObj);
	}

    return err;
}


//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslNT220xGetRf:
//
// DESCRIPTION: Get the frequency programmed in the tuner
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              NT220x_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:       The value returned is the one stored in the Object
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslNT220xGetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puRF    /* O: RF frequency in hertz */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if(puRF == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        /* Get RF */
        *puRF = pObj->uRF/* - pObj->Std_Array[pObj->StandardMode].CF_Offset*/;

        (void)NT220xMutexRelease(pObj);
    }
    return err;
}

/*============================================================================*/
/* tmbslNT220xReset                                                         */
/*============================================================================*/
tmErrorCode_t
tmbslNT220xReset
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    Bool                bIRQWait = True;

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xGetIRQWait(tUnit, &bIRQWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetIRQWait(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            err = NT220xInit(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xInit(0x%08X) failed.", tUnit));
        }
        if(err == TM_OK)
        {
            //  Wait for  XtalCal End ( Master only )
            if (pObj->Master)
            {
                err = NT220xWaitXtalCal_End ( pObj, 100, 5);
            }
            else
            {
                /* Initialize Fmax_LO and N_CP_Current */
                err = tmddNT220xSetFmax_Lo(tUnit, 0x00);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetFmax_Lo(0x%08X, 0x0A) failed.", tUnit));

                if(err == TM_OK)
                {
                    err = tmddNT220xSetN_CP_Current(tUnit, 0x68);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetN_CP_Current(0x%08X, 0x68) failed.", tUnit));
                }
            }
        }
        if(err == TM_OK)
        {
            // initialize tuner
            err =  tmddNT220xReset(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xReset(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* Initialize Fmax_LO and N_CP_Current */
            err = tmddNT220xSetFmax_Lo(tUnit, 0x0A);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetFmax_Lo(0x%08X, 0x0A) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddNT220xSetLT_Enable(tUnit, pObj->LT_Enable );
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetLT_Enable(0x%08X, 0) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddNT220xSetPSM_AGC1(tUnit, pObj->PSM_AGC1 );
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetPSM_AGC1(0x%08X, 1) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddNT220xSetAGC1_6_15dB(tUnit, pObj->AGC1_6_15dB );
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetAGC1_6_15dB(0x%08X, 0) failed.", tUnit));
        }

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xGetIF                                                         */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetIF
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(puIF == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }


    if(err == TM_OK)
    {

        printf("IF = 0x%lx, CF_Offset = 0x%lx \n",pObj->Std_Array[pObj->StandardMode].IF,pObj->Std_Array[pObj->StandardMode].CF_Offset);
        *puIF = pObj->Std_Array[pObj->StandardMode].IF - pObj->Std_Array[pObj->StandardMode].CF_Offset;

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xGetCF_Offset                                                  */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetCF_Offset(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32*         puOffset    /* O: Center frequency offset in hertz */
    )
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(puOffset == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *puOffset = pObj->Std_Array[pObj->StandardMode].CF_Offset;

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xGetLockStatus                                                 */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetLockStatus
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uValue, uValueLO;

    if( pLockStatus == Null )
    {
        err = NT220x_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);

        if(err == TM_OK)
        {
            err =  tmddNT220xGetLO_Lock(tUnit, &uValueLO);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetLO_Lock(0x%08X) failed.", tUnit));
        }
        if(err == TM_OK)
        {
            err =  tmddNT220xGetIRQ_status(tUnit, &uValue);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetIRQ_status(0x%08X) failed.", tUnit));

            uValue = uValue & uValueLO;
        }
        if(err == TM_OK)
        {
            *pLockStatus =  (uValue)? tmbslFrontEndStateLocked : tmbslFrontEndStateNotLocked;
        }
        else
        {
            *pLockStatus = tmbslFrontEndStateUnknown;
        }

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xGetPowerLevel                                                 */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetPowerLevel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32*         pPowerLevel /* O: Power Level in dBµV */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pPowerLevel == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pPowerLevel = 0;

        err = tmddNT220xGetPower_Level(tUnit, (UInt8 *)pPowerLevel);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetPower_Level(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }
    return err;
}

/*============================================================================*/
/* tmbslNT220xSetIRQWait                                                  */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xSetIRQWait
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    Bool            bWait   /* I: Determine if we need to wait IRQ in driver functions */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xSetIRQWait(tUnit, bWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xSetIRQWait(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }
    return err;
}

/*============================================================================*/
/* tmbslNT220xGetIRQWait                                                  */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetIRQWait
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    Bool*           pbWait  /* O: Determine if we need to wait IRQ in driver functions */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pbWait == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xGetIRQWait(tUnit, pbWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetIRQWait(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xGetIRQ                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetIRQ
(
    tmUnitSelect_t  tUnit  /* I: Unit number */,
    Bool*           pbIRQ  /* O: IRQ triggered */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pbIRQ == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pbIRQ = 0;

        err = tmddNT220xGetIRQ_status(tUnit, (UInt8 *)pbIRQ);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetIRQ_status(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xWaitIRQ                                                       */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xWaitIRQ
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          timeOut,    /* I: timeOut for IRQ wait */
    UInt32          waitStep,   /* I: wait step */
    UInt8           irqStatus   /* I: IRQs to wait */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xWaitIRQ(tUnit, timeOut, waitStep, irqStatus);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xWaitIRQ(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xGetXtalCal_End                                                */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xGetXtalCal_End
(
    tmUnitSelect_t  tUnit  /* I: Unit number */,
    Bool*           pbXtalCal_End  /* O: XtalCal_End triggered */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pbXtalCal_End == Null)
        err = NT220x_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pbXtalCal_End = 0;

        err = tmddNT220xGetMSM_XtalCal_End(tUnit, (UInt8 *)pbXtalCal_End);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetMSM_XtalCal_End(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xWaitXtalCal_End                                               */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xWaitXtalCal_End
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          timeOut,    /* I: timeOut for IRQ wait */
    UInt32          waitStep   /* I: wait step */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddNT220xWaitXtalCal_End(tUnit, timeOut, waitStep);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xWaitXtalCal_End(0x%08X) failed.", tUnit));

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}
/*============================================================================*/
/* tmbslNT220xCheckRFFilterRobustness                                       */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xCheckRFFilterRobustness
(
 tmUnitSelect_t                         tUnit,      /* I: Unit number */
 ptmNT220xRFFilterRating              rating      /* O: RF Filter rating */
 )
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = NT220xGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        UInt8  rfcal_log_0 = 0;
        UInt8  rfcal_log_2 = 0;
        UInt8  rfcal_log_3 = 0;
        UInt8  rfcal_log_5 = 0;
        UInt8  rfcal_log_6 = 0;
        UInt8  rfcal_log_8 = 0;
        UInt8  rfcal_log_9 = 0;
        UInt8  rfcal_log_11 = 0;

        double   VHFLow_0 = 0.0;
        double   VHFLow_1 = 0.0;
        double   VHFHigh_0 = 0.0;
        double   VHFHigh_1 = 0.0;
        double   UHFLow_0 = 0.0;
        double   UHFLow_1 = 0.0;
        double   UHFHigh_0 = 0.0;
        double   UHFHigh_1 = 0.0;

        err = tmddNT220xGetrfcal_log_0(tUnit, &rfcal_log_0);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_0(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_2(tUnit, &rfcal_log_2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_2(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_3(tUnit, &rfcal_log_3);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_3(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_5(tUnit, &rfcal_log_5);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_5(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_6(tUnit, &rfcal_log_6);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_6(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_8(tUnit, &rfcal_log_8);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_8(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_9(tUnit, &rfcal_log_9);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_9(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddNT220xGetrfcal_log_11(tUnit, &rfcal_log_11);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddNT220xGetrfcal_log_11(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            if (rfcal_log_0 & 0x80)
            {
                rating->VHFLow_0_Margin = 0;
                rating->VHFLow_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // VHFLow_0
                VHFLow_0 = 100 * (45 - 39.8225 * (1 + (0.31 * (rfcal_log_0 < 64 ? rfcal_log_0 : rfcal_log_0 - 128)) / 1.0 / 100.0)) / 45.0;
                rating->VHFLow_0_Margin = 0.0024 * VHFLow_0 * VHFLow_0 * VHFLow_0 - 0.101 * VHFLow_0 * VHFLow_0 + 1.629 * VHFLow_0 + 1.8266;
                if (rating->VHFLow_0_Margin >= 0)
                {
                    rating->VHFLow_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->VHFLow_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_2 & 0x80)
            {
                rating->VHFLow_1_Margin = 0;
                rating->VHFLow_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // VHFLow_1
                VHFLow_1 = 100 * (152.1828 * (1 + (1.53 * (rfcal_log_2 < 64 ? rfcal_log_2 : rfcal_log_2 - 128)) / 1.0 / 100.0) - (144.896 - 6)) / (144.896 - 6);
                rating->VHFLow_1_Margin = 0.0024 * VHFLow_1 * VHFLow_1 * VHFLow_1 - 0.101 * VHFLow_1 * VHFLow_1 + 1.629 * VHFLow_1 + 1.8266;
                if (rating->VHFLow_1_Margin >= 0)
                {
                    rating->VHFLow_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->VHFLow_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_3 & 0x80)
            {
                rating->VHFHigh_0_Margin = 0;
                rating->VHFHigh_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // VHFHigh_0
                VHFHigh_0 = 100 * ((144.896 + 6) - 135.4063 * (1 + (0.27 * (rfcal_log_3 < 64 ? rfcal_log_3 : rfcal_log_3 - 128)) / 1.0 / 100.0)) / (144.896 + 6);
                rating->VHFHigh_0_Margin = 0.0024 * VHFHigh_0 * VHFHigh_0 * VHFHigh_0 - 0.101 * VHFHigh_0 * VHFHigh_0 + 1.629 * VHFHigh_0 + 1.8266;
                if (rating->VHFHigh_0_Margin >= 0)
                {
                    rating->VHFHigh_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->VHFHigh_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_5 & 0x80)
            {
                rating->VHFHigh_1_Margin = 0;
                rating->VHFHigh_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // VHFHigh_1
                VHFHigh_1 = 100 * (383.1455 * (1 + (0.91 * (rfcal_log_5 < 64 ? rfcal_log_5 : rfcal_log_5 - 128)) / 1.0 / 100.0) - (367.104 - 8)) / (367.104 - 8);
                rating->VHFHigh_1_Margin = 0.0024 * VHFHigh_1 * VHFHigh_1 * VHFHigh_1 - 0.101 * VHFHigh_1 * VHFHigh_1 + 1.629 * VHFHigh_1 + 1.8266;
                if (rating->VHFHigh_1_Margin >= 0)
                {
                    rating->VHFHigh_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->VHFHigh_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_6 & 0x80)
            {
                rating->UHFLow_0_Margin = 0;
                rating->UHFLow_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // UHFLow_0
                UHFLow_0 = 100 * ((367.104 + 8) - 342.6224 * (1 + (0.21 * (rfcal_log_6 < 64 ? rfcal_log_6 : rfcal_log_6 - 128)) / 1.0 / 100.0)) / (367.104 + 8);
                rating->UHFLow_0_Margin = 0.0024 * UHFLow_0 * UHFLow_0 * UHFLow_0 - 0.101 * UHFLow_0 * UHFLow_0 + 1.629 * UHFLow_0 + 1.8266;
                if (rating->UHFLow_0_Margin >= 0)
                {
                    rating->UHFLow_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->UHFLow_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_8 & 0x80)
            {
                rating->UHFLow_1_Margin = 0;
                rating->UHFLow_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // UHFLow_1
                UHFLow_1 = 100 * (662.5595 * (1 + (0.33 * (rfcal_log_8 < 64 ? rfcal_log_8 : rfcal_log_8 - 128)) / 1.0 / 100.0) - (624.128 - 2)) / (624.128 - 2);
                rating->UHFLow_1_Margin = 0.0024 * UHFLow_1 * UHFLow_1 * UHFLow_1 - 0.101 * UHFLow_1 * UHFLow_1 + 1.629 * UHFLow_1 + 1.8266;
                if (rating->UHFLow_1_Margin >= 0)
                {
                    rating->UHFLow_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->UHFLow_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_9 & 0x80)
            {
                rating->UHFHigh_0_Margin = 0;
                rating->UHFHigh_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // UHFHigh_0
                UHFHigh_0 = 100 * ((624.128 + 2) - 508.2747 * (1 + (0.23 * (rfcal_log_9 < 64 ? rfcal_log_9 : rfcal_log_9 - 128)) / 1.0 / 100.0)) / (624.128 + 2);
                rating->UHFHigh_0_Margin = 0.0024 * UHFHigh_0 * UHFHigh_0 * UHFHigh_0 - 0.101 * UHFHigh_0 * UHFHigh_0 + 1.629 * UHFHigh_0 + 1.8266;
                if (rating->UHFHigh_0_Margin >= 0)
                {
                    rating->UHFHigh_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->UHFHigh_0_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }

            if (rfcal_log_11 & 0x80)
            {
                rating->UHFHigh_1_Margin = 0;
                rating->UHFHigh_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Error;
            }
            else
            {
                // UHFHigh_1
                UHFHigh_1 = 100 * (947.8913 * (1 + (0.3 * (rfcal_log_11 < 64 ? rfcal_log_11 : rfcal_log_11 - 128)) / 1.0 / 100.0) - (866 - 14)) / (866 - 14);
                rating->UHFHigh_1_Margin = 0.0024 * UHFHigh_1 * UHFHigh_1 * UHFHigh_1 - 0.101 * UHFHigh_1 * UHFHigh_1 + 1.629 * UHFHigh_1 + 1.8266;
                if (rating->UHFHigh_1_Margin >= 0)
                {
                    rating->UHFHigh_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_High;
                }
                else
                {
                    rating->UHFHigh_1_RFFilterRobustness =  tmNT220xRFFilterRobustness_Low;
                }
            }
        }

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslNT220xWrite                                                         */
/*============================================================================*/

tmErrorCode_t
tmbslNT220xWrite
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          uIndex,     /* I: Start index to write */
    UInt32          WriteLen,   /* I: Number of bytes to write */
    UInt8*          pData       /* I: Data to write */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = NT220xMutexAcquire(pObj, NT220x_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // Call tmddNT220xWrite

        (void)NT220xMutexRelease(pObj);
    }

    return err;
}
//-----------------------------------------------------------------------------
// Internal functions:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// FUNCTION:    NT220xInit:
//
// DESCRIPTION: initialization of the Tuner
//
// RETURN:      always True
//
// NOTES:
//-----------------------------------------------------------------------------
//
static tmErrorCode_t
NT220xInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmNT220xObject_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    err = NT220xGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "NT220xGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        //err = tmddNT220xSetIRQWait(tUnit, True);

        //if(pObj->bIRQWait)
        //{
        //    err = NT220xWaitIRQ(pObj);
        //}
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    NT220xWait
//
// DESCRIPTION: This function waits for requested time
//
// RETURN:      True or False
//
// NOTES:
//-------------------------------------------------------------------------------------
//
static tmErrorCode_t
NT220xWait
(
    ptmNT220xObject_t pObj,   /* I: Driver object */
    UInt32              Time   /*  I: Time to wait for */
)
{
    tmErrorCode_t   err  = TM_OK;

    // wait Time ms
    err = POBJ_SRVFUNC_STIME.Wait(pObj->tUnit, Time);

    // Return value
    return err;
}

/*============================================================================*/
/* NT220xMutexAcquire                                                       */
/*============================================================================*/
extern tmErrorCode_t
NT220xMutexAcquire
(
 ptmNT220xObject_t    pObj,
 UInt32                 timeOut
 )
{
    tmErrorCode_t       err = TM_OK;

    if(pObj->sMutex.Acquire != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Acquire(pObj->pMutex, timeOut);
    }

    return err;
}

/*============================================================================*/
/* NT220xMutexRelease                                                       */
/*============================================================================*/
extern tmErrorCode_t
NT220xMutexRelease
(
 ptmNT220xObject_t    pObj
 )
{
    tmErrorCode_t       err = TM_OK;

    if(pObj->sMutex.Release != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Release(pObj->pMutex);
    }

    return err;
}
/*============================================================================*/
/* FUNCTION:    ddNT220xWaitXtalCal_End                                     */
/*                                                                            */
/* DESCRIPTION: Wait for MSM_XtalCal_End to trigger                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
NT220xWaitXtalCal_End
(
    ptmNT220xObject_t   pObj,       /* I: Instance object */
    UInt32                  timeOut,    /* I: timeout */
    UInt32                  waitStep    /* I: wait step */
)
{
    tmErrorCode_t   err  = TM_OK;
    UInt32          counter = timeOut/waitStep; /* Wait max timeOut/waitStepms */
    UInt8           uMSM_XtalCal_End = 0;

    while(err == TM_OK && (--counter)>0)
    {
        err = tmddNT220xGetMSM_XtalCal_End(pObj->tUnit, &uMSM_XtalCal_End);

        if(uMSM_XtalCal_End == 1)
        {
            /* MSM_XtalCal_End triggered => Exit */
            break;
        }

        NT220xWait(pObj, waitStep);
    }

    if(counter == 0)
    {
        err = ddNT220x_ERR_NOT_READY;
    }

    return err;
}
