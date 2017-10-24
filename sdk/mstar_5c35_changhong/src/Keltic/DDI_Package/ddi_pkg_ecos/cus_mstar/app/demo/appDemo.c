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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo.c
/// @author MStar Semiconductor Inc.
/// @brief  Uranus Sample Code Command Shell
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo.c
   @brief The sample code for the MDrv_XXX functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

//#include <linux/string.h>
#include <ctype.h>
#include <cyg/fileio/fileio.h>

#include "string.h"
#include "MsCommon.h"
#include "Board.h"
#include "MsFS.h"
#include "drvSYS.h"
#include "drvMMIO.h"
#include "drvUART.h"

#include "appDemo.h"
#include "appDemo_SMC.h"
#include "appDemo_XC.h"
#include "appDemo_DMX.h"
#include "appDemo_DigiTuner.h"

#include "appDemo_GE.h"
#include "appDemo_GOP.h"
#include "appDemo_AV.h"
#include "appDemo_Input.h"
#include "appDemo_Video.h"
#include "SysInit.h"
#include "appDemo_Audio.h"


#include "appDemo_Gpio.h"
#include "appDemo_Scart.h"
#include "appDemo_SerialFlash.h"
#include "appDemo_Keypad.h"
#include "appDemo_FrontPnl.h"
#include "appDemo_PM.h"
#include "appDemo_JPEG.h"
#include "appDemo_WDT.h"
#include "appTemplate.h"
#include "appDemo_MsOSQueue.h"
//#include "ver_dtv_comm.h"
#ifdef ENABLE_MM_PACKAGE
#include "appDemo_MM.h"
#endif
#ifdef ENABLE_PVR_PACKAGE
#include "appDemo_PVR.h"
#endif /*ENABLE_PVR_PACKAGE*/
#include "appDemo_XMLParser.h"
#include "appDemo_CIPHER.h"
#include "appDemo_RSA.h"
#include "appDemo_Zapper.h"

#include "appDemo_USB.h"
#include "appDemo_JFFS2.h"
#include "appDemo_DSCMB.h"
#include "appDemo_7Segments.h"

#include "appDemo_CH34.h"

#include <cyg/cpuload/cpuload.h>

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------

//verify RX via Echo or Capture File
#define VERIFY_RX_VIA_ECHO
#define MAX_CMD_SIZE   1024 //file size < 100KB
#define DEMO_CMD_READY 0x1
#define DEMO_CMD_END  0x2
#define DEMO_CMD_ALL   (DEMO_CMD_READY | DEMO_CMD_END)
#define TASK_CPULOAD_STACKSIZE 0x2000
#ifdef ENABLE_PVR_PACKAGE
#define MAX_CMD_PARAMETER 14
#else
#define MAX_CMD_PARAMETER 8
#endif /*ENABLE_PVR_PACKAGE*/
#define EN_ESC
#define EASY_CMD    1
#define CMD_DELIM       "\n"

#define DEMO_EVENT_WAIT_TIME 100
#define DEMO_EVENT_WAIT_FOREVER 0xffffffff

#ifdef ENABLE_PVR_PACKAGE

typedef MS_BOOL (*func_ptr)(void* u32Input0, void* u32Input1, void* u32Input2, void* u32Input3, void* u32Input4, void* u32Input5, void* u32Input6, void* u32Input7, void* u32Input8, void* u32Input9, void* u32Input10);

typedef struct _DemoFun
{
    MS_U8 u8CMDName[32];
    MS_U8 u8InputMask; //1: input string(MS_U8*), 0: input (MS_U16*);
                    //example: u8InputMask = 0x03, u32Input0,u32Input1  = (char*), u32Input2,u32Input3,u32Input4 = (MS_U16*)
    MS_BOOL (*func_ptr)(void* u32Input0, void* u32Input1, void* u32Input2, void* u32Input3, void* u32Input4, void* u32Input5, void* u32Input6, void* u32Input7, void* u32Input8, void* u32Input9, void* u32Input10);
} DemoFun,*pDemoFun;

#else  /*!ENABLE_PVR_PACKAGE*/

typedef MS_BOOL (*func_ptr)(void* u32Input0, void* u32Input1, void* u32Input2, void* u32Input3, void* u32Input4, void* u32Input5);


typedef struct _DemoFun
{
    MS_U8 u8CMDName[32];
    MS_U8 u8InputMask; //1: input string(MS_U8*), 0: input (MS_U16*);
                    //example: u8InputMask = 0x03, u32Input0,u32Input1  = (char*), u32Input2,u32Input3,u32Input4 = (MS_U16*)
    MS_BOOL (*func_ptr)(void* u32Input0, void* u32Input1, void* u32Input2, void* u32Input3, void* u32Input4, void* u32Input5);
} DemoFun,*pDemoFun;
#endif /*ENABLE_PVR_PACKAGE*/
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_PVR_PACKAGE
MS_U32 gu32Input0, gu32Input1, gu32Input2, gu32Input3, gu32Input4, gu32Input5, gu32Input6, gu32Input7, gu32Input8, gu32Input9, gu32Input10;

void *gInput0, *gInput1, *gInput2, *gInput3, *gInput4, *gInput5, *gInput6, *gInput7, *gInput8, *gInput9, *gInput10;

#else /*!ENABLE_PVR_PACKAGE*/
MS_U32 gu32Input0, gu32Input1, gu32Input2, gu32Input3, gu32Input4, gu32Input5;

void *gInput0, *gInput1, *gInput2, *gInput3, *gInput4, *gInput5;
#endif /* ENABLE_PVR_PACKAGE */

MS_U8   g_u8CMDArray[MAX_CMD_SIZE];
MS_U8   *g_argv[MAX_CMD_PARAMETER+1];
MS_U8   *g_NullStr = NULL;

extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;

static MS_U8 _u8TaskCPULoad[TASK_CPULOAD_STACKSIZE];
MS_S32 s32CPULoadID = -1;
cyg_uint32 calibration;
cyg_cpuload_t cpuload;
cyg_handle_t cpuhandle;
cyg_uint32 average_point1s;
cyg_uint32 average_1s;
cyg_uint32 average_10s;

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_PVR_PACKAGE  /* the metadata is about 3k */
static Task_Info _appDemo_Task = {-1, E_TASK_PRI_HIGH, NULL, (0x8000), "Demo Task"};
#else
static Task_Info _appDemo_Task = {-1, E_TASK_PRI_HIGH, NULL, 0x8000, "Demo Task"};
#endif

static Task_Info _appDemoMon_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Demo Mon Task"};


static MS_S32  _s32DemoEventId = 0;
static MS_S32  _s32DemoMonEventId = 0;
static MS_U8 _argc;

extern MS_BOOL appDemo_UART_Uart2Test(void);

