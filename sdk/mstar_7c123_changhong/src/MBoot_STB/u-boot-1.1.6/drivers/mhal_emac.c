////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mhal_emac.c
/// @brief  EMAC Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
//#include <linux/mii.h>
//#include <linux/delay.h>
//include <linux/netdevice.h>
//#include <linux/ethtool.h>
//#include <linux/pci.h>
#include <asm/types.h>
#include "mhal_emac.h"

//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
struct _MHalBasicConfigEMAC
{
    u8                  connected;          // 0:No, 1:Yes    <== (20070515) Wait for Julian's reply
	u8					speed;				// 10:10Mbps, 100:100Mbps
	// ETH_CTL Register:
	u8                  wes;				// 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
	// ETH_CFG Register:
	u8					duplex;				// 0:Half-duplex, 1:Full-duplex
	u8					cam;                // 0:No CAM, 1:Yes
	u8 					rcv_bcast;      	// 0:No, 1:Yes
	u8					rlf;                // 0:No, 1:Yes receive long frame(1522)
	// MAC Address:
	u8					sa1[6];				// Specific Addr 1 (MAC Address)
	u8					sa2[6];				// Specific Addr 2
	u8					sa3[6];				// Specific Addr 3
	u8					sa4[6];				// Specific Addr 4
};
typedef struct _MHalBasicConfigEMAC MHalBasicConfigEMAC;

struct _MHalUtilityVarsEMAC
{
    u32                 cntChkCableConnect;
    u32                 cntChkINTCounter;
	u32 				readIdxRBQP;		// Reset = 0x00000000
	u32 				rxOneFrameAddr;		// Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")

	u8                  flagISR_INT_DONE;
};
typedef struct _MHalUtilityVarsEMAC MHalUtilityVarsEMAC;

MHalBasicConfigEMAC MHalThisBCE;
MHalUtilityVarsEMAC MHalThisUVE;

typedef volatile unsigned int EMAC_REG;

