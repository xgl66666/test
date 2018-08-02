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

////////////////////////////////////////////////////////////////////////////////
///
/// @file   demo_dip.c
/// @author MStar Semiconductor Inc.
/// @brief  DIP Function Sample Demo
///
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_DIP_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include "MsOS.h"
#include <string.h>

#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiXC.h"
#include "apiXC_EX.h"
#include "apiXC_PCMonitor_EX.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "apiXC_Ace_EX.h"
#include "apiXC_DWIN_EX.h"
#include "apiPNL_EX.h"
#include "apiGOP.h"
#include "apiGOP_v2.h"
#include "apiGFX.h"
#include "drvMVOP.h"
#include "drvXC_IOPort.h"
#include "drvDTC.h"

#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif

#include "demo_dip.h"
#include "demo_osd.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define DIP_CONTI_BUFF_CNT 4 //frame count >=3 when continually capture. Max value is 8.
#define DIP_CONTI_DEMO_FRAME_COUNT 500

//-------------------------------------------------------------------------------------------------
// Debug Macros
//-------------------------------------------------------------------------------------------------
#if ENABLE_DEBUG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static PNL_DeviceId _stPNL_DeviceId = {0, E_PNL_EX_DEVICE0};
static XC_DIP_EX_DATA_FMT _DIPOutputFormat = E_XC_DIP_EX_DATA_FMT_YUV420;
static EN_DDI_DIP_SOURCE_TYPE eDDIDIPSrc = E_DDI_DIP_SOURCE_SC0_OP_CAPTURE;
static EN_DDI_DIP_DISPLAY_TYPE eDDIDIPDispType = E_DDI_DIP_DISPLAY_NONE;
static EN_GOP_COLOR_TYPE _eGOPOutputFormat = E_GOP_COLOR_YUV422;
static MS_U8 u8ARGBAlpha = 0xFF;
static MS_U8 displayGOPNum = 0;
static MS_U8 displayGWINNum = 0;
static MS_S32 DIP_Event_id = -1;
static MS_U32 u32DIPAddress = 0x0;
static MS_U32 u32DIPAddress_pa = 0x0;
static MS_U32 u32DIPAddressMFE = 0x0;
static MS_U32 u32DIPAddressMFE_pa = 0x0;
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U16 _CalcPitch(XC_DIP_EX_DATA_FMT eFmt, MS_U16 u16Width)
{
    MS_U16 u16Pitch = 0;

    switch (eFmt)
    {
        case E_XC_DIP_EX_DATA_FMT_YUV420:
            u16Pitch = u16Width * 3 / 2; //*1.5
            break;
        case E_XC_DIP_EX_DATA_FMT_RGB565:
        case E_XC_DIP_EX_DATA_FMT_YUV422:
            u16Pitch = u16Width << 1;
            break;
        case E_XC_DIP_EX_DATA_FMT_ARGB8888:
            u16Pitch = u16Width << 2;
            break;
        default :
            u16Pitch = 0;
            break;
    }
    return u16Pitch;
}

static MS_ColorFormat _GFX_Format_Mapping(XC_DIP_EX_DATA_FMT eDipFormat)
{
    MS_ColorFormat eGFXFormat = E_MS_FMT_YUV422;

    switch (eDipFormat)
    {
        case E_XC_DIP_EX_DATA_FMT_RGB565:
            eGFXFormat = E_MS_FMT_RGB565;
            break;
        case E_XC_DIP_EX_DATA_FMT_YUV422:
            eGFXFormat = E_MS_FMT_YUV422;
            break;
        case E_XC_DIP_EX_DATA_FMT_ARGB8888:
            eGFXFormat = E_MS_FMT_ARGB8888;
            break;
        case E_XC_DIP_EX_DATA_FMT_YUV420:
            printf("No support GFX format YUV420! \n");
            break;
        default :
            printf("Invalid DIP format! \n");
            break;
    }
    return eGFXFormat;
}

static MS_BOOL _DeleteMemoryPool(void)
{
    MS_BOOL ret;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
    if (u32DIPAddress)
    {
        db_print("Free memory pool for DIP\n");
        ret = MsOS_FreeMemory((void *)u32DIPAddress, s32MstarNonCachedPoolID);
        if(FALSE == ret)
        {
            db_print("Allocate memory pool for DIP failed\n");
            return FALSE;
        }
        u32DIPAddress = 0;
    }
    return TRUE;
}

static MS_BOOL _CreateMemoryforMFE(MS_U32 u32Size, MS_U32 *u32addr, MS_U32 *u32addr_pa)
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(*u32addr != 0x0)
    {
        _DeleteMemoryPool();
    }
    //DIP must align 128 bit. But for GOP show the result, GOP must align 256 bit.
    *u32addr = (MS_U32)MsOS_AllocateMemory(u32Size + 0x100,  s32MstarNonCachedPoolID);
    if(*u32addr == 0x0)
    {
        printf("[%s]-Error : MsOS_AllocateMemory fail\n", __func__);
        return FALSE;
    }
    printf("*u32addr %"DTC_MS_U32_x"\n", *u32addr);
    MS_U32 AddrAlign = ((*u32addr + 0x100) & ~(0x100-1));
    printf("AddrAlign %"DTC_MS_U32_x"\n", AddrAlign);

    *u32addr_pa = MsOS_VA2PA((MS_U32)AddrAlign);
    if (*u32addr_pa == 0x0)
    {
        printf("[%s]-Error : MsOS_VA2PA fail\n", __func__);
        return FALSE;
    }
    printf("DIP create physical buffer address at : 0x%"DTC_MS_U32_x"\n", *u32addr_pa);
    return TRUE;
}

static MS_BOOL _CreateMemoryPool(XC_DIP_EX_DATA_FMT fmt, MS_U32 u32Size)
{
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
    if(u32DIPAddress != 0x0)
    {
        _DeleteMemoryPool();
    }
    //DIP must align 128 bit. But for GOP show the result, GOP must align 256 bit.
    u32DIPAddress = (MS_U32)MsOS_AllocateMemory(u32Size + 0x20,  s32MstarNonCachedPoolID);
    if(u32DIPAddress == 0x0)
    {
        printf("[%s]-Error : MsOS_AllocateMemory fail\n", __func__);
        return FALSE;
    }
    printf("u32DIPAddress %"DTC_MS_U32_x"\n", u32DIPAddress);
    MS_U32 AddrAlign = ((u32DIPAddress + 0x20) & ~(0x20-1));
    printf("AddrAlign %"DTC_MS_U32_x"\n", AddrAlign);

    u32DIPAddress_pa = MsOS_VA2PA((MS_U32)AddrAlign);
    if (u32DIPAddress_pa == 0x0)
    {
        printf("[%s]-Error : MsOS_VA2PA fail\n", __func__);
        return FALSE;
    }
    printf("DIP create physical buffer address at : 0x%"DTC_MS_U32_x"\n", u32DIPAddress_pa);
    return TRUE;
}

static void _DIP_InterruptCb(void)
{
	MS_U16 IntStatus = MApi_XC_DIP_EX_GetIntStatus(E_XC_DIP_EX_DIP_WINDOW);
	MsOS_SetEvent (DIP_Event_id, IntStatus);
    MApi_XC_DIP_EX_ClearInt(IntStatus, E_XC_DIP_EX_DIP_WINDOW);
}

