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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_FileIn.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __API_PVRPL_FileIn_H__
#define __API_PVRPL_FileIn_H__

#include "MsCommon.h"
#include "PVRPL_Common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FILEIN_ERROR_Handler 0xff



#define PVRPL_FILEIN_FILTER_TYPE_MASK                    0x0000000F   ///< DMX filter type mask
#define PVRPL_FILEIN_FILTER_TYPE_VIDEO                   0x00000000   ///< DMX video filter type
#define PVRPL_FILEIN_FILTER_TYPE_AUDIO                   0x00000001   ///< DMX audio filter type
#define PVRPL_FILEIN_FILTER_TYPE_AUDIO2                  0x00000002   ///< DMX audio2 filter type
#define PVRPL_FILEIN_FILTER_TYPE_SECTION                 0x00000003   ///< DMX section filter type
#define PVRPL_FILEIN_FILTER_TYPE_PCR                     0x00000006   ///< DMX PCR filter type
#define PVRPL_FILEIN_FILTER_TYPE_PES                     0x00000004   ///< DMX PES filter type
#define PVRPL_FILEIN_FILTER_TYPE_VIDEO3D                 0x00000008   ///< DMX V3D filter type
#define PVRPL_FILEIN_FILTER_TYPE_AUDIO3                  0x00000009   ///< DMX audio 3 filter type
#define PVRPL_FILEIN_FILTER_TYPE_AUDIO4                  0x0000000A   ///< DMX audio 4 filter type


typedef enum
{
    PVRPL_DBG_NONE,
    PVRPL_DBG_ERR,
    PVRPL_DBG_WARN,
    PVRPL_DBG_INFO,
    PVRPL_DBG_FUNC,
    PVRPL_DBG_TRACE,
} PVRPL_DBGMSG_LEVEL;

typedef enum
{
     E_PVRPL_FILEIN_SUCESS,
     E_PVRPL_FILEIN_FAIL,
}EN_PVRPL_FILEIN_STATUS;

typedef enum
{
    EN_FILEIN_TRICK_MODE  = 1, // can add a cmd if there is three empty cmd
    EN_FILEIN_NORMAL_MODE = 2, // can add a cmd if there is two empty cmd
}PVRPL_FileInMode;

typedef enum
{
    E_PVRPL_PCR_ENG0                     = 0x00,
    E_PVRPL_PCR_ENG1                     = 0x01,
    E_PVRPL_PCR_ENG_INVALID,

}EN_PVRPL_PCR_ENG;

typedef enum
{
    E_PVRPL_FILEIN_TSIF0,
    E_PVRPL_FILEIN_TSIF1,
    E_PVRPL_FILEIN_TSIF2,
    E_PVRPL_FILEIN_TSIF3,
    E_PVRPL_FILEIN_INVALID,
} EN_PVRPL_FILEIN_PATH;

typedef enum
{
    E_PVRPL_DMX_FLOW_PLAYBACK                   = 0,                       ///< DMX playback flow
    E_PVRPL_DMX_FLOW_PVR                        = 1,                       ///< DMX recording flow
    E_PVRPL_DMX_FLOW_PVR1                       = 2,                       ///< DMX recording flow, note: Not support for all chip

    E_PVRPL_DMX_FLOW_PVRCA                      = 3,                       ///< CI+ PVR

    E_PVRPL_DMX_FLOW_FILEIN_MM                  = 4,                       ///< DMX Filein flow with Main path Vido and MMFI path Audio
    E_PVRPL_DMX_FLOW_MMFI0                      = E_PVRPL_DMX_FLOW_FILEIN_MM,      ///< DMX Filein flow with MMFI 0 (MMFI version 2.0)
    E_PVRPL_DMX_FLOW_FILEIN_MM3D                = 5,                       ///< DMX Filein flow with Main path Video, MMFI path 3DVideo, and MMFI Audio
    E_PVRPL_DMX_FLOW_MMFI1                      = E_PVRPL_DMX_FLOW_FILEIN_MM3D,    ///< DMX Filein flow with MMFI 1 (MMFI version 2.0)

    E_PVRPL_DMX_FLOW_CIPHSS_PLAYBACK            = 6,                       ///< Playback flow for CI+ HSS 2 path input for CI+
    E_PVRPL_DMX_FLOW_CIPHSS_PVRCA               = 7,                       ///< PVR flow for CI+ HSS 2 path input for CI+

    E_PVRPL_DMX_FLOW_PVR2                       = 8,                       ///< DMX recording flow, note: Not support for all chip, PVR2
    E_PVRPL_DMX_FLOW_PVR3                       = 9,                       ///< DMX recording flow, note: Not support for all chip
    E_PVRPL_DMX_FLOW_PVR4                       = 10,                      ///< DMX recording flow, note: Not support for all chip
    E_PVRPL_DMX_FLOW_PVR5                       = 11,                      ///< DMX recording flow, note: Not support for all chip

    E_PVRPL_DMX_FLOW_TSO_PLAYBACK               = 12,                      ///< DMX TSO playback flow (live0 & file0)
    E_PVRPL_DMX_FLOW_PLAYBACK1                  = 13,                      ///< DMX playback flow from TS1 (version 3.0)
    E_PVRPL_DMX_FLOW_PVRCA1                     = 14,                      ///< CI+ PVR (version 3.0)

    E_PVRPL_DMX_FLOW_TSO_PLAYBACK1              = 15,                      ///< DMX TSO1 playback flow (live1 & file1)
    E_PVRPL_DMX_FLOW_PLAYBACK2                  = 16,                      ///< DMX playback flow from TS2 (version 3.0/4.0)
    E_PVRPL_DMX_FLOW_PLAYBACK3                  = 17,                      ///< DMX playback flow from TS3 (version 4.0)

    E_PVRPL_DMX_FLOW_PLAYBACK_NOCA              = 18,                      ///< DMX playback flow without pass through CA
    E_PVRPL_DMX_FLOW_PLAYBACK_SRC_TS1           = 19,                      ///< DMX playback, CA input from TS1 output to Live0

	E_PVRPL_DMX_FLOW_PLAYBACK_FI				= 20,					   ///< DMX playback flow from TSFI (version 3.0)

    E_PVRPL_DMX_FLOW_NUM,
} EN_PVRPL_DMX_FLOW;

