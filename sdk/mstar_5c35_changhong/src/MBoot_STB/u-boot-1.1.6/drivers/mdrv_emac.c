
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
/// @file   drvEMAC.c
/// @brief  u-boot EMAC Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------#include "e1000.h"

#include "mdrv_emac.h"

u8 MY_MAC[6] = { 0x00, 0x30, 0x1B, 0xBA, 0x02, 0xDB };
unsigned char phy_id=0;

#define EMAC_DBG_TFTP       (0)
#define EMAC_DBG_MSG(x)     //x
#define RTK_OUI_MSB         (0x1C)
#if (EMAC_DBG_TFTP)
s32 rxpre_seq = 0, txpre_seq = 0;
#endif

//For BIST
#define PHY_LOOPBACK    (1)
#define MAC_LOOPBACK    (2)

//--------------------------------------------------------------------------------------------------
//  Global variable
//--------------------------------------------------------------------------------------------------
u32 EMAC_MEM_BASE = 0xA0000000, RX_BUFFER_BASE=0, RBQP_BASE=0, TX_BUFFER_BASE=0;
u8 *packet = (u8 *)packet_data;
struct eth_device nic_device;

static int MDrv_EMAC_tx(struct eth_device *nic, volatile void *tx_packet, int length);
void MDrv_EMAC_DumpMem(u32 addr, u32 len);
#if (SUPPORT_ALBANY)
u32 albany_read_reg(u32 reg);
void albany_write_reg(u32 reg, u32 val);
#endif

//-------------------------------------------------------------------------------------------------
//Initialize and start the Receiver and Transmit subsystems
//-------------------------------------------------------------------------------------------------
static void MDrv_EMAC_start (struct eth_device *nic)
{
    struct emac_hw *hw = (struct emac_hw *) nic->priv;
    u32 uRegVal;
#ifdef SOFTWARE_DESCRIPTOR
    u32 i;
#endif

    // Initialize Receive Buffer Descriptors
    memset((u8*)(RBQP_BASE|RAM_ADDR_BASE),0x00,RBQP_SIZE);
#ifdef SOFTWARE_DESCRIPTOR
    for(i=0; i<RBQP_LENG;i++)
    {
        if(i<(RBQP_LENG-1))
            MHal_EMAC_WritRam32((RBQP_BASE|RAM_ADDR_BASE), i*8, (RX_BUFFER_BASE+0x600*i));
        else
            MHal_EMAC_WritRam32((RBQP_BASE|RAM_ADDR_BASE), i*8, (RX_BUFFER_BASE+0x600*i+EMAC_DESC_WRAP));
    }
#else
    MHal_EMAC_WritRam32((RBQP_BASE|RAM_ADDR_BASE),(RBQP_SIZE-0x08),0x00000002);
#endif

#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    Chip_Flush_Memory();
#endif

    // set offset of read and write pointers in the receive circular buffer //
    uRegVal = MHal_EMAC_Read_BUFF();
    uRegVal = (RX_BUFFER_BASE|RX_BUFFER_SEL);
    MHal_EMAC_Write_BUFF(uRegVal);
    MHal_EMAC_Write_RDPTR(0);
    MHal_EMAC_Write_WRPTR(0);

    // Program address of descriptor list in Rx Buffer Queue register //
    MHal_EMAC_Write_RBQP(RBQP_BASE);
    //Reset buffer index//
    hw->rxBuffIndex = 0;

    // Enable Receive and Transmit //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= (EMAC_RE | EMAC_TE);
    MHal_EMAC_Write_CTL(uRegVal);
}

