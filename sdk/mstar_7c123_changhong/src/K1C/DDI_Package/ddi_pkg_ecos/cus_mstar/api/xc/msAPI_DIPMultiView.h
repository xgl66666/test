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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   msAPI_DIP_MultiView.h
/// @brief  msAPI_XC Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MSAPI_DIP_MULTIVIEW_H_
#define _MSAPI_DIP_MULTIVIEW_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MSAPI_XC_DIPWindow_INFO_VERSION 1
#define MSAPI_DIP_MULTIVIEW_SET_CAPTURE_INFO_VERSION 1
#define MSAPI_DIP_MULTIVIEW_CAPTURE_INFO_VERSION 1
#define MSAPI_DIP_SET_MEMORY_TYPE_INFO_VERSION 1
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Length;
}MSAPI_XC_DIPWindow_INFO;

typedef enum
{
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_16X32TILE,      //YUV420 16(w)x32(h) tiled format
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_32X16TILE,      //YUV420 32(w)x16(h) tiled format
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_32X32TILE,      //YUV420 32(w)x32(h) tiled format
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_YUYV,           //YUV422 YUYV
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_YVYU,           //YUV422 YVYU
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_UYVY,           //YUV422 UYVY
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_VYUY,           //YUV422 VYUY
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_NV12,           //YUV420 SemiPlanar
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_NV21,           //YUV420 SemiPlanar
    E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_MAX,
} EN_MSAPI_DMS_CAPTURE_COLORFORMAT;

typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    MS_U32 u32Enable;
    MS_U32 u32Visible;
    MS_U32 u32WindowID;
    MS_U32 u32Width;
    MS_U32 u32Height;
    MS_U32 u32FrameRate;
    EN_MSAPI_DMS_CAPTURE_COLORFORMAT eFmt;

    MS_PHY phyAddr;
    MS_PHY phySize;
    MS_U32 u32BufferLessMode;
}ST_MSAPI_DIP_MULTIVIEW_SET_CAPTURE_INFO;

typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    MS_PHY phyBufferYAddr;
    MS_PHY phyBufferCAddr;
    MS_U64 u64Pts;
    MS_U32 u32frameCount;
}ST_MSAPI_DIP_MULTIVIEW_CAPTURE_INFO;

typedef enum
{
    E_MSAPI_DMS_MEMORY_TYPE_XC,
    E_MSAPI_DMS_MEMORY_TYPE_GOP_DISPLAY,
    E_MSAPI_DMS_MEMORY_TYPE_MVOP_DISPLAY,
    E_MSAPI_DMS_MEMORY_TYPE_DIPDI,
    E_MSAPI_DMS_MEMORY_TYPE_FREEZE,
    E_MSAPI_DMS_MEMORY_TYPE_PQ_MISC,
    E_MSAPI_DMS_MEMORY_TYPE_DS_INFO,
    E_MSAPI_DMS_MEMORY_TYPE_MAX,
} EN_MSAPI_DMS_MEMORY_TYPE;

typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Length;

    EN_MSAPI_DMS_MEMORY_TYPE eMemoryType;
    MS_PHY phyAddr;
    MS_PHY phySize;
    MS_U32 u32CMAHeapID;
} ST_MSAPI_DIP_SET_MEMORY_TYPE;

typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Length;
    MS_U32 u32Enable;
} ST_MSAPI_DIP_SET_FREEZE_TYPE;


//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MSAPI_DIP_MULTIVIEW_MUTE_OFF      = 0,
    E_MSAPI_DIP_MULTIVIEW_MUTE_ON,
    E_MSAPI_DIP_MULTIVIEW_MUTE_ALL,
    E_MSAPI_DIP_MULTIVIEW_MUTE_MAX,
} E_MSAPI_DIP_MULTIVIEW_MUTE_ACTION;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_Init(MS_U8 u8GOP, E_MSAPI_XC_DIP_FMT eDIPWColorFmt);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_PushFrame(MSAPI_XC_VDECFRAME_INFO *VDECDispFrame);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_PullFrame(MSAPI_XC_VDECFRAME_INFO *VDECDispFrame);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_CreateWindow(MSAPI_XC_DIPWindow_INFO *pstAPI_XC_DIPWindowInfo, E_MSAPI_XC_WINDOW *pWindow);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_Finalize(E_MSAPI_XC_WINDOW eWindow);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_SetZOrder(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_U8 eZOrder);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_SetDigitalDecodeSignalInfo(MSAPI_XC_VDECFRAME_INFO *VDECDispFrame, E_MSAPI_XC_WINDOW eWindow);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_ClearDigitalDecodeSignalInfo(E_MSAPI_XC_WINDOW eWindow);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_SetMute(E_MSAPI_XC_WINDOW eWindow, E_MSAPI_DIP_MULTIVIEW_MUTE_ACTION eAction);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_Set_CaptureInfo(ST_MSAPI_DIP_MULTIVIEW_SET_CAPTURE_INFO *pstDMSCAPInfo);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_Get_CaptureBuffer(MS_U32 u32WindowID, ST_MSAPI_DIP_MULTIVIEW_CAPTURE_INFO *pstGetCapInfo);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_Release_CaptureBuffer(MS_U32 u32WindowID, ST_MSAPI_DIP_MULTIVIEW_CAPTURE_INFO *pstGetCapInfo);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_SetMemory(ST_MSAPI_DIP_SET_MEMORY_TYPE *pstDMSMemInfo);
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_SetVideoFreeze(E_MSAPI_XC_WINDOW eWindow, ST_MSAPI_DIP_SET_FREEZE_TYPE *pstDMSFreezeInfo);


//-----------------------------------------------------------------------------
/// @brief: This API is used when input change. The behavior is to invalidate the queue of window and prevent out-of-date be displayed out.
/// @param[in] eWindow, Number of display window
/// @return retun E_MSAPI_XC_RESULT
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_DIPMultiView_InvalidateWindow(E_MSAPI_XC_WINDOW eWindow);


#ifdef __cplusplus
}
#endif

#endif  // _MSAPI_DIP_MULTIVIEW_H_

