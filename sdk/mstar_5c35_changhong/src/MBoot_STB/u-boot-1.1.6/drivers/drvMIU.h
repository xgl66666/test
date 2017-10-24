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
/// @file   drvMIU.h
/// @brief  MIU driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRVMIU__
#define __DRVMIU__

#include <common.h>
#include <malloc.h>
#include <asm/io.h>

#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

#define FALSE       (0)
#define TRUE        (1)

#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)

#define		MIUREG_BASE		            (0xA0200000)
//Max MIU Group number
#define		MIU_MAX_GP_NUM		        (4)
//MIU group priority
#define		REG_MIU_GP_PRIORITY		    (0x6E << 1)

#else

#define		MIUREG_BASE		            (0xBF200000)
//Max MIU Group number
#define		MIU_MAX_GP_NUM		        (4)

//MIU group priority
#if defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
#define		REG_MIU_GP_PRIORITY		    (0x7F << 1)
#else
#define		REG_MIU_GP_PRIORITY		    (0x6E << 1)
#endif

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Register Map
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU0_REG_BASE				    (0x1200)
#define MIU1_REG_BASE				    (0x0600)

//MIU select group 0
#define		REG_MIUSEL_GP0				(0x78 << 1)
//MIU select group 1
#define		REG_MIUSEL_GP1				(0x79 << 1)
//MIU select group 2
#define		REG_MIUSEL_GP2				(0x7A << 1)
//MIU select group 3
#define		REG_MIUSEL_GP3				(0x7B << 1)

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Global Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);

#endif	//__DRVMIU__

