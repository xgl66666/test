/**
 * @file
 * EMAC Driver for lwIP
 *
 */
#ifndef __EMAC_NETIF_C__
#define __EMAC_NETIF_C__

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "arch/emac_netif.h"
#include <linux/mii.h>
#include <mhal_emac.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

static void
emac_netif_input(struct pbuf * p, struct netif *netif);


/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */

struct emac_rx_memory * emac_rx_mem;
struct emac_rx_memory __emac_rx_mem__ __attribute__ ((aligned (16384))) = {{0}};
//struct emac_rx_descriptor __attribute__ ((aligned (4096))) desc[RBQP_LENG];
//struct emac_rx_packet_buffer __attribute__ ((aligned (16384))) pkt_buf[RBQP_LENG];
struct pbuf * emac_tx_mem = NULL;



struct netif emac_netif = {0};
struct emac_private emac_priv = {0};

/**************************************************************************
POLL - Wait for a frame
***************************************************************************/
/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */

static int emac_netif_rx(void)
{
//    MS_U32 u32Wrptr = MHal_EMAC_Read_WRPTR();
    unsigned char *p_recv;
    struct pbuf *p, *q;

    u32_t pktlen;
    u32_t uRegVal=0;
    u32_t wrap_bit;

#if 1
    MS_U32 u32Rsr = MHal_EMAC_Read_RSR();
    MS_U32 u32Rdptr = MHal_EMAC_Read_RDPTR();
    MS_U32 u32Rbqp = MHal_EMAC_Read_RBQP();

    LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s BufInd %d RSR %08x RDPTR %08x RBQP %08x\n", __LINE__, __FUNCTION__, emac_priv.u32rxBuffIndex, u32Rsr, u32Rdptr, u32Rbqp));
#else
    LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s BufInd %d \n", __LINE__, __FUNCTION__, emac_priv.u32rxBuffIndex));
#endif
//    dlist = hw->dlist ;
    // If any Ownership bit is 1, frame received.

    flush_cache((ulong)&emac_rx_mem->desc ,sizeof(emac_rx_mem->desc));
#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    Chip_Flush_Memory();
    //flush read buffer data
    ((volatile u32 *)0x80001000)[0];
#endif


#if 1
/*
    for(pktlen = 0; pktlen < RBQP_LENG; pktlen++)
    {
//        LWIP_DEBUGF(NETIF_DEBUG, ("%08x %08x ", emac_rx_mem->desc[pktlen], emac_rx_mem->desc[pktlen].word1));
        LWIP_DEBUGF(NETIF_DEBUG, ("%01x %01x %08x %08x\n", emac_rx_mem->desc[pktlen].wrap, emac_rx_mem->desc[pktlen].ownership, emac_rx_mem->desc[pktlen].pkt_addr, emac_rx_mem->desc[pktlen].pkt_leng));
    }
*/






//    while ( (dlist->descriptors[hw->rxBuffIndex].addr )& EMAC_DESC_DONE)

    while(emac_rx_mem->desc[emac_priv.u32rxBuffIndex].ownership)
    {
//        p_recv = (unsigned char *) ((((dlist->descriptors[hw->rxBuffIndex].addr)&0x0FFFFFFF) |RAM_ADDR_BASE) &~(EMAC_DESC_DONE | EMAC_DESC_WRAP));
//        pktlen = (u32)dlist->descriptors[hw->rxBuffIndex].size & 0x7ff;    /* Length of frame including FCS */
        p_recv = (emac_rx_mem->desc[emac_priv.u32rxBuffIndex].pkt_addr << 2) | RAM_ADDR_BASE ;
        pktlen = emac_rx_mem->desc[emac_priv.u32rxBuffIndex].pkt_leng;
        flush_cache((ulong)p_recv,pktlen);
#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
        Chip_Flush_Memory();
#endif
//        dump_pkt(p_recv, pktlen);



#if 0
        if ((unsigned char *) emac_rx_mem->pkt_buf + sizeof(emac_rx_mem->pkt_buf) >
        (p_recv + roundup(pktlen, 4)))
        {
            // the frame is not splitted in two parts //
//            memcpy(u8PktBuf, p_recv, pktlen);
            // update consumer pointer//
            uRegVal = MHal_EMAC_Read_RDPTR();
//            uRegVal += ROUND_SUP_4(pktlen);
            uRegVal += roundup(pktlen, 4);
            MHal_EMAC_Write_RDPTR(uRegVal);
        }
        else
        {
            //the frame is splitted in two parts //
            int firstPacketPartSize =
            (unsigned char *) emac_rx_mem->pkt_buf + sizeof(emac_rx_mem->pkt_buf) - p_recv;
            int secondPacketSize =(roundup(pktlen, 4) - firstPacketPartSize);
//            memcpy((unsigned char *)u8PktBuf, p_recv, firstPacketPartSize);
//            memcpy((unsigned char *)(u8PktBuf+firstPacketPartSize), (unsigned char *) emac_rx_mem->pkt_buf[0].buf, secondPacketSize);

            // update consumer pointer and toggle the wrap bit //
            wrap_bit = (MHal_EMAC_Read_RDPTR() & EMAC_WRAP_R) ^ EMAC_WRAP_R;
            MHal_EMAC_Write_RDPTR(secondPacketSize | wrap_bit);
        }
#endif

//        flush_cache((ulong)u8PktBuf,pktlen);
#if defined(CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12)|| defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
        Chip_Flush_Memory();
#endif

//        NetReceive((uchar *)u8PktBuf, pktlen);

        /* We allocate a pbuf chain of pbufs from the pool. */
        p = pbuf_alloc(PBUF_RAW, pktlen, PBUF_POOL);

        if (p != NULL) {

        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
//        for(q = p; q != NULL; q = q->next) {
            /* Read enough bytes to fill this pbuf in the chain. The
             * available data in the pbuf is given by the q->len
             * variable.
             * This does not necessarily have to be a memcpy, you can also preallocate
             * pbufs for a DMA-enabled MAC and after receiving truncate it to the
             * actually received size. In this case, ensure the tot_len member of the
             * pbuf is the sum of the chained pbuf len members.
             */
        //      read data into(q->payload, q->len);
//            memcpy(p->payload, p_recv, pktlen);
            pbuf_take(p, p_recv, pktlen);
//        }

            emac_netif_input(p, &emac_netif);

//    acknowledge that packet has been read();

            LINK_STATS_INC(link.recv);
        } else {
            LINK_STATS_INC(link.memerr);
            LINK_STATS_INC(link.drop);
        }

        emac_rx_mem->desc[emac_priv.u32rxBuffIndex].ownership = FALSE;
        //wrap after last buffer //
        emac_priv.u32rxBuffIndex++;
        if (emac_priv.u32rxBuffIndex == MAX_RX_DESCR)
        {
            emac_priv.u32rxBuffIndex = 0;
        }

    }
    return 1;
#endif
}

