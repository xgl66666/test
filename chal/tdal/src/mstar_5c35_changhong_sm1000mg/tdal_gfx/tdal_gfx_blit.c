#include <string.h>

#include "crules.h"
#include "tbox.h"

#ifdef   PERF_GFX
#include   "tkel.h"   /*   add   path   in   makefile   */
#endif

#include "tdal_common.h"
#include "tdal_disp.h"
#include "tdal_gfx.h"

#include "tdal_common.h"

#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"

#include "drvXC_IOPort.h"
#include "drvTVEncoder.h"
//Api
#include "apiGOP.h"
#include "apiXC.h"

#include "drvXC_IOPort.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiXC.h"
#include "apiGFX.h"
#include "drvTVEncoder.h"
#include "apiDAC.h"
#include "apiVDEC_EX.h"
//MApi
#include "xc/Panel.h"
#include "xc/msAPI_XC.h"
#include "xc/msAPI_VE.h"

#include "tdal_gfx_p.h"

mTBOX_SET_MODULE(eTDAL_GFX);
GLOBAL tTDAL_GFX_Palette *pTDAL_GFXi_Pallete;
GLOBAL GFX_PaletteEntry    _gePalette[GOP_PALETTE_ENTRY_NUM];
LOCAL void TDAL_GFXi_SetupGfxRectFillInfo(GFX_RectFillInfo *pGfxRectFillInfo, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight,
                                    GFX_Buffer_Format gfxBufFmt, GFX_RgbColor ColorS, GFX_RgbColor ColorE, MS_U32 u32Flag);
tTDAL_RegionDesc *TDAL_GFXm_RgnDescGet(tTDAL_GFX_RegionHandle regionHandle);
tTDAL_GFX_Color * TDAL_GFXm_GetCurPallete(uint8_t index);
tTDAL_GFXi_BitmapDescriptor TDAL_GFXi_BitmapDescriptor[BITMAP_DESCRIPTOR_COUNT];

void TDAL_GFXi_OverscanScale(void *);
void TDAL_GFXm_InitBitmapDescriptors(void)
{
	int i;

	for (i = 0; i < BITMAP_DESCRIPTOR_COUNT; i++)
	{
		memset(&TDAL_GFXi_BitmapDescriptor[i],0,sizeof(tTDAL_GFXi_BitmapDescriptor));
		TDAL_GFXi_BitmapDescriptor[i].buffer            = NULL;
		TDAL_GFXi_BitmapDescriptor[i].used              = false;
		TDAL_GFXi_BitmapDescriptor[i].frameBufferId     = -1;
		TDAL_GFXi_BitmapDescriptor[i].bFrameBufferUsed  = false;
	}
}

int32_t TDAL_GFXi_FindFreeBitmapDescriptor(void)
{
	int32_t i;
	for (i = 0; i < BITMAP_DESCRIPTOR_COUNT; i++)
	{
		if (TDAL_GFXi_BitmapDescriptor[i].used == false)
		{
			return i;
		}
	}

	return -1;
}

#define FB_ALIGN_CONSTANT 16

bool TDAL_GFXi_AllocBitmapBuffer(uint32_t size, void ** buffer, void ** alignedAddress)
{
	mTBOX_FCT_ENTER("TDAL_GFXi_AllocBitmapBuffer");

	*buffer = TDAL_Malloc(size + FB_ALIGN_CONSTANT);

	if (*buffer != NULL)
	{
		*alignedAddress = (void *)MEMALIGN(FB_ALIGN_CONSTANT, (uint32_t) *buffer);
		mTBOX_RETURN(true);
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bitmap allocation failed, size = %d\n", size));
		mTBOX_RETURN(false);
	}
}

bool TDAL_GFXi_AllocBitmapFrame(uint32_t size, void ** buffer, void ** alignedAddress, uint8_t bmpDesc, bool bIsFrameBuffer)
{
    mTBOX_FCT_ENTER("TDAL_GFXi_AllocBitmapFrame");

    if (bIsFrameBuffer)
    {
        uint8_t         frameBufferId = MApi_GOP_GWIN_GetFreeFBID();
        MS_U16          width=0, height=0, fbFmt=0;
        GOP_GwinFBAttr  DstFBInfo;

		memset(&DstFBInfo,0,sizeof(GOP_GwinFBAttr));

        if(MAX_GWIN_FB_SUPPORT >= frameBufferId)
        {
            width   = TDAL_GFXi_BitmapDescriptor[bmpDesc].size.width; 
            height  = TDAL_GFXi_BitmapDescriptor[bmpDesc].size.height; 
            fbFmt   = TDAL_GFXi_BitmapDescriptor[bmpDesc].fbFmt;

            E_GOP_API_Result Res;

            //mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]frameBufferId=0x%x width=%d height=%d fbFmt=%d\n",__FUNCTION__,__LINE__,frameBufferId, width, height, fbFmt));
            Res = MApi_GOP_GWIN_CreateFB(frameBufferId, 0, 0, width, height, fbFmt);

            /*
	            if (Res != GWIN_OK)
	            {
	                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failure\n"));
	            }

	            frameBufferId = MApi_GOP_GWIN_GetFreeFBID();
	            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "frameBufferId x width x height x fbFmt = %d x %d x %d x %d\n", frameBufferId, width, height, fbFmt));
	            Res = MApi_GOP_GWIN_CreateFB(frameBufferId, 0, 0, width, height, fbFmt);
	            if (Res != GWIN_OK)
	            {
	                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failure\n"));
	            }

	            frameBufferId = MApi_GOP_GWIN_GetFreeFBID();
	            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "frameBufferId x width x height x fbFmt = %d x %d x %d x %d\n", frameBufferId, width, height, fbFmt));
	            Res = MApi_GOP_GWIN_CreateFB(frameBufferId, 0, 0, width, height, fbFmt);
	            if (Res != GWIN_OK)
	            {
	                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failure\n"));
	            }

	            frameBufferId = MApi_GOP_GWIN_GetFreeFBID();
	            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "frameBufferId x width x height x fbFmt = %d x %d x %d x %d\n", frameBufferId, width, height, fbFmt));
	            Res = MApi_GOP_GWIN_CreateFB(frameBufferId, 0, 0, width, height, fbFmt);
	            if (Res != GWIN_OK)
	            {
	                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failure\n"));
	            }
	            */

            //MApi_GOP_GWIN_CreateFB
            if (Res == GWIN_OK)
            {
                TDAL_GFXi_BitmapDescriptor[bmpDesc].frameBufferId = frameBufferId;
                MApi_GOP_GWIN_GetFBInfo(frameBufferId, &DstFBInfo);
				//mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]addr=0x%x pitch=%d width=%d height=%d fbFmt=%d\n",__FUNCTION__,__LINE__,DstFBInfo.addr,DstFBInfo.pitch,DstFBInfo.width,DstFBInfo.height,DstFBInfo.fbFmt));
                MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0);
                MApi_GFX_FlushQueue();
                *buffer = *alignedAddress = (void*)MS_PA2KSEG1(DstFBInfo.addr);
                mTBOX_RETURN(true);
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_CreateFB: failure\n"));
                mTBOX_RETURN(false);
            }
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"no free frame buffer: frameBufferId=%d >= MAX_GWIN_FB_SUPPORT!!\n", frameBufferId));
            mTBOX_RETURN(false);
        }
    }
    else
    {
        return TDAL_GFXi_AllocBitmapBuffer(size, buffer, alignedAddress);
    }
}

