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
/// @file   apiWLAN.h
/// @brief  WLAN API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_WLAN_H_
#define _API_WLAN_H_

#if defined (MSOS_TYPE_ECOS)
#ifdef __cplusplus
extern "C"
{
#endif

#include "Board.h"
#include "drvWlan.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
///WIFI ConnectStatus defined For MTK WiFi chip MT7601U
///-1 :init fail
///0 : init value
///1 : Authancation on going
///12: Authancation fail
///13: Authancation success
///2 : Assocation on going
///22: Assocation fail
///23: Assocation success
///3 : 4-way handle shark on going (this state if for wpapsk&wpa2psk only)
///32: 4-way handle shark fail,key fail
///33: 4-way handle shark success
///4 : connected
//-------------------------------------------------------------------------------------------------
#define WIFI_STATE_UNINITIALIZED                -1
#define WIFI_STATE_INITIALIZED                   0
#define WIFI_STATE_AUTH_ONGOING                  1
#define WIFI_STATE_AUTH_FAIL                    12
#define WIFI_STATE_AUTH_SUCCESS                 13
#define WIFI_STATE_ASSOCATION_ONGOING            2
#define WIFI_STATE_ASSOCATION_FAIL              22
#define WIFI_STATE_ASSOCATION_SUCCESS           23
#define WIFI_STATE_4WAY_HANDSHARK_ONGOING        3
#define WIFI_STATE_4WAY_HANDSHARK_FAIL          32
#define WIFI_STATE_4WAY_HANDSHARK_SUCCESS       33
#define WIFI_STATE_CONNECTED                     4

#define PING_LOG 1
#define AP_LIST_BUFFER_SIZE 4096


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct _WLAN_ACCESS_POINT {
    char  channel[4];   //Channel
    char  ssid[33];     //SSID
    char  mac[20];      //MAC Address, BSSID
    char  security[23]; //Security Mode
    char  signal[9];    //Siganl
    char  mode[8];      //Wireless Mode
    char  ext_ch[7];    //Ext Channel
    char  nt[3];        //Network Type In:Infrastructure  Ad:Ad-hoc
} WLAN_ACCESS_POINT, *PWLAN_ACCESS_POINT;

typedef struct _WLAN_ACCESS_POINT_LIST {
	MS_U8 NumberOfAP;
	WLAN_ACCESS_POINT AP[];
} WLAN_ACCESS_POINT_LIST, *PWLAN_ACCESS_POINT_LIST;

typedef enum{
  INFA,     //Infrastructure mode
  ADHOC     //Ad-hoc mode
} NetworkType;

typedef enum{
   OPEN,    //For open system
   SHARED,  //For shared key system
   WPAPSK,  //For WPA pre-shared key (Infra)
   WPA2PSK, //For WPA2 pre-shared key (Infra)
   WEPAUTO, //For Auto switch between OPEN and SHARED
   WPANONE, //For WPA pre-shared key (Adhoc)
   WPA,     //For enterprise mode (Need wpa_supplicant)
   WPA2     //For enterprise mode (Need wpa_supplicant)
} AuthMode;

typedef enum{
    NONE,   //for AuthMode=OPEN
    WEP,    //for AuthMode=OPEN or AuthMode=SHARED
    TKIP,   //for AuthMode=WPAPSK or WPA2PSK
    AES     //for AuthMode=WPAPSK or WPA2PSK
}EncryptType;


typedef struct _WIFI_CONFIG {
    NetworkType  network_type;//<ENUM>
    AuthMode     auth_mode;   //<ENUM>
    EncryptType  encrypt_type;//<ENUM>
    MS_U8  default_keyid;     //<Integer> 1~4 for WEP
    char  key[64];           //<String>  key
    char  ssid[33]; //<String>SSID
} WIFI_CONFIG, *PWIFI_CONFIG;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_WLAN_Init(void);
MS_BOOL MApi_WLAN_Config (WIFI_CONFIG wifi_config );
MS_BOOL MApi_WLAN_RadioOn(void);
MS_BOOL MApi_WLAN_RadioOff(void);
MS_BOOL MApi_WLAN_GetAPList(PWLAN_ACCESS_POINT_LIST *ppAPList);
void    MApi_WLAN_ReleaseAPList(PWLAN_ACCESS_POINT_LIST *ppAPList);
MS_S8   MApi_WLAN_ConnectStatus(void);
MS_U32  MApi_WLAN_GetIp(void);
MS_U32  MApi_WLAN_GetSubnet(void);
MS_U32  MApi_WLAN_GetGateway(void);
MS_BOOL MApi_WLAN_GetMac(MS_U8* mac);
MS_BOOL MApi_WLAN_IpConfig(MS_U8* address);
MS_BOOL MApi_WLAN_SubnetConfig(MS_U8* subnet);
MS_BOOL MApi_WLAN_GatewaySet(MS_U8* gateway);
MS_BOOL MApi_WLAN_GatewayDel(MS_U8* gateway);
MS_BOOL MApi_WLAN_MacConfig(MS_U8* mac);
MS_BOOL MApi_WLAN_DhcpConfig(MS_BOOL on);
MS_U8   MApi_WLAN_GetDhcpState(void);
void    MApi_WLAN_SetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2);
MS_BOOL MApi_WLAN_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size);
MS_BOOL MApi_WLAN_IntfReset(void);
MS_BOOL MApi_WLAN_ShowIntf(void);
MS_BOOL MApi_WLAN_Scan(void);
MS_BOOL MApi_WLAN_IsSecured(WLAN_ACCESS_POINT AP);
MS_U8   MApi_WLAN_Ping(char* target_addr);
MS_BOOL MApi_WLAN_IsRadioOn(void);
MS_BOOL MApi_WLAN_GetConnetedAPBSSID(char * bssid,int len);


#ifdef __cplusplus
}
#endif


#endif // _API_WLAN_H_

#endif
