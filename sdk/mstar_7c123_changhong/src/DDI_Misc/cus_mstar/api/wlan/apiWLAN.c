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
/// file   apiWLAN.c
/// @brief  WLAN API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1//(OTT_ENABLE == 1)
#include <network.h>
#if CYGHWR_NET_DRIVER_RA0

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_ECOS)
#include "Board.h"
#include "MsCommon.h"
#include <network.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <fcntl.h>
#include <dhcp.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <pkgconf/ns_dns.h>

#include "apiWLAN.h"

//-------------------------------------------------------------------------------------------------
//  extern reference
//-------------------------------------------------------------------------------------------------
externC int get_current_dhcp_mode(void);
externC void set_current_dhcp_mode(int val);
externC MS_U32 get_default_gateway(void);
extern unsigned long dns_server[2];


extern int CmdIwpriv(int argc, char *argv[]);
extern int get_wifi_connect_status(void);
extern int get_scan_list(char *ifname,char * buf,int len);
extern int get_wifi_radio_status(void);


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
struct drv_wlantab_entry* gWlanTab=NULL;


//--------------------------------------------------------------------------------------------------
//  Global Functions
//--------------------------------------------------------------------------------------------------
#if WLAN_SUPPORT_NUMBER > 0
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_00TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 1
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_01ST_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 2
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_02ND_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 3
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_03RD_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 4
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_04TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 5
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_05TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 6
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_06TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 7
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_07TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 8
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_08TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 9
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_09TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 10
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_10TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 11
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_11TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 12
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_12TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 13
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_13TH_SUPPORT_DEVICE);
#endif
#if WLAN_SUPPORT_NUMBER > 14
extern DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_14TH_SUPPORT_DEVICE);
#endif

