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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file    drvPQ.h
/// @brief  PQ interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVPQ_H_
#define _DRVPQ_H_

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifdef _MDRV_PQ_C_
#define MSIF_TAG                    {'M','S','I','F'}                   // MSIF
#define MSIF_CLASS                  {'0','0'}                           // DRV/API (DDI)
#define MSIF_CUS                    0x0000                              // MStar Common library
#define MSIF_MOD                    0x0000                              // MStar Common library
#define MSIF_CHIP                   0x0000
#define MSIF_CPU                    '0'
#define MSIF_OS                     '0'
#endif

//  library information
#define MSIF_PQ_LIB_CODE               {'P','Q','_','_'}
#define MSIF_PQ_LIBVER                 {'0','0'}
#define MSIF_PQ_BUILDNUM               {'0','7'}
#define MSIF_PQ_CHANGELIST             {'0','0','4','3','4','6','5','1'}

#define PQ_DRV_VERSION /* Character String for DRV/API version */  \
    MSIF_TAG,          /* 'MSIF'                               */  \
    MSIF_CLASS,        /* '00'                                 */  \
    MSIF_CUS,          /* 0x0000                               */  \
    MSIF_MOD,          /* 0x0000                               */  \
    MSIF_CHIP,                                                     \
    MSIF_CPU,                                                      \
    MSIF_PQ_LIB_CODE  , /* IP__                                */  \
    MSIF_PQ_LIBVER    , /* 0.0 ~ Z.Z                           */  \
    MSIF_PQ_BUILDNUM  , /* 00 ~ 99                             */  \
    MSIF_PQ_CHANGELIST, /* CL#                                 */  \
    MSIF_OS
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//----------------------------
// Return value
//----------------------------
/**
 *  The return value for PQ fuction.
 */
typedef enum
{
    /// fail
    E_PQ_RET_FAIL = 0,
    /// success
    E_PQ_RET_OK,
} PQ_ReturnValue;

typedef enum
{
    E_DRVPQ_RET_FAIL = 0,   ///<Fail
    E_DRVPQ_RET_OK,         ///<OK
} E_DRVPQ_ReturnValue;

#if 0
/**
 * PQ DEINTERLACE MODE
 */
typedef enum
{
    /// off
    PQ_DEINT_OFF=0,
    /// 2DDI BOB
    PQ_DEINT_2DDI_BOB,
    /// 2DDI AVG
    PQ_DEINT_2DDI_AVG,
    /// 3DDI History(24 bit)
    PQ_DEINT_3DDI_HISTORY,
    /// 3DDI(16 bit)
    PQ_DEINT_3DDI,
} PQ_DEINTERLACE_MODE;
#endif
/**
 * PQ 3D_NR_FUNCTION_TYPE
 */
typedef enum
{
    /// off
    PQ_3D_NR_MIN,
    /// off
    PQ_3D_NR_OFF = PQ_3D_NR_MIN,
    /// Low
    PQ_3D_NR_LOW,
    /// Middle
    PQ_3D_NR_MID,
    /// High
    PQ_3D_NR_HIGH,
    /// Auto
    PQ_3D_NR_AUTO,
    /// Default
    PQ_3D_NR_DEFAULT,
    /// The max support number of PQ 3D Noise reduction
    PQ_3D_NR_NUM,
}PQ_3D_NR_FUNCTION_TYPE;

/**
 * PQ STB_QUALITY_TYPE
 */
typedef enum
{
    /// Off
    PQ_STB_QUALITY_MIN,
    /// Off
    PQ_STB_QUALITY_OFF = PQ_STB_QUALITY_MIN,
    /// Level1
    PQ_STB_QUALITY_LEVEL1,
    /// Level2
    PQ_STB_QUALITY_LEVEL2,
    /// The max support number of PQ STB quality type
    PQ_STB_QUALITY_NUM,
}PQ_STB_QUALITY_TYPE;

/**
 * PQ 3D_NR_FUNCTION_TYPE
 */
typedef enum
{
    /// Default
    PQ_DE_FLICK_DEFAULT,
    /// Low
    PQ_DE_FLICK_LOW,
    /// Middle
    PQ_DE_FLICK_MID,
    /// High
    PQ_DE_FLICK_HIGH,

}PQ_DE_FLICKER_FUNCTION_TYPE;

/**
 * PQ MPEG_NR_FUNCTION_TYPE
 */
