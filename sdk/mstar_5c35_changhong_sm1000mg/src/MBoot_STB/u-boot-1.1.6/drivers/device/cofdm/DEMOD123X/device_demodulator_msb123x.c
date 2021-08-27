/*@ <FileComment ID=1246257763790> @*/
/**********************************************************************
Copyright (c) 2006-2009 MStar Semiconductor, Inc.
All rights reserved.
Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
MStar Semiconductor Inc. and be kept in strict confidence
(MStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of MStar Confidential
Information is unlawful and strictly prohibited. MStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
 * Class : mapi_demodulator_msb123x
 * File  : mapi_demodulator_msb123x.cpp
 **********************************************************************/

#include <common.h>
#include "sys/common/MsOS.h"
#include <sys/drv/drvGPIO.h>
#include "msb123x_dvbt.h"
#include <device/drvDemod.h>
#include <sys/drv/drvIIC.h>
#include "device_demodulator_msb123x.h"
#include <frontend_config.h>
#include <device/drvTuner.h>
#if(FRONTEND_TUNER_TYPE == TUNER_RT828)
#include "../../tuner/RT82X/drvTuner_R82x.h"
#endif

#if(FRONTEND_DEMOD_TYPE ==DEMOD_MSB123X)
#define TS_PARALLEL_OUTPUT 1
#define MSB123X_MDrv_Timer_Delayms MsOS_DelayTask
#define MSB123X_MDrv_Timer_Delayus MsOS_DelayTaskUs



#define WIMBLEDON_BOOT_START_ADDR     0x00000
#define WIMBLEDON_BOOT_END_ADDR       0x00FFF
#define WIMBLEDON_DVBT2_P1_START_ADDR 0x01000
#define WIMBLEDON_DVBT2_P1_END_ADDR   0x08FFF
#define WIMBLEDON_DVBT2_P2_START_ADDR 0x09000
#define WIMBLEDON_DVBT2_P2_END_ADDR   0x0FFFF
#define WIMBLEDON_DVBT_START_ADDR     0x10000
#define WIMBLEDON_DVBT_END_ADDR       0x17FFF
#define WIMBLEDON_DVBC_START_ADDR     0x18000
#define WIMBLEDON_DVBC_END_ADDR       0x1FFFF

#define WIMBLEDON_WINDOWS_BASE                0x100
#define WIMBLEDON_BOOT_WINDOWS_OFFSET         WIMBLEDON_WINDOWS_BASE
#define WIMBLEDON_DVBT2_P2_WINDOWS_OFFSET    (WIMBLEDON_WINDOWS_BASE + 0x08)
#define WIMBLEDON_DVBT2_P1_WINDOWS_OFFSET    (WIMBLEDON_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define WIMBLEDON_DVBT_WINDOWS_OFFSET        (WIMBLEDON_DVBT2_P1_WINDOWS_OFFSET + 0x08)
#define WIMBLEDON_DVBC_WINDOWS_OFFSET        (WIMBLEDON_DVBT_WINDOWS_OFFSET + 0x08)

/*@ </Definitions> @*/
MS_U8 u8MsbData[6];
#define DEMOD_MSB123X_SLAVE_ID          (0xD2)
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define SDRAM_BASE                               0x5000

#define MSB123x_BOOT  0x01
#define MSB123x_DVBT2 0x02
#define MSB123x_DVBT  0x04
#define MSB123x_DVBC  0x08
#define MSB123x_ALL   0x0F


#define MSB123X_DEBUG   1
#define ENABLEDEMODGPIO 0

#if MSB123X_DEBUG
#define PRINTE(p)       printf p
#else
#define PRINTE(p)
#endif

typedef enum
{
    E_DEVICE_DEMOD_ATV = 0,
    E_DEVICE_DEMOD_DVB_T,
    E_DEVICE_DEMOD_DVB_C,
    E_DEVICE_DEMOD_DVB_S,
    E_DEVICE_DEMOD_DTMB,
    E_DEVICE_DEMOD_ATSC,
    E_DEVICE_DEMOD_ATSC_VSB,
    E_DEVICE_DEMOD_ATSC_QPSK,
    E_DEVICE_DEMOD_ATSC_16QAM,
    E_DEVICE_DEMOD_ATSC_64QAM,
    E_DEVICE_DEMOD_ATSC_256QAM,
    E_DEVICE_DEMOD_DVB_T2,
    E_DEVICE_DEMOD_MAX,
    E_DEVICE_DEMOD_NULL = E_DEVICE_DEMOD_MAX,

} EN_DEVICE_DEMOD_TYPE;

//operation   DVBT
#define RFAGC_EN        0x00
#define HUMDET_EN       0x00
#define IIS_EN          0x01
#define CCI_EN          0x01
#define ACI_EN          0x01
#define AUTO_IQ_SWAP    0x01
#define MODE_CP_FORCED  0x00
#define TPS_FORCED      0x00
#define ZIF             0x00
#define BW              0x03
#define MODE            0x01
#define CP              0x03
#define LP_SEL          0x00
#define CSTL            0x00
#define HIER            0x00
#define HPCR            0x00
#define LPCR            0x00
#define ATV_SYSTEM      0x01
#define ICFO_RANGE      0x01
#define RFAGC_REF               0x64
#define IFAGC_REF_2K            0x4B
#define IFAGC_REF_8K            0x4B
#define IFAGC_REF_ACI           0x4B
#define DVBT_CRC                0x00
#define SFO_2K_H                0x00
#define SFO_2K_L                0xC8
#define SFO_8K_H                0x00
#define SFO_8K_L                0xC8
#define CHECK_CHANNEL           0x00
#define SNR_POS                 0x00
#define CCI_KP                  0x00
#define CCI_FSWEEP              0x00
#define TS_CLK_RATE_AUTO        0x00
#define IFAGC_REF_IIS_2K        0xA0
#define IFAGC_REF_IIS_8K        0xA0
#define IFAGC_ACI_DET_TH_L      0x9A
#define IFAGC_ACI_DET_TH_H      0x01
#define DVBT_IF_INV_PWM_OUT_EN  0x00
#if((FRONTEND_TUNER_TYPE == TUNER_RT828) || (FRONTEND_TUNER_TYPE == TUNER_RT820T)\
  ||(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219))
#define FRONTEND_DEMOD_IQ_SWAP     1
#else
#define FRONTEND_DEMOD_IQ_SWAP     0
#endif
#define IQ_SWAP         FRONTEND_DEMOD_IQ_SWAP


//DVBC
#define REAGC_EN            0x00
#define HUMDET_EN_C         0x01
#define AUTO_IQ             0x01
#define AUTO_SCAN_SYM_RATE  0x01
#define AUTO_SCAN_QAM       0x01
#define ATV_DET_EN          0x01
#define RSV_0B              0x00
#define RSV_0C              0x00
#define BW_L                0xDB
#define BW_H                0x1A
#define BW1_L               0xDB
#define BW1_H               0x1A
#define BW2_L               0xDB
#define BW2_H               0x1A
#define BW3_L               0xDB
#define BW3_H               0x1A
#define RSV_0x1F            0x00
#define QAM                 0x02
#define IQ_SWAP_C               0x01
#define CCI                 0x01

//configure
#define DCR_EN          0x01
#define IQB_EN          0x00
#define AUTO_RFMAX      0x00
#define AUTO_ACI          0x00
#define AUTO_SCAN       0x00
#define RSV_0D              0x00
#define RSV_0E              0x00
#define RSV_0F              0x00
#define FREQ                0x00
#define RSSI                0x00
#define FS_L                0x80
#define FS_H                0xBB
#define RFMAX               0x01
#if TS_PARALLEL_OUTPUT
#define SERIAL_TS               0x00
#else
#define SERIAL_TS               0x01
#endif
#define TS_CLK_SEL             0x06
#define TS_DATA_SWAP       0x00
#define TS_OUT_INV             TS_CLK_INV


#if(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
//IF =3950000
#define FC_L            0x82     //8M
#define FC_H            0xAA
#elif(FRONTEND_TUNER_TYPE == TUNER_NuTune_FT2125)
#define FC_L            0xE0
#define FC_H            0x2E
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212)
//IF=4000
#define FC_L            0xE0
#define FC_H            0xAB
#elif(FRONTEND_TUNER_TYPE == TUNER_MXL201RF)
//IF=4570
#define FC_L            0xA6
#define FC_H            0xA9
#elif((FRONTEND_TUNER_TYPE == TUNER_RT828) || (FRONTEND_TUNER_TYPE == TUNER_RT820T))
#define FC_L            0xA6
#define FC_H            0xA9  //7M AND 8M     IF=4.57MHZ

#define FC_L_6M              0x8E
#define FC_H_6M             0xAD    //6M     IF= 3.57MHZ
#elif(FRONTEND_TUNER_TYPE == TUNER_NMI120)
//IF=4800
#define FC_L            0xC0
#define FC_H            0xA8
#endif


//configure   DVBT2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL           0x04
// FC: FC = FS - IF = 48000 - 4500 = 0xA9EC     (4.50MHz IF)
// FC: FC = FS - IF = 48000 - 36000 = 0x2EE0    (36.0MHz IF)
#if(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
//IF =4570000
#define T2_FC_L_VAL            0xA6    //8M
#define T2_FC_H_VAL            0xA9
//IF =4350000
#define T2_FC_L_VAL_7M           0x82     //7M
#define T2_FC_H_VAL_7M           0xAA
#elif(FRONTEND_TUNER_TYPE == TUNER_NuTune_FT2125)
#define T2_FC_L_VAL            0xE0
#define T2_FC_H_VAL            0x2E
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212)
#define T2_FC_L_VAL            0xE0
#define T2_FC_H_VAL            0xAB
#elif(FRONTEND_TUNER_TYPE == TUNER_MXL201RF)
#define T2_FC_L_VAL            0xA6
#define T2_FC_H_VAL            0xA9
#elif(FRONTEND_TUNER_TYPE == TUNER_RT828)
#define T2_FC_L_VAL            0xA6
#define T2_FC_H_VAL            0xA9  //7M and 8M  IF=4.57MHZ

#define T2_FC_L_VAL_6M           0x8E
#define T2_FC_H_VAL_6M          0xAD    //6M  IF=3.57MHZ

#define T2_FC_L_VAL_1_7M        0x14
#define T2_FC_H_VAL_1_7M        0Xb4  // BW=1.7M IF =1.9MHZ
#elif(FRONTEND_TUNER_TYPE == TUNER_RT820T)
#define T2_FC_L_VAL            0xA6
#define T2_FC_H_VAL            0xA9  //7M and 8M  IF=4.57MHZ
#define T2_FC_L_VAL_6M           0x8E
#define T2_FC_H_VAL_6M          0xAD    //6M  IF=3.57MHZ
#elif(FRONTEND_TUNER_TYPE == TUNER_NMI120)
#define T2_FC_L_VAL            0xC0
#define T2_FC_H_VAL            0xA8
#endif
#if TS_PARALLEL_OUTPUT
#define T2_TS_SERIAL_VAL       0x00
#define T2_TS_OUT_INV_VAL      0x01
#define T2_TS_DATA_SWAP_VAL    0x00
#else
#define T2_TS_SERIAL_VAL       0x01
#define T2_TS_OUT_INV_VAL      0x00
#define T2_TS_DATA_SWAP_VAL    0x01
#endif
#define T2_TS_CLK_RATE_VAL     0x06
#define T2_TS_ERR_POL_VAL      0x00
#define T2_IF_AGC_INV_PWM_EN_VAL    0x00

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

//DVBC
MS_U8 MSB1231_DVBC_DSPREG_TABLE[] =
{
    REAGC_EN,       HUMDET_EN_C,        DCR_EN,     IQB_EN,         AUTO_IQ,        AUTO_RFMAX,
    AUTO_ACI,       AUTO_SCAN,      AUTO_SCAN_SYM_RATE,         AUTO_SCAN_QAM,  ATV_DET_EN,
    RSV_0B,         RSV_0C,         RSV_0D,     RSV_0E,         RSV_0F,
    RSSI,           FREQ,           FC_L,       FC_H,           FS_L,           FS_H,
    BW_L,           BW_H,           BW1_L,      BW1_H,          BW2_L,          BW2_H,
    BW3_L,          BW3_H,          RSV_0x1F,
    RFMAX,          QAM,            IQ_SWAP_C,  CCI,            SERIAL_TS,      TS_CLK_SEL,
    TS_OUT_INV,     TS_DATA_SWAP
};

//DVBT
MS_U8 MSB1231_DVBT_DSPREG_TABLE[] = // andy 2009-12-15 ?? 3:55:03 TW model
{
    RFAGC_EN,     HUMDET_EN,    DCR_EN,     IIS_EN,         CCI_EN,      ACI_EN,
    IQB_EN,       AUTO_IQ_SWAP, AUTO_RFMAX, AUTO_ACI,       MODE_CP_FORCED, TPS_FORCED,
    AUTO_SCAN,    RSV_0D,       RSV_0E,     RSV_0F,
    RSSI,         ZIF,          FREQ,       FC_L,           FC_H,        FS_L,      FS_H,
    BW,           MODE,         CP,         LP_SEL,         CSTL,        HIER,      HPCR,
    LPCR,         IQ_SWAP,      RFMAX,      ATV_SYSTEM,     ICFO_RANGE,  RFAGC_REF,
    IFAGC_REF_2K, IFAGC_REF_8K, IFAGC_REF_ACI,  IFAGC_REF_IIS_2K,  IFAGC_REF_IIS_8K, IFAGC_ACI_DET_TH_L,
    IFAGC_ACI_DET_TH_H,         SERIAL_TS,  TS_CLK_SEL,  TS_OUT_INV,  TS_DATA_SWAP,
    SFO_2K_H,     SFO_2K_L,     SFO_8K_H,   SFO_8K_L,       CHECK_CHANNEL,          SNR_POS,
    CCI_KP,       CCI_FSWEEP    , TS_CLK_RATE_AUTO, DVBT_IF_INV_PWM_OUT_EN, DVBT_CRC
};

//DVBT2
MS_U8 MSB1231_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL,     T2_FC_L_VAL,     T2_FC_H_VAL
};

//
S_RFAGC_SSI RFAGC_SSI[] =
{
    { -19.40,    0x1B},
    { -20.40,    0x1C},
    { -21.40,    0x1D},
    { -22.40,    0x1E},
    { -23.40,    0x1F},
    { -24.40,    0x20},
    { -25.20,    0x21},
    { -26.60,    0x22},
    { -28.00,    0x23},
    { -29.20,    0x24},
    { -31.50,    0x25},
    { -32.75,    0x26},
    { -34.00,    0x27},
    { -35.25,    0x28},
    { -36.75,    0x29},
    { -38.50,    0x2a},
    { -40.00,    0x2b},
    { -42.00,    0x2c},
    { -43.50,    0x2d},
    { -44.80,    0x2e},
    { -46.20,    0x2f},
    { -47.40,    0x30},
    { -48.00,    0x31},
    { -48.40,    0x32},
    { -49.00,    0x33},
    { -99.00,    0x3f},
};

