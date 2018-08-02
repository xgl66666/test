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
#if (DEMO_BENCH_MEM_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/string.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "demo_utility.h"
#include "drvDTC.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
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
#define MAX_MemBench_NUM 4
MS_BOOL Demo_Bench_Memory(MS_U32 *memSize, MS_U32 *cpyCounter)
{
    MS_U32 u32MemSize = 1024UL*256UL; //256kB
    MS_U32 u32CopyCounter = 1000;
    void * pSrcAddr = NULL;
    void * pDestAddr = NULL;
    MS_U32 u32timeStart = 0;
    MS_U32 u32timeEnd = 0;
    MS_U8 i = 0;
    MS_U32 j = 0;

    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
    MS_S32 s32MstarNonCachedPoolID=INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    MS_U32 u32SrcPool[MAX_MemBench_NUM] = {s32MstarCachedPoolID,s32MstarCachedPoolID,s32MstarNonCachedPoolID,s32MstarNonCachedPoolID};
    MS_U32 u32DestPool[MAX_MemBench_NUM] = {s32MstarCachedPoolID,s32MstarNonCachedPoolID,s32MstarCachedPoolID,s32MstarNonCachedPoolID};
    printf("Demo_Bench_Memory start.\n");
    if(*memSize != 0)
    {
        u32MemSize = *memSize;
    }
    if(*cpyCounter != 0)
    {
        u32CopyCounter = *cpyCounter;
    }
    printf("MemSize = %"DTC_MS_U32_u" Bytes, CopyCounter = %"DTC_MS_U32_u"\n", u32MemSize, u32CopyCounter);
    for(i = 0 ; i < MAX_MemBench_NUM ; i++)
    {
        pSrcAddr = MsOS_AllocateMemory(u32MemSize,u32SrcPool[i]);
        if(pSrcAddr == NULL)
        {
            printf("Demo_Bench_Memory alloc source buffer failed.\n");
            return FALSE;
        }
        pDestAddr = MsOS_AllocateMemory(u32MemSize,u32DestPool[i]);
        if(pDestAddr == NULL)
        {
            MsOS_FreeMemory(pSrcAddr,u32SrcPool[i]);
            printf("Demo_Bench_Memory alloc destination buffer failed.\n");
            return FALSE;
        }
        u32timeStart = MsOS_GetSystemTime();
        for(j = 0 ; j < u32CopyCounter ; j++)
        {
            memcpy(pDestAddr,pSrcAddr,u32MemSize);
        }
        u32timeEnd = MsOS_GetSystemTime();
        if(s32MstarCachedPoolID == u32SrcPool[i])
        {
            printf("***Source alloc in cache pool, ");
        }
        else
        {
            printf("***Source alloc in non-cache pool, ");
        }
        if(s32MstarCachedPoolID == u32DestPool[i])
        {
            printf("destination alloc in cache pool.***\n");
        }
        else
        {
            printf("destination alloc in non-cache pool.***\n");
        }
        printf("Spend %"DTC_MS_U32_u" ms to copy size %"DTC_MS_U32_u" Bytes, %"DTC_MS_U32_u" times\n", u32timeEnd - u32timeStart, u32MemSize, u32CopyCounter);
        MsOS_FreeMemory(pSrcAddr,u32SrcPool[i]);
        MsOS_FreeMemory(pDestAddr,u32DestPool[i]);
        pSrcAddr = NULL;
        pDestAddr = NULL;
    }
    return TRUE;
}

MS_BOOL Demo_Bench_Memory_Help(void)
{
    printf ("------------------------------------Bench Demo Help--------------------------------------\n");
    printf("1.Bench_Memory [memsize] [memcpycounter]\n");
    printf("-----memsize : The Memory size which want to allocate by Byte.\n");
    printf("-----memcpycounter : Memory copy counter for test.\n");
    printf ("---------------------------------End of Bench Demo Help----------------------------------\n");

    return TRUE;
}
#endif
