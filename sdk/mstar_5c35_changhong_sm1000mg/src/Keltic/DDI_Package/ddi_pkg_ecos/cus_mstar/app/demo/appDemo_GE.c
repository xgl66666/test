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
// (!!¡ÓMStar Confidential Information!!L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_GE.c
/// @author MStar Semiconductor Inc.
/// @brief  GE Function Sample Demo
///
/// @name GE Function Sample Command Usage
/// @verbatim
/// ---------------------------------------------------------------------
/// 1. Initial GE
/// Usage:GE_Init index
///     index : The GOP output path index, range {0..2}
/// Sample Command:GE_Init 0
///                GE_Init 1
///                GE_Init 2
///
///
/// ---------------------------------------------------------------------
/// 2. Set configurations
/// Usage:GE_SetConfig
///
/// Sample Command:GE_SetConfig
///
///
/// ----------------------------------------------------------------------------
/// 3. Create GWin
/// Usage:GE_CreateWin GWinWidth0 GWinHeight0 GWinWidth1 GWinHeight1
///     GWinWidth0 : The width of GWin0 to create. it is small than stretch win width
///     GWinHeight0 : The height of GWin0 to create. it is small than stretch win height
///     GWinWidth1 : The width of GWin1 to create. it is small than stretch win width
///     GWinHeight1 : The height of GWin1 to create. it is small than stretch win height
///
/// Sample Command:GE_CreateWin 1280 720
///                GE_CreateWin 1280 720 720 480
///
///
/// ---------------------------------------------------------------------
/// 4. Draw rectangle in the test GOP (0)
/// Usage:GE_FillBlock index RectX RectY RectWidth RectHeight
///     index : The color index, range {0..7}
///         0: r = 255, g = 0, b = 0, a = 255
///         1: r = 0, g = 255, b = 0, a = 255
///         2: r = 0, g = 0, b = 255, a = 255
///         3: r = 255, g = 255, b = 0, a = 128
///         4: r = 0, g = 255, b = 255, a = 128
///         5: r = 255, g = 0, b = 255, a = 128
///         6: r = 255, g = 255, b = 255, a = 255
///         7: r = 0, g = 0, b = 0, a = 255
///     RectX : The x start point, range {0..1280} for this sample code
///     RectY : The y start point, range {0..720} for this sample code
///     RectWidth : The filling width, (RectX + RectWidth -1) range {0..1280} for this sample code
///     RectHeight : The filling height, (RectY + RectHeight -1) range {0..720} for this sample code
///
/// Sample Command:GE_FillBlock 0 100 100 300 300
///                GE_FillBlock 2 400 400 300 300
///
///
/// ---------------------------------------------------------------------
/// 5. Draw Line
/// Usage:GE_DrawLine Color X1 Y1 X2 Y2
///     index : The color index, range {0..7}
///         0: r = 255, g = 0, b = 0, a = 255
///         1: r = 0, g = 255, b = 0, a = 255
///         2: r = 0, g = 0, b = 255, a = 255
///         3: r = 255, g = 255, b = 0, a = 128
///         4: r = 0, g = 255, b = 255, a = 128
///         5: r = 255, g = 0, b = 255, a = 128
///         6: r = 255, g = 255, b = 255, a = 255
///         7: r = 0, g = 0, b = 0, a = 255
///     X1 : X1, range {0..1280} for this sample code
///     Y1 : Y1, range {0..720} for this sample code
///     X2 : X2, range {0..1280} for this sample code
///     Y2 : Y2, range {0..720} for this sample code
///
/// Sample Command:GE_DrawLine 1 0 0 400 400
///                GE_DrawLine 5 0 400 400 0
///
///
/// ---------------------------------------------------------------------
/// 6. Draw Bitmap
/// Usage:GE_DrawBMP Index X Y Width Height
///     Index : The bitmap index, range {0..3}
///         0: ARGB8888 format bitmap
///         1: RGB565 format bitmap
///         2: ARGB4444 format bitmap
///         3: I8 format bitmap
///     X : X, range {0..1280} for this sample code
///     Y : Y, range {0..720} for this sample code
///     Width : The drawing width, (X + Width -1) range {0..1280} for this sample code
///     Height : The drawing height, (Y + Height -1) range {0..720} for this sample code
///
/// Sample Command:GE_DrawBMP 0 200 200 400 400
///                GE_DrawBMP 1 600 200 400 400
///
///
/// ---------------------------------------------------------------------
/// 7. Draw Bitmap with rotation and mirror option
/// Usage:GE_DrawBmpRotate X Y RotateIndex MirrorX MirrorY)
///     X : X, range {0..1280} for this sample code
///     Y : Y, range {0..720} for this sample code
///     RotateIndex : Rotation Direction
///         GEROTATE_0   = 0
///         GEROTATE_90  = 1
///         GEROTATE_180 = 2
///         GEROTATE_270 = 3
///     MirrorX : Enable mirror by X-axis
///     MirrorY : Enable mirror by Y-axis
///
/// Sample Command:GE_DrawBmpRotate 200 200 1 0 0
///                GE_DrawBmpRotate 200 200 2 0 0
///                GE_DrawBmpRotate 200 200 3 0 0
///                GE_DrawBmpRotate 200 200 0 1 0
///                GE_DrawBmpRotate 200 200 0 0 0
///
///
/// ---------------------------------------------------------------------
/// 8. Draw a bitmap (400x400) with scaling option
/// Usage:GE_DrawBmpScale  X Y ScaleX ScaleY
///     X : X, range {0..1280} for this sample code
///     Y : Y, range {0..720} for this sample code
///     ScaleX : X-axis scaling parameter, range {1..3} for this sample code
///     ScaleY : Y-axis scaling parameter, range {1..3} for this sample code
///
/// Sample Command:GE_DrawBmpScale 100 100 1 1
///                GE_DrawBmpScale 100 100 2 1
///
///
/// ---------------------------------------------------------------------
/// 9. convert a RGB565 color format to ARGB4444 image bugger. We use GWIN 0 to see the result
/// Usage:GE_ConvertColor
///
/// Sample Command:GE_ConvertColor
///
/// ---------------------------------------------------------------------
/// 10. ScreenCopy with Rop option
/// Usage:GE_ScreenCopyRop RopIndex SrcColorIndex DstColorIndex
///     RopIndex : Rop operation
///         ROP2_OP_ZORE          = 0,  // rop_result = 0;
///         ROP2_OP_NOT_PS_OR_PD  = 1,  // rop_result = ~( rop_src | rop_dst );
///         ROP2_OP_NS_AND_PD     = 2,  // rop_result = ((~rop_src) & rop_dst);
///         ROP2_OP_NS            = 3,  // rop_result = ~(rop_src);
///         ROP2_OP_PS_AND_ND     = 4,  // rop_result = (rop_src & (~rop_dst));
///         ROP2_OP_ND            = 5,  // rop_result = ~(rop_dst);
///         ROP2_OP_PS_XOR_PD     = 6,  // rop_result = ( rop_src ^ rop_dst);
///         ROP2_OP_NOT_PS_AND_PD = 7,  // rop_result = ~(rop_src & rop_dst);
///         ROP2_OP_PS_AND_PD     = 8,  // rop_result = (rop_src & rop_dst);
///         ROP2_OP_NOT_PS_XOR_PD = 9,  // rop_result = ~(rop_dst ^ rop_src);
///         ROP2_OP_PD            = 10, // rop_result = rop_dst;
///         ROP2_OP_NS_OR_PD      = 11, // rop_result = (rop_dst | (~rop_src));
///         ROP2_OP_PS            = 12, // rop_result = rop_src;
///         ROP2_OP_PS_OR_ND      = 13, // rop_result = (rop_src | (~rop_dst));
///         ROP2_OP_PD_OR_PS      = 14, // rop_result = 0xffffff;
///         ROP2_OP_ONE           = 15, // rop_result = (rop_dst | rop_src);
///
///     SrcColorIndex : The color index, range {0..7}
///         0: r = 255, g = 0, b = 0, a = 255
///         1: r = 0, g = 255, b = 0, a = 255
///         2: r = 0, g = 0, b = 255, a = 255
///         3: r = 255, g = 255, b = 0, a = 128
///         4: r = 0, g = 255, b = 255, a = 128
///         5: r = 255, g = 0, b = 255, a = 128
///         6: r = 255, g = 255, b = 255, a = 255
///         7: r = 0, g = 0, b = 0, a = 255
///     DstColorIndex : The color index, range {0..7}
///         0: r = 255, g = 0, b = 0, a = 255
///         1: r = 0, g = 255, b = 0, a = 255
///         2: r = 0, g = 0, b = 255, a = 255
///         3: r = 255, g = 255, b = 0, a = 128
///         4: r = 0, g = 255, b = 255, a = 128
///         5: r = 255, g = 0, b = 255, a = 128
///         6: r = 255, g = 255, b = 255, a = 255
///         7: r = 0, g = 0, b = 0, a = 255
///
/// Sample Command:GE_ScreenCopyRop 1 0 1
///                GE_ScreenCopyRop 2 1 2
///                GE_ScreenCopyRop 4 2 0
///                GE_ScreenCopyRop 8 0 1
///                GE_ScreenCopyRop 10 1 2
///                GE_ScreenCopyRop 12 2 0
///
///
/// ---------------------------------------------------------------------
/// 11. GE Buffer BitBlt sample code
/// Usage:GE_BitBltSample SrcX SrcY DstX DstY Width Height
///     SrcX : The X coordinate in source GE buffer. Range {0..1280} in this sample code
///     SrcY : The Y coordinate in source GE buffer. Range {0..720} in this sample code
///     DstX : The X coordinate in destination GE buffer. Range {0..1280} in this sample code
///     DstY : DstY The Y coordinate in destination GE buffer. Range {0..720} in this sample code
///     Width : The copy width, (X + Width -1) range {0..1280} for this sample code
///     Height : The copy height, (Y + Height -1) range {0..720} for this sample code
///
/// Sample Command:GE_BitBltSample 100 100 400 400 200 200
///
///
/// ---------------------------------------------------------------------
/// 12. Text Out
/// Usage:GE_TextOut FontIndex ColorIndex FlagIndex X Y
///     FontIndex : font data in this samle code, range {0..2}
///     ColorIndex : The color index, range {0..7}
///         0: r = 255, g = 0, b = 0, a = 255
///         1: r = 0, g = 255, b = 0, a = 255
///         2: r = 0, g = 0, b = 255, a = 255
///         3: r = 255, g = 255, b = 0, a = 128
///         4: r = 0, g = 255, b = 255, a = 128
///         5: r = 255, g = 0, b = 255, a = 128
///         6: r = 255, g = 255, b = 255, a = 255
///         7: r = 0, g = 0, b = 0, a = 255
///     FlagIndex : font drawing flag, range {0..7}
///           GFXFONT_FLAG_SCALE   = 0x1
///           GFXFONT_FLAG_GAP  = 0x2
///           GFXFONT_FLAG_COMPACT = 0x4
///     X : X  {0..1280} for this sample code
///     Y : Y  {0..720} for this sample code
///
/// Sample Command:GE_TextOut 0 0 0 300 300
///                GE_TextOut 1 2 1 300 350
///                GE_TextOut 1 4 3 300 400
///                GE_TextOut 2 5 4 500 450
///
/// @endverbatim
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_GE.c
   @brief The Sample code for the GE functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//#include <linux/string.h>