static void _Blt_DIPBuffer_To_GWINBuffer(MS_U16 width, MS_U16 height, MS_U32 u32DIPAddr)
{
    MS_U32 u32Flag = GFXDRAW_FLAG_SCALE;
    GFX_BufferInfo srcbuf;
    GFX_DrawRect bitbltInfo;
    GFX_Point gfxPt0;
    GFX_Point gfxPt1;
    GOP_GwinInfo gwinAttr;

    if(!MApi_GOP_GWIN_GetWinInfo(displayGWINNum, &gwinAttr))
        printf("GOP GWIN GetWinInfo fail \n");

    //Set Clip window for rendering
    gfxPt0.x = 0;
    gfxPt0.y = 0;
    gfxPt1.x = (gwinAttr.u16DispHPixelEnd-gwinAttr.u16DispHPixelStart);
    gfxPt1.y = (gwinAttr.u16DispVPixelEnd-gwinAttr.u16DispVPixelStart);
    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    MApi_GOP_GWIN_Switch2Gwin(displayGWINNum);

    // Bitble the bitmap to DRAM of GE
    srcbuf.u32ColorFmt = _GFX_Format_Mapping(_DIPOutputFormat);
    srcbuf.u32Addr = u32DIPAddr;
    srcbuf.u32Width = width;
    srcbuf.u32Height = height;
    srcbuf.u32Pitch = _CalcPitch(_DIPOutputFormat, width);
    MApi_GFX_SetSrcBufferInfo(&srcbuf, 0);

    bitbltInfo.srcblk.x = 0;
    bitbltInfo.srcblk.y = 0;
    bitbltInfo.srcblk.width = srcbuf.u32Width;
    bitbltInfo.srcblk.height = srcbuf.u32Height;

    bitbltInfo.dstblk.x = 0;
    bitbltInfo.dstblk.y = 0;
    bitbltInfo.dstblk.width = (gwinAttr.u16DispHPixelEnd-gwinAttr.u16DispHPixelStart);
    bitbltInfo.dstblk.height = (gwinAttr.u16DispVPixelEnd-gwinAttr.u16DispVPixelStart);

    MApi_GFX_BitBlt(&bitbltInfo, u32Flag);

    MApi_GFX_FlushQueue();
}

static XC_DIP_EX_SOURCE_TYPE _DIPSource_Mapping(EN_DDI_DIP_SOURCE_TYPE eDDISource)
{
    XC_DIP_EX_SOURCE_TYPE eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_OP_CAPTURE;

    switch(eDDISource)
    {
		case E_DDI_DIP_SOURCE_SC0_IP_MAIN:
			eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_MAIN;
			break;
		case E_DDI_DIP_SOURCE_SC0_IP_SUB:
			eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_SUB;
			break;
		case E_DDI_DIP_SOURCE_SC0_OP_CAPTURE:
			eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_OP_CAPTURE;
			break;
		case E_DDI_DIP_SOURCE_DRAM:
			eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_DRAM;
			break;
                case E_DDI_DIP_SOURCE_SC1_OP_CAPTURE:
			eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_OP_SC1_CAPTURE;
			break;
		default:
			eXCDIPSource = E_XC_DIP_EX_SOURCE_TYPE_OP_CAPTURE;
			break;
    }

    return eXCDIPSource;
}

