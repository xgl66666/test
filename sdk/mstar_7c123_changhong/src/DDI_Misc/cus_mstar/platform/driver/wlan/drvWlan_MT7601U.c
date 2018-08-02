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

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvWlan_MT7601U.c
/// @brief  MT7601U Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <ctype.h>
#include "MsCommon.h"
#include "drvWlan.h"
#include "drvWlanNull.h"
#include "apiWLAN.h"
#include "drvWlan_MT7601U.h"
#include <network.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <fcntl.h>
#include <dhcp.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <pkgconf/ns_dns.h>

//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
externC int get_current_dhcp_mode(void);
externC void set_current_dhcp_mode(int val);
externC MS_U32 get_default_gateway(void);
extern unsigned long dns_server[2];

typedef void (*device_Callback)(MS_U8 u8Event);
extern int CmdIwpriv(int argc, char *argv[]);
extern int get_wifi_connect_status(void);
extern int get_scan_list(char *ifname,char * buf,int len);
extern int get_wifi_radio_status(void);
extern int usb_wifi_init(device_Callback);
extern int get_connected_ap_bssid(char * buf,int len);



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
drv_wlanop_Callback _pfEventCallback;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _trim(char *c,int arrayLen)
{
    int idx=0;

    for(idx=arrayLen-1;idx>=0;idx--)
    {
        if(!isspace(*(c+idx)))
        {
            *(c+idx+1)= '\0';
            break;
        }
    }
}

void _lower_string(char *s)
{
   int c = 0;
 
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
}

static int _MDrv_Wlan_MT7601U_TicksToMs(MS_U32 u32Ticks)
{
    int ticks_per_second = 1000000000/(CYGNUM_HAL_RTC_NUMERATOR/CYGNUM_HAL_RTC_DENOMINATOR);
    int ms = (u32Ticks * 1000)/ticks_per_second;
    return ms;
}


// Compute INET checksum
static int _MDrv_Wlan_MT7601U_NetCksum(MS_U16* addr, int len)
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
    while( nleft > 1 )
    {
        sum += *w;
        w ++;
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



static int _MDrv_Wlan_MT7601U_ShowIcmp(unsigned char *pkt, int len,
          struct sockaddr_in *from, struct sockaddr_in *to)
{
    struct icmp *icmp;
    cyg_tick_count_t *tp;
    struct ip *ip;
    cyg_tick_count_t tv;

    ip = (struct ip *)pkt;//pointer to IP header

    tv = cyg_current_time();// get current time

    if ((len < sizeof(*ip)) || ip->ip_v != IPVERSION) {//check packet length and IP vertion
#if PING_LOG
        diag_printf("%s: Short packet or not IP! - Len: %d, Version: %d\n",
                    inet_ntoa(from->sin_addr), len, ip->ip_v);
#endif
        return 0;
    }
    icmp = (struct icmp *)(pkt + sizeof(*ip));
    len -= (sizeof(*ip) + 8);
    tp = (cyg_tick_count_t *)&icmp->icmp_data;
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
#if PING_LOG
        diag_printf("%s: Invalid ICMP - type: %d\n",
                    inet_ntoa(from->sin_addr), icmp->icmp_type);
#endif

        return 0;
    }
    if (icmp->icmp_id != 0x1234) {
        diag_printf("%s: ICMP received for wrong id - sent: %x, recvd: %x\n",
                    inet_ntoa(from->sin_addr), 0x1234, icmp->icmp_id);
    }
#if PING_LOG
    diag_printf("%d bytes from %s: ", len, inet_ntoa(from->sin_addr));
    diag_printf("icmp_seq=%d", icmp->icmp_seq);
    diag_printf(", time=%dms\n", _MDrv_Wlan_MT7601U_TicksToMs(tv - *tp));
#endif
    return (from->sin_addr.s_addr == to->sin_addr.s_addr);
}


