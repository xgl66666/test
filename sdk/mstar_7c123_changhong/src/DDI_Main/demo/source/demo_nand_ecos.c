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
#if (DEMO_NAND_ECOS_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include <string.h>
#include <cyg/kernel/kapi.h>
#include "MsOS.h"
#include "demo_nand_ecos.h"
#include "demo_utility.h"

#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <cyg/fileio/fileio.h>

#include "drv_unfd.h"
#include "drvNAND_err_codes.h"
#include "drvNAND_kaiser_noos.h"

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
static MS_U8 _gu8CISData[] = {
    #include "NANDINFO_v2.h"
};

static MS_U8 _gu8PNIData[] = {
    #include "PARTITION_v2_default.h"
};

extern void*  AllocCachedMem(int size);
extern void   FreeCachedMem(void* pBuf);
extern void*  AllocNoncachedMem(int size);
extern void   FreeNoncachedMem(void* pBuf);

static __inline void dump_mem_line(MS_U8 *buf, int cnt)
{
        int i;
        printf(" 0x%08lx: ", (MS_U32)buf);
        for (i= 0; i < cnt; i++)
                printf("%02X ", buf[i]);

        printf(" | ");

        for (i = 0; i < cnt; i++)
                printf("%c", (buf[i] >= 32 && buf[i] < 128) ? buf[i] : '.');

        printf("\n");
}

static void dump_mem(MS_U8 *buf, int cnt)
{
        int i;
        for (i= 0; i < cnt; i+= 16)
                dump_mem_line(buf + i, 16);
}

MS_BOOL Demo_NAND_WriteCIS(void)
{
    MS_U32 u32_Ret;
    u32_Ret = drvNAND_Init(_gu8PNIData);
    if (u32_Ret == UNFD_ST_ERR_NO_CIS)
    {
       printf("Write CIS\n");
       u32_Ret = drvNAND_WriteCIS(_gu8CISData, _gu8PNIData);
       if (u32_Ret != UNFD_ST_SUCCESS)
       {
            printf("ERROR, drvNAND_Init fail: %X\n", u32_Ret);
            return FALSE;
       }
    }

    return TRUE;
}

MS_BOOL Demo_NAND_Init(void)
{
    if(!MDrv_Nand_init(
        AllocCachedMem,
        FreeCachedMem,
        AllocNoncachedMem,
        FreeNoncachedMem,
        _gu8CISData,
        _gu8PNIData))
        return FALSE;

    return TRUE;
}

MS_BOOL Demo_UNFD_Init(void)
{
    if (!drv_unfd_init(_gu8PNIData))
    {
        printf("ERROR, drvNAND_Init fail\n");
        return FALSE;

    }
    return TRUE;
}

MS_BOOL Demo_NAND_GetFATCapacity(void)
{
    MS_U32 u32_FATCapacity;
    u32_FATCapacity = drvNAND_GetFATCapacity();
    printf("[NAND] FATCapacity:%x\n", u32_FATCapacity);
    return TRUE;
}

MS_BOOL Demo_NAND_WriteFAT(MS_U32 *u32_SrcVA, MS_U32 *u32_StartSector, MS_U32 *u32_SectorCnt)
{
    MS_U32 u32_Ret;
    u32_Ret = drvNAND_WriteFAT(*u32_SrcVA, *u32_StartSector, *u32_SectorCnt);
    if (UNFD_ST_SUCCESS != u32_Ret)
    {
        printf("drvNAND_WriteFAT: %X \n", u32_Ret);
        return FALSE;
    }

    dump_mem((MS_U8*)*u32_SrcVA, 512);
    return TRUE;
}

MS_BOOL Demo_NAND_ReadFAT(MS_U32 *u32_SrcVA, MS_U32 *u32_StartSector, MS_U32 *u32_SectorCnt)
{
    MS_U32 u32_Ret;
    u32_Ret = drvNAND_ReadFAT(*u32_SrcVA, *u32_StartSector, *u32_SectorCnt);
    if (UNFD_ST_SUCCESS != u32_Ret)
    {
        printf("drvNAND_ReadFAT: %X \n", u32_Ret);
        return FALSE;
    }
    dump_mem((MS_U8*)*u32_SrcVA, 512);
    return TRUE;
}

MS_BOOL Demo_NAND_Compare(MS_U32 *pu32_buf0, MS_U32 *pu32_buf1, MS_U32 *u32_byteCnt)
{
    MS_U32 u32_i;
    MS_U32 *W_PHY = (MS_U32*)*pu32_buf0;
    MS_U32 *R_PHY = (MS_U32*)*pu32_buf1;
    for (u32_i=0; u32_i < *u32_byteCnt; u32_i++)
    {
        if (W_PHY[u32_i] != R_PHY[u32_i])
        {
            printf("error: i:%08X  buf0:%02X  buf1:%02X \n", u32_i, W_PHY[u32_i], R_PHY[u32_i]);
            return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL Demo_NAND_Verify(MS_U32 *pu32_buf0, MS_U32 *pu32_buf1)
{
    MS_U32 u32_Ret;
    MS_U32 *R_PHY0 = (MS_U32*)*pu32_buf0;
    MS_U32 *R_PHY1 = (MS_U32*)*pu32_buf1;
    MS_U32 *u32_StartSector = (MS_U32*)0x0;
    MS_U32 *u32_SectorCnt1  = (MS_U32*)0x1;
    MS_U32 *u32_SectorCnt2  = (MS_U32*)0x2;
    MS_U32 *u32_byteCnt     = (MS_U32*)0x80;

    Demo_NAND_Init();

    u32_Ret = Demo_NAND_ReadFAT((MS_U32*)&R_PHY0, (MS_U32*)&u32_StartSector, (MS_U32*)&u32_SectorCnt1);
    if (TRUE != u32_Ret)
    {
        printf("Demo_NAND_ReadFAT: %X \n", u32_Ret);
        return FALSE;
    }

    u32_Ret = Demo_NAND_WriteFAT((MS_U32*)&R_PHY0, (MS_U32*)&u32_StartSector, (MS_U32*)&u32_SectorCnt2);
    if (TRUE != u32_Ret)
    {
        printf("Demo_NAND_WriteFAT: %X \n", u32_Ret);
        return FALSE;
    }

    u32_Ret = Demo_NAND_ReadFAT((MS_U32*)&R_PHY1, (MS_U32*)&u32_StartSector, (MS_U32*)&u32_SectorCnt2);
    if (TRUE != u32_Ret)
    {
        printf("Demo_NAND_ReadFAT: %X \n", u32_Ret);
        return FALSE;
    }

    u32_Ret = Demo_NAND_Compare((MS_U32*)&R_PHY0, (MS_U32*)&R_PHY1, (MS_U32*)&u32_byteCnt);
    if (TRUE != u32_Ret)
    {
        printf("Demo_NAND_Compare: %X \n", u32_Ret);
        return FALSE;
    }

    printf("\nVerify UNFD_NAND Compare: ok\n");

    return TRUE;
}
#endif
