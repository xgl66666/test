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
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///
/// @file 
/// @brief The ITuner interface
/// @details The ITuner interface supports dynamic tuner switch during run time. It is the user's responsibility to to ensure that the function pointers in the tuner data structure are not NULL.
/// 
//$Revision: 355 $ 
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
// 
#ifndef ITuner_h_h
	#define ITuner_h_h

	#include "avl_dvbsx.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	/// @cond

	/// @endcond

	/// The Tuner data structure
	/// 
	struct AVL_Tuner
	{
		AVL_uint16 m_uiSymbolRate_Hz;
		AVL_uint16 m_uiSlaveAddress;		///< The Tuner slave address. It is the write address of the tuner device. In particular it is an 8-bit address, with the LSB set to zero. The Availink device does not support 10-bit I2C addresses.
		AVL_uint16 m_uiI2CBusClock_kHz;		///< The clock speed of the I2C bus that is dedicated to tuner control.  The units are kHz.
		AVL_uint16 m_uiFrequency_100kHz;	///< The tuned frequency in units of 100kHz.
		AVL_uint16 m_uiLPF_100kHz;			///< The lowpass filter bandwidth of the tuner.
		void * m_pParameters;				///< A pointer to the tuner's customized parameters baseband gain, etc.
		struct AVL_DVBSx_Chip * m_pAVLChip;	///< A pointer to the Availink device connected to the Tuner.
		AVL_DVBSx_ErrorCode (* m_pInitializeFunc)(struct AVL_Tuner *);	 	///< A pointer to the tuner initialization function.
		AVL_DVBSx_ErrorCode (* m_pGetLockStatusFunc)(struct AVL_Tuner *); 	///< A pointer to the tuner GetLockStatus function.
		AVL_DVBSx_ErrorCode (* m_pDumpDataFunc)(AVL_puchar ucpData, AVL_puchar ucpSize, struct AVL_Tuner *); 	///< A pointer to the DumpData function. This function is optional; it is used to dump debug information.
		AVL_DVBSx_ErrorCode (* m_pLockFunc)(struct AVL_Tuner *);			///< A pointer to the tuner Lock function.
	};

	/// This function calculates the tuner lowpass filter bandwidth based on the symbol rate of the received signal.
	/// 
	/// @param uiSymbolRate_10kHz  The symbol rate of the received signal in units of 10kHz.
	/// @param pTuner  A pointer to the tuner object for which the lowpass filter bandwidth is being computed. The function updats the member m_uiLPF_100kHz of this object.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the tuner lowpass filter bandwidth member of the tuner object has been updated.
	/// This function is provided as an example of how to set the lowpass filter bandwidth in a manner that is proportional to the symbol rate.  The user may use their own calculation instead.
	AVL_DVBSx_ErrorCode AVL_DVBSx_ITuner_CalculateLPF(AVL_uint16 uiSymbolRate_10kHz, struct AVL_Tuner * pTuner);

	#ifdef AVL_CPLUSPLUS
}
	#endif
#endif