void TDAL_GFXi_FreeBitmapBuffer(void * buffer)
{
	mTBOX_ASSERT(buffer != NULL);
	TDAL_Free(buffer);
}

void TDAL_GFXi_FreeBitmapFrame(void * buffer, uint32_t bitmapHandle, bool bIsFrameBuffer)
{
    mTBOX_FCT_ENTER("TDAL_GFXi_FreeBitmapFrame");
    
    if (bIsFrameBuffer)
    {
        tTDAL_GFXi_BitmapDescriptor *bmpDesc = (tTDAL_GFXi_BitmapDescriptor *)bitmapHandle; 
        if (MApi_GOP_GWIN_IsFBExist(bmpDesc->frameBufferId))
        {
            MApi_GOP_GWIN_DestroyFB(bmpDesc->frameBufferId);
        }
    }
    else
    {
        TDAL_GFXi_FreeBitmapBuffer(buffer);
    }
}
/*===========================================================================
 *
 * TDAL_GFX_RectangleDraw
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
#include <math.h>
static double color_diff(int ar, int ag, int ab, int br, int bg, int bb)
{
	int absR = ar - br;
	int absG = ag - bg;
	int absB = ab - bb;

	return sqrt((double)(absR*absR + absG*absG + absB*absB));
}

static int convert_true_color_2_index(tTDAL_GFX_Color *color, tTDAL_GFX_Palette *pallete)
{
	int i, index;
	double diff, min_diff;

	index = 0;
	for (i=0; i<pallete->nbColors; i++)
	{
		diff = color_diff(color->ARGB8888.R, color->ARGB8888.G, color->ARGB8888.B, pallete->pColor[i].ARGB8888.R, pallete->pColor[i].ARGB8888.G, pallete->pColor[i].ARGB8888.B);
		if (i == 0)
		{
			min_diff = diff;
			index = 0;
		}
		else
		{
			if (min_diff > diff)
			{
				min_diff = diff;
				index = i;
			}
		}
	}

	//printf("index %d\n", index);
	return index;
}

tTDAL_GFX_Error TDAL_GFX_RectangleDraw (tTDAL_GFX_RegionHandle regionHandle,
                   tTDAL_GFX_Point      offset,
                   tTDAL_GFX_Size       size,
                   tTDAL_GFX_ColorType  colorType,
                   tTDAL_GFX_Color      color
                   )
{
    GFX_RectFillInfo gfxFillBlock;
    GFX_BufferInfo destinationBuffer;
    GOP_GwinFBAttr DstFBInfo;
    GFX_RgbColor color_s;
    GFX_Result gfx_result = GFX_SUCCESS;
    uint16_t RectX, RectY, RectWidth, RectHeight;
    tTDAL_RegionDesc *regionDesc;
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    tTDAL_GFX_Color palleteColor;
    tTDAL_GFX_Palette *pallete = NULL;
    
    mTBOX_FCT_ENTER("TDAL_GFX_RectangleDraw");
    
    RectX       = offset.x;
    RectY       = offset.y;
    RectWidth   = size.width;
    RectHeight  = size.height;
    
    regionDesc = TDAL_GFXm_RgnDescGet(regionHandle);
    MApi_GOP_GWIN_Switch2Gwin(regionDesc->GeWinId);
    if (regionDesc == NULL)
    {
        error = eTDAL_GFX_UNKNOWN_ERROR;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GFXm_RgnDescGet: fail to find appropriate region descriptor\n"));
        mTBOX_RETURN(error);
    }

    if ((RectX >= regionDesc->size.width) || ((RectX + RectWidth) > regionDesc->size.width)
        || (RectY >= regionDesc->size.height) || ((RectY + RectHeight) > regionDesc->size.height))
    {
        return eTDAL_GFX_BAD_PARAMETER;
    }

    switch (colorType)
    {
        case eTDAL_GFX_COLOR_CLUT_ARGB8888:
            if (regionDesc->fbFmt != E_MS_FMT_I8)
            {
                pallete = (tTDAL_GFX_Palette *)TDAL_GFXm_RegionPaletteGet(regionHandle);
				if (!pallete)
				{
					return eTDAL_GFX_BAD_PARAMETER;
				}
                //mTBOX_RETURN(eTDAL_GFX_FEATURE_NOT_SUPPORTED);
			}
            break;
        case eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888:
            if (regionDesc->fbFmt != E_MS_FMT_ARGB8888)
            {
				int index;
				pallete = (tTDAL_GFX_Palette *)TDAL_GFXm_RegionPaletteGet(regionHandle);
				if (!pallete)
				{
					return eTDAL_GFX_BAD_PARAMETER;
				}
				index = convert_true_color_2_index(&color, pallete);
				color = pallete->pColor[index];
				color.paletteEntry = index;
				//mTBOX_RETURN(eTDAL_GFX_FEATURE_NOT_SUPPORTED);
            }
            break;
        default:
            mTBOX_RETURN(eTDAL_GFX_FEATURE_NOT_SUPPORTED);
            break;
    }

    //Map color index to ARGB color
    if (colorType==eTDAL_GFX_COLOR_CLUT_AYCRCB8888 || colorType==eTDAL_GFX_COLOR_CLUT_ARGB8888)
    {
        //TDAL_GFX_RegionPaletteGet(regionHandle, pallete);
        pallete = (tTDAL_GFX_Palette *)TDAL_GFXm_RegionPaletteGet(regionHandle);
        if (pallete == NULL)
        {
            memset((void*)&palleteColor, 0, sizeof(tTDAL_GFX_Color));
        }
        else
        {
            palleteColor = *(pallete->pColor + color.paletteEntry);
        }
    }
    else
    {
        palleteColor = color;
    }
    
    if (colorType==eTDAL_GFX_COLOR_CLUT_AYCRCB8888 || colorType==eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888)
    {
        color_s.a = palleteColor.AYCrCb8888.alpha;
        color_s.r = TDAL_GFX_ycrcb2R(palleteColor.AYCrCb8888.Y, palleteColor.AYCrCb8888.Cr, palleteColor.AYCrCb8888.Cb);
        color_s.g = TDAL_GFX_ycrcb2G(palleteColor.AYCrCb8888.Y, palleteColor.AYCrCb8888.Cr, palleteColor.AYCrCb8888.Cb);
        color_s.b = TDAL_GFX_ycrcb2B(palleteColor.AYCrCb8888.Y, palleteColor.AYCrCb8888.Cr, palleteColor.AYCrCb8888.Cb);
    }
    else
    {
        color_s.a = (uint8_t)(palleteColor.ARGB8888.alpha);
        color_s.r = (uint8_t)(palleteColor.ARGB8888.R);
        color_s.g = (uint8_t)(palleteColor.ARGB8888.G);
        color_s.b = (uint8_t)(palleteColor.ARGB8888.B);
    }

    //get current GWIN fbInfo.
    MApi_GOP_GWIN_GetFBInfo(regionDesc->frameBufferId, &DstFBInfo);
    
    //Set Dst buffer
    destinationBuffer.u32ColorFmt   = (MS_U8)DstFBInfo.fbFmt;
    destinationBuffer.u32Addr       = DstFBInfo.addr;
    destinationBuffer.u32Pitch      = DstFBInfo.pitch;
    MApi_GFX_SetDstBufferInfo(&destinationBuffer, 0);

    TDAL_GFXi_SetupGfxRectFillInfo(&gfxFillBlock, RectX, RectY, RectWidth, RectHeight,
            (MS_U8)DstFBInfo.fbFmt, color_s, color_s, GFXRECT_FLAG_COLOR_CONSTANT);
    if (regionDesc->fbFmt == E_MS_FMT_I8)
    {
        gfxFillBlock.colorRange.color_s.b = color.paletteEntry;
    }
    TDAL_GFXi_OverscanScale(&gfxFillBlock.dstBlock);
    gfx_result = MApi_GFX_RectFill(&gfxFillBlock);
    if (gfx_result != GFX_SUCCESS)
    {
        error = eTDAL_GFX_DRIVER_ERROR;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_RectFill: failure %d\n", gfx_result));
    }
    else
    {
        gfx_result = MApi_GFX_FlushQueue();
        mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
    }
    
    mTBOX_RETURN(error);
}


/*===========================================================================
 *
 * TDAL_GFX_BmpDraw
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_BmpDraw   (
                   tTDAL_GFX_RegionHandle   regionHandle,
                   tTDAL_GFX_Point      offset,
                   tTDAL_GFX_Bitmap      *pBitmap
                   )
{
    GFX_Result      gfx_result = GFX_SUCCESS;
    GOP_GwinFBAttr  DstFBAttr;
    GFX_BufferInfo  sourceBuffer, destinationBuffer;
    GFX_DrawRect    gfxDrawRect;
    
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    tTDAL_RegionDesc *regionDesc;
    mTBOX_FCT_ENTER("TDAL_GFX_BmpDraw");

    if (pBitmap == NULL)
    {
        return eTDAL_GFX_BAD_PARAMETER;
    }
	
    //Switch destination GWIn to GeWinId and setting a destination buffer(frame)
    regionDesc = TDAL_GFXm_RgnDescGet(regionHandle);
    if (regionDesc == NULL)
    {
        return eTDAL_GFX_UNKNOWN_REGION;
    }

    if ((offset.x >= regionDesc->size.width) || ((offset.x + pBitmap->size.width) > regionDesc->size.width)
        || (offset.y >= regionDesc->size.height) || ((offset.y + pBitmap->size.height) > regionDesc->size.height))
    {
        return eTDAL_GFX_BAD_PARAMETER;
    }

    switch (pBitmap->colorType)
    {
        case eTDAL_GFX_COLOR_CLUT_ARGB8888:
            if (regionDesc->fbFmt != E_MS_FMT_I8)
            {
                tTDAL_GFX_Palette *pallete = (tTDAL_GFX_Palette *)TDAL_GFXm_RegionPaletteGet(regionHandle);
				if (!pallete)
				{
					return eTDAL_GFX_BAD_PARAMETER;
				}
                //mTBOX_RETURN(eTDAL_GFX_FEATURE_NOT_SUPPORTED);
            }
            break;
        case eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888:
            if (regionDesc->fbFmt != E_MS_FMT_ARGB8888)
            {
				//mTBOX_RETURN(eTDAL_GFX_FEATURE_NOT_SUPPORTED);
            }
            break;
        default:
            mTBOX_RETURN(eTDAL_GFX_FEATURE_NOT_SUPPORTED);
            break;
    }

    MApi_GOP_GWIN_Switch2Gwin(regionDesc->GeWinId);
    MApi_GOP_GWIN_GetFBInfo(regionDesc->frameBufferId, &DstFBAttr);
    
    destinationBuffer.u32Addr       = DstFBAttr.addr;
    destinationBuffer.u32ColorFmt   = DstFBAttr.fbFmt;
    destinationBuffer.u32Width      = DstFBAttr.width;
    destinationBuffer.u32Height     = DstFBAttr.height;
    destinationBuffer.u32Pitch      = DstFBAttr.pitch;
    gfx_result = MApi_GFX_SetDstBufferInfo(&destinationBuffer, 0);
    mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
	if (gfx_result != GFX_SUCCESS)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d] err=%d,addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,gfx_result,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
	}
    /* setting source buffer */
    switch (pBitmap->colorType)
    {
        case eTDAL_GFX_COLOR_CLUT_ARGB8888:
        case eTDAL_GFX_COLOR_CLUT_AYCRCB8888:
            sourceBuffer.u32ColorFmt    = BMP_FMT_I8;
            sourceBuffer.u32Width       = pBitmap->size.width;
            sourceBuffer.u32Height      = pBitmap->size.height;
            sourceBuffer.u32Pitch       = pBitmap->size.width;        
            break;
        case eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888:
        case eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888:
        case eTDAL_GFX_COLOR_ALPHA_OVER_LAYER:
            sourceBuffer.u32ColorFmt    = BMP_FMT_ARGB8888;
            sourceBuffer.u32Width       = pBitmap->size.width;
            sourceBuffer.u32Height      = pBitmap->size.height;
            sourceBuffer.u32Pitch       = pBitmap->size.width<<2;
            break;
        default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Non-handled color format %d\n", pBitmap->colorType));
            break;
    }
    sourceBuffer.u32Addr = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pBitmap->pData);
    gfx_result = MApi_GFX_SetSrcBufferInfo(&sourceBuffer, 0);
    mTBOX_ASSERT(gfx_result == GFX_SUCCESS);  
    
    gfxDrawRect.srcblk.x        = 0;
    gfxDrawRect.srcblk.y        = 0;
    gfxDrawRect.srcblk.height   = sourceBuffer.u32Height;
    gfxDrawRect.srcblk.width    = sourceBuffer.u32Width;
    
    gfxDrawRect.dstblk.x        = /*regionDesc->offSet.x +*/ offset.x;
    gfxDrawRect.dstblk.y        = /*regionDesc->offSet.y +*/ offset.y;
    gfxDrawRect.dstblk.height   = sourceBuffer.u32Height;
    gfxDrawRect.dstblk.width    = sourceBuffer.u32Width;
    
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "(x,y)=(%d,%d) \n",  offset.x, offset.y));
    
    //TDAL_GFXi_OverscanScale(&gfxDrawRect);
    /// BitBlt
    gfx_result = MApi_GFX_BitBlt(&gfxDrawRect, GFXDRAW_FLAG_DEFAULT);
    if (gfx_result != GFX_SUCCESS)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_BitBlt returned error %d\n", gfx_result));
    }
    else
    {
        gfx_result = MApi_GFX_FlushQueue();
        mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
    }
    
    mTBOX_RETURN(error);
}

