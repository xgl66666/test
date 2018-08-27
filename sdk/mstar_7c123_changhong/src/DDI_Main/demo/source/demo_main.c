//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012, 2016 MStar Semiconductor, Inc. All rights reserved.
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
#include <sys/stat.h>
#include "string.h"
#include "MsCommon.h"
#include "Board.h"
#include "MsFS.h"
#include "drvSYS.h"
#include "drvMMIO.h"
#include "drvUART.h"
#include <stdbool.h>
#include "drvDTC.h"

#include "demo_main.h"

#if (DEMO_DMX_TEST == 1)
#include "demo_dmx.h"
#endif
#if (DEMO_PM_TEST == 1)
#include "demo_pm.h"
#endif
#if ( DEMO_DSCMB_ADVANCED_TEST == 1)
#include "demo_dscmb_advanced.h"
#endif
#if ( DEMO_DSCMB_BASIC_TEST == 1)
#include "demo_dscmb_basic.h"
#endif
#if (DEMO_MM_TEST == 1)
#include "mmsdk_interface_def.h"
#include "demo_mm.h"
#endif
#if (DEMO_CH34_TEST == 1)
#include "demo_ch34.h"
#endif
#if (DEMO_DIP_TEST == 1)
#include "demo_dip.h"
#endif
#if (DEMO_XC_TEST == 1)
#include "demo_xc.h"
#endif
#if (DEMO_OSD_TEST == 1)
#include "demo_osd.h"
#endif
#if (DEMO_7SEGMENTS_TEST == 1)
#include "demo_7segments.h"
#endif
#if (DEMO_BDMA_TEST == 1)
#include "demo_bdma.h"
#endif
#if (DEMO_AUDIO_TEST == 1)
#include "apiAUDIO.h"
#include "demo_audio.h"
#endif
#if (DEMO_AUDIO_KTV_TEST == 1)
#include "demo_audio_ktv.h"
#endif
#if (DEMO_MFE_TEST == 1)
#include "demo_mfe.h"
#endif
#if (DEMO_NET_TEST == 1)
#include "demo_net.h"
#endif
#if (DEMO_SEGFAULT_TEST == 1)
#include "demo_segfault.h"
#endif
#if (DEMO_SNMP_TEST == 1)
#include "demo_snmp.h"
#endif
#if (DEMO_UART_TTY_TEST == 1)
#include "demo_uart_tty.h"
#endif
#if (DEMO_BENCH_DHRYSTON_ECOS_TEST == 1)
#include "demo_bench_dhryston.h"
#endif
#if (DEMO_BENCH_LIMITDHRYSTONE_LINUX_TEST == 1)
#include "demo_bench_limitdhryston_linux.h"
#endif
#if (DEMO_BENCH_MEM_TEST == 1)
#include "demo_bench_mem.h"
#endif
#if (DEMO_AV_TEST == 1)
#include "demo_av.h"
#endif
#if (DEMO_VDEC_TEST == 1)
#include "demo_vdec.h"
#endif
#if (DEMO_ZAPPING_TEST == 1)
#include "demo_zapping.h"
#endif
#if (DEMO_JPEG_TEST == 1)
#include "demo_jpeg.h"
#endif
#if (DEMO_GPD_TEST == 1)
#include "demo_gpd.h"
#endif
#if (DEMO_OS_TEST == 1)
#include "demo_os.h"
#endif
#if (DEMO_USB_ECOS_TEST == 1)
#include "demo_usb.h"
#endif
#if (DEMO_WDT_TEST == 1)
#include "demo_wdt.h"
#endif
#if (DEMO_FUART_TEST == 1)
#include "demo_fuart.h"
#endif

#if (DEMO_URDMA_TEST == 1)
#include "demo_urdma.h"
#endif

#if (DEMO_SMC_TEST == 1)
#include "demo_smc.h"
#endif
#if (DEMO_LINUX_MOUNT_NOTIFIER_TEST == 1)
#include "demo_linux_mount_notifier.h"
#endif
#if (DEMO_UART_TEST == 1)
#include "demo_uart.h"
#endif
#if (DEMO_DVFS_TEST == 1)
#include "demo_dvfs.h"
#endif
#if (DEMO_TSENSOR_TEST == 1)
#include "demo_tsensor.h"
#endif
#if (DEMO_DTM_TEST == 1)
#include "demo_dtm.h"
#endif
#if (DEMO_FRONTPNL_TEST == 1)
#include "demo_frontpnl.h"
#endif
#if (DEMO_KEYPAD_TEST == 1)
#include "demo_keypad.h"
#endif
#if (DEMO_IR_ECOS_TEST == 1)
#include "demo_ir_ecos.h"
#endif
#if (DEMO_IR_LINUX_TEST == 1)
#include "demo_ir_linux.h"
#endif
#if (DEMO_PCMCIA_TEST == 1)
#include "demo_pcmcia.h"
#endif
#if (DEMO_GPIO_TEST ==1)
#include "demo_gpio.h"
#endif
#if (DEMO_SPI_TEST == 1)
#include "demo_spi.h"
#include "demo_mspi.h"
#endif
#if (DEMO_PVR_TEST == 1)
#include "demo_pvr.h"
#endif
#if (DEMO_RSA_TEST == 1)
#include "demo_rsa.h"
#endif
#if (DEMO_AESDMA_TEST == 1)
#include "demo_aesdma.h"
#endif
#if (DEMO_CIPHER_TEST == 1)
#include "demo_cipher.h"
#endif
#if (DEMO_OPENGLES2_TEST == 1)
#include "demo_opengl_es2.h"
#endif
#if (DEMO_DRM_TEST == 1)
#include "demo_drm.h"
#include "demo_oemcrypto.h"
#endif
#if (DEMO_PLAYREADY_TEST == 1)
#include "demo_playready.h"
#endif
#if (DEMO_HBBTV_TEST == 1)
#include "demo_hbbtv.h"
#endif
#if (DEMO_APM_TEST == 1)
#include "demo_apm.h"
#endif
#if (DEMO_DFB_TEST == 1)
#include "demo_dfb.h"
#endif
#if (DEMO_GST_TEST == 1)
#include "demo_gst.h"
#endif
#if (DEMO_QT_TEST == 1)
#include "demo_qt.h"
#endif
#if (DEMO_NET_ECOS_TEST == 1)
#include "demo_net_ecos.h"
#endif
#if (DEMO_BOOST_TEST == 1)
#include "demo_boost.h"
#endif
#if (DEMO_TSPLAYER_TEST == 1)
#include "demo_tsplayer.h"
#endif
#if (DEMO_WIDI_TEST == 1)
#include "demo_widi.h"
#endif
#if (DEMO_eCos_API_TEST == 1)
#include "demo_eCos_api.h"
#endif
#if (DEMO_BOOTLOGO_TEST == 1)
#include "demo_bootlogo.h"
#endif
#if (DEMO_BOOTVIDEO_TEST == 1)
#include "demo_bootvideo.h"
#endif
#if (DEMO_EMMC_ECOS_TEST == 1)
#include "demo_emmc_ecos.h"
#endif
#if (DEMO_NAND_ECOS_TEST == 1)
#include "demo_nand_ecos.h"
#endif
#if (DEMO_DTE_TEST == 1)
#include "demo_dimension.h"
#endif

#include "apiDigiTuner.h"
#include "appDemo_DigiTuner.h"
#if (ECOS_WIFI_SUPPORT == 1)
#include "appDemo_Wlan.h"
#endif
#if (DEMO_CH_SCAN_TEST == 1)
#include "demo_ch_scan.h"
#endif

#if (DEMO_CEC_TEST == 1)
#include "demo_cec.h"
#endif

#if (DEMO_CI_TEST == 1)
#include "demo_ci.h"
#endif

#if (FE_AUTO_TEST == 1)
#include "demo_FEAutoTest.h"
#endif

#if(DEMO_ZUI_TEST == 1 || DEMO_AUTOIN_ZUI_TEST == 1 )
#include "demo_ui.h"
#endif
#include "demo_utility.h"

#if (DEMO_ECOS_BOOT_LINUX == 1)
#include "demo_bootlinux.h"
#endif

#if (DEMO_IR_TX_TEST == 1)
#include "demo_ir_tx.h"
#endif

#if (DEMO_GPU_BENCHMARK_TEST == 1)
#include "demo_gpu_benchmark.h"
#endif

#if (DEMO_SYS_INFO_TEST == 1)
#include "demo_SysInfo.h"
#endif

#if (DEMO_FASTCALL_ECOS_TEST == 1)
#include "demo_fastcall_ecos.h"
#endif

#if (DEMO_DOLBYHDR_PREIDK_TEST ==1)
#include "demo_dolbyhdr_preidk.h"
#endif

#if (DEMO_PROCFS_TEST == 1)
#include "demo_procfs.h"
#endif

#include "SysInit.h"
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//verify RX via Echo or Capture File
#define VERIFY_RX_VIA_ECHO
#define MAX_CMD_SIZE   1024 //file size < 100KB
#define DEMO_CMD_READY      0x1
#define DEMO_CMD_END        0x2
#define DEMO_CMD_EXIT       0x4
#define DEMO_RELEASE        0x8

#define DEMO_CMD_ALL   (DEMO_CMD_READY | DEMO_CMD_END | DEMO_CMD_RECV_EXIT | DEMO_CMD_PROC_EXIT)


#define MAX_CMD_PARAMETER 14
#define EN_ESC
#define CMD_DELIM       "\n"

#define DEMO_EVENT_WAIT_TIME 100
#define DEMO_EVENT_WAIT_FOREVER 0xffffffff

typedef MS_BOOL (*func_ptr)(void* u32Input0, void* u32Input1, void* u32Input2, void* u32Input3, void* u32Input4, void* u32Input5, void* u32Input6, void* u32Input7, void* u32Input8, void* u32Input9, void* u32Input10);

typedef struct _DemoFun
{
    MS_U8 u8CMDName[32];
    MS_U8 u8InputMask; //1: input string(MS_U8*), 0: input (MS_U16*);
                    //example: u8InputMask = 0x03, u32Input0,u32Input1  = (char*), u32Input2,u32Input3,u32Input4 = (MS_U16*)
    MS_BOOL (*func_ptr)(void* u32Input0, void* u32Input1, void* u32Input2, void* u32Input3, void* u32Input4, void* u32Input5, void* u32Input6, void* u32Input7, void* u32Input8, void* u32Input9, void* u32Input10);
} DemoFun,*pDemoFun;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_U32 gu32Input0, gu32Input1, gu32Input2, gu32Input3, gu32Input4, gu32Input5, gu32Input6, gu32Input7, gu32Input8, gu32Input9, gu32Input10;

void *gInput0, *gInput1, *gInput2, *gInput3, *gInput4, *gInput5, *gInput6, *gInput7, *gInput8, *gInput9, *gInput10;

MS_U8   g_u8CMDArray[MAX_CMD_SIZE];
MS_U8   *g_argv[MAX_CMD_PARAMETER+1];
MS_U8   *g_NullStr = NULL;


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

static Task_Info _Demo_Main_Task = {-1, E_TASK_PRI_HIGH, NULL, 0x9000, "Demo Task"};
static Task_Info _Demo_Main_Mon_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Demo Mon Task"};

static MS_S32  _s32DemoEventId = 0;
static MS_S32  _s32DemoMonEventId = 0;
static MS_U8   _argc;
static MS_BOOL _Demo_Exit = FALSE;
//static void _Demo_Main_Exit(MS_U32 argc, VOID *argv);
extern MS_BOOL appDemo_UART_Uart2Test(void);