void emac_netif_isr(MHAL_SavedRegisters *pHalReg, U32 u32Data)
{
//    LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s\n", __LINE__, __FUNCTION__));

    u32_t u32IntStatus;

    if(u32IntStatus = (MHal_EMAC_Read_ISR() & ~MHal_EMAC_Read_IMR()))
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s IntStatus %08x\n", __LINE__, __FUNCTION__, u32IntStatus));
        if (u32IntStatus & EMAC_INT_RBNA)
        {
            //            LocPtr->stats.rx_dropped ++;
            LINK_STATS_INC(link.drop);
            //            ThisUVE.flagRBNA = 1;
            //            xReceiveFlag = 1;
            //write 1 clear
            MHal_EMAC_Write_RSR(EMAC_BNA);
        }

        if(u32IntStatus & EMAC_INT_TCOM)
        {

            LINK_STATS_INC(link.xmit);
            LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s EMAC_INT_TCOM\n", __LINE__, __FUNCTION__));
            if(u32IntStatus & EMAC_INT_TUND)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s EMAC_INT_TUND\n", __LINE__, __FUNCTION__));

                //write 1 clear
                MHal_EMAC_Write_TSR(EMAC_UND);
            }

            if(u32IntStatus &EMAC_INT_RTRY)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s EMAC_INT_RTRY\n", __LINE__, __FUNCTION__));
            }

            if (u32IntStatus & EMAC_INT_TBRE)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s EMAC_INT_TBRE\n", __LINE__, __FUNCTION__));
            }

        }

        if(u32IntStatus & EMAC_INT_RCOM)
        {
            MHal_EMAC_Write_RSR(EMAC_REC);
            emac_netif_rx();

        }

        if(u32IntStatus&EMAC_INT_DONE)
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s EMAC_INT_DONE\n", __LINE__, __FUNCTION__));
        }

        //Overrun //
        if(u32IntStatus & EMAC_INT_ROVR)
        {
            LINK_STATS_INC(link.drop);
            //write 1 clear
            MHal_EMAC_Write_RSR(EMAC_RSROVR);
            LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s EMAC_INT_ROVR\n", __LINE__, __FUNCTION__));
        }

    }
    MsOS_EnableInterrupt(E_INT_IRQ_EMAC);
    return;
}