#ifdef GFX_DUMPING_MEMORY_2_USB
#include "MsTypes.h"
#include <cyg/fileio/fileio.h>
#include "MsFS.h"

bool usb_mounted = false;

void MountUsb()
{
    MS_S32 ret = -1;
    if (!usb_mounted)
    {
        ret = mount( "/dev/sdx/1", "/sdx1", "fatfs");
        printf("%s: mount\n", (ret == 0) ? "ok!" : "Fail!");
        if (ret == 0)
        {
        	usb_mounted = true;
        	MsFS_MkDir("/sdx1/part_a", 0777);
        }
    }
}

void DumpFileToUsb(const char * fileName, void * buff, int width, int pitch, int height)
{
	unsigned char fn[70];

	MountUsb();
	char ppm_header[50];
	memset(ppm_header, 0, sizeof(ppm_header));
	
	sprintf(fn, "/sdx1/part_a/%s", fileName);
	sprintf(ppm_header, "P6\n%d %d\n%d\n", width, height, 255);
	FILE* filePtr = NULL;
	filePtr = MsFS_Fopen(fn, "w");

	if (filePtr == NULL)
	{
		printf("Could not open file %s\n", fileName);
		return;
	}
	int i;
	int bytesWritten = 0;
	bytesWritten = MsFS_Fwrite(ppm_header, 1, strlen(ppm_header), filePtr);
    if (bytesWritten < 0)
    {
        printf("Error writing file %s\n", fileName);
    }

    int j;
	for(i=0; i < height; i++)
	{
	    for(j=0; j<width; j++)
	    {
	        bytesWritten = MsFS_Fwrite(&buff[j*4], 1, 3, filePtr);

	        if (bytesWritten < 0)
	        {
	            printf("Error writing file %s\n", fileName);
	        }
	    }
	    buff += pitch;
	}
	MsFS_Fflush(filePtr);
	MsFS_Fclose(filePtr);
}

