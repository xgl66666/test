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
/// @file   appDemo_GOP.c
/// @author MStar Semiconductor Inc.
/// @brief  GOP Function Sample Demo
///
/// @name GOP Function Sample Command Usage
/// @verbatim
/// ----------------------------------------------------------------------------
/// 1. Initial GOP
/// Usage:GOP_Init
///
/// Sample Command:GOP_Init
///
///
/// ---------------------------------------------------------------------
/// 2. Set configurations
/// Usage:GOP_SetConfig
///
/// Sample Command:GOP_SetConfig
///
///
/// ----------------------------------------------------------------------------
/// 3. Create GWin
/// Usage:GOP_CreateWin ColorType GWinWidth0 GWinHeight0 GWinWidth1 GWinHeight1
///     ColorType : Gwin color format
///         HD: E_GOP_COLOR_ARGB8888  SD: E_GOP_COLOR_ARGB8888      =0
///         HD: E_GOP_COLOR_RGB565    SD: E_GOP_COLOR_RGB565        =1
///         HD: E_GOP_COLOR_ARGB4444  SD: E_GOP_COLOR_ARGB4444      =2
///         HD: E_GOP_COLOR_I8        SD: E_MS_FMT_RGB565           =3
///     GWinWidth0 The width of HD GWin to create. it is small than stretch win width
///     GWinHeight0 The height of HD GWin to create. it is small than stretch win height
///     GWinWidth1 The width of SD GWin to create. it is small than stretch win width
///     GWinHeight1 The height of SD GWin to create. it is small than stretch win height
///
/// Sample Command:GOP_CreateWin 0 1280 720 720 576
///
///
/// ----------------------------------------------------------------------------
/// 4. Draw Bitmap
/// Usage:GOP_DrawBmp Draw_X Draw_Y Draw_X1 Draw_Y1
///     Draw_X The left point of HD GWin to draw.
///     Draw_Y The top point of HD GWin to draw.
///     Draw_X1 The left point of SD GWin to draw.
///     Draw_Y1 The top point of SD GWin to draw.
///
/// Sample Command:GOP_DrawBmp 0 0 0 0
///
///
/// ----------------------------------------------------------------------------
/// 5. DWIN capture video frame
/// Usage:GOP_DWinCapture CaptureOneFrame GWinDsipCount Source
///     CaptureOneFrame : Continuously or Auto-stop capture
///         Continuously capture                                = 0
///         Auto-stop capture after one frame has been captured = 1
///     GWinDsipCount The count that using GWin to displpay the captured frame
///     Source The source to capture.
///         DWIN_SRC_OP = 0
///         DWIN_SRC_IP = 1
///
/// Sample Command:GOP_DWinCapture 0 10 0
///                GOP_DWinCapture 1 1 0
///                GOP_DWinCapture 0 10 1
///                GOP_DWinCapture 1 1 1
///
///
/// ----------------------------------------------------------------------------
/// 6. Enable Gwin
/// Usage:GOP_EnableWin Enable
///     Enable: 0 disable, 1 enable
///
/// Sample Command:GOP_EnableWin 0
///                GOP_EnableWin 1
///
///
/// ----------------------------------------------------------------------------
/// 7. Config Gwin Global Alpha Value
/// Usage:GOP_SetGlobalAlpha Auto AlphaValue
///     Auto: 0 disable, 1 enable
///     AlphaValue: The Alpha value, range {0..255}, This parameter only take effect
///                 when Auto=0
///
/// Sample Command:GOP_SetGlobalAlpha 1
///                GOP_SetGlobalAlpha 0 0
///                GOP_SetGlobalAlpha 0 255
///
///
/// ---------------------------------------------------------------------
/// 8. Enable individual Alpha Value of an image in a GWIN. Only ARGB8888 and ARGB4444
///    support the individual alpha.
/// Usage:GOP_EnablePerPixelAlpha Auto AlphaValue
///     Auto: 0 disable, 1 enable
///     AlphaValue: The Alpha value, range {0..255}, This parameter only take effect
///                 when Auto=0
///
/// Sample Command:GOP_EnablePerPixelAlpha 1
///                GOP_EnablePerPixelAlpha 0 0
///                GOP_EnablePerPixelAlpha 0 255
///
///
/// ----------------------------------------------------------------------------
/// 9. Config Fade Effect
/// Usage:GOP_SetFade WinId FadeType Rate
///     FadeType: 0 fade in, 1 fade out
///     Enable : 0 disable, 1 enable
///     Rate: The fade rate, range {0..65535}
///
/// Sample Command:GOP_SetFade 0 1 32767
///                GOP_SetFade 1 1 8191
///
///
/// ----------------------------------------------------------------------------
/// 10. Free GWin
/// Usage:GOP_FreeWin
///
/// Sample Command:GOP_FreeWin
///
/// ----------------------------------------------------------------------------
/// 11. Config Zlist Effect
/// Usage:GOP_Zlist
///     Layer_select: 0: Video->GOP1->GOP2
///                        1: FrameColor->Video->GOP1->GOP2
///                        2: FrameColor->Video->GOP2->GOP1
///                        3: FrameColor->GOP1->Video->GOP2
///                        4: FrameColor->GOP1->GOP2->Video
///                        5: FrameColor->GOP2->Video->GOP1
///                        6: FrameColor->GOP2->GOP1->Video
///
/// Sample Command:GOP_Zlist 4
///                GOP_Zlist 0
///
/// ----------------------------------------------------------------------------
/// 12. Set video alpha
/// Usage:GOP_SetVideoAlpha
///    u8Alpha:  The Video Alpha value, range {0..255}
///
/// Sample Command:GOP_SetVideoAlpha 128
///                GOP_SetVideoAlpha 100
///
/// @endverbatim
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_GOP.c
   @brief The Template code for the MDrv_XXX functions

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
//#include "apiGOP_SC.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvSEM.h"
#include "MsOS.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"

//Scalar
#include "MsTypes2.h"
//#include "ver_dtv_comm.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include "Panel.h"
#include "apiPNL.h"
#ifdef ENABLE_PVR_PACKAGE
#include "appDemo_PVR.h"
#endif

#include "appDemo_Video.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define E_GOP 0
#define E_GOP1 1
#define HDPath 0
#define SDPath 1

//#define GWIN_WIDTH 400
//#define GWIN_HEIGHT 400

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

#define DWIN_OP2GE_SCALE_FACTOR 1
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_U32 Path;
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
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

static MS_U8 GOPCount = 0;

static MS_U32 u32BmpDRAMStrAddr = 0x0;
static MS_U32 u32BmpAllocPhyAddr = 0x0;
extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;

//each bitmap dram adr
static MS_U32 u32BmpRGB565Addr = 0x0;
static MS_U32 u32BmpARGB4444Addr = 0x0;
static MS_U32 u32BmpARGB8888Addr = 0x0;
static MS_U32 u32BmpI8Addr = 0x0;

