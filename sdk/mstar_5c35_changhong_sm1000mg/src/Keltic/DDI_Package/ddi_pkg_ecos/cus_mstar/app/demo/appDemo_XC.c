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

//-----------------------------------------------------------------------------
//
/// @file   appDemo_XC.c
/// @author MStar Semiconductor Inc.
/// @brief  scaler setting demo code
//
// @name Box App Command Usage
// @verbatim
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 1. Init Scaler with default resolution.
// Usage: XC_Init
//
// Sample Command: XC_Init
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 2. Set Scaler Output Timing without input video(no signal setting). This Function Will Also Set Scaler Destination Window Size also.
// Usage: XC_SetOutputTiming OutTiming x y width height
//     OutTiming :
//         E_OUTPUT_TIMING_720X480_60I = 0,
//         E_OUTPUT_TIMING_720X480_60P = 1,
//         E_OUTPUT_TIMING_720X576_50I = 2,
//         E_OUTPUT_TIMING_720X576_50P = 3,
//         E_OUTPUT_TIMING_1280X720_50P = 4,
//         E_OUTPUT_TIMING_1280X720_60P = 5,
//         E_OUTPUT_TIMING_1920X1080_50I = 6,
//         E_OUTPUT_TIMING_1920X1080_60I = 7,
//         E_OUTPUT_TIMING_1920X1080_24P = 8,
//         E_OUTPUT_TIMING_1920X1080_25P = 9,
//         E_OUTPUT_TIMING_1920X1080_30P = 10,
//         E_OUTPUT_TIMING_1920X1080_50P = 11,
//         E_OUTPUT_TIMING_1920X1080_60P = 12,
//     x : HStart Position
//     y : HStart Position
//     width : Width of Display
//     height : Height of Display
//
// Sample Command: XC_SetOutputTiming 5 0 0 1280 720
//                 XC_SetOutputTiming 12 0 0 1920 1080
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 3. Init VE with default resolution.
// Usage: VE_Init
//
// Sample Command: VE_Init
//
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 4. Set VE Output Timing. This Function Will Also Set Scaler Destination Window Size Automatically.
// Usage: VE_SetOutputTiming OutTiming x y width height
//     OutTiming : Output Timing
//        NTSC = 0
//        PAL = 1
//     x : HStart Position
//     y : HStart Position
//     width : Width of Display
//     height : Height of Display
//
// Sample Command: VE_SetOutputTiming 0 0 0 720 480
//                 VE_SetOutputTiming 1 0 0 720 576
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 5. VE Set MV Type
//
// Usage: VE_SetMV [bEnable] [eType]
//     [bEnable]
//         - 0 :disable
//         - 1 :enable
//     [eType]
//         -   MS_VE_MV_NTSC_TYPE1 = 0
//         -   MS_VE_MV_NTSC_TYPE2 = 1
//         -   MS_VE_MV_NTSC_TYPE3 = 2
//         -   MS_VE_MV_PAL_TYPE1 = 16
//         -   MS_VE_MV_PAL_TYPE2 = 17
//         -   MS_VE_MV_PAL_TYPE3 = 18
//
// Sample Command: VE_SetMV 0 0   //DISABLE MV
//                 VE_SetMV 1 0   //ENABLE MV NTSC TYPE1
//                 VE_SetMV 0 16
//                 VE_SetMV 1 16  //ENABLE MV PAL TYPE1
//----------------------------------------------------------------------------
// @endverbatim
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
//#include "ver_dtv_comm.h"
#include <string.h>
#include <time.h>
#include "MsOS.h"

//Api
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "apiXC_Ace.h"
//#include "apiGOP_SC.h"
#include "apiGOP.h"
#include "apiACP.h"
#include "apiDMX.h"

//Drv
#include "drvTVEncoder.h"
#include "drvMVOP.h"
//#include "drvPQ.h"
#include "drvSYS.h"
#include "drvMVOP.h"
#include "drvUART.h"

//MApi
#include "msAPI_XC.h"
#include "msAPI_VE.h"
#include "msAPI_GEGOP.h"
#include "Panel.h"
#include "appDemo_XC.h"
#include "appDemo_Video.h"

#ifdef ENABLE_MM_PACKAGE
#include "msAPI_MM.h"
extern MS_BOOL b_MM_Initied;
#endif

// VBI raw data
#include "bindata/VBI_ttx_raw.h"
//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
/// xc print function
#if ENABLE_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
/// XC check if init ok
#define APPDEMO_XC_CHECK_INITIALIZED                            \
    if(msAPI_XC_CheckInitStatus() == FALSE)                     \
    {                                                           \
        printf("ERROR: execute msAPI_XC_Init First!! \n");      \
        return FALSE;                                           \
    }

/// VE check if init ok
#define APPDEMO_VE_CHECK_INITIALIZED                            \
    if(msAPI_VE_CheckInitStatus() == FALSE)                     \
    {                                                           \
        printf("ERROR: execute msAPI_VE_Init First!! \n");      \
        return FALSE;                                           \
    }

#define VIDEO_SIZE_DEC_RATE 0.1  ///< video size decrease rate
#define VIDEO_SIZE_INC_STEP 5 ///< video size increase step
#define TEST_PAUSE_TIME 3000  ///< test pause time( us )
#define CROP_ZOOM_MOVING_DELAY 200 ///< crop-zoom window moving delay

// Typically GOP0 is used for the HD UI, GOP1 is used for the SD UI and GOP2 is used for subtitle
// Therefore in still image zapping we choose to use GOP3 for HD still image and SD still image shares GOP1 with the SD UI
#define STILL_IMG_ZAPPING_HD_GOP   0 ///< HD GOP used for still image zapping
#define STILL_IMG_ZAPPING_SD_GOP   0xff ///< SD GOP used for still image zapping

//XC SeamlessZapping DS buffer size
#define SEAMLESS_ZAPPING_DSBUF_LEN    0x1000  /* 4KB */

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_DEMO_DISP_INFO g_Demo_DispInfo;
MS_BOOL g_Demo_VE_CaptureModeEn;
MS_U8 *_buf = 0;
MS_U32 _NumOfRawData = 0;
MS_U32 _vbicnt = 0;
extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;
void *_pDWinBuf = NULL;
MS_U8 u8DSBuf[SEAMLESS_ZAPPING_DSBUF_LEN];
static MS_BOOL bSeamlessEn = FALSE;
MS_U32 u32DWinBufSize = 0;
MS_WINDOW_TYPE stCropZoomWin;

//******************************************************
//  Customer Horizontal Ratio
//  Customer Vertical Ratio
//******************************************************
E_AR_TYPE g_eAspectRatio = E_AR_INVALID;
MS_U16 g_u16CusHRatio = 1, g_u16CusVRatio = 1;


//******************************************************
//  Flag for Request Frame Buffer Less Mode
//  MS Scaler has 3 different modes to assess MIU
//  1. Normal Mode(access MIU)
//  2. FBL(access no MIU)
//  3. RFBL(some frames/fields access MIU, some not)
//******************************************************
//MS_BOOL g_bRFBL_Mode = FALSE;
//MS_BOOL g_bCusRFBL_Mode = FALSE; //Flag for APP or Customer layer to set RFBL Mode

//-------------------------------------------------------------------------------------------------
// Static Variable
//-------------------------------------------------------------------------------------------------
// @cond INTERNAL
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
#if 0
//----------------------------------------------------------------------------------
/// @brief function to get input information of VE   \n
/// \b this function is needed if VE is in capture mode  \n
//----------------------------------------------------------------------------------

void _Demo_VE_ResetCapWindow(void)
{
    MS_WINDOW_TYPE dispWin;

    MApi_XC_GetDispWinFromReg(&dispWin, MAIN_WINDOW);

    //-----------------------------------------------------------------------------
    // VE Capture Mode:
    // (1)Default set sync inverse
    // (2)H/V: detect raising edge
    // (3)dispWin.x/y from scaler is x/y + sync width + back porch
    // (4)H/V start is sync width + back porch
    //  So, we need to calculate the really capture offset by following formula
    //-----------------------------------------------------------------------------
    g_Demo_DispInfo.SD_SrcWin.x = dispWin.x - g_IPanel.HStart() + (MS_U16)g_IPanel.HSynBackPorch() ;
    g_Demo_DispInfo.SD_SrcWin.y = dispWin.y - g_IPanel.VStart() + (MS_U16)g_IPanel.VSynBackPorch();
    g_Demo_DispInfo.SD_SrcWin.width = dispWin.width+1;
    g_Demo_DispInfo.SD_SrcWin.height = dispWin.height+1;

    db_print("dispWin.x = %d \n", dispWin.x);
    db_print("dispWin.y = %d \n", dispWin.y);
    db_print("dispWin.width+1 = %d \n", dispWin.width+1);
    db_print("dispWin.height+1 = %d \n", dispWin.height+1);

    db_print("SD_SrcWin.x = %d \n", g_Demo_DispInfo.SD_SrcWin.x);
    db_print("SD_SrcWin.y = %d \n", g_Demo_DispInfo.SD_SrcWin.y);
    db_print("SD_SrcWin.width = %d \n", g_Demo_DispInfo.SD_SrcWin.width);
    db_print("SD_SrcWin.height = %d \n", g_Demo_DispInfo.SD_SrcWin.height);

}
#endif