static DemoFun _DemoFun[]=
{

    {"test",            0x00,  (func_ptr)&TestFun   },
    {"test1",           0x03,  (func_ptr)&TestStrFun},
    {"help",            0x00,  (func_ptr)&HelpFun   },
    {"HelpGOP",         0x00,  (func_ptr)&GOP_Help },
    {"HelpGE",          0x00,  (func_ptr)&GE_Help_Part0 },
    {"HelpGE_More",     0x00,  (func_ptr)&GE_Help_Part1},
    {"HelpTV",          0x00,  (func_ptr)&DemoAV_Help_TV},
    {"HelpXC",          0x00,  (func_ptr)&appDemo_XC_Help},
    {"HelpVideo",          0x00,  (func_ptr)&appDemo_Video_Help},

     //XC Func
    {"XC_Init",        0x00,   (func_ptr)&appDemo_XC_Init},
    {"VE_Init",        0x00,   (func_ptr)&appDemo_VE_Init},
    {"XC_SetOutputTiming",        0x00,   (func_ptr)&appDemo_XC_SetOutputTiming},
    {"VE_SetOutputTiming",        0x00,   (func_ptr)&appDemo_VE_SetOutputTiming},
    {"XC_PlayVideo_UsingDefaultSetting",        0x00,   (func_ptr)&appDemo_XC_PlayVideo_UsingDefaultSetting},
    {"XC_Exit",        0x00,   (func_ptr)&appDemo_XC_Shutdown},
    {"XC_SetFrameBufferSize",        0x00,   (func_ptr)&appDemo_XC_SetFrameBufferSize},
    {"XC_SetFrameColor",        0x00,   (func_ptr)&appDemo_XC_SetFrameColor},
    {"VE_VbiTtxDemo",        0x00,   (func_ptr)&appDemo_VE_VbiTtxDemo},
    {"XC_EnableDrvDmsg",        0x00,   (func_ptr)&appDemo_XC_EnableDmesg},
    {"XC_SetBrightness", 0x0, (func_ptr)&appDemo_XC_SetBrightness},
    {"XC_SetSharpness", 0x0, (func_ptr)&appDemo_XC_SetSharpness},
    {"XC_SetContrast", 0x0, (func_ptr)&appDemo_XC_SetContrast},
    {"XC_SetHue", 0x0, (func_ptr)&appDemo_XC_SetHue},
    {"XC_SetSaturation", 0x0, (func_ptr)&appDemo_XC_SetSaturation},
    {"VE_CCIROut", 0x0, (func_ptr)&appDemo_VE_CCIROut},
    {"XC_PlayVideo_ScalingDown", 0x0, (func_ptr)&appDemo_XC_PlayVideo_ScalingDown},
    {"XC_PlayVideo_ScalingUp", 0x0, (func_ptr)&appDemo_XC_PlayVideo_ScalingUp},
    {"ACP_Set625CGMS_A", 0x0, (func_ptr)&appDemo_ACP_Set625CGMS_A},
    {"ACP_SetMV",       0x0,     (func_ptr)&appDemo_ACP_SetMV},
    {"ACP_SetDCS",      0x00,    (func_ptr)&appDemo_ACP_SetDCS},
    {"XC_ForcePrescaling", 0x0, (func_ptr)&appDemo_XC_ForcePrescaling},
    {"XC_StatusMonitor", 0x0, (func_ptr)&appDemo_XC_StatusMonitor},
    {"VE_SetWSS",        0x00,   (func_ptr)&appDemo_VE_SetWSS},
    {"VE_Set525WSS", 0x0, (func_ptr)&appDemo_VE_Set525WSS},
    {"ACP_Set525CGMS_A_IEC61880", 0x0, (func_ptr)&appDemo_ACP_Set525CGMS_A_IEC61880},
    {"ACP_Set525APS_IEC61880", 0x0, (func_ptr)&appDemo_ACP_Set525APS_IEC61880},
    {"XC_EnableStillImage", 0x0, (func_ptr)&appDemo_XC_EnableStillImage},
    {"XC_EnableFreezeImage", 0x0, (func_ptr)&appDemo_XC_EnableFreezeImage},
    {"XC_EnableCropZoomMode", 0x0, (func_ptr)&appDemo_XC_EnableCropZoomMode},
    {"XC_MoveCropZoomWin", 0x0, (func_ptr)&appDemo_XC_MoveCropZoomWin},
    {"XC_GenerateBlackVideo", 0x0, (func_ptr)&appDemo_XC_GenerateBlackVideo},

    //DigiTuner Func
    {"DigiTuner_SetIndex",        0x00,   (func_ptr)&appDemo_DigiTuner_SetIndex},
    {"DigiTuner_SetFreq",         0x00,   (func_ptr)&appDemo_DigiTuner_SetFreq},
    {"FE_SetDetectMode",         0x00,   (func_ptr)&appDemo_Frontend_SetDetectMode},
    {"FE_SetBroadcastType",      0x00,   (func_ptr)&appDemo_Frontend_SetBroadcastType},
    {"FE_SetInExteralPath",      0x00,   (func_ptr)&appDemo_Frontend_SetInExteralPath},
    {"FE_SetFrontEnd",           0x00,   (func_ptr)&appDemo_Frontend_SetTunerDemod},
    #if(MS_DVBT2_INUSE == 1)
    {"DigiTuner_GetPlpInfo",        0x00,   (func_ptr)&appDemo_DigiTuner_GetPlpInfo},
    {"DigiTuner_SetMPLP",        0x00,   (func_ptr)&appDemo_DigiTuner_SetMPLP},
    #endif
    {"DigiTuner_GetLock",        0x00,   (func_ptr)&appDemo_DigiTuner_CheckLock},
    #if(MS_DVB_TYPE_SEL == DVBS)
    {"DigiTuner_BlindScan",         0x00,   (func_ptr)&appDemo_DigiTuner_BlindScan},
    {"DigiTuner_SetLNBPower",       0x00,   (func_ptr)&appDemo_DigiTuner_SetLNBPower},
    {"DigiTuner_Set22K",            0x00,   (func_ptr)&appDemo_DigiTuner_Satellite_Set22K},
    {"FE_DiseqcCmd",                0x00,   (func_ptr)&appDemo_FE_DiseqcCmd},
    #endif

    // Demo Dmux
    {"DMX_Help",       0x00,   (func_ptr)&appDemo_Dmx_help},
    {"DMX_Pat",        0x00,   (func_ptr)&appDemo_Dmx_Pat},
    {"DMX_Pmt",        0x00,   (func_ptr)&appDemo_Dmx_Pmt},
    {"DMX_Sec",        0x00,   (func_ptr)&appDemo_Dmx_Sec},
    {"DMX_Scan",       0x00,   (func_ptr)&appDemo_Dmx_Scan},
    {"DMX_Pcr",        0x00,   (func_ptr)&appDemo_Dmx_PCR},
    {"DMX_nmask",      0x07,   (func_ptr)&appDemo_Dmx_Nmask},
    {"DMX_PES",       0x00,    (func_ptr)&appDemo_Dmx_PES},
    {"DMX_Pkt",        0x00,   (func_ptr)&appDemo_Dmx_Packet},
    {"DMX_FileIn_Start",0x01,    (func_ptr)&appDemo_DmxFileIn_Start},
    {"DMX_FileIn_Stop", 0x00,    (func_ptr)&appDemo_DmxFileIn_Stop},
    {"DMX_TSO", 0x00,    (func_ptr)&appDemo_Dmx_TSO},

    //TV Demo Funcs
    {"TV_Tuner_Config", 0x0, (func_ptr)&DemoAV_Tuner_Config},
    {"TV_TSP_SetPid", 0x0, (func_ptr)&DemoAV_TSP_SetPid},
    {"TV_TSP_SetPid_File", 0x0, (func_ptr)&DemoAV_TSP_SetPVRPid},
    {"TV_MVD_GetVideoInfo", 0x0, (func_ptr)&DemoAV_MVD_GetVideoInfo},
    {"TV_MVD_DecIFrame", 0x0, (func_ptr)&DemoAV_MVD_DecIFrame},
    {"TV_PlayAV", 0x0, (func_ptr)&DemoAV_PlayAV},
    {"TV_PauseAV", 0x0, (func_ptr)&DemoAV_PauseAV},
    {"TV_StopAV", 0x0, (func_ptr)&DemoAV_StopAV},
    {"TV_Set_AVSync_Mode",  0x0, (func_ptr)&DemoAV_Set_AVSYNC_Mode},
    {"TV_MAD_SetVol", 0x0, (func_ptr)&DemoAV_MAD_SetVol},

    //Video Func
    {"Video_GetDecoderStatus",        0x00,   (func_ptr)&appDemo_Video_GetDecoderStatus},
    {"Video_SetOutputDest", 0x0, (func_ptr)&appDemo_Video_SetOutputDest},
    {"Video_DACDetect", 0x0, (func_ptr)&appDemo_Video_DACDetect},
    {"Video_HD_SetCropWin", 0x0, (func_ptr)&appDemo_Video_HD_SetCropWin},
    {"Video_HD_SetDestWin", 0x0, (func_ptr)&appDemo_Video_HD_SetDestWin},
    {"Video_HD_SetAS", 0x0, (func_ptr)&appDemo_Video_HD_SetAS},
    {"Video_HD_SetCusAsInfo",     0x0, (func_ptr)&appDemo_Video_HD_SetCusASInfo},
    {"Video_SD_SetSrcWin", 0x0, (func_ptr)&appDemo_Video_SD_SetSrcWin},
    {"Video_SD_SetDestWin", 0x0, (func_ptr)&appDemo_Video_SD_SetDestWin},
    {"Video_SD_SetAS", 0x0, (func_ptr)&appDemo_Video_SD_SetAS},
    {"Video_SD_SetCaptureMode", 0x0, (func_ptr)&appDemo_Video_SD_SetCaptureMode},
    {"Video_PlayVideo_UsingCusSetting", 0x0, (func_ptr)&appDemo_Video_PlayVideo_UsingCusSetting},//Video Path 1
    //{"Video_PlayVideo_ThroughPath2", 0x0, (func_ptr)&appDemo_Video_PlayVideo_ThroughPath2},//Video Path 2
    {"Video_HD_SetRFBLMode", 0x0, (func_ptr)&appDemo_Video_HD_SetRFBLMode},
    {"Video_Mute", 0x0, (func_ptr)&appDemo_Video_Mute},
    {"Video_CCIRIn", 0x0, (func_ptr)&appDemo_Video_CCIRIn},

    // Smart Card
    {"HelpSMC",         0x00,  (func_ptr)&appDemo_SMC_Help},
    {"SMC_Start",       0x00,   (func_ptr)&appDemo_SMC_Start},
    {"SMC_Stop",        0x00,   (func_ptr)&appDemo_SMC_Stop},

    // CIPHER
    {"CIPHER_Demo",0x00,(func_ptr)&appDemo_CIPHER_DoCIPHERDemo},
    
    // RSA
    {"RSA_SW_Demo",         0x00,(func_ptr)&appDemo_RSA_DoRSADemo},
    {"RSA_HW_Demo",         0x00,(func_ptr)&appDemo_RSA_DoRSADemo_HwKey},

    // Descrambler
    {"DSCMB_Help",          0x00, (func_ptr)&appDemo_DSCMB_Help},
    {"DSCMB_SetCAVID",      0x00, (func_ptr)&appDemo_DSCMB_SetCAVID},
    {"DSCMB_ConnectPID",    0x00, (func_ptr)&appDemo_DSCMB_ConnectPID},
    {"DSCMB_DisconnectPID", 0x00, (func_ptr)&appDemo_DSCMB_DisconnectPID},
    {"DSCMB_SetType",       0x02, (func_ptr)&appDemo_DSCMB_SetType},
    {"DSCMB_CW",            0x06, (func_ptr)&appDemo_DSCMB_CW},
    {"DSCMB_IV",            0x06, (func_ptr)&appDemo_DSCMB_IV},
    {"DSCMB_KeyLadderMode", 0x01, (func_ptr)&appDemo_DSCMB_KeyLadderMode},
    {"DSCMB_KeyLadder",     0x1e, (func_ptr)&appDemo_DSCMB_KeyLadder},


    //GE Demo Funs
    {"GE_Init",         0x00,   (func_ptr)&GE_Initialize},
    {"GE_SetConfig",      0x00,   (func_ptr)&GE_SetConfig},
    {"GE_SetColorFmt",      0x00,   (func_ptr)&GE_SetColorFmt},
    {"GE_CreateWin",      0x00,   (func_ptr)&GE_CreateWin},
    {"GE_FillBlock",    0x00,   (func_ptr)&GE_FillBlock},
    {"GE_DrawLine",    0x00,   (func_ptr)&GE_DrawLine},
    {"GE_DrawBmp",    0x00,   (func_ptr)&GE_DrawBmp},
    {"GE_DrawBmpRotate",    0x00,   (func_ptr)&GE_DrawBmpRotate},
    {"GE_DrawBmpScale",    0x00,   (func_ptr)&GE_DrawBmpScale},
    {"GE_ConvertColor",    0x00,   (func_ptr)&GE_ConvertColor},
    {"GE_ScreenCopyRop",    0x00,   (func_ptr)&GE_ScreenCopyRop},
    {"GE_BitBltSample",    0x00,   (func_ptr)&GE_BitBltSample},
    {"GE_TextOut",    0x00,   (func_ptr)&GE_TextOut},

    //GOP Demo Funs
    {"GOP_Init",         0x00,   (func_ptr)&GOP_Init},
    {"GOP_SetConfig",            0x00,   (func_ptr)&GOP_SetConfig},
    {"GOP_CreateWin",    0x00,   (func_ptr)&GOP_CreateWin},
    {"GOP_DrawBmp",    0x00,   (func_ptr)&GOP_DrawBmp},
    {"GOP_DWinCapture",    0x00,   (func_ptr)&GOP_DWinCapture},
    {"GOP_EnableWin",         0x00,   (func_ptr)&GOP_EnableWin},
    {"GOP_FreeWin",    0x00,   (func_ptr)&GOP_FreeWin},
    {"GOP_SetGlobalAlpha",         0x00,   (func_ptr)&GOP_SetGlobalAlpha},
    {"GOP_EnablePerPixelAlpha",    0x00,   (func_ptr)&GOP_EnablePerPixelAlpha},
    {"GOP_SetFade",    0x00,   (func_ptr)&GOP_SetFade},
    {"GOP_Zlist",    0x00,   (func_ptr)&GOP_Zlist},
    {"GOP_SetVideoAlpha",    0x00,   (func_ptr)&GOP_SetVideoAlpha},

   //Demo IR Func
    {"HelpIR",          0x00,  (func_ptr)&appDemo_Input_Help},
    {"IR_Init",         0x00,  (func_ptr)&appDemo_Input_Init   },
    {"IR_Exit",         0x00,  (func_ptr)&appDemo_Input_Exit   },


    //Audio Func
    {"Audio_Init",        0x00,   (func_ptr)&appDemo_Audio_Init},
    {"Audio_demo",        0x00,   (func_ptr)&appDemo_Audio_Out_Init},
    {"Audio_demoStop",        0x00,   (func_ptr)&appDemo_Audio_Stop_Demo},
    {"Audio_volume",        0x00,   (func_ptr)&appDemo_Audio_SetAbsoluteVolume},
    {"Audio_mute",        0x00,   (func_ptr)&appDemo_Audio_SetMute},
    {"Audio_SetOutputMode",        0x00,   (func_ptr)&appDemo_Audio_SetOutputMode},
    {"Audio_Monitor",        0x00,   (func_ptr)&appDemo_Audio_SetMonitor},
    {"Audio_SPDIF_Mode",        0x00,   (func_ptr)&appDemo_Audio_Set_SPDIF_Mode},
    {"Audio_Debug",        0x00,   (func_ptr)&appDemo_Audio_Debug},
    {"Audio_FMTX_start",        0x00,   (func_ptr)&appDemo_Audio_FMTX_Start},
    {"Audio_ATV_stop",        0x00,   (func_ptr)&appDemo_Audio_ATV_Stop},

    //Gpio Func
    {"Gpio_Init",        0x00,   (func_ptr)&appDemo_Gpio_Init},
    {"Gpio_Set",        0x00,   (func_ptr)&appDemo_Gpio_Set},
    {"Gpio_getLevel",        0x00,   (func_ptr)&appDemo_Gpio_GetLevel},
    {"Gpio_EnableInterrupt",        0x00,   (func_ptr)&appDemo_Gpio_EnableInterrupt},
    {"Gpio_DisableInterrupt",        0x00,   (func_ptr)&appDemo_Gpio_DisableInterrupt},

    //Demo Keypad Func
    {"Keypad_Init",         0x00,  (func_ptr)&appDemo_Keypad_Init   },

    //Demo Keypad Func
    {"FrontPnl_Init",         0x00,  (func_ptr)&appDemo_FrontPnl_Init   },
    {"FrontPnl_ShowEnable",        0x00,   (func_ptr)&appDemo_FrontPnl_ShowOpen_Enable},
    {"FrontPnl_DigitalSet",        0x00,   (func_ptr)&appDemo_FrontPnl_Digital_Set},
    #if (FRONTPNL_TYPE == FRONTPANEL_TYPE_PT6958)
    {"FrontPnl_LedEnable",        0x00,   (func_ptr)&appDemo_FrontPnl_LED_Enable},
    {"FrontPnl_DotEnable",        0x00,   (func_ptr)&appDemo_FrontPnl_Dot_Enable},
    #endif

    //Demo Serial Flash Func
    {"SerialFlash_Init",         0x00,  (func_ptr)&appDemo_SerFlash_Init},
    {"SerialFlash_ReadID",         0x00,  (func_ptr)&appDemo_SerFlash_ReadID},
    {"SerialFlash_GetInfo",         0x00,  (func_ptr)&appDemo_SerFlash_GetInfo},
    {"SerialFlash_RWTest",         0x00,  (func_ptr)&appDemo_SerFlash_RWTest},
    {"SerialFlash_Switch",         0x00,  (func_ptr)&appDemo_SerFlash_switch},

    //Demo PM Func
    {"PM_Init",                0x00,  (func_ptr)&appDemo_PM_Init},
    {"PM_PowerDown",           0x00,  (func_ptr)&appDemo_PM_PowerDown},
    {"PM_RTC_Init",         0x00,  (func_ptr)&appDemo_PM_RTC_Init},
    {"PM_RTC_GetTime",         0x00,  (func_ptr)&appDemo_PM_RTC_GetTime},
    {"PM_RTC_SetTime",         0x00,  (func_ptr)&appDemo_PM_RTC_SetTime},
    {"PM_RTC_SetMatchTime",    0x00,  (func_ptr)&appDemo_PM_RTC_SetMatchTime},
    {"PM_FastStandby",         0x00,  (func_ptr)&appDemo_FastStandby},

    // Demo JPEG Func
    {"JPEG_MAIN",         0x00,  (func_ptr)&verJPD_Main},
    {"JPEG_XC_DISPLAY",    0x00,	(func_ptr)&appDemo_JPEG_XC_Display},

    // Demo all kind's of zapping flow
    {"Zapper_Channel",         0x00,  (func_ptr)&appDemo_Zapper_Channel},// Demo Still Image Zapping
    {"Zapping_FQ_Init",         0x00,  (func_ptr)&appDemo_FQ_Init},//FQ Init
    {"Zapping_FQ_Exit",         0x00,  (func_ptr)&appDemo_FQ_Exit},//FQ Exit
    {"Zapping_FQ",         0x00,  (func_ptr)&appDemo_FQ_Zapping},// Demo FQ Zapping
    {"Zapper_Help",     0x00,   (func_ptr)&appDemo_Zapper_Help},

#ifdef ENABLE_MM_PACKAGE
    //Demo Multi-Media Func
    {"MM_INIT",	 0x00,	 (func_ptr)&appDemo_MM_Init},
    {"MM_PLAY",	 0x02,	 (func_ptr)&appDemo_MM_Play},
    {"GE_DrawPhoto",	 0x00,	 (func_ptr)&appDemo_MM_DrawPhoto},
    {"GE_SetFBFmt",	 0x00,	 (func_ptr)&appDemo_MM_Photo_SetGwinFmt},
    {"GE_ClearPhoto",	 0x00,	 (func_ptr)&appDemo_MM_ClearPhoto},
    {"MM_SubtitleTrack", 0x00,     (func_ptr)&appDemo_MM_SetSubtitleTrack},
    {"MM_AudioTrack", 0x00,     (func_ptr)&appDemo_MM_SetAudioTrack},
    {"MM_FF",	   0x00,   (func_ptr)&appDemo_MM_FF},
    {"MM_RW",	   0x00,   (func_ptr)&appDemo_MM_RW},
    {"MM_PAUSE",	  0x00,   (func_ptr)&appDemo_MM_Pause},
    {"MM_RESUME",	   0x00,   (func_ptr)&appDemo_MM_Resume},
    {"MM_STOP",	 0x00,	 (func_ptr)&appDemo_MM_Stop},
    {"MM_Repeat",	 0x00,	 (func_ptr)&appDemo_MM_Repeat},
    {"MM_RW_Replay",	 0x00,	 (func_ptr)&appDemo_MM_RW_Replay},
    {"MM_SEEK",	 0x00,	 (func_ptr)&appDemo_MM_Seek},
    {"MM_GETOPTION",	  0x00,   (func_ptr)&appDemo_MM_GetOption},
    {"MM_PlayVideo_UsingDefaultSetting",        0x00,   (func_ptr)&appDemo_XC_PlayVideo_UsingDefaultSetting},
    {"MM_DEINIT",	 0x00,	 (func_ptr)&appDemo_MM_DeInit},
#endif

#ifdef ENABLE_PVR_PACKAGE
    {"PVR_Help",      0x00, (func_ptr)&appDemo_PVR_Help},
    {"PVR_Init",      0x0E, (func_ptr)&appDemo_PVR_Init},
    {"PVR_Exit",      0x00, (func_ptr)&appDemo_PVR_Destroy},
    {"PVR_SetIdx",    0x00, (func_ptr)&appDemo_PVR_SetIdx},
    {"PVR_GetIdx",    0x00, (func_ptr)&appDemo_PVR_GetIdx},
    {"PVR_RecordStart",      0x20, (func_ptr)&appDemo_PVR_Record_Start},
    {"PVR_RecordPause",      0x00, (func_ptr)&appDemo_PVR_Record_Pause},
    {"PVR_RecordResume",      0x00, (func_ptr)&appDemo_PVR_Record_Resume},
    {"PVR_RecordStop",      0x00, (func_ptr)&appDemo_PVR_Record_Stop},
    {"PVR_PlaybackStart",      0x20, (func_ptr)&appDemo_PVR_PlaybackStart},
    {"PVR_PlaybackStop",      0x00, (func_ptr)&appDemo_PVR_PlaybackStop},
    {"PVR_PlaybackJumpToTime",      0x00, (func_ptr)&appDemo_PVR_PlaybackJumpToTime},
    {"PVR_TimeshiftRecordStart",    0x20, (func_ptr)&appDemo_PVR_Timeshift_RecordStart},
    {"PVR_TimeshiftRecordStop",     0x00, (func_ptr)&appDemo_PVR_Timeshift_RecordStop},
    {"PVR_TimeshiftPlaybackStart",  0x00, (func_ptr)&appDemo_PVR_Timeshift_PlaybackStart},
    {"PVR_TimeshiftPlaybackStop",   0x00, (func_ptr)&appDemo_PVR_Timeshift_PlaybackStop},
    {"PVR_TimeshiftStop",           0x00, (func_ptr)&appDemo_PVR_Timeshift_Stop},
    {"PVR_PlaybackSF",      0x00, (func_ptr)&appDemo_PVR_PlaybackSlowForward},
    {"PVR_PlaybackFF",      0x00, (func_ptr)&appDemo_PVR_PlaybackFastForward},
    {"PVR_PlaybackPause",      0x00, (func_ptr)&appDemo_PVR_PlaybackPause},
    {"PVR_PlaybackResume",      0x00, (func_ptr)&appDemo_PVR_PlaybackResume},
    {"PVR_PlaybackFB",      0x00, (func_ptr)&appDemo_PVR_PlaybackFastBackward},
    {"PVR_GetFileInfo",           0x01, (func_ptr)&appDemo_PVR_GetFileInfo},
    {"PVR_StepInPlay",           0x00, (func_ptr)&appDemo_PVR_PlaybackStepIn},
    {"PVR_PlaybackWithNormalSpeed",           0x00, (func_ptr)&appDemo_PVR_PlaybackWithNormalSpeed},
    {"PVR_CaptureThumbnail",      0x01, (func_ptr)&appDemo_PVR_CaptureThumbnail},
    {"PVR_JumpToThumbnail",      0x01, (func_ptr)&appDemo_PVR_JumpToThumbnail},
    {"PVR_SpeedCheck",           0x00, (func_ptr)&appDemo_PVR_CheckUSBSpeed},
    {"PVR_HideThumbnail",           0x00, (func_ptr)&appDemo_PVR_HideThumbnail},
    {"PVR_SetABLoop",           0x00, (func_ptr)&appDemo_PVR_PlaybackABLoop},
    {"PVR_ResetABLoop", 0x00,(func_ptr)&appDemo_PVR_PlaybackResetABLoop},
    {"PVR_SetSkipTime",           0x00, (func_ptr)&appDemo_PVR_PlaybackAddSkipTime},
    {"PVR_RemoveSkipTime", 0x00, (func_ptr)&appDemo_PVR_PlaybackRemoveSkipTime},
    {"PVR_SetRetentionTime", 0x0,(func_ptr)&appDemo_PVR_SetPlaybackRetentionLimit},
    {"PVR_PlayJumpForward", 0x00, (func_ptr)&appDemo_PVR_PlaybackJumpForward},
    {"PVR_PlayJumpBackward", 0x0,(func_ptr)&appDemo_PVR_PlaybackJumpBackward},
    {"PVR_ShowTime", 0x00, (func_ptr)&appDemo_PVR_ShowTime},
    {"PVR_RemoveFile", 0x01, (func_ptr)&appDemo_PVR_RemoveFile},
    {"PVR_Rename", 0x03, (func_ptr)&appDemo_PVR_RenameFile},
    {"PVR_GetTimeshiftRecStartTime", 0x00, (func_ptr)&appDemo_PVR_GetTimeshiftRecStartTime},
    {"PVR_SetTimeshiftRecScreenFrozen", 0x00, (func_ptr)&appDemo_PVR_SetTimeshiftRecScreenFrozen},
    {"PVR_EnableStillImageZapping", 0x00, (func_ptr)&appDemo_PVR_EnableStillImageZapping},
    {"PVR_SetDBGLevel",    0x00, (func_ptr)&appDemo_PVR_SetDBGLevel},
    {"PVR_SetTimeshiftRecDuration", 0x00, (func_ptr)&appDemo_PVR_SetTimeshiftRecDuration},
#endif /* end of ENABLE_PVR_PACKAGE */
    // XML Parsrt
    {"XML_Demo",0x00,(func_ptr)&Demo_XML_Parser},
    // USB Part
    {"USB_Fdisk",0x01,(func_ptr)&appDemo_USB_Fdisk},
    {"USB_Format",0x07,(func_ptr)&appDemo_USB_Format},
    {"USB_Umount",0x01,(func_ptr)&appDemo_USB_Umount},
    {"USB_Mount",0x07,(func_ptr)&appDemo_USB_Mount},
    // JFFS2 Part
    {"JFFS2_Mount",0x03,(func_ptr)&appDemo_JFFS2_Mount},
    {"JFFS2_GetFreeSectors",0x01,(func_ptr)&appDemo_JFFS2_GetFreeSectors},

    {"7SegmentSet", 0x00 , (func_ptr)&Demo_DisplayDigiNum},
    {"7SegmentStartTime", 0x00 , (func_ptr)&Demo_DisplayTime_Start},
    {"7SegmentStopTime", 0x00 , (func_ptr)&Demo_DisplayTime_Stop},

    {"CPU_LOAD_MONITOR", 0x00 , (func_ptr)&appDemo_CPULoad},
    {"CPU_LOAD_MONITOR_OFF", 0x00 , (func_ptr)&appDemo_CPULoad_OFF},

	{"CH34_Init", 		0x00,	(func_ptr)&appDemo_CH34_Init},
	{"CH34_SetMode",	  0x00,   (func_ptr)&appDemo_CH34_SetMode},
	{"CH34_Help",	  0x00,   (func_ptr)&appDemo_CH34_Help},

};

