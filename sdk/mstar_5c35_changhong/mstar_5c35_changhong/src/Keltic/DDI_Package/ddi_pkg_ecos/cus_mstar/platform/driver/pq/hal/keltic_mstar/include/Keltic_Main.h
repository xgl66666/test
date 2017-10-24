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
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//****************************************************
//   Quality Map CodeGen Version 4.0
// 1920
// Keltic
// 2013/6/6 14:29
// 2008/7/1 10:31
//****************************************************

#ifndef _KELTIC_MAIN_H_
#define _KELTIC_MAIN_H_

#define PQ_IP_NUM_Main 54
#define PQ_IP_SRAM1_SIZE_Main 320
#define PQ_IP_SRAM2_SIZE_Main 320
#define PQ_IP_C_SRAM1_SIZE_Main 320
#define PQ_IP_C_SRAM2_SIZE_Main 320

typedef enum
{
    #if PQ_QM_CVBS
    QM_RF_NTSC_44_Main, //0
    #endif
    #if PQ_QM_CVBS
    QM_RF_NTSC_M_Main, //1
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_BGHI_Main, //2
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_60_Main, //3
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_M_Main, //4
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_N_Main, //5
    #endif
    #if PQ_QM_CVBS
    QM_RF_SECAM_Main, //6
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_44_Main, //7
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_M_Main, //8
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_BGHI_Main, //9
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_60_Main, //10
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_M_Main, //11
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_N_Main, //12
    #endif
    #if PQ_QM_CVBS
    QM_VIF_SECAM_Main, //13
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_44_Main, //14
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_M_Main, //15
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_BGHI_Main, //16
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_60_Main, //17
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_M_Main, //18
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_N_Main, //19
    #endif
    #if PQ_QM_CVBS
    QM_SV_SECAM_Main, //20
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_44_Main, //21
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_M_Main, //22
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_BGHI_Main, //23
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_60_Main, //24
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_M_Main, //25
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_N_Main, //26
    #endif
    #if PQ_QM_CVBS
    QM_AV_SECAM_Main, //27
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_44_Main, //28
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_M_Main, //29
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_BGHI_Main, //30
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_60_Main, //31
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_M_Main, //32
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_N_Main, //33
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_SECAM_Main, //34
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_44_Main, //35
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_M_Main, //36
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_BGHI_Main, //37
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_60_Main, //38
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_M_Main, //39
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_N_Main, //40
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_SECAM_Main, //41
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_NTSC_Main, //42
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_PAL_Main, //43
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480i_Main, //44
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576i_Main, //45
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480p_Main, //46
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576p_Main, //47
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_24hz_Main, //48
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_50hz_Main, //49
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_60hz_Main, //50
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_50hz_Main, //51
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_60hz_Main, //52
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_24hz_Main, //53
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_50hz_Main, //54
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_60hz_Main, //55
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480i_Main, //56
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576i_Main, //57
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480p_Main, //58
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576p_Main, //59
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_24hz_Main, //60
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_50hz_Main, //61
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_60hz_Main, //62
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_50hz_Main, //63
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_60hz_Main, //64
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_24hz_Main, //65
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_60hz_Main, //66
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_50hz_Main, //67
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vup_Main, //68
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vdown_Main, //69
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vno_Main, //70
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vup_Main, //71
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vdown_Main, //72
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vno_Main, //73
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vup_Main, //74
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vdown_Main, //75
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_Main, //76
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vup_Main, //77
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vdown_Main, //78
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vno_Main, //79
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vup_Main, //80
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vdown_Main, //81
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vno_Main, //82
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vup_Main, //83
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vdown_Main, //84
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vno_Main, //85
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Main, //86
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Main, //87
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Main, //88
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Main, //89
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Main, //90
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Main, //91
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Main, //92
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Main, //93
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main, //94
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_MPEG2_Main, //95
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_MPEG2_Main, //96
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_MPEG2_Main, //97
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_MPEG2_Main, //98
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_MPEG2_Main, //99
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_MPEG2_Main, //100
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_MPEG2_Main, //101
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_MPEG2_Main, //102
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_MPEG2_Main, //103
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_MPEG2_Main, //104
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_MPEG2_Main, //105
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_MPEG2_Main, //106
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_MPEG2_Main, //107
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_H264_Main, //108
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_H264_Main, //109
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_H264_Main, //110
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_H264_Main, //111
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_H264_Main, //112
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_H264_Main, //113
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_H264_Main, //114
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_H264_Main, //115
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_H264_Main, //116
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_H264_Main, //117
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_H264_Main, //118
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_H264_Main, //119
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_H264_Main, //120
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_interlace_Main, //121
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_Main, //122
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_interlace_Main, //123
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_Main, //124
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_SD_progressive_Main, //125
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_HD_progressive_Main, //126
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_interlace_Main, //127
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_progressive_Main, //128
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_interlace_Main, //129
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_progressive_Main, //130
    #endif
    QM_INPUTTYPE_NUM_Main, // 131
} SC_QUALITY_MAP_INDEX_e_Main;

