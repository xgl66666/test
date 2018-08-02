#include <stdio.h>
#include <string.h>

#include "MsTypes.h"
#include "apiGFX.h"

#include "porting_sys.h"
#include "porting_gfx.h"

#ifdef MS_DEBUG
#define MPL_GFX_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GFX_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GFX_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MPL_GFX_TRACE(fmt, args...)     //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GFX_DBG(fmt, args...)       //printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#define MPL_GFX_ERR(fmt, args...)       printf("[%s][%d]"fmt, __FUNCTION__, __LINE__, ##args)
#endif
#define MEM_ALIGN( unit, val ) (1 + (((val) - 1) | (unit - 1)))

static MS_BOOL _MPL_GFX_CheckSeg(
    const MS_U16 *pu16Src,
    MS_U16 *pu16Dst,
    const MS_U16 u16Target
    )
{
    MS_BOOL bNeedSeg = FALSE;

    GFX_BLT_DownScaleInfo stBltInfo;

    if(MApi_GFX_GetGECaps(E_GFX_CAP_BLT_DOWN_SCALE, (MS_U32 *)&stBltInfo, 4))
    {
        return FALSE;
    }

    if(*pu16Dst > *pu16Src) //scale up case, return directly
    {
        return FALSE;
    }

    if(stBltInfo.bFullRangeSupport)
    {
        if((*pu16Src /stBltInfo.u8RangeMin)  >= u16Target)   //need segment blit
        {
            if(!(((*pu16Src%stBltInfo.u8RangeMin)==0) && ((*pu16Src /stBltInfo.u8RangeMin) == u16Target)))
            {
                bNeedSeg = TRUE;
                *pu16Dst =  (*pu16Src / stBltInfo.u8RangeMin) +1 ;
            }
        }
    }
    else    //Not fully range support, need check separately between fully and not fully (not continuous) range
    {
        stBltInfo.u8ContinuousRangeMin = 2;
        if(!
            (((*pu16Src /stBltInfo.u8ContinuousRangeMin ) < u16Target) //on fully support range
            ||(((*pu16Src /stBltInfo.u8ContinuousRangeMin ) == u16Target) && ((*pu16Src%stBltInfo.u8ContinuousRangeMin) ==0))) //match continuous range
            )
        {
            bNeedSeg = TRUE;
            MS_U8 i=0;

            //check on not fully support range
            for(i=stBltInfo.u8RangeMin ; i>stBltInfo.u8ContinuousRangeMin; )
            {
                if(((*pu16Src /i) == u16Target) && ((*pu16Src %i) == 0)) //don't need segment, match not fully support range
                {
                    bNeedSeg = FALSE;
                    break;
                }
                i = i>>1;
            }

            //find upperbound range
            if(bNeedSeg)
            {
                MS_U8 u8UpperBound = stBltInfo.u8ContinuousRangeMin;
                *pu16Dst =  (*pu16Src /u8UpperBound) +1;

                for(i=stBltInfo.u8RangeMin ; i>stBltInfo.u8ContinuousRangeMin; )
                {
                    if(((*pu16Src /i) >= u16Target) && ((*pu16Src %i) == 0))
                    {
                        u8UpperBound = i;
                        *pu16Dst =  (*pu16Src /u8UpperBound) ;
                        break;
                    }
                    i = i>>1;
                }
            }
        }
    }

    MPL_GFX_TRACE("NeedSeg (%d) : from (%d) to (%d) \n", bNeedSeg, *pu16Src, *pu16Dst);

    return bNeedSeg;
}

static MS_BOOL _MPL_GFX_GetPixUnit(GFX_Buffer_Format eFmt, MS_U8 *pu8Align)
{
    switch(eFmt)
    {
        case GFX_FMT_ARGB8888:
            *pu8Align = 4;
            break;
        case GFX_FMT_RGB565:
        case GFX_FMT_ARGB4444:
        case GFX_FMT_ARGB1555:
        case GFX_FMT_YUV422:
        //case E_MI_OSD_FMT_ARGB1555_DST:
            *pu8Align = 2;
            break;
        case GFX_FMT_I1:
        case GFX_FMT_I2:
        case GFX_FMT_I4:
        case GFX_FMT_I8:
            *pu8Align = 1;
            break;
        default:
            MPL_GFX_ERR("not defined format.\n");
            return FALSE;
    }

    return TRUE;
}