//-------------------------------------------------------------------------------------------------
// Open the ethernet interface
//-------------------------------------------------------------------------------------------------
static int MDrv_EMAC_open (struct eth_device *nic, bd_t * bis)
{
    // struct emac_hw *hw = (struct emac_hw *) nic->priv;
    u32 uRegVal;

    //ato  EMAC_SYS->PMC_PCER = 1 << EMAC_ID_EMAC;   //Re-enable Peripheral clock //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= EMAC_CSR;
    MHal_EMAC_Write_CTL(uRegVal);
    // Enable PHY interrupt //
    MHal_EMAC_enable_phyirq ();

    // Enable MAC interrupts //
    uRegVal = EMAC_INT_RCOM |EMAC_INT_RBNA
    | EMAC_INT_TUND | EMAC_INT_RTRY | EMAC_INT_TCOM
    | EMAC_INT_ROVR | EMAC_INT_HRESP;
    MHal_EMAC_Write_IER(uRegVal);

    MDrv_EMAC_start (nic);
    return 0;
}

/**************************************************************************
POLL - Wait for a frame
***************************************************************************/
static int MDrv_EMAC_rx(struct eth_device *nic)
{
    struct emac_hw *hw = (struct emac_hw *) nic->priv;
    struct recv_desc_bufs *dlist;
    unsigned char *p_recv;

    u32 pktlen;
    u32 uRegVal=0;
    u32 wrap_bit;
#if (EMAC_DBG_TFTP)
    u8 type = 0;
    u16 block = 0;
#endif

    dlist = hw->dlist ;
    // If any Ownership bit is 1, frame received.
    flush_cache((ulong)dlist ,sizeof(dlist));
#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    Chip_Flush_Memory();
    //flush read buffer data
    ((volatile u32 *)0x80001000)[0];
#endif

    while ( (dlist->descriptors[hw->rxBuffIndex].addr )& EMAC_DESC_DONE)
    {
        p_recv = (unsigned char *) ((((dlist->descriptors[hw->rxBuffIndex].addr)&0x0FFFFFFF) |RAM_ADDR_BASE) &~(EMAC_DESC_DONE | EMAC_DESC_WRAP));
        pktlen = (u32)dlist->descriptors[hw->rxBuffIndex].size & 0x7ff;    /* Length of frame including FCS */

#if (EMAC_DBG_TFTP)
        block = *(p_recv+0x2d) | (*(p_recv+0x2c) << 8);
        type = *(p_recv+0x2b);
        if (3 == type && block >0)
        {
            if (block == rxpre_seq)
            {
                EMAC_DBG_MSG(printf("xxxxxx Rx tftp Packet:%lx Type:%u block:%u again!!!\n", p_recv, type, block));
            }
            else
                rxpre_seq++;
        }
        else if (4 == type)
        {
            EMAC_DBG_MSG(printf("Rx tftp ACK Packet:%lx Type:%u block:%u\n", p_recv, type, block));
        }
        else if (6 == type)
        {
            EMAC_DBG_MSG(printf("\nRx tftp Opt ACK Packet:%lx Type:%u\n", p_recv, type));
            EMAC_DBG_MSG(printf("Ready to tftp transaction!!!!!!!\n"));
        }
        else if (block != (rxpre_seq+1) && rxpre_seq > 0)
        {
            EMAC_DBG_MSG(printf("\nError tftp packet block:%u rsr:%x tsr:%x\n", (rxpre_seq+1), MHal_EMAC_Read_RSR(), MHal_EMAC_Read_TSR()));
            EMAC_DBG_MSG(printf("\ndescriptor:%lx size:%lx\n", &(dlist->descriptors[hw->rxBuffIndex]), pktlen));
            //MDrv_EMAC_DumpMem(p_recv, 0x40);
            //MDev_EMAC_stats();
        }
#endif

        flush_cache((ulong)p_recv,pktlen);
#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
        Chip_Flush_Memory();
#endif
        if ((unsigned char *) (dlist->recv_buf + RX_BUFFER_SIZE) >
        (p_recv + ROUND_SUP_4 (pktlen)))
        {
            // the frame is not splitted in two parts //
            memcpy(packet, p_recv, pktlen);
            // update consumer pointer//
            uRegVal = MHal_EMAC_Read_RDPTR();
            uRegVal += ROUND_SUP_4(pktlen);
            MHal_EMAC_Write_RDPTR(uRegVal);
        }
        else
        {
            //the frame is splitted in two parts //
            int firstPacketPartSize =
            (unsigned char *) dlist->recv_buf + RX_BUFFER_SIZE - p_recv;
            int secondPacketSize =(ROUND_SUP_4(pktlen) - firstPacketPartSize);
            memcpy((unsigned char *)packet, p_recv, firstPacketPartSize);
            memcpy((unsigned char *)(packet+firstPacketPartSize), (unsigned char *) dlist->recv_buf, secondPacketSize);

            // update consumer pointer and toggle the wrap bit //
            wrap_bit = (MHal_EMAC_Read_RDPTR() & EMAC_WRAP_R) ^ EMAC_WRAP_R;
            MHal_EMAC_Write_RDPTR(secondPacketSize | wrap_bit);
        }
        flush_cache((ulong)packet,pktlen);
#if defined(CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12)|| defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
        Chip_Flush_Memory();
#endif
        if (ThisBCE.loopback == PHY_LOOPBACK)
        {
            printf("Rx Data");
            MDrv_EMAC_DumpMem((u32)packet, 0x40);
        }
        else if (ThisBCE.loopback == MAC_LOOPBACK)
        {
            MDrv_EMAC_tx(nic, (void *)packet, pktlen);
        }
        else
            NetReceive((uchar *)packet, pktlen);

        dlist->descriptors[hw->rxBuffIndex].addr  &= ~EMAC_DESC_DONE;  /* reset ownership bit */
        //wrap after last buffer //
        hw->rxBuffIndex++;
        if (hw->rxBuffIndex == MAX_RX_DESCR)
        {
            hw->rxBuffIndex = 0;
        }
    }
    return 1;
}

