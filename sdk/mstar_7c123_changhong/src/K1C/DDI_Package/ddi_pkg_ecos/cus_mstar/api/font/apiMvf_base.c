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
/// file apiMvf_base.c
/// @brief MStar Vector Font Engine module.
/// @author MStar Semiconductor Inc.
///
///  Vector Font Engine render. Reference "smooth" render of FreeType2 library
///
/// Features:
/// - Providing rendering function to initial new Face and get Glyph information
///   by ID (UNICODE)
//
////////////////////////////////////////////////////////////////////////////////


#include "string.h"

#include "mvf_config.h"
#include "apiMvf_types.h"
#include "apiMvf.h"

#ifdef MVF_DEBUG

#define MVF_BASE_DEBUG(x)   x

#else //MVF_DEBUG

#define MVF_BASE_DEBUG(x) //x

#endif //MVF_DEBUG

//////////////////////////////////////////////////////////////////////////
// define file structure in little endian stream....

#define MVF_NEXT_ULONG(p) p += sizeof(MVF_ULong)
#define MVF_NEXT_LONG(p) p += sizeof(MVF_Long)
#define MVF_NEXT_U24(p) p += 3
#define MVF_NEXT_S24(p) p += 3
#define MVF_NEXT_USHORT(p) p += sizeof(MVF_UShort)
#define MVF_NEXT_SHORT(p) p += sizeof(MVF_Short)
#define MVF_NEXT_BYTE(p) p += sizeof(MVF_Byte)

#define MVF_PEEK_USHORT(p) ((((MVF_UShort)p[1])<<8)|p[0])
#define MVF_PEEK_SHORT(p) ((MVF_Short)MVF_PEEK_USHORT(p))
#define MVF_PEEK_U24(p) ((((MVF_ULong)p[2])<<16)|(p[1]<<8)|p[0])
#define MVF_PEEK_S24(p) ((MVF_Long)MVF_PEEK_U24(p))
#define MVF_PEEK_ULONG(p) ((((MVF_ULong)p[3])<<24)|(p[2]<<16)|(p[1]<<8)|p[0])
#define MVF_PEEK_LONG(p) ((MVF_Long)MVF_PEEK_ULONG(p))
#define MVF_PEEK_BYTE(p) (*p)

extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;

