////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#include <common.h>
#include <command.h>
#include <config.h>
#include <configs/uboot_board_config.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/api/apiGOP.h>
#include <sys/api/apiGFX.h>
#include <sys/api/drvXC_IOPort.h>
#include <sys/api/drvSYS.h>
#include <sys/api/apiXC.h>
#include <sys/drv/drvSEM.h>
#include <sys/drv/drvGPIO.h>
#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES))
#include <sys/api/apiDAC.h>
#include <sys/api/apiHDMITx.h>
#include <sys/drv/drvTVEncoder.h>
#include <display_logo/mdrv_dac_tbl.h>
#include "mdrv_dac_tbl.c"
#elif  defined(CONFIG_KELTIC) //  no HDMI

#include <sys/api/apiDAC.h>
#include <sys/drv/drvTVEncoder.h>
#include <display_logo/mdrv_dac_tbl.h>
#include "mdrv_dac_tbl.c"

#endif

#if (defined(CONFIG_URANUS4))
#include <sys/api/apiGOP_SC.h>
#endif

#include <display_logo/panel.h>
#include <display_logo/display_main.h>
#include <display_logo/DemoFineTune.h>
#include <display_logo/verJPD.h>
#include <display_logo/util_symbol.h>

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
#include "uboot_mmap.h"

#define DISPLAY_DBG(x)                      //x

#define UBOOT_CHUNK_HEADER                  0x20000     //physical address
#define UBOOT_CHUNK_SIZE                    128
#define GOP_FRAME_BUFFER_LENGTH             0x003F4800
#define SYS_REGOP(addr) *((volatile unsigned int*)( (0xBF200000) + (addr)))
#if defined(CONFIG_KELTIC)
#define E_GOP 0
#else
#define E_GOP 3
#endif

static PANEL_RESOLUTION_TYPE g_enPanelType = PANEL_RES_MIN;
static U8 u8FbId = 0xFF, u8GwinId = 0xFF;
static U8 _bModeNTSC = FALSE;
#if defined(CONFIG_KELTIC)
static U8 u8BootLogo[] __attribute__ ((section (".logo"))) =
{
    #include "Mstar_logo_sd.dat"
};
#else
static U8 u8BootLogo[] __attribute__ ((section (".logo"))) =
{
    #include "Mstar_logo.dat"
};
#endif
static U32 u32LogoSize __attribute__ ((section (".logo.size"))) = sizeof(u8BootLogo);

MS_BOOL _sc_is_interlace(void)
{
    return 0;
}

MS_U16 _sc_get_h_cap_start(void)
{
    return 0x60;
}
void _Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
}

#if 0
int Logo_LoadBinFile(U32 u32LogoDramAddr)
{
    U8 *pu8BinAddr = (U8 *)(u32LogoDramAddr |0x80000000);
    char buffer[50];
    U32 u32LogoAddrInSPI;
    U32 u32LogoSizeAlignment;
    DISPLAY_DBG(printf("start get logo bin file\n"));

    //get uboot chunk header
    memset(buffer, 0 , 50);
    //sprintf(buffer, "spi_dma 0x%x 0x%x 0x%x", UBOOT_CHUNK_HEADER, pu8BinAddr, UBOOT_CHUNK_SIZE);
    sprintf(buffer, "spi_rdc 0x%x 0x%x 0x%x", pu8BinAddr, UBOOT_CHUNK_HEADER, UBOOT_CHUNK_SIZE);
    printf("=>cmd: %s\n", buffer);
    run_command(buffer, 0);

    u32LogoAddrInSPI = *(((U32 *)pu8BinAddr)+8);
    u32LogoSize = (*(((U32 *)pu8BinAddr)+9))-u32LogoAddrInSPI;
    u32LogoSizeAlignment = ((u32LogoSize+0x07)&(~0x07));
    printf("logo addr in spi: 0x%x; logo size:0x%x\n", u32LogoAddrInSPI, u32LogoSizeAlignment);

    if (0== u32LogoSize)
        return -1;
    //get logo data
    memset(buffer, 0 , 50);
    //sprintf(buffer, "spi_dma 0x%x 0x%x 0x%x", (u32LogoAddrInSPI&(~0xBFC00000)), pu8BinAddr, u32LogoSizeAlignment);
    sprintf(buffer, "spi_rdc 0x%x 0x%x 0x%x", pu8BinAddr, (u32LogoAddrInSPI&(~0xBFC00000)), u32LogoSizeAlignment);
    printf("=>cmd: %s\n", buffer);
    run_command(buffer, 0);
    DISPLAY_DBG(printf("end get logo bin file\n"));
    return 0;
}
#else
int Logo_LoadBinFile(U32 u32LogoDramAddr)
{
    U8 *pu8BinAddr = (U8 *) MS_PA2KSEG1(u32LogoDramAddr);

    if (0== u32LogoSize)
    {
        return -1;
    }
    else
    {
        memcpy(pu8BinAddr,u8BootLogo,u32LogoSize);
    }

    return 0;
}
#endif

