///////////////////////////////////////////////////////////////////////////////////////////////////
//
// * Copyright (c) 2006 - 2007 Mstar Semiconductor, Inc.
// This program is free software.
// You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   EMAC.h
/// @author MStar Semiconductor Inc.
/// @brief  EMAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------
// Linux EMAC.h define start
// -----------------------------------------------------------------------------

#ifndef __DRV_EMAC_H_
#define __DRV_EMAC_H_


#ifdef __cplusplus
extern "C"
{
#endif
#include <stddef.h>
    /// data type unsigned char, data length 1 byte
    typedef unsigned char               MS_U8;                              // 1 byte
    /// data type unsigned short, data length 2 byte
    typedef unsigned short              MS_U16;                             // 2 bytes
    /// data type unsigned int, data length 4 byte
    typedef unsigned int                MS_U32;                             // 4 bytes
    /// data type unsigned int, data length 8 byte
    typedef unsigned long long          MS_U64;                             // 8 bytes
    /// data type signed char, data length 1 byte
    typedef signed char                 MS_S8;                              // 1 byte
    /// data type signed short, data length 2 byte
    typedef signed short                MS_S16;                             // 2 bytes
    /// data type signed int, data length 4 byte
    typedef signed int                  MS_S32;                             // 4 bytes
    /// data type signed int, data length 8 byte
    typedef signed long long            MS_S64;                             // 8 bytes
    /// data type float, data length 4 byte
    typedef float                       MS_FLOAT;                           // 4 bytes
    /// data type pointer content
    typedef size_t                      MS_VIRT;                            // 8 bytes
    /// data type hardware physical address
    typedef size_t                      MS_PHYADDR;                         // 8 bytes
    /// data type 64bit physical address
    typedef MS_U64                      MS_PHY;                             // 8 bytes
    /// data type size_t
    typedef size_t                      MS_SIZE;                            // 8 bytes
    /// definition for MS_BOOL
    typedef unsigned char               MS_BOOL;                            // 1 byte
    /// print type  MPRI_PHY
    #define                             MPRI_PHY                            "%x"
    /// print type  MPRI_PHY
    #define                             MPRI_VIRT                           "%tx"

    typedef unsigned int                TaskPriority;                             // 4 bytes
    typedef unsigned int                InterruptNum;                             // 4 bytes
    //typedef unsigned int                InterruptCb;                             // 4 bytes
    typedef void ( *InterruptCb ) (InterruptNum eIntNum);               ///< Interrupt callback function


#define ISR_IMPLEMENTED						1			// 1:Yes, 0:No
#define MAIN_THREAD_SLEEP_MS                2000        // (20071029_CHARLES) == 2 seconds
#define SPEED_10              10 //ethertool.h
#define SPEED_100              100 //ethertool.h
#define SPEED_1000              1000 //ethertool.h
#define DUPLEX_HALF		0x00//ethertool.h
#define DUPLEX_FULL		0x01//ethertool.h



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

#define JULIAN_104_VAL                      (0x04020080)
//#include "Mhal_emac_v3.h"
//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
#ifdef RX_DESC_MODE
#ifdef EMAC_GMAC_DESCRIPTOR
struct rx_descriptor
{
  u32   addr;
  u32   low_tag;
  u32   high_tag;
  u32   reserve;
};
#else
struct rx_descriptor
{
  u32   addr;
  u32   size;
};
#endif
#endif

struct recv_desc_bufs
{
    struct rx_descriptor descriptors[RX_DESC_NUMBER];       /* must be on sizeof (rbf_t) boundary */
    char recv_buf[RX_BUFF_SIZE];                /* must be on MAX_RBUFF_SZ boundary */
};

struct _BasicConfigEMAC
{
    u8 connected;           // 0:No, 1:Yes    <== (20070515) Wait for Julian's reply
    u8 speed;               // 10:10Mbps, 100:100Mbps
    // ETH_CTL Register:
    u8 wes;                 // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    // ETH_CFG Register:
    u8 duplex;              // 1:Half-duplex, 2:Full-duplex
    u8 cam;                 // 0:No CAM, 1:Yes
    u8 rcv_bcast;           // 0:No, 1:Yes
    u8 rlf;                 // 0:No, 1:Yes receive long frame(1522)
    // MAC Address:
    u8 sa1[6];              // Specific Addr 1 (MAC Address)
    u8 sa2[6];              // Specific Addr 2
    u8 sa3[6];              // Specific Addr 3
    u8 sa4[6];              // Specific Addr 4
};
typedef struct _BasicConfigEMAC BasicConfigEMAC;

struct _UtilityVarsEMAC
{
    u32 cntChkINTCounter;
    u32 readIdxRBQP;        // Reset = 0x00000000
    u32 rxOneFrameAddr;     // Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")
    // Statistics Counters : (accumulated)
    u32 cntREG_ETH_FRA;
    u32 cntREG_ETH_SCOL;
    u32 cntREG_ETH_MCOL;
    u32 cntREG_ETH_OK;
    u32 cntREG_ETH_SEQE;
    u32 cntREG_ETH_ALE;
    u32 cntREG_ETH_DTE;
    u32 cntREG_ETH_LCOL;
    u32 cntREG_ETH_ECOL;
    u32 cntREG_ETH_TUE;
    u32 cntREG_ETH_CSE;
    u32 cntREG_ETH_RE;
    u32 cntREG_ETH_ROVR;
    u32 cntREG_ETH_SE;
    u32 cntREG_ETH_ELR;
    u32 cntREG_ETH_RJB;
    u32 cntREG_ETH_USF;
    u32 cntREG_ETH_SQEE;
    // Interrupt Counter :
    u32 cntHRESP;           // Reset = 0x0000
    u32 cntROVR;            // Reset = 0x0000
    u32 cntLINK;            // Reset = 0x0000
    u32 cntTIDLE;           // Reset = 0x0000
    u32 cntTCOM;            // Reset = 0x0000
    u32 cntTBRE;            // Reset = 0x0000
    u32 cntRTRY;            // Reset = 0x0000
    u32 cntTUND;            // Reset = 0x0000
    u32 cntTOVR;            // Reset = 0x0000
    u32 cntRBNA;            // Reset = 0x0000
    u32 cntRCOM;            // Reset = 0x0000
    u32 cntDONE;            // Reset = 0x0000
    // Flags:
    u8  flagMacTxPermit;    // 0:No,1:Permitted.  Initialize as "permitted"
    u8  flagISR_INT_RCOM;
    u8  flagISR_INT_RBNA;
    u8  flagISR_INT_DONE;
    u8  flagPowerOn;        // 0:Poweroff, 1:Poweron
    u8  initedEMAC;         // 0:Not initialized, 1:Initialized.
    u8  flagRBNA;
    // Misc Counter:
    u32 cntRxFrames;        // Reset = 0x00000000 (Counter of RX frames,no matter it's me or not)
    u32 cntReadONE_RX;      // Counter for ReadONE_RX_Frame
    u32 cntCase20070806;
    u32 cntChkToTransmit;
    // Misc Variables:
    u32 mainThreadTasks;    // (20071029_CHARLES) b0=Poweroff,b1=Poweron
};
typedef struct _UtilityVarsEMAC UtilityVarsEMAC;
#if 0
struct EMAC_private
{
    struct net_device *dev;
    struct net_device_stats stats;
    struct mii_if_info mii;             /* ethtool support */
    struct timer_list Link_timer;
    u32 padmux_type;
    u32 hardware_type;
    u32 initstate;
    u32 msglvl;
    BasicConfigEMAC ThisBCE;
    UtilityVarsEMAC ThisUVE;
    /* Memory */
    phys_addr_t     RAM_VA_BASE;
    phys_addr_t     RAM_PA_BASE;
    phys_addr_t     RAM_VA_PA_OFFSET;
    phys_addr_t     RX_DESC_BASE;
#ifndef RX_ZERO_COPY
    phys_addr_t     RX_BUFFER_BASE;
#endif
#ifdef TX_DESC_MODE
    phys_addr_t     TX_LP_DESC_BASE;
    phys_addr_t     TX_HP_DESC_BASE;
#endif
    phys_addr_t     TX_BUFFER_BASE;
    /* PHY */
    u8  phyaddr;
    u32 phy_status_register;
    u32 phy_type;             /* type of PHY (PHY_ID) */
    spinlock_t irq_lock;                /* lock for MDI interface */
    spinlock_t tx_lock;                 /* lock for MDI interface */
    short phy_media;                    /* media interface type */
    /* Transmit */
    u32 tx_index;
    u32 tx_ring_entry_number;
#ifdef TX_DESC_MODE
    struct tx_descriptor *tx_desc_list;
    struct sk_buff *tx_desc_sk_buff_list[TX_LOW_PRI_DESC_NUMBER];
    u32 tx_desc_write_index;
    u32 tx_desc_read_index;
    u32 tx_desc_queued_number;
    u32 tx_desc_count;
    u32 tx_desc_full_count;
#endif
#ifdef TX_SOFTWARE_QUEUE
    struct tx_ring tx_swq[TX_SW_QUEUE_SIZE];
    unsigned int tx_rdidx;              /* TX_SW_QUEUE read to hw index */
    unsigned int tx_wridx;              /* TX_SW_QUEUE write index */
    unsigned int tx_clidx;              /* TX_SW_QUEUE clear index */

    unsigned int tx_rdwrp;              /* TX_SW_QUEUE read to hw index wrap*/
    unsigned int tx_wrwrp;              /* TX_SW_QUEUE write index wrap*/
    unsigned int tx_clwrp;              /* TX_SW_QUEUE clear index wrap */
    unsigned int tx_swq_full_cnt;       /* TX_SW_QUEUE full stopped count*/

    unsigned int irqcnt;
    unsigned int tx_irqcnt;
#endif
#ifdef TX_NAPI
    struct napi_struct napi_tx;
#endif
    /* Receive */
    u32 ROVR_count;
    u32 full_budge_count;
    u32 polling_count;
    u32 max_polling;
    u32 rx_ring_entry_number;
#ifdef RX_DESC_MODE
    struct rx_descriptor *rx_desc_list;
    u32 rx_desc_read_index;
#ifdef RX_ZERO_COPY
    struct sk_buff *rx_desc_sk_buff_list[RX_DESC_NUMBER];
    u32 rx_desc_free_index;
    u32 rx_desc_free_number;
#endif
#endif
#ifdef RX_NAPI
    struct napi_struct napi_rx;
#endif
    /* suspend/resume */
    u32 ep_flag;
};
#endif

#ifdef __cplusplus
}
#endif

#endif