//-------------------------------------------------------------------------------------------------
/// Load a font face into memory
/// @param  file_base    \b IN: MVF raw data start address
/// @param  file_limit   \b IN: MVF raw data end address (exclude)
/// @param  *aface        \b OUT: font face handle
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
MVF_Error MVF_New_Face(
                      MVF_Byte *  file_base,
                      MVF_Byte *  file_limit, //range: [file_base, file_limit)
                      MVF_Face *        aface)
{
    MVF_Byte * pos;
    //MVF_Byte * end;
    MVF_Short i;
    MVF_UShort n, m, flags, touch_count, hint_sizes;
    MVF_UShort max_outline_size = 1, outline_size; //at least one byte, don't allocate zero

    MS_ASSERT( file_base != 0 );
    MS_ASSERT( file_limit > file_base );
    MS_ASSERT( aface != 0 );

    MVF_BASE_DEBUG(printf("MVF_New_Face() 1, base=%lx\n",(MS_U32) file_base));

    pos = file_base;
    //end = file_base + file_size;
    if ( MVF_PEEK_USHORT(pos) != 0x0710 ) //magic number
        return -2; //file format error
    MVF_NEXT_USHORT(pos);

    if ( MVF_PEEK_USHORT(pos) != MVF_FILE_FORMAT_VERSION ) //file format version
        return -12; //file format error
    MVF_NEXT_USHORT(pos);

    aface->file_base = file_base;
    aface->file_limit = file_limit;
    aface->num_glyphs = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);

    MS_ASSERT( aface->num_glyphs > 0 );
    aface->glyphs = (MVF_Glyph*) MVF_MALLOC(sizeof(MVF_Glyph)*aface->num_glyphs);
    if ( !aface->glyphs )
    {
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
printf("\033[1;35;40mERR!!!! %s %d\033[0m\n", __FUNCTION__, __LINE__);
        return -1; //out of memory....
    }

    for (i = 0; i < aface->num_glyphs; i++)
    {
        if ( pos >= file_limit )
            return -3; //end of file...

        //unicode
        aface->glyphs[i].unicode = MVF_PEEK_USHORT(pos);
        MVF_BASE_DEBUG(printf("parse unicode = %d\n", aface->glyphs[i].unicode));
        MVF_NEXT_USHORT(pos);

        //outline data....
        aface->glyphs[i].outline_data = pos;
        //pos += 4 * (sizeof(MVF_Short)); //width, height, x_shift, y_shift
        m = 0;
        n = MVF_PEEK_USHORT(pos); //n_point
        touch_count = 0;
        MVF_NEXT_USHORT(pos);
        if ( n > 0 )
        {
            flags = MVF_PEEK_BYTE(pos); //use16
            MVF_NEXT_BYTE(pos);
            if (flags & MVF_OUTLINE_X_USE16) //X
                pos += n * (sizeof(MVF_Short));
            else
                pos += n * (3); //use 24bits
            if (flags & MVF_OUTLINE_Y_USE16) //Y
                pos += n * (sizeof(MVF_Short));
            else
                pos += n * (3); //use 24bits
            pos += n * (sizeof(MVF_Byte)); //tags
            pos += sizeof(MVF_Long)*2; //min_x,min_y
            if ( n < 256 ) //can present in a byte...
            {
                m = MVF_PEEK_BYTE(pos); //n_cornor
                MVF_NEXT_BYTE(pos);
                pos += m * (sizeof(MVF_Byte));
            }
            else
            {
                m = MVF_PEEK_USHORT(pos); //n_cornor
                MVF_NEXT_USHORT(pos);
                pos += m * (sizeof(MVF_Short));
            }
#if MVF_HINTING_ENABLE

            hint_sizes = MVF_PEEK_BYTE(pos); //number for hint font sizes...
            MVF_NEXT_BYTE(pos);
            pos += sizeof(MVF_Byte)*hint_sizes; // list of size...
            pos += sizeof(MVF_Byte)*hint_sizes; // list of advance.x...
            touch_count = MVF_PEEK_USHORT(pos);
            MVF_NEXT_USHORT(pos);
            pos += sizeof(MVF_Short)*touch_count*hint_sizes; //touch points..

#endif

        }

#if MVF_HINTING_ENABLE
        outline_size = n * (2*sizeof(MVF_Vector)+sizeof(MVF_HintPoint)) + //orig outline + scaled outline
            m * (sizeof(MVF_Short)) +
            touch_count * sizeof(MVF_Short);
#else
        outline_size = n * (2*sizeof(MVF_Vector)) + //orig outline + scaled outline
            m * (sizeof(MVF_Short));
#endif

        if ( outline_size > max_outline_size )
            max_outline_size = outline_size;

        pos += 4 * sizeof(MVF_Short); //advance.x, metrics_height, ascender, descender

        //kerning data.....
        aface->glyphs[i].kerning_data = pos;
        n = MVF_PEEK_USHORT(pos); //n_kerning_pair
        MVF_NEXT_USHORT(pos);
        pos += n * (sizeof(MVF_Short)+sizeof(MVF_Short)); //unicode,delta_x

    }

    aface->outline_buffer_start = (MVF_Byte*) MVF_MALLOC((max_outline_size*8)/7); //+14% for safe...
    if ( !aface->outline_buffer_start )
        return -1; //out of memory....
    aface->outline_buffer_max = max_outline_size;
    aface->outline_buffer_pos = aface->outline_buffer_start;

    return 0; //ok
}

//-------------------------------------------------------------------------------------------------
/// Free a font face handle
/// @param  face         \b IN: font face handle
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
void MVF_Done_Face(MVF_Face * face)
{
    MS_ASSERT(face != 0);

    if (face->glyphs)
        MVF_FREE(face->glyphs);
    if (face->outline_buffer_start)
        MVF_FREE(face->outline_buffer_start);

    MVF_MEM_ZERO((void*)face, sizeof(MVF_Face));
}