void gop_stretch(U16 graph_pitch, U16 graph_width, U16 graph_height, MS_BOOL bHorStretch, MS_BOOL bVerStretch)
{
    U32 u32GWinDispX_Max;
    U32 u32GWinDispY_Max;

    u32GWinDispX_Max = g_IPanel.Width();
    u32GWinDispY_Max = g_IPanel.Height();
    MApi_GOP_GWIN_SwitchGOP(E_GOP);

#if 0
    if (bHorStretch)
        MApi_GOP_GWIN_Set_HSCALE(TRUE, graph_width, u32GWinDispX_Max);
    if (bVerStretch)
        MApi_GOP_GWIN_Set_VSCALE(TRUE, graph_height, u32GWinDispY_Max);

    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP, E_GOP_DST_OP0, 0, 0, graph_width, graph_height);
#else
    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP, E_GOP_DST_OP0, 0, 0, u32GWinDispX_Max, u32GWinDispY_Max);
#endif
}

static void BurstYUV422(U16 u16X, U16 u16Y,  U16 u16Width, U16 u16Height, U16 u16Pitch, U32 u32InAddr, U32 u32OutAddr, MS_BOOL bHorStretch, MS_BOOL bVerStretch)
{
    GFX_Config tGFXcfg;
    GFX_BufferInfo src_info, dst_info;
    GFX_DrawRect rect;

    //printf("BurstYUV422, W = %d, H = %d, Pitch = %d, InAdr = 0x%x, OutAdr = 0x%x\n", u16Width, u16Height, u16Pitch, u32InAddr, u32OutAddr);

    U16 u16FBWidth = u16Width;
    U16 u16FBHeight = u16Height;
    MS_U32 drawFlag = GFXDRAW_FLAG_DEFAULT;

    if (bHorStretch)
    {
        u16FBWidth = g_IPanel.Width();
        drawFlag = GFXDRAW_FLAG_SCALE;
    }
    if (bVerStretch)
    {
        u16FBHeight = g_IPanel.Height();
        drawFlag = GFXDRAW_FLAG_SCALE;
    }

    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    GFX_Point gfxPt0 = { 0, 0 };
    GFX_Point gfxPt1 = { u16FBWidth, u16FBHeight };
    MApi_GFX_Init(&tGFXcfg);

    src_info.u32Addr = (U32)u32InAddr;
    src_info.u32ColorFmt = GFX_FMT_YUV422;
    src_info.u32Width = u16Width;
    src_info.u32Height = u16Height;
    src_info.u32Pitch = u16Pitch<<1;
    src_info.u32Pitch = ((src_info.u32Pitch+15)/16)*16; // pitch 16 byte-align
    MApi_GFX_SetSrcBufferInfo(&src_info, 0);

    dst_info.u32Addr = u32OutAddr;
    dst_info.u32ColorFmt = GFX_FMT_YUV422;
    dst_info.u32Width = u16FBWidth;
    dst_info.u32Height = u16FBHeight;
    dst_info.u32Pitch = u16FBWidth<<1;           //unit: byte, I8:1 bytes
    dst_info.u32Pitch = ((dst_info.u32Pitch+15)/16)*16; // pitch 16 byte-align
    MApi_GFX_SetDstBufferInfo(&dst_info, 0);

    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_PC, GFX_YUV_IN_255, GFX_YUV_YVYU,  GFX_YUV_YUYV);

    rect.srcblk.x = u16X;
    rect.srcblk.y = u16X;
    rect.srcblk.width = u16Width;
    rect.srcblk.height = u16Height;

    rect.dstblk.x = u16X;
    rect.dstblk.y = u16X;
    rect.dstblk.width = u16FBWidth;
    rect.dstblk.height = u16FBHeight;

    MApi_GFX_BitBlt(&rect, drawFlag);
    MApi_GFX_FlushQueue();
}

