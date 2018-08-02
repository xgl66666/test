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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiLAN.c
/// @brief  LAN API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "apiLAN.h"
#include <network.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <fcntl.h>
#include "drvGMAC.h"
#include <dhcp.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <pkgconf/ns_dns.h>
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern unsigned long dns_server[2];
MS_U8 MAPI_IEEE802_11_GetApList(MS_U32* u32ap_list_array, MS_U8 u8max_num) __attribute__ ((weak)); //return num of AP;
void MAPI_IEEE802_11_Join_By_Name(MS_U8* pu8Name) __attribute__ ((weak));
void MAPI_IEEE802_11_Auth_Mode(MS_U8 mode, MS_U8* password, MS_U8 pw_len) __attribute__ ((weak));
//void MDrv_UsbCdc_ChangeMacAddr(MS_U8* mac);
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U8 _MApi_LAN_dhcp_cfig_busy_state;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
// Compute INET checksum
static int _MApi_LAN_NetCksum(MS_U16* addr, int len)
{
    int nleft = len;
    MS_U16* w = addr;
    MS_U16 answer;
    MS_U32 sum = 0;
    MS_U16 odd_byte = 0;

    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while( nleft > 1 )  {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if( nleft == 1 ) {
        *(u_char *)(&odd_byte) = *(u_char *)w;
        sum += odd_byte;
    }

    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    sum = (sum >> 16) + (sum & 0x0000ffff); /* add hi 16 to low 16 */
    sum += (sum >> 16);                     /* add carry */
    answer = ~sum;                          /* truncate to 16 bits */
    return (answer);
}



static int _MApi_LAN_TicksToMs( MS_U32 ticks )
{
    int ticks_per_second = 1000000000/(CYGNUM_HAL_RTC_NUMERATOR/CYGNUM_HAL_RTC_DENOMINATOR);
    int ms = (ticks * 1000)/ticks_per_second;
    return ms;
}


static MS_BOOL _MApi_LAN_GetIfParam(struct ifreq* ifr_p, MS_U32 item){

	static int _sock=0;

	_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( _sock == -1 ){
		perror("socket error");
		return FALSE;
	}

	memset(ifr_p, 0, sizeof( struct ifreq ) );
	strcpy(ifr_p->ifr_name, "eth0");

	if( ioctl( _sock, item, ifr_p ) == -1 ){
		perror("ioctl error");
		close(_sock);
		return FALSE;
	}
	close(_sock);
	return TRUE;
}


static MS_BOOL _MApi_LAN_SetIfParam(struct ifreq ifr, MS_U32 item){

	static int _sock = 0;

	_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( _sock == -1 )
	{
		perror("socket error");
		return FALSE;
	}

	strcpy( ifr.ifr_name, "eth0");
	if( ioctl( _sock, item, &ifr ) != 0 ){
          perror("ioctl error");
          close(_sock);
	   return FALSE;
	}

	close(_sock);
	return TRUE;

}


static int _MApi_LAN_ShowIcmp(unsigned char *pkt, int len,
          struct sockaddr_in *from, struct sockaddr_in *to)
{
    struct icmp *icmp;
    cyg_tick_count_t *tp;
    struct ip *ip;
    cyg_tick_count_t tv;

    ip = (struct ip *)pkt;//pointer to IP header

    tv = cyg_current_time();// get current time

    if ((len < sizeof(*ip)) || ip->ip_v != IPVERSION) {//check packet length and IP vertion
        diag_printf("%s: Short packet or not IP! - Len: %d, Version: %d\n",
                    inet_ntoa(from->sin_addr), len, ip->ip_v);
        return 0;
    }
    icmp = (struct icmp *)(pkt + sizeof(*ip));
    len -= (sizeof(*ip) + 8);
    tp = (cyg_tick_count_t *)&icmp->icmp_data;
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
        diag_printf("%s: Invalid ICMP - type: %d\n",
                    inet_ntoa(from->sin_addr), icmp->icmp_type);
        return 0;
    }
    if (icmp->icmp_id != 0x1234) {
        diag_printf("%s: ICMP received for wrong id - sent: %x, recvd: %x\n",
                    inet_ntoa(from->sin_addr), 0x1234, icmp->icmp_id);
    }
    diag_printf("%d bytes from %s: ", len, inet_ntoa(from->sin_addr));
    diag_printf("icmp_seq=%d", icmp->icmp_seq);
    diag_printf(", time=%dms\n", _MApi_LAN_TicksToMs(tv - *tp));
    return (from->sin_addr.s_addr == to->sin_addr.s_addr);
}




