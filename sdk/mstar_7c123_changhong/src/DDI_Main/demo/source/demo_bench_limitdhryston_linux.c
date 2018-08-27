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
#if (DEMO_BENCH_LIMITDHRYSTONE_LINUX_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsFS.h"
#include "demo_utility.h"
#include "demo_bench_limitdhryston_linux.h"

//-------------------------------------------------------------------------------------------------
// MACROS
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
//  Global Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief      Create or kill dhrystone task(s)
//
/// @param[in]           <on/off> 1 is create task(s), 0 is kill all tasks
//                    < task num> number of dhrystone, 1~4
//              <Percentage CPUx> CPU Limit percentage, 1~100%
//
// @return     TRUE: Process success.
// @return     FALSE: Process fail.
// @sa
// @note       User should copy cpulimit foler in USB disk.
//             The cpulimit folder will include files as below
//             - cpulimit64_static
//             - dhrystone
//             - input.txt
//             - run.sh
//
// Sample Command: \n
//          \b Demo_LimitDhrystone <on/off> <task num is n> <Percentage CPU0> .. <Percentage CPUn-1> \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Bench_LimitDhrystone( MS_U32* pu32Create , MS_U32* pu32TaskNum ,MS_U32* pu32Percentage0 ,MS_U32* pu32Percentage1, MS_U32* pu32Percentage2, MS_U32* pu32Percentage3)
{
    MS_BOOL bRet = TRUE;
    MS_U32  u32Loop;
    MS_U32  u32ArrayPercentage[4];

    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;

    u32ArrayPercentage[0] = * pu32Percentage0;
    u32ArrayPercentage[1] = * pu32Percentage1;
    u32ArrayPercentage[2] = * pu32Percentage2;
    u32ArrayPercentage[3] = * pu32Percentage3;

    // Check input setting
    if( *pu32Create != 0 && *pu32Create != 1 )
    {
        printf("Task create setting error!\n");
        bRet = FALSE;
    }
    else if( *pu32Create == 1 )
    {
        if( *pu32TaskNum < 1 || *pu32TaskNum > 4 )
        {
            printf("Task number setting error!\n");
            bRet = FALSE;
        }

        for( u32Loop = 0 ; u32Loop < *pu32TaskNum ; u32Loop++ )
        {
            if( u32ArrayPercentage[u32Loop] < 1 || u32ArrayPercentage[u32Loop] > 100 )
            {
                printf("Task%d Percentage setting error!\n",u32Loop);
                bRet = FALSE;
            }
        }
    }

    //allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(256, s32MstarNonCachedPoolID);
    if (pu8buf == NULL)
    {
        printf("Fail to allocate memory\n");
        bRet = FALSE;
    }

    if( bRet == TRUE )
    {
        if( *pu32Create == 1 )
        {
            switch( *pu32TaskNum )
            {
                case 1 :    sprintf((char*)pu8buf, "cd //mnt/sda1/cpulimit/ ; ./run.sh 1 1 %d",u32ArrayPercentage[0]);
                            break;

                case 2 :    sprintf((char*)pu8buf, "cd //mnt/sda1/cpulimit/ ; ./run.sh 1 2 %d %d",u32ArrayPercentage[0]
                                                                                          ,u32ArrayPercentage[1] );
                            break;

                case 3 :    sprintf((char*)pu8buf, "cd //mnt/sda1/cpulimit/ ; ./run.sh 1 3 %d %d %d",u32ArrayPercentage[0]
                                                                                             ,u32ArrayPercentage[1]
                                                                                             ,u32ArrayPercentage[2] );
                            break;

                default :   sprintf((char*)pu8buf, "cd //mnt/sda1/cpulimit/ ; ./run.sh 1 4 %d %d %d %d",u32ArrayPercentage[0]
                                                                                                ,u32ArrayPercentage[1]
                                                                                                ,u32ArrayPercentage[2]
                                                                                                ,u32ArrayPercentage[3]);
                            break;
            }
        }
        else
        {
            sprintf((char*)pu8buf, "cd //mnt/sda1/cpulimit/ ; ./run.sh 0");
        }
        system((char*)pu8buf);
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
    }
    else
    {
        printf("Format : Demo_LimitDhrystone <on/off> <task num> <Percentage CPU0> ... <Percentage CPU3>\n");
        printf("                   <on/off> 1 or 0\n");
        printf("                 <task num> 1 ~ 4\n");
        printf("          <Percentage CPUx> 1 ~ 100%%\n");
    }

    return bRet;
}

#endif
