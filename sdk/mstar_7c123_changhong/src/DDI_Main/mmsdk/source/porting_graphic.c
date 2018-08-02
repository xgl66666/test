#include "MsCommon.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "porting_graphic.h"
#include "apiGFX.h"
#include "MsOS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Panel.h"
#include "porting_os.h"
#include "porting_sysinfo.h"

#define gprintf(...)

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define GFX_ENTRY() {}
#define GFX_RETURN(_ret) {}

#define IS_DISPLAY_PHOTO_IN_MMSDK_FLOW do {\
                                           if (FALSE == PT_SYS_GetGraphicDisplayFlag()) \
                                           { \
                                               return TRUE;\
                                           }\
                                       } while(0);

#define IS_VALID_SURFACE(a) do {\
                                if (NULL == a)\
                                {\
                                    gprintf("[%s] Err:Invalid param!\n",__FUNCTION__);\
                                    return FALSE;\
                                }\
                            } while(0);


typedef struct {
     int            x1;  /* X coordinate of top-left point */
     int            y1;  /* Y coordinate of top-left point */
     int            x2;  /* X coordinate of lower-right point */
     int            y2;  /* Y coordinate of lower-right point */
} ST_GRAPHIC_REGION;

typedef struct
{
    /// Color format
    EN_MMSDK_COLOR_FORMAT eFormat;
    MMSDK_U32 u32Width;
    MMSDK_U32 u32Height;
    MMSDK_U32 u32BuffAddr;
    MMSDK_U32 u32Pitch;
    ST_GRAPHIC_REGION stRegion;
    ST_MMSDK_THUMBNAIL_PARAM stPhotoRequest;
    EN_MMSDK_ROTATE_ANGLE eAngle;
    MMSDK_BOOL IsThumbnail;
    MMSDK_BOOL IsFadeInOrOut;
    MMSDK_U16 u16Alpha;
    MMSDK_BOOL IsAlphaBlending;
    MMSDK_BOOL IsTransformed;
} ST_GRAPHIC_SURFACE;

GFX_Buffer_Format _TranslateToGFXBufferFormat(EN_MMSDK_COLOR_FORMAT eFormat)
{
    switch (eFormat)
    {
        case E_MMSDK_COLOR_FORMAT_ARGB8888:
            return GFX_FMT_ARGB8888;
        case E_MMSDK_COLOR_FORMAT_ARGB1555:
            return GFX_FMT_ARGB1555;
        case E_MMSDK_COLOR_FORMAT_ARGB4444:
            return GFX_FMT_ARGB4444;
        case E_MMSDK_COLOR_FORMAT_YUV422:
            return GFX_FMT_YUV422;
        default:
            printf("_TranslateToGFXBufferFormat : possible wrong MMSDK color fornat input\n");
            return E_MMSDK_COLOR_FORMAT_ARGB8888;
    }
}

static MS_U32 _CalcPitch(MS_U8 fbFmt, MS_U16 width)
{
    MS_U16 pitch=0;

    switch ( fbFmt )
    {
    case E_MS_FMT_I8 :
        pitch = width;
        break;
    case E_MS_FMT_RGB565 :
    case E_MS_FMT_ARGB4444 :
    case E_MS_FMT_YUV422:
	case E_MS_FMT_ARGB1555:
	case E_MS_FMT_ARGB1555_DST:
        pitch = width << 1;
        break;
    case E_MS_FMT_ARGB8888 :
        pitch = width << 2;
        break;
    default :
        pitch = 0;
        break;
    }
    return (pitch + (0xF)) & ~(0xF); //pitch should be 16 alignment
}

