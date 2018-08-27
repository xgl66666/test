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
/// @file   Demo_Audio.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for audio functions
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_AUDIO_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include "MsCommon.h"
#include "MsFS.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "drvGPIO.h"
#include "MsMemory.h"
#include "apiHDMITx.h"

#include "drvXC_IOPort.h"
#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#include "apiXC_PCMonitor_EX.h"
#endif

#include "../data/JAPAN_48_2_mp3.h"
#include "../data/WAV_44K.h"

#include "demo_xc.h"
#include "demo_utility.h"
#include "demo_audio.h"
#include "demo_av.h"

#include "SysBsp.h"

#include "drvDTC.h"

#if defined(MSOS_TYPE_ECOS)
#include "demo_ir_ecos.h"
#elif defined(MSOS_TYPE_LINUX)
#include "demo_ir_linux.h"
#include "drvMMIO.h"
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)

#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
//nclude <sys/types.h>
//nclude <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "exp_emz_common.h"
#include "exp_heaacenc_api.h"
#include "emheaac_enc_ioctl.h"
#endif

#endif

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
static Task_Info _audioDemo_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Demo Task"};
static Task_Info _audioMonitor_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Monitor"};
static ST_AUDIO_MONITOR_TEST_TASK _audioTestMonitor_Task = {.stAudioTask_Info.enTaskStatus=AUDIO_MONITOR_TASK_STOP, .stAudioTask_Info.stTask_Info.iId=-1, .stAudioTask_Info.stTask_Info.ePriority=E_TASK_PRI_HIGH, .stAudioTask_Info.stTask_Info.pStack=NULL, .stAudioTask_Info.stTask_Info.u32StackSize=4096, .stAudioTask_Info.stTask_Info.szName="Audio Test Mon"};

#if defined (DEMO_AUDIO_SUPPORT_MS12_B_TEST) && (DEMO_AUDIO_SUPPORT_MS12_B_TEST == 1)
static ST_DAP_List _st_dap_list[]=
{
    {"pregain",                     offsetof(struct dap_params_s,pregain), 1},
    {"postgain",                    offsetof(struct dap_params_s,postgain), 1},
    {"systemgain",                  offsetof(struct dap_params_s,systemgain), 1},
    {"surround_decoder_enable",     offsetof(struct dap_params_s,surround_decoder_enable), 1},
    {"virtualizer_enable",          offsetof(struct dap_params_s,virtualizer_enable), 1},
    {"headphone_reverb",            offsetof(struct dap_params_s,headphone_reverb), 1},
    {"speaker_angle",               offsetof(struct dap_params_s,speaker_angle), 1},
    {"speaker_start",               offsetof(struct dap_params_s,speaker_start), 1},
    {"surround_boost",              offsetof(struct dap_params_s,surround_boost), 1},
    {"mi_ieq_enable",               offsetof(struct dap_params_s,mi_ieq_enable), 1},
    {"mi_dv_enable",                offsetof(struct dap_params_s,mi_dv_enable), 1},
    {"mi_de_enable",                offsetof(struct dap_params_s,mi_de_enable), 1},
    {"mi_surround_enable",          offsetof(struct dap_params_s,mi_surround_enable), 1},
    {"calibration_boost",           offsetof(struct dap_params_s,calibration_boost), 1},
    {"leveler_amount",              offsetof(struct dap_params_s,leveler_amount), 1},
    {"leveler_input",               offsetof(struct dap_params_s,leveler_input), 1},
    {"leveler_output",              offsetof(struct dap_params_s,leveler_output), 1},
    {"leveler_enable",              offsetof(struct dap_params_s,leveler_enable), 1},
    {"modeler_enable",              offsetof(struct dap_params_s,modeler_enable), 1},
    {"modeler_calibration",         offsetof(struct dap_params_s,modeler_calibration), 1},
    {"ieq_enable",                  offsetof(struct dap_params_s,ieq_enable), 1},
    {"ieq_amount",                  offsetof(struct dap_params_s,ieq_amount), 1},
    {"ieq_nb_bands",                offsetof(struct dap_params_s,ieq_nb_bands), 1},
    {"a_ieq_band_center",           offsetof(struct dap_params_s,a_ieq_band_center), 20},
    {"a_ieq_band_target",           offsetof(struct dap_params_s,a_ieq_band_target), 20},
    {"de_enable",                   offsetof(struct dap_params_s,de_enable), 1},
    {"de_amount",                   offsetof(struct dap_params_s,de_amount), 1},
    {"de_ducking",                  offsetof(struct dap_params_s,de_ducking), 1},
    {"volmax_boost",                offsetof(struct dap_params_s,volmax_boost), 1},
    {"geq_enable",                  offsetof(struct dap_params_s,geq_enable), 1},
    {"geq_nb_bands",                offsetof(struct dap_params_s,geq_nb_bands), 1},
    {"a_geq_band_center",           offsetof(struct dap_params_s,a_geq_band_center), 20},
    {"a_geq_band_target",           offsetof(struct dap_params_s,a_geq_band_target), 20},
    {"optimizer_enable",            offsetof(struct dap_params_s,optimizer_enable), 1},
    {"ao_bands",                    offsetof(struct dap_params_s,optimizer_nb_bands), 1},
    {"ao_band_center_freq",         offsetof(struct dap_params_s,a_opt_band_center_freq), 20},
    {"ao_band_gains_0",             offsetof(struct dap_params_s,a_opt_band_gain[0]), 20},
    {"ao_band_gains_1",             offsetof(struct dap_params_s,a_opt_band_gain[1]), 20},
    {"ao_band_gains_2",             offsetof(struct dap_params_s,a_opt_band_gain[2]), 20},
    {"ao_band_gains_3",             offsetof(struct dap_params_s,a_opt_band_gain[3]), 20},
    {"ao_band_gains_4",             offsetof(struct dap_params_s,a_opt_band_gain[4]), 20},
    {"ao_band_gains_5",             offsetof(struct dap_params_s,a_opt_band_gain[5]), 20},
    {"bass_enable",                 offsetof(struct dap_params_s,bass_enable), 1},
    {"bass_boost",                  offsetof(struct dap_params_s,bass_boost), 1},
    {"bass_cutoff",                 offsetof(struct dap_params_s,bass_cutoff), 1},
    {"bass_width",                  offsetof(struct dap_params_s,bass_width), 1},
    {"ar_bands",                    offsetof(struct dap_params_s,reg_nb_bands), 1},
    {"ar_band_center_freq",         offsetof(struct dap_params_s,a_reg_band_center), 20},
    {"ar_low_thresholds",           offsetof(struct dap_params_s,a_reg_low_thresholds), 20},
    {"ar_high_thresholds",          offsetof(struct dap_params_s,a_reg_high_thresholds), 20},
    {"ar_isolated_bands",           offsetof(struct dap_params_s,a_reg_isolated_bands), 20},
    {"regulator_overdrive",         offsetof(struct dap_params_s,regulator_overdrive), 1},
    {"regulator_timbre",            offsetof(struct dap_params_s,regulator_timbre), 1},
    {"regulator_distortion",        offsetof(struct dap_params_s,regulator_distortion), 1},
    {"regulator_mode",              offsetof(struct dap_params_s,regulator_mode), 1},
    {"regulator_enable",            offsetof(struct dap_params_s,regulator_enable), 1},
    {"virtual_bass_mode",           offsetof(struct dap_params_s,virtual_bass_mode), 1},
    {"virtual_bass_low_src_freq",   offsetof(struct dap_params_s,virtual_bass_low_src_freq), 1},
    {"virtual_bass_high_src_freq",  offsetof(struct dap_params_s,virtual_bass_high_src_freq), 1},
    {"virtual_bass_overall_gain",   offsetof(struct dap_params_s,virtual_bass_overall_gain), 1},
    {"virtual_bass_slope_gain",     offsetof(struct dap_params_s,virtual_bass_slope_gain), 1},
    {"virtual_bass_subgain",        offsetof(struct dap_params_s,virtual_bass_subgain), 3},
    {"virtual_bass_mix_low_freq",   offsetof(struct dap_params_s,virtual_bass_mix_low_freq), 1},
    {"virtual_bass_mix_high_freq",  offsetof(struct dap_params_s,virtual_bass_mix_high_freq), 1},
    {"virtual_bass_enable",         offsetof(struct dap_params_s,virtual_bass_enable), 1}
};

static MS_U16 _NumOfDAPList = sizeof(_st_dap_list) / sizeof(ST_DAP_List);

#endif

typedef struct
{
    MS_U32 u32HeaderSize;
    MS_U32 u32RIFFChunkSize;
    MS_U32 u32fmtChunkSize;
    MS_U32 u32DataChunkSize;
    MS_U16 u16FormatTag;
    MS_U16 u16Channels;
    MS_U32 u32SamplesPerSec;
    MS_U32 u32AvgBytesPerSec;
    MS_U16 u16BlockSize;
    MS_U16 u16BitsPerSample;
    MS_U16 u16CExtensionSize;
} ST_WAVINFO;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define CHECK_PARSER_TIMEOUT     1000
#define E_DECODER_NUMBER (4)
#if ((DEMO_AUDIO_DMA_READER_TEST == 1) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
#define DMA_READER_NUMBER (5)
#endif
#if (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)
#define PCM_CAPTURE_NUMBER (4)
#endif
#define ENCODER_DUMP_LOG 0

#if defined(MSOS_TYPE_LINUX)
#define REG16_NPM(addr)            *((volatile MS_U16*)(virt32NonPmBase+ (addr)))
#define REG16_PM(addr )            *((volatile MS_U16*)(virt32PmBase+ (addr)))
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
#define NO_INPUT_SAMPLES 2048
#define NO_CHANNELS 2
#endif

#define TRANSCODE_NORMAL_FLOW
//#define TRANSCODE_OUTPUT_RAW_PCMBUF

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 *_apu8AudioData[E_DECODER_NUMBER];
static MS_S32 _au32AudioLen[E_DECODER_NUMBER];
static MS_U32 _au32clipWriteLastAddr[E_DECODER_NUMBER];
static MS_U32 _au32AudioFileIndex[E_DECODER_NUMBER];
static MS_BOOL _bNew_MM_Mode[E_DECODER_NUMBER];
static MS_U32 _u32StartTime[E_DECODER_NUMBER];
static ST_AUDIO_DEC_INFO stDualDemoInfo[E_DECODER_NUMBER]={{.eDecID=AU_DEC_INVALID},{.eDecID=AU_DEC_INVALID},{.eDecID=AU_DEC_INVALID},{.eDecID=AU_DEC_INVALID}};

#if (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)
static ST_AUDIO_PCM_IO_TASK _stAudioPCM_Capture_Task[PCM_CAPTURE_NUMBER]={{.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .enOutputPath=DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE, .enTaskStatus=AUDIO_MONITOR_TASK_STOP, .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="PCMCap Task 1"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .enOutputPath=DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE, .enTaskStatus=AUDIO_MONITOR_TASK_STOP,  .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="PCMCap Task 2"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .enOutputPath=DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE, .enTaskStatus=AUDIO_MONITOR_TASK_STOP,  .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="PCMCap Task 3"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .enOutputPath=DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE, .enTaskStatus=AUDIO_MONITOR_TASK_STOP,  .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="PCMCap Task 4"}};

#endif


#if ((DEMO_AUDIO_DMA_READER_TEST == 1) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
static ST_AUDIO_PCM_IO_TASK _stAudioDMAReader_Task[DMA_READER_NUMBER]={{.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="DMAReader Task 1"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="DMAReader Task 2"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="DMAReader Task 3"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="DMAReader Task 4"},
                                                                         {.s32DeviceId=-1, .u8Filename={0}, .bOutFileAppend=FALSE, .tPcmInfo={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="DMAReader Task 5"}};
#endif


#if (DEMO_AUDIO_ENCODER_TEST == 1)
static ST_AUDIO_ENC_DUMP_FILE_TASK _stAudioEncTask[E_DECODER_NUMBER]={{.s32DeviceId=-1, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="AudioEnc Task 1"},
                                                                        {.s32DeviceId=-1, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="AudioEnc Task 2"},
                                                                        {.s32DeviceId=-1, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="AudioEnc Task 3"},
                                                                        {.s32DeviceId=-1, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="AudioEnc Task 4"}};
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
static ST_AUDIO_TRANSCODE_TASK _stAudioTranscodeTask[E_DECODER_NUMBER]={{.s32DeviceId=-1, .enTranscodePath=DEMO_AUDIO_TRANSCODE_PATH_FILE, .enTranscodeFormat=DEMO_AUDIO_TRANSCODE_FORMAT_PCM, .enTaskStatus=AUDIO_MONITOR_TASK_STOP, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="Trans Task 1"},
                                                                        {.s32DeviceId=-1, .enTranscodePath=DEMO_AUDIO_TRANSCODE_PATH_FILE, .enTranscodeFormat=DEMO_AUDIO_TRANSCODE_FORMAT_PCM, .enTaskStatus=AUDIO_MONITOR_TASK_STOP, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="Trans Task 2"},
                                                                        {.s32DeviceId=-1, .enTranscodePath=DEMO_AUDIO_TRANSCODE_PATH_FILE, .enTranscodeFormat=DEMO_AUDIO_TRANSCODE_FORMAT_PCM, .enTaskStatus=AUDIO_MONITOR_TASK_STOP, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="Trans Task 3"},
                                                                        {.s32DeviceId=-1, .enTranscodePath=DEMO_AUDIO_TRANSCODE_PATH_FILE, .enTranscodeFormat=DEMO_AUDIO_TRANSCODE_FORMAT_PCM, .enTaskStatus=AUDIO_MONITOR_TASK_STOP, .u8Filename={0}, .stTask_Info.iId=-1, .stTask_Info.ePriority=E_TASK_PRI_HIGH, .stTask_Info.u32StackSize=4096, .stTask_Info.szName="Trans Task 4"}};

#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
#ifdef TRANSCODE_NORMAL_FLOW
static MS_S32 _s32fd = -1;
#endif
#endif
#endif

static OUTPUT_SOURCE_INFO _Output_SourceInfo = {  .SpeakerOut = 0,
                                                    .HpOut = 0,
                                                    .MonitorOut = 0,

.ScartOut = 0,
                                                    .SpdifOut = 0,
                                                    .ArcOut = 0,
                                                    .HDMIOut = 0};      //0 is main, 1 is sub
#if (DEMO_XC_HDMIRX_TEST == 1)
static MS_BOOL _bFirstIn = TRUE;
static MS_BOOL _bf_HdmiIsRaw = FALSE;
static AUDIO_HDMI_RX_TYPE _bfHDMI_RX_Mode = HDMI_RX_PCM;
static MS_U32 _bfHdmiAC3inSE = 0;
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
static AUDIO_FS_TYPE _stCurSampleType = AUDIO_FS_NONE;
static HDMI_TX_OUTPUT_TYPE _stCurHDMIType = HDMI_OUT_PCM;
static ST_AUDIO_EDID _st_Audio_EDID = {0};
#endif

static MS_BOOL _bExit_Monitor_Task = FALSE;
static MS_BOOL _bExit_Demo_Task = FALSE;

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
static MS_BOOL _bFMTX_enable = FALSE, _bBTSC_enable = FALSE, _bPCM_CAPTURE_enable = FALSE;
static MS_BOOL _bDDP_BYPASS_enable = FALSE, _bDSP2_Decode = FALSE;
#endif

static MS_BOOL _bDynamicLoad = FALSE;

// Audio test - check ok frame
static MS_U32 _u32AudioTest_ok_frame_timer=0;
static MS_U32 _u32AudioTest_ok_frame_count=0;

// Audio test - check error frame
static MS_U32 _u32AudioTest_err_frame_timer=0;
static MS_U32 _u32AudioTest_err_frame_count=0;

// Audio test - check skip frame
static MS_U32 _u32AudioTest_skip_frame_timer=0;
static MS_U32 _u32AudioTest_skip_frame_count=0;

// Audio test - check repeat frame
static MS_U32 _u32AudioTest_repeat_frame_timer=0;
static MS_U32 _u32AudioTest_repeat_frame_count=0;

// Audio test - SE buffer empty count
static MS_U32 _u32AudioTest_SE_Buf_empty_timer=0;
static MS_U32 _u32AudioTest_SE_Buf_empty_count=0;
static MS_U32 _u32AudioTest_SE_Buf_empty_continue_count=0;

#if defined (DM_ADDR_HDMI_IO_PORT_L) && defined(DM_ADDR_HDMI_IO_PORT_R) && defined(AUDIO_DSP_BANK) \
    && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) \
    && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)
// Audio test - HDMI IO port
static MS_U32 _u32AudioTest_HDMI_IO_Port_timer=0;
static MS_U32 _u32AudioTest_HDMI_IO_Port_L_level=0;
static MS_U32 _u32AudioTest_HDMI_IO_Port_R_level=0;
static MS_U32 _u32AudioTest_HDMI_IO_Port_count=0;
#endif

#if defined (DM_ADDR_SPDIF_IO_PORT_L) && defined(DM_ADDR_SPDIF_IO_PORT_R) && defined(AUDIO_DSP_BANK) \
    && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) \
    && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)
// Audio test - SPDIF IO port
static MS_U32 _u32AudioTest_SPDIF_IO_Port_timer=0;
static MS_U32 _u32AudioTest_SPDIF_IO_Port_L_level=0;
static MS_U32 _u32AudioTest_SPDIF_IO_Port_R_level=0;
static MS_U32 _u32AudioTest_SPDIF_IO_Port_count=0;
#endif

#if defined (DM_ADDR_LR_IO_PORT_L) && defined(DM_ADDR_LR_IO_PORT_R) && defined(AUDIO_DSP_BANK) \
    && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) \
    && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)
// Audio test - Lineout IO port
static MS_U32 _u32AudioTest_Lineout_IO_Port_timer=0;
static MS_U32 _u32AudioTest_Lineout_IO_Port_L_level=0;
static MS_U32 _u32AudioTest_Lineout_IO_Port_R_level=0;
static MS_U32 _u32AudioTest_Lineout_IO_Port_count=0;
#endif

// Audio test - HDMI nonpcm level
static MS_U32 _u32AudioTest_HDMI_NonPCM_Level_timer=0;
static MS_U32 _u32AudioTest_HDMI_NonPCM_Level_count=0;

// Audio test - SPDIF nonpcm level
static MS_U32 _u32AudioTest_SPDIF_NonPCM_Level_timer=0;
static MS_U32 _u32AudioTest_SPDIF_NonPCM_Level_count=0;

// Audio test - HDMI pcm mode
static MS_U32 _u32AudioTest_HDMI_PCM_Mode_timer=0;

// Audio test - SPDIF pcm mode
static MS_U32 _u32AudioTest_SPDIF_PCM_Mode_timer=0;

// Audio test - HDMI volume
static MS_U32 _u32AudioTest_HDMI_volume_timer=0;

// Audio test - SPDIF volume
static MS_U32 _u32AudioTest_SPDIF_volume_timer=0;

// Audio test - Lineout volume
static MS_U32 _u32AudioTest_Lineout_volume_timer=0;

// Audio test - Main PCM
static MS_U32 _u32AudioTest_Main_PCM_timer=0;
static MS_U32 _u32AudioTest_Main_PCM_level=0;

// Audio test - AD PCM
static MS_U32 _u32AudioTest_AD_PCM_timer=0;
static MS_U32 _u32AudioTest_AD_PCM_level=0;

// Audio test - PlayCmd Status
static MS_U32 _u32AudioTest_PlayCmd_Status_timer=0;

// Audio test - Codec type
static MS_U32 _u32AudioTest_Codec_Type_timer=0;

// Audio test - Audio sync
static MS_U32 _u32AudioTest_Audio_Sync_timer=0;

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
MS_U16 AudioVolumeTable[] =
{ //    1     2       3        4      5       6        7      8        9       10
                                                                                                                    0x7F00, //  00
    0x4700, 0x4400, 0x4100, 0x3E00, 0x3C00, 0x3A00, 0x3800, 0x3600, 0x3400, 0x3200, //  10
    0x3000, 0x2E00, 0x2D00, 0x2C00, 0x2B00, 0x2A00, 0x2900, 0x2800, 0x2700, 0x2600, //  20
    0x2500, 0x2400, 0x2300, 0x2200, 0x2100, 0x2000, 0x1F00, 0x1E80, 0x1E00, 0x1D80, //  30
    0x1D00, 0x1C80, 0x1C00, 0x1B80, 0x1B00, 0x1A80, 0x1A00, 0x1980, 0x1900, 0x1880, //  40
    0x1800, 0x1780, 0x1700, 0x1680, 0x1600, 0x1580, 0x1540, 0x1500, 0x14C0, 0x1480, //  50
    0x1440, 0x1400, 0x13C0, 0x1380, 0x1340, 0x1300, 0x12C0, 0x1280, 0x1240, 0x1200, //  60
    0x11C0, 0x1180, 0x1140, 0x1100, 0x10C0, 0x1080, 0x1040, 0x1000, 0x0FE0, 0x0FC0, //  70
    0x0FA0, 0x0F80, 0x0F60, 0x0F40, 0x0F20, 0x0F00, 0x0EE0, 0x0EC0, 0x0EA0, 0x0E80, //  80
    0x0E60, 0x0E40, 0x0E20, 0x0E00, 0x0DE0, 0x0DC0, 0x0DA0, 0x0D80, 0x0D60, 0x0D40, //  90
    0x0D20, 0x0D00, 0x0CE0, 0x0CC0, 0x0CA0, 0x0C80, 0x0C60, 0x0C40, 0x0C20, 0x0C00  //  100
};

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL _Demo_Audio_Debug_SetDM(MS_U32 u32RegValue)
{
#if defined(AUDIO_DSP_BANK) && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET)\
    && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)

    MS_U32 u32Bank = AUDIO_DSP_BANK;
    MS_U32 u32CmdAddr = AUDIO_DSP_DM_PM_OFFSET;
    MS_U32 u32Addr = AUDIO_DSP_CMD_TYPE_OFFSET;
    MS_U32 u32DMCmd = AUDIO_DSP_DM_ADDR;
    Demo_Audio_Debug_WriteReg(&u32Bank, &u32CmdAddr, &u32DMCmd);
    Demo_Audio_Debug_WriteReg(&u32Bank, &u32Addr, &u32RegValue);
    MsOS_DelayTask(10);
    return TRUE;
#else
    printf("Warning!!! DM Cmd define not set properly, check SysBsp.h\n");
    return FALSE;
#endif
}

//------------------------------------------------------------------------------
/// @brief The sample code to read register value
/// @param pu32Bank      \b : bank address
/// @param pu32Offset    \b : register offset (8-bit)
/// @param pu32RegValue  \b : value
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_U16 _Demo_Audio_Debug_ReadReg(MS_U32 u32Bank, MS_U32 u32Offset)
{
    MS_U16 u16RetValue = 0;

#if defined(MSOS_TYPE_ECOS)
    u16RetValue = *(volatile MS_U16*)(0xBF000000+((2*(u32Bank))<<8)+((u32Offset)<<1));

#elif defined(MSOS_TYPE_LINUX)
    MS_PHY phyBankSize;

    MS_VIRT virt32NonPmBase = 0;
    MS_VIRT virt32PmBase = 0;
    if( !MDrv_MMIO_GetBASE(&virt32NonPmBase, &phyBankSize, MS_MODULE_HW))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_HW)!\n");
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&virt32PmBase, &phyBankSize, MS_MODULE_PM))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_PM)!\n");
        return FALSE;
    }

    if(u32Bank >= 0x1000)
    {
        u16RetValue = REG16_NPM(((u32Bank-0x1000)<<9) + ((u32Offset)<<1));
    }
    else
    {
        u16RetValue = REG16_PM((u32Bank<<9) + ((u32Offset)<<1));
    }
#endif
    return u16RetValue;
}


MS_BOOL _Demo_Audio_Monitor_OK_Frame_Test(AUDIO_DEC_ID eDecID)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_ok_frame_timer) > 500)
    {
        MS_U32 u32CurrOKFrame_count = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_GetAudioInfo2(eDecID, Audio_infoType_okFrmCnt, &u32CurrOKFrame_count);
        }
        else
        {
            u32CurrOKFrame_count = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_okFrmCnt);
        }

        // if ok frame increase, return TRUE
        if (u32CurrOKFrame_count > _u32AudioTest_ok_frame_count)
        {
            // ok frame increased
            _u32AudioTest_ok_frame_count = u32CurrOKFrame_count;
            bRet = TRUE;
        }
        else
        {
            // no increase
            printf("[Audio Test] fail!!! (OK frame not increase)\n");
            bRet = FALSE;
        }

        _u32AudioTest_ok_frame_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Err_Frame_Test(AUDIO_DEC_ID eDecID)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_err_frame_timer) > 500)
    {
        MS_U32 u32CurrErrFrame_count = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_GetAudioInfo2(eDecID, Audio_infoType_errFrmCnt, &u32CurrErrFrame_count);
        }
        else
        {
            u32CurrErrFrame_count = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_errFrmCnt);
        }

        // if error frame not increase, return FALSE
        if (u32CurrErrFrame_count > _u32AudioTest_err_frame_count)
        {
            // error frame increased
            printf("[Audio Test] fail!!! (Error frame increased)\n");
            _u32AudioTest_err_frame_count = u32CurrErrFrame_count;
            bRet = FALSE;
        }
        else
        {
            // no increase
            bRet = TRUE;
        }

        _u32AudioTest_err_frame_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Skip_Frame_Test(AUDIO_DEC_ID eDecID)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_skip_frame_timer) > 500)
    {
        MS_U32 u32CurrSkipFrame_count = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            // no support enum
            //MApi_AUDIO_GetAudioInfo2(eDecID, Audio_infoType_okFrmCnt, &u32CurrSkipFrame_count);
            printf("[%s][%d] no support skip frame enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
            u32CurrSkipFrame_count = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_Skip_Frame_Count);
#else
            printf("[%s][%d] no support skip frame enum\n", __FUNCTION__, __LINE__);
#endif
        }

        // if skip frame increase, return FALSE
        if (u32CurrSkipFrame_count > _u32AudioTest_skip_frame_count)
        {
            // skip frame increased
            printf("[Audio Test] fail!!! (Skip frame increased)\n");
            _u32AudioTest_skip_frame_count = u32CurrSkipFrame_count;
            bRet = FALSE;
        }
        else
        {
            // no increase
            bRet = TRUE;
        }

        _u32AudioTest_skip_frame_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Repeat_Frame_Test(AUDIO_DEC_ID eDecID)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_repeat_frame_timer) > 500)
    {
        MS_U32 u32CurrRepeatFrame_count = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            // no support enum
            //MApi_AUDIO_GetAudioInfo2(eDecID, Audio_infoType_okFrmCnt, &u32CurrSkipFrame_count);
            printf("[%s][%d] no support repeat frame enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
            u32CurrRepeatFrame_count = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_Repeat_Frame_Count);
#else
            printf("[%s][%d] no support repeat frame enum\n", __FUNCTION__, __LINE__);
#endif
        }

        // if repeat frame increase, return FALSE
        if (u32CurrRepeatFrame_count > _u32AudioTest_repeat_frame_count)
        {
            // repeat frame increased
            printf("[Audio Test] fail!!! (Repeat frame increased)\n");
            _u32AudioTest_repeat_frame_count = u32CurrRepeatFrame_count;
            bRet = FALSE;
        }
        else
        {
            // no increase
            bRet = TRUE;
        }

        _u32AudioTest_repeat_frame_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_SE_Buf_Test(void)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_SE_Buf_empty_timer) > 50)
    {
        MS_U64 u64SEBuf_empty_count = 0;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
        u64SEBuf_empty_count = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_SE_Buffer_Empty_Count);
#else
        printf("[%s][%d] no support SE buf enum\n", __FUNCTION__, __LINE__);
#endif
        // if SE buffer count change, return fail
        if (u64SEBuf_empty_count != _u32AudioTest_SE_Buf_empty_count)
        {
            // change
            if (_u32AudioTest_SE_Buf_empty_continue_count < 10)
            {
                _u32AudioTest_SE_Buf_empty_continue_count++;
            }
            else
            {
                printf("[Audio Test] fail!!! (SE buf count change)\n");
                _u32AudioTest_SE_Buf_empty_continue_count = 0;
                bRet = FALSE;
            }
            _u32AudioTest_SE_Buf_empty_count = u64SEBuf_empty_count;
        }
        else
        {
            // no change
            bRet = TRUE;
            _u32AudioTest_SE_Buf_empty_continue_count = 0;
        }

        _u32AudioTest_SE_Buf_empty_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_HDMI_Out_Port_Test(void)
{
#if defined (DM_ADDR_HDMI_IO_PORT_L) && defined(DM_ADDR_HDMI_IO_PORT_R) && defined(AUDIO_DSP_BANK) \
    && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) \
    && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)

    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_HDMI_IO_Port_timer) > 50)
    {
        _Demo_Audio_Debug_SetDM(DM_ADDR_HDMI_IO_PORT_L);

        MS_U32 u32LeftChannel = (_Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK1_OFFSET)<<16) + _Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK2_OFFSET);

        _Demo_Audio_Debug_SetDM(DM_ADDR_HDMI_IO_PORT_R);

        MS_U32 u32RightChannel = (_Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK1_OFFSET)<<16) + _Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK2_OFFSET);

        // if LR level change
        if ((_u32AudioTest_HDMI_IO_Port_L_level == u32LeftChannel) && (_u32AudioTest_HDMI_IO_Port_R_level == u32RightChannel))
        {
            // L and R not change
            if (_u32AudioTest_HDMI_IO_Port_count < 10)
            {
                _u32AudioTest_HDMI_IO_Port_count++;
            }
            else
            {
                printf("[Audio Test] fail!!! (HDMI IO port not increase)\n");
                _u32AudioTest_HDMI_IO_Port_count = 0;
                bRet = FALSE;
            }
        }
        else
        {
            // L or R change
            _u32AudioTest_HDMI_IO_Port_L_level = u32LeftChannel;
            _u32AudioTest_HDMI_IO_Port_R_level = u32RightChannel;
            _u32AudioTest_HDMI_IO_Port_count = 0;
            bRet = TRUE;
        }

        _u32AudioTest_HDMI_IO_Port_timer = MsOS_GetSystemTime();
    }
    return bRet;
#else
    printf("Warning!!! DM Cmd define not set properly, check SysBsp.h\n");
    return FALSE;
#endif

}

