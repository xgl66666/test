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
#if (DEMO_BENCH_DHRYSTON_ECOS_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "demo_bench_dhryston.h"

#include <cyg/kernel/kapi.h>
#include <cyg/kernel/kapidata.h>
#include <cyg/cpuload/cpuload.h>
MS_BOOL Demo_eCos_CPULoad(void);
MS_BOOL Demo_eCos_CPULoad_OFF(void);

static MS_U32 cpu_delay = 1000;
//-------------------------------------------------------------------------------------------------
//                                MACROS
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
//
#define TASK_CPULOAD_STACKSIZE       0x2000
#define TASK_CPUEXTLOAD_STACKSIZE       0x2000
//static MS_U8 _u8TaskCPULoad[TASK_CPULOAD_STACKSIZE];
static MS_U8 _u8TaskCPUEXTLoad[TASK_CPUEXTLOAD_STACKSIZE];
MS_BOOL stopflag = FALSE;
MS_S32 s32CPUEXTLoadID = -1;
//static MS_S32 s32CPULoadID = -1;

static void _CPUExtLoadTaskStart (void)
{
    //int iArraySize = (int)(*(volatile unsigned short*)(REG_ADDR_BASE+(0x000e00<<1)+(0x34<<2)));
    int iArraySize = 0;
    char* x;
    char* z;

    int a = 1;
    int b = 3;
    int c = 0;
    int d = 0;
    int Run = 1;
    int Cnt = 0;
    int i = 0;


    if (iArraySize == 0)
    {
        iArraySize = 50;
        // 100 83%
        // 120 92%
    }

    x = (char*)malloc(iArraySize*sizeof(char));
    z = (char*)malloc(iArraySize*sizeof(char));


    while(1)
    {
        for(i=0;i<iArraySize;i++)
        {
            x[i] = 0;
            z[i] = 0;
        }

        x[1] = 2;
        z[1] = 2;
        Run = 1;
        Cnt = 0;
        a = 1;
        b = 3;
        c = 0;
        d = 0;

        while(Run && (++Cnt< 200000000))
        {
            d = 0;

            for(i=iArraySize-1; i>0; i--)
            {
                c = z[i]*a + d;
                z[i] = c % 10;
                d = c / 10;
            }

            d = 0;
            for(i=0; i< iArraySize; i++)
            {
                c = z[i]+d*10;
                z[i] = c / b;
                d = c % b;
            }

            Run = 0;
            for(i=iArraySize-1; i>0; i--)
            {
                c = x[i] + z[i];
                x[i] = c%10;
                x[i-1] += c/10;
                Run |= z[i];
            }
            a++;
            b+=2;
            //MsOS_DelayTask(1);
        }
        for(i = 0;i<10;i++)
        {
            //HB_DBG("%x",x[i]);
        }
        //HB_DBG("\n");
        //MsOS_DelayTask(u8CPUDelay);
        MsOS_DelayTaskUs(cpu_delay);
    }
}


MS_BOOL _CPUExtLoad(void)
{
    // for eCos
    if (s32CPUEXTLoadID < 0)
    {
        //_CPUAdjustDelay();
        memset(_u8TaskCPUEXTLoad,0,sizeof(MS_U8)*TASK_CPUEXTLOAD_STACKSIZE);
        s32CPUEXTLoadID = MsOS_CreateTask((TaskEntry) _CPUExtLoadTaskStart,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_LOWEST,
                                        TRUE,
                                        _u8TaskCPUEXTLoad,
                                        TASK_CPUEXTLOAD_STACKSIZE,
                                        "CPUExtLoad" );
    }
    return TRUE;
}

MS_BOOL execute_cpu_unload(void)
{
    if (s32CPUEXTLoadID != -1) {
        MsOS_DeleteTask(s32CPUEXTLoadID);
        s32CPUEXTLoadID = -1;
        printf("delete cpu load task\n");
    }
    return TRUE;
}

MS_BOOL bench_set_delay(MS_U32 *delay)
{
    printf("new cpu delay value = %d\n", *delay);
    cpu_delay = *delay;
    return TRUE;
}

MS_BOOL execute_cpu_load(void)
{
    _CPUExtLoad();
    return TRUE;
}


MS_BOOL Demo_Bench_Dhryston_Run(MS_U32 *pLoop, MS_U32 *pDelayTime, MS_U32 *pbBackground)
{
#if 0
    extern int cyg_dhrystone_test(void);
    MS_U32 u32Count = *pLoop;
    printf("this command will run test for %ld loops \n",u32Count);
    while(u32Count)
    {
        printf("%ld start \n",u32Count);
        cyg_dhrystone_test();
        printf("%ld End \n",u32Count);
        u32Count -- ;
        MsOS_DelayTask(*pDelayTime);
    }
#endif
    return TRUE;
}




#endif