//------------------------------------------------------------------------------
/// Release IDirectFBSurface
/// @param pSurface             \b IN: pointer (of IDirectFBSurface)
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_Release(PT_GRAPHICITEM* pSurface)
{
    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)*pSurface;

    //IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;
    if (FALSE == PT_SYS_GetGraphicDisplayFlag())
    {
        if (pGraphicSurface!= NULL)
        {
            printf("Surface is not NULL?! shall it free Surface??\n");
        }
        return TRUE;
    }

    //printf("[PT_Graphic_Release] pGraphicSurface =0x%x\n", (unsigned int)pGraphicSurface);

    if (pGraphicSurface!= NULL)
    {
        PT_MsOS_FreeMemory(pGraphicSurface);
        *pSurface = NULL;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// Create IDirectFBSurface
/// @param ptr                  \b IN: pointer (of IDirectFBSurface)
/// @param eFormat              \b IN: photo format
/// @param u32Width             \b IN:
/// @param u32Height            \b IN:
/// @param u32BuffAddr          \b IN:
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_CreateSurface(PT_GRAPHICITEM* pSurface, EN_MMSDK_COLOR_FORMAT eFormat, MMSDK_U32 u32Width, MMSDK_U32 u32Height, MMSDK_U32 u32BuffAddr, MMSDK_U32 u32Pitch)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)PT_MsOS_AllocateMemory(sizeof(ST_GRAPHIC_SURFACE));
    if (pGraphicSurface)
        memset(pGraphicSurface, 0, sizeof(ST_GRAPHIC_SURFACE));
    else
    {
        printf("[PT_Graphic_CreateSurface] malloc fail !\n");
        return FALSE;
    }

    //GFX_DrawRect _GFX_DrawRect;

    gprintf("[PT_Graphic_CreateSurface] eFormat =%d, u32BuffAddr=0x%x, u32Height=%d, u32Width=%d, u32Pitch=%d\n",
    eFormat, (unsigned int)u32BuffAddr, (int)u32Height, (int)u32Width, (int)u32Pitch);
    gprintf("[PT_Graphic_CreateSurface] pGraphicSurface =0x%x\n", (unsigned int)pGraphicSurface);

    pGraphicSurface->eFormat = eFormat;
    pGraphicSurface->u32BuffAddr = (MS_PHYADDR)u32BuffAddr; //Not use DFB, use PA is OK
    pGraphicSurface->u32Height = u32Height;
    pGraphicSurface->u32Width = u32Width;
    pGraphicSurface->u32Pitch = u32Pitch;

    *pSurface = (PT_GRAPHICITEM)pGraphicSurface;

/*    IDirectFBSurface* pGraphicSurface = NULL;
    DFBResult eResult = DFB_FAILURE;
    DFBSurfaceDescription* pDsc = (DFBSurfaceDescription*)malloc(sizeof(DFBSurfaceDescription));
    IDirectFB* dfb = MSystem::GetInstance()->dfb;

    if(*pSurface != NULL)
    {
        PT_DirectFB_Release(pSurface);
        *pSurface = NULL;
    }

    memset(pDsc, 0, sizeof(DFBSurfaceDescription));
#if NEW_DFB
    pDsc->flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS |
                  DSDESC_WIDTH |
                  DSDESC_HEIGHT |
                  DSDESC_PIXELFORMAT |
                  DSDESC_PREALLOCATED |
                  DSDESC_PREALLOCATED_IN_VIDEO);
#else
    pDsc->flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS |
                  DSDESC_WIDTH |
                  DSDESC_HEIGHT |
                  DSDESC_PIXELFORMAT |
                  DSDESC_PREALLOCATED);
    pDsc->caps = (DFBSurfaceCapabilities)(DSCAPS_SYSTEMONLY);
#endif
    pDsc->width = u32Width;
    pDsc->height = u32Height;

    switch(eFormat)
    {
        case E_MMSDK_COLOR_FORMAT_ARGB8888:
            pDsc->pixelformat = DSPF_ARGB;
            pDsc->preallocated[0].pitch = PHOTO_WIDTH_ALIGN(u32Pitch * 4);
            break;
        case E_MMSDK_COLOR_FORMAT_ARGB1555:
            pDsc->pixelformat = DSPF_ARGB1555;
            pDsc->preallocated[0].pitch = PHOTO_WIDTH_ALIGN(u32Pitch * 2);
            break;
        case E_MMSDK_COLOR_FORMAT_ARGB4444:
            pDsc->pixelformat = DSPF_ARGB4444;
            pDsc->preallocated[0].pitch = PHOTO_WIDTH_ALIGN(u32Pitch * 2);
            break;
        case E_MMSDK_COLOR_FORMAT_YUV422:
            pDsc->pixelformat = DSPF_YUY2; //DSPF_UYVY;
            pDsc->preallocated[0].pitch = PHOTO_WIDTH_ALIGN(u32Pitch * 2);
            break;
        default:
            return FALSE;
    }

    pDsc->preallocated[0].data = (void*)u32BuffAddr;
    eResult = dfb->CreateSurface(dfb, pDsc, &pGraphicSurface);
    PHOTO_DFB_CHECK(eResult);
    *pSurface = (void*)pGraphicSurface;
    if (eResult == DFB_OK)
        return TRUE;
    else
        return FALSE;*/
    return TRUE;

}