DRV_WLAN_TABLE_TYPE* gp_wlan_entry[WLAN_SUPPORT_NUMBER]=
{
#if WLAN_SUPPORT_NUMBER > 0
    &GET_WLAN_ENTRY_NODE(WLAN_00TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 1
    ,&GET_WLAN_ENTRY_NODE(WLAN_01ST_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 2
    ,&GET_WLAN_ENTRY_NODE(WLAN_02ND_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 3
    ,&GET_WLAN_ENTRY_NODE(WLAN_03RD_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 4
    ,&GET_WLAN_ENTRY_NODE(WLAN_04TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 5
    ,&GET_WLAN_ENTRY_NODE(WLAN_05TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 6
    ,&GET_WLAN_ENTRY_NODE(WLAN_06TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 7
    ,&GET_WLAN_ENTRY_NODE(WLAN_07TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 8
    ,&GET_WLAN_ENTRY_NODE(WLAN_08TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 9
    ,&GET_WLAN_ENTRY_NODE(WLAN_09TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 10
    ,&GET_WLAN_ENTRY_NODE(WLAN_10TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 11
    ,&GET_WLAN_ENTRY_NODE(WLAN_11TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 12
    ,&GET_WLAN_ENTRY_NODE(WLAN_12TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 13
    ,&GET_WLAN_ENTRY_NODE(WLAN_13TH_SUPPORT_DEVICE)
#endif
#if WLAN_SUPPORT_NUMBER > 14
    ,&GET_WLAN_ENTRY_NODE(WLAN_14TH_SUPPORT_DEVICE)
#endif
};

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL _gbInOperation = FALSE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
void _MApi_WLAN_Callback(MS_U32 u32Module, WLAN_EN_DeviceEvent eEvent)
{
    int i;
    if(eEvent == WLAN_EN_INSERTED)
    {
        printf("DEVICE %d INSERTED\n",u32Module);
        if(_gbInOperation == FALSE)
        {
            for(i=0;i<WLAN_SUPPORT_NUMBER;i++)
            {
                if(gp_wlan_entry[i]->data == u32Module)
                {
                    printf("DEVICE %d ENABLED\n",u32Module);
                    gWlanTab = gp_wlan_entry[i];
                    _gbInOperation = TRUE;
                }
            }
        }
    }
    if(eEvent == WLAN_EN_REMOVED)
    {
        printf("DEVICE %d REMOVED\n",u32Module);
        gWlanTab = NULL;
        _gbInOperation = FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
//  Functions Prototypes
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// WLAN initialize
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Init(void)
{
    int i=0;
    for(i=0;i<WLAN_SUPPORT_NUMBER;i++)
    {
        gp_wlan_entry[i]->Init(_MApi_WLAN_Callback);
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// WLAN Configuration and connect to AP
/// @param wifi configuration \b IN: NULL for Stop WIFI connect
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Config (WIFI_CONFIG wifi_config)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    WLAN_MS_WIFI_CONFIG stWifiConfig;
    memset(&stWifiConfig,0,sizeof(WLAN_MS_WIFI_CONFIG));
    memcpy(&stWifiConfig,&wifi_config,sizeof(WLAN_MS_WIFI_CONFIG));
    return gWlanTab->Config(stWifiConfig);
}


//-------------------------------------------------------------------------------------------------
/// WLAN Radio On
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_RadioOn(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->RadioOn();
}


//-------------------------------------------------------------------------------------------------
/// WLAN Radio Off
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_RadioOff(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->RadioOff();
}

//-------------------------------------------------------------------------------------------------
/// WLAN WiFi Scan Trigger
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Scan(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->Scan();
}

//-------------------------------------------------------------------------------------------------
/// Check Is AP Secured
/// @return TRUE
/// @return FALSE
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_IsSecured(WLAN_ACCESS_POINT AP)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    WLAN_MS_ACCESS_POINT stWlanAp;
    memset(&stWlanAp,0,sizeof(WLAN_MS_ACCESS_POINT));
    memcpy(&stWlanAp, &AP, sizeof(WLAN_MS_ACCESS_POINT));
    return gWlanTab->IsSecured(stWlanAp);

}


//-------------------------------------------------------------------------------------------------
/// WLAN Get&Parse WiFi AP List
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GetAPList(PWLAN_ACCESS_POINT_LIST *ppAPList)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->GetAPList((WLAN_MS_ACCESS_POINT_LIST**)ppAPList);
}


//-------------------------------------------------------------------------------------------------
/// WLAN Release&Free WiFi AP List
//-------------------------------------------------------------------------------------------------
void MApi_WLAN_ReleaseAPList(PWLAN_ACCESS_POINT_LIST *ppAPList)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return;
    }
    gWlanTab->ReleaseAPList((WLAN_MS_ACCESS_POINT_LIST**)ppAPList);
}

//-------------------------------------------------------------------------------------------------
/// WLAN Get ConnectStatus
///Return value
///-1 :init fail
///0 : init value
///1 : Authancation on going
///12:Authancation fail
///13:Authancation success
///2 : Assocation on going
///22:Assocation fail
///23:Assocation success
///3 : 4-way handle shark on going (this state if for wpapsk&wpa2psk only)
///32:4-way handle shark fail,key fail
///33:4-way handle shark success
///4 : connected
//-------------------------------------------------------------------------------------------------
MS_S8 MApi_WLAN_ConnectStatus(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return -1;
    }
    return gWlanTab->ConnectStatus();
}

//-------------------------------------------------------------------------------------------------
/// Check Radio On/Off
/// @return TRUE - RadioOn
/// @return FALSE - RadioOff
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_IsRadioOn(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->IsRadioOn();
}


//-------------------------------------------------------------------------------------------------
/// Get current IP configuration
/// @param intf_name \b IN: Interface name
/// @return others - Current IP value
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_WLAN_GetIp(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return 0;
    }
    return gWlanTab->GetIp();
}

//-------------------------------------------------------------------------------------------------
/// Get current Subnet configuration
/// @param intf_name \b IN: Interface name
/// @return others - Current Subnet value
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_WLAN_GetSubnet(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return 0;
    }
    return gWlanTab->GetSubnet();
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Set WLAN IP
/// @param address \b IN: WLAN IP
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_IpConfig(MS_U8* address)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->IpConfig(address);
}

//-------------------------------------------------------------------------------------------------
/// Set WLAN Subnet
/// @param subnet \b IN: subnet
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_SubnetConfig(MS_U8* subnet)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->SubnetConfig(subnet);
}

//-------------------------------------------------------------------------------------------------
/// Get current Gateway configuration
/// @param void \b IN: NONE
/// @return others - Current gateway value
/// @return 0 - No gateway specified
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_WLAN_GetGateway(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return 0;
    }
    return gWlanTab->GetGateway();
}

//-------------------------------------------------------------------------------------------------
/// Set WLAN Gateway
/// @param gateway \b IN: WLAN gateway
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GatewaySet(MS_U8* gateway)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->GatewaySet(gateway);
}
//-------------------------------------------------------------------------------------------------
/// Delete WLAN Gateway
/// @param gateway \b IN: WLAN gateway
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GatewayDel(MS_U8* gateway)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->GatewayDel(gateway);
}

//-------------------------------------------------------------------------------------------------
/// Get current MAC configuration
/// @param mac \b IN: Storage to put current MAC
/// @return 1 - Success
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GetMac(MS_U8* mac)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->GetMac(mac);
}

//-------------------------------------------------------------------------------------------------
/// MAC configuration
/// @param mac \b IN: Storage to put MAC for config
/// @return 1 - Success
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_MacConfig(MS_U8* mac)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->MacConfig(mac);
}

//-------------------------------------------------------------------------------------------------
/// Set DHCP mode
/// @param TRUE to do DHCP  FALSE to disable DHCP
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_DhcpConfig(MS_BOOL on)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->DhcpConfig(on);
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
MS_U8 MApi_WLAN_GetDhcpState(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return 0;
    }
    return gWlanTab->GetDhcpState();
}

//-------------------------------------------------------------------------------------------------
/// Get DNS Server
/// @param pu8Server1 \b In: Dns1
/// @param pu8Server2 \b In: Dns2
/// @note
//-------------------------------------------------------------------------------------------------

void MApi_WLAN_SetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2)
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
/// Get DNS Server
/// @param pu8Server1 \b OUT: Dns1
/// @param pu8Server2 \b OUT: Dns2
/// @param u32Size \b In: size of output buffer
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size)
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
/// Init Interface & DHCP
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_IntfReset(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->IntfReset();
}

