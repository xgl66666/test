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
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#ifndef _DEMO_UI_INTERFACE_H_
#define _DEMO_UI_INTERFACE_H_ 1
/****************************************************************************/
/* local enum                                                                                                            */
/****************************************************************************/
typedef enum{
    E_UI_TX_COLORFORMAT_RGB444 =1,
    E_UI_TX_COLORFORMAT_YUV422,
    E_UI_TX_COLORFORMAT_YUV444,
    E_UI_TX_COLORFORMAT_YUV420,
}EN_INTERFACE_TX_COLORFORMAT;

typedef enum{
    E_UI_TX_COLORDEPTH_8BITS =1,
    E_UI_TX_COLORDEPTH_10BITS,
    E_UI_TX_COLORDEPTH_12BITS,
    E_UI_TX_COLORDEPTH_16BITS,
}EN_INTERFACE_TX_COLORDEPTH;

typedef enum{
    E_UI_TX_OUTPUTMODE_HDMI =1,
    E_UI_TX_OUTPUTMODE_DVI,
}EN_INTERFACE_TX_OUTPUTMODE;

typedef enum{
    E_UI_TX_OUTPUTTIMING_720X480_60I =1,
    E_UI_TX_OUTPUTTIMING_720X480_60P,
    E_UI_TX_OUTPUTTIMING_720X576_50I,
    E_UI_TX_OUTPUTTIMING_720X576_50P,
    E_UI_TX_OUTPUTTIMING_1280X720_50P,
    E_UI_TX_OUTPUTTIMING_1280X720_60P,
    E_UI_TX_OUTPUTTIMING_1920X1080_50I,
    E_UI_TX_OUTPUTTIMING_1920X1080_60I,
    E_UI_TX_OUTPUTTIMING_1920X1080_24P,
    E_UI_TX_OUTPUTTIMING_1920X1080_25P,
    E_UI_TX_OUTPUTTIMING_1920X1080_30P,
    E_UI_TX_OUTPUTTIMING_1920X1080_50P,
    E_UI_TX_OUTPUTTIMING_1920X1080_60P,
    E_UI_TX_OUTPUTTIMING_3840X2160_24P,
    E_UI_TX_OUTPUTTIMING_3840X2160_25P,
    E_UI_TX_OUTPUTTIMING_3840X2160_30P,
    E_UI_TX_OUTPUTTIMING_3840X2160_50P,
    E_UI_TX_OUTPUTTIMING_3840X2160_60P,
    E_UI_TX_OUTPUTTIMING_4096X2160_24P,
    E_UI_TX_OUTPUTTIMING_4096X2160_25P,
    E_UI_TX_OUTPUTTIMING_4096X2160_30P,
    E_UI_TX_OUTPUTTIMING_4096X2160_50P,
    E_UI_TX_OUTPUTTIMING_4096X2160_60P,
    E_UI_TX_OUTPUTTIMING_MAX,
}EN_INTERFACE_TX_OUTPUTTIMING;

typedef enum{
    E_UI_TX_ASPECTRATIO_4_3 =1,
    E_UI_TX_ASPECTRATIO_16_9,
}EN_INTERFACE_TX_ASPECTRATIO;

typedef enum{
    E_UI_TX_3DSTRUCTURE_CLOSE =1,
    E_UI_TX_3DSTRUCTURE_1920x1080p23d98Hz_24Hz_FRAME_PACKING,
    E_UI_TX_3DSTRUCTURE_1280x720p59d94Hz_60Hz_FRAME_PACKING,
    E_UI_TX_3DSTRUCTURE_1280x720p50Hz_FRAME_PACKING,
    E_UI_TX_3DSTRUCTURE_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE,
    E_UI_TX_3DSTRUCTURE_1920x1080i50Hz_SIDE_BY_SIDE,
    E_UI_TX_3DSTRUCTURE_1920x1080p23d98Hz_24Hz_TOP_BOTTOM,
    E_UI_TX_3DSTRUCTURE_1280x720p59d94Hz_60Hz_TOP_BOTTOM,
    E_UI_TX_3DSTRUCTURE_1280x720p50Hz_TOP_BOTTOM,
    E_UI_TX_3DSTRUCTURE_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE,
    E_UI_TX_3DSTRUCTURE_1280x720p50Hz_SIDE_BY_SIDE,
    E_UI_TX_3DSTRUCTURE_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE,
    E_UI_TX_3DSTRUCTURE_1920x1080p59d94Hz_60Hz_TOP_BOTTOM,
    E_UI_TX_3DSTRUCTURE_1920x1080p50Hz_TOP_BOTTOM,
    E_UI_TX_3DSTRUCTURE_1920x1080p29d97Hz_30Hz_TOP_BOTTOM,
}EN_INTERFACE_TX_3DSTRUCTURE;