MS_BOOL _Demo_Audio_Monitor_SPDIF_Out_Port_Test(void)
{
#if defined (DM_ADDR_SPDIF_IO_PORT_L) && defined(DM_ADDR_SPDIF_IO_PORT_R) && defined(AUDIO_DSP_BANK) \
    && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) \
    && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)

    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_SPDIF_IO_Port_timer) > 50)
    {
        _Demo_Audio_Debug_SetDM(DM_ADDR_SPDIF_IO_PORT_L);

        MS_U32 u32LeftChannel = (_Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK1_OFFSET)<<16) + _Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK2_OFFSET);

        _Demo_Audio_Debug_SetDM(DM_ADDR_SPDIF_IO_PORT_R);

        MS_U32 u32RightChannel = (_Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK1_OFFSET)<<16) + _Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK2_OFFSET);

        // if LR level change
        if ((_u32AudioTest_SPDIF_IO_Port_L_level == u32LeftChannel) && (_u32AudioTest_SPDIF_IO_Port_R_level == u32RightChannel))
        {
            // L and R not change
            if (_u32AudioTest_SPDIF_IO_Port_count < 10)
            {
                _u32AudioTest_SPDIF_IO_Port_count++;
            }
            else
            {
                printf("[Audio Test] fail!!! (SPDIF IO port not increase)\n");
                _u32AudioTest_SPDIF_IO_Port_count = 0;
                bRet = FALSE;
            }
        }
        else
        {
            // L or R increase
            _u32AudioTest_SPDIF_IO_Port_L_level = u32LeftChannel;
            _u32AudioTest_SPDIF_IO_Port_R_level = u32RightChannel;
            _u32AudioTest_SPDIF_IO_Port_count = 0;
            bRet = TRUE;
        }

        _u32AudioTest_SPDIF_IO_Port_timer = MsOS_GetSystemTime();
    }

    return bRet;
#else
    printf("Warning!!! DM Cmd define not set properly, check SysBsp.h\n");
    return FALSE;
#endif
}

MS_BOOL _Demo_Audio_Monitor_Lineout_Out_Port_Test(void)
{
#if defined (DM_ADDR_LR_IO_PORT_L) && defined(DM_ADDR_LR_IO_PORT_R) && defined(AUDIO_DSP_BANK) \
    && defined(AUDIO_DSP_DM_PM_OFFSET) && defined(AUDIO_DSP_CMD_TYPE_OFFSET) && defined(AUDIO_DSP_DM_PM_ACK1_OFFSET) \
    && defined(AUDIO_DSP_DM_PM_ACK2_OFFSET) && defined(AUDIO_DSP_DM_ADDR)

    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_Lineout_IO_Port_timer) > 50)
    {
        _Demo_Audio_Debug_SetDM(DM_ADDR_LR_IO_PORT_L);

        MS_U32 u32LeftChannel = (_Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK1_OFFSET)<<16) + _Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK2_OFFSET);

        _Demo_Audio_Debug_SetDM(DM_ADDR_LR_IO_PORT_R);

        MS_U32 u32RightChannel = (_Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK1_OFFSET)<<16) + _Demo_Audio_Debug_ReadReg(AUDIO_DSP_BANK, AUDIO_DSP_DM_PM_ACK2_OFFSET);

        // if LR level change
        if ((_u32AudioTest_Lineout_IO_Port_L_level == u32LeftChannel) && (_u32AudioTest_Lineout_IO_Port_R_level == u32RightChannel))
        {

            if (_u32AudioTest_Lineout_IO_Port_count < 10)
            {
                _u32AudioTest_Lineout_IO_Port_count++;
            }
            else
            {
                // L and R not increase
                printf("[Audio Test] fail!!! (Line out IO port not increase)\n");
                _u32AudioTest_Lineout_IO_Port_count = 0;
                bRet = FALSE;
            }
        }
        else
        {
            // L or R increase
            _u32AudioTest_Lineout_IO_Port_L_level = u32LeftChannel;
            _u32AudioTest_Lineout_IO_Port_R_level = u32RightChannel;
            _u32AudioTest_Lineout_IO_Port_count = 0;
            bRet = TRUE;
        }

        _u32AudioTest_Lineout_IO_Port_timer = MsOS_GetSystemTime();
    }

    return bRet;
#else
    printf("Warning!!! DM Cmd define not set properly, check SysBsp.h\n");
    return FALSE;
#endif
}

MS_BOOL _Demo_Audio_Monitor_HDMI_PCM_NonPCM_Mode_Test(MS_U32 u32PCMMode)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_HDMI_PCM_Mode_timer) > 500)
    {
        MS_U32 u32CurrHDMI_NonPCM_mode = 0;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
        // 0: pcm mode
        // 1: non pcm mode
        u32CurrHDMI_NonPCM_mode = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Hdmi_Pcm_Mode);
#else
        printf("[%s][%d] no support HDMI PCM/Non PCM Mode enum\n", __FUNCTION__, __LINE__);
#endif
        // if user define mode equal to current mode
        if (u32CurrHDMI_NonPCM_mode == u32PCMMode)
        {
            // match
            bRet = TRUE;
        }
        else
        {
            // non match
            printf("[Audio Test] fail!!! (HDMI PCM/Non PCM Mode not match(%"DTC_MS_U32_d", %"DTC_MS_U32_d"))\n", u32PCMMode, u32CurrHDMI_NonPCM_mode);
            bRet = FALSE;
        }

        _u32AudioTest_HDMI_PCM_Mode_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_SPDIF_PCM_NonPCM_Mode_Test(MS_U32 u32PCMMode)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_SPDIF_PCM_Mode_timer) > 500)
    {
        MS_U32 u32CurrSPDIF_NonPCM_mode = 0;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
        // 0: pcm mode
        // 2: non pcm mode
        u32CurrSPDIF_NonPCM_mode = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Spdif_Pcm_Mode);
#else
        printf("[%s][%d] no support SPDIF PCM/Non PCM Mode enum\n", __FUNCTION__, __LINE__);
#endif

        // if user define mode equal to current mode
        if (u32CurrSPDIF_NonPCM_mode == u32PCMMode)
        {
            // match
            bRet = TRUE;
        }
        else
        {
            // non match
            printf("[Audio Test] fail!!! (SPDIF PCM/Non PCM Mode not match(%"DTC_MS_U32_d", %"DTC_MS_U32_d"))\n", u32PCMMode, u32CurrSPDIF_NonPCM_mode);
            bRet = FALSE;
        }

        _u32AudioTest_SPDIF_PCM_Mode_timer = MsOS_GetSystemTime();
    }

    return bRet;
}


MS_BOOL _Demo_Audio_Monitor_HDMI_NonPCM_Level_Test(void)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_HDMI_NonPCM_Level_timer) > 500)
    {
        MS_U32 u32CurrHDMI_NonPCM_Level = 0;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
        u32CurrHDMI_NonPCM_Level = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Hdmi_Nonpcm_Level);
#else
        printf("[%s][%d] no support HDMI PCM/Non PCM level enum\n", __FUNCTION__, __LINE__);
#endif
        // if non pcm level change, return true, otherwise return false
        if (u32CurrHDMI_NonPCM_Level != _u32AudioTest_HDMI_NonPCM_Level_count)
        {
            // non pcm level change
            _u32AudioTest_HDMI_NonPCM_Level_count = u32CurrHDMI_NonPCM_Level;
            bRet = TRUE;
        }
        else
        {
            // no change
            printf("[Audio Test] fail!!! (HDMI non pcm level not increase)\n");
            bRet = FALSE;
        }

        _u32AudioTest_HDMI_NonPCM_Level_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_SPDIF_NonPCM_Level_Test(void)
{

    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_SPDIF_NonPCM_Level_timer) > 500)
    {
        MS_U32 u32CurrSPDIF_NonPCM_Level = 0;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
        u32CurrSPDIF_NonPCM_Level = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Spdif_Nonpcm_Level);
#else
        printf("[%s][%d] no support SPDIF PCM/Non PCM level enum\n", __FUNCTION__, __LINE__);
#endif

        // if non pcm level change, return fail
        if (u32CurrSPDIF_NonPCM_Level != _u32AudioTest_SPDIF_NonPCM_Level_count)
        {
            // non pcm level change
            _u32AudioTest_SPDIF_NonPCM_Level_count = u32CurrSPDIF_NonPCM_Level;
            bRet = TRUE;
        }
        else
        {
            // no change
            printf("[Audio Test] fail!!! (SPDIF non pcm level not increase)\n");
            bRet = FALSE;
        }

        _u32AudioTest_SPDIF_NonPCM_Level_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_HDMI_Mute_Status_Test(void)
{
    printf("[%s][%d] no support HDMI mute status enum\n", __FUNCTION__, __LINE__);
    return TRUE;
}

MS_BOOL _Demo_Audio_Monitor_SPDIF_Mute_Status_Test(void)
{
    printf("[%s][%d] no support SPDIF mute status enum\n", __FUNCTION__, __LINE__);
    return TRUE;
}

MS_BOOL _Demo_Audio_Monitor_HDMI_Volume_Level_Test(MS_U32 u32Volume)
{
    MS_BOOL bRet = TRUE;

    if (u32Volume > 100)
    {
        printf("[Audio Test] fail!!! (User define HDMI volume too large (%"DTC_MS_U32_d"))\n", u32Volume);
        return FALSE;
    }
    MS_U32 u32TargetVolume = 0;
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
    u32TargetVolume = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Hdmi_Volume);
#else
        printf("[%s][%d] no support HDMI volume enum\n", __FUNCTION__, __LINE__);
#endif

    MS_U32 u32UserVolume = AudioVolumeTable[u32Volume] + AUDIO_VOL_HDMI_COMPENSATION;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_HDMI_volume_timer) > 500)
    {
        // if volume level match the test value
        if (u32UserVolume == u32TargetVolume)
        {
            // match
            bRet = TRUE;
        }
        else
        {
            // not match
            printf("[Audio Test] fail!!! (HDMI volume not match(0x%"DTC_MS_U32_x", 0x%"DTC_MS_U32_x"))\n", u32UserVolume, u32TargetVolume);
            bRet = FALSE;
        }

        _u32AudioTest_HDMI_volume_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_SPDIF_Volume_Level_Test(MS_U32 u32Volume)
{
    MS_BOOL bRet = TRUE;

    if (u32Volume > 100)
    {
        printf("[Audio Test] fail!!! (User define SPDIF volume too large (%"DTC_MS_U32_d"))\n", u32Volume);
        return FALSE;
    }
    MS_U32 u32TargetVolume = 0;
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
    u32TargetVolume = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Spdif_Volume);
#else
        printf("[%s][%d] no support SPDIF volume enum\n", __FUNCTION__, __LINE__);
#endif

    MS_U32 u32UserVolume = AudioVolumeTable[u32Volume] + AUDIO_VOL_SPDIF_COMPENSATION;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_SPDIF_volume_timer) > 500)
    {
        // if volume level match the test value
        if (u32UserVolume == u32TargetVolume)
        {
            // match
            bRet = TRUE;
        }
        else
        {
            // not match
            printf("[Audio Test] fail!!! (SPDIF volume not match(0x%"DTC_MS_U32_x", 0x%"DTC_MS_U32_x"))\n", u32UserVolume, u32TargetVolume);
            bRet = FALSE;
        }

        _u32AudioTest_SPDIF_volume_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Lineout_Volume_Level_Test(MS_U32 u32Volume)
{
    MS_BOOL bRet = TRUE;

    if (u32Volume > 100)
    {
        printf("[Audio Test] fail!!! (User define LR volume too large (%"DTC_MS_U32_d"))\n", u32Volume);
        return FALSE;
    }

    MS_U32 u32TargetVolume = 0;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
    u32TargetVolume = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Cvbs_Volume);
#else
        printf("[%s][%d] no support SPDIF volume enum\n", __FUNCTION__, __LINE__);
#endif

    MS_U32 u32UserVolume = AudioVolumeTable[u32Volume] + AUDIO_VOL_LR_COMPENSATION;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_Lineout_volume_timer) > 500)
    {
        // if volume level match the test value
        if (u32UserVolume == u32TargetVolume)
        {
            // match
            bRet = TRUE;
        }
        else
        {
            // not match
            printf("[Audio Test] fail!!! (LR volume not match(0x%"DTC_MS_U32_x", 0x%"DTC_MS_U32_x"))\n", u32UserVolume, u32TargetVolume);
            bRet = FALSE;
        }

        _u32AudioTest_Lineout_volume_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Main_PCM_Level_Test(AUDIO_DEC_ID eDecID)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_Main_PCM_timer) > 500)
    {
        MS_U32 u32CurrMain_PCM_Level = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //no enum for multi decode
            printf("[%s][%d] no support AD PCM level enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
            u32CurrMain_PCM_Level = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Main_Pcm_Level);
#else
            printf("[%s][%d] no support AD PCM level enum\n", __FUNCTION__, __LINE__);
#endif
        }

        // if level change, return true
        if (u32CurrMain_PCM_Level != _u32AudioTest_Main_PCM_level)
        {
            // change
            bRet = TRUE;
        }
        else
        {
            // no change
            printf("[Audio Test] fail!!! (main PCM level not increase)\n");
            bRet = FALSE;
            _u32AudioTest_Main_PCM_level = u32CurrMain_PCM_Level;
        }

        _u32AudioTest_Main_PCM_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_AD_PCM_Level_Test(AUDIO_DEC_ID eDecID)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_AD_PCM_timer) > 500)
    {
        MS_U32 u32CurrAD_PCM_Level = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //no enum for multi decode
            printf("[%s][%d] no support AD PCM level enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
            //not support
            printf("[%s][%d] no support AD PCM level enum\n", __FUNCTION__, __LINE__);

            //u32CurrAD_PCM_Level = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Main_Pcm_Level);
        }

        // if level change, return true
        if (u32CurrAD_PCM_Level != _u32AudioTest_AD_PCM_level)
        {
            // change
            bRet = TRUE;
        }
        else
        {
            // no change
            printf("[Audio Test] fail!!! (AD PCM level not increase)\n");
            bRet = FALSE;
            _u32AudioTest_AD_PCM_level = u32CurrAD_PCM_Level;
        }

        _u32AudioTest_AD_PCM_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_PlayCmd_Status_Test(AUDIO_DEC_ID eDecID, MS_U32 u32PlayCmd)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_PlayCmd_Status_timer) > 500)
    {
        MS_U32 u64PlayCmd = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //no enum for multi decode
            printf("[%s][%d] no support play command status enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
            u64PlayCmd = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_Play_Cmd);
#else
            printf("[%s][%d] no support play command status enum\n", __FUNCTION__, __LINE__);
#endif
        }

        // if command the same, return true
        if (u64PlayCmd == u32PlayCmd)
        {
            // the same
            bRet = TRUE;
        }
        else
        {
            // not the same
            printf("[Audio Test] fail!!! (Play command(0x%"DTC_MS_U32_x") not match(0x%"DTC_MS_U32_x"))\n", u32PlayCmd, u64PlayCmd);
            bRet = FALSE;
        }

        _u32AudioTest_PlayCmd_Status_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Codec_Type_Test(AUDIO_DEC_ID eDecID, MS_U32 u32CodecType)
{
    MS_BOOL bRet = TRUE;

    // check every 500 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_Codec_Type_timer) > 500)
    {
        MS_U32 u64CodecType = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //no enum for multi decode
            printf("[%s][%d] no support codec type enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
            u64CodecType = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_Codec_Type);
#else
            printf("[%s][%d] no support codec type enum\n", __FUNCTION__, __LINE__);
#endif
        }

        // if command the same, return true
        if (u64CodecType == u32CodecType)
        {
            // the same
            bRet = TRUE;
        }
        else
        {
            // not the same
            printf("[Audio Test] fail!!! (Codec type(0x%"DTC_MS_U32_x") not match(0x%"DTC_MS_U32_x"))\n", u32CodecType, u64CodecType);
            bRet = FALSE;
        }

        _u32AudioTest_Codec_Type_timer = MsOS_GetSystemTime();
    }

    return bRet;
}

MS_BOOL _Demo_Audio_Monitor_Audio_Sync_Status_Test(void)
{

    MS_BOOL bRet = TRUE;

    // check every 10 ms
    if ((MsOS_GetSystemTime() - _u32AudioTest_Audio_Sync_timer) > 500)
    {
        MS_U64 u64SyncStatus = 0;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            printf("[%s][%d] no support audio sync enum\n", __FUNCTION__, __LINE__);
        }
        else
        {
            u64SyncStatus = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Is_Audio_Sync);
        }

        // if command the same, return true
        if (u64SyncStatus)
        {
            // synced
            bRet = TRUE;
        }
        else
        {
            // not synced
            printf("[Audio Test] fail!!! (Audio not sync)\n");
            bRet = FALSE;
        }

        _u32AudioTest_Audio_Sync_timer = MsOS_GetSystemTime();
    }

    return bRet;
}


