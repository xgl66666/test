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

#ifndef _DEMO_VDEC_H_
#define _DEMO_VDEC_H_ 1
#define DRV_PROC_SIZE               0xa000//(1024*8)  //1024 entries, 8 bytes/entry
#define PLAY_SIZE_LIMITITATION      0x600000
#define AVSYNC_TIMEOUT_INTERVAL     2000
#define VDEC_MAX_DQNUM 8
#define VDEC_MAX_SUPPORT_STREAM_NUM 4

#include "MsCommon.h"
#include "apiVDEC_EX.h"
#include "drvMVOP.h"

typedef enum
{
    E_VDEC_DEVICE_MAIN = 0,     /// Main
    E_VDEC_DEVICE_SUB,          /// Sub
#if (DEMO_VDEC_NDECODE_TEST == 1)
    E_VDEC_DEVICE_FIRST = E_VDEC_DEVICE_MAIN,
    E_VDEC_DEVICE_SECOND,
    E_VDEC_DEVICE_THIRD,
    E_VDEC_DEVICE_FOURTH,
#endif
    E_VDEC_DEVICE_MAX,          /// Max Device
} EN_VDEC_Device;

typedef enum
{
    E_VDEC_SYNC_PCR_MODE = 0,
    E_VDEC_SYNC_AUDIO_MASTER_MODE,
    E_VDEC_SYNC_VIDEO_MASTER_MODE,
    E_VDEC_SYNC_MODE_MAX,
}EN_VDEC_AVSYNC_Type;

typedef enum
{
    E_VDEC_MVOP_MODE = 0,
    E_VDEC_DIP_MODE = 2,
    E_VDEC_SWDetile_MODE,
    E_VDEC_PureMCU_MODE,
    E_VDEC_ShowFrame_MODE_MAX,
}EN_VDEC_ShowFrame_Mode;

typedef enum
{
    //MPEG 1/2
    E_VDEC_DDI_CODEC_TYPE_MPEG2 = 0,
    //H264
    E_VDEC_DDI_CODEC_TYPE_H264,

    E_VDEC_DDI_CODEC_TYPE_AVS,

    E_VDEC_DDI_CODEC_TYPE_HEVC,

    E_VDEC_DDI_CODEC_TYPE_MVC,

    E_VDEC_DDI_CODEC_TYPE_VP9,
    //MPEG 1/2 for I-Frame
    E_VDEC_DDI_CODEC_TYPE_MPEG2_IFRAME = 0x100,
    //H264 for I-Frame
    E_VDEC_DDI_CODEC_TYPE_H264_IFRAME,

    E_VDEC_DDI_CODEC_TYPE_AVS_IFRAME,

    E_VDEC_DDI_CODEC_TYPE_HEVC_IFRAME,

    E_VDEC_DDI_CODEC_TYPE_VP9_IFRAME,

    E_VDEC_DDI_CODEC_TYPE_MPEG2_MEMORY = 0x200,

    E_VDEC_DDI_CODEC_TYPE_H264_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_AVS_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_HEVC_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_MPEG4_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_VC1_ADV_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_VC1_MAIN_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_MVC_MEMORY,

    E_VDEC_DDI_CODEC_TYPE_VP9_MEMORY,

}EN_VDEC_DDI_CodecType;

typedef enum
{
    ///DTV mode
    E_VDEC_DDI_SRC_MODE_DTV = 0,
    ///TS file mode
    E_VDEC_DDI_SRC_MODE_TS_FILE,
    ///generic file mode
    E_VDEC_DDI_SRC_MODE_FILE,
    /// TS file and dual ES buffer mode
    E_VDEC_DDI_SRC_MODE_TS_FILE_DUAL_ES,
    ///generic file and dual ES buffer mode
    E_VDEC_DDI_SRC_MODE_FILE_DUAL_ES,
} EN_VDEC_DDI_SrcMode;

