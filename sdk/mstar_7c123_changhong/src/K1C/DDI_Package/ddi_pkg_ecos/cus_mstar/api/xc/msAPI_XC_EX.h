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
/// file   msAPI_XC_EX.h
/// @brief  msAPI_XC Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MSAPI_XC_H_
#define _MSAPI_XC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSAPI_XC_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//  library information
#define MSAPI_XC_LIB_CODE               {'M','S','X','C'}
#define MSAPI_XC_LIBVER                 {'0','2'}
#define MSAPI_XC_BUILDNUM               {'3','9'}
#define MSAPI_XC_CHANGELIST             {'0','0','5','9','1','9','8','7'}
#define MSAPI_XC_API_VERSION            /* Character String for MSAPI_XC version            */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSAPI_XC_LIB_CODE  ,                /* IP__                                             */  \
    MSAPI_XC_LIBVER    ,                /* 0.00 ~ Z.Z                                       */  \
    MSAPI_XC_BUILDNUM  ,                /* 00 ~ 99                                          */  \
    MSAPI_XC_CHANGELIST                 /* previous CL#                                              */
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSAPI_SWDRHISTOGRAM_INDEX 32
//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MSAPI_XC_DEVICE0      = 0,
    E_MSAPI_XC_DEVICE1,
    E_MSAPI_XC_DEVICE_DIP_0 = 0x10,
    E_MSAPI_XC_DEVICE_DIP_1,
    E_MSAPI_XC_DEVICE_MAX
} E_MSAPI_XC_DEVICE_NUM;

typedef enum
{
    E_MSAPI_XC_MAIN_WINDOW  = 0,
    E_MSAPI_XC_SUB_WINDOW,
    E_MSAPI_XC_WINDOW_0 = E_MSAPI_XC_MAIN_WINDOW,
    E_MSAPI_XC_WINDOW_1,
    E_MSAPI_XC_WINDOW_2,
    E_MSAPI_XC_WINDOW_3,
    E_MSAPI_XC_MAX_WINDOW
} E_MSAPI_XC_WINDOW;

typedef enum
{
    E_MSAPI_XC_DIP_FMT_YUV422         = 0,
    /// CrYCb domain.
    E_MSAPI_XC_DIP_FMT_RGB565,
    /// RGB domain (CSC bypass).
    E_MSAPI_XC_DIP_FMT_ARGB8888,
    /// YUV420
    E_MSAPI_XC_DIP_FMT_YUV420,
    E_MSAPI_XC_DIP_FMT_MAX
} E_MSAPI_XC_DIP_FMT;

typedef enum
{
    E_MSAPI_XC_INPUT_SOURCE_VGA = 0,           ///<0   VGA input
    E_MSAPI_XC_INPUT_SOURCE_TV,            ///<1   TV input

    E_MSAPI_XC_INPUT_SOURCE_CVBS,          ///<2   AV 1
    E_MSAPI_XC_INPUT_SOURCE_CVBS2,         ///<3   AV 2
    E_MSAPI_XC_INPUT_SOURCE_CVBS3,         ///<4   AV 3
    E_MSAPI_XC_INPUT_SOURCE_CVBS4,         ///<5   AV 4
    E_MSAPI_XC_INPUT_SOURCE_CVBS5,         ///<6   AV 5
    E_MSAPI_XC_INPUT_SOURCE_CVBS6,         ///<7   AV 6
    E_MSAPI_XC_INPUT_SOURCE_CVBS7,         ///<8   AV 7
    E_MSAPI_XC_INPUT_SOURCE_CVBS8,         ///<9   AV 8
    E_MSAPI_XC_INPUT_SOURCE_CVBS_MAX,      ///<10 AV max

    E_MSAPI_XC_INPUT_SOURCE_SVIDEO,        ///<11 S-video 1
    E_MSAPI_XC_INPUT_SOURCE_SVIDEO2,       ///<12 S-video 2
    E_MSAPI_XC_INPUT_SOURCE_SVIDEO3,       ///<13 S-video 3
    E_MSAPI_XC_INPUT_SOURCE_SVIDEO4,       ///<14 S-video 4
    E_MSAPI_XC_INPUT_SOURCE_SVIDEO_MAX,    ///<15 S-video max

    E_MSAPI_XC_INPUT_SOURCE_YPBPR,         ///<16 Component 1
    E_MSAPI_XC_INPUT_SOURCE_YPBPR2,        ///<17 Component 2
    E_MSAPI_XC_INPUT_SOURCE_YPBPR3,        ///<18 Component 3
    E_MSAPI_XC_INPUT_SOURCE_YPBPR_MAX,     ///<19 Component max

    E_MSAPI_XC_INPUT_SOURCE_SCART,         ///<20 Scart 1
    E_MSAPI_XC_INPUT_SOURCE_SCART2,        ///<21 Scart 2
    E_MSAPI_XC_INPUT_SOURCE_SCART_MAX,     ///<22 Scart max

    E_MSAPI_XC_INPUT_SOURCE_HDMI,          ///<23 HDMI 1
    E_MSAPI_XC_INPUT_SOURCE_HDMI2,         ///<24 HDMI 2
    E_MSAPI_XC_INPUT_SOURCE_HDMI3,         ///<25 HDMI 3
    E_MSAPI_XC_INPUT_SOURCE_HDMI4,         ///<26 HDMI 4
    E_MSAPI_XC_INPUT_SOURCE_HDMI_MAX,      ///<27 HDMI max

    E_MSAPI_XC_INPUT_SOURCE_DTV,           ///<28 DTV

    E_MSAPI_XC_INPUT_SOURCE_DVI,           ///<29 DVI 1
    E_MSAPI_XC_INPUT_SOURCE_DVI2,          ///<30 DVI 2
    E_MSAPI_XC_INPUT_SOURCE_DVI3,          ///<31 DVI 2
    E_MSAPI_XC_INPUT_SOURCE_DVI4,          ///<32 DVI 4
    E_MSAPI_XC_INPUT_SOURCE_DVI_MAX,       ///<33 DVI max

    // Application source
    E_MSAPI_XC_INPUT_SOURCE_STORAGE,       ///<34 Storage
    E_MSAPI_XC_INPUT_SOURCE_KTV,           ///<35 KTV
    E_MSAPI_XC_INPUT_SOURCE_JPEG,          ///<36 JPEG

    //Support Dual MVOP port
    E_MSAPI_XC_INPUT_SOURCE_DTV2,          ///<37 DTV
    E_MSAPI_XC_INPUT_SOURCE_STORAGE2,	    ///<38 Storage

    //Support Third MVOP port
    E_MSAPI_XC_INPUT_SOURCE_DTV3,          ///<39 DTV

    // Support OP capture
    E_MSAPI_XC_INPUT_SOURCE_SCALER_OP,     ///<40 scaler OP

    E_MSAPI_XC_INPUT_SOURCE_VGA2,          /// <41 VGA2
    E_MSAPI_XC_INPUT_SOURCE_VGA3,          /// <42 VGA3

    E_MSAPI_XC_INPUT_SOURCE_SC0_DI,        ///<43 scaler0 DI

    E_MSAPI_XC_INPUT_SOURCE_NUM,           ///<44 number of the source
    E_MSAPI_XC_INPUT_SOURCE_NONE = E_MSAPI_XC_INPUT_SOURCE_NUM,    ///<NULL input
} E_MSAPI_XC_INPUT_SRC;

#define MSAPI_XC_IsSrcTypeAV(x)             (((x)>=E_MSAPI_XC_INPUT_SOURCE_CVBS)&&((x)<=E_MSAPI_XC_INPUT_SOURCE_CVBS_MAX))/// x is AV
#define MSAPI_XC_IsSrcTypeSV(x)             (((x)>=E_MSAPI_XC_INPUT_SOURCE_SVIDEO)&&((x)<=E_MSAPI_XC_INPUT_SOURCE_SVIDEO_MAX))///< x is SV
#define MSAPI_XC_IsSrcTypeATV(x)            ((x)==E_MSAPI_XC_INPUT_SOURCE_TV)///< x is ATV
#define MSAPI_XC_IsSrcTypeScart(x)          (((x)>=E_MSAPI_XC_INPUT_SOURCE_SCART)&&((x)<=E_MSAPI_XC_INPUT_SOURCE_SCART_MAX))///< x is Scart
#define MSAPI_XC_IsSrcTypeHDMI(x)           (((x)>=E_MSAPI_XC_INPUT_SOURCE_HDMI)&&((x)<=E_MSAPI_XC_INPUT_SOURCE_HDMI_MAX))///< x is HDMI
#define MSAPI_XC_IsSrcTypeVga(x)            ((x)==E_MSAPI_XC_INPUT_SOURCE_VGA)///< x is VGA
#define MSAPI_XC_IsSrcTypeYPbPr(x)          (((x)>=E_MSAPI_XC_INPUT_SOURCE_YPBPR)&&((x)<=E_MSAPI_XC_INPUT_SOURCE_YPBPR_MAX))///<  x is YPbPr
#define MSAPI_XC_IsSrcTypeDTV(x)            (((x)==E_MSAPI_XC_INPUT_SOURCE_DTV) || ((x)==E_MSAPI_XC_INPUT_SOURCE_DTV2) || ((x)==E_MSAPI_XC_INPUT_SOURCE_DTV3))///< x is DTV
#define MSAPI_XC_IsSrcTypeDVI(x)            (((x)>=E_MSAPI_XC_INPUT_SOURCE_DVI)&&((x)<=E_MSAPI_XC_INPUT_SOURCE_DVI_MAX))///< x is DVI
#define MSAPI_XC_IsSrcTypeJpeg(x)           ((x)==E_MSAPI_XC_INPUT_SOURCE_JPEG)///< x is JPEG
#define MSAPI_XC_IsSrcTypeStorage(x)        (((x)==E_MSAPI_XC_INPUT_SOURCE_STORAGE) || ((x)==E_MSAPI_XC_INPUT_SOURCE_STORAGE2) || XC_EX_IsSrcTypeJpeg(x))///< x is Storage
#define MSAPI_XC_IsSrcTypeDigitalVD(x)      ( XC_EX_IsSrcTypeAV(x)||XC_EX_IsSrcTypeSV(x)||XC_EX_IsSrcTypeATV(x)||XC_EX_IsSrcTypeScart(x))///< x is VD
#define MSAPI_XC_IsSrcTypeAnalog(x)         ( XC_EX_IsSrcTypeVga(x)|| XC_EX_IsSrcTypeYPbPr(x))///< x is Analog
#define MSAPI_XC_IsSrcTypeVideo(x)          ( XC_EX_IsSrcTypeYPbPr(x) || XC_EX_IsSrcTypeATV(x) || XC_EX_IsSrcTypeAV(x) || XC_EX_IsSrcTypeSV(x) || XC_EX_IsSrcTypeScart(x) || XC_EX_IsSrcTypeDTV(x) ) ///< x is Video
#define MSAPI_XC_IsSrcTypeCapture(x)        ((x)==E_MSAPI_XC_INPUT_SOURCE_SCALER_OP)///< x is scaler OP


typedef enum
{
    E_MSAPI_XC_NOT_SUPPORT      = -1,
    E_MSAPI_XC_FAIL             = 0,
    E_MSAPI_XC_OK               = 1,
    E_MSAPI_XC_INVALID_PARAM    = 2,
    E_MSAPI_XC_HDMITX_RX_NOT_READY        = 10, // HDMITX related enum
    E_MSAPI_XC_HDMITX_RX_NOT_SUPPORT      = 11,
} E_MSAPI_XC_RESULT;

typedef enum
{
    E_XC_PQ_3D_NR_OFF,
    E_XC_PQ_3D_NR_LOW,
    E_XC_PQ_3D_NR_MID,
    E_XC_PQ_3D_NR_HIGH,
    E_XC_PQ_3D_NR_AUTO,
    E_XC_PQ_3D_NR_DEFAULT,
    E_XC_PQ_3D_NR_NUM = E_XC_PQ_3D_NR_AUTO,
}E_XC_PQ_3D_NR;

