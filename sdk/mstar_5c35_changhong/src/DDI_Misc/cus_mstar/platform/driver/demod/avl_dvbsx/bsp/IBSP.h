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
/// @brief Defines the BSP functions which the user needs to implement. 
/// @details These BSP functions are called by SDK API functions. This file also defines some hardware related macros which also need to be 
/// customized by the user according to their hardware platform. Most of the functions declared here should 
/// NOT be directly called by the user's applications explicitly.  There are two exceptions.  These are ::AVL_DVBSx_IBSP_Initialize and ::AVL_DVBSx_IBSP_Dispose.
///
//$Revision: 355 $ 
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
// 
#ifndef IBSP_h_h
	#define IBSP_h_h

	#include "avl_dvbsx.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	#define MAX_II2C_READ_SIZE	64		///< The maximum number of bytes the back end chip can handle in a single I2C read operation. This value must >= 2.
	#define MAX_II2C_Write_SIZE	64		///< The maximum number of bytes the back end chip can handle in a single I2C write operation. This value must >= 8.

	/// Performs initialization for BSP operations.
	/// 
	/// @remarks This function should never called inside the SDK. The user can redeclare this function to any prototype.
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void);

	/// Destroys all resources allocated in AVL_DVBSx_IBSP_Initialize and other BSP operations.
	/// 
	/// @remarks This function should never called inside the SDK. The user can redeclare this function to any prototype.
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void);

	/// Implements a delay in units of milliseconds.
	/// 
	/// @param uiMS: The delay period in units of milliseconds.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS );

	/// Performs an I2C read operation.
	/// 
	/// @param pAVLChip Pointer to the Availink device for which the read operation is being performed. 
	/// @param pucBuff Pointer to a buffer in which to place the read data.
	/// @param puiSize The number of bytes to be read. The function updates this value with the number of bytes actually read. If there is an error, the function sets this value to 0.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the read operation is successful. 
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function should perform a direct I2C read operation without first writing the device internal address. The Availink SDK automatically handles writing the device internal address prior to performing the read operation.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead( AVL_uint16 uiSlaveAddr,  AVL_puchar pucBuff, AVL_puint16 puiSize );

	/// Performs an I2C write operation.
	/// 
	/// @param pAVLChip Pointer to the Availink device for which the write operation is being performed.
	/// @param pucBuff Pointer to a buffer which contains the data to be written.
	/// @param puiSize The number of bytes to be written. The function updates this value with the number of bytes actually written. If there is an error, the function sets this value to 0.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the write operation is successful. 
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite( AVL_uint16 uiSlaveAddr,  AVL_puchar pucBuff, AVL_puint16 puiSize );

	/// Initializes a semaphore object.
	/// 
	/// @param pSemaphore A pointer to the ::AVL_semaphore object to be initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	/// @remarks All of the semaphore objects should be initialized with 1 as maximum count and the initialized state should be signaled. In particular, after initialization, the first query should succeed.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore ); 

	/// Queries the semaphore. If the semaphore is held by another thread, the function should be blocked until the semaphore is available.
	/// 
	/// @param pSemaphore A pointer to the ::AVL_semaphore object being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore );         

	/// Releases the semaphore so that it is available.
	/// 
	/// @param pSemaphore A pointer to the ::AVL_semaphore object which is being released.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore );

	#ifdef AVL_CPLUSPLUS
}
	#endif
#endif