//------------------------------------------------------------------------------
/// SetClip
//------------------------------------------------------------------------------

MMSDK_BOOL PT_Graphic_SetClip(PT_GRAPHICITEM Surface, ST_MMSDK_RECT Rect)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)Surface;
    GFX_Point  ClipA,ClipB;

    IS_VALID_SURFACE(pGraphicSurface);

    pGraphicSurface->stRegion.x1 = Rect.s32X;
    pGraphicSurface->stRegion.y1 = Rect.s32Y;
    pGraphicSurface->stRegion.x2 = (Rect.s32X + Rect.u16Width) - 1;
    pGraphicSurface->stRegion.y2 = (Rect.s32Y + Rect.u16Height) - 1;

    gprintf("[PT_Graphic_SetClip] pGraphicSurface =0x%x, x1 = %d, y1 = %d, x2 = %d, y2 = %d \n",
        (int)pGraphicSurface, pGraphicSurface->stRegion.x1, pGraphicSurface->stRegion.y1,
        pGraphicSurface->stRegion.x2, pGraphicSurface->stRegion.y2);

    ClipA.x = pGraphicSurface->stRegion.x1;
    ClipA.y = pGraphicSurface->stRegion.y1;
    ClipB.x = pGraphicSurface->stRegion.x2;
    ClipB.y = pGraphicSurface->stRegion.y2;

    MApi_GFX_SetClip(&ClipA, &ClipB);


/*    IDirectFBSurface* pGraphicSurface = (IDirectFBSurface*)pSurface;
    ST_GRAPHIC_REGION stRegion;
    DFBResult eResult = DFB_FAILURE;

    stRegion.x1 = Rect.s32X;
    stRegion.y1 = Rect.s32Y;
    stRegion.x2 = (Rect.s32X + Rect.u16Width) - 1;
    stRegion.y2 = (Rect.s32Y + Rect.u16Height) - 1;
    //printf("\n pThumbnailSurface->SetClip x1[%d]  y1[%d]  x2[%d]  y2[%d] ", stRegion.x1, stRegion.y1, stRegion.x2, stRegion.y2);
    eResult = pGraphicSurface->SetClip(pGraphicSurface, &stRegion);
    PHOTO_DFB_CHECK(eResult);

    if (eResult == DFB_OK)
        return TRUE;
    else
        return FALSE;*/
    return TRUE;
}

MMSDK_BOOL PT_Graphic_SetColor(PT_GRAPHICITEM Surface, ST_MMSDK_THUMBNAIL_PARAM* pstPhotoRequest)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)Surface;

    IS_VALID_SURFACE(pGraphicSurface);

    gprintf("[PT_Graphic_SetColor] pGraphicSurface =0x%x, u8A =%d, u8R=%d, u8G=%d, u8B=%d\n",
        (unsigned int)pGraphicSurface, pstPhotoRequest->u8A, pstPhotoRequest->u8R, pstPhotoRequest->u8G, pstPhotoRequest->u8B);


    pGraphicSurface->stPhotoRequest.u8A = pstPhotoRequest->u8A;
    pGraphicSurface->stPhotoRequest.u8R = pstPhotoRequest->u8R;
    pGraphicSurface->stPhotoRequest.u8G = pstPhotoRequest->u8G;
    pGraphicSurface->stPhotoRequest.u8B = pstPhotoRequest->u8B;

/*    IDirectFBSurface* pGraphicSurface = (IDirectFBSurface*)pSurface;
    DFBResult eResult = DFB_FAILURE;
    eResult = pGraphicSurface->SetColor(pGraphicSurface, pstPhotoRequest->u8R, pstPhotoRequest->u8G, pstPhotoRequest->u8B, pstPhotoRequest->u8A);
    PHOTO_DFB_CHECK(eResult);
    if (eResult == DFB_OK)
        return TRUE;
    else
        return FALSE;*/
    return TRUE;
}

