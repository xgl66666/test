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
// Copyright (c) 2008-2012 MStar Semiconductor, Inc.
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
/// @file   apiXC_ex.h
/// @brief  XC Interface for extension
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _APIXC_CUS_H_
#define _APIXC_CUS_H_

#include "apiXC.h"
#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------
// 3D macros
//-----------------------------
#define DOUBLEHD_1080X2P_PRESCALE_GARBAGE_VSIZE (23)
#define DOUBLEHD_1080X2P_PRESCALE_VSIZE (DOUBLEHD_PRESCALE_VONEHDSIZE*2+DOUBLEHD_1080X2P_PRESCALE_GARBAGE_VSIZE)

#define DOUBLEHD_720X2P_PRESCALE_GARBAGE_VSIZE (24)
#define DOUBLEHD_720X2P_PRESCALE_VSIZE (DOUBLEHD_PRESCALE_VONEHDSIZE*2+DOUBLEHD_720X2P_PRESCALE_GARBAGE_VSIZE)

#define DOUBLEHD_1080X2I_50_VACT_SPACE1 (85) // not sure spec
#define DOUBLEHD_1080X2I_50_VACT_SPACE2 (85) // not sure spec
#define DOUBLEHD_1080X2I_50_VSIZE (DOUBLEHD_1080X2I_FRAME_VSIZE*2+DOUBLEHD_1080X2I_50_VACT_SPACE1*2+DOUBLEHD_1080X2I_50_VACT_SPACE1)

#define DOUBLEHD_1080X2P_MVC_FRAME_SIZE  (2176)

#define KOREA_3D_VSIZE (2160)
#define XC_NEW_FILL_EVENT_STABLE_MOTHOD                           1//ldl121117


//---------------------------------------
// Refrigerator macros
//---------------------------------------
#define DUMMY_REFRIGLATOR REG_SC_BK00_34_L
#define KERNEL_INIT_NUMBER 0x5
#define KERNEL_FREEZE_BIT BIT(3)
#define KERNEL_YCBCR_BIT BIT(4)

#define INITID  //MApi_XC_W2BYTEMSK(DUMMY_REFRIGLATOR, KERNEL_INIT_NUMBER, KERNEL_INIT_NUMBER);
#define ClearKernelFreezeID  MApi_XC_W2BYTEMSK(DUMMY_REFRIGLATOR, 0, KERNEL_FREEZE_BIT);
#define ClearForceRW     MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(11));\
    MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(10));\
    MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(9));\
    MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(8));\


//mdrv_mhl_stl.h
#define MHL_MSC_SCRATCHPAD_SIZE
#define MHL_CBUS_DATA_SIZE              (MHL_MSC_SCRATCHPAD_SIZE +2) // offset + maxi 16 packets
typedef enum
{
	CBUS_REQ_IDLE       = 0,
	CBUS_REQ_PENDING,           // Request is waiting to be sent
	CBUS_REQ_SENT,              // Request has been sent
	CBUS_REQ_RECEIVED,          // Request data has been received
} CbusReqState_T;
typedef struct
{
	MS_U8 msgData[MHL_CBUS_DATA_SIZE +1];    // Cbus message data
	MS_U8 cmd;    // MSC command
	MS_U8 len;    // message length
	MS_U8 rcState;    // received state
	CbusReqState_T reqStatus;    // Cbus request status
} CbusReq_S;


////////////////////////////////////////////////////
//         Specific enums
////////////////////////////////////////////////////
typedef enum
{
    E_XC_3D_EX_OUTPUT_1920X2160,
    E_XC_3D_EX_OUTPUT_KOREA_SIDE_BY_SIDE,
    #ifdef KR3DINTERLACE
    E_XC_3D_EX_OUTPUT_1920X2160_KR3D,
    #endif
    E_XC_3D_EX_OUTPUT_1280X1440,
    E_XC_3D_EX_OUTPUT_OSD_1920X2160,
    E_XC_3D_EX_OUTPUT_MAX,
} E_XC_3D_EX_OUTPUT_SPECIFIC_TIMING;
#if 1
typedef enum
{
    E_DSLOAD_UNSUPPORTED                = 0, // DS is not supported by the driver or hardware
    E_DSLOAD_DISABLED                   = 1, // DS mode disabled
    E_DSLOAD_ENABLED_SWMODE_LOADING     = 2, // DS Software mode enabled, and it is in loading-command-process (Loading and not-Loading process is triggered by driver)
    E_DSLOAD_ENABLED_SWMODE_NOT_LOADING = 3, // DS Software mode enabled, but it is not in loading-command-process  (Loading and not-Loading process is triggered by driver)
    E_DSLOAD_ENABLED_HWMODE             = 4, // DS Hardware mode
} DSLOAD_TYPE;
#endif
// Custom 3D Enum
typedef enum
{
    MAIN_SOURCE_CHANNEL,
    PIP_SOURCE_CHANNEL,
    MAX_CHANNEL,
} SOURCE_CHANNEL_TYPE;

