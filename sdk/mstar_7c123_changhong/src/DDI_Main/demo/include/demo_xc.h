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
#ifndef _DEMO_XC_H_
#define _DEMO_XC_H_
#include "apiXC.h"
#include "drvXC_IOPort.h"
#include "drvTVEncoder.h"

#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#define IPANEL(dev, func) g_IPanelEx.func(dev)
#else
#include "apiXC.h"
#define IPANEL(dev, func) g_IPanel.func()
#endif
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif
typedef enum
{
    E_OUTPUT_TIMING_720X480_60I = 0,
    E_OUTPUT_TIMING_720X480_60P = 1,
    E_OUTPUT_TIMING_720X576_50I = 2,
    E_OUTPUT_TIMING_720X576_50P = 3,
    E_OUTPUT_TIMING_1280X720_50P = 4,
    E_OUTPUT_TIMING_1280X720_60P = 5,
    E_OUTPUT_TIMING_1920X1080_50I = 6,
    E_OUTPUT_TIMING_1920X1080_60I = 7,
    E_OUTPUT_TIMING_1920X1080_24P = 8,
    E_OUTPUT_TIMING_1920X1080_25P = 9,
    E_OUTPUT_TIMING_1920X1080_30P = 10,
    E_OUTPUT_TIMING_1920X1080_50P = 11,
    E_OUTPUT_TIMING_1920X1080_60P = 12,
    E_OUTPUT_TIMING_640X480_60P = 13,
    E_OUTPUT_TIMING_3840X2160_24P = 14,
    E_OUTPUT_TIMING_3840X2160_25P = 15,
    E_OUTPUT_TIMING_3840X2160_30P = 16,
    E_OUTPUT_TIMING_3840X2160_50P = 17,
    E_OUTPUT_TIMING_3840X2160_60P = 18,
    E_OUTPUT_TIMING_4096X2160_24P = 19,
    E_OUTPUT_TIMING_4096X2160_25P = 20,
    E_OUTPUT_TIMING_4096X2160_30P = 21,
    E_OUTPUT_TIMING_4096X2160_50P = 22,
    E_OUTPUT_TIMING_4096X2160_60P = 23,
}E_OUTPUT_TIMING_TYPE;

typedef enum
{
    E_DDI_OUTPUT_DEST_SD_CVBS      = 0,
    E_DDI_OUTPUT_DEST_SD_SVIDEO    = 1,
    E_DDI_OUTPUT_DEST_SD_SCART_RGB = 2,
    E_DDI_OUTPUT_DEST_SD_YPBPR     = 3,
    E_DDI_OUTPUT_DEST_HD_COMPONENT = 4,
    E_DDI_OUTPUT_DEST_HD_HDMITX    = 5,
}E_DDI_OUTPUT_DESTINTATION;

typedef enum
{
    E_ASPECT_RATIO_NORMAL           = 0,
    E_ASPECT_RATIO_4_3              = 1,
    E_ASPECT_RATIO_16_9             = 2,
    E_ASPECT_RATIO_CUT_4_3          = 3,
    E_ASPECT_RATIO_CUT_16_9         = 4,
    E_ASPECT_RATIO_FULL             = 5,
}E_ASPECT_RATIO_TYPE;

typedef enum
{
    E_CCIR656_INPUT_720X480_60I        = 0,
    E_CCIR656_INPUT_720X480_60P        = 1,
    E_CCIR656_INPUT_720X576_50I        = 2,
    E_CCIR656_INPUT_720X576_50P        = 3,
}E_CCIR656_INPUT_TYPE;

typedef enum
{
    E_DDI_XC_INPUT_SOURCE_DTV           = 0,
    E_DDI_XC_INPUT_SOURCE_DTV2          = 1,
    E_DDI_XC_INPUT_SOURCE_HDMI          = 2,
    E_DDI_XC_INPUT_SOURCE_SCALER0_OP    = 3,
}E_DDI_XC_INPUT_SOURCE_TYPE;

typedef struct
{
    MSAPI_XC_WINDOW_TYPE stCropWin[E_MSAPI_XC_MAX_WINDOW];
    MSAPI_XC_WINDOW_TYPE stDestWin[E_MSAPI_XC_MAX_WINDOW];
    E_ASPECT_RATIO_TYPE eAS;
}MS_DEMO_XC_DISP_INFO;

typedef struct
{
    MS_VE_WINDOW_TYPE stSrcWin;
    MS_VE_WINDOW_TYPE stDestWin;
    E_ASPECT_RATIO_TYPE eAS;
}MS_DEMO_VE_DISP_INFO;

