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

#if (DEMO_DVFS_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsFS.h"
#include "demo_SysInfo.h"
#include "demo_utility.h"
#include "demo_dvfs.h"
#include "drvCPU.h"


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

MS_BOOL force_cpu_frequency( MS_U32 u32CPUFreq )
{
    FILE *cpu_calibrating_fd;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(10, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }

    // open file description
    cpu_calibrating_fd = MsFS_Fopen("/proc/CPU_calibrating","w");
    if( cpu_calibrating_fd == NULL )
    {
        printf("Fail to open file descruption\n");
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        return (FALSE);
    }

    snprintf((char*)pu8buf, 10, "%d", u32CPUFreq);
    MsFS_Fwrite(pu8buf,1,10,cpu_calibrating_fd);
    MsFS_Fclose(cpu_calibrating_fd);
    MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);

    return (TRUE);
}

//-------------------------------------------------------------------------------------------------
// Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DVFS_UnLock_CPU_Frequency(void)
{
    if( force_cpu_frequency(0) == FALSE )
    {
        printf("Fail to Un-Lock CPU Frequency!\n");
        return (FALSE);
    }
    return (TRUE);
}

MS_BOOL Demo_DVFS_Lock_CPU_Frequency( MS_U32* pu32CPUFreq )
{
   if( Demo_DVFS_UnLock_CPU_Frequency() == FALSE )
    {
        printf("Fail to Un-Lock CPU Frequency!\n");
        return (FALSE);
    }

    if( force_cpu_frequency(*pu32CPUFreq) == FALSE )
    {
        printf("Fail to Lock CPU Frequency %d MHz!\n",*pu32CPUFreq);
        return (FALSE);
    }

    return (TRUE);
}

MS_BOOL Demo_DVFS_Get_CPU_Frequency( void )
{
    MS_U32 u32CpuFreq;

    u32CpuFreq = MDrv_CPU_QueryClock();
    u32CpuFreq /= 1000000;
    printf("CPU Freq : %d MHz\n\n",u32CpuFreq);

    return (TRUE);
}

MS_S32 ParseIntValue( MS_U8 u8BOF , MS_U8 u8EOF , MS_U8* pu8StreamPntr ,MS_U32* u32IncreaseIndex ,MS_S32 s32Read_size )
{
    MS_U8* pu8Pntr = pu8StreamPntr;
    MS_U32 u32Index = 0;
    MS_U32 u32NumOfByte = 0;
    MS_U8 u8ASCII_Array[5] = {0,0,0,0,0};
    MS_S32 s32Retvalue = -1;
    MS_BOOL bExit = FALSE;

    while( s32Read_size > 2 )                                       // BOF <Value> EOF , at least 2 Bytes
    {
        if( bExit == TRUE )break;

        if( *pu8Pntr == u8BOF )                                      // BOF
        {
            while( s32Read_size > 0 )
            {
                pu8Pntr++;
                u32Index++;
                s32Read_size--;
                if( *pu8Pntr == u8EOF )                             // EOF
                {
                    u32Index++;
                    bExit = TRUE;
                    break;
                }
                u8ASCII_Array[u32NumOfByte] = *pu8Pntr;
                u32NumOfByte++;
            }
        }
        else
        {
            pu8Pntr++;
            u32Index++;
            s32Read_size--;
        }
    }
    *u32IncreaseIndex = u32Index;

    switch( u32NumOfByte )
    {
        case 1 :    s32Retvalue = (u8ASCII_Array[0]-0x30);
                    break;

        case 2 :    s32Retvalue = (u8ASCII_Array[0]-0x30);
                    s32Retvalue *= 10;
                    s32Retvalue += (u8ASCII_Array[1]-0x30);
                    break;

        case 3 :    u32Index = (u8ASCII_Array[0]-0x30);
                    u32Index *= 100;
                    s32Retvalue = u32Index;
                    u32Index = (u8ASCII_Array[1]-0x30);
                    u32Index *= 10;
                    s32Retvalue += u32Index;
                    s32Retvalue += (u8ASCII_Array[2]-0x30);
                    break;

        case 4 :    u32Index = (u8ASCII_Array[0]-0x30);
                    u32Index *= 1000;
                    s32Retvalue = u32Index;
                    u32Index = (u8ASCII_Array[1]-0x30);
                    u32Index *= 100;
                    s32Retvalue += u32Index;
                    u32Index = (u8ASCII_Array[2]-0x30);
                    u32Index *= 10;
                    s32Retvalue += u32Index;
                    s32Retvalue += (u8ASCII_Array[3]-0x30);
                    break;

        default :   s32Retvalue = -1;
                    break;
    }

    return( s32Retvalue );

}

