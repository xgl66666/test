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

//-------------------------------------------------------------------------------------------------
// File Name:
//		msAPI_Subtitle_h
// Description:
//		DVB subtitle interface api.
// Note:
//		MStar Semi.
//-------------------------------------------------------------------------------------------------

#ifndef _MSAPI_DVB_SUBTITLE_H
#define _MSAPI_DVB_SUBTITLE_H

//#include "MsTypes.h"
#include "apiGFX.h"
#include "apiGOP.h"
#include "apiMsMWCommon.h"

#include "apiVDEC_EX.h"
#include "apiXC.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SBTL_ERR(fmt, args...)    printf("[%s][%d] " fmt, __FUNCTION__, __LINE__, ## args)
#define SBTL_TRACE(fmt, args...)  //printf("[%s][%d] " fmt, __FUNCTION__, __LINE__, ## args)
#define SBTL_DBG(fmt, args...)    //printf("[%s][%d] " fmt, __FUNCTION__, __LINE__, ## args)
#define SBTL_INFO(fmt, args...)   //printf("[%s][%d] " fmt, __FUNCTION__, __LINE__, ## args)
#define SBTL_TIMER(fmt, args...)  //printf("[%s][%d] " fmt, __FUNCTION__, __LINE__, ## args)
#define SBTL_TEST(fmt, args...)   //printf("[%s][%d] " fmt, __FUNCTION__, __LINE__, ## args)
#define SBTL_TIMER_VAR(x)         //x
#define SBTL_TEST_VAR(x)          //x

typedef enum
{
    SBTL_LIVE_MODE,
    SBTL_FILE_MODE
} SUB_TSP_MODE;

typedef enum
{
    SBTL_OSD_DST_AUTO,
    SBTL_OSD_DST_IP,
    SBTL_OSD_DST_OP,
} SUB_OSD_DST;

typedef struct
{
    MS_U8 u8_R;
    MS_U8 u8_G;
    MS_U8 u8_B;
    MS_U8 u8_A;  // Transparency
} DvbSubPalette_Colour;

typedef enum
{
    SBTL_CONFIG_SD_ONLY, //param: (MS_BOOL*), TRUE or FALSE
} SUB_CONFIG;

typedef struct
{
    MS_U8 u8TspId;
    MS_U16 u16Pid;
    MS_U16 u16CPID;
    MS_U16 u16APDI;
}SUB_STREAM_INFO;

typedef struct
{
    MS_U16 u16CPID;
    MS_U16 u16APID;
}stSub_PageID;

typedef struct /* not complete */
{
    MS_BOOL bAllocFB; /* ask module to allocate FB itself */
    MS_U32 u32FrmBufSize;
    //MS_U16 u16FrmBufWidth;
    //MS_U16 u16FrmBufHeight;
    //MS_ColorFormat eColorFmt; /* color format for off/on-screen buffer */

}stSub_Param;

typedef enum
{
    SUB_TYPE_DVB = 0,
    SUB_TYPE_DC2,
    SUB_TYPE_PGS,

    SUB_TYPE_ALL
}eSub_Type;

typedef enum
{
    /* common command */
    SUB_DECCMD_SET_PID = 0,
    SUB_DECCMD_SET_SD_ONLY,
    SUB_DECCMD_SET_TSP_MODE,
    SUB_DECCMD_SET_DMX_SRC,
    SUB_DECCMD_SET_GOP_NUM,
    SUB_DECCMD_SET_SYS_OP_INTF,

    /* dvb subtitling commands */
    SUB_DECCMD_DVB_CMD_START = 0x200,
    SUB_DECCMD_SET_PAGE_ID = SUB_DECCMD_DVB_CMD_START,
    SUB_DECCMD_DVB_CMD_END = 0x3FF,

    /* DC2 subtitling commands */
    SUB_DECCMD_DC2_CMD_START = 0x400,
    SUB_DECCMD_DC2_CMD_END = 0x5FF,

    SUB_DECCMD_ALL
}eSub_DecCmd;

typedef void* Sub_ObjHandle;

typedef int (*MsMW_SubOsdEnable)(int bEnable);
typedef int (*MsMW_SubOsdAlloc)(GFX_BufferInfo* pBufInfo, GFX_Buffer_Format ColorFmt, unsigned char* pu8Buf, unsigned int u16Width, unsigned int u16Height);
typedef int (*MsMW_SubOsdFree)(void);
typedef int (*MsMW_SubOsdResulotionSet)(unsigned int u16X, unsigned int u16Y, unsigned int u16W, unsigned int u16H,
                                            unsigned int u16ScaledW, unsigned int u16ScaledH, MS_BOOL bHScale, MS_BOOL bVScale);