static void _appAudioTestMonitor_task(MS_VIRT pdata)
{

    ST_AUDIO_MONITOR_TEST_TASK* pstAudiotestTask = (ST_AUDIO_MONITOR_TEST_TASK*)pdata;

    while (pstAudiotestTask->stAudioTask_Info.enTaskStatus != AUDIO_MONITOR_TASK_STOP)
    {
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_OK_FRAME_COUNT)
        {
            if (_Demo_Audio_Monitor_OK_Frame_Test(pstAudiotestTask->eDecID) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_ERR_FRAME_COUNT)
        {
            if (_Demo_Audio_Monitor_Err_Frame_Test(pstAudiotestTask->eDecID) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SKIP_FRAME_COUNT)
        {
            if (_Demo_Audio_Monitor_Skip_Frame_Test(pstAudiotestTask->eDecID) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_REPEAT_FRAME_COUNT)
        {
            if (_Demo_Audio_Monitor_Repeat_Frame_Test(pstAudiotestTask->eDecID) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SE_BUF_EMPTY_COUNT)
        {
            if (_Demo_Audio_Monitor_SE_Buf_Test() == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_HDMI_OUT_PORT)
        {
            _Demo_Audio_Monitor_HDMI_Out_Port_Test();
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SPDIF_OUT_PORT)
        {
            _Demo_Audio_Monitor_SPDIF_Out_Port_Test();
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_LINEOUT_OUT_PORT)
        {
            _Demo_Audio_Monitor_Lineout_Out_Port_Test();
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_HDMI_PCM_NONPCM_MODE)
        {
            if (_Demo_Audio_Monitor_HDMI_PCM_NonPCM_Mode_Test(pstAudiotestTask->u32HDMIPCMMode) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SPDIF_PCM_NONPCM_MODE)
        {
            if (_Demo_Audio_Monitor_SPDIF_PCM_NonPCM_Mode_Test(pstAudiotestTask->u32SPDIFPCMMode) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_HDMI_NONPCM_LEVEL)
        {
            if (_Demo_Audio_Monitor_HDMI_NonPCM_Level_Test() == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SPDIF_NONPCM_LEVEL)
        {
            if (_Demo_Audio_Monitor_SPDIF_NonPCM_Level_Test() == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_HDMI_MUTE_STATUS)
        {
            if (_Demo_Audio_Monitor_HDMI_Mute_Status_Test() == FALSE)
            {
                printf("[Audio Test] fail!!! (HDMI not mute)\n");
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SPDIF_MUTE_STATUS)
        {
            if (_Demo_Audio_Monitor_SPDIF_Mute_Status_Test() == FALSE)
            {
                printf("[Audio Test] fail!!! (SPDIF not mute)\n");
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_HDMI_VOLUME)
        {
            if (_Demo_Audio_Monitor_HDMI_Volume_Level_Test(pstAudiotestTask->u32Volume) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_SPDIF_VOLUME)
        {
            if (_Demo_Audio_Monitor_SPDIF_Volume_Level_Test(pstAudiotestTask->u32Volume) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_LINEOUT_VOLUME)
        {
            if (_Demo_Audio_Monitor_Lineout_Volume_Level_Test(pstAudiotestTask->u32Volume) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_MAIN_PCM_LEVEL)
        {
            if (_Demo_Audio_Monitor_Main_PCM_Level_Test(pstAudiotestTask->eDecID) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_AD_PCM_LEVEL)
        {
            if (_Demo_Audio_Monitor_AD_PCM_Level_Test(pstAudiotestTask->eDecID) == FALSE)
            {

            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_PLAYCMD_STATUS)
        {
            if (_Demo_Audio_Monitor_PlayCmd_Status_Test(pstAudiotestTask->eDecID, pstAudiotestTask->u32PlayCmd) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_CODEC_TYPE)
        {
            if (_Demo_Audio_Monitor_Codec_Type_Test(pstAudiotestTask->eDecID, pstAudiotestTask->u32CodecType) == FALSE)
            {
                //fail
            }
        }
        if (pstAudiotestTask->u32MonitorType1 & AUDIO_MONITOR_AUDIO_SYNC_STATUS)
        {
            if (_Demo_Audio_Monitor_Audio_Sync_Status_Test() == FALSE)
            {
                //fail
            }
        }

        MsOS_DelayTask(1);
    }

    pstAudiotestTask->stAudioTask_Info.stTask_Info.iId = -1;

}


//------------------------------------------------------------------------------
/// @brief The sample code is a callback function to customize IR key handler.
/// @param[in] u32key IR key
/// @return TRUE: Load success.
/// @return FALSE: Load fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL _Demo_Audio_IR_Key_Handler(MS_U32 u32key)
{
#if defined(AUDIO_R2_BANK) && defined(AUDIO_R2_CMD_OFFSET) && defined(AUDIO_R2_CMD_OFFSET) \
    && defined(UART_BANK) && defined(UART_SOURCE_OFFSET) \
    && defined(UART_SOURCE_SETTING_MCU_TO_AUDIO_R2) && defined(UART_SOURCE_SETTING_AUDIO_R2_TO_MCU)

    static MS_U32 _u32IR_R2_cmd = 0;
    MS_U32 u32R2bank = AUDIO_R2_BANK;
    MS_U32 u32R2offset = AUDIO_R2_CMD_OFFSET;
    MS_U32 u32DisableWFIoffset = AUDIO_R2_SYSTEM_DBG_OFFSET;
    MS_U32 u32UARTbank = UART_BANK;
    MS_U32 u32UARToffset = UART_SOURCE_OFFSET;
    MS_U32 u32DsiableWFIvalue = 0x800;
    MS_U32 u32UART_MCU_to_R2 = UART_SOURCE_SETTING_MCU_TO_AUDIO_R2;
    MS_U32 u32UART_R2_to_MCU = UART_SOURCE_SETTING_AUDIO_R2_TO_MCU;
    switch (u32key)
    {
        case 0x50:
            //vk_0
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 0;
            break;
        case 0x49:
            //vk_1
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 1;
            break;
        case 0x55:
            //vk_2
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 2;
            break;
        case 0x59:
            //vk_3
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 3;
            break;
        case 0x4d:
            //vk_4
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 4;
            break;
        case 0x51:
            //vk_5
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 5;
            break;
        case 0x5d:
            //vk_6
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 6;
            break;
        case 0x48:
            //vk_7
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 7;
            break;
        case 0x54:
            //vk_8
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 8;
            break;
        case 0x58:
            //vk_9
            _u32IR_R2_cmd = _u32IR_R2_cmd*10 + 9;
            break;
        case 0xf:
            //ok

            //issue R2 command
            Demo_Audio_Debug_WriteReg(&u32R2bank, &u32R2offset, &_u32IR_R2_cmd);
            Demo_Audio_Debug_WriteReg(&u32R2bank, &u32DisableWFIoffset, &u32DsiableWFIvalue);

            //switch UART source to R2
            Demo_Audio_Debug_WriteReg(&u32UARTbank, &u32UARToffset, &u32UART_MCU_to_R2);
            break;
        case 0x1b:
            //VK_Exit
        case 0x5c:
            //VK_CHRTN
            _u32IR_R2_cmd = 0;
            //switch UART source to MCU
            Demo_Audio_Debug_WriteReg(&u32UARTbank, &u32UARToffset, &u32UART_R2_to_MCU);
            break;
    }

    if (_u32IR_R2_cmd >= 1000)
    {
        _u32IR_R2_cmd = _u32IR_R2_cmd%1000;
    }

    printf("======== Audio R2 cmd = %3"DTC_MS_U32_d"(0x%"DTC_MS_U32_x") ========\n", _u32IR_R2_cmd, _u32IR_R2_cmd);

#else
    printf("Warning!!! R2 debug register not set properly, check SysBsp.h\n");
#endif

    return TRUE;
}

#if defined (DEMO_AUDIO_SUPPORT_MS12_B_TEST) && (DEMO_AUDIO_SUPPORT_MS12_B_TEST == 1)

//------------------------------------------------------------------------------
/// @brief The sample code to load DAP data from a script file to a structure
/// @param[in] filename DAP param script file
/// @param[in] pst_dap_params pointer to a dap param structure
/// @return TRUE: Load success.
/// @return FALSE: Load fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_Audio_Load_DAP_Param_FromFile(char* filename, dap_params_t* pst_dap_params )
{
    if (filename == NULL)
    {
        return FALSE;
    }

    MS_BOOL bAllMatch = TRUE;

    FILE *file = NULL;

    //open file
    file = MsFS_Fopen((char*)filename, "r");
    if (file == NULL)
    {
        printf("open file fail!!!\n");
        return FALSE;
    }

    //load value from file
    while(!feof(file))
    {
        char attribute[30] = {0};
        MS_S32 temp[20]={0};
        MS_U32 u32match_count = 0;
        MS_U32 u32dap_list_index = 0;

        u32match_count = fscanf(file, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                                attribute, &temp[0], &temp[1], &temp[2], &temp[3], &temp[4], &temp[5], &temp[6], &temp[7], &temp[8], &temp[9],
                                           &temp[10], &temp[11], &temp[12], &temp[13], &temp[14], &temp[15], &temp[16], &temp[17], &temp[18], &temp[19]);

        //check at least match 2 values (attribute + value)
        if(u32match_count == 0)
        {
            printf("[%s][%d] %s - wrong format, no match format\n", __FUNCTION__, __LINE__, attribute);

            //some format is wrong
            bAllMatch = FALSE;

            continue;
        }
        else if(u32match_count == 1)
        {
            printf("[%s][%d] %s - wrong format, too less match format (less than 2)\n", __FUNCTION__, __LINE__, attribute);

            //some format is wrong
            bAllMatch = FALSE;

            continue;
        }

        //search match attribute
        for (u32dap_list_index=0; u32dap_list_index<_NumOfDAPList; u32dap_list_index++)
        {
            //check if found the field name in structure ST_DAP_List
            if(strcmp((char *)attribute, (char *)&_st_dap_list[u32dap_list_index].field_name) == 0)
            {
                //check if the input value count is equal to the required count
                if( _st_dap_list[u32dap_list_index].u32field_value_Count == (u32match_count-1))
                {
                    //load into dap_params_t structure
                    memcpy(((MS_U8*)pst_dap_params+_st_dap_list[u32dap_list_index].u32field_offset), temp, _st_dap_list[u32dap_list_index].u32field_value_Count*sizeof(MS_S32));
                    continue;
                }
                else
                {
                    printf("[%s][%d] %s - wrong format, need %d values, but only %d values\n", __FUNCTION__, __LINE__, _st_dap_list[u32dap_list_index].field_name, _st_dap_list[u32dap_list_index].u32field_value_Count, (u32match_count-1));

                    //some format is wrong
                    bAllMatch = FALSE;
                }
            }
        }
    }

    //close file
    MsFS_Fclose(file);

    if (bAllMatch)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code to feed data to audio codec
/// @param[in] writeAddress The address that audio data should put in
/// @param[in] writeNumber The length that audio data should put in
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static MS_BOOL _Demo_Audio_CheckAudioCapability(MS_U32 u32DevIdx, En_DVB_decSystemType eDecSysType)
{
    MMA_AudioType AudioType = Audio_DEC_NULL;
    MMA_AudioType AudioCapability = Audio_DEC_NULL;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if (MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[u32DevIdx].eDecID, Audio_infoType_capability, &AudioCapability) == FALSE)
        {
            printf("[%s][%d] get audio capability fail!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }
    else
    {
        AudioCapability = (MMA_AudioType)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_capability);
    }

    switch(eDecSysType)
    {
        case MSAPI_AUD_DVB_AAC:
            AudioType = (Audio_DEC_AAC_LC | Audio_DEC_HEAAC_V1 | Audio_DEC_HEAAC_V2);   // GAAC
            AudioType |= Audio_DEC_DOLBYPULSE;  // MS10
            break;
        case MSAPI_AUD_DVB_AC3:
            AudioType = Audio_DEC_AC3;
            break;
        case MSAPI_AUD_DVB_AC3P:
            AudioType = Audio_DEC_AC3P;
            break;
        case MSAPI_AUD_DVB_AMR_NB:
            AudioType = Audio_DEC_AMR_NB;
            break;
        case MSAPI_AUD_DVB_AMR_WB:
            AudioType = Audio_DEC_AMR_WB;
            break;
        case MSAPI_AUD_DVB_DTS:
            AudioType = Audio_DEC_DTS;
            break;
        case MSAPI_AUD_DVB_DTSLBR:
            AudioType = Audio_DEC_DTSLBR;
            break;
        case MSAPI_AUD_DVB_FLAC:
            AudioType = Audio_DEC_FLAC;
            break;
        case MSAPI_AUD_DVB_MP3:
            AudioType = Audio_DEC_MP3;
            break;
        case MSAPI_AUD_DVB_MPEG:
            AudioType = Audio_DEC_MPEG;
            break;
        case MSAPI_AUD_DVB_RA8LBR:
            AudioType = Audio_DEC_RA8;
            break;
        case MSAPI_AUD_DVB_VORBIS:
            AudioType = Audio_DEC_VORBIS;
            break;
        case MSAPI_AUD_DVB_WMA:
            AudioType = Audio_DEC_WMA;
            break;
        case MSAPI_AUD_DVB_WMA_PRO:
            AudioType = Audio_DEC_WMAPRO;
            break;
        case MSAPI_AUD_DVB_XPCM: //AUDIO_CODEC_WAV
            AudioType = Audio_DEC_XPCM | Audio_DEC_PCM;
            break;
        case MSAPI_AUD_DVB_DRA:
            AudioType = Audio_DEC_DRA;
            break;
#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
        case MSAPI_AUD_DVB_AC4:
            AudioType = Audio_DEC_AC4;
            break;
#endif
        default :
            AudioType = Audio_DEC_NULL;
            break;
    }

    if ( (AudioType != Audio_DEC_NULL) && ((AudioCapability & AudioType) != 0))
    {
        return TRUE;
    }
    else if ( eDecSysType == MSAPI_AUD_DVB_NONE )
    {
        //No capability of codec_none, but has codec_none indentity, return TRUE for load codec
        return TRUE;
    }
    else if ( ( eDecSysType == MSAPI_AUD_ATVENC_BTSC ) || ( eDecSysType == MSAPI_AUD_ATVENC_FMTX ) )
    {
        //No capability for BTSC and FMTX, return TRUE for load codec
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to feed data to audio codec
/// @param[in] writeAddress The address that audio data should put in
/// @param[in] writeNumber The length that audio data should put in
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_Audio_File_play(MS_U32 u32DevIdx, MS_U32 writeAddress, MS_U32 writeNumber, MS_BOOL bNewMMmode)
{
    //printf("\nDemo_Audio_File_play(0x%"DTC_MS_U32_x",0x%"DTC_MS_U32_x")...\n",writeAddress,writeNumber);
    if(bNewMMmode)
    {
        //new MM mode
        MS_U32 u32RemainedSize = _au32AudioLen[u32DevIdx] - _au32AudioFileIndex[u32DevIdx];
        AUDIO_DEC_ID DecID = AU_DEC_INVALID;
        MS_U32 u32es_size = 0;
        MS_U64 u64PTS = 0;

        //TRUE: Set PTS; FALSE: Not set PTS (New MM mode support this feature)
        MS_BOOL bSetPTS = TRUE;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //Audio V2 flow
            DecID = stDualDemoInfo[u32DevIdx].eDecID;
        }
        else
        {
            //Audio V1 flow
            DecID = AU_DEC_ID1;
        }

        if (_au32AudioFileIndex[u32DevIdx] ==  0 )
        {
            if (_u32StartTime[u32DevIdx] == 0)
            {
                _u32StartTime[u32DevIdx] = MsOS_GetSystemTime();
            }
        }

        if (u32RemainedSize > 0)
        {
            if(writeNumber>0)
            {
                u32es_size = MIN(writeNumber, u32RemainedSize);

                memcpy((void*)MS_PA2KSEG1(MApi_AUDIO_GetDDRInfo(DecID, E_AUD_MEMORY_BASE)+writeAddress), (_apu8AudioData[u32DevIdx]+_au32AudioFileIndex[u32DevIdx]), u32es_size);

                _au32AudioFileIndex[u32DevIdx] += u32es_size;

                //example of setting system time differemce (ticks) as PTS
                u64PTS = (MS_U64)(MsOS_GetSystemTime() - _u32StartTime[u32DevIdx] ) * 90;

                //info DSP data is ready
                MApi_AUDIO_MM2_inputAesFinished(DecID, u32es_size, bSetPTS, u64PTS);
            }
        }
        else
        {
            printf("[%s][%d] play finished!!!\n", __FUNCTION__, __LINE__);

            //reset file index
            _au32AudioFileIndex[u32DevIdx] = 0;
        }
    }
    else
    {
        //old MM mode
        MS_U8* pAudioData = NULL;
        MS_U32 u32FileIndex = 0;
        MS_U32 u32LastAddr = 0;
        AUDIO_DEC_ID DecID = AU_DEC_INVALID;
        MS_U32 u32AudioLen = 0;

        pAudioData = _apu8AudioData[u32DevIdx];
        u32FileIndex = _au32AudioFileIndex[u32DevIdx];
        u32LastAddr = _au32clipWriteLastAddr[u32DevIdx];
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            DecID = stDualDemoInfo[u32DevIdx].eDecID;
        }
        else
        {
            DecID = AU_DEC_ID1;
        }
        u32AudioLen = _au32AudioLen[u32DevIdx];



        if( writeAddress == u32LastAddr )
        {
            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                MApi_AUDIO_SetAudioParam2(DecID, Audio_ParamType_esBuf_informDataRdy, 0);
            }
            else
            {
                MApi_AUDIO_SetInput();
            }
            return;
        }

        if(u32DevIdx==1)
        {
            //printf("+++++++++++tets1, writeAddress=0x%x, writeNumber=%d\n", writeAddress, writeNumber);
            //while(1);
        }

        u32LastAddr = writeAddress ;

        if ((u32FileIndex + writeNumber) <= u32AudioLen )
        {
            memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+u32FileIndex),writeNumber);
            u32FileIndex += writeNumber;
        }
        else
        {
            // clear the buffer first
            memset((void*)MS_PA2KSEG1(writeAddress), 0, writeNumber);

            if (writeNumber <= u32AudioLen )
            {
                // normal case
                int feedsize = 0;

                memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+u32FileIndex),u32AudioLen  - u32FileIndex);
                feedsize += (u32AudioLen - u32FileIndex);

                u32FileIndex = 0;
                memcpy((void*)MS_PA2KSEG1(writeAddress+feedsize), (void*)(pAudioData+u32FileIndex), writeNumber - feedsize);

                u32FileIndex = writeNumber - feedsize;
            }
        }

        _au32AudioFileIndex[u32DevIdx] = u32FileIndex;

        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetAudioParam2(DecID, Audio_ParamType_esBuf_informDataRdy, 0);
        }
        else
        {
            MApi_AUDIO_SetInput();
        }

    }

}

#if (DEMO_XC_HDMIRX_TEST == 1)
static void _Demo_HDMI_RX_HBR_Monitor(void)
{
    MS_BOOL bHDMIRX_OffLine = FALSE;
    msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bHDMIRX_OffLine);

    //check if HDMI Rx connected
    if ( bHDMIRX_OffLine == TRUE)
    {
        return;
    }

    MSAPI_HDMIRX_EXTEND_PACKET_RECEIVE stHDMIRxExtendPacketReceived = {0};
    stHDMIRxExtendPacketReceived.u16Version = MSAPI_HDMI_RX_EXTEND_PACKET_RECEIVE_VERSION;
    stHDMIRxExtendPacketReceived.u16Size = sizeof(MSAPI_HDMIRX_EXTEND_PACKET_RECEIVE);
    if (msAPI_HDMIRx_CTRL(E_MSAPI_HDMI_RX_EXTEND_PACKET_RECEIVE_GET, (void*)&stHDMIRxExtendPacketReceived, sizeof(MSAPI_HDMIRX_EXTEND_PACKET_RECEIVE)) )
    {

        if (stHDMIRxExtendPacketReceived.bPKT_HBR_RECEIVE)
        {
            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Set_HDMI_Rx_HighBitRate, 1, 0);
        }
        else
        {
            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Set_HDMI_Rx_HighBitRate, 0, 0);
        }
    }
}

static void _Demo_HDMI_RX_Monitor(void)
{
    MS_BOOL bHdmiInSub = FALSE;
    MS_BOOL bcur_hdmiIsRaw = MApi_AUDIO_HDMI_GetNonpcmFlag();
    AUDIO_HDMI_RX_TYPE st_crHDMI_RX_Mode = MApi_AUDIO_HDMI_RX_GetNonPCM();
    MS_U32 u32crHdmiAC3inSE = MApi_AUDIO_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE);
    MS_BOOL bHDMIRX_OffLine = FALSE;
    MS_U8 u8Idx = 0;

    //check if first run in here, always run the following flow, or some internal path may wrong.
    if(!_bFirstIn)
    {
        if((bcur_hdmiIsRaw == _bf_HdmiIsRaw) &&
          (st_crHDMI_RX_Mode == _bfHDMI_RX_Mode) &&
          (u32crHdmiAC3inSE == _bfHdmiAC3inSE))
        {
            return;
        }
    }

    msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bHDMIRX_OffLine);

    //check if HDMI Rx connected
    if ( bHDMIRX_OffLine == TRUE)
    {
        return;
    }

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        for(u8Idx=0; u8Idx<E_DECODER_NUMBER; u8Idx++)
        {
            if(stDualDemoInfo[u8Idx].stAudioDecStatus_t.bConnect)
            {
                //check if HDMI Rx application type
                if (stDualDemoInfo[u8Idx].stAudioDecStatus_t.eSourceType == E_AUDIO_INFO_HDMI_IN)
                {
                    // Check which audio codec is assigned to handle HDMI Rx data
                    // Use AU_DEC_ID1 if u32crHdmiAC3inSE == 0
                    // Use AU_DEC_ID3 if u32crHdmiAC3inSE == 1
                    if ( ((u32crHdmiAC3inSE == 0) && (stDualDemoInfo[u8Idx].eDecID == AU_DEC_ID1))
                        || ((u32crHdmiAC3inSE == 1) && (stDualDemoInfo[u8Idx].eDecID == AU_DEC_ID3)) )
                    {
                        if(bcur_hdmiIsRaw)
                        {
                            if (stDualDemoInfo[u8Idx].stAudioDecStatus_t.eGroup == E_CONNECT_MAIN)
                            {
                                MApi_AUDIO_HDMI_RX_SetNonpcm(0x01); //HDMI non-PCM setting in PIP Main
                            }
                            else
                            {
                                MApi_AUDIO_HDMI_RX_SetNonpcm(0x11); //HDMI non-PCM setting in PIP Sub
                            }

                            if(MApi_AUDIO_HDMI_RX_GetNonPCM() == HDMI_RX_DD)
                            {
                                MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DrcMode, LINE_MODE, 0);//Line Mod
                                MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DownmixMode, DOLBY_DOWNMIX_MODE_LTRT, 0); //LtRt
                            }
                        }
                        else
                        {
                            if (stDualDemoInfo[u8Idx].stAudioDecStatus_t.eGroup == E_CONNECT_MAIN)
                            {
                                MApi_AUDIO_HDMI_RX_SetNonpcm(0x00); //HDMI non-PCM setting in PIP Main
                            }
                            else
                            {
                                MApi_AUDIO_HDMI_RX_SetNonpcm(0x10); //HDMI non-PCM setting in PIP Sub
                            }
                        }
                        _bFirstIn= FALSE;
                    }
                }
            }
        }
    }
    else
    {
        if(bcur_hdmiIsRaw)
        {
            if(bHdmiInSub)
            {
                MApi_AUDIO_HDMI_RX_SetNonpcm(0x11); //HDMI non-PCM setting in PIP Sub
            }
            else
            {
                MApi_AUDIO_HDMI_RX_SetNonpcm(0x01); //HDMI non-PCM setting in PIP Main
            }

            if(MApi_AUDIO_HDMI_RX_GetNonPCM() == HDMI_RX_DD)
            {
                MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DrcMode, LINE_MODE, 0);                                            //Line Mod
                MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DownmixMode, DOLBY_DOWNMIX_MODE_LTRT, 0);                          //LtRt
            }
        }
        else
        {
            if(bHdmiInSub)
            {
                MApi_AUDIO_HDMI_RX_SetNonpcm(0x10); //HDMI non-PCM setting in PIP Sub
            }
            else
            {
                MApi_AUDIO_HDMI_RX_SetNonpcm(0x00); //HDMI non-PCM setting in PIP Main
            }
        }
        _bFirstIn= FALSE;
    }

    _bfHdmiAC3inSE = u32crHdmiAC3inSE;
    _bf_HdmiIsRaw = bcur_hdmiIsRaw;
    _bfHDMI_RX_Mode = st_crHDMI_RX_Mode;

}
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
static MS_U8* _Demo_Audio_EDID_Codec_String_Map(EN_AUDIO_EDID_CODEC_TYPE enCodec)
{
    switch (enCodec)
    {
        case DEMO_AUDIO_EDID_CODEC_LPCM: return (MS_U8*)"LPCM";
        case DEMO_AUDIO_EDID_CODEC_AC3: return (MS_U8*)"AC3";
        case DEMO_AUDIO_EDID_CODEC_MPEG1: return (MS_U8*)"MPEG1";
        case DEMO_AUDIO_EDID_CODEC_MP3: return (MS_U8*)"MP3";
        case DEMO_AUDIO_EDID_CODEC_MPEG2: return (MS_U8*)"MPEG2";
        case DEMO_AUDIO_EDID_CODEC_AAC: return (MS_U8*)"AAC";
        case DEMO_AUDIO_EDID_CODEC_DTS: return (MS_U8*)"DTS";
        case DEMO_AUDIO_EDID_CODEC_ATRAC: return (MS_U8*)"ATRAC";
        case DEMO_AUDIO_EDID_CODEC_SACD: return (MS_U8*)"SACD";
        case DEMO_AUDIO_EDID_CODEC_AC3P: return (MS_U8*)"DD+";
        case DEMO_AUDIO_EDID_CODEC_DTS_HD: return (MS_U8*)"DTS HD";
        case DEMO_AUDIO_EDID_CODEC_MLP_DOLBY_TRUEHD: return (MS_U8*)"MLP/Dolby True HD";
        case DEMO_AUDIO_EDID_CODEC_DST_AUDIO: return (MS_U8*)"DTS Audio";
        case DEMO_AUDIO_EDID_CODEC_WMAPRO: return (MS_U8*)"WMA Pro";
        default: return NULL;
    }
}

static MS_BOOL _Demo_Audio_HDMI_TX_Show_EDID(void)
{
    MS_U32 u32idx = 0;
    printf("================ Audio EDID Begin ==============\n");
    printf("%3s%25s%16s\n", (MS_U8*)"NO.", (MS_U8*)"Audio Codec", (MS_U8*)"Channel Num");
    for (u32idx=0;u32idx<_st_Audio_EDID.u32NumOfSAD;u32idx++)
    {
        if ((_st_Audio_EDID.stAudioEDID_SADs_t[u32idx].enCodec == DEMO_AUDIO_EDID_CODEC_AC3P) && (_st_Audio_EDID.bSupport_Dolby_Atmos))
        {
           printf("%3"DTC_MS_U32_d"%14s (w/ Atmos)%16d\n", u32idx+1
                                   , _Demo_Audio_EDID_Codec_String_Map(_st_Audio_EDID.stAudioEDID_SADs_t[u32idx].enCodec)
                                   , _st_Audio_EDID.stAudioEDID_SADs_t[u32idx].u8ChannelNum);
        }
        else
        {
           printf("%3"DTC_MS_U32_d"%25s%16d\n", u32idx+1
                                   , _Demo_Audio_EDID_Codec_String_Map(_st_Audio_EDID.stAudioEDID_SADs_t[u32idx].enCodec)
                                   , _st_Audio_EDID.stAudioEDID_SADs_t[u32idx].u8ChannelNum);
        }
    }

    printf("================ Audio EDID End =================\n");

    return TRUE;
}

static MS_BOOL _Demo_Audio_HDMI_TX_Update_EDID(MS_U8* pu8buf, MS_U32 u32length)
{
    MS_U32 u32idx = 0;

    //reset to zero
    memset(&_st_Audio_EDID, 0, sizeof(_st_Audio_EDID));

    // each SAD has 3 bytes
    _st_Audio_EDID.u32NumOfSAD = u32length/3;

    if (_st_Audio_EDID.u32NumOfSAD > EDID_MAX_SAD_COUNT)
    {
        printf("[%s][%d] EDID update fail!! Insufficient SAD array size!! Require(%"DTC_MS_U32_d") > Allocate(%d)\n", __FUNCTION__, __LINE__, _st_Audio_EDID.u32NumOfSAD, EDID_MAX_SAD_COUNT);
        return FALSE;
    }

    for (u32idx=0;u32idx<_st_Audio_EDID.u32NumOfSAD;u32idx++)
    {
        /*
        SAD Byte 1
            bit 7: Reserved (0)
            bit 6..3: Audio format code
            bit 2..0: number of channels minus 1
                      (i.e. 000 = 1 channel; 001 = 2 channels; 111 =8 channels)
        */
        _st_Audio_EDID.stAudioEDID_SADs_t[u32idx].enCodec = ((pu8buf[3*u32idx+0] >> 3) & 0xF);
        _st_Audio_EDID.stAudioEDID_SADs_t[u32idx].u8ChannelNum = (pu8buf[3*u32idx+0] & 0x07) + 1;

        /*
        SAD Byte 2 (sampling frequencies supported)
            bit 7: Reserved (0)
            bit 6: 192kHz
            bit 5: 176kHz
            bit 4: 96kHz
            bit 3: 88kHz
            bit 2: 48kHz
            bit 1: 44kHz
            bit 0: 32kHz
        */
        _st_Audio_EDID.stAudioEDID_SADs_t[u32idx].u8SampleRate = (pu8buf[3*u32idx+1] & 0x7);

        /*
        SAD Byte 3 (bit rate)
            For LPCM, bits 7:3 are reserved and the remaining bits define bit depth
                bit 2: 24 bit
                bit 1: 20 bit
                bit 0: 16 bit
            For all other sound formats, bits 7..0 designate the maximum supported bitrate divided by 8 kbit/s.
        */
        _st_Audio_EDID.stAudioEDID_SADs_t[u32idx].u8bitRate = pu8buf[3*u32idx+2];

        //update support codec into a bit string
        _st_Audio_EDID.u32SupportCodec_bitstring |= (1 << (_st_Audio_EDID.stAudioEDID_SADs_t[u32idx].enCodec));

        if ((_st_Audio_EDID.stAudioEDID_SADs_t[u32idx].enCodec == DEMO_AUDIO_EDID_CODEC_AC3P) && (_st_Audio_EDID.stAudioEDID_SADs_t[u32idx].u8bitRate & 0x01))
        {
            _st_Audio_EDID.bSupport_Dolby_Atmos = 1;
        }

    }

    return TRUE;
}

static void _Demo_HDMI_TX_Status_Monitor(void)
{
    MS_BOOL hdmi_tx_en = FALSE;
    AUDIO_FS_TYPE fs_type = AUDIO_FS_NONE;
    HDMI_TX_OUTPUT_TYPE outType = HDMI_OUT_PCM;
    HDMITX_AUDIO_FREQUENCY hreq = HDMITX_AUDIO_FREQ_NO_SIG;
    HDMITX_AUDIO_CODING_TYPE htype = HDMITX_AUDIO_PCM;

    MApi_AUDIO_HDMI_Tx_GetStatus(&hdmi_tx_en, &fs_type, &outType);

    if (hdmi_tx_en)
    {
        if((_stCurSampleType == fs_type) && (_stCurHDMIType == outType))
        {
            return;
        }
        else
        {
            _stCurSampleType = fs_type;
            _stCurHDMIType = outType;
        }

        if (outType == HDMI_OUT_NONPCM)
        {
            htype = HDMITX_AUDIO_NONPCM;
            switch (fs_type)
            {
            case AUDIO_FS_32KHZ:
                hreq = HDMITX_AUDIO_32K;
                break;
            case AUDIO_FS_44KHZ:
                hreq = HDMITX_AUDIO_44K;
                break;
            case AUDIO_FS_176KHZ:
                hreq = HDMITX_AUDIO_176K;
                break;
            case AUDIO_FS_192KHZ:
                hreq = HDMITX_AUDIO_192K;
                break;
            case AUDIO_FS_48KHZ:
            default:
                hreq = HDMITX_AUDIO_48K;
                break;
            }
        }
        else
        {
            htype = HDMITX_AUDIO_PCM;
            hreq = HDMITX_AUDIO_48K;                            //for PCM mode always SRC to 48K
        }

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
        if ((hreq == HDMITX_AUDIO_192K) || (hreq == HDMITX_AUDIO_176K))
        {
            //disable features(depend on chip's capability) when DDP bypass enable
            //kaiser disable feature: whole sound effect, 2nd decode(disable by upper layer), PCM capture, ATV encode
            if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DDP_Enable_FineTune_Feature, ENABLE, 0) == TRUE)
            {
                _bDDP_BYPASS_enable = TRUE;
            }
        }
        else
        {
            if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DDP_Enable_FineTune_Feature, DISABLE, 0) == TRUE)
            {
                _bDDP_BYPASS_enable = FALSE;
            }
        }
#endif

        printf("Hdmx Tx:%x , %x\n", hreq, htype);
        MApi_HDMITx_SetAudioOnOff(TRUE);
        MApi_HDMITx_SetAudioFrequency(hreq);
        MApi_HDMITx_SetAudioConfiguration(hreq, HDMITX_AUDIO_CH_2, htype);
    }

}
#endif

static MS_BOOL _appAudioDemo_MM_CheckInputRequest(AUDIO_DEC_ID DecID, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes, MS_BOOL bNewMMmode)
{
    AES_INFO es_info;
    memset(&es_info, 0, sizeof(AES_INFO));

    if((pU32WrtAddr == NULL) || (pU32WrtBytes == NULL))
    {
        return FALSE;
    }

    //check multi decode capability
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        //Audio V2 flow (multi decode)
        if (bNewMMmode)
        {
            //New MM mode
            if (MApi_AUDIO_MM2_checkAesInfo(DecID, &es_info))
            {
                *pU32WrtAddr = es_info.aes_write_addr;
                *pU32WrtBytes = es_info.aes_freeSpace;
                return TRUE;
            }
        }
        else
        {
            //Old MM mode
            Audio_MM_Data_Request st_Data_Request;

            if (MApi_AUDIO_GetAudioInfo2(DecID, Audio_infoType_esBuf_req, &st_Data_Request))
            {
                *pU32WrtAddr = st_Data_Request.U32WrtAddr;
                *pU32WrtBytes = st_Data_Request.U32WrtBytes;
                //printf("+++++++DecID=%d, pU32WrtAddr=0x%x, pU32WrtBytes=%d\n", DecID, *pU32WrtAddr, *pU32WrtBytes);
                return TRUE;
            }
        }
    }
    else
    {
        //Audio V1 flow
        if(bNewMMmode)
        {
            //New MM mode
            if (MApi_AUDIO_MM2_checkAesInfo(AU_DEC_ID1, &es_info))
            {
                *pU32WrtAddr = es_info.aes_write_addr;
                *pU32WrtBytes = es_info.aes_freeSpace;
                return TRUE;
            }
        }
        else
        {
            //Old MM mode
            return MApi_AUDIO_CheckInputRequest((MS_PHY*)pU32WrtAddr, pU32WrtBytes);
        }
    }

    return FALSE;
}