typedef enum{
    E_UI_TX_AUDIO_PCM =1,
    E_UI_TX_AUDIO_NONPCM,
    E_UI_TX_AUDIO_NONPCM_DDPBypass,
}EN_INTERFACE_TX_AUDIO;

typedef enum{
    E_UI_TX_COLORMETRY_NODATA =1,
    E_UI_TX_COLORMETRY_ITU601,
    E_UI_TX_COLORMETRY_ITU709,
    E_UI_TX_COLORMETRY_EXTEND_XVYCC601 ,
    E_UI_TX_COLORMETRY_EXTEND_XVYCC709,
    E_UI_TX_COLORMETRY_EXTEND_SYCC601,
    E_UI_TX_COLORMETRY_EXTEND_ADOBEYCC601,
    E_UI_TX_COLORMETRY_EXTEND_ADOBERGB,
    E_UI_TX_COLORMETRY_EXTEND_BT2020CYCC,
    E_UI_TX_COLORMETRY_EXTEND_BT2020YCC,
    E_UI_TX_COLORMETRY_EXTEND_BT2020RGB,
}EN_INTERFACE_TX_COLORMETRY;

typedef enum{
    E_UI_TX_AVMUTE_ON =1,
    E_UI_TX_AVMUTE_OFF,
}EN_INTERFACE_TX_AVMute;

typedef enum{
    E_UI_TX_QUANTIZATIONRANGE_RGB444_LIMIT =1,
    E_UI_TX_QUANTIZATIONRANGE_YUV422_LIMIT,
    E_UI_TX_QUANTIZATIONRANGE_YUV444_LIMIT,
    E_UI_TX_QUANTIZATIONRANGE_RGB444_FULL,
    E_UI_TX_QUANTIZATIONRANGE_YUV422_FULL,
    E_UI_TX_QUANTIZATIONRANGE_YUV444_FULL,
    E_UI_TX_QUANTIZATIONRANGE_TEST_PURPLE,
    E_UI_TX_QUANTIZATIONRANGE_TEST_BLACK,
}EN_INTERFACE_TX_QUANTIZATIONRANGE;

typedef enum{
  E_UI_TX_SHOWMM_VIDEO =1,
  E_UI_TX_SHOWMM_AUDIO_32K_DD,
  E_UI_TX_SHOWMM_AUDIO_44K1_DD,
  E_UI_TX_SHOWMM_AUDIO_48K_DD,
  E_UI_TX_SHOWMM_AUDIO_44K1_DDP,
  E_UI_TX_SHOWMM_AUDIO_48K_DDP,
}EN_INTERFACE_TX_SHOWTXINFO;

typedef enum
{
    E_UI_TX_INFO_COLOR_FORMAT_DESCRIPTION = 0,
    E_UI_TX_INFO_COLOR_DEPTH_DESCRIPTION,
    E_UI_TX_INFO_OUTPUT_MODE_DESCRIPTION,
    E_UI_TX_INFO_OUTPUT_TIMING_DESCRIPTION,
    E_UI_TX_INFO_ASPECT_RATIO_DESCRIPTION,
    E_UI_TX_INFO_3D_STRUCTURE_DESCRIPTION,
    E_UI_TX_INFO_AUDIO_INFO_DESCRIPTION,
    E_UI_TX_INFO_COLORIMETRY_DESCRIPTION,
    E_UI_TX_INFO_AVMUTE_DESCRIPTION,
    E_UI_TX_INFO_QUANTIZATION_RANGE_DESCRIPTION,
}EN_INTERFACE_TX_INFO_ITEM;

typedef enum
{
    E_UI_REPEATER_INFO_AVI_PACKET            =  0,
    E_UI_REPEATER_INFO_VS_PACKET             =  1,
    E_UI_REPEATER_INFO_GC_PACKET             =  2,
    E_UI_REPEATER_INFO_AUDIO_PACKET          =  3,
}EN_INTERFACE_REPEATER_INFO_PACEKT_TYPE;

typedef enum
{
    E_UI_REPEATER_INFO_SCAN_INFO             =  0,
    E_UI_REPEATER_INFO_COLOR_FORMAT          =  1,
    E_UI_REPEATER_INFO_ASPECT_RATIO          =  2,
    E_UI_REPEATER_INFO_COLORMETRY            =  3,
    E_UI_REPEATER_INFO_EXT_COLORMETRY        =  4,
    E_UI_REPEATER_INFO_COLOR_RANGE           =  5,
    E_UI_REPEATER_INFO_VIC                   =  6,
    E_UI_REPEATER_INFO_VF_PRESENT            =  7,
    E_UI_REPEATER_INFO_3D_MODE               =  8,
    E_UI_REPEATER_INFO_4K2K_VIC              =  9,
    E_UI_REPEATER_INFO_COLOR_DEPTH           = 10,
    E_UI_REPEATER_INFO_AVMUTE                = 11,
    E_UI_REPEATER_INFO_AUDIO_CHANNEL_COUNT   = 12,
    E_UI_REPEATER_INFO_AUDIO_CODING_TYPE     = 13,
    E_UI_REPEATER_INFO_AUDIO_FREQUENCY       = 14,
}EN_INTERFACE_REPEATER_INFO_ITEM;