typedef enum
{
    MS_WSS_4x3_FULL                    = 0x08,
    MS_WSS_14x9_LETTERBOX_CENTER       = 0x01,
    MS_WSS_14x9_LETTERBOX_TOP          = 0x02,
    MS_WSS_16x9_LETTERBOX_CENTER       = 0x0B,
    MS_WSS_16x9_LETTERBOX_TOP          = 0x04,
    MS_WSS_ABOVE16x9_LETTERBOX_CENTER  = 0x0D,
    MS_WSS_14x9_FULL_CENTER            = 0x0E,
    MS_WSS_16x9_ANAMORPHIC             = 0x07,

    MS_WSS_625_COPYRIGHT               = 0x1000,
    MS_WSS_625_COPY_PROTECTION         = 0x2000
}MS_VE_WSS_TYPE;

typedef enum
{
    MS_WSS_525_4x3_NORMAL               = 0x00,
    MS_WSS_525_4x3_LETTERBOX            = 0x02,
    MS_WSS_525_16x9_ANAMORPHIC          = 0x01,
    MS_WSS_525_RESERVED                 = 0x03,

    MS_WSS_525_COPYRIGHT                = 0x40,
    MS_WSS_525_COPY_PROTECTION          = 0x80,

    MS_WSS_525_APS_NO_APS               = 0x0000,
    MS_WSS_525_APS_PSP_CS_OFF           = 0x0200,
    MS_WSS_525_APS_PSP_2_LINE_CS        = 0x0100,
    MS_WSS_525_APS_PSP_4_LINE_CS        = 0x0300,
}MS_VE_WSS_525_TYPE;


/// CGMS_A type, Bit Definitions in CEA-805A
typedef enum
{
    E_CGMS_A_COPY_FREELY   = 0x00,
    E_CGMS_A_COPY_NO_MORE  = 0x01,
    E_CGMS_A_COPY_ONCE     = 0x02,
    E_CGMS_A_COPY_NEVER    = 0x03,
    E_CGMS_A_INVALID       = 0xFF
} E_CGMS_A_TYPE;

/// APS type, Bit Definitions in IEC 61880 and CEA608
typedef enum
{
    E_APS_NO_APS            = 0x00,
    E_APS_PSP_ONLY          = 0x01,
    E_APS_PSP_2_LINE_CB     = 0x02,
    E_APS_PSP_4_LINE_CB     = 0x03,
    E_APS_INVALID           = 0xFF
} E_APS_TYPE;


/*
 * This Enum type was defined base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
 * - PAL denotes signal standards PAL-B, -D, -G, -H, -I, -K, -L
 * - NTSC TYPE 1/2/3 include NTSC-M, NTSC-J, PAL-M and NTSC-443
 * - PAL TYPE 1/2/3 include PAL, PAL-N, PAL-Nc and SECAM
 */
typedef enum
{
    MS_ACP_CVBS_NTSC_TYPE1 = 0,         //NTSC, AGC only (Table2)
    MS_ACP_CVBS_NTSC_TYPE2,             //NTSC, AGC + 2-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TYPE3,             //NTSC, AGC + 4-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TEST_N01,          //NTSC, RGB Copy Protect on (N01) (Table2)
    MS_ACP_CVBS_NTSC_TEST_N02,          //NTSC, RGB Copy Protect on (N02) (Table2)
    MS_ACP_CVBS_PAL_TYPE1 = 0x10,       //PAL, Type1, BPP = 0x40 (Table2)
    MS_ACP_CVBS_PAL_TYPE2,              //PAL, Type2, BPP = 0x60 (Table2)
    MS_ACP_CVBS_PAL_TYPE3,              //PAL, Type3, BPP = 0x50 (Table2)
    MS_ACP_CVBS_PAL_TEST_P01,           //PAL, RGB Copy Protect on (P01) (Table2)
    MS_ACP_CVBS_PAL_TEST_P02,           //PAL, RGB Copy Protect on (P02) (Table2)
    MS_ACP_COMPONENT_480I_TYPE2 = 0x20, //YPbPr, NTSC-Type2:700/300 Levels (Table4)
    MS_ACP_COMPONENT_480I_TYPE3,        //YPbPr, NTSC-Type3:714/286 Levels (Table4)
    MS_ACP_COMPONENT_480I_TEST_N01,     //YPbPr, 480I Test N01 (Table)
    MS_ACP_COMPONENT_480I_TEST_N02,     //YPbPr, 480I Test N02 (Table)
    MS_ACP_COMPONENT_480P_EIA_7701,     //YPbPr, 525P EIA-770.1 (Table6)
    MS_ACP_COMPONENT_480P_EIA_7702,     //YPbPr, 525P EIA-770.2 (Table6)
    MS_ACP_COMPONENT_576I_TEST_P01,     //YPbPr, Test P01 (Table8)
    MS_ACP_COMPONENT_576I_TEST_P02,     //YPbPr, Test P02 (Table8)
    MS_ACP_COMPONENT_576P,              //YPbPr, 625P (Table9)
    MS_ACP_SVIDEO_NTSC_TYPE2 = 0x40,    //S-video, NTSC-Type2 (Table4)
    MS_ACP_SVIDEO_NTSC_TYPE3,           //S-video, NTSC-Type3 (Table4)
    MS_ACP_SVIDEO_PAL_TEST_P01,         //S-video, PAL Test P01 (Table8)
    MS_ACP_SVIDEO_PAL_TEST_P02,         //S-video, PAL Test P02 (Table8)
    MS_ACP_TYPE_MAX,
} MS_ACP_TYPE;