void DumpImageToUsb(void * buff, int length, int width, int height, const char * extension)
{
	unsigned char fileName[25];
	static int number = 0;
	number++;

	sprintf(fileName, "imagealpha%d_%d_%d.%s", number, width, height, extension);
	//DumpFileToUsb(fileName, buff, length);
}

void DumpBitmapToUsb(tTDAL_GFX_BitmapHandle handle)
{
	unsigned char fileName[70];
	unsigned char * buff =  TDAL_GFXi_BitmapDescriptor[handle].alignedBuffer;
	static int number = 0;
	int i = 0;

	number++;
	uint32_t buffSize = TDAL_GFXi_BitmapDescriptor[handle].pitch * TDAL_GFXi_BitmapDescriptor[handle].size.height;

	MountUsb();
	
	sprintf(fileName, "/sdx1/image%d_%d_%d_%d", number, TDAL_GFXi_BitmapDescriptor[handle].size.width, TDAL_GFXi_BitmapDescriptor[handle].size.height, TDAL_GFXi_BitmapDescriptor[handle].fbFmt);

	FILE* filePtr = NULL;
	filePtr = MsFS_Fopen(fileName, "w");
	if (filePtr == NULL)
	{
		printf("Could not open file\n");
		return;
	}

	while (i < buffSize)
	{
		int bytesToWrite = (buffSize - i) < 512 ? buffSize - i : 512;
		size_t bytesWritten;

		bytesWritten = MsFS_Fwrite(buff, 1, bytesToWrite, filePtr);
		if (bytesWritten <= 0)
		{
			printf("Could not write to file. Exiting...\n");
			break;
		}

		i+=bytesWritten;
	}

	MsFS_Fflush(filePtr);
	MsFS_Fclose(filePtr);
}

void DumpScreenToUsb()
{
    static int name = 0;
    unsigned char fileName[100];
    GOP_GwinFBAttr  gwinFB;
    E_GOP_API_Result gop_result;

    MountUsb();

    gop_result = MApi_GOP_GWIN_GetFBInfo(TDAL_GFX_RgnDesc[0].frameBufferId, &gwinFB);
    if (gop_result == GOP_API_SUCCESS)
    {
        name++;
        sprintf(fileName, "/sdx1/screen%d_%d_%d_%d.raw", name, gwinFB.width, gwinFB.height, gwinFB.pitch);
        size_t bytesWritten;
        size_t bytesToWrite;

        FILE* filePtr = NULL;
        filePtr = MsFS_Fopen(fileName, "w");
        if (filePtr == NULL)
        {
            printf("Could not open file\n");
            return;
        }

        bytesToWrite = gwinFB.height * gwinFB.pitch;

        bytesWritten = MsFS_Fwrite(MsOS_PA2KSEG0(gwinFB.addr), 1, bytesToWrite, filePtr);

        if (bytesToWrite != bytesWritten)
        {
            printf("Error writing to file, written %d bytes, should write %d bytes\n", bytesWritten, bytesToWrite);
        }

        MsFS_Fflush(filePtr);
        MsFS_Fclose(filePtr);

    }
    else
    {
        printf("Could not MApi_GOP_GWIN_GetFBInfo, %d\n", gop_result);
    }

}
#endif /*GFX_DUMPING_MEMORY_2_USB*/

bool TDAL_GFXi_GetRegionBuffer(tTDAL_GFX_BlitContext * context, GFX_BufferInfo * bufferInfo)
{
	tTDAL_GFX_RegionHandle regionHandle;
	GOP_GwinFBAttr      bfInfo;
	E_GOP_API_Result    gop_res;
    uint32_t            i;
    uint32_t            regDescription = -1;

    
	mTBOX_FCT_ENTER("TDAL_GFXi_GetRegionBuffer");

	mTBOX_ASSERT(context->type == eTDAL_GFX_TYPE_DISPLAY);

	regionHandle = (tTDAL_GFX_RegionHandle)context->pBuffer;

	for (i = 0; i < kTDAL_GFX_REGCOUNT; i++)
	{
		if (TDAL_GFX_RgnDesc[i].used && regionHandle==(tTDAL_GFX_RegionHandle)&TDAL_GFX_RgnDesc[i])
		{
			regDescription = i;
			break;
		}
	}

	if (regDescription == -1)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_GFXi_GetRegionBuffer]   Unknown region\n"));
		mTBOX_RETURN(false);
	}
	
	gop_res = MApi_GOP_GWIN_GetFBInfo(TDAL_GFX_RgnDesc[regDescription].frameBufferId, &bfInfo);
	mTBOX_ASSERT(gop_res == GOP_API_SUCCESS);
	
	bufferInfo->u32Addr = 		bfInfo.addr;
	bufferInfo->u32ColorFmt = 	(MS_U8)bfInfo.fbFmt;
	bufferInfo->u32Width = 		bfInfo.width;
	bufferInfo->u32Height =		bfInfo.height;
	bufferInfo->u32Pitch =		bfInfo.pitch;
	//mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]fmt=%d,pitch=%d,width=%d,height=%d)\n",__FUNCTION__,__LINE__,bufferInfo->u32ColorFmt,bufferInfo->u32Pitch, bufferInfo->u32Width, bufferInfo->u32Height));

	mTBOX_RETURN(true);
}

bool TDAL_GFXi_GetBitmapBuffer(tTDAL_GFX_BlitContext * context, GFX_BufferInfo * bufferInfo)
{
	tTDAL_GFX_BitmapHandle bitmapHandle;

	mTBOX_FCT_ENTER("TDAL_GFXi_GetBitmapBuffer");

	mTBOX_ASSERT(context->type == eTDAL_GFX_TYPE_BITMAP);

	bitmapHandle = (tTDAL_GFX_BitmapHandle)context->pBuffer;

	if (bitmapHandle < 0 || bitmapHandle >= BITMAP_DESCRIPTOR_COUNT)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_GFXi_GetBitmapBuffer]  Invalid bitmap handle\n"));
		mTBOX_RETURN(false);
	}

	if (!TDAL_GFXi_BitmapDescriptor[bitmapHandle].used)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_GFXi_GetBitmapBuffer]  Unused bitmap handle\n"));
		mTBOX_RETURN(false);
	}

	bufferInfo->u32Addr = MS_VA2PA((MS_U32)TDAL_GFXi_BitmapDescriptor[bitmapHandle].alignedBuffer);
	bufferInfo->u32ColorFmt = TDAL_GFXi_BitmapDescriptor[bitmapHandle].fbFmt;
	bufferInfo->u32Height = TDAL_GFXi_BitmapDescriptor[bitmapHandle].size.height;
	bufferInfo->u32Width = TDAL_GFXi_BitmapDescriptor[bitmapHandle].size.width;
	bufferInfo->u32Pitch = TDAL_GFXi_BitmapDescriptor[bitmapHandle].pitch;

	mTBOX_TRACE((kTBOX_NIV_3, "[TDAL_GFXi_GetBitmapBuffer] colorFormat = %d, size=(%dx%d)\n", bufferInfo->u32ColorFmt, bufferInfo->u32Width, bufferInfo->u32Height));

	mTBOX_RETURN(true);

}

