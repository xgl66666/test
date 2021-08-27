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


///@file
///@brief This interface provides functions for performing atomic I2C operations.
///@details The user should always use the functions provided in this file to perform I2C operations. These functions provide 
/// multi_thread protection and they automatically break potentially large I2C transactions into smaller I2C transactions to 
/// meet the hardware limitaions defined in the BSP. 
/// These functions also eliminate the difference between the big endian and little endian systems. Please note that 
/// these I2C functions can only be used to perform I2C operations with the Availnk device. When reading and writing data from and to
/// the Availink device, all data is treated as unsigned data. The user needs to cast data properly to get desired values.
/// 
//$Revision: 323 $ 
//$Date: 2008-03-14 11:31:11 -0400 (Fri, 14 Mar 2008) $
// 
#ifndef II2C_h_h
	#define II2C_h_h

	#include "avl_dvbsx.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	/// @cond
	
	/// Initializes I2C operation.
	/// 
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if I2C functionality has been initialized.
	/// Return ::AVL_DVBSx_EC_MemoryRunout if the semaphores cannot be initialized.
	/// @remarks This is an internal SDK function. This function must be called before any I2C operations. The user does not need to worry about calling this function directly because it is called by the SDK function ::InitAVL_DVBSx_ChipObject.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Initialize(void);

	/// @endcond
	
	/// This function reads one or more bytes from the Availink device.  The bytes are read from the device internal address specified by the user.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object for which the read operation is being performed.
	/// @param uiOffset  The Availink device internal address from where the function is to read data.
	/// @param pucBuff  Pointer to a buffer in which to store the read data.
	/// @param uiSize The number of bytes to read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks The function first performs an I2C write operation to send the read address to the Availink device.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiOffset, AVL_puchar pucBuff, AVL_uint16 uiSize);

	/// Reads one or more bytes from the Availink device at the current address.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object for which the read operation is being performed.
	/// @param pucBuff  Pointer to a buffer in which to store the read data.
	/// @param uiSize The number of bytes to read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks The function will directly performs an I2C read operation.  
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_ReadDirect( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puchar pucBuff, AVL_uint16 uiSize);

	/// This function writes one or more bytes to the Availink device.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object to which data is to be written.
	/// @param pucBuff  Pointer to a buffer which contains the data to be written.
	/// @param uiSize The number of bytes to be written.  The minimum permissible value is 3.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been written to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if uiSize is less than 3.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puchar pucBuff, AVL_uint16 uiSize);

	/// Reads a 16-bit unsigned integer from the Availink device.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object from which data is being read.
	/// @param uiAddr  The Availink device internal address from where the data is to be read.
	/// @param puiData Pointer to a variable in which to store the read data.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read16( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_puint16 puiData );

	/// Reads a 32-bit unsigned integer from the Availink device.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object from which data is being read.
	/// @param uiAddr  The Availink device internal address from where the data is to be read.
	/// @param puiData Pointer to a variable in which to store the read data.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read32( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_puint32 puiData );

	/// Writes a 16-bit unsigned integer to the Availink device.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object to which data is being written.
	/// @param uiAddr  The Availink device internal address to which the data is to be written.
	/// @param uiData The data to be written to the Availink device.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been written to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write16( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_uint16 uiData );

	/// Writes a 32-bit unsigned integer to the Availink device.
	/// 
	/// @param pAVLChip  Pointer to the ::AVL_DVBSx_Chip object to which the data is being written.
	/// @param uiAddr  The Availink device internal address to which the data is to be written.
	/// @param uiData The data to be written to the Availink device.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been written to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write32( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_uint32 uiData );

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