/*
 * This Enum type was defined base on DCS for composite (CVBS) and component outputs
 */
typedef enum
{
    MS_ACP_DCS_TYPE_MIN = 0,
    MS_ACP_DCS_CVBS_NTSC = MS_ACP_DCS_TYPE_MIN,
    MS_ACP_DCS_CVBS_PAL,
    MS_ACP_DCS_TYPE_MAX = MS_ACP_DCS_CVBS_PAL,
    MS_ACP_DCS_TYPE_NUM,
} MS_ACP_DCS_TYPE;

typedef enum
{
    MS_XC_MM_Init,
    MS_XC_MM_PlayVidio,
    MS_XC_MM_EnableMute,
    MS_XC_MM_DisableMute,
    MS_XC_MM_SetXCPath,
    MS_XC_MM_MAX,
} MS_XC_MM_Command;

typedef struct
{
    MS_U8 u8DeviceId;
    MS_U8 u8Window;
    E_DDI_XC_INPUT_SOURCE_TYPE eDDIXCInputSrc;
}MS_XC_MM_Params;
//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
//Init
MS_BOOL Demo_XC_Init(MS_U32 *pu32XCDevice);
MS_BOOL Demo_VE_Init(void);

//Set Output
MS_BOOL Demo_XC_SetOutputTiming(MS_U32 *pu32XCDevice, MS_U32 *pu32OutputTiming);
MS_BOOL Demo_XC_SetOutputDest(MS_U32 *pu32OutputDest, MS_U32 *pu32Enable);
MS_BOOL Demo_VE_SetOutputTiming(MS_U32 *pu32OutputTiming);
MS_BOOL Demo_VE_PlayVideo(void);

//Set Window Number
MS_BOOL Demo_XC_PlayWindow(MS_U8 *pu8WinNum);
//Play Video
MS_BOOL Demo_XC_PlayVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc);
MS_BOOL Demo_XC_PlayCusVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc);
//Stop Video
MS_BOOL Demo_XC_StopVideo(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc);
MS_BOOL Demo_XC_SetCropWin(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);
MS_BOOL Demo_XC_SetDestWin(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);
MS_BOOL Demo_XC_SetAS(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32AS);
MS_BOOL Demo_XC_SetVideoMute(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable);
MS_BOOL Demo_XC_SetMainWindowFirst(MS_U32 *pu32XCDevice, MS_U32 *bMainFirst);
MS_BOOL Demo_XC_EnableWindow(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32Width, MS_U32 *pu32Height);

// Regression Test
MS_BOOL Demo_XC_RegressionTest_ScalingDown(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc);

#if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
// Seamless Zapping
MS_BOOL Demo_XC_EnableSeamlessZapping(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable);
MS_BOOL Demo_XC_SeamlessZapping(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32XCInputSrc);
MS_BOOL Demo_XC_SeamlessZapping_SetFreeze(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U32 *pu32Enable);
MS_BOOL Demo_XC_SeamlessZapping_IsFreeze(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow);
#endif

#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
// Seamless Zapping by using GOP still image
MS_BOOL Demo_XC_EnableStillImage(MS_BOOL *bEnable);
#if (DEMO_XC_DUALXC_TEST == 1)
MS_BOOL Demo_XC_EnableStillImage_ByDevice(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_BOOL *pbEnable);
#endif
#endif

#if ((DEMO_XC_3DVIDEO_TEST == 1) || (DEMO_XC_3DVIDEO2DOUT_TEST == 1))
//3D Video
MS_BOOL Demo_XC_Enable3D(MS_U32 *pu32XCDevice, MS_U32 *bEnable, MS_U32 *enOutMode);
MS_BOOL Demo_XC_PlayVideo_3D(MS_U32 *pu32XCDevice, MS_U32 *pu32XCInputSrc);
#endif

#if (DEMO_XC_DSSWDB_TEST == 1)
// use DS to set XC
MS_BOOL Demo_XC_EnableDS_SWDB(MS_U32 *bEnable);
MS_BOOL Demo_XC_ChangeWindowSize(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow);
#endif

