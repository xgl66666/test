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


//$Revision: 355 $
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
//

#include "Board.h"
#if(FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)

#include "IDiseqc.h"
#include "II2C.h"
#include "IBSP.h"
#include "IBase.h"

///@cond
enum AVL_DVBSx_Diseqc_OPStatus
{
	AVL_DVBSx_DOS_Uninitialized = 0,	///< = 0  Diseqc has not been initialized yet.
	AVL_DVBSx_DOS_Initialized   = 1,	///< = 1  Diseqc has been initialized.
	AVL_DVBSx_DOS_InContinuous  = 2,	///< = 2  Diseqc is in continuous mode.
	AVL_DVBSx_DOS_InTone        = 3,	///< = 3  Diseqc is in tone burst mode.
	AVL_DVBSx_DOS_InModulation  = 4		///< = 4  Diseqc is in modulation mode.
};

static enum AVL_DVBSx_Diseqc_OPStatus Diseqc_OP_Status = AVL_DVBSx_DOS_Uninitialized;

/// Check if it is safe to switch Diseqc operation mode.
///
/// @param pAVLChip A pointer point to a ::AVL_DVBSx_Chip object which is used to tell function which chip it is working on.
///
/// @return ::AVL_DVBSx_ErrorCode,
/// Return ::AVL_DVBSx_EC_OK if it is OK to switch.
/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch since the last transmit is not done yet.
/// Return ::AVL_DVBSx_EC_I2CFail if there is a I2C problem.
AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_IsSafeToSwitchMode( struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint32 i1;
	switch( Diseqc_OP_Status )
	{
	case AVL_DVBSx_DOS_InModulation:
	case AVL_DVBSx_DOS_InTone:
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_st_addr, &i1);
		if( 1 != ((i1 & 0x00000040) >> 6) )	//check if the last transmit is done
		{
			r |= AVL_DVBSx_EC_Running;
		}
		break;
	case AVL_DVBSx_DOS_InContinuous:
	case AVL_DVBSx_DOS_Initialized:
		break;
	default:
		r |= AVL_DVBSx_EC_GeneralFail;
		break;
	}
	return(r);
}

