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


//$Revision: 323 $
//$Date: 2008-03-14 11:31:11 -0400 (Fri, 14 Mar 2008) $
//
#include "II2C.h"
#include "IBSP.h"

/// @cond
AVL_semaphore gI2CSem;
/// @endcond

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Initialize(void)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	static AVL_uchar gI2CSem_inited = 0;
	if( 0 == gI2CSem_inited )
	{
		gI2CSem_inited = 1;
		r = AVL_DVBSx_IBSP_InitSemaphore(&gI2CSem);
	}
	return r;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiOffset, AVL_puchar pucBuff, AVL_uint16 uiSize)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pucBuffTemp[3];
	AVL_uint16 ui1, ui2;
	AVL_uint16 iSize;

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(gI2CSem));
	if( AVL_DVBSx_EC_OK == r )
	{
		ChunkAddr(uiOffset, pucBuffTemp);
		ui1 = 3;
		r = AVL_DVBSx_IBSP_I2CWrite(pAVLChip->m_SlaveAddr, pucBuffTemp, &ui1);
		if( AVL_DVBSx_EC_OK == r )
		{
			if( uiSize & 1 )
			{
				iSize = uiSize - 1;
			}
			else
			{
				iSize = uiSize;
			}
			ui2 = 0;
			while( iSize > MAX_II2C_READ_SIZE )
			{
				ui1 = MAX_II2C_READ_SIZE;
				r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuff+ui2, &ui1);
				ui2 += MAX_II2C_READ_SIZE;
				iSize -= MAX_II2C_READ_SIZE;
			}

			if( 0 != iSize )
			{
				r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuff+ui2, &iSize);
			}

			if( uiSize & 1 )
			{
				ui1 = 2;
				r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuffTemp, &ui1);
				pucBuff[uiSize-1] = pucBuffTemp[0];
			}
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(gI2CSem));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_ReadDirect( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puchar pucBuff, AVL_uint16 uiSize)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pucBuffTemp[3];
	AVL_uint16 ui1, ui2;
	AVL_uint16 iSize;
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(gI2CSem));
	if( AVL_DVBSx_EC_OK == r )
	{
		if( uiSize & 1 )
		{
			iSize = uiSize - 1;
		}
		else
		{
			iSize = uiSize;
		}
		ui2 = 0;
		while( iSize > MAX_II2C_READ_SIZE )
		{
			ui1 = MAX_II2C_READ_SIZE;
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuff+ui2, &ui1);
			ui2 += MAX_II2C_READ_SIZE;
			iSize -= MAX_II2C_READ_SIZE;
		}

		if( 0 != iSize )
		{
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuff+ui2, &iSize);
		}

		if( uiSize & 1 )
		{
			ui1 = 2;
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuffTemp, &ui1);
			pucBuff[uiSize-1] = pucBuffTemp[0];
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(gI2CSem));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puchar pucBuff, AVL_uint16 uiSize)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pucBuffTemp[5];
	AVL_uint16 ui1, ui2, uTemp;
	AVL_uint16 iSize;
	AVL_uint32 uAddr;
	if( uiSize<3 )
	{
		return(AVL_DVBSx_EC_GeneralFail);	 //at least 3 bytes
	}

	uiSize -= 3;			//actual data size
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(gI2CSem));
	if( AVL_DVBSx_EC_OK == r )
	{
		//dump address
		uAddr = pucBuff[0];
		uAddr = uAddr<<8;
		uAddr += pucBuff[1];
		uAddr = uAddr<<8;
		uAddr += pucBuff[2];

		if( uiSize & 1 )
		{
			iSize = uiSize -1;
		}
		else
		{
			iSize = uiSize;
		}

		uTemp = (MAX_II2C_Write_SIZE-3) & 0xfffe; //how many bytes data we can transfer every time

		ui2 = 0;
		while( iSize > uTemp )
		{
			ui1 = uTemp+3;
			//save the data
			pucBuffTemp[0] = pucBuff[ui2];
			pucBuffTemp[1] = pucBuff[ui2+1];
			pucBuffTemp[2] = pucBuff[ui2+2];
			ChunkAddr(uAddr, pucBuff+ui2);
			r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip->m_SlaveAddr, pucBuff+ui2, &ui1);
			//restore data
			pucBuff[ui2] = pucBuffTemp[0];
			pucBuff[ui2+1] = pucBuffTemp[1];
			pucBuff[ui2+2] = pucBuffTemp[2];
			uAddr += uTemp;
			ui2 += uTemp;
			iSize -= uTemp;
		}
		ui1 = iSize+3;
		//save the data
		pucBuffTemp[0] = pucBuff[ui2];
		pucBuffTemp[1] = pucBuff[ui2+1];
		pucBuffTemp[2] = pucBuff[ui2+2];
		ChunkAddr(uAddr, pucBuff+ui2);
		r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip->m_SlaveAddr, pucBuff+ui2, &ui1);
		//restore data
		pucBuff[ui2] = pucBuffTemp[0];
		pucBuff[ui2+1] = pucBuffTemp[1];
		pucBuff[ui2+2] = pucBuffTemp[2];
		uAddr += iSize;
		ui2 += iSize;

		if( uiSize & 1 )
		{
			ChunkAddr(uAddr, pucBuffTemp);
			ui1 = 3;
			r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip->m_SlaveAddr, pucBuffTemp, &ui1);
			ui1 = 2;
			r |= AVL_DVBSx_IBSP_I2CRead(pAVLChip->m_SlaveAddr, pucBuffTemp+3, &ui1);
			pucBuffTemp[3] = pucBuff[ui2+3];
			ui1 = 5;
			r |= AVL_DVBSx_IBSP_I2CWrite(pAVLChip->m_SlaveAddr, pucBuffTemp, &ui1);
		}
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(gI2CSem));

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read16( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_puint16 puiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[2];

	r = AVL_DVBSx_II2C_Read(pAVLChip, uiAddr, pBuff, 2);
	if( AVL_DVBSx_EC_OK == r )
	{
		*puiData = DeChunk16(pBuff);
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Read32( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_puint32 puiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[4];

	r = AVL_DVBSx_II2C_Read(pAVLChip, uiAddr, pBuff, 4);
	if( AVL_DVBSx_EC_OK == r )
	{
		*puiData = DeChunk32(pBuff);
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write16( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_uint16 uiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[5];

	ChunkAddr(uiAddr, pBuff);
	Chunk16(uiData, pBuff+3);

	r = AVL_DVBSx_II2C_Write(pAVLChip, pBuff, 5);
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2C_Write32( const struct AVL_DVBSx_Chip * pAVLChip, AVL_uint32 uiAddr, AVL_uint32 uiData )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[7];

	ChunkAddr(uiAddr, pBuff);
	Chunk32(uiData, pBuff+3);
	r = AVL_DVBSx_II2C_Write(pAVLChip, pBuff, 7);
	return(r);
}