#if(FRONTEND_TUNER_TYPE == TUNER_MXL201RF)
S_IFAGC_SSI IFAGC_SSI[] =
{
    { -43,   0x65},
    { -44,   0x67},
    { -45,   0x68},
    { -46,   0x69},
    { -47,   0x6b},
    { -48,   0x6c},
    { -49,   0x6e},
    { -50,   0x6f},
    { -51,   0x71},
    { -52,   0x72},
    { -53,   0x74},
    { -54,   0x75},
    { -55,   0x77},
    { -56,   0x78},
    { -57,   0x7a},
    { -58,   0x7c},
    { -59,   0x7d},
    { -60,   0x7f},
    { -61,   0x80},
    { -62,   0x82},
    { -63,   0x83},
    { -64,   0x84},
    { -65,   0x86},
    { -66,   0x87},
    { -67,   0x89},
    { -68,   0x8a},
    { -69,   0x8b},
    { -70,   0x8d},
    { -71,   0x8e},
    { -72,   0x90},
    { -73,   0x91},
    { -74,   0x93},
    { -75,   0x94},
    { -76,   0x96},
    { -77,   0x97},
    { -78,   0x99},
    { -79,   0x9a},
    { -80,   0x9c},
    { -90,   0xaa},
    { -99,   0xFF},
};

S_IFAGC_SSI T2IFAGC_SSI[] =
{
    {0.0   ,     0x00},   //default
    { -45.0,    0x69},
    { -50.0,    0x70},
    { -55.0,    0x78},
    { -60.0,    0x80},
    { -65.0,    0x87},
    { -70.0,    0x8e},
    { -75.0,    0x95},
    { -80.0,    0x9d},
    { -85.0,    0xa3},
    { -90.0,    0xa9},
    { -95.0,    0xb1},
    { -99.0,    0xff}  //default
};

S_SSI_PREF SsiPref[] =
{
    {_QPSK,  _CR1Y2, -94.0},
    {_QPSK,  _CR2Y3, -92.0},
    {_QPSK,  _CR3Y4, -91.0},
    {_QPSK,  _CR5Y6, -90.0},
    {_QPSK,  _CR7Y8, -89.0},

    {_16QAM, _CR1Y2, -88.0},
    {_16QAM, _CR2Y3, -86.0},
    {_16QAM, _CR3Y4, -85.0},
    {_16QAM, _CR5Y6, -84.0},
    {_16QAM, _CR7Y8, -83.0},

    {_64QAM, _CR1Y2, -82.0},
    {_64QAM, _CR2Y3, -81.0},
    {_64QAM, _CR3Y4, -80.0},
    {_64QAM, _CR5Y6, -79.0},
    {_64QAM, _CR7Y8, -78.0},
};
#elif(FRONTEND_TUNER_TYPE == TUNER_RT828)
S_IFAGC_SSI IFAGC_SSI[] =
{
    { -46,   0x19},
    { -47,   0x29},
    { -48,   0x2e},
    { -49,   0x33},
    { -50,   0x37},
    { -51,   0x3b},
    { -52,   0x3f},
    { -53,   0x43},
    { -54,   0x47},
    { -55,   0x4b},
    { -56,   0x4f},
    { -57,   0x53},
    { -58,   0x56},
    { -59,   0x5a},
    { -60,   0x5d},
    { -61,   0x60},
    { -62,   0x63},
    { -63,   0x67},
    { -64,   0x6a},
    { -65,   0x6d},
    { -66,   0x71},
    { -67,   0x74},
    { -68,   0x78},
    { -69,   0x7b},
    { -70,   0x7e},
    { -71,   0x81},
    { -72,   0x84},
    { -73,   0x86},
    { -74,   0x89},
    { -75,   0x8b},
    { -76,   0x8d},
    { -77,   0x8f},
    { -78,   0x91},
    { -79,   0x93},
    { -80,   0x95},
    { -81,   0x98},
    { -82,   0x9a},
    { -83,   0x9d},
    { -84,   0xa0},
    { -85,   0xa3},
    { -99,   0xFF},
};

S_IFAGC_SSI T2IFAGC_SSI[] =
{
    {0.0   ,     0x00},   //default
    { -45.0,    0x25},
    { -50.0,    0x40},
    { -55.0,    0x55},
    { -60.0,    0x66},
    { -65.0,    0x77},
    { -70.0,    0x88},
    { -75.0,    0x93},
    { -80.0,    0x9f},
    { -85.0,    0xae},
    { -90.0,    0xc2},
    { -95.0,    0xed},
    { -99.0,    0xff}   //default
};

S_SSI_PREF SsiPref[] =
{
    {_QPSK,  _CR1Y2, -97.0},
    {_QPSK,  _CR2Y3, -95.0},
    {_QPSK,  _CR3Y4, -94.0},
    {_QPSK,  _CR5Y6, -93.0},
    {_QPSK,  _CR7Y8, -92.0},

    {_16QAM, _CR1Y2, -91.0},
    {_16QAM, _CR2Y3, -89.0},
    {_16QAM, _CR3Y4, -88.0},
    {_16QAM, _CR5Y6, -87.0},
    {_16QAM, _CR7Y8, -86.0},

    {_64QAM, _CR1Y2, -85.0},
    {_64QAM, _CR2Y3, -84.0},
    {_64QAM, _CR3Y4, -82.0},
    {_64QAM, _CR5Y6, -81.0},
    {_64QAM, _CR7Y8, -80.0},
};

#else
S_IFAGC_SSI IFAGC_SSI[] =
{
    { -48,   0x3A},
    { -49,   0x3B},
    { -50,   0x3C},
    { -51,   0x3E},
    { -52,   0x40},
    { -53,   0x43},
    { -54,   0x45},
    { -55,   0x47},
    { -56,   0x49},
    { -57,   0x4C},
    { -58,   0x4f},
    { -59,   0x51},
    { -60,   0x54},
    { -61,   0x56},
    { -62,   0x59},
    { -63,   0x5C},
    { -64,   0x5F},
    { -65,   0x62},
    { -66,   0x65},
    { -67,   0x69},
    { -68,   0x6E},
    { -69,   0x73},
    { -70,   0x78},
    { -71,   0x7F},
    { -72,   0x85},
    { -73,   0x8b},
    { -74,   0x9c},
    { -85,   0xc0},
    { -99,   0xFF},
};

S_IFAGC_SSI T2IFAGC_SSI[] =
{
    {0.0   ,     0x00},   //default
    { -45.0,    0x69},
    { -50.0,    0x70},
    { -55.0,    0x78},
    { -60.0,    0x80},
    { -65.0,    0x87},
    { -70.0,    0x8e},
    { -75.0,    0x95},
    { -80.0,    0x9d},
    { -85.0,    0xa3},
    { -90.0,    0xa9},
    { -95.0,    0xb1},
    { -99.0,    0xff}  //default
};

S_SSI_PREF SsiPref[] =
{
    {_QPSK,  _CR1Y2, -81.0},
    {_QPSK,  _CR2Y3, -79.0},
    {_QPSK,  _CR3Y4, -78.0},
    {_QPSK,  _CR5Y6, -77.0},
    {_QPSK,  _CR7Y8, -76.0},

    {_16QAM, _CR1Y2, -75.0},
    {_16QAM, _CR2Y3, -73.0},
    {_16QAM, _CR3Y4, -72.0},
    {_16QAM, _CR5Y6, -71.0},
    {_16QAM, _CR7Y8, -70.0},

    {_64QAM, _CR1Y2, -69.0},
    {_64QAM, _CR2Y3, -68.0},
    {_64QAM, _CR3Y4, -66.0},
    {_64QAM, _CR5Y6, -65.0},
    {_64QAM, _CR7Y8, -64.0},
};
#endif

typedef struct
{
    MS_BOOL     bInited;
    MS_BOOL    bOpen;
    MS_BOOL     bLoaded;
    EN_DEVICE_DEMOD_TYPE  enDemodType;
    MS_S32     s32_MSB123X_Mutex;
    MS_S32     s32_MSB123X_RW_Mutex;
    MS_S32    s32_MSB123X_RW2_Mutex;
    MS_U8       u8MsbData[6];
    MS_U8       u8SdramCode;
    MS_U8       u8sramCode;
    MS_U8     *pMSB123XLib;
    MS_U32     u32MSB123XlibSize;
    MS_U8     *pMSB123X_RegDVBT;
    MS_U8     *pMSB123X_RegDVBT2;
    MS_U32     u32CurrFreq;
    MS_U8       u8CurrPlpID;
    MS_U8       u8CurrScanType;
    DEMOD_EN_TER_BW_MODE     enCurrBW;
    DEMOD_EN_TER_BW_MODE     enTuneBw;
    //MS_BOOL   bCheckLayer;
    MS_U8       PlpIDSize;
    MS_U32     u32ChkScanTimeStart;
    MS_BOOL   bFECLock;
    //MS_BOOL   bDoReset;
    MS_U8       u8ScanStatus;
} MDvr_CofdmDmd_CONFIG;
MDvr_CofdmDmd_CONFIG MSB123X;
typedef enum
{
    E_RF_CH_BAND_6MHz = 0x01,
    E_RF_CH_BAND_7MHz = 0x02,
    E_RF_CH_BAND_8MHz = 0x03,
    E_RF_CH_BAND_INVALID
} RF_CHANNEL_BANDWIDTH;

MS_U8 u8Type, u8Plp;

#define REG_MB_CNTL     0x0C00
#define REG_MB_ADDR_L   0x0C02
#define REG_MB_ADDR_H   0x0C03
#define REG_MB_DATA     0x0C04

static float g_msb123x_fSNR = 0.0;
MS_BOOL MDrv_msb123x_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    return MDrv_IIC_WriteBytes(u16BusNumSlaveID, AddrCnt, pu8addr, u16size, pBuf);
}

MS_BOOL MDrv_msb123x_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    return MDrv_IIC_ReadBytes(u16BusNumSlaveID, AddrCnt, pu8addr, u16size, pBuf);

}

/****************************************************************************
Subject:    Function providing approx. result of Log10(X)
Function:   Log10Approx
Parmeter:   Operand X in float
Return:     Approx. value of Log10(X) in float
Remark:      Ouput range from 0.0, 0.3 to 9.6 (input 1 to 2^32)
 *******:**********************************************************************/
static float Log10Approx(float flt_x)
{
    MS_U32 u32_temp = 1;
    MS_U8 indx = 0;
    do
    {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }while (++indx < 32);
    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 * indx;
}

MS_BOOL MSB123X_I2C_CH_Reset(MS_U8 ch_num, MS_BOOL enable)
{
    MS_BOOL bRet  = TRUE;
    MS_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);

    PRINTE(("[start],chan %d\n", ch_num));
    if (enable)       //in 7853,always execute this
    {
        data[0] = 0x53;
        bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 5);
        PRINTE(("return %s\n", bRet ? "TRUE" : "FALSE"));
    }
    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = (ch_num & 0x01) ? 0x81 : 0x80;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = (ch_num & 0x02) ? 0x83 : 0x82;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = (ch_num & 0x04) ? 0x85 : 0x84;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    PRINTE(("[end] return %d\n", bRet));
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);

    return bRet;
}

/*@ <Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
MS_BOOL MSB123X_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet  = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);
    pMSB123X->u8MsbData[0] = 0x10;
    pMSB123X->u8MsbData[1] = 0x00;
    pMSB123X->u8MsbData[2] = 0x00;
    pMSB123X->u8MsbData[3] = (u16Addr >> 8) & 0xff;
    pMSB123X->u8MsbData[4] = u16Addr & 0xff;
    pMSB123X->u8MsbData[5] = u8Data;

    pMSB123X->u8MsbData[0] = 0x35;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);

    pMSB123X->u8MsbData[0] = 0x10;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 6);

    pMSB123X->u8MsbData[0] = 0x34;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);
    //PRINTE(("[%s] return %d\n",__FUNCTION__,bRet));
    return    bRet;
}

MS_BOOL MSB123X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8MsbData[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) & 0xff;
    u8MsbData[4] = u16Addr & 0xff;
    // u8MsbData[5] = 0x00;

    for (idx = 0; idx < data_size ; idx++)
    {
        u8MsbData[5 + idx] = u8pData[idx];
    }

    u8MsbData[0] = 0x35;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, u8MsbData, 1);

    u8MsbData[0] = 0x10;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, u8MsbData, 5 + data_size);

    u8MsbData[0] = 0x34;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, u8MsbData, 1);

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);

    return    bRet;

}

MS_BOOL MSB123X_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL ret = TRUE;
    ret &= MSB123X_WriteReg(u16Addr, (MS_U8)u16Data & 0x00ff);
    ret &= MSB123X_WriteReg(u16Addr + 0x0001, (MS_U8)(u16Data >> 8) & 0x00ff);
    return ret;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6fcf> @*/
MS_BOOL MSB123X_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);

    pMSB123X->u8MsbData[0] = 0x10;
    pMSB123X->u8MsbData[1] = 0x00;
    pMSB123X->u8MsbData[2] = 0x00;
    pMSB123X->u8MsbData[3] = (u16Addr >> 8) & 0xff;
    pMSB123X->u8MsbData[4] = u16Addr & 0xff;

    pMSB123X->u8MsbData[0] = 0x35;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);

    pMSB123X->u8MsbData[0] = 0x10;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 5);
    bRet &= MDrv_msb123x_IIC_Read(DEMOD_MSB123X_SLAVE_ID, 0, 0, pu8Data, 1);

    pMSB123X->u8MsbData[0] = 0x34;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);
    return bRet;

}


MS_BOOL MSB123X_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8     status = TRUE;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW2_Mutex, MSOS_WAIT_FOREVER);
    status &= MSB123X_WriteReg(REG_MB_DATA, u8Data);
    status &= MSB123X_WriteReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    status &= MSB123X_WriteReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    status &= MSB123X_WriteReg(REG_MB_CNTL, 0x04);

    do
    {
        status &= MSB123X_ReadReg(REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            PRINTE(("MSB123X_MB_WRITE_FAILURE!!!\n"));
            MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW2_Mutex);
            return FALSE;
        }
    }while (cntl != 0xff);
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW2_Mutex);
    return status;
}

void  MSB123X_Reset(void)
{
    MS_BOOL bRet = TRUE;
    MS_U8     u8Data = 0x00;
    MS_U32    u32Retry = 0x00;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return ;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return ;
    }

    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {

        // mask miu access for all and mcu
        bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x7f);
        bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        MSB123X_MDrv_Timer_Delayus(10);

        MSB123X_WriteReg(0x0B00 + (0x10) * 2, 0x01);
        MSB123X_WriteReg(0x0B00 + (0x19) * 2, 0x03);
        MSB123X_WriteReg(0x0C00, 0x00);             //clear MB_CNTL
        MSB123X_MDrv_Timer_Delayus(5 * 100);
        // enable miu access of mcu gdma
        bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        MSB123X_MDrv_Timer_Delayus(10);


        MSB123X_WriteReg(0x0B00 + (0x19) * 2, 0x00);
        MSB123X_WriteReg(0x0B00 + (0x10) * 2, 0x00);
        MSB123X_MDrv_Timer_Delayus(5 * 1000);
        MSB123X_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready
        while (0xff != u8Data)
        {
            MSB123X_MDrv_Timer_Delayus(5 * 1000);
            MSB123X_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready

            if (u32Retry++ > 200)
            {
                PRINTE((">>>MSB123x: Reset Fail!\n"));
                break;
            }
        }
    }
    else if ((pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        // mask miu access for all and mcu
        MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1 , 0x7f);
        MSB123X_WriteReg(0x1200 + (0x23) * 2  , 0xfe);
        MSB123X_MDrv_Timer_Delayus(10);
        MSB123X_WriteReg(0x0B00 + (0x19) * 2, 0x01);
        MSB123X_WriteReg(0x0B00 + (0x10) * 2, 0x01);
        MSB123X_WriteReg(0x0C00, 0x00);           //clear MB_CNTL
        MSB123X_MDrv_Timer_Delayus(5 * 1000);

        // enable miu access of mcu gdma
        MSB123X_WriteReg(0x1200 + (0x23) * 2  , 0xf0);

        MSB123X_MDrv_Timer_Delayus(10);
        MSB123X_WriteReg(0x0B00 + (0x10) * 2, 0x00);
        MSB123X_WriteReg(0x0B00 + (0x19) * 2, 0x00);
        MSB123X_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            MSB123X_MDrv_Timer_Delayus(5 * 1000);
            MSB123X_ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                PRINTE((">>>MSB123x: Reset Fail!\n"));
                break;
            }
        }
    }
    pMSB123X->u32ChkScanTimeStart = MsOS_GetSystemTime();
}