typedef enum
{
    /// off
    PQ_MPEG_NR_MIN,
    /// off
    PQ_MPEG_NR_OFF = PQ_MPEG_NR_MIN,
    /// Low
    PQ_MPEG_NR_LOW,
    /// Middle
    PQ_MPEG_NR_MID,
    /// High
    PQ_MPEG_NR_HIGH,
    /// Auto
    PQ_MPEG_NR_AUTO,
    /// Default
    PQ_MPEG_NR_DEFAULT,
    /// The max support number of PQ MPEG Noise reduction
    PQ_MPEG_NR_NUM,
}PQ_MPEG_NR_FUNCTION_TYPE;


typedef enum
{
    PQ_OSD_BW_ON,
    PQ_OSD_BW_NUM,
}PQ_OSD_BW_FUNCTION_TYPE;

#if 0
/**
 *  INPUT SOURCE TYPE
 */
typedef enum
{
    /// VGA
    PQ_INPUT_SOURCE_VGA,
    /// TV
    PQ_INPUT_SOURCE_TV,

    /// CVBS
    PQ_INPUT_SOURCE_CVBS,

    /// S-video
    PQ_INPUT_SOURCE_SVIDEO,

    /// Component
    PQ_INPUT_SOURCE_YPBPR,
    /// Scart
    PQ_INPUT_SOURCE_SCART,


    /// HDMI
    PQ_INPUT_SOURCE_HDMI,

    /// DTV
    PQ_INPUT_SOURCE_DTV,

    /// DVI
    PQ_INPUT_SOURCE_DVI,

    // Application source
    /// Storage
    PQ_INPUT_SOURCE_STORAGE,
    /// KTV
    PQ_INPUT_SOURCE_KTV,
    /// JPEG
    PQ_INPUT_SOURCE_JPEG,

    /// The max support number of PQ input source
    PQ_INPUT_SOURCE_NUM,
    /// None
    PQ_INPUT_SOURCE_NONE = PQ_INPUT_SOURCE_NUM,
} PQ_INPUT_SOURCE_TYPE;
#endif
/**
 *  HDMI COLOR FORMAT
 */
typedef enum
{
    /// RGB
    PQ_HDMI_COLOR_RGB,
    /// YUV422
    PQ_HDMI_COLOR_YUV_422,
    /// YUV444
    PQ_HDMI_COLOR_YUV_444,
}PQ_HDMI_COLOR_TYPE;
#if 0
/**
 *  The active Window for PQ function to takes place.
 */
typedef enum
{
    /// Main window
    PQ_MAIN_WINDOW=0,
    /// Sub window
    PQ_SUB_WINDOW=1,
    /// The max support window of PQ
    PQ_MAX_WINDOW
}PQ_WIN;

/**
 *  FOURCE COLOR FMT
 */
typedef enum
{
    /// Default
    PQ_FOURCE_COLOR_DEFAULT,
    /// RGB
    PQ_FOURCE_COLOR_RGB,
    /// YUV
    PQ_FOURCE_COLOR_YUV,
}PQ_FOURCE_COLOR_FMT;
#endif
/**
 *  VIDEO STANDARD TYPE
 */
typedef enum
{
    /// PAL BGHI
    E_PQ_VIDEOSTANDARD_PAL_BGHI        = 0x00,
    /// NTSC M
    E_PQ_VIDEOSTANDARD_NTSC_M          = 0x01,
    /// SECAM
    E_PQ_VIDEOSTANDARD_SECAM           = 0x02,
    /// NTSC 44
    E_PQ_VIDEOSTANDARD_NTSC_44         = 0x03,
    /// PAL M
    E_PQ_VIDEOSTANDARD_PAL_M           = 0x04,
    /// PAL N
    E_PQ_VIDEOSTANDARD_PAL_N           = 0x05,
    /// PAL 60
    E_PQ_VIDEOSTANDARD_PAL_60          = 0x06,
    /// NOT standard
    E_PQ_VIDEOSTANDARD_NOTSTANDARD     = 0x07,
    /// AUTO
    E_PQ_VIDEOSTANDARD_AUTO            = 0x08,
    /// The max support number of Video standard
    E_PQ_VIDEOSTANDARD_MAX
} PQ_VIDEOSTANDARD_TYPE;