typedef enum
{
PQ_IP_AFEC_Main,  //0
PQ_IP_Comb_Main,  //1
PQ_IP_SECAM_Main,  //2
PQ_IP_SCinit_Main,  //3
PQ_IP_CSC_Main,  //4
PQ_IP_CSC_Dither_Main,  //5
PQ_IP_IP422To444_Main,  //6
PQ_IP_HSD_Y_Main,  //7
PQ_IP_HSD_C_Main,  //8
PQ_IP_VSD_Main,  //9
PQ_IP_CTI_Main,  //10
PQ_IP_MemFormat_Main,  //11
PQ_IP_444To422_Main,  //12
PQ_IP_PreSNR_Main,  //13
PQ_IP_DNR_Main,  //14
PQ_IP_DNR_YOnly_Main,  //15
PQ_IP_DNR_Y_Main,  //16
PQ_IP_DNR_MED_Main,  //17
PQ_IP_DNR_C_Main,  //18
PQ_IP_PostCCS_Main,  //19
PQ_IP_PNR_Main,  //20
PQ_IP_PNR_Y_Main,  //21
PQ_IP_PNR_C_Main,  //22
PQ_IP_420CUP_Main,  //23
PQ_IP_MADi_Main,  //24
PQ_IP_EODi_Main,  //25
PQ_IP_Film_Main,  //26
PQ_IP_Film22_Main,  //27
PQ_IP_Film32_Main,  //28
PQ_IP_SPF_Main,  //29
PQ_IP_SPF_DBK_Main,  //30
PQ_IP_SPF_DBK_MR_Main,  //31
PQ_IP_SPF_DBK_BKN_Main,  //32
PQ_IP_SPF_MR_LPF_Main,  //33
PQ_IP_VSP_Y_Main,  //34
PQ_IP_VSP_C_Main,  //35
PQ_IP_VSP_Coring_Main,  //36
PQ_IP_VSP_Dither_Main,  //37
PQ_IP_VSP_PreVBound_Main,  //38
PQ_IP_HSP_Y_Main,  //39
PQ_IP_HSP_C_Main,  //40
PQ_IP_HSP_Coring_Main,  //41
PQ_IP_HSP_Dither_Main,  //42
PQ_IP_HnonLinear_Main,  //43
PQ_IP_SRAM1_Main,  //44
PQ_IP_SRAM2_Main,  //45
PQ_IP_C_SRAM1_Main,  //46
PQ_IP_C_SRAM2_Main,  //47
PQ_IP_422To444_Main,  //48
PQ_IP_Peaking_Main,  //49
PQ_IP_SwDriver_Main,  //50
PQ_IP_3x3_Main,  //51
PQ_IP_BRI_CR_Main,  //52
PQ_IP_Display_Main,  //53
 }   PQ_IPTYPE_Main;

