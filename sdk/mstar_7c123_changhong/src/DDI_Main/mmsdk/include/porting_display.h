//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2017 MStar Semiconductor, Inc. All rights reserved.
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

#include "mmsdk_interface_def.h"
#ifndef __mmsdk_porting_display__
#define __mmsdk_porting_display__


#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/// @file
/// @defgroup porting_display porting_display functions
/// @{

// Standard control flow
// PT_Display_Initialize => allocate resource and init display path
// PT_Display_RegisterCallback => after initialized set callback
// PT_Display_SetMode => set scaler
// PT_Display_SetXCWindow => Set window
// PT_Display_Finalize => free resource and finial display path

/// define display multiple-instance item
typedef void* PT_DISPLAYITEM;

/// Define the porting_display version to check the new structure version
#define PORTING_DISP_VERSION (1)

/// Define the number of Super Wide Dynamic Range histogram index
#define MMSDK_SWDRHISTOGRAM_INDEX_NUM (32)

/// Define the structure version of ST_MMSDK_VIDEO_FRAME_FORMAT_EXT
#define MMSDK_FRAME_FORMAT_EXT_VER (1)

/// Define the structure version of ST_MMSDK_DMS_DISPFRAMEFORMAT
#define VERSION_MMSDK_DMS_DISP_FRAME_FORMAT (1)
/// Define the structure version of ST_MMSDK_DMS_COLORDESCRIPTION
#define VERSION_MMSDK_DMS_COLOR_DESCRIPTION (1)
/// Define the structure version of ST_MMSDK_DMS_DOLBYHDRINFO
#define VERSION_MMSDK_DMS_DOLBY_HDR_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_HDR_FRAME_INFO
#define VERSION_MMSDK_DMS_HDR_FRAME_INFO (1)
/// Define the structure version of EN_MMSDK_DISP_FRAME_INFO_EXT_TYPE
#define VERSION_MMSDK_DMS_DISP_FRAME_INFO_EXT (1)
/// Define the structure version of ST_MMSDK_DMS_COLORHWFORMAT
#define VERSION_MMSDK_DMS_COLOR_HW_FORMAT (1)
/// Define the structure version of ST_MMSDK_DMS_COLORSWFORMAT
#define VERSION_MMSDK_DMS_COLOR_SW_FORMAT (1)
/// Define the structure version of ST_MMSDK_DMS_FRAMEFORMAT
#define VERSION_MMSDK_DMS_FRAME_FORMAT (1)
/// Define the structure version of ST_MMSDK_DMS_INITDATA
#define VERSION_MMSDK_DMS_INIT_DATA (1)
/// Define the structure version of ST_MMSDK_DMS_SETMEMORY_TYPE
#define VERSION_MMSDK_DMS_SET_MEMORY_TYPE (1)
/// Define the structure version of ST_MMSDK_DMS_SETWIN_INFO
#define VERSION_MMSDK_DMS_SET_WIN_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_WINDOW_INFO
#define VERSION_MMSDK_DMS_WINDOW_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_CREATE_WIN_INFO
#define VERSION_MMSDK_DMS_CREATE_WIN_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_ZORDER_INFO
#define VERSION_MMSDK_DMS_ZORDER_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_3D_INFO
#define VERSION_MMSDK_DMS_3D_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_HDR_FRAME_INFO
#define VERSION_MMSDK_DMS_HDR_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_SETCAPTURE_INFO
#define VERSION_MMSDK_DMS_SET_CAPTURE_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_CAPTURE_INFO
#define VERSION_MMSDK_DMS_CAPTURE_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_OUTPUT_WIN_INFO
#define VERSION_MMSDK_DMS_OUTPUT_WIN_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_MUTE_WINDOW_INFO
#define VERSION_MMSDK_DMS_MUTE_WIN_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_FREEZE_WINDOW_INFO
#define VERSION_MMSDK_DMS_FREEZE_WIN_INFO (1)
/// Define the structure version of ST_MMSDK_DMS_MUTE_COLOR_INFO
#define VERSION_MMSDK_DMS_MUTE_COLOR_INFO (1)
/// Define DMS result base
#define MMSDK_DMS_RESULT_BASE 0XFFFFFFFF

/// the display resolution type
typedef enum
{
    /// unknown
    MMSDK_DISPLAY_RES_UNKNOWN = -1,
    /// minimum
    MMSDK_DISPLAY_RES_MIN = 0,
    /// SEC32_LE32A full-hd
    MMSDK_DISPLAY_SEC32_LE32A_FULLHD = MMSDK_DISPLAY_RES_MIN,

    /// For Normal LVDS panel
    ///< 1280x1024, Pnl_AU17_EN05_SXGA
    MMSDK_DISPLAY_RES_SXGA = 1,
    ///< 1366x768, Pnl_AU20_T200XW02_WXGA
    MMSDK_DISPLAY_RES_WXGA = 2,
    ///< 1440x900, Pnl_CMO19_M190A1_WXGA, Pnl_AU19PW01_WXGA
    MMSDK_DISPLAY_RES_WXGA_PLUS = 3,
    ///< 1680x1050, Pnl_AU20_M201EW01_WSXGA,
    MMSDK_DISPLAY_RES_WSXGA = 4,
    ///< 1920x1080, Pnl_AU37_T370HW01_HD, Pnl_CMO216H1_L01_HD.h
    MMSDK_DISPLAY_RES_FULL_HD = 5,

    /// PAL
    MMSDK_DISPLAY_DACOUT_PAL_MIN = 6,
    MMSDK_DISPLAY_DACOUT_576I = MMSDK_DISPLAY_DACOUT_PAL_MIN,
    MMSDK_DISPLAY_DACOUT_576P,
    MMSDK_DISPLAY_DACOUT_720P_50,
    MMSDK_DISPLAY_DACOUT_1080P_24,
    MMSDK_DISPLAY_DACOUT_1080P_25,
    MMSDK_DISPLAY_DACOUT_1080I_50,
    MMSDK_DISPLAY_DACOUT_1080P_50,
    MMSDK_DISPLAY_DACOUT_PAL_MAX = MMSDK_DISPLAY_DACOUT_1080P_50,

    // For NTSC
    MMSDK_DISPLAY_DACOUT_NTSC_MIN = 13,
    MMSDK_DISPLAY_DACOUT_480I = MMSDK_DISPLAY_DACOUT_NTSC_MIN,
    MMSDK_DISPLAY_DACOUT_480P,
    MMSDK_DISPLAY_DACOUT_720P_60,
    MMSDK_DISPLAY_DACOUT_1080P_30,
    MMSDK_DISPLAY_DACOUT_1080I_60,
    MMSDK_DISPLAY_DACOUT_1080P_60,
    MMSDK_DISPLAY_DACOUT_NTSC_MAX = MMSDK_DISPLAY_DACOUT_1080P_60,
    MMSDK_DISPLAY_DACOUT_AUTO,

    /// 1920*1200
    MMSDK_DISPLAY_CMO_CMO260J2_WUXGA,
    //For VGA OUTPUT 60HZ
    MMSDK_DISPLAY_VGAOUT_60_MIN = 0x40,
    MMSDK_DISPLAY_VGAOUT_640x480P_60 = MMSDK_DISPLAY_VGAOUT_60_MIN,
    MMSDK_DISPLAY_VGAOUT_1280x720P_60,
    MMSDK_DISPLAY_VGAOUT_1920x1080P_60,
    MMSDK_DISPLAY_VGAOUT_60_MAX = MMSDK_DISPLAY_VGAOUT_1920x1080P_60,

    /// For VGA OUTPUT 50HZ
    /// For TTL output
    MMSDK_DISPLAY_TTLOUT_60_MIN = 0xC0,
    MMSDK_DISPLAY_TTLOUT_480X272_60 = MMSDK_DISPLAY_TTLOUT_60_MIN,
    MMSDK_DISPLAY_TTLOUT_60_MAX = MMSDK_DISPLAY_TTLOUT_480X272_60,
    MMSDK_DISPLAY_RES_MAX_NUM,
} EN_MMSDK_DISPLAY_RES_TYPE;