static void _appAudioDemo_task(MS_U32 argc, void *argv)
{
    MS_U32 u32WrtAddr=0;
    MS_U32 u32WrtBytes=0;
    MS_U8 u8Idx=0;

    while(_bExit_Demo_Task == FALSE)
    {
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //Audio V2 flow
            for(u8Idx=0; u8Idx<E_DECODER_NUMBER; u8Idx++)
            {
                if(stDualDemoInfo[u8Idx].stAudioDecStatus_t.bConnect)
                {
                    if(_appAudioDemo_MM_CheckInputRequest(
                        stDualDemoInfo[u8Idx].eDecID,
                        &u32WrtAddr,
                        &u32WrtBytes, _bNew_MM_Mode[u8Idx]))
                    {
                        if ((u32WrtAddr!=0) && (u32WrtBytes!=0))
                        {
                            _Demo_Audio_File_play(u8Idx, u32WrtAddr, u32WrtBytes, _bNew_MM_Mode[u8Idx]);
                        }
                    }
                }
            }
        }
        else
        {
            //Audio V1 flow
            if(_appAudioDemo_MM_CheckInputRequest(
                    AU_DEC_ID1,
                    &u32WrtAddr,
                    &u32WrtBytes, _bNew_MM_Mode[0]))
            {
                if ((u32WrtAddr!=0) && (u32WrtBytes!=0))
                {
                    _Demo_Audio_File_play(0, u32WrtAddr, u32WrtBytes, _bNew_MM_Mode[0]);
                }
            }
        }

        MsOS_DelayTask(1);
    }
    _bExit_Demo_Task = FALSE;
    _audioDemo_Task.iId = -1;

}

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
static void _Demo_DDP_BYPASS_Set_Feature_Status(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    switch (eAppType)
    {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            if(stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
            {
                 _bDSP2_Decode = TRUE;
            }
            break;
        case AUDIO_APP_ATV_FMTX:
            _bFMTX_enable = TRUE;
            break;
        case AUDIO_APP_ATV_BTSC:
            _bBTSC_enable = TRUE;
            break;
        default:
            break;
    }
}

static void _Demo_DDP_BYPASS_Clear_Feature_Status(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    switch (eAppType)
    {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            if(stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
            {
                 _bDSP2_Decode = FALSE;
            }
            break;
        case AUDIO_APP_ATV_FMTX:
            _bFMTX_enable = FALSE;
            break;
        case AUDIO_APP_ATV_BTSC:
            _bBTSC_enable = FALSE;
            break;
        default:
            break;
    }
}

static MS_BOOL _Demo_DDP_BYPASS_Check_Allow_Feature(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{

    if (_bDDP_BYPASS_enable)
    {
        if ((eAppType == AUDIO_APP_ES_PLAY) || (eAppType == AUDIO_APP_DTV) || (eAppType == AUDIO_APP_DTV_AM))
        {
            if (stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
            {
                return FALSE;
            }
        }
        else if((eAppType == AUDIO_APP_ATV_FMTX) || (eAppType == AUDIO_APP_ATV_BTSC))
        {
            return FALSE;
        }
    }

    return TRUE;

}
#endif

static void _appAudioMonitor_task(MS_U32 argc, void *argv)
{
    while(_bExit_Monitor_Task == FALSE)
    {
        //dump debug message and monitor if DSP alive
        MApi_Audio_Monitor();

#if (DEMO_XC_HDMIRX_TEST == 1)
        //monitor if send data from HDMI Rx
        _Demo_HDMI_RX_Monitor();

        _Demo_HDMI_RX_HBR_Monitor();
#endif


#if (DEMO_AUDIO_HDMI_TEST == 1)
        //monitor if HDMI Tx status change
        _Demo_HDMI_TX_Status_Monitor();
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
        //monitor smple rate change
        MApi_Audio_SPDIF_Monitor();
#endif

        MsOS_DelayTask(1);
    }
    _bExit_Monitor_Task = FALSE;
    _audioMonitor_Task.iId = -1;
}

#if (DEMO_AUDIO_ENCODER_TEST == 1)

static MS_BOOL _appAudio_Enc_DumpFile_task(MS_VIRT data)
{
    ST_AUDIO_ENC_DUMP_FILE_TASK* pstDumpFileTask = (ST_AUDIO_ENC_DUMP_FILE_TASK*)data;

    if (pstDumpFileTask == NULL)
    {
        return FALSE;
    }

    if (pstDumpFileTask->u8Filename == NULL)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"), no u8Filename!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }

    if (pstDumpFileTask->u32RequireDumpSize == 0)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"), no u32RequireDumpSize!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }

    if (pstDumpFileTask->s32DeviceId == -1)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"), no encode id!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }

    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    const MS_U32 u32bufsize = 4096;
    MS_U32 u32DumpSize = 0;
    MS_U8* pu8buf = NULL;
    FILE *outputfile = NULL;

#if (ENCODER_DUMP_LOG == 1)
    FILE *log_fp = NULL;
#endif

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    //allocate buffer for encode data
    pu8buf = MsOS_AllocateMemory(u32bufsize, s32MstarNonCachedPoolID);

    if (pu8buf == NULL)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"),allocate memory fail!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }

#if (ENCODER_DUMP_LOG == 1)
    if(pstDumpFileTask->s32DeviceId == 0)
    {
        log_fp = MsFS_Fopen("/mnt/sda1/log1.txt", "w+t");
    }
    else
    {
        log_fp = MsFS_Fopen("/mnt/sda1/log2.txt", "w+t");
    }

    if (log_fp == NULL)
    {
        printf("[%s][%d] open log_fp file fail!!!\n", __FUNCTION__, __LINE__);
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }
#endif

    //open file
    outputfile = MsFS_Fopen((char*)pstDumpFileTask->u8Filename, "w");
    if (outputfile == NULL)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"),open file fail!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }

    //start encode
    if (MApi_AUDIO_ENCODER_Start(pstDumpFileTask->s32DeviceId) < 0)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"), start dump fail!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        MsFS_Fclose(outputfile);
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        pstDumpFileTask->stTask_Info.iId = -1;
        return FALSE;
    }

    while(u32DumpSize < pstDumpFileTask->u32RequireDumpSize)
    {
        char s8Cmd[AUDIO_STRCMD_LENGTH] = {0};
        MS_U32 u32PTS_H = 0;
        MS_U32 u32PTS_L = 0;
        MS_U32 u32FrameSize = 0;
        MS_U32 u32AvailabeSize = 0;
        MS_U32 u32ReadSize = 0;
        MS_U32 u32FileWriteSize = 0;

        //get high 32 bit PTS
        snprintf(s8Cmd, AUDIO_STRCMD_LENGTH, "GetTimestampHigh32Bit");
        if (MApi_AUDIO_ENCODER_Ioctl(pstDumpFileTask->s32DeviceId, s8Cmd, &u32PTS_H) < 0)
        {
            printf("[%s][%d] encode id(%"DTC_MS_S32_d"),GetTimestampHigh32Bit error!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
            MsOS_DelayTask(1);
            continue;
        }

        //get low 32 bit PTS
        snprintf(s8Cmd, AUDIO_STRCMD_LENGTH, "GetTimestampLow32Bit");
        if (MApi_AUDIO_ENCODER_Ioctl(pstDumpFileTask->s32DeviceId, s8Cmd, &u32PTS_L) < 0)
        {
            printf("[%s][%d] encode id(%"DTC_MS_S32_d"),GetTimestampLow32Bit error!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
            MsOS_DelayTask(1);
            continue;
        }

        //get 1 frame size (byte)
        snprintf(s8Cmd, AUDIO_STRCMD_LENGTH, "GetFrameSize");
        if (MApi_AUDIO_ENCODER_Ioctl(pstDumpFileTask->s32DeviceId, s8Cmd, &u32FrameSize) < 0)
        {
            printf("[%s][%d] encode id(%"DTC_MS_S32_d"),GetFrameSize error!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
            MsOS_DelayTask(1);
            continue;
        }

        //get availabe size (byte)
        snprintf(s8Cmd, AUDIO_STRCMD_LENGTH, "GetAvailableSize");
        if (MApi_AUDIO_ENCODER_Ioctl(pstDumpFileTask->s32DeviceId, s8Cmd, &u32AvailabeSize) < 0)
        {
            printf("[%s][%d] encode id(%"DTC_MS_S32_d"),GetAvailableSize error!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
            MsOS_DelayTask(1);
            continue;
        }

        //check if frame size == 0
        if (u32FrameSize == 0)
        {
            printf("[%s][%d] encode id(%"DTC_MS_S32_d"),u32FrameSize = 0!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
            MsOS_DelayTask(1);
            continue;
        }

#if (ENCODER_DUMP_LOG == 1)
        fprintf(log_fp, "u32AvailabeSize=%d\n", u32AvailabeSize);
#endif

        //get encode data
        if (u32AvailabeSize >= u32FrameSize)
        {
            MS_U32 u32RequiredSize = u32FrameSize;

            u32ReadSize = MApi_AUDIO_ENCODER_Read(pstDumpFileTask->s32DeviceId, pu8buf, u32RequiredSize);

            if (u32ReadSize > 0)
            {
                //fwrite data
                u32FileWriteSize = MsFS_Fwrite(pu8buf, 1, u32ReadSize, outputfile);
                MsFS_Fflush(outputfile);
                u32DumpSize += u32FileWriteSize;
            }
        }

        MsOS_DelayTask(1);
    }

    //stop encode
    if (MApi_AUDIO_ENCODER_Stop(pstDumpFileTask->s32DeviceId) < 0)
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"), stop dump fail!!!\n", __FUNCTION__, __LINE__, pstDumpFileTask->s32DeviceId);
        MsFS_Fflush(outputfile);
        MsFS_Fclose(outputfile);
        MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);
        pstDumpFileTask->stTask_Info.iId = -1;

        return FALSE;
    }

    MsFS_Fflush(outputfile);

    //close file
    MsFS_Fclose(outputfile);

#if (ENCODER_DUMP_LOG == 1)
    MsFS_Fflush(log_fp);
    MsFS_Fclose(log_fp);
#endif

    printf("======== encode id(%"DTC_MS_S32_d"), Finish dump to file !!! ========\n", pstDumpFileTask->s32DeviceId);

    MsOS_FreeMemory(pu8buf, s32MstarNonCachedPoolID);

    pstDumpFileTask->stTask_Info.iId = -1;
    return TRUE;
}
#endif


#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)

static MS_BOOL _appAudio_Transcode_task(MS_VIRT data)
{
    ST_AUDIO_TRANSCODE_TASK* pstTranscodeTask = (ST_AUDIO_TRANSCODE_TASK*)data;

    MS_BOOL bFunRet = FALSE;
    MS_U16 aSrc[NO_INPUT_SAMPLES * NO_CHANNELS] = {0};
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

#ifdef TRANSCODE_NORMAL_FLOW
    MS_U32 u32NoOfFrames = 0;
    MS_U32 u32aSrcSize = 0;
    MS_U32 u32pcmbuf_remain_size = 0;
    MS_U32 u32prepcmbufsize=0;
    MS_U32 u32pcm_buffer_cnt = 0;

#endif

    FILE *outputfp = NULL;

#ifdef TRANSCODE_OUTPUT_RAW_PCMBUF
    FILE *outputfp1 = NULL;
#endif

    Buffer_Info_t stpcm_buffer = {0, 0, 0, 0};

#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
#ifdef TRANSCODE_NORMAL_FLOW
    MS_U64 u64BaseAudioEnc = 0;
    ioctl_reset_enc_args stReset_args = {0};
    ioctl_encode_args stEnc_args = {0};
    tMp4HEAACEncParams stAACParam = {.NumberOfChannels = 2,
                                      .SamplingFrequency = 48000,
                                      .OutputBitRate = 64000,
                                      .OutputFormat = FORMAT_ADTS,
                                      .BandWidthSel = 100,
                                      .ChannelMode = JOINT_STEREO,
                                      .TurnOnPns = 0,
                                      .TurnOnTns = 0,
                                      .Flag_VBR = 0,
                                      .VersionInfo = 0,
                                      .sbr_mode = 0,
                                      .BitsPerSample = 0,
                                      .parametricstereo = 1};
    tEmzInt32 aDstLen=0;
	tEmzUint8 aDst[6144]={0};
#endif
#endif

    if (pstTranscodeTask == NULL)
    {
        bFunRet = FALSE;
        goto TheEnd;
    }

    // change task status to START
    pstTranscodeTask->enTaskStatus = AUDIO_MONITOR_TASK_START;

    // check if transcode to a file, and make sure file exist
    if ((pstTranscodeTask->enTranscodePath == DEMO_AUDIO_TRANSCODE_PATH_FILE) && (pstTranscodeTask->u8Filename == NULL))
    {
        printf("[%s][%d] encode id(%"DTC_MS_S32_d"), no u8Filename!!!\n", __FUNCTION__, __LINE__, pstTranscodeTask->s32DeviceId);
        bFunRet = FALSE;
        goto TheEnd;
    }

    // check if open file success
    if (pstTranscodeTask->enTranscodePath == DEMO_AUDIO_TRANSCODE_PATH_FILE)
    {
        if (pstTranscodeTask->bOutFileAppend)
        {
            outputfp = MsFS_Fopen((char*)pstTranscodeTask->u8Filename, "ab");
        }
        else
        {
            outputfp = MsFS_Fopen((char*)pstTranscodeTask->u8Filename, "wb");
        }

        if (outputfp == NULL)
        {
            printf("[%s][%d]open file %s fail\n", __FUNCTION__, __LINE__, (char*)pstTranscodeTask->u8Filename);
            bFunRet = FALSE;
            goto TheEnd;
        }


#ifdef TRANSCODE_OUTPUT_RAW_PCMBUF
        outputfp1 = MsFS_Fopen((char*)"/mnt/sda1/test.pcm", "wb");
        if (outputfp1 == NULL)
        {
            printf("[%s][%d]open file test.pcm fail\n", __FUNCTION__, __LINE__);
            bFunRet = FALSE;
            goto TheEnd;
        }
#endif
    }

    if ((pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_ES) ||
        (pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_PES))
    {

#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
#ifdef TRANSCODE_NORMAL_FLOW
        // Open the heaac encoder device with read/write access

        if (_s32fd == -1)
        {
            _s32fd = open("/dev/heaac_enc", O_RDWR);

            if (_s32fd < 0)
            {
                printf("Failed to open the device...\n");
                bFunRet = FALSE;
                goto TheEnd;
            }
        }

        // create HEAAC encoder device
        if (ioctl(_s32fd, EMZ_CMD_CREATE_ENCODER,&u64BaseAudioEnc) != E_EMZ_SUCCESS)
        {
            /* Error in creating the encoder insance */
            printf("Error in creation \n");
            bFunRet = FALSE;
            goto TheEnd;
        }

        stReset_args.u64AACParams = (tEmzUint32)&stAACParam;
        stReset_args.u64handle = u64BaseAudioEnc;

        // reset HEAAC encoder device
        if (ioctl(_s32fd, EMZ_CMD_RESET_ENCODER,&stReset_args) != E_EMZ_SUCCESS)
        {
            /* Error in resetting the values */
            printf("Error in reset %d\n", stAACParam.OutputFormat);
            bFunRet = FALSE;
            goto TheEnd;
        }

#endif
#endif

    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    printf("[%s][%d] Transcode Start!!!\n", __FUNCTION__, __LINE__);

    while( (pstTranscodeTask->enTaskStatus == AUDIO_MONITOR_TASK_START) || (pstTranscodeTask->enTaskStatus == AUDIO_MONITOR_TASK_PAUSE))
    {

        MS_BOOL bret = FALSE;

        if (pstTranscodeTask->enTaskStatus == AUDIO_MONITOR_TASK_PAUSE)
        {
            MsOS_DelayTask(1);
            continue;
        }
#ifdef TRANSCODE_NORMAL_FLOW

        // put sample to aSrc till the buffer size is full.
        while (((pstTranscodeTask->enTaskStatus == AUDIO_MONITOR_TASK_START) || (pstTranscodeTask->enTaskStatus == AUDIO_MONITOR_TASK_PAUSE))
               && (u32aSrcSize < sizeof(aSrc)))
        {

            if (pstTranscodeTask->enTaskStatus == AUDIO_MONITOR_TASK_PAUSE)
            {
                MsOS_DelayTask(1);
                continue;
            }

            MS_U32 u32targer_cnt = 0;
            MS_U32 u32pcmbuf_remain_sample_cnt = 0;
            MS_U32 u32aSrc_remain_sample_cnt = 0;

            // check if pcm buffer empty then get new pcm data from audio
            if (u32pcmbuf_remain_size == 0)
            {
                // get current frame size, for allocate buffer
                bret = FALSE;
                stpcm_buffer.u32Size = 0;
                bret = MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, Audio_infoType_Decoder_Pcm_Size, (void *)&stpcm_buffer.u32Size);

                if ((bret == FALSE) || (stpcm_buffer.u32Size == 0))
                {
                    MsOS_DelayTask(1);
                    continue;
                }

                //printf("++++++dec id=%d, stpcm_buffer.u32Size=%d\n", stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, stpcm_buffer.u32Size);

                if (stpcm_buffer.pBuffer)
                {
                    //if buffer exist, check if need allocate again
                    if (u32prepcmbufsize < stpcm_buffer.u32Size)
                    {
                        // free old one
                        if (MsOS_FreeMemory(stpcm_buffer.pBuffer, s32MstarCachedPoolID))
                        {
                            // allocate PCM size of current frame.
                            stpcm_buffer.pBuffer = MsOS_AllocateMemory(stpcm_buffer.u32Size, s32MstarCachedPoolID);
                            memset(stpcm_buffer.pBuffer, 0, stpcm_buffer.u32Size);
                            u32prepcmbufsize = stpcm_buffer.u32Size;
                        }
                        else
                        {
                            MsOS_DelayTask(1);
                            continue;
                        }
                    }
                    else
                    {
                        //clean buffer
                        memset(stpcm_buffer.pBuffer, 0, u32prepcmbufsize);
                    }
                }
                else
                {
                    // if buffer not exist, create one
                    stpcm_buffer.pBuffer = MsOS_AllocateMemory(stpcm_buffer.u32Size, s32MstarCachedPoolID);
                    memset(stpcm_buffer.pBuffer, 0, stpcm_buffer.u32Size);
                    u32prepcmbufsize = stpcm_buffer.u32Size;
                }

                //get PCM data
                bret = FALSE;
                bret = MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, Audio_infoType_Decoder_Pcm_Buffer_Info, (void *)&stpcm_buffer);

                if (bret == TRUE)
                {
                    u32pcmbuf_remain_size = stpcm_buffer.u32Size;
                    u32NoOfFrames++;
                    u32pcm_buffer_cnt = 0;

#ifdef TRANSCODE_OUTPUT_RAW_PCMBUF
                    if (pstTranscodeTask->enTranscodePath == DEMO_AUDIO_TRANSCODE_PATH_FILE)
                    {
                        MsFS_Fwrite(stpcm_buffer.pBuffer, 1, stpcm_buffer.u32Size, outputfp1);

                        MsFS_Fflush(outputfp1);
                    }
#endif
                }
                continue;

            }
            else
            {
                MS_U16* u16dest = NULL;
                MS_U16* u16src = NULL;
                MS_U32 u32Sample_idx = 0;

                // PCM buffer remain sample count, 10 = 10 ch, 2 = 2 bytes
                u32pcmbuf_remain_sample_cnt = u32pcmbuf_remain_size/(10*2);

                // aSrc buufer remain sample count (aSrc buffer is the buffer wait for encoding), 2 = 2 downmix channel. 2 = 2bytes
                u32aSrc_remain_sample_cnt = (sizeof(aSrc) - u32aSrcSize)/(2*2);

                // calculate the 1 channel sample count thar need to put in aSrc buffer
                if(u32aSrc_remain_sample_cnt >= u32pcmbuf_remain_sample_cnt)
                {
                    u32targer_cnt = u32pcmbuf_remain_sample_cnt;
                }
                else
                {
                    u32targer_cnt = u32aSrc_remain_sample_cnt;
                }

                u16dest = (MS_U16 *) (&aSrc[u32aSrcSize/2]);
                u16src = (MS_U16 *) stpcm_buffer.pBuffer;

                //update pcm buffer offset
                u16src += (u32pcm_buffer_cnt*10);

                for (u32Sample_idx = 0; u32Sample_idx < u32targer_cnt; u32Sample_idx++)
                {
                    //src[0]: downmix L
                    //src[1]" downmix R
                    //src[2]" L
                    //src[3]" C
                    //src[4]" R
                    //src[5]" Ls
                    //src[6]" Rs
                    //src[7]" LFE
                    //src[8]" reserve
                    //src[9]" reserve
                    u16dest[0] = u16src[0];
                    u16dest[1] = u16src[1];
                    u16dest+=2;
                    u16src+=10;
                }

                // update aSrc level
                u32aSrcSize = u32aSrcSize + (u32targer_cnt*2*2);

                // update pcm buffer remain size
                u32pcmbuf_remain_size = u32pcmbuf_remain_size - (u32targer_cnt*10*2);

                // update pcm buffer count
                u32pcm_buffer_cnt += u32targer_cnt;

                //printf("++++++dec id=%d, u32aSrcSize=%d, u32pcmbuf_remain_size=%d, u32targer_cnt=%d, u32pcm_buffer_cnt=%d\n", stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, u32aSrcSize, u32pcmbuf_remain_size, u32targer_cnt, u32pcm_buffer_cnt);

                if((u32pcm_buffer_cnt*10*2)>stpcm_buffer.u32Size)
                {
                    printf("[%s][%d] Error!!!\n", __FUNCTION__, __LINE__);
                }

            }
            MsOS_DelayTask(1);
        }

        if ((pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_ES) ||
                (pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_PES))
        {
#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
            // start encode frame
            aDstLen = 6144;
            stEnc_args.u64handle = u64BaseAudioEnc;
            stEnc_args.u64Src = (tEmzUint32)aSrc;
            stEnc_args.SrcLen = sizeof(aSrc);
            stEnc_args.u64Dst = (tEmzUint32)aDst;
            stEnc_args.u64DstLen = (tEmzUint32)(&aDstLen);

            if (ioctl(_s32fd, EMZ_CMD_CODEC_ENCODE,&stEnc_args) != E_EMZ_SUCCESS)
            {
                // Error in encoding
                printf("Error in encode @593 \n");
                goto TheEnd;
            }

            if(aDstLen > 6144)
            {
                printf("Error!!!!! More Error \n");
            }
#endif

        }

        if (pstTranscodeTask->enTranscodePath == DEMO_AUDIO_TRANSCODE_PATH_FILE)
        {
            if ((pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_ES) ||
                (pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_PES))
            {
#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
                // encode data
                MsFS_Fwrite(aDst, 1, aDstLen, outputfp);
#endif
            }
            else if (pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_PCM)
            {
                // raw pcm data
                MsFS_Fwrite(aSrc, 1, sizeof(aSrc), outputfp);
            }

            MsFS_Fflush(outputfp);

        }

        // clean aSrc buffer
        memset(aSrc, 0, sizeof(aSrc));

        // update aSrc level
        u32aSrcSize = 0;
#else

        stpcm_buffer.u32Size = 0;
        bret = MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, Audio_infoType_Decoder_Pcm_Size, (void *)&stpcm_buffer.u32Size);

        if (bret)
        {
            printf("++++++dec id=%d, stpcm_buffer.u32Size=%d\n", stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, stpcm_buffer.u32Size);
        }
        else
        {
            MsOS_DelayTask(1);
            continue;
        }

        if((stpcm_buffer.u32Size/10)*2 > sizeof(aSrc))
        {
            printf("[%s][%d] Error!!! stpcm_buffer.u32Size > szieof(aSrc)!!!\n", __FUNCTION__, __LINE__);
            goto TheEnd;
        }

        if(stpcm_buffer.u32Size != 0)
        {
            if(stpcm_buffer.pBuffer)
            {
                MsOS_FreeMemory(stpcm_buffer.pBuffer, s32MstarCachedPoolID);
                stpcm_buffer.pBuffer = NULL;
            }

            stpcm_buffer.pBuffer = MsOS_AllocateMemory(stpcm_buffer.u32Size, s32MstarCachedPoolID);
            memset(stpcm_buffer.pBuffer, 0, stpcm_buffer.u32Size);
        }
        else
        {
            MsOS_DelayTask(1);
            continue;
        }

        bret = MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[pstTranscodeTask->s32DeviceId].eDecID, Audio_infoType_Decoder_Pcm_Buffer_Info, (void *)&stpcm_buffer);

        if (bret == FALSE)
        {
            if(stpcm_buffer.pBuffer)
            {
                MsOS_FreeMemory(stpcm_buffer.pBuffer, s32MstarCachedPoolID);
                stpcm_buffer.pBuffer = NULL;
            }
            MsOS_DelayTask(1);
            continue;
        }

        MS_U16* u16dest = aSrc;
        MS_U16* u16src = (MS_U16 *) stpcm_buffer.pBuffer;
        MS_U32 u32Sample_idx = 0;

        if (bret == TRUE)
        {
            for (u32Sample_idx = 0; u32Sample_idx < (stpcm_buffer.u32Size/(10*2)); u32Sample_idx++)
                {
                    //src[0]: downmix L
                    //src[1]" downmix R
                    //src[2]" L
                    //src[3]" C
                    //src[4]" R
                    //src[5]" Ls
                    //src[6]" Rs
                    //src[7]" LFE
                    //src[8]" reserve
                    //src[9]" reserve
                    u16dest[0] = u16src[0];
                    u16dest[1] = u16src[1];
                    u16dest+=2;
                    u16src+=10;
                }
        }

        if (pstTranscodeTask->enTranscodePath == DEMO_AUDIO_TRANSCODE_PATH_FILE)
        {
            MsFS_Fwrite(aSrc, 1, stpcm_buffer.u32Size/10 * 2, outputfp);

            MsFS_Fflush(outputfp);

#ifdef TRANSCODE_OUTPUT_RAW_PCMBUF

            MsFS_Fwrite(stpcm_buffer.pBuffer, 1, stpcm_buffer.u32Size, outputfp1);

            MsFS_Fflush(outputfp1);
#endif
        }

        if (stpcm_buffer.pBuffer)
        {
            MsOS_FreeMemory(stpcm_buffer.pBuffer, s32MstarCachedPoolID);
            stpcm_buffer.pBuffer = NULL;
        }

#endif//#ifdef TRANSCODE_NORMAL_FLOW

        MsOS_DelayTask(1);
    }

TheEnd:

    // free memory
    if (stpcm_buffer.pBuffer)
    {
        MsOS_FreeMemory(stpcm_buffer.pBuffer, s32MstarCachedPoolID);
        stpcm_buffer.pBuffer = NULL;
    }

    if ((pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_ES) ||
        (pstTranscodeTask->enTranscodeFormat == DEMO_AUDIO_TRANSCODE_FORMAT_PES))
    {
#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 1)
#ifdef TRANSCODE_NORMAL_FLOW
        // Close the heaac encoder device with read/write access
        if (_s32fd >= 0)
        {
            //close(_s32fd);
        }
#endif
#endif
    }

    if (pstTranscodeTask->enTranscodePath == DEMO_AUDIO_TRANSCODE_PATH_FILE)
    {
        if (outputfp)
        {
            MsFS_Fflush(outputfp);
            MsFS_Fclose(outputfp);
        }

#ifdef TRANSCODE_OUTPUT_RAW_PCMBUF
        if (outputfp1)
        {
            MsFS_Fflush(outputfp1);
            MsFS_Fclose(outputfp1);
        }
#endif

    }

    // change task status to STOP
    pstTranscodeTask->enTaskStatus = AUDIO_MONITOR_TASK_STOP;

    pstTranscodeTask->stTask_Info.iId = -1;

    return bFunRet;
}

#endif

#if ((DEMO_AUDIO_DMA_READER_TEST == 1) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
static void _appAudio_DMA_Reader_EX_Task(MS_VIRT data)
{
    ST_AUDIO_PCM_IO_TASK* pstDMAReaderTask = (ST_AUDIO_PCM_IO_TASK*)data;

    if (pstDMAReaderTask == NULL)
    {
        return;
    }

    if ( strcmp((char*)pstDMAReaderTask->u8Filename, "") == 0)
    {
        printf("[%s][%d] Error!!! No u8Filename!!!\n", __FUNCTION__, __LINE__);
        pstDMAReaderTask->stTask_Info.iId = -1;
        return;
    }

    //Allocate DMA Reader device
    pstDMAReaderTask->s32DeviceId = MApi_AUDIO_PCM_Open(&(pstDMAReaderTask->tPcmInfo));

    if (pstDMAReaderTask->s32DeviceId >= 0)
    {
        MS_U32 u32RepeatLoop = pstDMAReaderTask->u32RepeatLoop;
        FILE *file = NULL;
        MS_U8* pbuf = NULL;
        MS_U32 u32File_size = 0;
        MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
        MS_U32 u32PCM_Write_size = 0;
        MS_U32 u32Residual_size = 0;
        MS_U32 u32File_Read_size = 0;
        MS_U32 u32Actual_File_Read_size = 0;
        MS_U32 u32Actual_PCM_Read_size = 0;

        file = MsFS_Fopen((char*)pstDMAReaderTask->u8Filename, "rb");

        if (file == NULL)
        {
            printf("open file fail!!!\n");
            pstDMAReaderTask->stTask_Info.iId = -1;
            return;
        }

        //Get total file size
        MsFS_Fseek(file, 0, SEEK_END);
        u32File_size = MsFS_Ftell(file);
        MsFS_Fseek(file, 0, SEEK_SET);

        //Calculate 20ms PCM data size. Suggest 20ms.
        u32PCM_Write_size = ((20 * pstDMAReaderTask->tPcmInfo.u32SampleRate) / 1000) * pstDMAReaderTask->tPcmInfo.u32Channel * (pstDMAReaderTask->tPcmInfo.u32BitWidth / 8);

        //Allocate 20ms buffer size.
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

        pbuf = MsOS_AllocateMemory(u32PCM_Write_size, s32MstarCachedPoolID);
        if (pbuf == NULL)
        {
            printf("allocate memory fail!!!\n");
            MsFS_Fclose(file);
            pstDMAReaderTask->stTask_Info.iId = -1;
            return;
        }

        //Start DMA Reader
        MApi_AUDIO_PCM_Start(pstDMAReaderTask->s32DeviceId);

        while ((u32RepeatLoop!=0) || (u32Residual_size!=0))
        {
            if (u32Residual_size == 0)
            {
                MsFS_Fseek(file, 0, SEEK_SET);     // Seek to file begin if no data can read
                u32Residual_size = u32File_size;
                u32RepeatLoop--;
                printf("%s: count = %"DTC_MS_U32_d"\n", pstDMAReaderTask->tPcmInfo.u8Name, u32RepeatLoop);
            }

            u32File_Read_size = (u32Residual_size >= u32PCM_Write_size) ? u32PCM_Write_size : u32Residual_size;

            u32Actual_File_Read_size = MsFS_Fread((void *)pbuf, 1, u32File_Read_size, file);

            //Write PCM data
            u32Actual_PCM_Read_size = MApi_AUDIO_PCM_Write(pstDMAReaderTask->s32DeviceId, (void *)pbuf, u32Actual_File_Read_size);

            u32Residual_size = u32Residual_size - u32Actual_PCM_Read_size;

            MsOS_DelayTask(1);
        }

        //Stop DMA reader
        MApi_AUDIO_PCM_Stop(pstDMAReaderTask->s32DeviceId);

        //Flush(clean) buffer data
        MApi_AUDIO_PCM_Flush(pstDMAReaderTask->s32DeviceId);

        //Close DMA reader device
        MApi_AUDIO_PCM_Close(pstDMAReaderTask->s32DeviceId);

        MsFS_Fclose(file);

        MsOS_FreeMemory(pbuf, s32MstarCachedPoolID);

    }
    else
    {
        printf("Error!!! \"%s\" not availabe \n", pstDMAReaderTask->tPcmInfo.u8Name);
    }

    pstDMAReaderTask->stTask_Info.iId = -1;
    return;
}
#endif

#if (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)
void _appAudio_PCM_Capture_Task(MS_VIRT data)
{
    ST_AUDIO_PCM_IO_TASK* pstPCMCaptureTask = (ST_AUDIO_PCM_IO_TASK*)data;
    FILE *file = NULL;
    AUDIO_DEVICE_TYPE enDeviceID = E_DEVICE0;

    if (pstPCMCaptureTask == NULL)
    {
        return;
    }

    if ( strcmp((char*)pstPCMCaptureTask->u8Filename, "") == 0)
    {
        printf("[%s][%d] Error!!! No u8Filename!!!\n", __FUNCTION__, __LINE__);
        goto TheEnd;
    }

    //Allocate PCM Capture device
    pstPCMCaptureTask->s32DeviceId = MApi_AUDIO_PCM_Open(&(pstPCMCaptureTask->tPcmInfo));

    if (pstPCMCaptureTask->s32DeviceId >= 0)
    {
        MS_U8 u8PCMbuf[4096] = {0};
        MS_U32 u32RetSize = 0;

        if (strcmp((char*)&pstPCMCaptureTask->tPcmInfo.u8Name, (char*)AUDIO_PCM_CAPTURE1) == 0 )
        {
            // H/W capture 1 used.
            enDeviceID = E_DEVICE0;
        }
        else if (strcmp((char*)&pstPCMCaptureTask->tPcmInfo.u8Name, (char*)AUDIO_PCM_CAPTURE2) == 0 )
        {
            // H/W capture 2 used.
            enDeviceID = E_DEVICE1;
        }
        else
        {
            printf("Error !!! Not support device (%s)\n", pstPCMCaptureTask->tPcmInfo.u8Name);
            goto TheEnd1;
        }

        if (pstPCMCaptureTask->enOutputPath == DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE)
        {
            if (pstPCMCaptureTask->bOutFileAppend)
            {
                file = MsFS_Fopen((char*)pstPCMCaptureTask->u8Filename, "ab");
            }
            else
            {
                file = MsFS_Fopen((char*)pstPCMCaptureTask->u8Filename, "wb");
            }

            if (file == NULL)
            {
                printf("open file fail!!!\n");
                goto TheEnd1;
            }
        }

        //[20180110][Notice] Darren.Chen temporary add api here, will include in Common PCM IO in the future.
        if (MApi_AUDIO_SetDataCaptureSource(enDeviceID, pstPCMCaptureTask->enCaptureSource) == FALSE)
        {
            printf("Set Data Capture Source fail!!!\n");
            goto TheEnd1;
        }

        if (MApi_AUDIO_PCM_Start(pstPCMCaptureTask->s32DeviceId) >= 0)
        {

            printf("[%s][%d] %s Start!!!\n", __FUNCTION__, __LINE__, (char*)pstPCMCaptureTask->tPcmInfo.u8Name);
            pstPCMCaptureTask->enTaskStatus = AUDIO_MONITOR_TASK_START;

            while (pstPCMCaptureTask->enTaskStatus == AUDIO_MONITOR_TASK_START)
            {
                u32RetSize = MApi_AUDIO_PCM_Read(pstPCMCaptureTask->s32DeviceId, u8PCMbuf, 4096);

                if (pstPCMCaptureTask->enOutputPath == DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE)
                {
                    MsFS_Fwrite(u8PCMbuf, sizeof(MS_U8), u32RetSize, file);
                }
                MsOS_DelayTask(1);
            }
        }
        else
        {
            printf("MApi_AUDIO_PCM_Start(%d) fail!!!\n", pstPCMCaptureTask->s32DeviceId);
            goto TheEnd1;
        }

    }
    else
    {
        printf("Error!!! \"%s\" not availabe, err type(%d) \n", pstPCMCaptureTask->tPcmInfo.u8Name, pstPCMCaptureTask->s32DeviceId);
        goto TheEnd;
    }

TheEnd1:
    if (MApi_AUDIO_PCM_Stop(pstPCMCaptureTask->s32DeviceId) < 0)
    {
        //fail
        printf("MApi_AUDIO_PCM_Stop fail!!!\n");
        goto TheEnd;
    }

    if (MApi_AUDIO_PCM_Close(pstPCMCaptureTask->s32DeviceId) < 0)
    {
        //fail
        printf("MApi_AUDIO_PCM_Close fail!!!\n");
        goto TheEnd;
    }

TheEnd:
    if (pstPCMCaptureTask->enOutputPath == DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE)
    {
        if (file)
        {
            MsFS_Fflush(file);
            MsFS_Fclose(file);
        }
    }

    pstPCMCaptureTask->stTask_Info.iId = -1;
    return;
}
#endif

static En_DVB_decSystemType _Demo_Audio_CodecTypeMap(EN_AUDIO_CODEC_TYPE eCodecType)
{
    switch(eCodecType)
    {
        case DEMO_AUDIO_CODEC_NONE: return MSAPI_AUD_DVB_NONE;
        case DEMO_AUDIO_CODEC_MPEG: return MSAPI_AUD_DVB_MPEG;
        case DEMO_AUDIO_CODEC_AC3: return MSAPI_AUD_DVB_AC3;
        case DEMO_AUDIO_CODEC_AC3P: return MSAPI_AUD_DVB_AC3P;
        case DEMO_AUDIO_CODEC_AAC: return MSAPI_AUD_DVB_AAC;
        case DEMO_AUDIO_CODEC_MP3: return MSAPI_AUD_DVB_MP3;
        case DEMO_AUDIO_CODEC_WMA: return MSAPI_AUD_DVB_WMA;
        case DEMO_AUDIO_CODEC_CDLPCM: return MSAPI_AUD_DVB_CDLPCM;
        case DEMO_AUDIO_CODEC_RA8LBR: return MSAPI_AUD_DVB_RA8LBR;
        case DEMO_AUDIO_CODEC_XPCM: return MSAPI_AUD_DVB_XPCM;
        case DEMO_AUDIO_CODEC_TONE: return MSAPI_AUD_DVB_TONE;
        case DEMO_AUDIO_CODEC_DTS: return MSAPI_AUD_DVB_DTS;
        case DEMO_AUDIO_CODEC_MS10_DDT: return MSAPI_AUD_DVB_MS10_DDT;
        case DEMO_AUDIO_CODEC_MS10_DDC: return MSAPI_AUD_DVB_MS10_DDC;
        case DEMO_AUDIO_CODEC_WMA_PRO: return MSAPI_AUD_DVB_WMA_PRO;
        case DEMO_AUDIO_CODEC_FLAC: return MSAPI_AUD_DVB_FLAC;
        case DEMO_AUDIO_CODEC_VORBIS: return MSAPI_AUD_DVB_VORBIS;
        case DEMO_AUDIO_CODEC_DTSLBR: return MSAPI_AUD_DVB_DTSLBR;
        case DEMO_AUDIO_CODEC_AMR_NB: return MSAPI_AUD_DVB_AMR_NB;
        case DEMO_AUDIO_CODEC_AMR_WB: return MSAPI_AUD_DVB_AMR_WB;
        case DEMO_AUDIO_CODEC_DRA: return MSAPI_AUD_DVB_DRA;
        case DEMO_AUDIO_CODEC_DolbyTrueHDBypass: return MSAPI_AUD_DVB_DolbyTrueHDBypass;
        case DEMO_AUDIO_CODEC_DTSHDADO: return MSAPI_AUD_DVB_DTSHDADO;
        case DEMO_AUDIO_CODEC_DTSXLL: return MSAPI_AUD_DVB_DTSXLL;
        case DEMO_AUDIO_CODEC_ES_Bypass: return MSAPI_AUD_DVB_ES_Bypass;
        case DEMO_AUDIO_CODEC_AC4: return MSAPI_AUD_DVB_AC4;
        case DEMO_AUDIO_CODEC_MPEG_H: return MSAPI_AUD_DVB_MPEG_H;

        //AD
        case DEMO_AUDIO_CODEC_AD_NONE: return MSAPI_AUD_DVB2_NONE;
        case DEMO_AUDIO_CODEC_AD_MPEG: return MSAPI_AUD_DVB2_MPEG;
        case DEMO_AUDIO_CODEC_AD_AC3: return MSAPI_AUD_DVB2_AC3;
        case DEMO_AUDIO_CODEC_AD_MP3: return MSAPI_AUD_DVB2_MP3;
        case DEMO_AUDIO_CODEC_AD_AAC: return MSAPI_AUD_DVB2_AAC;
        case DEMO_AUDIO_CODEC_AD_GAAC: return MSAPI_AUD_DVB2_GAAC;
        case DEMO_AUDIO_CODEC_AD_AC3P: return MSAPI_AUD_DVB2_AC3P;
        case DEMO_AUDIO_CODEC_AD_DDE: return MSAPI_AUD_DVB2_DDE;
        case DEMO_AUDIO_CODEC_AD_DTSE: return MSAPI_AUD_DVB2_DTSE;
        case DEMO_AUDIO_CODEC_AD_XPCM: return MSAPI_AUD_DVB2_XPCM;
        case DEMO_AUDIO_CODEC_AD_KTV: return MSAPI_AUD_DVB2_KTV;
        case DEMO_AUDIO_CODEC_AD_KTV2: return MSAPI_AUD_DVB2_KTV2;
        case DEMO_AUDIO_CODEC_AD_DTS: return MSAPI_AUD_DVB2_DTS;
        case DEMO_AUDIO_CODEC_AD_TONEGEN: return MSAPI_AUD_DVB2_TONEGEN;
        case DEMO_AUDIO_CODEC_AD_DolbyTrueHDBypass: return MSAPI_AUD_DVB2_DolbyTrueHDBypass;
        case DEMO_AUDIO_CODEC_AD_DTSHDADO: return MSAPI_AUD_DVB2_DTSHDADO;
        case DEMO_AUDIO_CODEC_AD_AC4: return MSAPI_AUD_DVB2_AC4;
        case DEMO_AUDIO_CODEC_AD_MPEG_H: return MSAPI_AUD_DVB2_MPEG_H;

        //ATV SIF
        case DEMO_AUDIO_CODEC_ATV_NONE: return MSAPI_AUD_ATV_NONE;
        case DEMO_AUDIO_CODEC_ATV_BTSC: return MSAPI_AUD_ATV_BTSC;
        case DEMO_AUDIO_CODEC_ATV_EIAJ: return MSAPI_AUD_ATV_EIAJ;
        case DEMO_AUDIO_CODEC_ATV_PAL: return MSAPI_AUD_ATV_PAL;
        case DEMO_AUDIO_CODEC_ATV_FM_RADIO: return MSAPI_AUD_ATV_FM_RADIO;
        case DEMO_AUDIO_CODEC_ATV_MAX: return MSAPI_AUD_ATV_MAX;

        //ATV ENC
        case DEMO_AUDIO_CODEC_ATVENC_NONE: return MSAPI_AUD_ATVENC_NONE;
        case DEMO_AUDIO_CODEC_ATVENC_FMTX: return MSAPI_AUD_ATVENC_FMTX;
        case DEMO_AUDIO_CODEC_ATVENC_BTSC: return MSAPI_AUD_ATVENC_BTSC;
        default: return MSAPI_AUD_DVB_INVALID;
    }
}

#if 0
static En_DVB_decSystemType _Demo_Audio_DspCodecTypeMap(AUDIO_DSP_CODE_TYPE eDspCodecType)
{
    switch(eDspCodecType)
    {
        case AU_DVB_STANDARD_MPEG: return MSAPI_AUD_DVB_MPEG;
        case AU_DVB_STANDARD_AC3: return MSAPI_AUD_DVB_AC3;
        case AU_DVB_STANDARD_AC3P: return MSAPI_AUD_DVB_AC3P;
        case AU_DVB_STANDARD_AAC: return MSAPI_AUD_DVB_AAC;
        case AU_DVB_STANDARD_MP3: return MSAPI_AUD_DVB_MP3;
        case AU_DVB_STANDARD_WMA: return MSAPI_AUD_DVB_WMA;
        case AU_DVB_STANDARD_RA8LBR: return MSAPI_AUD_DVB_RA8LBR;
        case AU_DVB_STANDARD_XPCM: return MSAPI_AUD_DVB_XPCM;
        case AU_DVB_STANDARD_DTS: return MSAPI_AUD_DVB_DTS;
        case AU_DVB_STANDARD_MS10_DDT: return MSAPI_AUD_DVB_AAC;
        case AU_DVB_STANDARD_MS10_DDC: return MSAPI_AUD_DVB_MS10_DDC;
        case AU_DVB_STANDARD_WMA_PRO: return MSAPI_AUD_DVB_WMA_PRO;
        case AU_DVB_STANDARD_FLAC: return MSAPI_AUD_DVB_FLAC;
        case AU_DVB_STANDARD_VORBIS: return MSAPI_AUD_DVB_VORBIS;
        case AU_DVB_STANDARD_DTSLBR: return MSAPI_AUD_DVB_DTSLBR;
        case AU_DVB_STANDARD_AMR_NB: return MSAPI_AUD_DVB_AMR_NB;
        case AU_DVB_STANDARD_AMR_WB: return MSAPI_AUD_DVB_AMR_WB;
        case AU_DVB_STANDARD_DRA: return MSAPI_AUD_DVB_DRA;
        default: return MSAPI_AUD_DVB_INVALID;

    }
}
#endif

#if (DEMO_AUDIO_MULTI_TEST==0)
static MS_U32 _Demo_Audio_Get_UsedResourceCount(void)
{
    MS_U32 u32index = 0, u32UsedResources=0;

    //check available audio resource
    for (u32index=0;u32index<AU_DEC_MAX;u32index++)
    {
        AudioDecStatus_t stTempDecStatus = {0};
        if (MApi_AUDIO_GetDecodeSystem(u32index, &stTempDecStatus))
        {
            if (stTempDecStatus.bConnect)
            {
                u32UsedResources++;
            }
        }
    }
    return u32UsedResources;
}
#endif

#if 1
static MS_BOOL _Demo_Audio_OpenDecSys(MS_U32 u32DevIdx, AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType, EN_AUDIO_APP_TYPE eAppType, En_DVB_decSystemType eCodecType)
{
    AUDIO_SOURCE_INFO_TYPE eSrcType=E_AUDIO_INFO_GAME_IN;
    ST_AUDIO_DEC_INFO eTemp_DEC_INFO = {0};

    if(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.bConnect == TRUE)
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") already opened!!!\n", __FUNCTION__, __LINE__, u32DevIdx);
        return TRUE;
    }

#if (DEMO_AUDIO_MULTI_TEST==0)
    MS_U32 u32Count = _Demo_Audio_Get_UsedResourceCount();
    if ( u32Count >= 1)
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") no available audio resource!!! count=%"DTC_MS_U32_u"\n", __FUNCTION__, __LINE__, u32DevIdx, u32Count);
        return FALSE;
    }
