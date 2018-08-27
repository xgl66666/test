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

#if (DEMO_SYS_INFO_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "MsCommon.h"
#include "MsFS.h"
#include "demo_utility.h"
#include "demo_SysInfo.h"
#include "drvMMIO.h"
#include "drvMIU.h"
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


//-------------------------------------------------------------------------------------------------
// Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL get_cpu_number(MS_U32* pu32CpuNum )
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;

    FILE *cpuinfo_fp;
    MS_S32  s32Index = -1;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(TEMP_BUFFER_SZ, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }

    // open file description
    cpuinfo_fp = MsFS_Fopen("/proc/stat","r");
    if( cpuinfo_fp == NULL )
    {
        printf("Fail to open file descruption\n");
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        return(FALSE);
    }

    while( s32Index < CPU_MAX_NUM )
    {
        if( fgets((char*)pu8buf, TEMP_BUFFER_SZ, cpuinfo_fp) == NULL )break;
        if( strstr((char*)pu8buf, "cpu") )
        {
            s32Index++;
        }
    }
    MsFS_Fclose(cpuinfo_fp);
    MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);

    *pu32CpuNum = s32Index;
    return(TRUE);

}

void calculate_cpu_utilization( TSample* ptCPU_1st, TSample* ptCPU_2nd,MS_U32 u32CpuNum , MS_FLOAT* pfCpuUtilization )
{
    /* Calculate CPU utilization

        now_total = USER+NICE+SYSTEM+IDLE+IOWAIT+IRQ+SOFTIRQ
        now_busy = USER+NICE+SYSTEM+IOWAIT+IRQ+SOFTIRQ

        delta_busy = now_busy - previous_busy
        delta_total = now_total - previous_total

        CPU utilization = delta_busy / delta_total *100

        previous_xxxx = now_xxxx
    */
    MS_U32      u32Index;
    MS_S32      s32Delta_Busy;
    MS_S32      s32Delta_Total;
    MS_FLOAT    fCnt;

    for( u32Index = 0 ; u32Index < u32CpuNum ; u32Index++ )
    {
        s32Delta_Busy = ptCPU_2nd->u32User;
        s32Delta_Busy += ptCPU_2nd->u32Nice;
        s32Delta_Busy += ptCPU_2nd->u32System;
        s32Delta_Busy += ptCPU_2nd->u32IOWait;
        s32Delta_Busy += ptCPU_2nd->u32Irq;
        s32Delta_Busy += ptCPU_2nd->u32SWIrq;


        s32Delta_Busy -= ptCPU_1st->u32User;
        s32Delta_Busy -= ptCPU_1st->u32Nice;
        s32Delta_Busy -= ptCPU_1st->u32System;
        s32Delta_Busy -= ptCPU_1st->u32IOWait;
        s32Delta_Busy -= ptCPU_1st->u32Irq;
        s32Delta_Busy -= ptCPU_1st->u32SWIrq;

        s32Delta_Total = s32Delta_Busy;
        s32Delta_Total += ptCPU_2nd->u32Idle;
        s32Delta_Total -= ptCPU_1st->u32Idle;

        if( s32Delta_Busy > 0 && s32Delta_Total > 0 )
        {
            fCnt = s32Delta_Busy;
            fCnt /= s32Delta_Total;
            fCnt *= 100;

            *pfCpuUtilization = fCnt;
        }
        else
        {
            *pfCpuUtilization = 0;
        }
        pfCpuUtilization++;
        ptCPU_1st++;
        ptCPU_2nd++;
    }
}


