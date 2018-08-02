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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
/// @file   Demo_XC.c
/// @author MStar Semiconductor Inc.
/// @brief  Scaler setting Demo code
//
// @name Box App Command Usage
// @verbatim
//-----------------------------------------------------------------------------
#if (DEMO_XC_TEST == 1)
//-----------------------------------------------------------------------------
// 1. Init Scaler with default resolution.
// Usage: XC_Init
//
// Sample Command: XC_Init
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 2. Set Scaler Output Timing without input video(no signal setting). This Function Will Also Set Scaler Destination Window Size also.
// Usage: XC_SetOutputTiming OutTiming x y width height
//     OutTiming :
//         E_OUTPUT_TIMING_720X480_60I = 0,
//         E_OUTPUT_TIMING_720X480_60P = 1,
//         E_OUTPUT_TIMING_720X576_50I = 2,
//         E_OUTPUT_TIMING_720X576_50P = 3,
//         E_OUTPUT_TIMING_1280X720_50P = 4,
//         E_OUTPUT_TIMING_1280X720_60P = 5,
//         E_OUTPUT_TIMING_1920X1080_50I = 6,
//         E_OUTPUT_TIMING_1920X1080_60I = 7,
//         E_OUTPUT_TIMING_1920X1080_24P = 8,
//         E_OUTPUT_TIMING_1920X1080_25P = 9,
//         E_OUTPUT_TIMING_1920X1080_30P = 10,
//         E_OUTPUT_TIMING_1920X1080_50P = 11,
//         E_OUTPUT_TIMING_1920X1080_60P = 12,
//     x : HStart Position
//     y : HStart Position
//     width : Width of Display
//     height : Height of Display
//
// Sample Command: XC_SetOutputTiming 5 0 0 1280 720
//                 XC_SetOutputTiming 12 0 0 1920 1080
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 3. Init VE with default resolution.
// Usage: VE_Init
//
// Sample Command: VE_Init
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 4. Set VE Output Timing. This Function Will Also Set Scaler Destination Window Size Automatically.
// Usage: VE_SetOutputTiming OutTiming x y width height
//     OutTiming : Output Timing
//        NTSC = 0
//        PAL = 1
//     x : HStart Position
//     y : HStart Position
//     width : Width of Display
//     height : Height of Display
//
// Sample Command: VE_SetOutputTiming 0 0 0 720 480
//                 VE_SetOutputTiming 1 0 0 720 576
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 5. VE Set MV Type
//
// Usage: VE_SetMV [bEnable] [eType]
//     [bEnable]
//         - 0 :disable
//         - 1 :enable
//     [eType]
//         -   MS_VE_MV_NTSC_TYPE1 = 0
//         -   MS_VE_MV_NTSC_TYPE2 = 1
//         -   MS_VE_MV_NTSC_TYPE3 = 2
//         -   MS_VE_MV_PAL_TYPE1 = 16
//         -   MS_VE_MV_PAL_TYPE2 = 17
//         -   MS_VE_MV_PAL_TYPE3 = 18
//
// Sample Command: VE_SetMV 0 0   //DISABLE MV
//                 VE_SetMV 1 0   //ENABLE MV NTSC TYPE1
//                 VE_SetMV 0 16
//                 VE_SetMV 1 16  //ENABLE MV PAL TYPE1
//----------------------------------------------------------------------------
// @endverbatim
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include <string.h>
#include <time.h>
#include "MsOS.h"
//Api
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiACP.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#include "apiPNL_EX.h"
#include "apiXC_PCMonitor_EX.h"
#include "apiXC_Ace_EX.h"
#include "apiGOP.h"
#else
#include "apiPNL.h"
#endif
//Drv
#include "drvTVEncoder.h"
#include "drvMVOP.h"
#include "drvPQ.h"
//msAPI
#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif
#include "msAPI_VE.h"
#include "msAPI_DAC.h"

#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
#include "msAPI_GEGOP.h"
#endif
// Demo
#include "demo_utility.h"
#include "demo_xc.h"
#include "demo_av.h"
#include "demo_vdec.h"
// VBI raw data
#include "../data/VBI_ttx_raw.h"
// HDCP Key
#include "hdmi_tx_hdcp_key.h"

#if (DEMO_XC_HDMIRX_TEST == 1)
#include "hdmi_rx_hdcp_key.h"
#endif

#include "drvDTC.h"

//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
/// xc print function
#if ENABLE_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
/// XC check if init ok
#define DEMO_XC_CHECK_INITIALIZED(u32Id)                         \
    if(msAPI_XC_CheckInitStatus_EX(&g_stXC_DeviceId[u32Id]) == FALSE)  \
    {                                                               \
        printf("ERROR: execute msAPI_XC_Init First!! \n");          \
        return FALSE;                                               \
    }

/// VE check if init ok
#define DEMO_VE_CHECK_INITIALIZED                            \
    if(msAPI_VE_CheckInitStatus() == FALSE)                     \
    {                                                           \
        printf("ERROR: execute msAPI_VE_Init First!! \n");      \
        return FALSE;                                           \
    }

/// XC check device ID
#if (DEMO_XC_DUALXC_TEST == 1)
#define DEMO_XC_CHECK_DEVICE_ID(u32Id)                            \
    if ((E_MSAPI_XC_DEVICE_NUM)u32Id > E_MSAPI_XC_DEVICE1)        \
    {                                                             \
        printf("WANRING: Device ID %"DTC_MS_U32_d" exceeded \n", u32Id);      \
        return FALSE;                                             \
    }
#else
#define DEMO_XC_CHECK_DEVICE_ID(u32Id)                            \
    if ((E_MSAPI_XC_DEVICE_NUM)u32Id > E_MSAPI_XC_DEVICE0)        \
    {                                                             \
        printf("WANRING: Device ID %"DTC_MS_U32_d" exceeded \n", u32Id);      \
        return FALSE;                                             \
    }
#endif

#define VIDEO_SIZE_DEC_RATE 0.1  ///< video size decrease rate
#define VIDEO_SIZE_INC_STEP 5 ///< video size increase step
#define TEST_PAUSE_TIME 2000  ///< test pause time( us )
#ifdef MSOS_TYPE_ECOS
#define DAC_DETECTTASK_STACK_SIZE     0x2000
#define VBITTX_TASK_STACK_SIZE        0x2000
#endif
#define DEMO_VBITTX_EVT_TT_DONE       0x00000001

#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
#if (DEMO_GOP2_GOP3_TEST == 0)
#define STILL_IMG_ZAPPING_HD_GOP   0 ///< HD GOP used for still image zapping
#define STILL_IMG_ZAPPING_SD_GOP   0xFF ///< SD GOP used for still image zapping
#else
// Typically GOP0 is used for the HD UI, GOP1 is used for the SD UI and GOP2 is used for subtitle
// Therefore in still image zapping we choose to use GOP3 for HD still image and SD still image shares GOP1 with the SD UI
#define STILL_IMG_ZAPPING_HD_GOP   3 ///< HD GOP used for still image zapping
#define STILL_IMG_ZAPPING_SD_GOP   1 ///< SD GOP used for still image zapping
#endif
#endif
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MSAPI_XC_DEVICE_ID g_stXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
MS_DEMO_XC_DISP_INFO g_stXC_DispInfo[E_MSAPI_XC_DEVICE_MAX];
MS_DEMO_VE_DISP_INFO g_stVE_DispInfo;

MS_U8 *_pu8VBITTX_Buff = 0;
MS_U32 _u32VBITTX_NumOfRawData = 0;
MS_U32 _u32VBITTX_VBICnt = 0;

static MS_S32 s32VBITTXEventId = -1;
static MS_S32 s32VBITTXTaskID = -1;
static MS_U32 u32VBITTXEvents = 0;
#ifdef MSOS_TYPE_ECOS
static void* _pVBITTXTaskStack = NULL;
#endif
static MS_BOOL bExitVBITTXDemoTask = FALSE;

#if (DEMO_XC_3DVIDEO_TEST == 1)
MS_BOOL g_Demo_3DEnable = FALSE;
E_MSAPI_XC_3D_OUTPUT_MODE g_stXC_3D_OutputMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
#endif

#if (DEMO_XC_3DVIDEO2DOUT_TEST == 1)
MS_BOOL g_Demo_3DEnable = FALSE;
E_MSAPI_XC_3D_INPUT_MODE g_stXC_3D_InputMode = E_MSAPI_XC_3D_INPUT_MODE_NONE;
E_MSAPI_XC_3D_OUTPUT_MODE g_stXC_3D_OutputMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
PNL_DeviceId g_stPNL_DeviceId = {0, 0};
#endif

#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
void *_pDWinBuf = NULL;
MS_U32 u32DWinBufSize = 0;
#if (DEMO_XC_DUALXC_TEST == 1)
void *_pDipBuf[MSAPI_GEGOP_XC_DEVICE_MAX] = {NULL, NULL};
MS_U32 u32DipBufSize = 0;
#endif
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
XC_EX_INPUT_SOURCE_TYPE g_eSC1SourceType = E_XC_EX_INPUT_SOURCE_DTV;
#endif

//DAC Detect
static MS_BOOL _bVideo_DACHD_IN = TRUE;
static MS_BOOL _bVideo_DACSD_IN = TRUE;
static MS_BOOL _bEnHDDetect = FALSE;
static MS_BOOL _bEnSDDetect = FALSE;
#ifdef MSOS_TYPE_ECOS
static void*   _pDetectTaskStack = NULL;
#endif
static MS_S32  s32DetectTaskID = -1;
static MS_BOOL bExitDACDetectDemoTask = FALSE;

static MS_U8 u8DIPWindowNum = 0;
static MS_BOOL bSupportPIP = 0;

#if (DEMO_XC_HDMIRX_TEST == 1)
//******************************************************
//  HDMI Rx EDID data
//******************************************************
static MS_U8 UserDefEDID[256] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x74, 0x30, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x18, 0x0F, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78, 0x0A, 0xCF, 0x74, 0xA3, 0x57, 0x4C, 0xB0, 0x23,
    0x09, 0x48, 0x4C, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0xFF, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x20, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28,
    0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,
    0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D,
    0x53, 0x74, 0x61, 0x72, 0x20, 0x44, 0x65, 0x6D, 0x6F, 0x0A, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x3B, 0x3C, 0x1F, 0x2D, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x25,
    0x02, 0x03, 0x25, 0xF2, 0x4B, 0x05, 0x84, 0x03, 0x01, 0x12, 0x93, 0x14, 0x16, 0x07, 0x10, 0x1F,
    0x23, 0x09, 0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0x6C, 0x03, 0x0C, 0x00, 0x20, 0x00, 0xB8, 0x2D,
    0x20, 0xA0, 0x02, 0x01, 0x40, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96,
    0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xA0, 0x14, 0x51, 0xF0, 0x16, 0x00, 0x26,
    0x7C, 0x43, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x98, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E,
    0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72,
    0x1C, 0x16, 0x20, 0x10, 0x2C, 0x25, 0x80, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35,
};
#endif
//-------------------------------------------------------------------------------------------------
// Static Variable
//-------------------------------------------------------------------------------------------------
// @cond INTERNAL
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief function to get time
//------------------------------------------------------------------------------
static MS_U32 _Demo_GetTime(void)
{
    #if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
        return MsOS_GetSystemTime();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec* 1000+ ts.tv_nsec/1000000;
    #endif
}
// @endcond
#if ((DEMO_XC_3DVIDEO_TEST == 1) || (DEMO_XC_3DVIDEO2DOUT_TEST == 1))
//------------------------------------------------------------------------------
/// @brief map HDMITx and msAPI_XC output timing enum
//------------------------------------------------------------------------------
static MS_BOOL _Demo_HDMITx_Timing_Mapping(E_MSAPI_XC_OUTPUT_TIMING_TYPE eXCTiming, HDMITX_VIDEO_TIMING *eHDMITxTiming)
{
    MS_BOOL ret = TRUE;
    switch(eXCTiming)
    {
        case E_MSAPI_XC_RES_720x480I_60Hz:
            *eHDMITxTiming   = HDMITX_RES_720x480i;
            break;
        case E_MSAPI_XC_RES_720x480P_60Hz:
            *eHDMITxTiming   = HDMITX_RES_720x480p;
            break;
        case E_MSAPI_XC_RES_720x576I_50Hz:
            *eHDMITxTiming   = HDMITX_RES_720x576i;
            break;
        case E_MSAPI_XC_RES_720x576P_50Hz:
            *eHDMITxTiming   = HDMITX_RES_720x576p;
            break;
        case E_MSAPI_XC_RES_1280x720P_50Hz:
            *eHDMITxTiming   = HDMITX_RES_1280x720p_50Hz;
            break;
        case  E_MSAPI_XC_RES_1280x720P_60Hz:
            *eHDMITxTiming   = HDMITX_RES_1280x720p_60Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080I_50Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080i_50Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080I_60Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080i_60Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080P_24Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080p_24Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080P_25Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080p_25Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080P_30Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080p_30Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080P_50Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080p_50Hz;
            break;
        case E_MSAPI_XC_RES_1920x1080P_60Hz:
            *eHDMITxTiming   = HDMITX_RES_1920x1080p_60Hz;
            break;
        case E_MSAPI_XC_RES_640x480P_60Hz:
            *eHDMITxTiming   = HDMITX_RES_640x480p;
            break;
        default:
            ret = FALSE;
            db_print("No HDMITx timing mapping for msAPI_XC timing: %d\n", eXCTiming);
            break;
    }
    return ret;
}

//------------------------------------------------------------------------------
/// @brief map HDMITx and msAPI_XC 3D enum
//------------------------------------------------------------------------------
static MS_BOOL _Demo_HDMITx_3D_Mapping(E_MSAPI_XC_3D_OUTPUT_MODE eXC3DMode, HDMITX_VIDEO_3D_STRUCTURE *eHDMITx3DMode)
{
    MS_BOOL ret = TRUE;
    switch(eXC3DMode)
    {
    case E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_TopandBottom;
        break;
    case E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_FramePacking;
        break;
    case E_MSAPI_XC_3D_OUTPUT_LINE_ALTERNATIVE:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_LineAlternative;
        break;
    case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_SidebySide_Half;
        break;
    case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_FULL:
        *eHDMITx3DMode = HDMITX_VIDEO_3D_SidebySide_FULL;
        break;
    case E_MSAPI_XC_3D_OUTPUT_MODE_NONE:
        *eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;
        break;
    default:
        ret = FALSE;
        db_print("No HDMITx 3D mode mapping for XC 3D Mode: %d\n", eXC3DMode);
        break;
    }
    return ret;
}
#endif

//------------------------------------------------------------------------------
/// @brief map DDI and XC input source
//------------------------------------------------------------------------------
static MS_BOOL _Demo_XC_InputSource_Mapping(E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc, E_MSAPI_XC_INPUT_SRC *eXCInputSrc)
{
    MS_BOOL ret = TRUE;
    switch(eDDIXCInputSrc)
    {
    case E_DDI_XC_INPUT_SOURCE_DTV:
        *eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_DTV;
        break;
    case E_DDI_XC_INPUT_SOURCE_DTV2:
        *eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_DTV2;
        break;
    case E_DDI_XC_INPUT_SOURCE_HDMI:
        *eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_HDMI;
        break;
    case E_DDI_XC_INPUT_SOURCE_SCALER0_OP:
        *eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_SCALER_OP;
        break;
    default:
        ret = FALSE;
        db_print("Invalid input parameter eDDIXCInputSrc %d\n", eDDIXCInputSrc);
        break;
    }
    return ret;
}

