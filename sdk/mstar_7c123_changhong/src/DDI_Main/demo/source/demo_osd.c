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
/// @file   demo_osd.c
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
/// Usage:GOP_DrawBmp
///
/// Sample Command:GOP_DrawBmp
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
///
///-----------------------------------------------------------------------------
///@endverbatim
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
   @file Demo_OSD.c
   @brief The Template code for the MDrv_XXX functions

*/
#if (DEMO_OSD_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/string.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "apiGFX.h"
#include "apiGOP.h"
#include "drvSEM.h"
#include "MsOS.h"
#include "apiVDEC_EX.h"

//Scalar
#include "MsTypes2.h"
//#include "ver_dtv_comm.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include "apiHDMITx.h"
#include "Panel.h"
#include "apiPNL.h"
#include "apiJPEG.h"
#include "drvMMIO.h"
#include "drvDTC.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#if defined(MSOS_TYPE_LINUX)
#include "iniparser.h"
#endif

#if (DEMO_GOP_GOPSC_TEST == 1)
#include "apiGOPSC_Ex.h"
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiPNL_EX.h"
#endif

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
#include "apiXC_DWIN.h"
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif

#include "demo_utility.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_osd.h"
#include "demo_osd_resource.h"

#if (DEMO_MM_TEST == 1)
#include "mmsdk_interface_def.h"
#include "demo_mm.h"
#endif

#if (DEMO_GPD_TEST == 1)
#include "apiGPD.h"
#include "demo_gpd.h"
#endif

#if (DEMO_ZUI_TEST == 1)
extern MS_U32 msAPI_OCP_GetBitmapInfo(MS_S16 handle, GFX_BitmapInfo* pinfo);
extern MS_U32 msAPI_OCP_GetFontInfo(MS_S8 handle, GFX_FontInfo* pinfo);
#endif
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define OSD_DBG            0

#if OSD_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

static MS_U32 u32GOP0 = 0;
static MS_U32 u32GOP1 = 1;

//#define GWIN_WIDTH 400
//#define GWIN_HEIGHT 400

#define BMPRGB565SIZE    (BMPRGB565W*BMPRGB565H*2ul)
#define BMPARGB4444SIZE  (BMPRGB4444W*BMPRGB4444H*2ul)
#define BMPARGB8888SIZE  (BMPRGB8888W*BMPRGB8888H*4ul)
#define BMPI8SIZE        (BMPRGBI8W*BMPRGBI8H)
#if (DEMO_GOP_3DOSD_TEST == 1)
#define BMP3DSIZE        (BMP3DW*BMP3DH*4ul)
#endif

#define BMPRGB565W    400ul
#define BMPRGB565H    400ul
#define BMPRGB4444W   400ul
#define BMPRGB4444H   400ul
#define BMPRGB8888W   400ul
#define BMPRGB8888H   400ul
#define BMPRGBI8W     400ul
#define BMPRGBI8H     400ul
#if (DEMO_GOP_3DOSD_TEST == 1)
#define BMP3DW        384ul
#define BMP3DH        432ul
#endif

//font size
#define FONTI1SIZE    0x800
#define FONTI2SIZE    0x1800
#define FONTI4SIZE    0x3000

//bitmap items
#if (DEMO_GOP_3DOSD_TEST == 1)
#define BMPNUM           6
#else
#define BMPNUM           4
#endif
#define ARGB8888BMP    0x1
#define RGB565BMP      0x2
#define ARGB4444BMP    0x4
#define I8BMP          0x8
#if (DEMO_GOP_3DOSD_TEST == 1)
#define L3DBMP         0x10
#define R3DBMP         0x20
#endif

//font items
#define FONTNUM     3
#define FontI1    0x1
#define FontI2    0x2
#define FontI4    0x4

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
unsigned char _FONTI1BIN_2Miu0[]  =
{
    #include "../data/ftMstar_I1.dat"
};

unsigned char _FONTI2BIN_2Miu0[]  =
{
    #include "../data/ftMstar_I2.dat"
};

unsigned char _FONTI4BIN_2Miu0[]  =
{
    #include "../data/ftMstar_I4.dat"
};

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32BmpDramRblkStart;
    MS_U16 u16bmpWidth;
    MS_U16 u16bmpHeight;
}DrawBmpInfo;

typedef struct
{
    EN_GOP_DST_TYPE eGopDest;
    MS_U8 u8GOPId;
    MS_U8 u8GeWinId;
    MS_U8 u8FBId;
    MS_U16 u16GopBitmapWidth;
    MS_U16 u16GopBitmapHeight;
    MS_U16 u16GeDstFmt;
    DrawBmpInfo stbmpInfo;
}GWinInfo;

typedef enum
{
    E_BMP_ARGB8888,
    E_BMP_RGB565,
    E_BMP_ARGB4444,
    E_BMP_I8,
#if (DEMO_GOP_3DOSD_TEST == 1)
    E_BMP_3D_L,
    E_BMP_3D_R,
#endif
}BMPITEM;

typedef enum
{
    E_FONT_I1,
    E_FONT_I2,
    E_FONT_I4,
}FONTITEM;

typedef struct
{
    BMPHANDLE hBmp;
    GFX_Buffer_Format BmpClrFmt;
}BmpInfo;

BmpInfo bmpinfo[BMPNUM];
FONTHANDLE hFontHandle[FONTNUM];

#if (DEMO_MM_TEST == 1)
static MS_U8 PhotoDispDone = 0;
#endif

static MS_U8 u8GOPCount = 0;
static MS_U8 u8DrawGOPCount = 0;

static MS_U16 u16FontWidth = 50;
static MS_U16 u16FontHeight = 50;
static MS_U16 u16FontDis = 50;

static MS_U8 u8BmpCnt = 0;
static MS_U8 u8FontCnt = 0;
static MS_U32 u32BmpDRAMStrAddr;
static MS_U32 u32FontDRAMStrAddr;

//each font bitmap
static BMPHANDLE  hBmpRGB565;
static BMPHANDLE  hBmpARGB4444;
static BMPHANDLE  hBmpARGB8888;
static BMPHANDLE  hBmpI8;

//each font handle
static FONTHANDLE hFontI1;
static FONTHANDLE hFontI2;
static FONTHANDLE hFontI4;

//each bitmap dram adr
static MS_U32 u32BmpRGB565Addr;
static MS_U32 u32BmpARGB4444Addr;
static MS_U32 u32BmpARGB8888Addr;
static MS_U32 u32BmpI8Addr;
static MS_U32 u32BmpUV8Y8Addr;
#if (DEMO_GOP_3DOSD_TEST == 1)
static MS_U32 u32Bmp3DLAddr;
static MS_U32 u32Bmp3DRAddr;
#endif

//each font dram adr
static MS_U32 u32FontI1Addr;
static MS_U32 u32FontI2Addr;
static MS_U32 u32FontI4Addr;

//static MS_U8 gwin_num = 0xFF;
static MS_U32 u32delay_time = 500;
//static MS_U8 gwin_fmt[]={E_MS_FMT_ARGB4444, E_MS_FMT_ARGB4444};
static MS_U16 u16gwin_fmt =(MS_U16) E_MS_FMT_ARGB4444;
static MS_BOOL bSetClrFmt = TRUE;
static GWinInfo gWinInfo[] = {{E_GOP_DST_OP0, 0xFF, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB4444, {0}},
                        {E_GOP_DST_MIXER2VE, 0xFF, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB4444, {0}}};
#if (DEMO_GOP_ZORDER_TEST == 1)
static GWinInfo cusGwinInfo[] = {
                                    {E_GOP_DST_OP0, 0, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB8888, {0}},
                                    {E_GOP_DST_OP0, 1, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB8888, {0}},
                                    {E_GOP_DST_OP0, 2, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB8888, {0}},
                                    {E_GOP_DST_OP0, 3, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB8888, {0}},
                                    {E_GOP_DST_OP0, 4, 0xFF, 0xFF, 0, 0, E_MS_FMT_ARGB8888, {0}}
                                };
static MS_U32 s_u32VideoLayer = 0;
static MS_U32 s_u32GOPNum = 0;
static MS_U8 cfgFileName[] = "/config/DMS/dms_cfg.ini";
#endif
static MS_BOOL bGOPInit = FALSE;
static MS_U8 u8DWinFbId = 0xFF;
static GOP_MIXER_TIMINGTYPE gopMixerTimingType;
static GOP_VE_TIMINGTYPE gopVETimingType;

#if (DEMO_XC_DUALXC_TEST == 1)
static PNL_DeviceId gDevId = {0,E_PNL_EX_DEVICE0};
#endif

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
#define DIP_Timeout 500 //(ms)
static MS_U32 u32DIPStartTime = 0;
static MS_BOOL bDwinInit = FALSE;
static GOP_DwinProperty DwinProperty;
static EN_GOP_DWIN_SRC_SEL SrcSlt =DWIN_SRC_OP;
static EN_XC_DIP_SCAN_MODE ScanMode = DIP_SCAN_MODE_PROGRESSIVE;
EN_XC_DIP_DATA_FMT DataFmt = DIP_DATA_FMT_YUV422;
#endif

extern unsigned char _RGB565BMPBIN[];
extern unsigned char _ARGB4444BMPBIN[];
extern unsigned char _ARGB8888BMPBIN[];
extern unsigned char _I8BMPBIN[];
#if (DEMO_GOP_3DOSD_TEST == 1)
extern unsigned char _3DLBMPBIN[];
extern unsigned char _3DRBMPBIN[];
#endif

//this palette is used for I8 bitmap testing
extern GOP_PaletteEntry _gopI8PaletteEntry[];

///=== START=== Remove WHEN SC1 HardCode Ready=== START===////
MS_U32 u32NonPmBase = 0;
MS_U32 u32PmBase = 0;

#define REG16_NPM(addr)         *((volatile MS_U16*)(u32NonPmBase + (addr)))
#define REG16_PM(addr )         *((volatile MS_U16*)(u32PmBase+ (addr)))

extern void* Demo_VDEC_GetStreamID(EN_VDEC_Device* pedevice);
#if 0
void _appDemo_XC_WriteByte(MS_U32 u32Addr, MS_U8 u8Value)
{
    u32Addr = (((u32Addr & 0xFFFFF) << 1) - (u32Addr & 0x1));
    REG16_NPM(u32Addr) = u8Value | (REG16_NPM(u32Addr) & 0xFF00);
    return;
}

void _appDemo_XC_WriteByteMask(MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask)
{
    u32Addr = (((u32Addr & 0xFFFFF) << 1) - (u32Addr & 0x1));
    REG16_NPM(u32Addr) = (u8Value & u8Mask) | (REG16_NPM(u32Addr) & (~u8Mask));
    return;
}

void _appDemo_XC_Write2ByteMask(MS_U32 u32Addr, MS_U16 u16Value, MS_U16 u16Mask)
{
    u32Addr = (((u32Addr & 0xFFFFF) << 1) - (u32Addr & 0x1));
    REG16_NPM(u32Addr) = (u16Value & u16Mask) | (REG16_NPM(u32Addr) & (~u16Mask));
    return;
}

void _appDemo_SC_Init_Hardcode(void)
{

    MS_U32 u32MMIOBaseAdr, u32BankSize;

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
    }
    else
    {
        printf("Get IOMap u32NonPmBase = 0x%lx \n",(MS_U32)u32MMIOBaseAdr);
    }

    u32NonPmBase = u32MMIOBaseAdr;

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_PM))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
    }
    else
    {
        printf("Get IOMap u32PmBase = 0x%lx \n",(MS_U32)u32MMIOBaseAdr);
    }

    u32PmBase = u32MMIOBaseAdr;

//******below setting need check SC is implement before remove*********//
    _appDemo_XC_WriteByte(0x102f00, 0x0f);
    _appDemo_XC_WriteByte(0x102f8c, 0x20);   //OP0 capture stage to the top one
    if (GOP_REG_DMA_BASE_MEMORY_TYPE & MIU1)
    {
        _appDemo_XC_WriteByteMask(0x1012F7, 0x18, 0x18);   //GOP45 to MIU1
        _appDemo_XC_WriteByteMask(0x1012F6, 0x3C, 0x3C);   //GOP3 to MIU1
    }
    else
    {
        _appDemo_XC_WriteByteMask(0x1012F7, 0x0, 0x18);  //GOP45 to MIU0
        _appDemo_XC_WriteByteMask(0x1012F6, 0x0, 0x3C);  //GOP3 to MIU0
    }

//*********************************************************//
}
#endif
///=== END=== Remove WHEN SC1 HardCode Ready=== END===////

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
#if 0
static void _GOP_ScalerInit(void)
{
    /*-------------XC Init----------------*/
    MS_U32 deviceId = 0;
    Demo_XC_Init(&deviceId);

    /*-------------VE path init----------------*/
    deviceId = 1;
    Demo_XC_Init(&deviceId);

    _appDemo_SC_Init_Hardcode();
}
#endif
static MS_U32 _OSD_SetFBFmt(MS_U16 u16pitch, MS_PHY u32addr , MS_U16 u16fmt )
{
    GFX_BufferInfo stdstbuf;
    printf("OSD set format\n");
    printf("format = 0x%x\n",u16fmt);
    stdstbuf.u32ColorFmt = (GFX_Buffer_Format)(u16fmt&0xff);
    stdstbuf.u32Addr = u32addr;
    stdstbuf.u32Pitch = u16pitch;
    printf("SetFBFmt: %"DTC_MS_U32_x",  %"DTC_MS_PHY_x", %"DTC_MS_U32_x"\n", (MS_U32)stdstbuf.u32ColorFmt,stdstbuf.u32Addr,stdstbuf.u32Pitch);
    MApi_GFX_SetDstBufferInfo(&stdstbuf, 0);

    return 0;
}

static void _XC_Sys_PQ_ReduceBW_ForOSD(MS_U8 u8PqWin, MS_BOOL bOSD_On)
{
    u8PqWin=bOSD_On=0;
}

static MS_U16 _sc_get_h_cap_start(void)
{
    // this function should support "multiple process"
    MS_WINDOW_TYPE stCapWin;

    // this function only used when OSD input from IP, so if no input, OSD should be attached to OP
    MApi_XC_GetCaptureWindow(&stCapWin, MAIN_WINDOW);       // read from register directly

    return stCapWin.x;
}

static EN_AV_Device _Get_SCInputSource(E_DEST_TYPE edest)
{
    XC_MUX_PATH_INFO stPaths[MAX_SYNC_DATA_PATH_SUPPORTED];
    MS_U8 u8Count;

    memset(stPaths, 0, sizeof(XC_MUX_PATH_INFO)*MAX_SYNC_DATA_PATH_SUPPORTED);

    MApi_XC_Mux_GetPathInfo(stPaths);

    for(u8Count=0; u8Count<MAX_SYNC_DATA_PATH_SUPPORTED;u8Count++)
    {
        if(stPaths[u8Count].dest == edest)
        {
            if(stPaths[u8Count].src == INPUT_SOURCE_DTV2)
            {
                return E_AV_DEVICE_SUB;
            }
            else
            {
                return E_AV_DEVICE_MAIN;
            }
        }
    }

    return E_AV_DEVICE_MAX;
}

static MS_BOOL _sc_is_interlace(void)
{
    VDEC_EX_DispInfo stinfo;
    VDEC_StreamId *stVDECSteamID;
    EN_AV_Device eSC0_Source;

    eSC0_Source = _Get_SCInputSource(OUTPUT_SCALER_MAIN_WINDOW);

    if(eSC0_Source == E_AV_DEVICE_MAX)
    {
        db_print(" Video Get Input source fail!! \n");
        return FALSE;
    }

    stVDECSteamID = Demo_VDEC_GetStreamID((EN_VDEC_Device*)&eSC0_Source);

    memset(&stinfo, 0, sizeof(VDEC_EX_DispInfo));

    if(E_VDEC_EX_OK !=  MApi_VDEC_EX_GetDispInfo(stVDECSteamID,&stinfo))
    {
        printf(" Video Get Info Failed!! \n");
        return FALSE;
    }

    //printf("[%s][%d]Interlace = %u \n", __FUNCTION__, __LINE__,info.u8Interlace);
    return stinfo.u8Interlace;
}

static MS_BOOL _get_video_info(MS_U8 *u8Interlace, MS_U16 *u16HSize, MS_U16 *u16VSize, MS_U16 *u16CropBottom, E_DEST_TYPE edest)
{
    VDEC_EX_DispInfo   stinfo;
    VDEC_EX_Result     eret;
    VDEC_StreamId *stVDECSteamID;
    EN_AV_Device eSC_Source;

    eSC_Source = _Get_SCInputSource(edest);

    if(eSC_Source == E_AV_DEVICE_MAX)
    {
        printf(" Video Get Input source fail!! \n");
        return FALSE;
    }

    stVDECSteamID = Demo_VDEC_GetStreamID((EN_VDEC_Device*)&eSC_Source);

    memset(&stinfo, 0, sizeof(VDEC_EX_DispInfo));
    eret = MApi_VDEC_EX_GetDispInfo(stVDECSteamID,&stinfo);

    if (E_VDEC_EX_OK != eret)
    {
        return FALSE;
    }
    else
    {
        *u8Interlace = stinfo.u8Interlace;
        *u16HSize = stinfo.u16HorSize;
        *u16VSize = stinfo.u16VerSize;
        *u16CropBottom = stinfo.u16CropBottom;
        //printf("[%s][%d]Interlace = %u , H = %d, V = %d\n", __FUNCTION__, __LINE__, *bInterlace, *HSize, *VSize);
        return TRUE;
    }
}
static MS_U32 _GOP_Create_FB(MS_U8 *u8FB, MS_U16 u16FBWidth, MS_U16 u16FBHeight, MS_U16 u16FBFmt)
{
    GOP_GwinFBAttr stDstFBInfo;
    // look for available Sdram slot
    *u8FB = MApi_GOP_GWIN_GetFreeFBID();
    if(MAX_GWIN_FB_SUPPORT <= *u8FB)
        return GWIN_NO_AVAILABLE;

    printf("Create FB %d with size (%d,%d) \n", *u8FB, u16FBWidth, u16FBHeight);
    if (MApi_GOP_GWIN_CreateFB( *u8FB, 0, 0, u16FBWidth, u16FBHeight, u16FBFmt) != GWIN_OK)
    {
        printf("[%s][%d] MApi_GOP_GWIN_CreateFB: failed to create FB \n", __FUNCTION__, __LINE__);
        return GWIN_NO_AVAILABLE;
    }

    MApi_GOP_GWIN_GetFBInfo( *u8FB, &stDstFBInfo);
    if(u16FBFmt==(MS_U16)E_MS_FMT_YUV422)
    {
        MApi_GFX_ClearFrameBufferByWord(stDstFBInfo.addr, stDstFBInfo.size, 0x80108010);
    }
    else
    {
        MApi_GFX_ClearFrameBuffer(stDstFBInfo.addr, stDstFBInfo.size, 0x00);
    }

    MApi_GFX_FlushQueue();

    return 0;
}

static MS_U32 _GOP_Create_GWin(MS_U8 u8GOP, MS_U8 *u8GWin, MS_U8 u8FB, MS_U16 u16FBFmt)
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
    if( u16FBFmt == E_MS_FMT_ARGB8888 || u16FBFmt == E_MS_FMT_ARGB4444)
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

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
static void GOP_XC_Create3RDPath(INPUT_SOURCE_TYPE_t eSrcPath)
{
    XC_MUX_PATH_INFO stPath_Info; // the path information for set create path on scaler

    stPath_Info.DestOnOff_Event_Handler = NULL;
    stPath_Info.dest_periodic_handler = NULL;
    stPath_Info.path_thread = NULL;
    stPath_Info.SyncEventHandler = NULL;
    stPath_Info.Path_Type = PATH_TYPE_SYNCHRONOUS;
    stPath_Info.dest = OUTPUT_SCALER_DWIN;
    stPath_Info.src = eSrcPath;//INPUT_SOURCE_DTV3;
    MApi_XC_Mux_CreatePath(&stPath_Info, sizeof(XC_MUX_PATH_INFO));
    MApi_XC_DIP_SetInputSource(eSrcPath,DIP_WINDOW);

}

static void GOP_XC_Set3RDWindow(INPUT_SOURCE_TYPE_t eSrcPath,MS_BOOL bInterlace,MS_U16 u16SrcX,MS_U16 u16SrcY,MS_U16 u16SrcW,MS_U16 u16SrcH,MS_U16 u16DstW,MS_U16 u16DstH)
{
    XC_SETWIN_INFO stXC_SetWin_Info;

    memset(&stXC_SetWin_Info, 0, sizeof(XC_SETWIN_INFO));

    stXC_SetWin_Info.enInputSourceType = eSrcPath;
    stXC_SetWin_Info.stCapWin.x = u16SrcX;
    stXC_SetWin_Info.stCapWin.y = u16SrcY;
    stXC_SetWin_Info.stCapWin.width = u16SrcW;
    stXC_SetWin_Info.stCapWin.height = u16SrcH;

    stXC_SetWin_Info.bPreHCusScaling = TRUE;
    stXC_SetWin_Info.u16PreHCusScalingSrc = u16SrcW;
    stXC_SetWin_Info.u16PreHCusScalingDst = u16DstW;

    stXC_SetWin_Info.bPreVCusScaling = TRUE;
    stXC_SetWin_Info.u16PreVCusScalingSrc = u16SrcH;
    stXC_SetWin_Info.u16PreVCusScalingDst = u16DstH;
    stXC_SetWin_Info.bInterlace     = bInterlace;

    MApi_XC_DIP_SetWindow(&stXC_SetWin_Info, sizeof(XC_SETWIN_INFO), DIP_WINDOW);

}
#endif

void _GOP_InitGlobalVar(void)
{
    u32BmpDRAMStrAddr = MEM_ADR_BY_MIU(SDRAM_ADR_BMP_ADR, SDRAM_ADR_BMP_MEMORY_TYPE); //temp adr for testing
    u32FontDRAMStrAddr = MEM_ADR_BY_MIU(SDRAM_ADR_FONT_ADR, SDRAM_ADR_FONT_MEMORY_TYPE);

    memset (&bmpinfo, 0xFF, sizeof (bmpinfo));
    memset (&hFontHandle, 0xFF, sizeof (hFontHandle));
}

void _GOP_SetBmpFontAdr(void)
{
    u32BmpARGB8888Addr=u32BmpDRAMStrAddr;
    u32BmpRGB565Addr= GE_ADDR_ALIGNMENT(u32BmpARGB8888Addr + BMPARGB8888SIZE);
    u32BmpARGB4444Addr= GE_ADDR_ALIGNMENT(u32BmpRGB565Addr + BMPRGB565SIZE);
    u32BmpI8Addr= GE_ADDR_ALIGNMENT(u32BmpARGB4444Addr + BMPARGB4444SIZE);
#if (DEMO_GOP_3DOSD_TEST == 1)
    u32Bmp3DLAddr = GE_ADDR_ALIGNMENT(u32BmpI8Addr + BMPI8SIZE);
    u32Bmp3DRAddr = GE_ADDR_ALIGNMENT(u32Bmp3DLAddr + BMP3DSIZE);
    u32BmpUV8Y8Addr = GE_ADDR_ALIGNMENT(u32Bmp3DRAddr + BMP3DSIZE);
#else
    u32BmpUV8Y8Addr = GE_ADDR_ALIGNMENT(u32BmpI8Addr + BMPI8SIZE);
#endif

    u32FontI1Addr=u32FontDRAMStrAddr;
    u32FontI2Addr= GE_ADDR_ALIGNMENT(u32FontI1Addr + FONTI1SIZE);
    u32FontI4Addr= GE_ADDR_ALIGNMENT(u32FontI2Addr + FONTI2SIZE);
}

static void _Bitmap2Miu0(BMPITEM eBmpOpt)
{
    //BININFO BinInfo;

    switch(eBmpOpt)
    {
        case E_BMP_ARGB8888:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpARGB8888Addr), (void*)_ARGB8888BMPBIN, BMPARGB8888SIZE );
            hBmpARGB8888 = Demo_OSD_RESOURCE_LoadBitmap(u32BmpARGB8888Addr, BMPARGB8888SIZE, BMPRGB8888W, BMPRGB8888H, BMP_FMT_ARGB8888);
            bmpinfo[u8BmpCnt].hBmp = hBmpARGB8888;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_ARGB8888;
            break;
        }
        case E_BMP_RGB565:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpRGB565Addr), (void*)_RGB565BMPBIN, BMPRGB565SIZE );
            hBmpRGB565 = Demo_OSD_RESOURCE_LoadBitmap(u32BmpRGB565Addr, BMPRGB565SIZE, BMPRGB565W, BMPRGB565H, BMP_FMT_RGB565);
            bmpinfo[u8BmpCnt].hBmp = hBmpRGB565;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_RGB565;
            break;
        }
        case E_BMP_ARGB4444:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpARGB4444Addr), (void*)_ARGB4444BMPBIN, BMPARGB4444SIZE );
            hBmpARGB4444 = Demo_OSD_RESOURCE_LoadBitmap(u32BmpARGB4444Addr, BMPARGB4444SIZE, BMPRGB4444W, BMPRGB4444H, BMP_FMT_ARGB4444);
            bmpinfo[u8BmpCnt].hBmp = hBmpARGB4444;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_ARGB4444;
            break;
        }
        case E_BMP_I8:
        {
            memcpy((void*)MS_PA2KSEG1(u32BmpI8Addr), (void*)_I8BMPBIN, BMPI8SIZE );
            hBmpI8 = Demo_OSD_RESOURCE_LoadBitmap(u32BmpI8Addr, BMPI8SIZE, BMPRGBI8W, BMPRGBI8H, BMP_FMT_I8);
            bmpinfo[u8BmpCnt].hBmp = hBmpI8;
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_I8;
            break;
        }