typedef int (*MsMW_SubOsdReset)(EN_GOP_DST_TYPE eGOPDstType);
typedef int (*MsMW_SubOsdPaletteSet)(unsigned char* pPalArray, unsigned char u32PalCount, unsigned int u32PalOffset);
typedef int (*MsMW_SubOsdDestinationSet)(SUB_OSD_DST *pGOPDstType);
typedef int (*MsMW_SubOsdStreamIDSet)(SUB_STREAM_INFO *pStreamInfo, VDEC_StreamId *pStreamId);

typedef int (*MsMW_SubOsdEnableT2)(Sub_ObjHandle hSubObj, int bEnable);
typedef int (*MsMW_SubOsdAllocT2)(Sub_ObjHandle hSubObj, GFX_BufferInfo* pBufInfo, GFX_Buffer_Format ColorFmt, unsigned char* pu8Buf, unsigned int u16Width, unsigned int u16Height);
typedef int (*MsMW_SubOsdFreeT2)(Sub_ObjHandle hSubObj);
typedef int (*MsMW_SubOsdResulotionSetT2)(Sub_ObjHandle hSubObj, unsigned int u16X, unsigned int u16Y, unsigned int u16W, unsigned int u16H,
                                            unsigned int u16ScaledW, unsigned int u16ScaledH, MS_BOOL bHScale, MS_BOOL bVScale);
typedef int (*MsMW_SubOsdResetT2)(Sub_ObjHandle hSubObj, EN_GOP_DST_TYPE eGOPDstType);
typedef int (*MsMW_SubOsdPaletteSetT2)(Sub_ObjHandle hSubObj, unsigned char* pPalArray, unsigned char u32PalCount, unsigned int u32PalOffset);
typedef int (*MsMW_SubOsdDestinationSetT2)(Sub_ObjHandle hSubObj, SUB_OSD_DST *pGOPDstType);
typedef int (*MsMW_SubOsdStreamIDSetT2)(Sub_ObjHandle hSubObj, SUB_STREAM_INFO *pStreamInfo, VDEC_StreamId *pStreamId);
typedef int (*MsMW_SubOsdScalerWinSetT2)(Sub_ObjHandle hSubObj, SCALER_WIN *pWin);

#if 1

#define SUB_MAX_SUPPORT_WIN (8)
#define SUB_INVALID_WIN_ID (SUB_MAX_SUPPORT_WIN+1)
#define INVALID_WINDOW_HANDLE ((void *)0xFFFFFFFF)

typedef void* MsMW_WinHandle;

typedef enum
{
    MsMW_CLRFMT_I2 = 0,
    MsMW_CLRFMT_I8,
    MsMW_CLRFMT_ARGB4444,
    MsMW_CLRFMT_ARGB8888,

    MsMW_ALL
}MsMW_ColorFmt;

typedef struct
{
    MS_U16 u16X;
    MS_U16 u16Y;
    MS_U16 u16Width;
    MS_U16 u16Height;
}MsMW_Rect;

typedef struct
{
    MS_U8 u8A;  // Transparency
    MS_U8 u8R;
    MS_U8 u8G;
    MS_U8 u8B;
}MsMW_PaletteColour;

typedef enum
{
    MsMW_CK_OP_EQUAL = 0,
    MsMW_CK_OP_NOT_EQUAL
}MsMW_ColorKeyMode;

