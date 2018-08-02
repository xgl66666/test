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
//------------------------------------------------------------------------------
//
//    Portions of this software are copyright 2007 The FreeType
//    Project (www.freetype.org).  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// @file apiMvf.h (mvf.h)
/// @brief MStar Vector Font Engine module.
/// @author MStar Semiconductor Inc.
///
///  Vector Font Engine main API.
///
/// Features:
/// - Main functions and structures for calling vector fonts APIs
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __API_MVF_H__
#define __API_MVF_H__

//////////////////////////////////////////////////////////////////////////
// Debug Options

//#define MVF_DEBUG

#if defined( _MSC_VER ) //defined for Visual C
#include <assert.h>
#ifndef ASSERT
#define ASSERT(x)  assert(x)
#endif //ASSERT
#endif //defined( _MSC_VER )


//////////////////////////////////////////////////////////////////////////
// Memory Management

#if defined( _MSC_VER ) //defined for Visual C
#include <stdlib.h>
#include <string.h>
#define MVF_MALLOC(s)  malloc(s)
#define MVF_FREE(s)    free(s)
#define MVF_MEM_SET( d, s, c )  memset( d, s, c )
#define MVF_MEM_ZERO( dest, count )  memset( dest, 0, count )
#define MVF_MEM_COPY( dest, source, count )  memcpy( dest, source, count )
#else //defined( _MSC_VER )
#include "MsCommon.h"
#include "MsTypes.h"
#if 1//ATSC_CC_ENABLE //PATA test
#define MVF_MALLOC(s)  MsOS_AllocateMemory(s, gs32CachedPoolID)
#define MVF_FREE(s)    MsOS_FreeMemory(s, gs32CachedPoolID)
#else
#define MVF_MALLOC(s) MsOS_AllocateMemory(s, gs32CachedPoolID);printf("alloc mem 0x%x\n",s);
//#define MVF_MALLOC(s) MsOS_AllocateMemory(s, gs32NonCachedPoolID)
#define MVF_FREE(s)    MsOS_FreeMemory(s, gs32CachedPoolID)
//#define MVF_FREE(s)    MsOS_FreeMemory(s, gs32NonCachedPoolID)
#endif
#define MVF_MEM_SET( d, s, c )  memset( d, s, c )
#define MVF_MEM_ZERO( dest, count )  memset( dest, 0, count )
#define MVF_MEM_COPY( dest, source, count )  memcpy( dest, source, count )
#endif //defined( _MSC_VER )


//////////////////////////////////////////////////////////////////////////
// Font Engine Options
#define MVF_MAX_FONT_SIZE       120
#define MVF_RENDER_POOL_SIZE  4096L   //--> Original

//NOTE: use this constant to enable QMEMORY in AEON,
//      QMEMORY size should be large or equal than MVF_RENDER_POOL_SIZE
//      remark this macro in the general case
//#define MVF_RENDER_POOL_START_ADDR 0xC0000000

//#define MVF_BITMAP_POOL_SIZE  (1*1024L) //--> Original
#define MVF_BITMAP_POOL_SIZE  (8*1024L)

//#define MVF_OUTLINE_BUFFER_SIZE (8*1024L)

#define MVF_ADDR_ALIGN(x) (((x)+3)&(~3)) //for MIPS word alignment

// small bitmap caching...
#if   	1   //(MEMORY_MAP != MMAP_8MB) //[OBSOLETE][TEMP][TODO] BD_xxx
#define MVF_CACHE_SBITMAP_ENABLE     1
#define MVF_CACHE_SBITMAP_UNIT_SIZE  (32*32)
#define MVF_CACHE_SBITMAP_UNIT_COUNT 256
#define MVF_CACHE_SBITMAP_FONT_SIZE_MAX   30
#define MVR_CACHE_SBITMAP_REFRESH    1000000
#else //[OBSOLETE]
#define MVF_CACHE_SBITMAP_ENABLE     0
#endif

// outline caching...
#define MVF_CACHE_OUTLINE_ENABLE     0
#define MVF_CACHE_OUTLINE_UNIT_COUNT 4

#define MVF_HINTING_ENABLE 1

#define MVF_BITMAP_FORMAT_MASK(x) ((x)&0x0F)
#define MVF_BITMAP_FORMAT_I8    8 // 256-level grays and 4-bytes memory alignment
#define MVF_BITMAP_FORMAT_I4    4
#define MVF_BITMAP_FORMAT_I2    2
#define MVF_BITMAP_FORMAT_I1    1 // mono render not supported yet