typedef enum
{
PQ_IP_AFEC_COM_Main,  //0
PQ_IP_Comb_COM_Main,  //1
PQ_IP_SECAM_COM_Main,  //2
PQ_IP_SCinit_COM_Main,  //3
PQ_IP_CSC_COM_Main,  //4
PQ_IP_CSC_Dither_COM_Main,  //5
PQ_IP_IP422To444_COM_Main,  //6
PQ_IP_HSD_Y_COM_Main,  //7
PQ_IP_HSD_C_COM_Main,  //8
PQ_IP_VSD_COM_Main,  //9
PQ_IP_CTI_COM_Main,  //10
PQ_IP_MemFormat_COM_Main,  //11
PQ_IP_444To422_COM_Main,  //12
PQ_IP_PreSNR_COM_Main,  //13
PQ_IP_DNR_COM_Main,  //14
PQ_IP_DNR_YOnly_COM_Main,  //15
PQ_IP_DNR_Y_COM_Main,  //16
PQ_IP_DNR_MED_COM_Main,  //17
PQ_IP_DNR_C_COM_Main,  //18
PQ_IP_PostCCS_COM_Main,  //19
PQ_IP_PNR_COM_Main,  //20
PQ_IP_PNR_Y_COM_Main,  //21
PQ_IP_PNR_C_COM_Main,  //22
PQ_IP_420CUP_COM_Main,  //23
PQ_IP_MADi_COM_Main,  //24
PQ_IP_EODi_COM_Main,  //25
PQ_IP_Film_COM_Main,  //26
PQ_IP_Film22_COM_Main,  //27
PQ_IP_Film32_COM_Main,  //28
PQ_IP_SPF_COM_Main,  //29
PQ_IP_SPF_DBK_COM_Main,  //30
PQ_IP_SPF_DBK_MR_COM_Main,  //31
PQ_IP_SPF_DBK_BKN_COM_Main,  //32
PQ_IP_SPF_MR_LPF_COM_Main,  //33
PQ_IP_VSP_Y_COM_Main,  //34
PQ_IP_VSP_C_COM_Main,  //35
PQ_IP_VSP_Coring_COM_Main,  //36
PQ_IP_VSP_Dither_COM_Main,  //37
PQ_IP_VSP_PreVBound_COM_Main,  //38
PQ_IP_HSP_Y_COM_Main,  //39
PQ_IP_HSP_C_COM_Main,  //40
PQ_IP_HSP_Coring_COM_Main,  //41
PQ_IP_HSP_Dither_COM_Main,  //42
PQ_IP_HnonLinear_COM_Main,  //43
PQ_IP_SRAM1_COM_Main,  //44
PQ_IP_SRAM2_COM_Main,  //45
PQ_IP_C_SRAM1_COM_Main,  //46
PQ_IP_C_SRAM2_COM_Main,  //47
PQ_IP_422To444_COM_Main,  //48
PQ_IP_Peaking_COM_Main,  //49
PQ_IP_SwDriver_COM_Main,  //50
PQ_IP_3x3_COM_Main,  //51
PQ_IP_BRI_CR_COM_Main,  //52
PQ_IP_Display_COM_Main,  //53
PQ_IP_COM_NUMS_Main
} PQ_IP_COM_Group_Main;

typedef enum
{
PQ_IP_AFEC_ADCIN_Main,
PQ_IP_AFEC_RFIN_Main,
PQ_IP_AFEC_ADCIN_SECAM_Main,
PQ_IP_AFEC_NUMS_Main
} PQ_IP_AFEC_Group_Main;

typedef enum
{
PQ_IP_Comb_NTSC_AV_Main,
PQ_IP_Comb_PAL_AV_Main,
PQ_IP_Comb_NTSC_RF_Main,
PQ_IP_Comb_PAL_RF_Main,
PQ_IP_Comb_NTSC_SV_Main,
PQ_IP_Comb_PAL_SV_Main,
PQ_IP_Comb_NTSC443_Main,
PQ_IP_Comb_PAL60_Main,
PQ_IP_Comb_SECAM_Main,
PQ_IP_Comb_NUMS_Main
} PQ_IP_Comb_Group_Main;

typedef enum
{
PQ_IP_SECAM_ALL_Main,
PQ_IP_SECAM_NUMS_Main
} PQ_IP_SECAM_Group_Main;

typedef enum
{
PQ_IP_SCinit_Init_Main,
PQ_IP_SCinit_NUMS_Main
} PQ_IP_SCinit_Group_Main;