static MS_U8 _MApi_LAN_PingHost(int socket_fd, struct sockaddr_in *host, MS_U16 u16Seq)
{
    unsigned char ping_out_pkt[128];
    unsigned char ping_in_pkt[128];
    struct icmp *icmp = (struct icmp *)ping_out_pkt;
    int icmp_len = 64;
    int ok_recv, bogus_recv;
    cyg_tick_count_t *tp;
    long *dp;
    struct sockaddr_in from;
    int i, len, fromlen;
    MS_U8 result=0;

    ok_recv = 0;
    bogus_recv = 0;

    // Build ICMP packet
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq = u16Seq;
    icmp->icmp_id = 0x1234;
    // Set up ping data
    tp = (cyg_tick_count_t *)&icmp->icmp_data;
    *tp++ = cyg_current_time();
    dp = (long *)tp;
    for (i = sizeof(*tp);  i < icmp_len;  i += sizeof(*dp)) {
        *dp++ = i;
    }
    // Add checksum
    icmp->icmp_cksum = _MApi_LAN_NetCksum( (u_short *)icmp, icmp_len+8);
    // Send it off
    if (sendto(socket_fd, icmp, icmp_len+8, 0, (struct sockaddr *)host, sizeof(*host)) < 0) {
        perror("sendto");
        return 0;
    }
    // Wait for a response
    fromlen = sizeof(from);
    len = recvfrom(socket_fd, ping_in_pkt, sizeof(ping_in_pkt), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
    if (len < 0) {
        perror("recvfrom");
    }
    else {
        if (_MApi_LAN_ShowIcmp(ping_in_pkt, len, &from, host))
        {
            ok_recv++;
            result = 1;
        }
        else
            bogus_recv++;
    }
    return result;
}


//-------------------------------------------------------------------------------------------------
/// MApi_LAN_init 
/// @param void
/// @return others - Current IP value
/// @return 0 - OK
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_LAN_init(void)
{
	return MDrv_EMAC_Init();
}


//-------------------------------------------------------------------------------------------------
/// Get current IP configuration
/// @param intf_name \b IN: Interface name
/// @return others - Current IP value
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_LAN_GetIp(MS_U8* intf_name)
{
    int s;
    struct sockaddr_in * ip_p;
    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
       perror("Socket error");
       return FALSE;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name,(char*)intf_name);

    if(ioctl(s,SIOCGIFADDR,&ifr) < 0)
    {
       perror("ioctl error");
	close(s);
       return FALSE;
    }
    ip_p = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr;
    close(s);
    return ip_p->sin_addr.s_addr;
}

//-------------------------------------------------------------------------------------------------
/// Get current Subnet configuration
/// @param intf_name \b IN: Interface name
/// @return others - Current Subnet value
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_LAN_GetSubnet(MS_U8* intf_name)
{
    int s;
    struct sockaddr_in* netmask_p;
    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
       perror("Socket error");
       return FALSE;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name,(char*)intf_name);

    if(ioctl(s,SIOCGIFNETMASK,&ifr) < 0)
    {
       perror("ioctl error");
	close(s);
       return FALSE;
    }


    netmask_p = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr;

    close(s);
    return netmask_p->sin_addr.s_addr;
}

//-------------------------------------------------------------------------------------------------
/// Get current Gateway configuration
/// @param void \b IN: NONE
/// @return others - Current gateway value
/// @return 0 - No gateway specified
/// @note
//-------------------------------------------------------------------------------------------------

externC MS_U32 get_default_gateway(void);
MS_U32 MApi_LAN_GetGateway(void)
{
    return get_default_gateway();
}

//-------------------------------------------------------------------------------------------------
/// Get current MAC configuration
/// @param intf_name \b IN: Interface name
/// @param mac \b IN: Storage to put current MAC
/// @return 1 - Success
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_GetMac(MS_U8* intf_name, MS_U8* mac)
{

   int s;

    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket error");
        return FALSE;
    }

    struct ifreq ifr;

    strcpy(ifr.ifr_name,(char*)intf_name);

    if(ioctl(s,SIOCGIFHWADDR,&ifr) != 0)
    {
        perror("ioctl error");
	 close(s);
        return FALSE;
    }

    close(s);
    memcpy(mac, &ifr.ifr_hwaddr.sa_data[0], 6);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Set LAN IP
