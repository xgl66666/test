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
#ifndef _DEMO_PVR_H_
#define _DEMO_PVR_H_
#include "demo_dmx.h"

typedef enum
{
    E_DDI_PVR_VIDEO_CODEC_TYPE_MPEG2 = 0,///MPEG 1/2
    E_DDI_PVR_VIDEO_CODEC_TYPE_H264,///H264
    E_DDI_PVR_VIDEO_CODEC_TYPE_AVS,///AVS
    E_DDI_PVR_VIDEO_CODEC_TYPE_HEVC,///H.265
} eDDI_PVR_VDEC_CodecType;

typedef enum
{
    E_DDI_PVR_AUDIO_CODEC_TYPE_NONE=0,
    E_DDI_PVR_AUDIO_CODEC_TYPE_MPEG=1,
    E_DDI_PVR_AUDIO_CODEC_TYPE_AC3=2,
    E_DDI_PVR_AUDIO_CODEC_TYPE_AC3P=3,
    E_DDI_PVR_AUDIO_CODEC_TYPE_AAC=4
} eDDI_PVR_ADEC_CodecType;

// for debug task message config
typedef MS_U8 PVR_DBG_LEVEL;
#define E_PVR_DBG_NONE              0x00        // no message printed and task deleted
#define E_PVR_DBG_VDEC_MAIN         0x01        // VPTS related, ES data related, Sync status, FrameCnt, ES buf R/W
#define E_PVR_DBG_VDEC_SUB          0x02
#define E_PVR_DBG_ADEC_MAIN         0x04        // APTS and diff in ms, FrameCnt, Buffer level
#define E_PVR_DBG_ADEC_SUB          0x08
#define E_PVR_DBG_DMX_REC           0x10        // record write pointer , timestamp
#define E_PVR_DBG_DMX_PLAY          0x20        // CMDQ info, File Timestamp, play Timestamp, STC/PCR, AV fifo

MS_BOOL Demo_PVR_Help(void);
MS_BOOL Demo_PVR_Init(MS_U32 *u32FSType, char *pMountPath);
MS_BOOL Demo_PVR_Destroy(void);
MS_BOOL Demo_PVR_SetRecordType(MS_U32 *u32RecordType);
MS_BOOL Demo_PVR_SetRecIdx(MS_U32 *idx);
MS_BOOL Demo_PVR_GetRecIdx(void);
MS_BOOL Demo_PVR_SetPlaybackPath(MS_U32 *PlaybackIdx);
MS_BOOL Demo_PVR_GetPlaybackPath(void);

MS_BOOL Demo_PVR_RecFlowSet(EN_DEMO_DMX_FLOW_INPUT* peDmxInput,EN_DEMO_DMX_FLOW* pePlayback);
MS_BOOL Demo_PVR_Record_Start(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char* fileName);
MS_BOOL Demo_PVR_Record_Stop(void);
MS_BOOL Demo_PVR_Record_Pause(void);
MS_BOOL Demo_PVR_Record_Resume(void);
MS_BOOL Demo_PVR_Record_ChangeProgram(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec);
#if (DEMO_PVR_SUPPORT_CAPVR_TEST == 1)
#if (DEMO_DSCMB_ADVANCED_TEST == 1)
MS_BOOL Demo_PVR_Record_DSCMB_SetCAVID(MS_U32 *u32CAVid);
#endif
MS_BOOL Demo_PVR_Record_DSCMB_Open(MS_U8 *u8Type,MS_U8 *u8StrOddCW,MS_U8 *u8StrEvenCW);
MS_BOOL Demo_PVR_Record_DSCMB_Close(void);
MS_BOOL Demo_PVR_Record_DSCMB_Change(MS_U8 *u8Type,MS_U8 *u8StrOddCW,MS_U8 *u8StrEvenCW);
#if (DEMO_PVR_SUPPORT_DYNAMICKEY_TEST == 1)
MS_BOOL Demo_PVR_SetEncryptKeyIdx(MS_U32 *u32KeyIdx);
MS_BOOL Demo_PVR_CheckEncryptFile(char *fileName);
#endif
#endif
MS_BOOL Demo_PVR_Timeshift_RecordStart(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec, char* fileName);
MS_BOOL Demo_PVR_Timeshift_RecordStop(void);

MS_BOOL Demo_PVR_Timeshift_Stop(void);
MS_BOOL Demo_PVR_Timeshift_PlaybackStart(void);
MS_BOOL Demo_PVR_Timeshift_PlaybackStop(void);