// 144 ms roughly
MS_BOOL MSB123X_HW_init(void)
{

    MS_BOOL bRet = TRUE;
    MS_U8 u8_tmp = 0;
    MS_U8 u8_timeout = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!\n"));
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);

    PRINTE(("[start]MSB123x_HW_init\n"));

    // mask miu access of mcu
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x7f);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xfe);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    // Reset MCU
    bRet &= MSB123X_WriteReg2bytes(0x0b00 + (0x19) * 2, 0x0003);

    bRet &= MSB123X_WriteReg2bytes(0x1200, 0x0000);

    // miu software reset for pwr down tolerance
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c01);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c00);

    // -------------------------------------------------------------------
    // Initialize DMD_ANA_MISC
    // -------------------------------------------------------------------

    // [1:0]    reg_mpll_loop_div_first
    // [15:8]    reg_mpll_loop_div_second
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x33) * 2, 0x0902);

    // [2:0]    reg_mpll_ictrl
    // [3]    reg_mpll_in_sel
    // [4]    reg_mpll_xtal2adc_sel
    // [5]    reg_mpll_xtal2next_pll_sel
    // [6]    reg_mpll_vco_offset
    // [7]    reg_mpll_pd
    // [8]    reg_xtal_en
    // [10:9]    reg_xtal_sel
    // [11]     reg_mpll_porst
    // [12]     reg_mpll_reset
    // [13]     reg_pd_dmpll_clk
    // [14]     reg_pd_3p3_1
    // [15]     reg_pd_3p3_2

    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x35) * 2, 0x1804);
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x35) * 2, 0x0004);

    // [0]    reg_mpll_clk_dp_pd    dummy
    // [1]    reg_adc_clk_pd        ADC output clock power down
    // [2]    reg_mpll_div2_pd    MPLL_DIV2 power down
    // [3]    reg_mpll_div3_pd    MPLL_DIV3 power down
    // [4]    reg_mpll_div4_pd    MPLL_DIV4 power down
    // [5]    reg_mpll_div8_pd    MPLL_DIV8 power down
    // [6]    reg_mpll_div10_pd    MPLL_DIV10 power down
    // [13:8]  reg_mpll_adc_div_sel select the ADC clock divide ratio,ADC clk=XTAL_IN * (LOOP_DIV_FIRST*LOOP_DIV_SECOND)/div_ratio

    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x30) * 2, 0x1200);

    // -------------------------------------------------------------------
    // Begin Initialize Initialize ADC I/Q
    // -------------------------------------------------------------------
    // [0]    Q channel ADC power down
    // [1]    I channel ADC power down
    // [2]    Q channel clamp enable. 0:enable, 1:disable
    // [3]    I channel clamp enable. 0:enable, 1:disable
    // [6:4]    I channel input mux control;
    //        3'b000=I channel ADC calibration mode input
    //            3'b001=VIF signal from VIFPGA
    //            3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //            all the other combination are only for test mode, don't use without understanding.
    // [10:8]    Q channel input mux control;
    //        3'b000=Q channel ADC calibration mode input
    //            3'b001=VIF signal from VIFPGA 3'b010 = SSIF signal from PAD_SIFP(M)
    //            3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //            all the other combination are only for test mode, don't use without understanding.
    // [12] ADC I,Q swap enable; 1: swap
    // [13] ADC clock out select; 1: ADC_CLKQ
    // [14] ADC linear calibration bypass enable; 1:enable
    // [15] ADC internal 1.2v regulator control always 0 in T3
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x01) * 2, 0x0440);


    // [0]    Channel I ADC power down: 1=power dwon
    // [1]    Channel Q ADC power down: 1=power dwon
    // [2]    power down clamp buffer for test mode
    // [3]    change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x0c) * 2, 0x0002);

    // [3:0]    clamp voltage control
    //            3'b000 = 0.7v
    //            3'b001 = 0.75v
    //            3'b010 = 0.5v
    //            3'b011 = 0.4v
    //            3'b100 = 0.8v
    //            3'b101 = 0.9v
    //            3'b110 = 0.65v
    //            3'b111 = 0.60v
    // [4]    REFERENCE power down
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x20) * 2, 0x0000);

    // Set ADC gain is 1
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x0b) * 2, 0x0909);

    // Disable ADC Sign bit
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x2e) * 2, 0x0200);

    // ADC I channel offset
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x2a) * 2, 0x0c00);

    // ADC Q channel offset
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x2b) * 2, 0x0c00);
    // -------------------------------------------------------------------
    // End Initialize Initialize ADC I/Q
    // -------------------------------------------------------------------

    // [15:0]    reg_mpll_test
    // [4]    mpll lock detector enable
    bRet &= MSB123X_WriteReg2bytes(0x0A00 + (0x34) * 2, 0x0010);

    // @0x0919
    // [5:0]    reg_ckg_spif
    // [15:8]    reg_ckg_miu
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x19) * 2, 0x3c0c);


    // @0x091b
    // [2:0]    reg_ckg_dvbt2_outer1x
    // [6:4]    reg_ckg_dvbt2_outer2x
    // [10:8]    reg_ckg_syn_miu
    // [14:12]    reg_ckg_syn_ts
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x1b) * 2, 0x0000);

    /*Beg initialize MIU*/

    // dummy register setting
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0000);

    // dummy register setting
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0000);

    // dummy register setting
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0000);

    // miu software reset
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c00);

    // APLL PORST
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x11) * 2, 0x60cc);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x11) * 2, 0x00cc);

    // set frequence 180MHz
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x11) * 2, 0x0000);



    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x10) * 2, 0x4200); //0x4200 //0x4220 NG //0x4a20 for IQ.TS debugg
    // bRet &= MSB123X_WriteReg2bytes(0x1200+(0x10)*2, 0x3a20);  // miu b/w issue djb fail, michael huang


    // 0x4220 -> 0x5220 for hi/low temp test.

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x12) * 2, 0x4000);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x13) * 2, 0x0500);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x16) * 2, 0x0030);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x01) * 2, 0x8100);
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x01) * 2, 0xe100);
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x01) * 2, 0x8100);

    // in_phase
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x02) * 2, 0x0371);

    // tREFRESH                      : [7:0] * 16
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x03) * 2, 0x0040);

    // reg_tRAS                      : [3:0]         // 48.00 / 6.0ns = 8
    // reg_tRC                         : [7:4]         // 90.00 / 7.5ns = 12
    // reg_tRCD                      : [11:8]         // (min  12.50 ns) / 2.5ns = 5   // 5
    // reg_tRCD                      : [11:8]    // 22.50 / 7.5ns = 3
    // reg_tRP                         : [15:12]         // 22.50 / 7.5ns = 3
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x04) * 2, 0x44c8);


    // reg_tRRD                      : [3:0]         3
    // tWR                             : [7:4]         2
    // reg_tMRD                      : [11:8]         2
    // reg_tRTP                      : [15:12]    0
    // reg_tRAS                      : [3:0]         // 48.00 / 6.0ns = 8
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x05) * 2, 0x0233);


    // reg_w2r_dly(tWTR)             : [3:0]         3
    // reg_w2r_oen_dly                 : [7:4]         6
    // reg_r2w_dly                     : [11:8]         6
    // reg_r2w_oen_dly                 : [15:12]         8
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x06) * 2, 0x5634);

    // tRFC                          : [5:0]         e
    // reg_tRAS[4]                     : [6]             0
    // reg_tRC[4]                     : [7]             0
    // reg_write_latency             : [10:8]         1
    // reg_tCCD                      : [15:14]         1
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x07) * 2, 0x410e);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0e) * 2, 0x1800);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x1b) * 2, 0x0161); // 0x0167);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x1c) * 2, 0x00b0);

    /*Beg SIM SPIF initialize*/

    // mask other request
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x23) * 2, 0x0000);

    // reg_rq0_round_robin : [0]
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x20) * 2, 0xc001);

    // miu software reset
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c01);


    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c00);

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x01) * 2, 0x010d);

    /*Beg Initial DRAM start here!!!*/

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x00) * 2, 0x0001);

    // wait init_done

    bRet &= MSB123X_ReadReg(0x1201, &u8_tmp);
    PRINTE(("MIU wait init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));
    while (u8_tmp != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            PRINTE(("MIU init failure...\n"));
            MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
            return FALSE;
        }
        // 10us delay
        MSB123X_MDrv_Timer_Delayus(10);
        bRet &= MSB123X_ReadReg(0x1201, &u8_tmp);
    }
    PRINTE(("MIU init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));
    /*Beg set MIU self test parameter*/

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x71) * 2, 0x0000);

    // length
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x72) * 2, 0x0010);

    // test data
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x74) * 2, 0x5aa5);

    // reg_test_en                     : [0]
    // reg_test_mode                 : [2:1] 00 : address,
    //                                         01 : test data
    //                                         10 : shift data
    // reg_inv_data                  : [3]
    // reg_test_loop                 : [4]
    // reg_force_out                 : [5]
    // reg_force_in                  : [6]
    // reg_read_only                 : [8]
    // reg_write_only                 : [9]
    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x70) * 2, 0x0001);

    // wait for test done
    bRet &= MSB123X_ReadReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);
    PRINTE(("MIU wait test done, u8_tmp=0x%x,bRet=0x%x\n", u8_tmp, bRet));
    while (u8_tmp != 0x80)
    {
        bRet &= MSB123X_ReadReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);
    }
    // test done.
    // test result check
    bRet &= MSB123X_ReadReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);

    if (u8_tmp == 0x40)
    {
        PRINTE(("@msb123x, error, MIU self test fail !!!!\n"));
        bRet = FALSE;
    }
    else
    {
        PRINTE(("@msb123x, MIU self test successful.\n"));
    }

    bRet &= MSB123X_WriteReg2bytes(0x1200 + (0x70) * 2, 0x0000);

    /*End initialize MIU*/

    /* Beg Turn on pads */
    // ------Turn off all pad in
    // [0] reg_set_pad_low
    // [1] reg_set_pad_high
    // [2] reg_set_i2cs_pad_low
    // [3] reg_set_i2cs_pad_high
    // [8] reg_turn_off_pad
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x28) * 2, 0x0000);


    // ------IF AGC PWM pad on
    // [0]        reg_rf_agc_pad_oen
    // [1]        reg_if_agc_pad_oen
    // [9:8]    reg_rf_agc_sel[1:0]
    // [11:10]    reg_if_agc_sel[1:0]
    // [13:12]    reg_rf_agc_ctrl_sel[1:0] 1:PWN on 2:UP GPIO on
    // [15:14]    reg_if_agc_ctrl_sel[1:0] 1:PWN on 2:DOWN GPIO on
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x2a) * 2, 0x8001);

    // ------I2CM pad on
    // [1:0]    reg_iicm_pad_sel[1:0]    1:iicm enable 2:UART enable
    // [4]        reg_i2c_sbpm_en     1: enable I2CS bypass to I2CM function
    // [12:8]    reg_i2c_sbpm_idle_num[4:0]    a: default
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x08) * 2, 0x0a01);

    // ------Transport Stream pad on (except TS ERR pad)
    // [15:0]    reg_en_ts_pad[15:0] 0x00ff:normal TS location 0xff00:reverse TS location
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x2d) * 2, 0x00ff);

    // ------Transport Stream pad on (TS ERR pad)
    // [0]        reg_en_ts_err_pad    1: enable
    // [4]        reg_ts_err_pol    1: inverse 0:normal
    bRet &= MSB123X_WriteReg2bytes(0x0900 + (0x2e) * 2, 0x0001);


    // Initialize special registers

    bRet &= MSB123X_WriteReg(0x0C0E, 0x00);
    bRet &= MSB123X_WriteReg(0x0C0F, 0x00);
    bRet &= MSB123X_WriteReg(0x0C10, 0x00);
    bRet &= MSB123X_WriteReg(0x0C11, 0x00);

    PRINTE(("[end]MSB123x_HW_init, bRet=0x%x\n", bRet));

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return bRet;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm71bd> @*/
MS_BOOL MSB123X_IIC_Bypass_Mode(MS_BOOL enable)
{
    MS_U8 u8Retry = 10;

    //FRA_T2_BRINGUP
    while (u8Retry--)
    {
        if (enable)
            MSB123X_WriteReg(0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            MSB123X_WriteReg(0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off
    }
    return TRUE;
}

MS_BOOL MSB123X_ReadDspReg(MS_U16 u16Addr, MS_U8* pData)
{
    MS_U8     status = TRUE;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW2_Mutex, MSOS_WAIT_FOREVER);

    status &= MSB123X_WriteReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    status &= MSB123X_WriteReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    status &= MSB123X_WriteReg(REG_MB_CNTL, 0x03);

    do
    {
        status &= MSB123X_ReadReg(REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            PRINTE(("MSB123X_MB_READ_FAILURE !!!\n"));
            MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW2_Mutex);
            return FALSE;
        }
    }while (cntl != 0xff);

    status &= MSB123X_ReadReg(REG_MB_DATA, pData);
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW2_Mutex);
    return status;
}


MS_U8   MSB123X_DTV_DVBT_DSPReg_CRC(void)
{
    MS_U8 crc = 0;
    MS_U8 idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    for (idx = 0; idx < ((MS_U8)DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        crc ^= pMSB123X->pMSB123X_RegDVBT[idx];
    }

    crc = ~crc;

    return crc;
}

void   MSB123X_DTV_DVBT_DSPReg_ReadBack(void)
{
    MS_U8 ret = 0;
    MS_U8 idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return ;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return ;
    }

    for (idx = T_OPMODE_RFAGC_EN; idx < (MS_U8)DVBT_PARAM_LEN; idx++)
    {
        MSB123X_ReadDspReg(idx, &ret);
        PRINTE(("@msb123x, idx=0x%x, dsp=0x%x, sw=0x%x\n", idx, ret, pMSB123X->pMSB123X_RegDVBT[idx - (MS_U8)T_OPMODE_RFAGC_EN]));
    }
}

MS_BOOL   MSB123X_DTV_DVBT_DSPReg_Init(void)
{
    MS_U8    idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
#if(FRONTEND_TUNER_TYPE == TUNER_RT828)
    if (DEMOD_BW_MODE_6MHZ == pMSB123X->enTuneBw)  //6M
    {
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_FC_L - (MS_U8)T_OPMODE_RFAGC_EN] = FC_L_6M;
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_FC_H - (MS_U8)T_OPMODE_RFAGC_EN] = FC_H_6M;
    }
    else
    {
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_FC_L - (MS_U8)T_OPMODE_RFAGC_EN] = FC_L;
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_FC_H - (MS_U8)T_OPMODE_RFAGC_EN] = FC_H;
    }
