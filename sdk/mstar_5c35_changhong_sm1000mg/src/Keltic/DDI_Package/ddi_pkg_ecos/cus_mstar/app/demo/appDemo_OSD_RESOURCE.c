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
//#include <linux/string.h>
#include "string.h"
#include "MsCommon.h"

//#include "MsTypes.h"
#include "apiGFX.h"
#include "apiGOP.h"
#include "appDemo_OSD_RESOURCE.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
///GOP_PALETTE_ENTRY_NUM
#define GOP_PALETTE_ENTRY_NUM   256

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

unsigned char _RGB565BMPBIN[]  =
{
    #include "./bindata/_p400x400_RGB565bmp.dat"
};

unsigned char _ARGB4444BMPBIN[]  =
{
    #include "./bindata/_p400x400_ARGB4444bmp.dat"
};

unsigned char _ARGB8888BMPBIN[]  =
{
    #include "./bindata/_p400x400_ARGB8888bmp.dat"
};

unsigned char _I8BMPBIN[]  =
{
    #include "./bindata/_p400x400_I8bmp.dat"
};

unsigned char _FONTI1BIN[]  =
{
    #include "./bindata/ftMstar_I1.dat"
};

unsigned char _FONTI2BIN[]  =
{
    #include "./bindata/ftMstar_I2.dat"
};

unsigned char _FONTI4BIN[]  =
{
    #include "./bindata/ftMstar_I4.dat"
};