//NOTE: MVF_BITMAP_ALIGN_DEFAULT for default space border (suitable for TTX BIN generating),
//      otherwise will remove all left space boder (MVF_BITMAP_ALIGN_LEFT is suitable for MVF_AEON generating for LG)
#define MVF_BITMAP_ALIGN_LEFT   0x100
#define MVF_BITMAP_ALIGN_TOP    0x200

/*#if defined( _MSC_VER )
#define MVF_BITMAP_FORMAT     MVF_BITMAP_FORMAT_I8 //always 256-level for windows...
#else
#define MVF_BITMAP_FORMAT     MVF_BITMAP_FORMAT_I2
#endif*/

#define MVF_BASE_EM_SIZE 1024
//#define MVF_BASE_EM_SCALE(x,size) (((x)*(size))>>10)
#define MVF_BASE_EM_ROUND(x,size) (((x)*(size)+(MVF_BASE_EM_SIZE/2))>>10)
#define MVF_BASE_EM_FLOOR(x,size) (((x)*(size)                     )>>10)
#define MVF_BASE_EM_CEIL(x,size)  (((x)*(size)+(MVF_BASE_EM_SIZE-1))>>10)

//#define MVF_BASE_BORDER 15000

#define MVF_FOR_TRUE_TYPE_ONLY //to disable cubic curves rendering

//////////////////////////////////////////////////////////////////////////
// Contant Definitions

#define MVF_FILE_FORMAT_VERSION 0x7052 //2007/05 ver.2: modify this if file format is changed

#define MVF_OUTLINE_X_USE24 0 //use 24bits in X
#define MVF_OUTLINE_X_USE16 1 //use 16bits in X
#define MVF_OUTLINE_Y_USE24 0 //use 24bits in Y
#define MVF_OUTLINE_Y_USE16 2 //use 16bits in Y

#define MVF_CURVE_TAG( flag )  ( flag & 3 )
#define MVF_CURVE_TAG_ON           1
#define MVF_CURVE_TAG_CONIC        0
#define MVF_CURVE_TAG_CUBIC        2

#define MVF_HINTING_X_TOUCH_TAG  8  /* reserved for the TrueType hinter */
#define MVF_HINTING_Y_TOUCH_TAG 16  /* reserved for the TrueType hinter */
#define MVF_HINTING_TOUCH_MASK (MVF_HINTING_Y_TOUCH_TAG|MVF_HINTING_X_TOUCH_TAG)

#define MVF_PIX_MAKE( x )      ( (MVF_Short)(x) << 6 )
#define MVF_PIX_FLOOR( x )     ( (x) & ~63 )
#define MVF_PIX_ROUND( x )     MVF_PIX_FLOOR( (x) + 32 )
#define MVF_PIX_CEIL( x )      MVF_PIX_FLOOR( (x) + 63 )

#define MVF_MIN( a, b )  ( (a) < (b) ? (a) : (b) )
#define MVF_MAX( a, b )  ( (a) > (b) ? (a) : (b) )
#define MVF_ABS( a )     ( (a) < 0 ? -(a) : (a) )


