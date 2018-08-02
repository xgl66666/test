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
#ifndef __MSAPI_MM_FS_H__
#define __MSAPI_MM_FS_H__

#include "MsMMTypes.h"
#ifdef _MSAPI_DATA_STREAM_IO_C_
#define INTERFACE extern
#else
#define INTERFACE extern
#endif

/*definition ====================================================================================*/
#ifdef NEW_VDPLAYER
#define MAX_FILEHANDLE_CNT 9//256
#else
#define MAX_FILEHANDLE_CNT 8//256
#endif
#define BYTES_PER_KBYTE    1024
#define INVALID_FILE_HANDLE                        0xff
#define INVALID_DATA_STREAM_HDL (0xffffffff)

#define MAX_PATH_NAME_LEN 512
#define MAX_DRV_COUNT 32
#define MAX_ENV_CNT 4
#define MAX_DEVICE_CNT 16

/*definition ====================================================================================*/
#define INVALID_DRIVE_INDEX                        0xff
#define INVALID_DEVICE_INDEX                    0xFF
#define INVALID_FS_ENVIRONMENT_INDEX            0xFF
#define INVALID_FILE_HANDLE                        0xff



#ifndef FCTRL_INVALID_FILE_HANDLE
#define FCTRL_INVALID_FILE_HANDLE                0xff
#endif
#define FCTRL_RW_DATA_CACHE_SIZE                2

//#define MSD_NAND_INDEX_END     (MSD_NAND_INDEX_START + MSD_NAND_NUM)
//#define STORAGE_DEVICE_NUM   (MSD_USB_NUM + MSD_USB_2_NUM + MSD_CREADER_NUM + MSD_CF_NUM + MSD_NAND_NUM)
//#define STORAGE_DRIVE_NUM DISK_DRIVE_NUM
//#define FILE_SYS_STRUCT_SIZE 512
#define FAT_SECTOR_SIZE_512BYTE 512


typedef struct
{
   int fd;
   //U8 drvID;
   char fullPathName[MAX_PATH_NAME_LEN];
}FILE_HANDLE_INFO;

typedef struct
{
    U32 Hi;
    U32 Lo;
}LongLong;

typedef enum
{
    FILE_SEEK_SET   = 0,
    FILE_SEEK_CUR   = 1,
#ifdef NEW_VDPLAYER
    FILE_SEEK_END   = 2,
#endif
}EN_FILE_SEEK_OPTION;
typedef enum
{
    ENTRY_TYPE_FILE,
    ENTRY_TYPE_DIR,
    ENTRY_TYPE_DIR_DOT,      // Current directory
    ENTRY_TYPE_DIR_DOTDOT,   // Parent directory
    ENTRY_TYPE_LAST,
    ENTRY_TYPE_NULL=0xFF,
} EN_ENTRY_TYPE;
typedef enum
{
    ENTRY_NEXT_RESULT_ERROR,
    ENTRY_NEXT_RESULT_SUCCESS,
    ENTRY_NEXT_RESULT_LAST
}EN_ENTRY_NEXT_RESULT;

typedef enum
{
    FILE_SYSTEM_MOUNT_RO=0,
    FILE_SYSTEM_MOUNT_RW,
}EN_FILE_SYSTEM_MNT_MODE;

typedef enum
{
    FILE_SYSTEM_TYPE_NONE=0,
    FILE_SYSTEM_TYPE_FAT,
    FILE_SYSTEM_TYPE_NTFS,
    FILE_SYSTEM_TYPE_EXT2,
    FILE_SYSTEM_TYPE_EXT3,
    FILE_SYSTEM_TYPE_N51FS,
    FILE_SYSTEM_TYPE_UDF,
    FILE_SYSTEM_TYPE_ISO9660,
    FILE_SYSTEM_TYPE_ERROR=0xFF,
}EN_FILE_SYSTEM_TYPE;

/*
typedef struct
{
     EN_FILE_SYSTEM_TYPE fsType;
     EN_FILE_SYSTEM_MNT_MODE mountMode;
     U8 portNO;
     U8 hubportNO;
     U8 deviceID;
     U32 totalSpaceInKB;
     U32 freeSpaceInKB;
     char devName[MAX_PATH_NAME_LEN];
     char mountpoint[MAX_PATH_NAME_LEN];
}DRIVE_INFO;
*/