//static MS_U8 gwin_num = 0xFF;
static MS_U32 u32delay_time = 500;
//static MS_U8 gwin_fmt[]={E_MS_FMT_ARGB4444, E_MS_FMT_ARGB4444};
static GWinInfo gWinInfo[] = {{E_GOP_DST_OP0, 0xFF, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB4444, {0}},
                        {E_GOP_DST_MIXER2VE, 0xFF, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB4444, {0}}};

static MS_U8 u8DWinFbId = 0xFF;
static MS_BOOL bGOP_init=FALSE;
extern unsigned char _RGB565BMPBIN[];
extern unsigned char _ARGB4444BMPBIN[];
extern unsigned char _ARGB8888BMPBIN[];
extern unsigned char _I8BMPBIN[];

//this palette is used for I8 bitmap testing
extern GOP_PaletteEntry _gopI8PaletteEntry[];

extern MS_BOOL appDemo_XC_Init(void);
extern MS_BOOL appDemo_VE_Init(void);
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static void _GOP_ScalerInit(void)
{
    /*-------------XC Init----------------*/
    appDemo_XC_Init();
    printf("[%s][%d]\n", __FUNCTION__,__LINE__);
    /*-------------VE path init----------------*/
    appDemo_VE_Init();
}

static MS_U32 _OSD_SetFBFmt(MS_U16 pitch,MS_U32 addr , MS_U16 fmt )
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
    VDEC_EX_DispInfo   info;
    VDEC_EX_Result     ret;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    memset(&info, 0, sizeof(VDEC_EX_DispInfo));
    ret = MApi_VDEC_EX_GetDispInfo(stVDECStreamId,&info);


    if (E_VDEC_EX_OK != ret)
    {
        return FALSE;
    }

    return info.u8Interlace;
}

static MS_BOOL _get_video_info(MS_U8 *bInterlace, MS_U16 *HSize, MS_U16 *VSize)
{
    VDEC_EX_DispInfo   info;
    VDEC_EX_Result     ret;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    memset(&info, 0, sizeof(VDEC_EX_DispInfo));
    ret = MApi_VDEC_EX_GetDispInfo(stVDECStreamId,&info);

    if (E_VDEC_EX_OK != ret)
    {
        return FALSE;
    }
    else
    {
        *bInterlace = info.u8Interlace;
        *HSize = info.u16HorSize;
        *VSize = info.u16VerSize;
        printf("[%s][%d]Interlace = %u , H = %d, V = %d\n", __FUNCTION__, __LINE__, *bInterlace, *HSize, *VSize);
        return TRUE;
    }
}
static MS_U8 _GOP_Create_FB(MS_U8 *u8FB, MS_U16 FBWidth, MS_U16 FBHeight, MS_U16 FBFmt)
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
    MApi_GFX_ClearFrameBuffer(DstFBInfo.addr, DstFBInfo.size, 0x00);
    MApi_GFX_FlushQueue();

    return GWIN_OK;
}

static MS_U32 _GOP_Create_GWin(MS_U8 u8GOP, MS_U8 *u8GWin, MS_U8 u8FB, MS_U16 FBFmt)
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
/*
    if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(*u8GWin, TRUE) )
    {
         printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
         return GWIN_FAIL;
    }
*/
    return 0;
}

static void _GOP_InitGlobalVar(void)
{
    u32BmpDRAMStrAddr = SDRAM_ADR_BMP_ADR; //temp adr for testing
}

static void _GOP_SetBmpFontAdr(void)
{
    u32BmpARGB8888Addr=u32BmpDRAMStrAddr;
    u32BmpRGB565Addr= GE_ADDR_ALIGNMENT(u32BmpARGB8888Addr+BMPARGB8888SIZE);
    u32BmpARGB4444Addr= GE_ADDR_ALIGNMENT(u32BmpRGB565Addr+BMPRGB565SIZE);
    u32BmpI8Addr= GE_ADDR_ALIGNMENT(u32BmpARGB4444Addr+BMPARGB4444SIZE);
}


static void _LoadBmp(void)
{
    memcpy((void*)MS_PA2KSEG1(u32BmpARGB8888Addr), (void*)_ARGB8888BMPBIN, BMPARGB8888SIZE );
    printf("u32BmpARGB8888Addr= 0x%lx\n", (MS_U32)u32BmpARGB8888Addr);
    printf("BMPARGB8888SIZE = %lu\n", BMPARGB8888SIZE);

    memcpy((void*)MS_PA2KSEG1(u32BmpRGB565Addr), (void*)_RGB565BMPBIN, BMPRGB565SIZE );
    printf("u32BmpRGB565Addr= 0x%lx\n", (MS_U32)u32BmpRGB565Addr);
    printf("BMPRGB565SIZE = %lu\n", BMPRGB565SIZE);

    memcpy((void*)MS_PA2KSEG1(u32BmpARGB4444Addr), (void*)_ARGB4444BMPBIN, BMPARGB4444SIZE );
    printf("u32BmpARGB4444Addr= 0x%lx\n", (MS_U32)u32BmpARGB4444Addr);
    printf("BMPARGB4444SIZE = %lu\n", (MS_U32)BMPARGB4444SIZE);

    memcpy((void*)MS_PA2KSEG1(u32BmpI8Addr), (void*)_I8BMPBIN, BMPI8SIZE );
    printf("u32BmpI8Addr= 0x%lx\n", (MS_U32)u32BmpI8Addr);
    printf("BMPI8SIZE = %lu\n", BMPI8SIZE);
}

