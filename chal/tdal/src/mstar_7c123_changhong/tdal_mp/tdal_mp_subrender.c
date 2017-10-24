/*
 * tdal_mp_subrender.c
 *
 *  Created on: 5 Feb 2013
 *      Author: mijovic
 */

#include<ctype.h>
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes2.h"
#include "apiVDEC.h"

#include "msAPI_MM.h"
#include "MSrv_ZmplayerSubtitleDecoder.h"
#include "tdal_mp_p.h"
#include "apiGFX.h"
#include "apiGOP.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiPNL.h"
#include "apiVDEC.h"

#include "crules.h"

#define ST_PRPT_FLAG_NONE 0
#define ST_PRPT_FLAG_SHOW 1
#define ST_PRPT_FLAG_GWIN 2
#define ST_PRPT_FLAG_INDEX_MODE 4
#define ST_PRPT_FLAG_COLOR_KEY 8
#define ST_PRPT_FLAG_LOCK_READ 16
#define ST_PRPT_FLAG_LOCK_WRITE 32
#define ST_PRPT_FLAG_IN_USE 64
#define ST_PRPT_FLAG_SET_PALETTE 128

#define ST_GET_COLOR_U32(a,r,g,b) (((MS_U32)(a))<<24)+(((MS_U32)(r))<<16)+(((MS_U32)(g))<<8)+(MS_U32)(b)

#define ST_SET_PROPERTY(pWindowData, Flag) do {pWindowData->u8PropertyFlag|=Flag;}while(0)
#define ST_CLEAR_PROPERTY(pWindowData, Flag) do {pWindowData->u8PropertyFlag&=(~Flag);}while(0)
#define ST_HAS_PROPERTY(pWindowData, Flag)  (pWindowData->u8PropertyFlag&Flag)

#define ST_WIN_MAX 10
#define ST_BUF_ALIGN(_x_)    (((MS_U32)(_x_) + 31) & ~31)
#define ST_DEBUG(fmt, arg...)  printf((char *)fmt,##arg)
#define ST_DEBUG_FLOW(fmt, arg...) //printf((char *)fmt,##arg)

#define E_GOP_Sub 2
#define ST_GOP_TO_OP 1
#define ST_GOP_TO_IP 2
#define ST_GOP_RENDER_TYPE ST_GOP_TO_IP
//just for demo purpose,
#define ST_USE_GOP_FB 0
#define ST_GOP_TRAN_ENABLE 0

typedef struct
{
    MS_U32 u32VAAddress;
    GFX_BufferInfo stBufferInfo;
    MS_U8 u8GWinId;
    MS_U8 u8WinId;
    MS_U8 u8PropertyFlag;
    GOP_PaletteEntry I8Palette[GOP_PALETTE_ENTRY_NUM];
    MS_U8 u8ColorkeyIndex;
    MS_U32 u32ColorkeyValue;
    MS_S32 s32ReadMutex;
    MS_S32 s32WriteMutex;
#if ST_USE_GOP_FB
    MS_U8 u8GOPFBId;
#endif
}stSubtitleWindow;


extern MS_S32 gs32NonCachedPoolID;
LOCAL stSubtitleWindow _pWindowData[ST_WIN_MAX];
LOCAL MS_BOOL _bRenderCreated = FALSE;

LOCAL stSubtitleWindow *_GetWindowData(U8 u8WinID)
{
    MS_U32 i=0;
    for ( i=0; i< ST_WIN_MAX; i++)
    {
        if ((ST_HAS_PROPERTY((&_pWindowData[i]), ST_PRPT_FLAG_IN_USE) )&&
            (u8WinID == _pWindowData[i].u8WinId))
        {
            return &_pWindowData[i];
        }
    }
    return NULL;
}
LOCAL stSubtitleWindow *_FindShowWindow(void)
{
    MS_U32 i=0;

     for ( i=0; i< ST_WIN_MAX; i++)
    {
        if ( ST_HAS_PROPERTY((&_pWindowData[i]), ST_PRPT_FLAG_SHOW))
        {
            return &_pWindowData[i];
        }
    }
     return NULL;
}