#endif

    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
        eSrcType = E_AUDIO_INFO_GAME_IN;
        break;
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
    case AUDIO_APP_DTV_AD:
    case AUDIO_APP_TSP_FRAMEDECODE:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
        //FIX ME, No Enum now. Mantis 1149420
        //eSrcType = E_AUDIO_INFO_ATV_ENC_IN;
        break;
#endif

#if (DEMO_XC_HDMIRX_TEST == 1)
    case AUDIO_APP_HDMI_RX:
        eSrcType = E_AUDIO_INFO_HDMI_IN;
        _bFirstIn = TRUE;
        break;
#endif

    default:
        eSrcType = E_AUDIO_INFO_GAME_IN;
    }

    //set status
    eTemp_DEC_INFO.stAudioDecStatus_t.bIsAD = FALSE;
    eTemp_DEC_INFO.stAudioDecStatus_t.eAudFormat  = eCodecType;
    eTemp_DEC_INFO.stAudioDecStatus_t.eSourceType = eSrcType;
    eTemp_DEC_INFO.stAudioDecStatus_t.eGroup = eOutSrcType;

#if (DEMO_AUDIO_SUPPORT_UTPA20_HEADER_TEST == 1)
    eTemp_DEC_INFO.stAudioDecStatus_t.eAudio_Force_ID = AUDIO_FORCE_INVALID;
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
    if(eAppType == AUDIO_APP_TSP_FRAMEDECODE)
    {
        eTemp_DEC_INFO.stAudioDecStatus_t.eMMType = AUDIO_MM_FRAME_DECODE;
    }
#endif

    //open decoder system
    eTemp_DEC_INFO.eDecID = MApi_AUDIO_OpenDecodeSystem(&eTemp_DEC_INFO.stAudioDecStatus_t);
    if(eTemp_DEC_INFO.eDecID == AU_DEC_INVALID)
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") no available resource!!!\n", __FUNCTION__, __LINE__, u32DevIdx);
        return FALSE;
    }

    stDualDemoInfo[u32DevIdx].eDecID = eTemp_DEC_INFO.eDecID;

    if (MApi_AUDIO_GetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDualDemoInfo[u32DevIdx].stAudioDecStatus_t) == FALSE)
    {
        printf("[%s][%d] Get decode system (%"DTC_MS_U32_u") fail!!!\n", __FUNCTION__, __LINE__, u32DevIdx);
        return FALSE;
    }

    //set default channel path, or set output source may be fail
    if (eOutSrcType == E_CONNECT_MAIN)
    {
        if (stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID1)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
        }
        else if (stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP3_DVB_INPUT, E_AUDIO_GROUP_MAIN);
        }
    }
    else
    {
        if (stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID1)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_SUB);
        }
        else if (stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
        {
            MApi_AUDIO_InputSwitch(AUDIO_DSP3_DVB_INPUT, E_AUDIO_GROUP_SUB);
        }
    }

    return TRUE;
}

static MS_BOOL _Demo_Audio_CloseDecSys(MS_U32 u32DevIdx)
{
    if((stDualDemoInfo[u32DevIdx].eDecID != AU_DEC_INVALID) && (stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.bConnect == TRUE))
    {
        MApi_AUDIO_ReleaseDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID);

        memset(&stDualDemoInfo[u32DevIdx].stAudioDecStatus_t, 0x00, sizeof(AudioDecStatus_t));
        stDualDemoInfo[u32DevIdx].eDecID = AU_DEC_INVALID;
    }
    else
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") already released!!!\n", __FUNCTION__, __LINE__, u32DevIdx);
    }
    return TRUE;
}
#endif