//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////
  typedef signed long  MVF_Pos;

  typedef struct  MVF_Vector_
  {
    MVF_Pos  x;
    MVF_Pos  y;

  } MVF_Vector;

  typedef struct  MVF_InternalOutline_
  {
    MVF_Short       n_contours;      /* number of contours in glyph        */
    MVF_Short       n_points;        /* number of points in the glyph      */

    MVF_Vector*     points;          /* the outline's points               */
    MVF_Byte*       tags;            /* the points flags                   */
    MVF_Short*      contours;        /* the contour end points             */
  } MVF_InternalOutline;

  typedef struct  MVF_Outline_
  {
    MVF_Short       n_contours;      /* number of contours in glyph        */
    MVF_Short       n_points;        /* number of points in the glyph      */

    MVF_Vector*     points;          /* the outline's points               */
    MVF_Byte*       tags;            /* the points flags                   */
    MVF_UShort*      contours;        /* the contour end points             */

	//MVF_Short           x_shift, y_shift; // x MVF_BASE_EM_SIZE
	//MVF_UShort       width, height;    // x MVF_BASE_EM_SIZE
	MVF_UShort             advance_x;   // x MVF_BASE_EM_SIZE
	MVF_UShort             metrics_height;   // x MVF_BASE_EM_SIZE
	MVF_Short           ascender, descender; // x MVF_BASE_EM_SIZE

#if MVF_HINTING_ENABLE
	MVF_UShort       hinting_n_sizes;
	MVF_Byte*       hinting_sizes;
	MVF_Byte*       hinting_adv_x;
	MVF_UShort      hinting_n_touch_points;
	MVF_Byte*       hinting_data;
#endif

  } MVF_Outline;

  typedef struct  MVF_Bitmap_
  {
    MVF_Byte*  buffer;
    MVF_UShort             rows;
    MVF_UShort             width;
    MVF_UShort             pitch;
	MVF_Short             advance_x;

	MVF_Short             x_shift;
	MVF_Short             y_shift;
	MVF_UShort             metrics_height;
	MVF_Short           ascender, descender;
  } MVF_Bitmap;

  typedef struct  MVF_InternalBitmap_
  {
    MVF_Byte*  buffer;
    MVF_UShort             rows;
    MVF_UShort             width;
    MVF_UShort             pitch;
	MVF_Short             advance_x;
  } MVF_InternalBitmap;

  typedef struct  MVF_Span_
  {
    MVF_Short           x;
    MVF_UShort  len;
    MVF_Byte   coverage;

  } MVF_Span;

  typedef struct  MVF_BBox_
  {
    MVF_Pos  xMin, yMin;
    MVF_Pos  xMax, yMax;

  } MVF_BBox;

////////////////////////////////////

  typedef struct MVF_GlyphRec_
  {
	MVF_UShort        unicode;
	MVF_Byte * outline_data;
	MVF_Byte * kerning_data;

  } MVF_Glyph;

  typedef struct  MVF_FaceRec_
  {
	MVF_Byte*  file_base;
    MVF_Byte*  file_limit;

	MVF_Short         num_glyphs;
	MVF_Glyph *       glyphs;

	MVF_Byte*   outline_buffer_start;
	MVF_Byte*   outline_buffer_pos;
	MVF_UShort  outline_buffer_max;

  } MVF_Face;

typedef struct MVF_HintPoint_
{
	MVF_Pos x, y, u, v, ox, oy;
	MVF_Byte touch_flags;
} MVF_HintPoint;

//typedef int MVF_Error;


/////////////////////////////////////////////////
// Extern functions

extern MVF_Error MVF_New_Face(
					  MVF_Byte *  file_base,
                      MVF_Byte *  file_limit,  //range: [file_base, file_limit)
					  MVF_Face *        aface); //[OUT]

extern void MVF_Done_Face(
				 MVF_Face * face);

extern MVF_Error MVF_Get_Kerning( MVF_Face *     face,
                  MVF_UShort     left_char,
                  MVF_UShort     right_char,
				  MVF_UShort     font_size,
                  MVF_Vector *    akerning ); //[OUT]

extern MVF_Error MVF_Load_Glyph( MVF_Face *   face,
                 MVF_UShort   unicode,
				 MVF_Outline * outline); //[OUT]

extern MVF_Error MVF_Load_Glyph_By_ID( MVF_Face *   face,
                 MVF_UShort   glyph_id,
				 MVF_Outline * outline); //[OUT]


/*extern void MVF_Done_Outline(
				 MVF_Outline * outline);*/

extern MVF_Error MVF_Render(
					MVF_Face * face,
			         MVF_Outline * outline,
					 MVF_UShort font_size,
					 MVF_UShort unicode,
					 MVF_Bool only_metrics,
					 MVF_UShort flags,
					 MVF_Bitmap * bitmap); //[OUT]

MVF_Error MVF_Render_From_Cache(MVF_Face * face,
					 MVF_UShort font_size,
					 MVF_UShort unicode,
					 MVF_Bitmap * bitmap); //[OUT]


extern MVF_Error MVF_Render_Private_Bitmap(
					MVF_Face * face,
			         MVF_Outline * outline,
					 MVF_UShort font_size,
					 MVF_UShort flags,
					 MVF_InternalBitmap * bitmap); //[IN,OUT]

/*extern MVF_Error _MVF_Render(
					 MVF_Outline * outline,
					 MVF_Bitmap * bitmap);*/

extern void MVF_Done_Render(void);


#ifdef __cplusplus
}
#endif


#endif /* __API_MVF_H__ */


/* END */
