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

/// Define the push frame format information
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
}ST_MMSDK_VIDEO_FRAME_FORMAT;

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
    /// Frame format information
    ST_MMSDK_VIDEO_FRAME_FORMAT astFrameFormat[2];
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

/// @} */ // end of porting_display

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__mmsdk_porting_display__
///*** please do not remove change list tag ***///
///***$Change: 1429000 $***///
