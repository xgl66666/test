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
#include "MsCommon.h"
#include "mhal_gmac.h"
#define EMAC_DBG(fmt, args...)              //{diag_printf("%04d %s " fmt, __LINE__, __FUNCTION__, ##args);}

//-------------------------------------------------------------------------------------------------
//  Defined Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
struct _MHalBasicConfigEMAC
{
    MS_U8 connected;          // 0:No, 1:Yes
    MS_U32 speed;               // 10:10Mbps, 100:100Mbps
    // ETH_CTL Register:
    MS_U8 wes;             // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    // ETH_CFG Register:
    MS_U8 duplex;              // 1:Half-duplex, 2:Full-duplex
    MS_U8 cam;                // 0:No CAM, 1:Yes
    MS_U8 rcv_bcast;       // 0:No, 1:Yes
    MS_U8 rlf;                // 0:No, 1:Yes receive long frame(1522)
    // MAC Address:
    MS_U8 sa1[6];              // Specific Addr 1 (MAC Address)
    MS_U8 sa2[6];              // Specific Addr 2
    MS_U8 sa3[6];              // Specific Addr 3
    MS_U8 sa4[6];              // Specific Addr 4
};
typedef struct _MHalBasicConfigEMAC MHalBasicConfigEMAC;

struct _MHalUtilityVarsEMAC
{
    MS_U32 cntChkCableConnect;
    MS_U32 cntChkINTCounter;
    MS_U32 readIdxRBQP;        // Reset = 0x00000000
    MS_U32 rxOneFrameAddr;     // Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")
    MS_U8 flagISR_INT_DONE;
};
typedef struct _MHalUtilityVarsEMAC MHalUtilityVarsEMAC;

MHalBasicConfigEMAC MHalThisBCE;
MHalUtilityVarsEMAC MHalThisUVE;

#define MHal_MAX_INT_COUNTER    100
//-------------------------------------------------------------------------------------------------
//  EMAC hardware for Titania
//-------------------------------------------------------------------------------------------------

/*8-bit RIU address*/
MS_U8 MHal_EMAC_ReadReg8( MS_U32 bank, MS_U32 reg )
{
    MS_U8 val;
    MS_U32 address = EMAC_RIU_REG_BASE + bank*0x100UL*2;
    address = address + (reg << 1) - (reg & 1);

    val = *( ( volatile MS_U8* ) address );
    return val;
}

void MHal_EMAC_WritReg8( MS_U32 bank, MS_U32 reg, MS_U8 val )
{
    MS_U32 address = EMAC_RIU_REG_BASE + bank*0x100UL*2;
    address = address + (reg << 1) - (reg & 1);

    *( ( volatile MS_U8* ) address ) = val;
}

MS_U32 MHal_EMAC_ReadReg32( MS_U32 xoffset )
{
    MS_U32 address = REG_ADDR_BASE + xoffset*2;

    MS_U32 xoffsetValueL = *( ( volatile MS_U32* ) address ) & 0x0000FFFF;
    MS_U32 xoffsetValueH = *( ( volatile MS_U32* ) ( address + 4) ) << 0x10;
//    EMAC_DBG("IN\n");

    return( xoffsetValueH | xoffsetValueL );
}

void MHal_EMAC_WritReg32( MS_U32 xoffset, MS_U32 xval )
{
    MS_U32 address = REG_ADDR_BASE + xoffset*2;
//    EMAC_DBG("IN\n");

    *( ( volatile MS_U32 * ) address ) = ( MS_U32 ) ( xval & 0x0000FFFF );
    *( ( volatile MS_U32 * ) ( address + 4 ) ) = ( MS_U32 ) ( xval >> 0x10 );
}

MS_U32 MHal_EMAC_ReadRam32( MS_U32 uRamAddr, MS_U32 xoffset)
{
//    EMAC_DBG("IN\n");

    return (*( MS_U32 * ) ( ( char * ) uRamAddr + xoffset ) );
}

void MHal_EMAC_WritRam32( MS_U32 uRamAddr, MS_U32 xoffset, MS_U32 xval )
{
//    EMAC_DBG("IN\n");

    *( ( MS_U32 * ) ( ( char * ) uRamAddr + xoffset ) ) = xval;
}

void MHal_EMAC_Write_SA1_MAC_Address( MS_U8 m0, MS_U8 m1, MS_U8 m2, MS_U8 m3, MS_U8 m4, MS_U8 m5 )
{
    MS_U32 w0 = ( MS_U32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    MS_U32 w1 = ( MS_U32 ) m5 << 8 | m4;
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA1L, w0 );
    MHal_EMAC_WritReg32( REG_ETH_SA1H, w1 );
}

void MHal_EMAC_Write_SA2_MAC_Address( MS_U8 m0, MS_U8 m1, MS_U8 m2, MS_U8 m3, MS_U8 m4, MS_U8 m5 )
{
    MS_U32 w0 = ( MS_U32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    MS_U32 w1 = ( MS_U32 ) m5 << 8 | m4;
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA2L, w0 );
    MHal_EMAC_WritReg32( REG_ETH_SA2H, w1 );
}