/**************************************************************************
TRANSMIT - Transmit a frame
***************************************************************************/
#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)
//temp solution for janus flush cache problem
static char t_packet[2096];
#endif
static int MDrv_EMAC_tx(struct eth_device *nic, volatile void *tx_packet, int length)
{
#if (EMAC_DBG_TFTP)
    u8 *ptr = (u8 *)tx_packet, type;
    u16 block = 0;
#endif
    while (length < 60)         //padding to 60 bytes
    {
        *((u8*)tx_packet+length)=0;
        length++;
    }
    flush_cache((ulong)tx_packet,length);
#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    Chip_Flush_Memory();
#endif
#if (EMAC_DBG_TFTP)
    type = *(ptr+0x2b);
    block = *(ptr+0x2d) | (*(ptr+0x2c) << 8);
    if (1 == type)
     {
        EMAC_DBG_MSG(printf("\nTx tftp Request Type:%u\n", ptr, type));
     }
    else if (3 == type)
    {
        EMAC_DBG_MSG(printf("Tx tftp Packet:%lx Type:%u block:%u\n", ptr, type, block));
    }
    else if (4 == type && block >0)
    {
        if (block == txpre_seq)
            EMAC_DBG_MSG(printf("xxxxxxx Tx tftp ACK Packet:%lx Type:%u block:%u again!!!!\n", ptr, type, block));
        else
            block++;
    }
#endif

#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)
    memcpy((void *)t_packet, (void *)tx_packet, length);
    MHal_EMAC_Write_TAR((u32)t_packet);
#else
    MHal_EMAC_Write_TAR((u32)tx_packet);
#endif

    if (ThisBCE.loopback == PHY_LOOPBACK)
    {
        printf("Tx Data");
        MDrv_EMAC_DumpMem((u32)tx_packet, 0x40);
    }
    // Set length of the packet in the Transmit Control register //
    MHal_EMAC_Write_TCR(length);

    return 1;
}