LOCAL stSubtitleWindow *_CreateWindowData(U8 u8WinID,  SBT_Rect *pRect, MS_BOOL bDisplayAble)
{
    MS_U8 u8FbId=0xFF;
    MS_U32 i=0;
    stSubtitleWindow *pWindowData = NULL;
    if (_GetWindowData(u8WinID))
    {
        ST_DEBUG("Duplicated window ID %d\n", u8WinID);
        return NULL;
    }

    for ( i=0; i< ST_WIN_MAX; i++)
    {
        if ( !ST_HAS_PROPERTY((&_pWindowData[i]), ST_PRPT_FLAG_IN_USE))
        {
            pWindowData = &_pWindowData[i];
            ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_IN_USE);
            break;
        }
    }
    if (pWindowData == NULL)
    {
        return NULL;
    }

    pWindowData->u8WinId = u8WinID;

    if (bDisplayAble)
    {
        ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_SHOW);
    }
    pWindowData->stBufferInfo.u32Width = pRect->u16Width;
    pWindowData->stBufferInfo.u32Height = pRect->u16Height;

    //decide pitch
    switch(pRect->enFormat)
    {
        case EN_I2:
            ST_DEBUG("Format is I2\n");
            ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_INDEX_MODE);
            pWindowData->stBufferInfo.u32ColorFmt = GFX_FMT_I2;
            pWindowData->stBufferInfo.u32Pitch = pRect->u16Width/4;
            break;

        case EN_I8:
            ST_DEBUG("Format is I8\n");
            ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_INDEX_MODE);
            pWindowData->stBufferInfo.u32ColorFmt = GFX_FMT_I8;
            pWindowData->stBufferInfo.u32Pitch = pRect->u16Width;
            break;

        case EN_ARGB4444:
            ST_DEBUG("Format is ARGB4444\n");
            pWindowData->stBufferInfo.u32ColorFmt = GFX_FMT_ARGB4444;
            pWindowData->stBufferInfo.u32Pitch = pRect->u16Width*2;
            break;

        default:
        case EN_ARGB8888:
            ST_DEBUG("Format is ARGB8888\n");
            pWindowData->stBufferInfo.u32ColorFmt = GFX_FMT_ARGB8888;
            pWindowData->stBufferInfo.u32Pitch = pRect->u16Width*4;
            break;
    }
#if ST_USE_GOP_FB
    u8FbId= MApi_GOP_GWIN_GetFreeFBID();
    pWindowData->u8GOPFBId = u8FbId;
    MApi_GOP_GWIN_CreateFB(u8FbId,
                                                0,
                                                0,
                                                pWindowData->stBufferInfo.u32Width,
                                                pWindowData->stBufferInfo.u32Height,
                                                pWindowData->stBufferInfo.u32ColorFmt);

    MApi_GOP_GWIN_GetFBAddr(u8FbId, &pWindowData->u32VAAddress);
    pWindowData->u32VAAddress = MsOS_PA2KSEG1(pWindowData->u32VAAddress);
#else
    pWindowData->u32VAAddress =(MS_U32)
    MsOS_AllocateMemory ((pWindowData->stBufferInfo.u32Pitch *pWindowData->stBufferInfo.u32Height)+32, gs32NonCachedPoolID);
#endif

    if ( pWindowData->u32VAAddress == 0)
    {
        ST_DEBUG("Allocate Memory fail!\n");
        memset(pWindowData, 0, sizeof(stSubtitleWindow));
        return NULL;
    }

#if ST_USE_GOP_FB
    pWindowData->stBufferInfo.u32Addr = (MS_U32) MsOS_VA2PA(pWindowData->u32VAAddress);
    //clear frame buffer
    memset((void*)pWindowData->u32VAAddress, 0, pWindowData->stBufferInfo.u32Pitch*pWindowData->stBufferInfo.u32Height);
#else
    //Alignment & PA Address setting
    pWindowData->stBufferInfo.u32Addr = (MS_U32) MsOS_VA2PA(pWindowData->u32VAAddress);
    pWindowData->stBufferInfo.u32Addr = ST_BUF_ALIGN(pWindowData->stBufferInfo.u32Addr);
    //clear frame buffer
    memset((void*)pWindowData->u32VAAddress, 0, pWindowData->stBufferInfo.u32Pitch*pWindowData->stBufferInfo.u32Height+32);
#endif

    pWindowData->s32ReadMutex = MsOS_CreateMutex ( E_MSOS_FIFO, "MM_Subtitle_Read", MSOS_PROCESS_SHARED);
    pWindowData->s32WriteMutex = MsOS_CreateMutex ( E_MSOS_FIFO, "MM_Subtitle_Write", MSOS_PROCESS_SHARED);
    if ( ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_SHOW) && !ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_GWIN))
    {
#if ST_USE_GOP_FB
        u8FbId= pWindowData->u8GOPFBId;
#else
        u8FbId= MApi_GOP_GWIN_GetFreeFBID();
#endif
        MApi_GOP_GWIN_CreateFBbyStaticAddr(u8FbId,
                                                                    0,
                                                                    0,
                                                                    pWindowData->stBufferInfo.u32Width,
                                                                    pWindowData->stBufferInfo.u32Height,
                                                                    pWindowData->stBufferInfo.u32ColorFmt,
                                                                    pWindowData->stBufferInfo.u32Addr);


        pWindowData->u8GWinId = MApi_GOP_GWIN_CreateWin_Assign_FB(E_GOP_Sub, u8FbId, 0, 0);
        ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_GWIN);
    }
    return pWindowData;
}

