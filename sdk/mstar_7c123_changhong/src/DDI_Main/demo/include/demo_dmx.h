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

#ifndef _DEMO_DMX_H_
#define _DEMO_DMX_H_

#include "MsCommon.h"
#include "apiDMX.h"

//--------------------------------------------------------------------------------------------------
// Enumerate
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_DMX_FLOW_LIVE0,   ///< DMX playback flow
    E_DMX_FLOW_LIVE1,   ///< DMX playback flow from TS1 (version 3.0)
    E_DMX_FLOW_LIVE2,   ///< DMX playback flow from TS2 (version 3.0/4.0)
    E_DMX_FLOW_LIVE3,   ///< DMX playback flow from TS3 (version 4.0)
    E_DMX_FLOW_FILE0,
    E_DMX_FLOW_FILE1,
    E_DMX_FLOW_FILE2,
    E_DMX_FLOW_FILE3,
    E_DMX_FLOW_INVALID,

}EN_DEMO_DMX_FLOW;

typedef enum
{
    E_DMX_FLOW_INPUT_DEMOD0,           ///< DMX input from internal demod
    E_DMX_FLOW_INPUT_DEMOD1,           ///< DMX input from internal demod
    E_DMX_FLOW_INPUT_MEM,              ///< DMX input from memory
    E_DMX_FLOW_INPUT_EXT_INPUT0,       ///< DMX input from external input 0
    E_DMX_FLOW_INPUT_EXT_INPUT1,       ///< DMX input from external input 1
    E_DMX_FLOW_INPUT_EXT_INPUT2,       ///< DMX input from external input 2.
    E_DMX_FLOW_INPUT_EXT_INPUT3,       ///< DMX input from external input 3.
    E_DMX_FLOW_INPUT_EXT_INPUT4,
    E_DMX_FLOW_INPUT_EXT_INPUT5,
    E_DMX_FLOW_INPUT_EXT_INPUT0_3WIRE,
    E_DMX_FLOW_INPUT_EXT_INPUT1_3WIRE,
    E_DMX_FLOW_INPUT_EXT_INPUT2_3WIRE,
    E_DMX_FLOW_INPUT_EXT_INPUT3_3WIRE,
    E_DMX_FLOW_INPUT_INVALID,

} EN_DEMO_DMX_FLOW_INPUT;

typedef enum
{
    E_DMX_FLT_TYPE_VID0,      ///< DMX video filter type
    E_DMX_FLT_TYPE_VID1,      ///< DMX V3D filter type
    E_DMX_FLT_TYPE_AUD0,      ///< DMX audio filter type
    E_DMX_FLT_TYPE_AUD1,      ///< DMX audio 2 filter type
    E_DMX_FLT_TYPE_AUD2,      ///< DMX audio 3 filter type
    E_DMX_FLT_TYPE_AUD3,      ///< DMX audio 4 filter type
    E_DMX_FLT_TYPE_REC,       ///< DMX record filter type no destination needs to be set
    E_DMX_FLT_TYPE_INVALID,

}EN_DEMO_DMX_FLT_TYPE;

typedef enum
{
    E_DMX_FLT_SOURCE_LIVE0,    ///< DMX live stream filter source
    E_DMX_FLT_SOURCE_LIVE1,    ///< DMX live stream filter source 1
    E_DMX_FLT_SOURCE_LIVE2,    ///< DMX live stream filter source 2
    E_DMX_FLT_SOURCE_LIVE3,    ///< DMX live stream filter source 3
    E_DMX_FLT_SOURCE_FILE0,    ///< DMX file in filter type
    E_DMX_FLT_SOURCE_FILE1,    ///< DMX file1 source from TS1
    E_DMX_FLT_SOURCE_FILE2,    ///< DMX file1 source from TS2
    E_DMX_FLT_SOURCE_FILE3,    ///< DMX file1 source from TS3
    E_DMX_FLT_SOURCE_INVALID,

}EN_DEMO_DMX_FLT_SOURCE;

typedef enum
{
    E_DMX_PVR_SOURCE_LIVE0,
    E_DMX_PVR_SOURCE_LIVE1,
    E_DMX_PVR_SOURCE_LIVE2,
    E_DMX_PVR_SOURCE_LIVE3,
    E_DMX_PVR_SOURCE_FILE0,
    E_DMX_PVR_SOURCE_FILE1,
    E_DMX_PVR_SOURCE_FILE2,
    E_DMX_PVR_SOURCE_FILE3,
    E_DMX_PVR_SOURCE_INVALID,

} EN_DEMO_DMX_PVR_SOURCE;

typedef enum
{
    E_DMX_PVR_EGN0,
    E_DMX_PVR_EGN1,
    E_DMX_PVR_EGN2,
    E_DMX_PVR_EGN3,
    E_DMX_PVR_EGN4,
    E_DMX_PVR_EGN5,
    E_DMX_PVR_ENG_INVALID,

} EN_DEMO_DMX_PVR_ENG;

