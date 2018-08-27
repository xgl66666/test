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
/// file   msAPI_XC.h
/// @brief  msAPI_XC Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MSAPI_EDID_H_
#define _MSAPI_EDID_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSAPI_XC_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#define MS_FLOAT MS_U32
#endif
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSAPI_HDMI_RX_EXTEND_PACKET_RECEIVE_VERSION          4

//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_800x600_60HZ,  //E_MSAPI_HDMITX_ESTABLISHED_TIMINGS_I
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_800x600_56HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_640x480_75HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_640x480_72HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_640x480_67HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_640x480_60HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_720x400_88HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_720x400_70HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_1280x1024_75HZ,  //E_MSAPI_HDMITX_ESTABLISHED_TIMINGS_II
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_1024x768_75HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_1024x768_70HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_1024x768_60HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_1024x768_87HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_832x624_75HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_800x600_75HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_800x600_72HZ,
    E_MSAPI_HDMITX_ESTABLISHED_TIMING_1152x870_75HZ    //E_MSAPI_HDMITX_MANUFACTURER_TIMINGS
} E_MSAPI_HDMITX_ESTABLISHED_TIMINGS;

typedef enum
{
    E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_16_10,
    E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_4_3,
    E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_5_4,
    E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_16_9
} E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO;

typedef enum
{
    E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_AUDIO = 1,
    E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_VIDEO,
    E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_VENDER,
    E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_SPEAKER,
    E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_VESA_DISPLAY,
    E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_EXTEND = 7
} E_MSAPI_HDMITX_CEA_DATA_BLOCK_TAG_CODE;

typedef enum
{
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_LPCM,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_AC3,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG1,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MP3,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG2,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_AACLC,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_DTS,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_ATRAC,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_OneBitAudio,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_EnhancedAC3,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_DTSHD,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MAT,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_DST,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_WMAPRO,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4HE_AAC,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4HE_AACv2,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4AAC_LC,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_DRA,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4HE_AAC_MPEG_SURROUND,
    E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4_AACLC_MPEG_SURROUND
} E_MSAPI_HDMI_AUDIO_CODING_TYPE;

typedef enum
{
    E_MSAPI_HDMI_CE_NOT_SUPPORT,
    E_MSAPI_HDMI_CE_ALWAYS_OVERSCANNED,
    E_MSAPI_HDMI_CE_ALWAYS_UNDERSCANNED,
    E_MSAPI_HDMI_CE_SUPPORTS_OVER_AND_UNDERSCAN,
}E_MSAPI_HDMI_CE_TYPE;

typedef enum
{
    E_MSAPI_HDMI_IT_NOT_SUPPORT,
    E_MSAPI_HDMI_IT_ALWAYS_OVERSCANNED,
    E_MSAPI_HDMI_IT_ALWAYS_UNDERSCANNED,
    E_MSAPI_HDMI_IT_SUPPORTS_OVER_AND_UNDERSCAN,
}E_MSAPI_HDMI_IT_TYPE;

typedef enum
{
    E_MSAPI_HDMI_PT_NOT_SUPPORT,
    E_MSAPI_HDMI_PT_ALWAYS_OVERSCANNED,
    E_MSAPI_HDMI_PT_ALWAYS_UNDERSCANNED,
    E_MSAPI_HDMI_PT_SUPPORTS_OVER_AND_UNDERSCAN,
}E_MSAPI_HDMI_PT_TYPE;

typedef enum
{
    E_MSAPI_HDMI_RX_SWITCH_VSDB_TO_HDR_PACKET, // XC resource
    E_MSAPI_HDMI_RX_HDR_INFOFRAME_GET,
    E_MSAPI_HDMI_RX_EXTEND_PACKET_RECEIVE_GET,

    E_MSAPI_HDMI_RX_INTERFACE_CMD_GET_HDCP_STATE = 0x500, // HDMIRX resource
    /*************************** HDCP Repeater ***************************/
    E_MSAPI_HDMI_RX_INTERFACE_CMD_WRITE_X74,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_READ_X74,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_SET_REPEATER,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_SET_BSTATUS,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_SET_HDMI_MODE,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_GET_INTERRUPT_STATUS,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_WRITE_KSV_LIST,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_SET_VPRIME,
    /*************************** HDCP Repeater ***************************/
    E_MSAPI_HDMI_RX_INTERFACE_CMD_GET_HDCP_ENC_STATE,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_HDR_INFOFRAME_GET,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_EXTEND_PACKET_RECEIVE_GET,
    E_MSAPI_HDMI_RX_INTERFACE_CMD_DATA_RTERM_CONTROL,
} E_MSAPI_HDMI_RX_CTRL_ID;