typedef struct _TITANIA_EMAC {
// Constant: ----------------------------------------------------------------
// Register MAP:
  EMAC_REG   REG_EMAC_CTL_L; 	      //0x00000000 Network Control Register Low  16 bit
  EMAC_REG   REG_EMAC_CTL_H; 	      //0x00000004 Network Control Register High 16 bit
  EMAC_REG   REG_EMAC_CFG_L; 	      //0x00000008 Network Configuration Register Low  16 bit
  EMAC_REG   REG_EMAC_CFG_H; 	      //0x0000000C Network Configuration Register High 16 bit
  EMAC_REG   REG_EMAC_SR_L; 	      //0x00000010 Network Status Register Low  16 bit
  EMAC_REG   REG_EMAC_SR_H; 	      //0x00000014 Network Status Register High 16 bit
  EMAC_REG   REG_EMAC_TAR_L; 	      //0x00000018 Transmit Address Register Low  16 bit
  EMAC_REG   REG_EMAC_TAR_H; 	      //0x0000001C Transmit Address Register High 16 bit
  EMAC_REG   REG_EMAC_TCR_L; 	      //0x00000020 Transmit Control Register Low  16 bit
  EMAC_REG   REG_EMAC_TCR_H; 	      //0x00000024 Transmit Control Register High 16 bit
  EMAC_REG   REG_EMAC_TSR_L; 	      //0x00000028 Transmit Status Register Low  16 bit
  EMAC_REG   REG_EMAC_TSR_H; 	      //0x0000002C Transmit Status Register High 16 bit
  EMAC_REG   REG_EMAC_RBQP_L;         //0x00000030 Receive Buffer Queue Pointer Low  16 bit
  EMAC_REG   REG_EMAC_RBQP_H;         //0x00000034 Receive Buffer Queue Pointer High 16 bit
  EMAC_REG   REG_EMAC_RBCFG_L;        //0x00000038 Receive buffer configuration Low  16 bit
  EMAC_REG   REG_EMAC_RBCFG_H;        //0x0000003C Receive buffer configuration High 16 bit
  EMAC_REG   REG_EMAC_RSR_L; 	      //0x00000040 Receive Status Register Low  16 bit
  EMAC_REG   REG_EMAC_RSR_H; 	      //0x00000044 Receive Status Register High 16 bit
  EMAC_REG   REG_EMAC_ISR_L;          //0x00000048 Interrupt Status Register Low  16 bit
  EMAC_REG   REG_EMAC_ISR_H;          //0x0000004C Interrupt Status Register High 16 bit
  EMAC_REG   REG_EMAC_IER_L;          //0x00000050 Interrupt Enable Register Low  16 bit
  EMAC_REG   REG_EMAC_IER_H;          //0x00000054 Interrupt Enable Register High 16 bit
  EMAC_REG   REG_EMAC_IDR_L; 	      //0x00000058 Interrupt Disable Register Low  16 bit
  EMAC_REG   REG_EMAC_IDR_H; 	      //0x0000005C Interrupt Disable Register High 16 bit
  EMAC_REG   REG_EMAC_IMR_L; 	      //0x00000060 Interrupt Mask Register Low  16 bit
  EMAC_REG   REG_EMAC_IMR_H; 	      //0x00000064 Interrupt Mask Register High 16 bit
  EMAC_REG   REG_EMAC_MAN_L; 	      //0x00000068 PHY Maintenance Register Low  16 bit
  EMAC_REG   REG_EMAC_MAN_H; 	      //0x0000006C PHY Maintenance Register High 16 bit
  EMAC_REG   REG_EMAC_RBRP_L;         //0x00000070 Receive Buffer First full pointer Low  16 bit
  EMAC_REG   REG_EMAC_RBRP_H;         //0x00000074 Receive Buffer First full pointer High 16 bit
  EMAC_REG   REG_EMAC_RBWP_L;         //0x00000078 Receive Buffer Current pointer Low  16 bit
  EMAC_REG   REG_EMAC_RBWP_H;         //0x0000007C Receive Buffer Current pointer High 16 bit
  EMAC_REG   REG_EMAC_FRA_L; 	      //0x00000080 Frames Transmitted OK Register Low  16 bit
  EMAC_REG   REG_EMAC_FRA_H; 	      //0x00000084 Frames Transmitted OK Register High 16 bit
  EMAC_REG   REG_EMAC_SCOL_L;         //0x00000088 Single Collision Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_SCOL_H;         //0x0000008C Single Collision Frame Register High 16 bit
  EMAC_REG   REG_EMAC_MCOL_L;         //0x00000090 Multiple Collision Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_MCOL_H;         //0x00000094 Multiple Collision Frame Register High 16 bit
  EMAC_REG   REG_EMAC_OK_L; 	      //0x00000098 Frames Received OK Register Low  16 bit
  EMAC_REG   REG_EMAC_OK_H; 	      //0x0000009C Frames Received OK Register High 16 bit
  EMAC_REG   REG_EMAC_SEQE_L;         //0x000000A0 Frame Check Sequence Error Register Low  16 bit
  EMAC_REG   REG_EMAC_SEQE_H;         //0x000000A4 Frame Check Sequence Error Register High 16 bit
  EMAC_REG   REG_EMAC_ALE_L; 	      //0x000000A8 Alignment Error Register Low  16 bit
  EMAC_REG   REG_EMAC_ALE_H; 	      //0x000000AC Alignment Error Register High 16 bit
  EMAC_REG   REG_EMAC_DTE_L; 	      //0x000000B0 Deferred Transmission Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_DTE_H; 	      //0x000000B4 Deferred Transmission Frame Register High 16 bit
  EMAC_REG   REG_EMAC_LCOL_L;         //0x000000B8 Late Collision Register Low  16 bit
  EMAC_REG   REG_EMAC_LCOL_H;         //0x000000BC Late Collision Register High 16 bit
  EMAC_REG   REG_EMAC_ECOL_L;         //0x000000C0 Excessive Collision Register Low  16 bit
  EMAC_REG   REG_EMAC_ECOL_H;         //0x000000C4 Excessive Collision Register High 16 bit
  EMAC_REG   REG_EMAC_TUE_L; 	      //0x000000C8 Transmit Underrun Error Register Low  16 bit
  EMAC_REG   REG_EMAC_TUE_H; 	      //0x000000CC Transmit Underrun Error Register High 16 bit
  EMAC_REG   REG_EMAC_CSE_L;          //0x000000D0 Carrier sense errors Register Low  16 bit
  EMAC_REG   REG_EMAC_CSE_H;          //0x000000D4 Carrier sense errors Register High 16 bit
  EMAC_REG   REG_EMAC_RE_L;           //0x000000D8 Receive resource error Low  16 bit
  EMAC_REG   REG_EMAC_RE_H;           //0x000000DC Receive resource error High 16 bit
  EMAC_REG   REG_EMAC_ROVR_L;         //0x000000E0 Received overrun Low  16 bit
  EMAC_REG   REG_EMAC_ROVR_H;         //0x000000E4 Received overrun High 16 bit
  EMAC_REG   REG_EMAC_SE_L;           //0x000000E8 Received symbols error Low  16 bit
  EMAC_REG   REG_EMAC_SE_H;           //0x000000EC Received symbols error High 16 bit
//  EMAC_REG	 REG_EMAC_CDE; 	      //           Code Error Register
  EMAC_REG   REG_EMAC_ELR_L; 	      //0x000000F0 Excessive Length Error Register Low  16 bit
  EMAC_REG   REG_EMAC_ELR_H; 	      //0x000000F4 Excessive Length Error Register High 16 bit

  EMAC_REG   REG_EMAC_RJB_L; 	      //0x000000F8 Receive Jabber Register Low  16 bit
  EMAC_REG   REG_EMAC_RJB_H; 	      //0x000000FC Receive Jabber Register High 16 bit
  EMAC_REG   REG_EMAC_USF_L; 	      //0x00000100 Undersize Frame Register Low  16 bit
  EMAC_REG   REG_EMAC_USF_H; 	      //0x00000104 Undersize Frame Register High 16 bit
  EMAC_REG   REG_EMAC_SQEE_L; 	      //0x00000108 SQE Test Error Register Low  16 bit
  EMAC_REG   REG_EMAC_SQEE_H; 	      //0x0000010C SQE Test Error Register High 16 bit
//  EMAC_REG	 REG_EMAC_DRFC;       //           Discarded RX Frame Register
  EMAC_REG   REG_Reserved1_L; 	      //0x00000110 Low  16 bit
  EMAC_REG   REG_Reserved1_H; 	      //0x00000114 High 16 bit
  EMAC_REG   REG_Reserved2_L; 	      //0x00000118 Low  16 bit
  EMAC_REG   REG_Reserved2_H; 	      //0x0000011C High 16 bit
  EMAC_REG   REG_EMAC_HSH_L; 	      //0x00000120 Hash Address High[63:32] Low  16 bit
  EMAC_REG   REG_EMAC_HSH_H; 	      //0x00000124 Hash Address High[63:32] High 16 bit
  EMAC_REG   REG_EMAC_HSL_L; 	      //0x00000128 Hash Address Low[31:0] Low  16 bit
  EMAC_REG   REG_EMAC_HSL_H; 	      //0x0000012C Hash Address Low[31:0] High 16 bit

  EMAC_REG   REG_EMAC_SA1L_L;         //0x00000130 Specific Address 1 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA1L_H;         //0x00000134 Specific Address 1 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA1H_L;         //0x00000138 Specific Address 1 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA1H_H;         //0x0000013C Specific Address 1 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA2L_L;         //0x00000140 Specific Address 2 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA2L_H;         //0x00000144 Specific Address 2 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA2H_L;         //0x00000148 Specific Address 2 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA2H_H;         //0x0000014C Specific Address 2 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA3L_L;         //0x00000150 Specific Address 3 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA3L_H;         //0x00000154 Specific Address 3 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA3H_L;         //0x00000158 Specific Address 3 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA3H_H;         //0x0000015C Specific Address 3 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA4L_L;         //0x00000160 Specific Address 4 Low, First 4 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA4L_H;         //0x00000164 Specific Address 4 Low, First 4 bytes High 16 bit
  EMAC_REG   REG_EMAC_SA4H_L;         //0x00000168 Specific Address 4 High, Last 2 bytes Low  16 bit
  EMAC_REG   REG_EMAC_SA4H_H;         //0x0000016C Specific Address 4 High, Last 2 bytes High 16 bit
  EMAC_REG   REG_TAG_TYPE_L; 	      //0x00000170 tag type of the frame Low  16 bit
  EMAC_REG   REG_TAG_TYPE_H; 	      //0x00000174 tag type of the frame High 16 bit
  EMAC_REG   REG_Reserved3[34];       //0x00000178 Low  16 bit
  EMAC_REG   REG_JULIAN_0100_L;       //0x00000200
  EMAC_REG   REG_JULIAN_0100_H;       //0x00000204
  EMAC_REG   REG_JULIAN_0104_L;       //0x00000208
  EMAC_REG   REG_JULIAN_0104_H;       //0x0000020C
  EMAC_REG   REG_JULIAN_0108_L;       //0x00000210
  EMAC_REG   REG_JULIAN_0108_H;       //0x00000214
} TITANIA_EMAC_Str, *TITANIA_EMAC;