static EN_AV_Device _Get_SCInputSource(E_DEST_TYPE dest)
{
    XC_MUX_PATH_INFO Paths[MAX_SYNC_DATA_PATH_SUPPORTED];
    MS_U8 i;

    memset(Paths, 0, sizeof(XC_MUX_PATH_INFO)*MAX_SYNC_DATA_PATH_SUPPORTED);

    MApi_XC_Mux_GetPathInfo(Paths);

    for(i=0; i<MAX_SYNC_DATA_PATH_SUPPORTED;i++)
    {
        if(Paths[i].dest == dest)
        {
            if(Paths[i].src == INPUT_SOURCE_DTV2)
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

static MS_BOOL _get_video_info(MS_U8 *bInterlace, MS_U16 *HSize, MS_U16 *VSize, MS_U16 *CropBottom, E_DEST_TYPE dest)
{
    VDEC_EX_DispInfo   info;
    VDEC_EX_Result     ret;
    VDEC_StreamId *stVDECSteamID;
    EN_AV_Device SC_Source;

    SC_Source = _Get_SCInputSource(dest);

    if(SC_Source == E_AV_DEVICE_MAX)
    {
        printf(" Video Get Input source fail!! \n");
        return FALSE;
    }

    stVDECSteamID = Demo_VDEC_GetStreamID(SC_Source);

    memset(&info, 0, sizeof(VDEC_EX_DispInfo));
    ret = MApi_VDEC_EX_GetDispInfo(stVDECSteamID,&info);

    if (E_VDEC_EX_OK != ret)
    {
        return FALSE;
    }
    else
    {
        *bInterlace = info.u8Interlace;
        *HSize = info.u16HorSize;
        *VSize = info.u16VerSize;
        *CropBottom = info.u16CropBottom;
        //printf("[%s][%d]Interlace = %u , H = %d, V = %d\n", __FUNCTION__, __LINE__, *bInterlace, *HSize, *VSize);
        return TRUE;
    }
}

static MS_U32 _GOP_Create_FB(MS_U8 *u8FB, MS_U16 u16FBWidth, MS_U16 u16FBHeight, MS_U16 u16FBFmt, MS_U32 u32addr)
{
    GOP_GwinFBAttr stDstFBInfo;
    // look for available Sdram slot
    *u8FB = MApi_GOP_GWIN_GetFreeFBID();
    if(MAX_GWIN_FB_SUPPORT <= *u8FB)
        return GWIN_NO_AVAILABLE;

    printf("Create FB %d with size (%d,%d) \n", *u8FB, u16FBWidth, u16FBHeight);
    if (MApi_GOP_GWIN_CreateFBbyStaticAddr( *u8FB, 0, 0, u16FBWidth, u16FBHeight, u16FBFmt, u32addr) != GWIN_OK)
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

MS_BOOL _CreateGWINandGOPStretch(MS_U32 u32GWinWidth, MS_U32 u32GWinHeight, MS_U32 u32StretchWidth, MS_U32 u32StretchHeight, MS_U32 u32addr)
{
    MS_U8 u8GOP = 0;
    MS_U8 u8GWin = 0;
    MS_U8 u8FB = 0;

    //GWin settings
    //Create Frame buffer and Gwin
    if( 0 != _GOP_Create_FB(&u8FB, u32GWinWidth, u32GWinHeight, _eGOPOutputFormat, u32addr))
    {
         printf("Create FB fail\n");
         return FALSE;
    }

    u8GOP = 0;
    u8GWin = displayGWINNum;
    // -Create GWin
    if( 0 != _GOP_Create_GWin(u8GOP, &u8GWin, u8FB, _eGOPOutputFormat))
    {
         printf("Create GWin fail, GOPnum = %d, u8GWin = %d, FBId = %d, FBFmt = %d\n", u8GOP,  u8GWin, u8FB, _eGOPOutputFormat);
         return FALSE;
    }
    MApi_GOP_GWIN_SwitchGOP(displayGOPNum);
    MApi_GOP_GWIN_SetGOPDst(displayGOPNum, E_GOP_DST_OP0);
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
    MApi_GOP_GWIN_Set_STRETCHWIN(displayGOPNum,E_GOP_DST_OP0, 0, 0, u32GWinWidth, u32GWinHeight);


    if(u32StretchWidth > u32GWinWidth)
    {
        MApi_GOP_GWIN_Set_HSCALE(TRUE, u32GWinWidth, u32StretchWidth);
    }

    if(u32StretchHeight > u32GWinHeight)
    {
        MApi_GOP_GWIN_Set_VSCALE(TRUE, u32GWinHeight, u32StretchHeight);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initial DIP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_Init(void)
{
    db_print("Demo for DIP_Init \n");

    MApi_XC_DIP_EX_InitByDIP(E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(0xFF, FALSE, E_XC_DIP_EX_DIP_WINDOW);

    DIP_Event_id = MsOS_CreateEventGroup("DIP_Event");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Exit DIP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_Exit \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_Exit(void)
{
    db_print("Demo for DIP_Exit \n");

    if(FALSE == _DeleteMemoryPool())
        return FALSE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to select input source of DIP
/// @param[in] *pu32DIPSrc : DIP input source select
///     -    SC0_IP_MAIN         = 0,
///     -    SC0_IP_SUB          = 1,
///     -    SC0_OP_CAPTURE      = 2,
///     -    DRAM                = 3,
///     -    SC1_OP_CAPTURE      = 4,
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_InputSourceSelect 2 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_InputSourceSelect(MS_U32 *pu32DIPSrc)
{
    eDDIDIPSrc = (EN_DDI_DIP_SOURCE_TYPE)(*pu32DIPSrc);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to select output format of DIP
/// @param[in] *pu32Format : DIP output format select
///     -    YUV422              = 0,
///     -    RGB565              = 1,
///     -    ARGB8888            = 2,
///     -    YUV420              = 3,
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_OutputFormatSelect 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_OutputFormatSelect(MS_U32 *pu32Format)
{
    switch(*pu32Format)
    {
        case 0:
            _DIPOutputFormat = E_XC_DIP_EX_DATA_FMT_YUV422;
            _eGOPOutputFormat = E_GOP_COLOR_YUV422;
	        break;
        case 1:
            _DIPOutputFormat = E_XC_DIP_EX_DATA_FMT_RGB565;
            MApi_XC_DIP_EX_EnableY2R(TRUE,E_XC_DIP_EX_DIP_WINDOW);
            MApi_XC_DIP_EX_SetAlpha(u8ARGBAlpha,E_XC_DIP_EX_DIP_WINDOW);
            _eGOPOutputFormat = E_GOP_COLOR_RGB565;
            break;
        case 2:
            _DIPOutputFormat = E_XC_DIP_EX_DATA_FMT_ARGB8888;
            MApi_XC_DIP_EX_EnableY2R(TRUE,E_XC_DIP_EX_DIP_WINDOW);
            MApi_XC_DIP_EX_SetAlpha(u8ARGBAlpha,E_XC_DIP_EX_DIP_WINDOW);
            _eGOPOutputFormat = E_GOP_COLOR_ABGR8888;
            break;
        case 3:
            _DIPOutputFormat = E_XC_DIP_EX_DATA_FMT_YUV420;
            break;
        default:
           _DIPOutputFormat = E_XC_DIP_EX_DATA_FMT_YUV422;
    }
    MApi_XC_DIP_EX_SetOutputDataFmt(_DIPOutputFormat, E_XC_DIP_EX_DIP_WINDOW);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set alpha for format ARGB
/// @param[in] *pu32Alpha : Alpha value
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_ARGBAlphaSet 125 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_ARGBAlphaSet(MS_U32 *pu32Alpha)
{
    u8ARGBAlpha = (MS_U8)(*pu32Alpha);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set show result type
/// @param[in] *pu32DIPDispType : DIP show use type
///     -    DISPLAY_NONE             = 0,
///     -    DISPLAY_FROM_GOP         = 1,
///     -    DISPLAY_FROM_GEGOP       = 2,
/// @param[in] *pu32GOP : GOP number
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_ShowResultType 2 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_ShowResultType(MS_U32 *pu32DIPDispType, MS_U32 *pu32GOP)
{
    eDDIDIPDispType = (EN_DDI_DIP_DISPLAY_TYPE)(*pu32DIPDispType);

    if((E_XC_DIP_EX_DATA_FMT_YUV420 == _DIPOutputFormat) && (E_DDI_DIP_DISPLAY_NONE != eDDIDIPDispType))
    {
        printf("GE/GOP not support format YUV420! Set display type to none.");
        eDDIDIPDispType = E_DDI_DIP_DISPLAY_NONE;
        return FALSE;
    }

    if(E_DDI_DIP_DISPLAY_NONE != eDDIDIPDispType)
    {
        switch(*pu32GOP)
	{
            // Always use GOP first GWIN to show
            case 0:
                displayGWINNum = GOP0_GWIN_START;
                break;
            case 1:
                displayGWINNum = GOP1_GWIN_START;
                break;
            case 2:
                displayGWINNum = GOP2_GWIN_START;
                break;
            case 3:
                displayGWINNum = GOP3_GWIN_START;
                break;
            case 4:
                displayGWINNum = GOP4_GWIN_START;
                break;
            case 5:
                displayGWINNum = GOP5_GWIN_START;
                break;
            default:
                displayGWINNum = GOP0_GWIN_START;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use DIP capture one frame
/// @param[in] *pu32Width : DIP show width
/// @param[in] *pu32Height : DIP show height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_CaptureOneFrame 720 576\n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_CaptureOneFrame(MS_U32 *pu32Width, MS_U32 *pu32Height)
{
    MS_U8 u8BufCnt = 1;
    MS_U32 u32Size = 0;
    MS_U8 u8Interlace = 0;
    MS_U16 u16WinWidth = 0;
    MS_U16 u16WinHeight = 0;
    MS_U16 u16CropBtm = 0;
    XC_DIP_EX_WINPROPERTY DIPWinProperty;
    XC_EX_SETWIN_INFO xc_dip_wininfo;
    // Set GWIN Address
    GOP_GwinInfo stGWin;

    memset(&DIPWinProperty,0,sizeof(XC_DIP_EX_WINPROPERTY));
    memset(&xc_dip_wininfo,0,sizeof(XC_EX_SETWIN_INFO));
    memset(&stGWin,0,sizeof(GOP_GwinInfo));

    //Get input source info
    if(!(_get_video_info(&u8Interlace, &u16WinWidth, &u16WinHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW)))
    {
        printf("Get input source info fail !! \n");
        return FALSE;
    }

    // Create memory
    u32Size = ((*pu32Height) * _CalcPitch(_DIPOutputFormat, *pu32Width)) * u8BufCnt;
    if(FALSE == _CreateMemoryPool(_DIPOutputFormat, u32Size))
    {
        printf("DIP Create Memory Pool fail!! \n");
        return FALSE;
    }

    if(MApi_GOP_GWIN_IsGWINEnabled(displayGWINNum))
    {
        MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);
        printf("GWIN %d need to disable \n",displayGWINNum);
    }

    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB)
    {
        if(u8Interlace)
        {
            printf("DIP capture once not support interlace source !! \n");
            return TRUE;
        }
    }

    // Set DIPW window Property
    DIPWinProperty.enSource = _DIPSource_Mapping(eDDIDIPSrc);
    DIPWinProperty.u16Width = (*pu32Width);
    DIPWinProperty.u16Height = (*pu32Height);
    DIPWinProperty.u16Pitch = _CalcPitch(_DIPOutputFormat,*pu32Width);
    DIPWinProperty.u32BufStart = u32DIPAddress_pa;
    DIPWinProperty.u32BufEnd = u32DIPAddress_pa + u32Size;
    DIPWinProperty.u8BufCnt = u8BufCnt;
    MApi_XC_DIP_EX_SetDIPWinProperty(&DIPWinProperty, E_XC_DIP_EX_DIP_WINDOW);

    //capture window
    xc_dip_wininfo.stCapWin.x = 0;
    xc_dip_wininfo.stCapWin.y = 0;

    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB)
    {
        xc_dip_wininfo.stCapWin.width = u16WinWidth;
        xc_dip_wininfo.stCapWin.height = u16WinHeight;
    }
    else if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_OP_CAPTURE)
    {
        xc_dip_wininfo.stCapWin.width = g_IPanelEx.Width(&_stPNL_DeviceId);
        xc_dip_wininfo.stCapWin.height = g_IPanelEx.Height(&_stPNL_DeviceId);
    }
    else
    {
        printf("DIP input source wrong , not ready \n");
    }

    //pre-scaling down
    xc_dip_wininfo.bPreHCusScaling = TRUE;
    xc_dip_wininfo.u16PreHCusScalingSrc = xc_dip_wininfo.stCapWin.width;
    xc_dip_wininfo.u16PreHCusScalingDst = (*pu32Width);
    xc_dip_wininfo.bPreVCusScaling = TRUE;
    xc_dip_wininfo.u16PreVCusScalingSrc = xc_dip_wininfo.stCapWin.height;
    xc_dip_wininfo.u16PreVCusScalingDst = (*pu32Height);
    MApi_XC_DIP_EX_SetWindow(&xc_dip_wininfo,sizeof(XC_EX_SETWIN_INFO),E_XC_DIP_EX_DIP_WINDOW);

    // need to clear and enable interrupt for capture once
    MApi_XC_DIP_EX_ClearInt(BIT(0), E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(BIT(0), TRUE, E_XC_DIP_EX_DIP_WINDOW);

    MApi_XC_DIP_EX_CapOneFrame(E_XC_DIP_EX_DIP_WINDOW);//enable capture once

    // Display result type
    if(E_DDI_DIP_DISPLAY_FROM_GOP == eDDIDIPDispType) //Use GOP
    {
        MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
        stGWin.clrType = _eGOPOutputFormat;
        stGWin.u32DRAMRBlkStart = u32DIPAddress_pa;
        MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);
    }
    else if(E_DDI_DIP_DISPLAY_FROM_GEGOP == eDDIDIPDispType) //Use GE+GOP
    {
        _Blt_DIPBuffer_To_GWINBuffer((*pu32Width), (*pu32Height), u32DIPAddress_pa);
    }
    else
    {
        printf("DIP Display result type none! \n");
    }

    MApi_GOP_GWIN_Enable(displayGWINNum, TRUE);//enable GWin

    MApi_XC_DIP_EX_EnaInt(BIT(0), FALSE, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_ClearInt(BIT(0), E_XC_DIP_EX_DIP_WINDOW);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to use DIP capture continually
/// @param[in] *pu32Width : DIP show width
/// @param[in] *pu32Height : DIP show height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_CaptureContinually 720 576\n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_CaptureContinually(MS_U32 *pu32Width, MS_U32 *pu32Height)
{
    MS_U8 u8BufCnt = DIP_CONTI_BUFF_CNT;
    MS_U32 u32Size = 0;
    MS_U32 u32Events = 0;
    MS_U32 u32WaitEventFlag = 0;
    MS_U8 u8FrameIdx = 0;
    MS_U8 u8Interlace = 0;
    MS_U16 u16WinWidth = 0;
    MS_U16 u16WinHeight = 0;
    MS_U16 u16CropBtm = 0;
    MS_U32 u32DemoFrameCount = 0;
    XC_DIP_EX_WINPROPERTY DIPWinProperty;
    XC_EX_SETWIN_INFO xc_dip_wininfo;
    XC_DIP_EX_BUFFER_INFO DIPBufferInfo;
    // Set GWIN Address
    GOP_GwinInfo stGWin;

    memset(&DIPWinProperty,0,sizeof(XC_DIP_EX_WINPROPERTY));
    memset(&xc_dip_wininfo,0,sizeof(XC_EX_SETWIN_INFO));
    memset(&DIPBufferInfo,0,sizeof(XC_DIP_EX_BUFFER_INFO));
    memset(&stGWin,0,sizeof(GOP_GwinInfo));

    //Get input source info
    if(!(_get_video_info(&u8Interlace, &u16WinWidth, &u16WinHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW)))
    {
        printf("Get input source info fail !! \n");
        return FALSE;
    }

    u32Size = ((*pu32Height) * _CalcPitch(_DIPOutputFormat, *pu32Width)) * u8BufCnt;
    if(FALSE == _CreateMemoryPool(_DIPOutputFormat, u32Size))
    {
        printf("DIP Create Memory Pool fail!! \n");
        return FALSE;
    }

    if(MApi_GOP_GWIN_IsGWINEnabled(displayGWINNum))
    {
        MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);
        printf("GWIN %d need to disable \n",displayGWINNum);
    }

    //Set interlace DIPW
    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB )
    {
        if(u8Interlace)
        {
            printf("DIP capture continually not support interlace source !! \n");
            return TRUE;
        }
    }

    // Set DIPW window Property
    DIPWinProperty.enSource = _DIPSource_Mapping(eDDIDIPSrc);
    DIPWinProperty.u16Width = (*pu32Width);
    DIPWinProperty.u16Height = (*pu32Height);
    DIPWinProperty.u16Pitch = _CalcPitch(_DIPOutputFormat, *pu32Width);
    DIPWinProperty.u32BufStart = u32DIPAddress_pa;
    DIPWinProperty.u32BufEnd = u32DIPAddress_pa + u32Size;
    DIPWinProperty.u8BufCnt = u8BufCnt;
    MApi_XC_DIP_EX_SetDIPWinProperty(&DIPWinProperty, E_XC_DIP_EX_DIP_WINDOW);

    //capture window
    xc_dip_wininfo.stCapWin.x = 0;
    xc_dip_wininfo.stCapWin.y = 0;

    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB)
    {
        xc_dip_wininfo.stCapWin.width = u16WinWidth;
        xc_dip_wininfo.stCapWin.height = u16WinHeight;
    }
    else if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_OP_CAPTURE)
    {
        xc_dip_wininfo.stCapWin.width = g_IPanelEx.Width(&_stPNL_DeviceId);
        xc_dip_wininfo.stCapWin.height = g_IPanelEx.Height(&_stPNL_DeviceId);
    }
    else
    {
        printf("DIP input source wrong , not ready \n");
    }

    //pre-scaling down
    xc_dip_wininfo.bPreHCusScaling = TRUE;
    xc_dip_wininfo.u16PreHCusScalingSrc = xc_dip_wininfo.stCapWin.width;
    xc_dip_wininfo.u16PreHCusScalingDst = (*pu32Width);
    xc_dip_wininfo.bPreVCusScaling = TRUE;
    xc_dip_wininfo.u16PreVCusScalingSrc = xc_dip_wininfo.stCapWin.height;
    xc_dip_wininfo.u16PreVCusScalingDst = (*pu32Height);
    MApi_XC_DIP_EX_SetWindow(&xc_dip_wininfo,sizeof(XC_EX_SETWIN_INFO),E_XC_DIP_EX_DIP_WINDOW);

    DIPBufferInfo = MApi_XC_DIP_EX_GetBufInfo(E_XC_DIP_EX_DIP_WINDOW);// Get DIP frame buffer info

    //clear and enable DIP interrupt
    MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0)&0xFF, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0)&0xFF, TRUE, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_InterruptAttach((InterruptCb)_DIP_InterruptCb, E_XC_DIP_EX_DIP_WINDOW);

    MApi_XC_DIP_EX_Ena(TRUE,E_XC_DIP_EX_DIP_WINDOW);//enable DIPW

    while(u32DemoFrameCount < DIP_CONTI_DEMO_FRAME_COUNT)
    {
        u8FrameIdx %= DIP_CONTI_BUFF_CNT;
        u32WaitEventFlag = BIT(u8FrameIdx);
        MsOS_WaitEvent(DIP_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        // Display result type
        if(E_DDI_DIP_DISPLAY_FROM_GOP == eDDIDIPDispType) //Use GOP
        {
            MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
            stGWin.clrType = _eGOPOutputFormat;
            stGWin.u32DRAMRBlkStart = DIPBufferInfo.u32YBuf[u8FrameIdx];
            MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);
        }
        else if(E_DDI_DIP_DISPLAY_FROM_GEGOP == eDDIDIPDispType) //Use GE+GOP
        {
            _Blt_DIPBuffer_To_GWINBuffer((*pu32Width), (*pu32Height), DIPBufferInfo.u32YBuf[u8FrameIdx]);
        }
        else
        {
            printf("DIP Display result type none! \n");
        }

        MApi_GOP_GWIN_Enable(displayGWINNum, TRUE);//enable GWIN
        u8FrameIdx++;
        u32DemoFrameCount++;
    }

    //clear and disable DIP interrupt
    MApi_XC_DIP_EX_InterruptDetach(E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0), FALSE, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0), E_XC_DIP_EX_DIP_WINDOW);

    MApi_XC_DIP_EX_Ena(FALSE,E_XC_DIP_EX_DIP_WINDOW);
    MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);//disable GWIN

    printf("DIP continuous show 500 times done \n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to capture and pingpong with GOP
/// @param[in] *pu32Width : DIP show width
/// @param[in] *pu32Height : DIP show height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b DIP_GOPPingpong 720 576\n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_GOPPingpong(MS_U32 *u32Width, MS_U32 *u32Height)
{
    MS_U8 u8BufCnt = 2;
    MS_U32 u32Size = 0;
    //MS_U32 pinpon_addr = 0;//for utopia2.0 pinpon interface format
    MS_U8 u8Interlace = 0;
    MS_U16 u16WinWidth = 0;
    MS_U16 u16WinHeight = 0;
    MS_U16 u16CropBtm = 0;
    MS_U32 u32DemoFrameCount = 0;
    XC_DIP_EX_WINPROPERTY DIPWinProperty;
    XC_EX_SETWIN_INFO xc_dip_wininfo;

    memset(&DIPWinProperty,0,sizeof(XC_DIP_EX_WINPROPERTY));
    memset(&xc_dip_wininfo,0,sizeof(XC_EX_SETWIN_INFO));

    if(!(_get_video_info(&u8Interlace, &u16WinWidth, &u16WinHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW)))
    {
        printf("Get input source info fail !! \n");
        return FALSE;
    }

    u32Size = ((*u32Height) * _CalcPitch(_DIPOutputFormat, *u32Width)) * u8BufCnt;
    if(FALSE == _CreateMemoryPool(_DIPOutputFormat, u32Size))
    {
        printf("DIP Create Memory Pool fail!! \n");
        return FALSE;
    }

    //Set interlace DIPW
    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB )
    {
        if(u8Interlace)
        {
            printf("DIP pingpong with GOP not support interlace source !! \n");
            return TRUE;
        }
    }

    if(MApi_GOP_GWIN_IsGWINEnabled(displayGWINNum))
    {
        MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);
        printf("GWIN %d need to disable \n",displayGWINNum);
    }

    // Set DIPW window Property
    DIPWinProperty.enSource = _DIPSource_Mapping(eDDIDIPSrc);
    DIPWinProperty.u16Width = (*u32Width);
    DIPWinProperty.u16Height = (*u32Height);
    DIPWinProperty.u16Pitch = _CalcPitch(_DIPOutputFormat,*u32Width);
    DIPWinProperty.u32BufStart = u32DIPAddress_pa;
    DIPWinProperty.u32BufEnd = u32DIPAddress_pa+u32Size;
    DIPWinProperty.u8BufCnt = u8BufCnt;
    MApi_XC_DIP_EX_SetPinpon(TRUE, u32DIPAddress_pa,E_XC_DIP_EX_DIP_WINDOW); // Set DIP Pinpon
    MApi_XC_DIP_EX_SetDIPWinProperty(&DIPWinProperty, E_XC_DIP_EX_DIP_WINDOW);

    //capture window
    xc_dip_wininfo.stCapWin.x = 0;
    xc_dip_wininfo.stCapWin.y = 0;
    xc_dip_wininfo.stCapWin.width = g_IPanelEx.Width(&_stPNL_DeviceId);
    xc_dip_wininfo.stCapWin.height = g_IPanelEx.Height(&_stPNL_DeviceId);
    //pre-scaling down
    xc_dip_wininfo.bPreHCusScaling = TRUE;
    xc_dip_wininfo.u16PreHCusScalingSrc = g_IPanelEx.Width(&_stPNL_DeviceId);
    xc_dip_wininfo.u16PreHCusScalingDst = (*u32Width);
    xc_dip_wininfo.bPreVCusScaling = TRUE;
    xc_dip_wininfo.u16PreVCusScalingSrc = g_IPanelEx.Height(&_stPNL_DeviceId);
    xc_dip_wininfo.u16PreVCusScalingDst = (*u32Height);
    MApi_XC_DIP_EX_SetWindow(&xc_dip_wininfo,sizeof(XC_EX_SETWIN_INFO),E_XC_DIP_EX_DIP_WINDOW);

    MApi_XC_DIP_EX_Ena(TRUE,E_XC_DIP_EX_DIP_WINDOW);
    MApi_GOP_SetPINPON(displayGWINNum,TRUE,GOP_PINPON_DIP);
    MApi_GOP_GWIN_Enable(displayGWINNum, TRUE);

    while(u32DemoFrameCount < DIP_CONTI_DEMO_FRAME_COUNT)
    {
        MsOS_DelayTask(10);
        u32DemoFrameCount++;
    }

    MApi_XC_DIP_EX_Ena(FALSE,E_XC_DIP_EX_DIP_WINDOW);
    MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);
    MApi_GOP_SetPINPON(displayGWINNum,FALSE,GOP_PINPON_DIP);
    MApi_XC_DIP_EX_SetPinpon(FALSE, u32DIPAddress_pa,E_XC_DIP_EX_DIP_WINDOW);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show MFE path use DIP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Example: if you want to use appDemo_DIP_MFE function, please use command like below
/// DIP_Init
/// DIP_InputSourceSelect 2
/// DIP_OutputFormatSelect 1
/// DIP_MFE 720 512 480 480
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_MFE(MS_U32 *u32Width, MS_U32 *u32Height)
{
    MS_U8 u8BufCnt = 2;
    MS_U32 u32Size = 0;
    MS_U8 u8Interlace = 0;
    MS_U16 u16CropBtm = 0;
    MS_U16 u16WinWidth = 0;
    MS_U16 u16WinHeight = 0;
    XC_DIP_EX_WINPROPERTY DIPWinProperty;
    XC_EX_SETWIN_INFO xc_dip_wininfo;

    memset(&DIPWinProperty,0,sizeof(DIPWinProperty));
    memset(&xc_dip_wininfo,0,sizeof(xc_dip_wininfo));

    if(!(_get_video_info(&u8Interlace, &u16WinWidth, &u16WinHeight, &u16CropBtm, OUTPUT_SCALER_MAIN_WINDOW)))
    {
        printf("Get input source info fail !! \n");
        return FALSE;
    }

    u32Size = ((*u32Height) * _CalcPitch(_DIPOutputFormat, *u32Width)) * u8BufCnt;
    if(FALSE == _CreateMemoryforMFE(u32Size,&u32DIPAddressMFE,&u32DIPAddressMFE_pa))
    {
        return FALSE;
    }


    //Set interlace DIPW
    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB )
    {
        if(u8Interlace)
        {
            printf("DIP not support interlace source !! \n");
            return TRUE;
        }
    }

    DIPWinProperty.enSource = _DIPSource_Mapping(eDDIDIPSrc);
    DIPWinProperty.u16Width = (*u32Width);
    DIPWinProperty.u16Height = (*u32Height);
    DIPWinProperty.u16Pitch = _CalcPitch(_DIPOutputFormat,*u32Width);
    DIPWinProperty.u32BufStart = u32DIPAddressMFE_pa;
    DIPWinProperty.u32BufEnd = u32DIPAddressMFE_pa + u32Size;
    DIPWinProperty.u8BufCnt = u8BufCnt;
    MApi_XC_DIP_EX_SetDIPWinProperty(&DIPWinProperty, E_XC_DIP_EX_DIP_WINDOW);

    //capture window
    xc_dip_wininfo.stCapWin.x = 0;
    xc_dip_wininfo.stCapWin.y = 0;

    if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_MAIN || eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_IP_SUB)
    {
        if(u16WinWidth > g_IPanelEx.Width(&_stPNL_DeviceId))
            xc_dip_wininfo.stCapWin.width = g_IPanelEx.Width(&_stPNL_DeviceId);
        else
            xc_dip_wininfo.stCapWin.width = u16WinWidth;
	if(u16WinHeight > g_IPanelEx.Height(&_stPNL_DeviceId))
            xc_dip_wininfo.stCapWin.height = g_IPanelEx.Height(&_stPNL_DeviceId);
        else
            xc_dip_wininfo.stCapWin.height = u16WinHeight;
    }
    else if(eDDIDIPSrc == E_DDI_DIP_SOURCE_SC0_OP_CAPTURE)
    {
        xc_dip_wininfo.stCapWin.width = g_IPanelEx.Width(&_stPNL_DeviceId);
        xc_dip_wininfo.stCapWin.height = g_IPanelEx.Height(&_stPNL_DeviceId);
    }

    //pre-scaling down
    xc_dip_wininfo.bPreHCusScaling = TRUE;
    xc_dip_wininfo.u16PreHCusScalingSrc = xc_dip_wininfo.stCapWin.width;
    xc_dip_wininfo.u16PreHCusScalingDst = (*u32Width);
    xc_dip_wininfo.bPreVCusScaling = TRUE;
    xc_dip_wininfo.u16PreVCusScalingSrc = xc_dip_wininfo.stCapWin.height;
    xc_dip_wininfo.u16PreVCusScalingDst = (*u32Height);
    MApi_XC_DIP_EX_SetWindow(&xc_dip_wininfo,sizeof(XC_EX_SETWIN_INFO),E_XC_DIP_EX_DIP_WINDOW);

    //clear and enable DIP interrupt
    MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0)&0xFF, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0)&0xFF, TRUE, E_XC_DIP_EX_DIP_WINDOW);

    //enable DIPW
    MApi_XC_DIP_EX_Ena(TRUE,E_XC_DIP_EX_DIP_WINDOW);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show DIPW source from DIPR ,memory address from VDEC
