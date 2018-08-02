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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appTemplate.h
/// @author MStar Semiconductor Inc.
/// @brief  Uranus Sample Code Template
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
   @file appTemplate.c
   @brief The Template code for the MDrv_XXX functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_NET_ECOS_TEST == 1)
#include "MsCommon.h"
#include "apiLAN.h"

#include <network.h>
#include <dhcp.h>

#include "demo_utility.h"
#include "demo_net_ecos.h"

#define FUNC_DBG(format, args...) printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define DEMO_EVENT_WAIT_FOREVER 0xffffffff

#define LAN_DHCP_START 0x1

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////
//  Lan Structure Type
//////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    MS_U8      u8Mac[6];
    MS_BOOL    bDhcpOn;
    MS_U8      u8IpAdr[4];
    MS_U8      u8SubNet[4];
    MS_U8      u8GatWay[4];
}ST_NET_USER_Lan_SET;

//------------------------------------------------------------------------------
/// Initiate Lan .
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//------------------------------------------------------------------------------
static MS_S32  _s32LanEventId = 0;
static Task_Info _appLan_DHCPTask = {-1, E_TASK_PRI_LOW, NULL, 4096, "DHCP Task"};
static MS_BOOL _bDHCP = FALSE;


static void _Demo_Net_LanDhcp_task(MS_U32 argc, VOID *argv)
{
    MS_U32 u32Event;

    while(1)
    {
        if (MsOS_WaitEvent(_s32LanEventId,
                           LAN_DHCP_START,
                           &u32Event,
                           E_AND_CLEAR,
                           DEMO_EVENT_WAIT_FOREVER))
        {
            MS_U32 u32StartTime = MsOS_GetSystemTime();
            FUNC_DBG("LAN DHCP Start......\n");
            MApi_LAN_DhcpConfig(1);
            while(MApi_LAN_GetDhcpState() != DHCPSTATE_BOUND
                &&MApi_LAN_GetDhcpState() != DHCPSTATE_BOOTP_FALLBACK
                &&MApi_LAN_GetDhcpState() != DHCPSTATE_FAILED)
            {
                FUNC_DBG("Doing DHCP,Waiting:0x%x\n",MApi_LAN_GetDhcpState());
                MsOS_DelayTask(1000);
                if(MsOS_GetSystemTime() - 20000 > u32StartTime)
                    break;
            }
            if(MApi_LAN_GetDhcpState() == DHCPSTATE_BOUND) //  had reci the request response
            {
                FUNC_DBG("DHCP Success\n");
                MApi_LAN_ShowIntf();
    //            show_network_tables((pr_fun *)HB_printf);
            }
            else
            {
                FUNC_DBG("DHCP Failed\n");
            }
            MApi_LAN_DhcpConfig(0);
        }
        
        if (!_bDHCP)
            return;
    }
}

//------------------------------------------------------------------------------
/// @brief Enable DHCP.
/// @return NA
/// @sa
/// @note
/// Command: \b Net_DHCP\n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_DHCP(MS_U32* u32DHCP)
{
    if (*u32DHCP == 0)
    {
        _bDHCP = FALSE;
    }
    else
    {
        _bDHCP = TRUE;
    }
    return TRUE;
}

static MS_BOOL _Demo_Net_LanInit(void) //move to Tv Let Logo show first
{
    MS_S32 s32MstarCachedPoolID;
    printf("before Demo_Util_GetSystemPoolID \n");
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarCachedPoolID);
    printf("after Demo_Util_GetSystemPoolID \n");
    // Check if EMAC is initialized then bring up all network interfaces.
    while (FALSE == MApi_LAN_EthReady());
    {
        FUNC_DBG("in MApi_LAN_IntfReset loops \n");

        printf("in MApi_LAN_IntfReset loops \n");
        MApi_LAN_IntfReset();
    }

    printf("after MApi_LAN_IntfReset loops \n");

    ST_NET_USER_Lan_SET stLanSetting;

    stLanSetting.bDhcpOn = _bDHCP;
    stLanSetting.u8IpAdr[0] = 192;
    stLanSetting.u8IpAdr[1] = 168;
    stLanSetting.u8IpAdr[2] = 123;
    stLanSetting.u8IpAdr[3] = 2;

    stLanSetting.u8SubNet[0] = 255;
    stLanSetting.u8SubNet[1] = 255;
    stLanSetting.u8SubNet[2] = 255;
    stLanSetting.u8SubNet[3] = 0;

    stLanSetting.u8GatWay[0] = 192;
    stLanSetting.u8GatWay[1] = 168;
    stLanSetting.u8GatWay[2] = 0;
    stLanSetting.u8GatWay[3] = 101;

    FUNC_DBG("LAN Set MAC Done\n");
    if(stLanSetting.bDhcpOn == TRUE)
    {

        if(_appLan_DHCPTask.iId == -1) // task not created
        {
            _appLan_DHCPTask.pStack = MsOS_AllocateMemory(_appLan_DHCPTask.u32StackSize, s32MstarCachedPoolID);

            if(!_appLan_DHCPTask.pStack)
            {
                return FALSE;
            }

            _appLan_DHCPTask.iId = MsOS_CreateTask( _Demo_Net_LanDhcp_task,
                                            (MS_U32)NULL,
                                            _appLan_DHCPTask.ePriority,
                                            TRUE,
                                            _appLan_DHCPTask.pStack,
                                            _appLan_DHCPTask.u32StackSize,
                                            _appLan_DHCPTask.szName);
            if(_appLan_DHCPTask.iId < 0)
            {
                return FALSE;
            }

            /// - Create OS Event Flag
            _s32LanEventId = MsOS_CreateEventGroup("Lan DHCP");

            if (_s32LanEventId < 0)
            {
                return FALSE;
            }


        }

        FUNC_DBG("nodify LAN task Start......\n");
        MsOS_SetEvent(_s32LanEventId, LAN_DHCP_START);
    }
    else
    {
        MS_U8 au8Buf[32];

        FUNC_DBG("LAN FIX Setting Config Start......\n");

        //MApi_LAN_DhcpConfig(0);
        // IP config
        sprintf((char*)au8Buf,"%d.%d.%d.%d",\
                stLanSetting.u8IpAdr[0],\
                stLanSetting.u8IpAdr[1],\
                stLanSetting.u8IpAdr[2],\
                stLanSetting.u8IpAdr[3]);
        FUNC_DBG("Configing Fix IP:%s\n",au8Buf);
        MApi_LAN_IpConfig(au8Buf);

        // Subnet config
        sprintf((char*)au8Buf,"%d.%d.%d.%d",\
                stLanSetting.u8SubNet[0],\
                stLanSetting.u8SubNet[1],\
                stLanSetting.u8SubNet[2],\
                stLanSetting.u8SubNet[3]);
        FUNC_DBG("Configing Fix SubNet:%s\n",au8Buf);
        MApi_LAN_SubnetConfig(au8Buf);

        // Gate way config
        sprintf((char*)au8Buf,"%d.%d.%d.%d",\
                stLanSetting.u8GatWay[0],\
                stLanSetting.u8GatWay[1],\
                stLanSetting.u8GatWay[2],\
                stLanSetting.u8GatWay[3]);
        FUNC_DBG("Configing Fix Gateway:%s\n",au8Buf);
        MApi_LAN_GatewaySet(au8Buf);


        MApi_LAN_ShowIntf();


        FUNC_DBG("LAN FIX Setting Config Done......\n");
    }

        MApi_LAN_ShowIntf();
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Initial network settings.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b Demo_Net_Init\n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_Init(void)
{
    printf("Demo_Net_Init \n");
	if(MApi_LAN_init()!=0)
	{
		printf("MApi_LAN_init failed\n");
	}
		
    return _Demo_Net_LanInit();
}

