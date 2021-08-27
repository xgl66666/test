#include <mhal_emac.h>

#define RBQP_LENG                           (0x10)
#define RTK_OUI_MSB                         (0x1C)
#define RAM_ADDR_BASE                       (0xA0000000)

#ifdef SOFTWARE_DESCRIPTOR
#define RX_BUFFER_SEL                       0x0001          // 0x0=2KB,0x1=4KB,0x2=8KB,0x3=16KB, 0x09=1MB
#define RX_BUFFER_SIZE                      (RBQP_LENG*0x600)  //0x10000//0x20000//
#else
#define RX_BUFFER_SEL						0x0003		// 0x0=2KB,0x1=4KB,0x2=8KB,0x3=16KB, 0x09=1MB
#define RX_BUFFER_SIZE                      0x4000		//0x10000//0x20000//
#endif

#define SOFTWARE_DESCRIPTOR_ENABLE          0x0001
#define MAX_RX_DESCR                        RBQP_LENG       //32   /* max number of receive buffers */

typedef struct _BasicConfigEMAC
{
    u8                  connected;          // 0:No, 1:Yes    <== (20070515) Wait for Julian's reply
    u8                  speed;              // 10:10Mbps, 100:100Mbps
	// ETH_CTL Register:
    u8                  wes;                // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
	// ETH_CFG Register:
    u8					duplex;             // 1:Half-duplex, 2:Full-duplex
    u8					cam;                // 0:No CAM, 1:Yes
    u8 					rcv_bcast;          // 0:No, 1:Yes
    u8                  rlf;                // 0:No, 1:Yes receive long frame(1522)
    // MAC Address:
    u8                  sa1[6];             // Specific Addr 1 (MAC Address)
    u8                  sa2[6];             // Specific Addr 2
    u8                  sa3[6];             // Specific Addr 3
    u8                  sa4[6];             // Specific Addr 4
    u8                  loopback;
} BasicConfigEMAC;

typedef struct _UtilityVarsEMAC
{
    u32                 cntChkINTCounter;
    u32 				readIdxRBQP;        // Reset = 0x00000000
    u32 				rxOneFrameAddr;     // Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")
    // Statistics Counters : (accumulated)
    u32                 cntREG_ETH_FRA;
    u32                 cntREG_ETH_SCOL;
    u32                 cntREG_ETH_MCOL;
    u32                 cntREG_ETH_OK;
    u32                 cntREG_ETH_SEQE;
    u32                 cntREG_ETH_ALE;
    u32                 cntREG_ETH_DTE;
    u32                 cntREG_ETH_LCOL;
    u32                 cntREG_ETH_ECOL;
    u32                 cntREG_ETH_TUE;
    u32                 cntREG_ETH_CSE;
    u32                 cntREG_ETH_RE;
    u32                 cntREG_ETH_ROVR;
    u32                 cntREG_ETH_SE;
    u32                 cntREG_ETH_ELR;
    u32                 cntREG_ETH_RJB;
    u32                 cntREG_ETH_USF;
    u32                 cntREG_ETH_SQEE;
    // Interrupt Counter :
    u32                 cntHRESP;
    u32                 cntROVR;
    u32                 cntLINK;
    u32                 cntTIDLE;
    u32                 cntTCOM;
    u32                 cntTBRE;
    u32                 cntRTRY;
    u32                 cntTUND;
    u32                 cntTOVR;
    u32                 cntRBNA;
    u32                 cntRCOM;
    u32                 cntDONE;
    // Flags:
    u8                  flagMacTxPermit;    // 0:No,1:Permitted.  Initialize as "permitted"
    u8                  flagISR_INT_RCOM;
    u8                  flagISR_INT_RBNA;
    u8                  flagISR_INT_DONE;
    u8                  flagPowerOn;        // 0:Poweroff, 1:Poweron
    u8                  initedEMAC;         // 0:Not initialized, 1:Initialized.
    u8                  flagRBNA;
    // Misc Counter:
    u32                 cntRxFrames;        // Reset = 0x00000000 (Counter of RX frames,no matter it's me or not)
    u32                 cntReadONE_RX;      // Counter for ReadONE_RX_Frame
    u32                 cntCase20070806;
    u32                 cntChkToTransmit;
    // Misc Variables:
    u32                 mainThreadTasks;    // (20071029_CHARLES) b0=Poweroff,b1=Poweron
} UtilityVarsEMAC;



struct emac_private{
    unsigned char phy_id;
    struct _BasicConfigEMAC ThisBCE;
    struct _UtilityVarsEMAC ThisUVE;
    u32_t u32rxBuffIndex;
};

struct emac_rx_descriptor{
    u32_t ownership : 1;
    u32_t wrap : 1;
    u32_t pkt_addr : 30;
    u32_t pkt_leng : 11;
    u32_t word1 : 21;
} __attribute__((packed));

struct emac_rx_packet_buffer{
    u8_t buf[0x600];
};

struct emac_rx_memory{
    struct emac_rx_descriptor desc[RBQP_LENG];
    struct emac_rx_packet_buffer __attribute__ ((aligned (16384))) pkt_buf[RBQP_LENG];
};

#ifndef __EMAC_NETIF_C__
extern struct netif emac_netif;
extern struct emac_rx_memory * emac_rx_mem;
//extern struct emac_rx_descriptor __attribute__ ((aligned (4096))) desc[RBQP_LENG];
//extern struct emac_rx_packet_buffer __attribute__ ((aligned (16384))) pkt_buf[RBQP_LENG];

#endif

err_t emac_netif_init(struct netif *netif);