#include "string.h"
#include "MsCommon.h"
#include "MsMemory.h"
#include "apiGFX.h"
#include "apiGOP.h"
#include "appDemo_OSD_RESOURCE.h"
#include "apiJPEG.h"
#include "drvSEM.h"

//Scalar
#include "MsTypes2.h"
//#include "ver_dtv_comm.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include "Panel.h"
#include "apiPNL.h"
#ifdef ENABLE_MM_PACKAGE

#ifdef ENABLE_RTSP_PACKAGE
#include "msAPI_RTSP.h"
#endif
#include "msAPI_MM.h"
#include "appDemo_MM.h"
#endif

//------------------------------------------------------------------------------
//  function declare
//------------------------------------------------------------------------------
static void _LoadBitmap(MS_U16 BmpItem);
static void _LoadFont(MS_U16 FontItem);
extern void MAsm_CPU_SwDbgBp(void);
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define E_GOP   0
#define E_GOP1   1

//bitmap size
#define BMPRGB565SIZE    (BMPRGB565W*BMPRGB565H*2ul)
#define BMPARGB4444SIZE    (BMPRGB4444W*BMPRGB4444H*2ul)
#define BMPARGB8888SIZE    (BMPRGB8888W*BMPRGB8888H*4ul)
#define BMPI8SIZE    (BMPRGBI8W*BMPRGBI8H)
#define BMPUV8Y8SIZE (BMPUV8Y8W*BMPUV8Y8H*2ul)

#define BMPRGB565W    400ul
#define BMPRGB565H    400ul
#define BMPRGB4444W    400ul
#define BMPRGB4444H    400ul
#define BMPRGB8888W    400ul
#define BMPRGB8888H    400ul
#define BMPRGBI8W    400ul
#define BMPRGBI8H    400ul
#define BMPUV8Y8W    400ul
#define BMPUV8Y8H    400ul

//font size
#define FONTI1SIZE    0x800
#define FONTI2SIZE    0x1800
#define FONTI4SIZE    0x3000

//bitmap items
#define BMPNUM    4
#define ARGB8888BMP    0x1
#define RGB565BMP    0x2
#define ARGB4444BMP    0x4
#define I8BMP    0x8

//font items
#define FONTNUM    3
#define FontI1    0x1
#define FontI2    0x2
#define FontI4    0x4
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U16 gwin_fmt =(MS_U16) E_MS_FMT_ARGB4444;
#ifdef ENABLE_MM_PACKAGE
static MS_U8 bPhotoDispDone =  FALSE;
#endif
static MS_U8 PathNum = 0;
static MS_U8 GOPCount = 0;

static MS_U16 FontWidth = 25;
static MS_U16 FontHeight = 25;
static MS_U16 FontDis = 10;

static MS_U8 u8BmpCnt = 0;
static MS_U8 u8FontCnt = 0;
static MS_U32 u32BmpDRAMStrAddr;
static MS_U32 u32FontDRAMStrAddr;

//each font bitmap
static BMPHANDLE  hBmpRGB565;
static BMPHANDLE  hBmpARGB4444;
static BMPHANDLE  hBmpARGB8888;
static BMPHANDLE  hBmpI8;

//each font handle
static FONTHANDLE hFontI1;
static FONTHANDLE hFontI2;
static FONTHANDLE hFontI4;

//each bitmap dram adr
static MS_U32 u32BmpRGB565Addr;
static MS_U32 u32BmpARGB4444Addr;
static MS_U32 u32BmpARGB8888Addr;
static MS_U32 u32BmpI8Addr;
static MS_U32 u32BmpUV8Y8Addr;

//each font dram adr
static MS_U32 u32FontI1Addr;
static MS_U32 u32FontI2Addr;
static MS_U32 u32FontI4Addr;

typedef struct
{
    EN_GOP_DST_TYPE GopDest;
    MS_U8 GeWinId;
    MS_U8 u8FBId;
    MS_U16 GopBitmapWidth;
    MS_U16 GopBitmapHeight;
    MS_U16 GeDstFmt;
}GWinInfo;

typedef enum
{
    E_BMP_ARGB8888,
    E_BMP_RGB565,
    E_BMP_ARGB4444,
    E_BMP_I8,
}BMPITEM;

typedef enum
{
    E_FONT_I1,
    E_FONT_I2,
    E_FONT_I4,
}FONTITEM;

typedef struct
{
    BMPHANDLE hBmp;
    GFX_Buffer_Format BmpClrFmt;
}BmpInfo;

BmpInfo bmpinfo[BMPNUM];
FONTHANDLE hFontHandle[FONTNUM];
static GWinInfo gWinInfo[] = {{E_GOP_DST_OP0, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB4444},
                        {E_GOP_DST_MIXER2VE, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB4444}};

extern unsigned char _RGB565BMPBIN[];
extern unsigned char _ARGB4444BMPBIN[];
extern unsigned char _ARGB8888BMPBIN[];
extern unsigned char _I8BMPBIN[];

extern unsigned char _FONTI1BIN[];
extern unsigned char _FONTI2BIN[];
extern unsigned char _FONTI4BIN[];

//this palette is used for I8 bitmap testing
extern GOP_PaletteEntry _gopI8PaletteEntry[];

extern MS_BOOL appDemo_XC_Init(void);
extern MS_BOOL appDemo_VE_Init(void);
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static void _Bitmap2Miu0(BMPITEM eBmpOpt)
{
    //BININFO BinInfo;

    switch(eBmpOpt)
    {
        case E_BMP_ARGB8888:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpARGB8888Addr), (void*)_ARGB8888BMPBIN, BMPARGB8888SIZE );
            hBmpARGB8888 = OSD_RESOURCE_LoadBitmap(u32BmpARGB8888Addr, BMPARGB8888SIZE, BMPRGB8888W, BMPRGB8888H, BMP_FMT_ARGB8888);
            bmpinfo[u8BmpCnt].hBmp = hBmpARGB8888;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_ARGB8888;
            break;
        }
        case E_BMP_RGB565:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpRGB565Addr), (void*)_RGB565BMPBIN, BMPRGB565SIZE );
            hBmpRGB565 = OSD_RESOURCE_LoadBitmap(u32BmpRGB565Addr, BMPRGB565SIZE, BMPRGB565W, BMPRGB565H, BMP_FMT_RGB565);
            bmpinfo[u8BmpCnt].hBmp = hBmpRGB565;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_RGB565;
            break;
        }
        case E_BMP_ARGB4444:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpARGB4444Addr), (void*)_ARGB4444BMPBIN, BMPARGB4444SIZE );
            hBmpARGB4444 = OSD_RESOURCE_LoadBitmap(u32BmpARGB4444Addr, BMPARGB4444SIZE, BMPRGB4444W, BMPRGB4444H, BMP_FMT_ARGB4444);
            bmpinfo[u8BmpCnt].hBmp = hBmpARGB4444;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_ARGB4444;
            break;
        }
        case E_BMP_I8:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpI8Addr), (void*)_I8BMPBIN, BMPI8SIZE );
            hBmpI8 = OSD_RESOURCE_LoadBitmap(u32BmpI8Addr, BMPI8SIZE, BMPRGBI8W, BMPRGBI8H, BMP_FMT_I8);
            bmpinfo[u8BmpCnt].hBmp = hBmpI8;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_I8;
            break;
        }
        default:
        {
            printf("\nerror bitmap handle");
            break;
        }
    }
    u8BmpCnt++;
}

static void _Font2Miu0(FONTITEM eFontOpt)
{
    //BININFO BinInfo;

    switch(eFontOpt)
    {
        case E_FONT_I1:
        {
            memcpy((void*)MS_PA2KSEG1(u32FontI1Addr), (void*)_FONTI1BIN, FONTI1SIZE);
            hFontI1 = OSD_RESOURCE_LoadFont(u32FontI1Addr, u32FontI1Addr, 0, 0, 10, 0, 17, 16, 1, 0, 2);
            hFontHandle[u8FontCnt] = hFontI1;
            break;
        }
        case E_FONT_I2:
        {
            memcpy((void*)MS_PA2KSEG1(u32FontI2Addr), (void*)_FONTI2BIN, FONTI2SIZE);
            hFontI2 = OSD_RESOURCE_LoadFont(u32FontI2Addr, u32FontI2Addr, 0, 0, 10, 0, 17, 24, 2, 0, 2);
            hFontHandle[u8FontCnt] = hFontI2;
            break;
        }
        case E_FONT_I4:
        {
            memcpy((void*)MS_PA2KSEG1(u32FontI4Addr), (void*)_FONTI4BIN, FONTI4SIZE);
            hFontI4 = OSD_RESOURCE_LoadFont((MS_U32)&_FONTI4BIN, u32FontI4Addr, 0, 0, 10, 0, 17, 24, 4, 0, 2);
            hFontHandle[u8FontCnt] = hFontI4;
            break;
        }
        default:
        {
            printf("\nerror bitmap handle~~~~~");
            break;
        }
    }
    u8FontCnt++;
}

static void _LoadBitmap(MS_U16 BmpItem)
{
    MS_U8 u8Idx;

    for (u8Idx=0; u8Idx<BMPNUM; u8Idx++)
    {
        switch(BmpItem&(1<<u8Idx))
        {
             case ARGB8888BMP:
                _Bitmap2Miu0(E_BMP_ARGB8888);
                printf("\nload hBmpARGB8888:%d\n",hBmpARGB8888);
                break;

            case RGB565BMP:
                _Bitmap2Miu0(E_BMP_RGB565);
                printf("\nload hBmpRGB565:%d\n",hBmpRGB565);
                break;

            case ARGB4444BMP:
                _Bitmap2Miu0(E_BMP_ARGB4444);
                printf("\nload hBmpARGB4444:%d\n",hBmpARGB4444);
                break;

            case I8BMP:
                _Bitmap2Miu0(E_BMP_I8);
                printf("\nload hBmpI8:%d\n",hBmpI8);
                break;

            default:
                break;
        }
    }
}

static void _LoadFont(MS_U16 FontItem)
{
    MS_U8 u8Idx;

    printf("\nstart load font from flash to dram :)");

    for (u8Idx=0; u8Idx<FONTNUM; u8Idx++)
    {
        switch(FontItem&(1<<u8Idx))
        {
            case FontI1:
                _Font2Miu0(E_FONT_I1);
                printf("\nFontI1 handle:%d",hFontI1);
                break;

            case FontI2:
                _Font2Miu0(E_FONT_I2);
                printf("\nFontI2 handle:%d",hFontI2);
                break;

            case FontI4:
                _Font2Miu0(E_FONT_I4);
                printf("\nFontI4 handle:%d",hFontI4);
                break;
        }
    }
}

static void _GE_InitGlobalVar(void)
{
    u32BmpDRAMStrAddr = SDRAM_ADR_BMP_ADR;
    u32FontDRAMStrAddr = SDRAM_ADR_FONT_ADR;
    memset (&bmpinfo, 0xFF, sizeof (bmpinfo));
    memset (&hFontHandle, 0xFF, sizeof (hFontHandle));
}