typedef enum
{
    E_SI_VID_INVALID               = 0x00,
    E_SI_VID_MPEG                  = 0x01,  // SiMPEG
    E_SI_VID_H264                  = 0x02,  // SiH264
    E_SI_VID_AVS                   = 0x03,  // SiAVS
    E_SI_VID_HEVC                  = 0x04,  // SiHEVC

} EN_SI_VideoFormat;

typedef enum
{
    E_SI_AUD_INVALID               = 0x00,  // SiNOT_VALID
    E_SI_AUD_MPEG                  = 0x01,  // SiMPEG
    E_SI_AUD_AC3                   = 0x02,  // SiAC3
    E_SI_AUD_MPEG_AD               = 0x03,  // SiMPEG_AD
    E_SI_AUD_MPEG4                 = 0x04,  // SiMPEG4
    E_SI_AUD_AAC                   = 0x05,  // SiAAC
    E_SI_AUD_AC3P                  = 0x06,  // SiAC3P
    E_SI_AUD_AC3_AD                = 0x07,  // SiAC3_AD

} EN_SI_AudioFormat;


typedef enum
{
    E_PCR_ENG0                     = 0x00,
    E_PCR_ENG1                     = 0x01,
    E_PCR_ENG_INVALID,

} EN_PCR_ENG;


typedef enum
{
    E_FILE_TS                      = 0x00,
    E_FILE_PES                     = 0x01,
    E_FILE_PS                      = 0x02,
    E_FILE_NOT_SUPPORT             = 0xFF
} EN_DMX_FILEIN_FILE_FORMAT;

typedef enum
{
    E_ENG_MMFI                     = 0x00,
    E_ENG_FILE                     = 0x01,
    E_ENG_NOT_SUPPORT              = 0xFF
} EN_DMX_FILEIN_ENG_TYPE;

//--------------------------------------------------------------------------------------------------
// Structure Define
//--------------------------------------------------------------------------------------------------
#define MAX_PG_IN_PAT_DDI   64//sync with HB , max support up to 64 program
#define MAX_AUDIO_PG        64 // maximum 64 audio ES per program

typedef struct
{
    MS_U32 u32PidAudio;
    MS_U32 u32StreamTypeAudio;
    EN_SI_AudioFormat eAudioFmt;
    MS_BOOL bISO_Lang_exist;
    MS_U8 u8Lang[4];
    MS_U8 u8Lang_audio_type;
    MS_BOOL bSupplematary_Audio_exist;
    MS_U8 u8Supplementary_Lang[4];
    MS_U8 u8Supplementary_mix_type;
    MS_U8 u8Supplementary_classification;
}st_Audio_PG;

typedef struct
{
    MS_U32  u32PidVideo;
    st_Audio_PG  Audio_PG[MAX_AUDIO_PG];
    MS_U32  u32PidPcr;
    MS_U32  u32StreamTypeVideo;
    MS_U32  u32ProgNum;
    MS_U32  u32PidPmt;
    EN_SI_VideoFormat eVideoFmt;
} st_PG;

typedef struct
{
    MS_U8             u8FltId;
} ST_PCR_INFO;

typedef struct
{
    st_PG stPgList[MAX_PG_IN_PAT_DDI];
    MS_U32 u32PgNum;
} stFreqPG;

typedef enum
{
    E_SI_DESC_TAG_ISO_639_LANG     = 0x0A,
    E_SI_DESC_TAG_EXTENSION        = 0x7F,
    E_SI_DESC_TAG_STREAM_ID        = 0x52,
    E_SI_DESC_TAG_AC3               = 0x6A,
    E_SI_DESC_TAG_E_AC3             = 0x7A,
}EN_SI_DescriptorTag;

