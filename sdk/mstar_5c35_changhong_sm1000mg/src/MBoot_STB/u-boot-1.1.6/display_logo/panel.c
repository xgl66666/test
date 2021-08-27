////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¨MStar Confidential Information〃) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#include <common.h>
#include <configs/uboot_panel_config.h>
#include <sys/api/apiPNL.h>
#include <sys/api/apiDAC.h>
#include <sys/api/apiHDMITx.h>
#include <sys/drv/drvGPIO.h>
#include <display_logo/panel.h>

#define ms_delay(x)        \
        do  \
        {   \
            udelay(x*1000); \
        }while(0);

PanelType  tDACOUT640X480P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_640X480P_60",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        96,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        48,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        2,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        33,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        96+48,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        2+33,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        640,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        480,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        800 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        800,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        800 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        525 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        525,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        525 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (800*525*60)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (800*525*60)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (800*525*60)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x73FBDE,   //   define PANEL_MAX_SET
       0x3555E9,   //   define PANEL_MIN_SET
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT480I_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_480I_60",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        62,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        60,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        30,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        62+60,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+30,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        480,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        857 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        857,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        857 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        524 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        524,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        524 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (857*524*60)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (857*524*60)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (857*524*60)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x2CD3FE,
       0x1ED4C4,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT480P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_480P_60",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        62,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        60,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        30,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        62+60,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+30,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        480,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        858 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        858,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        858 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        525 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        525,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        525 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (858*525*60)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (858*525*60)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (858*525*60)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x73FBDE,
       0x3555E9,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT576I_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_576I_50",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
            PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power
