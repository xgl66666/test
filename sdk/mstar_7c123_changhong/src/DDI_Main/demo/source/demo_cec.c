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

//-----------------------------------------------------------------------------
//
/// @file   Demo_CEC.c
/// @author MStar Semiconductor Inc.
/// @brief  CEC Demo code
//
// @name Box App Command Usage
// @verbatim
//-----------------------------------------------------------------------------

#if (DEMO_CEC_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsTypes2.h"
#include <stdio.h>
#include <fcntl.h>
#include "string.h"
#include "MsCommon.h"
#include "MsMemory.h"
#include "demo_cec.h"
#include "demo_xc.h"

#include "msAPI_CEC.h"

#if (DEMO_PM_TEST == 1)
#include "demo_pm.h"
#endif

#include "drvDTC.h"  //Fix MS data type

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define CEC_DEBUG 1

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

#if CEC_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL _bCECInit = FALSE;

//CEC Message bugger
E_MSAPI_CEC_MESSAGE_BUFFER* g_stCECMessageBuffer = 0;

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL _Demo_CEC_Response(MS_U8 u8header, MS_U8 u8opcode , MS_U8 *pu8Cmd, MS_U16 u16ContentLen)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8index = 0;

    printf("\033[0;32;34m""[DDI] Recieve CEC command:  u8header:%X u8opcode:%X cmdlen = %d at time %"DTC_MS_U32_d" ms\r\n""\033[m",u8header,u8opcode,u16ContentLen,MsOS_GetSystemTime());
    for(u8index = 0;u8index < u16ContentLen;u8index++)
    {
        printf("\033[0;32;34m""pu8Cmd[%d] = 0x%02X  ""\033[m",u8index,pu8Cmd[u8index]);
    }
    printf("\n");

    switch(u8opcode)
    {
        case E_MSAPI_CEC_STANDBY:
            // Switch the device into the Standby state
            printf("Standby! \n");
            #if (DEMO_PM_TEST == 1)
            #if (DEMO_PM_HK_LINUX_TEST == 1) || (DEMO_PM_HK_eCos_TEST == 1)
            db_print("DEMO_PM_HK_LINUX_TEST = 1 or DEMO_PM_HK_eCos_TEST = 1, Not support PM Powerdown! \n");
            #else
            printf("Demo_PM_PowerDown() \n");
            //Demo_PM_PowerDown();  //For 882C and TV test, mark powerdown to avoid restarting every time.
            #endif
            #endif

            break;

        case E_MSAPI_CEC_DMC_MENU_REQUEST:
            // HDMI Spec : A request from the TV for a device to show/remove a menu
            // or to query if a device is currently showing a menu
            printf("\033[1;33m""<E_MSAPI_CEC_DMC_MENU_REQUEST> pu8Cmd = 0x%02X\n""\033[m",pu8Cmd[0]);
            switch (pu8Cmd[0])
            {
                case E_MSAPI_CEC_MENU_REQ_ACTIVATED:

                    // TBD : UI Show Menu!!!!!!!!

                    break;

                case E_MSAPI_CEC_MENU_REQ_DEACTIVATED:

                    // TBD : UI Remove Menu!!!!!!!!

                    break;

                case E_MSAPI_CEC_MENU_REQ_QUERY:
                default:
                    break;
            }
            break;

        case E_MSAPI_CEC_UI_PRESS:
            //TBD: Interprets keys in same way as local key presses
            printf("\033[1;33m""<E_MSAPI_CEC_UI_PRESS> pu8Cmd = 0x%02X\n""\033[m",pu8Cmd[0]);

            switch (pu8Cmd[0])
            {
                case E_MSAPI_CEC_UI_SELECT:
                    printf("E_MSAPI_CEC_UI_SELECT\n");
                    break;

                case E_MSAPI_CEC_UI_UP:
                    printf("E_MSAPI_CEC_UI_UP\n");
                    break;

                case E_MSAPI_CEC_UI_DOWN:
                    printf("E_MSAPI_CEC_UI_DOWN\n");
                    break;

                case E_MSAPI_CEC_UI_LEFT:
                    printf("E_MSAPI_CEC_UI_LEFT\n");
                    break;

                case E_MSAPI_CEC_UI_RIGHT:
                    printf("E_MSAPI_CEC_UI_RIGHT\n");
                    break;

                case E_MSAPI_CEC_UI_EXIT:
                    printf("E_MSAPI_CEC_UI_EXIT\n");
                    break;

                case E_MSAPI_CEC_UI_PLAY:
                    db_print("UI_PLAY!\n");
                    break;

                case E_MSAPI_CEC_UI_STOP:
                    db_print("UI_STOP!\n");
                    break;

                case E_MSAPI_CEC_UI_PAUSE:
                    db_print("UI_PAUSE!\n");
                    break;

                case E_MSAPI_CEC_UI_REWIND:
                    db_print("UI_REWIND!\n");
                    break;

                case E_MSAPI_CEC_UI_FAST_FORWARD:
                    db_print("UI_FAST_FORWARD!\n");
                    break;

                default:
                    break;
            }
            break;

        case E_MSAPI_CEC_DC_DECK_CONTROL:
            // TBD: Perform the specified commands actions, or return a <feature abort> message.
            printf("<Deck Control> \n");
            printf("pu8Cmd = 0x%02X \n",pu8Cmd[0]);

            switch(pu8Cmd[0])
            {
                case E_MSAPI_CEC_DCM_CMD_SKIP_FORWARD:
                case E_MSAPI_CEC_DCM_CMD_SKIP_REVERSE:
                case E_MSAPI_CEC_DCM_CMD_STOP:
                case E_MSAPI_CEC_DCM_CMD_EJECT:
                default:
                    break;
            }
            break;

        case E_MSAPI_CEC_DC_PLAY:
            // TBD: Perform the specified commands actions, or return a <feature abort> message.
            printf("<PLAY> \n");
            printf("pu8Cmd = 0x%02X \n",pu8Cmd[0]);

            switch(pu8Cmd[0])
            {
                case E_MSAPI_CEC_DI_CMD_PLAY:
                default:
                    break;
            }
            break;

        case  E_MSAPI_CEC_OSDNT_GIVE_OSD_NAME:
            //GIVE OSD Name
            printf("GIVE OSD Name\n");
            sprintf((char *)pu8Cmd,"MSTAR");

            break;

        default:
            db_print("Not support yet! ");
            bRet = FALSE;
            break;
    }
    return bRet;

}