/**************************************************************************
TRANSMIT - Transmit a frame
***************************************************************************/
// RT_ALIGNMENT_SIZE 4 => pbuf->payload alignment = 4
static err_t emac_netif_tx(struct netif * netif, struct pbuf * p)
{
    LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s\n", __LINE__, __FUNCTION__));
    volatile MS_U32 check;
    MS_U32 tsrval;
    for (check = 0; check < 5000; check++)
    {
        tsrval = MHal_EMAC_Read_TSR();
        if ((tsrval & EMAC_IDLETSR) && (tsrval & EMAC_BNQ))
        break;
    }
    if(check >= 5000)
    {
        return ERR_IF;
    }

    if(emac_tx_mem!= NULL) {
        if (pbuf_copy(emac_tx_mem, p) != ERR_OK) {
            return ERR_MEM;
        }
    }


    flush_cache((ulong)emac_tx_mem->payload, p->tot_len);

#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    Chip_Flush_Memory();
#endif

    MHal_EMAC_Write_TAR((u32)emac_tx_mem->payload);

    // Set length of the packet in the Transmit Control register //
    MHal_EMAC_Write_TCR(p->tot_len);

    return ERR_OK;
}

static int emac_netif_hw_init(struct netif * netif)
{
    struct emac_private * priv = (struct emac_private *) netif->state;
    u32 word_ETH_CTL = 0x00000000;
    u32 word_ETH_CFG = 0x00000800;
    u32 uJulian104Value=0;
    s32 uNegPhyVal = 0;
    u32_t RX_BUFFER_BASE  = ((u32_t)&emac_rx_mem->pkt_buf) & (~RAM_ADDR_BASE);
    u32_t RBQP_BASE        = ((u32_t)&emac_rx_mem->desc) & (~RAM_ADDR_BASE);

    LWIP_DEBUGF(NETIF_DEBUG, ("RX_BUFFER_BASE %08x RBQP_BASE %08x\n", RX_BUFFER_BASE, RBQP_BASE));

    // (20071026_CHARLES) Disable TX, RX and MDIO:   (If RX still enabled, the RX buffer will be overwrited)
//    MHal_EMAC_Write_CTL(word_ETH_CTL);
    // Init RX --------------------------------------------------------------
//    memset((u8*)(RAM_ADDR_BASE | RX_BUFFER_BASE),0x00,RX_BUFFER_SIZE);
//    memset(&emac_rx_mem, 0, sizeof(struct emac_rx_memory));
    memset(emac_rx_mem->desc, 0, sizeof(emac_rx_mem->desc));
    memset(emac_rx_mem->pkt_buf, 0, sizeof(emac_rx_mem->pkt_buf));

    MHal_EMAC_Write_BUFF( RX_BUFFER_BASE | RX_BUFFER_SEL);
    MHal_EMAC_Write_RDPTR(0x00000000);
    MHal_EMAC_Write_WRPTR(0x00000000);
    // Initialize "Receive Buffer Queue Pointer"
    MHal_EMAC_Write_RBQP(RBQP_BASE);

    // Enable Interrupts ----------------------------------------------------

    // Set MAC address ------------------------------------------------------
    MHal_EMAC_Write_SA1_MAC_Address(netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);

#ifdef SOFTWARE_DESCRIPTOR
    #ifdef CHECKSUM
        uJulian104Value=uJulian104Value|(CHECKSUM_ENABLE|SOFTWARE_DESCRIPTOR_ENABLE);
    #else
        uJulian104Value=uJulian104Value|SOFTWARE_DESCRIPTOR_ENABLE;
    #endif
        MHal_EMAC_Write_JULIAN_0104(uJulian104Value);//Disable interrupt delay
#else
    #if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
        MHal_EMAC_Write_JULIAN_0100(0x0000F007);
        MHal_EMAC_Write_JULIAN_0104(0x00);
    #else
        uJulian104Value=0x00000000;
        MHal_EMAC_Write_JULIAN_0104(uJulian104Value);
    #endif
#endif //#ifdef SOFTWARE_DESCRIPTOR

    // IMPORTANT: Run NegotiationPHY() before writing REG_ETH_CFG.
//    ThisBCE.duplex = 1;
//    ThisBCE.speed = SPEED_10;
    uNegPhyVal = MHal_EMAC_NegotiationPHY(priv->phy_id);

//    ThisUVE.flagMacTxPermit = 0x01;
    switch(uNegPhyVal)
    {
    case 1:
//        ThisBCE.duplex = 1;
//        ThisBCE.speed = SPEED_10;
        break;

    case 2:
//        ThisBCE.duplex = 2;
//        ThisBCE.speed = SPEED_10;
        word_ETH_CFG |= (EMAC_FD);
        break;

    case 3:
//        ThisBCE.duplex = 1;
//        ThisBCE.speed = SPEED_100;
        word_ETH_CFG |= (EMAC_SPD);
        break;

    case 4:
        word_ETH_CFG |= (EMAC_SPD|EMAC_FD);
        break;

    default:
        LWIP_DEBUGF(NETIF_DEBUG, ("PHY Status Error!\n"));
    }

#if (SUPPORT_ALBANY)
//    if (ThisBCE.speed == SPEED_100)
    if(uNegPhyVal == 3 || uNegPhyVal == 4)
        albany_write_reg(0x0b7b, 0x08a6);
#endif

	LWIP_DEBUGF(NETIF_DEBUG, ("word_ETH_CFG %x", word_ETH_CFG));
    MHal_EMAC_Write_CFG(word_ETH_CFG);
    // ETH_CTL Register -----------------------------------------------------
    word_ETH_CTL = 0x0000010;  // Enable MDIO
//    if(ThisBCE.wes == 1) word_ETH_CTL |= 0x00000080;
    MHal_EMAC_Write_CTL(word_ETH_CTL);

#ifdef SOFTWARE_DESCRIPTOR
#if (SUPPORT_ALBANY)
    MHal_EMAC_Write_JULIAN_0100(0x0000F001);    //MII
#else
    MHal_EMAC_Write_JULIAN_0100(0x0000F007);    //RMII
#endif
#else
    MHal_EMAC_Write_JULIAN_0100(0x00000107);
#endif

//    ThisUVE.flagPowerOn = 1;
//    ThisUVE.initedEMAC  = 1;

    return 1;
}