typedef enum
{
PQ_IP_CSC_OFF_Main,
PQ_IP_CSC_L_RGB2YCC_SD_Main,
PQ_IP_CSC_F_RGB2YCC_SD_Main,
PQ_IP_CSC_L_RGB2YCC_HD_Main,
PQ_IP_CSC_F_RGB2YCC_HD_Main,
PQ_IP_CSC_NUMS_Main
} PQ_IP_CSC_Group_Main;

typedef enum
{
PQ_IP_CSC_Dither_OFF_Main,
PQ_IP_CSC_Dither_ON_Main,
PQ_IP_CSC_Dither_NUMS_Main
} PQ_IP_CSC_Dither_Group_Main;

typedef enum
{
PQ_IP_IP422To444_OFF_Main,
PQ_IP_IP422To444_ON_Main,
PQ_IP_IP422To444_Bypass_Main,
PQ_IP_IP422To444_NUMS_Main
} PQ_IP_IP422To444_Group_Main;

typedef enum
{
PQ_IP_HSD_Y_OFF_Main,
PQ_IP_HSD_Y_CB_Main,
PQ_IP_HSD_Y_NUMS_Main
} PQ_IP_HSD_Y_Group_Main;

typedef enum
{
PQ_IP_HSD_C_OFF_Main,
PQ_IP_HSD_C_CB_Main,
PQ_IP_HSD_C_NUMS_Main
} PQ_IP_HSD_C_Group_Main;

typedef enum
{
PQ_IP_VSD_OFF_Main,
PQ_IP_VSD_CB_Main,
PQ_IP_VSD_Bilinear_Main,
PQ_IP_VSD_NUMS_Main
} PQ_IP_VSD_Group_Main;

typedef enum
{
PQ_IP_CTI_OFF_Main,
PQ_IP_CTI_SD_Main,
PQ_IP_CTI_HD_Main,
PQ_IP_CTI_NUMS_Main
} PQ_IP_CTI_Group_Main;

typedef enum
{
PQ_IP_MemFormat_422MF_Main,
PQ_IP_MemFormat_444_10BIT_Main,
PQ_IP_MemFormat_444_8BIT_Main,
PQ_IP_MemFormat_422FBL_10BIT_Main,
PQ_IP_MemFormat_444FBL_10BIT_Main,
PQ_IP_MemFormat_444FBL_8BIT_Main,
PQ_IP_MemFormat_NUMS_Main
} PQ_IP_MemFormat_Group_Main;

typedef enum
{
PQ_IP_444To422_OFF_Main,
PQ_IP_444To422_ON_Main,
PQ_IP_444To422_NUMS_Main
} PQ_IP_444To422_Group_Main;

typedef enum
{
PQ_IP_PreSNR_OFF_Main,
PQ_IP_PreSNR_PS_1_Main,
PQ_IP_PreSNR_PS_2_Main,
PQ_IP_PreSNR_PS_3_Main,
PQ_IP_PreSNR_PS_4_Main,
PQ_IP_PreSNR_PS_5_Main,
PQ_IP_PreSNR_PS_6_Main,
PQ_IP_PreSNR_PS_7_Main,
PQ_IP_PreSNR_PS_8_Main,
PQ_IP_PreSNR_PS_9_Main,
PQ_IP_PreSNR_PS_10_Main,
PQ_IP_PreSNR_PS_11_Main,
PQ_IP_PreSNR_PS_12_Main,
PQ_IP_PreSNR_PS_13_Main,
PQ_IP_PreSNR_PS_14_Main,
PQ_IP_PreSNR_PS_15_Main,
PQ_IP_PreSNR_NUMS_Main
} PQ_IP_PreSNR_Group_Main;

typedef enum
{
PQ_IP_DNR_NR_OFF_Main,
PQ_IP_DNR_DNR_OFF_Main,
PQ_IP_DNR_ON_Main,
PQ_IP_DNR_NUMS_Main
} PQ_IP_DNR_Group_Main;

typedef enum
{
PQ_IP_DNR_YOnly_OFF_Main,
PQ_IP_DNR_YOnly_ON_Main,
PQ_IP_DNR_YOnly_NUMS_Main
} PQ_IP_DNR_YOnly_Group_Main;

