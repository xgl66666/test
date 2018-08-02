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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_GPD.c
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for audio functions
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_GPD_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>

#include "MsMemory.h"
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsOS.h"
#include "apiGPD.h"

#include "demo_gpd.h"
#include "demo_osd.h"
#include "demo_utility.h"
#include "drvDTC.h"


//----------------------------------------------------------------------------------------
//Local definitoin
//----------------------------------------------------------------------------------------
#define GPD_DBG            0
#define COLOR_BUFFER_SIZE       (1024 * 1024)       // CBUFFER_SIZE
#define DEPTH_BUFFER_SIZE       (1024 * 1024)     // ZBUFFER_SIZE
#define MAX_FILENAME_LENGTH 1024
#define RESERVE_SPACE 32

#define GPD_SUCCESS     0
#define GPD_FAILED      -1

#if GPD_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

#define GET_PIXEL_BYTE(_outputColor, _pixelByte) \
    do{ \
        switch(_outputColor)       \
        {   case ARGB8888: _pixelByte = 4; break;  \
            case RGB565:    \
            case YUV422:    \
            case YVYU422:   \
            case GRAY_ALPHA: _pixelByte = 2; break;  \
            case GRAY:  _pixelByte = 1; break;   \
            default:    _pixelByte = 0; break;  \
        }   \
    }while(0)

#define ALIGNMENT(_offset, _align)    ( (_offset + (_align - 1)) & ~(_align - 1) )

//----------------------------------------------------------------------------------------
//Global Variable
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Local Variable
//----------------------------------------------------------------------------------------
static gpd_pic_info _gstGIfPicInfo;
static gpd_pic_info _gstPngPicInfo;
static MS_S32 _gs32OutuptColor;

static MS_U32 _gu32FrameBufferSize    = 0;
static MS_U32 _gu32DecodeBuffer         = 0;
static MS_U32 _gu32DecodeBufferAlign     = 0;
static MS_U32 _gu32FrameBuffer         = 0;
static MS_U32 _gu32FrameBufferAlign   = 0;

static MS_U8 *_gpu8InputDataAddr = NULL;
static MS_U8 *_gpu8InputDataAddrAlign = NULL;

static MS_U32 _gu32InputDataLength = 0;
static MS_U8 *_gpu8GoldenDataAddr = NULL;
static MS_U32 _gu32GoldenDataLength = 0;

static MS_U8 _gu8BaseLinePNG[] = {
    #include "../data/f1_2002_png.dat"
};

static MS_U8 _gu8BaseLinePNG_Golden[] = {
    #include "../data/f1_2002_png_argb8888.dat"
};

static MS_U8 _gu8BaseLineGIF[] = {
    #include "../data/peo_008_gif.dat"
};

static MS_U8 _gu8BaseLineGIF_Golden[] = {
    #include "../data/peo_008_gif_argb8888.dat"
};

//----------------------------------------------------------------------------------------
//Global Function
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Local Function
//----------------------------------------------------------------------------------------
static MS_BOOL _Demo_GPD_SetDefaultBuffer(void)
{
    MS_U32 u32DecodeBufferSize = COLOR_BUFFER_SIZE + DEPTH_BUFFER_SIZE;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    _gu32DecodeBuffer = (MS_U32)MsOS_AllocateMemory(u32DecodeBufferSize + RESERVE_SPACE, s32MstarNonCachedPoolID);
    if(_gu32DecodeBuffer == 0)
    {
        db_print("MsOS_AllocateMemory FAILED !!\n");
        return FALSE;
    }
    else
    {
        _gu32DecodeBufferAlign = ALIGNMENT(_gu32DecodeBuffer, 32);
        memset((char *)_gu32DecodeBuffer, 0, u32DecodeBufferSize);
        return TRUE;
    }
}