//----------------------------------------------------------------------------------
/// @brief function to calculate aspect ratio of VE  \n
//----------------------------------------------------------------------------------
void _Demo_VE_CalculateAspectRatio(E_ASPECT_RATIO_TYPE enVideoScreen,
                                       MS_VE_WINDOW_TYPE *pstCapWin,
                                       MS_VE_WINDOW_TYPE *pstDstWin)
{
    MS_U16 u16Temp;
    VE_DrvStatus veStatus;

    MDrv_VE_GetStatus(&veStatus);

    switch( enVideoScreen )
    {
        case E_ASPECT_RATIO_16_9:
            {
                db_print("Aspect Ratio Type: E_ASPECT_RATIO_16_9\n");
                u16Temp = (U32)pstDstWin->height * 16 / 9;
                if (u16Temp <= pstDstWin->width) // H:V >= 16:9
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstDstWin->width * 9 / 16;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;
        case E_ASPECT_RATIO_4_3:
            {
                db_print("Aspect Ratio Type: E_ASPECT_RATIO_4_3\n");
                u16Temp = (U32)pstDstWin->height * 4 / 3;
                if (u16Temp <= pstDstWin->width) // H:V >= 4:3
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 4:3
                {
                    u16Temp = (U32)pstDstWin->width * 3 / 4;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;

        case E_ASPECT_RATIO_FULL:
        default:
            {
                db_print("Aspect Ratio Type: E_ASPECT_RATIO_FULL\n");
                pstDstWin->x = 0;
                pstDstWin->y = 0;
                if(veStatus.VideoSystem <= MS_VE_NTSC_J)
                {
                    pstDstWin->width= 720;
                    pstDstWin->height = 480;
                }
                else
                {
                    pstDstWin->width= 720;
                    pstDstWin->height = 576;
                }
            }
            break;

        case E_ASPECT_RATIO_NORMAL:
            db_print("Aspect Ratio Type: E_ASPECT_RATIO_NORMAL\n");
            break;
        case E_ASPECT_RATIO_CUT_4_3: // cut off content to reserve aspect ratio
            db_print("Aspect Ratio Type: E_ASPECT_RATIO_CUT_4_3\n");
            //capture window
            if((pstDstWin->width != 0) || (pstDstWin->height != 0))
            {
                u16Temp = (U32)pstCapWin->height * pstDstWin->width / pstDstWin->height;
                if (u16Temp <= pstCapWin->width) // H:V >= 4:3
                {
                    pstCapWin->x += (pstCapWin->width - u16Temp) / 2;
                    pstCapWin->width = u16Temp;
                }
                else // H:V <= 4:3
                {
                    u16Temp = (U32)pstCapWin->width * pstDstWin->height / pstDstWin->width;
                    pstCapWin->y += (pstCapWin->height - u16Temp) / 2;
                    pstCapWin->height = u16Temp;
                }
            }
            break;
        case E_ASPECT_RATIO_CUT_16_9:
            db_print("Aspect Ratio Type: E_ASPECT_RATIO_CUT_16_9\n");
            //capture window
            {
                u16Temp = (U32)pstCapWin->height * 16 / 9;
                if (u16Temp <= pstCapWin->width) // H:V >= 16:9
                {
                    pstCapWin->x += (pstCapWin->width - u16Temp) / 2;
                    pstCapWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstCapWin->width * 9 / 16;
                    pstCapWin->y += (pstCapWin->height - u16Temp) / 2;
                    pstCapWin->height = u16Temp;
                }
            }
            //display window
            {
                u16Temp = (U32)pstDstWin->height * 16 / 9;
                if (u16Temp <= pstDstWin->width) // H:V >= 16:9
                {
                    pstDstWin->x += (pstDstWin->width - u16Temp) / 2;
                    pstDstWin->width = u16Temp;
                }
                else // H:V <= 16:9
                {
                    u16Temp = (U32)pstDstWin->width * 9 / 16;
                    pstDstWin->y += (pstDstWin->height - u16Temp) / 2;
                    pstDstWin->height = u16Temp;
                }
            }
            break;

    }

}
//----------------------------------------------------------------------------------
/// @brief function to get input information of VE   \n
/// \b this function is needed if VE is in capture mode  \n
//----------------------------------------------------------------------------------
void _Demo_VE_ResetCapWindow(void)
{
    MS_WINDOW_TYPE dispWin;

    MApi_XC_GetDispWinFromReg(&dispWin, MAIN_WINDOW);

    //-----------------------------------------------------------------------------
    // VE Capture Mode:
    // (1)Default set sync inverse
    // (2)H/V: detect raising edge
    // (3)dispWin.x/y from scaler is x/y + sync width + back porch
    // (4)H/V start is sync width + back porch
    //  So, we need to calculate the really capture offset by following formula
    //-----------------------------------------------------------------------------
    g_Demo_DispInfo.SD_SrcWin.x = dispWin.x - g_IPanel.HStart() + (MS_U16)g_IPanel.HSynBackPorch() ;
    g_Demo_DispInfo.SD_SrcWin.y = dispWin.y - g_IPanel.VStart() + (MS_U16)g_IPanel.VSynBackPorch();
    g_Demo_DispInfo.SD_SrcWin.width = dispWin.width+1;
    g_Demo_DispInfo.SD_SrcWin.height = dispWin.height+1;

    db_print("dispWin.x = %d \n", dispWin.x);
    db_print("dispWin.y = %d \n", dispWin.y);
    db_print("dispWin.width+1 = %d \n", dispWin.width+1);
    db_print("dispWin.height+1 = %d \n", dispWin.height+1);

    db_print("SD_SrcWin.x = %d \n", g_Demo_DispInfo.SD_SrcWin.x);
    db_print("SD_SrcWin.y = %d \n", g_Demo_DispInfo.SD_SrcWin.y);
    db_print("SD_SrcWin.width = %d \n", g_Demo_DispInfo.SD_SrcWin.width);
    db_print("SD_SrcWin.height = %d \n", g_Demo_DispInfo.SD_SrcWin.height);

}

//----------------------------------------------------------------------------
/// @brief demo function for SCART out with RGB data
//----------------------------------------------------------------------------
void _Demo_Set_SCART_RGB(MS_BOOL bEnable)
{
    db_print("is invoked\n");

    if(bEnable)
    {
        //CVBS from SD DAC
        MApi_DAC_Enable(ENABLE, FALSE);

        //SCART is output from HD DAC
        //Set HD DAC CLK as clk_ve, Enable HD RGB DAC.
        MApi_DAC_Enable(ENABLE, TRUE);
        MApi_DAC_SetOutputSource(E_DAC_TO_VE_YPBPR, TRUE);
        MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, TRUE);
        MApi_DAC_SetOutputSwapSel(E_DAC_R_G_B, TRUE);
    }
    else
    {
        MApi_DAC_Enable(DISABLE, FALSE);
        MApi_DAC_Enable(DISABLE, TRUE);
    }
}

//----------------------------------------------------------------------------
/// @brief demo function for YPbPr out which is from SD PATH
//----------------------------------------------------------------------------
void _Demo_Set_SD_YPBPR(MS_BOOL bEnable)
{
    db_print("is invoked\n");

    if(bEnable)
    {
        //CVBS from SD DAC
        MApi_DAC_Enable(ENABLE, FALSE);

        //YPbPr is output from HD DAC
        //Set HD DAC CLK as clk_ve, Enable HD RGB DAC.
        MApi_DAC_Enable(ENABLE, TRUE);
        MApi_DAC_SetOutputSource(E_DAC_TO_VE_YPBPR, TRUE);
        MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, TRUE);
        MApi_DAC_SetOutputSwapSel(E_DAC_R_B_G, TRUE);
    }
    else
    {
        MApi_DAC_Enable(DISABLE, FALSE);
        MApi_DAC_Enable(DISABLE, TRUE);
    }
}

//----------------------------------------------------------------------------
/// @brief Video Path 2 need the same frame rate on HD and SD \n
///         -   HD: DTV -> XC -> HDMITx/Component \n
///         -   SD: DTV -> XC -> GOP_SC -> VE_OSD_BLENDER -> VE Encoder \n
//----------------------------------------------------------------------------
MS_BOOL _Demo_HD2SD_CheckFrameRate(void)
{
    switch(g_PNL_TypeSel)
    {
        case PANEL_DACOUT_480I:
        case PANEL_DACOUT_480P:
        case PANEL_DACOUT_720P_60:
        case PANEL_DACOUT_1080I_60:
        case PANEL_DACOUT_1080P_60:
            MDrv_VE_SetOutputVideoStd(MS_VE_NTSC);
            break;
        case PANEL_DACOUT_576I:
        case PANEL_DACOUT_576P:
        case PANEL_DACOUT_720P_50:
        case PANEL_DACOUT_1080I_50:
        case PANEL_DACOUT_1080P_50:
            MDrv_VE_SetOutputVideoStd(MS_VE_PAL);
            break;
        case PANEL_DACOUT_1080P_24:
        case PANEL_DACOUT_1080P_25:
        case PANEL_DACOUT_1080P_30:
        default:
            db_print("ERROR: HD/SD need have the same frame rate\n");
            db_print("ERROR: Not support current resolution!\n");
            return FALSE;
    }
    return TRUE;
}
#if 0
//----------------------------------------------------------------------------
/// @brief demo function to set VE output through PATH2 and the video source is from GOP_SC
//----------------------------------------------------------------------------
MS_BOOL _Demo_HD2SD_Path2(void)
{
    MAPI_GOP_SC_INFO info;
    VE_DrvStatus veStatus;

    // Enable op2 to ve path
    MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND2);

    MDrv_VE_GetStatus(&veStatus);

    //set GOP_SC
    info.enSrcSel = EN_SC_RGB;//HD: VIDEO + OSD
    info.stDst = EN_GOP_SC_DST_FBL;//VIDEO + OSD Blend on VE

    //also can get parmater from Scaler
    info.stInputCfg.u16VTotal = g_IPanel.VTotal();
    info.stInputCfg.u16HTotal = g_IPanel.HTotal();
    info.stInputCfg.u16Width = g_IPanel.Width();
    info.stInputCfg.u16Hight = g_IPanel.Height();
    info.stInputCfg.u16Vde_St = g_IPanel.VStart();
    info.stInputCfg.u16Hde_St = g_IPanel.HStart();

    MApi_GOP_SC_Init();
    if(veStatus.VideoSystem <= MS_VE_NTSC_J)
    {
        info.u8TVSys = EN_GOP_SC_NTSC;
    }
    else
    {
        info.u8TVSys = EN_GOP_SC_PAL;
    }

    MApi_GOP_SC_SetCfg(&info);

    //Enable VE OSD
    MDrv_VE_SetOSD(TRUE);

    return TRUE;
}
#endif

//----------------------------------------------------------------------------
/// @brief Enable/Disable HD Output YpbPr/HDMItx
//----------------------------------------------------------------------------
void _Demo_HD_EnableMiscOutput(MS_BOOL bEnable)
{
    if(bEnable)
    {
        printf("[%s][%d]fixed me for HDMI \n",__FUNCTION__,__LINE__);
        //db_print("Enable HD DAC output...\n");
        MApi_DAC_Enable(TRUE, TRUE);
        //db_print("Enable HDMITx output...\n");
        //do below functions in _SetDacTimingSample();
        //MApi_HDMITx_SetHDMITxMode_CD(HDMITX_HDMI, g_eCurColorDepth);
        //MApi_HDMITx_SetVideoOutputTiming(MApi_XC_GetHDMIType(g_PNL_TypeSel));
        //MApi_HDMITx_SetTMDSOnOff(TRUE);
        //MApi_HDMITx_Exhibit();
    }
    else
    {
        printf("[%s][%d]fixed me for HDMI \n",__FUNCTION__,__LINE__);
        //MApi_HDMITx_SetAVMUTE(TRUE);
        //MApi_HDMITx_SetTMDSOnOff(FALSE);
        MApi_DAC_Enable(FALSE, TRUE);
    }
}