typedef enum
{
    E_MSAPI_HDMI_POWER_SUSPEND     = 1,    // State is backup in DRAM, components power off.
    E_MSAPI_HDMI_POWER_RESUME      = 2,    // Resume from Suspend or Hibernate mode
    E_MSAPI_HDMI_POWER_MECHANICAL  = 3,    // Full power off mode. System return to working state only after full reboot
    E_MSAPI_HDMI_POWER_SOFT_OFF    = 4,    // The system appears to be off, but some components remain powered for trigging boot-up.
} E_MSAPI_HDMI_POWER_MODE;

typedef enum
{
    E_MSAPI_HDMI_HDCP_WARM_BOOT  = 0,    // warm boot, means optee support STR
    E_MSAPI_HDMI_HDCP_COLD_BOOT  = 1,    // cold boot, means optee not support STR
} E_MSAPI_HDMI_HDCP_STR_TYPE;

typedef enum
{
    E_MSAPI_HDMITX_DOVI_OUTPUT_DEFAULT = 0x00000000,
    E_MSAPI_HDMITX_DOVI_OUTPUT_SDR = 0x00000001,
    E_MSAPI_HDMITX_DOVI_OUTPUT_HDR10 = 0x00000002,
    E_MSAPI_HDMITX_DOVI_OUTPUT_DOLBY_VISION = 0x00000004,
} E_MSAPI_HDMITX_DOLBY_VISION_OUTPUT_FORMAT;

//-------------------------------------------------------------------------------------------------
//  Structure Types
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_BOOL bSupport3840x2160_30Hz;
    MS_BOOL bSupport3840x2160_25Hz;
    MS_BOOL bSupport3840x2160_24Hz;
    MS_BOOL bSupport4096x2160_24Hz;
} MSAPI_XC_HDMITX_4K_SUPPORT_INFO;

typedef struct
{
    MS_BOOL bSidBySide;
    MS_BOOL bFramePacking;
    MS_BOOL bTopAndBottom;
} MSAPI_XC_HDMITX_3D_SUPPORT_TYPE;

typedef struct
{
    MS_BOOL bSDRLuminanceRange;
    MS_BOOL bHDRLuminanceRange;
    MS_BOOL bEOTFInSMPTEST2084;
    MS_BOOL bFutureEOTF;
    MS_BOOL bStaticMetadataType1;
} MSAPI_XC_HDMITX_SUPPORT_HDR;

typedef struct
{
    MS_U16 u16HorizontalActivePixels;
    MS_U16 u16VerticalActivePixels;
    E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO enAspectRatio;
    MS_U8 u8RefreshRate;
} MSAPI_XC_HDMITX_STANDARD_TIMING_INFO;

typedef struct
{
    MS_U8 u8MaxSupportedVertRates;
    MS_U8 u8MinSupportedVertRates;
    MS_U8 u8MaxSupportedHorRates;
    MS_U8 u8MinSupportedHorRates;
    MS_U8 u8MaxPixelClock;
    MS_U8 u8NumOfStandardTiming;
    MS_BOOL bSecondaryTimingFormula;
    MS_U16 u16SecondaryFreq;
    MS_U16 u16GTFParaM;
    MS_U8 u8GTFParaC;
    MS_U8 u8GTFParaK;
    MS_U8 u8GTFParaJ;
}MSAPI_XC_HDMITX_MONITOR_RANGE_LIMITS;

typedef struct
{
    MS_U8 u8TypeOf3D;
    MS_U8 u8VICof2D;
} MSAPI_XC_HDMITX_DETAILED_3D_DECLARATIONS;

typedef struct
{
    MS_FLOAT Pixelclock;
    MS_U16 u16HorzActive;
    MS_U16 u16VertActive;
    MS_U16 u16HorzBlank;
    MS_U16 u16VertBlank;
    MS_U16 u16HSyncOffset;
    MS_U16 u16HSyncPulseWidth;
    MS_U8 u8VSyncOffset;
    MS_U8 u8VSyncPulseWidth;
    MS_U16 u16HImageSize;
    MS_U16 u16VImageSize;
    MS_U8 u8HorizontalBorder;
    MS_U8 u8VerticalBorder;
    MS_BOOL bInterlace;
    MS_BOOL  bStereoMode;
    MS_U8 u8DecodeOfStereoMode;
    MS_U8 u8SyncSignalDescription;
} MSAPI_XC_HDMITX_DESCRIPTION_BLOCK_DETAILED_TIMING;