typedef enum
{
    E_MSAPI_XC_ASPECT_RATIO_NORMAL              = 0,        // unchange the window setting
    E_MSAPI_XC_ASPECT_RATIO_4_3                 = 1,        // pillarbox 4x3 image on 16x9 display
    E_MSAPI_XC_ASPECT_RATIO_16_9                = 2,        // letterbox 16x9 image on 4x3 display
    E_MSAPI_XC_ASPECT_RATIO_CUT_4_3             = 3,        // 4x3 center cutout
    E_MSAPI_XC_ASPECT_RATIO_CUT_16_9            = 4,        // 16x9 center cutout
    E_MSAPI_XC_ASPECT_RATIO_FULL                = 5,        // Image epands to fill display
    E_MSAPI_XC_ASPECT_RATIO_LETTERBOX           = 6,        // pillarbox 4x3 image on 16x9 display OR letterbox 16x9 image on 4x3 display
    E_MSAPI_XC_ASPECT_RATIO_CUTOUT              = 7,        // 16x9 center cutout of 4x3 image on 16x9 display OR 4x3 center cutout of 16x9 image on 4x3 display
    E_MSAPI_XC_ASPECT_RATIO_14_9                = 8,        // zoom 16x9 image on 16x9 TV to 6/7 size (14x9 SP area on 4x3 centre)
    E_MSAPI_XC_ASPECT_RATIO_CUT_14_9            = 9,        // 14x9 center cutout of 4x3 image on 16x9 display OR 14x9 center cutout of 16x9 image on 4x3 display
    E_MSAPI_XC_ASPECT_RATIO_CUSTOM1             = 10,       // customer Ratio
    E_MSAPI_XC_ASPECT_RATIO_AUTO                = 11,       // dynamic Ratio
    E_MSAPI_XC_ASPECT_RATIO_MSTK_START          = 0x80,     // for MSTAR TK Usage
    E_MSAPI_XC_ASPECT_RATIO_MSTK_LB_4_3         = 0x80,     // MSTAR TK 4:3 letterbOX
    E_MSAPI_XC_ASPECT_RATIO_MSTK_PB_16_9        = 0x81,     // MSTAR TK 16:9 pillarbox
    E_MSAPI_XC_ASPECT_RATIO_MSTK_PS_4_3         = 0x82,     // MSTAR TK 4:3 pan&scan
    E_MSAPI_XC_ASPECT_RATIO_MSTK_PS_16_9        = 0x83,     // MSTAR TK 16:9 pan&scan
}E_MSAPI_XC_ASPECT_RATIO_TYPE;


typedef enum
{
    E_MSAPI_XC_RES_MIN = 0,
    E_MSAPI_XC_RES_720x480I_60Hz = 0,
    E_MSAPI_XC_RES_720x480P_60Hz,
    E_MSAPI_XC_RES_720x576I_50Hz,
    E_MSAPI_XC_RES_720x576P_50Hz,
    E_MSAPI_XC_RES_1280x720P_50Hz,
    E_MSAPI_XC_RES_1280x720P_60Hz,
    E_MSAPI_XC_RES_1920x1080I_50Hz,
    E_MSAPI_XC_RES_1920x1080I_60Hz,
    E_MSAPI_XC_RES_1920x1080P_24Hz,
    E_MSAPI_XC_RES_1920x1080P_25Hz,
    E_MSAPI_XC_RES_1920x1080P_30Hz,
    E_MSAPI_XC_RES_1920x1080P_50Hz,
    E_MSAPI_XC_RES_1920x1080P_60Hz,
    E_MSAPI_XC_RES_640x480P_60Hz,
    E_MSAPI_XC_RES_1920x2205P_3DFP_24Hz,
    E_MSAPI_XC_RES_1280x1470P_3DFP_50Hz,
    E_MSAPI_XC_RES_1280x1470P_3DFP_60Hz,
    E_MSAPI_XC_RES_3840x2160P_24Hz,
    E_MSAPI_XC_RES_3840x2160P_25Hz,
    E_MSAPI_XC_RES_3840x2160P_30Hz,
    E_MSAPI_XC_RES_3840x2160P_50Hz,
    E_MSAPI_XC_RES_3840x2160P_60Hz,
    E_MSAPI_XC_RES_4096x2160P_24Hz,
    E_MSAPI_XC_RES_4096x2160P_25Hz,
    E_MSAPI_XC_RES_4096x2160P_30Hz,
    E_MSAPI_XC_RES_4096x2160P_50Hz,
    E_MSAPI_XC_RES_4096x2160P_60Hz,
    E_MSAPI_XC_RES_1600X1200P_60Hz,
    E_MSAPI_XC_RES_1440X900P_60Hz,
    E_MSAPI_XC_RES_1280X1024P_60Hz,
    E_MSAPI_XC_RES_1024X768P_60Hz,
    E_MSAPI_XC_RES_1280x720P_24Hz,
    E_MSAPI_XC_RES_1280x720P_25Hz,
    E_MSAPI_XC_RES_1280x720P_30Hz,
    E_MSAPI_XC_RES_AUTO = 255,
    E_MSAPI_XC_RES_MAX = E_MSAPI_XC_RES_AUTO,
}E_MSAPI_XC_OUTPUT_TIMING_TYPE;

typedef enum
{
    E_MSAPI_XC_DBG_LEVEL_NONE = 0,
    E_MSAPI_XC_DBG_LEVEL_API,
    E_MSAPI_XC_DBG_LEVEL_DRV,
    E_MSAPI_XC_DBG_LEVEL_PQ,
    E_MSAPI_XC_DBG_LEVEL_MONITOR,
}E_MSAPI_XC_DBG_LEVEL_TYPE;

typedef enum
{
    E_MSAPI_XC_BW_TBL_MIU_128 = 0,
    E_MSAPI_XC_BW_TBL_MIU_128X128,
}E_MSAPI_XC_BW_TBL_TYPE;

typedef enum
{
    E_MSAPI_XC_MADI_MOTION_Y = 0,
    E_MSAPI_XC_MADI_MOTION_C = 1,
    E_MSAPI_XC_MADI_MOTION_YC = 2,
}E_MSAPI_XC_MADI_MOTION_TYPE;

typedef enum
{
    E_MSAPI_XC_HDMITX_OUTPUT_HDMI = 0,
    E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP,
    E_MSAPI_XC_HDMITX_OUTPUT_DVI,
    E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP,
}E_MSAPI_XC_HDMITX_OUTPUT_MODE;

typedef enum
{
    E_MSAPI_XC_HDMITX_OUTPUT_RGB444 = 0,
    E_MSAPI_XC_HDMITX_OUTPUT_YUV422,
    E_MSAPI_XC_HDMITX_OUTPUT_YUV444,
    E_MSAPI_XC_HDMITX_OUTPUT_YUV420,
    E_MSAPI_XC_HDMITX_OUTPUT_UNKOWN,
}E_MSAPI_XC_HDMITX_OUTPUT_COLOR;

typedef enum
{
    E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT = 0,
    E_MSAPI_XC_HDMITX_OUTPUT_QUANT_FULL ,
    E_MSAPI_XC_HDMITX_OUTPUT_QUANT_DEFAULT ,
}E_MSAPI_XC_HDMITX_OUTPUT_QUANT_RANGE;

typedef enum
{
    E_MSAPI_XC_HDMITX_CD_8BITS = 0,
    E_MSAPI_XC_HDMITX_CD_10BITS,
    E_MSAPI_XC_HDMITX_CD_12BITS,
    E_MSAPI_XC_HDMITX_CD_AUTO,
    E_MSAPI_XC_HDMITX_CD_16BITS,
}E_MSAPI_XC_HDMITX_CD_TYPE;

typedef enum
{
    E_MSAPI_XC_HDCP_NONE = 0,  // For backward compatible. Use new enum below
    E_MSAPI_XC_HDCP_14,        // For backward compatible. Use new enum below
    E_MSAPI_XC_HDCP_14_TEE,    // For backward compatible. Use new enum below
    E_MSAPI_XC_HDCP_22,        // For backward compatible. Use new enum below
    E_MSAPI_XC_HDCP_22_TEE,    // For backward compatible. Use new enum below
    E_MSAPI_XC_HDCP_14NONTEE        = E_MSAPI_XC_HDCP_14,     // Utopia HDCP14
    E_MSAPI_XC_HDCP_14TEE           = E_MSAPI_XC_HDCP_14_TEE, // HDCP14 with TEE
    E_MSAPI_XC_HDCP_22TEE_14NONTEE  = E_MSAPI_XC_HDCP_22_TEE, // HDCP22 with Utopia HDCP14
    E_MSAPI_XC_HDCP_22TEE_14TEE,                              // HDCP22 with HDCP14_TEE
}E_MSAPI_XC_HDCP_VERSION;

typedef enum
{
    E_MSAPI_XC_CAPTURE_WINDOW ,
    E_MSAPI_XC_CROP_WINDOW ,
    E_MSAPI_XC_DISPLAY_WINDOW ,
    E_MSAPI_XC_WINDOW_MAX
} E_MSAPI_XC_WINDOW_TYPE;

typedef enum
{
    E_MSAPI_XC_MVOP_SOURCE_TYPE_420 = 0,
    E_MSAPI_XC_MVOP_SOURCE_TYPE_422 ,
} E_MSAPI_XC_MVOP_SOURCE_TYPE;

typedef enum
{
    E_MSAPI_XC_MAIN_DECODER_PATH = 0,
    E_MSAPI_XC_SUB_DECODER_PATH,
    E_MSAPI_XC_MAX_DECODER_PATH
} E_MSAPI_XC_DECODER_PATH;

typedef enum
{
    E_MSAPI_XC_SETWINDOW_TYPE_NORMAL = 0,
    E_MSAPI_XC_SETWINDOW_TYPE_UPDATE_SHM_XC_INFO,
    E_MSAPI_XC_SETWINDOW_TYPE_UPDATE_SHM_ZOOM_INFO
} E_MSAPI_XC_SETWINDOW_TYPE;

///MSAPI_XC commands used by msAPI_XC_SetCommand() and msAPI_XC_GetCommand()
typedef enum
{
    E_MSAPI_XC_CMD_CLEAR                            = 0x000,    ///< clear all command flag

    E_MSAPI_XC_CMD_SET_TYPE                         = 0x100,
    E_MSAPI_XC_CMD_SET_DYNAMIC_SCALING              = 0x101,    ///< enable/disable dynamic scaling flow, Param: 1(Disable), 0(Enable)
    E_MSAPI_XC_CMD_SET_FORCE_H_PRESCALING_HALF      = 0x102,    ///< force doing pre-scale down horizontal to 1/2 to reduce memory bandwidth (video quality will be descresed), Param: 1(Disable), 0(Enable)
    E_MSAPI_XC_CMD_SET_FORCE_H_PRESCALING_QUARTER   = 0x103,    ///< force doing pre-scale down horizontal to 1/4 to reduce memory bandwidth (video quality will be descresed), Param: 1(Disable), 0(Enable)
    E_MSAPI_XC_CMD_SET_IGNORE_CROP_INFO             = 0x104,    ///< ignore Crop information from VDEC, Param: 1(Disable), 0(Enable)
    E_MSAPI_XC_CMD_SET_DS_SWDB                      = 0x105,    ///< enable/disable DS SWDB flow, Param: 1(Disable), 0(Enable)
    E_MSAPI_XC_CMD_SET_HDR                          = 0x106,    ///< enable/disable HDR flow, Param: 0(Disable), 1(Enable)
    E_MSAPI_XC_CMD_SET_SETWINDOW_TYPE               = 0x107,    ///< set setwindow type, Param: E_MSAPI_XC_DECODER_PATH
    E_MSAPI_XC_CMD_SET_NONCACHE_BUFFER_ID           = 0x108,    ///< set system NonCached Buffer ID

    E_MSAPI_XC_CMD_GET_TYPE                         = 0x400,
} E_MSAPI_XC_COMMAND;

typedef enum
{
    E_MSAPI_XC_YUV_FULL_TO_LIMIT,     ///< YUV full input convert to limit YUV out
    E_MSAPI_XC_YUV_LIMIT_TO_FULL,     ///< limit in convert to full out
    E_MSAPI_XC_YUV_BYPASS,            ///< Bypass. (= limit in -> limit out or full in -> full out)
}E_MSAPI_XC_Color_Convert_Type;