void MHal_EMAC_Write_SA3_MAC_Address( MS_U8 m0, MS_U8 m1, MS_U8 m2, MS_U8 m3, MS_U8 m4, MS_U8 m5 )
{
    MS_U32 w0 = ( MS_U32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    MS_U32 w1 = ( MS_U32 ) m5 << 8 | m4;
//    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA3L, w0 );
    MHal_EMAC_WritReg32( REG_ETH_SA3H, w1 );
}

void MHal_EMAC_Write_SA4_MAC_Address( MS_U8 m0, MS_U8 m1, MS_U8 m2, MS_U8 m3, MS_U8 m4, MS_U8 m5 )
{
    MS_U32 w0 = ( MS_U32 ) m3 << 24 | m2 << 16 | m1 << 8 | m0;
    MS_U32 w1 = ( MS_U32 ) m5 << 8 | m4;
//    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA4L, w0 );
    MHal_EMAC_WritReg32( REG_ETH_SA4H, w1 );
}

//-------------------------------------------------------------------------------------------------
//  R/W EMAC register for Titania
//-------------------------------------------------------------------------------------------------

void MHal_EMAC_update_HSH(MS_U32 mc0, MS_U32 mc1)
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_HSL, mc0 );
    MHal_EMAC_WritReg32( REG_ETH_HSH, mc1 );
}

//-------------------------------------------------------------------------------------------------
//  Read control register
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_CTL( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_CTL );
}

//-------------------------------------------------------------------------------------------------
//  Write Network control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CTL( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_CTL, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Network configuration register
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_CFG( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_CFG );
}

//-------------------------------------------------------------------------------------------------
//  Write Network configuration register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_CFG( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_CFG, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_RBQP( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_RBQP );
}

//-------------------------------------------------------------------------------------------------
//  Write RBQP
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RBQP( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_RBQP, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Address register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TAR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_TAR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read RBQP
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_TCR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_TCR);
}

//-------------------------------------------------------------------------------------------------
//  Write Transmit Control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TCR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_TCR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Transmit Status Register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_TSR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_TSR, xval );
}

MS_U32 MHal_EMAC_Read_TSR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_TSR );
}

void MHal_EMAC_Write_RSR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_RSR, xval );
}

MS_U32 MHal_EMAC_Read_RSR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_RSR );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt status register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_ISR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_ISR, xval );
}

MS_U32 MHal_EMAC_Read_ISR( void )
{
    EMAC_DBG("IN\n");
    return MHal_EMAC_ReadReg32( REG_ETH_ISR );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt enable register
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_IER( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_IER );
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt enable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IER( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_IER, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt disable register
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_IDR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_IDR );
}

//-------------------------------------------------------------------------------------------------
//  Write Interrupt disable register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_IDR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_IDR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Interrupt mask register
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_IMR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_IMR );
}

void MHal_EMAC_Write_IMR(MS_U32 xval)
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_WritReg32( REG_ETH_IMR, xval);
}
//-------------------------------------------------------------------------------------------------
//  Read PHY maintenance register
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_MAN( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_MAN );
}

//-------------------------------------------------------------------------------------------------
//  Write PHY maintenance register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_MAN( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_MAN, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_BUFF( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_BUFF, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Receive Buffer Configuration
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_BUFF( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_BUFF );
}

//-------------------------------------------------------------------------------------------------
//  Read Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_RDPTR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_BUFFRDPTR );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_RDPTR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_BUFFRDPTR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_WRPTR( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_BUFFWRPTR, xval );
}

//-------------------------------------------------------------------------------------------------
//  Write Receive First Full Pointer
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_WRPTR(void)
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_BUFFWRPTR );
}

//-------------------------------------------------------------------------------------------------
//  Frames transmitted OK
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_FRA( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_FRA );
}

//-------------------------------------------------------------------------------------------------
//  Single collision frames
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_SCOL( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SCOL );
}

//-------------------------------------------------------------------------------------------------
//  Multiple collision frames
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_MCOL( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_MCOL );
}

//-------------------------------------------------------------------------------------------------
//  Frames received OK
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_OK( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_OK );
}

//-------------------------------------------------------------------------------------------------
//  Frame check sequence errors
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_SEQE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SEQE );
}

//-------------------------------------------------------------------------------------------------
//  Alignment errors
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_ALE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_ALE );
}

//-------------------------------------------------------------------------------------------------
//  Late collisions
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_LCOL( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_LCOL );
}

//-------------------------------------------------------------------------------------------------
//  Excessive collisions
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_ECOL( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_ECOL );
}

//-------------------------------------------------------------------------------------------------
//  Transmit under-run errors
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_TUE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_TUE );
}

//-------------------------------------------------------------------------------------------------
//  Carrier sense errors
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_CSE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_CSE );
}

//-------------------------------------------------------------------------------------------------
//  Receive resource error
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_RE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_RE );
}

//-------------------------------------------------------------------------------------------------
//  Received overrun
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_ROVR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_ROVR );
}

//-------------------------------------------------------------------------------------------------
//  Received symbols error
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_SE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SE );
}

//-------------------------------------------------------------------------------------------------
//  Excessive length errors
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_ELR( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_ELR );
}

//-------------------------------------------------------------------------------------------------
//  Receive jabbers
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_RJB( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_RJB );
}

//-------------------------------------------------------------------------------------------------
//  Undersize frames
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_USF( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_USF );
}

