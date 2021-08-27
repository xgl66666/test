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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
/// @file   MsMemory64_MI.h
/// @brief  MStar Common Interface Header File
/// @author MStar Semiconductor Inc.
/// @note   MsCommon.h includes most command header files including basic data type, macro(MsTypes.h),\n
/// board configuration(MsBoard.h), and OS related API(MsOS.h).\n
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MS_MEMORY64_SD_MI_H_
#define _MS_MEMORY64_SD_MI_H_


//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#define MIU0                        (0x0000)
#define MIU1                        (0x0001)

#define ENABLE_MIU_1 0
#define GOP_ENABLE_MIU_1 0
#define ADD_MIU_INTERVAL(x)     (((x) & MIU1) == MIU1 ? MIU_INTERVAL : 0)
#define MIU_INTERVAL                0x10000000



//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Memory Map
//-------------------------------------------------------------------------------------------------
#define MEM0_SIZE                               (64 * 0x00100000) /*  64Mb */

#define MEMORY_MIU_END_0                MEM0_SIZE


// OS memory pool
#define POOL_START                              0
#define POOL_SIZE                               (HW_BUFFER_END - POOL_START)//RESERVED_START_0

#define CACHED_POOL_BUF_LEN                     0x00400000  /* 4M */
#define CACHED_POOL_BUF_ADDR                    0 //it would get pool addr in create mem pool function

#define NON_CACHED_POOL_BUF_LEN                 POOL_SIZE //it would get pool size in create mem pool function
#define NON_CACHED_POOL_BUF_ADDR                0 //it would get pool addr in create mem pool function

//-------------------------------------------------------------------------------------------------
// Macro Setting
//-------------------------------------------------------------------------------------------------

#define VDEC_MIU_TYPE                           MIU0
#define VDEC_AEON_MEMORY_TYPE                   (VDEC_MIU_TYPE)
#define VDEC_FRAMEBUFFER_MEMORY_TYPE            (VDEC_MIU_TYPE)
#define VDEC_BITSTREAM_MEMORY_TYPE              (VDEC_MIU_TYPE)
#if (ENABLE_MM_PACKAGE == 1)
#define MM_COPROCESSOR_TYPE                     (MIU0)
#define VDPLAYER_MEMORY_TYPE                    (MIU0)
#define MIU0_LEN                                (64*1024*1024)
#define MIU1_LEN                                (0)
#endif
//-------------------------------------------------------------------------------------------------
// Function Block Memory Size
//-------------------------------------------------------------------------------------------------
#define SCALER_DNR_BUF_LEN                      0x00270000  /* 2.4M */
#define MAD_DEC_BUFFER_LEN                      0x00180000  /* 1.5MB */
#define MAD_SE_BUFFER_LEN                       MAD_DEC_BUFFER_LEN
#define VDEC_AEON_LEN                           0x00100000  /* 1M */
#define VDEC_BITSTREAM_LEN                      0x00200000  /* 2M */
#define VDEC_FRAMEBUFFER_LEN                    0x00570000  /* 5.5M*/
#define VDEC_LEN                                ((VDEC_AEON_LEN + VDEC_BITSTREAM_LEN) + VDEC_FRAMEBUFFER_LEN)
#define VE_FRAMEBUFFER_LEN                      0x00000000  /* 0 MB */
#define GOP_GWIN_RB_LEN                         0x006A0000
#define GOP_REGDMABASE_LEN                      0x00001000
#define TSP_VQ_LEN                              0x1000      /* 1K */
#define TSP_FW_LEN                              0x4000      /* 4K */
#define MENULOAD_BUFFER_LEN                     0x4000      /* 4K */
//-------------------------------------------------------------------------------------------------
// Memory Map
//-------------------------------------------------------------------------------------------------
//XC Scaler Buffer
#define MAIN_FB_START_ADDR                      (MEMORY_MIU_END_0 - SCALER_DNR_BUF_LEN)
#define SUB_FB_START_ADDR                       MAIN_FB_START_ADDR


/* MAD_DEC_BUFFER   */
#define MAD_DEC_BUFFER_ADR                      LOWMEMALIGN(0x800, (SUB_FB_START_ADDR - MAD_DEC_BUFFER_LEN))
#define MAD_SE_BUFFER_ADR                       MAD_DEC_BUFFER_ADR

/* VDEC_BUFFER    */
#define VDEC_MEM_START                          MAD_SE_BUFFER_ADR - VDEC_LEN
#define VDEC_AEON_ADR                           MEMALIGN(0x10, VDEC_MEM_START) /* Alignment 0x00010 */
#define VDEC_FRAMEBUFFER_ADR                    MEMALIGN(0x2000, VDEC_AEON_ADR + VDEC_AEON_LEN)  /* Alignment 0x00200 */
#define VDEC_BITSTREAM_ADR                      MEMALIGN(0x8, VDEC_FRAMEBUFFER_ADR + VDEC_FRAMEBUFFER_LEN)  /* Alignment 0x00008 */