/// @param[in] *pu32Device
/// @param[in] *pu32Width : DIP show width
/// @param[in] *pu32Height : DIP show height
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Demo_DIP_DIPRFromVDEC 720 576\n
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_DIPRFromVDEC(MS_U32 *pu32Device,MS_U32 *u32Width, MS_U32 *u32Height)
{
    XC_DEVICE_ID stXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
    MS_U8 u8BufCnt = 2;
    MS_BOOL bDIPHSD = FALSE;      // DIP HSD enable
    MS_BOOL bDIPVSD = FALSE;      // DIP VSD enable
    MS_U16 u16GWINWidth = 0;
    MS_U16 u16GWINHeight = 0;
    MS_U32 u32SizeToShow = 0;
    MS_U32 u32WaitEventFlag = 0;
    MS_U32 u32Events = 0;
    MS_U8 u8FrameIdx = 0;
    MS_U32 u32DemoFrameCount = 0;

    XC_DIP_EX_DIPR_PROPERTY DIPRWinPropertytoShow;//DIPR
    XC_DIP_EX_WINPROPERTY DIPWinPropertytoShow;//DIPW
    XC_EX_SETWIN_INFO xc_dip_wininfotoShow;
    XC_DIP_EX_BUFFER_INFO DIPBufferInfo; //DIPW Buffer Info
    GOP_GwinInfo stGWin;// Set GWIN
    //VDEC
    VDEC_StreamId *streamID = NULL;
    VDEC_EX_FrameInfo stFrameInfo;
    MS_U16 VDECFrameHSize = 0;
    MS_U16 VDECFrameVSize = 0;
    MS_U16 VDECFramePitch = 0;
    MS_U32 VDECFrameYaddress = 0;
    MS_U32 VDECFrameCaddress = 0;

    //Clear memory
    memset(&DIPRWinPropertytoShow,0,sizeof(DIPRWinPropertytoShow));
    memset(&DIPWinPropertytoShow,0,sizeof(DIPWinPropertytoShow));
    memset(&xc_dip_wininfotoShow,0,sizeof(xc_dip_wininfotoShow));
    memset(&DIPBufferInfo,0,sizeof(DIPBufferInfo));
    memset(&stFrameInfo,0,sizeof(stFrameInfo));
    memset(&stGWin,0,sizeof(stGWin));

    //Get VDEC data
    streamID = Demo_VDEC_GetStreamID(*pu32Device);
    MApi_VDEC_EX_GetDecFrameInfo(streamID,&stFrameInfo);
    VDECFrameHSize = stFrameInfo.u16Width;
    VDECFrameVSize = stFrameInfo.u16Height;
    VDECFramePitch = stFrameInfo.u16Pitch;
    VDECFrameYaddress = stFrameInfo.u32LumaAddr;
    VDECFrameCaddress = stFrameInfo.u32ChromaAddr;

    // Check DIP enable HSD and DIPW Width
    if((*u32Width) <= VDECFrameHSize)
    {
        bDIPHSD = TRUE;
        u16GWINWidth = *u32Width;
    }
    else
    {
        u16GWINWidth = VDECFrameHSize;
    }
    // Check DIP enable VSD and DIPW Height
    if(*u32Height <= VDECFrameVSize)
    {
        bDIPVSD = TRUE;
        u16GWINHeight = *u32Height;
    }
    else
    {
        u16GWINHeight = VDECFrameVSize;
    }

    //Create DIP dram size setting
    u32SizeToShow = ((*u32Height) * _CalcPitch(_DIPOutputFormat, VDECFrameHSize)) * u8BufCnt;
    if(FALSE == _CreateMemoryPool(_DIPOutputFormat, u32SizeToShow))
    {
        printf("DIP Create Memory Pool fail!! \n");
        return FALSE;
    }

    //DIPR win setting
    DIPRWinPropertytoShow.enDataFmt = E_XC_DIP_EX_DATA_FMT_YUV420;
    DIPRWinPropertytoShow.u16Width = VDECFramePitch;
    DIPRWinPropertytoShow.u16Height = VDECFrameVSize;
    DIPRWinPropertytoShow.u16Pitch = _CalcPitch(E_XC_DIP_EX_DATA_FMT_YUV420,VDECFramePitch);
    DIPRWinPropertytoShow.u32YBufAddr = VDECFrameYaddress;
    DIPRWinPropertytoShow.u32CBufAddr = VDECFrameCaddress;
    MApi_XC_DIP_EX_SetDIPRProperty(&DIPRWinPropertytoShow,E_XC_DIP_EX_DIP_WINDOW);

    //DIPW win setting
    DIPWinPropertytoShow.enSource = _DIPSource_Mapping(eDDIDIPSrc);
    DIPWinPropertytoShow.u16Width = VDECFrameHSize;    // Because DIPR and DIPW is share line offset,so set the Maximum value between DIPR and DIPW
    DIPWinPropertytoShow.u16Height = u16GWINHeight;
    DIPWinPropertytoShow.u16Pitch = _CalcPitch(_DIPOutputFormat,VDECFrameHSize);
    DIPWinPropertytoShow.u8BufCnt = u8BufCnt;
    DIPWinPropertytoShow.u32BufStart = u32DIPAddress_pa;
    DIPWinPropertytoShow.u32BufEnd = u32DIPAddress_pa + u32SizeToShow;
    MApi_XC_DIP_EX_SetDIPWinProperty(&DIPWinPropertytoShow, E_XC_DIP_EX_DIP_WINDOW);

    //Capture window
    xc_dip_wininfotoShow.stCapWin.x = 0;
    xc_dip_wininfotoShow.stCapWin.y = 0;
    xc_dip_wininfotoShow.stCapWin.width = DIPRWinPropertytoShow.u16Width;
    xc_dip_wininfotoShow.stCapWin.height = DIPRWinPropertytoShow.u16Height;
    xc_dip_wininfotoShow.bPreHCusScaling = bDIPHSD;
    xc_dip_wininfotoShow.u16PreHCusScalingSrc = xc_dip_wininfotoShow.stCapWin.width;
    xc_dip_wininfotoShow.u16PreHCusScalingDst = *u32Width;
    xc_dip_wininfotoShow.bPreVCusScaling = bDIPVSD;
    xc_dip_wininfotoShow.u16PreVCusScalingSrc = xc_dip_wininfotoShow.stCapWin.height;
    xc_dip_wininfotoShow.u16PreVCusScalingDst = *u32Height;
    MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId,E_XC_EX_MAIN_WINDOW,TRUE);
    MApi_XC_DIP_EX_SetWindow(&xc_dip_wininfotoShow,sizeof(XC_EX_SETWIN_INFO),E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId,E_XC_EX_MAIN_WINDOW,FALSE);

    //DIPW buffer info
    DIPBufferInfo = MApi_XC_DIP_EX_GetBufInfo(E_XC_DIP_EX_DIP_WINDOW);

    //CbCr swap
    if(_DIPOutputFormat == E_XC_DIP_EX_DATA_FMT_YUV422)
        MApi_XC_DIP_EX_SwapUV(TRUE,E_XC_DIP_EX_DIP_WINDOW);

    //clear and enable DIP interrupt
    MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0)&0xFF, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0)&0xFF, TRUE, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_InterruptAttach((InterruptCb)_DIP_InterruptCb, E_XC_DIP_EX_DIP_WINDOW);

    //enable DIPW
    MApi_XC_DIP_EX_Ena(TRUE,E_XC_DIP_EX_DIP_WINDOW);

    // Use GOP stretch to scaler up
    _CreateGWINandGOPStretch(u16GWINWidth,u16GWINHeight,*u32Width,*u32Height,u32DIPAddress_pa);

    MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
    stGWin.u16RBlkHRblkSize = _CalcPitch(_DIPOutputFormat,VDECFrameHSize);
    stGWin.clrType = _eGOPOutputFormat;
    stGWin.u32DRAMRBlkStart = DIPBufferInfo.u32YBuf[u8FrameIdx];
    MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);

    while(u32DemoFrameCount < DIP_CONTI_DEMO_FRAME_COUNT)
    {
        //Set DIPR read buffer address
        DIPRWinPropertytoShow.u32YBufAddr = VDECFrameYaddress;
        DIPRWinPropertytoShow.u32CBufAddr = VDECFrameCaddress;
        MApi_XC_DIP_EX_SetDIPRProperty(&DIPRWinPropertytoShow,E_XC_DIP_EX_DIP_WINDOW);

        u8FrameIdx %= u8BufCnt;
        u32WaitEventFlag = BIT(u8FrameIdx);
        MApi_XC_DIP_EX_CapOneFrameFast(E_XC_DIP_EX_DIP_WINDOW);// dipr softwave trigger
        MsOS_WaitEvent(DIP_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        //Change GWIN address
        MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
        stGWin.u32DRAMRBlkStart = DIPBufferInfo.u32YBuf[u8FrameIdx];
        MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);

        if(u8FrameIdx == 0)
            MApi_GOP_GWIN_Enable(displayGWINNum, TRUE);//enable GWIN

        u8FrameIdx++;
        u32DemoFrameCount++;
    }
    //clear and disable DIP interrupt
    MApi_XC_DIP_EX_InterruptDetach(E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0), FALSE, E_XC_DIP_EX_DIP_WINDOW);
    MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0), E_XC_DIP_EX_DIP_WINDOW);

    MApi_XC_DIP_EX_Ena(FALSE,E_XC_DIP_EX_DIP_WINDOW);
    MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);//disable GWIN

    printf("DIPR From VDEC show 500 times done \n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to show DIPW source from DIPR
/// @param[in] *pu32Device
/// @param[in] bEnable : enable DIPR
/// @param[in] bDIPSetInit : enable DIP set
/// @param[in] pu32Width : DIP show width
/// @param[in] pu32Height : DIP show height
/// @param[in] *p_Demo_DIPRSrcInfo : DIPR source info
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command:
//------------------------------------------------------------------------------
MS_BOOL Demo_DIP_DIPR(MS_U32 *pu32Device, MS_BOOL bEnable, MS_BOOL bDIPSetInit,MS_U32 u32Width, MS_U32 u32Height, ST_DEMO_DIPR_SOURCE_INFO *p_Demo_DIPRSrcInfo)
{
    MS_U8 u8BufCnt = 2; // DIPW buffer number
    if(bEnable)
    {
        XC_DEVICE_ID stXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
        MS_BOOL bDIPHSD = FALSE;      // DIP HSD enable
        MS_BOOL bDIPVSD = FALSE;      // DIP VSD enable
        MS_U16 u16GWINWidth = 0;
        MS_U16 u16GWINHeight = 0;
        MS_U32 u32SizeToShow = 0;
        MS_U32 u32Events = 0;

        XC_DIP_EX_DIPR_PROPERTY DIPRWinPropertytoShow;//DIPR
        XC_DIP_EX_WINPROPERTY DIPWinPropertytoShow;//DIPW
        XC_EX_SETWIN_INFO xc_dip_wininfotoShow;
        XC_DIP_EX_BUFFER_INFO DIPBufferInfo; //DIPW Buffer Info
        GOP_GwinInfo stGWin;// Set GWIN

        //Clear
        memset(&DIPRWinPropertytoShow,0,sizeof(DIPRWinPropertytoShow));
        memset(&DIPWinPropertytoShow,0,sizeof(DIPWinPropertytoShow));
        memset(&xc_dip_wininfotoShow,0,sizeof(xc_dip_wininfotoShow));
        memset(&DIPBufferInfo,0,sizeof(DIPBufferInfo));
        memset(&stGWin,0,sizeof(stGWin));

        // Check DIP enable HSD and DIPW Width
        if(u32Width <= p_Demo_DIPRSrcInfo->u16Width)
        {
            bDIPHSD = TRUE;
            u16GWINWidth = u32Width;
        }
        else
        {
            u16GWINWidth = p_Demo_DIPRSrcInfo->u16Width;
        }
        // Check DIP enable VSD and DIPW Height
        if(u32Height <= (p_Demo_DIPRSrcInfo->u16Height))
        {
            bDIPVSD = TRUE;
            u16GWINHeight = u32Height;
        }
        else
        {
            u16GWINHeight = p_Demo_DIPRSrcInfo->u16Height;
        }


        //Create DIP dram size setting
        u32SizeToShow = ((u32Height) * _CalcPitch(_DIPOutputFormat, p_Demo_DIPRSrcInfo->u16Width)) * u8BufCnt;
        if(FALSE == _CreateMemoryPool(_DIPOutputFormat, u32SizeToShow))
        {
            printf("DIP Create Memory Pool fail!! \n");
            return FALSE;
        }

        //DIPR win setting
        DIPRWinPropertytoShow.enDataFmt = p_Demo_DIPRSrcInfo->enDataFmt;
        DIPRWinPropertytoShow.u16Width = p_Demo_DIPRSrcInfo->u16Pitch;    // Need to use VDEC pitch, because HW bug
        DIPRWinPropertytoShow.u16Height = p_Demo_DIPRSrcInfo->u16Height;
        DIPRWinPropertytoShow.u16Pitch = _CalcPitch(p_Demo_DIPRSrcInfo->enDataFmt,p_Demo_DIPRSrcInfo->u16Pitch);// Need to use VDEC pitch, because HW bug
        DIPRWinPropertytoShow.u32YBufAddr = p_Demo_DIPRSrcInfo->u32YBufAddr;
        DIPRWinPropertytoShow.u32CBufAddr = p_Demo_DIPRSrcInfo->u32CBufAddr;
        MApi_XC_DIP_EX_SetDIPRProperty(&DIPRWinPropertytoShow,E_XC_DIP_EX_DIP_WINDOW);

        //DIPW win setting
        DIPWinPropertytoShow.enSource = _DIPSource_Mapping(eDDIDIPSrc);
        DIPWinPropertytoShow.u16Width = p_Demo_DIPRSrcInfo->u16Width; // Because DIPR and DIPW is share line offset,so set the Maximum value between DIPR and DIPW
        DIPWinPropertytoShow.u16Height = u16GWINHeight;
        DIPWinPropertytoShow.u16Pitch = _CalcPitch(_DIPOutputFormat,p_Demo_DIPRSrcInfo->u16Width);
        DIPWinPropertytoShow.u8BufCnt = u8BufCnt;
        DIPWinPropertytoShow.u32BufStart = u32DIPAddress_pa;
        DIPWinPropertytoShow.u32BufEnd = u32DIPAddress_pa + u32SizeToShow;
        MApi_XC_DIP_EX_SetDIPWinProperty(&DIPWinPropertytoShow, E_XC_DIP_EX_DIP_WINDOW);

        //Capture window
        xc_dip_wininfotoShow.stCapWin.x = 0;
        xc_dip_wininfotoShow.stCapWin.y = 0;
        xc_dip_wininfotoShow.stCapWin.width = DIPRWinPropertytoShow.u16Width;
        xc_dip_wininfotoShow.stCapWin.height = DIPRWinPropertytoShow.u16Height;
        xc_dip_wininfotoShow.bPreHCusScaling = bDIPHSD;
        xc_dip_wininfotoShow.u16PreHCusScalingSrc = xc_dip_wininfotoShow.stCapWin.width;
        xc_dip_wininfotoShow.u16PreHCusScalingDst = u32Width;
        xc_dip_wininfotoShow.bPreVCusScaling = bDIPVSD;
        xc_dip_wininfotoShow.u16PreVCusScalingSrc = xc_dip_wininfotoShow.stCapWin.height;
        xc_dip_wininfotoShow.u16PreVCusScalingDst = u32Height;
        MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId,E_XC_EX_MAIN_WINDOW,TRUE);
        MApi_XC_DIP_EX_SetWindow(&xc_dip_wininfotoShow,sizeof(XC_EX_SETWIN_INFO),E_XC_DIP_EX_DIP_WINDOW);
        MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId,E_XC_EX_MAIN_WINDOW,FALSE);

        //DIPW buffer info
        DIPBufferInfo = MApi_XC_DIP_EX_GetBufInfo(E_XC_DIP_EX_DIP_WINDOW);

        //CbCr swap
        if(_DIPOutputFormat == E_XC_DIP_EX_DATA_FMT_YUV422)
            MApi_XC_DIP_EX_SwapUV(TRUE,E_XC_DIP_EX_DIP_WINDOW);

        if(bDIPSetInit)
        {
            //clear and enable DIP interrupt
            MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0)&0xFF, E_XC_DIP_EX_DIP_WINDOW);
            MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0)&0xFF, TRUE, E_XC_DIP_EX_DIP_WINDOW);
            MApi_XC_DIP_EX_InterruptAttach((InterruptCb)_DIP_InterruptCb, E_XC_DIP_EX_DIP_WINDOW);

            MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
            stGWin.u16RBlkHRblkSize = _CalcPitch(_DIPOutputFormat,p_Demo_DIPRSrcInfo->u16Width);
            stGWin.clrType = _eGOPOutputFormat;
            MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);

            //enable DIPW
            MApi_XC_DIP_EX_Ena(TRUE,E_XC_DIP_EX_DIP_WINDOW);

            // Create GWIN and GOP Stretch
            _CreateGWINandGOPStretch(u16GWINWidth,u16GWINHeight,u32Width,u32Height,u32DIPAddress_pa);

            MApi_XC_DIP_EX_CapOneFrameFast(E_XC_DIP_EX_DIP_WINDOW);// dipr softwave trigger
            MsOS_WaitEvent(DIP_Event_id, BIT(u8BufCnt%u8BufCnt), &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

            MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
            stGWin.u32DRAMRBlkStart = DIPBufferInfo.u32YBuf[u8BufCnt%u8BufCnt];
            MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);
            MApi_GOP_GWIN_Enable(displayGWINNum, TRUE);//enable GWIN
        }
        else
        {
            //Change GWIN address
            MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);

            if(stGWin.u32DRAMRBlkStart == DIPBufferInfo.u32YBuf[0])
            {
                MApi_XC_DIP_EX_CapOneFrameFast(E_XC_DIP_EX_DIP_WINDOW);// dipr softwave trigger
                MsOS_WaitEvent(DIP_Event_id, BIT(1), &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

                MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
                stGWin.u32DRAMRBlkStart = DIPBufferInfo.u32YBuf[1];
                MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);
            }
            else if(stGWin.u32DRAMRBlkStart == DIPBufferInfo.u32YBuf[1])
            {
                MApi_XC_DIP_EX_CapOneFrameFast(E_XC_DIP_EX_DIP_WINDOW);// dipr softwave trigger
                MsOS_WaitEvent(DIP_Event_id, BIT(0), &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

                MApi_GOP_GWIN_GetWinInfo(displayGWINNum,&stGWin);
                stGWin.u32DRAMRBlkStart = DIPBufferInfo.u32YBuf[0];
                MApi_GOP_GWIN_SetWinInfo(displayGWINNum,&stGWin);
            }
        }
    }
    else
    {
        //clear and disable DIP interrupt
        MApi_XC_DIP_EX_InterruptDetach(E_XC_DIP_EX_DIP_WINDOW);
        MApi_XC_DIP_EX_EnaInt(BMASK((u8BufCnt-1):0), FALSE, E_XC_DIP_EX_DIP_WINDOW);
        MApi_XC_DIP_EX_ClearInt(BMASK((u8BufCnt-1):0), E_XC_DIP_EX_DIP_WINDOW);
        MApi_XC_DIP_EX_Ena(FALSE,E_XC_DIP_EX_DIP_WINDOW);
        MApi_GOP_GWIN_Enable(displayGWINNum, FALSE);//disable GWIN
    }
    return TRUE;
}