/// HDR metadata structure
/// Define control flag for  High Dynamic Range Imaging type
typedef enum
{
    /// No HDR
    E_MMSDK_HDR_TYPE_CTL_FLAG_NONE             = 0x00000000,
    /// If VUI is, set this flag
    E_MMSDK_HDR_TYPE_CTL_FLAG_VUI              = 0x00000001,
    /// If SEI Color Volume is, set the flag
    E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_COLOR_VOLUME = 0x00000002,
    /// If SEI Knee Function is, set the flag
    E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_KNEE_FUNCTION = 0x00000004,
    /// If SEI Tone Mapping is, set this flag
    E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_TONE_MAPPING = 0x00000008,
    /// If SEI Content Light Level is, set this flag
    E_MMSDK_HDR_TYPE_CTL_FLAG_SEI_CONTENT_LIGHT_LEVEL = 0x00000010,
    /// HDR metadata HDMI infoframe type. Document : CEA-861.3_V16BallotDraft(reserved.)
    E_MMSDK_HDR_TYPE_CTL_FLAG_HDMI_INFOFRAME = 0x00000020,
    /// Dolby HDR, inform the current hdr type to the display module
    E_MMSDK_HDR_TYPE_CTL_FLAG_DOLBY_HDR = 0x00000040,
    /// TCH HDR, inform the current hdr type to the display module
    E_MMSDK_HDR_TYPE_CTL_FLAG_TCH = 0x00000080,
} E_MMSDK_HDR_TYPE_CTL_FLAG;

/// Define VUI Info (Document : JCTVC-P1005_v1_Annex_E_changes_for_Q0084_r1)
typedef struct
{
    /// u(1)
    MMSDK_BOOL bAspectRatioInfo_PresentFlag;
    /// u(8)
    MMSDK_U8   u8AspectRatio_IDC;
    /// u(16)
    MMSDK_U16  u16SarWidth;
    /// u(16)
    MMSDK_U16  u16SarHeight;
    /// u(1)
    MMSDK_BOOL bOverscanInfo_PresentFlag;
    /// u(1)
    MMSDK_BOOL bOverscan_AppropriateFlag;
    /// u(1)
    MMSDK_BOOL bVideoSignalType_PresentFlag;
    /// u(3)
    MMSDK_U8   u8VideoFormat;
    /// u(1)
    MMSDK_BOOL bVideoFullRangeFlag;
    /// u(1)
    MMSDK_BOOL bColourDescription_PresentFlag;
    /// u(8)
    MMSDK_U8   u8ColourPrimaries;
    /// u(8)
    MMSDK_U8   u8TransferCharacteristics;
    /// u(8)
    MMSDK_U8   u8MatrixCoefficients;
    /// u(1)
    MMSDK_BOOL bChromaLocationInfo_PresentFlag;
    /// ue(v) 0~5
    MMSDK_U8   u8ChromaSample_Loc_Type_Top_Field;
    /// ue(v) 0~5
    MMSDK_U8   u8ChromaSample_Loc_Type_Bottom_Field;
    /// u(1)
    MMSDK_BOOL bTimingInfo_PresentFlag;
    /// u(1)
    MMSDK_BOOL bFixedFrameRate_Flag;
    /// u(32)
    MMSDK_U32  u32Num_Units_In_Tick;
    /// u(32)
    MMSDK_U32  u32TimeScale;
} ST_MMSDK_HDR_METADATA_MPEG_VUI;

/// Define SEI ColourVolume Info (Document : w14700 SEI - D.2.27 : Mastering display colour volume SEI message syntax)
typedef struct
{
    /// u(1)
    MMSDK_BOOL bUsed;
    /// enable SEI ColourVolume
    MMSDK_BOOL bColourVolumeSEIEnabled;
    /// max luminance value
    MMSDK_U32  u32MmaxLuminance;
    /// min luminance value
    MMSDK_U32  u32MinLuminance;
    /// [0~2][0]:DisplayPrimariesX,  [0~2][1]:DisplayPrimariesY
    MMSDK_U16  u16Primaries[3][2];
    /// [0]:WhitePointX, [1]:WhitePointY
    MMSDK_U16  u16WhitePoint[2];
}ST_MMSDK_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME;

/// Define SEI Content Light Level Info
typedef struct
{
    MMSDK_BOOL bUsed;
    MMSDK_BOOL bContentLightLevelEnabled;
    MMSDK_U16  u16MaxContentLightLevel;
    MMSDK_U16  u16MaxPicAverageLightLevel;
}ST_MMSDK_HDR_METADATA_CONTENT_LIGHT_LEVEL_SEI;

/// Define High Dynamic Range Imaging Info
typedef struct
{
    /// HDR Metadata type
    MMSDK_U32 u32HDRMetadataType;
    /// HDR Metadata Mpeg VUI
    ST_MMSDK_HDR_METADATA_MPEG_VUI stHdrMetadataMpegVUI;
    /// HDR Metadata Mpeg Color Volume
    ST_MMSDK_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME stHdrMetadataMpegSEIMasteringColorVolume;
    /// HDR Metadata Mpeg Content Light Level
    ST_MMSDK_HDR_METADATA_CONTENT_LIGHT_LEVEL_SEI stHdrMetadataSEIContentLightLevel;
} ST_MMSDK_HDR_METADATA;

/// Define callback function message.
typedef enum
{
    /// if enable dynamic scaling or not.
    E_MMSDK_DISPLAY_CALLBACK_IS_ENABLE_DYNAMIC_SCALING,
    /// update zoom info for dynamic scaling (DS)
    E_MMSDK_DISPLAY_CALLBACK_UPDATE_ZOOM_INFO,
    /// update scaler(XC) info for dynamic scaling (DS)
    E_MMSDK_DISPLAY_CALLBACK_UPDATE_XC_INFO,
    /// *((MMSDK_BOOL*)pMsgInfo) = TRUE  => Suspend DS
    /// *((MMSDK_BOOL*)pMsgInfo) = FALSE => Resume DS
    E_MMSDK_DISPLAY_CALLBACK_SUSPENDDS,
    /// check dynamic scaling is mcu mode or firmware mode
    /// TRUE: MCU mode
    /// FALSE: Firmware mode
    E_MMSDK_DISPLAY_CALLBACK_IS_MCU_MODE_DS,
    /// update Dolby&Open HDR info ,it's on or off
    E_MMSDK_DISPLAY_CALLBACK_SWITCH_HDR,
} EN_MMSDK_DISPLAY_CALLBACK_MSG;

/// Define the VDEC frame field type for push
typedef enum
{
    /// no field
    E_MMSDK_VIDEO_FRAME_FIELD_TYPE_NONE,
    /// Top field only
    E_MMSDK_VIDEO_FRAME_FIELD_TYPE_TOP,
    /// Bottom field only
    E_MMSDK_VIDEO_FRAME_FIELD_TYPE_BOTTOM,
    /// Both fields
    E_MMSDK_VIDEO_FRAME_FIELD_TYPE_BOTH,
} EN_MMSDK_VIDEO_FRAME_FIELD_TYPE;

/// Define the HDR information
typedef enum
{
    /// Open&DolbyHDR is turn off
    E_MMSDK_DISPLAY_VIDEO_HDR_STOP       = 0,
    /// Open HDR is turn on
    E_MMSDK_DISPLAY_VIDEO_OPEN_HDR_RUNNING,
    /// Dolby HDR is turn on
    E_MMSDK_DISPLAY_VIDEO_DOLBY_HDR_RUNNING,
}EN_MMSDK_DISPLAY_VIDEO_HDR_MSG;

/// Define the VDEC tile mode
typedef enum
{
    /// VDEC tile mode none
    E_MMSDK_VIDEO_TILEMODE_NONE = 0,
    /// VDEC tile mode 16x16
    E_MMSDK_VIDEO_TILEMODE_16x16,
    /// VDEC tile mode 16x32
    E_MMSDK_VIDEO_TILEMODE_16x32,
    /// VDEC tile mode 32x16
    E_MMSDK_VIDEO_TILEMODE_32x16,
    /// VDEC tile mode 32x32
    E_MMSDK_VIDEO_TILEMODE_32x32,
    /// VDEC tile mode num
    E_MMSDK_VIDEO_TILEMODE_NUM
} EN_MMSDK_VIDEO_TILEMODE;

typedef enum
{
    /// In MVC case it is L view 2 bit
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT,
    /// Interlace bottom 8bit will share the same enum value
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_INTERLACE = 1,
    /// With dolby enhance layer 8bit
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_DOLBY_EL = 1,
    /// Interlace bottom 2bit will share the same enum
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE = 2,
    /// Value with dolby enhance layer 2bit
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT_DOLBY_EL = 2,
    /// R view 2 bit
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT_MVC,
    /// u32LumaAddrI and u32ChromaAddrI from VDEC
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_INTERLACE_MVC,
    /// MVC interlace R-View 2bit will share the
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE_MVC = 5,
    /// Same enum with dolby meta data
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_DOLBY_META = 5,
    /// The MFC bit len of chroma and luma
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN,
    /// The MFC bit len of chroma and luma for the other view
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN_MVC,
    /// MAX value for declaration
    E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MAX,
} EN_MMSDK_DISP_FRM_INFO_EXT_TYPE;