//------------------------------------------------------------------------------
/// @brief map DDI and msAPI_XC aspect ratio
//------------------------------------------------------------------------------
static MS_BOOL _Demo_XC_AspectRatio_Mapping(E_ASPECT_RATIO_TYPE eDDIXCAs, E_MSAPI_XC_ASPECT_RATIO_TYPE *eMSAPIXCAs)
{
    MS_BOOL ret = TRUE;

    switch(eDDIXCAs)
    {
        case E_ASPECT_RATIO_NORMAL:
            *eMSAPIXCAs = E_MSAPI_XC_ASPECT_RATIO_NORMAL;
            break;
        case E_ASPECT_RATIO_4_3:
            *eMSAPIXCAs = E_MSAPI_XC_ASPECT_RATIO_MSTK_LB_4_3;
            break;
        case E_ASPECT_RATIO_16_9:
            *eMSAPIXCAs = E_MSAPI_XC_ASPECT_RATIO_MSTK_PB_16_9;
            break;
        case E_ASPECT_RATIO_CUT_4_3:
            *eMSAPIXCAs = E_MSAPI_XC_ASPECT_RATIO_MSTK_PS_4_3;
            break;
        case E_ASPECT_RATIO_CUT_16_9:
            *eMSAPIXCAs = E_MSAPI_XC_ASPECT_RATIO_MSTK_PS_16_9;
            break;
        case E_ASPECT_RATIO_FULL:
        default:
            *eMSAPIXCAs = E_MSAPI_XC_ASPECT_RATIO_FULL;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// @brief map DDI and msAPI_XC Output Timing
//------------------------------------------------------------------------------
static MS_BOOL _Demo_XC_OutputTiming_Mapping(E_OUTPUT_TIMING_TYPE eDDIXCOutputTiming, E_MSAPI_XC_OUTPUT_TIMING_TYPE *eMSAPIXCOutputTiming)
{
    switch (eDDIXCOutputTiming)
    {
        case E_OUTPUT_TIMING_640X480_60P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_640x480P_60Hz;
            break;
        case E_OUTPUT_TIMING_720X480_60I:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_720x480I_60Hz;
            break;
        case E_OUTPUT_TIMING_720X480_60P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_720x480P_60Hz;
            break;
        case E_OUTPUT_TIMING_720X576_50I:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_720x576I_50Hz;
            break;
        case E_OUTPUT_TIMING_720X576_50P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_720x576P_50Hz;
            break;
#if (DEMO_HD_TEST == 1)
        case E_OUTPUT_TIMING_1280X720_50P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1280x720P_50Hz;
            break;
        case E_OUTPUT_TIMING_1280X720_60P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1280x720P_60Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_50I:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080I_50Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_60I:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080I_60Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_24P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080P_24Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_25P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080P_25Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_30P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080P_30Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_50P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080P_50Hz;
            break;
        case E_OUTPUT_TIMING_1920X1080_60P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_1920x1080P_60Hz;
            break;
#if (DEMO_4K2K_TEST == 1)
        case E_OUTPUT_TIMING_3840X2160_24P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_3840x2160P_24Hz;
            break;
        case E_OUTPUT_TIMING_3840X2160_25P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_3840x2160P_25Hz;
            break;
        case E_OUTPUT_TIMING_3840X2160_30P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_3840x2160P_30Hz;
            break;
        case E_OUTPUT_TIMING_3840X2160_50P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_3840x2160P_50Hz;
            break;
        case E_OUTPUT_TIMING_3840X2160_60P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_3840x2160P_60Hz;
            break;
        case E_OUTPUT_TIMING_4096X2160_24P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_4096x2160P_24Hz;
            break;
        case E_OUTPUT_TIMING_4096X2160_25P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_4096x2160P_25Hz;
            break;
        case E_OUTPUT_TIMING_4096X2160_30P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_4096x2160P_30Hz;
            break;
        case E_OUTPUT_TIMING_4096X2160_50P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_4096x2160P_50Hz;
            break;
        case E_OUTPUT_TIMING_4096X2160_60P:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_4096x2160P_60Hz;
            break;
#endif
#endif
        default:
            *eMSAPIXCOutputTiming = E_MSAPI_XC_RES_720x480I_60Hz;
            printf("OutputTiming not supported, switch to 720x480i 60Hz\n");
            break;
    }
    return TRUE;
}

static void _Demo_XC_SetPanelID(MS_U32 u32XCDevice)
{
#if (DEMO_XC_DUALXC_TEST == 1)
    g_stPNL_DeviceId.u32Version = g_stXC_DeviceId[u32XCDevice].u32Version;
    g_stPNL_DeviceId.u32Id = g_stXC_DeviceId[u32XCDevice].u32Id;
#else
    UNUSED(u32XCDevice);
#endif
}

static MS_BOOL _Demo_XC_SetMVOPConfig(EN_VDEC_Device eVDECDevice)
{
    VDEC_EX_DispInfo stDispinfo;
    MSAPI_XC_VDEC_DispInfo stMSAPIDispinfo;

    memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
    memset(&stMSAPIDispinfo, 0, sizeof(MSAPI_XC_VDEC_DispInfo));

    /////////////////////////
    //get video information//
    /////////////////////////
    if (Demo_VDEC_GetDispInfo(&eVDECDevice,&stDispinfo) == FALSE)
    {
        printf("Demo_VDEC_GetDispInfo fail!\n");
        return FALSE;
    }

    Demo_XC_VdecDispInfo2msAPIVdecDispInfo(&stDispinfo, &stMSAPIDispinfo);

    msAPI_XC_SetVDECInfo_EX_ByPath((E_MSAPI_XC_DECODER_PATH)eVDECDevice, stMSAPIDispinfo);

    return TRUE;
}

//----------------------------------------------------------------------------------
/// @brief function to calculate aspect ratio of VE  \n
//----------------------------------------------------------------------------------
void _Demo_VE_CalculateAspectRatio(E_ASPECT_RATIO_TYPE enVideoScreen,
                                       MS_VE_WINDOW_TYPE *pstCapWin,
                                       MS_VE_WINDOW_TYPE *pstDstWin)
{
    MS_U16 u16Temp;
    MS_U32 u32VEStd;
    VE_DrvStatus veStatus;

    MDrv_VE_GetStatus(&veStatus);

    if(veStatus.VideoSystem <= MS_VE_NTSC_J)
        u32VEStd = 0;
    else
        u32VEStd = 1;

    if((pstDstWin->width == 0) || (pstDstWin->height== 0))
    {
        pstDstWin->width = 720;
        pstDstWin->height= (u32VEStd)? 576 : 480;
    }

    switch( enVideoScreen )
    {
        case E_ASPECT_RATIO_16_9:
            {
                db_print("Aspect Ratio Type: E_ASPECT_RATIO_16_9\n");
                u16Temp = (U32)pstDstWin->height * 16 / 9;
                if (u16Temp <= pstDstWin->width) // H:V >= 16:9
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstDstWin->width * 9 / 16;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;
        case E_ASPECT_RATIO_4_3:
            {
                db_print("Aspect Ratio Type: E_ASPECT_RATIO_4_3\n");
                u16Temp = (U32)pstDstWin->height * 4 / 3;
                if (u16Temp <= pstDstWin->width) // H:V >= 4:3
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 4:3
                {
                    u16Temp = (U32)pstDstWin->width * 3 / 4;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;

        case E_ASPECT_RATIO_FULL:
        default:
            {
                db_print("Aspect Ratio Type: E_ASPECT_RATIO_FULL\n");
                pstDstWin->x = 0;
                pstDstWin->y = 0;
            }
            break;

        case E_ASPECT_RATIO_NORMAL:
            db_print("Aspect Ratio Type: E_ASPECT_RATIO_NORMAL\n");
            break;
        case E_ASPECT_RATIO_CUT_4_3: // cut off content to reserve aspect ratio
            db_print("Aspect Ratio Type: E_ASPECT_RATIO_CUT_4_3\n");
            //capture window
            //if((pstDstWin->width != 0) || (pstDstWin->height != 0))
            {
                if((pstCapWin->width == 0) || (pstCapWin->height== 0))
                {
                    pstCapWin->width = IPANEL(&g_stPNL_DeviceId, Width);
                    pstCapWin->height = IPANEL(&g_stPNL_DeviceId, Height);
                }

                db_print("pstCapWin->width is %d\n", pstCapWin->width);
                db_print("pstCapWin->height is %d\n", pstCapWin->height);

                u16Temp = (U32)pstCapWin->height * pstDstWin->width / pstDstWin->height;
                if (u16Temp <= pstCapWin->width) // H:V >= 4:3
                {
                    pstCapWin->x += (pstCapWin->width - u16Temp) / 2;
                    pstCapWin->width = u16Temp;
                }
                else // H:V <= 4:3
                {
                    u16Temp = (U32)pstCapWin->width * pstDstWin->height / pstDstWin->width;
                    pstCapWin->y += (pstCapWin->height - u16Temp) / 2;
                    pstCapWin->height = u16Temp;
                }

                db_print("pstCapWin->width is %d\n", pstCapWin->width);
                db_print("pstCapWin->height is %d\n", pstCapWin->height);
            }
            break;
        case E_ASPECT_RATIO_CUT_16_9:
            db_print("Aspect Ratio Type: E_ASPECT_RATIO_CUT_16_9\n");
            //capture window
            {
                if((pstCapWin->width == 0) || (pstCapWin->height== 0))
                {
                    pstCapWin->width = IPANEL(&g_stPNL_DeviceId, Width);
                    pstCapWin->height = IPANEL(&g_stPNL_DeviceId, Height);
                }

                u16Temp = (U32)pstCapWin->height * 16 / 9;
                if (u16Temp <= pstCapWin->width) // H:V >= 16:9
                {
                    pstCapWin->x += (pstCapWin->width - u16Temp) / 2;
                    pstCapWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstCapWin->width * 9 / 16;
                    pstCapWin->y += (pstCapWin->height - u16Temp) / 2;
                    pstCapWin->height = u16Temp;
                }
            }
            //display window
            {
                u16Temp = (U32)pstDstWin->height * 16 / 9;
                if (u16Temp <= pstDstWin->width) // H:V >= 16:9
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstDstWin->width * 9 / 16;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;

    }

}


//----------------------------------------------------------------------------
/// @brief to check whether the display windows are valid
//----------------------------------------------------------------------------
MS_BOOL _Demo_Check_DispWin_IsValid(MS_U32 u32XCDevice, MS_DEMO_XC_DISP_INFO *p_Demo_DispInfo, MS_U32 u32ScalerWindow)
{
    _Demo_XC_SetPanelID(u32XCDevice);

    if((p_Demo_DispInfo->stDestWin[u32ScalerWindow].width == 0)||(p_Demo_DispInfo->stDestWin[u32ScalerWindow].height == 0))
    {
        p_Demo_DispInfo->stDestWin[u32ScalerWindow].x = 0;
        p_Demo_DispInfo->stDestWin[u32ScalerWindow].x = 0;
        p_Demo_DispInfo->stDestWin[u32ScalerWindow].width= IPANEL(&g_stPNL_DeviceId, Width);
        p_Demo_DispInfo->stDestWin[u32ScalerWindow].height= IPANEL(&g_stPNL_DeviceId, Height);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief function to tune on/off DAC voltage
//------------------------------------------------------------------------------
static void _SetDACFullVolt(MS_BOOL bFullOut, MS_BOOL bHDDAC)
{
    MApi_DAC_SetIHalfOutput(!bFullOut, bHDDAC);
    MApi_DAC_SetQuartOutput(!bFullOut, bHDDAC);
}

//------------------------------------------------------------------------------
/// @brief function to detectDAC state
//------------------------------------------------------------------------------
void _Demo_Video_DACDetect(MS_BOOL OnlySDDac)
{
    MS_BOOL bIsPlugIn;

    db_print("OnlySDDac %d!\n",OnlySDDac);

    while(bExitDACDetectDemoTask == FALSE)
    {
        if(OnlySDDac)  //if HD DAC is not using YPbPr, Using only SD for control HD&SD DAC
        {
            if(_bVideo_DACSD_IN)  //Detect Plug Out
            {
                _SetDACFullVolt(TRUE, TRUE);
                _SetDACFullVolt(TRUE, FALSE);
                MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_LOW, TRUE); //detect need 1.0V voltage
                MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGOUT, &bIsPlugIn);

                if(!bIsPlugIn)
                {
                    _SetDACFullVolt(FALSE, TRUE);
                    _SetDACFullVolt(FALSE, FALSE);
                }
            }
            else //Detect Plug In
            {
                _SetDACFullVolt(FALSE, TRUE);
                _SetDACFullVolt(FALSE, FALSE);
                MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, TRUE); //detect need 1.0V voltage
                MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGIN, &bIsPlugIn);

                if(bIsPlugIn)
                {
                    _SetDACFullVolt(TRUE, TRUE);
                    _SetDACFullVolt(TRUE, FALSE);
                }
            }

            _bVideo_DACSD_IN = bIsPlugIn;
            db_print("_bVideo_DACSD_IN %d!\n",_bVideo_DACSD_IN);
        }
        else  // HD DAC is using YPbPr
        {
            if(_bEnSDDetect)
            {
                if(_bVideo_DACSD_IN)  //Detect Plug Out
                {
                    _SetDACFullVolt(TRUE, FALSE);
                    MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGOUT, &bIsPlugIn);

                    if(!bIsPlugIn)
                    {
                       _SetDACFullVolt(FALSE, FALSE);
                    }
                }
                else  //Detect Plug In
                {
                    _SetDACFullVolt(FALSE, FALSE);
                    MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGIN, &bIsPlugIn);

                    if(bIsPlugIn)
                    {
                        _SetDACFullVolt(TRUE, FALSE);
                    }
                }

                _bVideo_DACSD_IN = bIsPlugIn;

                db_print("_bVideo_DACSD_IN %d!\n",_bVideo_DACSD_IN);
            }

            if(_bEnHDDetect)
            {
                if(_bVideo_DACHD_IN)  //Detect Plug Out
                {
                    _SetDACFullVolt(TRUE, TRUE);
                    MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_LOW, TRUE); //detect need 1.0V voltage
                    MApi_DAC_HotPlugDetect(E_DAC_DETECT_HD, E_DAC_DETECT_PLUGOUT, &bIsPlugIn);

                    if(!bIsPlugIn)
                    {
                        _SetDACFullVolt(FALSE, TRUE);
                    }
                }
                else  //Detect Plug In
                {
                    _SetDACFullVolt(FALSE, TRUE);
                    MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, TRUE); //detect need 1.0V voltage
                    MApi_DAC_HotPlugDetect(E_DAC_DETECT_HD, E_DAC_DETECT_PLUGIN, &bIsPlugIn);

                    if(bIsPlugIn)
                    {
                        _SetDACFullVolt(TRUE, TRUE);
                    }
                }

                _bVideo_DACHD_IN = bIsPlugIn;

                db_print("_bVideo_DACHD_IN %d!\n",_bVideo_DACHD_IN);
            }
        }

        MsOS_DelayTask(10);
    }
}