//-------------------------------------------------------------------------------------------------
//  SQE test errors
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_SQEE( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SQEE );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 100
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_JULIAN_0100( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_EMAC_JULIAN_0100 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 100
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0100( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_EMAC_JULIAN_0100, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 104
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_JULIAN_0104( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_EMAC_JULIAN_0104 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 104
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0104( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_EMAC_JULIAN_0104, xval );
}

//-------------------------------------------------------------------------------------------------
//  Read Julian 108
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_Read_JULIAN_0108( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_EMAC_JULIAN_0108 );
}

//-------------------------------------------------------------------------------------------------
//  Write Julian 108
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Write_JULIAN_0108( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_EMAC_JULIAN_0108, xval );
}

void MHal_EMAC_Set_Tx_JULIAN_T(MS_U32 xval)
{
	MS_U32 value;
    EMAC_DBG("IN\n");

	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xff0fffff;
	value |= xval << 20;

	MHal_EMAC_WritReg32(0x134, value);
}

void MHal_EMAC_Set_TEST(MS_U32 xval)
{
	MS_U32 value = 0xffffffff;
	int i=0;
    EMAC_DBG("IN\n");

	for(i = 0x100; i< 0x160;i+=4){
		MHal_EMAC_WritReg32(i, value);
		}

}

MS_U32 MHal_EMAC_Get_Tx_FIFO_Threshold(void)
{
    EMAC_DBG("IN\n");

	return (MHal_EMAC_ReadReg32(0x134) & 0x00f00000) >> 20;
}

void MHal_EMAC_Set_Rx_FIFO_Enlarge(MS_U32 xval)
{
	MS_U32 value;
    EMAC_DBG("IN\n");

	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xfcffffff;
	value |= xval << 24;

	MHal_EMAC_WritReg32(0x134, value);
}

MS_U32 MHal_EMAC_Get_Rx_FIFO_Enlarge(void)
{
    EMAC_DBG("IN\n");

	return (MHal_EMAC_ReadReg32(0x134) & 0x03000000) >> 24;
}

void MHal_EMAC_Set_Miu_Priority(MS_U32 xval)
{
	MS_U32 value;
    EMAC_DBG("IN\n");

	value = MHal_EMAC_ReadReg32(0x100);
	value &= 0xfff7ffff;
	value |= xval << 19;

	MHal_EMAC_WritReg32(0x100, value);
}

MS_U32 MHal_EMAC_Get_Miu_Priority(void)
{
    EMAC_DBG("IN\n");

	return (MHal_EMAC_ReadReg32(0x100) & 0x00080000) >> 19;
}

void MHal_EMAC_Set_Tx_Hang_Fix_ECO(MS_U32 xval)
{
	MS_U32 value;
    EMAC_DBG("IN\n");

	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xfffbffff;
	value |= xval << 18;

	MHal_EMAC_WritReg32(0x134, value);
}

void MHal_EMAC_Set_MIU_Out_Of_Range_Fix(MS_U32 xval)
{
    EMAC_DBG("IN\n");

#if 1
	MS_U32 value;
	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xefffffff;
	value |= xval << 28;

	MHal_EMAC_WritReg32(0x134, value);
#endif
}

void MHal_EMAC_Set_Rx_Tx_Burst16_Mode(MS_U32 xval)
{
    EMAC_DBG("IN\n");

#if 1
	MS_U32 value;
	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xdfffffff;
	value |= xval << 29;

	MHal_EMAC_WritReg32(0x134, value);
#endif
}

void MHal_EMAC_Set_Tx_Rx_Req_Priority_Switch(MS_U32 xval)
{
	MS_U32 value;
    EMAC_DBG("IN\n");

	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xfff7ffff;
	value |= xval << 19;

	MHal_EMAC_WritReg32(0x134, value);
}

void MHal_EMAC_Set_Rx_Byte_Align_Offset(MS_U32 xval)
{
	MS_U32 value;
    EMAC_DBG("IN\n");

	value = MHal_EMAC_ReadReg32(0x134);
	value &= 0xf3ffffff;
	value |= xval << 26;

	MHal_EMAC_WritReg32(0x134, value);
}


void MHal_EMAC_Write_Protect(MS_U32 start_addr, MS_U32 length)
{
        MS_U32 value;
    EMAC_DBG("IN\n");

        value = MHal_EMAC_ReadReg32(0x11c);
	 value &= 0x0000ffff;
	 value |= ((start_addr+length) >> 3) << 16;
        MHal_EMAC_WritReg32(0x11c, value);

        value = MHal_EMAC_ReadReg32(0x120);
	 value &= 0x00000000;
	 value |= ((start_addr+length) >> 3) >> 16;
        value |= (start_addr >> 3) << 16;
        MHal_EMAC_WritReg32(0x120, value);

        value = MHal_EMAC_ReadReg32(0x124);
	 value &= 0xffff0000;
        value |= (start_addr >> 3) >> 16;
        MHal_EMAC_WritReg32(0x124, value);
}

void MHal_EMAC_HW_init(void)
{
    EMAC_DBG("IN\n");

    MHal_EMAC_Set_Tx_JULIAN_T(0x4);
    MHal_EMAC_Set_Rx_FIFO_Enlarge(0x2);

    MHal_EMAC_Set_Miu_Priority(0x1);

    MHal_EMAC_Set_Tx_Hang_Fix_ECO(1);
}