typedef enum 
{
    EN_FILEIN_INFO_PCR
}EN_FILEIN_INFO;

typedef struct
{
    MS_U32 u32Size;
    MS_U32 u32PhyAddr;
}FileInBlock;

typedef struct
{
    MS_U32 eVideoType;
    MS_U32 eAudioType;
}AVFIFOInfo;

typedef struct
{
    //FileEng Info
    EN_PVRPL_DMX_FLOW       eDMXFlow;
    EN_PVRPL_FILEIN_PATH    eFileInPath;
    AVFIFOInfo              eAVFIFO;
    MS_U32                  u32NumofFeeded;
    MS_U32                  u32CurWaterLevel;
    MS_U8                   u8QueFront;         // index for consuming the element from TSP HW Queue
    MS_U8                   u8QueRear;          // index for feeding to PVR SW Queue
    MS_U8                   u8Feeded2HW;          // index for consuming the element from PVR SW Queue or index for feeding to TSP HW Queue
    MS_BOOL                 bQueFullTag;
    MS_U8                   u8MaxQueNum;
    FileInBlock             *psFileInQueue;
    PVRPL_FileInMode        eQueThreshold;
    MS_U8                   u8FileInPktMode;
    MS_U8                   u8STCEng;
    MS_BOOL                 bByPassFileIn;
    MS_U32                  u32PausedTime;
    PVR_PATH                u8PathIdx;
    MS_BOOL                 bInit;
}FileInResource;

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_Init(FileInResource *sCurFileInInfo);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_PIDFilterControl(FileInResource *sCurFileInInfo,MS_BOOL bAdd,MS_U8 *pu8FltID,MS_U16 u16PID,MS_U32 u32PVRPLFltType);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_SetPlaybackTime(FileInResource *sCurFileInInfo,MS_U32 u32PlaybakTime);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_EnableFileInPause(FileInResource *sCurFileInInfo,MS_BOOL bEnable);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_EnablePlaybackTime(FileInResource *sCurFileInInfo,MS_BOOL bEnable);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_EnableBypassFileInTime(FileInResource *sCurFileInInfo,MS_BOOL bBypass);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ADDFileInBlock(FileInResource *sCurFileInInfo,MS_U32 u32PhyAddr,MS_U32 u32Size);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ResetFileInEng(FileInResource *sCurFileInInfo);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ChangeFileInMode(FileInResource *sCurFileInInfo,PVRPL_FileInMode eFileInMode);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_FeedFileInBlock(FileInResource *sCurFileInInfo,MS_U32 u32PhyAddr,MS_U32 u32Size);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ConsumeFileInBlock(FileInResource *sCurFileInInfo);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ResetFileInEng(FileInResource *sCurFileInInfo);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_CaliberatePlaybackTime(FileInResource *sCurFileInInfo,MS_BOOL bForward,MS_U32 u32Time);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetPlaybackTime(FileInResource *sCurFileInInfo,MS_U32 *pu32PlaybakTime);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetFileInTime(FileInResource *sCurFileInInfo,MS_U32 *pu32FileInTime);


EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ResetAVFIFO(FileInResource *sCurFileInInfo);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_FlushData(FileInResource *sCurFileInInfo);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetSTC(FileInResource *sCurFileInInfo,MS_U32 *pu32Stc32,MS_U32 *pu32Stc);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_SetSTC(FileInResource *sCurFileInInfo,MS_U32 u32Stc32,MS_U32 u32Stc);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_IsFileInEnd(FileInResource *sCurFileInInfo);
MS_BOOL      PVRPL_FileIn_IsFileInBlockFull(FileInResource *sCurFileInInfo);
MS_BOOL      PVRPL_FileIn_IsFileInBlockEmpty(FileInResource *sCurFileInInfo);
MS_U8        PVRPL_FileIn_GetPCREng(FileInResource *sCurFileInInfo);

MS_U32       PVRPL_FileIn_Mapping_DmxFltType_To_PVRPLFltType(MS_U32 u32DmxFltType);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_Exit(FileInResource *sCurFileInInfo);
void         PVRPL_FileIn_SetDBGLevel(FileInResource *sCurFileInInfo,PVRPL_DBGMSG_LEVEL eDBGLevel);
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetInfo(FileInResource *sCurFileInInfo, EN_FILEIN_INFO FileInInfo, MS_U32 *u32pmtr, void* structure);




#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_PVRPL_FileIn__
