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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   demo_os.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for the watchdog timer functions
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
  @file demo_os.c
  @brief The demo code for the OS functions
*/

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_ECOS
#include <cyg/fileio/fileio.h>
#include <cyg/cpuload/cpuload.h>
#include <cyg/kernel/kapi.h>
#include "demo_eCos_api.h"

#if (DEMO_eCos_API_TEST == 1)

#define TASK_CPULOAD_STACKSIZE 0x2000

static MS_S32 _s32CPULOAD_RUNIING = -1;  /*-1: off line 0: Command 1: on line*/

static MS_U8 _u8TaskCPULoad[TASK_CPULOAD_STACKSIZE];
MS_S32 s32CPULoadID = -1;
cyg_uint32 calibration;
cyg_cpuload_t cpuload;
cyg_handle_t cpuhandle;
cyg_uint32 average_point1s;
cyg_uint32 average_1s;
cyg_uint32 average_10s;


//-------------------------------------------------------------------------------------------------
/// Get CPU usage of each task in a period
/// unit: ms
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_eCos_CPUUsage(MS_U16* pu16Period)
{
  MApi_eCos_Get_CPU_Usage(*pu16Period);
  return TRUE;
}

MS_BOOL Demo_eCos_show_cpu_average_load(MS_U16* on)
{
    if (1 == *on)
        Demo_eCos_CPULoad();
    else if (0 == *on)
        Demo_eCos_CPULoad_OFF();
    return TRUE;
}

static void _CPULoadTaskStart (void)
{
    cyg_cpuload_calibrate(&calibration);
    cyg_cpuload_create(&cpuload,calibration,&cpuhandle);
    _s32CPULOAD_RUNIING = 1;

    while(_s32CPULOAD_RUNIING == 1)
    {
       // MsOS_DelayTask(1000);

        cyg_cpuload_get(cpuhandle,&average_point1s,&average_1s,&average_10s);
        printf("@@--%s--average_point1s = %d ; average_1s = %d ; average_10s = %d\n", __func__, average_point1s, average_1s, average_10s);
        MsOS_DelayTask(5000);
    }

    _s32CPULOAD_RUNIING = -1;
    cyg_cpuload_delete(cpuhandle);
}

MS_BOOL Demo_eCos_CPULoad(void)
{
    if (s32CPULoadID < 0)
    {
        memset(_u8TaskCPULoad,0,sizeof(MS_U8)*TASK_CPULOAD_STACKSIZE);

        s32CPULoadID = MsOS_CreateTask((TaskEntry) _CPULoadTaskStart,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_HIGH,
                                        TRUE,
                                        _u8TaskCPULoad,
                                        TASK_CPULOAD_STACKSIZE,
                                        "CPULoad" );
    }
    return TRUE;
}

MS_BOOL Demo_eCos_CPULoad_OFF(void)
{
    if (s32CPULoadID > 0)
    {
        _s32CPULOAD_RUNIING = 0;
    }

    while (_s32CPULOAD_RUNIING == 0)
    {
    	  MsOS_DelayTask(100);
    }

    if (_s32CPULOAD_RUNIING == -1)
    {
        return TRUE;
    }
    else
    {
    	  return FALSE;
    }
}


#endif

#endif