static void BurstInitGOP(U16 u16X, U16 u16Y, U16 u16Width, U16 u16Height, U32 u32GopBuffer, MS_BOOL bHorStretch, MS_BOOL bVerStretch)
{
    U32 u32DispX;
    U32 u32DispY;
    U32 u32Width;
    U32 u32Height;
    U32 u32Panel_Hstart;
    GOP_InitInfo pGopInit;
    U16 u16FBWidth = u16Width;
    U16 u16FBHeight = u16Height;

    if (bHorStretch)
    {
        u16FBWidth = g_IPanel.Width();
    }
    if (bVerStretch)
    {
        u16FBHeight = g_IPanel.Height();
    }

    u32DispX = u16X;
    u32DispY = u16Y;
    u32Width = g_IPanel.Width();
    u32Height = g_IPanel.Height();
    u32Panel_Hstart = g_IPanel.HStart();

    //MApi_GOP_RegisterFBFmtCB(( MS_U32(*)(MS_U16 pitch,MS_U32 addr , MS_U16 fmt ))OSD_RESOURCE_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_Sys_PQ_ReduceBW_ForOSD);
    pGopInit.u16PanelWidth = u32Width;
    pGopInit.u16PanelHeight = u32Height;
    pGopInit.u16PanelHStr = u32Panel_Hstart;
    pGopInit.u32GOPRBAdr = u32GopBuffer;//((GOP_GWIN_RB_MEMORY_TYPE & MIU1) ? (GOP_GWIN_RB_ADR | MIU_INTERVAL) : (GOP_GWIN_RB_ADR));
    pGopInit.u32GOPRBLen = GOP_FRAME_BUFFER_LENGTH;
    //there is a GOP_REGDMABASE_MIU1_ADR for MIU1
    pGopInit.u32GOPRegdmaAdr = 0;//((GOP_REGDMABASE_MEMORY_TYPE & MIU1) ? (GOP_REG_DMA_BASE_ADR | MIU_INTERVAL) : (GOP_REG_DMA_BASE_ADR));
    pGopInit.u32GOPRegdmaLen = 0;
    pGopInit.bEnableVsyncIntFlip = FALSE;
    MApi_GOP_InitByGOP(&pGopInit, E_GOP);
    MApi_GOP_GWIN_SwitchGOP(E_GOP);
    MApi_GOP_GWIN_SetGOPDst(E_GOP, E_GOP_DST_OP0);
#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES))
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
#endif


    u8FbId = MApi_GOP_GWIN_GetFreeFBID();
    u8GwinId = MApi_GOP_GWIN_GetFreeWinID();
    //printf("\nGOP=%d, u8GwinId = %d, u8FbId = %d\n", E_GOP, u8GwinId, u8FbId);

    MApi_GOP_GWIN_CreateFBbyStaticAddr(u8FbId, u32DispX, u32DispY, u16FBWidth, u16FBHeight, E_MS_FMT_YUV422, u32GopBuffer);

    MApi_GOP_GWIN_MapFB2Win(u8FbId, u8GwinId);

    MApi_GOP_GWIN_SetWinDispPosition(u8GwinId, u32DispX, u32DispY);
#if defined(CONFIG_URANUS4)
    MApi_GOP_GWIN_SetBlending(u8GwinId, FALSE, 63);
#elif defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES)
    MApi_GOP_GWIN_SetBlending(u8GwinId, FALSE, 255);
#endif
    MApi_GOP_GWIN_Enable(u8GwinId, TRUE);

}