//------------------------------------------------------------------------------
/// @brief The sample code to init CEC
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_init(void)
{
    if (_bCECInit)
    {
        printf("[CEC] Already init CEC");
        return FALSE;
    }

    //Init XC first
    MS_U32 u32XCDevice = 0; //0 HDMI
    Demo_XC_Init(&u32XCDevice);

    // Set CB function for some CEC features
    msAPI_CECSetResponseCB(_Demo_CEC_Response);

    // Creater CEC message buffer
    g_stCECMessageBuffer = (E_MSAPI_CEC_MESSAGE_BUFFER*)malloc(sizeof(E_MSAPI_CEC_MESSAGE_BUFFER) * CEC_MESSAGE_BUFFER_SIZE);
    if (g_stCECMessageBuffer == 0)
    {
        printf("[CEC] allocate CEC Message Buffer Fail\n");
        return FALSE;
    }
    memset(g_stCECMessageBuffer, 0, sizeof(E_MSAPI_CEC_MESSAGE_BUFFER) * CEC_MESSAGE_BUFFER_SIZE);

    _bCECInit = TRUE;

    return msAPI_CEC_Init();
}

//------------------------------------------------------------------------------
/// @brief The sample code to exit CEC
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Exit \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_Exit(void)
{
    if (_bCECInit == FALSE)
    {
        return TRUE;
    }
    _bCECInit = FALSE;

    if (g_stCECMessageBuffer != 0)
    {
        free(g_stCECMessageBuffer);
        g_stCECMessageBuffer = NULL;
    }

    return msAPI_CEC_Exit();
}

