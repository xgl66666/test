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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
///@file apiCC.h
///@brief  ClosedCaption User Interface, header file for release
///@author MStarSemi Inc.
//////////////////////////////////////////////////////////////////////////////


#ifndef API_CC_H
#define API_CC_H

#include "MsTypes.h"
#include "apiMsMWCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/* Define                                                               */
/************************************************************************/


/************************************************************************/
/* Data structure                                                       */
/************************************************************************/
typedef struct
{
    MS_S32 s32X_offset;
    MS_S32 s32Y_offset;
}stCaptionWindowSetting;

typedef enum
{
    CC_MODE_DTV             = 0x00,
    CC_MODE_ATV             = 0x01,

} EN_CC_SYSINFO;

/* This Enum should be followed by Menu Setting Enum */
typedef enum
{
    CC_SERVICE_OFF          = 0x00,
    CC_SERVICE_CC1          = 0x01,
    CC_SERVICE_CC2          = 0x02,
    CC_SERVICE_CC3          = 0x03,
    CC_SERVICE_CC4          = 0x04,
    CC_SERVICE_TEXT1        = 0x05,
    CC_SERVICE_TEXT2        = 0x06,
    CC_SERVICE_TEXT3        = 0x07,
    CC_SERVICE_TEXT4        = 0x08,
    CC_SERVICE_SERVICE1     = 0x09,
    CC_SERVICE_SERVICE2     = 0x0a,
    CC_SERVICE_SERVICE3     = 0x0b,
    CC_SERVICE_SERVICE4     = 0x0c,
    CC_SERVICE_SERVICE5     = 0x0d,
    CC_SERVICE_SERVICE6     = 0x0e,

} EN_CC_SERVICE;

typedef enum
{
    CC_FONTSIZE_NORMAL      = 0x00,
    CC_FONTSIZE_SMALL       = 0x01,
    CC_FONTSIZE_LARGE       = 0x02

} EN_CC_FONT;

typedef enum
{
    EXIT_NULL = 0,
    EXIT_CLOSE,

    EXIT_GOTO_CLOSEDCAPTION,
    EXIT_CLOSEDCAPTION_NULL,
    EXIT_CLOSEDCAPTION_DONE,
    EXIT_TV_GOTO_CLOSEDCAPTION,

} EN_CC_RET;

typedef enum
{
    // action...
    CC_SELECTOR_MODE                = 0x0000,
    CC_SELECTOR_SERVICE,
    CC_SELECTOR_BIN_ID_FONT_START,
    CC_SELECTOR_MAX_FONTTABLE_NUM,
    CC_SELECTOR_TRIGGER_LOAD_FONT,
    CC_SELECTOR_CSOPTION_STYLE,
    CC_SELECTOR_EDGE_STYLE,
    CC_SELECTOR_EDGE_COLOR,
    CC_SELECTOR_FG_COLOR,
    CC_SELECTOR_BG_COLOR,
    CC_SELECTOR_FG_OPACITY,
    CC_SELECTOR_BG_OPACITY,
    CC_SELECTOR_FONT,
    CC_SELECTOR_FONTSTYLE,
    CC_SELECTOR_RESET_TO_DEFAULT,
    CC_SELECTOR_TSP_MODE,

    // attribute...
    CC_SELECTOR_ENABLE24FONTS                   = 0x0100,
    CC_SELECTOR_ENABLEGOPZOOM,
    CC_SELECTOR_ENABLEAUTOSTABYVIDEOFMT,
    CC_SELECTOR_ENABLEFONTEDGE,
    CC_SELECTOR_ENABLERRVBLK,
    CC_SELECTOR_EDGEOPACITY_NOTRANSPARENT_FLAG,
    CC_SELECTOR_SUPPORT_WORDWRAP_FLAG,
    CC_SELECTOR_PARITY_ERROR_SOLID_BLOCK_FLAG,

    // offset, length...
    CC_SELECTOR_ROW_TOPRSVHEIGHT                = 0x0200,
    CC_SELECTOR_ROW_BTMRSVHEIGHT,
    CC_SELECTOR_AXIS_XPOS,
    CC_SELECTOR_AXIS_YPOS,
    CC_SELECTOR_CC_LANG,
    CC_SELECTOR_TEXT_ROWNO,
    CC_SELECTOR_TEXT_YPOS,
    CC_SELECTOR_608_TIMEOUT_CAPITON,
    CC_SELECTOR_608_TIMEOUT_TEXT,
    CC_SELECTOR_DTV_TIMEOUT_CAPTION,
    CC_SELECTOR_AXIS_X_OFFSET,
    CC_SELECTOR_AXIS_Y_OFFSET,
    CC_SELECTOR_CC_FREERUN,
    CC_SELECTOR_DEBUG,

    // info...
    CC_SELECTOR_ERROR_CODE          = 0x0300,
    CC_SELECTOR_708_AVAILABLETYPE,
    CC_SELECTOR_608_AVAILABLETYPE,
    CC_SELECTOR_STATUS_CODE,

    // CC selector tail
    CC_SELECTOR_END                 = 0x0f00

} EN_CC_SELECTOR;