//-------------------------------------------------------------------------------------------------
//  PHY INTERFACE
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Enable the MDIO bit in MAC control register
// When not called from an interrupt-handler, access to the PHY must be
// protected by a spinlock.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_mdi( void )
{
    MS_U32 xval;
    EMAC_DBG("IN\n");

    xval = MHal_EMAC_Read_CTL();
    xval |= EMAC_MPE;
    MHal_EMAC_Write_CTL( xval );
}

//-------------------------------------------------------------------------------------------------
//  Disable the MDIO bit in the MAC control register
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_mdi( void )
{
    MS_U32 xval;

    EMAC_DBG("IN\n");

    xval = MHal_EMAC_Read_CTL();
    xval &= ~EMAC_MPE;
    MHal_EMAC_Write_CTL( xval );
}

//-------------------------------------------------------------------------------------------------
// Write value to the a PHY register
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_write_phy( unsigned char phy_addr, unsigned char address, MS_U32 value )
{
#ifdef CONFIG_ETHERNET_ALBANY
    MS_U32 uRegBase = INTERNEL_PHY_REG_BASE;

    phy_addr =0;

    *(volatile unsigned int *)(uRegBase + address*4) = value;
    MsOS_DelayTask(1);
#else
    MS_U32 uRegVal = 0, uCTL = 0;
    EMAC_DBG("IN\n");

    uRegVal =  ( EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_W) | (( phy_addr & 0x1F ) << PHY_ADDR_OFFSET )
                | ( address << PHY_REGADDR_OFFSET ) | (value & 0xFFFF);

    uCTL = MHal_EMAC_Read_CTL();
    MHal_EMAC_enable_mdi();

    MHal_EMAC_Write_MAN( uRegVal );
    // Wait until IDLE bit in Network Status register is cleared //
    uRegVal = MHal_EMAC_ReadReg32( REG_ETH_SR );  //Must read Low 16 bit.
    while ( !( uRegVal & EMAC_IDLE ) )
    {
        uRegVal = MHal_EMAC_ReadReg32( REG_ETH_SR );
        barrier();
    }
    MHal_EMAC_Write_CTL(uCTL);
#endif
}
//-------------------------------------------------------------------------------------------------
// Read value stored in a PHY register.
// Note: MDI interface is assumed to already have been enabled.
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_read_phy( unsigned char phy_addr, unsigned char address, MS_U32* value )
{
#ifdef CONFIG_ETHERNET_ALBANY
    MS_U32 uRegBase  = INTERNEL_PHY_REG_BASE;
    MS_U32 tempvalue ;

    phy_addr =0;

    tempvalue = *(volatile unsigned int *)(INTERNEL_PHY_REG_BASE + 0x04UL);
    tempvalue |= 0x0004UL;
    *(volatile unsigned int *)(INTERNEL_PHY_REG_BASE + 0x04UL) = tempvalue;
    MsOS_DelayTask(1);
    *value = *(volatile unsigned int *)(uRegBase + address*4);
#else
    MS_U32 uRegVal = 0, uCTL = 0;
    EMAC_DBG("IN\n");

    uRegVal = (EMAC_HIGH | EMAC_CODE_802_3 | EMAC_RW_R)
            | ((phy_addr & 0x1f) << PHY_ADDR_OFFSET) | (address << PHY_REGADDR_OFFSET) | (0) ;

    uCTL = MHal_EMAC_Read_CTL();
    MHal_EMAC_enable_mdi();
    MHal_EMAC_Write_MAN( uRegVal );

    //Wait until IDLE bit in Network Status register is cleared //
    uRegVal = MHal_EMAC_ReadReg32( REG_ETH_SR );  //Must read Low 16 bit.
    while ( !( uRegVal & EMAC_IDLE ) )
    {
        uRegVal = MHal_EMAC_ReadReg32( REG_ETH_SR );
        barrier();
    }
    *value = ( MHal_EMAC_Read_MAN() & 0x0000ffff );
    MHal_EMAC_Write_CTL(uCTL);
#endif
}

//-------------------------------------------------------------------------------------------------
// Update MAC speed and H/F duplex
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_update_speed_duplex( MS_U32 uspeed, MS_U32 uduplex )
{
    MS_U32 xval;
    EMAC_DBG("IN\n");

    xval = MHal_EMAC_ReadReg32( REG_ETH_CFG ) & ~( EMAC_SPD | EMAC_FD );

    if ( uspeed == SPEED_100 )
    {
        if ( uduplex == DUPLEX_FULL )    // 100 Full Duplex //
        {
            xval = xval | EMAC_SPD | EMAC_FD;
        }
        else                           // 100 Half Duplex ///
        {
            xval = xval | EMAC_SPD;
        }
    }
    else
    {
        if ( uduplex == DUPLEX_FULL )    //10 Full Duplex //
        {
            xval = xval | EMAC_FD;
        }
        else                           // 10 Half Duplex //
        {
        }
    }
    MHal_EMAC_WritReg32( REG_ETH_CFG, xval );
}

