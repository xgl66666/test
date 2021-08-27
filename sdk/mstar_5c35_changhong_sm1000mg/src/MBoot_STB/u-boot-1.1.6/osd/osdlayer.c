////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
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
#include <sys/drv/drvSEM.h>
#include <sys/drv/drvGPIO.h>
#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI)|| defined(CONFIG_KERES))
#include <sys/api/apiDAC.h>
#include <sys/api/apiHDMITx.h>
#include <sys/drv/drvTVEncoder.h>
#include <display_logo/mdrv_dac_tbl.h>
#elif defined(CONFIG_KELTIC)
#include <sys/api/apiDAC.h>
#include <sys/drv/drvTVEncoder.h>
#include <display_logo/mdrv_dac_tbl.h>
#endif
#include <display_logo/panel.h>
#include <display_logo/DemoFineTune.h>
#include <display_logo/util_symbol.h>

#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
#include <uboot_mmap.h>
#include "LoaderCharset.h"
#include "LoaderCharset_Eng.h"

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
extern U8 logo_display_modeNTSC(void);
#endif

#define DISPLAY_DBG(x)                      //x


#if defined(CONFIG_KELTIC)
#define E_GOP 0
#else
#define E_GOP 3
#endif



#define ASC_CHAR_TABLE      LoaderEngCharacterset
#define ASC_FONT_TABLE      LoaderEngFont
#define CHAR_WIDTH_TABLE    LoaderEngWidthData

#define UNI_CHAR_TABLE      LoaderCharacterset
#define UNI_FONT_TABLE      LoaderChineseFont

#define ASC_CHAR_ARRAY      EngFont24X32
#define UNI_CHAR_ARRAY      Font24X24

#define ONE_CHAR_WIDTH      24
#define ONE_CHAR_HEIGHT     24
#define EACH_CHAR_INTERVAL  2
#define SPACE_WIDTH         20
#define PROGRESS_NUM        32
#define PROGRESS_WIDTH      10
#define PROGRESS_INTERVAL   4

typedef enum
{
   eTextAlignLeft,
   eTextAlignMiddle,
   eTextAlignRight

} TextAttrib;

static MS_U32 u32FontBitMask[] =
{
    0x80000000, 0x40000000, 0x20000000, 0x10000000,
    0x08000000, 0x04000000, 0x02000000, 0x01000000,
    0x00800000, 0x00400000, 0x00200000, 0x00100000,
    0x00080000, 0x00040000, 0x00020000, 0x00010000,
    0x00008000, 0x00004000, 0x00002000, 0x00001000,
    0x00000800, 0x00000400, 0x00000200, 0x00000100
};

static unsigned char Font24X24[72] = {0};//24X24
static unsigned int EngFont24X32[24] = {0};//24X32
static MS_U8 u8OsdLayerFbId = 0xFF;
static MS_U8 u8OsddisplayFbId = 0xFF;
static MS_U8 u8OsdLayerGwinId = 0xFF;
static PANEL_RESOLUTION_TYPE enPanelType;
static U16 u16OsdX = 0;
static U16 u16OsdY = 0;
static U16 u16OsdW = 0;
static U16 u16OsdH = 0;

static MS_BOOL _sc_is_interlace(void)
{
    return 0;
}

static MS_U16 _sc_get_h_cap_start(void)
{
    return 260;
}

static void _Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
}

static void gop_stretch(U16 graph_width, U16 graph_height, MS_BOOL bHorStretch, MS_BOOL bVerStretch)
{
    U32 u32GWinDispX_Max;
    U32 u32GWinDispY_Max;

    u32GWinDispX_Max = g_IPanel.Width();
    u32GWinDispY_Max = g_IPanel.Height();
    MApi_GOP_GWIN_SwitchGOP(E_GOP);

    printf("GOP_GWIN_Set_STRETCHWIN Horizonal Src: %d Des %d\n", graph_width, u32GWinDispX_Max);
    printf("GOP_GWIN_Set_STRETCHWIN Vertical Src: %d Des %d\n", graph_height, u32GWinDispY_Max);
    if (bHorStretch)
        MApi_GOP_GWIN_Set_HSCALE(TRUE, graph_width, u32GWinDispX_Max);

    if (bVerStretch)
        MApi_GOP_GWIN_Set_VSCALE(TRUE, graph_height, u32GWinDispY_Max);

    MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP, E_GOP_DST_OP0, 0, 0, graph_width, graph_height);
}