static MS_BOOL _MPL_GFX_GetDefaultBitbltParam(stMPL_GFX_BitbltParam *pstParam)
{
    static GFX_RgbColor stDummyCK = {};

    pstParam->stAlpha.bEnable = FALSE;
    pstParam->stAlpha.eAlphaSrc = ABL_FROM_ASRC;
    pstParam->stAlpha.eCoef = COEF_ADST;

    pstParam->stSrcClrKey.bEnable = FALSE;
    pstParam->stSrcClrKey.eOpMode = CK_OP_EQUAL;
    pstParam->stSrcClrKey.eFmt = GFX_FMT_ARGB8888;
    pstParam->stSrcClrKey.pvColorStart = &stDummyCK;
    pstParam->stSrcClrKey.pvColorEnd= &stDummyCK;

    pstParam->stDstClrKey.bEnable = FALSE;
    pstParam->stDstClrKey.eOpMode = CK_OP_EQUAL;
    pstParam->stDstClrKey.eFmt = GFX_FMT_ARGB8888;
    pstParam->stDstClrKey.pvColorStart = &stDummyCK;
    pstParam->stDstClrKey.pvColorEnd= &stDummyCK;

    pstParam->bIsDfbBlend = FALSE;
    pstParam->eSrcDfbBlendMode = GFX_DFB_BLD_OP_ZERO;
    pstParam->eDstDfbBlendMode = GFX_DFB_BLD_OP_ZERO;

    pstParam->eSrcYUVFmt = GFX_YUV_YVYU;
    pstParam->eDstYUVFmt = GFX_YUV_YVYU;

    memset(&pstParam->stClipRect, 0x00, sizeof(GFX_Block));

    return TRUE;
}

