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
/// @brief Defines functions for initializing and controlling the I2C repeater.
/// @details The I2C repeater provides a dedicated I2C bus for tuner control. It is 
/// recommended that the functions in this interface be used to implement a tuner 
/// driver for the tuner being used. For customer convenience, Availink provides tested 
/// tuner drivers for a variety tuner devices.
/// 
//$Revision: 514 $ 
//$Date: 2008-08-12 17:51:32 -0400 (Tue, 12 Aug 2008) $
// 
#ifndef II2CRepeater_h_h
	#define II2CRepeater_h_h

	#include "avl_dvbsx.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond

	#define I2CM_CMD_LENGTH   0x14
	#define I2CM_RSP_LENGTH   0x14

	#define OP_I2CM_NOOP      0x00
	#define OP_I2CM_INIT	  0x01
	#define OP_I2CM_WRITE     0x02
	#define OP_I2CM_READ      0x03

	///@endcond

	///@cond

	/// Sends an I2C repeater operational command to the Availink device. 
	/// 
	/// @param pBuff Pointer to the array which contains the operational command and its parameters.
	/// @param ucSize The number of command related bytes in the array to which pBuff points.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object to which an I2C repeater operational command is being sent.
	/// 
	/// Return ::AVL_DVBSx_EC_OK if the I2C repeater operational command has been sent to the Availnk device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent to the Availink device because the device is still processing a previous command.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendOP(AVL_puchar pBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip );

	///@endcond

	/// Initializes the I2C repeater.
	/// 
	/// @param I2CBusClock_kHz The clock speed of the I2C bus between the tuner and the Availink device.
	/// @param pAVLChip A pointer to a ::AVL_DVBSx_Chip object for which the I2C repeater is being initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the initialize command has been sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the initialize command could not be sent to the Availink device because the device is still processing a previous command.
	/// @remarks This function must be called before any other function in this interface. This is a nonblocking function.  Call ::AVL_DVBSx_II2CRepeater_GetOPStatus to determine if the I2C repeater is initialized.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_Initialize( AVL_uint16 I2CBusClock_kHz, struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads data back from the tuner via the I2C repeater.  This function is used with tuners which insert a stop
	/// bit between messages.
	/// 
	/// @param ucSlaveAddr The slave address of the tuner device. Please note that the Availink device only supports a 7 bit slave address.
	/// @param pucBuff Pointer to a buffer in which to store the data read from the tuner.
	/// @param uiSize The number of bytes to read from the tuner.  The maximum value is 20.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which data is being read from the tuner.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the tuner.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the read commmand could not be sent to the Availink device because the device is still processing  a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if \a uiSize is larger than 20.
	/// @remarks This function will trigger a I2C read operation. It is used with tuners which insert a stop bit between messages.  The read position (or device internal address) can be determined by calling ::AVL_DVBSx_II2CRepeater_SendData.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData(  AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads data back from the tuner via the I2C repeater.  This function is used with tuners which do not insert a stop
	/// bit between messages.
	/// 
	/// @param ucSlaveAddr The slave address of the tuner device. Please note that the Availink device only supports a 7 bit slave address.
	/// @param pucBuff Pointer to the buffer in which to store the read data.
	/// @param ucRegAddr The address of the register being read.
	/// @param uiSize The number of bytes to read from the tuner.  The maximum value is 20.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which data is being read from the tuner.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the tuner.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the read command could not be sent to the Availink device because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if \a uiSize is larger than 20.
	/// @remarks This function will trigger a I2C read operation. It is used with tuners which do not insert a stop bit between messages.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData_Multi(  AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uchar ucRegAddr, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Writes data to the tuner via the I2C repeater.
	/// 
	/// @param ucSlaveAddr The slave address of the tuner device. Please note that the Availink device only supports a 7 bit slave address.
	/// @param ucBuff Pointer to the buffer which contains the data to be sent to the tuner.
	/// @param uiSize The number of bytes to be sent to the tuner.  The maximum value is 17.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which data is being sent to the tuner.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the send command has been sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the send command could not be sent to the Availink device because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if \a uiSize is larger than 17.
	/// @remarks The internal register address is buried in the buffer to which \a ucBuff points. This function is a nonblocking function.  Call ::AVL_DVBSx_II2CRepeater_GetOPStatus to determine if the write operation is complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendData(  AVL_uchar ucSlaveAddr, const AVL_puchar ucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks if the last I2C repeater operation is finished.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which I2C repeater operation status is being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the last I2C repeater operation is complete.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the Availink device is still processing the last I2C repeater operation.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_GetOPStatus(const struct AVL_DVBSx_Chip * pAVLChip );

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