///Define Open&Dolby HDR status for vsyncbridge
typedef struct
{
    /// WinID
    /// 0: Main Win
    /// 1: Sub Win
    MMSDK_U32 u32WinID;
    /// check HDR status is on of off
    /// 0:HDR STOP
    /// 1:OPEN HDR RUNNING
    /// 2:DOLBY HDR RUNNING
    EN_MMSDK_DISPLAY_VIDEO_HDR_MSG eDolbyHdrStatus;
    /// Check event is Dolby HDR
    MMSDK_BOOL bIsDolbyEvent;
}ST_MMSDK_DISPLAY_HDR_STATUS;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SET_DISPLAY.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Media item type.
    EN_MMSDK_MEDIA_TYPE enMediaType;
    /// Video codec type. (Movie)
    EN_MMSDK_VIDEO_CODEC eVideoType;
    /// Enable/Disable video display.
    MMSDK_BOOL bEnableDisplay;
    /// Video frame rate (30 frame/sec = 30000).
    MMSDK_U16 u16FrameRate;
    /// Video is interlace or progressive.
    MMSDK_U8 u8Interlace;
    /// Video width (pixel unit).
    MMSDK_U16 u16SrcWidth;
    /// Video height (pixel unit).
    MMSDK_U16 u16SrcHeight;
    /// Video right crop size (pixel unit).
    MMSDK_U16 u16CropRight;
    /// Video left crop size (pixel unit).
    MMSDK_U16 u16CropLeft;
    /// Video bottom crop size (pixel unit).
    MMSDK_U16 u16CropBottom;
    /// Video top crop size (pixel unit).
    MMSDK_U16 u16CropTop;
    /// Is 3D Layout
    MMSDK_BOOL bIs3DLayout;
    /// Is 3D format Frame Alternative
    MMSDK_BOOL bIsFrameAlternative;
    /// 3D layout type
    EN_MMSDK_3DLAYOUT en3DLayout;
    /// @deprecated Video display on MIU0 (Movie) new:#ST_MMSDK_SET_DISPLAY_INFO::u32MiuNo
    MMSDK_BOOL bMIU0;
    /// Is scaler frame buffer mode (Movie)
    MMSDK_BOOL bFrameBufferMode;
    /// Scaler buffer start.(Movie/Photo)
    MMSDK_U32 u32ScalerBuffStart;
    /// Scaler buffer size.(Movie/Photo)
    MMSDK_U32 u32ScalerBuffSize;
    ///FRC output rate(Movie)
    MMSDK_U32 u32OutputRate;
    /// Video pixel range(Movie)
    ///1: Y from  0 to 255 and Cb , Cr from  0 to 255
    ///0: Y from 16 to 235 and Cb , Cr from 16 to 240
    MMSDK_U8 u8VideoRange;
    /// Enable Dynamic Scaling by vdplayer(Movie)
    MMSDK_BOOL bEnableDynScaling;
    /// DS info buffer address (Movie)
    MMSDK_U32 u32DSInfoBaseAddr;
    /// DS info buffer size (Movie)
    MMSDK_U32 u32DSSize;
    /// DZ info in MIU? (Movie)
    MMSDK_U8 u8DSMIUSelect;
    /// DS index depth (Movie)
    MMSDK_U8 u8DSIndexDepth;
    /// Photo Display Buffer Addr (Photo)
    MMSDK_U32 u32DisplayBuffPhysicalAddr;
    /// multi miu setting
    MMSDK_U32 u32MiuNo;
    ///DS mode
    EN_MMSDK_DS_MODE enDSMode;
} ST_MMSDK_SET_DISPLAY_INFO;

/// Define rectangle information.
typedef struct
{
    /// Horizontal start.
    MMSDK_U16 u16X;
    /// Vertical start.
    MMSDK_U16 u16Y;
    /// Width.
    MMSDK_U16 u16Width;
    /// Height.
    MMSDK_U16 u16Height;
} ST_MMSDK_XC_RECT;

/// Define the information for Video Frame.
typedef struct
{
    /// Video codec type.
    EN_MMSDK_VIDEO_CODEC enType;
    /// interlace or not
    MMSDK_BOOL bInterlace;
    /// Luma address
    MMSDK_U32 u32LumaAddr;
    /// Chrom address
    MMSDK_U32 u32ChromAddr;
    /// frame width
    MMSDK_U32 u32Width;
    /// frame height
    MMSDK_U32 u32Height;
    /// frame pitch
    MMSDK_U32 u32Pitch;
}ST_MMSDK_VIDEO_FRAME_INFO;

/// @deprecated Define the push frame format information
typedef struct
{
    /// Frame index
    MMSDK_U32 u32FrameIndex;
    /// Frame reference count
    MMSDK_U32 u32PriData;
    /// Frame left crop size (pixel unit).
    MMSDK_U32 u32CropLeft;
    /// Frame right crop size (pixel unit).
    MMSDK_U32 u32CropRight;
    /// Frame top crop size (pixel unit).
    MMSDK_U32 u32CropTop;
    /// Frame bottom crop size (pixel unit).
    MMSDK_U32 u32CropBottom;
    /// 10 bit stream have the 2bit with their pitch
    MMSDK_U16 u16Pitch_2bit;
    /// LumaBitdepth for 10-bit stream
    MMSDK_U8 u8LumaBitDepth;
    /// ChromaBitdepth for 10-bit stream
    MMSDK_U8 u8ChromaBitdepth;
    /// Check is 10 bit or not
    MMSDK_BOOL bIs10Bit;
    /// @ref EN_MMSDK_VIDEO_FRAME_FIELD_TYPE Field type: Top, Bottom, Both
    EN_MMSDK_VIDEO_FRAME_FIELD_TYPE eFieldType;
} ST_MMSDK_VIDEO_FRAME_FORMAT;

/// Define the push frame format information
typedef struct
{
    /// physical luma address of subsample frame buffer
    MMSDK_U64 u64SubsampleLumaAddr;
    /// physical chroma address of subsample frame buffer
    MMSDK_U64 u64SubsampleChromaAddr;
    /// HTLB Table Address
    MMSDK_U64 u64HTLBTableAddr;
    /// HTLB Entries Addr (chroma)
    MMSDK_U64 u64HTLBChromaEntriesAddr;
    /// The user's version for compatible
    MMSDK_U32 u32Version;
    /// The user's structure size for compatible
    MMSDK_U32 u32StructureSize;
    /// Frame index
    MMSDK_U32 u32FrameIndex;
    /// Frame reference count
    MMSDK_U32 u32PriData;
    /// Frame left crop size (pixel unit).
    MMSDK_U32 u32CropLeft;
    /// Frame right crop size (pixel unit).
    MMSDK_U32 u32CropRight;
    /// Frame top crop size (pixel unit).
    MMSDK_U32 u32CropTop;
    /// Frame bottom crop size (pixel unit).
    MMSDK_U32 u32CropBottom;
    /// stream_frame_rate = time_scale / num_units_in_tick
    MMSDK_U32 u32NumUnitsInTick;
    /// The scale of frame rate
    MMSDK_U32 u32TimeScale;
    /// Super Wide Dynamic Range histogram data
    MMSDK_U16 au16Histogram[MMSDK_SWDRHISTOGRAM_INDEX_NUM];
    /// 10 bit stream have the 2bit with their pitch
    MMSDK_U16 u16Pitch_2bit;
    /// width of subsample frame buffer
    MMSDK_U16 u16SubsampleWidth;
    /// height of subsample frame buffer
    MMSDK_U16 u16SubsampleHeight;
    /// pitch of subsample frame buffer
    MMSDK_U16 u16SubsamplePitch;
    /// Max Content Light Level
    MMSDK_U16 u16MaxContentLightLevel;
    /// Max PicAverage Light Level
    MMSDK_U16 u16MaxPicAverageLightLevel;
    /// MFCodec luma compression ratio
    MMSDK_U16 u16MFCLumaRatio;
    /// MFCodec chroma compression ratio
    MMSDK_U16 u16MFCChromaRatio;
    /// LumaBitdepth for 10-bit stream
    MMSDK_U8 u8LumaBitDepth;
    /// ChromaBitdepth for 10-bit stream
    MMSDK_U8 u8ChromaBitdepth;
    /// Check is 10 bit or not
    MMSDK_BOOL bIs10Bit;
    /// To check following information is valid or not
    /// Subsample:           bits: [0] 0: invalid, 1: valid
    /// HTLB:                bits: [1] 0: invalid, 1: valid
    /// Content light:       bits: [2] 0: invalid, 1: valid
    /// MFcodec compression: bits: [3] 0: invalid, 1: valid
    /// Stream frame rate:   bits: [4] 0: invalid, 1: valid
    /// SWDR histogram:      bits: [5] 0: invalid, 1: valid
    /// V5_PAR_CROP_INFO:    bits: [6] 0: invalid, 1: valid
    /// V6_PROFILE_INFO:     bits: [7] 0: invalid, 1: valid
    MMSDK_U8 u8V7DataValid;
    /// tile mode of subsample frame buffer
    MMSDK_U8  u8SubsampleTileMode;
    /// HTLB page sizes
    /// bits: [2:0] 1st page size, [6:4] min page size
    MMSDK_U8  u8HTLBPageSizes;
    /// HTLB Entries Size (chroma)
    MMSDK_U8  u8HTLBChromaEntriesSize;
    /// @ref EN_MMSDK_VIDEO_FRAME_FIELD_TYPE Field type: Top, Bottom, Both
    EN_MMSDK_VIDEO_FRAME_FIELD_TYPE eFieldType;
} ST_MMSDK_VIDEO_FRAME_FORMAT_EXT;

