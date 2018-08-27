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

#ifndef _DEMO_AUDIO_COMMON_H_
#define _DEMO_AUDIO_COMMON_H_

#define SE_BASE_ADDR    0x0000000
#define DEC_BASE_ADDR   SE_BASE_ADDR+0x100000
#define DEFAULT_VOLUME 100
#define EDID_MAX_SAD_COUNT  (10)

#define IS_SUPPORT_EDID(x)  (_st_Audio_EDID.u32SupportCodec_bitstring & (1<<x))

typedef enum
{
    AUDIO_MONITOR_OK_FRAME_COUNT            = 0x00000001,
    AUDIO_MONITOR_ERR_FRAME_COUNT           = 0x00000002,
    AUDIO_MONITOR_SKIP_FRAME_COUNT          = 0x00000004,
    AUDIO_MONITOR_REPEAT_FRAME_COUNT        = 0x00000008,
    AUDIO_MONITOR_SE_BUF_EMPTY_COUNT        = 0x00000010,
    AUDIO_MONITOR_HDMI_OUT_PORT             = 0x00000020,
    AUDIO_MONITOR_SPDIF_OUT_PORT            = 0x00000040,
    AUDIO_MONITOR_LINEOUT_OUT_PORT          = 0x00000080,
    AUDIO_MONITOR_HDMI_NONPCM_LEVEL         = 0x00000100,
    AUDIO_MONITOR_SPDIF_NONPCM_LEVEL        = 0x00000200,
    AUDIO_MONITOR_HDMI_PCM_NONPCM_MODE      = 0x00000400,
    AUDIO_MONITOR_SPDIF_PCM_NONPCM_MODE     = 0x00000800,
    AUDIO_MONITOR_HDMI_MUTE_STATUS          = 0x00001000,
    AUDIO_MONITOR_SPDIF_MUTE_STATUS         = 0x00002000,
    AUDIO_MONITOR_HDMI_VOLUME               = 0x00004000,
    AUDIO_MONITOR_SPDIF_VOLUME              = 0x00008000,
    AUDIO_MONITOR_LINEOUT_VOLUME            = 0x00010000,
    AUDIO_MONITOR_MAIN_PCM_LEVEL            = 0x00020000,
    AUDIO_MONITOR_AD_PCM_LEVEL              = 0x00040000,
    AUDIO_MONITOR_PLAYCMD_STATUS            = 0x00080000,
    AUDIO_MONITOR_CODEC_TYPE                = 0x00100000,
    AUDIO_MONITOR_AUDIO_SYNC_STATUS         = 0x00200000,
}EN_AUDIO_MONITOR_STATUS_TYPE;

typedef enum
{
    AUDIO_APP_ES_PLAY = 0,
    AUDIO_APP_DTV,
    AUDIO_APP_DTV_AM,
    AUDIO_APP_DTV_AD,
    AUDIO_APP_DTV_AD_1PID,
    AUDIO_APP_ATV_FMTX,
    AUDIO_APP_ATV_BTSC,
    AUDIO_APP_HDMI_RX,
    AUDIO_APP_PES_PLAY,
    AUDIO_APP_TSP_FRAMEDECODE,
    AUDIO_APP_ALL
}EN_AUDIO_APP_TYPE;

