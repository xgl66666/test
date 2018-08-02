#ifndef _TDAL_GFX_P_H_
#define _TDAL_GFX_P_H_


#define   GOP_WordUnit    16

typedef struct
{
    MS_U32 u32BmpDramRblkStart;
    MS_U16 bmpWidth;
    MS_U16 bmpHeight;
}DrawBmpInfo;

typedef struct
{
    EN_GOP_DST_TYPE GopDest;
    MS_U8 u8GOPId;
    MS_U8 GeWinId;
    MS_U8 u8FBId;
    MS_U16 GopBitmapWidth;
    MS_U16 GopBitmapHeight;
    MS_U16 GeDstFmt;
    DrawBmpInfo bmpInfo;
}GWinInfo;

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

typedef struct
{
    tTDAL_GFX_RegionHandle  frameBufferId;
    uint32_t                layerHandle;
    uint8_t                 alpha;
    GWINID                  GeWinId;
    bool                    used;
    tTDAL_GFX_Point         offSet;
    tTDAL_GFX_Size          size;
    bool                    deletableWin;
}tTDAL_RegionDesc;

typedef struct
{
	void * buffer;
	bool  used;
	void * alignedBuffer;
	MS_U16 fbFmt;
	uint32_t pitch;
	tTDAL_GFX_Size size;
	uint8_t  frameBufferId;
	bool  bFrameBufferUsed;
} tTDAL_GFXi_BitmapDescriptor;

typedef struct
{
    MS_S16 hBmp;
    GFX_Buffer_Format BmpClrFmt;
}BmpInfo;

#define kTDAL_GFX_REGCOUNT 32
#define BITMAP_DESCRIPTOR_COUNT     32

IMPORT tTDAL_RegionDesc    TDAL_GFX_RgnDesc[kTDAL_GFX_REGCOUNT];
IMPORT tTDAL_RegionDesc    TDAL_GFX_Scaled_RgnDesc;

IMPORT tTDAL_GFXi_BitmapDescriptor TDAL_GFXi_BitmapDescriptor[BITMAP_DESCRIPTOR_COUNT];

IMPORT GFX_PaletteEntry    _gePalette[GOP_PALETTE_ENTRY_NUM];;

__INLINE uint8_t TDAL_GFX_ycrcb2R(uint8_t bY, uint8_t bCr, uint8_t bCb);
__INLINE uint8_t TDAL_GFX_ycrcb2G(uint8_t bY, uint8_t bCr, uint8_t bCb);
__INLINE uint8_t TDAL_GFX_ycrcb2B(uint8_t bY, uint8_t bCr, uint8_t bCb);

int32_t TDAL_GFXm_FindAvailableRegionDescription(void);
tTDAL_GFX_Error TDAL_GFXm_CreateFB(uint8_t *frameBufferId, uint32_t regionDescriptor, tTDAL_GFX_Size *size, tTDAL_GFX_Point *offset, uint16_t fbFmt);
void TDAL_GFXm_InitBitmapDescriptors(void);
IMPORT tTDAL_GFX_Error TDAL_GFXm_Blit_GIF(void);
enum
{
    LAYER_OSD1 = 0,
    LAYER_OSD2 = 1,
    LAYER_OSD3 = 2
};

#define kTDAL_GFX_RESOLUTION_COLUMNS_NUM 720
#define kTDAL_GFX_RESOLUTION_ROWS_NUM 576
#define SCALEX(x) (MS_U16)(((int)(x)*(660)+kTDAL_GFX_RESOLUTION_COLUMNS_NUM/2)/kTDAL_GFX_RESOLUTION_COLUMNS_NUM)
#define SCALEY(y) (MS_U16)(((int)(y)*(556)+kTDAL_GFX_RESOLUTION_ROWS_NUM/2)/kTDAL_GFX_RESOLUTION_ROWS_NUM)

#define E_GOP   0
#define E_GOP1   1
#define BMPRGB565SIZE    (BMPRGB565W*BMPRGB565H*2ul)
#define BMPARGB4444SIZE    (BMPRGB4444W*BMPRGB4444H*2ul)
#define BMPARGB8888SIZE    (BMPRGB8888W*BMPRGB8888H*4ul)
#define BMPI8SIZE    (BMPRGBI8W*BMPRGBI8H)

#define BMPRGB565W    400ul
#define BMPRGB565H    400ul
#define BMPRGB4444W    400ul
#define BMPRGB4444H    400ul
#define BMPRGB8888W    400ul
#define BMPRGB8888H    400ul
#define BMPRGBI8W    400ul
#define BMPRGBI8H    400ul

#endif /*_TDAL_GFX_P_H_*/