typedef enum
{
    E_UI_HDCP_INFO_TX_HDCPVERSION = 0,
    E_UI_HDCP_INFO_TX_HDCPSTATUS = 1,
    E_UI_HDCP_INFO_RX_HDCPSTATUS = 2,
}EN_INTERFACE_HDCP_INFO_ITEM;

typedef enum
{
    E_UI_CEC_INFO_MESSAGE1_DESCRIPTION       =  0,
    E_UI_CEC_INFO_MESSAGE1_OPERAND           =  1,
    E_UI_CEC_INFO_MESSAGE2_DESCRIPTION       =  2,
    E_UI_CEC_INFO_MESSAGE2_OPERAND           =  3,
    E_UI_CEC_INFO_MESSAGE3_DESCRIPTION       =  4,
    E_UI_CEC_INFO_MESSAGE3_OPERAND           =  5,
    E_UI_CEC_INFO_MESSAGE4_DESCRIPTION       =  6,
    E_UI_CEC_INFO_MESSAGE4_OPERAND           =  7,
    E_UI_CEC_INFO_MESSAGE5_DESCRIPTION       =  8,
    E_UI_CEC_INFO_MESSAGE5_OPERAND           =  9,
    E_UI_CEC_INFO_MESSAGE6_DESCRIPTION       = 10,
    E_UI_CEC_INFO_MESSAGE6_OPERAND           = 11,
    E_UI_CEC_INFO_MESSAGE7_DESCRIPTION       = 12,
    E_UI_CEC_INFO_MESSAGE7_OPERAND           = 13,
    E_UI_CEC_INFO_MESSAGE8_DESCRIPTION       = 14,
    E_UI_CEC_INFO_MESSAGE8_OPERAND           = 15,
    E_UI_CEC_INFO_MESSAGE9_DESCRIPTION       = 16,
    E_UI_CEC_INFO_MESSAGE9_OPERAND           = 17,
    E_UI_CEC_INFO_MESSAGE10_DESCRIPTION      = 18,
    E_UI_CEC_INFO_MESSAGE10_OPERAND          = 19,
}EN_INTERFACE_CEC_INFO_ITEM;

typedef enum
{
    E_UI_EDID_BLOCK0_INFO   =  0,
    E_UI_EDID_BLOCK1_INFO   =  1,
    E_UI_EDID_BLOCK2_INFO   =  2,
    E_UI_EDID_BLOCK3_INFO   =  3,
}EN_INTERFACE_EDID_BLOCK_ITEM;

typedef enum
{
    E_UI_EDID_INFO_BLOCK0_VERSION     = 0,
    E_UI_EDID_INFO_BLOCK0_REVISION,
    E_UI_EDID_INFO_BLOCK0_EXTENSION_BLOCK_NUMBER,
    E_UI_EDID_INFO_BLOCK0_PREFERRED_IMING_IS_NATIVE,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS1,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS2,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS3,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS4,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS5,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS6,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS7,
    E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS8,
    E_UI_EDID_INFO_BLOCK0_DTD0_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK0_DTD0_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK0_DTD1_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK0_DTD1_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK0_DTD2_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK0_DTD2_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK0_DTD3_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK0_DTD3_SCAN_TYPE,
}EN_INTERFACE_EDID_BLOCK0_INFO_ITEM;
typedef enum
{
    E_UI_EDID_INFO_BLOCK1_CEA_EXTENSION_VERSION = 0,
    E_UI_EDID_INFO_BLOCK1_SUPPORTS_UNDERSCAN,
    E_UI_EDID_INFO_BLOCK1_SUPPORTS_BASIC_AUDIO,
    E_UI_EDID_INFO_BLOCK1_SUPPORTS_YCBCR,
    E_UI_EDID_INFO_BLOCK1_NATIVE_DTDS_IN_EDID,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_FIRST_LINE,
    E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_SECOND_LINE,
    E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1,
    E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2,
    E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IEEE_ID,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_CEC_PA,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DEEP_COLOR,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_MAX_TMDS_CLOCK,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_LATENCY,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_BASIC_3D,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IMAGE_SIZE,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_4KX2K_SUPPORT,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT1,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT2,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DETAILED_3D,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_IEEE_ID_AND_VERSION,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_MAX_TMDS_CHARACTER_RATE,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_3D_OSD_DISPARITY_AND_LTE,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_VIEW,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_RR_AND_SCDC,
    E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_DC420,
    E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1,
    E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY2,
    E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF,
    E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR,
    E_UI_EDID_INFO_BLOCK1_COLORIMETRY1,
    E_UI_EDID_INFO_BLOCK1_COLORIMETRY2,
    E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1,
    E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2,
    E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1,
    E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2,
    E_UI_EDID_INFO_BLOCK1_DTD0_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK1_DTD0_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK1_DTD1_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK1_DTD1_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK1_DTD2_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK1_DTD2_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK1_DTD3_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK1_DTD3_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK1_DTD4_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK1_DTD4_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK1_DTD5_HORZ_AND_VERT_ACTIVE,
    E_UI_EDID_INFO_BLOCK1_DTD5_SCAN_TYPE,
    E_UI_EDID_INFO_BLOCK1_MAX,
}EN_INTERFACE_EDID_BLOCK1_INFO_ITEM;





