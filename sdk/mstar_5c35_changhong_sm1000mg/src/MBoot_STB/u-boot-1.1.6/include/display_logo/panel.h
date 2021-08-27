#ifndef _PANEL_H_
#define _PANEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/api/apiPNL.h>

#define INTERFACE extern

#ifndef LVDS_PN_SWAP_L
#define LVDS_PN_SWAP_L                  0x00
#endif
#ifndef LVDS_PN_SWAP_H
#define LVDS_PN_SWAP_H                  0x00
#endif
////////////////////////////////////////////////////
//      Type define
////////////////////////////////////////////////////

typedef enum{
    PANEL_RES_MIN = 0,

    PNL_FULLHD_SEC_LE32A = PANEL_RES_MIN,
    // FOR NORMAL LVDS PANEL
    PNL_SXGA_AU17_EN05       = 1,            // 1280X1024, PNL_AU17_EN05_SXGA
    PNL_WXGA_AU20_T200XW02   = 2,            // 1366X768, PNL_AU20_T200XW02_WXGA,
    PNL_WXGA_PLUS_CMO190_M190A1 = 3,         // 1440X900, PNL_CMO19_M190A1_WXGA, PNL_AU19PW01_WXGA
    PNL_WSXGA_AU22_M201EW01  = 4,            // 1680X1050, PNL_AU20_M201EW01_WSXGA,
    PNL_FULLHD_CMO216_H1L01  = 5,            // 1920X1080, PNL_AU37_T370HW01_HD, PNL_CMO216H1_L01_HD.H
    PANEL_RES_FULL_HD       = 5,            // for auotbuild compatitable

    // FOR DAC/HDMI TX OUTPUT
    DACOUT_480I              = 6,            // JUST FOR U3 DAC OUTPUT 480I TIMING USAGE
    DACOUT_480P              = 7,            // JUST FOR U3 DAC OUTPUT 480P TIMING USAGE
    DACOUT_576I              = 8,            // JUST FOR U3 DAC OUTPUT 576I TIMING USAGE
    DACOUT_576P              = 9,            // JUST FOR U3 DAC OUTPUT 576P TIMING USAGE
    DACOUT_720P_50           = 10,           // JUST FOR U3 DAC OUTPUT 720P TIMING USAGE
    DACOUT_720P_60           = 11,           // JUST FOR U3 DAC OUTPUT 720P TIMING USAGE
    DACOUT_1080I_50          = 12,           // JUST FOR U3 DAC OUTPUT 1080I TIMING USAGE
    DACOUT_1080I_60          = 13,           // JUST FOR U3 DAC OUTPUT 1080I TIMING USAGE
    DACOUT_1080P_24          = 14,           // just for U3 Dac output 1080p timing usage
    DACOUT_1080P_25          = 15,           // just for U3 Dac output 1080p timing usage
    DACOUT_1080P_30          = 16,           // just for U3 Dac output 1080p timing usage
    DACOUT_1080P_50          = 17,           // just for U3 Dac output 1080p timing usage
    DACOUT_1080P_60          = 18,           // just for U3 Dac output 1080p timing usage
    DACOUT_640X480P          = 19,           // JUST FOR U3 DAC OUTPUT 1080P TIMING USAGE

    PNL_WUXGA_CMO260_J2,           // 1920*1200
#if ENABLE_MFC
    MFC_DEFAULT,
    MFC120_FULLHD_CMO216_H1L01 = MFC_DEFAULT,
    MFC60_FULLHD_CMO216_H1L01,
#endif
#if ENABLE_TCON
    PNL_FULLHD_TCON,
#endif
    PNL_RES_MAX_NUM,
}PANEL_RESOLUTION_TYPE;

PanelType * MApi_XC_GetPanelSpec(PANEL_RESOLUTION_TYPE enResolutionType);
void MApi_PNL_En(MS_BOOL bPanelOn );
void MApi_PNL_SetBackLight(MS_BOOL bEnable);
PANEL_RESOLUTION_TYPE MApi_PNL_GetPnlTypeSetting(void);
void MApi_PNL_Dump(void);

E_OUTPUT_VIDEO_TIMING_TYPE MApi_XC_GetDACType(PANEL_RESOLUTION_TYPE enResolutionType);

#ifndef CONFIG_KELTIC 
HDMITX_VIDEO_TIMING MApi_XC_GetHDMIType(PANEL_RESOLUTION_TYPE enResolutionType);
#endif

#endif