typedef enum
{
    /// checkerboard - pixels are alternatively from L and R
    E_VDEC_DDI_FRM_PACK_CHECKERBOARD = 0,
    /// column alternation - L and R are interlaced by column
    E_VDEC_DDI_FRM_PACK_COLUMNALT,
    /// row alternation - L and R are interlaced by row
    E_VDEC_DDI_FRM_PACK_RAWALT,
    /// side by side - L is on the left, R on the right
    E_VDEC_DDI_FRM_PACK_SIDEBYSIDE,
    /// top bottom - L is on top, R on bottom
    E_VDEC_DDI_FRM_PACK_TOPBOTTOM,
    /// frame alternation - one view per frame
    E_VDEC_DDI_FRM_PACK_FRMALT,
} EN_VDEC_FrmPackMode;

typedef enum
{
    E_VDEC_SetCmd_ShowFrameInfo = 0,
    E_VDEC_SetCmd_Initialize,
    E_VDEC_SetCmd_DIPStopType,
    E_VDEC_SetCmd_StreamType,
    E_VDEC_SetCmd_PUSIControl,
    E_VDEC_SetCmd_TimeOutMech,
    E_VDEC_SetCmd_SetVDECWinInfo,
    E_VDEC_SetCmd_MAX,

    E_VDEC_GetCmd_VideoSrcMode = 0x100,
    E_VDEC_GetCmd_GetDevice_ByStreamID,
    E_VDEC_GetCmd_GetIsHDRStream,
    E_VDEC_GetCmd_MvopInputSel,
    E_VDEC_GetCmd_BUFFERTYPE_ALLOC_BS,
    E_VDEC_GetCmd_GetDSIndexDepth,
    E_VDEC_GetCmd_MAX,
} EN_VDEC_Cmd;

typedef enum
{
    E_VDEC_XC_Window_0 = 0,
    E_VDEC_XC_Window_1,
    E_VDEC_XC_Window_2,
    E_VDEC_XC_Window_3,
    E_VDEC_XC_MAX_Window
} EN_VDEC_XC_Window;

typedef enum
{
    E_VDEC_NotInital = 0,
    E_VDEC_Start,
    E_VDEC_Stop,
    E_VDEC_ReallyStop,
} EN_VDEC_DIP_Status;

typedef enum
{
    E_VDEC_Frame_Invalid = 0,
    E_VDEC_Frame_Display,
    E_VDEC_Frame_Repeat,
    E_VDEC_Frame_Release,
} EN_VDEC_DIP_FrameStatus;

typedef enum
{
    E_VDEC_DDI_DISPLAY_PATH_MVOP_MAIN = 0,
    E_VDEC_DDI_DISPLAY_PATH_MVOP_SUB,

    E_VDEC_DDI_DISPLAY_PATH_NONE = 0xFF,
} EN_VDEC_DDI_DISPLAY_PATH;

typedef struct
{
    MS_U32  u32Address;
    MS_U32  u32PlaySize;
    MS_U32  u32Address2;
    MS_U32  u32PlaySize2;
}ST_VDEC_FilePlay;

typedef struct
{
    MS_U16 u16CropRight;
    MS_U16 u16CropLeft;
    MS_U16 u16CropTop;
    MS_U16 u16CropBottom;
}ST_VDEC_CropInfo;

typedef struct
{
    MS_U32 win_id;
    MS_U8 u8EnableSetDigitalSignal;     // 0: default , 1: force enable , 2: force disable
    MS_U8 u8EnableClearDigitalSignal;   // 0: default , 1: force enable , 2: force disable
} ST_VDEC_WIN_INFO;

typedef struct
{
    MS_BOOL bValid;
    EN_VDEC_Device eDevice;
    EN_VDEC_ShowFrame_Mode eShowFrame_Mode;

    MS_U32 u32Window;
    MS_U16 u16X;
    MS_U16 u16Y;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U16 u16Layer;

    EN_VDEC_DDI_DISPLAY_PATH eShowFrame_MvopPath;
    ST_VDEC_WIN_INFO stVDECWinControl;

}ST_VDEC_ShowFrameInfo;