static MS_U16 NumOfDemoFun = sizeof(_DemoFun) / sizeof(DemoFun);


//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
//static MS_BOOL _appDemo_Reset(void);

static void _CPULoadTaskStart (void)
{
    cyg_cpuload_calibrate(&calibration);
    cyg_cpuload_create(&cpuload,calibration,&cpuhandle);

    while(1)
    {
        MsOS_DelayTask(100);

        cyg_cpuload_get(cpuhandle,&average_point1s,&average_1s,&average_10s);
        printf("@@--%s--average_point1s = %d% ; average_1s = %d% ; average_10s = %d%\n", __func__, average_point1s, average_1s, average_10s);
    }

}

MS_BOOL appDemo_CPULoad(void)
{
    if (s32CPULoadID < 0)
    {
        memset(_u8TaskCPULoad,0,sizeof(MS_U8)*TASK_CPULOAD_STACKSIZE);

        s32CPULoadID = MsOS_CreateTask((TaskEntry) _CPULoadTaskStart,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_HIGH,
                                        TRUE,
                                        _u8TaskCPULoad,
                                        TASK_CPULOAD_STACKSIZE,
                                        "CPULoad" );
    }
    return TRUE;
}

MS_BOOL appDemo_CPULoad_OFF(void)
{
    if (s32CPULoadID > 0)
    {
        MsOS_DeleteTask(s32CPULoadID);
        s32CPULoadID = -1;
        cyg_cpuload_delete(cpuhandle);

    }
    return TRUE;
}