/* VE Buffer      */
#define VE_FRAMEBUFFER_ADR                      (VDEC_MEM_START - VE_FRAMEBUFFER_LEN)  /* Alignment 0x00008 */


#define SECTION_BUF_LEN                         0x00100000 /*  1M */
#define SECTION_BUF_ADDR                        LOWMEMALIGN(0x10, VE_FRAMEBUFFER_ADR - SECTION_BUF_LEN)

/* TSP VQ FW Buffer */
#define TSP_FW_BUF                              LOWMEMALIGN(0x100, SECTION_BUF_ADDR - TSP_FW_LEN)

#define TSP_FQ_BUFFER_LEN                       LOWMEMALIGN(192,0x200000)
#if (TSP_FQ_BUFFER_LEN > VDEC_BITSTREAM_LEN)
#warning "TSP_FQ_BUFFER_LEN > VDEC_BITSTREAM_LEN"
#endif
#define TSP_FQ_BUFFER_ADR                       LOWMEMALIGN(0x10,(TSP_FW_BUF - TSP_FQ_BUFFER_LEN))
#define TSP_VQ_BUF                              (TSP_FQ_BUFFER_ADR - TSP_VQ_LEN)

/* PM51_MEM   */
//STR variable
#define PM51_RESERVED_LEN                         0x00010000
#define PM51_RESERVED_ADDR                        MEMALIGN(0x10000, TSP_VQ_BUF - 0x10000)

#define PM51_LITE_LEN                             0x00008000
#define PM51_LITE_ADDR                            MEMALIGN(0x1000, PM51_RESERVED_ADDR)
#define PM51_MIU_TYPE                             (MIU0)

//STR variable
#define PM51_MIU_ENTER_SR_LEN                   0x100
#define PM51_MIU_ENTER_SR_ADDR                  MEMALIGN(0x1000, TSP_VQ_BUF + TSP_VQ_LEN)
#define PM51_MIU_EXIT_SR_LEN                    0x100
#define PM51_MIU_EXIT_SR_ADDR                   MEMALIGN(0x1000, PM51_MIU_ENTER_SR_ADDR + PM51_MIU_ENTER_SR_LEN)
#define PM51_HWIP_CLOSE_LEN                     0x100
#define PM51_HWIP_CLOSE_ADDR                    MEMALIGN(0x1000, PM51_MIU_EXIT_SR_ADDR + PM51_MIU_EXIT_SR_LEN)
#define PM51_HWIP_OPEN_LEN                      0x100
#define PM51_HWIP_OPEN_ADDR                     MEMALIGN(0x1000, PM51_HWIP_CLOSE_ADDR + PM51_HWIP_CLOSE_LEN)

// PM51_MEM
#define PM51_MEM_LEN                            0x00001000   // Size 4K
#define PM51_MEM_ADR                            MEMALIGN(0x1000, PM51_RESERVED_ADDR + PM51_LITE_LEN) // Alignment 0x01000(4K alignment)
//#define PM51_LITE_LEN                           0x00004000
//#define PM51_LITE_ADDR                          MEMALIGN(0x1000, PM51_MEM_ADR + PM51_MEM_LEN)
/* PM51_MEM END */


/* GOP GWIN DMA Buffer */
#define GOP_GWIN_RB_ADR                         MEMALIGN(0x8, PM51_RESERVED_ADDR - GOP_GWIN_RB_LEN) /*  Alignment 0x00008 */
#define SEAMLESS_ZAPPING_BUF_LEN                0x4000
#define SEAMLESS_ZAPPING_BUF_ADR                MEMALIGN(0x00020, (GOP_GWIN_RB_ADR - SEAMLESS_ZAPPING_BUF_LEN)) /* Alignment 0x00020 */
#define SEAMLESS_ZAPPING_BUF_MEMORY_TYPE        (VDEC_MIU_TYPE)

#define GOP_REGDMABASE_ADR                      MEMALIGN(0x01000,(SEAMLESS_ZAPPING_BUF_ADR - GOP_REGDMABASE_LEN)) /*   Alignment 0x01000 */

/* MENULOAD_BUFFER Must the same MIU with VDEC_AEON to Do Dynamic Scaling */
// Scaler Menu Load, co_buffer L0
#define MENULOAD_BUFFER_ADR                     MEMALIGN(0x10,(GOP_REGDMABASE_ADR - MENULOAD_BUFFER_LEN)) /* Alignment 0x00010 */
#define MENULOAD_BUFFER_MEMORY_TYPE             (MIU0)
/* MENULOAD_BUFFER END */