//------------------------------------------------------------------------------
/// Clear IDirectFBSurface
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Graphic_Clear(PT_GRAPHICITEM Surface, const MMSDK_U8 u8A, const MMSDK_U8 u8R, const MMSDK_U8 u8G, const MMSDK_U8 u8B)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)Surface;
    GFX_RectFillInfo sRectInfo;
    GFX_BufferInfo dstbuf;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    int i = 0;

    IS_VALID_SURFACE(pGraphicSurface);

    memset((void*)&sRectInfo, 0, sizeof(GFX_RectFillInfo));
    memset((void*)&dstbuf, 0, sizeof(GFX_BufferInfo));
    memset((void*)&gfxPt0, 0, sizeof(GFX_Point));
    memset((void*)&gfxPt1, 0, sizeof(GFX_Point));

    gprintf("[PT_Graphic_Clear] pGraphicSurface =0x%lx, u8A =%d, u8R=%d, u8G=%d, u8B=%d\n",
    pGraphicSurface, u8A, u8R, u8G, u8B);

    if (GFX_FMT_YUV422 == _TranslateToGFXBufferFormat(pGraphicSurface->eFormat))
    {
        sRectInfo.colorRange.color_e.a= sRectInfo.colorRange.color_s.a = 0xFF;//(MS_U8)pGraphicSurface->stPhotoRequest.u8A;
        sRectInfo.colorRange.color_e.r= sRectInfo.colorRange.color_s.b = 128 + (MS_U8)(-0.169 * u8R  - 0.331 * u8G + 0.5 * u8B);
        sRectInfo.colorRange.color_e.g= sRectInfo.colorRange.color_s.g = (MS_U8)(0.299 * u8R + 0.587 * u8G + 0.144 * u8B);
        sRectInfo.colorRange.color_e.b= sRectInfo.colorRange.color_s.r = 128 + (MS_U8)(0.5 * u8R - 0.419 * u8G - 0.0813 * u8B);
        gprintf("[PT_Graphic_Clear] after transfer : v= %d, y=%d, u=%d\n", sRectInfo.colorRange.color_e.r, sRectInfo.colorRange.color_e.g,sRectInfo.colorRange.color_e.b);
    }
    else
    {
        sRectInfo.colorRange.color_e.a= sRectInfo.colorRange.color_s.a = u8A;
        sRectInfo.colorRange.color_e.r= sRectInfo.colorRange.color_s.r = u8R;
        sRectInfo.colorRange.color_e.g= sRectInfo.colorRange.color_s.g = u8G;
        sRectInfo.colorRange.color_e.b= sRectInfo.colorRange.color_s.b = u8B;
    }

    sRectInfo.dstBlock.x = 0;
    sRectInfo.dstBlock.y = 0;
    sRectInfo.dstBlock.width = pGraphicSurface->u32Width;
    sRectInfo.dstBlock.height = pGraphicSurface->u32Height;
    sRectInfo.flag = GFXRECT_FLAG_COLOR_CONSTANT;
    sRectInfo.fmt = _TranslateToGFXBufferFormat(pGraphicSurface->eFormat);


    GFX_ENTRY();

    while(MApi_GFX_BeginDraw() != 1)
    {
        MsOS_DelayTask(10);
        i++;
        if(i > 200)
        {
            printf("%s : MApi_GFX_BeginDraw GFX wait lock timeout.\n", __FUNCTION__);
            return FALSE;
        }
    }

    dstbuf.u32Addr = pGraphicSurface->u32BuffAddr;
    dstbuf.u32ColorFmt = _TranslateToGFXBufferFormat(pGraphicSurface->eFormat);
    dstbuf.u32Height = pGraphicSurface->u32Height;
    dstbuf.u32Pitch = _CalcPitch(dstbuf.u32ColorFmt, pGraphicSurface->u32Pitch);
    dstbuf.u32Width = pGraphicSurface->u32Width;

    gprintf("[PT_Graphic_Clear] dstbuf.u32Addr =0x%x, u32ColorFmt=%d, u32Height=%d, u32Width=%d, u32Pitch=%d\n",
    (unsigned int)dstbuf.u32Addr, dstbuf.u32ColorFmt, (int)dstbuf.u32Height , (int)dstbuf.u32Width, (int)dstbuf.u32Pitch);

    gfxPt0.x = sRectInfo.dstBlock.x;
    gfxPt0.y = sRectInfo.dstBlock.y;
    gfxPt1.x = sRectInfo.dstBlock.x + sRectInfo.dstBlock.width;
    gfxPt1.y = sRectInfo.dstBlock.y + sRectInfo.dstBlock.height;
    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    MApi_GFX_SetDstBufferInfo(&dstbuf, 0);
    GFX_Result result_ =MApi_GFX_RectFill(&sRectInfo);

    if (result_!=GFX_SUCCESS)
    {
        gprintf("[PT_Graphic_Clear] MApi_GFX_RectFill FAIL result_=%d\n", result_);
        MApi_GFX_EndDraw();
        GFX_RETURN(GF_FAIL);
        return FALSE;
    }
    gprintf("[PT_Graphic_Clear] MApi_GFX_RectFill Success result_=%d\n", result_);
    MApi_GFX_EndDraw();
    GFX_RETURN(GF_SUCCESS);

