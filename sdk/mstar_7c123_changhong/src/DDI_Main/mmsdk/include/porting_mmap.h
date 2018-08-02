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

#ifndef __porting_mmap__
#define __porting_mmap__
#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/// @file
/// @defgroup porting_mmap porting_mmap functions
/// @{

///The memory layout info
typedef struct
{
    /// total size of memory layout
    MMSDK_U32    u32TotalMemSize;
    /// size of Miu0
    MMSDK_U32    u32Miu0MemSize;
    /// size of Miu1
    MMSDK_U32    u32Miu1MemSize;
    /// Miu interval
    MMSDK_U32    u32MiuBoundary;
    /// number of MMAP info
    MMSDK_U32    u32NMmapItems;
    /// size of Miu2
    MMSDK_U32    u32Miu2MemSize;
    /// Miu interval 2
    MMSDK_U32    u32MiuBoundary2;
    /// Check the miu is 4k alignment
    MMSDK_BOOL   bIs4KAlignment;
}ST_MMSDK_MMAP_INFO;

 ///@deprecated
typedef struct
{
    MMSDK_U32 u32Pool_Handle_Id;
    MMSDK_U64 u64Offset_In_Pool;
    MMSDK_U32 u32Length;
    MMSDK_BOOL bIsAllocated;
}ST_MMSDK_CMA_Pool_Param;

/// The memory buffer layout info
typedef struct
{
    /// address
    MMSDK_U32 u32Addr;
    /// virtual addr
    MMSDK_U32 u32VirtualAddr;
    /// size
    MMSDK_U32 u32Size;
    /// @deprecated miu setting new:#ST_MMSDK_BUF_INFO::u32MiuNo
    MMSDK_BOOL bIsMiu0;
    /// multi miu setting
    MMSDK_U32 u32MiuNo;
    /// miu alignment
    MMSDK_U32 u32Align;
    /// use cma or not
    MMSDK_BOOL bIsCMA;
    /// heap id
    MMSDK_U32 u32CMAHid;
    ///@deprecated
    ST_MMSDK_CMA_Pool_Param stCMAPoolPara;
} ST_MMSDK_BUF_INFO;

///@deprecated
typedef enum
{
    /// physical address
    E_MMSDK_BUF_INFO_PHY_ADDR,
    /// virtual address
    E_MMSDK_BUF_INFO_VIRTUAL_ADDR,
    /// buffer size
    E_MMSDK_BUF_INFO_SIZE,
    /// locate on MIU0 or MIU1
    E_MMSDK_BUF_INFO_ISMIU0,
}EN_MMSDK_BUF_INFO;