//#define SDRAMADDR_BMP                           (MENULOAD_BUFFER_ADR - SDRAMADDR_BMP_LEN)
//#define SDRAMADDR_BMP_LEN                       0x001ADB00
//#define SDRAMADDR_BMP_LEN                       0x00A00000

//#define SDRAMADDR_FONT                          (SDRAMADDR_BMP -  SDRAMADDR_FONT_LEN)
//#define SDRAMADDR_FONT_LEN                      0x00005000
//#define SDRAMADDR_FONT_LEN                      0x00100000

#define JPD_READBUFF_LEN                        0x00050000
#define JPD_INTERBUFF_LEN                       0x00630000
#define JPD_OUT_LEN                             0x003F4800
#define PHOTO_SHAREMEM_LEN                      0x00001000
#if (ENABLE_MM_PACKAGE == 1)
#define VDPLAYER_DATA_LEN                       0x00380000 /* size 3.5M */
#define VDPLAYER_SUB_BS_LEN                     0x00300000 /* size 3M */
#define VDPLAYER_BS_LEN                         0x00200000 /* size 2M */
#define VDPLAYER_BS_EXT_LEN                     0x00280000 /* size 2.5M */
#define VDPLAYER_SUB_LEN                        0x00040000 /* size 256K */
#define VDPLAYER_DMX_SEC_LEN                    0x00040000 /* size 256K */
#define COBUFFER_LEN  (VDPLAYER_DATA_LEN + VDPLAYER_SUB_BS_LEN + VDPLAYER_BS_LEN + VDPLAYER_BS_EXT_LEN + VDPLAYER_SUB_LEN + VDPLAYER_DMX_SEC_LEN)
#else
#define COBUFFER_LEN      (JPD_READBUFF_LEN + JPD_INTERBUFF_LEN + JPD_OUT_LEN + PHOTO_SHAREMEM_LEN)
#endif

#define COBUFFER_ADR  MEMALIGN(0x10, MENULOAD_BUFFER_ADR - COBUFFER_LEN)

#define STILL_IMG_ZAPPING_BUF_ADR               COBUFFER_ADR
#define STILL_IMG_ZAPPING_BUF_LEN               0x32A000

/* JPD Memory */ //0x820000
#define JPD_READBUFF_ADR                        COBUFFER_ADR /* Alignment 0x00010 */
#define JPD_INTERBUFF_ADR                       MEMALIGN(0x200, JPD_READBUFF_ADR + JPD_READBUFF_LEN) /* Alignment 0x00200 */
//(TVfunc == TVfunc_MpegSD)
#define JPD_OUT_ADR                             MEMALIGN(0x8, JPD_INTERBUFF_ADR + JPD_INTERBUFF_LEN) /* Alignment 0x00008 */
#define PHOTO_SHAREMEM_ADR                      (JPD_OUT_ADR + JPD_OUT_LEN)


#if (ENABLE_MM_PACKAGE == 1)
/* MM co-procesor   */
#define MM_COPROCESSOR_ADDR                     0x00000000  /* Alignment 0 */
#define MM_COPROCESSOR_LEN                      0x00200000 /* 2M */
/* VDPLAYER_MEM   */
#define VDPLAYER_ADR                            COBUFFER_ADR
#define VDPLAYER_DATA_ADR                       (VDPLAYER_ADR)
#define VDPLAYER_SUB_BS_ADR                     (VDPLAYER_DATA_ADR + VDPLAYER_DATA_LEN)
#define VDPLAYER_BS_ADR                         (VDPLAYER_SUB_BS_ADR + VDPLAYER_SUB_BS_LEN)
#define VDPLAYER_BS_EXT_ADR                     (VDPLAYER_BS_ADR + VDPLAYER_BS_LEN)
#define VDPLAYER_SUB_ADR                        (VDPLAYER_BS_EXT_ADR + VDPLAYER_BS_EXT_LEN)
#define VDPLAYER_DMX_SEC_ADR                    (VDPLAYER_SUB_ADR + VDPLAYER_SUB_LEN)
/* VDPLAYER END */
#endif

#if (MI_INJECT_FEATURE_ENABLE == 1)
/*INJECT_MEM*/
#define INJECTION_BUFFER_LEN                    0x400000  /* Size 4M*/
#define INJECTION_BUFFER_ADR                    MEMALIGN(0x1000, COBUFFER_ADR)
#endif
#define HW_BUFFER_END                           COBUFFER_ADR

//-------------------------------------------------------------------------------------------------
// Memory Map End
//-------------------------------------------------------------------------------------------------



#endif //