typedef enum
{
    DEMO_AUDIO_CODEC_NONE = 0,
    DEMO_AUDIO_CODEC_MPEG,
    DEMO_AUDIO_CODEC_AC3,
    DEMO_AUDIO_CODEC_AC3P,
    DEMO_AUDIO_CODEC_AAC,
    DEMO_AUDIO_CODEC_MP3,
    DEMO_AUDIO_CODEC_WMA,
    DEMO_AUDIO_CODEC_CDLPCM,
    DEMO_AUDIO_CODEC_RA8LBR,
    DEMO_AUDIO_CODEC_XPCM,
    DEMO_AUDIO_CODEC_TONE,
    DEMO_AUDIO_CODEC_DTS,
    DEMO_AUDIO_CODEC_MS10_DDT,
    DEMO_AUDIO_CODEC_MS10_DDC,
    DEMO_AUDIO_CODEC_WMA_PRO,
    DEMO_AUDIO_CODEC_FLAC,
    DEMO_AUDIO_CODEC_VORBIS = 0x10,
    DEMO_AUDIO_CODEC_DTSLBR,
    DEMO_AUDIO_CODEC_AMR_NB,
    DEMO_AUDIO_CODEC_AMR_WB,
    DEMO_AUDIO_CODEC_DRA,
    DEMO_AUDIO_CODEC_DolbyTrueHDBypass,
    DEMO_AUDIO_CODEC_DTSHDADO,
    DEMO_AUDIO_CODEC_DTSXLL,
    DEMO_AUDIO_CODEC_ES_Bypass,
    DEMO_AUDIO_CODEC_AC4,
    DEMO_AUDIO_CODEC_MPEG_H,

    //For AD
    DEMO_AUDIO_CODEC_AD_NONE,
    DEMO_AUDIO_CODEC_AD_MPEG,
    DEMO_AUDIO_CODEC_AD_AC3,
    DEMO_AUDIO_CODEC_AD_MP3,
    DEMO_AUDIO_CODEC_AD_AAC,
    DEMO_AUDIO_CODEC_AD_GAAC,
    DEMO_AUDIO_CODEC_AD_AC3P,
    DEMO_AUDIO_CODEC_AD_DDE,
    DEMO_AUDIO_CODEC_AD_DTSE,
    DEMO_AUDIO_CODEC_AD_XPCM,
    DEMO_AUDIO_CODEC_AD_KTV,
    DEMO_AUDIO_CODEC_AD_KTV2,
    DEMO_AUDIO_CODEC_AD_DTS,
    DEMO_AUDIO_CODEC_AD_TONEGEN,
    DEMO_AUDIO_CODEC_AD_DolbyTrueHDBypass,
    DEMO_AUDIO_CODEC_AD_DTSHDADO,
    DEMO_AUDIO_CODEC_AD_AC4,
    DEMO_AUDIO_CODEC_AD_MPEG_H,

    //ATV SIF
    DEMO_AUDIO_CODEC_ATV_NONE,
    DEMO_AUDIO_CODEC_ATV_BTSC,
    DEMO_AUDIO_CODEC_ATV_EIAJ,
    DEMO_AUDIO_CODEC_ATV_PAL,
    DEMO_AUDIO_CODEC_ATV_FM_RADIO,
    DEMO_AUDIO_CODEC_ATV_MAX,

    //ATV ENC
    DEMO_AUDIO_CODEC_ATVENC_NONE,
    DEMO_AUDIO_CODEC_ATVENC_FMTX,
    DEMO_AUDIO_CODEC_ATVENC_BTSC,

    DEMO_AUDIO_CODEC_MAX,
    DEMO_AUDIO_CODEC_INVALID = 0xFF,
} EN_AUDIO_CODEC_TYPE;

//follow 3-byte Short Audio Descriptors(SAD) format
typedef enum
{
    DEMO_AUDIO_EDID_CODEC_LPCM = 1,
    DEMO_AUDIO_EDID_CODEC_AC3,      //MPEG1 (Layers 1 and 2)
    DEMO_AUDIO_EDID_CODEC_MPEG1,
    DEMO_AUDIO_EDID_CODEC_MP3,
    DEMO_AUDIO_EDID_CODEC_MPEG2,
    DEMO_AUDIO_EDID_CODEC_AAC,
    DEMO_AUDIO_EDID_CODEC_DTS,
    DEMO_AUDIO_EDID_CODEC_ATRAC,
    DEMO_AUDIO_EDID_CODEC_SACD,     //one-bit audio aka SACD
    DEMO_AUDIO_EDID_CODEC_AC3P,     //aka DDP, DD+, DDPlus, AC3P, EAC3
    DEMO_AUDIO_EDID_CODEC_DTS_HD,
    DEMO_AUDIO_EDID_CODEC_MLP_DOLBY_TRUEHD,
    DEMO_AUDIO_EDID_CODEC_DST_AUDIO,
    DEMO_AUDIO_EDID_CODEC_WMAPRO,
}EN_AUDIO_EDID_CODEC_TYPE;

typedef enum
{
    AUDIO_MONITOR_TASK_STOP,
    AUDIO_MONITOR_TASK_START,
    AUDIO_MONITOR_TASK_PAUSE,
}EN_AUDIO_MONITOR_TASK_STATUS;

typedef enum
{
    DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_FILE,
    DEMO_AUDIO_PCM_CAPTURE_OUTPUT_PATH_MEM,
}EN_AUDIO_PCM_CAPTURE_OUTPUT_PATH;

typedef enum
{
    DEMO_AUDIO_TRANSCODE_PATH_FILE,
    DEMO_AUDIO_TRANSCODE_PATH_MEM,
}EN_AUDIO_TRANSCODE_PATH;

typedef enum
{
    DEMO_AUDIO_TRANSCODE_FORMAT_PCM,
    DEMO_AUDIO_TRANSCODE_FORMAT_ES,
    DEMO_AUDIO_TRANSCODE_FORMAT_PES,
}EN_AUDIO_TRANSCODE_FORMAT;

typedef struct
{
    MS_U8 u8Filter;
    MS_U16 u16Attenuation;
    MS_U16 u16OutScaling;
}ST_AUDIO_FMTX_PARAM;

typedef struct
{
    MS_U8 u8Filter;
    MS_U16 u16Attenuation;
    MS_U16 u16OutScaling;
    MS_U16 u16_M_Gain;
    MS_U16 u16_D_Gain;
    MS_U16 u16_SAP_Gain;
}ST_AUDIO_BTSC_PARAM;