typedef struct
{
    MS_BOOL bInited;
    VDEC_StreamId stVDECSteamID;
    VDEC_EX_Stream eVDECStreamType;

    MS_U32  u32STCEng;
    MS_U32  u32CmdPreSetControl;
    EN_VDEC_DDI_CodecType   eVideoCodec;
    EN_VDEC_DDI_SrcMode eVideoSrcMode;
    ST_VDEC_ShowFrameInfo stShowFrameInfo;

    ST_VDEC_FilePlay stFilePlayInfo;

    MS_U8 u8IsHDRStream;
    MVOP_InputSel eMvopInputSel;
}ST_VDEC_INFORMATION;

typedef struct
{
    MS_U32 u32version;
    MS_U32 u32size;

    ST_VDEC_CropInfo stDIPDispCropInfo;

#if (DEMO_XC_DIPMultiWin_TEST == 1) //need to remove

    VDEC_EX_DispFrame stDIPDispFrameInfo;
    VDEC_EX_FrameInfoExt stDIPDispFrameInfoEXT;

#ifdef VDEC_CAP_FRAME_INFO_EXT_V6
    VDEC_EX_TileMode eDIPFrameTileMode;
#endif

#endif
}ST_VDEC_DIP_DispFrameInfo;

typedef struct
{
    MS_BOOL b10bit;
    MS_BOOL bFieldOrder;
    MS_U32 u32ScanType;
    MS_U64 u64TimeStemp;
    EN_VDEC_DIP_FrameStatus eDIPFrameStatus;
    ST_VDEC_DIP_DispFrameInfo stDispFrames[2];
}ST_VDEC_DIP_FrameInfo;

typedef struct
{
    MS_BOOL bNeedSync;
    MS_U32 u32SyncDelay;
    MS_U16 u16Tolerance;
    MS_U32 u32FreerunThreshold;
    MS_U32 u32STCEng;
    MS_U32 u32DropCnt;
    MS_U32 u32DispCnt;
    MS_U16 u16InputTolerance;
}ST_VDEC_DIP_FrameSyncInfo;

typedef struct
{
    MS_U16  u16WritePointer;
    MS_U16  u16ReadPointer;
}ST_VDEC_DIP_FrameWriteReadPointer;

typedef struct
{
    VDEC_StreamId   StreamId;
    EN_VDEC_XC_Window   eWindow;
    EN_VDEC_DIP_Status  eDecodeStatus;
    MS_BOOL bDIPStopType;   // False: normal  TRUE:seamless
    VDEC_EX_CodecType   eCodec;
    MS_U32 u32FrameRate;
    ST_VDEC_DIP_FrameInfo    stDIPFrameInfo[VDEC_MAX_DQNUM];
    ST_VDEC_DIP_FrameSyncInfo   stDIPFrameSyncInfo;

    ST_VDEC_DIP_FrameWriteReadPointer   stFrameWriteReadPointer;
}ST_VDEC_DIP_DispInfo;

typedef struct
{
    Task_Info   stGetFrameTask;
    EN_VDEC_DIP_Status  eGetTaskStatus;

    Task_Info   stReleaseFrameTask;
    EN_VDEC_DIP_Status  eReleaseTaskStatus;
}ST_VDEC_DIP_FrameController;

