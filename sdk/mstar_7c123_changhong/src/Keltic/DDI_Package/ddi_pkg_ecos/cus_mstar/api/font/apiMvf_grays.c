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
//
/// file apiMvf_grays.c
/// @brief MStar Vector Font Engine module.
/// @author MStar Semiconductor Inc.
///
///  Vector Font Engine render. Reference "smooth" render of FreeType2 library
///
/// Features:
/// - Providing rendering function to generate bitmaps from outlines
//
////////////////////////////////////////////////////////////////////////////////


  /*************************************************************************/
  /*                                                                       */
  /* This file can be compiled without the rest of the FreeType engine, by */
  /* defining the _STANDALONE_ macro when compiling it.  You also need to  */
  /* put the files `ftgrays.h' and `ftimage.h' into the current            */
  /* compilation directory.  Typically, you could do something like        */
  /*                                                                       */
  /* - copy `src/smooth/ftgrays.c' (this file) to your current directory   */
  /*                                                                       */
  /* - copy `include/freetype/ftimage.h' and `src/smooth/ftgrays.h' to the */
  /*   same directory                                                      */
  /*                                                                       */
  /* - compile `ftgrays' with the _STANDALONE_ macro defined, as in        */
  /*                                                                       */
  /*     cc -c -D_STANDALONE_ ftgrays.c                                    */
  /*                                                                       */
  /* The renderer can be initialized with a call to                        */
  /* `mvf_gray_raster.raster_new'; an anti-aliased bitmap can be generated  */
  /* with a call to `mvf_gray_raster.raster_render'.                        */
  /*                                                                       */
  /* See the comments and documentation in the file `ftimage.h' for more   */
  /* details on how the raster works.                                      */
  /*                                                                       */
  /*************************************************************************/

  /*************************************************************************/
  /*                                                                       */
  /* This is a new anti-aliasing scan-converter for FreeType 2.  The       */
  /* algorithm used here is _very_ different from the one in the standard  */
  /* `ftraster' module.  Actually, `ftgrays' computes the _exact_          */
  /* coverage of the outline on each pixel cell.                           */
  /*                                                                       */
  /* It is based on ideas that I initially found in Raph Levien's          */
  /* excellent LibArt graphics library (see http://www.levien.com/libart   */
  /* for more information, though the web pages do not tell anything       */
  /* about the renderer; you'll have to dive into the source code to       */
  /* understand how it works).                                             */
  /*                                                                       */
  /* Note, however, that this is a _very_ different implementation         */
  /* compared to Raph's.  Coverage information is stored in a very         */
  /* different way, and I don't use sorted vector paths.  Also, it doesn't */
  /* use floating point values.                                            */
  /*                                                                       */
  /* This renderer has the following advantages:                           */
  /*                                                                       */
  /* - It doesn't need an intermediate bitmap.  Instead, one can supply a  */
  /*   callback function that will be called by the renderer to draw gray  */
  /*   spans on any target surface.  You can thus do direct composition on */
  /*   any kind of bitmap, provided that you give the renderer the right   */
  /*   callback.                                                           */
  /*                                                                       */
  /* - A perfect anti-aliaser, i.e., it computes the _exact_ coverage on   */
  /*   each pixel cell.                                                    */
  /*                                                                       */
  /* - It performs a single pass on the outline (the `standard' FT2        */
  /*   renderer makes two passes).                                         */
  /*                                                                       */
  /* - It can easily be modified to render to _any_ number of gray levels  */
  /*   cheaply.                                                            */
  /*                                                                       */
  /* - For small (< 20) pixel sizes, it is faster than the standard        */
  /*   renderer.                                                           */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* The macro MVF_COMPONENT is used in trace mode.  It is an implicit      */
  /* parameter of the MVF_TRACE() and MVF_ERROR() macros, used to print/log  */
  /* messages during execution.                                            */
  /*                                                                       */
#undef  MVF_COMPONENT
#define MVF_COMPONENT  trace_smooth

#include "string.h"
#include "mvf_config.h"
#include "apiMvf_types.h"
#include "apiMvf.h"

#include <setjmp.h>
#include <limits.h>
#define MVF_UINT_MAX  UINT_MAX

#define mvf_setjmp   setjmp
#define mvf_longjmp  longjmp
#define mvf_jmp_buf  jmp_buf


#define ErrRaster_Invalid_Mode      -2
#define ErrRaster_Invalid_Outline   -1
#define ErrRaster_Invalid_Argument  -3
#define ErrRaster_Memory_Overflow   -4


  /* This macro is used to indicate that a function parameter is unused. */
  /* Its purpose is simply to reduce compiler warnings.  Note also that  */
  /* simply defining it as `(void)x' doesn't avoid warnings with certain */
  /* ANSI compilers (e.g. LCC).                                          */
#ifndef MVF_UNUSED
#define MVF_UNUSED( x )  (x) = (x)
#endif

  /* Disable the tracing mechanism for simplicity -- developers can      */
  /* activate it easily by redefining these two macros.                  */
#ifndef MVF_ERROR
#define MVF_ERROR( x )  do ; while ( 0 )     /* nothing */
#endif

#ifndef MVF_TRACE
#define MVF_TRACE( x )  do ; while ( 0 )     /* nothing */
#endif

/*define debug message*/
#ifdef MVF_DEBUG

#define MVF_BASE_DEBUG(x)   x

#else //MVF_DEBUG

#define MVF_BASE_DEBUG(x) //x

#endif //MVF_DEBUG



  /* define this to dump debugging information */
#define xxxDEBUG_GRAYS

  /* as usual, for the speed hungry :-) */

#ifndef MVF_STATIC_RASTER


#define RAS_ARG   PWorker  worker
#define RAS_ARG_  PWorker  worker,

#define RAS_VAR   worker
#define RAS_VAR_  worker,

#define ras       (*worker)


#else /* MVF_STATIC_RASTER */


#define RAS_ARG   /* empty */
#define RAS_ARG_  /* empty */
#define RAS_VAR   /* empty */
#define RAS_VAR_  /* empty */

  static TWorker  ras;


#endif /* MVF_STATIC_RASTER */


  /* must be at least 6 bits! */
#define PIXEL_BITS  6

#define ONE_PIXEL       ( 1L << PIXEL_BITS )
#define PIXEL_MASK      ( -1L << PIXEL_BITS )
#define TRUNC( x )      ( (TCoord)((x) >> PIXEL_BITS) )
#define SUBPIXELS( x )  ( (TPos)(x) << PIXEL_BITS )
#define FLOOR( x )      ( (x) & -ONE_PIXEL )
#define CEILING( x )    ( ( (x) + ONE_PIXEL - 1 ) & -ONE_PIXEL )
#define ROUND( x )      ( ( (x) + ONE_PIXEL / 2 ) & -ONE_PIXEL )

#if PIXEL_BITS >= 6
#define UPSCALE( x )    ( (x) << ( PIXEL_BITS - 6 ) )
#define DOWNSCALE( x )  ( (x) >> ( PIXEL_BITS - 6 ) )
#else
#define UPSCALE( x )    ( (x) >> ( 6 - PIXEL_BITS ) )
#define DOWNSCALE( x )  ( (x) << ( 6 - PIXEL_BITS ) )
#endif

extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;


  /*************************************************************************/
  /*                                                                       */
  /*   TYPE DEFINITIONS                                                    */
  /*                                                                       */

  /* don't change the following types to MVF_Int or MVF_Pos, since we might */
  /* need to define them to "float" or "double" when experimenting with   */
  /* new algorithms                                                       */

  typedef int   TCoord;   /* integer scanline/pixel coordinate */
  typedef long  TPos;     /* sub-pixel coordinate              */

  /* determine the type used to store cell areas.  This normally takes at */
  /* least PIXEL_BITS*2 + 1 bits.  On 16-bit systems, we need to use      */
  /* `long' instead of `int', otherwise bad things happen                 */

#if PIXEL_BITS <= 7

  typedef int  TArea;

#else /* PIXEL_BITS >= 8 */

  /* approximately determine the size of integers using an ANSI-C header */
#if MVF_UINT_MAX == 0xFFFFU
  typedef long  TArea;
#else
  typedef int   TArea;
#endif

#endif /* PIXEL_BITS >= 8 */

  /* maximal number of gray spans in a call to the span callback */
#define MVF_MAX_GRAY_SPANS  32

  typedef void  (*MVF_Raster_Span_Func)(
                    int             y,
                    int             count,
                    const MVF_Span*  spans,
                    MVF_InternalBitmap* map); //PWorker         worker );

  typedef struct TCell_*  PCell;

  typedef struct  TCell_
  {
    int    x;
    int    cover;
    TArea  area;
    PCell  next;

  } TCell;


  typedef struct  TWorker_
  {
    TCoord  ex, ey;
    TPos    min_ex, max_ex;
    TPos    min_ey, max_ey;
    TPos    count_ex, count_ey;

    TArea   area;
    int     cover;
    int     invalid;

    PCell   cells;
    int     max_cells;
    int     num_cells;

    TCoord  cx, cy;
    TPos    x,  y;

    TPos    last_ey;

    MVF_Vector   bez_stack[32 * 3 + 1];
    int         lev_stack[32];

    MVF_InternalOutline  outline;
    MVF_InternalBitmap   target;
    MVF_BBox     clip_box;

    MVF_Span     gray_spans[MVF_MAX_GRAY_SPANS];
    int         num_gray_spans;

    MVF_Raster_Span_Func  render_span;
    //void*                render_span_data;
    int                  span_y;

    int  band_size;
    int  band_shoot;
    int  conic_level;
#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
    int  cubic_level;
#endif

    mvf_jmp_buf  jump_buffer;

    void*       buffer;
    long        buffer_size;

    PCell*     ycells;
    int        ycount;

  } TWorker, *PWorker;


  typedef struct TRaster_
  {
    void*    buffer;
    long     buffer_size;
    int      band_size;
    void*    memory;
    PWorker  worker;

  } TRaster, *PRaster;

