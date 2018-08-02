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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
//
/// @file drvWlanNull.c
/// @brief wlan default null function
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////




#ifndef _WLANNULL_C_
#define _WLANNULL_C_

#include "MsCommon.h"
#include "Board.h"
#include "drvWlan.h"
#include "drvWlanNull.h"

MS_BOOL MDrv_Wlan_Null_Init(drv_wlanop_Callback pfEventCallback)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_Config(WLAN_MS_WIFI_CONFIG stWlanConfig)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_RadioOn(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_RadioOff(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_GetAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

void MDrv_Wlan_Null_ReleaseAPList(WLAN_MS_ACCESS_POINT_LIST **ppAPList)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return;
}

MS_S8 MDrv_Wlan_Null_ConnectStatus(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_U32 MDrv_Wlan_Null_GetIp(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_U32 MDrv_Wlan_Null_GetSubnet(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_U32 MDrv_Wlan_Null_GetGateway(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_GetMac(MS_U8* mac)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_IpConfig(MS_U8* address)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_SubnetConfig(MS_U8* subnet)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_GatewaySet(MS_U8* gateway)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_GatewayDel(MS_U8* gateway)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_MacConfig(MS_U8* mac)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_DhcpConfig(MS_BOOL on)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_U8 MDrv_Wlan_Null_GetDhcpState(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_GetDnsServer(MS_U8* pu8Server1, MS_U8* pu8Server2, MS_U32 u32Size)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_IntfReset(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_Scan(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_IsSecured(WLAN_MS_ACCESS_POINT AP)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_U8 MDrv_Wlan_Null_Ping(char* target_addr)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_IsRadioOn(void)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_GetConnetedAPBSSID(char * bssid,int len)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

MS_BOOL MDrv_Wlan_Null_Extension_Function(WLAN_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    WLAN_DBG(("%s, %d \n", __FUNCTION__,__LINE__));
    return TRUE;
}

DRV_WLAN_TABLE_TYPE GET_WLAN_ENTRY_NODE(WLAN_NULL) DDI_DRV_WLAN_TABLE_ENTRY(wlantab) =      
{                                                                       
    .name               = "WLAN_NULL",          // demod system name
    .data               = WLAN_NULL,            // private data value
    .Init               = MDrv_Wlan_Null_Init,
    .Config             = MDrv_Wlan_Null_Config,
    .RadioOn            = MDrv_Wlan_Null_RadioOn,
    .RadioOff           = MDrv_Wlan_Null_RadioOff,
    .GetAPList          = MDrv_Wlan_Null_GetAPList,
    .ReleaseAPList      = MDrv_Wlan_Null_ReleaseAPList,
    .ConnectStatus      = MDrv_Wlan_Null_ConnectStatus,
    .GetIp              = MDrv_Wlan_Null_GetIp,
    .GetSubnet          = MDrv_Wlan_Null_GetSubnet,
    .GetGateway         = MDrv_Wlan_Null_GetGateway,
    .GetMac             = MDrv_Wlan_Null_GetMac,
    .IpConfig           = MDrv_Wlan_Null_IpConfig,
    .SubnetConfig       = MDrv_Wlan_Null_SubnetConfig,
    .GatewaySet         = MDrv_Wlan_Null_GatewaySet,
    .GatewayDel         = MDrv_Wlan_Null_GatewayDel,
    .MacConfig          = MDrv_Wlan_Null_MacConfig,
    .DhcpConfig         = MDrv_Wlan_Null_DhcpConfig,
    .GetDhcpState       = MDrv_Wlan_Null_GetDhcpState,
    .IntfReset          = MDrv_Wlan_Null_IntfReset,
    .Scan               = MDrv_Wlan_Null_Scan,
    .IsSecured          = MDrv_Wlan_Null_IsSecured,
    .Ping               = MDrv_Wlan_Null_Ping,
    .IsRadioOn          = MDrv_Wlan_Null_IsRadioOn,
    .GetConnetedAPBSSID = MDrv_Wlan_Null_GetConnetedAPBSSID,
    .Extension_Function = MDrv_Wlan_Null_Extension_Function
};

#endif

