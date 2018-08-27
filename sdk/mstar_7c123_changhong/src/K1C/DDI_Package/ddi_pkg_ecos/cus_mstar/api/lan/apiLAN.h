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
/// @file   apiLAN.h
/// @brief  LAN API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _API_LAN_H_
#define _API_LAN_H_


#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define ENCRYPTION_MODE_80211_DISABLE       0
#define ENCRYPTION_MODE_80211_WEP_OPEN      1
#define ENCRYPTION_MODE_80211_WEP_SHARE_KEY 2

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// EMAC OID
typedef MS_U32                                 EMAC_OID;
enum {
    EOID_LINK_SPEED                       = 0x00000001  ,   // 4B
    EOID_MEDIA_CONNECT_STATUS                           ,   // 4B
    EOID_MAC_ADDRESS                                    ,   // 6B
    EOID_DUPLEX                                         ,   // 4B
    EOID_XMIT_OK                                        ,   // 4B
    EOID_RCV_OK                                         ,   // 4B
    EOID_XMIT_ERROR                                     ,   // 4B
    EOID_RCV_ERROR                                      ,   // 4B
    EOID_RCV_NO_BUFFER                                  ,   // 4B
};

typedef struct _ApListTag
{
//public for api layer: (see drvIeee802_11.h)
    MS_U8  MAC_addr[6];
    MS_U8  BSS_addr[6];
    MS_U8  channel;
    MS_U8  SSID_STR[100];//byte[0]:reserve, byte[1]:len, byte [2:len]:SSID
    MS_U8  encryption_method;//0: open, 1: key
}ApListTag;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_LAN_init(void);
MS_U32 MApi_LAN_QueryInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xread);
MS_U32 MApi_LAN_SetInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xwrit);
MS_U32 MApi_LAN_GetIp(MS_U8* intf_name);
MS_U32 MApi_LAN_GetSubnet(MS_U8* intf_name);
MS_BOOL MApi_LAN_GetMac(MS_U8* intf_name, MS_U8* mac);
MS_U32 MApi_LAN_GetGateway(void);
MS_BOOL MApi_LAN_MacConfig(MS_U8* mac);
MS_BOOL MApi_LAN_IpConfig(MS_U8* inaddr);
MS_BOOL MApi_LAN_SubnetConfig(MS_U8* subnet);
MS_BOOL MApi_LAN_DhcpConfig(MS_U16 on);
MS_BOOL MApi_LAN_DhcpConfigBusy(void);
MS_BOOL MApi_LAN_DhcpConfigSync(MS_U16 on);
MS_BOOL MApi_LAN_GatewaySet(MS_U8* gateway);
MS_BOOL MApi_LAN_GatewayDel(MS_U8* gateway);
MS_BOOL MApi_LAN_Poweron(void);
MS_BOOL MApi_LAN_Poweroff(void);
void MApi_LAN_Resume(void);
void MApi_LAN_Suspend(void);
MS_BOOL MApi_LAN_ShowIntf(void);
MS_BOOL MApi_LAN_IntfReset(void);
MS_BOOL MApi_LAN_EthReady(void);
MS_U8 MApi_LAN_GetDhcpState(void);
MS_U8 MApi_LAN_Ping(MS_U8* target_addr);
MS_U8 MApi_LAN_PingMultiple(MS_U8* target_addr, MS_U8 u8Count);
MS_BOOL MApi_LAN_ConnectStatus(void);
MS_BOOL MApi_LAN_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size);
void MApi_LAN_SetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2);
struct hostent *MApi_LAN_gethostbyname(MS_U8* name);
MS_U8 MAPI_WLAN_GetApList(MS_U32* u32ap_list_array, MS_U8 u8max_num); //return num of AP;
void MAPI_WLAN_Join_By_Name(MS_U8* pu8Name);
void MAPI_WLAN_Auth_Mode(MS_U8 mode, MS_U8* password, MS_U8 pw_len);


#ifdef __cplusplus
}
#endif


#endif // _API_LAN_H_