///MSAPI_XC CFD control used by msAPI_XC_CFD_Control()
typedef enum
{
    E_MSAPI_XC_CFD_GET_CAP,                         ///< support cfd or not, Param:(out) MS_U8, 0 mean not support CFD, 1 mean support CFD version 1
    E_MSAPI_XC_CFD_INIT,                            ///< init cfd, Param:
    E_MSAPI_XC_CFD_SET_SINK_EDID_TO_CFD,            ///< set sink edid to cfd, Param:
    E_MSAPI_XC_CFD_SET_INFOFRAME_FROM_CFD,          ///< set infoframe from cfd, Param:(in) MS_BOOL, FALSE do not send infoframe by cfd(use default), TRUE send infoframe by cfd
    E_MSAPI_XC_CFD_FIRE,                            ///< fire cfd, Param:
    E_MSAPI_XC_CFD_LOAD_HDR_METADATA_BY_HDRIP,      ///< load hdr metadata by hdrip
    E_MSAPI_XC_CFD_SEND_CFD,                        ///< send cfd info, Param
    E_MSAPI_XC_CFD_SET_HDR_ONOFF,                   ///< HDR onoff, Param: MS_U8, 0 means auto, 1 means HDR on, and 2 means HDR off
    E_MSAPI_XC_CFD_DOVI_RESET,                      ///< Dolby Vision HDR reset
} E_MSAPI_XC_CFD_CONTROL;

typedef enum
{
    E_MSAPI_PANEL_INFO_WIDTH,
    E_MSAPI_PANEL_INFO_HEIGHT,
    E_MSAPI_PANEL_INFO_H_TOTAL,
    E_MSAPI_PANEL_INFO_V_TOTAL,
    E_MSAPI_PANEL_INFO_H_START,
    E_MSAPI_PANEL_INFO_V_START,
    E_MSAPI_PANEL_INFO_DEFAULT_V_FREQ,
    E_MSAPI_PANEL_INFO_LPLL_MODE,
    E_MSAPI_PANEL_INFO_CUS_WIDTH,
    E_MSAPI_PANEL_INFO_CUS_HEIGHT,
    E_MSAPI_PANEL_INFO_CUS_H_START,
    E_MSAPI_PANEL_INFO_CUS_V_START,
    E_MSAPI_PANEL_INFO_NUM,
} E_MSAPI_PANEL_INFO_TYPE;

typedef enum
{
    E_MSAPI_HDMITX_CMD_STANDARD = 0,
    E_MSAPI_HDMITX_CMD_PREFERRED_STANDARD,
    E_MSAPI_HDMITX_CMD_FORMAT,
    E_MSAPI_HDMITX_CMD_FORMAT_SUPPORT_COUNT,
    E_MSAPI_HDMITX_CMD_SUPPORT_FORMAT,
    E_MSAPI_HDMITX_CMD_PREFERREDTIMING_FIRSTDTD,
    E_MSAPI_HDMITX_CMD_RESTIMING_SUPPORT,

    E_MSAPI_HDMITX_CMD_MAX_NUM,
}E_MSAPI_HDMITX_QUERY_CMD;


typedef enum
{
    E_MSAPI_HDMITX_FORMAT_640X480P_60_4X3       = 1,
    E_MSAPI_HDMITX_FORMAT_720X480P_60_4X3       = 2,
    E_MSAPI_HDMITX_FORMAT_720X480P_60_16X9      = 3,
    E_MSAPI_HDMITX_FORMAT_1280X720P_60_16X9     = 4,
    E_MSAPI_HDMITX_FORMAT_1920X1080I_60_16X9    = 5,
    E_MSAPI_HDMITX_FORMAT_720X480I_60_4X3       = 6,
    E_MSAPI_HDMITX_FORMAT_720X480I_60_16X9      = 7,
    E_MSAPI_HDMITX_FORMAT_1920X1080P_60_16X9    = 16,
    E_MSAPI_HDMITX_FORMAT_720X576P_50_4X3       = 17,
    E_MSAPI_HDMITX_FORMAT_720X576P_50_16X9      = 18,
    E_MSAPI_HDMITX_FORMAT_1280X720P_50_16X9     = 19,
    E_MSAPI_HDMITX_FORMAT_1920X1080I_50_16X9    = 20,
    E_MSAPI_HDMITX_FORMAT_720X576I_50_4X3       = 21,
    E_MSAPI_HDMITX_FORMAT_720X576I_50_16X9      = 22,
    E_MSAPI_HDMITX_FORMAT_1920X1080P_50_16X9    = 31,
    E_MSAPI_HDMITX_FORMAT_1920X1080P_24_16X9    = 32,
    E_MSAPI_HDMITX_FORMAT_1920X1080P_25_16X9    = 33,
    E_MSAPI_HDMITX_FORMAT_1920X1080P_30_16X9    = 34,
    E_MSAPI_HDMITX_FORMAT_3840X2160P_24_16X9    = 93,
    E_MSAPI_HDMITX_FORMAT_3840X2160P_25_16X9    = 94,
    E_MSAPI_HDMITX_FORMAT_3840X2160P_30_16X9    = 95,
    E_MSAPI_HDMITX_FORMAT_3840X2160P_50_16X9    = 96,
    E_MSAPI_HDMITX_FORMAT_3840X2160P_60_16X9    = 97,
    E_MSAPI_HDMITX_FORMAT_4096X2160P_24_256X135 = 98,
    E_MSAPI_HDMITX_FORMAT_4096X2160P_25_256X135 = 99,
    E_MSAPI_HDMITX_FORMAT_4096X2160P_30_256X135 = 100,
    E_MSAPI_HDMITX_FORMAT_4096X2160P_50_256X135 = 101,
    E_MSAPI_HDMITX_FORMAT_4096X2160P_60_256X135 = 102,
    E_MSAPI_HDMITX_FORMAT_MAX_NUM = 256,
} E_MSAPI_HDMITX_VIDO_FORMAT;

typedef enum
{
    E_MSAPI_MVOP_CLK_27MHZ = 0,
    E_MSAPI_MVOP_CLK_54MHZ,
    E_MSAPI_MVOP_CLK_123MHZ,
    E_MSAPI_MVOP_CLK_144MHZ,
    E_MSAPI_MVOP_CLK_160MHZ,
    E_MSAPI_MVOP_CLK_172MHZ,
    E_MSAPI_MVOP_CLK_192MHZ,
    E_MSAPI_MVOP_CLK_320MHZ,
    E_MSAPI_MVOP_CLK_MAX = E_MSAPI_MVOP_CLK_320MHZ,
    E_MSAPI_MVOP_CLK_NUM
} E_MSAPI_MVOP_CLK_FREQ;

typedef enum
{
    E_MSAPI_FRC_MODE_NONE = 0,
    E_MSAPI_FRC_MODE_XC,
    E_MSAPI_FRC_MODE_VDEC,
    E_MSAPI_FRC_MODE_NUM
} E_MSAPI_XC_FRC_MODE;

typedef enum
{
    E_MSAPI_XC_VIDEO_MUTE_MODE_IMMEDIATE      = 0,
    E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC      = 1,
} E_MSAPI_XC_VIDEO_MUTE_MODE;

typedef enum
{
    E_MSAPI_XC_MISC_TYPE_YPBPR      = BIT(0),
    E_MSAPI_XC_MISC_TYPE_HDMI      = BIT(1),
    E_MSAPI_XC_MISC_TYPE_NUM
} E_MSAPI_XC_MISC_TYPE;

typedef enum
{
    E_MSAPI_XC_HDMIRX_MONITOR_SC0_MAIN  = 0x00000001,
    E_MSAPI_XC_HDMIRX_MONITOR_SC0_SUB   = 0x00000002,
    E_MSAPI_XC_HDMIRX_MONITOR_SC1_MAIN  = 0x00000004,
} E_MSAPI_XC_HDMIRX_MONITOR_Event;

typedef enum
{
    E_MSAPI_XC_INITIALIZED        = 0x00000001,  ///< return true msapi xc is initialized
} E_MSAPI_XC_STATUS_TYPE;

/// Define 3D input type
typedef enum
{
    /// 3D input type is None
    E_MSAPI_XC_3D_INPUT_MODE_NONE,
    /// 3D input type is Frame Packing
    E_MSAPI_XC_3D_INPUT_FRAME_PACKING,
    /// 3D input type is Frame Alternative
    E_MSAPI_XC_3D_INPUT_FRAME_ALTERNATIVE,
    /// 3D input type is Field Alternative
    E_MSAPI_XC_3D_INPUT_FIELD_ALTERNATIVE,
    /// 3D input type is Line Alternativ
    E_MSAPI_XC_3D_INPUT_LINE_ALTERNATIVE,
    /// 3D input type is Side by Side Half
    E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_HALF,
    /// 3D input type is Side by Side Full
    E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_FULL,
    /// 3D input type is Top and Bottom
    E_MSAPI_XC_3D_INPUT_TOP_BOTTOM,
    /// 3D input type is Check board
    E_MSAPI_XC_3D_INPUT_CHECK_BORAD,
    /// 3D input type is Pixel Alternative
    E_MSAPI_XC_3D_INPUT_PIXEL_ALTERNATIVE,
} E_MSAPI_XC_3D_INPUT_MODE;

/// Define 3D output type
typedef enum
{
    /// 3D output type is None
    E_MSAPI_XC_3D_OUTPUT_MODE_NONE,
    /// 3D output type is Top and Bottom
    E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM,
    /// 3D output type is Frame Packing
    E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING,
    /// 3D output type is Line Alternative
    E_MSAPI_XC_3D_OUTPUT_LINE_ALTERNATIVE,
    /// 3D output type is Side by Side Half
    E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF,
    /// 3D output type is Side by Side Full
    E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_FULL,
    /// 3D output type is Top and Top
    E_MSAPI_XC_3D_OUTPUT_TOP_TOP,
    /// 3D output type is Bottom and Bottom
    E_MSAPI_XC_3D_OUTPUT_BOTTOM_BOTTOM,
    /// 3D output type is Left and Left
    E_MSAPI_XC_3D_OUTPUT_LEFT_LEFT,
    /// 3D output type is Right and Right
    E_MSAPI_XC_3D_OUTPUT_RIGHT_RIGHT,
} E_MSAPI_XC_3D_OUTPUT_MODE;

typedef enum
{
    E_MSAPI_XC_3D_PANEL_NONE,
    E_MSAPI_XC_3D_PANEL_SHUTTER,      //240hz panel, which can process updown, leftright,vertical or horizontal line weave
    E_MSAPI_XC_3D_PANEL_PELLICLE,     //120hz panel, which can only process horizontal line weave
    E_MSAPI_XC_3D_PANEL_MAX,
} E_MSAPI_XC_3D_PANEL_TYPE;

/// OP2 Video/GOP layer switch
typedef enum
{
    E_MSAPI_XC_VOP_LAYER_VIDEO_MUX1_MUX2 = 0,        ///<0: Video->GOP1->GOP2 (GOP2 is one pipe later than GOP1)
    E_MSAPI_XC_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2,      ///<1: FrameColor->Video->GOP1->GOP2 (GOP1/GOP2 smae pipe)
    E_MSAPI_XC_VOP_LAYER_FRAME_VIDEO_MUX2_MUX1,      ///<2: FrameColor->Video->GOP2->GOP1 (GOP1/GOP2 smae pipe)
    E_MSAPI_XC_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2,      ///<3: FrameColor->GOP1->Video->GOP2 (GOP1/GOP2 smae pipe)
    E_MSAPI_XC_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO,      ///<4: FrameColor->GOP1->GOP2->Video (GOP1/GOP2 smae pipe)
    E_MSAPI_XC_VOP_LAYER_FRAME_MUX2_VIDEO_MUX1,      ///<5: FrameColor->GOP2->Video->GOP1 (GOP1/GOP2 smae pipe)
    E_MSAPI_XC_VOP_LAYER_FRAME_MUX2_MUX1_VIDEO,      ///<6: FrameColor->GOP2->GOP1->Video (GOP1/GOP2 smae pipe)
    E_MSAPI_XC_VOP_LAYER_RESERVED,
} E_MSAPI_XC_VOP_OSD_LAYER_SEL;