static MS_BOOL _MPL_GFX_SetBitbltParam(stMPL_GFX_BitbltParam *pstParam)
{
    GFX_RgbColor stDummyCK[2] = {};

    if(MApi_GFX_SetDC_CSC_FMT(
        GFX_YUV_RGB2YUV_PC,
        GFX_YUV_OUT_PC,
        GFX_YUV_IN_255,
        (GFX_YUV_422)(pstParam->eSrcYUVFmt),
        (GFX_YUV_422)(pstParam->eDstYUVFmt)) != GFX_SUCCESS)
    {
        return FALSE;
    }

    if(!pstParam->stSrcClrKey.bEnable)
    {
        pstParam->stSrcClrKey.pvColorStart = &stDummyCK[0];
        pstParam->stSrcClrKey.pvColorEnd = &stDummyCK[1];
    }

    if(MApi_GFX_SetSrcColorKey(
        pstParam->stSrcClrKey.bEnable,
        pstParam->stSrcClrKey.eOpMode,
        pstParam->stSrcClrKey.eFmt,
        pstParam->stSrcClrKey.pvColorStart,
        pstParam->stSrcClrKey.pvColorEnd) != GFX_SUCCESS)
    {
        return FALSE;
    }

    if(!pstParam->stDstClrKey.bEnable)
    {
        pstParam->stDstClrKey.pvColorStart = &stDummyCK[0];
        pstParam->stDstClrKey.pvColorEnd = &stDummyCK[1];
    }

    if(MApi_GFX_SetDstColorKey(
        pstParam->stDstClrKey.bEnable,
        pstParam->stDstClrKey.eOpMode,
        pstParam->stDstClrKey.eFmt,
        pstParam->stDstClrKey.pvColorStart,
        pstParam->stDstClrKey.pvColorEnd) != GFX_SUCCESS)
    {
        return FALSE;
    }

    if(pstParam->stPalette.pPalArray)
    {
        if(MApi_GFX_SetPaletteOpt(
            pstParam->stPalette.pPalArray,
            pstParam->stPalette.u16PalStart,
            pstParam->stPalette.u16PalEnd) != GFX_SUCCESS)
        {
            return FALSE;
        }
    }

    if(pstParam->bIsDfbBlend)
    {
        if(MApi_GFX_EnableAlphaBlending(FALSE) != GFX_SUCCESS)
        {
            return FALSE;
        }

        if(MApi_GFX_SetDFBBldOP(pstParam->eSrcDfbBlendMode, pstParam->eDstDfbBlendMode) != GFX_SUCCESS)
        {
            return FALSE;
        }

        if(MApi_GFX_EnableDFBBlending(TRUE) != GFX_SUCCESS)
        {
            return FALSE;
        }
    }
    else
    {
        if(MApi_GFX_EnableDFBBlending(FALSE) != GFX_SUCCESS)
        {
            return FALSE;
        }

        if(MApi_GFX_SetAlpha(
            pstParam->stAlpha.bEnable,
            pstParam->stAlpha.eCoef,
            pstParam->stAlpha.eAlphaSrc,
            pstParam->stAlpha.u8Const) != GFX_SUCCESS)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static MS_BOOL _MPL_GFX_GetAlign(GFX_Buffer_Format eFmt, EN_GFX_BUF_TYPE eBufType, MS_U32 *pu32Align)
{
    GFX_CapMultiPixelInfo stMultiPixelInfo;
    GFX_FmtAlignCapsInfo stAlignInfo;

    if(MApi_GFX_GetGECaps(E_GFX_CAP_MULTI_PIXEL, (MS_U32*)&stMultiPixelInfo, sizeof(GFX_CapMultiPixelInfo)) != GFX_SUCCESS)
    {
        return FALSE;
    }

    stAlignInfo.u32ColorFmt = eFmt;
    stAlignInfo.eBufferType = eBufType;
    stAlignInfo.eFmtCapsType = stMultiPixelInfo.bSupportMultiPixel ? E_GFX_FMT_CAP_MULTI_PIXEL: E_GFX_FMT_CAP_NONE;
    if(MApi_GFX_GetAlignCaps(&stAlignInfo) != GFX_SUCCESS)
    {
        return FALSE;
    }

    *pu32Align = stAlignInfo.u8BaseAlign;

    return TRUE;
}

GFX_Result MPL_GFX_ClearFrameBuffer(MS_PHYADDR StrAddr, MS_U32 length, MS_U8 ClearValue)
{
    MPL_GFX_TRACE("Entered.\n");

    if (length == 0)
    {
        MPL_GFX_ERR("Invalid length %ld\n", length);
        return GFX_INVALID_PARAMETERS;
    }

    MPL_GFX_DBG("address %ld len %ld value %d\n", StrAddr, length, ClearValue);
    return MApi_GFX_ClearFrameBuffer(StrAddr, length, ClearValue);
}

GFX_Result MPL_GFX_ClearFrameBufferByWord(MS_PHYADDR StrAddr, MS_U32 length, MS_U32 ClearValue)
{
    MPL_GFX_TRACE("Entered.\n");

    if(((length % 4) != 0) || (length == 0))
    {
        MPL_GFX_ERR("Invalid length %ld\n", length);
        return GFX_INVALID_PARAMETERS;
    }

    MPL_GFX_DBG("address %ld len %ld value %ld\n", StrAddr, length, ClearValue);
    return MApi_GFX_ClearFrameBufferByWord(StrAddr, length, ClearValue);
}

GFX_Result MPL_GFX_FlushQueue(void)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_FlushQueue();
}

GFX_Result MPL_GFX_BeginDraw(void)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_BeginDraw();
}

GFX_Result MPL_GFX_EndDraw(void)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_EndDraw();
}

GFX_Result MPL_GFX_SetClip(GFX_Point* v0, GFX_Point* v1)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((v0 == NULL) || (v1 == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_SetClip(v0, v1);
}

GFX_Result MPL_GFX_GetClip(GFX_Point* v0, GFX_Point* v1)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((v0 == NULL) || (v1 == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_GetClip(v0, v1);
}

GFX_Result MPL_GFX_SetNearestMode(MS_BOOL enable)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_SetNearestMode(enable);
}

GFX_Result MPL_GFX_EnableAlphaBlending(MS_BOOL enable)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_EnableAlphaBlending(enable);
}

GFX_Result MPL_GFX_SetAlpha(MS_BOOL enable, GFX_BlendCoef coef, GFX_AlphaSrcFrom db_abl, MS_U8 abl_const)
{
    MPL_GFX_TRACE("Entered.\n");

    MPL_GFX_DBG("enable %d coef %d db_abl %d abl_const 0x%02x\n", enable, coef, db_abl, abl_const);
    return MApi_GFX_SetAlpha(enable, coef, db_abl, abl_const);
}