#define MHal_MAX_INT_COUNTER    100
//-------------------------------------------------------------------------------------------------
//  EMAC hardware for Titania
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_WritRam32(u32 uRamAddr, u32 xoffset,u32 xval)
{
	*((u32*)((char*)uRamAddr + xoffset)) = xval;
}

u32 MHal_EMAC_ReadReg32( u32 xoffset )
{
    volatile u32 *ptrs = (u32*)REG_ADDR_BASE;
    volatile u32 val_l = *(ptrs + (xoffset >> 1)) & 0x0000FFFF;
    volatile u32 val_h = (*(ptrs+ (xoffset >> 1) + 1) & 0x0000FFFF) << 0x10;
    return( val_l | val_h );
}

void MHal_EMAC_WritReg32( u32 xoffset, u32 xval )
{
    u32 address = REG_ADDR_BASE + xoffset*2;

    *( ( volatile u32 * ) address ) = ( u32 ) ( xval & 0x0000FFFF );
    *( ( volatile u32 * ) ( address + 4 ) ) = ( u32 ) ( xval >> 0x10 );
}

void MHal_EMAC_Write_SA1_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA1L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA1L_H= w0>>16;
	regs->REG_EMAC_SA1H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA1H_H= w1>>16;
}

void MHal_EMAC_Write_SA2_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA2L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA2L_H= w0>>16;
	regs->REG_EMAC_SA2H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA2H_H= w1>>16;
}

void MHal_EMAC_Write_SA3_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA3L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA3L_H= w0>>16;
	regs->REG_EMAC_SA3H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA3H_H= w1>>16;
}

void MHal_EMAC_Write_SA4_MAC_Address(u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
	u32 w1 = (u32)m5 <<  8 | m4;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

	regs->REG_EMAC_SA4L_L= w0&0x0000FFFF;
    regs->REG_EMAC_SA4L_H= w0>>16;
	regs->REG_EMAC_SA4H_L= w1&0x0000FFFF;
    regs->REG_EMAC_SA4H_H= w1>>16;
}

//-------------------------------------------------------------------------------------------------
//  R/W EMAC register for Titania
//-------------------------------------------------------------------------------------------------

