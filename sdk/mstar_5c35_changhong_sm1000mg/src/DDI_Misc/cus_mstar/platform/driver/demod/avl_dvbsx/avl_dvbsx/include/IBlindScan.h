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
/// @brief Declares functions for blind scan.
/// 
/// @details The Availink device can store up to 120 detected carrier channels. 
/// If more than 120 channels are detected in the scan range, the function ::AVL_DVBSx_IBlindScan_GetScanStatus will 
/// set ::AVL_DVBSx_BlindScanPara.m_uiResultCode to 1.
/// The user can read blind scan results with the function ::AVL_DVBSx_IBlindScan_ReadChannelInfo. 
/// 
//$Revision: 985 $ 
//$Date: 2009-11-09 18:09:24 -0500 (Mon, 09 Nov 2009) $
// 
#ifndef IBlindScan_h_h
	#define IBlindScan_h_h

	#include "avl_dvbsx.h"
	#include "ITuner.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond

	/// @endcond

	/// Stores the blind scan parameters which are passed to the ::AVL_DVBSx_IBlindScan_Scan function.
	struct AVL_DVBSx_BlindScanPara
	{
		AVL_uint16 m_uiStartFreq_100kHz;	///< The start scan frequency in units of 100kHz. The minimum value depends on the tuner specification. 
		AVL_uint16 m_uiStopFreq_100kHz;		///< The stop scan frequency in units of 100kHz. The maximum value depends on the tuner specification.
		AVL_uint16 m_uiMinSymRate_kHz;		///< The minimum symbol rate to be scanned in units of kHz. The minimum value is 1000 kHz.
		AVL_uint16 m_uiMaxSymRate_kHz;		///< The maximum symbol rate to be scanned in units of kHz. The maximum value is 45000 kHz.
	};

	/// Stores the blind scan status information.
	struct AVL_DVBSx_BlindScanInfo
	{
		AVL_uint16 m_uiProgress;				///< The percentage completion of the blind scan procedure. A value of 100 indicates that the blind scan is finished.
		AVL_uint16 m_uiChannelCount;			///< The number of channels detected thus far by the blind scan operation.  The Availink device can store up to 120 detected channels.
		AVL_uint16 m_uiNextStartFreq_100kHz;	///< The start frequency of the next scan in units of 100kHz.
		AVL_uint16 m_uiResultCode;				///< The result of the blind scan operation.  Possible values are:  0 - blind scan operation normal; 1 -- more than 120 channels have been detected.
	};

	/// Performs a blind scan operation. Call the function ::AVL_DVBSx_IBlindScan_GetScanStatus to check the status of the blind scan operation.
	/// 
	/// @param pBSPara  A pointer to the blind scan configuration parameters.
	/// @param uiTumerLPF_100kHz The tuner LPF bandwidth setting in units of 100 kHz.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object on which blind scan is being performed.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the scan command is successfully sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the scan command could not be sent because the Availink device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the device is not in the blind scan functional mode or if the parameter \a pBSPara->m_uiStartFreq_100kHz is larger than the parameter \a pBSPara->m_uiStopFreq_100kHz
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Scan(struct AVL_DVBSx_BlindScanPara * pBSPara, AVL_uint16 uiTunerLPF_100kHz, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Queries the blind scan status.
	/// 
	/// @param pBSInfo  Pointer to the object in which the blind scan status is to be stored.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the blind scan status is being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the blind scan status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_GetScanStatus(struct AVL_DVBSx_BlindScanInfo * pBSInfo, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Cancels the current blind scan operation.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the blind scan operation is being canceled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the Availink device has been commanded to cancel the blind scan operation.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the blind scan could not be canceled because the device is still processing a previous command.
	/// @remarks This function sends a cancel command to the Availink device. The internal scan is not be canceled until the current scan section is finished. 
	/// Call ::AVL_DVBSx_IBase_GetRxOPStatus to determine when the scan cancellation is complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Cancel(struct AVL_DVBSx_Chip * pAVLChip );

	/// Retrieves the blind scan results.
	/// 
	/// @param uiStartIndex  The blind scan results are stored in an array internal to the Availink device. This parameter tells the function the array index at which to retrive the results.
	/// @param pChannelCount The number of channel results to be retrieved.  The function updates this value with the actual number of channel results that are reported.
	/// @param pChannel Pointer to an object in which the blind scan results are to be stored.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the blind scan results are being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the blind scan results have been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks The scan results internal to the Availink device are overwritten after a subsequent call to the ::AVL_DVBSx_IBlindScan_Reset function. Be sure to read out all of the channel information before calling ::AVL_DVBSx_IBlindScan_Reset.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_ReadChannelInfo(AVL_uint16 uiStartIndex, AVL_puint16 pChannelCount, struct AVL_DVBSx_Channel * pChannel, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Resets the Availink device internal blind scan results.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the internal blind scan results are being reset.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the reset operation is successful.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Reset( const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to indicate whether the tuner inverts the received signal spectrum.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the blind scan tuner spectrum inversion setting is being configured.
	/// @param enumSpectrumInversion Indicates whether the tuner being used inverts the received signal spectrum.  
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the spectrum inversion configuration is successful.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_SetSpectrumInversion( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_BlindscanSpectrum enumSpectrumInversion );

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