//this palette is used for I8 bitmap testing
GOP_PaletteEntry _gopI8PaletteEntry[GOP_PALETTE_ENTRY_NUM] =
{
    {{  0,  0,  0,  0}}, {{  0,  0,128,  0}}, {{  0,128,  0,  0}}, {{  0,128,128,  0}},
    {{128,  0,  0,  0}}, {{128,  0,128,  0}}, {{128,128,  0,  0}}, {{192,192,192,  0}},
    {{192,220,192,  0}}, {{240,202,166,  0}}, {{  0, 32, 64,  0}}, {{  0, 32, 96,  0}},
    {{  0, 32,128,  0}}, {{  0, 32,160,  0}}, {{  0, 32,192,  0}}, {{  0, 32,224,  0}},
    {{  0, 64,  0,  0}}, {{  0, 64, 32,  0}}, {{  0, 64, 64,  0}}, {{  0, 64, 96,  0}},
    {{  0, 64,128,  0}}, {{  0, 64,160,  0}}, {{  0, 64,192,  0}}, {{  0, 64,224,  0}},
    {{  0, 96,  0,  0}}, {{  0, 96, 32,  0}}, {{  0, 96, 64,  0}}, {{  0, 96, 96,  0}},
    {{  0, 96,128,  0}}, {{  0, 96,160,  0}}, {{  0, 96,192,  0}}, {{  0, 96,224,  0}},
    {{  0,128,  0,  0}}, {{  0,128, 32,  0}}, {{  0,128, 64,  0}}, {{  0,128, 96,  0}},
    {{  0,128,128,  0}}, {{  0,128,160,  0}}, {{  0,128,192,  0}}, {{  0,128,224,  0}},
    {{  0,160,  0,  0}}, {{  0,160, 32,  0}}, {{  0,160, 64,  0}}, {{  0,160, 96,  0}},
    {{  0,160,128,  0}}, {{  0,160,160,  0}}, {{  0,160,192,  0}}, {{  0,160,224,  0}},
    {{  0,192,  0,  0}}, {{  0,192, 32,  0}}, {{  0,192, 64,  0}}, {{  0,192, 96,  0}},
    {{  0,192,128,  0}}, {{  0,192,160,  0}}, {{  0,192,192,  0}}, {{  0,192,224,  0}},
    {{  0,224,  0,  0}}, {{  0,224, 32,  0}}, {{  0,224, 64,  0}}, {{  0,224, 96,  0}},
    {{  0,224,128,  0}}, {{  0,224,160,  0}}, {{  0,224,192,  0}}, {{  0,224,224,  0}},
    {{ 64,  0,  0,  0}}, {{ 64,  0, 32,  0}}, {{ 64,  0, 64,  0}}, {{ 64,  0, 96,  0}},
    {{ 64,  0,128,  0}}, {{ 64,  0,160,  0}}, {{ 64,  0,192,  0}}, {{ 64,  0,224,  0}},
    {{ 64, 32,  0,  0}}, {{ 64, 32, 32,  0}}, {{ 64, 32, 64,  0}}, {{ 64, 32, 96,  0}},
    {{ 64, 32,128,  0}}, {{ 64, 32,160,  0}}, {{ 64, 32,192,  0}}, {{ 64, 32,224,  0}},
    {{ 64, 64,  0,  0}}, {{ 64, 64, 32,  0}}, {{ 64, 64, 64,  0}}, {{ 64, 64, 96,  0}},
    {{ 64, 64,128,  0}}, {{ 64, 64,160,  0}}, {{ 64, 64,192,  0}}, {{ 64, 64,224,  0}},
    {{ 64, 96,  0,  0}}, {{ 64, 96, 32,  0}}, {{ 64, 96, 64,  0}}, {{ 64, 96, 96,  0}},
    {{ 64, 96,128,  0}}, {{ 64, 96,160,  0}}, {{ 64, 96,192,  0}}, {{ 64, 96,224,  0}},
    {{ 64,128,  0,  0}}, {{ 64,128, 32,  0}}, {{ 64,128, 64,  0}}, {{ 64,128, 96,  0}},
    {{ 64,128,128,  0}}, {{ 64,128,160,  0}}, {{ 64,128,192,  0}}, {{ 64,128,224,  0}},
    {{ 64,160,  0,  0}}, {{ 64,160, 32,  0}}, {{ 64,160, 64,  0}}, {{ 64,160, 96,  0}},
    {{ 64,160,128,  0}}, {{ 64,160,160,  0}}, {{ 64,160,192,  0}}, {{ 64,160,224,  0}},
    {{ 64,192,  0,  0}}, {{ 64,192, 32,  0}}, {{ 64,192, 64,  0}}, {{ 64,192, 96,  0}},
    {{ 64,192,128,  0}}, {{ 64,192,160,  0}}, {{ 64,192,192,  0}}, {{ 64,192,224,  0}},
    {{ 64,224,  0,  0}}, {{ 64,224, 32,  0}}, {{ 64,224, 64,  0}}, {{ 64,224, 96,  0}},
    {{ 64,224,128,  0}}, {{ 64,224,160,  0}}, {{ 64,224,192,  0}}, {{ 64,224,224,  0}},
    {{128,  0,  0,  0}}, {{128,  0, 32,  0}}, {{128,  0, 64,  0}}, {{128,  0, 96,  0}},
    {{128,  0,128,  0}}, {{128,  0,160,  0}}, {{128,  0,192,  0}}, {{128,  0,224,  0}},
    {{128, 32,  0,  0}}, {{128, 32, 32,  0}}, {{128, 32, 64,  0}}, {{128, 32, 96,  0}},
    {{128, 32,128,  0}}, {{128, 32,160,  0}}, {{128, 32,192,  0}}, {{128, 32,224,  0}},
    {{128, 64,  0,  0}}, {{128, 64, 32,  0}}, {{128, 64, 64,  0}}, {{128, 64, 96,  0}},
    {{128, 64,128,  0}}, {{128, 64,160,  0}}, {{128, 64,192,  0}}, {{128, 64,224,  0}},
    {{128, 96,  0,  0}}, {{128, 96, 32,  0}}, {{128, 96, 64,  0}}, {{128, 96, 96,  0}},
    {{128, 96,128,  0}}, {{128, 96,160,  0}}, {{128, 96,192,  0}}, {{128, 96,224,  0}},
    {{128,128,  0,  0}}, {{128,128, 32,  0}}, {{128,128, 64,  0}}, {{128,128, 96,  0}},
    {{128,128,128,  0}}, {{128,128,160,  0}}, {{128,128,192,  0}}, {{128,128,224,  0}},
    {{128,160,  0,  0}}, {{128,160, 32,  0}}, {{128,160, 64,  0}}, {{128,160, 96,  0}},
    {{128,160,128,  0}}, {{128,160,160,  0}}, {{128,160,192,  0}}, {{128,160,224,  0}},
    {{128,192,  0,  0}}, {{128,192, 32,  0}}, {{128,192, 64,  0}}, {{128,192, 96,  0}},
    {{128,192,128,  0}}, {{128,192,160,  0}}, {{128,192,192,  0}}, {{128,192,224,  0}},
    {{128,224,  0,  0}}, {{128,224, 32,  0}}, {{128,224, 64,  0}}, {{128,224, 96,  0}},
    {{128,224,128,  0}}, {{128,224,160,  0}}, {{128,224,192,  0}}, {{128,224,224,  0}},
    {{192,  0,  0,  0}}, {{192,  0, 32,  0}}, {{192,  0, 64,  0}}, {{192,  0, 96,  0}},
    {{192,  0,128,  0}}, {{192,  0,160,  0}}, {{192,  0,192,  0}}, {{192,  0,224,  0}},
    {{192, 32,  0,  0}}, {{192, 32, 32,  0}}, {{192, 32, 64,  0}}, {{192, 32, 96,  0}},
    {{192, 32,128,  0}}, {{192, 32,160,  0}}, {{192, 32,192,  0}}, {{192, 32,224,  0}},
    {{192, 64,  0,  0}}, {{192, 64, 32,  0}}, {{192, 64, 64,  0}}, {{192, 64, 96,  0}},
    {{192, 64,128,  0}}, {{192, 64,160,  0}}, {{192, 64,192,  0}}, {{192, 64,224,  0}},
    {{192, 96,  0,  0}}, {{192, 96, 32,  0}}, {{192, 96, 64,  0}}, {{192, 96, 96,  0}},
    {{192, 96,128,  0}}, {{192, 96,160,  0}}, {{192, 96,192,  0}}, {{192, 96,224,  0}},
    {{192,128,  0,  0}}, {{192,128, 32,  0}}, {{192,128, 64,  0}}, {{192,128, 96,  0}},
    {{192,128,128,  0}}, {{192,128,160,  0}}, {{192,128,192,  0}}, {{192,128,224,  0}},
    {{192,160,  0,  0}}, {{192,160, 32,  0}}, {{192,160, 64,  0}}, {{192,160, 96,  0}},
    {{192,160,128,  0}}, {{192,160,160,  0}}, {{192,160,192,  0}}, {{192,160,224,  0}},
    {{192,192,  0,  0}}, {{192,192, 32,  0}}, {{192,192, 64,  0}}, {{192,192, 96,  0}},
    {{192,192,128,  0}}, {{192,192,160,  0}}, {{240,251,255,  0}}, {{164,160,160,  0}},
    {{128,128,128,  0}}, {{  0,  0,255,  0}}, {{  0,255,  0,  0}}, {{  0,255,255,  0}},
    {{255,  0,  0,  0}}, {{255,  0,255,  0}}, {{255,255,  0,  0}}, {{255,255,255,  0}}
};

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static OSD_RESOURCE_FONT_INFO                _FontTable[MAX_FONT];
static OSD_RESOURCE_BITMAP_INFO              _BitmapTable[MAX_BITMAP];
OSD_RESOURCE_FONT_INFO GEFontTable;
static MS_U8 font_count = 0;