typedef enum
{
    PQ_DISPLAY_ONE,
    PQ_DISPLAY_PIP,
    PQ_DISPLAY_POP,
}PQ_DISPLAY_TYPE;

/**
 *  Different PQ Types for PQ information merged to code, need to re-compile if changed PQ setting.
 */
typedef enum
{
    /// Select main window standard PQ setting for each input source/resolution
    PQ_BIN_STD_MAIN,
    /// Select sub window standard PQ setting for each input source/resolution
    PQ_BIN_STD_SUB,
    /// Select main window extension PQ setting for each input source/resolution
    PQ_BIN_EXT_MAIN,
    /// Select sub window extension PQ setting for each input source/resolution
    PQ_BIN_EXT_SUB,
    /// The max number of PQ Bin
    MAX_PQ_BIN_NUM,
}PQ_BIN_TYPE;
/**
 *  Different PQ Types for PQ information merged to binary, need to modify binary if changed PQ setting.
 */
typedef enum
{
    /// Select main window standard PQ setting for each input source/resolution
    PQ_TEXT_BIN_STD_MAIN,
    /// Select sub window standard PQ setting for each input source/resolution
    PQ_TEXT_BIN_STD_SUB,
    /// The max number of PQ Text Bin
    MAX_PQ_TEXT_BIN_NUM,
}PQ_TEXT_BIN_TYPE;

typedef enum
{
    /// PQ VD sampling is on
    PQ_VD_SAMPLING_ON,
    /// PQ VD sampling is off
    PQ_VD_SAMPLING_OFF,
    /// the max number of PQ VD sampling
    PQ_VD_SAMPLING_NUM,
}PQ_VD_SAMPLING_TYPE;


/**
 *  PQ library idetifcation information.
 */
typedef struct
{
    /// ID
    MS_U8  u8PQID;
    /// Virtual address
    void * pPQBin_AddrVirt;
    /// Physical address
    MS_PHYADDR PQBin_PhyAddr;
}PQ_Bin_Info;

/**
 *  PQ initialize information.
 */
typedef struct
{
    /// DDR2
    MS_BOOL bDDR2;
    /// DDR Frequency
    MS_U32  u32DDRFreq;
    /// Bus width
    MS_U8   u8BusWidth;
    /// Panel width
    MS_U16  u16PnlWidth;
    /// Panel Vtotal
    MS_U16  u16Pnl_vtotal;
    /// OSD Hsize
    MS_U16  u16OSD_hsize;
    /// Bin count
    MS_U8   u8PQBinCnt;
    /// Text Bin count
    MS_U8   u8PQTextBinCnt;
    /// PQ Bin informaton array
    PQ_Bin_Info  stPQBinInfo[MAX_PQ_BIN_NUM];
    /// PQ Text bin information array
    PQ_Bin_Info  stPQTextBinInfo[MAX_PQ_TEXT_BIN_NUM];
}MS_PQ_Init_Info;

/**
 *  The information to infer whether the input source is HDMI or DVI, and format.
 */
typedef struct
{
    /// is HDMI or not
    MS_BOOL bIsHDMI;
    /// HDMI color format
    PQ_HDMI_COLOR_TYPE enColorFmt;
}MS_PQ_Hdmi_Info;

/**
 *  The information of video.
 */
typedef struct
{
    /// is SCART_RGB or not
    MS_BOOL bIsSCART_RGB;
    /// Video standard type
    PQ_VIDEOSTANDARD_TYPE enVideoStandard;
    /// is VIFIN or not
    MS_BOOL bIsVIFIN;
}MS_PQ_Vd_Info;
#if 0
/**
 *  Video data information to supply when in PQ mode.
 */
typedef struct
{
    /// is FBL or not
    MS_BOOL bFBL;
    /// is interlace mode or not
    MS_BOOL bInterlace;
    /// input Horizontal size
    MS_U16  u16input_hsize;
    /// input Vertical size
    MS_U16  u16input_vsize;
    /// input Vertical total
    MS_U16  u16input_vtotal;
    /// input Vertical frequency
    MS_U16  u16input_vfreq;
    /// output Vertical frequency
    MS_U16  u16ouput_vfreq;
    /// Display Horizontal size
    MS_U16  u16display_hsize;
    /// Display Vertical size
    MS_U16  u16display_vsize;
}MS_PQ_Mode_Info;
#endif
/**
 *  The media type. Photo or Movie
 */