static DemoFun _DemoFun[]=
{
#if (DEMO_FUART_TEST == 1)
    {"uart",                    0x00,  (func_ptr)&demo_fast_uart_test},
#endif

#if (DEMO_URDMA_TEST == 1)
    {"urdma",                    0x00,  (func_ptr)&demo_urdma_test},
    {"urdma_stop",               0x00,  (func_ptr)&demo_urdma_test_stop},
#endif

#if (DEMO_AUTO_GEN_TEST == 1)
    {"CL_INFO",                 0x01,   (func_ptr)&ChangeList_InfoQuery},
#endif
#if (DEMO_PM_TEST == 1)
    //Demo PM Func
    {"PM_RTC_Init",             0x00,   (func_ptr)&Demo_PM_RTC_Init},
    {"PM_RTC_GetTime",          0x00,   (func_ptr)&Demo_PM_RTC_GetTime},
    {"PM_RTC_SetTime",          0x00,   (func_ptr)&Demo_PM_RTC_SetTime},
    {"PM_RTC_SetMatchTime",     0x00,   (func_ptr)&Demo_PM_RTC_SetMatchTime},
#if (DEMO_PM_HK_LINUX_TEST == 1) || (DEMO_PM_HK_eCos_TEST == 1)
    {"PM_PowerSaving",          0x00,   (func_ptr)&Demo_PM_PowerSaving},
#else
    {"PM_Init",                 0x00,   (func_ptr)&Demo_PM_Init},
    {"PM_PowerDown",            0x00,   (func_ptr)&Demo_PM_PowerDown},
    {"PM_FastStandby",          0x00,   (func_ptr)&Demo_PM_FastStandby},
    {"PM_Dram_Disable",         0x00,   (func_ptr)&Demo_PM_DRAM_Disable},
#if (DEMO_PM_STR_LINUX_TEST == 1) || (DEMO_PM_STR_eCos_TEST == 1)
    {"PM_STR",                  0x00,   (func_ptr)&Demo_PM_STR},
#endif
    {"PM_RTC_Wakeup",           0x00,   (func_ptr)&Demo_PM_RTC_WakeUp},
#if (DEMO_PM_WOL_TEST == 1)
    {"PM_WOL",                  0x02,   (func_ptr)&Demo_PM_ConfigWOL},
#endif
#endif
#endif

#if (DEMO_DMX_TEST == 1)
    // Demo Dmux
//    {"DMX_Help",       0x00,   (func_ptr)&appDemo_Dmx_help},
//    {"DMX_Pat",        0x00,   (func_ptr)&appDemo_Dmx_Pat},
//    {"DMX_Pmt",        0x00,   (func_ptr)&appDemo_Dmx_Pmt},
    {"DMX_Sec",                 0x01,   (func_ptr)&Demo_DMX_Sec_CMD},
    {"DMX_Scan",                0x01,   (func_ptr)&Demo_DMX_Scan_CMD},
    {"DMX_PCR",                 0x01,   (func_ptr)&Demo_DMX_PCR_CMD},
    {"DMX_nmask",               0x0F,   (func_ptr)&Demo_DMX_Nmask_CMD},
    {"DMX_PES",                 0x01,   (func_ptr)&Demo_DMX_PES_CMD},
    {"DMX_Pkt",                 0x01,   (func_ptr)&Demo_DMX_Packet_CMD},
    {"DMX_Rec",                 0x03,   (func_ptr)&Demo_DMX_Record_CMD},
    {"DMX_MMFI",                0x00,   (func_ptr)&Demo_DMX_MMFI_CMD},
    {"DMX_FileIn_Start",        0x0D,   (func_ptr)&Demo_DMX_FileIn_Start},
    {"DMX_FileIn_Stop",         0x01,   (func_ptr)&Demo_DMX_FileIn_Stop},
    {"DMX_SetSrcID",            0x00,   (func_ptr)&Demo_DMX_SetSrcID_CMD},
    {"DMX_GetSrcID",            0x00,   (func_ptr)&Demo_DMX_GetSrcID_CMD},
    {"DMX_SetPktMode",          0x00,   (func_ptr)&Demo_DMX_SetPktMode},
    {"DMX_FIFO_RST",            0x00,   (func_ptr)&Demo_DMX_AVFifo_Reset_CMD},

#if (DEMO_DMX_TSO_10_TEST == 1) && (DEMO_DMX_TSO_20_TEST == 1)
    #error "TSO 1.0 / 2.0 could not enable concurrently";
#elif (DEMO_DMX_TSO_10_TEST == 1) || (DEMO_DMX_TSO_20_TEST == 1)
    {"DMX_TSO",                 0x00,   (func_ptr)&Demo_DMX_TSO},
    {"DMX_TSO_FileIn",          0x00,   (func_ptr)&Demo_DMX_TSO_FileIn},
#endif

#endif

#if ( DEMO_DSCMB_BASIC_TEST == 1)
    {"DSCMB_Help",              0x00,   (func_ptr)&Demo_DSCMB_Help},
    {"DSCMB_ConnectPID",        0x00,   (func_ptr)&Demo_DSCMB_ConnectPID},
    {"DSCMB_DisconnectPID",     0x00,   (func_ptr)&Demo_DSCMB_DisconnectPID},
    {"DSCMB_SetType",           0x02,   (func_ptr)&Demo_DSCMB_SetType},
    {"DSCMB_CW",                0x06,   (func_ptr)&Demo_DSCMB_CW},
    {"DSCMB_IV",                0x06,   (func_ptr)&Demo_DSCMB_IV},
#endif


#if ( DEMO_DSCMB_ADVANCED_TEST == 1)
    {"DSCMB_Help",              0x00,   (func_ptr)&Demo_DSCMB_Help},
    {"DSCMB_SetCAVID",          0x00,   (func_ptr)&Demo_DSCMB_SetCAVID},
    {"DSCMB_ConnectPID",        0x00,   (func_ptr)&Demo_DSCMB_ConnectPID},
    {"DSCMB_DisconnectPID",     0x00,   (func_ptr)&Demo_DSCMB_DisconnectPID},
    {"DSCMB_SetType",           0x02,   (func_ptr)&Demo_DSCMB_SetType},
    {"DSCMB_CW",                0x06,   (func_ptr)&Demo_DSCMB_CW},
    {"DSCMB_IV",                0x06,   (func_ptr)&Demo_DSCMB_IV},
    {"DSCMB_KeyLadderMode",     0x01,   (func_ptr)&Demo_DSCMB_KeyLadderMode},
    {"DSCMB_KeyLadder",         0x1e,   (func_ptr)&Demo_DSCMB_KeyLadder},
#endif

#if (DEMO_ZAPPING_TEST == 1)
    {"Zapping_SavePid",         0x00,   (func_ptr)&Demo_Zapping_SavePid},
    {"Zapping_SaveFreq",        0x00,   (func_ptr)&Demo_Zapping_SaveFreq},
    {"Zapping_SaveTunerConfig", 0x00,   (func_ptr)&Demo_Zapping_SaveTunerConfig},
    {"Zapping_SaveDeviceInfo",  0x00,   (func_ptr)&Demo_Zapping_SaveDeviceInfo},
    {"Zapping_ZappingType",     0x00,   (func_ptr)&Demo_Zapping_ZappingType},
    {"Zapping_ZappingType_EX",  0x00,   (func_ptr)&Demo_Zapping_ZappingType_EX},
    {"Zapping_Channel",         0x00,   (func_ptr)&Demo_Zapping_Channel},
#if (DEMO_ZAPPING_FQ_TEST == 1)
    {"Zapping_FQ_Init",         0x00,   (func_ptr)&Demo_FQ_Init},           //FQ Init
    {"Zapping_FQ_Exit",         0x00,   (func_ptr)&Demo_FQ_Exit},           //FQ Exit
    {"Zapping_FQ",              0x00,   (func_ptr)&Demo_FQ_Zapping},        //Demo FQ Zapping
    {"Zapping_FQ_Eng_Init",     0x00,   (func_ptr)&Demo_FQ_Eng_Init},       //FQ Eng Init
    {"Zapping_FQ_Eng_Exit",     0x00,   (func_ptr)&Demo_FQ_Eng_Exit},       //FQ Eng Exit
    {"Zapping_FQ_Eng",          0x00,   (func_ptr)&Demo_FQ_Eng_Zapping},    //Demo FQ Eng Zapping
#endif
#endif

#if (DEMO_MM_TEST == 1)
    //Demo Multi-Media Func
    {"MM_INIT",                 0x00,   (func_ptr)&Demo_MM_Init},
    {"MM_PLAY",                 0x02,   (func_ptr)&Demo_MM_Play},
    {"OSD_DrawPhoto",           0x00,   (func_ptr)&Demo_MM_DrawPhoto},
    {"OSD_SetFBFmt",            0x00,   (func_ptr)&Demo_MM_Photo_SetGwinFmt},
    {"OSD_ClearPhoto",          0x00,   (func_ptr)&Demo_MM_ClearPhoto},
    {"MM_SubtitleTrack",        0x00,   (func_ptr)&Demo_MM_SetSubtitleTrack},
    {"MM_AudioTrack",           0x00,   (func_ptr)&Demo_MM_SetAudioTrack},
    {"MM_ProgramTrack",         0x00,   (func_ptr)&Demo_MM_SetProgramTrack},
    {"MM_FF",                   0x00,   (func_ptr)&Demo_MM_FF},
    {"MM_RW",                   0x00,   (func_ptr)&Demo_MM_RW},
    {"MM_STEP",                 0x00,   (func_ptr)&Demo_MM_Step},
    {"MM_STEPPLAY",             0x02,   (func_ptr)&Demo_MM_StepPlay},
    {"MM_PAUSE",                0x00,   (func_ptr)&Demo_MM_Pause},
    {"MM_RESUME",               0x00,   (func_ptr)&Demo_MM_Resume},
    {"MM_STOP_TYPE",            0x00,   (func_ptr)&Demo_MM_Stop_MediaType},
    {"MM_STOP",                 0x00,   (func_ptr)&Demo_MM_Stop},
    {"MM_Repeat",               0x00,   (func_ptr)&Demo_MM_Repeat},
    {"MM_Repeat_EX",            0x00,   (func_ptr)&Demo_MM_Repeat_EX},
    {"MM_RW_Replay",            0x00,   (func_ptr)&Demo_MM_RW_Replay},
    {"MM_SEEK",                 0x00,   (func_ptr)&Demo_MM_Seek},
    {"MM_GETOPTION",            0x00,   (func_ptr)&Demo_MM_GetOption},
    {"MM_SetDrawPhotoPath",     0x00,   (func_ptr)&Demo_MM_Set_DrawPhotoPath},
    {"MM_Rotate",               0x00,   (func_ptr)&Demo_MM_Display_Rotate},
    {"MM_Zoom",                 0x00,   (func_ptr)&Demo_MM_Display_Zoom},
    {"MM_DisplayResume",        0x00,   (func_ptr)&Demo_MM_Display_Resume},
    {"MM_MoveView",             0x00,   (func_ptr)&Demo_MM_Display_MoveView},
    {"MM_ShowEffect",           0x00,   (func_ptr)&Demo_MM_Set_SlideShowEffect},
    {"MM_SetDebugLevel",        0x00,   (func_ptr)&Demo_MM_SetDebugLevel},
    {"MM_SetMotionGif",         0x00,   (func_ptr)&Demo_MM_SetEnableMotionGif},
    {"MM_SetWindow",            0x00,   (func_ptr)&Demo_MM_SetWindow},
    {"MM_SetKeepRatio",         0x00,   (func_ptr)&Demo_MM_SetKeepRatio},
    {"MM_PlayVideo_UsingDefaultSetting",        0x00,   (func_ptr)&Demo_XC_PlayVideo},
    {"MM_DEINIT",               0x00,   (func_ptr)&Demo_MM_DeInit},
#if (DEMO_MM_AVP_TEST == 1)
    {"MM_SwitchMediaItemAudio", 0x00,   (func_ptr)&Demo_MM_SwitchMediaItemAudio},
    {"MM_SetOperableStreamID",  0x00,   (func_ptr)&Demo_MM_SetOperableMediaItem},
    {"MM_SetDipWindow",         0x00,   (func_ptr)&Demo_MM_SetWindow},
    {"MM_MuteMediaItem",        0x00,   (func_ptr)&Demo_MM_MuteMediaItem},
    {"MM_SetAudioSwitchMode",   0x00,   (func_ptr)&Demo_MM_SetAudioSwitchMode},
    {"MM_SetZOrder",            0x00,   (func_ptr)&Demo_MM_SetAVPZOrder},
    {"MM_ResetWindow",          0x00,   (func_ptr)&Demo_MM_ResetWindow},
    {"MM_PlayMulti",            0x01,   (func_ptr)&Demo_MM_PlayMultiStreams},
#endif


#endif //#if (DEMO_MM_TEST ==1)

#if (DEMO_DOLBYHDR_PREIDK_TEST ==1)
    {"IDK_LOADFRM",             0x03,   (func_ptr)&Demo_DolbyHDR_PreIDK_LoadFrame},
    {"IDK_LOADPIC",             0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_LoadPicture},
    {"IDK_AUTO",                0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_AutoVerify},
    {"IDK_FRM",                 0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_VerifyFrame},
    {"IDK_SCP",                 0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_RunRiuScripts},
    {"IDK_DUMP",                0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_SaveDram2Bin},
    {"IDK_ADC",                 0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_ADC_Dump},
    {"IDK_CAPTURE",             0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_Capture2File},
    {"IDK_DUMPMM",              0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_DumpMM},
    {"IDK_OSD",                 0x01,   (func_ptr)&Demo_DolbyHDR_PreIDK_OSDBlending},
    {"IDK_MODE",                0x00,   (func_ptr)&Demo_DolbyHDR_PreIDK_IDKMode},
    {"IDK_SET_OUTTYPE",         0x00,   (func_ptr)&Demo_DolbyHDR_PreIDK_Set_OutputType},
    {"IDK_SET_VSVDB",           0x00,   (func_ptr)&Demo_DolbyHDR_PreIDK_Set_VSVDB},
    {"IDK_SET_GPRIORITY",       0x00,   (func_ptr)&Demo_DolbyHDR_PreIDK_Set_GraphicPriority},
    {"IDK_SET_LOWLATENCY",      0x00,   (func_ptr)&Demo_DolbyHDR_PreIDK_Set_LowLatencyMode},
    {"IDK_SET_DOVIMAPHDR10",    0x00,   (func_ptr)&Demo_DolbyHDR_PreIDK_Set_DolbyMappingHDR10},
#endif

#if (DEMO_7SEGMENTS_TEST == 1)
    {"7SegmentSet",             0x00,   (func_ptr)&Demo_DisplayDigiNum},
    {"7SegmentStartTime",       0x00,   (func_ptr)&Demo_DisplayTime_Start},
    {"7SegmentStopTime",        0x00,   (func_ptr)&Demo_DisplayTime_Stop},
#endif

#if (DEMO_BDMA_TEST == 1)
    {"BDMA_Help",               0x00,   (func_ptr)&Demo_BDMA_Help},
    {"BDMA_Demo",               0x00,   (func_ptr)&Demo_BDMA_Demo},
    {"BDMA_Init",               0x00,   (func_ptr)&Demo_BDMA_Init},
    {"BDMA_Cmp",                0x00,   (func_ptr)&Demo_BDMA_CmpMem},
    {"BDMA_Dump",               0x00,   (func_ptr)&Demo_BDMA_DumpMem},
    {"BDMA_Fill",               0x00,   (func_ptr)&Demo_BDMA_FillMem},
    {"BDMA_HKFill",             0x00,   (func_ptr)&Demo_BDMA_HKFillMem},
    {"BDMA_Copy",               0x00,   (func_ptr)&Demo_BDMA_Copy},
    {"BDMA_Allocate",           0x00,   (func_ptr)&Demo_BDMA_AllocateMemTest},
    {"BDMA_Free",               0x00,   (func_ptr)&Demo_BDMA_FreeMemTest},
#endif

#if (DEMO_AUDIO_TEST == 1)
    {"HelpAudio",               0x00,   (func_ptr)&Demo_Audio_Help},
    {"Audio_Init",              0x00,   (func_ptr)&Demo_Audio_Init},
    {"Audio_Exit",              0x00,   (func_ptr)&Demo_Audio_Exit},
    {"Audio_Open",              0x00,   (func_ptr)&Demo_Audio_Open},
    {"Audio_Release",           0x00,   (func_ptr)&Demo_Audio_Release},
    {"Audio_demo",              0x00,   (func_ptr)&Demo_Audio_Out_Init},
    {"Audio_SetOutputMode",     0x00,   (func_ptr)&Demo_Audio_SetOutputMode},
    {"Audio_SetDecOutputMode",  0x00,   (func_ptr)&Demo_Audio_SetDecOutputMode},
    {"Audio_volume",            0x00,   (func_ptr)&Demo_Audio_SetAbsoluteVolume},
    {"Audio_demoStop",          0x00,   (func_ptr)&Demo_Audio_Stop_Demo},
    {"Audio_mute",              0x00,   (func_ptr)&Demo_Audio_SetMute},
    {"Audio_dec_mute",          0x00,   (func_ptr)&Demo_Audio_SetDecMute},
    {"Audio_Monitor",           0x00,   (func_ptr)&Demo_Audio_SetMonitor},
    {"Audio_Debug",             0x00,   (func_ptr)&Demo_Audio_Debug},
    {"Audio_DRC",               0x00,   (func_ptr)&Demo_Audio_Set_DRC_Mode},
    {"Audio_DRC_Scale",         0x00,   (func_ptr)&Demo_Audio_Set_DRC_Scale},
    {"Audio_Downmix",           0x00,   (func_ptr)&Demo_Audio_Set_DownMix_Mode},
    {"Audio_DRC_EX",            0x00,   (func_ptr)&Demo_Audio_Set_DRC_Mode_EX},
    {"Audio_DRC_Scale_EX",      0x00,   (func_ptr)&Demo_Audio_Set_DRC_Scale_EX},
    {"Audio_Downmix_EX",        0x00,   (func_ptr)&Demo_Audio_Set_DownMix_Mode_EX},
    {"Audio_TB11",              0x00,   (func_ptr)&Demo_Audio_Set_TB11},
    {"Audio_Dynamic_Alloc",     0x00,   (func_ptr)&Demo_Audio_Set_Dynamic_Allocate_Resource},
    {"Audio_Codec_Delay",       0x00,   (func_ptr)&Demo_Audio_Codec_Delay},
    {"Audio_Codec_Delay_EX",    0x00,   (func_ptr)&Demo_Audio_Codec_Delay_EX},
    {"Audio_Delay",             0x00,   (func_ptr)&Demo_Audio_Delay},
    {"Audio_DTSHD",             0x00,   (func_ptr)&Demo_Audio_Set_DTSHD},
    {"Audio_OutSource_Config",  0x00,   (func_ptr)&Demo_Audio_Set_Output_Source_Config},
    {"Audio_WReg",              0x00,   (func_ptr)&Demo_Audio_Debug_WriteReg},
    {"Audio_IR",                0x00,   (func_ptr)&Demo_Audio_IR_Debug},
    {"Audio_Debug_R2",          0x00,   (func_ptr)&Demo_Audio_Debug_R2},
    {"Audio_PUSI",              0x00,   (func_ptr)&Demo_Audio_PUSI},
    {"Audio_Test_Monitor_Start", 0x00,   (func_ptr)&Demo_Audio_Test_Monitor_Start},
    {"Audio_Test_Monitor_Stop" , 0x00,   (func_ptr)&Demo_Audio_Test_Monitor_Stop},

#if (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)
    {"Audio_PCM_Capture_Start", 0x20,   (func_ptr)&Demo_Audio_PCM_Capture_Start},
    {"Audio_PCM_Capture_Stop",  0x00,   (func_ptr)&Demo_Audio_PCM_Capture_Stop},
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
    {"Audio_Transcode_Start",   0x08,   (func_ptr)&Demo_Audio_Transcode_Start},
    {"Audio_Transcode_Stop",    0x00,   (func_ptr)&Demo_Audio_Transcode_Stop},
    {"Audio_Transcode_Pause",   0x00,   (func_ptr)&Demo_Audio_Transcode_Pause},
#endif

#if (DEMO_XC_HDMIRX_TEST == 1)
    {"Audio_EDID_Output",       0x00,   (func_ptr)&Demo_Audio_EDID_Output},
    {"Audio_EDID_Output_EX",    0x00,   (func_ptr)&Demo_Audio_EDID_Output_EX},
#endif

#if(DEMO_AUDIO_AD_FADER_CONTROL_TEST == 1)
    {"Audio_AD_Fader",          0x00,   (func_ptr)&Demo_Audio_Set_AD_Fader},
#endif

#if(DEMO_AUDIO_SE_DV258_TEST == 1)
    {"Audio_DV258",             0x00,   (func_ptr)&Demo_Audio_Set_DV258},
#endif

#if (DEMO_AUDIO_DMA_READER_TEST == 1)
    {"Audio_DMA_Test",          0x01,   (func_ptr)&Demo_Audio_DMA_Reader_Test},
#endif
#if ((DEMO_AUDIO_DMA_READER_TEST == 1) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
    {"Audio_DMA_EX_Test",       0x02,   (func_ptr)&Demo_Audio_DMA_Reader_EX_Test},
#endif

#if (DEMO_AUDIO_FMTX_TEST == 1)
    {"Audio_FMTX_start",        0x00,   (func_ptr)&Demo_Audio_FMTX_Start},
    {"Audio_FMTX_stop",         0x00,   (func_ptr)&Demo_Audio_FMTX_Stop},
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
    {"Audio_BTSC_start",        0x00,   (func_ptr)&Demo_Audio_BTSC_Start},
    {"Audio_BTSC_stop",         0x00,   (func_ptr)&Demo_Audio_BTSC_Stop},
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
    {"Audio_HDMITx_Mode",       0x00,   (func_ptr)&Demo_Audio_Set_HDMITx_Mode},
    {"Audio_DDP",               0x00,   (func_ptr)&Demo_Audio_DDP},
    {"Audio_EDID",              0x00,   (func_ptr)&Demo_Audio_Show_HDMITx_EDID},
    {"Audio_HDMI_Delay",        0x00,   (func_ptr)&Demo_Audio_HDMI_Delay},
#endif

#if(DEMO_AUDIO_SPDIF_TEST == 1)
    {"Audio_SPDIF_Mode",        0x00,   (func_ptr)&Demo_Audio_Set_SPDIF_Mode},
    {"Audio_SPDIF_Delay",       0x00,   (func_ptr)&Demo_Audio_SPDIF_Delay},
#endif

#if(DEMO_AUDIO_KTV_TEST == 1)
    {"Audio_KTV",               0x00,   (func_ptr)&Demo_Audio_KTVTest},
    {"Audio_MIC",               0x00,   (func_ptr)&Demo_Audio_MICTest},
#endif

#if(DEMO_AUDIO_SOUND_EFFECT_TEST == 1)
    {"Audio_PreScale",          0x00,   (func_ptr)&Demo_Audio_SetPreScale},
    {"Audio_Bass_Param",        0x00,   (func_ptr)&Demo_Audio_SetBass_Param},
    {"Audio_Treble_Param",      0x00,   (func_ptr)&Demo_Audio_SetTreble_Param},
    {"Audio_Bass_Treble_Enable",0x00,   (func_ptr)&Demo_Audio_SetBassTreble_Enable},
    {"Audio_AVC_Param",         0x00,   (func_ptr)&Demo_Audio_SetAVC_Param},
    {"Audio_AVC_Enable",        0x00,   (func_ptr)&Demo_Audio_SetAVC_Enable},
    {"Audio_PEQ_Param",         0x00,   (func_ptr)&Demo_Audio_SetPEQ_Param},
    {"Audio_PEQ_Enable",        0x00,   (func_ptr)&Demo_Audio_SetPEQ_Enable},
    {"Audio_GEQ_Param",         0x00,   (func_ptr)&Demo_Audio_SetGEQ_Param},
    {"Audio_GEQ_Enable",        0x00,   (func_ptr)&Demo_Audio_SetGEQ_Enable},
    {"Audio_HPF_Enable",        0x00,   (func_ptr)&Demo_Audio_SetHPF_Enable},
    {"Audio_Balance_Enable",    0x00,   (func_ptr)&Demo_Audio_SetBalance_Enable},
    {"Audio_Balance_Param",     0x00,   (func_ptr)&Demo_Audio_SetBalance_Param},
#endif

#if(DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
    {"Audio_TSHD_Param",        0x00,   (func_ptr)&Demo_Audio_Set_TSHD_Param},
    {"Audio_TSHD_Enable",       0x00,   (func_ptr)&Demo_Audio_Set_TSHD_Enable},
#endif
#if (DEMO_AUDIO_AD_TEST == 1)
    {"Audio_Volume_AD",         0x00,   (func_ptr)&Demo_Audio_SetADAbsoluteVolume},
    {"Audio_Mute_AD",           0x00,   (func_ptr)&Demo_Audio_ADSetMute},
#endif

#if (DEMO_AUDIO_ENCODER_TEST == 1)
    {"Audio_Enc_Open",          0x02,   (func_ptr)&Demo_Audio_Enc_Open},
    {"Audio_Enc_Close",         0x00,   (func_ptr)&Demo_Audio_Enc_Close},
    {"Audio_Enc_Dump",          0x02,   (func_ptr)&Demo_Audio_Enc_Dump},
    {"Audio_Enc_Ioctl",         0x02,   (func_ptr)&Demo_Audio_Enc_Ioctl},
    {"Audio_Enc_Flush",         0x00,   (func_ptr)&Demo_Audio_Enc_Flush},
#endif

#if defined (DEMO_AUDIO_SUPPORT_MS12_B_TEST) && (DEMO_AUDIO_SUPPORT_MS12_B_TEST == 1)
    {"Audio_DAP",               0x00,   (func_ptr)&Demo_Audio_SetDAP_Enable},
    {"Audio_DAP_Param",         0x01,   (func_ptr)&Demo_Audio_SetDAP_Param},
#endif

#endif
#if (DEMO_CH34_TEST == 1)
    //Demo CH34 func
    {"CH34_Init",               0x00,   (func_ptr)&Demo_CH34_Init},
    {"CH34_SelectSpec",            0x00,   (func_ptr)&Demo_CH34_SelectSpec},
    {"CH34_SetDACSwingLevel",            0x00,   (func_ptr)&Demo_CH34_SetDACSwingLevel},
    {"CH34_SetSigmaDeltaGainLevel",            0x00,   (func_ptr)&Demo_CH34_SetSigmaDeltaGainLevel},
    {"CH34_SetMode",            0x00,   (func_ptr)&Demo_CH34_SetMode},
    {"CH34_Help",               0x00,   (func_ptr)&Demo_CH34_Help},
#endif
#if (DEMO_DIP_TEST == 1)
    // Demo DIP
    {"DIP_Init",                0x00,   (func_ptr)&Demo_DIP_Init},
    {"DIP_InputSourceSelect",   0x00,   (func_ptr)&Demo_DIP_InputSourceSelect},
    {"DIP_OutputFormatSelect",  0x00,   (func_ptr)&Demo_DIP_OutputFormatSelect},
    {"DIP_ARGBAlphaSet",        0x00,   (func_ptr)&Demo_DIP_ARGBAlphaSet},
    {"DIP_ShowResultType",      0x00,   (func_ptr)&Demo_DIP_ShowResultType},
    {"DIP_CaptureOneFrame",     0x00,   (func_ptr)&Demo_DIP_CaptureOneFrame},
    {"DIP_CaptureContinually",  0x00,   (func_ptr)&Demo_DIP_CaptureContinually},
    {"DIP_GOPPingpong",         0x00,   (func_ptr)&Demo_DIP_GOPPingpong},
    {"DIP_MFE",                 0x00,   (func_ptr)&Demo_DIP_MFE},
    {"DIP_DIPRFromVDEC",        0x00,   (func_ptr)&Demo_DIP_DIPRFromVDEC},
    {"DIP_Exit",                0x00,   (func_ptr)&Demo_DIP_Exit},
#endif

#if (DEMO_MFE_TEST == 1)
    {"MFE_Init",                0x00,   (func_ptr)&Demo_MFE_Init},
    {"MFE_Encode_File",         0x03,   (func_ptr)&Demo_MFE_Encode_File},
    {"MFE_Encode_DIP",          0x01,   (func_ptr)&Demo_MFE_Encode_From_DIP},
    {"MFE_Encode_FMT_EX",       0x00,   (func_ptr)&Demo_MFE_Encode_FMT_EX},
    {"MFE_Init_EX",             0x00,   (func_ptr)&Demo_MFE_Init_EX},
    {"MFE_Encode_File_EX",      0x06,   (func_ptr)&Demo_MFE_Encode_File_EX},
    {"MFE_Encode_DIP_EX",       0x02,   (func_ptr)&Demo_MFE_Encode_From_DIP_EX},
#if (DEMO_DMS_TEST == 1)
    {"MFE_Encode_Cap_EX",       0x02,   (func_ptr)&Demo_MFE_Encode_From_Capture_EX},
    {"MFE_Set_CaptureInfo",     0x00,   (func_ptr)&Demo_MFE_Set_CaptureInfo},
#endif
#endif

#if (DEMO_NET_TEST == 1)
    {"NET_Init",                0x00,   (func_ptr)&Demo_Net_Init},
#endif

#if (DEMO_SEGFAULT_TEST == 1)
    {"SegFault",                0x00,   (func_ptr)&Demo_SegFault_Init},
    {"HelpSegFault",            0x00,   (func_ptr)&Demo_SegFault_Help},
#endif

#if (DEMO_SNMP_TEST == 1)
    {"snmp_init",               0x00,   (func_ptr)&Demo_SNMP_Init},
    {"snmp_get",                0x00,   (func_ptr)&Demo_SNMP_Get},
#endif

#if (DEMO_UART_TTY_TEST == 1)
    {"uart_tty_init",           0x00,   (func_ptr)&Demo_UART_TTY_Init },
#endif

//Benchmarks
#if (DEMO_BENCH_DHRYSTON_ECOS_TEST == 1)
    {"Bench_Dhryston",          0x00,   (func_ptr)&Demo_Bench_Dhryston_Run},
    {"cpu_load",                0x00,   (func_ptr)&execute_cpu_load},
    {"cpu_unload",              0x00,   (func_ptr)&execute_cpu_unload},
    {"set_delay",               0x00,   (func_ptr)&bench_set_delay},
#endif
#if (DEMO_BENCH_LIMITDHRYSTONE_LINUX_TEST == 1)
    {"Demo_LimitDhrystone",     0x00,   (func_ptr)&Demo_Bench_LimitDhrystone},
#endif
#if (DEMO_BENCH_MEM_TEST == 1)
    {"Bench_Memory",            0x00,   (func_ptr)&Demo_Bench_Memory},
#endif

#if (DEMO_AV_TEST == 1)
    {"DTV_Tuner_Config",        0x00,   (func_ptr)&Demo_AV_Tuner_Config},
    {"DTV_TSP_SetPid",          0x00,   (func_ptr)&Demo_AV_TSP_SetPid},
    {"DTV_TSP_FileIn_SetPid",   0x7A,   (func_ptr)&Demo_AV_TSP_FileIn_SetPid},
    {"DTV_PlayAV",              0x00,   (func_ptr)&Demo_AV_PlayAV},
    {"DTV_PauseAV",             0x00,   (func_ptr)&Demo_AV_PauseAV},
    {"DTV_StopAV",              0x00,   (func_ptr)&Demo_AV_StopAV},
    {"DTV_FileIn_StopAV",       0x02,   (func_ptr)&Demo_AV_FileIn_StopAV},
    {"DTV_PlayRadio",           0x00,   (func_ptr)&Demo_AV_PlayRadio},
    {"DTV_StopRadio",           0x00,   (func_ptr)&Demo_AV_StopRadio},
    {"DTV_SetAVSyncMode",       0x00,   (func_ptr)&Demo_AV_SetAVSYNCMode},
    {"DTV_SetShowFrameMode",    0x00,   (func_ptr)&Demo_AV_SetShowFrameMode},
    {"DTV_ChangeDispWindow",    0x00,   (func_ptr)&Demo_AV_ChangeDispWindow},
    {"DTV_Lang",                0x00,   (func_ptr)&Demo_AV_SetLanguage},
    {"DTV_LangList",            0x00,   (func_ptr)&Demo_AV_LanguageList},
    {"DTV_Audio_Switch",        0x00,   (func_ptr)&Demo_AV_Audio_Switch},
    {"DTV_ExChangeWindow",      0x00,   (func_ptr)&Demo_AV_ExChangeWindow},
#if (DEMO_AUDIO_AD_TEST == 1)
    {"DTV_PlayAD",              0x00,   (func_ptr)&Demo_AV_PlayAD},
    {"DTV_StopAD",              0x00,   (func_ptr)&Demo_AV_StopAD},
#endif
#if(DEMO_AUDIO_AD_1PID_TEST == 1)
    {"DTV_PlayAD_1PID",         0x00,   (func_ptr)&Demo_AV_PlayAD_1PID},
    {"DTV_StopAD_1PID",         0x00,   (func_ptr)&Demo_AV_StopAD_1PID},
#endif
    {"DTV_DecIFrame",           0x04,   (func_ptr)&Demo_AV_DecodeIFrame},
    {"DTV_PlayFromMemory",      0x08,   (func_ptr)&Demo_AV_PlayFromMemory},
    {"DTV_StopFromMemory",      0x00,   (func_ptr)&Demo_AV_StopFromMemory},
#if (DEMO_MVC_TEST == 1)
    {"DTV_MVC_PlayFromMemory",  0x06,   (func_ptr)&Demo_AV_MVC_PlayFromMemory},
    {"DTV_MVC_StopFromMemory",  0x00,   (func_ptr)&Demo_AV_MVC_StopFromMemory},
#endif
#endif
#if (DEMO_VDEC_TEST == 1)
    {"DTV_CheckDecoderStatus",  0x00,   (func_ptr)&Demo_VDEC_CheckDecoderStatus},
    {"DTV_DelayTaskTime",       0x00,   (func_ptr)&Demo_VDEC_DelayTaskTime},
    {"DTV_WaitAVSync",          0x00,   (func_ptr)&Demo_VDEC_WaitAVSync},
    {"DTV_SetVideoDebugLevel",  0x00 ,  (func_ptr)&Demo_VDEC_SetVDECDebugLevel},
    {"VDEC_SetVDECDebugLevel",  0x00,   (func_ptr)&Demo_VDEC_SetVDECDebugLevel},
    {"VDEC_OpenDBGMSG",         0x00,   (func_ptr)&Demo_VDEC_OpenDBGMSG},
    {"VDEC_PreSetControl",      0x00,   (func_ptr)&Demo_VDEC_CmdPreSetControl},
    {"VDEC_DIPSyncOn",          0x00,   (func_ptr)&Demo_VDEC_FrameSyncOn},
#endif

#if (DEMO_PVR_TEST == 1)
    {"HelpPVR",      0x00, (func_ptr)&Demo_PVR_Help},
    {"PVR_Init",      0x02, (func_ptr)&Demo_PVR_Init},
    {"PVR_Exit",      0x00, (func_ptr)&Demo_PVR_Destroy},
    {"PVR_SetRecordType",    0x00, (func_ptr)&Demo_PVR_SetRecordType},
    {"PVR_SetRecIdx",    0x00, (func_ptr)&Demo_PVR_SetRecIdx},
    {"PVR_GetRecIdx",    0x00, (func_ptr)&Demo_PVR_GetRecIdx},
    {"PVR_SetRecSrcID",    0x00, (func_ptr)&Demo_PVR_SetRecSrcID},
    {"PVR_GetRecSrcID",    0x00, (func_ptr)&Demo_PVR_GetRecSrcID},
    {"PVR_RecFlowSet",    0x00, (func_ptr)&Demo_PVR_RecFlowSet},
    {"PVR_SetPlaybackPath",    0x00, (func_ptr)&Demo_PVR_SetPlaybackPath},
    {"PVR_GetPlaybackPath",    0x00, (func_ptr)&Demo_PVR_GetPlaybackPath},
    {"PVR_RecordStart",      0x20, (func_ptr)&Demo_PVR_Record_Start},
    {"PVR_RecordPause",      0x00, (func_ptr)&Demo_PVR_Record_Pause},
    {"PVR_RecordResume",      0x00, (func_ptr)&Demo_PVR_Record_Resume},
    {"PVR_RecordStop",      0x00, (func_ptr)&Demo_PVR_Record_Stop},
    {"PVR_SetPerformanceTest",    0x00, (func_ptr)&Demo_PVR_SetPerformanceTest},
    {"PVR_SetSyncConfig",    0x1E, (func_ptr)&Demo_PVR_SetSyncConfig},
    {"PVR_Disable2ndEncryption",    0x00, (func_ptr)&Demo_PVR_Disable2ndEncryption},

#if (DEMO_PVR_SUPPORT_CAPVR_TEST == 1)
#if (DEMO_DSCMB_ADVANCED_TEST == 1)
    {"PVR_Record_DSCMB_SetCAVID",      0x01, (func_ptr)&Demo_PVR_Record_DSCMB_SetCAVID},
#endif
    {"PVR_Record_DSCMB_Open",      0x07, (func_ptr)&Demo_PVR_Record_DSCMB_Open},
    {"PVR_Record_DSCMB_Close",      0x00, (func_ptr)&Demo_PVR_Record_DSCMB_Close},
    {"PVR_Record_DSCMB_Change",      0x07, (func_ptr)&Demo_PVR_Record_DSCMB_Change},
#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
    {"PVR_SetEncryptKeyIdx",    0x00, (func_ptr)&Demo_PVR_SetEncryptKeyIdx},
    {"PVR_EncryptFileCheck",           0x01, (func_ptr)&Demo_PVR_CheckEncryptFile},
#endif
#endif
    {"PVR_Record_ChangeProgram",      0x00, (func_ptr)&Demo_PVR_Record_ChangeProgram},
    {"PVR_PlaybackStart",      0x01, (func_ptr)&Demo_PVR_PlaybackStart},
    {"PVR_PlaybackPause",      0x00, (func_ptr)&Demo_PVR_PlaybackPause},
    {"PVR_PlaybackResume",      0x00, (func_ptr)&Demo_PVR_PlaybackResume},
    {"PVR_PlaybackStop",      0x00, (func_ptr)&Demo_PVR_PlaybackStop},
    {"PVR_PlaybackSF",      0x00, (func_ptr)&Demo_PVR_PlaybackSlowForward},
    {"PVR_PlaybackFF",      0x00, (func_ptr)&Demo_PVR_PlaybackFastForward},
    {"PVR_PlaybackFB",      0x00, (func_ptr)&Demo_PVR_PlaybackFastBackward},
    {"PVR_PlaybackSetSpeed",      0x00, (func_ptr)&Demo_PVR_PlaybackSetSpeed},
    {"PVR_PlaybackJumpToTime",      0x00, (func_ptr)&Demo_PVR_PlaybackJumpToTime},
    {"PVR_PlaybackChangeProgram",   0x20, (func_ptr)&Demo_PVR_PlaybackChangeProgram},
    {"PVR_TimeshiftRecordStart",    0x20, (func_ptr)&Demo_PVR_Timeshift_RecordStart},
    {"PVR_TimeshiftRecordStop",     0x00, (func_ptr)&Demo_PVR_Timeshift_RecordStop},
    {"PVR_TimeshiftPlaybackStart",  0x00, (func_ptr)&Demo_PVR_Timeshift_PlaybackStart},
    {"PVR_TimeshiftPlaybackStop",   0x00, (func_ptr)&Demo_PVR_Timeshift_PlaybackStop},
    {"PVR_TimeshiftStop",           0x00, (func_ptr)&Demo_PVR_Timeshift_Stop},
    {"PVR_SetTimeshiftRecScreenFrozen", 0x00, (func_ptr)&Demo_PVR_SetTimeshiftRecScreenFrozen},
    {"PVR_SetTimeshiftRecDuration", 0x00, (func_ptr)&Demo_PVR_SetTimeshiftRecDuration},
    {"PVR_GetFileInfo",           0x01, (func_ptr)&Demo_PVR_GetFileInfo},
    {"PVR_StepInPlay",           0x00, (func_ptr)&Demo_PVR_PlaybackStepIn},
    {"PVR_PlaybackWithNormalSpeed",           0x00, (func_ptr)&Demo_PVR_PlaybackWithNormalSpeed},
    {"PVR_CaptureThumbnail",      0x01, (func_ptr)&Demo_PVR_CaptureThumbnail},
    {"PVR_JumpToThumbnail",      0x01, (func_ptr)&Demo_PVR_JumpToThumbnail},
    {"PVR_SpeedCheck",           0x00, (func_ptr)&Demo_PVR_CheckUSBSpeed},
    {"PVR_HideThumbnail",           0x00, (func_ptr)&Demo_PVR_HideThumbnail},
    {"PVR_SetABLoop",           0x00, (func_ptr)&Demo_PVR_PlaybackABLoop},
    {"PVR_ResetABLoop", 0x00,(func_ptr)&Demo_PVR_PlaybackResetABLoop},
    {"PVR_SetSkipTime",           0x00, (func_ptr)&Demo_PVR_PlaybackAddSkipTime},
    {"PVR_RemoveSkipTime", 0x00, (func_ptr)&Demo_PVR_PlaybackRemoveSkipTime},
    {"PVR_SetRetentionTime", 0x0,(func_ptr)&Demo_PVR_SetPlaybackRetentionLimit},
    {"PVR_PlayJumpForward", 0x00, (func_ptr)&Demo_PVR_PlaybackJumpForward},
    {"PVR_PlayJumpBackward", 0x0,(func_ptr)&Demo_PVR_PlaybackJumpBackward},
    {"PVR_ShowTime", 0x00, (func_ptr)&Demo_PVR_ShowTime},
    {"PVR_RemoveFile", 0x01, (func_ptr)&Demo_PVR_RemoveFile},
    {"PVR_RecFlowSet",    0x00, (func_ptr)&Demo_PVR_RecFlowSet},
#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
    {"PVR_SeamlessBackRecordStart",    0x00, (func_ptr)&Demo_PVR_SeamlessBackRecordStart},
    {"PVR_SeamlessBackRecordStop",    0x00, (func_ptr)&Demo_PVR_SeamlessBackRecordStop},
#endif
#if (DEMO_PVR_SUPPORT_CHANGE_DISPLAYWIN_TEST == 1)
    {"PVR_ChangeDisplayWindowSize",    0x00, (func_ptr)&Demo_PVR_ChangeDisplayWindowSize},
#endif
    {"PVR_SetConfigure", 0x00, (func_ptr)&Demo_PVR_SetConfigure},
    {"PVR_SetDBGLevel",    0x00, (func_ptr)&Demo_PVR_SetDBGLevel},
    {"PVR_ShowAudioTrack",  0x00, (func_ptr)&Demo_PVR_ShowAudioTrackInfo},
    {"PVR_SetAudioTrack",   0x00, (func_ptr)&Demo_PVR_SetAudioTrackByIndex},
    {"PVR_SetRecPrecision", 0x00, (func_ptr)&Demo_PVR_SetRecordPrecision},
    {"PVR_SetBoundedLinearRecord", 0x00, (func_ptr)&Demo_PVR_SetBoundedLinearRecord},
    {"PVR_SetTimeshiftRecFileSize", 0x00, (func_ptr)&Demo_PVR_SetRecordFileSize},
    {"PVR_SetRecordAppend", 0x00, (func_ptr)&Demo_PVR_SetRecordAppend},
    {"PVR_SetRecFileSize", 0x00, (func_ptr)&Demo_PVR_SetRecordFileSize},
    {"PVR_SetRecDuration", 0x00, (func_ptr)&Demo_PVR_SetRecordDuration},
    {"PVR_SetRecFileSlice", 0x00, (func_ptr)&Demo_PVR_SetRecordFileSlice},
#endif


#if (DEMO_JPEG_TEST == 1)
    {"JPD_Decode",              0x00,   (func_ptr)&Demo_JPEG_Decode},
#endif

#if (DEMO_GPD_TEST == 1)
    {"GPD_Decode",              0x00,   (func_ptr)&Demo_GPD_Decode},
#endif


#if (DEMO_OS_TEST == 1)
    {"OS_Test",                 0x00,   (func_ptr)&Demo_OS_Test},
    {"USB_Test",                 0x01,   (func_ptr)&Demo_USB_Test},
#endif

#if (DEMO_USB_ECOS_TEST == 1)
    {"USB_BurnIn",               0x01,   (func_ptr)&Demo_USB_BurnIn},
    {"USB_BurnIn_2W",            0x01,   (func_ptr)&Demo_USB_BurnIn_2W},
    {"USB_API_perf",             0x00,   (func_ptr)&Demo_USB_API_perf},
    {"USB_driver_load_test",     0x00,   (func_ptr)&Demo_USB_driver_load_test},
#endif

#if (DEMO_WDT_TEST == 1)
    {"WDT_Test",                0x00,   (func_ptr)&Demo_WDT_Test},
#endif
#if (DEMO_SMC_TEST == 1)
    {"SMC_Start",               0x00,   (func_ptr)&Demo_SMC_Start},
    {"SMC_Stop",                0x00,   (func_ptr)&Demo_SMC_Stop},
    {"SMC_Help",                0x00,   (func_ptr)&Demo_SMC_Help},
    {"SMC_SetDBGLevel",         0x00,   (func_ptr)&Demo_SMC_SetDbgLevel},
#endif
#if (DEMO_OSD_TEST == 1)
    #if (DEMO_GOP_ZORDER_TEST == 1)
    {"ZOrder_Init",             0x00,   (func_ptr)&Demo_ZOrder_Init},
    {"ZOrder_SetZOrder",        0x00,   (func_ptr)&Demo_ZOrder_SetZOrder},
    #endif
    //GOP Demo Funs
    {"OSD_Help",                0x00,   (func_ptr)&Demo_OSD_Help},
    {"OSD_Init",                0x00,   (func_ptr)&Demo_OSD_Init},
    {"OSD_PTGOP_Init",          0x00,   (func_ptr)&Demo_OSD_PTGOP_Init},
    {"OSD_SetConfig",           0x00,   (func_ptr)&Demo_OSD_SetConfig},
    {"OSD_CreateWin",           0x00,   (func_ptr)&Demo_OSD_CreateWin},
    {"OSD_DrawBmp",             0x00,   (func_ptr)&Demo_OSD_DrawBmp},
    #if (DEMO_GOP_3DOSD_TEST == 1)
    {"OSD_3DOSD",               0x00,   (func_ptr)&Demo_OSD_Draw3DOSD},
    #endif
    {"OSD_HVScale",             0x00,   (func_ptr)&Demo_OSD_HVScale},
    {"OSD_DWinCapture",         0x00,   (func_ptr)&Demo_OSD_DWinCapture},
    #if (DEMO_DWIN_PINPON_RINGBUF_TEST == 1)
    {"OSD_DWinPingPong",        0x00,   (func_ptr)&Demo_OSD_DWinPingPong},
    #endif
    {"OSD_DWinSkip",            0x00,   (func_ptr)&Demo_OSD_DWinSkipFrame},
    {"OSD_EnableWin",           0x00,   (func_ptr)&Demo_OSD_EnableWin},
    {"OSD_FreeWin",             0x00,   (func_ptr)&Demo_OSD_FreeWin},
    {"OSD_SetGlobalAlpha",      0x00,   (func_ptr)&Demo_OSD_SetGlobalAlpha},
    {"OSD_EnablePerPixelAlpha", 0x00,   (func_ptr)&Demo_OSD_EnablePerPixelAlpha},
    {"OSD_SetFade",             0x00,   (func_ptr)&Demo_OSD_SetFade},
    {"OSD_FillBlock",           0x00,   (func_ptr)&Demo_OSD_FillBlock},
    {"OSD_DrawLine",            0x00,   (func_ptr)&Demo_OSD_DrawLine},
    {"OSD_DrawBmpXY",           0x00,   (func_ptr)&Demo_OSD_DrawBmpXY},

    {"OSD_DrawBmpRotate",       0x00,   (func_ptr)&Demo_OSD_DrawBmpRotate},
    {"OSD_DrawBmpScale",        0x00,   (func_ptr)&Demo_OSD_DrawBmpScale},
    {"OSD_ConvertColor",        0x00,   (func_ptr)&Demo_OSD_ConvertColor},
    {"OSD_ScreenCopyRop",       0x00,   (func_ptr)&Demo_OSD_ScreenCopyRop},

    {"OSD_BitBltSample",        0x00,   (func_ptr)&Demo_OSD_BitBltSample},
    {"OSD_TextOut",             0x00,   (func_ptr)&Demo_OSD_TextOut},
    {"OSD_SetGWinFmt",          0x00,   (func_ptr)&Demo_OSD_SetGWinFmt},
    #if (DEMO_GE_TWOSOURCEBITBLT_TEST == 1)
    {"OSD_TwoSourceBitblt",     0x00,   (func_ptr)&Demo_OSD_TwoSourceBitblt},
    #endif
    {"OSD_DrawJpeg",            0x00,   (func_ptr)&Demo_OSD_DrawJpeg},

    #if (DEMO_GOP_GOPSC_TEST == 1)
    //GOPSC Demo Funs
    {"OSD_GOPSC_Init",          0x00,   (func_ptr)&Demo_OSD_GOPSC_Init},
    {"OSD_GOPSC_HVSP",          0x00,   (func_ptr)&Demo_OSD_GOPSC_HVScalingProcess},
    #endif

    #if (DEMO_DTE_TEST == 1)
    // Dimension trans Part
    {"Dim_SetConfig",0x00,(func_ptr)&Demo_Dimension_SetConfig},
    {"Dim_2dto3d",0x01,(func_ptr)&Demo_Dimension_2Dto3D},
    {"Dim_Help",0x00,(func_ptr)&Demo_Dimension_Help},
    #endif
#endif

#if (DEMO_XC_TEST == 1)
    {"HelpXC",                  0x00,   (func_ptr)&Demo_XC_Help},
    {"XC_Init",                 0x00,   (func_ptr)&Demo_XC_Init},
    {"XC_SetOutputTiming",      0x00,   (func_ptr)&Demo_XC_SetOutputTiming},
    {"XC_SetOutputDest",        0x00,   (func_ptr)&Demo_XC_SetOutputDest},
    {"XC_DACDetect",            0x00,   (func_ptr)&Demo_Video_DACDetect},
    {"XC_PlayVideo",            0x00,   (func_ptr)&Demo_XC_PlayVideo},
    {"XC_PlayCusVideo",         0x00,   (func_ptr)&Demo_XC_PlayCusVideo},
    {"XC_StopVideo",            0x00,   (func_ptr)&Demo_XC_StopVideo},
    {"XC_SetCropWin",           0x00,   (func_ptr)&Demo_XC_SetCropWin},
    {"XC_SetDestWin",           0x00,   (func_ptr)&Demo_XC_SetDestWin},
    {"XC_SetAS",                0x00,   (func_ptr)&Demo_XC_SetAS},
    {"XC_SetVideoMute",         0x00,   (func_ptr)&Demo_XC_SetVideoMute},
    {"XC_SetMainWindowFirst",   0x00,   (func_ptr)&Demo_XC_SetMainWindowFirst},
    {"XC_EnableWindow",         0x00,   (func_ptr)&Demo_XC_EnableWindow},
    {"XC_RegressionTest_ScalingDown",       0x00,   (func_ptr)&Demo_XC_RegressionTest_ScalingDown},
    {"XC_DisplayWindowTest",    0x00,   (func_ptr)&Demo_XC_DisplayWindowTest},
    {"XC_SetFrameBufferSize",   0x00,   (func_ptr)&Demo_XC_SetFrameBufferSize},
    {"VE_Init",                 0x00,   (func_ptr)&Demo_VE_Init},
    {"VE_SetOutputTiming",      0x00,   (func_ptr)&Demo_VE_SetOutputTiming},
    {"VE_PlayVideo",            0x00,   (func_ptr)&Demo_VE_PlayVideo},
    {"VE_PlayCusVideo",         0x00,   (func_ptr)&Demo_VE_PlayCusVideo},
    {"VE_SetVideoMute",         0x00,   (func_ptr)&Demo_VE_SetVideoMute},
    {"VE_SetPath",              0x00,   (func_ptr)&Demo_VE_SetPath},
    {"VE_SetSrcWin",            0x00,   (func_ptr)&Demo_VE_SetSrcWin},
    {"VE_SetDestWin",           0x00,   (func_ptr)&Demo_VE_SetDestWin},
    {"VE_SetWinAll",            0x00,   (func_ptr)&Demo_VE_SetWinAll},
    {"VE_SetAS",                0x00,   (func_ptr)&Demo_VE_SetAS},

    #if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
    {"XC_EnableSeamlessZapping",0x00,   (func_ptr)&Demo_XC_EnableSeamlessZapping},
    {"XC_SeamlessZapping",      0x00,   (func_ptr)&Demo_XC_SeamlessZapping},
    {"XC_SeamlessZapping_SetFreeze",        0x00,   (func_ptr)&Demo_XC_SeamlessZapping_SetFreeze},
    #endif
    #if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
    {"XC_EnableStillImage",     0x00,   (func_ptr)&Demo_XC_EnableStillImage},
    #if (DEMO_XC_DUALXC_TEST == 1)
    {"XC_EnableStillImage_ByDevice",        0x00,   (func_ptr)&Demo_XC_EnableStillImage_ByDevice},
    #endif
    #endif
    #if (DEMO_XC_3D_TEST == 1)
    {"XC_Enable3D",             0x00,   (func_ptr)&Demo_XC_Enable3D},
    {"XC_PlayVideo_3D",         0x00,   (func_ptr)&Demo_XC_PlayVideo_3D},
    #endif
    #if (DEMO_XC_DSSWDB_TEST == 1)
    {"XC_EnableDS_SWDB",        0x00,   (func_ptr)&Demo_XC_EnableDS_SWDB},
    {"XC_ChangeWinSize",        0x00,   (func_ptr)&Demo_XC_ChangeWindowSize},
    #endif

    {"XC_SetBrightness",        0x00,   (func_ptr)&Demo_XC_SetBrightness},
    {"XC_SetSharpness",         0x00,   (func_ptr)&Demo_XC_SetSharpness},
    {"XC_SetContrast",          0x00,   (func_ptr)&Demo_XC_SetContrast},
    {"XC_SetHue",               0x00,   (func_ptr)&Demo_XC_SetHue},
    {"XC_SetSaturation",        0x00,   (func_ptr)&Demo_XC_SetSaturation},
    {"XC_SetFrameColor",        0x00,   (func_ptr)&Demo_XC_SetFrameColor},
    {"XC_SetVideoAlpha",        0x00,   (func_ptr)&Demo_XC_SetVideoAlpha},
    #if (DEMO_XC_HDMIRX_TEST == 1)
    {"XC_SetHDMIByPass",        0x00,   (func_ptr)&Demo_HDMI_SetRxBypass},
    {"XC_SetHDMIColorThroughMode",  0x00,   (func_ptr)&Demo_HDMI_SetColorThroughMode},
    {"XC_GetHDMIRXPacketInfo",  0x00,   (func_ptr)&Demo_HDMI_GetRXPacketInfo},
    {"HDMI_SetRXEDID",          0x00,   (func_ptr)&Demo_HDMI_SetRxEDID},
    #endif
    {"XC_EnableDrvDmsg",        0x00,   (func_ptr)&Demo_XC_EnableDmesg},
    {"XC_Exit",                 0x00,   (func_ptr)&Demo_XC_Exit},
    {"XC_SetHDREnable",                 0x00,   (func_ptr)&Demo_XC_SetHDREnable},
    {"XC_SetHDRSeamlessEnable", 0x00,   (func_ptr)&Demo_XC_SetHDRSeamlessFlag},
    {"VE_Exit",                 0x00,   (func_ptr)&Demo_VE_Exit},
    {"VE_VbiTtxDemo",           0x00,   (func_ptr)&Demo_VE_VbiTtxDemo},
    {"VE_CCIROut",              0x00,   (func_ptr)&Demo_VE_CCIROut},
    {"VE_SetWSS",               0x00,   (func_ptr)&Demo_VE_SetWSS},
    {"VE_Set525WSS",            0x00,   (func_ptr)&Demo_VE_Set525WSS},
    {"VE_SetFrameColor",        0x00,   (func_ptr)&Demo_VE_SetFrameColor},
    {"VE_SetVideoAlpha",        0x00,   (func_ptr)&Demo_VE_SetVideoAlpha},
    {"ACP_Set625CGMS_A",        0x00,   (func_ptr)&Demo_ACP_Set625CGMS_A},
    {"ACP_Set525CGMS_A_IEC61880",       0x00,   (func_ptr)&Demo_ACP_Set525CGMS_A_IEC61880},
    {"ACP_Set525APS_IEC61880",  0x00,   (func_ptr)&Demo_ACP_Set525APS_IEC61880},
    {"ACP_SetMV",               0x00,   (func_ptr)&Demo_ACP_SetMV},
    #if (DEMO_ACP_DCS_TEST == 1)
    {"ACP_SetDCS",              0x00,   (func_ptr)&Demo_ACP_SetDCS},
    #endif
#if (DEMO_XC_DOVI_TEST == 1)
    {"XC_SetDoViGraphicPriority",0x00,  (func_ptr)&Demo_XC_SetDolbyVisionGraphicPriority},
    {"XC_EnableDoViGraphicLum",  0x00,  (func_ptr)&Demo_XC_SetDolbyVisionGraphicLuminanceEnable},
    {"XC_SetDoViGraphicLum",     0x00,  (func_ptr)&Demo_XC_SetDolbyVisionGraphicLuminance},
    {"HDMI_ShowDoViHDRInfo",     0x00,  (func_ptr)&Demo_HDMI_ShowDoViHDRInfo},
    {"HDMI_SetDoViOutputFormat",0x00,  (func_ptr)&Demo_HDMI_SetDolbyVisionOutputFormat},
    {"HDMI_SetDoViLowLatencyMode",0x00,  (func_ptr)&Demo_HDMI_SetDolbyVisionLowLatencyMode},
#endif
#if (DEMO_DAC_SD_ONLY_TEST == 0)
    {"HDMI_SetTxOutputMode",    0x00,   (func_ptr)&Demo_HDMI_SetTxOutputMode},
#if (DEMO_HDMI8bits_ONLY_TEST == 0)
    {"HDMI_SetOutputColorDepth",0x00,   (func_ptr)&Demo_HDMI_SetOutputColorDepth},
#endif
    {"HDMI_SetOutputColorRange", 0x00,  (func_ptr)&Demo_HDMI_SetOutputColorRange},
    {"HDMI_ShowHDCPInfo",        0x00,  (func_ptr)&Demo_HDMI_ShowHDCPInfo},
    {"HDMI_SetOutputColorFormat",0x00,  (func_ptr)&Demo_HDMI_SetOutputColorFormat},
#if (DEMO_XC_3D_TEST == 1)
    {"HDMI_Set3DStructure",     0x00,   (func_ptr)&Demo_HDMI_Set3DStructure},
#endif
    {"HDMI_SetAspectRatio",     0x00,   (func_ptr)&Demo_HDMI_SetAspectRatio},
    {"HDMI_SetAVMute",          0x00,   (func_ptr)&Demo_HDMI_SetAVMute},
    {"HDMI_SetColorimetry",     0x00,   (func_ptr)&Demo_HDMI_SetColorimetry},
#if (DEMO_AUDIO_HDMI_TEST == 1)
    {"HDMI_SetAudio",           0x00,   (func_ptr)&Demo_HDMI_SetAudio},
#endif
    {"HDMI_ShowHDMITxOutputInfo",0x00,  (func_ptr)&Demo_HDMI_ShowHDMITxOutputInfo},
#if (DEMO_HDMI_CTS_CERTIFICATION_TEST == 1)
    {"HDMI_EnableCertification",0x00,   (func_ptr)&Demo_HDMI_EnableCertification},
#endif
    {"HDMI_GetEDIDInfo",        0x00,   (func_ptr)&Demo_HDMI_GetEDIDInfo},
#endif //#if (DEMO_DAC_SD_ONLY_TEST == 0)
    #if (DEMO_SCART_OUTPUT_SWITCH_TEST == 1)
    {"SCART_OutputSwitch",      0x00,   (func_ptr)&Demo_SCART_OutputSwitch},
    #endif
#endif

#if (DEMO_CEC_TEST == 1)
    {"CEC_Init",                0x00,   (func_ptr)&Demo_CEC_init},
    {"CEC_Exit",                0x00,   (func_ptr)&Demo_CEC_Exit},
    {"CEC_SetOnOff",            0x00,   (func_ptr)&Demo_CEC_SetOnOff},
    {"CEC_SendMessage",         0x00,   (func_ptr)&Demo_CEC_SendMessage},
    {"CEC_Show_Device_List",    0x00,   (func_ptr)&Demo_CEC_Show_Device_List},
    {"CEC_OneTouchPlay",        0x00,   (func_ptr)&Demo_CEC_OneTouchPlay},
    {"CEC_Standby",             0x00,   (func_ptr)&Demo_CEC_Standby},
    {"CEC_GiveDevicePowerStatus", 0x00, (func_ptr)&Demo_CEC_GiveDevicePowerStatus},
    {"CEC_SystemAudioModeRequest",0x00, (func_ptr)&Demo_CEC_SystemAudioModeRequest},
    {"CEC_ShowMessageBuffer",   0x00,   (func_ptr)&Demo_CEC_ShowMessageBuffer},
#endif

#if (DEMO_LINUX_MOUNT_NOTIFIER_TEST == 1)
    {"MountNotifier_Init",      0x00,   (func_ptr)&Demo_MountNotifier_Init},
#endif

#if (DEMO_UART_TEST == 1)
    {"UART_RX_Test",            0x00,   (func_ptr)&Demo_UART_RX_Test},
#endif

#if (DEMO_DVFS_TEST == 1)
    {"DVFS_LockCPUFreq",        0x00,   (func_ptr)&Demo_DVFS_Lock_CPU_Frequency     },
    {"DVFS_UnLockFreq",         0x00,   (func_ptr)&Demo_DVFS_UnLock_CPU_Frequency   },
    {"DVFS_GetCPUFreq",         0x00,   (func_ptr)&Demo_DVFS_Get_CPU_Frequency      },
    {"DVFS_GetCPUTemp",         0x00,   (func_ptr)&Demo_DVFS_Get_CPU_Temperature    },
    {"DVFS_GetCPUVolt",         0x00,   (func_ptr)&Demo_DVFS_Get_CPU_Voltage        },
#endif

#if (DEMO_TSENSOR_TEST == 1)
#if (DEMO_SAR_TSENSOR_SUPPORT == 1)
    {"TSensor_GetTemp",             0x00,   (func_ptr)&Demo_TSensor_GetTemp},
    {"TSensor_SetTempRange",             0x00,   (func_ptr)&Demo_TSensor_SetTempMonitorRange},
    {"TSensor_GetTempRange",             0x00,   (func_ptr)&Demo_TSensor_GetTempMonitorRange},
    {"TSensor_ClearIRQ",             0x00,   (func_ptr)&Demo_TSensor_ClearIRQ},
#else
    {"TSensor_DemoISR",         0x00,   (func_ptr)&Demo_TSensor_demo},
    {"TSensor_PRI",             0x00,   (func_ptr)&Demo_TSensor_PriTemp},
#endif
#endif

#if (DEMO_DTM_TEST == 1)
    {"DTM_Start",               0x00,   (func_ptr)&Demo_DTM_Start},
    {"DTM_SetThreshold",        0x00,   (func_ptr)&Demo_DTM_SetThreshold},
#endif

#if (DEMO_FRONTPNL_TEST == 1)
    {"FrontPnl_Init",           0x00,   (func_ptr)&Demo_FrontPnl_Init },
    {"FrontPnl_ShowEnable",     0x00,   (func_ptr)&Demo_FrontPnl_ShowOpen_Enable},
    {"FrontPnl_LedEnable",      0x00,   (func_ptr)&Demo_FrontPnl_LED_Enable},
    #if (FRONTPNL_TYPE == FRONTPANEL_TYPE_PT6958)
    {"FrontPnl_DotEnable",      0x00,   (func_ptr)&Demo_FrontPnl_Dot_Enable},
    {"FrontPnl_DigitalSet",     0x00,   (func_ptr)&Demo_FrontPnl_Digital_Set},
    #endif

    {"FrontPnl_KeyEnable",     0x00,   (func_ptr)&Demo_FrontPnl_KeyEnable},
#endif

#if (DEMO_KEYPAD_TEST == 1)
    {"Keypad_Init",             0x00,   (func_ptr)&Demo_Keypad_Init },
#endif

#if (DEMO_IR_ECOS_TEST == 1)
    {"HelpIR_ecos",             0x00,   (func_ptr)&Demo_Input_Help_ecos},
    {"IR_Init",                 0x00,   (func_ptr)&Demo_Input_Init_ecos},
    {"IR_Exit_ecos",            0x00,   (func_ptr)&Demo_Input_Exit_ecos},
#endif

#if (DEMO_IR_TX_TEST == 1)
    {"IR_TX_SetIRType",         0x00,   (func_ptr)&Demo_IR_TX_SetIRType},
    {"IR_TX_SendKeyCode",       0x00,   (func_ptr)&Demo_IR_TX_SendKeyCode},
#endif


#if (DEMO_IR_LINUX_TEST == 1)
    {"IR_Init",                 0x00,   (func_ptr)&Demo_Input_Init_linux},
    {"IR_Exit_Linux",           0x00,   (func_ptr)&Demo_Input_Exit_linux},
#endif

#if (DEMO_PCMCIA_TEST == 1)
    {"Demo_PCM",                0x00,   (func_ptr)&Demo_PCMCIA},
#endif

#if (DEMO_SPI_TEST == 1)
    {"SerialFlash_Init",        0x00,   (func_ptr)&Demo_SerFlash_Init},
    {"SerialFlash_ReadID",      0x00,   (func_ptr)&Demo_SerFlash_ReadID},
    {"SerialFlash_GetInfo",     0x00,   (func_ptr)&Demo_SerFlash_GetInfo},
    {"SerialFlash_RWTest",      0x00,   (func_ptr)&Demo_SerFlash_RWTest},
    {"Mspi_MutiUser_Test",      0x00,   (func_ptr)&Demo_MSPI_LoopBack_Test},
#endif

#if (DEMO_GPIO_TEST ==1)
    {"Gpio_Init",               0x00,   (func_ptr)&Demo_Gpio_Init},
    {"Gpio_Set",                0x00,   (func_ptr)&Demo_Gpio_Set},
    {"Gpio_GetLevel",           0x00,   (func_ptr)&Demo_Gpio_GetLevel},
#endif

#if (DEMO_RSA_TEST ==1)
    {"Demo_RSA_DoRSADemo",      0x00,   (func_ptr)&Demo_RSA_DoRSADemo},
    {"Demo_RSA_DoRSADemo_HwKey",0x00,   (func_ptr)&Demo_RSA_DoRSADemo_HwKey},
#endif

#if (DEMO_AESDMA_TEST ==1)
    {"AESDMA_Demo",             0x00,   (func_ptr)&Demo_AESDMA_DoAESDMADemo},
#endif

#if (DEMO_CIPHER_TEST ==1)
    {"CIPHER_Demo",             0x00,   (func_ptr)&Demo_CIPHER_DoCIPHERDemo},
    {"CIPHER_Demo_Rand",        0x00,   (func_ptr)&Demo_CIPHER_RandonNumber},
#endif

#if (DEMO_OPENGLES2_TEST == 1)
    {"Demo_OpenGL",      	0x00,   (func_ptr)&demo_opengl},
    {"Demo_OpenGLBenchmark",	0x1f,   (func_ptr)&demo_OpenglBenchmark},
#endif

#if (DEMO_DRM_TEST == 1)
    {"DRM_RE_ENCRYPT",                  0x00,  (func_ptr)&Demo_DRM_PlayReady_Re_encrypt},
#ifndef DDI_TEE
    //{"DRM_SECURE_AUTO_TEST",            0x00,  (func_ptr)&Demo_DRM_Secure_Autotest},
    {"DRM_SECURE_STORAGE_AUTO_TEST",    0x00,  (func_ptr)&Demo_DRM_Secure_Storage_Autotest},
    {"DRM_SetProductKey",               0x00,  (func_ptr)&Demo_DRM_SetProductKey},
#endif
#if (DEMO_WIDEVINE_TEST == 1)
    {"DRM_OEMCrypto_Test",              0x00,  (func_ptr)&Demo_DRM_OEMCryptoTest},
#endif
#endif
#if (DEMO_PLAYREADY_TEST == 1)
    {"playready_test",                  0x00,  (func_ptr)&appDemo_PlayReadyTest},
#endif

#if (DEMO_HBBTV_TEST == 1)
    //{"hbbtv_TuneAndPlayFor100825",    0x00,  (func_ptr)&Demo_hbbtv_TuneAndPlayFor100825},
    {"hbbtv_Init",                  0x00,  (func_ptr)&Demo_hbbtv_Init},
    {"hbbtv_Deinit",                0x00,  (func_ptr)&Demo_hbbtv_DeInit},
    {"hbbtv_StartService",          0x00,  (func_ptr)&Demo_hbbtv_StartService},
    {"hbbtv_StopService",           0x00,  (func_ptr)&Demo_hbbtv_StopService},
    {"hbbtv_AITSendFor100825",      0x00,  (func_ptr)&Demo_hbbtv_AITSendFor100825},
    {"hbbtv_Script",                0x00,  (func_ptr)&Demo_hbbtv_Script},
    {"hbbtv_PlayDTV",               0x00,  (func_ptr)&Demo_hbbtv_PlayDTV},
    {"hbbtv_PrepareCh",             0x00,  (func_ptr)&Demo_hbbtv_PrepareServiceForDemo},
    {"hbbtv_SI_dumpdata",           0x01,  (func_ptr)&Demo_hbbtv_Util_Dump_SIData},
    {"hbbtv_Script2",               0x01,  (func_ptr)&Demo_hbbtv_Script2},
    {"hbbtv_pollingAIT",            0x00,  (func_ptr)&Demo_hbbtv_PollingAIT},
    {"hbbtv_ait",                   0x00,  (func_ptr)&Demo_hbbtv_NotifyAIT_Command},
    {"hbbtv_ligada",                0x00,  (func_ptr)&Demo_hbbtv_Ligada},
#endif

    {"DigiTuner_SetIndex",      0x00,   (func_ptr)&appDemo_DigiTuner_SetIndex},
    {"DigiTuner_SetFreq",       0x00,   (func_ptr)&appDemo_DigiTuner_SetFreq},
    {"FE_SetDetectMode",        0x00,   (func_ptr)&appDemo_Frontend_SetDetectMode},
    {"FE_SetBroadcastType",     0x00,   (func_ptr)&appDemo_Frontend_SetBroadcastType},
    {"FE_SetInExteralPath",     0x00,   (func_ptr)&appDemo_Frontend_SetInExteralPath},
    {"FE_SetFrontEnd",          0x00,   (func_ptr)&appDemo_Frontend_SetTunerDemod},
    {"FE_SetFrontReset",        0x00,   (func_ptr)&appDemo_Frontend_HWReset},
    {"FE_GetFrontendList",      0x00,   (func_ptr)&appDemo_Frontend_FEListShow},
    {"FE_InitFrontend",         0x00,   (func_ptr)&appDemo_Frontend_Init},
    {"FE_SetPIDFlt",            0x00,   (func_ptr)&appDemo_Frontend_SetDMD_PIDFlt},
    #if(MS_DVBT2_INUSE == 1)
    {"DigiTuner_GetPlpInfo",    0x00,   (func_ptr)&appDemo_DigiTuner_GetPlpInfo},
    {"DigiTuner_SetMPLP",       0x00,   (func_ptr)&appDemo_DigiTuner_SetMPLP},
    #endif
    {"DigiTuner_GetLock",       0x00,   (func_ptr)&appDemo_DigiTuner_CheckLock},
    #if(MS_DVBS_INUSE == 1)
    {"DigiTuner_SetLNBPower",   0x00,   (func_ptr)&appDemo_DigiTuner_SetLNBPower},
    {"DigiTuner_Set22K",        0x00,   (func_ptr)&appDemo_DigiTuner_Satellite_Set22K},
    {"DigiTuner_Set22KTone",    0x00,   (func_ptr)&appDemo_DigiTuner_Satellite_Set22KTone},
    {"FE_DiseqcCmd",            0x00,   (func_ptr)&appDemo_FE_DiseqcCmd},
    {"FE_GetISIDInfo",          0x00,   (func_ptr)&appDemo_DigiTuner_GetVCM_ISID_Info},
    {"FE_SetISID",              0x00,   (func_ptr)&appDemo_DigiTuner_SetVCM_ISID},
    {"FE_SetLNB_LO",             0x00,   (func_ptr)&appDemo_DigiTuner_SetLNBLOF},
    #endif
    {"FE_GetParam",             0x00,   (func_ptr)&appDemo_DigiTuner_GetParam},
    {"FE_RFStatusPolling",      0x00,   (func_ptr)&appDemo_DigiTuner_GetSignalStatus},


#if (FE_AUTO_TEST == 1)
    {"FE_AutoTest",             0x00,   (func_ptr)&appDemo_FEAutoTest_Start},
    #endif
#if (ECOS_WIFI_SUPPORT == 1)
    {"Wlan_Init",               0x00,   (func_ptr)&appDemo_Wlan_Init},
    {"Wlan_Scan",               0x00,   (func_ptr)&appDemo_Wlan_Scan},
    {"Wlan_Connect",            0x09,   (func_ptr)&appDemo_Wlan_Connect},
    {"Wlan_Ping",               0x01,   (func_ptr)&appDemo_Wlan_Ping},
    {"Wlan_Help",               0x01,   (func_ptr)&appDemo_Wlan_Help},
#endif

#if (DEMO_APM_TEST == 1)
    {"HelpAPM",         0x00, (func_ptr)&Demo_APM_Help},
    {"APM_Help",        0x00, (func_ptr)&Demo_APM_Help},
    {"APM_RUN",         0x00, (func_ptr)&Demo_APM_Requestrun},
#endif

#if (DEMO_DFB_TEST == 1)
    //DFB Demo Funs
    {"DFB",         0x00, (func_ptr)&Demo_DFB},
    {"DFB_res",          0x00,   (func_ptr)&Demo_DFB_ChangeResolution},

#endif

#if (DEMO_GST_TEST == 1)
    {"gst_init",         0x00, (func_ptr)&Demo_GST_PLAYER_Init},
    {"gst_play",         0x01, (func_ptr)&Demo_GST_PLAYER_Play},
    {"gst_stop",         0x00, (func_ptr)&Demo_GST_PLAYER_Stop},
    {"gst_seek",         0x00, (func_ptr)&Demo_GST_PLAYER_Seek},
    {"gst_ff",           0x00, (func_ptr)&Demo_GST_PLAYER_FF},
    {"gst_fb",           0x02, (func_ptr)&Demo_GST_PLAYER_FB},
    {"gst_pause",        0x02, (func_ptr)&Demo_GST_PLAYER_Pause},
    {"gst_resume",       0x02, (func_ptr)&Demo_GST_PLAYER_Resume},
    {"gst_show",         0x01, (func_ptr)&Demo_GST_PLAYER_PosShow},
    {"gst_deinit",       0x00, (func_ptr)&Demo_GST_PLAYER_DeInit},
#endif

#if (DEMO_QT_TEST == 1)
    {"QT_Run",         0x00, (func_ptr)&Demo_HelloWorld},
#endif

#if (DEMO_BOOST_TEST == 1)
    {"boost_regx",       0x00,  (func_ptr)&Demo_BOOST_REGX},
    {"boost_lambda",       0x00,  (func_ptr)&Demo_BOOST_LAMBDA},
    {"boost_lexical",       0x00,  (func_ptr)&Demo_BOOST_LEXICAL_CAST},
    {"boost_any",       0x00,  (func_ptr)&Demo_BOOST_ANY},
    {"boost_chrono",       0x00,  (func_ptr)&Demo_BOOST_CHRONO},
    {"boost_thread",       0x00,  (func_ptr)&Demo_BOOST_THREAD},
#endif

#if (DEMO_NET_ECOS_TEST == 1)
    {"Demo_Net_Init",               0x00,   (func_ptr)&Demo_Net_Init},
    {"Demo_Net_GetConnectionStatus",0x00,   (func_ptr)&Demo_Net_GetConnectionStatus},
    {"Demo_Net_GetMac",             0x00,   (func_ptr)&Demo_Net_GetMac},
    {"Demo_Net_SetMac",             0x00,   (func_ptr)&Demo_Net_SetMac},
    {"Demo_Net_Ping",               0x01,   (func_ptr)&Demo_Net_Ping},
    {"Demo_Net_Help",               0x00,   (func_ptr)&Demo_Net_Help},
    {"Demo_Net_GetDHCPStatus",      0x00,   (func_ptr)&Demo_Net_GetDHCPStatus},
    {"Demo_Net_DHCP",               0x00,   (func_ptr)&Demo_Net_DHCP},
#endif

#if (DEMO_TSPLAYER_TEST == 1)
    {"ts_init",         0x00, (func_ptr)&Demo_TSPLAYER_Init},
    {"ts_play",         0x00, (func_ptr)&Demo_TSPLAYER_Play},
#endif

#if (DEMO_WIDI_TEST == 1)
    {"widi",         0x00, (func_ptr)&Demo_widi},
#endif

#if (DEMO_eCos_API_TEST == 1)
    {"CPU_usage",         0x00, (func_ptr)&Demo_eCos_CPUUsage},
    {"cpu_show",          0x00, (func_ptr)&Demo_eCos_show_cpu_average_load},
#endif

#if (DEMO_BOOTLOGO_TEST == 1)
    {"Bootlogo_close",         0x00, (func_ptr)&Demo_BOOTLOGO_Close},
    {"Bootlogo_checkenv",      0x00, (func_ptr)&Demo_BOOTLOGO_CheckEnv},
#endif

#if (DEMO_BOOTVIDEO_TEST == 1)
    {"BOOTVIDEO_DwinCapture",     0x00,   (func_ptr)&Demo_BOOTVIDEO_DwinCapture},
    {"BOOTVIDEO_Stop",            0x00,   (func_ptr)&Demo_BOOTVIDEO_Stop},
    {"BOOTVIDEO_DestroyGwin",     0x00,   (func_ptr)&Demo_BOOTVIDEO_DesrtoyGwin},
#endif

#if (DEMO_EMMC_ECOS_TEST == 1)
    {"emmc_init",   0x00,   (func_ptr)&Demo_EMMC_Init},
    {"emmc_write",  0x00,   (func_ptr)&Demo_EMMC_WriteData},
    {"emmc_read",   0x00,   (func_ptr)&Demo_EMMC_ReadData},
    {"emmc_dump",   0x00,   (func_ptr)&Demo_EMMC_DumpData},
    {"emmc_compare",0x00,   (func_ptr)&Demo_EMMC_Compare},
    {"emmc_verify",   0x00, (func_ptr)&Demo_EMMC_Verify},
#endif

#if (DEMO_NAND_ECOS_TEST == 1)
    {"NAND_WriteCIS",       0x00,   (func_ptr)&Demo_NAND_WriteCIS},
    {"NAND_NandInit",       0x00,   (func_ptr)&Demo_NAND_Init},
    {"NAND_Compare",        0x00,   (func_ptr)&Demo_NAND_Compare},
    {"NAND_unfdInit",       0x00,   (func_ptr)&Demo_UNFD_Init},
    {"NAND_GetFATCapacity", 0x00,   (func_ptr)&Demo_NAND_GetFATCapacity},
    {"NAND_WriteFAT",       0x00,   (func_ptr)&Demo_NAND_WriteFAT},
    {"NAND_ReadFAT",        0x00,   (func_ptr)&Demo_NAND_ReadFAT},
    {"NAND_Verify",         0x00,   (func_ptr)&Demo_NAND_Verify},
#endif

#if (DEMO_CH_SCAN_TEST == 1)
    {"ChScan_Auto",         0x00,   (func_ptr)&Demo_ChScan_Auto},
#endif

#if(DEMO_ZUI_TEST == 1)
    {"UI_Init",             0x00,   (func_ptr)&Demo_UI_Create},
    {"UI_Exit",             0x00,   (func_ptr)&Demo_UI_Exit},
    {"UI_GEGOPInit",        0x00,   (func_ptr)&Demo_UI_GEGOPInit},
#endif
#if(DEMO_AUTOIN_ZUI_TEST == 1)
    {"UI_AutoIn",           0x00,   (func_ptr)&Demo_UI_AutoIn},
#endif
#if (DEMO_ECOS_BOOT_LINUX == 1)
    {"demo_bootlinux32",      0x00,   (func_ptr)&Demo_Boot_Linux32},
    {"demo_bootlinux64",      0x00,   (func_ptr)&Demo_Boot_Linux64},
    {"demo_bootlinux64_optee",      0x00,   (func_ptr)&Demo_Boot_Linux64_OPTEE},
#endif
#if(DEMO_GPU_BENCHMARK_TEST == 1)
    {"demo_open_off_scr_gpu_benchmark",     0x00,   (func_ptr)&Demo_Open_off_screen_gpu_benchmark},
    {"demo_get_off_scr_gpu_utilization",    0x00,   (func_ptr)&Demo_Get_off_screen_gpu_utilization},
#endif
#if (DEMO_SYS_INFO_TEST == 1)
    {"demo_cpu",            0x00,   (func_ptr)&Demo_cpu_information     },
    {"demo_mem",            0x00,   (func_ptr)&Demo_memory_information  },
    {"demo_bw",             0x00,   (func_ptr)&Demo_miu_bandwidth       },
    {"demo_printk_level",   0x00,   (func_ptr)&Demo_printk_level        },
#endif
#if (DEMO_FASTCALL_ECOS_TEST == 1)
    {"demo_fastcall",       0x00,   (func_ptr)&Demo_FASTCALL_DoFASTCALLDemo},
#endif
#if (DEMO_CI_TEST == 1)
    //Demo CI Func
    {"CI_Start",      0x00,  (func_ptr)&Demo_CI_Start},
    {"CI_Setpath",    0x00,  (func_ptr)&Demo_CI_SetPath},
#endif
#if (DEMO_PROCFS_TEST == 1)
    {"cat",           0x01,   (func_ptr)&Demo_PROCFS_Cat},
    {"echo",          0x01,   (func_ptr)&Demo_PROCFS_Echo},
#endif
    {"demo_exit",           0x00,   (func_ptr)&Demo_Main_Exit},
};