MS_BOOL get_cpu_jiffer( MS_U32 u32CpuNum , TSample* ptCPU_jiffer )
{
    FILE *cpuinfo_fp ;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;
    char*  pcPntr;

    MS_S32  s32Index = -1;
    MS_S32  s32CpuNum = u32CpuNum;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(TEMP_BUFFER_SZ, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }

    // open file description
    cpuinfo_fp = MsFS_Fopen("/proc/stat","r");
    if( cpuinfo_fp == NULL )
    {
        printf("Fail to open file descruption\n");
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        return(FALSE);
    }

    memset((char*)pu8buf,0x00,TEMP_BUFFER_SZ);
    while( s32Index < s32CpuNum )
    {
        if( fgets((char*)pu8buf, TEMP_BUFFER_SZ, cpuinfo_fp) == NULL )break;
        if( strstr((char*)pu8buf, "cpu") )
        {
            if( s32Index != -1 )
            {
                pcPntr = (char*)pu8buf;
                pcPntr += 4;

                sscanf(pcPntr, "%d%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]"  ,&ptCPU_jiffer->u32User
                                                                                    ,&ptCPU_jiffer->u32Nice
                                                                                    ,&ptCPU_jiffer->u32System
                                                                                    ,&ptCPU_jiffer->u32Idle
                                                                                    ,&ptCPU_jiffer->u32IOWait
                                                                                    ,&ptCPU_jiffer->u32Irq
                                                                                    ,&ptCPU_jiffer->u32SWIrq );
                ptCPU_jiffer++;
            }
            s32Index++;
        }
    }
    MsFS_Fclose(cpuinfo_fp);
    MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);

    return(TRUE);
}

MS_BOOL Demo_cpu_information(void)
{
    MS_U32      u32CpuNum = 0;
    MS_FLOAT    fArrayCpuUtilization[CPU_MAX_NUM];
    MS_U32      u32Index;

    TSample     tCPU_1st[CPU_MAX_NUM];
    TSample     tCPU_2nd[CPU_MAX_NUM];

    if( get_cpu_number(&u32CpuNum) == FALSE )
    {
        printf("==> get_cpu_number error\n");
        return(FALSE);
    }

    if( get_cpu_jiffer( u32CpuNum , &tCPU_1st[0] ) == FALSE )
    {
        printf("==> 1st get_cpu_jiffer error\n");
        return(FALSE);
    }
    sleep(1);                   // At least delay 1 sec to let jiffer update
    if( get_cpu_jiffer( u32CpuNum , &tCPU_2nd[0] ) == FALSE )
    {
        printf("==> 2nd get_cpu_jiffer error\n");
        return(FALSE);
    }
    memset (fArrayCpuUtilization,0,CPU_MAX_NUM);
    calculate_cpu_utilization( &tCPU_1st[0], &tCPU_2nd[0], u32CpuNum , &fArrayCpuUtilization[0] );

    printf("==> CPU Numbers : %d \n",u32CpuNum);
    for( u32Index = 0 ; u32Index < u32CpuNum ; u32Index++ )
    {
        printf("    CPU%d = %.2f %%\n",u32Index,fArrayCpuUtilization[u32Index]);
    }
    printf("\n");

    return(TRUE);
}



MS_BOOL get_memory_info( MS_U32* pu32CacheMem , MS_U32* pu32FreeMem )
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;

    FILE *meminfo_fp ;
    char*  pcPntr;

    MS_U32  u32FreeMem = 0;
    MS_U32  u32CacheMem = 0;
    MS_BOOL bRet = FALSE;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(TEMP_BUFFER_SZ, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }
    memset (pu8buf,0x00,TEMP_BUFFER_SZ);

    meminfo_fp = MsFS_Fopen("/proc/meminfo","r");
    if( meminfo_fp == NULL )
    {
        printf("Fail to open file descruption\n");
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        return (FALSE);
    }

    while(1)
    {
        if( fgets((char*)pu8buf, TEMP_BUFFER_SZ, meminfo_fp) == NULL )break;

        if( strstr((char*)pu8buf, "MemFree:") )
        {
            pcPntr = (char*)pu8buf;
            pcPntr += sizeof("MemFree:");
            sscanf(pcPntr, "%d",&u32FreeMem);
        }

        if( strstr((char*)pu8buf, "Cached:") )
        {
            pcPntr = (char*)pu8buf;
            pcPntr += sizeof("Cached:");
            sscanf(pcPntr, "%d",&u32CacheMem);
            bRet = TRUE;
            break;
        }
    }
    MsFS_Fclose(meminfo_fp);
    MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);

    *pu32CacheMem = u32CacheMem;
    *pu32FreeMem = u32FreeMem;

    return bRet;
}