//-------------------------------------------------------------------------------------------------
// Close the interface
//-------------------------------------------------------------------------------------------------
void MDrv_EMAC_close(struct eth_device *nic)
{
    u32 uRegVal;
    //Disable Receiver and Transmitter //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal &= ~(EMAC_TE | EMAC_RE);
    MHal_EMAC_Write_CTL(uRegVal);
    // Disable PHY interrupt //
    MHal_EMAC_disable_phyirq ();

    //Disable MAC interrupts //
    uRegVal = EMAC_INT_RCOM | EMAC_INT_RBNA
    | EMAC_INT_TUND | EMAC_INT_RTRY | EMAC_INT_TCOM
    | EMAC_INT_ROVR | EMAC_INT_HRESP;
    MHal_EMAC_Write_IDR(uRegVal);
    netif_stop_queue (nic);
}

void MDev_EMAC_stats(void)
{
   printf("Rx Ok:%u\n", MHal_EMAC_Read_OK());       /* Good frames received */
   printf("ALE:%u\n", MHal_EMAC_Read_ALE());
   printf("ELR:%u\n", MHal_EMAC_Read_ELR());        /* Excessive Length or Undersize Frame error */
   printf("SEQE:%u\n", MHal_EMAC_Read_SEQE());      /* Excessive Length or Undersize Frame error */
   printf("ROVR:%u\n", MHal_EMAC_Read_ROVR());      //rx fifo error
    printf("SE:%u\n", MHal_EMAC_Read_SE());
    printf("RJB:%u\n", MHal_EMAC_Read_RJB());

    printf("Tx Ok:%u\n", MHal_EMAC_Read_FRA());
    printf("TUE:%u\n", MHal_EMAC_Read_TUE());        /* Transmit FIFO underruns */
    printf("CSE:%u\n", MHal_EMAC_Read_CSE());      /* Carrier Sense errors */
    printf("SQEE:%u\n", MHal_EMAC_Read_SQEE());      /* Heartbeat error */
 }