typedef enum
{
    FREQUENCY_48HZ,
    FREQUENCY_50HZ,
    FREQUENCY_60HZ,
    FREQUENCY_MAX,
} FREQUENCY_TO_PANEL;

typedef enum
{
    MAIN_PIP_OUTPUT_SYNC,
    MAIN_PIP_OUTPUT_ASYNC,
    MAIN_PIP_OUTPUT_MAX,
} MAIN_PIP_OUTPUT_TYPE;

typedef enum
{
    E_CUS3D_NO,
    E_CUS3D_NORMAL,
    E_CUS3D_OUTPUTASYNC,
    E_CUS3D_DEINTERLACESIGNAL,
    E_CUS3D_DEINTERLACESIGNAL_VER,
    E_CUS3D_MAX,
} CUS3DMODE;

typedef enum
{
        MSD_EXTIN0,
        MSD_EXTIN1,
        MSD_EXTIN2,
        MSD_EXTIN3,
        MSD_EXTIN4,
        MSD_EXTIN5,
        MSD_EXTIN_HDMI0,
        MSD_EXTIN_HDMI1,
        MSD_EXTIN_MAX,
} MSdExtInID_k;

#if 1 //NEED_IMPLEMENT_PQL_FUNCTIONS
#define GM_GBD_MAX_SIZE                     21

// packet Colorimetry
typedef enum
{
    HDMI_COLORIMETRY_NONE,
    HDMI_COLORIMETRY_ITU601,
    HDMI_COLORIMETRY_ITU709,
    HDMI_COLORIMETRY_EXTEND,
} SC_HDMI_COLORIMETRY_FORMAT_e;

// packet Extended Colorimetry
typedef enum
{
    HDMI_EXTENDEDCOLORIMETRY_XVYCC601,
    HDMI_EXTENDEDCOLORIMETRY_XVYCC709,
    HDMI_EXTENDEDCOLORIMETRY_RESERVED,
} SC_HDMI_EXTENDEDCOLORIMETRY_FORMAT_e;

// IOCTL_SC_GET_HDMI_XVYCC
typedef struct
{
    SCALER_WIN srcIdx;
    SC_HDMI_COLORIMETRY_FORMAT_e eColorimetry;
    SC_HDMI_EXTENDEDCOLORIMETRY_FORMAT_e eExtColorimetry;
    MS_U8 GM_GBD[(GM_GBD_MAX_SIZE+0x1) & ~0x1];        // Currently, support up to 21 bytes GBD

} SC_GET_HDMI_XVYCC_t;
#endif


// ========================================================
// ============== Customer define function =========================
// ========================================================
typedef struct
{
	EN_XC_OP1_PATTERN ePattern;
    EN_XC_OP1_PATGEN_MODE eMode;
	MS_U16 u16R;
	MS_U16 u16G;
	MS_U16 u16B;
	MS_BOOL bEnable;
	MS_U16 u16CSC;
	MS_BOOL bCCIR;
	MS_U8 u8TestMode;
} MS_XC_CUS_PATTERN_PARA;

typedef struct
{
	MS_BOOL eWindow;
    MS_BOOL bUseYUVSpace;
	MS_U8 u8Hue;
	MS_U8 u8Saturation;
	MS_U8 u8Contrast;
} MS_XC_CUS_PQ_PARA;

typedef struct
{
	XC_SETWIN_INFO stXC_SetWin_Info;
    MS_U32 u32InitDataLen;
	SCALER_WIN eWindow;
	MS_BOOL bParam;
} MS_XC_CUS_DS_WIN_PARA;

typedef struct
{
	SCALER_WIN eWindow;
    E_MUX_INPUTPORT enInputPortType;
	INPUT_SOURCE_TYPE_t enInputSrcType;
	MS_BOOL bParam;
	MS_U16 u16Param;
	MS_U16 u16Param1;
	MS_U32 u32Param;
	void* pParam;
} MS_XC_CUS_CTRL_PARA;