MS_U8 appDemo_toupper(MS_U8 input)
{

    if( input>='a' && input<='z')
    {
        return (input = input +'A' - 'a');
    }
    else
    {
        return input;
    }

}


MS_BOOL TestFun(MS_U32 *a, MS_U32 *b, MS_U32 *c, MS_U32 *d, MS_U32 *e)
{

    printf ("Test Fun Input 0 = %lx\n", *a);
    printf ("Test Fun Input 1 = %lx\n", *b);
    printf ("Test Fun Input 2 = %lx\n", *c);
    printf ("Test Fun Input 3 = %lx\n", *d);
    printf ("Test Fun Input 4 = %lx\n", *e);


    printf ("Test end\n", *e);
    return TRUE;

}


MS_BOOL TestStrFun(MS_U8 *a, MS_U8 *b, MS_U32 *c, MS_U32 *d, MS_U32 *e)
{

    printf ("TestStrFun Fun Input 0 = %s\n", a);
    printf ("TestStrFun Fun Input 1 = %s\n", b);
    printf ("TestStrFun Fun Input 2 = %lx\n", *c);
    printf ("TestStrFun Fun Input 3 = %lx\n", *d);
    printf ("TestStrFun Fun Input 4 = %lx\n", *e);
    return TRUE;

}