//--------------------------------------------------------------------------------------------------
// DMX Demo API
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PreInit(void);
MS_BOOL Demo_DMX_Init(void);
MS_BOOL Demo_DMX_Exit(void);
MS_BOOL Demo_DMX_FlowSet(EN_DEMO_DMX_FLOW eDmxFlow, EN_DEMO_DMX_FLOW_INPUT eDmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel);
MS_BOOL Demo_DMX_PVR_FlowSet(EN_DEMO_DMX_PVR_ENG ePvrEng, EN_DEMO_DMX_PVR_SOURCE ePvrSrc, MS_BOOL bDscmb);
MS_BOOL Demo_DMX_FltOpen(EN_DEMO_DMX_FLT_SOURCE eFltSource, EN_DEMO_DMX_FLT_TYPE eFltType, MS_U8* pu8FltId, MS_U16 u16Pid);
MS_BOOL Demo_DMX_Open(EN_DEMO_DMX_FLT_SOURCE eFltSource, EN_DEMO_DMX_FLT_TYPE eFltType, MS_U8* pu8FltId);
MS_BOOL Demo_DMX_PCR_FltOpen(EN_DEMO_DMX_FLT_SOURCE eFltSource, MS_U16 u16Pid, MS_U8* u8PCREngId);
MS_BOOL Demo_DMX_PCR_FltStart(MS_U8 u8PCREngId);
MS_BOOL Demo_DMX_PCR_FltStop(MS_U8 u8PCREngId);
MS_BOOL Demo_DMX_PCR_FltClose(MS_U8 u8PCREngId);
MS_BOOL Demo_DMX_AVFifo_Reset(EN_DEMO_DMX_FLT_TYPE eAVFltType, MS_BOOL bRst);
MS_BOOL Demo_DMX_GetProgramInfo(st_PG** ppstPG, MS_U32* pu32PgNum);
DMX_MMFI_FLTTYPE Demo_DMX_MMFI_FltTypeMapping(EN_DEMO_DMX_FLT_TYPE eFltType);
EN_DEMO_DMX_FLT_SOURCE Demo_DMX_FlowToFltSrcMapping(EN_DEMO_DMX_FLOW eDmxFlow);
MS_U32 Demo_DMX_FlowToFQEngMapping(EN_DEMO_DMX_FLOW eDmxFlow);
MS_BOOL Demo_DMX_CheckAudioPgExist(st_PG* pstPG, MS_U32 u32PID);

//--------------------------------------------------------------------------------------------------
// DMX Demo Function
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PCR_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid);
MS_BOOL Demo_DMX_PCR(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid);
MS_BOOL Demo_DMX_Scan_CMD(MS_U8* pu8DmxFlow);
MS_BOOL Demo_DMX_Scan(EN_DEMO_DMX_FLOW eDmxFlow, MS_BOOL bDisableDump);
MS_BOOL Demo_DMX_Sec_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid, MS_U32* pu32Pat0, MS_U32* pu32Pat1, MS_U32* pu32Mask0 , MS_U32* pu32Mask1);
MS_BOOL Demo_DMX_Sec(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid, MS_U32* pu32Pat0, MS_U32* pu32Pat1, MS_U32* pu32Mask0 , MS_U32* pu32Mask1);
MS_BOOL Demo_DMX_Packet_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid);
MS_BOOL Demo_DMX_Packet(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid);
MS_BOOL Demo_DMX_PES_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid);
MS_BOOL Demo_DMX_PES(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid);
MS_BOOL Demo_DMX_Nmask_CMD(MS_U8* pu8DmxFlow, MS_U8 *pu8Pat, MS_U8 *pu8Mask, MS_U8 *pu8Nmask );
MS_BOOL Demo_DMX_Nmask(EN_DEMO_DMX_FLOW eDmxFlow, MS_U8 *pu8Pat, MS_U8 *pu8Mask, MS_U8 *pu8Nmask );

//MS_BOOL appDemo_DmxFileIn_Start(char* srcFileName, MS_U32 pktSize);
//MS_BOOL appDemo_DmxFileIn_Stop(void);
MS_BOOL Demo_DMX_Record_CMD(MS_U8 *pu8Live, MS_U8 *pu8Eng, MS_U32 *pu32VideoPid,MS_U32 *pu32AudioPid,MS_U32 *pu32PCRPid);
MS_BOOL Demo_DMX_Record(EN_DEMO_DMX_PVR_SOURCE ePvrSrc, EN_DEMO_DMX_PVR_ENG ePvrEng, MS_U32 u32VideoPid,MS_U32 u32AudioPid,MS_U32 u32PCRPid);

MS_BOOL Demo_DMX_TSO(MS_U8* pu8TSOEng, MS_U8* pu8Pad);
MS_BOOL Demo_DMX_MMFI_CMD(MS_U8 *pu8MMFIEng,MS_U8 *FltType,MS_U8 *PacketMode,MS_U16 *Pid,const char *FileName);
MS_BOOL Demo_DMX_FileIn_Start(MS_U8 *pu8DmxEngine,MS_U8 *EngineNum, const char *FileName, MS_U8 *Format, MS_U8 *PacketMode);
MS_BOOL Demo_DMX_FileIn_Stop(MS_U8 *pu8DmxEngine,MS_U8 *EngineNum);
MS_BOOL Demo_DMX_Help(void);
#if 0
#if (DEMO_DSCMB_TEST == 1)
MS_BOOL Demo_DSCMB_ConnectFlt_CallBack( MS_BOOL (*func)(MS_U32, MS_U32));
#endif

MS_BOOL DEMO_DSCMB_ConnectFlt(MS_U32 u32EngId, MS_U32 u32FltId)
{
    u32DscmbId = MDrv_DSCMB2_FltAlloc(0); //allocate dscmb
    Drv_DSCMB2_FltConnectFltId(0, u32DscmbId, u32FltId); //connect to tsp

}
#endif


#endif