u32_t emac_netif_variable_init(struct netif * netif)
{
    struct emac_private * priv = (struct emac_private *) netif->state;
    u32 alloRAM_ADDR_BASE;
    int i;
    char *s, *e;

    s = getenv ("ethaddr");
    if (s)
    {
        for (i = 0; i < NETIF_MAX_HWADDR_LEN; ++i)
        {
            netif->hwaddr[i] = s ? simple_strtoul (s, &e, 16) : 0;
            if (s)
            {
                s = (*e) ? e + 1 : e;
            }
        }
    }

    return alloRAM_ADDR_BASE;
}


void emac_netif_phyidscan(struct netif * netif)
{
    struct emac_private * priv = (struct emac_private *)netif->state;
    u32_t word_ETH_MAN  = 0x00000000;

    priv->ThisUVE.flagISR_INT_DONE = 0x00;
    MHal_EMAC_read_phy(priv->phy_id, PHY_REG_STATUS, &word_ETH_MAN);
    if((0xffff != word_ETH_MAN)&&(0x0000 != word_ETH_MAN))
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("phy [%d]=%x\n", priv->phy_id, word_ETH_MAN));
        return;
    }

    for (priv->phy_id = 0; priv->phy_id < 32; priv->phy_id++)
    {
        MHal_EMAC_read_phy(priv->phy_id, PHY_REG_STATUS, &word_ETH_MAN);
        LWIP_DEBUGF(NETIF_DEBUG, ("phy [%d]=%x\n", priv->phy_id, word_ETH_MAN));
        if((0xffff != word_ETH_MAN)&&(0x0000 != word_ETH_MAN))
        {
            return;
        }
    }

    LWIP_DEBUGF(NETIF_DEBUG, ("Can't get correct PHY Addr and reset to 0\n"));
    priv->phy_id = 0;
    return;
}

