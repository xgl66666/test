/******************************************************************************
 *         COPYRIGHT 2004 IWEDIA TECHNOLOGIES         *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_GFX
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_gfx.h $
 *      $Rev: 774 $
 *      $Date: 2009-02-04 10:45:38 +0100 (mer., 04 févr. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : TDAL GFX header.
 *
 *****************************************************************************/

#ifndef _TDAL_GFX_H_
#define _TDAL_GFX_H_

#ifdef __cplusplus
extern "C"  {
#endif //__cplusplus

/*******************************************************/
/*       Defines              */
/*******************************************************/


/*******************************************************/
/*       Macros               */
/*******************************************************/


/*******************************************************/
/*       Typedefs              */
/*******************************************************/
#define   kTDAL_GFX_TRANSPARENT   0
#define   kTDAL_GFX_OPAQUE   255

typedef enum
{
   eTDAL_GFX_NO_ERROR,
   eTDAL_GFX_BAD_PARAMETER,
   eTDAL_GFX_NO_MEMORY,
   eTDAL_GFX_FEATURE_NOT_SUPPORTED,
   eTDAL_GFX_DRIVER_ERROR,
   eTDAL_GFX_UNKNOWN_REGION,
   eTDAL_GFX_UNKNOWN_ERROR
}tTDAL_GFX_Error;

typedef enum
{
   eTDAL_GFX_COLOR_CLUT_AYCRCB8888 = 0x0001,
   eTDAL_GFX_COLOR_CLUT_ARGB8888 = 0x0002,
   eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888 = 0x0004,
   eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888 = 0x0008,
   eTDAL_GFX_COLOR_TRUE_COLOR_RGB888 = 0x0010,
   eTDAL_GFX_COLOR_TRUE_COLOR_YCRCB888_422 = 0x0020,
   eTDAL_GFX_COLOR_TRUE_COLOR_YCRCB888_444 = 0x0040,
   eTDAL_GFX_COLOR_TRUE_COLOR_ARGB4444 = 0x0080,
   eTDAL_GFX_COLOR_TRUE_COLOR_ARGB1555 = 0x0100,
   eTDAL_GFX_COLOR_ALPHA_OVER_LAYER = 0x200,
    eTDAL_GFX_COLOR_TRUE_COLOR_YUV422 = 0x400
}tTDAL_GFX_ColorType;

typedef enum
{
    eTDAL_GFX_BLIT_METHOD_COPY      = 0x00,
    eTDAL_GFX_BLIT_METHOD_SCALE     = 0x01,
    eTDAL_GFX_BLIT_METHOD_MOVE      = 0x02
}tTDAL_GFX_BlitMethod;

typedef enum
{
   eTDAL_GFX_TYPE_NONE=0,
   eTDAL_GFX_TYPE_DISPLAY, /* tdal_gfx Region */
   eTDAL_GFX_TYPE_BITMAP,  /* tdal_gfx Bitmap */
   eTDAL_GFX_TYPE_MEMORY,   /* basic memory */
   eTDAL_GFX_TYPE_COLOR   /* basic memory */
}tTDAL_GFX_Type;

typedef struct
{
   int32_t   x;
   int32_t   y;
}tTDAL_GFX_Point;

typedef struct
{
   uint32_t   width;
   uint32_t   height;
}tTDAL_GFX_Size;

typedef struct
{
   tTDAL_GFX_Point     offset;
   tTDAL_GFX_Size     size;
   tTDAL_GFX_Size     refSize;
   tTDAL_GFX_ColorType   colorType;
   uint32_t      alpha;
}tTDAL_GFX_RegionConfig;

typedef struct
{
   uint8_t         dummy;
}tTDAL_GFX_Capabilities;

typedef struct
{
   tTDAL_GFX_ColorType   colorType;
   tTDAL_GFX_Size   size;
   void*      pData;
}tTDAL_GFX_Bitmap;

typedef struct
{
   uint8_t   Cb;
   uint8_t   Cr;
   uint8_t   Y;
   uint8_t   alpha;
}tTDAL_GFX_AYCrCb8888;

typedef struct
{
   uint8_t   B;
   uint8_t   G;
   uint8_t   R;
   uint8_t   alpha;
}tTDAL_GFX_ARGB8888;

typedef union
{
   uint8_t       paletteEntry;
   tTDAL_GFX_AYCrCb8888   AYCrCb8888;
   tTDAL_GFX_ARGB8888   ARGB8888;
}tTDAL_GFX_Color;

typedef struct
{
   uint32_t     nbColors;
   tTDAL_GFX_ColorType   colorType;
   tTDAL_GFX_Color   *pColor;
}tTDAL_GFX_Palette;

typedef struct 
{
    tTDAL_GFX_Type      type;
    void                *pBuffer;   /* Region handle, Bitmap handle or buffer pointer */
    tTDAL_GFX_Palette   *pPalette;
    tTDAL_GFX_ColorType colorType;
    tTDAL_GFX_Point     offset;
    tTDAL_GFX_Size      size;
    tTDAL_GFX_Point     clipOffset;
    tTDAL_GFX_Size      clipSize;

    /* Only Used for MOVE command */
    tTDAL_GFX_Point     dstOffset;
}tTDAL_GFX_BlitContext ;

typedef uint32_t tTDAL_GFX_RegionHandle;
typedef uint32_t tTDAL_GFX_BitmapHandle;

tTDAL_GFX_Error TDAL_GFX_Init(void);
tTDAL_GFX_Error TDAL_GFX_Terminate(void);
tTDAL_GFX_Error TDAL_GFX_CapabilitiesGet(tTDAL_GFX_Capabilities* pCapabilities);
const char *TDAL_GFX_APIRevisionGet(void);
const char *TDAL_GFX_PlatformRevisionGet(void);
/****************************/
tTDAL_GFX_Error TDAL_GFX_RegionCreate(tTDAL_GFX_RegionHandle *pRegionHandle,tTDAL_GFX_RegionConfig *pRegionConfig);
tTDAL_GFX_Error TDAL_GFX_RegionDestinationSet(tTDAL_GFX_RegionHandle regionHandle,uint32_t layerHandle);
tTDAL_GFX_Error TDAL_GFX_RegionConfigGet(tTDAL_GFX_RegionHandle regionHandle,   tTDAL_GFX_RegionConfig *pRegionConfig);
tTDAL_GFX_Error TDAL_GFX_RegionDelete(tTDAL_GFX_RegionHandle regionHandle);

/****************************/
tTDAL_GFX_Error TDAL_GFX_RegionShow(tTDAL_GFX_RegionHandle regionHandle);
tTDAL_GFX_Error TDAL_GFX_RegionHide (tTDAL_GFX_RegionHandle regionHandle);

/****************************/
tTDAL_GFX_Error TDAL_GFX_RegionPaletteSet (tTDAL_GFX_RegionHandle regionHandle,   tTDAL_GFX_Palette *pPalette);
tTDAL_GFX_Error TDAL_GFX_RegionPaletteGet (tTDAL_GFX_RegionHandle regionHandle,   tTDAL_GFX_Palette *pPalette);
tTDAL_GFX_Error TDAL_GFX_RegionTransparencySet (tTDAL_GFX_RegionHandle regionHandle,uint8_t alpha);

/****************************/
tTDAL_GFX_Error TDAL_GFX_BitmapCreate(tTDAL_GFX_BitmapHandle *pBitmapHandle,tTDAL_GFX_Bitmap *pBitmapConfig);
tTDAL_GFX_Error TDAL_GFX_BitmapDelete(tTDAL_GFX_BitmapHandle bitmapHandle);

/****************************/
tTDAL_GFX_Error TDAL_GFX_RectangleDraw (tTDAL_GFX_RegionHandle regionHandle,
            tTDAL_GFX_Point   offset,
            tTDAL_GFX_Size   size,
            tTDAL_GFX_ColorType   colorType,
            tTDAL_GFX_Color   color
            );
tTDAL_GFX_Error TDAL_GFX_BmpDraw (
            tTDAL_GFX_RegionHandle regionHandle,
            tTDAL_GFX_Point     offset,
            tTDAL_GFX_Bitmap   *pBitmap
            );
tTDAL_GFX_Error TDAL_GFX_BmpGet (
            tTDAL_GFX_RegionHandle regionHandle,
            tTDAL_GFX_Point   offset,
            tTDAL_GFX_Bitmap   *pBitmap
            );
/****************************/


tTDAL_GFX_Error TDAL_GFX_Blit ( tTDAL_GFX_BlitMethod blitMethod,
                          tTDAL_GFX_BlitContext src,
                          tTDAL_GFX_BlitContext dest );


tTDAL_GFX_Error TDAL_GFX_Copy2D (
            tTDAL_GFX_ColorType   colorType,
            void     *pDest,
            tTDAL_GFX_Size   destSize,
            void     *pSrc,
            tTDAL_GFX_Point srcOffset,
            tTDAL_GFX_Size   srcSize
            );
tTDAL_GFX_Error TDAL_GFX_Copy2DNonZero (
            tTDAL_GFX_ColorType   colorType,
            void   *pDest,
            tTDAL_GFX_Size   destSize,
            void   *pSrc,
            tTDAL_GFX_Point srcOffset,
            tTDAL_GFX_Size   srcSize
            );
tTDAL_GFX_Error TDAL_GFX_Fill2D (
            tTDAL_GFX_ColorType   colorType,
            tTDAL_GFX_Color   color,
            void     *pDest,
            tTDAL_GFX_Size   destSize,
            tTDAL_GFX_Point srcOffset,
            tTDAL_GFX_Size   srcSize
            );
/****************************/
tTDAL_GFX_Error TDAL_GFX_Malloc (
            uint32_t uSize,
            void **ppBuffer
            );
tTDAL_GFX_Error TDAL_GFX_Free (
            void *pBuffer
            );

#ifdef __cplusplus
}
#endif //__cplusplus
			
#endif /*_TDAL_GFX_H_*/