MS_U8 MHal_EMAC_CalcMACHash( MS_U8 m0, MS_U8 m1, MS_U8 m2, MS_U8 m3, MS_U8 m4, MS_U8 m5 )
{

    MS_U8 hashIdx0 = ( m0&0x01 ) ^ ( ( m0&0x40 ) >> 6 ) ^ ( ( m1&0x10 ) >> 4 ) ^ ( ( m2&0x04 ) >> 2 )
                 ^ ( m3&0x01 ) ^ ( ( m3&0x40 ) >> 6 ) ^ ( ( m4&0x10 ) >> 4 ) ^ ( ( m5&0x04 ) >> 2 );

     MS_U8 hashIdx1 = ( m0&0x02 ) ^ ( ( m0&0x80 ) >> 6 ) ^ ( ( m1&0x20 ) >> 4 ) ^ ( ( m2&0x08 ) >> 2 )
                ^ ( m3&0x02 ) ^ ( ( m3&0x80 ) >> 6 ) ^ ( ( m4&0x20 ) >> 4 ) ^ ( ( m5&0x08 ) >> 2 );

    MS_U8 hashIdx2 = ( m0&0x04 ) ^ ( ( m1&0x01 ) << 2 ) ^ ( ( m1&0x40 ) >> 4 ) ^ ( ( m2&0x10 ) >> 2 )
                ^ ( m3&0x04 ) ^ ( ( m4&0x01 ) << 2 ) ^ ( ( m4&0x40 ) >> 4 ) ^ ( ( m5&0x10 ) >> 2 );

    MS_U8 hashIdx3 = ( m0&0x08 ) ^ ( ( m1&0x02 ) << 2 ) ^ ( ( m1&0x80 ) >> 4 ) ^ ( ( m2&0x20 ) >> 2 )
                ^ ( m3&0x08 ) ^ ( ( m4&0x02 ) << 2 ) ^ ( ( m4&0x80 ) >> 4 ) ^ ( ( m5&0x20 ) >> 2 );

    MS_U8 hashIdx4 = ( m0&0x10 ) ^ ( ( m1&0x04 ) << 2 ) ^ ( ( m2&0x01 ) << 4 ) ^ ( ( m2&0x40 ) >> 2 )
                ^ ( m3&0x10 ) ^ ( ( m4&0x04 ) << 2 ) ^ ( ( m5&0x01 ) << 4 ) ^ ( ( m5&0x40 ) >> 2 );

    MS_U8 hashIdx5 = ( m0&0x20 ) ^ ( ( m1&0x08 ) << 2 ) ^ ( ( m2&0x02 ) << 4 ) ^ ( ( m2&0x80 ) >> 2 )
                ^ ( m3&0x20 ) ^ ( ( m4&0x08 ) << 2 ) ^ ( ( m5&0x02 ) << 4 ) ^ ( ( m5&0x80 ) >> 2 );
    EMAC_DBG("IN\n");

    return( hashIdx0 | hashIdx1 | hashIdx2 | hashIdx3 | hashIdx4 | hashIdx5 );
}

//-------------------------------------------------------------------------------------------------
//Initialize and enable the PHY interrupt when link-state changes
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_enable_phyirq( void )
{
    EMAC_DBG("IN\n");

#if 0

#endif
}

//-------------------------------------------------------------------------------------------------
// Disable the PHY interrupt
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_disable_phyirq( void )
{
    EMAC_DBG("IN\n");

#if 0

#endif
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

MS_U32 MHal_EMAC_get_SA1H_addr( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SA1H );
}

MS_U32 MHal_EMAC_get_SA1L_addr( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SA1L );
}

MS_U32 MHal_EMAC_get_SA2H_addr( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SA2H );
}

MS_U32 MHal_EMAC_get_SA2L_addr( void )
{
    EMAC_DBG("IN\n");

    return MHal_EMAC_ReadReg32( REG_ETH_SA2L );
}

void MHal_EMAC_Write_SA1H( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA1H, xval );
}

void MHal_EMAC_Write_SA1L( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA1L, xval );
}

void MHal_EMAC_Write_SA2H( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA2H, xval );
}

void MHal_EMAC_Write_SA2L( MS_U32 xval )
{
    EMAC_DBG("IN\n");

    MHal_EMAC_WritReg32( REG_ETH_SA2L, xval );
}

void* MDev_memset( void* s, MS_U32 c, unsigned long count )
{
    char* xs = ( char* ) s;
    EMAC_DBG("IN\n");

    while ( count-- )
        *xs++ = c;

    return s;
}

//-------------------------------------------------------------------------------------------------
// Check INT Done
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_CheckINTDone( void )
{

    MS_U32 retIntStatus;
    EMAC_DBG("IN\n");

    retIntStatus = MHal_EMAC_Read_ISR();
    MHalThisUVE.cntChkINTCounter = ( MHalThisUVE.cntChkINTCounter %
                                     MHal_MAX_INT_COUNTER );
    MHalThisUVE.cntChkINTCounter ++;
    if ( ( retIntStatus & EMAC_INT_DONE ) ||
         ( MHalThisUVE.cntChkINTCounter == ( MHal_MAX_INT_COUNTER - 1 ) ) )
    {
        MHalThisUVE.flagISR_INT_DONE = 0x01;
        return TRUE;
    }
    return FALSE;
}

