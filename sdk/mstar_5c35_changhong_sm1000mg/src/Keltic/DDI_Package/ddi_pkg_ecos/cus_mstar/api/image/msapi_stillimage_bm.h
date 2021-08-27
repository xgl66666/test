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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

#ifndef _Dapi_STILLIMAGE_BM_H_
#define _Dapi_STILLIMAGE_BM_H_

#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus

//-------------------------------------------------------------------------------------------------
// Project include files:
//-------------------------------------------------------------------------------------------------
//#include "mm_types.h"

#include "MsCommon.h"
//#include "MsTypes2.h"
//#include "msapi_stillimage_dataio.h"
#include "msapi_stillimage_dataio_file.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#undef INTERFACE
#ifdef  _Dapi_STILLIMAGE_BM_C_
#define INTERFACE
#else
#define INTERFACE       extern
#endif  // #ifdef  _Dapi_STILLIMAGE_BM_C_

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_BM_FILEPOS_SEEK_CUR,      // Seek from current position of file pointer
    E_BM_FILEPOS_SEEK_SET,      // Seek from the beginning of file.
} EN_BM_FILEPOS;

typedef enum
{
    E_BM_RET_OK,
    E_BM_RET_FAIL,
} EN_BM_RET;

typedef enum
{
    E_STILLIMAGE_DATA_IO_PATH_FILE = 0,
    E_STILLIMAGE_DATA_IO_PATH_MEM,
    E_STILLIMAGE_DATA_IO_PATH_NUM,
} EN_STILLIMAGE_DATA_IO_PATH;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------
#define U8 MS_U8
#define U16 MS_U16
#define U32 MS_U32
#define S8 MS_S8
#define S16 MS_S16
#define S32 MS_S32
#define BOOL MS_BOOL
#define U64 MS_U64

// move from msapi_stillimage_defines.h
#define ENABLE_BM_READ_4KALIGNMENT      1
#define ENABLE_BM_NONBLOCKING_READ      1
#define BM_DEFAULT_PRELOAD_SIZE         (0x20000)    // Preload after the seek operation.
#define DATA_SIZE_UNKNOWN               (0xffffffff)
#define BM_NON_BLOCKING_READ_TIME_OUT   (2000L) // millisecond
#define BM_NON_BLOCKING_SEEK_TIME_OUT   (2000L) // millisecond
#define BM_DEFAULT_EXPIRETIME_VALUE     (5L)    // 5 ms during the BM_Task.
#define INVALID_HDL         (0xFF)
#define U64_MAX         (0xFFFFFFFFFFFFFFFFULL)
#define U32_MAX         (0xFFFFFFFFUL)
#if ENABLE_BM_NONBLOCKING_READ
#define ENABLE_BM_NONBLOCKING_SEEK      1 // <-- modify this value for enabling non-blocking seek
#else
#define ENABLE_BM_NONBLOCKING_SEEK      0 // <-- this value can't be changed
#endif // #if ENABLE_BM_NONBLOCKING_READ
typedef union
{
    U64 u64;
    struct {
        U32 u32_0;
        U32 u32_1;
    };
    struct {
        U16 u16_0;
        U16 u16_1;
        U16 u16_2;
        U16 u16_3;
    };
    struct {
        U8 u8_0;
        U8 u8_1;
        U8 u8_2;
        U8 u8_3;
        U8 u8_4;
        U8 u8_5;
        U8 u8_6;
        U8 u8_7;
    };
} UU64;
#define u32u64_0(u)             (((UU64*)&(u))->u32_0)
#define u32u64_1(u)             (((UU64*)&(u))->u32_1)

// move from msapi_stillimage_defines.h
// NOTE: One who implement IO for Still Image has to provide initial function
typedef void                        (*StillImage_DataIO_Init)(MS_U32 u32ObjNum);
typedef void                        (*StillImage_DataIO_Task)(MS_U32 u32ObjNum);
typedef EN_STILLIMAGE_DATA_IO_RET   (*StillImage_DataIO_Open)(MS_U8* filename, MS_U64 *pu64Size, MS_U8 *pu8Hdl, MS_U32 u32ObjNum);
typedef EN_STILLIMAGE_DATA_IO_RET   (*StillImage_DataIO_Close)(MS_U8 u8Hdl, MS_U32 u32ObjNum);
typedef MS_U32                         (*StillImage_DataIO_Read)(MS_U8 u8Hdl, MS_U32 u32Addr, MS_U32 u32Size, MS_U32 u32ObjNum);
typedef EN_STILLIMAGE_DATA_IO_RET   (*StillImage_DataIO_Seek)(MS_U8 u8Hdl, MS_U64 u64Offset, MS_U32 u32ObjNum);