/// @param address \b IN: LAN IP
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_IpConfig(MS_U8* address)
{
    int s=0;
    struct sockaddr_in* addr=NULL;
    struct ifreq ifr;

    s = socket( PF_INET, SOCK_DGRAM, 0 );
    if( s == -1 ){
        perror("socket error");
        return FALSE;
    }

    memset(&ifr,0,sizeof( struct ifreq ) );
    strcpy(ifr.ifr_name,"eth0");

    if( ioctl( s, SIOCGIFADDR, &ifr ) == 0 ){
       if( ioctl( s, SIOCDIFADDR, &ifr ) != 0 ){
           perror("ioctl error");
           close(s);
           return FALSE;
       }
    }
    memset( &ifr, 0, sizeof( struct ifreq ) );
    addr= (struct sockaddr_in *)&(ifr.ifr_addr);
    memset(addr, 0, sizeof( struct sockaddr_in) );
    addr->sin_len=sizeof(struct sockaddr_in);
    addr->sin_family=AF_INET;
    addr->sin_addr.s_addr=inet_addr((char*)address);
    strcpy(ifr.ifr_name,"eth0");

    if( ioctl( s, SIOCSIFADDR, &ifr ) != 0 ){
        perror("ioctl error");
        close(s);
        return FALSE;
    }
    close(s);
    //set_eth0_interface_data(address, NULL, NULL, NULL, 0);
#if 0

	init_all_network_interfaces();
#endif
	return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set LAN Subnet
/// @param subnet \b IN: subnet
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_SubnetConfig(MS_U8* subnet)
{
    int s=0;
    struct sockaddr_in* addr=NULL;
    struct ifreq ifr;

    s = socket( PF_INET, SOCK_DGRAM, 0 );
    if( s == -1 ){
        perror("socket error");
        return FALSE;
    }
    memset( &ifr, 0, sizeof( struct ifreq ) );
    addr= (struct sockaddr_in *)&(ifr.ifr_addr);
    memset(addr, 0, sizeof( struct sockaddr_in) );
    addr->sin_len=sizeof(struct sockaddr_in);
    addr->sin_family=AF_INET;
    addr->sin_addr.s_addr=inet_addr((char*)subnet);
    strcpy(ifr.ifr_name,"eth0");

    if( ioctl( s, SIOCSIFNETMASK, &ifr ) != 0 ){
        perror("ioctl error");
        close(s);
        return FALSE;
    }
    close(s);

    //set_eth0_interface_data(NULL, subnet, NULL, NULL, 0);
    //init_all_network_interfaces();
    return TRUE;
}

externC int get_current_dhcp_mode(void);
externC void set_current_dhcp_mode(int val);
//-------------------------------------------------------------------------------------------------
/// Set DHCP mode
/// @param on \b IN: on
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_DhcpConfig(MS_U16 on)
{
    int s=0;
    struct ifreq ifr;
    s = socket( PF_INET, SOCK_DGRAM, 0 );
    if( s == -1 ){
        perror("socket error");
        return FALSE;
    }

    memset(&ifr,0,sizeof( struct ifreq ) );
    strcpy(ifr.ifr_name,"eth0");

    if(0 == on)
    {
        _MApi_LAN_dhcp_cfig_busy_state = 10;//turn off busy
    }
    else
    {
        _MApi_LAN_dhcp_cfig_busy_state = 0;//turn off busy
    }

    if(on != get_current_dhcp_mode())
    {
        set_current_dhcp_mode(on);
        if(1 == get_current_dhcp_mode()){
            eth0_dhcpstate = 0;
            if( ioctl( s, SIOCGIFADDR, &ifr ) == 0 ){
                if( ioctl( s, SIOCDIFADDR, &ifr ) != 0 ){
                    perror("ioctrl error");
                     close(s);
                    return FALSE;
                }
            }
        cyg_semaphore_post( &dhcp_needs_attention );
	    }
        // if turn off DHCP mode
	    else{
	        if(eth0_dhcpstate != DHCPSTATE_FAILED){
                eth0_dhcpstate = DHCPSTATE_BOOTP_FALLBACK;
                cyg_semaphore_post( &dhcp_needs_attention );
                MsOS_DelayTask(100);
                init_loopback_interface(0);
    	    }
            else{
                eth0_dhcpstate = DHCPSTATE_BOOTP_FALLBACK;
                cyg_semaphore_post( &dhcp_needs_attention );
            }
        }
    }

    close(s);
    //init_all_network_interfaces();
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Set DHCP mode busy status
/// @return TRUE - busy
/// @return FALSE - not busy
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_DhcpConfigBusy(void)
{
    return (MApi_LAN_GetDhcpState() == _MApi_LAN_dhcp_cfig_busy_state);
}
//-------------------------------------------------------------------------------------------------
/// Set DHCP mode
/// @param on \b IN: on
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_DhcpConfigSync(MS_U16 on)
{
    MS_BOOL result = TRUE;
    result = MApi_LAN_DhcpConfig(on);

    while(MApi_LAN_DhcpConfigBusy())
    {
        MsOS_DelayTask(100);
    }

    return result;
}
//-------------------------------------------------------------------------------------------------
/// Set LAN Gateway
/// @param gateway \b IN: LAN gateway
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_GatewaySet(MS_U8* gateway)
{
    struct sockaddr_in* addrp;
    struct sockaddr_in pre_gateway;
    struct ifreq ifr;
    int s=-1;
    int one = 1;
    struct ecos_rtentry route;

    pre_gateway.sin_addr.s_addr = MApi_LAN_GetGateway();
    MApi_LAN_GatewayDel((MS_U8*)inet_ntoa(pre_gateway.sin_addr));

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket error");
        return FALSE;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one))) {
        perror("setsockopt error");
        goto out;
    }

    addrp = (struct sockaddr_in *) &ifr.ifr_addr;

    memset(addrp, 0, sizeof(*addrp));
    addrp->sin_family = AF_INET;
    addrp->sin_len = sizeof(*addrp);
    addrp->sin_port = 0;
    strcpy(ifr.ifr_name, "eth0");
    addrp->sin_addr.s_addr= MApi_LAN_GetIp((MS_U8*)"eth0");

    // Must do this temporarily with default route and netmask so that
    // [sub]netmask can be set.

    if (ioctl(s, SIOCSIFADDR, &ifr)) {
        perror("SIOCSIFADDR error");
        goto out;
    }

    addrp->sin_addr.s_addr= MApi_LAN_GetSubnet((MS_U8*)"eth0");
    if (ioctl(s, SIOCSIFNETMASK, &ifr)) {
        perror("SIOCSIFNETMASK error");
        goto out;
    }
    // Must do this again so that [sub]netmask (and so default route)
    // is taken notice of.
    addrp->sin_addr.s_addr= MApi_LAN_GetIp((MS_U8*)"eth0");
    if (ioctl(s, SIOCSIFADDR, &ifr)) {
        perror("SIOCSIFADDR error");
        goto out;
    }

    ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_RUNNING;
    if (ioctl(s, SIOCSIFFLAGS, &ifr)) {
        perror("SIOCSIFFLAGS error");
        goto out;
    }

    // Set up routing
    memset(&route, 0, sizeof(route));
    addrp->sin_family = AF_INET;
    addrp->sin_port = 0;
    addrp->sin_len = sizeof(*addrp);
    addrp->sin_addr.s_addr = 0; // Use 0,0,GATEWAY for the default route
    memcpy(&route.rt_dst, addrp, sizeof(*addrp));
    addrp->sin_addr.s_addr = 0;
    memcpy(&route.rt_genmask, addrp, sizeof(*addrp));
    inet_aton((char*)gateway, &addrp->sin_addr);
    memcpy(&route.rt_gateway, addrp, sizeof(*addrp));

    route.rt_dev = ifr.ifr_name;
    route.rt_flags = RTF_UP|RTF_GATEWAY;
    route.rt_metric = 0;
    if (ioctl(s, SIOCADDRT, &route)) {
        perror("SIOCADDRT error");
        goto out;
    }
    //set_eth0_interface_data(NULL, NULL, NULL, gateway, 0);
    //init_all_network_interfaces();
    close(s);
    return TRUE;
