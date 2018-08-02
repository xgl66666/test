
#include <network.h>
#include <stdlib.h>

#define SHELL_PRINT diag_printf

#if 0
struct bootp {
    unsigned char    bp_op;			/* packet opcode type */
    unsigned char    bp_htype;			/* hardware addr type */
    unsigned char    bp_hlen;			/* hardware addr length */
    unsigned char    bp_hops;			/* gateway hops */
#ifdef __ECOS
    u_int32_t        bp_xid;			/* transaction ID */
#else
    unsigned int     bp_xid;			/* transaction ID */
#endif
    unsigned short   bp_secs;			/* seconds since boot began */
    unsigned short   bp_flags;			/* RFC1532 broadcast, etc. */
    struct in_addr   bp_ciaddr;			/* client IP address */
    struct in_addr   bp_yiaddr;			/* 'your' IP address */
    struct in_addr   bp_siaddr;			/* server IP address */
    struct in_addr   bp_giaddr;			/* gateway IP address */
    unsigned char    bp_chaddr[BP_CHADDR_LEN];	/* client hardware address */
    char	     bp_sname[BP_SNAME_LEN];	/* server host name */
    char	     bp_file[BP_FILE_LEN];	/* boot file name */
    unsigned char    bp_vend[BP_VEND_LEN];	/* vendor-specific area */
    /* note that bp_vend can be longer, extending to end of packet. */
};
#endif

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

static int network_init_static(const char *if_name,struct network_info *net_info);
externC unsigned int network_cable_link(void);

struct network_info net_info ={
		"192.168.1.22", 	// Ip Address
		"255.255.255.0",	// netmask IP
		"192.168.1.255",	// Broadcase IP
		"192.168.1.1",	// Gateway IP
		"192.168.1.1",	// Serverip Address
};

cyg_uint8 state;

const char *eth_name =  "ra0";
struct bootp bootp_data;

externC
void network_init(cyg_uint16 mode,struct network_info *net_info){

	if(mode == NETWORK_DHCP)
	{
		SHELL_PRINT("NETWORK_DHCP\n");
		SHELL_PRINT("gateware is %d\n",eth0_bootp_data.bp_giaddr.s_addr);
	}
	if(mode == NETWORK_STATIC)
	{
		SHELL_PRINT("NETWORK_STATIC\n");
		network_init_static(eth_name,net_info);
	}
}

int network_init_static(const char *if_name,struct network_info *net_info)
{

	build_bootp_record(
						&bootp_data,
						if_name,
						net_info->ipaddress,
						net_info->netmask,
						net_info->broadcast,
						net_info->gateway,
						net_info->serverip);
	if(!init_net(if_name, &bootp_data))
	{
		SHELL_PRINT("init_net failed !\n");
		return -1;
	}
	SHELL_PRINT("init_net succed.\n");
	return 0;

}


int network_connect_mode(const char* if_name,unsigned short cmd,struct network_info *net_info,cyg_uint8 *user,cyg_uint8 *password )
{
	int iRet = -1;

	switch(cmd)
	{
		case NETWORK_STATIC:
			iRet	= network_init_static(if_name, net_info);
			if(iRet < 0)
			{
				SHELL_PRINT("Init Network Fail,Func = network_init_static() \n");
			}
			break;
		default:
			SHELL_PRINT("Please Choose The Right Command To Initial The Network \n");
			break;
	}

	if(iRet < 0)
	{

	}

	return iRet;
}


