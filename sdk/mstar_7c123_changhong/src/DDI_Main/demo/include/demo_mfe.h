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

#ifndef _APPDEMO_MFE_H_
#define _APPDEMO_MFE_H_

#include "drv_mfe_st.h"
#include "apiVOIPMFE.h"
#include "apiMFE_v2.h"

typedef enum
{
    E_MFE_DEVICE_FIRST = 0,
    E_MFE_DEVICE_SECOND,
    E_MFE_DEVICE_MAX,          /// Max Device
} EN_MFE_Device;

typedef enum
{
    E_MFE_NotInital = 0,
    E_MFE_Inital,
    E_MFE_Start,
    E_MFE_Stop,
} EN_MFE_Status;

typedef enum
{
    E_MFE_INPUTSOURCE_NONE = 0,
    E_MFE_INPUTSOURCE_FILE,
    E_MFE_INPUTSOURCE_DIP,
    E_MFE_INPUTSOURCE_DMS,
} EN_MFE_INPUTSOURCE;

typedef struct
{
    MS_U32 u32Index;
    MS_S32 s32Frame_type;
    MS_U32 u32Frame_size;
    MS_U32 u32Framecnt;
}ST_MFE_CTX;

typedef struct
{
    MS_U8* pu8Ptr;
    MS_U32 u32Pa;
    MS_U32 u32Size;
}ST_MFE_MEMADDR;

typedef struct
{
    ST_MFE_MEMADDR stCurY;
    ST_MFE_MEMADDR stCurUV;
} ST_MFE_MEMADDR_CUR;

typedef struct
{
    PVR_Info *encode_info;
    ST_MFE_CTX *g_compress_ctx;
}ST_MFE_VENC_INFO;

typedef struct
{
    MS_BOOL bCaptureInited;
    MS_U32 u32Window;

    MS_U8 *pu8BufferAddr;
    MS_PHY phyBufferAddr;
    MS_PHY phyBufferSize;

    MS_U32 u32CaptureEnable;
    MS_U32 u32WindowVisible;
}ST_MFE_VENC_SET_CAPTUREINFO;

typedef struct
{
    MS_PHY phyBufferYAddr;
    MS_PHY phyBufferCAddr;
    MS_U64 u64Pts;
    MS_U32 u32frameCount;
}ST_MFE_VENC_CAPTUREINFO;

typedef struct
{
    void *pInstanceMFE;
    EN_MFE_Status  eMFEStatus;
    Task_Info   stMFETask;
    MS_S32 MFE_DIPEventid;
}ST_MFE_Controller;

typedef struct
{
    FILE *out_f;
    FILE *in_f;
}ST_MFE_FILE;

typedef struct
{
    MS_BOOL bInited;
    EN_MFE_Status stVENCStatus;

    ST_MFE_FILE stFile;

    MS_U8  u8Codec;        // 1:Avc  3:Hevc
    MS_U32 u32Width;       //16 align
    MS_U32 u32Height;      //16 align
    MS_U32 u32FrameRatex100;
    MS_U32 u32Bitrate;
    MS_U32 u32FrameNum;
    MS_U32 u32MiuOffset;
    MS_U32 u32InitMode;     // 1 : IMI flow
    MFE_COLOR_FORMAT_INFO eColorFormat;

    ST_MFE_VENC_INFO stVENCInfo;

    MS_U8 *pu8VENCBufOri;   //memory allocate
    ST_MFE_MEMADDR stMFEBufAddr;

    ST_MFE_VENC_SET_CAPTUREINFO stSetCaptureInfo;
    ST_MFE_VENC_CAPTUREINFO stCaptureInfo;
}ST_MFE_VENC;

MS_BOOL Demo_MFE_Encode_FMT_EX(MS_U32 *pu32Index,MS_U32 *pu32Fmt);
MS_BOOL Demo_MFE_Init(const MS_U32 *pu32FrameWidth, const MS_U32 *pu32FrameHeight, const MS_U32 *pu32FrameRate, const MS_U32 *pu32BitRate);
MS_BOOL Demo_MFE_Init_EX(const MS_U32 *pu32Index, const MS_U8 *pu8Codec,const MS_U32 *pu32FrameWidth, const MS_U32 *pu32FrameHeight, const MS_U32 *pu32FrameRate, const MS_U32 *pu32BitRate, const MS_U32 *pu32InitMode);

// Encode from File
MS_BOOL Demo_MFE_Encode_File(char *pInfile, char *pOutfile, const MS_U32 *pu32FrameNumber);
MS_BOOL Demo_MFE_Encode_File_EX(MS_U32 *pu32Index,char *pInfile, char *pOutfile, const MS_U32 *pu32FrameNumber);

// Encode from SC_OP by DIP capture
MS_BOOL Demo_MFE_Encode_From_DIP(char *pOutfile, const MS_U32 *pu32FrameNumber);
MS_BOOL Demo_MFE_Encode_From_DIP_EX(const MS_U32 *pu32Index,char *pOutfile,const MS_U32 *pu32FrameNumber);

#if (DEMO_DMS_TEST == 1)
// Encode from Display manage service capture
MS_BOOL Demo_MFE_Set_CaptureInfo(MS_U32 *pu32Index,MS_U32 *u32Window,MS_U32 *pu32Enable,MS_U32 *pu32Visible);
MS_BOOL Demo_MFE_Encode_From_Capture_EX(const MS_U32 *pu32Index,char *pOutfile);
#endif
#endif