typedef enum
{
    /// Photo
    E_PQ_MULTIMEDIA_PHOTO,
    /// Movie
    E_PQ_MULTIMEDIA_MOVIE,
}MS_PQ_MULTIMEDIA_TYPE;
/**
 *  MultiMedia information.
 */
typedef struct
{
    /// PQ MultiMedia information
    MS_PQ_MULTIMEDIA_TYPE eType;
}MS_PQ_MuliMedia_Info;


/**
 *  Supported DTV TYPE
 */
typedef enum
{
    /// H264
    E_PQ_DTV_H264,
    /// MPEG2
    E_PQ_DTV_MPEG2,
    /// IFrame
    E_PQ_DTV_IFRAME,
}MS_PQ_DTV_TYPE;


typedef enum
{
    E_PQ_ADC_SAMPLING_NONE,
    E_PQ_ADC_SAMPLING_X_1,
    E_PQ_ADC_SAMPLING_X_2,
    E_PQ_ADC_SAMPLING_MAX,
}MS_PQ_ADC_SAMPLING_TYPE;
/**
 *  The Detail information of supported DTV
 */
typedef struct
{
    /// PQ DTV information
    MS_PQ_DTV_TYPE eType;
}MS_PQ_Dtv_Info;

#define PQ_DBG_PQTAB        (0x0001) ///< Debug PQ Table
#define PQ_DBG_SRULE        (0x0002)///< Debug S RULE
#define PQ_DBG_CSCRULE      (0x0004)///< Debug CSC RULE
#define PQ_DBG_GRULE        (0x0008)///< Debug G RULE
#define PQ_DBG_BW           (0x0010)///< Debug BANDWIDTH
#define PQ_DBG_INFO         (0x0020)///< Debug INFO

/**
*   PQ Status
*/
typedef struct
{
    MS_BOOL bIsInitialized;
    MS_BOOL bIsRunning;
}MS_PQ_Status;

/**
*   PQ Information
*/
typedef struct
{
    MS_U16   u16Input_Src_Num;
    MS_U8    u8IP_Num;
}MS_PQ_INFO;


typedef struct
{
    PQ_VD_SAMPLING_TYPE eType;
    MS_U16 u16Hstart;
    MS_U16 u16Vstart;
    MS_U16 u16Hsize;
    MS_U16 u16Vsize;
    MS_U16 u16Htt;
}MS_PQ_VD_Sampling_Info;

typedef struct
{
    MS_BOOL bPIP_Supported;
    MS_BOOL b3DVideo_Supported;
}MS_PQ_CAP_INFO;

typedef struct
{
    ///RFBL mode enable/disable
    MS_BOOL bEnable;
    ///RFBL mode with/without film type
    MS_BOOL bFilm;
    ///PQ MADi type
    MS_U8 u8MADiType;
}MS_PQ_RFBL_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Get library version
/// @param  ppVersion                    \b IN: point to point to MSIF_Version
/// @return PQ_ReturnValue
//-------------------------------------------------------------------------------------------------
E_DRVPQ_ReturnValue  MDrv_PQ_GetLibVer(const MSIF_Version **ppVersion);      ///< Get version (without Mutex protect)
PQ_ReturnValue    MDrv_PQ_GetInfo(MS_PQ_INFO *pInfo);
MS_BOOL           MDrv_PQ_GetStatus(MS_PQ_Status *pStatus);
/*
DEBUG MASK:
0x01;///< Debug PQ Table
0x02;///< Debug S RULE
0x04;///< Debug CSC RULE
0x08;///< Debug G RULE
0x10;///< Debug BANDWIDTH
0x20;///< Debug INFO
*/
MS_BOOL           MDrv_PQ_SetDbgLevel(MS_U16 u16DbgSwitch);
// common PQ function
//-------------------------------------------------------------------------------------------------
/// Initial function
/// @param  pstPQInitInfo                 \b IN: point to PQ initial information
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Init(MS_PQ_Init_Info *pstPQInitInfo);

//-------------------------------------------------------------------------------------------------
/// Exit function
/// @retval Success or fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Deside source type
/// @param  eWindow                    \b IN: TBD
/// @param  enInputSourceType          \b IN: input source type
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_DesideSrcType(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType);

