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
/**
// (C) Copyright Nutune, All rights reserved
//
// This source code and any compilation or derivative thereof is the sole
// property of Nutune and is provided pursuant to a Software
// License Agreement.  This code is the proprietary information of Nutune
// and is confidential in nature.  Its use and dissemination by
// any party other than Nutune is strictly limited by the
// confidential information provisions of the Agreement referenced above.
*
* \file          tmddNT220x.h
*                %version: 11 %
*
* \date          %date_modified%
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*
* Detailed description may be added here.
*
* \section info Change Information
*
* \verbatim
Date          Modified by CRPRNr  TASKNr  Maintenance description
-------------|-----------|-------|-------|-----------------------------------
|            |           |       |
-------------|-----------|-------|-------|-----------------------------------
|            |           |       |
-------------|-----------|-------|-------|-----------------------------------
\endverbatim
*
*/
#ifndef _TMDD_NT220x_H //-----------------
#define _TMDD_NT220x_H

//-----------------------------------------------------------------------------
// Standard include files:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// Project include files:
//-----------------------------------------------------------------------------
//

#ifdef __cplusplus
extern "C"
{
#endif

    //-----------------------------------------------------------------------------
    // Types and defines:
    //-----------------------------------------------------------------------------
    //

    /* SW Error codes */
#define ddNT220x_ERR_BASE               (CID_COMP_TUNER | CID_LAYER_BSL)
#define ddNT220x_ERR_COMP               (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define ddNT220x_ERR_BAD_UNIT_NUMBER    (ddNT220x_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define ddNT220x_ERR_NOT_INITIALIZED    (ddNT220x_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define ddNT220x_ERR_INIT_FAILED        (ddNT220x_ERR_BASE + TM_ERR_INIT_FAILED)
#define ddNT220x_ERR_BAD_PARAMETER      (ddNT220x_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define ddNT220x_ERR_NOT_SUPPORTED      (ddNT220x_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define ddNT220x_ERR_HW_FAILED          (ddNT220x_ERR_COMP + 0x0001)
#define ddNT220x_ERR_NOT_READY          (ddNT220x_ERR_COMP + 0x0002)
#define ddNT220x_ERR_BAD_VERSION        (ddNT220x_ERR_COMP + 0x0003)


    typedef enum _tmddNT220xPowerState_t {
        tmddNT220x_PowerNormalMode,                                               /* Device normal mode */
        tmddNT220x_PowerStandbyWithLNAOnAndWithXtalOnAndWithSyntheOn,             /* Device standby mode with LNA and Xtal Output and Synthe on*/
        tmddNT220x_PowerStandbyWithLNAOnAndWithXtalOn,                            /* Device standby mode with LNA and Xtal Output */
        tmddNT220x_PowerStandbyWithXtalOn,                                        /* Device standby mode with Xtal Output */
        tmddNT220x_PowerStandby,                                                  /* Device standby mode */
        tmddNT220x_PowerMax
    } tmddNT220xPowerState_t, *ptmddNT220xPowerState_t;

    typedef enum _tmddNT220x_LPF_Gain_t {
        tmddNT220x_LPF_Gain_Unknown = 0,                                          /* LPF_Gain Unknown */
        tmddNT220x_LPF_Gain_Frozen,                                               /* LPF_Gain Frozen */
        tmddNT220x_LPF_Gain_Free                                                  /* LPF_Gain Free */
    } tmddNT220x_LPF_Gain_t, *ptmddNT220x_LPF_Gain_t;

    tmErrorCode_t
    tmddNT220xInit
    (
    tmUnitSelect_t    tUnit,    //  I: Unit number
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
    );
    tmErrorCode_t 
    tmddNT220xDeInit
    (
    tmUnitSelect_t    tUnit     //  I: Unit number
    );
    tmErrorCode_t
    tmddNT220xGetSWVersion
    (
    ptmSWVersion_t    pSWVersion        //  I: Receives SW Version 
    );
    tmErrorCode_t
    tmddNT220xReset
    (
    tmUnitSelect_t  tUnit     //  I: Unit number
    );
    tmErrorCode_t
    tmddNT220xSetLPF_Gain_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uMode   /* I: Unknown/Free/Frozen */
    );
    tmErrorCode_t
    tmddNT220xGetLPF_Gain_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           *puMode /* O: Unknown/Free/Frozen */
    );
    tmErrorCode_t
    tmddNT220xWrite
    (
    tmUnitSelect_t      tUnit,      //  I: Unit number
    UInt32              uIndex,         //  I: Start index to write
    UInt32              uNbBytes,       //  I: Number of bytes to write
    UInt8*             puBytes         //  I: Pointer on an array of bytes
    );
    tmErrorCode_t
    tmddNT220xRead
    (
    tmUnitSelect_t      tUnit,      //  I: Unit number
    UInt32              uIndex,         //  I: Start index to read
    UInt32              uNbBytes,       //  I: Number of bytes to read
    UInt8*             puBytes         //  I: Pointer on an array of bytes
    );
    tmErrorCode_t
    tmddNT220xGetPOR
    (
    tmUnitSelect_t    tUnit,    //  I: Unit number
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetLO_Lock
    (
    tmUnitSelect_t    tUnit,    //  I: Unit number
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMS
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetIdentity
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt16*        puValue      //  I: Address of the variable to output item value
    );
    tmErrorCode_t
    tmddNT220xGetMinorRevision
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMajorRevision
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetTM_D
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetTM_ON
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetTM_ON
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPowerState
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    tmddNT220xPowerState_t  powerState    //  I: Power state of this device
    );
    tmErrorCode_t
    tmddNT220xGetPowerState
    (
    tmUnitSelect_t        tUnit,    //  I: Unit number
    ptmddNT220xPowerState_t    pPowerState  //  O: Power state of this device
    );

    tmErrorCode_t
    tmddNT220xGetPower_Level
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIRQ_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIRQ_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetXtalCal_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetXtalCal_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RSSI_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RSSI_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_LOCalc_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_LOCalc_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RFCAL_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RFCAL_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_IRCAL_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_IRCAL_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RCCal_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RCCal_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIRQ_Clear
    (
        tmUnitSelect_t  tUnit,      /* I: Unit number */
        UInt8           irqStatus   /* I: IRQs to clear */
    );
    tmErrorCode_t
    tmddNT220xSetXtalCal_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetXtalCal_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RSSI_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RSSI_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_LOCalc_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_LOCalc_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RFCal_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RFCal_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_IRCAL_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_IRCAL_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RCCal_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RCCal_Clear
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIRQ_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIRQ_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetXtalCal_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetXtalCal_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RSSI_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RSSI_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_LOCalc_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_LOCalc_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RFCal_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RFCal_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_IRCAL_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_IRCAL_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_RCCal_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RCCal_Set
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetIRQ_status
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_XtalCal_End
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RSSI_End
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_LOCalc_End
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RFCal_End
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_IRCAL_End
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetMSM_RCCal_End
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetLT_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetLT_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC1_6_15dB
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetAGC1_6_15dB
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC1_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC1_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC2_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC2_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGCs_Up_Step_assym
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetAGCs_Up_Step_assym
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGCs_Up_Step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetAGCs_Up_Step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPulse_Shaper_Disable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPulse_Shaper_Disable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGCK_Step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGCK_Step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGCK_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGCK_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPD_RFAGC_Adapt
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetPD_RFAGC_Adapt
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFAGC_Adapt_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFAGC_Adapt_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFAGC_Low_BW
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFAGC_Low_BW
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_Atten_3dB
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRF_Atten_3dB
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFAGC_Top
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFAGC_Top
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Mixer_Top
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Mixer_Top
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGCs_Do_Step_assym
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetAGCs_Do_Step_assym
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC5_Ana
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC5_Ana
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC5_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC5_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIF_Level
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIF_Level
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIF_HP_Fc
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIF_HP_Fc
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIF_ATSC_Notch
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIF_ATSC_Notch
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetLP_FC_Offset
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetLP_FC_Offset
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetLP_FC
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetLP_FC
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetI2C_Clock_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetI2C_Clock_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetDigital_Clock_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetDigital_Clock_Mode
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetXtalOsc_AnaReg_En
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetXtalOsc_AnaReg_En
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetXTout
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetXTout
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIF_Freq
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32          uValue      //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetIF_Freq
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*        puValue      //  I: Address of the variable to output item value
    );
    tmErrorCode_t
    tmddNT220xSetRF_Freq
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32          uValue      //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRF_Freq
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*        puValue      //  I: Address of the variable to output item value
    );
    tmErrorCode_t
    tmddNT220xSetRSSI_Meas
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRSSI_Meas
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_CAL_AV
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_CAL_AV
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_CAL
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_CAL
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_CAL_Loop
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_CAL_Loop
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Cal_Image
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Cal_Image
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_CAL_Wanted
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_CAL_Wanted
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRC_Cal
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRC_Cal
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetCalc_PLL
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetCalc_PLL
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetXtalCal_Launch
    (
    tmUnitSelect_t      tUnit    //  I: Unit number
    );
    tmErrorCode_t
    tmddNT220xGetXtalCal_Launch
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetMSM_Launch
    (
    tmUnitSelect_t      tUnit    //  I: Unit number
    );
    tmErrorCode_t
    tmddNT220xGetMSM_Launch
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPSM_AGC1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPSM_AGC1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPSM_StoB
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPSM_StoB
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPSMRFpoly
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPSMRFpoly
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPSM_Mixer
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPSM_Mixer
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPSM_Ifpoly
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPSM_Ifpoly
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPSM_Lodriver
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetPSM_Lodriver
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetDCC_Bypass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetDCC_Bypass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetDCC_Slow
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetDCC_Slow
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetDCC_psm
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetDCC_psm
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetFmax_Lo
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetFmax_Lo
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Loop
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Loop
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Target
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Target
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_GStep
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_GStep
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Corr_Boost
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Corr_Boost
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_FreqLow_Sel
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_FreqLow_Sel
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_mode_ram_store
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetIR_mode_ram_store
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_FreqLow
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_FreqLow
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_FreqMid
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_FreqMid
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetCoarse_IR_FreqHigh
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetCoarse_IR_FreqHigh
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_FreqHigh
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_FreqHigh
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );

    tmErrorCode_t
    tmddNT220xSetPD_Vsync_Mgt
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetPD_Vsync_Mgt
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPD_Ovld
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetPD_Ovld
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetPD_Udld
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetPD_Udld
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC_Ovld_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetAGC_Ovld_TOP
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC_Ovld_Timer
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetAGC_Ovld_Timer
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Mixer_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Mixer_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIR_Mixer_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIR_Mixer_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetHi_Pass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetHi_Pass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIF_Notch
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIF_Notch
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC1_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC1_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC1_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC1_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetForce_AGC1_gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetForce_AGC1_gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC1_Gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC1_Gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC5_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC5_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC5_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC5_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetForce_AGC5_gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetForce_AGC5_gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC5_Gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC5_Gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq0
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq0
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq3
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq3
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq6
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq6
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq7
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq7
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq8
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq8
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq9
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq9
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq10
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq10
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Freq11
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Freq11
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset0
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset0
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset3
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset3
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue   //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset6
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset6
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset7
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset7
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset8
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset8
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset9
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset9
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset10
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset10
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Offset11
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Offset11
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_SW_Algo_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_SW_Algo_Enable
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_Filter_Bypass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_Filter_Bypass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC2_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC2_loop_off
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetForce_AGC2_gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetForce_AGC2_gain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_Filter_Gv
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_Filter_Gv
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_Filter_Band
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_Filter_Band
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_Filter_Cap
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_Filter_Cap
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetAGC2_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC2_Do_step
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetGain_Taper
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetGain_Taper
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_BPF
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_BPF
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRF_BPF_Bypass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRF_BPF_Bypass
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetN_CP_Current
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetN_CP_Current
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetUp_AGC5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetDo_AGC5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetUp_AGC4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetDo_AGC4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetUp_AGC2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetDo_AGC2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetUp_AGC1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetDo_AGC1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC2_Gain_Read
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC1_Gain_Read
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetTOP_AGC3_Read
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC5_Gain_Read
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetAGC4_Gain_Read
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRSSI
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRSSI
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRSSI_AV
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRSSI_AV
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRSSI_Cap_Reset_En
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRSSI_Cap_Reset_En
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRSSI_Cap_Val
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRSSI_Cap_Val
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRSSI_Ck_Speed
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRSSI_Ck_Speed
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRSSI_Dicho_not
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRSSI_Dicho_not
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_Phi2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_Phi2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetDDS_Polarity
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uValue  //  I: Item value
    );
    tmErrorCode_t
    tmddNT220xGetDDS_Polarity
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetRFCAL_DeltaGain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetRFCAL_DeltaGain
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xSetIRQ_Polarity
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8           uValue  /* I: Item value */
    );
    tmErrorCode_t
    tmddNT220xGetIRQ_Polarity
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_0
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_1
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_2
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_3
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_4
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_5
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_6
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_7
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_8
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_9
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_10
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xGetrfcal_log_11
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
        UInt8*          puValue /* I: Address of the variable to output item value */
    );
    tmErrorCode_t
    tmddNT220xLaunchRF_CAL
    (
        tmUnitSelect_t  tUnit       /* I: Unit number */
    );
    tmErrorCode_t
    tmddNT220xWaitIRQ
    (
        tmUnitSelect_t  tUnit,      /* I: Unit number */
        UInt32          timeOut,    /* I: timeout */
        UInt32          waitStep,   /* I: wait step */
        UInt8           irqStatus   /* I: IRQs to wait */
    );
    tmErrorCode_t
    tmddNT220xWaitXtalCal_End
    (
        tmUnitSelect_t  tUnit,      /* I: Unit number */
        UInt32          timeOut,    /* I: timeout */
        UInt32          waitStep    /* I: wait step */
    );
    tmErrorCode_t
    tmddNT220xSetIRQWait
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    Bool            bWait       /* I: Determine if we need to wait IRQ in driver functions */
    );
    tmErrorCode_t
    tmddNT220xGetIRQWait
    (
        tmUnitSelect_t  tUnit,  /* I: Unit number */
    Bool*           pbWait      /* O: Determine if we need to wait IRQ in driver functions */
    );
    tmErrorCode_t
    tmddNT220xAGC1_Adapt
    (
    tmUnitSelect_t  tUnit  /* I: Unit number */
	);

#ifdef __cplusplus
}
#endif

#endif // _TMDD_NT220x_H //---------------