typedef enum
{
PQ_IP_DNR_Y_OFF_Main,
PQ_IP_DNR_Y_DY_1_Main,
PQ_IP_DNR_Y_DY_2_Main,
PQ_IP_DNR_Y_DY_3_Main,
PQ_IP_DNR_Y_DY_4_Main,
PQ_IP_DNR_Y_DY_5_Main,
PQ_IP_DNR_Y_NUMS_Main
} PQ_IP_DNR_Y_Group_Main;

typedef enum
{
PQ_IP_DNR_MED_OFF_Main,
PQ_IP_DNR_MED_M_1_Main,
PQ_IP_DNR_MED_NUMS_Main
} PQ_IP_DNR_MED_Group_Main;

typedef enum
{
PQ_IP_DNR_C_OFF_Main,
PQ_IP_DNR_C_DC_1_Main,
PQ_IP_DNR_C_DC_2_Main,
PQ_IP_DNR_C_DC_3_Main,
PQ_IP_DNR_C_DC_4_Main,
PQ_IP_DNR_C_DC_5_Main,
PQ_IP_DNR_C_NUMS_Main
} PQ_IP_DNR_C_Group_Main;

typedef enum
{
PQ_IP_PostCCS_OFF_Main,
PQ_IP_PostCCS_PC_0_Main,
PQ_IP_PostCCS_PC_1_Main,
PQ_IP_PostCCS_PC_2_Main,
PQ_IP_PostCCS_PC_3_Main,
PQ_IP_PostCCS_PC_4_Main,
PQ_IP_PostCCS_PC_5_Main,
PQ_IP_PostCCS_NUMS_Main
} PQ_IP_PostCCS_Group_Main;

typedef enum
{
PQ_IP_PNR_OFF_Main,
PQ_IP_PNR_ON_Main,
PQ_IP_PNR_AVG_ON_Main,
PQ_IP_PNR_NUMS_Main
} PQ_IP_PNR_Group_Main;

typedef enum
{
PQ_IP_PNR_Y_OFF_Main,
PQ_IP_PNR_Y_PY1_Main,
PQ_IP_PNR_Y_NUMS_Main
} PQ_IP_PNR_Y_Group_Main;

typedef enum
{
PQ_IP_PNR_C_OFF_Main,
PQ_IP_PNR_C_PC1_Main,
PQ_IP_PNR_C_NUMS_Main
} PQ_IP_PNR_C_Group_Main;

typedef enum
{
PQ_IP_420CUP_OFF_Main,
PQ_IP_420CUP_ON_Main,
PQ_IP_420CUP_NUMS_Main
} PQ_IP_420CUP_Group_Main;

typedef enum
{
    PQ_IP_MADi_25_4R_Main,
    PQ_IP_MADi_24_4R_Main,
    PQ_IP_MADi_25_2R_Main,
    PQ_IP_MADi_24_2R_Main,
    PQ_IP_MADi_27_4R_Main,
    PQ_IP_MADi_27_2R_Main,
    PQ_IP_MADi_27_2R_Y8Only_Main,
    PQ_IP_MADi_FBL_EODi_Main,
    PQ_IP_MADi_P_MODE10_Main,
    PQ_IP_MADi_P_MODE8_Main,
    PQ_IP_MADi_24_RFBL_NFilm_Main,
    PQ_IP_MADi_24_RFBL_Film_Main,
    PQ_IP_MADi_FBL_DNR_Main,
    PQ_IP_MADi_FBL_MIU_Main,
    PQ_IP_MADi_P_MODE8_NO_MIU_Main,
    PQ_IP_MADi_P_MODE8_3Frame_Main,
    PQ_IP_MADi_24_4R_P_Main,
    PQ_IP_MADi_25_4R_MC_Main,
    PQ_IP_MADi_26_4R_Main,
    PQ_IP_MADi_26_2R_Main,
    PQ_IP_MADi_P_MODE_MOT8_Main,
    PQ_IP_MADi_P_MODE_MOT10_Main,
    PQ_IP_MADi_24_4R_880_Main,
    PQ_IP_MADi_24_2R_880_Main,
    PQ_IP_MADi_25_4R_MC_NW_Main,
    PQ_IP_MADi_25_6R_MC_Main,
    PQ_IP_MADi_25_4R_884_Main,
    PQ_IP_MADi_25_4R_880_Main,
    PQ_IP_MADi_25_2R_884_Main,
    PQ_IP_MADi_25_2R_880_Main,
    PQ_IP_MADi_25_6R_MC_NW_Main,
    PQ_IP_MADi_25_12F_8R_MC_Main,
    PQ_IP_MADi_25_14F_8R_MC_Main,
    PQ_IP_MADi_25_16F_8R_MC_Main,
    PQ_IP_MADi_24_2R_P_Main,
    PQ_IP_MADi_NUMS_Main
} PQ_IP_MADi_Group_Main;