/// Define the initialize information for graphic video
typedef struct
{
    /// Structure version for version control
    MMSDK_U32 u32Version;
    /// Structure size for version control
    MMSDK_U32 u32Size;
    /// @ref ST_MMSDK_SET_DISPLAY_INFO The original display information
    ST_MMSDK_SET_DISPLAY_INFO stDispInfo;
    /// @ref EN_MMSDK_COLOR_FORMAT
    EN_MMSDK_COLOR_FORMAT enInputColorFormat;
    /// Specific display output path is XC(0) or DIP(1)
    MMSDK_U8 u8DisplayOutputPath;
    /// Display width
    MMSDK_U32 u32AspectWidth;
    /// Display height
    MMSDK_U32 u32AspectHeight;
    /// Aspect ratio code
    MMSDK_U8 u8AspectRate;
    ///active frame code
    MMSDK_U8 u8AFD;
} ST_MMSDK_SET_DISPLAY_INFO_EXT;

/// Define the extern special Graphic Video information for push Graphic Video info.
typedef struct
{
    /// Structure version for version control
    MMSDK_U32 u32Version;
    /// Structure size for version control
    MMSDK_U32 u32Size;
    /// @ref ST_MMSDK_VIDEO_FRAME_INFO The array contain two ST_MMSDK_VIDEO_FRAME_INFO for frame push
    ST_MMSDK_VIDEO_FRAME_INFO astFrameInfo[2];
    /// @ref ST_MMSDK_HDR_METADATA HDR meta data information
    ST_MMSDK_HDR_METADATA stHDRMetaData;
    /// The frame rate for push Graphic Video
    MMSDK_U32 u32FrameRate;
    /// @ref EN_MMSDK_3DLAYOUT 3D layout type
    EN_MMSDK_3DLAYOUT en3DLayout;
    /// The field is bottom first or top first; 0:top->bottom, 1:bottom->top
    MMSDK_BOOL bIsBottomFieldFirst;
    /// MFC Codec information
    /// bit[29]     MFCodec 3.0 vp9 mode (1: vp9, 0: h26x)
    /// bit[28]     MFCodec uncompress mode
    /// bits[27:24] MFCodec blens buffer miu sel
    /// bit[23:16]  MFCodec pitch setting
    /// bit[8]      MFDec ID
    /// bits[7:0]   MFCodec mode
    MMSDK_U32 u32MFCodecInfo;
    /// MFC Bit length address u32LumaMFCbitlen in VDEC
    MMSDK_U32 u32MFCBitlenAddr;
    /// Tile Mode
    EN_MMSDK_VIDEO_TILEMODE eTileMode;
    //// Profiling / benchmark
    MMSDK_U16 u16MIUBandwidth;
    /// Bit rate
    MMSDK_U16 u16Bitrate;
    /// HTLB Table ID
    MMSDK_U8  u8HTLBTableId;
    /// HTLB Entries Size
    MMSDK_U8  u8HTLBEntriesSize;
    /// HTLB Entries Addr
    MMSDK_U64 u64HTLBEntriesAddr;
    /// Extended Luma address
    MMSDK_U32 au32LumaAddrExt[E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MAX];
    /// Extended Chroma address
    MMSDK_U32 au32ChromaAddrExt[E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MAX];
    /// Interlace format:  ProgressiveFrame = 0, InterlacedFrame = 2, InterlacedField  = 3
    MMSDK_U8 u8ScanType;
    /// Provide the VDEC stream ID to XC to access MApi_VDEC_EX_DisplayFrame() and MApi_VDEC_EX_ReleaseFrame()
    MMSDK_U32 u32VDECStreamID;
    /// Provide the VDEC stream ID Version to XC to access MApi_VDEC_EX_DisplayFrame() and MApi_VDEC_EX_ReleaseFrame()
    MMSDK_U32 u32VDECStreamVersion;
    /// Frame Presentation Time Stamp
    MMSDK_U64 u64PTS;
    /// @deprecated Frame format information
    ST_MMSDK_VIDEO_FRAME_FORMAT astFrameFormat[2];
    /// Frame format extension information
    ST_MMSDK_VIDEO_FRAME_FORMAT_EXT astFrameFormatExt[2];
} ST_MMSDK_VIDEO_FRAME_INFO_EXT;

/// Define the timing change type
typedef enum
{
    /// Timing change type none
    E_MMSDK_TIMING_CHANGE_INFO_TYPE_NONE         = 0,
    /// Timing change for resolution change
    E_MMSDK_TIMING_CHANGE_INFO_TYPE_RESOLUTION   = 0x00000001,
    /// Timing change for frame rate
    E_MMSDK_TIMING_CHANGE_INFO_TYPE_FRAME_RATE   = 0x00000002,
    /// Timing change for pre-notify video info (param:ST_MMSDK_SET_DISPLAY_INFO)
    E_MMSDK_TIMING_CHANGE_INFO_TYPE_DISPLAY_INFO   = 0x00000003,
} EN_MMSDK_TIMING_CHANGE_INFO_TYPE;

/// Define the timing change information
typedef struct
{
    /// The real frame rate
    MMSDK_U32 u32FrameRate;
    /// The timing change type
    MMSDK_U16 u16TimingChangeType;
    /// Video width (pixel unit).
    MMSDK_U16 u16SrcWidth;
    /// Video height (pixel unit).
    MMSDK_U16 u16SrcHeight;
    /// Reserved
    MMSDK_U32 u32Reserved;
    /// Reserved
    void *pReserved;
} ST_MMSDK_TIMING_CHANGE_INFO;

/// Define return value of DMS
typedef enum
{
    /// Return ok
    E_MMSDK_DMS_OK                = (0),
    /// Return fail
    E_MMSDK_DMS_FAIL              = (MMSDK_DMS_RESULT_BASE),
    /// Input parameter incorrect
    E_MMSDK_DMS_INPUT_INVALID     = (MMSDK_DMS_RESULT_BASE -1),
    /// Window id is incorrect
    E_MMSDK_DMS_WINID_ILLEGAL     = (MMSDK_DMS_RESULT_BASE -2),
    /// 3D mode
    E_MMSDK_DMS_EXIST_3D          = (MMSDK_DMS_RESULT_BASE -3),
    /// HDR mode
    E_MMSDK_DMS_EXIST_HDR         = (MMSDK_DMS_RESULT_BASE -4),
    /// Ve output
    E_MMSDK_DMS_EXIST_VE_OUT      = (MMSDK_DMS_RESULT_BASE -5),
    /// Memory output
    E_MMSDK_DMS_MEMORY_OUT        = (MMSDK_DMS_RESULT_BASE -6),
    /// Display win size is incorrect
    E_MMSDK_DMS_WINSIZE_ERROR     = (MMSDK_DMS_RESULT_BASE -7),
    /// Display zorder is duplicated
    E_MMSDK_DMS_ZORDER_DUPLICATED = (MMSDK_DMS_RESULT_BASE -8),
} EN_MMSDK_DMS_RESULT;

