


#include <stdlib.h>
#include <stdio.h>

#include <network.h>
#include <arpa/inet.h>
//#include <sys/types.h>
//#include <cyg/kernel/kapi.h>
#ifdef CYGPKG_NET_INET6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#endif


#define SHELL_PRINT diag_printf

#ifdef CYGBLD_DEVS_ETH_DEVICE_H    // Get the device config if it exists
#include CYGBLD_DEVS_ETH_DEVICE_H  // May provide CYGTST_DEVS_ETH_TEST_NET_REALTIME
#endif

#ifdef CYGPKG_NET_TESTS_USE_RT_TEST_HARNESS // do we use the rt test?
# ifdef CYGTST_DEVS_ETH_TEST_NET_REALTIME // Get the test ancilla if it exists
#  include CYGTST_DEVS_ETH_TEST_NET_REALTIME
# endif
#endif

//#include "gxcore_ecos_module.hxx"
#define NETWORK_STATIC 0x02
#define NETWORK_DHCP 0x01
#define DHCPSTATE_FAILED  12 // Net is down

struct network_info{
	const  char *ipaddress;
	const  char  *netmask;
	const  char  *broadcast;
	const  char  *gateway;
	const  char  *serverip;
};


#define UNIQUEID 0x1234
#define NUM_PINGS 4
#define MAX_PACKET 4096
#define MIN_PACKET   64
#define MAX_SEND   4000
#define PACKET_ADD  ((MAX_SEND - MIN_PACKET)/NUM_PINGS)
#define	PING_SERVER_NUM				    30

/* Private Macros ---------------------------------------------------------- */
static unsigned int ok_recv = 0;
static unsigned int  bogus_recv = 0;
static unsigned char pkt1[MAX_PACKET];
static unsigned char pkt2[MAX_PACKET];

static struct bootp mbp;
extern struct network_info net_info;

externC cyg_tick_count_t cyg_current_time(void);
externC cyg_tick_count_t cyg_current_time(void);

static int isRightAddr(const char *ip);
void network_init(cyg_uint16 mode,struct network_info *net_info);
static void ping_test(struct bootp *bp,const char *ping_server,int num);

	 
int netinit(int argc,char *argv[]){

	int i;
	unsigned int mode = NETWORK_DHCP;
	
	for(i=0;i<argc;i++){

		if(strncmp((const char*)argv[i],"static",1)==0){
			mode = NETWORK_STATIC;
			continue;
		}
		if(strncmp((const char*)argv[i],"dynamic",1)==0){
			mode = NETWORK_DHCP;
			continue;
		}
		if(strncmp((const char*)argv[i],"ip",2)==0){
			if(i<argc && isRightAddr((const char*)argv[++i]))
				net_info.ipaddress = argv[i];
			mode = NETWORK_STATIC;
			continue;
		}
		if(strncmp((const char*)argv[i],"netmask",3)==0){
			if(i<argc && isRightAddr((const char*)argv[++i]))
				net_info.netmask = argv[i];
			continue;
		}
		if(strncmp((const char*)argv[i],"gateway",4)==0){
			if(i<argc && isRightAddr((const char*)argv[++i]))
				net_info.gateway = argv[i];
			continue;
		}
		if(strncmp((const char*)argv[i],"broadcast",5)==0){
			if(i<argc && isRightAddr((const char*)argv[++i]))
				net_info.broadcast = argv[i];
			continue;
		}
		if(strncmp((const char*)argv[i],"serverip",6)==0){
			if(i<argc && isRightAddr((const char*)argv[++i]))
				net_info.serverip = argv[i];
			continue;
		}
	}
	
	network_init(mode, &net_info);
	return 0;
}


int ping(int argc,char *argv[]){
	unsigned int i;
	
	int cmdFlag = 0;
	char *ip = NULL;
	static int ping_num = 4;
	if(argc<1){
		
		SHELL_PRINT("ping [-n] host  \n");
		return 0;
	}
	for(i=0;i<argc;i++){
		if(isRightAddr((const char*)argv[i])){
			ip = (char*)argv[i];
			cmdFlag = 1;
			SHELL_PRINT("ip is  %s \n",ip);
			continue;
		}
		if(strcmp((char*)argv[i],"-n")==0){

			if(i<argc){
				if(isRightAddr((const char*)argv[i+1]))
					continue;
				ping_num = atol((char*)argv[i+1]);
			}
			continue;
		}		
	}
	SHELL_PRINT("ip is  %s in line %d \n",ip,__LINE__);
	if(cmdFlag)ping_test(&mbp,ip,ping_num);
	else SHELL_PRINT("Ivalid argument  \n");
	
	return 0;
}