#if MVF_HINTING_ENABLE
//-------------------------------------------------------------------------------------------------
/// parse hinting touched points (internal use)
/// @param  outline           \b IN: original outline data
/// @param  hindex            \b IN: index of hinting sizes
/// @param  hpoints           \b OUT: touched points coodinates
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
void _mvf_load_hinting_touch_points(MVF_Outline * outline,
                 MVF_Short hindex,
                 //MVF_UShort touch_count,
                 MVF_HintPoint * hpoints ) //[OUT]
{
    MVF_Short i;
    MVF_Byte * pos;

    MS_ASSERT( outline != 0 );
    MS_ASSERT( outline->hinting_data != 0 );
    MS_ASSERT( hpoints != 0 );

    pos = outline->hinting_data + sizeof(MVF_Short)*outline->hinting_n_touch_points*hindex;

    for ( i = 0; i < outline->n_points; i++)
    {
        if (hpoints[i].touch_flags & MVF_HINTING_X_TOUCH_TAG)
        {
            hpoints[i].x = MVF_PEEK_SHORT(pos);
            MVF_NEXT_SHORT(pos);
        }
        if (hpoints[i].touch_flags & MVF_HINTING_Y_TOUCH_TAG)
        {
            hpoints[i].y = MVF_PEEK_SHORT(pos);
            MVF_NEXT_SHORT(pos);
        }
    }

}

#endif


//-------------------------------------------------------------------------------------------------
/// get kerning pair distance
/// @param  face              \b IN: font face handle
/// @param  left_char         \b IN: left char unicode
/// @param  right_char        \b IN: right char unicode
/// @param  font_size         \b IN: font pixel size
/// @param  *akerning         \b OUT: offset distance (only use x)
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
MVF_Error MVF_Get_Kerning( MVF_Face *     face,
                  MVF_UShort     left_char,
                  MVF_UShort     right_char,
                  MVF_UShort     font_size,
                  MVF_Vector *    akerning )
{
    MVF_UShort index, i, n;
    MVF_Short L, R, M;
    MVF_Byte * pos;

    MS_ASSERT( face != 0 );
    MS_ASSERT( face->glyphs != 0 );
    MS_ASSERT( akerning != 0 );

    //binary search for matched unicode
    L = 0;
    R = face->num_glyphs - 1;
    while ( L < R )
    {
        M = (L+R)/2;
        index = face->glyphs[M].unicode;
        if (index < left_char)
            L = M+1;
        else if (index > left_char)
            R = M-1;
        else
        {
            L = M;
            break;
        }
    }
    index = face->glyphs[L].unicode;
    if ( index != left_char )
        return -3; //char code not found...

    akerning->x = akerning->y = 0;

    pos = face->glyphs[L].kerning_data;
    n = MVF_PEEK_USHORT(pos);
    if ( n == 0 ) //no kerning pairs....
        return 0;
    MVF_NEXT_USHORT(pos);
    for (i = 0; i < n; i++)
    {
        index = MVF_PEEK_USHORT(pos);
        MVF_NEXT_USHORT(pos);
        if (index == right_char)
        {
            akerning->x = MVF_BASE_EM_ROUND( MVF_PEEK_SHORT(pos), font_size );
            return 0;
        }
        MVF_NEXT_SHORT(pos);
    }
    return 0; //no matched kerning....
}


#if MVF_CACHE_OUTLINE_ENABLE

  typedef struct  MVF_CacheOutlineRec_
  {
    //MVF_Bool   used;
    MVF_Byte * raw_start; //raw outline data pointer

    MVF_Outline outline;

  } MVF_CacheOutline;

static MVF_Byte * _s_cache_outline_buff = 0; //bitmap buffer
static MVF_CacheOutline * _s_cache_outline_pool = 0; //array of small bitmaps
static MVF_UShort _s_cache_outline_last = 0;

#endif

//-------------------------------------------------------------------------------------------------
/// Load a outline of a specified char code (unicode)
/// @param  face              \b IN: font face handle
/// @param  unicode           \b IN: char coce (unicode)
/// @param  *outline          \b OUT: outline and metrics data
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
MVF_Error MVF_Load_Glyph( MVF_Face *   face,
                 MVF_UShort   unicode,
                 MVF_Outline * outline)
{
    MVF_UShort index;
    MVF_Long L, R, M;

    MS_ASSERT( face != 0 );
    MS_ASSERT( outline != 0 );
    MS_ASSERT( face->glyphs != 0 );

    //binary search for matched unicode
    L = 0;
    R = face->num_glyphs - 1;
    while ( L < R )
    {
        M = (L+R)/2;
        index = face->glyphs[M].unicode;
        if (index < unicode)
            L = M+1;
        else if (index > unicode)
            R = M-1;
        else
        {
            L = M;
            break;
        }
    }
    //printf(" L = %ld \n", L);
    index = face->glyphs[L].unicode;
    //printf(" index = %d \n", index);
    if ( index != unicode )
        return -3; //char code not found...

    return MVF_Load_Glyph_By_ID(face, L, outline);
}