void Display_Logo(U16 u16X, U16 u16Y, U16 u16Width, U16 u16Height,
    U16 u16Pitch, MS_BOOL bHorStretch, MS_BOOL bVerStretch, U32 u32LogoAddr, U32 u32FramBufferAddr)
{
    // fill frame buffer to black color
    U32 i;
    U16 *u16TempAddr;
    u16TempAddr = (U16 *)(u32FramBufferAddr|0xA0000000);
    for (i=0;i<(u16Pitch*u16Height);i++)
    {
        *u16TempAddr = 0x8000;
        u16TempAddr++;
    }
    Chip_Flush_Memory();

    BurstYUV422(0, 0, u16Width, u16Height, u16Pitch, u32LogoAddr,u32FramBufferAddr, bHorStretch, bVerStretch);

    if (bHorStretch)
    {
        u16X = 0;
    }
    if (bVerStretch)
    {
        u16Y = 0;
    }

    BurstInitGOP(u16X, u16Y, u16Width, u16Height, u32FramBufferAddr, bHorStretch, bVerStretch);

    gop_stretch(u16Pitch, u16Width, u16Height, bHorStretch, bVerStretch);
}

#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES))

typedef enum
{
    E_HDMITX_VIDEO_AFD_SameAsPictureAR    = 8, // 1000
    E_HDMITX_VIDEO_AFD_4_3_Center         = 9, // 1001
    E_HDMITX_VIDEO_AFD_16_9_Center        = 10, // 1010
    E_HDMITX_VIDEO_AFD_14_9_Center        = 11, // 1011
    E_HDMITx_VIDEO_AFD_Others = 15, // 0000~ 0111, 1100 ~ 1111
} MsHDMITX_VIDEO_AFD_RATIO;

typedef enum
{
    E_HDMITX_VIDEO_SI_NoData    = 0,
    E_HDMITX_VIDEO_SI_Overscanned         = 1,
    E_HDMITX_VIDEO_SI_Underscanned        = 2,
    E_HDMITX_VIDEO_SI_Reserved    = 3,
} MsHDMITX_VIDEO_SCAN_INFO;

static MS_BOOL IsReciverSupportYPbPr(void)
{
    #define BLOCK_INDEX1    1

    MS_U8 u8BlockData[128] = {0};
    MApi_HDMITx_GetEDIDData(u8BlockData, BLOCK_INDEX1);

    if ( ( u8BlockData[0x03]& 0x30) == 0x00)
    {
        DISPLAY_DBG(printf("Rx Not Support YCbCr\n"));
        return FALSE;
    }
    else
    {
        DISPLAY_DBG(printf("Rx Support YUV444 or YUV422 \n"));
        return TRUE;
    }
}

#define VE_WRITE_ADDR_ADJUSTMENT    (-8)
#define VE_READ_ADDR_ADJUSTMENT     (0)
void verDispTx_Main(void)
{
    MS_VE_VIDEOSYS eVEStd = MS_VE_PAL;
    MS_VE_WINDOW_TYPE stVEWin = {0, 0, 720, 576};
    XC_INITDATA sXC_InitData;
    DISPLAY_DBG(printf("verDispTx_Main\n"));

    memset(&sXC_InitData, 0, sizeof(XC_INITDATA));
    MApi_XC_Init(&sXC_InitData, sizeof(XC_INITDATA));

    MDrv_SYS_Init();

    //case E_VOP_SEL_OSD_BLEND2:
    //    SC_W2BYTEMSK(REG_SC_BK2F_28_L, 0x0000, 0x0001);
    //    SC_W2BYTEMSK(REG_SC_BK0F_46_L, 0x0020, 0x00F0);
    //    SC_W2BYTEMSK(REG_SC_BK0F_57_L, 0x0800, 0x0800);
    *(volatile unsigned int*)0xBF205E00 = 0x000F;
    *(volatile unsigned int*)0xBF205F18 = 0x0020;

    //2. Initialization - DAC and HDMI initialization
    //DISPLAY_DBG(printf("verDispTx_Main - step1\n"));
    MApi_DAC_Init();
    MApi_DAC_Enable(TRUE, TRUE);
    MApi_DAC_Enable(TRUE, FALSE);
    MApi_DAC_SetClkInv(TRUE, TRUE);

#ifndef CONFIG_KELTIC
    MApi_HDMITx_SetDbgLevel(0);
    MApi_HDMITx_SetHPDGpioPin(HDMITX_HPD_PM_GPIO);
    MApi_HDMITx_Init();
    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);
    //MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif

    //3.
    //DISPLAY_DBG(printf("verDispTx_Main - step2\n"));
    DAC_TAB_INFO DacTbl;
    DacTbl = DACMAP_Main[MApi_XC_GetDACType(g_enPanelType)];
    // dump DAC general/scaler/HDGEN register tables
    MApi_DAC_DumpTable(DacTbl.pDacINITTab, DacTbl.eINITtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_SCTab, DacTbl.eINIT_SCtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDGENTab, DacTbl.eINIT_HDGENtype);

    //4.
    //DISPLAY_DBG(printf("verDispTx_Main - step3\n"));