typedef PRaster MVF_Raster;


  /*************************************************************************/
  /*                                                                       */
  /* Initialize the cells table.                                           */
  /*                                                                       */
  static void
  gray_init_cells( RAS_ARG_ void*  buffer,
                   long            byte_size )
  {
    ras.buffer      = buffer;
    ras.buffer_size = byte_size;

    ras.ycells      = (PCell*) buffer;
    ras.cells       = NULL;
    ras.max_cells   = 0;
    ras.num_cells   = 0;
    ras.area        = 0;
    ras.cover       = 0;
    ras.invalid     = 1;
  }


  /*************************************************************************/
  /*                                                                       */
  /* Compute the outline bounding box.                                     */
  /*                                                                       */
  static void
  gray_compute_cbox( RAS_ARG )
  {
    MVF_InternalOutline*  outline = &ras.outline;
    MVF_Vector*   vec     = outline->points;
    MVF_Vector*   limit   = vec + outline->n_points;


    if ( outline->n_points <= 0 )
    {
      ras.min_ex = ras.max_ex = 0;
      ras.min_ey = ras.max_ey = 0;
      return;
    }

    ras.min_ex = ras.max_ex = vec->x;
    ras.min_ey = ras.max_ey = vec->y;

    vec++;

    for ( ; vec < limit; vec++ )
    {
      TPos  x = vec->x;
      TPos  y = vec->y;


      if ( x < ras.min_ex ) ras.min_ex = x;
      if ( x > ras.max_ex ) ras.max_ex = x;
      if ( y < ras.min_ey ) ras.min_ey = y;
      if ( y > ras.max_ey ) ras.max_ey = y;
    }

    /* truncate the bounding box to integer pixels */
    ras.min_ex = ras.min_ex >> 6;
    ras.min_ey = ras.min_ey >> 6;
    ras.max_ex = ( ras.max_ex + 63 ) >> 6;
    ras.max_ey = ( ras.max_ey + 63 ) >> 6;
  }


  /*************************************************************************/
  /*                                                                       */
  /* Record the current cell in the table.                                 */
  /*                                                                       */
  static PCell
  gray_find_cell( RAS_ARG )
  {
    PCell  *pcell, cell;
    int     x = ras.ex;


    pcell = &ras.ycells[ras.ey];
    for (;;)
    {
      cell = *pcell;
      if ( cell == NULL || cell->x > x )
        break;

      if ( cell->x == x )
        goto Exit;

      pcell = &cell->next;
    }

    if ( ras.num_cells >= ras.max_cells )
      mvf_longjmp( ras.jump_buffer, 1 );

    cell        = ras.cells + ras.num_cells++;
    cell->x     = x;
    cell->area  = 0;
    cell->cover = 0;

    cell->next  = *pcell;
    *pcell      = cell;

  Exit:
    return cell;
  }


  static void
  gray_record_cell( RAS_ARG )
  {
    if ( !ras.invalid && ( ras.area | ras.cover ) )
    {
      PCell  cell = gray_find_cell( RAS_VAR );


      cell->area  += ras.area;
      cell->cover += ras.cover;
    }
  }


  /*************************************************************************/
  /*                                                                       */
  /* Set the current cell to a new position.                               */
  /*                                                                       */
  static void
  gray_set_cell( RAS_ARG_ TCoord  ex,
                          TCoord  ey )
  {
    /* Move the cell pointer to a new position.  We set the `invalid'      */
    /* flag to indicate that the cell isn't part of those we're interested */
    /* in during the render phase.  This means that:                       */
    /*                                                                     */
    /* . the new vertical position must be within min_ey..max_ey-1.        */
    /* . the new horizontal position must be strictly less than max_ex     */
    /*                                                                     */
    /* Note that if a cell is to the left of the clipping region, it is    */
    /* actually set to the (min_ex-1) horizontal position.                 */

    /* All cells that are on the left of the clipping region go to the */
    /* min_ex - 1 horizontal position.                                 */
    ey -= ras.min_ey;
    ex -= ras.min_ex;
    if ( ex < 0 )
      ex = -1;

    /* are we moving to a different cell ? */
    if ( ex != ras.ex || ey != ras.ey )
    {
      /* record the current one if it is valid */
      if ( !ras.invalid )
        gray_record_cell( RAS_VAR );

      ras.area  = 0;
      ras.cover = 0;
    }

    ras.ex      = ex;
    ras.ey      = ey;
    ras.invalid = ( (unsigned)ey >= (unsigned)ras.count_ey ||
                              ex >= ras.count_ex           );
  }


  /*************************************************************************/
  /*                                                                       */
  /* Start a new contour at a given cell.                                  */
  /*                                                                       */
  static void
  gray_start_cell( RAS_ARG_ TCoord  ex,
                            TCoord  ey )
  {
    if ( ex < ras.min_ex )
      ex = (TCoord)( ras.min_ex - 1 );

    ras.area    = 0;
    ras.cover   = 0;
    ras.ex      = ex - ras.min_ex;
    ras.ey      = ey - ras.min_ey;
    ras.last_ey = SUBPIXELS( ey );
    ras.invalid = 0;

    gray_set_cell( RAS_VAR_ ex, ey );
  }


  /*************************************************************************/
  /*                                                                       */
  /* Render a scanline as one or more cells.                               */
  /*                                                                       */
  static void
  gray_render_scanline( RAS_ARG_ TCoord  ey,
                                 TPos    x1,
                                 TCoord  y1,
                                 TPos    x2,
                                 TCoord  y2 )
  {
    TCoord  ex1, ex2, fx1, fx2, delta;
    long    p, first, dx;
    int     incr, lift, mod, rem;


    dx = x2 - x1;

    ex1 = TRUNC( x1 );
    ex2 = TRUNC( x2 );
    fx1 = (TCoord)( x1 - SUBPIXELS( ex1 ) );
    fx2 = (TCoord)( x2 - SUBPIXELS( ex2 ) );

    /* trivial case.  Happens often */
    if ( y1 == y2 )
    {
      gray_set_cell( RAS_VAR_ ex2, ey );
      return;
    }

    /* everything is located in a single cell.  That is easy! */
    /*                                                        */
    if ( ex1 == ex2 )
    {
      delta      = y2 - y1;
      ras.area  += (TArea)( fx1 + fx2 ) * delta;
      ras.cover += delta;
      return;
    }

    /* ok, we'll have to render a run of adjacent cells on the same */
    /* scanline...                                                  */
    /*                                                              */
    p     = ( ONE_PIXEL - fx1 ) * ( y2 - y1 );
    first = ONE_PIXEL;
    incr  = 1;

    if ( dx < 0 )
    {
      p     = fx1 * ( y2 - y1 );
      first = 0;
      incr  = -1;
      dx    = -dx;
    }

    delta = (TCoord)( p / dx );
    mod   = (TCoord)( p % dx );
    if ( mod < 0 )
    {
      delta--;
      mod += (TCoord)dx;
    }

    ras.area  += (TArea)( fx1 + first ) * delta;
    ras.cover += delta;

    ex1 += incr;
    gray_set_cell( RAS_VAR_ ex1, ey );
    y1  += delta;

    if ( ex1 != ex2 )
    {
      p    = ONE_PIXEL * ( y2 - y1 + delta );
      lift = (TCoord)( p / dx );
      rem  = (TCoord)( p % dx );
      if ( rem < 0 )
      {
        lift--;
        rem += (TCoord)dx;
      }

      mod -= (int)dx;

      while ( ex1 != ex2 )
      {
        delta = lift;
        mod  += rem;
        if ( mod >= 0 )
        {
          mod -= (TCoord)dx;
          delta++;
        }

        ras.area  += (TArea)ONE_PIXEL * delta;
        ras.cover += delta;
        y1        += delta;
        ex1       += incr;
        gray_set_cell( RAS_VAR_ ex1, ey );
      }
    }

    delta      = y2 - y1;
    ras.area  += (TArea)( fx2 + ONE_PIXEL - first ) * delta;
    ras.cover += delta;
  }


  /*************************************************************************/
  /*                                                                       */
  /* Render a given line as a series of scanlines.                         */
  /*                                                                       */
  static void
  gray_render_line( RAS_ARG_ TPos  to_x,
                             TPos  to_y )
  {
    TCoord  ey1, ey2, fy1, fy2;
    TPos    dx, dy, x, x2;
    long    p, first;
    int     delta, rem, mod, lift, incr;


    ey1 = TRUNC( ras.last_ey );
    ey2 = TRUNC( to_y );     /* if (ey2 >= ras.max_ey) ey2 = ras.max_ey-1; */
    fy1 = (TCoord)( ras.y - ras.last_ey );
    fy2 = (TCoord)( to_y - SUBPIXELS( ey2 ) );

    dx = to_x - ras.x;
    dy = to_y - ras.y;

    /* XXX: we should do something about the trivial case where dx == 0, */
    /*      as it happens very often!                                    */

    /* perform vertical clipping */
    {
      TCoord  min, max;


      min = ey1;
      max = ey2;
      if ( ey1 > ey2 )
      {
        min = ey2;
        max = ey1;
      }
      if ( min >= ras.max_ey || max < ras.min_ey )
        goto End;
    }

    /* everything is on a single scanline */
    if ( ey1 == ey2 )
    {
      gray_render_scanline( RAS_VAR_ ey1, ras.x, fy1, to_x, fy2 );
      goto End;
    }

    /* vertical line - avoid calling gray_render_scanline */
    incr = 1;

    if ( dx == 0 )
    {
      TCoord  ex     = TRUNC( ras.x );
      TCoord  two_fx = (TCoord)( ( ras.x - SUBPIXELS( ex ) ) << 1 );
      TPos    area;


      first = ONE_PIXEL;
      if ( dy < 0 )
      {
        first = 0;
        incr  = -1;
      }

      delta      = (int)( first - fy1 );
      ras.area  += (TArea)two_fx * delta;
      ras.cover += delta;
      ey1       += incr;

      gray_set_cell( &ras, ex, ey1 );

      delta = (int)( first + first - ONE_PIXEL );
      area  = (TArea)two_fx * delta;
      while ( ey1 != ey2 )
      {
        ras.area  += area;
        ras.cover += delta;
        ey1       += incr;

        gray_set_cell( &ras, ex, ey1 );
      }

      delta      = (int)( fy2 - ONE_PIXEL + first );
      ras.area  += (TArea)two_fx * delta;
      ras.cover += delta;

      goto End;
    }

    /* ok, we have to render several scanlines */
    p     = ( ONE_PIXEL - fy1 ) * dx;
    first = ONE_PIXEL;
    incr  = 1;

    if ( dy < 0 )
    {
      p     = fy1 * dx;
      first = 0;
      incr  = -1;
      dy    = -dy;
    }

    delta = (int)( p / dy );
    mod   = (int)( p % dy );
    if ( mod < 0 )
    {
      delta--;
      mod += (TCoord)dy;
    }

    x = ras.x + delta;
    gray_render_scanline( RAS_VAR_ ey1, ras.x, fy1, x, (TCoord)first );

    ey1 += incr;
    gray_set_cell( RAS_VAR_ TRUNC( x ), ey1 );

    if ( ey1 != ey2 )
    {
      p     = ONE_PIXEL * dx;
      lift  = (int)( p / dy );
      rem   = (int)( p % dy );
      if ( rem < 0 )
      {
        lift--;
        rem += (int)dy;
      }
      mod -= (int)dy;

      while ( ey1 != ey2 )
      {
        delta = lift;
        mod  += rem;
        if ( mod >= 0 )
        {
          mod -= (int)dy;
          delta++;
        }

        x2 = x + delta;
        gray_render_scanline( RAS_VAR_ ey1, x,
                                       (TCoord)( ONE_PIXEL - first ), x2,
                                       (TCoord)first );
        x = x2;

        ey1 += incr;
        gray_set_cell( RAS_VAR_ TRUNC( x ), ey1 );
      }
    }

    gray_render_scanline( RAS_VAR_ ey1, x,
                                   (TCoord)( ONE_PIXEL - first ), to_x,
                                   fy2 );

  End:
    ras.x       = to_x;
    ras.y       = to_y;
    ras.last_ey = SUBPIXELS( ey2 );
  }


  static void
  gray_split_conic( MVF_Vector*  base )
  {
    TPos  a, b;


    base[4].x = base[2].x;
    b = base[1].x;
    a = base[3].x = ( base[2].x + b ) / 2;
    b = base[1].x = ( base[0].x + b ) / 2;
    base[2].x = ( a + b ) / 2;

    base[4].y = base[2].y;
    b = base[1].y;
    a = base[3].y = ( base[2].y + b ) / 2;
    b = base[1].y = ( base[0].y + b ) / 2;
    base[2].y = ( a + b ) / 2;
  }


  static void
  gray_render_conic( RAS_ARG_ const MVF_Vector*  control,
                              const MVF_Vector*  to )
  {
    TPos        dx, dy;
    int         top, level;
    int*        levels;
    MVF_Vector*  arc;


    dx = DOWNSCALE( ras.x ) + to->x - ( control->x << 1 );
    if ( dx < 0 )
      dx = -dx;
    dy = DOWNSCALE( ras.y ) + to->y - ( control->y << 1 );
    if ( dy < 0 )
      dy = -dy;
    if ( dx < dy )
      dx = dy;

    level = 1;
    dx = dx / ras.conic_level;
    while ( dx > 0 )
    {
      dx >>= 2;
      level++;
    }

    /* a shortcut to speed things up */
    if ( level <= 1 )
    {
      /* we compute the mid-point directly in order to avoid */
      /* calling gray_split_conic()                          */
      TPos  to_x, to_y, mid_x, mid_y;


      to_x  = UPSCALE( to->x );
      to_y  = UPSCALE( to->y );
      mid_x = ( ras.x + to_x + 2 * UPSCALE( control->x ) ) / 4;
      mid_y = ( ras.y + to_y + 2 * UPSCALE( control->y ) ) / 4;

      gray_render_line( RAS_VAR_ mid_x, mid_y );
      gray_render_line( RAS_VAR_ to_x, to_y );

      return;
    }

    arc       = ras.bez_stack;
    levels    = ras.lev_stack;
    top       = 0;
    levels[0] = level;

    arc[0].x = UPSCALE( to->x );
    arc[0].y = UPSCALE( to->y );
    arc[1].x = UPSCALE( control->x );
    arc[1].y = UPSCALE( control->y );
    arc[2].x = ras.x;
    arc[2].y = ras.y;

    while ( top >= 0 )
    {
      level = levels[top];
      if ( level > 1 )
      {
        /* check that the arc crosses the current band */
        TPos  min, max, y;


        min = max = arc[0].y;

        y = arc[1].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;

        y = arc[2].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;

        if ( TRUNC( min ) >= ras.max_ey || TRUNC( max ) < ras.min_ey )
          goto Draw;

        gray_split_conic( arc );
        arc += 2;
        top++;
        levels[top] = levels[top - 1] = level - 1;
        continue;
      }

    Draw:
      {
        TPos  to_x, to_y, mid_x, mid_y;


        to_x  = arc[0].x;
        to_y  = arc[0].y;
        mid_x = ( ras.x + to_x + 2 * arc[1].x ) / 4;
        mid_y = ( ras.y + to_y + 2 * arc[1].y ) / 4;

        gray_render_line( RAS_VAR_ mid_x, mid_y );
        gray_render_line( RAS_VAR_ to_x, to_y );

        top--;
        arc -= 2;
      }
    }

    return;
  }

