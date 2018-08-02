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
#ifndef __DRV_TUNER_NULL_H__
#define __DRV_TUNER_NULL_H__

MS_BOOL     MDrv_Wlan_Null_Init(drv_wlanop_Callback pfEventCallback);
MS_BOOL     MDrv_Wlan_Null_Config(WLAN_MS_WIFI_CONFIG stWlanConfig);
MS_BOOL     MDrv_Wlan_Null_RadioOn(void);
MS_BOOL     MDrv_Wlan_Null_RadioOff(void);
MS_BOOL     MDrv_Wlan_Null_GetAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList);
void        MDrv_Wlan_Null_ReleaseAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList);
MS_S8       MDrv_Wlan_Null_ConnectStatus(void);
MS_U32      MDrv_Wlan_Null_GetIp(void);
MS_U32      MDrv_Wlan_Null_GetSubnet(void);
MS_U32      MDrv_Wlan_Null_GetGateway(void);
MS_BOOL     MDrv_Wlan_Null_GetMac(MS_U8* mac);
MS_BOOL     MDrv_Wlan_Null_IpConfig(MS_U8* address);
MS_BOOL     MDrv_Wlan_Null_SubnetConfig(MS_U8* subnet);
MS_BOOL     MDrv_Wlan_Null_GatewaySet(MS_U8* gateway);
MS_BOOL     MDrv_Wlan_Null_GatewayDel(MS_U8* gateway);
MS_BOOL     MDrv_Wlan_Null_MacConfig(MS_U8* mac);
MS_BOOL     MDrv_Wlan_Null_DhcpConfig(MS_BOOL on);
MS_U8       MDrv_Wlan_Null_GetDhcpState(void);
MS_BOOL     MDrv_Wlan_Null_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size);
MS_BOOL     MDrv_Wlan_Null_IntfReset(void);
MS_BOOL     MDrv_Wlan_Null_Scan(void);
MS_BOOL     MDrv_Wlan_Null_IsSecured(WLAN_MS_ACCESS_POINT AP);
MS_U8       MDrv_Wlan_Null_Ping(char* target_addr);
MS_BOOL     MDrv_Wlan_Null_IsRadioOn(void);
MS_BOOL     MDrv_Wlan_Null_GetConnetedAPBSSID(char * bssid,int len);
MS_BOOL     MDrv_Wlan_Null_Extension_Function(WLAN_EXT_FUNCTION_TYPE fuction_type, void *data);


#endif // #define __DRV_TUNER_NULL_H__