void TDAL_GFXi_WriteBackSourceBufferToMemory(MS_U32 buffPtr, uint32_t len)
{
    MS_U32 newAddr = (buffPtr / 16) * 16;
    MS_U32 newLen = (buffPtr - newAddr) + len;
    MS_BOOL r;

    if (MS_PA2KSEG0(MS_VA2PA(buffPtr)) != buffPtr)
    {
        mTBOX_TRACE((kTBOX_NIV_3, "Skipping writeback for address %p\n", buffPtr));
        return;
    }

    if ((newLen % 16) != 0)
    {
        newLen = (newLen / 16) * 16 + 16;
    }

    r = MsOS_Dcache_Flush(newAddr, newLen);
    mTBOX_TRACE((kTBOX_NIV_3, "Write back to cache of ptr = %p, len = %d, newBuff = %p, newLen = %d, success = %d", buffPtr, len, newAddr, newLen, r));
}

bool TDAL_GFXi_GetMemoryBuffer(tTDAL_GFX_BlitContext * context, GFX_BufferInfo * bufferInfo)
{
	void * memoryHandle;
	GFX_Result          gfx_result = GFX_SUCCESS;
	int i;
	mTBOX_FCT_ENTER("TDAL_GFXi_GetMemoryBuffer");
	mTBOX_ASSERT(context->type == eTDAL_GFX_TYPE_MEMORY);
	
	memoryHandle = context->pBuffer;
	
	switch(context->colorType)
	{
    case eTDAL_GFX_COLOR_CLUT_ARGB8888:
    case eTDAL_GFX_COLOR_CLUT_AYCRCB8888:
    	{
    	    bufferInfo->u32ColorFmt = GFX_FMT_I8;
    	    bufferInfo->u32Pitch = context->size.width;
    	    for (i = 0; i < 256; i++)
    	    {
    	        _gePalette[i].RGB.u8R = context->pPalette->pColor[i].ARGB8888.R;
    	        _gePalette[i].RGB.u8G = context->pPalette->pColor[i].ARGB8888.G;
    	        _gePalette[i].RGB.u8B = context->pPalette->pColor[i].ARGB8888.B;
    	        _gePalette[i].RGB.u8A = context->pPalette->pColor[i].ARGB8888.alpha;
    	    }
    	    gfx_result = MApi_GFX_SetPaletteOpt(_gePalette, 0, GOP_PALETTE_ENTRY_NUM - 1);
    	    if (gfx_result != GFX_SUCCESS)
    	    {
    	        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_SetPaletteOpt: error occurred %d!\n", gfx_result));
    	    }
    	}
        break;
    case eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888:
    case eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888:
    case eTDAL_GFX_COLOR_ALPHA_OVER_LAYER:
        bufferInfo->u32ColorFmt = GFX_FMT_ARGB8888;
        bufferInfo->u32Pitch = context->size.width<<2;
        break;
    default:
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Non-handled color format %d\n", context->colorType));
        mTBOX_RETURN(false);
        break;
	}

	bufferInfo->u32Addr = MS_VA2PA((MS_U32)memoryHandle);
	bufferInfo->u32Height = context->size.height;
	bufferInfo->u32Width = context->size.width;

	TDAL_GFXi_WriteBackSourceBufferToMemory((MS_U32) memoryHandle, bufferInfo->u32Height * bufferInfo->u32Pitch);

	mTBOX_RETURN(true);
}

