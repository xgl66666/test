/*
 * netutils: ping implementation
 */

#include "lwip/opt.h"

#include "lwip/mem.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"

#include "lwip/debug.h"

/**
 * PING_DEBUG: Enable debugging for PING.
 */
#ifndef PING_DEBUG
#define PING_DEBUG     LWIP_DBG_ON
#endif

/** ping receive timeout - in milliseconds */
#define PING_RCV_TIMEO 1000
/** ping delay - in milliseconds */
#define PING_DELAY     100

/** ping identifier - must fit on a u16_t */
#ifndef PING_ID
#define PING_ID        0xAFAF
#endif

/** ping additional data size to include in the packet */
#ifndef PING_DATA_SIZE
#define PING_DATA_SIZE 65
#endif

/* ping variables */
static u16_t ping_seq_num;
struct _ip_addr
{
	rt_uint8_t addr0, addr1, addr2, addr3;
};

/** Prepare a echo ICMP request */
static void ping_prepare_echo( struct icmp_echo_hdr *iecho, u16_t len)
{
	size_t i;
	size_t data_len = len - sizeof(struct icmp_echo_hdr);

	ICMPH_TYPE_SET(iecho, ICMP_ECHO);
	ICMPH_CODE_SET(iecho, 0);
	iecho->chksum = 0;
	iecho->id     = PING_ID;
	iecho->seqno  = htons(++ping_seq_num);

	/* fill the additional data buffer with some data */
	for(i = 0; i < data_len; i++)
	{
		((char*)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
	}

	iecho->chksum = inet_chksum(iecho, len);
}

u8_t ping_raw_recv(void *arg, struct raw_pcb *pcb, struct pbuf *p, ip_addr_t *addr)
{
    u16_t u16Counter = 0;
    struct icmp_echo_hdr *icmphdr = p->payload + IP_HLEN;

    for(u16Counter = 0; u16Counter < p->len; u16Counter++)
    {
        LWIP_DEBUGF(LWIP_DBG_ON, ("%02x ", *((u8_t *)p->payload + u16Counter)));
        if(u16Counter % 16 == 15)
        {
            LWIP_DEBUGF(LWIP_DBG_ON, ("\n"));
        }
    }



    LWIP_DEBUGF(LWIP_DBG_ON, ("%04d %s type %x code %x id %x seqno %x chksum %x\n", __LINE__, __FUNCTION__, ICMPH_TYPE(icmphdr), ICMPH_TYPE(icmphdr),  icmphdr->id, icmphdr->seqno, icmphdr->chksum));
    return 0;
}


/* Ping using the socket ip */
static err_t ping_send(int arg_size, struct ip_addr *addr)
{
	int err;
	struct icmp_echo_hdr *iecho;
	err_t errt;
//	struct sockaddr_in to;
    struct raw_pcb * ping_pcb = NULL;
    struct pbuf* p;
	size_t ping_size = sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;
	LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);

	ping_pcb = raw_new(IP_PROTO_ICMP);

	raw_recv(ping_pcb, ping_raw_recv, NULL);

	raw_connect(ping_pcb, addr);

	iecho = rt_malloc(ping_size);
	if (iecho == RT_NULL)
	{
		return ERR_MEM;
	}

	ping_prepare_echo(iecho, (u16_t)ping_size);

//	dump_pkt(iecho, ping_size);

    p = pbuf_alloc(PBUF_TRANSPORT, ping_size + sizeof(struct pbuf), PBUF_REF);
    LWIP_ASSERT("buffer null", p != NULL);

    p->payload = iecho;

	errt = raw_send(ping_pcb, p);
	if( errt != ERR_OK )
	{
	    return ERR_VAL;
	}






//	to.sin_len = sizeof(to);
//	to.sin_family = AF_INET;
//	to.sin_addr.s_addr = addr->addr;

//	err = lwip_sendto(arg_size, iecho, ping_size, 0, (struct sockaddr*)&to, sizeof(to));
	rt_free(iecho);

	pbuf_free(p);

	raw_remove(ping_pcb);

	return (err ? ERR_OK : ERR_VAL);
}