/*
        126,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        138,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        19,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        126+138,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        3+19,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)
*/
        64,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        68,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        39,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        64+68,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+39,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        576,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        864 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        864,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        864 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        625 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        625,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        625 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (864*625*50)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (864*625*50)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (864*625*50)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x2CE213,
       0x1EDB6D,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT576P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_576P_50",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
            PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        //#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A) || ( MS_BOARD_TYPE_SEL== BD_MST087C_D02A_S ) )
        //1,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#else
        //0,          //BOOL m_bPanelSwapLVDS_POL :1;     //PANEL_SWAP_LVDS_POL
        //#endif
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        64,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        68,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        39,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        64+68,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+39,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        720,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        576,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        864 + 100,  //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        864,        //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        864 - 100,  //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        625 + 100,  //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        625,        //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        625 - 100,  //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (864*625*50)/1000000 + 10,  //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (864*625*50)/1000000,       //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (864*625*50)/1000000 - 10,  //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB

       0,          //   double clock
       0x742B06,
       0x355FE0,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT720P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_60",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1650 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1650,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1650 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1650*750*60)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1650*750*60)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1650*750*60)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT720P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_720P_50",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        40,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        220,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        20,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        40+220,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+20,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        720,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1980 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1980,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1980 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        750 + 100,      //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        750,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        750 - 100,      //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (1980*750*50)/1000000 + 10,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (1980*750*50)/1000000,          //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (1980*750*50)/1000000 - 10,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1EBCB1,
       0x1770C0,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT1080I_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080I_50",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        15,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+15,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2640 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2640,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2640 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2640*1125*50)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2640*1125*50)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2640*1125*50)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT1080I_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080I_60",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_I,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        15,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+15,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT1080P_50[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_50",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2640 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2640,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2640 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2640*1125*50)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2640*1125*50)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2640*1125*50)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT1080P_60[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_60",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*60)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*60)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*60)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT1080P_24[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_24",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*24)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*24)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*24)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};


PanelType  tDACOUT1080P_25[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_25",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*25)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*25)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*25)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tDACOUT1080P_30[] =
{
    {//Dac Out for U3 bringup
        "DACOUT_1080P_30",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_DAC_P,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

#ifdef PANEL_SWAP_PORT
        PANEL_SWAP_PORT,
#else
        0,          //BOOL m_bPanelSwapPort :1;         //PANEL_SWAP_PORT
#endif

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        44,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        148,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        5,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        36,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        44+148,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        5+36,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2200 + 100,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,           //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        2200 - 100,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1125 + 100,     //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1125,           //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1125 - 100,     //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (2200*1125*30)/1000000 + 10,    //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (2200*1125*30)/1000000,         //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (2200*1125*30)/1000000 - 10,    //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1C848E,
       0x18EB59,
       E_PNL_CHG_VTOTAL,
    },
};


PanelType  tPanelSXGA_AU17_EN05[] =
{
   {
        "SXGA_AU17_EN05",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        1,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;        //PANEL_EVEN_DATA_CURRENT    // even data current

        10,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        250,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        100,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        24,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        2,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        38,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+24,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH-1)
        2+38,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1280,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1024,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1800,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1688,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        1664,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        2047,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1066,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL
        1035,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        140,        //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        108,        //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        100,        //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,        //m_ucDimmingCtl
        0xFF,        //m_ucMaxPWMVal;
        0x50,         //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_4_3,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x277F44,
       0x1C3655,
       E_PNL_CHG_VTOTAL,
    },
};//


PanelType  tPanelWXGA_AU20_T200XW02[] =
{
    {
        "WXGA_AU20_T200XW02",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        0^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30, //45,   //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        400,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        80,         //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30, //20,   //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        20,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        40,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        4,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        34,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        20+40,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        4+34,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1366,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        768,        //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1722ul,     //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1560ul,//1430,  //WORD m_wPanelHTotal;          //PANEL_HTOTAL
        1414ul,     //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        822,        //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        806ul,      //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        789,        //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        88, //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        76, //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        60, //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl
        255,        //m_ucMaxPWMVal;
        0x50,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x20ea0e,
       0x167109,
       E_PNL_CHG_VTOTAL,
    },
};//!<Panel Data 저장 구조체.(Multi Panel 대응을 위해 사용)

PanelType  tPanelWXGAP_CMO_M190A1[] =
{
    {

        "WXGAP_CMO_M190A1",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        500,            //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        150,            //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        104,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        24,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        3,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        6,          //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        104+24,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        3+6,            //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1440,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        900,            //WORD m_wPanelHeight;              //PANEL_HEIGHT

        1920,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1760,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        1520,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1050,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        950,            //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        910,            //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        112,     //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        100,     //DWORD m_dwPanelDCLK;          //PANEL_DCLK
        80,     //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0000,//0x0010,        //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x0000,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x68,       //m_ucDimmingCtl
        0xDD,                //m_ucMaxPWMVal;
        0x20,//63,        //m_ucMinPWMVal;

        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x315F15,
       0x2343EB,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tPanelWSXGA_AU22_M201EW01[] =
{
    {
        "WSXGA_AU22_M201EW01",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        500,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        120,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        16,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        40,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        2,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        8,          //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        16+40,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        2+8,        //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1680,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1050,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2000,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        1840,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        1780,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1195,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1080,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1060,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (150),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (119),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (110),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;            //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;            //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0xA0,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x50,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x23E80F,
       0x1A54D8,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tPanelFullHD_CMO216_H1L01[] =
{
    {
        "FullHD_CMO216_H1L01",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A_HX)||(MS_BOARD_TYPE_SEL==BD_MST087E_D01A_HX))
	0,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE
#else
	1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE
#endif

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        8,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        8+16,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1130,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (150),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (120),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
#if ( (MS_BOARD_TYPE_SEL==BD_MST556A_T2_10A_HX)||(MS_BOARD_TYPE_SEL==BD_MST087E_D01A_HX))
	TI_10BIT_MODE,               //8bit ti bit mode
#else
	TI_8BIT_MODE,               //8bit ti bit mode
#endif
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x20EA0E,
       0x181568,
       E_PNL_CHG_VTOTAL,
    },
};
#if ENABLE_MFC
PanelType  tMFCFullHD120_CMO216_H1L01[] =
{
    {
        "URSA120_CMO216H1_L01",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_MFC,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        8,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        8+16,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1130,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (150),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x181568,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tMFCFullHD60_CMO216_H1L01[] =
{
    {
        "URSA60_CMO216H1_L01",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_MFC,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        8,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        8+16,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1130,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (150),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x181568,
       E_PNL_CHG_VTOTAL,
    },
};
#endif

PanelType  tPanelFULLHD_SEC32_LE32A[] =
{
    {
        "SEC_LE32A",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        0,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        8,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,          //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        8+16,            //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1130,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (150),      //DWORD m_dwPanelDCLK;          //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //0x0010,       //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,   //0x87,     //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,    //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue
       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x181568,
       E_PNL_CHG_VTOTAL,
    },
};

PanelType  tWUXGA_CMO_260J2[] =
{
    {
        "CMO260J2_WUXGA",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_LVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        1,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT
        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB
        PANEL_SWAP_LVDS_POL,
        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
        1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE

        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        600,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        300,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        30,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        6,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        26,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        6+26,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1200,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2159,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2141,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2060,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1245,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1235,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1209,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (170),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (158),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        100,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x015E,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x7D,       //m_ucDimmingCtl

        0xFF,        //m_ucMaxPWMVal;
        0x00,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

       TI_8BIT_MODE,               //8bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       1,          //   double clock
       0x1D0AC1,
       0x173BCE,
       E_PNL_CHG_VTOTAL,
    },
};

#if ENABLE_TCON
PanelType  tPanelFullHD_TCON[] =
{
    {
        "tPanelFullHD_TCON",//m_pPanelName
        //////////////////////////////////////////////
        // Panel output
        //////////////////////////////////////////////
        0,          //BOOL m_bPanelDither :1;           //PANEL_DITHER          // 8/6 bits panel
        LINK_MINILVDS,  //BOOL m_ePanelLinkType :2;         //PANEL_LINK

        0,          //BOOL m_bPanelDualPort :1;         //PANEL_DUAL_PORT

        1^PANEL_CONNECTOR_SWAP_PORT,                    // shall swap if
                                                        // (PANEL_SWAP_PORT XOR Board_Connect_Swap) is TRUE

        0,          //BOOL m_bPanelSwapOdd_ML   :1;     //PANEL_SWAP_ODD_ML
        0,          //BOOL m_bPanelSwapEven_ML  :1;     //PANEL_SWAP_EVEN_ML
        0,          //BOOL m_bPanelSwapOdd_RB   :1;     //PANEL_SWAP_ODD_RB
        0,          //BOOL m_bPanelSwapEven_RB  :1;     //PANEL_SWAP_EVEN_RB

        PANEL_SWAP_LVDS_POL,

        0,          //BOOL m_bPanelSwapLVDS_CH  :1;     //PANEL_SWAP_LVDS_CH
        PANEL_PDP_10BIT,          //BOOL m_bPanelPDP10BIT     :1;     //PANEL_PDP_10BIT
	    1,          //BOOL m_bPanelLVDS_TI_MODE :1;     //PANEL_LVDS_TI_MODE


        0x00,       //BYTE m_ucPanelDCLKDelay;          //PANEL_DCLK_DELAY
        0,          //BOOL m_bPanelInvDCLK  :1;         //PANEL_INV_DCLK
        0,          //BOOL m_bPanelInvDE        :1;     //PANEL_INV_DE
        0,          //BOOL m_bPanelInvHSync :1;         //PANEL_INV_HSYNC
        0,          //BOOL m_bPanelInvVSync :1;         //PANEL_INV_VSYNC

        ///////////////////////////////////////////////
        // Output tmming setting
        ///////////////////////////////////////////////
        // driving current setting (0x00=4mA, 0x01=6mA, 0x02=8mA, 0x03=12mA)
        0x01,       //BYTE m_ucPanelDCKLCurrent;        //PANEL_DCLK_CURRENT         // DCLK current
        0x01,       //BYTE m_ucPanelDECurrent;          //PANEL_DE_CURRENT           // DE signal current
        0x01,       //BYTE m_ucPanelODDDataCurrent;     //PANEL_ODD_DATA_CURRENT     // odd data current
        0x01,       //BYTE m_ucPanelEvenDataCurrent;    //PANEL_EVEN_DATA_CURRENT    // even data current

        30,         //BYTE m_ucPanelOnTiming1;          //PANEL_ON_TIMING1          // time between panel & data while turn on power
        200,        //BYTE m_ucPanelOnTiming2;          //PANEL_ON_TIMING2          // time between data & back light while turn on power
        220,        //BYTE m_ucPanelOffTiming1;         //PANEL_OFF_TIMING1         // time between back light & data while turn off power
        10,         //BYTE m_ucPanelOffTiming2;         //PANEL_OFF_TIMING2         // time between data & panel while turn off power

        32,         //BYTE m_ucPanelHSyncWidth;         //PANEL_HSYNC_WIDTH
        80,         //BYTE m_ucPanelHSyncBackPorch;     //PANEL_HSYNC_BACK_PORCH

        16,          //BYTE m_ucPanelVSyncWidth;         //PANEL_VSYNC_WIDTH
        16,         //BYTE m_ucPanelBackPorch;          //PANEL_VSYNC_BACK_PORCH

        32+80,      //WORD m_wPanelHStart;              //PANEL_HSTART             (PANEL_HSYNC_WIDTH + PANEL_HSYNC_BACK_PORCH)
        16+16,       //WORD m_wPanelVStart;              //PANEL_VSTART             (PANEL_VSYNC_WIDTH + PANEL_VSYNC_BACK_PORCH)

        1920,       //WORD m_wPanelWidth;               //PANEL_WIDTH
        1080,       //WORD m_wPanelHeight;              //PANEL_HEIGHT

        2360,       //WORD m_wPanelMaxHTotal;           //PANEL_MAX_HTOTAL
        2200,       //WORD m_wPanelHTotal;              //PANEL_HTOTAL
        2020,       //WORD m_wPanelMinHTotal;           //PANEL_MIN_HTOTAL

        1200,       //WORD m_wPanelMaxVTotal;           //PANEL_MAX_VTOTAL
        1135,       //WORD m_wPanelVTotal;              //PANEL_VTOTAL 20060511 chris :for Frame Lock operation
        1090,       //WORD m_wPanelMinVTotal;           //PANEL_MIN_VTOTAL

        (164),      //DWORD m_dwPanelMaxDCLK;           //PANEL_MAX_DCLK
        (149),      //DWORD m_dwPanelDCLK;              //PANEL_DCLK
        (136),      //DWORD m_dwPanelMinDCLK;           //PANEL_MIN_DCLK

        0x0019,     //m_wSpreadSpectrumStep;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)
        0x00C0,     //m_wSpreadSpectrumSpan;        //Value for Spread_Spectrum_Control register(B7..3:Period,B2..0:Amplitude)

        0x6D,       //m_ucDimmingCtl

        255,        //m_ucMaxPWMVal;
        0x20,//63,  //m_ucMinPWMVal;
        0,          //BOOL m_bPanelDeinterMode  :1;     //PANEL_DEINTER_MODE
        E_PNL_ASPECT_RATIO_WIDE,
        //
        //  Board related params.
        //
       (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L,            //MS_U16 m_u16LVDSTxSwapValue

	    TI_10BIT_MODE,             //10bit ti bit mode
       OUTPUT_10BIT_MODE,          //10bit ti bit mode
       0,          //   PANEL_SWAP_ODD_RG
       0,          //   PANEL_SWAP_EVEN_RG
       0,          //   PANEL_SWAP_ODD_GB
       0,          //   PANEL_SWAP_EVEN_GB
       0,          //   double clock
       0x1D0AC1,
       0x181568,
       E_PNL_CHG_HTOTAL,
    },
};
#endif

PanelType * tPanelIndexTbl[] =
{
    tPanelFULLHD_SEC32_LE32A,   // LE32A_FULLHD
    tPanelSXGA_AU17_EN05,
    tPanelWXGA_AU20_T200XW02,
    tPanelWXGAP_CMO_M190A1,     // 1440*900  //CHR_060912_1 Resolution 추가
    tPanelWSXGA_AU22_M201EW01,  // 1680*1050
    tPanelFullHD_CMO216_H1L01,  // 1920x1080
    tDACOUT480I_60,             // DAC 720*480
    tDACOUT480P_60,             // DAC 720*480
    tDACOUT576I_50,             // DAC 720*576
    tDACOUT576P_50,             // DAC 720*576
    tDACOUT720P_50,             // DAC 1280*720
    tDACOUT720P_60,             // DAC 1280*720
    tDACOUT1080I_50,            // DAC 1920*1080I 50MHz
    tDACOUT1080I_60,            // DAC 1920*1080I 60MHz
    tDACOUT1080P_24,            // DAC 1920*1080P 50MHz
    tDACOUT1080P_25,            // DAC 1920*1080P 50MHz
    tDACOUT1080P_30,            // DAC 1920*1080P 50MHz
    tDACOUT1080P_50,            // DAC 1920*1080P 50MHz
    tDACOUT1080P_60,            // DAC 1920*1080P 60MHz
    tDACOUT640X480P_60,         // DAC 640*480
    tWUXGA_CMO_260J2,           // CMO_260J2_WUXGA
#if ENABLE_MFC
    tMFCFullHD120_CMO216_H1L01,       // 1920x1080
    tMFCFullHD60_CMO216_H1L01,       // 1920x1080
#endif
#if ENABLE_TCON
    tPanelFullHD_TCON,             // TCON 1920x1080
#endif
};

typedef struct
{
    U16 ucX;
    U16 ucY;
}T_MS_NONLINEAR_POINT_16;//fix me

typedef struct
{
    T_MS_NONLINEAR_POINT_16     stPoint0;
    T_MS_NONLINEAR_POINT_16     stPoint1;
    T_MS_NONLINEAR_POINT_16     stPoint2;
    T_MS_NONLINEAR_POINT_16     stPoint3;
}T_MS_NONLINEAR_CURVE_16;  //fix me

static T_MS_NONLINEAR_CURVE_16 stBacklightCurveTbl =  //fix me
{
    {0,     0 },
    {0,    0 },
    {0,    0 },
    {0,   0 },
};


PanelType * MApi_XC_GetPanelSpec(PANEL_RESOLUTION_TYPE enResolutionType)
{
    return tPanelIndexTbl[(MS_U8)enResolutionType];
}

//--------
// panel
//--------
static void _msAPI_Scaler_SetPanelVCC(MS_BOOL bEnable)
{
    if(bEnable)
    {
        PANEL_CTL_On();
        //stSystemInfo[MAIN_WINDOW].u8PanelPowerStatus |= PANEL_POWER_VCC;
    }
    else
    {
        PANEL_CTL_Off();
        //stSystemInfo[MAIN_WINDOW].u8PanelPowerStatus &= ~PANEL_POWER_VCC;
    }
}

void MApi_PNL_En(MS_BOOL bPanelOn )
{
    //MS_U32 delayTime;

    if(bPanelOn)
    {
        /* enable/disable VCC */
        _msAPI_Scaler_SetPanelVCC(bPanelOn);
        //delayTime = g_IPanel.OnTiming(E_APIPNL_POWER_TIMING_1);//_getPanelOnTiming1();
        //MsOS_DelayTask(delayTime);
        g_IPanel.Enable(bPanelOn);

        // >>> Ken 20090914
        /*
        #if(MS_BOARD_TYPE_SEL == BD_MST087B_S7M_D01A_S)
        MDrv_Write2Byte(0x103240, 0x1100);
        MDrv_WriteByte(0x103264, 0x03);
        MDrv_Write2Byte(0x1032EE, 0x0000);
        MDrv_Write2Byte(0x1032F0, 0x0001);
        MDrv_WriteByte(0x1032EA, 0x01);
        #endif
        */
        // <<< Ken 20090914
    }
    else
    {
        g_IPanel.Enable(bPanelOn);
        //MsOS_DelayTask(20);
        /* enable/disable VCC */
        _msAPI_Scaler_SetPanelVCC(bPanelOn);
    }
}

/******************************************************************************/
///Enable/Disable Panel backlight
///@param bEnable \b IN:
///- TRUE: Enable
///- FALSE: Disable
/******************************************************************************/
#define MAX_BACKLIGHT    100
void MApi_PNL_SetInverter(MS_BOOL bEnable)
{
    if(bEnable)
    {
        INV_CTL_On();
        //stSystemInfo[MAIN_WINDOW].u8PanelPowerStatus |= PANEL_POWER_LIGHT_ON;
    }
    else
    {
        INV_CTL_Off();
        //stSystemInfo[MAIN_WINDOW].u8PanelPowerStatus &= ~PANEL_POWER_LIGHT_ON;
    }
}

U16 msAPI_CalNonLinearCurve_BackLight(BYTE AdjustValue, T_MS_NONLINEAR_CURVE_16* stNonLinearCurve )
{
    U16 rValue,ucY0,ucY1,ucX0,ucX1,ucIntercept;
    U16 wDistanceOfY, wDistanceOfX;

    if (AdjustValue < stNonLinearCurve->stPoint1.ucX)
    {
          ucY0 = stNonLinearCurve->stPoint0.ucY;
          ucY1 = stNonLinearCurve->stPoint1.ucY;
          ucX0 = stNonLinearCurve->stPoint0.ucX;
          ucX1 = stNonLinearCurve->stPoint1.ucX;
    }
    else if (AdjustValue < stNonLinearCurve->stPoint2.ucX)
    {
          ucY0 = stNonLinearCurve->stPoint1.ucY;
          ucY1 = stNonLinearCurve->stPoint2.ucY;
          ucX0 = stNonLinearCurve->stPoint1.ucX;
          ucX1 = stNonLinearCurve->stPoint2.ucX;
    }
    else //if (AdjustValue < stNonLinearCurve->stPoint3.ucX)
    {
          ucY0 = stNonLinearCurve->stPoint2.ucY;
          ucY1 = stNonLinearCurve->stPoint3.ucY;
          ucX0 = stNonLinearCurve->stPoint2.ucX;
          ucX1 = stNonLinearCurve->stPoint3.ucX;
    }
//    else
//    {
//          ucY0 = stNonLinearCurve->stPoint3.ucY;
//          ucY1 = stNonLinearCurve->stPoint4.ucY;
//          ucX0 = stNonLinearCurve->stPoint3.ucX;
//          ucX1 = stNonLinearCurve->stPoint4.ucX;
//    }

    if (ucY1 > ucY0)
    {
        wDistanceOfY = ucY1 - ucY0;
        wDistanceOfX = ucX1 - ucX0;
        ucIntercept  = ucY0;
        AdjustValue  = AdjustValue - ucX0;
    }
    else
    {
        wDistanceOfY = ucY0 - ucY1;
        wDistanceOfX = ucX1 - ucX0;
        ucIntercept  = ucY1;
        AdjustValue  = ucX1 - AdjustValue;
    }

  //printf("wDistanceOfY %u\n", wDistanceOfY);
  //printf("wDistanceOfX %u\n", wDistanceOfX);
  //printf("ucIntercept %bu\n", ucIntercept);
  //printf("AdjustValue %bu\n", AdjustValue);

    rValue = ((U16)wDistanceOfY*AdjustValue/(wDistanceOfX)) + ucIntercept;
    return rValue;
}

U16 msAPI_Mode_PictureBackLightN100toReallyValue ( U8 u8value )
{
    return msAPI_CalNonLinearCurve_BackLight(u8value, &stBacklightCurveTbl );  //fix me
}

void MApi_PNL_SetBackLight(MS_BOOL bEnable)
{
    U8 u8TransientBacklight;

    if ( bEnable )
    {
        ms_delay( g_IPanel.OnTiming(E_APIPNL_POWER_TIMING_2) );
        MApi_PNL_SetInverter(ENABLE);

        u8TransientBacklight = MIN(MAX_BACKLIGHT, MAX_BACKLIGHT);

        //Panel_Backlight_PWM_ADJ(msAPI_Mode_PictureBackLightN100toReallyValue( u8TransientBacklight ));
    #if 0 //desable by jp for reduce boot time
        U8 i;
        i = 0;
        msAPI_Scaler_SetPanelBacklight(ENABLE);
        ms_delay( 10 );
        Panel_Backlight_Max_Current(PANEL_DIMMING_CTL);

        g_bIsSoftStartValid = TRUE;

        while ( g_bIsSoftStartValid == TRUE )
        {
            if ( g_u8TransientBacklight < MAX_BACKLIGHT )
            {
                g_u8TransientBacklight += 5;
            }

            g_u8TransientBacklight = MIN(g_u8TransientBacklight, MAX_BACKLIGHT);

            msAPI_Scaler_SetBacklight( msAPI_Mode_PictureBackLightN100toReallyValue( g_u8TransientBacklight ));

            ms_delay( 10 );
            if ( g_u8TransientBacklight == MAX_BACKLIGHT )
            {
                g_bIsSoftStartValid = FALSE;

            }
        }
    #endif
    }
    else
    {
        //Panel_Backlight_PWM_ADJ(0);

        ms_delay( 5 );
        MApi_PNL_SetInverter(DISABLE);
    }
}

PANEL_RESOLUTION_TYPE MApi_PNL_GetPnlTypeSetting(void)
{
#if (ENABLE_PNL_FULLHD_SEC_LE32A)
        return PNL_FULLHD_SEC_LE32A;
#elif (ENABLE_PNL_SXGA_AU17_EN05)
        return PNL_SXGA_AU17_EN05;
#elif (ENABLE_PNL_WXGA_AU20_T200XW02)
        return PNL_WXGA_AU20_T200XW02;
#elif (ENABLE_PNL_WXGA_PLUS_CMO190_M190A1)
        return PNL_WXGA_PLUS_CMO190_M190A1;
#elif (ENABLE_PNL_WSXGA_AU22_M201EW01)
        return PNL_WSXGA_AU22_M201EW01;
#elif (ENABLE_PNL_FULLHD_CMO216_H1L01)
        return PNL_FULLHD_CMO216_H1L01;
#elif (ENABLE_PANEL_RES_FULL_HD)
        return PANEL_RES_FULL_HD;
#elif (ENABLE_PNL_WUXGA_CMO260_J2)
        return PNL_WUXGA_CMO260_J2;
#elif (ENABLE_MFC120_FULLHD_CMO216_H1L01)
        return MFC120_FULLHD_CMO216_H1L01;
#elif (ENABLE_MFC60_FULLHD_CMO216_H1L01)
        return MFC60_FULLHD_CMO216_H1L01;
#elif (ENABLE_PNL_DACOUT_576I)
        return DACOUT_576I;
#elif (ENABLE_PNL_DACOUT_576P)
        return DACOUT_576P;
#elif (ENABLE_PNL_DACOUT_720P_50)
        return DACOUT_720P_50;
#elif (ENABLE_PNL_DACOUT_1080I_50)
        return DACOUT_1080I_50;
#elif (ENABLE_PNL_DACOUT_1080P_50)
        return DACOUT_1080P_50;
#elif (ENABLE_PNL_DACOUT_480I)
        return DACOUT_480I;
#elif (ENABLE_PNL_DACOUT_480P)
        return DACOUT_480P;
#elif (ENABLE_PNL_DACOUT_720P_60)
        return DACOUT_720P_60;
#elif (ENABLE_PNL_DACOUT_1080I_60)
        return DACOUT_1080I_60;
#elif (ENABLE_PNL_DACOUT_1080P_60)
        return DACOUT_1080P_60;
#elif (ENABLE_PNL_FULLHD_TCON)
        return PNL_FULLHD_TCON;
#endif
}

E_OUTPUT_VIDEO_TIMING_TYPE MApi_XC_GetDACType(PANEL_RESOLUTION_TYPE enResolutionType)
{
    E_OUTPUT_VIDEO_TIMING_TYPE eTiming =
        (enResolutionType == DACOUT_480I) ? E_RES_720x480I_60Hz :
        (enResolutionType == DACOUT_480P) ? E_RES_720x480P_60Hz :
        (enResolutionType == DACOUT_576I) ? E_RES_720x576I_50Hz:
        (enResolutionType == DACOUT_576P) ? E_RES_720x576P_50Hz :
        (enResolutionType == DACOUT_720P_50) ? E_RES_1280x720P_50Hz:
        (enResolutionType == DACOUT_720P_60) ? E_RES_1280x720P_60Hz :
        (enResolutionType == DACOUT_1080I_50)? E_RES_1920x1080I_50Hz :
        (enResolutionType == DACOUT_1080I_60)? E_RES_1920x1080I_60Hz :
        (enResolutionType == DACOUT_1080P_24)? E_RES_1920x1080P_24Hz :
        (enResolutionType == DACOUT_1080P_25)? E_RES_1920x1080P_25Hz :
        (enResolutionType == DACOUT_1080P_30)? E_RES_1920x1080P_30Hz :
        (enResolutionType == DACOUT_1080P_50)? E_RES_1920x1080P_50Hz : E_RES_1920x1080P_60Hz;

    printf("DAC eTiming =%d\n", eTiming);
    return eTiming;
}

HDMITX_VIDEO_TIMING MApi_XC_GetHDMIType(PANEL_RESOLUTION_TYPE enResolutionType)
{
    HDMITX_VIDEO_TIMING eTiming =
        (enResolutionType == DACOUT_480I) ? HDMITX_RES_720x480i :
        (enResolutionType == DACOUT_480P) ? HDMITX_RES_720x480p :
        (enResolutionType == DACOUT_576I) ? HDMITX_RES_720x576i :
        (enResolutionType == DACOUT_576P) ? HDMITX_RES_720x576p :
        (enResolutionType == DACOUT_720P_50) ? HDMITX_RES_1280x720p_50Hz :
        (enResolutionType == DACOUT_720P_60) ? HDMITX_RES_1280x720p_60Hz :
        (enResolutionType == DACOUT_1080I_50)? HDMITX_RES_1920x1080i_50Hz :
        (enResolutionType == DACOUT_1080I_60)? HDMITX_RES_1920x1080i_60Hz :
        (enResolutionType == DACOUT_1080P_24)? HDMITX_RES_1920x1080p_24Hz :
        (enResolutionType == DACOUT_1080P_25)? HDMITX_RES_1920x1080p_25Hz :
        (enResolutionType == DACOUT_1080P_30)? HDMITX_RES_1920x1080p_30Hz :
        (enResolutionType == DACOUT_1080P_50)? HDMITX_RES_1920x1080p_50Hz : HDMITX_RES_1920x1080p_60Hz;

    printf("HDMITx eTiming =%d\n", eTiming);

    return eTiming;
}

void MApi_PNL_Dump(void)
{
    printf("PNL name:%s\n",g_IPanel.Name());
    printf("PNL HStart:%d\n",g_IPanel.HStart());
    printf("PNL VStart:%d\n",g_IPanel.VStart());
    printf("PNL Width:%d\n",g_IPanel.Width());
    printf("PNL Height:%d\n",g_IPanel.Height());
    printf("PNL HTotal:%d\n",g_IPanel.HTotal());
    printf("PNL VTotal:%d\n",g_IPanel.VTotal());
    printf("PNL HSynWidth:%d\n",g_IPanel.HSynWidth());
    printf("PNL HSynBackPorch:%d\n",g_IPanel.HSynBackPorch());
    printf("PNL VSynBackPorch:%d\n",g_IPanel.VSynBackPorch());
    printf("PNL DefaultVFreq:%d\n",g_IPanel.DefaultVFreq());
    printf("PNL LPLL_Mode:%d\n",g_IPanel.LPLL_Mode());
    printf("PNL LPLL_Type:%d\n",g_IPanel.LPLL_Type());
    printf("PNL AspectRatio:%d\n",g_IPanel.AspectRatio());
    printf("PNL MinSET:%d\n",g_IPanel.MinSET());
    printf("PNL MaxSET:%d\n",g_IPanel.MaxSET());
}