//-------------------------------------------------------------------------------------------------
/// Get PE bitmap info
/// @param handle \b IN: bitmap handle
/// @param pinfo \b OUT: bitmap information.
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_BMP_HANDLE - Invalid Bitmap handle
//-------------------------------------------------------------------------------------------------
MS_U32 OSD_RESOURCE_GetBitmapInfoGFX(BMPHANDLE handle, GFX_BitmapInfo* pinfo)
{

    if ((handle < MAX_BITMAP) && (handle >= 0))
    {

        pinfo->addr    = _BitmapTable[handle].addr;
        pinfo->len     = _BitmapTable[handle].len;
        pinfo->inUsed  = _BitmapTable[handle].inUsed;
        pinfo->width   = _BitmapTable[handle].width;
        pinfo->height  = _BitmapTable[handle].height;
        pinfo->pitch   = _BitmapTable[handle].pitch;
        pinfo->fmt     = (GFX_Buffer_Format)_BitmapTable[handle].fmt;
        return OSDSRC_SUCCESS;
    }
    return OSDSRC_INVALID_BMP_HANDLE;
}

//-------------------------------------------------------------------------------------------------
/// load PE bitmap info
/// @param addr   \b IN: address
/// @param len    \b IN: total size of bitmap
/// @param width  \b IN: width
/// @param height \b IN: height
/// @param fmt    \b IN: color format (only support 6 color format)
///                     GFX_FMT_I8    \n
///                     GFX_FMT_RGB565\n
///                     GFX_FMT_ARGB1555\n
///                     GFX_FMT_ARGB4444\n
///                     GFX_FMT_1BAAFGBG123433\n
///                     GFX_FMT_ARGB8888\n
/// @return  bitmap handle (  fail to create handle will return ERR_HANDLE )
//-------------------------------------------------------------------------------------------------
BMPHANDLE OSD_RESOURCE_LoadBitmap(MS_U32 addr, MS_U32 len, MS_U32 width, MS_U32 height, BMP_Buffer_Format fmt)
{
    int count;

    printf( "OSD_RESOURCE_LoadBitmap\n" );

    if (addr & 0x7)         // 64 bits aligned check
    {
        return ERR_HANDLE;
    }

    if( ( fmt == BMP_FMT_I1 ) || ( fmt == BMP_FMT_I2 ) || ( fmt == BMP_FMT_I4 ) )
    {
        return ERR_HANDLE;
    }

    for (count = 0; count<MAX_BITMAP; count++ )
    {
        if (_BitmapTable[count].inUsed == false)
            break;
    }

    if ( count == MAX_BITMAP)
    {
        return ERR_HANDLE;
    }

    if( fmt == BMP_FMT_ARGB8888 ) //E_GOP_COLOR_ARGB8888 16-bit align
    {
        _BitmapTable[count].pitch = width << 2;
    }
    else if( fmt == BMP_FMT_I8 ) //E_GOP_COLOR_I8
    {
        _BitmapTable[count].pitch = width ;
    }
    else // E_GOP_COLOR_RGB555 , E_GOP_COLOR_RGB565, E_GOP_COLOR_ARGB4444, E_GOP_COLOR_BLINK
    {
        _BitmapTable[count].pitch = width << 1;
    }
//printf("LoadBMP pre: %d, %x %x %x %x\n", count, _BitmapTable[count].width, _BitmapTable[count].height, _BitmapTable[count].addr, _BitmapTable[count].pitch);
//    _BitmapTable[count].pitch = (((_BitmapTable[count].pitch + 15)>>4)<<4);

    _BitmapTable[count].addr = addr;
    _BitmapTable[count].len = len;
    _BitmapTable[count].width = width;
    _BitmapTable[count].height = height;
    _BitmapTable[count].fmt = fmt;
    _BitmapTable[count].inUsed = true;
//printf("LoadBMP: %d, %x %x %x %x\n", count, _BitmapTable[count].width, _BitmapTable[count].height, _BitmapTable[count].addr, _BitmapTable[count].pitch);
    return  count;
}