//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief The sample code to initialize SW/HW of SCALER, DAC and HDMITx/VE
/// @param[in] *pu32XCDevice the XC Device
///     -    HD path (HDMI)     = 0,
///     -    SD path (CVBS)     = 1
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_Init  0\n
///          \b XC_Init  1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_Init(MS_U32 *pu32XCDevice)
{
    MSAPI_XC_INITDATA msAPI_XC_InitData;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    // Global Parameter Initialization
    memset(&g_stXC_DispInfo[*pu32XCDevice], 0, sizeof(MS_DEMO_XC_DISP_INFO));

    // Parameter Initialization
    memset(&msAPI_XC_InitData, 0, sizeof(MSAPI_XC_INITDATA));

    if(*pu32XCDevice == E_MSAPI_XC_DEVICE0)
    {
        MSAPI_XC_HDMITX_BOARD_INFO stHDMITxInfo =
        {
            HDMITX_ANALOG_TUNING_SD,
            HDMITX_ANALOG_TUNING_HD,
            HDMITX_ANALOG_TUNING_DEEP_HD,
        };
#if (DEMO_HD_TEST == 1)
        msAPI_XC_InitData.eTimingType = E_MSAPI_XC_RES_1920x1080I_50Hz;
#else
        msAPI_XC_InitData.eTimingType = E_MSAPI_XC_RES_720x576I_50Hz;
#endif
        msAPI_XC_InitData.u32XCMemAddress = MEM_ADR_BY_MIU(SC0_MAIN_FB_ADR, SC0_MAIN_FB_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCMemSize = SC0_MAIN_FB_LEN;
        msAPI_XC_InitData.u32XCSubMemAddress = MEM_ADR_BY_MIU(SC0_SUB_FB_ADR, SC0_SUB_FB_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCSubMemSize = SC0_SUB_FB_LEN;

        msAPI_XC_InitData.u32XCMenuLoadAddress = MEM_ADR_BY_MIU(SC0_MENULOAD_BUF_ADR, SC0_MENULOAD_BUF_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCMenuLoadSize = SC0_MENULOAD_BUF_LEN;

        //For Bandwidth Table selection and to MIU configuration
#if (ENABLE_MIU_1)
        msAPI_XC_InitData.eBWTBLType= E_MSAPI_XC_BW_TBL_MIU_128X128;
#else
        msAPI_XC_InitData.eBWTBLType= E_MSAPI_XC_BW_TBL_MIU_128;
#endif

        //For HDMITx Configuration
        msAPI_XC_InitData.u16HdmitxHpdPin = HDMITX_HPD_PM_GPIO;
        msAPI_XC_InitData.eHdmitxOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_HDMI;
        msAPI_XC_InitData.eHdmitxCDType = E_MSAPI_XC_HDMITX_CD_8BITS;
        msAPI_XC_InitData.stHdmitxHdcpKeyInfo.pu8HdmitxHdcpKey = HDMI_TX_HDCP_KEY;
        msAPI_XC_InitData.stHdmitxHdcpKeyInfo.u16HdmitxHdcpKeySize = sizeof(HDMI_TX_HDCP_KEY);

        memcpy(&msAPI_XC_InitData.stHdmitxBoardInfo, &stHDMITxInfo, sizeof(MSAPI_XC_HDMITX_BOARD_INFO));

        //For Output Control
        msAPI_XC_InitData.bDisableHDMI = FALSE;
        msAPI_XC_InitData.bDisableDacHD = FALSE;
        msAPI_XC_InitData.bDisableDacSD = TRUE;
#if (DEMO_XC_HDMIRX_TEST == 1)
        //For HDMI Rx EDID Information
        msAPI_XC_InitData.eHdmirxEDIDInfo.eHdmirxEDIDType = EDID_TYPE;
        if(EDID_CUSTOMER == EDID_TYPE)
        {
            msAPI_XC_InitData.eHdmirxEDIDInfo.UserDefEDID = UserDefEDID;
            msAPI_XC_InitData.eHdmirxEDIDInfo.u16UserDefEDIDSize = sizeof(UserDefEDID);
        }
        msAPI_XC_InitData.stHdmirxHdcpKeyInfo.pu8HdmirxHdcpKey = HDMI_RX_HDCP_KEY;
        msAPI_XC_InitData.stHdmirxHdcpKeyInfo.u16HdmirxHdcpKeySize = sizeof(HDMI_RX_HDCP_KEY);
#endif
    }
    else if(*pu32XCDevice == E_MSAPI_XC_DEVICE1)
    {
        msAPI_XC_InitData.eTimingType = E_MSAPI_XC_RES_720x576I_50Hz;

#if (DEMO_VE_FRAME_BUF_SUP_TEST == 1)
        msAPI_XC_InitData.u32XCMemAddress = MEM_ADR_BY_MIU(VE_FRAME_BUF_ADR, VE_FRAME_BUF_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCMemSize = VE_FRAME_BUF_LEN;
        msAPI_XC_InitData.u32XCSubMemAddress = 0;
        msAPI_XC_InitData.u32XCSubMemSize = 0;

        msAPI_XC_InitData.u32XCMenuLoadAddress = 0;
        msAPI_XC_InitData.u32XCMenuLoadSize = 0;
#else
        msAPI_XC_InitData.u32XCMemAddress = MEM_ADR_BY_MIU(SC1_MAIN_FB_ADR, SC1_MAIN_FB_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCMemSize = SC1_MAIN_FB_LEN;
        msAPI_XC_InitData.u32XCSubMemAddress = MEM_ADR_BY_MIU(SC1_MAIN_FB_ADR, SC1_MAIN_FB_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCSubMemSize = SC1_MAIN_FB_LEN;

        msAPI_XC_InitData.u32XCMenuLoadAddress = MEM_ADR_BY_MIU(SC1_MENULOAD_BUF_ADR, SC1_MENULOAD_BUF_MEMORY_TYPE);
        msAPI_XC_InitData.u32XCMenuLoadSize = SC1_MENULOAD_BUF_LEN;
#endif
        //For Output Control
        msAPI_XC_InitData.bDisableHDMI = FALSE;
        msAPI_XC_InitData.bDisableDacHD = FALSE;
        msAPI_XC_InitData.bDisableDacSD = TRUE;
    }
    else
    {
        printf("[%s][%d] Wrong XC device number!\n", __FUNCTION__, __LINE__);
    }

    msAPI_XC_InitData.s32CachedPoolID = s32MstarCachedPoolID;
#if (MBOOT_LOGO_ENABLE)
    msAPI_XC_InitData.bBootLogoEnable = TRUE;
#else
    msAPI_XC_InitData.bBootLogoEnable = FALSE;
#endif

    //For XC Menu Load, this function to switch XC mode after V-Sync
    msAPI_XC_InitData.bMenuLoadEnable = TRUE;

    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_PQ);
    msAPI_XC_Init_EX(&g_stXC_DeviceId[*pu32XCDevice], msAPI_XC_InitData);

#if (DEMO_DAC_SD_ONLY_TEST == 1)
    MApi_DAC_Enable(FALSE, TRUE);
#endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change output resolution of HD/SD display path
/// @param[in] *pu32XCDevice the XC Device
///     -    HD path (HDMI)     = 0,
///     -    SD path (CVBS)     = 1
/// @param[in] *pu32OutputTiming the output resolution of enum E_OUTPUT_TIMING_TYPE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetOutputTiming 0 6\n
///          \b XC_SetOutputTiming 1 1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetOutputTiming(MS_U32 *pu32XCDevice, MS_U32 *pu32OutputTiming)
{
    E_OUTPUT_TIMING_TYPE u32Res = *pu32OutputTiming;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eResType;

    DEMO_XC_CHECK_INITIALIZED(*pu32XCDevice)
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    _Demo_XC_OutputTiming_Mapping(u32Res, &eResType);

    //[Disable output misc] to avoid garbage line
    if(*pu32XCDevice == E_MSAPI_XC_DEVICE0)
    {
        msAPI_XC_EnableMiscOutput(DISABLE);
#if (DEMO_XC_DUALXC_TEST == 0)
        msAPI_VE_EnableMiscOutput(DISABLE);
#else
#if (DEMO_XC_DUALXC_IDENTICAL_DISPLAY_TEST == 1)
        // Set black screen on CVBS output if its input is from HD path
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[E_MSAPI_XC_DEVICE1], ENABLE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
#endif
#endif
    }
    else
    {
        msAPI_VE_EnableMiscOutput(DISABLE);
    }

    //Reset XC Timing
    msAPI_XC_ChangeOutputResolution_EX(&g_stXC_DeviceId[*pu32XCDevice], eResType);

    //Set Black Screen (no video)
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
    if(*pu32XCDevice == E_MSAPI_XC_DEVICE0)
    {
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, E_MSAPI_XC_SUB_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
    }

    //[Enable output misc]
    if(*pu32XCDevice == E_MSAPI_XC_DEVICE0)
    {
        msAPI_XC_EnableMiscOutput(ENABLE);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize SW/HW of Video Endcoder which is the main component of SD display path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_Init(void)
{
    MSAPI_VE_INITDATA msAPI_VE_InitData;

    // Global Parameter Initialization
    memset(&g_stVE_DispInfo, 0, sizeof(MS_DEMO_VE_DISP_INFO));

    // VE init
    memset(&msAPI_VE_InitData, 0, sizeof(MSAPI_VE_INITDATA));
    msAPI_VE_InitData.u32VEMemAddress = VE_FRAME_BUF_ADR;
    msAPI_VE_InitData.u32VEMemSize= VE_FRAME_BUF_LEN;
    msAPI_VE_InitData.eOutputVideoStd = MS_VE_PAL;
    msAPI_VE_InitData.eInputSrcType = MS_VE_SRC_SCALER;
    msAPI_VE_InitData.eOutputDstType = MS_VE_DEST_CVBS;

    //For Output Control
    msAPI_VE_InitData.bEnableDacSD = TRUE;

#if (MBOOT_LOGO_ENABLE)
    msAPI_VE_InitData.bBootLogoEnable = TRUE;
#else
    msAPI_VE_InitData.bBootLogoEnable = FALSE;
#endif

    msAPI_VE_Init(msAPI_VE_InitData);


    //msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_DRV);
    //turn off SD OSD at initial step and turn on it after GOP is initialized
    MDrv_VE_SetOSD(FALSE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change output video standard of SD display path
/// @param[in] *pu32OutputTiming the output resolution of enum MS_VE_VIDEOSYS
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetOutputTiming 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetOutputTiming(MS_U32 *pu32OutputTiming)
{
    MS_U32 u32Res = *pu32OutputTiming;

    DEMO_VE_CHECK_INITIALIZED

    //[Disable output misc] to avoid garbage line
    msAPI_VE_EnableMiscOutput(DISABLE);

    switch(u32Res)
    {
        case 0:
            msAPI_VE_SetOutputVideoStd(MS_VE_NTSC);
            break;
        case 1:
            msAPI_VE_SetOutputVideoStd(MS_VE_PAL);
            break;
        case 2:
            msAPI_VE_SetOutputVideoStd(MS_VE_SECAM);
            break;
        default:
            msAPI_VE_SetOutputVideoStd(MS_VE_PAL);
            break;
    }

    msAPI_VE_SetMode();
    msAPI_VE_SetVideoMute(ENABLE);

    //[Enable output misc]
    msAPI_VE_EnableMiscOutput(ENABLE);

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Set DACDetect task \n
/// @param[in] *OnlySDDac 1=SD Dac Only, 0=HD&SD Dac Only
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_DACDetect 0 \n
///          \b Video_DACDetect 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Video_DACDetect(MS_BOOL *OnlySDDac, MS_BOOL *EnableSDDetect, MS_BOOL *EnableHDDetect)
{
    //Detect function need use triming IC !!!
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(s32DetectTaskID == -1)
    {
#ifdef MSOS_TYPE_ECOS
        _pDetectTaskStack = MsOS_AllocateMemory(DAC_DETECTTASK_STACK_SIZE, s32MstarNonCachedPoolID);

        if(_pDetectTaskStack == NULL)
        {
            db_print("Task stack create fail!\n");
            return FALSE;
        }

       s32DetectTaskID = MsOS_CreateTask((TaskEntry)_Demo_Video_DACDetect,
                                        (MS_BOOL)*OnlySDDac,
                                        E_TASK_PRI_HIGH,
                                        TRUE,
                                        _pDetectTaskStack,
                                        DAC_DETECTTASK_STACK_SIZE,
                                        "DAC_Detect_Task");
#else  //linux
       s32DetectTaskID = MsOS_CreateTask((TaskEntry)_Demo_Video_DACDetect,
                                        (MS_BOOL)*OnlySDDac,
                                        E_TASK_PRI_HIGH,
                                        TRUE,
                                        NULL,
                                        0,
                                        "DAC_Detect_Task");
#endif

        if(s32DetectTaskID == -1)
        {
            db_print("Task create fail!\n");
            return FALSE;
        }

        db_print("DetectTask Create %"DTC_MS_U32_d"!\n", s32DetectTaskID);

        _bEnHDDetect = *EnableHDDetect;
        _bEnSDDetect = *EnableSDDetect;

        return TRUE;
    }
    else
    {
        db_print("Task Alread Create!\n");
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief Enable/disable selected HD/SD output destination:
/// @param[in] *pu32OutputDest Output Destination Selection of E_OUTPUT_DESTINTATION
///     -    OUTPUT_DESTINATION_SD_CVBS      = 0,
///     -    OUTPUT_DESTINATION_SD_SVIDEO    = 1,
///     -    OUTPUT_DESTINATION_SD_SCART_RGB = 2,
///     -    OUTPUT_DESTINATION_SD_YPBPR     = 3,
///     -    OUTPUT_DESTINATION_HD_COMPONENT = 4,
///     -    OUTPUT_DESTINATION_HD_HDMITX    = 5
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetOutputDest 4 0 \n
///          \b XC_SetOutputDest 4 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetOutputDest(MS_U32 *pu32OutputDest, MS_U32 *pu32Enable)
{
    MS_Switch_VE_Dest_Info DestInfo;

    memset(&DestInfo, 0, sizeof(MS_Switch_VE_Dest_Info));

    switch(*pu32OutputDest)
    {
        case E_DDI_OUTPUT_DEST_HD_COMPONENT:
            msAPI_DAC_SetOutputSource(E_MSAPI_DAC_OUTPUT_YPBPR);
            MApi_DAC_Enable(*pu32Enable, TRUE);
            break;
        case E_DDI_OUTPUT_DEST_HD_HDMITX:
            MApi_HDMITx_SetTMDSOnOff(*pu32Enable);
            if(*pu32Enable)
            {
                MApi_HDMITx_Exhibit();
            }
            break;
        case E_DDI_OUTPUT_DEST_SD_CVBS:
            msAPI_DAC_SetOutputSource(E_MSAPI_DAC_OUTPUT_CVBS);
            MApi_DAC_Enable(*pu32Enable, FALSE);
            break;
        case E_DDI_OUTPUT_DEST_SD_SVIDEO:
            DestInfo.OutputDstType = MS_VE_DEST_CVBS;
            MDrv_VE_SwitchOuputDest(&DestInfo);
            msAPI_DAC_SetOutputSource(E_MSAPI_DAC_OUTPUT_SVIDEO);
            MApi_DAC_Enable(*pu32Enable, FALSE); // Kaiser S-Video use SD DAC
            break;
        case E_DDI_OUTPUT_DEST_SD_SCART_RGB:
            DestInfo.OutputDstType = MS_VE_DEST_SCART;
            MDrv_VE_SwitchOuputDest(&DestInfo);
            msAPI_DAC_SetOutputSource(E_MSAPI_DAC_OUTPUT_SCART);
            MApi_DAC_Enable(*pu32Enable, TRUE);
            break;
        case E_DDI_OUTPUT_DEST_SD_YPBPR:
            DestInfo.OutputDstType = MS_VE_DEST_YPBPR;
            MDrv_VE_SwitchOuputDest(&DestInfo);
            MApi_DAC_SetOutputSource(E_DAC_TO_VE_YPBPR, TRUE);
            MApi_DAC_SetOutputSwapSel(E_DAC_R_B_G, TRUE);
            MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_LOW, TRUE);
            MApi_DAC_Enable(*pu32Enable, TRUE);
            break;
        default:
            printf("Invalid output destination!\n");
            return FALSE;
    }

    return TRUE;
}

MS_BOOL Demo_XC_PlayWindow(MS_U8 *pu8WinNum)
{
    u8DIPWindowNum = *pu8WinNum;
    msAPI_XC_SetWinNum(u8DIPWindowNum);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Play video using default setting (default size, default aspect ratio)
///         -   HD: DTV/HDMIRx -> SC0 -> HDMITx/Component
///         -   SD: DTV/HDMIRx -> SC1 -> VE Encoder
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32XCInputSrc the XC Input source type 0:DTV, 1:DTV2, 2:HDMIRx, 3:SC0 OP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_PlayVideo 0 0 1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_PlayVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc = (E_DDI_XC_INPUT_SOURCE_TYPE)(*pu32XCInputSrc);
    E_MSAPI_XC_INPUT_SRC eXCInputSrc = 0;
    MS_BOOL bIsSeamlessZapping = FALSE;

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    if(u8DIPWindowNum > 2)// DIP * 4
    {
        bSupportPIP = FALSE;
        MSAPI_XC_WINDOW_TYPE stDispWin = {0, 0, (IPANEL(&g_stPNL_DeviceId, Width)/3), (IPANEL(&g_stPNL_DeviceId, Height)/3)};
        if(*pu32XCWindow == E_MSAPI_XC_MAIN_WINDOW)
        {
            msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);
        }
        else if(*pu32XCWindow == E_MSAPI_XC_SUB_WINDOW)
        {
            msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);
        }
        else if(*pu32XCWindow == E_MSAPI_XC_SUB2_WINDOW)
        {
            msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);
        }
        else if(*pu32XCWindow == E_MSAPI_XC_SUB3_WINDOW)
        {
            msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if(*pu32XCWindow == E_MSAPI_XC_MAIN_WINDOW)
        {
            bSupportPIP = TRUE;
        }

        if(*pu32XCWindow == E_MSAPI_XC_SUB_WINDOW)
        {
            bSupportPIP = TRUE;
#if( DEMO_XC_DIPMultiWin == 1)
            bSupportPIP = FALSE;
#endif
        }
    }

    if(bSupportPIP)
    {
        if (eDDIXCInputSrc == E_DDI_XC_INPUT_SOURCE_DTV)
        {
            if( TRUE != _Demo_XC_SetMVOPConfig(E_VDEC_DEVICE_MAIN))
            {
                db_print("Main stream video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
                return FALSE;
            }
        }
        else if (eDDIXCInputSrc == E_DDI_XC_INPUT_SOURCE_DTV2)
        {
            if( TRUE != _Demo_XC_SetMVOPConfig(E_VDEC_DEVICE_SUB))
            {
                db_print("Sub stream video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
                return FALSE;
            }
        }

#if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
        if (Demo_XC_SeamlessZapping_IsFreeze(pu32XCDevice, pu32XCWindow) == TRUE)
        {
            bIsSeamlessZapping = TRUE;
        }
#endif
#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
#if (DEMO_XC_DUALXC_TEST == 1)
        MSAPI_GEGOP_XC_DEVICE_ID stGEGOP_XCDeviceId = {0, g_stXC_DeviceId[*pu32XCDevice].u32Id};
        MSAPI_GEGOP_XC_DEVICE_ID stGEGOP_XCDevice0 = {0, g_stXC_DeviceId[0].u32Id};

        //If SC1 input source is SC0 and SC0 already enable GOP seamless zapping, SC1 doesn't need to enable black screen.
        if ((msAPI_GEGOP_IsStillImageEnable(&stGEGOP_XCDeviceId, *pu32XCWindow) == TRUE) || ((*pu32XCDevice == E_MSAPI_XC_DEVICE1) && (eDDIXCInputSrc == E_DDI_XC_INPUT_SOURCE_SCALER0_OP) && (msAPI_GEGOP_IsStillImageEnable(&stGEGOP_XCDevice0, *pu32XCWindow) == TRUE)))
        {
            bIsSeamlessZapping = TRUE;
        }
#else
        if (msAPI_GEGOP_IsStillVideoEnable() == TRUE)
        {
            bIsSeamlessZapping = TRUE;
        }
#endif
#endif

        if (bIsSeamlessZapping == FALSE)
        {
            //Enable Black Screen
            msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        }

        //Set DTV/DTV2/HDMI Rx path to Scaler
        _Demo_XC_InputSource_Mapping(eDDIXCInputSrc, &eXCInputSrc);

        msAPI_XC_SetConnect_EX(&g_stXC_DeviceId[*pu32XCDevice], eXCInputSrc, *pu32XCWindow);

        _Demo_XC_SetPanelID(*pu32XCDevice);

        //Below setting only for XC input source DTV/DTV2
        if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
        {
            u32StartTime = _Demo_GetTime();
        }

        if(*pu32XCWindow == E_MSAPI_XC_MAIN_WINDOW)
        {
            msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, NULL, *pu32XCWindow);
            if(msAPI_XC_IsBlackVideoEnable_EX(&g_stXC_DeviceId[*pu32XCDevice],E_MSAPI_XC_MAIN_WINDOW) == TRUE)
            {   //Disable Black Screen
                msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
            }
        }
        else
        {
            MSAPI_XC_WINDOW_TYPE stDispWin = {0, 0, (IPANEL(&g_stPNL_DeviceId, Width)/3), (IPANEL(&g_stPNL_DeviceId, Height)/3)};
            msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);
            if(msAPI_XC_IsBlackVideoEnable_EX(&g_stXC_DeviceId[*pu32XCDevice], E_MSAPI_XC_SUB_WINDOW) == TRUE)
            {   //Disable Black Screen
                msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
            }
        }
#if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
        //Disable Freeze Screen
        if(Demo_XC_SeamlessZapping_IsFreeze(pu32XCDevice, pu32XCWindow)==TRUE)
        {
            msAPI_XC_SetVideoFreeze(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow);
        }
#endif
        //Below setting only for XC input source DTV/DTV2
        if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
        {
            u32EndTime = _Demo_GetTime();
            db_print("[%s][%d] DIFF TIME after MVOP and AV-Sync = %"DTC_MS_U32_u" ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);
            db_print("########## XC Black Screen DISABLE!!\n");
        }
    }
    else
    {
        MSAPI_XC_WINDOW_TYPE stDispWin = {0, 0, (IPANEL(&g_stPNL_DeviceId, Width)/3), (IPANEL(&g_stPNL_DeviceId, Height)/3)};
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);

    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Play video using customer setting (crop size, window size, aspect ratio...)
///         -   HD: DTV/HDMIRx -> SC0 -> HDMITx/Component
///         -   SD: DTV/HDMIRx -> SC1 -> VE Encoder
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32XCInputSrc the XC Input source type 0:DTV, 1:DTV2, 2:HDMIRx, 3:SC0 OP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_PlayCusVideo 0 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_PlayCusVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc = (E_DDI_XC_INPUT_SOURCE_TYPE)(*pu32XCInputSrc);
    E_MSAPI_XC_INPUT_SRC eXCInputSrc = 0;

    db_print("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__);

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    //Enable Black Screen
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

    //Set DTV/DTV2/HDMI Rx path to Scaler
    _Demo_XC_InputSource_Mapping(eDDIXCInputSrc, &eXCInputSrc);

    msAPI_XC_SetConnect_EX(&g_stXC_DeviceId[*pu32XCDevice], eXCInputSrc, *pu32XCWindow);

    //Check customer setting display window
    _Demo_Check_DispWin_IsValid(*pu32XCDevice, &g_stXC_DispInfo[*pu32XCDevice], *pu32XCWindow);

    if((g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].height == 0)||(g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].width== 0))
    {
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow], *pu32XCWindow);
    }
    else
    {
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, &g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow], &g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow], *pu32XCWindow);
    }

    //Disable Black Screen
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC Stop video
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32XCInputSrc the XC Input source type 0:DTV, 1:DTV2, 2:HDMIRx, 3:SC0 OP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
/// Command: \b XC_StopVideo 0 0 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_StopVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    E_MSAPI_XC_RESULT eRet = E_MSAPI_XC_FAIL;
    E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc = (E_DDI_XC_INPUT_SOURCE_TYPE)(*pu32XCInputSrc);
    E_MSAPI_XC_INPUT_SRC eXCInputSrc = 0;

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    //Enable Black Screen
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

    _Demo_XC_InputSource_Mapping(eDDIXCInputSrc, &eXCInputSrc);

    eRet = msAPI_XC_SetDisconnect_EX(&g_stXC_DeviceId[*pu32XCDevice], eXCInputSrc, *pu32XCWindow);
    if(eRet != E_MSAPI_XC_OK)
    {
        printf("[%s][%d] XC Stop Video Failed!: Device ID is %"DTC_MS_U32_d", Window is %"DTC_MS_U32_d" and Input Src is %d\n",
            __FUNCTION__, __LINE__, *pu32XCDevice, *pu32XCWindow, eXCInputSrc);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set scaler crop window size (for custom setting function)
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *x Start Position of X
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetCropWin 0 0 0 0 360 240 \n
///          \b XC_SetCropWin 0 0 0 0 720 480 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetCropWin(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].x = *x;
    g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].y = *y;
    g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].width= *width;
    g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].height = *height;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set scaler destination window(display window) size (for custom setting function)
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *x Start Position of Y
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetDestWin 0 0 0 0 1280 720 \n
///          \b XC_SetDestWin 0 0 360 240 920 480 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetDestWin(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow].x = *x;
    g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow].y = *y;
    g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow].width= *width;
    g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow].height = *height;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set scaler aspect ratio type \n
/// Usage: XC_SetAS [eAS]:
///    - E_ASPECT_RATIO_NORMAL           = 0,
///    - E_ASPECT_RATIO_4_3              = 1,
///    - E_ASPECT_RATIO_16_9             = 2,
///    - E_ASPECT_RATIO_CUT_4_3          = 3,
///    - E_ASPECT_RATIO_CUT_16_9         = 4,
///    - E_ASPECT_RATIO_FULL             = 5,
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32AS aspect ratio type of E_ASPECT_RATIO_TYPE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetAS 0 0 0 \n
///          \b XC_SetAS 0 1 2 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetAS(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32AS)
{
    E_MSAPI_XC_ASPECT_RATIO_TYPE eAS = E_MSAPI_XC_ASPECT_RATIO_NORMAL;

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    g_stXC_DispInfo[*pu32XCDevice].eAS = *pu32AS;
    _Demo_XC_AspectRatio_Mapping(g_stXC_DispInfo[*pu32XCDevice].eAS, &eAS);
    msAPI_XC_SetAspectRatioType_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, eAS);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set VE Capture Window(Source Window) Size(for cutting function) \n
/// Usage: Video_SD_SetSrcWin [XStart] [YStart] [HSize] [VSize] \n
/// \b \n
/// @param[in] *x Start Position of X
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetSrcWin 0 0 360 240 \n
///          \b VE_SetSrcWin 0 0 720 480 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetSrcWin(MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    g_stVE_DispInfo.stSrcWin.x = *x;
    g_stVE_DispInfo.stSrcWin.y = *y;
    g_stVE_DispInfo.stSrcWin.width= *width;
    g_stVE_DispInfo.stSrcWin.height= *height;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set VE Destination Window(Display Window) Size \n
/// Usage: Video_SD_SetDestWin [XStart] [YStart] [HSize] [VSize] \n
/// @param[in] *x Start Position of X
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetDestWin 0 0 720 480 \n
///          \b VE_SetDestWin 360 240 360 240 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetDestWin(MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    g_stVE_DispInfo.stDestWin.x = *x;
    g_stVE_DispInfo.stDestWin.y = *y;
    g_stVE_DispInfo.stDestWin.width= *width;
    g_stVE_DispInfo.stDestWin.height= *height;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set VE Aspect Ratio Type \n
/// Usage: Video_SD_SetAS [eAS]:
///    - E_ASPECT_RATIO_NORMAL           = 0,
///    - E_ASPECT_RATIO_4_3              = 1,
///    - E_ASPECT_RATIO_16_9             = 2,
///    - E_ASPECT_RATIO_CUT_4_3          = 3,
///    - E_ASPECT_RATIO_CUT_16_9         = 4,
///    - E_ASPECT_RATIO_FULL             = 5,
/// @param[in] *pu32SD_AS aspect ratio type of E_ASPECT_RATIO_TYPE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetAS 1 \n
///          \b VE_SetAS 5 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetAS(MS_U32 *pu32eAS)
{
    g_stVE_DispInfo.eAS = *pu32eAS;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SD Play video using default setting (default size, default aspect ratio)
/// @param[in] NA
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_PlayVideo\n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_PlayVideo(void)
{
    E_MSAPI_VE_SOURCE_TYPE eSourceType = E_MSAPI_VE_SOURCE_SCALER_OP2;
    MS_BOOL bIsSeamlessZapping = FALSE;
#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
#if (DEMO_XC_DUALXC_TEST == 1)
    MSAPI_GEGOP_XC_DEVICE_ID stGEGOP_XCDeviceId = {0, MSAPI_GEGOP_XC_DEVICE1};
    if (msAPI_GEGOP_IsStillImageEnable(&stGEGOP_XCDeviceId, MSAPI_GEGOP_MAIN_WINDOW) == TRUE)
    {
        bIsSeamlessZapping = TRUE;
    }
#else
    if (msAPI_GEGOP_IsStillVideoEnable() == TRUE)
    {
        bIsSeamlessZapping = TRUE;
    }
#endif
#endif

    eSourceType = msAPI_VE_GetSourceType();

    if ((bIsSeamlessZapping == FALSE) && (eSourceType == E_MSAPI_VE_SOURCE_SCALER_OP2))
    {
        msAPI_VE_SetVideoMute(ENABLE);
    }

#if (DEMO_XC_3DVIDEO2DOUT_TEST == 1)
    msAPI_VE_Get3DMode(g_stXC_3D_InputMode);
#endif

    msAPI_VE_SetMode();
    msAPI_VE_SetWin(NULL, NULL);
    msAPI_VE_SetVideoMute(DISABLE);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief SD Play video using default setting (default size, default aspect ratio)
/// @param[in] NA
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_PlayVideo\n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_PlayCusVideo(void)
{
    E_MSAPI_VE_SOURCE_TYPE eSourceType = E_MSAPI_VE_SOURCE_SCALER_OP2;
    MS_BOOL bIsSeamlessZapping = FALSE;
#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
#if (DEMO_XC_DUALXC_TEST == 1)
    MSAPI_GEGOP_XC_DEVICE_ID stGEGOP_XCDeviceId = {0, MSAPI_GEGOP_XC_DEVICE1};
    if (msAPI_GEGOP_IsStillImageEnable(&stGEGOP_XCDeviceId, MSAPI_GEGOP_MAIN_WINDOW) == TRUE)
    {
        bIsSeamlessZapping = TRUE;
    }
#else
    if (msAPI_GEGOP_IsStillVideoEnable() == TRUE)
    {
        bIsSeamlessZapping = TRUE;
    }
#endif
#endif

    eSourceType = msAPI_VE_GetSourceType();

    if ((bIsSeamlessZapping == FALSE) && (eSourceType == E_MSAPI_VE_SOURCE_SCALER_OP2))
    {
        msAPI_VE_SetVideoMute(ENABLE);
    }

    //Calculate VE Aspect Ratio
    _Demo_VE_CalculateAspectRatio(g_stVE_DispInfo.eAS, &g_stVE_DispInfo.stSrcWin, &g_stVE_DispInfo.stDestWin);
    db_print("VE stSrcWin.x = %d \n", g_stVE_DispInfo.stSrcWin.x);
    db_print("VE stSrcWin.y = %d \n", g_stVE_DispInfo.stSrcWin.y);
    db_print("VE stSrcWin.width = %d \n", g_stVE_DispInfo.stSrcWin.width);
    db_print("VE stSrcWin.height = %d \n", g_stVE_DispInfo.stSrcWin.height);
    db_print("VE stDestWin.x = %d \n", g_stVE_DispInfo.stDestWin.x);
    db_print("VE stDestWin.y = %d \n", g_stVE_DispInfo.stDestWin.y);
    db_print("VE stDestWin.width = %d \n", g_stVE_DispInfo.stDestWin.width);
    db_print("VE stDestWin.height = %d \n", g_stVE_DispInfo.stDestWin.height);

    msAPI_VE_SetMode();
    msAPI_VE_SetWin(&g_stVE_DispInfo.stSrcWin, &g_stVE_DispInfo.stDestWin);
    msAPI_VE_SetVideoMute(DISABLE);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief VE set all window
/// @param[in] *crop_x crop start x
/// @param[in] *crop_y crop start y
/// @param[in] *crop_width crop window width
/// @param[in] *crop_height crop window height
/// @param[in] *disp_x display start x
/// @param[in] *disp_y display start y
/// @param[in] *disp_width display window width
/// @param[in] *disp_height display window height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b VE_SetWinAll \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetWinAll(
        MS_U16 *crop_x, MS_U16 *crop_y, MS_U16 *crop_width, MS_U16 *crop_height,
        MS_U16 *disp_x, MS_U16 *disp_y, MS_U16 *disp_width, MS_U16 *disp_height)
{
    MS_VE_WINDOW_TYPE stSrcWin, stCropWin, stDispWin;
    E_MSAPI_VE_SOURCE_TYPE eSourceType = E_MSAPI_VE_SOURCE_SCALER_OP2;
    memset(&stSrcWin, 0, sizeof(MS_VE_WINDOW_TYPE));
    memset(&stCropWin, 0, sizeof(MS_VE_WINDOW_TYPE));
    memset(&stDispWin, 0, sizeof(MS_VE_WINDOW_TYPE));

    if ((!crop_x) || (!crop_y) || (!crop_width) || (!crop_height) ||
            (!disp_x) || (!disp_y) || (!disp_width) || (!disp_height)) {
        return FALSE;
    }

    // set source window
    stSrcWin.x = 0;
    stSrcWin.y = 0;

    eSourceType = msAPI_VE_GetSourceType();
    if ((eSourceType == E_MSAPI_VE_SOURCE_SCALER_DNR2VE)
     || (eSourceType == E_MSAPI_VE_SOURCE_SCALER_DI2VE))
    {
        // Source size of DNR2VE/DI2VE is video size
        VDEC_EX_DispInfo stVidStatus;
        VDEC_StreamId *stVDECStreamId = NULL;
        stVDECStreamId = Demo_VDEC_GetStreamID(E_VDEC_DEVICE_MAIN);
        msAPI_XC_GetVDECInfo_EX(stVDECStreamId,&stVidStatus);

        stSrcWin.width = stVidStatus.u16HorSize;
        stSrcWin.height = stVidStatus.u16VerSize;
    }
    else
    {
        // Source size of OP2VE is panel size
        stSrcWin.width = IPANEL(&g_stPNL_DeviceId, Width);
        stSrcWin.height = IPANEL(&g_stPNL_DeviceId, Height);
    }

    // set crop window
    stCropWin.x = *crop_x;
    stCropWin.y = *crop_y;
    stCropWin.width = *crop_width;
    stCropWin.height = *crop_height;
    // set display window
    stDispWin.x = *disp_x;
    stDispWin.y = *disp_y;
    stDispWin.width = *disp_width;
    stDispWin.height = *disp_height;

    db_print("VE Set Window\n");
    db_print("Src  (x, y, width, height) = (%d, %d, %d, %d)\n",
            stSrcWin.x, stSrcWin.y, stSrcWin.width, stSrcWin.height);
    db_print("Crop (x, y, width, height) = (%d, %d, %d, %d)\n",
            stCropWin.x, stCropWin.y, stCropWin.width, stCropWin.height);
    db_print("Disp (x, y, width, height) = (%d, %d, %d, %d)\n",
            stDispWin.x, stDispWin.y, stDispWin.width, stDispWin.height);

    if (E_VE_OK != MDrv_VE_SetWindow(&stSrcWin, &stCropWin, &stDispWin)) {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief VE set source path
/// @param[in] *pu32Path source path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b VE_SetPath \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetPath(MS_U32 *pu32Path)
{
    if (!pu32Path)
    {
        return FALSE;
    }

    switch ((E_MSAPI_VE_SOURCE_TYPE) *pu32Path)
    {
        case E_MSAPI_VE_SOURCE_AUTO:    // 0
            printf("E_MSAPI_VE_SOURCE_AUTO\n");
            break;
        case E_MSAPI_VE_SOURCE_SCALER_OP2:  // 1
            printf("E_MSAPI_VE_SOURCE_SCALER_OP2\n");
            break;
        case E_MSAPI_VE_SOURCE_SCALER_DI2VE:    // 2
            printf("E_MSAPI_VE_SOURCE_SCALER_DI2VE\n");
            break;
        case E_MSAPI_VE_SOURCE_EXT_BT656:   // 3
            printf("E_MSAPI_VE_SOURCE_EXT_BT656\n");
            break;
        case E_MSAPI_VE_SOURCE_SCALER_DNR2VE:   // 4
            printf("E_MSAPI_VE_SOURCE_SCALER_DNR2VE\n");
            break;
        default:
            printf("unknown path\n");
            break;

    }
    msAPI_VE_SetSourceType((E_MSAPI_VE_SOURCE_TYPE) *pu32Path);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Enable/disable video mute
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetVideoMute 0 0 1\n
///          \b XC_SetVideoMute 0 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetVideoMute(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    if (*pu32XCDevice == E_MSAPI_XC_DEVICE1)
    {
        // If device is XC1 and its source is OP, do not mute XC1 to keep UI on SD output
        E_MSAPI_XC_INPUT_SRC eXCInputSrc = E_MSAPI_XC_INPUT_SOURCE_NONE;
        eXCInputSrc = msAPI_XC_GetConnect_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow);
        if (eXCInputSrc == E_MSAPI_XC_INPUT_SOURCE_SCALER_OP)
        {
           return TRUE;
        }
    }
    msAPI_XC_SetVideoMute_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32Enable, *pu32XCWindow);
    return TRUE;
}

MS_BOOL Demo_XC_SetMainWindowFirst(MS_U32 *pu32XCDevice, MS_U32 *bMainFirst)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    db_print("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__);

    if(E_MSAPI_XC_DEVICE1 ==g_stXC_DeviceId[*pu32XCDevice].u32Id)
    {
        db_print("XC device1 not support sub window.\n");
        return FALSE;
    }

    msAPI_XC_ZorderMainWindowFirst_EX(&g_stXC_DeviceId[*pu32XCDevice], (MS_BOOL)*bMainFirst);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to eable XC window on/off
///           Please note that when x, y, width and height are 0s, original window size iskept.
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32Enable Enable/Disable window
/// @param[in] *x Start Position of Y
/// @param[in] *y Start Position of Y
/// @param[in] *width Horizontal Size
/// @param[in] *height Vertical Size
/// @return TRUE: Get success.
/// @return FALSE: Get fail.
/// @sa
/// @note
/// Command: \b DigiTuner_GetIndex \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_EnableWindow(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable,
                                    MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32Width, MS_U32 *pu32Height)
{
    MSAPI_XC_WINDOW_TYPE stWindowType = {(MS_U16)*pu32X, (MS_U16)*pu32Y, (MS_U16)*pu32Width, (MS_U16)*pu32Height};

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    if ((*pu32X == 0) && (*pu32Y == 0) && (*pu32Width == 0) && (*pu32Height == 0))
    {
        msAPI_XC_EnableWindow_EX(&g_stXC_DeviceId[*pu32XCDevice], (E_MSAPI_XC_WINDOW)*pu32XCWindow, (MS_BOOL)*pu32Enable, NULL);
    }
    else
    {
        msAPI_XC_EnableWindow_EX(&g_stXC_DeviceId[*pu32XCDevice], (E_MSAPI_XC_WINDOW)*pu32XCWindow, (MS_BOOL)*pu32Enable, &stWindowType);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to gradually scale down the output video size
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32XCInputSrc the XC Input source type 0:DTV, 1:DTV2, 2:HDMIRx, 3:SC0 OP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b XC_RegressionTest_ScalingDown 0 0 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_RegressionTest_ScalingDown(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    MSAPI_XC_WINDOW_TYPE tDstWin;
    MS_FLOAT fVideoSizeDecRate = 0.0;

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)
    //g_stPNL_DeviceId = {g_stXC_DeviceId[*pu32XCDevice].u32Version, g_stXC_DeviceId[*pu32XCDevice].u32Id};
    _Demo_XC_SetPanelID(*pu32XCDevice);

    Demo_XC_PlayVideo(pu32XCDevice, pu32XCWindow, pu32XCInputSrc);
    MsOS_DelayTask(TEST_PAUSE_TIME);

    // Same aspect video size decreasing
    for(fVideoSizeDecRate = VIDEO_SIZE_DEC_RATE; fVideoSizeDecRate < 1; fVideoSizeDecRate = fVideoSizeDecRate + VIDEO_SIZE_DEC_RATE)
    {
        db_print("DEC RATE = %lf\n", fVideoSizeDecRate);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

        tDstWin.x = IPANEL(&g_stPNL_DeviceId, Width) * (fVideoSizeDecRate / 2);
        tDstWin.y = IPANEL(&g_stPNL_DeviceId, Height) * (fVideoSizeDecRate / 2);
        tDstWin.width = IPANEL(&g_stPNL_DeviceId, Width) * (1 - fVideoSizeDecRate);
        tDstWin.height = IPANEL(&g_stPNL_DeviceId, Height) * (1 - fVideoSizeDecRate);

        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &tDstWin, *pu32XCWindow);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    Demo_XC_PlayVideo(pu32XCDevice, pu32XCWindow, pu32XCInputSrc);
    MsOS_DelayTask(TEST_PAUSE_TIME);

    // Only H-size decreasing
    for(fVideoSizeDecRate = VIDEO_SIZE_DEC_RATE; fVideoSizeDecRate < 1; fVideoSizeDecRate = fVideoSizeDecRate + VIDEO_SIZE_DEC_RATE)
    {
        db_print("DEC RATE = %lf\n", fVideoSizeDecRate);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

        tDstWin.x = IPANEL(&g_stPNL_DeviceId, Width) * (fVideoSizeDecRate / 2);
        tDstWin.y = 0;
        tDstWin.width = IPANEL(&g_stPNL_DeviceId, Width) * (1 - fVideoSizeDecRate);
        tDstWin.height = IPANEL(&g_stPNL_DeviceId, Height);

        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &tDstWin, *pu32XCWindow);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    Demo_XC_PlayVideo(pu32XCDevice, pu32XCWindow, pu32XCInputSrc);
    MsOS_DelayTask(TEST_PAUSE_TIME);

    // Only V-size decreasing
    for(fVideoSizeDecRate = VIDEO_SIZE_DEC_RATE; fVideoSizeDecRate < 1; fVideoSizeDecRate = fVideoSizeDecRate + VIDEO_SIZE_DEC_RATE)
    {
        db_print("DEC RATE = %lf\n", fVideoSizeDecRate);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

        tDstWin.x = 0;
        tDstWin.y = IPANEL(&g_stPNL_DeviceId, Height) * (fVideoSizeDecRate / 2);
        tDstWin.width = IPANEL(&g_stPNL_DeviceId, Width);
        tDstWin.height = IPANEL(&g_stPNL_DeviceId, Height) * (1 - fVideoSizeDecRate);

        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &tDstWin, *pu32XCWindow);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    Demo_XC_PlayVideo(pu32XCDevice, pu32XCWindow, pu32XCInputSrc);
    return TRUE;
}
#if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Enable/disable seamless zapping
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_EnableSeamlessZapping 0 1 1\n
///          \b XC_EnableSeamlessZapping 0 1 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_EnableSeamlessZapping(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    if(E_MSAPI_XC_DEVICE0 == *pu32XCDevice) // SC0
    {
        msAPI_XC_SetSeamlessZapping_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu32Enable, MEM_ADR_BY_MIU(SC0_SEAMLESS_ZAPPING_BUF_ADR, SC0_SEAMLESS_ZAPPING_BUF_MEMORY_TYPE));
    }
    else // SC1
    {
        msAPI_XC_SetSeamlessZapping_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu32Enable, MEM_ADR_BY_MIU(SC1_SEAMLESS_ZAPPING_BUF_ADR, SC1_SEAMLESS_ZAPPING_BUF_MEMORY_TYPE));
    }
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Seamless zapping using default setting (default size, default aspect ratio)
///         -   HD: DTV/HDMIRx -> SC0 -> HDMITx/Component
///         -   SD: DTV/HDMIRx -> SC1 -> VE Encoder
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32XCInputSrc the XC Input source type 0:DTV, 1:DTV2, 2:HDMIRx, 3:SC0 OP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SeamlessZapping 0 0 1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SeamlessZapping(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc)
{
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc = (E_DDI_XC_INPUT_SOURCE_TYPE)(*pu32XCInputSrc);
    E_MSAPI_XC_INPUT_SRC eXCInputSrc;

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    _Demo_XC_SetPanelID(*pu32XCDevice);

    printf("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__);

    //Set DTV/DTV2/HDMI Rx path to Scaler

    _Demo_XC_InputSource_Mapping(eDDIXCInputSrc, &eXCInputSrc);
    msAPI_XC_SetConnect_EX(&g_stXC_DeviceId[*pu32XCDevice], eXCInputSrc, *pu32XCWindow);

    u32StartTime = _Demo_GetTime();

    if(*pu32XCWindow == E_MSAPI_XC_MAIN_WINDOW)
    {
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, NULL, *pu32XCWindow);
    }
    else
    {
        MSAPI_XC_WINDOW_TYPE stDispWin = {0, 0, (IPANEL(&g_stPNL_DeviceId, Width)/3), (IPANEL(&g_stPNL_DeviceId, Height)/3)};
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &stDispWin, *pu32XCWindow);
        msAPI_XC_EnableWindow_EX(&g_stXC_DeviceId[*pu32XCDevice], E_MSAPI_XC_SUB_WINDOW, TRUE, &stDispWin);
    }

    //Disable video freeze
    msAPI_XC_SetVideoFreeze(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, *pu32XCWindow);

    u32EndTime = _Demo_GetTime();
    db_print("[%s][%d] DIFF TIME after MVOP and AV-Sync = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);
    db_print("########## XC Black Screen DISABLE!!\n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Enable/disable seamless zapping freeze image
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SeamlessZapping_SetFreeze 0 0 1\n
///          \b XC_SeamlessZapping_SetFreeze 0 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SeamlessZapping_SetFreeze(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_SetVideoFreeze(&g_stXC_DeviceId[*pu32XCDevice], *pu32Enable, *pu32XCWindow);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC window is freeze or not
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @return TRUE: freeze.
/// @return FALSE: not freeze.
/// @sa
/// @note
/// Sample Command: \n
///          \b Demo_XC_SeamlessZapping_IsFreeze 0 0
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SeamlessZapping_IsFreeze(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow)
{
    MS_BOOL bRet;

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    bRet = msAPI_XC_IsFreezeImg_EX(&g_stXC_DeviceId[*pu32XCDevice], (E_MSAPI_XC_WINDOW)*pu32XCWindow );

    return  bRet;
}
#endif // (DEMO_XC_SEAMLESS_ZAPPING)

#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to enable still image by GOP for zapping without blackscreen
/// @param[in] *bEnable enabling flag for still image, 1=enable, 0=disable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \b XC_EnableStillImage \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_EnableStillImage(MS_BOOL *bEnable)
{
    MS_U32 u32DWinBufPA = 0;
    MS_BOOL bRet = FALSE;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8 u8SDGOP = STILL_IMG_ZAPPING_SD_GOP;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    if(msAPI_VE_GetSourceType() == E_MSAPI_VE_SOURCE_SCALER_OP2)
    {
        u8SDGOP = 0xFF;
    }

    // Show still image
    if(*bEnable)
    {
        MS_BOOL bNewMem = FALSE;

        if(!_pDWinBuf)
        {
            bNewMem = TRUE;
        }
        else
        {
            if(u32DWinBufSize < msAPI_GEGOP_GetStillVideoRequireMem(u8SDGOP))
            {
                if(!MsOS_FreeMemory(_pDWinBuf, s32MstarNonCachedPoolID))
                {
                    printf("[%s][%d] Free memory failed!!\n", __FUNCTION__, __LINE__);
                }

                bNewMem = TRUE;
            }
        }

        if(bNewMem)
        {
            u32DWinBufSize = msAPI_GEGOP_GetStillVideoRequireMem(u8SDGOP);
            _pDWinBuf = MsOS_AllocateMemory(u32DWinBufSize, s32MstarNonCachedPoolID);

            if(!_pDWinBuf)
            {
                printf("[%s][%d] Cannot obtain memory!!\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
        }

        u32DWinBufPA = MsOS_VA2PA((MS_U32)_pDWinBuf);
        db_print("Still zapping DWIN buffer VA = %"DTC_MS_U32_u", PA = %"DTC_MS_U32_u", size = %"DTC_MS_U32_u"\n",
            (MS_U32)_pDWinBuf, u32DWinBufPA, u32DWinBufSize);

        bRet = msAPI_GEGOP_CreateStillVideo(STILL_IMG_ZAPPING_HD_GOP, u8SDGOP, u32DWinBufPA, u32DWinBufSize);

        if(bRet)
        {
            bRet = msAPI_GEGOP_ShowStillVideo(STILL_IMG_ZAPPING_HD_GOP, u8SDGOP, u32DWinBufPA, u32DWinBufSize);
        }
    }
    // Hide still image
    else
    {
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[0], TRUE, MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[0], FALSE, MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        msAPI_XC_WaitOutputVSync_EX(&g_stXC_DeviceId[0], 2, 50, MAIN_WINDOW);

        bRet = msAPI_GEGOP_HideStillVideo();

        if(bRet)
        {
            bRet = msAPI_GEGOP_DestroyStillVideo();
        }

        if(!MsOS_FreeMemory(_pDWinBuf, s32MstarNonCachedPoolID))
        {
            printf("[%s][%d] Free memory failed!!\n", __FUNCTION__, __LINE__);
        }

        _pDWinBuf = NULL;
    }

    return bRet;
}
#if (DEMO_XC_DUALXC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to enable still image by GOP for zapping without blackscreen
/// @param[in] *bEnable enabling flag for still image, 1=enable, 0=disable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \b XC_FreezeStillImage_EX \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_EnableStillImage_ByDevice(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_BOOL *pbEnable)
{
    MS_U32 u32DipBufPA = 0;
    MS_BOOL bRet = FALSE;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_U8 u8GOP = 0;
    MS_U32 u32wordunit = 0;
    MSAPI_GEGOP_XC_DEVICE_ID stGEGOP_XCDeviceId[MSAPI_GEGOP_XC_DEVICE_MAX] = {{0, MSAPI_GEGOP_XC_DEVICE0}, {0, MSAPI_GEGOP_XC_DEVICE1}};

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    MApi_GOP_GetChipCaps(E_GOP_CAP_WORD_UNIT, &u32wordunit, 0);

    // Show still image
    if(*pbEnable)
    {
        if(*pu32XCDevice == MSAPI_GEGOP_XC_DEVICE0)
        {
            u8GOP = STILL_IMG_ZAPPING_HD_GOP;
        }
        else
        {
            u8GOP = STILL_IMG_ZAPPING_SD_GOP;
        }

        u32DipBufSize = msAPI_GEGOP_GetStillImageRequireMem(&stGEGOP_XCDeviceId[*pu32XCDevice],*pu32XCWindow);
        _pDipBuf[*pu32XCDevice] = MsOS_AllocateMemory(u32DipBufSize + u32wordunit, s32MstarNonCachedPoolID);

        if(!_pDipBuf[*pu32XCDevice])
        {
            printf("[%s][%d] Cannot obtain memory!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        u32DipBufPA = (MsOS_VA2PA((MS_U32)_pDipBuf[*pu32XCDevice]) + u32wordunit ) & (~(u32wordunit-1));
        if(u32DipBufPA > MIU_INTERVAL)
        {
            MApi_GOP_MIUSel(u8GOP,E_GOP_SEL_MIU1);
        }
        else
        {
            MApi_GOP_MIUSel(u8GOP,E_GOP_SEL_MIU0);
        }
        db_print("Still zapping DIP buffer VA = %"DTC_MS_U32_u", PA = %"DTC_MS_U32_u", size = %"DTC_MS_U32_u"\n",
            (MS_U32)_pDipBuf[*pu32XCDevice], u32DipBufPA, u32DipBufSize);
        bRet = msAPI_GEGOP_EnableStillImage(&stGEGOP_XCDeviceId[*pu32XCDevice], *pu32XCWindow, u8GOP, u32DipBufPA, u32DipBufSize);
    }
    // Hide still image
    else
    {
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], TRUE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], FALSE, *pu32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
        msAPI_XC_WaitOutputVSync_EX(&g_stXC_DeviceId[*pu32XCDevice], 2, 50, *pu32XCWindow);

        bRet = msAPI_GEGOP_DisableStillImage(&stGEGOP_XCDeviceId[*pu32XCDevice],*pu32XCWindow);

        if(!MsOS_FreeMemory(_pDipBuf[*pu32XCDevice], s32MstarNonCachedPoolID))
        {
            printf("[%s][%d] Free memory failed!!\n", __FUNCTION__, __LINE__);
        }

        _pDipBuf[*pu32XCDevice] = NULL;
    }

    return bRet;
}
#endif // (DEMO_XC_DUALXC)
#endif // (DEMO_GOP_SEAMLESS_ZAPPING)
#if (DEMO_XC_3D_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Enable/disable 3D output
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *bEnable 1=ENABLE, 0=DISABLE
/// @param[in] *enOutMode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_Enable3D 0 1 1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_Enable3D(MS_U32 *pu32XCDevice, MS_U32 *bEnable, MS_U32 *enOutMode)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    g_Demo_3DEnable = *bEnable;

    //Check input parameter
    if(E_MSAPI_XC_DEVICE1 == g_stXC_DeviceId[*pu32XCDevice].u32Id)
    {
        db_print("XC_DEVICE1 not support 3D output mode.\n");
        g_stXC_3D_OutputMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
    }

    //Set Input 3D mode by user defined
    if(g_Demo_3DEnable)
    {
        E_MSAPI_XC_OUTPUT_TIMING_TYPE eXCTiming = E_MSAPI_XC_RES_MIN;
        HDMITX_VIDEO_TIMING eHDMITxTiming = HDMITX_RES_640x480p;

        msAPI_XC_GetOutputResolution_EX(&g_stXC_DeviceId[*pu32XCDevice], &eXCTiming);

        if(!_Demo_HDMITx_Timing_Mapping(eXCTiming, &eHDMITxTiming))
        {
            db_print("Invalid output timing!\n");
            return FALSE;
        }

        msAPI_XC_Correct3DOutputMode(*enOutMode, &g_stXC_3D_OutputMode);

#if (DEMO_XC_3DVIDEO2DOUT_TEST == 1)
        g_stXC_3D_OutputMode = (E_MSAPI_XC_3D_OUTPUT_MODE)(*enOutMode);
#endif
    }
    else
    {
        g_stXC_3D_OutputMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Play video in 3D output mode
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCInputSrc the XC Input source type 0:DTV, 1:DTV2, 2:HDMIRx, 3:SC0 OP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_PlayVideo_3D 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_PlayVideo_3D(MS_U32 *pu32XCDevice, MS_U32 *pu32XCInputSrc)
{
    MS_U32 u32StartTime = 0, u32EndTime = 0;
    E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc = (E_DDI_XC_INPUT_SOURCE_TYPE)(*pu32XCInputSrc);
    E_MSAPI_XC_INPUT_SRC eXCInputSrc;
    EN_VDEC_FrmPackMode videoFrmPackMode;
    E_MSAPI_XC_3D_INPUT_MODE en3DInMode = E_MSAPI_XC_3D_INPUT_MODE_NONE;
    EN_AV_Device device = E_AV_DEVICE_MAIN;
    HDMITX_VIDEO_3D_STRUCTURE eHDMITx3DMode = HDMITx_VIDEO_3D_Not_in_Use;

    db_print("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__);

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    //Check input parameter and return if unsupported
    if(E_MSAPI_XC_DEVICE1 ==g_stXC_DeviceId[*pu32XCDevice].u32Id)
    {
        db_print("SW not ready for XC_DEVICE1 with 3D video input.\n");
        return FALSE;
    }

    //Enable Black Screen
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], ENABLE, E_MSAPI_XC_SUB_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

    //Set DTV/DTV2/HDMI Rx path to Scaler
    _Demo_XC_InputSource_Mapping(eDDIXCInputSrc, &eXCInputSrc);

    msAPI_XC_SetConnect_EX(&g_stXC_DeviceId[*pu32XCDevice], eXCInputSrc, E_MSAPI_XC_MAIN_WINDOW);

    //Set Input 3D mode by input video information
    if(g_Demo_3DEnable)
    {
        ///FIXME: DTV2 need to adjust 'device'
        if((E_DDI_XC_INPUT_SOURCE_DTV == eDDIXCInputSrc) || (E_DDI_XC_INPUT_SOURCE_DTV2 == eDDIXCInputSrc))
        {
            if(E_DDI_XC_INPUT_SOURCE_DTV == eDDIXCInputSrc)
                device = E_AV_DEVICE_MAIN;
            else
                device = E_AV_DEVICE_SUB;
            videoFrmPackMode = Demo_VDEC_GetFramePackMode(device);
            if(E_VDEC_DDI_FRM_PACK_SIDEBYSIDE == videoFrmPackMode)
                en3DInMode = E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
            else if(E_VDEC_DDI_FRM_PACK_TOPBOTTOM == videoFrmPackMode)
                en3DInMode = E_MSAPI_XC_3D_INPUT_TOP_BOTTOM;
        }
        else
        {
            db_print("Input parameter error!\n");
            en3DInMode = E_MSAPI_XC_3D_INPUT_MODE_NONE;
        }
    }
    else
    {
        en3DInMode = E_MSAPI_XC_3D_INPUT_MODE_NONE;
    }

#if (DEMO_XC_3DVIDEO2DOUT_TEST == 1)
    g_stXC_3D_InputMode = en3DInMode;
    if(E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM == g_stXC_3D_OutputMode)
    {
        if(en3DInMode != E_MSAPI_XC_3D_INPUT_TOP_BOTTOM)
        {
            printf( "Fail!!! 3D Video 2D Out : Input = SBS, Output = TB \n");
            return FALSE;
        }
    }
    else if(E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == g_stXC_3D_OutputMode)
    {
        if(en3DInMode != E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        {
            printf( "Fail!!! 3D Video 2D Out  : Input = TB, Output = SBS \n");
            return FALSE;
        }
    }
    else
    {
        printf( "Fail!!! 3D Video 2D Out : Not support type \n");
        return FALSE;
    }
#endif

#if (DEMO_XC_3DVIDEO_TEST == 1)
    // - If the source is HDMIRx input, the 3D input mode will be auto detect in HDMI Rx monitor
    // - The input parameter 'en3DInMode' is don't care if the source is HDMIRx
    // - Use 'msAPI_XC_HDMIRx_Get3DMode' to get input 3D mode
    if(E_MSAPI_XC_OK != msAPI_XC_Set3DMode_EX(&g_stXC_DeviceId[*pu32XCDevice], en3DInMode, g_stXC_3D_OutputMode))
    {
        db_print("msAPI_XC_Set3DMode_EX failed\n");
        return FALSE;
    }
#endif

    //Below setting only for XC input source DTV/DTV2
    if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
    {
        u32StartTime = _Demo_GetTime();
    }

#if (DEMO_XC_3DVIDEO2DOUT_TEST == 1)
        MSAPI_XC_WINDOW_TYPE pt3Dvideo2DOutWin;
        memset(&pt3Dvideo2DOutWin,0,sizeof(MSAPI_XC_WINDOW_TYPE));
        pt3Dvideo2DOutWin.width = g_IPanel.Width();
        pt3Dvideo2DOutWin.height = g_IPanel.Height();
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], &pt3Dvideo2DOutWin, &pt3Dvideo2DOutWin, &pt3Dvideo2DOutWin, E_MSAPI_XC_MAIN_WINDOW);
#endif

#if (DEMO_XC_3DVIDEO_TEST == 1)
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, NULL, E_MSAPI_XC_MAIN_WINDOW);
#endif
    // Clone setting from main window to sub window

    //Enable HDMITx 3D Packet
    _Demo_HDMITx_3D_Mapping(g_stXC_3D_OutputMode, &eHDMITx3DMode);
    msAPI_XC_HDMITx_Set3DMode(g_Demo_3DEnable, eHDMITx3DMode);

    //Disable Black Screen
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
    msAPI_XC_SetVideoMuteByMode_EX(&g_stXC_DeviceId[*pu32XCDevice], DISABLE, E_MSAPI_XC_SUB_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

    //Below setting only for XC input source DTV/DTV2
    if( MSAPI_XC_IsSrcTypeDTV(eXCInputSrc) || MSAPI_XC_IsSrcTypeCapture(eXCInputSrc) )
    {
        u32EndTime = _Demo_GetTime();
        db_print("[%s][%d] DIFF TIME after MVOP and AV-Sync = %"DTC_MS_U32_u" ms\n",
            __FUNCTION__, __LINE__, u32EndTime - u32StartTime);
        db_print("########## XC Black Screen DISABLE!!\n");
    }

    return TRUE;
}
#endif

#if (DEMO_XC_DSSWDB_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Enable DS to set XC function
/// @param[in] *pbEnable enable for DS SWDB function
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_EnableDS_SWDB 0 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_EnableDS_SWDB(MS_U32 *pbEnable)
{
    static MS_U32 gu32DS_SWDBAddr = 0;
    if(*pbEnable == TRUE && gu32DS_SWDBAddr == 0)
    {
        MS_S32 s32NonCachedPoolID;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);
        gu32DS_SWDBAddr = (MS_U32)MsOS_AllocateMemory((16 * 32 * 2),s32NonCachedPoolID);
    }
    else if(*pbEnable == FALSE && gu32DS_SWDBAddr != 0)
    {
        MS_S32 s32NonCachedPoolID;
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);
        MsOS_FreeMemory((void*)gu32DS_SWDBAddr,s32NonCachedPoolID);
    }
    else
    {
        printf("Flow error\n");
        return TRUE;
    }

    if(*pbEnable == TRUE)
    {
        XC_DynamicScaling_Info stXC_DS_Info;
        memset(&stXC_DS_Info, 0, sizeof(XC_DynamicScaling_Info));
        stXC_DS_Info.u32DS_Info_BaseAddr = MsOS_VA2PA(gu32DS_SWDBAddr);
        stXC_DS_Info.u8DS_Index_Depth = 32;
        stXC_DS_Info.u8MIU_Select = 0;
        stXC_DS_Info.bOP_DS_On = TRUE;
        stXC_DS_Info.bIPM_DS_On = TRUE;
        stXC_DS_Info.bIPS_DS_On = FALSE;

        memset((void*)gu32DS_SWDBAddr, 0, (16 * 32 * 2));

        MApi_XC_SetDynamicScaling(&stXC_DS_Info, sizeof(XC_DynamicScaling_Info), MAIN_WINDOW);
        MApi_XC_EnableDSRegMode(*pbEnable,MAIN_WINDOW);
        MSAPI_XC_Handle stHD;
        stHD.eWin = E_MSAPI_XC_MAIN_WINDOW;
        msAPI_XC_SetCommand(&stHD,E_MSAPI_XC_CMD_SET_DS_SWDB,pbEnable);
    }
    else
    {
        gu32DS_SWDBAddr = 0;
        XC_DynamicScaling_Info stXC_DS_Info;
        memset(&stXC_DS_Info, 0, sizeof(XC_DynamicScaling_Info));
        stXC_DS_Info.u32DS_Info_BaseAddr = 0;
        stXC_DS_Info.u8DS_Index_Depth = 32;
        stXC_DS_Info.u8MIU_Select = 0;
        stXC_DS_Info.bOP_DS_On = FALSE;
        stXC_DS_Info.bIPM_DS_On = FALSE;
        stXC_DS_Info.bIPS_DS_On = FALSE;
        MApi_XC_SetDynamicScaling(&stXC_DS_Info, sizeof(XC_DynamicScaling_Info), MAIN_WINDOW);
        MApi_XC_EnableDSRegMode(*pbEnable,MAIN_WINDOW);
        MSAPI_XC_Handle stHD;
        stHD.eWin = E_MSAPI_XC_MAIN_WINDOW;
        msAPI_XC_SetCommand(&stHD,E_MSAPI_XC_CMD_SET_DS_SWDB,pbEnable);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set scaler window size using customer setting (crop size, window size, aspect ratio...)
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_ChangeWinSize 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_ChangeWindowSize(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow)
{
    db_print("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__);

    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    //Check customer setting display window
    _Demo_Check_DispWin_IsValid(*pu32XCDevice, &g_stXC_DispInfo[*pu32XCDevice], *pu32XCWindow);

    if((g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].height == 0)||(g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow].width== 0))
    {
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, NULL, &g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow], *pu32XCWindow);
    }
    else
    {
        msAPI_XC_SetWin_EX(&g_stXC_DeviceId[*pu32XCDevice], NULL, &g_stXC_DispInfo[*pu32XCDevice].stCropWin[*pu32XCWindow], &g_stXC_DispInfo[*pu32XCDevice].stDestWin[*pu32XCWindow], *pu32XCWindow);
    }

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief Set Video Brightness
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu8Value brightness value(0~255)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetBrightness 0 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetBrightness(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *pu8Value)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_ACE_SetBrightness_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Video Sharpness
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu8Value Sharpness value(0~63)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetSharpness 0 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetSharpness(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *pu8Value)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_ACE_SetSharpness_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Adjust Video contrast in XC
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu8Value - video color Contrast setting 0~100
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetContrast 0 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetContrast(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *pu8Value)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_ACE_SetContrast_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Adjust Video Hue in XC
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu8Value - Hue Value 0~100
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetHue 0 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetHue(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *pu8Value)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_ACE_SetHue_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Adjust Video Saturation in XC
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32XCWindow the XC Window
/// @param[in] *pu8Value - Saturation Value 0~255
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetSaturation 0 0 128 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetSaturation(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *pu8Value)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_ACE_SetSaturation_EX(&g_stXC_DeviceId[*pu32XCDevice], *pu32XCWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC set frame color (background color)
/// @param[in] *pu32XCDevice the XC Device
/// @param[in] *pu32Index the color index of back ground color of SCALER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetFrameColor 0 0   //BLACK \n
///          \b XC_SetFrameColor 0 1   //WHITE \n
///          \b XC_SetFrameColor 0 2   //RED \n
///          \b XC_SetFrameColor 0 3   //GREEN \n
///          \b XC_SetFrameColor 0 4   //BLUE \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetFrameColor(MS_U32 *pu32XCDevice, MS_U32 *pu32Index)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    switch(*pu32Index)
    {
        default:
        case 0:
            msAPI_XC_SetFrameColor_EX(&g_stXC_DeviceId[*pu32XCDevice], 0x00000000);
            break;
        case 1:
            msAPI_XC_SetFrameColor_EX(&g_stXC_DeviceId[*pu32XCDevice], 0x00FFFFFF);
            break;
        case 2:
            msAPI_XC_SetFrameColor_EX(&g_stXC_DeviceId[*pu32XCDevice], 0x00FF0000);
            break;
        case 3:
            msAPI_XC_SetFrameColor_EX(&g_stXC_DeviceId[*pu32XCDevice], 0x0000FF00);
            break;
        case 4:
            msAPI_XC_SetFrameColor_EX(&g_stXC_DeviceId[*pu32XCDevice], 0x000000FF);
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Generate Black Video
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_GenerateBlackVideo 0 1 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_GenerateBlackVideo(MS_U32 *pu32XCDevice, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    //Set black Screen (no video)
    msAPI_XC_SetVideoMute_EX(&g_stXC_DeviceId[*pu32XCDevice],bEnable, eWindow);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Set Video Alpha
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetVideoAlpha 0 0 255 1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_SetVideoAlpha(MS_U32 *pu32XCDevice,MS_U32 *pu32XCWindow, MS_U8 *pu8Value, MS_U32 *pu32VOPOSDLayer)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice)

    msAPI_XC_SetOSDLayer_EX(&g_stXC_DeviceId[*pu32XCDevice],*pu32VOPOSDLayer,*pu32XCWindow);
    msAPI_XC_SetVideoAlpha_EX(&g_stXC_DeviceId[*pu32XCDevice],*pu8Value, *pu32XCWindow);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC set drvier dbg level to print common debug message (only for debug)
/// @param[in] *pbEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_EnableDrvDmsg 0 \n
///          \b XC_EnableDrvDmsg 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_EnableDmesg(MS_U32 *pbEnable)
{
    if(*pbEnable)
    {
       msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_PQ);
       msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_DRV);
    }
    else
    {
        msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_NONE);
        msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_NONE);
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief: Convert vdec display info to MSAPI display info
/// @param[in] pstDispInfo vdec display info
/// @param[out] pstMSAPIDispInfo XC MSAPI display info
//-----------------------------------------------------------------------------
MS_BOOL Demo_XC_VdecDispInfo2msAPIVdecDispInfo(VDEC_EX_DispInfo *pstDispInfo, MSAPI_XC_VDEC_DispInfo *pstMSAPIDispInfo )
{
    pstMSAPIDispInfo->u16HorSize          = pstDispInfo->u16HorSize;
    pstMSAPIDispInfo->u16VerSize          = pstDispInfo->u16VerSize;
    pstMSAPIDispInfo->u32FrameRate        = pstDispInfo->u32FrameRate;
    pstMSAPIDispInfo->u8Interlace         = pstDispInfo->u8Interlace;
    pstMSAPIDispInfo->u8AFD               = pstDispInfo->u8AFD;
    pstMSAPIDispInfo->u16SarWidth         = pstDispInfo->u16SarWidth;
    pstMSAPIDispInfo->u16SarHeight        = pstDispInfo->u16SarHeight;
    pstMSAPIDispInfo->u16CropRight        = pstDispInfo->u16CropRight;
    pstMSAPIDispInfo->u16CropLeft         = pstDispInfo->u16CropLeft;
    pstMSAPIDispInfo->u16CropBottom       = pstDispInfo->u16CropBottom;
    pstMSAPIDispInfo->u16CropTop          = pstDispInfo->u16CropTop;
    pstMSAPIDispInfo->u16Pitch            = pstDispInfo->u16Pitch;
    pstMSAPIDispInfo->u16PTSInterval      = pstDispInfo->u16PTSInterval;
    pstMSAPIDispInfo->u8MPEG1             = pstDispInfo->u8MPEG1;
    pstMSAPIDispInfo->u8PlayMode          = pstDispInfo->u8PlayMode;
    pstMSAPIDispInfo->u8FrcMode           = pstDispInfo->u8FrcMode;
    pstMSAPIDispInfo->u8AspectRate        = pstDispInfo->u8AspectRate;
    pstMSAPIDispInfo->bWithChroma         = pstDispInfo->bWithChroma;
    pstMSAPIDispInfo->bColorInXVYCC       = pstDispInfo->bColorInXVYCC;
    pstMSAPIDispInfo->u32DynScalingAddr   = pstDispInfo->u32DynScalingAddr;
    pstMSAPIDispInfo->u32DynScalingSize   = pstDispInfo->u32DynScalingSize;
    pstMSAPIDispInfo->u8DynScalingDepth   = pstDispInfo->u8DynScalingDepth;
    pstMSAPIDispInfo->bEnableMIUSel       = pstDispInfo->bEnableMIUSel;
    pstMSAPIDispInfo->u32AspectWidth      = pstDispInfo->u32AspectWidth;
    pstMSAPIDispInfo->u32AspectHeight     = pstDispInfo->u32AspectHeight;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Disable SCALER
/// @param[in] *pu32XCDevice the XC Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_Exit 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_Exit(MS_U32 *pu32XCDevice)
{
    DEMO_XC_CHECK_DEVICE_ID(*pu32XCDevice);

    msAPI_XC_Exit_EX(&g_stXC_DeviceId[*pu32XCDevice]);

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Disable VE
/// @param[in] N/A
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_Exit \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_Exit(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    msAPI_VE_Exit();

    bExitDACDetectDemoTask = TRUE;
    bExitVBITTXDemoTask = TRUE;
    if (_pu8VBITTX_Buff) {
        MsOS_FreeMemory(_pu8VBITTX_Buff, s32MstarCachedPoolID);
        _pu8VBITTX_Buff = 0;
    }

    return TRUE;
}

//VBI DEMO
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  VBI Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  VBI Local Variables
//-------------------------------------------------------------------------------------------------
#define PACKETS_SEND_THRESHOLD 32 //By experiment, it'd better to send 32 packets in one run
#define VE_DRIVER_TIMEOUT 1000 // millisecond
#define PEDDING_BUFFER_PAYLOAD_NUM 16
#define TTX_PACKET_SIZE 48
#define LINECOUNT 4 // number of line that we insert ttx data in

static void _Demo_VE_VBITtx_ISR(void)
{
    MsOS_SetEvent(s32VBITTXEventId, DEMO_VBITTX_EVT_TT_DONE);

    MsOS_EnableInterrupt(E_INT_FIQ_VE_DONE_TT);
}
void _Demo_VE_VBITTX_Task(void)
{
    while(bExitVBITTXDemoTask == FALSE)
    {
        // Wait for TTX event
        MsOS_WaitEvent(s32VBITTXEventId, DEMO_VBITTX_EVT_TT_DONE,
                       &u32VBITTXEvents, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        if (_pu8VBITTX_Buff)
        {
            if (_u32VBITTX_VBICnt <= _u32VBITTX_NumOfRawData)
            {
                _u32VBITTX_VBICnt++;
            }
            else
            {
                // ring back
                _u32VBITTX_VBICnt = 0;
            }
            MDrv_VE_SetTtxBuffer(MsOS_VA2PA(
                    (MS_U32)(_pu8VBITTX_Buff + _u32VBITTX_VBICnt * TTX_PACKET_SIZE * LINECOUNT)),
                    TTX_PACKET_SIZE * LINECOUNT);
        }

        MsOS_DelayTask(1);
    }
}
//------------------------------------------------------------------------------
/// @brief VE set VBI ttx demo
/// \n Create the app demo task to read/write TTx
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_VbiTtxDemo   //Start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_VbiTtxDemo(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    // Create TTX Event

    if (s32VBITTXEventId < 0)
    {
        s32VBITTXEventId = MsOS_CreateEventGroup("VBITTX_Event");
        if (s32VBITTXEventId > 0)
        {
            db_print("[%s][%d] Event create ok\n",__FUNCTION__,__LINE__);
        }
        else
        {
            db_print("[%s][%d] create failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }

    // VBI TTX task
    if(s32VBITTXTaskID == -1)
    {
#ifdef MSOS_TYPE_ECOS
        _pVBITTXTaskStack = MsOS_AllocateMemory(VBITTX_TASK_STACK_SIZE, s32MstarCachedPoolID);

        if(_pVBITTXTaskStack == NULL)
        {
            db_print("Task stack create fail!\n");
            return FALSE;
        }

        s32VBITTXTaskID = MsOS_CreateTask((TaskEntry)_Demo_VE_VBITTX_Task,
                                            0,
                                            E_TASK_PRI_HIGH,
                                            TRUE,
                                            _pVBITTXTaskStack,
                                            VBITTX_TASK_STACK_SIZE,
                                            "VBITTX_Task");
#else   //linux
        s32VBITTXTaskID = MsOS_CreateTask((TaskEntry)_Demo_VE_VBITTX_Task,
                                            0,
                                            E_TASK_PRI_HIGH,
                                            TRUE,
                                            NULL,
                                            0,
                                            "VBITTX_Task");
#endif

        if(s32VBITTXTaskID == -1)
        {
            db_print("Task create fail!\n");
            return FALSE;
        }

        db_print("VBI TTX Task Create %"DTC_MS_U32_d"!\n", s32VBITTXTaskID);
    }
    else
    {
        db_print("Task Already Create!\n");
        return FALSE;
    }

    //read ttx raw data
    if (!_pu8VBITTX_Buff)
    {
        _pu8VBITTX_Buff = MsOS_AllocateMemory(sizeof(ttxRawData), s32MstarCachedPoolID);
        memcpy(_pu8VBITTX_Buff, ttxRawData, sizeof(ttxRawData));
        if (!_pu8VBITTX_Buff)
        {
            printf("Error: memory allocation error [%s][%d]\n", __FUNCTION__, __LINE__);
        }
    }

    _u32VBITTX_NumOfRawData = sizeof(ttxRawData) / TTX_PACKET_SIZE;
    _u32VBITTX_VBICnt = 0;

    MDrv_VE_EnableTtx(ENABLE);
    MDrv_VE_SetVbiTtxActiveLines(0xff);
    MDrv_VE_ClearTtxReadDoneStatus();
    MsOS_AttachInterrupt(E_INT_FIQ_VE_DONE_TT, (InterruptCb)_Demo_VE_VBITtx_ISR);

    // start ttvbi
    MsOS_EnableInterrupt(E_INT_FIQ_VE_DONE_TT);
    printf("Enable TTX VBI\n");
    MDrv_VE_SetTtxBuffer(MsOS_VA2PA(
                (MS_U32)(_pu8VBITTX_Buff + _u32VBITTX_VBICnt * TTX_PACKET_SIZE * LINECOUNT)),
            TTX_PACKET_SIZE * LINECOUNT);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief set ve output to CCIR656 or TV encoder
/// @param[in] pbEnable 1=CCIR656, 0=TV encoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b VE_CCIROut 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_CCIROut(MS_U32 *pbEnable)
{
    if (*pbEnable)
    {
        msAPI_VE_SetOutputCtrl(MS_VE_OUT_CCIR656);
    }
    else
    {
        msAPI_VE_SetOutputCtrl(MS_VE_OUT_TVENCODER);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief set WWS mode
/// @param[in] *mode a MS_U16 value of the following enum
///  typedef enum
///  1:   MS_WSS_4x3_FULL
///  2:   MS_WSS_14x9_LETTERBOX_CENTER
///  3:   MS_WSS_14x9_LETTERBOX_TOP
///  4:   MS_WSS_16x9_LETTERBOX_CENTER
///  5:   MS_WSS_16x9_LETTERBOX_TOP
///  6:   MS_WSS_ABOVE16x9_LETTERBOX_CENTER
///  7:   MS_WSS_14x9_FULL_CENTER
///  8:   MS_WSS_16x9_ANAMORPHIC
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetWSS  1\n
///          \b VE_SetWSS  8\n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetWSS(MS_U32 *mode)
{
    printf("[%s][%d] WSS mode: 0x%02"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, *mode);
    MS_U16 u16Mode = *mode;
    MS_U16 u16WSSData = 0, u16PreWSSData = 0;

    DEMO_XC_CHECK_INITIALIZED(E_MSAPI_XC_DEVICE1)
    DEMO_VE_CHECK_INITIALIZED

    switch (u16Mode)
    {
        case 1:
            u16WSSData = MS_WSS_4x3_FULL;
            break;
        case 2:
            u16WSSData =  MS_WSS_14x9_LETTERBOX_CENTER;
            break;
        case 3:
            u16WSSData =  MS_WSS_14x9_LETTERBOX_TOP;
            break;
        case 4:
            u16WSSData =  MS_WSS_16x9_LETTERBOX_CENTER;
            break;
        case 5:
            u16WSSData =  MS_WSS_16x9_LETTERBOX_TOP;
            break;
        case 6:
            u16WSSData =  MS_WSS_ABOVE16x9_LETTERBOX_CENTER;
            break;
        case 7:
            u16WSSData =  MS_WSS_14x9_FULL_CENTER;
            break;
        case 8:
            u16WSSData =  MS_WSS_16x9_ANAMORPHIC;
            break;
        default:
            printf("known WSS mode: 0x%02"DTC_MS_U32_x"\n", *mode);
            printf("1:   MS_WSS_4x3_FULL\n"
                    "2:   MS_WSS_14x9_LETTERBOX_CENTER\n"
                    "3:   MS_WSS_14x9_LETTERBOX_TOP\n"
                    "4:   MS_WSS_16x9_LETTERBOX_CENTER\n"
                    "5:   MS_WSS_16x9_LETTERBOX_TOP\n"
                    "6:   MS_WSS_ABOVE16x9_LETTERBOX_CENTER\n"
                    "7:   MS_WSS_14x9_FULL_CENTER\n"
                    "8:   MS_WSS_16x9_ANAMORPHIC\n");
            return FALSE;
    }
    u16PreWSSData = MDrv_VE_GetWSSData();
    u16WSSData = (u16PreWSSData & 0xFFF0) | u16WSSData;
    MDrv_VE_SetWSSData(ENABLE, u16WSSData);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief set 525 WWS mode
/// @param[in] *mode - a MS_U32 value of the following enum
///  typedef enum
///  1:   MS_WSS_4x3_FULL
///  2:   MS_WSS_14x9_LETTERBOX_CENTER
///  3:   MS_WSS_14x9_LETTERBOX_TOP
///  4:   MS_WSS_16x9_LETTERBOX_CENTER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_Set525WSS 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_Set525WSS(MS_U32 *mode)
{
    MS_U32 u32Mode = *mode;
    MS_U32 u32WSSData = 0, u32PreWSSData = 0;

    DEMO_XC_CHECK_INITIALIZED(E_MSAPI_XC_DEVICE1)
    DEMO_VE_CHECK_INITIALIZED

    printf("[%s][%d] WSS mode: 0x%08"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, *mode);

    switch (u32Mode) {
    case 1:
        u32WSSData = MS_WSS_525_4x3_NORMAL;
        break;
    case 2:
        u32WSSData =  MS_WSS_525_4x3_LETTERBOX;
        break;
    case 3:
        u32WSSData =  MS_WSS_525_16x9_ANAMORPHIC;
        break;
    case 4:
        u32WSSData =  MS_WSS_525_RESERVED;
        break;
    default:
        printf("unknown WSS mode: 0x%02"DTC_MS_U32_x"\n", u32Mode);
        printf("1:   MS_WSS_525_4x3_NORMAL\n"
               "2:   MS_WSS_525_4x3_LETTERBOX\n"
               "3:   MS_WSS_525_16x9_ANAMORPHIC\n"
               "4:   MS_WSS_525_RESERVED\n");
        return FALSE;
    }

    u32PreWSSData = MDrv_VE_GetWSS525Data();
    u32WSSData = (u32PreWSSData & 0xFFFC) | u32WSSData;

    if ( E_VE_OK != MDrv_VE_SetWSS525Data(ENABLE, u32WSSData) )
    {
        printf("[Error] MDrv_VE_SetWSS525Data return not OK!\n");
    }

    u32WSSData = MDrv_VE_GetWSS525Data();

    db_print("[%s][%d] WSS: 0x%08"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32WSSData);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief VE set frame color (background color)
/// @param[in] *pu32Index the color index of back ground color of VE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetFrameColor 0   //BLACK \n
///          \b VE_SetFrameColor 1   //WHITE \n
///          \b VE_SetFrameColor 2   //RED \n
///          \b VE_SetFrameColor 3   //GREEN \n
///          \b VE_SetFrameColor 4   //BLUE \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetFrameColor(MS_U32 *pu32Index)
{
    switch(*pu32Index)
    {
        default:
        case 0:
            MDrv_VE_SetFrameColor(0x00000000);
            break;
        case 1:
            MDrv_VE_SetFrameColor(0x00FFFFFF);
            break;
        case 2:
            MDrv_VE_SetFrameColor(0x00FF0000);
            break;
        case 3:
            MDrv_VE_SetFrameColor(0x0000FF00);
            break;
        case 4:
            MDrv_VE_SetFrameColor(0x000000FF);
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  Set VE Video Alpha \n
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetVideoAlpha 255 1 //Start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetVideoAlpha(MS_U8 *pu8Value, MS_U32 *pu32VOPOSDLayer)
{
    MDrv_VE_Set_VideoAlpha(*pu8Value);
    MDrv_VE_Set_OSDLayer(*pu32VOPOSDLayer);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Enable/disable video mute
/// @param[in] *pu32Enable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetVideoMute 0\n
///          \b VE_SetVideoMute 1\n
//------------------------------------------------------------------------------
MS_BOOL Demo_VE_SetVideoMute(MS_U32 *pu32Enable)
{
    if ( msAPI_VE_GetSourceType() == E_MSAPI_VE_SOURCE_SCALER_OP2)
    {
        return TRUE;
    }
    msAPI_VE_SetVideoMute((MS_BOOL)*pu32Enable);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief 625i(576i) systems are based on ITU-R BT.1119 and ETSI EN 309 294. \n
///         The Analog Copy Generation Management System(CGMS-A) is also supportted by the WSS signal.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b ACP_Set625CGMS_A   //Start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_ACP_Set625CGMS_A(MS_U16 *pu16Type)
{
    MS_U16 u16Wss = 0x0;
    E_CGMS_A_TYPE eCGMS_A = E_CGMS_A_INVALID;

    DEMO_XC_CHECK_INITIALIZED(E_MSAPI_XC_DEVICE1)
    DEMO_VE_CHECK_INITIALIZED
    eCGMS_A = (E_CGMS_A_TYPE)*pu16Type;
    u16Wss = MDrv_VE_GetWSSData();

    /*
    *  In 625i systems:
    *  CGMS-A consists of two bits of digital information:
    *  (b12, b13): (copyright, copy protection)
    *          00: copying permitted
    *          01: reserverd (One generation of copies has already been made; no further copying is allowed)
    *          10: one copy permitted
    *          11: no copying permitted
    */
    switch(eCGMS_A)
    {
        case E_CGMS_A_COPY_FREELY:
        {
            // CGMS-A: 00
            u16Wss = u16Wss & ~(MS_WSS_625_COPYRIGHT) & ~(MS_WSS_625_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NO_MORE:
        {
            // CGMS-A: 01
            u16Wss = (u16Wss & ~(MS_WSS_625_COPYRIGHT)) | MS_WSS_625_COPY_PROTECTION;
        }
        break;
        case E_CGMS_A_COPY_ONCE:
        {
            // CGMS-A: 10
            u16Wss = (u16Wss | MS_WSS_625_COPYRIGHT) & ~(MS_WSS_625_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NEVER:
        {
            // CGMS-A: 11
            u16Wss = u16Wss | MS_WSS_625_COPYRIGHT | MS_WSS_625_COPY_PROTECTION;
        }
        break;
        default:
            printf("[Warning!] Abnormal Case!\n");
            return FALSE;
    }

    MDrv_VE_SetWSSData(TRUE, u16Wss);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief EIA-J CPR-1204 and IEC 61880 define a widescreen signaling stnadard for 525i (480i) system \n
///         The Analog Copy Generation Management System(CGMS-A) is also supportted by the WSS signal. \n
///         Bits b6 and b7 specify the copy generation management system in an analog signal.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b ACP_Set525CGMS_A_IEC61880 0  //CGMS-A COPY_FREELY\n
///          \b ACP_Set525CGMS_A_IEC61880 3  //CGMS-A COPY_NEVER\n
//------------------------------------------------------------------------------
MS_BOOL Demo_ACP_Set525CGMS_A_IEC61880(MS_U16 *pu16Type)
{
    MS_U32 u32Wss = 0x0;
    E_CGMS_A_TYPE eCGMS_A = E_CGMS_A_INVALID;

    DEMO_XC_CHECK_INITIALIZED(E_MSAPI_XC_DEVICE1)
    DEMO_VE_CHECK_INITIALIZED

    eCGMS_A = (E_CGMS_A_TYPE)*pu16Type;
    u32Wss = MDrv_VE_GetWSS525Data();

    /*
     *  In 525i systems:
     *  CGMS-A consists of two bits of digital information:
     *  (b6, b7): (copyright, copy protection)
     *          00: copying permitted
     *          01: reserverd (One generation of copies has already been made; no further copying is allowed)
     *          10: one copy permitted
     *          11: no copying permitted
     */
    switch(eCGMS_A)
    {
        case E_CGMS_A_COPY_FREELY:
        {
            // CGMS-A: 00
            u32Wss = u32Wss & ~(MS_WSS_525_COPYRIGHT) & ~(MS_WSS_525_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NO_MORE:
        {
            // CGMS-A: 01
            u32Wss = (u32Wss & ~(MS_WSS_525_COPYRIGHT)) | MS_WSS_525_COPY_PROTECTION;
            break;
        }
        break;
        case E_CGMS_A_COPY_ONCE:
        {
            // CGMS-A: 10
            u32Wss = (u32Wss | MS_WSS_525_COPYRIGHT) & ~(MS_WSS_525_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NEVER:
        {
            // CGMS-A: 11
            u32Wss = u32Wss | MS_WSS_525_COPYRIGHT | MS_WSS_525_COPY_PROTECTION;
        }
        break;
        default:
            printf("[Warning!] Abnormal Case!\n");
            return FALSE;
    }

    if ( E_VE_OK != MDrv_VE_SetWSS525Data(TRUE, u32Wss))
    {
        printf("[Error] MDrv_VE_SetWSS525Data return not OK!\n");
    }

    u32Wss = MDrv_VE_GetWSS525Data();

    db_print("[%s][%d] WSS: 0x%08"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32Wss);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief EIA-J CPR-1204 and IEC 61880 define a widescreen signaling stnadard for 525i (480i) system \n
///         Bits b8 and b9 specify the Analog Protection Service (APS) added to the analog NTSC or PAL video signal.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b ACP_Set525APS_IEC61880   //APS \n
//------------------------------------------------------------------------------
MS_BOOL Demo_ACP_Set525APS_IEC61880(MS_U16 *pu16Type)
{
    MS_U32 u32Wss = 0, u32PreWss = 0;
    E_APS_TYPE eAPStype = E_APS_INVALID;

    DEMO_XC_CHECK_INITIALIZED(E_MSAPI_XC_DEVICE1)
    DEMO_VE_CHECK_INITIALIZED

    eAPStype = (E_APS_TYPE)*pu16Type;
    u32Wss = MDrv_VE_GetWSS525Data();

    /*
     *  In 525i systems:
     *  APS consists of two bits of digital information:
     *  (b8, b9): specify the APS type for 525i system
     *          00: no Analog Protection Service
     *          01: PSP on, color striping off
     *          10: PSP on, 2-line color striping on
     *          11: PSP on, 4-line color striping on
     */
    switch(eAPStype)
    {
        case E_APS_NO_APS:
        {
            // APS: 00
            u32Wss = MS_WSS_525_APS_NO_APS;
        }
        break;
        case E_APS_PSP_ONLY:
        {
            // APS: 01
            u32Wss = MS_WSS_525_APS_PSP_CS_OFF;
            break;
        }
        break;
        case E_APS_PSP_2_LINE_CB:
        {
            // APS: 10
            u32Wss = MS_WSS_525_APS_PSP_2_LINE_CS;
        }
        break;
        case E_APS_PSP_4_LINE_CB:
        {
            // APS: 11
            u32Wss = MS_WSS_525_APS_PSP_4_LINE_CS;
        }
        break;
        default:
            printf("[Warning!] Abnormal Case!\n");
            return FALSE;
    }

    u32PreWss = MDrv_VE_GetWSS525Data();
    u32Wss = (u32PreWss & 0xFCFF) | u32Wss;

    if ( E_VE_OK != MDrv_VE_SetWSS525Data(TRUE, u32Wss))
    {
        printf("[Error] MDrv_VE_SetWSS525Data return not OK!\n");
    }

    u32Wss = MDrv_VE_GetWSS525Data();

    db_print("[%s][%d] WSS: 0x%08"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32Wss);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief this demo code is implemented base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @param[in] *pu32index the MV Types
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b VE_SetMV 0 0   //DISABLE MV \n
///          \b ACP_SetMV 1 2   //ENABLE MV NTSC TYPE2 \n
///          \b ACP_SetMV 0 20  \n
///          \b ACP_SetMV 1 20  //ENABLE MV PAL TYPE1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_ACP_SetMV(MS_U32 *pu32Enable, MS_BOOL *pbIsYPbPr, MS_U32 *pu32Index)
{
    MS_BOOL bEnable =  *pu32Enable;
    MS_BOOL bIsYPbPr =  *pbIsYPbPr;
    MS_ACP_TYPE eACPType = (MS_ACP_TYPE)(*pu32Index);
    ACP_Result eRet = E_ACP_FAIL;

    DEMO_XC_CHECK_INITIALIZED(E_MSAPI_XC_DEVICE1)
    DEMO_VE_CHECK_INITIALIZED

    /*
     * For normal use, we recommend following macrovision types to be used in each case:
     * - for NTSC:
     *   MS_ACP_CVBS_NTSC_TYPE2 or MS_ACP_CVBS_NTSC_TYPE3
     *
     * - for NTSC with SCART RGB:
     *   MS_ACP_CVBS_NTSC_TEST_N01 or MS_ACP_CVBS_NTSC_TEST_N02
     *
     * - for PAL or PAL with SCART RGB:
     *   MS_ACP_CVBS_PAL_TEST_P01 or MS_ACP_CVBS_PAL_TEST_P02
     *
     * - for PAL-N, PAL-Nc and SECAM:
     *   MS_ACP_CVBS_PAL_TYPE2
     */
    if (eACPType >= MS_ACP_TYPE_MAX)
    {
        return FALSE;
    }

    /*
     * Note:
     * YPbPr Macrovision function is not ready, please set the second parameter to FALSE
     */
    eRet = MApi_ACP_SetProtection(bEnable, bIsYPbPr, eACPType);
    if( eRet != E_ACP_OK )
    {
        db_print("ACP set Macrovision Failed, return value = %d \n", eRet);
        return FALSE;
    }

    return TRUE;
}

#if (DEMO_ACP_DCS_TEST == 1)
//------------------------------------------------------------------------------
/// @brief this demo code is implemented DCS analog copy protection
/// @param[in] *pbEnable 1=ENABLE, 0=DISABLE
/// @param[in] *pu16DCSIdx is the DCS Types
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Sample Command: \n
///         \b ACP_SetDCS 1 0   //ENABLE  CVBS NTSC DCS  \n
///         \b ACP_SetDCS 0 0   //DISABLE CVBS NTSC DCS  \n
///         \b ACP_SetDCS 1 1   //ENABLE  CVBS PAL  DCS  \n
///         \b ACP_SetDCS 0 1   //DISABLE CVBS PAL  DCS  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_ACP_SetDCS(MS_BOOL *pbEnable, MS_U16 *pu16DCSIdx)
{
    ACP_Result eRet = E_ACP_FAIL;
    MS_ACP_DCS_TYPE eDCS_ACP_Idx = (MS_ACP_DCS_TYPE)*pu16DCSIdx;

    //****************************************************
    // DCS ACTIVATION KEY
    // Key TBL DCSKEYTBL is 4*8 array
    //  The first 4 is order by
    //      DCS STATE OFF_TO_TRANSITIONON
    //      DCS STATE TRANSITIONON_TO_ON
    //      DCS STATE ON_TO_TRANSITIONOFF
    //      DCS STATE TRANSITIONOFF_TO_OFF
    //  And the second 8 is order by
    //      [7:0], [15:8], [23:16], [31:24], [39:32], [47:40], [55:48], [63:56] (bit)
    // Ex: 0x1122334455667788 => 0x88, 0x77, ..., 0x22, 0x11
    //****************************************************
    MS_U8 DCSKEYTBL[4][8] =
    {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    eRet = MApi_DCS_SetActivationKey(&(DCSKEYTBL[0][0]), 4*8);
    if( eRet != E_ACP_OK )
    {
        db_print("ACP set DCS Activation Key Failed, return value = %d \n", eRet);
        return FALSE;
    }

    eRet = MApi_DCS_SetProtection(*pbEnable, (MS_U8)eDCS_ACP_Idx);

    if( eRet != E_ACP_OK )
    {
        db_print("ACP set DCS Index %d Failed, return value = %d \n", *pu16DCSIdx, eRet);
        return FALSE;
    }

    return TRUE;
}
#endif

#if (DEMO_XC_HDMIRX_TEST == 1)
//------------------------------------------------------------------------------
/// @brief this demo code is implemented base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @param[in] *pMode Input Freq
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetHDMIByPass 0   //DISABLE Bypass Mode\n
///          \b XC_SetHDMIByPass 1   //ENABLE  Bypass Mode\n
//------------------------------------------------------------------------------
MS_BOOL Demo_HDMI_SetRxBypass(MS_U32 *pu32Enable)
{
    E_MSAPI_XC_RESULT eRet = E_MSAPI_XC_FAIL;

    if(*pu32Enable == 0)
    {
        eRet = msAPI_XC_SetHDMIByPassOnOff(FALSE, HDMIRX_RXBYPASS_PIN);
    }
    else if(*pu32Enable == 1)
    {
        eRet = msAPI_XC_SetHDMIByPassOnOff(TRUE, HDMIRX_RXBYPASS_PIN);
    }

    if(eRet != E_MSAPI_XC_OK)
    {
        printf("[%s][%d] HDMI SetRxBypass Failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Enable/disable colorthrough mode
/// @param[in] *bEn the colorthrough mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetHDMIColorThroughMode 1\n
///          \b XC_SetHDMIColorThroughMode 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_HDMI_SetColorThroughMode(MS_U32 *bEn)
{
    msAPI_XC_HDMIRx_ColorThroughMode(*bEn);
    return TRUE;
}

#endif

//------------------------------------------------------------------------------
/// @brief Set HDMI Tx output mode
/// @param[in] *u32OutputModemode a MS_U32 value of the following enum
///  typedef enum
///  0:   E_MSAPI_XC_HDMITX_OUTPUT_HDMI
///  1:   E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP
///  2:   E_MSAPI_XC_HDMITX_OUTPUT_DVI
///  3:   E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b  HDMI_SetTxOutputMode 0  \n
//------------------------------------------------------------------------------
MS_BOOL Demo_HDMI_SetTxOutputMode(MS_U32 *u32OutputMode)
{

    MApi_HDMITx_UnHDCPRxControl(E_UNHDCPRX_BLUE_SCREEN);
    MApi_HDMITx_HDCPRxFailControl(E_HDCPRXFail_BLUE_SCREEN);

    msAPI_XC_SetHDMITxMode((E_MSAPI_XC_HDMITX_OUTPUT_MODE)*u32OutputMode);

    printf("Switch to %s%s mode!\n", (*u32OutputMode & 0x2)?"DVI":"HDMI",
                                     (*u32OutputMode & 0x1)?"_HDCP":"");
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set HDMI output color depth
/// @param[in] *pu32CDType the color depth type
///     -    8bit      = 0,
///     -    10bit     = 1,
///     -    12bit     = 2,
///     -    16bit     = 3,
///     -    auto      = 4,
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b HDMI_SetOutputColorDepth 0 \n
///          \b HDMI_SetOutputColorDepth 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_HDMI_SetOutputColorDepth(MS_U32 *pu32CDType)
{
    db_print("Set color depth to %"DTC_MS_U32_u" \n", *pu32CDType);

    msAPI_XC_EnableMiscOutput(DISABLE);

    if(E_MSAPI_XC_OK != msAPI_XC_SetHDMITxCD((E_MSAPI_XC_HDMITX_CD_TYPE)(*pu32CDType)))
    {
        printf("Set color depth Fail or Not Support! \n");
        return FALSE;
    }

    msAPI_XC_EnableMiscOutput(ENABLE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC command menu
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:\n
///         \b HelpXC \n
//------------------------------------------------------------------------------
MS_BOOL Demo_XC_Help(void)
{
    printf ("************************* XC function List *************************\n");
    printf ("1. XC_Init [ScalerDevice]\n");
    printf ("-----ScalerDevice : Scaler device\n");
    printf ("---------------: SC0 = 0,\n");
    printf ("---------------: SC1 = 1,\n");
    printf ("2. XC_SetOutputTiming [ScalerDevice] [OutTiming]\n");
    printf ("-----OutTiming : Output Resolution\n");
    printf ("---------------: E_OUTPUT_TIMING_720X480_60I = 0,\n");
    printf ("---------------: E_OUTPUT_TIMING_720X480_60P = 1,\n");
    printf ("---------------: E_OUTPUT_TIMING_720X576_50I = 2,\n");
    printf ("---------------: E_OUTPUT_TIMING_720X576_50P = 3,\n");
    printf ("---------------: E_OUTPUT_TIMING_1280X720_50P = 4,\n");
    printf ("---------------: E_OUTPUT_TIMING_1280X720_60P = 5,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_50I = 6,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_60I = 7,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_24P = 8,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_25P = 9,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_30P = 10,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_50P = 11,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_60P = 12,\n");
    printf ("---------------: E_OUTPUT_TIMING_640X480_60P = 13,\n");
    printf ("====>example : XC_SetOutputTiming 0 6\n");
    printf ("====>example : XC_SetOutputTiming 1 1\n");
    printf ("3. XC_SetOutputDest [OutputDest] [bEnable]\n");
    printf ("-----OutputDest : Output Destination\n");
    printf ("---------------: E_DDI_OUTPUT_DEST_SD_CVBS = 0,\n");
    printf ("---------------: E_DDI_OUTPUT_DEST_SD_SVIDEO = 1,\n");
    printf ("---------------: E_DDI_OUTPUT_DEST_SD_SCART_RGB = 2,\n");
    printf ("---------------: E_DDI_OUTPUT_DEST_SD_YPBPR = 3,\n");
    printf ("---------------: E_DDI_OUTPUT_DEST_HD_COMPONENT = 4,\n");
    printf ("---------------: E_DDI_OUTPUT_DEST_HD_HDMITX = 5,\n");
    printf ("====>example : XC_SetOutputDest 4 1\n");
    printf ("====>example : XC_SetOutputDest 0 0\n");
    printf ("4. XC_PlayVideo [ScalerDevice] [ScalerWindow] [InputSrc] \n");
    printf ("-----InputSrc : Input Source\n");
    printf ("---------------: DTV = 0,\n");
    printf ("---------------: DTV2 = 1,\n");
    printf ("---------------: HDMIRx = 2,\n");
    printf ("---------------: SC0 OP = 3,\n");
    printf ("====>example : XC_PlayVideo 0 0 0\n");
    printf ("5. XC_PlayCusVideo [ScalerDevice] [ScalerWindow] [InputSrc] \n");
    printf ("6. XC_SetCropWin [ScalerDevice] [ScalerWindow] [x] [y] [width] [height] \n");
    printf ("====>example : XC_SetCropWin 0 0 0 0 360 240\n");
    printf ("7. XC_SetDestWin [ScalerDevice] [ScalerWindow] [x] [y] [width] [height] \n");
    printf ("====>example : XC_SetDestWin 0 0 360 240 920 480\n");
    printf ("8. XC_SetAS [ScalerDevice] [ScalerWindow] [eAspectRatio] \n");
    printf ("-----eAspectRatio : Aspect Ratio\n");
    printf ("---------------: E_ASPECT_RATIO_NORMAL = 0,\n");
    printf ("---------------: E_ASPECT_RATIO_4_3 = 1,\n");
    printf ("---------------: E_ASPECT_RATIO_16_9 = 2,\n");
    printf ("---------------: E_ASPECT_RATIO_CUT_4_3 = 3,\n");
    printf ("---------------: E_ASPECT_RATIO_CUT_16_9 = 4,\n");
    printf ("---------------: E_ASPECT_RATIO_FULL = 5,\n");
    printf ("====>example : XC_SetAS 0 0 5\n");
    printf ("9. XC_SetVideoMute [ScalerDevice] [ScalerWindow] [bEnable] \n");
    printf ("10. XC_RegressionTest_ScalingDown [ScalerDevice] [ScalerWindow] [InputSrc] \n");
    printf ("====>example : XC_RegressionTest_ScalingDown 0 0 0\n");
    printf ("11. XC_EnableSeamlessZapping [ScalerDevice] [ScalerWindow] [bEnable] \n");
    printf ("12. XC_SeamlessZapping [ScalerDevice] [ScalerWindow] [InputSrc] \n");
    printf ("13. XC_SeamlessZapping_SetFreeze [ScalerDevice] [ScalerWindow] [bEnable] \n");
    printf ("14. XC_Enable3D [ScalerDevice] [bEnable] \n");
    printf ("15. XC_PlayVideo_3D [ScalerDevice] [InputSrc] \n");
    printf ("16. XC_SetBrightness [ScalerDevice] [ScalerWindow] [value(0~255)]\n");
    printf ("====>example : XC_SetBrightness 0 0 128\n");
    printf ("17. XC_SetSharpness [ScalerDevice] [ScalerWindow] [value(0~63)]\n");
    printf ("====>example : XC_SetSharpness 0 0 50\n");
    printf ("18. XC_SetContrast [ScalerDevice] [ScalerWindow] [value(0~255)]\n");
    printf ("====>example : XC_SetContrast 0 0 50\n");
    printf ("19. XC_SetHue [ScalerDevice] [ScalerWindow] [value(0~100)]\n");
    printf ("====>example : XC_SetHue 0 0 50\n");
    printf ("20. XC_SetSaturation [ScalerDevice] [ScalerWindow] [value(0~255)]\n");
    printf ("====>example : XC_SetSaturation 0 0 128\n");
    printf ("21. XC_SetFrameColor [ScalerDevice] [ColorIndex] \n");
    printf ("-----[ColorIndex] :\n");
    printf ("---------------: BLACK = 0 \n");
    printf ("---------------: WHITE = 1 \n");
    printf ("---------------: RED = 2 \n");
    printf ("---------------: GREEN = 3 \n");
    printf ("---------------: BLUE = 4 \n");
    printf ("====>example : XC_SetFrameColor 0 1   //WHITE \n");
    printf ("====>example : XC_SetFrameColor 0 2   //RED \n");
    printf ("22. XC_EnableDrvDmsg [bEnable] \n");
    printf ("====>example : XC_EnableDrvDmsg 0 \n");
    printf ("====>example : XC_EnableDrvDmsg 1 \n");
    printf ("23. XC_Exit 0 \n");
    printf ("24. VE_SetWSS [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: MS_WSS_4x3_FULL               = 1 \n");
    printf ("---------------: MS_WSS_14x9_LETTERBOX_CENTER  = 2 \n");
    printf ("---------------: MS_WSS_14x9_LETTERBOX_TOP     = 3 \n");
    printf ("---------------: MS_WSS_16x9_LETTERBOX_CENTER  = 4 \n");
    printf ("---------------: MS_WSS_16x9_LETTERBOX_TOP     = 5 \n");
    printf ("---------------: MS_WSS_ABOVE16x9_LETTERBOX_CENTER = 6 \n");
    printf ("---------------: MS_WSS_14x9_FULL_CENTER       = 7 \n");
    printf ("---------------: MS_WSS_16x9_ANAMORPHIC        = 8 \n");
    printf ("====>example : VE_SetWSS 1     //MS_WSS_4x3_FULL \n");
    printf ("====>example : VE_SetWSS 4     //MS_WSS_16x9_LETTERBOX_CENTER \n");
    printf ("25. VE_Set525WSS [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: MS_WSS_525_4x3_NORMAL         = 1 \n");
    printf ("---------------: MS_WSS_525_4x3_LETTERBOX      = 2 \n");
    printf ("---------------: MS_WSS_525_16x9_ANAMORPHIC    = 3 \n");
    printf ("---------------: MS_WSS_525_RESERVED           = 4 \n");
    printf ("====>example : VE_Set525WSS 1     //MS_WSS_525_4x3_NORMAL \n");
    printf ("====>example : VE_Set525WSS 3     //MS_WSS_525_16x9_ANAMORPHIC \n");
    printf ("26. ACP_Set625CGMS_A [type] \n");
    printf ("---------------: 0 = copying permitted \n");
    printf ("---------------: 1 = reserverd \n");
    printf ("---------------: 2 = one copy permitted \n");
    printf ("---------------: 3 = no copying permitted \n");
    printf ("====>example : ACP_Set625CGMS_A 0 \n");
    printf ("====>example : ACP_Set625CGMS_A 3 \n");
    printf ("27. ACP_Set525CGMS_A_IEC61880 [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: E_CGMS_A_COPY_FREELY      = 0 \n");
    printf ("---------------: E_CGMS_A_COPY_NO_MORE     = 1 \n");
    printf ("---------------: E_CGMS_A_COPY_ONCE        = 2 \n");
    printf ("---------------: E_CGMS_A_COPY_NEVER       = 3 \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 0   //E_CGMS_A_COPY_FREELY \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 3   //E_CGMS_A_COPY_NEVER \n");
    printf ("28. ACP_Set525APS_IEC61880 [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: E_APS_NO_APS              = 0 \n");
    printf ("---------------: E_APS_PSP_ONLY            = 1 \n");
    printf ("---------------: E_APS_PSP_2_LINE_CB       = 2 \n");
    printf ("---------------: E_APS_PSP_4_LINE_CB       = 3 \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 1   //E_APS_NO_APS \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 3   //E_APS_PSP_4_LINE_CB \n");
    printf ("29. ACP_SetMV [type] \n");
    printf ("-----[bEnable] :\n");
    printf ("---------------: 0 = disable\n");
    printf ("---------------: 1 = enable\n");
    printf ("-----[eType] :\n");
    printf ("---------------: MS_ACP_CVBS_NTSC_TYPE1    = 0x0 \n");
    printf ("---------------: MS_ACP_CVBS_NTSC_TYPE2    = 0x1 \n");
    printf ("---------------: MS_ACP_CVBS_NTSC_TYPE3    = 0x2 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE1     = 0x10 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE2     = 0x11 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE3     = 0x12 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE4     = 0x13 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE5     = 0x14 \n");
    printf ("====>example : ACP_SetMV 1 0x2   //ENABLE MV NTSC TYPE2 \n");
    printf ("====>example : ACP_SetMV 1 0x14  //ENABLE MV PAL Test P02 \n");
    printf ("====>example : ACP_SetMV 0 0x14  //DISABLE MV PAL Test P02 \n");
    return TRUE;
}

#endif

