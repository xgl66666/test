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
/// file   apiCellular.c
/// @brief  Mobile Broadband API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined(NMGR_ENABLE) && (NMGR_ENABLE==1)
#if defined(CELLULAR_ENABLE) && (CELLULAR_ENABLE==1)

#include "MsCommon.h"
#include "HbCommon.h"
#include "apiCellular.h"

#if defined (MSOS_TYPE_ECOS)
#include <network.h>
#include <cyg/ppp/ppp_io.h>
#include <cyg/ppp/pppd.h>
#include <cyg/io/serialio.h>
#include "drvMobileBroadband_Modem.h"

extern char *ip_ntoa(u_int32_t ipaddr);
extern unsigned long dns_server[2];
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DECLARE_VID_PID(VID, PID) (((MS_U32)VID & 0xFFFF) << 16 | ((MS_U32)PID & 0xFFFF))
#define SIMCOM_SIM5360E DECLARE_VID_PID(0x05C6, 0x9000)
#define HAUWEI_EC122    DECLARE_VID_PID(0x12D1, 0x140C)


#define PPP_TPS                 (1000000000/(CYGNUM_HAL_RTC_NUMERATOR/CYGNUM_HAL_RTC_DENOMINATOR))
#define INIT_DELAY_MS           5000
#define CELLULAR_MUTEX_TIMEOUT  10000

#define AT_CGDCONT_FRONT        "AT+CGDCONT=1,\"IP\",\""
#define AT_CGDCONT_END          "\",,0,0"
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32 s32StateMutex = -1;
static cyg_ppp_handle_t ppp_handle;
//inserted device name
static MS_U8 u8USBDevName[20] = {0};
//state
static CELLULAR_PHASE_STATE ePhaseState = PHASE_UNPLUGGED;
//state handler callback function
static void (*CellularStateHandler_CB)(CELLULAR_PHASE_STATE);
//System Setting
static MS_U8 u8SettingAPN[MAX_STRING_LENGTH] = {0};
static MS_U8 u8SettingUsername[MAX_STRING_LENGTH] = {0};
static MS_U8 u8SettingPassword[MAX_STRING_LENGTH] = {0};
static MS_BOOL bSettingCellularOn = TRUE;
static MS_BOOL bSettingRoamingOn = FALSE;

static cyg_ppp_options_t options;

//AT Command
static char apn_script[60] = {0};

static char *gsm_dial_script[] =
{
    "ABORT"             ,       "BUSY"                                  ,
    "ABORT"             ,       "NO CARRIER"                            ,
    "ABORT"             ,       "ERROR"                                 ,
    ""                  ,       "ATZ"                                   ,
    "OK"                ,       "AT S7=45 S0=0 L1 V1 X4 &C1 E1 Q0"      ,
    "OK"                ,       apn_script                              ,
    "OK"                ,       "ATDT*99#"                              ,
    "CONNECT 115200"    ,       ""                                      ,
    0
};

static char *cdma_dial_script[] =
{
    "ABORT"             ,       "BUSY"                                  ,
    "ABORT"             ,       "NO CARRIER"                            ,
    "ABORT"             ,       "ERROR"                                 ,
    ""                  ,       "ATZ"                                   ,
    "OK"                ,       "ATQ0 V1 E1 S0=0 &C1 &D2 +FCLASS=0"     ,
    "OK"                ,       "ATDT#777"                              ,
    "CONNECT"           ,       ""                                      ,
    0
};