//----------------------------------------------------------------------------
/// @brief to check whether the display windows are valid
//----------------------------------------------------------------------------
MS_BOOL _Demo_Check_DispWin_IsValid(MS_DEMO_DISP_INFO *p_Demo_DispInfo, MS_U32 u32ScalerWindow)
{
    VE_DrvStatus VeStatus;
    //HD
    if((p_Demo_DispInfo->HD_DestWin[u32ScalerWindow].width == 0)||(p_Demo_DispInfo->HD_DestWin[u32ScalerWindow].height == 0))
    {
        p_Demo_DispInfo->HD_DestWin[u32ScalerWindow].x = 0;
        p_Demo_DispInfo->HD_DestWin[u32ScalerWindow].x = 0;
        p_Demo_DispInfo->HD_DestWin[u32ScalerWindow].width= g_IPanel.Width();
        p_Demo_DispInfo->HD_DestWin[u32ScalerWindow].height= g_IPanel.Height();
    }

    //SD
    if((p_Demo_DispInfo->SD_DestWin.width == 0)||(p_Demo_DispInfo->SD_DestWin.height == 0))
    {
        MDrv_VE_GetStatus(&VeStatus);

        p_Demo_DispInfo->SD_DestWin.x = 0;
        p_Demo_DispInfo->SD_DestWin.x = 0;
        p_Demo_DispInfo->SD_DestWin.width= 720;
        if(VeStatus.VideoSystem <= MS_VE_NTSC_J)
            p_Demo_DispInfo->SD_DestWin.height= 480;
        else
            p_Demo_DispInfo->SD_DestWin.height= 576;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief function to get time
//------------------------------------------------------------------------------
static MS_U32 _appDemo_GetTime(void)
{
    #if defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_ECOS)
        return MsOS_GetSystemTime();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec* 1000+ ts.tv_nsec/1000000;
    #endif
}

//------------------------------------------------------------------------------
/// @brief function to set cus aspect ratio
//------------------------------------------------------------------------------

void _appDemo_LB_CalTempWin(MS_WINDOW_TYPE *pWin,
                            MS_U32 u32AspectWidth,
                            MS_U32 u32AspectHeight,
                            MS_U32 u32DstWidth,
                            MS_U32 u32DstHeight,
                            MS_U32 u32ConWidth,
                            MS_U32 u32ConHeight)
{
    MS_U32 u32Temp = 0;
    MS_U32 u32Temp2 = 0;

    u32Temp = u32AspectHeight * u32ConWidth / u32ConHeight;

    printf("u32AspectWidth = %lu \n", u32AspectWidth);
    printf("u32AspectHeight = %lu \n", u32AspectHeight);
    printf("u32DstWidth = %lu \n", u32DstWidth);
    printf("u32DstHeight = %lu \n", u32DstHeight);
    printf("u32ConWidth = %lu \n", u32ConWidth);
    printf("u32ConHeight = %lu \n", u32ConHeight);

    printf("u32Temp = %lu \n", u32Temp);

    if (u32Temp <= u32AspectWidth) // H:V >= u32ConWidth : u32ConHeight (e.g. 16:9)
    {
        printf("[CASE 1] H:V >= %lu:%lu \n", u32ConWidth, u32ConHeight);

        u32Temp2 = ((u32DstWidth * u32AspectHeight) / u32AspectWidth);
        u32Temp2 = (u32Temp2 <= u32DstHeight) ? u32Temp2 : u32DstHeight;
        pWin->x = (MS_U16)0;
        pWin->y = (MS_U16)((u32DstHeight - u32Temp2) / 2 );
        pWin->width = (MS_U16)u32DstWidth;
        pWin->height = (MS_U16)u32Temp2;
    }
    else // H:V < u32ConWidth : u32ConHeight
    {
        printf("[CASE 2] H:V < %lu:%lu \n", u32ConWidth, u32ConHeight);

        u32Temp2 = ((u32DstHeight * u32AspectWidth) / u32AspectHeight);
        u32Temp2 = (u32Temp2 <= u32DstWidth) ? u32Temp2 : u32DstWidth;
        pWin->x = (MS_U16)((u32DstWidth - u32Temp2) / 2);
        pWin->y = (MS_U16)0;
        pWin->width = (MS_U16)u32Temp2;
        pWin->height = (MS_U16)u32DstHeight;
    }

     printf("Temp Window: x,y,w,h=%u,%u,%u,%u\n", pWin->x, pWin->y, pWin->width, pWin->height);

}

void _appDemo_PS_CalTempWin(MS_WINDOW_TYPE *pWin,
                            MS_U32 u32AspectWidth,
                            MS_U32 u32AspectHeight,
                            MS_U32 u32ConWidth,
                            MS_U32 u32ConHeight)
{
    MS_U32 u32Temp = 0;
    MS_U32 u32Temp2 = 0;

    u32Temp = u32AspectHeight * u32ConWidth / u32ConHeight;

    printf("u32AspectWidth = %lu \n", u32AspectWidth);
    printf("u32AspectHeight = %lu \n", u32AspectHeight);
    printf("u32ConWidth = %lu \n", u32ConWidth);
    printf("u32ConHeight = %lu \n", u32ConHeight);

    printf("u32Temp = %lu \n", u32Temp);

    if (u32Temp <= u32AspectWidth) // H:V >= u32ConWidth : u32ConHeight (e.g. 16:9)
    {
        printf("[CASE 1] H:V >= %lu:%lu \n", u32ConWidth, u32ConHeight);

        u32Temp2 = ((u32AspectHeight * u32ConWidth) / u32ConHeight);
        u32Temp2 = (u32Temp2 <= u32AspectWidth) ? u32Temp2 : u32AspectWidth;

        pWin->x = (MS_U16)((u32AspectWidth - u32Temp2) / 2);
        pWin->y = (MS_U16)0;
        pWin->width = (MS_U16)u32Temp2;
        pWin->height = (MS_U16)u32AspectHeight;
    }
    else // H:V < u32ConWidth : u32ConHeight
    {
        printf("[CASE 2] H:V < %lu:%lu \n", u32ConWidth, u32ConHeight);

        u32Temp2 = ((u32AspectWidth * u32ConHeight) / u32ConWidth);
        u32Temp2 = (u32Temp2 <= u32AspectHeight) ? u32Temp2 : u32AspectHeight;
        pWin->x = (MS_U16)0;
        pWin->y = (MS_U16)((u32AspectHeight - u32Temp2) / 2 );
        pWin->width = (MS_U16)u32AspectWidth;
        pWin->height = (MS_U16)u32Temp2;
    }

     printf("Temp Window: x,y,w,h=%u,%u,%u,%u\n", pWin->x, pWin->y, pWin->width, pWin->height);

}



void _appDemo_SetCusAspectRatio(MS_WINDOW_TYPE *pstCropWin, MS_WINDOW_TYPE *pstDstWin)
{
    MS_U16 u16Temp = 0;
    MS_WINDOW_TYPE stCropWin = {0}, stDstWin = {0};
    MS_WINDOW_TYPE stTempWin = {0};
    VDEC_EX_DispInfo stVdecInfo = {0};
    E_ASPECT_RATIO_TYPE eAS = E_ASPECT_RATIO_NORMAL; // do nothing

    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    if((pstCropWin == NULL) || (pstDstWin == NULL))
    {
        printf("Invalid CropWin (%4u,%4u,%4u,%4u) or DstWin (%4u,%4u,%4u,%4u) \n", pstCropWin->x, pstCropWin->y, pstCropWin->height, pstCropWin->width, pstDstWin->x, pstDstWin->y, pstDstWin->height, pstDstWin->width);
        return;
    }

    if(E_VDEC_EX_OK!=MApi_VDEC_EX_GetDispInfo(stVDECStreamId,&stVdecInfo))
    {
        printf("Cannot get VdecDispInfo\n");
        return;
    }
    else
    {
        stVdecInfo.u32AspectWidth = (stVdecInfo.u32AspectWidth == 0)? stVdecInfo.u16HorSize:stVdecInfo.u32AspectWidth;
        stVdecInfo.u32AspectHeight = (stVdecInfo.u32AspectHeight == 0)? stVdecInfo.u16VerSize:stVdecInfo.u32AspectHeight;

        if(MApi_VDEC_EX_GetActiveCodecType(stVDECStreamId) == E_VDEC_EX_CODEC_TYPE_MPEG2)
        {
            if(stVdecInfo.u8AspectRate == 2) // According to spec.
            {
                eAS = E_ASPECT_RATIO_4_3;
            }
            else if(stVdecInfo.u8AspectRate == 3)
            {
                eAS = E_ASPECT_RATIO_16_9;
            }
        }
        else if(MApi_VDEC_EX_GetActiveCodecType(stVDECStreamId) == E_VDEC_EX_CODEC_TYPE_H264)
        {
            if(((MS_U32)stVdecInfo.u32AspectWidth *(MS_U32)2) > ((MS_U32)stVdecInfo.u32AspectHeight*(MS_U32)3))
            {
                eAS = E_ASPECT_RATIO_16_9;
            }
            else
            {
                eAS = E_ASPECT_RATIO_4_3;
            }
        }
    }

    printf("Input:\n");
    printf("\tCropWin : (%4u,%4u,%4u,%4u)\n", pstCropWin->x, pstCropWin->y, pstCropWin->height, pstCropWin->width);
    printf("\tDstWin  : (%4u,%4u,%4u,%4u)\n", pstDstWin->x, pstDstWin->y, pstDstWin->height, pstDstWin->width);
    printf("\teAS      : %u\n", eAS);

    memcpy(&stCropWin, pstCropWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&stDstWin, pstDstWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&stTempWin, pstCropWin, sizeof(MS_WINDOW_TYPE));

    if((stCropWin.height == 0) || (stCropWin.width == 0) || (stDstWin.height == 0) || (stDstWin.width == 0))
    {
        // do nothing
        printf("Invalid CropWin (%4u,%4u,%4u,%4u) or DstWin (%4u,%4u,%4u,%4u) \n", stCropWin.x, stCropWin.y, stCropWin.height, stCropWin.width, stDstWin.x, stDstWin.y, stDstWin.height, stDstWin.width);
    }
    else
    {
        if((g_u16CusHRatio == 0) || (g_u16CusVRatio == 0))
        {
            // do nothing
            printf("Invalid aspect ratio %u:%u\n", g_u16CusHRatio, g_u16CusVRatio);

        }
        else
        {
            switch(g_eAspectRatio)
            {
                case E_AR_PS:
                    _appDemo_PS_CalTempWin(&stTempWin, stVdecInfo.u32AspectWidth, stVdecInfo.u32AspectHeight, g_u16CusHRatio, g_u16CusVRatio);
                    stCropWin.x = stCropWin.x + stTempWin.x * stCropWin.width / stVdecInfo.u32AspectWidth;
                    stCropWin.y = stCropWin.y + stTempWin.y * stCropWin.height / stVdecInfo.u32AspectHeight;
                    stCropWin.width = stTempWin.width * stCropWin.width / stVdecInfo.u32AspectWidth;
                    stCropWin.height = stTempWin.height * stCropWin.height / stVdecInfo.u32AspectHeight;
                    break;

                case E_AR_LB:
                    _appDemo_LB_CalTempWin(&stTempWin, stVdecInfo.u32AspectWidth, stVdecInfo.u32AspectHeight, g_u16CusHRatio*100, g_u16CusVRatio*100, g_u16CusHRatio, g_u16CusVRatio);
                    stDstWin.x = stDstWin.x + stTempWin.x * stDstWin.width / (g_u16CusHRatio*100);
                    stDstWin.y = stDstWin.y + stTempWin.y * stDstWin.height / (g_u16CusVRatio*100);
                    stDstWin.width = stTempWin.width * stDstWin.width / (g_u16CusHRatio*100);
                    stDstWin.height = stTempWin.height * stDstWin.height / (g_u16CusVRatio*100);
                    break;

                case E_AR_4x3_Combind:
                    if(eAS == E_ASPECT_RATIO_16_9)    //16x9 content
                    {
                        u16Temp = stDstWin.height * (9+ ((12-9)>>1) ) / 12;
                        stDstWin.y += (stDstWin.height - u16Temp) / 2;
                        stDstWin.height = u16Temp;

                        u16Temp = stCropWin.width * (12+ ((16-12)>>1) ) / 16;
                        stCropWin.x += (stCropWin.width - u16Temp) / 2;
                        stCropWin.width = u16Temp;
                    }
                    break;

                case E_AR_16x9_Combind:
                    if(eAS == E_ASPECT_RATIO_4_3)    //4x3 content
                    {
                        u16Temp = stCropWin.height * (9+ ((12-9)>>1) ) / 12;
                        stCropWin.y += (stCropWin.height - u16Temp) / 2;
                        stCropWin.height = u16Temp;

                        u16Temp = stDstWin.width * (12+ ((16-12)>>1) ) / 16;
                        stDstWin.x += (stDstWin.width - u16Temp) / 2;
                        stDstWin.width = u16Temp;
                    }
                    break;

                default:
                    break;

            }
        }
    }

	printf("Customer Output:\n");
    printf("\tCropWin (%4u,%4u,%4u,%4u)\n", stCropWin.x, stCropWin.y, stCropWin.height, stCropWin.width);
    printf("\tDstWin  (%4u,%4u,%4u,%4u)\n", stDstWin.x, stDstWin.y, stDstWin.height, stDstWin.width);
    printf("\n");

    if((stDstWin.y < pstDstWin->y) || (stDstWin.x < pstDstWin->x) || ((stDstWin.width + stDstWin.x) > (pstDstWin->width + pstDstWin->x)) || ((stDstWin.height + stDstWin.y) > (pstDstWin->height + pstDstWin->y)))
    {
        printf("ERROR: Customer Disp Window(%u,%u,%u,%u) is larger than Disp Window(%u,%u,%u,%u). ERROR!!\n", stDstWin.x, stDstWin.y, stDstWin.width, stDstWin.height, pstDstWin->x, pstDstWin->y, pstDstWin->width, pstDstWin->height);
    }
    else
    {
        memcpy( pstCropWin, &stCropWin, sizeof(MS_WINDOW_TYPE));
        memcpy( pstDstWin, &stDstWin, sizeof(MS_WINDOW_TYPE));
        printf("Set successfully!!!!!!!\n\n");
    }
}

// @endcond
//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief The sample code to initialize SW/HW of SCALER, DAC and HDMITx which are components of HD display path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_Init \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_Init(void)
{
    MSAPI_XC_CALLBACK_INFO stXCNotifyInfo;
    MSAPI_XC_INITDATA msAPI_XC_InitData;
    printf("[%s][%d]fixed me for HDMI \n",__FUNCTION__,__LINE__);
    MSAPI_XC_HDMITX_BOARD_INFO stHDMITxInfo =
    {
        HDMITX_ANALOG_TUNING_SD,
        HDMITX_ANALOG_TUNING_HD,
        HDMITX_ANALOG_TUNING_DEEP_HD,
    };

    // Parameter Initialization
    memset(&msAPI_XC_InitData, 0, sizeof(MSAPI_XC_INITDATA));

    msAPI_XC_InitData.u32XCMemAddress = SC0_MAIN_FB_ADR;
    msAPI_XC_InitData.u32XCMemSize = SC0_MAIN_FB_LEN;

    msAPI_XC_InitData.eTimingType = E_MSAPI_XC_RES_720x576I_50Hz;

    msAPI_XC_InitData.s32CachedPoolID = gs32CachedPoolID;
#if (MBOOT_LOGO_ENABLE)
    msAPI_XC_InitData.bBootLogoEnable = TRUE;
#else
    msAPI_XC_InitData.bBootLogoEnable = FALSE;
#endif

    //For XC Menu Load, this function to switch XC mode after V-Sync
    msAPI_XC_InitData.bMenuLoadEnable = TRUE;
    msAPI_XC_InitData.u32XCMenuLoadAddress = SC0_MENULOAD_BUF_ADR;
    msAPI_XC_InitData.u32XCMenuLoadSize = SC0_MENULOAD_BUF_LEN;

    //For Bandwidth Table selection and to MIU configuration
    #if (ENABLE_MIU_1)
    msAPI_XC_InitData.eBWTBLType= E_MSAPI_XC_BW_TBL_MIU_128X128;
    #else
    msAPI_XC_InitData.eBWTBLType= E_MSAPI_XC_BW_TBL_MIU_128;
    #endif

    //For HDMITx Configuration
    msAPI_XC_InitData.u16HdmitxHpdPin = HDMITX_HPD_PM_GPIO;
    msAPI_XC_InitData.eHdmitxOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_HDMI;
    msAPI_XC_InitData.eHdmitxCDType = E_MSAPI_XC_HDMITX_CD_8BITS;
    memcpy(&msAPI_XC_InitData.stHdmitxBoardInfo, &stHDMITxInfo, sizeof(MSAPI_XC_HDMITX_BOARD_INFO));

    //For Output Control
    msAPI_XC_InitData.bDisableHDMI = FALSE;
    msAPI_XC_InitData.bDisableDacHD = FALSE;
    msAPI_XC_InitData.bDisableDacSD = TRUE;

    //msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_PQ);
    msAPI_XC_Init(msAPI_XC_InitData);

    //Set DTV path to Scaler
    msAPI_XC_SetConnect(INPUT_SOURCE_DTV);

#if (TVfunc == TVfunc_MpegSD)
    msAPI_MVOP_SetUserDefClkFreq(TRUE, E_MSAPI_MVOP_CLK_27MHZ, E_MSAPI_MVOP_CLK_54MHZ);
#endif //(TVfunc == TVfunc_MpegSD)

    stXCNotifyInfo.pfnCusAspectRatioNotify = _appDemo_SetCusAspectRatio;
    msAPI_XC_RegisterFunction(&stXCNotifyInfo);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change output resolution of HD display path
/// @param[in] *pu32OutputTiming the output resolution of enum E_OUTPUT_TIMING_TYPE
/// @param[in] *x start x of the display window of SCALER
/// @param[in] *y start y of the display window of SCALER
/// @param[in] *width width of the display window of SCALER
/// @param[in] *height height of the display window of SCALER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetOutputTiming \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetOutputTiming(MS_U32 *pu32OutputTiming, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    MS_WINDOW_TYPE dispWin;
    E_OUTPUT_TIMING_TYPE u32Res = *pu32OutputTiming;
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eResType;

    APPDEMO_XC_CHECK_INITIALIZED

    if(u32Res > E_MSAPI_XC_RES_720x576P_50Hz)
    {
        printf("[%s][%d]Error OutputTiming %d\n",__FUNCTION__,__LINE__,u32Res);
        return FALSE;
    }

    eResType = (u32Res == E_OUTPUT_TIMING_640X480_60P)?E_MSAPI_XC_RES_640x480P_60Hz:
                    (u32Res == E_OUTPUT_TIMING_720X480_60I)?E_MSAPI_XC_RES_720x480I_60Hz:
                    (u32Res == E_OUTPUT_TIMING_720X480_60P)?E_MSAPI_XC_RES_720x480P_60Hz:
                    (u32Res == E_OUTPUT_TIMING_720X576_50I)?E_MSAPI_XC_RES_720x576I_50Hz: E_MSAPI_XC_RES_720x576P_50Hz;


    //[Disable output misc] to avoid garbage line
    //_Demo_HD_EnableMiscOutput(DISABLE);
    msAPI_XC_EnableMiscOutput(DISABLE);
    msAPI_VE_EnableMiscOutput(DISABLE);

    //Reset XC Timing
    msAPI_XC_ChangeOutputResolution(eResType);

    //Set Display Window
    dispWin.x = *x + g_IPanel.HStart();
    dispWin.y = *y + g_IPanel.VStart();
    dispWin.width = *width;
    dispWin.height= *height;
    MApi_XC_SetDispWinToReg(&dispWin, MAIN_WINDOW);

    //Set Blue Screen (no video)
    MApi_XC_GenerateBlackVideo(ENABLE, MAIN_WINDOW );

    //[Enable output misc]
    msAPI_XC_EnableMiscOutput(ENABLE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize SW/HW of Video Endcoder which is the main component of SD display path
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_Init \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_Init(void)
{
    MSAPI_VE_INITDATA msAPI_VE_InitData;

    // VE init
    memset(&msAPI_VE_InitData, 0, sizeof(MSAPI_VE_INITDATA));
    msAPI_VE_InitData.u32VEMemAddress = VE_FRAME_BUF_ADR;
    msAPI_VE_InitData.u32VEMemSize= VE_FRAME_BUF_LEN;
    msAPI_VE_InitData.eOutputVideoStd = MS_VE_PAL;
    msAPI_VE_InitData.eInputSrcType = MS_VE_SRC_SCALER;
    msAPI_VE_InitData.eOutputDstType = MS_VE_DEST_CVBS;

    //For Output Control
    msAPI_VE_InitData.bEnableDacSD = TRUE;

#if (MBOOT_LOGO_ENABLE)
    msAPI_VE_InitData.bBootLogoEnable = TRUE;
#else
    msAPI_VE_InitData.bBootLogoEnable = FALSE;
#endif

    msAPI_VE_Init(msAPI_VE_InitData);

#if (TVfunc == TVfunc_MpegSD) || (TVfunc == TVfunc_H264SD)
    msAPI_VE_ForceSetMode(TRUE);
#endif

    //msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_DRV);
    //turn off SD OSD at initial step and turn on it after GOP is initialized
    MDrv_VE_SetOSD(FALSE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change output video standard of SD display path
/// @param[in] *pu32OutputTiming the output resolution of enum MS_VE_VIDEOSYS
/// @param[in] *x start x of the display window of VE
/// @param[in] *y start y of the display window of VE
/// @param[in] *width width of the display window of VE
/// @param[in] *height height of the display window of VE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetOutputTiming 0 0 0 1920 1080 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_SetOutputTiming(MS_U32 *pu32OutputTiming, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    MS_VE_WINDOW_TYPE dispWin = {*x, *y, *width, *height};
    MS_U32 u32Res = *pu32OutputTiming;

    APPDEMO_VE_CHECK_INITIALIZED

    //[Disable output misc] to avoid garbage line
    msAPI_VE_EnableMiscOutput(DISABLE);

    switch(u32Res)
    {
        case 0:
            msAPI_VE_SetOutputVideoStd(MS_VE_NTSC);
            break;
        case 1:
        default:
            msAPI_VE_SetOutputVideoStd(MS_VE_PAL);
            break;
    }

    msAPI_VE_SetMode();
    msAPI_VE_SetWin(NULL, &dispWin);

    //[Enable output misc]
    msAPI_VE_EnableMiscOutput(ENABLE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Play Video Using Default Setting(default size, default aspect ratio)
///         -   HD: DTV -> XC -> HDMITx/Component
///         -   SD: DTV -> XC -> VE Source -> VE Encoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_PlayVideo_UsingDefaultSetting \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_PlayVideo_UsingDefaultSetting(void)
{
    MS_U32 u32StartTime = 0, u32EndTime = 0;

    printf("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__);

    if((!msAPI_GEGOP_IsStillVideoEnable()) && (!bSeamlessEn))
    {
        //Enable Blue Screen to avoid garbage video
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);
    }

    msAPI_XC_EnableCCIRInput(0, DISABLE);

#ifdef ENABLE_MM_PACKAGE
    if((b_MM_Initied == TRUE))// && (msAPI_MM_GetFileType() != MM_ES_MODE))
    {
        if( TRUE != appDemo_MM_Video_GetDecoderStatus() )
        {
            db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
            return FALSE;
        }
    }
    else
#endif
    {
    //Check Video Decoder Status
    if( TRUE != appDemo_Video_GetDecoderStatus())
    {
        db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }
    }

    u32StartTime = _appDemo_GetTime();

    //Set VDEC FRC
    msAPI_XC_DTV_SetMode();

    //Set Scaler(HD PATH)
    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW);

    //reset SD Capture window
    //_Demo_VE_ResetCapWindow();

    msAPI_VE_SetMode();

    //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
    MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);

    //Disable Blue Screen
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);

    u32EndTime = _appDemo_GetTime();
    db_print("[%s][%d] DIFF TIME after MVOP and AV-Sync = %lu ms\n", __FUNCTION__, __LINE__, u32EndTime - u32StartTime);
    db_print("########## XC Bluescreen DISABLE!!\n");

    return TRUE;
}
#ifdef ENABLE_MM_PACKAGE
//------------------------------------------------------------------------------
/// @brief Play Video Using Default Setting(default size, default aspect ratio)
///         -   HD: DTV -> XC -> HDMITx/Component
///         -   SD: DTV -> XC -> VE Source -> VE Encoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b MM_XC_PlayVideo_UsingDefaultSetting \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_XC_PlayVideo_UsingDefaultSetting(void)
{
    //Enable Blue Screen to avoid garbage video
    msAPI_VE_SetVideoMute(ENABLE);
    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

    //Check Video Decoder Status
    if( TRUE != appDemo_MM_Video_GetDecoderStatus())
    {
        db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }

    //Set VDEC FRC
    msAPI_XC_DTV_SetMode();

    //Set Scaler(HD PATH)
    msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW);

    msAPI_VE_EnableCaptureMode(FALSE);
    msAPI_VE_SetMode();

   //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
    MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);

    //Disable Blue Screen
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief Play Video Using Customer Setting(crop size, window size, aspect ratio...)
///         -   HD: DTV -> XC -> HDMITx/Component
///         -   SD: DTV -> XC -> VE Source -> VE Encoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_PlayVideo_UsingCusSetting 0 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_PlayVideo_UsingCusSetting(MS_U32 *pu32ScalerWindow)
{
    MS_U32 u32VEStd;
    VE_DrvStatus VeStatus;

    if(!msAPI_GEGOP_IsStillVideoEnable())
    {
        //Enable Blue Screen to avoid garbage video
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, *pu32ScalerWindow);
    }

    //Check Video Decoder Status
    if( TRUE != appDemo_Video_GetDecoderStatus())
    {
        db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }

    //Set VDEC FRC and Scaler RFBL/FBL Mode
    msAPI_XC_DTV_SetMode();

    //Set Scaler(HD PATH)
    _Demo_Check_DispWin_IsValid(&g_Demo_DispInfo, *pu32ScalerWindow);

    if((g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow].height == 0)||(g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow].width== 0))
        msAPI_XC_SetWin(NULL, NULL, &g_Demo_DispInfo.HD_DestWin[*pu32ScalerWindow], *pu32ScalerWindow);
    else
        msAPI_XC_SetWin(NULL, &g_Demo_DispInfo.HD_CropWin[*pu32ScalerWindow], &g_Demo_DispInfo.HD_DestWin[*pu32ScalerWindow], *pu32ScalerWindow);

    //Set VE(SD PATH)
    MDrv_VE_GetStatus(&VeStatus);
    if(VeStatus.VideoSystem <= MS_VE_NTSC_J)
        u32VEStd = 0;
    else
        u32VEStd = 1;

    if((g_Demo_DispInfo.SD_DestWin.width == 0) || (g_Demo_DispInfo.SD_DestWin.height== 0))
    {
        g_Demo_DispInfo.SD_DestWin.width = 720;
        g_Demo_DispInfo.SD_DestWin.height= (u32VEStd)? 576 : 480;
    }

    //reset SD Capture window
    if (g_Demo_VE_CaptureModeEn == TRUE)
    {
        _Demo_VE_ResetCapWindow();
    }

    //Calculate SD Aspect Ratio
    _Demo_VE_CalculateAspectRatio(g_Demo_DispInfo.SD_AS, &g_Demo_DispInfo.SD_SrcWin, &g_Demo_DispInfo.SD_DestWin);
    db_print("SD_DestWin.x = %d \n", g_Demo_DispInfo.SD_DestWin.x);
    db_print("SD_DestWin.y = %d \n", g_Demo_DispInfo.SD_DestWin.y);
    db_print("SD_DestWin.width = %d \n", g_Demo_DispInfo.SD_DestWin.width);
    db_print("SD_DestWin.height = %d \n", g_Demo_DispInfo.SD_DestWin.height);

    msAPI_VE_EnableCaptureMode(g_Demo_VE_CaptureModeEn);
    msAPI_VE_SetMode();
    msAPI_VE_SetWin(&g_Demo_DispInfo.SD_SrcWin, &g_Demo_DispInfo.SD_DestWin);

    //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
    MApi_XC_WaitOutputVSync(2, 100, MAIN_WINDOW);

    //Disable Blue Screen to avoid garbage video
    msAPI_XC_SetVideoMute(DISABLE, *pu32ScalerWindow);
    msAPI_VE_SetVideoMute(DISABLE);

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief to enable/disable selected video output destination:
/// @param[in] *pu16OutputDest Output Destination Selection of E_OUTPUT_DESTINTATION
///     -    OUTPUT_DESTINATION_SD_CVBS      = 0,
///     -    OUTPUT_DESTINATION_SD_SVIDEO    = 1,
///     -    OUTPUT_DESTINATION_SD_SCART_RGB = 2,
///     -    OUTPUT_DESTINATION_SD_YPBPR     = 3,
///     -    OUTPUT_DESTINATION_HD_COMPONENT = 4,
///     -    OUTPUT_DESTINATION_HD_HDMITX    = 5,
///
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SetOutputDest 3 0 \n
///          \b Video_SetOutputDest 3 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_SetOutputDest(MS_U16 *pu16OutputDest, MS_U16 *pEnable)
{
    MS_Switch_VE_Dest_Info DestInfo;

    switch(*pu16OutputDest)
    {
        default:
        case OUTPUT_DESTINATION_SD_CVBS:
        case OUTPUT_DESTINATION_SD_SVIDEO:
            //CVBS+S-VIDEO
            DestInfo.OutputDstType = MS_VE_DEST_CVBS;
            MDrv_VE_SwitchOuputDest(&DestInfo);
            MApi_DAC_Enable(*pEnable, FALSE);
            break;
        case OUTPUT_DESTINATION_SD_SCART_RGB:
            //CVBS+SCART-RGB
            DestInfo.OutputDstType = MS_VE_DEST_SCART;
            MDrv_VE_SwitchOuputDest(&DestInfo);
            _Demo_Set_SCART_RGB(*pEnable);
            break;
        case OUTPUT_DESTINATION_SD_YPBPR:
            //CVBS+YPbPr
            DestInfo.OutputDstType = MS_VE_DEST_YPBPR;
            MDrv_VE_SwitchOuputDest(&DestInfo);
            _Demo_Set_SD_YPBPR(*pEnable);
            break;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief to enable/disable selected hd output destination:
///
/// @param[in] *pu16OutputDest Output Destination Selection of E_OUTPUT_DESTINTATION
///     -    OUTPUT_DESTINATION_HD_COMPONENT      = 0,
///     -    OUTPUT_DESTINATION_HD_HDMITX    = 1
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b Video_SetOutputDest 3 0 \n
///          \b Video_SetOutputDest 3 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetOutputDest(MS_U16 *pu16OutputDest, MS_U16 *pEnable)
{
    printf("[%s][%d]fixed me for HDMI \n",__FUNCTION__,__LINE__);
    switch(*pu16OutputDest)
    {
        case OUTPUT_DESTINATION_HD_COMPONENT:
            MApi_DAC_Enable(*pEnable, TRUE);
            MApi_DAC_SetClkInv(*pEnable, TRUE);
            break;
        case OUTPUT_DESTINATION_HD_HDMITX:
        default:
            if(*pEnable)
            {
                //Enable Video on HDMITx
                //MApi_HDMITx_SetTMDSOnOff(TRUE);
            }
            else
            {
                //Disable both Video and Audio
                //MApi_HDMITx_SetTMDSOnOff(FALSE);
            }
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Video Brightness
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value brightness value(0~255)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetBrightness 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetBrightness(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    msAPI_XC_ACE_SetBrightness(*pu32ScalerWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Video Sharpness
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value Sharpness value(0~63)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetSharpness 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetSharpness(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    msAPI_XC_ACE_SetSharpness(*pu32ScalerWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Adjust Video contrast in XC
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value - video color Contrast setting 0~100
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetContrast 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetContrast(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    msAPI_XC_ACE_SetContrast(*pu32ScalerWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Adjust Video Hue in XC
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value - Hue Value 0~100
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetHue 0 50 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetHue(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    msAPI_XC_ACE_SetHue(*pu32ScalerWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Adjust Video Saturation in XC
/// @param[in] *pu32ScalerWindow SCALER display window selection
/// @param[in] *pu8Value - Saturation Value 0~255
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetSaturation 0 128 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetSaturation(MS_U32 *pu32ScalerWindow, MS_U8 *pu8Value)
{
    msAPI_XC_ACE_SetSaturation(*pu32ScalerWindow, *pu8Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Disable scaler
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_Exit  \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_Shutdown(void)
{
    msAPI_XC_Exit();
    msAPI_VE_Exit();

    if (_buf) {
        MsOS_FreeMemory(_buf, gs32CachedPoolID);
        _buf = 0;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Scaler Set Frame Buffer Size
///         -   if size==0, switch to FBL Mode
///         -   FBL Mode:
///                       (1)Do Crop function at MVOP
///                       (2)No Post Scaling Down
///                       (3)Preview is not supportted
/// @param[in] *pu32Size the buffer size be allocated to SCALER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note if (size == 0), SCALER will display video in FBL Mode
/// Sample Command: \n
///          \b XC_SetFrameBufferSize 0 \n
///          \b XC_SetFrameBufferSize 16777216 //16MB \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetFrameBufferSize(MS_U32 *pu32Size)
{
    msAPI_XC_SetFrameBufferSize(*pu32Size);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC Set Frame Color(Back Ground Color)
/// @param[in] *pu32Index the color index of back ground color of SCALER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_SetFrameColor 0   //BLACK \n
///          \b XC_SetFrameColor 1   //WHITE \n
///          \b XC_SetFrameColor 2   //RED \n
///          \b XC_SetFrameColor 3   //GREEN \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_SetFrameColor(MS_U32 *pu32Index)
{
    switch(*pu32Index)
    {
        default:
        case 0:
            MApi_XC_SetFrameColor(0x00000000);
            break;
        case 1:
            MApi_XC_SetFrameColor(0x00FFFFFF);
            break;
        case 2:
            MApi_XC_SetFrameColor(0x00FF0000);
            break;
        case 3:
            MApi_XC_SetFrameColor(0x0000FF00);
            break;
        case 4:
            MApi_XC_SetFrameColor(0x000000FF);
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Generate Black Video
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_GenerateBlackVideo \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_GenerateBlackVideo(void)
{
    //Set black Screen (no video)
    MApi_XC_GenerateBlackVideo(ENABLE, MAIN_WINDOW);

    return TRUE;
}

#if defined(MSOS_TYPE_ECOS)
//VBI DEMO
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  VBI Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S32 gs32CachedPoolID;

//-------------------------------------------------------------------------------------------------
//  VBI Local Variables
//-------------------------------------------------------------------------------------------------

#define PACKETS_SEND_THRESHOLD 32 //By experiment, it'd better to send 32 packets in one run
#define VE_DRIVER_TIMEOUT 1000 // millisecond
#define PEDDING_BUFFER_PAYLOAD_NUM 16
#define TTX_PACKET_SIZE 48
#define LINECOUNT 4 // number of line that we insert ttx data in

static void _appDemo_VE_VBITtx_ISR(void)
{
    if (_buf) {
        if (_vbicnt <= _NumOfRawData) {
            _vbicnt++;
        } else {
            // ring back
            _vbicnt = 0;
        }
        MDrv_VE_SetTtxBuffer(MsOS_VA2PA(
                    (MS_U32)(_buf + _vbicnt * TTX_PACKET_SIZE * LINECOUNT)),
                TTX_PACKET_SIZE * LINECOUNT);
    }
    MsOS_EnableInterrupt(E_INT_FIQ_VE_DONE_TT);
}

//------------------------------------------------------------------------------
/// @brief VE Set VBI Ttx demo
/// \n Create the app demo task to read/write TTx
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_VbiTtxDemo   //Start \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_VbiTtxDemo(void)
{
    //read ttx raw data
    if (!_buf) {
        _buf = MsOS_AllocateMemory(sizeof(ttxRawData), gs32CachedPoolID);
        memcpy(_buf, ttxRawData, sizeof(ttxRawData));
        if (!_buf) {
            printf("Error: memory allocation error [%s][%d]\n", __FUNCTION__, __LINE__);
        }
    }
    _NumOfRawData = sizeof(ttxRawData) / TTX_PACKET_SIZE;
    _vbicnt = 0;

    MDrv_VE_EnableTtx(ENABLE);
    MDrv_VE_SetVbiTtxActiveLines(0xff);
    MDrv_VE_ClearTtxReadDoneStatus();
    MsOS_AttachInterrupt(E_INT_FIQ_VE_DONE_TT, (InterruptCb)_appDemo_VE_VBITtx_ISR);

    // start ttvbi
    MsOS_EnableInterrupt(E_INT_FIQ_VE_DONE_TT);
    printf("enable ttvbi\n");
    MDrv_VE_SetTtxBuffer(MsOS_VA2PA(
                (MS_U32)(_buf + _vbicnt * TTX_PACKET_SIZE * LINECOUNT)),
            TTX_PACKET_SIZE * LINECOUNT);
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief XC set drvier dbg level to print common Debug Message (Only for Debug)
/// @param[in] *pbEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_EnableDrvDmsg 0 \n
///          \b XC_EnableDrvDmsg 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_EnableDmesg(MS_U32 *pbEnable)
{
    if(*pbEnable)
    {
       msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_PQ);
       msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_DRV);
    }
    else
    {
        msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_NONE);
        msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_NONE);
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief set ve output to CCIR656 or TV encoder
/// @param[in] pbEnable 1=CCIR656, 0=TV encoder
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b VE_CCIROut 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_CCIROut(MS_U32 *pbEnable)
{
    if (*pbEnable)
    {
        msAPI_VE_SetOutputCtrl(MS_VE_OUT_CCIR656);
    }
    else
    {
        msAPI_VE_SetOutputCtrl(MS_VE_OUT_TVENCODER);
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief set CCIR type
/// @param[in] eCCIRType E_CCIR656_INPUT_720X480_60I, E_CCIR656_INPUT_720X480_60P,
///                      E_CCIR656_INPUT_720X576_50I, E_CCIR656_INPUT_720X576_50P
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:\n
///         \b VE_CCIROut 0\n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_CCIRIn(MS_U32 *eCCIRType)
{
    MS_BOOL bIsInterlace;
    MS_U16 u16FrameRate;
    VDEC_EX_DispInfo stVDEC_DispInfo;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    switch((E_CCIR656_INPUT_TYPE)*eCCIRType)
    {
        case E_CCIR656_INPUT_720X480_60I:
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].x = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].y = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].width= 704;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].height= 480;
            bIsInterlace = TRUE;
            u16FrameRate = 29976;
            break;
        case E_CCIR656_INPUT_720X480_60P:
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].x = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].y = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].width= 720;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].height= 480;
            bIsInterlace = FALSE;
            u16FrameRate = 59940;
            break;
        case E_CCIR656_INPUT_720X576_50I:
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].x = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].y = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].width= 720;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].height= 576;
            bIsInterlace = TRUE;
            u16FrameRate = 50;
            break;
        case E_CCIR656_INPUT_720X576_50P:
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].x = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].y = 0;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].width= 720;
            g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].height= 576;
            bIsInterlace = TRUE;
            u16FrameRate = 50;
            break;
        default:
            db_print(" Type Wrong!! \n");
            return FALSE;
    }

    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

    /////////////////////////////////////
    //Set VDEC information and Set MVOP//
    ////////////////////////////////////

    {
        memset(&stVDEC_DispInfo, 0, sizeof(VDEC_EX_DispInfo));

        stVDEC_DispInfo.u16HorSize = g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].width;
        stVDEC_DispInfo.u16VerSize = g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].height;
        stVDEC_DispInfo.u8Interlace = (MS_U8)bIsInterlace;
        stVDEC_DispInfo.u32FrameRate = (MS_U32)u16FrameRate;

        msAPI_XC_SetVDECInfo_EX(stVDECStreamId,&stVDEC_DispInfo);
    }

    if(TRUE != msAPI_XC_SetMVOPConfig(MVOP_INPUT_MVD))
    {
        db_print(" Video Set Output Config Failed!! \n");
        return FALSE;
    }

    msAPI_XC_EnableCCIRInput(0, ENABLE);

    //Set Scaler(HD PATH)
    msAPI_XC_SetWin(&g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW], NULL, NULL, MAIN_WINDOW);

    //Disable Blue Screen
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);

    db_print("CCIR SrcWin.x = %d \n", g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].x);
    db_print("CCIR SrcWin.x = %d \n", g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].y);
    db_print("CCIR SrcWin.x = %d \n", g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].width);
    db_print("CCIR SrcWin.x = %d \n", g_Demo_DispInfo.HD_CropWin[MAIN_WINDOW].height);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to gradually scale down the output video size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///         \b XC_PlayVideo_ScalingDow \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_PlayVideo_ScalingDown(void)
{
    MS_WINDOW_TYPE tDstWin;
    MS_FLOAT fVideoSizeDecRate = 0.0;
    VDEC_EX_DispInfo PicData;

    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    appDemo_XC_PlayVideo_UsingDefaultSetting();
    MsOS_DelayTask(TEST_PAUSE_TIME);

    if ((MApi_VDEC_EX_GetDispInfo(stVDECStreamId,&PicData) != E_VDEC_EX_OK) || (PicData.u16HorSize==0 || PicData.u16VerSize==0))
    {
        printf("[%s][%d] Get video information failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    // Same aspect video size decreasing
    for(fVideoSizeDecRate = VIDEO_SIZE_DEC_RATE; fVideoSizeDecRate < 1; fVideoSizeDecRate = fVideoSizeDecRate + VIDEO_SIZE_DEC_RATE)
    {
        db_print("DEC RATE = %lf\n", fVideoSizeDecRate);
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

        tDstWin.x = g_IPanel.Width() * (fVideoSizeDecRate / 2);
        tDstWin.width = g_IPanel.Width() * (1 - fVideoSizeDecRate);

        tDstWin.height = g_IPanel.Height() * (1 - fVideoSizeDecRate);
        tDstWin.y = g_IPanel.Height() * (fVideoSizeDecRate / 2);

        //Keltic NTSC V down scaling only support to 1/7 downscale
        if((PicData.u16HorSize/7) > tDstWin.height)
        {
            db_print("Over V_Scaling_Down = %lf\n", fVideoSizeDecRate);
            break;
        }

        msAPI_XC_SetWin(NULL, NULL, &tDstWin, MAIN_WINDOW);
        MApi_XC_WaitOutputVSync(4, 100, MAIN_WINDOW);
        msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
        msAPI_VE_SetVideoMute(DISABLE);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    appDemo_XC_PlayVideo_UsingDefaultSetting();
    MsOS_DelayTask(TEST_PAUSE_TIME);

    // Only H-size decreasing
    for(fVideoSizeDecRate = VIDEO_SIZE_DEC_RATE; fVideoSizeDecRate < 1; fVideoSizeDecRate = fVideoSizeDecRate + VIDEO_SIZE_DEC_RATE)
    {
        db_print("DEC RATE = %lf\n", fVideoSizeDecRate);
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

        tDstWin.x = g_IPanel.Width() * (fVideoSizeDecRate / 2);
        tDstWin.y = 0;
        tDstWin.width = g_IPanel.Width() * (1 - fVideoSizeDecRate);
        tDstWin.height = g_IPanel.Height();

        msAPI_XC_SetWin(NULL, NULL, &tDstWin, MAIN_WINDOW);
        MApi_XC_WaitOutputVSync(4, 100, MAIN_WINDOW);
        msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
        msAPI_VE_SetVideoMute(DISABLE);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    appDemo_XC_PlayVideo_UsingDefaultSetting();
    MsOS_DelayTask(TEST_PAUSE_TIME);

    // Only V-size decreasing
    for(fVideoSizeDecRate = VIDEO_SIZE_DEC_RATE; fVideoSizeDecRate < 1; fVideoSizeDecRate = fVideoSizeDecRate + VIDEO_SIZE_DEC_RATE)
    {
        db_print("DEC RATE = %lf\n", fVideoSizeDecRate);
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

        tDstWin.x = 0;
        tDstWin.width = g_IPanel.Width();

        tDstWin.height = g_IPanel.Height() * (1 - fVideoSizeDecRate);
        tDstWin.y = g_IPanel.Height() * (fVideoSizeDecRate / 2);

        //Keltic NTSC V down scaling only support to 1/7 downscale
        if((PicData.u16HorSize/7) > tDstWin.height)
        {
            db_print("Over V_Scaling_Down = %lf\n", fVideoSizeDecRate);
            break;
        }

        msAPI_XC_SetWin(NULL, NULL, &tDstWin, MAIN_WINDOW);
        MApi_XC_WaitOutputVSync(4, 100, MAIN_WINDOW);
        msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
        msAPI_VE_SetVideoMute(DISABLE);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    appDemo_XC_PlayVideo_UsingDefaultSetting();

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to gradually scale up the output video size
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///                 \b XC_PlayVideo_ScalingUp \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_PlayVideo_ScalingUp(void)
{
    MS_WINDOW_TYPE tDstWin;
    MS_FLOAT fVideoSizeIncRatio = 1.0;
    MS_FLOAT fVideoSizeIncRate = 0.0;
    MS_BOOL isWidthLimit = FALSE, isHeightLimit = FALSE;
    VDEC_EX_DispInfo stVidStatus;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    appDemo_XC_PlayVideo_UsingDefaultSetting();
    MsOS_DelayTask(TEST_PAUSE_TIME);

    msAPI_XC_GetVDECInfo_EX(stVDECStreamId,&stVidStatus);
    tDstWin.width = stVidStatus.u16HorSize;
    tDstWin.height = stVidStatus.u16VerSize;
    tDstWin.x = (g_IPanel.Width() - tDstWin.width) / 2;
    tDstWin.y = (g_IPanel.Height() - tDstWin.height) / 2;

    // Source video size is larger than or equal to the panel size, no need to do scaling up test
    if(tDstWin.width >= g_IPanel.Width() && tDstWin.height >= g_IPanel.Height())
    {
        return TRUE;
    }

    // Set initial video size to original video size
    msAPI_VE_SetVideoMute(ENABLE);
    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);
    msAPI_XC_SetWin(NULL, NULL, &tDstWin, MAIN_WINDOW);
    MApi_XC_WaitOutputVSync(4, 100, MAIN_WINDOW);
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);
    MsOS_DelayTask(TEST_PAUSE_TIME);

    // Make video size grow VIDEO_SIZE_INC_STEP times to reach one side maximum of panel
    if((tDstWin.width / tDstWin.height) >= (g_IPanel.Width() / g_IPanel.Height()))
    {
        fVideoSizeIncRate = ((MS_FLOAT)(g_IPanel.Width() - tDstWin.width) / VIDEO_SIZE_INC_STEP) / stVidStatus.u16HorSize;
    }
    else
    {
        fVideoSizeIncRate = ((MS_FLOAT)(g_IPanel.Height() - tDstWin.height) / VIDEO_SIZE_INC_STEP) / stVidStatus.u16VerSize;
    }

    while(!isWidthLimit || !isHeightLimit) {
        db_print("RATIO=%lf, RATE=%lf\n", fVideoSizeIncRatio, fVideoSizeIncRate);
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

        fVideoSizeIncRatio += fVideoSizeIncRate;

        if(!isWidthLimit)
        {
            tDstWin.width = stVidStatus.u16HorSize * fVideoSizeIncRatio;
            tDstWin.x = (g_IPanel.Width() - tDstWin.width) / 2;
        }
        if(!isHeightLimit)
        {
            tDstWin.height = stVidStatus.u16VerSize * fVideoSizeIncRatio;
            tDstWin.y = (g_IPanel.Height() - tDstWin.height) / 2;
        }

        // Once one side reach maximum, make another side keeps growing and reach maximum in VIDEO_SIZE_INC_STEP times
        if(tDstWin.width >= g_IPanel.Width() && !isWidthLimit)
        {
            isWidthLimit = TRUE;
            tDstWin.width = g_IPanel.Width();
            tDstWin.x = 0;
            fVideoSizeIncRate = ((MS_FLOAT)(g_IPanel.Height() - tDstWin.height) / VIDEO_SIZE_INC_STEP) / stVidStatus.u16HorSize;
        }
        if(tDstWin.height >= g_IPanel.Height() && !isHeightLimit)
        {
            isHeightLimit = TRUE;
            tDstWin.height = g_IPanel.Height();
            tDstWin.y = 0;
            fVideoSizeIncRate = ((MS_FLOAT)(g_IPanel.Width() - tDstWin.width) / VIDEO_SIZE_INC_STEP) / stVidStatus.u16VerSize;
        }

        msAPI_XC_SetWin(NULL, NULL, &tDstWin, MAIN_WINDOW);
        MApi_XC_WaitOutputVSync(4, 100, MAIN_WINDOW);
        msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
        msAPI_VE_SetVideoMute(DISABLE);
        MsOS_DelayTask(TEST_PAUSE_TIME);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief 625i(576i) systems are based on ITU-R BT.1119 and ETSI EN 309 294. \n
///         The Analog Copy Generation Management System(CGMS-A) is also supportted by the WSS signal.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b ACP_Set625CGMS_A   //Start \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_ACP_Set625CGMS_A(MS_U16 *pu16Type)
{
    MS_U16 u16Wss = 0x0;
    E_CGMS_A_TYPE eCGMS_A = E_CGMS_A_INVALID;

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED
    eCGMS_A = (E_CGMS_A_TYPE)*pu16Type;
    u16Wss = MDrv_VE_GetWSSData();

    /*
    *  In 625i systems:
    *  CGMS-A consists of two bits of digital information:
    *  (b12, b13): (copyright, copy protection)
    *          00: copying permitted
    *          01: reserverd (One generation of copies has already been made; no further copying is allowed)
    *          10: one copy permitted
    *          11: no copying permitted
    */
    switch(eCGMS_A)
    {
        case E_CGMS_A_COPY_FREELY:
        {
            // CGMS-A: 00
            u16Wss = u16Wss & ~(MS_WSS_625_COPYRIGHT) & ~(MS_WSS_625_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NO_MORE:
        {
            // CGMS-A: 01
            u16Wss = (u16Wss & ~(MS_WSS_625_COPYRIGHT)) | MS_WSS_625_COPY_PROTECTION;
        }
        break;
        case E_CGMS_A_COPY_ONCE:
        {
            // CGMS-A: 10
            u16Wss = (u16Wss | MS_WSS_625_COPYRIGHT) & ~(MS_WSS_625_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NEVER:
        {
            // CGMS-A: 11
            u16Wss = u16Wss | MS_WSS_625_COPYRIGHT | MS_WSS_625_COPY_PROTECTION;
        }
        break;
        default:
            printf("[Warning!] Abnormal Case!\n");
            return FALSE;
}

    MDrv_VE_SetWSSData(TRUE, u16Wss);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief this demo code is implemented base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
/// @param[in] *pEnable 1=ENABLE, 0=DISABLE
/// @param[in] *pu32index the MV Types
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n  \b VE_SetMV 0 0   //DISABLE MV \n
///          \b ACP_SetMV 1 2   //ENABLE MV NTSC TYPE2 \n
///          \b ACP_SetMV 0 20  \n
///          \b ACP_SetMV 1 20  //ENABLE MV PAL TYPE1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_ACP_SetMV(MS_U32 *pEnable, MS_U32 *pu32index)
{
    MS_BOOL bEnable =  *pEnable;
    MS_ACP_TYPE eACPType = (MS_ACP_TYPE)(*pu32index);
    ACP_Result eRet = E_ACP_FAIL;

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED

    /*
     * For normal use, we recommend following macrovision types to be used in each case:
     * - for NTSC:
     *   MS_ACP_CVBS_NTSC_TYPE2 or MS_ACP_CVBS_NTSC_TYPE3
     *
     * - for NTSC with SCART RGB:
     *   MS_ACP_CVBS_NTSC_TEST_N01 or MS_ACP_CVBS_NTSC_TEST_N02
     *
     * - for PAL or PAL with SCART RGB:
     *   MS_ACP_CVBS_PAL_TEST_P01 or MS_ACP_CVBS_PAL_TEST_P02
     *
     * - for PAL-N, PAL-Nc and SECAM:
     *   MS_ACP_CVBS_PAL_TYPE2
     */
    if (eACPType >= MS_ACP_TYPE_MAX)
    {
        return FALSE;
    }

    /*
     * Note:
     * YPbPr Macrovision function is not ready, please set the second parameter to FALSE
     */
    eRet = MApi_ACP_SetProtection(bEnable, FALSE, eACPType);
    if( eRet != E_ACP_OK )
    {
        db_print("ACP set Macrovision Failed, return value = %d \n", eRet);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief this demo code is implemented DCS analog copy protection
/// @param[in] *pbEnable 1=ENABLE, 0=DISABLE
/// @param[in] *pu16DCSIdx is the DCS Types
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Sample Command: \n
///         \b ACP_SetDCS 1 0   //ENABLE  CVBS NTSC DCS  \n
///         \b ACP_SetDCS 0 0   //DISABLE CVBS NTSC DCS  \n
///         \b ACP_SetDCS 1 1   //ENABLE  CVBS PAL  DCS  \n
///         \b ACP_SetDCS 0 1   //DISABLE CVBS PAL  DCS  \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_ACP_SetDCS(MS_BOOL *pbEnable, MS_U16 *pu16DCSIdx)
{
    ACP_Result eRet = E_ACP_FAIL;
    MS_ACP_DCS_TYPE eDCS_ACP_Idx = (MS_ACP_DCS_TYPE)*pu16DCSIdx;

    //****************************************************
    // DCS ACTIVATION KEY
    // Key TBL DCSKEYTBL is 4*8 array
    //  The first 4 is order by
    //      DCS STATE OFF_TO_TRANSITIONON
    //      DCS STATE TRANSITIONON_TO_ON
    //      DCS STATE ON_TO_TRANSITIONOFF
    //      DCS STATE TRANSITIONOFF_TO_OFF
    //  And the second 8 is order by
    //      [7:0], [15:8], [23:16], [31:24], [39:32], [47:40], [55:48], [63:56] (bit)
    // Ex: 0x1122334455667788 => 0x88, 0x77, ..., 0x22, 0x11
    //****************************************************
    MS_U8 DCSKEYTBL[4][8] =
    {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    eRet = MApi_DCS_SetActivationKey(&(DCSKEYTBL[0][0]), 4*8);
    if( eRet != E_ACP_OK )
    {
        db_print("ACP set DCS Activation Key Failed, return value = %d \n", eRet);
        return FALSE;
    }

    eRet = MApi_DCS_SetProtection(*pbEnable, (MS_U8)eDCS_ACP_Idx);

    if( eRet != E_ACP_OK )
    {
        db_print("ACP set DCS Index %d Failed, return value = %d \n", *pu16DCSIdx, eRet);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop UART connection for debugging
/// @param[in] *pbEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b UART_Stop 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_UART_Stop(MS_U32 *pbEnable)
{
    printf("Stop UART! \n");
    MDrv_UART_SetIOMapBase();
    if(*pbEnable)
    {
        if(mdrv_uart_get_connection(E_UART_PORT0) == E_UART_PIU_UART0)
        {
            mdrv_uart_connect(E_UART_PORT0, E_UART_PIU_UART1);
        }
        else
        {
            mdrv_uart_connect(E_UART_PORT0, E_UART_PIU_UART0);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief set WWS mode
/// @param[in] *mode a MS_U16 value of the following enum
///  typedef enum
///  1:   MS_WSS_4x3_FULL
///  2:   MS_WSS_14x9_LETTERBOX_CENTER
///  3:   MS_WSS_14x9_LETTERBOX_TOP
///  4:   MS_WSS_16x9_LETTERBOX_CENTER
///  5:   MS_WSS_16x9_LETTERBOX_TOP
///  6:   MS_WSS_ABOVE16x9_LETTERBOX_CENTER
///  7:   MS_WSS_14x9_FULL_CENTER
///  8:   MS_WSS_16x9_ANAMORPHIC
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_SetWSS  1\n
///          \b VE_SetWSS  8\n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_SetWSS(MS_U32 *mode)
{
    printf("[%s][%d] WSS mode: 0x%02lx\n", __FUNCTION__, __LINE__, *mode);
    MS_U16 u16Mode = *mode;
    MS_U16 u16WSSData = 0, u16PreWSSData = 0;

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED

    switch (u16Mode)
    {
        case 1:
            u16WSSData = MS_WSS_4x3_FULL;
            break;
        case 2:
            u16WSSData =  MS_WSS_14x9_LETTERBOX_CENTER;
            break;
        case 3:
            u16WSSData =  MS_WSS_14x9_LETTERBOX_TOP;
            break;
        case 4:
            u16WSSData =  MS_WSS_16x9_LETTERBOX_CENTER;
            break;
        case 5:
            u16WSSData =  MS_WSS_16x9_LETTERBOX_TOP;
            break;
        case 6:
            u16WSSData =  MS_WSS_ABOVE16x9_LETTERBOX_CENTER;
            break;
        case 7:
            u16WSSData =  MS_WSS_14x9_FULL_CENTER;
            break;
        case 8:
            u16WSSData =  MS_WSS_16x9_ANAMORPHIC;
            break;
        default:
            printf("known WSS mode: 0x%02lx\n", *mode);
            printf("1:   MS_WSS_4x3_FULL\n"
                    "2:   MS_WSS_14x9_LETTERBOX_CENTER\n"
                    "3:   MS_WSS_14x9_LETTERBOX_TOP\n"
                    "4:   MS_WSS_16x9_LETTERBOX_CENTER\n"
                    "5:   MS_WSS_16x9_LETTERBOX_TOP\n"
                    "6:   MS_WSS_ABOVE16x9_LETTERBOX_CENTER\n"
                    "7:   MS_WSS_14x9_FULL_CENTER\n"
                    "8:   MS_WSS_16x9_ANAMORPHIC\n");
    }
    u16PreWSSData = MDrv_VE_GetWSSData();
    u16WSSData = (u16PreWSSData & 0xFFF0) | u16WSSData;
    MDrv_VE_SetWSSData(ENABLE, u16WSSData);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief set 525 WWS mode
/// @param[in] *mode - a MS_U32 value of the following enum
///  typedef enum
///  1:   MS_WSS_4x3_FULL
///  2:   MS_WSS_14x9_LETTERBOX_CENTER
///  3:   MS_WSS_14x9_LETTERBOX_TOP
///  4:   MS_WSS_16x9_LETTERBOX_CENTER
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b VE_Set525WSS 1 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_VE_Set525WSS(MS_U32 *mode)
{
    MS_U32 u32Mode = *mode;
    MS_U32 u32WSSData = 0, u32PreWSSData = 0;

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED

    printf("[%s][%d] WSS mode: 0x%08lx\n", __FUNCTION__, __LINE__, *mode);

    switch (u32Mode) {
    case 1:
        u32WSSData = MS_WSS_525_4x3_NORMAL;
        break;
    case 2:
        u32WSSData =  MS_WSS_525_4x3_LETTERBOX;
        break;
    case 3:
        u32WSSData =  MS_WSS_525_16x9_ANAMORPHIC;
        break;
    case 4:
        u32WSSData =  MS_WSS_525_RESERVED;
        break;
    default:
        printf("unknown WSS mode: 0x%02lx\n", u32Mode);
        printf("1:   MS_WSS_525_4x3_NORMAL\n"
               "2:   MS_WSS_525_4x3_LETTERBOX\n"
               "3:   MS_WSS_525_16x9_ANAMORPHIC\n"
               "4:   MS_WSS_525_RESERVED\n");
        return FALSE;
    }

    u32PreWSSData = MDrv_VE_GetWSS525Data();
    u32WSSData = (u32PreWSSData & 0xFFFC) | u32WSSData;

    if ( E_VE_OK != MDrv_VE_SetWSS525Data(ENABLE, u32WSSData) )
    {
        printf("[Error] MDrv_VE_SetWSS525Data return not OK!\n");
    }

    u32WSSData = MDrv_VE_GetWSS525Data();

    db_print("[%s][%d] WSS: 0x%08lx\n", __FUNCTION__, __LINE__, u32WSSData);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief EIA-J CPR-1204 and IEC 61880 define a widescreen signaling stnadard for 525i (480i) system \n
///         The Analog Copy Generation Management System(CGMS-A) is also supportted by the WSS signal. \n
///         Bits b6 and b7 specify the copy generation management system in an analog signal.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b ACP_Set525CGMS_A_IEC61880 0  //CGMS-A COPY_FREELY\n
///          \b ACP_Set525CGMS_A_IEC61880 3  //CGMS-A COPY_NEVER\n
//------------------------------------------------------------------------------
MS_BOOL appDemo_ACP_Set525CGMS_A_IEC61880(MS_U16 *pu16Type)
{
    MS_U32 u32Wss = 0x0;
    E_CGMS_A_TYPE eCGMS_A = E_CGMS_A_INVALID;

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED

    eCGMS_A = (E_CGMS_A_TYPE)*pu16Type;
    u32Wss = MDrv_VE_GetWSS525Data();

    /*
     *  In 525i systems:
     *  CGMS-A consists of two bits of digital information:
     *  (b6, b7): (copyright, copy protection)
     *          00: copying permitted
     *          01: reserverd (One generation of copies has already been made; no further copying is allowed)
     *          10: one copy permitted
     *          11: no copying permitted
     */
    switch(eCGMS_A)
    {
        case E_CGMS_A_COPY_FREELY:
        {
            // CGMS-A: 00
            u32Wss = u32Wss & ~(MS_WSS_525_COPYRIGHT) & ~(MS_WSS_525_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NO_MORE:
        {
            // CGMS-A: 01
            u32Wss = (u32Wss & ~(MS_WSS_525_COPYRIGHT)) | MS_WSS_525_COPY_PROTECTION;
            break;
        }
        break;
        case E_CGMS_A_COPY_ONCE:
        {
            // CGMS-A: 10
            u32Wss = (u32Wss | MS_WSS_525_COPYRIGHT) & ~(MS_WSS_525_COPY_PROTECTION);
        }
        break;
        case E_CGMS_A_COPY_NEVER:
        {
            // CGMS-A: 11
            u32Wss = u32Wss | MS_WSS_525_COPYRIGHT | MS_WSS_525_COPY_PROTECTION;
        }
        break;
        default:
            printf("[Warning!] Abnormal Case!\n");
            return FALSE;
    }

    if ( E_VE_OK != MDrv_VE_SetWSS525Data(TRUE, u32Wss))
    {
        printf("[Error] MDrv_VE_SetWSS525Data return not OK!\n");
    }

    u32Wss = MDrv_VE_GetWSS525Data();

    db_print("[%s][%d] WSS: 0x%08lx\n", __FUNCTION__, __LINE__, u32Wss);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief EIA-J CPR-1204 and IEC 61880 define a widescreen signaling stnadard for 525i (480i) system \n
///         Bits b8 and b9 specify the Analog Protection Service (APS) added to the analog NTSC or PAL video signal.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b ACP_Set525APS_IEC61880   //APS \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_ACP_Set525APS_IEC61880(MS_U16 *pu16Type)
{
    MS_U32 u32Wss = 0, u32PreWss = 0;
    E_APS_TYPE eAPStype = E_APS_INVALID;

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED

    eAPStype = (E_APS_TYPE)*pu16Type;
    u32Wss = MDrv_VE_GetWSS525Data();

    /*
     *  In 525i systems:
     *  APS consists of two bits of digital information:
     *  (b8, b9): specify the APS type for 525i system
     *          00: no Analog Protection Service
     *          01: PSP on, color striping off
     *          10: PSP on, 2-line color striping on
     *          11: PSP on, 4-line color striping on
     */
    switch(eAPStype)
    {
        case E_APS_NO_APS:
        {
            // APS: 00
            u32Wss = MS_WSS_525_APS_NO_APS;
        }
        break;
        case E_APS_PSP_ONLY:
        {
            // APS: 01
            u32Wss = MS_WSS_525_APS_PSP_CS_OFF;
            break;
        }
        break;
        case E_APS_PSP_2_LINE_CB:
        {
            // APS: 10
            u32Wss = MS_WSS_525_APS_PSP_2_LINE_CS;
        }
        break;
        case E_APS_PSP_4_LINE_CB:
        {
            // APS: 11
            u32Wss = MS_WSS_525_APS_PSP_4_LINE_CS;
        }
        break;
        default:
            printf("[Warning!] Abnormal Case!\n");
            return FALSE;
    }

    u32PreWss = MDrv_VE_GetWSS525Data();
    u32Wss = (u32PreWss & 0xFCFF) | u32Wss;

    if ( E_VE_OK != MDrv_VE_SetWSS525Data(TRUE, u32Wss))
    {
        printf("[Error] MDrv_VE_SetWSS525Data return not OK!\n");
    }

    u32Wss = MDrv_VE_GetWSS525Data();

    db_print("[%s][%d] WSS: 0x%08lx\n", __FUNCTION__, __LINE__, u32Wss);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Thinking to some pourpose, to reduce bandwidth cost, we use this command to do force pre-scaling . \n
///        What MSUT be careful is that the API may cause a decrease in quality also.
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_ForcePrescaling   //Start \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_ForcePrescaling(MS_U32 *pU32Type)
{
    MS_U32 u32Type = *pU32Type;
    MS_BOOL bEnable = FALSE;

    if (u32Type == 2)
    {
        bEnable = TRUE;
        msAPI_XC_SetCommand(NULL , E_MSAPI_XC_CMD_SET_FORCE_H_PRESCALING_HALF, &bEnable);
    }
    else if (u32Type == 4)
    {
        bEnable = TRUE;
        msAPI_XC_SetCommand(NULL , E_MSAPI_XC_CMD_SET_FORCE_H_PRESCALING_QUARTER, &bEnable);
    }
    else
    {
        bEnable = FALSE;
        msAPI_XC_SetCommand(NULL , E_MSAPI_XC_CMD_SET_FORCE_H_PRESCALING_HALF, &bEnable);
        msAPI_XC_SetCommand(NULL , E_MSAPI_XC_CMD_SET_FORCE_H_PRESCALING_QUARTER, &bEnable);
    }

    return TRUE;
}

#define XC_STACK_SIZE     4096  ///< Demo XC Monitor task stack size
static MS_U8            _XCStack[XC_STACK_SIZE];
static MS_S32           _s32XCTaskId = -1;

static void _XC_Monitor(void)
{
    MS_U32 u32stc;
    MS_U32 u32prestc;
    MS_U32 u32VWptr = 0 ;//MApi_VDEC_EX_GetESWritePtr();
    MS_U32 u32VRptr = 0 ;//MApi_VDEC_EX_GetESReadPtr();
    MS_U32 u32BufferSize = 0x400000;
    MS_U32 u32VESlevel;

    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    while (1)
    {
        MApi_XC_WaitOutputVSync( 1, 50, MAIN_WINDOW);
        printf("Field = %d, FD_Msk = %d \n",
            MApi_XC_ReadByte(0x103D77) & BIT(2),
            MApi_XC_ReadByte(0x103D77) & BIT(1)
            );

        u32VWptr = MApi_VDEC_EX_GetESWritePtr(stVDECStreamId);
        u32VRptr = MApi_VDEC_EX_GetESReadPtr(stVDECStreamId);
        if (u32VWptr>=u32VRptr)
        {
            u32VESlevel = u32VWptr - u32VRptr;
        }
        else
        {
            u32VESlevel = u32BufferSize - ( u32VRptr - u32VWptr );
        }
        if(u32VESlevel>=(0x400000-2048)) //4// 4M -2K  20120627 liuhaijun
        {
            printf("[_MonitorVideoTaskEntry] vdec_flush u32VESlevel =0x%lx,curms:%ld\n",u32VESlevel,MsOS_GetSystemTime());
        }

        MApi_DMX_Stc_Get(&u32prestc, &u32stc);
        printf("FrameCnt:%ld(%ld,%ld,%ld)STC: %ld  VPTS:%ld VDELTA: %ld APTS:%ld ADelta:%ld, VESLevel:%ld,AES:%lx,APCM:%lx DISPCNT=%ld SYSTIMME=%ld,Sync=%d,%ld\n",
            MApi_VDEC_EX_GetFrameCnt(stVDECStreamId),
            MApi_VDEC_EX_GetErrCnt(stVDECStreamId),
            MApi_VDEC_EX_GetSkipCnt(stVDECStreamId),
            MApi_VDEC_EX_GetDropCnt(stVDECStreamId),
            u32stc / 90,
            MApi_VDEC_EX_GetPTS(stVDECStreamId),
            MApi_VDEC_EX_GetPTS(stVDECStreamId)-u32stc / 90,
            (MS_U32)((MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS)) / 90),
            (MS_U32)((MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS)) / 90)-u32stc / 90,
            u32VESlevel,
            (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_ESBufferSize),
            (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_PCMBufferSize),
            MApi_VDEC_EX_GetDispCnt(stVDECStreamId),
            MsOS_GetSystemTime(),
            MApi_VDEC_EX_IsReachSync(stVDECStreamId),
            (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_AudioSyncStatus)
            );

        MsOS_DelayTask(10);
    }
}

//------------------------------------------------------------------------------
/// @brief Task to monitor video related status
/// @param[in] *pbEnable 1=ENABLE, 0=DISABLE
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \n
///          \b XC_StatusMonitor   //Start \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_StatusMonitor(MS_U32 *pbEnable)
{
    MS_BOOL bEnable = *pbEnable;
    VDEC_EX_Status stVedcStatus = {0};
    MS_BOOL bMvopEnable = FALSE;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    APPDEMO_XC_CHECK_INITIALIZED
    APPDEMO_VE_CHECK_INITIALIZED

    MApi_VDEC_EX_GetStatus(stVDECStreamId,&stVedcStatus);
    MDrv_MVOP_GetIsEnable(&bMvopEnable);
    if ( stVedcStatus.bInit != TRUE )
    {
        printf("ERROR: execute VDEC Init First!! \n");
    }

    if ( bMvopEnable == FALSE)
    {
        printf("ERROR: execute MVOP Enable First!! \n");
    }

    if (bEnable == TRUE)
    {
        if (_s32XCTaskId == -1)
        {
            _s32XCTaskId = MsOS_CreateTask((TaskEntry)_XC_Monitor,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_MEDIUM,
                                        TRUE,
                                        (void *)_XCStack,
                                        XC_STACK_SIZE,
                                        "Demo XC Monitor");
            if( _s32XCTaskId == -1)
            {
                return FALSE;
            }
        }
        else
        {
            MsOS_ResumeTask(_s32XCTaskId);
        }
    }
    else
    {
        if( _s32XCTaskId == -1)
        {
            return FALSE;
        }
        else
        {
            MsOS_SuspendTask(_s32XCTaskId);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable still image for zapping without blackscreen
/// @param[in] *bEnable enabling flag for still image, 1=enable, 0=disable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \b XC_EnableStillImage \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_EnableStillImage(MS_BOOL *bEnable)
{
    MS_U32 u32DWinBufPA = 0;
    MS_BOOL bRet = FALSE;

    // Show still image
    if(*bEnable)
    {
        MS_BOOL bNewMem = FALSE;

        if(!_pDWinBuf)
        {
            bNewMem = TRUE;
        }
        else
        {
            if(u32DWinBufSize < msAPI_GEGOP_GetStillVideoRequireMem(STILL_IMG_ZAPPING_SD_GOP))
            {
                if(!MsOS_FreeMemory(_pDWinBuf, gs32NonCachedPoolID))
                {
                    printf("[%s][%d] Free memory failed!!\n", __FUNCTION__, __LINE__);
                }

                bNewMem = TRUE;
            }
        }

        if(bNewMem)
        {
            u32DWinBufSize = msAPI_GEGOP_GetStillVideoRequireMem(STILL_IMG_ZAPPING_SD_GOP);
            _pDWinBuf = MsOS_AllocateMemory(u32DWinBufSize, gs32NonCachedPoolID);

            if(!_pDWinBuf)
            {
                printf("[%s][%d] Cannot obtain memory!!\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
        }

        u32DWinBufPA = MsOS_VA2PA((MS_U32)_pDWinBuf);
        db_print("Still zapping DWIN buffer VA = %lu, PA = %lu, size = %lu\n", (MS_U32)_pDWinBuf, u32DWinBufPA, u32DWinBufSize);

        bRet = msAPI_GEGOP_CreateStillVideo(STILL_IMG_ZAPPING_HD_GOP, STILL_IMG_ZAPPING_SD_GOP, u32DWinBufPA, u32DWinBufSize);

        if(bRet)
        {
            bRet = msAPI_GEGOP_ShowStillVideo(STILL_IMG_ZAPPING_HD_GOP, STILL_IMG_ZAPPING_SD_GOP, u32DWinBufPA, u32DWinBufSize);
        }
    }
    // Hide still image
    else
    {
        bRet = msAPI_GEGOP_HideStillVideo();

        if(bRet)
        {
            bRet = msAPI_GEGOP_DestroyStillVideo();
        }

        if(!MsOS_FreeMemory(_pDWinBuf, gs32NonCachedPoolID))
        {
            printf("[%s][%d] Free memory failed!!\n", __FUNCTION__, __LINE__);
        }

        _pDWinBuf = NULL;
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable XC freeze image for zapping without blackscreen
/// @param[in] *bEnable enabling flag for still image, 1=enable, 0=disable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \b XC_EnableFreezeImage \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_EnableFreezeImage(MS_BOOL *bEnable)
{
    if(!bSeamlessEn)
    {
        if(msAPI_XC_SetSeamlessZapping(MAIN_WINDOW, TRUE, MEMALIGN(0x8, MsOS_VA2PA((MS_U32)u8DSBuf))) != E_MSAPI_XC_OK)
        {
            printf("[%s][%d] Set SeamlessZapping Fail!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        bSeamlessEn = TRUE;
    }

    if(MApi_XC_IsFreezeImg(MAIN_WINDOW) && (*bEnable == TRUE))
    {
        printf("[%s][%d] Already Freeze, SKIP!!\n", __FUNCTION__, __LINE__);
        return TRUE;
    }

    MApi_XC_FreezeImg(*bEnable, MAIN_WINDOW);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable crop-zoom mode
/// @param[in] *bEnable enabling flag, 1=enable, 0=disable
/// @param[in] *u32InitX crop-zoom window initial x position
/// @param[in] *u32InitY crop-zoom window initial y position
/// @param[in] *u32Width crop-zoom window width
/// @param[in] *u32Height crop-zoom window height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \b XC_EnableCropZoomMode \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_EnableCropZoomMode(MS_BOOL *bEnable, MS_U32 *u32InitX, MS_U32 *u32InitY, MS_U32 *u32Width, MS_U32 *u32Height)
{
    msAPI_VE_SetVideoMute(ENABLE);
    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

    // Set MVOP crop and XC source window size
    if(*bEnable)
    {
        stCropZoomWin.x = *u32InitX;
        stCropZoomWin.y = *u32InitY;
        stCropZoomWin.width= *u32Width;
        stCropZoomWin.height= *u32Height;

        msAPI_XC_SetWin(&stCropZoomWin, NULL, NULL, MAIN_WINDOW);
        MDrv_MVOP_SetStartPos(stCropZoomWin.x, stCropZoomWin.y);
        MDrv_MVOP_SetImageWidthHight(stCropZoomWin.x + stCropZoomWin.width, stCropZoomWin.y + stCropZoomWin.height);
        MDrv_MVOP_SetRegSizeFromMVD(FALSE);

    }
    else
    {
        msAPI_XC_SetWin(NULL, NULL, NULL, MAIN_WINDOW);
    }

    MApi_XC_WaitOutputVSync(2, 50, MAIN_WINDOW);
    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    msAPI_VE_SetVideoMute(DISABLE);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to move crop-zoom window
/// @param[in] *u32Direction crop-zoom window moving direction, 0=up, 1=down, 2=left, 3=right
/// @param[in] *u32Distance distance of one moving step
/// @param[in] *u32Step total steps of moving
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command: \b XC_MoveCropZoomWin \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_MoveCropZoomWin(MS_U32 *u32Direction, MS_U32 *u32Distance, MS_U32 *u32Step)
{
    VDEC_EX_DispInfo stVidStatus;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    msAPI_XC_GetVDECInfo_EX(stVDECStreamId,&stVidStatus);

    switch (*u32Direction)
    {
        case 0: // Up
            while (((int)stCropZoomWin.y - 2*((int)(*u32Distance))) >= 0 && (*u32Step) > 0)
            {
                stCropZoomWin.y -= 2*(*u32Distance);
                (*u32Step)--;
                MApi_XC_WaitOutputVSync(1, 30, MAIN_WINDOW);
                MDrv_MVOP_SetStartPos(stCropZoomWin.x, stCropZoomWin.y);
                MDrv_MVOP_SetImageWidthHight(stCropZoomWin.x + stCropZoomWin.width, stCropZoomWin.y + stCropZoomWin.height);
                MsOS_DelayTask(CROP_ZOOM_MOVING_DELAY);
            }
            break;
        case 1: // Down
            while (stCropZoomWin.y + stCropZoomWin.height + 2*(*u32Distance) <= stVidStatus.u16VerSize && (*u32Step) > 0)
            {
                stCropZoomWin.y += 2*(*u32Distance);
                (*u32Step)--;
                MApi_XC_WaitOutputVSync(1, 30, MAIN_WINDOW);
                MDrv_MVOP_SetImageWidthHight(stCropZoomWin.x + stCropZoomWin.width, stCropZoomWin.y + stCropZoomWin.height);
                MDrv_MVOP_SetStartPos(stCropZoomWin.x, stCropZoomWin.y);
                MsOS_DelayTask(CROP_ZOOM_MOVING_DELAY);
            }
            break;
        case 2: // Left
            while (((int)stCropZoomWin.x - 2*((int)(*u32Distance))) >= 0 && (*u32Step) > 0)
            {
                stCropZoomWin.x -= 2*(*u32Distance);
                (*u32Step)--;
                MApi_XC_WaitOutputVSync(1, 30, MAIN_WINDOW);
                MDrv_MVOP_SetStartPos(stCropZoomWin.x, stCropZoomWin.y);
                MDrv_MVOP_SetImageWidthHight(stCropZoomWin.x + stCropZoomWin.width, stCropZoomWin.y + stCropZoomWin.height);
                MsOS_DelayTask(CROP_ZOOM_MOVING_DELAY);
            }
            break;
        case 3: // Right
            while (stCropZoomWin.x + stCropZoomWin.width + 2*(*u32Distance) <= stVidStatus.u16HorSize && (*u32Step) > 0)
            {
                stCropZoomWin.x += 2*(*u32Distance);
                (*u32Step)--;
                MApi_XC_WaitOutputVSync(1, 30, MAIN_WINDOW);
                MDrv_MVOP_SetImageWidthHight(stCropZoomWin.x + stCropZoomWin.width, stCropZoomWin.y + stCropZoomWin.height);
                MDrv_MVOP_SetStartPos(stCropZoomWin.x, stCropZoomWin.y);
                MsOS_DelayTask(CROP_ZOOM_MOVING_DELAY);
            }
            break;
        default:
            printf("[%s][%d] Wrong direction!!\n", __FUNCTION__, __LINE__);
            return FALSE;
    }

    db_print("Crop-zoom: x=%d, y=%d\n", stCropZoomWin.x, stCropZoomWin.y);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief XC command menu
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:\n
///         \b HelpXC \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_XC_Help(void)
{
    printf ("************************* XC function List *************************\n");
    printf ("1. XC_Init \n");
    printf ("2. XC_SetOutputTiming [OutTiming] [x] [y] [width] [height] \n");
    printf ("-----OutTiming : Output Resolution\n");
    printf ("---------------: E_OUTPUT_TIMING_720X480_60I = 0,\n");
    printf ("---------------: E_OUTPUT_TIMING_720X480_60P = 1,\n");
    printf ("---------------: E_OUTPUT_TIMING_720X576_50I = 2,\n");
    printf ("---------------: E_OUTPUT_TIMING_720X576_50P = 3,\n");
    printf ("---------------: E_OUTPUT_TIMING_1280X720_50P = 4,\n");
    printf ("---------------: E_OUTPUT_TIMING_1280X720_60P = 5,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_50I = 6,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_60I = 7,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_24P = 8,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_25P = 9,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_30P = 10,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_50P = 11,\n");
    printf ("---------------: E_OUTPUT_TIMING_1920X1080_60P = 12,\n");
    printf ("---------------: E_OUTPUT_TIMING_640X480_60P = 13,\n");
    printf ("-----x : HStart position \n");
    printf ("-----y : VStart Position \n");
    printf ("-----width : Width of Display \n");
    printf ("-----height : Height of Display \n");
    printf ("====>example : XC_SetOutputTiming 5 0 0 1280 720\n");
    printf ("====>example : XC_SetOutputTiming 12 0 0 1920 1080\n");
    printf ("3. VE_Init \n");
    printf ("4. VE_SetOutputTiming [OutTiming] [x] [y] [width] [height]\n");
    printf ("-----OutTiming : Output Standard(NTSC/PAL)\n");
    printf ("---------------: NTSC = 0,\n");
    printf ("---------------: PAL = 1,\n");
    printf ("-----x : HStart position \n");
    printf ("-----y : VStart Position \n");
    printf ("-----width : Width of Display \n");
    printf ("-----height : Height of Display \n");
    printf ("====>example : VE_SetOutputTiming 0 0 0 720 480\n");
    printf ("====>example : VE_SetOutputTiming 1 0 0 720 576\n");
    printf ("5. XC_PlayVideo_UsingDefaultSetting\n");
    printf ("6. XC_Exit \n");
    printf ("7. XC_SetFrameBufferSize [Buffer Size] \n");
    printf ("-----Buffer Size : Frame Buffer Size for Scaler \n");
    printf ("---------------:if size==0, switch to FBL Mode \n");
    printf ("====>example : XC_SetFrameBufferSize 0 \n");
    printf ("====>example : XC_SetFrameBufferSize 16777216 //16MB \n");
    printf ("8. XC_SetFrameColor [ColorIndex] \n");
    printf ("-----[ColorIndex] :\n");
    printf ("---------------: BLACK = 0 \n");
    printf ("---------------: WHITE = 1 \n");
    printf ("---------------: RED = 2 \n");
    printf ("---------------: GREEN = 3 \n");
    printf ("---------------: BLUE = 4 \n");
    printf ("====>example : XC_SetFrameColor 1   //WHITE \n");
    printf ("====>example : XC_SetFrameColor 2   //RED \n");
    printf ("9. XC_SetBrightness [ScalerWindow] [value(0~255)]\n");
    printf ("====>example : XC_SetBrightness 0 128\n");
    printf ("10. XC_SetSharpness [ScalerWindow] [value(0~63)]\n");
    printf ("====>example : XC_SetSharpness 0 50\n");
    printf ("11. XC_SetContrast [ScalerWindow] [value(0~255)]\n");
    printf ("====>example : XC_SetContrast 0 50\n");
    printf ("12. XC_SetHue [ScalerWindow] [value(0~100)]\n");
    printf ("====>example : XC_SetHue 0 50\n");
    printf ("13. XC_SetSaturation [ScalerWindow] [value(0~255)]\n");
    printf ("====>example : XC_SetSaturation 0 128\n");
    printf ("14. XC_EnableDrvDmsg [bEnable] \n");
    printf ("====>example : XC_EnableDrvDmsg 0 \n");
    printf ("====>example : XC_EnableDrvDmsg 1 \n");
    printf ("15. ACP_Set625CGMS_A [type] \n");
    printf ("---------------: 0 = copying permitted \n");
    printf ("---------------: 1 = reserverd \n");
    printf ("---------------: 2 = one copy permitted \n");
    printf ("---------------: 3 = no copying permitted \n");
    printf ("====>example : ACP_Set625CGMS_A 0 \n");
    printf ("====>example : ACP_Set625CGMS_A 3 \n");
    printf ("16. ACP_SetMV [type] \n");
    printf ("-----[bEnable] :\n");
    printf ("---------------: 0 = disable\n");
    printf ("---------------: 1 = enable\n");
    printf ("-----[eType] :\n");
    printf ("---------------: MS_ACP_CVBS_NTSC_TYPE1    = 0x0 \n");
    printf ("---------------: MS_ACP_CVBS_NTSC_TYPE2    = 0x1 \n");
    printf ("---------------: MS_ACP_CVBS_NTSC_TYPE3    = 0x2 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE1     = 0x10 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE2     = 0x11 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE3     = 0x12 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE4     = 0x13 \n");
    printf ("---------------: MS_ACP_CVBS_PAL_TYPE5     = 0x14 \n");
    printf ("====>example : ACP_SetMV 1 0x2   //ENABLE MV NTSC TYPE2 \n");
    printf ("====>example : ACP_SetMV 1 0x14  //ENABLE MV PAL Test P02 \n");
    printf ("====>example : ACP_SetMV 0 0x14  //DISABLE MV PAL Test P02 \n");
    printf ("17. VE_SetWSS [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: MS_WSS_4x3_FULL               = 1 \n");
    printf ("---------------: MS_WSS_14x9_LETTERBOX_CENTER  = 2 \n");
    printf ("---------------: MS_WSS_14x9_LETTERBOX_TOP     = 3 \n");
    printf ("---------------: MS_WSS_16x9_LETTERBOX_CENTER  = 4 \n");
    printf ("---------------: MS_WSS_16x9_LETTERBOX_TOP     = 5 \n");
    printf ("---------------: MS_WSS_ABOVE16x9_LETTERBOX_CENTER = 6 \n");
    printf ("---------------: MS_WSS_14x9_FULL_CENTER       = 7 \n");
    printf ("---------------: MS_WSS_16x9_ANAMORPHIC        = 8 \n");
    printf ("====>example : VE_SetWSS 1     //MS_WSS_4x3_FULL \n");
    printf ("====>example : VE_SetWSS 4     //MS_WSS_16x9_LETTERBOX_CENTER \n");
    printf ("18. VE_Set525WSS [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: MS_WSS_525_4x3_NORMAL         = 1 \n");
    printf ("---------------: MS_WSS_525_4x3_LETTERBOX      = 2 \n");
    printf ("---------------: MS_WSS_525_16x9_ANAMORPHIC    = 3 \n");
    printf ("---------------: MS_WSS_525_RESERVED           = 4 \n");
    printf ("====>example : VE_Set525WSS 1     //MS_WSS_525_4x3_NORMAL \n");
    printf ("====>example : VE_Set525WSS 3     //MS_WSS_525_16x9_ANAMORPHIC \n");
    printf ("19. ACP_Set625CGMS_A [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: E_CGMS_A_COPY_FREELY      = 0 \n");
    printf ("---------------: E_CGMS_A_COPY_NO_MORE     = 1 \n");
    printf ("---------------: E_CGMS_A_COPY_ONCE        = 2 \n");
    printf ("---------------: E_CGMS_A_COPY_NEVER       = 3 \n");
    printf ("====>example : ACP_Set625CGMS_A 0   //E_CGMS_A_COPY_FREELY \n");
    printf ("====>example : ACP_Set625CGMS_A 3   //E_CGMS_A_COPY_NEVER \n");
    printf ("====>example : ACP_SetMV 0 20  //DISABLE MV PAL Test P02 \n");
    printf ("20. ACP_Set525CGMS_A_IEC61880 [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: E_CGMS_A_COPY_FREELY      = 0 \n");
    printf ("---------------: E_CGMS_A_COPY_NO_MORE     = 1 \n");
    printf ("---------------: E_CGMS_A_COPY_ONCE        = 2 \n");
    printf ("---------------: E_CGMS_A_COPY_NEVER       = 3 \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 0   //E_CGMS_A_COPY_FREELY \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 3   //E_CGMS_A_COPY_NEVER \n");
    printf ("21. ACP_Set525APS_IEC61880 [type] \n");
    printf ("-----[eType] :\n");
    printf ("---------------: E_APS_NO_APS              = 0 \n");
    printf ("---------------: E_APS_PSP_ONLY            = 1 \n");
    printf ("---------------: E_APS_PSP_2_LINE_CB       = 2 \n");
    printf ("---------------: E_APS_PSP_4_LINE_CB       = 3 \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 1   //E_APS_NO_APS \n");
    printf ("====>example : ACP_Set525CGMS_A_IEC61880 3   //E_APS_PSP_4_LINE_CB \n");
    printf ("22. XC_StatusMonitor [type] \n");
    printf ("-----[bEnable] :\n");
    printf ("---------------: 0 = disable\n");
    printf ("---------------: 1 = enable\n");
    printf ("====>example : XC_StatusMonitor 1   //ENABLE Status Monitor \n");
    printf ("====>example : XC_StatusMonitor 0   //DISABLE Status Monitor \n");
    printf ("23. XC_ForcePrescaling [type] \n");
    printf ("====>example : XC_ForcePrescaling 0    //No Force Pre-Scaling \n");
    printf ("====>example : XC_ForcePrescaling 2    //Force Pre-Scaling HALF\n");
    printf ("====>example : XC_ForcePrescaling 4    //Force Pre-Scaling QUARTER\n");
    printf ("24. UART_Stop [type] \n");
    printf ("-----[bEnable] :\n");
    printf ("---------------: 0 = disable\n");
    printf ("---------------: 1 = enable\n");
    printf ("====>example : UART_Stop 1     //Stop uart for debugging \n");
    printf ("25. XC_EnableStillImage [bEnable] \n");
    printf ("-----[bEnable] :\n");
    printf ("---------------: 0 = disable\n");
    printf ("---------------: 1 = enable\n");
    printf ("====>example : XC_EnableStillImage 1   //Enable still image for zapping \n");
    printf ("26. XC_EnableCropZoomMode [bEnable] [initX] [initY] [width] [height] \n");
    printf ("-----[bEnable] :\n");
    printf ("---------------: 0 = disable\n");
    printf ("---------------: 1 = enable\n");
    printf ("-----[initX] : Initial start x position\n");
    printf ("---------------: 1 = enable\n");
    printf ("-----[initY] : Initial start y position\n");
    printf ("-----[width] : Crop-zoom window width\n");
    printf ("---------------: 1 = enable\n");
    printf ("-----[height] : Crop-zoom window height\n");
    printf ("====>example : XC_EnableCropZoomMode 1 100 100 300 300   //Enable crop-zoom window with start at (100,100) and size (300,300) \n");
    printf ("27. XC_MoveCropZoomWin [direction] [distance] [step] \n");
    printf ("-----[direction] :\n");
    printf ("---------------: 0 = up\n");
    printf ("---------------: 1 = down\n");
    printf ("---------------: 2 = lefp\n");
    printf ("---------------: 3 = right\n");
    printf ("-----[distance] : Distance of one moving step\n");
    printf ("-----[step] : Total steps of moving\n");
    printf ("====>example : XC_MoveCropZoomWin 3 10 20  //Move crop-zoom window 20 steps to right, one step is 10 pixels \n");

    return TRUE;
}