#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI) || defined(CONFIG_KELTIC)|| defined(CONFIG_KERES))
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


static void enableMiscOutput(MS_BOOL bEnable)
{
    if(bEnable)
    {
        MApi_DAC_Enable(TRUE, FALSE);
        MApi_DAC_Enable(TRUE, TRUE);
#ifndef CONFIG_KELTIC
        MApi_HDMITx_SetTMDSOnOff(TRUE);
#endif
    }
    else
    {
#ifndef CONFIG_KELTIC
        MApi_HDMITx_SetTMDSOnOff(FALSE);
#endif
        MApi_DAC_Enable(FALSE, TRUE);
        MApi_DAC_Enable(FALSE, FALSE);
    }
}

static void setVEMode(void)
{
    MS_VE_WINDOW_TYPE stVEWin = {0, 0, 720, 576};

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
    if (logo_display_modeNTSC() == TRUE)
    {
        stVEWin.height = 480;
        MDrv_VE_SetOutputVideoStd(MS_VE_NTSC);
    }
    else
    {
        MDrv_VE_SetOutputVideoStd(MS_VE_PAL);
    }
#else
    MDrv_VE_SetOutputVideoStd(MS_VE_PAL);
#endif

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

    MDrv_VE_set_display_window(stVEWin);
}

static MS_BOOL changeResolution(void)
{
#ifndef CONFIG_KELTIC
    static HDMITX_VIDEO_COLORDEPTH_VAL _eCurColorDepth;
#endif
    DAC_TAB_INFO DacTbl;

    DacTbl = DACMAP_Main[MApi_XC_GetDACType(enPanelType)];
    // dump DAC general/scaler/HDGEN register tables
    MApi_DAC_DumpTable(DacTbl.pDacINITTab, DacTbl.eINITtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_SCTab, DacTbl.eINIT_SCtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDGENTab, DacTbl.eINIT_HDGENtype);
#ifndef CONFIG_KELTIC
    MApi_HDMITx_GetRxDCInfoFromEDID(&_eCurColorDepth);
    //DISPLAY_DBG(printf("SetHDMIOutputMode_CD: g_eCurColorDepth = %d \n", _eCurColorDepth));
    switch(_eCurColorDepth)
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

    MApi_HDMITx_SetHDMITxMode_CD(HDMITX_HDMI, _eCurColorDepth);
    MApi_HDMITx_SetVideoOutputTiming(MApi_XC_GetHDMIType(enPanelType));

    if(MApi_XC_GetHDMIType(enPanelType) <= HDMITX_RES_720x576p)
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center);
    }
    else
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center);
    }
#endif
    //reset VE
    setVEMode();

    return TRUE;
}

#define VE_WRITE_ADDR_ADJUSTMENT    (-8)
#define VE_READ_ADDR_ADJUSTMENT     (0)
static MS_BOOL dispInit(void)
{
#ifndef CONFIG_KELTIC
    static HDMITX_VIDEO_COLORDEPTH_VAL _eCurColorDepth;
#endif
    MS_VE_VIDEOSYS eVEStd = MS_VE_PAL;
    MS_VE_WINDOW_TYPE stVEWin = {0, 0, 720, 576};
    //DISPLAY_DBG(printf("verDispTx_Main\n"));



    //2. Initialization - DAC and HDMI initialization
    //DISPLAY_DBG(printf("verDispTx_Main - step1\n"));
    MApi_DAC_Init();
    MApi_DAC_Enable(TRUE, TRUE);
    MApi_DAC_Enable(TRUE, FALSE);
    MApi_DAC_SetClkInv(TRUE, TRUE);
#ifndef CONFIG_KELTIC

    MApi_HDMITx_SetHPDGpioPin(HDMITX_HPD_PM_GPIO);
    MApi_HDMITx_Init();
    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);
    MApi_HDMITx_SetHDMITxMode(HDMITX_HDMI);
    MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_YUV444, HDMITX_VIDEO_COLOR_RGB444);
    //MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif
    //3.
    //DISPLAY_DBG(printf("verDispTx_Main - step2\n"));
    DAC_TAB_INFO DacTbl;
    DacTbl = DACMAP_Main[MApi_XC_GetDACType(enPanelType)];
    // dump DAC general/scaler/HDGEN register tables
    MApi_DAC_DumpTable(DacTbl.pDacINITTab, DacTbl.eINITtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_SCTab, DacTbl.eINIT_SCtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDGENTab, DacTbl.eINIT_HDGENtype);
    #if 0
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITXTab, DacTbl.eINIT_HDMITXtype);
    #else

    /*
     * force read EDID for HDMITx Auto Color Depth Mode
     */