void MDrv_EMAC_PhyAddrScan(void)
{
    U32 word_ETH_MAN  = 0x00000000;

    ThisUVE.flagISR_INT_DONE = 0x00;
    MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &word_ETH_MAN);
    if((0xffff != word_ETH_MAN)&&(0x0000 != word_ETH_MAN))
    {
        printf("phy [%d]=%x\n",phy_id, word_ETH_MAN);
        return;
    }

    for (phy_id = 0; phy_id < 32; phy_id++)
    {
        MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &word_ETH_MAN);
        printf("phy [%d]=%x\n",phy_id, word_ETH_MAN);
        if((0xffff != word_ETH_MAN)&&(0x0000 != word_ETH_MAN))
        {
            return;
        }
    }
    printf("Can't get correct PHY Addr and reset to 0\n");
    phy_id = 0;
}

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
static int MDrv_EMAC_HW_init(struct eth_device *nic)
{
    u32 word_ETH_CTL = 0x00000000;
    u32 word_ETH_CFG = 0x00000800;
    u32 uJulian104Value=0;
    s32 uNegPhyVal = 0;

    // (20071026_CHARLES) Disable TX, RX and MDIO:   (If RX still enabled, the RX buffer will be overwrited)
    MHal_EMAC_Write_CTL(word_ETH_CTL);
    // Init RX --------------------------------------------------------------
    memset((u8*)(RAM_ADDR_BASE | RX_BUFFER_BASE),0x00,RX_BUFFER_SIZE);

    MHal_EMAC_Write_BUFF(RX_BUFFER_BASE|RX_BUFFER_SEL);
    MHal_EMAC_Write_RDPTR(0x00000000);
    MHal_EMAC_Write_WRPTR(0x00000000);
    // Initialize "Receive Buffer Queue Pointer"
    MHal_EMAC_Write_RBQP(RBQP_BASE);

    // Enable Interrupts ----------------------------------------------------
    MHal_EMAC_Write_IER(0x0000FFFF);
    printf("MAC Address %02X:%02X:%02X:%02X:%02X:%02X\n",ThisBCE.sa1[0],ThisBCE.sa1[1],
    ThisBCE.sa1[2],ThisBCE.sa1[3],ThisBCE.sa1[4],ThisBCE.sa1[5]);
    // Set MAC address ------------------------------------------------------
    MHal_EMAC_Write_SA1_MAC_Address(ThisBCE.sa1[0],ThisBCE.sa1[1],ThisBCE.sa1[2],ThisBCE.sa1[3],ThisBCE.sa1[4],ThisBCE.sa1[5]);
    MHal_EMAC_Write_SA2_MAC_Address(ThisBCE.sa2[0],ThisBCE.sa2[1],ThisBCE.sa2[2],ThisBCE.sa2[3],ThisBCE.sa2[4],ThisBCE.sa2[5]);
    MHal_EMAC_Write_SA3_MAC_Address(ThisBCE.sa3[0],ThisBCE.sa3[1],ThisBCE.sa3[2],ThisBCE.sa3[3],ThisBCE.sa3[4],ThisBCE.sa3[5]);
    MHal_EMAC_Write_SA4_MAC_Address(ThisBCE.sa4[0],ThisBCE.sa4[1],ThisBCE.sa4[2],ThisBCE.sa4[3],ThisBCE.sa4[4],ThisBCE.sa4[5]);

#ifdef SOFTWARE_DESCRIPTOR
    #ifdef CHECKSUM
        uJulian104Value=uJulian104Value|(CHECKSUM_ENABLE|SOFTWARE_DESCRIPTOR_ENABLE);
    #else
        uJulian104Value=uJulian104Value|SOFTWARE_DESCRIPTOR_ENABLE;
    #endif
        MHal_EMAC_Write_JULIAN_0104(uJulian104Value);//Disable interrupt delay
#else

        uJulian104Value=0x00000000;
        MHal_EMAC_Write_JULIAN_0104(uJulian104Value);

#endif //#ifdef SOFTWARE_DESCRIPTOR

    // IMPORTANT: Run NegotiationPHY() before writing REG_ETH_CFG.
    ThisBCE.duplex = 1;
    ThisBCE.speed = SPEED_10;
    uNegPhyVal = MHal_EMAC_NegotiationPHY(phy_id);

    ThisUVE.flagMacTxPermit = 0x01;
    switch(uNegPhyVal)
    {
    case 1:
        ThisBCE.duplex = 1;
        ThisBCE.speed = SPEED_10;
        break;

    case 2:
        ThisBCE.duplex = 2;
        ThisBCE.speed = SPEED_10;
        word_ETH_CFG |= (EMAC_FD);
        break;

    case 3:
        ThisBCE.duplex = 1;
        ThisBCE.speed = SPEED_100;
        word_ETH_CFG |= (EMAC_SPD);
        break;

    case 4:
        word_ETH_CFG |= (EMAC_SPD|EMAC_FD);
        break;

    default:
        printf("Status Error!\n");
    }

#if (SUPPORT_ALBANY)
    if (ThisBCE.speed == SPEED_100)
        albany_write_reg(0x0b7b, 0x08a6);
#endif

	printf("word_ETH_CFG %x", word_ETH_CFG);
    MHal_EMAC_Write_CFG(word_ETH_CFG);
    // ETH_CTL Register -----------------------------------------------------
    word_ETH_CTL = 0x0000010;  // Enable MDIO
    if(ThisBCE.wes == 1) word_ETH_CTL |= 0x00000080;
    MHal_EMAC_Write_CTL(word_ETH_CTL);

    #if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS)
        MHal_EMAC_Write_JULIAN_0100(0x0000F007);
    #elif defined(CONFIG_KAISERIN)
        MHal_EMAC_Write_JULIAN_0100(0x0000F001);
    #else
    #endif
    ThisUVE.flagPowerOn = 1;
    ThisUVE.initedEMAC  = 1;

    return 1;
}