#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
  static void
  gray_split_cubic( MVF_Vector*  base )
  {
    TPos  a, b, c, d;


    base[6].x = base[3].x;
    c = base[1].x;
    d = base[2].x;
    base[1].x = a = ( base[0].x + c ) / 2;
    base[5].x = b = ( base[3].x + d ) / 2;
    c = ( c + d ) / 2;
    base[2].x = a = ( a + c ) / 2;
    base[4].x = b = ( b + c ) / 2;
    base[3].x = ( a + b ) / 2;

    base[6].y = base[3].y;
    c = base[1].y;
    d = base[2].y;
    base[1].y = a = ( base[0].y + c ) / 2;
    base[5].y = b = ( base[3].y + d ) / 2;
    c = ( c + d ) / 2;
    base[2].y = a = ( a + c ) / 2;
    base[4].y = b = ( b + c ) / 2;
    base[3].y = ( a + b ) / 2;
  }


  static void
  gray_render_cubic( RAS_ARG_ const MVF_Vector*  control1,
                              const MVF_Vector*  control2,
                              const MVF_Vector*  to )
  {
    TPos        dx, dy, da, db;
    int         top, level;
    int*        levels;
    MVF_Vector*  arc;


    dx = DOWNSCALE( ras.x ) + to->x - ( control1->x << 1 );
    if ( dx < 0 )
      dx = -dx;
    dy = DOWNSCALE( ras.y ) + to->y - ( control1->y << 1 );
    if ( dy < 0 )
      dy = -dy;
    if ( dx < dy )
      dx = dy;
    da = dx;

    dx = DOWNSCALE( ras.x ) + to->x - 3 * ( control1->x + control2->x );
    if ( dx < 0 )
      dx = -dx;
    dy = DOWNSCALE( ras.y ) + to->y - 3 * ( control1->x + control2->y );
    if ( dy < 0 )
      dy = -dy;
    if ( dx < dy )
      dx = dy;
    db = dx;

    level = 1;
    da    = da / ras.cubic_level;
    db    = db / ras.conic_level;
    while ( da > 0 || db > 0 )
    {
      da >>= 2;
      db >>= 3;
      level++;
    }

    if ( level <= 1 )
    {
      TPos   to_x, to_y, mid_x, mid_y;


      to_x  = UPSCALE( to->x );
      to_y  = UPSCALE( to->y );
      mid_x = ( ras.x + to_x +
                3 * UPSCALE( control1->x + control2->x ) ) / 8;
      mid_y = ( ras.y + to_y +
                3 * UPSCALE( control1->y + control2->y ) ) / 8;

      gray_render_line( RAS_VAR_ mid_x, mid_y );
      gray_render_line( RAS_VAR_ to_x, to_y );
      return;
    }

    arc      = ras.bez_stack;
    arc[0].x = UPSCALE( to->x );
    arc[0].y = UPSCALE( to->y );
    arc[1].x = UPSCALE( control2->x );
    arc[1].y = UPSCALE( control2->y );
    arc[2].x = UPSCALE( control1->x );
    arc[2].y = UPSCALE( control1->y );
    arc[3].x = ras.x;
    arc[3].y = ras.y;

    levels    = ras.lev_stack;
    top       = 0;
    levels[0] = level;

    while ( top >= 0 )
    {
      level = levels[top];
      if ( level > 1 )
      {
        /* check that the arc crosses the current band */
        TPos  min, max, y;


        min = max = arc[0].y;
        y = arc[1].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        y = arc[2].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        y = arc[3].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        if ( TRUNC( min ) >= ras.max_ey || TRUNC( max ) < 0 )
          goto Draw;
        gray_split_cubic( arc );
        arc += 3;
        top ++;
        levels[top] = levels[top - 1] = level - 1;
        continue;
      }

    Draw:
      {
        TPos  to_x, to_y, mid_x, mid_y;


        to_x  = arc[0].x;
        to_y  = arc[0].y;
        mid_x = ( ras.x + to_x + 3 * ( arc[1].x + arc[2].x ) ) / 8;
        mid_y = ( ras.y + to_y + 3 * ( arc[1].y + arc[2].y ) ) / 8;

        gray_render_line( RAS_VAR_ mid_x, mid_y );
        gray_render_line( RAS_VAR_ to_x, to_y );
        top --;
        arc -= 3;
      }
    }

    return;
  }
#endif


  static int
  gray_move_to( const MVF_Vector*  to,
                PWorker           worker )
  {
    TPos  x, y;


    /* record current cell, if any */
    gray_record_cell( worker );

    /* start to a new position */
    x = UPSCALE( to->x );
    y = UPSCALE( to->y );

    gray_start_cell( worker, TRUNC( x ), TRUNC( y ) );

    worker->x = x;
    worker->y = y;
    return 0;
  }


  static int
  gray_line_to( const MVF_Vector*  to,
                PWorker           worker )
  {
    gray_render_line( worker, UPSCALE( to->x ), UPSCALE( to->y ) );
    return 0;
  }


  static int
  gray_conic_to( const MVF_Vector*  control,
                 const MVF_Vector*  to,
                 PWorker           worker )
  {
    gray_render_conic( worker, control, to );
    return 0;
  }

#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
  static int
  gray_cubic_to( const MVF_Vector*  control1,
                 const MVF_Vector*  control2,
                 const MVF_Vector*  to,
                 PWorker           worker )
  {
    gray_render_cubic( worker, control1, control2, to );
    return 0;
  }
#endif

  static void
  gray_render_span_I8( int             y,
                    int             count,
                    const MVF_Span*  spans,
                    MVF_InternalBitmap* map) //PWorker         worker )
  {
    unsigned char*  p;
    //MVF_InternalBitmap*      map = &worker->target;


    /* first of all, compute the scanline offset */
    p = (unsigned char*)map->buffer - y * map->pitch;
    //if ( map->pitch >= 0 )
      p += ( map->rows - 1 ) * map->pitch;

    for ( ; count > 0; count--, spans++ )
    {
      unsigned char  coverage = spans->coverage;

      if ( coverage )
      {
        /* For small-spans it is faster to do it by ourselves than
         * calling `memset'.  This is mainly due to the cost of the
         * function call.
         */
        if ( spans->len >= 8 )
          MVF_MEM_SET( p + spans->x, (unsigned char)coverage, spans->len );
        else
        {
          unsigned char*  q = p + spans->x;


          switch ( spans->len )
          {
          case 7: *q++ = (unsigned char)coverage;
          case 6: *q++ = (unsigned char)coverage;
          case 5: *q++ = (unsigned char)coverage;
          case 4: *q++ = (unsigned char)coverage;
          case 3: *q++ = (unsigned char)coverage;
          case 2: *q++ = (unsigned char)coverage;
          case 1: *q   = (unsigned char)coverage;
          default:
            ;
          }
        }
      }
    }
  }


  static void
  gray_render_span_I4( int             y,
                    int             count,
                    const MVF_Span*  spans,
                    MVF_InternalBitmap* map) //PWorker         worker )
  {
    unsigned char*  p;
    //MVF_InternalBitmap*      map = &worker->target;


    /* first of all, compute the scanline offset */
    p = (unsigned char*)map->buffer - y * map->pitch;
    //if ( map->pitch >= 0 )
      p += ( map->rows - 1 ) * map->pitch;

    for ( ; count > 0; count--, spans++ )
    {
      unsigned char  coverage0 = spans->coverage >> 4;
      unsigned short len = spans->len;

      if ( coverage0 ) //&& len )
      {

        unsigned char  coverage4 = coverage0 << 4;
        unsigned char  coverage8 = coverage4|coverage0;
        unsigned char*  q = p + spans->x/2;

        if ( (spans->x%2) != 0 )
        {
            *q |= coverage0;
            q++;
            len--;
        }

        /*if ( len >= 16 )
        {
            MVF_MEM_SET(q, coverage8, len/2);
            q += len/2;
            len %= 2;
            if (len) { *q &= 0x0F; *q |= coverage4; }
            continue;
        }*/


        if ( (len%2) != 0 )
        {
            switch(len)
            {
                case 15: *q++ = coverage8;
                case 13: *q++ = coverage8;
                case 11: *q++ = coverage8;
                case 9:  *q++ = coverage8;
                case 7:  *q++ = coverage8;
                case 5:  *q++ = coverage8;
                case 3:  *q++ = coverage8;
                case 1: *q &= 0x0F; *q |= coverage4;
                    break;
                default:
                    MVF_MEM_SET(q, coverage8, len/2);
                    q += len/2;
                    *q &= 0x0F; *q |= coverage4;
            }
        }
        else
        {
            switch(len)
            {
                case 14: *q++ = coverage8;
                case 12: *q++ = coverage8;
                case 10: *q++ = coverage8;
                case 8:  *q++ = coverage8;
                case 6:  *q++ = coverage8;
                case 4:  *q++ = coverage8;
                case 2:  *q = coverage8;
                case 0:
                    break;
                default:
                    MVF_MEM_SET(q, coverage8, len/2);

            }
        }

        /*while( len )
        {
            *q &= 0x0F;  //clean orig???
            *q |= coverage4;
            len--;
            if (!len) break;

            *q &= 0xF0;  //clean orig???
            *q |= coverage0;
            q++;
            len--;
        }*/

      }
    }
  }

  static void
  gray_render_span_I2( int             y,
                    int             count,
                    const MVF_Span*  spans,
                    MVF_InternalBitmap* map) //PWorker         worker )
  {
    unsigned char*  p;
    //MVF_InternalBitmap*      map = &worker->target;


    /* first of all, compute the scanline offset */
    p = (unsigned char*)map->buffer - y * map->pitch;
    //if ( map->pitch >= 0 )
      p += ( map->rows - 1 ) * map->pitch;

    for ( ; count > 0; count--, spans++ )
    {

      unsigned char  coverage0 = spans->coverage >> 6;
      unsigned short len = spans->len;

      if ( coverage0 ) //&& len )
      {

        //version 2
#define LEFTMASK  "\x00\xC0\xF0\xFC"
#define RIGHTMASK "\xFF\x3F\x0F\x03"
#define R2MASK    "\x3F\x0F\x03\x00"
#define L2MASK    "\xC0\xF0\xFC\xFF"

        unsigned int x = spans->x, xi;
        unsigned int xf=x/4, xt=(x+len-1)/4;
        unsigned int xmf=x&3, xmt=(x+len-1)&3;
        unsigned char coverage55 = coverage0 * 0x55;
        unsigned char mask;

        if (xf==xt)
        {
            // special case: both pixels in same byte

            mask = LEFTMASK[xmf]|R2MASK[xmt];
            p[xf]=(p[xf]&mask) | ((mask^0xff)&coverage55);
        }
        else
        {
            mask = LEFTMASK[xmf];
            p[xf] = (p[xf]&mask) | ((mask^0xff)&coverage55);
            for (xi=xf+1; xi<xt; xi++) p[xi] = coverage55;
            mask = R2MASK[xmt];
            p[xt] = (p[xt]&mask) | ((mask^0xff)&coverage55);
        }


        /* version1:
        unsigned char  coverage2 = coverage0 << 2;
        unsigned char  coverage4 = coverage0 << 4;
        unsigned char  coverage6 = coverage0 << 6;
        unsigned char*  q = p + spans->x/4;

        switch ( (spans->x%4) )
        {
            LOOP_START:
            case 0:
                *q &= 0x3F;  //clean orig???
                *q |= coverage6;
                len--;
                if (!len) break;

            case 1:
                *q &= 0xCF;  //clean orig???
                *q |= coverage4;
                len--;
                if (!len) break;

            case 2:
                *q &= 0xF3;  //clean orig???
                *q |= coverage2;
                len--;
                if (!len) break;

            case 3:
                *q &= 0xFC;  //clean orig???
                *q |= coverage0;
                q++;
                len--;
                if (!len) break;
                goto LOOP_START;
        }
        */



      }

    }
  }

  static void
  gray_render_span_I1( int             y,
                    int             count,
                    const MVF_Span*  spans,
                    MVF_InternalBitmap* map) //PWorker         worker )
  {
    unsigned char*  p;
    //MVF_InternalBitmap*      map = &worker->target;


    /* first of all, compute the scanline offset */
    p = (unsigned char*)map->buffer - y * map->pitch;
    //if ( map->pitch >= 0 )
      p += ( map->rows - 1 ) * map->pitch;

    for ( ; count > 0; count--, spans++ )
    {

      unsigned char  coverage0 = spans->coverage >> 7;
      unsigned short len = spans->len;

      if ( coverage0 ) //&& len )
      {

        //version 2
#undef  LEFTMASK
#undef  RIGHTMASK
#undef  R2MASK
#undef  L2MASK
#define LEFTMASK  "\x00\x80\xC0\xE0\xF0\xF8\xFC\xFE"
#define RIGHTMASK "\xFF\x7F\x3F\x1F\x0F\x07\x03\x01"
#define R2MASK    "\x7F\x3F\x1F\x0F\x07\x03\x01\x00"
#define L2MASK    "\x80\xC0\xE0\xF0\xF8\xFC\xFE\xFF"

        unsigned int x = spans->x, xi;
        unsigned int xf=x/8, xt=(x+len-1)/8;
        unsigned int xmf=x&7, xmt=(x+len-1)&7;
        //unsigned char coverageFF = 0xFF;
        unsigned char mask;

        if (xf==xt)
        {
            // special case: both pixels in same byte

            mask = LEFTMASK[xmf]|R2MASK[xmt];
            p[xf]=(p[xf]&mask) | ((mask^0xff));
        }
        else
        {
            mask = LEFTMASK[xmf];
            p[xf] = (p[xf]&mask) | ((mask^0xff));
            for (xi=xf+1; xi<xt; xi++) p[xi] = 0xFF;
            mask = R2MASK[xmt];
            p[xt] = (p[xt]&mask) | ((mask^0xff));
        }


      }

    }
  }

  static void
  gray_hline( RAS_ARG_ TCoord  x,
                       TCoord  y,
                       TPos    area,
                       int     acount )
  {
    MVF_Span*  span;
    int       count;
    int       coverage;


    /* compute the coverage line's coverage, depending on the    */
    /* outline fill rule                                         */
    /*                                                           */
    /* the coverage percentage is area/(PIXEL_BITS*PIXEL_BITS*2) */
    /*                                                           */
    coverage = (int)( area >> ( PIXEL_BITS * 2 + 1 - 8 ) );
                                                    /* use range 0..256 */
    if ( coverage < 0 )
      coverage = -coverage;

    /*if ( ras.outline.flags & MVF_OUTLINE_EVEN_ODD_FILL )
    {
      coverage &= 511;

      if ( coverage > 256 )
        coverage = 512 - coverage;
      else if ( coverage == 256 )
        coverage = 255;
    }
    else*/
    {
      /* normal non-zero winding rule */
      if ( coverage >= 256 )
        coverage = 255;
    }

    y += (TCoord)ras.min_ey;
    x += (TCoord)ras.min_ex;

    if ( acount )
    {
      /* see whether we can add this span to the current list */
      count = ras.num_gray_spans;
      span  = ras.gray_spans + count - 1;
      if ( count > 0                          &&
           ras.span_y == y                    &&
           (int)span->x + span->len == (int)x &&
           span->coverage == coverage         )
      {
        span->len = (unsigned short)( span->len + acount );
        return;
      }

      if ( ras.span_y != y || count >= MVF_MAX_GRAY_SPANS )
      {
        if ( count > 0 )
            ras.render_span( ras.span_y, count, ras.gray_spans, &ras.target );

#ifdef DEBUG_GRAYS

        if ( ras.span_y >= 0 )
        {
          int  n;


          fprintf( stderr, "y=%3d ", ras.span_y );
          span = ras.gray_spans;
          for ( n = 0; n < count; n++, span++ )
            fprintf( stderr, "[%d..%d]:%02x ",
                     span->x, span->x + span->len - 1, span->coverage );
          fprintf( stderr, "\n" );
        }

#endif /* DEBUG_GRAYS */

        ras.num_gray_spans = 0;
        ras.span_y         = y;

        count = 0;
        span  = ras.gray_spans;
      }
      else
        span++;

      /* add a gray span to the current list */
      span->x        = (short)x;
      span->len      = (unsigned short)acount;
      span->coverage = (unsigned char)coverage;

      ras.num_gray_spans++;
    }
  }