typedef enum
{
    E_MSAPI_XC_HDMITX_AUTO_CONFIG_OUTPUT_MODE = 0x00000001,
    E_MSAPI_XC_HDMITX_AUTO_CONFIG_COLOR_DEPTH = 0x00000002,
    E_MSAPI_XC_HDMITX_AUTO_CONFIG_COLOR_FORMAT = 0x00000004,
    E_MSAPI_XC_HDMITX_AUTO_CONFIG_OUTPUT_TIMING = 0x00000008,
} E_MSAPI_XC_HDMITX_AUTO_CONFIG;

typedef enum
{
    E_MSAPI_XC_HDMIRX_AUTO_CONFIG_2Dto3D = 0x00000001,
} E_MSAPI_XC_HDMIRX_AUTO_CONFIG;

typedef enum
{
    E_MSAPI_XC_HDMIRX_PORT0 = 0,
    E_MSAPI_XC_HDMIRX_PORT1,
    E_MSAPI_XC_HDMIRX_PORT2,
    E_MSAPI_XC_HDMIRX_PORT3,
    E_MSAPI_XC_HDMIRX_PORT_NUM
} E_MSAPI_XC_HDMIRX_PORT;

typedef enum
{
    E_MSAPI_XC_HDMI_ADDITIONAL_NONE = 0,
    E_MSAPI_XC_HDMI_ADDITIONAL_4Kx2K_FORMAT,
    E_MSAPI_XC_HDMI_ADDITIONAL_3D_FORMAT,
    E_MSAPI_XC_HDMI_ADDITIONAL_RESERVED,
    E_MSAPI_XC_HDMI_ADDITIONAL_NA
}E_MSAPI_XC_HDMI_ADDITIONAL_VIDEO_FORMAT;

typedef enum
{
    E_MSAPI_XC_HDMI_VIC_4Kx2K_30Hz = 0x01, // 1920(x2)x1080(x2) @ 29.97/30Hz
    E_MSAPI_XC_HDMI_VIC_4Kx2K_25Hz = 0x02, // 1920(x2)x1080(x2) @ 25Hz
    E_MSAPI_XC_HDMI_VIC_4Kx2K_24Hz = 0x03, // 1920(x2)x1080(x2) @ 23.976/24Hz
    E_MSAPI_XC_HDMI_VIC_4Kx2K_24Hz_SMPTE = 0x04, // 2048(x2)x1080(x2) @ 24Hz
    E_MSAPI_XC_HDMI_VIC_RESERVED,                // 0x00, 0x05 ~ 0xFF
    E_MSAPI_XC_HDMI_VIC_NA                             // Not available
} E_MSAPI_XC_HDMI_VIC_4Kx2K_CODE;

typedef enum
{
    E_MSAPI_XC_HDMI_AUDIO_CODING_TYPE_PCM = 0x00,
    E_MSAPI_XC_HDMI_AUDIO_CODING_TYPE_NONPCM,
} E_MSAPI_XC_HDMI_AUDIO_CODING_TYPE;

typedef enum
{
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_REFER_TO_HEADER = 0x00,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_2CH,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_3CH,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_4CH,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_5CH,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_6CH,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_7CH,
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_8CH,
} E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT;

typedef enum
{
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_REFER_TO_HEADER = 0x00,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_32KHZ,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_44KHZ,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_48KHZ,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_88KHZ,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_96KHZ,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_176KHZ,
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_192KHZ,
} E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY;

typedef enum
{
    E_MSAPI_XC_HDMI_SCAN_INFO_NODATA = 0x00,
    E_MSAPI_XC_HDMI_SCAN_INFO_OVERSCAN,
    E_MSAPI_XC_HDMI_SCAN_INFO_UNDERSCAN,
    E_MSAPI_XC_HDMI_SCAN_INFO_FEATURE,
    E_MSAPI_XC_HDMI_SCAN_INFO_UNKNOW,
} E_MSAPI_XC_HDMI_SCAN_INFO;

typedef enum
{
    E_MSAPI_XC_HDMI_ASPECT_RATIO_Reserve_0  = 0x00,
    E_MSAPI_XC_HDMI_ASPECT_RATIO_Reserve_1  = 0x01,
    E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_Top   = 0x02,
    E_MSAPI_XC_HDMI_ASPECT_RATIO_14x9_Top = 0x03,
    E_MSAPI_XC_HDMI_ASPECT_RATIO_GT_16x9 = 0x04,
    E_MSAPI_XC_HDMI_ASPECT_RATIO_Reserve_5  = 0x05,
    E_MSAPI_XC_HDMI_ASPECT_RATIO_Reserve_6  = 0x06,    ///< IF0[11..8] 0110, Reserved
    E_MSAPI_XC_HDMI_ASPECT_RATIO_Reserve_7  = 0x07,    ///< IF0[11..8] 0111, Reserved
    E_MSAPI_XC_HDMI_ASPECT_RATIO_SAME   = 0x08,    ///< IF0[11..8] 1000, same as picture
    E_MSAPI_XC_HDMI_ASPECT_RATIO_4x3_C  = 0x09,    ///< IF0[11..8] 1001, 4:3 Center
    E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_C = 0x0A,    ///< IF0[11..8] 1010, 16:9 Center
    E_MSAPI_XC_HDMI_ASPECT_RATIO_14x9_C = 0x0B,    ///< IF0[11..8] 1011, 14:9 Center
    E_MSAPI_XC_HDMI_ASPECT_RATIO_Reserve_12   = 0x0C,    ///< IF0[11..8] 1100, Reserved.
    E_MSAPI_XC_HDMI_ASPECT_RATIO_4x3_with_14x9_C  = 0x0D,    ///< IF0[11..8] 1101, 4:3 with shoot and protect 14:9 centre.
    E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_with_14x9_C = 0x0E,    ///< IF0[11..8] 1110, 16:9 with shoot and protect 14:9 centre.
    E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_with_4x3_C = 0x0F,    ///< IF0[11..8] 1111, 16:9 with shoot and protect 4:3 centre.

    // Picture Aspect Ratio - PAR
    E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_NODATA = 0x00,     ///< IF0[13..12] 00
    E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_4x3    = 0x10,     ///< IF0[13..12] 01, 4:3
    E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_16x9   = 0x20,     ///< IF0[13..12] 10, 16:9
    E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_RSV    = 0x30,     ///< IF0[13..12] 11, reserved
} E_MSAPI_XC_HDMI_ASPECT_RATIO;

typedef enum
{
    E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC601 = 0x00,    ///< xvycc 601
    E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC709,    ///< xvycc 709
    E_MSAPI_XC_HDMI_EXT_COLOR_SYCC601,    ///< sYCC 601
    E_MSAPI_XC_HDMI_EXT_COLOR_ADOBEYCC601,    ///< Adobe YCC 601
    E_MSAPI_XC_HDMI_EXT_COLOR_ADOBERGB,    ///< Adobe RGB
    E_MSAPI_XC_HDMI_EXT_COLOR_BT2020CYCC,    ///< Adobe BT2020 CYCC
    E_MSAPI_XC_HDMI_EXT_COLOR_BT2020YCC,      ///< Adobe BT2020  YCC
    E_MSAPI_XC_HDMI_EXT_COLOR_BT2020RGB,      ///< Adobe BT2020 RGB
    E_MSAPI_XC_HDMI_EXT_COLOR_UNKNOWN,    ///< Unknow
} E_MSAPI_XC_HDMI_EXT_COLORMETRY;

typedef enum
{
    E_MSAPI_XC_HDMI_COLOR_NODATA = 0x00,    ///< NODATA
    E_MSAPI_XC_HDMI_COLOR_ITU601,    ///< ITU601
    E_MSAPI_XC_HDMI_COLOR_ITU709,    ///< ITU709
    E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID,    ///EXTEND_VALID
    E_MSAPI_XC_HDMI_COLOR_UNKNOWN,    ///< Unknow
} E_MSAPI_XC_HDMI_COLORMETRY;

typedef enum
{
    E_MSAPI_XC_HDMIRX_AVI = 0,
    E_MSAPI_XC_HDMIRX_VS,
    E_MSAPI_XC_HDMIRX_GC,
    E_MSAPI_XC_HDMIRX_AUDIO,
} E_MSAPI_XC_HDMIRX_PACKET_TYPE;



//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    HDMITX_ANALOG_TUNING stSDInfo;      //clock < 75MHz
    HDMITX_ANALOG_TUNING stHDInfo;      //clock < 165MHz
    HDMITX_ANALOG_TUNING stDeepHDInfo;  //clock > 165MHz
} MSAPI_XC_HDMITX_BOARD_INFO;


/// Define HDMI RX EDID Type
typedef enum
{
    E_MSAPI_XC_HDMIRX_EDID_EEPROM = 0,
    E_MSAPI_XC_HDMIRX_EDID_INTERNAL,
    E_MSAPI_XC_HDMIRX_EDID_CUSTOMER,
} E_MSAPI_XC_HDMIRX_EDID_TYPE;

typedef enum
{
    /// Video codec type is unknown.
    E_MSAPI_XC_VIDEO_CODEC_UNKNOWN = -1,
    /// Video codec type is MPEG 4.
    E_MSAPI_XC_VIDEO_CODEC_MPEG4,
    /// Video codec type is motion JPG.
    E_MSAPI_XC_VIDEO_CODEC_MJPEG,
    /// Video codec type is H264.
    E_MSAPI_XC_VIDEO_CODEC_H264,
    /// Video codec type is RealVideo.
    E_MSAPI_XC_VIDEO_CODEC_RM,
    /// Video codec type is TS File.
    E_MSAPI_XC_VIDEO_CODEC_TS,
    /// Video codec type is MPEG 1/2.
    E_MSAPI_XC_VIDEO_CODEC_MPEG,
    /// Video codec type is VC1.
    E_MSAPI_XC_VIDEO_CODEC_VC1,
    /// Video codec type is Audio Video Standard.
    E_MSAPI_XC_VIDEO_CODEC_AVS,
    /// Video codec type is FLV.
    E_MSAPI_XC_VIDEO_CODEC_FLV,
    /// Video codec type is MVC.
    E_MSAPI_XC_VIDEO_CODEC_MVC,
    /// Video codec type is VP6.
    E_MSAPI_XC_VIDEO_CODEC_VP6,
    /// Video codec type is VP8.
    E_MSAPI_XC_VIDEO_CODEC_VP8,
    /// Video codec type is HEVC.
    E_MSAPI_XC_VIDEO_CODEC_HEVC,
    /// Video codec type is VP9.
    E_MSAPI_XC_VIDEO_CODEC_VP9,
    /// Video codec type is HEVC DV
    E_MSAPI_XC_VIDEO_CODEC_HEVC_DV,
    E_MSAPI_XC_VIDEO_CODEC_TYPE_RV8,
    E_MSAPI_XC_VIDEO_CODEC_TYPE_RV9
}E_MSAPI_XC_VIDEO_CODEC;

typedef enum
{
    E_MSAPI_XC_VIDEO_SCAN_TYPE_PROGRESSIVE = 0 ,
    E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FRAME,
    E_MSAPI_XC_VIDEO_SCAN_TYPE_INTERLACE_FIELD,
    E_MSAPI_XC_VIDEO_SCAN_TYPE_MAX,
}E_MSAPI_XC_VIDEO_SCAN_TYPE;

typedef enum
{
    // bottom first
    E_MSAPI_XC_VIDEO_FIELD_ORDER_TYPE_BOTTOM = 0,
    // top first
    E_MSAPI_XC_VIDEO_FIELD_ORDER_TYPE_TOP,
    E_MSAPI_XC_VIDEO_FIELD_ORDER_TYPE_MAX,
}E_MSAPI_XC_VIDEO_FIELD_ORDER_TYPE;

typedef enum
{
    E_MSAPI_XC_VIDEO_FIELD_TYPE_NONE = 0,
    E_MSAPI_XC_VIDEO_FIELD_TYPE_TOP,
    E_MSAPI_XC_VIDEO_FIELD_TYPE_BOTTOM,
    E_MSAPI_XC_VIDEO_FIELD_TYPE_BOTH,
    E_MSAPI_XC_VIDEO_FIELD_TYPE_MAX,
}E_MSAPI_XC_VIDEO_FIELD_TYPE;