/* Error code */
typedef enum
{
    CC_ERRORCODE_SUCCESS            = 0x0000,
    CC_ERRORCODE_FAIL               = 0x0001,

    CC_ERRORCODE_LOAD_FONT_FAIL     = 0xf000

} EN_CC_ERRORCODE;

typedef enum
{
    CC_LIVE,
    CC_RECORD,
    CC_FILE
} EN_CC_TSPMODE;

typedef struct
{
    MsMW_Alloc                  pAllocCache;
    MsMW_Free                   pFreeCache;
    MsMW_Alloc                  pAllocNonCache;
    MsMW_Free                   pFreeNonCache;
    MsMW_VA2PA                  pVa2Pa;
    MsMW_PA2VA                  pPa2Va;
} MsMW_CcMemOp_t;

typedef enum
{
    E_APICC_GOP_DST_AUTO = 0,
   	E_APICC_GOP_DST_IP,
   	E_APICC_GOP_DST_OP,
} APICC_GOP_DST_TYPE;
//-------------------------------------------------------------------------------------------------
/// set CC GOP destination type
/// @param APICC_GOP_DST_TYPE
/// @return None
//-------------------------------------------------------------------------------------------------
void apiCC_Set_GOP_Dst(APICC_GOP_DST_TYPE type);
//-------------------------------------------------------------------------------------------------
/// set CC GOP Number
/// @param MS_U8
/// @return None
//-------------------------------------------------------------------------------------------------
void apiCC_Set_GOP_Num(MS_U8 u8Gop);
//-------------------------------------------------------------------------------------------------
/// get CC GOP destination type
/// @param None
/// @return APICC_GOP_DST_TYPE
//-------------------------------------------------------------------------------------------------
APICC_GOP_DST_TYPE apiCC_Get_GOP_Dst(void);

/***************************************************************************************/
/* Variables                                                                           */
/***************************************************************************************/


/***************************************************************************************/
/* Functions                                                                           */
/***************************************************************************************/

/*
 * CC UI Interface
 */
MS_U32 MApi_CC_StartParser(void);
MS_U32 MApi_CC_CtrlParser(MS_U32 selector, MS_U32 value);
MS_U32 MApi_CC_StopParser(void);
MS_U32 MApi_CC_GetInfo(MS_U32 selector);
void MApi_ClosedCaption_Open_Filter(MS_U16 u16PID);
void MApi_ClosedCaption_GetPid_Cb(MS_U16 u16Pid);

/*
 * CC UI handler Interface
 */
void apiCC_Start_Task(void);
void apiCC_Stop_Task(void);
void apiCC_Open(void);
void apiCC_Init(MsMW_CcMemOp_t* pMemOp);

#ifdef __cplusplus
}
#endif


#endif