typedef enum
{
    /// Csrc
    MsMW_COEF_ONE = 0,
    /// Csrc * Aconst + Cdst * (1 - Aconst)
    MsMW_COEF_CONST,
    ///  Csrc * Asrc + Cdst * (1 - Asrc)
    MsMW_COEF_ASRC,
    /// Csrc * Adst + Cdst * (1 - Adst)
    MsMW_COEF_ADST,

    /// Cdst
    MsMW_COEF_ZERO,
    /// Csrc * (1 - Aconst) + Cdst * Aconst
    MsMW_COEF_1_CONST,
    /// Csrc * (1 - Asrc) + Cdst * Asrc
    MsMW_COEF_1_ASRC,
    ///  Csrc * (1 - Adst) + Cdst * Adst
    MsMW_COEF_1_ADST,

    /// ((Asrc * Aconst) * Csrc + (1-(Asrc *Aconst)) * Cdst) / 2
    MsMW_COEF_ROP8_ALPHA,
    /// ((Asrc * Aconst) * Csrc + Adst * Cdst * (1-(Asrc * Aconst))) / (Asrc * Aconst) + Adst * (1- Asrc * Aconst))
    MsMW_COEF_ROP8_SRCOVER,
    /// ((Asrc * Aconst) * Csrc * (1-Adst) + Adst * Cdst) / (Asrc * Aconst) * (1-Adst) + Adst)
    MsMW_COEF_ROP8_DSTOVER,

    /// Csrc * Aconst
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_CONST_SRC</em></b>
    MsMW_COEF_CONST_SRC,
    /// Csrc * (1 - Aconst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_1_CONST_SRC</em></b>
    MsMW_COEF_1_CONST_SRC,

    /// Csrc * Adst * Asrc * Aconst + Cdst * Adst * (1 - Asrc * Aconst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_SRC_ATOP_DST</em></b>
    MsMW_COEF_SRC_ATOP_DST,
    /// Cdst * Asrc * Aconst * Adst + Csrc * Asrc * Aconst * (1 - Adst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_DST_ATOP_SRC</em></b>
    MsMW_COEF_DST_ATOP_SRC,
    /// (1 - Adst) * Csrc * Asrc * Aconst + Adst * Cdst * (1 - Asrc * Aconst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_SRC_XOR_DST</em></b>
    MsMW_COEF_SRC_XOR_DST,
} MsMW_BlendCoef;

typedef enum
{
    /// constant
    MsMW_ABL_FROM_CONST = 0,
    /// source alpha
    MsMW_ABL_FROM_ASRC,
    /// destination alpha
    MsMW_ABL_FROM_ADST,
    /// Aout = Asrc*Aconst
    MsMW_ABL_FROM_ROP8_SRC,
    /// Aout = Asrc*Aconst * Adst
    MsMW_ABL_FROM_ROP8_IN,
    /// Aout = (1-Asrc*Aconst) * Adst
    MsMW_ABL_FROM_ROP8_DSTOUT,
    /// Aout = (1-Adst) * Asrc*Aconst
    MsMW_ABL_FROM_ROP8_SRCOUT,
    /// Aout = (Asrc*Aconst) + Adst*(1-Asrc*Aconst) or (Asrc*Aconst)*(1-Adst) + Adst
    MsMW_ABL_FROM_ROP8_OVER,

    /// 1 - Aconst
    MsMW_ABL_FROM_ROP8_INV_CONST,
    /// 1 - Asrc
    MsMW_ABL_FROM_ROP8_INV_ASRC,
    /// 1 - Adst
    MsMW_ABL_FROM_ROP8_INV_ADST,
    /// Adst * Asrc * Aconst + Adst * (1 - Asrc * Aconst) A atop B
    MsMW_ABL_FROM_ROP8_SRC_ATOP_DST,
    /// Asrc * Aconst * Adst + Asrc * Aconst * (1 - Adst) B atop A
    MsMW_ABL_FROM_ROP8_DST_ATOP_SRC,
    /// (1 - Adst) * Asrc * Aconst + Adst * (1 - Asrc * Aconst) A xor B
    MsMW_ABL_FROM_ROP8_SRC_XOR_DST,
    /// Asrc * Asrc * Aconst + Adst * (1 - Asrc * Aconst)
    MsMW_ABL_FROM_ROP8_INV_SRC_ATOP_DST,
    /// Asrc * (1 - Asrc * Aconst) + Adst * Asrc * Aconst
    MsMW_ABL_FROM_ROP8_INV_DST_ATOP_SRC
} MsMW_AlphaSrcFrom;

typedef struct
{
    MS_VIRT virtBufAddr;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MsMW_ColorFmt eColor;
    MS_BOOL bDisplayAble;
}MsMW_RndWinAttr;

typedef struct
{
    MS_BOOL bEnable;
    MsMW_BlendCoef eCoef;
    MS_U8 u8Const;
    MsMW_AlphaSrcFrom eAlphaSrc;
}MsMW_AlphaInfo;

typedef struct
{
    MS_BOOL bEnable;
    MsMW_ColorKeyMode eOpMode;
    MsMW_ColorFmt eFmt;
    void *pvColorStart;
    void *pvColorEnd;
}MsMW_ClrKeyInfo;