typedef enum
{
    E_MSAPI_XC_VIDEO_TILE_MODE_NONE = 0,
    E_MSAPI_XC_VIDEO_TILE_MODE_16x16,
    E_MSAPI_XC_VIDEO_TILE_MODE_16x32,
    E_MSAPI_XC_VIDEO_TILE_MODE_32x16,
    E_MSAPI_XC_VIDEO_TILE_MODE_32x32,
    E_MSAPI_XC_VIDEO_TILE_MODE_MAX,
}E_MSAPI_XC_VIDEO_TILE_MODE;

/// DIP source data format
typedef enum
{
    /// YCrYCb.
    E_MSAPI_XC_VIDEO_DATA_FMT_YUV422 = 0,
    /// RGB domain
    E_MSAPI_XC_VIDEO_DATA_FMT_RGB565,
    /// RGB domain
    E_MSAPI_XC_VIDEO_DATA_FMT_ARGB8888,
    /// YUV420 HVD tile
    E_MSAPI_XC_VIDEO_DATA_FMT_YUV420,
    /// YC separate 422
    E_MSAPI_XC_VIDEO_DATA_FMT_YC422,
    /// YUV420 H265 tile
    E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_H265,
    /// YUV420 H265_10bits tile
    E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_H265_10BITS,
    /// YUV420 planer
    E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_PLANER,
    /// YUV420 semi planer
    E_MSAPI_XC_VIDEO_DATA_FMT_YUV420_SEMI_PLANER,
    E_MSAPI_XC_VIDEO_DATA_FMT_MAX
} E_MSAPI_XC_VIDEO_DATA_FMT;

typedef struct
{
    MS_U32 u32version;
    MS_U32 u32size;
} MSAPI_XC_VIDEO_MFDEC_VER_CRL;

typedef enum
{
    E_MSAPI_XC_VIDEO_H26X_MODE =0x00,
    E_MSAPI_XC_VIDEO_VP9_MODE  =0x01,
}E_MSAPI_XC_VIDEO_MFDEC_VP9_MODE;

typedef struct
{
    MSAPI_XC_VIDEO_MFDEC_VER_CRL stMFDec_HTLB_VerCtl;
    MS_PHY u32HTLBEntriesAddr;
    MS_U8  u8HTLBEntriesSize;
    MS_U8  u8HTLBTableId;
    void* pHTLBInfo;
} MSAPI_XC_VIDEO_MFDEC_HTLB_INFO;

typedef struct
{
    MS_BOOL bMFDec_Enable;
    MS_U8 u8MFDec_Select;
    MS_BOOL bHMirror; // no use
    MS_BOOL bVMirror; // no use
    MS_BOOL bUncompress_mode;
    MS_BOOL bBypass_codec_mode;
    E_MSAPI_XC_VIDEO_MFDEC_VP9_MODE en_MFDecVP9_mode;
    MS_U16 u16StartX;
    MS_U16 u16StartY;
    MS_PHY phyBitlen_Base;
    MS_U16 u16Bitlen_Pitch;
    MSAPI_XC_VIDEO_MFDEC_HTLB_INFO stMFDec_HTLB_Info; //reserve
    void* pMFDecInfo; //reserve
    MS_U8 u8MIU_Select;
}MSAPI_XC_VIDEO_MFDEC_INFO;

typedef struct
{
    /// Provide the VDEC stream ID to XC to access MApi_VDEC_EX_DisplayFrame() and MApi_VDEC_EX_ReleaseFrame()
    MS_U32 u32VDECStreamID;
    /// Provide the VDEC stream ID Version to XC to access MApi_VDEC_EX_DisplayFrame() and MApi_VDEC_EX_ReleaseFrame()
    MS_U32 u32VDECStreamVersion;
}MSAPI_XC_VDECSTREAM_ID_VERSION;

typedef struct
{
    MS_U32 u32Version;                         /// MSAPI_XC_COLORHWFORMAT version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_COLORHWFORMAT)
    /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_PHY u32SrcLumaAddr;
    /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_PHY u32SrcChromaAddr;
    /// physical address of Luma LSB 2bit buffer (Main10 profile)
    MS_PHY u32SrcLumaAddr_2bit;
    /// physical address of Chroma LSB 2bit buffer (Main10 profile)
    MS_PHY u32SrcChromaAddr_2bit;
    /// frame buffer 10bit Pitch
    MS_U16 u16Src10bitPitch;
    /// frame buffer pitch
    MS_U16 u16SrcPitch;
    /// frame buffer 10bit data
    /// u16Pitch_subsample > 0, then u32LumaAddr_subsample & u32ChromaAddr_subsample can be used.
    /// physical luma address of subsample frame buffer
    MS_PHY u32LumaAddr_subsample;
    /// physical chroma address of subsample frame buffer
    MS_PHY u32ChromaAddr_subsample;
    /// pitch of subsample frame buffer
    MS_U16 u16Pitch_subsample;
    /// tile mode of subsample frame buffer
    MS_U8  u8TileMode_subsample;
    MSAPI_XC_VIDEO_MFDEC_INFO stMFdecInfo;
    MS_U8 u8LumaBitdepth;
    MS_U8 u8ChromaBitdepth;
    MS_PHY u32HTLBTableAddr;
    MS_U8  u8HTLBPageSizes;
    MS_U8  u8HTLBChromaEntriesSize;
    MS_PHY u32HTLBChromaEntriesAddr;
    MS_U16 u16MaxContentLightLevel;
    MS_U16 u16MaxPicAverageLightLevel;

    MS_U8  u8V7DataValid;
    MS_U16 u16Width_subsample;
    MS_U16 u16Height_subsample;
    MS_U64 u64NumUnitsInTick;
    MS_U64 u64TimeScale;
    /// SWDR histogram data
    MS_U16 u16Histogram[MSAPI_SWDRHISTOGRAM_INDEX];
} MSAPI_XC_COLORHWFORMAT;

typedef struct
{
    MS_U32 u32Version;                         /// MSAPI_XC_FRAME_FORMAT version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_FRAME_FORMAT)
    E_MSAPI_XC_VIDEO_FIELD_TYPE eFieldType;
    /// frame buffer Width
    MS_U16 u16SrcWidth;
    /// frame buffer Height
    MS_U16 u16SrcHeight;
    ///right cropping
    MS_U16 u16CropRight;
    ///left cropping
    MS_U16 u16CropLeft;
    ///bottom cropping
    MS_U16 u16CropBottom;
    ///top cropping
    MS_U16 u16CropTop;
    MSAPI_XC_COLORHWFORMAT stHWFormat;
    /// Frame index
    MS_U32 u32FrameIndex;
    /// Frame reference count
    MS_U32 u32PriData;
    MS_BOOL b10bitData;
} MSAPI_XC_FRAME_FORMAT;

////Frame Info set
typedef struct
{
    MS_U32 u32Version;                         /// MSAPI_XC_COLORDESCRIPTION version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_COLORDESCRIPTION)
    //color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    MS_U8 u8ColorPrimaries;
    MS_U8 u8TransferCharacteristics;
    // matrix coefficients in deriving YUV signal from RGB
    MS_U8 u8MatrixCoefficients;
} MSAPI_XC_COLORDESCRIPTION;

typedef struct
{
    MS_U32 u32Version;                         /// MSAPI_XC_MASTERCOLORDISPLAY version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_MASTERCOLORDISPLAY)
    //mastering color display: color volumne of a display
    MS_U32 u32MaxLuminance;
    MS_U32 u32MinLuminance;
    MS_U16 u16DisplayPrimaries[3][2];
    MS_U16 u16WhitePoint[2];
} MSAPI_XC_MASTERCOLORDISPLAY;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// MSAPI_XC_DOLBYHDRINFO version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_DOLBYHDRINFO)
    // bit[0:1] 0: Disable 1:Single layer 2: Dual layer, bit[2] 0:Base Layer 1:Enhance Layer
    MS_U8  u8DVMode;
    MS_PHY phyHDRMetadataAddr;
    MS_U32 u32HDRMetadataSize;
    MS_PHY phyHDRRegAddr;
    MS_U32 u32HDRRegSize;
    MS_PHY phyHDRLutAddr;
    MS_U32 u32HDRLutSize;
    MS_U8  u8DMEnable;
    MS_U8  u8CompEnable;
    MS_U8  u8CurrentIndex;
} MSAPI_XC_DOLBYHDRINFO;

// MSAPI HDR frame info
typedef struct
{
    MS_U32 u32Version;                         /// MSAPI_XC_HDR_FRAME_INFO version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_HDR_FRAME_INFO)

    // bit[0]: MS_ColorDescription present or valid, bit[1]: MS_MasterColorDisplay present or valid
    MS_U32 u32FrmInfoExtAvail;
    // //color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    MSAPI_XC_COLORDESCRIPTION   stColorDescription;
    // mastering color display: color volumne of a display
    MSAPI_XC_MASTERCOLORDISPLAY stMasterColorDisplay;
    //Dolby Info
    MSAPI_XC_DOLBYHDRINFO       stDolbyHDRInfo;
} MSAPI_XC_HDR_FRAME_INFO;

typedef struct
{
    MS_U32 u32Version;                         /// MSAPI_XC_VDECFRAME_INFO version
    MS_U32 u32Length;                         /// sizeof(MSAPI_XC_VDECFRAME_INFO)
    /// frame buffer Valid
    MS_BOOL bValid;
    E_MSAPI_XC_VIDEO_SCAN_TYPE eScanType;
    E_MSAPI_XC_VIDEO_FIELD_ORDER_TYPE eFieldOrderType;
    MSAPI_XC_FRAME_FORMAT stFrames[2];
    MS_U32 u32FrameRate ;
    E_MSAPI_XC_VIDEO_DATA_FMT eFmt;
    E_MSAPI_XC_WINDOW eWindow;
    E_MSAPI_XC_VIDEO_CODEC eCODEC;
    E_MSAPI_XC_VIDEO_TILE_MODE eTileMode;
    MSAPI_XC_VDECSTREAM_ID_VERSION stVDECStream_Id_Version;
    MS_U64 u64Pts;

    MSAPI_XC_HDR_FRAME_INFO stHDRInfo;
    MS_U8 u8FieldCtrl;          // control one field mode, always top or bot when FF or FR
    MS_U8 u8AspectRate;
    MS_U8 u8Interlace;
    MS_U8 u8FrcMode;
    MS_U8 u83DMode;
    MS_U8 u8FreezeThisFrame;
    MS_U8 u8ToggleTime;
    MS_U8 u83DLayout;
    MS_U8 u8ColorInXVYCC;
    MS_U8 u8LowLatencyMode;
    MS_U8 u8VdecComplexity;
    MS_U8 u8AFD;               //active frame code
    MS_U8 u8HTLBTableId;
    MS_U8 u8HTLBEntriesSize;
    MS_PHY phyHTLBEntriesAddr;
} MSAPI_XC_VDECFRAME_INFO;

typedef enum
{
    //The higher the number, mean z-order higher priority
    E_MSAPI_XC_LAYER_FrameColor = 0, // Bottom
    E_MSAPI_XC_LAYER_1,
    E_MSAPI_XC_LAYER_2,
    E_MSAPI_XC_LAYER_3,
    E_MSAPI_XC_LAYER_4, // top
    E_MSAPI_XC_LAYER_MAX,
} E_MSAPI_XC_ZORDER;