/// Define the structure of DMS color hw format
typedef struct
{
    /// NumUnitsInTick
    MMSDK_U64 u64NumUnitsInTick;
    /// TimeScale
    MMSDK_U64 u64TimeScale;
    /// phyLumaAddr
    MMSDK_PHY phyLumaAddr;
    /// phyChromaAddr
    MMSDK_PHY phyChromaAddr;
    /// phyLumaAddr2Bit
    MMSDK_PHY phyLumaAddr2Bit;
    /// phyChromaAddr2Bit
    MMSDK_PHY phyChromaAddr2Bit;
    /// phyMFCBITLEN
    MMSDK_PHY phyMFCBITLEN;
    /// phyLumaAddr_subsample
    MMSDK_PHY phyLumaAddr_subsample;
    /// phyChromaAddr_subsample
    MMSDK_PHY phyChromaAddr_subsample;
    /// HTLBTableAddr
    MMSDK_PHY phyHTLBTableAddr;
    /// HTLBChromaEntriesAddr
    MMSDK_PHY phyHTLBChromaEntriesAddr;
    /// ST_DMS_COLORHWFORMAT version
    MMSDK_U32 u32Version;
    /// sizeof(ST_DMS_COLORHWFORMAT)
    MMSDK_U32 u32Length;
    /// LumaPitch
    MMSDK_U32 u32LumaPitch;
    /// ChromaPitch
    MMSDK_U32 u32ChromaPitch;
    /// phyLumaAddr
    MMSDK_U32 u32LumaPitch2Bit;
    /// ChromaPitch2Bit
    MMSDK_U32 u32ChromaPitch2Bit;
    /// MFCodecInfo
    MMSDK_U32 u32MFCodecInfo;
    /// Histogram
    MMSDK_U16 u16Histogram[MMSDK_SWDRHISTOGRAM_INDEX_NUM];
    /// Width_subsample
    MMSDK_U16 u16Width_subsample;
    /// Height_subsample
    MMSDK_U16 u16Height_subsample;
    /// Pitch_subsample
    MMSDK_U16 u16Pitch_subsample;
    /// MaxContentLightLevel
    MMSDK_U16 u16MaxContentLightLevel;
    /// MaxPicAverageLightLevel
    MMSDK_U16 u16MaxPicAverageLightLevel;
    /// V7DataValid
    MMSDK_U8  u8V7DataValid;
    /// TileMode_subsample
    MMSDK_U8  u8TileMode_subsample;
    /// HTLBPageSizes
    MMSDK_U8  u8HTLBPageSizes;
    /// HTLBChromaEntriesSize
    MMSDK_U8  u8HTLBChromaEntriesSize;
} ST_MMSDK_DMS_COLORHWFORMAT;

/// Define the enum of DMS frame type
typedef enum
{
    E_MMSDK_DMS_FRAME_TYPE_I,
    E_MMSDK_DMS_FRAME_TYPE_P,
    E_MMSDK_DMS_FRAME_TYPE_B,
    E_MMSDK_DMS_FRAME_TYPE_OTHER,
    E_MMSDK_DMS_FRAME_TYPE_MAX,
} EN_MMSDK_DMS_FRAMETYPE;

/// Define the enum of DMS field type
typedef enum
{
    E_MMSDK_DMS_FIELD_TYPE_NONE,
    E_MMSDK_DMS_FIELD_TYPE_TOP,
    E_MMSDK_DMS_FIELD_TYPE_BOTTOM,
    E_MMSDK_DMS_FIELD_TYPE_BOTH,
    E_MMSDK_DMS_FIELD_TYPE_MAX,
} EN_MMSDK_DMS_FIELDTYPE;

/// Define the enum of DMS view type
typedef enum
{
    E_MMSDK_DMS_VIEW_TYPE_CENTER,
    E_MMSDK_DMS_VIEW_TYPE_LEFT,
    E_MMSDK_DMS_VIEW_TYPE_RIGHT,
    E_MMSDK_DMS_VIEW_TYPE_TOP,
    E_MMSDK_DMS_VIEW_TYPE_BOTTOM,
    E_MMSDK_DMS_VIEW_TYPE_MAX,
} EN_MMSDK_DMS_VIEWTYPE;

/// Define the enum of DMS aspect ratio
typedef enum
{
    E_MMSDK_DMS_AR_DEFAULT = 0,
    E_MMSDK_DMS_AR_16x9,
    E_MMSDK_DMS_AR_4x3,
    E_MMSDK_DMS_AR_AUTO,
    E_MMSDK_DMS_AR_Panorama,
    E_MMSDK_DMS_AR_JustScan,
    E_MMSDK_DMS_AR_Zoom1,
    E_MMSDK_DMS_AR_Zoom2,
    E_MMSDK_DMS_AR_14x9,
    E_MMSDK_DMS_AR_DotByDot,
    E_MMSDK_DMS_AR_Subtitle,
    E_MMSDK_DMS_AR_Movie,
    E_MMSDK_DMS_AR_Personal,
    E_MMSDK_DMS_AR_4x3_PanScan,
    E_MMSDK_DMS_AR_4x3_LetterBox,
    E_MMSDK_DMS_AR_16x9_PillarBox,
    E_MMSDK_DMS_AR_16x9_PanScan,
    E_MMSDK_DMS_AR_4x3_Combind,
    E_MMSDK_DMS_AR_16x9_Combind,
    E_MMSDK_DMS_AR_Zoom_2x,
    E_MMSDK_DMS_AR_Zoom_3x,
    E_MMSDK_DMS_AR_Zoom_4x,
    E_MMSDK_DMS_AR_CUS = 0x20,
    E_MMSDK_DMS_AR_MAX = 0x40,
} EN_MMSDK_DMS_ASPECT_RATIO;

/// Define the enum of DMS capability
typedef enum
{
    E_MMSDK_DMS_CAPABILITY_MULTI_WINDOW   = (0x1 << 1),
    E_MMSDK_DMS_CAPABILITY_VSYNC_BRIDGE   = (0x1 << 2),
    E_MMSDK_DMS_CAPABILITY_AVSYNC         = (0x1 << 3),
    E_MMSDK_DMS_CAPABILITY_SECURE_VIDEO   = (0x1 << 4),
    E_MMSDK_DMS_CAPABILITY_3D             = (0x1 << 5),
    E_MMSDK_DMS_CAPABILITY_HDR            = (0x1 << 6),
    E_MMSDK_DMS_CAPABILITY_MEMC           = (0x1 << 7),
    E_MMSDK_DMS_CAPABILITY_VE_OUT         = (0x1 << 8),
    E_MMSDK_DMS_CAPABILITY_CAPTURE_WITH_BUFFER_MODE      = (0x1 << 9),
    E_MMSDK_DMS_CAPABILITY_CAPTURE_WITH_BUFFERLESS_MODE  = (0x1 << 10),
} EN_MMSDK_DMS_CAPABILITY;

/// Define the enum of DMS color format
typedef enum
{
    E_MMSDK_DMS_COLOR_FORMAT_HW_HVD,                 //YUV420 HVD tiled format
    E_MMSDK_DMS_COLOR_FORMAT_HW_MVD,                 //YUV420 MVD tiled format
    E_MMSDK_DMS_COLOR_FORMAT_SW_YUV420_PLANAR,       //YUV420 Planar
    E_MMSDK_DMS_COLOR_FORMAT_SW_RGB565,              //RGB565
    E_MMSDK_DMS_COLOR_FORMAT_SW_ARGB8888,            //ARGB8888
    E_MMSDK_DMS_COLOR_FORMAT_YUYV,                   //YUV422 YUYV
    E_MMSDK_DMS_COLOR_FORMAT_SW_RGB888,              //RGB888
    E_MMSDK_DMS_COLOR_FORMAT_10BIT_TILE,             //YUV420 tiled 10 bits mode
    E_MMSDK_DMS_COLOR_FORMAT_SW_YUV420_SEMIPLANAR,   //YUV420 SemiPlanar
    E_MMSDK_DMS_COLOR_FORMAT_YUYV_CSC_BIT601,        //YUV422 YUYV from RGB2YUV bit601 mode
    E_MMSDK_DMS_COLOR_FORMAT_YUYV_CSC_255,           //YUV422 YUYV from RGB2YUV 0~255 mode
    E_MMSDK_DMS_COLOR_FORMAT_HW_EVD,                 //YUV420 EVD tiled format
    E_MMSDK_DMS_COLOR_FORMAT_MAX,
} EN_MMSDK_DMS_COLORFORMAT;

