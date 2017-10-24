/******************************************************************************
 *            COPYRIGHT 2006 IWEDIA TECHNOLOGIES       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DISP
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_disp.h $
 *      $Rev: 774 $
 *      $Date: 2009-02-04 10:45:38 +0100 (mer., 04 févr. 2009) $
 *
 * Description  : Display  library primitives header.
 *
 *****************************************************************************/

#ifndef _TDAL_DISP_H_
#define _TDAL_DISP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------*/
/*     Public types definition   */
/*--------------------------------------*/
/* This constant is used to define a special value of blender z-order list entry.
   It is used to indicate that an entry in the list must be ignored.
   Note:
   When used, this special value allows to ignore the positions in the z-order list
   entries where layers have hardware fixed positions.
*/
#define kTDAL_DISP_BLENDER_ZORDER_LIST_ENTRY_NULL (-1)
/* This constant is used to define a special value of blender z-order list entry.
   It is used to indicate that an entry in the list must be removed from blender.
*/
#define kTDAL_DISP_BLENDER_ZORDER_LIST_ENTRY_REMOVE (-2)

/* This constant is used to define pixel data encoded in 32 bit with 8 bit
   for Alpha, Red, Green and Blue components.
   In a 32 bit integer, the pixel is mapped as follow:
   Bits   Map
  [31..24]  Alpha
  [23..16]  Red
  [15..8]   Green
  [7..0]   Blue
*/
#define kTDAL_DISP_PIXEL_FORMAT_ARGB8888 0x00000002

/* This constant is used to define pixel data encoded in 32 bit with 8 bit
   for Alpha, Luminance (Y), Chrominance (U-V) components.
   In a 32 bit integer, the pixel is mapped as follow:
   Bits   Map
   [31..24] Alpha
   [23..16] Y
   [15..8]  U
   [7..0]   V
*/
#define kTDAL_DISP_PIXEL_FORMAT_AYUV8888 0x00000004

/* This constant is used to define pixel data encoded in 24 bit with 8 bit
  for Red, Green and Blue components.
  In a 32 bit integer, the pixel is mapped as follow:
  Bits   Map
  [31..24] Red
  [23..16] Green
  [15..8]  Blue
  [7..0]   unused
*/
#define kTDAL_DISP_PIXEL_FORMAT_RGB888 0x00000008

/* This constant is used to define pixel data encoded in 24 bit with 8 bit
   for Luminance (Y), Chrominance (U-V) components.
   In a 32 bit integer, the pixel is mapped as follow:
   Bits   Map
   [31..24] Y
   [23..16] U
   [15..8]  V
   [7..0]   unused
*/
#define kTDAL_DISP_PIXEL_FORMAT_YUV888_444 0x00000010

/* This constant is used to define 2 pixels data encoded in 32 bit with 8 bit
   for Luminance (Y), Chrominance (U-V) components.
   In a 32 bit integer, the pixel is mapped as follow:
   Bits   Map
   [31..24] Y of 2nd pixel
   [23..16] U
   [15..8]  Y of 1st pixel
   [7..0]   V
*/
#define kTDAL_DISP_PIXEL_FORMAT_YUV888_422 0x00000020

/* This constant is used to define pixel data encoded in 16 bit with 4 bit
   for Red, Green and Blue components.
   In a 32 bit integer, the pixel is mapped as follow:
   Bits   Map
   [31..24] Unused
   [23..16] Unused
   [15..12] Alpha
   [11..8]  Red
   [7..4]   Green
   [3..0]   Blue
*/
#define kTDAL_DISP_PIXEL_FORMAT_ARGB4444 0x00000040

/* This constant is used to define pixel data encoded in 16 bit with 5 bit
   for red and blue components and 6 bit for green component.
   In a 32 bit integer, the pixel is mapped as follow:
   Bits   Map
   [31..24] Unused
   [23..16] Unused
   [15..11] Red
   [10..5]  Green
   [4..0]   Blue
*/
#define kTDAL_DISP_PIXEL_FORMAT_RGB565 0x00000080

typedef enum
{
   eTDAL_DISP_ASPECT_RATIO_4_3,   /* 4/3 aspect ratio */
   eTDAL_DISP_ASPECT_RATIO_16_9,  /* 16/9 aspect ratio */
   eTDAL_DISP_ASPECT_RATIO_221_1, /* 22.1/1 aspect ratio */
   eTDAL_DISP_ASPECT_RATIO_1_1   /* 1/1 aspect ratio */
} tTDAL_DISP_AspectRatio;

