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
/// file    apiSystemInit.c
/// @brief  Initialize HW, memory pool & API.
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsMemory.h"
#include "Board.h"
#include "drvMMIO.h"
#include "apiDMX.h"
#include "drvMIU.h"
#include "drvSEM.h"
#include "drvGPIO.h"
#include "drvIIC.h"
#include <string.h>
#include "drvSYS.h"
#include "drvUSB.h"
#include "drvBDMA.h"
#include "drvMVOP.h"
#include "drvSAR.h"
#include "apiDigiTuner.h"
#ifdef FE_AUTO_TEST
#include "UartDebug.h"
#endif

//For IP Authority
#include "Customer_Info.h"
#include "drvIPAUTH.h"

#ifndef DDI_MAIN_TEST
#ifdef ENABLE_PVR_PACKAGE
#include "appDemo_PVR.h"
#endif
#else
#if (DEMO_PVR_TEST == 1)
#include "demo_pvr.h"
#endif
#endif

#include <cyg/fileio/fileio.h>
#include <pkgconf/io_fileio.h>
#include <cyg/io/io.h>
#include <cyg/io/disk.h>
#include <fcntl.h>
#include "MsFS.h"

#ifdef DDI_MAIN_TEST
#include "demo_pool.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

#if (defined(ENABLE_PVR_PACKAGE) || defined(ENABLE_MM_PACKAGE))
#define USBHOST_INIT
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//  For IP Authority
unsigned char Customer_info[49];
#define DEBUG_CUSTOMER_INFO_ENABLE 0
#define DRAM_START 0x80000000

#define MAP_TABLE_FIXED_REC     (3)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _MS_MAP_TABLE
{
    MS_U8       name[32];
    MS_U16      type;
    MS_U32      adr;
    MS_U32      size;
}MS_MAP_TABLE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MS_S32 gs32CachedPoolID    = 0;
MS_S32 gs32NonCachedPoolID = 0;

extern MS_S32  gs32SystemPoolID;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MemoryPool_Info _MemoryPPool_Info[2] =
{
    {-1, CACHED_POOL_BUF_LEN, 0x0, 0x0, E_MSOS_FIFO, "CachedPool"},
    {-1, 0x0, 0x0, 0x0, E_MSOS_FIFO, "NonCachedPool"},
};


#ifdef USBHOST_INIT
extern MS_S32 gs32CachedPoolID;//    = 0;
extern MS_S32 gs32NonCachedPoolID;
#endif