static int inet_cksum(u_short *addr, int len)
{
    register int nleft = len;
    register u_short *w = addr;
    register u_short answer;
    register u_int sum = 0;
    u_short odd_byte = 0;

/* Our algorithm is simple, using a 32 bit accumulator (sum),
we add sequential 16 bit words to it, and at the end, fold
back all the carry bits from the top 16 bits into the lower
16 bits. */
    while( nleft > 1 )  
    {
        sum += *w++;
        nleft -= 2;
    }

/* mop up an odd byte, if necessary                                         */
    if( nleft == 1 ) 
    {
        *(u_char *)(&odd_byte) = *(u_char *)w;
        sum += odd_byte;
    }

/* add back carry outs from top 16 bits to low 16 bits                      */
    sum = (sum >> 16) + (sum & 0x0000ffff);          /* add hi 16 to low 16 */
    sum += (sum >> 16);                              /* add carry           */
    answer = ~sum;                                   /* truncate to 16 bits */
    return (answer);
}

/*---------------------------------------------------------------------------*/
/* check up icmp					                            		     */ 		
static int show_icmp(unsigned char *pkt, unsigned int len, struct sockaddr_in *from, struct sockaddr_in *to)
{
    volatile unsigned long long *tp, tv;
    struct ip *ip;
    struct icmp *icmp;
    tv = cyg_current_time();
    ip = (struct ip *)pkt;

    if ((len < sizeof(*ip)) || ip->ip_v != IPVERSION) {
        SHELL_PRINT("%s: Short packet or not IP! - Len: %d, Version: %d\n", 
                    inet_ntoa(from->sin_addr), len, ip->ip_v);
        return 0;
    }

    icmp = (struct icmp *)(pkt + sizeof(*ip));
    len -= (sizeof(*ip) + 8);
    tp = (unsigned long long *)&icmp->icmp_data;
    
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
        SHELL_PRINT("%s: Invalid ICMP - type: %d\n", 
                    inet_ntoa(from->sin_addr), icmp->icmp_type);
        return 0;
    }

    if (icmp->icmp_id != UNIQUEID) {
        SHELL_PRINT("%s: ICMP received for wrong id - sent: %x, recvd: %x\n", 
                    inet_ntoa(from->sin_addr), UNIQUEID, icmp->icmp_id);
    }

    SHELL_PRINT("%d bytes from %s: ", len, inet_ntoa(from->sin_addr));
    SHELL_PRINT("icmp_seq=%d", ntohs(icmp->icmp_seq));
   // SHELL_PRINT(", time=%dms\n", (int)(tv - *tp)*10);
	
    SHELL_PRINT(", time=%dms\n", (int)(tv - *tp));//Mstar platform Hz=1000
    return (from->sin_addr.s_addr == to->sin_addr.s_addr);
}

/*---------------------------------------------------------------------------*/
/* Ping host IP address	
*/
int icmp_index = 0;
static void ping_host(int s, struct sockaddr_in *host)
{
    struct icmp *icmp = (struct icmp *)pkt1;
    int icmp_len = MIN_PACKET;
    int seq;
    volatile unsigned long long *tp;
    long *dp;
    struct sockaddr_in from;
    //unsigned int len, fromlen,send_len;    
   	int len, fromlen,send_len;    
	int i;
    for (seq = 0;  seq < 1;  seq++, icmp_len += PACKET_ADD ) 
    {
        // Build ICMP packet
        icmp->icmp_type = ICMP_ECHO;
        icmp->icmp_code = 0;
        icmp->icmp_cksum = 0;
        icmp->icmp_seq = ntohs(icmp_index ++);
        icmp->icmp_id = 0x1234;

        // Set up ping data
        tp = (unsigned long long *)&icmp->icmp_data;
        *tp++ = cyg_current_time();
        dp = (long *)tp;
        for (i = sizeof(*tp);  i < icmp_len;  i += sizeof(*dp)) {
            *dp++ = i;
        }

        // Add checksum
        icmp->icmp_cksum = inet_cksum( (unsigned short *)icmp, icmp_len+8);
	    
        // Send it off
        send_len = sendto(s, icmp, icmp_len+8, 0, (struct sockaddr *)host, sizeof(*host));
		
		
        if ( send_len < 0) 
        {
           // TNR_OFF();
            perror("sendto");
            continue;
           
        }                     
        fromlen = sizeof(from);

        //receive ICMP packets        
        len = recvfrom(s, pkt2, sizeof(pkt2), 0, (struct sockaddr *)&from, &fromlen);
        if (len < 0) 
        {
        	/*
            icmp_len = MIN_PACKET - PACKET_ADD; 
            SHELL_PRINT("MIN_PACKET:%d  ",MIN_PACKET);
            SHELL_PRINT("PACKET_ADD:%d  ",PACKET_ADD);
            SHELL_PRINT("icmp_len:%d \r\n",icmp_len);
            */
            SHELL_PRINT("ping request time out ! \r\n");
			bogus_recv++;
        }        
        if(len >= 0)                  
        {
            if (show_icmp(pkt2, len, &from, host)) 
            {
                ok_recv++;
            } 
            else 
            {
                bogus_recv++;
            }
        }
		//if(len ==0){
		//	SHELL_PRINT("receive len is %d \r\n ",len);
		//}
    }
}