MS_BOOL Demo_VDEC_SetDispConfig(EN_VDEC_Device* peDevice,EN_VDEC_ShowFrame_Mode* peMode);
MS_BOOL Demo_VDEC_DIP_FrameController(VDEC_StreamId *pHandle,MS_U32 *pu32DecoderIdx);
MS_BOOL Demo_VDEC_IsSupportDS(VDEC_StreamId stVDECStreamId);
MS_BOOL Demo_VDEC_FrameSyncOn(EN_VDEC_Device* peDevice,MS_BOOL *pbSync,MS_U32 *pu32SyncDelay,MS_U16 *pu16tolerance,MS_U32 *pu32FreerunThreshold);
MS_BOOL Demo_VDEC_StopFrameController(EN_VDEC_Device* peDevice);
MS_BOOL Demo_VDEC_SetMode(EN_VDEC_Device* peDevice,EN_VDEC_Cmd eVDECCmd,void* pVDECInfo);
MS_U32 Demo_VDEC_UtlLoadStream(MS_U32 u32Addr,MS_U32 u32AddrSize,MS_U32 u32CodecType,char* pPath);
MS_BOOL Demo_VDEC_WaitAVSync(MS_U32* pu32Enable);
MS_BOOL Demo_VDEC_DelayTaskTime(MS_U32* pu32DelayTaskTime);
MS_BOOL Demo_VDEC_SetVDECDebugLevel(EN_VDEC_Device* peDevice,MS_U32* u32VDECDebugLv);
MS_BOOL Demo_VDEC_GetMode(EN_VDEC_Device* peDevice,EN_VDEC_Cmd eVDECCmd,void* pVDECInfo);
void* Demo_VDEC_GetStreamID(EN_VDEC_Device* peDevice);
EN_VDEC_FrmPackMode Demo_VDEC_GetFramePackMode(EN_VDEC_Device* peDevice);
MS_BOOL Demo_VDEC_GetDispInfo(EN_VDEC_Device* peDevice,VDEC_EX_DispInfo* pstDispinfo);
MS_BOOL Demo_VDEC_GetDecFrameInfo(EN_VDEC_Device* peDevice,VDEC_EX_FrameInfo* pstFrameInfo);
MS_BOOL Demo_VDEC_IsAVSyncDone(EN_VDEC_Device* peDevice);
MS_BOOL Demo_VDEC_CheckDecoderStatus(EN_VDEC_Device* peDevice);
MS_BOOL Demo_VDEC_Init(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId,EN_VDEC_DDI_CodecType eVideoCodec,EN_VDEC_DDI_SrcMode eMode,MS_U32 u32STCEng);
MS_BOOL Demo_VDEC_Play(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId,EN_VDEC_AVSYNC_Type eAVSYNC_Mode);
MS_BOOL Demo_VDEC_Stop(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId);
MS_BOOL Demo_VDEC_DecIFrame(EN_VDEC_Device* peDevice,int* pCodec, char* pLogopath);
MS_BOOL Demo_VDEC_FilePlay(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId,MS_U32 u32Address,MS_U32 u32ActualBitstreamSize,MS_U32 u32PushTime,MS_U32 u32Address2,MS_U32 u32ActualBitstreamSize2,EN_VDEC_ShowFrame_Mode eShowFrame_Mode);
MS_BOOL Demo_VDEC_FileStop(EN_VDEC_Device* peDevice);
MS_BOOL Demo_VDEC_OpenDBGMSG(MS_U8 *u8DbgMsg , MS_U32 *u32IntervalTime);
MS_BOOL Demo_VDEC_TimeshiftSeamlessControl(VDEC_StreamId* pstVDECStreamId, MS_U32 u32Param);
MS_BOOL Demo_VDEC_SW_DeTile(EN_VDEC_Device* peDevice, MS_U8 *pu8GOPNum, VDEC_EX_FrameInfo *pstDispFrame);
MS_BOOL Demo_VDEC_GetVideoPTS(EN_VDEC_Device* peDevice, MS_U32* pu32VPTS, MS_U64* pu64VPTS);
MS_BOOL Demo_VDEC_CmdPreSetControl(EN_VDEC_Device* peDevice,MS_U32* pu32CmdPreSetControl);
MS_BOOL Demo_VDEC_PreSetControl(EN_VDEC_Device* peDevice,VDEC_StreamId* pstVDECStreamId);


#endif