/// Define the structure of DMS frame format
typedef struct
{
    ST_MMSDK_DMS_COLORHWFORMAT stHWFormat;
    MMSDK_U32 u32Version;                         /// ST_DMS_FRAMEFORMAT version
    MMSDK_U32 u32Length;                         /// sizeof(ST_DMS_FRAMEFORMAT)
    MMSDK_U32 u32Width;
    MMSDK_U32 u32Height;
    MMSDK_U32 u32CropLeft;
    MMSDK_U32 u32CropRight;
    MMSDK_U32 u32CropTop;
    MMSDK_U32 u32CropBottom;
    MMSDK_U32 u32Idx;
    MMSDK_U32 u32PriData;
    EN_MMSDK_DMS_FRAMETYPE enFrameType;
    EN_MMSDK_DMS_FIELDTYPE enFieldType;
    EN_MMSDK_DMS_VIEWTYPE enViewType;
    MMSDK_U8 u8LumaBitdepth;
    MMSDK_U8 u8ChromaBitdepth;
} ST_MMSDK_DMS_FRAMEFORMAT;

/// Define the structure of DMS color description
typedef struct
{
    MMSDK_U32 u32Version;                         /// ST_DMS_COLORDESCRIPTION version
    MMSDK_U32 u32Length;                         /// sizeof(ST_DMS_COLORDESCRIPTION)
    //color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    MMSDK_U8 u8ColorPrimaries;
    MMSDK_U8 u8TransferCharacteristics;
    // matrix coefficients in deriving YUV signal from RGB
    MMSDK_U8 u8MatrixCoefficients;
} ST_MMSDK_DMS_COLORDESCRIPTION;

/// Define the structure of DMS master color display
typedef struct
{
    MMSDK_U32 u32Version;                         /// ST_DMS_MASTER_COLOR_DISPLAY version
    MMSDK_U32 u32Length;                         /// sizeof(ST_DMS_MASTER_COLOR_DISPLAY)
    //mastering color display: color volumne of a display
    MMSDK_U32 u32MaxLuminance;
    MMSDK_U32 u32MinLuminance;
    MMSDK_U16 u16DisplayPrimaries[3][2];
    MMSDK_U16 u16WhitePoint[2];
} ST_MMSDK_DMS_MASTERCOLORDISPLAY;

/// Define the structure of DMS dolby hdr info
typedef struct
 {
     MMSDK_U32 u32Version;                         /// ST_DMS_DOLBYHDRINFO version
     MMSDK_U32 u32Length;                         /// sizeof(ST_DMS_DOLBYHDRINFO)
     // bit[0:1] 0: Disable 1:Single layer 2: Dual layer, bit[2] 0:Base Layer 1:Enhance Layer
     MMSDK_U8  u8DVMode;
     MMSDK_PHY phyHDRMetadataAddr;
     MMSDK_U32 u32HDRMetadataSize;
     MMSDK_PHY phyHDRRegAddr;
     MMSDK_U32 u32HDRRegSize;
     MMSDK_PHY phyHDRLutAddr;
     MMSDK_U32 u32HDRLutSize;
     MMSDK_U8  u8DMEnable;
     MMSDK_U8  u8CompEnable;
     MMSDK_U8  u8CurrentIndex;
 } ST_MMSDK_DMS_DOLBYHDRINFO;

/// Define the structure of DMS HDR frame info
typedef struct
{
    MMSDK_U32 u32Version;                         /// ST_DMS_HDR_FRAME_INFO version
    MMSDK_U32 u32Length;                         /// sizeof(ST_DMS_HDR_FRAME_INFO)

    /// bit[0]: MS_ColorDescription present or valid, bit[1]: MS_MasterColorDisplay present or valid
    MMSDK_U32 u32FrmInfoExtAvail;
    ///color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    ST_MMSDK_DMS_COLORDESCRIPTION   stColorDescription;
    /// mastering color display: color volumne of a display
    ST_MMSDK_DMS_MASTERCOLORDISPLAY stMasterColorDisplay;
    /// DMS_MasterColorDisplay stMasterColorDisplay;
    ST_MMSDK_DMS_DOLBYHDRINFO       stDolbyHDRInfo;
} ST_MMSDK_DMS_HDR_FRAME_INFO;

/// Define the structure of external DMS frame info
typedef struct
 {
     MMSDK_PHY phyLumaAddrExt[E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MAX];
     MMSDK_PHY phyChromaAddrExt[E_MMSDK_DISP_FRM_INFO_EXT_TYPE_MAX];
     MMSDK_U16 u16Width;      // the width of second frame
     MMSDK_U16 u16Height;     // the height of second frame
     MMSDK_U16 u16Pitch[2];   // the pitch of second frame
 } ST_MMSDK_DMS_DISP_FRM_INFO_EXT;

/// Define the structure of DMS window info
typedef struct
{
    MMSDK_U32 u32x;
    MMSDK_U32 u32y;
    MMSDK_U32 u32width;
    MMSDK_U32 u32height;
} ST_MMSDK_DMS_WINDOW;

/// Define the structure of DMS create window info
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;

    MMSDK_U32 u32InputSourceType;
} ST_MMSDK_DMS_CREATE_WIN_INFO;

/// Define the structure of DMS output window info
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;

    ST_MMSDK_DMS_WINDOW stCropWin;    // if no use, set to 0; if crop valid, it means output window is moved outside of the outputlayer
    ST_MMSDK_DMS_WINDOW stOutputWin;
} ST_MMSDK_DMS_OUTPUT_WIN_INFO;

/// Define the information for DMS to set XC window
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;

    //-------------
    // Input
    //-------------
    MMSDK_U32 u32InputSourceType;      ///<Input source

    //-------------
    // Window
    //-------------
    ST_MMSDK_DMS_WINDOW stCapWin;        ///<Capture window
    ST_MMSDK_DMS_WINDOW stDispWin;       ///<Display window
    ST_MMSDK_DMS_WINDOW stCropWin;       ///<Crop window

    //-------------
    // Timing
    //-------------
    MMSDK_U32  u32Interlace;             ///<Interlaced or Progressive
    MMSDK_U32  u32HDuplicate;            ///<flag for vop horizontal duplicate, for MVD, YPbPr, indicate input double sampled or not
    MMSDK_U32  u32InputVFreq;          ///<Input V Frequency, VFreqx10, for calculate output panel timing
    MMSDK_U32  u32InputVTotal;         ///<Input Vertical total, for calculate output panel timing
    MMSDK_U32  u32DefaultHtotal;       ///<Default Htotal for VGA/YPbPr input
    MMSDK_U32  u32DefaultPhase;            ///<Default Phase for VGA/YPbPr input

    //-------------------------
    // customized post scaling
    //-------------------------
    MMSDK_U32  u32HCusScaling;               ///<assign post H customized scaling instead of using XC scaling
    MMSDK_U32  u32HCusScalingSrc;          ///<post H customized scaling src width
    MMSDK_U32  u32HCusScalingDst;          ///<post H customized scaling dst width
    MMSDK_U32  u32VCusScaling;               ///<assign post V manuel scaling instead of using XC scaling
    MMSDK_U32  u32VCusScalingSrc;          ///<post V customized scaling src height
    MMSDK_U32  u32VCusScalingDst;          ///<post V customized scaling dst height

    //--------------
    // 9 lattice
    //--------------
    MMSDK_U32  u32DisplayNineLattice;        ///<used to indicate where to display in panel and where to put in frame buffer

    //-------------------------
    // customized pre scaling
    //-------------------------
    MMSDK_U32  u32PreHCusScaling;            ///<assign pre H customized scaling instead of using XC scaling
    MMSDK_U32  u32PreHCusScalingSrc;       ///<pre H customized scaling src width
    MMSDK_U32  u32PreHCusScalingDst;       ///<pre H customized scaling dst width
    MMSDK_U32  u32PreVCusScaling;            ///<assign pre V manuel scaling instead of using XC scaling
    MMSDK_U32  u32PreVCusScalingSrc;       ///<pre V customized scaling src height
    MMSDK_U32  u32PreVCusScalingDst;       ///<pre V customized scaling dst height
} ST_MMSDK_DMS_XC_SETWIN_INFO;

/// Define the structure of set window info
typedef struct
{
    MMSDK_U32 u32Version;          // ST_DMS_SETWIN_INFO version
    MMSDK_U32 u32Length;           // sizeof(ST_DMS_SETWIN_INFO)

    MMSDK_U32 u32XCWinInfoValid;
    ST_MMSDK_DMS_XC_SETWIN_INFO stXCWinInfo;

    ST_MMSDK_DMS_OUTPUT_WIN_INFO stOutputWinInfo;

    EN_MMSDK_DMS_ASPECT_RATIO enARC;
    MMSDK_U32 u32OnOutputLayer;
} ST_MMSDK_DMS_SETWIN_INFO;