//-------------------------------------------------------------------------------------------------
/// Load a outline of a specified glyph ID
/// @param  face              \b IN: font face handle
/// @param  glyph_id          \b IN: glyph ID (the serial number in the font file)
/// @param  *outline          \b OUT: outline and metrics data
/// @return ==0 : successful
/// @return !=0 : fail
//-------------------------------------------------------------------------------------------------
MVF_Error MVF_Load_Glyph_By_ID( MVF_Face *   face,
                 MVF_UShort   glyph_id,
                 MVF_Outline * outline) //[OUT]
{
    MVF_UShort i, flags;
    MVF_Long min_x, min_y;
    MVF_Byte * pos;
    MVF_ULong obuff;

    MS_ASSERT( face != 0 );
    MS_ASSERT( outline != 0 );
    MS_ASSERT( face->glyphs != 0 );

    obuff = (MVF_ULong)face->outline_buffer_start;
    pos = face->glyphs[glyph_id].outline_data;

    MS_ASSERT( pos != 0 );
    MS_ASSERT( obuff != 0 );

    if ( glyph_id >= face->num_glyphs )
        return -6; //no such ID


#if MVF_CACHE_OUTLINE_ENABLE
    if (!_s_cache_outline_buff)
    {
        _s_cache_outline_buff = (MVF_Byte*) MVF_MALLOC(face->outline_buffer_max*MVF_CACHE_OUTLINE_UNIT_COUNT);
        MS_ASSERT(_s_cache_outline_buff);
        if (!_s_cache_outline_buff) return -1; //out of memory
    }

    if (!_s_cache_outline_pool)
    {
        _s_cache_outline_pool = (MVF_CacheOutline*) MVF_MALLOC(sizeof(MVF_CacheOutline)*MVF_CACHE_OUTLINE_UNIT_COUNT);
        MS_ASSERT(_s_cache_outline_pool);
        if (!_s_cache_outline_pool) return -1; //out of memory
        MVF_MEM_ZERO(_s_cache_outline_pool, sizeof(MVF_CacheOutline)*MVF_CACHE_OUTLINE_UNIT_COUNT);
    }

    for (i = 0; i < MVF_CACHE_OUTLINE_UNIT_COUNT; i++)
    {
        if (_s_cache_outline_pool[i].raw_start == pos)
        {
            MVF_MEM_COPY(outline, &_s_cache_outline_pool[i].outline, sizeof(MVF_Outline));
            return 0; //hit cache
        }
    }

    _s_cache_outline_pool[_s_cache_outline_last].raw_start = pos;
    obuff = (MVF_ULong)_s_cache_outline_buff +
        _s_cache_outline_last*face->outline_buffer_max; //points, tags, contours
    /*outline->width = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->height = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->x_shift = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);
    outline->y_shift = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);*/

    outline->n_points = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);

    //AWU
    //touch_count = 0;

    if ( outline->n_points == 0 )
    {
        outline->points = 0;
        outline->tags = 0;
    }
    else
    {
        flags = MVF_PEEK_BYTE(pos); //use16
        MVF_NEXT_BYTE(pos);
        min_x = MVF_PEEK_LONG(pos);
        MVF_NEXT_LONG(pos);
        min_y = MVF_PEEK_LONG(pos);
        MVF_NEXT_LONG(pos);

        outline->points = (MVF_Vector*) MVF_ADDR_ALIGN(obuff);
        obuff = (MVF_ULong)outline->points + sizeof(MVF_Vector)*outline->n_points;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
        //outline->tags = (MVF_Byte*) MVF_ADDR_ALIGN(obuff);
        //obuff = (MVF_ULong)outline->tags + sizeof(MVF_Byte)*outline->n_points;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
        //if ( !outline->points || !outline->tags )
        //  return -1; //out of memory
        if (flags & MVF_OUTLINE_X_USE16) //X
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].x = (MVF_Pos)MVF_PEEK_USHORT(pos) + min_x;
                MVF_NEXT_USHORT(pos);
            }
        else //use 24bits
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].x = (MVF_Pos)MVF_PEEK_U24(pos) + min_x;
                MVF_NEXT_U24(pos);
            }
        if (flags & MVF_OUTLINE_Y_USE16) //Y
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].y = (MVF_Pos)MVF_PEEK_USHORT(pos) + min_y;
                MVF_NEXT_USHORT(pos);
            }
        else //use 24bits
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].y = (MVF_Pos)MVF_PEEK_U24(pos) + min_y;
                MVF_NEXT_U24(pos);
            }

        outline->tags = pos;
        pos += outline->n_points * (sizeof(MVF_Byte)); //tags

        if ( outline->n_points < 256 ) //can present in a byte...
        {
            outline->n_contours = MVF_PEEK_BYTE(pos);
            MVF_NEXT_BYTE(pos);
        }
        else
        {
            outline->n_contours = MVF_PEEK_USHORT(pos);
            MVF_NEXT_USHORT(pos);
        }
    }

    if ( outline->n_contours == 0 )
    {
        outline->contours = 0;
    }
    else
    {

        outline->contours = (MVF_UShort*) MVF_ADDR_ALIGN(obuff);
        obuff = (MVF_ULong)outline->contours + sizeof(MVF_Short)*outline->n_contours;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
        //if ( !outline->contours )
        //  return -1; //out of memory...
        if ( outline->n_points < 256 ) //can present in a byte...
        {
            for (i = 0; i < outline->n_contours; i++)
            {
                outline->contours[i] = MVF_PEEK_BYTE(pos);
                MVF_NEXT_BYTE(pos);
            }
        }
        else
        {
            for (i = 0; i < outline->n_contours; i++)
            {
                outline->contours[i] = MVF_PEEK_USHORT(pos);
                MVF_NEXT_USHORT(pos);
            }
        }
    }