MS_BOOL HelpFun(void)
{
    printf ("------------------------------------help--------------------------------------\n");
    printf ("press \"HelpTemplate\" to show demo Template cmds\n");
    printf ("press \"Help\" to show Help\n");
    printf ("press \"HelpGOP\" to show demo GOP cmds\n");
    printf ("press \"HelpGE\" to show demo GE cmds\n");
    printf ("press \"HelpGE_More\" to show more demo GE cmds\n");
    printf ("press \"HelpTV\" to show demo AV cmds\n");
    printf ("press \"HelpTV_More\" to show more demo AV cmds\n");
    printf ("press \"HelpXC\" to show demo XC cmds\n");
    printf ("press \"DMX_Help\" to show demo DMX cmds\n");
    printf ("press \"HelpVideo\" to show demo Video cmds\n");
    printf ("press \"HelpSMC\" to show demo smart card cmds\n");
    printf ("press \"HelpIR\" to show demo IR cmds\n");
    printf ("press \"HelpAudio\" to show demo Audio cmds\n");
#ifdef ENABLE_PVR_PACKAGE
    printf ("press \"PVR_HELP\" to show demo PVR cmds\n");
#endif /* ENABLE_PVR_PACKAGE */
    printf ("---------------------------------End of help----------------------------------\n");
    return TRUE;
}