GFX_Result MPL_GFX_SetPaletteOpt( GFX_PaletteEntry *pPalArray, MS_U16 u16PalStart, MS_U16 u16PalEnd)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pPalArray == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    MPL_GFX_DBG("pPalArray %p PalStart %d PalEnd %d\n", pPalArray, u16PalStart, u16PalEnd);
    return MApi_GFX_SetPaletteOpt(pPalArray, u16PalStart, u16PalEnd);
}

GFX_Result MPL_GFX_SetAlphaBlending(GFX_BlendCoef blendcoef, MS_U8 blendfactor)
{
    MPL_GFX_TRACE("Entered.\n");

    MPL_GFX_DBG("blendcoef %d blendfactor %d\n", blendcoef, blendfactor);
    return MApi_GFX_SetAlphaBlending(blendcoef, blendfactor);
}

GFX_Result MPL_GFX_SetAlphaSrcFrom(GFX_AlphaSrcFrom eMode)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_SetAlphaSrcFrom(eMode);
}

GFX_Result MPL_GFX_SetDither(MS_BOOL enable)
{
    MPL_GFX_TRACE("Entered.\n");

    return MPL_GFX_SetDither(enable);
}

GFX_Result MPL_GFX_Set_Line_Pattern(MS_BOOL enable, MS_U8 linePattern, MS_U8 repeatFactor)
{
    MPL_GFX_TRACE("Entered.\n");

    MPL_GFX_DBG("enable %d linePattern %d repeatFactor %d\n", enable, linePattern, repeatFactor);
    return MApi_GFX_Set_Line_Pattern(enable, linePattern, repeatFactor);
}

GFX_Result MPL_GFX_DrawLine(GFX_DrawLineInfo *pline)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pline == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_DrawLine(pline);
}

GFX_Result MPL_GFX_RectFill(GFX_RectFillInfo *pfillblock)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pfillblock == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_RectFill(pfillblock);
}

