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

#if (DEMO_DSCMB_ADVANCED_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
///
/// @file   demo_dscmb_advanced.c
/// @author MStar Semiconductor Inc.
/// @brief  Descrambler Function Sample Demo
///
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"
#if (DEMO_DSCMB_UTPA2_TEST == 1)
#include "utopia.h"
#include "drvDSCMB_v2.h"
#else
#include "drvDSCMB.h"
#endif
#include "demo_av.h"
#include "demo_dscmb_advanced.h"
#include "demo_keyladder.h"
//--------------------------------------------------------------------------------------------------
// Global variable
//--------------------------------------------------------------------------------------------------
static MS_U32 u32DscmbId[2] = {DRV_DSCMB_FLT_NULL,DRV_DSCMB_FLT_NULL};
static MS_U32 u32DscmbEngId = 0;
static MS_U32 u32PidFlt[3] = {0xffff, 0xffff, 0xffff};
static MS_U32 u32CAVid = 0xF;
static DSCMB_KLEng u32EngMode = E_DSCMB_KL_TDES;
#if (DEMO_DSCMB_UTPA2_TEST == 1)
static void *pu32DscmbInst = NULL;
#endif
//--------------------------------------------------------------------------------------------------
// Structure define
//--------------------------------------------------------------------------------------------------

struct
{
    const char *name;
    DSCMB_Algo_Cfg CfgType;
} DSCMB_Type_Map[] =
{
    {
        "CSA", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_CSA2,
            .eSubAlgo = E_DSCMB_SUB_ALGO_NONE,
            .eResAlgo = E_DSCMB_RESSB_ALGO_NONE,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_NONE,
            .bDecrypt = TRUE
        }
    },
    {
        "CSA_CONF", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_CSA2_CONF,
            .eSubAlgo = E_DSCMB_SUB_ALGO_NONE,
            .eResAlgo = E_DSCMB_RESSB_ALGO_NONE,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_NONE,
            .bDecrypt = TRUE
        }
    },
    {
        "CSA3", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_CSA3,
            .eSubAlgo = E_DSCMB_SUB_ALGO_NONE,
            .eResAlgo = E_DSCMB_RESSB_ALGO_NONE,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_NONE,
            .bDecrypt = TRUE
        }
    },
    {
        "CI+DES",{
            .eMainAlgo= E_DSCMB_MAIN_ALGO_DES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_ECB,
            .eResAlgo = E_DSCMB_RESSB_ALGO_CLR,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_CLR,
            .bDecrypt = TRUE
        }
    },
    {
        "CI+AES", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_AES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_CBC,
            .eResAlgo = E_DSCMB_RESSB_ALGO_CLR,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_CLR,
            .bDecrypt = TRUE
        }
    },
    {
        "AES_SCTE52",{
            .eMainAlgo= E_DSCMB_MAIN_ALGO_AES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_CBC,
            .eResAlgo = E_DSCMB_RESSB_ALGO_SCTE52,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_SCTE52,
            .bDecrypt = TRUE
        }
    },
    {
        "TDES_SCTE52", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_TDES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_CBC,
            .eResAlgo = E_DSCMB_RESSB_ALGO_SCTE52,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_SCTE52,
            .bDecrypt = TRUE
        }
    },
    {
        "AES_ECB", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_AES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_ECB,
            .eResAlgo = E_DSCMB_RESSB_ALGO_CLR,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_CLR,
            .bDecrypt = TRUE
        }
    },
    {
        "TDES_ECB", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_TDES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_ECB,
            .eResAlgo = E_DSCMB_RESSB_ALGO_CLR,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_CLR,
            .bDecrypt = TRUE
        }
    },
    {
        "SYN_AES", {
            .eMainAlgo= E_DSCMB_MAIN_ALGO_AES,
            .eSubAlgo = E_DSCMB_SUB_ALGO_LEADING_CLEAR,
            .eResAlgo = E_DSCMB_RESSB_ALGO_NONE,
            .eSBAlgo  = E_DSCMB_RESSB_ALGO_NONE,
            .bDecrypt = TRUE
        }
    },
};

struct
{
    const char *name;
    DSCMB_KLSrc src;
} DSCMB_KLSrc_Map[] =
{
    {"ACPU", E_DSCMB_KL_SRC_ACPU},
    {"SECRET_1", E_DSCMB_KL_SRC_SECRET_1},
    {"SECRET_2", E_DSCMB_KL_SRC_SECRET_2},
    {"SECRET_3", E_DSCMB_KL_SRC_SECRET_3},
    {"SECRET_4", E_DSCMB_KL_SRC_SECRET_4},
    {"SECRET_5", E_DSCMB_KL_SRC_SECRET_5},
    {"SECRET_6", E_DSCMB_KL_SRC_SECRET_6},
    {"SECRET_7", E_DSCMB_KL_SRC_SECRET_7},
    {"SECRET_8", E_DSCMB_KL_SRC_SECRET_8}
};

