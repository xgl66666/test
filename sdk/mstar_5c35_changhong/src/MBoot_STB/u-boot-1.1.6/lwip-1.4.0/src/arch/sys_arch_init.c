#include <rtthread.h>
#ifdef RT_USING_FINSH
#include <finsh.h>
#endif

#include "lwip/debug.h"
#include "lwip/mem.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"

#include "netif/ethernetif.h"
#include "netif/etharp.h"
#include "arch/emac_netif.h"

extern u8_t g_bIsInitialized;

/*
 * lwip system initial entry
 */
void lwip_sys_init(void)
{
	struct ip_addr ipaddr, netmask, gw;
    if(g_bIsInitialized)
    {
        LWIP_DEBUGF(LWIP_DEBUG, ("%04d %s Already Initialized", __LINE__, __FUNCTION__));
        return;
    }
//  	tcpip_init(RT_NULL, RT_NULL);
    lwip_init();

#if LWIP_DHCP
	rt_uint32_t mscnt = 0;

	IP4_ADDR(&gw, 0,0,0,0);
	IP4_ADDR(&ipaddr, 0,0,0,0);
	IP4_ADDR(&netmask, 0,0,0,0);
#else
	IP4_ADDR(&ipaddr, RT_LWIP_IPADDR0, RT_LWIP_IPADDR1, RT_LWIP_IPADDR2, RT_LWIP_IPADDR3);
	IP4_ADDR(&gw, RT_LWIP_GWADDR0, RT_LWIP_GWADDR1, RT_LWIP_GWADDR2, RT_LWIP_GWADDR3);
	IP4_ADDR(&netmask, RT_LWIP_MSKADDR0, RT_LWIP_MSKADDR1, RT_LWIP_MSKADDR2, RT_LWIP_MSKADDR3);
#endif

//  	tcpip_init(RT_NULL, RT_NULL);

    netif_add(&emac_netif, &ipaddr, &netmask, &gw, NULL, emac_netif_init, ethernet_input);

    netif_set_default(&emac_netif);

    netif_set_up(&emac_netif);

#if 1 // Ningyuan 110727 due to EMAC porting not finish yet

#if LWIP_DHCP
    dhcp_start(&emac_netif);
	/* use DHCP client */


    while (emac_netif.ip_addr.addr == 0)
	{
        MsOS_DelayTask(DHCP_FINE_TIMER_MSECS * RT_TICK_PER_SECOND / 1000);
        LWIP_DEBUGF(RAW_DEBUG,("TIME:%d\n", MsOS_GetSystemTime()));
        dhcp_fine_tmr();
        mscnt += DHCP_FINE_TIMER_MSECS;
        if (mscnt >= DHCP_COARSE_TIMER_SECS*1000)
        {
            dhcp_coarse_tmr();
            mscnt = 0;
        }
    }

	rt_kprintf("Acquired IP address from DHCP server:");
	rt_kprintf("%d.%d.%d.%d\n", emac_netif.ip_addr.addr & 0xff,
		(emac_netif.ip_addr.addr>>8) & 0xff,
		(emac_netif.ip_addr.addr>>16) & 0xff,
		(emac_netif.ip_addr.addr>>24) & 0xff);
#endif
#endif

#if defined(RT_USING_FINSH) && (LWIP_STATS_DISPLAY)
    finsh_syscall_append("lwip_info", (syscall_func)stats_display);
#endif
    g_bIsInitialized = TRUE;
}