MS_BOOL Demo_DVFS_Get_CPU_Temperature( void )
{
    FILE *fd;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;
    MS_U8* pu8Free = NULL;
    MS_S32 s32Read_size;

    MS_S32  s32CPUIndex = 0;
    MS_S32  s32CPUNum;
    MS_S32  s32CPU_Temperature[CPU_MAX_NUM];
    MS_U32  u32StreamIndex;

    MS_BOOL bRet = TRUE;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(DVFS_TEMP_BUFF_SIZE, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }

    // open file description
    fd = MsFS_Fopen("/proc/mstar_dvfs/temperature","r");
    if( fd == NULL )
    {
        printf("Fail to open file descruption\n");
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        return (FALSE);
    }

    /*
        Node return format
        ------------------------
        Det_<CPU_Index>_temp:<Value>:
    */
    memset((char*)pu8buf,0x00,DVFS_TEMP_BUFF_SIZE);
    s32Read_size = MsFS_Fread((void *)pu8buf, 1, DVFS_TEMP_BUFF_SIZE, fd);
    pu8Free = pu8buf;

    while( s32Read_size > 0 )
    {
        s32CPUNum = s32CPUIndex;
        s32CPUIndex = ParseIntValue(DVFS_LINUX_NODE_CPU_BOF,DVFS_LINUX_NODE_CPU_EOF,pu8buf,&u32StreamIndex ,s32Read_size);
        if( s32CPUIndex == -1 )
        {
            break;
        }
        pu8buf += u32StreamIndex;
        s32Read_size -= u32StreamIndex;

        s32CPU_Temperature[s32CPUIndex] = ParseIntValue(DVFS_LINUX_NODE_VALUE_BOF,DVFS_LINUX_NODE_VALUE_EOF,pu8buf,&u32StreamIndex ,s32Read_size );
        if( s32CPU_Temperature[s32CPUIndex] == -1 )
        {
            bRet = FALSE;
            break;
        }
        pu8buf += u32StreamIndex;
        s32Read_size -= u32StreamIndex;
    }

    if( bRet )
    {
        for( u32StreamIndex = 0 ; u32StreamIndex< (s32CPUNum+1) ; u32StreamIndex++ )
        {
            printf("CPU_%d : %d degree C\n",u32StreamIndex,s32CPU_Temperature[u32StreamIndex]);
        }
    }
    else
    {
        printf("Fail to parse the information of cpus temperature! \n");
    }

    MsFS_Fclose(fd);
    MsOS_FreeMemory(pu8Free, s32MstarNonCachedPoolID);

    return (bRet);
}

MS_BOOL Demo_DVFS_Get_CPU_Voltage( void )
{
    FILE *fd;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8* pu8buf = NULL;
    MS_U8* pu8Free = NULL;

    MS_S32 s32Read_size;
    MS_S32 s32CPUIndex = 0;
    MS_S32 s32CPUNum;
    TDVFS_CPU_Voltage   stVolt[CPU_MAX_NUM];

    MS_U32  u32StreamIndex;

    MS_BOOL bRet = TRUE;

    // allocate buffer
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    pu8buf = MsOS_AllocateMemory(DVFS_TEMP_BUFF_SIZE, s32MstarNonCachedPoolID);
    if ( pu8buf == NULL )
    {
        printf("Fail to allocate memory\n");
        return (FALSE);
    }

    // open file description
    fd = MsFS_Fopen("/proc/mstar_dvfs/voltage","r");
    if( fd == NULL )
    {
        printf("Fail to open file descruption\n");
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        return (FALSE);
    }

    /*
        Node return format
        ------------------------
        Det_<CPU_Index>_avg:<Value>:
        Det_<CPU_Index>_min:<Value>:
        Det_<CPU_Index>_max:<Value>:
    */

    memset((char*)pu8buf,0x00,DVFS_TEMP_BUFF_SIZE);
    s32Read_size = MsFS_Fread((void *)pu8buf, 1, DVFS_TEMP_BUFF_SIZE, fd);
    pu8Free = pu8buf;
    while( s32Read_size > 0 )
    {
        s32CPUNum = s32CPUIndex;
        s32CPUIndex = ParseIntValue(DVFS_LINUX_NODE_CPU_BOF,DVFS_LINUX_NODE_CPU_EOF,pu8buf,&u32StreamIndex ,s32Read_size);
        if( s32CPUIndex == -1 )
        {
            break;
        }
        pu8buf += u32StreamIndex;
        s32Read_size -= u32StreamIndex;

        stVolt[s32CPUIndex].s32volt_avg = ParseIntValue(DVFS_LINUX_NODE_VALUE_BOF,DVFS_LINUX_NODE_VALUE_EOF,pu8buf,&u32StreamIndex ,s32Read_size );
        if( stVolt[s32CPUIndex].s32volt_avg == -1 )
        {
            bRet = FALSE;
            break;
        }
        pu8buf += u32StreamIndex;
        s32Read_size -= u32StreamIndex;

        stVolt[s32CPUIndex].s32volt_min = ParseIntValue(DVFS_LINUX_NODE_VALUE_BOF,DVFS_LINUX_NODE_VALUE_EOF,pu8buf,&u32StreamIndex ,s32Read_size );
        if( stVolt[s32CPUIndex].s32volt_min == -1 )
        {
            bRet = FALSE;
            break;
        }
        pu8buf += u32StreamIndex;
        s32Read_size -= u32StreamIndex;

        stVolt[s32CPUIndex].s32volt_max = ParseIntValue(DVFS_LINUX_NODE_VALUE_BOF,DVFS_LINUX_NODE_VALUE_EOF,pu8buf,&u32StreamIndex ,s32Read_size );
        if( stVolt[s32CPUIndex].s32volt_max == -1 )
        {
            bRet = FALSE;
            break;
        }
        pu8buf += u32StreamIndex;
        s32Read_size -= u32StreamIndex;
    }

    if( bRet )
    {
        for( u32StreamIndex = 0 ; u32StreamIndex< (s32CPUNum+1) ; u32StreamIndex++ )
        {
            printf("CPU_%d : [avg : %d mV ] [ min : %d mV ] [ max : %d mV ]\n"  ,u32StreamIndex
                                                                                ,stVolt[u32StreamIndex].s32volt_avg
                                                                                ,stVolt[u32StreamIndex].s32volt_min
                                                                                ,stVolt[u32StreamIndex].s32volt_max);
        }
    }
    else
    {
        printf("Fail to parse the information of cpus temperature! \n");
    }

    MsFS_Fclose(fd);
    MsOS_FreeMemory(pu8Free, s32MstarNonCachedPoolID);

    return (bRet);
}

#endif