void MHal_EMAC_update_HSH(u8 mc1, u8 mc0)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_HSL_L= mc1&0x0000FFFF;
    regs->REG_EMAC_HSL_H= mc1>>16;
	regs->REG_EMAC_HSH_L= mc0&0x0000FFFF;
    regs->REG_EMAC_HSH_H= mc0>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read control register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CTL(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_CTL_L)&0x0000FFFF)+((regs->REG_EMAC_CTL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Network control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CTL(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_CTL_L= xval&0x0000FFFF;
    regs->REG_EMAC_CTL_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Network configuration register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CFG(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_CFG_L)&0x0000FFFF)+((regs->REG_EMAC_CFG_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Network configuration register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CFG(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_CFG_L= xval&0x0000FFFF;
    regs->REG_EMAC_CFG_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RBQP(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RBQP_L)&0x0000FFFF)+((regs->REG_EMAC_RBQP_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write RBQP
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RBQP(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBQP_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBQP_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Address register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TAR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_TAR_L= xval&0x0000FFFF;
    regs->REG_EMAC_TAR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TCR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_TCR_L= xval&0x0000FFFF;
    regs->REG_EMAC_TCR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RSR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RSR_L)&0x0000FFFF)+((regs->REG_EMAC_RSR_H)<<0x10);
	return xval;
}

void MHal_EMAC_Write_RSR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RSR_L= xval&0x0000FFFF;
    regs->REG_EMAC_RSR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_TSR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_TSR_L)&0x0000FFFF)+((regs->REG_EMAC_TSR_H)<<0x10);
	return xval;
}

void MHal_EMAC_Write_TSR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_TSR_L= xval&0x0000FFFF;
    regs->REG_EMAC_TSR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt status register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ISR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ISR_L)&0x0000FFFF)+((regs->REG_EMAC_ISR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt enable register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IER(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_IER_L)&0x0000FFFF)+((regs->REG_EMAC_IER_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt enable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IER(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_IER_L= xval&0x0000FFFF;
    regs->REG_EMAC_IER_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt disable register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IDR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_IDR_L)&0x0000FFFF)+((regs->REG_EMAC_IDR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt disable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IDR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_IDR_L= xval&0x0000FFFF;
    regs->REG_EMAC_IDR_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt mask register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_IMR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_IMR_L)&0x0000FFFF)+((regs->REG_EMAC_IMR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read PHY maintenance register
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_MAN(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_MAN_L)&0x0000FFFF)+((regs->REG_EMAC_MAN_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write PHY maintenance register
//-------------------------------------------------------------------------------------------------
extern unsigned char phy_id;
void MHal_EMAC_Write_MAN(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

//	xval = (xval & 0xF07FFFFF) | (phy_id << 23); // <-@@@ specify PHY ID

	regs->REG_EMAC_MAN_L= xval&0x0000FFFF;
    regs->REG_EMAC_MAN_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_BUFF(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBCFG_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBCFG_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_BUFF(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RBCFG_L)&0x0000FFFF)+((regs->REG_EMAC_RBCFG_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Read Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RDPTR(void)
{
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RBRP_L)&0x0000FFFF)+((regs->REG_EMAC_RBRP_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RDPTR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBRP_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBRP_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_WRPTR(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_RBWP_L= xval&0x0000FFFF;
    regs->REG_EMAC_RBWP_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Frames transmitted OK
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_FRA(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_FRA);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_FRA_L)&0x0000FFFF)+((regs->REG_EMAC_FRA_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Single collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SCOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SCOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SCOL_L)&0x0000FFFF)+((regs->REG_EMAC_SCOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Multiple collision frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_MCOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_MCOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_MCOL_L)&0x0000FFFF)+((regs->REG_EMAC_MCOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Frames received OK
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_OK(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_OK);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_OK_L)&0x0000FFFF)+((regs->REG_EMAC_OK_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Frame check sequence errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SEQE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SEQE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SEQE_L)&0x0000FFFF)+((regs->REG_EMAC_SEQE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Alignment errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ALE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ALE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ALE_L)&0x0000FFFF)+((regs->REG_EMAC_ALE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Late collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_LCOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_LCOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_LCOL_L)&0x0000FFFF)+((regs->REG_EMAC_LCOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Excessive collisions
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ECOL(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ECOL);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ECOL_L)&0x0000FFFF)+((regs->REG_EMAC_ECOL_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Transmit under-run errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_TUE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_TUE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_TUE_L)&0x0000FFFF)+((regs->REG_EMAC_TUE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Carrier sense errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_CSE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_CSE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_CSE_L)&0x0000FFFF)+((regs->REG_EMAC_CSE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Receive resource error
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_RE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RE_L)&0x0000FFFF)+((regs->REG_EMAC_RE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Received overrun
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ROVR(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ROVR);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ROVR_L)&0x0000FFFF)+((regs->REG_EMAC_ROVR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Received symbols error
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SE_L)&0x0000FFFF)+((regs->REG_EMAC_SE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Excessive length errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_ELR(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_ELR);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_ELR_L)&0x0000FFFF)+((regs->REG_EMAC_ELR_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Receive jabbers
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_RJB(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_RJB);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_RJB_L)&0x0000FFFF)+((regs->REG_EMAC_RJB_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Undersize frames
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_USF(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_USF);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_USF_L)&0x0000FFFF)+((regs->REG_EMAC_USF_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  SQE test errors
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_SQEE(void)
{
    //return MHal_EMAC_ReadReg32(REG_ETH_SQEE);
    u32 xval;
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    xval = ((regs->REG_EMAC_SQEE_L)&0x0000FFFF)+((regs->REG_EMAC_SQEE_H)<<0x10);
	return xval;
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 100
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0100(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	//regs->REG_JULIAN_0100= xval&0x0000FFFF;
	regs->REG_JULIAN_0100_L= xval&0x0000FFFF;
    regs->REG_JULIAN_0100_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 104
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0104(void)
{
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	return ((regs->REG_JULIAN_0104_L)&0x0000FFFF)+((regs->REG_JULIAN_0104_H)<<0x10);
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 104
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0104(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_JULIAN_0104_L= xval&0x0000FFFF;
    regs->REG_JULIAN_0104_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 108
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_Read_JULIAN_0108(void)
{
    TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	return ((regs->REG_JULIAN_0108_L)&0x0000FFFF)+((regs->REG_JULIAN_0108_H)<<0x10);
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 108
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0108(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_JULIAN_0108_L= xval&0x0000FFFF;
    regs->REG_JULIAN_0108_H= xval>>16;
}

//-------------------------------------------------------------------------------------------------
//  PHY INTERFACE
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Enable the MDIO bit in MAC control register
// When not called from an interrupt-handler, access to the PHY must be
// protected by a spinlock.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_mdi(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   regs->REG_EMAC_CTL_L |= EMAC_MPE; //enable management port //
   regs->REG_EMAC_CTL_H &= EMAC_ALLFF;
}

//-------------------------------------------------------------------------------------------------
//  Disable the MDIO bit in the MAC control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_mdi(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   regs->REG_EMAC_CTL_L &= ~EMAC_MPE;    // disable management port //
   regs->REG_EMAC_CTL_H &= EMAC_ALLFF;
}

//-------------------------------------------------------------------------------------------------
// Write value to the a PHY register
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_write_phy (unsigned char phy_addr, unsigned char address, u32 value)
{
   u32 uRegVal, uCTL = 0;
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

   uRegVal =(EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_W)
            | ((phy_addr & 0x1f) << PHY_ADDR_SHIFT) | (address << PHY_REG_SHIFT) | (value & 0xFFFF) ;

    uCTL = MHal_EMAC_Read_CTL();
    MHal_EMAC_enable_mdi();
    MHal_EMAC_Write_MAN(uRegVal);

   // Wait until IDLE bit in Network Status register is cleared //
   uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);

   while (!(uRegVal& EMAC_IDLE))
   {
       uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);
       barrier ();
   }

    MHal_EMAC_Write_CTL(uCTL);
}
//-------------------------------------------------------------------------------------------------
// Read value stored in a PHY register.
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_read_phy(unsigned char phy_addr, unsigned char address,u32 *value)
{
   u32 uRegVal, uCTL = 0;
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

   uRegVal = (EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_R)
           | ((phy_addr & 0x1f) << PHY_ADDR_SHIFT) | (address << PHY_REG_SHIFT) | (0) ;

   uCTL = MHal_EMAC_Read_CTL();
   MHal_EMAC_enable_mdi();
   MHal_EMAC_Write_MAN(uRegVal);

   //Wait until IDLE bit in Network Status register is cleared //
   uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);

   while (!(uRegVal & EMAC_IDLE))
   {
       uRegVal = ((regs->REG_EMAC_SR_L)&0x0000FFFF)+((regs->REG_EMAC_SR_H)<<0x10);
       barrier ();
   }

    *value = (MHal_EMAC_Read_MAN() & 0x0000ffff);
    MHal_EMAC_Write_CTL(uCTL);
}

//-------------------------------------------------------------------------------------------------
// Update MAC speed and H/F duplex
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_update_speed_duplex(u32 uspeed, u32 uduplex)
{
   u32 mac_cfg_L, mac_cfg_H;
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;

   mac_cfg_L = regs->REG_EMAC_CFG_L & ~(EMAC_SPD | EMAC_FD);
   mac_cfg_H = regs->REG_EMAC_CFG_H & ~(EMAC_SPD | EMAC_FD);

   if (uspeed == SPEED_100)
   {
       if (uduplex == DUPLEX_FULL)    // 100 Full Duplex //
       {
           regs->REG_EMAC_CFG_L = mac_cfg_L | EMAC_SPD | EMAC_FD;
       }
       else                           // 100 Half Duplex ///
       {
		   regs->REG_EMAC_CFG_L = mac_cfg_L | EMAC_SPD;
       }
   }
   else
   {
       if (uduplex == DUPLEX_FULL)    //10 Full Duplex //
       {
           regs->REG_EMAC_CFG_L = mac_cfg_L |EMAC_FD;
       }
       else                           // 10 Half Duplex //
       {
           regs->REG_EMAC_CFG_L = mac_cfg_L;
       }
   }
   regs->REG_EMAC_CFG_H &= EMAC_ALLFF;//Write to CFG
}

//-------------------------------------------------------------------------------------------------
//Initialize and enable the PHY interrupt when link-state changes
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_phyirq (void)
{

}

//-------------------------------------------------------------------------------------------------
// Disable the PHY interrupt
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_phyirq (void)
{

}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

u32 MHal_EMAC_get_SA1H_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA1H_L&0x0000FFFF) + (regs->REG_EMAC_SA1H_H<<16);
}

u32 MHal_EMAC_get_SA1L_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA1L_L&0x0000FFFF) + (regs->REG_EMAC_SA1L_H<<16);
}

u32 MHal_EMAC_get_SA2H_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA2H_L&0x0000FFFF) + (regs->REG_EMAC_SA2H_H<<16);
}

u32 MHal_EMAC_get_SA2L_addr(void)
{
   TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
   return (regs->REG_EMAC_SA2L_L&0x0000FFFF) + (regs->REG_EMAC_SA2L_H<<16);
}

void MHal_EMAC_Write_SA1H(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA1H_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA1H_H= xval>>16;
}

void MHal_EMAC_Write_SA1L(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA1L_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA1L_H= xval>>16;
}

void MHal_EMAC_Write_SA2H(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA2H_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA2H_H= xval>>16;
}

void MHal_EMAC_Write_SA2L(u32 xval)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
	regs->REG_EMAC_SA2L_L= xval&0x0000FFFF;
    regs->REG_EMAC_SA2L_H= xval>>16;
}

void * MDev_memset(void * s,int c,unsigned long count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

//-------------------------------------------------------------------------------------------------
// Check INT Done
//-------------------------------------------------------------------------------------------------
u32 MHal_EMAC_CheckINTDone(void)
{
   u32 retIntStatus;
   retIntStatus = MHal_EMAC_Read_ISR();
   MHalThisUVE.cntChkINTCounter = (MHalThisUVE.cntChkINTCounter%MHal_MAX_INT_COUNTER);
   MHalThisUVE.cntChkINTCounter ++;
   if((retIntStatus&EMAC_INT_DONE)||(MHalThisUVE.cntChkINTCounter==(MHal_MAX_INT_COUNTER-1)))
   {
      MHalThisUVE.flagISR_INT_DONE = 0x01;
	  return TRUE;
   }
   return FALSE;
}

void MHal_EMAC_WritCAM_Address(u32 addr,u8 m0,u8 m1,u8 m2,u8 m3,u8 m4,u8 m5)
{
	u32 w0 = (u32)m1 << 24 | m0 << 16;
	u32 w1 = (u32)m5 << 24 | m4 << 16 | m3 << 8 | m2;
	MHal_EMAC_WritReg32(addr    ,w0);
    MHal_EMAC_WritReg32(addr + 4,w1);
}

void MHal_EMAC_ReadCAM_Address(u32 addr,u32 *w0,u32 *w1)
{
	*w0 = MHal_EMAC_ReadReg32(addr);
	*w1 = MHal_EMAC_ReadReg32(addr + 4);
}

extern unsigned char phy_id;
//-------------------------------------------------------------------------------------------------
// MAC cable connection detection
//-------------------------------------------------------------------------------------------------
s32 MHal_EMAC_CableConnection(void)
{
    u32 value, retValue, partner, counter;

    //check status

    counter = 0;
    MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    while(!(value & PHY_AN_DONE))
    {
        //wait 4 secs
        counter++;
        if(counter > 20)
        {
            printf("AN fail");
            return 0;
        }

        mdelay(200);
        MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    }

    counter = 0;
    MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    while(!(value & PHY_LINK_UP))
    {
        //wait 4 secs
        counter++;
        if(counter > 20)
        {
            printf("Link up fail");
            return 0;
        }

        mdelay(200);
        MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &value);
    }

    MHal_EMAC_read_phy(phy_id, PHY_REG_LINK_PARTNER, &partner);
    value = (value >> 11) & 0x0000000F;
    partner = (partner>> 5) & 0x0000000F;
    value &= (partner);

    //100MB Full
    if (value & 0x08)
    {
        MHalThisBCE.duplex = 1;
        MHalThisBCE.speed = SPEED_100;
        retValue = 4;
    }
    //100MB Half
    else if (value & 0x04)
    {
        MHalThisBCE.speed = SPEED_100;
        retValue = 3;
    }
    //10MB Full
    else if (value & 0x02)
    {
        MHalThisBCE.duplex = 1;
        retValue = 2;
    }
    else
        retValue = 1;

    return(retValue);
}

//-------------------------------------------------------------------------------------------------
// EMAC Negotiation PHY
//-------------------------------------------------------------------------------------------------
s32 MHal_EMAC_NegotiationPHY(unsigned char phyid)
{
    s32 retValue;


    // Set default as 10Mb half-duplex if negotiation fails.
    MHalThisBCE.duplex = 0;
    MHalThisBCE.speed = SPEED_10;
    //Auto-Negotiation
    printf("Auto-Negotiation...\n");
    MHal_EMAC_write_phy(phyid, PHY_REG_BASIC, 0x1000);
    retValue = MHal_EMAC_CableConnection();

    printf("Link Status Speed:%u Full-duplex:%u\n", MHalThisBCE.speed, MHalThisBCE.duplex);
    return(retValue);
}

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_timer_callback(unsigned long value)
{
	TITANIA_EMAC regs = (TITANIA_EMAC) REG_ADDR_BASE;
    regs->REG_EMAC_IER_L |= (EMAC_INT_RCOM );
    regs->REG_EMAC_IER_H &= EMAC_ALLFF;
}

//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_WriteProtect(u8 bEnable, u32 u32AddrUB, u32 u32AddrLB)
{
    u32 val = MHal_EMAC_ReadReg32(0x100), addr = 0;;

    val |= (bEnable) ? EMAC_MIU_WP_EN : 0;
    MHal_EMAC_WritReg32(0x100, val);

    //0x11E
    addr = u32AddrUB / MIU_ADDR_UNIT;
    printf("Upper addr:%lx\n", addr);
    MHal_EMAC_WritReg32(0x11E, (addr & 0x0000FFFF));
    MHal_EMAC_WritReg32(0x120, (addr & 0x00FF0000) >> 16);
    //0x122
    addr = u32AddrLB / MIU_ADDR_UNIT;
    printf("Lower addr:%lx\n", addr);
    MHal_EMAC_WritReg32(0x122, (u32AddrLB/MIU_ADDR_UNIT) & 0x0000FFFF);
    MHal_EMAC_WritReg32(0x124, ((u32AddrLB/MIU_ADDR_UNIT) & 0x00FF0000) >> 16);
}

#if (SUPPORT_ALBANY)
void MHal_EMAC_Albany_I2C(u8 enable)
{
    //I2C [6:0]
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x58*2) ) &= 0x80;
    if (enable)
        *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x58*2) ) |= 0x47;
    else
        *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x58*2) ) |= 0x17;
}