static MS_U32 _AllocAndLoadOneBmp(MS_U8 gwinFmt)
{
    //Sometime demo code load no bmp because limitation of Memory size.
    //In this case, bmp must load and free dynamic.
    //This function supply to load one bmp from bin file.
    //return value is AlignPhyAddr, if 0 is fail.
    MS_U32 u32AlignPhyAddr = 0x0;
    MS_U16 u16align = 32;
    switch( gwinFmt )
    {
        case E_MS_FMT_ARGB8888:
            u32BmpAllocPhyAddr = MsOS_VA2PA((MS_U32)MsOS_AllocateMemory(BMPARGB8888SIZE + u16align,gs32NonCachedPoolID));
            if(0 == u32BmpAllocPhyAddr)
            {
                printf("[%s][%d]MsOS_AllocateMemory Failed\n", __FUNCTION__, __LINE__);
                break;
            }
            u32AlignPhyAddr = (((u32BmpAllocPhyAddr+u16align-1)/u16align)*(u16align));
            memcpy((void*)MS_PA2KSEG1(u32AlignPhyAddr), (void*)_ARGB8888BMPBIN, BMPARGB8888SIZE );
            break;
        case E_MS_FMT_RGB565:
            u32BmpAllocPhyAddr = MsOS_VA2PA((MS_U32)MsOS_AllocateMemory(BMPRGB565SIZE + u16align,gs32NonCachedPoolID));
            if(0 == u32BmpAllocPhyAddr)
            {
                printf("[%s][%d]MsOS_AllocateMemory Failed\n", __FUNCTION__, __LINE__);
                break;
            }
            u32AlignPhyAddr = (((u32BmpAllocPhyAddr+u16align-1)/u16align)*(u16align));
            memcpy((void*)MS_PA2KSEG1(u32AlignPhyAddr), (void*)_RGB565BMPBIN, BMPRGB565SIZE );
            break;
        case E_MS_FMT_ARGB4444:
            u32BmpAllocPhyAddr = MsOS_VA2PA((MS_U32)MsOS_AllocateMemory(BMPARGB4444SIZE + u16align,gs32NonCachedPoolID));
            if(0 == u32BmpAllocPhyAddr)
            {
                printf("[%s][%d]MsOS_AllocateMemory Failed\n", __FUNCTION__, __LINE__);
                break;
            }
            u32AlignPhyAddr = (((u32BmpAllocPhyAddr+u16align-1)/u16align)*(u16align));
            memcpy((void*)MS_PA2KSEG1(u32AlignPhyAddr), (void*)_ARGB4444BMPBIN, BMPARGB4444SIZE );
            break;
        case E_MS_FMT_I8:
            u32BmpAllocPhyAddr = MsOS_VA2PA((MS_U32)MsOS_AllocateMemory(BMPI8SIZE + u16align,gs32NonCachedPoolID));
            if(0 == u32BmpAllocPhyAddr)
            {
                printf("[%s][%d]MsOS_AllocateMemory Failed\n", __FUNCTION__, __LINE__);
                break;
            }
            u32AlignPhyAddr = (((u32BmpAllocPhyAddr+u16align-1)/u16align)*(u16align));
            memcpy((void*)MS_PA2KSEG1(u32AlignPhyAddr), (void*)_I8BMPBIN, BMPI8SIZE );
            break;
        default:
            break;
    }
    printf("[%s] AllocAlignAddr = %lx\n", __FUNCTION__, u32AlignPhyAddr);
    return u32AlignPhyAddr;
}