/*    IDirectFBSurface* pGraphicSurface = (IDirectFBSurface*)pSurface;
    DFBResult eResult = DFB_FAILURE;

    eResult = pGraphicSurface->Clear(pGraphicSurface, pstPhotoRequest->u8R, pstPhotoRequest->u8G, pstPhotoRequest->u8B, pstPhotoRequest->u8A);
    PHOTO_DFB_CHECK(eResult);

    if (eResult == DFB_OK)
        return TRUE;
    else
        return FALSE;*/
    return TRUE;
}

MMSDK_BOOL PT_Graphic_SetBlittingFlags(PT_GRAPHICITEM Surface, EN_MMSDK_ROTATE_ANGLE eAngle, MMSDK_U32 u32BlitFlags)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)Surface;
    gprintf(" [PT_Graphic_SetBlittingFlags] pGraphicSurface->eAngle =%d, u32BlitFlags =%d\n",
         eAngle, u32BlitFlags);

    IS_VALID_SURFACE(pGraphicSurface);

    pGraphicSurface->eAngle = eAngle;

/*    IDirectFBSurface* pGraphicSurface = (IDirectFBSurface*)pSurface;
    DFBSurfaceBlittingFlags eBlitFlag = DSBLIT_NOFX;
    DFBResult eResult = DFB_FAILURE;

    switch(eAngle)
    {
        case E_MMSDK_ROTATE_0:
            eBlitFlag = DSBLIT_NOFX;
            break;
        case E_MMSDK_ROTATE_90:
            eBlitFlag = DSBLIT_ROTATE90;
            break;
        case E_MMSDK_ROTATE_180:
            eBlitFlag = DSBLIT_ROTATE180;
            break;
        case E_MMSDK_ROTATE_270:
            eBlitFlag = DSBLIT_ROTATE270;
            break;
        default:
            break;
    }

    if (IsThumbnail)
    {
        eBlitFlag = DSBLIT_BLEND_COLORALPHA;
    }

    if (IsFadeInOrOut)
    {
        eBlitFlag = DSBLIT_BLEND_ALPHACHANNEL;
    }

    eResult = pGraphicSurface->SetBlittingFlags(pGraphicSurface, eBlitFlag);
    PHOTO_DFB_CHECK(eResult);

    if (eResult == DFB_OK)
        return TRUE;
    else
        return FALSE;*/
    return TRUE;
}

MMSDK_BOOL PT_Graphic_SetRenderOptions(PT_GRAPHICITEM Surface, MMSDK_U32 u32DFBSurfaceRenderOptions)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    gprintf("[PT_Graphic_SetRenderOptions]\n");

    IS_VALID_SURFACE(Surface);

/*    IDirectFBSurface* pGraphicSurface = (IDirectFBSurface*)pSurface;
    DFBResult eResult = DFB_FAILURE;

    eResult = pGraphicSurface->SetRenderOptions(pGraphicSurface, (DFBSurfaceRenderOptions)(DSRO_SMOOTH_UPSCALE | DSRO_SMOOTH_DOWNSCALE));
    PHOTO_DFB_CHECK(eResult);

    if (eResult == DFB_OK)
        return TRUE;
    else
        return FALSE;*/
    return TRUE;
}