void MHal_EMAC_Power_On_Albany(void)
{
    printf("Albany's PHY setting\n");
    //Chip config [6][4]= 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x40*2+1) ) &= 0xAF;
    //Reset output mode, bit[3] = 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x98*2) ) &= 0xF7;
    //Reset -> High, bit[3] = 1
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x96*2) ) |= 0x08;

    //clk enable
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0xF0*2) ) &= 0xFC;
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0xF0*2) ) |= 0x2;
    //BIT[2] = 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x38*2) ) &= 0xFB;

    //Chip config [6:0]= 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x5A*2) ) &= 0x80;
    //I2C enable [6:0]
    MHal_EMAC_Albany_I2C(TRUE);
    //Reset -> low, bit[3] = 0
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x96*2) ) &= 0xF7;
    udelay(10000);
    //Reset -> High, bit[3] = 1
    *( (volatile u8 * ) ( ( char * ) 0xA0203C00 +  0x96*2) ) |= 0x08;
    udelay(5000);
}
#endif
//-------------------------------------------------------------------------------------------------
// EMAC clock on/off
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_ClkGen(void)
{
#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS)
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x60*4) ) = 0x0400;
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x61*4) ) = 0x0004;
#elif (CONFIG_KAISERIN)
    *( ( u32 * ) ( ( char * ) 0xBF206600 +  0x32*4) ) = 0x0400;
    *( ( u32 * ) ( ( char * ) 0xBF206600 +  0x33*4) ) = 0x0004;
    *( ( u32 * ) ( ( char * ) 0xBF206600 +  0x34*4) ) = 0x0000;
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x60*4) ) = 0x0000;
    *( ( u32 * ) ( ( char * ) 0xBF201600 +  0x61*4) ) = 0x0000;