#define U32_B0(val)  ((U8)(val))
#define U32_B1(val)  ((U8)((val)>>8))
#define U32_B2(val)  ((U8)((val)>>16))
#define U32_B3(val)  ((U8)((val)>>24))

#define MK_U32(B0, B1, B2, B3) ((((U32)(B3))<<24) |(((U32)(B2))<<16)|(((U32)(B1))<<8)|((U32)(B0)))
//static DRIVE_INFO  g_drvInfo[MAX_DRV_COUNT];
//static U32 g_drvUsedMask[(MAX_DRV_COUNT+31)/32];

//#define IS_DRV_SLOT_USED(i)    (g_drvUsedMask[i/32] & (1<<(i&31)))
//#define SET_DRV_SLOT_USED(i)    do{g_drvUsedMask[i/32] |=  (1<<(i&31));}while(0)
//#define CLR_DRV_SLOT_USED(i)    do{g_drvUsedMask[i/32] &= ~(1<<(i&31));}while(0)


typedef enum
{
    FILE_CLOSE_RESULT_ERROR,
    FILE_CLOSE_RESULT_SUCCESS,
    FILE_CLOSE_RESULT_CLOSED_ALREADY,
}EN_FILE_CLOSE_RESULT;

typedef enum
{
    E_DATA_STREAM_SEEK_SET    = 0,
    E_DATA_STREAM_SEEK_CUR    = 1,
} E_DATA_STREAM_SEEK_OPTION;
typedef enum
{
    OPEN_MODE_FOR_READ                          = 0,
    OPEN_MODE_BINARY_FOR_READ                   = 1,
    OPEN_MODE_FOR_READ_WRITE                    = 2,
    OPEN_MODE_BINARY_FOR_READ_WRITE             = 3,
    OPEN_MODE_EMPTY_FOR_WRITE                   = 4,
    OPEN_MODE_BINARY_EMPTY_FOR_WRITE            = 5,
    OPEN_MODE_EMPTY_FOR_READ_WRITE              = 6,
    OPEN_MODE_BINARY_EMPTY_FOR_READ_WRITE       = 7,
    OPEN_MODE_FOR_APPEND                        = 8,
    OPEN_MODE_BINARY_FOR_APPEND                 = 9,
    OPEN_MODE_FOR_READ_APPEND                   = 10,
    OPEN_MODE_BINARY_FOR_READ_APPEND            = 11,
    OPEN_MODE_FAST_FOR_READ                     = 0x80,
    OPEN_MODE_NONE                              =0xFF,
}EN_OPEN_MODE;

typedef enum
{
    E_DATA_STREAM_TYPE_IGNORE     = 0,

    E_DATA_STREAM_TYPE_PHOTO,

    E_DATA_STREAM_TYPE_MUSIC,
    E_DATA_STREAM_TYPE_MUSIC2,
    E_DATA_STREAM_TYPE_LYRIC,

    E_DATA_STREAM_TYPE_VIDEO,
    E_DATA_STREAM_TYPE_AUDIO,
    E_DATA_STREAM_TYPE_IN_SUB,
    E_DATA_STREAM_TYPE_EX_SUB0,
    E_DATA_STREAM_TYPE_EX_SUB1,
    E_DATA_STREAM_TYPE_ATTACHMENT,

    E_DATA_STREAM_TYPE_NUM,
} E_DATA_STREAM_TYPE;

typedef struct
{
    U32         (*pfnOpen)  (void *pPrivate, U8 u8Mode, E_DATA_STREAM_TYPE eType);
    BOOLEAN     (*pfnClose) (U32 u32Hdl);
    U32         (*pfnRead)  (U32 u32Hdl, void *pBuffAddr, U32 u32Length);
    BOOLEAN     (*pfnSeek)  (U32 u32Hdl, LongLong u64Pos, E_DATA_STREAM_SEEK_OPTION eOption);
    LongLong    (*pfnTell)  (U32 u32Hdl);
    LongLong    (*pfnLength)    (U32 u32Hdl);
} FN_DATA_STRM_IO;