typedef struct
{
    MsMW_PaletteColour *pPalArray;
    MS_U16 u16PalStart;
    MS_U16 u16PalEnd;
}MsMW_PaletteInfo;

typedef struct
{
    /* source and destination buffer parameters */
    MsMW_WinHandle hSrcWin;
    MsMW_WinHandle hDstWin;

    /* alpha blending parameters */
    MsMW_AlphaInfo stAlpha;

    /* color key parameters */
    MsMW_ClrKeyInfo stSrcClrKey;
    MsMW_ClrKeyInfo stDstClrKey;

    /* palette parameter */
    MsMW_PaletteInfo stPalette;

    /* clip parameter */
    MsMW_Rect stClipRect;
}MsMW_BitbltParam;

typedef struct
{
    MsMW_Rect stSrcRect;
    MsMW_Rect stDstRect;
}MsMW_DrawRect;

/// the callback for open
typedef MS_BOOL (*MsMW_Render_Open)(void *pvArg);
/// the callback for close
typedef MS_BOOL (*MsMW_Render_Close)(void *pvArg);
/// the callback for create window
typedef MsMW_WinHandle (*MsMW_Render_CreateWindow)(MsMW_RndWinAttr *ptWinAttr, void *pvArg);
/// the callback for destroy window
typedef MS_BOOL (*MsMW_Render_DestroyWindow)(MsMW_WinHandle hWinHnd, void *pvArg);
/// the callback for set palette
typedef MS_BOOL (*MsMW_Render_SetPalette)(MsMW_WinHandle hWinHnd, MS_U8* pPalArray, MS_U16 u16PaletteSize, MS_U8 u8Index, void *pvArg);
/// the callback for bitblt
typedef MS_BOOL (*MsMW_Render_Bitblt)(MsMW_DrawRect *pstRect, MsMW_BitbltParam *pstParam, void *pvArg);
/// the callback for clear window by color index
typedef MS_BOOL (*MsMW_Render_ClearWindowByIndex)(MsMW_WinHandle hWinHnd, MsMW_Rect *pRect, MS_U8 u8ColorIndex, void *pvArg);
/// the callback for clear window by color
typedef MS_BOOL (*MsMW_Render_ClearWindowByColor)(MsMW_WinHandle hWinHnd, MsMW_Rect *pRect, MsMW_PaletteColour *pColor, void *pvArg);
//  the callback for getting display size
typedef MS_BOOL (*MsMW_Render_GetDispalySize)(MsMW_Rect *pRect, void *pvArg);
/// the callback for show
typedef MS_BOOL (*MsMW_Render_Show)(MsMW_WinHandle phWinHnd, MS_BOOL bShow, void *pvArg);

typedef struct MsMW_SubRndOp
{
    MsMW_Render_Open               pRndOpen;
    MsMW_Render_Close              pRndClose;
    MsMW_Render_CreateWindow       pRndCreateWindow;
    MsMW_Render_DestroyWindow      pRndDestroyWindow;
    MsMW_Render_SetPalette         pRndSetPalette;
    MsMW_Render_Bitblt             pRndBitblt;
    MsMW_Render_ClearWindowByIndex pRndClearWindowByIndex;
    MsMW_Render_ClearWindowByColor pRndClearWindowByColor;
    MsMW_Render_GetDispalySize     pRndGetDisplaySize;
    MsMW_Render_Show               pRndShow;
} MsMW_RndOp_t; /* renderer operation */
#endif

/* for backward compatible */
typedef struct MsMW_SubOsdOp
{
    MsMW_SubOsdAlloc            pOsdAlloc;
    MsMW_SubOsdFree             pOsdFree;
    MsMW_SubOsdReset            pOsdReset;
    MsMW_SubOsdResulotionSet    pOsdResulotionSet;
    MsMW_SubOsdEnable           pOsdEnable;
    MsMW_SubOsdPaletteSet       pOsdPaletteSet;
    MsMW_SubOsdDestinationSet   pOsdDestinationSet;
    MsMW_SubOsdStreamIDSet      pOsdStreamIDSet;
} MsMW_SubOsdOp_t;