#ifdef DEBUG_GRAYS

  /* to be called while in the debugger */
  gray_dump_cells( RAS_ARG )
  {
    int  yindex;


    for ( yindex = 0; yindex < ras.ycount; yindex++ )
    {
      PCell  cell;


      printf( "%3d:", yindex );

      for ( cell = ras.ycells[yindex]; cell != NULL; cell = cell->next )
        printf( " (%3d, c:%4d, a:%6d)", cell->x, cell->cover, cell->area );
      printf( "\n" );
    }
  }

#endif /* DEBUG_GRAYS */


  static void
  gray_sweep( RAS_ARG_ const MVF_InternalBitmap*  target )
  {
    int  yindex;

    MVF_UNUSED( target );


    if ( ras.num_cells == 0 )
      return;

    ras.num_gray_spans = 0;

    for ( yindex = 0; yindex < ras.ycount; yindex++ )
    {
      PCell   cell  = ras.ycells[yindex];
      TCoord  cover = 0;
      TCoord  x     = 0;


      for ( ; cell != NULL; cell = cell->next )
      {
        TArea  area;


        if ( cell->x > x && cover != 0 )
          gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
                      cell->x - x );

        cover += cell->cover;
        area   = cover * ( ONE_PIXEL * 2 ) - cell->area;

        if ( area != 0 && cell->x >= 0 )
          gray_hline( RAS_VAR_ cell->x, yindex, area, 1 );

        x = cell->x + 1;
      }

      if ( cover != 0 )
        gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
                    ras.count_ex - x );
    }

    if ( ras.num_gray_spans > 0 )
        ras.render_span( ras.span_y, ras.num_gray_spans,
                       ras.gray_spans, &ras.target );
  }



  /*************************************************************************/
  /*                                                                       */
  /*  The following function should only compile in stand_alone mode,      */
  /*  i.e., when building this component without the rest of FreeType.     */
  /*                                                                       */
  /*************************************************************************/

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    MVF_Outline_Decompose                                               */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Walks over an outline's structure to decompose it into individual  */
  /*    segments and Bezier arcs.  This function is also able to emit      */
  /*    `move to' and `close to' operations to indicate the start and end  */
  /*    of new contours in the outline.                                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    outline        :: A pointer to the source target.                  */
  /*                                                                       */
  /*    func_interface :: A table of `emitters', i.e,. function pointers   */
  /*                      called during decomposition to indicate path     */
  /*                      operations.                                      */
  /*                                                                       */
  /*    user           :: A typeless pointer which is passed to each       */
  /*                      emitter during the decomposition.  It can be     */
  /*                      used to store the state during the               */
  /*                      decomposition.                                   */
  /*                                                                       */
  /* <Return>                                                              */
  /*    Error code.  0 means success.                                      */
  /*                                                                       */
  static
  int  MVF_Outline_Decompose( const MVF_InternalOutline*        outline,
                             //const MVF_Outline_Funcs*  func_interface,
                             void*                    user )
  {
#undef SCALED
#if 0
#define SCALED( x )  ( ( (x) << shift ) - delta )
#else
#define SCALED( x )  (x)
#endif

    MVF_Vector   v_last;
    MVF_Vector   v_control;
    MVF_Vector   v_start;

    MVF_Vector*  point;
    MVF_Vector*  limit;
    MVF_Byte*       tags;

    int   n;         /* index of contour in outline     */
    int   first;     /* index of first point in contour */
    int   error;
    MVF_Byte  tag;       /* current point's state           */

#if 0
    int   shift = gray_shift;
    TPos  delta = gray_delta;
#endif


    first = 0;

    for ( n = 0; n < outline->n_contours; n++ )
    {
      int  last;  /* index of last point in contour */


      last  = outline->contours[n];
      limit = outline->points + last;

      v_start = outline->points[first];
      v_last  = outline->points[last];

      v_start.x = SCALED( v_start.x );
      v_start.y = SCALED( v_start.y );

      v_last.x  = SCALED( v_last.x );
      v_last.y  = SCALED( v_last.y );

      v_control = v_start;

      point = outline->points + first;
      tags  = outline->tags  + first;
      tag   = MVF_CURVE_TAG( tags[0] );

#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
      /* A contour cannot start with a cubic control point! */
      if ( tag == MVF_CURVE_TAG_CUBIC )
        goto Invalid_Outline;
#endif

      /* check first point to determine origin */
      if ( tag == MVF_CURVE_TAG_CONIC )
      {
        /* first point is conic control.  Yes, this happens. */
        if ( MVF_CURVE_TAG( outline->tags[last] ) == MVF_CURVE_TAG_ON )
        {
          /* start at last point if it is on the curve */
          v_start = v_last;
          limit--;
        }
        else
        {
          /* if both first and last points are conic,         */
          /* start at their middle and record its position    */
          /* for closure                                      */
          v_start.x = ( v_start.x + v_last.x ) / 2;
          v_start.y = ( v_start.y + v_last.y ) / 2;

          v_last = v_start;
        }
        point--;
        tags--;
      }

      error = gray_move_to( &v_start, user );
      if ( error )
        goto Exit;

      while ( point < limit )
      {
        point++;
        tags++;

        tag = MVF_CURVE_TAG( tags[0] );
        switch ( tag )
        {
        case MVF_CURVE_TAG_ON:  /* emit a single line_to */
          {
            MVF_Vector  vec;


            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            error = gray_line_to( &vec, user );
            if ( error )
              goto Exit;
            continue;
          }

        case MVF_CURVE_TAG_CONIC:  /* consume conic arcs */
          {
            v_control.x = SCALED( point->x );
            v_control.y = SCALED( point->y );

          Do_Conic:
            if ( point < limit )
            {
              MVF_Vector  vec;
              MVF_Vector  v_middle;


              point++;
              tags++;
              tag = MVF_CURVE_TAG( tags[0] );

              vec.x = SCALED( point->x );
              vec.y = SCALED( point->y );

              if ( tag == MVF_CURVE_TAG_ON )
              {
                error = gray_conic_to( &v_control, &vec,
                                                  user );
                if ( error )
                  goto Exit;
                continue;
              }

              if ( tag != MVF_CURVE_TAG_CONIC )
                goto Invalid_Outline;

              v_middle.x = ( v_control.x + vec.x ) / 2;
              v_middle.y = ( v_control.y + vec.y ) / 2;

              error = gray_conic_to( &v_control, &v_middle,
                                                user );
              if ( error )
                goto Exit;

              v_control = vec;
              goto Do_Conic;
            }

            error = gray_conic_to( &v_control, &v_start,
                                              user );
            goto Close;
          }

        default:  /* MVF_CURVE_TAG_CUBIC */
#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
          {
            MVF_Vector  vec1, vec2;

            if ( point + 1 > limit                             ||
                 MVF_CURVE_TAG( tags[1] ) != MVF_CURVE_TAG_CUBIC )
              goto Invalid_Outline;

            point += 2;
            tags  += 2;

            vec1.x = SCALED( point[-2].x );
            vec1.y = SCALED( point[-2].y );

            vec2.x = SCALED( point[-1].x );
            vec2.y = SCALED( point[-1].y );

            if ( point <= limit )
            {
              MVF_Vector  vec;


              vec.x = SCALED( point->x );
              vec.y = SCALED( point->y );

              error = gray_cubic_to( &vec1, &vec2, &vec, user );
              if ( error )
                goto Exit;
              continue;
            }

            error = gray_cubic_to( &vec1, &vec2, &v_start, user );
            goto Close;
          }
#else
            goto Invalid_Outline;
#endif
        }
      }

      /* close the contour with a line segment */
      error = gray_line_to( &v_start, user );

   Close:
      if ( error )
        goto Exit;

      first = last + 1;
    }

    return 0;

  Exit:
    return error;

  Invalid_Outline:
    return ErrRaster_Invalid_Outline;
  }



  typedef struct  TBand_
  {
    TPos  min, max;

  } TBand;


  static int
  gray_convert_glyph_inner( RAS_ARG )
  {
    /*static
    const MVF_Outline_Funcs  func_interface =
    {
      (MVF_Outline_MoveTo_Func) gray_move_to,
      (MVF_Outline_LineTo_Func) gray_line_to,
      (MVF_Outline_ConicTo_Func)gray_conic_to,
      (MVF_Outline_CubicTo_Func)gray_cubic_to,
      0,
      0
    };*/

    volatile int  error = 0;

    if ( mvf_setjmp( ras.jump_buffer ) == 0 )
    {
      error = MVF_Outline_Decompose( &ras.outline, /*&func_interface,*/ &ras );
      gray_record_cell( RAS_VAR );
    }
    else
    {
      error = ErrRaster_Memory_Overflow;
    }

    return error;
  }


  static int
  gray_convert_glyph( RAS_ARG )
  {
    TBand            bands[40];
    TBand* volatile  band;
    int volatile     n, num_bands;
    TPos volatile    min, max, max_y;
    MVF_BBox*         clip;


    /* Set up state in the raster object */
    gray_compute_cbox( RAS_VAR );

    /* clip to target bitmap, exit if nothing to do */
    clip = &ras.clip_box;

    if ( ras.max_ex <= clip->xMin || ras.min_ex >= clip->xMax ||
         ras.max_ey <= clip->yMin || ras.min_ey >= clip->yMax )
      return 0;

    if ( ras.min_ex < clip->xMin ) ras.min_ex = clip->xMin;
    if ( ras.min_ey < clip->yMin ) ras.min_ey = clip->yMin;

    if ( ras.max_ex > clip->xMax ) ras.max_ex = clip->xMax;
    if ( ras.max_ey > clip->yMax ) ras.max_ey = clip->yMax;

    ras.count_ex = ras.max_ex - ras.min_ex;
    ras.count_ey = ras.max_ey - ras.min_ey;

    /* simple heuristic used to speed-up the bezier decomposition -- see */
    /* the code in gray_render_conic() and gray_render_cubic() for more  */
    /* details                                                           */
    ras.conic_level = 32;
#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
    ras.cubic_level = 16;
#endif

    {
      int level = 0;


      if ( ras.count_ex > 24 || ras.count_ey > 24 )
        level++;
      if ( ras.count_ex > 120 || ras.count_ey > 120 )
        level++;

      ras.conic_level <<= level;
#ifndef MVF_FOR_TRUE_TYPE_ONLY // no this case in TTF!!
      ras.cubic_level <<= level;
#endif
    }

    /* setup vertical bands */
    num_bands = (int)( ( ras.max_ey - ras.min_ey ) / ras.band_size );
    if ( num_bands == 0 )  num_bands = 1;
    if ( num_bands >= 39 ) num_bands = 39;

    ras.band_shoot = 0;

    min   = ras.min_ey;
    max_y = ras.max_ey;

    for ( n = 0; n < num_bands; n++, min = max )
    {
      max = min + ras.band_size;
      if ( n == num_bands - 1 || max > max_y )
        max = max_y;

      bands[0].min = min;
      bands[0].max = max;
      band         = bands;

      while ( band >= bands )
      {
        TPos  bottom, top, middle;
        int   error;

        {
          PCell  cells_max;
          int    yindex;
          long   cell_start, cell_end, cell_mod;


          ras.ycells = (PCell*)ras.buffer;
          ras.ycount = band->max - band->min;

          cell_start = sizeof ( PCell ) * ras.ycount;
          cell_mod   = cell_start % sizeof ( TCell );
          if ( cell_mod > 0 )
            cell_start += sizeof ( TCell ) - cell_mod;

          cell_end  = ras.buffer_size;
          cell_end -= cell_end % sizeof( TCell );

          cells_max = (PCell)( (char*)ras.buffer + cell_end );
          ras.cells = (PCell)( (char*)ras.buffer + cell_start );
          if ( ras.cells >= cells_max )
            goto ReduceBands;

          ras.max_cells = cells_max - ras.cells;
          if ( ras.max_cells < 2 )
            goto ReduceBands;

          for ( yindex = 0; yindex < ras.ycount; yindex++ )
            ras.ycells[yindex] = NULL;
        }

        ras.num_cells = 0;
        ras.invalid   = 1;
        ras.min_ey    = band->min;
        ras.max_ey    = band->max;
        ras.count_ey  = band->max - band->min;

        error = gray_convert_glyph_inner( RAS_VAR );

        if ( !error )
        {
          gray_sweep( RAS_VAR_ &ras.target );
          band--;
          continue;
        }
        else if ( error != ErrRaster_Memory_Overflow )
          return 1;

      ReduceBands:
        /* render pool overflow; we will reduce the render band by half */
        bottom = band->min;
        top    = band->max;
        middle = bottom + ( ( top - bottom ) >> 1 );

        /* This is too complex for a single scanline; there must */
        /* be some problems.                                     */
        if ( middle == bottom )
        {
#ifdef DEBUG_GRAYS
          fprintf( stderr, "Rotten glyph!\n" );
#endif
          return 1;
        }

        if ( bottom-top >= ras.band_size )
          ras.band_shoot++;

        band[1].min = bottom;
        band[1].max = middle;
        band[0].min = middle;
        band[0].max = top;
        band++;
      }
    }

    if ( ras.band_shoot > 8 && ras.band_size > 16 )
      ras.band_size = ras.band_size / 2;

    return 0;
  }


  static int
  gray_raster_render( PRaster raster,
                const MVF_InternalOutline*  outline,
                const MVF_InternalBitmap*   target_map,
                unsigned char bitmap_format)
  {
    PWorker            worker;

    if ( !outline || !outline->contours || !outline->points )
      return ErrRaster_Invalid_Outline;

    /* return immediately if the outline is empty */
    if ( outline->n_points == 0 || outline->n_contours <= 0 )
      return 0;

    if ( outline->n_points !=
           outline->contours[outline->n_contours - 1] + 1 )
      return ErrRaster_Invalid_Outline;

    worker = raster->worker;

    /* if direct mode is not set, we must have a target bitmap */
    {
      if ( !target_map )
        return ErrRaster_Invalid_Argument;

      /* nothing to do */
      if ( !target_map->width || !target_map->rows )
        return 0;

      if ( !target_map->buffer )
        return ErrRaster_Invalid_Argument;
    }


    /* compute clipping box */
    {
      /* compute clip box from target pixmap */
      ras.clip_box.xMin = 0;
      ras.clip_box.yMin = 0;
      ras.clip_box.xMax = target_map->width;
      ras.clip_box.yMax = target_map->rows;
    }


    gray_init_cells( worker, raster->buffer, raster->buffer_size );

    ras.outline   = *outline;
    ras.num_cells = 0;
    ras.invalid   = 1;
    ras.band_size = raster->band_size;
    ras.num_gray_spans = 0;

    if ( target_map )
      ras.target = *target_map;

    //ras.render_span      = (MVF_Raster_Span_Func)gray_render_span;
    switch (bitmap_format)
    {
    case MVF_BITMAP_FORMAT_I8:
        worker->render_span = gray_render_span_I8;
        break;
    case MVF_BITMAP_FORMAT_I4:
        worker->render_span = gray_render_span_I4;
        break;
    case MVF_BITMAP_FORMAT_I2:
        worker->render_span = gray_render_span_I2;
        break;
    case MVF_BITMAP_FORMAT_I1:
        worker->render_span = gray_render_span_I1;
        break;
    default:
        MS_ASSERT(0);
    }

    //ras.render_span_data = &ras;


    return gray_convert_glyph( worker );
  }


  /**** RASTER OBJECT CREATION: In standalone mode, we simply use *****/
  /****                         a static object.                  *****/

  static int
  gray_raster_new( void*       memory,
                   MVF_Raster*  araster )
  {
    static TRaster  the_raster;

    MVF_UNUSED( memory );


    *araster = (MVF_Raster)&the_raster;
    MVF_MEM_ZERO( &the_raster, sizeof ( the_raster ) );

    return 0;
  }


  static void
  gray_raster_done( MVF_Raster  raster )
  {
    /* nothing */
    MVF_UNUSED( raster );
  }



  static void
  gray_raster_reset( MVF_Raster  raster,
                     MVF_Byte*      pool_base,
                     long       pool_size )
  {
    PRaster  rast = (PRaster)raster;


    MS_ASSERT( raster );
    {
      MS_ASSERT( pool_base && pool_size >= (long)sizeof ( TWorker ) + 2048 );
      {
        PWorker  worker = (PWorker)pool_base;


        rast->worker      = worker;
        rast->buffer      = pool_base +
                              ( ( sizeof ( TWorker ) + sizeof ( TCell ) - 1 ) &
                                ~( sizeof ( TCell ) - 1 ) );
        rast->buffer_size = (long)( ( pool_base + pool_size ) -
                                    (MVF_Byte*)rast->buffer ) &
                                      ~( sizeof ( TCell ) - 1 );
        rast->band_size   = (int)( rast->buffer_size /
                                     ( sizeof ( TCell ) * 8 ) );

      }
      /*else
      {
        rast->buffer      = NULL;
        rast->buffer_size = 0;
        rast->worker      = NULL;
      }*/
    }
  }


  /*const MVF_Raster_Funcs  mvf_grays_raster =
  {
    MVF_GLYPH_FORMAT_OUTLINE,

    (MVF_Raster_New_Func)     gray_raster_new,
    (MVF_Raster_Reset_Func)   gray_raster_reset,
    (MVF_Raster_Set_Mode_Func)0,
    (MVF_Raster_Render_Func)  gray_raster_render,
    (MVF_Raster_Done_Func)    gray_raster_done
  };*/

