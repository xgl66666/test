/**********************************************************************
 Copyright (c) 2006-2009 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.

**********************************************************************/
#ifndef __ZMPLAYER_SUB_DEC_H__
#define __ZMPLAYER_SUB_DEC_H__

/* Include */

#ifdef __cplusplus
extern "C"
{
#endif

//====================original from MSrv_subtitle_decoder_base.h====================

///  the enum for graphic lock type
typedef enum
{
    ///  Lock dfb for read
    EN_LOCK_READ,
    ///  Lock dfb for write
    EN_LOCK_WRITE
} EN_GRAPHIC_LOCK_TYPE;

///  the enum for framebuffer format
typedef enum
{
    ///  I2 format
    EN_I2,
    ///  I8 format
    EN_I8,
    ///  ARGB4444 format
    EN_ARGB4444,
    ///  ARGB8888 format
    EN_ARGB8888,
    ///  Default Unknown format
    EN_UNKNOWN
} EN_FRAMEBUFFER_FORMAT;

///  the enum for subtitle type
typedef enum
{
    ///  DivX bitmap subtitle
    E_SUBTITLE_TYPE_BMP,
    ///  vobsub bitmap subtitle
    E_SUBTITLE_TYPE_VOBSUB,
    ///  transport stream (live stream)
    E_SUBTITLE_TYPE_TS,
    /// True Type Font Rendering
    E_SUBTITLE_TYPE_TTF,
    /// Specify the number of video type
    E_SUBTITLE_TYPE_NUM,
    /// Default unknow type
    E_SUBTITLE_TYPE_UNKNOWN
} EN_SBT_TYPE;

/// the struct of subtitle rect info
typedef struct
{
    ///  the rect of x
    U16    x;
    ///  the rect of y
    U16    y;
    ///  the rect of width
    U16    u16Width;
    ///  the rect of height
    U16    u16Height;
    ///  the rect of buffer format
    EN_FRAMEBUFFER_FORMAT   enFormat;
} SBT_Rect;

///  the struct of palette color
typedef struct
{
    ///  Alpha
    U8 a;
    ///  Red
    U8 r;
    ///  Green
    U8 g;
    ///  Blue
    U8 b;
} RGBPaletteColor;

///   the struct of gwin info
typedef struct
{
    ///  the gwin of width
    U16    u16Width;
    ///  the gwin of height
    U16    u16Height;
    ///  the gwin of pitch
    U16    u16Pitch;
    ///  the gwin of buffer format
    EN_FRAMEBUFFER_FORMAT   enFormat;
} GWinInfo;

///  the struct of render target callback
typedef struct
{
    /// the callback for creating dfb
    unsigned char(*Create)(void *arg);
    /// the callback for getting video size
    unsigned char(*GetVideoSize)(SBT_Rect *size, void *arg);
    /// the callback for open dfb
    void (*Open)(void *arg);
    /// the callback for close dfb
    void (*Close)(void *arg);
    /// the callback for create dfb window
    unsigned char(*CreateWindow)(U8 u8WinID, SBT_Rect *pRect, unsigned char bDisplayAble, void *arg);
    /// the callback for destroy dfb window
    unsigned char(*DestroyWindow)(U8 u8WinID, void *arg);
    /// the callback for set dfb palette
    void (*SetPalette)(U8 u8WinID, RGBPaletteColor *pPaletteTbl, U16 u16PaletteSize, U8 u8Index, void *arg);
    /// the callback for dfb bitblt
    void (*Bitblt)(U8 u8SrcWinID, U8 u8DstWinID, SBT_Rect *pSrcRect, SBT_Rect *pDstRect, void *arg);
    /// the callback for clear dfb window by color index
    void (*ClearWindowByIndex)(U8 u8WinID, SBT_Rect *pRect, U8 u8ColorIndex, void *arg);
    /// the callback for clear dfb window by palette color
    void (*ClearWindowByColor)(U8 u8WinID, SBT_Rect *pRect, RGBPaletteColor *pColor, void *arg);
    /// the callback for get window info
    unsigned char(*GetWindowInfo)(U8 u8WinID, GWinInfo *pInfo, void *arg);
    /// the callback for lock dfb
    unsigned char(*Lock)(U8 u8WinID, EN_GRAPHIC_LOCK_TYPE enLockType, U8 **pu8Buffer, void *arg);
    /// the callback for unlock dfb
    void (*UnLock)(U8 u8WinID, void *arg);
    /// the callback for render font
    unsigned char(*RenderFont)(U8 *pu8TextBuff, U16 u16TextLength, U8 u8TransPaletteIdx, U8 *pu8DisplayBuffer, U16 u16DisplayWidth, U16 u16DisplayHeight, U16 u16DisplayPitch);
    /// the callback for show dfb
    void (*Show)(void *arg);
    /// the callback for unshow dfb
    void (*Unshow)(void *arg);
} SUBTITLE_IRenderTarget;

//====================original from MSrv_subtitle_decoder_base.h====================

unsigned char MApi_MPlayer_Connect(SUBTITLE_IRenderTarget* m_RenderTarge);
unsigned char MApi_MPlayer_Disconnect(void);
unsigned char MApi_MPlayer_Open(void);
unsigned char MApi_MPlayer_Close(void);
void MApi_MPlayer_Set_VobSub_Palette(RGBPaletteColor *pPaletteTbl, U8 palette_size);
void MApi_MPlayer_Set_Subtitle_Dimension(U16 u16Width, U16 u16Height);
void MApi_MPlayer_Push_SpuQueue(U8 *pu8Buf, U16 u16Len, U32 u32PTS, EN_SBT_TYPE enSubType, U32 u32Duration);
void MApi_MPlayer_Set_Subtitle_Type(EN_SBT_TYPE enSubType);
void MApi_MPlayer_SetPageIDs(U16 u16CPID, U16 u16APID);

#ifdef __cplusplus
}
#endif

#endif
