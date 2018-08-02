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

#ifndef __mw_porting_gfx__
#define __mw_porting_gfx__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "apiGFX.h"

typedef struct
{
    MS_BOOL bEnable;
    GFX_BlendCoef eCoef;
    MS_U8 u8Const;
    GFX_AlphaSrcFrom eAlphaSrc;
}stMPL_GFX_AlphaInfo;

typedef struct
{
    MS_BOOL bEnable;
    GFX_ColorKeyMode eOpMode;
    GFX_Buffer_Format eFmt;
    void *pvColorStart;
    void *pvColorEnd;
}stMPL_GFX_ClrKeyInfo;

typedef struct
{
    GFX_PaletteEntry *pPalArray;
    MS_U16 u16PalStart;
    MS_U16 u16PalEnd;
}stMPL_GFX_PaletteInfo;

typedef struct
{
    /* source and destination buffer parameters */
    GFX_BufferInfo *pstSrcBuf;
    GFX_BufferInfo *pstDstBuf;

    /* alpha blending parameters */
    stMPL_GFX_AlphaInfo stAlpha;

    /* color key parameters */
    stMPL_GFX_ClrKeyInfo stSrcClrKey;
    stMPL_GFX_ClrKeyInfo stDstClrKey;

    /* palette parameter */
    stMPL_GFX_PaletteInfo stPalette;

    /* DFB parameters */
    MS_BOOL bIsDfbBlend;
    GFX_DFBBldOP eSrcDfbBlendMode;
    GFX_DFBBldOP eDstDfbBlendMode;

    /* YUV parameters */
    GFX_YUV_422 eSrcYUVFmt;
    GFX_YUV_422 eDstYUVFmt;

    /* clip parameter */
    GFX_Block stClipRect;
}stMPL_GFX_BitbltParam;

//-------------------------------------------------------------------------------------------------
/// This function will fill a specific 8-bit pattern in a specific buffer
/// @param StrAddr             \b IN: Physical start address. Byte alignment
/// @param length               \b IN: Buffer length. Byte alignment
/// @param ClearValue        \b IN: 8 bit pattern
/// @return GFX_SUCCESS - Success
/// @return GFX_INIT_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_ClearFrameBuffer(MS_PHYADDR StrAddr, MS_U32 length, MS_U8 ClearValue);

//-------------------------------------------------------------------------------------------------
/// This function will fill a specific 32-bit pattern in a specific buffer
/// @param StrAddr              \b IN: Physical start address. 4-Byte alignment
/// @param length                \b IN: Buffer length. 4-Byte alignment
/// @param ClearValue         \b IN: 32-bit pattern
/// @return GFX_SUCCESS - Success
/// @return GFX_INIT_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_ClearFrameBufferByWord(MS_PHYADDR StrAddr, MS_U32 length, MS_U32 ClearValue);

//-------------------------------------------------------------------------------------------------
/// Flush GFX queue
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_FlushQueue(void);

//-------------------------------------------------------------------------------------------------
/// Begin GFX Engine drawing
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_BeginDraw(void);

//-------------------------------------------------------------------------------------------------
/// End GFX engine drawing
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_EndDraw(void);

//-------------------------------------------------------------------------------------------------
/// Set GFX clipping window
/// @param v0           \b IN: Left-top position
/// @param v1           \b IN: Right-down position
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetClip(GFX_Point* v0, GFX_Point* v1);

//-------------------------------------------------------------------------------------------------
/// Get GFX clipping window
/// @param v0           \b IN: Left-top position
/// @param v1           \b IN: Right-down position
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_GetClip(GFX_Point* v0, GFX_Point* v1);

//-------------------------------------------------------------------------------------------------
/// Enable GFX NearestMode
/// @param enable           \b IN: True/False
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetNearestMode(MS_BOOL enable);

//-------------------------------------------------------------------------------------------------
/// Enable GFX alpha blending
/// @param enable           \b IN: True/False
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_EnableAlphaBlending(MS_BOOL enable);

//-------------------------------------------------------------------------------------------------
/// Disable/Enable and set Alpha Blending parameters.
/// @param enable            \b IN : Enable/Disable
/// @param coef                \b IN  Source coefficient for blending. (source blending mode)
/// @param db_abl            \b IN  Source of Destination Alpha.
/// @param abl_const       \b IN  Alpha Blending constant
/// @return GFX_SUCCESS - Success
/// @return GFX_INIT_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetAlpha(MS_BOOL enable, GFX_BlendCoef coef, GFX_AlphaSrcFrom db_abl, MS_U8 abl_const);