/*===========================================================================
 *
 * TDAL_GFX_Blit
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error TDAL_GFX_Blit(tTDAL_GFX_BlitMethod   blitMethod,
                              tTDAL_GFX_BlitContext   src,
                              tTDAL_GFX_BlitContext   dest   )
{
    tTDAL_GFX_Error         error       = eTDAL_GFX_NO_ERROR;
    E_GOP_API_Result        gop_result  = GOP_API_SUCCESS;
    GFX_Result              gfx_result  = GFX_SUCCESS;
    GFX_BufferInfo          sourceBuffer, destinationBuffer;
    tTDAL_GFX_Color         *pColor = NULL;
    bool                    operationBlit = false;
	GOP_GwinFBAttr          gwinFB;
    
    mTBOX_FCT_ENTER("TDAL_GFX_Blit");

    memset(&sourceBuffer, 		0, 	sizeof(sourceBuffer));
    memset(&destinationBuffer, 	0, 	sizeof(destinationBuffer));

	switch (blitMethod)
	{
		case eTDAL_GFX_BLIT_METHOD_COPY:
		{
            switch (src.type)
            {
				case eTDAL_GFX_TYPE_MEMORY:
				{
					switch (dest.type)
					{                  
						case eTDAL_GFX_TYPE_BITMAP:
						{
                            break;
                        }
                        case eTDAL_GFX_TYPE_MEMORY:
                        {
                            break;
                        }
                        default:
                        {
                            mTBOX_TRACE((kTBOX_NIV_WARNING, "Non-supported GFX destination type\n"));
                            break;
                        }
                    }
                    break;
                }
                case eTDAL_GFX_TYPE_COLOR:
                {
                    switch (dest.type)
                    {                      
                        case eTDAL_GFX_TYPE_BITMAP:
                        {                            
                            GFX_RgbColor        color_s;
                            GFX_RectFillInfo    gfxFillBlock;
                            GOP_GwinFBAttr      DstFBAttr;
                           
                            if (src.colorType==eTDAL_GFX_COLOR_CLUT_AYCRCB8888 || src.colorType==eTDAL_GFX_COLOR_CLUT_ARGB8888)
                            {
                                pColor = TDAL_GFXm_GetCurPallete(src.pPalette->pColor->paletteEntry);

                            }
                            else if (src.colorType==eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888 || src.colorType==eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888)
                            {
                                pColor = (tTDAL_GFX_Color *)src.pBuffer;
                            }
                            else
                            {
                                error = eTDAL_GFX_BAD_PARAMETER;
                            }
                            
                            if (pColor == NULL)
                            {
                                error = eTDAL_GFX_BAD_PARAMETER;
                                mTBOX_TRACE((kTBOX_NIV_WARNING, "Empty color from palette\n"));
                                mTBOX_RETURN(error);
                            }
                            
                            if (pColor)
                            {
                                if ((src.colorType==eTDAL_GFX_COLOR_CLUT_AYCRCB8888 || src.colorType==eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888))
                                {
                                    color_s.r = TDAL_GFX_ycrcb2R(pColor->AYCrCb8888.Y, pColor->AYCrCb8888.Cr, pColor->AYCrCb8888.Cb); 
                                    color_s.g = TDAL_GFX_ycrcb2G(pColor->AYCrCb8888.Y, pColor->AYCrCb8888.Cr, pColor->AYCrCb8888.Cb);
                                    color_s.b = TDAL_GFX_ycrcb2B(pColor->AYCrCb8888.Y, pColor->AYCrCb8888.Cr, pColor->AYCrCb8888.Cb);
                                    color_s.a = pColor->AYCrCb8888.alpha;
                                }
                                else
                                {
                                    color_s.r = pColor->ARGB8888.R;
                                    color_s.g = pColor->ARGB8888.G;
                                    color_s.b = pColor->ARGB8888.B;
                                    color_s.a = pColor->ARGB8888.alpha;
                                }
                            }
                            else
                            {
                                color_s.a = color_s.r = color_s.g = color_s.b = 50; //default color
                            }

                            gop_result = MApi_GOP_GWIN_GetFBInfo(((tTDAL_GFXi_BitmapDescriptor *)dest.pBuffer)->frameBufferId, &DstFBAttr);
                            if (gop_result != GOP_API_SUCCESS)
                            {
                                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
                            } 
                            destinationBuffer.u32Addr       = DstFBAttr.addr;
                            destinationBuffer.u32ColorFmt   = DstFBAttr.fbFmt;
                            destinationBuffer.u32Height     = DstFBAttr.height;
                            destinationBuffer.u32Width      = DstFBAttr.width;
                            destinationBuffer.u32Pitch      = DstFBAttr.pitch;
                            gfx_result = MApi_GFX_SetDstBufferInfo(&destinationBuffer, 0);
							if (gfx_result != GFX_SUCCESS)
							{
								error = eTDAL_GFX_DRIVER_ERROR;
								mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]err=%d,addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,gfx_result,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
							}
							else
							{
								//mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
							}
							
                            TDAL_GFXi_SetupGfxRectFillInfo(&gfxFillBlock, dest.offset.x, dest.offset.y, dest.size.width, dest.size.height,
                                    destinationBuffer.u32ColorFmt, color_s, color_s, GFXRECT_FLAG_COLOR_CONSTANT);
                            gfx_result = MApi_GFX_RectFill(&gfxFillBlock);
                            if (gfx_result != GFX_SUCCESS)
                            {
                                error = eTDAL_GFX_DRIVER_ERROR;
                                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_RectFill: failure %d\n", gfx_result));
                            }
                            else
                            {
                                MApi_GFX_FlushQueue();
                                mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
                            }
                            break;
                        }
                        case eTDAL_GFX_TYPE_MEMORY:
                        {
                            break;
                        }
                        default:
                        {
                            mTBOX_TRACE((kTBOX_NIV_WARNING, "Non-supported GFX destination type\n"));
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    mTBOX_TRACE((kTBOX_NIV_WARNING, "Non-supported GFX src type\n"));
                    break;
                }
            }
            break;
        }
        case eTDAL_GFX_BLIT_METHOD_SCALE:
        {
        	switch (src.type)
        	{
	        	case eTDAL_GFX_TYPE_BITMAP:
					memset(&gwinFB,0,sizeof(GOP_GwinFBAttr));
					
        	        gop_result = MApi_GOP_GWIN_GetFBInfo(((tTDAL_GFXi_BitmapDescriptor *)src.pBuffer)->frameBufferId, &gwinFB);
                    if (gop_result != GOP_API_SUCCESS)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
                    }
                    
                    sourceBuffer.u32Addr        = gwinFB.addr;
                    sourceBuffer.u32ColorFmt    = gwinFB.fbFmt;
                    sourceBuffer.u32Height      = gwinFB.height;
                    sourceBuffer.u32Width       = gwinFB.width;
                    sourceBuffer.u32Pitch       = (gwinFB.pitch%8)?((gwinFB.pitch/8 + 1) * 8):(gwinFB.pitch);//gwinFB.pitch; 8bytes aglin
	        		break;
	        	case eTDAL_GFX_TYPE_MEMORY:
	        		if (TDAL_GFXi_GetMemoryBuffer(&src, &sourceBuffer) == false)
	        		{
	        			error = eTDAL_GFX_BAD_PARAMETER;
	        		}
	        		
	        		break;
	        	default:
	        		mTBOX_ASSERT(FALSE);
	        		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unsupported combination of blit method, source and destination\n"));
	        		error = eTDAL_GFX_FEATURE_NOT_SUPPORTED;
	        		break;
        	}

        	switch (dest.type)
        	{
				case eTDAL_GFX_TYPE_BITMAP:               
					memset(&gwinFB,0,sizeof(GOP_GwinFBAttr));
					
					gop_result = MApi_GOP_GWIN_GetFBInfo(((tTDAL_GFXi_BitmapDescriptor *)dest.pBuffer)->frameBufferId, &gwinFB);
					if (gop_result != GOP_API_SUCCESS)
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GOP_GWIN_GetFBInfo: failure %d\n", gop_result));
					}

					destinationBuffer.u32Addr       = gwinFB.addr;
					destinationBuffer.u32ColorFmt   = gwinFB.fbFmt;
					destinationBuffer.u32Height     = gwinFB.height;
					destinationBuffer.u32Width      = gwinFB.width;
					destinationBuffer.u32Pitch      = gwinFB.pitch;
					break;
	        	case eTDAL_GFX_TYPE_MEMORY:
					if (TDAL_GFXi_GetMemoryBuffer(&dest, &destinationBuffer) == false)
					{
						error = eTDAL_GFX_BAD_PARAMETER;
					}
					break;
				case eTDAL_GFX_TYPE_DISPLAY:
					if (TDAL_GFXi_GetRegionBuffer(&dest, &destinationBuffer) == false)
					{
						error = eTDAL_GFX_BAD_PARAMETER;
					}
					break;
				default:
					mTBOX_ASSERT(FALSE);
					mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unsupported combination of blit method, source and destination\n"));
					error = eTDAL_GFX_FEATURE_NOT_SUPPORTED;
					break;
			}
        	operationBlit = true;
        	break;
        }
        default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Error: Unknown TDAL BLIT method\n"));
            break;
    }

    if (error==eTDAL_GFX_NO_ERROR && operationBlit==true)
    {
        GFX_DrawRect drawRect;
		memset(&drawRect,0,sizeof(GFX_DrawRect));
		
        gfx_result = MApi_GFX_SetSrcBufferInfo(&sourceBuffer, 0);
        mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
		if (gfx_result != GFX_SUCCESS)
        {
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_SetSrcBufferInfo err=%d,addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n", gfx_result,sourceBuffer.u32Addr,sourceBuffer.u32Pitch,sourceBuffer.u32Height,sourceBuffer.u32Width,sourceBuffer.u32ColorFmt));
        }

        gfx_result = MApi_GFX_SetDstBufferInfo(&destinationBuffer, 0);
        mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
		if (gfx_result != GFX_SUCCESS)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d] err=%d,addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,gfx_result,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
        }
		else
		{
			//mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]addr=0x%x,pitch=0x%x,height=%d,width=%d,colorfmt=%d\n",__FUNCTION__,__LINE__,destinationBuffer.u32Addr,destinationBuffer.u32Pitch,destinationBuffer.u32Height,destinationBuffer.u32Width,destinationBuffer.u32ColorFmt));
		}

        drawRect.srcblk.x = src.clipOffset.x;
        drawRect.srcblk.y = src.clipOffset.y;
        drawRect.srcblk.height = src.clipSize.height;
        drawRect.srcblk.width = src.clipSize.width;

        drawRect.dstblk.x = dest.clipOffset.x;
        drawRect.dstblk.y = dest.clipOffset.y;
        drawRect.dstblk.height = dest.clipSize.height;
        drawRect.dstblk.width = dest.clipSize.width;

        if (dest.type == eTDAL_GFX_TYPE_DISPLAY)
        {
			/* For now this scaling is needed for proper setting
			* the so called "safe area" for graphics
			*/
            TDAL_GFXi_OverscanScale(&drawRect.dstblk);
      
            gfx_result = MApi_GFX_BitBlt(&drawRect, GFXDRAW_FLAG_SCALE);
            mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
			if (gfx_result != GFX_SUCCESS)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]MApi_GFX_BitBlt returned error %d\n",__FUNCTION__,__LINE__,gfx_result));
			}
        }
        else
        {
            /* For the text/image with transparent background */
            gfx_result = MApi_GFX_SetAlpha(TRUE, COEF_ASRC, ABL_FROM_ROP8_OVER, 0xFF);
            if (gfx_result != GFX_SUCCESS)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "MApi_GFX_SetAlpha returned error %d\n", gfx_result));
            }
			
            gfx_result = MApi_GFX_BitBlt(&drawRect, GFXDRAW_FLAG_SCALE);
            mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
			if (gfx_result != GFX_SUCCESS)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]MApi_GFX_BitBlt returned error %d\n",__FUNCTION__,__LINE__,gfx_result));
			}
        }

        if (gfx_result != GFX_SUCCESS)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_GFX_BitBlt returned error %d\n", gfx_result));
        }
        else
        {
            /* Resetting alpha blending - common case */
            MApi_GFX_SetAlpha(TRUE, COEF_ONE, ABL_FROM_ASRC, 0xFF);
            gfx_result = MApi_GFX_FlushQueue();
            mTBOX_ASSERT(gfx_result == GFX_SUCCESS);
			if (gfx_result != GFX_SUCCESS)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]MApi_GFX_FlushQueue returned error %d\n",__FUNCTION__,__LINE__,gfx_result));
			}
        }
    }

    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_BmpGet
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_BmpGet   (
                   tTDAL_GFX_RegionHandle   regionHandle,
                   tTDAL_GFX_Point      offset,
                   tTDAL_GFX_Bitmap      *pBitmap
                   )
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_GFX_BmpGet");
    
    mTBOX_RETURN(error);
}