#elif (CONFIG_JANUS) || (CONFIG_MARIA10)
#if (SUPPORT_ALBANY)
    //Go MII Mode
    *((u32*)(u32)((char*)0xA0201780))=0x00000000;
    *((u32*)(u32)((char*)0xA0201784))=0x00000000;
#else
    //Go RMII Mode
    *((u32*)(u32)((char*)0xA0201780))=0x0400;
    *((u32*)(u32)((char*)0xA0201784))=0x0004;
#endif
#else
   // u32 emac_clk;
    *((u32*)((char*)0xbf8069c4)) = 0x20;
#endif
}

void MHal_EMAC_PadMux(void)
{
#if (CONFIG_JANUS) || (CONFIG_MARIA10)
    #if (SUPPORT_ALBANY)
        *((u32*)(u32)((char*)0xA0203c80))=0x0001;  //[1:0] reg_et_mode=1
        MHal_EMAC_Power_On_Albany();
    #else
        *((u32*)(u32)((char*)0xA0203c80))=0x0003;  //[1:0] reg_et_mode=3
    #endif
    *((u32*)(u32)((char*)0xA0203d40))=0x0000;  //[15] reg_allpad_in=0
#elif (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1)
    //chiptop [15] allpad_in
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x50*4 ) ) &= 0x7FFF;
    //chiptop [9:8] et_mode
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x6F*4 ) ) &= (~0x0100); //et_mode = 1
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x6F*4 ) ) |= 0x0100;    //et_mode = 1
#elif (CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    //chiptop [15] allpad_in
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x50*4 ) ) &= 0x7FFF;
    //chiptop [10:8] et_mode
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x7*4 ) ) &= (~0x0700);  //et_mode = 2
    *( ( u32 * ) ( ( char * ) 0xBF203C00 + 0x7*4 ) ) |= 0x0200;     //et_mode = 2