//-------------------------------------------------------------------------------------------------
/// Set GFX palette table
/// @param pPalArray            \b IN: Array of GFX_PaletteEntry
/// @param u32PalStart        \b IN: Palette index start
/// @param u32PalEnd          \b IN: Palette index end
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetPaletteOpt( GFX_PaletteEntry *pPalArray, MS_U16 u16PalStart, MS_U16 u16PalEnd);

//-------------------------------------------------------------------------------------------------
/// Set PE alpha blending
/// @param blendcoef                    \b IN: Alpha source from
/// @param u8ConstantAlpha        \b IN: Contant alpha
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetAlphaBlending(GFX_BlendCoef blendcoef, MS_U8 blendfactor);

//-------------------------------------------------------------------------------------------------
/// Set GFX alpha source
/// @param eMode                        \b IN: Alpha source
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetAlphaSrcFrom(GFX_AlphaSrcFrom eMode);

//-------------------------------------------------------------------------------------------------
/// Set GFX Engine dither
/// @param  enable                      \b IN: True/False
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetDither(MS_BOOL enable);

//-------------------------------------------------------------------------------------------------
/// Set GFX line pattern
/// @param enable                       \b IN: True/False
/// @param linePattern                 \b IN: p0-0x3F one bit represent draw(1) or not draw(0)
/// @param repeatFactor              \b IN: 0 : Repeat factor
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_Set_Line_Pattern(MS_BOOL enable, MS_U8 linePattern, MS_U8 repeatFactor);

//-------------------------------------------------------------------------------------------------
/// GFX draw line
/// @param pline                            \b IN: Line info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_DrawLine(GFX_DrawLineInfo *pline);

//-------------------------------------------------------------------------------------------------
/// GFX rectangle fill
/// @param pfillblock                       \b IN: Block info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_RectFill(GFX_RectFillInfo *pfillblock);

//-------------------------------------------------------------------------------------------------
/// Set PE source color key
/// @param enable                           \b IN: True/False
/// @param opMode                         \b IN: Src color key mode
/// @param fmt                                 \b IN: Src color key format
/// @param ps_color                         \b IN: Src color key start
/// @param pe_color                         \b IN: Src color key end
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetSrcColorKey(MS_BOOL enable, GFX_ColorKeyMode opMode,
                                      GFX_Buffer_Format fmt, void *ps_color, void *pe_color);

//-------------------------------------------------------------------------------------------------
/// Set PE destination color key
/// @param enable                           \b IN: True/False
/// @param opMode                         \b IN: Dst color key mode
/// @param fmt                                 \b IN: Dst color key format
/// @param ps_color                         \b IN: Dst color key start
/// @param pe_color                         \b IN: Dst color key end
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetDstColorKey(MS_BOOL enable, GFX_ColorKeyMode opMode,
                                      GFX_Buffer_Format fmt, void *ps_color, void *pe_color);

//-------------------------------------------------------------------------------------------------
/// Set GFX Bit blt
/// @param drawbuf                          \b IN: Drawbuf info
/// @param drawflag                         \b IN: Draw flag
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_BitBlt(GFX_DrawRect *drawbuf, MS_U32 drawflag);
GFX_Result MPL_GFX_BitBlt_Ex(GFX_DrawRect *ptDrawRect, stMPL_GFX_BitbltParam *ptParam);

//-------------------------------------------------------------------------------------------------
/// Set GFX italic
/// @param enable                           \b IN: True/False
/// @param ini_line                           \b IN: Line start to get italic effect
/// @param ini_dis                            \b IN: Pixel start to get italic effect
/// @param delta                              \b IN: Italic delta 0-0x1f
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetItalic(MS_BOOL enable, MS_U8 ini_line, MS_U8 ini_dis, MS_U8 delta);

//-------------------------------------------------------------------------------------------------
/// GFX drawing characters
/// @param fhandle                          \b IN: Font handle
/// @param pindex                           \b IN: Character index array
/// @param strwidth                         \b IN: Character index length
/// @param pfmt                               \b IN: Text out info
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_FONT_HANDLE - Invalid font handle
/// @return GFX_INVALID_PARAMETERS - Inavlid input parameters
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_TextOut(MS_S32 fhandle, MS_U8 *pindex, MS_U32 strwidth, GFX_TextOutInfo *pfmt);

//-------------------------------------------------------------------------------------------------
/// GFX text out one character
/// @param pChar                              \b IN: Character information
/// @param pfmt                                 \b IN: Text out info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_CharacterOut(GFX_CharInfo* pChar, GFX_TextOutInfo *pfmt);

//-------------------------------------------------------------------------------------------------
/// Set PE intensity
/// @param id                                   \b IN: Id of intensity
/// @param fmt                                 \b IN: Intensity color format
/// @param pColor                            \b IN: Intensity data
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_INVALID_INTENSITY_ID - Inavlid index
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetIntensity(MS_U32 id, GFX_Buffer_Format fmt, MS_U32 *pColor);