/****************************************************************************/
/* Function                                                                 */
/****************************************************************************/
MS_BOOL Demo_MM_Play_Test(void);
MS_BOOL Demo_Analysis_USB_Start_Test( void);
MS_BOOL Demo_Analysis_USB_Stop_Test( void);
MS_BOOL Demo_Analysis_CPU_Start_Test(void);
MS_BOOL Demo_Analysis_CPU_Stop_Test(void);
MS_BOOL Demo_Analysis_GPU_Start_Test(void);
MS_BOOL Demo_Analysis_GPU_Stop_Test(void);
MS_BOOL Demo_PVR_test_case_1(void);
MS_BOOL Demo_PVR_test_case_2(void);
MS_BOOL Demo_PVR_test_case_3(void);
MS_BOOL Demo_VDEC_test_case_1(void);
MS_BOOL Demo_MP4_test_case_1(void);
MS_BOOL Demo_Audio_test_case_1(void);
MS_BOOL Demo_Audio_test_case_2(void);
MS_U16  Demo_HDMI_Get_DisplayInfoLineCount(void);
MS_BOOL Demo_HDMI_ColorFormat_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_ColorDepth_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_OuputMode_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_OutputTiming_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_AspectRatio_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_3DStructure_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_Audio_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_Colorimetry_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_AVMute_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_QuantizationRang_case(MS_U32 *u32Sel);
MS_BOOL Demo_HDMI_PlayVideo_case(MS_U32 *u32Sel);
MS_BOOL Demo_TX_INFO(void);
MS_U8*  Demo_TX_GetUIInfoString(MS_U16 u16Line, MS_U16 u16Pos);
MS_BOOL Demo_HDMI_REPEATER_SetTXOnlyMode(void);
MS_BOOL Demo_HDMI_REPEATER_SetConverterMode(void);
MS_BOOL Demo_HDMI_REPEATER_SetRXBypassMode(void);
MS_BOOL Demo_HDMI_REPEATER_ShowRXInfo(void);
MS_U8*  Demo_HDMI_REPEATER_GetRXInfoContext(MS_U16 u16Line, MS_U16 u16Pos);
MS_BOOL Demo_HDCP_INFO(void);
MS_BOOL Demo_HDCP_GetINFO(void);
MS_U8*  Demo_HDCP_GetUIInfoString(MS_U16 u16Line, MS_U16 u16Pos);
MS_BOOL Demo_HDCP_Enable(MS_U32 *pu32Sel);
MS_BOOL Demo_CEC_SendOneTouchPlay(void);
MS_BOOL Demo_CEC_SendStandby(void);
MS_BOOL Demo_CEC_SendGiveDevicePowerStatus(void);
MS_BOOL Demo_CEC_SendSystemAudioModeControl(MS_U32 *pu32Sel);
MS_BOOL Demo_CEC_SendUserCommand(void);
MS_BOOL Demo_CEC_ClearTxSetting(void);
MS_BOOL Demo_CEC_INFO(void);
MS_BOOL Demo_CEC_GetReceivedMessage(void);
MS_U8*  Demo_CEC_GetUIInfoString(MS_U16 u16Line, MS_U16 u16Pos);
MS_U8*  Demo_EDID_GetUIInfoString_Block0(MS_U16 u16Line, MS_U16 u16Pos);
MS_U8*  Demo_EDID_GetUIInfoString_Block1(MS_U16 u16Line, MS_U16 u16Pos);
MS_U8*  Demo_EDID_GetUIInfoString_Block2(MS_U16 u16Line, MS_U16 u16Pos);
MS_U8*  Demo_EDID_GetUIInfoString_Block3(MS_U16 u16Line, MS_U16 u16Pos);
MS_BOOL Demo_EDID_GetEDIDInfo(EN_INTERFACE_EDID_BLOCK_ITEM eEDIDBlock);
MS_BOOL Demo_EDID_INFO(void);

#endif

