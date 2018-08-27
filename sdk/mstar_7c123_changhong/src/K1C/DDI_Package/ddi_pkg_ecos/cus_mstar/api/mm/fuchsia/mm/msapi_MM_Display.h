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
#ifndef __MSAPI_MM_DISPLAY_H__
#define __MSAPI_MM_DISPLAY_H__


#include "MsMMTypes.h"
#include "MM_Player_Def.h"
#include "MMAP_Player.h"
#include "drvXC_IOPort.h"
#include "apiHDMITx.h"
#include "drvTVEncoder.h"
#include "apiXC.h"
#include "Panel.h"

/// Aspect Ratio Type ==========================================================
typedef enum
{
    /* general */
    VIDEOSCREEN_MIN,                        ///< Video Screen Min
    VIDEOSCREEN_PROGRAM = VIDEOSCREEN_MIN, ///< according AFD or WSS setting
    VIDEOSCREEN_NORMAL,                 ///< Video Screen Normal
    VIDEOSCREEN_FULL,                      ///< video full panel resolution
    VIDEOSCREEN_ZOOM,                   ///< Video Screen Zoom
    VIDEOSCREEN_CINEMA,                 ///< Video Screen Cinema
#if 0//def ATSC_SYSTEM
    VIDEOSCREEN_WIDE1,
    VIDEOSCREEN_WIDE2,
#endif
    /* specific options for 4:3 panel */
    VIDEOSCREEN_LETTERBOX,          ///< Video Screen Letterbox
    /* specific options for wide panel */
    VIDEOSCREEN_16by9_SUBTITLE,     ///< Video Screen 16:9 subtitle
    VIDEOSCREEN_PANORAMA,           ///< Video Screen Panorama
    /* others */
    VIDEOSCREEN_14by9,                        ///< 14:9
    VIDEOSCREEN_WSS_16by9,                    ///< WSS 16:9
    VIDEOSCREEN_WSS_14by9_LETTERBOX_CENTER,   ///< WSS: 0001 14:9 Letterbox Center
    VIDEOSCREEN_WSS_14by9_LETTERBOX_TOP,      ///< WSS: 0010 14:9 Letterbox Top
    VIDEOSCREEN_WSS_16by9_LETTERBOX_CENTER,   ///< WSS: 1011 16:9 Letterbox Center
    VIDEOSCREEN_WSS_16by9_LETTERBOX_TOP,      ///< WSS: 0100 16:9 Letterbox Top
    VIDEOSCREEN_ZOOM1,                        ///< Video Screen Zoom1
    VIDEOSCREEN_ZOOM2,                        ///< Video Screen Zoom2
    VIDEOSCREEN_JUSTSCAN,                     ///< Video Screen Justscan
    VIDEOSCREEN_SCENE4_3to16_9,               ///< Video Screen Scene 4:3 to 16:9
    VIDEOSCREEN_SCENE16_9to4_3,               ///< Video Screen Scene 16:9 to 4:3
    VIDEOSCREEN_SCENE4_3to16_9_WITH_CCO,      ///< Video Screen Scene 4:3 to 16:9 with CCO
    VIDEOSCREEN_SCENE4_3to4_3_WITH_CCO,       ///< Video Screen Scene 4:3 to 4:3 with CCO
    VIDEOSCREEN_SCENE4_3to16_9_WITH_LB,       ///< Video Screen Scene 4:3 to 16:9 with LB
    VIDEOSCREEN_SCENE4_3to4_3_WITH_LB,        ///< Video Screen Scene 4:3 to 4:3 with LB
    VIDEOSCREEN_ORIGIN,
    VIDEOSCREEN_PROGRAM_16X9,
    VIDEOSCREEN_PROGRAM_4X3,
    VIDEOSCREEN_PROGRAM_14X9,
    #if 0//VGA_HDMI_YUV_POINT_TO_POINT
    VIDEOSCREEN_POINT_TO_POINT, // Video Screen Scene point to point
    #endif

    VIDEOSCREEN_NUMS,       //< numbers of video screen type
}EN_ASPECT_RATIO_TYPE;