static MS_BOOL _Demo_Audio_SetSystem(MS_U32 u32DevIdx, En_DVB_decSystemType eCodecType)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        AudioDecStatus_t stDecStatus;
        MS_BOOL bret = FALSE;

        bret = MApi_AUDIO_GetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
        if (bret == FALSE)
        {
            printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        stDecStatus.eAudFormat = eCodecType;
        bret = MApi_AUDIO_SetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
        if (bret == FALSE)
        {
            printf("[%s][%d] MApi_AUDIO_SetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        return MApi_AUDIO_SetSystem(eCodecType);
    }

    return TRUE;
}

static MS_BOOL _Demo_Audio_STC_Switch(MS_U32 u32DevIdx, AUDIO_STC_SOURCE eSTC_Source)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        AudioDecStatus_t stDecStatus;
        MS_BOOL bret = FALSE;

        bret = MApi_AUDIO_GetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
        if (bret == FALSE)
        {
            printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        stDecStatus.eStcSource = eSTC_Source;
        bret = MApi_AUDIO_SetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
        if (bret == FALSE)
        {
            printf("[%s][%d] MApi_AUDIO_SetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    return TRUE;
}

static MS_BOOL _Demo_Audio_CheckAppSupport(EN_AUDIO_APP_TYPE eAppType)
{
    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
    case AUDIO_APP_PES_PLAY:
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
#if (DEMO_AUDIO_AD_TEST == 1)
    case AUDIO_APP_DTV_AD:
#endif
#if (DEMO_AUDIO_AD_1PID_TEST == 1)
    case AUDIO_APP_DTV_AD_1PID:
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
#endif
    case AUDIO_APP_HDMI_RX:
    case AUDIO_APP_TSP_FRAMEDECODE:
        return TRUE;
    default:
        return FALSE;
    }
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#if (DEMO_AUDIO_PCM_CAPTURE_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Start Audio transcoding data
/// @param[in] u32DevIdx device index
///            0: PCM Capture 1
///            1: PCM Capture 2
/// @param[in] enCaptureSource
///            main: E_CAPTURE_CH5
///            sub : E_CAPTURE_CH7 (depend on chip)
/// @param[in] u32channel
///            chnnel num (only support 2 ch)
/// @param[in] u32SampleRate
///            sample rate (only support 48000)
/// @param[in] u32OutputPath
///     0: FILE, output to a file
///     1: MEM, output to memory
/// @param[in] u32OutputFilename
/// @param[in] bOutFileAppend
///     0: file overwrite
///     1: file append
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_PCM_Capture_Start 0 0 /mnt/sda1/xxx.pcm 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_PCM_Capture_Start(MS_U32 *u32DevIdx, AUDIO_CAPTURE_SOURCE_TYPE* enCaptureSource, MS_U32* u32channel, MS_U32* u32SampleRate, EN_AUDIO_PCM_CAPTURE_OUTPUT_PATH* enOutputPath, MS_U32* u32OutputFilename, MS_BOOL* bOutFileAppend)
{
    printf("===============================================================================\n");
    printf("Audio_PCM_Capture_Start <$1> <$2> <$3> <$4> <$5> <$6> <$7>\n");
    printf("<$1>: device id      -- 1: PCM Capture 1, 2: PCM Capture 2\n");
    printf("<$2>: capture source -- 4: CH5 (main), 6: CH7(sub), other refer to Enum\n");
    printf("<$3>: channel number -- now only support \"2\"\n");
    printf("<$4>: sample rate    -- now only support \"48000\"\n");
    printf("<$5>: output type    -- 0: output to file, 1: output to memory\n");
    printf("<$6>: output path    -- user specific path + filename\n");
    printf("<$7>: file append    -- 0: No, 1: Yes\n");
    printf("ex: main -->   Audio_PCM_Capture_Start 0 4 2 48000 0 /mnt/sda1/test_main.pcm 0\n");
    printf("ex: sub -->    Audio_PCM_Capture_Start 1 6 2 48000 0 /mnt/sda1/test_sub.pcm 0\n");
    printf("===============================================================================\n");

    if (*u32DevIdx >= PCM_CAPTURE_NUMBER)
    {
        printf("Warning!!! Demo code not handle the device index(%"DTC_MS_U32_d")\n", *u32DevIdx);
        return FALSE;
    }

    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    if(_stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.iId == -1)
    {
        _stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.pStack = MsOS_AllocateMemory(_stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.u32StackSize, s32MstarCachedPoolID);
        if(!_stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }

        //Set DMA Reader parameters
        memset(_stAudioPCM_Capture_Task[*u32DevIdx].u8Filename, 0, sizeof(_stAudioPCM_Capture_Task[*u32DevIdx].u8Filename));
        memcpy(_stAudioPCM_Capture_Task[*u32DevIdx].u8Filename, (char*)u32OutputFilename, strlen((char*)u32OutputFilename));

        memset((void *)&_stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));

        _stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u32StructVersion = AUDIO_PCM_INFO_VERSION;
        _stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u32StructSize = sizeof(AUDIO_PCM_INFO_t);

        _stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u8CaptureFlag = TRUE;

        //[20180110][Notice] current chips only support 2 channel data, set other than 2 will open device fail
        _stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u32Channel = *u32channel;

        //[20180110][Notice] current chips only support 48000, set other than 48000 will open device fail
        _stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u32SampleRate = *u32SampleRate;

        _stAudioPCM_Capture_Task[*u32DevIdx].enOutputPath = *enOutputPath;
        _stAudioPCM_Capture_Task[*u32DevIdx].enCaptureSource = *enCaptureSource;

        _stAudioPCM_Capture_Task[*u32DevIdx].bOutFileAppend = *bOutFileAppend;

        //Set PCM IO Name
        if (*u32DevIdx == 0)
        {
            // Use AUDIO_PCM_CAPTURE1
            strcpy((char *)_stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_PCM_CAPTURE1);
        }
        else if (*u32DevIdx == 1)
        {
            // Use AUDIO_PCM_CAPTURE2
            strcpy((char *)_stAudioPCM_Capture_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_PCM_CAPTURE2);
        }
        else
        {
            printf("Warning!!! Demo code not handle the device index(%"DTC_MS_U32_d")\n", *u32DevIdx);
            return FALSE;
        }

        //Create PCM Capture task
        _stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.iId = MsOS_CreateTask((TaskEntry) _appAudio_PCM_Capture_Task,
                                            (MS_VIRT)(&_stAudioPCM_Capture_Task[*u32DevIdx]),
                                            _stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.ePriority,
                                            TRUE,
                                            _stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.pStack,
                                            _stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.u32StackSize,
                                            _stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.szName);

        if(_stAudioPCM_Capture_Task[*u32DevIdx].stTask_Info.iId < 0)
        {
            printf("create task fail \n");
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop Audio transcoding data
/// @param[in] u32EncIdx
///     0: PCM capture device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_PCM_Capture_Stop 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_PCM_Capture_Stop(MS_U32 *u32DevIdx)
{
    _stAudioPCM_Capture_Task[*u32DevIdx].enTaskStatus = AUDIO_MONITOR_TASK_STOP;
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code is used to enable/disable PUSI
///
/// @param[in] u32DevIdx device index
/// @param[in] u32enable 0: disable PUSI
///                      1: enable PUSI
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b Audio_PUSI \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_PUSI(MS_U32* u32DevIdx, MS_U32* u32enable)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        //disable PUSI flag
        if (MApi_AUDIO_SetAudioParam2(stDualDemoInfo[*u32DevIdx].eDecID, Audio_ParamType_PUSI_Enable, *u32enable) == FALSE)
        {
            printf("[%s][%d] dev_idx=%u, Set PUSI flag fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)(*u32DevIdx));
            return FALSE;
        }
    }
    else
    {
        if (*u32enable == 0)
        {
            //disable PUSI flag
            if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_PUSI_disable_Info, 1, 0) == FALSE)
            {
                printf("[%s][%d] dev_idx=%u, Set PUSI flag fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                return FALSE;
            }
        }
        else
        {
            //enable PUSI flag
            if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_PUSI_disable_Info, 0, 0) == FALSE)
            {
                printf("[%s][%d] dev_idx=%u, Set PUSI flag fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                return FALSE;
            }
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code is used to adjust sound effect delay
///
/// @param[in] u32Delayms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b AUdio_delay \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Delay(MS_U8* u8Delayms)
{
    MApi_AUDIO_SetBufferProcess(*u8Delayms);
    return TRUE;
}

#if(DEMO_AUDIO_SPDIF_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code is used to adjust SPDIF delay
///
/// @param[in] u32Delayms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b AUdio_SPDIF_delay \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SPDIF_Delay(MS_U32* u32Delayms)
{
    return MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_setSpdifDelay, (MS_VIRT)*u32Delayms, 0);
}
#endif

#if(DEMO_AUDIO_HDMI_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code is used to adjust HDMI delay
///
/// @param[in] u32Delayms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b AUdio_HDMI_delay \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_HDMI_Delay(MS_U32* u32Delayms)
{
    return MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_setHdmiDelay, (MS_VIRT)*u32Delayms, 0);
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code is used to adjust codec delay
///
/// @param[in] eCodecType 1: MPEG 2: AC3 3: AC3P 4: AAC
/// @param[in] u32Delayms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b Audio_codec_delay \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Codec_Delay(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U32* u32Delayms)
{
    MS_BOOL bret = FALSE;
    MS_U32 u32DelayValue = 90 * (*u32Delayms);
    switch (*eCodecType)
    {
        case DEMO_AUDIO_CODEC_MPEG:
            bret = MApi_AUDIO_SetMpegInfo(Audio_MPEG_DTV_AVSYNC_OFFSET, u32DelayValue, 0);
            break;
        case DEMO_AUDIO_CODEC_AC3:
        case DEMO_AUDIO_CODEC_AC3P:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_DTV_AVSYNC_OFFSET, u32DelayValue, 0);
            break;
        case DEMO_AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAACInfo(Audio_AAC_DTV_AVSYNC_OFFSET, u32DelayValue, 0);
            break;
        default:
            printf("Codec delay not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}

//------------------------------------------------------------------------------
/// @brief The sample code is used to adjust codec delay
///
/// @param[in] DecId Decoder ID
/// @param[in] eCodecType 1: MPEG 2: AC3 3: AC3P 4: AAC
/// @param[in] u32Delayms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b Audio_Codec_Delay_EX \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Codec_Delay_EX(AUDIO_DEC_ID* DecId, EN_AUDIO_CODEC_TYPE* eCodecType, MS_U32* u32Delayms)
{
    MS_BOOL bret = FALSE;

#if (DEMO_AUDIO_PARTIAL_CHIP_SUPPORT_FUNCTION_TEST == 1)
    MS_U32 u32DelayValue = 90 * (*u32Delayms);
    switch (*eCodecType)
    {
        case DEMO_AUDIO_CODEC_MPEG:
            bret = MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_MPEG_Decoder_Delay, u32DelayValue);
            break;
        case DEMO_AUDIO_CODEC_AC3:
        case DEMO_AUDIO_CODEC_AC3P:
            bret = MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_AC3_Decoder_Delay, u32DelayValue);
            break;
        case DEMO_AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_AAC_Decoder_Delay, u32DelayValue);
            break;
        case DEMO_AUDIO_CODEC_AC4:
            bret = MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_AC4_Decoder_Delay, u32DelayValue);
            break;
        default:
            printf("DecId(%d), codec(%d) codec delay not implement yet\n", *DecId, *eCodecType);
            break;
    }
#else
    printf("Codec delay disabled by DDI feature DEMO_AUDIO_PARTIAL_CHIP_SUPPORT_FUNCTION_TEST\n");
#endif

    return bret;
}


//------------------------------------------------------------------------------
/// @brief The sample code is used to set DTS HD bypass mode
///
/// @param[in] none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b Audio_DTSHD \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DTSHD(MS_U8* u8Mode)
{
    MS_BOOL bRet = FALSE;
    if (*u8Mode == 0)
    {
        bRet = MApi_AUDIO_SetDTSCommonCtrl(Audio_DTS_infoType_DTSHD_HDMITxBypass_Enable, Audio_DTS_Ctrl_DTSHDBypass_OFF);
    }
    else if (*u8Mode == 1)
    {
        bRet = MApi_AUDIO_SetDTSCommonCtrl(Audio_DTS_infoType_DTSHD_HDMITxBypass_Enable, Audio_DTS_Ctrl_DTSHDBypass_ON);
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code is used to check if enable/disable dynamic allocate
///        resource flow
/// @param[in] none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note na
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Dynamic_Allocate_Resource(void)
{
    return _bDynamicLoad;
}

//------------------------------------------------------------------------------
/// @brief The sample code is used to enable/disable dynamic allocate resource
///        flow
/// @param[in] u32Mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note need set before audio_init
/// Command: \b Audio_Dynamic_Alloc \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_Dynamic_Allocate_Resource(MS_U32* u32Mode)
{
    _bDynamicLoad = *u32Mode;
    return TRUE;
}

#if(DEMO_AUDIO_SE_DV258_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to enable/disable Dolby Volume 258
/// @param[in] u8Enable
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DV258 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DV258(MS_U8* u8Enable)
{
    MS_BOOL bret = FALSE;
    bret = MApi_AUDIO_ADVSOUND_SetParam(DV258_ENABLE, *u8Enable, 0);
    return bret;
}
#endif

#if(DEMO_AUDIO_AD_FADER_CONTROL_TEST == 1)
/// @brief The sample code demonstrate how to set AD mixer balance
/// @param[in] u8mode 32: BALANCE_EQUAL (main = ad)
///                   0: BALANCE_MAIN_0 (-32, main louder than ad, ad no sound)
///                   16: BALANCE_MAIN_25 (-16, main louder than ad)
///                   48: BALANCE_MAIN_75 (16, ad louder than main)
///                   64: BALANCE_MAIN_100 (32, ad louder than main, main no sound)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_AD_Fader \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_AD_Fader(MS_U8* u8mode)
{
    return MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_AD_Mixer_Balance, *u8mode, 0);
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set DRC scale value
/// @param[in] u32CutScale
/// @param[in] u32BoostScale
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DRC_Scale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Scale(MS_U32* u32CutScale, MS_U32* u32BoostScale)
{
    MS_BOOL bret = FALSE, bret1 = FALSE;
    bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_Drc_HighCutScale, *u32CutScale, 0);
    bret1 = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_Drc_LowBootScale, *u32BoostScale, 0);
    if (bret && bret1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set DRC scale value
/// @param[in] DecId Decoder ID
/// @param[in] u32CutScale
/// @param[in] u32BoostScale
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DRC_Scale_EX \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Scale_EX(AUDIO_DEC_ID* DecId, MS_U32* u32CutScale, MS_U32* u32BoostScale)
{
    MS_BOOL bret=FALSE, bret1=FALSE;

    bret = MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_DolbyDrc_HighCutScale,*u32CutScale);
    bret1 = MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_DolbyDrc_LowBootScale,*u32BoostScale);

    if (bret && bret1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set down mix mode
/// @param[in] eCodecType 2: AC3 3: AC3P 4: AAC
/// @param[in] mode 0: LtRt mode 1: LoRo mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_Downmix \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DownMix_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode)
{
    MS_BOOL bret = FALSE;
    switch (*eCodecType)
    {
        case DEMO_AUDIO_CODEC_AC3:
        case DEMO_AUDIO_CODEC_AC3P:
        case DEMO_AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DownmixMode, *mode, 0);
            break;
        default:
            bret = TRUE;
            printf("Down Mix mode not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set down mix mode
/// @param[in] DecId Decoder ID
/// @param[in] mode 0: LtRt mode 1: LoRo mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_Downmix_EX \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DownMix_Mode_EX(AUDIO_DEC_ID* DecId, MS_VIRT* mode)
{
    return MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_Downmix_Mode,*mode);
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set technical bulletin 11 mode (DRC mode)
/// @param[in] mode 0: disable 1: enable
/// @return none
/// @sa
/// @note
/// Command: \b Audio_TB11 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_TB11(MS_U8* mode)
{
    return MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , *mode, 0);
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set dyanmic range control mode (DRC mode)
/// @param[in] eCodecType 2: AC3 3: AC3P 4: AAC
/// @param[in] mode 0: LINE mode 1: RF mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_DRC \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode)
{
    MS_BOOL bret = FALSE;
    switch (*eCodecType)
    {
        case DEMO_AUDIO_CODEC_AC3:
        case DEMO_AUDIO_CODEC_AC3P:
        case DEMO_AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode, *mode, 0);
            break;
        default:
            bret = TRUE;
            printf("Down Mix mode not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set dyanmic range control mode (DRC mode)
/// @param[in] DecId Decoder ID
/// @param[in] mode 0: LINE mode 1: RF mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_DRC_EX \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Mode_EX(AUDIO_DEC_ID* DecId, MS_VIRT* mode)
{
    return MApi_AUDIO_SetAudioParam2(*DecId, Audio_ParamType_DolbyDrc_Mode,*mode);
}

#if (DEMO_AUDIO_FMTX_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode FMTX signal
/// @param[in] u8Filter 0: PAL 1: NTSC
/// @return none
/// @sa
/// @note
/// Command: \b Audio_FMTX_start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_FMTX_Start(MS_U8* u8Filter, MS_U16* u16Attenuation, MS_U16* u16OutScaling)
{
    ST_AUDIO_FMTX_PARAM stParam;

    stParam.u8Filter = *u8Filter;
    stParam.u16Attenuation = *u16Attenuation;
    stParam.u16OutScaling = *u16OutScaling;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        /* please be notice that FMTX can only be used for sub */
        _Demo_Audio_CloseDecSys(1);

        if(_Demo_Audio_OpenDecSys(1, E_CONNECT_SUB, AUDIO_APP_ATV_FMTX, MSAPI_AUD_DVB_MPEG) == FALSE)
        {
            printf("[%s][%d] open decode system (1) fail.\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        if (Demo_Audio_Play(1, AUDIO_APP_ATV_FMTX, DEMO_AUDIO_CODEC_ATVENC_FMTX, (void *)&stParam) == FALSE)
        {
            _Demo_Audio_CloseDecSys(1);
            return FALSE;
        }
    }
    else
    {
        return Demo_Audio_Play(0, AUDIO_APP_ATV_FMTX, DEMO_AUDIO_CODEC_ATVENC_FMTX, (void *)&stParam);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop FMTX
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_FMTX_stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_FMTX_Stop(void)
{
    return Demo_Audio_Stop(1, AUDIO_APP_ATV_FMTX);
}
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode BTSC signal
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_BTSC_start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_BTSC_Start(MS_U16* u16Attenuation, MS_U16* u16OutScaling, MS_U16* u16_M_Gain, MS_U16* u16_D_Gain, MS_U16* u16_SAP_Gain)
{
    ST_AUDIO_BTSC_PARAM stParam = {0};

    stParam.u16Attenuation = *u16Attenuation;
    stParam.u16OutScaling = *u16OutScaling;
    stParam.u16_M_Gain = *u16_M_Gain;
    stParam.u16_D_Gain = *u16_D_Gain;
    stParam.u16_SAP_Gain = *u16_SAP_Gain;

    return Demo_Audio_Play(0, AUDIO_APP_ATV_BTSC, DEMO_AUDIO_CODEC_ATVENC_BTSC, (void *)&stParam);
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop BTSC
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_ATV_stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_BTSC_Stop(void)
{
    return Demo_Audio_Stop(0, AUDIO_APP_ATV_BTSC);
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio
/// @param[in] vol 0~99(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAbsoluteVolume(MS_U8* vol)
{
    MS_U8 value1, value2;
    MS_U16 u16VolumeValue=0;
    MS_BOOL bMute = FALSE;

    MS_U16 u16VolumeValue_LR = 0;
    MS_S16 s16LR_Compensation = AUDIO_VOL_LR_COMPENSATION;

#if (DEMO_AUDIO_HDMI_TEST == 1)
    MS_U16 u16VolumeValue_HDMI = 0;
    MS_S16 s16HDMI_Compensation = AUDIO_VOL_HDMI_COMPENSATION;
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    MS_U16 u16VolumeValue_SPDIF = 0;
    MS_S16 s16SPDIF_Compensation = AUDIO_VOL_SPDIF_COMPENSATION;
#endif

    if( *vol > 100 )
    {
        *vol = 100;
    }

    if(*vol == 0)
    {
        bMute = TRUE;
        Demo_Audio_SetMute(&bMute);
    }
    else
    {
        bMute = FALSE;
        Demo_Audio_SetMute(&bMute);
    }

    printf("\n[%s][%d]########## audio volume = %d ##########\n\n", __FUNCTION__, __LINE__, *vol);
    u16VolumeValue = AudioVolumeTable[*vol];

    //Line-out (LR) vol + compensation
    u16VolumeValue_LR = u16VolumeValue+s16LR_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_LR > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_LR>>8);
        value2 = (MS_U8)((u16VolumeValue_LR&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_MAIN_SPEAKER, value1, value2);

#if (DEMO_AUDIO_HDMI_TEST == 1)
    //HDMI vol + compensation
    u16VolumeValue_HDMI = u16VolumeValue+s16HDMI_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_HDMI > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {

        value1 = (MS_U8)(u16VolumeValue_HDMI>>8);
        value2 = (MS_U8)((u16VolumeValue_HDMI&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_HDMI, value1, value2);
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    //SPDIF vol + compensation
    u16VolumeValue_SPDIF = u16VolumeValue+s16SPDIF_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_SPDIF > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_SPDIF>>8);
        value2 = (MS_U8)((u16VolumeValue_SPDIF&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_SPDIF, value1, value2);
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
    //BTSC output to ch34 via dac circuit, the compensation is equal to Line-out
    //vol + compensation
    u16VolumeValue_LR = u16VolumeValue + s16LR_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_LR > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_LR>>8);
        value2 = (MS_U8)((u16VolumeValue_LR&0x00FF)>>5);
    }

    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_BTSC1, value1, value2);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_BTSC2, value1, value2);

#endif
    return TRUE;
}

#if (DEMO_AUDIO_DMA_READER_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to do DMA Reader test
/// @param[in] pFilename input file name
/// @param[in] u8SampleRate file sampling rate
///            0: 48K
///            1: 44.1K
///            2: 32K
///            3: 24K
///            4: 22.05K
///            5: 16K
///            6: 12K
///            7: 11.025K
///            8: 8K
///            9: 4K
/// @param[in] u8RepeatTime repeat time
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_DMA_Test \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime)
{
    FILE *file = NULL;
    MS_U32 read_size = 0;
    MS_U32 file_size = 0;
    MS_S32 residual_size = 0;
    MS_U8* pbuf = NULL;
    MS_U32 buf_size = 4096;
    MS_U64 u64RingInbuf_size = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferSize);
    MS_U64 u64pcm_level = 0;
    MS_S8 u8loop_left = *u8RepeatTime;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_BOOL bResend = FALSE;

    if (u64RingInbuf_size == 0)
    {
        printf("[%s][%d] Ring buffer size is 0 !!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    pbuf = MsOS_AllocateMemory(buf_size, s32MstarNonCachedPoolID);
    if (pbuf == NULL)
    {
        printf("allocate memory fail!!!\n");
        return FALSE;
    }

    file = MsFS_Fopen((char*)pFilename, "rb");

    if (file == NULL)
    {
        printf("open file fail!!!\n");
        return FALSE;
    }

    MsFS_Fseek(file, 0, SEEK_END);
    file_size = MsFS_Ftell(file);           // Get total file size
    MsFS_Fseek(file, 0, SEEK_SET);

    // init DMA reader
    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);

    MApi_AUDIO_DmaReader_Init(*u8SampleRate);
    printf("==========> dma reader init, smp rate is 48k\n");

    // set DMA reader command start
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DMAReader_Command, 1, 0);

    while ((u8loop_left!=0) || (residual_size!=0))
    {
        if (residual_size == 0)
        {
            MsFS_Fseek(file, 0, SEEK_SET);     // Seek to file begin if no data can read
            residual_size = file_size;
            //u32index=0;
            u8loop_left--;
            printf("count = %d\n", u8loop_left);
        }

        read_size = (residual_size >= buf_size) ? buf_size : residual_size;

        // check dma_reader buffer level
        u64pcm_level = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferLevel);
        printf("pcm lvl = %"DTC_MS_U64_x"\n", u64pcm_level);

        // residual buffer size > read size, keep sending PCM data to DMA reader buffer
        if ((u64RingInbuf_size - u64pcm_level) >= read_size)
        {
            if (bResend == FALSE)
            {
                // read data from usb
                read_size = MsFS_Fread((void *)pbuf, 1, read_size, file);
            }

            // write data to buffer
            if(MApi_AUDIO_DmaReader_WritePCM((void*)(MS_PHYADDR)(MS_VA2PA((MS_U32)pbuf)), read_size) == FALSE)
            {
                printf("write PCM ng!\n");
                bResend = TRUE;
            }
            else
            {
                residual_size -= read_size;
                bResend = FALSE;
            }
        }

        MsOS_DelayTask(1);    // for 48K ring tone
    }

    MsFS_Fclose(file);

    MsOS_FreeMemory(pbuf, s32MstarNonCachedPoolID);
    return TRUE;
}
#endif

#if ((DEMO_AUDIO_DMA_READER_TEST == 1) && (DEMO_AUDIO_R2_MEM_ARCHI_TEST == 1))
//------------------------------------------------------------------------------
/// @brief The sample code to do DMA Reader test
/// @param[in] u32DevIdx device index
///            0: HW DMA Reader 1
///            1: HW DMA Reader 2
///            2: SW DMA Reader 1
///            3: R2 DMA Reader 1 (Need feed in multi channel PCM data)
///            4: R2 DMA Reader 2 (Need feed in multi channel PCM data)
/// @param[in] pFilename input file name
/// @param[in] u32ChannelNum PCM channel number
/// @param[in] u32SampleRate PCM sampling rate
/// @param[in] u32BitDepth PCM Bit Depth
/// @param[in] u32RepeatTime repeat time
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_DMA_EX_Test \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_DMA_Reader_EX_Test(MS_U32* u32DevIdx, MS_U8* pFilename, MS_U32* u32ChannelNum, MS_U32* u32SampleRate, MS_U32* u32BitWidth, MS_U32* u32RepeatLoop)
{
    if (*u32DevIdx >= DMA_READER_NUMBER)
    {
        printf("Warning!!! Demo code not handle the device index(%"DTC_MS_U32_d")\n", *u32DevIdx);
        return FALSE;
    }

    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    if(_stAudioDMAReader_Task[*u32DevIdx].stTask_Info.iId == -1)
    {
        _stAudioDMAReader_Task[*u32DevIdx].stTask_Info.pStack = MsOS_AllocateMemory(_stAudioDMAReader_Task[*u32DevIdx].stTask_Info.u32StackSize, s32MstarCachedPoolID);
        if(!_stAudioDMAReader_Task[*u32DevIdx].stTask_Info.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }

        //Set DMA Reader parameters
        memset(_stAudioDMAReader_Task[*u32DevIdx].u8Filename, 0, sizeof(_stAudioDMAReader_Task[*u32DevIdx].u8Filename));
        memcpy(_stAudioDMAReader_Task[*u32DevIdx].u8Filename, (char*)pFilename, strlen((char*)pFilename));

        memset((void *)&_stAudioDMAReader_Task[*u32DevIdx].tPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32StructVersion = AUDIO_PCM_INFO_VERSION;
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32StructSize = sizeof(AUDIO_PCM_INFO_t);

        //Set PCM IO Name
        if (*u32DevIdx == 0)
        {
            // Use HW_DMA_READER1
            strcpy((char *)_stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_HW_DMA_READER1);
        }
        else if (*u32DevIdx == 1)
        {
            // Use HW_DMA_READER2
            strcpy((char *)_stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_HW_DMA_READER2);
        }
        else if (*u32DevIdx == 2)
        {
            // Use SW_DMA_READER1
            strcpy((char *)_stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_SW_DMA_READER1);
        }
        else if (*u32DevIdx == 3)
        {
            // Use R2_DMA_READER1
            strcpy((char *)_stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_R2_DMA_READER1);
        }
        else if (*u32DevIdx == 4)
        {
            // Use R2_DMA_READER2
            strcpy((char *)_stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u8Name, AUDIO_R2_DMA_READER2);
        }
        else
        {
            printf("Warning!!! Demo code not handle the device index(%"DTC_MS_U32_d")\n", *u32DevIdx);
            return FALSE;
        }

        //Set buffer size (mini second unit). Suggest 100. Buffer size depend on chip's memory layout.
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32BufferDuration = 100;

        //PCM data channel num
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32Channel = *u32ChannelNum;

        //PCM data sample rate
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32SampleRate = *u32SampleRate;

        //PCM data bit width
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32BitWidth = *u32BitWidth;

        //Not support yet
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32Weighting = 100;

        //Not support yet
        _stAudioDMAReader_Task[*u32DevIdx].tPcmInfo.u32Volume = 100;

        //Set repeat loop
        _stAudioDMAReader_Task[*u32DevIdx].u32RepeatLoop = *u32RepeatLoop;

        //Create DMA reader task
        _stAudioDMAReader_Task[*u32DevIdx].stTask_Info.iId = MsOS_CreateTask((TaskEntry) _appAudio_DMA_Reader_EX_Task,
                                            (MS_VIRT)(&_stAudioDMAReader_Task[*u32DevIdx]),
                                            _stAudioDMAReader_Task[*u32DevIdx].stTask_Info.ePriority,
                                            TRUE,
                                            _stAudioDMAReader_Task[*u32DevIdx].stTask_Info.pStack,
                                            _stAudioDMAReader_Task[*u32DevIdx].stTask_Info.u32StackSize,
                                            _stAudioDMAReader_Task[*u32DevIdx].stTask_Info.szName);

        if(_stAudioDMAReader_Task[*u32DevIdx].stTask_Info.iId < 0)
        {
            printf("create task fail \n");
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;

}
#endif  //#if (DEMO_AUDIO_DMA_READER_TEST == 1)

//------------------------------------------------------------------------------
/// @brief The sample code to enable audio monitor
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Monitor \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMonitor(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    if(_audioMonitor_Task.iId == -1)
    {
        _audioMonitor_Task.pStack = MsOS_AllocateMemory(_audioMonitor_Task.u32StackSize, s32MstarCachedPoolID);
        if(!_audioMonitor_Task.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }
        _audioMonitor_Task.iId = MsOS_CreateTask( _appAudioMonitor_task,
                                            0,
                                            _audioMonitor_Task.ePriority,
                                            TRUE,
                                            _audioMonitor_Task.pStack,
                                            _audioMonitor_Task.u32StackSize,
                                            _audioMonitor_Task.szName);

        if(_audioMonitor_Task.iId < 0)
        {
            printf("create task fail \n");
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use IR control audio function
/// @param pu32Enable      \b : Enable IR control
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_IR \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_IR_Debug(MS_U32* pu32Enable)
{
    if (*pu32Enable)
    {
        //set audio key handler
        Demo_Input_Set_Handlekey_Func_Ptr(_Demo_Audio_IR_Key_Handler);
        //enable
#if defined(MSOS_TYPE_ECOS)
        Demo_Input_Init_ecos();
#elif defined(MSOS_TYPE_LINUX)
        Demo_Input_Init_linux();
#endif
    }
    else
    {
        //disable
#if defined(MSOS_TYPE_ECOS)
        Demo_Input_Exit_ecos();
#elif defined(MSOS_TYPE_LINUX)
        Demo_Input_Exit_linux();
#endif
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set register value
/// @param pu32Bank      \b : bank address
/// @param pu32Offset    \b : register offset (8-bit)
/// @param pu32RegValue  \b : value
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_WReg \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Debug_WriteReg(MS_U32* pu32Bank, MS_U32* pu32Offset, MS_U32* pu32RegValue)
{
#if defined(MSOS_TYPE_ECOS)
    *(volatile MS_U16*)(0xBF000000+((2*(*pu32Bank))<<8)+((*pu32Offset)<<1)) = *pu32RegValue;

#elif defined(MSOS_TYPE_LINUX)
    MS_PHY phyBankSize;

    MS_VIRT virt32NonPmBase = 0;
    MS_VIRT virt32PmBase = 0;
    if( !MDrv_MMIO_GetBASE(&virt32NonPmBase, &phyBankSize, MS_MODULE_HW))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_HW)!\n");
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&virt32PmBase, &phyBankSize, MS_MODULE_PM))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_PM)!\n");
        return FALSE;
    }

    if(*pu32Bank >= 0x1000)
    {
        REG16_NPM(((*pu32Bank-0x1000)<<9) + ((*pu32Offset)<<1)) = *pu32RegValue;
    }
    else
    {
        REG16_PM((*pu32Bank<<9) + ((*pu32Offset)<<1)) = *pu32RegValue;
    }
#endif
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to set audio monitor debug level and enable monitor task
/// @param u8Level      \b : debug level
///                            0: Audio_DumpInfo_Off          = 0x0,
///                            1: Audio_DumpInfo_Avsync       = 0x1,
///                            2: Audio_DumpInfo_Dec1         = 0x2,
///                            4: Audio_DumpInfo_IoInfo       = 0x4,
///                            8: Audio_DumpInfo_mmFileAVsync = 0x8,
///                           21: Audio_DumpInfo_All          = 0x15,
///                           25: Audio_DumpInfo_testSTC      = 0x19,
///                          128: Audio_DumpInfo_AutoTest     = 0x80,
/// @param u16Interval  \b : print message interval
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Debug \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval)
{
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Dump_Dsp_Info, *u8Level, *u16Interval);
    if(*u8Level != 0)
    {
        Demo_Audio_SetMonitor();
    }
    else
    {
        _bExit_Monitor_Task = TRUE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to print audio R2 debug log.
///        R2 command can refer to Audio survival guide in one_piece.
/// @param u32R2_CMD      \b : R2 command
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Debug_R2 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Debug_R2(MS_U32* u32R2_CMD)
{
#if defined(AUDIO_R2_BANK) && defined(AUDIO_R2_CMD_OFFSET) && defined(AUDIO_R2_CMD_OFFSET) \
    && defined(UART_BANK) && defined(UART_SOURCE_OFFSET) \
    && defined(UART_SOURCE_SETTING_MCU_TO_AUDIO_R2) && defined(UART_SOURCE_SETTING_AUDIO_R2_TO_MCU)

    MS_U32 u32R2bank = AUDIO_R2_BANK;
    MS_U32 u32R2offset = AUDIO_R2_CMD_OFFSET;
    MS_U32 u32DisableWFIoffset = AUDIO_R2_SYSTEM_DBG_OFFSET;
    MS_U32 u32UARTbank = UART_BANK;
    MS_U32 u32UARToffset = UART_SOURCE_OFFSET;
    MS_U32 u32DsiableWFIvalue = 0x800;
    MS_U32 u32UART_MCU_to_R2 = UART_SOURCE_SETTING_MCU_TO_AUDIO_R2;

    //issue R2 command
    Demo_Audio_Debug_WriteReg(&u32R2bank, &u32R2offset, u32R2_CMD);
    Demo_Audio_Debug_WriteReg(&u32R2bank, &u32DisableWFIoffset, &u32DsiableWFIvalue);

    //switch UART source to R2
    Demo_Audio_Debug_WriteReg(&u32UARTbank, &u32UARToffset, &u32UART_MCU_to_R2);

#else
    printf("Warning!!! R2 debug register not set properly, check SysBsp.h\n");
#endif
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set output source for main/sub path.
/// @param u8HDMI     \b : 0: main
///                        1: sub
/// @param u8LineOut  \b : 0: main
///                        1: sub
/// @param u8SPDIF    \b : 0: main
///                        1: sub
/// @param u8SCART    \b : 0: main
///                        1: sub
/// @param u8I2S      \b : 0: main
///                        1: sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// Command: \b Audio_OutSource_Config \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_Output_Source_Config(MS_U8* u8HDMI, MS_U8* u8LineOut, MS_U8* u8SPDIF, MS_U8* u8SCART, MS_U8* u8I2S)
{
    _Output_SourceInfo.HDMIOut= *u8HDMI;
    _Output_SourceInfo.SpeakerOut = *u8LineOut;
    _Output_SourceInfo.SpdifOut = *u8SPDIF;
    _Output_SourceInfo.ScartOut = *u8SCART;
    _Output_SourceInfo.HpOut = *u8I2S;

    //Output source config (output to device)
    MApi_AUDIO_SetOutputSourceInfo(&_Output_SourceInfo);

    //Execute Output Path connection
    MApi_AUDIO_SetOutConnectivity();

    return TRUE;
}

#if (DEMO_AUDIO_HDMI_TEST == 1)

////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set HDMI Tx mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - 0x0 : HDMIT Tx, PCM mode
///                - 0x1 : HDMIT Tx, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: constraints on HDMI non PCM mode.
///        If set HDMI to non pcm mode, you need do the following actions. (For more
///        detail steps, please refer to the test plan.)
/// ================================================================================
///           |         48K          |        44.1K         |         32K          |
/// ================================================================================
///  Kronus   | script:              | script:              | script:              |
///  Kaiserin | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///  Kaiser   |                      | Audio_SPDIF_Mode 2   | Audio_SPDIF_Mode 2   |
///           |                      |                      |                      |
///           | (HDMI, SPDIF can be  | (If set HDMI to non  | (If set HDMI to non  |
///           | different mode)      | PCM mode, need set   | PCM mode, need set   |
///           |                      | SPDIF to non PCM     | SPDIF to non PCM     |
///           |                      | mode)                | mode)                |
/// ================================================================================
///  Kappa    | script:              | script:              | script:              |
///  Keltic   | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///  Kenya    |                      |                      |                      |
///  Kriti    | (If set HDMI to non  | (If set HDMI to non  | (If set HDMI to non  |
///           | PCM mode, SPDIF will | PCM mode, SPDIF will | PCM mode, SPDIF will |
///           | change to non PCM)   | change to non PCM)   | change to non PCM)   |
/// ================================================================================
///  Keres    | script:              | script:              | script:              |
///  Clippers | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///           |                      |                      |                      |
///           | (HDMI, SPDIF can be  | (HDMI, SPDIF can be  | (HDMI, SPDIF can be  |
///           | different mode)      | different mode)      | different mode)      |
/// ================================================================================
///
/// Command: \b Audio_HDMITx_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_HDMITx_Mode(MS_U8* u8Mode)
{
    if (u8Mode == NULL)
    {
        return FALSE;
    }

    /////////////////////////////////////////////////////////////////
    /// Steps for implement a switch HDMI_Tx pcm/nonpcm function
    /////////////////////////////////////////////////////////////////

    //step2: set pcm/nonpcm mode
    MApi_AUDIO_HDMI_TX_SetMode(*u8Mode);

    //step3: add a HDMI_Tx status monitor in the monitor loop,
    //       please refer to the _appDemo_HDMI_TX_Status_Monitor() in _appAudioMonitor_task()

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code is used to set HDMI DDP bypass.
/// @param u8Mode  0: bypass disable
///                1: bypass enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: Two prerequisites brfore enable HDMI DDP bypass
///        (1) Need set HDMI non PCM first
///        (2) Need create a task to monitor _appDemo_HDMI_TX_Status_Monitor(), this is used to set correct freq and type to HDMI module.
///
/// Command: \b Audio_DDP \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_DDP(MS_VIRT* pmode)
{

    if (pmode == NULL)
    {
        return FALSE;
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
    if(_bFMTX_enable || _bBTSC_enable || _bPCM_CAPTURE_enable || _bDSP2_Decode)
    {
        printf("\nWarning!!! DDP bypass NOT allowed due to ATV, PCM capture or SUB AV is used!!!\n\n");
        return FALSE;
    }
    else
#endif
    {
        if (MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, *pmode, 0) == TRUE)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}
#endif

#if (DEMO_XC_HDMIRX_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code is used to set EDID output interface.
///
/// @param[in] DecId Decoder ID
/// @param pmode  0: SPDIF
///               1: HDMI ARC
///               2: HDMI
///               3: I2S
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
///
/// Command: \b Audio_EDID_Output_EX \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_EDID_Output_EX(AUDIO_DEC_ID* enDecId, MS_VIRT* pmode)
{
    MS_BOOL bRet = FALSE;
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        bRet = MApi_AUDIO_SetAudioParam2(*enDecId, Audio_ParamType_SetEDIDOutputInterface, *pmode);
    }
    else
    {
        bRet = MApi_AUDIO_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_SetEDIDOutputInterface, *pmode);
    }
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code is used to set EDID output interface.
/// @param pmode  0: SPDIF
///               1: HDMI ARC
///               2: HDMI
///               3: I2S
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
///
/// Command: \b Audio_EDID_Output \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_EDID_Output(MS_U32* u32DevIdx, MS_VIRT* pmode)
{
    MS_BOOL bRet = FALSE;
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        bRet = MApi_AUDIO_SetAudioParam2(stDualDemoInfo[*u32DevIdx].eDecID, Audio_ParamType_SetEDIDOutputInterface, *pmode);
    }
    else
    {
        bRet = MApi_AUDIO_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_SetEDIDOutputInterface, *pmode);
    }
    return bRet;
}
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code is used to show EDID infomation.
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
///
/// Command: \b Audio_EDID \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Show_HDMITx_EDID(void)
{
    //Check if HDMI Tx connect or not
    if (MApi_HDMITx_GetRxStatus() == FALSE)
    {
        //if HDMI Tx not connected
        printf("Warning!! HDMI Tx not connected\n");
        return TRUE;
    }
    else
    {
        //if HDMI Tx connected
        MS_U8 u8idx = 0;
        MS_U8 u8RealBlockLength = 0;
        MS_U8* pu8Buffer = NULL;
        MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

        //get non cache buffer ID
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

        //check audio EDID block length
        if (MApi_HDMITx_GetDataBlockLengthFromEDID(&u8RealBlockLength, E_CEA_TAG_CODE_AUDIO) == FALSE)
        {
            printf("Warning!! No EDID data, Data Block Length = %d\n", u8RealBlockLength);
            return TRUE;
        }

        pu8Buffer = (MS_U8*)MsOS_AllocateMemory(u8RealBlockLength, s32MstarNonCachedPoolID);
        if (pu8Buffer == NULL)
        {
            printf("Error!!! MsOS_AllocateMemory fail!!! size=%d\n", u8RealBlockLength);
            return FALSE;
        }

        //get Audio EDID from downstream device
        if (MApi_HDMITx_GetRxAudioFormatFromEDID(pu8Buffer, u8RealBlockLength) == FALSE)
        {
            printf("Error!!! Get EDID fail!!!\n");
            MsOS_FreeMemory(pu8Buffer, s32MstarNonCachedPoolID);
            return FALSE;
        }

        //store EDID to a local static structure
        if (_Demo_Audio_HDMI_TX_Update_EDID(pu8Buffer, u8RealBlockLength) == FALSE)
        {
            printf("Error!!! Store EDID fail!!!\n");
            MsOS_FreeMemory(pu8Buffer, s32MstarNonCachedPoolID);
            return FALSE;
        }

        //show EDID
        _Demo_Audio_HDMI_TX_Show_EDID();

        //show raw data
        printf("Raw EDID data:");
        for (u8idx=0;u8idx<u8RealBlockLength;u8idx++)
        {
            printf(" %02x", pu8Buffer[u8idx]);
        }
        printf("\n\n");

        MsOS_FreeMemory(pu8Buffer, s32MstarNonCachedPoolID);
    }

    return TRUE;
}

MS_BOOL Demo_Audio_HDMITx_EDID_Check_Support_Codec(En_DVB_decSystemType enCurCodec)
{
    switch (enCurCodec)
    {
        case MSAPI_AUD_DVB_MPEG:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_MPEG1) || IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_MPEG2))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_AC3:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_AC3))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_AC3P:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_AC3P))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_AAC:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_AAC))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_WMA_PRO:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_WMAPRO))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_DTS:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_DTS))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_DTSHDADO:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_DTS_HD))
            {
                return TRUE;
            }
            break;
        case MSAPI_AUD_DVB_DolbyTrueHDBypass:
            if(IS_SUPPORT_EDID(DEMO_AUDIO_EDID_CODEC_MLP_DOLBY_TRUEHD))
            {
                return TRUE;
            }
            break;
        default:
            printf("[%s][%d] No mapping EDID codec, MSTAR codec(0x%x)!!!\n", __FUNCTION__, __LINE__, enCurCodec);
            return FALSE;
    }

    printf("[%s][%d] Downstream EDID not support, MSTAR codec(0x%x)!!!\n", __FUNCTION__, __LINE__, enCurCodec);

    return FALSE;
}

#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)

////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set S/PDIF mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: Compare with HDMI, no additional actions needs for SPDIF non PCM mode.
///        (For more detail steps, please refer to the test plan.)
///
/// Command: \b Audio_SPDIF_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_SPDIF_Mode(MS_U8* u8Mode)
{
    MApi_AUDIO_SPDIF_SetMode(*u8Mode);
    return TRUE;
}

#endif