static void _SetupEnvByDstFmt(MS_U8 gwinFmt, MS_U32 *pbmpAddr, MS_U16 *pbmpWidth, MS_U16 *pbmpHeight)
{
    GOP_PaletteEntry _gopI8Palette[GOP_PALETTE_ENTRY_NUM];
    int i;

    /// - Select the sample image by color type for the GWin that we create.
    switch( gwinFmt )
    {
        case E_MS_FMT_ARGB8888:
            *pbmpAddr = u32BmpARGB8888Addr;
            *pbmpWidth = BMPRGB8888W;
            *pbmpHeight = BMPRGB8888H;
            break;
        case E_MS_FMT_RGB565:
            *pbmpAddr = u32BmpRGB565Addr;
            *pbmpWidth = BMPRGB565W;
            *pbmpHeight = BMPRGB565H;
            break;
        case E_MS_FMT_ARGB4444:
            *pbmpAddr = u32BmpARGB4444Addr;
            *pbmpWidth = BMPRGB4444W;
            *pbmpHeight = BMPRGB4444H;
            break;
        case E_MS_FMT_I8:
            *pbmpAddr = u32BmpI8Addr;
            *pbmpWidth = BMPRGBI8W;
            *pbmpHeight = BMPRGBI8H;
            for (i=0; i<GOP_PALETTE_ENTRY_NUM; i++)
            {
            _gopI8Palette[i].RGB.u8A =  _gopI8PaletteEntry[i].RGB.u8A;
            _gopI8Palette[i].RGB.u8R =  _gopI8PaletteEntry[i].RGB.u8R;
            _gopI8Palette[i].RGB.u8G =  _gopI8PaletteEntry[i].RGB.u8G;
            _gopI8Palette[i].RGB.u8B =  _gopI8PaletteEntry[i].RGB.u8B;
            //printf("A:%d, R: %d, G: %d, B: %d\t", _gopI8Palette[i].RGB.u8A, _gopI8Palette[i].RGB.u8R, _gopI8Palette[i].RGB.u8G, _gopI8Palette[i].RGB.u8B);
            }
            //Set GOP palette table when GOP output frame buffer is I8
            MApi_GOP_GWIN_SwitchGOP(2);
            MApi_GOP_GWIN_SetPaletteOpt(_gopI8Palette, 0, GOP_PALETTE_ENTRY_NUM - 1, E_GOP_PAL_ARGB8888);
            break;
        default:
            printf("GOP_CreateWin() --> Wrong parameter\n");
            break;
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
        pitch = width << 1;
        break;
    case E_MS_FMT_ARGB8888 :
        pitch = width << 2;
        break;
    default :
        pitch = 0;
        break;
    }
    return pitch;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initial GOP
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b Show GOP_Help Info\n
//------------------------------------------------------------------------------
MS_BOOL GOP_Help(void)
{

    printf ("*************************GOP function List*************************\n");
    printf ("1.GOP_Init --Initial GOP\n");
    printf ("2.GOP_SetConfig --Set configurations including stretch win size\n");
    printf ("3.GOP_CreateWin  [ColorType] [GWinWidth0] [GWinHeight0] [GWinWidth1] [GWinHeight1]\n");
    printf ("-----ColorType : Gwin color format\n");
    printf ("-------HD: E_GOP_COLOR_ARGB8888  SD: E_GOP_COLOR_ARGB8888      =0\n");
    printf ("-------HD: E_GOP_COLOR_RGB565    SD: E_GOP_COLOR_RGB565        =1\n");
    printf ("-------HD: E_GOP_COLOR_ARGB4444  SD: E_GOP_COLOR_ARGB4444      =2\n");
    printf ("-------HD: E_GOP_COLOR_I8        SD: E_MS_FMT_RGB565           =3\n");
    printf ("-----GWinWidth0 : The width of HD GWin to create. it is small than stretch win width\n");
    printf ("-----GWinHeight0 : The height of HD GWin to create. it is small than stretch win height\n");
    printf ("-----GWinWidth1 : The width of SD GWin to create. it is small than stretch win width\n");
    printf ("-----GWinHeight1 : The height of SD GWin to create. it is small than stretch win height\n");
    printf ("4.GE_DrawBmp [Draw_X] [Draw_Y] [Draw_X1] [Draw_Y1] --To draw bitmap in the image buffer for this GWin\n");
    printf ("-----Draw_X : The left point of HD GWin to draw.\n");
    printf ("-----Draw_Y : The top point of HD GWin to draw.\n");
    printf ("-----Draw_X1 : The left point of SD GWin to draw.\n");
    printf ("-----Draw_Y1 : The top point of SD GWin to draw.\n");
    printf ("5.GOP_DWinCapture [CaptureOneFrame] [GWinDsipCount] [Source]\n");
    printf ("-----CaptureOneFrame : 0 disable, 1 enable\n");
    printf ("-----GWinDsipCount : The count that using GWin to displpay the captured frame\n");
    printf ("-----Source : 0 DWIN_SRC_OP, 1 DWIN_SRC_IP\n");
    printf ("6.GOP_EnableWin [Enable]\n");
    printf ("-----Enable: 0 disable, 1 enable\n");
    printf ("7.GOP_SetGlobalAlpha [Auto] [AlphaValue]\n");
    printf ("-----Auto: 0 disable, 1 enable\n");
    printf ("-----AlphaValue: range {0..255}, effect when Auto=0\n");
    printf ("8.GOP_EnablePerPixelAlpha [Auto] [AlphaValue]\n");
    printf ("-----Auto: 0 disable, 1 enable\n");
    printf ("-----AlphaValue: range {0..255}, effect when Auto=0\n");
    printf ("9.GOP_SetFade [FadeType] [Enable] [Rate]\n");
    printf ("-----FadeType: 0 E_WEAKER, 1 E_STRONGER\n");
    printf ("-----Enable: 0 disable, 1 enable\n");
    printf ("-----Rate: The fade rate\n");
    printf ("10.GOP_FreeWin \n");
    printf ("*************************End of GOP function List*************************\n");
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initial GOP
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_Init \n
//------------------------------------------------------------------------------
MS_BOOL GOP_Init(MS_U32 *getpath)
{
    printf("Demo_GOP_Init\n");

    GOP_InitInfo pGopInit;
    GFX_Config tGFXcfg;
    MS_VE_Output_Ctrl OutputCtrl;
    //GFX_Point gfxPt0 = { 0, 0 };
    //GFX_Point gfxPt1 = { GWIN_WIDTH, GWIN_HEIGHT };

    MDrv_SEM_Init();

    /// -Initial GE
    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);
    printf("driver GE init ok\n");

    /// -Initial GOP and Scaler
    //Initial Scaler
    printf("[%s][%d]\n", __FUNCTION__,__LINE__);
    _GOP_ScalerInit();
    //Initial GOP
    MApi_GOP_RegisterFBFmtCB(( MS_U32(*)(MS_U16 pitch,MS_U32 addr , MS_U16 fmt ))_OSD_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_XC_Sys_PQ_ReduceBW_ForOSD);

    pGopInit.u16PanelWidth = g_IPanel.Width();
    pGopInit.u16PanelHeight = g_IPanel.Height();
    pGopInit.u16PanelHStr = g_IPanel.HStart();


    pGopInit.u32GOPRBAdr = GOP_GWIN_RB_ADR;
    pGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
    pGopInit.u32GOPRegdmaAdr = GOP_REG_DMA_BASE_ADR;
    pGopInit.u32GOPRegdmaLen = GOP_REG_DMA_BASE_LEN;
    pGopInit.bEnableVsyncIntFlip = FALSE;
    MApi_GOP_Init(&pGopInit);
    printf("GOP_GWIN_RB_ADR : %x, GOP_REG_DMA_BASE_ADR: %x\n", GOP_GWIN_RB_ADR, GOP_REG_DMA_BASE_ADR);
    printf("driver GOP init ok\n");

    ////////////////////////////////////////////////////////////////////////
    //Use two GOPs, here we use GOP2 to display for HD and use GOP1 to display for SD

    GOPCount = 2;

    ///--------------GOP0 To HD---------------///
    gWinInfo[0].GopDest = E_GOP_DST_OP0;
    MApi_GOP_GWIN_SwitchGOP(E_GOP);//use GOP 0
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
    MApi_GOP_GWIN_SetGOPDst(E_GOP, gWinInfo[0].GopDest);

    Path= *getpath;

    if(Path == 1)
    {
        ///--------------GOP1 To HD---------------///
        gWinInfo[1].GopDest = E_GOP_DST_OP0;
        MApi_GOP_GWIN_SwitchGOP(E_GOP1);//use GOP 0
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
        MApi_GOP_GWIN_SetGOPDst(E_GOP1, gWinInfo[1].GopDest);

    /*If use 'GOP2' and the destination of GOP is 'OP',
      use this to change GOP output layer to the last one.
      The input is the GOP number
    */
    //MApi_GOP_GWIN_OutputLayerSwitch(2);
    }
    else
    {
        ///---------------GOP1 To SD---------------///
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

    // Init GOPD
    MApi_GOP_DWIN_Init();


    _GOP_InitGlobalVar();
    _GOP_SetBmpFontAdr();
    _LoadBmp();


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set configurations including stretch win size
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_SetConfig \n
//------------------------------------------------------------------------------
MS_BOOL GOP_SetConfig(void)
{

    GOP_MIXER_TIMINGTYPE gopMixerTimingType;
    GOP_MixerTiming gopMixerTiming;
    VE_DrvStatus DrvStatus;
    MS_VE_DST_DispInfo DrvVEDispInfo;
    MS_U16 GOPSC_width;
    MS_BOOL VE_PAL;

    printf("GE_SetConfig (with 2 GOPs)\n");
    ///--------------To HD---------------///
    MApi_GOP_GWIN_SwitchGOP(E_GOP);//use GOP 0
    //Set stretch window every re-initial
    //Set GOP0 destination to OP
    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP,gWinInfo[0].GopDest,0,0,g_IPanel.Width(),g_IPanel.Height());

    // Every time change resolution, the HStart should be set again
    printf("Panel HStart = %d\n",g_IPanel.HStart());
    MApi_GOP_SetGOPHStart(E_GOP,g_IPanel.HStart());

    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);//FIXME

    if(Path == 1)
    {
        MApi_GOP_GWIN_SwitchGOP(E_GOP1);//use GOP 1
        //Set stretch window every re-initial
        //Set GOP1 destination to OP
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP1,gWinInfo[1].GopDest,0,0,g_IPanel.Width(),g_IPanel.Height());

        // Every time change resolution, the HStart should be set again
        printf("Panel HStart = %d\n",g_IPanel.HStart());
        MApi_GOP_SetGOPHStart(E_GOP1,g_IPanel.HStart());

        MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);//FIXME
    }
    else
    {
    ///---------------To SD---------------///
    MApi_GOP_GWIN_SwitchGOP(E_GOP1);//use GOP 1
    MDrv_VE_GetStatus(&DrvStatus);
    if(DrvStatus.VideoSystem <= 2)
    {
        printf("Set NTSC\n");
        VE_PAL = FALSE;
        GOPSC_width = 480;
        gopMixerTimingType = GOP_NTSC;
    }else
    {
        printf("Set PAL\n");
        VE_PAL = TRUE;
        GOPSC_width = 576;
        gopMixerTimingType = GOP_PAL;
    }

    MApi_VE_GetDstInfo(&DrvVEDispInfo, sizeof(DrvVEDispInfo));

    MApi_GOP_MIXER_SetOutputTiming(gopMixerTimingType, &gopMixerTiming);

    if(!VE_PAL)
    {
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP1,gWinInfo[1].GopDest, 0, 0, 720, 480);
    }else
    {
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP1,gWinInfo[1].GopDest, 0, 0, 720, 576);
    }

    }
    MApi_GOP_GWIN_EnableProgressive(TRUE); //FIXME

    bGOP_init=TRUE;


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to create GWin
/// @param[in] ColorType color type for the GWin.
/// @param[in] GWinWidth0 The width of HD GWin to create. it is small than stretch win width
/// @param[in] GWinHeight0 The height of HD GWin to create. it is small than stretch win height
/// @param[in] GWinWidth1 The width of SD GWin to create. it is small than stretch win width
/// @param[in] GWinHeight1 The height of SD GWin to create. it is small than stretch win height
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_CreateWin \n
//------------------------------------------------------------------------------
MS_BOOL GOP_CreateWin(MS_U32 *ColorType, MS_U32 *GWinWidth0, MS_U32 *GWinHeight0, MS_U32 *GWinWidth1, MS_U32 *GWinHeight1)
{
    MS_U8 u8GOP[2];
    MS_U8 u8GWin[2];
    MS_U8 u8FB[2];
    MS_U16 FBWidth[2];
    MS_U16 FBHeight[2];
    MS_U16 FBFmt[2];
    MS_U32 _u32BmpDramRblkStart[2];
    MS_U16 bmpWidth[2];
    MS_U16 bmpHeight[2];

    MS_U8 i;

    //If there is any GWin exit, disable it!
    for(i = SDPath; i < GOPCount; i++)
    {
        if( gWinInfo[i].GeWinId!= 0xFF)
        {
            printf("Delete win %d\n", gWinInfo[i].GeWinId);
            MApi_GOP_GWIN_DeleteWin( gWinInfo[i].GeWinId );
        }
    }

    //Select frame buffer format to create
    switch(*ColorType)
    {
        case 0:
            gWinInfo[0].GeDstFmt = E_MS_FMT_ARGB8888;
            gWinInfo[1].GeDstFmt = E_MS_FMT_ARGB8888;
            break;
        case 1:
            gWinInfo[0].GeDstFmt = E_MS_FMT_RGB565;
            gWinInfo[1].GeDstFmt = E_MS_FMT_RGB565;
            break;
        case 2:
            gWinInfo[0].GeDstFmt = E_MS_FMT_ARGB4444;
            gWinInfo[1].GeDstFmt = E_MS_FMT_ARGB4444;
            break;
        case 3:
            printf("[%s][%d] GWIN1 can no use palette table, set format to RGB565!\n", __FUNCTION__, __LINE__);
            gWinInfo[0].GeDstFmt = E_MS_FMT_I8;
            gWinInfo[1].GeDstFmt = E_MS_FMT_RGB565;
            break;
        default:
            gWinInfo[0].GeDstFmt = E_MS_FMT_ARGB4444;
            gWinInfo[1].GeDstFmt = E_MS_FMT_ARGB4444;
            break;
    }
    _SetupEnvByDstFmt(gWinInfo[0].GeDstFmt, &_u32BmpDramRblkStart[0], &bmpWidth[0], &bmpHeight[0]);
    _SetupEnvByDstFmt(gWinInfo[1].GeDstFmt, &_u32BmpDramRblkStart[1], &bmpWidth[1], &bmpHeight[1]);

    for(i = SDPath; i < GOPCount; i++)
    {
        gWinInfo[i].bmpInfo.u32BmpDramRblkStart = _u32BmpDramRblkStart[i];
        gWinInfo[i].bmpInfo.bmpWidth = bmpWidth[i];
        gWinInfo[i].bmpInfo.bmpHeight = bmpHeight[i];
    }

    //GWin settings
    u8GOP[0] = gWinInfo[0].u8GOPId = E_GOP;
    u8FB[0] = gWinInfo[0].u8FBId;
    FBWidth[0] = gWinInfo[0].GopBitmapWidth = *GWinWidth0;
    FBHeight[0] = gWinInfo[0].GopBitmapHeight = *GWinHeight0;
    FBFmt[0] = gWinInfo[0].GeDstFmt;// = gwin_fmt;

    u8GOP[1] = gWinInfo[1].u8GOPId = E_GOP1;
    u8FB[1] = gWinInfo[1].u8FBId;
    FBWidth[1] = gWinInfo[1].GopBitmapWidth = *GWinWidth1;
    FBHeight[1] = gWinInfo[1].GopBitmapHeight = *GWinHeight1;
    FBFmt[1] = gWinInfo[1].GeDstFmt;// = gwin_fmt;

    //Create Frame buffer and Gwin
    for(i = SDPath; i < GOPCount; i++)
    {
        if( GWIN_OK != _GOP_Create_FB(&u8FB[i], FBWidth[i], FBHeight[i], FBFmt[i]))
        {
             printf("Create FB fail\n");
             return FALSE;
        }
        gWinInfo[i].u8FBId = u8FB[i];

        u8GWin[i] = gWinInfo[i].GeWinId;
        /// -Create GWin
        if( 0 != _GOP_Create_GWin(u8GOP[i], &u8GWin[i], u8FB[i], FBFmt[i] ))
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
/// @brief The sample code to draw bitmap in the image buffer for this GWin
/// @param[in] Draw_X The left point of GWin0 to create.
/// @param[in] Draw_X The top point of GWin0 to create.
/// @param[in] Draw_X1 The left point of GWin4 to create.
/// @param[in] Draw_X1 The top point of GWin4 to create.
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_DrawBmp \n
//------------------------------------------------------------------------------
MS_BOOL GOP_DrawBmp(MS_U32 *Draw_X, MS_U32 *Draw_Y, MS_U32 *Draw_X1, MS_U32 *Draw_Y1)
{
    MS_U8 i;
    GFX_BufferInfo srcbuf[2];
    GFX_DrawRect bitbltInfo[2];
    GFX_Point gfxPt0[2];
    GFX_Point gfxPt1[2];
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;

    for(i = SDPath; i < GOPCount; i++)
    {
        MApi_GOP_GWIN_SwitchGOP(gWinInfo[i].u8GOPId);
        //Set Clip window for rendering
        gfxPt0[i].x = 0;
        gfxPt0[i].y = 0;
        gfxPt1[i].x = gWinInfo[i].GopBitmapWidth;
        gfxPt1[i].y = gWinInfo[i].GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0[i], &gfxPt1[i]);

        MApi_GOP_GWIN_Switch2FB( gWinInfo[i].u8FBId );  //set dst buffer

        // Bitble the bitmap to DRAM of GE
        srcbuf[i].u32ColorFmt = (GFX_Buffer_Format)gWinInfo[i].GeDstFmt;
        if(gWinInfo[i].bmpInfo.u32BmpDramRblkStart == 0x0) //MemoryMap does not allocate BMP
        {
            gWinInfo[i].bmpInfo.u32BmpDramRblkStart = _AllocAndLoadOneBmp(gWinInfo[i].GeDstFmt);
            if(gWinInfo[i].bmpInfo.u32BmpDramRblkStart == 0x0)
            {
                printf("Fail: bmpInfo.u32BmpDramRblkStart = NULL\n");
                return FALSE;
            }
        }
        srcbuf[i].u32Addr = gWinInfo[i].bmpInfo.u32BmpDramRblkStart;
        printf("Src buffer addr = 0x%lx\n", gWinInfo[i].bmpInfo.u32BmpDramRblkStart);
        srcbuf[i].u32Width = gWinInfo[i].bmpInfo.bmpWidth;
        srcbuf[i].u32Height = gWinInfo[i].bmpInfo.bmpHeight;
        srcbuf[i].u32Pitch = _CalcPitch(gWinInfo[i].GeDstFmt, gWinInfo[i].bmpInfo.bmpWidth);
        MApi_GFX_SetSrcBufferInfo(&srcbuf[i], 0);
        bitbltInfo[i].srcblk.x = 0;
        bitbltInfo[i].srcblk.y = 0;
        bitbltInfo[i].srcblk.width = gWinInfo[i].bmpInfo.bmpWidth;
        bitbltInfo[i].srcblk.height = gWinInfo[i].bmpInfo.bmpHeight;

        if(i==0)
        {
            bitbltInfo[i].dstblk.x = *Draw_X;
            bitbltInfo[i].dstblk.y = *Draw_Y;
        }else
        {
            bitbltInfo[i].dstblk.x = *Draw_X1;
            bitbltInfo[i].dstblk.y = *Draw_Y1;
        }
        bitbltInfo[i].dstblk.width = gWinInfo[i].GopBitmapWidth;
        bitbltInfo[i].dstblk.height = gWinInfo[i].GopBitmapHeight;

        MApi_GFX_BitBlt(&bitbltInfo[i], u32Flag);

        MApi_GFX_FlushQueue();

        if(u32BmpAllocPhyAddr != 0x0)
        {
            MsOS_FreeMemory((void*)MS_PA2KSEG1(u32BmpAllocPhyAddr),gs32NonCachedPoolID);
            u32BmpAllocPhyAddr = 0x0;
        }

        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[i].GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return GWIN_FAIL;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use DWin to captrue a video frame
/// @param[in] CaptureOneFrame auto-stop capture after one frame has been captured.
/// @param[in] GWinDsipCount The count that using GWin to displpay the captured frame
/// @param[in] Source The source to capture.
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_DWinCapture \n
//------------------------------------------------------------------------------
MS_BOOL GOP_DWinCapture(MS_U32 *CaptureOneFrame, MS_U32 *GWinDsipCount, MS_U32 *Source)
{
    GOP_DwinProperty dwinProperty;
    MS_PHYADDR phyDWinBufAddr;
    MS_U8 u8IsInterlace;
    MS_BOOL bRetValue = TRUE;
    //Dwin buffer size set to panel width and height to capture the whole frame
    MS_U16 u16DWinBufWidth = 0;
    MS_U16 u16DWinBufHeight = 0;
    GOP_GwinFBAttr bufAttr;

    //Gwin size maybe different to panel size; GE_Bitblt with scaling to change size to display
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;
    GFX_BufferInfo srcbufInfo;
    GFX_BufferInfo dstbuf[2];
    GFX_DrawRect drawrectInfo;
    GFX_Point gfxPt0[2];
    GFX_Point gfxPt1[2];
    MS_U32 u32DispCount = *GWinDsipCount;
    MS_U8 i;


    //GOP address and add double buffer to avoid blink
    MS_U32 u32FBAddr0[2] = {0,0};//original GOP address for HD,SD
    MS_U32 u32FBAddr1[2] = {0,0};//second buffer GOP address for HD,SD
    GOP_GwinFBAttr gwinattr;
    void* dualgopbuf[2] = {NULL,NULL};

    if(0 == *Source)
    {
        //Source can only from Scaler OP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);
        //OP scan type is progressive
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_extern);
        u16DWinBufWidth = g_IPanel.Width();
        u16DWinBufHeight = g_IPanel.Height();
        printf("Source is OP, u16DWinBufWidth = %d, u16DWinBufHeight = %d\n", u16DWinBufWidth, u16DWinBufHeight);
    }
    else if(1 == *Source)
    {
        //Source can only from Scaler IP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_IP);
        //Is the DWIN source is IP, we need to know the video size and interlace or progressive
        if(FALSE == _get_video_info(&u8IsInterlace, &u16DWinBufWidth, &u16DWinBufHeight))
        {
            printf("Get video infomation fail.");
            return FALSE;
        }
        printf("Source is IP, u16DWinBufWidth = %d, u16DWinBufHeight = %d\n", u16DWinBufWidth, u16DWinBufHeight);

        //(1)SCAN type: DWIN_SCAN_MODE_PROGRESSIVE
        // - Contiguous captured/Auto stop captured: In each vsync, capture the top or bottom field
        //(2)SCAN type: DWIN_SCAN_MODE_extern
        // - Contiguous captured: contiguously captured top and bottom field
        // - Auto stop captured: Hardware will auto stop once the bottom field is captured.
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);

        //Interlaced video, 'dwinProperty.u16h' is set to 1/2 of video height
        //If the scan type is DWIN_SCAN_MODE_PROGRESSIVE, the dwin buffer size should be also set to 1/2
        //If the scan type is DWIN_SCAN_MODE_extern, the dwin buffer size should not change
        if(1 == u8IsInterlace)
        {
            u16DWinBufHeight = u16DWinBufHeight>>1;
        }
    }
    else
    {
        printf("This source number is not supported now.\n");
        return FALSE;
    }

    //Data format only YUV; Set data format to YUV422
    MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);

    if( GWIN_OK != _GOP_Create_FB(&u8DWinFbId, u16DWinBufWidth, u16DWinBufHeight, E_MS_FMT_YUV422))
    {
        printf("[%s][%d] Create FB fail\n", __FUNCTION__, __LINE__);
        u8DWinFbId = 0xFF;
        return FALSE;
    }

    MApi_GOP_GWIN_GetFBInfo(u8DWinFbId, &bufAttr);
    phyDWinBufAddr = bufAttr.addr;
    printf("[%s][%d] captured FB addr : 0x%lx\n", __FUNCTION__, __LINE__, phyDWinBufAddr);

    // Set DWin property
    dwinProperty.u16x = 0;
    dwinProperty.u16y = 0;
    dwinProperty.u16fbw = u16DWinBufWidth;
    dwinProperty.u16w = u16DWinBufWidth;
    if(0 == *Source)
    {
        /*  for OP source is interlace, dwin only capture half of height at a time */
        dwinProperty.u16h = u16DWinBufHeight/2;
    }
    else
    {
        dwinProperty.u16h = u16DWinBufHeight;
    }
    dwinProperty.u32fbaddr0 = phyDWinBufAddr;
    dwinProperty.u32fbaddr1 = phyDWinBufAddr + bufAttr.size;
    printf("[%s][%d] addr : 0x%lx, size : %ld\n", __FUNCTION__, __LINE__, phyDWinBufAddr, bufAttr.size);

    MApi_GOP_DWIN_SetWinProperty(&dwinProperty);

    //Enable Dwin to capture each frame if CaptureOneFrame is 0
    if( 0 == *CaptureOneFrame )
    {
        MApi_GOP_DWIN_Enable(TRUE);
    }

    //Create GOP double buffer to avoid blink
    for(i = SDPath; i < GOPCount; i++)
    {
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[i].u8FBId, &gwinattr);
        u32FBAddr0[i] = gwinattr.addr;

        if(u32DispCount > 1 || u32DispCount == 0)
        {
            MS_U32 u32wordunit;
            MApi_GOP_GetChipCaps(E_GOP_CAP_WORD_UNIT,&u32wordunit,0);
            dualgopbuf[i] = MsOS_AllocateMemory(_CalcPitch(gWinInfo[i].GeDstFmt,gWinInfo[i].GopBitmapWidth)*gWinInfo[i].GopBitmapHeight + u32wordunit,gs32NonCachedPoolID);

            if(dualgopbuf[i] == NULL)
            {
                printf("[%s][%d] Create Temp FB fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }

            //Adjust address alignment to avoid graph shift
            u32FBAddr1[i] = (MsOS_VA2PA((MS_U32)dualgopbuf[i]) + u32wordunit ) &(~(u32wordunit-1));
        }
    }

    do{
        //Auto-stop capture after one frame has been successfully captured if CaptureOneFrame is 1
        if( 0 != *CaptureOneFrame )
        {
            MApi_GOP_DWIN_CaptureOneFrame();
        }
        else
        {
            //delay for dwin capture ready
            MsOS_DelayTask(20);
        }
        //GE bitblt to copy DWin captured frame to GWin buffer to display
        for(i = SDPath; i < GOPCount; i++)
        {
            //Lock GE for portect setting
            MApi_GFX_BeginDraw();
            //Set Clip window for rendering
            gfxPt0[i].x = 0;
            gfxPt0[i].y = 0;
            gfxPt1[i].x = gWinInfo[i].GopBitmapWidth;
            gfxPt1[i].y = gWinInfo[i].GopBitmapHeight;
            MApi_GFX_SetClip(&gfxPt0[i], &gfxPt1[i]);

            //set dst buffer
            if(1 == (u32DispCount % 2))
            {
            MApi_GOP_GWIN_Switch2FB( gWinInfo[i].u8FBId );
            }
            else
            {
                dstbuf[i].u32ColorFmt = gWinInfo[i].GeDstFmt;
                dstbuf[i].u32Addr = u32FBAddr1[i];
                dstbuf[i].u32Pitch = _CalcPitch(gWinInfo[i].GeDstFmt, gWinInfo[i].GopBitmapWidth);
                MApi_GFX_SetDstBufferInfo(&dstbuf[i], 0);
            }

            // Bitble the bitmap to DRAM of GE
            srcbufInfo.u32ColorFmt = GFX_FMT_YUV422;
            srcbufInfo.u32Addr = phyDWinBufAddr;
            printf("[%s][%d] Src buffer addr = 0x%lx\n", __FUNCTION__, __LINE__, phyDWinBufAddr);
            srcbufInfo.u32Width = u16DWinBufWidth;
            if(0 == *Source)
            {
                srcbufInfo.u32Height = u16DWinBufHeight>> DWIN_OP2GE_SCALE_FACTOR;
                srcbufInfo.u32Pitch = _CalcPitch(E_MS_FMT_YUV422, (u16DWinBufWidth << DWIN_OP2GE_SCALE_FACTOR));
            }
            else
            {
                srcbufInfo.u32Height = u16DWinBufHeight;
                srcbufInfo.u32Pitch = _CalcPitch(E_MS_FMT_YUV422, (u16DWinBufWidth));
            }
            MApi_GFX_SetSrcBufferInfo(&srcbufInfo, 0);

            drawrectInfo.srcblk.x = 0;
            drawrectInfo.srcblk.y = 0;
            drawrectInfo.srcblk.width = u16DWinBufWidth;
            if(0 == *Source)
            {
                drawrectInfo.srcblk.height = u16DWinBufHeight>>DWIN_OP2GE_SCALE_FACTOR;
            }
            else
            {
                drawrectInfo.srcblk.height = u16DWinBufHeight;
            }

            drawrectInfo.dstblk.x = 0;
            drawrectInfo.dstblk.y = 0;
            drawrectInfo.dstblk.width = gWinInfo[i].GopBitmapWidth;
            drawrectInfo.dstblk.height = gWinInfo[i].GopBitmapHeight;

            MApi_GFX_BitBlt(&drawrectInfo, u32Flag);
            MApi_GFX_FlushQueue();

            //After GE bitblt then enable the GWin
            MApi_GOP_GWIN_SwitchGOP(gWinInfo[i].u8GOPId);

            if(1 == (u32DispCount % 2))
            {
                GOP_GwinInfo stGWin;
                MApi_GOP_GWIN_GetWinInfo(gWinInfo[i].GeWinId,&stGWin);
                stGWin.u32DRAMRBlkStart = u32FBAddr0[i];
                MApi_GOP_GWIN_SetWinInfo(gWinInfo[i].GeWinId,&stGWin);
            }
            else
            {
                GOP_GwinInfo stGWin;
                MApi_GOP_GWIN_GetWinInfo(gWinInfo[i].GeWinId,&stGWin);
                stGWin.u32DRAMRBlkStart = u32FBAddr1[i];
                MApi_GOP_GWIN_SetWinInfo(gWinInfo[i].GeWinId,&stGWin);
            }

            //Lock GE for portect setting
            MApi_GFX_EndDraw();
            if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[i].GeWinId, TRUE) )
            {
                 printf("[%s][%d] MApi_GOP_GWIN_Enable failed: GOP%d\n", __FUNCTION__, __LINE__, gWinInfo[i].GeWinId);
                 bRetValue = FALSE;
                 break;
            }
        }
    }while( --u32DispCount > 0 && bRetValue == TRUE);

    //Befire leaving this function, disable Dwin to capture each frame if CaptureOneFrame is 0
    if( 0 == *CaptureOneFrame )
    {
        MApi_GOP_DWIN_Enable(FALSE);
    }

    //Before leaving this function, FB has been created must delete
    for(i = SDPath; i < GOPCount; i++)
    {
        if(dualgopbuf[i] != NULL)
            MsOS_FreeMemory(dualgopbuf[i],gs32NonCachedPoolID);
    }
    MApi_GOP_GWIN_DeleteFB( u8DWinFbId );
    u8DWinFbId = 0xFF;
    return bRetValue;

}