////////////////////////////////////////////////////////////////////////////////////////

static MVF_Byte * _s_bitmap_pool = 0;
static MVF_Byte * _s_render_pool = 0;

#if MVF_CACHE_SBITMAP_ENABLE

  typedef struct  MVF_SBmpRec_
  {
    MVF_Bool   used;
    MVF_UShort  mru_order;
    //MVF_UShort hash;
    MVF_Face * face;
    MVF_UShort font_size;
    MVF_UShort unicode;
    MVF_Bitmap bitmap;
    //MVF_Byte   bitmap_data[MVF_CACHE_SBITMAP_UNIT_SIZE];

  } MVF_SBmp;

static MVF_Byte * _s_cache_sbmp_buff = 0; //bitmap buffer
static MVF_SBmp * _s_cache_sbmp_pool = 0; //array of small bitmaps
//static MVF_UShort _s_cache_sbmp_mru_largest = 0;

int _mvf_cache_sbmp_compare(void * a, void * b)
{
    int aa = ((MVF_SBmp*)a)->mru_order;
    int bb = ((MVF_SBmp*)b)->mru_order;
    return bb-aa;
}

#endif

#if MVF_HINTING_ENABLE

//Copy From FTCALC.C///////////////////////////////////////////////////

#define MVF_MIN( a, b )  ( (a) < (b) ? (a) : (b) )
#define MVF_MAX( a, b )  ( (a) > (b) ? (a) : (b) )

#define MVF_ABS( a )     ( (a) < 0 ? -(a) : (a) )


/* we need to define a 64-bits data type here */

#ifdef MVF_LONG64

  typedef MVF_INT64  MVF_Int64;

#else

  typedef struct  MVF_Int64_
  {
    MVF_UInt32  lo;
    MVF_UInt32  hi;

  } MVF_Int64;

#endif /* MVF_LONG64 */


#ifdef MVF_LONG64


  /* documentation is in freetype.h */

  MVF_EXPORT_DEF( MVF_Long )
  MVF_MulDiv( MVF_Long  a,
             MVF_Long  b,
             MVF_Long  c )
  {
    MVF_Int   s;
    MVF_Long  d;


    s = 1;
    if ( a < 0 ) { a = -a; s = -1; }
    if ( b < 0 ) { b = -b; s = -s; }
    if ( c < 0 ) { c = -c; s = -s; }

    d = (MVF_Long)( c > 0 ? ( (MVF_Int64)a * b + ( c >> 1 ) ) / c
                         : 0x7FFFFFFFL );

    return ( s > 0 ) ? d : -d;
  }