GFX_Result MPL_GFX_SetSrcColorKey(MS_BOOL enable, GFX_ColorKeyMode opMode,
                                      GFX_Buffer_Format fmt, void *ps_color, void *pe_color)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((ps_color == NULL) || (pe_color == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_SetSrcColorKey(enable, opMode, fmt, ps_color, pe_color);
}

GFX_Result MPL_GFX_SetDstColorKey(MS_BOOL enable, GFX_ColorKeyMode opMode,
                                      GFX_Buffer_Format fmt, void *ps_color, void *pe_color)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((ps_color == NULL) || (pe_color == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_SetDstColorKey(enable, opMode, fmt, ps_color, pe_color);
}

GFX_Result MPL_GFX_BitBlt(GFX_DrawRect *drawbuf, MS_U32 drawflag)
{
    MPL_GFX_TRACE("Entered.\n");

    if (drawbuf == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_BitBlt(drawbuf, drawflag);
}

GFX_Result MPL_GFX_BitBlt_Ex(GFX_DrawRect *ptDrawRect, stMPL_GFX_BitbltParam *ptParam)
{
    MS_U32 u32TgtSrcW;
    MS_U32 u32TgtSrcH;
    MS_U32 u32TgtSrcVAddr;
    MS_U32 u32TgtSrcPitch;
    GFX_Buffer_Format u8TgtSrcFmt;

    MS_U32 u32TgtDstW;
    MS_U32 u32TgtDstH;
    MS_U32 u32TgtDstVAddr;
    MS_U32 u32TgtDstPitch;
    GFX_Buffer_Format u8TgtDstFmt;

    MS_U32 u32TmpSrcW;
    MS_U32 u32TmpSrcH;
    MS_U32 u32TmpSrcVAddr;
    MS_U32 u32TmpSrcPitch;
    GFX_Buffer_Format u8TmpSrcFmt;

    MS_U32 u32TmpDstW;
    MS_U32 u32TmpDstH;
    MS_U32 u32TmpDstVAddr;
    MS_U32 u32TmpDstPitch;
    GFX_Buffer_Format u8TmpDstFmt;

    GFX_Block stTmpSrcRect;
    GFX_Block stTmpDstRect;

    MS_BOOL bNeedSegW = TRUE;
    MS_BOOL bNeedSegH = TRUE;

    MS_U16 u32TargetW;
    MS_U16 u32TargetH;

    MS_U32 u32ExtSrcAddr = 0;
    MS_U32 u32ExtDstAddr = 0;

    stMPL_GFX_BitbltParam stBitBltParam;

    GFX_Result eRet = GFX_SUCCESS;

    if(!ptDrawRect || !ptParam)
    {
        return eRet;
    }

/*
    if((eRet = MApi_GFX_GetBufferInfo(&stSrcBufInfo, &stDstBufInfo)) != GFX_SUCCESS)
    {
        return eRet;
    }
*/

    u32TgtSrcW  = ptParam->pstSrcBuf->u32Width;
    u32TgtSrcH  = ptParam->pstSrcBuf->u32Height;
    u32TgtSrcVAddr = ptParam->pstSrcBuf->u32Addr;
    u32TgtSrcPitch = ptParam->pstSrcBuf->u32Pitch;
    u8TgtSrcFmt = ptParam->pstSrcBuf->u32ColorFmt;

    u32TgtDstW  = ptParam->pstDstBuf->u32Width;
    u32TgtDstH  = ptParam->pstDstBuf->u32Height;
    u32TgtDstVAddr = ptParam->pstDstBuf->u32Addr;
    u32TgtDstPitch = ptParam->pstDstBuf->u32Pitch;
    u8TgtDstFmt = ptParam->pstDstBuf->u32ColorFmt;

    u32TmpSrcW  = u32TgtSrcW;
    u32TmpSrcH  = u32TgtSrcH;
    u32TmpSrcVAddr  = u32TgtSrcVAddr;
    u32TmpSrcPitch = u32TgtSrcPitch;
    u8TmpSrcFmt = u8TgtSrcFmt;

    u32TmpDstW  = u32TgtDstW;
    u32TmpDstH  = u32TgtDstH;
    u32TmpDstVAddr  = u32TgtDstVAddr;
    u32TmpDstPitch = u32TgtDstPitch;
    u8TmpDstFmt = u8TgtDstFmt;

    stTmpSrcRect = ptDrawRect->srcblk;
    stTmpDstRect = ptDrawRect->dstblk;

    //parameter check
    if((u32TgtSrcW == 0) ||(u32TgtSrcH == 0)
        ||(u32TgtDstW == 0) ||(u32TgtDstH == 0)
        ||(ptDrawRect->srcblk.width == 0) ||(ptDrawRect->srcblk.height == 0)
        ||(ptDrawRect->dstblk.width == 0) ||(ptDrawRect->dstblk.height == 0)
        )
    {
        MPL_GFX_ERR("BitBlit fail, invalied parameter : width or height = 0 \n");
        return GFX_INVALID_PARAMETERS;
    }

    u32TargetW = ptDrawRect->srcblk.width;
    u32TargetH = ptDrawRect->srcblk.height;

    do
    {
        bNeedSegW &= _MPL_GFX_CheckSeg(&(stTmpSrcRect.width), &(stTmpDstRect.width), u32TargetW);
        bNeedSegH &= _MPL_GFX_CheckSeg(&(stTmpSrcRect.height), &(stTmpDstRect.height), u32TargetH);

        if(bNeedSegW || bNeedSegH)
        {
            MS_U8 u8TmpSrcPixelSize = 0;
            MS_U32 u32TmpSrcAlign = 0;

            _MPL_GFX_GetPixUnit(u8TgtSrcFmt, &u8TmpSrcPixelSize);
            _MPL_GFX_GetAlign(u8TgtSrcFmt, E_GFX_BUF_DST, &u32TmpSrcAlign);

            u32ExtDstAddr = (MS_U32)MPL_SYS_AllocateMemory( (stTmpDstRect.width*stTmpDstRect.height*u8TmpSrcPixelSize + u32TmpSrcAlign), E_MEM_TYPE_NONCACHED);
            if(u32ExtDstAddr == 0)
            {
                eRet = GFX_FAIL;
                break;
            }

            MPL_GFX_TRACE("allocate tmp buf addr : 0x%lx\n", u32ExtDstAddr);
            u32TmpDstVAddr = MEM_ALIGN(u32TmpSrcAlign, u32ExtDstAddr);
            memset((void*)u32TmpDstVAddr, 0, (u32TmpDstW*u32TmpDstH*u8TmpSrcPixelSize));

            stTmpDstRect.x = 0;
            stTmpDstRect.y = 0;
            u32TmpDstW = stTmpDstRect.width;
            u32TmpDstH = stTmpDstRect.height;
            u32TmpDstPitch = u32TgtSrcPitch;
            u8TmpDstFmt = u8TgtSrcFmt;

            _MPL_GFX_GetDefaultBitbltParam(&stBitBltParam);
        }
        else
        {
            stTmpDstRect = ptDrawRect->dstblk;
            u32TmpDstVAddr = u32TgtDstVAddr;
            u32TmpDstW = u32TgtDstW;
            u32TmpDstH = u32TgtDstH;
            u32TmpDstPitch = u32TgtDstPitch;
            u8TmpDstFmt = u8TgtDstFmt;
#if 0
            if((pstBlitOpt->eRotate == E_MI_OSD_ROTATE_90) ||(pstBlitOpt->eRotate == E_MI_OSD_ROTATE_270))
            {
                if(pstSrcRect->u32Width >= u32TmpDstH)
                {
                    u32TmpSrcRect.u32Width = u32TmpDstH;
                    u32TmpDstRect.u32Width = u32TmpSrcH;
                    u32TmpDstRect.u32Height = u32TmpDstH;
                }

                if(pstSrcRect->u32Height >= u32TmpDstW)
                {
                    u32TmpSrcRect.u32Height= u32TmpDstW;
                    u32TmpDstRect.u32Width = u32TmpDstW;
                    u32TmpDstRect.u32Height = u32TmpSrcH;
                }
            }
#endif
            stBitBltParam = *ptParam;
        }

        GFX_DrawRect bitbltInfo;
        GFX_BufferInfo gfxSrcBuf, gfxDstBuf;
        GFX_Point v0, v1;

        //gfxSrcBuf.u32Addr = MPL_SYS_VA2PA(u32TmpSrcVAddr);
        gfxSrcBuf.u32Addr = u32TmpSrcVAddr;
        gfxSrcBuf.u32Pitch = u32TmpSrcPitch;
        gfxSrcBuf.u32Width = u32TmpSrcW;
        gfxSrcBuf.u32Height = u32TmpSrcH;
        gfxSrcBuf.u32ColorFmt = u8TmpSrcFmt ;
        if((eRet = MApi_GFX_SetSrcBufferInfo(&gfxSrcBuf, 0)) != GFX_SUCCESS)
        {
            break;
        }

        //gfxDstBuf.u32Addr = MPL_SYS_VA2PA(u32TmpDstVAddr);
        gfxDstBuf.u32Addr = u32TmpDstVAddr;
        gfxDstBuf.u32Pitch = u32TmpDstPitch;
        gfxDstBuf.u32Width = u32TmpDstW;
        gfxDstBuf.u32Height = u32TmpDstH;
        gfxDstBuf.u32ColorFmt = u8TmpDstFmt;
        if(u8TmpDstFmt == GFX_FMT_ARGB1555)
        {
            gfxDstBuf.u32ColorFmt = GFX_FMT_ARGB1555_DST;
        }

        if((eRet = MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0)) != GFX_SUCCESS)
        {
            break;
        }

        if((ptParam->stClipRect.width== 0) ||(ptParam->stClipRect.height== 0)) //default : clip rectangle = dest rect
        {
            v0.x = stTmpDstRect.x;
            v0.y = stTmpDstRect.y;
            v1.x = stTmpDstRect.width+ v0.x;
            v1.y = stTmpDstRect.height+ v0.y;
        }
        else
        {
            v0.x = ptParam->stClipRect.x;
            v0.y = ptParam->stClipRect.y;
            v1.x = ptParam->stClipRect.width+ v0.x;
            v1.y = ptParam->stClipRect.height+ v0.y;
        }

        //protect to avoid out of range
        v1.x = (v1.x > u32TmpDstW)?u32TmpDstW:v1.x;
        v1.y = (v1.y > u32TmpDstH)?u32TmpDstH :v1.y;
        MPL_GFX_TRACE("Clip range (%d, %d -> %d, %d )\n",v0.x,v0.y,v1.x,v1.y);
        if((eRet = MApi_GFX_SetClip(&v0, &v1)) != GFX_SUCCESS)
        {
            break;
        }

        bitbltInfo.srcblk = stTmpSrcRect;
        bitbltInfo.dstblk = stTmpDstRect;

        // if non-stretch bitblt, use default draw flag to enable 2P mode
        MS_U32 u32DrawFlag = 0;

        u32DrawFlag = ((stTmpSrcRect.width== stTmpDstRect.width) && (stTmpSrcRect.height== stTmpDstRect.height))?(GFXDRAW_FLAG_DEFAULT):(GFXDRAW_FLAG_SCALE);

        MPL_GFX_TRACE("bitblit from src to dst  ( %d, %d, %d, %d) -> ( %d, %d, %d, %d)\n",
            bitbltInfo.srcblk.x,bitbltInfo.srcblk.y,bitbltInfo.srcblk.width,bitbltInfo.srcblk.height,
            bitbltInfo.dstblk.x,bitbltInfo.dstblk.y,bitbltInfo.dstblk.width,bitbltInfo.dstblk.height);

        if(_MPL_GFX_SetBitbltParam(&stBitBltParam) == FALSE)
        {
            break;
        }

        MApi_GFX_SetMirror(FALSE, FALSE);
        MApi_GFX_SetRotate(GEROTATE_0);

        if((eRet = MApi_GFX_BitBlt(&bitbltInfo, u32DrawFlag)) != GFX_SUCCESS)
        {
            break;
        }

        if(u32ExtSrcAddr != 0)
        {
            MPL_SYS_FreeMemory((void*)u32ExtSrcAddr, E_MEM_TYPE_NONCACHED);
            u32ExtSrcAddr = 0;
        }

        //update value, set dst as source and loop again
        stTmpSrcRect = stTmpDstRect;
        u32TmpSrcVAddr = u32TmpDstVAddr;
        u32TmpSrcW = u32TmpDstW;
        u32TmpSrcH = u32TmpDstH;

        u32ExtSrcAddr = u32ExtDstAddr;
        u32ExtDstAddr = 0;
    }while(bNeedSegW || bNeedSegH);

    //reset to default blit setting
/*
    _MPL_GFX_GetDefaultBitbltParam(&stBitBltParam);
    if(_MPL_GFX_SetBitbltParam(&stBitBltParam) == FALSE)
    {
        return GFX_FAIL;
    }
*/
    if(u32ExtSrcAddr != 0)
    {
        MPL_SYS_FreeMemory((void*)u32ExtSrcAddr, E_MEM_TYPE_NONCACHED);
        u32ExtSrcAddr = 0;
    }

    if(u32ExtDstAddr != 0)
    {
        MPL_SYS_FreeMemory((void*)u32ExtDstAddr, E_MEM_TYPE_NONCACHED);
        u32ExtDstAddr = 0;
    }
#if 0
    //save rect for flip fun
    //last update rect (only when flip done, keep the region on last update rect)
    if(_stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].bFlipDone)
    {
        _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stLastUpdateRect = _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect;
    }

    //update update rect
    if(!_stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].bFlipDone)
    {
        _MI_OSD_UnionUpdateRect(
            &_stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect,
            &u32TmpDstRect,
            &_stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect);
    }
    else
    {
        _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect.u32X = u32TmpDstRect.u32X;
        _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect.u32Y = u32TmpDstRect.u32Y;
        _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect.u32Width = u32TmpDstRect.u32Width;
        _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].stUpdateRect.u32Height = u32TmpDstRect.u32Height;

        _stOsdSurfaceCtrl.pstSurfaceCfg[u8DstSufID].bFlipDone = FALSE;
    }