//-------------------------------------------------------------------------------------------------
// EMAC init Variable
//-------------------------------------------------------------------------------------------------
static volatile U8 emac_var[EMAC_ABSO_MEM_SIZE];
u32 MDrv_EMAC_VarInit(void)
{
    u32 alloRAM_ADDR_BASE;
    int i;
    char *s, *e;

    alloRAM_ADDR_BASE = (u32)emac_var;
    flush_cache((ulong)alloRAM_ADDR_BASE ,EMAC_ABSO_MEM_SIZE);
    memset((u32 *)alloRAM_ADDR_BASE,0x00,EMAC_ABSO_MEM_SIZE);
    EMAC_MEM_BASE = (alloRAM_ADDR_BASE + 0x3FFF) & ~0x3FFF;   // IMPORTANT: Let lowest 14 bits as zero.
    printf("EMAC Mem Base:%lx\n", EMAC_MEM_BASE);

    RX_BUFFER_BASE   = EMAC_MEM_BASE & (~RAM_ADDR_BASE);
    RBQP_BASE        = RX_BUFFER_BASE + RX_BUFFER_SIZE;
    if ((RBQP_BASE + RBQP_SIZE+ EMAC_MEM_GAP) > (alloRAM_ADDR_BASE + EMAC_ABSO_MEM_SIZE))
    {
        printf("MMAP overflow: %lx > %lx\n", (RBQP_BASE + RBQP_SIZE+ EMAC_MEM_GAP), (alloRAM_ADDR_BASE + EMAC_ABSO_MEM_SIZE));
        return 0;
    }

    printf("Rx buff:%lx\nRBQP:%lx\n", RX_BUFFER_BASE, RBQP_BASE);
    memset(&ThisBCE,0x00,sizeof(BasicConfigEMAC));
    memset(&ThisUVE,0x00,sizeof(UtilityVarsEMAC));

    ThisBCE.wes         = 0;					// 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    ThisBCE.duplex      = 2;					// 1:Half-duplex, 2:Full-duplex
    ThisBCE.cam         = 0;                 // 0:No CAM, 1:Yes
    ThisBCE.rcv_bcast   = 0;      			// 0:No, 1:Yes
    ThisBCE.rlf         = 0;                 // 0:No, 1:Yes receive long frame(1522)
    ThisBCE.rcv_bcast   = 1;
    ThisBCE.speed       = EMAC_SPEED_100;

    s = getenv ("ethaddr");
    if (s)
    {
        for (i = 0; i < 6; ++i)
        {
            ThisBCE.sa1[i] = s ? simple_strtoul (s, &e, 16) : 0;
            if (s)
            {
                s = (*e) ? e + 1 : e;
            }
        }
    }
    else
    {
        ThisBCE.sa1[0]      = MY_MAC[0];
        ThisBCE.sa1[1]      = MY_MAC[1];
        ThisBCE.sa1[2]      = MY_MAC[2];
        ThisBCE.sa1[3]      = MY_MAC[3];
        ThisBCE.sa1[4]      = MY_MAC[4];
        ThisBCE.sa1[5]      = MY_MAC[5];
    }

    ThisBCE.loopback = 0;
    if (!((u32)packet_data% 4))
        packet +=2;
    printf("Packet buffer:%lx\n", packet);

    return alloRAM_ADDR_BASE;
}

/**************************************************************************
PROBE - Look for an adapter, this routine's visible to the outside
You should omit the last argument struct pci_device * for a non-PCI NIC
***************************************************************************/
struct emac_hw hw_device;
int emac_init=0;