typedef struct
{
    MS_U8 u8DataTag;
    MS_U8 u8MonitorSerialNumber[14];
    MS_U8 u8ASCIIDataString[14];
    MS_U8 u8MonitorName[14];
    MSAPI_XC_HDMITX_STANDARD_TIMING_INFO stStandardTiming[6];
    MS_U8 u8NumofStandardTiming;
    MSAPI_XC_HDMITX_MONITOR_RANGE_LIMITS stMonitorRange;
}MSAPI_XC_HDMITX_MONITOR_DESCRIPTION_BLOCK;

typedef struct
{
    E_MSAPI_HDMI_AUDIO_CODING_TYPE eAudioCodingType;
    MS_U8 u8MaxChannel;
    MS_BOOL bSupportLPCM24bit;
    MS_BOOL bSupportLPCM20bit;
    MS_BOOL bSupportLPCM16bit;
    MS_U16 u16MaxBitRate;
    MS_BOOL bSupport192kHz;
    MS_BOOL bSupport176d4kHz;
    MS_BOOL bSupport96kHz;
    MS_BOOL bSupport88d2kHz;
    MS_BOOL bSupport48kHz;
    MS_BOOL bSupport44d1kHz;
    MS_BOOL bSupport32kHz;
}MSAPI_XC_HDMITX_SHORT_AUDIO_DESCRIPTION;

typedef struct
{
    MS_U8 u8RegistrationID[3];
    MS_U8 u8Phyaddr[2];
    MS_BOOL bIsSupportIA;
    MS_BOOL bSupportedYCbCr444;
    MS_BOOL bSupported30bitsDeepColor;
    MS_BOOL bSupported36bitsDeepColor;
    MS_BOOL bSupported48bitsDeepColor;
    MS_BOOL bDVIDualLink;
    MS_U16 u16MaxTMDSclock;
    MS_BOOL bGraphicsContentTypes;
    MS_BOOL bPhotoContentTypes;
    MS_BOOL bCinemaContentTypes;
    MS_BOOL bGameContentTypes;
    MS_BOOL bLatency;
    MS_BOOL bInterlacedLatency;
    MS_BOOL bHDMIVideoPresent;
    MS_BOOL bBasic3D;
    MS_U8 u8Multi3D;
    MS_U8 u8ImageSize;
    MS_U8 u8LenOfVIC;
    MS_U8 u8LenOf3D;
    MSAPI_XC_HDMITX_4K_SUPPORT_INFO st4KSupport;
    MSAPI_XC_HDMITX_3D_SUPPORT_TYPE st3DSupport;
    MS_U8 u8SVDof3D[16];
    MS_U8 u8LenOf3DSVD;
    MSAPI_XC_HDMITX_DETAILED_3D_DECLARATIONS stDetail3D[10];
    MS_U8 u8LenOf3DDetail;
} MSAPI_XC_HDMITX_HDMI_VENDOR_SPECIFIC_DATA;

typedef struct
{
    MS_U8 u8RegistrationID[3];
    MS_U8 u8Version;
    MS_U16 u16MaxTMDSCharacterRate;
    MS_BOOL bOSDDisparityOf3D;
    MS_BOOL bDualView;
    MS_BOOL bIndependentView;
    MS_BOOL bLTE340MHzScramble;
    MS_BOOL bRRCapable;
    MS_BOOL bSCDCPresent;
    MS_BOOL bDC48bit420;
    MS_BOOL bDC36bit420;
    MS_BOOL bDC30bit420;
} MSAPI_XC_HDMITX_HDMI20_VENDOR_SPECIFIC_DATA;

typedef struct
{
    MS_BOOL bSupportFLandFR;
    MS_BOOL bSupportLFE;
    MS_BOOL bSupportFC;
    MS_BOOL bSupportRLandRR;
    MS_BOOL bSupportRC;
    MS_BOOL bSupportFLCandFRC;
    MS_BOOL bSupportFRLCandRRC;
    MS_BOOL bSupportFLWandFRW;
    MS_BOOL bSupportFLHandFRH;
    MS_BOOL bSupportTC;
    MS_BOOL bSupportFCH;
}MSAPI_XC_HDMITX_SPEAKER_ALLOCATION_DATA_BLOCK;