#ifndef CONFIG_KELTIC

    MApi_HDMITx_GetRxDCInfoFromEDID(&_eCurColorDepth);
    //DISPLAY_DBG(printf("SetHDMIOutputMode_CD: g_eCurColorDepth = %d \n", _eCurColorDepth));
    switch(_eCurColorDepth)
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

#endif
#endif

#ifndef CONFIG_KELTIC

    //4.
    //DISPLAY_DBG(printf("verDispTx_Main - step3\n"));
    MApi_HDMITx_SetVideoOutputTiming(MApi_XC_GetHDMIType(enPanelType));
    MApi_HDMITx_Exhibit();
    if(MApi_XC_GetHDMIType(enPanelType) <= HDMITX_RES_720x576p)
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center);
    }
    else
    {
        MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center);
    }
    //udelay(500000);
    MApi_HDMITx_SetTMDSOnOff(TRUE);
    //DISPLAY_DBG(printf("verDispTx_Main - Done\n"));
#endif

    switch(enPanelType)
    {
        case DACOUT_480I:
        case DACOUT_480P:
        case DACOUT_720P_60:
        case DACOUT_1080I_60:
        case DACOUT_1080P_30:
        case DACOUT_1080P_60:
            {
                eVEStd = MS_VE_NTSC;
            }
            break;
        default:
            {
                eVEStd = MS_VE_PAL;
            }
            break;
    }

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

    //case E_VOP_SEL_OSD_BLEND2:
    //    SC_W2BYTEMSK(REG_SC_BK2F_28_L, 0x0000, 0x0001);
    //    SC_W2BYTEMSK(REG_SC_BK0F_46_L, 0x0020, 0x00F0);
    //    SC_W2BYTEMSK(REG_SC_BK0F_57_L, 0x0800, 0x0800);
    *(volatile unsigned int*)0xBF205E00 = 0x000F;
    *(volatile unsigned int*)0xBF205F18 = 0x0020;

    return TRUE;
}

static MS_BOOL dispExit(void)
{
    VE_Result res;
    VE_DrvStatus status;
    memset(&status, 0, sizeof(status));
    res = MDrv_VE_GetStatus(&status);
    if(res != E_VE_OK)
        return FALSE;
    if((U8)status.u16H_CapSize == 0 && (U8)status.u16V_CapSize == 0) //check if not init, if no init, return
        return FALSE;
    MApi_DAC_Enable(FALSE, TRUE);
    MApi_DAC_Enable(FALSE, FALSE);
    MApi_DAC_Exit();
    MDrv_VE_Exit();
    return TRUE;
}
#endif

static void osdDestroy(void)
{
    if (u8OsdLayerGwinId != 0xFF)
    {
        MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, FALSE);
        MApi_GOP_GWIN_DeleteWin(u8OsdLayerGwinId);
        u8OsdLayerGwinId = 0xFF;
        u8OsddisplayFbId = 0xFF;
    }

    if (u8OsdLayerFbId != 0xFF)
    {
        MApi_GOP_GWIN_DeleteFB(u8OsdLayerFbId);
        u8OsdLayerFbId = 0xFF;
    }
}

extern void logo_display_osdDestroy(void);

static MS_U32 GOP_CalcPitch(MS_U8 fbFmt, MS_U16 width)
{
    MS_U16 pitch=0;

    switch ( fbFmt )
    {
    case E_MS_FMT_I1 :
        pitch = (width) >> 3;
        break;
    case E_MS_FMT_I2 :
        pitch = (width<<1) >> 3;
        break;
    case E_MS_FMT_I4 :
        pitch = (width<<2) >> 3;
        break;
    case E_MS_FMT_I8 :
        pitch = width;
        break;
    case E_MS_FMT_RGB565 :
    case E_MS_FMT_ARGB1555 :
    case E_MS_FMT_RGBA5551 :
    case E_MS_FMT_ARGB4444 :
    case E_MS_FMT_RGBA4444 :
    case E_MS_FMT_1ABFgBg12355:
    case E_MS_FMT_FaBaFgBg2266:
    case E_MS_FMT_YUV422:
    case E_MS_FMT_ARGB1555_DST :
        pitch = width << 1;
        break;
    case E_MS_FMT_ARGB8888 :
    case E_MS_FMT_ABGR8888 :
        pitch = width << 2;
        break;
    default :
        //print err
        pitch = 0;
        break;
    }
    return pitch;
}