// User should always call PT_Graphic_SetClip before calling PT_Graphic_StretchBlit
MMSDK_BOOL PT_Graphic_StretchBlit(PT_GRAPHICITEM DesSurface, PT_GRAPHICITEM SrcSurface, ST_MMSDK_RECT * SrcRect, ST_MMSDK_RECT * DesRect)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pSrcGraphicSurface = (ST_GRAPHIC_SURFACE*)SrcSurface;
    ST_GRAPHIC_SURFACE* pDesGraphicSurface = (ST_GRAPHIC_SURFACE*)DesSurface;

    GFX_BufferInfo sBltSrcInfo;
    GFX_BufferInfo dstbuf;
    GFX_DrawRect sBitInfo;
    GFX_Result ret;
    GFX_RotateAngle egfxangle;

    GFX_Point gfxPt0;
    GFX_Point gfxPt1;

    int i = 0;

    gprintf("[PT_Graphic_StretchBlit]\n");

    IS_VALID_SURFACE(pSrcGraphicSurface);
    IS_VALID_SURFACE(pDesGraphicSurface);

    memset((void*)&sBltSrcInfo, 0, sizeof(GFX_BufferInfo));
    memset((void*)&dstbuf, 0, sizeof(GFX_BufferInfo));

    GFX_ENTRY();

    while (MApi_GFX_BeginDraw() != 1)
    {
        MsOS_DelayTask(10);
        i++;
        if(i > 200)
        {
            printf("%s : MApi_GFX_BeginDraw GFX wait lock timeout.\n", __FUNCTION__);
            return FALSE;
        }
    }

    gfxPt0.x = 0;
    gfxPt0.y = 0;
    gfxPt1.x = DesRect->s32X + DesRect->u16Width;
    gfxPt1.y = DesRect->s32Y + DesRect->u16Height;
    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);


    sBltSrcInfo.u32Addr = pSrcGraphicSurface->u32BuffAddr;
    sBltSrcInfo.u32ColorFmt = _TranslateToGFXBufferFormat(pSrcGraphicSurface->eFormat);
    sBltSrcInfo.u32Height = pSrcGraphicSurface->u32Height;
    sBltSrcInfo.u32Width = pSrcGraphicSurface->u32Width;
    sBltSrcInfo.u32Pitch = _CalcPitch(sBltSrcInfo.u32ColorFmt, pSrcGraphicSurface->u32Pitch);

    //printf("\n PT_Graphic_StretchBlit pSrcGraphicSurface =0x%x", pSrcGraphicSurface);
    gprintf("[PT_Graphic_StretchBlit] src eFormat =%d, u32BuffAddr=0x%x, u32Height=%d, u32Width=%d, u32Pitch=%d\n",
    sBltSrcInfo.u32ColorFmt, (unsigned int)sBltSrcInfo.u32Addr, (int)sBltSrcInfo.u32Height, (int)sBltSrcInfo.u32Width, (int)sBltSrcInfo.u32Pitch);

    MApi_GFX_SetSrcBufferInfo(&sBltSrcInfo, 0);

    dstbuf.u32Addr = pDesGraphicSurface->u32BuffAddr;
    dstbuf.u32ColorFmt = _TranslateToGFXBufferFormat(pDesGraphicSurface->eFormat);
    dstbuf.u32Height = pDesGraphicSurface->u32Height;
    dstbuf.u32Width = pDesGraphicSurface->u32Width;
    dstbuf.u32Pitch = _CalcPitch(dstbuf.u32ColorFmt, pDesGraphicSurface->u32Pitch);

    //printf("\n PT_Graphic_StretchBlit pDesGraphicSurface =0x%x", pDesGraphicSurface);
    gprintf("[PT_Graphic_StretchBlit] dst eFormat =%d, u32BuffAddr=0x%x, u32Height=%d, u32Width=%d, u32Pitch=%d, eAngle=%d\n",
    dstbuf.u32ColorFmt, (unsigned int)dstbuf.u32Addr, (int)dstbuf.u32Height, (int)dstbuf.u32Width, (int)dstbuf.u32Pitch, pDesGraphicSurface->eAngle);


    MApi_GFX_SetDstBufferInfo(&dstbuf, 0);

    sBitInfo.srcblk.x = SrcRect->s32X;
    sBitInfo.srcblk.y = SrcRect->s32Y;
    sBitInfo.srcblk.width = SrcRect->u16Width;
    sBitInfo.srcblk.height = SrcRect->u16Height;

    sBitInfo.dstblk.x = DesRect->s32X;
    sBitInfo.dstblk.y = DesRect->s32Y;

    if (pDesGraphicSurface->eAngle == E_MMSDK_ROTATE_90 || pDesGraphicSurface->eAngle == E_MMSDK_ROTATE_270)
    {
        sBitInfo.dstblk.width = DesRect->u16Height;
        sBitInfo.dstblk.height = DesRect->u16Width;
    }
    else
    {
        sBitInfo.dstblk.width = DesRect->u16Width;
        sBitInfo.dstblk.height = DesRect->u16Height;
    }

    gprintf("[PT_Graphic_StretchBlit] src x=%d, y=%d, w=%d, h=%d\n",
    sBitInfo.srcblk.x, sBitInfo.srcblk.y, sBitInfo.srcblk.width , sBitInfo.srcblk.height);
    gprintf("[PT_Graphic_StretchBlit] dst x=%d, y=%d, w=%d, h=%d\n",
    sBitInfo.dstblk.x, sBitInfo.dstblk.y, sBitInfo.dstblk.width , sBitInfo.dstblk.height);

    MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_255, GFX_YUV_IN_255, GFX_YUV_YVYU, GFX_YUV_YVYU);

    gprintf("[PT_Graphic_StretchBlit] pSrcGraphicSurface->IsTransformed = %d\n", pSrcGraphicSurface->IsTransformed);
    if (pSrcGraphicSurface->IsAlphaBlending)
    {
        gprintf("[PT_Graphic_StretchBlit] pSrcGraphicSurface->u16Alpha=%d\n", pSrcGraphicSurface->u16Alpha);
        MApi_GFX_SetAlpha(TRUE, COEF_1_CONST, ABL_FROM_CONST, pSrcGraphicSurface->u16Alpha);
        pSrcGraphicSurface->IsAlphaBlending = FALSE;
    }
    else if (!pSrcGraphicSurface->IsTransformed)
    {
        MApi_GFX_SetAlpha(FALSE, COEF_ASRC, ABL_FROM_ASRC, 0x0);

        gprintf("[PT_Graphic_StretchBlit] sBltSrcInfo.u32ColorFmt=%d\n", sBltSrcInfo.u32ColorFmt);
        if (((dstbuf.u32ColorFmt ==GFX_FMT_YUV422) && (sBltSrcInfo.u32ColorFmt==GFX_FMT_ARGB8888 )) ||
            ((dstbuf.u32ColorFmt ==GFX_FMT_YUV422) && (sBltSrcInfo.u32ColorFmt==GFX_FMT_ARGB1555 )))
        {
            // only first time GFX_FMT_ARGB8888 from decoder to GFX_FMT_YUV422 need to switch GFX_YUV_YVYU to GFX_YUV_YUYV
            gprintf("[PT_Graphic_StretchBlit] GFX_FMT_ARGB8888 to GFX_FMT_YUV422\n");
            pDesGraphicSurface->IsTransformed = TRUE;
            MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_255, GFX_YUV_IN_255, GFX_YUV_YVYU, GFX_YUV_YUYV);
        }
        else if (((dstbuf.u32ColorFmt ==GFX_FMT_ARGB8888) && (sBltSrcInfo.u32ColorFmt==GFX_FMT_YUV422)) ||
            ((dstbuf.u32ColorFmt ==GFX_FMT_ARGB1555) && (sBltSrcInfo.u32ColorFmt==GFX_FMT_YUV422)))
        {
            gprintf("[PT_Graphic_StretchBlit] GFX_FMT_YUV422 to  GFX_FMT_ARGB8888\n");
            pDesGraphicSurface->IsTransformed = TRUE;
            //MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_255, GFX_YUV_IN_255, GFX_YUV_YVYU, GFX_YUV_YVYU);
        }
        else
        {
            gprintf("[PT_Graphic_StretchBlit] no transform... \n");
            //MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_255, GFX_YUV_IN_255, GFX_YUV_YVYU, GFX_YUV_YUYV);
        }
    }else
    {
        pDesGraphicSurface->IsTransformed = TRUE;
        MApi_GFX_SetAlpha(FALSE, COEF_ASRC, ABL_FROM_ASRC, 0x0);
    }

    MApi_GFX_SetROP2(FALSE, ROP2_OP_PS_XOR_PD);

    MsOS_FlushMemory();


    switch (pDesGraphicSurface->eAngle)
    {
        case E_MMSDK_ROTATE_0:
            egfxangle = GEROTATE_0;
            break;
        case E_MMSDK_ROTATE_90:
            egfxangle = GEROTATE_90;
            break;
        case E_MMSDK_ROTATE_180:
            egfxangle = GEROTATE_180;
            break;
        case E_MMSDK_ROTATE_270:
            egfxangle = GEROTATE_270;
            break;
        default:
            egfxangle = GEROTATE_0;
            break;
    }
    MApi_GFX_SetRotate(egfxangle);

    gprintf("[PT_Graphic_StretchBlit] MApi_GFX_SetRotate egfxangle=%d\n", egfxangle);

    ret = MApi_GFX_BitBlt(&sBitInfo, GFXDRAW_FLAG_SCALE);
    gprintf("[PT_Graphic_StretchBlit] ret val of MApi_GFX_BitBlt = %d\n", ret);

    MApi_GFX_SetRotate(GEROTATE_0);

    if (ret != GFX_SUCCESS)
    {
        gprintf("[PT_Graphic_StretchBlit] FAIL ret val of MApi_GFX_BitBlt  = %d\n", ret);

        MApi_GFX_EndDraw();
        return FALSE;
    }

    MApi_GFX_FlushQueue();
    MApi_GFX_EndDraw();

    GFX_RETURN(GF_SUCCESS);

    return TRUE;
}

