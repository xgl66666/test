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
/// @brief Declares the functions for Diseqc operations.
/// @details There are some limitations of the Diseqc operation. First, it is a half duplex bus. You cannot send and receive data simultaneously. Second, the maximum size of each transmission is 8 bytes.
/// The diseqc interface can operate in different modes such as modulation mode, tone mode and continuous mode. There are functions corresponding to these modes.  If the user changes the operating mode by calling functions
/// which belong to another mode, the previous operation will be cancelled if it has not finished yet.
///
//$Revision: 355 $ 
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
// 
#ifndef IDiseqc_h_h
	#define IDiseqc_h_h

	#include "avl_dvbsx.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond

	#define diseqc_tx_cntrl_addr			0x00700000
	#define diseqc_tone_frac_n_addr			0x00700004
	#define diseqc_tone_frac_d_addr			0x00700008
	#define diseqc_tx_st_addr				0x0070000c
	#define diseqc_rx_parity_addr			0x00700010
	#define diseqc_rx_msg_tim_addr			0x00700014
	#define diseqc_rx_st_addr				0x00700018
	#define diseqc_rx_cntrl_addr			0x0070001c
	#define diseqc_srst_addr				0x00700020
	#define diseqc_bit_time_addr			0x00700024
	#define diseqc_samp_frac_n_addr			0x00700028
	#define diseqc_samp_frac_d_addr			0x0070002c
	#define diseqc_bit_decode_range_addr	0x00700030
	#define diseqc_rx_fifo_map_addr			0x00700040
	#define diseqc_tx_fifo_map_addr			0x00700080

	///@endcond

	///
	/// When transmitting data in Tone0 or Tone1 mode, there is a gap between two tones. This enumeration defines the gap length.
	enum AVL_DVBSx_Diseqc_TxGap
	{
		AVL_DVBSx_DTXG_15ms = 0,		///< = 0  The gap is 15 ms.
		AVL_DVBSx_DTXG_20ms = 1,		///< = 1  The gap is 20 ms.
		AVL_DVBSx_DTXG_25ms = 2,		///< = 2  The gap is 25 ms.
		AVL_DVBSx_DTXG_30ms = 3			///< = 3  The gap is 30 ms.
	};

	///
	/// Defines the transmit mode.
	enum AVL_DVBSx_Diseqc_TxMode
	{
		AVL_DVBSx_DTM_Modulation = 0,			///< = 0  Use modulation mode.
		AVL_DVBSx_DTM_Tone0 = 1,				///< = 1  Send out tone 0.
		AVL_DVBSx_DTM_Tone1 = 2,				///< = 2  Send out tone 1.
		AVL_DVBSx_DTM_Continuous = 3			///< = 3  Continuously send out pulses.
	};

	///
	/// Configures the Diseqc output waveform mode.
	enum AVL_DVBSx_Diseqc_WaveFormMode
	{
		AVL_DVBSx_DWM_Normal = 0,			///< = 0  Normal waveform mode
		AVL_DVBSx_DWM_Envelope = 1			///< = 1  Envelope waveform mode
	};

	///
	/// After data is transmitted to the Diseqc device, the Diseqc device may return some data. 
	/// This enumeration controls the amount of time for which the Availink device will open the Diseqc input FIFO to receive the data. Data that 
	/// is received outside of this time frame is abandoned.
	enum AVL_DVBSx_Diseqc_RxTime
	{
		AVL_DVBSx_DRT_150ms = 0,		///< = 0  Wait 150 ms for receive data and then close the input FIFO.
		AVL_DVBSx_DRT_170ms = 1,		///< = 1  Wait 170 ms for receive data and then close the input FIFO.
		AVL_DVBSx_DRT_190ms = 2,		///< = 2  Wait 190 ms for receive data and then close the input FIFO.
		AVL_DVBSx_DRT_210ms = 3			///< = 3  Wait 210 ms for receive data and then close the input FIFO.
	};

	/// Stores the Diseqc configuration parameters.
	/// 
	struct AVL_DVBSx_Diseqc_Para
	{
		AVL_uint16 m_ToneFrequency_kHz;						///< The Diseqc bus speed in units of kHz. Normally, it is 22kHz. 
		enum AVL_DVBSx_Diseqc_TxGap m_TXGap;				///< Transmit gap
		enum AVL_DVBSx_Diseqc_WaveFormMode m_TxWaveForm;	///< Transmit waveform format
		enum AVL_DVBSx_Diseqc_RxTime m_RxTimeout;			///< Receive time frame window
		enum AVL_DVBSx_Diseqc_WaveFormMode m_RxWaveForm;	///< Receive waveform format
	};

	/// Stores the Diseqc transmitter status.
	/// 
	struct AVL_DVBSx_Diseqc_TxStatus
	{
		AVL_uchar m_TxDone;				///< Indicates whether the transmittsion is complete (1 - transmission is finished, 0 - transmission is still in progress).
		AVL_uchar m_TxFifoCount;		///< The number of bytes remaining in the transmit FIFO
	};

	/// Stores the Diseqc receiver status
	/// 
	struct AVL_DVBSx_Diseqc_RxStatus
	{
		AVL_uchar m_RxFifoCount;		///< The number of bytes in the Diseqc receive FIFO.
		AVL_uchar m_RxFifoParChk;		///< The parity check result of the received data. This is a bit-mapped field in which each bit represents the parity check result for each each byte in the receive FIFO.  The upper bits without corresponding data are undefined. If a bit is 1, the corresponding byte in the FIFO has good parity. For example, if three bytes are in the FIFO, and the parity check value is 0x03 (value of bit 2 is zero), then the first and the second bytes in the receive FIFO are good. The third byte had bad parity. 
		AVL_uchar m_RxDone;				///< 1 if the receiver window is turned off, 0 if it is still in receiving state.
	};

	/// Initializes the Diseqc component using the configuration parameters in \a pDiseqcPara.
	/// 
	/// @param pDiseqcPara  Pointer to the Diseqc configuration parameters.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the Diseqc interface is being initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the Diseqc interface has been initialized.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_Initialize( const struct AVL_DVBSx_Diseqc_Para * pDiseqcPara, struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads data from the Diseqc input FIFO.
	/// 
	/// @param pucBuff Pointer to a buffer in which the read data should be stored.
	/// @param pucSize The number of bytes to read from the FIFO. The maximum value is 8.  The function updates this parameter to indicate the number of bytes that have actually been read.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the Diseqc input FIFO is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the Diseqc input FIFO has been read. 
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the Diseqc interface is not in modulation mode or if the Diseqc interface is still receiving the data.
	/// @remarks Availink recommends that the user call ::AVL_DVBSx_IDiseqc_GetRxStatus before calling this function. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_ReadModulationData( AVL_puchar pucBuff, AVL_puchar pucSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Sends data to the Diseqc bus. 
	/// 
	/// @param pucBuff Pointer to an array which contains the data to be sent to the Diseqc bus.
	/// @param ucSize The number of bytes to be sent. The maximum is 8.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which data is to be sent to the Diseqc bus.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been sent
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch the Diseqc mode because the last transmission is not complete yet.
	/// Return ::AVL_DVBSx_EC_MemoryRunout if \aucSize is larger than 8.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SendModulationData( const AVL_puchar pucBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks the current status of the Diseqc transmitter.
	/// 
	/// @param pTxStatus  Pointer to an object in which to store the Diseqc transmitter status.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the transmit status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the transmit status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetTxStatus( struct AVL_DVBSx_Diseqc_TxStatus * pTxStatus, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks the current status of the Diseqc receiver.
	/// 
	/// @param pRxStatus  Pointer to an object in which to store the Diseqc receiver status information.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the receiver status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the receiver status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the Diseqc component is not in modulation mode.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetRxStatus( struct AVL_DVBSx_Diseqc_RxStatus * pRxStatus, struct AVL_DVBSx_Chip * pAVLChip );

	/// Sets the output level of pin LNB_CNTRL_0.  This pin is typically used to control LNB polarization.
	/// 
	/// @param uiOut Controls the level of pin LNB_CNTRL_0 (0 - Set output level low, 1 - Set output level high).
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the output level of pin LNB_CNTRL_0 is being set.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the pin level has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SetLNBOut( AVL_uchar uiOut, struct AVL_DVBSx_Chip * pAVLChip );    

	/// Gets the output level of pin LNB_CNTRL_0.
	/// 
	/// @param puiOut Pointer to a variable in which to store the pin output level (0 - Output level is low, 1 - Output level is high).
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the pin output level is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the pin level has been read.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetLNBOut( AVL_puchar puiOut, struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to transmit the Diseqc Tone.
	/// 
	/// @param ucTone  Configures the tone to be transmitted (0 - Tone_0, 1 - Tone_1).
	/// @param ucCount  The number of tones to be transmitted.  The maximum value is 8.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being configured to transmit the tones.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been configured to transmit the Diseqc tones.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_MemoryRunout if \aucCount is larger than 8.
	/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch the Diseqc mode because the last transmission is not complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SendTone( AVL_uchar ucTone, AVL_uchar ucCount, struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to output a continuous 22 kHz Diseqc waveform.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being configured to output the waveform.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been configured to transmit the waveform.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch the Diseqc mode because the last transmission is not complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_StartContinuous (struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to stop outputting a continuous  22kHz Diseqc waveform.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being configured to stop outputting the waveform.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been configured to stop transmtitting the waveform.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_StopContinuous (struct AVL_DVBSx_Chip * pAVLChip );


	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
