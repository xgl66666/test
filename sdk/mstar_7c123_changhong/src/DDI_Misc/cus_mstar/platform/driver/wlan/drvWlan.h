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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvWlan.h
/// @brief  Wlan Common Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_WLAN_H_
#define _DRV_WLAN_H_

#if defined (MSOS_TYPE_ECOS)

#include "MsCommon.h"
#include "apiWLAN.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//============ debug setting ========================
#define WLAN_EN_ERR 0x01
#define WLAN_EN_DBG 0x02
#define WLAN_EN_API 0x04
#define WLAN_DEBUG_OPTIONS  1


#if (WLAN_DEBUG_OPTIONS & WLAN_EN_ERR)
#define WLAN_ERR(x) printf("[WLAN ERR]");printf x
#else
#define WLAN_ERR(x) 
#endif

#if (WLAN_DEBUG_OPTIONS & WLAN_EN_DBG)
#define WLAN_DBG(x) printf("[WLAN DBG]");printf x
#else
#define WLAN_DBG(x) 
#endif



#if (WLAN_DEBUG_OPTIONS & WLAN_EN_API)
#define WLAN_ENTERAPI(x) printf x
#define WLAN_ENTERAPISTR(devId) printf("+++ %s [%d] \n\r", __FUNCTION__, devId)
#define WLAN_EXITAPI(x) printf x
#define WLAN_EXITAPISTR(devId, retcode) printf("--- %s [%d] (retcode: %d)\n\r", __FUNCTION__, devId, retcode)
#else
#define WLAN_ENTERAPI(x)
#define WLAN_ENTERAPISTR(devId)
#define WLAN_EXITAPI(x)
#define WLAN_EXITAPISTR(devId, retcode)
#endif

//===============================================


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define _GET_WLAN_ENTRY_NODE(var, Name) var##Name
#define GET_WLAN_ENTRY_NODE(Name) _GET_WLAN_ENTRY_NODE(wlan_entry_,Name) 

#define DDIBLD_ATTRIB_WLAN_SECTION(__sect__) __attribute__((section (__sect__)))

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

#ifndef DDI_DRV_WLAN_TABLE_ENTRY
#define DDI_DRV_WLAN_TABLE_ENTRY( _name ) \
        DDIBLD_ATTRIB_WLAN_SECTION(".ecos.wlan." __xstring(_name) ".data")
#endif

/// Define extension function types
typedef enum
{
    WLAN_EXT_FUNC_RESERVED
} WLAN_EXT_FUNCTION_TYPE;

typedef struct _WLAN_MS_ACCESS_POINT {
    char  channel[4];   //Channel
    char  ssid[33];     //SSID
    char  mac[20];      //MAC Address, BSSID
    char  security[23]; //Security Mode
    char  signal[9];    //Siganl
    char  mode[8];      //Wireless Mode
    char  ext_ch[7];    //Ext Channel
    char  nt[3];        //Network Type In:Infrastructure  Ad:Ad-hoc
} WLAN_MS_ACCESS_POINT;

typedef struct _WLAN_MS_ACCESS_POINT_LIST {
	MS_U8 NumberOfAP;
	WLAN_MS_ACCESS_POINT AP[];
} WLAN_MS_ACCESS_POINT_LIST;

typedef enum{
  WLAN_EN_INFA,     //Infrastructure mode
  WLAN_EN_ADHOC     //Ad-hoc mode
} WLAN_EN_NetworkType;

typedef enum{
   WLAN_EN_OPEN,    //For open system
   WLAN_EN_SHARED,  //For shared key system
   WLAN_EN_WPAPSK,  //For WPA pre-shared key (Infra)
   WLAN_EN_WPA2PSK, //For WPA2 pre-shared key (Infra)
   WLAN_EN_WEPAUTO, //For Auto switch between OPEN and SHARED
   WLAN_EN_WPANONE, //For WPA pre-shared key (Adhoc)
   WLAN_EN_WPA,     //For enterprise mode (Need wpa_supplicant)
   WLAN_EN_WPA2     //For enterprise mode (Need wpa_supplicant)
} WLAN_EN_AuthMode;

typedef enum{
    WLAN_EN_NONE,   //for AuthMode=OPEN
    WLAN_EN_WEP,    //for AuthMode=OPEN or AuthMode=SHARED
    WLAN_EN_TKIP,   //for AuthMode=WPAPSK or WPA2PSK
    WLAN_EN_AES     //for AuthMode=WPAPSK or WPA2PSK
}WLAN_EN_EncryptType;