typedef enum
{
PQ_IP_EODi_OFF_Main,
PQ_IP_EODi_SD_Main,
PQ_IP_EODi_NUMS_Main
} PQ_IP_EODi_Group_Main;

typedef enum
{
PQ_IP_Film_OFF_Main,
PQ_IP_Film_SD_2_Main,
PQ_IP_Film_SD_1_Main,
PQ_IP_Film_SD_3_Main,
PQ_IP_Film_HD_2_Main,
PQ_IP_Film_HD_1_Main,
PQ_IP_Film_HD_3_Main,
PQ_IP_Film_NUMS_Main
} PQ_IP_Film_Group_Main;

typedef enum
{
PQ_IP_Film22_OFF_Main,
PQ_IP_Film22_SD_1_Main,
PQ_IP_Film22_SD_2_Main,
PQ_IP_Film22_SD_3_Main,
PQ_IP_Film22_SD_4_Main,
PQ_IP_Film22_SD_5_Main,
PQ_IP_Film22_SD_6_Main,
PQ_IP_Film22_SD_7_Main,
PQ_IP_Film22_SD_8_Main,
PQ_IP_Film22_SD_9_Main,
PQ_IP_Film22_SD_10_Main,
PQ_IP_Film22_HD_1_Main,
PQ_IP_Film22_HD_2_Main,
PQ_IP_Film22_HD_3_Main,
PQ_IP_Film22_HD_4_Main,
PQ_IP_Film22_HD_5_Main,
PQ_IP_Film22_HD_6_Main,
PQ_IP_Film22_NUMS_Main
} PQ_IP_Film22_Group_Main;

typedef enum
{
PQ_IP_Film32_OFF_Main,
PQ_IP_Film32_SD_1_Main,
PQ_IP_Film32_SD_2_Main,
PQ_IP_Film32_SD_3_Main,
PQ_IP_Film32_SD_4_Main,
PQ_IP_Film32_SD_5_Main,
PQ_IP_Film32_SD_6_Main,
PQ_IP_Film32_SD_7_Main,
PQ_IP_Film32_SD_8_Main,
PQ_IP_Film32_SD_9_Main,
PQ_IP_Film32_SD_10_Main,
PQ_IP_Film32_HD_1_Main,
PQ_IP_Film32_HD_2_Main,
PQ_IP_Film32_HD_3_Main,
PQ_IP_Film32_HD_4_Main,
PQ_IP_Film32_HD_5_Main,
PQ_IP_Film32_HD_6_Main,
PQ_IP_Film32_NUMS_Main
} PQ_IP_Film32_Group_Main;

typedef enum
{
PQ_IP_SPF_OFF_Main,
PQ_IP_SPF_ON_Main,
PQ_IP_SPF_NUMS_Main
} PQ_IP_SPF_Group_Main;

typedef enum
{
PQ_IP_SPF_DBK_OFF_Main,
PQ_IP_SPF_DBK_E2S2_Main,
PQ_IP_SPF_DBK_NUMS_Main
} PQ_IP_SPF_DBK_Group_Main;

typedef enum
{
PQ_IP_SPF_DBK_MR_OFF_Main,
PQ_IP_SPF_DBK_MR_ON_Main,
PQ_IP_SPF_DBK_MR_NUMS_Main
} PQ_IP_SPF_DBK_MR_Group_Main;