static void _GE_SetBmpFontAdr(void)
{
    u32BmpARGB8888Addr=u32BmpDRAMStrAddr;
    u32BmpRGB565Addr= GE_ADDR_ALIGNMENT(u32BmpARGB8888Addr+BMPARGB8888SIZE);
    u32BmpARGB4444Addr= GE_ADDR_ALIGNMENT(u32BmpRGB565Addr+BMPRGB565SIZE);
    u32BmpI8Addr= GE_ADDR_ALIGNMENT(u32BmpARGB4444Addr+BMPARGB4444SIZE);
    u32BmpUV8Y8Addr = GE_ADDR_ALIGNMENT(u32BmpI8Addr + BMPI8SIZE);
    u32FontI1Addr=u32FontDRAMStrAddr;
    u32FontI2Addr= GE_ADDR_ALIGNMENT(u32FontI1Addr+FONTI1SIZE);
    u32FontI4Addr= GE_ADDR_ALIGNMENT(u32FontI2Addr+FONTI2SIZE);

    printf("GE Demo Memory Layout Usage \n");
    printf("u32BmpARGB8888Addr   Start  0x%08lx   End 0x%08lx\n",u32BmpARGB8888Addr,u32BmpARGB8888Addr+BMPARGB8888SIZE);
    printf("u32BmpRGB565Addr     Start  0x%08lx   End 0x%08lx\n",u32BmpRGB565Addr,u32BmpRGB565Addr+BMPRGB565SIZE);
    printf("u32BmpARGB4444Addr   Start  0x%08lx   End 0x%08lx\n",u32BmpARGB4444Addr,u32BmpARGB4444Addr+BMPARGB4444SIZE);
    printf("u32BmpI8Addr         Start  0x%08lx   End 0x%08lx\n",u32BmpI8Addr,u32BmpI8Addr+BMPI8SIZE);
    printf("u32BmpUV8Y8Addr      Start  0x%08lx   End 0x%08lx\n",u32BmpUV8Y8Addr,u32BmpUV8Y8Addr + BMPUV8Y8SIZE );
    printf("u32FontI1Addr        Start  0x%08lx   End 0x%08lx\n",u32FontI1Addr,u32FontI1Addr + FONTI1SIZE);
    printf("u32FontI2Addr        Start  0x%08lx   End 0x%08lx\n",u32FontI2Addr,u32FontI2Addr + FONTI2SIZE);
    printf("u32FontI4Addr        Start  0x%08lx   End 0x%08lx\n",u32FontI4Addr,u32FontI4Addr + FONTI4SIZE);
}

static void _GE_ScalerInit(void)
{
    /*-------------XC Init----------------*/
    appDemo_XC_Init();

    /*-------------VE path init----------------*/
    appDemo_VE_Init();
}

static MS_U32 _OSD_SetFBFmt(MS_U16 pitch, MS_U32 addr ,MS_U16 fmt )
{

    GFX_BufferInfo dstbuf;

    printf("OSD set format\n");
    printf("format = 0x%x\n",fmt);
    dstbuf.u32ColorFmt = (GFX_Buffer_Format)(fmt&0xff);
    dstbuf.u32Addr = addr;
    dstbuf.u32Pitch = pitch;
    printf("SetFBFmt: %x, %lx, %lx\n",dstbuf.u32ColorFmt,dstbuf.u32Addr,dstbuf.u32Pitch  );
    MApi_GFX_SetDstBufferInfo(&dstbuf, 0);

    return 0;
}

static void _XC_Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
}

static MS_U16 _sc_get_h_cap_start(void)
{
    // this function should support "multiple process"
    MS_WINDOW_TYPE stCapWin;

    // this function only used when OSD input from IP, so if no input, OSD should be attached to OP
    MApi_XC_GetCaptureWindow(&stCapWin, MAIN_WINDOW);       // read from register directly

    return stCapWin.x;
}
static MS_BOOL _sc_is_interlace(void)
{
    return 0;
}

static MS_U32 _Create_FB(MS_U8 *u8FB, MS_U16 FBWidth, MS_U16 FBHeight, MS_U16 FBFmt)
{
    GOP_GwinFBAttr DstFBInfo;
    // look for available Sdram slot
    *u8FB = MApi_GOP_GWIN_GetFreeFBID();
    if(MAX_GWIN_FB_SUPPORT <= *u8FB)
        return GWIN_NO_AVAILABLE;

    printf("Create FB %d with size (%d,%d) \n", *u8FB, FBWidth, FBHeight);
    if (MApi_GOP_GWIN_CreateFB( *u8FB, 0, 0, FBWidth, FBHeight, FBFmt) != GWIN_OK)
    {
        printf("[%s][%d] MApi_GOP_GWIN_CreateFB: failed to create FB \n", __FUNCTION__, __LINE__);
        return GWIN_NO_AVAILABLE;
    }

    MApi_GOP_GWIN_GetFBInfo( *u8FB, &DstFBInfo);
    if(FBFmt==(MS_U16)E_MS_FMT_YUV422)
        MApi_GFX_ClearFrameBufferByWord(DstFBInfo.addr, DstFBInfo.size, 0x80108010);
    else
        MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0x0);

    MApi_GFX_FlushQueue();

    return 0;
}

static MS_U32 _Create_GWin(MS_U8 u8GOP, MS_U8 *u8GWin, MS_U8 u8FB, MS_U16 FBFmt)
{
    *u8GWin = MApi_GOP_GWIN_CreateWin_Assign_FB(u8GOP, u8FB, 0, 0);
    printf("[%s][%d] Creat GOP %d with GWIM %d\n", __FUNCTION__, __LINE__, u8GOP, *u8GWin);
    if(*u8GWin == GWIN_NO_AVAILABLE)
    {
        return GWIN_NO_AVAILABLE;
    }

    //MApi_GOP_GWIN_SwitchGOP(E_GOP);//use GOP 0
    //MApi_GOP_GWIN_SetGOPDst(E_GOP, GopDest);
    MApi_GOP_GWIN_Switch2Gwin( *u8GWin );  //set gop gwin address for ge render

    //set transparent color
    if( FBFmt == E_MS_FMT_ARGB8888 || FBFmt == E_MS_FMT_ARGB4444)
    {
        MApi_GOP_GWIN_SetBlending( *u8GWin, true, 0xff);
    }else
    {
        MApi_GOP_GWIN_SetBlending( *u8GWin, false, 0xff);
    }

    if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(*u8GWin, TRUE) )
    {
         printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
         return GWIN_FAIL;
    }

    return 0;
}

static void _SetGfxPalette(void)
{
    GFX_PaletteEntry _gePalette[GOP_PALETTE_ENTRY_NUM];
    int i;

    printf("\nload I8 palette to GE");

    for (i=0; i<GOP_PALETTE_ENTRY_NUM; i++)
    {
        _gePalette[i].RGB.u8A =  _gopI8PaletteEntry[i].RGB.u8A;
        _gePalette[i].RGB.u8R =  _gopI8PaletteEntry[i].RGB.u8R;
        _gePalette[i].RGB.u8G =  _gopI8PaletteEntry[i].RGB.u8G;
        _gePalette[i].RGB.u8B =  _gopI8PaletteEntry[i].RGB.u8B;
    }
    //Set GE palette table when bitblt from I8 to ARGB
     MApi_GFX_SetPaletteOpt(_gePalette, 0, GOP_PALETTE_ENTRY_NUM - 1 );
}

static void _SetupGfxDrawRect(GFX_DrawRect *pGfxDrawRect, MS_U16 u16SrcX, MS_U16 u16SrcY, MS_U16 u16SrcWidth,
                                MS_U16 u16SrcHeight, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight)
{
    pGfxDrawRect->srcblk.x = u16SrcX;
    pGfxDrawRect->srcblk.y = u16SrcY;
    pGfxDrawRect->srcblk.width = u16SrcWidth;
    pGfxDrawRect->srcblk.height = u16SrcHeight;

    pGfxDrawRect->dstblk.x = u16DstX;
    pGfxDrawRect->dstblk.y = u16DstY;
    pGfxDrawRect->dstblk.width = u16DstWidth;
    pGfxDrawRect->dstblk.height = u16DstHeight;
}

static void _SetupGfxRectFillInfo(GFX_RectFillInfo *pGfxRectFillInfo, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight,
                                    GFX_Buffer_Format gfxBufFmt, GFX_RgbColor ColorS, GFX_RgbColor ColorE, MS_U32 u32Flag)
{
    pGfxRectFillInfo->dstBlock.x = u16DstX;
    pGfxRectFillInfo->dstBlock.y = u16DstY;
    pGfxRectFillInfo->dstBlock.width= u16DstWidth;
    pGfxRectFillInfo->dstBlock.height= u16DstHeight;


    pGfxRectFillInfo->fmt = gfxBufFmt;

    pGfxRectFillInfo->colorRange.color_s.a = ColorS.a;
    pGfxRectFillInfo->colorRange.color_s.r = ColorS.r;
    pGfxRectFillInfo->colorRange.color_s.g = ColorS.g;
    pGfxRectFillInfo->colorRange.color_s.b = ColorS.b;
    //printf("## s a=%x r=%x g=%x b=%x ##\n",pGfxRectFillInfo->colorRange.color_s.a,pGfxRectFillInfo->colorRange.color_s.r,pGfxRectFillInfo->colorRange.color_s.g,pGfxRectFillInfo->colorRange.color_s.b);
    pGfxRectFillInfo->colorRange.color_e.a = ColorE.a;
    pGfxRectFillInfo->colorRange.color_e.r = ColorE.r;
    pGfxRectFillInfo->colorRange.color_e.g = ColorE.g;
    pGfxRectFillInfo->colorRange.color_e.b = ColorE.g;
    //printf("## e a=%x r=%x g=%x b=%x ##\n",pGfxRectFillInfo->colorRange.color_e.a,pGfxRectFillInfo->colorRange.color_e.r,pGfxRectFillInfo->colorRange.color_e.g,pGfxRectFillInfo->colorRange.color_e.b);

    pGfxRectFillInfo->flag = u32Flag;
}

static void _SetupGfxDrawLineInfo(GFX_DrawLineInfo *pGfxDrawLineInfo, MS_U32 *x1, MS_U32 *y1, MS_U32 *x2, MS_U32 *y2,
                                            GFX_Buffer_Format gfxBufFmt, GFX_RgbColor ColorS, GFX_RgbColor ColorE, MS_U32 u32Width, MS_U32 u32Flag)
{
    pGfxDrawLineInfo->x1 = *x1;
    pGfxDrawLineInfo->y1 = *y1;
    pGfxDrawLineInfo->x2 = *x2;
    pGfxDrawLineInfo->y2 = *y2;

    pGfxDrawLineInfo->fmt = gfxBufFmt;

    pGfxDrawLineInfo->colorRange.color_s.a = ColorS.a;
    pGfxDrawLineInfo->colorRange.color_s.r = ColorS.r;
    pGfxDrawLineInfo->colorRange.color_s.g = ColorS.g;
    pGfxDrawLineInfo->colorRange.color_s.b = ColorS.b;

    pGfxDrawLineInfo->colorRange.color_e.a = ColorE.a;
    pGfxDrawLineInfo->colorRange.color_e.r = ColorE.r;
    pGfxDrawLineInfo->colorRange.color_e.g = ColorE.g;
    pGfxDrawLineInfo->colorRange.color_e.b = ColorE.g;

    pGfxDrawLineInfo->width = u32Width;
    pGfxDrawLineInfo->flag = u32Flag;
}

