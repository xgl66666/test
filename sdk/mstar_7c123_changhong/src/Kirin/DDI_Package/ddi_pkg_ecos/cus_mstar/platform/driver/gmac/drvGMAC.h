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
/// @file   drvEMAC.h
/// @author MStar Semiconductor Inc.
/// @brief  EMAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRV_GMAC_H__
#define __DRV_GMAC_H__


#ifdef __cplusplus
extern "C"
{
#endif


#define EMAC_ON_URANUS                      0
#define EMAC_ON_TITANIA                     1
#define EMAC_ON_OBERON                      2
#define OBERON

#ifdef OBERON
#define EMAC_CHIP_ID EMAC_ON_OBERON
#else
#define EMAC_CHIP_ID EMAC_ON_URANUS
#endif

// Compiler Switches
#define USE_TASK							1			// 1:Yes, 0:No
#define ISR_IMPLEMENTED						1			// 1:Yes, 0:No
#define USE_EMAC_DRIVER                     0           // 1:Yes, 0:No
#define FPGA_CPU_CLOCK                      143			// Unit: 100Khz. Affect the "NopDelayUS()"
#define MAIN_THREAD_SLEEP_MS                2000        // (20071029_CHARLES) == 2 seconds
//EMAC_USE_RMII for Oberon only
#define EMAC_USE_RMII                       1

#if (EMAC_USE_RMII==0)
**** need to (1)modify REG_TOP_ET_PAD=0x9, (2)disable MDrv_CReader_Initial();//tony*****
#endif

//#if (EMAC_CHIP_ID == EMAC_ON_OBERON)//In Oberon, must use SW_DESC_SUPPORT to prevent rx len error
#define SW_DESC_SUPPORT                     1
//#else
//#define SW_DESC_SUPPORT                     0
//#endif

#define RX_CHKSUM_SUPPORT                   0
#define DELAY_INTR                          0
#define MIU_WRITE_PROTECT                   0
// end of Compiler Switches


#define SW_DESC_RX_BUFFER_SIZE              (64 * 1024)

// Constant: ----------------------------------------------------------------
#if (EMAC_CHIP_ID == EMAC_ON_URANUS)
#define _REG_ADDR_BASE 0xBF809C00
#endif

#if (EMAC_CHIP_ID == EMAC_ON_TITANIA)
#define _REG_ADDR_BASE 0xBF804000
#endif

#if (EMAC_CHIP_ID == EMAC_ON_OBERON)
#define _REG_ADDR_BASE 0xBF806A00
#endif

// Register MAP:
#define REG_ETH_CTL    		    			0x00000000         // Network control register
#define REG_ETH_CFG 						0x00000004         // Network configuration register
#define REG_ETH_SR 			    			0x00000008         // Network status register
#define REG_ETH_TAR 						0x0000000C         // Transmit address register
#define REG_ETH_TCR 						0x00000010         // Transmit control register
#define REG_ETH_TSR 						0x00000014         // Transmit status register
#define REG_ETH_RBQP 		    			0x00000018         // Receive buffer queue pointer
#define REG_ETH_BUFF_CONF 	    			0x0000001C         // Receive Buffer Configuration
#define REG_ETH_RSR 						0x00000020         // Receive status register
#define REG_ETH_ISR 						0x00000024         // Interrupt status register
#define REG_ETH_IER 						0x00000028         // Interrupt enable register
#define REG_ETH_IDR 						0x0000002C         // Interrupt disable register
#define REG_ETH_IMR 						0x00000030         // Interrupt mask register
#define REG_ETH_MAN 						0x00000034         // PHY maintenance register
#define REG_ETH_BUFFRDPTR 	    			0x00000038         // Receive First Full Pointer
#define REG_ETH_BUFFWRPTR 	    			0x0000003C         // Receive Current pointer
#define REG_ETH_FRA 						0x00000040         // Frames transmitted OK
#define REG_ETH_SCOL 		    			0x00000044         // Single collision frames
#define REG_ETH_MCOL 		    			0x00000048         // Multiple collision frames
#define REG_ETH_OK 			    			0x0000004C         // Frames received OK
#define REG_ETH_SEQE 		    			0x00000050         // Frame check sequence errors
#define REG_ETH_ALE 						0x00000054         // Alignment errors
#define REG_ETH_DTE 						0x00000058         // Deferred transmission frames
#define REG_ETH_LCOL 		    			0x0000005C         // Late collisions
#define REG_ETH_ECOL 		    			0x00000060         // Excessive collisions
#define REG_ETH_TUE 						0x00000064         // Transmit under-run errors
#define REG_ETH_CSE 						0x00000068         // Carrier sense errors
#define REG_ETH_RE 			    			0x0000006C         // Receive resource error
#define REG_ETH_ROVR 		    			0x00000070         // Received overrun
#define REG_ETH_SE 			    			0x00000074         // Received symbols error
#define REG_ETH_ELR 						0x00000078         // Excessive length errors
#define REG_ETH_RJB 						0x0000007C         // Receive jabbers
#define REG_ETH_USF 						0x00000080         // Undersize frames
#define REG_ETH_SQEE 		    			0x00000084         // SQE test errors
#define REG_ETH_HSL 						0x00000090         // Hash register [31:0]
#define REG_ETH_HSH 						0x00000094         // Hash register [63:32]
#define REG_ETH_SA1L 		    			0x00000098         // Specific address 1 first 4 bytes
#define REG_ETH_SA1H 		    			0x0000009C         // Specific address 1 last  2 bytes
#define REG_ETH_SA2L 		    			0x000000A0         // Specific address 2 first 4 bytes
#define REG_ETH_SA2H 		    			0x000000A4         // Specific address 2 last  2 bytes
#define REG_ETH_SA3L 		    			0x000000A8         // Specific address 3 first 4 bytes
#define REG_ETH_SA3H 		    			0x000000AC         // Specific address 3 last  2 bytes
#define REG_ETH_SA4L 		    			0x000000B0         // Specific address 4 first 4 bytes
#define REG_ETH_SA4H 		    			0x000000B4         // Specific address 4 last  2 bytes
#define REG_TAG_TYPE 		    			0x000000B8         // tag type of the frame
#define EMAC_JULIAN_0100					0x00000100
#define EMAC_JULIAN_0104					0x00000104
#define EMAC_JULIAN_0108					0x00000108
#define DELAY_INTR_CTRL_REG                 0x0000010C
#define DELAY_INTR_STATUS_REG               0x00000110
#define REG_MIU_WRUPB_l                     0x0000013C
#define REG_MIU_WRUPB_h                     0x00000140
#define REG_MIU_WRLPB_l                     0x00000144
#define REG_MIU_WRLPB_h                     0x00000148
#define REG_CAMA0_l 						0x00000200         // 16 LSB of CAM address  0
#define REG_CAMA0_h 						0x00000204         // 32 MSB of CAM address  0
#define REG_CAMA62_l 		    			0x000003F0         // 16 LSB of CAM address 62
#define REG_CAMA62_h  		    			0x000003F4         // 32 MSB of CAM address 62

// Interrupt Mask :
#define INT_HRESP 							0x00000800
#define INT_ROVR  							0x00000400
#define INT_LINK  							0x00000200
#define INT_TIDLE 							0x00000100
#define INT_TCOM  							0x00000080
#define INT_TBRE  							0x00000040
#define INT_RTRY  							0x00000020
#define INT_TUND  							0x00000010
#define INT_TOVR  							0x00000008
#define INT_RBNA  							0x00000004
#define INT_RCOM  							0x00000002
#define INT_DONE 							0x00000001

// Basic Config EMAC:
#define EMAC_SPEED_10						10			//  10Mbps
#define EMAC_SPEED_100						100			// 100Mbps
#define EMAC_DUPLEX_HALF					1
#define EMAC_DUPLEX_FULL					2

/* matt.lin -- PHY control */
#define PHY_LOW_HIGH	0x01
#define PHY_LOW_HIGH_SHIFT	30
#define PHY_WRITE_OP	0x01
#define PHY_READ_OP	0x02
#define PHY_RW_SHIFT	28

#define PHY_ADDR_SHIFT	23
#define PHY_REG_CONFIG	0
//#define PHY_REG_STATUS	1
#define PHY_REG_AN_ADV  4
#define PHY_REG_AN_LPA  5
#define PHY_REG_SPECIAL 31
#define PHY_REG_IP100A_STATUS_MON 16
#define PHY_REG_SHIFT	18
#define PHY_CODE		0x02
#define PHY_CODE_SHIFT	16

#define SW_DESC_ENABLE 1
#define ONE_RX_BUF_SIZE 1600
#define RX_CHKSUM_ENABLE 0xE
#define DELAY_INTR_ENABLE 0x80

#include "mhal_gmac.h"
//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
struct rbf_t
{
  unsigned int  addr;
  unsigned long low_tag;
  unsigned long high_tag;
  unsigned long dummy;
};

struct recv_desc_bufs
{
    struct rbf_t descriptors[RBQP_LENG];       /* must be on sizeof (rbf_t) boundary */
    char recv_buf[RX_BUFFER_SIZE];                /* must be on MAX_RBUFF_SZ boundary */
};

struct _BasicConfigEMAC
{
    MS_U8                  connected;          // 0:No, 1:Yes    <== (20070515) Wait for Julian's reply
	MS_U8					speed;				// 10:10Mbps, 100:100Mbps
	// ETH_CTL Register:
	MS_U8                  wes;				// 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
	// ETH_CFG Register:
	MS_U8					duplex;				// 1:Half-duplex, 2:Full-duplex
	MS_U8					cam;                // 0:No CAM, 1:Yes
	MS_U8 					rcv_bcast;      	// 0:No, 1:Yes
	MS_U8					rlf;                // 0:No, 1:Yes receive long frame(1522)
	// MAC Address:
	MS_U8					sa1[6];				// Specific Addr 1 (MAC Address)
	MS_U8					sa2[6];				// Specific Addr 2
	MS_U8					sa3[6];				// Specific Addr 3
	MS_U8					sa4[6];				// Specific Addr 4
};
typedef struct _BasicConfigEMAC BasicConfigEMAC;

struct _UtilityVarsEMAC
{
	MS_U32 				readIdxRBQP;		// Reset = 0x00000000
	MS_U32 				rxOneFrameAddr;		// Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")
	// Statistics Counters : (accumulated)
	MS_U32                 cntREG_ETH_FRA;
	MS_U32                 cntREG_ETH_SCOL;
	MS_U32                 cntREG_ETH_MCOL;
	MS_U32                 cntREG_ETH_OK;
	MS_U32                 cntREG_ETH_SEQE;
	MS_U32                 cntREG_ETH_ALE;
	MS_U32                 cntREG_ETH_DTE;
	MS_U32                 cntREG_ETH_LCOL;
	MS_U32                 cntREG_ETH_ECOL;
	MS_U32                 cntREG_ETH_TUE;
	MS_U32                 cntREG_ETH_CSE;
	MS_U32                 cntREG_ETH_RE;
	MS_U32                 cntREG_ETH_ROVR;
	MS_U32                 cntREG_ETH_SE;
	MS_U32                 cntREG_ETH_ELR;
	MS_U32                 cntREG_ETH_RJB;
	MS_U32                 cntREG_ETH_USF;
	MS_U32                 cntREG_ETH_SQEE;
	// Interrupt Counter :
	MS_U32 				cntHRESP;			// Reset = 0x0000
	MS_U32 				cntROVR;			// Reset = 0x0000
	MS_U32 				cntLINK;			// Reset = 0x0000
	MS_U32 				cntTIDLE;			// Reset = 0x0000
	MS_U32 				cntTCOM;			// Reset = 0x0000
	MS_U32 				cntTBRE;			// Reset = 0x0000
	MS_U32 				cntRTRY;			// Reset = 0x0000
	MS_U32 				cntTUND;			// Reset = 0x0000
	MS_U32 				cntTOVR;			// Reset = 0x0000
	MS_U32 				cntRBNA;			// Reset = 0x0000
	MS_U32 				cntRCOM;			// Reset = 0x0000
	MS_U32 				cntDONE;			// Reset = 0x0000
	// Flags:
	MS_U8                  flagMacTxPermit;    // 0:No,1:Permitted.  Initialize as "permitted"
	MS_U8                  flagISR_INT_RCOM;
	MS_U8                  flagISR_INT_RBNA;
	MS_U8                  flagISR_INT_DONE;
	MS_U8                  flagPowerOn;        // 0:Poweroff, 1:Poweron
	MS_U8                  initedEMAC;         // 0:Not initialized, 1:Initialized.
	// Misc Counter:
	MS_U32     			cntRxFrames;		// Reset = 0x00000000 (Counter of RX frames,no matter it's me or not)
	MS_U32                 cntReadONE_RX;      // Counter for ReadONE_RX_Frame
	MS_U32                 cntCase20070806;
	MS_U32                 cntChkToTransmit;
	// Misc Variables:
	MS_U32                 mainThreadTasks;    // (20071029_CHARLES) b0=Poweroff,b1=Poweron
};
typedef struct _UtilityVarsEMAC UtilityVarsEMAC;

/// EMAC DDI return value
typedef MS_U32                                 DRVEMAC_RESULT;
/// @name DRVEMAC_RESULT
/// @ref DRVEMAC_RESULT
/// return value
/// @{
#define DRVEMAC_OK                          0x00000000
#define DRVEMAC_FAIL                        0x00000001

/// EMAC OID
typedef MS_U32                                 EMAC_OID;
enum {
    EOID_LINK_SPEED                       = 0x00000001  ,   // 4B
    EOID_MEDIA_CONNECT_STATUS                           ,   // 4B
    EOID_MAC_ADDRESS                                    ,   // 6B
    EOID_DUPLEX                                         ,   // 4B
    EOID_XMIT_OK                                        ,   // 4B
    EOID_RCV_OK                                         ,   // 4B
    EOID_XMIT_ERROR                                     ,   // 4B
    EOID_RCV_ERROR                                      ,   // 4B
    EOID_RCV_NO_BUFFER                                  ,   // 4B
};
#define EMAC_LINK_SPEED_10                  10
#define EMAC_LINK_SPEED_100                 100
#define EMAC_CONN_STATUS_CONNECTED          1
#define EMAC_CONN_STATUS_DISCONNECTED       0
#define EMAC_DUPLEX_HALF                    1
#define EMAC_DUPLEX_FULL                    2

//EMAC link status
#define EMAC_LS_WAIT   0x80
//Link: 0=unlink, 1=link
#define EMAC_LS_LINK   0x04
//Speed: 0=10Mb, 1=100Mb
#define EMAC_LS_SPEED  0x02
//Duplex: 0=half, 1=full
#define EMAC_LS_DUPLEX 0x01
// Function Declaration -----------------------------------------------------
void EMAC_Sync(void);
void CvtEndian(void *xval,MS_U32 xsize);
MS_U32  ReadSINGLE_RX_Frames(void);
MS_U32  RevMACAddressRXFrame(MS_U32 xFrameAddr);
void CheckToTransmit(MS_U32 xFrameLeng);
void SendFrameOfStatusData(void);
int CheckEMAC_Initialized(void);
// Four main functions:
MS_U32 ReceivePacket(MS_U32 xframeAddr,MS_U32 xframeLeng);      // IMPORTANT: Callback function of APPL
void           SendPacket(MS_U32 xframeAddr,MS_U32 xframeLeng);
DRVEMAC_RESULT QueryInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xread);
DRVEMAC_RESULT SetInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xwrit);

MS_BOOL appEMAC_Create(void);
DRVEMAC_RESULT MDrv_EMAC_Init(void);

/* matt.lin(20071018) */
void MDrv_EMAC_ChangeMacAddr(MS_U8*);
MS_BOOL MDrv_EMAC_ConnectStatus(void);


// Interface between drvEMAC and kernel driver
//void emac_deliver(MS_U32 xTxRxWord);

/* matt.lin(20071009) -- Use Network Alarm Task to handle DSR. */
void emac_dsr(MS_U32 xTxRxWord);
void init_all_network_interfaces(void);

// 20071029_CHARLES : Power off mode APIs
void EMAC_PowerOff(void);
void EMAC_PowerOff_Set(void);
void EMAC_PowerOn(void);
void EMAC_PowerOn_Set(void);

extern MS_BOOL (*PHY_ConnectStatus)(void);


#ifdef __cplusplus
}
#endif


#endif