/***************************************************************************************/

void _MApp_FONT_unicode_2_ascii(MS_U16 *in, MS_U8 *out)
{
    MS_U8 *pout = out;
    MS_BOOL byUpperU8 = FALSE;
    while( *in){
        *pout = (MS_U8)*in;
        if ((*in) < 0x80 && byUpperU8 == FALSE)
        {
            *pout = (MS_U8)*in;
        }
        else
        {
            byUpperU8 = TRUE;
            *pout = (MS_U8)*in;
            pout++;
            if(*in>>8 != 0)
            {
                *pout = (MS_U8)(*in>>8);
            }
            else
            {
                break;
            }
        }
        ++in;
        ++pout;
    }
    *pout = 0x00;
}

/***************************************************************************************/

// example: read_dir_files("/fs");
void read_dir_files(char * fsPath)
{
    char *mountPath = fsPath ;
    DIR *dp = NULL;
    if((dp = (DIR *)MsFS_OpenDir(mountPath)) == NULL)
    {
        printf("[%s][%d]open dir fail\n", __FUNCTION__, __LINE__);
        return ;
    }
    printf("MsFS_OpenDir , dp=%x\r\n",dp);
    struct dirent* entry = NULL;
    MS_U32 count = 0;
    if(dp != NULL)
    {
        while((entry = (struct dirent*)MsFS_ReadDir(dp))!=NULL)
        {
            count++;
            MS_U8 out[sizeof(entry->d_name)];
            memset(out, 0, sizeof(entry->d_name));
            _MApp_FONT_unicode_2_ascii((MS_U16*)entry->d_name, out);
            printf("%s\n", out);
        }
        printf("total count:%d\n", count);
        closedir(dp);
    }
}

/***************************************************************************************/
MS_U8 *GetNextWhiteChar(MS_U8 *sptr)
{
    while(1)
    {
        if(*sptr == 0)
        {
            return (sptr);
        }

        if(*sptr !=' ' && *sptr != '\t' && *sptr !=',')
        {
            sptr ++;
            continue;
        }

        return(sptr);
    }
}

/***************************************************************************************/

MS_U8* GetNextNonWhiteChar(MS_U8 *sptr)
{
    while(1)
    {
        if(*sptr==' ' || *sptr=='\t' || *sptr==',')
        {
            *sptr = 0;
            sptr ++;
            continue;
        }

        return(sptr);
    }
}

/***************************************************************************************/

void GetCmdArgv(MS_U8 *_lineBuf, MS_U8 *_argc, MS_U8 **argv)
{
    MS_U8  *sptr;
    int argc,argno;
    MS_U8  seperator = 0;

    /*[01]reset argc & argv ===========================================================*/
    argc = 0;

    for(argno=0;argno<MAX_CMD_PARAMETER;argno++)
    {
        argv[argno] = NULL;
    }

    /*[02]get command arguments =======================================================*/
    sptr = _lineBuf;
    sptr = GetNextNonWhiteChar(sptr);
    argv[argc] = sptr;

    while(1)
    {
        if(*sptr == 0) // end of line
        {
            *_argc = argc;
            return;
        }

        if(seperator == 0 && *sptr == ',')  // for single seprator
        {
            *sptr = 0;
            sptr ++;            // move to next char
            seperator = 1;      // set seperator flag

            sptr = GetNextWhiteChar(sptr);
            sptr = GetNextNonWhiteChar(sptr);
            continue;
        }
        else if(seperator == 1 && *sptr == ',') // for continuos seperator
        {
            *sptr = 0;
            argc ++;
            argv[argc] = sptr;

            sptr ++;            // move to next char

            sptr = GetNextWhiteChar(sptr);
            sptr = GetNextNonWhiteChar(sptr);
            continue;
        }
        else
        {
            seperator = 0;
        }

        argc ++;
        if(argc > MAX_CMD_PARAMETER)
        {
            printf ("Warring!! :too many parameter \n ");
            return;
        }
        sptr = GetNextWhiteChar(sptr);
        sptr = GetNextNonWhiteChar(sptr);
        argv[argc] = sptr;
    }
}