//-------------------------------------------------------------------------------------------------
/// Get PE font info
/// @param handle \b IN: font handle
/// @param pinfo \b OUT: font information.
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_FONT_HANDLE - Invalid font handle
//-------------------------------------------------------------------------------------------------
MS_U32 OSD_RESOURCE_GetFontInfoGFX(FONTHANDLE handle, GFX_FontInfo* pinfo)
{

//printf("GetFont: %d, %x, %d, %d\n", handle, _FontTable[handle].glyph_addr,  _FontTable[handle].width, _FontTable[handle].height);

    if ((handle < MAX_FONT) && (handle >= 0))
    {
        pinfo->addr     = _FontTable[handle].glyph_addr;
        pinfo->height   = _FontTable[handle].height;
        pinfo->offset   = _FontTable[handle].char_glyph_bytes;
        pinfo->width    = _FontTable[handle].width;
        pinfo->pitch    = _FontTable[handle].pitch;
        pinfo->pBBox    = (GFX_GlyphBbox*)_FontTable[handle].bbox_addr;
        pinfo->len      = _FontTable[handle].char_glyph_bytes;
        pinfo->inUsed   = _FontTable[handle].inUsed;

        pinfo->fmt = GFX_FMT_I2;
        if(_FontTable[handle].fmt == BMP_FMT_I1)
            pinfo->fmt = GFX_FMT_I1;
        else if(_FontTable[handle].fmt == BMP_FMT_I2)
            pinfo->fmt = GFX_FMT_I2;
        else if(_FontTable[handle].fmt == BMP_FMT_I4)
            pinfo->fmt = GFX_FMT_I4;

        return OSDSRC_SUCCESS;
    }
    return OSDSRC_INVALID_FONT_HANDLE;
}