//------------------------------------------------------------------------------
/// @brief The sample code to enable(visable) GWin
/// @param[in] Enable enable or disable the GWin
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_EnableWin \n
//------------------------------------------------------------------------------
MS_BOOL GOP_EnableWin(MS_U32 *Enable)
{
    MS_U8 i;

    for(i = SDPath; i < GOPCount; i++)
    {
        if (*Enable)
            MApi_GOP_GWIN_Enable( gWinInfo[i].GeWinId, TRUE);
        else
            MApi_GOP_GWIN_Enable( gWinInfo[i].GeWinId, FALSE);

    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to config global Alpha for a GWin
/// @param[in] Auto automatic to show the global alpha demo
/// @param[in] AlphaValue alpha value
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_SetGlobalAlpha \n
//------------------------------------------------------------------------------
MS_BOOL GOP_SetGlobalAlpha(MS_U32 *Auto, MS_U32 *AlphaValue)
{
    MS_U32 index;
    MS_U8 i;

    if (*Auto)
    {
        // - Automatic show the effect of global alpha
        for ( index = 0 ; index < 255 ; index += 32)
        {
            for(i = SDPath; i < GOPCount; i++)
            {
                MApi_GOP_GWIN_SetBlending(gWinInfo[i].GeWinId, FALSE, index); //set transparent color
            }
            MsOS_DelayTask(u32delay_time);
        }
        for(i = SDPath; i < GOPCount; i++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[i].GeWinId, FALSE, 0xff);
        }
    }
    else
    {
        // - Assign the global alpha value
        //Alpha Range 0 ~ 255
        for(i = SDPath; i < GOPCount; i++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[i].GeWinId, FALSE, *AlphaValue);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable individual alpha value of an image in a GWin
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_EnablePerPixelAlpha \n
//------------------------------------------------------------------------------
MS_BOOL GOP_EnablePerPixelAlpha(MS_U32 *Auto, MS_U32 *AlphaValue)
{
    MS_U32 index;
    MS_U8 i;

    if (*Auto)
    {
        // - Automatic show the effect of global alpha
        for ( index = 0 ; index < 255 ; index += 32)
        {
            for(i = SDPath; i < GOPCount; i++)
            {
                MApi_GOP_GWIN_SetBlending(gWinInfo[i].GeWinId, TRUE, index); //set transparent color
            }
            MsOS_DelayTask(u32delay_time);
        }
        for(i = SDPath; i < GOPCount; i++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[i].GeWinId, TRUE, 0xff);
        }
    }
    else
    {
        // - Assign the global alpha value
        //Alpha Range 0 ~ 255
        for(i = SDPath; i < GOPCount; i++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[i].GeWinId, TRUE, *AlphaValue);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show the fade effect for a GWin
/// @param[in] FadeType Fade type
/// @param[in] bEnable Enable the fade effect
/// @param[in] Rate Fade rate
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_SetFade \n
//------------------------------------------------------------------------------
MS_BOOL GOP_SetFade(MS_U32 *FadeType, MS_U32 *bEnable, MS_U32 *Rate)
{
    EN_GOP_FADE_TYPE eFADE_Type = (EN_GOP_FADE_TYPE)*FadeType;
    MS_U8 i;

    for(i = SDPath; i < GOPCount; i++)
    {
        MApi_GOP_GWIN_SetFadeInOut(gWinInfo[i].GeWinId, eFADE_Type, *bEnable, *Rate);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to free(destory) GWin
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_FreeWin \n
//------------------------------------------------------------------------------
MS_BOOL GOP_FreeWin(void)
{
    MS_U8 i;

    for(i = SDPath; i < GOPCount; i++)
    {
        MApi_GOP_GWIN_DeleteWin(gWinInfo[i].GeWinId);
    }

    if(u32BmpAllocPhyAddr != 0x0)
    {
        MsOS_FreeMemory((void*)MS_PA2KSEG1(u32BmpAllocPhyAddr),gs32NonCachedPoolID);
        u32BmpAllocPhyAddr = 0x0;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change two GOPs & video display layer
/// @param[in] Layer_select For select display layer type
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_Zlist \n
//------------------------------------------------------------------------------
MS_BOOL GOP_Zlist(MS_U32 *Layer_select)
{
    GOP_MuxConfig pGopMuxConfig;

    //**Adjust GOP to mux1&mux2(Only mux1&mux2 with zlist function)***//
    //**Notice: For avoiding multiple gops set on one mux, the following code will set other GOPs to indicate mux**//
    pGopMuxConfig.GopMux[0].u8GopIndex=0;
    pGopMuxConfig.GopMux[0].u8MuxIndex=1;
    pGopMuxConfig.GopMux[1].u8GopIndex=1;
    pGopMuxConfig.GopMux[1].u8MuxIndex=2;
    pGopMuxConfig.GopMux[2].u8GopIndex=2;
    pGopMuxConfig.GopMux[2].u8MuxIndex=3;
    pGopMuxConfig.GopMux[3].u8GopIndex=3;
    pGopMuxConfig.GopMux[3].u8MuxIndex=0;
    pGopMuxConfig.u8MuxCounts=4;
    MApi_GOP_GWIN_SetMux(&pGopMuxConfig, sizeof(GOP_MuxConfig));

    if(E_APIXC_RET_FAIL == MApi_XC_SetOSDLayer((E_VOP_OSD_LAYER_SEL)*Layer_select,MAIN_WINDOW))
    {
        printf("[%s][%d] MApi_XC_SetOSDLayer failed", __FUNCTION__, __LINE__);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the video alpha
/// @param[in]
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GOP_Zlist \n
//------------------------------------------------------------------------------
MS_BOOL GOP_SetVideoAlpha(MS_U8 *u8Alpha)
{
    return TRUE;
}

MS_BOOL GOP_IsGOPInit(void)
{
    return bGOP_init;
}