/* !!Memory Map table, please keep first MAP_TABLE_FIXED_REC record to be fixed!! */
static MS_MAP_TABLE _mapTable[] =
{
#ifdef KERNEL_POOL_MEMORY_TYPE
    {"KERNEL_POOL\0", KERNEL_POOL_MEMORY_TYPE, KERNEL_POOL_ADR, KERNEL_POOL_LEN},
#endif
#ifdef EMAC_MEMORY_TYPE
    {"EMAC\0", EMAC_MEMORY_TYPE, EMAC_ADR, EMAC_LEN},
#endif
#ifdef CACHED_POOL_BUF_MEMORY_TYPE
    {"CACHED_POOL_BUF\0", CACHED_POOL_BUF_MEMORY_TYPE, CACHED_POOL_BUF_ADR, CACHED_POOL_BUF_LEN},
#endif
#ifdef NON_CACHED_POOL_BUF_MEMORY_TYPE
    {"NON_CACHED_POOL_BUF\0", NON_CACHED_POOL_BUF_MEMORY_TYPE, NON_CACHED_POOL_BUF_ADR, NON_CACHED_POOL_BUF_LEN},
#endif
#ifdef HW_BUF0_START_TAG_MEMORY_TYPE
    {"HW_BUF0_START_TAG\0", HW_BUF0_START_TAG_MEMORY_TYPE, HW_BUF0_START_TAG_ADR, HW_BUF0_START_TAG_LEN},
#endif
#ifdef HW_BUF1_START_TAG_MEMORY_TYPE
    {"HW_BUF1_START_TAG\0", HW_BUF1_START_TAG_MEMORY_TYPE, HW_BUF1_START_TAG_ADR, HW_BUF1_START_TAG_LEN},
#endif
#ifdef MAD_SE_BUF_MEMORY_TYPE
    {"MAD_SE_BUF\0", MAD_SE_BUF_MEMORY_TYPE, MAD_SE_BUF_ADR, MAD_SE_BUF_LEN},
#endif
#ifdef MAD_DEC_BUF_MEMORY_TYPE
    {"MAD_DEC_BUF\0", MAD_DEC_BUF_MEMORY_TYPE, MAD_DEC_BUF_ADR, MAD_DEC_BUF_LEN},
#endif
#ifdef MIU0_COBUF_START_MEMORY_TYPE
    {"MIU0_COBUF_START\0", MIU0_COBUF_START_MEMORY_TYPE, MIU0_COBUF_START_ADR, MIU0_COBUF_START_LEN},
#endif
#ifdef MIU0_COBUF_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF_END_TAG\0", MIU0_COBUF_END_TAG_MEMORY_TYPE, MIU0_COBUF_END_TAG_ADR, MIU0_COBUF_END_TAG_LEN},
#endif
#ifdef PHOTO_SHARE_MEM_MEMORY_TYPE
    {"PHOTO_SHARE_MEM\0", PHOTO_SHARE_MEM_MEMORY_TYPE, PHOTO_SHARE_MEM_ADR, PHOTO_SHARE_MEM_LEN},
#endif
#ifdef VDPLAYER_TAG_MEMORY_TYPE
    {"VDPLAYER_TAG\0", VDPLAYER_TAG_MEMORY_TYPE, VDPLAYER_TAG_ADR, VDPLAYER_TAG_LEN},
#endif
#ifdef VDPLAYER_DATA_MEMORY_TYPE
    {"VDPLAYER_DATA\0", VDPLAYER_DATA_MEMORY_TYPE, VDPLAYER_DATA_ADR, VDPLAYER_DATA_LEN},
#endif
#ifdef VDPLAYER_SUB_BS_MEMORY_TYPE
    {"VDPLAYER_SUB_BS\0", VDPLAYER_SUB_BS_MEMORY_TYPE, VDPLAYER_SUB_BS_ADR, VDPLAYER_SUB_BS_LEN},
#endif
#ifdef VDPLAYER_BS_MEMORY_TYPE
    {"VDPLAYER_BS\0", VDPLAYER_BS_MEMORY_TYPE, VDPLAYER_BS_ADR, VDPLAYER_BS_LEN},
#endif
#ifdef VDPLAYER_BS_EXT_MEMORY_TYPE
    {"VDPLAYER_BS_EXT\0", VDPLAYER_BS_EXT_MEMORY_TYPE, VDPLAYER_BS_EXT_ADR, VDPLAYER_BS_EXT_LEN},
#endif
#ifdef VDPLAYER_SUB_MEMORY_TYPE
    {"VDPLAYER_SUB\0", VDPLAYER_SUB_MEMORY_TYPE, VDPLAYER_SUB_ADR, VDPLAYER_SUB_LEN},
#endif
#ifdef VDPLAYER_DMX_SEC_MEMORY_TYPE
    {"VDPLAYER_DMX_SEC\0", VDPLAYER_DMX_SEC_MEMORY_TYPE, VDPLAYER_DMX_SEC_ADR, VDPLAYER_DMX_SEC_LEN},
#endif
#ifdef MIU0_COBUF_L1_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF_L1_END_TAG\0", MIU0_COBUF_L1_END_TAG_MEMORY_TYPE, MIU0_COBUF_L1_END_TAG_ADR, MIU0_COBUF_L1_END_TAG_LEN},
#endif
#ifdef JPD_READ_BUF_MEMORY_TYPE
    {"JPD_READ_BUF\0", JPD_READ_BUF_MEMORY_TYPE, JPD_READ_BUF_ADR, JPD_READ_BUF_LEN},
#endif
#ifdef JPD_INTER_BUF_MEMORY_TYPE
    {"JPD_INTER_BUF\0", JPD_INTER_BUF_MEMORY_TYPE, JPD_INTER_BUF_ADR, JPD_INTER_BUF_LEN},
#endif
#ifdef JPD_OUT_MEMORY_TYPE
    {"JPD_OUT\0", JPD_OUT_MEMORY_TYPE, JPD_OUT_ADR, JPD_OUT_LEN},
#endif
#ifdef MIU0_COBUF_L2_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF_L2_END_TAG\0", MIU0_COBUF_L2_END_TAG_MEMORY_TYPE, MIU0_COBUF_L2_END_TAG_ADR, MIU0_COBUF_L2_END_TAG_LEN},
#endif
#ifdef MIU0_COBUF2_START_MEMORY_TYPE
    {"MIU0_COBUF2_START\0", MIU0_COBUF2_START_MEMORY_TYPE, MIU0_COBUF2_START_ADR, MIU0_COBUF2_START_LEN},
#endif
#ifdef MIU0_COBUF2_L1_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF2_L1_END_TAG\0", MIU0_COBUF2_L1_END_TAG_MEMORY_TYPE, MIU0_COBUF2_L1_END_TAG_ADR, MIU0_COBUF2_L1_END_TAG_LEN},
#endif
#ifdef MIU0_COBUF2_L2_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF2_L2_END_TAG\0", MIU0_COBUF2_L2_END_TAG_MEMORY_TYPE, MIU0_COBUF2_L2_END_TAG_ADR, MIU0_COBUF2_L2_END_TAG_LEN},
#endif
#ifdef MIU0_COBUF2_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF2_END_TAG\0", MIU0_COBUF2_END_TAG_MEMORY_TYPE, MIU0_COBUF2_END_TAG_ADR, MIU0_COBUF2_END_TAG_LEN},
#endif
#ifdef GOP_STILL_IMG_ZAPPING_MEMORY_TYPE
    {"GOP_STILL_IMG_ZAPPING\0", GOP_STILL_IMG_ZAPPING_MEMORY_TYPE, GOP_STILL_IMG_ZAPPING_ADR, GOP_STILL_IMG_ZAPPING_LEN},
#endif
#ifdef PVR_BASE_TAG_MEMORY_TYPE
    {"PVR_BASE_TAG\0", PVR_BASE_TAG_MEMORY_TYPE, PVR_BASE_TAG_ADR, PVR_BASE_TAG_LEN},
#endif
#ifdef PVR_DOWNLOAD_MEMORY_TYPE
    {"PVR_DOWNLOAD\0", PVR_DOWNLOAD_MEMORY_TYPE, PVR_DOWNLOAD_ADR, PVR_DOWNLOAD_LEN},
#endif
#ifdef PVR_UPLOAD_MEMORY_TYPE
    {"PVR_UPLOAD\0", PVR_UPLOAD_MEMORY_TYPE, PVR_UPLOAD_ADR, PVR_UPLOAD_LEN},
#endif
#ifdef PVR_AUDIO_BUF_MEMORY_TYPE
    {"PVR_AUDIO_BUF\0", PVR_AUDIO_BUF_MEMORY_TYPE, PVR_AUDIO_BUF_ADR, PVR_AUDIO_BUF_LEN},
#endif
#ifdef PVR_MN_VIDEO_BUF_MEMORY_TYPE
    {"PVR_MN_VIDEO_BUF\0", PVR_MN_VIDEO_BUF_MEMORY_TYPE, PVR_MN_VIDEO_BUF_ADR, PVR_MN_VIDEO_BUF_LEN},
#endif
#ifdef PVR_MN_AUDIO_BUF_MEMORY_TYPE
    {"PVR_MN_AUDIO_BUF\0", PVR_MN_AUDIO_BUF_MEMORY_TYPE, PVR_MN_AUDIO_BUF_ADR, PVR_MN_AUDIO_BUF_LEN},
#endif
#ifdef PVR_TSR_VIDEO_BUF_MEMORY_TYPE
    {"PVR_TSR_VIDEO_BUF\0", PVR_TSR_VIDEO_BUF_MEMORY_TYPE, PVR_TSR_VIDEO_BUF_ADR, PVR_TSR_VIDEO_BUF_LEN},
#endif
#ifdef PVR_TSR_AUDIO_BUF_MEMORY_TYPE
    {"PVR_TSR_AUDIO_BUF\0", PVR_TSR_AUDIO_BUF_MEMORY_TYPE, PVR_TSR_AUDIO_BUF_ADR, PVR_TSR_AUDIO_BUF_LEN},
#endif
#ifdef MIU0_COBUF_L3_END_TAG_MEMORY_TYPE
    {"MIU0_COBUF_L3_END_TAG\0", MIU0_COBUF_L3_END_TAG_MEMORY_TYPE, MIU0_COBUF_L3_END_TAG_ADR, MIU0_COBUF_L3_END_TAG_LEN},
#endif
#ifdef VE_FRAME_BUF_MEMORY_TYPE
    {"VE_FRAME_BUF\0", VE_FRAME_BUF_MEMORY_TYPE, VE_FRAME_BUF_ADR, VE_FRAME_BUF_LEN},
#endif
#ifdef SC0_MAIN_FB_MEMORY_TYPE
    {"SC0_MAIN_FB\0", SC0_MAIN_FB_MEMORY_TYPE, SC0_MAIN_FB_ADR, SC0_MAIN_FB_LEN},
#endif
#ifdef SC0_SUB_FB_MEMORY_TYPE
    {"SC0_SUB_FB\0", SC0_SUB_FB_MEMORY_TYPE, SC0_SUB_FB_ADR, SC0_SUB_FB_LEN},
#endif
#ifdef GOP_REG_DMA_BASE_MEMORY_TYPE
    {"GOP_REG_DMA_BASE\0", GOP_REG_DMA_BASE_MEMORY_TYPE, GOP_REG_DMA_BASE_ADR, GOP_REG_DMA_BASE_LEN},
#endif
#ifdef GOP_GWIN_RB_MEMORY_TYPE
    {"GOP_GWIN_RB\0", GOP_GWIN_RB_MEMORY_TYPE, GOP_GWIN_RB_ADR, GOP_GWIN_RB_LEN},
#endif
#ifdef GOP_VIDEO_MEM_MEMORY_TYPE
    {"GOP_VIDEO_MEM\0", GOP_VIDEO_MEM_MEMORY_TYPE, GOP_VIDEO_MEM_ADR, GOP_VIDEO_MEM_LEN},
#endif
#ifdef TSP_FQ_BUF_MEMORY_TYPE
    {"TSP_FQ_BUF\0", TSP_FQ_BUF_MEMORY_TYPE, TSP_FQ_BUF_ADR, TSP_FQ_BUF_LEN},
#endif
#ifdef TSP_FW_BUF_MEMORY_TYPE
    {"TSP_FW_BUF\0", TSP_FW_BUF_MEMORY_TYPE, TSP_FW_BUF_ADR, TSP_FW_BUF_LEN},
#endif
#ifdef TSP_VQ_BUF_MEMORY_TYPE
    {"TSP_VQ_BUF\0", TSP_VQ_BUF_MEMORY_TYPE, TSP_VQ_BUF_ADR, TSP_VQ_BUF_LEN},
#endif
#ifdef PM51_MIU_ENTER_SR_MEMORY_TYPE
    {"PM51_MIU_ENTER_SR\0", PM51_MIU_ENTER_SR_MEMORY_TYPE, PM51_MIU_ENTER_SR_ADR, PM51_MIU_ENTER_SR_LEN},
#endif
#ifdef PM51_MIU_EXIT_SR_MEMORY_TYPE
    {"PM51_MIU_EXIT_SR\0", PM51_MIU_EXIT_SR_MEMORY_TYPE, PM51_MIU_EXIT_SR_ADR, PM51_MIU_EXIT_SR_LEN},
#endif
#ifdef PM51_HWIP_CLOSE_MEMORY_TYPE
    {"PM51_HWIP_CLOSE\0", PM51_HWIP_CLOSE_MEMORY_TYPE, PM51_HWIP_CLOSE_ADR, PM51_HWIP_CLOSE_LEN},
#endif
#ifdef PM51_HWIP_OPEN_MEMORY_TYPE
    {"PM51_HWIP_OPEN\0", PM51_HWIP_OPEN_MEMORY_TYPE, PM51_HWIP_OPEN_ADR, PM51_HWIP_OPEN_LEN},
#endif
#ifdef PM51_LOADER_MEMORY_TYPE
    {"PM51_LOADER\0", PM51_LOADER_MEMORY_TYPE, PM51_LOADER_ADR, PM51_LOADER_LEN},
#endif
#ifdef PM51_LITE_MEMORY_TYPE
    {"PM51_LITE\0", PM51_LITE_MEMORY_TYPE, PM51_LITE_ADR, PM51_LITE_LEN},
#endif
#ifdef PM51_STR_MEMORY_TYPE
    {"PM51_STR\0", PM51_STR_MEMORY_TYPE, PM51_STR_ADR, PM51_STR_LEN},
#endif
#ifdef PM51_MEM_MEMORY_TYPE
    {"PM51_MEM\0", PM51_MEM_MEMORY_TYPE, PM51_MEM_ADR, PM51_MEM_LEN},
#endif
#ifdef PM51_VAR_MEM_MEMORY_TYPE
    {"PM51_VAR_MEM\0", PM51_VAR_MEM_MEMORY_TYPE, PM51_VAR_MEM_ADR, PM51_VAR_MEM_LEN},
#endif
#ifdef KERNEL_POOL1_MEMORY_TYPE
    {"KERNEL_POOL1\0", KERNEL_POOL1_MEMORY_TYPE, KERNEL_POOL1_ADR, KERNEL_POOL1_LEN},
#endif
#ifdef VDEC_MEM_START_TAG_MEMORY_TYPE
    {"VDEC_MEM_START_TAG\0", VDEC_MEM_START_TAG_MEMORY_TYPE, VDEC_MEM_START_TAG_ADR, VDEC_MEM_START_TAG_LEN},
#endif
#ifdef VDEC_AEON_MEMORY_TYPE
    {"VDEC_AEON\0", VDEC_AEON_MEMORY_TYPE, VDEC_AEON_ADR, VDEC_AEON_LEN},
#endif
#ifdef VDEC_FRAME_BUF_MEMORY_TYPE
    {"VDEC_FRAME_BUF\0", VDEC_FRAME_BUF_MEMORY_TYPE, VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_LEN},
#endif
#ifdef VDEC_BIT_STREAM_MEMORY_TYPE
    {"VDEC_BIT_STREAM\0", VDEC_BIT_STREAM_MEMORY_TYPE, VDEC_BIT_STREAM_ADR, VDEC_BIT_STREAM_LEN},
#endif
#ifdef SC0_MENULOAD_BUF_MEMORY_TYPE
    {"SC0_MENULOAD_BUF\0", SC0_MENULOAD_BUF_MEMORY_TYPE, SC0_MENULOAD_BUF_ADR, SC0_MENULOAD_BUF_LEN},
#endif
#ifdef INJECTION_BUF_MEMORY_TYPE
    {"INJECTION_BUF\0", INJECTION_BUF_MEMORY_TYPE, INJECTION_BUF_ADR, INJECTION_BUF_LEN},
#endif
#ifdef SDRAM_ADR_FONT_MEMORY_TYPE
    {"SDRAM_ADR_FONT\0", SDRAM_ADR_FONT_MEMORY_TYPE, SDRAM_ADR_FONT_ADR, SDRAM_ADR_FONT_LEN},
#endif
#ifdef SDRAM_ADR_BMP_MEMORY_TYPE
    {"SDRAM_ADR_BMP\0", SDRAM_ADR_BMP_MEMORY_TYPE, SDRAM_ADR_BMP_ADR, SDRAM_ADR_BMP_LEN},
#endif
#ifdef SECTION_BUF_MEMORY_TYPE
    {"SECTION_BUF\0", SECTION_BUF_MEMORY_TYPE, SECTION_BUF_ADR, SECTION_BUF_LEN},
#endif
#ifdef HW_BUF0_END_TAG_MEMORY_TYPE
    {"HW_BUF0_END_TAG\0", HW_BUF0_END_TAG_MEMORY_TYPE, HW_BUF0_END_TAG_ADR, HW_BUF0_END_TAG_LEN},
#endif
#ifdef HW_BUF1_END_TAG_MEMORY_TYPE
    {"HW_BUF1_END_TAG\0", HW_BUF1_END_TAG_MEMORY_TYPE, HW_BUF1_END_TAG_ADR, HW_BUF1_END_TAG_LEN},
#endif
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

extern void mdrv_gpio_init( void );
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#ifdef USBHOST_INIT
void*  AllocCachedMem(int size)
{
    return MsOS_AllocateMemory(size, gs32CachedPoolID);
}
void   FreeCachedMem(void* pBuf)
{
    if (FALSE == MsOS_FreeMemory(pBuf, gs32CachedPoolID))
        printf("!!!! ERROR: FreeCachedMem 0x%lx fail \n",  (MS_U32)pBuf);
}
void*  AllocNoncachedMem(int size)
{
    return MsOS_AllocateMemory(size, gs32NonCachedPoolID);
}
void   FreeNoncachedMem(void* pBuf)
{
    if (FALSE ==  MsOS_FreeMemory(pBuf, gs32NonCachedPoolID))
        printf("!!!! ERROR: FreeNoncachedMem 0x%lx fail \n",  (MS_U32)pBuf);
}
MS_U32 MemVA2PA(MS_U32 addr)
{
    return MsOS_VA2PA(addr);
}
MS_U32 MemPA2VA(MS_U32 addr, int bCached)
{
    if (bCached)
        return MsOS_PA2KSEG0(addr);
    else
        return MsOS_PA2KSEG1(addr);
}
void*  MemCached2Noncached(MS_U32 addr)
{
    void *padr = (void *)MsOS_PA2KSEG1(MsOS_VA2PA((U32)addr));
    //diag_printf("[MemCached2Noncached] padr=%08lX,%08lX\n",addr,padr);

    return padr;
}
void*  MemNonCached2Cached(MS_U32 addr)
{

    void *padr = (void *)MsOS_PA2KSEG0(MsOS_VA2PA((U32)addr));
    //diag_printf("[MemNonCached2Cached] padr=%08lX,%08lX\n",addr,padr);

    return padr;
}

static MS_U8 get_dev_partitions(const MS_U8 * dev_path) // ex dev_path: /dev/sdx/0
{
    int result;
    cyg_disk_info_t info;
    cyg_uint32 len = 1;
    char buf = 1;
    int i;
    MS_U8 partition_count = 0;
    cyg_io_handle_t device;

    result = cyg_io_lookup( dev_path, &device );

    result = cyg_io_set_config(device, CYG_IO_SET_CONFIG_DISK_MOUNT, &buf, &len);

    len = sizeof(cyg_disk_info_t);
    result = cyg_io_get_config( device, CYG_IO_GET_CONFIG_DISK_INFO, &info, &len );

    for(i=0;i<8;i++) // one device at most have 8 partition, 24 partition at most is allowed in STB.
    {
        if (info.partitions[i].type != 0) // fat32:0xb ntfs:0x7
        {
            ++partition_count;
        }
    }
    return partition_count;
}


static void  _USB_Callback_Test(MS_U8 u8Event, MS_U32 u32Para,char *s)
{
#define MOUNT_FS_LEN 8
#define MAX_PARTITIONS 8
#define MAX_STR_LENGTH 30
    // ex. s = "/dev/sdx/"
    MS_U8 pU8whole_disk_path[MAX_STR_LENGTH] = {0}; // /dev/sdx/0
    MS_U8 pre_mount_path[MAX_STR_LENGTH] = {0};  // /sdx
    MS_U8 inputStr[MAX_STR_LENGTH] = {0}; // /dev/sdx/
    MS_U8 mountfs[][MOUNT_FS_LEN] = {"fatfs","ntfs","vfat","ntfs3g"};
    MS_U8 *delim = "/";
    MS_U8 *pch = NULL;
    MS_S32 mountnum = sizeof(mountfs)/MOUNT_FS_LEN;
    MS_S32 ret = -1;
    MS_U32 i = 0;
    MS_U32 j = 0;
    MS_U8 u8num_partitions = 0;

    if (s == NULL || strlen(s) < 6) //  s = "/dev/sdx/", at least should be "/dev/x"
    {
        printf("[%s][%d]invalid mount device:%s\n", (s == NULL) ? "NULL" : s);
        return ;
    }

    // copy input string for strtok usage
    strcpy(inputStr, s);
    pch = strtok(inputStr, delim); // now pch = "dev"

    if (pch == NULL || strcmp(pch, "dev") != 0)
    {
        printf("[%s][%d]invalid mount device path:%s\n", (s == NULL) ? "NULL" : s);
        return ;
    }

    pch = strtok (NULL, delim); // now pch = "sdx"
    if (pch == NULL || strlen(pch) == 0)
    {
        printf("[%s][%d]invalid  mount path:%s\n", (s == NULL) ? "NULL" : s);
        return ;
    }

    strcpy(pU8whole_disk_path, s);
    strcat(pU8whole_disk_path, "0"); // /dev/sdx/0

    if (u8Event == USB_PLUG_IN)
    {
        u8num_partitions = get_dev_partitions((const MS_U8 *)pU8whole_disk_path);
    }

    pre_mount_path[0] = '/';
    pre_mount_path[1] = 'r';
    pre_mount_path[2] = 'o';
    pre_mount_path[3] = 'o';
    pre_mount_path[4] = 't';
    pre_mount_path[5] = '/';
    strcat(pre_mount_path, pch); // pre_mount_path = "/sdx"

    printf("USB Call Back Event %.2x Parameter %x %s \n",u8Event,u32Para,s);
    printf("USB Sector Read/Write Speed Test \n");

    switch(u8Event)
    {
        case USB_PLUG_IN:
            for(i=u32Para; i < u8num_partitions+u32Para; i++)
            {
                for(j=0;j<mountnum;j++)
                {
                    MS_U8 partition_path[MAX_STR_LENGTH] = {0};
                    MS_U8 mount_path[MAX_STR_LENGTH] = {0};
                    sprintf(partition_path,"%s%d",s,i);// partition_path = "/dev/sdx/1"
                    sprintf(mount_path,"%s%d",pre_mount_path,i);// mount_path = "/sdx1"
                    ret = mount( partition_path, mount_path, mountfs[j]);
                    if(ret==0)
                    {
                        printf("mount \"%s\" to \"%s\" successfully, FS:%s\n", partition_path, mount_path, mountfs[j]);
                        break;
                    }
                }
            }
            if(ret!=0)
            {
                printf("mount failed\n");
            }
            break;
        case USB_PLUG_OUT:
            printf("Uplug USB\n");
            for(i=u32Para; i < MAX_PARTITIONS+u32Para; i++) // if u32Para = 1, umount /sdx1~/sdx8
            {
                MS_U8 umount_path[MAX_STR_LENGTH] = {0};
                sprintf(umount_path,"%s%d",pre_mount_path,i);// umount_path = "/sdx1"
#if 1//TODO  mark temp for chal build pass, should fix after checking chal compiler option
#ifndef DDI_MAIN_TEST
#ifdef ENABLE_PVR_PACKAGE
//                if(appDemo_PVR_USB_UnplugEvent(umount_path)==FALSE)
				{
#endif
#else
#if (DEMO_PVR_TEST == 1)
                if(Demo_PVR_USB_UnplugEvent(umount_path)==FALSE)
                {

#endif
#endif
                    if((ret = umount(umount_path))==0)
                    {
                        printf("Umount \"%s\" successfully!\n", umount_path);
						printf("\n""\033[1;31m""[FUN][%s()@%04d] skip appDemo_PVR_USB_UnplugEvent for chal on[%s] ""\033[m""\n", __FUNCTION__, __LINE__, umount_path);
					}
#ifndef DDI_MAIN_TEST
#ifdef ENABLE_PVR_PACKAGE
                }
#endif
#else
#if (DEMO_PVR_TEST == 1)
                }
#endif
#endif
#endif
            }
            break;
        default:

            break;
    }
}
#endif



void SysInit_InitPower(void)
{


}


static MS_BOOL _MApi_SystemInit_InitHW(void *pFirstAvailableMemory);


//-------------------------------------------------------------------------------------------------
/// Create memory pool for cached, non-cached and display memory pool.
/// User can change the definition in apiSystemInit.h.
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MApi_SystemInit_CreateMemoryPool(void *pFirstAvailableMemory)
{
    _MemoryPPool_Info[0].uPoolSize = CACHED_POOL_BUF_LEN;
    _MemoryPPool_Info[0].u32Addr = MS_VA2PA((MS_U32)pFirstAvailableMemory);
    _MemoryPPool_Info[0].u32Addr = MEMALIGN(64,_MemoryPPool_Info[0].u32Addr);
    _MemoryPPool_Info[1].u32Addr = CACHED_POOL_BUF_LEN + _MemoryPPool_Info[0].u32Addr;
    _MemoryPPool_Info[1].u32Addr = MEMALIGN(64,_MemoryPPool_Info[1].u32Addr);
    // need to be alligned with largest cache line size (64 byte)
    // or the data in the boundary between cache and non-cache pool may be mis-cached
    // if not alligned, the data at the start of non-cache pool may be treat as cached data
    // which may causes unpredictable errors
    _MemoryPPool_Info[1].uPoolSize = HW_BUF0_START_TAG_ADR - _MemoryPPool_Info[1].u32Addr;
    if (_MemoryPPool_Info[1].u32Addr >= HW_BUF0_START_TAG_ADR)
    {
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Create Cached Pool
    _MemoryPPool_Info[0].iId = MsOS_CreateMemoryPool( _MemoryPPool_Info[0].uPoolSize,
                                                      _MemoryPPool_Info[0].u32MinAllocation,
                                                      (void*)MS_PA2KSEG0(_MemoryPPool_Info[0].u32Addr),
                                                      _MemoryPPool_Info[0].eAttribute,
                                                      _MemoryPPool_Info[0].szName);
    MS_ASSERT( _MemoryPPool_Info[0].iId >= 0 );

    if(_MemoryPPool_Info[0].iId  < 0)
    {
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    }

    _MemoryPPool_Info[1].iId = MsOS_CreateMemoryPool( _MemoryPPool_Info[1].uPoolSize,
                                                      _MemoryPPool_Info[1].u32MinAllocation,
                                                      (void*)MS_PA2KSEG1(_MemoryPPool_Info[1].u32Addr),
                                                      _MemoryPPool_Info[1].eAttribute,
                                                      _MemoryPPool_Info[1].szName);
    MS_ASSERT( _MemoryPPool_Info[1].iId >= 0 );
    if(_MemoryPPool_Info[1].iId  < 0)
    {
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    }

    //Set Cached, NonCached & Display Pool ID
    gs32CachedPoolID    = _MemoryPPool_Info[0].iId;
    gs32NonCachedPoolID = _MemoryPPool_Info[1].iId;
    gs32SystemPoolID    = gs32CachedPoolID;

#ifdef DDI_MAIN_TEST
    Demo_Pool_SetSystemPoolID(E_DDI_POOL_SYS_CACHE,gs32CachedPoolID);
    Demo_Pool_SetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,gs32NonCachedPoolID);
#endif


    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Initialize MsOS & all HW & drivers
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MApi_SystemInit_InitHW(void *pFirstAvailableMemory)
{


    printf("MDrv_MIOMap_Init\n");
    MDrv_MMIO_Init(); // I/O remap

    MApi_eCos_GetCpuFreq();
    MDrv_SEM_Init();
    MDrv_MIU_SetIOMapBase();

    printf("MsOS_Init\n");
    if(FALSE == MsOS_Init())
    {
        return FALSE;
    }


    //Init System Information for MDrv_SYS_GetInfo
    MDrv_SYS_Init();


    printf("_MApi_SystemInit_CreateMemoryPool\n");
    _MApi_SystemInit_CreateMemoryPool(pFirstAvailableMemory);


    printf("mdrv_gpio_init\n");
    mdrv_gpio_init();

    printf("MDrv_IIC_Init\n");
    MDrv_IIC_Init(FRONTEND_TUNER_PORT);

    if (TRUE != MDrv_BDMA_Init(0))
    {
        printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
    }

    //disable vsync protection before VDEC init
    MDrv_MVOP_Init();

    MDrv_SAR_Kpd_Init(); // for demod usage


    SysInit_InitPower();


    printf("_MApi_SystemInit_InitHW finish\n");
    return TRUE;
}


static void _MApi_Init_CustomerInfo(void)
{
    unsigned char * pu8Temp;
    MS_U8 i,u8Data;
    MS_U8 u8ASCII_Mapping[16] = {'0','1','2','3','4','5','6','7','8','9','A',
        'B','C','D','E','F'};
    MS_U32 dolbycontrol=0, chiptype=0, reverse=0;

#if (DEBUG_CUSTOMER_INFO_ENABLE)
        printf("****** Customer_info:Before ******");
        for (i=0;i<48;i++)
        {
            if (i%8 ==0)
                printf("\n");
            printf("0x%bx,",Customer_info[i]);

        }

        printf("\n abcdef => 0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,",'a','b','c','d','e','f');
        printf("\n ABCDEF => 0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,",'A','B','C','D','E','F');
        printf("\n\n");
#endif

    pu8Temp = &CID_Buf[6];//(Byte6,7: Customer ID)(Byte8,9: Model ID)(Byte1011: Chip ID)

    //<1>.To Prepare the Customer-ID + Model-ID + Chip-ID
          for (i=0;i<3;i++)
          { //Get hight byte
            u8Data = ((*(pu8Temp+1))>> 4);
            Customer_info[0+i*4] = u8ASCII_Mapping[u8Data];
            u8Data = (*(pu8Temp+1) & 0x0F);
            Customer_info[1+i*4] =  u8ASCII_Mapping[u8Data];
            //Get Low byte
            u8Data = ((*(pu8Temp))>>4) ;
            Customer_info[2+i*4] = u8ASCII_Mapping[u8Data];
            u8Data = (*(pu8Temp) & 0x0F);
            Customer_info[3+i*4] = u8ASCII_Mapping[u8Data];
            pu8Temp +=2;
          }

          //support hash key v2.0
          dolbycontrol = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_CONTROL_BIT);
          chiptype = MDrv_SYS_GetChipType();
          reverse = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_REVERSE_BIT);
          if(dolbycontrol == 1 && chiptype == 1 && reverse == 0)
          {
              printf("\033[1;32m *** hash key v2.0, use Customer_Info_V2.h to replace Customer_Info.h *** \033[0m \r\n");
              //Get hight byte
              u8Data = ((*(pu8Temp + 1)) >> 4);
              Customer_info[0 + 12] = u8ASCII_Mapping[u8Data];
              u8Data = (*(pu8Temp + 1) & 0x0F);
              Customer_info[1 + 12] =  u8ASCII_Mapping[u8Data];
              //Get Low byte
              u8Data = ((*(pu8Temp)) >> 4) ;
              Customer_info[2 + 12] = u8ASCII_Mapping[u8Data];
              u8Data = (*(pu8Temp) & 0x0F);
              Customer_info[3 + 12] = u8ASCII_Mapping[u8Data];
          }
          else
          {
              printf("\033[1;32m *** hash key v1.0, use Customer_Info.h directly *** \033[0m \r\n");
              for (i=0;i<4;i++)
                Customer_info[12+i] = 0x30;
          }

    //<2>.To prepare the Customer IP
          pu8Temp = &IP_Cntrol_Mapping_1[0];
          memcpy(&Customer_info[16], pu8Temp, 8);
          pu8Temp = &IP_Cntrol_Mapping_2[0];
          memcpy(&Customer_info[16+8], pu8Temp, 8);
          pu8Temp = &IP_Cntrol_Mapping_3[0];
          memcpy(&Customer_info[16+16], pu8Temp, 8);
          pu8Temp = &IP_Cntrol_Mapping_4[0];
          memcpy(&Customer_info[16+24], pu8Temp, 8);

#if (DEBUG_CUSTOMER_INFO_ENABLE)
        printf("****** Customer_info ******");
        for (i=0;i<48;i++)
        {
            if (i%8 ==0)
                printf("\n");
            printf("0x%bx,",Customer_info[i]);

        }
#endif
        Customer_info[48] = 0;  //Important, string end symbol
}