out:
    close(s);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Delete LAN Gateway
/// @param gateway \b IN: LAN gateway
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_GatewayDel(MS_U8* gateway)
{
       struct sockaddr_in addr;
	int s=-1;
	int one = 1;
	struct ecos_rtentry route;
	char* dev_name = "eth0";

	if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		return FALSE;
	}
	if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one))) {
		perror("setsockopt error");
		close(s);
		return FALSE;
	}

	memset(&addr, 0, sizeof(addr));
	memset(&route, 0, sizeof(route));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_len = sizeof(addr);
	addr.sin_addr.s_addr = 0;
	memcpy(&route.rt_dst, &addr, sizeof(addr));
	addr.sin_addr.s_addr = 0;
	memcpy(&route.rt_genmask, &addr, sizeof(addr));
	inet_aton((char*)gateway, &addr.sin_addr);
	memcpy(&route.rt_gateway, &addr, sizeof(addr));

	route.rt_dev = dev_name;
	route.rt_flags = RTF_UP | RTF_GATEWAY;
	route.rt_metric = 0;
	if (ioctl(s, SIOCDELRT, &route)) {
		close(s);
		return FALSE;
	}
	//set_eth0_interface_data(NULL, NULL, NULL, NULL, 0);
	close(s);
	//init_all_network_interfaces();
	return TRUE;
}