#else
    //power on init
    *((u32*)((char*)0xbf803d3c)) = 0x0440;
    *((u32*)((char*)0xbf803d40)) &= 0x7FFF;
    *((u32*)((char*)0xbf803dBc)) &= 0xF73F;
#endif
}

void MHal_EPHY_ClkGen(void)
{
#if (CONFIG_KAISERIN)
// Albany
#define REG_8(reg) (*(volatile u8*)( 0xBF000000 + reg * 2 - reg % 2))
REG_8(0x32FC) = 0x00;//wriu    0x0032fc    0x00    // Power-on LDO
REG_8(0x32B7) = 0x07;//wriu    0x0032b7    0x07    // Power-on ADC
REG_8(0x32CB) = 0x11;//wriu    0x0032cb    0x11    // Power-on BGAP
REG_8(0x32CC) = 0x80;//wriu    0x0032cc    0x80    // Power-on ADCPL
REG_8(0x32CD) = 0xD1;//wriu    0x0032cd    0xd1    // Power-on ADCPL
REG_8(0x32D4) = 0x00;//wriu    0x0032d4    0x00    // Power-on LPF_OP
REG_8(0x32B9) = 0x40;//wriu    0x0032b9    0x40    // Power-on LPF
REG_8(0x32BB) = 0x05;//wriu    0x0032bb    0x05    // Power-on REF
REG_8(0x32B0) = 0x02;//wriu -w 0x0032b0    0x6002  // Power-on TX
REG_8(0x32B1) = 0x60;

// DAC settings for buggy link partner
// ltp
REG_8(0x00311e)=0x00; //wriu 0x00311e 0x00
REG_8(0x00311f)=0x98; //wriu 0x00311f 0x98
REG_8(0x003120)=0x84; //wriu 0x003120 0x84
REG_8(0x003121)=0x84; //wriu 0x003121 0x84
REG_8(0x003122)=0x88; //wriu 0x003122 0x88
REG_8(0x003123)=0x88; //wriu 0x003123 0x88
REG_8(0x003124)=0x8c; //wriu 0x003124 0x8c
REG_8(0x003125)=0x8c; //wriu 0x003125 0x8c
REG_8(0x003126)=0x98; //wriu 0x003126 0x98
REG_8(0x003127)=0xc8; //wriu 0x003127 0xc8
REG_8(0x003128)=0x00; //wriu 0x003128 0x00
REG_8(0x003129)=0x00; //wriu 0x003129 0x00
REG_8(0x00312a)=0x00; //wriu 0x00312a 0x00
REG_8(0x00312b)=0x10; //wriu 0x00312b 0x10
REG_8(0x00312c)=0x00; //wriu 0x00312c 0x00
// tin
REG_8(0x003188)=0x3c; //wriu 0x003188 0x3c
REG_8(0x003189)=0x3c; //wriu 0x003189 0x3c
REG_8(0x00318a)=0x3c; //wriu 0x00318a 0x3c
REG_8(0x00318b)=0x3c; //wriu 0x00318b 0x3c
REG_8(0x00318c)=0x30; //wriu 0x00318c 0x30
REG_8(0x00318d)=0x3c; //wriu 0x00318d 0x3c
REG_8(0x00318e)=0x7c; //wriu 0x00318e 0x7c
REG_8(0x00318f)=0x68; //wriu 0x00318f 0x68
REG_8(0x003190)=0x34; //wriu 0x003190 0x34
REG_8(0x003191)=0x78; //wriu 0x003191 0x78
REG_8(0x003192)=0x94; //wriu 0x003192 0x94
REG_8(0x003193)=0xd4; //wriu 0x003193 0xd4
REG_8(0x003194)=0xa0; //wriu 0x003194 0xa0
REG_8(0x003195)=0x84; //wriu 0x003195 0x84
REG_8(0x003196)=0xc8; //wriu 0x003196 0xc8
REG_8(0x003197)=0xe4; //wriu 0x003197 0xe4
REG_8(0x003198)=0xc8; //wriu 0x003198 0xc8
REG_8(0x003199)=0xc8; //wriu 0x003199 0xc8
REG_8(0x00319a)=0xe8; //wriu 0x00319a 0xe8
REG_8(0x00319b)=0xc8; //wriu 0x00319b 0xc8
REG_8(0x00319c)=0x3c; //wriu 0x00319c 0x3c
REG_8(0x00319d)=0x3c; //wriu 0x00319d 0x3c
REG_8(0x00319e)=0x3c; //wriu 0x00319e 0x3c
REG_8(0x00319f)=0x3c; //wriu 0x00319f 0x3c
REG_8(0x0031a0)=0x30; //wriu 0x0031a0 0x30
REG_8(0x0031a1)=0x24; //wriu 0x0031a1 0x24
REG_8(0x0031a2)=0x7c; //wriu 0x0031a2 0x7c
REG_8(0x0031a3)=0x70; //wriu 0x0031a3 0x70
REG_8(0x0031a4)=0x20; //wriu 0x0031a4 0x20
REG_8(0x0031a5)=0x64; //wriu 0x0031a5 0x64
REG_8(0x0031a6)=0xa0; //wriu 0x0031a6 0xa0
REG_8(0x0031a7)=0xd4; //wriu 0x0031a7 0xd4
REG_8(0x0031a8)=0x98; //wriu 0x0031a8 0x98
REG_8(0x0031a9)=0x84; //wriu 0x0031a9 0x84
REG_8(0x0031aa)=0xc8; //wriu 0x0031aa 0xc8
REG_8(0x0031ab)=0xd0; //wriu 0x0031ab 0xd0
REG_8(0x0031ac)=0xc8; //wriu 0x0031ac 0xc8
REG_8(0x0031ad)=0xc8; //wriu 0x0031ad 0xc8
REG_8(0x0031ae)=0xc8; //wriu 0x0031ae 0xc8
REG_8(0x0031af)=0xc8; //wriu 0x0031af 0xc8
#endif
}

void MHal_EMAC_Power_On_Clk(void)
{
    MHal_EMAC_ClkGen();
    MHal_EMAC_PadMux();
    MHal_EPHY_ClkGen();
}

void MHal_EMAC_Power_Off_Clk(void)
{

}

