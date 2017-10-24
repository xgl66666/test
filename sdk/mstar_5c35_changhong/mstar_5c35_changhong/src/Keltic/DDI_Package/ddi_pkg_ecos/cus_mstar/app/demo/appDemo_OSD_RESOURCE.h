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
typedef MS_S8                   FONTHANDLE;      ///< Font handle
typedef MS_S16                  BMPHANDLE;       ///< Bitmap handle

/********************************************************************************/
/*                                 Macro                                        */
/********************************************************************************/
#define MAX_FONT                    30  //5
#define MAX_BITMAP                  600

/********************************************************************************/
/*                                 Enum                                         */
/********************************************************************************/
// return code
/// OSD RESOURCE API return value
typedef enum
{
    OSDSRC_SUCCESS                     = 0,
    OSDSRC_FAIL                        ,
    OSDSRC_NON_ALIGN_ADDRESS           ,
    OSDSRC_NON_ALIGN_PITCH             ,

    OSDSRC_INVALID_BUFF_INFO           ,
    OSDSRC_INVALID_FONT_HANDLE,
    OSDSRC_INVALID_BMP_HANDLE,
    OSDSRC_INVALID_PARAMETERS,
} OSDSRC_Result;

// GE buffer format
typedef enum
{
    /// font mode I1
    BMP_FMT_I1          = 0x0,
    /// font mode I2
    BMP_FMT_I2          = 0x1,
    /// font mode I4
    BMP_FMT_I4          = 0x2,
    /// color format palette 256(I8)
    BMP_FMT_I8          = 0x4,
    /// color format blinking display
    BMP_FMT_FaBaFgBg2266  = 0x6,
    /// color format blinking display
    BMP_FMT_1ABFgBg12355  = 0x7,
    /// color format RGB565
    BMP_FMT_RGB565      = 0x8,
    /// color format ORGB1555
    BMP_FMT_ARGB1555    = 0x9,
    /// color format ARGB4444
    BMP_FMT_ARGB4444    = 0xa,
    /// color format for blinking display (will be obsoleted)
    BMP_FMT_1BAAFGBG123433= 0xb,
    /// color format YUV422
    BMP_FMT_YUV422       = 0xe,
    /// color format ARGB8888
    BMP_FMT_ARGB8888    = 0xf
} BMP_Buffer_Format;

/// Define font table information
typedef struct
{
    MS_U32 start_addr;         ///< start address of glyph data. For the use of reload font.
    MS_U32 glyph_addr;         ///< address of glyph data
    MS_U32 codemap_addr;       ///< address of codemap block data. Each codemap block is a tuple of (start code, end code, glyph start index)
    MS_U16 char_num;           ///< Character number
    MS_U16 codemap_block_num;  ///< number of codemap block.
    MS_U8  width;              ///< font size: width
    MS_U8  pitch;              ///< pitch
    MS_U8  hw_pitch;           ///< pitch
    MS_U8  height;             ///< font size: height
    MS_U8  bpp;                ///< bits per pixel: I1 and I2 are currently supported.
    MS_U32 bbox_addr;            ///< glyph bounding box address. BBox is in tuple of (x, width, y, height).
    MS_U16 char_glyph_bytes;   ///< Size of each character glyph including possible paddings. Value 0 indicates to be calculated by driver.
    MS_U8  memory_type;        ///< Memory type: MIU_FLASH and MIU_SDRAM
    BMP_Buffer_Format fmt;
    MS_U32 inUsed;
    MS_U32 reserved_tag;
    MS_U8 reserved[8];
} OSD_RESOURCE_FONT_INFO;

//=============================================================================
// Bitmap information
//=============================================================================
typedef struct
{
    MS_U32 addr;       // flat address of whole memory map
    MS_U32 len;
    MS_U32 inUsed;
    MS_U32 width;
    MS_U32 pitch;
    MS_U32 height;
    BMP_Buffer_Format fmt;
    MS_U32 reserved_tag;
    MS_U8 reserved[8];
} OSD_RESOURCE_BITMAP_INFO;

FONTHANDLE OSD_RESOURCE_LoadFont (MS_U32 start_addr, MS_U32 glyph_addr, MS_U32 bbox_addr, MS_U32 codemap_addr, MS_U16 MS_U16char_num, MS_U16 codemap_block_num, MS_U8 width, MS_U8 height, MS_U8 bpp, MS_U16 char_glyph_bytes, MS_U32 pitch);
BMPHANDLE OSD_RESOURCE_LoadBitmap(MS_U32 addr, MS_U32 len, MS_U32 width, MS_U32 height, BMP_Buffer_Format fmt);
MS_U32 OSD_RESOURCE_GetFontInfoGFX(FONTHANDLE handle, GFX_FontInfo* pinfo);
MS_U32 OSD_RESOURCE_GetBitmapInfoGFX(BMPHANDLE handle, GFX_BitmapInfo* pinfo);