typedef struct
{
    MS_U8 u8Tag;
    MS_U8 u8SupportY420[32];
    MS_U8 u8NumofSupportY420;
    MS_U8 u8Y420VideoDataBlock[32];
    MS_U8 u8NumOfY420VideoDataBlock;
    E_MSAPI_HDMI_CE_TYPE enCE;
    E_MSAPI_HDMI_IT_TYPE enIT;
    E_MSAPI_HDMI_PT_TYPE enPT;
    MS_BOOL bRGBQuantization;
    MS_BOOL bYCCQuantization;
    MS_BOOL bBT2020RGB;
    MS_BOOL bBT2020YCC;
    MS_BOOL bBT2020cYCC;
    MS_BOOL bAdobeRGB;
    MS_BOOL bAdobeYCC601;
    MS_BOOL bsYCC601;
    MS_BOOL bxvYCC709;
    MS_BOOL bxvYCC601;
    MSAPI_XC_HDMITX_SUPPORT_HDR stHDRStaticMetadataDataBlock;
}MSAPI_XC_HDMITX_EXTEND_CODE;

typedef struct
{
    MS_BOOL bDigital;
    MS_BOOL bVESADFPCompatible;
    MS_U8 u8SignalLevel;
    MS_BOOL bSetupSignalLevel;
    MS_BOOL bSeparateSyncs;
    MS_BOOL bCompositeSync;
    MS_BOOL bSyncOnGreenVideo;
    MS_BOOL bSerrationOfTheVsync;
} MSAPI_XC_HDMITX_VIDEO_INPUT_DEFINITION;

typedef struct
{
    MS_BOOL bBasedOnGTF;
    MS_BOOL bPreferredTiming;
    MS_BOOL bDefaultsRGB;
    MS_U8 u8DisplayType;
    MS_BOOL bActiveOff;
    MS_BOOL bSuspend;
    MS_BOOL bStandby;

} MSAPI_XC_HDMITX_FEATURE_SUPPORT;

typedef struct
{
    MS_U8 u8ManufacturerName[3];
    MS_U16 u16ProductCode;
    MS_U32 u32SerialNumber;
    MS_U8 u8WeekofManufacture;
    MS_U32 u32YearofManufacture;
    MS_U8 u8VersionOfEDID;
    MS_U8 u8RevisionOfEDID;
    MS_U8 u8ExtensionBlockNumber;
    MSAPI_XC_HDMITX_VIDEO_INPUT_DEFINITION stVideoInputDefinition;
    MSAPI_XC_HDMITX_FEATURE_SUPPORT stFeatureSupport;
    MS_U8 u8HorizontalImageSize;
    MS_U8 u8VerticalImageSize;
    MS_U16 u8DisplayTransferCharacteristic;
    MS_U8 u8EstablishedTiming[17];      //MAX==17
    MS_U8 u8NumOfEstablishedTiming;
    MSAPI_XC_HDMITX_STANDARD_TIMING_INFO stStandardTiming[8];
    MS_U8 u8NumofStandardTiming;
    MSAPI_XC_HDMITX_DESCRIPTION_BLOCK_DETAILED_TIMING stDTD[4];
    MSAPI_XC_HDMITX_MONITOR_DESCRIPTION_BLOCK stMonitorDB[4];
    MS_U8 u8NumOfDTD;
    MS_U8 u8NumOfMonitorDB;
    } MSAPI_HDMITX_EDID_BLOCK0;