#if 0
typedef struct MsMW_SubOsdOpT2
{
    MsMW_SubOsdAllocT2            pOsdAlloc;
    MsMW_SubOsdFreeT2             pOsdFree;
    MsMW_SubOsdResetT2            pOsdReset;
    MsMW_SubOsdResulotionSetT2    pOsdResulotionSet;
    MsMW_SubOsdEnableT2           pOsdEnable;
    MsMW_SubOsdPaletteSetT2       pOsdPaletteSet;
    MsMW_SubOsdDestinationSetT2   pOsdDestinationSet;
    MsMW_SubOsdStreamIDSetT2      pOsdStreamIDSet;
    MsMW_SubOsdScalerWinSetT2     pOsdScalerWinSet;
} MsMW_SubOsdOpT2_t;
#endif

typedef struct MsMW_SubMemOp
{
    MsMW_Alloc                  pAllocCache;
    MsMW_Free                   pFreeCache;
    MsMW_Alloc                  pAllocNonCache;
    MsMW_Free                   pFreeNonCache;
    MsMW_VA2PA                  pVa2Pa;
    MsMW_PA2VA                  pPa2Va;
} MsMW_SubMemOp_t;

typedef MsMW_SubMemOp_t MsMW_SubMemOpT2_t;

typedef int (*MsMW_SubTspFlowSet)(void);

extern Sub_ObjHandle msAPI_Subtitle_CreateObject(eSub_Type eType);
extern MS_BOOL msAPI_Subtitle_ReleaseObject(Sub_ObjHandle pHandle);
#if 1
MS_BOOL msAPI_Subtitle_Init_EX(Sub_ObjHandle pHandle, stSub_Param *pstParam, MsMW_RndOp_t* pRndOp, MsMW_SubMemOp_t* pMemOp);
#else
extern MS_BOOL msAPI_Subtitle_Init_EX(Sub_ObjHandle pHandle, MsMW_SubOsdOpT2_t* pOsdOp, MsMW_SubMemOpT2_t* pMemOp);
#endif
extern MS_BOOL msAPI_Subtitle_DeInit_EX(Sub_ObjHandle pHandle);
extern MS_BOOL msAPI_Subtitle_Reset_EX(Sub_ObjHandle pHandle);
extern MS_BOOL msAPI_Subtitle_Start_EX(Sub_ObjHandle pHandle);
extern MS_BOOL msAPI_Subtitle_Stop_EX(Sub_ObjHandle pHandle);
extern MS_BOOL msAPI_Subtitle_Clear_EX(Sub_ObjHandle pHandle);
extern MS_BOOL msAPI_Subtitle_Show_EX(Sub_ObjHandle pHandle, MS_BOOL bShow);
extern MS_BOOL msAPI_Subtitle_DoCmd(Sub_ObjHandle pHandle, eSub_DecCmd eCmd, void *pvParam, MS_U32 u32ParamSize);

/*
  look up corresponding queue object for those use case which push data directly into queue.
  this function will be deprecated once the feeder callback interface is implemented.
 */
extern void *msAPI_Subtitle_GetQueueObj(Sub_ObjHandle pHandle);

/* for backward compatible */
extern MS_BOOL msAPI_Subtitle_Start(void);
extern MS_BOOL msAPI_Subtitle_Stop(void);
extern MS_BOOL msAPI_Subtitle_Clear(void);
extern MS_BOOL msAPI_Subtitle_EnableDisplay(MS_BOOL blEnableDisplay);
extern MS_BOOL msAPI_Subtitle_Init(MsMW_SubOsdOp_t* pOsdOp, MsMW_SubMemOp_t* pMemOp);
extern MS_BOOL msAPI_Subtitle_DeInit(void);
extern MS_BOOL msAPI_Subtitle_Reset(void);
extern MS_BOOL msAPI_Subtitle_SetGOPNum(MS_U8 u8GOP);
extern MS_BOOL msAPI_Subtitle_SetPageIDs( MS_U16 u16CPID, MS_U16 u16APID );
extern MS_BOOL msAPI_Subtitle_SetPid(MS_U16 u16Pid);
extern MS_BOOL msAPI_Subtitle_SetTSPMode(SUB_TSP_MODE eSetTSPMode);
extern MS_BOOL msAPI_Subtitle_SetDMXFltSrc(MS_U32 u32DmxFltSrc);
extern MS_BOOL msAPI_Subtitle_SetConfig(SUB_CONFIG eConfig, void *pSetValue);
extern MS_BOOL msAPI_Subtitle_SetDVBMode(MS_BOOL bDVB_Mode);
extern MS_BOOL msAPI_Subtitle_GetDVBMode(void);

#define GOP_RENDER 0

#ifdef __cplusplus
}
#endif

#endif // _MSAPI_SUBTITLE_H
