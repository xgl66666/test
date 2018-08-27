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
#if defined(NMGR_ENABLE) && (NMGR_ENABLE==1) || defined(MI_ENABLE) && (MI_ENABLE == 1)
#if defined(WLAN_ENABLE) && (WLAN_ENABLE==1) || defined(MI_WLAN_ENABLE) && (MI_WLAN_ENABLE == 1)

#include <network.h>
#if CYGHWR_NET_DRIVER_RA0

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_ECOS)
#include <ctype.h>

#include "Board.h"
#if defined(WLAN_ENABLE) && (WLAN_ENABLE==1)
#include "HbCommon.h"
#endif
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
#include "MsMemory.h"


//-------------------------------------------------------------------------------------------------
//  extern reference
//-------------------------------------------------------------------------------------------------
externC int get_current_dhcp_mode(void);
externC void set_current_dhcp_mode(int val);
externC MS_U32 get_default_gateway(void);
extern unsigned long dns_server[2];

extern int usb_wifi_init(void);
extern int usb_wifi_exit(void);
extern int CmdIwpriv(int argc, char *argv[]);
extern int get_wifi_connect_status(void);
extern int get_scan_list(char *ifname,char * buf,int len);
extern int get_wifi_radio_status(void);
extern int get_connected_ap_bssid(char * buf,int len);
extern int (*platform_get)(char *, int, char *);


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PING_LOG 0
#define AP_LIST_BUFFER_SIZE 6144


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _trim(char *c,int arrayLen);
void _lower_string(char *s);


//-------------------------------------------------------------------------------------------------
//  Functions Prototypes
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Config ( PWIFI_CONFIG wifi_config );
MS_BOOL MApi_WLAN_RadioOn(void);
MS_BOOL MApi_WLAN_RadioOff(void);
MS_BOOL MApi_WLAN_GetAPList(PWLAN_ACCESS_POINT_LIST *ppAPList);
void    MApi_WLAN_ReleaseAPList(PWLAN_ACCESS_POINT_LIST *ppAPList);
MS_S8   MApi_WLAN_ConnectStatus(void);
MS_U32  MApi_WLAN_GetIp(void);
MS_U32  MApi_WLAN_GetSubnet(void);
MS_U32  MApi_WLAN_GetGateway(void);
MS_BOOL MApi_WLAN_GetMac(MS_U8* mac);
MS_BOOL MApi_WLAN_DhcpConfig(MS_BOOL on);
MS_U8   MApi_WLAN_GetDhcpState(void);
MS_BOOL MApi_WLAN_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size);
MS_BOOL MApi_WLAN_IntfReset(void);
MS_BOOL MApi_WLAN_ShowIntf(void);
MS_BOOL MApi_WLAN_Scan(void);
MS_BOOL MApi_WLAN_IsSecured(PWLAN_ACCESS_POINT pAP);
MS_BOOL MApi_WLAN_IsRadioOn(void);
MS_U8   MApi_WLAN_Ping(char* target_addr);


#define WLAN_POOL_SIZE                    0x160000
extern MS_S32 gs32NonCachedPoolID;


static MemoryPool_Info _WLANMemoryPPool_Info[1] =
{
    {-1, WLAN_POOL_SIZE, 0x0, 0x0, E_MSOS_FIFO, "WLANCachedPool"},
};

MS_S32 gs32WLANNonCachedPoolID     = -1;

MS_S32 MApi_WLAN_CreateMemoryPool(void)
{
    diag_printf("\n\n\n<<<[%s]>>>\n\n\n",__FUNCTION__);
    _WLANMemoryPPool_Info[0].uPoolSize = WLAN_POOL_SIZE;
    #if defined(WIFI_POOL_SIZE) && (WIFI_POOL_SIZE == WLAN_POOL_SIZE )
    _WLANMemoryPPool_Info[0].u32Addr = MsOS_PA2KSEG1(WIFI_POOL_START);
    #else
    _WLANMemoryPPool_Info[0].u32Addr = (MS_U32) MsOS_AllocateMemory(WLAN_POOL_SIZE,gs32NonCachedPoolID);
    #endif
    _WLANMemoryPPool_Info[0].iId = MsOS_CreateMemoryPool( _WLANMemoryPPool_Info[0].uPoolSize,
                                                          _WLANMemoryPPool_Info[0].u32MinAllocation,
                                                  (void*)(_WLANMemoryPPool_Info[0].u32Addr),
                                                          _WLANMemoryPPool_Info[0].eAttribute,
                                                          _WLANMemoryPPool_Info[0].szName);

#if defined(WLAN_ENABLE) && (WLAN_ENABLE==1)
    HB_ASSERT( _WLANMemoryPPool_Info[0].iId >= 0 );
#else
    MS_ASSERT( _WLANMemoryPPool_Info[0].iId >= 0 );
#endif
    gs32WLANNonCachedPoolID = _WLANMemoryPPool_Info[0].iId;
    return TRUE;
}