LOCAL void _FreeWindowData(MS_U8 u8WinID)
{
    stSubtitleWindow *pWindowData = _GetWindowData(u8WinID);
    if (pWindowData )
    {
        if ( ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_GWIN))
        {
            MApi_GOP_GWIN_DeleteWin(pWindowData->u8GWinId);
        }
#if ST_USE_GOP_FB
        MApi_GOP_GWIN_DeleteFB(pWindowData->u8GOPFBId);
#else
        MsOS_FreeMemory ((void *)pWindowData->u32VAAddress, gs32NonCachedPoolID);
#endif
        MsOS_DeleteMutex (pWindowData->s32ReadMutex);
        MsOS_DeleteMutex (pWindowData->s32WriteMutex);
        memset(pWindowData, 0, sizeof(stSubtitleWindow));
    }
}
LOCAL void _SetupGfxRectFillInfo(GFX_RectFillInfo *pGfxRectFillInfo, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight,
                                    GFX_Buffer_Format gfxBufFmt, MS_U32 u32ColorS, MS_U32 u32ColorE, MS_U32 u32Flag)
{
    pGfxRectFillInfo->dstBlock.x = u16DstX;
    pGfxRectFillInfo->dstBlock.y = u16DstY;
    pGfxRectFillInfo->dstBlock.height= u16DstHeight;
    pGfxRectFillInfo->dstBlock.width= u16DstWidth;

    pGfxRectFillInfo->fmt = gfxBufFmt;

    pGfxRectFillInfo->colorRange.color_s.a = (MS_U8)((u32ColorS>>24)&0xFF);
    pGfxRectFillInfo->colorRange.color_s.r = (MS_U8)((u32ColorS>>16)&0xFF);
    pGfxRectFillInfo->colorRange.color_s.g = (MS_U8)((u32ColorS>>8)&0xFF);
    pGfxRectFillInfo->colorRange.color_s.b = (MS_U8)((u32ColorS)&0xFF);

    pGfxRectFillInfo->colorRange.color_e.a = (MS_U8)((u32ColorE>>24)&0xFF);
    pGfxRectFillInfo->colorRange.color_e.r = (MS_U8)((u32ColorE>>16)&0xFF);
    pGfxRectFillInfo->colorRange.color_e.g = (MS_U8)((u32ColorE>>8)&0xFF);
    pGfxRectFillInfo->colorRange.color_e.b = (MS_U8)((u32ColorE)&0xFF);

    pGfxRectFillInfo->flag = u32Flag;
}

LOCAL void _FillRect(stSubtitleWindow *pWindowData, SBT_Rect *pRect,MS_U32 Color)
{
    GFX_RectFillInfo gfxDrawRect;
    MApi_GFX_BeginDraw();
    MApi_GFX_SetDstBufferInfo(&pWindowData->stBufferInfo, 0);
    MApi_GFX_SetDither(FALSE);

    if (pRect)
    {
        _SetupGfxRectFillInfo(&gfxDrawRect,
                                        pRect->x,
                                        pRect->y,
                                        pRect->u16Width,
                                        pRect->u16Height,
                                        pWindowData->stBufferInfo.u32ColorFmt,
                                        Color,
                                        Color,
                                        GFXRECT_FLAG_COLOR_CONSTANT);
    }
    else
    {
        _SetupGfxRectFillInfo(&gfxDrawRect,
                                        0,
                                        0,
                                        pWindowData->stBufferInfo.u32Width,
                                        pWindowData->stBufferInfo.u32Height,
                                        pWindowData->stBufferInfo.u32ColorFmt,
                                        Color,
                                        Color,
                                        GFXRECT_FLAG_COLOR_CONSTANT);
    }

    MApi_GFX_RectFill(&gfxDrawRect);
    //must call flush queue to wait ge done
    MApi_GFX_FlushQueue();
    MApi_GFX_EndDraw();

}
LOCAL void _ResetDisplayTransColorKey(stSubtitleWindow *pWindowData)
{
//no need to use trans-color key, disable now
#if ST_GOP_TRAN_ENABLE

    if (!ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_GWIN))
    {
        return;
    }

    //re-set trans colorkey setting if necessory
     if (ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_COLOR_KEY))
    {
        EN_GOP_TRANSCLR_FMT eTransFmt;
        MS_U32 u32ColorKey = pWindowData->u32ColorkeyValue;

        MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);
        if (ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_INDEX_MODE))
        {
            eTransFmt = GOPTRANSCLR_FMT0;
        }
        else
        {
            eTransFmt = GOPTRANSCLR_FMT0;
        }

       switch(pWindowData->stBufferInfo.u32ColorFmt)
       {
            case GFX_FMT_ARGB4444:
            {
                MS_U32 u32A, u32R, u32G, u32B;
                u32A = (u32ColorKey>>28) & 0x0F;
                u32R = (u32ColorKey>>20) & 0x0F;
                u32G = (u32ColorKey>>12) & 0x0F;
                u32B = (u32ColorKey>>4)   & 0x0F;
                u32ColorKey = (u32A<<24)+(u32R<<16)+(u32G<<8)+u32B;
                u32ColorKey|=(u32ColorKey<<4);
                break;
            }
            default:
                break;
       }
       MApi_GOP_GWIN_EnableTransClr(eTransFmt, TRUE);
       MApi_GOP_GWIN_SetTransClr_8888(u32ColorKey, 0xff);
    }
#endif
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Create
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

MS_BOOL MM_SubRender_Create(void *arg)
{
    ST_DEBUG_FLOW("======> _MM_SubRender_Create!\n");
#if ST_GOP_RENDER_TYPE== ST_GOP_TO_IP
        ST_DEBUG("MM_SubRender_Show-GOP to E_GOP_DST_IP0\n");
        MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);
        MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
        MApi_GOP_GWIN_SetGOPDst(E_GOP_Sub, E_GOP_DST_IP0);