#endif

    for (idx = 0; idx < ((MS_U8)DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        if (MSB123X_WriteDspReg(idx + (MS_U8)T_OPMODE_RFAGC_EN, pMSB123X->pMSB123X_RegDVBT [idx]) != TRUE)
        {
            PRINTE(("dsp reg init NG\n"));
            return FALSE;
        }
    }

    pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MSB123X_DTV_DVBT_DSPReg_CRC();

    if (MSB123X_WriteDspReg((MS_U8)T_PARAM_CHECK_SUM, pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]) != TRUE)
    {
        PRINTE(("dsp reg write crc NG\n"));
        return FALSE;
    }

    PRINTE(("dsp reg init ok\n"));

    return TRUE;
}

/*@ <Operation ID=Im17018142m1221763cc7cmm464e> @*/
MS_BOOL  MSB123X_Active(MS_BOOL bEnable)
{
    MS_U8  status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }
    else if ((pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        status = MSB123X_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
        if (status)
        {
            return  TRUE;
        }
        else
        {
            PRINTE((" @MSB1231_Active NG\n"));
            return FALSE;
        }
    }
    return FALSE;
}

MS_U32 MSB123X_DTV_GetBER(void)
{
    MS_U8         status = TRUE;
    MS_U8         reg, reg_frz;
    MS_U16            BitErrPeriod;
    MS_U32            BitErr;
    MS_U16            PktErr;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }

    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }
    else if ((pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        /////////// Post-Viterbi BER /////////////

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB123X_ReadReg(0x1132, &reg_frz);
        status &= MSB123X_WriteReg(0x1132, reg_frz | 0x80);

        // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= MSB123X_ReadReg(0x1131, &reg);
        BitErrPeriod = reg;

        status &= MSB123X_ReadReg(0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 7 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 7 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24

        status &= MSB123X_ReadReg(0x113D, &reg);
        BitErr = reg;

        status &= MSB123X_ReadReg(0x113C, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB123X_ReadReg(0x113B, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB123X_ReadReg(0x113A, &reg);
        BitErr = (BitErr << 8) | reg;

#if 1
        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //             [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB123X_ReadReg(0x113F, &reg);
        PktErr = reg;

        status &= MSB123X_ReadReg(0x113E, &reg);
        PktErr = (PktErr << 8) | reg;
#endif

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB123X_WriteReg(0x1132, reg_frz);

        if (BitErrPeriod == 0)  //protect 0
            BitErrPeriod = 1;

#if 0
        if (BitErr <= 0)
            *ber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
        else
            *ber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);


        printf("MSB PostVitBER = %8.3e \n ", *ber);
        printf("MSB PktErr = %d \n ", (int)PktErr);
#endif

        if (status)
        {
            return  TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

EN_FRONTEND_SIGNAL_CONDITION MSB123X_DTV_GetSNR(void)
{
    MS_U32  snr_out = 0;
    MS_U16  snr_ave_num = 0;
    MS_U8   reg = 0;
    EN_FRONTEND_SIGNAL_CONDITION   fe_falg ;
    float   f_snr = (float)0.0;
    float   fSNRDivider[] = {1024 * 2, 2048 * 2, 4096 * 2, 8192 * 2, 16384 * 2, 32768 * 2, 32768 * 2, 32768 * 2};

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    MS_U8 status = TRUE;
    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:

        // freeze
        status &= MSB123X_ReadReg(0x2802, &reg);
        reg |= 0x02;
        status &= MSB123X_WriteReg(0x2802, reg);

        // load
        status &= MSB123X_ReadReg(0x2802, &reg);
        reg |= 0x04;
        status &= MSB123X_WriteReg(0x2802, reg);

        status &= MSB123X_ReadReg(0x280D, &reg);
        snr_out = reg;
        status &= MSB123X_ReadReg(0x280C, &reg);
        snr_out = (snr_out << 8) | reg;
        status &= MSB123X_ReadReg(0x280B, &reg);
        snr_out = (snr_out << 8) | reg;
        status &= MSB123X_ReadReg(0x280A, &reg);
        snr_out = (snr_out << 8) | reg;
        snr_out &= 0x1FFFFFFF;
        if (snr_out == 0)
        {
            snr_out = 1;
        }
        status &= MSB123X_ReadReg(0x2805, &reg);
        snr_ave_num = (reg >> 5) & 0x07;

        // unfreeze
        status &= MSB123X_ReadReg(0x2802, &reg);
        reg &= (0xff - 0x02);
        status &= MSB123X_WriteReg(0x2802, reg);

        f_snr = (float)10.0 * Log10Approx((float)snr_out / fSNRDivider[snr_ave_num]);

        //PRINTE(("[DTV_GetSNR]snr_out=0x%lx, snr_ave_num=0x%x,fSNRDivider[snr_ave_num]=%f\n",snr_out,snr_ave_num,fSNRDivider[snr_ave_num]));
        g_msb123x_fSNR = f_snr;
        if (f_snr > 42) f_snr = 42;
        if (f_snr < 0) f_snr = 0;
        //PRINTE(("[dvbt2]snr=%f, snr_out=%ld, snr_ave_num=%d\n",fsnr,snr_out,snr_ave_num));
        if (f_snr > 25)
            fe_falg = E_FE_SIGNAL_STRONG;
        else if (f_snr > 20)
            fe_falg = E_FE_SIGNAL_MODERATE;
        else if (f_snr > 15)
            fe_falg = E_FE_SIGNAL_WEAK;
        else
            fe_falg = E_FE_SIGNAL_NO;
        break;
    default:
        fe_falg = E_FE_SIGNAL_NO;
        break;
    }
    return fe_falg;
}

float MSB123X_DTV_GetSignalQuality(void)
{
    MS_U8 status = TRUE;
    MS_U8 reg = 0;            // reg_frz;
    MS_U16 BitErrPeriod = 0;
    MS_U32 BitErr = 0;
    MS_U16 FecType = 0;
    float fber = (float)0.0;
    float log_ber = (float)0.0;
    float  SQI = (float)0.0;
    static float  PrevSQI = (float)0.0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        PRINTE(("\nE_DEVICE_DEMOD_DVB_T2\n"));
        /////////// Check lock status /////////////
        MSB123X_ReadDspReg(0x00f0, &reg);
        if ((reg & BIT7) == 0x00)
        {
            return 0;
        }

        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB123X_WriteReg(0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB123X_ReadReg(0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB123X_ReadReg(0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x32 Data BER count[15:0]
        // bank 38 0x33 Data BER count[31:16]
        status &= MSB123X_ReadReg(0x2667, &reg);
        BitErr = reg;
        status &= MSB123X_ReadReg(0x2666, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB123X_ReadReg(0x2665, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB123X_ReadReg(0x2664, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB123X_WriteReg(0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
            BitErrPeriod = 1;

        status &= MSB123X_ReadReg(0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB123X_ReadReg(0x278e, &reg);    //FEC Type[8:7]
        FecType = (FecType << 8) | reg;
        if (FecType & 0x0180)
            fber = (float)BitErr / (BitErrPeriod * 64800);
        else
            fber = (float)BitErr / (BitErrPeriod * 16200);

        PRINTE(("MSB123x Data BER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            PRINTE(("MSB123X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }

        log_ber = (-1) * Log10Approx((double)(1.0 / fber));
        if (log_ber <= (- 3.3))
        {
            SQI = 100.0;
        }
        else if (log_ber <= (-3.1))
        {
            SQI = (90 + (((- 3.1) - log_ber) / ((- 3.1) - (- 3.3)) * (100 - 90)));
        }
        else if (log_ber < (- 2.9))
        {
            SQI = (80 + (((- 2.9) - log_ber) / ((- 2.9) - (- 3.1)) * (90 - 80)));
        }
        else if (log_ber < (- 2.7))
        {
            SQI = (70 + (((- 2.7) - log_ber) / ((- 2.7) - (- 2.9)) * (80 - 70)));
        }
        else if (log_ber < (- 2.5))
        {
            SQI = (60 + (((- 2.5) - log_ber) / ((- 2.5) - (- 2.7)) * (70 - 60)));
        }
        else if (log_ber < (- 2.3))
        {
            SQI = (50 + (((- 2.3) - log_ber) / ((- 2.3) - (- 2.5)) * (60 - 50)));
        }
        else if (log_ber < (- 2.1))
        {
            SQI = (40 + (((- 2.1) - log_ber) / ((- 2.1) - (- 2.3)) * (50 - 40)));
        }
        else if (log_ber < (- 1.9))
        {
            SQI = (30 + (((- 1.9) - log_ber) / ((- 1.9) - (- 2.1)) * (40 - 30)));
        }
        else if (log_ber < (- 1.7))
        {
            SQI = (20 + (((- 1.7) - log_ber) / ((- 1.7) - (- 1.9)) * (30 - 20)));
        }
        else if (log_ber < (- 1.5))
        {
            SQI = (10 + (((- 1.5) - log_ber) / ((- 1.5) - (- 1.7)) * (20 - 10)));
        }
        else if (log_ber < (- 1.3))
        {
            SQI = (0 + (((- 1.3) - log_ber) / ((- 1.3) - (-1.5)) * (10 - 0)));
        }
        else
            SQI = 0;

        if (PrevSQI <= 0.0)
        {
            PrevSQI = SQI;
        }
        else
        {
            SQI = (SQI + PrevSQI) / 2.0;
            PrevSQI = SQI;
        }

        return SQI;
        break;
    }

    case E_DEVICE_DEMOD_DVB_T:
    case E_DEVICE_DEMOD_DVB_C:
    default:
    {
        float fber = 0.0;
        double log_ber = 0.0;
        MS_U8 u8Data = 0;
        if (MSB123X_ReadReg(0x112C, &u8Data) == FALSE)
        {
            PRINTE(("MSB123X_DTV_GetSignalQuality MSB123X_ReadReg fail!!! \n"));
            return 0;
        }
        if (u8Data & 0x02) /*BIT1*/
        {
            MS_U8 status = TRUE;
            MS_U8 reg, reg_frz;
            MS_U16 BitErrPeriod;
            MS_U32 BitErr;
            MS_U16 PktErr;
            /////////// Post-Viterbi BER /////////////
            // bank 7 0x32 [7] reg_bit_err_num_freeze

            status &= MSB123X_ReadReg(0x1132, &reg_frz);
            status &= MSB123X_WriteReg(0x1132, reg_frz | 0x80);
            // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
            //             [15:8] reg_bit_err_sblprd_15_8
            status &= MSB123X_ReadReg(0x1131, &reg);
            BitErrPeriod = reg;
            status &= MSB123X_ReadReg(0x1130, &reg);
            BitErrPeriod = (BitErrPeriod << 8) | reg;
            // bank 7 0x3a [7:0] reg_bit_err_num_7_0
            //             [15:8] reg_bit_err_num_15_8
            // bank 7 0x3c [7:0] reg_bit_err_num_23_16
            //             [15:8] reg_bit_err_num_31_24
            status &= MSB123X_ReadReg(0x113D, &reg);
            BitErr = reg;
            status &= MSB123X_ReadReg(0x113C, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= MSB123X_ReadReg(0x113B, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= MSB123X_ReadReg(0x113A, &reg);
            BitErr = (BitErr << 8) | reg;
            // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
            //             [15:8] reg_uncrt_pkt_num_15_8
            status &= MSB123X_ReadReg(0x113F, &reg);
            PktErr = reg;
            status &= MSB123X_ReadReg(0x113E, &reg);
            PktErr = (PktErr << 8) | reg;
            // bank 7 0x32 [7] reg_bit_err_num_freeze
            status &= MSB123X_WriteReg(0x1132, reg_frz);
            if (BitErrPeriod == 0)
                //protect 0
                BitErrPeriod = 1;
            if (BitErr <= 0)
                fber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
            else
                fber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);
            PRINTE(("MSB123x PostVitBER = %8.3e \n ", fber));
            PRINTE(("MSB123x PktErr = %d \n ", (int)PktErr));

            if (status == FALSE)
            {
                PRINTE(("MSB123X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                return 0;
            }

            log_ber = (-1) * Log10Approx((double)(1.0 / fber));
            if (log_ber <= (- 7.0))
            {
                SQI = 100;    //*quality = 100;
            }
            else if (log_ber < -6.0)
            {
                SQI = (90 + (((- 6.0) - log_ber) / ((- 6.0) - (- 7.0)) * (100 - 90)));
            }
            else if (log_ber < -5.5)
            {
                SQI = (80 + (((- 5.5) - log_ber) / ((- 5.5) - (- 6.0)) * (90 - 80)));
            }
            else if (log_ber < -5.0)
            {
                SQI = (70 + (((- 5.0) - log_ber) / ((- 5.0) - (- 5.5)) * (80 - 70)));
            }
            else if (log_ber < -4.5)
            {
                SQI = (60 + (((- 4.5) - log_ber) / ((-4.5) - (- 5.0)) * (70 - 50)));
            }
            else if (log_ber < -4.0)
            {
                SQI = (50 + (((- 4.0) - log_ber) / ((- 4.0) - (- 45)) * (60 - 50)));
            }
            else if (log_ber < -3.5)
            {
                SQI = (40 + (((- 3.5) - log_ber) / ((- 3.5) - (- 4.0)) * (50 - 40)));
            }
            else if (log_ber < -3.0)
            {
                SQI = (30 + (((- 3.0) - log_ber) / ((- 3.0) - (- 3.5)) * (40 - 30)));
            }
            else if (log_ber < -2.5)
            {
                SQI = (20 + (((- 2.5) - log_ber) / ((- 2.5) - (-3.0)) * (30 - 20)));
            }
            else if (log_ber < -1.9)
            {
                SQI = (0 + (((- 1.9) - log_ber) / ((- 1.9) - (- 2.5)) * (20 - 0)));
            }
            else
            {
                SQI = 0;
            }

            if (PrevSQI <= 0.0)
            {
                PrevSQI = SQI;
            }
            else
            {
                SQI = (SQI + PrevSQI) / 2.0;
                PrevSQI = SQI;
            }
            return SQI;
        }
        else
        {
            return 0;    //*quality = 0;
        }
        break;
    }
    }  // end of switch
    return 0;
}

MS_U16 MSB123X_DTV_GetSignalStrength(void)
{
    MS_U8    status = TRUE;
    MS_U8    reg_tmp = 0;
    MS_U8    rf_agc_val = 0;
    MS_U8    if_agc_val = 0;
    MS_U8    i = 0;
    MS_U8    ssi_tbl_len = 0;
    MS_U8    tps_cnstl = 0;
    MS_U8    tps_cr = 0;
    MS_U8    ssi = 0;
    MS_U8    u8RFAGC;
    //MS_U16   ypIFAGC1 = 0, ypIFAGC2 = 0;
    float    ch_power_db = 0;
    float    ch_power_if = 0;
    float    ch_power_ref = 0;
    float    ch_power_rel = 0;
    float    ch_power_rf = 0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (pMSB123X->enDemodType)
    {

    case E_DEVICE_DEMOD_DVB_T2:
    {
        u8RFAGC = 150;
        //PRINTE((">>> RFAGC = %d <<<\n", (int)u8RFAGC));
        if (u8RFAGC < 89)
        {
            ch_power_rf = -45;
        }
        else if (u8RFAGC < 111)
        {
            ch_power_rf = (-45) - (float)((u8RFAGC - 89) * 5) / (111 - 89);
        }
        else if (u8RFAGC < 149)
        {
            ch_power_rf = (-50) - (float)((u8RFAGC - 111) * 5) / (148 - 111);
        }
        else
        {
            ch_power_rf = -55;
        }
        // IFAGC
        // debug select
        MSB123X_WriteReg(0x3e00 + 0x13 * 2, 0x03);
        MSB123X_WriteReg(0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB123X_WriteReg(0x3e00 + 0x02 * 2 + 1, 0x80);

        // agc gain2 24 Bit
        //ReadReg(0x3e00+0x14*2+1, &reg_tmp);
        MSB123X_ReadReg(0x3e00 + 0x14 * 2 + 2, &reg_tmp);
        //ypIFAGC2 = reg_tmp;
        MSB123X_ReadReg(0x3e00 + 0x14 * 2 + 1, &reg_tmp);
        //ypIFAGC1 = reg_tmp;
        MSB123X_ReadReg(0x3e00 + 0x14 * 2 + 0, &reg_tmp);
        //ypIFAGC1 = (ypIFAGC1 << 8) | reg_tmp;
        //printf("=======>[YP] IFAGC_2 = %x, IFAGC_1 = %x\n", ypIFAGC2, ypIFAGC1);
        MSB123X_ReadReg(0x3e00 + 0x14 * 2 + 1, &reg_tmp);

        MSB123X_WriteReg(0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB123X_WriteReg(0x3e00 + 0x02 * 2 + 1, 0x00);
        //PRINTE((">>> reg_tmp = %x <<<\n", (int)reg_tmp));
        if_agc_val = reg_tmp;
        PRINTE((">>> if_agc_val = 0x%x <<<\n", (int)if_agc_val));

        for (i = 1; i < sizeof(T2IFAGC_SSI) / sizeof(S_IFAGC_SSI); i++)
        {
            if (if_agc_val < T2IFAGC_SSI[i].agc_val)
            {
                ch_power_if = T2IFAGC_SSI[i].power_db - (float)((if_agc_val - T2IFAGC_SSI[i - 1].agc_val) * 5) / (T2IFAGC_SSI[i].agc_val - T2IFAGC_SSI[i - 1].agc_val);
                break;
            }
        }

        if (ch_power_rf > (-55.0 + 0.5))
        {
            ch_power_db = ch_power_rf;
        }
        else if (ch_power_if < (-55.0 - 0.5))
        {
            ch_power_db = ch_power_if;
        }
        else
        {
            ch_power_db = (ch_power_if + ch_power_rf) / 2;
        }

        PRINTE(("ch_power_rf = %f\n", ch_power_rf));
        PRINTE(("ch_power_if = %f\n", ch_power_if));
        PRINTE(("ch_power_db = %f\n", ch_power_db));
        // -50 dB ~ -95 dB     ->   100 ~ 0

        if (ch_power_db > -50)
        {
            ssi = 100;
        }
        else if (ch_power_db < -95)
        {
            ssi = 0;
        }
        else
        {
            ssi = 100 - ((-50 - ch_power_db) * 2.22);
        }
        PRINTE((">>> SSI = %d <<<\n", (int)ssi));
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    case E_DEVICE_DEMOD_DVB_C:
    {
        ///////// RF/IF-AGC Gain Out /////////////
        rf_agc_val = 0x3f;

        // ==== IFAGC_GAIN_OUT (GAIN2) ====

        // reg_agc_debug_sel[3:0]
        MSB123X_ReadReg(0x3e26, &reg_tmp);
        reg_tmp = (reg_tmp & 0xf0) | 0x03; // select ifagc_gain
        MSB123X_WriteReg(0x3e26, reg_tmp);

        // reg_agc_gain2_out
        // use only high byte value
        MSB123X_ReadReg(0x3e29, &reg_tmp);
        if_agc_val = reg_tmp;
        PRINTE(("\n IF arg=0x%x,%d\n\n", if_agc_val, if_agc_val));

#if(FRONTEND_TUNER_TYPE != TUNER_MXL201RF)
        float    ch_power_rf = 0;
        for (i = 0; i < sizeof(RFAGC_SSI) / sizeof(S_RFAGC_SSI); i++)
        {
            if (rf_agc_val <= RFAGC_SSI[i].sar3_val)
            {
                ch_power_rf = RFAGC_SSI[i].power_db;
                break;
            }
        }
#endif

        ssi_tbl_len = sizeof(IFAGC_SSI) / sizeof(S_IFAGC_SSI);

        for (i = 0; i < ssi_tbl_len; i++)
        {
            if (if_agc_val <= IFAGC_SSI[i].agc_val)
            {
                ch_power_if = IFAGC_SSI[i].power_db;
                break;
            }
        }
#if (FRONTEND_TUNER_TYPE == TUNER_MXL201RF)
        ch_power_db = ch_power_if;
#else
        ch_power_db = (ch_power_rf > ch_power_if) ? ch_power_rf : ch_power_if;
#endif

        ///////// Get Constellation and Code Rate to determine Ref. Power //////////
        ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////
        status &= MSB123X_ReadReg(0x0f24, &reg_tmp);
        tps_cnstl = reg_tmp & 0x03;

        status &= MSB123X_ReadReg(0x0f24 + 1, &reg_tmp);
        tps_cr = (reg_tmp & 0x70) >> 4;

        for (i = 0; i < sizeof(SsiPref) / sizeof(S_SSI_PREF); i++)
        {
            if ((tps_cnstl == SsiPref[i].constel)
                    && (tps_cr == SsiPref[i].code_rate))
            {
                ch_power_ref = SsiPref[i].p_ref;
                break;
            }
        }
        if (i >= sizeof(SsiPref) / sizeof(S_SSI_PREF))
            ch_power_ref = SsiPref[sizeof(SsiPref) / sizeof(S_SSI_PREF) - 1].p_ref;

        ch_power_rel = ch_power_db - ch_power_ref;
        if (ch_power_rel < -15.0)
            ssi = 0.0;
        else if (ch_power_rel < 0.0)
            ssi = (ch_power_rel + 15.0) * (10.0 / 15.0);
        else if (ch_power_rel < 20.0)
            ssi = ch_power_rel * 4.0 + 10.0;
        else if (ch_power_rel < 35.0)
            ssi = (ch_power_rel - 20.0) * (10.0 / 15.0) + 90.0;
        else
            ssi = 100.0;
        PRINTE((">>>T SSI = %d <<<\n", (int)ssi));
    }
    break;
    default:
        ssi = 0;
        break;
    }
    return ssi;
}

void  MSB123X_Driving_Control(MS_BOOL bEnable)
{
    MS_U8 u8Temp;
    MSB123X_ReadReg(0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    MSB123X_WriteReg(0x0958, u8Temp);

    MSB123X_ReadReg(0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp & (~0x0F);
    }
    MSB123X_WriteReg(0x0959, u8Temp);
}

MS_BOOL MSB123X_DTV_Serial_Control(MS_BOOL bEnable)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);
    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        //FRA_T2_BRINGUP
    }
    else if ((pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        // set serial/parallel mode
        if (bEnable == 1) //enable serial control
        {
            if (MSB123X_WriteDspReg((MS_U16)T_CONFIG_TS_SERIAL, 1) != TRUE)
            {
                PRINTE(("T dsp reg write NG\n"));
            }
            if (MSB123X_WriteDspReg((MS_U16)T_CONFIG_TS_OUT_INV, 0) != TRUE)
            {
                PRINTE(("T dsp reg write NG\n"));
            }
            pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_TS_SERIAL - (MS_U8)T_OPMODE_RFAGC_EN] = 1;
            pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_TS_OUT_INV - (MS_U8)T_OPMODE_RFAGC_EN] = 0;
        }
        else  //enable parallel mode
        {
            if (MSB123X_WriteDspReg((MS_U16)T_CONFIG_TS_SERIAL, 0) != TRUE)
            {
                PRINTE(("T dsp reg write NG\n"));
            }
            if (MSB123X_WriteDspReg((MS_U16)T_CONFIG_TS_OUT_INV, 1) != TRUE)
            {
                PRINTE(("T dsp reg write NG\n"));
            }
            pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_TS_SERIAL - (MS_U8)T_OPMODE_RFAGC_EN] = 0;
            pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_TS_OUT_INV - (MS_U8)T_OPMODE_RFAGC_EN] = 1;
        }
    }
    MSB123X_Driving_Control(1);
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_U16  MSB123X_Lock(EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
{
    // printf("%s(),%d\n",__FUNCTION__,__LINE__);
    MS_U16        u16Address = 0;
    MS_U8         cData = 0;
    MS_U8         cBitMask = 0;
    MS_U16        Ret = 0;
    MS_U8         use_dsp_reg = 0;//addy update 0805

    switch (eStatus)
    {
        //############################ DVB-C ###############################
    case COFDM_FEC_LOCK_DVBC:
        //Update reg information
        MSB123X_ReadReg(0x11E0, &cData);//addy update 0805
        if (cData == 0x0C)
        {
            Ret = TRUE;
        }
        else
        {
            Ret = FALSE;
        }

        return Ret;
        break;
    case COFDM_TR_LOCK_DVBC://addy update 0805
        u16Address =  0x2250; //DVB-C : TR Lock,
        cBitMask = BIT0;
        break;
        //############################ DVB-T ###############################
    case COFDM_FEC_LOCK_DVBT:
        //Update reg information
        MSB123X_ReadReg(0x11E0, &cData);//addy update 0805
        if (cData == 0x0B)
        {
            return TRUE;
        }
        else
            return FALSE;
        break;
    case COFDM_PSYNC_LOCK:
        u16Address =  0x112C; //FEC: P-sync Lock,ok
        cBitMask = BIT1;
        break;
    case COFDM_TPS_LOCK:
        u16Address =  0x0f22; //TPS Lock,ok
        cBitMask = BIT1;
        break;
    case COFDM_TPS_LOCK_HISTORY:
        // change to use dsp reg ok
        use_dsp_reg = 1;
        u16Address =  0x00F0; //TPS lock history,// andy 2009-9-28 ?? 7:20:03
        cBitMask = BIT3;
        break;
    case COFDM_DCR_LOCK:
        u16Address =  0x3E45; //DCR Lock,ok
        cBitMask = BIT0;
        break;
    case COFDM_AGC_LOCK:
        u16Address =  0x3E2F; //AGC Lock,ok
        cBitMask = BIT0;
        break;
    case COFDM_MODE_DET:
        u16Address =  0x0ECF; //Mode CP Detect,ok
        cBitMask = BIT4;
        break;
    case COFDM_LOCK_STABLE_DVBT: //0923 update
        // change to use dsp reg
        use_dsp_reg = 1;
        u16Address =  0x00F0; //Lock Stable,
        cBitMask = BIT6;
        break;
    case COFDM_SYNC_LOCK_DVBT://addy 0629
        // change to use dsp reg
        use_dsp_reg = 1;
        u16Address =  0x00F0; //Valid sync,
        cBitMask = BIT2;
        break;
    case COFDM_FAST_LOCK_DVBT://addy 0920
        // change to use dsp reg
        use_dsp_reg = 1;
        u16Address =  0x00F0; //Valid sync,
        cBitMask = BIT5;
        break;
        //############################ DVB-T2 #############################
    case COFDM_P1_LOCK:
        u16Address =  0x2482; //Pl lock,
        cBitMask = BIT3;
        break;
    case COFDM_P1_LOCK_HISTORY:
        use_dsp_reg = 1;
        u16Address =  0x00F0; //Pl ever lock,
        cBitMask = BIT3;
        break;
    case COFDM_L1_CRC_LOCK:
        u16Address =  0x2741; //L1 CRC check,
        cBitMask = BIT5 | BIT6 | BIT7;
        break;
    case COFDM_FEC_LOCK_T2:
#if 0
    {
        U16 u16RegAddress;
        U8 u8Data;
        u16RegAddress = 0x0900 + (0x48) * 2;
        ReadReg(u16RegAddress, &u8Data);

        DBG_DOMOD_MSB1231(printf(">>>MSB123x:DVB-T2 [%s] Lock Status = 0x%x\n", __FUNCTION__, u8Data));

        if (u8Data == 0x09)
            Ret = TRUE;
        else
            Ret = FALSE;

        return Ret;
    }
#else
    use_dsp_reg = 1;
    u16Address =  0x00F0; //FEC lock,
    cBitMask = BIT7;
#endif
    break;
    default:
        return FALSE;
    }

    //addy update 0805
    if (!use_dsp_reg)
    {
        if (MSB123X_ReadReg(u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (MSB123X_ReadDspReg(u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }
    return FALSE;
}

EN_LOCK_STATUS MSB123X_DTV_DVB_T_GetLockStatus(void)
{
    MS_U32    u32Timeout = 0;
    MS_U32    u32LockTimeout = 2000;
    MS_U8     u8Data = 0;
    MS_U16    u16RegAddress = 0;
    MS_U8     u8LockState = 0;
    MS_U8     u8BitMask = 0;
    MS_BOOL   bCheckPass = FALSE;
    static MS_U32 u32LockTimeStartDVBT = 0;
    static MS_U32 u32LockTimeStartDVBT2 = 0;
    static MS_U32  u32LockTimeStartDVBC = 0;
    MS_U32 u32NowTime = MsOS_GetSystemTime();
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        u32Timeout = 1500;

        MSB123X_ReadDspReg(0x00f0, &u8Data);
        if ((u8Data & BIT7) != 0x00)
            bCheckPass = TRUE;
        else
        {
            u16RegAddress =  0x00f0; //P1 Lock History
            u8BitMask = BIT5;
            MSB123X_ReadDspReg(u16RegAddress, &u8Data);
            if ((u8Data & u8BitMask) != 0)
            {
                u32Timeout = 6000;
                PRINTE(("====> Short T2 Locking Time, P1 Lock Flag = 0x%x\n", u8Data));
            }
        }

        if (bCheckPass)
        {
            u32LockTimeStartDVBT2 = MsOS_GetSystemTime();
            pMSB123X->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB123X->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBT2 < u32LockTimeout))
        {
            // SET u32LockTimeStartDVBT2 every time reset demodulator to T2
            //PRINTE(("NowTime: %d  -  pMSB123X->u32ChkScanTimeStart  %d = %d  \n",
            //    u32NowTime,pMSB123X->u32ChkScanTimeStart,(u32NowTime-pMSB123X->u32ChkScanTimeStart)));
            //PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB123X->bFECLock = FALSE;
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB123X->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        u32Timeout = 1500;
        u16RegAddress = 0x11E0;
        u8LockState = 0x0B;
        MSB123X_ReadReg(u16RegAddress, &u8Data);
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        else if (u8Data == 0xBB)
        {
            // dsp_table_crc_error
            PRINTE(("@msb123x Dsp table init param crc error !!!\n"));
            MSB123X_DTV_DVBT_DSPReg_ReadBack();
        }
        else
        {
            if (MSB123X_Lock(E_DEVICE_DEMOD_DVB_T, COFDM_TPS_LOCK_HISTORY) == TRUE)
            {
                PRINTE(("T TPS locked!\n"));
                u32Timeout = 6000;
            }
        }

        if (bCheckPass)
        {
            u32LockTimeStartDVBT = MsOS_GetSystemTime();
            pMSB123X->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB123X->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBT < u32LockTimeout))
        {
            //PRINTE(("NowTime: %d  -  pMSB123X->u32ChkScanTimeStart  %d = %d  \n",
            //  u32NowTime,pMSB123X->u32ChkScanTimeStart,(u32NowTime-pMSB123X->u32ChkScanTimeStart)));
            // PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB123X->bFECLock = FALSE;
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB123X->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
        break;
    }
    case E_DEVICE_DEMOD_DVB_C://mick
    {
        u32Timeout = 1500;
        u16RegAddress = 0x11E0;
        u8LockState = 0x0C;
        MSB123X_ReadReg(u16RegAddress, &u8Data);
        //printf("[msb123x]+++++++++DVBC State---->%d\n",u8Data);//mick
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        else
        {
            u16RegAddress =  0x0990; //TPS his Lock,
            u8BitMask = BIT3;
            MSB123X_ReadReg(u16RegAddress, &u8Data);
            if ((u8Data & u8BitMask) > 0)
                u32Timeout = 6000;
        }
        //DBG_DEMOD_MSB(printf(">>>MSB123x: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
        if (bCheckPass)
        {
            u32LockTimeStartDVBC = MsOS_GetSystemTime();
            pMSB123X->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB123X->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBC < u32LockTimeout))
        {
            pMSB123X->bFECLock = FALSE;
            return  E_DEMOD_CHECKING;
        }
        else
        {
            pMSB123X->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
    }
    break;
    default:
        return E_DEMOD_UNLOCK;
    }
}

MS_BOOL MSB123X_DTV_DVBT2_DSPReg_Init(void)
{
    MS_U8    idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

#if(FRONTEND_TUNER_TYPE == TUNER_RT828)
    pMSB123X->pMSB123X_RegDVBT2[0] = T2_BW_VAL;
    if (DEMOD_BW_MODE_1_7MHZ == pMSB123X->enTuneBw)  // 1.7M
    {
        pMSB123X->pMSB123X_RegDVBT2[1] = T2_FC_L_VAL_1_7M;
        pMSB123X->pMSB123X_RegDVBT2[2] = T2_FC_H_VAL_1_7M;
    }
    else if (DEMOD_BW_MODE_6MHZ == pMSB123X->enTuneBw)  //6M
    {
        pMSB123X->pMSB123X_RegDVBT2[1] = T2_FC_L_VAL_6M;
        pMSB123X->pMSB123X_RegDVBT2[2] = T2_FC_H_VAL_6M;
    }
    else     //7, 8M
    {
        pMSB123X->pMSB123X_RegDVBT2[1] = T2_FC_L_VAL;
        pMSB123X->pMSB123X_RegDVBT2[2] = T2_FC_H_VAL;
    }
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
    pMSB123X->pMSB123X_RegDVBT2[0] = T2_BW_VAL;
    if (DEMOD_BW_MODE_7MHZ == pMSB123X->enTuneBw)  //7M
    {
        pMSB123X->pMSB123X_RegDVBT2[1] = T2_FC_L_VAL_7M;
        pMSB123X->pMSB123X_RegDVBT2[2] = T2_FC_H_VAL_7M;
    }
    else     // 8M
    {
        pMSB123X->pMSB123X_RegDVBT2[1] = T2_FC_L_VAL;
        pMSB123X->pMSB123X_RegDVBT2[2] = T2_FC_H_VAL;
    }
#endif

    for (idx = 0; idx < sizeof(pMSB123X->pMSB123X_RegDVBT2); idx++)
    {
        if (MSB123X_WriteDspReg(idx + 0x40, pMSB123X->pMSB123X_RegDVBT2[idx]) != TRUE)
        {
            PRINTE(("T2 dsp reg init NG\n"));
            return FALSE;
        }
    }

    if (MSB123X_WriteDspReg((MS_U8)T2_TS_SERIAL, T2_TS_SERIAL_VAL) != TRUE)
    {
        PRINTE(("T2 dsp reg init NG\n"));
    }
    if (MSB123X_WriteDspReg((MS_U8)T2_TS_CLK_RATE, T2_TS_CLK_RATE_VAL) != TRUE)
    {
        PRINTE(("T2 dsp reg init NG\n"));
    }
    if (MSB123X_WriteDspReg((MS_U8)T2_TS_OUT_INV, T2_TS_OUT_INV_VAL) != TRUE)
    {
        PRINTE(("T2 dsp reg init NG\n"));
    }
    if (MSB123X_WriteDspReg((MS_U8)T2_TS_DATA_SWAP, T2_TS_DATA_SWAP_VAL) != TRUE)
    {
        PRINTE(("T2 dsp reg init NG\n"));
    }
    if (MSB123X_WriteDspReg((MS_U8)T2_TS_ERR_POL, T2_TS_ERR_POL_VAL) != TRUE)
    {
        PRINTE(("T2 dsp reg init NG\n"));
    }
    if (MSB123X_WriteDspReg((MS_U16)T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL) != TRUE)
    {
        PRINTE(("T2 dsp reg init NG\n"));
    }
    PRINTE(("T2 dsp reg init ok\n"));
    return TRUE;
}

/************************************************************************************************
Subject:    Get PLP-ID Bit Map
Function:   DTV_GetPlpBitMap
Parmeter:   u32PlpBitMap
Return:     BOOLEAN :
Remark:     32 byte of data to show PLP-ID existense
            |00000000|00000000|...|00000000|00011111|11111111|
             byte31   byte30   ... byte2    byte1    byte0   => 256 bit in total
 *************************************************************************************************/
MS_BOOL MSB123X_DTV_GetPlpBitMap(MS_U8* u8PlpBitMap)
{
    MS_BOOL   status = TRUE;
    MS_U8     u8Data = 0;
    MS_U8     indx = 0;

    status &= MSB123X_ReadDspReg(0x0120, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
        return FALSE;

    while (indx < 32)
    {
        status &= MSB123X_ReadDspReg(0x0100 + indx, &u8Data);
        u8PlpBitMap[indx] = u8Data;
        indx++;
    }
    if (status)
    {
        printf("\n+++++++++u8PlpBitMap data+++++++++++++++\n");
        for (indx = 0; indx < 32; indx++)
            printf("[%d] ", u8PlpBitMap[indx]);
        printf("\n+++++++++u8PlpBitMap end+++++++++++++++\n");
    }
    return status;
}

/*************************************************************************************************
Subject:    Get GROUP_ID upon PLP_ID for MPLP mode
Function:   DTV_GetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB123X_DTV_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    MS_BOOL   status = TRUE;
    MS_U8 u8Data;
    status &= MSB123X_ReadDspReg(0x0120, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        PRINTE(("[msb123x]dvbt2 L1 not ready yet\n"));
        return FALSE;
    }
    MSB123X_ReadDspReg(0x00f0, &u8Data);

    if ((u8Data & BIT7) == 0x00)
    {
        PRINTE(("[msb123x]dvbt2 is un-lock\n"));
        return FALSE;
    }
    // assign PLP-ID value
    status &= MSB123X_WriteReg(0x2700 + (0x78) * 2, u8PlpID);
    status &= MSB123X_WriteReg(0x2700 + (0x01) * 2 + 1, 0x01); // MEM_EN
    MSB123X_MDrv_Timer_Delayus(1000);
    status &= MSB123X_ReadReg(0x2700 + (0x79) * 2, u8GroupID);
    status &= MSB123X_WriteReg(0x2700 + (0x01) * 2 + 1, 0x00); // ~MEM_EN
    PRINTE(("[end, return %d]", status));
    return status;
}


/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB123X_DTV_SetPlpGroupID(MS_U8 u8PlpID, MS_U8 u8GroupID)
{

    MS_BOOL   status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    pMSB123X->u32ChkScanTimeStart = MsOS_GetSystemTime();
    PRINTE(("[start]\n"));

    // assign Group-ID and PLP-ID value (must be written in order)
    status &= MSB123X_WriteDspReg(0x0122, u8GroupID);
    status &= MSB123X_WriteDspReg(0x0121, u8PlpID);
    PRINTE(("[end, return %d]\n", status));
    return status;
}

MS_U8 MSB123X_GetPlpIDList(MS_U8* u8PlpID)
{
    MS_BOOL bRet;
    MS_U8 i, j, u8PlpBitMap[32];
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    pMSB123X->PlpIDSize = 0;
    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));

    bRet = MSB123X_DTV_GetPlpBitMap(u8PlpBitMap);
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((u8PlpBitMap[i] >> j) & 1)
            {
                u8PlpID[pMSB123X->PlpIDSize] = i * 8 + j;
                pMSB123X->PlpIDSize++;
            }
        }
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return pMSB123X->PlpIDSize;
}

#if 0
//????
MS_U16 MSB123X_DTV_GetCellID(void)
{
    MS_U8 id;
    MS_U8 status = TRUE;
    MS_U16  cell_id;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        _qe("pMSB123X error !");
        return FALSE;
    }
    if (!pMSB123X->bInited || !pMSB123X->bOpen)
    {
        _qe("pMSB123X have not inited !!!");
        return FALSE;
    }
    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);

    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
        return 0;
    }
    else if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        status &= MSB123X_WriteReg(0x0ffe, 0x01);

        status &= MSB123X_ReadReg(0x0f2b, &id);
        cell_id = (MS_U16)id;

        status &= MSB123X_ReadReg(0x0f2a, &id);
        cell_id |= (MS_U16)id << 8;

        status &= MSB123X_WriteReg(0x0ffe, 0x00);

        if (status)
        {
            printf(" @MSB1231_GetCellId OK,cell_id %d\n", cell_id);
            MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
            return  TRUE;
        }
        else
        {
            printf(" @MSB1231_GetCellId NG\n");
            MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
            return FALSE;
        }
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return cell_id;
}
#endif

MS_U8 MSB123X_GetPlpIDSize(void)
{
    MS_U8    u8Size = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    u8Size = pMSB123X->PlpIDSize;
    PRINTE(("[end],PlpID size %d", u8Size));
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return u8Size;

}

MS_BOOL MSB123X_T2_SetPlpID(MS_U8 u8PlpID)
{
    MS_BOOL bRet = FALSE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    {
        MS_U32 u32Timeout;
        MS_U8 u8Data;
        MS_U16 u16RegAddress;
        MS_U8 u8LockState;
        MS_BOOL bCheckPass = FALSE;

        u32Timeout = 1500;
        u16RegAddress = 0x0990;
        u8LockState = 0x09;
        MSB123X_ReadReg(u16RegAddress, &u8Data);
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        if ((u8PlpID != 0xFF) && (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2) && (bCheckPass))
        {

            MS_U16 u16Retry = 0;
            MS_U8 u8GroupId = 0;
            MSB123X_MDrv_Timer_Delayms(500);

            bRet = MSB123X_DTV_GetPlpGroupID(u8PlpID, &u8GroupId);
            while ((bRet == FALSE) && (u16Retry < 60))
            {
                u16Retry++;
                PRINTE(("DoSet_DVBT2 get groupid retry %d \n", u16Retry));
                MSB123X_MDrv_Timer_Delayus(100 * 1000);
                bRet = MSB123X_DTV_GetPlpGroupID(u8PlpID, &u8GroupId);
            }
            if (bRet == FALSE)
            {
                PRINTE(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_GetPlpGroupID(%d) Error \n", u8PlpID));
                return FALSE;
            }

            bRet = MSB123X_DTV_SetPlpGroupID(u8PlpID, u8GroupId);
            if (bRet == FALSE)
            {
                PRINTE(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_SetPlpGroupID(%d,%d) Error", u8PlpID, u8GroupId));
                return FALSE;
            }
        }
    }
    return TRUE;
}

MS_BOOL  MSB123X_DTV_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth, MS_U8 u8PlpID)
{
    MS_BOOL bRet = FALSE;
    MS_U8 bw_t2 = 0, bw_t = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    printf("\n[%s]->freq=%d,bdwh=%d,PlpID %d,type %s\n", __FUNCTION__, u32Frequency, eBandWidth, u8PlpID, E_DEVICE_DEMOD_DVB_T == pMSB123X->enDemodType ? "T" : "T2");

    pMSB123X->u32ChkScanTimeStart = MsOS_GetSystemTime();

    //set freq.bw and plp id to demod
    switch (eBandWidth)
    {
    case 6:
        bw_t2 = E_DEMOD_BW_6M;
        bw_t   = E_DEMOD_BW_6M - 1;
        break;
    case 7:
        bw_t2 = E_DEMOD_BW_7M;
        bw_t  = E_DEMOD_BW_7M - 1;
        break;
    case 10:
        bw_t2 = E_DEMOD_BW_10M;
        break;
    case 17:
        bw_t2 = E_DEMOD_BW_17M;
        break;
    case 8:
    default:
        bw_t2 = E_DEMOD_BW_8M;
        bw_t  = E_DEMOD_BW_8M - 1;
        break;
    }
    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        MSB123X_Reset();
        MSB123X_MDrv_Timer_Delayus(500);
        bRet =  MSB123X_WriteDspReg((MS_U8)T_CONFIG_BW, bw_t);   // BW: 1->6M, 2->7M, 3->8M
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_BW - (MS_U8)T_OPMODE_RFAGC_EN] = (MS_U8)bw_t;
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MSB123X_DTV_DVBT_DSPReg_CRC();
        bRet = MSB123X_WriteDspReg((MS_U8)T_PARAM_CHECK_SUM, pMSB123X->pMSB123X_RegDVBT [(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]);    // BW: 1->6M, 2->7M, 3->8M
        MSB123X_Active(TRUE);
    }
    else if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        MSB123X_Reset();
        MSB123X_MDrv_Timer_Delayus(500);
        MSB123X_WriteDspReg((MS_U16)T2_BW, bw_t2);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB123X_WriteDspReg((MS_U16)T2_PLP_ID, u8PlpID);
        MSB123X_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }
    else if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C)
    {
        MSB123X_Reset();
    }
    return TRUE;
}

MS_BOOL MSB123x_MEM_switch(MS_U8 mem_type)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8_tmp = 0;
    MS_U16 timeout = 0;

    PRINTE(("[start]MSB123x_MEM_switch, mem_type=0x%x\n", mem_type));

    if (mem_type == 1)
    {

        bRet &= MSB123X_WriteReg(0x2B80, 0x10);

        // SRAM_START_ADDR 0x0000
        bRet &= MSB123X_WriteReg2bytes(0x1000, 0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x1004, 0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= MSB123X_WriteReg2bytes(0x1002, 0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x1006, 0x7FFF);

        // DRAM_START_ADDR 0x8000
        bRet &= MSB123X_WriteReg2bytes(0x1008, 0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100C, 0x8000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= MSB123X_WriteReg2bytes(0x100A, 0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100E, 0xFFFF);

        // Enable SRAM&SDRAM memory map
        bRet &= MSB123X_WriteReg(0x1018, 0x05);

        // Wait memory map to be enabled
        do
        {
            bRet &= MSB123X_ReadReg(0x1018, &u8_tmp);
            if (timeout++ > 500)
            {
                PRINTE(("@msb123x, D+S memory mapping failure.!!!\n"));
                return FALSE;
            }
        }while (u8_tmp != 0x05);
    }
    else if (mem_type == 0)
    {
        // Enable SRAM&SDRAM memory map

        bRet &= MSB123X_WriteReg(0x2B80, 0x10);

        // DRAM_START_ADDR 0x8000
        bRet &= MSB123X_WriteReg2bytes(0x1008, 0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100C, 0x0000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= MSB123X_WriteReg2bytes(0x100A, 0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100E, 0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= MSB123X_WriteReg(0x1018, 0x04);

        // Wait memory map to be enabled
        do
        {
            bRet &= MSB123X_ReadReg(0x1018, &u8_tmp);
            if (timeout++ > 500)
            {
                PRINTE(("@msb123x, D memory mapping failure.!!!\n"));
                return FALSE;
            }
        }while (u8_tmp != 0x04);
    }
    else
    {
        PRINTE(("@msb123x, invalid mem type mapping.\n"));
        return FALSE;
    }
    PRINTE(("[end]MSB123x_MEM_switch, , mem_type=0x%x, bRet=0x%x\n", mem_type, bRet));

    return bRet;
}

// i2c 150Kb/s, average rate 110Kb/s, 32KB, 2436ms.
MS_BOOL MSB123X_Load2Sdram(MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sdram_win_offset_base)
{
    MS_BOOL bRet = TRUE;
    MS_U16  sdram_win_offset = sdram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;
    PRINTE(("[msb123x][beg]MSB123X_Load2Sdram, len=0x%x, win_offset=0x%x\n", data_length, sdram_win_offset_base));
    if (!MSB123X_I2C_CH_Reset(0, FALSE))
    {
        PRINTE((">>>MSB123x CH0 Reset:Fail\n"));
        return FALSE;
    }
    else
    {
        // set xData map upper and low bound for 64k DRAM window
        bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x63) * 2, 0x3F24);

        // set xData map offset for 64k DRAM window, 64kbytes alignment
        bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x64) * 2, 0x0000);

        // set xData map upper and low bound for 4k DRAM window
        bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x65) * 2, 0x2014);

        // set xData map offset for 4k DRAM window, 4kbytes alignment
        bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x66) * 2, sdram_win_offset++);

        // enable xData map for DRAM
        bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x62) * 2, 0x0007);

        for (x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
        {
            if (y_cir_addr == 0x6000)
            {
                //set xData map offset for 4k DRAM window, 4kbytes alignment
                // 0x1000, 4096 bytes
                bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x66) * 2, sdram_win_offset++);
                y_cir_addr = SDRAM_BASE;
            }

            // max 0x200, error above.....
            if ((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
            {
                bRet &= MSB123X_WriteRegs(y_cir_addr, (u8_ptr + x_data_offset), LOAD_CODE_I2C_BLOCK_NUM);
                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
            }
            else
            {
                bRet &= MSB123X_WriteRegs(y_cir_addr, (u8_ptr + x_data_offset), data_length - x_data_offset);
                y_cir_addr += (data_length - x_data_offset);
                x_data_offset += (data_length - x_data_offset);
            }
        }
        PRINTE(("x_data_offset=%d,y_cir_addr=%d,z_block_num=%d\n", x_data_offset, y_cir_addr, sdram_win_offset));
        //  Release xData map for SDRAM
        bRet &= MSB123X_WriteReg2bytes(0x2B00 + (0x62) * 2, 0x0000);

        // Channel changed from CH 0x00 to CH 0x03
        if (!MSB123X_I2C_CH_Reset(3, FALSE))
        {
            PRINTE((">>>MSB123x CH3 Reset:Fail\n"));
            return FALSE;
        }
    }
    PRINTE(("[end]MSB123X_Load2Sdram, len=0x%x, win_offset=0x%x,bRet= 0x%02x\n", data_length, sdram_win_offset_base, bRet));
    return bRet;
}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_Boot(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    PRINTE(("[start] BOOT\n"));

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (!(pMSB123X->u8SdramCode & MSB123x_BOOT))
    {
        if (pMSB123X->u32MSB123XlibSize > WIMBLEDON_BOOT_START_ADDR)
        {
            // boot code
            data_ptr = pMSB123X->pMSB123XLib + WIMBLEDON_BOOT_START_ADDR;
            code_size = WIMBLEDON_BOOT_END_ADDR - WIMBLEDON_BOOT_START_ADDR + 1;
            win_offset = WIMBLEDON_BOOT_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB123X->u8SdramCode |= MSB123x_BOOT;
            }
        }
        else
        {
            PRINTE(("@msb123x, boot code is unavailable!!!\n"));
        }
    }
    PRINTE(("[end]BOOT bRet 0x%02x\n", bRet));
    return bRet;

}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_dvbt2(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    PRINTE(("[start] DVBT2\n"));
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (!(pMSB123X->u8SdramCode & MSB123x_DVBT2))
    {
        if (pMSB123X->u32MSB123XlibSize > WIMBLEDON_DVBT2_P1_START_ADDR)
        {
            // dvbt2_p2
            data_ptr = pMSB123X->pMSB123XLib  + WIMBLEDON_DVBT2_P2_START_ADDR;
            code_size = WIMBLEDON_DVBT2_P2_END_ADDR - WIMBLEDON_DVBT2_P2_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBT2_P2_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr, code_size, win_offset);

            // dvbt2_p1
            data_ptr = pMSB123X->pMSB123XLib  + WIMBLEDON_DVBT2_P1_START_ADDR;
            code_size = WIMBLEDON_DVBT2_P1_END_ADDR - WIMBLEDON_DVBT2_P1_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBT2_P1_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr, code_size, win_offset);

            if (bRet == TRUE)
            {
                pMSB123X->u8SdramCode |= MSB123x_DVBT2;
            }
        }
        else
        {
            PRINTE(("@msb123x, dvbt2 code is unavailable!!!\n"));
        }
    }
    PRINTE(("[end]MSB123X_LoadDspCodeToSDRAM_dvbt2\n"));
    return bRet;
}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_dvbt(void)
{

    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    PRINTE(("[start] DVBT\n"));
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (!(pMSB123X->u8SdramCode & MSB123x_DVBT))
    {
        // dvbt code
        if (pMSB123X->u32MSB123XlibSize > WIMBLEDON_DVBT_START_ADDR)
        {
            data_ptr = pMSB123X->pMSB123XLib + WIMBLEDON_DVBT_START_ADDR;
            code_size = WIMBLEDON_DVBT_END_ADDR - WIMBLEDON_DVBT_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBT_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB123X->u8SdramCode |= MSB123x_DVBT;
            }
        }
        else
        {
            PRINTE(("@msb123x, dvbt code is unavailable!!!\n"));
        }
    }
    PRINTE(("[end]MSB123X_LoadDspCodeToSDRAM_dvbt,bRet=0x%02x\n", bRet));
    return bRet;
}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_dvbc(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (!(pMSB123X->u8SdramCode & MSB123x_DVBC))
    {
        // dvbc code
        if (pMSB123X->u32MSB123XlibSize > WIMBLEDON_DVBC_START_ADDR)
        {
            data_ptr = pMSB123X->pMSB123XLib  + WIMBLEDON_DVBC_START_ADDR;
            code_size = WIMBLEDON_DVBC_END_ADDR - WIMBLEDON_DVBC_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBC_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB123X->u8SdramCode |= MSB123x_DVBC;
            }
        }
        else
        {
            PRINTE(("@msb123x, dvbc code is unavailable!!!\n"));
        }
    }
    return bRet;
}


// [0] bootloader [1] dvbt2, [2] dvbt, [3] dvbc [4] all
MS_BOOL MSB123X_LoadDspCodeToSDRAM(MS_U8 code_n)
{
    MS_BOOL bRet = TRUE;
    PRINTE(("[start]code_n=0x%02x\n", code_n));

    switch (code_n)
    {
    case MSB123x_BOOT:
    {
        // boot code
        bRet &= MSB123X_LoadDspCodeToSDRAM_Boot();
    }
    break;
    case MSB123x_DVBT2:
    {
        // dvbt2 code
        bRet &= MSB123X_LoadDspCodeToSDRAM_dvbt2();
    }
    break;
    case MSB123x_DVBT:
    {
        // dvbt
        bRet &= MSB123X_LoadDspCodeToSDRAM_dvbt();
    }
    break;
    case MSB123x_DVBC:
    {
        // dvbtc
        bRet &= MSB123X_LoadDspCodeToSDRAM_dvbc();
    }
    break;
    case MSB123x_ALL:
    default:
    {
        // boot+dvbt2+dvbt+dvbc

        // boot code
        bRet &= MSB123X_LoadDspCodeToSDRAM_Boot();
        // dvbt2
        bRet &= MSB123X_LoadDspCodeToSDRAM_dvbt2();
        // dvbt
        bRet &= MSB123X_LoadDspCodeToSDRAM_dvbt();
        // dvbtc
        //   bRet &= MSB123X_LoadDspCodeToSDRAM_dvbc();

    }
    break;
    }
    PRINTE(("[end]MSB123X_LoadDspCodeToSDRAM, code_n=0x%x, bRet=0x%x\n", code_n, bRet));
    return bRet;
}

MS_BOOL MSB123X_LoadSdram2Sram(MS_U8 CodeNum)
{
    PRINTE(("[start],CodeNum %d\n", CodeNum));

    MS_BOOL bRet = TRUE;
    MS_U8   u8_tmp = 0;
    MS_U8   u8DoneFlag = 0;
    MS_U32  u32Timeout = 0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (CodeNum == pMSB123X->u8sramCode)
    {
        PRINTE(("MSB123X_LoadSdram2Sram, code is available.\n"));
        return bRet;
    }

    bRet &= MSB123x_MEM_switch(0);

    if (CodeNum == MSB123x_DVBT2)
    {
        u8_tmp = 1 | 0x10;
    }
    else if (CodeNum == MSB123x_DVBT)
    {
        u8_tmp = 2 | 0x10;
    }
    else if (CodeNum == MSB123x_DVBC)
    {
        u8_tmp = 3 | 0x10;
    }
    else
    {
        u8_tmp = 0 | 0x10;
    }

    // Assign f/w code type to load => 0x11: dvbt2, 0x12: dvbt, 0x13: dvbc
    bRet &= MSB123X_WriteReg(0x0900 + (0x4f) * 2, u8_tmp);

    // enable miu access of mcu gdma
    //bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xf0);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x0f);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf0);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    // enable mcu
    bRet &= MSB123X_WriteReg(0x0b00 + (0x19) * 2, 0x00);

    do
    {
        bRet &= MSB123X_ReadReg(0x0900 + (0x4f) * 2, &u8DoneFlag);

        if (u32Timeout++ > 500)
        {
            PRINTE(("@msb123x, MSB123X_LoadSdram2Sram boot move code fail.!!!\n"));
            return FALSE;
        }

        MSB123X_MDrv_Timer_Delayus(1000);

    } while(u8DoneFlag != 0xaa);

    // mask miu access of mcu
    //bRet &= MSB123X_WriteReg(0x1200+(0x23)*2 + 1,0x7f);
    //bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xfe);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf2);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    // Reset MCU
    bRet &= MSB123X_WriteReg(0x0b00 + (0x19) * 2, 0x03);

    // enable miu mask, miu, mcu, gdma
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x0f);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf0);

    bRet &= MSB123x_MEM_switch(1);

    if (bRet == FALSE)
    {
        pMSB123X->u8sramCode = 0x00;
    }
    else
    {
        pMSB123X->u8sramCode = CodeNum;
    }

    PRINTE(("[end]return %d\n", bRet));
    return bRet;
}

MS_BOOL  MSB123X_PreLoadDSPcode(void)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8CodeNum = 0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);

    bRet &= MSB123X_LoadDspCodeToSDRAM(MSB123x_ALL);
    if (bRet == FALSE)
    {
        PRINTE((" MSB123X_LoadDspCodeToSDRAM failure...\n"));
    }

    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        u8CodeNum = MSB123x_DVBT2;
        //bRet &= MSB123X_LoadDspCodeToSDRAM(MSB123x_DVBT2);
        break;
    case E_DEVICE_DEMOD_DVB_T:
        u8CodeNum = MSB123x_DVBT;
        //bRet &= MSB123X_LoadDspCodeToSDRAM(MSB123x_DVBT);
        break;
    case E_DEVICE_DEMOD_DVB_C:
        u8CodeNum = MSB123x_DVBC;
        break;
    default:
        u8CodeNum = MSB123x_DVBT;
        MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
        break ;
    }
    // mask miu access for all and
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x7f);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xfe);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    bRet &= MSB123X_WriteReg(0x0b00 + (0x19) * 2, 0x03);

    bRet &= MSB123X_LoadSdram2Sram(u8CodeNum);

    // enable miu access of mcu gdma
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf0);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);
    // Enable MCU
    bRet &= MSB123X_WriteReg(0x0b00 + (0x19) * 2, 0x00);

    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        bRet &=  MSB123X_DTV_DVBT2_DSPReg_Init();
        break;
    case E_DEVICE_DEMOD_DVB_T:
        bRet &= MSB123X_DTV_DVBT_DSPReg_Init();
        break;
    default:
        bRet &= MSB123X_DTV_DVBT_DSPReg_Init();
        break;
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    PRINTE(("[end],ret %d\n", bRet));
    return bRet;
}

MS_BOOL MSB123X_GetPreLoadStatus(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    return pMSB123X->bLoaded;
}

/*======================================================
**function: judgement wheather the tuner's IF is chagned or not
**exit param:TRUE: TUNER's IF is changed ;   FALSE:TUNER's IF is not changed
**description:  tuner RT828 : 7M and 8M have the same IF, 6M is different
**                   TDA18219: 7M and 8M have different IF
**                   others are the same
=======================================================*/
MS_BOOL  MSB123X_DTV_TUNER_IsIFChanged(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    printf("Demod CurrBW=%d, TuneBW=%d\n", pMSB123X->enCurrBW, pMSB123X->enTuneBw);
    if (pMSB123X->enCurrBW == pMSB123X->enTuneBw)
    {
        return FALSE;
    }
#if(FRONTEND_TUNER_TYPE == TUNER_RT828)
    if (((DEMOD_BW_MODE_6MHZ != pMSB123X->enCurrBW) && (DEMOD_BW_MODE_6MHZ == pMSB123X->enTuneBw)) || \
            ((DEMOD_BW_MODE_6MHZ == pMSB123X->enCurrBW) && (DEMOD_BW_MODE_6MHZ != pMSB123X->enTuneBw)))
    {
        return TRUE;
    }
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
    if (pMSB123X->enCurrBW != pMSB123X->enTuneBw)
    {
        if (E_DEMOD_TYPE_T2 == MSB123X_GetCurrenDemodType())
        {
            return TRUE;
        }
    }
#endif

    return FALSE;
}

MS_BOOL MSB123X_SetCurrenDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    MS_U8 bRet = TRUE, u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
	printf("\n==================type================ 1\n");
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (enDemodType == pMSB123X->enDemodType)
    {
        if (FALSE == MSB123X_DTV_TUNER_IsIFChanged())
            return TRUE;
    }

    pMSB123X->u32ChkScanTimeStart = MsOS_GetSystemTime();
    pMSB123X->enDemodType = enDemodType;
    pMSB123X->bFECLock = FALSE;

    // mask miu access for all and mcu
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x7f);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xfe);

    // 10us delay
    MSB123X_MDrv_Timer_Delayms(10);

    // Disable MCU
    bRet &= MSB123X_WriteReg(0x0b00 + (0x19) * 2, 0x03);
    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        u8Data = MSB123x_DVBT2;
        break;
    case E_DEVICE_DEMOD_DVB_T:
        u8Data = MSB123x_DVBT;
        break;
    case E_DEVICE_DEMOD_DVB_C:
        u8Data = MSB123x_DVBC;
        break;
    default :
        pMSB123X->bLoaded = FALSE;
        return FALSE;
    }
    bRet &= MSB123X_LoadDspCodeToSDRAM(u8Data);
    bRet &= MSB123X_LoadSdram2Sram(u8Data);
    // enable miu access of mcu gdma
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf0);
    // 10us delay
    MSB123X_MDrv_Timer_Delayms(10);
    // Enable MCU
    bRet &= MSB123X_WriteReg(0x0b00 + (0x19) * 2, 0x00);
    MSB123X_MDrv_Timer_Delayms(10);

    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        MSB123X_DTV_DVBT2_DSPReg_Init();
        break;
    case E_DEVICE_DEMOD_DVB_T:
        MSB123X_DTV_DVBT_DSPReg_Init();
        break;
    default :
        pMSB123X->bLoaded = FALSE;
        return FALSE;
    }
    //MSB123X_DTV_Serial_Control(FALSE);
    return bRet;

}

EN_DEVICE_DEMOD_TYPE MSB123X_GetCurrenDemodType(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    return pMSB123X->enDemodType ;
}

MS_BOOL MSB123X_SetScanType(MS_U8 status)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }

    switch (status)
    {
    case 0:
        pMSB123X->u8ScanStatus = 0;
        break;
    case 1:
        pMSB123X->u8ScanStatus = 1;
        break;

    case 2:
        pMSB123X->u8ScanStatus = 2;
        break;

    case 3:
        pMSB123X->u8ScanStatus = 3;
        break;
    default:
        MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
        return FALSE;

    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_U8 MSB123X_GetScanType(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }

    return pMSB123X->u8ScanStatus;
}
MS_BOOL MDrv_cofdmDmd_InitParameter(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    pMSB123X->PlpIDSize = 0;
    pMSB123X->u8ScanStatus = 0;
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetPlpID(MS_U8 u8PlpID)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }

    MSB123X_T2_SetPlpID(u8PlpID);  //set plp id

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_U8 MDrv_Demod_GetPlpIDSize(void)
{
    MS_U8    u8Size = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    u8Size = pMSB123X->PlpIDSize;
    PRINTE(("[end],PlpID size %d", u8Size));
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return u8Size;
}

MS_U8 MApi_FE_GetPlpIDList(MS_U8* u8PlpID)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return 0;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return 0;
    }

    if (0 == pMSB123X->PlpIDSize)
    {
        if (FALSE == MSB123X_GetPlpIDList(u8PlpID))
            pMSB123X->PlpIDSize = 0;
    }
    return pMSB123X->PlpIDSize;
}

MS_BOOL MApi_FE_ClearPlpList(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    pMSB123X->PlpIDSize = 0;
    return TRUE;
}

extern void MDrv_Tuner_SetTunerMode(FEMode mode);
MS_BOOL MDrv_Demod_SetCurrentDemodType(MS_U8  type)
{
    FEMode mode;
    mode.Femode = FAST_MODE;
    if (E_DEMOD_TYPE_T == type)
    {
        mode.Fetype = E_TUNE_TYPE_T;
        MDrv_Tuner_SetTunerMode(mode);
        return MSB123X_SetCurrenDemodType(E_DEVICE_DEMOD_DVB_T);
    }
    else
    {
        mode.Fetype = E_TUNE_TYPE_T2;
        MDrv_Tuner_SetTunerMode(mode);
        return MSB123X_SetCurrenDemodType(E_DEVICE_DEMOD_DVB_T2);
    }
}

MS_U8 msAPI_GetDemodType(void)
{
    if (E_DEVICE_DEMOD_DVB_T == MSB123X_GetCurrenDemodType())
        return E_DEMOD_TYPE_T;
    else
        return E_DEMOD_TYPE_T2;
}

MS_BOOL MDrv_Demod_Open(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB123X->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    pMSB123X->bOpen = FALSE;
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return TRUE;
}

MS_U32 MApi_FE_GetSNR(void)
{
    MS_U32 pu32SNR;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        pu32SNR = 0;
        if (E_DEMOD_LOCK == MSB123X_DTV_DVB_T_GetLockStatus())
        {
            pu32SNR  = (MS_U32)MSB123X_DTV_GetSignalQuality();
            pu32SNR = pu32SNR * 35 / 100;
        }
        if (pu32SNR > 100)
        {
            pu32SNR = 100;
        }
        else if (pu32SNR < 0)
        {
            pu32SNR = 0;
        }
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return pu32SNR;
}

MS_U16 MApi_FE_GetSignalQuality(void)
{
    MS_U16 u16Quality;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Quality = 0;
    if (E_DEMOD_LOCK == MSB123X_DTV_DVB_T_GetLockStatus())
    {
        u16Quality  = (MS_U16)MSB123X_DTV_GetSignalQuality();
    }
    if (u16Quality > 100)
    {
        u16Quality = 100;
    }
    else if (u16Quality < 0)
    {
        u16Quality = 0;
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return u16Quality;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *ps32Signal = 0;
        if (E_DEMOD_LOCK == MSB123X_DTV_DVB_T_GetLockStatus())
        {
            *ps32Signal  = (MS_U32)MSB123X_DTV_GetSignalStrength();
        }
        if (*ps32Signal > 100)
            *ps32Signal = 100;
        else if (*ps32Signal < 0)
            *ps32Signal = 0;
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

EN_LOCK_STATUS MApi_FE_GetT2LockStatus(void)
{
    EN_LOCK_STATUS LockStatus = E_DEMOD_UNLOCK;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetLockStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        LockStatus = MSB123X_DTV_DVB_T_GetLockStatus();
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return LockStatus;
}

MS_BOOL  MApi_FE_CheckLock(void)
{
    MS_BOOL pbLock;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    MSB123X_MDrv_Timer_Delayms(10);
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        pbLock = (E_DEMOD_LOCK == MSB123X_DTV_DVB_T_GetLockStatus()) ? TRUE : FALSE;
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return pbLock;
}

void MApi_FE_SetT2Param(MS_U8 u8ScanType, MS_U8 u8PlpId)
{
    u8Type = u8ScanType;
    u8Plp = u8PlpId;
}

MS_BOOL MApi_FE_T2MI_Restart(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    MS_U8    PlpId, BandWidth;

    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if (E_DEVICE_DEMOD_DVB_T2 != pMSB123X->enDemodType)
    {
        MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
        return TRUE;
    }

    MSB123X_ReadDspReg((MS_U16)T2_PLP_ID, &PlpId);
    if (pMSB123X->u8CurrPlpID != PlpId)
    {
        switch (pMSB123X->enCurrBW)
        {
        case DEMOD_BW_MODE_6MHZ:
            BandWidth = E_DEMOD_BW_6M;
            break;
        case DEMOD_BW_MODE_7MHZ:
            BandWidth = E_DEMOD_BW_7M;
            break;
        case DEMOD_BW_MODE_1_7MHZ:
            BandWidth = E_DEMOD_BW_17M;
            break;
        case DEMOD_BW_MODE_8MHZ:
        default:
            BandWidth = E_DEMOD_BW_8M;
            break;
        }
        PRINTE(("Curr_PLP_ID=%d, GET_PLP_ID=%d\n", pMSB123X->u8CurrPlpID, PlpId));
        MSB123X_WriteDspReg((MS_U16)T2_BW, BandWidth);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB123X_WriteDspReg((MS_U16)T2_PLP_ID, pMSB123X->u8CurrPlpID);
        MSB123X_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

void MApi_FE_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth)
{
    MS_U32              dwError = FALSE;
    MS_U8               BandWidth = DEMOD_BW_MODE_8MHZ;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return ;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return ;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return ;
    }

    eBandWidth -= 1;
    pMSB123X->enTuneBw = eBandWidth;
    printf("\n[%s]tuner demod Freq=%d, Bw=%d,type=%d,plpid=%d\n", __FUNCTION__, u32Frequency, eBandWidth , u8Type, u8Plp);

    if (DEMOD_BW_MODE_1_7MHZ == eBandWidth)
    {
        MDrv_Demod_SetCurrentDemodType(E_DEMOD_TYPE_T2);
    }
    else if ((E_DEMOD_TYPE_T == u8Type) && (pMSB123X->u8CurrScanType != u8Type))
    {
        MDrv_Demod_SetCurrentDemodType(E_DEMOD_TYPE_T);
    }
    else if ((E_DEMOD_TYPE_T2 == u8Type) && (pMSB123X->u8CurrScanType != u8Type))
    {
        MDrv_Demod_SetCurrentDemodType(E_DEMOD_TYPE_T2);
    }

    switch (eBandWidth)
    {
    case DEMOD_BW_MODE_6MHZ:
        BandWidth = 6;
        break;
    case DEMOD_BW_MODE_7MHZ:
        BandWidth = 7;
        break;
    case DEMOD_BW_MODE_8MHZ:
        BandWidth = 8;
        break;
    case DEMOD_BW_MODE_1_7MHZ: // 1.7MHz
        BandWidth = 17;
        break;
    }

    if ((TRUE == pMSB123X->bFECLock) && (pMSB123X->u8CurrScanType == u8Type) && \
            (E_DEMOD_TYPE_T2 == pMSB123X->u8CurrScanType) && (pMSB123X->u32CurrFreq == u32Frequency) && \
            (pMSB123X->enCurrBW == eBandWidth))
    {
    }
    else
    {
        extern  MS_U32 MDrv_Tuner_SetTuner(MS_U32 u32Freq, MS_U8 feBw);
        //set freq and bw to tuner
        MSB123X_IIC_Bypass_Mode(TRUE);
        MDrv_Tuner_SetTuner(u32Frequency, BandWidth);
        MSB123X_IIC_Bypass_Mode(FALSE);
    }

    pMSB123X->u32CurrFreq = u32Frequency;
    pMSB123X->enCurrBW = eBandWidth;
    pMSB123X->u8CurrPlpID = u8Plp;
    pMSB123X->u8CurrScanType = u8Type;

    dwError = MSB123X_DTV_SetFrequency(u32Frequency, eBandWidth, u8Plp);
    if (E_DEMOD_TYPE_T2 == pMSB123X->enDemodType)
    {
        dwError &= MSB123X_T2_SetPlpID(u8Plp);
    }
    PRINTE((" ==return %d==\n", dwError));
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
}

void MApi_Demod_HWReset(void)
{
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(100);
    mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_DelayTask(200);
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(100);
}

/****************************************************************************
**
**function: Reset DJB when MPLP T2MI stream occur PAT timeout
**
****************************************************************************/
void MDrv_Demod_CtrlResetDJBFlag(MS_U8 Ctrl)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        if (Ctrl == TRUE)
        {
            if (pMSB123X->bFECLock == TRUE)
            {
                MS_U16 bch_eflag2_sum = 0;
                MS_U8  reg = 0;
                static MS_U32 t1  = 0;

                MSB123X_ReadReg(0x2600 + 0x26 * 2 + 1, &reg);
                bch_eflag2_sum = reg;
                MSB123X_ReadReg(0x2600 + 0x26 * 2, &reg);
                bch_eflag2_sum = (bch_eflag2_sum << 8) | reg;
                if ((bch_eflag2_sum == 0x00) && ((MsOS_GetSystemTime() - t1) > 1000))
                {
                    //djb rst
                    MSB123X_ReadReg(0x2900 + 0x00 * 2, &reg);
                    reg |= 0x01;
                    MSB123X_WriteReg(0x2900 + 0x00 * 2, reg);
                    MsOS_DelayTask(1000);
                    MSB123X_ReadReg(0x2900 + 0x00 * 2, &reg);
                    reg &= (0xff - 0x01);
                    MSB123X_WriteReg(0x2900 + 0x00 * 2, reg);
                    t1 = MsOS_GetSystemTime();
                    printf("\n[msb123x]Warning!!!, djb rst......\n");
                }
            }
        }
    }
}
void MApi_FE_Power_On_Initialization(void)
{
    MS_BOOL bRet = FALSE;
    if (MSB123X.bInited)
        return ;
    MSB123X.bInited = TRUE;
    MSB123X.bOpen = TRUE;
    MSB123X.bLoaded = FALSE;
    MSB123X.u8SdramCode = 0;
    MSB123X.u8sramCode = 0;
    MSB123X.PlpIDSize = 0;
    MSB123X.bFECLock = FALSE;
    MSB123X.pMSB123XLib = MSB123X_LIB ;
    MSB123X.u32MSB123XlibSize = sizeof(MSB123X_LIB);
    MSB123X.pMSB123X_RegDVBT = MSB1231_DVBT_DSPREG_TABLE ;
    MSB123X.pMSB123X_RegDVBT2 = MSB1231_DVBT2_DSPREG_TABLE;
    MSB123X.enDemodType = E_DEVICE_DEMOD_DVB_T;
    MSB123X.u8ScanStatus = 0;
    MSB123X.enCurrBW = DEMOD_BW_MODE_8MHZ;
    MSB123X.enTuneBw = DEMOD_BW_MODE_8MHZ;
   //    MSB123X.bDoReset = FALSE;
    MSB123X.s32_MSB123X_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2", MSOS_PROCESS_SHARED);
    MSB123X.s32_MSB123X_RW_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2RW", MSOS_PROCESS_SHARED);
    MSB123X.s32_MSB123X_RW2_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2RW2", MSOS_PROCESS_SHARED);

    if ((MSB123X.s32_MSB123X_Mutex < 0) || (MSB123X.s32_MSB123X_RW_Mutex < 0) || (MSB123X.s32_MSB123X_RW2_Mutex < 0))
    {
        PRINTE(("creat MSB123X Mutex error!!!\n"));
        return ;
    }
    void MApi_Demod_HWReset(void);
    MApi_Demod_HWReset();
    bRet = MSB123X_I2C_CH_Reset(3, TRUE);
    if (!bRet)
    {
        MSB123X.bInited = FALSE;
        MSB123X.bOpen = FALSE;
        MSB123X.bLoaded = FALSE;

        PRINTE(("[%s]  MSB123X I2C CH reset error!!!\n", __FUNCTION__));
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);
        return ;
    }

    bRet = MSB123X_HW_init();
    if (!bRet)
    {
        MSB123X.bInited = FALSE;
        MSB123X.bOpen = FALSE;
        MSB123X.bLoaded = FALSE;

        PRINTE(("[%s]  MSB123X HW init error!!!\n", __FUNCTION__));
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);
        return ;
    }
    bRet = MSB123X_PreLoadDSPcode();
    if (!bRet)
    {
        MSB123X.bInited = FALSE;
        MSB123X.bOpen = FALSE;
        MSB123X.bLoaded = FALSE;

        PRINTE(("[%s]  MSB123X pre load dspceode error!!!\n", __FUNCTION__));
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);
        return ;
    }
    MSB123X.bLoaded = TRUE;

#if TS_PARALLEL_OUTPUT
    MSB123X_DTV_Serial_Control(FALSE);
#else
    MSB123X_DTV_Serial_Control(TRUE);
#endif
#if((FRONTEND_TUNER_TYPE==TUNER_RT828)||(FRONTEND_TUNER_TYPE==TUNER_NXP_TDA18212)||(FRONTEND_TUNER_TYPE == TUNER_MXL201RF)||(FRONTEND_TUNER_TYPE == TUNER_NMI120))
    MSB123X_IIC_Bypass_Mode(TRUE);
    extern MS_BOOL MDrv_Tuner_Init(void);
    if (MDrv_Tuner_Init() == FALSE)
    {
        MSB123X_IIC_Bypass_Mode(FALSE);
        return ;
    }
    MSB123X_IIC_Bypass_Mode(FALSE);
#endif
    return ;

}

#endif