typedef enum
{
    // 0x00, first 4 bits are unused
    E_MSAPI_XC_INT_DIPW = 1,    //DIPW write one frame done interrupt
    E_MSAPI_XC_INT_MEMSYNC_MAIN = 3,
    E_MSAPI_XC_INT_START = 4,
    E_MSAPI_XC_INT_RESERVED1 = E_MSAPI_XC_INT_START,  // before is SC_INT_TUNE_FAIL_P, FBL line buffer overrun/underrun
                                                // scaler dosen't have this interrupt now,

    E_MSAPI_XC_INT_VSINT,                          // output Vsync interrupt, can select polarity with BK0_04[1]
    E_MSAPI_XC_INT_F2_VTT_CHG,                     // main window, HDMI mute or Vsync polarity changed, Vtt change exceed BK1_1D[11:8]
    E_MSAPI_XC_INT_F1_VTT_CHG,
    E_MSAPI_XC_INT_F2_VS_LOSE,                     // didn't received Vsync for a while or Vtt count BK1_1F[10:0] exceed max value
    E_MSAPI_XC_INT_F1_VS_LOSE,
    E_MSAPI_XC_INT_F2_JITTER,                      // H/V start/end didn't be the same with privous value, usually used in HDMI/DVI input
    E_MSAPI_XC_INT_F1_JITTER,
    E_MSAPI_XC_INT_F2_IPVS_SB,                     // input V sync interrupt, can select which edge to trigger this interrupt with BK0_04[0]
    E_MSAPI_XC_INT_F1_IPVS_SB,
    E_MSAPI_XC_INT_F2_IPHCS_DET,                   // input H sync interrupt
    E_MSAPI_XC_INT_F1_IPHCS_DET,

    // 0x10
    E_MSAPI_XC_INT_PWM_RP_L_INT,                   // pwm rising edge of left frame
    E_MSAPI_XC_INT_PWM_FP_L_INT,                   // pwm falling edge of left frame
    E_MSAPI_XC_INT_F2_HTT_CHG,                     // Hsync polarity changed or Hperiod change exceed BK1_1D[5:0]
    E_MSAPI_XC_INT_F1_HTT_CHG,
    E_MSAPI_XC_INT_F2_HS_LOSE,                     // didn't received H sync for a while or Hperiod count BK1_20[13:0] exceed max value
    E_MSAPI_XC_INT_F1_HS_LOSE,
    E_MSAPI_XC_INT_PWM_RP_R_INT,                   // pwm rising edge of right frame
    E_MSAPI_XC_INT_PWM_FP_R_INT,                   // pwm falling edge of right frame
    E_MSAPI_XC_INT_F2_CSOG,                        // composite sync or SoG input signal type changed (for example, SoG signal from none -> valid, valid -> none)
    E_MSAPI_XC_INT_F1_CSOG,
    E_MSAPI_XC_INT_F2_RESERVED2,                   // scaler dosen't have this interrupt now, before is SC_INT_F2_ATS_READY
    E_MSAPI_XC_INT_F1_RESERVED2,                   // scaler dosen't have this interrupt now, before is SC_INT_F1_ATS_READY
    E_MSAPI_XC_INT_F2_ATP_READY,                   // auto phase ready interrupt
    E_MSAPI_XC_INT_F1_ATP_READY,
    E_MSAPI_XC_INT_F2_RESERVED3,                   // scaler dosen't have this interrupt now, before is SC_INT_F2_ATG_READY
    E_MSAPI_XC_INT_F1_RESERVED3,                   // scaler dosen't have this interrupt now, before is SC_INT_F1_ATG_READY

    E_MSAPI_XC_MAX_SC_INT,
}E_MSAPI_XC_INT_SRC;

typedef enum
{
    E_MSAPI_XC_HDMIRX_HPD_CTRL_DISABLE = 0,
    E_MSAPI_XC_HDMIRX_HPD_CTRL_ENABLE,
} E_MSAPI_XC_HDMIRX_HPD_CTRL_TYPE;

typedef enum
{
    E_MSAPI_XC_HDMITX_HDCP_GET_VERSION = 0,
    E_MSAPI_XC_HDMITX_HDCP_GET_STATUS,
    E_MSAPI_XC_HDMITX_HDCP_GET_TEESUPPORT,
} E_MSAPI_XC_HDMITX_HDCP_INFO;

typedef enum
{
    E_MSAPI_XC_HDMITX_HDCP_VERSION_UNKNOWN = -1,
    E_MSAPI_XC_HDMITX_HDCP_VERSION_14,
    E_MSAPI_XC_HDMITX_HDCP_VERSION_22,
} E_MSAPI_XC_HDMITX_HDCP_VERSION_INFO;

typedef enum
{
    E_MSAPI_XC_HDMITX_HDCP_STATUS_DISABLE = -1,
    E_MSAPI_XC_HDMITX_HDCP_STATUS_FAIL,
    E_MSAPI_XC_HDMITX_HDCP_STATUS_SUCCESS,
    E_MSAPI_XC_HDMITX_HDCP_STATUS_PROCESS,
} E_MSAPI_XC_HDMITX_HDCP_STATUS_INFO;

typedef enum
{
    E_MSAPI_XC_HDMITX_HDCP_TEESUPPORT_UNKNOWN = -1,
    E_MSAPI_XC_HDMITX_HDCP_TEESUPPORT_NONTEE,
    E_MSAPI_XC_HDMITX_HDCP_TEESUPPORT_TEE,
} E_MSAPI_XC_HDMITX_HDCP_TEESUPPORT_INFO;

typedef enum
{
    /// Initialize
    E_MSAPI_XC_CFD_CTRL_TYPE_INIT = 0,
    /// VGA
    E_MSAPI_XC_CFD_CTRL_TYPE_VGA = 1,
    /// TV (ATV)
    E_MSAPI_XC_CFD_CTRL_TYPE_TV = 2,
    /// CVBS (AV)
    E_MSAPI_XC_CFD_CTRL_TYPE_CVBS = 3,
    /// S-Video
    E_MSAPI_XC_CFD_CTRL_TYPE_SVIDEO = 4,
    /// YPbPr
    E_MSAPI_XC_CFD_CTRL_TYPE_YPBPR = 5,
    /// Scart
    E_MSAPI_XC_CFD_CTRL_TYPE_SCART = 6,
    /// HDMI
    E_MSAPI_XC_CFD_CTRL_TYPE_HDMI = 7,
    /// DTV
    E_MSAPI_XC_CFD_CTRL_TYPE_DTV = 8,
    /// DVI
    E_MSAPI_XC_CFD_CTRL_TYPE_DVI = 9,
    /// MM
    E_MSAPI_XC_CFD_CTRL_TYPE_MM = 10,
    /// Panel
    E_MSAPI_XC_CFD_CTRL_TYPE_PANEL = 11,
    /// HDR
    E_MSAPI_XC_CFD_CTRL_TYPE_HDR = 12,
    /// EDID
    E_MSAPI_XC_CFD_CTRL_TYPE_EDID = 13,
    /// OSD
    E_MSAPI_XC_CFD_CTRL_TYPE_OSD = 14,
    /// Fire
    E_MSAPI_XC_CFD_CTRL_TYPE_FIRE = 15,
    /// DLC
    E_MSAPI_XC_CFD_CTRL_TYPE_DLC = 16,
    /// Linear RGB
    E_MSAPI_XC_CFD_CTRL_TYPE_LINEAR_RGB = 17,
    /// Get CFD Output to HDMI Status and dicison.
    E_MSAPI_XC_CFD_CTRL_GET_HDMI_STATUS = 18,
    /// Set force HDR OnOff or auto select
    E_MSAPI_XC_CFD_CTRL_SET_HDR_ONOFF_SETTING = 19,
    /// Status
    E_MSAPI_XC_CFD_CTRL_TYPE_STATUS = 20,
    /// TMO
    E_MSAPI_KDRV_XC_CFD_CTRL_SET_TMO = 21,

    /// (Set this base is 0x40000000 for int type)
    E_MSAPI_XC_CFD_CTRL_TYPE_EX_BASE = 0x40000000,

    /// OSD_STATUS
    E_MSAPI_XC_CFD_CTRL_GET_OSD_PROCESS_CONFIGS= 0x40000001,
    /// Max
    E_MSAPI_XC_CFD_CTRL_TYPE_MAX,
} E_MSAPI_XC_CFD_CONTROL_TYPE;

typedef void (*pfnMSAPI_XC_InterruptCb) (E_MSAPI_XC_INT_SRC enIntNum, void *pParam);

typedef struct
{
    MS_U8 *UserDefEDID;                            ///< customer EDID
    MS_U16 u16UserDefEDIDSize;                    ///< 128 for VGA/DVI , 256 for HDMI
    E_MSAPI_XC_HDMIRX_EDID_TYPE  eHdmirxEDIDType;
} MSAPI_XC_HDMIRX_EDID_INFO;

typedef struct
{
    MS_U8* pu8HdmitxHdcpKey;                                ///<HDMI Tx HDCP Key
    MS_U16 u16HdmitxHdcpKeySize;                            ///<Size of HDMI Tx HDCP key
    MS_U8* pu8HdmitxHdcp2Key;                               ///<HDMI Tx HDCP2.2 Key
    MS_U16 u16HdmitxHdcp2KeySize;                           ///<Size of HDMI Tx HDCP2.2 key
} MSAPI_XC_HDMITX_HDCPKEY_INFO;

typedef struct
{
    MS_U8* pu8HdmirxHdcpKey;                                ///<HDMI Rx HDCP key
    MS_U16 u16HdmirxHdcpKeySize;                            ///<Size of HDMI Rx HDCP key
    MS_U8* pu8HdmirxHdcp2Key;                               ///<HDMI Rx HDCP2.2 key
    MS_U16 u16HdmirxHdcp2KeySize;                           ///<Size of HDMI Rx HDCP2.2 key
} MSAPI_XC_HDMIRX_HDCPKEY_INFO;


// Define XC device ID
typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Id;
}MSAPI_XC_DEVICE_ID;

/// information for display setting
typedef struct
{
    ///bitstream horizontal size
    MS_U16 u16HorSize;
    ///bitstream vertical size
    MS_U16 u16VerSize;
    ///frame rate
    MS_U32 u32FrameRate;
    ///interlace flag
    MS_U8 u8Interlace;
    ///active frame code
    MS_U8 u8AFD;
    ///Sample aspect rate width
    MS_U16 u16SarWidth;
    ///Sample aspect rate height
    MS_U16 u16SarHeight;
    ///right cropping
    MS_U16 u16CropRight;
    ///left cropping
    MS_U16 u16CropLeft;
    ///bottom cropping
    MS_U16 u16CropBottom;
    ///top cropping
    MS_U16 u16CropTop;
    ///pitch
    MS_U16 u16Pitch;
    ///interval of PTS
    MS_U16 u16PTSInterval;
    ///MPEG1 flag
    MS_U8 u8MPEG1;
    ///play mode (fixme)
    MS_U8 u8PlayMode;
    ///FRC mode
    MS_U8 u8FrcMode;
    ///aspect ratio code
    MS_U8 u8AspectRate;
    ///if FALSE, set VOP as mono mode (only for H264)
    MS_BOOL bWithChroma;
    /// if true, color space is xvYCC (Y from 16 to 235 and Cb , Cr from 16 to 240).
    /// if false, color space is BT.601/709 (Y from  0 to 255 and Cb , Cr from  0 to 255).
    /// only MPEG might be with BT.601/709
    MS_BOOL bColorInXVYCC;
    ///Dynamic scaling buffer address
    MS_U32 u32DynScalingAddr;
    ///Dynamic scaling buffer size
    MS_U32 u32DynScalingSize;
    ///Dynamic scaling depth
    MS_U8 u8DynScalingDepth;
    ///Dynamic scaling DS buffer on miu1 or miu0
    MS_BOOL bEnableMIUSel;
    ///Display width
    MS_U32 u32AspectWidth;
    ///Display height
    MS_U32 u32AspectHeight;
} MSAPI_XC_VDEC_DispInfo;