#if MVF_HINTING_ENABLE

    if (outline->n_points == 0)
    {
        outline->hinting_n_touch_points = 0;
        outline->hinting_n_sizes = 0;
        outline->hinting_sizes = 0;
        outline->hinting_data = 0;
    }
    else
    {
        outline->hinting_n_sizes = MVF_PEEK_BYTE(pos);
        MVF_NEXT_BYTE(pos);
        outline->hinting_sizes = pos; //list of size...
        pos += sizeof(MVF_Byte)*outline->hinting_n_sizes;
        outline->hinting_adv_x = pos; //list of advx...
        pos += sizeof(MVF_Byte)*outline->hinting_n_sizes;
        outline->hinting_n_touch_points = MVF_PEEK_USHORT(pos);
        MVF_NEXT_USHORT(pos);
        outline->hinting_data = pos; //touch points....
        pos += sizeof(MVF_Short)*outline->hinting_n_touch_points*outline->hinting_n_sizes;
    }

#endif

    outline->advance_x = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->metrics_height = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->ascender = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);
    outline->descender = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);

    //face->outline_buffer_pos = (MVF_Byte*)obuff;

    MVF_MEM_COPY(&_s_cache_outline_pool[_s_cache_outline_last].outline, outline, sizeof(MVF_Outline));

    _s_cache_outline_last = (_s_cache_outline_last+1)%MVF_CACHE_OUTLINE_UNIT_COUNT;