static void _MemMap_Sort(void)
{
    MS_U8 i=0, j=0;
    MS_U16 record_size=0, table_size=0;
    MS_MAP_TABLE temp_record;

    record_size = sizeof(MS_MAP_TABLE);
    table_size  = sizeof(_mapTable)/record_size;

    for (i=0; i<table_size-1; i++)
    {
        for (j=MAP_TABLE_FIXED_REC; j<table_size-i-1; j++)
        {
            if (_mapTable[j].adr > _mapTable[j+1].adr)
            {
                memcpy((MS_S8*)&temp_record, (MS_S8*)&_mapTable[j], record_size);
                memcpy((MS_S8*)&_mapTable[j], (MS_S8*)&_mapTable[j+1], record_size);
                memcpy((MS_S8*)&_mapTable[j+1], (MS_S8*)&temp_record, record_size);
            }
        }
    }
}

static void _SysInit_MemPrint(void *pFirstAvailableMemory)
{
    MS_U8 i=0, j=0;
    MS_U8 miu0LayerCnt = MIU0_LAYER_COUNT;
#if (ENABLE_MIU_1==1)
    MS_U8 miu1LayerCnt = MIU1_LAYER_COUNT;
#endif
    _MemMap_Sort();

    printf("===================================================================================== \n");
    printf("[MAPI_MEMINFO] MIU: System Memory Usage [%d MB]", MIU_DRAM_LEN0>>20);
#if (ENABLE_MIU_1==1)
    printf("[%d MB]", MIU_DRAM_LEN1>>20);
    printf(" -- Interval=%d MB", MIU_INTERVAL>>20);
#endif
    printf("\n[MAPI_MEMINFO] pFirstAvailableMemory=0x%08lx\n", MS_VA2PA((MS_U32)pFirstAvailableMemory));
    printf("[MAPI_MEMINFO] ========== MIU 0 ============ START         END           SIZE\n");
#if 1
    printf("[MAPI_MEMINFO] %-28s  0x%08lx    0x%08lx    0x%08lx\n", _mapTable[0].name, _mapTable[0].adr, _mapTable[0].adr + MS_VA2PA((MS_U32)pFirstAvailableMemory), MS_VA2PA((MS_U32)pFirstAvailableMemory));
    printf("[MAPI_MEMINFO] %-28s  0x%08lx    0x%08lx    0x%08lx\n", _mapTable[1].name, _MemoryPPool_Info[0].u32Addr, _MemoryPPool_Info[0].u32Addr + _MemoryPPool_Info[0].uPoolSize, _MemoryPPool_Info[0].uPoolSize);
    printf("[MAPI_MEMINFO] %-28s  0x%08lx    0x%08lx    0x%08lx\n", _mapTable[2].name, _MemoryPPool_Info[1].u32Addr, _MemoryPPool_Info[1].u32Addr + _MemoryPPool_Info[1].uPoolSize, _MemoryPPool_Info[1].uPoolSize);

#else
    for (i=0; i<MAP_TABLE_FIXED_REC; i++)
    {
        {
            printf("[MAPI_MEMINFO] %-28s  0x%08lx    0x%08lx    0x%08lx\n", _mapTable[i].name, _mapTable[i].adr, _mapTable[i].adr + _mapTable[i].size, _mapTable[i].size);
        }
    }
#endif
    printf("[MAPI_MEMINFO] \n");
    printf("[MAPI_MEMINFO] Memory Layout:\n");
    for (j=0; j<miu0LayerCnt; j++)
    {
        printf("[MAPI_MEMINFO] ********** Layer %d ********** START         END           SIZE\n", j);
        for (i=MAP_TABLE_FIXED_REC; i<sizeof(_mapTable)/sizeof(_mapTable[0]); i++)
        {
            if ((_mapTable[i].type>>CO_BUF_OFFSET) == j && (_mapTable[i].type&0x1) == MIU0)
                printf("[MAPI_MEMINFO] %-28s  0x%08lx    0x%08lx    0x%08lx\n", _mapTable[i].name, _mapTable[i].adr, _mapTable[i].adr + _mapTable[i].size, _mapTable[i].size);
        }
    }

#if (ENABLE_MIU_1==1)
    printf("[MAPI_MEMINFO]\n");
    printf("[MAPI_MEMINFO] ========== MIU 1 ============ \n");
    for (j=0; j<miu1LayerCnt; j++)
    {
        printf("[MAPI_MEMINFO] ********** Layer %d ********** START         END           SIZE\n", j);
        for (i=0; i<sizeof(_mapTable)/sizeof(_mapTable[0]); i++)
        {
            if ((_mapTable[i].type>>CO_BUF_OFFSET) == j && (_mapTable[i].type&0x1) == MIU1)
                printf("[MAPI_MEMINFO] %-28s  0x%08lx    0x%08lx    0x%08lx\n", _mapTable[i].name, _mapTable[i].adr, _mapTable[i].adr + _mapTable[i].size, _mapTable[i].size);
        }
    }
#endif

    printf("\n");
    printf("===================================================================================== \n");
    printf("\n");
}