static void _SetupGfxTextOutInfo(GFX_TextOutInfo *pGfxTextOutInfo, MS_U16 u16X, MS_U16 u16Y, MS_U16 u16Width, MS_U16 u16Height,
                                     MS_U16 dis, GFX_RgbColor Color, MS_U32 u32Flag)
{
    pGfxTextOutInfo->flag = u32Flag;

    //test font bin no glyphinfo, should set NULL
    pGfxTextOutInfo->pGlyphDispInfo = NULL;

    //if don't set "GEFONT_FLAG_COMPACT" flag and "GEFONT_FLAG_GAP" flag, don't care this setting
    pGfxTextOutInfo->dis = dis;
    pGfxTextOutInfo->color.a = Color.a;
    pGfxTextOutInfo->color.r = Color.r;
    pGfxTextOutInfo->color.g = Color.g;
    pGfxTextOutInfo->color.b = Color.b;
    pGfxTextOutInfo->dstblk.x = u16X;
    pGfxTextOutInfo->dstblk.y = u16Y;

    //if don't set "GEFONT_FLAG_SCALE", don't care this setting.
    pGfxTextOutInfo->dstblk.width = u16Width;
    pGfxTextOutInfo->dstblk.height = u16Height;

}

static void _ColorMapping(MS_U8 index, GFX_RgbColor *color)
{
    switch(index)
    {
        case 0:
            (*color).r = 255;
            (*color).g = 0;
            (*color).b = 0;
            (*color).a = 255;
            break;
        case 1:
            (*color).r = 0;
            (*color).g = 255;
            (*color).b = 0;
            (*color).a = 255;
            break;
        case 2:
            (*color).r = 0;
            (*color).g = 0;
            (*color).b = 255;
            (*color).a = 255;
            break;
        case 3:
            (*color).r = 255;
            (*color).g = 255;
            (*color).b = 0;
            (*color).a = 128;
            break;
        case 4:
            (*color).r = 0;
            (*color).g = 255;
            (*color).b = 255;
            (*color).a = 128;
            break;
        case 5:
            (*color).r = 255;
            (*color).g = 0;
            (*color).b = 255;
            (*color).a = 128;
            break;
        case 6:
            (*color).r = 255;
            (*color).g = 255;
            (*color).b = 255;
            (*color).a = 255;
            break;
        case 7:
            (*color).r = 0;
            (*color).g = 0;
            (*color).b = 0;
            (*color).a = 255;
            break;
    }
}