typedef enum
{
PQ_IP_SPF_DBK_BKN_OFF_Main,
PQ_IP_SPF_DBK_BKN_T0_Main,
PQ_IP_SPF_DBK_BKN_T1_Main,
PQ_IP_SPF_DBK_BKN_T2_Main,
PQ_IP_SPF_DBK_BKN_AV_Main,
PQ_IP_SPF_DBK_BKN_2xsampling_Main,
PQ_IP_SPF_DBK_BKN_NUMS_Main
} PQ_IP_SPF_DBK_BKN_Group_Main;

typedef enum
{
PQ_IP_SPF_MR_LPF_OFF_Main,
PQ_IP_SPF_MR_LPF_LPF3x3_Main,
PQ_IP_SPF_MR_LPF_NUMS_Main
} PQ_IP_SPF_MR_LPF_Group_Main;

typedef enum
{
PQ_IP_VSP_Y_Bypass_Main,
PQ_IP_VSP_Y_Bilinear_Main,
PQ_IP_VSP_Y_SRAM_1_3Tap_Main,
PQ_IP_VSP_Y_SRAM_2_3Tap_Main,
PQ_IP_VSP_Y_SRAM_1_4Tap_Main,
PQ_IP_VSP_Y_SRAM_2_4Tap_Main,
PQ_IP_VSP_Y_NUMS_Main
} PQ_IP_VSP_Y_Group_Main;

typedef enum
{
PQ_IP_VSP_C_Bypass_Main,
PQ_IP_VSP_C_Bilinear_Main,
PQ_IP_VSP_C_C_SRAM_1_Main,
PQ_IP_VSP_C_C_SRAM_2_Main,
PQ_IP_VSP_C_SRAM_1_3Tap_Main,
PQ_IP_VSP_C_SRAM_2_3Tap_Main,
PQ_IP_VSP_C_SRAM_1_4Tap_Main,
PQ_IP_VSP_C_SRAM_2_4Tap_Main,
PQ_IP_VSP_C_NUMS_Main
} PQ_IP_VSP_C_Group_Main;

typedef enum
{
PQ_IP_VSP_Coring_OFF_Main,
PQ_IP_VSP_Coring_Y_Coring_1_Main,
PQ_IP_VSP_Coring_Y_Coring_2_Main,
PQ_IP_VSP_Coring_Y_Coring_3_Main,
PQ_IP_VSP_Coring_Y_Coring_4_Main,
PQ_IP_VSP_Coring_Y_Coring_5_Main,
PQ_IP_VSP_Coring_NUMS_Main
} PQ_IP_VSP_Coring_Group_Main;

typedef enum
{
PQ_IP_VSP_Dither_OFF_Main,
PQ_IP_VSP_Dither_ON_Main,
PQ_IP_VSP_Dither_NUMS_Main
} PQ_IP_VSP_Dither_Group_Main;

typedef enum
{
PQ_IP_VSP_PreVBound_OFF_Main,
PQ_IP_VSP_PreVBound_ON_Main,
PQ_IP_VSP_PreVBound_NUMS_Main
} PQ_IP_VSP_PreVBound_Group_Main;

typedef enum
{
PQ_IP_HSP_Y_Bypass_Main,
PQ_IP_HSP_Y_Bilinear_Main,
PQ_IP_HSP_Y_SRAM_1_4Tap_Main,
PQ_IP_HSP_Y_SRAM_2_4Tap_Main,
PQ_IP_HSP_Y_NUMS_Main
} PQ_IP_HSP_Y_Group_Main;

typedef enum
{
PQ_IP_HSP_C_Bypass_Main,
PQ_IP_HSP_C_Bilinear_Main,
PQ_IP_HSP_C_C_SRAM_1_Main,
PQ_IP_HSP_C_C_SRAM_2_Main,
PQ_IP_HSP_C_SRAM_1_4Tap_Main,
PQ_IP_HSP_C_SRAM_2_4Tap_Main,
PQ_IP_HSP_C_NUMS_Main
} PQ_IP_HSP_C_Group_Main;

typedef enum
{
PQ_IP_HSP_Coring_OFF_Main,
PQ_IP_HSP_Coring_Y_Coring_1_Main,
PQ_IP_HSP_Coring_Y_Coring_2_Main,
PQ_IP_HSP_Coring_Y_Coring_3_Main,
PQ_IP_HSP_Coring_Y_Coring_4_Main,
PQ_IP_HSP_Coring_Y_Coring_5_Main,
PQ_IP_HSP_Coring_NUMS_Main
} PQ_IP_HSP_Coring_Group_Main;