/// Define the initial data for XC_Sys init
typedef struct
{
    MS_U32 u32XCMemAddress;                                 ///<msAPI XC Memory Address
    MS_U32 u32XCMemSize;                                    ///<msAPI XC Memory Size
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eTimingType;              ///<System Panel Type
    MS_U16 u16HdmitxHpdPin;                                 ///<HDMITX_HPD_PM_GPIO Pin Number
    MS_BOOL bBootLogoEnable;                                ///<Flag to control if Boot Logo is Enable and to Bypass some init flow
    MS_BOOL bMenuLoadEnable;                                ///<Flag to control if Menu Load is Enable and to set XC register in V-sync and Blanking region
    MS_U32 u32XCMenuLoadAddress;                            ///<msAPI XC Menu Load Address
    MS_U32 u32XCMenuLoadSize;                               ///<msAPI XC Menu Load Size
    MS_S32 s32CachedPoolID;                                 ///<Cached Pool ID for stack of functional task
    E_MSAPI_XC_BW_TBL_TYPE eBWTBLType;                      ///<fill the table type to load Bandwidth Table, and to configure the MIU usage in msAPI_XC
    E_MSAPI_XC_HDMITX_OUTPUT_MODE eHdmitxOutputMode;        ///<HDMITx Output Mode
    E_MSAPI_XC_HDMITX_CD_TYPE eHdmitxCDType;                ///<HDMITx Color Depth Value
    MSAPI_XC_HDMITX_BOARD_INFO stHdmitxBoardInfo;           ///<HDMITx Analog Configuration of Board
    MS_BOOL bDisableHDMI;                                   ///<Flag to turn off HDMITx after XC initialization
    MS_BOOL bDisableDacHD;                                  ///<Flag to turn off HD DAC after XC initialization
    MS_BOOL bDisableDacSD;                                  ///<Flag to turn off SD DAC after XC initialization
    MS_U32 u32XCSubMemAddress;                              ///<msAPI XC Memory Address
    MS_U32 u32XCSubMemSize;                                 ///<msAPI XC Memory Size
    MSAPI_XC_HDMIRX_EDID_INFO  eHdmirxEDIDInfo;             ///<HDMIRx EDID Info
    MS_BOOL bBootVideoEnable;                               ///<Flag to control if Boot Video is Enable and to Bypass some init flow
    MSAPI_XC_HDMITX_HDCPKEY_INFO stHdmitxHdcpKeyInfo;       ///<HDMITx HDCP Key Info
    MSAPI_XC_HDMIRX_HDCPKEY_INFO stHdmirxHdcpKeyInfo;       ///<HDMIRx HDCP Key Info
    MS_BOOL bIgnoreHdmirxConfig;                            ///Ignore HDMIRx Setting
    MS_BOOL bForceHdmiOutputColorEnable;                    ///Flag to control HdmiOutputColor type
    E_MSAPI_XC_HDMITX_OUTPUT_COLOR eHDMIOutputColor;        ///HDMITx Output Color Value
    MS_BOOL bDTDTimingSet;                                  ///Use DTD to set output timing once AP didn't indicate
    E_MSAPI_XC_HDCP_VERSION eHDCPVersion;                   ///HDCP Version

} MSAPI_XC_INITDATA;

// information to JPEG
typedef struct
{
    MS_U8   u8MiuSel;       // JPEG buffer belongs to MIU0 or MIU1?
    MS_U32  u32BufAddr;     // JPEG buffer address
} MSAPI_XC_JPEG_INFO;

///MSAPI_XC handle to carry out info.
typedef struct
{
    E_MSAPI_XC_WINDOW eWin;
} MSAPI_XC_Handle;

typedef struct
{
    MS_BOOL  bHorizontal;
    MS_BOOL  bVertical;
    MS_U16   u16HorizontalRange;
    MS_U16   u16HorizontalValue;
    MS_U16   u16VerticalRange;
    MS_U16   u16VerticalValue;
} MSAPI_XC_MANUAL_PAN_INFO;


/// Define Window position and size attribute
typedef struct
{
    MS_U16 x;           ///<start x of the window
    MS_U16 y;           ///<start y of the window
    MS_U16 width;       ///<width of the window
    MS_U16 height;      ///<height of the window
} MSAPI_XC_WINDOW_TYPE;

//MSAPI XC STATUS
typedef struct
{
    MSAPI_XC_DEVICE_ID API_XC_DeviceId;  ///< Input: device id
    MS_BOOL            bInitialized;     ///< Output: Is msapi xc initialized?
}MSAPI_XC_STATUS_INITIALIZE;

typedef struct
{
    MS_BOOL b3DEnable;
    E_MSAPI_XC_3D_INPUT_MODE eXC3DInputMode;
    E_MSAPI_XC_3D_OUTPUT_MODE eXC3DOutputMode;
    E_MSAPI_XC_3D_PANEL_TYPE enXC3DPanelType;
}MSAPI_XC_3D_INFO;

typedef void (*pfnMSAPI_XC_CustomAspectRatioCallback)(MSAPI_XC_WINDOW_TYPE *pstCropWin, MSAPI_XC_WINDOW_TYPE *pstDstWin);
typedef void (*pfnMSAPI_XC_IsRxStableCallback)(MS_BOOL);

typedef struct
{
    pfnMSAPI_XC_CustomAspectRatioCallback pfnCusAspectRatioNotify;
    pfnMSAPI_XC_IsRxStableCallback pfnIsRXStable;
} MSAPI_XC_CALLBACK_INFO;

typedef struct
{
    E_MSAPI_XC_HDMI_SCAN_INFO eScanInfo;
    E_MSAPI_XC_HDMITX_OUTPUT_COLOR eColorFormat;
    E_MSAPI_XC_HDMI_ASPECT_RATIO eAspectRatio;
    E_MSAPI_XC_HDMI_COLORMETRY eColorMetry;
    E_MSAPI_XC_HDMI_EXT_COLORMETRY eExtColorMetry;
    E_MSAPI_XC_HDMITX_OUTPUT_QUANT_RANGE eColorRange;
    MS_U8 u8VICCode;
} MSAPI_XC_HDMIRX_AVI_PACKET;

typedef struct
{
    E_MSAPI_XC_HDMI_ADDITIONAL_VIDEO_FORMAT eVideoFormatPresent;
    E_MSAPI_XC_3D_INPUT_MODE e3DMode;
    E_MSAPI_XC_HDMI_VIC_4Kx2K_CODE e4K2KVIC;
} MSAPI_XC_HDMIRX_VS_PACKET;

typedef struct
{
    MS_BOOL bAVMute;
    E_MSAPI_XC_HDMITX_CD_TYPE eCDType;
} MSAPI_XC_HDMIRX_GC_PACKET;

typedef struct
{
    E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT eChannelCount;
    E_MSAPI_XC_HDMI_AUDIO_CODING_TYPE eCodingType;
    E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY eFrequency;
} MSAPI_XC_HDMIRX_AUDIO_PACKET;

typedef struct
{
    E_MSAPI_XC_HDMITX_OUTPUT_COLOR eColorFormat;
    E_MSAPI_XC_HDMITX_CD_TYPE eColorDepth;
    E_MSAPI_XC_HDMITX_OUTPUT_MODE eOutputMode;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eOutputTiming;
    E_MSAPI_XC_ASPECT_RATIO_TYPE eAspectRatio;
    E_MSAPI_XC_3D_OUTPUT_MODE eXC3DOutputMode;
    E_MSAPI_XC_HDMI_COLORMETRY eColorimetry;
    E_MSAPI_XC_HDMI_EXT_COLORMETRY eExtendColorimetry;
    MS_BOOL bAVMute;
    E_MSAPI_XC_HDMITX_OUTPUT_QUANT_RANGE QuantizationRange;
} MSAPI_XC_HDMITX_OUTPUT_INFO;

typedef struct
{
    E_MSAPI_XC_CFD_CONTROL_TYPE etype;
    MS_U16 u16Length;
    void* pParam;
} MSAPI_XC_CFD_SENDCFD_INFO;

// DLC CGC
#define MSAPI_XC_DLC_CGC_CGAIN_OFFSET        0x08
#define MSAPI_XC_DLC_CGC_CGAIN_LIMIT_H       0x38
#define MSAPI_XC_DLC_CGC_CGAIN_LIMIT_L       0x08
#define MSAPI_XC_DLC_CGC_YC_SLOP             0x0C
#define MSAPI_XC_DLC_PURE_IMAGE_DLC_CURVE    0
#define MSAPI_XC_DLC_PURE_IMAGE_LINEAR_CURVE 1
#define MSAPI_XC_DLC_PURE_IMAGE_DO_NOTHING   2

#define INVALID_PQ_SRC_TYPE             (0xFFFF)
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_BOOL g_msAPI_XC_bIsPreviewEnable;
extern MS_BOOL g_msAPI_XC_bIsCropByMVOP;

#ifdef MSAPI_XC_MUTEX
extern MS_S32 _MSAPI_XC_Mutex;
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//3D
void msAPI_XC_Correct3DOutputMode(E_MSAPI_XC_3D_OUTPUT_MODE OutputMode, E_MSAPI_XC_3D_OUTPUT_MODE* CorrectMode);
E_MSAPI_XC_RESULT msAPI_XC_Get3DInfo(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MSAPI_XC_3D_INFO *pstAPI_XC_3DInfo);
E_MSAPI_XC_RESULT msAPI_XC_Set3DMode_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_3D_INPUT_MODE en3DInMode, E_MSAPI_XC_3D_OUTPUT_MODE en3DOutMode);

void msAPI_XC_Init_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MSAPI_XC_INITDATA stAPI_XC_InitData);

//Init
INTERFACE void msAPI_XC_Init(MSAPI_XC_INITDATA stAPI_XC_InitData);

//Set Input Path
void msAPI_XC_SetConnect_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_INPUT_SRC eInputSrc, E_MSAPI_XC_WINDOW eWindow);

//Disconnect Input Path
E_MSAPI_XC_RESULT msAPI_XC_SetDisconnect_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_INPUT_SRC eInputSrc, E_MSAPI_XC_WINDOW eWindow);

//Get Input Path
E_MSAPI_XC_INPUT_SRC msAPI_XC_GetConnect_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow);

//For Boot Logo Function
INTERFACE MS_BOOL msAPI_XC_GetBootLogoFlag(void);
E_MSAPI_XC_RESULT msAPI_XC_SetBootLogoFlag_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_BOOL bEnable);

//PQ Related
INTERFACE void msAPI_XC_Periodic_Handler(E_MSAPI_XC_WINDOW eWindow, MS_BOOL bRealTimeMonitorOnly);

/******************************************************************************/
/// Set NR
/// @param enInputSourceType \b IN: for different input source
/// @param bAuto \b IN: auto on/off
/// @param en3DNRType \b IN: different NR mode
/******************************************************************************/
void msAPI_XC_PQ_SetNR_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_XC_PQ_3D_NR en3DNRType, E_MSAPI_XC_WINDOW eWindow);

//wrapper for PQ reduce BW
INTERFACE void msAPI_XC_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On);

//DAC related
INTERFACE void msAPI_XC_Init_DAC_Panel_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId);

//Exit
INTERFACE void msAPI_XC_Exit_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId);

//------------------------------------------------------------------------------
/// @brief DTV Set Mode - Set VDEC FRC(frame rate convert) mode and Scaler RFBL Mode for some chip
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
extern MS_BOOL msAPI_XC_DTV_SetMode(void);

//Set information of Cap/Crop/Disp windows to XC
extern void msAPI_XC_SetWin_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MSAPI_XC_WINDOW_TYPE *ptSrcWin, MSAPI_XC_WINDOW_TYPE *ptCropWin, MSAPI_XC_WINDOW_TYPE *ptDstWin, E_MSAPI_XC_WINDOW eWindow);

//---------------------------------------------------------------------------------------------
/// @brief: (1) Enable/Disable Blue Screen
///         (2) MVOP send blue video to clear Scaler buffer to fix display garbage at beginning
//---------------------------------------------------------------------------------------------
extern void msAPI_XC_SetVideoMute_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_BOOL bEnable, E_MSAPI_XC_WINDOW eWindow);

//---------------------------------------------------------------------------------------------
/// @brief: (1) Enable/Disable Blue Screen
///         (2) MVOP send blue video to clear Scaler buffer to fix display garbage at beginning
//---------------------------------------------------------------------------------------------
extern E_MSAPI_XC_RESULT msAPI_XC_SetVideoMuteByMode_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_BOOL bEnable, E_MSAPI_XC_WINDOW eWindow, E_MSAPI_XC_VIDEO_MUTE_MODE eMode);


E_MSAPI_XC_RESULT msAPI_XC_SetVideoFreeze(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_BOOL bEnable, E_MSAPI_XC_WINDOW eWindow);

//----------------------------------------------------------------------------
/// @brief: Enable/Disable HD Output YpbPr/HDMItx
//----------------------------------------------------------------------------
extern MS_BOOL msAPI_XC_EnableMiscOutput(MS_BOOL bEnable);

//----------------------------------------------------------------------------
/// @brief: Enable/Disable HD Output accroding to the type
//----------------------------------------------------------------------------
MS_BOOL msAPI_XC_EnableMiscOutputByType(MS_BOOL bEnable,E_MSAPI_XC_MISC_TYPE eMiscType);