typedef struct _WLAN_MS_WIFI_CONFIG {
    WLAN_EN_NetworkType  network_type;//<ENUM>
    WLAN_EN_AuthMode     auth_mode;   //<ENUM>
    WLAN_EN_EncryptType  encrypt_type;//<ENUM>
    MS_U8  default_keyid;     //<Integer> 1~4 for WEP
    char  key[64];           //<String>  key
    char  ssid[33]; //<String>SSID
}WLAN_MS_WIFI_CONFIG;

typedef enum{
    WLAN_EN_INSERTED,     //for insert device
    WLAN_EN_REMOVED    //for device removed
}WLAN_EN_DeviceEvent;


typedef void (*drv_wlanop_Callback)(MS_U32 u32Module, WLAN_EN_DeviceEvent eEvent);


typedef MS_BOOL     drv_wlanop_Init(drv_wlanop_Callback pfEventCallback);
typedef MS_BOOL     drv_wlanop_Config(WLAN_MS_WIFI_CONFIG stWlanConfig);
typedef MS_BOOL     drv_wlanop_RadioOn(void);
typedef MS_BOOL     drv_wlanop_RadioOff(void);
typedef MS_BOOL     drv_wlanop_GetAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList);
typedef void        drv_wlanop_ReleaseAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList);
typedef MS_S8       drv_wlanop_ConnectStatus(void);
typedef MS_U32      drv_wlanop_GetIp(void);
typedef MS_U32      drv_wlanop_GetSubnet(void);
typedef MS_U32      drv_wlanop_GetGateway(void);
typedef MS_BOOL     drv_wlanop_GetMac(MS_U8* mac);
typedef MS_BOOL     drv_wlanop_IpConfig(MS_U8* address);
typedef MS_BOOL     drv_wlanop_SubnetConfig(MS_U8* subnet);
typedef MS_BOOL     drv_wlanop_GatewaySet(MS_U8* gateway);
typedef MS_BOOL     drv_wlanop_GatewayDel(MS_U8* gateway);
typedef MS_BOOL     drv_wlanop_MacConfig(MS_U8* mac);
typedef MS_BOOL     drv_wlanop_DhcpConfig(MS_BOOL on);
typedef MS_U8       drv_wlanop_GetDhcpState(void);
typedef MS_BOOL     drv_wlanop_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size);
typedef MS_BOOL     drv_wlanop_IntfReset(void);
typedef MS_BOOL     drv_wlanop_Scan(void);
typedef MS_BOOL     drv_wlanop_IsSecured(WLAN_MS_ACCESS_POINT AP);
typedef MS_U8       drv_wlanop_Ping(char* target_addr);
typedef MS_BOOL     drv_wlanop_IsRadioOn(void);
typedef MS_BOOL     drv_wlanop_GetConnetedAPBSSID(char * bssid,int len);
typedef MS_BOOL     drv_wlanop_Extension_Function(WLAN_EXT_FUNCTION_TYPE fuction_type, void *data);


typedef struct 
drv_wlantab_entry
{
    const char                     *name;          // wifi dongle module name
    MS_U32                         data;            // private data value
    drv_wlanop_Init                *Init;
    drv_wlanop_Config              *Config;
    drv_wlanop_RadioOn             *RadioOn;
    drv_wlanop_RadioOff            *RadioOff;
    drv_wlanop_GetAPList           *GetAPList;
    drv_wlanop_ReleaseAPList       *ReleaseAPList;
    drv_wlanop_ConnectStatus       *ConnectStatus;
    drv_wlanop_GetIp               *GetIp;
    drv_wlanop_GetSubnet           *GetSubnet;
    drv_wlanop_GetGateway          *GetGateway;
    drv_wlanop_GetMac              *GetMac;
    drv_wlanop_IpConfig            *IpConfig;
    drv_wlanop_SubnetConfig        *SubnetConfig;
    drv_wlanop_GatewaySet          *GatewaySet;
    drv_wlanop_GatewayDel          *GatewayDel;
    drv_wlanop_MacConfig           *MacConfig;
    drv_wlanop_DhcpConfig          *DhcpConfig;
    drv_wlanop_GetDhcpState        *GetDhcpState;
    drv_wlanop_IntfReset           *IntfReset;
    drv_wlanop_Scan                *Scan;
    drv_wlanop_IsSecured           *IsSecured;
    drv_wlanop_Ping                *Ping;
    drv_wlanop_IsRadioOn           *IsRadioOn;
    drv_wlanop_GetConnetedAPBSSID  *GetConnetedAPBSSID;
    drv_wlanop_Extension_Function  *Extension_Function;
} DRV_WLAN_TABLE_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

#endif // _DRV_WLAN_H_

#endif