extern unsigned char phyaddr;
//-------------------------------------------------------------------------------------------------
// MAC cable connection detection
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_CableConnection( void )
{
    MS_U32 value, retValue, counter;
    MS_U8 uRegVal;
    MS_U32 speed, duplex;

    //check status

    counter = 0;
    MHal_EMAC_read_phy(phyaddr, PHY_REG_STATUS, &value);
    while(!(value & PHY_AN_DONE))
    {
        //wait 4 secs
        counter++;
        if(counter > 20)
        {
            printf("AN fail");
            return 0;
        }

        MsOS_DelayTask(200);
        MHal_EMAC_read_phy(phyaddr, PHY_REG_STATUS, &value);
    }

    counter = 0;
    MHal_EMAC_read_phy(phyaddr, PHY_REG_STATUS, &value);
    while(!(value & PHY_LINK_UP))
    {
        //wait 4 secs
        counter++;
        if(counter > 20)
        {
            printf("Link up fail");
            return 0;
        }

        MsOS_DelayTask(200);
        MHal_EMAC_read_phy(phyaddr, PHY_REG_STATUS, &value);
    }

#ifndef CONFIG_ETHERNET_ALBANY
    MHal_EMAC_read_phy(phyaddr, PHY_REG_PHYSR, &value);
    speed = value & PHYSR_SPEED_MASK;
    duplex = value & PHYSR_DUPLEX_MASK;
#else
    MHal_EMAC_read_phy(phyaddr, PHY_REG_BASIC, &value);
    if ((value & 0x2000) == 0x2000)
        speed = PHYSR_100;
    else
        speed = PHYSR_10;

    if ((value & 0x0100) == 0x0100)
        duplex = PHYSR_FULL_DUPLEX;
    else
        duplex = PHYSR_HALF_DUPLEX;
#endif

    //1000MB Full
    if( speed == PHYSR_1000)
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x000e, 0xd6);
        uRegVal &= 0x7f;
        MHal_EMAC_WritReg8(0x000e, 0xd6, uRegVal);

        /* Set reg_xmii_type as 1G */
        uRegVal = MHal_EMAC_ReadReg8(0x121f, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x00;
        MHal_EMAC_WritReg8(0x121f, 0x60, uRegVal);

        MHalThisBCE.speed = SPEED_1000;
        MHalThisBCE.duplex = 1;

        retValue = 5;
    }
    //100MB Full/HALF
    else if (speed == PHYSR_100)
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Set reg_rgmii10_100 as 100M*/
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x02;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        // only external phy need this code
#ifndef CONFIG_ETHERNET_ALBANY
        /* Set reg_xmii_type as 10M/100M */
        uRegVal = MHal_EMAC_ReadReg8(0x121f, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x121f, 0x60, uRegVal);
#endif

        MHalThisBCE.speed = SPEED_100;

        if(duplex == PHYSR_FULL_DUPLEX)
        {
            MHalThisBCE.duplex = 1;
            retValue = 4;
        }
        else
        {
            MHalThisBCE.duplex = 0;
            retValue = 3;
        }


    }
    //10MB Full/Half
    else
    {
        /* Enable reg_rgmii_slow */
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        /* Enable reg_rgmii10_100 as 10M*/
        uRegVal = MHal_EMAC_ReadReg8(0x0033, 0xb4);
        uRegVal &= 0xfd;
        MHal_EMAC_WritReg8(0x0033, 0xb4, uRegVal);

        // only external phy need this code
#ifndef CONFIG_ETHERNET_ALBANY
        /* Set reg_xmii_type as 10M/100M */
        uRegVal = MHal_EMAC_ReadReg8(0x121f, 0x60);
        uRegVal &= 0xfc;
        uRegVal |= 0x01;
        MHal_EMAC_WritReg8(0x121f, 0x60, uRegVal);
#endif

        MHalThisBCE.speed = SPEED_10;

        if(duplex == PHYSR_FULL_DUPLEX)
        {
            MHalThisBCE.duplex = 1;
            retValue = 2;
        }
        else
        {
            MHalThisBCE.duplex = 0;
            retValue = 1;
        }

    }

    return(retValue);
}