#if (DEMO_GOP_3DOSD_TEST == 1)
        case E_BMP_3D_L:
        {
            memcpy((void*)MS_PA2KSEG1(u32Bmp3DLAddr), (void*)_3DLBMPBIN, BMP3DSIZE );
            Demo_OSD_RESOURCE_LoadBitmap(u32Bmp3DLAddr, BMP3DSIZE, BMP3DW, BMP3DH, BMP_FMT_ARGB8888);
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_ARGB8888;
            break;
        }
        case E_BMP_3D_R:
        {
            memcpy((void*)MS_PA2KSEG1(u32Bmp3DRAddr), (void*)_3DRBMPBIN, BMP3DSIZE );
            Demo_OSD_RESOURCE_LoadBitmap(u32Bmp3DRAddr, BMP3DSIZE, BMP3DW, BMP3DH, BMP_FMT_ARGB8888);
            bmpinfo[u8BmpCnt].BmpClrFmt = GFX_FMT_ARGB8888;
            break;
        }
#endif
        default:
        {
            printf("\nerror bitmap handle");
            break;
        }
    }
    u8BmpCnt++;
}

static void _Font2Miu0(FONTITEM eFontOpt)
{
    //BININFO BinInfo;

    switch(eFontOpt)
    {
        case E_FONT_I1:
        {
            if(sizeof(_FONTI1BIN_2Miu0)<FONTI1SIZE)
            {
                memcpy((void*)MS_PA2KSEG1(u32FontI1Addr), (void*)_FONTI1BIN_2Miu0, sizeof(_FONTI1BIN_2Miu0));
            }
            else
            {
                memcpy((void*)MS_PA2KSEG1(u32FontI1Addr), (void*)_FONTI1BIN_2Miu0, FONTI1SIZE);
            }
            hFontI1 = Demo_OSD_RESOURCE_LoadFont(u32FontI1Addr, u32FontI1Addr, 0, 0, 10, 0, 17, 16, 1, 0, 2);
            hFontHandle[u8FontCnt] = hFontI1;
            break;
        }
        case E_FONT_I2:
        {
            if(sizeof(_FONTI2BIN_2Miu0)<FONTI2SIZE)
            {
                memcpy((void*)MS_PA2KSEG1(u32FontI2Addr), (void*)_FONTI2BIN_2Miu0, sizeof(_FONTI2BIN_2Miu0));
            }
            else
            {
                memcpy((void*)MS_PA2KSEG1(u32FontI2Addr), (void*)_FONTI2BIN_2Miu0, FONTI2SIZE);
            }
            hFontI2 = Demo_OSD_RESOURCE_LoadFont(u32FontI2Addr, u32FontI2Addr, 0, 0, 10, 0, 17, 24, 2, 0, 2);
            hFontHandle[u8FontCnt] = hFontI2;
            break;
        }
        case E_FONT_I4:
        {
            if(sizeof(_FONTI4BIN_2Miu0)<FONTI4SIZE)
            {
                memcpy((void*)MS_PA2KSEG1(u32FontI4Addr), (void*)_FONTI4BIN_2Miu0, sizeof(_FONTI4BIN_2Miu0));
            }
            else
            {
                memcpy((void*)MS_PA2KSEG1(u32FontI4Addr), (void*)_FONTI4BIN_2Miu0, FONTI4SIZE);
            }
            hFontI4 = Demo_OSD_RESOURCE_LoadFont(u32FontI4Addr, u32FontI4Addr, 0, 0, 10, 0, 17, 24, 4, 0, 2);
            hFontHandle[u8FontCnt] = hFontI4;
            break;
        }
        default:
        {
            printf("\nerror bitmap handle~~~~~");
            break;
        }
    }
    u8FontCnt++;
}

void _LoadBitmap(MS_U16 u16BmpItem)
{
    MS_U8 u8Idx;

    for (u8Idx=0; u8Idx<BMPNUM; u8Idx++)
    {
        switch(u16BmpItem&(1<<u8Idx))
        {
             case ARGB8888BMP:
                _Bitmap2Miu0(E_BMP_ARGB8888);
                printf("\nload hBmpARGB8888:%d\n",hBmpARGB8888);
                break;

            case RGB565BMP:
                _Bitmap2Miu0(E_BMP_RGB565);
                printf("\nload hBmpRGB565:%d\n",hBmpRGB565);
                break;

            case ARGB4444BMP:
                _Bitmap2Miu0(E_BMP_ARGB4444);
                printf("\nload hBmpARGB4444:%d\n",hBmpARGB4444);
                break;

            case I8BMP:
                _Bitmap2Miu0(E_BMP_I8);
                printf("\nload hBmpI8:%d\n",hBmpI8);
                break;
#if (DEMO_GOP_3DOSD_TEST == 1)
            case L3DBMP:
                _Bitmap2Miu0(E_BMP_3D_L);
                break;
            case R3DBMP:
                _Bitmap2Miu0(E_BMP_3D_R);
                break;
#endif
            default:
                break;
        }
    }
}

void _LoadFont(MS_U16 u16FontItem)
{
    MS_U8 u8Idx;

    printf("\nstart load font from flash to dram :)");

    for (u8Idx=0; u8Idx<FONTNUM; u8Idx++)
    {
        switch(u16FontItem&(1<<u8Idx))
        {
            case FontI1:
                _Font2Miu0(E_FONT_I1);
                printf("\nFontI1 handle:%d",hFontI1);
                break;

            case FontI2:
                _Font2Miu0(E_FONT_I2);
                printf("\nFontI2 handle:%d",hFontI2);
                break;

            case FontI4:
                _Font2Miu0(E_FONT_I4);
                printf("\nFontI4 handle:%d",hFontI4);
                break;
        }
    }
}

static void _SetGfxPalette(void)
{
    GFX_PaletteEntry _gePalette[GOP_PALETTE_ENTRY_NUM];
    MS_U16 u16Count;

    printf("\nload I8 palette to GE");

    for (u16Count=0; u16Count<GOP_PALETTE_ENTRY_NUM; u16Count++)
    {
        _gePalette[u16Count].RGB.u8A =  _gopI8PaletteEntry[u16Count].RGB.u8A;
        _gePalette[u16Count].RGB.u8R =  _gopI8PaletteEntry[u16Count].RGB.u8R;
        _gePalette[u16Count].RGB.u8G =  _gopI8PaletteEntry[u16Count].RGB.u8G;
        _gePalette[u16Count].RGB.u8B =  _gopI8PaletteEntry[u16Count].RGB.u8B;
    }
    //Set GE palette table when bitblt from I8 to ARGB
     MApi_GFX_SetPaletteOpt(_gePalette, 0, GOP_PALETTE_ENTRY_NUM - 1 );
}

static void _SetupGfxDrawRect(GFX_DrawRect *pGfxDrawRect, MS_U16 u16SrcX, MS_U16 u16SrcY, MS_U16 u16SrcWidth,
                                MS_U16 u16SrcHeight, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight)
{
    pGfxDrawRect->srcblk.x = u16SrcX;
    pGfxDrawRect->srcblk.y = u16SrcY;
    pGfxDrawRect->srcblk.width = u16SrcWidth;
    pGfxDrawRect->srcblk.height = u16SrcHeight;

    pGfxDrawRect->dstblk.x = u16DstX;
    pGfxDrawRect->dstblk.y = u16DstY;
    pGfxDrawRect->dstblk.width = u16DstWidth;
    pGfxDrawRect->dstblk.height = u16DstHeight;
}

static void _SetupGfxRectFillInfo(GFX_RectFillInfo *pGfxRectFillInfo, MS_U16 u16DstX, MS_U16 u16DstY, MS_U16 u16DstWidth, MS_U16 u16DstHeight,
                                    GFX_Buffer_Format egfxBufFmt, GFX_RgbColor stColorS, GFX_RgbColor stColorE, MS_U32 u32Flag)
{
    pGfxRectFillInfo->dstBlock.x = u16DstX;
    pGfxRectFillInfo->dstBlock.y = u16DstY;
    pGfxRectFillInfo->dstBlock.width= u16DstWidth;
    pGfxRectFillInfo->dstBlock.height= u16DstHeight;

    pGfxRectFillInfo->fmt = egfxBufFmt;

    pGfxRectFillInfo->colorRange.color_s.a = stColorS.a;
    pGfxRectFillInfo->colorRange.color_s.r = stColorS.r;
    pGfxRectFillInfo->colorRange.color_s.g = stColorS.g;
    pGfxRectFillInfo->colorRange.color_s.b = stColorS.b;
    //printf("## s a=%x r=%x g=%x b=%x ##\n",pGfxRectFillInfo->colorRange.color_s.a,pGfxRectFillInfo->colorRange.color_s.r,pGfxRectFillInfo->colorRange.color_s.g,pGfxRectFillInfo->colorRange.color_s.b);
    pGfxRectFillInfo->colorRange.color_e.a = stColorE.a;
    pGfxRectFillInfo->colorRange.color_e.r = stColorE.r;
    pGfxRectFillInfo->colorRange.color_e.g = stColorE.g;
    pGfxRectFillInfo->colorRange.color_e.b = stColorE.g;
    //printf("## e a=%x r=%x g=%x b=%x ##\n",pGfxRectFillInfo->colorRange.color_e.a,pGfxRectFillInfo->colorRange.color_e.r,pGfxRectFillInfo->colorRange.color_e.g,pGfxRectFillInfo->colorRange.color_e.b);

    pGfxRectFillInfo->flag = u32Flag;
}

static void _SetupGfxDrawLineInfo(GFX_DrawLineInfo *pGfxDrawLineInfo, MS_U32 *u32x1, MS_U32 *u32y1, MS_U32 *u32x2, MS_U32 *u32y2,
                                            GFX_Buffer_Format egfxBufFmt, GFX_RgbColor stColorS, GFX_RgbColor stColorE, MS_U32 u32Width, MS_U32 u32Flag)
{
    pGfxDrawLineInfo->x1 = *u32x1;
    pGfxDrawLineInfo->y1 = *u32y1;
    pGfxDrawLineInfo->x2 = *u32x2;
    pGfxDrawLineInfo->y2 = *u32y2;

    pGfxDrawLineInfo->fmt = egfxBufFmt;

    pGfxDrawLineInfo->colorRange.color_s.a = stColorS.a;
    pGfxDrawLineInfo->colorRange.color_s.r = stColorS.r;
    pGfxDrawLineInfo->colorRange.color_s.g = stColorS.g;
    pGfxDrawLineInfo->colorRange.color_s.b = stColorS.b;

    pGfxDrawLineInfo->colorRange.color_e.a = stColorE.a;
    pGfxDrawLineInfo->colorRange.color_e.r = stColorE.r;
    pGfxDrawLineInfo->colorRange.color_e.g = stColorE.g;
    pGfxDrawLineInfo->colorRange.color_e.b = stColorE.g;

    pGfxDrawLineInfo->width = u32Width;
    pGfxDrawLineInfo->flag = u32Flag;
}

static void _SetupGfxTextOutInfo(GFX_TextOutInfo *pstGfxTextOutInfo, MS_U16 u16X, MS_U16 u16Y, MS_U16 u16Width, MS_U16 u16Height,
                                     MS_U16 u16dis, GFX_RgbColor stColor, MS_U32 u32Flag)
{
    pstGfxTextOutInfo->flag = u32Flag;

    //test font bin no glyphinfo, should set NULL
    pstGfxTextOutInfo->pGlyphDispInfo = NULL;

    //if don't set "GEFONT_FLAG_COMPACT" flag and "GEFONT_FLAG_GAP" flag, don't care this setting
    pstGfxTextOutInfo->dis = u16dis;
    pstGfxTextOutInfo->color.a = stColor.a;
    pstGfxTextOutInfo->color.r = stColor.r;
    pstGfxTextOutInfo->color.g = stColor.g;
    pstGfxTextOutInfo->color.b = stColor.b;
    pstGfxTextOutInfo->dstblk.x = u16X;
    pstGfxTextOutInfo->dstblk.y = u16Y;

    //if don't set "GEFONT_FLAG_SCALE", don't care this setting.
    pstGfxTextOutInfo->dstblk.width = u16Width;
    pstGfxTextOutInfo->dstblk.height = u16Height;

}

static void _ColorMapping(MS_U8 u8index, GFX_RgbColor *stcolor)
{
    switch(u8index)
    {
        case 0:
            (*stcolor).r = 255;
            (*stcolor).g = 0;
            (*stcolor).b = 0;
            (*stcolor).a = 255;
            break;
        case 1:
            (*stcolor).r = 0;
            (*stcolor).g = 255;
            (*stcolor).b = 0;
            (*stcolor).a = 255;
            break;
        case 2:
            (*stcolor).r = 0;
            (*stcolor).g = 0;
            (*stcolor).b = 255;
            (*stcolor).a = 255;
            break;
        case 3:
            (*stcolor).r = 255;
            (*stcolor).g = 255;
            (*stcolor).b = 0;
            (*stcolor).a = 128;
            break;
        case 4:
            (*stcolor).r = 0;
            (*stcolor).g = 255;
            (*stcolor).b = 255;
            (*stcolor).a = 128;
            break;
        case 5:
            (*stcolor).r = 255;
            (*stcolor).g = 0;
            (*stcolor).b = 255;
            (*stcolor).a = 128;
            break;
        case 6:
            (*stcolor).r = 255;
            (*stcolor).g = 255;
            (*stcolor).b = 255;
            (*stcolor).a = 255;
            break;
        case 7:
            (*stcolor).r = 0;
            (*stcolor).g = 0;
            (*stcolor).b = 0;
            (*stcolor).a = 255;
            break;
        case 8:
            (*stcolor).r = 255;
            (*stcolor).g = 255;
            (*stcolor).b = 0;
            (*stcolor).a = 255;
            break;
        case 9:
            (*stcolor).r = 0;
            (*stcolor).g = 255;
            (*stcolor).b = 255;
            (*stcolor).a = 255;
            break;
        case 10:
            (*stcolor).r = 255;
            (*stcolor).g = 0;
            (*stcolor).b = 255;
            (*stcolor).a = 255;
            break;
    }
}

