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
/// @file   appDemo_JFFS2.c
/// @author MStar Semiconductor Inc.
/// @brief A sample code for illustrating how to use JFFS2 specific within flash
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
   @file appDemo_JFFS2.c
   @brief The sample code for the JFFS2_XXX functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "appDemo_JFFS2.h"
#include <cyg/fileio/fileio.h>
#include "MsFS.h"
#include "Board.h"

//-------------------------------------------------------------------------------------------------
// Local Available
//-------------------------------------------------------------------------------------------------
static MS_U8 GC_OverHead = 0;
//------------------------------------------------------------------------------
/// @brief Mount SPI Flash with JFFS2 to mounting directory. 
/// @param[in] devPath - select SPI device to be mounted. 
/// @param[in] mountPath - indicate to mount path .
/// @param[in] startAddr - start address of SPI Flash.
/// @param[in] size - allocate size of SPI Flash for JFFS2.
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b jffs2_mount \n
///
/// Sample Command: \b jffs2_mount /dev/flash/0 /fs 0x500000 0x100000 => mount SPI Flash device "/dev/flash/0" to /fs from 5MB of SPI for 1MB size. \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_JFFS2_Mount(MS_U8 *devPath, MS_U8 *mountPath, MS_U32 *startAddr, MS_U32 *size)
{
    MS_S32 ret = -1;
    MS_U8 file_path[64];
    sprintf(file_path, "%s/0x%X,0x%X", devPath, *startAddr, *size);

    ret = mount( file_path, mountPath, "jffs2");
    printf("%s: mount \"%s\" to \"%s\" with FS:%s \n", (ret == 0) ? "ok!" : "Fail!", file_path, mountPath, "jffs2");
    if (ret == 0)
    {
        // GC Recommended overhead => 2 + (( flashsize/50 ) + (flashsectors*100) + (sectorsize-1)) / sectorsize 
        // add 1 more sector for reserved space.
        GC_OverHead = 1 + 2 + (( (*size)/50 ) + ( ((*size)/FAKE_SPI_BLOCK_SIZE) * 100) + (FAKE_SPI_BLOCK_SIZE - 1)) / FAKE_SPI_BLOCK_SIZE;
        printf("file_path:%s mountPath:%s GC_OverHead:%d\n", file_path, mountPath, GC_OverHead);
    }
    return (ret == 0) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
/// @brief Get the Free Sectors for JFFS2.
/// @param[in] mountPath - select the mounted path of JFFS2. 
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b usb_fdisk \n
///
/// Sample Command: \b usb_fdisk /dev/sdx/ 3 102400 102400 999999 => total 3 partitions, first 100G, second 100G, third keep the remain space. \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_JFFS2_GetFreeSectors(MS_U8 *mountPath)
{
    MApi_FsInfo FsInfo;
    MS_BOOL ret = MApi_FS_Info(mountPath, FALSE, &FsInfo);
    if (ret)
    {
        printf("===========================================================\n");
        printf("totalSectors:%08X freeSectors:%08X sectorSize:%08X\n", FsInfo.u32ClusTotal, FsInfo.u32ClusFree, FsInfo.u32ClusSize);
        printf("===========================================================\n");
    }
    else
    {
        printf("Fail! get free sectors.\n");
    }
    return ret;
}
