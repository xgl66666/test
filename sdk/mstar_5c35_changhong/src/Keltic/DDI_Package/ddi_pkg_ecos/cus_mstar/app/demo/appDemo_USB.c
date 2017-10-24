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
/// @file   appDemo_USB.c
/// @author MStar Semiconductor Inc.
/// @brief A sample code for illustrating how to control USB functions 
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_USB.c
   @brief The sample code for the USB_XXX functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include <cyg/io/disk.h>
#include "appDemo_USB.h"

//------------------------------------------------------------------------------
/// @brief Umount USB or Mass Storage.
/// @param[in] umountPath - umount device path. 
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b usb_umount \n
///
/// Sample Command: \b usb_umount /sdx1 => umount /sdx1 \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_USB_Umount(MS_U8 *umountPath)
{
    MS_S32 ret = -1;
    ret = umount(umountPath);
    printf("%s: umount %s\n", (ret == 0) ? "ok!" : "Fail!", umountPath);
    return (ret == 0) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
/// @brief Mount USB or Mass Storage device to mounting directory. 
/// @param[in] devPath - select device to be fdisk. 
/// @param[in] mountPath - indicate to mount path  
/// @param[in] mountFS - mount file system. ex. fatfs/ntfs. 
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b usb_mount \n
///
/// Sample Command: \b usb_mount /dev/sdx/1 /sdx1 fatfs => mount device to /sdx1 with fatfs file system. \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_USB_Mount(MS_U8 *devPath, MS_U8 *mountPath, MS_U8 *mountFS)
{
    MS_S32 ret = -1;
    ret = mount( devPath, mountPath, mountFS);
    printf("%s: mount \"%s\" to \"%s\" with FS:%s \n", (ret == 0) ? "ok!" : "Fail!", devPath, mountPath, mountFS);
    return (ret == 0) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
/// @brief USB or Mass Storage can be divided into multi-partition
/// @param[in] devPath - select device to be fdisk. 
/// @param[in] partitions - decide how many partition will be done. at most 4 partitions.
/// @param[in] p1_size - partition 1 size in MB unit.
/// @param[in] p2_size - partition 2 size in MB unit.
/// @param[in] p3_size - partition 3 size in MB unit.
/// @param[in] p4_size - partition 4 size in MB unit.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b usb_fdisk \n
///
/// Sample Command: \b usb_fdisk /dev/sdx/ 3 102400 102400 999999 => total 3 partitions, first 100G, second 100G, third keep the remain space. \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_USB_Fdisk(MS_U8 *devPath, MS_U32 * partitions, MS_U32 *p1_size, MS_U32 *p2_size, MS_U32 *p3_size, MS_U32 *p4_size)
{
    MS_S32 err = 0;
    do_partitions_info buf ;
    cyg_uint32 len = sizeof(do_partitions_info);
    buf.devname = devPath;
    buf.part_count = *partitions;
    buf.p1_size_MB = *p1_size;
    buf.p2_size_MB = *p2_size;
    buf.p3_size_MB = *p3_size;
    buf.p4_size_MB = *p4_size;
    cyg_io_handle_t device;

    err = cyg_io_lookup( devPath, &device );

    err = cyg_io_set_config(device, CYG_IO_SET_CONFIG_DISK_SET_PARTITIONS, &buf, &len);
    printf("err:%d\nfdisk path:%s\npartition_count:%d\nSize:%d %d %d %d\n", 
            err, devPath, *partitions, *p1_size, *p2_size, *p3_size, *p4_size);
    return (err == 0) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
/// @brief USB or Mass Storage can be formated as specific File System
/// @param[in] devname - select device partition to be formated. 
/// @param[in] lablename - Decide partition lable name.
/// @param[in] format_type - fatfs/ntfs.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b usb_format \n
/// Sample Command: \b usb_format /dev/sdx/1 vol_1 fatfs => format device with new lable name and file system\n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_USB_Format(MS_U8 *devname, MS_U8 *lablename, MS_U8 *format_type) 
{
    MS_S32 ret = 0;
    ret = format((MS_U8 *)devname, (MS_U8 *)lablename, (MS_U8 *)format_type);
    printf("%s: USB format %s to %s with labelName:%s \n", (ret == 0) ? "ok!" : "Fail!",  devname, format_type, lablename);

    return (ret == 0) ? TRUE : FALSE;
}
