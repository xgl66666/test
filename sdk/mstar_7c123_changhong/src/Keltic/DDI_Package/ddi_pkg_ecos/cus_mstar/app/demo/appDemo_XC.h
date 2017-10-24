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

#ifndef _APPDEMO_XC_H_
#define _APPDEMO_XC_H_

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include "appDemo_Video.h"

#define VE_FRAMEBUFFER_OFFSET 16 //8(pixels)*2(Bytes)

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
}E_OUTPUT_TIMING_TYPE;

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

typedef struct
{
    //HD
    MS_WINDOW_TYPE HD_CropWin[MAX_WINDOW];
    MS_WINDOW_TYPE HD_DestWin[MAX_WINDOW];
    E_ASPECT_RATIO_TYPE HD_AS;

    //SD
    MS_VE_WINDOW_TYPE SD_SrcWin;
    MS_VE_WINDOW_TYPE SD_DestWin;
    E_ASPECT_RATIO_TYPE SD_AS;

}MS_DEMO_DISP_INFO;

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

/// Aspect Ratio type
typedef enum
{
    E_AR_PS            = 0x00,  // Pan and Scan
    E_AR_LB            = 0x01,  // Letterbox
    E_AR_4x3_Combind   = 0x02,  // LetterBox with 87.5% width when Video Input src with aspect ratio 16:9
    E_AR_16x9_Combind  = 0x03,  // Pillarbox with 83.33% height when Video Input src with aspect ratio :4:3 
    E_AR_INVALID
} E_AR_TYPE;
//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
extern MS_DEMO_DISP_INFO g_Demo_DispInfo;
extern MS_BOOL g_Demo_VE_CaptureModeEn;
extern MS_BOOL g_bRFBL_Mode;
extern MS_BOOL g_bCusRFBL_Mode;//Flag for APP or Customer layer to set RFBL Mode
extern MS_BOOL g_bIsPathDI2VE;
extern MS_BOOL g_bIsSDPreviewEnable;
extern MS_BOOL g_bIsAVSyncTimeout;
extern MS_U16 g_u16CusHRatio, g_u16CusVRatio;
extern E_AR_TYPE g_eAspectRatio;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
//Init
extern MS_BOOL appDemo_XC_Init(void);
extern MS_BOOL appDemo_VE_Init(void);

//DTV Video SetMode(VDEC/MVOP/Scaler FBL)
extern MS_BOOL appDemo_XC_DTV_SetMode(void);

//Set Resolution
extern MS_BOOL appDemo_XC_SetOutputTiming(MS_U32 *pu32OutputTiming, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);
extern MS_BOOL appDemo_VE_SetOutputTiming(MS_U32 *pu32OutputTiming, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);

//Play Video
extern MS_BOOL appDemo_XC_PlayVideo_UsingDefaultSetting(void);
#if 0////ENABLE_MM_PACKAGE
extern MS_BOOL appDemo_MM_XC_PlayVideo_UsingDefaultSetting(void);
#endif
extern MS_BOOL appDemo_XC_PlayVideo_UsingCusSetting(MS_U32 *pu32ScalerWindow);
extern MS_BOOL appDemo_VE_ZapperSetMode(void);

//Set HD/SD Destination
extern MS_BOOL appDemo_VE_SetOutputDest(MS_U16 *pu16OutputDest, MS_U16 *pEnable);
extern MS_BOOL appDemo_XC_SetOutputDest(MS_U16 *pu16OutputDest, MS_U16 *pEnable);

//Color Related
extern MS_BOOL appDemo_XC_SetBrightness(MS_U32 *pu32ScalerWindow, MS_U8 *u8Value);
extern MS_BOOL appDemo_XC_SetSharpness(MS_U32 *pu32ScalerWindow, MS_U8 *u8Value);
extern MS_BOOL appDemo_XC_SetFrameColor(MS_U32 *pu32Index);
extern MS_BOOL appDemo_XC_SetContrast(MS_U32 *pu32ScalerWindow, MS_U8 *u8Value);
extern MS_BOOL appDemo_XC_SetHue(MS_U32 *pu32ScalerWindow, MS_U8 *u8Value);
extern MS_BOOL appDemo_XC_SetSaturation(MS_U32 *pu32ScalerWindow, MS_U8 *u8Value);
extern MS_BOOL appDemo_XC_SetRBRange(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value);

//Set XC Frame Buffer Size;FB/FBL Mode Switch
extern MS_BOOL appDemo_XC_SetFrameBufferSize(MS_U32 *pu32Size);

//Help
extern MS_BOOL appDemo_XC_Help(void);

//Exit
extern MS_BOOL appDemo_XC_Shutdown(void);

//VE VBI TTx Demo
extern MS_BOOL appDemo_VE_VbiTtxDemo(void);

//For MSTAR Debug
extern MS_BOOL appDemo_XC_DumpSRAM4Debug(void);
extern MS_BOOL appDemo_XC_EnableDmesg(MS_U32 *pbEnable);

// For regression test
extern MS_BOOL appDemo_XC_PlayVideo_ScalingDown(void);
extern MS_BOOL appDemo_XC_PlayVideo_ScalingUp(void);

// WSS demo
extern MS_BOOL appDemo_VE_SetWSS(MS_U32 *mode);
extern MS_BOOL appDemo_VE_Set525WSS(MS_U32 *mode);
extern MS_BOOL appDemo_UART_Stop(MS_U32 *pbEnable);
extern MS_BOOL appDemo_XC_StatusMonitor(MS_U32 *bEnable);

// For CCIR interface
extern MS_BOOL appDemo_VE_CCIROut(MS_U32 *pbEnable);
extern MS_BOOL appDemo_XC_CCIRIn(MS_U32 *eCCIRType);

//Set Analog Copy Protection
extern MS_BOOL appDemo_ACP_Set625CGMS_A(MS_U16 *pu16Type);                //CGMS-A type for 625i System
extern MS_BOOL appDemo_ACP_SetMV(MS_U32 *pEnable, MS_U32 *pu32index);     //Macrovision type
extern MS_BOOL appDemo_ACP_Set525CGMS_A_IEC61880(MS_U16 *pu16Type);       //CGMS-A type, IEC61880
extern MS_BOOL appDemo_ACP_Set525APS_IEC61880(MS_U16 *pu16Type);          //APS type, IEC61880
extern MS_BOOL appDemo_ACP_SetDCS(MS_BOOL *pbEnable, MS_U16 *pu16DCSIdx); //DCS type

extern MS_BOOL appDemo_XC_ForcePrescaling(MS_U32 *pU32Type);

// For still image zapping
extern MS_BOOL appDemo_XC_EnableStillImage(MS_BOOL *bEnable);
extern MS_BOOL appDemo_XC_EnableFreezeImage(MS_BOOL *bEnable);

// For crop-zoom demo
extern MS_BOOL appDemo_XC_EnableCropZoomMode(MS_BOOL *bEnable, MS_U32 *u32InitX, MS_U32 *u32InitY, MS_U32 *u32Width, MS_U32 *u32Height);
extern MS_BOOL appDemo_XC_MoveCropZoomWin(MS_U32 *u32Direction, MS_U32 *u32Distance, MS_U32 *u32Step);

//For Generate Black Video
extern MS_BOOL appDemo_XC_GenerateBlackVideo(void);

#endif