static MS_BOOL osdBitblt(void)
{
    GFX_BufferInfo src_info, dst_info;
    GOP_GwinFBAttr SrcFBInfo, DstFBInfo;
    GFX_DrawRect rect;

    MApi_GOP_GWIN_SwitchGOP(E_GOP);
    //************GE_stretch_updateFrame*************************//
    MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &SrcFBInfo);
    src_info.u32ColorFmt = SrcFBInfo.fbFmt>>8;
    src_info.u32Addr = SrcFBInfo.addr;
    src_info.u32Width = SrcFBInfo.width;
    src_info.u32Height = SrcFBInfo.height;
    src_info.u32Pitch = SrcFBInfo.pitch;
    MApi_GFX_SetSrcBufferInfo(&src_info, 0);

    MApi_GOP_GWIN_GetFBInfo(u8OsddisplayFbId, &DstFBInfo);
    dst_info.u32ColorFmt = DstFBInfo.fbFmt>>8;
    dst_info.u32Addr = DstFBInfo.addr;
    dst_info.u32Width = DstFBInfo.width;
    dst_info.u32Height = DstFBInfo.height;
    dst_info.u32Pitch = DstFBInfo.pitch;
    MApi_GFX_SetDstBufferInfo(&dst_info, 0);

    GFX_Point gfxPt0 = { 0, 0 };
    GFX_Point gfxPt1 = {g_IPanel.Width(), g_IPanel.Height()};
    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    rect.srcblk.x = 0;
    rect.srcblk.y = 0;
    rect.srcblk.width = SrcFBInfo.width;
    rect.srcblk.height = SrcFBInfo.height;

    rect.dstblk.x = 0;
    rect.dstblk.y = 0;
    rect.dstblk.width = DstFBInfo.width;
    rect.dstblk.height = DstFBInfo.height;

    MApi_GFX_BitBlt(&rect, GFXDRAW_FLAG_SCALE);
    MApi_GFX_FlushQueue();

    return TRUE;
}


static MS_BOOL osdCreate(GFX_Block* pBlk, U32 u32GopBuffer)
{
    U32 u32DispX;
    U32 u32DispY;
    U32 u32Width;
    U32 u32Height;
    U32 u32Panel_Hstart;
    GOP_InitInfo pGopInit;
    MS_BOOL bRet;
    GOP_GwinFBAttr SrcFBInfo;

    osdDestroy();

    enPanelType = MApi_PNL_GetPnlTypeSetting();

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
    if (logo_display_modeNTSC() == TRUE)
    {
        enPanelType = DACOUT_720P_60;
    }
#endif

    Util_InitSymbolTBL();
    bRet = MApi_PNL_Init( MApi_XC_GetPanelSpec(enPanelType));

#if (CONFIG_COMMANDS & CFG_CMD_BOOTLOGO)
    if(bRet == FALSE)
    {
        enableMiscOutput(FALSE);
        logo_display_osdDestroy();
        MApi_PNL_ChangePanelType( MApi_XC_GetPanelSpec(enPanelType));
    }
#endif

#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC))
#else
    g_IPanel.Dump();
    g_IPanel.SetGammaTbl(E_APIPNL_GAMMA_12BIT, tAllGammaTab, GAMMA_MAPPING_MODE);
#endif

    MApi_PNL_En(TRUE);
    MApi_PNL_SetBackLight(BACKLITE_INIT_SETTING);

    /// -Initial GE
    GFX_Config tGFXcfg;
    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);
    printf("driver GE init ok\n");

#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI) || defined(CONFIG_KELTIC)|| defined(CONFIG_KERES))
    if(bRet == FALSE)
    {
        changeResolution();
    }
    else
    {
        dispInit();
    }