/// DMS window mute info
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;

    MMSDK_U32 u32Enable;
} ST_MMSDK_DMS_MUTE_WINDOW_INFO;


/// DMS freeze window info
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;
    MMSDK_U32 u32Enable;
} ST_MMSDK_DMS_FREEZE_WINDOW_INFO;

/// DMS color info
typedef struct
{
    MMSDK_U32 u32R;
    MMSDK_U32 u32G;
    MMSDK_U32 u32B;
    MMSDK_U32 u32A;
} ST_MMSDK_DMS_COLOR;

/// DMS mute color info
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;
    ST_MMSDK_DMS_COLOR stMuteColor;
} ST_MMSDK_DMS_MUTE_COLOR_INFO;

///DMS Zorder info define
typedef struct
{
    MMSDK_U32 u32Version;
    MMSDK_U32 u32Length;
    MMSDK_U32 u32Zorder;
} ST_MMSDK_DMS_ZORDER_INFO;

/// Define the structure of DMS display frame info
typedef struct
{
    /// DMS frame format
    ST_MMSDK_DMS_FRAMEFORMAT stFrames[2];
    /// HDR information
    ST_MMSDK_DMS_HDR_FRAME_INFO stHDRInfo;
    /// Extra of display information
    ST_MMSDK_DMS_DISP_FRM_INFO_EXT stDispFrmInfoExt;
    /// PTS of display frame
    MMSDK_U64 u64Pts;
    /// Phy addr of HTLB entries
    MMSDK_PHY phyHTLBEntriesAddr;
    /// Phy addr of vsyncbridge
    MMSDK_PHY phyVsyncBridgeAddr;
    /// Extra phy addr of vsyncbridge
    MMSDK_PHY phyVsyncBridgeExtAddr;
    /// ST_DMS_DispFrameFormat version
    MMSDK_U32 u32Version;
    /// sizeof(ST_DMS_DispFrameFormat)
    MMSDK_U32 u32Length;
    /// Display overlay id
    MMSDK_U32 u32OverlayID;
    /// Number of frame
    MMSDK_U32 u32FrameNum;
    /// Codec Type
    MMSDK_U32 u32CodecType;
    /// Display frame rate
    MMSDK_U32 u32FrameRate;
    /// Display window aspect width
    MMSDK_U32 u32AspectWidth;
    /// Display window aspect height
    MMSDK_U32 u32AspectHeight;
    /// Vdec stream version
    MMSDK_U32 u32VdecStreamVersion;
    /// Vdec stream id
    MMSDK_U32 u32VdecStreamId;
    /// Vdec stream unique id
    MMSDK_U32 u32UniqueId;
    /// Display tile mode
    MMSDK_U32 u32TileMode;
    /// DMS color format
    EN_MMSDK_DMS_COLORFORMAT enColorFormat;
    /// MIU band width
    MMSDK_U16 u16MIUBandwidth;
    /// Bit rate
    MMSDK_U16 u16Bitrate;
    /// Display win aspect rate
    MMSDK_U8 u8AspectRate;
    /// 1:Interlace mode 0: progressive mode
    MMSDK_U8 u8Interlace;
    /// Frame rate convert mode
    MMSDK_U8 u8FrcMode;
    /// 3D mode
    MMSDK_U8 u83DMode;
    /// 1:Bottom field first 0:Top field first
    MMSDK_U8 u8BottomFieldFirst;
    /// XC to freeze this frame
    MMSDK_U8 u8FreezeThisFrame;
    /// Display toggle time
    MMSDK_U8 u8ToggleTime;
    /// 1:Mcu mode 0: non-mcu mode
    MMSDK_U8 u8MCUMode;
    /// Control one field mode, always top or bot when FF or FR
    MMSDK_U8 u8FieldCtrl;
    /// Application Type
    MMSDK_U8 u8ApplicationType;
    /// 3D layout from SEI, the possible value is OMX_3D_LAYOUT enum in OMX_Video.h
    MMSDK_U8 u83DLayout;
    /// Color in Xvycc mode
    MMSDK_U8 u8ColorInXVYCC;
    /// For CTS or other application, drop new frame when render too fast
    MMSDK_U8 u8LowLatencyMode;
    /// Vdec Complexity
    MMSDK_U8 u8VdecComplexity;
    /// HTLB table id
    MMSDK_U8 u8HTLBTableId;
    /// HTLB entries size
    MMSDK_U8 u8HTLBEntriesSize;
    /// Active frame code
    MMSDK_U8 u8AFD;
} ST_MMSDK_DMS_DISPFRAMEFORMAT;


/// Define callback functio prototype.
typedef MMSDK_BOOL(*pfnDisplayCallback)(void* pClass, MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo);

//-------------------------------------------------------------------------------------------------
/// Display Initialize: malloc resource return display instance item
/// @param pDisplayItem  \b OUT: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_Initialize(PT_DISPLAYITEM* pDisplayItem);

//-------------------------------------------------------------------------------------------------
/// Display Initialize: malloc resource return display instance item
/// @since version 1.04
/// @param pDisplayItem  \b OUT: display instance item
/// @param pWindow       \b IN: Specific the scalar window, main or sub
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_InitializeEx(PT_DISPLAYITEM* pDisplayItem, void* pWindow) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Display Finalize: free resource
/// @param pDisplayItem  \b IN: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_Finalize(PT_DISPLAYITEM* pDisplayItem);

//-------------------------------------------------------------------------------------------------
/// Register Callback function
/// @param DisplayItem  \b IN: display instance item
/// @param pClass       \b IN: Up-layer application class point.
/// @param pCallback    \b IN: Callback function point.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_RegisterCallback(PT_DISPLAYITEM DisplayItem, void * pClass, pfnDisplayCallback pCallback);

//-------------------------------------------------------------------------------------------------
/// Setup display output path for XC
/// @param DisplayItem   \b IN: display instance item
/// @param pDisplayInfo  \b IN: display info for XC
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_SetMode(PT_DISPLAYITEM DisplayItem, ST_MMSDK_SET_DISPLAY_INFO *pDisplayInfo);

//-------------------------------------------------------------------------------------------------
/// Setup display output path with extension setting(XC/DIP)
/// @param DisplayItem      \b IN: display instance item
/// @param pDisplayInfoExt  \b IN: display info for XC or DIP
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_SetModeEX(PT_DISPLAYITEM DisplayItem, ST_MMSDK_SET_DISPLAY_INFO_EXT *pDisplayInfoExt) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Set Window
/// @param DisplayItem   \b IN: display instance item
/// @param pstCropRect   \b IN: display window
/// @param pstDispRect   \b IN: video crop window
/// @param u32MuteTimeMS \b IN: video mute time when set window
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_SetXCWindow(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_XC_RECT *pstCropRect, const ST_MMSDK_XC_RECT * pstDispRect, const MMSDK_U32 u32MuteTimeMS);

//-------------------------------------------------------------------------------------------------
/// Close window : reset close screen/ disable 3D/ disable DS/...
/// @param DisplayItem   \b IN: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_CloseWindow(PT_DISPLAYITEM DisplayItem);

/// TODO: remove
//-------------------------------------------------------------------------------------------------
/// Set DS on off
/// @param DisplayItem   \b IN: display instance item
/// @param bEnableDS     \b IN: enable DS or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_SetDSOnOFF(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnableDS);

//-------------------------------------------------------------------------------------------------
/// Set black screen on off
/// @param DisplayItem   \b IN: display instance item
/// @param bOnOff        \b IN: black image or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_BlackScreen(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bOnOff);

//-------------------------------------------------------------------------------------------------
/// Freeze and un freeze image
/// currently using for freeze when photo show effect
/// @param DisplayItem   \b IN: display instance item
/// @param bOnOff        \b IN: freeze image or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_FreezeImage(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bOnOff);

//-------------------------------------------------------------------------------------------------
/// Get scaler info max frame num in memory
/// @param DisplayItem   \b IN: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_U16 PT_Display_GetMaxFrameNumInMem(PT_DISPLAYITEM DisplayItem);

//-------------------------------------------------------------------------------------------------
/// Get scaler info max field num in memory
/// @param DisplayItem   \b IN: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_U16 PT_Display_GetMaxFieldNumInMem(PT_DISPLAYITEM DisplayItem);

