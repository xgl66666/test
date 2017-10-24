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


//$Revision: 581 $
//$Date: 2008-10-23 13:31:49 -0400 (Thu, 23 Oct 2008) $
//
#include "Board.h"
#if(FRONTEND_DEMOD_TYPE == DEMOD_AVL6211)
#include "II2CRepeater.h"
#include "IBSP.h"
#include "II2C.h"

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendOP(AVL_puchar pBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	const AVL_uint16 uiTimeDelay = 10;
	const AVL_uint16 uiMaxRetries = 20;
	AVL_uint32 i;

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semI2CRepeater));

	i = 0;
	while (AVL_DVBSx_EC_OK != AVL_DVBSx_II2CRepeater_GetOPStatus(pAVLChip))
	{
		if (uiMaxRetries < i++)
		{
			r |= AVL_DVBSx_EC_Running;
			break;
		}
		AVL_DVBSx_IBSP_Delay(uiTimeDelay);
	}

	if ( AVL_DVBSx_EC_OK == r )
	{
		r = AVL_DVBSx_II2C_Write(pAVLChip, pBuff, ucSize);
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_Initialize( AVL_uint16 I2CBusClock_kHz, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uchar pBuff[5];
	AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Write16(pAVLChip, rc_i2cm_speed_kHz_addr, I2CBusClock_kHz);
	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-2, pBuff);
	pBuff[3] = 0x01;
	pBuff[4] = OP_I2CM_INIT;
	r |= AVL_DVBSx_II2CRepeater_SendOP(pBuff, 5, pAVLChip);
	return r;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_GetOPStatus( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[2];
	r = AVL_DVBSx_II2C_Read(pAVLChip, i2cm_cmd_addr+I2CM_CMD_LENGTH-2, pBuff, 2);
	if ( AVL_DVBSx_EC_OK == r )
	{
		if ( pBuff[1] != 0 )
		{
			r = AVL_DVBSx_EC_Running;
		}
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData( AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[I2CM_RSP_LENGTH];
	AVL_uint16 uiTimeOut;
	const AVL_uint16 uiTimeOutTh = 10;
	const AVL_uint32 uiTimeDelay = 100;  //100 ms

	if ( uiSize > I2CM_RSP_LENGTH )
	{
		return(AVL_DVBSx_EC_GeneralFail);
	}
	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semI2CRepeater_r));

	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-4, pBuff);
	pBuff[3] = 0x0;
	pBuff[4] = (AVL_uchar)uiSize;
	pBuff[5] = ucSlaveAddr;
	pBuff[6] = OP_I2CM_READ;
	r |= AVL_DVBSx_II2CRepeater_SendOP(pBuff, 7, pAVLChip);
	if ( AVL_DVBSx_EC_OK == r )
	{
		uiTimeOut = 0;
		while ( AVL_DVBSx_EC_OK != AVL_DVBSx_II2CRepeater_GetOPStatus(pAVLChip) )
		{
			if((++uiTimeOut) >= uiTimeOutTh)
			{
				r |= AVL_DVBSx_EC_TimeOut;
				r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));
				return(r);
			}
			r |= AVL_DVBSx_IBSP_Delay(uiTimeDelay);
		}
		r |= (AVL_DVBSx_II2C_Read(pAVLChip, i2cm_rsp_addr, pucBuff, uiSize));
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData_Multi(  AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uchar ucRegAddr, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip )
{
   	AVL_DVBSx_ErrorCode r;
	AVL_uchar pBuff[I2CM_RSP_LENGTH];
	AVL_uint16 uiTimeOut;
	const AVL_uint16 uiTimeOutTh = 10;
	const AVL_uint32 uiTimeDelay = 100;  //100 ms

	if ( uiSize > I2CM_RSP_LENGTH )
	{
		return(AVL_DVBSx_EC_GeneralFail);
	}

	r = AVL_DVBSx_IBSP_WaitSemaphore(&(pAVLChip->m_semI2CRepeater_r));

	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-6, pBuff);
	pBuff[3] = 0;
	pBuff[4] = ucRegAddr;
	pBuff[5] = 0x1;
	pBuff[6] = (AVL_uchar)uiSize;
	pBuff[7] = ucSlaveAddr;
	pBuff[8] = OP_I2CM_READ;
	r |= AVL_DVBSx_II2CRepeater_SendOP(pBuff, 9, pAVLChip);
	if ( AVL_DVBSx_EC_OK == r )
	{
		uiTimeOut = 0;
		while ( AVL_DVBSx_EC_OK != AVL_DVBSx_II2CRepeater_GetOPStatus(pAVLChip) )
		{
			if((++uiTimeOut) >= uiTimeOutTh)
			{
				r |= AVL_DVBSx_EC_TimeOut;
				r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));
				return(r);
			}
			r |= AVL_DVBSx_IBSP_Delay(uiTimeDelay);
		}
		r |= (AVL_DVBSx_II2C_Read(pAVLChip, i2cm_rsp_addr, pucBuff, uiSize));
	}
	r |= AVL_DVBSx_IBSP_ReleaseSemaphore(&(pAVLChip->m_semI2CRepeater_r));

	return(r);
}


AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendData(  AVL_uchar ucSlaveAddr, const AVL_puchar pucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_uchar pBuff[I2CM_CMD_LENGTH+3];
	AVL_uint16 i1, i2;
	AVL_uint16 cmdSize;

	if ( uiSize>I2CM_CMD_LENGTH-3 )
	{
		return(AVL_DVBSx_EC_GeneralFail);
	}

	cmdSize = ((3+uiSize)%2)+3+uiSize;	  /* How many bytes need send to Availink device through i2c interface */
	ChunkAddr(i2cm_cmd_addr+I2CM_CMD_LENGTH-cmdSize, pBuff);

	i1 = 3+((3+uiSize)%2);	  /* skip one byte if the uisize+3 is odd*/

	for ( i2=0; i2<uiSize; i2++ )
	{
		pBuff[i1++] = pucBuff[i2];
	}
	pBuff[i1++] = (AVL_uchar)uiSize;
	pBuff[i1++] = ucSlaveAddr;
	pBuff[i1++] = OP_I2CM_WRITE;

	return(AVL_DVBSx_II2CRepeater_SendOP(pBuff, (AVL_uchar)(cmdSize+3), pAVLChip));
}
#endif