/***************************************************************************************/
MS_U32 DHConvert(char *string, int parano)
{
    MS_U16 charno,totalchar;
    MS_U32 value;

    /*[01]error condition =============================================================*/
    if(string==NULL)
    {
        return 0;
    }

    /*[02]get total legal character ===================================================*/
    charno=0;
    while((string[charno]!=10 && string[charno]!=13 && string[charno]!=0) || (charno==10))
    {
        charno++;
    }
    string[charno]=0;

    /*[03-1]hex format ================================================================*/
    if((string[0]=='0') && (string[1]=='X'||string[1]=='x'))
    {
        value = 0;
        for(charno=2; charno<strlen(string); charno++)
        {
            if(string[charno]==' '||string[charno]=='\t')
            {
                break;
            }
            value <<=4;
            if('0'<=string[charno]&&string[charno]<='9')
            {
                value += (string[charno]-'0');
            }
            else if('a'<=string[charno]&&string[charno]<='f')
            {
                value += (string[charno]- 'a' + 10);
            }
            else if('A'<=string[charno]&&string[charno]<='F')
            {
                value += (string[charno]- 'A' + 10);
            }
        }

        return(value);
    }

    /*[03-2]dec format ================================================================*/
    else
    {
        if(strlen(string)>10)
        {
            return (MS_U32)(-1);
        }

        value = 0;
        totalchar = strlen(string);
        for(charno=0; charno<totalchar; charno++)
        {
            if((totalchar-charno)==10)
            {
                if ((string[charno] - '0') > 4)
                    return 0xFFFFFFFF;
                else
                    value += (string[charno] - '0')*1000000000L;
            }
            if((totalchar-charno)==9)
            {
                value += (string[charno] - '0')*100000000L;
            }
            if((totalchar-charno)==8)
            {
                value += (string[charno] - '0')*10000000L;
            }
            if((totalchar-charno)==7)
            {
                value += (string[charno] - '0')*1000000L;
            }
            if((totalchar-charno)==6)
            {
                value += (string[charno] - '0')*100000L;
            }
            if((totalchar-charno)==5)
            {
                value += (string[charno] - '0')*10000L;
            }
            if((totalchar-charno)==4)
            {
                value += (string[charno] - '0')*1000L ;
            }
            if((totalchar-charno)==3)
            {
                value += (string[charno] - '0')*100L  ;
            }
            if((totalchar-charno)==2)
            {
                value += (string[charno] - '0')*10L   ;
            }
            if((totalchar-charno)==1)
            {
                value += (string[charno] - '0')*1L    ;
            }
        }

        return(value);
    }
}



static int _appDemo_strcmp(const char *string1, const char *string2)
{
    int iRet, i;

    i = 0;
    while(string1[i] || string2[i])
    {
#if (EASY_CMD)
        iRet = appDemo_toupper(string1[i]) - appDemo_toupper(string2[i]);
#else
        iRet = string1[i] - string2[i];
#endif
        if(iRet)
        {
            return iRet;
        }
        i++;
    }
    return 0;
}



static void _appDemo_task(MS_U32 argc, VOID *argv)
{

    MS_U32 u32Event;
    MS_U16 u16I;
    MS_BOOL bknownCmd = FALSE;
    MS_BOOL bCmdRet = FALSE;

    while(1)
    {

        if (MsOS_WaitEvent(_s32DemoEventId,
                           DEMO_CMD_READY,
                           &u32Event,
                           E_AND_CLEAR,
                           DEMO_EVENT_WAIT_FOREVER))
        {

            if (u32Event & DEMO_CMD_READY)
            {
                GetCmdArgv(g_u8CMDArray, &_argc, g_argv);
                for (u16I = 0; u16I < NumOfDemoFun; u16I++)
                {
                    if(_appDemo_strcmp((char *)_DemoFun[u16I].u8CMDName, (char *)g_argv[0])==0)    // matching the function name
                    {

                        if (_DemoFun[u16I].u8InputMask & 0x1)
                        {
                            gInput0 = (g_argv[1]!=0) ? g_argv[1] : g_NullStr;
                        }
                        else
                        {
                            gu32Input0 = DHConvert((char *)g_argv[1],1);
                            gInput0 = &gu32Input0;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x2)
                        {
                            gInput1 = (g_argv[2]!=0) ? g_argv[2] : g_NullStr;
                        }
                        else
                        {
                            gu32Input1 = DHConvert((char *)g_argv[2],1);
                            gInput1 = &gu32Input1;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x4)
                        {
                            gInput2 = (g_argv[3]!=0) ? g_argv[3] : g_NullStr;
                        }
                        else
                        {
                            gu32Input2 = DHConvert((char *)g_argv[3],1);
                            gInput2 = &gu32Input2;
                        }


                        if (_DemoFun[u16I].u8InputMask & 0x8)
                        {
                            gInput3 = (g_argv[4]!=0) ? g_argv[4] : g_NullStr;
                        }
                        else
                        {
                            gu32Input3 = DHConvert((char *)g_argv[4],1);
                            gInput3 = &gu32Input3;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x10)
                        {
                            gInput4 = (g_argv[5]!=0) ? g_argv[5] : g_NullStr;
                        }
                        else
                        {
                            gu32Input4 = DHConvert((char *)g_argv[5],1);
                            gInput4 = &gu32Input4;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x20)
                        {
                            gInput5 = (g_argv[6]!=0) ? g_argv[6] : g_NullStr;
                        }
                        else
                        {
                            gu32Input5 = DHConvert((char *)g_argv[6],1);
                            gInput5 = &gu32Input5;
                        }
#ifdef ENABLE_PVR_PACKAGE

                        gu32Input6 = DHConvert((char *)g_argv[7],1);
                        gInput6 = &gu32Input6;

                        gu32Input7 = DHConvert((char *)g_argv[8],1);
                        gInput7 = &gu32Input7;

                        gu32Input8 = DHConvert((char *)g_argv[9],1);
                        gInput8 = &gu32Input8;

                        gu32Input9 = DHConvert((char *)g_argv[10],1);
                        gInput9 = &gu32Input9;

                        gu32Input10= DHConvert((char *)g_argv[11],1);
                        gInput10 = &gu32Input10;

                        bCmdRet = _DemoFun[u16I].func_ptr(gInput0,gInput1,gInput2,gInput3,gInput4, gInput5, gInput6,gInput7,gInput8,gInput9,gInput10);
#else
                        bCmdRet = _DemoFun[u16I].func_ptr(gInput0,gInput1,gInput2,gInput3,gInput4, gInput5);
#endif /*ENABLE_PVR_PACKAGE*/

                        printf("\n");
                        if (bCmdRet)
                        {
                            printf("\n>> %s, OK",g_argv[0]);
                        }
                        else
                        {
                            printf("\n>> %s, FAIL",g_argv[0]);
                        }
                        printf("\n>> ");
                        bknownCmd = TRUE;
                        break;
                    }
                    bknownCmd = FALSE;
                }


                if(_argc & !bknownCmd)
                {
                    (printf("\n>> unknow command"));
                    (printf("\n>> "));
                }


                /*
                printf("You Press:");
                MDrv_UART_PutString( g_u8CMDArray);
                printf("\n");

                for (u8I = 0; u8I<_argc; u8I++)
                {
                    printf("CMD = %s\n" ,g_argv[u8I]);

                    printf("Convert to decimal = %d\n",DHConvert(g_argv[u8I], 1) );
                }
                */

                MsOS_SetEvent(_s32DemoEventId, DEMO_CMD_END);

            }
        }

    }
}

static void Prompt(void)
{
    printf("\n>> ");
}