#endif

    return eRet;
}

GFX_Result MPL_GFX_SetItalic(MS_BOOL enable, MS_U8 ini_line, MS_U8 ini_dis, MS_U8 delta)
{
    MPL_GFX_TRACE("Entered.\n");

    MPL_GFX_DBG("enable %d ini_line %d ini_dis %d delta %d\n", enable, ini_line, ini_dis, delta);
    return MApi_GFX_SetItalic(enable, ini_line, ini_dis, delta);
}

GFX_Result MPL_GFX_TextOut(MS_S32 fhandle, MS_U8 *pindex, MS_U32 strwidth, GFX_TextOutInfo *pfmt)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((pindex == NULL) || (pfmt == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_TextOut(fhandle, pindex, strwidth, pfmt);
}

GFX_Result MPL_GFX_CharacterOut(GFX_CharInfo *pChar, GFX_TextOutInfo *pfmt)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((pChar == NULL) || (pfmt == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_CharacterOut(pChar, pfmt);
}

GFX_Result MPL_GFX_SetIntensity(MS_U32 id, GFX_Buffer_Format fmt, MS_U32 *pColor)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pColor == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_SetIntensity(id, fmt, pColor);
}

GFX_Result MPL_GFX_GetIntensity(MS_U32 id, MS_U32 *pColor)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pColor == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_GetIntensity(id, pColor);
}