#if (DEMO_AUDIO_SOUND_EFFECT_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code is used to enable Balance function.
/// @param u32Enable  - 0: disable
///                   - not 0: enable
/// @return TRUE: Process success.
/// @sa
/// @note
/// Command: \b Audio_Balance_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetBalance_Enable(MS_U32* u32Enable)
{
    if (*u32Enable > 1)
    {
        *u32Enable = 1;
    }
    MApi_AUDIO_EnableBalance(*u32Enable);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the balance value
/// @param u8PreScale  \b : range from 0 ~ 100
///                        - 0   :  R channel is muted
///                        - 50  :  L & R channel output the same level
///                        - 100 :  L channel is muted
/// @return TRUE: Process success.
/// @sa
/// @note
/// Command: \b Audio_Balance_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetBalance_Param(MS_U32* u32Balance)
{
    MApi_AUDIO_SetBalance(*u32Balance);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the prescale value
/// @param u8PreScale  \b : range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                       - 0x00: disable pre-scale
///                       - 0x6F: gain = 0db
///                       - 0xFF: gain = +18db
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_PreScale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetPreScale(MS_U8* u8PreScale)
{

    MS_U32 p_param[4] = {0};
    p_param[0] = AUDIO_PATH_MAIN;
    p_param[1] = *u8PreScale;

    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetPreScale, p_param);

/*
    MApi_AUDIO_SetPreScale(AUDIO_PATH_MAIN, *u8PreScale);
*/

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the bass value
/// @param u8Bass      \b :Bass Level (0~100) mapping to -16 ~ +15 dB Gain
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Bass_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetBass_Param(MS_U8* u8Bass)
{
/*
    MApi_AUDIO_SetBass(*u8Bass);
    MApi_AUDIO_EnableTone(TRUE);
*/
    MS_U32 p_param[4] = {0};
    p_param[0] = *u8Bass;
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetBass, p_param);
    //MApi_SND_ProcessEnable(Sound_ENABL_Type_Tone, TRUE);


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the treble value
/// @param u8Treble      \b :Bass Level (0~100) mapping to -16 ~ +15 dB Gain
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Treble_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetTreble_Param(MS_U8* u8Treble)
{
/*
    MApi_AUDIO_SetTreble(*u8Treble);
    MApi_AUDIO_EnableTone(TRUE);
*/
    MS_U32 p_param[4] = {0};
    p_param[0] = *u8Treble;
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetTreble, p_param);

    //MApi_SND_ProcessEnable(Sound_ENABL_Type_Tone, TRUE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the bass/treble function
/// @param bEnable      \b : enable/disable the bass/treble
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Bass_Treble_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetBassTreble_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_Tone, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable auto volume control
/// @param <IN>        \b u8AvcMode    :  AVC u8Mode
///                                    - 0: L mode
///                                    - 1: S mode
///                                    - 2: M mode
/// @param Level       \b :  AVC threshold level
///                       - 0x00--  0    dBFS
///                       - 0x01--  -0.5    dBFS
///                       - 0x20--  -16    dBFS
///                       - 0x50--  -40    dBFS
/// @param AvcAT       \b :  AVC attack time
///                       - AvcAT = 0 --> 20  ms
///                       - AvcAT = 1 --> 100 ms
///                       - AvcAT = 2 --> 200 ms
///                       - AvcAT = 3 --> 1 sec
/// @param AvcRT       \b :  AVC release time
///                       - AvcRT = 0 --> 1 sec
///                       - AvcRT = 1 --> 2 sec
///                       - AvcRT = 2 --> 100 ms
///                       - AvcRT = 3 --> 200 ms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_AVC_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAVC_Param(MS_U8* AvcMode, MS_U8* Level, MS_U8* AvcAT, MS_U8* AvcRT)
{

/*
    MApi_AUDIO_SetAvcMode( *AvcMode );
    MApi_AUDIO_SetAvcThreshold(*Level);
    MApi_AUDIO_SetAvcAT( *AvcAT );
    MApi_AUDIO_SetAvcRT( *AvcRT );
    MApi_AUDIO_EnableAutoVolume(*bEnAVC);
*/
    MS_U32 p_param[4] = {0};
    p_param[0] = *Level;
    p_param[1] = *AvcMode;
    p_param[2] = *AvcAT;
    p_param[3] = *AvcRT;
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetAvc, p_param);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the AVC function
/// @param bEnable      \b : enable/disable the AVC function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_AVC_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAVC_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_AutoVolume, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable parameter equalizer
/// @param <IN>         \b Band: 0~4
///                     \b Gain: 0~240, -12db ~ +12 db
///                     \b Foh: 1~160
///                     \b Fol: 0~99
///                     \b QValue: 5~160
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_PEQ_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetPEQ_Param(MS_U8* Band, MS_U8* Gain, MS_U16* Fc, MS_U8* QValue)
{
    double coef;
    double v0, dem;
    double G, fc = *Fc, Q;

    double k;
    double kpow2;
    AUDIO_PEQ_COEF PEQCoef;
    MS_BOOL error_config = FALSE;

    if(*Gain > 240)
    {
        printf("%s err! PEQ Gain should be 0~240\n", __FUNCTION__);
        error_config = TRUE;
    }

    if(*Fc<50)
    {
        printf("%s err! PEQ fc should be > 50\n", __FUNCTION__);
        error_config = TRUE;
    }

/*
    if(*Fol > 99)
    {
        printf("%s err! PEQ Fol should be 0~99\n", __FUNCTION__);
        error_config = TRUE;
    }

    if((*Foh== 0) && (*Fol < 50))
    {
         printf("%s err! PEQ Fo should be > 50\n", __FUNCTION__);
        error_config = TRUE;
    }
*/

    if((*QValue < 5) || (*QValue > 160))
    {
        printf("%s err! PEQ QValue should be 5~160\n", __FUNCTION__);
        error_config = TRUE;
    }

    if(error_config == TRUE)
    {
        return FALSE;
    }

    PEQCoef.band = *Band;
    //fc = (float)(((int)*Foh) * 100 + (int)*Fol);

    for(PEQCoef.sfs = 0; PEQCoef.sfs <= 1; PEQCoef.sfs++)
    {
        if(PEQCoef.sfs)
        {
            //AUD_DEBUG(printf("48k: %x\r\n",1));
            k = tan((3.1415926 * fc) / 48000);
        }
        else
        {
            //AUD_DEBUG(printf("32k: %x\r\n",0));
            k = tan((3.1415926 * fc) / 32000);
        }

        Q = ((double)*QValue) / 10;

        if(*Gain >= 120)
        {
            G = ((double)(*Gain - 120)) / 10;
        }
        else
        {
            G = ((double)(120 - *Gain)) / 10;
        }

        //fc = (float)(((int)*Foh) * 100 + (int)*Fol);

        kpow2 = k * k;

        v0 = pow(10.0, (G / 20.0));

        if(*Gain >= 120)
        {
            //Peak,boost
            dem = 1 + k / Q + kpow2;
            coef = (1 + v0 * k / Q + kpow2) / dem;
            PEQCoef.a0 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.a1 = (long)(coef * 4194304);
            coef = (1 - v0 * k / Q + kpow2) / dem;
            PEQCoef.a2 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.b1 = (long)(-coef * 4194304);
            coef = (1 - k / Q + kpow2) / dem;
            PEQCoef.b2 = (long)(-coef * 4194304);
        }
        else
        {
            //peak,cut
            dem = 1 + v0 * k / Q + kpow2;
            coef = (1 + k / Q + kpow2) / dem;
            PEQCoef.a0 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.a1 = (long)(coef * 4194304);
            coef = (1 - k / Q + kpow2) / dem;
            PEQCoef.a2 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.b1 = (long)(-coef * 4194304);
            coef = (1 - v0 * k / Q + kpow2) / dem;
            PEQCoef.b2 = (long)(-coef * 4194304);
        }

        MApi_SND_ProcessEnable(Sound_ENABL_Type_PEQ, FALSE);

        if(PEQCoef.sfs)
        {
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_A0, (PEQCoef.band << 8) + ((PEQCoef.a0 >> 16) & 0xFF), (PEQCoef.a0 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_A1, (PEQCoef.band << 8) + ((PEQCoef.a1 >> 16) & 0xFF), (PEQCoef.a1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_A2, (PEQCoef.band << 8) + ((PEQCoef.a2 >> 16) & 0xFF), (PEQCoef.a2 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_B1, (PEQCoef.band << 8) + ((PEQCoef.b1 >> 16) & 0xFF), (PEQCoef.b1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_B2, (PEQCoef.band << 8) + ((PEQCoef.b2 >> 16) & 0xFF), (PEQCoef.b2 & 0xFFFF));
        }
        else
        {
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_A0, (PEQCoef.band << 8) + ((PEQCoef.a0 >> 16) & 0xFF), (PEQCoef.a0 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_A1, (PEQCoef.band << 8) + ((PEQCoef.a1 >> 16) & 0xFF), (PEQCoef.a1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_A2, (PEQCoef.band << 8) + ((PEQCoef.a2 >> 16) & 0xFF), (PEQCoef.a2 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_B1, (PEQCoef.band << 8) + ((PEQCoef.b1 >> 16) & 0xFF), (PEQCoef.b1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_B2, (PEQCoef.band << 8) + ((PEQCoef.b2 >> 16) & 0xFF), (PEQCoef.b2 & 0xFFFF));
        }

    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the PEQ function
/// @param bEnable      \b : enable/disable the PEQ function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_PEQ_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetPEQ_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_PEQ, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable graphic Equalizer
/// @param u8band      \b :  EQ band 0~4
/// @param u8level     \b :  Absolute EQ register value  ;
///                       - 0011-0000: +12.00db. (Max)
///                       - 0000-0001: +0.25db.
///                       - 0000-0000: +0.00db.
///                       - 1111-1111: -0.25db.
///                       - 1101-0000: -12.00db. (Min)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_GEQ_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetGEQ_Param(MS_U8* u8band, MS_U8* u8level)
{
/*
    MApi_AUDIO_SetEq(*u8band, *u8level);
*/

    MS_U32 p_param[4] = {0};
    p_param[0] = *u8band;
    p_param[1] = *u8level;
    return MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetEq, p_param);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the GEQ function
/// @param bEnable      \b : enable/disable the GEQ function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_GEQ_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetGEQ_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_EQ, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable High Pass Filter
/// @param bEnable       \b : TRUE --Enable HPF;
///                           FALSE--Disable HPF
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_HPF_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetHPF_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_Hpf, *bEnable);
}

#endif

#if (DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to enable TSHD.
///                        TSHD contains 4 moudle:
///                        (1) TruSurround
///                        (2) TruBass
///                        (3) Definition
///                        (4) Dialog Clarity
///
/// @param u8SpeakerSize \b : 0:40Hz
///                           1:60Hz
///                           2:100Hz
///                           3:150Hz
/// @param u8Bass        \b : level 0 ~ 20
/// @param u8Definition  \b : level 0 ~ 20
/// @param u8Surround    \b : level 0 ~ 20
/// @param u8InputGain   \b : level 0 ~ 20
/// @param u8OutputGain  \b : level 0 ~ 16
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
///
/// Command: \b Audio_SetTSHD \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_TSHD_Param(MS_U8* u8SpeakerSize, MS_U8* u8Bass, MS_U8* u8Definition, MS_U8* u8Surround, MS_U8* u8InputGain, MS_U8* u8OutputGain)
{

    if(*u8SpeakerSize>3)
    {
        printf("Invalid param1, Speaker Size must be 0~3, 0:40Hz 1:60Hz 2:100Hz 3:150Hz\n");
        return FALSE;
    }
    else if(*u8Bass>20)
    {
        printf("Invalid param2, TruBass level must be 0~20\n");
        return FALSE;
    }
    else if(*u8Definition>20)
    {
        printf("Invalid param3, definition level must be 0~20\n");
        return FALSE;
    }
    else if(*u8Surround>20)
    {
        printf("Invalid param4, Surround level must be 0~20\n");
        return FALSE;
    }
    else if(*u8InputGain>20)
    {
        printf("Invalid param5, Input Gain must be 0~20\n");
        return FALSE;
    }
    else if(*u8OutputGain>16)
    {
        printf("Invalid param6, Output Gain must be 0~16\n");
        return FALSE;
    }
    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SPEAKERSIZE, MS_U16 u16value1, 0);        //u16value1:0~3 0:40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SPEAKERSIZE, *u8SpeakerSize, 0);
    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_TRUBASS_CONTROL, MS_U16 u16value1, 0);    //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_TRUBASS_CONTROL, *u8Bass, 0);

    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_DEFINITION_CONTROL, MS_U16 u16value1, 0); //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_DEFINITION_CONTROL, *u8Definition, 0);

    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SURROUND_LEVEL, MS_U16 u16value1, 0);     //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SURROUND_LEVEL, *u8Surround, 0);

    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_INPUT_GAIN, MS_U16 u16value1, 0);         //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_INPUT_GAIN, *u8InputGain, 0);
    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_OUTPUT_GAIN, MS_U16 u16value1, 0);        //u16value1:0~16
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_OUTPUT_GAIN, *u8OutputGain, 0);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the TSHD function
/// @param bEnable      \b : enable/disable the TSHD function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_TSHD_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_TSHD_Enable(MS_BOOL* bEnable)
{
    ADVSND_TYPE bSRS_Type = ADV_NONE;
    if(*bEnable)
    {
        bSRS_Type = SRS_TSHD;
    }
    else
    {
        bSRS_Type = ADV_NONE;
    }

    if (MApi_AUDIO_ADVSOUND_ProcessEnable(bSRS_Type) == FALSE)
    {
        printf("SRS_TSHD Enable/Disable process fail\n");
        return FALSE;
    }
    else if(MApi_AUDIO_ADVSOUND_SubProcessEnable(SRS_TSHD_TRUBASS, *bEnable) == FALSE)
    {
        printf("SRS_TSHD_TRUBASS Enable/Disable process fail\n");
        return FALSE;
    }
    else if(MApi_AUDIO_ADVSOUND_SubProcessEnable(SRS_TSHD_DC, *bEnable) == FALSE)
    {
        printf("SRS_TSHD_DC Enable/Disable process fail\n");
        return FALSE;
    }
    else if(MApi_AUDIO_ADVSOUND_SubProcessEnable(SRS_TSHD_DEFINITION, *bEnable) == FALSE)
    {
        printf("SRS_TSHD_DEFINITION Enable/Disable process fail\n");
        return FALSE;
    }

    return TRUE;

}

#endif

#if (DEMO_AUDIO_AD_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio description
/// @param[in] vol 0~100(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume_ad \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetADAbsoluteVolume(MS_U8* vol)
{
    MS_U16 u16VolumeValue = 0;
    MS_U8 value1 = 0;
    u16VolumeValue = AudioVolumeTable[*vol];
    value1 = (MS_U8)(u16VolumeValue >> 8);
    MApi_AUDIO_SetADAbsoluteVolume(value1);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute_ad \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_ADSetMute(MS_BOOL* bEnable)
{
    //mute AD
    MApi_AUDIO_ADSetMute(*bEnable);

    return TRUE;
}
#endif


//------------------------------------------------------------------------------
/// @brief The sample code to set audio decoder output mode
/// @param mode
///     0:STEREO     (AU_DVB_MODE_STEREO)
///     1: LL        (AU_DVB_MODE_LL)
///     2: RR        (AU_DVB_MODE_RR)
///     3: L+R mix   (AU_DVB_MODE_MIXED_LR)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_SetDecOutputMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetDecOutputMode(MS_U32* u32DevIdx, MS_U32* mode)
{
    //set decoder sound mode
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if (MApi_AUDIO_SetAudioParam2(stDualDemoInfo[*u32DevIdx].eDecID, Audio_ParamType_acMode, *mode) == FALSE)
        {
            printf("[%s][%d] u32DevIdx(0x%"DTC_MS_U32_x") eDecID(%d) Set sound mode(0x%"DTC_MS_U32_x") fail!!!\n", __FUNCTION__, __LINE__, *u32DevIdx, stDualDemoInfo[0].eDecID, *mode);
            return FALSE;
        }
    }
    else
    {
        if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_soundmod, *mode,0) == FALSE)
        {
            printf("[%s][%d] Set sound mode(0x%"DTC_MS_U32_x") fail!!!\n", __FUNCTION__, __LINE__, *mode);
            return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set audio output mode
/// @param mode
///     0:STEREO     (AU_DVB_MODE_STEREO)
///     1: LL        (AU_DVB_MODE_LL)
///     2: RR        (AU_DVB_MODE_RR)
///     3: L+R mix   (AU_DVB_MODE_MIXED_LR)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_SetOutputMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetOutputMode(MS_U32* mode)
{
    if (mode == NULL)
    {
        return FALSE;
    }

    //set sound effect sound mode
    if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, *mode, 0) == FALSE)
    {
        printf("[%s][%d] Set sound mode(0x%"DTC_MS_U32_x") fail!!!\n", __FUNCTION__, __LINE__, *mode);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to demonstrate decoder mute
/// @param[in] u32DevIdx device index
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_dec_mute \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetDecMute(MS_U32* u32DevIdx, MS_BOOL* bEnable)
{
    MS_BOOL bRet = FALSE;

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        bRet = MApi_AUDIO_SetAudioParam2(stDualDemoInfo[*u32DevIdx].eDecID, Audio_ParamType_mute, *bEnable);
    }
    else
    {
        bRet = MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_mute, *bEnable, 0);
    }

    return bRet;
}


//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMute(MS_BOOL* bEnable)
{
    //mute line out
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, *bEnable);
#if (DEMO_AUDIO_HDMI_TEST == 1)
    //mute HDMI PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, *bEnable);

    //mute HDMI PCM + non PCM
    MApi_AUDIO_HDMI_Tx_SetMute(*bEnable);
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    //mute SPDIF PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, *bEnable);

    //mute SPDIF PCM + non PCM
    MApi_AUDIO_SPDIF_SetMute(*bEnable);
#endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code mute by path
/// @param[in] u8Path
///     0: main path
///     1: sub path
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute_path \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMutePath(MS_U8* u8Path, MS_U8* u8Mute)
{
    MS_BOOL bConnect = FALSE;
    if(*u8Mute)
    {
        bConnect=0;
    }
    else
    {
        bConnect=1;
    }

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SourceConnect(*u8Path, bConnect);
    }
    else
    {
        Demo_Audio_SetMute(u8Mute);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop music
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_demoStop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Stop_Demo(void)
{
    MS_U8 u8Idx = 0;
    memset((void *)_au32clipWriteLastAddr, 0, sizeof(MS_U32)*E_DECODER_NUMBER);
    memset((void *)_au32AudioFileIndex, 0, sizeof(MS_U32)*E_DECODER_NUMBER);
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        //Audio V2 flow
        for(u8Idx=0; u8Idx<E_DECODER_NUMBER; u8Idx++)
        {
            if(stDualDemoInfo[u8Idx].stAudioDecStatus_t.bConnect)
            {
                Demo_Audio_Stop(u8Idx, AUDIO_APP_ES_PLAY);
            }
        }
    }
    else
    {
        Demo_Audio_Stop(0, AUDIO_APP_ES_PLAY);
    }
    _bExit_Demo_Task = TRUE;
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to de-init audio
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Exit \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Exit(void)
{
    MApi_AUDIO_ExitAudioSystem();
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to initialize audio function
/// @param[in] Init type
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Init(MS_U32 *Type)
{
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    EN_AUDIO_APP_TYPE eType;
    AUDIO_SOURCE_INFO_TYPE eSrcType;

    printf("%s\n", __FUNCTION__);

    eType = *(EN_AUDIO_APP_TYPE *)Type;
    if(_Demo_Audio_CheckAppSupport(eType) == FALSE)
    {
        printf("unsupport application type: %u\n", (unsigned int)eType);
        return FALSE;
    }

    MApi_AUDIO_WritePreInitTable();

    //======================================
    // Set system info
    //======================================
    SystemInfo.dtv_system=0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    //======================================
    // Set output info(Output data from)
    //======================================
    OutputInfo.SpeakerOut = AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut = AUDIO_NULL_OUTPUT;
    OutputInfo.ScartOut = AUDIO_NULL_OUTPUT;
    OutputInfo.MonitorOut = AUDIO_NULL_OUTPUT;
#if (DEMO_AUDIO_SPDIF_TEST == 1)
    OutputInfo.SpdifOut = AUDIO_OUTPUT_SPDIF;
#else
    OutputInfo.SpdifOut = AUDIO_NULL_OUTPUT;
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
    OutputInfo.ArcOut = AUDIO_OUTPUT_ARC;
    OutputInfo.HDMIOut = AUDIO_OUTPUT_HDMI;
#else
    OutputInfo.ArcOut = AUDIO_NULL_OUTPUT;
    OutputInfo.HDMIOut = AUDIO_NULL_OUTPUT;
#endif

    MApi_AUDIO_SetOutputInfo(&OutputInfo);

    //======================================
    // Set path info (volume path control)
    //======================================
    PathInfo.SpeakerOut = AUDIO_PATH_MAIN_SPEAKER;  //path4(main)
    PathInfo.HpOut = AUDIO_PATH_I2S;  //K3 has no HP, so used for I2S
    PathInfo.MonitorOut = AUDIO_PATH_NULL;
    PathInfo.ScartOut = AUDIO_PATH_NULL;
#if (DEMO_AUDIO_SPDIF_TEST == 1)
    PathInfo.SpdifOut = AUDIO_PATH_SPDIF;
#else
    PathInfo.SpdifOut = AUDIO_PATH_NULL;
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
    PathInfo.ArcOut = AUDIO_PATH_SPDIF;
    PathInfo.HDMIOut = AUDIO_PATH_HDMI;
#else
    PathInfo.ArcOut = AUDIO_PATH_NULL;
    PathInfo.HDMIOut = AUDIO_PATH_NULL;
#endif
    MApi_AUDIO_SetPathInfo(&PathInfo);

    //======================================
    // Set audio base address
    //======================================
#if (DEMO_AUDIO_R2_MEM_ARCHI_TEST)
    MDrv_AUDIO_SetDspBaseAddr(DSP_ADV,  0, MEM_ADR_BY_MIU(MAD_ADV_BUF_ADR, MAD_ADV_BUF_MEMORY_TYPE));
#else
    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MEM_ADR_BY_MIU(MAD_DEC_BUF_ADR, MAD_DEC_BUF_MEMORY_TYPE));
#endif

    MApi_AUDIO_Initialize();

    //set intenal path by outinfo setting
    MApi_AUDIO_SetOutConnectivity();

    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);


#if ( (defined (DEMO_AUDIO_SUPPORT_MS12_B_TEST) && (DEMO_AUDIO_SUPPORT_MS12_B_TEST == 1)) || (defined (DEMO_AUDIO_SUPPORT_MS12_D_TEST) && (DEMO_AUDIO_SUPPORT_MS12_D_TEST == 1)) )
    EN_AUDIO_CODEC_TYPE enCodec = DEMO_AUDIO_CODEC_AAC;
    MS_U32 u32Delayms = 193;
    Demo_Audio_Codec_Delay( &enCodec, &u32Delayms);
#endif

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        //Audio V2 flow (multi decode)
        if (Demo_Audio_Dynamic_Allocate_Resource() == FALSE)
        {
            /* release decode system */
            _Demo_Audio_CloseDecSys(0);

            /* init decoder system for main */
            if(_Demo_Audio_OpenDecSys(0, E_CONNECT_MAIN, eType, MSAPI_AUD_DVB_MPEG) == FALSE)
            {
                printf("[%s][%d] open decode system (0) fail.\n", __FUNCTION__, __LINE__);
            }

#if (DEMO_AUDIO_MULTI_TEST == 1)
            _Demo_Audio_CloseDecSys(1);

            if(_Demo_Audio_OpenDecSys(1, E_CONNECT_SUB, eType, MSAPI_AUD_DVB_MPEG) == FALSE)
            {
                printf("[%s][%d] open decode system (1) fail.\n", __FUNCTION__, __LINE__);
            }
#endif

        }
    }
    else
    {
        //Audio V1 flow
        switch(eType)
        {
        case AUDIO_APP_ES_PLAY:
            eSrcType = E_AUDIO_INFO_GAME_IN;
            break;
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
        case AUDIO_APP_DTV_AD:
            eSrcType = E_AUDIO_INFO_DTV_IN;
            break;
        case AUDIO_APP_ATV_FMTX:
            eSrcType = E_AUDIO_INFO_DTV_IN;
            break;
#if (DEMO_XC_HDMIRX_TEST == 1)
        case AUDIO_APP_HDMI_RX:
            eSrcType = E_AUDIO_INFO_HDMI_IN;
            _bFirstIn = TRUE;
            break;
#endif
        default:
            eSrcType = E_AUDIO_INFO_GAME_IN;
        }

        stDualDemoInfo[0].eDecID = AU_DEC_ID1;

        MApi_AUDIO_SetSourceInfo(eSrcType);
        MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
    }

#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Switch DSP/R2 resource.
/// @param[in] u32DevIdx1
/// @param[in] u32DevIdx2
/// @return TRUE: Open success.
/// @return FALSE: Open fail.
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SwitchResource(MS_U32 *u32DevIdx1, MS_U32 *u32DevIdx2)
{
    if (*u32DevIdx1 == *u32DevIdx2)
    {
        return TRUE;
    }

    ST_AUDIO_DEC_INFO eTemp = {0};
    memcpy((void*)&eTemp, (void*)&stDualDemoInfo[*u32DevIdx1], sizeof(ST_AUDIO_DEC_INFO));
    memcpy((void*)&stDualDemoInfo[*u32DevIdx1], (void*)&stDualDemoInfo[*u32DevIdx2], sizeof(ST_AUDIO_DEC_INFO));
    memcpy((void*)&stDualDemoInfo[*u32DevIdx2], (void*)&eTemp, sizeof(ST_AUDIO_DEC_INFO));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to allocate DSP/R2 resource.
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
/// @return TRUE: Open success.
/// @return FALSE: Open fail.
/// @sa
/// @note
/// Command: \b Audio_Open \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Open(MS_U32 *u32DevIdx, AUDIO_OUTPORT_SOURCE_TYPE *eOutSrcType, EN_AUDIO_APP_TYPE *eAppType)
{
    MS_BOOL bRet = FALSE;

    //allocate audio resource
    switch (*eAppType)
    {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV_AM:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AD:
        case AUDIO_APP_HDMI_RX:
        case AUDIO_APP_TSP_FRAMEDECODE:
        {
            if (_Demo_Audio_OpenDecSys(*u32DevIdx, *eOutSrcType, *eAppType, MSAPI_AUD_DVB_MPEG) == FALSE)
            {
                bRet = FALSE;
            }
            else
            {
                bRet = TRUE;
            }
            break;
        }
        default:
            bRet = TRUE;
            break;
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to release DSP/R2 resource.
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @return TRUE: Release success.
/// @return FALSE: Release fail.
/// @sa
/// @note
/// Command: \b Audio_Release \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Release(MS_U32 *u32DevIdx)
{
    return _Demo_Audio_CloseDecSys(*u32DevIdx);
}

//------------------------------------------------------------------------------
/// @brief Start audio dec play
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @param[in] eCodecType
///     codec type
/// @param[in] pvParam
///     input parameter (depend on codec type)
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Play(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, EN_AUDIO_CODEC_TYPE eCodecType, void *pvParam)
{
    En_DVB_decSystemType eDecSysType;

    if(u32DevIdx >= E_DECODER_NUMBER)
    {
        //printf("unsupport device index: %u\n", (unsigned int)u32DevIdx);
        return FALSE;
    }

    if(_Demo_Audio_CheckAppSupport(eAppType) == FALSE)
    {
        //printf("unsupport application type: %u\n", (unsigned int)eAppType);
        return FALSE;
    }

    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if(stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_INVALID)
        {
            return FALSE;
        }
    }

    eDecSysType = _Demo_Audio_CodecTypeMap(eCodecType);

    if(eDecSysType == MSAPI_AUD_DVB_INVALID)
    {
        return FALSE;
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
    if(_Demo_DDP_BYPASS_Check_Allow_Feature(u32DevIdx, eAppType) == FALSE)
    {
        printf("\nWarning!!! ATV, PCM capture or SUB AV NOT allowed due to DDP bypass is used!!!\n\n");
        return FALSE;
    }
#endif

    if (eAppType != AUDIO_APP_DTV_AD_1PID)
    {
        if (_Demo_Audio_CheckAudioCapability(u32DevIdx, eDecSysType))
        {
            _Demo_Audio_SetSystem(u32DevIdx, eDecSysType);
        }
        else
        {
            printf("[%s][%d] Unsupport Audio Codec(%d)!!!\n",__FUNCTION__,__LINE__, eDecSysType);
            return FALSE;
        }
    }

    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            if(eDecSysType == MSAPI_AUD_DVB_XPCM)
            {
                MApi_AUDIO_SetAudioParam2(stDualDemoInfo[u32DevIdx].eDecID, Audio_ParamType_setXPCMDecParam, (MS_U32)pvParam);
            }

            if(_bNew_MM_Mode[u32DevIdx] == TRUE)
            {
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
            }
            else
            {
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYFILE);
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
            }
        }
        else
        {
            if(eDecSysType == MSAPI_AUD_DVB_XPCM)
            {
                ST_WAVINFO *pstWAV = (ST_WAVINFO *)pvParam;

                MApi_AUDIO_SetXPCMParam(
                    LPCM,
                    pstWAV->u16Channels-1,
                    pstWAV->u32SamplesPerSec,
                    pstWAV->u16BitsPerSample,
                    pstWAV->u16BlockSize,
                    pstWAV->u16BitsPerSample
                );
            }

            if(_bNew_MM_Mode[u32DevIdx] == TRUE)
            {
                //new MM mode play command
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
                //can set av sync or free run
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
            }
            else
            {
                MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
            }
        }
        break;
    case AUDIO_APP_PES_PLAY:
        {
            MS_U32 u32Time = 0;

            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {

                //disable PUSI flag
                if (MApi_AUDIO_SetAudioParam2(stDualDemoInfo[u32DevIdx].eDecID, Audio_ParamType_PUSI_Enable, 0) == FALSE)
                {
                    printf("[%s][%d] dev_idx=%u, disable PUSI flag fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                }

                if(_Demo_Audio_STC_Switch(u32DevIdx, *(AUDIO_STC_SOURCE *)pvParam) == FALSE)
                {
                    printf("[%s][%d] dev_idx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                }

                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
                MApi_AUDIO_SourceConnect(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eGroup, FALSE);

                //check PES parser ready
                MS_U32 u32ParserReady = 0;

                u32Time = MsOS_GetSystemTime();
                while (u32ParserReady == 0)
                {
                    if ((MsOS_GetSystemTime()-u32Time) > CHECK_PARSER_TIMEOUT)
                    {
                        printf("[%s][%d] Check parser ready timeout!!!\n", __FUNCTION__,__LINE__);
                        return FALSE;
                    }
                    MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[u32DevIdx].eDecID, Audio_infoType_get_parser_info, &u32ParserReady);
                    MsOS_DelayTask(1);
                }
                printf("[%s][%d] Check parser ready done!!! spent time = %"DTC_MS_U32_d"ms\n", __FUNCTION__,__LINE__, MsOS_GetSystemTime() - u32Time);
            }
            else
            {
                //disable PUSI flag
                if (MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_PUSI_disable_Info, 1, 0) == FALSE)
                {
                    printf("[%s][%d] dev_idx=%u, disable PUSI flag fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                }

                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);

                //check PES parser ready
                MS_U64 u64ParserReady = 0;

                u32Time = MsOS_GetSystemTime();
                while (u64ParserReady == 0)
                {
                    u64ParserReady = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Parser_Info);

                    if ((MsOS_GetSystemTime()-u32Time) > CHECK_PARSER_TIMEOUT)
                    {
                        printf("[%s][%d] Check parser ready timeout!!!\n", __FUNCTION__,__LINE__);
                        return FALSE;
                    }
                    MsOS_DelayTask(1);
                }
                printf("[%s][%d] Check parser ready done!!! spent time = %"DTC_MS_U32_d"ms\n", __FUNCTION__,__LINE__, MsOS_GetSystemTime() - u32Time);
            }
        }
        break;
    case AUDIO_APP_DTV:
        {
            MS_U32 u32Time = 0;

            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                AUDIO_STC_SOURCE eSTCSource = (AUDIO_STC_SOURCE)(*(MS_U8*)pvParam);

                if(_Demo_Audio_STC_Switch(u32DevIdx, eSTCSource) == FALSE)
                {
                    printf("[%s][%d] dev_idx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                }

                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAY);
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_AVSYNC);
                MApi_AUDIO_SourceConnect(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eGroup, FALSE);

                //check PES parser ready
                MS_U32 u32ParserReady = 0;

                u32Time = MsOS_GetSystemTime();
                while (u32ParserReady == 0)
                {
                    if ((MsOS_GetSystemTime()-u32Time) > CHECK_PARSER_TIMEOUT)
                    {
                        printf("[%s][%d] Check parser ready timeout!!!\n", __FUNCTION__,__LINE__);
                        return FALSE;
                    }
                    MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[u32DevIdx].eDecID, Audio_infoType_get_parser_info, &u32ParserReady);
                    MsOS_DelayTask(1);
                }
                printf("[%s][%d] Check parser ready done!!! spent time = %"DTC_MS_U32_d"\n", __FUNCTION__,__LINE__, MsOS_GetSystemTime() - u32Time);
            }
            else
            {
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_AVSYNC);

                //check PES parser ready
                MS_U64 u64ParserReady = 0;

                u32Time = MsOS_GetSystemTime();
                while (u64ParserReady == 0)
                {
                    u64ParserReady = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Parser_Info);

                    if ((MsOS_GetSystemTime()-u32Time) > CHECK_PARSER_TIMEOUT)
                    {
                        printf("[%s][%d] Check parser ready timeout!!!\n", __FUNCTION__,__LINE__);
                        return FALSE;
                    }
                    MsOS_DelayTask(1);
                }
                printf("[%s][%d] Check parser ready done!!! spent time = %"DTC_MS_U32_d"\n", __FUNCTION__,__LINE__, MsOS_GetSystemTime() - u32Time);
            }
        }
        break;
    case AUDIO_APP_DTV_AM:
        {
            MS_U32 u32Time = 0;

            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                if(_Demo_Audio_STC_Switch(u32DevIdx, *(AUDIO_STC_SOURCE *)pvParam) == FALSE)
                {
                    printf("[%s][%d] dev_idx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                }
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
                MApi_AUDIO_SourceConnect(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eGroup, FALSE);

                //check PES parser ready
                MS_U32 u32ParserReady = 0;

                u32Time = MsOS_GetSystemTime();
                while (u32ParserReady == 0)
                {
                    if ((MsOS_GetSystemTime()-u32Time) > CHECK_PARSER_TIMEOUT)
                    {
                        printf("[%s][%d] Check parser ready timeout!!!\n", __FUNCTION__,__LINE__);
                        return FALSE;
                    }
                    MApi_AUDIO_GetAudioInfo2(stDualDemoInfo[u32DevIdx].eDecID, Audio_infoType_get_parser_info, &u32ParserReady);
                    MsOS_DelayTask(1);
                }
                printf("[%s][%d] Check parser ready done!!! spent time = %"DTC_MS_U32_d"ms\n", __FUNCTION__,__LINE__, MsOS_GetSystemTime() - u32Time);
            }
            else
            {
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);

                //check PES parser ready
                MS_U64 u64ParserReady = 0;

                u32Time = MsOS_GetSystemTime();
                while (u64ParserReady == 0)
                {
                    u64ParserReady = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Parser_Info);

                    if ((MsOS_GetSystemTime()-u32Time) > CHECK_PARSER_TIMEOUT)
                    {
                        printf("[%s][%d] Check parser ready timeout!!!\n", __FUNCTION__,__LINE__);
                        return FALSE;
                    }
                    MsOS_DelayTask(1);
                }
                printf("[%s][%d] Check parser ready done!!! spent time = %"DTC_MS_U32_d"ms\n", __FUNCTION__,__LINE__, MsOS_GetSystemTime() - u32Time);
            }
        }
        break;
#if (DEMO_AUDIO_AD_TEST == 1)
    case AUDIO_APP_DTV_AD:
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAY_AD);
        }
        else
        {
            MApi_AUDIO_SetCommand(MSAPI_AUD_DVB2_DECCMD_PLAY);
            MApi_AUDIO_SetADOutputMode(AD_OUT_BOTH);
        }
        break;