///define mmsdk needed physical buffer
typedef enum
{
    /// photo display buf
    E_MMSDK_BUF_PHOTO_DISPLAY,
    /// MPO has two jpeg frames, need extra buffer to store one frame.
    E_MMSDK_BUF_PHOTO_DISPLAY_MPO,
    /// photo internal buffuer \n
    /// decode (SW:PNG/BMP/GIF, HW:JPD) \n
    /// display: photo display showeffect buffer \n
    /// Using HW JPD, these should in the same MIU E_MMSDK_BUF_PHOTO_READ  E_MMSDK_BUF_PHOTO_OUTPUT E_MMSDK_BUF_PHOTO_INTERNAL
    E_MMSDK_BUF_PHOTO_INTERNAL,
    /// @deprecated new:#E_MMSDK_BUF_PHOTO_INTERNAL
    /// memory pool for software decode, co-buffer with E_MMSDK_BUF_PHOTO_INTERNAL inside mmsdk
    E_MMSDK_BUF_PHOTO_MEMORY_POOL,
    /// photo output buf (SW:PNG/BMP/GIF, HW:JPD) \n
    /// Using HW JPD, these should in the same MIU E_MMSDK_BUF_PHOTO_READ  E_MMSDK_BUF_PHOTO_OUTPUT E_MMSDK_BUF_PHOTO_INTERNAL
    E_MMSDK_BUF_PHOTO_OUTPUT,
    /// @deprecated new:#E_MMSDK_BUF_PHOTO_INTERNAL
    /// memory for JPD, co-buffer with E_MMSDK_BUF_PHOTO_INTERNAL inside mmsdk
    E_MMSDK_BUF_PHOTO_PROGRESSIVE,
    /// photo read data buf (SW:PNG/BMP/GIF, HW:JPD) \n
    /// Using HW JPD, these should in the same MIU E_MMSDK_BUF_PHOTO_READ  E_MMSDK_BUF_PHOTO_OUTPUT E_MMSDK_BUF_PHOTO_INTERNAL
    E_MMSDK_BUF_PHOTO_READ,
    /// photo share buf size 0x1000 is needed
    E_MMSDK_BUF_PHOTO_SHAREMEMORY,
    /// video aeon data buf\n
    /// due to vdplayer will also use VDEC_FRAMEBUFFER as data buffer before vdec init\n
    /// if E_MMSDK_BUF_VDEC_FRAMEBUFFER is not in MMAP or use CMA, then E_MMSDK_BUF_VDPLAYER_AEON_DATA 27MB is needed
    E_MMSDK_BUF_VDPLAYER_AEON_DATA,
    /// video bitstream extension buf
    E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO,
    /// vidoe subtitle buf
    E_MMSDK_BUF_VDPLAYER_BITSTREAM_SUBTITLE,
    /// video bitstream buf
    E_MMSDK_BUF_VDPLAYER_BITSTREAM_VIDEO,
    /// vdplayer load code buf (vdplayer run on co-processor will need this)
    E_MMSDK_BUF_VDPLAYER_LOADCODE,
    /// @deprecated new:#E_MMSDK_BUF_VDPLAYER_AEON_DATA \n
    /// video sharememory buf, co-buffer with E_MMSDK_BUF_VDPLAYER_AEON_DATA inside mmsdk
    E_MMSDK_BUF_VDPLAYER_SHAREMEMORY,
    /// vidoe divx font table buf
    E_MMSDK_BUF_SUBTITLE_FONT_TABLE,
    /// vidoe subtitle push buf
    E_MMSDK_BUF_SUBTITLE_PUSH,
    /// video demuxersec buf
    E_MMSDK_BUF_DEMUX_SEC,
    /// vidoe mad decode buf
    E_MMSDK_BUF_MAD_DECODE,
    /// video vdec aeon buf
    E_MMSDK_BUF_VDEC_CPU,
    /// vdec framebuffer, vdplayer will temp use this buffer before init VDEC.\n
    /// if VDEC framebuffer use CMA, mmsdk should release this buffer before vdec init.\n
    /// if VDEC framebuffer use CMA, then E_MMSDK_BUF_VDPLAYER_AEON_DATA 27MB is needed.
    E_MMSDK_BUF_VDEC_FRAMEBUFFER,
    /// sub video frame buf
    E_MMSDK_BUF_VDEC_SUB_FRAMEBUFFER,
    /// video MVC frame buf
    E_MMSDK_BUF_VDEC_FRAMEBUFFER_MVC,
    /// video SD frame buf
    E_MMSDK_BUF_VDEC_FRAMEBUFFER_SD,
    /// video stream buf
    E_MMSDK_BUF_VDEC_STREAMBUFFER,
    /// sub video stream buf
    E_MMSDK_BUF_VDEC_SUB_STREAMBUFFER,
    /// video MVC stream buf
    E_MMSDK_BUF_VDEC_STREAMBUFFER_MVC,
    /// scaler buffer information
    E_MMSDK_BUF_XC_MAIN_FB,
    /// @deprecated co-buffer with E_MMSDK_BUF_VDPLAYER_BITSTREAM_SUBTITLE
    /// physical address for IPC to copy data (IPC cannot used dynamic allocate address)
    E_MMSDK_BUF_PHY_ADDR_FOR_COPY_DATA,
    /// buffer for E_MMAP_ID_VDEC_GN_IAP
    E_MMSDK_BUF_VDEC_GN_IAP,
    /// video teletext push buf
    E_MMSDK_BUF_TTX_VE,
    /// video SD stream buf
    E_MMSDK_BUF_VDEC_STREAMBUFFER_SD,
    /// photo pre-rotate buf
    E_MMSDK_BUF_PHOTO_ROTATE,
    /// video extension frame buf(for EVD performance)
    E_MMSDK_BUF_VDEC_FRAMEBUFFER_EVD_EXT,
    /// VDEC share memory, provide v sync bridge to use at TEE case
    E_MMSDK_BUF_VDEC_SHARE_MEM,
    /// XC DS buffer information, provide v sync bridge to setup DS
    E_MMSDK_BUF_XC_DS,
    /// video aeon data extension buffer(for mov demux index table)\n
    /// if AEON_DATA_EXT is not zero(bigger than 23.5.MB), vdplayer does not use VDEC_FRAMEBUFFER as data extension buffer before vdec init\n
    /// if AEON_DATA_EXT is zero and E_MMSDK_BUF_VDEC_FRAMEBUFFER is not in MMAP or use CMA, then E_MMSDK_BUF_VDPLAYER_AEON_DATA 27MB is needed
    /// if AEON_DATA_EXT is not zero(bigger than 23.5.MB) and E_MMSDK_BUF_VDEC_FRAMEBUFFER is not in MMAP or use CMA, then E_MMSDK_BUF_VDPLAYER_AEON_DATA 3.5MB and is needed
    E_MMSDK_BUF_VDPLAYER_AEON_DATA_EXT,
    ///DV XC buffer information, for Dolby vision HDR
    E_MMSDK_BUF_DV_XC,
    /// video sharememory extend buf
    E_MMSDK_BUF_VDPLAYER_SHAREMEMORY_EXT,
    /// number of buffer
    E_MMSDK_BUF_NUM,
}EN_MMSDK_BUF_ID;

