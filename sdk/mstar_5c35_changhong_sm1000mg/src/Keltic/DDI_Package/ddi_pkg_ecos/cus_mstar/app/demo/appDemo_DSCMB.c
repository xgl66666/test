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


////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_DSCMB.c
/// @author MStar Semiconductor Inc.
/// @brief  Descrambler Function Sample Demo
///
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"

#include "apiDMX.h"
#include "drvDSCMB.h"

#include <string.h>
#include <ctype.h>

extern MS_BOOL DemoAV_TSP_GetLiveTVPIDFlts(MS_U8* u8LiveVideoFltId, MS_U8* u8LiveAudioFltId, MS_U8* u8LivePCRFltId);

//--------------------------------------------------------------------------------------------------
// Global variable
//--------------------------------------------------------------------------------------------------

static MS_U32 u32DscmbId[6] = {DRV_DSCMB_FLT_NULL};
static MS_U32 u32DscmbEngId = 0;
static MS_U32 u32CAVid = 0xF;
static MS_U32 u32PidFlt[3] = {0xffff, 0xffff, 0xffff};
static DSCMB_KLEng u32EngMode = E_DSCMB_KL_TDES;

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

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
int _hex2str(unsigned char *s, const unsigned char *h, int size)
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

static void _dump_buf(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
}

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Set global CAVID
/// @param[in] CAVid the CA Vendor ID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_SetCAVID \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DSCMB_SetCAVID(MS_U32 *CAVid)
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
MS_BOOL appDemo_DSCMB_ConnectPID(MS_U32 *u32DeviceId)
{
    MS_U32 i = 0;
    MS_U8 u32VideoFltid  = 0, u32AudioFltid = 0, u32PCRFltid = 0;
    if (u32DeviceId == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if( DemoAV_TSP_GetLiveTVPIDFlts( &u32PCRFltid, &u32VideoFltid, &u32AudioFltid))
    {
        u32PidFlt[0] = (MS_U32)u32VideoFltid;
        u32PidFlt[1] = (MS_U32)u32AudioFltid;
        u32PidFlt[2] = (MS_U32)u32PCRFltid;
        printf("VideoPidFlt=%ld AudioFlt=%ld PCRFlt=%ld\n", u32PidFlt[0], u32PidFlt[1], u32PidFlt[2]);
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
            MDrv_DSCMB2_FltDisconnectFltId(u32DscmbEngId, u32DscmbId[*u32DeviceId], u32PidFlt[i]);
        }
        MDrv_DSCMB2_FltFree(u32DscmbEngId, u32DscmbId[*u32DeviceId]);
        u32DscmbId[*u32DeviceId] = DRV_DSCMB_FLT_NULL;
    }

    if((u32DscmbId[*u32DeviceId] = MDrv_DSCMB2_FltAlloc(u32DscmbEngId)) == DRV_DSCMB_FLT_NULL)
    {
        printf("MDrv_DSCMB2_FltAlloc() failed\n");
        return FALSE;
    }

    printf("\033[32mAllocate DSCMB ID = %lu\n\033[m",u32DscmbId[*u32DeviceId]);

    for(i = 0; i < 2; i++)
    {
        printf("\033[32m Connect filter ID = %lu\n\033[m", u32PidFlt[i]);
        if(MDrv_DSCMB2_FltConnectFltId(u32DscmbEngId, u32DscmbId[*u32DeviceId], u32PidFlt[i]) == FALSE)
        {
            printf("MDrv_DSCMB2_FltConnectFltId() failed\n");
            return FALSE;
        }
    }

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
MS_BOOL appDemo_DSCMB_DisconnectPID(MS_U32 *u32DeviceId)
{
    MS_U32 i = 0;
    MS_U8 u32VideoFltid  = 0, u32AudioFltid = 0, u32PCRFltid = 0;
    if (u32DeviceId == NULL)
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(DemoAV_TSP_GetLiveTVPIDFlts(&u32VideoFltid,&u32AudioFltid,&u32PCRFltid))
    {
        u32PidFlt[0] = (MS_U32)u32VideoFltid;
        u32PidFlt[1] = (MS_U32)u32AudioFltid;
        u32PidFlt[2] = (MS_U32)u32PCRFltid;
        printf("VideoPidFlt=%ld AudioFlt=%ld PCRFlt=%ld\n", u32PidFlt[0], u32PidFlt[1], u32PidFlt[2]);
    }
    else
    {
        printf("Get Live PID Flts failed\n");
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] != DRV_DSCMB_FLT_NULL)
    {
        printf("\033[32mFree DSCMB ID = %lu\n\033[m",u32DscmbId[*u32DeviceId]);

        for(i = 0; i < 2; i++)
        {
            printf("\033[32m Disconnect filter ID = %lu\n\033[m", u32PidFlt[i]);
            if(MDrv_DSCMB2_FltDisconnectFltId(u32DscmbEngId, u32DscmbId[*u32DeviceId], u32PidFlt[i]) == FALSE)
            {
                printf("MDrv_DSCMB2_FltDisconnectFltId() failed\n");
                return FALSE;
            }
        }
        if(MDrv_DSCMB2_FltFree(u32DscmbEngId, u32DscmbId[*u32DeviceId]) == FALSE)
        {
            printf("MDrv_DSCMB2_FltFree() failed\n");
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
MS_BOOL appDemo_DSCMB_SetType(MS_U32 *u32DeviceId, MS_U8 *u8Type)
{
    MS_U32 x = 0, len = sizeof(DSCMB_Type_Map) / sizeof(DSCMB_Type_Map[0]);

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

    if(MDrv_DSCMB2_EngSetAlgo(u32DscmbEngId, u32DscmbId[*u32DeviceId], E_DSCMB_ENG_ESA, DSCMB_Type_Map[x].CfgType) == FALSE)
    {
        printf("MDrv_DSCMB2_SetEngAlgo() failed\n");
        return FALSE;
    }

    if (MDrv_DSCMB2_EngSetSwitch(u32DscmbEngId, u32DscmbId[*u32DeviceId], 0, E_DSCMB_ENG_ESA) == FALSE)
    {
        printf("MDrv_DSCMB2_SetSwitch() failed\n");
        return FALSE;
    }


    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Setup the clear control word
/// @param[in] u32DeviceId the device id
/// @param[in] u8HexOddCW a hex string represent a odd control word
/// @param[in] u8HexEvenCW a hex string represent a even control word
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_CW \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DSCMB_CW(MS_U32 *u32DeviceId, MS_U8 *u8HexOddCW, MS_U8 *u8HexEvenCW)
{
    MS_U8 u8OddCW[16] = {0};
    MS_U8 u8EvenCW[16]= {0};
    MS_U32 i = 0 ;

    if ((u32DeviceId == NULL)||(u8HexOddCW == NULL)||(u8HexEvenCW == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    memset(u8OddCW, 0, sizeof(u8OddCW));
    memset(u8EvenCW, 0, sizeof(u8EvenCW));

    _hex2str(u8OddCW, u8HexOddCW, sizeof(u8OddCW));
    _hex2str(u8EvenCW, u8HexEvenCW, sizeof(u8EvenCW));

    printf(" Odd Key: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8OddCW[i]);

    printf("\nEven Key: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8EvenCW[i]);
    printf("\n");

    if(MDrv_DSCMB2_EngSetKey(u32DscmbEngId, u32DscmbId[*u32DeviceId], E_DSCMB_ENG_ESA, E_DSCMB_KEY_ODD, u8OddCW) == FALSE)
    {
        printf("MDrv_DSCMB2_SetEngKey() failed\n");
        return FALSE;
    }

    if(MDrv_DSCMB2_EngSetKey(u32DscmbEngId, u32DscmbId[*u32DeviceId], E_DSCMB_ENG_ESA, E_DSCMB_KEY_EVEN, u8EvenCW) == FALSE)
    {
        printf("MDrv_DSCMB2_SetEngKey() failed\n");
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Setup the initialization vector
/// @param[in] u32DeviceId the device id
/// @param[in] u8HexOddIV a hex string represent a odd initialization vector
/// @param[in] u8HexEvenIV a hex string represent a even initialization vector
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Command:DSCMB_IV \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DSCMB_IV(MS_U32 *u32DeviceId, MS_U8 *u8HexOddIV, MS_U8 *u8HexEvenIV)
{
    MS_U32 i = 0;
    MS_U8 u8OddIV[16]= {0}, u8EvenIV[16]= {0};

    if ((u32DeviceId == NULL)||(u8HexOddIV == NULL)||(u8HexEvenIV == NULL))
    {
        printf("[%s][%d]Invalid Parameters \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(u32DscmbId[*u32DeviceId] == DRV_DSCMB_FLT_NULL)
    {
        printf("[%s][%d]DSCMB_FLT is NULL \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    memset(u8OddIV, 0, sizeof(u8OddIV));
    memset(u8EvenIV, 0, sizeof(u8EvenIV));

    _hex2str(u8OddIV, u8HexOddIV, sizeof(u8OddIV));
    _hex2str(u8EvenIV, u8HexEvenIV, sizeof(u8EvenIV));

    printf("Odd IV: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8OddIV[i]);

    printf("\nEven IV: ");
    for(i = 0; i < 16; i++)
        printf("%02x ", u8EvenIV[i]);
    printf("\n");

    if(MDrv_DSCMB2_EngSetIV(u32DscmbEngId, u32DscmbId[*u32DeviceId], E_DSCMB_KEY_ODD, u8OddIV) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetIV() failed\n");
        return FALSE;
    }

    if(MDrv_DSCMB2_EngSetIV(u32DscmbEngId, u32DscmbId[*u32DeviceId], E_DSCMB_KEY_EVEN, u8EvenIV) == FALSE)
    {
        printf("MDrv_DSCMB2_EngSetIV() failed\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_DSCMB_KL_One(MS_U32 u32Cavid, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, DSCMB_KLSrc KLSrc,
                                  MS_U8 *u8HostKey, MS_U32 u32HostKeyLen, MS_U8* u8EncryptedCW, MS_U32 u32CWLen)
{
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};
    MS_BOOL ret = FALSE;
    // Level one setting for DSCMB destination
    DSCMB_KLCfg_All KLConfigAll =
    {
        .eAlgo = u32EngMode,
        .eSrc = KLSrc,
        .eDst = E_DSCMB_KL_DST_KT_ESA,  //Set to Key Table ESA for DSCMB
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = eKeyType,
        .u32Level = 1,                  //Set to Level one
        .u32EngID = u32DscmbEngId,      //Set to zero in STB chip
        .u32DscID = u32DscmbId,
        .u8KeyACPU = u8HostKey,
        .pu8KeyKLIn = u8EncryptedCW,    //Level one input is encrypted CW
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .eKLSel   = E_DSCMB_KL_SEL_DEFAULT,
        .u32CAVid = u32Cavid,
    };

    // Key Ladder
    ret = MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!! Status = 0x%X\n", (unsigned int)KL_Status);
        return FALSE;
    }
    if(KLConfigAll.eDst == E_DSCMB_KL_DST_CPU_ACPU)
    {
        _dump_buf(ACPU_Out, 16);
    }
    printf(" \n");

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
MS_BOOL appDemo_DSCMB_KeyLadderMode(MS_U8 *u8Mode)
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
MS_BOOL appDemo_DSCMB_KeyLadder(MS_U32 *u32DeviceId, MS_U8 *u8Source, MS_U8 *u8HexHostKey, MS_U8 *u8HexOddCW, MS_U8 *u8HexEvenCW)
{
    MS_U32 i=0;
    MS_U32 x=0, len = sizeof(DSCMB_KLSrc_Map) / sizeof(DSCMB_KLSrc_Map[0]);

    MS_U8 u8HostKey[16]= {0}, *pu8HostKey = NULL;
    MS_U32 u32HostKeyLen = 0;

    MS_U8 u8OddCW[16]= {0};
    MS_U32 u32OddCWLen=0;
    MS_U8 u8EvenCW[16]= {0};
    MS_U32 u32EvenCWLen=0;

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

    printf("Set type to %d\n", DSCMB_KLSrc_Map[x].src);

    if(DSCMB_KLSrc_Map[x].src == E_DSCMB_KL_SRC_ACPU)
    {
        pu8HostKey = u8HostKey;
        memset(u8HostKey, 0, sizeof(u8HostKey));
        u32HostKeyLen = _hex2str(u8HostKey, u8HexHostKey, sizeof(u8HostKey));
        printf("Host Key: ");
        for(i = 0; i < u32HostKeyLen; i++)
            printf("%02x ", u8HostKey[i]);
        printf(" \n");
    }

    memset(u8OddCW, 0, sizeof(u8OddCW));
    u32OddCWLen = _hex2str(u8OddCW, u8HexOddCW, sizeof(u8OddCW));
    printf("  Odd CW: ");
    for(i = 0; i < u32OddCWLen; i++)
        printf("%02x ", u8OddCW[i]);
    printf(" \n");

    memset(u8EvenCW, 0, sizeof(u8EvenCW));
    u32EvenCWLen = _hex2str(u8EvenCW, u8HexEvenCW, sizeof(u8EvenCW));
    printf(" Even CW: ");
    for(i = 0; i < u32EvenCWLen; i++)
        printf("%02x ", u8EvenCW[i]);
    printf(" \n");

    _Demo_DSCMB_KL_One(u32CAVid, u32DscmbId[*u32DeviceId], E_DSCMB_KEY_ODD, DSCMB_KLSrc_Map[x].src, pu8HostKey, u32HostKeyLen, u8OddCW, u32OddCWLen);
    _Demo_DSCMB_KL_One(u32CAVid, u32DscmbId[*u32DeviceId], E_DSCMB_KEY_EVEN, DSCMB_KLSrc_Map[x].src, pu8HostKey, u32HostKeyLen, u8EvenCW, u32EvenCWLen);

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
MS_BOOL appDemo_DSCMB_Help(void)
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