MMSDK_BOOL PT_Graphic_Dump(PT_GRAPHICITEM Surface, const char * directory, const char * prefix)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    gprintf("[PT_Graphic_Dump]\n");
    return TRUE;
}

MMSDK_BOOL PT_Graphic_SetAlpha(PT_GRAPHICITEM Surface, MMSDK_U16 u16Alpha)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    ST_GRAPHIC_SURFACE* pGraphicSurface = (ST_GRAPHIC_SURFACE*)Surface;
#if 0
    int Width = 0, Height = 0;
    void *ptr;
    int  pitch;
    int x = 0, y = 0;
    U32 *pixel32;
#endif

    IS_VALID_SURFACE(pGraphicSurface);

    gprintf("[PT_Graphic_SetAlpha]  u16Alpha = %d, pGraphicSurface = %p\n", u16Alpha, pGraphicSurface);
    pGraphicSurface->u16Alpha = u16Alpha;
    pGraphicSurface->IsAlphaBlending = TRUE;

#if 0
    Width = pGraphicSurface->u32Width;
    Height = pGraphicSurface->u32Height;
    pitch = pGraphicSurface->u32Pitch;
    ptr = (void*)MsOS_PA2KSEG0(PT_MsOS_BA2PA((MS_U32)pGraphicSurface->u32BuffAddr));

    for (y=0; y<Height; y++)
    {
        pixel32 =(U32*) ((U8*)ptr + y * pitch);
        for (x=0; x<Width; x++)
        {
            if(y == 0&& x<10)
            {
                printf(" PT_Graphic_SetAlpha orig pixel32(%d,%d) = 0x%x\n", x, y, pixel32[x]);
            }

            pixel32[x] = ((pixel32[x] & 0x00FFFFFF) | ((u16Alpha << 24) & 0xFF000000) );
            if(y == 0&& x<10)
            {
                printf(" PT_Graphic_SetAlpha new pixel32(%d,%d) = 0x%x\n", x, y, pixel32[x]);
            }
        }
    }
#endif

    return TRUE;

}

MMSDK_BOOL PT_Graphic_GetSurfaceInfo(PT_GRAPHICITEM Surface, MMSDK_U32 *surface_width, MMSDK_U32 *surface_height, EN_MMSDK_COLOR_FORMAT *surface_format, void**ptr_data, MMSDK_U32 *surface_pitch)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    return TRUE;
}

MMSDK_BOOL PT_Graphic_YUV2RGB(PT_GRAPHICITEM Surface, MMSDK_U32 width, MMSDK_U32 height, MMSDK_U32 pitch, MMSDK_U8 *imgRGB)
{
    IS_DISPLAY_PHOTO_IN_MMSDK_FLOW;

    return TRUE;
}

#ifdef __cplusplus
}
#endif //__cplusplus