//-------------------------------------------------------------------------------------------------
// EMAC Negotiation PHY
//-------------------------------------------------------------------------------------------------
MS_U32 MHal_EMAC_NegotiationPHY( void )
{
    // Set PHY --------------------------------------------------------------
    MS_U32 i;
    MS_U32 word_PHY = 0x00000000;
    MS_U32 retValue = 0;

    // IMPORTANT: Get real duplex by negotiation with peer.
    MS_U32 word_ETH_CTL = MHal_EMAC_Read_CTL();

    EMAC_DBG("IN\n");

    MHal_EMAC_Write_CTL( 0x0000001C | word_ETH_CTL );

    MHalThisUVE.cntChkINTCounter = 0;
    word_PHY = 0x50022100 | ( phyaddr << PHY_ADDR_OFFSET );
    MHal_EMAC_Write_MAN( word_PHY );
    while ( MHal_EMAC_CheckINTDone() != 1 );
    MHalThisUVE.flagISR_INT_DONE = 0x00;
    MHalThisUVE.cntChkINTCounter = 0;

    //Support 100 F/H-duplex & 10 F/H-duplex
    word_PHY = 0x501201E1 | ( phyaddr << PHY_ADDR_OFFSET );
    MHal_EMAC_Write_MAN( word_PHY );

    while ( MHal_EMAC_CheckINTDone() != 1 );
    MHalThisUVE.flagISR_INT_DONE = 0x00;
    MHalThisUVE.cntChkINTCounter = 0;

    word_PHY = 0x50021200 | ( phyaddr << PHY_ADDR_OFFSET );
    MHal_EMAC_Write_MAN( word_PHY );
    while ( MHal_EMAC_CheckINTDone() != 1 );
    MHalThisUVE.flagISR_INT_DONE = 0x00;
    MHalThisUVE.cntChkINTCounter = 0;

    // IMPORTANT: (20070906) There must be some delay (about 2 ~ 3 seconds) between writing 0x57821200 and 0x67FE0000.
    // Otherwise, the later EMAC_WritReg32(REG_ETH_MAN,word_PHY) has no effect.
    MHalThisBCE.duplex = 1;   // Set default as Half-duplex if negotiation fails.
    retValue = 1;
    word_PHY = 0x607E0000 | ( phyaddr << PHY_ADDR_OFFSET );  // Read for 32th reg
    MHal_EMAC_Write_MAN( word_PHY );
    MHalThisUVE.cntChkINTCounter = 0;
    while ( MHal_EMAC_CheckINTDone() != 1 );
    MHalThisUVE.flagISR_INT_DONE = 0x00;
    MHalThisUVE.cntChkINTCounter = 0;
    MHalThisUVE.cntChkCableConnect = 0;
    for ( i = 0; i < 100; i++ )
    {
        word_PHY = MHal_EMAC_Read_MAN();
        word_PHY &= 0x0000FFFF;
        if ( word_PHY & 0x00001000 )
        {
            MS_U32 test1 = ( word_PHY & 0x0000001C ) >> 2;
            if ( test1 == 0x001 || test1 == 0x002 )
            {
                MHalThisBCE.duplex = 1;
                retValue = 1;
            }
            else
            {
                MHalThisBCE.duplex = 2;
                retValue = 2;
            } // else
            //retValue = 1;
            break;
        } // if

        MsOS_DelayTask(1);//        mdelay( 1 );


        MHalThisUVE.cntChkCableConnect++;
        if ( MHalThisUVE.cntChkCableConnect == MAX_INT_COUNTER )
        {
            break;
        }
    } // for
    // NOTE: REG_ETH_CFG must be set according to new ThisBCE.duplex.
    MHal_EMAC_Write_CTL( word_ETH_CTL );
    // Set PHY --------------------------------------------------------------
    return( retValue );
}

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_timer_callback( unsigned long value )
{
    MS_U32 uRegVal;
    uRegVal = MHal_EMAC_Read_IER();
    uRegVal |= ( EMAC_INT_RCOM );
    EMAC_DBG("IN\n");

    MHal_EMAC_Write_IER( uRegVal );
}