static MS_U16 NumOfDemoFun = sizeof(_DemoFun) / sizeof(DemoFun);
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
//static MS_BOOL _appDemo_Reset(void);

MS_BOOL TestFun(MS_U32 *a, MS_U32 *b, MS_U32 *c, MS_U32 *d, MS_U32 *e)
{
    printf ("Test Fun Input 0 = %"DTC_MS_U32_x"\n", *a);
    printf ("Test Fun Input 1 = %"DTC_MS_U32_x"\n", *b);
    printf ("Test Fun Input 2 = %"DTC_MS_U32_x"\n", *c);
    printf ("Test Fun Input 3 = %"DTC_MS_U32_x"\n", *d);
    printf ("Test Fun Input 4 = %"DTC_MS_U32_x"\n", *e);

    printf ("Test end\n");
    return TRUE;
}

MS_BOOL TestStrFun(MS_U8 *a, MS_U8 *b, MS_U32 *c, MS_U32 *d, MS_U32 *e)
{
    printf ("TestStrFun Fun Input 0 = %s\n", a);
    printf ("TestStrFun Fun Input 1 = %s\n", b);
    printf ("TestStrFun Fun Input 2 = %"DTC_MS_U32_x"\n", *c);
    printf ("TestStrFun Fun Input 3 = %"DTC_MS_U32_x"\n", *d);
    printf ("TestStrFun Fun Input 4 = %"DTC_MS_U32_x"\n", *e);
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
#if (DEMO_PVR_TEST == 1)
    printf ("press \"PVR_HELP\" to show demo PVR cmds\n");
#endif /* DEMO_PVR_TEST */
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
#if (DEMO_PROCFS_TEST == 1)
    if((0 == strncasecmp((char*)sptr,"cat ",4)) || (0 == strncasecmp((char*)sptr,"echo ",5)))
    {
        sptr = GetNextWhiteChar(sptr);
        sptr = GetNextNonWhiteChar(sptr);
        argc ++;
        argv[argc] = sptr;
        return;
    }
#endif
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

void Demo_Main_Exit(MS_U32 argc, VOID *argv)
{
     /* Set Flag to exit demo task */
     _Demo_Exit = TRUE;
}

static void _Demo_Main_task(MS_U32 argc, VOID *argv)
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
                    if(Demo_Util_strcmp((char *)_DemoFun[u16I].u8CMDName, (char *)g_argv[0])==0)    // matching the function name
                    {

                        if (_DemoFun[u16I].u8InputMask & 0x1)
                        {
                            gInput0 = (g_argv[1]!=0) ? g_argv[1] : g_NullStr;
                        }
                        else
                        {
                            gu32Input0 = Demo_Util_DHConvert((char *)g_argv[1],1);
                            gInput0 = &gu32Input0;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x2)
                        {
                            gInput1 = (g_argv[2]!=0) ? g_argv[2] : g_NullStr;
                        }
                        else
                        {
                            gu32Input1 = Demo_Util_DHConvert((char *)g_argv[2],1);
                            gInput1 = &gu32Input1;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x4)
                        {
                            gInput2 = (g_argv[3]!=0) ? g_argv[3] : g_NullStr;
                        }
                        else
                        {
                            gu32Input2 = Demo_Util_DHConvert((char *)g_argv[3],1);
                            gInput2 = &gu32Input2;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x8)
                        {
                            gInput3 = (g_argv[4]!=0) ? g_argv[4] : g_NullStr;
                        }
                        else
                        {
                            gu32Input3 = Demo_Util_DHConvert((char *)g_argv[4],1);
                            gInput3 = &gu32Input3;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x10)
                        {
                            gInput4 = (g_argv[5]!=0) ? g_argv[5] : g_NullStr;
                        }
                        else
                        {
                            gu32Input4 = Demo_Util_DHConvert((char *)g_argv[5],1);
                            gInput4 = &gu32Input4;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x20)
                        {
                            gInput5 = (g_argv[6]!=0) ? g_argv[6] : g_NullStr;
                        }
                        else
                        {
                            gu32Input5 = Demo_Util_DHConvert((char *)g_argv[6],1);
                            gInput5 = &gu32Input5;
                        }

                        if (_DemoFun[u16I].u8InputMask & 0x40)
                        {
                            gInput6 = (g_argv[7]!=0) ? g_argv[7] : g_NullStr;
                        }
                        else
                        {
                            gu32Input6 = Demo_Util_DHConvert((char *)g_argv[7],1);
                            gInput6 = &gu32Input6;
                        }

                        gu32Input7 = Demo_Util_DHConvert((char *)g_argv[8],1);
                        gInput7 = &gu32Input7;

                        gu32Input8 = Demo_Util_DHConvert((char *)g_argv[9],1);
                        gInput8 = &gu32Input8;

                        gu32Input9 = Demo_Util_DHConvert((char *)g_argv[10],1);
                        gInput9 = &gu32Input9;

                        gu32Input10= Demo_Util_DHConvert((char *)g_argv[11],1);
                        gInput10 = &gu32Input10;

                        bCmdRet = _DemoFun[u16I].func_ptr(gInput0,gInput1,gInput2,gInput3,gInput4, gInput5, gInput6,gInput7,gInput8,gInput9,gInput10);

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

                    printf("Convert to decimal = %d\n",Demo_Util_DHConvert(g_argv[u8I], 1) );
                }
                */
                if (_Demo_Exit == TRUE)
                {
                    MsOS_SetEvent(_s32DemoEventId, DEMO_CMD_EXIT);
                    return ;
                }
                else
                {
                    MsOS_SetEvent(_s32DemoEventId, DEMO_CMD_END);
                }

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
    MS_U32 u32Input6, u32Input7, u32Input8, u32Input9, u32Input10;

    void *Input0, *Input1, *Input2, *Input3, *Input4, *Input5;
    void *Input6, *Input7, *Input8, *Input9, *Input10;

    MS_U8   *argv[MAX_CMD_PARAMETER+1];
    MS_U8 argc=0;
    MS_U8   *NullStr = NULL;
    MS_BOOL bCmdRet = FALSE;

    GetCmdArgv((MS_U8 *)cmd, &argc, argv);
    for (u16I = 0; u16I < NumOfDemoFun; u16I++)
    {
        // coverity[uninit_use_in_call]
        if(Demo_Util_strcmp((char *)_DemoFun[u16I].u8CMDName, (char *)argv[0])==0)    // matching the function name
        {

            if (_DemoFun[u16I].u8InputMask & 0x1)
            {
                Input0 = (argv[1]!=0) ? argv[1] : NullStr;
            }
            else
            {
                u32Input0 = Demo_Util_DHConvert((char *)argv[1],1);
                Input0 = &u32Input0;
            }

            if (_DemoFun[u16I].u8InputMask & 0x2)
            {
                Input1 = (argv[2]!=0) ? argv[2] : NullStr;
            }
            else
            {
                u32Input1 = Demo_Util_DHConvert((char *)argv[2],1);
                Input1 = &u32Input1;
            }

            if (_DemoFun[u16I].u8InputMask & 0x4)
            {
                Input2 = (argv[3]!=0) ? argv[3] : NullStr;
            }
            else
            {
                u32Input2 = Demo_Util_DHConvert((char *)argv[3],1);
                Input2 = &u32Input2;
            }


            if (_DemoFun[u16I].u8InputMask & 0x8)
            {
                Input3 = (argv[4]!=0) ? argv[4] : NullStr;
            }
            else
            {
                u32Input3 = Demo_Util_DHConvert((char *)argv[4],1);
                Input3 = &u32Input3;
            }

            if (_DemoFun[u16I].u8InputMask & 0x10)
            {
                Input4 = (argv[5]!=0) ? argv[5] : NullStr;
            }
            else
            {
                u32Input4 = Demo_Util_DHConvert((char *)argv[5],1);
                Input4 = &u32Input4;
            }

            if (_DemoFun[u16I].u8InputMask & 0x20)
            {
                Input5 = (argv[6]!=0) ? argv[6] : NullStr;
            }
            else
            {
                u32Input5 = Demo_Util_DHConvert((char *)argv[6],1);
                Input5 = &u32Input5;
            }

            u32Input6 = Demo_Util_DHConvert((char *)argv[7],1);
            Input6 = &u32Input6;

            u32Input7 = Demo_Util_DHConvert((char *)argv[8],1);
            Input7 = &u32Input7;

            u32Input8 = Demo_Util_DHConvert((char *)argv[9],1);
            Input8 = &u32Input8;

            u32Input9 = Demo_Util_DHConvert((char *)argv[10],1);
            Input9 = &u32Input9;

            u32Input10= Demo_Util_DHConvert((char *)argv[11],1);
            Input10 = &u32Input10;


            bCmdRet = _DemoFun[u16I].func_ptr(Input0,Input1,Input2,Input3,Input4, Input5, Input6,Input7,Input8,Input9,Input10);

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

static void _Demo_Main_Mon_task(MS_U32 argc, VOID *argv)
{
    MS_S32 CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&CACHE_POOL_ID);

    printf("\n");
    Prompt();

    {
        MS_U32 u32Event;
        while (1)
        {
            if (GetCmd() == TRUE)
            {
                MsOS_SetEvent(_s32DemoEventId, DEMO_CMD_READY);
                MsOS_WaitEvent(_s32DemoEventId,
                               DEMO_CMD_END|DEMO_CMD_EXIT,
                               &u32Event,
                               E_OR_CLEAR,
                               DEMO_EVENT_WAIT_FOREVER);
                if (u32Event == DEMO_CMD_EXIT)
                {
                    MsOS_SetEvent(_s32DemoEventId, DEMO_RELEASE);
                    return;
                }
                Prompt();
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// Demo Main Function - Demo_Main_Release_Check
/// @return TRUE - succeed
/// @return EXCEPTION - fail
/// @sa
/// @note to Release Demo Main Resource
/// Command: \b none \n
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_Main_Release_Check(void)
{
    MS_U32 u32Event = 0;
    MS_S32 CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&CACHE_POOL_ID);

    MsOS_WaitEvent(_s32DemoEventId,
               DEMO_RELEASE,
               &u32Event,
               E_AND_CLEAR,
               DEMO_EVENT_WAIT_FOREVER);

    if (u32Event == DEMO_RELEASE)
    {
        /*Free Demo Main Task Resource*/
        if (_Demo_Main_Task.pStack != NULL)
        {
            MsOS_FreeMemory(_Demo_Main_Task.pStack,CACHE_POOL_ID);
        }
        else
        {
            printf("[%s][%d]Free Memory Failed \n",__FUNCTION__,__LINE__);
        }

        if (_Demo_Main_Mon_Task.pStack != NULL)
        {
            MsOS_FreeMemory(_Demo_Main_Mon_Task.pStack,CACHE_POOL_ID);
        }
        else
        {
            printf("[%s][%d]Free Memory Failed \n",__FUNCTION__,__LINE__);
        }

        if (MsOS_DeleteEventGroup(_s32DemoEventId) == FALSE)
        {
            printf("[%s][%d]Delete Event Group Failed \n",__FUNCTION__,__LINE__);
        }

        return TRUE;
    }

    return FALSE;
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
MS_BOOL Demo_Main(void)
{
    MS_S32 CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&CACHE_POOL_ID);

    /// - Create Demo Main Task

    /// - Allocate Memory
    _Demo_Main_Task.pStack = MsOS_AllocateMemory(_Demo_Main_Task.u32StackSize, CACHE_POOL_ID);

    if(!_Demo_Main_Task.pStack)
    {
        printf("[%s][%d] Allocate Memory Failed \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    /// - Create Task Application
    _Demo_Main_Task.iId = MsOS_CreateTask( _Demo_Main_task,
                                            (MS_U32)NULL,
                                            _Demo_Main_Task.ePriority,
                                            TRUE,
                                            _Demo_Main_Task.pStack,
                                            _Demo_Main_Task.u32StackSize,
                                            _Demo_Main_Task.szName);
    if(_Demo_Main_Task.iId < 0)
    {
        if (_Demo_Main_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Task.pStack,CACHE_POOL_ID);
        }
        printf("[%s][%d] Create Task Failed \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    /// - Create OS Event Flag
    _s32DemoEventId = MsOS_CreateEventGroup("Demo AP");

    if (_s32DemoEventId < 0)
    {
        if (_Demo_Main_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Task.pStack,CACHE_POOL_ID);
        }
        printf("[%s][%d] Create EventGroup Failed \n",__FUNCTION__,__LINE__);
        return FALSE;

    }

    /// - Create Demo Monitor Task

    /// - Allocate Memory
    _Demo_Main_Mon_Task.pStack = MsOS_AllocateMemory(_Demo_Main_Mon_Task.u32StackSize, CACHE_POOL_ID);

    if(!_Demo_Main_Mon_Task.pStack)
    {
        if (_Demo_Main_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Task.pStack,CACHE_POOL_ID);
        }
        printf("[%s][%d] Allocate Monitor Memory Failed \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    /// - Create Task Application
    _Demo_Main_Mon_Task.iId = MsOS_CreateTask( _Demo_Main_Mon_task,
                                            (MS_U32)NULL,
                                            _Demo_Main_Mon_Task.ePriority,
                                            TRUE,
                                            _Demo_Main_Mon_Task.pStack,
                                            _Demo_Main_Mon_Task.u32StackSize,
                                            _Demo_Main_Mon_Task.szName);
    if(_Demo_Main_Mon_Task.iId < 0)
    {
        if (_Demo_Main_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Task.pStack,CACHE_POOL_ID);
        }

        if (_Demo_Main_Mon_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Mon_Task.pStack,CACHE_POOL_ID);
        }

        printf("[%s][%d] Create Monitor Task Failed \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    /// - Create OS Event Flag
    _s32DemoMonEventId = MsOS_CreateEventGroup("Demo Mointor");

    if (_s32DemoMonEventId < 0)
    {
        if (_Demo_Main_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Task.pStack,CACHE_POOL_ID);
        }

        if (_Demo_Main_Mon_Task.pStack)
        {
            MsOS_FreeMemory(_Demo_Main_Mon_Task.pStack,CACHE_POOL_ID);
        }

        printf("[%s][%d] Create Monitor EventGroup Failed \n",__FUNCTION__,__LINE__);
        return FALSE;

    }

    /// - Print UART message
    (printf("----------Hello World----------\n"));
    (printf("-------Welcome to Demo AP------\n"));
    (printf(">> "));

#if (DEMO_DMX_TEST == 1)

    #if (DEMO_BOOTVIDEO_TEST == 0)
        Demo_DMX_Init();
    #endif

#endif

#if (DEMO_PM_STR_AUTO_DC_OFF == 1)
    while(1)
    {
        Demo_PM_STR();
        MsOS_DelayTask(3000);
    }
#elif (DEMO_PM_STR_AUTO_IR_DC_OFF == 1)
    #if(DEMO_IR_ECOS_TEST == 1)
    {
        Demo_Input_Init_ecos();
        execute_cpu_load();
    }
     #elif(DEMO_IR_LINUX_TEST == 1)
    {
	 Demo_Input_Init_linux();
    }
    #endif
#endif

    return TRUE;
}