struct
{
    const char *name;
    DSCMB_KLEng mode;
} DSCMB_KLEng_Map[] =
{
    {"TDES", E_DSCMB_KL_TDES},
    {"AES",  E_DSCMB_KL_AES}
};

struct
{
    MS_BOOL bConnectStatus;
    MS_U8 u8Type[32];
    MS_U8 u8StrOddCW[32];
    MS_U8 u8StrEvenCW[32];
    MS_U8 u8StrOddIV[32];
    MS_U8 u8StrEvenIV[32];
} Demo_DSCMB_Config[2];

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
static int _str2hex(unsigned char *s, const unsigned char *h, int size)
{
    int i, j, len = strlen((const char *)h);
    unsigned char a, b;

    for(i = 0, j = 0; j < size && i < len; j++, i += 2)
    {
        if(h[i] == '\0' || h[i+1] == '\0')
            break;
        a = h[i] <= '9' ? h[i] - '0' : tolower(h[i]) - 'a' + 10;
        b = h[i+1] <= '9' ? h[i+1] - '0' : tolower(h[i+1]) - 'a' + 10;

        s[j] = (a << 4) | b;
    }

    return j;
}

#if 0
static void _dump_buf(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
}
#endif

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
#if (DEMO_DSCMB_UTPA2_TEST == 1)
MS_BOOL Demo_DSCMB_Open(void)
{
    if(pu32DscmbInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, &pu32DscmbInst, 0x0, NULL))
        {
            printf("UtopiaOpen DSCMB fail\n");
            return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL Demo_DSCMB_Close(void)
{
    if(UTOPIA_STATUS_FAIL == UtopiaClose(pu32DscmbInst))
    {
        printf("UtopiaClose DSCMB fail\n");
        return FALSE;
    }

    pu32DscmbInst = NULL;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Allocate a descrambler filter and return descrambler id
/// @param[in]:
/// @return Descrambler id: Process success.
/// @return DRV_DSCMB_FLT_NULL: Process fial.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_U32 Demo_DSCMB_Create_Resource(void)
{
    DSCMB_FLTALLOC pArgs = {u32DscmbEngId, E_DSCMB_FLT_2_KEYS, DRV_DSCMB_FLT_NULL};

    Demo_DSCMB_Open();

    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltAlloc, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_FltAlloc failed\n", __FUNCTION__,__LINE__);
        return DRV_DSCMB_FLT_NULL;
    }

    Demo_DSCMB_Close();

    return pArgs.u32DscmbId;
}

//------------------------------------------------------------------------------
/// @brief Free a descrambler filter
/// @param[in] u32DscmbId the descrambler id
/// @return TRUE: Process success.
/// @return FALSE: Process fial.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_Destroy_Resource(MS_U32 u32DscmbId)
{
    DSCMB_FLTFREE pArgs = {u32DscmbEngId, u32DscmbId};

    //If u32DscmbId == NULL, NO need destory
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return TRUE;
    }

    Demo_DSCMB_Open();

    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltFree, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_FltFree failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Close();

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Connect a pid filter
/// @param[in] u32DscmbId the descrambler id
/// @param[in] u32PidFlt the Pid filter index that get from TSP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_ConnectFltId(MS_U32 u32DscmbId, MS_U32 u32PidFlt)
{
    DSCMB_CONNECTFLT pArgs = {u32DscmbEngId, u32DscmbId, u32PidFlt};

    //If u32DscmbId == NULL, Descrambler filter NOT allocated. CAN NOT connect.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Open();

    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltConnectFltId, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_FltConnectFltId failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Close();

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Disconnect a pid filter
/// @param[in] u32DscmbId the descrambler id
/// @param[in] u32PidFlt the Pid filter index that get from TSP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_DisconnectFltId(MS_U32 u32DscmbId, MS_U32 u32PidFlt)
{
    DSCMB_DISCONNECTFLT pArgs = {u32DscmbEngId, u32DscmbId, u32PidFlt};

    //if u32DscmbId == NULL, Descrambler filter NOT allocated. NO need disconnect.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return TRUE;
    }

    Demo_DSCMB_Open();

    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltDisconnectFltId, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_FltDisconnectFltId failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Close();

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set the descramble algorithm
/// @param[in] u32DscmbId the descrambler id
/// @param[in] u8Type a string represent the algorithm.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetFltType(MS_U32 u32DscmbId, MS_U8 *u8Type)
{
    MS_U32 x = 0;
    MS_U32 len = 0;
    EN_DEMO_DSCMB_TYPE eDSCMBType;
    DSCMB_ENGALGO pArgsAlgo;
    DSCMB_ENGSWITCH pArgsSwitch;

    len = sizeof(DSCMB_Type_Map) / sizeof(DSCMB_Type_Map[0]);

    //if u32DscmbId == NULL, Descrambler filter NOT allocated. Can NOT set fliter type.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    for(x = 0; x < len; x++)
    {
        if(strcmp((char *)u8Type, DSCMB_Type_Map[x].name) == 0)
            break;
    }

    if(x == len)
    {
        printf("Unknown type `%s'\n", u8Type);
        return FALSE;
    }

    printf("Set type %s to %lu\n", u8Type, x);
    eDSCMBType = x;

    memset(&pArgsAlgo, 0, sizeof(DSCMB_ENGALGO));
    memset(&pArgsSwitch, 0, sizeof(DSCMB_ENGSWITCH));

    Demo_DSCMB_Open();

    pArgsAlgo.u32EngId    = u32DscmbEngId;
    pArgsAlgo.u32DscmbId  = u32DscmbId;
    pArgsAlgo.eEngType    = E_DSCMB_ENG_ESA;
    pArgsAlgo.stConfig    = DSCMB_Type_Map[eDSCMBType].CfgType;
    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetAlgo, (void*)&pArgsAlgo))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_EngSetAlgo failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    pArgsSwitch.u32EngId    = u32DscmbEngId;
    pArgsSwitch.u32DscmbId  = u32DscmbId;
    pArgsSwitch.eUppSwitch  = 0;
    pArgsSwitch.eLowSwitch  = E_DSCMB_ENG_ESA;
    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetSwitch, (void*)&pArgsSwitch))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_EngSetSwitch failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Close();

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the control word
/// @param[in] u32DscmbId the dscmbler id
/// @param[in] u8HexOddCW a hex string represent a odd control word
/// @param[in] u8HexEvenCW a hex string represent a even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetCW(MS_U32 u32DscmbId, MS_U8 *u8StrOddCW, MS_U8 *u8StrEvenCW)
{
    MS_U8 u8HexOddCW[16] = {0};
    MS_U8 u8HexEvenCW[16]= {0};
    MS_U32 i = 0 ;
    DSCMB_ENGKEY pArgs;

    if ((u8StrOddCW == NULL)||(u8StrEvenCW == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    //If u32DscmbId == NULL, Descrambler filter NOT allocated. Can NOT set CW.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    memset(u8HexOddCW, 0, sizeof(u8HexOddCW));
    memset(u8HexEvenCW, 0, sizeof(u8HexEvenCW));
    memset(&pArgs, 0, sizeof(DSCMB_ENGKEY));

    _str2hex(u8HexOddCW, u8StrOddCW, sizeof(u8HexOddCW));
    _str2hex(u8HexEvenCW, u8StrEvenCW, sizeof(u8HexEvenCW));

    printf(" Odd Key: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexOddCW[i]);

    printf("\nEven Key: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexEvenCW[i]);
    printf("\n");

    Demo_DSCMB_Open();

    pArgs.u32EngId    = u32DscmbEngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = E_DSCMB_ENG_ESA;
    pArgs.eKeyType    = E_DSCMB_KEY_ODD;
    pArgs.pu8Key      = u8HexOddCW;
    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetKey, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_EngSetKey failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    pArgs.u32EngId    = u32DscmbEngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = E_DSCMB_ENG_ESA;
    pArgs.eKeyType    = E_DSCMB_KEY_EVEN;
    pArgs.pu8Key      = u8HexEvenCW;
    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetKey, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_EngSetKey failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Close();

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the initialization vector
/// @param[in] u32DeviceId the device id
/// @param[in] u8OddIV a hex represent a odd initialization vector
/// @param[in] u8EvenIV a hex represent a even initialization vector
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetIV(MS_U32 u32DscmbId, MS_U8 *u8StrOddIV, MS_U8 *u8StrEvenIV)
{
    MS_U8 u8HexOddIV[16]= {0};
    MS_U8 u8HexEvenIV[16]= {0};
    MS_U32 i = 0;
    DSCMB_ENGIV pArgs;

    if ((u8StrOddIV == NULL)||(u8StrEvenIV == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    //If u32DscmbId == NULL, Descrambler filter NOT allocated. Can NOT set IV.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    memset(u8HexOddIV, 0, sizeof(u8HexOddIV));
    memset(u8HexEvenIV, 0, sizeof(u8HexEvenIV));
    memset(&pArgs, 0, sizeof(DSCMB_ENGIV));

    _str2hex(u8HexOddIV, u8StrOddIV, sizeof(u8HexOddIV));
    _str2hex(u8HexEvenIV, u8StrEvenIV, sizeof(u8HexEvenIV));

    printf("Odd IV: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexOddIV[i]);

    printf("\nEven IV: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexEvenIV[i]);
    printf("\n");

    Demo_DSCMB_Open();

    pArgs.u32EngId    = u32DscmbEngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eKeyType    = E_DSCMB_KEY_ODD;
    pArgs.pu8IV       = u8HexOddIV;
    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetIV, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_EngSetIV failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    pArgs.u32EngId    = u32DscmbEngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eKeyType    = E_DSCMB_KEY_EVEN;
    pArgs.pu8IV       = u8HexEvenIV;
    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetIV, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_EngSetIV failed\n", __FUNCTION__,__LINE__);
        return FALSE;
    }

    Demo_DSCMB_Close();

    return TRUE;
}
#else

//------------------------------------------------------------------------------
/// @brief Allocate a descrambler filter and return descrambler id
/// @param[in]:
/// @return Descrambler id: Process success.
/// @return DRV_DSCMB_FLT_NULL: Process fial.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_U32 Demo_DSCMB_Create_Resource(void)
{
    MS_U32 u32DscmbId;
    u32DscmbId = MDrv_DSCMB2_FltAlloc(u32DscmbEngId);

    return u32DscmbId;
}

//------------------------------------------------------------------------------
/// @brief Free a descrambler filter
/// @param[in] u32DscmbId the descrambler id
/// @return TRUE: Process success.
/// @return FALSE: Process fial.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_Destroy_Resource(MS_U32 u32DscmbId)
{
    //If u32DscmbId == NULL, NO need destory
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return TRUE;
    }

    if(MDrv_DSCMB2_FltFree(u32DscmbEngId, u32DscmbId) == FALSE)
    {
        printf("MDrv_DSCMB2_FltFree() failed\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Connect a pid filter
/// @param[in] u32DscmbId the descrambler id
/// @param[in] u32PidFlt the Pid filter index that get from TSP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_ConnectFltId(MS_U32 u32DscmbId, MS_U32 u32PidFlt)
{
    //If u32DscmbId == NULL, Descrambler filter NOT allocated. CAN NOT connect.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(MDrv_DSCMB2_FltConnectFltId(u32DscmbEngId, u32DscmbId, u32PidFlt) == FALSE)
    {
        printf("MDrv_DSCMB2_FltConnectFltId() failed\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Disconnect a pid filter
/// @param[in] u32DscmbId the descrambler id
/// @param[in] u32PidFlt the Pid filter index that get from TSP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_DisconnectFltId(MS_U32 u32DscmbId, MS_U32 u32PidFlt)
{
    //if u32DscmbId == NULL, Descrambler filter NOT allocated. NO need disconnect.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return TRUE;
    }

    if(MDrv_DSCMB2_FltDisconnectFltId(u32DscmbEngId, u32DscmbId, u32PidFlt) == FALSE)
    {
        printf("MDrv_DSCMB2_FltDisconnectFltId() failed\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set the descramble algorithm
/// @param[in] u32DscmbId the descrambler id
/// @param[in] u8Type a string represent the algorithm.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetFltType(MS_U32 u32DscmbId, MS_U8 *u8Type)
{
    MS_U32 x = 0;
    MS_U32 len = 0;
    EN_DEMO_DSCMB_TYPE eDSCMBType;

    len = sizeof(DSCMB_Type_Map) / sizeof(DSCMB_Type_Map[0]);

    //if u32DscmbId == NULL, Descrambler filter NOT allocated. Can NOT set fliter type.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    for(x = 0; x < len; x++)
    {
        if(strcmp((char *)u8Type, DSCMB_Type_Map[x].name) == 0)
            break;
    }

    if(x == len)
    {
        printf("Unknown type `%s'\n", u8Type);
        return FALSE;
    }

    printf("Set type %s to %lu\n", u8Type, (long unsigned int)x);
    eDSCMBType = x;

    if(MDrv_DSCMB2_EngSetAlgo(u32DscmbEngId, u32DscmbId, E_DSCMB_ENG_ESA, DSCMB_Type_Map[eDSCMBType].CfgType) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetAlgo() failed\n");
        return FALSE;
    }

    if (MDrv_DSCMB2_EngSetSwitch(u32DscmbEngId, u32DscmbId, 0, E_DSCMB_ENG_ESA) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetSwitch() failed\n");
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the control word
/// @param[in] u32DscmbId the dscmbler id
/// @param[in] u8HexOddCW a hex string represent a odd control word
/// @param[in] u8HexEvenCW a hex string represent a even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetCW(MS_U32 u32DscmbId, MS_U8 *u8StrOddCW, MS_U8 *u8StrEvenCW)
{
    MS_U8 u8HexOddCW[16] = {0};
    MS_U8 u8HexEvenCW[16]= {0};
    MS_U32 i = 0 ;

    if ((u8StrOddCW == NULL)||(u8StrEvenCW == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    //If u32DscmbId == NULL, Descrambler filter NOT allocated. Can NOT set CW.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    memset(u8HexOddCW, 0, sizeof(u8HexOddCW));
    memset(u8HexEvenCW, 0, sizeof(u8HexEvenCW));

    _str2hex(u8HexOddCW, u8StrOddCW, sizeof(u8HexOddCW));
    _str2hex(u8HexEvenCW, u8StrEvenCW, sizeof(u8HexEvenCW));

    printf(" Odd Key: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexOddCW[i]);

    printf("\nEven Key: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexEvenCW[i]);
    printf("\n");

    if(MDrv_DSCMB2_EngSetKey(u32DscmbEngId, u32DscmbId, E_DSCMB_ENG_ESA, E_DSCMB_KEY_ODD, u8HexOddCW) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetKey() failed\n");
        return FALSE;
    }

    if(MDrv_DSCMB2_EngSetKey(u32DscmbEngId, u32DscmbId, E_DSCMB_ENG_ESA, E_DSCMB_KEY_EVEN, u8HexEvenCW) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetKey() failed\n");
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the initialization vector
/// @param[in] u32DeviceId the device id
/// @param[in] u8OddIV a hex represent a odd initialization vector
/// @param[in] u8EvenIV a hex represent a even initialization vector
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetIV(MS_U32 u32DscmbId, MS_U8 *u8StrOddIV, MS_U8 *u8StrEvenIV)
{
    MS_U8 u8HexOddIV[16]= {0};
    MS_U8 u8HexEvenIV[16]= {0};
    MS_U32 i = 0;

    if ((u8StrOddIV == NULL)||(u8StrEvenIV == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    //If u32DscmbId == NULL, Descrambler filter NOT allocated. Can NOT set IV.
    if(u32DscmbId == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    memset(u8HexOddIV, 0, sizeof(u8HexOddIV));
    memset(u8HexEvenIV, 0, sizeof(u8HexEvenIV));

    _str2hex(u8HexOddIV, u8StrOddIV, sizeof(u8HexOddIV));
    _str2hex(u8HexEvenIV, u8StrEvenIV, sizeof(u8HexEvenIV));

    printf("Odd IV: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexOddIV[i]);

    printf("\nEven IV: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8HexEvenIV[i]);
    printf("\n");

    if(MDrv_DSCMB2_EngSetIV(u32DscmbEngId, u32DscmbId, E_DSCMB_KEY_ODD, u8HexOddIV) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetIV() failed\n");
        return FALSE;
    }

    if(MDrv_DSCMB2_EngSetIV(u32DscmbEngId, u32DscmbId, E_DSCMB_KEY_EVEN, u8HexEvenIV) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetIV() failed\n");
        return FALSE;
    }

    return TRUE;
}

#endif

//------------------------------------------------------------------------------
/// @brief Inform Live AV DSCMB Info by device id
/// @param[in] u32DeviceId the device id
/// @param[out]  stInfo the stDEMODSCMBINFO struct record Live AV DSCMB Info
/// @return TRUE:  Process success.
/// @return FALSE:  Process fail.
/// @sa
/// @note
/// Command: \b Command: \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_InfoGet(MS_U32 u32DeviceId, stDEMODSCMBINFO *stInfo)
{
    if(stInfo == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    stInfo->bConnectStatus= Demo_DSCMB_Config[u32DeviceId].bConnectStatus;
    stInfo->u8Type = Demo_DSCMB_Config[u32DeviceId].u8Type;
    stInfo->u8StrEvenCW= Demo_DSCMB_Config[u32DeviceId].u8StrEvenCW;
    stInfo->u8StrEvenIV= Demo_DSCMB_Config[u32DeviceId].u8StrEvenIV;
    stInfo->u8StrOddCW= Demo_DSCMB_Config[u32DeviceId].u8StrOddCW;
    stInfo->u8StrOddIV= Demo_DSCMB_Config[u32DeviceId].u8StrOddIV;
    stInfo->u32CAVid= u32CAVid;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set global CAVID
/// @param[in] CAVid the CA Vendor ID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_SetCAVID \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetCAVID(MS_U32 *CAVid)
{
    if (CAVid == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(MDrv_DSCMB2_SetDefaultCAVid(u32DscmbEngId, *CAVid) == FALSE)
    {
        printf("MDrv_DSCMB2_SetDefaultCAVid() failed\n");
        return FALSE;
    }
    u32CAVid = *CAVid;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Allocate a descrambler filter and connect the pid to filter
/// @param[in] u32DeviceId the device id
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_ConnectPID \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_ConnectPID(MS_U32 *u32DeviceId)
{
    MS_U32 i = 0;
    ST_AV_INFO pstAVInfo = {0};
    if (u32DeviceId == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_AV_GetAVInfo((EN_AV_Device*)u32DeviceId,E_AV_GetCmd_LiveInfo,&pstAVInfo))
    {
        u32PidFlt[0] = (MS_U32)pstAVInfo.stVideoParams.u8Filter;
        u32PidFlt[1] = (MS_U32)pstAVInfo.stAudioParams.u8Filter;
        u32PidFlt[2] = (MS_U32)pstAVInfo.stPCRParams.ePCREngID;
        printf("VideoPidFlt=%lu AudioFlt=%lu\n", (long unsigned int)u32PidFlt[0], (long unsigned int)u32PidFlt[1]);
    }
    else
    {
        printf("Get Live PID Flts failed\n");
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] != DRV_DSCMB_FLT_NULL)
    {
        for(i = 0; i < 2; i++)
        {
            Demo_DSCMB_DisconnectFltId(u32DscmbId[*u32DeviceId], u32PidFlt[i]);
        }
        Demo_DSCMB_Destroy_Resource(u32DscmbId[*u32DeviceId]);
        u32DscmbId[*u32DeviceId] = DRV_DSCMB_FLT_NULL;

        //Record LIVE DSCMB Config: Clear buff
        memset(&Demo_DSCMB_Config[*u32DeviceId], 0x0, sizeof(Demo_DSCMB_Config[0]));
    }

    if((u32DscmbId[*u32DeviceId] = Demo_DSCMB_Create_Resource()) == DRV_DSCMB_FLT_NULL)
    {
        printf("Demo_DSCMB_Create_Resource() failed\n");
        return FALSE;
    }

    printf("\033[32mAllocate DSCMB ID = %lu\n\033[m", (long unsigned int)u32DscmbId[*u32DeviceId]);

    for(i = 0; i < 2; i++)
    {
        printf("\033[32m Connect filter ID = %lu\n\033[m", (long unsigned int)u32PidFlt[i]);
        if(Demo_DSCMB_ConnectFltId(u32DscmbId[*u32DeviceId], u32PidFlt[i]) == FALSE)
        {
            printf("Demo_DSCMB_ConnectFltId() failed\n");
            return FALSE;
        }
    }
    //Record LIVE DSCMB config: .bConnectStatus
    Demo_DSCMB_Config[*u32DeviceId].bConnectStatus = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Free a descrambler filter and disconnect the pid to filter
/// @param[in] u32DeviceId the device id
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_DisconnectPID \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_DisconnectPID(MS_U32 *u32DeviceId)
{
    MS_U32 i = 0;
    ST_AV_INFO pstAVInfo = {0};

    if (u32DeviceId == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_AV_GetAVInfo((EN_AV_Device*)u32DeviceId,E_AV_GetCmd_LiveInfo,&pstAVInfo))
    {
        u32PidFlt[0] = (MS_U32)pstAVInfo.stVideoParams.u8Filter;
        u32PidFlt[1] = (MS_U32)pstAVInfo.stAudioParams.u8Filter;
        u32PidFlt[2] = (MS_U32)pstAVInfo.stPCRParams.ePCREngID;
        printf("VideoPidFlt=%lu AudioFlt=%lu\n", (long unsigned int)u32PidFlt[0], (long unsigned int)u32PidFlt[1]);
    }
    else
    {
        printf("Get Live PID Flts failed\n");
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] != DRV_DSCMB_FLT_NULL)
    {
        printf("\033[32mFree DSCMB ID = %lu\n\033[m", (long unsigned int)u32DscmbId[*u32DeviceId]);

        for(i = 0; i < 2; i++)
        {
            printf("\033[32m Disconnect filter ID = %lu\n\033[m", (long unsigned int)u32PidFlt[i]);
            if(Demo_DSCMB_DisconnectFltId(u32DscmbId[*u32DeviceId], u32PidFlt[i]) == FALSE)
            {
                printf("Demo_DSCMB_DisconnectFltId() failed\n");
                return FALSE;
            }
        }
        //Record LIVE DSCMB Config: Clean buff
        memset(&Demo_DSCMB_Config[*u32DeviceId], 0x0, sizeof(Demo_DSCMB_Config[0]));

        if(Demo_DSCMB_Destroy_Resource(u32DscmbId[*u32DeviceId]) == FALSE)
        {
            printf("Demo_DSCMB_Destroy_Resource() failed\n");
            return FALSE;
        }

        u32DscmbId[*u32DeviceId] = DRV_DSCMB_FLT_NULL;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set the descramble algorithm
/// @param[in] u32DeviceId the device id
/// @param[in] u8Type a string represent the algorithm.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_SetType \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_SetType(MS_U32 *u32DeviceId, MS_U8 *u8Type)
{

    if ((u32DeviceId == NULL)||(u8Type == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_DSCMB_SetFltType(u32DscmbId[*u32DeviceId], u8Type) == FALSE)
    {
        printf("Demo_DSCMB_SetFltType() failed\n");
        return FALSE;
    }
    //Record LIVE DSCMB Config: .u8Type
    memcpy(Demo_DSCMB_Config[*u32DeviceId].u8Type, u8Type, strlen((const char *)u8Type));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the clear control word
/// @param[in] u32DeviceId the device id
/// @param[in] u8OddCW a hex string represent a odd control word
/// @param[in] u8EvenCW a hex string represent a even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_CW \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_CW(MS_U32 *u32DeviceId, MS_U8 *u8OddCW, MS_U8 *u8EvenCW)
{
    if ((u32DeviceId == NULL)||(u8OddCW == NULL)||(u8EvenCW == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_DSCMB_SetCW(u32DscmbId[*u32DeviceId], u8OddCW, u8EvenCW) == FALSE)
    {
        printf("Demo_DSCMB_SetCW() failed\n");
        return FALSE;
    }
    //Record LIVE DSCMB Config: .u8StrOddCW .u8StrEnevCW
    memcpy(Demo_DSCMB_Config[*u32DeviceId].u8StrOddCW, u8OddCW, sizeof(Demo_DSCMB_Config[*u32DeviceId].u8StrOddCW));
    memcpy(Demo_DSCMB_Config[*u32DeviceId].u8StrEvenCW, u8EvenCW, sizeof(Demo_DSCMB_Config[*u32DeviceId].u8StrEvenCW));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the initialization vector
/// @param[in] u32DeviceId the device id
/// @param[in] u8OddIV a hex string represent a odd initialization vector
/// @param[in] u8EvenIV a hex string represent a even initialization vector
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_IV \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_IV(MS_U32 *u32DeviceId, MS_U8 *u8OddIV, MS_U8 *u8EvenIV)
{
    if ((u32DeviceId == NULL)||(u8OddIV == NULL)||(u8EvenIV == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_DSCMB_SetIV(u32DscmbId[*u32DeviceId], u8OddIV, u8EvenIV) == FALSE)
    {
        printf("Demo_DSCMB_SetIV() failed\n");
        return FALSE;
    }
    //Record LIVE DSCMB Config: .u8StrOddIV .u8StrEnevIV
    memcpy(Demo_DSCMB_Config[*u32DeviceId].u8StrOddIV, u8OddIV, sizeof(Demo_DSCMB_Config[*u32DeviceId].u8StrOddIV));
    memcpy(Demo_DSCMB_Config[*u32DeviceId].u8StrEvenIV, u8EvenIV, sizeof(Demo_DSCMB_Config[*u32DeviceId].u8StrEvenIV));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup the keyladder decryption algorithm
/// @param[in] u8Mode the keyladder decryption algorithm.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_KeyLadderMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_KeyLadderMode(MS_U8 *u8Mode)
{
    MS_U32 x=0, len = sizeof(DSCMB_KLEng_Map) / sizeof(DSCMB_KLEng_Map[0]);

    if (u8Mode == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    for(x = 0; x < len; x++)
    {
        if(strcmp((char *)u8Mode, DSCMB_KLEng_Map[x].name) == 0)
            break;
    }

    if(x == len)
    {
        printf("Unknown key ladder mode `%s'\n", u8Mode);
        return FALSE;
    }

    printf("Set type to %d\n", DSCMB_KLEng_Map[x].mode);
    u32EngMode = DSCMB_KLEng_Map[x].mode;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Setup a one-level key ladder
/// @param[in] u32DeviceId the device id
/// @param[in] u8Source select ROOT key source. Available sources are ACPU and SECRET_1 ~ 8
/// @param[in] u8HexHostKey The ROOT key if Source is ACPU
/// @param[in] u8HexOddCW encrypted odd control word
/// @param[in] u8HexOddCW encrypted even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_KeyLadder \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_KeyLadder(MS_U32 *u32DeviceId, MS_U8 *u8Source, MS_U8 *u8HexHostKey, MS_U8 *u8HexOddCW, MS_U8 *u8HexEvenCW)
{
    MS_U32 i=0;
    MS_U32 x=0, len = sizeof(DSCMB_KLSrc_Map) / sizeof(DSCMB_KLSrc_Map[0]);

    MS_U8 u8HostKey[16]= {0}, *pu8HostKey = NULL;
    MS_U32 u32HostKeyLen = 0;

    MS_U8 u8OddCW[16]= {0};
    MS_U32 u32OddCWLen=0;
    MS_U8 u8EvenCW[16]= {0};
    MS_U32 u32EvenCWLen=0;
    DEMOKLCONFIG stKLInfo;

    stKLInfo.pu8KeyACPU = NULL;

    if ((u32DeviceId == NULL)||(u8Source == NULL)||(u8HexHostKey == NULL)||(u8HexOddCW == NULL)||(u8HexEvenCW == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    stKLInfo.u32DscmbId = u32DscmbId[*u32DeviceId];
    stKLInfo.u32EngID = u32DscmbEngId;
    stKLInfo.u32Level = 1;
    stKLInfo.u32CAVid = u32CAVid;

    for(x = 0; x < len; x++)
    {
        if(strcmp((char *)u8Source, DSCMB_KLSrc_Map[x].name) == 0)
            break;
    }

    if(x == len)
    {
        printf("Unknown type `%s'\n", u8Source);
        return FALSE;
    }
    stKLInfo.eSrc = DSCMB_KLSrc_Map[x].src;
    stKLInfo.eKLMode = u32EngMode;
    stKLInfo.eDst = E_DEMO_KL_DST_KT_ESA;

    printf("Set type to %d\n", DSCMB_KLSrc_Map[x].src);

    if(DSCMB_KLSrc_Map[x].src == E_DSCMB_KL_SRC_ACPU)
    {
        pu8HostKey = u8HostKey;
        memset(u8HostKey, 0, sizeof(u8HostKey));
        u32HostKeyLen = _str2hex(u8HostKey, u8HexHostKey, sizeof(u8HostKey));
        printf("Host Key: ");
        for(i = 0; i < u32HostKeyLen; i++)
            printf("%02x ", u8HostKey[i]);
        printf(" \n");
        stKLInfo.pu8KeyACPU = pu8HostKey;
    }

    memset(u8OddCW, 0, sizeof(u8OddCW));
    u32OddCWLen = _str2hex(u8OddCW, u8HexOddCW, sizeof(u8OddCW));
    printf("  Odd CW: ");
    for(i = 0; i < u32OddCWLen; i++)
        printf("%02x ", u8OddCW[i]);
    printf(" \n");
    stKLInfo.pu8KeyKLIn = u8OddCW;
    stKLInfo.eKeyType = E_DEMO_KL_KEY_ODD;
    DEMO_KeyLadder_Setup(stKLInfo);

    memset(u8EvenCW, 0, sizeof(u8EvenCW));
    u32EvenCWLen = _str2hex(u8EvenCW, u8HexEvenCW, sizeof(u8EvenCW));
    printf(" Even CW: ");
    for(i = 0; i < u32EvenCWLen; i++)
        printf("%02x ", u8EvenCW[i]);
    printf(" \n");
    stKLInfo.pu8KeyKLIn = u8EvenCW;
    stKLInfo.eKeyType = E_DEMO_KL_KEY_EVEN;
    DEMO_KeyLadder_Setup(stKLInfo);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The help function for demo Dscrambler app
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b HelpDSCMB \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DSCMB_Help(void)
{
    const char *help =
        "DSCMB_SetCAVID CAVID\n"
        "DSCMB_ConnectPID DeviceId\n"
        "  DeviceId: The device id\n\n"
        "DSCMB_DisconnectPID DeviceId\n"
        "  DeviceId: The device id\n\n"
        "DSCMB_SetType DeviceId Type\n"
        "  DeviceId: The device id\n"
        "  Type: The descramble algorithm. Available types are CSA, CSA_CONF, CSA3, CI+DES,\n"
        "        CI+AES, AES_SCTE52, TDES_SCTE52, AES_ECB, TDES_ECB, and SYN_AES\n\n"
        "DSCMB_CW DeviceId OddKey EvenKey\n"
        "  DeviceId: The device id\n"
        "  OddKey/EvenKey: Setup clear control word\n\n"
        "DSCMB_IV DeviceId OddIV EvenIV\n"
        "  DeviceId: The device id\n"
        "  OddIV/EvenIV: Setup initialization vector\n\n"
        "DSCMB_KeyLadderMode Mode\n"
        "  Mode: The keyladder decryption algorithm. Available modes are TDES and AES\n\n"
        "DSCMB_KeyLadder DeviceId Source HostKey EncryptedOddCW EncryptedEvenCW\n"
        "  DeviceId: The device id\n"
        "  Source: Select ROOT key. Available sources are ACPU and SECRET_1 ~ 8\n"
        "  HostKey: The ROOT key if Source is ACPU\n"
        "  EncryptedOddCW/EncryptedEvenCW: Encrypted control word\n";

    puts(help);

    return TRUE;
}

#endif //DEMO_DSCMB_ADVANCED_TEST