void MDrv_EMAC_DumpMem(u32 addr, u32 len)
{
    u8 *ptr = (u8 *)addr;
    u32 i;

    printf("\n ===== Dump %lx =====\n", ptr);
    for (i=0; i<len;i++, ptr++)
    {
        if ((u32)i%0x10 ==0)
            printf("%lx: ", ptr);
        if (*ptr < 0x10)
            printf("0%x ", *ptr);
        else
            printf("%x ", *ptr);
        if ((u32)i%0x10 == 0x0f)
            printf("\n");
    }
    printf("\n");
}

void MDrv_EMAC_DumpReg(void)
{
    u32 idx, val = 0;

    printf("======= PHY Reg =======\n");
    for (idx = 0; idx < 0x20; idx++)
    {
        MHal_EMAC_read_phy(phy_id, idx, &val);
        printf("  Reg-%-3X=%-4x   ",idx, val);
        if (idx%5 == 4)
            printf("\n");
    }

    printf("\n======= EMAC Reg =======\n");
    for (idx = 0; idx < 0x120; idx+=4)
    {
        printf("  Reg-%-3X=%-8x   ",idx, MHal_EMAC_ReadReg32(idx));
        if (idx%0x10 == 0x0c)
            printf("\n");
    }
}

u8 MDrv_EMAC_Bist(u8 mode)
{
    u16 timeout = 0;
    u32 status = 0;

    ThisBCE.loopback = mode;
    switch(mode)
    {
    case PHY_LOOPBACK:
        MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, 0x6100);
        do
        {
            MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &status);
            timeout++;
        }while(!(status & 0x0004) && (timeout < 25000));
        if (timeout == 50000)
        {
            printf("Open PHY loopback mode timeout!!\n");
            MHal_EMAC_NegotiationPHY(phy_id);
            return FALSE;
        }
        else
            printf("Open PHY loopback mode!\n");
        break;

    case MAC_LOOPBACK:
        MDrv_EMAC_open(&nic_device, NULL);
        printf("Long loopback test\n");
        for (;;)
        {
           MDrv_EMAC_rx(&nic_device);
        }
        break;

    default:
        MHal_EMAC_NegotiationPHY(phy_id);
        printf("EMAC normal mode!\n");
        break;
    }

    return TRUE;
}

#if (SUPPORT_ALBANY)
u32 albany_read_reg(u32 reg)
{
    u32 val;

    MHal_EMAC_write_phy(phy_id, 0x1d, reg);
    MHal_EMAC_read_phy(phy_id, 0x1f, &val);
    return val;
}

void albany_write_reg(u32 reg, u32 val)
{
    MHal_EMAC_write_phy(phy_id, 0x1c, reg);
    MHal_EMAC_write_phy(phy_id, 0x1e, val);
}