//------------------------------------------------------------------------------
/// @brief The sample code to Pause/Resume CEC
/// @param[in] *pu32CECOnOff  Pause/Resume
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_SetOnoff 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_SetOnOff(MS_U32 *pu32CECOnOff)
{
    MS_BOOL bRet = FALSE;
    MS_BOOL bStart = *pu32CECOnOff;
    bRet = msAPI_CEC_SetOnOff(bStart);
#if (DEMO_XC_HDMIRX_TEST == 1)
    if(bStart)
    {
        msAPI_XC_HDMIRx_SetHPD(E_MSAPI_XC_HDMIRX_PORT0, FALSE, ENABLE);
        msAPI_XC_HDMIRx_SetHPD(E_MSAPI_XC_HDMIRX_PORT0, TRUE, ENABLE);
    }
#endif
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to send CEC message for some CEC feature
/// @param[in] *pu32Opcode the Opcode of CEC message
/// @param[in] *pu32Operand the Operand of some CEC Opcode
/// @param[in] *pu32Length the length of CEC message
/// @param[in] *pu32Dest the destination of CEC message
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_SendMessage 0x82 0 2 F //Active Source \n
///          \b CEC_SendMessage 0x04 0 0 0 //Image View On \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_SendMessage(MS_U32 *pu32Opcode , MS_U8 *pu8Operand , MS_U32 *pu32Length, MS_U32 *pu32Dest)
{
    MSAPI_CEC_MESSAGE stmsAPI_CEC_Message;

    memset(&stmsAPI_CEC_Message, 0, sizeof(MSAPI_CEC_MESSAGE));

    stmsAPI_CEC_Message.eOpcode = *pu32Opcode;
    stmsAPI_CEC_Message.pOperand = pu8Operand;
    stmsAPI_CEC_Message.eDeviceLA = *pu32Dest;
    stmsAPI_CEC_Message.u8Length = *pu32Length;

    return msAPI_CEC_SendMessage(stmsAPI_CEC_Message);
}

//------------------------------------------------------------------------------
/// @brief The sample code to show CEC device list
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Show_Device_List \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_Show_Device_List(void)
{
    MSAPI_XC_DEV_INFO _st_Device_List[16];
    MS_U8 _u8_logical_address;
    MS_U8 _u8_device_maximum = 16;
    MS_U16 _u16_empty_address = 0xFFFF;

    printf("Scanning......\n");
    if(msAPI_CEC_Get_Device_List(&_st_Device_List[0]))
    {
        printf("=============================================\n");
        for(_u8_logical_address=0;_u8_logical_address<_u8_device_maximum;_u8_logical_address++)
        {
            if(_st_Device_List[_u8_logical_address].physical_addr != _u16_empty_address)
                printf("Logical Address:%02d,  Physical Address(%04X).\n",_u8_logical_address,_st_Device_List[_u8_logical_address].physical_addr);
        }
        printf("=============================================\n");
    }
    else
    {
        printf("Scan Failed !\n");
        return FALSE;
    }
    printf("Scan Finished !\n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show CEC device list
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_ShowMessageBuffer \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_ShowMessageBuffer(void)
{
    if (_bCECInit == FALSE)
    {
        printf("[CEC] Please Init CEC first");
        return FALSE;
    }

    MS_U8 u8LatestIdx = 0;
    MS_U8 u8BufIdx = 0;
    MS_U8 u8index = 0;
    MS_U8 u8OperandIndex = 0;

    //Get CEC message Buffer
    if(msAPI_CEC_GetMessageBuffer(g_stCECMessageBuffer, &u8LatestIdx) == FALSE)
    {
        printf("[CEC] Get CEC message buffer fail \n");
        return FALSE;
    }

    printf("[CEC] Sys Time = %"DTC_MS_U32_d" \n",MsOS_GetSystemTime());
    for(u8index = 0; u8index < CEC_MESSAGE_BUFFER_SIZE; u8index++)
    {
        // Print message from the latest message index in decreasing order EX:  2 1 0 9 8 ...
        u8BufIdx = (u8LatestIdx + CEC_MESSAGE_BUFFER_SIZE - u8index) % CEC_MESSAGE_BUFFER_SIZE; // Avoid negative number
        printf("System time = %10"DTC_MS_U32_d" ",g_stCECMessageBuffer[u8BufIdx].u32RecvTime);
        printf("Header = 0x%2X ",g_stCECMessageBuffer[u8BufIdx].u8Header);
        printf("Opcode = 0x%2X ",g_stCECMessageBuffer[u8BufIdx].u8Opcode);

        //Print Operand
        if (g_stCECMessageBuffer[u8BufIdx].u8Length > 2)
        {
            printf("Operand = ");
            // Start from 2 (Opcode and Header)
            for (u8OperandIndex = 2; u8OperandIndex < g_stCECMessageBuffer[u8BufIdx].u8Length; u8OperandIndex++)
            {
                printf("0x%2X ",g_stCECMessageBuffer[u8BufIdx].u8Operand[u8OperandIndex]);
            }
        }
        printf(" \n");
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to send one touch play message
/// @param[in]
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Show_Device_List \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_OneTouchPlay(void)
{
    return msAPI_CEC_OneTouchPlay();
}

//------------------------------------------------------------------------------
/// @brief The sample code to send standby message
/// @param[in] *pu32DeviceLA the target device logic address
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Show_Device_List \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_Standby(MS_U32 *pu32DeviceLA)
{
    return msAPI_CEC_Standby(*pu32DeviceLA);
}

//------------------------------------------------------------------------------
/// @brief The sample code to send standby message
/// @param[in] *pu32DeviceLA the target device logic address
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Show_Device_List \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_GiveDevicePowerStatus(MS_U32 *pu32DeviceLA)
{
    return msAPI_CEC_GiveDevicePowerStatus(*pu32DeviceLA);
}

//------------------------------------------------------------------------------
/// @brief The sample code to send standby message
/// @param[in] *pu32Enable request audio On of Off
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b CEC_Show_Device_List \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CEC_SystemAudioModeRequest(MS_U32 *pu32Enable)
{
    return msAPI_CEC_SystemAudioModeRequest(*pu32Enable);
}

#endif