typedef struct
{
    MS_BOOL bCEAExtension;
    MS_U8 u8CEAExtensionVersion;
    MS_U8 u8DataBlockOffset;
    MS_BOOL bUnderscans;
    MS_BOOL bAudio;
    MS_BOOL bYCBCR444;
    MS_BOOL bYCBCR422;
    MS_U8 u8NumofNativeDTDs;
    MSAPI_XC_HDMITX_SHORT_AUDIO_DESCRIPTION u8AudioDataBlock[10];
    MS_U8 u8AudioDataLen;
    MS_U8 u8VideoDataLen;
    MS_U8 u8SVD[32];
    MS_U8 u8HDMIVendorSpecificDataLen;
    MSAPI_XC_HDMITX_HDMI_VENDOR_SPECIFIC_DATA stHDMISpecificData;
    MS_U8 u8AudioVendorSpecificDataLen;
    MSAPI_XC_HDMITX_HDMI20_VENDOR_SPECIFIC_DATA stHDMI20SpecificData;
    MSAPI_XC_HDMITX_SPEAKER_ALLOCATION_DATA_BLOCK stSpeakerDataBlock;
    MS_U8 u8SpeakerAllocationDataLen;
    MS_U8 u8TransferCharacteristicDataLen;
    MSAPI_XC_HDMITX_DESCRIPTION_BLOCK_DETAILED_TIMING stDTD[6];
    MS_U8 u8NumofDTD;
    MS_U8 u8NumofExtendCode;
    MSAPI_XC_HDMITX_EXTEND_CODE stExteandCode[10];
} MSAPI_HDMITX_EDID_BLOCK1;

typedef struct __attribute__((packed))
{
    MS_U16 u16Version;                     /// Version.
    MS_U16 u16Size;                        /// Structure size.
    MS_BOOL bPKT_MPEG_RECEIVE;       ///< MPEG packet received
    MS_BOOL bPKT_AUI_RECEIVE;         ///< AUI packet received
    MS_BOOL bPKT_SPD_RECEIVE;         ///< SPD packet received
    MS_BOOL bPKT_AVI_RECEIVE;         ///< AVI packet received
    MS_BOOL bPKT_GC_RECEIVE;         ///< GC packet received
    MS_BOOL bPKT_ASAMPLE_RECEIVE;    ///< ASAMPLE packet received
    MS_BOOL bPKT_ACR_RECEIVE;        ///< ACR packet received
    MS_BOOL bPKT_VS_RECEIVE;          ///< VS packet received
    MS_BOOL bPKT_NULL_RECEIVE;       ///< NULL packet received
    MS_BOOL bPKT_ISRC2_RECEIVE;      ///< ISRC2 packet received
    MS_BOOL bPKT_ISRC1_RECEIVE;      ///< ISRC1 packet received
    MS_BOOL bPKT_ACP_RECEIVE;         ///< ACP packet received
    MS_BOOL bPKT_ONEBIT_AUD_RECEIVE;    ///< ONE bit AUD packet received
    MS_BOOL bPKT_GM_RECEIVE;     /// <GM packet received
    MS_BOOL bPKT_HBR_RECEIVE;    /// <HBR packet received
    MS_BOOL bPKT_VBI_RECEIVE;     /// <VBIpacket received
    MS_BOOL bPKT_HDR_RECEIVE;    ///< HDR packet received
    MS_BOOL bPKT_RSV_RECEIVE;    ///< RSV packet received
    MS_BOOL bPKT_EDR_VALID;      ///< Dolby EDR packet valid
    MS_BOOL bPKT_AUDIO_DST_RECEIVE; // DST
    MS_BOOL bPKT_AUDIO_3D_ASP_RECEIVE; // 3D L-PCM
    MS_BOOL bPKT_AUDIO_3D_DSD_RECEIVE; // One bit 3D audio
    MS_BOOL bPKT_AUDIO_METADATA_RECEIVE; // Audio metadata
    MS_BOOL bPKT_AUDIO_MULTI_ASP_RECEIVE; // Multi-stream
    MS_BOOL bPKT_AUDIO_MULTI_DSD_RECEIVE; // Multi-stream one bit audio
    MS_BOOL bPKT_EM_RECEIVE;    // EM packet received
}MSAPI_HDMIRX_EXTEND_PACKET_RECEIVE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
E_MSAPI_XC_3D_INPUT_MODE msAPI_XC_HDMIRx_Get3DMode(MSAPI_XC_DEVICE_ID *pstAPI_XC_DeviceId);
E_MSAPI_XC_RESULT msAPI_XC_HDMITx_Set3DMode(MS_BOOL enable, HDMITX_VIDEO_3D_STRUCTURE e3DMode);

//RX init
E_MSAPI_XC_RESULT msAPI_XC_Init_HDMIRx(void);
E_MSAPI_XC_RESULT msAPI_XC_HDMIRx_SetEDID(MSAPI_XC_HDMIRX_EDID_INFO stHdmirxEDIDInfo);
E_MSAPI_XC_RESULT msAPI_XC_HDMIRx_ColorThroughMode(MS_BOOL bEn);