void albany_patch(void)
{
    u32 mpe;
    u32 regval;

    mpe = MHal_EMAC_Read_CTL();
    MHal_EMAC_Write_CTL(mpe | 0x10);
    MHal_EMAC_Albany_I2C(TRUE);

    /*
     * bank B, Reg39, Bit14: Fix Rx Alignment issue
     * bank B, Reg39, Bit02: Disable SQE Test
     * bank B, Reg39, Bit04: Disable Jabber function
     * bank B, Reg20, Bit13: 100m detection mode (switch_rxclk_timer)
     */
    regval = albany_read_reg(0x0b39);
    regval |= BIT(14);
    regval &= ~(BIT(2) | BIT(14));
    albany_write_reg(0x0b39, regval);

    regval = albany_read_reg(0x0b20);
    regval |= BIT(13);
    albany_write_reg(0x0b20, regval);

    /*
     * bank C, Reg5B, Bit5:XTAL_EN setting
     */
    regval = albany_read_reg(0x0c5b);
    regval |= BIT(5);
    albany_write_reg(0x0c5b, regval);

    /*
     *  For 24 MHz clock setting
     */
    // div set to 25
    albany_write_reg(0x0b65, 0x1000);
    albany_write_reg(0x0c62, 0x0019);
    // circuit issue , need MPL_TEST[5] =1
    albany_write_reg(0x0c61, 0x0020);
    albany_write_reg(0x0c62, 0x0219);

    albany_write_reg(0x0c60, 0x80c4);
    albany_write_reg(0x0c65, 0x9184);
    // hkr force
    albany_write_reg(0x0c1F, 0x8030);
    albany_write_reg(0x0c1E, 0);
    //checking
    // force hkr load
    albany_write_reg(0x0c1D, 0x0140);
    // an timer
    // flp test  max/min
    albany_write_reg(0x0b23, 0x8259);
    // nlp test  max/min
    albany_write_reg(0x0b24, 0x918c);
    // data detect  max/min
    albany_write_reg(0x0b25, 0x855c);
    // transmit link burst
    albany_write_reg(0x0b26, 0xbb51);
    // break link
    albany_write_reg(0x0b27, 0x897a);
    // link fail inhibit
    albany_write_reg(0x0b28, 0x8c9f);
    // nlp link test max
    albany_write_reg(0x0b29, 0x9191);
    // pll lock check timer
    albany_write_reg(0x0c70, 0x855);
    albany_write_reg(0x0c71, 0xd46a);
    albany_write_reg(0x0c72, 0xa6a);
    albany_write_reg(0x0c73, 0xff85);
    // mii_rx_CLK_POL
    albany_write_reg(0x0b39, 0x4498);
#if 0
    // for 140m long cable test
    albany_write_reg(0x0c5a, 0x5156);

    /*
     *  2.5V LDO
     */
    albany_write_reg(0x0c5f, 0x5000);
#endif

    /*
     *  Combine 100T/10T Act LED
     */
    albany_write_reg(0x0b7b, 0x08a6);

    albany_write_reg(0x0c1f, 0x0030);

    MHal_EMAC_Write_CTL(mpe);
    MHal_EMAC_Albany_I2C(FALSE);
}
#endif

void MDrv_EMAC_PatchPHY(void)
{
    u32 val = 0;
    MHal_EMAC_read_phy(phy_id, 2, &val);
    if (RTK_OUI_MSB == val)
    {
        MHal_EMAC_read_phy(phy_id, 25, &val);
        MHal_EMAC_write_phy(phy_id, 25, 0x400);
        MHal_EMAC_read_phy(phy_id, 25, &val);
    }
#if (SUPPORT_ALBANY)
    albany_patch();
#endif
}

int MDrv_EMAC_initialize(bd_t * bis)
{
    struct eth_device *nic = NULL;
    struct emac_hw *hw = NULL;
    char* phy_id_string=NULL;

    if ((phy_id_string = getenv ("PHY_ID")) != NULL)
    {

        phy_id =phy_id_string ? (int)simple_strtol(phy_id_string, NULL, 10) : 0;
    }

    if(emac_init)
    {
        MDrv_EMAC_PhyAddrScan();
#if (EMAC_DBG_TFTP)
        rxpre_seq = txpre_seq = 0;
#endif
        MDrv_EMAC_HW_init(nic);
        return -1;
    }

    if (!MDrv_EMAC_VarInit())
        return -1;
    nic=&nic_device;
    hw=&hw_device;

    hw->dlist_phys = hw->dlist = (struct recv_desc_bufs *) (RX_BUFFER_BASE|RAM_ADDR_BASE);

    MHal_EMAC_Power_On_Clk();
	MDrv_EMAC_PhyAddrScan();

    MDrv_EMAC_PatchPHY();
    nic->priv = hw;
    MDrv_EMAC_HW_init(nic);

    /* EMAC HW init */
    nic->init = MDrv_EMAC_open;
    nic->recv = MDrv_EMAC_rx;
    nic->send = MDrv_EMAC_tx;
    nic->halt = MDrv_EMAC_close;

    memcpy(nic->enetaddr, MY_MAC, sizeof(MY_MAC));
    eth_register(nic);
    emac_init=1;
    return 0;

    //test start
}


