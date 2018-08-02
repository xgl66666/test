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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_FS.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __API_PVRPL_FS_H__
#define __API_PVRPL_FS_H__

#include "MsCommon.h"
#include <sys/stat.h>
#include <dirent.h>
#include "PVRPL_Common.h"

#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
/// Open a file
/// @param  filename \b IN: the pathname of the file
/// @param  mode     \b IN:  r or rb - Open file for reading.
///                         w or wb - Truncate to zero length or create file for writing.
///                         a or ab - Append; open or create file for writing at end-of-file.
///                         r+ or rb+ or r+b - Open file for update (reading and writing).
///                         w+ or wb+ or w+b - Truncate to zero length or create file for update.
///                         a+ or ab+ or a+b Append; open or create file for update, writing at end-of-file.
/// @return the file descriptor associated with the file
/// @return NULL : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
FILE * PVRPL_FileOpen(const char *filename, const char *mode, PVR_PATH u8PathIdx);
//-------------------------------------------------------------------------------------------------
/// Close a file
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileClose(FILE *stream);
//-------------------------------------------------------------------------------------------------
/// Read from a file
/// @param  data     \b OUT: read buffer
/// @param  itemsize \b IN:  item size in bytes
/// @param  nitems   \b IN:  number of items
/// @param  stream   \b IN:  the file descriptor associated with the file
/// @return number of items actually read
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
size_t PVRPL_FileRead(void *data, size_t itemsize, size_t nitems, FILE *stream);
//-------------------------------------------------------------------------------------------------
/// Write to a file
/// @param  data     \b IN: write buffer
/// @param  itemsize \b IN: item size in bytes
/// @param  nitems   \b IN: number of items
/// @param  stream   \b IN: the file descriptor associated with the file
/// @return number of items actually written
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
size_t PVRPL_FileWrite(const void *data, size_t itemsize, size_t nitems, FILE *stream);
//-------------------------------------------------------------------------------------------------
/// Flush to a file
/// @param  stream   \b IN: the file descriptor associated with the file
/// @return number of items actually written
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileFlush(FILE *stream);
//-------------------------------------------------------------------------------------------------
/// Seek a file
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @param  pos     \b IN:  file byte offset relative to whence
/// @param  whence  \b IN:  SEEK_SET / SEEK_CUR / SEEK_END
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileSeek(FILE *stream, MS_U64 pos, int whence);
//-------------------------------------------------------------------------------------------------
/// Obtain current file position
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @return current file position measured in bytes from the beginning of the file
/// @return 0xFFFFFFFF : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_U64 PVRPL_FileTell(FILE *stream);
//-------------------------------------------------------------------------------------------------
/// Reset the file position
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @return NONE
//-------------------------------------------------------------------------------------------------
void PVRPL_FileRewind(FILE *stream);
//-------------------------------------------------------------------------------------------------
/// Rename a file/directory
/// @param  path1    \b IN: the old pathname of the file
/// @param  path2    \b IN: The new pathname of the file
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileRename(const char *path1, const char *path2);
//-------------------------------------------------------------------------------------------------
/// Get file status
/// @param  path    \b IN:  the pathname of the file
/// @param  buf     \b IN:  a pointer to a stat structure as defined in <sys/stat.h>
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
/// @note   equivalent to MsFS_FStat
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileStat(const char *path, struct stat *buf);
//-------------------------------------------------------------------------------------------------
/// Synchronize changes to a file (all data for the open file will be transferred to the storage device)
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
void PVRPL_FileSync(void);
//-------------------------------------------------------------------------------------------------
/// Unlink/Remove a file
/// @param  path    \b IN: the pathname of the file
/// @return =0 : the file descriptor associated with the named file
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileUnlink(const char *path);


// Current not support
//-------------------------------------------------------------------------------------------------
/// Mount a filesystem
/// @param  devname \b IN: name of hardware device:     eg. ""      , "/dev/fd0/"   , ...
/// @param  dir     \b IN: name of mount point:             "/ram"  , "/floppy"     , ...
/// @param  fsname  \b IN: name of implementing filesystem: "ramfs" , "fatfs"       , ...
/// @return 0 : no error
/// @return otherwise : error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileMount( const char *devname, const char *dir, const char *fsname, unsigned long mountflags, const void* data);
//-------------------------------------------------------------------------------------------------
/// Unmount a filesystem
/// @param  dir     \b IN: name of mount point:         eg.   "/ram"  , "/floppy"     , ...
/// @return 0 : no error
/// @return otherwise : error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileUmount( const char *dir);
//-------------------------------------------------------------------------------------------------
/// Open a directory
/// @param  dirname \b IN:  the pathname of the directory
/// @return a pointer to DIR object
/// @return null : errno set to indicate the error.
//-------------------------------------------------------------------------------------------------
DIR* PVRPL_FileOpenDir( const char *dirname);
//-------------------------------------------------------------------------------------------------
/// Close a directory stream
/// @param  dirp    \b IN:  a pointer to DIR object
/// @return NONE
//-------------------------------------------------------------------------------------------------
void PVR_FileCloseDir( DIR *dirp);
//-------------------------------------------------------------------------------------------------
/// Make a directory
/// @param  path    \b IN: the pathname of the directory
/// @param  mode    \b IN: not support
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_S32 PVRPL_FileMkDir( const char *path, mode_t mode);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_PVRPL_FS_H__