#else /* !MVF_LONG64 */


  static void
  mvf_multo64( MVF_UInt32  x,
              MVF_UInt32  y,
              MVF_Int64  *z )
  {
    MVF_UInt32  lo1, hi1, lo2, hi2, lo, hi, i1, i2;


    lo1 = x & 0x0000FFFFU;  hi1 = x >> 16;
    lo2 = y & 0x0000FFFFU;  hi2 = y >> 16;

    lo = lo1 * lo2;
    i1 = lo1 * hi2;
    i2 = lo2 * hi1;
    hi = hi1 * hi2;

    /* Check carry overflow of i1 + i2 */
    i1 += i2;
    hi += (MVF_UInt32)( i1 < i2 ) << 16;

    hi += i1 >> 16;
    i1  = i1 << 16;

    /* Check carry overflow of i1 + lo */
    lo += i1;
    hi += ( lo < i1 );

    z->lo = lo;
    z->hi = hi;
  }


  static MVF_UInt32
  mvf_div64by32( MVF_UInt32  hi,
                MVF_UInt32  lo,
                MVF_UInt32  y )
  {
    MVF_UInt32  r, q;
    MVF_Int     i;


    q = 0;
    r = hi;

    if ( r >= y )
      return (MVF_UInt32)0x7FFFFFFFL;

    i = 32;
    do
    {
      r <<= 1;
      q <<= 1;
      r  |= lo >> 31;

      if ( r >= (MVF_UInt32)y )
      {
        r -= y;
        q |= 1;
      }
      lo <<= 1;
    } while ( --i );

    return q;
  }


  static void
  MVF_Add64( MVF_Int64*  x,
            MVF_Int64*  y,
            MVF_Int64  *z )
  {
    register MVF_UInt32  lo, hi;


    lo = x->lo + y->lo;
    hi = x->hi + y->hi + ( lo < x->lo );

    z->lo = lo;
    z->hi = hi;
  }


  /* documentation is in freetype.h */

  /* The MVF_MulDiv function has been optimized thanks to ideas from      */
  /* Graham Asher.  The trick is to optimize computation when everything */
  /* fits within 32-bits (a rather common case).                         */
  /*                                                                     */
  /*  we compute 'a*b+c/2', then divide it by 'c'. (positive values)     */
  /*                                                                     */
  /*  46340 is FLOOR(SQRT(2^31-1)).                                      */
  /*                                                                     */
  /*  if ( a <= 46340 && b <= 46340 ) then ( a*b <= 0x7FFEA810 )         */
  /*                                                                     */
  /*  0x7FFFFFFF - 0x7FFEA810 = 0x157F0                                  */
  /*                                                                     */
  /*  if ( c < 0x157F0*2 ) then ( a*b+c/2 <= 0x7FFFFFFF )                */
  /*                                                                     */
  /*  and 2*0x157F0 = 176096                                             */
  /*                                                                     */

  MVF_EXPORT_DEF( MVF_Long )
  MVF_MulDiv( MVF_Long  a,
             MVF_Long  b,
             MVF_Long  c )
  {
    long  s;


    if ( a == 0 || b == c )
      return a;

    s  = a; a = MVF_ABS( a );
    s ^= b; b = MVF_ABS( b );
    s ^= c; c = MVF_ABS( c );

    if ( a <= 46340L && b <= 46340L && c <= 176095L && c > 0 )
      a = ( a * b + ( c >> 1 ) ) / c;

    else if ( c > 0 )
    {
      MVF_Int64  temp, temp2;


      mvf_multo64( a, b, &temp );

      temp2.hi = 0;
      temp2.lo = (MVF_UInt32)(c >> 1);
      MVF_Add64( &temp, &temp2, &temp );
      a = mvf_div64by32( temp.hi, temp.lo, c );
    }
    else
      a = 0x7FFFFFFFL;

    return ( s < 0 ? -a : a );
  }
#endif

//////////////////////////////////////////////////////////////////////////

  static void mvf_hints_iup_shift( MVF_HintPoint*  p1,
                MVF_HintPoint*  p2,
                MVF_HintPoint*  ref )
  {
    MVF_HintPoint*  p;
    MVF_Pos    delta = ref->u - ref->v;


    for ( p = p1; p < ref; p++ )
      p->u = p->v + delta;

    for ( p = ref + 1; p <= p2; p++ )
      p->u = p->v + delta;
  }


  static void mvf_hints_iup_interp( MVF_HintPoint*  p1,
                 MVF_HintPoint*  p2,
                 MVF_HintPoint*  ref1,
                 MVF_HintPoint*  ref2 )
  {
    MVF_HintPoint*  p;
    MVF_Pos    u;
    MVF_Pos    v1 = ref1->v;
    MVF_Pos    v2 = ref2->v;
    MVF_Pos    d1 = ref1->u - v1;
    MVF_Pos    d2 = ref2->u - v2;


    if ( p1 > p2 )
      return;

    if ( v1 == v2 )
    {
      for ( p = p1; p <= p2; p++ )
      {
        u = p->v;

        if ( u <= v1 )
          u += d1;
        else
          u += d2;

        p->u = u;
      }
      return;
    }

    if ( v1 < v2 )
    {
      for ( p = p1; p <= p2; p++ )
      {
        u = p->v;

        if ( u <= v1 )
          u += d1;
        else if ( u >= v2 )
          u += d2;
        else
          u = ref1->u + MVF_MulDiv( u - v1, ref2->u - ref1->u, v2 - v1 );

        p->u = u;
      }
    }
    else
    {
      for ( p = p1; p <= p2; p++ )
      {
        u = p->v;

        if ( u <= v2 )
          u += d2;
        else if ( u >= v1 )
          u += d1;
        else
          u = ref1->u + MVF_MulDiv( u - v1, ref2->u - ref1->u, v2 - v1 );

        p->u = u;
      }
    }
  }



  static void mvf_hints_align_weak_points(MVF_UShort n_points, MVF_HintPoint * points,
                                        MVF_UShort * contours, MVF_UShort n_contours,
                                        MVF_Bool is_vertical)
  {
    //AF_Point*  contour       = hints->contours;
    //AF_Point*  contour_limit = contour + hints->num_contours;
    MVF_Byte touch_mask;
    MVF_UShort contour;

    //AF_Point   points        = hints->points;
    MVF_HintPoint*   point_limit   = points + n_points;
    MVF_HintPoint*   point;
    MVF_HintPoint*   end_point;
    MVF_HintPoint*   first_point;


    /* PASS 1: Move segment points to edge positions */

    if (!is_vertical)
    {
      touch_mask = MVF_HINTING_X_TOUCH_TAG; //touch_flag = AF_FLAG_TOUCH_X;

      for ( point = points; point < point_limit; point++ )
      {
        point->u = point->x;
        point->v = point->ox;
      }
    }
    else
    {
      touch_mask = MVF_HINTING_Y_TOUCH_TAG; //touch_flag = AF_FLAG_TOUCH_Y;

      for ( point = points; point < point_limit; point++ )
      {
        point->u = point->y;
        point->v = point->oy;
      }
    }

    point = points;

    //for ( ; contour < contour_limit; contour++ )
    for (contour = 0; contour < n_contours; contour++)
    {
        point       = (contour==0)? points: points+contours[contour-1]; //*contour;
      end_point   = &points[contours[contour]]; //point->prev;
      first_point = point;

      while ( point <= end_point && !( point->touch_flags&touch_mask ) )
        point++;

      if ( point <= end_point )
      {
        MVF_HintPoint*  first_touched = point;
        MVF_HintPoint*  cur_touched   = point;


        point++;
        while ( point <= end_point )
        {
          if ( point->touch_flags & touch_mask )
          {
            /* we found two successive touched points; we interpolate */
            /* all contour points between them                        */
            mvf_hints_iup_interp( cur_touched + 1, point - 1,
                           cur_touched, point );
            cur_touched = point;
          }
          point++;
        }

        if ( cur_touched == first_touched )
        {
          /* this is a special case: only one point was touched in the */
          /* contour; we thus simply shift the whole contour           */
          mvf_hints_iup_shift( first_point, end_point, cur_touched );
        }
        else
        {
          /* now interpolate after the last touched point to the end */
          /* of the contour                                          */
          mvf_hints_iup_interp( cur_touched + 1, end_point,
                         cur_touched, first_touched );

          /* if the first contour point isn't touched, interpolate */
          /* from the contour start to the first touched point     */
          if ( first_touched > points )
            mvf_hints_iup_interp( first_point, first_touched - 1,
                           cur_touched, first_touched );
        }
      }
    }

    /* now save the interpolated values back to x/y */
    if ( !is_vertical )
    {
      for ( point = points; point < point_limit; point++ )
        point->x = point->u;
    }
    else
    {
      for ( point = points; point < point_limit; point++ )
        point->y = point->u;
    }
  }

#endif