#endif

    u32DispX = pBlk->x;
    u32DispY = pBlk->y;
    u32Width = g_IPanel.Width();
    u32Height = g_IPanel.Height();
    u32Panel_Hstart = g_IPanel.HStart();
    MS_U16 u16GWidth = (pBlk->width > u32Width) ? u32Width : pBlk->width;
    MS_U16 u16GHeight = (pBlk->height > u32Height) ? u32Height : pBlk->height;

    //MApi_GOP_RegisterFBFmtCB(( MS_U32(*)(MS_U16 pitch,MS_U32 addr , MS_U16 fmt ))OSD_RESOURCE_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_Sys_PQ_ReduceBW_ForOSD);
    pGopInit.u16PanelWidth = u32Width;
    pGopInit.u16PanelHeight = u32Height;
    pGopInit.u16PanelHStr = u32Panel_Hstart;
    pGopInit.u32GOPRBAdr = u32GopBuffer;
    pGopInit.u32GOPRBLen = 0x003F4800;
    //there is a GOP_REGDMABASE_MIU1_ADR for MIU1
    pGopInit.u32GOPRegdmaAdr = 0;
    pGopInit.u32GOPRegdmaLen = 0;
    pGopInit.bEnableVsyncIntFlip = FALSE;
    MApi_GOP_InitByGOP(&pGopInit, E_GOP);
    MApi_GOP_GWIN_SwitchGOP(E_GOP);
    MApi_GOP_GWIN_SetGOPDst(E_GOP, E_GOP_DST_OP0);
#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI) || defined(CONFIG_KELTIC)|| defined(CONFIG_KERES))
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
#endif

    u8OsdLayerFbId = MApi_GOP_GWIN_GetFreeFBID();
    MApi_GOP_GWIN_CreateFBbyStaticAddr(u8OsdLayerFbId, u32DispX, u32DispY, u16GWidth, u16GHeight, E_MS_FMT_ARGB4444, u32GopBuffer);

    u8OsddisplayFbId = MApi_GOP_GWIN_GetFreeFBID();
    u8OsdLayerGwinId = MApi_GOP_GWIN_GetFreeWinID();
    MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &SrcFBInfo);
    U32 DstBufferAddr = (u16GHeight*GOP_CalcPitch((MS_U8)SrcFBInfo.fbFmt, u16GWidth))+u32GopBuffer;
    MApi_GOP_GWIN_CreateFBbyStaticAddr(u8OsddisplayFbId, u32DispX, u32DispY, u32Width, u32Height, E_MS_FMT_ARGB4444, DstBufferAddr);
    MApi_GOP_GWIN_MapFB2Win(u8OsddisplayFbId, u8OsdLayerGwinId);
    MApi_GFX_ClearFrameBufferByWord(DstBufferAddr, (u32Height*GOP_CalcPitch((MS_U8)SrcFBInfo.fbFmt, u32Width)), 0xf000f000);

    MApi_GOP_GWIN_SetWinDispPosition(u8OsdLayerGwinId, u32DispX, u32DispY);
#if defined(CONFIG_URANUS4)
    MApi_GOP_GWIN_SetBlending(u8OsdLayerGwinId, FALSE, 63);
#elif defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI) || defined(CONFIG_KELTIC) || defined(CONFIG_KERES)
    MApi_GOP_GWIN_SetBlending(u8OsdLayerGwinId, FALSE, 255);
#endif
    MApi_GOP_SetGOPHStart(E_GOP, g_IPanel.HStart());
    MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, TRUE);
    gop_stretch(u32Width, u32Height, FALSE, FALSE);
    printf("GWIN width:%d, height:%d\n", u32Width, u32Height);

    if(bRet == FALSE)
    {
        enableMiscOutput(TRUE);
    }

    return TRUE;
}

static MS_BOOL drawRect(GFX_Block* pBlk, GFX_RgbColor color)
{
    GOP_GwinFBAttr sFBInfo;
    if(u8OsdLayerFbId == 0xFF)
        return FALSE;
    MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &sFBInfo);

    GFX_Point v0 = {sFBInfo.x0, sFBInfo.y0};
    GFX_Point v1 = {sFBInfo.x1, sFBInfo.y1};
    MApi_GFX_SetClip(&v0, &v1);

    GFX_BufferInfo gfxDstBuf;
    gfxDstBuf.u32ColorFmt = (MS_U8)sFBInfo.fbFmt;
    gfxDstBuf.u32Addr = sFBInfo.addr;
    gfxDstBuf.u32Width = sFBInfo.width;
    gfxDstBuf.u32Height = sFBInfo.height;
    gfxDstBuf.u32Pitch = sFBInfo.pitch;
    MApi_GFX_SetDstBufferInfo(&gfxDstBuf, 0);

    GFX_RectFillInfo gfxFillBlock;
    gfxFillBlock.dstBlock.x = pBlk->x;
    gfxFillBlock.dstBlock.y = pBlk->y;
    gfxFillBlock.dstBlock.height = pBlk->height;
    gfxFillBlock.dstBlock.width = pBlk->width;

    gfxFillBlock.fmt = (MS_U8)sFBInfo.fbFmt;

    gfxFillBlock.colorRange.color_s = color;
    gfxFillBlock.colorRange.color_e = color;

    gfxFillBlock.flag = GFXRECT_FLAG_COLOR_CONSTANT;

    MApi_GFX_BeginDraw();
    MApi_GFX_RectFill(&gfxFillBlock);
    MApi_GFX_EndDraw();

    return TRUE;
}