#else
        MS_WINDOW_TYPE dispWin;
        ST_DEBUG("MM_SubRender_Show-GOP to E_GOP_DST_OP0\n");
        MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);
        MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
        MApi_GOP_GWIN_SetGOPDst(E_GOP_Sub, E_GOP_DST_OP0);
#endif

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.GetVideoSize
/// @param[out] pVideoSize Video size must filled in function.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

MS_BOOL MM_SubRender_GetVideoSizeFromVdec(SBT_Rect *pVideoSize, void *arg)
{
    pVideoSize->x = 0;
    pVideoSize->y = 0;

    VDEC_DispInfo   info = {};
    MApi_VDEC_GetDispInfo(&info);
    pVideoSize->u16Width = info.u16HorSize;
    pVideoSize->u16Height = info.u16VerSize;
    ST_DEBUG("======> _MM_SubRender_GetVideoSizeFromVdec: %d, %d %d %d\n", pVideoSize->x, pVideoSize->y, pVideoSize->u16Width, pVideoSize->u16Height);
    ST_DEBUG("======> _MM_SubRender_GetVideoSizeFromVdec, u8Interlace = %d\n",info.u8Interlace);

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Open
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

void MM_SubRender_Open(void *arg)
{
    ST_DEBUG_FLOW("======> _MM_SubRender_Open!\n");
    _bRenderCreated = TRUE;
    memset((void*)_pWindowData, 0, sizeof(stSubtitleWindow)*ST_WIN_MAX);
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Close
/// @param[in] arg redundant parameter
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

void MM_SubRender_Close(void *arg)
{
    U8 i;
    ST_DEBUG_FLOW("======> _MM_SubRender_Close!\n");

    for(i = 0; i < ST_WIN_MAX; i++)
    {
        if (ST_HAS_PROPERTY((&_pWindowData[i]), ST_PRPT_FLAG_IN_USE))
        {
            MM_SubRender_DestroyWindow(_pWindowData[i].u8WinId, arg);
        }
    }
    _bRenderCreated = FALSE;
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.CreateWindow
/// @param[in] u8WinID Window Id.
/// @param[in] pRect Window rectangle.
/// @param[in] bDisplayAble TRUE if it is OnScreen window.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

MS_BOOL MM_SubRender_CreateWindow(MS_U8 u8WinID, SBT_Rect *pRect, MS_BOOL bDisplayAble, void *arg)
{
    stSubtitleWindow *pWindowData =_CreateWindowData(u8WinID, pRect, bDisplayAble);
    ST_DEBUG_FLOW("======> MM_SubRender_CreateWindow,  u8WinID= %d\n", u8WinID);

    if (pWindowData == NULL)
    {
        ST_DEBUG("======> _CreateWindowData FAIL!\n");
        return FALSE;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.DestroyWindow
/// @param[in] u8WinID Window Id.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

MS_BOOL MM_SubRender_DestroyWindow(U8 u8WinID, void *arg)
{
    ST_DEBUG_FLOW("======> MM_SubRender_DestroyWindow,  u8WinID= %d\n", u8WinID);
    _FreeWindowData(u8WinID);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.SetPalette
/// @param[in] u8WinID Window Id.
/// @param[in] pPaletteTbl Palette entry.
/// @param[in] u16PaletteSize Palette size.
/// @param[in] u8Index redundant parameter.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

void MM_SubRender_SetPalette(U8 u8WinID, RGBPaletteColor *pPaletteTbl, U16 u16PaletteSize, U8 u8Index, void *arg)
{
    MS_U16 i=0;
    stSubtitleWindow *pWindowData = _GetWindowData(u8WinID);
    GOP_PaletteEntry *pTrsansColorEntry;

    ST_DEBUG_FLOW("\n======> _MM_SubRender_SetPalette %d\n", u8WinID);

    if (pWindowData == NULL)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return ;
    }
    if (!( ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_INDEX_MODE)))
    {
        return;
    }

    for ( i=0; i< u16PaletteSize; i++)
    {
       // the alpha value is reverted from subtitle spec
        pWindowData->I8Palette[i].RGB.u8A = 0xFF-pPaletteTbl[i].a;
        pWindowData->I8Palette[i].RGB.u8R = pPaletteTbl[i].r;
        pWindowData->I8Palette[i].RGB.u8G = pPaletteTbl[i].g;
        pWindowData->I8Palette[i].RGB.u8B = pPaletteTbl[i].b;
    }
    pTrsansColorEntry = &pWindowData->I8Palette[pWindowData->u8ColorkeyIndex];
    pWindowData->u32ColorkeyValue= ST_GET_COLOR_U32(pTrsansColorEntry->RGB.u8A, pTrsansColorEntry->RGB.u8R, pTrsansColorEntry->RGB.u8G, pTrsansColorEntry->RGB.u8B);

    //if window is used to display , must update gop palette.
    if (ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_GWIN) )
    {
        MApi_GOP_GWIN_SetPaletteOpt(pWindowData->I8Palette, 0, 255 , E_GOP_PAL_ARGB8888);
        _ResetDisplayTransColorKey(pWindowData);
    }

}

//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Bitblt
/// @param[in] u8SrcWinID Source window ( buffer) Id.
/// @param[in] u8DstWinID Destination window ( buffer) Id.
/// @param[in] pSrcRect Source window rectangle description.
/// @param[in] pSrcRect Destination window rectangle description.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------


void MM_SubRender_Bitblt(U8 u8SrcWinID, U8 u8DstWinID, SBT_Rect *pSrcRect, SBT_Rect *pDstRect, void *arg)
{
    stSubtitleWindow *pSrcWin = _GetWindowData(u8SrcWinID);
    stSubtitleWindow *pDstWin = _GetWindowData(u8DstWinID);
    GFX_DrawRect     DrawRect;
    GFX_Point        ptV0, ptV1;
    ST_DEBUG_FLOW("======> MM_SubRender_Bitblt Src:%d, Dst:%d\n", u8SrcWinID, u8DstWinID);
    if (pSrcWin  == NULL || pDstWin == NULL)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return;
    }

    MApi_GFX_BeginDraw();

    if (pSrcRect)
    {
        DrawRect.srcblk.x = pSrcRect->x;
        DrawRect.srcblk.y = pSrcRect->y;
        DrawRect.srcblk.width = pSrcRect->u16Width;
        DrawRect.srcblk.height = pSrcRect->u16Height;
    }
    else
    {
        DrawRect.srcblk.x = 0;
        DrawRect.srcblk.y = 0;
        DrawRect.srcblk.width = pSrcWin->stBufferInfo.u32Width;
        DrawRect.srcblk.height =pSrcWin->stBufferInfo.u32Height;
    }

    if (pDstRect)
    {
        DrawRect.dstblk.x = pDstRect->x;
        DrawRect.dstblk.y = pDstRect->y;
        DrawRect.dstblk.width = pDstRect->u16Width;
        DrawRect.dstblk.height = pDstRect->u16Height;
    }
    else
    {
        DrawRect.dstblk.x = 0;
        DrawRect.dstblk.y = 0;
        DrawRect.dstblk.width = pDstWin->stBufferInfo.u32Width;
        DrawRect.dstblk.height =pDstWin->stBufferInfo.u32Height;
    }


    ptV0.x = 0;
    ptV0.y = 0;
    ptV1.x = pDstWin->stBufferInfo.u32Width;
    ptV1.y = pDstWin->stBufferInfo.u32Height;
    MApi_GFX_SetSrcBufferInfo(&pSrcWin->stBufferInfo, 0);
    MApi_GFX_SetDstBufferInfo(&pDstWin->stBufferInfo, 0);
    MApi_GFX_SetPaletteOpt( (GFX_PaletteEntry *)pSrcWin->I8Palette, 0, 255);
    MApi_GFX_SetAlpha(TRUE, COEF_ONE, ABL_FROM_ASRC, 0xFF);
    MApi_GFX_SetClip(&ptV0, &ptV1);
    if (pSrcWin->stBufferInfo.u32ColorFmt == GFX_FMT_I8)
    {
        MApi_GFX_SetDither(FALSE);
        MApi_GFX_SetNearestMode(TRUE);
    }
    else
    {
        MApi_GFX_SetNearestMode(TRUE);
    }
    MApi_GFX_BitBlt(&DrawRect, GFXDRAW_FLAG_SCALE);
    MApi_GFX_SetNearestMode(FALSE);
    //must call flush to wait bitblt done.
    MApi_GFX_FlushQueue();
    MApi_GFX_EndDraw();

}


//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.ClearWindowByIndex
/// @param[in] u8WinID window Id.
/// @param[in] pRect window rectangle description which want to clear.
/// @param[in] u8ColorIndex Color index.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

void MM_SubRender_ClearWindowByIndex(U8 u8WinID, SBT_Rect *pRect, U8 u8ColorIndex, void *arg)
{
    stSubtitleWindow *pWindowData = _GetWindowData(u8WinID);

    ST_DEBUG_FLOW("======> MM_SubRender_ClearWindowByIndex %d\n", u8WinID);

    if ( pWindowData == FALSE)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return;
    }
    if (!( ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_INDEX_MODE)))
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return ;
    }
    ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_COLOR_KEY);
    pWindowData->u8ColorkeyIndex = u8ColorIndex;

    if (pWindowData->stBufferInfo.u32ColorFmt == GFX_FMT_I2 ||
        pWindowData->stBufferInfo.u32ColorFmt == GFX_FMT_I8)
    {
        //temp solution due to hw limit
        memset((void*)pWindowData->u32VAAddress, u8ColorIndex, (pWindowData->stBufferInfo.u32Height * pWindowData->stBufferInfo.u32Pitch) +32);
        return;
    }
    _FillRect(pWindowData, pRect,pWindowData->u8ColorkeyIndex);
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.ClearWindowByColor
/// @param[in] u8WinID window Id.
/// @param[in] pRect window rectangle description which want to clear.
/// @param[in] pColor Color.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