//Color Related
MS_BOOL Demo_XC_SetBrightness(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *u8Value);
MS_BOOL Demo_XC_SetSharpness(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *u8Value);
MS_BOOL Demo_XC_SetContrast(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *u8Value);
MS_BOOL Demo_XC_SetHue(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *u8Value);
MS_BOOL Demo_XC_SetSaturation(MS_U32 *pu32XCDevice, MS_U32 *pu32XCWindow, MS_U8 *u8Value);

MS_BOOL Demo_XC_SetFrameColor(MS_U32 *pu32XCDevice, MS_U32 *pu32Index);
MS_BOOL Demo_XC_SetVideoAlpha(MS_U32 *pu32XCDevice,MS_U32 *pu32XCWindow, MS_U8 *pu8Value, MS_U32 *pu32VOPOSDLayer);

MS_BOOL Demo_XC_EnableDmesg(MS_U32 *pbEnable);

MS_BOOL Demo_XC_VdecDispInfo2msAPIVdecDispInfo(VDEC_EX_DispInfo *pstDispInfo, MSAPI_XC_VDEC_DispInfo *pstMSAPIDispInfo );

MS_BOOL Demo_XC_Exit(MS_U32 *pu32XCDevice);
MS_BOOL Demo_VE_Exit(void);


//DAC detect
MS_BOOL Demo_Video_DACDetect(MS_BOOL *OnlySDDac, MS_BOOL *EnableSDDetect, MS_BOOL *EnableHDDetect);

//VE VBI TTx Demo
MS_BOOL Demo_VE_VbiTtxDemo(void);

// VE CCIR out
MS_BOOL Demo_VE_CCIROut(MS_U32 *pbEnable);

// WSS demo
MS_BOOL Demo_VE_SetWSS(MS_U32 *mode);
MS_BOOL Demo_VE_Set525WSS(MS_U32 *mode);

//Set VE Frame Color
MS_BOOL Demo_VE_SetFrameColor(MS_U32 *pu32Index);

//Set VE video alpha
MS_BOOL Demo_VE_SetVideoAlpha(MS_U8 *pu8Value, MS_U32 *pu32VOPOSDLayer);

//Set VE Mute
MS_BOOL Demo_VE_SetVideoMute(MS_U32 *pu32Enable);

//Set VE Path
MS_BOOL Demo_VE_SetPath(MS_U32 *pu32Path);

//Set VE Customized Window
MS_BOOL Demo_VE_SetSrcWin(MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);
MS_BOOL Demo_VE_SetDestWin(MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);
MS_BOOL Demo_VE_SetAS(MS_U32 *pu32eAS);
MS_BOOL Demo_VE_SetWinAll(MS_U16 *crop_x, MS_U16 *crop_y, MS_U16 *crop_width, MS_U16 *crop_height,
                          MS_U16 *disp_x, MS_U16 *disp_y, MS_U16 *disp_width, MS_U16 *disp_height);

MS_BOOL Demo_VE_PlayCusVideo(void);

//Set Analog Copy Protection
MS_BOOL Demo_ACP_Set625CGMS_A(MS_U16 *pu16Type);                //CGMS-A type for 625i System
MS_BOOL Demo_ACP_Set525CGMS_A_IEC61880(MS_U16 *pu16Type);       //CGMS-A type, IEC61880
MS_BOOL Demo_ACP_Set525APS_IEC61880(MS_U16 *pu16Type);          //APS type, IEC61880
MS_BOOL Demo_ACP_SetMV(MS_U32 *pu32Enable, MS_BOOL *pbIsYPbPr, MS_U32 *pu32Index);     //Macrovision type

#if (DEMO_ACP_DCS_TEST == 1)
MS_BOOL Demo_ACP_SetDCS(MS_BOOL *pbEnable, MS_U16 *pu16DCSIdx); //DCS type
#endif

#if (DEMO_XC_HDMIRX_TEST == 1)
//HDMI setting
MS_BOOL Demo_HDMI_SetRxBypass(MS_U32 *pu32Enable);
MS_BOOL Demo_HDMI_SetColorThroughMode(MS_U32 *bEn);
#endif

//Set HDMI Tx output mode
MS_BOOL Demo_HDMI_SetTxOutputMode(MS_U32 *u32OutputMode);

//Set HDMI Color Depth
MS_BOOL Demo_HDMI_SetOutputColorDepth(MS_U32 *pu32CDType);

//For Generate Black Video
MS_BOOL Demo_XC_GenerateBlackVideo(MS_U32 *pu32XCDevice, MS_BOOL bEnable, SCALER_WIN eWindow);

//Help
MS_BOOL Demo_XC_Help(void);

#endif