void emac_netif_patch_phy(struct netif * netif)
{
    struct emac_private * priv = (struct emac_private *)netif->state;
    u8_t u8PhyId = priv->phy_id;
    u32 val = 0;

    MHal_EMAC_read_phy(u8PhyId, 2, &val);
    if (RTK_OUI_MSB == val)
    {
        MHal_EMAC_read_phy(u8PhyId, 25, &val);
        MHal_EMAC_write_phy(u8PhyId, 25, 0x400);
        MHal_EMAC_read_phy(u8PhyId, 25, &val);
    }
#if (SUPPORT_ALBANY)
    albany_patch();
#endif
}

//-------------------------------------------------------------------------------------------------
//Initialize and start the Receiver and Transmit subsystems
//-------------------------------------------------------------------------------------------------
static void emac_netif_start (struct netif * netif)
{
//    struct emac_hw *hw = (struct emac_hw *) nic->priv;
    struct emac_private * priv = (struct emac_private *) netif->state;
    u32 uRegVal;
#ifdef SOFTWARE_DESCRIPTOR
    u32 i;
#endif
    u32_t RX_BUFFER_BASE  = ((u32_t)emac_rx_mem->pkt_buf) & (~RAM_ADDR_BASE);
    u32_t RBQP_BASE        = ((u32_t)emac_rx_mem->desc) & (~RAM_ADDR_BASE);

#if 0
    // Initialize Receive Buffer Descriptors
    memset((u8*)(RBQP_BASE | RAM_ADDR_BASE), 0x00, RBQP_SIZE);

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
#else
    for(i = 0; i < RBQP_LENG; i++)
    {
        emac_rx_mem->desc[i].pkt_addr = ((u32_t)(emac_rx_mem->pkt_buf[i].buf) & ~RAM_ADDR_BASE) >> 2;

//        LWIP_DEBUGF(NETIF_DEBUG, ("%08x %08x %08x %08x\n", emac_rx_mem->desc[i].pkt_addr, (&emac_rx_mem->pkt_buf[i].buf), emac_rx_mem->desc[i], emac_rx_mem->desc[i].word1));

        if( i == RBQP_LENG - 1)
        {
            emac_rx_mem->desc[i].wrap = TRUE;
        }

    }
//    dump_pkt(emac_rx_mem->desc, sizeof(emac_rx_mem->desc));
#endif

#if (CONFIG_URANUS4) || (CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || (CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
    Chip_Flush_Memory();
#endif

#if 0
    // set offset of read and write pointers in the receive circular buffer //
    uRegVal = MHal_EMAC_Read_BUFF();
    uRegVal = (RX_BUFFER_BASE|RX_BUFFER_SEL);
    MHal_EMAC_Write_BUFF(uRegVal);
    MHal_EMAC_Write_RDPTR(0);
    MHal_EMAC_Write_WRPTR(0);
    // Program address of descriptor list in Rx Buffer Queue register //
    MHal_EMAC_Write_RBQP(RBQP_BASE);
#endif

    //Reset buffer index//
//    hw->rxBuffIndex = 0;

    // Enable Receive and Transmit //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= (EMAC_RE | EMAC_TE);
    MHal_EMAC_Write_CTL(uRegVal);
}