static MS_BOOL drawPixel(GFX_Point p, GFX_RgbColor color)
{
    GFX_Block dstBlk = {p.x, p.y, 1, 1};
    return drawRect(&dstBlk, color);
}

static MS_BOOL drawASCChar(MS_U16 X, MS_U16 Y, MS_U16 W, GFX_RgbColor color)
{
    MS_U16 h = 0;
    for (; h < ONE_CHAR_HEIGHT; ++h)
    {
        if (0 == ASC_CHAR_ARRAY[h])
        {
            continue;
        }

        GFX_Point point;
        point.y = h + Y;

        MS_U16 w = 0;
        for (; w < W; ++w)
        {
            //Draw one ponit
            if (ASC_CHAR_ARRAY[h] & u32FontBitMask[w])
            {
                point.x = w + X;
                drawPixel(point, color);
            }
        }
    }

    return TRUE;
}

static MS_BOOL drawUniChar(MS_U16 X, MS_U16 Y, MS_U16 W, GFX_RgbColor color)
{
    MS_U16 h = 0;
    for (; h < ONE_CHAR_HEIGHT; ++h)
    {
        MS_U16 u16Idx = 3 * h;
        MS_U32 u32Line = (UNI_CHAR_ARRAY[u16Idx] << 24) | (UNI_CHAR_ARRAY[u16Idx + 1] << 16) | (UNI_CHAR_ARRAY[u16Idx + 2] << 8);

        if (0 == u32Line)
        {
            continue;
        }

        GFX_Point point;
        point.y = h + Y;

        MS_U16 w = 0;
        for (; w < W; ++w)
        {
            //Draw one ponit
            if (u32Line & u32FontBitMask[w])
            {
                point.x = w + X;
                drawPixel(point, color);
            }
        }
    }

    return TRUE;
}

static MS_S16 findASCCharIndex(const char* pStrText)
{
    MS_U16 u16Len = strlen(ASC_CHAR_TABLE);
    MS_S16 idx = 0;
    for (; idx < u16Len; ++idx)
    {
        if (ASC_CHAR_TABLE[idx] == pStrText[0])
        {
            return idx;
        }
    }

    return -1;
}

static MS_S16 findUniCharIndex(const char* pStrText)
{
    MS_U16 u16Len = strlen(UNI_CHAR_TABLE);
    MS_S16 idx = 0;
    for (; idx < u16Len; ++idx)
    {
        if (UNI_CHAR_TABLE[idx++] == pStrText[0] && UNI_CHAR_TABLE[idx] == pStrText[1])
        {
            return idx >> 1;
        }
    }

    return -1;
}

static MS_U16 getASCStrTextTotalWidth(const char* pStrText)
{
    MS_U16 u16Total = 0;
    MS_U16 u16Len = strlen(pStrText);
    MS_U16 i = 0;
    for (; i < u16Len; ++i)
    {
        MS_S16 s16Idx = findASCCharIndex(&pStrText[i]);
        u16Total += ((s16Idx < 0) ? SPACE_WIDTH : (CHAR_WIDTH_TABLE[s16Idx] + EACH_CHAR_INTERVAL));
    }

    return u16Total - EACH_CHAR_INTERVAL;
}

static MS_U16 getUniStrTextTotalWidth(const char* pStrText)
{
    MS_U16 u16Total = 0;
    MS_U16 u16Len = strlen(pStrText) >> 1;
    MS_U16 i = 0;
    for (; i < u16Len; ++i)
    {
        u16Total += (ONE_CHAR_WIDTH + EACH_CHAR_INTERVAL);
    }

    return u16Total - EACH_CHAR_INTERVAL;
}

static MS_BOOL drawOneASCCharByIdx(MS_S16 s16Idx, MS_U16 X, MS_U16 Y, GFX_RgbColor color)
{
    if (s16Idx < 0)
    {
        return FALSE;
    }

    memcpy(ASC_CHAR_ARRAY, &ASC_FONT_TABLE[s16Idx * 24], 96);
    drawASCChar(X, Y, CHAR_WIDTH_TABLE[s16Idx], color);
    return TRUE;
}