extern int PHY_Source;

//-------------------------------------------------------------------------------------------------
/// Set LAN Mac address
/// @param mac \b IN: Mac address
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_MacConfig(MS_U8* mac)
{
	int idx;
	struct ifreq ifr;

	if(!_MApi_LAN_GetIfParam(&ifr, SIOCGIFHWADDR)){
		return FALSE;
	}

	// Start to indicate new MAC address
	for ( idx = 0; idx < 6; idx++ )
		ifr.ifr_hwaddr.sa_data[idx] = mac[idx];

	printf( "Mac addr %02x:%02x:%02x:%02x:%02x:%02x\n",
								(MS_U8)ifr.ifr_hwaddr.sa_data[0],
								(MS_U8)ifr.ifr_hwaddr.sa_data[1],
								(MS_U8)ifr.ifr_hwaddr.sa_data[2],
								(MS_U8)ifr.ifr_hwaddr.sa_data[3],
								(MS_U8)ifr.ifr_hwaddr.sa_data[4],
								(MS_U8)ifr.ifr_hwaddr.sa_data[5] );

	if(_MApi_LAN_SetIfParam(ifr, SIOCSIFHWADDR)){
		if (PHY_Source) // Richard
		{
		    MDrv_EMAC_ChangeMacAddr(mac);
	    }
	    else //Jesse
	    {
//            MDrv_UsbCdc_ChangeMacAddr(mac);
	    }
	}

	return TRUE;

}

//-------------------------------------------------------------------------------------------------
/// Power on LAN
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_Poweron(void)
{
    EMAC_PowerOn_Set();
return(TRUE);
}
//-------------------------------------------------------------------------------------------------
/// Power off LAN
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_Poweroff(void)
{
    EMAC_PowerOff_Set();
return(TRUE);
}