/*===========================================================================
 *
 * TDAL_GFX_Copy2D
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_Copy2D   (
                   tTDAL_GFX_ColorType   colorType,
                   void      *pDest,
                   tTDAL_GFX_Size   dstSize,
                   void      *pSrc,
                   tTDAL_GFX_Point   srcOffset,
                   tTDAL_GFX_Size   srcSize
                 )
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_GFX_Copy2D");
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_Copy2DNonZero
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_Copy2DNonZero   (
                   tTDAL_GFX_ColorType   colorType,
                   void      *pDest,
                   tTDAL_GFX_Size   dstSize,
                   void      *pSrc,
                   tTDAL_GFX_Point   srcOffset,
                   tTDAL_GFX_Size   srcSize
                   )
{
	 tTDAL_GFX_Point   srcOffsetToCopy;
	 tTDAL_GFX_Size   srcSizeToCopy;
	 
	 tTDAL_GFX_Point   dstOffsetToCopy;
	 
	 register	uint32_t   indexHeight, indexWidth;
	 
	 char*	 dstWritePointer = (char*)pDest;
	 char*	 srcReadPointer = (char*)pSrc;
	 
	 mTBOX_FCT_ENTER("TDAL_GFX_Copy2DNonZero");
	 
	 /*   if   source	to	 copy	is	 outside   dest   ->   return	*/
	 if   ((srcOffset.x > (int32_t)dstSize.width)	||
	   (srcOffset.y > (int32_t)dstSize.height)	 ||
	   ((srcOffset.x + (int32_t)srcSize.width)	 <	 0)   ||
	   ((srcOffset.y + (int32_t)srcSize.height)   <   0))
	 {
		 return   eTDAL_GFX_NO_ERROR;
	 }
	 /*   Find	 Offset   of   src	 and   dst*/
	
	 if   (srcOffset.x	 >=   0)
	 {
		 srcOffsetToCopy.x = 0;
		 dstOffsetToCopy.x = srcOffset.x;
	 }
	 else
	 {
		 srcOffsetToCopy.x = -srcOffset.x;
		 dstOffsetToCopy.x = 0;
	 }
	
	 if   (srcOffset.y	 >=   0)
	 {
		 srcOffsetToCopy.y = 0;
		 dstOffsetToCopy.y = srcOffset.y;
	 }
	 else
	 {
		 srcOffsetToCopy.y = -srcOffset.y;
		 dstOffsetToCopy.y = 0;
	 }
	
	/*	 Find	size   of	src   and	dst*/
	 if   (srcOffset.x	 <	 0)
	 {
		 if   ((   srcOffset.x + srcSize.width)   <=   dstSize.width)
		 {
			 srcSizeToCopy.width = srcOffset.x + srcSize.width;
		 }
		 else
		 {
			 srcSizeToCopy.width = dstSize.width;
		 }
	 }
	 else
	 {
		 if   ((srcOffset.x + srcSize.width)   <=	dstSize.width)
		 {
			 srcSizeToCopy.width = srcSize.width;
		 }
		 else
		 {
			 srcSizeToCopy.width = dstSize.width   -   srcOffset.x;
		 }
	 }
	
	 if   (srcOffset.y	 <	 0)
	 {
		 if   ((   srcOffset.y + srcSize.height)   <=	dstSize.height)
		 {
			 srcSizeToCopy.height = srcOffset.y + srcSize.height;
		 }
		 else
		 {
			 srcSizeToCopy.height = dstSize.height;
		 }
	 }
	 else
	 {
		 if   ((srcOffset.y + srcSize.height)	<=	 dstSize.height)
		 {
			 srcSizeToCopy.height = srcSize.height;
		 }
		 else
		 {
			 srcSizeToCopy.height = dstSize.height	 -	 srcOffset.y;
		 }
	 }
	
	 dstWritePointer   +=	(dstOffsetToCopy.y*dstSize.width) + dstOffsetToCopy.x;
	 srcReadPointer   +=   (srcOffsetToCopy.y*srcSize.width) + srcOffsetToCopy.x;
	 
	 for   (indexHeight = 0;   indexHeight	 <	 srcSizeToCopy.height;	 indexHeight++)
	 {
		 for(indexWidth = 0;   indexWidth	<	srcSizeToCopy.width;   indexWidth++)
		 {
			 if   (*srcReadPointer != 0)
				 *dstWritePointer = *srcReadPointer;
			 
			 dstWritePointer++;
			 srcReadPointer++;
		 }
		 dstWritePointer   +=	(dstSize.width	 -	 srcSizeToCopy.width);
		 srcReadPointer   +=   (srcSize.width	-	srcSizeToCopy.width);
	 }
	
	return	 eTDAL_GFX_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_GFX_Malloc
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_Malloc   (
                   uint32_t   uSize,
                   void   **ppBuffer
                   )
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_GFX_Malloc");
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_Free
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_Free   (
                   void   *pBuffer
                   )
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_GFX_Free");
    
    mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_BitmapCreate
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_BitmapCreate(tTDAL_GFX_BitmapHandle   *pBitmapHandle, tTDAL_GFX_Bitmap   *pBitmapConfig)
{
	tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_GFX_BitmapCreate");

	uint16_t            fbFmt, fmtSize;
	int32_t             bitmapDesc;
	void * buffer = NULL;
	void * alignedBuffer = NULL;
	bool bRes;

	if (pBitmapHandle==NULL || pBitmapConfig==NULL)
	{
		error = eTDAL_GFX_BAD_PARAMETER;
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Invalid parameter(s) bmpHandle=0x%x bmpConfig=0x%x\n", pBitmapHandle, pBitmapConfig));
		mTBOX_RETURN(error);
	}

	if (pBitmapConfig->size.width == 0 || pBitmapConfig->size.height == 0)
	{
		error = eTDAL_GFX_BAD_PARAMETER;
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad bitmap size (w,h)=(%d,%d)\n"));
		mTBOX_RETURN(error);
	}

	/*aglin by skyworth test*/
	pBitmapConfig->size.width = (pBitmapConfig->size.width%2)?((pBitmapConfig->size.width/2+1)*2):(pBitmapConfig->size.width);

	//mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[%s %d]colorType = %d, width=%d height=%d)\n",__FUNCTION__,__LINE__,pBitmapConfig->colorType,pBitmapConfig->size.width,pBitmapConfig->size.height));

	bitmapDesc = TDAL_GFXi_FindFreeBitmapDescriptor();

	if (bitmapDesc < 0)
	{
		error = eTDAL_GFX_NO_MEMORY;
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No available bitmap descriptors!\n"));
		mTBOX_RETURN(error);
	}

	switch (pBitmapConfig->colorType)
	{
		case eTDAL_GFX_COLOR_CLUT_AYCRCB8888:
		case eTDAL_GFX_COLOR_CLUT_ARGB8888:
			fbFmt = E_MS_FMT_I8;
			fmtSize = 1;
			break;
		case eTDAL_GFX_COLOR_TRUE_COLOR_AYCRCB8888:
		case eTDAL_GFX_COLOR_TRUE_COLOR_ARGB8888:
			fbFmt = E_MS_FMT_ARGB8888;
			fmtSize = 4;
			break;
		default:
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unhandled color type=0x%x\n", pBitmapConfig->colorType));
			error = eTDAL_GFX_FEATURE_NOT_SUPPORTED;
			mTBOX_RETURN(error);
			break;
	}
    
	TDAL_GFXi_BitmapDescriptor[bitmapDesc].fbFmt       = fbFmt;
    TDAL_GFXi_BitmapDescriptor[bitmapDesc].pitch       = pBitmapConfig->size.width * fmtSize;
    TDAL_GFXi_BitmapDescriptor[bitmapDesc].size.width  = pBitmapConfig->size.width;
	TDAL_GFXi_BitmapDescriptor[bitmapDesc].size.height = pBitmapConfig->size.height;

	//bRes = TDAL_GFXi_AllocBitmapBuffer(pBitmapConfig->size.width * fmtSize * pBitmapConfig->size.height, &buffer, &alignedBuffer);
	bRes = TDAL_GFXi_AllocBitmapFrame(pBitmapConfig->size.width * fmtSize * pBitmapConfig->size.height, &buffer, &alignedBuffer, bitmapDesc, TRUE);
	if(bRes != true)
	{
		error = eTDAL_GFX_NO_MEMORY;
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GFXi_AllocBitmapFrame: failure!\n"));
		mTBOX_RETURN(error);
	}

	TDAL_GFXi_BitmapDescriptor[bitmapDesc].buffer           = buffer;
	TDAL_GFXi_BitmapDescriptor[bitmapDesc].alignedBuffer    = alignedBuffer;
	TDAL_GFXi_BitmapDescriptor[bitmapDesc].used             = true;

	*pBitmapHandle          = (tTDAL_GFX_BitmapHandle)&TDAL_GFXi_BitmapDescriptor[bitmapDesc];
	pBitmapConfig->pData    = alignedBuffer;
    
	mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_GFX_BitmapCreate]  frameID = %d Bitmap created 0x%x\n", TDAL_GFXi_BitmapDescriptor[bitmapDesc].frameBufferId,TDAL_GFXi_BitmapDescriptor[bitmapDesc].alignedBuffer));

	mTBOX_RETURN(error);
}

