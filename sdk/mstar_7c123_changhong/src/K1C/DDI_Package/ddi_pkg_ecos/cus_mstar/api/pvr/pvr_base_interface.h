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
//    modification/derivatives thereof is transfErred to you under Terms.
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
// Copyright (c) 2010-2011 MStar Semiconductor, Inc.
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

// pvr_interface.h: interface for the PVR_API class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PVR_BASE_INTERFACE_H_
#define _PVR_BASE_INTERFACE_H_

#include "mapi_types.h"

// Base



// limitation
#define DUALDOWNLOAD_INDEX 0
#define PVR_RECORD_THREAD_NUM 5
#define PVR_PLAYBACK_THREAD_NUM 2

//
#define PVR_INVALID_PID_ID (0x1FFF)
#define PVRMW_IS_VALID_PID(x)  (x < PVR_INVALID_PID_ID)

// types
typedef MAPI_U8 Record_Handle;
typedef MAPI_U8 Playback_Handle;

//#define INVALID_RECORD_HANDLE 0xFF
//#define INVALID_PLAYBACK_HANDLE 0xFF
static const Record_Handle INVALID_RECORD_HANDLE = 0xFF;
static const Playback_Handle INVALID_PLAYBACK_HANDLE = 0xFF;

// PVR Path = mode mod idx
#define PVR_PATH                     MAPI_U8
#define PVR_PATH_IDX_MASK            0x3F
#define PVR_PATH_MODE_MASK           0xC0
#define PVR_PATH_MODE_RECORD         0x00
#define PVR_PATH_MODE_PLAYBACK       0x80
#define CONVERT_TO_PVR_PATH(_PvrMode, _PathIdx) (((_PvrMode)&PVR_PATH_MODE_MASK)|((_PathIdx)&PVR_PATH_IDX_MASK))
#define GET_PVR_PATH_MODE(_PvrPath) ((_PvrPath)&(PVR_PATH_MODE_MASK))
#define GET_PVR_PATH_IDX(_PvrPath) ((_PvrPath)&(PVR_PATH_IDX_MASK))

///define download type enum
typedef enum
{
    /// one record in one thread
    EN_PVR_DOWNLOAD_TYPE_SINGLE = 1,
    /// record in linear and circular type in one thread
    EN_PVR_DOWNLOAD_TYPE_DUAL   = 2,
    /// record in linear or circular type in one thread => multi record
    EN_PVR_DOWNLOAD_TYPE_SWPVR  = 3,
    /// invalid type
    EN_PVR_DOWNLOAD_TYPE_INVALID
} PVR_DOWNLOAD_TYPE;

typedef enum
{
    PVR_DBG_NONE,
    PVR_DBG_ERR,
    PVR_DBG_WARN,
    PVR_DBG_INFO,
    PVR_DBG_FUNC,
    PVR_DBG_TRACE,
} PVR_DBGMSG_LEVEL;

typedef enum
{
    PVR_DBG_MODEL_NONE   = 0x00,
    PVR_DBG_MODEL_VIDEO  = 0x01,
    PVR_DBG_MODEL_AUDIO  = 0x02,
    PVR_DBG_MODEL_SYS    = 0x04,
    PVR_DBG_MODEL_FILEIN = 0x08,
    PVR_DBG_MODEL_RECORD = 0x100,
    PVR_DBG_MODEL_CA = 0x200,
    PVR_DBG_MODEL_SWPVR = 0x300,
    PVR_DBG_MODEL_LAYER  = 0xf0f,
    PVR_DBG_HANDLE_PLAYBACK  = 0x10,
    PVR_DBG_HANDLE_RECORD    = 0x20,
    PVR_DBG_HANDLE_VIDEO     = 0x40,
    PVR_DBG_HANDLE_AUDIO     = 0x80,
    PVR_DBG_HANDLE_LAYER     = 0xf0,
    PVR_DBG_ALL              = 0xfff,
} PVR_DBGMSG_MODEL;

typedef enum
{
    EN_PVR_PLAYBACK_SPEED_32XFB    = -32000,   // -32 X
    EN_PVR_PLAYBACK_SPEED_16XFB    = -16000,   // -16 X
    EN_PVR_PLAYBACK_SPEED_8XFB     =  -8000,   // - 8 X
    EN_PVR_PLAYBACK_SPEED_4XFB     =  -4000,   // - 4 X
    EN_PVR_PLAYBACK_SPEED_2XFB     =  -2000,   // - 2 X
    EN_PVR_PLAYBACK_SPEED_1XFB     =  -1000,   // - 1 X
    EN_PVR_PLAYBACK_SPEED_0X       =      0,
    EN_PVR_PLAYBACK_SPEED_STEP_IN  =      1,   // 1 Frame
    EN_PVR_PLAYBACK_SPEED_FF_1_32X =     32,   // 1 / 32 X
    EN_PVR_PLAYBACK_SPEED_FF_1_16X =     64,   // 1 / 16 X
    EN_PVR_PLAYBACK_SPEED_FF_1_8X  =    125,   // 1 / 8  X
    EN_PVR_PLAYBACK_SPEED_FF_1_4X  =    250,   // 1 / 4  X
    EN_PVR_PLAYBACK_SPEED_FF_1_2X  =    500,   // 1 / 2  X
    EN_PVR_PLAYBACK_SPEED_1X       =   1000,   //     1  X
    EN_PVR_PLAYBACK_SPEED_2XFF     =   2000,   //     2  X
    EN_PVR_PLAYBACK_SPEED_4XFF     =   4000,   //     4  X
    EN_PVR_PLAYBACK_SPEED_8XFF     =   8000,   //     8  X
    EN_PVR_PLAYBACK_SPEED_16XFF    =  16000,   //    16  X
    EN_PVR_PLAYBACK_SPEED_32XFF    =  32000,   //    32  X
    EN_PVR_PLAYBACK_SPEED_INVALID  = 0xFFFF,
} PVR_PLAYBACK_SPEED;


#endif // #ifndef _PVR_BASE_INTERFACE_H_

///*** please do not remove or modify change list tag ***///
///***$Change: 495631 $***///