typedef enum
{
   eTDAL_DISP_ASPECT_RATIO_CONV_FULL_SCREEN,    /* Full screen conv. */
   eTDAL_DISP_ASPECT_RATIO_CONV_16_9_LETTER_BOX,   /* 16/9 letter box conv. */
   eTDAL_DISP_ASPECT_RATIO_CONV_14_9_LETTER_BOX,   /* 14/9 letter box conv. */
   eTDAL_DISP_ASPECT_RATIO_CONV_4_3_CENTER_CUT_OUT, /* 4/3 center cut out conv. */
   eTDAL_DISP_ASPECT_RATIO_CONV_4_3_PILLAR_BOX,   /* 4/3 pillar box conv. */
   eTDAL_DISP_ASPECT_RATIO_CONV_IGNORE       /* No aspect ratio conv.*/
} tTDAL_DISP_AspectRatioConv;

typedef enum
{
   eTDAL_DISP_NO_ERROR,      /* No Error returned */
   eTDAL_DISP_NOT_INIT_ERROR,   /* TDAL_DISP has been initialised */
   eTDAL_DISP_ALREADY_INIT_ERROR,  /* TDAL_DISP already initialised */
   eTDAL_DISP_BAD_PARAMETER_ERROR, /* Bad parameter given to the function */
   eTDAL_DISP_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_DISP_NO_MEMORY_ERROR,   /* Not enough memory to perform action */
   eTDAL_DISP_NOT_AVAILABLE_ERROR  /* feature not available */
} tTDAL_DISP_Error;

typedef enum
{
   eTDAL_DISP_BLENDER_ID_0, /* Blender identifier 0 */
   eTDAL_DISP_BLENDER_ID_1, /* Blender identifier 1 */
   eTDAL_DISP_BLENDER_ID_2, /* Blender identifier 2 */
   eTDAL_DISP_BLENDER_ID_3  /* Blender identifier 3 */
} tTDAL_DISP_BlenderId;

typedef enum
{
   eTDAL_DISP_LAYER_VIDEO_ID_0 = 0x00000000,  /* Layer video identifier 0 */
   eTDAL_DISP_LAYER_VIDEO_ID_1,  /* Layer video identifier 1 */
   eTDAL_DISP_LAYER_VIDEO_ID_2,  /* Layer video identifier 2 */
   eTDAL_DISP_LAYER_VIDEO_ID_3,  /* Layer video identifier 3 */
   /* ... */
   eTDAL_DISP_LAYER_GFX_ID_0 = 0x0000010,   /* Layer graphics identifier 0 */
   eTDAL_DISP_LAYER_GFX_ID_1,   /* Layer graphics identifier 1 */
   eTDAL_DISP_LAYER_GFX_ID_2,   /* Layer graphics identifier 2 */
   eTDAL_DISP_LAYER_GFX_ID_3,   /* Layer graphics identifier 3 */
   /* ... */
   eTDAL_DISP_LAYER_STILL_ID_0 = 0x0000020,  /* Layer still-picture identifier 0 */
   eTDAL_DISP_LAYER_STILL_ID_1,  /* Layer still-picture identifier 1 */
   /* ... */
   eTDAL_DISP_LAYER_CURSOR_ID_0 = 0x0000030, /* Layer cursor identifier 0 */
   eTDAL_DISP_LAYER_CURSOR_ID_1  /* Layer cursor identifier 1 */
   /* ... */
} tTDAL_DISP_LayerId;

typedef struct
{
   uint8_t nbBlender; /* Number of different blenders that can be controlled. */
} tTDAL_DISP_Capability;

typedef struct
{
   uint16_t widthMax;           /* Maximum width (in pixels) of the blender layer disposal. */
   uint16_t heightMax;          /* Maximum height (in lines) of the blender layer disposal. */
   bool   bBackgroundColor;       /* FALSE: no background colour supported on this blender.
                     TRUE: background colour supported. */
   uint32_t backgroundColorPixelFormatMask; /* Bitfield where pixel formats are set to indicate the supported
                     background pixel format.
                     This field is only relevant if bBackgroundColor is TRUE. */
   uint8_t  nbLayer;            /* Number of different layers which can be added in the z-order
                     list of this blender. */
} tTDAL_DISP_BlenderCapability;

typedef struct
{
   tTDAL_DISP_LayerId LayerId;     /* Layer identifier.*/
   uint32_t      ZOrderPositions; /* Bitmask where each bit set to 1 indicates where the layer
                   can be positioned in the z-order list. */
} tTDAL_DISP_BlenderZOrderPositions;

typedef struct
{
   uint32_t PixelFormat; /* The pixel colour format coded in Color field. It is one of kTDAL_DISP_PIXEL_FORMAT_XXX
             indicated in the backgoundColorPixelFormatMask field of the blender capabilities. */
   uint32_t Color;    /* The background colour value coded with the format indicated in the PixelFormat field.
             See kTDAL_DISP_PIXEL_FORMAT_XXX description of the pixel mapping in a 32 bit integer. */
} tTDAL_DISP_BlenderBackgroundColor;