static MS_BOOL _Demo_GPD_LoadPattern(MS_BOOL bGif)
{
    MS_U32 u32InputBufferAdderss;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(bGif)
    {
        _gu32InputDataLength = sizeof(_gu8BaseLineGIF);
        _gpu8InputDataAddr = (MS_U8*)MsOS_AllocateMemory(_gu32InputDataLength+ RESERVE_SPACE, s32MstarNonCachedPoolID);
        if(_gpu8InputDataAddr == 0)
        {
            db_print("MsOS_AllocateMemory FAILED !!\n");
            return FALSE;
        }

        u32InputBufferAdderss = (MS_U32)_gpu8InputDataAddr;
        u32InputBufferAdderss = ALIGNMENT(u32InputBufferAdderss, 32);
        _gpu8InputDataAddrAlign = (MS_U8*)u32InputBufferAdderss;

        memcpy((void*)_gpu8InputDataAddrAlign,(void*)_gu8BaseLineGIF,_gu32InputDataLength);
        _gpu8GoldenDataAddr = (MS_U8*)_gu8BaseLineGIF_Golden;
        _gu32GoldenDataLength = sizeof(_gu8BaseLineGIF_Golden);

        db_print("Loading GIF pattern file (%"DTC_MS_U32_d" bytes)...from addr %"DTC_MS_U32_x" ,PHYAddr %"DTC_MS_U32_x"\n",
               _gu32InputDataLength, (MS_U32)_gu8BaseLineGIF, (MS_U32)_gpu8InputDataAddrAlign);
        db_print("Loading GIF golden file (%"DTC_MS_U32_d" bytes)...\n", _gu32GoldenDataLength);
    }
    else
    {
        _gu32InputDataLength = sizeof(_gu8BaseLinePNG);
        _gpu8InputDataAddr = (MS_U8*)MsOS_AllocateMemory(_gu32InputDataLength, s32MstarNonCachedPoolID);
        if(_gpu8InputDataAddr == 0)
        {
            db_print("MsOS_AllocateMemory FAILED !!\n");
            return FALSE;
        }

        u32InputBufferAdderss = (MS_U32)_gpu8InputDataAddr;
        u32InputBufferAdderss = ALIGNMENT(u32InputBufferAdderss, 32);
        _gpu8InputDataAddrAlign = (MS_U8*)u32InputBufferAdderss;

        memcpy((void*)_gpu8InputDataAddrAlign,(void*)_gu8BaseLinePNG,_gu32InputDataLength);
        _gpu8GoldenDataAddr = (MS_U8*)_gu8BaseLinePNG_Golden;
        _gu32GoldenDataLength = sizeof(_gu8BaseLinePNG_Golden);

        db_print("Loading PNG pattern file (%"DTC_MS_U32_d" bytes)...from VAddr %"DTC_MS_U32_x", PHYAddr %"DTC_MS_U32_x"\n",
                _gu32InputDataLength, (MS_U32)_gu8BaseLinePNG, (MS_U32)_gpu8InputDataAddrAlign);
        db_print("Loading PNG golden file (%"DTC_MS_U32_d" bytes)...\n", _gu32GoldenDataLength);
    }

	return TRUE;
}