#if 0
// Demo for Demo_DIP_DIPR
MS_BOOL Demo_DIP_DIPRTest(MS_U32 *pu32Device,MS_U32 *u32Width, MS_U32 *u32Height)
{
    MS_U16 test = 0;
    //VDEC
    VDEC_StreamId *streamID = NULL;
    VDEC_EX_FrameInfo stFrameInfo;
    MS_U16 VDECFrameHSize = 0;
    MS_U16 VDECFrameVSize = 0;
    MS_U16 VDECFramePitch = 0;
    MS_U32 VDECFrameYaddress = 0;
    MS_U32 VDECFrameCaddress = 0;

    //Get VDEC data
    streamID = Demo_VDEC_GetStreamID(*pu32Device);
    MApi_VDEC_EX_GetDecFrameInfo(streamID,&stFrameInfo);
    VDECFrameHSize = stFrameInfo.u16Width;
    VDECFrameVSize = stFrameInfo.u16Height;
    VDECFramePitch = stFrameInfo.u16Pitch;
    VDECFrameYaddress = stFrameInfo.u32LumaAddr;
    VDECFrameCaddress = stFrameInfo.u32ChromaAddr;

    ST_DEMO_DIPR_SOURCE_INFO DIPRinfo;

    DIPRinfo.enDataFmt = E_XC_DIP_EX_DATA_FMT_YUV420;
    DIPRinfo.u16Width = VDECFrameHSize;
    DIPRinfo.u16Height = VDECFrameVSize;
    DIPRinfo.u16Pitch = VDECFramePitch;
    DIPRinfo.u32YBufAddr = VDECFrameYaddress;
    DIPRinfo.u32CBufAddr = VDECFrameCaddress;

    while(test < 500 )
    {
       if(test == 0)
        Demo_DIP_DIPR(&pu32Device,TRUE,TRUE,*u32Width,*u32Height,&DIPRinfo);
       else
        Demo_DIP_DIPR(&pu32Device,TRUE,FALSE,*u32Width,*u32Height,&DIPRinfo);

       test++;
    }

    Demo_DIP_DIPR(&pu32Device,FALSE,FALSE,*u32Width,*u32Height,&DIPRinfo);

    return TRUE;
}
#endif

#endif