static MS_BOOL drawOneUniCharByIdx(MS_S16 s16Idx, MS_U16 X, MS_U16 Y, GFX_RgbColor color)
{
    if (s16Idx < 0)
    {
        return FALSE;
    }

    memcpy(UNI_CHAR_ARRAY, &UNI_FONT_TABLE[s16Idx * 72], 72);
    drawUniChar(X, Y, ONE_CHAR_WIDTH, color);
    return TRUE;
}

static MS_BOOL drawASCStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib)
{
    switch (eTextAttrib)
    {
        case eTextAlignMiddle:
        {
            GOP_GwinFBAttr sFBInfo;
            MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &sFBInfo);
            X = (sFBInfo.width - getASCStrTextTotalWidth(pStrText)) >> 1;
            break;
        }

        case eTextAlignRight:
            X -= getASCStrTextTotalWidth(pStrText);
            break;

        case eTextAlignLeft:
        default:
            break;
    }

    MS_U16 u16Offset = 0;
    MS_U16 u16Len = strlen(pStrText);
    MS_U16 i = 0;

    for(; i < u16Len; ++i)
    {
        X += u16Offset;
        MS_S16 s16Idx = findASCCharIndex(&pStrText[i]);
        drawOneASCCharByIdx(s16Idx, X, Y, color);
        u16Offset = (s16Idx < 0) ? SPACE_WIDTH : (CHAR_WIDTH_TABLE[s16Idx] + EACH_CHAR_INTERVAL);
    }

    return TRUE;
}

static MS_BOOL drawUniStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib)
{
    switch (eTextAttrib)
    {
        case eTextAlignMiddle:
        {
            GOP_GwinFBAttr sFBInfo;
            MApi_GOP_GWIN_GetFBInfo(u8OsdLayerFbId, &sFBInfo);
            X = (sFBInfo.width - getUniStrTextTotalWidth(pStrText)) >> 1;
            break;
        }

        case eTextAlignRight:
            X -= getUniStrTextTotalWidth(pStrText);
            break;

        case eTextAlignLeft:
        default:
            break;
    }

    MS_U16 u16Offset = 0;
    MS_U16 u16Len = strlen(pStrText);
    MS_U16 i = 0;

    for(; i < u16Len; i += 2)
    {
        X += u16Offset;
        MS_S16 s16Idx = findUniCharIndex(&pStrText[i]);
        drawOneUniCharByIdx(s16Idx, X, Y, color);
        u16Offset = (s16Idx < 0) ? SPACE_WIDTH : (ONE_CHAR_WIDTH + EACH_CHAR_INTERVAL);
    }

    return TRUE;
}

static MS_BOOL drawStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib)
{
    return ((MS_U8)pStrText[0] < 0xA0) ? drawASCStrText(pStrText, X, Y, color, eTextAttrib) : drawUniStrText(pStrText, X, Y, color, eTextAttrib);
}

static MS_BOOL drawProgress(MS_U16 X, MS_U16 Y, GFX_RgbColor fcolor, MS_U8 per)
{
    if (per > 100)
    {
        per = 100;
    }

    MS_U16 u16StartX = X;
    MS_U16 u16Total = (PROGRESS_NUM * PROGRESS_WIDTH) * per / 100;
    MS_U16 u16Count = u16Total / PROGRESS_WIDTH;

    memset(ASC_CHAR_ARRAY, 0xFF, 96);

    MS_U16 i = 0;
    for (; i < u16Count; ++i)
    {
        drawASCChar(u16StartX, Y, PROGRESS_WIDTH, fcolor);
        u16StartX += (PROGRESS_WIDTH + PROGRESS_INTERVAL);
    }

    MS_U16 u16Left = u16Total % PROGRESS_WIDTH;
    if (u16Left != 0)
    {
        drawASCChar(u16StartX, Y, u16Left, fcolor);
        u16StartX += (u16Left + PROGRESS_INTERVAL);
    }

    char cStr[5] = {0};
    sprintf(cStr, "%d%%", per);
    drawASCStrText(cStr, X + PROGRESS_NUM * (PROGRESS_WIDTH + PROGRESS_INTERVAL), Y, fcolor, eTextAlignLeft);

    return TRUE;
}