MS_BOOL MApi_WLAN_DeleteMemoryPool(void)
{
    diag_printf("\n\n\n<<<[%s]>>>\n\n\n",__FUNCTION__);
#if defined(WLAN_ENABLE) && (WLAN_ENABLE==1)
    HB_ASSERT(gs32WLANNonCachedPoolID != -1);
#else
    MS_ASSERT(gs32WLANNonCachedPoolID != -1);
#endif
    MsOS_DeleteMemoryPool (gs32WLANNonCachedPoolID);
    memset((char*)_WLANMemoryPPool_Info[0].u32Addr, 0x00,WLAN_POOL_SIZE);
    #if defined(WIFI_POOL_SIZE) && (WIFI_POOL_SIZE == WLAN_POOL_SIZE )
    #else
    MsOS_FreeMemory((void *) _WLANMemoryPPool_Info[0].u32Addr,gs32NonCachedPoolID);
    #endif
    gs32WLANNonCachedPoolID = -1;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// WLAN SetCountryCode
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
int MApi_WLAN_SetCountryCode(char *buf, int buf_len, char *key)
{
    if (buf && key)
    {
        strncpy(buf, "", buf_len);
        return 0;
    }

    return -1;
}

//-------------------------------------------------------------------------------------------------
/// WLAN Initialization
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Init(void)
{
    int ret=0;
    platform_get = MApi_WLAN_SetCountryCode;
    ret=usb_wifi_init();

    if( ret< 0 ) 
    {
        diag_printf("Unable to register WiFi Dongle driver\n");
        return FALSE;
    } 

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// WLAN De-Initialization
/// @note
//-------------------------------------------------------------------------------------------------
void MApi_WLAN_DeInit(void)
{
    usb_wifi_exit();
}

//-------------------------------------------------------------------------------------------------
/// WLAN Configuration and connect to AP
/// @param wifi configuration \b IN: NULL for Stop WIFI connect
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Config ( PWIFI_CONFIG wifi_config )
{
    char *cmd_args[]={"ra0","set","xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};

    //NULL for Stop WIFI connect
    if(wifi_config==NULL)
    {
        strcpy(cmd_args[2],"SSID=");
        if(!CmdIwpriv(3,cmd_args))
        {
           diag_printf("Set %s Fail\n",cmd_args[2]);
           return FALSE;
        }

        return TRUE;
    }


    switch(wifi_config->network_type)
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

            switch(wifi_config->auth_mode)
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

                    switch(wifi_config->encrypt_type)
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

                           switch( wifi_config->default_keyid )
                           {
                               case 1:
                               {
                                   //<<Send cmd Key1=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key1=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 2:
                               {
                                   //<<Send cmd Key2=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key2=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 3:
                               {
                                   //<<Send cmd Key3=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key3=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 4:
                               {
                                   //<<Send cmd Key4=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key4=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
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

                    switch(wifi_config->encrypt_type)
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

                           switch( wifi_config->default_keyid )
                           {
                               case 1:
                               {
                                   //<<Send cmd Key1=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key1=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 2:
                               {
                                   //<<Send cmd Key2=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key2=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 3:
                               {
                                   //<<Send cmd Key3=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key3=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
                                   if(!CmdIwpriv(3,cmd_args))
                                   {
                                       diag_printf("Set %s Fail\n",cmd_args[2]);
                                       return FALSE;
                                   }
                                   break;
                               }

                               case 4:
                               {
                                   //<<Send cmd Key4=wifi_config->key>>>
                                   strcpy(cmd_args[2],"Key4=");
                                   strcat(cmd_args[2],(char *)wifi_config->key);
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
                    if(wifi_config->auth_mode==WPAPSK)
                    strcpy(cmd_args[2],"AuthMode=WPAPSK");

                    if(wifi_config->auth_mode==WPA2PSK)
                    strcpy(cmd_args[2],"AuthMode=WPA2PSK");

                    if(!CmdIwpriv(3,cmd_args))
                    {
                        diag_printf("Set %s Fail\n",cmd_args[2]);
                        return FALSE;
                    }

                    switch(wifi_config->encrypt_type)
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

                    //<<Send cmd WPAPSK=wifi_config->key>>
                    strcpy(cmd_args[2],"WPAPSK=");
                    strcat(cmd_args[2],(char *)wifi_config->key);
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

    //<<Send cmd SSID=wifi_config->ssid>>
    strcpy(cmd_args[2],"SSID=");
    strcat(cmd_args[2],(char *)wifi_config->ssid);
    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// WLAN Radio On
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_RadioOn(void)
{
    char *cmd_args[]={"ra0","set","RadioOn=1"};

    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// WLAN Radio Off
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_RadioOff(void)
{
    char *cmd_args[]={"ra0","set","RadioOn=0"};

    if(!CmdIwpriv(3,cmd_args))
    {
       diag_printf("Set %s Fail\n",cmd_args[2]);
       return FALSE;
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// WLAN WiFi Scan Trigger
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Scan(void)
{
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

//-------------------------------------------------------------------------------------------------
/// Check Is AP Secured
/// @return TRUE
/// @return FALSE
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_IsSecured(PWLAN_ACCESS_POINT pAP)
{
    return strcmp((char *)pAP->security,"NONE");
}


//-------------------------------------------------------------------------------------------------
/// WLAN Get&Parse WiFi AP List
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_GetAPList(PWLAN_ACCESS_POINT_LIST *ppAPList)
{
    char ap_list_buff[AP_LIST_BUFFER_SIZE];//AP list in list[]
    char ap_list_row[108];
    ap_list_row[107]='\0';
    int ret=0;
    int i=0;
    int offset=0;
    PWLAN_ACCESS_POINT AP=NULL;
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

    (*ppAPList)=(PWLAN_ACCESS_POINT_LIST)malloc(sizeof(WLAN_ACCESS_POINT_LIST)+NumOfAP*sizeof(WLAN_ACCESS_POINT));
    (*ppAPList)->NumberOfAP=NumOfAP;

    //AP List Parsing
    for(i=0,offset=107;i<(*ppAPList)->NumberOfAP;i++,offset+=107)
    {
       AP=(*ppAPList)->AP+i;

       memcpy(ap_list_row,(char *)(ap_list_buff+offset+1), 107);

       //Skip The AP Which No SSID
       if(*(ap_list_row+4)==' ')
       {
           (*ppAPList)->NumberOfAP--;
           i--;
           continue;
       }

       memcpy((char *)AP->ssid    ,(char *)(ap_list_row+4),33);
       _trim((char *)AP->ssid,33);


       memcpy((char *)AP->channel ,(char *)(ap_list_row),3);
       _trim((char *)AP->channel,3);


       memcpy((char *)AP->mac     ,(char *)(ap_list_row+4+33),20);
       _trim((char *)AP->mac,20);


       memcpy((char *)AP->security,(char *)(ap_list_row+4+33+20),23);
       _trim((char *)AP->security,23);

       memcpy((char *)AP->signal  ,(char *)(ap_list_row+4+33+20+23),9);
       _trim((char *)AP->signal,9);

       memcpy((char *)AP->mode    ,(char *)(ap_list_row+4+33+20+23+9),7);
       _trim((char *)AP->mode,7);

       memcpy((char *)AP->ext_ch  ,(char *)(ap_list_row+4+33+20+23+9+7+1),7);
       _trim((char *)AP->ext_ch,7);

       memcpy((char *)AP->nt      ,(char *)(ap_list_row+4+33+20+23+9+7+1+7),3);
       _trim((char *)AP->nt,3);


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

MS_BOOL MApi_WLAN_GetConnetedAPBSSID(char * bssid,int len)
{
    int ret;

    ret=get_connected_ap_bssid(bssid,len);
    _lower_string(bssid);

    if(ret)
        return TRUE;
    else
        return FALSE;
    
}



//-------------------------------------------------------------------------------------------------
/// WLAN Release&Free WiFi AP List
//-------------------------------------------------------------------------------------------------
void MApi_WLAN_ReleaseAPList(PWLAN_ACCESS_POINT_LIST *ppAPList)
{
    free(*ppAPList);
    *ppAPList=NULL;
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
    return (MS_S8)get_wifi_connect_status();
}

//-------------------------------------------------------------------------------------------------
/// Check Radio On/Off
/// @return TRUE - RadioOn
/// @return FALSE - RadioOff
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_IsRadioOn(void)
{
    if(get_wifi_radio_status())
        return TRUE;
    else
        return FALSE;
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

//-------------------------------------------------------------------------------------------------
/// Get current Subnet configuration
/// @param intf_name \b IN: Interface name
/// @return others - Current Subnet value
/// @return 0 - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_WLAN_GetSubnet(void)
{
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

//-------------------------------------------------------------------------------------------------
/// Get current Gateway configuration
/// @param void \b IN: NONE
/// @return others - Current gateway value
/// @return 0 - No gateway specified
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_WLAN_GetGateway(void)
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
MS_BOOL MApi_WLAN_GetMac(MS_U8* mac)
{
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



//-------------------------------------------------------------------------------------------------
/// Set DHCP mode
/// @param TRUE to do DHCP  FALSE to disable DHCP
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_DhcpConfig(MS_BOOL on)
{
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
   return ra0_dhcpstate;
}

//-------------------------------------------------------------------------------------------------
/// Get DNS Server
/// @param on \b IN: on
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
   if(get_wifi_connect_status()==WIFI_STATE_UNINITIALIZED)
   {
       return FALSE;
   }

   init_all_network_interfaces();
   ra0_up = 1;

   return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Show Interface Info
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_ShowIntf(void)
{
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

// Compute INET checksum
static int _MApi_WLAN_NetCksum(MS_U16* addr, int len)
{
    MS_U16* addr_bak = addr;
    MS_U16 ret;
    MS_U32 checksum = 0;
    MS_U16 odd_byte = 0;
    MS_S32 left = len;

    while( left > 1 )
    {
        checksum += *addr_bak;
        addr_bak++;
        left -= 2;
    }

    if( left == 1 ) {
        *(u_char *)(&odd_byte) = *(u_char *)addr_bak;
        checksum += odd_byte;
    }

    checksum = (checksum >> 16) + (checksum & 0x0000ffff); 
    checksum += (checksum >> 16);                    
    ret = ~checksum;                       
    return (ret);
}


#if PING_LOG  
static int _MApi_WLAN_TicksToMs( MS_U32 ticks )
{
    int ticks_per_second = 1000000000/(CYGNUM_HAL_RTC_NUMERATOR/CYGNUM_HAL_RTC_DENOMINATOR);
    int ms = (ticks * 1000)/ticks_per_second;
    return ms;
}
#endif

static int _MApi_WLAN_ShowIcmp(unsigned char *pkt, int len,
          struct sockaddr_in *from, struct sockaddr_in *to)
{
    struct icmp *icmp;
    struct ip *ip;    
#if PING_LOG    
    cyg_tick_count_t *start_time;
    cyg_tick_count_t curent_time;
    curent_time = cyg_current_time();// get current time
#endif

    ip = (struct ip *)pkt;//pointer to IP header

    if ((len < sizeof(*ip)) || ip->ip_v != IPVERSION) {//check packet length and IP vertion
#if PING_LOG
        printf("not IP or Short packet!   ");
        printf("%s: Len: %d, Version: %d\n", inet_ntoa(from->sin_addr), len, ip->ip_v);
#endif
        return 0;
    }
    icmp = (struct icmp *)(pkt + sizeof(*ip));
    len -= (sizeof(*ip) + 8);
#if PING_LOG    
    start_time = (cyg_tick_count_t *)&icmp->icmp_data;
#endif
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
#if PING_LOG
        printf("ICMP error   ");
        printf("%s: type: %d\n",inet_ntoa(from->sin_addr), icmp->icmp_type);
#endif

        return 0;
    }
    if (icmp->icmp_id != 0x1234) {
        printf(" ICMP received for error id - ");
        printf("%s sent: %x, recvd: %x\n",inet_ntoa(from->sin_addr), 0x1234, icmp->icmp_id);
    }
#if PING_LOG
    printf("%d bytes from %s: icmp_seq=%d , time=%dms\n", len, inet_ntoa(from->sin_addr), icmp->icmp_seq, _MApi_WLAN_TicksToMs(curent_time - *start_time));
#endif
    return (from->sin_addr.s_addr == to->sin_addr.s_addr);
}


static MS_U8 _MApi_WLAN_PingHost(int socket_fd, struct sockaddr_in *host, MS_U16 u16Seq)
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
    icmp->icmp_cksum = _MApi_WLAN_NetCksum( (u_short *)icmp, icmp_len+8);
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
        if (_MApi_WLAN_ShowIcmp(ping_in_pkt, len, &from, host))
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
/// Ping command
/// @return TRUE - Ready
/// @return FALSE - Not Ready
/// @note
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_WLAN_Ping(char* target_addr)
{
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

        result = _MApi_WLAN_PingHost(socket_fd, &host, iCount);

        close(socket_fd);
        socket_fd = -1;
    }

    return result;
}


//Removes trailing spaces
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

#endif
#endif
#endif
#endif