static MS_BOOL _Demo_GPD_SetOutputBuffer(gpd_pic_info *pstPicInfo,MS_S32 s32OutputColor,MS_BOOL bGif)
{
    MS_U32 u32FrameSize = 0;
    MS_U32 u32PixelBytes = 0;

    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    GET_PIXEL_BYTE(s32OutputColor, u32PixelBytes);
    if(u32PixelBytes == 0)
    {
        db_print("Unsupported output color %"DTC_MS_U32_d"\n", s32OutputColor);
        return FALSE;
    }

    u32FrameSize = ALIGNMENT(pstPicInfo->u32Width * u32PixelBytes, 8) * pstPicInfo->u32Height;   // 8-align for image rows
    u32FrameSize = ALIGNMENT(u32FrameSize, 8);    //do 8-alignment for GPD memory alignment

    if(bGif)
    {
        _gu32FrameBufferSize    = u32FrameSize*2;
        memcpy((void*)(&_gstGIfPicInfo),pstPicInfo,sizeof(gpd_pic_info));
    }
    else
    {
        _gu32FrameBufferSize    = u32FrameSize;
        memcpy((void*)(&_gstPngPicInfo),pstPicInfo,sizeof(gpd_pic_info));
    }

    _gu32FrameBuffer = (MS_U32)MsOS_AllocateMemory(_gu32FrameBufferSize+ RESERVE_SPACE, s32MstarNonCachedPoolID);

    // do 32-alignment for output buffer
    _gu32FrameBufferAlign = ALIGNMENT(_gu32FrameBuffer, 32);

    db_print("_gu32FrameBuffer        = %"DTC_MS_U32_x" \n",_gu32FrameBuffer);
    db_print("_gu32FrameBufferAlign  = %"DTC_MS_U32_x" \n",_gu32FrameBufferAlign);
    db_print("_gu32FrameBufferSize   = %"DTC_MS_U32_x" \n",_gu32FrameBufferSize);

    if(_gu32FrameBuffer != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void _Demo_GPD_ReleaseBuffer(void)
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    if(_gu32DecodeBuffer != 0)
    {
        MsOS_FreeMemory((void *) _gu32DecodeBuffer, s32MstarNonCachedPoolID);
        _gu32DecodeBuffer = NULL;
    }

    if(_gu32FrameBuffer != 0)
    {
        MsOS_FreeMemory((void *) _gu32FrameBuffer, s32MstarNonCachedPoolID);
        _gu32FrameBuffer = NULL;
    }

    if(_gpu8InputDataAddr != 0)
    {
        MsOS_FreeMemory((void *) _gpu8InputDataAddr, s32MstarNonCachedPoolID);
        _gpu8InputDataAddr = NULL;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to compare raw data and ROI data
/// @this function is not used
//------------------------------------------------------------------------------
static MS_S32 _Demo_GPD_CompareRawROI(MS_U32 u32Width, MS_U32 u32Height, gpd_roi_info *pstRoiInfo, MS_S32 s32OutputColor)
{
    db_print(" Start compare raw and roi data\n ");

    MS_U32 u32PixelBytes, u32RowBytes, u32CmpBytes, u32RowOffset, u32pitch, u32ReadSize;
	MS_U32 u32RowNum, u32RowLeft, u32ReadRows, u32MaxReadRows, u32RowPadding = 0;

	MS_U8 *pu8Raw = 0;      // data from .dat file
    MS_U8 *pu8Frame = 0;    // data from GPD decode
    size_t result;
    gpd_roi_info stRoiInfo;
    MS_BOOL ret = FALSE;

    if (!pstRoiInfo)
    {
        stRoiInfo.width = u32Width;
        stRoiInfo.height = u32Height;
        stRoiInfo.hstart = 0;
        stRoiInfo.vstart = 0;
    }
    else
    {
        stRoiInfo = *pstRoiInfo;
    }

    GET_PIXEL_BYTE(s32OutputColor, u32PixelBytes);
    if(u32PixelBytes == 0)
    {
        db_print("Unsupported output color %"DTC_MS_U32_d"\n", s32OutputColor);
        ret = FALSE;
        goto FINISH;
    }

    u32RowOffset = u32PixelBytes * stRoiInfo.hstart;            // offset of the beginning in each row
    u32CmpBytes = u32PixelBytes * stRoiInfo.width;              // once compare the byte size
    u32RowBytes = u32PixelBytes * u32Width + u32RowPadding;     // the byte size in a row
    //u32pitch = (u32CmpBytes + 7) & (~7);                    // 8-align and equal to u32CmpBytes size
    u32pitch = ALIGNMENT(u32CmpBytes, 8);                    // 8-align and equal to u32CmpBytes size

    if(_gpu8GoldenDataAddr == NULL)
    {
        printf("%s  %d  can't load golden data\n",__FUNCTION__,__LINE__);
    }

	// Start comparison
	u32RowNum = 0;                   // the current cmp row
	u32RowLeft = u32Height;        // image height

	u32MaxReadRows = _gu32GoldenDataLength / u32RowBytes;       // row num of the golden data (= height of the golden data)
    pu8Frame = (MS_U8 *)_gu32FrameBufferAlign;

	while (u32RowLeft)
	{
		MS_U32 j;

		if (u32RowLeft < u32MaxReadRows)
			u32ReadRows = u32RowLeft;
		else
			u32ReadRows = u32MaxReadRows;   // how many rows will be compared

		u32ReadSize = u32RowBytes * u32ReadRows;

        pu8Raw = (MS_U8*)_gpu8GoldenDataAddr;
		result = _gu32GoldenDataLength;
        db_print("Loading raw file (%"DTC_MS_U32_d" bytes) and result (%dbytes)\n", u32ReadSize, result);
        if (result != u32ReadSize)
        {
        	db_print ("Fail to read raw file. row_left = %"DTC_MS_U32_d", max_read_rows = %"DTC_MS_U32_d"\n", u32RowLeft, u32MaxReadRows);
        	ret = FALSE;
        	goto FINISH;
        }

        for (j = 0; j < u32ReadRows; j++)
        {
            if (u32RowNum >= stRoiInfo.vstart && u32RowNum < stRoiInfo.vstart + stRoiInfo.height)
            {
                result = memcmp(pu8Frame, pu8Raw + u32RowOffset, u32CmpBytes);
            	if (result)
            	{
    	        	db_print("Compare failed at row %"DTC_MS_U32_d"\n", u32RowNum);
                    db_print("_gpu8GoldenDataAddr = %"DTC_MS_U32_x" ,pu8Raw =%"DTC_MS_U32_x"\n", (MS_U32)_gpu8GoldenDataAddr, (MS_U32)pu8Raw);
                    db_print("u32RowOffset = %"DTC_MS_U32_d" ,u32RowBytes = %"DTC_MS_U32_d" ,u32pitch = %"DTC_MS_U32_d"\n", u32RowOffset, u32RowBytes, u32pitch);
                    db_print("pu8Frame = %"DTC_MS_U32_x"\n", (MS_U32)pu8Frame);
    	        	ret = FALSE;
    	        	goto FINISH;
            	}
                if (!pstRoiInfo)
                {
                   pu8Frame += u32pitch;
                }
                else
                {
            	   pu8Frame += u32RowBytes;
                }
            }
        	u32RowNum++;
        	pu8Raw += u32RowBytes;
        }

        u32RowLeft -= u32ReadRows;  // let  u32RowLeft = 0
	}
    ret = TRUE;

FINISH:
    if (!ret)
    {
    	db_print("Not match or error occurred with color %"DTC_MS_S32_d"!\n", s32OutputColor);
        db_print("Output buffer addr= %"DTC_MS_U32_x" and size = %"DTC_MS_U32_x"\n",MsOS_VA2PA((MS_U32)_gu32FrameBufferAlign), (MS_U32)_gu32GoldenDataLength);
    }
    else
    {
    	db_print("File match with golden with color %"DTC_MS_S32_d"!\n", s32OutputColor);
    }
    return ret;
}

static MS_BOOL _Demo_batch_verification(MS_BOOL bEnableGif)
{
    MS_BOOL bGif = bEnableGif;
    MS_BOOL bRoi = FALSE;
    MS_BOOL bStatus = TRUE;
    MS_S32 s32OutputColor = ARGB8888;   // default surpport ARGB8888 of gif and png

    gpd_pic_info stPicInfo;
    gpd_roi_info stRoiInfo;
    gpd_roi_info *pstTtmpRoiInfo = NULL;
    MS_S32 s32State;
    MS_S32 s32Duration = 0;

    _gs32OutuptColor = s32OutputColor;

	memset(&stPicInfo, 0, sizeof(stPicInfo));
	memset(&stRoiInfo, 0, sizeof(stRoiInfo));
    _Demo_GPD_SetDefaultBuffer();
    db_print("_gu32DecodeBuffer = %"DTC_MS_U32_x" and MSOS_VA2PA(_gu32DecodeBuffer)= %"DTC_MS_U32_x"\n",_gu32DecodeBuffer,MS_VA2PA(_gu32DecodeBuffer));
    MApi_GPD_Init(MS_VA2PA(_gu32DecodeBuffer));
    if (!_Demo_GPD_LoadPattern(bGif))
    {
        printf("Load pattern error\n");
        bStatus = FALSE;
        goto ERROR_PATTERN;
    }

    /* Get image info */
    db_print("Get info start\n");
    db_print("_gpu8InputDataAddr = %"DTC_MS_U32_x" \n",(MS_U32)_gpu8InputDataAddrAlign);

    if(MApi_GPD_InputSource(&stPicInfo, (MS_U32)(MS_VA2PA((MS_U32)_gpu8InputDataAddrAlign)), _gu32InputDataLength) == GPD_FAILED)
    {
        db_print("Get info error\n");
        bStatus = FALSE;
        goto ERROR_PATTERN;
    }
    else
    {
        db_print("pic_info->height = %"DTC_MS_U32_d" ,width = %"DTC_MS_U32_d",BitPP = %d,Interlace = %d\n",
                stPicInfo.u32Height, stPicInfo.u32Width, stPicInfo.u8BitPP, stPicInfo.u8Interlace);
        if(!_Demo_GPD_SetOutputBuffer(&stPicInfo, s32OutputColor, bGif))
        {
            db_print("_Demo_GPD_SetOutputBuffer FAILED !!\n");
        }
    }

    /* Random ROI */
    if (bRoi)
    {
        stRoiInfo.width = stPicInfo.u32Width/2;
        stRoiInfo.height = stPicInfo.u32Height/2;
        stRoiInfo.hstart = 0;//(pic_info.u32Width - roi.width + 1);
        stRoiInfo.vstart = 0;//(pic_info.u32Height - roi.height + 1);
        pstTtmpRoiInfo = &stRoiInfo;
        db_print("ROI enable: x = %"DTC_MS_U32_d", y = %"DTC_MS_U32_d", w = %"DTC_MS_U32_d", h = %"DTC_MS_U32_d"\n",
                stRoiInfo.hstart, stRoiInfo.vstart, stRoiInfo.width, stRoiInfo.height);
    }
    else
    {
        stRoiInfo.width = stPicInfo.u32Width;
        stRoiInfo.height = stPicInfo.u32Height;
        stRoiInfo.hstart = 0;
        stRoiInfo.vstart = 0;
        pstTtmpRoiInfo = NULL;
    }

    /* Decode and compare */
    printf("Decode start\n");
    MApi_GPD_SetGIFMode(GPD_NO_SUPPORT_MGIF); // close MGIF mode to unsupport animate gif

    if((s32State = MApi_GPD_OutputDecode(MS_VA2PA((MS_U32)_gu32FrameBufferAlign), s32OutputColor, _gu32FrameBufferSize)) != GPD_FAILED)
    {
       	db_print("Decode success!\n");

        while(s32State == GPD_STATE_MGIF_WAIT_BUFFFER)
        {
            switch(s32State)
            {
                case GPD_STATE_MGIF_WAIT_BUFFFER:
                    Demo_OSD_DrawGPD(bGif);
                    //GE_DrawGPD(enable_gif);
                    s32Duration = MApi_GPD_GetDuration();
                    printf("s32duration = %"DTC_MS_S32_d" \n",s32Duration);
                    printf("_gu32GoldenDataLength = %"DTC_MS_U32_x",_gu32FrameBuffer = %"DTC_MS_PHY_x"\n",
                            stPicInfo.u32Width * stPicInfo.u32Height * 4,MS_VA2PA((MS_U32)_gu32FrameBufferAlign));
                    break;
                case GPD_STATE_MGIF_TERMINATE:
                case GPD_STATE_GIF_ERROR:
                case GPD_STATE_GIF_SUCCESS:
                    printf("[%s] [%d] state = %"DTC_MS_S32_d"\n",__FUNCTION__,__LINE__, s32State);
                    break;
            }
            s32State = MApi_GPD_OutputDecodeMGIF(MS_VA2PA((MS_U32)_gu32FrameBufferAlign), s32OutputColor, _gu32FrameBufferSize);
            MsOS_DelayTask(s32Duration);
        }
    }
    else
    {
    	db_print("Decode error!\n");
        bStatus = FALSE;
        goto ERROR_PATTERN;
    }

    //GE_DrawGPD(enable_gif);
    Demo_OSD_DrawGPD(bGif);

    _Demo_GPD_CompareRawROI(stPicInfo.u32Width, stPicInfo.u32Height, pstTtmpRoiInfo, s32OutputColor);

ERROR_PATTERN:
    _Demo_GPD_ReleaseBuffer();
    return bStatus;
}




//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief The sample code to decode GIF and PNG picture to YUV422 format and then bitblt to GOP frame buffer
/// @param none
/// @return TRUE: Decode finished
/// @return FALSE: Decode failed
/// Command: \b GPD_Decode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_GPD_Decode(MS_BOOL *bGif)
{
    printf("Start GPD decode testing .......\n");

    if(*bGif == TRUE)
    {
        printf("Decode GIF picture\n");
    }
    else
    {
        printf("Decode PNG picture\n");
    }

    if(_Demo_batch_verification(*bGif))
    {
        printf("DECODE SUCCESS!!\n");
    }
    else
    {
        printf("DECODE FAILED!!\n");
    }


    printf("GPD testing end!!");

	return TRUE;
}

MS_U32 Demo_GPD_GetOutputBufferAddr(void)
{
    if(_gu32FrameBufferAlign != 0)
    {
        return _gu32FrameBufferAlign;
    }
    else
    {
        return 0;
    }
}

MS_U32 Demo_GPD_GetOutputBufferSize(void)
{
    if(_gu32FrameBuffer != 0)
    {
        return _gu32FrameBufferSize;
    }
    else
    {
        return 0;
    }
}
MS_U32 Demo_GPD_GetHeight(MS_BOOL bGif)
{
    if(bGif)
    {
        return _gstGIfPicInfo.u32Height;
    }
    else
    {
        return _gstPngPicInfo.u32Height;
    }
}

MS_U32 Demo_GPD_GetWidth(MS_BOOL bGif)
{
    if(bGif)
    {
        return _gstGIfPicInfo.u32Width;
    }
    else
    {
        return _gstPngPicInfo.u32Width;
    }
}
MS_U32 Demo_GPD_GetColorFmt(void)
{
    return _gs32OutuptColor;
}

#endif

