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


//$Revision: 991 $ 
//$Date: 2009-11-19 16:21:06 -0500 (Thu, 19 Nov 2009) $
// 
#include "Board.h"
#if(MS_DVB_TYPE_SEL == DVBS)

#include "IRx.h"
#include "IBase.h"
#include "II2C.h"
#include "IBSP.h"

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_Initialize(const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	// Set clk to match the PLL 
	r = AVL_DVBSx_II2C_Write16(pAVLChip, rc_int_dmd_clk_MHz_addr,  pAVLChip->m_DemodFrequency_10kHz);
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_int_fec_clk_MHz_addr, pAVLChip->m_FecFrequency_10kHz);
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_int_mpeg_clk_MHz_addr, pAVLChip->m_MpegFrequency_10kHz);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_format_addr, 1);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetFreqSweepRange(AVL_uint16 uiFreqSweepRange_10kHz, const struct AVL_DVBSx_Chip * pAVLChip )
{
	if(uiFreqSweepRange_10kHz > 500)
	{
		uiFreqSweepRange_10kHz = 500;
	}
	return AVL_DVBSx_II2C_Write16(pAVLChip, rc_int_carrier_freq_half_range_MHz_addr, uiFreqSweepRange_10kHz);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetChannelLockMode( struct AVL_DVBSx_Channel * psChannel, enum AVL_DVBSx_LockMode enumChannelLockMode )
{
	if(enumChannelLockMode == AVL_DVBSx_LOCK_MODE_FIXED)
	{
		psChannel->m_Flags &= ~CI_FLAG_LOCK_MODE_BIT_MASK;
	}
	else
	{
		psChannel->m_Flags |= CI_FLAG_LOCK_MODE_BIT_MASK;
	}

	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_LockChannel( struct AVL_DVBSx_Channel * psChannel, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 IQ;
	AVL_uint32 autoIQ_Detect;
	AVL_uint16 Standard;
	enum AVL_DVBSx_LockMode LockMode;
	enum AVL_DVBSx_FunctionalMode enumFunctionalMode;
	r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_IBase_GetFunctionalMode(&enumFunctionalMode, pAVLChip);
	if(enumFunctionalMode == AVL_DVBSx_FunctMode_Demod)
	{
		if( (psChannel->m_uiSymbolRate_Hz > 800000) && (psChannel->m_uiSymbolRate_Hz < 60000000) && (r == AVL_DVBSx_EC_OK) )
		{		
			LockMode = (enum AVL_DVBSx_LockMode)((psChannel->m_Flags & CI_FLAG_LOCK_MODE_BIT_MASK) >> CI_FLAG_LOCK_MODE_BIT);
			if(LockMode == AVL_DVBSx_LOCK_MODE_ADAPTIVE)
			{
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_lock_mode_addr, 1);
				if(psChannel->m_uiSymbolRate_Hz < 3000000)
				{
					r |= AVL_DVBSx_IRx_SetFreqSweepRange(300, pAVLChip);
				}
				else
				{
					r |= AVL_DVBSx_IRx_SetFreqSweepRange(500, pAVLChip);
				}
			}
			else
			{
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_lock_mode_addr, 0);
			}
			IQ = ((psChannel->m_Flags) & CI_FLAG_IQ_BIT_MASK)>>CI_FLAG_IQ_BIT;
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_specinv_addr, IQ);
			Standard = (AVL_uint16)(((psChannel->m_Flags) & CI_FLAG_DVBS2_BIT_MASK)>>CI_FLAG_DVBS2_BIT);
			autoIQ_Detect = (((psChannel->m_Flags) & CI_FLAG_IQ_AUTO_BIT_MASK)>>CI_FLAG_IQ_AUTO_BIT);
			if((Standard == CI_FLAG_DVBS2_UNDEF) || (autoIQ_Detect == 1))
			{
				Standard = 0x14;
			}
			r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_decode_mode_addr, Standard);
			r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_iq_mode_addr, (AVL_uint16)autoIQ_Detect);
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_int_sym_rate_MHz_addr, psChannel->m_uiSymbolRate_Hz);
			r |= AVL_DVBSx_IBase_SendRxOP(OP_RX_INIT_GO, pAVLChip );
		}
		else
		{
			r = AVL_DVBSx_EC_GeneralFail;	
		}
	}
	else
	{
		r = AVL_DVBSx_EC_GeneralFail;
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetLockStatus( AVL_puint16 puiLockStatus, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Read16(pAVLChip, rs_fec_lock_addr, puiLockStatus);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ResetErrorStat( struct AVL_DVBSx_Chip * pAVLChip )
{
	enum AVL_DVBSx_FunctionalMode enumFunctionalMode;
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_IBase_GetFunctionalMode(&enumFunctionalMode, pAVLChip);

	if(enumFunctionalMode == AVL_DVBSx_FunctMode_Demod)
	{
		r |= AVL_DVBSx_IBase_SendRxOP( OP_RX_RESET_BERPER, pAVLChip );
	}
	else
	{
		r = AVL_DVBSx_EC_GeneralFail;
	}

	return(r);   
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetRFAGCPola( enum AVL_DVBSx_RfagcPola enumAGCPola, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 uiTemp;
	uiTemp = (AVL_uint32)enumAGCPola;
	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_rfagc_pol_addr, uiTemp);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegMode( const struct AVL_DVBSx_MpegInfo * pMpegMode, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_mode_addr, (AVL_uint32)(pMpegMode->m_MpegFormat));
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_mpeg_serial_addr, (AVL_uint16)(pMpegMode->m_MpegMode));
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_mpeg_posedge_addr, (AVL_uint16)(pMpegMode->m_MpegClockPolarity));

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegValidPolarity( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegValidPolarity enumValidPolarity )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint16 uiTemp;

	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rp_mpeg_config_addr, &uiTemp);
	uiTemp &= 0xFFFE;
	uiTemp |= (AVL_uint16)enumValidPolarity;
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rp_mpeg_config_addr, uiTemp);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegErrorPolarity(const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegErrorPolarity enumErrorLockPolarity, enum AVL_DVBSx_MpegErrorPolarity enumErrorUnlockPolarity)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint16 uiTemp;

	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rp_mpeg_config_addr, &uiTemp);
	uiTemp &= 0xFFF3;
	uiTemp |= (enumErrorLockPolarity << 2);
	uiTemp |= (enumErrorUnlockPolarity << 3);
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rp_mpeg_config_addr, uiTemp);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegBitOrder( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegMode enumMpegMode, enum AVL_DVBSx_MpegBitOrder enumMpegBitOrder )
{
	AVL_uint16 uiTemp;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rp_mpeg_config_addr, &uiTemp);
	uiTemp &= 0xFFFD;
	if(enumMpegMode == AVL_DVBSx_MPM_Serial)
	{
		AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_seri_seq_addr, (AVL_uint32)enumMpegBitOrder);	
	}
	else
	{
		uiTemp |= ((AVL_uint16)enumMpegBitOrder << 1);
	}
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rp_mpeg_config_addr, uiTemp);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegSerialPin( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegSerialPin enumSerialPin )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_outpin_sel_addr, enumSerialPin);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetSignalLevel(AVL_puint16 puiRFSignalLevel , const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uint32 uiData;
	AVL_uint16 uiSignalLevel;
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Read32(pAVLChip, (rx_aagc_gain), &uiData);

	if( AVL_DVBSx_EC_OK == r )
	{
		uiData += 0x800000;
		uiData &= 0xffffff;	
		uiSignalLevel = (AVL_uint16)(uiData>>8);
		*puiRFSignalLevel = uiSignalLevel;
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetScatterData( AVL_puchar ucpData, AVL_puint16 puiSize, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uint16 ucTemp1, ucTemp2;
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Read16(pAVLChip, rs_ScatterData_rdy_addr, &ucTemp1);
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, scatter_data_addr, &ucTemp2);
	if( (AVL_DVBSx_EC_OK != r) || (0 == ucTemp1) )
	{
		return(AVL_DVBSx_EC_Running);
	}
	if( ucTemp2>(*puiSize) )
	{
		ucTemp2 = (*puiSize);
	}
	else
	{
		(*puiSize) = ucTemp2;
	}

	r = AVL_DVBSx_II2C_Read(pAVLChip, scatter_data_addr+2, ucpData, (AVL_uint16)(ucTemp2<<1)); //both i and q
	ucTemp1 = 0;
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rs_ScatterData_rdy_addr, ucTemp1);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetSNR( AVL_puint32 puiSNR_db, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Read32(pAVLChip, rs_int_SNR_dB_addr, puiSNR_db);
	if( (*puiSNR_db) > 10000 )
	{
		// Not get stable SNR value yet.
		*puiSNR_db = 0;
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetPER( AVL_puint32 puiPER, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Read32(pAVLChip, rp_uint_PER_addr, puiPER);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetBER( AVL_puint32 puiBER, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Read32(pAVLChip, rp_uint_BER_addr, puiBER);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetDVBSBER( AVL_puint32 puiBER, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Read32(pAVLChip, rc_dvbs_ber_addr, puiBER);
	if( 0xffffffff == (*puiBER) )
	{
		r |= AVL_DVBSx_EC_GeneralFail;
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ResetDVBSBER( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_dvbs_ber_addr, 0xffffffff);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetRFOffset( AVL_pint16 piRFOffset_100kHz, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Read16(pAVLChip, rs_int_carrier_freq_100kHz_addr, (AVL_puint16)piRFOffset_100kHz);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetSignalInfo( struct AVL_DVBSx_SignalInfo * pSignalInfo, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 uiTemp;
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, rs_pilot_addr, &uiTemp);
	pSignalInfo->m_pilot = (enum AVL_DVBSx_Pilot)(uiTemp);
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, rs_code_rate_addr, &uiTemp);
	pSignalInfo->m_coderate = (enum AVL_DVBSx_FecRate)(uiTemp);
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, rs_modulation_addr, &uiTemp);
	pSignalInfo->m_modulation = (enum AVL_DVBSx_ModulationMode)(uiTemp);
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, rx_Rolloff_addr, &uiTemp);
	pSignalInfo->m_rolloff = (enum AVL_DVBSx_RollOff)((uiTemp>>22) & 0x03);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetDishPointingMode( AVL_uchar ucMode, const struct AVL_DVBSx_Chip *pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	enum AVL_DVBSx_FunctionalMode enumFunctionalMode;

	r |= AVL_DVBSx_IBase_GetFunctionalMode(&enumFunctionalMode, pAVLChip);
	if(enumFunctionalMode == AVL_DVBSx_FunctMode_Demod)
	{
		if(ucMode == 1)
		{
			r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_aagc_acq_gain_addr, 12);
			r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_dishpoint_mode_addr, 1);
		}
		else
		{
			r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_aagc_acq_gain_addr, 10);
			r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_dishpoint_mode_addr, 0);
		}
	}
	else
	{
		r = AVL_DVBSx_EC_GeneralFail;
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DriveMpegOutput( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_bus_tri_enb, 1);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ReleaseMpegOutput( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_bus_tri_enb, 0);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_EnableMpegPersistentClockMode( AVL_uint16 uiMpegDataClkFreq_10kHz, const struct AVL_DVBSx_Chip *pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	uiMpegDataClkFreq_10kHz |= 0x8000;
	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_mpeg_continuous_mode_control_addr, uiMpegDataClkFreq_10kHz);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DisableMpegPersistentClockMode( const struct AVL_DVBSx_Chip *pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_mpeg_continuous_mode_control_addr, 0);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_EnableMpegManualClockFrequency(AVL_uint16 uiMpegDataClkFreq_10kHz, enum AVL_DVBSx_MpegMode enumMpegMode, const struct AVL_DVBSx_Chip * pAVLChip) 
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint16 uiMpegRefClkFreq;
	AVL_uint32 uiTemp;
	r = AVL_DVBSx_II2C_Read32(pAVLChip, rc_mpeg_bus_cntrl_addr, &uiTemp );
	uiTemp |= 0x00000800;
	r |=AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_bus_cntrl_addr, uiTemp );

	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rc_int_mpeg_clk_MHz_addr, &uiMpegRefClkFreq);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_cntns_pkt_para_rate_frac_n_addr,(AVL_uint32)(uiMpegDataClkFreq_10kHz << 1));		
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_cntns_pkt_para_rate_frac_d_addr, (AVL_uint32)uiMpegRefClkFreq);		
	if(enumMpegMode == AVL_DVBSx_MPM_Serial)
	{
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_pkt_seri_rate_frac_n_addr, (AVL_uint32)(uiMpegDataClkFreq_10kHz<< 4));		
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_pkt_seri_rate_frac_d_addr, (AVL_uint32)uiMpegRefClkFreq);		
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DisableMpegManualClockFrequency(const struct AVL_DVBSx_Chip * pAVLChip)
{
	AVL_uint32 uiTemp;
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r = AVL_DVBSx_II2C_Read32(pAVLChip, rc_mpeg_bus_cntrl_addr, &uiTemp );
	uiTemp &= 0xFFFFF7FF;
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_bus_cntrl_addr, uiTemp );

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegPulldown( const struct AVL_DVBSx_Chip *pAVLChip, enum AVL_DVBSx_MpegPulldown enumPulldownState )
{
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_EC_OK;
	if(enumPulldownState == AVL_DVBSx_MPPD_Disable)
	{
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_bus_pe, 0);
	}
	else
	{
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, rc_mpeg_bus_pe, 0xFFF);
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DriveRFAGC( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_rfagc_tri_enb, 1);
	
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ReleaseRFAGC( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r = AVL_DVBSx_II2C_Write32(pAVLChip, rc_rfagc_tri_enb, 0);
	
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetIQ_Imbalance( const struct AVL_DVBSx_Chip * pAVLChip, AVL_pint16 piAmplitude, AVL_pint16 piPhase )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rp_amp_imb_addr, (AVL_puint16)piAmplitude);
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rp_phase_imb_addr, (AVL_puint16)piPhase);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetDeviceID( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puint32 puiDeviceID)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r = AVL_DVBSx_II2C_Read32(pAVLChip, rs_cust_chip_id_addr, puiDeviceID);

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetAdaptivePowerSaveMode(struct AVL_DVBSx_Channel * psChannel, AVL_uint16 uiEnable)
{
	if(uiEnable == 0)
	{
		psChannel->m_Flags &= ~CI_FLAG_ADAPTIVE_POWER_SAVE_BIT_MASK;
	}
	else
	{
		psChannel->m_Flags |= CI_FLAG_ADAPTIVE_POWER_SAVE_BIT_MASK;
	}

	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetIQ_Swap( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puint16 puiIQ_Swap )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 uiTemp1, uiTemp2;

	r |= AVL_DVBSx_II2C_Read32(pAVLChip, rc_specinv_addr, &uiTemp1);
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, rc_eq_out_iq_swap_addr, &uiTemp2);
	*puiIQ_Swap = (AVL_uint16)(uiTemp1 ^ uiTemp2);

	return(r);
}
#endif