static char *cpin_script[] =
{
    ""                  ,       "AT+CPIN?"                              ,
    "OK"                ,       ""                                      ,
    0
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _Cellular_Ready(void)
{
    MsOS_DelayTask(INIT_DELAY_MS);
    if(cyg_ppp_chat((const char *)u8USBDevName, (const char **)&cpin_script[0]))
    {
        diag_printf("\n\n>>Cellular Ready<<\n\n");
        return TRUE;
    }
    diag_printf("\n\n>>Cellular Not Ready<<\n\n");
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _Config_EC122(void)
{
    diag_printf("Config for EC122\n");

    //option initial
    options.debug              = 0;
    options.kdebugflag         = 0;
    options.default_route      = 1;
    options.modem              = 1;
    options.flowctl            = CYG_PPP_FLOWCTL_HARDWARE;
    options.refuse_pap         = 0;
    options.refuse_chap        = 0;
    options.baud               = CYGNUM_SERIAL_BAUD_9600;
    options.idle_time_limit    = 0;
    options.maxconnect         = 0;
    options.our_address        = 0;
    options.his_address        = 0x4040400A; //default: 10.64.64.64
    options.script             = &cdma_dial_script[0];

    strcpy((char*)u8SettingAPN,"CDMA");

    memset(options.user, 0, MAXNAMELEN);
    memset(options.passwd, 0, MAXSECRETLEN);

    if(*u8SettingUsername!=0)
    {
        strcpy(options.user,(char*)u8SettingUsername);
        HB_printf( ">>USER NAME:%s\n",options.user);
    }

    if(*u8SettingPassword!=0)
    {
        strcpy(options.passwd,(char*)u8SettingPassword);
        HB_printf( ">>PASSWD:%s\n",options.passwd);
    }

}

static void _Config_SIM5360E(void)
{
    diag_printf("Config for SIM5360E\n");
    //setup apn_script
    memset(apn_script, 0x0, sizeof(apn_script));
    strcpy(apn_script, AT_CGDCONT_FRONT);
    strcat(apn_script, (const char *)u8SettingAPN);
    strcat(apn_script, AT_CGDCONT_END);

    //option initial
    options.debug              = 0;
    options.kdebugflag         = 0;
    options.default_route      = 1;
    options.modem              = 1;
    options.flowctl            = CYG_PPP_FLOWCTL_HARDWARE;
    options.refuse_pap         = 0;
    options.refuse_chap        = 0;
    options.baud               = CYGNUM_SERIAL_BAUD_115200;
    options.idle_time_limit    = 0;
    options.maxconnect         = 0;
    options.our_address        = 0;
    options.his_address        = 0x4040400A; //default: 10.64.64.64
    options.script             = &gsm_dial_script[0];
    memset(options.user, 0, MAXNAMELEN);
    memset(options.passwd, 0, MAXSECRETLEN);

}

static MS_BOOL _Cellular_PppUp(void)
{
    // Bring up the TCP/IP network
    init_all_network_interfaces();

    MS_U32 vid_pid=MDrv_MobileBroadband_Modem_GetVidPid();

    //option initial
    switch(vid_pid)
    {
        case HAUWEI_EC122:
            _Config_EC122();
            break;

        case SIMCOM_SIM5360E:
            _Config_SIM5360E();
            break;

        default:
           diag_printf("Dongle Not Support!! (VID_PID=%x)\n",vid_pid);
           break;
    }

    //ppp up
    HB_printf( "Bringing PPP up\n");
    ppp_handle = cyg_ppp_up((const char *)u8USBDevName, &options );

    if(ppp_handle != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void MApi_Cellular_APNChange(void)
{
    while(MsOS_ObtainMutex(s32StateMutex, CELLULAR_MUTEX_TIMEOUT) == FALSE)
    {
        HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        MsOS_DelayTask(10);
    }
    if(ePhaseState >= PHASE_INIT && ePhaseState <= PHASE_INTERNET)
    {
        ePhaseState = PHASE_DOWN;
    }
    else if (ePhaseState == PHASE_FAIL)
    {
        ePhaseState = PHASE_UP;
    }
    MsOS_ReleaseMutex(s32StateMutex);
}

void MApi_Cellular_SetupAPN(MS_U8* pu8Str)
{
    if(pu8Str != NULL)
    {
        memset(u8SettingAPN, 0, sizeof(u8SettingAPN));
        strcpy((char *)u8SettingAPN, (const char *)pu8Str);
        MApi_Cellular_APNChange();
    }
    else
    {
        memset(u8SettingAPN, 0, sizeof(u8SettingAPN));
    }
    return;
}
void MApi_Cellular_SetupUsername(MS_U8* pu8Str)
{
    if(pu8Str != NULL)
    {
        strcpy((char *)u8SettingUsername, (const char *)pu8Str);
    }
    else
    {
        memset(u8SettingUsername, 0, sizeof(u8SettingUsername));
    }
    return;
}
void MApi_Cellular_SetupPassword(MS_U8* pu8Str)
{
    if(pu8Str != NULL)
    {
        strcpy((char *)u8SettingPassword, (const char *)pu8Str);
    }
    else
    {
        memset(u8SettingPassword, 0, sizeof(u8SettingPassword));
    }
    return;
}
void MApi_Cellular_SetupCelluarOn(MS_BOOL bOn)
{
    bSettingCellularOn = bOn;
    return;
}
void MApi_Cellular_SetupRoamingOn(MS_BOOL bOn)
{
    bSettingRoamingOn = bOn;
    return;
}

void MApi_Cellular_RegisterStateHandler_CB(void (*CallBackFn)(CELLULAR_PHASE_STATE))
{
    CellularStateHandler_CB = CallBackFn;
    return;
}

void MApi_Cellular_Reset(void)
{
    if(s32StateMutex < 0)
    {
        s32StateMutex = MsOS_CreateMutex(E_MSOS_FIFO, "Cellular_Mutex",MSOS_PROCESS_SHARED);
        if(s32StateMutex < 0)
        {
            HB_GEN_EXCEP;
        }
    }
    while(MsOS_ObtainMutex(s32StateMutex, CELLULAR_MUTEX_TIMEOUT) == FALSE)
    {
        HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        MsOS_DelayTask(10);
    }

    if(ePhaseState >= PHASE_INIT && ePhaseState <= PHASE_INTERNET)
    {
        HB_printf( "Bringing PPP down\n");
        cyg_ppp_down(ppp_handle);
        HB_printf( "Waiting for PPP to go down\n");
        cyg_ppp_wait_down(ppp_handle);
        HB_printf( "PPP down\n");

        if(bSettingCellularOn == FALSE || *u8SettingAPN == 0)
        {
            ePhaseState = PHASE_READY;
        }
        else
        {
            ePhaseState = PHASE_INIT;
        }
        if(CellularStateHandler_CB != NULL)
        {
            CellularStateHandler_CB(ePhaseState);
        }
    }
    MsOS_ReleaseMutex(s32StateMutex);

}

MS_BOOL MApi_Cellular_SetupPlugin(MS_BOOL bInserted, MS_U8* pu8Str)
{
    MS_BOOL bRet = FALSE;

    if(s32StateMutex < 0)
    {
        s32StateMutex = MsOS_CreateMutex(E_MSOS_FIFO, "Cellular_Mutex",MSOS_PROCESS_SHARED);
        if(s32StateMutex < 0)
        {
            HB_GEN_EXCEP;
            return FALSE;
        }
    }
    while(MsOS_ObtainMutex(s32StateMutex, CELLULAR_MUTEX_TIMEOUT) == FALSE)
    {
        HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        MsOS_DelayTask(10);
    }

    if(bInserted == TRUE)
    {
        if(ePhaseState == PHASE_UNPLUGGED)
        {
            if(pu8Str != NULL)
            {
                ePhaseState = PHASE_PLUGGED;
                if(CellularStateHandler_CB != NULL)
                {
                    CellularStateHandler_CB(ePhaseState);
                }
                strcpy((char *)u8USBDevName, (const char *)pu8Str);
                bRet = TRUE;
            }
        }
    }
    else
    {
        //unplug the cellular device actived currently
        if(*u8USBDevName != 0 && pu8Str != NULL && (strcmp((const char *)u8USBDevName, (const char *)pu8Str) == 0))
        {
            CELLULAR_PHASE_STATE ePrePhaseState = ePhaseState;
            ePhaseState = PHASE_UNPLUGGED;
            if(CellularStateHandler_CB != NULL)
            {
                CellularStateHandler_CB(ePhaseState);
            }

            if(ePrePhaseState >= PHASE_INIT && ePrePhaseState <= PHASE_INTERNET)
            {
                HB_printf( "Bringing PPP down\n");
                cyg_ppp_down(ppp_handle);
                HB_printf( "Waiting for PPP to go down\n");
                cyg_ppp_wait_down(ppp_handle);
                HB_printf( "PPP down\n");
            }
            memset(u8USBDevName, 0, sizeof(u8USBDevName));
            bRet = TRUE;

        }
    }
    MsOS_ReleaseMutex(s32StateMutex);
    return bRet;
}

MS_BOOL MApi_Cellular_StateMonitor(void)
{
    MS_S32 s32LinkPhase;

    if(s32StateMutex < 0)
    {
        s32StateMutex = MsOS_CreateMutex(E_MSOS_FIFO, "Cellular_Mutex",MSOS_PROCESS_SHARED);
        if(s32StateMutex < 0)
        {
            HB_GEN_EXCEP;
            return FALSE;
        }
    }
    while(MsOS_ObtainMutex(s32StateMutex, CELLULAR_MUTEX_TIMEOUT) == FALSE)
    {
        HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__);
        MsOS_DelayTask(10);
    }

    if(CellularStateHandler_CB != NULL)
    {
        CellularStateHandler_CB(ePhaseState);
    }

    switch(ePhaseState)
    {
        case PHASE_UNPLUGGED:
            break;
        case PHASE_PLUGGED:
            //Check SIM Card inserted?
            if(_Cellular_Ready())
            {
                ePhaseState = PHASE_READY;
            }
            break;
        case PHASE_READY:
            if(bSettingCellularOn == TRUE)
            {
                ePhaseState = PHASE_UP;
            }
            break;
        case PHASE_UP:
            if(bSettingCellularOn == TRUE)
            {
                if(_Cellular_PppUp())
                {
                    ePhaseState = PHASE_INIT;
                }
            }
            else
            {
                ePhaseState = PHASE_READY;
            }
            break;
        case PHASE_INIT:
            if(cyg_ppp_wait_up(ppp_handle) == 0)
            {
                ePhaseState = PHASE_CONNECT;
            }
            else
            {
                ePhaseState = PHASE_DOWN;
            }
            break;
        case PHASE_CONNECT:
            s32LinkPhase = cyg_ppp_phase_status(ppp_handle);
            if(cyg_ppp_is_up(ppp_handle) == 0)
            {
                cyg_ppp_neg_addrs_t neg_addrs;
                if(cyg_ppp_get_neg_addrs(&neg_addrs))
                {
                    HB_printf( "local  IP address %s\n", ip_ntoa(neg_addrs.local_ip));
                    HB_printf( "remote IP address %s\n", ip_ntoa(neg_addrs.peer_ip));
                    HB_printf( "primary   DNS address %s\n", ip_ntoa(neg_addrs.pri_dns));
                    HB_printf( "secondary DNS address %s\n", ip_ntoa(neg_addrs.alt_dns));
                    if(neg_addrs.pri_dns)
                    {
                        inet_pton(AF_INET, (char*)ip_ntoa(neg_addrs.pri_dns), (void *)&dns_server[0]);
                    }
                    if(neg_addrs.alt_dns)
                    {
                        inet_pton(AF_INET, (char*)ip_ntoa(neg_addrs.alt_dns), (void *)&dns_server[1]);
                    }
                    cyg_dns_res_init((struct in_addr *)&dns_server[0]);
                    setdomainname(NULL,0);

                    HB_printf( "Delaying for a while...\n");
                    cyg_thread_delay(5*PPP_TPS);
                    HB_printf( "PPP up\n");
                    ePhaseState = PHASE_INTERNET;
                }
                else
                {
                    ePhaseState = PHASE_DOWN;
                }
            }
            else if(s32LinkPhase == PHASE_DEAD)
            {
                ePhaseState = PHASE_DOWN;
            }
            break;
        case PHASE_INTERNET:
            if(bSettingCellularOn == FALSE || cyg_ppp_is_up(ppp_handle) != 0)
            {
                ePhaseState = PHASE_DOWN;
            }
            break;
        case PHASE_DOWN:
            s32LinkPhase = cyg_ppp_phase_status(ppp_handle);
            HB_printf( "Bringing PPP down\n");
            cyg_ppp_down(ppp_handle);
            HB_printf( "Waiting for PPP to go down\n");
            cyg_ppp_wait_down(ppp_handle);
            HB_printf( "PPP down\n");
            if(bSettingCellularOn == FALSE)
            {
                ePhaseState = PHASE_READY;
            }
            else if(s32LinkPhase != PHASE_CONNECT)
            {
                ePhaseState = PHASE_FAIL;
            }
            else
            {
                ePhaseState = PHASE_UP;
            }
            break;
        case PHASE_FAIL:
            break;
        default:
            return FALSE;
    }

    MsOS_ReleaseMutex(s32StateMutex);
    return TRUE;
}

CELLULAR_PHASE_STATE MApi_Cellular_GetState(void)
{
    return ePhaseState;
}

#endif
#endif
#endif