typedef struct
{
    INPUT_SOURCE_TYPE_t enInputSourceType; //< Input source type
    EN_ASPECT_RATIO_TYPE enAspectRatio;
    U8 u8PanelPowerStatus;
#if 0//def SCART_OUT_NEW_METHOD
    U8 SCART_1_Mute;
    U8 SCART_2_Mute;
#endif
} MS_SYS_INFO;


typedef enum
{
    EN_AspectRatio_Min,
    //EN_AspectRatio_Full,
    EN_AspectRatio_16X9 = EN_AspectRatio_Min,
    EN_AspectRatio_JustScan,
    EN_AspectRatio_Original,
    EN_AspectRatio_4X3,
    EN_AspectRatio_14X9,
    EN_AspectRatio_Zoom1,
    EN_AspectRatio_Zoom2,
    EN_AspectRatio_Panorama,
    EN_AspectRatio_Num,
} EN_MENU_AspectRatio;

typedef struct
{
    MS_WINDOW_TYPE stCapWin;        ///<Capture window

    U8 u8HCrop_Left;    // H Crop Left
    U8 u8HCrop_Right;    // H crop Right
    U8 u8VCrop_Up;      // V Crop Up
    U8 u8VCrop_Down;      // V Crop Down

    U8 u8H_OverScanRatio; //H Crop for DTV,storage
    U8 u8V_OverScanRatio; //V Crop for DTV,storage

} MS_VIDEO_Window_Info_EXT;


typedef struct
{
    U16 u16VideoWidth;
    U16 u16VideoHeight;
    U8 u8AspectRate;
    U8 u8AFD;
} SRC_RATIO_INFO;


typedef enum
{
    MVD_FORBIDDEN=0,
    MVD_ASP_1TO1,       //    1 : 1
    MVD_ASP_4TO3,       //    4 : 3
    MVD_ASP_16TO9,      //   16 : 9
    MVD_ASP_221TO100,   // 2.21 : 1
    MVD_ASP_MAXNUM,
} MVD_ASPECT_RATIO_CODE;


typedef enum
{
    /// Unknow AFD.
    E_MVD_AFD_UNKNOWN           = 0,
    /// LetterBox 16:9, top posistion.
    E_MVD_AFD_BOX_16X9_TOP      = 2,
    /// LetterBox 14:9, top posistion.
    E_MVD_AFD_BOX_14X9_TOP      = 3,
    /// LetterBox 16:9, cnetre posistion.
    E_MVD_AFD_BOX_16X9          = 4,
    /// Full frame.
    E_MVD_AFD_FULL_FRAME        = 8,
    /// Centre 4:3
    E_MVD_AFD_CENTRE_4x3        = 9,
    /// Centre 16:9
    E_MVD_AFD_CENTRE_16X9       = 10,
    /// Centre 14:9
    E_MVD_AFD_CENTRE_14X9       = 11,
    /// 4:3 (with shoot and protect 14:9 centre).
    E_MVD_AFD_CENTRE_4X3_14X9   = 13,
    /// 16:9 (with shoot and protect 14:9 centre).
    E_MVD_AFD_CENTRE_16X9_14X9  = 14,
    /// 16:9 (with shoot and protect 4:3 centre).
    E_MVD_AFD_CENTRE_16X9_4X3   = 15
} MVD_AFD, *pMVD_AFD;

typedef enum
{
    E_AR_DEFAULT = 0,  // 0
    E_AR_16x9,              // 1
    E_AR_4x3,                // 2
    E_AR_AUTO,             // 3
    E_AR_Panorama,     // 4
    E_AR_JustScan,       // 5
    E_AR_Zoom2,          // 6
    E_AR_Zoom1,          // 7

    E_AR_MAX,
}MAX_AspectRatio_Info;