typedef struct
{
   int32_t NbRegionMax;         /* Maximum number of regions that the layer is able to display.
                     If the number is unknown or is only limited by memory amount
                     available in the system, (-1) is indicated. */
   bool   IsMultiRegionOnSameLineCapable; /* It is only relevant if NbRegionMax > 1.
                     TRUE if the layer is able to display more than one region in a same line.
                     FALSE otherwise. */
   bool   IsInputWindowCapable;      /* TRUE if input windowing is supported by the layer. FALSE otherwise. */
   bool   IsOutputWindowCapable;     /* TRUE if output windowing is supported by the layer. FALSE otherwise. */
   bool   IsGlobalTransparencyCapable;   /* TRUE if the layer is able to perform a global transparency. */
   bool   IsAspectRatioWindowConvCapable; /* TRUE if TDAL_DISP implementation is capable of resizing output window
                     upon input window aspect ratio information.
                     In this case, the TDAL_DISP client is allowed to call
                     TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(-). */
}tTDAL_DISP_LayerCapability;

typedef struct
{
   uint32_t Left;   /* Left position of the window rectangle (in pixels). */
   uint32_t Top;   /* Top position of the window rectangle (in lines). */
   uint32_t Right;  /* Right position of the window rectangle (in pixels). */
   uint32_t Bottom; /* Bottom position of the window rectangle (in lines). */
} tTDAL_DISP_LayerWindow;

typedef struct
{
   int32_t sLeft;   
   int32_t sTop;   
   uint32_t uiRight;  
   uint32_t uiBottom; 
} tTDAL_DISP_BlenderWindow;
/*------------------------------------------*/
/*    Public Functions Declarations   */
/*------------------------------------------*/

/* Generic */
IMPORT tTDAL_DISP_Error TDAL_DISP_Init (void);
IMPORT tTDAL_DISP_Error TDAL_DISP_Term (void);
IMPORT tTDAL_DISP_Error TDAL_DISP_CapabilityGet(tTDAL_DISP_Capability *pstCapability);
IMPORT const char *   TDAL_DISP_APIRevisionGet(void );
IMPORT const char *   TDAL_DISP_PlatformRevisionGet(void );
/* Blender */
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderCapabilityGet(tTDAL_DISP_BlenderId Id,
                        tTDAL_DISP_BlenderCapability *pstCapability);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderBackgroundColorSet(tTDAL_DISP_BlenderId Id,
                          tTDAL_DISP_BlenderBackgroundColor *pstColor);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderDimensionsGet(tTDAL_DISP_BlenderId Id,
                        uint32_t *pWidth,
                        uint32_t *pHeight);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderDimensionsSet(tTDAL_DISP_BlenderId Id,
                        uint32_t Width,
                        uint32_t Height);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderDimensionsOutputSet(tTDAL_DISP_BlenderId Id,
                         tTDAL_DISP_BlenderWindow *pstOutputWindow);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderDisable(tTDAL_DISP_BlenderId Id);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderEnable(tTDAL_DISP_BlenderId Id);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderZOrderPositionsGet(tTDAL_DISP_BlenderId Id,
                          tTDAL_DISP_BlenderZOrderPositions *pstZOrderList);
IMPORT tTDAL_DISP_Error TDAL_DISP_BlenderZOrderListSet(tTDAL_DISP_BlenderId Id,
                        tTDAL_DISP_LayerId *pstZOrderList);

/* Layer */
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerCapabilityGet(tTDAL_DISP_LayerId Id,
                       tTDAL_DISP_LayerCapability *pstCapability);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerGlobalTransparencySet(tTDAL_DISP_LayerId Id,
                           uint8_t Transparency);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerHandleGet(tTDAL_DISP_LayerId Id,
                     void *pHandle);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsAdjust(tTDAL_DISP_LayerId Id,
                        tTDAL_DISP_LayerWindow *pstInputWindow,
                        tTDAL_DISP_LayerWindow *pstOutputWindow,
                        bool *pbAdjusted);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsAspectRatioParamsSet(tTDAL_DISP_LayerId Id,
                              tTDAL_DISP_LayerWindow *pstInputWindow,
                              tTDAL_DISP_AspectRatio InputAspectRatio,
                              uint32_t InputAFD,
                              tTDAL_DISP_AspectRatioConv AspectRatioConv);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsGet(tTDAL_DISP_LayerId Id,
                       tTDAL_DISP_LayerWindow *pstInputWindow,
                       tTDAL_DISP_LayerWindow *pstOutputWindow);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerIOWindowsSet(tTDAL_DISP_LayerId layerId,
                       tTDAL_DISP_LayerWindow *pstInputWindow,
                       tTDAL_DISP_LayerWindow *pstOutputWindow);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerDisable(tTDAL_DISP_LayerId Id);
IMPORT tTDAL_DISP_Error TDAL_DISP_LayerEnable(tTDAL_DISP_LayerId Id);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DISP_H_*/