//-------------------------------------------------------------------------------------------------
// EMAC clock on/off
//-------------------------------------------------------------------------------------------------
void MHal_EMAC_Power_On_Clk( void )
{
    EMAC_DBG("IN\n");

    MS_U8 uRegVal;

    /* Enable Ejtag*/
    //MHal_EMAC_WritReg8(0x0e, 0x3e, 0x3f);
    //MHal_EMAC_WritReg8(0x0e, 0x3f, 0x0c);

#ifndef CONFIG_ETHERNET_ALBANY //  ext phy
    /* Set GMAC ahb clock to 172MHZ */
    MHal_EMAC_WritReg8(0x1033, 0x64, 0x10);

    /* Enable GMAC tx clock */
    MHal_EMAC_WritReg8(0x121f, 0x23, 0x00);

    /* Enable gmac rx clock */
    MHal_EMAC_WritReg8(0x121f, 0x24, 0x00);
    MHal_EMAC_WritReg8(0x121f, 0x25, 0x00);

    /* IO setting (GT1), enable TX delay */
    MHal_EMAC_WritReg8(0x0033, 0xb2, 0x08);
    MHal_EMAC_WritReg8(0x0033, 0xb4, 0x1c);
    MHal_EMAC_WritReg8(0x0033, 0xb0, 0xaf);

    /* enable RX delay */
    MHal_EMAC_WritReg8(0x0033, 0xbb, 0x0f);

    /* Enable GT1 */
    uRegVal = MHal_EMAC_ReadReg8(0x0e, 0x39);
    uRegVal &= ~(0x02);
    MHal_EMAC_WritReg8(0x0e, 0x39, uRegVal);

    /* Digital synthesizer */
    MHal_EMAC_WritReg8(0x100b, 0xc6, 0x00);
    MHal_EMAC_WritReg8(0x110c, 0xc8, 0x08);
    MHal_EMAC_WritReg8(0x110c, 0xc9, 0x80);
    MHal_EMAC_WritReg8(0x110c, 0xca, 0x83);
    MHal_EMAC_WritReg8(0x110c, 0xcb, 0x06);
    MHal_EMAC_WritReg8(0x110c, 0xd0, 0x6a);
    MHal_EMAC_WritReg8(0x110c, 0xd1, 0xbc);
    MHal_EMAC_WritReg8(0x110c, 0xd2, 0x14);
    MHal_EMAC_WritReg8(0x110c, 0xd3, 0x00);
    MHal_EMAC_WritReg8(0x110c, 0xc0, 0x19);
    MHal_EMAC_WritReg8(0x110c, 0xc1, 0x00);
    MHal_EMAC_WritReg8(0x110c, 0xc2, 0x00);
    MHal_EMAC_WritReg8(0x110c, 0xc3, 0xaf);
    MHal_EMAC_WritReg8(0x110c, 0xc4, 0x0a);
    MHal_EMAC_WritReg8(0x110c, 0xc5, 0x00);
    MHal_EMAC_WritReg8(0x110c, 0xc6, 0x07);
    MHal_EMAC_WritReg8(0x110c, 0xc7, 0x00);

    /* Enable HW patch */
    uRegVal = MHal_GMAC_ReadReg8(0x121c, 0x2d);
    uRegVal |= 0x02;
    MHal_GMAC_WritReg8(0x121c, 0x2d, uRegVal);
#else // internal phy
    /* Set reg_xmii_type as int PHY mode */
    uRegVal = MHal_EMAC_ReadReg8(0x121f, 0x60);
    uRegVal &= 0xfc;
    uRegVal |= 0x02;
    MHal_EMAC_WritReg8(0x121f, 0x60, uRegVal);

    /* Set gmac ahb clock to 172MHZ */
    MHal_EMAC_WritReg8(0x1033, 0x64, 0x10);

    /* Enable gmac tx clock */
    MHal_EMAC_WritReg8(0x121f, 0x23, 0x08);

    /* Enable gmac rx clock */
    MHal_EMAC_WritReg8(0x121f, 0x24, 0x08);
    MHal_EMAC_WritReg8(0x121f, 0x25, 0x00);

    /* Init ePHY */
    uRegVal = MHal_EMAC_ReadReg8(0x000e, 0x60);
    uRegVal &= 0xfe;
    MHal_EMAC_WritReg8(0x000e, 0x60, uRegVal);

    MHal_EMAC_WritReg8(0x0032, 0x4F,  0x02);     // det max
    MHal_EMAC_WritReg8(0x0032, 0x51,  0x01);     // det min
    MHal_EMAC_WritReg8(0x0032, 0x77,  0x18);     // snr len (emc noise)

    MHal_EMAC_WritReg8(0x0031, 0x72,  0x80);     // lpbk_enable set to 0

    MHal_EMAC_WritReg8(0x0032, 0xfc,  0x00);     // Power-on LDO
    MHal_EMAC_WritReg8(0x0032, 0xfd,  0x00);     // Power-on LDO
    MHal_EMAC_WritReg8(0x0032, 0xb7,  0x07);     // Power-on ADC
    MHal_EMAC_WritReg8(0x0032, 0xcb,  0x11);     // Power-on BGAP
    MHal_EMAC_WritReg8(0x0032, 0xcc,  0x80);     // Power-on ADCPL
    MHal_EMAC_WritReg8(0x0032, 0xcd,  0xd1);     // Power-on ADCPL
    MHal_EMAC_WritReg8(0x0032, 0xd4,  0x00);     // Power-on LPF_OP
    MHal_EMAC_WritReg8(0x0032, 0xb9,  0x40);     // Power-on LPF
    MHal_EMAC_WritReg8(0x0032, 0xbb,  0x05);     // Power-on REF
    MHal_EMAC_WritReg8(0x0032, 0xea,  0x46);     // PORST
    MHal_EMAC_WritReg8(0x0033, 0xa1,  0x00);     // VBUF
    MHal_EMAC_WritReg8(0x0033, 0x3a,  0x03);     // PD_TX_IDAC, PD_TX_LD = 0
    MHal_EMAC_WritReg8(0x0033, 0x3b,  0x00);     // PD_TX_IDAC, PD_TX_LD = 0

    MHal_EMAC_WritReg8(0x0033, 0xc5,  0x00);     // 100gat
    MHal_EMAC_WritReg8(0x0033, 0x30,  0x43);     // 200gat

    MHal_EMAC_WritReg8(0x0033, 0x39,  0x41);     // [0] en_100t_phase; [6] save2x_tx

    MHal_EMAC_WritReg8(0x0033, 0xe8,  0x06);     // shadow
    MHal_EMAC_WritReg8(0x0031, 0x2b,  0x00);     // LUT tail
    MHal_EMAC_WritReg8(0x0033, 0xe8,  0x00);     // shadow
    MHal_EMAC_WritReg8(0x0031, 0x2b,  0x00);     // LUT tail

    MHal_EMAC_WritReg8(0x0033, 0xe8,  0x06);     // shadow
    MHal_EMAC_WritReg8(0x0031, 0xaa,  0x1c);     // tin17_s2
    MHal_EMAC_WritReg8(0x0031, 0xac,  0x1c);     // tin18_s2
    MHal_EMAC_WritReg8(0x0031, 0xad,  0x1c);     // tin18_s2
    MHal_EMAC_WritReg8(0x0031, 0xae,  0x1c);     // tin19_s2
    MHal_EMAC_WritReg8(0x0031, 0xaf,  0x1c);     // tin19_s2

    MHal_EMAC_WritReg8(0x0033, 0xe8,  0x00);     // shadow (back)

    MHal_EMAC_WritReg8(0x0033, 0xe8,  0x00);     // shadow
    MHal_EMAC_WritReg8(0x0031, 0xab,  0x28);     //

#endif
}

void MHal_EMAC_Power_Off_Clk( void )
{
    EMAC_DBG("IN\n");
}