#endif
#if (DEMO_AUDIO_AD_1PID_TEST == 1)
    case AUDIO_APP_DTV_AD_1PID:
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DDP_AD_Substream, TRUE, stDualDemoInfo[u32DevIdx].eDecID);
        }
        else
        {
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DDP_AD_Substream, TRUE, 0);
        }
        break;
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
        {
            ST_AUDIO_FMTX_PARAM *stFMTX = (ST_AUDIO_FMTX_PARAM *)pvParam;
            MS_U8 u8Filter = stFMTX->u8Filter;
            MS_U8 u8Attenuation_H = (MS_U8)((stFMTX->u16Attenuation) >> 8);
            MS_U8 u8Attenuation_L = (MS_U8)((stFMTX->u16Attenuation) & 0xFF);
            MS_U8 u8OutScaling_H = (MS_U8)((stFMTX->u16OutScaling) >> 8);
            MS_U8 u8OutScaling_L = (MS_U8)((stFMTX->u16OutScaling) & 0xFF);

            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);

            //set internal path, temporary mark because keltic has no enum INTERNAL_ATVENC
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_FMTX);

            if(u8Filter > 1)
            {
                u8Filter = 1;
            }

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP, u8Filter, 0);

            //start encode
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);

        }
        break;
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
        {
            ST_AUDIO_BTSC_PARAM *stBTSC = (ST_AUDIO_BTSC_PARAM *)pvParam;
            MS_U8 u8Attenuation_H = (stBTSC->u16Attenuation) >> 8;
            MS_U8 u8Attenuation_L = (stBTSC->u16Attenuation) & 0xFF;
            MS_U8 u8OutScaling_H = (stBTSC->u16OutScaling) >> 8;
            MS_U8 u8OutScaling_L = (stBTSC->u16OutScaling) & 0xFF;
            MS_U8 u8_M_Gain_H = (stBTSC->u16_M_Gain) >> 8;
            MS_U8 u8_M_Gain_L = (stBTSC->u16_M_Gain) & 0xFF;
            MS_U8 u8_D_Gain_H = (stBTSC->u16_D_Gain) >> 8;
            MS_U8 u8_D_Gain_L = (stBTSC->u16_D_Gain) & 0xFF;
            MS_U8 u8_SAP_Gain_H = (stBTSC->u16_SAP_Gain) >> 8;
            MS_U8 u8_SAP_Gain_L = (stBTSC->u16_SAP_Gain) & 0xFF;

            //MApi_AUDIO_SetSystem(eDecSysType);

            //disable Dolby SPDIF volume control, when BTSC start
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain, u8_M_Gain_H, u8_M_Gain_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain, u8_D_Gain_H, u8_D_Gain_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain, u8_SAP_Gain_H, u8_SAP_Gain_L);

            //used for keres and later BTSC supported CHIPs. Kaiser will ignore the setting, it hard code in the DSP.
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_BTSC1);
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_BTSC2);

            //start encode
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);

        }
        break;
#endif
#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
    case AUDIO_APP_TSP_FRAMEDECODE:
        {
            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                if (pvParam)
                {
                    AUDIO_STC_SOURCE eSTCSource = (AUDIO_STC_SOURCE)(*(MS_U8*)pvParam);

                    printf("[%s][%d] dev_idx=%u, Set STC(%d) \n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx, eSTCSource);

                    if(_Demo_Audio_STC_Switch(u32DevIdx, eSTCSource) == FALSE)
                    {
                        printf("[%s][%d] dev_idx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
                    }
                }

                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_TSP_FRAMEDECODE);
            }
            else
            {
                printf("[%s][%d] dev_idx=%u, MSAPI_AUD_DVB_DECCMD_TSP_FRAMEDECODE not support!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
            }
        }
        break;
#endif
    default:
            return FALSE;
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
    _Demo_DDP_BYPASS_Set_Feature_Status(u32DevIdx, eAppType);
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop audio dec
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @return TRUE: success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Stop(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    MS_BOOL bMute = TRUE;

    if(u32DevIdx >= E_DECODER_NUMBER)
    {
        //printf("unsupport device index: %u\n", (unsigned int)u32DevIdx);
        return FALSE;
    }

    if(_Demo_Audio_CheckAppSupport(eAppType) == FALSE)
    {
        //printf("unsupport application type: %u\n", (unsigned int)eAppType);
        return FALSE;
    }

    //if(MDrv_AUDIO_GET_INIT_FLAG())
    {
        switch(eAppType)
        {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                Demo_Audio_SetMutePath((MS_U8*)&u32DevIdx,&bMute);
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_STOP);
            }
            else
            {
                Demo_Audio_SetMute(&bMute);
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
            }
            break;
#if (DEMO_AUDIO_AD_TEST == 1)
        case AUDIO_APP_DTV_AD:
            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_STOP_AD);
                MApi_AUDIO_SetADOutputMode(AD_OUT_NONE);
            }
            else
            {
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB2_DECCMD_STOP);
                MApi_AUDIO_SetADOutputMode(AD_OUT_NONE);
            }
            break;
#endif
#if (DEMO_AUDIO_AD_1PID_TEST == 1)
        case AUDIO_APP_DTV_AD_1PID:
            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DDP_AD_Substream, FALSE, stDualDemoInfo[u32DevIdx].eDecID);
            }
            else
            {
                MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DDP_AD_Substream, FALSE, 0);
            }
            break;
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
        case AUDIO_APP_ATV_FMTX:
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , TRUE, 0);
            break;
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
        case AUDIO_APP_ATV_BTSC:
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , TRUE, 0);
            break;
#endif
        case AUDIO_APP_TSP_FRAMEDECODE:
            if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
            {
                MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_STOP);
            }
            else
            {
                printf("[%s][%d] dev_idx=%u, MSAPI_AUD_DVB_DECCMD_TSP_FRAMEDECODE not support!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
            }
            break;
        default:
            return FALSE;
        }
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1_TEST == 1)
    _Demo_DDP_BYPASS_Clear_Feature_Status(u32DevIdx, eAppType);
#endif

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Return a copy of audio dec info
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[out] pstInfo
///     pointer to output buffer
/// @return TRUE: success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_GetDecInfo(MS_U32 u32DevIdx, ST_AUDIO_DEC_INFO *pstInfo)
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        if((u32DevIdx >= E_DECODER_NUMBER) || (pstInfo == NULL))
        {
            return FALSE;
        }
        memcpy((void *)pstInfo, (void *)&stDualDemoInfo[u32DevIdx], sizeof(ST_AUDIO_DEC_INFO));
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set sync STC (for MMTS mode)
/// @param[in] DecId Decoder ID
/// @param[in] u32Param parameter
/// @return TRUE: success.
/// @return FALSE: fail
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SyncSTC(AUDIO_DEC_ID DecId, MS_U32 u32Param )
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_SetAudioParam2(DecId, Audio_ParamType_syncSTC, u32Param);
    }
    else
    {
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, u32Param, 0);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get sync STC status (for MMTS mode)
/// @param[in] DecId Decoder ID
/// @param[out] u32Param parameter
/// @return TRUE: success.
/// @return FALSE: fail
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_GetSyncSTCStatus(AUDIO_DEC_ID DecId, MS_U32* u32Param )
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        MApi_AUDIO_GetAudioInfo2(DecId, Audio_infoType_syncSTC_in_mmTs, u32Param);
    }
    else
    {
        *u32Param = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get decoder status
/// @param[in] DecId Decoder ID
/// @param[out] u32Param parameter
/// @return TRUE: success.
/// @return FALSE: fail
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_GetDecStatus(AUDIO_DEC_ID DecId, MS_U8* u8Param )
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        return MApi_AUDIO_GetAudioInfo2(DecId, Audio_infoType_DecStatus, u8Param);
    }
    else
    {
        *u8Param = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_currAudDecStatus);
        return TRUE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Get current PTS
/// @param[in] DecId Decoder ID
/// @param[in] u32Param parameter
/// @return TRUE: success.
/// @return FALSE: fail
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_GetCPTS(AUDIO_DEC_ID DecId, MS_U64* u64Param )
{
    if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
    {
        return MApi_AUDIO_GetAudioInfo2(DecId, Audio_infoType_33bit_PTS, u64Param);
    }
    else
    {
        *u64Param = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_33bit_PTS);
        return TRUE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to playback music
/// @param[in] pFileFmt
///     0: mp3
///     1: wav
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_demo \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Out_Init(MS_U32 *pFileFmt)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
    MS_U32 u32DevIdx=0;
    EN_AUDIO_APP_TYPE eAppType;
    EN_AUDIO_CODEC_TYPE eCodecType;
    MS_U32 u32Data = 0;
    void *pvParam = &u32Data;
    Audio_XPCM_Param stXPCM={};

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    //audio init need be called at the beginning
    eAppType = AUDIO_APP_ES_PLAY;
    Demo_Audio_Init((MS_U32 *)&eAppType);

    if(_audioDemo_Task.iId == -1)
    {
        _audioDemo_Task.pStack = MsOS_AllocateMemory(_audioDemo_Task.u32StackSize, s32MstarCachedPoolID);
        if(!_audioDemo_Task.pStack)
        {
            printf("fail 1\n");
            return FALSE;
        }

        /// - Create Task Application
        _audioDemo_Task.iId = MsOS_CreateTask( _appAudioDemo_task,
                                            0,
                                            _audioDemo_Task.ePriority,
                                            TRUE,
                                            _audioDemo_Task.pStack,
                                            _audioDemo_Task.u32StackSize,
                                            _audioDemo_Task.szName);
        if(_audioDemo_Task.iId < 0)
        {
            printf("fail 2\n");
            return FALSE;
        }
    }

    if(*pFileFmt == 0)
    {
        //demo code for playing mp3 with using old MM mode + main path
        u32DevIdx = 0;
        eCodecType = DEMO_AUDIO_CODEC_MP3;

        _au32clipWriteLastAddr[u32DevIdx]=0;
        _au32AudioFileIndex[u32DevIdx]=0;
        _apu8AudioData[u32DevIdx] = mp3_bin_var;
        _au32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = FALSE;
    }
    else if(*pFileFmt == 1)
    {
        //demo code for playing wav with using old MM mode + main path
        u32DevIdx = 0;
        eCodecType = DEMO_AUDIO_CODEC_XPCM;

        stXPCM.audioType = LPCM;
        memcpy(&stXPCM.channels, wav_bin_var + 22, 2);//set channel num
        memcpy(&stXPCM.sampleRate, wav_bin_var+ 24, 4);//set sampling rate
        memcpy(&stXPCM.bitsPerSample, wav_bin_var + 34, 2);//set bit per sample
        memcpy(&stXPCM.blockSize, wav_bin_var + 32, 2);//set block size

        _au32clipWriteLastAddr[u32DevIdx]=0;
        _au32AudioFileIndex[u32DevIdx]=0;
        _apu8AudioData[u32DevIdx] = wav_bin_var;
        _au32AudioLen[u32DevIdx] = WAV_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = FALSE;
        pvParam = (void *)&stXPCM;
    }
    else if(*pFileFmt == 2)
    {
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //demo code for playing mp3 with using old MM mode + sub path
            u32DevIdx = 1;
            eCodecType = DEMO_AUDIO_CODEC_MP3;

            _au32clipWriteLastAddr[u32DevIdx]=0;
            _au32AudioFileIndex[u32DevIdx]=0;
            _apu8AudioData[u32DevIdx] = mp3_bin_var;
            _au32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
            _bNew_MM_Mode[u32DevIdx] = FALSE;
        }
        else
        {
            printf("Not support multi decode!!!\n");
            return FALSE;
        }
    }
    else if(*pFileFmt == 3)
    {
        //demo code for playing mp3 with using new MM mode + main path
        u32DevIdx = 0;
        eCodecType = DEMO_AUDIO_CODEC_MP3;
        MApi_AUDIO_MM2_initAesInfo(stDualDemoInfo[u32DevIdx].eDecID);
        _au32clipWriteLastAddr[u32DevIdx]=0;
        _au32AudioFileIndex[u32DevIdx]=0;
        _apu8AudioData[u32DevIdx] = mp3_bin_var;
        _au32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = TRUE;
    }
    else if(*pFileFmt == 4)
    {
        if (MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_MultiPlayer_Capability) == 1)
        {
            //demo code for playing mp3 with using new MM mode + sub path
            u32DevIdx = 1;
            eCodecType = DEMO_AUDIO_CODEC_MP3;
            MApi_AUDIO_MM2_initAesInfo(stDualDemoInfo[u32DevIdx].eDecID);
            _au32clipWriteLastAddr[u32DevIdx]=0;
            _au32AudioFileIndex[u32DevIdx]=0;
            _apu8AudioData[u32DevIdx] = mp3_bin_var;
            _au32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
            _bNew_MM_Mode[u32DevIdx] = TRUE;
        }
        else
        {
            printf("Not support multi decode!!!\n");
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

#if 0
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif

	if (Demo_Audio_Play(u32DevIdx, eAppType, eCodecType, (void *)pvParam) == FALSE)
	{
        return FALSE;
	}

    bMute = FALSE;
    Demo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    Demo_Audio_SetAbsoluteVolume(&u8Volume);

    return TRUE;
}

#if defined (DEMO_AUDIO_SUPPORT_MS12_B_TEST) && (DEMO_AUDIO_SUPPORT_MS12_B_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to disable/enable MS12 DAP
/// @param[in] bEnable disble/enable DAP
/// @return TRUE: Set DAP enable success.
/// @return FALSE: Set DAP enable fail.
/// @sa
/// @note
/// Command: \b Audio_DAP \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetDAP_Enable(MS_BOOL* bEnable)
{
    if (bEnable == NULL)
    {
        return FALSE;
    }

    if (*bEnable == TRUE)
    {
        //this is used to switch ADV sound effect to DAP
        if (MApi_AUDIO_ADVSOUND_ProcessEnable(DOLBY_DAP) == FALSE)
        {
            return FALSE;
        }
    }

    return MApi_AUDIO_ADVSOUND_SubProcessEnable(DOLBY_DAP_EN, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to laod MS12 DAP parameter from file
/// @param[in] filename script file of DAP parameter
/// @return TRUE: Set DAP parameter success.
/// @return FALSE: Set DAP parameter fail.
/// @sa
/// @note
/// Command: \b Audio_DAP_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetDAP_Param(char* filename)
{
    dap_params_t st_dap_params = {0};

    if (filename == NULL)
    {
        return FALSE;
    }

    //load script to a structure
    if (_Demo_Audio_Load_DAP_Param_FromFile(filename, &st_dap_params) == TRUE)
    {
        //set DAP api
        MApi_AUDIO_COPY_Parameter(DOLBY_DAP_SET_PARAMS, &st_dap_params, sizeof(st_dap_params));
        return TRUE;
    }

    return FALSE;
}
#endif

#if (DEMO_AUDIO_ENCODER_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Allocate Audio encoder resource
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @param[in] encode_type
///     MP3_ENCODER1: encoder 1
///     MP3_ENCODER2: encoder 2
///     3rd_HEAAC_ENCODER1: 3rd party heaac encoder 1
///     3rd_HEAAC_ENCODER2: 3rd party heaac encoder 2
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Enc_Open \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Enc_Open(MS_U32 *u32EncIdx, char* encode_type)
{
    char s8Name[AUDIO_STRNAME_LENGTH] = {0};
    MS_S32 s32Encid = -1;

    if (u32EncIdx == NULL)
    {
        return FALSE;
    }

    if (encode_type == NULL)
    {
        return FALSE;
    }

    snprintf(s8Name, AUDIO_STRNAME_LENGTH, "%s", encode_type);

    s32Encid = MApi_AUDIO_ENCODER_Open(s8Name);

    if (s32Encid >= 0)
    {
        _stAudioEncTask[*u32EncIdx].s32DeviceId = s32Encid;
        printf("[%s][%d] u32EncIdx(%"DTC_MS_U32_u"), encode id(%"DTC_MS_S32_d"), allocate Encode resource ok!!!\n", __FUNCTION__, __LINE__, *u32EncIdx, s32Encid);
        return TRUE;
    }
    else
    {
        printf("[%s][%d] u32EncIdx(%"DTC_MS_U32_u"), encode id(%"DTC_MS_S32_d"), no available Encode resource!!!\n", __FUNCTION__, __LINE__, *u32EncIdx, s32Encid);
        return FALSE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Release Audio encoder resource
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Enc_Close \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Enc_Close(MS_U32 *u32EncIdx)
{
    if (u32EncIdx == NULL)
    {
        return FALSE;
    }

    if (_stAudioEncTask[*u32EncIdx].s32DeviceId >= 0)
    {
        if (MApi_AUDIO_ENCODER_Close(_stAudioEncTask[*u32EncIdx].s32DeviceId) == 0)
        {
            _stAudioEncTask[*u32EncIdx].s32DeviceId = -1;
            return TRUE;
        }
        else if (MApi_AUDIO_ENCODER_Close(_stAudioEncTask[*u32EncIdx].s32DeviceId) < 0)
        {
            printf("[%s][%d] u32EncIdx(%"DTC_MS_U32_u"), encode id(%"DTC_MS_S32_d"), release Encode resource fail!!!\n", __FUNCTION__, __LINE__, *u32EncIdx, _stAudioEncTask[*u32EncIdx].s32DeviceId);
            return FALSE;
        }
    }
    else
    {
        printf("[%s][%d] u32EncIdx(%"DTC_MS_U32_u"), encode id(%"DTC_MS_S32_d"), no Encode resource!!!\n", __FUNCTION__, __LINE__, *u32EncIdx, _stAudioEncTask[*u32EncIdx].s32DeviceId);
        return FALSE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Send Audio encode ioctl
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Enc_Ioctl \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Enc_Ioctl(MS_U32 *u32EncIdx, char* pCmd, MS_U32 *u32Data)
{
    char s8Cmd[AUDIO_STRCMD_LENGTH] = {0};

    if (u32EncIdx == NULL)
    {
        return FALSE;
    }

    snprintf(s8Cmd, AUDIO_STRCMD_LENGTH, "%s", pCmd);
    if (MApi_AUDIO_ENCODER_Ioctl(_stAudioEncTask[*u32EncIdx].s32DeviceId, s8Cmd, u32Data) < 0)
    {
        printf("[%s][%d] u32EncIdx(%"DTC_MS_U32_u"), encode id(%"DTC_MS_S32_d"), pcmd(%s), ioctl fail!!!\n", __FUNCTION__, __LINE__, *u32EncIdx, _stAudioEncTask[*u32EncIdx].s32DeviceId, pCmd);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Flush Audio encoded buffer
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Enc_Flush \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Enc_Flush(MS_U32 *u32EncIdx)
{
    if (u32EncIdx == NULL)
    {
        return FALSE;
    }

    if (MApi_AUDIO_ENCODER_Flush(_stAudioEncTask[*u32EncIdx].s32DeviceId) < 0)
    {
        printf("[%s][%d] u32EncIdx(%"DTC_MS_U32_u"), encode id(%"DTC_MS_S32_d"), flush fail!!!\n", __FUNCTION__, __LINE__, *u32EncIdx, _stAudioEncTask[*u32EncIdx].s32DeviceId);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Dump Audio encode data
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Enc_Dump \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Enc_Dump(MS_U32 *u32EncIdx, char* outputfilename, MS_U32* u32RequireDumpSize)
{
    if (u32EncIdx == NULL)
    {
        return FALSE;
    }

    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    if(_stAudioEncTask[*u32EncIdx].stTask_Info.iId == -1)
    {
        _stAudioEncTask[*u32EncIdx].stTask_Info.pStack = MsOS_AllocateMemory(_stAudioEncTask[*u32EncIdx].stTask_Info.u32StackSize, s32MstarCachedPoolID);
        if(!_stAudioEncTask[*u32EncIdx].stTask_Info.pStack)
        {
            printf("[%s][%d] encode device(%"DTC_MS_S32_d"), allocate stack fail!!!\n", __FUNCTION__, __LINE__, *u32EncIdx);
            return FALSE;
        }

        //set task param
        memset(_stAudioEncTask[*u32EncIdx].u8Filename, 0, sizeof(_stAudioEncTask[*u32EncIdx].u8Filename));
        memcpy(_stAudioEncTask[*u32EncIdx].u8Filename, outputfilename, strlen(outputfilename));
        _stAudioEncTask[*u32EncIdx].u32RequireDumpSize = *u32RequireDumpSize;

        _stAudioEncTask[*u32EncIdx].stTask_Info.iId = MsOS_CreateTask((TaskEntry) _appAudio_Enc_DumpFile_task,
                                            (MS_VIRT)(&_stAudioEncTask[*u32EncIdx]),
                                            _stAudioEncTask[*u32EncIdx].stTask_Info.ePriority,
                                            TRUE,
                                            _stAudioEncTask[*u32EncIdx].stTask_Info.pStack,
                                            _stAudioEncTask[*u32EncIdx].stTask_Info.u32StackSize,
                                            _stAudioEncTask[*u32EncIdx].stTask_Info.szName);

        if(_stAudioEncTask[*u32EncIdx].stTask_Info.iId < 0)
        {
            printf("[%s][%d] encode device(%"DTC_MS_S32_d"), create task fail!!!\n", __FUNCTION__, __LINE__, *u32EncIdx);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        printf("[%s][%d] encode device(%"DTC_MS_S32_d"), encode task already exist!!!\n", __FUNCTION__, __LINE__, *u32EncIdx);
        return FALSE;
    }


    return TRUE;
}
#endif

#if (DEMO_AUDIO_FRAME_DECODE_TEST == 1)
//------------------------------------------------------------------------------
/// @brief Start Audio transcoding data
/// @param[in] u32DevIdx
///     0: reserve for Main DTV (can control by DDI command)
///     1: reserve for Sub DTV (can control by DDI command)
///     2: Transcoder 1
///     3: Transcoder 2
/// @param[in] u32OutputPath
///     0: FILE, output to a file
///     1: MEM, output to memory
/// @param[in] u32OutputFormat
///     0: PCM, Pulse code modulation stream
///     1: ES, elementary stream
///     2: PES, packetized elementary stream
/// @param[in] u32OutputFilename
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Transcode_Start 0 0 0 /mnt/sda1/xxx.pcm \n
/// Command: \b Audio_Transcode_Start 0 0 1 /mnt/sda1/xxx.aac \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Transcode_Start(MS_U32 *u32DevIdx, EN_AUDIO_TRANSCODE_PATH* enOutputPath, EN_AUDIO_TRANSCODE_FORMAT* enOutputFormat, MS_U32* u32OutputFilename, MS_BOOL* bOutFileAppend)
{
    EN_AV_Device eAvDev = *u32DevIdx;
    ST_AV_Params stAudioParam = {0};
    ST_AV_Params stPCRParam = {0};

    //check if device available
    if(stDualDemoInfo[*u32DevIdx].stAudioDecStatus_t.bConnect == FALSE)
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") not allocate!!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        return FALSE;
    }

#if (DEMO_AUDIO_3RD_PARTY_AAC_ENCODER_TEST == 0)
    if ((*enOutputFormat == DEMO_AUDIO_TRANSCODE_FORMAT_ES) || (*enOutputFormat == DEMO_AUDIO_TRANSCODE_FORMAT_ES))
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") not support 3rd-party AAC encoder!!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        return FALSE;
    }
#endif

    if (Demo_AV_GetAVInfo(&eAvDev, E_AV_GetCmd_AudioInfo, &stAudioParam) == FALSE)
    {
        printf("[%s][%d] Device (%"DTC_MS_U32_u") Get AV Info fail !!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        return FALSE;
    }

    if (Demo_AV_GetAVInfo(&eAvDev, E_AV_GetCmd_PCRInfo, &stPCRParam) == FALSE)
    {
        printf("[%s][%d] Device (%"DTC_MS_U32_u") Get AV Info fail !!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        return FALSE;
    }

    //set transcoding command
    if (stPCRParam.ePCREngID == E_PCR_ENG_INVALID)
    {
        if (Demo_Audio_Play(*u32DevIdx, AUDIO_APP_TSP_FRAMEDECODE, stAudioParam.eAudioCodec, NULL) == FALSE)
        {
            printf("[%s][%d] Device (%"DTC_MS_U32_u") Demo Audio Play fail !!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
            return FALSE;
        }
    }
    else
    {
        if (Demo_Audio_Play(*u32DevIdx, AUDIO_APP_TSP_FRAMEDECODE, stAudioParam.eAudioCodec, &stPCRParam.ePCREngID) == FALSE)
        {
            printf("[%s][%d] Device (%"DTC_MS_U32_u") Demo Audio Play fail !!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
            return FALSE;
        }
    }

    //start transcoding task
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    if (Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID) == FALSE)
    {
        printf("[%s][%d] Device (%"DTC_MS_U32_u") Get system pool ID fail !!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        return FALSE;
    }

    if(_stAudioTranscodeTask[*u32DevIdx].stTask_Info.iId == -1)
    {
        _stAudioTranscodeTask[*u32DevIdx].stTask_Info.pStack = MsOS_AllocateMemory(_stAudioTranscodeTask[*u32DevIdx].stTask_Info.u32StackSize, s32MstarCachedPoolID);
        if(!_stAudioTranscodeTask[*u32DevIdx].stTask_Info.pStack)
        {
            printf("[%s][%d] encode device(%"DTC_MS_S32_d"), allocate stack fail!!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
            return FALSE;
        }

        //set task param
        memset(_stAudioTranscodeTask[*u32DevIdx].u8Filename, 0, sizeof(_stAudioTranscodeTask[*u32DevIdx].u8Filename));
        memcpy(_stAudioTranscodeTask[*u32DevIdx].u8Filename, u32OutputFilename, strlen((char*)u32OutputFilename));

        _stAudioTranscodeTask[*u32DevIdx].s32DeviceId = *u32DevIdx;
        _stAudioTranscodeTask[*u32DevIdx].enTranscodeFormat = *enOutputFormat;
        _stAudioTranscodeTask[*u32DevIdx].enTranscodePath = *enOutputPath;
        _stAudioTranscodeTask[*u32DevIdx].bOutFileAppend = *bOutFileAppend;

        _stAudioTranscodeTask[*u32DevIdx].stTask_Info.iId = MsOS_CreateTask((TaskEntry) _appAudio_Transcode_task,
                                            (MS_VIRT)(&_stAudioTranscodeTask[*u32DevIdx]),
                                            _stAudioTranscodeTask[*u32DevIdx].stTask_Info.ePriority,
                                            TRUE,
                                            _stAudioTranscodeTask[*u32DevIdx].stTask_Info.pStack,
                                            _stAudioTranscodeTask[*u32DevIdx].stTask_Info.u32StackSize,
                                            _stAudioTranscodeTask[*u32DevIdx].stTask_Info.szName);

        if(_stAudioTranscodeTask[*u32DevIdx].stTask_Info.iId < 0)
        {
            printf("[%s][%d] encode device(%"DTC_MS_S32_d"), create task fail!!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        printf("[%s][%d] encode device(%"DTC_MS_S32_d"), encode task already exist!!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        _stAudioTranscodeTask[*u32DevIdx].enTaskStatus = AUDIO_MONITOR_TASK_START;
        return TRUE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop Audio transcoding data
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Transcode_Stop 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Transcode_Stop(MS_U32 *u32DevIdx)
{
    if (Demo_Audio_Stop(*u32DevIdx, AUDIO_APP_TSP_FRAMEDECODE) == FALSE)
    {
        printf("[%s][%d] Device (%"DTC_MS_U32_u") Demo Audio Stop fail !!!\n", __FUNCTION__, __LINE__, *u32DevIdx);
        return FALSE;
    }
    _stAudioTranscodeTask[*u32DevIdx].enTaskStatus = AUDIO_MONITOR_TASK_STOP;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Pause Audio transcoding data
/// @param[in] u32EncIdx
///     0: encode device 1
///     1: encode device 2
///     2: encode device 3
///     3: encode device 4
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Transcode_Pause 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Transcode_Pause(MS_U32 *u32DevIdx)
{
    _stAudioTranscodeTask[*u32DevIdx].enTaskStatus = AUDIO_MONITOR_TASK_PAUSE;
    return TRUE;
}

#endif

//------------------------------------------------------------------------------
/// @brief Start Audio test monitor
/// @param[in] enType status type
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Test_Monitor_Start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Test_Monitor_Start(AUDIO_DEC_ID* eDecID, MS_U32* u32MonitorType, MS_U32* u32CodecType, MS_U32* u32PlayCmd, MS_U32* u32HDMIPCMMode, MS_U32* u32SPDIFPCMMode, MS_U32* u32Volume)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    _audioTestMonitor_Task.eDecID = *eDecID;
    _audioTestMonitor_Task.u32MonitorType1 = *u32MonitorType;
    _audioTestMonitor_Task.u32CodecType = *u32CodecType;
    _audioTestMonitor_Task.u32PlayCmd = *u32PlayCmd;
    _audioTestMonitor_Task.u32HDMIPCMMode = *u32HDMIPCMMode;
    _audioTestMonitor_Task.u32SPDIFPCMMode = *u32SPDIFPCMMode;
    _audioTestMonitor_Task.u32Volume = *u32Volume;

    if(_audioTestMonitor_Task.stAudioTask_Info.stTask_Info.iId== -1)
    {
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

        _audioTestMonitor_Task.stAudioTask_Info.stTask_Info.pStack = MsOS_AllocateMemory(_audioTestMonitor_Task.stAudioTask_Info.stTask_Info.u32StackSize, s32MstarCachedPoolID);
        if(!_audioTestMonitor_Task.stAudioTask_Info.stTask_Info.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }

        _audioTestMonitor_Task.stAudioTask_Info.enTaskStatus = AUDIO_MONITOR_TASK_START;

        _audioTestMonitor_Task.stAudioTask_Info.stTask_Info.iId = MsOS_CreateTask((TaskEntry) _appAudioTestMonitor_task,
                                            (MS_VIRT)(&_audioTestMonitor_Task),
                                            _audioTestMonitor_Task.stAudioTask_Info.stTask_Info.ePriority,
                                            TRUE,
                                            _audioTestMonitor_Task.stAudioTask_Info.stTask_Info.pStack,
                                            _audioTestMonitor_Task.stAudioTask_Info.stTask_Info.u32StackSize,
                                            _audioTestMonitor_Task.stAudioTask_Info.stTask_Info.szName);

        if(_audioTestMonitor_Task.stAudioTask_Info.stTask_Info.iId < 0)
        {
            printf("create task fail \n");
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Stop Audio test monitor
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Test_Monitor_Stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Test_Monitor_Stop(void)
{
    _audioTestMonitor_Task.stAudioTask_Info.enTaskStatus = AUDIO_MONITOR_TASK_STOP;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Show all demo functions of audio
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b HelpAudio \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Help(void)
{
    printf("*************************Audio function List*************************\n");
    printf("1.Audio_Init --Initial Audio\n");
    printf("2.Audio_demo --Audio demo MP3\n");
    printf("3.Audio_demoStop -- Stop audio demo\n");
    printf("4.Audio_volume -- Volume control; 0~100\n");
    printf("5.Audio_mute -- mute control\n");


    return TRUE;
}
#endif