//-------------------------------------------------------------------------------------------------
/// Get PE intensity
/// @param id                                   \b IN: Id of intensity
/// @param pColor                            \b IN: Start of intensity
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_INVALID_INTENSITY_ID - Inavlid index
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_GetIntensity(MS_U32 id, MS_U32 *pColor);

//-------------------------------------------------------------------------------------------------
/// Query  text display length
/// @param fhandle                          \b IN: Font handle
/// @param pu8index                       \b IN: Character index array
/// @param u32strwidth                   \b IN: Character index length
/// @param pfmt                               \b IN: Text out info
/// @param pu32DispLength            \b OUT: Display length
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_FONT_HANDLE - Invalid font handle
/// @return GFX_INVALID_PARAMETERS - Inavlid input parameters
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_QueryTextDispLength(MS_S32 fhandle, MS_U8 *pu8index, MS_U32 u32strwidth,
                                               GFX_TextOutInfo *pfmt, MS_U32* pu32DispLength);

//-------------------------------------------------------------------------------------------------
/// GFX draw bitmap
/// @param handle                             \b IN: Handle of bitmap
/// @param pbmpfmt                          \b IN: Bitmap format
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_INVALID_BMP_HANDLE - Invalid bitmap handle
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_DrawBitmap(MS_S32 handle, GFX_DrawBmpInfo *pbmpfmt);

//-------------------------------------------------------------------------------------------------
/// Set GFX source buffer info
/// @param bufInfo                              \b IN: Buffer handle
/// @param offsetofByte                      \b IN: Start offset (should be 128 bit aligned)
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_NON_ALIGN_PITCH - The pitch is not 16 bytes alignment
/// @return GFX_Result_NON_ALIGN_ADDRESS - The address is not 16 bytes alignment
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetSrcBufferInfo(GFX_BufferInfo *bufInfo, MS_U32 offsetofByte);

//-------------------------------------------------------------------------------------------------
/// Set GFX destination buffer info
/// @param bufInfo                              \b IN: Buffer handle
/// @param offsetofByte                      \b IN: Start offset (should be 128 bit aligned)
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_NON_ALIGN_PITCH - The pitch is not 16 bytes alignment
/// @return GFX_Result_NON_ALIGN_ADDRESS - The address is not 16 bytes alignment
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetDstBufferInfo(GFX_BufferInfo *bufInfo, MS_U32 offsetofByte);

//-------------------------------------------------------------------------------------------------
/// Enable GFX DFB blending
/// @param enable                               \b IN: True/False
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_EnableDFBBlending(MS_BOOL enable);

//-------------------------------------------------------------------------------------------------
/// Set GFX DFB blending operations
/// @param gfxSrcBldOP                      \b IN: Source blending op
/// @param gfxDstBldOP                      \b IN: Dst blending op
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetDFBBldOP(GFX_DFBBldOP gfxSrcBldOP, GFX_DFBBldOP gfxDstBldOP);

//-------------------------------------------------------------------------------------------------
/// Enable GFX capability
/// @param eCapType                         \b IN: GFX capability type
/// @param pRet                                 \b OUT: GFX capability
/// @param ret_size                            \b IN: GFX capability size
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_PARAMETERS - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_GetGECaps(EN_GFX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);

//-------------------------------------------------------------------------------------------------
/// Get GFX buffer info
/// @param srcbufInfo                           \b IN: Src buffer info.
/// @param dstbufInfo                           \b IN: Dst buffer info.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_GetBufferInfo(PGFX_BufferInfo srcbufInfo, PGFX_BufferInfo dstbufInfo);

//-------------------------------------------------------------------------------------------------
/// Set alpha parameter for ARGB1555.
/// @param coef                                     \b IN:  Alpha coefficient
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetAlpha_ARGB1555(MS_U8 coef);

//-------------------------------------------------------------------------------------------------
///Get alpha parameter for ARGB1555.
/// @param coef                                     \b IN:  Alpha coefficient
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_GetAlpha_ARGB1555(MS_U8* coef);

//-------------------------------------------------------------------------------------------------
/// GFX Ddraw a Rectangle Frame
/// @param *pblock                                \b IN: Block info
/// @param *plinefmt                             \b IN: Line info
/// @return  GFX_SUCCESS if sucess.
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_RectFrame(GFX_RectFillInfo *pblock, GFX_DrawLineInfo *plinefmt);

//-------------------------------------------------------------------------------------------------
/// Configure GFX Patch mode
/// @param enable                                \b IN: True/False
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MPL_GFX_SetPatchMode(MS_BOOL enable);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__mw_porting_gfx__