//------------------------------------------------------------------------------
/// @brief check the network connect status.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b Demo_Net_GetConnectionStatus \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_GetConnectionStatus(void)
{
    return MApi_LAN_ConnectStatus();
}

//------------------------------------------------------------------------------
/// @brief get current Mac address
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b Demo_Net_GetMac \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_GetMac(void)
{
    MS_U8 u8IntfName[] = "eth0\0";
    MS_U8 u8MacAddr[10] = {0};
    MS_BOOL bRet = FALSE;
    bRet = MApi_LAN_GetMac(u8IntfName, u8MacAddr);

    if(!bRet)
    {
        return FALSE;
    }

    printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n", u8MacAddr[0], u8MacAddr[1], u8MacAddr[2], u8MacAddr[3], u8MacAddr[4], u8MacAddr[5], u8MacAddr[6], u8MacAddr[7], u8MacAddr[8], u8MacAddr[9]);

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Set Mac address
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b Demo_Net_SetMac \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_SetMac(MS_U32* u32val1, MS_U32* u32val2, MS_U32* u32val3, MS_U32* u32val4, MS_U32* u32val5, MS_U32* u32val6)
{
    //MS_U8 u8IntfName[] = "eth0\0";
    MS_U8 u8MacAddr[6] = {0};

    u8MacAddr[0] = *u32val1;
    u8MacAddr[1] = *u32val2;
    u8MacAddr[2] = *u32val3;
    u8MacAddr[3] = *u32val4;
    u8MacAddr[4] = *u32val5;
    u8MacAddr[5] = *u32val6;

    MS_BOOL bRet = FALSE;
    bRet = MApi_LAN_MacConfig(u8MacAddr);

    return bRet;

}

//------------------------------------------------------------------------------
/// @brief Execute ping function to check if network workable.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b Demo_Net_Ping \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_Ping(MS_U8* ipaddr)
{
    return MApi_LAN_Ping(ipaddr);
}

//------------------------------------------------------------------------------
/// @brief Network Help Hint
/// @return TRUE - Process success
/// @sa
/// @note
/// Command: \b Demo_Net_Help \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_Help(void)
{
    printf ("------------------------------------Network Help--------------------------------------\n");
    printf ("press \"Net_Init\" to Initial Network\n");
    printf ("---------------------------------End of Network Help----------------------------------\n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get DHCP Status \n
///     0 - DHCP IDLE                   \n
///     1 -- DHCPSTATE_INIT             \n
///     2 -- DHCPSTATE_SELECTING        \n
///     3 -- DHCPSTATE_REQUESTING       \n
///     4 -- DHCPSTATE_REQUEST_RECV     \n
///     5 -- DHCPSTATE_BOUND            \n
///     6 -- DHCPSTATE_RENEWING         \n
///     7 -- DHCPSTATE_RENEW_RECV       \n
///     8 -- DHCPSTATE_REBINDING        \n
///     9 -- DHCPSTATE_REBIND_RECV      \n
///     10 -- DHCPSTATE_BOOTP_FALLBACK  \n
///     11 -- DHCPSTATE_NOTBOUND        \n
///     12 -- DHCPSTATE_FAILED          \n
///     13 -- DHCPSTATE_DO_RELEASE
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b Demo_Net_GetDHCPStatus \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Net_GetDHCPStatus(void)
{
    MS_U8 u8Status;
    u8Status = MApi_LAN_GetDhcpState();
    printf ("DHCPStatus = 0x%x \n",u8Status);

    return TRUE;
}

#endif

