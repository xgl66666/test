/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    rbus_main_end.c

    Abstract:
    Create and register network interface for RBUS based chipsets in eCos platform.

    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
*/

#include "rt_config.h"

#define MAX_ETH_FRAME_SIZE 1520
#define PRIORITY_HIGH   0 /* the highest irq priorit */


//static cyg_interrupt rtmp_wlan_interrupt;
//static cyg_handle_t  rtmp_wlan_interrupt_handle;
static const char device_name[] = "ra0";
static int rt_ecos_init(struct cyg_netdevtab_entry *tab);


ETH_DRV_SC(devive_wireless_sc0,
           NULL,  /* Driver specific data */
           INF_MAIN_DEV_NAME "0",
           rt_ecos_start,
           rt_ecos_stop,
           rt_ecos_control,
           rt_ecos_can_send,
           rt_ecos_send,
           rt_ecos_recv,
           rt_ecos_deliver,
           rt_ecos_poll,
           rt_ecos_int_vector
           );
struct eth_drv_sc *sc_wifi = &devive_wireless_sc0;


NETDEVTAB_ENTRY(devive_wireless_netdev0,
                INF_MAIN_DEV_NAME "0",
                rt_ecos_init,
                &devive_wireless_sc0);

extern struct usb_driver rtusb_driver ;
static int rt_ecos_init(struct cyg_netdevtab_entry *tab)
{

	DBGPRINT(RT_DEBUG_OFF, ("===> rt_ecos_init()\n"));
 	PNET_DEV 			pNetDev = NULL;
	PRTMP_ADAPTER	    pAd = NULL;

	UINT8				MacAddr[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	/*NetDevInit============================================== */
	 diag_printf("===> rt_ecos_init()\n");

	pNetDev = (struct eth_drv_sc *)tab->device_instance;

	(pNetDev->funs->eth_drv->init)(pNetDev, MacAddr);
	return true;
	DBGPRINT(RT_DEBUG_OFF, ("<=== rt_ecos_init()\n"));
}


static void rt_ecos_start(struct eth_drv_sc *sc, unsigned char *enaddr, int flags)
{
        PNET_DEV		    pNetDev = NULL;
	POS_COOKIE			pOSCookie = NULL;
	PRTMP_ADAPTER	    pAd = NULL;
        char                devName[IFNAMSIZ];


		DBGPRINT(RT_DEBUG_ERROR, ("===> rt_ecos_start()\n"));
#if 0
	//MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_TRACE, ("===> rt_ecos_start()\n"));

        /* if the device is runnung, do nothing */
	if (sc->state & ETH_DRV_STATE_ACTIVE)
                goto exit;

	pNetDev = sc;
	pAd = (PRTMP_ADAPTER) RTMP_OS_NETDEV_GET_PRIV(sc);
	if (pAd == NULL) {
		DBGPRINT_ERR(("%s: pAd is NULL.\n", __FUNCTION__));
                goto exit;
        }

	pOSCookie = (POS_COOKIE) pAd->OS_Cookie;


        sprintf(devName, "%s0", INF_MAIN_DEV_NAME);
        if (strcmp(pNetDev->dev_name, devName) != 0)
        {
		DBGPRINT_ERR(("%s: device name(%s) is not found\n", __FUNCTION__, pNetDev->dev_name));
                goto exit;
        }


//	RTMP_DRIVER_AP_MAIN_OPEN(pAd);
	if (VIRTUAL_IF_UP(pAd) != 0)
		goto exit;

	/* increase MODULE use count */
	RT_MOD_INC_USE_COUNT();


	DBGPRINT(RT_DEBUG_INFO, ("<=== rt_ecos_start()\n"));
	return;

exit:
#endif
    return;
}

static void  rt_ecos_stop(struct eth_drv_sc *sc)
{
	INT BssId, i;
	PNET_DEV		pNetDev = NULL;
	PRTMP_ADAPTER   pAd = NULL;
	POS_COOKIE		pOSCookie = NULL;
	char devName[IFNAMSIZ];


	DBGPRINT(RT_DEBUG_ERROR, ("===> rt_ecos_stop()\n"));
#if 0
	DBGPRINT(RT_DEBUG_INFO, ("===> rt_ecos_stop()\n"));

    /* if the device is not runnung, do nothing */
    if ((sc->state & ETH_DRV_STATE_ACTIVE) == 0)
        return;

	pNetDev = sc;
	pAd = (PRTMP_ADAPTER) RTMP_OS_NETDEV_GET_PRIV(pNetDev);
	if (pAd == NULL)
		return;

	pOSCookie = (POS_COOKIE) pAd->OS_Cookie;

	MlmeRadioOn(pAd);



	RTMPInfClose(pAd);
	VIRTUAL_IF_DOWN(pAd);

	RT_MOD_DEC_USE_COUNT();

	DBGPRINT(RT_DEBUG_INFO, ("<=== rt_ecos_stop()\n"));
#endif
	return;
}


static int rt_ecos_control(struct eth_drv_sc *sc,
                          unsigned long cmd,
                          void *data,
                          int len)
{
	POS_COOKIE		 pOSCookie = NULL;
	PRTMP_ADAPTER	 pAd = NULL;
	char devName[IFNAMSIZ];

	DBGPRINT(RT_DEBUG_INFO, ("rt_ecos_control()\n"));
	pAd = (PRTMP_ADAPTER) RTMP_OS_NETDEV_GET_PRIV(sc);
	if (pAd == NULL)
	    return -1;

	pOSCookie = (POS_COOKIE) pAd->OS_Cookie;
#if 0 //by ziqiang
	switch(cmd)
	{
		case SIOCGIFPHY:
		{
			break;
		}

	}
#endif

	return 0;
}

static int rt_ecos_can_send(struct eth_drv_sc *sc)
{
    return 1;
}

//Copy from eCos kernel, remove the eth_drv_send(ifp);
static  __attribute__((nomips16)) void rt_eth_drv_tx_done(struct eth_drv_sc *sc, CYG_ADDRESS key, int status)
{
    struct ifnet *ifp = &sc->sc_arpcom.ac_if;
    struct mbuf *m0 = (struct mbuf *)key;
    CYGARC_HAL_SAVE_GP();

    // Check for errors here (via 'status')
    ifp->if_opackets++;
    // Done with packet

    // Guard against a NULL return - can be caused by race conditions in
    // the driver, this is the neatest fixup:
    if (m0) {
//        mbuf_key = m0;
        m_freem(m0);
    }
    // Start another if possible
//    eth_drv_send(ifp);
    CYGARC_HAL_RESTORE_GP();
}


extern NDIS_STATUS RTMP_AllocateNdisPacket_AppandMbuf(
	IN	PRTMP_ADAPTER	pAd,
	OUT PNDIS_PACKET   *ppPacket,
	IN	struct mbuf    *pMBuf);

static char rt_convert_sglist_to_mbuf_check_point_counter = 0;
static __attribute__((nomips16,noinline)) PECOS_PKT_BUFFER rt_convert_sglist_to_mbuf(struct eth_drv_sc *sc,
				struct eth_drv_sg *sg_list,
				int sg_len,
				int total_len,
				unsigned long key)
{
	PRTMP_ADAPTER		pAd = NULL;
	PECOS_PKT_BUFFER	pPacket = NULL;
	NDIS_STATUS			Status;
	INT					packetLength = 0;
	BOOLEAN				isFreeSG = TRUE;
        struct mbuf         *pMBuf = NULL;

       DBGPRINT(RT_DEBUG_INFO, ("==>Convert_sglist_to_mbuf()\n"));

	if (total_len >= MAX_ETH_FRAME_SIZE) {
                DBGPRINT(RT_DEBUG_ERROR, ("%s:total_len >= MAX_ETH_FRAME_SIZE\n", __FUNCTION__));
                isFreeSG = TRUE;
                goto FREE_SG;
        }

	pAd = (PRTMP_ADAPTER) RTMP_OS_NETDEV_GET_PRIV(sc);
        pMBuf = (struct mbuf *) key;
        if (sg_len == 2)
        {
                struct mbuf *pMBuf2 = pMBuf->m_next;

                if ((pMBuf2->m_flags & M_EXT) && ((pMBuf2->m_data - pMBuf2->m_ext.ext_buf) > sg_list[0].len))
                {
                        Status = RTMP_AllocateNdisPacket_AppandMbuf(pAd, (PNDIS_PACKET *)&pPacket, pMBuf2);
                        if (Status != NDIS_STATUS_SUCCESS)
                        {
                                DBGPRINT(RT_DEBUG_ERROR, ("%s:can't allocate NDIS PACKET AppandMbuf\n", __FUNCTION__));
                                goto FREE_SG;
                        }
                        pPacket->pDataPtr -= sg_list[0].len;
                        NdisCopyMemory(pPacket->pDataPtr, (void *) sg_list[0].buf, sg_list[0].len);
                        pPacket->pktLen += sg_list[0].len;
                        pMBuf->m_next = NULL;
                        goto XMIT;
                }
                goto COPY_MBUF;
        }
        else if (sg_len == 1)
        {
                if ((pMBuf->m_flags & M_EXT) && ((pMBuf->m_data - pMBuf->m_ext.ext_buf) <= pMBuf->m_ext.ext_size))
                {
                        Status = RTMP_AllocateNdisPacket_AppandMbuf(pAd, (PNDIS_PACKET *)&pPacket, pMBuf);
                        if (Status != NDIS_STATUS_SUCCESS)
                        {
                                DBGPRINT(RT_DEBUG_ERROR, ("%s:can't allocate NDIS PACKET AppandMbuf\n", __FUNCTION__));
                                goto FREE_SG;
                        }
                        isFreeSG = FALSE;
                        goto XMIT;
                }
                goto COPY_MBUF;
        }
        else if (sg_len == 0)
                goto FREE_SG;


COPY_MBUF: /*when sg_len > 2 */
        Status = RTMPAllocateNdisPacket(pAd, (PNDIS_PACKET *)&pPacket, NULL, 0, NULL, MAX_ETH_FRAME_SIZE);
        if (Status != NDIS_STATUS_SUCCESS)
        {		rt_convert_sglist_to_mbuf_check_point_counter++;
				if (rt_convert_sglist_to_mbuf_check_point_counter > 20)
				{
					rt_convert_sglist_to_mbuf_check_point_counter = 0;
                	DBGPRINT(RT_DEBUG_ERROR, ("%s:can't allocate NDIS PACKET\n", __FUNCTION__));
        		}
				goto FREE_SG;
        }

	pPacket->pDataPtr = (PUCHAR)ROUND_UP(pPacket->pDataPtr, 4) - 2;
        while (sg_len > 0)
        {
                NdisCopyMemory(GET_OS_PKT_DATAPTR(pPacket) + packetLength, (void *) sg_list->buf, sg_list->len);
                packetLength += sg_list->len;
                sg_len--;
                sg_list++;
        }

        pPacket->pDataMBuf->m_len = packetLength;
XMIT:
        GET_OS_PKT_LEN(pPacket) = total_len;

FREE_SG:
        if (isFreeSG)
		rt_eth_drv_tx_done(sc,key,0);

        return pPacket;
}

int rt28xx_send_packets(PECOS_PKT_BUFFER skb, PNET_DEV ndev);

static void rt_ecos_send(struct eth_drv_sc *sc,
				struct eth_drv_sg *sg_list,
				int sg_len,
				int total_len,
				unsigned long key)
{
	PECOS_PKT_BUFFER	pPacket = NULL;

   DBGPRINT(RT_DEBUG_INFO, ("==>rt_ecos_send()\n"));

    pPacket = rt_convert_sglist_to_mbuf(sc, sg_list, sg_len, total_len, key);

	//hex_dump("rt_ecos_send",pPacket->pDataPtr,60);
    if (pPacket != NULL)
		rt28xx_send_packets(pPacket, sc);

   DBGPRINT(RT_DEBUG_INFO,("<==rt_ecos_send()\n"));
    return;
}

static void rt_ecos_recv(struct eth_drv_sc *sc,
                        struct eth_drv_sg *sg_list,
                        int sg_len)
{
    /* Not Use */
	return;
}

#ifdef ECOS_NETTASK_SCHDULE_NEW

volatile UINT32 nettask_schedule=0;

static void rt_ecos_deliver(struct eth_drv_sc *sc)
{
	POS_COOKIE			    pOSCookie = NULL;
	PRTMP_ADAPTER	        pAd = NULL;
	RTMP_NET_TASK_STRUCT   NetTask;
	unsigned long flags;
	UINT32 IntSource = 0;
	unsigned long flag=0;
	struct MCU_CTRL *ctl=NULL;
	DBGPRINT(RT_DEBUG_ERROR, ("rt_ecos_deliver()\n"));
	pAd = (PRTMP_ADAPTER) RTMP_OS_NETDEV_GET_PRIV(sc);
	if (pAd == NULL)
	    return;

	pOSCookie = (POS_COOKIE) pAd->OS_Cookie;


	ctl = &pAd->MCUCtrl;
	HAL_DISABLE_INTERRUPTS(flag);
	IntSource = nettask_schedule;
	nettask_schedule = 0;
	HAL_RESTORE_INTERRUPTS(flag);

//#ifdef MT_MAC

	if (IntSource & WF_MAC_INT_3)	//27
	{
		NetTask = pOSCookie->mt_mac_int_3_task;
		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_BCN_DLY)	//11
	{
		NetTask = pOSCookie->bcn_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}



	if (IntSource & MT_INT_BMC_DLY)	//12
	{
		NetTask = pOSCookie->bmc_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

//#endif

	if (IntSource & 0)		//no use
	{
		NetTask = pOSCookie->fifo_statistic_full_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_MGMT_DLY)	//8
	{
		NetTask = pOSCookie->mgmt_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_RX_DATA)	//0
	{
		NetTask = pOSCookie->rx_done_task;
		//log_trace(5);
		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

//#ifdef CONFIG_ANDES_SUPPORT
	if (IntSource & MT_INT_RX_CMD)	//1//
	{
		NetTask = pOSCookie->rx1_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}
//#endif

	if (IntSource & MT_INT_CMD)	//9
	{
		NetTask = pOSCookie->hcca_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_AC3_DLY)	//7
	{
		NetTask = pOSCookie->ac3_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_AC2_DLY)	//6
	{
		NetTask = pOSCookie->ac2_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_AC1_DLY)	//5
	{
		NetTask = pOSCookie->ac1_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_AC0_DLY)	//4 //4
	{
		NetTask = pOSCookie->ac0_dma_done_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_MCU_CMD)	//4 //30
	{
		NetTask = ctl->cmd_msg_task;

		//diag_printf("cmd_msg_task:%s\n",(char *)(NetTask.taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_RESVD)	//4 //18
	{
		NetTask = pOSCookie->tbtt_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}

	if (IntSource & MT_INT_RESVD_2)	//4 //31
	{
		NetTask = pOSCookie->uapsd_eosp_sent_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}
	/*
	if (IntSource & MT_INT_ERR_DET0)	//4 //2
	{
		NetTask = pOSCookie->dfs_task;

		//diag_printf("get:%s\n",(char *)(pNetTask->taskName));
		NetTask.funcPtr(NetTask.data);
	}
	*/
	DBGPRINT(RT_DEBUG_INFO, ("<=== rt_ecos_deliver()\n"));
}

#else

static void rt_ecos_deliver(struct eth_drv_sc *sc)
{
	POS_COOKIE			    pOSCookie = NULL;
	PRTMP_ADAPTER	        pAd = NULL;
	PRTMP_NET_TASK_STRUCT   pNetTask = NULL;
	unsigned long flags;
	int i = 0;

	//DBGPRINT(RT_DEBUG_INFO, ("rt_ecos_deliver()\n"));
	pAd = (PRTMP_ADAPTER) RTMP_OS_NETDEV_GET_PRIV(sc);
	if (pAd == NULL)
	    return;

	pOSCookie = (POS_COOKIE) pAd->OS_Cookie;
	/* Initialize command response event URB */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_MLME_RESET_IN_PROGRESS) ||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS) ||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST) ||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_SUSPEND))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Device Halted or Removed or MlmeRest, exit MlmeHandler! (queue num = %ld)\n", pAd->Mlme.Queue.Num));
		return;
	}

	while(1)
	{
		if (pOSCookie->nettask_handle == 0)
			break;

        /* Note: here must call cyg_mbox_tryget, not cyg_mbox_get */
		RTMP_INT_LOCK(&pAd->irq_lock, flags);
		pNetTask = (PRTMP_NET_TASK_STRUCT) cyg_mbox_tryget(pOSCookie->nettask_handle);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);

		if (pNetTask == NULL)
			break;
		//diag_printf("01%s\n",(char *)(pNetTask->taskName));
		pNetTask->funcPtr(pNetTask->data);
		//diag_printf("02\n");
		i++;
		if (i > 60)break;
	}

	//DBGPRINT(RT_DEBUG_INFO,("<=== rt_ecos_deliver()\n"));
}

#endif //#endif MT7628
static void rt_ecos_poll(struct eth_drv_sc *sc)
{
	return;
}

static int  rt_ecos_int_vector(struct eth_drv_sc *sc)
{
	//usb doesn't has interrupt
	return -1;
}