#ifndef CONFIG_KELTIC
    MS_U32 u32StartTime = 0;
    MS_BOOL bHDMISupport = FALSE;
    HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
    HDMITX_OUTPUT_MODE  eOutMode = HDMITX_HDMI;
    HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth = HDMITX_VIDEO_CD_NoID;

    MApi_HDMITx_EdidChecking();
    u32StartTime = MsOS_GetSystemTime();
    while((MApi_HDMITx_EDID_HDMISupport(&bHDMISupport) != TRUE) && (MsOS_Timer_DiffTimeFromNow(u32StartTime) < 10))
    {
        DISPLAY_DBG(printf("EDID NOT READY! \n"));
    }
    /*
     * force read EDID for HDMITx Auto Color Depth Mode
     */
    MApi_HDMITx_GetRxDCInfoFromEDID(&eEDIDColorDepth);
    if(eEDIDColorDepth <= HDMITX_VIDEO_CD_24Bits)
    {
        //To pass simplay lab HDMI test, Item 7-19
        eEDIDColorDepth = HDMITX_VIDEO_CD_NoID;
    }
    //else
    //{
        //Only Support HDMI 8 bit mode in Mboot
        //eEDIDColorDepth = HDMITX_VIDEO_CD_24Bits;
    //}

    if(bHDMISupport)
    {
        if (IsReciverSupportYPbPr() == TRUE)
        {
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
        }
        else
        {
            eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;
        }

        eOutMode = HDMITX_HDMI;
        DISPLAY_DBG(printf("Rx Support HDMI mode! \n"));
    }
    else
    {
        eOutMode = HDMITX_DVI;
        eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;
        DISPLAY_DBG(printf("Rx Support DVI mode only! \n"));
    }
    
    switch(eEDIDColorDepth)
    {
        case HDMITX_VIDEO_CD_NoID:
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BIT_DividerTab, DacTbl.eINIT_HDMITX_8BIT_Dividertype);
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BITTab, DacTbl.eINIT_HDMITX_8BITtype);
            break;

        default:
        case HDMITX_VIDEO_CD_24Bits:
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BIT_DividerTab, DacTbl.eINIT_HDMITX_8BIT_Dividertype);
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BITTab, DacTbl.eINIT_HDMITX_8BITtype);
            break;

        case HDMITX_VIDEO_CD_30Bits:
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_10BIT_DividerTab, DacTbl.eINIT_HDMITX_10BIT_Dividertype);
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_10BITTab, DacTbl.eINIT_HDMITX_10BITtype);
            break;

        case HDMITX_VIDEO_CD_36Bits:
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_12BIT_DividerTab, DacTbl.eINIT_HDMITX_12BIT_Dividertype);
            MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_12BITTab, DacTbl.eINIT_HDMITX_12BITtype);
            break;
    }
    
    DISPLAY_DBG(printf("SetHDMIOutputMode_CD: g_eCurColorDepth = %d \n", eEDIDColorDepth));
    MApi_HDMITx_SetHDMITxMode_CD(eOutMode, eEDIDColorDepth);
    MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_YUV444, eOutColorFmt);
    MApi_HDMITx_SetVideoOutputTiming(MApi_XC_GetHDMIType(g_enPanelType));
    MApi_HDMITx_Exhibit();
    if(MApi_XC_GetHDMIType(g_enPanelType) <= HDMITX_RES_720x576p)
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center);
    }
    else
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center);
    }
    //udelay(500000);
    MApi_HDMITx_SetTMDSOnOff(TRUE);