static MS_U8 GetCmd(void)
{
    MS_U8* pu8Token;

    if (NULL == (fgets((char*)g_u8CMDArray, MAX_CMD_SIZE, stdin)))
    {
        return FALSE;
    }
    if (NULL == (pu8Token = (MS_U8*)strtok((char*)g_u8CMDArray, CMD_DELIM)))
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL run_Cmd (const char *cmd)
{
    MS_U16 u16I;
    MS_BOOL bknownCmd = FALSE;
    MS_U32 u32Input0, u32Input1, u32Input2, u32Input3, u32Input4, u32Input5;
#ifdef ENABLE_PVR_PACKAGE
    MS_U32 u32Input6, u32Input7, u32Input8, u32Input9, u32Input10;
#endif
    void *Input0, *Input1, *Input2, *Input3, *Input4, *Input5;
#ifdef ENABLE_PVR_PACKAGE
    void *Input6, *Input7, *Input8, *Input9, *Input10;
#endif

    MS_U8   *argv[MAX_CMD_PARAMETER+1];
    MS_U8 argc=0;
    MS_U8   *NullStr = NULL;
    MS_BOOL bCmdRet = FALSE;

    GetCmdArgv((MS_U8 *)cmd, &argc, argv);
    for (u16I = 0; u16I < NumOfDemoFun; u16I++)
    {
        if(_appDemo_strcmp((char *)_DemoFun[u16I].u8CMDName, (char *)argv[0])==0)    // matching the function name
        {

            if (_DemoFun[u16I].u8InputMask & 0x1)
            {
                Input0 = (argv[1]!=0) ? argv[1] : NullStr;
            }
            else
            {
                u32Input0 = DHConvert((char *)argv[1],1);
                Input0 = &u32Input0;
            }

            if (_DemoFun[u16I].u8InputMask & 0x2)
            {
                Input1 = (argv[2]!=0) ? argv[2] : NullStr;
            }
            else
            {
                u32Input1 = DHConvert((char *)argv[2],1);
                Input1 = &u32Input1;
            }

            if (_DemoFun[u16I].u8InputMask & 0x4)
            {
                Input2 = (argv[3]!=0) ? argv[3] : NullStr;
            }
            else
            {
                u32Input2 = DHConvert((char *)argv[3],1);
                Input2 = &u32Input2;
            }


            if (_DemoFun[u16I].u8InputMask & 0x8)
            {
                Input3 = (argv[4]!=0) ? argv[4] : NullStr;
            }
            else
            {
                u32Input3 = DHConvert((char *)argv[4],1);
                Input3 = &u32Input3;
            }

            if (_DemoFun[u16I].u8InputMask & 0x10)
            {
                Input4 = (argv[5]!=0) ? argv[5] : NullStr;
            }
            else
            {
                u32Input4 = DHConvert((char *)argv[5],1);
                Input4 = &u32Input4;
            }

            if (_DemoFun[u16I].u8InputMask & 0x20)
            {
                Input5 = (argv[6]!=0) ? argv[6] : NullStr;
            }
            else
            {
                u32Input5 = DHConvert((char *)argv[6],1);
                Input5 = &u32Input5;
            }
#ifdef ENABLE_PVR_PACKAGE

            u32Input6 = DHConvert((char *)argv[7],1);
            Input6 = &u32Input6;

            u32Input7 = DHConvert((char *)argv[8],1);
            Input7 = &u32Input7;

            u32Input8 = DHConvert((char *)argv[9],1);
            Input8 = &u32Input8;

            u32Input9 = DHConvert((char *)argv[10],1);
            Input9 = &u32Input9;

            u32Input10= DHConvert((char *)argv[11],1);
            Input10 = &u32Input10;


            bCmdRet = _DemoFun[u16I].func_ptr(Input0,Input1,Input2,Input3,Input4, Input5, Input6,Input7,Input8,Input9,Input10);
#else

            bCmdRet = _DemoFun[u16I].func_ptr(Input0,Input1,Input2,Input3,Input4,Input5);
#endif /*ENABLE_PVR_PACKAGE*/
            printf("\n");
            if (bCmdRet)
            {
                printf("\n>> %s, OK",argv[0]);
            }
            else
            {
                printf("\n>> %s, FAIL",argv[0]);
            }
            printf("\n>> ");
            bknownCmd = TRUE;
            break;
        }
        bknownCmd = FALSE;
    }


    if(argc & !bknownCmd)
    {
        (printf("\n>> unknow command"));
        (printf("\n>> "));
    }
    return bCmdRet;
}

static void _appDemoMon_task(MS_U32 argc, VOID *argv)
{
    MS_U32 u32Event;
    printf("\n");
    Prompt();
    while (1)
    {
        if (GetCmd() == TRUE)
        {
            MsOS_SetEvent(_s32DemoEventId, DEMO_CMD_READY);
            MsOS_WaitEvent(_s32DemoEventId,
                           DEMO_CMD_END,
                           &u32Event,
                           E_AND_CLEAR,
                           DEMO_EVENT_WAIT_FOREVER);
            Prompt();
        }
    }

}



//-------------------------------------------------------------------------------------------------
/// Demo Sample Main Function - Hello World Sample
/// @return TRUE - succeed
/// @return EXCEPTION - fail
/// @sa
/// @ref MsOS_AllocateMemory, @ref MsOS_CreateTask
/// @note
/// Command: \b none \n
//-------------------------------------------------------------------------------------------------
MS_BOOL appDemo_Main(void)
{

    /// - Create Demo Main Task

    /// - Allocate Memory
    _appDemo_Task.pStack = MsOS_AllocateMemory(_appDemo_Task.u32StackSize, gs32CachedPoolID);

    if(!_appDemo_Task.pStack)
    {
        GEN_EXCEP;
    }

    /// - Create Task Application
    _appDemo_Task.iId = MsOS_CreateTask( _appDemo_task,
                                            (MS_U32)NULL,
                                            _appDemo_Task.ePriority,
                                            TRUE,
                                            _appDemo_Task.pStack,
                                            _appDemo_Task.u32StackSize,
                                            _appDemo_Task.szName);
    if(_appDemo_Task.iId < 0)
    {
        GEN_EXCEP;
    }

    /// - Create OS Event Flag
    _s32DemoEventId = MsOS_CreateEventGroup("Demo AP");

    if (_s32DemoEventId < 0)
    {
        GEN_EXCEP;
    }


    /// - Create Demo Monitor Task

    /// - Allocate Memory
    _appDemoMon_Task.pStack = MsOS_AllocateMemory(_appDemoMon_Task.u32StackSize, gs32CachedPoolID);

    if(!_appDemoMon_Task.pStack)
    {
        GEN_EXCEP;
    }

    /// - Create Task Application
    _appDemoMon_Task.iId = MsOS_CreateTask( _appDemoMon_task,
                                            (MS_U32)NULL,
                                            _appDemoMon_Task.ePriority,
                                            TRUE,
                                            _appDemoMon_Task.pStack,
                                            _appDemoMon_Task.u32StackSize,
                                            _appDemoMon_Task.szName);
    if(_appDemoMon_Task.iId < 0)
    {
        GEN_EXCEP;
    }

    /// - Create OS Event Flag
    _s32DemoMonEventId = MsOS_CreateEventGroup("Demo Mointor");

    if (_s32DemoMonEventId < 0)
    {
        GEN_EXCEP;
    }


    //MDrv_UART_Init(E_UART_PIU_UART0,115200);
    //MDrv_UART_RegisterCallBack(_UART_CBfun);




    /// - Print UART message
    (printf("----------Hello World----------\n"));
    (printf("-------Welcome to Demo AP------\n"));
    (printf(">> "));

    return TRUE;
}


#if 0
static MS_BOOL _appDemo_Reset(void)
{
    MsOS_SuspendTask(_appDemo_Task.iId);
    MsOS_DelayTask(200);

    //Delete Task
    if (MsOS_DeleteTask(_appDemo_Task.iId))
    {
        MsOS_FreeMemory(_appDemo_Task.pStack, gs32CachedPoolID);


        _appDemo_Task.pStack = MsOS_AllocateMemory(_appDemo_Task.u32StackSize, gs32CachedPoolID);
        if(!_appDemo_Task.pStack)
        {
            GEN_EXCEP;
        }

        //Crete Task
        _appDemo_Task.iId = MsOS_CreateTask( _appDemo_task,
                                                NULL,
                                                _appDemo_Task.ePriority,
                                                TRUE,
                                                _appDemo_Task.pStack,
                                                _appDemo_Task.u32StackSize,
                                                _appDemo_Task.szName);


        if(_appDemo_Task.iId < 0)
        {
            GEN_EXCEP;
        }

        return TRUE;
    }

    return FALSE;
}
#endif