// NOTE: One who implement StillImage_IO_NonBlockingRead() has to call
// msAPI_StillImage_BMFile_NonBlockingReadDone() when non-blocking read is satisfied.
typedef void                        (*StillImage_DataIO_NonBlockingRead)(MS_U8 u8Hdl, MS_U32 u32Addr, MS_U32 u32Size, MS_U32 u32ObjNum);

// NOTE: One who implement StillImage_IO_NonBlockingSeek() has to call
// msAPI_StillImage_BMFile_NonBlockingSeekDone() when non-blocking seek is satisfied.
typedef void                        (*StillImage_DataIO_NonBlockingSeek)(MS_U8 u8Hdl, MS_U64 u64Offset, MS_U32 u32ObjNum);

typedef EN_STILLIMAGE_DATA_IO_RET   (*StillImage_DataIO_Control)(EN_STILLIMAGE_DATA_IO_CONTROL_CMD eCtrlCmd, MS_U32 u32Value, MS_U32 u32ObjNum);
typedef void                        (*StillImage_DataIO_CallBack)(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue, MS_U32 u32ObjNum);

typedef struct
{
    // NOTE: all this function pointers can't be NULL
    StillImage_DataIO_Init  pfStillImage_DataIO_Init;
    StillImage_DataIO_Task  pfStillImage_DataIO_Task;
    StillImage_DataIO_Open  pfStillImage_DataIO_Open;
    StillImage_DataIO_Close pfStillImage_DataIO_Close;
    StillImage_DataIO_Read  pfStillImage_DataIO_Read;
    StillImage_DataIO_Seek  pfStillImage_DataIO_Seek;
    StillImage_DataIO_NonBlockingRead   pfStillImage_DataIO_NonBlockingRead;
    StillImage_DataIO_NonBlockingSeek   pfStillImage_DataIO_NonBlockingSeek;

    StillImage_DataIO_Control   pfStillImage_DataIO_Control;
    StillImage_DataIO_CallBack  pfStillImage_DataIO_CallBack;
} ST_STILLIMAGE_DATA_IO_FUNC, *PST_STILLIMAGE_DATA_IO_FUNC;

