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
///
/// file    Memory.h
/// @brief  Memory Layout Configuration
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MS_MEMORY_H_
#define _MS_MEMORY_H_

#include "Board.h"
#define MEMALIGN(align, address)               ((((address)+(align)-1)/(align))*(align))
#define LOWMEMALIGN(align, address)            (((address)/(align))*(align))
#ifndef MI_ENABLE
#define MI_ENABLE 0
#endif

#ifdef AUDIO_GST
#error "No MMAP for Audio gstreamer lib now!!! Please add a proper MMAP first!!!"
#endif

#if (MI_ENABLE == 1)
#include "MMAP_MI.h"
#include "MMAP_MI_Convert.inc"
#define KERNEL_POOL_ADR        MI_KERNEL_POOL1_ADR
#define KERNEL_POOL_LEN        MI_KERNEL_POOL1_LEN
#else //#if (MI_ENABLE == 1)
#if(MEMORY_MAP == MMAP_128MB)
    //#include "MMAP_DDI_128.h"
    #include "MMAP_DDI_128_chal.h"
#elif(MEMORY_MAP == MMAP_64MB)
    #include "MMAP_DDI_64_SD.h"
#elif(MEMORY_MAP == MMAP_32MB)
    #include "MMAP_DDI_32.h"
#else
    #error "MEMORY_MAP is not defined!!"
#endif

#endif //#if (MI_ENABLE == 1)

#ifndef MEM_ADR_BY_MIU
#define MEM_ADR_BY_MIU(A, T)                    ((A) + (MIU_INTERVAL * ((T) & MIU1)))
#endif

#define ADD_MIU_INTERVAL(x)     (((x) & MIU1) == MIU1 ? MIU_INTERVAL : 0)

/* check miu boundary */
#if defined(MIU0_END_ADR) && defined(MIU_DRAM_LEN0) && (MIU0_END_ADR > MIU_DRAM_LEN0)
#error MIU0 buffer size exceed DRAM size
#endif

#if defined(MIU1_END_ADR) && defined(MIU_DRAM_LEN1) && (MIU1_END_ADR > MIU_DRAM_LEN1)
#error MIU1 buffer size exceed DRAM size
#endif


/* check co-buffer boundary */
#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L1_END_TAG_ADR) && (MIU0_COBUF_L1_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L1 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L2_END_TAG_ADR) && (MIU0_COBUF_L2_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L2 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L3_END_TAG_ADR) && (MIU0_COBUF_L3_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L3 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L4_END_TAG_ADR) && (MIU0_COBUF_L4_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L4 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L5_END_TAG_ADR) && (MIU0_COBUF_L5_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L5 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L6_END_TAG_ADR) && (MIU0_COBUF_L6_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L6 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L7_END_TAG_ADR) && (MIU0_COBUF_L7_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L7 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L8_END_TAG_ADR) && (MIU0_COBUF_L8_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L8 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L9_END_TAG_ADR) && (MIU0_COBUF_L9_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L9 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L10_END_TAG_ADR) && (MIU0_COBUF_L10_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L10 size exceed L0 size
#endif

#if defined(MIU0_COBUF_END_TAG_ADR) && defined(MIU0_COBUF_L11_END_TAG_ADR) && (MIU0_COBUF_L11_END_TAG_ADR > MIU0_COBUF_END_TAG_ADR)
#error MIU0 co-buffer L11 size exceed L0 size
#endif

#endif //_MS_MEMORY_H_