//-------------------------------------------------------------------------------------------------
/// Get penal width (current output width for STB)
/// @param DisplayItem   \b IN: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_U16 PT_Display_GetPanelWidth(PT_DISPLAYITEM DisplayItem);

//-------------------------------------------------------------------------------------------------
/// Get penal height (current output height for STB)
/// @param DisplayItem   \b IN: display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_U16 PT_Display_GetPanelHeight(PT_DISPLAYITEM DisplayItem);

//-------------------------------------------------------------------------------------------------
/// Wait v sync
/// @param DisplayItem   \b IN: display instance item
/// @param bIsInput      \b IN: wait v-sync or not
/// @param u8NumVSyncs   \b IN: numbers of v-sync to wait
/// @param u16Timeout    \b IN: timeout counter
///
/// @return N/A
//-------------------------------------------------------------------------------------------------
void PT_Display_WaitVSync(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bIsInput,MMSDK_U8 u8NumVSyncs, MMSDK_U16 u16Timeout);

//-------------------------------------------------------------------------------------------------
/// Capture frame from mvop output by dwin
/// @param DisplayItem   \b IN: display instance item
/// @param pstFrameInfo  \b IN: Video Frame info
/// @param pstCropRect   \b IN: crop rect
/// @param u32BuffAddr   \b IN: buffer address for capture useage
/// @param u32BuffSize   \b IN: buffer size for capture useage
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_CaptureMVopOutput(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO* pstFrameInfo, const ST_MMSDK_RECT* pstCropRect, MMSDK_U32 u32BuffAddr, MMSDK_U32 u32BuffSize);

//-------------------------------------------------------------------------------------------------
/// Setting 3D
/// @param DisplayItem   \b IN: display instance item
/// @param bEnable3D     \b IN: enable 3D or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_Set3DOnOff(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable3D);

//-------------------------------------------------------------------------------------------------
/// Get output frame rate, for vdec to do frame rate convert. (STB)
/// @param DisplayItem   \b IN: display instance item
///
/// @return : output frame rate
//-------------------------------------------------------------------------------------------------
MMSDK_U16 PT_Display_GetOutputFrameRate(PT_DISPLAYITEM DisplayItem);

//-------------------------------------------------------------------------------------------------
/// Get output is interlace or progressive, for vdec to do frame rate convert. (STB)
/// @param DisplayItem   \b IN: display instance item
///
/// @return TRUE: Interlace
/// @return FALSE: Progressive
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_IsOutputInterlace(PT_DISPLAYITEM DisplayItem);

//-------------------------------------------------------------------------------------------------
/// Set one field(stb)
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param bEnable       \b IN: enable/disable one field
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_SetOneField(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Send Mvop command
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pHandle       \b IN: Mvop handle(main/sub)
/// @param u32Cmd        \b IN: Mvop command(MVOP_Command)
/// @param pPara         \b IN/OUT: Mvop command parameter
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_MvopSendCommand(PT_DISPLAYITEM DisplayItem, void* pHandle, MMSDK_U32 u32Cmd, void *pPara) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Get Mvop command
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pHandle       \b IN: Mvop handle(main/sub)
/// @param u32Cmd        \b IN: Mvop command(MVOP_Command)
/// @param pPara         \b IN/OUT: Mvop command parameter
/// @param u32ParamSize  \b IN: parameter type size
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_MvopGetCommand(PT_DISPLAYITEM DisplayItem, void* pHandle, MMSDK_U32 u32Cmd, void *pPara, MMSDK_U32 u32ParamSize) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Query whether current XC is framebufferless or not
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pbFBL         \b OUT: Is FBL or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_IsCurrentFBLMode(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL *pbFBL) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Set two initial factors mode to improve quality in FBL
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param bEnable       \b IN: enable or disable two initial factors mode
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_Enable_FBL_TwoInitFactor(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Set high dynamic range info
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pHDRMetaData       \b IN: high dynamic range info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_SetHighDynamicRange(PT_DISPLAYITEM DisplayItem, ST_MMSDK_HDR_METADATA* pHDRMetaData) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Push the video frame with frame info, return until the push operation finished
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pstPushInfo   \b IN: The push frame info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_PushVideoFrame(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO_EXT *pstPushInfo) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Block function to waiting for the specific pushed frame is displayed done,
/// return until the frame is displayed.
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pstFrameInfo  \b IN: The pushed frame info to check is displayed done
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_WaitPushVideoFrameDone(PT_DISPLAYITEM DisplayItem, ST_MMSDK_VIDEO_FRAME_INFO_EXT *pstFrameInfo) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// This function provides information when timing changed
/// @since version 1.04
/// @param DisplayItem   \b IN: display instance item
/// @param pstTimingChangeInfo  \b IN: The timing change information
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_InformTimingChanged(PT_DISPLAYITEM DisplayItem, ST_MMSDK_TIMING_CHANGE_INFO *pstTimingChangeInfo) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Get openHDR and dolbyHDR informantion
/// @param DisplayItem   \b IN: display instance item
/// @param pstHDR        \b OUT: HDR information
///
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_GetHDRAttributes(PT_DISPLAYITEM DisplayItem, ST_MMSDK_DISPLAY_HDR_STATUS *pstHDR)__attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// This function provides to get xc id for register pipe
/// @param DisplayItem      \b IN: display instance item
/// @param pstPipeInfo      \b IN/Out: pipe info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_GetXCPipeId(PT_DISPLAYITEM DisplayItem, ST_MMSDK_PIPE_INFO * pstPipeInfo) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// This function provides to notify xc pipe lock or not
/// @param DisplayItem      \b IN: display instance item
/// @param pstPipeInfo      \b IN: pipe info
/// @param bEnable          \b IN: lock or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_XC_PipeLock(PT_DISPLAYITEM DisplayItem, ST_MMSDK_PIPE_INFO * pstPipeInfo, MMSDK_BOOL bEnable) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// This function provides information of enable/disable force interlace/progressive mode to mvop
/// @param DisplayItem      \b IN: display instance item
/// @param bEnable          \b IN: Enable/Disable force I/P mode
/// @param bInterlace       \b IN: Interlace mode or progressive mode
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_Display_EnableForceInterlaceMode(PT_DISPLAYITEM DisplayItem, MMSDK_BOOL bEnable, MMSDK_BOOL bInterlace) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Get DS version
/// @param DisplayItem   \b IN: display instance item
///
/// @return DS version
//-------------------------------------------------------------------------------------------------
MMSDK_U16 PT_Display_GetDSVersion(PT_DISPLAYITEM DisplayItem)__attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Set Digital Decode Signal
/// @param DisplayItem   \b IN: DMS display instance item
/// @param pstDispFrameFormat        \b IN: Display frame format
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_SetDigitalDecodeSignalInfo(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_DISPFRAMEFORMAT *pstDispFrameFormat) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// Clear Digital Decode Signal
/// @param DisplayItem   \b IN: Display instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_ClearDigitalDecodeSignalInfo(PT_DISPLAYITEM DisplayItem) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// Set DMS Win
/// @param DisplayItem   \b IN: Display instance item
/// @param pstDMS_SetWin_Info        \b IN: Display DMS info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_SetWindow(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_SETWIN_INFO *pstDMS_SetWin_Info) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// DMS display flip
/// @param DisplayItem   \b IN: Display instance item
/// @param pstDispFrameFormat        \b IN: Display frame format
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_Video_Flip(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_DISPFRAMEFORMAT* pstDispFrameFormat) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// Set DMS display Z-order
/// @param pstZorderInfo   \b IN: Z-order info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_SetZOrder(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_ZORDER_INFO *pstZorderInfo) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// Get DMS capability
/// @param DisplayItem   \b IN: Display instance item
/// @param pstZorderInfo   \b OUT: Capability info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_GetCapability(PT_DISPLAYITEM DisplayItem, EN_MMSDK_DMS_CAPABILITY *peCapability) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// Set DMS output layer
/// @param DisplayItem   \b IN: Display instance item
/// @param pstLayer   \b IN: Output layer info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_SetOutputLayer(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_WINDOW *pstLayer) __attribute__ ((weak));
//-------------------------------------------------------------------------------------------------
/// DMS Freeze screen
/// @param DisplayItem   \b IN: Display instance item
/// @param pstWindowFreeze   \b IN: Freeze window info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
EN_MMSDK_DMS_RESULT PT_Display_Video_Freeze(PT_DISPLAYITEM DisplayItem, const ST_MMSDK_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze) __attribute__ ((weak));
/// @} */ // end of porting_display

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__mmsdk_porting_display__
///*** please do not remove change list tag ***///
///***$Change: 1429000 $***///