#if (DEMO_PVR_SUPPORT_TIMESHIFTSEAMLESS_TEST == 1)
MS_BOOL Demo_PVR_TimeshiftSeamless_PlaybackStart(char *fn); // Olivia_Remove
MS_BOOL Demo_PVR_SeamlessBackRecordStart(void);
MS_BOOL Demo_PVR_SeamlessBackRecordStop(void);
#endif

#if (DEMO_PVR_SUPPORT_CHANGE_DISPLAYWIN_TEST == 1)
MS_BOOL Demo_PVR_ChangeDisplayWindowSize(MS_U32 *u32Width,MS_U32 *u32Height,MS_U32 *u32XPos,MS_U32 *u32YPos,MS_BOOL *bDef);
#endif

MS_BOOL Demo_PVR_PlaybackStart(char *filename);
MS_BOOL Demo_PVR_PlaybackStop(void);
MS_BOOL Demo_PVR_PlaybackPause(void);
MS_BOOL Demo_PVR_PlaybackResume(void);
MS_BOOL Demo_PVR_PlaybackSlowForward(void);
MS_BOOL Demo_PVR_PlaybackFastForward(void);
MS_BOOL Demo_PVR_PlaybackFastBackward(void);
MS_BOOL Demo_PVR_PlaybackJumpToTime(MS_U32 *u32Hour,MS_U32 *u32Minute,MS_U32 *u32Second);
MS_BOOL Demo_PVR_PlaybackChangeProgram(MS_U32* VideoPid, MS_U32* AudioPid, MS_U32* PCRPid,const  MS_U32* pu32VCodec,const  MS_U32* pu32ACodec,char *filename);
MS_BOOL Demo_PVR_GetFileInfo(char *fulFileName);

MS_BOOL Demo_PVR_USB_UnplugEvent(char *moutPath);
MS_BOOL Demo_PVR_PlaybackStepIn(void);
MS_BOOL Demo_PVR_PlaybackWithNormalSpeed(void);
MS_BOOL Demo_PVR_CheckUSBSpeed(void);
MS_BOOL Demo_PVR_CaptureThumbnail(char *filename);
MS_BOOL Demo_PVR_JumpToThumbnail(char *filename);
MS_BOOL Demo_PVR_HideThumbnail(void);
MS_BOOL Demo_PVR_PlaybackABLoop(MS_U32 *u32ABBeginTime,MS_U32 *u32ABEndTime);
MS_BOOL Demo_PVR_PlaybackResetABLoop(void);
MS_BOOL Demo_PVR_PlaybackRemoveSkipTime(MS_U32 *u32BeginTimeInSec,MS_U32 *u32EndTimeInSec);
MS_BOOL Demo_PVR_PlaybackAddSkipTime(MS_U32 *u32BeginTimeInSec,MS_U32 *u32EndTimeInSec);
MS_BOOL Demo_PVR_ShowTime(MS_U32 *u32Type,MS_U32 *u32Set);
MS_BOOL Demo_PVR_SetPlaybackRetentionLimit(MS_U32 *u32RetentionLimitTimeInSec);
MS_BOOL Demo_PVR_PlaybackJumpForward(void);
MS_BOOL Demo_PVR_PlaybackJumpBackward(void);
MS_BOOL Demo_PVR_RemoveFile(char *fileName);
MS_BOOL Demo_PVR_RenameFile(char *fileName,char *newFileName);
MS_BOOL Demo_PVR_SetTimeshiftRecScreenFrozen(const MS_BOOL *bFrozen);
MS_BOOL Demo_PVR_SetTimeshiftRecDuration(MS_U32 *u32PlanRecTimeInMin);

MS_BOOL Demo_PVR_SetDBGLevel(PVR_DBG_LEVEL *dbglevel , MS_U32 *intervelTime);

MS_BOOL Demo_PVR_GetRecFlowSet(EN_DEMO_DMX_FLOW_INPUT* peDmxInput,EN_DEMO_DMX_FLOW* pePlayback);
MS_BOOL Demo_PVR_GetRecEng(EN_DEMO_DMX_PVR_ENG *peDmxPvrEng);

MS_BOOL Demo_PVR_ShowAudioTrackInfo(void);
MS_BOOL Demo_PVR_SetAudioTrackByIndex(MS_U32 *u32AudIdx);


#endif /* _DEMO_PVR_H_ */