int do_create_osdlayer (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 w = simple_strtoul(argv[1], NULL, 10);
    MS_U16 h = simple_strtoul(argv[2], NULL, 10);
    MS_U32 size = w * h;

    U16* u16TempAddr = (U16*)(GOP_BUFFER_ADDR | 0xA0000000);
    MS_U32 i = 0;

    for (; i < size; ++i)
    {
        *(u16TempAddr++) = 0x0000;
    }

    Chip_Flush_Memory();

    u16OsdX = 0;
    u16OsdY = 0;
    u16OsdW = w;
    u16OsdH = h;
    GFX_Block dstBlk = {0, 0, w, h};
    osdCreate(&dstBlk, GOP_BUFFER_ADDR);

    return 0;
}

int do_destroy_osdlayer (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    {
        //Set OSD to black before destroy
        GFX_RgbColor color;
        color.a = 0;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        if(u16OsdW == 0 && u16OsdH == 0)
        {
            u16OsdW = 720;
            u16OsdW = 576;
        }
        GFX_Block dstBlk = {u16OsdX, u16OsdY, u16OsdW, u16OsdH};
        drawRect(&dstBlk, color);
        udelay(1000000);
    }
    osdDestroy();
#if (defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KRITI)|| defined(CONFIG_KELTIC)|| defined(CONFIG_KERES))
    dispExit();
#endif //(CONFIG_URANUS4)
    return 0;
}

int do_draw_rect (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc < 6)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U16 w = simple_strtoul(argv[3], NULL, 10);
    MS_U16 h = simple_strtoul(argv[4], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[5], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    GFX_Block dstBlk = {x, y, w, h};
    drawRect(&dstBlk, color);
    osdBitblt();
    return 0;
}

int do_draw_pixel (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[3], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    GFX_Point p = {x, y};
    drawPixel(p, color);
    return 0;
}

int do_draw_string (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc < 6)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[3], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    MS_U16 attrib = simple_strtoul(argv[4], NULL, 10);

    char cStr[202] = {0};
    MS_U16 u16Total = 0;
    MS_S16 i = 5;

    for (; i < argc; ++i)
    {
        MS_U16 u16Len = strlen(argv[i]);
        if (u16Total + u16Len > 200)
        {
            break;
        }

        sprintf(cStr + u16Total, "%s", argv[i]);
        u16Total += u16Len;
        cStr[u16Total++] = ' ';
    }

    cStr[u16Total] = 0;

    drawStrText(cStr, x, y, color, attrib);
    osdBitblt();
    return 0;
}

int do_draw_Progress (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MS_U16 x = simple_strtoul(argv[1], NULL, 10);
    MS_U16 y = simple_strtoul(argv[2], NULL, 10);
    MS_U8 per = simple_strtoul(argv[4], NULL, 10);
    MS_U32 tColor = simple_strtoul(argv[3], NULL, 16);

    GFX_RgbColor color;
    color.a = tColor >> 24;
    color.r = (tColor >> 16) & 0xFF;
    color.g = (tColor >> 8) & 0xFF;
    color.b = tColor & 0xFF;
    drawProgress(x, y, color, per);
    osdBitblt();
    return 0;
}

U_BOOT_CMD(
    osd_create,    3,   1,  do_create_osdlayer,
    "osd_create - create osd layer \n",
    "create osd layer \n"
    "    - w: in Dec\n"
    "    - h: in Dec\n"
);

U_BOOT_CMD(
    osd_destroy,    1,   1,  do_destroy_osdlayer,
    "osd_destroy - destroy osd layer \n",
    "destroy osd layer \n"
);

U_BOOT_CMD(
    draw_rect,    6,   1,  do_draw_rect,
    "draw_rect - draw rect with color \n",
    "draw rect \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - w: in Dec\n"
    "    - h: in Dec\n"
    "    - color: in Hex\n"
);

U_BOOT_CMD(
    draw_pixel,    4,   1,  do_draw_pixel,
    "draw_pixel - draw a pixel with color \n",
    "draw a pixel \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
);

U_BOOT_CMD(
    draw_string,    100,   1,  do_draw_string,
    "draw_string - draw string with color \n",
    "draw string \n"
    "    - string: in char\n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
    "    - attrib: align attrib, 0: left, 1: middle, 2: right\n"
);

U_BOOT_CMD(
    draw_progress,    5,   1,  do_draw_Progress,
    "draw_string - draw string with color \n",
    "draw string \n"
    "    - x: in Dec\n"
    "    - y: in Dec\n"
    "    - color: in Hex\n"
    "    - per: percent in Dec\n"
);

#endif