//------------------------------------------------------------------------------
/// Init and get all bufferID structure info form MMAP Layout
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MMAP_Initialize(void);

//------------------------------------------------------------------------------
/// Set buffer info(structure include physical addr, virtual addr, size, MIU_type)
/// (not implement in supernova)
/// @param eBufferID        \b IN: mmap buffer type
/// @param pstMemInfo       \b IN: buffer structure info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MMAP_SetBuffer(const EN_MMSDK_BUF_ID eBufferID, const ST_MMSDK_BUF_INFO* pstMemInfo);

//------------------------------------------------------------------------------
/// Get buffer structure info(structure include physical addr, virtual addr, size, MIU_type, CMA, HeapId, PoolParam)
/// @param eBufferID        \b IN: mmap buffer type
/// @param pstMemInfo       \b OUT: buffer structure info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MMAP_GetBuffer(const EN_MMSDK_BUF_ID eBufferID, ST_MMSDK_BUF_INFO* pstMemInfo);

//------------------------------------------------------------------------------
/// Free buffer when not used, for CMA this is important
/// @param eBufferID        \b IN: mmap buffer type
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MMAP_FreeBuffer(const EN_MMSDK_BUF_ID eBufferID)  __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Get MMAP info
/// wrapper function of MMAPInfo::get_mminfo()
/// @param pInfo            \b IN: mmap info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL  PT_MMAP_GetMminfo(ST_MMSDK_MMAP_INFO* pInfo);

//------------------------------------------------------------------------------
/// Finalize
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MMAP_Finalize(void);

/// @} // end of porting_mmap

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__porting_mmap__
///*** please do not remove change list tag ***///
///***$Change: 1368233 $***///