//----------------------------------------------------------------------------
/// @brief: Change the Output Resolution of HDMITx/YPbPr of HD Path according to the PNL type set into msAPI_XC_SetPNLType()
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_ChangeOutputResolution_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming);
E_MSAPI_XC_RESULT msAPI_XC_GetOutputResolution_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_OUTPUT_TIMING_TYPE *eTiming);
E_MSAPI_XC_RESULT msAPI_XC_GetOutputScanType_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_BOOL *bIsInterlaced);


//----------------------------------------------------------------------------
/// @brief: Set the Aspect Ratio Type of XC Sys
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetAspectRatioType_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, E_MSAPI_XC_ASPECT_RATIO_TYPE eAspRatio);
E_MSAPI_XC_RESULT msAPI_XC_GetAspectRatioType_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, E_MSAPI_XC_ASPECT_RATIO_TYPE *eAspRatio);

//----------------------------------------------------------------------------
/// @brief: Set Manual Panning information for Aspect Ratio change
/// For example,
/// We set stManualPanInfo.bHorizontal to TRUE and set stManualPanInfo.u16HorizontalRange to 100.
/// stManualPanInfo.u16HorizontalValue value 50 represents the 4:3 window in the center of the 16:9 image.
/// stManualPanInfo.u16HorizontalValue value 0 represents the 4:3 window start at the left hand edge of the 16:9 image.
/// stManualPanInfo.u16HorizontalValue value 100 represents the 4:3 window end at the right hand edge of the 16:9 image
//----------------------------------------------------------------------------
extern MS_BOOL msAPI_XC_SetManualPan(MSAPI_XC_MANUAL_PAN_INFO stManualPanInfo);

//---------------------------------------------------------------------------------------------
/// @brief: Check if msAPI_XC_Init have been invoked
//---------------------------------------------------------------------------------------------
MS_BOOL msAPI_XC_CheckInitStatus_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId);

//---------------------------------------------------------------------------------------------
/// @brief: Check if msAPI_XC_Init have been invoked
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_XC_CheckInitStatus(void);

//------------------------------------------------------------------------------
/// @brief set MVOP MIU selection for DTV input (Set with VDEC frame buffer MIU selection)
/// @param[in] eXC_DecoderPath select MVOP according to input decoder path
/// @param[in] bEnable enable flag for MVOP MIU selection setting
/// @param[in] eInputCfg set MVOP control mode
//------------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_DECODER_PATH eXC_DecoderPath, MS_BOOL bEnable, MS_U8 u8MIUSel);

//------------------------------------------------------------------------------
/// @brief set MVOP output timing according to MVD/h.264 video frame information
/// @param[in] eXC_DecoderPath select MVOP according to input decoder path
/// @param[in] eInputCfg set MVOP control mode
//------------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_DECODER_PATH eXC_DecoderPath, MVOP_InputSel eInputCfg);

//---------------------------------------------------------------------------------------------
/// @brief: Set/Get VDEC information
//---------------------------------------------------------------------------------------------
extern E_MSAPI_XC_RESULT msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_DECODER_PATH eXC_DecoderPath, MSAPI_XC_VDEC_DispInfo stVidStatus);
extern MS_BOOL msAPI_XC_GetVDECInfo_EX_ByPath(E_MSAPI_XC_DECODER_PATH eXC_DecoderPath, MSAPI_XC_VDEC_DispInfo *pstVidStatus);

//----------------------------------------------------------------------------
/// @brief: Set/Get VDEC Information by VDEC 2.0 api
//----------------------------------------------------------------------------
extern MS_BOOL msAPI_XC_SetVDECInfo_EX(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pstVidStatus);
extern MS_BOOL msAPI_XC_GetVDECInfo_EX(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pstVidStatus);

//---------------------------------------------------------------------------------------------
/// @brief: ACE Related Function
//---------------------------------------------------------------------------------------------
MS_BOOL msAPI_XC_ACE_SetBrightness_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_U8 u8Value);
MS_BOOL msAPI_XC_ACE_SetSharpness_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_U8 u8Value);
MS_BOOL msAPI_XC_ACE_SetContrast_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_U8 u8Value);
MS_BOOL msAPI_XC_ACE_SetHue_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_U8 u8Value);
MS_BOOL msAPI_XC_ACE_SetSaturation_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_U8 u8Value);

//----------------------------------------------------------------------------
/// @brief: XC Set Framebuffer
//----------------------------------------------------------------------------
MS_BOOL msAPI_XC_SetFrameBufferSize_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_U32 u32Size, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief: To set seamless zapping \n
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetSeamlessZapping_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_BOOL bEnable, MS_U32 u32DSAddr);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI XC set Debug Message Level
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_TYPE eDBGLevel);

//---------------------------------------------------------------------------------------------
/// @brief: Get Initial Data
//---------------------------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_GetInitInfo_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MSAPI_XC_INITDATA *stInitInfo);

//----------------------------------------------------------------------------
/// @brief: msAPI XC Enable/Disable RFBL Mode flag
/// note: system of XC will auto switch to FB mode in preview mode
//----------------------------------------------------------------------------
MS_BOOL msAPI_XC_EnableRFBLMode(MS_BOOL bEnable);

//----------------------------------------------------------------------------
/// @brief: msAPI XC Get RFBL Mode flag                     \n
/// return TRUE if:                                         \n
/// (1)msAPI_XC_EnableRFBLMode(TRUE) is set                 \n
/// (2)for some event that RFBL is needed on some chip
//----------------------------------------------------------------------------
MS_BOOL msAPI_XC_GetRFBLMode(void);

//----------------------------------------------------------------------------
/// @brief: msAPI XC Load Bandwidth Table \n
/// note: Load BW Table by index \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_LoadBWTable(E_MSAPI_XC_BW_TBL_TYPE eTBLType);

//----------------------------------------------------------------------------
/// @brief: msAPI XC Switch IPMUX to CCIR \n
/// note:  \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_EnableCCIRInput(MS_U8 u8Mode, MS_BOOL bEnable);

//----------------------------------------------------------------------------
/// @brief: To Enable MADI Force Motion Type \n
/// note:  De-interlace is BOB mode if Motion Type be ENABLE\n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_EnableMADiForceMotion_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, E_MSAPI_XC_MADI_MOTION_TYPE eType, MS_BOOL bEnable);

//----------------------------------------------------------------------------
/// @brief: To Get window information in stXC_SetWin_Info\n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_GetWindowInfo_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MSAPI_XC_WINDOW_TYPE *ptWin, E_MSAPI_XC_WINDOW_TYPE Type);

//----------------------------------------------------------------------------
/// @brief: To Get frame rate convert mode\n
//----------------------------------------------------------------------------
E_MSAPI_XC_FRC_MODE msAPI_XC_GetFRCMode_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow);

//----------------------------------------------------------------------------
/// @brief: To Get frame rate convert mode\n
//----------------------------------------------------------------------------
E_MSAPI_XC_FRC_MODE msAPI_XC_GetFRCMode(E_MSAPI_XC_WINDOW eWindow);

//----------------------------------------------------------------------------
/// @brief: To initialize functions of XC Advanced Color Engine \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_Init_ACE_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow);

//----------------------------------------------------------------------------
/// @brief: To Set Info for still JPEG \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetJPEGInfo(MSAPI_XC_JPEG_INFO stJpegInfo);

//------------------------------------------------------------------------------
/// @brief set MVOP output timing according to MVD/h.264 video frame information
/// @param[in] eInputCfg set MVOP control mode
//------------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_ForceMVOPDramType_EX(MVOP_DevID eMVOP_DeviceId, E_MSAPI_XC_MVOP_SOURCE_TYPE eType);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC set command interface.
/// @param[in] stHd include window info or call back function
/// @param[in] eCmd set command (enum of E_MSAPI_XC_Command)
/// @param[in] pPara paramenters for each command
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetCommand(MSAPI_XC_Handle* stHd, E_MSAPI_XC_COMMAND eCmd, void* pPara);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC set color range convert.
/// @param[in] eColorConType set command (enum of E_MSAPI_XC_Color_Convert_Type)
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetColorRangeConvert(E_MSAPI_XC_Color_Convert_Type eColorConType);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC set CFD control interface.
/// @param[in] stHd include window info or call back function
/// @param[in] eCmd set command (enum of E_MSAPI_XC_CFD_CONTROL)
/// @param[in] pPara paramenters for each command
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_CFD_Control(MSAPI_XC_Handle* stHd, E_MSAPI_XC_CFD_CONTROL eCmd, void* pPara);

//-----------------------------------------------------------------------------
/// @brief:msAPI_Panel_GetInfo Get panel info
/// @param[in] Device_ID
/// @param[in] enType set command
/// @param[out] pBuf value of panel info
//-----------------------------------------------------------------------------
void msAPI_Panel_GetInfo_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_PANEL_INFO_TYPE enType, void *pBuf);

//----------------------------------------------------------------------------
/// @brief: get MSAPI XC Status
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_GetStatus(E_MSAPI_XC_STATUS_TYPE eStatusType, MS_U32* pRet, MS_U32 ret_size);

//-----------------------------------------------------------------------------
/// @brief:msAPI_MVOP_SetUserDefClkFreq Set user defined MVOP clock frequency.
/// @param[in] bEnable Enable/disable user defined MVOP frequency
/// @param[in] eMinClkFreq Mininum value of user defined MVOP clock frequency
/// @param[in] eMaxClkFreq Maxinum value of user defined MVOP clock frequency
//-----------------------------------------------------------------------------
void msAPI_MVOP_SetUserDefClkFreq(MS_BOOL bEnable, E_MSAPI_MVOP_CLK_FREQ eMinClkFreq, E_MSAPI_MVOP_CLK_FREQ eMaxClkFreq);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_SoftwareColorCorrection
/// @param[in] bEnable Enable/disable software color correction
//-----------------------------------------------------------------------------
void msAPI_XC_SoftwareColorCorrection(MS_BOOL bEnable);

//-----------------------------------------------------------------------------
/// @brief: set callback for aspect ratio
/// @param[in] callback
//-----------------------------------------------------------------------------
void msAPI_XC_RegisterFunction(MSAPI_XC_CALLBACK_INFO *pstCBInfo);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_SetFrameColor_EX
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetFrameColor_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_U32 u32aRGB);

//-----------------------------------------------------------------------------
/// @brief:MApi_XC_EX_SetOSDLayer
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetOSDLayer_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_VOP_OSD_LAYER_SEL enOSD_Layer, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:MApi_XC_EX_SetVideoAlpha
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetVideoAlpha_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_U8 u8Val, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:MApi_XC_EX_IsBlackVideoEnable
//-----------------------------------------------------------------------------
MS_BOOL msAPI_XC_IsBlackVideoEnable_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_ZorderMainWindowFirst_EX
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_ZorderMainWindowFirst_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_BOOL bMainFirst);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_EnableWindow_EX
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_EnableWindow_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow, MS_BOOL bEnable, MSAPI_XC_WINDOW_TYPE *Type);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_IsFreezeImg_EX
//-----------------------------------------------------------------------------
MS_BOOL msAPI_XC_IsFreezeImg_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_SetDispWinToReg_EX
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetDispWinToReg_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MSAPI_XC_WINDOW_TYPE *Type, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_GetDispWinToReg_EX
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_GetDispWinFromReg_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MSAPI_XC_WINDOW_TYPE *Type, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:msAPI_XC_WaitOutputVSync_EX
/// @return @ref MS_U8 retun the number of left Vsync if timeout
//-----------------------------------------------------------------------------
MS_U8 msAPI_XC_WaitOutputVSync_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:Set VDEC stream ID
//-----------------------------------------------------------------------------
MS_BOOL msAPI_XC_SetVDECStreamId(VDEC_StreamId* pStreamId, E_MSAPI_XC_WINDOW eWindow);

//-----------------------------------------------------------------------------
/// @brief:Set VDEC stream ID
//-----------------------------------------------------------------------------
MS_BOOL msAPI_XC_SetVDECStreamId(VDEC_StreamId* pStreamId, E_MSAPI_XC_WINDOW eWindow);

E_MSAPI_XC_RESULT msAPI_XC_InterruptAttach_EX(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId, E_MSAPI_XC_INT_SRC enIntNum, pfnMSAPI_XC_InterruptCb pIntCb, void * pParam);

#include "msAPI_DIPMultiView.h"
#include "msAPI_HDMI.h"

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif  // _MSAPI_XC_H_