typedef enum
{
PQ_IP_HSP_Dither_OFF_Main,
PQ_IP_HSP_Dither_ON_Main,
PQ_IP_HSP_Dither_NUMS_Main
} PQ_IP_HSP_Dither_Group_Main;

typedef enum
{
PQ_IP_HnonLinear_OFF_Main,
PQ_IP_HnonLinear_H_1366_Main,
PQ_IP_HnonLinear_H_1440_Main,
PQ_IP_HnonLinear_H_1680_Main,
PQ_IP_HnonLinear_H_1920_Main,
PQ_IP_HnonLinear_NUMS_Main
} PQ_IP_HnonLinear_Group_Main;

typedef enum
{
PQ_IP_SRAM1_InvSinc4Tc4p4Fc45Apass01Astop40_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc50Apass01Astop55_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50G11_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50G12_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50G13_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc95Fstop148Apass0001Astop40_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc65Apass3Astop60G14_Main,
PQ_IP_SRAM1_InvSinc3Tc0p0Fc75Ap001As60G13_Main,
PQ_IP_SRAM1_InvSinc3Tc0p0Fc75Ap001As60_Main,
PQ_IP_SRAM1_NUMS_Main
} PQ_IP_SRAM1_Group_Main;

typedef enum
{
PQ_IP_SRAM2_InvSinc4Tc4p4Fc45Apass01Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc50Apass01Astop55_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Fstop124Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50G11_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50G12_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Fstop134Apass01Astop50G13_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc95Fstop148Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc65Apass3Astop60G14_Main,
PQ_IP_SRAM2_InvSinc3Tc0p0Fc75Ap001As60G13_Main,
PQ_IP_SRAM2_InvSinc3Tc0p0Fc75Ap001As60_Main,
PQ_IP_SRAM2_NUMS_Main
} PQ_IP_SRAM2_Group_Main;

typedef enum
{
PQ_IP_C_SRAM1_C2121_Main,
PQ_IP_C_SRAM1_C121_Main,
PQ_IP_C_SRAM1_NUMS_Main
} PQ_IP_C_SRAM1_Group_Main;

typedef enum
{
PQ_IP_C_SRAM2_C2121_Main,
PQ_IP_C_SRAM2_C121_Main,
PQ_IP_C_SRAM2_NUMS_Main
} PQ_IP_C_SRAM2_Group_Main;

typedef enum
{
PQ_IP_422To444_ON_Main,
PQ_IP_422To444_OFF_Main,
PQ_IP_422To444_SIMPLE_Main,
PQ_IP_422To444_BICUBIC_Main,
PQ_IP_422To444_Bypass_Main,
PQ_IP_422To444_NUMS_Main
} PQ_IP_422To444_Group_Main;

typedef enum
{
PQ_IP_Peaking_OFF_Main,
PQ_IP_Peaking_PC_mode_Main,
PQ_IP_Peaking_NUMS_Main
} PQ_IP_Peaking_Group_Main;

typedef enum
{
PQ_IP_SwDriver_ALL_Main,
PQ_IP_SwDriver_NUMS_Main
} PQ_IP_SwDriver_Group_Main;

typedef enum
{
PQ_IP_3x3_OFF_Main,
PQ_IP_3x3_NUMS_Main
} PQ_IP_3x3_Group_Main;

typedef enum
{
PQ_IP_BRI_CR_ALL_Main,
PQ_IP_BRI_CR_HDMI_Main,
PQ_IP_BRI_CR_NUMS_Main
} PQ_IP_BRI_CR_Group_Main;

typedef enum
{
PQ_IP_Display_ALL_Main,
PQ_IP_Display_HDMI_Main,
PQ_IP_Display_NUMS_Main
} PQ_IP_Display_Group_Main;

extern code U8 MST_SkipRule_IP_Main[PQ_IP_NUM_Main];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_Main[];
extern code U8 QMAP_1920_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_1366_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];

#endif
