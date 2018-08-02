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
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include "demo_utility.h"

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
//static MS_S32 s32SysCachePoolID = INVALID_POOL_ID;
//static MS_S32 s32SysNonCachePoolID = INVALID_POOL_ID;

#define EASY_CMD    1

static MS_S32 sPoolID[E_DDI_POOL_SYS_UNDEFINE];
static MS_BOOL bUtilityPool_Init = FALSE;

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Create and start the app demo task of RTC and initialze all parameters
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Util_Pool_Init(void)
{
    MS_U8 idx = 0;

    if (bUtilityPool_Init == FALSE)
    {
        for ( idx = 0 ;idx < E_DDI_POOL_SYS_UNDEFINE;idx++)
        {
            sPoolID[idx] = INVALID_POOL_ID;
        }
        bUtilityPool_Init = TRUE;
    }
    printf("[%s][%d]  %d \n",__FUNCTION__,__LINE__,E_DDI_POOL_SYS_UNDEFINE);
    /* Reserved */
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set System PoolID
/// @param[in] eSYSPOOL selection
/// @param[in] s32PoolID initail value
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Util_SetSystemPoolID(E_DDI_UTIL_SYSPOOLID eSYSPOOL,MS_S32 s32PoolID)
{
    if (bUtilityPool_Init == FALSE)
    {
        Demo_Util_Pool_Init();
    }

    if (eSYSPOOL >= E_DDI_POOL_SYS_UNDEFINE)
    {
        return FALSE;
    }
//    printf("[%s][%d] %d %d \n",__FUNCTION__,__LINE__,eSYSPOOL,s32PoolID);
    sPoolID[eSYSPOOL] = s32PoolID;
#if 0
    if (eSYSPOOL == E_DDI_POOL_SYS_CACHE)
    {
        s32SysCachePoolID = s32PoolID;
    }
    else if (eSYSPOOL == E_DDI_POOL_SYS_NONCACHE)
    {
        s32SysNonCachePoolID = s32PoolID;
    }
    else
    {
        return FALSE;
    }
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get System PoolID
/// @param[in] eSYSPOOL selection
/// @param[in] ps32PoolID get value
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Util_GetSystemPoolID(E_DDI_UTIL_SYSPOOLID eSYSPOOL,MS_S32 *ps32PoolID)
{
    if (bUtilityPool_Init == FALSE)
    {
       return FALSE;
    }

    if (eSYSPOOL >= E_DDI_POOL_SYS_UNDEFINE)
    {
        return FALSE;
    }

    *ps32PoolID =  sPoolID[eSYSPOOL];;

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief string to value
/// @param[in] string
/// @param[in] parameter number
/// @return Value - string to Value 
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U32 Demo_Util_DHConvert(char *string, int parano)
{
    MS_U16 charno,totalchar;
    MS_U32 value;

    /*[01]error condition =============================================================*/
    if(string==NULL)
    {
        return 0;
    }

    /*[02]get total legal character ===================================================*/
    charno=0;
    while((string[charno]!=10 && string[charno]!=13 && string[charno]!=0) || (charno==10))
    {
        charno++;
    }
    string[charno]=0;

    /*[03-1]hex format ================================================================*/
    if((string[0]=='0') && (string[1]=='X'||string[1]=='x'))
    {
        value = 0;
        for(charno=2; charno<strlen(string); charno++)
        {
            if(string[charno]==' '||string[charno]=='\t')
            {
                break;
            }
            value <<=4;
            if('0'<=string[charno]&&string[charno]<='9')
            {
                value += (string[charno]-'0');
            }
            else if('a'<=string[charno]&&string[charno]<='f')
            {
                value += (string[charno]- 'a' + 10);
            }
            else if('A'<=string[charno]&&string[charno]<='F')
            {
                value += (string[charno]- 'A' + 10);
            }
        }

        return(value);
    }

    /*[03-2]dec format ================================================================*/
    else
    {
        if(strlen(string)>10)
        {
            return (MS_U32)(-1);
        }

        value = 0;
        totalchar = strlen(string);
        for(charno=0; charno<totalchar; charno++)
        {
            if((totalchar-charno)==10)
            {
                if ((string[charno] - '0') > 4)
                    return 0xFFFFFFFF;
                else
                    value += (string[charno] - '0')*1000000000L;
            }
            if((totalchar-charno)==9)
            {
                value += (string[charno] - '0')*100000000L;
            }
            if((totalchar-charno)==8)
            {
                value += (string[charno] - '0')*10000000L;
            }
            if((totalchar-charno)==7)
            {
                value += (string[charno] - '0')*1000000L;
            }
            if((totalchar-charno)==6)
            {
                value += (string[charno] - '0')*100000L;
            }
            if((totalchar-charno)==5)
            {
                value += (string[charno] - '0')*10000L;
            }
            if((totalchar-charno)==4)
            {
                value += (string[charno] - '0')*1000L ;
            }
            if((totalchar-charno)==3)
            {
                value += (string[charno] - '0')*100L  ;
            }
            if((totalchar-charno)==2)
            {
                value += (string[charno] - '0')*10L   ;
            }
            if((totalchar-charno)==1)
            {
                value += (string[charno] - '0')*1L    ;
            }
        }

        return(value);
    }
}

//------------------------------------------------------------------------------
/// @brief toupper
/// @param[in] input
/// @return the string toupper 
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U8 Demo_Util_toupper(MS_U8 input)
{
    if( input>='a' && input<='z')
    {
        return (input = input +'A' - 'a');
    }
    else
    {
        return input;
    }
}

//------------------------------------------------------------------------------
/// @brief string compare
/// @param[in] string1
/// @param[in] string2
/// @return 0 - compare the same 
/// @return other number - compare different 
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
int Demo_Util_strcmp(const char *string1, const char *string2)
{
    int iRet, i;

    i = 0;
    while(string1[i] || string2[i])
    {
#if (EASY_CMD)
        iRet = Demo_Util_toupper(string1[i]) - Demo_Util_toupper(string2[i]);
#else
        iRet = string1[i] - string2[i];
#endif
        if(iRet)
        {
            return iRet;
        }
        i++;
    }
    return 0;
}