#endif
    switch(g_enPanelType)
    {
        case DACOUT_480I:
        case DACOUT_480P:
        case DACOUT_720P_60:
        case DACOUT_1080I_60:
        case DACOUT_1080P_30:
        case DACOUT_1080P_60:
            {
                eVEStd = MS_VE_NTSC;
                _bModeNTSC = TRUE;
            }
            break;
        default:
            {
                eVEStd = MS_VE_PAL;
                _bModeNTSC = FALSE;
            }
            break;
    }

    //VE initialization
    //DISPLAY_DBG(printf("verDispTx_Main - step4\n"));
    if(eVEStd >= MS_VE_PAL_N)
    {
        //shift 8 pixels in PAL mode
        DISPLAY_DBG(printf("shift %d pixels in PAL mode \n", VE_WRITE_ADDR_ADJUSTMENT));
        MDrv_VE_AdjustPositionBase(VE_WRITE_ADDR_ADJUSTMENT, VE_READ_ADDR_ADJUSTMENT);
    }
    else
    {
        //shift 0 pixels in NTSC mode
        DISPLAY_DBG(printf("shift 0 pixels in NTSC mode \n"));
        MDrv_VE_AdjustPositionBase(0, VE_READ_ADDR_ADJUSTMENT);
    }

#if (defined(CONFIG_URANUS4))
    MApi_GOP_SC_Init();
#endif
    MDrv_VE_Init(VE_BUFFER_ADDR + VE_FRAMEBUFFER_OFFSET);
    MDrv_VE_SetOSD(TRUE);

    MDrv_VE_SetOutputVideoStd(eVEStd);

    MDrv_VE_set_display_window(stVEWin);

    //1. VE configuration
    MS_Switch_VE_Src_Info SwitchInputSrc;
    SwitchInputSrc.InputSrcType = MS_VE_SRC_SCALER;
    MDrv_VE_SwitchInputSource(&SwitchInputSrc);

    MS_Switch_VE_Dest_Info SwitchOutputDest;
    SwitchOutputDest.OutputDstType = MS_VE_DEST_CVBS;
    MDrv_VE_SwitchOuputDest(&SwitchOutputDest);

    MS_VE_Set_Mode_Type SetModeType;
    SetModeType.u16H_CapSize     = g_IPanel.Width();
    SetModeType.u16V_CapSize     = g_IPanel.Height();
    SetModeType.u16H_CapStart    = g_IPanel.HStart();
    SetModeType.u16V_CapStart    = g_IPanel.VStart();
    SetModeType.u16H_SC_CapSize  = g_IPanel.Width();
    SetModeType.u16V_SC_CapSize  = g_IPanel.Height();
    SetModeType.u16H_SC_CapStart = g_IPanel.HStart();
    SetModeType.u16V_SC_CapStart = g_IPanel.VStart();
    SetModeType.u16InputVFreq = 2500*2/10;
    SetModeType.bSrcInterlace = FALSE;
    MDrv_VE_SetMode(&SetModeType);

    MS_VE_Output_Ctrl OutputCtrl;
    OutputCtrl.bEnable = TRUE;
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    MDrv_VE_SetOutputCtrl(&OutputCtrl);
}
#endif


U32 get_Boot_Logo_addr(void)
{
    return GOP_BUFFER_ADDR;
}