//-------------------------------------------------------------------------------------------------
/// Get source type
/// @param  eWindow                    \b IN: TBD
/// @return Main window or Sub window
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PQ_GetSrcType(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get input source type
/// @param  eWindow                    \b IN: TBD
/// @return Input Source Type of Main window or Sub window
//-------------------------------------------------------------------------------------------------
PQ_INPUT_SOURCE_TYPE MDrv_PQ_GetInputSourceType(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Set color range
/// @param  eWindow                     \b IN: TBD
/// @param  bColorRange0_255            \b IN: Color range
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_SetColorRange(PQ_WIN eWindow, MS_BOOL bColorRange0_255);

//-------------------------------------------------------------------------------------------------
/// Set CSC
/// @param  eWindow                          \b IN: TBD
/// @param  enFourceColor                    \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_SetCSC(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enFourceColor);


//-------------------------------------------------------------------------------------------------
/// Get Memory color format
/// @param  eWindow                          \b IN: TBD
/// @param  enFourceColor                    \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_Get_MemYUVFmt(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enFourceColor);

//-------------------------------------------------------------------------------------------------
/// Load scaling table
/// @param  eWindow                          \b IN: TBD
/// @param  eScalingType                     \b IN: TBD
/// @param  bPreV_ScalingDown                \b IN: TBD
/// @param  bInterlace                       \b IN: TBD
/// @param  bColorSpaceYUV                   \b IN: TBD
/// @param  u16InputSize                     \b IN: TBD
/// @param  u16SizeAfterScaling              \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_LoadScalingTable(PQ_WIN eWindow,
                                  MS_U8 eScalingType,
                                  MS_BOOL bPreV_ScalingDown,
                                  MS_BOOL bInterlace,
                                  MS_BOOL bColorSpaceYUV,
                                  MS_U16 u16InputSize,
                                  MS_U16 u16SizeAfterScaling);

//-------------------------------------------------------------------------------------------------
/// Set Memory format
/// @param  eWindow                          \b IN: TBD
/// @param  bMemFmt422                       \b IN: TBD
/// @param  bFBL                             \b IN: TBD
/// @param  u8BitsPerPixel                   \b IN: TBD
/// @return TBD
//-------------------------------------------------------------------------------------------------
PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat(PQ_WIN eWindow,
                                                MS_BOOL bMemFmt422,
                                                MS_BOOL bFBL,
                                                MS_U8 *u8BitsPerPixel);

//-------------------------------------------------------------------------------------------------
/// Reduce Bandwidth for OSD
/// @param  eWindow                          \b IN: TBD
/// @param  bOSD_On                          \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_ReduceBW_ForOSD(PQ_WIN eWindow, MS_BOOL bOSD_On);

//-------------------------------------------------------------------------------------------------
/// Reduce Bandwidth for PVR
/// @param  eWindow                          \b IN: TBD
/// @param  bPVR_On                          \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_ReduceBW_ForPVR(PQ_WIN eWindow, MS_BOOL bPVR_On);

//-------------------------------------------------------------------------------------------------
/// Set 420 up sampling
/// @param  eWindow                          \b IN: TBD
/// @param  bFBL                             \b IN: TBD
/// @param  bPreV_ScalingDown                \b IN: TBD
/// @param  u16V_CropStart                   \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set420upsampling(PQ_WIN eWindow, MS_BOOL bFBL, MS_BOOL bPreV_ScalingDown, MS_U16 u16V_CropStart);

//-------------------------------------------------------------------------------------------------
/// Set Film mode
/// @param  eWindow                          \b IN: TBD
/// @param  bEnable                          \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_SetFilmMode(PQ_WIN eWindow, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Load NR Table
/// @param  eWindow                          \b IN: TBD
/// @param  en3DNRType                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_LoadNRTable(PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType);

void MDrv_PQ_LoadSTBQualityTable(PQ_WIN eWindow, PQ_STB_QUALITY_TYPE enSTBQualityType);

//-------------------------------------------------------------------------------------------------
/// Load DeFlicker Table
/// @param  eWindow                          \b IN: TBD
/// @param  en3DNRType                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_LoadDeFlickerTable(PQ_WIN eWindow, PQ_DE_FLICKER_FUNCTION_TYPE enDeFlickType);

//-------------------------------------------------------------------------------------------------
/// Set non-linear scaling
/// @param  eWindow                       \b IN: TBD
/// @param  u8Level                       \b IN: TBD
/// @param  bEnable                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_SetNonLinearScaling(PQ_WIN eWindow, MS_U8 u8Level, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Check settings
/// @param  eWindow                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_CheckSettings(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Load settings
/// @param  eWindow                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_LoadSettings(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Load table
/// @param  eWindow                         \b IN: TBD
/// @param  u16TabIdx                       \b IN: TBD
/// @param  u16PQIPIdx                      \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_LoadTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx);

//-------------------------------------------------------------------------------------------------
/// Get IP number
/// @param  eWindow                         \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PQ_GetIPNum(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get current table number
/// @param  eWindow                         \b IN: TBD
/// @param  u16PQIPIdx                      \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PQ_GetTableNum(PQ_WIN eWindow, MS_U16 u16PQIPIdx);

//-------------------------------------------------------------------------------------------------
/// Get current table index
/// @param  eWindow                         \b IN: TBD
/// @param  u16PQIPIdx                      \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PQ_GetCurrentTableIndex(PQ_WIN eWindow, MS_U16 u16PQIPIdx);

//-------------------------------------------------------------------------------------------------
/// Patch fast play back
/// @param  eWindow                     \b IN: TBD
/// @param  bOn                         \b IN: TBD
/// @param  bOneField                   \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_PatchFastPlayback(PQ_WIN eWindow, MS_BOOL bOn, MS_BOOL bOneField);

//-------------------------------------------------------------------------------------------------
/// Disable film mode
/// @param  eWindow                     \b IN: Window type
/// @param  bOn                         \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_DisableFilmMode(PQ_WIN eWindow, MS_BOOL bOn);

//-------------------------------------------------------------------------------------------------
/// Get Source type name
/// @param  eWindow                         \b IN: Window type
/// @retval TBD
//-------------------------------------------------------------------------------------------------
char* MDrv_PQ_GetSrcTypeName(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get IP name
/// @param  u8PQIPIdx                         \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
char* MDrv_PQ_GetIPName(MS_U8 u8PQIPIdx);

//-------------------------------------------------------------------------------------------------
/// Get table name
/// @param  u8PQIPIdx                        \b IN: TBD
/// @param  u8TabIdx                         \b IN: TBD
/// @retval TBD
//-------------------------------------------------------------------------------------------------
char* MDrv_PQ_GetTableName(MS_U8 u8PQIPIdx, MS_U8 u8TabIdx);

//-------------------------------------------------------------------------------------------------
/// Adaptive tuning
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_AdaptiveTuning(void);

//-------------------------------------------------------------------------------------------------
/// Set DTV Info
/// @param  eWindow                        \b IN: TBD
/// @param  pstPQDTVInfo                   \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_DTVInfo(PQ_WIN eWindow, MS_PQ_Dtv_Info *pstPQDTVInfo);

//-------------------------------------------------------------------------------------------------
/// Set MultiMedia information
/// @param  eWindow                        \b IN: TBD
/// @param  pstPQMMInfo                    \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_MultiMediaInfo(PQ_WIN eWindow, MS_PQ_MuliMedia_Info *pstPQMMInfo);

//-------------------------------------------------------------------------------------------------
/// Set Video Decoder information
/// @param  eWindow                        \b IN: TBD
/// @param  pstPQVDInfo                    \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_VDInfo(PQ_WIN eWindow, MS_PQ_Vd_Info* pstPQVDInfo);

//-------------------------------------------------------------------------------------------------
/// Set mode information
/// @param  eWindow                             \b IN: TBD
/// @param  enInputSourceType                   \b IN: TBD
/// @param  pstPQModeInfo                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_ModeInfo(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo);

//-------------------------------------------------------------------------------------------------
/// Set HDMI information
/// @param  eWindow                             \b IN: TBD
/// @param  pstPQHDMIInfo                       \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_SetHDMIInfo(PQ_WIN eWindow, MS_PQ_Hdmi_Info *pstPQHDMIInfo);

//-------------------------------------------------------------------------------------------------
/// Load table by context
/// @param  eWindow                             \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_BW_LoadTableByContext(PQ_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Bandwidth load initial table
//-------------------------------------------------------------------------------------------------
void MDrv_BW_LoadInitTable(void);
void MDrv_BW_LoadInitTableByIndex(MS_U8 u8Index);

//-------------------------------------------------------------------------------------------------
/// Set HDMI information
/// @param  u16DisplayWidth                     \b IN: TBD
/// @param  enDisplaType                        \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType);

//-------------------------------------------------------------------------------------------------
/// Set force Y motion
/// @param  eWindow                             \b IN: TBD
/// @param  bEnable                             \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_MADiForceMotionY(PQ_WIN eWindow, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set force C motion
/// @param  eWindow                             \b IN: TBD
/// @param  bEnable                             \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_MADiForceMotionC(PQ_WIN eWindow, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set force Y and C motion, with customized motion data
/// @param  eWindow                             \b IN: Window Type
/// @param  bEnableY                            \b IN: Enable Force Y Motion
/// @param  u16DataY                            \b IN: Y motion 
/// @param  bEnableC                            \b IN: Enable Force C Motion
/// @param  u16DataC                            \b IN: C motion
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_MADiForceMotion(PQ_WIN eWindow, MS_BOOL bEnableY, MS_U16 u16DataY, 
                             MS_BOOL bEnableC, MS_U16 u16DataC);
//-------------------------------------------------------------------------------------------------
/// Set RFBL mode for U4 skip field mechanism
/// @param  bEnable                             \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_SetRFblMode(MS_BOOL bEnable,MS_BOOL bFilm);

//-------------------------------------------------------------------------------------------------
/// Get RFBL mode for U4 skip field mechanism
/// @param  bEnable                             \b IN: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_GetRFblMode(MS_PQ_RFBL_INFO * stInfo);

//-------------------------------------------------------------------------------------------------
/// Get VD Sampling
/// @param  eWindow                             \b IN: TBD
/// @param  enInputSourceType                   \b IN: TBD
/// @param  eStandard                           \b IN: TBD
/// @param  pInfo                               \b OUT: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Get_VDSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, PQ_VIDEOSTANDARD_TYPE eStandard, MS_PQ_VD_Sampling_Info *pInfo);


//-------------------------------------------------------------------------------------------------
/// IOCTL function
/// @param  eWindow                             \b IN: TBD
/// @param  u32Flag                             \b IN: TBD
/// @param  pBuf                                \b OUT: TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_IOCTL(PQ_WIN eWindow, MS_U32 u32Flag, void *pBuf, MS_U32 u32BufSize);


//-------------------------------------------------------------------------------------------------
/// Set MLoad enable/disable
/// @param  eWindow                             \b IN: TBD
/// @param  bEn                                 \b IN: TBD
/// @param  pBuf                                \b OUT: TBD
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_MLoadEn(PQ_WIN eWindow, MS_BOOL bEn);


//-------------------------------------------------------------------------------------------------
/// Get ADC Sampling
/// @param  eWindow                             \b IN: TBD
/// @param  enInputSourceType                   \b IN: TBD
/// @param  pstPQModeInfo                       \b IN: TBD
/// @param  MS_PQ_ADC_SAMPLING_TYPE             \b OUT: TBD
//-------------------------------------------------------------------------------------------------
MS_PQ_ADC_SAMPLING_TYPE MDrv_PQ_Get_ADCSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo);

//-------------------------------------------------------------------------------------------------
/// set the size of scaler buffer
/// @param  eWindow                             \b IN: TBD
/// @param  u32BufSize                          \b IN: TBD
/// @param  None
//-------------------------------------------------------------------------------------------------
void MDrv_PQ_Set_SCBufSize(PQ_WIN eWindow, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
/// set 3D Video mode
/// @param  bEn                                \b IN: TBD
/// @param  BOOL                               \b OUT: TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_Set3D_OnOff(MS_BOOL bEn);


//-------------------------------------------------------------------------------------------------
/// get the capibility of PQ
/// @param  pInfo                              \b IN: TBD
/// @param  BOOL                               \b OUT: TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_GetCaps(MS_PQ_CAP_INFO *pInfo);

//-------------------------------------------------------------------------------------------------
/// get the if 420 up-sampling is controlled by QMAP
/// @param  pInfo                              \b IN: TBD
/// @param  BOOL                               \b OUT: TBD
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_Get420UpSampling_OnOff(void);


//-------------------------------------------------------------------------------------------------
/// Set Power State
/// @param  enPowerState                          \b IN: Power state
/// @return status
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PQ_SetPowerState(EN_POWER_MODE enPowerState);


#ifdef __cplusplus
}
#endif

#endif /* _DRVPQ_H_ */