MS_BOOL Demo_memory_information(void)
{
    MS_U32 u32FreeMem = 0;
    MS_U32 u32CacheMem = 0;

    if( get_memory_info(&u32CacheMem,&u32FreeMem) == FALSE )
    {
        printf("==> get_memory_info error\n");
        return(FALSE);
    }

    printf("=> MemFree = %d KB , Cached = %d KB\n\n\n",u32FreeMem,u32CacheMem);

    return (TRUE);
}

MS_BOOL get_miu_number(MS_U32* pu32MIUNum )
{
    MS_PHY  BankSize;
    MS_VIRT NonPmBase;
    MS_U16  u16UseMIU;

    if( !MDrv_MMIO_GetBASE(&NonPmBase, &BankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }

    // Check if MIU1 init done
    u16UseMIU = REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU1,0x00));
    u16UseMIU &= 0x0001;
    if( u16UseMIU )
    {

        *pu32MIUNum = 2;      // Support MIU1
    }
    else
    {
        *pu32MIUNum = 1;      // MIU0 only
    }

    return TRUE;
}

MS_BOOL get_miu_band_width( MS_U32 u32MIUx , MS_U32* pu32BWCnt )
{
    MS_PHY  BankSize;
    MS_VIRT NonPmBase;

    MS_U16  u16MIUCnt;

    if( !MDrv_MMIO_GetBASE(&NonPmBase, &BankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }

    if( u32MIUx == 0 )
    {
        REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU0,0x0D)) = 0x0040;
        REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU0,0x0D)) = 0x0041;
        u16MIUCnt = REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU0,0x0E));
        *pu32BWCnt = u16MIUCnt;
    }
    else
    {
        REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU1,0x0D)) = 0x0040;
        REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU1,0x0D)) = 0x0041;
        u16MIUCnt = REG16_NPM(NonPmBase,MIX_ADDR(FOR_UTOPIA_BANK_MIU1,0x0E));
        *pu32BWCnt = u16MIUCnt;
    }

    return TRUE;
}

MS_BOOL Demo_miu_bandwidth(void)
{
    MS_U32  u32MIUNum;
    MS_U32  u32MIUCnt[2] = {0,0};
    MS_U32  u32Index;

    if( get_miu_number(&u32MIUNum) == FALSE )
    {
        printf("==> get_miu_number error\n");
        return(FALSE);
    }

    printf("=> MIU Support = MIU0");
    if( u32MIUNum == 2 )
    {
        printf(" , MIU1");
    }
    printf("\n\n");

    for( u32Index = 0 ; u32Index < u32MIUNum ; u32Index++ )
    {
        if( get_miu_band_width(u32Index,&u32MIUCnt[u32Index])== FALSE )
        {
            printf("==> get_miu_band_width error\n");
            return(FALSE);
        }
        printf("=> MIU%d cnt = %d %% (%d)\n",u32Index,(u32MIUCnt[u32Index]/10),u32MIUCnt[u32Index]);
    }
    printf("\n\n");


    return TRUE;
}

MS_BOOL Demo_printk_level( MS_U32* pu32Level )
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(50, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }
    memset((char*)pu8buf,0x00,50);

    if( *pu32Level > 8 )
    {
        printf("error input! level must be 0 ~ 8 \n");
        return(FALSE);
    }
    else
    {
        sprintf((char*)pu8buf, "echo %d > /proc/sys/kernel/printk",*pu32Level);
        printf("Current printk level : %d \n",*pu32Level);
    }
    system((char*)pu8buf);
    MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);

    return (TRUE);
}
#endif