void logo_display_main(LogoDispConfigParams *pstConfig)
{
    char *env_str;
    int ret;

    if (NULL == pstConfig)
        return;

    //if paneltype is stored in flash, get paneltype from MBoot env
    //example, panel_cmd set_paneltype 6
    env_str = getenv("panel_cmd");

    if (env_str != NULL)
    {
        //run do_set_paneltype
        ret = run_command (env_str, 0);
        DISPLAY_DBG(printf("[%s][%d] return value = %d \n", __FUNCTION__, __LINE__, ret));
        DISPLAY_DBG(printf("Set Panel Type by MBoot ENV, g_enPanelType = %d \n", g_enPanelType));
    }
    else
    {
        char tmpstr[20];

        g_enPanelType = MApi_PNL_GetPnlTypeSetting();
        sprintf(tmpstr, "set_paneltype %d", g_enPanelType);
        setenv ("panel_cmd", tmpstr);
        DISPLAY_DBG(printf("Set Panel Type by default value, g_enPanelType = %d \n", g_enPanelType));
    }

    DISPLAY_DBG(printf("logo display start\n"));
    DISPLAY_DBG(printf("[PNL:%d][X-axis:%d][Y-axis:%d][HorStretch:%d][VerStretch:%d] [Jpg Addr:0x%x][Fram Buffer:0x%x]\n", \
                g_enPanelType, pstConfig->u16DispX, pstConfig->u16DispY, pstConfig->bHorStretch, pstConfig->bVerStretch, \
                LOGO_JPG_DRAM_ADDR, GOP_BUFFER_ADDR));

    Util_InitSymbolTBL();
    MApi_PNL_Init( MApi_XC_GetPanelSpec(g_enPanelType));

#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES))
#else
    g_IPanel.Dump();
    g_IPanel.SetGammaTbl(E_APIPNL_GAMMA_12BIT, tAllGammaTab, GAMMA_MAPPING_MODE);
#endif

#if (defined(CONFIG_URANUS4) ||defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES))
    DISPLAY_DBG(printf("logo display start\n"));
    verDispTx_Main();
#endif

    //MApi_PNL_Dump();
    verJPD_Main(MS_VA2PA((MS_U32)u8BootLogo), u32LogoSize);

    Display_Logo(pstConfig->u16DispX, pstConfig->u16DispY, JPG_GetAlignmentWidth(), \
            JPG_GetAlignmentHeight(), JPG_GetAlignmentPitch(), pstConfig->bHorStretch, pstConfig->bVerStretch, \
            JPG_GetOutRawDataAddr(), get_Boot_Logo_addr());

    DISPLAY_DBG(printf("logo display end\n"));
}


int do_boot_logo (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    LogoDispConfigParams stConfig;

    /* check argc */
    if (argc < 5)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    stConfig.u16DispX = simple_strtoul(argv[1], NULL, 10);
    stConfig.u16DispY = simple_strtoul(argv[2], NULL, 10);
    stConfig.bHorStretch = simple_strtoul(argv[3], NULL, 10);
    stConfig.bVerStretch = simple_strtoul(argv[4], NULL, 10);

    logo_display_main(&stConfig);
    return 0;
}

int do_set_paneltype (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    DISPLAY_DBG(printf("[%s][%d] is invoked!! \n", __FUNCTION__, __LINE__));

    /* check argc */
    if (argc < 2)
    {
        printf("Wrong Command!! Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    DISPLAY_DBG(printf("argv[0]: %s\n", argv[0]));
    DISPLAY_DBG(printf("argv[1]: %s\n", argv[1]));
    g_enPanelType = simple_strtoul(argv[1], NULL, 10);
    DISPLAY_DBG(printf("[%s][%d] g_enPanelType = %d", __FUNCTION__, __LINE__, g_enPanelType));
    DISPLAY_DBG(printf("g_enPanelType = %d", g_enPanelType));

    return 0;
}
/*
          K1  Bring Up Modification
*/
void logo_display_osdDestroy(void)
{
    if (u8GwinId != 0xFF)
    {
        MApi_GOP_GWIN_Enable(u8GwinId, FALSE);
        MApi_GOP_GWIN_DeleteWin(u8GwinId);
        u8GwinId = 0xFF;
    }
}

U8 logo_display_modeNTSC(void)
{
    return _bModeNTSC;
}

U_BOOT_CMD(
    set_paneltype,    2,   1,  do_set_paneltype,
    "set_paneltype [type] - Set Mboot panel type and store the type value in env \n",
    "Set Mboot Panel Type\n"
    "    - pType: Panel Type\n"
);

U_BOOT_CMD(
    boot_logo,    5,   1,  do_boot_logo,
    "boot_logo - Logo display \n",
    "Boot up logo display\n"
    "    - DispX: Display X-axis\n"
    "    - DispY: Display Y-axis\n"
    "    - HorStretch: Horizontal Stretch\n"
    "    - VerStretch: Vertical Stretch\n"
);
#endif