static void ping_recv(int s)
{
	char buf[64];
	int fromlen, len;
//	struct sockaddr_in from;
	struct ip_hdr *iphdr;
	struct icmp_echo_hdr *iecho;
	struct _ip_addr *addr;

/*
	while((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, (socklen_t*)&fromlen)) > 0)
	{
		if (len >= (sizeof(struct ip_hdr)+sizeof(struct icmp_echo_hdr)))
		{
			addr = (struct _ip_addr *)&(from.sin_addr);
			rt_kprintf("ping: recv %d.%d.%d.%d\n", addr->addr0, addr->addr1, addr->addr2, addr->addr3);

			iphdr = (struct ip_hdr *)buf;
			iecho = (struct icmp_echo_hdr *)(buf+(IPH_HL(iphdr) * 4));
			if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num)))
			{
				return;
			}
			else
			{
				rt_kprintf("ping: drop\n");
			}
		}
	}
*/
	if (len <= 0)
	{
		rt_kprintf("ping: timeout\n");
	}
}

rt_err_t ping(char* target, rt_uint32_t time, rt_size_t size)
{
	int s;
	int timeout = PING_RCV_TIMEO;
	struct ip_addr ping_target;
	rt_uint32_t send_time;
	struct _ip_addr
	{
		rt_uint8_t addr0, addr1, addr2, addr3;
	} *addr;

    send_time = 0;

	if (inet_aton(target, (struct in_addr*)&ping_target) == 0)
	{
	    LWIP_DEBUGF(ICMP_DEBUG, ("%04d %s : inet_aton == 0\n", __LINE__, __FILE__));
    	return -RT_ERROR;
	}

	addr = (struct _ip_addr*)&ping_target;
	rt_kprintf("ping: send %d.%d.%d.%d\n", addr->addr0, addr->addr1, addr->addr2, addr->addr3);
/*
	if ((s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0)
	{
	    rt_kprintf("create socket failled\n");
		return -RT_ERROR;
	}
*/
//	lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	while (1)
	{
		if (ping_send(s, &ping_target) == ERR_OK)
		{

			ping_recv(s);
		}
		else
		{
			rt_kprintf("ping: send %d.%d.%d.%d - error\n", addr->addr0, addr->addr1, addr->addr2, addr->addr3);
		}

		send_time ++;
		if (send_time >= time) break; /* send ping times reached, stop */

		rt_thread_delay(PING_DELAY); /* take a delay */
	}

	return RT_EOK;
}

ip_addr_t dns_result_addr;

void lwip_dns_found_callback(const char *name, ip_addr_t *ipaddr, void *callback_arg)
{
	struct _ip_addr
	{
		rt_uint8_t addr0, addr1, addr2, addr3;
	} *addr;

	addr = (struct _ip_addr*)ipaddr;

    LWIP_DEBUGF(SYS_DEBUG, ("NAME:%s IP:%d.%d.%d.%d\n", name, addr->addr0, addr->addr1, addr->addr2, addr->addr3));
}

rt_err_t  lwip_dns_query(const char * hostname)
{
    err_t err_return;
    err_return = dns_gethostbyname(hostname, &dns_result_addr, lwip_dns_found_callback, NULL);
    if(err_return != ERR_OK)
    {
        LWIP_DEBUGF(SYS_DEBUG, ("dns_gethostbyname = %d\n", err_return));
    }

    return RT_EOK;
}

static err_t lwip_tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
static err_t lwip_tcp_client_connected(void *arg, struct tcp_pcb *pcb, err_t err);

void lwip_tcp_client_init(void)
{
   struct tcp_pcb *pcb;
   struct ip_addr dest;
   err_t ret_val;
   IP4_ADDR(&dest, 192, 168, 0, 101);

   pcb = tcp_new();
   tcp_bind(pcb, IP_ADDR_ANY, 7000); //client port for outcoming connection
   tcp_arg(pcb, NULL);
    ret_val = tcp_connect(pcb, &dest, 7, lwip_tcp_client_connected); //server port for incoming connection
   if (ret_val != ERR_OK)
   {
       printf("tcp_connect(): Errors on return value, returned value is %d\n", ret_val);
   }
}

static void lwip_tcp_client_close(struct tcp_pcb *pcb)
{
   tcp_arg(pcb, NULL);
   tcp_sent(pcb, NULL);
   tcp_close(pcb);

   printf("\nclient_close(): Closing...\n");
}

static err_t lwip_tcp_client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
   char *string = "Hello!";
   LWIP_UNUSED_ARG(arg);

    if (err != ERR_OK)
    printf("\nclient_connected(): err argument not set to ERR_OK, but is value is %d\n", err);

   else
   {
       tcp_sent(pcb, lwip_tcp_client_sent);
       tcp_write(pcb, string, sizeof(string), 0);
   }

   return err;
}

static err_t lwip_tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
   LWIP_UNUSED_ARG(arg);

   printf("\nclient_sent(): Number of bytes ACK'ed is %d", len);

   lwip_tcp_client_close(pcb);

   return ERR_OK;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(ping, ping network host);
#endif