static void _ROPMapping(MS_U8 u8index, GFX_DrawRect *pstGfxDrawRect)
{
    switch(u8index)
    {
        case 0:
            printf("\n Set GFX raster operation: ------>ROP2_OP_ZERO! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_ZERO);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 1:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NOT_PS_OR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NOT_PS_OR_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 2:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NS_AND_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NS_AND_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 3:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NS! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NS);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 4:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_AND_ND! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_AND_ND);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 5:
            printf("\n Set GFX raster operation: ------>ROP2_OP_ND! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_ND);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 6:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_XOR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_XOR_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 7:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NOT_PS_AND_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NOT_PS_AND_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 8:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_AND_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_AND_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 9:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NOT_PS_XOR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NOT_PS_XOR_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 10:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 11:
            printf("\n Set GFX raster operation: ------>ROP2_OP_NS_OR_PD! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_NS_OR_PD);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 12:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 13:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PS_OR_ND! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PS_OR_ND);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 14:
            printf("\n Set GFX raster operation: ------>ROP2_OP_PD_OR_PS! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_PD_OR_PS);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
        case 15:
            printf("\n Set GFX raster operation: ------>ROP2_OP_ONE! \n");
            MApi_GFX_SetROP2(TRUE, ROP2_OP_ONE);
            _SetupGfxDrawRect(pstGfxDrawRect, 90, 144, 180, 288, 450, 144, 180, 288);
            MApi_GFX_BitBlt(pstGfxDrawRect, GFXDRAW_FLAG_DEFAULT);
            break;
    }

    MApi_GFX_SetROP2(FALSE, ROP2_OP_PS_AND_PD);
}

static void _SetupEnvByDstFmt(MS_U8 u8gwinFmt, MS_U32 *pu32pbmpAddr, MS_U16 *pu16bmpWidth, MS_U16 *pu16bmpHeight)
{
    GOP_PaletteEntry _gopI8Palette[GOP_PALETTE_ENTRY_NUM];
    MS_U16 u16Count; //Be careful to change this type, since GOP_PALETTE_ENTRY_NUM is 256, don't use MS_U8.

    /// - Select the sample image by color type for the GWin that we create.
    switch( u8gwinFmt )
    {
        case E_MS_FMT_ARGB8888:
            *pu32pbmpAddr = u32BmpARGB8888Addr;
            *pu16bmpWidth = BMPRGB8888W;
            *pu16bmpHeight = BMPRGB8888H;
            break;
        case E_MS_FMT_RGB565:
            *pu32pbmpAddr = u32BmpRGB565Addr;
            *pu16bmpWidth = BMPRGB565W;
            *pu16bmpHeight = BMPRGB565H;
            break;
        case E_MS_FMT_ARGB4444:
            *pu32pbmpAddr = u32BmpARGB4444Addr;
            *pu16bmpWidth = BMPRGB4444W;
            *pu16bmpHeight = BMPRGB4444H;
            break;
        case E_MS_FMT_I8:
            *pu32pbmpAddr = u32BmpI8Addr;
            *pu16bmpWidth = BMPRGBI8W;
            *pu16bmpHeight = BMPRGBI8H;
            for (u16Count=0; u16Count<GOP_PALETTE_ENTRY_NUM; u16Count++)
            {
                _gopI8Palette[u16Count].RGB.u8A =  _gopI8PaletteEntry[u16Count].RGB.u8A;
                _gopI8Palette[u16Count].RGB.u8R =  _gopI8PaletteEntry[u16Count].RGB.u8R;
                _gopI8Palette[u16Count].RGB.u8G =  _gopI8PaletteEntry[u16Count].RGB.u8G;
                _gopI8Palette[u16Count].RGB.u8B =  _gopI8PaletteEntry[u16Count].RGB.u8B;
                //printf("A:%d, R: %d, G: %d, B: %d\t", _gopI8Palette[i].RGB.u8A, _gopI8Palette[i].RGB.u8R, _gopI8Palette[i].RGB.u8G, _gopI8Palette[i].RGB.u8B);
            }
            //Set GOP palette table when GOP output frame buffer is I8
            MApi_GOP_GWIN_SwitchGOP(u32GOP0);
            MApi_GOP_GWIN_SetPaletteOpt(_gopI8Palette, 0, GOP_PALETTE_ENTRY_NUM - 1, E_GOP_PAL_ARGB8888);
            break;
        default:
            printf("GOP_CreateWin() --> Wrong parameter\n");
            break;
    }
}

static MS_U32 _CalcPitch(MS_U8 u8fbFmt, MS_U16 u16width)
{
    MS_U16 u16pitch=0;

    switch ( u8fbFmt )
    {
        case E_MS_FMT_I8 :
            u16pitch = u16width;
            break;
        case E_MS_FMT_RGB565 :
        case E_MS_FMT_ARGB4444 :
        case E_MS_FMT_YUV422:
            u16pitch = u16width << 1;
            break;
        case E_MS_FMT_ARGB8888 :
            u16pitch = u16width << 2;
            break;
        default :
            u16pitch = 0;
            break;
    }
    return u16pitch;
}

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
static SCALER_DIP_SOURCE_TYPE _DIPDWIN_Mapping(EN_GOP_DWIN_SRC_SEL eDDISource)
{
    SCALER_DIP_SOURCE_TYPE eDIPSource = SCALER_DIP_SOURCE_TYPE_OP_CAPTURE;

    switch(eDDISource)
    {
        case DWIN_SRC_OP:
            eDIPSource = SCALER_DIP_SOURCE_TYPE_OP_CAPTURE;
            break;
        case DWIN_SRC_IP:
            eDIPSource = SCALER_DIP_SOURCE_TYPE_MAIN;
            break;
#if (DEMO_XC_DUALXC_TEST == 1)
        case DWIN_SRC_OP1:
            eDIPSource = SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE;
            break;
#endif
        case DWIN_SRC_MVOP:
        case DWIN_SRC_SUBMVOP:
        case DWIN_SRC_GOPScaling:
        default:
            eDIPSource = SCALER_DIP_SOURCE_TYPE_OP_CAPTURE;
            break;
        }

    return eDIPSource;
}
#endif
//------------------------------------------------------------------------------
/// @brief The sample code to initial GOP
/// @return None
/// @note
/// Command: \b HelpOSD \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_Help(void)
{
    printf ("*************************GOP function List*************************\n");
    printf ("1.OSD_Init [HD_GOP] [SD_GOP] [HD_DST] [SD_DST]--Initial GOP. Need to init XC and VE before init OSD.\n");
    printf ("-------HD_DST-- 0 OP0, 1 IP0, 2 VOP, 3 VOP_SUB,  4 IP_SUB, 5 GOPScaling\n");
    printf ("-------SD_DST-- 0 OP1, 1 IP1, 2 VE,  3 Mixer2VE, 4 OP2VE\n");
    printf ("-------Need to init XC and VE before init GOP.\n");
    printf ("2.OSD_SetConfig --Set configurations including stretch win size\n");
    printf ("3.OSD_CreateWin  [ColorType] [GWinWidth0] [GWinHeight0] [GWinWidth1] [GWinHeight1]\n");
    printf ("-----ColorType : Gwin color format\n");
    printf ("-------HD: ARGB8888  SD: ARGB8888      =0\n");
    printf ("-------HD: RGB565    SD: RGB565        =1\n");
    printf ("-------HD: ARGB4444  SD: ARGB4444      =2\n");
    printf ("-------HD: I8        SD: RGB565        =3\n");
    printf ("-------HD: YUV422    SD: EYUV422       =4\n");
    printf ("-----GWinWidth0 : The width of HD GWin to create. it is small than stretch win width\n");
    printf ("-----GWinHeight0 : The height of HD GWin to create. it is small than stretch win height\n");
    printf ("-----GWinWidth1 : The width of SD GWin to create. it is small than stretch win width\n");
    printf ("-----GWinHeight1 : The height of SD GWin to create. it is small than stretch win height\n");
    printf ("4.OSD_SetGWinFmt [ColorType]\n");
    printf ("-----ColorType : Gwin color format\n");
    printf ("-------4  I8,      8  RGB565, 9 ARGB1555, 10 ARGB4444, 14 YUV422, 15 ARGB8888\n");
    printf ("-------16 RGBA5551,17 RGBA4444, 18 BGRA5551, 19 ABGR1555, 20 ABGR4444\n");
    printf ("-------21 BGRA4444, 22 BGR565, 29 RGBA8888, 30 BGRA8888, 31 ABGR8888\n");
    printf ("5.OSD_DrawBmpXY [Target(0: HD, 1: SD)] [Index] [X]  [Y] [width] [height]\n");
    printf ("-----Index : The bitmap index\n");
    printf ("-------0 ARGB8888, 1 RGB565, 2 ARGB4444, 3 I8\n");
    printf ("6.OSD_DWinCapture [CaptureOneFrame] [GWinDsipCount] [Source]\n");
    printf ("-----CaptureOneFrame : 0 disable, 1 enable\n");
    printf ("-----GWinDsipCount : The count that using GWin to displpay the captured frame\n");
    printf ("-----Source : 0 DWIN_SRC_OP, 1 DWIN_SRC_OP1, 2 DWIN_SRC_GOPScaling, 3 DWIN_SRC_MVOP, 4 DWIN_SRC_SUBMVOP\n");
    printf ("7.OSD_EnableWin [Enable]\n");
    printf ("-----Enable: 0 disable, 1 enable\n");
    printf ("8.OSD_SetGlobalAlpha [Auto] [AlphaValue]\n");
    printf ("-----Auto: 0 disable, 1 enable\n");
    printf ("-----AlphaValue: range {0..255}, effect when Auto=0\n");
    printf ("9.OSD_EnablePerPixelAlpha [Auto] [AlphaValue]\n");
    printf ("-----Auto: 0 disable, 1 enable\n");
    printf ("-----AlphaValue: range {0..255}, effect when Auto=0\n");
    printf ("10.OSD_SetFade [FadeType] [Enable] [Rate]\n");
    printf ("-----FadeType: 0 E_WEAKER, 1 E_STRONGER\n");
    printf ("-----Enable: 0 disable, 1 enable\n");
    printf ("-----Rate: The fade rate\n");
    printf ("11.OSD_HVScale [EnableScale] [Hscale_rate] [Vscale_rate]\n");
    printf ("12.OSD_GOPSC_HVScalingProcess [Enable] [SrcW] [SrcH] [DestW] [DestH] [FrameConut]\n");
    printf ("13.OSD_3DOSD [Mode]\n");
    printf ("-----Mode : 3D Mode\n");
    printf ("-------Swithc by Frame      =0\n");
    printf ("-------Side by Side         =1\n");
    printf ("-------Top and Bottom       =2\n");
    printf ("-------Line Alternative     =3\n");
    printf ("-------Disable 3D           =4\n");
    printf ("-------Frame Packing        =5\n");
    printf ("14.OSD_FreeWin \n");
    printf ("*************************End of GOP function List*************************\n");
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initial GOP and GE. Need to init XC and VE before init OSD.
/// @return None
/// @note
/// Command: \b OSD_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_Init(MS_U32 *pu32HD_GOP, MS_U32 *pu32SD_GOP, MS_U32 *pu32HD_DST, MS_U32 *pu32SD_DST)
{
    printf("Demo_OSD_Init. Need to init XC and VE before init OSD.\n");

    MS_U16 u16BmpItem = 0, u16FontItem = 0;
    GOP_InitInfo stGopInit;
    GFX_Config stGFXcfg;
    EN_DDI_OSD_HD_DST_TYPE eDDIGOPHDDst = (EN_DDI_OSD_HD_DST_TYPE)(*pu32HD_DST);
    EN_DDI_OSD_SD_DST_TYPE eDDIGOPSDDst = (EN_DDI_OSD_SD_DST_TYPE)(*pu32SD_DST);
    memset(&stGopInit, 0, sizeof(GOP_InitInfo));
    memset(&stGFXcfg, 0, sizeof(GFX_Config));

    if( bGOPInit == FALSE)
    {
        if( FALSE == MDrv_SEM_Init() )
        {
             printf("[%s][%d] MDrv_SEM_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        // -Initial GE
        stGFXcfg.bIsCompt = TRUE;
        stGFXcfg.bIsHK = TRUE;
        MApi_GFX_Init(&stGFXcfg);
        printf("driver GE init ok\n");

#if (DEMO_ZUI_TEST == 1)
        //Set getting UI font and bitmap callback function
        if(GFX_SUCCESS != MApi_GFX_RegisterGetFontCB(msAPI_OCP_GetFontInfo))
        {
             printf("[%s][%d] MApi_GFX_RegisterGetFontCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        if(GFX_SUCCESS != MApi_GFX_RegisterGetBMPCB(msAPI_OCP_GetBitmapInfo))
        {
             printf("[%s][%d] MApi_GFX_RegisterGetBMPCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
#else
        //Set getting font and bitmap callback function
        if( GFX_SUCCESS != MApi_GFX_RegisterGetFontCB(Demo_OSD_RESOURCE_GetFontInfoGFX) )
        {
             printf("[%s][%d] MApi_GFX_RegisterGetFontCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GFX_SUCCESS != MApi_GFX_RegisterGetBMPCB(Demo_OSD_RESOURCE_GetBitmapInfoGFX) )
        {
             printf("[%s][%d] MApi_GFX_RegisterGetBMPCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
#endif
        // -Initial GOP and Scaler
        //Initial Scaler
        //_GOP_ScalerInit();

        //Initial GOP
        if( GOP_API_SUCCESS != MApi_GOP_RegisterFBFmtCB(_OSD_SetFBFmt) )
        {
             printf("[%s][%d] MApi_GOP_RegisterFBFmtCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCIsInterlaceCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCGetCapHStartCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCReduceBWForOSDCB(_XC_Sys_PQ_ReduceBW_ForOSD) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCReduceBWForOSDCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        stGopInit.u16PanelWidth = IPANEL(&gDevId ,Width);
        stGopInit.u16PanelHeight = IPANEL(&gDevId ,Height);
        stGopInit.u16PanelHStr = IPANEL(&gDevId ,HStart);

        printf("[%s][%d]====u16PanelWidth=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelWidth);
        printf("[%s][%d]====u16PanelHeight=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelHeight);
        printf("[%s][%d]====u16PanelHStr=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelHStr);

        stGopInit.u32GOPRBAdr = MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE);
        stGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
        stGopInit.u32GOPRegdmaAdr = MEM_ADR_BY_MIU(GOP_REG_DMA_BASE_ADR, GOP_REG_DMA_BASE_MEMORY_TYPE);
        stGopInit.u32GOPRegdmaLen = GOP_REG_DMA_BASE_LEN;
        stGopInit.bEnableVsyncIntFlip = FALSE;

        if( GOP_API_SUCCESS != MApi_GOP_Init(&stGopInit) )
        {
             printf("[%s][%d] MApi_GOP_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        printf("GOP_GWIN_RB_ADR : %"DTC_MS_PHY_x", GOP_REG_DMA_BASE_ADR: %"DTC_MS_PHY_x"\n", stGopInit.u32GOPRBAdr, stGopInit.u32GOPRegdmaAdr);
        printf("driver GOP init ok\n");
    }

    if((*pu32HD_GOP < MApi_GOP_GWIN_GetMaxGOPNum()) && (*pu32SD_GOP < MApi_GOP_GWIN_GetMaxGOPNum()))
    {
        u32GOP0  = *pu32HD_GOP;
        u32GOP1 = *pu32SD_GOP;
    }
    else
    {
       printf("Error GOP Number HD:%"DTC_MS_U32_d" SD:%"DTC_MS_U32_d"\n",*pu32HD_GOP,*pu32SD_GOP);
       return FALSE;
    }

    //Use two GOPs, here we use GOP2 to display for HD and use GOP1 to display for SD
    u8GOPCount = 2;
    u8DrawGOPCount = 2;

    //--------------To HD---------------//
    switch(eDDIGOPHDDst)
    {
        case E_DDI_OSD_HD_DST_OP0:
            gWinInfo[0].eGopDest = E_GOP_DST_OP0;
            break;
        case E_DDI_OSD_HD_DST_IP0:
            gWinInfo[0].eGopDest = E_GOP_DST_IP0;
            break;
        case E_DDI_OSD_HD_DST_VOP:
            gWinInfo[0].eGopDest = E_GOP_DST_VOP;
            break;
        case E_DDI_OSD_HD_DST_VOP_SUB:
            gWinInfo[0].eGopDest = E_GOP_DST_VOP_SUB;
            break;
        case E_DDI_OSD_HD_DST_IP_SUB:
            gWinInfo[0].eGopDest = E_GOP_DST_IP_SUB;
            break;
#if (DEMO_GOP_GOPSC_TEST == 1)
        case E_DDI_OSD_HD_DST_GOPScaling:
            gWinInfo[0].eGopDest = E_GOP_DST_GOPScaling;
            break;
#endif
        default:
            printf("[%s][%d]Not supported HD path:%"DTC_MS_U32_d"\n",__FUNCTION__,__LINE__,*pu32HD_DST);
            return FALSE;
    }

    /*If use 'GOP2' and the destination of GOP is 'OP',
      use this to change GOP output layer to the last one.
      The input is the GOP number
    */
    //MApi_GOP_GWIN_OutputLayerSwitch(2);

    //---------------To SD---------------//
    switch(eDDIGOPSDDst)
    {
        case E_DDI_OSD_SD_DST_OP1:
            gWinInfo[1].eGopDest = E_GOP_DST_OP1;
            break;
        case E_DDI_OSD_SD_DST_IP1:
            gWinInfo[1].eGopDest = E_GOP_DST_IP1;
            break;
        case E_DDI_OSD_SD_DST_VE:
            gWinInfo[1].eGopDest = E_GOP_DST_VE;
            break;
        case E_DDI_OSD_SD_DST_MIXER2VE:
            gWinInfo[1].eGopDest = E_GOP_DST_MIXER2VE;
            break;
        case E_DDI_OSD_SD_DST_OP2VE://OP2VE
            u8GOPCount = 1;
            u8DrawGOPCount = 1;
            break;
        default:
            printf("[%s][%d]Not supported SD path:%"DTC_MS_U32_d"\n",__FUNCTION__,__LINE__,*pu32SD_DST);
            return FALSE;
    }

//For K3, GE line, rectangle..., use 2 GOP to draw. HD dst is GOPScaling, SD dst is VE.
#if (DEMO_GOP_GOPSC_TEST == 1)
    if((gWinInfo[0].eGopDest == E_GOP_DST_OP0) && (eDDIGOPSDDst == E_DDI_OSD_SD_DST_OP2VE))
    {
        u8GOPCount = 2;
        u8DrawGOPCount = 1;
        gWinInfo[0].eGopDest = E_GOP_DST_GOPScaling;
        gWinInfo[1].eGopDest = E_GOP_DST_VE;
    }
#endif
    if((u8GOPCount == 2) && (u32GOP0 == u32GOP1))
    {
         printf("[%s][%d] Do not use the same GOP to set HDMI and CVBS.", __FUNCTION__, __LINE__);
         return FALSE;
    }
    MApi_GOP_GWIN_SwitchGOP(u32GOP0);//use GOP 0
#if ENABLE_MIU_1
    if(MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE) > MIU_INTERVAL)
    {
        MApi_GOP_MIUSel(u32GOP0,E_GOP_SEL_MIU1);
    }
    else
    {
        MApi_GOP_MIUSel(u32GOP0,E_GOP_SEL_MIU0);
    }
#endif
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
    MApi_GOP_GWIN_OutputLayerSwitch(u32GOP0);//Set GOP0 show in the top layer (mux2)

    if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetGOPDst(u32GOP0, gWinInfo[0].eGopDest))
    {
         printf("[%s][%d] HD MApi_GOP_GWIN_SetGOPDst failed", __FUNCTION__, __LINE__);
         return FALSE;
    }

    if(u8GOPCount == 2)
    {
        MApi_GOP_GWIN_SwitchGOP(u32GOP1);//use GOP 1
#if ENABLE_MIU_1
    if(MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE) > MIU_INTERVAL)
    {
        MApi_GOP_MIUSel(u32GOP1,E_GOP_SEL_MIU1);
    }
    else
    {
        MApi_GOP_MIUSel(u32GOP1,E_GOP_SEL_MIU0);
    }
#endif
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
        if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetGOPDst(u32GOP1, gWinInfo[1].eGopDest))
        {
             printf("[%s][%d] SD MApi_GOP_GWIN_SetGOPDst failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    // for CVBS output, use VE
    // To choose video to VE mux
    if(eDDIGOPSDDst == E_DDI_OSD_SD_DST_OP2VE)
    {
        // Set OP can through VE output to CVBS
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
        // Disable blackScreen. If video enable black screen, OP2VE mode can't show graph on CVBS.
        MDrv_VE_SetBlackScreen(FALSE);
    }
    else
    {
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND1);
    }

    if( bGOPInit == FALSE)
    {
#if (DEMO_GOP_3DOSD_TEST == 1)
        u16BmpItem |= (ARGB8888BMP |RGB565BMP |ARGB4444BMP| I8BMP| L3DBMP| R3DBMP);
#else
        //add test bitmap here, in this demo code only these four bitmaps
        u16BmpItem |= (ARGB8888BMP |RGB565BMP |ARGB4444BMP| I8BMP);
#endif
        //add test font here, in this demo code only these three fonts
        u16FontItem |= (FontI1|FontI2|FontI4);

        _GOP_InitGlobalVar();
        _GOP_SetBmpFontAdr();
        _LoadBitmap(u16BmpItem);
        _LoadFont(u16FontItem);
    }

    bGOPInit = TRUE;
    return TRUE;
}

#if (DEMO_GOP_ZORDER_TEST == 1)
static MS_BOOL _ZOrder_InitGop(const MS_U32 u32GopId);
static MS_BOOL _ZOrder_GetColorIndexByLayer(
                                    const EN_DISPLAY_LAYER keDisplayLayer,
                                    MS_U32 * const pu32ColorIndex);
static MS_BOOL _ZOrder_GetAreaByLayer(
                                    const MS_U32 u32Layer,
                                    MS_U32 * const pu32X,
                                    MS_U32 * const pu32Y,
                                    MS_U32 * const pu32W,
                                    MS_U32 * const pu32H);
static MS_BOOL _ZOrder_OsdFillBlock(const MS_U32 *pu32Target, MS_U32 *pu32index, MS_U32 *pu32RectX, MS_U32 *pu32RectY, MS_U32 *pu32RectWidth, MS_U32 *pu32RectHeight);
static MS_BOOL _ZOrder_SetVideoLayer(MS_U32* pu32VideoLayerOrder)
{
    E_VOP_OSD_LAYER_SEL vopOsdLayer = (E_VOP_OSD_LAYER_SEL)(*pu32VideoLayerOrder);
    MSAPI_XC_DEVICE_ID kstXcSetWindowDeviceId = {0, E_MSAPI_XC_DEVICE_DIP_0};
    MSAPI_XC_WINDOW_TYPE stXcCropWindow;
    MSAPI_XC_WINDOW_TYPE stXcDestWindow;
    MS_U32 u32VideoX = 0;
    MS_U32 u32VideoY = 0;
    MS_U32 u32VideoW = 0;
    MS_U32 u32VideoH = 0;

    // set layer
    switch((*pu32VideoLayerOrder))
    {
    case 0:
        vopOsdLayer = E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2_MUX3_MUX4;
        break;
    case 1:
        vopOsdLayer = E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2_MUX3_MUX4;
        break;
    case 2:
        vopOsdLayer = E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO_MUX3_MUX4;
        break;
    case 3:
        vopOsdLayer = E_VOP_LAYER_FRAME_MUX1_MUX2_MUX3_VIDEO_MUX4;
        break;
    case 4:
        vopOsdLayer = E_VOP_LAYER_FRAME_MUX1_MUX2_MUX3_MUX4_VIDEO;
        break;
    case 5:
        vopOsdLayer = E_VOP_LAYER_FRAME_MUX1_MUX2_MUX3_MUX4_MUX5_VIDEO;
        break;
    default:
        printf("Error! not support video layer = %u \n", (*pu32VideoLayerOrder));
        break;
    }

    MApi_XC_SetOSDLayer(vopOsdLayer, MAIN_WINDOW);

    // adjust window
    _ZOrder_GetAreaByLayer(
                            (*pu32VideoLayerOrder),
                            &u32VideoX,
                            &u32VideoY,
                            &u32VideoW,
                            &u32VideoH);

    stXcCropWindow.x      = 0;
    stXcCropWindow.y      = 0;
    stXcCropWindow.width  = 720;    // depends on video size
    stXcCropWindow.height = 576;    // depends on video size

    stXcDestWindow.x      = u32VideoX;
    stXcDestWindow.y      = u32VideoY;
    stXcDestWindow.width  = u32VideoW;
    stXcDestWindow.height = u32VideoH;

    msAPI_XC_SetWin_EX(
                        &kstXcSetWindowDeviceId,
                        NULL,
                        (&stXcCropWindow),
                        &(stXcDestWindow),
                        E_MSAPI_XC_MAIN_WINDOW);

    return TRUE;
}

static MS_BOOL _ZOrder_SetGopLayer(
                                    MS_U8* pu8Layer0Gop,
                                    MS_U8* pu8Layer1Gop,
                                    MS_U8* pu8Layer2Gop,
                                    MS_U8* pu8Layer3Gop,
                                    MS_U8* pu8Layer4Gop)
{
    MS_U32 u32GopLayerIndex  = 0;
    MS_U32 u32FillRecX       = 0;
    MS_U32 u32FillRecY       = 0;
    MS_U32 u32FillRecW       = 0;
    MS_U32 u32FillRecH       = 0;
    MS_U32 u32FillColorIndex = 0;
    GOP_LayerConfig stGopLayerConfig;
    memset(&stGopLayerConfig, 0, sizeof(GOP_LayerConfig));

    MApi_GOP_GWIN_GetLayer(&stGopLayerConfig, sizeof(GOP_LayerConfig));

    if((stGopLayerConfig.u32LayerCounts > 0)
    && (NULL != pu8Layer0Gop))
    {
        stGopLayerConfig.stGopLayer[0].u32GopIndex = (*pu8Layer0Gop);
    }

    if((stGopLayerConfig.u32LayerCounts > 1)
    && (NULL != pu8Layer1Gop))
    {
        stGopLayerConfig.stGopLayer[1].u32GopIndex = (*pu8Layer1Gop);
    }

    if((stGopLayerConfig.u32LayerCounts > 2)
    && (NULL != pu8Layer2Gop))
    {
        stGopLayerConfig.stGopLayer[2].u32GopIndex = (*pu8Layer2Gop);
    }

    if((stGopLayerConfig.u32LayerCounts > 3)
    && (NULL != pu8Layer3Gop))
    {
        stGopLayerConfig.stGopLayer[3].u32GopIndex = (*pu8Layer3Gop);
    }

    if((stGopLayerConfig.u32LayerCounts > 4)
    && (NULL != pu8Layer4Gop))
    {
        stGopLayerConfig.stGopLayer[4].u32GopIndex = (*pu8Layer4Gop);
    }

    printf("GOP set Layer: \n");
    for(u32GopLayerIndex = 0; u32GopLayerIndex < stGopLayerConfig.u32LayerCounts; ++u32GopLayerIndex)
    {
        printf(" GopLayer[%d].GopIndex = %u \n",
            u32GopLayerIndex,
            stGopLayerConfig.stGopLayer[u32GopLayerIndex].u32GopIndex);
        printf(" GopLayer[%d].u32LayerIndex = %u \n",
            u32GopLayerIndex,
            stGopLayerConfig.stGopLayer[u32GopLayerIndex].u32LayerIndex);
    }

    MApi_GOP_GWIN_SetLayer(&stGopLayerConfig, sizeof(GOP_LayerConfig));

    // adjust GOPs window
    for(u32GopLayerIndex = 0; u32GopLayerIndex < stGopLayerConfig.u32LayerCounts; ++u32GopLayerIndex)
    {
        MS_U32 u32GopDispLayer = 0;
        MS_U32 u32GopId = stGopLayerConfig.stGopLayer[u32GopLayerIndex].u32GopIndex;
        GOP_GwinFBAttr stDstFBInfo;

        // DMS driver GOPId
        if(u32GopId == *pu8Layer4Gop)
        {
            continue;
        }

        if(u32GopLayerIndex >= s_u32VideoLayer)
        {
            u32GopDispLayer = u32GopLayerIndex + 1;
        }
        else
        {
            u32GopDispLayer = u32GopLayerIndex;
        }

        printf("u32GopLayerIndex = %u \n", u32GopLayerIndex);
        printf("s_u32VideoLayer = %u \n", s_u32VideoLayer);
        printf("u32GopDispLayer = %u \n", u32GopDispLayer);

        _ZOrder_GetAreaByLayer(
                                u32GopDispLayer,
                                &u32FillRecX,
                                &u32FillRecY,
                                &u32FillRecW,
                                &u32FillRecH);
        _ZOrder_GetColorIndexByLayer(u32GopLayerIndex, &u32FillColorIndex);
        printf("[%s][%d] GOP[%u] fill color = %u, (X, Y, W, H) = (%u, %u, %u, %u) \n",
                __FUNCTION__,
                __LINE__,
                u32GopId,
                u32FillColorIndex,
                u32FillRecX,
                u32FillRecY,
                u32FillRecW,
                u32FillRecH);

        MApi_GOP_GWIN_GetFBInfo(cusGwinInfo[u32GopId].u8FBId, &stDstFBInfo);
        MApi_GFX_ClearFrameBuffer(stDstFBInfo.addr, stDstFBInfo.size, 0x00);
        _ZOrder_OsdFillBlock(
                        &u32GopId,
                        &u32FillColorIndex,
                        &u32FillRecX,
                        &u32FillRecY,
                        &u32FillRecW,
                        &u32FillRecH);
        MApi_GFX_FlushQueue();
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to test Z-Order
/// @return TRUE: success FALSE: failed
/// Command: \b ZOrder_Init GOPId \n
//------------------------------------------------------------------------------
MS_BOOL Demo_ZOrder_Init(void)
{
    MS_U32 u32GOPNum = 0;
    MS_U32 u32GopIndex = 0;
    MS_U32 u32MaxGopNum = 0;
    MS_U8 u8GopId[4] = {0};
    MS_U8 u8IdIndex = 0;
    GOP_InitInfo stGopInit;
    GFX_Config stGFXcfg;
#if defined(MSOS_TYPE_LINUX)
    dictionary *dms_cfg_ini = NULL;
#endif
    memset(&stGopInit, 0, sizeof(GOP_InitInfo));
    memset(&stGFXcfg, 0, sizeof(GFX_Config));

    printf("Demo_ZOrder_Init. Need to init XC and VE before init OSD.\n");

    u32MaxGopNum = MApi_GOP_GWIN_GetMaxGOPNum();
#if defined(MSOS_TYPE_LINUX)
    dms_cfg_ini = iniparser_load((const char *)cfgFileName);

    if (dms_cfg_ini == NULL)
    {
        u32GOPNum = 2;
        printf("Use default DMS GOP ID 2\n");
    }
    else
    {
        u32GOPNum = iniparser_getint(dms_cfg_ini, "DMS_GOP:dms_gop", NULL);
        printf("DMS GOP ID %d\n", u32GOPNum);
    }
    iniparser_freedict(dms_cfg_ini);
#else
    u32GOPNum = 2;
#endif

    if (u32GOPNum >= u32MaxGopNum)
    {
        printf("WANRING: GOP Max Number is %d, GOP Number %d exceeded\n",u32MaxGopNum,u32GOPNum);
        return FALSE;
    }

    s_u32GOPNum = u32GOPNum;

    if( bGOPInit == FALSE)
    {
        if( FALSE == MDrv_SEM_Init() )
        {
             printf("[%s][%d] MDrv_SEM_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        // -Initial GE
        stGFXcfg.bIsCompt = TRUE;
        stGFXcfg.bIsHK    = TRUE;
        MApi_GFX_Init(&stGFXcfg);
        printf("driver GE init ok\n");
#if (DEMO_ZUI_TEST == 1)
        //Set getting UI font and bitmap callback function
        if(GFX_SUCCESS != MApi_GFX_RegisterGetFontCB(msAPI_OCP_GetFontInfo))
        {
             printf("[%s][%d] MApi_GFX_RegisterGetFontCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        if(GFX_SUCCESS != MApi_GFX_RegisterGetBMPCB(msAPI_OCP_GetBitmapInfo))
        {
             printf("[%s][%d] MApi_GFX_RegisterGetBMPCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
#else
        //Set getting font and bitmap callback function
        if( GFX_SUCCESS != MApi_GFX_RegisterGetFontCB(Demo_OSD_RESOURCE_GetFontInfoGFX) )
        {
             printf("[%s][%d] MApi_GFX_RegisterGetFontCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GFX_SUCCESS != MApi_GFX_RegisterGetBMPCB(Demo_OSD_RESOURCE_GetBitmapInfoGFX) )
        {
             printf("[%s][%d] MApi_GFX_RegisterGetBMPCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
#endif
        // -Initial GOP and Scaler
        //Initial Scaler
        //_GOP_ScalerInit();

        //Initial GOP
        if( GOP_API_SUCCESS != MApi_GOP_RegisterFBFmtCB(_OSD_SetFBFmt) )
        {
             printf("[%s][%d] MApi_GOP_RegisterFBFmtCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCIsInterlaceCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCGetCapHStartCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCReduceBWForOSDCB(_XC_Sys_PQ_ReduceBW_ForOSD) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCReduceBWForOSDCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        stGopInit.u16PanelWidth = IPANEL(&gDevId ,Width);
        stGopInit.u16PanelHeight = IPANEL(&gDevId ,Height);
        stGopInit.u16PanelHStr = IPANEL(&gDevId ,HStart);

        printf("[%s][%d]====u16PanelWidth=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelWidth);
        printf("[%s][%d]====u16PanelHeight=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelHeight);
        printf("[%s][%d]====u16PanelHStr=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelHStr);

        stGopInit.u32GOPRBAdr         = MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE);
        stGopInit.u32GOPRBLen         = GOP_GWIN_RB_LEN;
        stGopInit.u32GOPRegdmaAdr     = MEM_ADR_BY_MIU(GOP_REG_DMA_BASE_ADR, GOP_REG_DMA_BASE_MEMORY_TYPE);
        stGopInit.u32GOPRegdmaLen     = GOP_REG_DMA_BASE_LEN;
        stGopInit.bEnableVsyncIntFlip = FALSE;

        if( GOP_API_SUCCESS != MApi_GOP_Init(&stGopInit) )
        {
             printf("[%s][%d] MApi_GOP_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        printf("GOP_GWIN_RB_ADR : %"DTC_MS_PHY_x", GOP_REG_DMA_BASE_ADR: %"DTC_MS_PHY_x"\n", stGopInit.u32GOPRBAdr, stGopInit.u32GOPRegdmaAdr);
        printf("driver GOP init ok\n");
    }

    for(u32GopIndex = 0; u32GopIndex < u32MaxGopNum; ++u32GopIndex)
    {
        // DMS driver hard code use GOP2, it will conflict when play video
        if(u32GopIndex == s_u32GOPNum)
        {
            continue;
        }

        if(FALSE == _ZOrder_InitGop(u32GopIndex))
        {
            printf("[%s][%d] init gop%u fail \n", __FUNCTION__, __LINE__, u32GopIndex);
            return FALSE;
        }
        u8GopId[u8IdIndex++] = u32GopIndex;
    }

    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);//FIXME

    // initialize GOP layer (always keep DMS GOP on the top layer)
    Demo_ZOrder_SetZOrder(
                            &s_u32VideoLayer,
                            &u8GopId[0],
                            &u8GopId[1],
                            &u8GopId[2],
                            &u8GopId[3]);

    bGOPInit = TRUE;
    printf("[%s][%d] init ok \n", __FUNCTION__, __LINE__);
    return TRUE;
}

static MS_BOOL _ZOrder_InitGop(const MS_U32 u32GopId)
{
    const EN_GOP_DST_TYPE GopDst = E_GOP_DST_OP0;
    static MS_U8 s_u8DefaultLayer = 0;
    MS_U16 u16StretchWinWidth     = IPANEL(&gDevId, Width);
    MS_U16 u16StretchWinHeight    = IPANEL(&gDevId, Height);
    MS_U32 u32StretchWinX         = 0;
    MS_U32 u32StretchWinY         = 0;
    MS_U16 u16FbWidth             = IPANEL(&gDevId, Width);
    MS_U16 u16FbHeight            = IPANEL(&gDevId, Height);
    MS_U16 u16FbFmt               = E_MS_FMT_ARGB8888;
    MS_U32 u32FillRecX            = 0;
    MS_U32 u32FillRecY            = 0;
    MS_U32 u32FillRecW            = 0;
    MS_U32 u32FillRecH            = 0;
    MS_U32 u32FillColorIndex      = 0;

    printf("s_u8DefaultLayer = %u \n", s_u8DefaultLayer);

    MApi_GOP_GWIN_SwitchGOP(u32GopId);

#if ENABLE_MIU_1
    if(MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE) > MIU_INTERVAL)
    {
        MApi_GOP_MIUSel(u32GopId,E_GOP_SEL_MIU1);
    }
    else
    {
        MApi_GOP_MIUSel(u32GopId,E_GOP_SEL_MIU0);
    }
#endif

    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);
    MApi_GOP_GWIN_OutputLayerSwitch(u32GopId);
    if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetGOPDst(u32GopId, GopDst))
    {
         printf("[%s][%d] Set Gop = %u E_GOP_DST_OP0 failed", __FUNCTION__, __LINE__, u32GopId);
         return FALSE;
    }

    MApi_GOP_GWIN_Set_STRETCHWIN(u32GopId, GopDst, u32StretchWinX, u32StretchWinY, u16StretchWinWidth, u16StretchWinHeight);

    // Every time change resolution, the HStart should be set again
    printf("Panel HStart = %d \n",IPANEL(&gDevId,HStart));
    MApi_GOP_SetGOPHStart(u32GopId,IPANEL(&gDevId,HStart));

    // Create FB
    if( 0 != _GOP_Create_FB(&(cusGwinInfo[u32GopId].u8FBId), u16FbWidth, u16FbHeight, u16FbFmt))
    {
         printf("Create FB fail\n");
         return FALSE;
    }

    // Create GWIN
    if( 0 != _GOP_Create_GWin(u32GopId, &(cusGwinInfo[u32GopId].u8GeWinId), cusGwinInfo[u32GopId].u8FBId, u16FbFmt))
    {
         printf("Create GWin fail, GOPnum = %u, u8GWin = %u, FBId = %u, FBFmt = %u\n", u32GopId,  cusGwinInfo[u32GopId].u8GeWinId, cusGwinInfo[u32GopId].u8FBId, u16FbFmt);
         return FALSE;
    }

    printf("Create GWin ok, GOPnum = %u, u8GWin = %u, FBId = %u, FBFmt = %u\n", u32GopId, cusGwinInfo[u32GopId].u8GeWinId, cusGwinInfo[u32GopId].u8FBId, u16FbFmt);

    // Fill Rectangle to GWIN FB
    _ZOrder_GetAreaByLayer(
                            s_u8DefaultLayer,
                            &u32FillRecX,
                            &u32FillRecY,
                            &u32FillRecW,
                            &u32FillRecH);
    _ZOrder_GetColorIndexByLayer(s_u8DefaultLayer, &u32FillColorIndex);
    printf("[%s][%d] GOP[%u] fill color = %u, (X, Y, W, H) = (%u, %u, %u, %u) \n",
            __FUNCTION__,
            __LINE__,
            u32GopId,
            u32FillColorIndex,
            u32FillRecX,
            u32FillRecY,
            u32FillRecW,
            u32FillRecH);
    _ZOrder_OsdFillBlock(&u32GopId, &u32FillColorIndex, &u32FillRecX, &u32FillRecY, &u32FillRecW, &u32FillRecH);

    ++s_u8DefaultLayer;

    return TRUE;
}

static MS_BOOL _ZOrder_GetAreaByLayer(
                                    const MS_U32 u32Layer,
                                    MS_U32 * const pu32X,
                                    MS_U32 * const pu32Y,
                                    MS_U32 * const pu32W,
                                    MS_U32 * const pu32H)
{
    const MS_U32 u32UnitW = 320;
    const MS_U32 u32UnitH = 160;
    MS_U32 u32UnitX        = u32UnitW >> 1;
    MS_U32 u32UnitY        = u32UnitH >> 1;

    (*pu32X) = u32UnitX * u32Layer;
    (*pu32Y) = u32UnitY * u32Layer;
    (*pu32W) = u32UnitW;
    (*pu32H) = u32UnitH;

    printf("bound (W, H) = (%u, %u) \n", ((*pu32X) + (*pu32W)), ((*pu32Y) + (*pu32H)));
    printf("panel (W, H) = (%u, %u) \n", IPANEL(&gDevId, Width), IPANEL(&gDevId, Height));

    return TRUE;
}

static MS_BOOL _ZOrder_GetColorIndexByLayer(
                                    const EN_DISPLAY_LAYER keDisplayLayer,
                                    MS_U32 * const pu32ColorIndex)
{
    // color value in _ColorMapping()
    switch(keDisplayLayer)
    {
    case E_DISPLAY_LAYER_0:
        (*pu32ColorIndex) = 0;  //red
        break;
    case E_DISPLAY_LAYER_1:
        (*pu32ColorIndex) = 8;  //yellow
        break;
    case E_DISPLAY_LAYER_2:
        (*pu32ColorIndex) = 1;  //green
        break;
    case E_DISPLAY_LAYER_3:
        (*pu32ColorIndex) = 2;  //blue
        break;
    case E_DISPLAY_LAYER_4:
        (*pu32ColorIndex) = 9;  // light blue
        break;
    case E_DISPLAY_LAYER_5:
        (*pu32ColorIndex) = 10; // purple
        break;
    default:
        printf("[%s][%d] Error! \n", __FUNCTION__, __LINE__);
        return FALSE;
        break;
    }
    return TRUE;
}

static MS_BOOL _ZOrder_OsdFillBlock(const MS_U32 *pu32Target, MS_U32 *pu32index, MS_U32 *pu32RectX, MS_U32 *pu32RectY, MS_U32 *pu32RectWidth, MS_U32 *pu32RectHeight)
{
    GFX_RectFillInfo stgfxFillBlock;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    GFX_BufferInfo stgfxDstBuf;
    GOP_GwinFBAttr stDstFBInfo;
    GFX_RgbColor stcolor_s;
    GFX_RgbColor stcolor_e;
    MS_U8 u8DstFBfmt;
    MS_U32 u32Flag;

    //Map color index to ARGB color
    _ColorMapping(*pu32index, &stcolor_s);
    _ColorMapping(*pu32index, &stcolor_e);

    //get current GWIN fbInfo.
    //MApi_GOP_GWIN_GetFBInfo(MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &DstFBInfo);
    MApi_GOP_GWIN_GetFBInfo(cusGwinInfo[*pu32Target].u8FBId, &stDstFBInfo);

    u8DstFBfmt = (MS_U8)stDstFBInfo.fbFmt;

    //Set Dst buffer
    stgfxDstBuf.u32ColorFmt = stDstFBInfo.fbFmt;
    stgfxDstBuf.u32Addr     = stDstFBInfo.addr;
    stgfxDstBuf.u32Pitch    = stDstFBInfo.pitch;
    stgfxDstBuf.u32Width    = stDstFBInfo.width;
    stgfxDstBuf.u32Height   = stDstFBInfo.height;
    MApi_GFX_SetDstBufferInfo(&stgfxDstBuf, 0);
    printf("[%s][%d] Dst buffer addr = 0x%llx\n", __FUNCTION__, __LINE__,stDstFBInfo.addr);
    printf("[%s][%d] Dst buffer size = 0x%x\n", __FUNCTION__, __LINE__,stDstFBInfo.size);

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = stDstFBInfo.width;
    stgfxPt1.y = stDstFBInfo.height;
    //Set Clip window for rendering
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    u32Flag = GFXRECT_FLAG_COLOR_CONSTANT;

    _SetupGfxRectFillInfo(&stgfxFillBlock, *pu32RectX, *pu32RectY, *pu32RectWidth, *pu32RectHeight,
                                u8DstFBfmt, stcolor_s, stcolor_e, u32Flag);
    MApi_GFX_RectFill(&stgfxFillBlock);

    if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(cusGwinInfo[*pu32Target].u8GeWinId, TRUE) )
    {
         printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
         return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to test Z-Order
/// @param[in] video layer: 0(BOTTOM) ~ 4(TOP)
/// @param[in] Gop Id of Gop layer0
/// @param[in] Gop Id of Gop layer1
/// @param[in] Gop Id of Gop layer2
/// @param[in] Gop Id of Gop layer3
/// @return TRUE: success FALSE: failed
/// @note DO NOT use DMS driver GOPId
/// Command: \b ZOrder_SetZOrder 0 0 1 3 4 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_ZOrder_SetZOrder(
                                MS_U32* pu32VideoLayerOrder,
                                MS_U8* pu8Layer0Gop,
                                MS_U8* pu8Layer1Gop,
                                MS_U8* pu8Layer2Gop,
                                MS_U8* pu8Layer3Gop)
{

    s_u32VideoLayer = (*pu32VideoLayerOrder);
    MS_U8 u8DmsGop = s_u32GOPNum;
    _ZOrder_SetGopLayer(
                            pu8Layer0Gop,
                            pu8Layer1Gop,
                            pu8Layer2Gop,
                            pu8Layer3Gop,
                            &u8DmsGop);

    _ZOrder_SetVideoLayer(pu32VideoLayerOrder);

    return TRUE;
}
#endif


#if (DEMO_ZUI_TEST == 1)
//---------------------------------------------------------------------------------------------
/// @brief The sample code to initial GOP and GE for UI. Need to init XC and VE before init OSD.
/// @return None
/// @note
/// Command: \b OSD_Init \n
//---------------------------------------------------------------------------------------------
MS_BOOL Demo_UI_GEGOPInit(MS_U32 *pu32HD_GOP, MS_U32 *pu32SD_GOP, MS_U32 *pu32HD_DST, MS_U32 *pu32SD_DST)
{
    printf("Demo_OSD_Init. Need to init XC and VE before init OSD.\n");

    GOP_InitInfo stGopInit;
    GFX_Config stGFXcfg;
    EN_DDI_OSD_HD_DST_TYPE eDDIGOPHDDst = (EN_DDI_OSD_HD_DST_TYPE)(*pu32HD_DST);
    EN_DDI_OSD_SD_DST_TYPE eDDIGOPSDDst = (EN_DDI_OSD_SD_DST_TYPE)(*pu32SD_DST);
    memset(&stGopInit, 0, sizeof(GOP_InitInfo));
    memset(&stGFXcfg, 0, sizeof(GFX_Config));

    if( bGOPInit == FALSE)
    {
        if( FALSE == MDrv_SEM_Init() )
        {
             printf("[%s][%d] MDrv_SEM_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        // -Initial GE
        stGFXcfg.bIsCompt = TRUE;
        stGFXcfg.bIsHK = TRUE;
        MApi_GFX_Init(&stGFXcfg);
        printf("driver GE init ok\n");

        //Set getting UI font and bitmap callback function
        if(GFX_SUCCESS != MApi_GFX_RegisterGetFontCB(msAPI_OCP_GetFontInfo))
        {
             printf("[%s][%d] MApi_GFX_RegisterGetFontCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        if(GFX_SUCCESS != MApi_GFX_RegisterGetBMPCB(msAPI_OCP_GetBitmapInfo))
        {
             printf("[%s][%d] MApi_GFX_RegisterGetBMPCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        //Initial GOP
        if( GOP_API_SUCCESS != MApi_GOP_RegisterFBFmtCB(_OSD_SetFBFmt) )
        {
             printf("[%s][%d] MApi_GOP_RegisterFBFmtCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCIsInterlaceCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCGetCapHStartCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        if( GOP_API_SUCCESS != MApi_GOP_RegisterXCReduceBWForOSDCB(_XC_Sys_PQ_ReduceBW_ForOSD) )
        {
             printf("[%s][%d] MApi_GOP_RegisterXCReduceBWForOSDCB failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        stGopInit.u16PanelWidth = IPANEL(&gDevId ,Width);
        stGopInit.u16PanelHeight = IPANEL(&gDevId ,Height);
        stGopInit.u16PanelHStr = IPANEL(&gDevId ,HStart);

        printf("[%s][%d]====u16PanelWidth=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelWidth);
        printf("[%s][%d]====u16PanelHeight=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelHeight);
        printf("[%s][%d]====u16PanelHStr=%d===\n",__FUNCTION__,__LINE__,stGopInit.u16PanelHStr);

        stGopInit.u32GOPRBAdr = MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE);
        stGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
        stGopInit.u32GOPRegdmaAdr = MEM_ADR_BY_MIU(GOP_REG_DMA_BASE_ADR, GOP_REG_DMA_BASE_MEMORY_TYPE);
        stGopInit.u32GOPRegdmaLen = GOP_REG_DMA_BASE_LEN;
        stGopInit.bEnableVsyncIntFlip = FALSE;

        if( GOP_API_SUCCESS != MApi_GOP_Init(&stGopInit) )
        {
             printf("[%s][%d] MApi_GOP_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
        printf("GOP_GWIN_RB_ADR : %"DTC_MS_PHY_x", GOP_REG_DMA_BASE_ADR: %"DTC_MS_PHY_x"\n", stGopInit.u32GOPRBAdr, stGopInit.u32GOPRegdmaAdr);
        printf("driver GOP init ok\n");
    }

    if((*pu32HD_GOP < MApi_GOP_GWIN_GetMaxGOPNum()) && (*pu32SD_GOP < MApi_GOP_GWIN_GetMaxGOPNum()))
    {
        u32GOP0  = *pu32HD_GOP;
        u32GOP1 = *pu32SD_GOP;
    }
    else
    {
       printf("Error GOP Number HD:%"DTC_MS_U32_d" SD:%"DTC_MS_U32_d"\n",*pu32HD_GOP,*pu32SD_GOP);
       return FALSE;
    }

    //Use two GOPs, here we use GOP2 to display for HD and use GOP2 to display for SD
    u8GOPCount = 2;
    u8DrawGOPCount = 2;

    //--------------To HD---------------//
    switch(eDDIGOPHDDst)
    {
        case E_DDI_OSD_HD_DST_OP0:
            gWinInfo[0].eGopDest = E_GOP_DST_OP0;
            break;
        case E_DDI_OSD_HD_DST_IP0:
            gWinInfo[0].eGopDest = E_GOP_DST_IP0;
            break;
        case E_DDI_OSD_HD_DST_VOP:
            gWinInfo[0].eGopDest = E_GOP_DST_VOP;
            break;
        case E_DDI_OSD_HD_DST_VOP_SUB:
            gWinInfo[0].eGopDest = E_GOP_DST_VOP_SUB;
            break;
        case E_DDI_OSD_HD_DST_IP_SUB:
            gWinInfo[0].eGopDest = E_GOP_DST_IP_SUB;
            break;
#if (DEMO_GOP_GOPSC_TEST == 1)
        case E_DDI_OSD_HD_DST_GOPScaling:
            gWinInfo[0].eGopDest = E_GOP_DST_GOPScaling;
            break;
#endif
        default:
            printf("[%s][%d]Not supported HD path:%"DTC_MS_U32_d"\n",__FUNCTION__,__LINE__,*pu32HD_DST);
            return FALSE;
    }

    /*If use 'GOP2' and the destination of GOP is 'OP',
      use this to change GOP output layer to the last one.
      The input is the GOP number
    */
    //MApi_GOP_GWIN_OutputLayerSwitch(2);

    //---------------To SD---------------//
    switch(eDDIGOPSDDst)
    {
        case E_DDI_OSD_SD_DST_OP1:
            gWinInfo[1].eGopDest = E_GOP_DST_OP1;
            break;
        case E_DDI_OSD_SD_DST_IP1:
            gWinInfo[1].eGopDest = E_GOP_DST_IP1;
            break;
        case E_DDI_OSD_SD_DST_VE:
            gWinInfo[1].eGopDest = E_GOP_DST_VE;
            break;
        case E_DDI_OSD_SD_DST_MIXER2VE:
            gWinInfo[1].eGopDest = E_GOP_DST_MIXER2VE;
            break;
        case E_DDI_OSD_SD_DST_OP2VE://OP2VE
            u8GOPCount = 1;
            u8DrawGOPCount = 1;
            break;
        default:
            printf("[%s][%d]Not supported SD path:%"DTC_MS_U32_d"\n",__FUNCTION__,__LINE__,*pu32SD_DST);
            return FALSE;
    }

    if((u8GOPCount == 2) && (u32GOP0 == u32GOP1))
    {
         printf("[%s][%d] Do not use the same GOP to set HDMI and CVBS.", __FUNCTION__, __LINE__);
         return FALSE;
    }
    MApi_GOP_GWIN_SwitchGOP(u32GOP0);//use GOP 0
#if ENABLE_MIU_1
    if(MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE) > MIU_INTERVAL)
    {
        MApi_GOP_MIUSel(u32GOP0,E_GOP_SEL_MIU1);
    }
    else
    {
        MApi_GOP_MIUSel(u32GOP0,E_GOP_SEL_MIU0);
    }
#endif
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, TRUE);
    MApi_GOP_GWIN_OutputLayerSwitch(u32GOP0);//Set GOP0 show in the top layer (mux2)

    if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetGOPDst(u32GOP0, gWinInfo[0].eGopDest))
    {
         printf("[%s][%d] HD MApi_GOP_GWIN_SetGOPDst failed", __FUNCTION__, __LINE__);
         return FALSE;
    }

    if(u8GOPCount == 2)
    {
        MApi_GOP_GWIN_SwitchGOP(u32GOP1);//use GOP 1
#if ENABLE_MIU_1
    if(MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE) > MIU_INTERVAL)
    {
        MApi_GOP_MIUSel(u32GOP1,E_GOP_SEL_MIU1);
    }
    else
    {
        MApi_GOP_MIUSel(u32GOP1,E_GOP_SEL_MIU0);
    }
#endif
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, TRUE);
        if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetGOPDst(u32GOP1, gWinInfo[1].eGopDest))
        {
             printf("[%s][%d] SD MApi_GOP_GWIN_SetGOPDst failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    // for CVBS output, use VE
    // To choose video to VE mux
    if(eDDIGOPSDDst == E_DDI_OSD_SD_DST_OP2VE)
    {
        // Set OP can through VE output to CVBS
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
        // Disable blackScreen. If video enable black screen, OP2VE mode can't show graph on CVBS.
        MDrv_VE_SetBlackScreen(FALSE);
    }
    else
    {
        MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_BLEND1);
    }

    bGOPInit = TRUE;
    return TRUE;
}

MS_U8 Demo_OSD_GetHDGOPDst(void)
{
    return gWinInfo[0].eGopDest;
}

MS_U8 Demo_OSD_GetSDGOPDst(void)
{
    return gWinInfo[1].eGopDest;
}

//------------------------------------------------------------------------------
/// @brief The sample code to get HD GOP Number
/// @return HD GOP Number
/// @note
/// Command: \b Demo_OSD_GetHDGOPNum \n
//------------------------------------------------------------------------------
MS_U8 Demo_OSD_GetHDGOPNum(void)
{
    return (MS_U8)u32GOP0;
}
//------------------------------------------------------------------------------
/// @brief The sample code to get SD GOP Number
/// @return SD GOP Number
/// @note
/// Command: \b Demo_OSD_GetSDGOPNum \n
//------------------------------------------------------------------------------
MS_U8 Demo_OSD_GetSDGOPNum(void)
{
    return (MS_U8)u32GOP1;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code to do OSD_Init and set palette GOP automatically. Need to init XC and VE before init OSD.
/// @return None
/// @note
/// Command: \b OSD_PTGOP_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_PTGOP_Init(void)
{
    MS_U32 u32HD_GOP = DEMO_PALETTE_GOP;
    MS_U32 u32SD_GOP = u32GOP1;
    MS_U32 u32HD_DST = (MS_U32) E_DDI_OSD_HD_DST_OP0;
    MS_U32 u32SD_DST = (MS_U32) E_DDI_OSD_SD_DST_OP2VE;

    Demo_OSD_Init(&u32HD_GOP, &u32SD_GOP, &u32HD_DST, &u32SD_DST);
    bGOPInit = TRUE;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set configurations including stretch win size
/// @return None
/// @note
/// Command: \b OSD_SetConfig \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_SetConfig(void)
{
    GOP_MixerTiming stgopMixerTiming; //GopMixerTiming is reserved for customers. It is useless in demo code.
    MS_BOOL bVE_PAL = TRUE;
    MS_U8 u8Interlace = 0;
    MS_U16 u16CropBtm = 0;

    //Dwin buffer size set to panel width and height to capture the whole frame
    MS_U16 u16WinWidth = IPANEL(&gDevId, Width);
    MS_U16 u16WinHeight = IPANEL(&gDevId, Height);

    //--------------To HD---------------//
    MApi_GOP_GWIN_SwitchGOP(u32GOP0);//use GOP 0
    //Set stretch window every re-initial
    //Set GOP destination to OP
    if((gWinInfo[0].eGopDest == E_GOP_DST_OP0) || (gWinInfo[0].eGopDest == E_GOP_DST_GOPScaling))
    {
        MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP0,gWinInfo[0].eGopDest,0,0,IPANEL(&gDevId, Width),IPANEL(&gDevId, Height));
    }
    else
    {
        _get_video_info(&u8Interlace, &u16WinWidth, &u16WinHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW);
        u16WinHeight = u16WinHeight - u16CropBtm;
        MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP0,gWinInfo[0].eGopDest,0,0,u16WinWidth,u16WinHeight);
    }
    // Every time change resolution, the HStart should be set again
    printf("Panel HStart = %d\n",IPANEL(&gDevId, HStart));
    MApi_GOP_SetGOPHStart(u32GOP0,IPANEL(&gDevId,HStart));

    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);//FIXME
    if(u8GOPCount == 2)
    {
        //---------------To SD---------------//
        MApi_GOP_GWIN_SwitchGOP(u32GOP1);//use GOP 1
        if(MDrv_VE_Get_Output_Video_Std() == MS_VE_NTSC)
        {
            printf("Set NTSC\n");
            bVE_PAL = FALSE;
            //GOPSC_width = 480;
            gopMixerTimingType = GOP_NTSC;
            gopVETimingType = GOP_VE_NTSC;
        }else
        {
            printf("Set PAL\n");
            bVE_PAL = TRUE;
            //GOPSC_width = 576;
            gopMixerTimingType = GOP_PAL;
            gopVETimingType = GOP_VE_PAL;
        }

        if(gWinInfo[1].eGopDest == E_GOP_DST_MIXER2VE)
        {
            MApi_GOP_MIXER_SetOutputTiming(gopMixerTimingType, &stgopMixerTiming);
        }
        else if(gWinInfo[1].eGopDest == E_GOP_DST_VE)
        {
            MApi_GOP_VE_SetOutputTiming(gopVETimingType);
        }
    }

//This case is only for K3, GE line, rectangle..., use 2 GOP to draw. HD dst is GOPScaling, SD dst is VE.
#if (DEMO_GOP_GOPSC_TEST == 1)
    if((u8DrawGOPCount == 1) && (u8GOPCount == 2))
    {
        MApi_GOP_VE_SetOutputTiming(gopVETimingType);
    }
#endif
    if(u8GOPCount == 2)
    {
        //Set stretch window every re-initial
        //Set GOP destination to OP
        if(!bVE_PAL)
        {
            MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP1,gWinInfo[1].eGopDest, 0, 0, 720, 480);
        }
        else
        {
            MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP1,gWinInfo[1].eGopDest, 0, 0, 720, 576);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to create GWin
/// @param[in] ColorType color type for the GWin.
/// @param[in] GWinWidth0 The width of HD GWin to create. it is small than stretch win width
/// @param[in] GWinHeight0 The height of HD GWin to create. it is small than stretch win height
/// @param[in] GWinWidth1 The width of SD GWin to create. it is small than stretch win width
/// @param[in] GWinHeight1 The height of SD GWin to create. it is small than stretch win height
/// @return None
/// @note
/// Command: \b OSD_CreateWin \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_CreateWin(MS_U32 *pu32ColorType, MS_U32 *pu32GWinWidth0, MS_U32 *pu32GWinHeight0, MS_U32 *pu32GWinWidth1, MS_U32 *pu32GWinHeight1)
{
    MS_U8 u8GOP[2] = {0,0};
    MS_U8 u8GWin[2] = {0,0};
    MS_U8 u8FB[2] = {0,0};
    MS_U16 u16FBWidth[2] = {0,0};
    MS_U16 u16FBHeight[2] = {0,0};
    MS_U16 u16FBFmt[2] = {0,0};
    MS_U32 u32BmpDramRblkStart[2] = {0,0};
    MS_U16 u16bmpWidth[2] = {0,0};
    MS_U16 u16bmpHeight[2] = {0,0};
    MS_U8 u8Count;

    //If there is any GWin exit, disable it!
    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( gWinInfo[u8Count].u8GeWinId!= 0xFF)
        {
            printf("Delete win %d\n", gWinInfo[u8Count].u8GeWinId);
            MApi_GOP_GWIN_DeleteWin( gWinInfo[u8Count].u8GeWinId );
        }
    }

    //Select frame buffer format to create
    if(bSetClrFmt == TRUE)
    {
        switch(*pu32ColorType)
        {
            case 0:
                gWinInfo[0].u16GeDstFmt = E_MS_FMT_ARGB8888;
                gWinInfo[1].u16GeDstFmt = E_MS_FMT_ARGB8888;
                break;
            case 1:
                gWinInfo[0].u16GeDstFmt = E_MS_FMT_RGB565;
                gWinInfo[1].u16GeDstFmt = E_MS_FMT_RGB565;
                break;
            case 2:
                gWinInfo[0].u16GeDstFmt = E_MS_FMT_ARGB4444;
                gWinInfo[1].u16GeDstFmt = E_MS_FMT_ARGB4444;
                break;
            case 3:
                printf("[%s][%d] GWIN1 can no use palette table, set format to RGB565!\n", __FUNCTION__, __LINE__);
                if(u32GOP0 != DEMO_PALETTE_GOP)
                {
                    printf("[%s][%d] Please use GOP%d when dst color I8 mode!\n", __FUNCTION__, __LINE__,DEMO_PALETTE_GOP);
                }
                gWinInfo[0].u16GeDstFmt = E_MS_FMT_I8;
                gWinInfo[1].u16GeDstFmt = E_MS_FMT_RGB565;
                break;
            case 4:
                gWinInfo[0].u16GeDstFmt = E_MS_FMT_YUV422;
                gWinInfo[1].u16GeDstFmt = E_MS_FMT_YUV422;
                break;
            default:
                gWinInfo[0].u16GeDstFmt = E_MS_FMT_ARGB4444;
                gWinInfo[1].u16GeDstFmt = E_MS_FMT_ARGB4444;
                break;
        }
    }
    else
    {
        for(u8Count = 0; u8Count < u8DrawGOPCount; u8Count++)
        {
            gWinInfo[u8Count].u16GeDstFmt = u16gwin_fmt;
        }
    }
    _SetupEnvByDstFmt(gWinInfo[0].u16GeDstFmt, &u32BmpDramRblkStart[0], &u16bmpWidth[0], &u16bmpHeight[0]);
    _SetupEnvByDstFmt(gWinInfo[1].u16GeDstFmt, &u32BmpDramRblkStart[1], &u16bmpWidth[1], &u16bmpHeight[1]);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        gWinInfo[u8Count].stbmpInfo.u32BmpDramRblkStart = u32BmpDramRblkStart[u8Count];
        gWinInfo[u8Count].stbmpInfo.u16bmpWidth = u16bmpWidth[u8Count];
        gWinInfo[u8Count].stbmpInfo.u16bmpHeight = u16bmpHeight[u8Count];
    }

    //GWin settings
    u8GOP[0] = gWinInfo[0].u8GOPId = u32GOP0;
    u8FB[0] = gWinInfo[0].u8FBId;
    u16FBWidth[0] = gWinInfo[0].u16GopBitmapWidth = *pu32GWinWidth0;
    u16FBHeight[0] = gWinInfo[0].u16GopBitmapHeight = *pu32GWinHeight0;
    u16FBFmt[0] = gWinInfo[0].u16GeDstFmt;// = u16gwin_fmt;
    //u16gwin_fmt = gWinInfo[0].GeDstFmt;

    if(u8DrawGOPCount == 2)
    {
        u8GOP[1] = gWinInfo[1].u8GOPId = u32GOP1;
        u8FB[1] = gWinInfo[1].u8FBId;
        u16FBWidth[1] = gWinInfo[1].u16GopBitmapWidth = *pu32GWinWidth1;
        u16FBHeight[1] = gWinInfo[1].u16GopBitmapHeight = *pu32GWinHeight1;
        u16FBFmt[1] = gWinInfo[1].u16GeDstFmt;// = u16gwin_fmt;
        //u16gwin_fmt = gWinInfo[1].GeDstFmt;
    }

//This case is only for K3, GE line, rectangle..., use 2 GOP to draw. HD dst is GOPScaling, SD dst is VE.
#if (DEMO_GOP_GOPSC_TEST == 1)
    if((u8DrawGOPCount == 1) && (u8GOPCount == 2))
    {
        u8GOP[1] = u32GOP1;
        u8FB[1] = gWinInfo[1].u8FBId;

        if(gopVETimingType == GOP_VE_NTSC)
        {
            u16FBWidth[1] = gWinInfo[1].u16GopBitmapWidth = 720;
            u16FBHeight[1] = gWinInfo[1].u16GopBitmapHeight = 480;
        }
        else
        {
            u16FBWidth[1] = gWinInfo[1].u16GopBitmapWidth = 720;
            u16FBHeight[1] = gWinInfo[1].u16GopBitmapHeight = 576;
        }
        u16FBFmt[1] = gWinInfo[1].u16GeDstFmt = E_MS_FMT_ARGB8888;
    }
#endif

    //Create Frame buffer and Gwin
    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( 0 != _GOP_Create_FB(&u8FB[u8Count], u16FBWidth[u8Count], u16FBHeight[u8Count], u16FBFmt[u8Count]))
        {
             printf("Create FB fail\n");
             return FALSE;
        }
        gWinInfo[u8Count].u8FBId = u8FB[u8Count];

        u8GWin[u8Count] = gWinInfo[u8Count].u8GeWinId;
        // -Create GWin
        if( 0 != _GOP_Create_GWin(u8GOP[u8Count], &u8GWin[u8Count], u8FB[u8Count], u16FBFmt[u8Count] ))
        {
             printf("Create GWin fail, GOPnum = %d, u8GWin = %d, FBId = %d, FBFmt = %d\n", u8GOP[u8Count],  u8GWin[u8Count], u8FB[u8Count], u16FBFmt[u8Count]);
             return FALSE;
        }
        gWinInfo[u8Count].u8GeWinId = u8GWin[u8Count];
        printf("Create GWin ok, GOPnum = %d, u8GWin = %d, FBId = %d, FBFmt = %d\n", u8GOP[u8Count],  u8GWin[u8Count], u8FB[u8Count], u16FBFmt[u8Count]);
    }

//This case is only for K3, GE line, rectangle..., use 2 GOP to draw. HD dst is GOPScaling, SD dst is VE.
#if (DEMO_GOP_GOPSC_TEST == 1)
    if((u8DrawGOPCount == 1) && (u8GOPCount == 2))
    {
        //Setup GOPSC
        //Init GOPSC
        MApi_GOPSC_Init(NULL);
        //Set size
        MApi_GOPSC_Enable(NULL,TRUE);
        GOPSC_SizeInfo gopscInfo;
        gopscInfo.srcWidth = IPANEL(&gDevId, Width);
        gopscInfo.srcHeight = IPANEL(&gDevId, Height);
        gopscInfo.destWidth= 720;
        gopscInfo.destHeight= (gopVETimingType == GOP_VE_NTSC)? 480 : 576;
        gopscInfo.bEnableScaleX = TRUE;
        gopscInfo.bEnableScaleY = TRUE;
        MApi_GOPSC_SetHVSPSize(NULL,&gopscInfo,0);
        //Set DWIN capture
        GOP_DwinProperty dwinProperty;
        MS_PHYADDR dWinBufAddr;

        //Dwin buffer size set to panel width and height to capture the whole frame
        MS_U16 dWinBufWidth = IPANEL(&gDevId, Width);
        MS_U16 dWinBufHeight = IPANEL(&gDevId, Height);
        GOP_GwinFBAttr bufAttr;

        //DWIN initialization
        MApi_GOP_DWIN_Init();

        //Source can only from Scaler IP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_GOPScaling);
        //Is the DWIN source is IP, we need to know the video size and interlace or progressive
        //OP scan type is progressive
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);

        if (gopVETimingType == GOP_VE_NTSC)
        {
            dWinBufWidth = 720;
            dWinBufHeight = 480;
        }
        else
        {
            dWinBufWidth = 720;
            dWinBufHeight = 576;
        }

        MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_ARGB8888);
        MApi_GOP_GWIN_SwitchGOP(u32GOP1);
        MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);
        //At GOP_Scaling case, dwin capture always capture YUV domain
        //So output color set to RGB for skip RGB to YUV transform
        MApi_GOP_DWIN_SetAlphaSrc(DWIN_ALPHA_SRC_SOURCE);
        MApi_GOP_DWIN_SetAlphaValue(0x00);

        MApi_GOP_GWIN_GetFBInfo(gWinInfo[1].u8FBId, &bufAttr);
        dWinBufAddr = bufAttr.addr;
        printf("[%s][%d] captured FB addr : 0x%lx\n", __FUNCTION__, __LINE__, dWinBufAddr);

        // Set DWin property
        dwinProperty.u16x = 0;
        dwinProperty.u16y = 0;
        dwinProperty.u16fbw = dWinBufWidth;
        dwinProperty.u16w = dWinBufWidth;
        dwinProperty.u16h = dWinBufHeight;
        dwinProperty.u32fbaddr0 = dWinBufAddr;
        dwinProperty.u32fbaddr1 = dWinBufAddr + bufAttr.size;
        printf("[%s][%d] addr : 0x%lx, size : %ld\n", __FUNCTION__, __LINE__, dWinBufAddr, bufAttr.size);

        MApi_GOP_DWIN_SetWinProperty(&dwinProperty);
        MApi_GOP_DWIN_Enable(TRUE);
    }
#endif
    if(u8DrawGOPCount == 1)
    {
        // Check again blackScreen is disable when OP2VE mode.(Since switching outputtiming doesn't need to set osd_init again.)
        if(MDrv_VE_IsBlackScreenEnabled() == TRUE)
        {
            MDrv_VE_SetBlackScreen(FALSE);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw bitmap in the image buffer for this GWin
/// @return None
/// @note
/// Command: \b OSD_DrawBmp \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawBmp(MS_U32 *pu32Index)
{
    MS_U32 u32X = 0;
    MS_U32 u32Y = 0;
    MS_U32 u32width = 0;
    MS_U32 u32height = 0;
    MS_U32 u32Target = 0;
    MS_U8  u8Count =0;

    if(gWinInfo[0].eGopDest == E_GOP_DST_GOPScaling)
    {
        u8Count = 1;
    }
    else
    {
        u8Count = u8GOPCount;
    }

    for(u32Target = 0; u32Target < u8Count; u32Target++)
    {
        u32width = gWinInfo[u32Target].u16GopBitmapWidth;
        u32height = gWinInfo[u32Target].u16GopBitmapHeight;
        Demo_OSD_DrawBmpXY(&u32Target, pu32Index, &u32X, &u32Y, &u32width, &u32height);
    }
    return TRUE;
}

#if (DEMO_GOP_3DOSD_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to bitblt 3D left and right picture
/// @return None
/// @note
/// Command: \b OSD_Draw3DBmp \n
//------------------------------------------------------------------------------
static MS_BOOL _OSD_Draw3DBmp(void)
{
    MS_U8 i;
    GFX_BufferInfo srcbuf[2];
    GFX_DrawRect bitbltInfo[2];
    GFX_Point gfxPt0[2];
    GFX_Point gfxPt1[2];
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;
    MS_U8 u8SubFB = 0;
    MS_U32 u32SrcColorFmt = E_MS_FMT_ARGB8888;//Source 3D color format

    //Create another frame buffer for 3D right picture. The width and height is the same as left picture.
    if( 0 != _GOP_Create_FB(&u8SubFB, gWinInfo[0].u16GopBitmapWidth, gWinInfo[0].u16GopBitmapHeight, gWinInfo[0].u16GeDstFmt))
    {
         printf("Create FB fail\n");
         return FALSE;
    }
    gWinInfo[1].u8FBId = u8SubFB;

    //Set 3D picture source address
    gWinInfo[0].stbmpInfo.u32BmpDramRblkStart = u32Bmp3DLAddr;
    gWinInfo[1].stbmpInfo.u32BmpDramRblkStart = u32Bmp3DRAddr;

    //Use GE to bitblt 3D left and right picture from source buffer to destination buffer
    //The width, height, color format, pitch of right picture is the same as left picture
    //Only different is source address and FBId
    for(i = 0; i < 2; i++)
    {
        MApi_GOP_GWIN_SwitchGOP(gWinInfo[0].u8GOPId);
        //Set Clip window for rendering
        gfxPt0[i].x = 0;
        gfxPt0[i].y = 0;
        gfxPt1[i].x = gWinInfo[0].u16GopBitmapWidth;
        gfxPt1[i].y = gWinInfo[0].u16GopBitmapHeight;
        MApi_GFX_SetClip(&gfxPt0[i], &gfxPt1[i]);

        MApi_GOP_GWIN_Switch2FB( gWinInfo[i].u8FBId );  //set dst buffer

        // Bitble the bitmap to DRAM of GE
        srcbuf[i].u32ColorFmt = u32SrcColorFmt;
        srcbuf[i].u32Addr = gWinInfo[i].stbmpInfo.u32BmpDramRblkStart;
        srcbuf[i].u32Width = BMP3DW;
        srcbuf[i].u32Height = BMP3DH;
        srcbuf[i].u32Pitch = _CalcPitch(u32SrcColorFmt, BMP3DW);
        MApi_GFX_SetSrcBufferInfo(&srcbuf[i], 0);

        bitbltInfo[i].srcblk.x = 0;
        bitbltInfo[i].srcblk.y = 0;
        bitbltInfo[i].srcblk.width = BMP3DW;
        bitbltInfo[i].srcblk.height = BMP3DH;

        bitbltInfo[i].dstblk.x = 0;
        bitbltInfo[i].dstblk.y = 0;
        bitbltInfo[i].dstblk.width = gWinInfo[0].u16GopBitmapWidth;
        bitbltInfo[i].dstblk.height = gWinInfo[0].u16GopBitmapHeight;

        MApi_GFX_BitBlt(&bitbltInfo[i], u32Flag);
        MApi_GFX_FlushQueue();
    }
    if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[0].u8GeWinId, TRUE) )
    {
         printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
         return GWIN_FAIL;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to display 3D OSD
/// @param[in] set 3DOSD mode.
/// @return None
/// @note
/// Command: \b OSD_3DOSD \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_Draw3DOSD(MS_U8 *pu8mode)
{
    _OSD_Draw3DBmp();
    MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP0,gWinInfo[0].eGopDest,0,0,gWinInfo[0].u16GopBitmapWidth,gWinInfo[0].u16GopBitmapHeight);

    switch(*pu8mode)
    {
        case 0:
            MApi_GOP_Set3DOSDMode( gWinInfo[0].u8GeWinId , gWinInfo[0].u8FBId,gWinInfo[1].u8FBId, E_GOP_3D_SWITH_BY_FRAME);
            break;
        case 1:
            MApi_GOP_Set3DOSDMode( gWinInfo[0].u8GeWinId , gWinInfo[0].u8FBId,gWinInfo[1].u8FBId, E_GOP_3D_SIDE_BY_SYDE);
            break;
        case 2:
            MApi_GOP_Set3DOSDMode( gWinInfo[0].u8GeWinId , gWinInfo[0].u8FBId,gWinInfo[1].u8FBId, E_GOP_3D_TOP_BOTTOM);
            break;
        case 3:
            MApi_GOP_Set3DOSDMode( gWinInfo[0].u8GeWinId , gWinInfo[0].u8FBId,gWinInfo[1].u8FBId, E_GOP_3D_LINE_ALTERNATIVE);
            break;
        case 4:
            MApi_GOP_Set3DOSDMode( gWinInfo[0].u8GeWinId , gWinInfo[0].u8FBId,gWinInfo[1].u8FBId, E_GOP_3D_DISABLE);
            break;
        case 5:
            MApi_GOP_Set3DOSDMode( gWinInfo[0].u8GeWinId , gWinInfo[0].u8FBId,gWinInfo[0].u8FBId, E_GOP_3D_FRAMEPACKING);
            MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP0, gWinInfo[0].eGopDest, 0, 0, gWinInfo[0].u16GopBitmapWidth, gWinInfo[0].u16GopBitmapHeight);
            break;
        default:
            printf("Not supported 3D mode %d\n",*pu8mode);
            break;
    }

    return TRUE;
}
#endif
//------------------------------------------------------------------------------
/// @brief The sample code to Scale OSD
/// @param[in] set H scale rate.
/// @param[in] set V scale rate.
/// @return None
/// @note
/// Command: \b OSD_HVScale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_HVScale(MS_BOOL *pbEnableScale, MS_U32 *pu32Hscale_rate, MS_U32 *pu32Vscale_rate)
{
    MS_U8 u8Count;

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if(u8Count == 0)
        {
            MApi_GOP_GWIN_SwitchGOP(u32GOP0);
            MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP0,gWinInfo[u8Count].eGopDest,0,0,gWinInfo[u8Count].u16GopBitmapWidth,gWinInfo[u8Count].u16GopBitmapHeight);
        }
        else
        {
            MApi_GOP_GWIN_SwitchGOP(u32GOP1);
            MApi_GOP_GWIN_Set_STRETCHWIN(u32GOP1,gWinInfo[u8Count].eGopDest,0,0,gWinInfo[u8Count].u16GopBitmapWidth,gWinInfo[u8Count].u16GopBitmapHeight);
        }

        MApi_GOP_GWIN_Set_HSCALE(*pbEnableScale, 1, *pu32Hscale_rate);
        MApi_GOP_GWIN_Set_VSCALE(*pbEnableScale, 1, *pu32Vscale_rate);
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use DWin to captrue a video frame
/// @param[in] CaptureOneFrame auto-stop capture after one frame has been captured.
/// @param[in] GWinDsipCount The count that using GWin to displpay the captured frame
/// @param[in] Source The source to capture.
/// @return None
/// @note
/// Command: \b OSD_DWinCapture \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DWinCapture(MS_U32 *pu32CaptureOneFrame, MS_U32 *pu32GWinDsipCount, MS_U32 *pu32Source)
{
    GOP_DwinProperty stdwinProperty;
    MS_PHYADDR dWinBufAddr;
#if (DEMO_XC_DUALXC_TEST == 1)
    MS_U8 u8Interlace = 0;
    MS_U16 u16CropBtm = 0;
#endif

    //Dwin buffer size set to panel width and height to capture the whole frame
    MS_U16 u16dWinBufWidth = IPANEL(&gDevId, Width);
    MS_U16 u16dWinBufHeight = IPANEL(&gDevId, Height);
    GOP_GwinFBAttr stbufAttr;

    //Gwin size maybe different to panel size; GE_Bitblt with scaling to change size to display
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;
    GFX_BufferInfo stsrcbuf[2];
    GFX_BufferInfo stdstbuf[2];
    GFX_DrawRect stbitbltInfo[2];
    GFX_Point stgfxPt0[2];
    GFX_Point stgfxPt1[2];
    MS_U32 u32DispCount = *pu32GWinDsipCount;
    MS_U8 u8Count;

    //GOP address and add double buffer to avoid blink
    MS_U32 u32FBAddr0[2] = {0,0};//original GOP address for HD,SD
    MS_U32 u32FBAddr1[2] = {0,0};//second buffer GOP address for HD,SD
    MS_U8 u8MIUSELAddr0 = E_GOP_SEL_MIU0;
    MS_U8 u8MIUSELAddr1 = E_GOP_SEL_MIU0;
    GOP_GwinFBAttr gwinattr;
    void* dualgopbuf[2] = {NULL,NULL};
    MS_PNL_DST_DispInfo stDstInfo;

    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    //DWIN initialization
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
    if(bDwinInit == FALSE)
    {
        MApi_XC_DIP_InitByDIP(DIP_WINDOW);
    }
    bDwinInit = TRUE;

    u32DIPStartTime = MsOS_GetSystemTime();
    while(MApi_XC_DIP_GetResource(DIP_WINDOW) == E_APIXC_RET_FAIL)
    {
        if(MsOS_GetSystemTime() - u32DIPStartTime > DIP_Timeout)
        {
            printf("[%s][%d] DIP Timeout!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        MsOS_DelayTask(1);
    }
#else
    MApi_GOP_DWIN_Init();
#endif

    if(0 == *pu32Source)
    {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        SrcSlt = DWIN_SRC_OP;
        memset(&stDstInfo, 0, sizeof(MS_PNL_DST_DispInfo));
        MApi_PNL_GetDstInfo(&stDstInfo, sizeof(stDstInfo));
        if(stDstInfo.bInterlaceMode == TRUE)
        {
            MApi_XC_DIP_SetSourceScanType(DIP_SCAN_MODE_extern, DIP_WINDOW);
        }
        else
        {
            MApi_XC_DIP_SetSourceScanType(DIP_SCAN_MODE_PROGRESSIVE, DIP_WINDOW);
        }
#else
        //Source can only from Scaler OP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);

        //OP scan type is progressive, but keltic and kaiser SC1 is interlace
        memset(&stDstInfo, 0, sizeof(MS_PNL_DST_DispInfo));
        MApi_PNL_GetDstInfo(&stDstInfo, sizeof(stDstInfo));
        if(stDstInfo.bInterlaceMode == TRUE)
        {
            MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_extern);
        }
        else
        {
            MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
        }

#endif
        u16dWinBufWidth = IPANEL(&gDevId, Width);
        u16dWinBufHeight = IPANEL(&gDevId, Height);
        printf("Source is OP, dWinBufWidth = %d, dWinBufHeight = %d\n", u16dWinBufWidth, u16dWinBufHeight);
    }
#if (DEMO_XC_DUALXC_TEST == 1)
    else if(1 == *pu32Source)
    {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        SrcSlt = DWIN_SRC_OP1;
        MApi_XC_DIP_SetSourceScanType(DIP_SCAN_MODE_PROGRESSIVE, DIP_WINDOW);
#else
        //Source can only from Scaler OP1
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP1);
        /*
        (1)SCAN type: DWIN_SCAN_MODE_PROGRESSIVE
             - Contiguous captured/Auto stop captured: In each vsync, capture the top or bottom field
        (2)SCAN type: DWIN_SCAN_MODE_extern
             - Contiguous captured: contiguously captured top and bottom field
             - Auto stop captured: Hardware will auto stop once the bottom field is captured.
        */
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
#endif
        if (gopVETimingType == GOP_VE_NTSC)
        {
            u16dWinBufWidth = 720;
            u16dWinBufHeight = 480>>1;
        }
        else
        {
            u16dWinBufWidth = 720;
            u16dWinBufHeight = 576>>1;
        }
    }
#endif

#if (DEMO_GOP_GOPSC_TEST == 1)
    else if(2 == *pu32Source)
    {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        SrcSlt = DWIN_SRC_GOPScaling;
        MApi_XC_DIP_SetSourceScanType(DIP_SCAN_MODE_PROGRESSIVE, DIP_WINDOW);
#else
        //Source can only from Scaler IP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_GOPScaling);
        //Is the DWIN source is IP, we need to know the video size and interlace or progressive
        //OP scan type is progressive
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
#endif
        if (gopVETimingType == GOP_VE_NTSC)
        {
            u16dWinBufWidth = 720;
            u16dWinBufHeight = 480;
        }
        else
        {
            u16dWinBufWidth = 720;
            u16dWinBufHeight = 576;
        }

        printf("Source is OP, dWinBufWidth = %d, dWinBufHeight = %d\n", u16dWinBufWidth, u16dWinBufHeight);

        switch(gWinInfo[0].u16GeDstFmt)
        {
           case E_MS_FMT_ARGB8888:
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
               MApi_XC_DIP_SetOutputDataFmt(DIP_DATA_FMT_ARGB8888, DIP_WINDOW);
#else
               MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_ARGB8888);
#endif
               MApi_GOP_GWIN_SwitchGOP(u32GOP1);
               MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);
               //At GOP_Scaling case, dwin capture always capture YUV domain
               //So output color set to RGB for skip RGB to YUV transform
               MApi_GOP_DWIN_SetAlphaValue(0xFF);
               break;
           case E_MS_FMT_RGB565:
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
               MApi_XC_DIP_SetOutputDataFmt(DIP_DATA_FMT_RGB565, DIP_WINDOW);
#else
               MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_RGB565);
#endif
               MApi_GOP_GWIN_SwitchGOP(u32GOP1);
               MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);
               //At GOP_Scaling case, dwin capture always capture YUV domain
               //So output color set to RGB for skip RGB to YUV transform
               break;
           default:
               printf("[%s][%d] Not supported DwinData Type\n",__FUNCTION__,__LINE__);
               return FALSE;
        }

        MApi_GOP_GWIN_GetFBInfo(gWinInfo[1].u8FBId, &stbufAttr);
        dWinBufAddr = stbufAttr.addr;
        printf("[%s][%d] captured FB addr : 0x%"DTC_MS_PHYADDR_x"\n", __FUNCTION__, __LINE__, dWinBufAddr);

        // Set DWin property
        stdwinProperty.u16x = 0;
        stdwinProperty.u16y = 0;
        stdwinProperty.u16fbw = u16dWinBufWidth;
        stdwinProperty.u16w = u16dWinBufWidth;
        stdwinProperty.u16h = u16dWinBufHeight;
        stdwinProperty.u32fbaddr0 = dWinBufAddr;
        stdwinProperty.u32fbaddr1 = dWinBufAddr + stbufAttr.size;
        printf("[%s][%d] addr : 0x%"DTC_MS_PHYADDR_x", size : %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, dWinBufAddr, stbufAttr.size);

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        XC_ApiStatusEx stXC_StatusEx;
        MS_WINDOW_TYPE stWin;
        MS_U16 u16X=0,u16Y=0;
        ST_XC_DIP_WINPROPERTY stDIPWinProperty;
        memcpy(&DwinProperty,&stdwinProperty,sizeof(GOP_DwinProperty));

        memset(&stXC_StatusEx, 0, sizeof(XC_ApiStatusEx));
        stXC_StatusEx.u16ApiStatusEX_Length = sizeof(XC_ApiStatusEx);
        stXC_StatusEx.u32ApiStatusEx_Version = API_STATUS_EX_VERSION;
        MApi_XC_GetStatusEx(&stXC_StatusEx, MAIN_WINDOW);

        u16X = stXC_StatusEx.stCapWin.x;
        u16Y = stXC_StatusEx.stCapWin.y;
        GOP_XC_Create3RDPath(stXC_StatusEx.enInputSourceType);
        GOP_XC_Set3RDWindow(stXC_StatusEx.enInputSourceType,ScanMode,u16X,u16Y,stdwinProperty.u16w,stdwinProperty.u16h,stdwinProperty.u16w,stdwinProperty.u16h);

        MApi_XC_DIP_DisableInputSource(DISABLE,DIP_WINDOW);
        stDIPWinProperty.u8BufCnt = 1;
        stDIPWinProperty.enSource = _DIPDWIN_Mapping(SrcSlt);
        stDIPWinProperty.u32BufStart = stdwinProperty.u32fbaddr0;
        stDIPWinProperty.u32BufEnd = stdwinProperty.u32fbaddr1;
        stDIPWinProperty.u16Width = stdwinProperty.u16w;
        if( (DataFmt == DIP_DATA_FMT_YUV422) || (DataFmt == DIP_DATA_FMT_RGB565))
        {
            stDIPWinProperty.u16Pitch = stdwinProperty.u16fbw*2;
        }
        else if(DataFmt == DIP_DATA_FMT_ARGB8888)
        {
            stDIPWinProperty.u16Pitch = stdwinProperty.u16fbw*4;
        }
        stDIPWinProperty.u16Height = stdwinProperty.u16h;
        MApi_XC_DIP_SetDIPWinProperty(&stDIPWinProperty,DIP_WINDOW);
        MApi_XC_DIP_Ena(TRUE, DIP_WINDOW);
#else
        MApi_GOP_DWIN_SetWinProperty(&stdwinProperty);
        MApi_GOP_DWIN_Enable(TRUE);
#endif
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[1].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return GWIN_FAIL;
        }

        return TRUE;
    }
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
    else if(3 == *pu32Source)
    {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        SrcSlt = DWIN_SRC_MVOP;
        _get_video_info(&u8Interlace, &u16dWinBufWidth, &u16dWinBufHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW);
        MApi_XC_DIP_SetSourceScanType(DIP_SCAN_MODE_PROGRESSIVE, DIP_WINDOW);
#else
        //Source can only from Scaler IP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_MVOP);
        //Is the DWIN source is IP, we need to know the video size and interlace or progressive
        //Notic: Get video from SC0_main. Must set MVOP to SC0_Main
        _get_video_info(&u8Interlace, &u16dWinBufWidth, &u16dWinBufHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW);
        printf("Source is IP, u16dWinBufWidth = %d, u16dWinBufHeight = %d\n", u16dWinBufWidth, u16dWinBufHeight);
        /*          (1)SCAN type: DWIN_SCAN_MODE_PROGRESSIVE
                                 - Contiguous captured/Auto stop captured: In each vsync, capture the top or bottom field
                          (2)SCAN type: DWIN_SCAN_MODE_extern
                                 - Contiguous captured: contiguously captured top and bottom field
                                 - Auto stop captured: Hardware will auto stop once the bottom field is captured.
              */
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
#endif
        /*
                          Interlaced video, 'dwinProperty.u16h' is set to 1/2 of video height
                          If the scan type is DWIN_SCAN_MODE_PROGRESSIVE, the dwin buffer size should be also set to 1/2
                          If the scan type is DWIN_SCAN_MODE_extern, the dwin buffer size should not change
             */
        if(1 == u8Interlace)
        {
            u16dWinBufHeight = u16dWinBufHeight>>1;
        }
    }
    else if(4 == *pu32Source)
    {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        SrcSlt = DWIN_SRC_SUBMVOP;
        _get_video_info(&u8Interlace, &u16dWinBufWidth, &u16dWinBufHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW);
        MApi_XC_DIP_SetSourceScanType(DIP_SCAN_MODE_PROGRESSIVE, DIP_WINDOW);
#else
        //Source can only from Scaler IP
        MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_SUBMVOP);
        //Is the DWIN source is IP, we need to know the video size and interlace or progressive
        //Notic: Get video from SC0_main. Must set MVOP_SUB to SC0_Main
        _get_video_info(&u8Interlace, &u16dWinBufWidth, &u16dWinBufHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW);
        printf("Source is IP, dWinBufWidth = %d, dWinBufHeight = %d\n", u16dWinBufWidth, u16dWinBufHeight);
        /*          (1)SCAN type: DWIN_SCAN_MODE_PROGRESSIVE
                                 - Contiguous captured/Auto stop captured: In each vsync, capture the top or bottom field
                          (2)SCAN type: DWIN_SCAN_MODE_extern
                                 - Contiguous captured: contiguously captured top and bottom field
                                 - Auto stop captured: Hardware will auto stop once the bottom field is captured.
              */
        MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
#endif
        /*
                          Interlaced video, 'dwinProperty.u16h' is set to 1/2 of video height
                          If the scan type is DWIN_SCAN_MODE_PROGRESSIVE, the dwin buffer size should be also set to 1/2
                          If the scan type is DWIN_SCAN_MODE_extern, the dwin buffer size should not change
             */
        if(1 == u8Interlace)
        {
            u16dWinBufHeight = u16dWinBufHeight>>1;
        }
    }
#endif
    else
    {
        printf("This source number is not supported now.\n");
        return FALSE;
    }

    //Data format only YUV; Set data format to YUV422
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
    MApi_XC_DIP_SetOutputDataFmt(DIP_DATA_FMT_YUV422, DIP_WINDOW);
    MApi_XC_DIP_SwapUV(TRUE, DIP_WINDOW);
#else
    MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);
#endif

    if( u8DWinFbId!= 0xFF)
    {
        printf("[%s][%d] Delete FB %d\n", __FUNCTION__, __LINE__, u8DWinFbId);
        MApi_GOP_GWIN_DeleteFB( u8DWinFbId );
        u8DWinFbId = 0xFF;
    }

    if( 0 != _GOP_Create_FB(&u8DWinFbId, u16dWinBufWidth, u16dWinBufHeight, E_MS_FMT_YUV422))
    {
         printf("[%s][%d] Create FB fail\n", __FUNCTION__, __LINE__);
         return FALSE;
    }

    MApi_GOP_GWIN_GetFBInfo(u8DWinFbId, &stbufAttr);
    dWinBufAddr = stbufAttr.addr;
    printf("[%s][%d] captured FB addr : 0x%"DTC_MS_PHYADDR_x"\n", __FUNCTION__, __LINE__, dWinBufAddr);

    // Set DWin property
    stdwinProperty.u16x = 0;
    stdwinProperty.u16y = 0;
    stdwinProperty.u16fbw = u16dWinBufWidth;
    stdwinProperty.u16w = u16dWinBufWidth;
    stdwinProperty.u16h = u16dWinBufHeight;
    stdwinProperty.u32fbaddr0 = dWinBufAddr;
    stdwinProperty.u32fbaddr1 = dWinBufAddr + stbufAttr.size;
    printf("[%s][%d] addr : 0x%"DTC_MS_PHYADDR_x", size : %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, dWinBufAddr, stbufAttr.size);

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
    XC_ApiStatusEx stXC_StatusEx;
    MS_WINDOW_TYPE stWin;
    MS_U16 u16X=0,u16Y=0;
    ST_XC_DIP_WINPROPERTY stDIPWinProperty;
    memcpy(&DwinProperty,&stdwinProperty,sizeof(GOP_DwinProperty));
    if(SrcSlt == DWIN_SRC_OP)
    {
        GOP_XC_Create3RDPath(INPUT_SOURCE_SCALER_OP);
        MApi_XC_SetOutputCapture(ENABLE,E_XC_OP2);
        memset(&stWin, 0, sizeof(MS_WINDOW_TYPE));
        MsOS_DelayTask(100);//Delay for signal stable
        MApi_XC_DIP_GetDEWindow(&stWin,DIP_WINDOW);
        u16X = stWin.x;
        u16Y = stWin.y;
        GOP_XC_Set3RDWindow(INPUT_SOURCE_SCALER_OP,ScanMode,u16X,u16Y,stdwinProperty.u16w,stdwinProperty.u16h,stdwinProperty.u16w,stdwinProperty.u16h);
    }
    else// IP
    {
        memset(&stXC_StatusEx, 0, sizeof(XC_ApiStatusEx));
        stXC_StatusEx.u16ApiStatusEX_Length = sizeof(XC_ApiStatusEx);
        stXC_StatusEx.u32ApiStatusEx_Version = API_STATUS_EX_VERSION;
        MApi_XC_GetStatusEx(&stXC_StatusEx, MAIN_WINDOW);

        u16X = stXC_StatusEx.stCapWin.x;
        u16Y = stXC_StatusEx.stCapWin.y;
        GOP_XC_Create3RDPath(stXC_StatusEx.enInputSourceType);
        GOP_XC_Set3RDWindow(stXC_StatusEx.enInputSourceType,ScanMode,u16X,u16Y,stdwinProperty.u16w,stdwinProperty.u16h,stdwinProperty.u16w,stdwinProperty.u16h);
    }
    MApi_XC_DIP_DisableInputSource(DISABLE,DIP_WINDOW);
    stDIPWinProperty.u8BufCnt = 1;
    stDIPWinProperty.enSource = _DIPDWIN_Mapping(SrcSlt);
    stDIPWinProperty.u32BufStart = stdwinProperty.u32fbaddr0;
    stDIPWinProperty.u32BufEnd = stdwinProperty.u32fbaddr1;
    stDIPWinProperty.u16Width = stdwinProperty.u16w;
    if( (DataFmt == DIP_DATA_FMT_YUV422) || (DataFmt == DIP_DATA_FMT_RGB565))
    {
        stDIPWinProperty.u16Pitch = stdwinProperty.u16fbw*2;
    }
    else if(DataFmt == DIP_DATA_FMT_ARGB8888)
    {
        stDIPWinProperty.u16Pitch = stdwinProperty.u16fbw*4;
    }
        stDIPWinProperty.u16Height = stdwinProperty.u16h;
    MApi_XC_DIP_SetDIPWinProperty(&stDIPWinProperty,DIP_WINDOW);
#else
    MApi_GOP_DWIN_SetWinProperty(&stdwinProperty);
#endif

    //Enable Dwin to capture each frame if CaptureOneFrame is 0
    if( !(*pu32CaptureOneFrame) )
    {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
        MApi_XC_DIP_Ena(TRUE, DIP_WINDOW);
#else
        MApi_GOP_DWIN_Enable(TRUE);
#endif
    }

    //Create GOP double buffer to avoid blink
    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[u8Count].u8FBId, &gwinattr);
        u32FBAddr0[u8Count] = gwinattr.addr;
        #if ENABLE_MIU_1
        if(u32FBAddr0[u8Count] > MIU_INTERVAL)
        {
            u8MIUSELAddr0 = E_GOP_SEL_MIU1;
        }
        #endif
        if(u32DispCount > 1 || u32DispCount == 0)
        {
            MS_U32 u32wordunit = 0;
            MApi_GOP_GetChipCaps(E_GOP_CAP_WORD_UNIT,&u32wordunit,sizeof(MS_U32));
            dualgopbuf[u8Count] = MsOS_AllocateMemory(_CalcPitch(gWinInfo[u8Count].u16GeDstFmt,gWinInfo[u8Count].u16GopBitmapWidth)*gWinInfo[u8Count].u16GopBitmapHeight + u32wordunit,s32MstarNonCachedPoolID);

            if(dualgopbuf[u8Count] == NULL)
            {
                printf("[%s][%d] Create Temp FB fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }

            //Adjust address alignment to avoid graph shift
            u32FBAddr1[u8Count] = (MsOS_VA2PA((MS_U32)dualgopbuf[u8Count]) + u32wordunit ) &(~(u32wordunit-1));//0x80000000 +
            #if ENABLE_MIU_1
            if(u32FBAddr1[u8Count] > MIU_INTERVAL)
            {
                u8MIUSELAddr1 = E_GOP_SEL_MIU1;
            }
            #endif
       }
    }

    do{
        //Auto-stop capture after one frame has been successfully captured if CaptureOneFrame is 1
        if( *pu32CaptureOneFrame )
        {
#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
            MApi_XC_DIP_CapOneFrame(DIP_WINDOW);
            MsOS_ReadMemory();
#else
            MApi_GOP_DWIN_CaptureOneFrame();
#endif
        }
        else
        {
            MsOS_DelayTask(20); //delay for dwin capture ready
        }

        //GE bitblt to copy DWin captured frame to GWin buffer to display
        for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
        {
            //Set Clip window for rendering
            stgfxPt0[u8Count].x = 0;
            stgfxPt0[u8Count].y = 0;
            stgfxPt1[u8Count].x = gWinInfo[u8Count].u16GopBitmapWidth;
            stgfxPt1[u8Count].y = gWinInfo[u8Count].u16GopBitmapHeight;
            MApi_GFX_SetClip(&stgfxPt0[u8Count], &stgfxPt1[u8Count]);
            MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

            //set dst buffer
            if(1 == (u32DispCount % 2))
            {
                MApi_GOP_GWIN_Switch2FB( gWinInfo[u8Count].u8FBId );
            }
            else
            {
                stdstbuf[u8Count].u32ColorFmt = gWinInfo[u8Count].u16GeDstFmt;
                stdstbuf[u8Count].u32Addr = u32FBAddr1[u8Count];
                stdstbuf[u8Count].u32Pitch = _CalcPitch(gWinInfo[u8Count].u16GeDstFmt, gWinInfo[u8Count].u16GopBitmapWidth);
                MApi_GFX_SetDstBufferInfo(&stdstbuf[u8Count], 0);
            }

            // Bitble the bitmap to DRAM of GE
            stsrcbuf[u8Count].u32ColorFmt = E_MS_FMT_YUV422;
            stsrcbuf[u8Count].u32Addr = dWinBufAddr;
            printf("[%s][%d] Src buffer addr = 0x%"DTC_MS_PHYADDR_x"\n", __FUNCTION__, __LINE__, dWinBufAddr);
            stsrcbuf[u8Count].u32Width = u16dWinBufWidth;
            stsrcbuf[u8Count].u32Height = u16dWinBufHeight;
            stsrcbuf[u8Count].u32Pitch = _CalcPitch(E_MS_FMT_YUV422, u16dWinBufWidth);
            MApi_GFX_SetSrcBufferInfo(&stsrcbuf[u8Count], 0);

            stbitbltInfo[u8Count].srcblk.x = 0;
            stbitbltInfo[u8Count].srcblk.y = 0;
            stbitbltInfo[u8Count].srcblk.width = u16dWinBufWidth;
            stbitbltInfo[u8Count].srcblk.height = u16dWinBufHeight;

            stbitbltInfo[u8Count].dstblk.x = 0;
            stbitbltInfo[u8Count].dstblk.y = 0;
            stbitbltInfo[u8Count].dstblk.width = gWinInfo[u8Count].u16GopBitmapWidth;
            stbitbltInfo[u8Count].dstblk.height = gWinInfo[u8Count].u16GopBitmapHeight;

            MApi_GFX_BitBlt(&stbitbltInfo[u8Count], u32Flag);
            MApi_GFX_FlushQueue();

            //After GE bitblt then enable the GWin
            MApi_GOP_GWIN_SwitchGOP(gWinInfo[u8Count].u8GOPId);
            MApi_GOP_GWIN_UpdateRegOnce(TRUE);
            if(1 == (u32DispCount % 2))
            {
                GOP_GwinInfo stGWin;
                MApi_GOP_GWIN_GetWinInfo(gWinInfo[u8Count].u8GeWinId,&stGWin);
                stGWin.u32DRAMRBlkStart = u32FBAddr0[u8Count];
                MApi_GOP_GWIN_SetWinInfo(gWinInfo[u8Count].u8GeWinId,&stGWin);
                MApi_GOP_MIUSel(MApi_GOP_GWIN_GetCurrentGOP(),u8MIUSELAddr0);
            }
            else
            {
                GOP_GwinInfo stGWin;
                MApi_GOP_GWIN_GetWinInfo(gWinInfo[u8Count].u8GeWinId,&stGWin);
                stGWin.u32DRAMRBlkStart = u32FBAddr1[u8Count];
                MApi_GOP_GWIN_SetWinInfo(gWinInfo[u8Count].u8GeWinId,&stGWin);
                MApi_GOP_MIUSel(MApi_GOP_GWIN_GetCurrentGOP(),u8MIUSELAddr1);
            }

            if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
            {
                 printf("[%s][%d] MApi_GOP_GWIN_Enable failed: GOP%d\n", __FUNCTION__, __LINE__, gWinInfo[u8Count].u8GeWinId);
                 return GWIN_FAIL;
            }
            MApi_GOP_GWIN_UpdateRegOnce(FALSE);
        }

    }while( --u32DispCount > 0 );

#if (DDI_DEMO_DIP_CAPTURE_TEST == 1)
    MApi_XC_DIP_ReleaseResource(DIP_WINDOW);
#endif

    //Before leaving this function, FB has been created must delete
    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if(dualgopbuf[u8Count] != NULL)
            MsOS_FreeMemory(dualgopbuf[u8Count],s32MstarNonCachedPoolID);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use DWin pingpong with GOP
/// @param[in] pingpong buffer count.
/// @return None
/// @note
/// Command: \b OSD_DWinPingPong \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DWinPingPong(MS_U32 *pu32BufferCount)
{
    MS_PHYADDR dWinBufAddr;
    GOP_GwinFBAttr stbufAttr;
    GOP_DwinProperty stdwinProperty;
    //Dwin buffer size set to panel width and height to capture the whole frame
    MS_U16 u16dWinBufWidth = IPANEL(&gDevId, Width);
    MS_U16 u16dWinBufHeight = IPANEL(&gDevId, Height);
    //Source can only from Scaler OP
    MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);

    //OP scan type is progressive
    MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);

    u16dWinBufWidth = gWinInfo[0].u16GopBitmapWidth;
    u16dWinBufHeight = gWinInfo[0].u16GopBitmapHeight;
    printf("Source is OP, u16dWinBufWidth = %d, u16dWinBufHeight = %d\n", u16dWinBufWidth, u16dWinBufHeight);

    //Data format only YUV; Set data format to YUV422
    switch(gWinInfo[0].u16GeDstFmt)
    {
       case E_MS_FMT_ARGB8888:
           MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_ARGB8888);
           MApi_GOP_DWIN_SetAlphaValue(0xFF);
           MApi_GOP_GWIN_SwitchGOP(u32GOP0);
           MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);
           break;
       case E_MS_FMT_RGB565:
           MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_RGB565);
           MApi_GOP_GWIN_SwitchGOP(u32GOP0);
           MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);
           break;
       default:
           printf("[%s][%d] Not supported DwinData Type\n",__FUNCTION__,__LINE__);
           return FALSE;
    }

    if( u8DWinFbId!= 0xFF)
    {
        printf("[%s][%d] Delete FB %d\n", __FUNCTION__, __LINE__, u8DWinFbId);
        MApi_GOP_GWIN_DeleteFB( u8DWinFbId );
        u8DWinFbId = 0xFF;
    }

    if( 0 != _GOP_Create_FB(&u8DWinFbId, u16dWinBufWidth, (u16dWinBufHeight*(*pu32BufferCount)), gWinInfo[0].u16GeDstFmt))
    {
         printf("[%s][%d] Create FB fail\n", __FUNCTION__, __LINE__);
         return FALSE;
    }

    MApi_GOP_GWIN_GetFBInfo(u8DWinFbId, &stbufAttr);
    dWinBufAddr = stbufAttr.addr;
    printf("[%s][%d] captured FB addr : 0x%"DTC_MS_PHYADDR_x"\n", __FUNCTION__, __LINE__, dWinBufAddr);

    // Set DWin property
    stdwinProperty.u16x = 0;
    stdwinProperty.u16y = 0;
    stdwinProperty.u16fbw = u16dWinBufWidth;
    stdwinProperty.u16w = u16dWinBufWidth;
    stdwinProperty.u16h = u16dWinBufHeight;
    stdwinProperty.u32fbaddr0 = dWinBufAddr;
    stdwinProperty.u32fbaddr1 = dWinBufAddr + stbufAttr.size;
    printf("[%s][%d] addr : 0x%"DTC_MS_PHYADDR_x", size : %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, dWinBufAddr, stbufAttr.size);

    MApi_GOP_DWIN_SetWinProperty(&stdwinProperty);
    MApi_GOP_DWIN_Enable(TRUE);
    MApi_GOP_SetPINPON(gWinInfo[0].u8GeWinId ,TRUE, GOP_PINPON_DWIN);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use DWin skip
/// @param[in] skip frame count.
/// @return None
/// @note
/// Command: \b OSD_DWinSkip \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DWinSkipFrame(MS_U32 *pu32FrameCount)
{
    MApi_GOP_DWIN_SetSkipFrame(*pu32FrameCount);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable(visable) GWin
/// @param[in] Enable enable or disable the GWin
/// @return None
/// @note
/// Command: \b OSD_EnableWin \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_EnableWin(MS_U32 *pu32Enable)
{
    MS_U8 u8Count;

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if (*pu32Enable)
        {
            MApi_GOP_GWIN_Enable( gWinInfo[u8Count].u8GeWinId, TRUE);
        }
        else
        {
            MApi_GOP_GWIN_Enable( gWinInfo[u8Count].u8GeWinId, FALSE);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to config global Alpha for a GWin
/// @param[in] Auto automatic to show the global alpha demo
/// @param[in] AlphaValue alpha value
/// @return None
/// @note
/// Command: \b OSD_SetGlobalAlpha \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_SetGlobalAlpha(MS_U32 *pu32Auto, MS_U32 *pu32AlphaValue)
{
    MS_U32 u32index;
    MS_U8 u8Count;

    if (*pu32Auto)
    {
        // - Automatic show the effect of global alpha
        for ( u32index = 0 ; u32index < 255 ; u32index += 32)
        {
            for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
            {
                MApi_GOP_GWIN_SetBlending(gWinInfo[u8Count].u8GeWinId, FALSE, u32index); //set transparent color
            }
            MsOS_DelayTask(u32delay_time);
        }
        for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[u8Count].u8GeWinId, FALSE, 0xff);
        }
    }
    else
    {
        // - Assign the global alpha value
        //Alpha Range 0 ~ 255
        for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[u8Count].u8GeWinId, FALSE, *pu32AlphaValue);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable individual alpha value of an image in a GWin
/// @return None
/// @note
/// Command: \b OSD_EnablePerPixelAlpha \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_EnablePerPixelAlpha(MS_U32 *pu32Auto, MS_U32 *pu32AlphaValue)
{
    MS_U32 u32index;
    MS_U8 u8Count;

    if (*pu32Auto)
    {
        // - Automatic show the effect of global alpha
        for ( u32index = 0 ; u32index < 255 ; u32index += 32)
        {
            for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
            {
                MApi_GOP_GWIN_SetBlending(gWinInfo[u8Count].u8GeWinId, TRUE, u32index); //set transparent color
            }
            MsOS_DelayTask(u32delay_time);
        }
        for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[u8Count].u8GeWinId, TRUE, 0xff);
        }
    }
    else
    {
        // - Assign the global alpha value
        //Alpha Range 0 ~ 255
        for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
        {
            MApi_GOP_GWIN_SetBlending(gWinInfo[u8Count].u8GeWinId, TRUE, *pu32AlphaValue);
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show the fade effect for a GWin
/// @param[in] FadeType Fade type
/// @param[in] bEnable Enable the fade effect
/// @param[in] Rate Fade rate
/// @return None
/// @note
/// Command: \b OSD_SetFade \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_SetFade(MS_U32 *pu32FadeType, MS_U32 *pu32Enable, MS_U32 *pu32Rate)
{
    EN_GOP_FADE_TYPE eFADE_Type = (EN_GOP_FADE_TYPE)*pu32FadeType;
    MS_U8 u8Count;

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        MApi_GOP_GWIN_SetFadeInOut(gWinInfo[u8Count].u8GeWinId, eFADE_Type, *pu32Enable, *pu32Rate);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to fill rectangle in the image buffer for a GWin
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] index The color index
/// @param[in] RectX The x start point
/// @param[in] RectY The y start point
/// @param[in] RectWidth The filling width
/// @param[in] RectHeight The filling height
/// @return None
/// @note
/// Command: \b OSD_FillBlock \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_FillBlock(MS_U32 *pu32Target, MS_U32 *pu32index, MS_U32 *pu32RectX, MS_U32 *pu32RectY, MS_U32 *pu32RectWidth, MS_U32 *pu32RectHeight)
{
    GFX_RectFillInfo stgfxFillBlock;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    GFX_BufferInfo stgfxDstBuf;
    GOP_GwinFBAttr stDstFBInfo;
    GFX_RgbColor stcolor_s;
    GFX_RgbColor stcolor_e;
    MS_U8 u8DstFBfmt;
    MS_U32 u32Flag;
    MS_U8 u8Count = 0;

    //Map color index to ARGB color
    _ColorMapping(*pu32index, &stcolor_s);
    _ColorMapping(*pu32index, &stcolor_e);

    //get current GWIN fbInfo.
    //MApi_GOP_GWIN_GetFBInfo(MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &DstFBInfo);
    MApi_GOP_GWIN_GetFBInfo(gWinInfo[*pu32Target].u8FBId, &stDstFBInfo);

    u8DstFBfmt = (MS_U8)stDstFBInfo.fbFmt;

    //Set Dst buffer
    stgfxDstBuf.u32ColorFmt = u8DstFBfmt;
    stgfxDstBuf.u32Addr = stDstFBInfo.addr;
    stgfxDstBuf.u32Pitch = stDstFBInfo.pitch;
    MApi_GFX_SetDstBufferInfo(&stgfxDstBuf, 0);
    printf("[%s][%d] Dst buffer addr = 0x%"DTC_MS_PHY_x"\n", __FUNCTION__, __LINE__,stDstFBInfo.addr);
    printf("[%s][%d] Dst buffer size = 0x%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__,stDstFBInfo.size);

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    //Set Clip window for rendering
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    u32Flag = GFXRECT_FLAG_COLOR_CONSTANT;

    _SetupGfxRectFillInfo(&stgfxFillBlock, *pu32RectX, *pu32RectY, *pu32RectWidth, *pu32RectHeight,
                                u8DstFBfmt, stcolor_s, stcolor_e, u32Flag);
    MApi_GFX_RectFill(&stgfxFillBlock);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw a line in the image buffer for a GWin
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] index The color index
/// @param[in] X1 The X1 coordinate
/// @param[in] Y1 The Y1 coordinate
/// @param[in] X2 The X2 coordinate
/// @param[in] Y2 The Y2 coordinate
/// @return None
/// @note
/// Command: \b OSD_DrawLine \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawLine(MS_U32 *pu32Target, MS_U32 *pu32index, MS_U32 *pu32X1, MS_U32 *pu32Y1, MS_U32 *pu32X2, MS_U32 *pu32Y2)
{
    GFX_DrawLineInfo stgfxDrawLineInfo;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    GFX_BufferInfo stgfxDstBuf;
    GOP_GwinFBAttr stDstFBInfo;
    GFX_RgbColor stcolor_s;
    GFX_RgbColor stcolor_e;
    MS_U8 u8DstFBfmt;
    MS_U32 u32width; //line width
    MS_U32 u32Flag;
    MS_U8 u8Count = 0;

    //Map color index to ARGB color
    _ColorMapping(*pu32index, &stcolor_s);
    _ColorMapping(*pu32index, &stcolor_e);

    //get current GWIN fbInfo.
    //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &DstFBInfo);
    MApi_GOP_GWIN_GetFBInfo(gWinInfo[*pu32Target].u8FBId, &stDstFBInfo);

    u8DstFBfmt = (MS_U8)stDstFBInfo.fbFmt;

    //Set Dst buffer
    stgfxDstBuf.u32ColorFmt = u8DstFBfmt;
    stgfxDstBuf.u32Addr = stDstFBInfo.addr;
    stgfxDstBuf.u32Pitch = stDstFBInfo.pitch;
    MApi_GFX_SetDstBufferInfo(&stgfxDstBuf, 0);
    printf("Dst buffer addr = 0x%"DTC_MS_PHY_x"\n", stDstFBInfo.addr);

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    //Set Clip window for rendering
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    // Draw Line parameters
    u32width = 5;
    u32Flag = GFXLINE_FLAG_COLOR_CONSTANT;

    MApi_GFX_Set_Line_Pattern(false, 0xAA, 2);

    _SetupGfxDrawLineInfo(&stgfxDrawLineInfo, pu32X1, pu32Y1, pu32X2, pu32Y2,
                                    u8DstFBfmt, stcolor_s, stcolor_e, u32width, u32Flag);

    if( GFX_SUCCESS != MApi_GFX_DrawLine(&stgfxDrawLineInfo))
    {
        printf("MApi_GFX_DrawLine fail\n");
        return false;
    }
    MApi_GFX_FlushQueue();

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw bitmap or fill rectangle in the image buffer for this GWin
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] Index The bitmap index
/// @param[in] X The x start point
/// @param[in] Y The y start point
/// @param[in] width The drawing width
/// @param[in] height The drawing height
/// @return None
/// @note
/// Command: \b OSD_DrawBmpXY \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawBmpXY(MS_U32 *pu32Target, MS_U32 *pu32Index, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32width, MS_U32 *pu32height)
{
    GFX_DrawBmpInfo stgfxBmpFmt;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count = 0;

    /*load palette to GE if source format is index mode
     I8 mode -> RGB/YUV (O)*/
    if(bmpinfo[*pu32Index].BmpClrFmt == GFX_FMT_I8)
    {
        _SetGfxPalette();
    }

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //Switch destination GWIn to GeWinId
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[*pu32Target].u8FBId);

    stgfxBmpFmt.bScale = true;
    stgfxBmpFmt.x = *pu32X;
    stgfxBmpFmt.y = *pu32Y;
    stgfxBmpFmt.width = *pu32width;
    stgfxBmpFmt.height = *pu32height;
    MApi_GFX_DrawBitmap(bmpinfo[*pu32Index].hBmp, &stgfxBmpFmt);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw a bitmap (400x400)with rotation and mirror option
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] X The X coordinate
/// @param[in] Y The Y coordinate
/// @param[in] RotateIndex The rotation type
/// @param[in] MirrorX Enable mirror by X-axis
/// @param[in] MirrorY Enable mirror by Y-axis
/// @return None
/// @note
/// Command: \b OSD_DrawBmpRotate \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawBmpRotate(MS_U32 *pu32Target, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32RotateIndex, MS_U32 *pu32MirrorX, MS_U32 *pu32MirrorY)
{
    GFX_DrawBmpInfo stgfxBmpFmt;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count = 0;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[*pu32Target].u8FBId);

    stgfxBmpFmt.bScale = FALSE;
    stgfxBmpFmt.x = *pu32X;
    stgfxBmpFmt.y = *pu32Y;
    stgfxBmpFmt.width = 400;
    stgfxBmpFmt.height = 400;
    MApi_GFX_SetMirror(*pu32MirrorX, *pu32MirrorY);// ymirror is not work???
    MApi_GFX_SetRotate(*pu32RotateIndex);
    MApi_GFX_DrawBitmap(hBmpARGB4444, &stgfxBmpFmt);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to draw a bitmap (400x400)with scaling option.
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] X The X coordinate
/// @param[in] Y The Y coordinate
/// @param[in] ScaleX Scaling parameter (X-axis)
/// @param[in] ScaleY Scaling parameter (Y-axis)
/// @return None
/// @note
/// Command: \b OSD_DrawBmpScale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawBmpScale(MS_U32 *pu32Target, MS_U32 *pu32X, MS_U32 *pu32Y, MS_U32 *pu32ScaleX, MS_U32 *pu32ScaleY)
{
    GFX_DrawBmpInfo stgfxBmpFmt;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count = 0;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    if (*pu32ScaleX > 3) *pu32ScaleX = 3;
    if (*pu32ScaleY > 3) *pu32ScaleY = 3;

    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[*pu32Target].u8FBId);

    stgfxBmpFmt.bScale = TRUE;
    stgfxBmpFmt.x = *pu32X;
    stgfxBmpFmt.y = *pu32Y;
    stgfxBmpFmt.width = 400*(*pu32ScaleX);
    stgfxBmpFmt.height = 400*(*pu32ScaleY);

    MApi_GFX_DrawBitmap(hBmpARGB4444, &stgfxBmpFmt);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to convert a RGB565 color format to ARGB4444. We use GWIN 0
//  to see the result
/// @return None
/// @note
/// Command: \b OSD_ConvertColor \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_ConvertColor(void)
{
    GFX_DrawBmpInfo stgfxBmpFmt;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count =0;

    for( u8Count = 0; u8Count < u8DrawGOPCount; u8Count++ )
    {
        stgfxPt0.x = 0;
        stgfxPt0.y = 0;
        stgfxPt1.x = gWinInfo[u8Count].u16GopBitmapWidth;
        stgfxPt1.y = gWinInfo[u8Count].u16GopBitmapHeight;
        MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
        MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

        //Destination Buffer
        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[u8Count].u8FBId);

        stgfxBmpFmt.bScale = FALSE;
        stgfxBmpFmt.x = 0;
        stgfxBmpFmt.y = 0;
        stgfxBmpFmt.width = 400;
        stgfxBmpFmt.height = 400;

        //Convert RGB565 to a ARGB4444 buffer
        MApi_GFX_DrawBitmap(hBmpRGB565, &stgfxBmpFmt);
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return GWIN_FAIL;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to copy a image (180x288) from screen to screen
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] RopIndex The ROP option
/// @param[in] SrcColorIndex The source rectangle color index
/// @param[in] DstColorIndex The destination X coordinate
/// @return None
/// @note
/// Command: \b OSD_ScreenCopyRop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_ScreenCopyRop(MS_U32 *pu32Target, MS_U32 *pu32RopIndex, MS_U32 *pu32SrcColorIndex, MS_U32 *pu32DstColorIndex)
{
    GFX_DrawRect stgfxDrawRect;
    GFX_RectFillInfo stgfxFillBlock;
    GOP_GwinFBAttr stSrcFBInfo;
    GFX_BufferInfo stsrcbuf;
    GFX_RgbColor stcolor_s;
    GFX_RgbColor stcolor_e;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count = 0;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //Destination Buffer
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[*pu32Target].u8FBId);

    //Set Src buffer
    //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &SrcFBInfo);
    MApi_GOP_GWIN_GetFBInfo(gWinInfo[*pu32Target].u8FBId, &stSrcFBInfo);

    stsrcbuf.u32ColorFmt = u16gwin_fmt;
    stsrcbuf.u32Addr = stSrcFBInfo.addr;
    stsrcbuf.u32Width = stSrcFBInfo.width;
    stsrcbuf.u32Height = stSrcFBInfo.height;
    stsrcbuf.u32Pitch = stSrcFBInfo.pitch;
    MApi_GFX_SetSrcBufferInfo(&stsrcbuf, 0);

    _ColorMapping(*pu32SrcColorIndex, &stcolor_s);
    _ColorMapping(*pu32SrcColorIndex, &stcolor_e);

    //Fill source rectangle with color SrcColorIndex
    _SetupGfxRectFillInfo(&stgfxFillBlock, 0, 0, 360, 576, u16gwin_fmt, stcolor_s, stcolor_e, GFXRECT_FLAG_COLOR_CONSTANT);
    MApi_GFX_RectFill(&stgfxFillBlock);

    _ColorMapping(*pu32DstColorIndex, &stcolor_s);
    _ColorMapping(*pu32DstColorIndex, &stcolor_e);

    //Fill destination rectangle with color DstColorIndex
    _SetupGfxRectFillInfo(&stgfxFillBlock, 360, 0, 360, 576, u16gwin_fmt, stcolor_s, stcolor_e, GFXRECT_FLAG_COLOR_CONSTANT);
    MApi_GFX_RectFill(&stgfxFillBlock);

    MsOS_DelayTask(1000);

    MApi_GFX_EnableAlphaBlending(FALSE);
    MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);

    //run ROP
    _ROPMapping(*pu32RopIndex, &stgfxDrawRect);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code BitBlt a bitmap in GE Buffer to another GE Buffer
///        GE buffer could be different or the same
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] SrcX The X coordinate in source GE buffer. Range {0..1280} in this sample code
/// @param[in] SrcY The Y coordinate in source GE buffer. Range {0..720} in this sample code
/// @param[in] DstX The X coordinate in destination GE buffer. Range {0..1280} in this sample code
/// @param[in] DstY The Y coordinate in destination GE buffer. Range {0..720} in this sample code
/// @param[in] Width The copy width
/// @param[in] Height The copy height
/// @return None
/// @note
/// Command: \b OSD_BitBltSample \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_BitBltSample(MS_U32 *pu32Target, MS_U32 *pu32SrcX, MS_U32 *pu32SrcY, MS_U32 *pu32DstX, MS_U32 *pu32DstY, MS_U32 *pu32Width, MS_U32 *pu32Height)
{
    GFX_DrawRect stgfxDrawRect;
    GOP_GwinFBAttr stSrcFBInfo;
    GFX_BufferInfo stsrcbuf;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count = 0;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //Destination Buffer
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[*pu32Target].u8FBId);

    //Set Src buffer
    //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &SrcFBInfo);
    MApi_GOP_GWIN_GetFBInfo(gWinInfo[*pu32Target].u8FBId, &stSrcFBInfo);

    stsrcbuf.u32ColorFmt = u16gwin_fmt;
    stsrcbuf.u32Addr = stSrcFBInfo.addr;
    stsrcbuf.u32Width = stSrcFBInfo.width;
    stsrcbuf.u32Height = stSrcFBInfo.height;
    stsrcbuf.u32Pitch = stSrcFBInfo.pitch;
    MApi_GFX_SetSrcBufferInfo(&stsrcbuf, 0);

    ///Config the src and dst clip in GE buffer
    printf("width = %"DTC_MS_U32_u", height = %"DTC_MS_U32_u"\n", *pu32Width, *pu32Height);
    _SetupGfxDrawRect(&stgfxDrawRect, *pu32SrcX, *pu32SrcY, *pu32Width, *pu32Height, *pu32DstX, *pu32DstY, *pu32Width, *pu32Height);

    /// BitBlt
    MApi_GFX_BitBlt(&stgfxDrawRect, GFXDRAW_FLAG_DEFAULT);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to output bitmap font by GE
/// @param[in] index The target index: 0 for HD, 1 for SD
/// @param[in] FontIndex The font index in the sample code. We have a bitmap font data in the HEX
///            array for this sample code
/// @param[in] ColorIndex The color index
/// @param[in] FlagIndex The showing flag
/// @param[in] X The X coordinate to output font
/// @param[in] Y The Y coordinate to output font
/// @return None
/// @note
/// Command: \b OSD_TextOut \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_TextOut(MS_U32 *pu32Target, MS_U32 *pu32FontIndex, MS_U32 *pu32ColorIndex, MS_U32 *pu32FlagIndex, MS_U32 *pu32X, MS_U32 *pu32Y)
{
    MS_U8 u8Font[16];
    MS_U16 u16FontIdx;
    GFX_TextOutInfo stgfxTextOutInfo;
    GFX_RgbColor stcolor;
    MS_U32 u32Flag = GFXFONT_FLAG_DEFAULT;
    FONTHANDLE fonthandle;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U8 u8Count = 0;

    printf("\n GE_TextOut: FontIdx=%"DTC_MS_U32_u" ColorIndex=%"DTC_MS_U32_u" FlagIndex=%"DTC_MS_U32_u" X=%"DTC_MS_U32_u" Y=%"DTC_MS_U32_u"\n", *pu32FontIndex,*pu32ColorIndex,*pu32FlagIndex,*pu32X,*pu32Y);

    switch(*pu32FontIndex)
    {
        case 0:
            fonthandle = hFontI1;
            break;
        case 1:
            fonthandle = hFontI2;
            break;
        case 2:
            fonthandle = hFontI4;
            break;
        default:
            fonthandle = hFontI1;
            break;
    }

    // GEFONT_FLAG_DEFAULT: Default font format
    // GEFONT_FLAG_SCALE: scalable font
    // GEFONT_FLAG_COMPACT: Compact font, controled by "dis"
    // GEFONT_FLAG_GAP: Gap font, controled by "dis"
    if( (*pu32FlagIndex) & 0x1 )
        u32Flag |= GFXFONT_FLAG_SCALE;
    if( (*pu32FlagIndex) & 0x2 )
        u32Flag |= GFXFONT_FLAG_GAP;
    else if( (*pu32FlagIndex) & 0x4 )
        u32Flag |= GFXFONT_FLAG_COMPACT;

    _ColorMapping(*pu32ColorIndex, &stcolor);

    //Generate a 2-byte sting in the sting array. The sting is 123456789ABCDEF in the sample code font system
    for( u16FontIdx = 0; u16FontIdx < 15; u16FontIdx++ )
    {
        u8Font[u16FontIdx] = u16FontIdx+1;
    }
    u8Font[u16FontIdx] = 0x0;   //stop draw command, should set it to "0"
    printf("\n Test Font: FontIdx=%"DTC_MS_U32_u" \n", *pu32FontIndex);

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[*pu32Target].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[*pu32Target].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);

    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[*pu32Target].u8FBId);

    _SetupGfxTextOutInfo(&stgfxTextOutInfo, *pu32X, *pu32Y, u16FontWidth, u16FontHeight, u16FontDis, stcolor, u32Flag);

    printf("\n Test Font: FontIdx=%"DTC_MS_U32_u", Handle=%d, x=%d, y=%d \n", *pu32FontIndex, fonthandle , stgfxTextOutInfo.dstblk.x, stgfxTextOutInfo.dstblk.y);

    MApi_GFX_TextOut( fonthandle , u8Font, 1, &stgfxTextOutInfo);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

static MS_BOOL _GE_PrintFormat(MS_U16 pu16pfmt)
{
    switch(pu16pfmt)
    {
        case E_MS_FMT_I1:
            printf("Color Format is E_MS_FMT_I1.\n");
            break;
        case E_MS_FMT_I2:
            printf("Color Format is E_MS_FMT_I2.\n");
            break;
        case E_MS_FMT_I4:
            printf("Color Format is E_MS_FMT_I4.\n");
            break;
        case E_MS_FMT_I8:
            printf("Color Format is E_MS_FMT_I8.\n");
            break;
        case E_MS_FMT_FaBaFgBg2266:
            printf("Color Format is E_MS_FMT_FaBaFgBg2266.\n");
            break;
        case E_MS_FMT_1ABFgBg12355:
            printf("Color Format is E_MS_FMT_1ABFgBg12355.\n");
            break;
        case E_MS_FMT_RGB565:
            printf("Color Format is E_MS_FMT_RGB565.\n");
            break;
        case E_MS_FMT_ARGB1555:
            printf("Color Format is E_MS_FMT_ARGB1555.\n");
            break;
        case E_MS_FMT_ARGB4444:
            printf("Color Format is E_MS_FMT_ARGB4444.\n");
            break;
        case E_MS_FMT_ARGB1555_DST:
            printf("Color Format is E_MS_FMT_ARGB1555_DST.\n");
            break;
        case E_MS_FMT_YUV422:
            printf("Color Format is E_MS_FMT_YUV422.\n");
            break;
        case E_MS_FMT_ARGB8888:
            printf("Color Format is E_MS_FMT_ARGB8888.\n");
            break;
        case E_MS_FMT_RGBA5551:
            printf("Color Format is E_MS_FMT_RGBA5551.\n");
            break;
        case E_MS_FMT_RGBA4444:
            printf("Color Format is E_MS_FMT_RGBA4444.\n");
            break;
        case E_MS_FMT_BGRA5551:
            printf("Color Format is E_MS_FMT_BGRA5551.\n");
            break;
        case E_MS_FMT_ABGR1555:
            printf("Color Format is E_MS_FMT_ABGR1555.\n");
            break;
        case E_MS_FMT_ABGR4444:
            printf("Color Format is E_MS_FMT_ABGR4444.\n");
            break;
        case E_MS_FMT_BGRA4444:
            printf("Color Format is E_MS_FMT_BGRA4444.\n");
            break;
        case E_MS_FMT_BGR565:
            printf("Color Format is E_MS_FMT_BGR565.\n");
            break;
        case E_MS_FMT_RGBA8888:
            printf("Color Format is E_MS_FMT_RGBA8888.\n");
            break;
        case E_MS_FMT_BGRA8888:
            printf("Color Format is E_MS_FMT_BGRA8888.\n");
            break;
        case E_MS_FMT_ABGR8888:
            printf("Color Format is E_MS_FMT_ABGR8888.\n");
            break;
        case E_MS_FMT_AYUV8888:
            printf("Color Format is E_MS_FMT_AYUV8888.\n");
            break;
        default:
            printf("Unknown Type.\n");
            return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code change GE destination color format and gop GWIN color format
///        This function must be call before GE_CreateWin. If not, default color format is E_MS_FMT_ARGB4444
/// @param[in] pfmt is color format defined in MsTypes.h, enum MS_ColorFormat
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b OSD_SetGWinFmt \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_SetGWinFmt(MS_U16 *pu16pfmt)
{
    if(E_MS_FMT_AYUV8888 < *pu16pfmt)
    {
        printf("Color format out of range!\n");
        return false;
    }
    if(TRUE == _GE_PrintFormat(*pu16pfmt))
    {
        u16gwin_fmt=*pu16pfmt;
        bSetClrFmt = FALSE;//Do not set color format again when OSD_createwin
        return true;
    }
    else
    {
        printf("Color format does not support!\n");
        return FALSE;
    }
}
#if (DEMO_GE_TWOSOURCEBITBLT_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to bitblt by two source buffer in this GWin
/// @return None
/// @note
/// Command: \b OSD_TwoSourceBitblt \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_TwoSourceBitblt(void)
{
    GFX_DrawRect stgfxDrawRect;
    GOP_GwinFBAttr stSrcFBInfo;
    GFX_BufferInfo stsrcbuf;
    GFX_DrawBmpInfo stgfxBmpFmt;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    MS_U16 u16twoSourceWidth = 400;
    MS_U16 u16twoSourceHeight = 400;
    MS_U8 u8Count = 0;
    for(u8Count = 0; u8Count < u8DrawGOPCount; u8Count++) //GOPCount
    {
        stgfxPt0.x = 0;
        stgfxPt0.y = 0;
        stgfxPt1.x = gWinInfo[u8Count].u16GopBitmapWidth;
        stgfxPt1.y = gWinInfo[u8Count].u16GopBitmapHeight;
        MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);

        MApi_GFX_SetAlpha(FALSE, COEF_CONST, ABL_FROM_ASRC, 0x80);

        //Switch destination GWIn to GeWinId
        //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
        MApi_GOP_GWIN_Switch2FB(gWinInfo[u8Count].u8FBId);

        stgfxBmpFmt.bScale = true;
        stgfxBmpFmt.x = 0;
        stgfxBmpFmt.y = 0;
        stgfxBmpFmt.width = u16twoSourceWidth;
        stgfxBmpFmt.height = u16twoSourceHeight;
        MApi_GFX_DrawBitmap(bmpinfo[2].hBmp, &stgfxBmpFmt);
        //Set Src buffer
        //MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetFBfromGWIN(GeWinId), &SrcFBInfo);
        MApi_GOP_GWIN_GetFBInfo(gWinInfo[u8Count].u8FBId, &stSrcFBInfo);

        stsrcbuf.u32ColorFmt = u16gwin_fmt;
        stsrcbuf.u32Addr = stSrcFBInfo.addr;
        stsrcbuf.u32Width = stSrcFBInfo.width;
        stsrcbuf.u32Height = stSrcFBInfo.height;
        stsrcbuf.u32Pitch = stSrcFBInfo.pitch;
        MApi_GFX_SetSrcBufferInfo(&stsrcbuf, 0);

        ///Config the src and dst clip in GE buffer
        _SetupGfxDrawRect(&stgfxDrawRect, u16twoSourceWidth - 1, 0, u16twoSourceWidth, u16twoSourceHeight, 0, u16twoSourceHeight, u16twoSourceWidth, u16twoSourceHeight);
        MApi_GFX_SetMirror(TRUE, FALSE);
        /// BitBlt
        MApi_GFX_BitBlt(&stgfxDrawRect, GFXDRAW_FLAG_DEFAULT);

        GFX_BufferInfo stdestbuf;
        stdestbuf.u32Addr = stSrcFBInfo.addr;
        stdestbuf.u32Width = stSrcFBInfo.width;
        stdestbuf.u32Height = stSrcFBInfo.height;
        stdestbuf.u32Pitch = stSrcFBInfo.pitch;
        stdestbuf.u32ColorFmt = u16gwin_fmt;
        GFX_DrawTwoSourceRect stbmpRect;

        MApi_GFX_SetMirror(FALSE, FALSE);
        stbmpRect.srcblk0.x = 0;
        stbmpRect.srcblk0.y = 0;
        stbmpRect.srcblk0.width= u16twoSourceWidth;
        stbmpRect.srcblk0.height= u16twoSourceHeight;
        stbmpRect.srcblk1.x = 0;
        stbmpRect.srcblk1.y = u16twoSourceHeight;
        stbmpRect.srcblk1.width= u16twoSourceWidth;
        stbmpRect.srcblk1.height= u16twoSourceHeight;
        stbmpRect.dstblk.x = u16twoSourceWidth;
        stbmpRect.dstblk.y = u16twoSourceHeight;
        stbmpRect.dstblk.width= u16twoSourceWidth;
        stbmpRect.dstblk.height= u16twoSourceHeight;
        MApi_GFX_SetAlpha(TRUE, COEF_CONST, ABL_FROM_CONST, 0x80);
        GFX_Result ret = MApi_GFX_BitbltByTwoSourceBuffer(&stdestbuf, &stbmpRect, GFXDRAW_FLAG_DEFAULT);
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        if(GFX_SUCCESS != ret)
        {
            printf("Two source bitblt failed! return %d\n",ret);
            return FALSE;
        }

    }
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief None
/// @return None
/// @note
/// Command: \b OSD_DrawJpeg \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawJpeg(void)
{
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;

    GFX_BufferInfo stsrcBufInfo;
    GFX_DrawRect stdrawBuf;
    MS_U32 u32Status;
    MS_U8 u8DrawGOPNum = 0;
    MS_U8 u8Count = 0;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[u8DrawGOPNum].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[u8DrawGOPNum].u16GopBitmapHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //Switch destination GWIn to GeWinId
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[u8DrawGOPNum].u8FBId);

    memcpy((void*)MS_PA2KSEG1(u32BmpUV8Y8Addr), (void*)MS_PA2KSEG1(MEM_ADR_BY_MIU(JPD_OUT_ADR, JPD_OUT_MEMORY_TYPE)), MApi_JPEG_GetWidth() * MApi_JPEG_GetHeight() * 2ul);
    printf("u32BmpUV8Y8Addr= 0x%"DTC_MS_U32_x"\n", (MS_U32)u32BmpUV8Y8Addr);
    printf("BMPUV8Y8SIZE = %lu\n", MApi_JPEG_GetWidth() * MApi_JPEG_GetHeight() * 2ul);

    stsrcBufInfo.u32Addr = u32BmpUV8Y8Addr;
    stsrcBufInfo.u32Width = MApi_JPEG_GetWidth();
    stsrcBufInfo.u32Height = MApi_JPEG_GetHeight();
    stsrcBufInfo.u32Pitch = MApi_JPEG_GetWidth() << 1;
    stsrcBufInfo.u32ColorFmt = GFX_FMT_YUV422;

    u32Status = MApi_GFX_SetSrcBufferInfo(&stsrcBufInfo, 0);
    printf("%d %s MApi_GFX_SetSrcBufferInfo = %"DTC_MS_U32_u"\n", __LINE__, __FUNCTION__, u32Status);

    stdrawBuf.srcblk.x = 0;
    stdrawBuf.srcblk.y = 0;
    stdrawBuf.srcblk.width = MApi_JPEG_GetWidth();
    stdrawBuf.srcblk.height = MApi_JPEG_GetHeight();

    stdrawBuf.dstblk.x = 0;
    stdrawBuf.dstblk.y = 0;
    stdrawBuf.dstblk.width = MApi_JPEG_GetWidth();
    stdrawBuf.dstblk.height = MApi_JPEG_GetHeight();

    printf("[%s]Blitter from (x,y,w,h) = (%d,%d,%d,%d) to (x,y,w,h) = (%d,%d,%d,%d)\n", __FUNCTION__,
    0, 0, MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(),
    0, 0, stdrawBuf.dstblk.width, stdrawBuf.dstblk.height);

    //MApi_GFX_BitBlt(&drawBuf, GFXDRAW_FLAG_DEFAULT);

    MApi_GFX_SetDC_CSC_FMT(0, 0, GFX_YUV_IN_255, GFX_YUV_YUYV, GFX_YUV_YUYV);

    u32Status = MApi_GFX_BitBlt(&stdrawBuf, GFXDRAW_FLAG_SCALE);
    printf("%d %s MApi_GFX_BitBlt = %"DTC_MS_U32_u"\n", __LINE__, __FUNCTION__, u32Status);

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief None
/// @return None
/// @note
/// Command: \b OSD_DrawGPD \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_DrawGPD(MS_BOOL bGif)
{
#if (DEMO_GPD_TEST == 1)
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;

    GFX_BufferInfo stsrcBufInfo;
    GFX_DrawRect stdrawBuf;
    MS_U32 u32Status;
    MS_U8 u8DrawGOPNum = 0;
    MS_U8 u8Count = 0;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[u8DrawGOPNum].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[u8DrawGOPNum].u16GopBitmapHeight;

    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //Switch destination GWIn to GeWinId
    MApi_GOP_GWIN_Switch2FB(gWinInfo[u8DrawGOPNum].u8FBId);

    memcpy((void*)MS_PA2KSEG1(u32BmpUV8Y8Addr), (void*)Demo_GPD_GetOutputBufferAddr(), Demo_GPD_GetOutputBufferSize());
    printf("u32BmpUV8Y8Addr= 0x%"DTC_MS_U32_x"\n", (MS_U32)u32BmpUV8Y8Addr);
    printf("BMPUV8Y8SIZE = %"DTC_MS_U32_u"\n", Demo_GPD_GetOutputBufferSize());

    stsrcBufInfo.u32Addr = u32BmpUV8Y8Addr;
    stsrcBufInfo.u32Width = Demo_GPD_GetWidth(bGif);
    stsrcBufInfo.u32Height = Demo_GPD_GetHeight(bGif);
    switch(Demo_GPD_GetColorFmt())
    {
        case ARGB8888:
            stsrcBufInfo.u32Pitch = ((Demo_GPD_GetWidth(bGif) << 2)+7)& (~(7));;
            stsrcBufInfo.u32ColorFmt = GFX_FMT_ARGB8888;
            break;
        default:
            printf("not surpport color %"DTC_MS_U32_d"\n",Demo_GPD_GetColorFmt());
            break;
    }

    u32Status = MApi_GFX_SetSrcBufferInfo(&stsrcBufInfo, 0);
    printf("[%s] [%d] MApi_GFX_SetSrcBufferInfo = %"DTC_MS_U32_u"\n", __FUNCTION__, __LINE__, u32Status);

    stdrawBuf.srcblk.x = 0;
    stdrawBuf.srcblk.y = 0;
    stdrawBuf.srcblk.width = Demo_GPD_GetWidth(bGif);
    stdrawBuf.srcblk.height = Demo_GPD_GetHeight(bGif);

    if(bGif)
    {
        stdrawBuf.dstblk.x = 0;
        stdrawBuf.dstblk.y = 0;
        stdrawBuf.dstblk.width = Demo_GPD_GetWidth(bGif);
        stdrawBuf.dstblk.height = Demo_GPD_GetHeight(bGif);
    }
    else
    {
        stdrawBuf.dstblk.x = gWinInfo[u8DrawGOPNum].u16GopBitmapWidth/2;
        stdrawBuf.dstblk.y = gWinInfo[u8DrawGOPNum].u16GopBitmapHeight/2;
        stdrawBuf.dstblk.width = Demo_GPD_GetWidth(bGif);
        stdrawBuf.dstblk.height = Demo_GPD_GetHeight(bGif);
    }

    printf("[%s]Blitter from (x,y,w,h) = (%d,%d,%"DTC_MS_U32_d",%"DTC_MS_U32_d") to (x,y,w,h) = (%d,%d,%d,%d)\n", __FUNCTION__,
    0, 0, Demo_GPD_GetWidth(bGif), Demo_GPD_GetHeight(bGif),
    stdrawBuf.dstblk.x, stdrawBuf.dstblk.y, stdrawBuf.dstblk.width, stdrawBuf.dstblk.height);

    //MApi_GFX_BitBlt(&drawBuf, GFXDRAW_FLAG_DEFAULT);

    u32Status = MApi_GFX_BitBlt(&stdrawBuf, GFXDRAW_FLAG_SCALE);
    printf("[%s] [%d] MApi_GFX_BitBlt = %"DTC_MS_U32_u"\n", __FUNCTION__, __LINE__, u32Status);
    //    }

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             printf("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    return TRUE;
#else
    return FALSE;
#endif
}


//------------------------------------------------------------------------------
/// @brief The sample code to free(destory) GWin
/// @return None
/// @note
/// Command: \b OSD_FreeWin \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_FreeWin(void)
{
    MS_U8 u8Count;

    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        MApi_GOP_GWIN_DeleteWin(gWinInfo[u8Count].u8GeWinId);
    }
    return TRUE;
}
#if (DEMO_GOP_GOPSC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to config global Alpha for a GWin
/// @param[in] Source 0 is GOP, 1 is OP
/// @param[in] cmdFlag
/// @return None
/// @note
/// Command: \b OSD_GOPSC_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_GOPSC_Init(MS_U32 *pu32Source, MS_U32 *pu32cmdFlag)
{
    printf("[%s]\n", __FUNCTION__);
    MApi_GOPSC_Init(NULL);
    GOPSC_Info stgopscInfo;
    MApi_GOPSC_GetGOPSCInfo(NULL, &stgopscInfo);
    switch(* pu32Source)
    {
        case 0:
            stgopscInfo.en_Source = EN_Source_GOP;
            break;
        case 1:
            stgopscInfo.en_Source = EN_Source_OP;
            break;
        default:
            stgopscInfo.en_Source = EN_Source_GOP;
    }
    MApi_GOPSC_SetGOPSCInfo(NULL, &stgopscInfo);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Process GOP Scaling
/// @param[in] ScalingOnce 0 - disable, 1 - enable(scaling down continuously), 2 - scaling down once
/// @param[in] SrcW The source width
/// @param[in] SrcH The source heigth
/// @param[in] DestW The destination width
/// @param[in] DestH The destination heigth
/// @param[in] FrameConut Frame count when scaling down once
/// @return None
/// @note
/// Command: \b OSD_GOPSC_HVScalingProcess \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_GOPSC_HVScalingProcess(MS_U32 *pu32Enable, MS_U32 *pu32SrcW, MS_U32 *pu32SrcH, MS_U32 *pu32DestW, MS_U32 *pu32DestH, MS_U32 *pu32FrameConut)
{
    printf("[%s]\n", __FUNCTION__);
    if(0 == *pu32Enable)
    {
        MApi_GOPSC_Enable(NULL,FALSE);
        return TRUE;
    }
    if(1 == *pu32Enable)
    {
        MApi_GOPSC_Enable(NULL,TRUE);
    }
    GOPSC_SizeInfo stgopscInfo;
    stgopscInfo.srcWidth = *pu32SrcW;
    stgopscInfo.srcHeight = *pu32SrcH;
    stgopscInfo.destWidth= *pu32DestW;
    stgopscInfo.destHeight= *pu32DestH;
    stgopscInfo.bEnableScaleX = TRUE;
    stgopscInfo.bEnableScaleY = TRUE;
    MApi_GOPSC_SetHVSPSize(NULL,&stgopscInfo,0);
    if(2 == *pu32Enable && 0 < *pu32FrameConut)
    {
        printf("[%s] Trigger Once\n", __FUNCTION__);
        MApi_GOPSC_ScalingDownOnce(NULL,(*pu32FrameConut));
    }
    return TRUE;
}
#endif

#if (DEMO_MM_TEST == 1)
#define PHOTO_WIDTH_ALIGN(x)                        (((x+15)>>4)<<4)

#if (DEMO_GOP_GOPSC_TEST == 1)
#include "porting_sysinfo.h"
#endif

static MS_BOOL _gbStartDrawPhoto = FALSE;

/// Command: \b OSD_ClearPhoto \n
MS_BOOL Demo_MM_ClearPhoto(void)
{
    GOP_GwinFBAttr stDstFBInfo;
    MS_U8 u8Count;

    //disable GOPSC Path first if necessary
#if (DEMO_GOP_GOPSC_TEST == 1)
    if((u8DrawGOPCount == 1) && (PT_SYS_GetMMPhotoPath() == 0))
    {
        printf("disable GOPSC Path\n");
        MApi_GOP_DWIN_Enable(FALSE);
        MApi_GOPSC_Enable(NULL,FALSE);
        MApi_GOP_GWIN_Enable(gWinInfo[1].u8GeWinId,FALSE);
    }
#endif

    if(PhotoDispDone==true)
    {
        if(MApi_GOP_GWIN_GetCurrentFBID()!=0xFF)
        {
            MApi_GOP_GWIN_GetFBInfo( MApi_GOP_GWIN_GetCurrentFBID(), &stDstFBInfo);
            if(u16gwin_fmt==(MS_U16)E_MS_FMT_YUV422)
                MApi_GFX_ClearFrameBufferByWord(stDstFBInfo.addr, stDstFBInfo.size, 0x80108010);
            else
                MApi_GFX_ClearFrameBuffer(stDstFBInfo.addr, stDstFBInfo.size, 0x0);

            MApi_GFX_FlushQueue();
            PhotoDispDone=false;
        }
    }

    for( u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( gWinInfo[u8Count].u8GeWinId!= 0xFF)
        {
            printf("Delete win %d\n", gWinInfo[u8Count].u8GeWinId);
            MApi_GOP_GWIN_DeleteWin( gWinInfo[u8Count].u8GeWinId );
        }
    }
    return TRUE;
}

/// Command: \b OSD_SetFBFmt \n
MS_BOOL Demo_MM_Photo_SetGwinFmt(MS_U16 *pu16pfmt)
{
    u16gwin_fmt=*pu16pfmt;
    bSetClrFmt = FALSE;//Do not set color format again when OSD_createwin
    printf("u16gwin_fmt [%d] \n",u16gwin_fmt);
    return true;
}

/// Command: \b OSD_DrawPhoto \n
MS_BOOL Demo_MM_DrawPhoto(void)
{
    if(!Demo_MM_IsPlay())
    {
        _gbStartDrawPhoto = FALSE;
        return TRUE;
    }

    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;

    GFX_BufferInfo stsrcBufInfo;
    GFX_DrawRect stdrawBuf;
    MS_U32 u32Status;
    MS_U32 u32ColorFmt=GFX_FMT_YUV422;

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = gWinInfo[0].u16GopBitmapWidth;
    stgfxPt1.y = gWinInfo[0].u16GopBitmapHeight;

    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[0].u8FBId);

    MS_U16 u16Width = 0;
    MS_U16 u16Height = 0;
    MS_U16 u16Pitch = 0;
    MS_U16 u16Format = 0;
    MS_U8 u8Count = 0;

    Demo_MM_Get_DrawPhotoInfo(&u16Width,&u16Height,&u16Pitch,&u16Format);
    switch(u16Format)
    {
        case E_MMSDK_COLOR_FORMAT_YUV422:
            u32ColorFmt=GFX_FMT_YUV422;
            db_print("GFX_FMT_YUV422\n");
            break;
        case E_MMSDK_COLOR_FORMAT_ARGB8888:
            u32ColorFmt=GFX_FMT_ARGB8888;
            db_print("GFX_FMT_ARGB8888\n");
            break;
        case E_MMSDK_COLOR_FORMAT_ARGB1555:
            u32ColorFmt=GFX_FMT_ARGB1555;
            db_print("GFX_FMT_ARGB1555\n");
            break;
        default:
            return FALSE;
            break;
    }

    stsrcBufInfo.u32Addr = Demo_MM_GetOutAdr();
    stsrcBufInfo.u32Width = u16Width;
    stsrcBufInfo.u32Height = u16Height;
    stsrcBufInfo.u32Pitch = u16Pitch;
    stsrcBufInfo.u32ColorFmt = u32ColorFmt;

    u32Status = MApi_GFX_SetSrcBufferInfo(&stsrcBufInfo, 0);

    stdrawBuf.srcblk.x = 0;
    stdrawBuf.srcblk.y = 0;
    stdrawBuf.srcblk.width = u16Width;
    stdrawBuf.srcblk.height = u16Height;

    MS_U32 u32PhotoWidth= u16Width;
    MS_U32 u32PhotoHeight= u16Height;
    MS_U32 u32GWinWidth= (U32)gWinInfo[0].u16GopBitmapWidth;
    MS_U32 u32GWinHeight= (U32)gWinInfo[0].u16GopBitmapHeight;

    if ((u32GWinWidth*u32PhotoHeight) <= (u32GWinHeight*u32PhotoWidth ))
    {
        //Width touch boundary
        stdrawBuf.dstblk.width = gWinInfo[0].u16GopBitmapWidth;
        stdrawBuf.dstblk.x = 0;
        //Get new Hight.
        stdrawBuf.dstblk.height = (U16)((u32PhotoHeight * u32GWinWidth)/u32PhotoWidth);
        stdrawBuf.dstblk.height = ((stdrawBuf.dstblk.height + 1) >> 1) << 1;
        stdrawBuf.dstblk.y = ((gWinInfo[0].u16GopBitmapHeight - stdrawBuf.dstblk.height) / 2);
        stdrawBuf.dstblk.y = (stdrawBuf.dstblk.y >> 1) << 1;
    }
    else
    {
        //Height touch boundary
        stdrawBuf.dstblk.height = gWinInfo[0].u16GopBitmapHeight;
        stdrawBuf.dstblk.y = 0;
        //Get new width
        stdrawBuf.dstblk.width = (U16)((u32PhotoWidth * u32GWinHeight)/u32PhotoHeight);
        stdrawBuf.dstblk.width = ((stdrawBuf.dstblk.width + 1) >> 1) << 1;
        stdrawBuf.dstblk.x = ((gWinInfo[0].u16GopBitmapWidth - stdrawBuf.dstblk.width) / 2);
        stdrawBuf.dstblk.x = (stdrawBuf.dstblk.x >> 1) << 1;
    }


    db_print("[%s]Blitter from (x,y,w,h) = (%d,%d,%d,%d) to (x,y,w,h) = (%d,%d,%d,%d)\n", __FUNCTION__,
        0, 0, stdrawBuf.srcblk.width, stdrawBuf.srcblk.height,
        0, 0, stdrawBuf.dstblk.width, stdrawBuf.dstblk.height);
    MApi_GFX_SetDC_CSC_FMT(0, 0, GFX_YUV_IN_255, GFX_YUV_YVYU, (u16Format == 3)? GFX_YUV_YUYV : GFX_YUV_YVYU);
    u32Status = MApi_GFX_BitBlt(&stdrawBuf, GFXDRAW_FLAG_SCALE);


    for(u8Count = 0; u8Count < u8GOPCount; u8Count++)
    {
        if( GOP_API_SUCCESS != MApi_GOP_GWIN_Enable(gWinInfo[u8Count].u8GeWinId, TRUE) )
        {
             db_print("[%s][%d] MApi_GOP_GWIN_Enable failed", __FUNCTION__, __LINE__);
             return FALSE;
        }
    }

    MApi_GFX_FlushQueue();
    printf("%d %s MApi_GFX_BitBlt = %"DTC_MS_U32_u"\n", __LINE__, __FUNCTION__, u32Status);

    PhotoDispDone = true;
    _gbStartDrawPhoto = TRUE;
    return TRUE;
}

MS_BOOL Demo_MM_StartDrawPhoto(void)
{
    return _gbStartDrawPhoto;
}
#endif

#if (DEMO_DTE_TEST == 1)
//------------------------------------------------------------------------------
/// @This function is used to draw 3D still image after 3D decode
/// @param[in] PhyAddr The address of 3d output
/// @param[in] width The drawing width
/// @param[in] height The drawing height
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b GE_DrawBmp \n
//------------------------------------------------------------------------------
MS_BOOL Demo_OSD_Draw3D(const MS_U32* PhyAddr, const MS_U32 *width, const MS_U32 *height)
{
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;

    GFX_BufferInfo srcBufInfo;
    GFX_DrawRect drawBuf;
    MS_U32 u32Status;

    MS_U32 u32width = *width;
    MS_U32 u32height = *height;

    gfxPt0.x = 0;
    gfxPt0.y = 0;
    gfxPt1.x = gWinInfo[0].u16GopBitmapWidth;
    gfxPt1.y = gWinInfo[0].u16GopBitmapHeight;
    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);
    MApi_GFX_SetAlphaSrcFrom(ABL_FROM_ASRC);

    //Switch destination GWIn to GeWinId
    //MApi_GOP_GWIN_Switch2Gwin(GeWinId);
    MApi_GOP_GWIN_Switch2FB(gWinInfo[0].u8FBId);

    srcBufInfo.u32Addr = *PhyAddr;
    srcBufInfo.u32Width = u32width;
    srcBufInfo.u32Height = u32height;
    srcBufInfo.u32Pitch = u32width << 2;
    srcBufInfo.u32ColorFmt = GFX_FMT_ARGB8888;

    u32Status = MApi_GFX_SetSrcBufferInfo(&srcBufInfo, 0);
    printf("%d %s MApi_GFX_SetSrcBufferInfo = %"DTC_MS_U32_u"\n", __LINE__, __FUNCTION__, u32Status);

    drawBuf.srcblk.x = 0;
    drawBuf.srcblk.y = 0;
    drawBuf.srcblk.width = u32width;
    drawBuf.srcblk.height = u32height;

    drawBuf.dstblk.x = 0;
    drawBuf.dstblk.y = 0;
    drawBuf.dstblk.width = u32width;
    drawBuf.dstblk.height = u32height;

    u32Status = MApi_GFX_BitBlt(&drawBuf, GFXDRAW_FLAG_DEFAULT);

    return TRUE;

}

#endif

#endif