#else

    /*outline->width = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->height = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->x_shift = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);
    outline->y_shift = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);*/

    outline->n_points = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);

    if ( outline->n_points == 0 )
    {
        outline->points = 0;
        outline->tags = 0;
        outline->n_contours = 0;
    }
    else
    {
        flags = MVF_PEEK_BYTE(pos); //use16
        MVF_NEXT_BYTE(pos);
        min_x = MVF_PEEK_LONG(pos);
        MVF_NEXT_LONG(pos);
        min_y = MVF_PEEK_LONG(pos);
        MVF_NEXT_LONG(pos);

        outline->points = (MVF_Vector*) MVF_ADDR_ALIGN(obuff);
        obuff = (MVF_ULong)outline->points + sizeof(MVF_Vector)*outline->n_points;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
        //outline->tags = (MVF_Byte*) MVF_ADDR_ALIGN(obuff);
        //obuff = (MVF_ULong)outline->tags + sizeof(MVF_Byte)*outline->n_points;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
        //if ( !outline->points || !outline->tags )
        //  return -1; //out of memory
        if (flags & MVF_OUTLINE_X_USE16) //X
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].x = (MVF_Pos)MVF_PEEK_USHORT(pos) + min_x;
                MVF_NEXT_USHORT(pos);
            }
        else //use 24bits
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].x = (MVF_Pos)MVF_PEEK_U24(pos) + min_x;
                MVF_NEXT_U24(pos);
            }
        if (flags & MVF_OUTLINE_Y_USE16) //Y
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].y = (MVF_Pos)MVF_PEEK_USHORT(pos) + min_y;
                MVF_NEXT_USHORT(pos);
            }
        else //use 24bits
            for (i = 0; i < outline->n_points; i++)
            {
                outline->points[i].y = (MVF_Pos)MVF_PEEK_U24(pos) + min_y;
                MVF_NEXT_U24(pos);
            }

        outline->tags = pos;
        pos += outline->n_points * (sizeof(MVF_Byte)); //tags

        if ( outline->n_points < 256 ) //can present in a byte...
        {
            outline->n_contours = MVF_PEEK_BYTE(pos);
            MVF_NEXT_BYTE(pos);
        }
        else
        {
            outline->n_contours = MVF_PEEK_USHORT(pos);
            MVF_NEXT_USHORT(pos);
        }
    }

    if ( outline->n_contours == 0 )
    {
        outline->contours = 0;
    }
    else
    {

        outline->contours = (MVF_UShort*) MVF_ADDR_ALIGN(obuff);
        obuff = (MVF_ULong)outline->contours + sizeof(MVF_Short)*outline->n_contours;
        //ASSERT( obuff < (MVF_ULong)face->outline_buffer_start + MVF_OUTLINE_BUFFER_SIZE );
        //if ( !outline->contours )
        //  return -1; //out of memory...
        if ( outline->n_points < 256 ) //can present in a byte...
        {
            for (i = 0; i < outline->n_contours; i++)
            {
                outline->contours[i] = MVF_PEEK_BYTE(pos);
                MVF_NEXT_BYTE(pos);
            }
        }
        else
        {
            for (i = 0; i < outline->n_contours; i++)
            {
                outline->contours[i] = MVF_PEEK_USHORT(pos);
                MVF_NEXT_USHORT(pos);
            }
        }
    }


#if MVF_HINTING_ENABLE

    if (outline->n_points == 0)
    {
        outline->hinting_n_touch_points = 0;
        outline->hinting_n_sizes = 0;
        outline->hinting_sizes = 0;
        outline->hinting_data = 0;
    }
    else
    {
        outline->hinting_n_sizes = MVF_PEEK_BYTE(pos);
        MVF_NEXT_BYTE(pos);
        outline->hinting_sizes = pos; //list of size...
        pos += sizeof(MVF_Byte)*outline->hinting_n_sizes;
        outline->hinting_adv_x = pos; //list of advx...
        pos += sizeof(MVF_Byte)*outline->hinting_n_sizes;
        outline->hinting_n_touch_points = MVF_PEEK_USHORT(pos);
        MVF_NEXT_USHORT(pos);
        outline->hinting_data = pos; //touch points....
        pos += sizeof(MVF_Short)*outline->hinting_n_touch_points*outline->hinting_n_sizes;
    }

#endif

    outline->advance_x = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->metrics_height = MVF_PEEK_USHORT(pos);
    MVF_NEXT_USHORT(pos);
    outline->ascender = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);
    outline->descender = MVF_PEEK_SHORT(pos);
    MVF_NEXT_SHORT(pos);

    face->outline_buffer_pos = (MVF_Byte*)obuff;
#endif

    return 0;
}

/*void MVF_Done_Outline(
                 MVF_Outline * face)
{
    ASSERT( face != 0 );

    if (face->contours)
        MVF_FREE(face->contours);
    if (face->tags)
        MVF_FREE(face->tags);
    if (face->points)
        MVF_FREE(face->points);
    MVF_MEM_ZERO(face, sizeof(MVF_Outline));
}*/

/* END */