GFX_Result MPL_GFX_QueryTextDispLength(MS_S32 fhandle, MS_U8 *pu8index, MS_U32 u32strwidth,
                                               GFX_TextOutInfo *pfmt, MS_U32 *pu32DispLength)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((pu8index == NULL) || (pfmt == NULL) || (pu32DispLength == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_QueryTextDispLength(fhandle, pu8index, u32strwidth, pfmt, pu32DispLength);
}

GFX_Result MPL_GFX_DrawBitmap(MS_S32 handle, GFX_DrawBmpInfo *pbmpfmt)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pbmpfmt == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_DrawBitmap(handle, pbmpfmt);
}

GFX_Result MPL_GFX_SetSrcBufferInfo(GFX_BufferInfo *bufInfo, MS_U32 offsetofByte)
{
    MPL_GFX_TRACE("Entered.\n");

    if (bufInfo == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_SetSrcBufferInfo(bufInfo, offsetofByte);
}

GFX_Result MPL_GFX_SetDstBufferInfo(GFX_BufferInfo *bufInfo, MS_U32 offsetofByte)
{
    MPL_GFX_TRACE("Entered.\n");

    if (bufInfo == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_SetDstBufferInfo(bufInfo, offsetofByte);
}

GFX_Result MPL_GFX_EnableDFBBlending(MS_BOOL enable)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_EnableDFBBlending(enable);
}

GFX_Result MPL_GFX_SetDFBBldOP(GFX_DFBBldOP gfxSrcBldOP, GFX_DFBBldOP gfxDstBldOP)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_SetDFBBldOP(gfxSrcBldOP, gfxDstBldOP);
}