//-------------------------------------------------------------------------------------------------
// Extern Functions
///-------------------------------------------------------------------------------------------------
INTERFACE void      Dapi_StillImage_BM_Init(void);
INTERFACE void      Dapi_StillImage_BM_SetFileIOTimeout_Ex(U32 u32NBReadTimeout, U32 u32NBSeekTimeout, U32 u32BFileTimeout, U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BM_SetIO(EN_STILLIMAGE_DATA_IO_PATH eStillImagePath);
INTERFACE void      Dapi_StillImage_BM_Task(U32 u32ObjNum);
INTERFACE EN_BM_RET Dapi_StillImage_BM_SetPreloadSize_Ex(U32 u32Size, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BM_GetPreloadSize_Ex(U32 u32ObjNum);
INTERFACE EN_BM_RET Dapi_StillImage_BM_Phase2(BOOL bEnable);
INTERFACE BOOL      Dapi_StillImage_BM_IsPhase2(void);

INTERFACE EN_BM_RET Dapi_StillImage_BMFile_Open(void);
INTERFACE EN_BM_RET Dapi_StillImage_BMFile_Close(void);
INTERFACE U64       Dapi_StillImage_BMFile_Size_Ex(U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BMFile_SetSize(U64 u64Size);
INTERFACE U64       Dapi_StillImage_BMFile_Tell(void);
INTERFACE BOOL      Dapi_StillImage_BMFile_EOF(void);
INTERFACE void      Dapi_StillImage_BMFile_NonBlockingReadDone(U8 u8FileHdl, U32 u32LeftUnRead, U32 u32ObjNum);
INTERFACE EN_BM_RET Dapi_StillImage_BMFile_Seek_Ex(U64 u64Offset, EN_BM_FILEPOS ePos, U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BMFile_NonBlockingSeekDone(U8 u8FileHdl, U32 u32ObjNum);

INTERFACE EN_BM_RET Dapi_StillImage_BMBuffer_Configuration_Ex(U32 u32Lower, U32 u32Upper, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ActiveBuffSize_Ex(U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ShowBytes(U8 u8NBytes, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ReadBytes(U8 u8NBytes, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ShowBytes_LE(U8 u8NBytes, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ReadBytes_LE(U8 u8NBytes, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_CustomFill_Ex(U32 u32Size, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ReadBytes_AutoLoad(U8 u8NBytes, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_ReadBytes_LE_AutoLoad_Ex(U8 u8NBytes, U32 u32ObjNum);
INTERFACE U8        Dapi_StillImage_BMBuffer_PeekNthByte_Ex (U32 u32Nth, U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_CopyNByte (U8 *pu8Buffer, U32 u32N, U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BMBuffer_FlushBytes_Ex(U32 u32Bytes, U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BMBuffer_ResetRead(U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_UpdateBase_Ex(U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_Remainder_Ex(U32 u32ObjNum);
INTERFACE U32       Dapi_StillImage_BMBuffer_SpaceLeft_Ex(U32 u32ObjNum);
INTERFACE EN_BM_RET Dapi_StillImage_BMBuffer_Reset_Ex(U32 u32ObjNum);
INTERFACE BOOL      Dapi_StillImage_BMBuffer_IsFull(U32 u32ObjNum);
INTERFACE BOOL      Dapi_StillImage_BMBuffer_IsEmpty(U32 u32ObjNum);


INTERFACE void      Dapi_StillImage_BM_Init_Ex(U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BM_SetIO_Ex(EN_STILLIMAGE_DATA_IO_PATH eStillImagePath, U32 u32ObjNum);
INTERFACE EN_BM_RET Dapi_StillImage_BM_Phase2_Ex(BOOL bEnable, U32 u32ObjNum);
INTERFACE BOOL      Dapi_StillImage_BM_IsPhase2_Ex(U32 u32ObjNum);

INTERFACE EN_BM_RET Dapi_StillImage_BMFile_Open_Ex(U32 u32ObjNum);
INTERFACE EN_BM_RET Dapi_StillImage_BMFile_Close_Ex(U32 u32ObjNum);
INTERFACE void      Dapi_StillImage_BMFile_SetSize_Ex(U64 u64Size, U32 u32ObjNum);
INTERFACE U64       Dapi_StillImage_BMFile_Tell_Ex(U32 u32ObjNum);
INTERFACE BOOL      Dapi_StillImage_BMFile_EOF_Ex(U32 u32ObjNum);

INTERFACE U32       Dapi_StillImage_BMBuffer_ReadBytes_LE_AutoLoad(U8 u8NBytes);
INTERFACE EN_BM_RET Dapi_StillImage_BMFile_Seek(U64 u64Offset, EN_BM_FILEPOS ePos);
INTERFACE U32       Dapi_StillImage_BM_GetPreloadSize(void);
INTERFACE U32       Dapi_StillImage_BMBuffer_CustomFill(U32 u32Size);
INTERFACE U64       Dapi_StillImage_BMFile_Size(void);
INTERFACE U32       Dapi_StillImage_BMBuffer_UpdateBase(void);
INTERFACE U32       Dapi_StillImage_BMBuffer_Remainder(void);
INTERFACE U8        Dapi_StillImage_BMBuffer_PeekNthByte (U32 u32Nth);
INTERFACE void      Dapi_StillImage_BMBuffer_FlushBytes(U32 u32Bytes);
INTERFACE EN_BM_RET Dapi_StillImage_BM_SetPreloadSize(U32 u32Size);
INTERFACE EN_BM_RET Dapi_StillImage_BMBuffer_Configuration(U32 u32Lower, U32 u32Upper);
INTERFACE EN_BM_RET Dapi_StillImage_BMBuffer_Reset(void);
INTERFACE void      Dapi_StillImage_BM_SetFileIOTimeout(U32 u32NBReadTimeout, U32 u32NBSeekTimeout, U32 u32BFileTimeout);
INTERFACE U32       Dapi_StillImage_BMBuffer_ActiveBuffSize(void);
INTERFACE U32       Dapi_StillImage_BMBuffer_SpaceLeft(void);


#undef INTERFACE

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus

#endif  // #ifndef _MSAPI_STILLIMAGE_BM_H_

///*** please do not remove change list tag ***///
///***$Change: 428882 $***///

