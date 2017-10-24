///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvMIU.c
/// @brief  MIU driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Inlucde Fils
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "drvMIU.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Internal Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 _MIU_ReadByte(U32 u32RegOffset)
{
    return ((volatile U8*)(MIUREG_BASE))[(u32RegOffset << 1) - (u32RegOffset & 1)];
}

U16 _MIU_Read2Bytes(U32 u32RegOffset)
{
    return ((volatile U16*)(MIUREG_BASE))[u32RegOffset];
}

void _MIU_WriteByte(U32 u32RegOffset, U8 u8Val)
{
    ((volatile U8*)(MIUREG_BASE))[(u32RegOffset << 1) - (u32RegOffset & 1)] = u8Val;
}

void _MIU_Write2Bytes(U32 u32RegOffset, U16 u16Val)
{
    ((volatile U16*)(MIUREG_BASE))[u32RegOffset] = u16Val;
}

void _MIU_WriteBytesBit(U32 u32RegOffset, U8 bEnable, U16 u16Mask)
{
    U16 val = _MIU_Read2Bytes(u32RegOffset);
    val = (bEnable) ? (val | u16Mask) : (val & ~u16Mask);
    _MIU_Write2Bytes(u32RegOffset, val);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Global Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum)
{
    u8 u8MIUGpPriority = 0, idx=0;
	U32 u32RegAddr = REG_MIU_GP_PRIORITY;

    u8MIUGpPriority=u8MIUGpPriority;
    idx=idx;
	if (u8GpNum > MIU_MAX_GP_NUM)
	{
		printf("%s: exced max group number!\n", __FUNCTION__);
		return FALSE;
	}

    for (idx = 0; idx < MIU_MAX_GP_NUM; idx++)
    {
        if (au8GpPriority[idx] > (MIU_MAX_GP_NUM-1))
        {
            printf("%s: wrong group number!\n", __FUNCTION__);
            return FALSE;
        }
        u8MIUGpPriority |= (au8GpPriority[idx] << (idx*2));
    }
    //printf("Set MIU%u group priority:%x.....\n", bMIU1, u8MIUGpPriority);

	u32RegAddr += (bMIU1) ? MIU1_REG_BASE : MIU0_REG_BASE;
	_MIU_WriteBytesBit(u32RegAddr,DISABLE, BIT8);
	_MIU_WriteByte(u32RegAddr, u8MIUGpPriority);
	_MIU_WriteBytesBit(u32RegAddr,ENABLE, BIT8);

	return TRUE;
}

const U16 client_gp[5] = {REG_MIUSEL_GP0, REG_MIUSEL_GP1, REG_MIUSEL_GP2, REG_MIUSEL_GP3, 0};
U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum)
{
	U8 idx;
	U32 u32RegAddr = (bMIU1) ? MIU1_REG_BASE : MIU0_REG_BASE;

	if (u8GpNum > MIU_MAX_GP_NUM)
	{
		printf("%s: exced max group number!\n", __FUNCTION__);
		return FALSE;
	}

    //printf("Change MIU%u select.....\n", bMIU1);
	for (idx = 0; idx < u8GpNum; idx++)
	{
	    //printf("Group%u:%-4x  ", idx, au16SelMiu[idx]);
		_MIU_Write2Bytes( ( u32RegAddr+client_gp[idx]), au16SelMiu[idx]);
    }
    //printf("\n");
    return TRUE;
}