static int emac_netif_device_open (struct netif * netif)
{
    printf("%04d %s\n", __LINE__, __FUNCTION__);
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
    LWIP_DEBUGF(NETIF_DEBUG, ("uRegVal %08x IER %08x IMR %08x IDR %08x\n", uRegVal, MHal_EMAC_Read_IER(), MHal_EMAC_Read_IMR(), MHal_EMAC_Read_IDR()));

    emac_netif_start (netif);
    return 0;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void
emac_netif_input(struct pbuf * p, struct netif *netif)
{
//  struct ethernetif *ethernetif;
    struct eth_hdr *ethhdr;

    //  ethernetif = netif->state;

    /* move received packet into a new pbuf */
    //  p = low_level_input(netif);
    /* no packet could be read, silently ignore this */
    if (p == NULL) return;
    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch(htons(ethhdr->type)) {
        /* IP packet? */
        case ETHTYPE_IP:
        case ETHTYPE_ARP:

            /* pass to network layer */
            netif->input(p, netif);
            break;
        default:
            pbuf_free(p);
            p = NULL;
            break;
    }
}


/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t emac_netif_low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf * q = p;
    err_t ret;
#if 0
    LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s ========================================\n", __LINE__, __FUNCTION__));
    for(q = p; q ;  q = q->next)
    {
//        dump_pkt(q->payload, q->len);
    }
    LWIP_DEBUGF(NETIF_DEBUG, ("%04d %s ========================================\n", __LINE__, __FUNCTION__));
#endif
    emac_netif_tx(netif, p);

#if 0
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *q;

    initiate transfer();

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
    time. The size of the data in each pbuf is kept in the ->len
    variable. */
    send data from(q->payload, q->len);
    }

    signal that packet should be sent();

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
#endif


    return ret;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */

static void emac_netif_low_level_init(struct netif *netif)
{
    MS_BOOL bStatus = FALSE;
    MHal_EMAC_Power_On_Clk();

    emac_netif_phyidscan(netif);
    emac_netif_patch_phy(netif);
    emac_netif_variable_init(netif);
    emac_netif_hw_init(netif);
    emac_netif_device_open(netif);
    bStatus = MsOS_AttachInterrupt(E_INT_IRQ_EMAC, emac_netif_isr);
    LWIP_DEBUGF(NETIF_DEBUG, ("MsOS_AttachInterrupt = %d\n", bStatus));
    bStatus = MsOS_EnableInterrupt(E_INT_IRQ_EMAC);
    LWIP_DEBUGF(NETIF_DEBUG, ("MsOS_EnableInterrupt = %d\n", bStatus));


}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */

err_t emac_netif_init(struct netif *netif)
{

    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
    * Initialize the snmp variables and counters inside the struct netif.
    * The last argument should be replaced with your link speed, in units
    * of bits per second.
    */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 100000000);

    netif->state = (void *)&emac_priv;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    /* maximum transfer unit */
    netif->mtu = 1500;
    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
    /* set MAC hardware address */
    netif->hwaddr[0] = 0x00;
    netif->hwaddr[1] = 0x14;
    netif->hwaddr[2] = 0xb9;
    netif->hwaddr[3] = 0x60;
    netif->hwaddr[4] = 0x91;
    netif->hwaddr[5] = 0x01;

    netif->linkoutput = emac_netif_low_level_output;

    //  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
    emac_rx_mem = (unsigned int)&__emac_rx_mem__ | RAM_ADDR_BASE;
    emac_tx_mem = pbuf_alloc(PBUF_RAW, 0x600, PBUF_RAM);
    /* initialize the hardware */
    emac_netif_low_level_init(netif);





    return ERR_OK;

}
#endif /* __EMAC_NETIF_C__ */