//-------------------------------------------------------------------------------------------------
/// Show Interface Info
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_ShowIntf(void)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    MS_U32 curr_ip;
    MS_U32 curr_netmask;
    MS_U32 curr_gateway;
    MS_U8 curr_mac[6]={0};

    curr_ip = MApi_WLAN_GetIp();
    curr_gateway = MApi_WLAN_GetGateway();
    curr_netmask = MApi_WLAN_GetSubnet();
    MApi_WLAN_GetMac(curr_mac);

    printf("IP = %d.%d.%d.%d\n", \
           curr_ip & 0xff, \
           (curr_ip >> 8) & 0XFF, \
           (curr_ip >> 16) & 0XFF, \
           (curr_ip >> 24) & 0XFF);

    printf("Subnet = %d.%d.%d.%d\n", \
        curr_netmask & 0xff, \
        (curr_netmask >> 8) & 0XFF, \
        (curr_netmask >> 16) & 0XFF, \
        (curr_netmask >> 24) & 0XFF);

    printf("Gateway = %d.%d.%d.%d\n", \
        curr_gateway & 0xff, \
        (curr_gateway >> 8) & 0XFF, \
        (curr_gateway >> 16) & 0XFF, \
        (curr_gateway >> 24) & 0XFF);

    printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", \
    *curr_mac,*(curr_mac+1),*(curr_mac+2),*(curr_mac+3),*(curr_mac+4),*(curr_mac+5));

    typedef void pr_fun(char *fmt, ...);
    externC void show_network_tables(pr_fun *pr);

    show_network_tables( (pr_fun *) diag_printf);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get SSID of the connected AP
/// @return TRUE - Ready
/// @return FALSE - Not Ready
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GetConnetedAPBSSID(char * bssid,int len)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->GetConnetedAPBSSID(bssid,len);
}

//-------------------------------------------------------------------------------------------------
/// Ping command
/// @return TRUE - Ready
/// @return FALSE - Not Ready
/// @note
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_WLAN_Ping(char* target_addr)
{
    if((_gbInOperation == FALSE)||(gWlanTab == NULL))
    {
        printf("Wlan device not detected.\n");
        return FALSE;
    }
    return gWlanTab->Ping(target_addr);
}

#endif
#endif
#endif