static MS_U8 _MDrv_Wlan_MT7601U_PingHost(int socket_fd, struct sockaddr_in *host, MS_U16 u16Seq)
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
    *tp= cyg_current_time();
    tp ++;
    dp = (long *)tp;
    for (i = sizeof(*tp);  i < icmp_len;  i += sizeof(*dp))
    {
        *dp = i;
        dp ++;
    }
    // Add checksum
    icmp->icmp_cksum = _MDrv_Wlan_MT7601U_NetCksum( (u_short *)icmp, icmp_len+8);
    // Send it off
    if (sendto(socket_fd, icmp, icmp_len+8, 0, (struct sockaddr *)host, sizeof(*host)) < 0) {
#if  PING_LOG
        perror("sendto");
#endif
        return 0;
    }
    // Wait for a response
    fromlen = sizeof(from);
    len = recvfrom(socket_fd, ping_in_pkt, sizeof(ping_in_pkt), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
    if (len < 0) {
#if  PING_LOG
        perror("recvfrom");
#endif
    }
    else {
        if (_MDrv_Wlan_MT7601U_ShowIcmp(ping_in_pkt, len, &from, host))
        {
            ok_recv++;
            result = 1;
        }
        else
            bogus_recv++;
    }
    return result;
}

void _MDrv_Wlan_MT7601U_Callback(MS_U8 u8Event)
{
    if(u8Event == 0x10)
    {
        _pfEventCallback(WLAN_MT7601U,WLAN_EN_INSERTED);
    }
    else if(u8Event == 0x11)
    {
        _pfEventCallback(WLAN_MT7601U,WLAN_EN_REMOVED);
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Wlan_MT7601U_Init(drv_wlanop_Callback pfEventCallback)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    _pfEventCallback = pfEventCallback;
    usb_wifi_init(_MDrv_Wlan_MT7601U_Callback);
    init_all_network_interfaces();
    ra0_up = 1;
    return TRUE;
}


MS_BOOL MDrv_Wlan_MT7601U_Config(WLAN_MS_WIFI_CONFIG stWlanConfig)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    char *cmd_args[]={"ra0","set","xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};

    //SSID = empty string for Stop WIFI connect
    if(strcmp(stWlanConfig.ssid,"")==0)
    {
        strcpy(cmd_args[2],"SSID=");
        if(!CmdIwpriv(3,cmd_args))
        {
           diag_printf("Set %s Fail\n",cmd_args[2]);
           return FALSE;
        }

        return TRUE;
    } //judge SSID = empty string


    switch(stWlanConfig.network_type)
    {
        case INFA:
        {
            //<<Send cmd NetworkType=Infra>>
            strcpy(cmd_args[2],"NetworkType=Infra");
            if(!CmdIwpriv(3,cmd_args))
            {
                diag_printf("Set %s Fail\n",cmd_args[2]);
                return FALSE;
            }

            switch(stWlanConfig.auth_mode)
            {
                case OPEN:    //For open system
                {
                    //<<Send cmd AuthMode=OPEN>>
                    strcpy(cmd_args[2],"AuthMode=OPEN");
                    if(!CmdIwpriv(3,cmd_args))
                    {
                        diag_printf("Set %s Fail\n",cmd_args[2]);
                        return FALSE;
                    }

                    switch(stWlanConfig.encrypt_type)
                    {
                       case NONE: //for AuthMode=OPEN
                       {
                           //<<Send cmd EncrypType=NONE>>
                           strcpy(cmd_args[2],"EncrypType=NONE");
                           if(!CmdIwpriv(3,cmd_args))
                           {
                               diag_printf("Set %s Fail\n",cmd_args[2]);
                               return FALSE;
                           }
                           break;
                       }
                       case WEP:  //for AuthMode=OPEN or AuthMode=SHARED
                       {
                           //<<Send cmd EncrypType=WEP>>
                           strcpy(cmd_args[2],"EncrypType=WEP");
                           if(!CmdIwpriv(3,cmd_args))
                           {
                              diag_printf("Set %s Fail\n",cmd_args[2]);
                               return FALSE;
                           }

                           switch( stWlanConfig.default_keyid )
                           {
                               case 1:
                               {
                                   //<<Send cmd Key1=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key1=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 2:
                               {
                                   //<<Send cmd Key2=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key2=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 3:
                               {
                                   //<<Send cmd Key3=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key3=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 4:
                               {
                                   //<<Send cmd Key4=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key4=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }
                               default:
                               {
                                   diag_printf("Wrong default_keyid, default_keyid range 1~4\n");
                                   return FALSE;
                               }
                           }
                           break;
                       }

                       case TKIP: //for AuthMode=WPAPSK or WPA2PSK
                       case AES:  //for AuthMode=WPAPSK or WPA2PSK
                       default:
                       {
                           diag_printf("Wrong EncrypType setting \n");
                           return FALSE;
                       }
                    }
                    break;
                }

                case SHARED:  //For shared key system
                {
                    //<<Send cmd AuthMode=SHARED>>
                    strcpy(cmd_args[2],"AuthMode=SHARED");
                    if(!CmdIwpriv(3,cmd_args))
                    {
                        diag_printf("Set %s Fail\n",cmd_args[2]);
                        return FALSE;
                    }

                    switch(stWlanConfig.encrypt_type)
                    {
                       case WEP:  //for AuthMode=OPEN or AuthMode=SHARED
                       {
                           //<<Send cmd EncrypType=WEP>>
                           strcpy(cmd_args[2],"EncrypType=WEP");
                           if(!CmdIwpriv(3,cmd_args))
                           {
                               diag_printf("Set %s Fail\n",cmd_args[2]);
                               return FALSE;
                           }

                           switch( stWlanConfig.default_keyid )
                           {
                               case 1:
                               {
                                   //<<Send cmd Key1=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key1=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 2:
                               {
                                   //<<Send cmd Key2=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key2=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 3:
                               {
                                   //<<Send cmd Key3=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key3=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 4:
                               {
                                   //<<Send cmd Key4=stWlanConfig->key>>>
                                   strcpy(cmd_args[2],"Key4=");
                                   strcat(cmd_args[2],(char *)stWlanConfig.key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }
                               default:
                               {
                                   diag_printf("Wrong default_keyid, default_keyid range 1~4\n");
                                   return FALSE;
                               }
                           }
                           break;
                       }

                       case NONE: //for AuthMode=OPEN
                       case TKIP: //for AuthMode=WPAPSK or WPA2PSK
                       case AES:  //for AuthMode=WPAPSK or WPA2PSK
                       default:
                       {
                           diag_printf("Wrong EncrypType setting \n");
                           return FALSE;
                       }
                    }

                    break;
                }

                case WPAPSK:  //For WPA pre-shared key (Infra)
                case WPA2PSK: //For WPA2 pre-shared key (Infra)
                {
                    //<<Send cmd AuthMode=WPAPSK>>
                    //0R
                    //<<Send cmd AuthMode=WPA2PSK>>
                    if(stWlanConfig.auth_mode==WPAPSK)
                    strcpy(cmd_args[2],"AuthMode=WPAPSK");

                    if(stWlanConfig.auth_mode==WPA2PSK)
                    strcpy(cmd_args[2],"AuthMode=WPA2PSK");

                    if(!CmdIwpriv(3,cmd_args))
                    {
                        diag_printf("Set %s Fail\n",cmd_args[2]);
                        return FALSE;
                    }

                    switch(stWlanConfig.encrypt_type)
                    {
                       case TKIP: //for AuthMode=WPAPSK or WPA2PSK
                       {
                           //<<Send cmd EncrypType=TKIP>>
                           strcpy(cmd_args[2],"EncrypType=TKIP");
                           if(!CmdIwpriv(3,cmd_args))
                           {
                               diag_printf("Set %s Fail\n",cmd_args[2]);
                               return FALSE;
                           }
                           break;
                       }

                       case AES:  //for AuthMode=WPAPSK or WPA2PSK
                       {
                           //<<Send cmd EncrypType=AES>>
                           strcpy(cmd_args[2],"EncrypType=AES");
                           if(!CmdIwpriv(3,cmd_args))
                           {
                               diag_printf("Set %s Fail\n",cmd_args[2]);
                               return FALSE;
                           }

                           break;
                       }

                       case NONE: //for AuthMode=OPEN
                       case WEP:  //for AuthMode=OPEN or AuthMode=SHARED
                       default:
                       {
                           diag_printf("Wrong EncrypType setting \n");
                           return FALSE;
                       }
                    }

                    //<<Send cmd WPAPSK=stWlanConfig->key>>
                    strcpy(cmd_args[2],"WPAPSK=");
                    strcat(cmd_args[2],(char *)stWlanConfig.key);
                    if(!CmdIwpriv(3,cmd_args))
                    {
                        diag_printf("Set %s Fail\n",cmd_args[2]);
                        return FALSE;
                    }

                    break;
                }

                case WEPAUTO: //For Auto switch between OPEN and SHARED
                case WPANONE: //For WPA pre-shared key (Adhoc)
                case WPA:     //For enterprise mode (Need wpa_supplicant)
                case WPA2:    //For enterprise mode (Need wpa_supplicant)
                default:
                {
                    diag_printf("Not Supported AuthMode \n");
                    return FALSE;
                }
            }

            break;
        }

        case ADHOC:
        default:
        {
            diag_printf("Not Supported NetworkType \n");
            return FALSE;
        }

    }

    //<<Send cmd SSID=stWlanConfig->ssid>>
    strcpy(cmd_args[2],"SSID=");
    strcat(cmd_args[2],(char *)stWlanConfig.ssid);
    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_Wlan_MT7601U_RadioOn(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    char *cmd_args[]={"ra0","set","RadioOn=1"};

    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_Wlan_MT7601U_RadioOff(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    char *cmd_args[]={"ra0","set","RadioOn=0"};

    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_Wlan_MT7601U_GetAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    char ap_list_buff[AP_LIST_BUFFER_SIZE];//AP list in list[]
    char ap_list_row[108];
    ap_list_row[107]='\0';
    int ret=0;
    int i=0;
    int offset=0;
    WLAN_MS_ACCESS_POINT* pAP=NULL;
    MS_U8 NumOfAP=0;

    memset(ap_list_buff,0,AP_LIST_BUFFER_SIZE);
    ret=get_scan_list("ra0",ap_list_buff,AP_LIST_BUFFER_SIZE);
    if(!ret)
    {
        diag_printf("[%s]Get AP List Fail\n",__FUNCTION__);
        return FALSE;
    }
    diag_printf("LEN:%d\n",strlen(ap_list_buff));

    NumOfAP=(strlen(ap_list_buff)/107)-1;

    (*ppAPList)=(WLAN_MS_ACCESS_POINT_LIST*)malloc(sizeof(WLAN_MS_ACCESS_POINT_LIST)+NumOfAP*sizeof(WLAN_MS_ACCESS_POINT));
    (*ppAPList)->NumberOfAP=NumOfAP;

    //AP List Parsing
    for(i=0,offset=107;i<(*ppAPList)->NumberOfAP;i++,offset+=107)
    {
       pAP=(*ppAPList)->AP+i;

       memcpy(ap_list_row,(char *)(ap_list_buff+offset+1), 107);

       //Skip The AP Which No SSID
       if(*(ap_list_row+4)==' ')
       {
           (*ppAPList)->NumberOfAP--;
           i--;
           continue;
       }

       memcpy((char *)pAP->ssid    ,(char *)(ap_list_row+4),33);
       _trim((char *)pAP->ssid,33);


       memcpy((char *)pAP->channel ,(char *)(ap_list_row),3);
       _trim((char *)pAP->channel,3);


       memcpy((char *)pAP->mac     ,(char *)(ap_list_row+4+33),20);
       _trim((char *)pAP->mac,20);


       memcpy((char *)pAP->security,(char *)(ap_list_row+4+33+20),23);
       _trim((char *)pAP->security,23);

       memcpy((char *)pAP->signal  ,(char *)(ap_list_row+4+33+20+23),9);
       _trim((char *)pAP->signal,9);

       memcpy((char *)pAP->mode    ,(char *)(ap_list_row+4+33+20+23+9),7);
       _trim((char *)pAP->mode,7);

       memcpy((char *)pAP->ext_ch  ,(char *)(ap_list_row+4+33+20+23+9+7+1),7);
       _trim((char *)pAP->ext_ch,7);

       memcpy((char *)pAP->nt      ,(char *)(ap_list_row+4+33+20+23+9+7+1+7),3);
       _trim((char *)pAP->nt,3);


#if 0
       diag_printf("channel:%s:%d\nssid:%s:%d\nmac:%s:%d\nsecurity:%s:%d\nsiganl:%s:%d\nmode:%s:%d\next_ch:%s:%d\nnt:%s:%d\n",
        (char *)AP->channel,strlen(AP->channel),
        (char *)AP->ssid,strlen(AP->ssid),
        (char *)AP->mac,strlen(AP->mac),
        (char *)AP->security,strlen(AP->security),
        (char *)AP->siganl,strlen(AP->siganl),
        (char *)AP->mode,strlen(AP->mode),
        (char *)AP->ext_ch,strlen(AP->ext_ch),
        (char *)AP->nt,strlen(AP->nt)
        );
       diag_printf("\n\n\n\n\n\n");
#endif

    }

    return TRUE;

}

void MDrv_Wlan_MT7601U_ReleaseAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    free(*ppAPList);
    *ppAPList=NULL;
    return;
}

MS_S8 MDrv_Wlan_MT7601U_ConnectStatus(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return (MS_S8)get_wifi_connect_status();
}

MS_U32 MDrv_Wlan_MT7601U_GetIp(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    int s;
    struct sockaddr_in * ip_p;
    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
       perror("Socket error");
       return FALSE;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name,"ra0");

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

MS_U32 MDrv_Wlan_MT7601U_GetSubnet(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    int s;
    struct sockaddr_in* netmask_p;
    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
       perror("Socket error");
       return FALSE;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name,"ra0");

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

MS_U32 MDrv_Wlan_MT7601U_GetGateway(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return get_default_gateway();
}

MS_BOOL MDrv_Wlan_MT7601U_GetMac(MS_U8* mac)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    int s;

    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket error");
        return FALSE;
    }

    struct ifreq ifr;

    strcpy(ifr.ifr_name,"ra0");

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

MS_BOOL MDrv_Wlan_MT7601U_IpConfig(MS_U8* address)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    int s=0;
    struct sockaddr_in* addr=NULL;
    struct ifreq ifr;

    s = socket( PF_INET, SOCK_DGRAM, 0 );
    if( s == -1 ){
        perror("socket error");
        return FALSE;
    }

    memset(&ifr,0,sizeof( struct ifreq ) );
    strcpy(ifr.ifr_name,"ra0");

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
    strcpy(ifr.ifr_name,"ra0");

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

MS_BOOL MDrv_Wlan_MT7601U_SubnetConfig(MS_U8* subnet)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
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
    strcpy(ifr.ifr_name,"ra0");

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

MS_BOOL MDrv_Wlan_MT7601U_GatewaySet(MS_U8* gateway)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    struct sockaddr_in* addrp;
    struct sockaddr_in pre_gateway;
    struct ifreq ifr;
    int s=-1;
    int one = 1;
    struct ecos_rtentry route;

    pre_gateway.sin_addr.s_addr = MDrv_Wlan_MT7601U_GetGateway();
    MApi_WLAN_GatewayDel((MS_U8*)inet_ntoa(pre_gateway.sin_addr));

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
    addrp->sin_addr.s_addr= MDrv_Wlan_MT7601U_GetIp();

    // Must do this temporarily with default route and netmask so that
    // [sub]netmask can be set.

    if (ioctl(s, SIOCSIFADDR, &ifr)) {
        perror("SIOCSIFADDR error");
        goto out;
    }

    addrp->sin_addr.s_addr= MDrv_Wlan_MT7601U_GetSubnet();
    if (ioctl(s, SIOCSIFNETMASK, &ifr)) {
        perror("SIOCSIFNETMASK error");
        goto out;
    }
    // Must do this again so that [sub]netmask (and so default route)
    // is taken notice of.
    addrp->sin_addr.s_addr= MDrv_Wlan_MT7601U_GetIp();
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

MS_BOOL MDrv_Wlan_MT7601U_GatewayDel(MS_U8* gateway)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    struct sockaddr_in addr;
	int s=-1;
	int one = 1;
	struct ecos_rtentry route;
	char* dev_name = "ra0";

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

MS_BOOL MDrv_Wlan_MT7601U_MacConfig(MS_U8* mac)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    WLAN_ERR(("MT7601U don't support MacConfig \n"));
    return FALSE;
}

MS_BOOL MDrv_Wlan_MT7601U_DhcpConfig(MS_BOOL on)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    int s=0;
    struct ifreq ifr;

    s = socket( PF_INET, SOCK_DGRAM, 0 );
    if( s == -1 ){
        printf("[%s][%d]socket error\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    memset(&ifr,0,sizeof( struct ifreq ) );
    strcpy(ifr.ifr_name,"ra0");

    if(on)
    {
        ra0_up = 1;
        ra0_dhcpstate = 0;

        if( ioctl( s, SIOCGIFADDR, &ifr ) == 0 ){

            if( ioctl( s, SIOCDIFADDR, &ifr ) != 0 ){
                printf("[%s][%d]ioctrl error\n",__FUNCTION__,__LINE__);
                close(s);
                return FALSE;
            }
        }

        cyg_semaphore_post( &dhcp_needs_attention );
	}
	else // turn off DHCP mode
    {
        ra0_up = 0;

	    if(ra0_dhcpstate != DHCPSTATE_FAILED)
        {
            ra0_dhcpstate = DHCPSTATE_BOOTP_FALLBACK;
            cyg_semaphore_post( &dhcp_needs_attention );
            MsOS_DelayTask(100);
    	}
        else
        {
            ra0_dhcpstate = DHCPSTATE_BOOTP_FALLBACK;
            cyg_semaphore_post( &dhcp_needs_attention );
        }
    }

    close(s);
    init_loopback_interface(0);

    return TRUE;
}

MS_U8 MDrv_Wlan_MT7601U_GetDhcpState(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return ra0_dhcpstate;
}

MS_BOOL MDrv_Wlan_MT7601U_IntfReset(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    init_all_network_interfaces();
    ra0_up = 1;
    
    return TRUE;
    
}

MS_BOOL MDrv_Wlan_MT7601U_Scan(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    //Triger Scan
    char *cmd_args[]={"ra0","set","SiteSurvey"};
    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }
    MsOS_DelayTask(100);
    return TRUE;
}

MS_BOOL MDrv_Wlan_MT7601U_IsSecured(WLAN_MS_ACCESS_POINT AP)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return strcmp((char *)AP.security,"NONE");
}

MS_U8 MDrv_Wlan_MT7601U_Ping(char* target_addr)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    struct protoent *p;
    struct timeval tv;
    struct sockaddr_in host;
    int socket_fd, iCount = 0;
    MS_U8 result=0;
    MS_U8 u8Count=1;

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

        result = _MDrv_Wlan_MT7601U_PingHost(socket_fd, &host, iCount);

        close(socket_fd);
        socket_fd = -1;
    }

    return result;
}

MS_BOOL MDrv_Wlan_MT7601U_IsRadioOn(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    if(get_wifi_radio_status())
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MDrv_Wlan_MT7601U_GetConnetedAPBSSID(char * bssid,int len)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    
    int ret;

    ret=get_connected_ap_bssid(bssid,len);
    _lower_string(bssid);

    if(ret)
        return TRUE;
    else
        return FALSE;    
}

MS_BOOL MDrv_Wlan_MT7601U_Extension_Function(WLAN_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}



DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_MT7601U) DDI_DRV_WLAN_TABLE_ENTRY(wlantab) =      
{                                                                       
    .name               = "WLAN_MT7601U",          // demod system name
    .data               = WLAN_MT7601U,            // private data value
    .Init               = MDrv_Wlan_MT7601U_Init,
    .Config             = MDrv_Wlan_MT7601U_Config,
    .RadioOn            = MDrv_Wlan_MT7601U_RadioOn,
    .RadioOff           = MDrv_Wlan_MT7601U_RadioOff,
    .GetAPList          = MDrv_Wlan_MT7601U_GetAPList,
    .ReleaseAPList      = MDrv_Wlan_MT7601U_ReleaseAPList,
    .ConnectStatus      = MDrv_Wlan_MT7601U_ConnectStatus,
    .GetIp              = MDrv_Wlan_MT7601U_GetIp,
    .GetSubnet          = MDrv_Wlan_MT7601U_GetSubnet,
    .GetGateway         = MDrv_Wlan_MT7601U_GetGateway,
    .GetMac             = MDrv_Wlan_MT7601U_GetMac,
    .IpConfig           = MDrv_Wlan_MT7601U_IpConfig,
    .SubnetConfig       = MDrv_Wlan_MT7601U_SubnetConfig,
    .GatewaySet         = MDrv_Wlan_MT7601U_GatewaySet,
    .GatewayDel         = MDrv_Wlan_MT7601U_GatewayDel,
    .MacConfig          = MDrv_Wlan_MT7601U_MacConfig,
    .DhcpConfig         = MDrv_Wlan_MT7601U_DhcpConfig,
    .GetDhcpState       = MDrv_Wlan_MT7601U_GetDhcpState,
    .IntfReset          = MDrv_Wlan_MT7601U_IntfReset,
    .Scan               = MDrv_Wlan_MT7601U_Scan,
    .IsSecured          = MDrv_Wlan_MT7601U_IsSecured,
    .Ping               = MDrv_Wlan_MT7601U_Ping,
    .IsRadioOn          = MDrv_Wlan_MT7601U_IsRadioOn,
    .GetConnetedAPBSSID = MDrv_Wlan_MT7601U_GetConnetedAPBSSID,
    .Extension_Function = MDrv_Wlan_MT7601U_Extension_Function
};