static void _ROPMapping(MS_U8 index, GFX_DrawRect *pGfxDrawRect)
{
    switch(index)
    {
        case 0:
            printf("\n Set GFX raster operation: ------>ROP2_OP_ZERO! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_ZERO);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 1:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NOT_PS_OR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NOT_PS_OR_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 2:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NS_AND_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NS_AND_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 3:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NS! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NS);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 4:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_AND_ND! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_AND_ND);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 5:
            printf("\n Set GFX raster operation: ------>ROP2_OP_ND! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_ND);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 6:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_XOR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_XOR_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 7:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NOT_PS_AND_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NOT_PS_AND_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 8:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_AND_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_AND_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 9:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NOT_PS_XOR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NOT_PS_XOR_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 10:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 11:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NS_OR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NS_OR_PD);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 12:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 13:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_OR_ND! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_OR_ND);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 14:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PD_OR_PS! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PD_OR_PS);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 15:
            printf("\n Set GFX raster operation: ------>ROP2_OP_ONE! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_ONE);
            _SetupGfxDrawRect(pGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
    }

    MApi_GFX_SetROP2(FALSE, ROP2_OP_PS_AND_PD);
}

//------------------------------------------------------------------------------
/// @brief The sample code to initial GE
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_Help Par0 \n
//------------------------------------------------------------------------------
MS_BOOL GE_Help_Part0(void)
{
    printf("*************************GE function List*************************\n");
    printf("1.GE_Init [PathIndex]\n");
    printf("-----PathIndex : The index to choose GOP output path, range {0..2}\n");
    printf("----------------0: GOP0 to OP0\n");
    printf("----------------1: GOP0 to MIXER2OP\n");
    printf("----------------2: GOP0 to OP0, GOP1 to MIXER2VE\n");
    printf("2.GE_SetConfig --Set configurations including stretch win size\n");
    printf("3.GE_CreateWin [GWinWidth0] [GWinHeight0] [GWinWidth1] [GWinHeight1]\n");
    printf("-----GWinWidth0 : The width of GWin0 to create. it is small than stretch win width\n");
    printf("-----GWinHeight0 : The height of GWin0 to create. it is small than stretch win height\n");
    printf("-----GWinWidth1 : The width of GWin1 to create. it is small than stretch win width\n");
    printf("-----GWinHeight1 : The height of GWin1 to create. it is small than stretch win height\n");
    printf("4.GE_FillBlock [Index] [RectX] [RectY] [RectWidth] [RectHeight]\n");
    printf("-----Index : The color index, range {0..7}\n");
    printf("----------------0: r = 255, g = 0, b = 0, a = 255\n");
    printf("----------------1: r = 0, g = 255, b = 0, a = 255\n");
    printf("----------------2: r = 0, g = 0, b = 255, a = 255\n");
    printf("----------------3: r = 255, g = 255, b = 0, a = 128\n");
    printf("----------------4: r = 0, g = 255, b = 255, a = 128\n");
    printf("----------------5: r = 255, g = 0, b = 255, a = 128\n");
    printf("----------------6: r = 255, g = 255, b = 255, a = 255\n");
    printf("----------------7: r = 0, g = 0, b = 0, a = 255\n");
    printf("-----RectX : The x start point, range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----RectY : The y start point, range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("-----RectWidth : The filling width, (RectX + RectWidth -1) range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----RectHeight : The filling height, (RectY + RectHeight -1) range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("5.GE_DrawLine [index] [X1] [Y1] [X2] [Y2]\n");
    printf("-----index : The color index, range {0..7}\n");
    printf("-------------The same with the color index of GE_FillBlock\n");
    printf("-----X1 : X1, range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Y1 : Y1, range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("-----X2 : X2, range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Y2 : Y2, range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("6.GE_DrawBmp [Index] [X] [Y] [Width] [Height]\n");
    printf("-----Index : The bitmap index, range {0..3}\n");
    printf("----------------0: ARGB8888 format bitmap\n");
    printf("----------------1: RGB565 format bitmap\n");
    printf("----------------2: ARGB4444 format bitmap\n");
    printf("----------------3: I8 format bitmap\n");
    printf("-----X : X, range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Y : Y, range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("-----Width : The drawing width, (X + Width -1) range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Height : The drawing height, (Y + Height -1) range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("7.GE_DrawBmpRotate [X] [Y] [RotateIndex] [MirrorX] [MirrorY]\n");
    printf("-----X : X, range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Y : Y, range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("-----RotateIndex : Rotation Direction\n");
    printf("----------------GEROTATE_0 = 0, GEROTATE_90 = 1, GEROTATE_180 = 2, GEROTATE_270 = 3\n");
    printf("-----MirrorX : Enable mirror by X-axis\n");
    printf("-----MirrorY : Enable mirror by Y-axis\n");
    printf("*****************Press \"HelpGE_More for more Function Info\" *****************\n");
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The sample code to initial GE
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_Help Par1 \n
//------------------------------------------------------------------------------
MS_BOOL GE_Help_Part1(void)
{
    printf("*************************GE More function List*************************\n");
    printf("8.GE_DrawBmpScale [X] [Y] [ScaleX] [ScaleY]\n");
    printf("-----X : X, range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Y : Y, range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("-----ScaleX : X-axis scaling parameter, range {1..3} for this sample code\n");
    printf("-----ScaleY : Y-axis scaling parameter, range {1..3} for this sample code\n");
    printf("9.GE_ConvertColor \n");
    printf("10.GE_ScreenCopyRop [RopIndex] [SrcColorIndex] [DstColorIndex]\n");
    printf("-----RopIndex : Rop operation\n");
    printf("----------------ROP2_OP_ZORE          = 0,  // rop_result = 0;\n");
    printf("----------------ROP2_OP_NOT_PS_OR_PD  = 1,  // rop_result = ~( rop_src | rop_dst );\n");
    printf("----------------ROP2_OP_NS_AND_PD     = 2,  // rop_result = ((~rop_src) & rop_dst);\n");
    printf("----------------ROP2_OP_NS            = 3,  // rop_result = ~(rop_src);\n");
    printf("----------------ROP2_OP_PS_AND_ND     = 4,  // rop_result = (rop_src & (~rop_dst));\n");
    printf("----------------ROP2_OP_ND            = 5,  // rop_result = ~(rop_dst);\n");
    printf("----------------ROP2_OP_PS_XOR_PD     = 6,  // rop_result = ( rop_src ^ rop_dst);\n");
    printf("----------------ROP2_OP_NOT_PS_AND_PD = 7,  // rop_result = ~(rop_src & rop_dst);\n");
    printf("----------------ROP2_OP_PS_AND_PD     = 8,  // rop_result = (rop_src & rop_dst);\n");
    printf("----------------ROP2_OP_NOT_PS_XOR_PD = 9,  // rop_result = ~(rop_dst ^ rop_src);\n");
    printf("----------------ROP2_OP_PD            = 10, // rop_result = rop_dst;\n");
    printf("----------------ROP2_OP_NS_OR_PD      = 11, // rop_result = (rop_dst | (~rop_src));\n");
    printf("----------------ROP2_OP_PS            = 12, // rop_result = rop_src;\n");
    printf("----------------ROP2_OP_PS_OR_ND      = 13, // rop_result = (rop_src | (~rop_dst));\n");
    printf("----------------ROP2_OP_PD_OR_PS      = 14, // rop_result = 0xffffff;\n");
    printf("----------------ROP2_OP_ONE           = 15, // rop_result = (rop_dst | rop_src);\n");
    printf("-----SrcColorIndex : The color index, range {0..7} for this sample code\n");
    printf("-------------The same with the color index of GE_FillBlock\n");
    printf("-----DstColorIndex : The color index, range {0..7} for this sample code\n");
    printf("-------------The same with the color index of GE_FillBlock\n");
    printf("11.GE_BitBltSample [SrcX] [SrcY] [DstX] [DstY] [Width] [Height]\n");
    printf("-----SrcX : The X coordinate in source GE buffer. Range {0..min(GWinWidth0, GWinWidth1)} in this sample code\n");
    printf("-----SrcY : The Y coordinate in source GE buffer. Range {0..min(GWinHeight0, GWinHeight1)} in this sample code\n");
    printf("-----DstX : The X coordinate in destination GE buffer. Range {0..min(GWinWidth0, GWinWidth1)} in this sample code\n");
    printf("-----DstY : The Y coordinate in destination GE buffer. Range {0..min(GWinHeight0, GWinHeight1)} in this sample code\n");
    printf("-----Width : The copy width, (X + Width -1) range {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Height : The copy height, (Y + Height -1) range {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("12.GE_TextOut [FontIndex] [ColorIndex] [FlagIndex] [X] [Y]\n");
    printf("-----FontIndex : font data in this samle code, range {0..2}\n");
    printf("-----ColorIndex : The color index, range {0..7}\n");
    printf("-------------The same with the color index of GE_FillBlock\n");
    printf("-----FlagIndex : font drawing flag, range {0..7}\n");
    printf("-------------GFXFONT_FLAG_SCALE   = 0x1\n");
    printf("-------------GFXFONT_FLAG_GAP  = 0x2\n");
    printf("-------------GFXFONT_FLAG_COMPACT = 0x4\n");
    printf("-----X : X  {0..min(GWinWidth0, GWinWidth1)} for this sample code\n");
    printf("-----Y : Y  {0..min(GWinHeight0, GWinHeight1)} for this sample code\n");
    printf("*************************End of GE function List*************************\n");
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initial GE
/// @param[in] index The GOP output path index
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_Init \n
//------------------------------------------------------------------------------
MS_BOOL GE_Initialize(MS_U32 *index)
{
    printf("Demo_GE_Init\n");

    MS_U16 u16BmpItem = 0, u16FontItem = 0;
    GOP_InitInfo pGopInit;
    GFX_Config tGFXcfg;
    MS_VE_Output_Ctrl OutputCtrl;

    MDrv_SEM_Init();
    //Set getting font and bitmap callback function
    MApi_GFX_RegisterGetFontCB(OSD_RESOURCE_GetFontInfoGFX);
    MApi_GFX_RegisterGetBMPCB(OSD_RESOURCE_GetBitmapInfoGFX);

    /// -Initial GE
    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);
    printf("driver GE init ok\n");

    /// -Initial GOP and Scaler
    ///The sample code to initial Scaler and GOP. For the overall project the Sacler and GOP
    ///will be initialized once somewhere else in the project.
    //Initial Scaler
    _GE_ScalerInit();
    printf("[%s][%d]\n", __FUNCTION__,__LINE__);
    //Initial GOP
    MApi_GOP_RegisterFBFmtCB(( MS_U32(*)(MS_U16 pitch,MS_U32 addr , MS_U16 fmt ))_OSD_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_XC_Sys_PQ_ReduceBW_ForOSD);
    printf("[%s][%d]\n", __FUNCTION__,__LINE__);
    pGopInit.u16PanelWidth = g_IPanel.Width();
    pGopInit.u16PanelHeight = g_IPanel.Height();
    pGopInit.u16PanelHStr = g_IPanel.HStart();


    pGopInit.u32GOPRBAdr = GOP_GWIN_RB_ADR;
    pGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
    pGopInit.u32GOPRegdmaAdr = GOP_REG_DMA_BASE_ADR;
    pGopInit.u32GOPRegdmaLen = GOP_REG_DMA_BASE_LEN;
    //printf("pGopInit.u32GOPRegdmaLen = 0x%lx\n",pGopInit.u32GOPRegdmaLen);
    pGopInit.bEnableVsyncIntFlip = FALSE;
    MApi_GOP_Init(&pGopInit);
    printf("driver GOP init ok\n");
    printf("[%s][%d]\n", __FUNCTION__,__LINE__);
    /*if( *index == 1 || *index == 2)
    {
        printf("Path 1 and 2 is not supported yet, change path to 0\n");
        *index = 0;
    }*/

    /// -To choose GOP path
    //Path 0 only use one GOP, here we use GOP0 to display for HD and SD
    //Path 1 only use one GOP, here we use GOP0 to display for HD and SD
    //Path 2 use two GOPs, here we use GOP0 to display for HD and use GOP1 to display for SD

    if(*index == 0)
    {
        //Note: If GE_Init with *index =0 & with SD video input, VE source will be changed to DI2VE in driver layer.
        // CVBS output won't display OSD.
        printf("GE_Init, Path0\n");
        PathNum = 0;
        GOPCount = 1;
        gWinInfo[0].GopDest = E_GOP_DST_OP0;
        MApi_GOP_GWIN_SwitchGOP(E_GOP);//use GOP 0
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
        MApi_GOP_GWIN_SetGOPDst(E_GOP, gWinInfo[0].GopDest);

        // for CVBS output, use VE
        // To choose video to VE mux
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND2); // set OP can through VE out-put to CVBS

    }else if(*index == 1) // not supported yet
    {
        printf("GE_Init, Path1\n");
        PathNum = 1;
        GOPCount = 1;
        gWinInfo[0].GopDest = E_GOP_DST_MIXER2OP;
        MApi_GOP_GWIN_SwitchGOP(E_GOP);//use GOP 0
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
        MApi_GOP_GWIN_SetGOPDst(E_GOP, gWinInfo[0].GopDest);

        //MApi_GOP_SC_Init();
        MDrv_VE_SetOSD(TRUE);

        // for CVBS output, use VE
        // To choose video to VE mux
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND1); // set OP can through VE out-put to CVBS
    }else if(*index == 2) // not supported yet
    {
        printf("GE_Init, Path2\n");
        PathNum = 2;
        GOPCount = 2;

        ///--------------To HD---------------///
        gWinInfo[0].GopDest = E_GOP_DST_OP0;
        MApi_GOP_GWIN_SwitchGOP(E_GOP);//use GOP 0
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
        MApi_GOP_GWIN_SetGOPDst(E_GOP, gWinInfo[0].GopDest);

        ///---------------To SD---------------///
        gWinInfo[1].GopDest = E_GOP_DST_MIXER2VE;
        MApi_GOP_GWIN_SwitchGOP(E_GOP1);//use GOP 1
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
        MApi_GOP_GWIN_SetGOPDst(E_GOP1, gWinInfo[1].GopDest);

        //MApi_GOP_SC_Init();
        MDrv_VE_SetOSD(TRUE);

        // for CVBS output, use VE
        // To choose video to VE mux
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND1); // set OP can through VE out-put to CVBS
    }

    // set output control
    OutputCtrl.bEnable = TRUE;
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    MDrv_VE_SetOutputCtrl(&OutputCtrl);
    // disable blackScreen
    MDrv_VE_SetBlackScreen(FALSE);
    //add test bitmap here, in this demo code only these four bitmaps
    u16BmpItem |= (ARGB8888BMP |RGB565BMP |ARGB4444BMP| I8BMP);

    //add test font here, in this demo code only these three fonts
    u16FontItem |= (FontI1|FontI2|FontI4);

    _GE_InitGlobalVar();
    _GE_SetBmpFontAdr();
    _LoadBitmap(u16BmpItem);
    _LoadFont(u16FontItem);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set configurations including stretch win size
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_SetConfig \n
//------------------------------------------------------------------------------
MS_BOOL GE_SetConfig(void)
{
    GOP_MIXER_TIMINGTYPE gopMixerTimingType;
    GOP_MixerTiming gopMixerTiming;
    GOP_Mixer2OPTiming gopMixer2OPTiming;
    VE_DrvStatus DrvStatus;
    MS_VE_DST_DispInfo DrvVEDispInfo;
    MS_U16 GOPSC_width;
    MS_BOOL VE_PAL;

    if(PathNum == 0 )
    {
        printf("GE_SetConfig, Path0\n");

        //Set stretch window every re-initial
        //Set GOP destination to OP
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP,gWinInfo[0].GopDest,0,0,g_IPanel.Width(),g_IPanel.Height());
        printf("Set GOP %d StretchWin: (%d,%d,%d,%d)\n", E_GOP, 0, 0, g_IPanel.Width(), g_IPanel.Height());

        // Every time change resolution, the HStart should be set again
        printf("Panel HStart = %d\n",g_IPanel.HStart());
        MApi_GOP_SetGOPHStart(E_GOP,g_IPanel.HStart());

        MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);//FIXME

    }else if(PathNum == 1 ) // not supported yet
    {
        printf("GE_SetConfig, Path1\n");

        //Set stretch window every re-initial
        //Set GOP destination to OP
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP,gWinInfo[0].GopDest,0,0,g_IPanel.Width(),g_IPanel.Height());
        printf("Set GOP %d StretchWin: (%d,%d,%d,%d)\n", E_GOP, 0, 0, g_IPanel.Width(), g_IPanel.Height());

        MDrv_VE_GetStatus(&DrvStatus);
        if(DrvStatus.VideoSystem <= 2)
        {
            printf("Set NTSC\n");
        }else
        {
            printf("Set PAL\n");
        }



        gopMixer2OPTiming.hstart = g_IPanel.HStart();
        gopMixer2OPTiming.hend = g_IPanel.HStart() + g_IPanel.Width();
        gopMixer2OPTiming.vstart = g_IPanel.VStart();
        gopMixer2OPTiming.vend = g_IPanel.VStart() + g_IPanel.Height();
        gopMixer2OPTiming.htotal = g_IPanel.HTotal() - 1;
        gopMixer2OPTiming.hsyncdelay = g_IPanel.HStart() + 31 - (g_IPanel.HSynWidth() - 1);

        MApi_GOP_MIXER_SetMIXER2OPOutputTiming(&gopMixer2OPTiming);
        printf("GOP Mixer timing : (HStart %ld, HEnd %ld, VStart %ld, VEnd %ld, HTotal %ld, HSyncDelay %ld)\n",
               gopMixer2OPTiming.hstart, gopMixer2OPTiming.hend, gopMixer2OPTiming.vstart,
               gopMixer2OPTiming.vend, gopMixer2OPTiming.htotal, gopMixer2OPTiming.hsyncdelay);

        //MApi_GOP_SC_SetFPLL_Enable(TRUE);

    }
    else if(PathNum == 2 ) // not supported yet
    {
        printf("GE_SetConfig, Path2\n");
        ///--------------To HD---------------///
        MApi_GOP_GWIN_SwitchGOP(E_GOP);
        //Set stretch window every re-initial
        //Set GOP destination to OP
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP,gWinInfo[0].GopDest,0,0,g_IPanel.Width(),g_IPanel.Height());

        // Every time change resolution, the HStart should be set again
        printf("Panel HStart = %d\n",g_IPanel.HStart());
        MApi_GOP_SetGOPHStart(E_GOP,g_IPanel.HStart());

        MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);//FIXME

        ///---------------To SD---------------///
        MApi_GOP_GWIN_SwitchGOP(E_GOP1);
        MDrv_VE_GetStatus(&DrvStatus);
        if(DrvStatus.VideoSystem <= 2)
        {
            printf("Set NTSC\n");
            VE_PAL = false;
            GOPSC_width = 480;
            gopMixerTimingType = GOP_NTSC;
        }else
        {
            printf("Set PAL\n");
            VE_PAL = true;
            GOPSC_width = 576;
            gopMixerTimingType = GOP_PAL;
        }

        MApi_VE_GetDstInfo(&DrvVEDispInfo, sizeof(DrvVEDispInfo));

        MApi_GOP_MIXER_SetOutputTiming(gopMixerTimingType, &gopMixerTiming);

        //Set stretch window every re-initial
        //Set GOP destination to OP
        if(!VE_PAL)
        {
            MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP1,gWinInfo[1].GopDest, 0, 0, 720, 480);
        }else
        {
            MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP1,gWinInfo[1].GopDest, 0, 0, 720, 576);
        }
        MApi_GOP_GWIN_EnableProgressive(TRUE); //FIXME

    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The PreSetting Color Format for GE demo
/// @param[in] GWin index
/// @param[in] Color Format
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_SetColorFmt \n
//------------------------------------------------------------------------------
MS_BOOL GE_SetColorFmt(MS_U32 *GWinIndex,MS_U32 *pColorFormat)
{
    if((GWinIndex == NULL) || (pColorFormat == NULL))
    {
        printf("Param 0 is GWIN index \n");
        printf("Param 1 is Color Format index \n");
        printf("Color Format 0: ARGB4444\n");
        printf("Color Format 1: ARGB8888\n");
        printf("Color Format 2: RGB565\n");
        printf("Color Format 3: I8\n");
        return FALSE;
    }

    switch (*pColorFormat)
    {
        case 0:
            gwin_fmt = E_MS_FMT_ARGB4444;
            gWinInfo[*GWinIndex].GeDstFmt = E_MS_FMT_ARGB4444;

            break;
        case 1:
            gwin_fmt = E_MS_FMT_ARGB8888;
            gWinInfo[*GWinIndex].GeDstFmt = E_MS_FMT_ARGB8888;
            break;
        case 2:
            gwin_fmt = E_MS_FMT_RGB565;
            gWinInfo[*GWinIndex].GeDstFmt = E_MS_FMT_RGB565;
            break;
        case 3:
            gwin_fmt = E_MS_FMT_I8;
            gWinInfo[*GWinIndex].GeDstFmt = E_MS_FMT_I8;
            break;
        default:
            gwin_fmt = E_MS_FMT_ARGB4444;
            gWinInfo[*GWinIndex].GeDstFmt = E_MS_FMT_ARGB4444;
            break;

    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to create GWin
/// @param[in] GWinWidth0 The width of GWin0 to create. it is small than stretch win width
/// @param[in] GWinHeight0 The height of GWin0 to create. it is small than stretch win height
/// @param[in] GWinWidth1 The width of GWin1 to create. it is small than stretch win width
/// @param[in] GWinHeight1 The height of GWin1 to create. it is small than stretch win height
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_CreateWin \n
//------------------------------------------------------------------------------
MS_BOOL GE_CreateWin(MS_U32 *GWinWidth0, MS_U32 *GWinHeight0, MS_U32 *GWinWidth1, MS_U32 *GWinHeight1)
{
    MS_U8 u8GOP[2];
    MS_U8 u8GWin[2];
    MS_U8 u8FB[2];
    MS_U16 FBWidth[2];
    MS_U16 FBHeight[2];
    MS_U16 FBFmt[2];
    int i;
    GOP_PaletteEntry _gopI8Palette[GOP_PALETTE_ENTRY_NUM];



    for(i = 0; i < GOPCount; i++)
    {
        if( gWinInfo[i].GeWinId!= 0xFF)
        {
            printf("Delete win %d\n", gWinInfo[i].GeWinId);
            MApi_GOP_GWIN_DeleteWin( gWinInfo[i].GeWinId );
        }
    }

    if(PathNum == 0  || PathNum == 1 )
    {
        printf("GE_CreateWin, Path0 and 1 settings\n");
        u8GOP[0] = E_GOP;
        u8FB[0] = gWinInfo[0].u8FBId;
        FBWidth[0] = gWinInfo[0].GopBitmapWidth = *GWinWidth0;
        FBHeight[0] = gWinInfo[0].GopBitmapHeight = *GWinHeight0;
        FBFmt[0] = gWinInfo[0].GeDstFmt = gwin_fmt;
    }else if(PathNum == 2 )
    {
        printf("GE_CreateWin, Path2 settings\n");
        u8GOP[0] = E_GOP;
        u8FB[0] = gWinInfo[0].u8FBId;
        FBWidth[0] = gWinInfo[0].GopBitmapWidth = *GWinWidth0;
        FBHeight[0] = gWinInfo[0].GopBitmapHeight = *GWinHeight0;
        FBFmt[0] = gWinInfo[0].GeDstFmt = gwin_fmt;

        u8GOP[1] = E_GOP1;
        u8FB[1] = gWinInfo[1].u8FBId;
        FBWidth[1] = gWinInfo[1].GopBitmapWidth = *GWinWidth1;
        FBHeight[1] = gWinInfo[1].GopBitmapHeight = *GWinHeight1;
        FBFmt[1] = gWinInfo[1].GeDstFmt = gwin_fmt;
    }

    if (gWinInfo[0].GeDstFmt == E_MS_FMT_I8)
    {
        for (i=0; i<GOP_PALETTE_ENTRY_NUM; i++)
        {
            _gopI8Palette[i].RGB.u8A =  _gopI8PaletteEntry[i].RGB.u8A;
            _gopI8Palette[i].RGB.u8R =  _gopI8PaletteEntry[i].RGB.u8R;
            _gopI8Palette[i].RGB.u8G =  _gopI8PaletteEntry[i].RGB.u8G;
            _gopI8Palette[i].RGB.u8B =  _gopI8PaletteEntry[i].RGB.u8B;
            //printf("A:%d, R: %d, G: %d, B: %d\t", _gopI8Palette[i].RGB.u8A, _gopI8Palette[i].RGB.u8R, _gopI8Palette[i].RGB.u8G, _gopI8Palette[i].RGB.u8B);
        }
        MApi_GOP_GWIN_SwitchGOP(0);
        MApi_GOP_GWIN_SetPaletteOpt(_gopI8Palette, 0, GOP_PALETTE_ENTRY_NUM - 1, E_GOP_PAL_ARGB8888);
    }
    
    for(i = 0; i < GOPCount; i++)
    {
        printf("GE_CreateWin, Create GWin count = %d\n", i);

        if( 0 != _Create_FB(&u8FB[i], FBWidth[i], FBHeight[i], FBFmt[i]))
        {
             printf("Create FB fail\n");
             return FALSE;
        }
        gWinInfo[i].u8FBId = u8FB[i];

        u8GWin[i] = gWinInfo[i].GeWinId;

        /// -Create GWin
        if( 0 != _Create_GWin(u8GOP[i], &u8GWin[i], u8FB[i], FBFmt[i] ))
        {
             printf("Create GWin fail, GOPnum = %d, u8GWin = %d, FBId = %d, FBFmt = %d\n", u8GOP[i],  u8GWin[i], u8FB[i], FBFmt[i]);
             return FALSE;
        }

        gWinInfo[i].GeWinId = u8GWin[i];
        printf("Create GWin ok, GOPnum = %d, u8GWin = %d, FBId = %d, FBFmt = %d\n", u8GOP[i],  u8GWin[i], u8FB[i], FBFmt[i]);
    }

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to fill rectangle in the image buffer for a GWin
/// @param[in] index The color index
/// @param[in] RectX The x start point
/// @param[in] RectY The y start point
/// @param[in] RectWidth The filling width
/// @param[in] RectHeight The filling height
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_FillBlock \n
//------------------------------------------------------------------------------
MS_BOOL GE_FillBlock(MS_U32 *index, MS_U32 *RectX, MS_U32 *RectY, MS_U32 *RectWidth, MS_U32 *RectHeight)
{
    GFX_RectFillInfo gfxFillBlock;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    GFX_BufferInfo gfxDstBuf;
    GOP_GwinFBAttr DstFBInfo;
    GFX_RgbColor color_s;
    GFX_RgbColor color_e;
    MS_U8 DstFBfmt;
    MS_U32 u32Flag;
    int i;

    //Map color index to ARGB color
    _ColorMapping(*index, &color_s);
    _ColorMapping(*index, &color_e);

    for(i = 0; i < GOPCount; i++)
    {
        //get current GWIN fbInfo.
        //MApi_GOP_GWIN_GetFBInfo(MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &DstFBInfo);
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[i].u8FBId, &DstFBInfo);

        DstFBfmt = (MS_U8)DstFBInfo.fbFmt;

        //Set Dst buffer
        gfxDstBuf.u32ColorFmt = DstFBfmt;
        gfxDstBuf.u32Addr = DstFBInfo.addr;
        gfxDstBuf.u32Pitch = DstFBInfo.pitch;
        MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0);
        printf("Dst buffer addr = 0x%lx\n", DstFBInfo.addr);

        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        //Set Clip window for rendering
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        u32Flag = GFXRECT_FLAG_COLOR_CONSTANT;

        _SetupGfxRectFillInfo(&gfxFillBlock, *RectX, *RectY, *RectWidth, *RectHeight,
                                    DstFBfmt, color_s, color_e, u32Flag);

        if (gWinInfo[0].GeDstFmt == E_MS_FMT_I8)
        {
            gfxFillBlock.colorRange.color_s.b = *index;
            printf("I8 Color Index %d \n",gfxFillBlock.colorRange.color_s.b);
        }

        MApi_GFX_RectFill(&gfxFillBlock);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw a line in the image buffer for a GWin
/// @param[in] index The color index
/// @param[in] X1 The X1 coordinate
/// @param[in] Y1 The Y1 coordinate
/// @param[in] X2 The X2 coordinate
/// @param[in] Y2 The Y2 coordinate
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawLine \n
//------------------------------------------------------------------------------
MS_BOOL GE_DrawLine(MS_U32 *index, MS_U32 *X1, MS_U32 *Y1, MS_U32 *X2, MS_U32 *Y2)
{
    GFX_DrawLineInfo gfxDrawLineInfo;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    GFX_BufferInfo gfxDstBuf;
    GOP_GwinFBAttr DstFBInfo;
    GFX_RgbColor color_s;
    GFX_RgbColor color_e;
    MS_U8 DstFBfmt;
    MS_U32 width; //line width
    MS_U32 u32Flag;
    int i;

    //Map color index to ARGB color
    _ColorMapping(*index, &color_s);
    _ColorMapping(*index, &color_e);

    for(i = 0; i < GOPCount; i++)
    {
        //get current GWIN fbInfo.
        //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &DstFBInfo);
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[i].u8FBId, &DstFBInfo);

        DstFBfmt = (MS_U8)DstFBInfo.fbFmt;

        //Set Dst buffer
        gfxDstBuf.u32ColorFmt = DstFBfmt;
        gfxDstBuf.u32Addr = DstFBInfo.addr;
        gfxDstBuf.u32Pitch = DstFBInfo.pitch;
        MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0);
        printf("Dst buffer addr = 0x%lx\n", DstFBInfo.addr);

        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        //Set Clip window for rendering
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        // Draw Line parameters
        width = 5;
        u32Flag = GFXLINE_FLAG_COLOR_CONSTANT;

        MApi_GFX_Set_Line_Pattern(false, 0xAA, 2);

        _SetupGfxDrawLineInfo(&gfxDrawLineInfo, X1, Y1, X2, Y2,
                                        DstFBfmt, color_s, color_e, width, u32Flag);

        if (gWinInfo[0].GeDstFmt == E_MS_FMT_I8)
        {
            gfxDrawLineInfo.colorRange.color_s.b = *index;
            printf("I8 Color Index %d \n",gfxDrawLineInfo.colorRange.color_s.b);
        }
        if( GFX_SUCCESS != MApi_GFX_DrawLine(&gfxDrawLineInfo))
        {
            printf("MApi_GFX_DrawLine fail\n");
            return false;
        }
        MApi_GFX_FlushQueue();
    }

    return true;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw bitmap or fill rectangle in the image buffer for this GWin
/// @param[in] Index The bitmap index
/// @param[in] X The x start point
/// @param[in] Y The y start point
/// @param[in] width The drawing width
/// @param[in] height The drawing height
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawBmp \n
//------------------------------------------------------------------------------
MS_BOOL GE_DrawBmp(MS_U32 *Index, MS_U32 *X, MS_U32 *Y, MS_U32 *width, MS_U32 *height)
{
    GFX_DrawBmpInfo gfxBmpFmt;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    /*load palette to GE if source format is index mode
     I8 mode -> RGB/YUV (O)*/
    if(bmpinfo[*Index].BmpClrFmt == GFX_FMT_I8)
    {
        _SetGfxPalette();
    }

    for(i = 0; i < GOPCount; i++)
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        //Switch destination GWIn to GeWinId
        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        gfxBmpFmt.bScale = true;
        gfxBmpFmt.x = *X;
        gfxBmpFmt.y = *Y;
        gfxBmpFmt.width = *width;
        gfxBmpFmt.height = *height;
        MApi_GFX_DrawBitmap(bmpinfo[*Index].hBmp, &gfxBmpFmt);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use GOP to display JPD decode result in appDemo_JPEG.c
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawJpeg \n
//------------------------------------------------------------------------------
MS_BOOL GE_DrawJpeg(void)
{
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;

    GFX_BufferInfo srcBufInfo;
    GFX_DrawRect drawBuf;
    MS_U32 u32Status;

    gfxPt0.x = 0;
    gfxPt0.y = 0;
    gfxPt1.x = gWinInfo[0].GopBitmapWidth;
    gfxPt1.y = gWinInfo[0].GopBitmapHeight;
    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    //Switch destination GWIn to GeWinId
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[0].u8FBId);

    memcpy((void*)MS_PA2KSEG1(u32BmpUV8Y8Addr), (void*)MS_PA2KSEG1(JPD_OUT_ADR), MApi_JPEG_GetWidth() * MApi_JPEG_GetHeight() * 2ul);
    printf("u32BmpUV8Y8Addr= 0x%lx\n", (MS_U32)u32BmpUV8Y8Addr);
    printf("BMPUV8Y8SIZE = %lu\n", MApi_JPEG_GetWidth() * MApi_JPEG_GetHeight() * 2ul);


    srcBufInfo.u32Addr = u32BmpUV8Y8Addr;
    srcBufInfo.u32Width = MApi_JPEG_GetWidth();
    srcBufInfo.u32Height = MApi_JPEG_GetHeight();
    srcBufInfo.u32Pitch = MApi_JPEG_GetWidth() << 1;
    srcBufInfo.u32ColorFmt = GFX_FMT_YUV422;

    u32Status = MApi_GFX_SetSrcBufferInfo(&srcBufInfo, 0);
    printf("%d %s MApi_GFX_SetSrcBufferInfo = %lu\n", __LINE__, __FUNCTION__, u32Status);

    drawBuf.srcblk.x = 0;
    drawBuf.srcblk.y = 0;
    drawBuf.srcblk.width = MApi_JPEG_GetWidth();
    drawBuf.srcblk.height = MApi_JPEG_GetHeight();

    drawBuf.dstblk.x = 0;
    drawBuf.dstblk.y = 0;
    drawBuf.dstblk.width = MApi_JPEG_GetWidth();
    drawBuf.dstblk.height = MApi_JPEG_GetHeight();



    printf("[%s]Blitter from (x,y,w,h) = (%d,%d,%d,%d) to (x,y,w,h) = (%d,%d,%d,%d)\n", __FUNCTION__,

    0, 0, MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(),

    0, 0, drawBuf.dstblk.width, drawBuf.dstblk.height);

    //MApi_GFX_BitBlt(&drawBuf, GFXDRAW_FLAG_DEFAULT);

    MApi_GFX_SetDC_CSC_FMT(0, 0, GFX_YUV_IN_255, GFX_YUV_YUYV, GFX_YUV_YUYV);

    u32Status = MApi_GFX_BitBlt(&drawBuf, GFXDRAW_FLAG_SCALE);
    printf("%d %s MApi_GFX_BitBlt = %lu\n", __LINE__, __FUNCTION__, u32Status);
//    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw a bitmap (400x400)with rotation and mirror option
/// @param[in] X The X coordinate
/// @param[in] Y The Y coordinate
/// @param[in] RotateIndex The rotation type
/// @param[in] MirrorX Enable mirror by X-axis
/// @param[in] MirrorY Enable mirror by Y-axis
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawBmpRotate \n
//------------------------------------------------------------------------------
MS_BOOL GE_DrawBmpRotate(MS_U32 *X, MS_U32 *Y, MS_U32 *RotateIndex, MS_U32 *MirrorX, MS_U32 *MirrorY)
{
    GFX_DrawBmpInfo gfxBmpFmt;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    for( i = 0; i < GOPCount; i++ )
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        gfxBmpFmt.bScale = FALSE;
        gfxBmpFmt.x = *X;
        gfxBmpFmt.y = *Y;
        gfxBmpFmt.width = 400;
        gfxBmpFmt.height = 400;
        MApi_GFX_SetMirror(*MirrorX, *MirrorY);// ymirror is not work???
        MApi_GFX_SetRotate(*RotateIndex);
        MApi_GFX_DrawBitmap(hBmpARGB4444, &gfxBmpFmt);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw a bitmap (400x400)with scaling option.
/// @param[in] X The X coordinate
/// @param[in] Y The Y coordinate
/// @param[in] ScaleX Scaling parameter (X-axis)
/// @param[in] ScaleY Scaling parameter (Y-axis)
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawBmpScale \n
//------------------------------------------------------------------------------
MS_BOOL GE_DrawBmpScale(MS_U32 *X, MS_U32 *Y, MS_U32 *ScaleX, MS_U32 *ScaleY)
{
    GFX_DrawBmpInfo gfxBmpFmt;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    for( i = 0; i < GOPCount; i++ )
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        if (*ScaleX > 3) *ScaleX = 3;
        if (*ScaleY > 3) *ScaleY = 3;

        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        gfxBmpFmt.bScale = TRUE;
        gfxBmpFmt.x = *X;
        gfxBmpFmt.y = *Y;
        gfxBmpFmt.width = 400*(*ScaleX);
        gfxBmpFmt.height = 400*(*ScaleY);

        MApi_GFX_DrawBitmap(hBmpARGB4444, &gfxBmpFmt);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to convert a RGB565 color format to ARGB4444. We use GWIN 0
//  to see the result
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_ConvertColor \n
//------------------------------------------------------------------------------
MS_BOOL GE_ConvertColor(void)
{
    GFX_DrawBmpInfo gfxBmpFmt;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    for( i = 0; i < GOPCount; i++ )
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        //Destination Buffer
        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        gfxBmpFmt.bScale = FALSE;
        gfxBmpFmt.x = 0;
        gfxBmpFmt.y = 0;
        gfxBmpFmt.width = 400;
        gfxBmpFmt.height = 400;

        //Convert RGB565 to a ARGB4444 buffer
        MApi_GFX_DrawBitmap(hBmpRGB565, &gfxBmpFmt);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to copy a image (180x288) from screen to screen
/// @param[in] RopIndex The ROP option
/// @param[in] SrcColorIndex The source rectangle color index
/// @param[in] DstColorIndex The destination X coordinate
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_ScreenCopyRop \n
//------------------------------------------------------------------------------
MS_BOOL GE_ScreenCopyRop(MS_U32 *RopIndex, MS_U32 *SrcColorIndex, MS_U32 *DstColorIndex)
{
    GFX_DrawRect gfxDrawRect;
    GFX_RectFillInfo gfxFillBlock;
    GOP_GwinFBAttr SrcFBInfo;
    GFX_BufferInfo srcbuf;
    GFX_RgbColor color_s;
    GFX_RgbColor color_e;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    for( i = 0; i < GOPCount; i++ )
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        //Destination Buffer
        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        //Set Src buffer
        //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &SrcFBInfo);
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[i].u8FBId, &SrcFBInfo);

        srcbuf.u32ColorFmt = gwin_fmt;
        srcbuf.u32Addr = SrcFBInfo.addr;
        srcbuf.u32Width = SrcFBInfo.width;
        srcbuf.u32Height = SrcFBInfo.height;
        srcbuf.u32Pitch = SrcFBInfo.pitch;
        MApi_GFX_SetSrcBufferInfo(&srcbuf, 0);

        _ColorMapping(*SrcColorIndex, &color_s);
        _ColorMapping(*SrcColorIndex, &color_e);

        //Fill source rectangle with color SrcColorIndex
        _SetupGfxRectFillInfo(&gfxFillBlock, 0, 0, 360, 576, gwin_fmt, color_s, color_e, GFXRECT_FLAG_COLOR_CONSTANT);
        MApi_GFX_RectFill(&gfxFillBlock);

        _ColorMapping(*DstColorIndex, &color_s);
        _ColorMapping(*DstColorIndex, &color_e);

        //Fill destination rectangle with color DstColorIndex
        _SetupGfxRectFillInfo(&gfxFillBlock, 360, 0, 360, 576, gwin_fmt, color_s, color_e, GFXRECT_FLAG_COLOR_CONSTANT);
        MApi_GFX_RectFill(&gfxFillBlock);

        MsOS_DelayTask(1000);

        MApi_GFX_EnableAlphaBlending(FALSE);
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);

        //run ROP
        _ROPMapping(*RopIndex, &gfxDrawRect);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code BitBlt a bitmap in GE Buffer to another GE Buffer
///        GE buffer could be different or the same
/// @param[in] SrcX The X coordinate in source GE buffer. Range {0..1280} in this sample code
/// @param[in] SrcY The Y coordinate in source GE buffer. Range {0..720} in this sample code
/// @param[in] DstX The X coordinate in destination GE buffer. Range {0..1280} in this sample code
/// @param[in] DstY The Y coordinate in destination GE buffer. Range {0..720} in this sample code
/// @param[in] Width The copy width
/// @param[in] Height The copy height
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_BitBltSample \n
//------------------------------------------------------------------------------
MS_BOOL GE_BitBltSample(MS_U32 *SrcX, MS_U32 *SrcY, MS_U32 *DstX, MS_U32 *DstY, MS_U32 *Width, MS_U32 *Height)
{
    GFX_DrawRect gfxDrawRect;
    GOP_GwinFBAttr SrcFBInfo;
    GFX_BufferInfo srcbuf;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    for( i = 0; i < GOPCount; i++ )
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        //Destination Buffer
        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        //Set Src buffer
        //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &SrcFBInfo);
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[i].u8FBId, &SrcFBInfo);

        srcbuf.u32ColorFmt = gwin_fmt;
        srcbuf.u32Addr = SrcFBInfo.addr;
        srcbuf.u32Width = SrcFBInfo.width;
        srcbuf.u32Height = SrcFBInfo.height;
        srcbuf.u32Pitch = SrcFBInfo.pitch;
        MApi_GFX_SetSrcBufferInfo(&srcbuf, 0);

        ///Config the src and dst clip in GE buffer
        printf("width = %lu, height = %lu\n", *Width, *Height);
        _SetupGfxDrawRect(&gfxDrawRect, *SrcX, *SrcY, *Width, *Height, *DstX, *DstY, *Width, *Height);

        /// BitBlt
        MApi_GFX_BitBlt(&gfxDrawRect, GFXDRAW_FLAG_DEFAULT);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to output bitmap font by GE
/// @param[in] FontIndex The font index in the sample code. We have a bitmap font data in the HEX
///            array for this sample code
/// @param[in] ColorIndex The color index
/// @param[in] FlagIndex The showing flag
/// @param[in] X The X coordinate to output font
/// @param[in] Y The Y coordinate to output font
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_TextOut \n
//------------------------------------------------------------------------------
MS_BOOL GE_TextOut(MS_U32 *FontIndex, MS_U32 *ColorIndex, MS_U32 *FlagIndex, MS_U32 *X, MS_U32 *Y)
{
    MS_U8 u8Font[16];
    MS_U16 u16FontIdx;
    GFX_TextOutInfo gfxTextOutInfo;
    GFX_RgbColor color;
    MS_U32 u32Flag = GFXFONT_FLAG_DEFAULT;
    FONTHANDLE fonthandle;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i;

    switch(*FontIndex)
    {
        case 0:
            fonthandle = hFontI1;
            break;
        case 1:
            fonthandle = hFontI2;
            break;
        case 2:
            fonthandle = hFontI4;
            break;
        default:
            fonthandle = hFontI1;
            break;
    }

    // GEFONT_FLAG_DEFAULT: Default font format
    // GEFONT_FLAG_SCALE: scalable font
    // GEFONT_FLAG_COMPACT: Compact font, controled by "dis"
    // GEFONT_FLAG_GAP: Gap font, controled by "dis"
    if( (*FlagIndex) & 0x1 )
        u32Flag |= GFXFONT_FLAG_SCALE;
    if( (*FlagIndex) & 0x2 )
        u32Flag |= GFXFONT_FLAG_GAP;
    else if( (*FlagIndex) & 0x4 )
        u32Flag |= GFXFONT_FLAG_COMPACT;

    _ColorMapping(*ColorIndex, &color);

    //Generate a 2-byte sting in the sting array. The sting is 123456789ABCDEF in the sample code font system
    for( u16FontIdx = 0; u16FontIdx < 15; u16FontIdx++ )
    {
        u8Font[u16FontIdx] = u16FontIdx+1;
    }
    u8Font[u16FontIdx] = 0x0;   //stop draw command, should set it to "0"

    for( i = 0; i < GOPCount; i++ )
    {
        gfxPt0.x = 0;
        gfxPt0.y = 0;
        gfxPt1.x = gWinInfo[i].GopBitmapWidth;
        gfxPt1.y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[i].u8FBId);

        _SetupGfxTextOutInfo(&gfxTextOutInfo, *X, *Y, FontWidth, FontHeight, FontDis, color, u32Flag);

        printf("\n Test Font: FontIdx=%lu, Handle=%d, x=%d, y=%d \n", *FontIndex, fonthandle , gfxTextOutInfo.dstblk.x, gfxTextOutInfo.dstblk.y);

        MApi_GFX_TextOut( fonthandle , u8Font, 1, &gfxTextOutInfo);
    }

    return TRUE;
}

#ifdef ENABLE_MM_PACKAGE
//------------------------------------------------------------------------------
/// @brief The sample code to clear(free) MM-photo display
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_ClearPhoto \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_MM_ClearPhoto(void)
{
    GOP_GwinFBAttr DstFBInfo;
    int i;
    if (bPhotoDispDone==TRUE)
    {
        if(MApi_GOP_GWIN_GetCurrentFBID()!=0xFF)
        {
            MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetCurrentFBID(), &DstFBInfo);
            if(gwin_fmt==(MS_U16)E_MS_FMT_YUV422)
                MApi_GFX_ClearFrameBufferByWord(DstFBInfo.addr, DstFBInfo.size, 0x80108010);
            else
                MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0x0);

            MApi_GFX_FlushQueue();
            bPhotoDispDone=FALSE;
        }
    }

    for( i = 0; i < GOPCount; i++)
    {
        if( gWinInfo[i].GeWinId!= 0xFF)
        {
            printf("Delete win %d\n", gWinInfo[i].GeWinId);
            MApi_GOP_GWIN_DeleteWin( gWinInfo[i].GeWinId );
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to set gwin-buffer format used in MM-photo display
/// @param[in] pfmt  Format value
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_SetFBFmt \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_MM_Photo_SetGwinFmt(MS_U16 * pfmt)
{
    gwin_fmt=*pfmt;
    printf("gwin_fmt [%d] \n",gwin_fmt);
    return true;
}
//------------------------------------------------------------------------------
/// @brief The sample code to use GOP to display MM-photo on screen
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawPhoto \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_DrawPhoto(void)
{
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;

    GFX_BufferInfo srcBufInfo;
    GFX_DrawRect drawBuf;
    MS_U32 u32Status;
    MS_U16 u16Shift=1;
    MS_U32 u32ColorFmt=GFX_FMT_YUV422;
    MS_U32 u32AlignByte=1;
    MS_U32 u32AlignWidth=1;

    memset(&srcBufInfo,0,sizeof(GFX_BufferInfo) );
    memset(&drawBuf,0,sizeof(GFX_DrawRect) );

    gfxPt0.x = 0;
    gfxPt0.y = 0;
    gfxPt1.x = gWinInfo[0].GopBitmapWidth;
    gfxPt1.y = gWinInfo[0].GopBitmapHeight;


    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    //Switch destination GWIn to GeWinId
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[0].u8FBId);

    MS_U32 u32Width = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, (unsigned int)MPLAYER_INFO_PHOTO_DECODED_WIDTH);
    MS_U32 u32Height = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, (unsigned int)MPLAYER_INFO_PHOTO_DECODED_HEIGHT);
    MS_U32 u32Pitch = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, (unsigned int)MPLAYER_INFO_PHOTO_DECODED_PITCH);
    MS_U32 u32Format=MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, (unsigned int)MPLAYER_INFO_PHOTO_DECODE_FORMAT);
    //U16 u16Width = msAPI_MM_GetOption((int)MM_OPTION_GET_PHOTO_INFO, (int)MM_INFO_PHOTO_DECODED_WIDTH);
    //U16 u16Height = msAPI_MM_GetOption((int)MM_OPTION_GET_PHOTO_INFO, (int)MM_INFO_PHOTO_DECODED_HEIGHT);
    //MApi_MPlayer_GetPhotoData(E_MPLAYER_MEDIA_PHOTO,   &stPhotoRequest,   &stPhotoInfo)

    switch(u32Format)
    {

        case E_PHOTO_OUTPUT_YUV422:
            u16Shift=2;
            u32ColorFmt=GFX_FMT_YUV422;
            printf("GFX_FMT_YUV422\n");
            break;
        case E_PHOTO_OUTPUT_ARGB8888:
            u16Shift=4;
            printf("E_PHOTO_OUTPUT_ARGB8888\n");
            u32ColorFmt=GFX_FMT_ARGB8888;
            break;
        case E_PHOTO_OUTPUT_ARGB1555:
            printf("E_PHOTO_OUTPUT_ARGB1555\n");
            u32ColorFmt=GFX_FMT_ARGB1555;
            break;
        default:
            return FALSE;
            break;
    }
    u32AlignWidth=(u32Width+u32AlignByte-1)/u32AlignByte*u32AlignByte;
    printf("OuptAddr=%lX\n",appDemo_MM_GetOutAdr());
    printf("BMPUV8Y8SIZE = %lu\n", u32Width* u32Height* u16Shift);
    printf("u16Width=0x%lx  u32AlignWidth=0x%lx\n",u32Width,u32AlignWidth);
    printf("u16Height=0x%lx \n",u32Height);
    printf("u32Pitch=%ld\n",u32Pitch *u16Shift);
    printf("u32Format=%lu\n",u32Format);
    printf("u16Shift=%d\n",u16Shift);
    printf("u32AlignByte=%ld\n",u32AlignByte);


    srcBufInfo.u32Addr = appDemo_MM_GetOutAdr();
    srcBufInfo.u32Width = u32Width;
    srcBufInfo.u32Height = u32Height;
    srcBufInfo.u32Pitch = u32Pitch *u16Shift;
    srcBufInfo.u32ColorFmt = u32ColorFmt;

    u32Status = MApi_GFX_SetSrcBufferInfo(&srcBufInfo, 0);
    printf("%d %s MApi_GFX_SetSrcBufferInfo = %lu\n", __LINE__, __FUNCTION__, u32Status);

    drawBuf.srcblk.x = 0;
    drawBuf.srcblk.y = 0;
    drawBuf.srcblk.width = u32AlignWidth;
    drawBuf.srcblk.height = u32Height;

    U32 u32PhotoWidth= u32Width;
    U32 u32PhotoHeight= u32Height;
    U32 u32GWinWidth= (U32)gWinInfo[0].GopBitmapWidth;
    U32 u32GWinHeight= (U32)gWinInfo[0].GopBitmapHeight;

    if ((u32GWinWidth*u32PhotoHeight) <= (u32GWinHeight*u32PhotoWidth ))
    {
        //Width touch boundary
        drawBuf.dstblk.width = gWinInfo[0].GopBitmapWidth;
        drawBuf.dstblk.x = 0;

        //Get new Hight.
        drawBuf.dstblk.height = (U16)((u32PhotoHeight * u32GWinWidth)/u32PhotoWidth);
        drawBuf.dstblk.height = ((drawBuf.dstblk.height + 1) >> 1) << 1;

        drawBuf.dstblk.y = ((gWinInfo[0].GopBitmapHeight - drawBuf.dstblk.height) / 2);
        drawBuf.dstblk.y = (drawBuf.dstblk.y >> 1) << 1;
    }
    else
    {
        //Height touch boundary
        drawBuf.dstblk.height = gWinInfo[0].GopBitmapHeight;
        drawBuf.dstblk.y = 0;

        //Get new width
        drawBuf.dstblk.width = (U16)((u32PhotoWidth * u32GWinHeight)/u32PhotoHeight);
        drawBuf.dstblk.width = ((drawBuf.dstblk.width + 1) >> 1) << 1;

        drawBuf.dstblk.x = ((gWinInfo[0].GopBitmapWidth - drawBuf.dstblk.width) / 2);
        drawBuf.dstblk.x = (drawBuf.dstblk.x >> 1) << 1;
    }

    printf("[%s]Blitter from (x,y,w,h) = (%d,%d,%ld,%ld) to (x,y,w,h) = (%d,%d,%d,%d)\n", __FUNCTION__,
    0, 0, u32Width, u32Height,
    drawBuf.dstblk.x, drawBuf.dstblk.y, drawBuf.dstblk.width, drawBuf.dstblk.height);


    if(u32ColorFmt==GFX_FMT_YUV422)
    {
        printf("Set MApi_GFX_SetDC_CSC_FMT\n");
        MApi_GFX_SetDC_CSC_FMT(0, 0, GFX_YUV_IN_255, GFX_YUV_YVYU, GFX_YUV_YUYV);

    }
    else
    {
        printf("Reset MApi_GFX_SetDC_CSC_FMT \n");
        MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_255, GFX_YUV_IN_255, GFX_YUV_YVYU, GFX_YUV_YVYU);
    }

    u32Status = MApi_GFX_BitBlt(&drawBuf, GFXDRAW_FLAG_SCALE);
    printf("%d %s MApi_GFX_BitBlt = %lu\n", __LINE__, __FUNCTION__, u32Status);
    MApi_GFX_FlushQueue();
    //Always disable pixel alpha when using gop show
    MApi_GOP_GWIN_SetBlending(gWinInfo[0].GeWinId, FALSE, 0xff);

    bPhotoDispDone = TRUE;
    return TRUE;
}
#endif