//-------------------------------------------------------------------------------------------------
// @brief:  Set HDMI ByPass On/Off
//-------------------------------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMIByPassOnOff(MS_BOOL bEnable, MS_U16 u16HdmiRxByPassPin);

//----------------------------------------------------------------------------
/// @brief: To Set HDMITx Output mode \n
/// note:  Determine HDMI/DVI HDCP mode\n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_MODE eType);

//----------------------------------------------------------------------------
/// @brief: To Set HDMITx Color Depth Type \n
/// note:  If type E_MSAPI_XC_HDMITX_CD_AUTO is set, the color depth of HDMITx is selected according to reciver's EDID \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxCD(E_MSAPI_XC_HDMITX_CD_TYPE eType);


//----------------------------------------------------------------------------
/// @brief: To Set HDMITx Color Format \n
/// @param[in] eType HDMITx output color format
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxColorFormat(E_MSAPI_XC_HDMITX_OUTPUT_COLOR eInColor, E_MSAPI_XC_HDMITX_OUTPUT_COLOR eOutColor, MS_BOOL bForceEnable);

//----------------------------------------------------------------------------
/// @brief: To Set HDMITx Output Color Range \n
/// @param[in] eInColor HDMITx input color format
/// @param[in] eOutColor HDMITx output color format
/// @param[in] eInRange HDMITx input color range
/// @param[in] eOutRange HDMITx output color range
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxColorRange(E_MSAPI_XC_HDMITX_OUTPUT_COLOR eInColor,
                                                           E_MSAPI_XC_HDMITX_OUTPUT_COLOR eOutColor,
                                                           E_MSAPI_XC_HDMITX_OUTPUT_QUANT_RANGE eInRange,
                                                           E_MSAPI_XC_HDMITX_OUTPUT_QUANT_RANGE eOutRange);

//----------------------------------------------------------------------------
/// @brief: To Set HDMITx Output Colorimetry \n
/// @param[in] eColorimetry HDMITx output Colorimetry
/// @param[in] eExtendColorimetry HDMITx output Extend Colorimetry
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxColorimetry(E_MSAPI_XC_HDMI_COLORMETRY eColorimetry,
                                                           E_MSAPI_XC_HDMI_EXT_COLORMETRY eExtendColorimetry);

//----------------------------------------------------------------------------
/// @brief: To Set HDMITx HDCP Key to driver \n
/// note:  Default Mstar encoded key is 304byte \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxHDCPKey(MS_U8* u8TxHDCPKey, MS_U16 u16TxHDCPKeySize);

//----------------------------------------------------------------------------
/// @brief: To Set HDMIRx HDCP Key to driver \n
/// note:  Default Mstar encoded key is 304byte \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMIRxHDCPKey(MS_U8* u8RxHDCPKey, MS_U16 u16RxHDCPKeySize);

//----------------------------------------------------------------------------
/// @brief: To Set HDMIRx HPD control  \n
/// note:  \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetRxHPDCtrl(E_MSAPI_XC_HDMIRX_HPD_CTRL_TYPE ctrl);

//----------------------------------------------------------------------------
/// @brief: To To Get HDMITx HDCP status, version \n
/// note:  It includes HDCP version, status, and TEE or NONTEE \n
//----------------------------------------------------------------------------
MS_BOOL msAPI_XC_GetHDMITxHDCPStatus(E_MSAPI_XC_HDMITX_HDCP_INFO eHDCPInfo, void* ptType);

//----------------------------------------------------------------------------
/// @brief: To Get HDMITx Output Information \n
/// note: It includes ColorFormat, ColorDepth, OutputMode, OutputTiming, ...etc \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_GetHDMITxOutputInfo(MSAPI_XC_HDMITX_OUTPUT_INFO *pstAPI_XC_HDMITxOutputInfo);

//----------------------------------------------------------------------------
/// @brief: To Set HDMITx HDCP2.2 Key to driver \n
/// note:  \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMITxHDCP2Key(MS_U8* u8TxHDCP2Key, MS_U16 u16TxHDCP2KeySize);

//----------------------------------------------------------------------------
/// @brief: To Set HDMIRx HDCP2.2 Key to driver \n
/// note:  \n
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_SetHDMIRxHDCP2Key(MS_U8* u8RxHDCP2Key, MS_U16 u16RxHDCP2KeySize);