void MM_SubRender_ClearWindowByColor(U8 u8WinID, SBT_Rect *pRect, RGBPaletteColor *pColor, void *arg)
{
    stSubtitleWindow *pWindowData = _GetWindowData(u8WinID);
    ST_DEBUG_FLOW("======> _MM_SubRender_ClearWindowByColor %d\n", u8WinID);

    if ( pWindowData == NULL)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return;
    }
    //if window format is index mode
    if ( ST_HAS_PROPERTY(pWindowData, ST_PRPT_FLAG_INDEX_MODE))
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return ;
    }

    // the alpha value is reverted from subtitle spec
    ST_SET_PROPERTY(pWindowData, ST_PRPT_FLAG_COLOR_KEY);
    pWindowData->u32ColorkeyValue = ST_GET_COLOR_U32((0xFF-pColor->a), pColor->r, pColor->g, pColor->b);
    _FillRect(pWindowData, pRect,pWindowData->u32ColorkeyValue);

}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.GetWindowInfo
/// @param[in] u8WinID window Id.
/// @param[out] pInfo window information must be filled in.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

MS_BOOL MM_SubRender_GetWindowInfo(U8 u8WinID, GWinInfo *pInfo, void *arg)
{
    stSubtitleWindow *pWin = _GetWindowData(u8WinID);
    ST_DEBUG_FLOW("======> _MM_SubRender_GetWindowInfo %d\n", u8WinID);
    if ( pWin == NULL)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return FALSE;
    }
    pInfo->u16Width = pWin-> stBufferInfo.u32Width;
    pInfo->u16Height = pWin-> stBufferInfo.u32Height;
    pInfo->u16Pitch = pWin-> stBufferInfo.u32Pitch;
    switch(pWin-> stBufferInfo.u32ColorFmt)
    {
        case GFX_FMT_I2:
            pInfo->enFormat = EN_I2;
            break;
        case GFX_FMT_I8:
            pInfo->enFormat = EN_I8;
            break;
        case GFX_FMT_ARGB4444:
            pInfo->enFormat = EN_ARGB4444;
            break;
        case GFX_FMT_ARGB8888:
            pInfo->enFormat = EN_ARGB8888;
            break;
        default:
            ST_DEBUG("Unexpected buffer format\n");
            break;

    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Lock
/// @param[in] u8WinID window Id.
/// @param[in] enLockType EN_LOCK_READ or EN_LOCK_WRITE.
/// @param[in] pu8Buffer window buffer pointer must be filled in.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

MS_BOOL MM_SubRender_Lock(U8 u8WinID, EN_GRAPHIC_LOCK_TYPE enLockType, U8 **pu8Buffer, void *arg)
{
    stSubtitleWindow *pWin = _GetWindowData(u8WinID);
    ST_DEBUG_FLOW("======> _MM_SubRender_Lock %d\n", u8WinID);
    if ( pWin == NULL)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return FALSE;
    }
    if (enLockType == EN_LOCK_READ)
    {
        ST_SET_PROPERTY(pWin, ST_PRPT_FLAG_LOCK_READ);
        MsOS_ObtainMutex (pWin->s32ReadMutex, MSOS_WAIT_FOREVER);
    }
    else
    {
        ST_SET_PROPERTY(pWin, ST_PRPT_FLAG_LOCK_WRITE);
        MsOS_ObtainMutex (pWin->s32WriteMutex, MSOS_WAIT_FOREVER);
    }
    *pu8Buffer =(MS_U8*) MsOS_PA2KSEG1(pWin->stBufferInfo.u32Addr);
    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.UnLock
/// @param[in] u8WinID window Id.
/// @param[in] arg redundant parameter.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------

void MM_SubRender_UnLock(U8 u8WinID, void *arg)
{
    stSubtitleWindow *pWin = _GetWindowData(u8WinID);

    ST_DEBUG_FLOW("======> _MM_SubRender_UnLock %d\n", u8WinID);
    if ( pWin == NULL)
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return;
    }

    if (ST_HAS_PROPERTY(pWin, ST_PRPT_FLAG_LOCK_READ))
    {
        ST_CLEAR_PROPERTY(pWin, ST_PRPT_FLAG_LOCK_READ);
        MsOS_ReleaseMutex (pWin->s32ReadMutex);
    }
    if (ST_HAS_PROPERTY(pWin, ST_PRPT_FLAG_LOCK_WRITE))
    {
        ST_CLEAR_PROPERTY(pWin, ST_PRPT_FLAG_LOCK_WRITE);
        MsOS_ReleaseMutex (pWin->s32WriteMutex);
    }
}

