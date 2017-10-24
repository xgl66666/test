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

#ifndef _Dapi_STILLIMAGE_DATA_IO_FILE_H_
#define _Dapi_STILLIMAGE_DATA_IO_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus
#include "MsCommon.h"
#include "msapi_stillimage_def.h"

//-------------------------------------------------------------------------------------------------
// Project include files:
//-------------------------------------------------------------------------------------------------
//#include "mm_types.h"

//#include "msapi_stillimage_dataio.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#undef INTERFACE
#ifdef  _Dapi_STILLIMAGE_DATA_IO_FILE_C_
#define INTERFACE
#else
#define INTERFACE       extern
#endif  // #ifdef  _Dapi_STILLIMAGE_DATA_IO_FILE_C_
#define MAX_MBX_PARAM_SIZE      10

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
typedef enum
{
    /// House Keeping Identifier
    E_MBX_ROLE_HK = 0,
    /// Co-Processer Identifier
    E_MBX_ROLE_CP = 1,
    /// PM Identifier
    E_MBX_ROLE_PM = 2,
    E_MBX_ROLE_MAX
}MBX_ROLE_ID;

/// Mail Message Class Type Defines
typedef enum
{
    /// Normal Message Class Type
    E_MBX_MSG_TYPE_NORMAL  = 0,
    /// Instant Message Class Type
    E_MBX_MSG_TYPE_INSTANT = 1
}MBX_MSG_Type;

typedef struct
{
    /// Role ID, for send, this is the Mail Target, for receive, this is the Source which sent this message
    MBX_ROLE_ID      eRoleID;
    /// Mail Message Type, Normal or Instant
    MBX_MSG_Type     eMsgType;

    /// Ctrl Byte in Mail Message
    MS_U8              u8Ctrl;
    /// @ref MBX_Class
    MS_U8              u8MsgClass; //should be MBX_Class, for alignment let it be S8, gcc default sizeof(enum) is 4 bytes.
    /// Command Index defined by Apps
    MS_U8              u8Index;
    /// Parameter Count
    MS_U8              u8ParameterCount;
    /// Parameters, Max Number @ref MAX_MBX_PARAM_SIZE
    MS_U8              u8Parameters[MAX_MBX_PARAM_SIZE];
    /// Status 0 byte of Mail Message
    MS_U8              u8S0;
    /// Status 1 byte of Mail Message
    MS_U8              u8S1;
}MBX_Msg;


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_STILLIMAGE_DATA_IO_FILE_ACKFLG_NULL           = 0,
    E_STILLIMAGE_DATA_IO_FILE_ACKFLG_WAIT_OPEN_ACK  = 0x0001,
    E_STILLIMAGE_DATA_IO_FILE_ACKFLG_WAIT_CLOSE_ACK =0x0002,
    E_STILLIMAGE_DATA_IO_FILE_ACKFLG_WAIT_READ_ACK  = 0x0004,
    E_STILLIMAGE_DATA_IO_FILE_ACKFLG_WAIT_SEEK_ACK  = 0x0008,
} E_STILLIMAGE_DATA_IO_FILE_ACK_FLAGS;

typedef enum
{
    E_STILLIMAGE_DATA_IO_OK,
    E_STILLIMAGE_DATA_IO_WAITING,
    E_STILLIMAGE_DATA_IO_FAIL,
} EN_STILLIMAGE_DATA_IO_RET;

typedef enum
{
    E_STILLIMAGE_DATA_IO_FLAG_NONE   = 0x0,

    E_STILLIMAGE_DATA_IO_FLAG_MORE           = 0x1,
    E_STILLIMAGE_DATA_IO_FLAG_NOT_SEEKABLE   = 0x2,
} EN_STILLIMAGE_DATA_IO_FLAGS;