GFX_Result MPL_GFX_GetGECaps(EN_GFX_CAPS eCapType, MS_U32 *pRet, MS_U32 ret_size)
{
    MPL_GFX_TRACE("Entered.\n");

    if (pRet == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    MPL_GFX_DBG("eCapType %d ret_size %ld\n", eCapType, ret_size);
    return MApi_GFX_GetGECaps(eCapType, pRet, ret_size);
}

GFX_Result MPL_GFX_GetBufferInfo(PGFX_BufferInfo srcbufInfo, PGFX_BufferInfo dstbufInfo)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_GetBufferInfo(srcbufInfo, dstbufInfo);
}

GFX_Result MPL_GFX_SetAlpha_ARGB1555(MS_U8 coef)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_SetAlpha_ARGB1555(coef);
}

GFX_Result MPL_GFX_GetAlpha_ARGB1555(MS_U8 *coef)
{
    MPL_GFX_TRACE("Entered.\n");

    if (coef == NULL)
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    return MApi_GFX_GetAlpha_ARGB1555(coef);
}

GFX_Result MPL_GFX_RectFrame(GFX_RectFillInfo *pblock, GFX_DrawLineInfo *plinefmt)
{
    MPL_GFX_TRACE("Entered.\n");

    if ((pblock == NULL) || (plinefmt == NULL))
    {
        MPL_GFX_ERR("Invalid parameter\n");
        return GFX_INVALID_PARAMETERS;
    }

    MApi_GFX_RectFrame(pblock, plinefmt);
    return GFX_SUCCESS;
}

GFX_Result MPL_GFX_SetPatchMode(MS_BOOL enable)
{
    MPL_GFX_TRACE("Entered.\n");

    return MApi_GFX_SetPatchMode(enable);
}