LOCAL U16 _UTF82UNICODE(MS_U8 *pu8Char,MS_U8 *pu8Offset)
{
    MS_U16 u16Char = 0;

    if (pu8Char[0] < 0x80)
    {
        u16Char = (pu8Char[0] & 0x7F);
        *pu8Offset = 1;
    }
    else if (((pu8Char[0] >= 0xC0) && (pu8Char[0] < 0xE0)) && (pu8Char[1] >= 0x80))
    {
        //UTF8 to UNICODE
        memset((MS_U8*)&u16Char,((pu8Char[1] & 0x3F) | ((pu8Char[0] & 0x3) << 6)),1);
        memset(((MS_U8*)&u16Char)+1,((pu8Char[0] & 0x1C) >> 2),1);
        *pu8Offset = 2;
    }
    else if ((pu8Char[0] >= 0xE0) && (pu8Char[1] >= 0x80) && (pu8Char[2] >= 0x80))
    {
        //UTF8 to UNICODE
        memset((MS_U8*)&u16Char,((pu8Char[2] & 0x3F) | ((pu8Char[1] & 3) << 6)),1);
        memset(((MS_U8*)&u16Char)+1,(((pu8Char[1] & 0x3C) >> 2) | ((pu8Char[0] & 0xF) << 4)),1);
        *pu8Offset = 3;
    }
    else
    {
        *pu8Offset = 1;
        printf("UTF8 to UNICODE error!!\n");
    }

    return u16Char;
}