typedef enum
{
    E_STILLIMAGE_DATA_IO_CTL_NONE   = 0x0,

    E_STILLIMAGE_DATA_IO_CTL_GET_READ_TIME_OUT  = 0x1,  // this second argument of StillImage_DataIO_Control is a pointer to U32
    E_STILLIMAGE_DATA_IO_CTL_SET_READ_TIME_OUT  = 0x2,  // this second argument of StillImage_DataIO_Control is a U32
} EN_STILLIMAGE_DATA_IO_CONTROL_CMD;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------
//extern ST_STILLIMAGE_DATA_IO_FUNC stStillImage_DataIO_File;

//-------------------------------------------------------------------------------------------------
// Extern Functions
///-------------------------------------------------------------------------------------------------
INTERFACE void                      Dapi_StillImage_DataIO_FileInit(MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_FileTask(MS_U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_FileOpen(MS_U8* filename, MS_U64 *pu64Size, MS_U8 *pu8Hdl, MS_U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_FileClose(MS_U8 u8Hdl, MS_U32 u32ObjNum);
INTERFACE MS_U32                    Dapi_StillImage_DataIO_FileRead(MS_U8 u8Hdl, MS_U32 u32Addr, MS_U32 u32Size, MS_U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_FileSeek(MS_U8 u8Hdl, MS_U64 u64Offset, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_FileNonBlockingRead(MS_U8 u8Hdl, MS_U32 u32Address, MS_U32 u32Size, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_FileNonBlockingSeek(MS_U8 u8Hdl, MS_U64 u64Offset, MS_U32 u32ObjNum);

INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_FileControl(EN_STILLIMAGE_DATA_IO_CONTROL_CMD eCtrlCmd, MS_U32 u32Value, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_FileCallBack(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_SetParas(ST_STILLIMAGE_CFG_PARAMS* pInputParas);
INTERFACE void                      Dapi_StillImage_SetFrameInfo(MS_U32 u32DisplayedFrameIndex);
INTERFACE void                      Dapi_StillImage_GetParas(ST_STILLIMAGE* pGetParas);

INTERFACE void                      Dapi_StillImage_DataIO_MemInit(MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_MemTask(MS_U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_MemOpen(MS_U8* filename, MS_U64 *pu64Size, MS_U8 *pu8Hdl, MS_U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_MemClose(MS_U8 u8Hdl, MS_U32 u32ObjNum);
INTERFACE MS_U32                       Dapi_StillImage_DataIO_MemRead(MS_U8 u8Hdl, MS_U32 u32Addr, MS_U32 u32Size, MS_U32 u32ObjNum);
INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_MemSeek(MS_U8 u8Hdl, MS_U64 u64Offset, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_MemNonBlockingRead(MS_U8 u8Hdl, MS_U32 u32Address, MS_U32 u32Size, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_MemNonBlockingSeek(MS_U8 u8Hdl, MS_U64 u64Offset, MS_U32 u32ObjNum);

INTERFACE EN_STILLIMAGE_DATA_IO_RET Dapi_StillImage_DataIO_MemControl(EN_STILLIMAGE_DATA_IO_CONTROL_CMD eCtrlCmd, MS_U32 u32Value, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DataIO_MemCallBack(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue, MS_U32 u32ObjNum);

INTERFACE void                      Dapi_StillImage_Init(MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_DeInit(MS_U32 u32ObjNum);

INTERFACE void                      Dapi_StillImage_GetParas_Ex(ST_STILLIMAGE* pGetParas, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_SetFrameInfo_Ex(MS_U32 u32DisplayedFrameIndex, MS_U32 u32ObjNum);
INTERFACE void                      Dapi_StillImage_SetParas_Ex(ST_STILLIMAGE_CFG_PARAMS* pInputParas, MS_U32 u32ObjNum);

#undef INTERFACE

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus

#endif  // #ifndef _Dapi_STILLIMAGE_DATA_IO_FILE_H_


///*** please do not remove change list tag ***///
///***$Change: 428882 $***///
