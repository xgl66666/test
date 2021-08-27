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
 * \file          System_Manage_HPMTO.c
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
#if(FRONTEND_TUNER_TYPE == TUNER_TDA18250A)

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmUnitParams.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "System_Manage_HPMTO.h"
#include "tmddTDA18250.h"
#include "tmbslTDA18250.h"
#include <tmbslTDA18250InstanceCustom.h>
#include "tmbslTDA18250local.h"


/*============================================================================*/
/* FUNCTION:    tmSystem_Manage_HPMTO:                                        */
/*                                                                            */
/* DESCRIPTION: Set the HP MTO on the Master if Slave is analogue             */
/*              and its RF is superior to 250 Mhz                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t tmSystem_Manage_HPMTO
(
    tmUnitSelect_t  TunerUnit   /* I: Tuner unit number */
)
{
    tmErrorCode_t               err  = TM_OK;
    UInt32                      uIndex0,uIndex2,uIndex3,uIndex;
    UInt32                      uRF;
    tmTDA18250StandardMode_t    StandardMode;
    UInt32                      TunerType;

    TunerType = UNIT_PATH_TYPE_GET (TunerUnit);
    uIndex0 = GET_INDEX_TUNIT(0)|UNIT_PATH_TYPE_VAL(TunerType);
    uIndex2 = GET_INDEX_TUNIT(2)|UNIT_PATH_TYPE_VAL(TunerType);
    uIndex3 = GET_INDEX_TUNIT(3)|UNIT_PATH_TYPE_VAL(TunerType);

    /* stop AGC1 loop */
    if (err == TM_OK)
    {
        err = TDA18250SetAGC1_loop_off(uIndex0, tmTDA18250_ON);
    }

    /* Manage HP MTO on Master if SEtRF for Slave2&3 in analogue with RF >250 MHz */
    if (err == TM_OK)
    {
        for (uIndex=uIndex2; uIndex<=uIndex3; uIndex++)
        {
            err = tmbslTDA18250GetRf (uIndex,&uRF);

            if (err == TM_OK)
            {
                err = tmbslTDA18250GetStandardMode (uIndex,&StandardMode);
            }

            if (err == TM_OK)
            {
                if ( (StandardMode<=tmTDA18250_DIG_9MHz) && (uRF >= 250000000) )
                {
                    err = TDA18250SetMTOClose(uIndex0, (UInt8) GET_INDEX_TUNIT(uIndex) );
                }
                else
                {
                    err = TDA18250SetMTOOpen(uIndex0, (UInt8) GET_INDEX_TUNIT(uIndex) );
                }
            }
        }
    }

    /* restart AGC1 loop */
    if (err == TM_OK)
    {
        err = TDA18250SetAGC1_loop_off(uIndex0, tmTDA18250_OFF);
    }

    return err;
}
#endif

