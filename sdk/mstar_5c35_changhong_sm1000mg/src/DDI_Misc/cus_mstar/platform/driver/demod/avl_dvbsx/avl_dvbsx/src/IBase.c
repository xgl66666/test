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


//$Revision: 985 $
//$Date: 2009-11-09 18:09:24 -0500 (Mon, 09 Nov 2009) $
//
#include "Board.h"
#if(FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)
#include "IBase.h"
#include "II2C.h"
#include "IBSP.h"
#include "IRx.h"
#include "ITuner.h"
#include "II2CRepeater.h"
#include "IBlindScan.h"



AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetPLL(const struct AVL_DVBSx_PllConf * pPLLConf, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, pll_clkf_map_addr, pPLLConf->m_uiClkf);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, pll_bwadj_map_addr, pPLLConf->m_uiClkf);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, pll_clkr_map_addr, pPLLConf->m_uiClkr);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, pll_od_map_addr, pPLLConf->m_uiPllod);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, pll_od2_map_addr, pPLLConf->m_uiPllod2);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, pll_od3_map_addr, pPLLConf->m_uiPllod3);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, pll_softvalue_en_map_addr,   1);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, reset_register_addr, 0);
	AVL_DVBSx_II2C_Write32(pAVLChip, reset_register_addr, 1);  //this is a reset, do not expect an ACK from the chip.
	pAVLChip->m_DemodFrequency_10kHz = pPLLConf->m_DmodFrequency_10kHz;
	pAVLChip->m_FecFrequency_10kHz =  pPLLConf->m_FecFrequency_10kHz;
	pAVLChip->m_MpegFrequency_10kHz = pPLLConf->m_MpegFrequency_10kHz;

	return(r);
}
/// @endcond

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_DownloadFirmware(AVL_puchar pFirmwareData,  const struct AVL_DVBSx_Chip * pAVLChip)
{
	AVL_uint32 uiSize, uiDataSize;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 i1;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, core_reset_b_reg, 0);

	uiDataSize = DeChunk32(pFirmwareData);
	i1 = 4;
	while( i1 < uiDataSize )
	{
		uiSize = DeChunk32(pFirmwareData+i1);
		i1 += 4;
		r |= AVL_DVBSx_II2C_Write(pAVLChip, pFirmwareData+i1+1, (AVL_uint16)(uiSize+3));
		i1 += 4 + uiSize;
	}
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, 0x00000000, 0x00003ffc);
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, core_ready_word_addr, 0x0000);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, error_msg_addr, 0x00000000);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, error_msg_addr+4, 0x00000000);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, core_reset_b_reg, 1);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetStatus( struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint16 uiReadValue;
	AVL_uint32 uiTemp;

	r = AVL_DVBSx_II2C_Read32(pAVLChip, core_reset_b_reg, &uiTemp);
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, core_ready_word_addr, &uiReadValue);
	if( (AVL_DVBSx_EC_OK == r) )
	{
		if( (0 == uiTemp) || (uiReadValue != 0xA55A) )
		{
			r = AVL_DVBSx_EC_GeneralFail;
		}
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetVersion( struct AVL_DVBSx_VerInfo * pVerInfo, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uint32 uiTemp;
	AVL_uchar ucBuff[4];
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r =  AVL_DVBSx_II2C_Read32(pAVLChip, rom_ver_addr, &uiTemp);
	if( AVL_DVBSx_EC_OK == r )
	{
		Chunk32(uiTemp, ucBuff);
		pVerInfo->m_Chip.m_Major = ucBuff[0];
		pVerInfo->m_Chip.m_Minor = ucBuff[1];
		pVerInfo->m_Chip.m_Build = ucBuff[2];
		pVerInfo->m_Chip.m_Build = ((AVL_uint16)((pVerInfo->m_Chip.m_Build)<<8)) + ucBuff[3];
		r |=  AVL_DVBSx_II2C_Read32(pAVLChip, rc_patch_ver_addr, &uiTemp);
		if( AVL_DVBSx_EC_OK == r )
		{
			Chunk32(uiTemp, ucBuff);
			pVerInfo->m_Patch.m_Major = ucBuff[0];
			pVerInfo->m_Patch.m_Minor = ucBuff[1];
			pVerInfo->m_Patch.m_Build = ucBuff[2];
			pVerInfo->m_Patch.m_Build = ((AVL_uint16)((pVerInfo->m_Patch.m_Build)<<8)) + ucBuff[3];

			pVerInfo->m_API.m_Major = AVL_DVBSx_API_VER_MAJOR;
			pVerInfo->m_API.m_Minor = AVL_DVBSx_API_VER_MINOR;
			pVerInfo->m_API.m_Build = AVL_DVBSx_API_VER_BUILD;
		}
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Initialize( const struct AVL_DVBSx_PllConf * pPLLConf, AVL_puchar pInitialData,  struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_IBase_SetPLL(pPLLConf, pAVLChip );
	r |= AVL_DVBSx_IBSP_Delay(1);	  // this delay is critical
	r |= AVL_DVBSx_IBase_DownloadFirmware(pInitialData, pAVLChip);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Halt( struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_IBase_SendRxOP(OP_RX_HALT, pAVLChip);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Sleep( struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_IBase_SendRxOP(OP_RX_SLEEP, pAVLChip);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Wake( struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_IBase_SendRxOP(OP_RX_WAKE, pAVLChip);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetFunctionalMode(const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_FunctionalMode enumFunctionalMode)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r = AVL_DVBSx_II2C_Write16(pAVLChip, rc_functional_mode_addr, (AVL_uint16)enumFunctionalMode);
      r |=AVL_DVBSx_II2C_Write16(pAVLChip, rc_iq_mode_addr,0);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetFunctionalMode(enum AVL_DVBSx_FunctionalMode * pFunctionalMode,  const struct AVL_DVBSx_Chip * pAVLChip)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint16 uiTemp;

	r = AVL_DVBSx_II2C_Read16(pAVLChip, rc_functional_mode_addr, &uiTemp);
	*pFunctionalMode = (enum AVL_DVBSx_FunctionalMode)(uiTemp & 0x0001);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SendRxOP(AVL_uchar ucOpCmd, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uchar pucBuff[2];
	AVL_uint16 uiTemp;

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semRx));
	r |= AVL_DVBSx_IBase_GetRxOPStatus(pAVLChip);
	if( AVL_DVBSx_EC_OK == r )
	{
		pucBuff[0] = 0;
		pucBuff[1] = ucOpCmd;
		uiTemp = DeChunk16(pucBuff);
		r |= AVL_DVBSx_II2C_Write16(pAVLChip, rx_cmd_addr, uiTemp);
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semRx));

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetRxOPStatus(const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uchar pBuff[2] = {0,0};

	r = AVL_DVBSx_II2C_Read(pAVLChip, rx_cmd_addr, pBuff, 2);
	if( AVL_DVBSx_EC_OK == r )
	{
		if( 0 != pBuff[1] )
		{
			r = AVL_DVBSx_EC_Running;
		}
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetGPIODir( AVL_uchar ucDir, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 uiTemp = (AVL_uint32)(ucDir & 0x7);

	r = AVL_DVBSx_II2C_Write32(pAVLChip, gpio_reg_enb, uiTemp);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetGPIOVal( AVL_uchar ucVal, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 uiTemp;

	uiTemp = (AVL_uint32)(ucVal & 0x7);
	r = AVL_DVBSx_II2C_Write32(pAVLChip, gpio_data_reg_out, uiTemp);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetGPIOVal( AVL_puchar pucVal, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 uiTemp;

	r = AVL_DVBSx_II2C_Read32(pAVLChip, gpio_data_in_to_reg, &uiTemp);
	*pucVal = (AVL_uchar)uiTemp;

	return(r);
}
#endif