#if 0
static void MVF_Outline_Get_CBox( const MVF_InternalOutline*  outline,
                       MVF_BBox           *acbox )
  {
    MVF_Pos  xMin, yMin, xMax, yMax;


    if ( outline && acbox )
    {
      if ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      else
      {
        MVF_Vector*  vec   = outline->points;
        MVF_Vector*  limit = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        for ( ; vec < limit; vec++ )
        {
          MVF_Pos  x, y;


          x = vec->x;
          if ( x < xMin ) xMin = x;
          if ( x > xMax ) xMax = x;

          y = vec->y;
          if ( y < yMin ) yMin = y;
          if ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }
#endif

//-------------------------------------------------------------------------------------------------
/// Render a outline to a bitmap buffer
/// (use bitmap pool or cache pool)
/// @param  face              \b IN: font face handle
/// @param  orig_outline      \b IN: original outline data (before scaled)
/// @param  font_size         \b IN: font pixel size
/// @param  unicode           \b IN: char code (unicode)
/// @param  only_metrics      \b IN: if true, only modify bitmap width/height/x_shift/y_shift to correct values, no rendering
/// @param  flags             \b IN, bitmap format (I1, I2, I4, I8)
/// @param  *bitmap           \b IN,OUT: bitmap data
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
MVF_Error MVF_Render(MVF_Face * face,
                     MVF_Outline * orig_outline,
                     MVF_UShort font_size,
                     MVF_UShort unicode,
                     MVF_Bool only_metrics,
                     MVF_UShort flags,
                     MVF_Bitmap * bitmap)
{

    MVF_Raster raster;
    MVF_Outline outline;
    MVF_Error     error = 0;
    MVF_Short i;
    MVF_Pos  xMin = 0, yMin = 0, xMax = 0, yMax = 0;
    MVF_ULong obuff = (MVF_ULong)face->outline_buffer_pos;
#if MVF_CACHE_SBITMAP_ENABLE
    static MVF_Short cache_idx = 0;
#endif

#if MVF_HINTING_ENABLE
    MVF_Short hindex = -1;
    MVF_HintPoint * hpoints;

    extern void _mvf_load_hinting_touch_points( MVF_Outline * outline,
                 MVF_Short hindex,
                 //MVF_UShort touch_count,
                 MVF_HintPoint * hpoints ); //[OUT]
#endif

    MS_ASSERT( orig_outline != 0 );
    MS_ASSERT( bitmap != 0 );


    if (!_s_bitmap_pool)
    {
        _s_bitmap_pool = (MVF_Byte*) MVF_MALLOC(MVF_BITMAP_POOL_SIZE);
        MS_ASSERT(_s_bitmap_pool);
        if (!_s_bitmap_pool) return -1; //out of memory
    }

#ifdef MVF_RENDER_POOL_START_ADDR
    _s_render_pool = (MVF_Byte*) MVF_RENDER_POOL_START_ADDR;
#else
    if (!_s_render_pool)
    {
        _s_render_pool = (MVF_Byte*) MVF_MALLOC(MVF_RENDER_POOL_SIZE);
        MS_ASSERT(_s_render_pool);
        if (!_s_render_pool) return -1; //out of memory
    }
#endif

    if (font_size < 8)
    {
        return -10; //font size too small...
    }

    //scaled outline
    MVF_MEM_COPY(&outline, orig_outline, sizeof(MVF_Outline));
    /*
    outline.width = MVF_BASE_EM_FLOOR(outline.width, font_size) + 1;
    outline.height = MVF_BASE_EM_CEIL(outline.height, font_size); //ex: Arial 16, align the base line
    outline.x_shift = MVF_BASE_EM_FLOOR(outline.x_shift, font_size); //ex: Arial 16
    outline.y_shift = MVF_BASE_EM_FLOOR(outline.y_shift, font_size); //ex: Arial 16, align the base line
    */

    outline.advance_x =         MVF_BASE_EM_ROUND(outline.advance_x, font_size);
    outline.metrics_height =    MVF_BASE_EM_ROUND(outline.metrics_height, font_size);
    outline.ascender =          MVF_BASE_EM_ROUND(outline.ascender, font_size);
    outline.descender =         MVF_BASE_EM_ROUND(outline.descender, font_size);
    outline.points = 0;
    if ( outline.n_points > 0 )
    {
        outline.points = (MVF_Vector*) MVF_ADDR_ALIGN(obuff);
        obuff = (MVF_ULong)outline.points + sizeof(MVF_Vector)*outline.n_points;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
#if MVF_HINTING_ENABLE
        for (hindex = orig_outline->hinting_n_sizes; --hindex >= 0; )
            if (font_size == orig_outline->hinting_sizes[hindex])
                break;
        if ( hindex >= 0 )
        {
            hpoints = (MVF_HintPoint*) MVF_ADDR_ALIGN(obuff);
            obuff = (MVF_ULong)hpoints + sizeof(MVF_HintPoint)*outline.n_points;
            for (i = 0; i < outline.n_points; i++)
            {
                hpoints[i].ox = hpoints[i].x = MVF_BASE_EM_ROUND(orig_outline->points[i].x, font_size);
                hpoints[i].oy = hpoints[i].y = MVF_BASE_EM_ROUND(orig_outline->points[i].y, font_size);
                hpoints[i].touch_flags = orig_outline->tags[i];
            }
            _mvf_load_hinting_touch_points(orig_outline,
                hindex, hpoints);
            mvf_hints_align_weak_points(outline.n_points, hpoints,
                outline.contours, outline.n_contours, 1); //vert
            mvf_hints_align_weak_points(outline.n_points, hpoints,
                outline.contours, outline.n_contours, 0); //hor
            for (i = 0; i < outline.n_points; i++)
            {
                outline.points[i].x = hpoints[i].x;
                outline.points[i].y = hpoints[i].y;
            }
            outline.advance_x = orig_outline->hinting_adv_x[hindex];
        }
        else
        {
            //MVF_MEM_COPY(outline.points, orig_outline->points, sizeof(MVF_Vector)*outline.n_points);
            for (i = 0; i < outline.n_points; i++)
            {
                outline.points[i].x = MVF_BASE_EM_ROUND(orig_outline->points[i].x, font_size);
                outline.points[i].y = MVF_BASE_EM_ROUND(orig_outline->points[i].y, font_size);
            }
        }
#else
        //MVF_MEM_COPY(outline.points, orig_outline->points, sizeof(MVF_Vector)*outline.n_points);
        for (i = 0; i < outline.n_points; i++)
        {
            outline.points[i].x = MVF_BASE_EM_ROUND(orig_outline->points[i].x, font_size);
            outline.points[i].y = MVF_BASE_EM_ROUND(orig_outline->points[i].y, font_size);
        }
#endif


        /* compute the control box, and grid fit it */
        {
            MVF_Vector*  vec   = outline.points;
            MVF_Vector*  limit = vec + outline.n_points;


            xMin = xMax = vec->x;
            yMin = yMax = vec->y;
            vec++;

            for ( ; vec < limit; vec++ )
            {
                MVF_Pos  x, y;


                x = vec->x;
                if ( x < xMin ) xMin = x;
                if ( x > xMax ) xMax = x;

                y = vec->y;
                if ( y < yMin ) yMin = y;
                if ( y > yMax ) yMax = y;
            }
        }

        xMin = MVF_PIX_FLOOR( xMin );
        yMin = MVF_PIX_FLOOR( yMin );
        xMax = MVF_PIX_CEIL( xMax );
        yMax = MVF_PIX_CEIL( yMax );

        /*width  = (MVF_UInt)( ( xMax - xMin ) >> 6 );
        height = (MVF_UInt)( ( yMax - yMin ) >> 6 );
        x_left  = (MVF_Int)( xMin >> 6 );
        y_top   = (MVF_Int)( yMax >> 6 );*/

        /* translate outline to render it into the bitmap */
        {
            MVF_UShort   n;
            MVF_Vector*  vec = outline.points;

            for ( n = 0; n < outline.n_points; n++ )
            {
                vec->x -= xMin;
                vec->y -= yMin;
                vec++;
            }
        }

    }

    bitmap->width           = (MVF_UInt)( ( xMax - xMin ) >> 6 );
    bitmap->rows            = (MVF_UInt)( ( yMax - yMin ) >> 6 );
    bitmap->x_shift         = (MVF_Int)( xMin >> 6 );
    bitmap->y_shift         = (MVF_Int)( yMax >> 6 );
    bitmap->advance_x       = outline.advance_x;
    bitmap->metrics_height  = outline.metrics_height;
    bitmap->ascender        = outline.ascender;
    bitmap->descender       = outline.descender;

    switch (MVF_BITMAP_FORMAT_MASK(flags))
    {
    case MVF_BITMAP_FORMAT_I8:
        bitmap->pitch      = ((bitmap->width+3)>>2)<<2;
        break;
    case MVF_BITMAP_FORMAT_I4:
        bitmap->pitch      = (((bitmap->width+1)/2+3)>>2)<<2;
        break;
    case MVF_BITMAP_FORMAT_I2:
        bitmap->pitch      = (((bitmap->width+3)/4+3)>>2)<<2;
        break;
    case MVF_BITMAP_FORMAT_I1:
        bitmap->pitch      = (((bitmap->width+7)/8+3)>>2)<<2;
        break;
    default:
        MS_ASSERT(0);
        return -12; //unknown bitmap format...
    }

    if ( only_metrics || bitmap->width == 0 || bitmap->rows == 0 )
    {
        bitmap->buffer = 0;
        return 0;
    }

#if 0/*MVF_CACHE_SBITMAP_ENABLE*/
    if (font_size <= MVF_CACHE_SBITMAP_FONT_SIZE_MAX)
    {
        MVF_SBmp * found = 0;
        //MVF_SBmp * unused = 0;
        //MVF_SBmp * smallest = 0;
        //MVF_SBmp * largest = 0;
        MVF_SBmp * ptr;
        MVF_SBmp * limit;

        if (bitmap->rows * bitmap->pitch > MVF_CACHE_SBITMAP_UNIT_SIZE)
            return -11; //font size too large, over bitmap buffer

        if (!_s_cache_sbmp_buff)
        {
            _s_cache_sbmp_buff = (MVF_Byte*) MVF_MALLOC(MVF_CACHE_SBITMAP_UNIT_SIZE*MVF_CACHE_SBITMAP_UNIT_COUNT);
            if (!_s_cache_sbmp_buff) return -1; //out of memory
        }

        if (!_s_cache_sbmp_pool)
        {
            _s_cache_sbmp_pool = (MVF_SBmp*) MVF_MALLOC(sizeof(MVF_SBmp)*MVF_CACHE_SBITMAP_UNIT_COUNT);
            if (!_s_cache_sbmp_pool) return -1; //out of memory

            MVF_MEM_ZERO(_s_cache_sbmp_pool, sizeof(MVF_SBmp)*MVF_CACHE_SBITMAP_UNIT_COUNT);
            for (i = 0; i < MVF_CACHE_SBITMAP_UNIT_COUNT; i++)
                _s_cache_sbmp_pool[i].bitmap.buffer = i*MVF_CACHE_SBITMAP_UNIT_SIZE + _s_cache_sbmp_buff;
        }

        limit = _s_cache_sbmp_pool + MVF_CACHE_SBITMAP_UNIT_COUNT;
        //for (i = 0; i < MVF_CACHE_SBITMAP_UNIT_COUNT; i++)
        for (ptr = _s_cache_sbmp_pool, i = 0; ptr < limit; ptr++, i++)
        {
            if (ptr->unicode == unicode && ptr->used &&
                ptr->font_size == font_size && ptr->face == face)
            {
                found = ptr;
                break;
            }
        }

        if (found)
        {
            //printf("MVF_Render() --> Cache Hit index = %d\n", i);

            #if 0
            bitmap->buffer = found->bitmap.buffer;
            #else
            //MVF_MEM_COPY(bitmap, &found->bitmap, sizeof(MVF_Bitmap));
            MVF_MEM_COPY(bitmap, &found->bitmap, sizeof(MVF_Bitmap));
            #endif

            //TBD:Swap the LU
            return 0; //cache hit!!
        }

        //printf("MVF_Render() --> Cache Missed\n");

    }

    //cache missed

    if (bitmap->rows * bitmap->pitch > MVF_BITMAP_POOL_SIZE)
        return -11; //font size too large, over bitmap buffer

    bitmap->buffer     = _s_bitmap_pool;

#else
    if (bitmap->rows * bitmap->pitch > MVF_BITMAP_POOL_SIZE)
        return -11; //font size too large, over bitmap buffer

    bitmap->buffer     = _s_bitmap_pool;
#endif

    MVF_MEM_ZERO(bitmap->buffer, bitmap->rows*bitmap->pitch);

    if ( outline.n_points != 0 )
    {
        gray_raster_new(0, &raster);
        gray_raster_reset(raster, _s_render_pool, MVF_RENDER_POOL_SIZE);
        error = gray_raster_render(raster, (MVF_InternalOutline*)&outline, (MVF_InternalBitmap*)bitmap, MVF_BITMAP_FORMAT_MASK(flags));
        gray_raster_done(raster);
    }

#if MVF_CACHE_SBITMAP_ENABLE
    //insert bitmap into cache
    if ( (!error) && (font_size<=MVF_CACHE_SBITMAP_FONT_SIZE_MAX)&&(_s_cache_sbmp_pool))
    {

        _s_cache_sbmp_pool[cache_idx].used = 1;
        _s_cache_sbmp_pool[cache_idx].face = face;
        _s_cache_sbmp_pool[cache_idx].font_size = font_size;
        _s_cache_sbmp_pool[cache_idx].unicode = unicode;
        //MVF_MEM_COPY(&_s_cache_sbmp_pool[cache_idx].bitmap, bitmap, sizeof(MVF_Bitmap));
        _s_cache_sbmp_pool[cache_idx].bitmap.rows = bitmap->rows;
        _s_cache_sbmp_pool[cache_idx].bitmap.width = bitmap->width;
        _s_cache_sbmp_pool[cache_idx].bitmap.pitch = bitmap->pitch;
        _s_cache_sbmp_pool[cache_idx].bitmap.advance_x = bitmap->advance_x;
        _s_cache_sbmp_pool[cache_idx].bitmap.x_shift = bitmap->x_shift;
        _s_cache_sbmp_pool[cache_idx].bitmap.y_shift = bitmap->y_shift;
        _s_cache_sbmp_pool[cache_idx].bitmap.metrics_height = bitmap->metrics_height;
        _s_cache_sbmp_pool[cache_idx].bitmap.ascender = bitmap->ascender;
        _s_cache_sbmp_pool[cache_idx].bitmap.descender = bitmap->descender;
        MVF_MEM_COPY(_s_cache_sbmp_pool[cache_idx].bitmap.buffer, bitmap->buffer, MVF_CACHE_SBITMAP_UNIT_SIZE);

        cache_idx++;
        cache_idx%= MVF_CACHE_SBITMAP_UNIT_COUNT;
    }
#endif

    return error;
}

#if MVF_CACHE_SBITMAP_ENABLE
MVF_Error MVF_Render_From_Cache(MVF_Face * face,
                     MVF_UShort font_size,
                     MVF_UShort unicode,
                     MVF_Bitmap * bitmap)
{
    MVF_Short i;
    MVF_SBmp * ptr;
    MVF_SBmp * limit;

    MS_ASSERT( bitmap != 0 );

    if ((font_size < 8) || (font_size > MVF_CACHE_SBITMAP_FONT_SIZE_MAX))
    {
        return 2;
    }

    if (!_s_cache_sbmp_buff)
    {
        _s_cache_sbmp_buff = (MVF_Byte*) MVF_MALLOC(MVF_CACHE_SBITMAP_UNIT_SIZE*MVF_CACHE_SBITMAP_UNIT_COUNT);
        MS_ASSERT(_s_cache_sbmp_buff);
        if (!_s_cache_sbmp_buff) return -1; //out of memory
    }

    if (!_s_cache_sbmp_pool)
    {
        _s_cache_sbmp_pool = (MVF_SBmp*) MVF_MALLOC(sizeof(MVF_SBmp)*MVF_CACHE_SBITMAP_UNIT_COUNT);
        MS_ASSERT(_s_cache_sbmp_pool);
        if (!_s_cache_sbmp_pool) return -1; //out of memory

        MVF_MEM_ZERO(_s_cache_sbmp_pool, sizeof(MVF_SBmp)*MVF_CACHE_SBITMAP_UNIT_COUNT);
        for (i = 0; i < MVF_CACHE_SBITMAP_UNIT_COUNT; i++)
            _s_cache_sbmp_pool[i].bitmap.buffer = i*MVF_CACHE_SBITMAP_UNIT_SIZE + _s_cache_sbmp_buff;
    }

    limit = _s_cache_sbmp_pool + MVF_CACHE_SBITMAP_UNIT_COUNT;

    for (ptr = _s_cache_sbmp_pool, i = 0; ptr < limit; ptr++, i++)
    {
        if (ptr->unicode == unicode && ptr->used &&
            ptr->font_size == font_size && ptr->face == face)
        {
            //printf("MVF_Render() --> Cache Hit index = %d\n", i);
            MVF_MEM_COPY(bitmap, &ptr->bitmap, sizeof(MVF_Bitmap));
            //TBD:Swap the LU

            return 0; //cache hit!!
        }
    }

    MVF_BASE_DEBUG(printf("MVF_Render_From_Cache() --> Cache Missed\n"););

    return 1;
}
#endif

//-------------------------------------------------------------------------------------------------
/// Render a outline to a private bitmap buffer
/// (don't use bitmap pool and cache pool)
/// assume bitmap buffer is ready, rows, pitch are correct values
/// keep margin for each bitmap, for generating bitmap table use
/// @param  face              \b IN: font face handle
/// @param  orig_outline      \b IN: original outline data (before scaled)
/// @param  font_size         \b IN: font pixel size
/// @param  flags             \b IN, bitmap format (I1, I2, I4, I8)
/// @param  *bitmap           \b IN,OUT: bitmap data
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
MVF_Error MVF_Render_Private_Bitmap(
                    MVF_Face * face,
                     MVF_Outline * orig_outline,
                     MVF_UShort font_size,
                     //MVF_UShort unicode,
                     MVF_UShort flags,
                     MVF_InternalBitmap * bitmap)
{

    MVF_Raster raster;
    MVF_Outline outline;
    MVF_Error     error = 0;
    MVF_Short i;
    MVF_Pos  xMin = 0, yMin = 0, xMax = 0, yMax = 0;
    MVF_ULong obuff = (MVF_ULong)face->outline_buffer_pos;
    MVF_UInt bitmap_width, bitmap_rows, bitmap_pitch;

#if MVF_HINTING_ENABLE
    MVF_Short hindex = -1;
    MVF_HintPoint * hpoints;

    extern void _mvf_load_hinting_touch_points( MVF_Outline * outline,
                 MVF_Short hindex,
                 //MVF_UShort touch_count,
                 MVF_HintPoint * hpoints ); //[OUT]
#endif

    MS_ASSERT( orig_outline != 0 );
    MS_ASSERT( bitmap != 0 );
    MS_ASSERT( bitmap->buffer != 0 );
    MS_ASSERT( bitmap->pitch != 0 );


    /*if (!_s_bitmap_pool)
    {
        _s_bitmap_pool = (MVF_Byte*) MVF_MALLOC(MVF_BITMAP_POOL_SIZE);
        //_s_bitmap_pool = (MVF_Byte*¡^MVF_MALLOC(MVF_BITMAP_POOL_SIZE);
        if (!_s_bitmap_pool) return -1; //out of memory
    }*/

#ifdef MVF_RENDER_POOL_START_ADDR
    _s_render_pool = (MVF_Byte*) MVF_RENDER_POOL_START_ADDR;
#else
    if (!_s_render_pool)
    {
        _s_render_pool = (MVF_Byte*) MVF_MALLOC(MVF_RENDER_POOL_SIZE);
        MS_ASSERT(_s_render_pool);
        if (!_s_render_pool) return -1; //out of memory
    }
#endif

    if (font_size < 8)
    {
        return -10; //font size too small...
    }

    //scaled outline
    MVF_MEM_COPY(&outline, orig_outline, sizeof(MVF_Outline));
    /*
    outline.width = MVF_BASE_EM_FLOOR(outline.width, font_size) + 1;
    outline.height = MVF_BASE_EM_CEIL(outline.height, font_size); //ex: Arial 16, align the base line
    outline.x_shift = MVF_BASE_EM_FLOOR(outline.x_shift, font_size); //ex: Arial 16
    outline.y_shift = MVF_BASE_EM_FLOOR(outline.y_shift, font_size); //ex: Arial 16, align the base line
    */

    outline.advance_x =         MVF_BASE_EM_ROUND(outline.advance_x, font_size);
    outline.metrics_height =    MVF_BASE_EM_ROUND(outline.metrics_height, font_size);
    outline.ascender =          MVF_BASE_EM_ROUND(outline.ascender, font_size);
    outline.descender =         MVF_BASE_EM_ROUND(outline.descender, font_size);
    outline.points = 0;
    if ( outline.n_points > 0 )
    {
        outline.points = (MVF_Vector*) MVF_ADDR_ALIGN(obuff);
        obuff = (MVF_ULong)outline.points + sizeof(MVF_Vector)*outline.n_points;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
#if MVF_HINTING_ENABLE
        for (hindex = orig_outline->hinting_n_sizes; --hindex >= 0; )
            if (font_size == orig_outline->hinting_sizes[hindex])
                break;
        if ( hindex >= 0 )
        {
            hpoints = (MVF_HintPoint*) MVF_ADDR_ALIGN(obuff);
            obuff = (MVF_ULong)hpoints + sizeof(MVF_HintPoint)*outline.n_points;
            for (i = 0; i < outline.n_points; i++)
            {
                hpoints[i].ox = hpoints[i].x = MVF_BASE_EM_ROUND(orig_outline->points[i].x, font_size);
                hpoints[i].oy = hpoints[i].y = MVF_BASE_EM_ROUND(orig_outline->points[i].y, font_size);
                hpoints[i].touch_flags = orig_outline->tags[i];
            }
            _mvf_load_hinting_touch_points(orig_outline,
                hindex, hpoints);
            mvf_hints_align_weak_points(outline.n_points, hpoints,
                outline.contours, outline.n_contours, 1); //vert
            mvf_hints_align_weak_points(outline.n_points, hpoints,
                outline.contours, outline.n_contours, 0); //hor
            for (i = 0; i < outline.n_points; i++)
            {
                outline.points[i].x = hpoints[i].x;
                outline.points[i].y = hpoints[i].y;
            }
            outline.advance_x = orig_outline->hinting_adv_x[hindex];
        }
        else
        {
            //MVF_MEM_COPY(outline.points, orig_outline->points, sizeof(MVF_Vector)*outline.n_points);
            for (i = 0; i < outline.n_points; i++)
            {
                outline.points[i].x = MVF_BASE_EM_ROUND(orig_outline->points[i].x, font_size);
                outline.points[i].y = MVF_BASE_EM_ROUND(orig_outline->points[i].y, font_size);
            }
        }
#else
        //MVF_MEM_COPY(outline.points, orig_outline->points, sizeof(MVF_Vector)*outline.n_points);
        for (i = 0; i < outline.n_points; i++)
        {
            outline.points[i].x = MVF_BASE_EM_ROUND(orig_outline->points[i].x, font_size);
            outline.points[i].y = MVF_BASE_EM_ROUND(orig_outline->points[i].y, font_size);
        }
#endif


        /* compute the control box, and grid fit it */
        {
            MVF_Vector*  vec   = outline.points;
            MVF_Vector*  limit = vec + outline.n_points;


            xMin = xMax = vec->x;
            yMin = yMax = vec->y;
            vec++;

            for ( ; vec < limit; vec++ )
            {
                MVF_Pos  x, y;


                x = vec->x;
                if ( x < xMin ) xMin = x;
                if ( x > xMax ) xMax = x;

                y = vec->y;
                if ( y < yMin ) yMin = y;
                if ( y > yMax ) yMax = y;
            }
        }

        if (flags & MVF_BITMAP_ALIGN_LEFT)
        xMin = MVF_PIX_FLOOR( xMin ); //NOTE: without space border...
        else
            xMin = 0; //NOTE: with space border:
        yMin = MVF_PIX_FLOOR( yMin );
        xMax = MVF_PIX_CEIL( xMax );
        if (flags & MVF_BITMAP_ALIGN_TOP)
            yMax = MVF_PIX_CEIL( yMax ); //NOTE: without space border...
        else
            yMax = MVF_MAX(MVF_PIX_MAKE(outline.metrics_height+outline.descender), MVF_PIX_CEIL( yMax )); //NOTE: with space border:

        /*width  = (MVF_UInt)( ( xMax - xMin ) >> 6 );
        height = (MVF_UInt)( ( yMax - yMin ) >> 6 );
        x_left  = (MVF_Int)( xMin >> 6 );
        y_top   = (MVF_Int)( yMax >> 6 );*/

        /* translate outline to render it into the bitmap */
        {
            MVF_UShort   n;
            MVF_Vector*  vec = outline.points;

            for ( n = 0; n < outline.n_points; n++ )
            {
                vec->x -= xMin;
                vec->y -= yMin;
                vec++;
            }
        }

    }

    bitmap->advance_x           = outline.advance_x;

    bitmap_width            = (MVF_UInt)( ( xMax - xMin ) >> 6 );
    bitmap_rows         = (MVF_UInt)( ( yMax - yMin ) >> 6 );

    //for private bitmap generate only...
    //NOTE: we assume align byte pitch is 1, so the formula is:
    //           pitch = (w+(p*8-1))/(p*8)*8*BPP
    switch (MVF_BITMAP_FORMAT_MASK(flags))
    {
    case MVF_BITMAP_FORMAT_I8:
        bitmap_pitch      = (bitmap->width+7)/8*8;
        break;
    case MVF_BITMAP_FORMAT_I4:
        bitmap_pitch      = (bitmap->width+7)/8*4;
        break;
    case MVF_BITMAP_FORMAT_I2:
        bitmap_pitch      = (bitmap->width+7)/8*2;
        break;
    case MVF_BITMAP_FORMAT_I1:
        bitmap_pitch      = (bitmap->width+7)/8;
        break;
    default:
        MS_ASSERT(0);
        return -12; //unknown bitmap format...
    }


    if (bitmap->rows < bitmap_rows)
        return -11; //font size too large, over private bitmap buffer
    if (bitmap->pitch < bitmap_pitch)
        return -11; //font size too large, over private bitmap buffer

    bitmap->rows = bitmap_rows;
    bitmap->width = bitmap_width;
    //don't change pitch....
    //MVF_MEM_ZERO(bitmap->buffer, bitmap->rows*bitmap->pitch);

    if ( outline.n_points != 0 )
    {
        gray_raster_new(0, &raster);
        gray_raster_reset(raster, _s_render_pool, MVF_RENDER_POOL_SIZE);
        error = gray_raster_render(raster, (MVF_InternalOutline*)&outline, bitmap, MVF_BITMAP_FORMAT_MASK(flags));
        gray_raster_done(raster);

        //reduce bitmap rows...
        for (i = bitmap_rows; --i >= 0; )
        {
            MVF_Byte * p = bitmap->buffer + bitmap->pitch*i;
            MVF_Byte * limit = p + bitmap->pitch;
            for (; p < limit; p++)
                if ( *p != 0 )
                    goto END_REDUCE_ROWS;
            bitmap->rows--;
        }
END_REDUCE_ROWS:
        ;
    }

    return error;
}


//-------------------------------------------------------------------------------------------------
/// Free the resource of the render
/// @return none
//-------------------------------------------------------------------------------------------------
void MVF_Done_Render(void)
{
    if (_s_bitmap_pool)
    {
        MVF_FREE(_s_bitmap_pool);
        _s_bitmap_pool = 0;
    }

#ifdef MVF_RENDER_POOL_START_ADDR
    //do nothing
#else
    if (_s_render_pool)
    {
        MVF_FREE(_s_render_pool);
        _s_render_pool = 0;
    }
#endif

#if MVF_CACHE_SBITMAP_ENABLE

    if (_s_cache_sbmp_buff)
    {
        MVF_FREE(_s_cache_sbmp_buff);
        _s_cache_sbmp_buff = 0;
    }
    if (_s_cache_sbmp_pool)
    {
        MVF_FREE(_s_cache_sbmp_pool);
        _s_cache_sbmp_pool = 0;
    }
#endif
}

/* END */