typedef enum
{
    /* Please follow Naming Rule,
    ** EX:
    **      E_XC_CUS_xxxx_yyyy;
    **      xxx is the object to operate
    **      yyy is the operation you need to take, which include [SET/GET/EN/DISE/DUMP]
    */

    // ADC
    E_XC_S_ADC_CMD_START,
    E_XC_S_ADC_SCART_OVERLAY_SET,
    E_XC_S_ADC_GAIN_SET,
    E_XC_S_ADC_GAIN_GET,
    E_XC_S_ADC_OFFSET_SET,
    E_XC_S_ADC_OFFSET_GET,
    E_XC_S_ADC_CMD_END,

    // HDMI
    E_XC_S_HDMI_CMD_START,
    E_XC_S_HDMI_HDE_GET,
    E_XC_S_HDMI_HTT_GET,
    E_XC_S_HDMI_VDE_GET,
    E_XC_S_HDMI_VTT_GET,
    E_XC_S_HDMI_DDC_CHANNEL_EN,
    E_XC_S_HDMI_HPD_SET,
    E_XC_S_HDMI_SYNC_MODE_SET,
    E_XC_S_HDMI_CMD_END,

	// PCMode
    E_XC_S_PCMODE_CMD_START,
    E_XC_S_PCMODE_HRES_GET,
    E_XC_S_PCMODE_VRES_GET,
    E_XC_S_PCMODE_HTT_GET,
    E_XC_S_PCMODE_CMD_END,

    // XC
    E_XC_S_XC_CMD_START,
    E_XC_S_XC_OPEN,
    E_XC_S_XC_CLOSE,
    E_XC_S_XC_FB_SIZE_GET,
    E_XC_S_XC_FB_SIZE_SET,
    E_XC_S_XC_RW_POINT_DIFF_SET,
    E_XC_S_XC_RW_POINT_DIFF_GET,
    E_XC_S_XC_OPW_OFF_GET,
    E_XC_S_XC_OPW_OFF_SET,
    E_XC_S_XC_INTERLACE_SET,
    E_XC_S_XC_FIELD_DETECT_SET,
    E_XC_S_XC_DISP_DE_WIN_GET,
    E_XC_S_XC_FORCE_NO_AUTO_FIT_SET,
    E_XC_S_XC_IOMAP_BASE_SET,
    E_XC_S_XC_CMD_END,

	// Pattern
    E_XC_S_PATTERN_CMD_START,
    E_XC_S_PATTERN_OP1_RGB_SET,
    E_XC_S_PATTERN_OP2_RGB_SET,
    E_XC_S_PATTERN_IP1_RGB_SET,
    E_XC_S_PATTERN_OP_SET,
    E_XC_S_PATTERN_VOP_SET,
    E_XC_S_PATTERN_CMD_END,

    // DS / GST
    E_XC_S_DS_CMD_START,
    E_XC_S_DS_STATUS_GET,
    E_XC_S_DS_SCALING_WIN_SET,
    E_XC_S_DS_GST_PROCESS_SET,
    E_XC_S_DS_GST_PROCESS_GET,
    E_XC_S_DS_SEAMLESS_PLAY_SET,
    E_XC_S_DS_SEAMLESS_PLAY_GET,
    E_XC_S_DS_CAPTURE_PROCESS_SET,
    E_XC_S_DS_CAPTURE_PROCESS_GET,
    E_XC_S_DS_CMD_END,

	// PIP / Smart Zoom
    E_XC_S_PIP_CMD_START,
    E_XC_S_PIP_SMART_ZOOM_SET,
    E_XC_S_PIP_SMART_ZOOM_GET,
    E_XC_S_PIP_WINDOW_ENABLE,
    E_XC_S_PIP_CMD_END,

	// 3D
    E_XC_S_3D_CMD_START,
    E_XC_S_3D_KR3D_SET,
    E_XC_S_3D_KR3D_GET,
    E_XC_S_3D_CMD_END,

	// PVR
    E_XC_S_PVR_CMD_START,
    E_XC_S_PVR_DUAL_WIN_EN,
    E_XC_S_PVR_DUAL_WIN_GET,
    E_XC_S_PVR_CMD_END,

	// PQ
    E_XC_S_PQ_CMD_START,
    E_XC_S_PQ_NR_DEMO_GET,
    E_XC_S_PQ_HSC_SET,
    E_XC_S_PQ_BT2020_SET,
    E_XC_S_PQ_CMD_END,

    // PNL
    E_XC_S_PNL_CMD_START,
    E_XC_S_PNL_PACKER_CHECK_SET,
    E_XC_S_PNL_SKIP_INIT_FLAG_SET,
    E_XC_S_PNL_STR_EN,
    E_XC_S_PNL_PANEL_INCH_SET,
    E_XC_S_PNL_CUS_FLAG_SET,
    E_XC_S_PNL_PRE_EMPHASIS_LEVEL_SET,
    E_XC_S_PNL_VREG_LEVEL_SET,
    E_XC_S_PNL_OUT_PE_CURRENT_SET,
    E_XC_S_PNL_OUT_PE_CURRENT_PARTIAL_SET,
    E_XC_S_PNL_HSTART_GET,
    E_XC_S_PNL_VSTART_GET,
    E_XC_S_PNL_PANEL_HEIGHT_GET,
    E_XC_S_PNL_PANEL_WIDTH_GET,
    E_XC_S_PNL_CMD_END,

}E_XC_S_DISPLAY_CTRL_ID;




#ifdef __cplusplus
}
#endif
#endif