//<< FileEntry STRUCTURE>>======================================================================//
//284 bytes
typedef struct
{
    //EN_ENTRY_TYPE  EntryType;
    //LongLong FileLength;
    //U32 EntryFileLength;
    //U32 EntrySeconds;//since 1980/01/01/0/0/0
    //U32 st_mode;
    char fullname[MAX_PATH_NAME_LEN];
    //U8 EnvironmentID;
    //U8 FileSystemID;//drvID
    //U8 EntryAttrib;
    //char dumpfortest[140];//no used, save this memory for FILE_INFO_LONG_FILENAME_SIZE
} FileEntry;

//------------------------------------------------------------------------------
// Extern Global Variabls
//------------------------------------------------------------------------------
U8 FS_FileOpen(const FileEntry *pFileEntry, EN_OPEN_MODE u8OpenMode);
EN_FILE_CLOSE_RESULT FS_FileClose(U8 u8FileHandle);
U32 FS_FileWrite(U8 u8HandleNo, void* pu32Buffer, U32 u32Length);
U32 FS_FileRead(U8 u8HandleNo, void* pu32Buffer, U32 u32Length);
#ifdef NEW_VDPLAYER
MS_S64 FS_FileSeek(U8 u8HandleNo, MS_S64 offset, EN_FILE_SEEK_OPTION option);
MS_S64 FS_TellFile(U8 u8HandleNo);
#else
BOOLEAN FS_FileSeek(U8 u8HandleNo,  LongLong seekOffset, EN_FILE_SEEK_OPTION option);
BOOLEAN FS_TellFile(U8 u8HandleNo, LongLong *pRetLongLong);
#endif
BOOLEAN FS_FileLength(U8 u8HandleNo, LongLong *pRetLongLong);
void FS_Stub_Init(void);
INTERFACE BOOLEAN msAPI_MM_FCtrl_FileSeek(U8 u8HandleNo, U32 u32Length, EN_FILE_SEEK_OPTION option);
INTERFACE U32 msAPI_MM_FCtrl_FileTell(U8 u8HandleNo);
INTERFACE U32 msAPI_MM_FCtrl_FileLength(U8 u8HandleNo);
INTERFACE U8 msAPI_MM_FCtrl_FileOpen(FileEntry* pFileEntry, U8 u8OpenMode);
INTERFACE EN_FILE_CLOSE_RESULT msAPI_MM_FCtrl_FileClose(U8 u8HandleNo);
INTERFACE U32 msAPI_MM_FCtrl_FileRead(U8 u8HandleNo, U32 u32Buffer, U32 u32Length);
INTERFACE U32 msAPI_MM_FCtrl_FileWrite(U8 u8HandleNo, U32 u32Buffer, U32 u32Length);

INTERFACE U32       msAPI_DataStreamIO_Open(void *pPrivate, U8 u8Mode, E_DATA_STREAM_TYPE eType);
INTERFACE BOOLEAN   msAPI_DataStreamIO_Close(U32 u32Hdl);
INTERFACE U32       msAPI_DataStreamIO_Read(U32 u32Hdl, void *pBuffAddr, U32 u32Length);
INTERFACE BOOLEAN   msAPI_DataStreamIO_Seek(U32 u32Hdl, U32 u32Pos, E_DATA_STREAM_SEEK_OPTION eOption);
INTERFACE BOOLEAN   msAPI_DataStreamIO_Seek_LongLong(U32 u32Hdl, LongLong u64Pos, E_DATA_STREAM_SEEK_OPTION option);
INTERFACE U32       msAPI_DataStreamIO_Tell(U32 u32Hdl);
INTERFACE LongLong  msAPI_DataStreamIO_Tell_LongLong(U32 u32Hdl);
INTERFACE U32       msAPI_DataStreamIO_Length(U32 u32Hdl);
INTERFACE LongLong  msAPI_DataStreamIO_Length_LongLong(U32 u32Hdl);

INTERFACE BOOLEAN   msAPI_DataStreamIO_OptionRegistation(FN_DATA_STRM_IO *pfnIO);

#undef  INTERFACE
#endif