typedef struct
{
    EN_AUDIO_EDID_CODEC_TYPE enCodec;   //Audio codec
    MS_U8 u8ChannelNum; //number of channel
    MS_U8 u8SampleRate; // bit 7: Reserved (0)
                         //bit 6: 192kHz
                         //bit 5: 176kHz
                         //bit 4: 96kHz
                         //bit 3: 88kHz
                         //bit 2: 48kHz
                         //bit 1: 44kHz
                         //bit 0: 32kHz
    MS_U8 u8bitRate; // For LPCM, bits 7:3 are reserved and the remaining bits define bit depth
                      //    bit 2: 24 bit
                      //    bit 1: 20 bit
                      //    bit 0: 16 bit
                      //For all other sound formats, bits 7..0 designate the maximum supported bitrate divided by 8 kbit/s.
                      //For DD+ codec, if bit 0
                      //    0: Dolby Digital Plus without Dolby Atmos support
                      //    1: Dolby Atmos in Dolby Digital Plus

}ST_AUDIO_EDID_SAD;

typedef struct
{
    MS_U32 u32NumOfSAD;
    MS_U32 u32SupportCodec_bitstring; // bit0, 15: Reserved
                                       //  bit1 = Linear Pulse Code Modulation (LPCM)
                                       // bit2 = AC-3
                                       // bit3 = MPEG1 (Layers 1 and 2)
                                       // bit4 = MP3
                                       // bit5 = MPEG2
                                       // bit6 = AAC
                                       // bit7 = DTS
                                       // bit8 = ATRAC
                                       // bit9 = One-bit audio aka SACD
                                       // bit10 = DD+
                                       // bit11 = DTS-HD
                                       // bit12 = MLP/Dolby TrueHD
                                       // bit13 = DST Audio
                                       // bit14 = Microsoft WMA Pro
    MS_BOOL bSupport_Dolby_Atmos;
    ST_AUDIO_EDID_SAD stAudioEDID_SADs_t[EDID_MAX_SAD_COUNT];
}ST_AUDIO_EDID;

typedef struct
{
    char field_name[30];
    MS_U32 u32field_offset;
    MS_U32 u32field_value_Count;
}ST_DAP_List;

#define DAP_MAX_BANDS          (20)
#define DAP_IEQ_MAX_BANDS      DAP_MAX_BANDS
#define DAP_GEQ_MAX_BANDS      DAP_MAX_BANDS
#define DAP_REG_MAX_BANDS      DAP_MAX_BANDS
#define DAP_OPT_MAX_BANDS      DAP_MAX_BANDS
#define DAP_MAX_CHANNELS       6

/**
 * @brief See dap_cpdp.h in the DAPv2 CIDK for a detailed description of the parameter.
 */
typedef struct dap_params_s
{
    int pregain;
    int postgain;
    int systemgain;
    int surround_decoder_enable;
    int virtualizer_enable;
    int headphone_reverb;
    int speaker_angle;
    int speaker_start;
    int surround_boost;
    int mi_ieq_enable;
    int mi_dv_enable;
    int mi_de_enable;
    int mi_surround_enable;

    int calibration_boost;

    int leveler_amount;
    int leveler_input;
    int leveler_output;
    int leveler_enable;

    int modeler_enable;
    int modeler_calibration;

    int ieq_enable;
    int ieq_amount;
    int ieq_nb_bands;
    int a_ieq_band_center[DAP_IEQ_MAX_BANDS];
    int a_ieq_band_target[DAP_IEQ_MAX_BANDS];

    int de_enable;
    int de_amount;
    int de_ducking;

    int volmax_boost;

    int geq_enable;
    int geq_nb_bands;
    int a_geq_band_center[DAP_GEQ_MAX_BANDS];
    int a_geq_band_target[DAP_GEQ_MAX_BANDS];

    int optimizer_enable;
    int optimizer_nb_bands;
    int a_opt_band_center_freq[DAP_OPT_MAX_BANDS];
    int a_opt_band_gain[DAP_MAX_CHANNELS][DAP_OPT_MAX_BANDS];

    int bass_enable;
    int bass_boost;
    int bass_cutoff;
    int bass_width;

    int reg_nb_bands;
    int a_reg_band_center[DAP_REG_MAX_BANDS];
    int a_reg_low_thresholds[DAP_REG_MAX_BANDS];
    int a_reg_high_thresholds[DAP_REG_MAX_BANDS];
    int a_reg_isolated_bands[DAP_REG_MAX_BANDS];

    int regulator_overdrive;
    int regulator_timbre;
    int regulator_distortion;
    int regulator_mode;
    int regulator_enable;
    int virtual_bass_mode;
    int virtual_bass_low_src_freq;
    int virtual_bass_high_src_freq;
    int virtual_bass_overall_gain;
    int virtual_bass_slope_gain;
    int virtual_bass_subgain[3];
    int virtual_bass_mix_low_freq;
    int virtual_bass_mix_high_freq;
    int virtual_bass_enable;
    int reserved[16];
} dap_params_t;


#endif