///@endcond

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_Initialize( const struct AVL_DVBSx_Diseqc_Para * pDiseqcPara, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	if( AVL_DVBSx_EC_OK == r )
	{
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_srst_addr, 1);

		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_samp_frac_n_addr, 200);		//2M=200*10kHz
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_samp_frac_d_addr, pAVLChip->m_DemodFrequency_10kHz);

		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tone_frac_n_addr, ((pDiseqcPara->m_ToneFrequency_kHz)<<1));
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tone_frac_d_addr, pAVLChip->m_DemodFrequency_10kHz*10);

		// Initialize the tx_control
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
		i1 &= 0x00000300;
		i1 |= 0x20;		//reset tx_fifo
		i1 |= ((AVL_uint32)(pDiseqcPara->m_TXGap) << 6);
		i1 |= ((AVL_uint32)(pDiseqcPara->m_TxWaveForm) << 4);
		i1 |= (1<<3);			//enable tx gap.
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);
		i1 &= ~(0x20);	//release tx_fifo reset
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);

		// Initialize the rx_control
		i1 = ((AVL_uint32)(pDiseqcPara->m_RxWaveForm) << 2);
		i1 |= (1<<1);	//active the receiver
		i1 |= (1<<3);	//enveolop high when tone present
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_rx_cntrl_addr, i1);
		i1 = (AVL_uint32)(pDiseqcPara->m_RxTimeout);
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_rx_msg_tim_addr, i1);

		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_srst_addr, 0);

		if( AVL_DVBSx_EC_OK == r )
		{
			Diseqc_OP_Status = AVL_DVBSx_DOS_Initialized;
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_ReadModulationData( AVL_puchar pucBuff, AVL_puchar pucSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1,i2;
	AVL_uchar pucBuffTemp[4];
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_rx_st_addr, &i1);
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i2);
	if((i2>>8) & 0x01)
	{
		Diseqc_OP_Status = AVL_DVBSx_DOS_InModulation;
	}
	if( AVL_DVBSx_DOS_InModulation == Diseqc_OP_Status )
	{
		// In modulation mode
		if( (!((i2>>8) & 0x01 ) && (0x00000004 == (i1 & 0x00000004)) )||( ((i2>>8) & 0x01 ) &&(0x00000004 != (i1 & 0x00000004))))
		{
			*pucSize = (AVL_uchar)((i1 & 0x00000078)>>3);
			//Receive data
			for( i1=0; i1<*pucSize; i1++ )
			{
				r |= AVL_DVBSx_II2C_Read(pAVLChip, diseqc_rx_fifo_map_addr, pucBuffTemp, 4);
				pucBuff[i1] = pucBuffTemp[3];
			}
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

	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SendModulationData( const AVL_puchar pucBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1, i2;
	AVL_uchar pucBuffTemp[8];

	if( ucSize>8 )
	{
		r = AVL_DVBSx_EC_MemoryRunout;
	}
	else
	{
		r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
		r |= AVL_DVBSx_IDiseqc_IsSafeToSwitchMode(pAVLChip);
		if( AVL_DVBSx_EC_OK ==  r)
		{
			//reset rx_fifo
			r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_rx_cntrl_addr, &i2);
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_rx_cntrl_addr, (i2|0x01));
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_rx_cntrl_addr, (i2&0xfffffffe));

			r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
			i1 &= 0xfffffff8;	//set to modulation mode and put it to FIFO load mode
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);

			//trunk address
			ChunkAddr(diseqc_tx_fifo_map_addr, pucBuffTemp);
			pucBuffTemp[3] = 0;
			pucBuffTemp[4] = 0;
			pucBuffTemp[5] = 0;
			for( i2=0; i2<ucSize; i2++ )
			{
				pucBuffTemp[6] = pucBuff[i2];

				r |= AVL_DVBSx_II2C_Write(pAVLChip, pucBuffTemp, 7);
			}
			i1 |= (1<<2);  //start fifo transmit.
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);

			if( AVL_DVBSx_EC_OK == r )
			{
				Diseqc_OP_Status = AVL_DVBSx_DOS_InModulation;
			}
		}
		r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetTxStatus( struct AVL_DVBSx_Diseqc_TxStatus * pTxStatus, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	if( (AVL_DVBSx_DOS_InModulation == Diseqc_OP_Status) || (AVL_DVBSx_DOS_InTone == Diseqc_OP_Status) )
	{
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_st_addr, &i1);
		pTxStatus->m_TxDone = (AVL_uchar)((i1 & 0x00000040)>>6);
		pTxStatus->m_TxFifoCount = (AVL_uchar)((i1 & 0x0000003c)>>2);
	}
	else
	{
		r |= AVL_DVBSx_EC_GeneralFail;
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetRxStatus( struct AVL_DVBSx_Diseqc_RxStatus * pRxStatus, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	if( AVL_DVBSx_DOS_InModulation == Diseqc_OP_Status )
	{
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_rx_st_addr, &i1);
		pRxStatus->m_RxDone = (AVL_uchar)((i1 & 0x00000004)>>2);
		pRxStatus->m_RxFifoCount = (AVL_uchar)((i1 & 0x000000078)>>3);
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_rx_parity_addr, &i1);
		pRxStatus->m_RxFifoParChk = (AVL_uchar)(i1 & 0x000000ff);
	}
	else
	{
		r |= AVL_DVBSx_EC_GeneralFail;
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SetLNBOut( AVL_uchar uiOut, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
	i1 &= 0xfffffdff;
	i1 |= ((uiOut & 0x1)<<9);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetLNBOut( AVL_puchar puiOut, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
	i1 &= 0x00000200;
	*puiOut = (AVL_uchar)(i1>>9);
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SendTone( AVL_uchar ucTone, AVL_uchar ucCount, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1, i2;
	AVL_uchar pucBuffTemp[8];
	if( ucCount>8 )
	{
		r = AVL_DVBSx_EC_MemoryRunout;
	}
	else
	{
		r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
		r |= AVL_DVBSx_IDiseqc_IsSafeToSwitchMode(pAVLChip);

		if( AVL_DVBSx_EC_OK == r )
		{
			//No data in the FIFO.
			r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
			i1 &= 0xfffffff8;  //put it into the FIFO load mode.
			if( 0 == ucTone )
			{
				i1 |= 0x01;
			}
			else
			{
				i1 |= 0x02;
			}
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);

			//trunk address
			ChunkAddr(diseqc_tx_fifo_map_addr, pucBuffTemp);
			pucBuffTemp[3] = 0;
			pucBuffTemp[4] = 0;
			pucBuffTemp[5] = 0;
			pucBuffTemp[6] = 1;

			for( i2=0; i2<ucCount; i2++ )
			{
				r |= AVL_DVBSx_II2C_Write(pAVLChip, pucBuffTemp, 7);
			}

			i1 |= (1<<2);  //start fifo transmit.
			r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);
			if( AVL_DVBSx_EC_OK == r )
			{
				Diseqc_OP_Status = AVL_DVBSx_DOS_InTone;
			}
		}
		r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_StartContinuous (struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	r |= AVL_DVBSx_IDiseqc_IsSafeToSwitchMode(pAVLChip);

	if( AVL_DVBSx_EC_OK == r )
	{
		//No data in FIFO
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
		i1 &= 0xfffffff8;
		i1 |= 0x03;		//switch to continuous mosw
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);

		//start to send out wave
		i1 |= (1<<10);
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);
		if( AVL_DVBSx_EC_OK == r )
		{
			Diseqc_OP_Status = AVL_DVBSx_DOS_InContinuous;
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_StopContinuous (struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	if( AVL_DVBSx_DOS_InContinuous == Diseqc_OP_Status )
	{
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
		i1 &= 0xfffff3ff;
		r |= AVL_DVBSx_II2C_Write32(pAVLChip, diseqc_tx_cntrl_addr, i1);
	}

	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetContinuous (AVL_puchar puiCon,struct AVL_DVBSx_Chip * pAVLChip )
{
      AVL_DVBSx_ErrorCode r;
	AVL_uint32 i1;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	if( AVL_DVBSx_DOS_InContinuous == Diseqc_OP_Status )
	{
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, diseqc_tx_cntrl_addr, &i1);
		i1 &= 0x00000403;
	      *puiCon = (AVL_uchar)(i1>>10);
	}

	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SetLNB1Out( AVL_uchar uiOut, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 uiTemp;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, gpio_reg_enb, &uiTemp);
	uiTemp &= ~(1<<1);
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, gpio_reg_enb, uiTemp);

	r |= AVL_DVBSx_II2C_Read32(pAVLChip, gpio_data_reg_out, &uiTemp);
	if(uiOut)
	{
		uiTemp |= 1<<1 ;
	}
	else
	{
		uiTemp &= ~(1<<1) ;
	}
	r |= AVL_DVBSx_II2C_Write32(pAVLChip, gpio_data_reg_out, uiTemp);
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetLNB1Out( AVL_puchar puiOut, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 uiTemp;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semDiseqc));
	r |= AVL_DVBSx_II2C_Read32(pAVLChip, gpio_data_reg_out, &uiTemp);
	if(uiTemp & (1<<1))
	{
		*puiOut = 1;
	}
	else
	{
		*puiOut = 0;
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semDiseqc));
	return(r);
}
#endif