/*---------------------------------------------------------------------------*/
/* Init host IP address					                           		     */ 	
static void ping_test(struct bootp *bp,const char *ping_server,int num)
{
    struct protoent *p;
    struct timeval tv;
    struct sockaddr_in host;
    int s,i;

/* Change domainname into IP Address                                         */ 
    struct hostent *hent;

    hent = gethostbyname(ping_server);  
    if (hent != NULL) 
    {
         SHELL_PRINT("#########################################################################");
         SHELL_PRINT("          PASS:<%s is %s>\n", hent->h_name, inet_ntoa(*(struct in_addr *)hent->h_addr_list[0]));  //´òÓ¡ÓòÃûºÍIPµØÖ·
         SHELL_PRINT("#########################################################################\n");
    }

    if ((p = getprotobyname("icmp")) == (struct protoent *)0) 
    {
        perror("getprotobyname");
        return;
    }

/* Init the socket parameter                                                 */    
    s = socket(AF_INET, SOCK_RAW, p->p_proto);
    if (s < 0) {
        perror("socket");
        return;
    }
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    // Set up host address   
    memset(&host,0,sizeof(host));              
    host.sin_family = AF_INET;
    host.sin_len = sizeof(host);
    host.sin_addr.s_addr = inet_addr(ping_server);
    host.sin_port = 0; 
	
/*---------------------------------------------------------------------------*/
// Ping right IP address
	SHELL_PRINT("##############################################################\n");
	SHELL_PRINT("                 Ping server %s\n", inet_ntoa(host.sin_addr));
	SHELL_PRINT("##############################################################\n");
	ok_recv = 0;
	bogus_recv = 0;
    for(i=0;i<num;i++)
	{
    	ping_host(s, &host);
    }
	SHELL_PRINT("\n**************************************************************\n");
	SHELL_PRINT("          Sent %d packets, received %d OK, %d bad\n",num , ok_recv, bogus_recv);
	SHELL_PRINT("**************************************************************\n");
	close(s);
}

static int isRightAddr(const char *ch){
	int a1,a2,a3,a4;	
	sscanf(ch,"%d.%d.%d.%d",&a1,&a2,&a3,&a4);
	if( (a1>=0 && a1<255) && (a2>=0 && a2<255) && (a3>=0 && a3<255) && (a4>=0 && a4<255))
		return 1;
	return 0;
}

int ping_cmd(unsigned char  *a, unsigned char *b,unsigned char *c, unsigned char *d)
{
	char  *argv[5];
	int argc =0;
	
	//printf("%s %s %s %s \n",a,b,c,d);
	if (a &&strlen(a))
	{
		argv[0]=a;
		argc++;
	}
	if (b&&strlen(b))
	{
		argv[1]=b;
		argc++;
	}	
	if (c&&strlen(c))
	{
		argv[2]=c;
		argc++;
	}	
	if (d&&strlen(d))
	{
		argv[3]=d;
		argc++;
	}
	printf("\n\n\n\n%s\n\n\n\n\n",__FUNCTION__);
        ping(argc,(char **)argv);
	return 1;
}
	

int netinit_cmd(unsigned char *a)
{
	if (a== NULL ||strlen(a ) == 0)
		{
		printf("Usage:netinit static\n");
		return false;
		}
	printf("\n\n\n\n%s\n\n\n\n\n",__FUNCTION__);
	netinit(1,(char **)&a);
	return 1;
}