//-------------------------------------------------------------------------------------------------
/// Show Interface Info
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_ShowIntf(void)
{
   MS_U32 curr_ip;
   MS_U32 curr_netmask;
   MS_U32 curr_gateway;
   MS_U8 curr_mac[6];

   curr_ip = MApi_LAN_GetIp((MS_U8*)"eth0");
   curr_gateway = MApi_LAN_GetGateway();
   curr_netmask = MApi_LAN_GetSubnet((MS_U8*)"eth0");
   MApi_LAN_GetMac((MS_U8*)"eth0", curr_mac);
   printf("IP = %08x\n", curr_ip);
   printf("Subnet = %08x\n", curr_netmask);
   printf("Gateway = %08x\n", curr_gateway);
   printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n",*curr_mac,*(curr_mac+1),*(curr_mac+2),*(curr_mac+3),
   	                                                                                  *(curr_mac+4),*(curr_mac+5));
   typedef void pr_fun(char *fmt, ...);
   externC void show_network_tables(pr_fun *pr);

   show_network_tables( (pr_fun *) diag_printf);

   return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Reset all Network interfaces
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_IntfReset(void)
{
   init_all_network_interfaces();
   eth0_up = 1;
   return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Check if ethernet is ready
/// @return TRUE - Ready
/// @return FALSE - Not Ready
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_EthReady(void)
{
   return CheckEMAC_Initialized();
}

//-------------------------------------------------------------------------------------------------
/// Check if DHCP is finished
/// @return 0 - DHCP IDLE
/// @return 1 -- DHCPSTATE_INIT
/// @return 2 -- DHCPSTATE_SELECTING
/// @return 3 -- DHCPSTATE_REQUESTING
/// @return 4 -- DHCPSTATE_REQUEST_RECV
/// @return 5 -- DHCPSTATE_BOUND
/// @return 6 -- DHCPSTATE_RENEWING
/// @return 7 -- DHCPSTATE_RENEW_RECV
/// @return 8 -- DHCPSTATE_REBINDING
/// @return 9 -- DHCPSTATE_REBIND_RECV
/// @return 10 -- DHCPSTATE_BOOTP_FALLBACK
/// @return 11 -- DHCPSTATE_NOTBOUND
/// @return 12 -- DHCPSTATE_FAILED
/// @return 13 -- DHCPSTATE_DO_RELEASE
/// @note
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_LAN_GetDhcpState(void)
{
   return eth0_dhcpstate;
}

//-------------------------------------------------------------------------------------------------
/// Ping command
/// @return TRUE - Ready
/// @return FALSE - Not Ready
/// @note
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_LAN_PingMultiple(MS_U8* target_addr, MS_U8 u8Count)
{
    struct protoent *p;
    struct timeval tv;
    struct sockaddr_in host;
    int socket_fd, iCount = 0;
    MS_U8 result=0;

    if ((p = getprotobyname("icmp")) == (struct protoent *)0) {
        perror("getprotobyname");
        return 0;
    }

    for(iCount = 0; iCount < u8Count; iCount++)
    {

        socket_fd = socket(AF_INET, SOCK_RAW, p->p_proto);
        if (socket_fd < 0) {
            perror("socket");
            return 0;
        }
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        // Set up host address
        host.sin_family = AF_INET;
        host.sin_len = sizeof(host);
        inet_aton((char*)target_addr, &host.sin_addr);
        host.sin_port = 0;

        result = _MApi_LAN_PingHost(socket_fd, &host, iCount);

        close(socket_fd);
        socket_fd = -1;
    }

    return result;
}

MS_U8 MApi_LAN_Ping(MS_U8* target_addr)
{
    return MApi_LAN_PingMultiple(target_addr, 1);
}


MS_BOOL MApi_LAN_ConnectStatus(void)
{
    if (PHY_ConnectStatus)
    {
        return PHY_ConnectStatus();
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LAN_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size)
{
    MS_U8 u8Name[INET_ADDRSTRLEN];

    if(u32Size < sizeof(u8Name))
    {
        return FALSE;
    }

    if(pu8Server1)
    {
        memset(u8Name, 0, sizeof(u8Name));
        if(inet_ntop(AF_INET, (char *)&dns_server[0], (char *)pu8Server1, u32Size) == NULL)
            return FALSE;
    }

    if(pu8Server2)
    {
        memset(u8Name, 0, sizeof(u8Name));
        if(inet_ntop(AF_INET, (char *)&dns_server[1], (char *)pu8Server2, u32Size) == NULL)
            return FALSE;
    }

    return TRUE;
}
//-------------------------------------------------------------------------------------------------
void MApi_LAN_SetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2)
{
    if(pu8Server1)
    {
        inet_pton(AF_INET, (char*)pu8Server1, (void *)&dns_server[0]);

    }
    if(pu8Server2)
    {
        inet_pton(AF_INET, (char*)pu8Server2, (void *)&dns_server[1]);

    }
}
//-------------------------------------------------------------------------------------------------
struct hostent *MApi_LAN_gethostbyname(MS_U8* name)
{
    struct hostent *hent;
    setdomainname(NULL,0);

    //Check DNS server1
    cyg_dns_res_init((struct in_addr *)&dns_server[0]);
    hent = gethostbyname((char*)name);
    if(hent) goto MApi_LAN_gethostbyname_done;

    //Check DNS server2
    cyg_dns_res_init((struct in_addr *)&dns_server[1]);
    hent = gethostbyname((char*)name);

MApi_LAN_gethostbyname_done:
    //restore server1
    cyg_dns_res_init((struct in_addr *)&dns_server[0]);
    return hent;
}
//------------------------------------------------------------------------------
MS_U8 MAPI_WLAN_GetApList(MS_U32* u32ap_list_array, MS_U8 u8max_num) //return num of AP;
{
    return MAPI_IEEE802_11_GetApList(u32ap_list_array, u8max_num);
}
//------------------------------------------------------------------------------
void MAPI_WLAN_Join_By_Name(MS_U8* pu8Name)
{
    MAPI_IEEE802_11_Join_By_Name(pu8Name);
}
//------------------------------------------------------------------------------
void MAPI_WLAN_Auth_Mode(MS_U8 mode, MS_U8* password, MS_U8 pw_len)
{
    MAPI_IEEE802_11_Auth_Mode(mode, password, pw_len);
}

