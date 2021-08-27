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
/// @brief Implements the functions declared in IBSP.h.
///
//$Revision: 563 $
//$Date: 2008-09-24 17:24:29 -0400 (Wed, 24 Sep 2008) $
//
#include "Board.h"
#if(FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)
#include "IBSP.h"
#include "AVL_DVBSx_Board.h"

/// Please use the following table of PLL configurations if 55 Msps symbol rate is desired at the demod end.

///	{503,  1, 6, 4, 2,  4000, 12600, 16800, 25200} < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{447,  1, 6, 4, 2,  4500, 12600, 16800, 25200} < Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  4, 6, 4, 2, 10000, 12600, 16800, 25200} < Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  7, 6, 4, 2, 16000, 12600, 16800, 25200} < Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{111,  2, 6, 4, 2, 27000, 12600, 16800, 25200} < Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz

/// Feel free to contact Availink Support staff for further infromation on this.

/// Users can remove unused elements from the following array to reduce the SDK footprint size.
const struct AVL_DVBSx_PllConf pll_conf[] =
{
	 {503,  1, 7, 4, 2,  4000, 11200, 16800, 25200} ///< Reference clock 4 MHz,   Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{447,  1, 7, 4, 2,  4500, 11200, 16800, 25200} ///< Reference clock 4.5 MHz, Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  4, 7, 4, 2, 10000, 11200, 16800, 25200} ///< Reference clock 10 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  7, 7, 4, 2, 16000, 11200, 16800, 25200} ///< Reference clock 16 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{111,  2, 7, 4, 2, 27000, 11200, 16800, 25200} ///< Reference clock 27 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz

	// The following set of PLL configuration at different reference clock frequencies refer to demod operation
	// in high performance mode.
	,{566,  1, 7, 4, 2,  4000, 12600, 18900, 28350} /// < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{503,  1, 7, 4, 2,  4500, 12600, 18900, 28350} ///< Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{566,  4, 7, 4, 2, 10000, 12600, 18900, 28350} ///< Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{566,  7, 7, 4, 2, 16000, 12600, 18900, 28350} ///< Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{377,  8, 7, 4, 2, 27000, 12600, 18900, 28350} ///< Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz

};
const AVL_uint16 pll_array_size = sizeof(pll_conf)/sizeof(struct AVL_DVBSx_PllConf);

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS )
{
    //SysDelay(uiMS);
    MsOS_DelayTask(uiMS);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead(  AVL_uint16 uiSlaveAddr,  AVL_puchar pucBuff, AVL_puint16 puiSize )
{
       if(MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT, uiSlaveAddr, 0, NULL, (*puiSize), pucBuff))
       {
            return(AVL_DVBSx_EC_OK);
       }
       printf("\n AVL6211 I2C Read failed Slave=0x%X",uiSlaveAddr);
       return(AVL_DVBSx_EC_I2CFail);
}
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite(  AVL_uint16 uiSlaveAddr,  AVL_puchar pucBuff,  AVL_puint16  puiSize )
{
       if(MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT, uiSlaveAddr, 0, NULL, (*puiSize), pucBuff))
       {
	     return(AVL_DVBSx_EC_OK);
       }
       printf("\n AVL6211 I2C Write failed Slave=0x%X",uiSlaveAddr);
       return(AVL_DVBSx_EC_I2CFail);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore )
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore )
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore )
{
	return(AVL_DVBSx_EC_OK);
}
#endif


