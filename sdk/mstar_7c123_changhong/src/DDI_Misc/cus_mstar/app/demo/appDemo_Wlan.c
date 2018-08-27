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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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
/// @file   appDemo_FrontPnl.c
/// @author MStar Semiconductor Inc.
/// @brief A sample code for illustrating how to control front panel
///////////////////////////////////////////////////////////////////////////////////////////////////
#if(ECOS_WIFI_SUPPORT == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include <network.h>
#include <dhcp.h>
#include "drvWlan.h"
#include "apiWLAN.h"
#include "appDemo_Wlan.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------


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

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// app demo of input device to handle all interrupts and events.
// return None
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief initialize wlan module
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// Command: \b Wlan_Init \n
///
/// Sample Command: \b Wlan_Init
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Wlan_Init(void)
{
    if(FALSE == MApi_WLAN_Init())
    {
        return FALSE;
    }
    return MApi_WLAN_RadioOn();
}

//------------------------------------------------------------------------------
/// @brief scan access points.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// Command: \b Wlan_Scan \n
///
/// Sample Command: \b Wlan_Scan
///
//------------------------------------------------------------------------------

MS_BOOL appDemo_Wlan_Scan(void)
{
    PWLAN_ACCESS_POINT_LIST pstAPList;
    int i=0;
    if(FALSE == MApi_WLAN_Scan())
    {
        return FALSE;
    }
    if(FALSE == MApi_WLAN_GetAPList(&pstAPList))
    {
        return FALSE;
    }
    for(i=0;i<pstAPList->NumberOfAP;i++)
    {
       printf("channel:%s ssid:%s mac:%s security:%s signal:%s mode:%s ext_ch:%s nt:%s \n",
        (char *)pstAPList->AP[i].channel,
        (char *)pstAPList->AP[i].ssid,
        (char *)pstAPList->AP[i].mac,
        (char *)pstAPList->AP[i].security,
        (char *)pstAPList->AP[i].signal,
        (char *)pstAPList->AP[i].mode,
        (char *)pstAPList->AP[i].ext_ch,
        (char *)pstAPList->AP[i].nt
        );
    }
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief connect to access points.
/// @param[in] pu8SSID: SSID of the AP
///
/// @param[in] pu16EncryptType: encrypt type of the AP
/// pu16EncryptType : 
/// 0:NONE,   //for AuthMode=OPEN
/// 1:WEP,    //for AuthMode=OPEN or AuthMode=SHARED
/// 2:TKIP,   //for AuthMode=WPAPSK or WPA2PSK
/// 3:AES     //for AuthMode=WPAPSK or WPA2PSK
///
/// @param[in] pu16EncryptType: encrypt type of the AP
/// pu16AuthMode : 
/// 0:OPEN,    //For open system
/// 1:SHARED,  //For shared key system
/// 2:WPAPSK,  //For WPA pre-shared key (Infra)
/// 3:WPA2PSK, //For WPA2 pre-shared key (Infra)
/// 4:WEPAUTO, //For Auto switch between OPEN and SHARED
/// 5:WPANONE, //For WPA pre-shared key (Adhoc)
/// 6:WPA,     //For enterprise mode (Need wpa_supplicant)
/// 7:WPA2     //For enterprise mode (Need wpa_supplicant)
///
/// @param[in] pu8passwd Password (if necessary)
///
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// Command: \b Wlan_Connect \n
///
/// Sample Command: \b Wlan_Connect SSID 3 3 password
/// Sample Command: \b Wlan_Connect SSID 
//------------------------------------------------------------------------------


MS_BOOL appDemo_Wlan_Connect(MS_U8 *pu8SSID, MS_U16 *pu16EncryptType, MS_U16 *pu16AuthMode, MS_U8 *pu8passwd)
{
    WIFI_CONFIG stWifiConfig = {0};
    int Retry = 0;
    snprintf((char*)stWifiConfig.ssid, 32, (char*)pu8SSID);
    snprintf((char*)stWifiConfig.key, 63, (char*)pu8passwd);
    stWifiConfig.encrypt_type = *pu16EncryptType;
    stWifiConfig.auth_mode = *pu16AuthMode;
    stWifiConfig.default_keyid = 1;
    stWifiConfig.network_type = INFA;
    if(FALSE == MApi_WLAN_Config(stWifiConfig))
    {
        return FALSE;
    }

    while(4!=MApi_WLAN_ConnectStatus())
    {
        MsOS_DelayTask(100);
        Retry++;
        if(Retry>100)
        {
            printf("Connect to AP %s Failed\n",stWifiConfig.ssid);
            return FALSE;
        }
    }
    
    if(FALSE == MApi_WLAN_DhcpConfig(TRUE))
    {
        return FALSE;
    }
    Retry = 0;
    while(DHCPSTATE_BOUND != MApi_WLAN_GetDhcpState())
    {
        MsOS_DelayTask(100);
        Retry++;
        if(Retry>100)
        {
            printf("DHCP Failed\n");
            return FALSE;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Ping particular IP
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// Command: \b Wlan_Ping \n
///
/// Sample Command: \b Wlan_Ping 74.125.224.72
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Wlan_Ping(MS_U8 *pu8TargetAddr)
{
    return MApi_WLAN_Ping((char*)pu8TargetAddr);
}

MS_BOOL appDemo_Wlan_Help(void)
{
    printf("Wlan Demo:\n\n");
    printf("1. Wlan_Init\n\n");
    printf("2. Wlan_Scan\n\n");
    printf("3. Wlan_Connect [SSID] [EncryptType] [AuthMode] [Password]\n");
    printf("  EncrptyType:\n");
    printf("    0:NONE,   //for AuthMode=OPEN\n");
    printf("    1:WEP,    //for AuthMode=OPEN or AuthMode=SHARED\n");
    printf("    2:TKIP,   //for AuthMode=WPAPSK or WPA2PSK\n");
    printf("    3:AES     //for AuthMode=WPAPSK or WPA2PSK\n");
    printf("  AuthMode:\n");
    printf("    0:OPEN,    //For open system\n");
    printf("    1:SHARED,  //For shared key system\n");
    printf("    2:WPAPSK,  //For WPA pre-shared key (Infra)\n");
    printf("    3:WPA2PSK, //For WPA2 pre-shared key (Infra)\n");
    printf("    4:WEPAUTO, //For Auto switch between OPEN and SHARED\n");
    printf("    5:WPANONE, //For WPA pre-shared key (Adhoc)\n");
    printf("    6:WPA,     //For enterprise mode (Need wpa_supplicant)\n");
    printf("    7:WPA2     //For enterprise mode (Need wpa_supplicant)\n");
    printf("*********You can get these information from scanned AP list.**********\n");
    printf(" e.g. Wlan_Connect SomeOnesIphone 3 3 12345678\n\n");
    printf("4. Wlan_Ping [IP]\n");
    printf(" e.g. Wlan_Ping 74.125.224.72  (Ping google)\n");
    return TRUE;
    
}

#endif