#ifdef NEW_VDPLAYER
#define CONSTANT_COLOR              0x01
#define GRADIENT_X_COLOR            0x02
#define GRADIENT_Y_COLOR            0x04
#define GRADIENT_X_CENTER_COLOR     0x16
#define GRADIENT_Y_CENTER_COLOR     0x32
#define GRADIENT_XY_COLOR           0x64

typedef struct
{
    MS_U16 x;                                  ///< x
    MS_U16 y;                                  ///< y
    MS_U16 width;                              ///< width
    MS_U16 height;                             ///< height
    MS_U16 radius;                             ///< radius
    MS_U32 f_clr;                              ///< frame color
    MS_U32 t_clr;                              ///< text_color
    MS_U32 b_clr;                              ///< background color
    MS_U32 g_clr;                              ///< gradient color
    MS_U8 u8Gradient;                          ///< gradient
    MS_BOOL fHighLight;                     ///< hight light or not
    MS_U8 bFrameWidth;                         ///< frame width
} OSDClrBtn;
void msAPI_OSD_DrawBlock(OSDClrBtn * pclrBtn);
#endif


void msAPI_OSD_SetClipWindow(U16 u16X0, U16 u16Y0, U16 u16X1, U16 u16Y1);
void MDrv_GE_BitBlt(GEBitBltInfo *BitbltInfo, GEPitBaseInfo *PitBaseInfo);
void MApp_Scaler_SetWindow(MS_WINDOW_TYPE *ptSrcWin,                                // start from (0,0)
                                            MS_WINDOW_TYPE *ptCropWin,              // start from (0,0)
                                            MS_WINDOW_TYPE *ptDstWin,               // start from (0,0)
                                            EN_ASPECT_RATIO_TYPE enVideoScreen,
                                            INPUT_SOURCE_TYPE_t enInputSourceType,
                                            SCALER_WIN eWindow);

//Set MM Display Function,call by MApp_Scaler_SetWindow.
//The SclaerSetupFUN define as MApp_Scaler_SetWindow....
void MM_DISP_Init(void *SclaerSetupFUN);
void MM_DISP_NPP_Init(void *NPPSclaerSetupFUN);

// for Hummingbird compile....
#define DEFAULT_SCREEN_UNMUTE_TIME  (200)
void msAPI_MM_Scaler_SetBlueScreen( BOOLEAN bEnable, U8 u8Color, U16 u16ScreenUnMuteTime, SCALER_WIN eWindow);
void msAPI_MM_Scaler_ForceOpen(void);
void msAPI_MM_Scaler_ResetDisplayStatus(void);
MS_BOOL msAPI_MM_Scaler_GetDisplayStatus(void);

#define msAPI_GE_BitBlt(_BitbltInfo, _PitBaseInfo) MDrv_GE_BitBlt(_BitbltInfo, _PitBaseInfo)
#define MDrv_GE_SetDC_CSC_FMT(mode, yuv_out_range, uv_in_range, srcfmt, dstfmt) \
        MApi_GFX_SetDC_CSC_FMT(mode, yuv_out_range, uv_in_range, srcfmt, dstfmt)

#define msAPI_GE_SetDC_CSC_FMT(mode, yuv_out_range, uv_in_range, srcfmt, dstfmt) \
        MApi_GFX_SetDC_CSC_FMT(mode, yuv_out_range, uv_in_range, srcfmt, dstfmt)
#define msAPI_GE_SetNearestMode(enable) MApi_GFX_SetNearestMode(enable)
//#define msAPI_GE_SetPatchMode(repeat) MApi_GFX_SetPatchMode(repeat)
#define msAPI_GE_SetMirror(MirrorX, MirrorY) MApi_GFX_SetMirror(MirrorX, MirrorY)
#define msAPI_GE_SetRotate(Rotate) MApi_GFX_SetRotate(Rotate)

#define MDrv_GE_ClearFrameBuffer(a,b,c)  MApi_GFX_ClearFrameBuffer(a,b,c)
#define MDrv_GE_ClearFrameBufferByWord(a,b,c)  MApi_GFX_ClearFrameBufferByWord(a,b,c)

#endif