static void MApi_Set_SecurityAuthentication(void)
{
    _MApi_Init_CustomerInfo();

    MApi_AUTH_Process(Customer_info, Customer_hash);
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Initialize HW, memory pool & API.
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_SystemInit_Init(void *pFirstAvailableMemory)
{
    printf("Hardware Initialize...\n");
    if(!_MApi_SystemInit_InitHW(pFirstAvailableMemory))
    {
        return FALSE;
    }

    _SysInit_MemPrint(pFirstAvailableMemory);


    //Ip Security Authentication
    MApi_Set_SecurityAuthentication();

#ifdef FE_AUTO_TEST
    MDrv_UART_DebugInit();
#endif
    //Set Frontend Mempool
    MApi_DigiTuner_Set_MemPool(E_DDI_FE_POOL_SYS_CACHE, gs32CachedPoolID);
    return TRUE;
}

MS_BOOL MApi_SystemDeviceHost_Init(void)
{

#ifdef USBHOST_INIT
    static MS_BOOL _bRet = FALSE;
    if (_bRet == TRUE)
    {
        printf("USB already init \n");
        return TRUE;
    }

    printf("USB init \n");
    MDrv_USB_RegisterCallBack(_USB_Callback_Test);

    printf("MDrv_Usb_Init\n");
    MDrv_Usb_Init(
         AllocCachedMem,
         FreeCachedMem,
         AllocNoncachedMem,
         FreeNoncachedMem,
         MemVA2PA,
         MemPA2VA,
         MemCached2Noncached,
         MemNonCached2Cached
    );

    MDrv_USB_Port_Init(0);
    printf("MDrv_Usb_Done .................................................\n");
    _bRet = TRUE;
#endif
    return TRUE;
}