#define LINE_LIMIT 6
#define LINE_TEXT_LIMIT 100
#define LINE_TEXT_MAX 256

LOCAL void _DrawText(MS_U8 u32LineNum,
                                    MS_U8 TotalLine,
                                    MS_U16 *pu16Text,
                                    MS_U16 u16Len,
                                    MS_U8 *pu8Buffer,
                                    MS_U16 u16Width,
                                    MS_U16 u16Height,
                                    MS_U16 u16Pitch)
{
    MS_U8 u8PrintText[LINE_TEXT_MAX] = {0};
    MS_U8 u8TextLen = 0;
    MS_U8 i=0;
    //just safe to use
    if (u16Len > LINE_TEXT_MAX)
    {
        u16Len = LINE_TEXT_MAX-1;
    }
    //pBuffer is I8 Mode
    for( i = 0; i< u16Len; i++)
    {
        u8PrintText[u8TextLen++] = pu16Text[i] < 0x80 ? (MS_U8)pu16Text[i]:'*';
    }
    printf("%s\n", u8PrintText);
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.RenderFont
/// @param[in] pu8TextBuff Text buffer entry. in current version, it's always UTF-8 encoded.
/// @param[in] u16TextLength Text Buffer Size
/// @param[in] u8TransPaletteIdx TransPaletteIdx
/// @param[in] pu8DisplayBuffer Display buffer. in current version, It's always I8 format.
/// @param[in] u16DisplayWidth Display buffer width.
/// @param[in] u16DisplayHeight Display buffer height.
/// @param[in] u16DisplayPitch Display buffer pitch.
/// @return TRUE:  success.
/// @return FALSE:  fail.
//------------------------------------------------------------------------------
MS_BOOL MM_SubRender_RenderFont(U8 *pu8TextBuff, U16 u16TextLength, U8 u8TransPaletteIdx, U8 *pu8DisplayBuffer, U16 u16DisplayWidth, U16 u16DisplayHeight, U16 u16DisplayPitch)
{//currently not supported
    ST_DEBUG_FLOW("\n======> _MM_SubRender_RenderFont");
    //only show
    MS_U32 i = 0;
    MS_U8 u8Offset = 0;
    MS_U16 u16LineText[LINE_LIMIT][LINE_TEXT_LIMIT];
    MS_U16 u16LineTextLength[LINE_LIMIT] = {0};
    MS_U16 u8TotalLine = 0;
    MS_U16 u8Index = 0;
    //MS_U32 PA = MS_VA2PA((MS_U32)pu8DisplayBuffer);
    memset(u16LineText, 0, sizeof(u16LineText));

    for(i = 0 ; i < u16TextLength; i += u8Offset)
    {
        MS_U16 u16Char = _UTF82UNICODE(&pu8TextBuff[i],&u8Offset); ;



        if (u8TotalLine>=LINE_LIMIT)
        {
            break;
        }
        if (u16Char == 0x0A)   // "0A" "0A0D" for new line
        {
            MS_U16 u16TempChar;
            MS_U8 u8TempOffset;

            u16TempChar = _UTF82UNICODE(&pu8TextBuff[i+1],&u8TempOffset);
            if(u16TempChar == 0x0D)
            {
                u8Offset += u8TempOffset;
            }
            u8TotalLine++;
            u8Index = 0;
            continue;
        }
        else if(u16Char == 0x5C)     // "\N" for new line!
        {
            U16 u16TempChar;
            U8 u8TempOffset;
            u16TempChar = _UTF82UNICODE(&pu8TextBuff[i+1],&u8TempOffset);
            if(u16TempChar == 0x4E)     //  'N'
            {
                u8Offset += u8TempOffset;
                u8TotalLine++;
                u8Index = 0;
                continue;
            }
        }
        else if(u16Char == 0)  //UNICODE error!
        {
            break;
        }
        if (u8Index>=LINE_TEXT_LIMIT-1 )
        {
            continue;
        }
        u16LineText[u8TotalLine][u8Index] = u16Char;
        u16LineTextLength[u8TotalLine]++;
        u8Index++;

    }
    //display line
    printf("\n******Text Subtitle Begin************\n");
    //display line
    for ( i = 0; i <= u8TotalLine; i++)
    {
        _DrawText(i,
                        u8TotalLine+1,
                        u16LineText[i],
                        u16LineTextLength[i],
                        pu8DisplayBuffer,
                        u16DisplayWidth,
                        u16DisplayHeight,
                        u16DisplayPitch);


    }
    printf("\n******Text Subtitle End************\n");


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Show to show On-Screen window
/// @param[in] arg redundant parameter.
/// @return none
//------------------------------------------------------------------------------

void MM_SubRender_Show(void *arg)
{
    stSubtitleWindow *pWindowData  = _FindShowWindow();
    VDEC_DispInfo Dispinfo;

    ST_DEBUG_FLOW("======> _MM_SubRender_Show\n");

    //check video status
    if ((E_VDEC_OK != MApi_VDEC_GetDispInfo(&Dispinfo)) ||
        (0 == Dispinfo.u16HorSize) ||
        (0 == Dispinfo.u16VerSize))
    {
        ST_DEBUG("Unexpected Error %d\n", __LINE__);
        return;
    }

#if ST_GOP_RENDER_TYPE== ST_GOP_TO_IP
    ST_DEBUG("MM_SubRender_Show-GOP to E_GOP_DST_IP0\n");
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);
    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP_Sub,
                                                         E_GOP_DST_IP0,
                                                         0,
                                                         0,
                                                         pWindowData->stBufferInfo.u32Width,
                                                         pWindowData->stBufferInfo.u32Height);
    MApi_GOP_GWIN_EnableProgressive((Dispinfo.u8Interlace == 1)?FALSE:TRUE );
    MApi_GOP_GWIN_Set_HSCALE(TRUE, pWindowData->stBufferInfo.u32Width, Dispinfo.u16HorSize);
    MApi_GOP_GWIN_Set_VSCALE(TRUE, pWindowData->stBufferInfo.u32Height, Dispinfo.u16VerSize);
    MApi_GOP_GWIN_SetFieldInver(TRUE);
#else
    MS_WINDOW_TYPE dispWin;
    ST_DEBUG("MM_SubRender_Show-GOP to E_GOP_DST_OP0\n");
    MApi_GOP_GWIN_SwitchGOP(E_GOP_Sub);
    MApi_XC_GetDispWinFromReg(&dispWin, MAIN_WINDOW);
    dispWin.x =dispWin.x- g_IPanel.HStart();
    dispWin.y =dispWin.y- g_IPanel.VStart();
    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP_Sub,
                                                         E_GOP_DST_OP0,
                                                         dispWin.x,
                                                         dispWin.y,
                                                         pWindowData->stBufferInfo.u32Width,
                                                         pWindowData->stBufferInfo.u32Height);
    MApi_GOP_GWIN_EnableProgressive(TRUE);
    //if dst to OP, must check dispWin.width & dispWin.height limitation
    //gop only support stretch up
    if ( (dispWin.width>= pWindowData->stBufferInfo.u32Width) &&
        (dispWin.height>= pWindowData->stBufferInfo.u32Height) )
    {
        MApi_GOP_GWIN_Set_HSCALE(TRUE, pWindowData->stBufferInfo.u32Width, dispWin.width);
        MApi_GOP_GWIN_Set_VSCALE(TRUE, pWindowData->stBufferInfo.u32Height, dispWin.height);
    }
    else
    {
        ST_DEBUG("XC  size is less then gwin buffer size\n");
        MApi_GOP_GWIN_Set_HSCALE(FALSE, pWindowData->stBufferInfo.u32Width, dispWin.width);
        MApi_GOP_GWIN_Set_VSCALE(FALSE, pWindowData->stBufferInfo.u32Height, dispWin.height);
    }
#endif
     //optional
    MApi_GOP_GWIN_Set_HStretchMode(E_GOP_HSTRCH_6TAPE_NEAREST);
    MApi_GOP_GWIN_Set_VStretchMode (E_GOP_VSTRCH_NEAREST);

    MApi_GOP_GWIN_Set_TranspColorStretchMode(E_GOP_TRANSPCOLOR_STRCH_DUPLICATE);
    MApi_GOP_GWIN_SetBlending(pWindowData->u8GWinId, TRUE, 0xFF);
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
    _ResetDisplayTransColorKey(pWindowData);
    MApi_GOP_GWIN_Enable(pWindowData->u8GWinId, TRUE);
}
//------------------------------------------------------------------------------
/// @brief MM-subtitle render implementation for SUBTITLE_IRenderTarget.Show to hide On-Screen window
/// @param[in] arg redundant parameter.
/// @return none
//------------------------------------------------------------------------------

void MM_SubRender_Unshow(void *arg)
{
    stSubtitleWindow *pWin  = _FindShowWindow();
    ST_DEBUG_FLOW("======> _MM_SubRender_Unshow\n");
    if ( pWin == NULL)
    {
        return;
    }
    MApi_GOP_GWIN_Enable(pWin->u8GWinId, FALSE);
}