//-------------------------------------------------------------------------------------------------
/// load PE font info
/// @param addr   \b IN: address
/// @param len    \b IN: total size of font
/// @param width  \b IN: width
/// @param height \b IN: height
/// @param pBBox  \b IN: pointer to bounding box
/// @image html BoundingBox.jpg "in_line & ini_dis"
/// @param fmt    \b IN: color format (only 3 font mode)
///                      GFX_FMT_I1 :  I1 mode\n
///                      GFX_FMT_I2 :  I2 mode\n
///                      GFX_FMT_I4 :  I4 mode\n
/// @return  font handle (  fail to create handle will return ERR_HANDLE )
//-------------------------------------------------------------------------------------------------
FONTHANDLE OSD_RESOURCE_LoadFont (MS_U32 start_addr, MS_U32 glyph_addr, MS_U32 bbox_addr, MS_U32 codemap_addr, MS_U16 u16char_num, MS_U16 codemap_block_num, MS_U8 width, MS_U8 height, MS_U8 bpp, MS_U16 char_glyph_bytes, MS_U32 pitch)
{
//    MS_S8 s8count;
    MS_U8 pitch_bits;

    printf( "msAPI_OSD_RESOURCE_LoadFont\n" );

    pitch_bits = pitch*8;

    GEFontTable.hw_pitch = pitch;
    GEFontTable.start_addr = start_addr;
    //printf("VA: %x, PA: %x\n", start_addr, GEFontTable.start_addr);
    GEFontTable.glyph_addr = glyph_addr;
    GEFontTable.bbox_addr = bbox_addr;
    GEFontTable.char_num = u16char_num;
    GEFontTable.width = width;
    GEFontTable.height = height;
    if (bpp == 3)
    {
        // 3 I1
        GEFontTable.pitch = (width + (pitch_bits-1)) / pitch_bits;   // GE hardware is 2-bytes alignment
        if (char_glyph_bytes == 0)
        {
            GEFontTable.char_glyph_bytes = GEFontTable.pitch * (pitch_bits/8) * height * 3; // 3 1-bit tables
        }
        else
        {
            GEFontTable.char_glyph_bytes = char_glyph_bytes;
        }
    }
    else
    {
        GEFontTable.pitch = (width + (pitch_bits-1)) / pitch_bits * bpp; // GE hardware is 2-bytes alignment
        //GEFontTable[font_count].pitch = (width*bpp + 15) / 16; // GE hardware is 2-bytes alignment
        if (char_glyph_bytes == 0)
        {
            GEFontTable.char_glyph_bytes = GEFontTable.pitch  * height;
        }
        else
        {
            GEFontTable.char_glyph_bytes = char_glyph_bytes;
        }
    }

    GEFontTable.inUsed = TRUE;

    GEFontTable.bpp = bpp;
    GEFontTable.fmt = BMP_FMT_I2;

    if(bpp == 1)
        GEFontTable.fmt = BMP_FMT_I1;
    if(bpp == 2)
        GEFontTable.fmt = BMP_FMT_I2;
    if(bpp == 4)
        GEFontTable.fmt = BMP_FMT_I4;



    GEFontTable.codemap_block_num = codemap_block_num;
    GEFontTable.codemap_addr = codemap_addr;

    //GEFontTable.memory_type = memory_type;



//    MDrv_MIU_Set_FontInfo(font_count, &GEFontTable);

    memcpy(&_FontTable[font_count], &GEFontTable, sizeof(OSD_RESOURCE_FONT_INFO));

 //   printf("LOADTEXT: %x, %x, %x, %x, %x, %x, %x, %x, %x\n", font_count, _FontTable[font_count].glyph_addr, _FontTable[font_count].fmt, _FontTable[font_count].height, _FontTable[font_count].char_glyph_bytes,_FontTable[font_count].char_num,
//       _FontTable[font_count].bbox_addr, _FontTable[font_count].pitch , _FontTable[font_count].width );


    return font_count++;
}