/*===========================================================================
 *
 * TDAL_GFX_BitmapDelete
 *
 * Parameters:
 *      None
 *
 * Description:
 *
 *
 * Returns:
 *      Revision
 *
 *===========================================================================*/
tTDAL_GFX_Error   TDAL_GFX_BitmapDelete(tTDAL_GFX_BitmapHandle   bitmapHandle)
{
    tTDAL_GFX_Error error = eTDAL_GFX_NO_ERROR;
    tTDAL_GFXi_BitmapDescriptor *bmpDesc = (tTDAL_GFXi_BitmapDescriptor *)bitmapHandle; 
    mTBOX_FCT_ENTER("TDAL_GFX_BitmapDelete");

    if (bitmapHandle == 0/* || bitmapHandle >= BITMAP_DESCRIPTOR_COUNT*/)
    {
    	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_GFX_BitmapDelete]   Invalid descriptor\n"));
    	mTBOX_RETURN(eTDAL_GFX_BAD_PARAMETER);
    }

    if (bmpDesc->used == false)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "The bitmap with handle %d is not used\n", bitmapHandle));
        mTBOX_RETURN(eTDAL_GFX_BAD_PARAMETER);
    }

    mTBOX_ASSERT(bmpDesc->buffer != NULL);
    TDAL_GFXi_FreeBitmapFrame(bmpDesc->buffer, bitmapHandle, TRUE);

    bmpDesc->buffer         = NULL;
    bmpDesc->alignedBuffer  = NULL;
    bmpDesc->used           = false;
    bmpDesc->frameBufferId  = -1;

    mTBOX_RETURN(error);
}

/* This is necessary for blending graphics with video
 * through video encoder. By default graphics layer is bigger than
 * video layer so we need to shrink a little bit. There are two ways
 * handling this issue, one is this and second is to change all graphical resources
 * according to graphical 'safe area'. http://en.wikipedia.org/wiki/Overscan
 */
void TDAL_GFXi_OverscanScale(void *drawRect)
{
    return;

    GFX_Block *pDrawRect = (GFX_Block *)drawRect;
    
    pDrawRect->x         = SCALEX(pDrawRect->x) + (kTDAL_GFX_RESOLUTION_COLUMNS_NUM - SCALEX(kTDAL_GFX_RESOLUTION_COLUMNS_NUM))/2;
    pDrawRect->y         = SCALEY(pDrawRect->y) + (kTDAL_GFX_RESOLUTION_ROWS_NUM - SCALEY(kTDAL_GFX_RESOLUTION_ROWS_NUM))/2;
    pDrawRect->width     = SCALEX(pDrawRect->width);
    pDrawRect->height    = SCALEY(pDrawRect->height);
}
/*********************************************************************************/

#ifdef   PERF_GFX
void   TDAL_GFXm_SysTimeStamp(void)
{

}

void   TDAL_GFXm_SysTimeStampBmpStart(void)
{
}

void   TDAL_GFXm_SysTimeStampBmpStop(void)
{
}

void   TDAL_GFXm_SysTimeStampTraceStart(void)
{
}

void   TDAL_GFXm_SysTimeStampTraceStop(void)
{
}
#endif

LOCAL void TDAL_GFXi_SetupGfxRectFillInfo(GFX_RectFillInfo *pGfxRectFillInfo, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight,
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
    pGfxRectFillInfo->colorRange.color_e.b = ColorE.b;
    //printf("## e a=%x r=%x g=%x b=%x ##\n",pGfxRectFillInfo->colorRange.color_e.a,pGfxRectFillInfo->colorRange.color_e.r,pGfxRectFillInfo->colorRange.color_e.g,pGfxRectFillInfo->colorRange.color_e.b);

    pGfxRectFillInfo->flag = u32Flag;
}
