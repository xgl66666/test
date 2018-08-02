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

#ifndef __porting_graphic__
#define __porting_graphic__

/*@ <Include> @*/
#include "mmsdk_interface_def.h"
/*@ </Include> @*/


#ifdef __cplusplus
extern "C"
{
#endif

/// @file
/// @defgroup porting_graphic porting_graphic functions
/// @{

/*@ <Definitions> @*/

/// define graphic multiple-instance item
typedef void* PT_GRAPHICITEM;


/// Photo draw and blit option flag
/// since version 1.01
typedef enum {
    /// None.
    E_MMSDK_DSRO_NONE                  = 0x00000000,
    /// Upscale for StretchBlit().
    E_MMSDK_DSRO_SMOOTH_UPSCALE        = 0x00000001,
    /// Downscale for StretchBlit().
    E_MMSDK_DSRO_SMOOTH_DOWNSCALE      = 0x00000002,
    /// DFB Transformation matrix.
    E_MMSDK_DSRO_MATRIX                = 0x00000004,
    /// Enable anti-aliasing.
    E_MMSDK_DSRO_ANTIALIAS             = 0x00000008,
    /// All.
    E_MMSDK_DSRO_ALL                   = 0x0000000F
} EN_MMSDK_DFBSurfaceRenderOptions;

/// Blit controlling flag.
/// since version 1.01
typedef enum {
     /// None
     E_MMSDK_DSBLIT_NOFX               = 0x00000000,
     /// Blending with alphachannel
     E_MMSDK_DSBLIT_BLEND_ALPHACHANNEL = 0x00000001,
     /// Blending with alpha value
     E_MMSDK_DSBLIT_BLEND_COLORALPHA   = 0x00000002,
     /// Modulates color with the rgb values
     E_MMSDK_DSBLIT_COLORIZE           = 0x00000004,
     /// Matching the source color key then blit
     E_MMSDK_DSBLIT_SRC_COLORKEY       = 0x00000008,
     /// If the dst pixel matches the dst color then write to destination only
     E_MMSDK_DSBLIT_DST_COLORKEY       = 0x00000010,
     /// Modulates the source color with the (modulated) source alpha
     E_MMSDK_DSBLIT_SRC_PREMULTIPLY    = 0x00000020,
     /// Modulates the dest. color with the dest. alpha
     E_MMSDK_DSBLIT_DST_PREMULTIPLY    = 0x00000040,
     /// Divides the color by the alpha before writing the data to the destination
     E_MMSDK_DSBLIT_DEMULTIPLY         = 0x00000080,
     /// Deinterlaces the source during blitting by reading only one field
     /// (every second line of full image) scaling it vertically by factor two
     E_MMSDK_DSBLIT_DEINTERLACE        = 0x00000100,
     /// Modulates the source color with the color alpha
     E_MMSDK_DSBLIT_SRC_PREMULTCOLOR   = 0x00000200,
     /// xor bitwise the dst and src pixels after premultiplication
     E_MMSDK_DSBLIT_XOR                = 0x00000400,
     /// Fast indexed translation
     E_MMSDK_DSBLIT_INDEX_TRANSLATION  = 0x00000800,
     /// rotate 90 degree
     E_MMSDK_DSBLIT_ROTATE90           = 0x00002000,
     /// rotate 180 degree
     E_MMSDK_DSBLIT_ROTATE180          = 0x00001000,
     /// rotate 270 degree
     E_MMSDK_DSBLIT_ROTATE270          = 0x00004000,
     /// Write pixels when the color key isn't matched
     E_MMSDK_DSBLIT_COLORKEY_PROTECT   = 0x00010000,
     /// Modulate source alpha channel with source mask alpha channel
     E_MMSDK_DSBLIT_SRC_MASK_ALPHA     = 0x00100000,
     /// Modulate source color channels with source mask color channels
     E_MMSDK_DSBLIT_SRC_MASK_COLOR     = 0x00200000,
     /// Enables blend with alphachannel.
     E_MMSDK_DSBLIT_BLEND_ALPHACHANNEL_2 = 0x00400000,
} EN_MMSDK_DFBSurfaceBlittingFlags;

//------------------------------------------------------------------------------
/// Create graphic surface, and get graphic instance
/// @param pSurface             \b OUT:  return graphic intance item
/// @param eFormat              \b IN: photo format
/// @param u32Width             \b IN:
/// @param u32Height            \b IN:
/// @param u32BuffAddr         \b IN:
/// @param u32Pitch               \b IN: pitch
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_CreateSurface(PT_GRAPHICITEM* pSurface, EN_MMSDK_COLOR_FORMAT eFormat, MMSDK_U32 u32Width, MMSDK_U32 u32Height, MMSDK_U32 u32BuffAddr, MMSDK_U32 u32Pitch);

//------------------------------------------------------------------------------
/// Release graphic resource
/// @param pSurface             \b IN: graphic intance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_Release(PT_GRAPHICITEM* pSurface);

//------------------------------------------------------------------------------
/// Set the clipping region used to limit the area for drawing and blitting.
/// @param Surface              \b IN: graphic intance item
/// @param Rect                 \b IN: limited rectangle
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_SetClip(PT_GRAPHICITEM Surface, ST_MMSDK_RECT Rect);

//------------------------------------------------------------------------------
/// Clear and set color of graphic surface
/// @param Surface              \b IN: graphic intance item
/// @param u8A \b IN: Background color for aplha
/// @param u8R \b IN: Background color for red.
/// @param u8G \b IN: Background color for green.
/// @param u8B \b IN: Background color for green.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_Clear(PT_GRAPHICITEM Surface, const MMSDK_U8 u8A, const MMSDK_U8 u8R, const MMSDK_U8 u8G, const MMSDK_U8 u8B);
//------------------------------------------------------------------------------
/// Set the flags for all subsequent blitting commands.
/// @param Surface         \b IN: graphic intance item
/// @param eAngle          \b IN: rotate angle
/// @param u32BlitFlags    \b IN: Flags controlling blitting commands
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_SetBlittingFlags(PT_GRAPHICITEM Surface, EN_MMSDK_ROTATE_ANGLE eAngle, MMSDK_U32 u32BlitFlags);

//------------------------------------------------------------------------------
/// Set options affecting the output of drawing and blitting operations.
/// @param Surface                        \b IN: graphic intance item
/// @param u32DFBSurfaceRenderOptions     \b IN: Options for drawing and blitting operations. Not mandatory for acceleration.
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_SetRenderOptions(PT_GRAPHICITEM Surface, MMSDK_U32 u32DFBSurfaceRenderOptions);

//------------------------------------------------------------------------------
/// Blit an area scaled from the source to the destination rectangle.
/// @param DesSurface              \b IN: destmation(graphic intance item)
/// @param SrcSurface              \b IN: source(graphic intance item)
/// @param SrcRect              \b IN: source rectangle
/// @param DesRect              \b IN: destmation rectangle
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_StretchBlit(PT_GRAPHICITEM DesSurface, PT_GRAPHICITEM SrcSurface, ST_MMSDK_RECT * SrcRect, ST_MMSDK_RECT * DesRect);

//------------------------------------------------------------------------------
/// Dump the contents of the surface to one or two files.
/// @param Surface              \b IN: graphic intance item
/// @param directory            \b IN: file saved path
/// @param prefix               \b IN: prefix for file name
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_Dump(PT_GRAPHICITEM Surface, const char * directory, const char * prefix);

//------------------------------------------------------------------------------
/// Set alpha value of graphic surface
/// @param Surface              \b IN: graphic intance item
/// @param u16Alpha             \b IN: alpha value
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_SetAlpha(PT_GRAPHICITEM Surface, MMSDK_U16 u16Alpha);

//------------------------------------------------------------------------------
/// Get graphic surface information
/// @param Surface              \b IN: graphic intance item
/// @param surface_width        \b IN: surface's width in pixels
/// @param surface_height       \b IN: surface's height in pixels
/// @param surface_format       \b IN: surface's format
/// @param ptr_data             \b IN: a data pointer
/// @param surface_pitch        \b IN: surface's pitch
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_GetSurfaceInfo(PT_GRAPHICITEM Surface, MMSDK_U32 *surface_width, MMSDK_U32 *surface_height, EN_MMSDK_COLOR_FORMAT *surface_format, void**ptr_data, MMSDK_U32 *surface_pitch);

//------------------------------------------------------------------------------
/// Transfer YUV(YCbCr) to RGB
/// @param Surface              \b IN: graphic intance item
/// @param width                \b IN: surface's width in pixels
/// @param height               \b IN: surface's height in pixels
/// @param pitch                \b IN: surface's pitch
/// @param imgRGB               \b IN: a data pointer(RGB format)
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_YUV2RGB(PT_GRAPHICITEM Surface, MMSDK_U32 width, MMSDK_U32 height, MMSDK_U32 pitch, MMSDK_U8 *imgRGB);

/*@ </Definitions> @*/

/// @} // end of porting_graphic

#ifdef __cplusplus
}
#endif
#endif // __porting_graphic__
///*** please do not remove change list tag ***///
///***$Change: 1179505 $***///