//-----------------------------------------------------------------------------
/// @brief:Judge Rx Cable is OffLine(connected and stable) or not
/// @param[in] HDMI Rx Port Num
/// @param[out] Rx Cable OffLine flag
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT eHDMIRXPort, MS_BOOL *bOffLine);

//-----------------------------------------------------------------------------
/// @brief:Get RX recevie packet info
/// @param[in] HDMI Rx Packet type
/// @param[out] HDMI RX Packet info
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_HDMIRx_GetPacketInfo(E_MSAPI_XC_HDMIRX_PACKET_TYPE ePacket, void* pPara);

//----------------------------------------------------------------------------
/// @brief: HDMI Rx control function
/// @param[in] eCmd RX command enum
/// @param[in] pBuf pointer to a buffer with user's data
/// @param[in] u32BufSize size of user's data
//----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMIRx_CTRL(E_MSAPI_HDMI_RX_CTRL_ID eCmd, void *pBuf, MS_U32 u32BufSize);

//-----------------------------------------------------------------------------
/// @brief:Set HDMI Rx Hot Plug
/// @param[in] HDMI Rx Port Num
/// @param[in] Set Rx Hot Plug
/// @param[in] Driver control Rx HPD permission
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_XC_HDMIRx_SetHPD(E_MSAPI_XC_HDMIRX_PORT eHDMIRXPort, MS_BOOL bEnable, MS_BOOL bDriverControlHPD);

//-----------------------------------------------------------------------------
/// @brief:msAPI_HDMITxQuery HDMI Tx Query command.
/// @param[in] eCMD set HDMI TX query command
/// @param[in/out] pBuf1 paramenters for in/out buffer (MS_S32)
/// @param[in/out] pBuf2 paramenters for in/out buffer (MS_BOOL)
/// @note
/// If command is E_MSAPI_HDMITX_CMD_RESTIMING_SUPPORT, it will return Supported video mode (bitmask table)
/// => pBuf1 = 0x1101   ////It represents that we support Output timging index 0 and 2 and 3
///                     //0: E_MSAPI_XC_RES_720x480I_60Hz
///                     //2: E_MSAPI_XC_RES_720x576I_50Hz
///                     //3: E_MSAPI_XC_RES_720x576P_50Hz
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMITxQuery(E_MSAPI_HDMITX_QUERY_CMD eCMD,void *pBuf1,void *pBuf2);

E_MSAPI_XC_RESULT msAPI_HDMI_GetEDIDBlockInfo(MS_U8 u8EDIDIndex, void* stEDID_Block, MS_U32 u32BlockLen);

//-----------------------------------------------------------------------------
/// @brief:Set HDCP power state
/// @param[in] power state
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMI_HDCPSetPowerState(E_MSAPI_HDMI_POWER_MODE ePowerState);

//-----------------------------------------------------------------------------
/// @brief:Set HDCP STR mode
/// @param[in] str_mode power state, 0: warm boot, optee support STR 1:cold boot, optee Not support STR
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMITx_HDCPSetStrMode(E_MSAPI_HDMI_HDCP_STR_TYPE eSTRMode);

//-----------------------------------------------------------------------------
/// @brief:Set HDCP force mode enable flag
/// @param[in] bForceEnable TRUE: force mode, FALSE: auto mode
/// @param[in] eHDMITxForceMode HDMI/DVI with or without HDCP
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMITx_SetForceHDMIOutputMode(MS_BOOL bForceEnable, E_MSAPI_XC_HDMITX_OUTPUT_MODE eHDMITxForceMode);

//-----------------------------------------------------------------------------
/// @brief:Start or Stop HDCP authentication
/// @param[in] bEnableHDCP TRUE: Start HDCP, FALSE: Stop HDCP
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMITx_StartHDCPAuthentication(MS_BOOL bEnableHDCP);

//-----------------------------------------------------------------------------
/// @brief:Set Dolby Vision HDMI output control
/// @param[in] set Dolby Vision control or not
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMITx_SetDolbyVisionCtrl(MS_BOOL bSetCtrl);

//-----------------------------------------------------------------------------
/// @brief:Set Dolby Vision output format
/// @param[in] set Dolby Vision output format
//-----------------------------------------------------------------------------
E_MSAPI_XC_RESULT msAPI_HDMITx_SetDolbyVisionOutputFormat(E_MSAPI_HDMITX_DOLBY_VISION_OUTPUT_FORMAT eDoViFormat);

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif  // _MSAPI_EDID_H_
