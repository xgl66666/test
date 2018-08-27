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

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#else
#include <stdio.h>
#include <string.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "Board.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB123X)
#include "drvIIC.h"
#include "msb123x_dvbt.h"
#include "MsOS.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "device_demodulator_msb123x.h"
#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
#include "MxL603_TunerApi.h"
#define MXL603_I2C_ADDR         0xC0
extern MS_BOOL MDrv_Tuner_Init(MS_U8 u8TunerIndex);
extern MS_U32 MDrv_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/);
#endif

MS_U8 DemodVersion[] = {"MSB123x v.3.12"};
extern MS_BOOL MApp_MenuChannel_ManualScan_Tune(void);

#define MSB123X_MDrv_Timer_Delayms MsOS_DelayTask
#define MSB123X_MDrv_Timer_Delayus MsOS_DelayTaskUs

#define MDrv_msb123x_IIC_Write(a, b, c, d, e)    MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT0,a, c, b, e, d)
#define MDrv_msb123x_IIC_Read(a, b, c, d, e)    MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT0,a, c, b, e, d)


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

#ifdef MS_DEBUG
#define MSB123X_DEBUG     1
#else
#define MSB123X_DEBUG     0
#endif

#if MSB123X_DEBUG == 1
#define PRINTE(p)       printf p
#else
#define PRINTE(p)
#endif


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
/*
#if ((FRONTEND_TUNER_TYPE == TUNER_MXL201RF) || (FRONTEND_TUNER_TYPE == TUNER_MXL603))
#undef FRONTEND_DEMOD_IQ_SWAP
#define FRONTEND_DEMOD_IQ_SWAP     1
#else
#undef FRONTEND_DEMOD_IQ_SWAP
#define FRONTEND_DEMOD_IQ_SWAP     0
#endif
*/
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



#if IF_THIS_TUNER_INUSE(TUNER_MXL201RF)

//IF=4570
#define FC_L            0xA6
#define FC_H            0xA9
#else //if(FRONTEND_TUNER_TYPE == TUNER_MXL603)
//IF=5000
#define FC_L            0xF8
#define FC_H            0xA7
#endif


//configure   DVBT2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL           0x04
// FC: FC = FS - IF = 48000 - 4500 = 0xA9EC     (4.50MHz IF)
// FC: FC = FS - IF = 48000 - 36000 = 0x2EE0    (36.0MHz IF)
#if IF_THIS_TUNER_INUSE(TUNER_MXL201RF)
#define T2_FC_L_VAL            0xA6
#define T2_FC_H_VAL            0xA9
#else //if(FRONTEND_TUNER_TYPE == TUNER_MXL603)
#define T2_FC_L_VAL            0xF8
#define T2_FC_H_VAL            0xA7
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
    CCI_KP,       CCI_FSWEEP    ,TS_CLK_RATE_AUTO, DVBT_IF_INV_PWM_OUT_EN, DVBT_CRC
};

//DVBT2
MS_U8 MSB1231_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL,     T2_FC_L_VAL,     T2_FC_H_VAL
};

//
const S_RFAGC_SSI RFAGC_SSI_123X[] =
{
    {-19.40,    0x1B},
    {-20.40,    0x1C},
    {-21.40,    0x1D},
    {-22.40,    0x1E},
    {-23.40,    0x1F},
    {-24.40,    0x20},
    {-25.20,    0x21},
    {-26.60,    0x22},
    {-28.00,    0x23},
    {-29.20,    0x24},
    {-31.50,    0x25},
    {-32.75,    0x26},
    {-34.00,    0x27},
    {-35.25,    0x28},
    {-36.75,    0x29},
    {-38.50,    0x2a},
    {-40.00,    0x2b},
    {-42.00,    0x2c},
    {-43.50,    0x2d},
    {-44.80,    0x2e},
    {-46.20,    0x2f},
    {-47.40,    0x30},
    {-48.00,    0x31},
    {-48.40,    0x32},
    {-49.00,    0x33},
    {-99.00,    0x3f},
};

#if IF_THIS_TUNER_INUSE(TUNER_MXL201RF)
const S_IFAGC_SSI IFAGC_SSI_123X[] =
{
    {-43,   0x65},
    {-44,   0x67},
    {-45,   0x68},
    {-46,   0x69},
    {-47,   0x6b},
    {-48,   0x6c},
    {-49,   0x6e},
    {-50,   0x6f},
    {-51,   0x71},
    {-52,   0x72},
    {-53,   0x74},
    {-54,   0x75},
    {-55,   0x77},
    {-56,   0x78},
    {-57,   0x7a},
    {-58,   0x7c},
    {-59,   0x7d},
    {-60,   0x7f},
    {-61,   0x80},
    {-62,   0x82},
    {-63,   0x83},
    {-64,   0x84},
    {-65,   0x86},
    {-66,   0x87},
    {-67,   0x89},
    {-68,   0x8a},
    {-69,   0x8b},
    {-70,   0x8d},
    {-71,   0x8e},
    {-72,   0x90},
    {-73,   0x91},
    {-74,   0x93},
    {-75,   0x94},
    {-76,   0x96},
    {-77,   0x97},
    {-78,   0x99},
    {-79,   0x9a},
    {-80,   0x9c},
    {-90,   0xaa},
    {-99,   0xFF},
};

const S_IFAGC_SSI T2IFAGC_SSI_123X[] =
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

const S_SSI_PREF SsiPref_123X[] =
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
#else
const S_IFAGC_SSI IFAGC_SSI_123X[] =
{
    {-48,   0x3A},
    {-49,   0x3B},
    {-50,   0x3C},
    {-51,   0x3E},
    {-52,   0x40},
    {-53,   0x43},
    {-54,   0x45},
    {-55,   0x47},
    {-56,   0x49},
    {-57,   0x4C},
    {-58,   0x4f},
    {-59,   0x51},
    {-60,   0x54},
    {-61,   0x56},
    {-62,   0x59},
    {-63,   0x5C},
    {-64,   0x5F},
    {-65,   0x62},
    {-66,   0x65},
    {-67,   0x69},
    {-68,   0x6E},
    {-69,   0x73},
    {-70,   0x78},
    {-71,   0x7F},
    {-72,   0x85},
    {-73,   0x8b},
    {-74,   0x9c},
    {-85,   0xc0},
    {-99,   0xFF},
};

const S_IFAGC_SSI T2IFAGC_SSI_123X[] =
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

const S_SSI_PREF SsiPref_123X[] =
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

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
static const S_SQI_CN_NORDIGP1 dvbt_ssi_dbm_nordigp1[] =
{
    { _QPSK , _CR1Y2, -93},
    { _QPSK , _CR2Y3, -91},
    { _QPSK , _CR3Y4, -90},
    { _QPSK , _CR5Y6, -89},
    { _QPSK , _CR7Y8, -88},

    { _16QAM , _CR1Y2, -87},
    { _16QAM , _CR2Y3, -85},
    { _16QAM , _CR3Y4, -84},
    { _16QAM , _CR5Y6, -83},
    { _16QAM , _CR7Y8, -82},

    { _64QAM , _CR1Y2, -82},
    { _64QAM , _CR2Y3, -80},
    { _64QAM , _CR3Y4, -78},
    { _64QAM , _CR5Y6, -77},
    { _64QAM , _CR7Y8, -76},
    { _UNKNOW_QAM , _UNKNOW_CR, 0.0},
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
    MS_U8       PlpIDList[256];
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


#define REG_MB_CNTL     0x0C00
#define REG_MB_ADDR_L   0x0C02
#define REG_MB_ADDR_H   0x0C03
#define REG_MB_DATA     0x0C04

static float g_msb123x_fSNR = 0.0;


MS_BOOL MSB123X_I2C_CH_Reset(MS_U8 ch_num,MS_BOOL enable)
{
    MS_BOOL bRet  =TRUE;
    MS_U8         data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }

    /*
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
   */
    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);

    PRINTE(("[start],chan %d\n",ch_num));
    if(enable)        //in 7853,always execute this
    {
        data[0] = 0x53;
        bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 5);
        PRINTE(("return %s\n",bRet?"TRUE":"FALSE"));
    }
    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, data, 1);
    PRINTE(("[end] return %d\n",bRet));
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);

    return bRet;

}

/*@ <Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
MS_BOOL MSB123X_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet  =TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }

    pMSB123X->u8MsbData[0] = 0x10;
    pMSB123X->u8MsbData[1] = 0x00;
    pMSB123X->u8MsbData[2] = 0x00;
    pMSB123X->u8MsbData[3] = (u16Addr >> 8) &0xff;
    pMSB123X->u8MsbData[4] = u16Addr &0xff;
    pMSB123X->u8MsbData[5] = u8Data;

    pMSB123X->u8MsbData[0] = 0x35;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);

    pMSB123X->u8MsbData[0] = 0x10;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 6);

    pMSB123X->u8MsbData[0] = 0x34;
    bRet &= MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);
     return    bRet;
}

MS_BOOL MSB123X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8MsbData[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }


    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    // u8MsbData[5] = 0x00;

    for(idx = 0; idx < data_size ; idx++)
    {
        u8MsbData[5+idx] = u8pData[idx];
    }

    u8MsbData[0] = 0x35;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, u8MsbData, 1);

    u8MsbData[0] = 0x10;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, u8MsbData, 5 + data_size);

    u8MsbData[0] = 0x34;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, u8MsbData, 1);

     MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);

     return    bRet;

}

MS_BOOL MSB123X_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL ret = TRUE;
    ret &= MSB123X_WriteReg(u16Addr, (MS_U8)u16Data&0x00ff);
    ret &= MSB123X_WriteReg(u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);
    return ret;
}

/*@ </Operation ID=I2b28dd03m121c8cf959bmm6ff4> @*/
/*@ <Operation ID=I2b28dd03m121c8cf959bmm6fcf> @*/
MS_BOOL MSB123X_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }

    /*
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
*/

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW_Mutex, MSOS_WAIT_FOREVER);

    pMSB123X->u8MsbData[0] = 0x10;
    pMSB123X->u8MsbData[1] = 0x00;
    pMSB123X->u8MsbData[2] = 0x00;
    pMSB123X->u8MsbData[3] = (u16Addr >> 8) &0xff;
    pMSB123X->u8MsbData[4] = u16Addr &0xff;

    pMSB123X->u8MsbData[0] = 0x35;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);

    pMSB123X->u8MsbData[0] = 0x10;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 5);
    bRet &=MDrv_msb123x_IIC_Read(DEMOD_MSB123X_SLAVE_ID, 0,0, pu8Data, 1);

    pMSB123X->u8MsbData[0] = 0x34;
    bRet &=MDrv_msb123x_IIC_Write(DEMOD_MSB123X_SLAVE_ID, 0, 0, pMSB123X->u8MsbData, 1);
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_RW_Mutex);
    return bRet;

}


MS_BOOL MSB123X_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8     status = TRUE;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW2_Mutex, MSOS_WAIT_FOREVER);
    status &= MSB123X_WriteReg(REG_MB_DATA, u8Data);
    status &= MSB123X_WriteReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    status &= MSB123X_WriteReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    status &= MSB123X_WriteReg(REG_MB_CNTL, 0x04);

    do {
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

void  MSB123X_Reset(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U8     u8Data = 0x00;
    MS_U32    u32Retry = 0x00;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return ;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return ;
    }

  if(pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
  {

              // mask miu access for all and mcu
              bRet &= MSB123X_WriteReg(0x1200+(0x23)*2 + 1,0x7f);
              bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xfe);
     // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    MSB123X_WriteReg(0x0B00+(0x10)*2, 0x01);
    MSB123X_WriteReg(0x0B00+(0x19)*2, 0x03);
    MSB123X_WriteReg(0x0C00, 0x00);             //clear MB_CNTL
    MSB123X_MDrv_Timer_Delayus(5*100);
    // enable miu access of mcu gdma
       bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xf0);
     // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);


         MSB123X_WriteReg(0x0B00+(0x19)*2, 0x00);
    MSB123X_WriteReg(0x0B00+(0x10)*2, 0x00);
    MSB123X_MDrv_Timer_Delayus(5*1000);
    MSB123X_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready
    while(0xff != u8Data)
            {
          MSB123X_MDrv_Timer_Delayus(5*1000);
               MSB123X_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready

      if(u32Retry++ > 200)
                     {
                            PRINTE((">>>MSB123x: Reset Fail!\n"));
                            break;
                      }
    }
        }
  else if(pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T ||
        pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C)
  {
    // mask miu access for all and mcu
    MSB123X_WriteReg(0x1200+(0x23)*2 + 1 , 0x7f);
    MSB123X_WriteReg(0x1200+(0x23)*2  , 0xfe);
    MSB123X_MDrv_Timer_Delayus(10);
    MSB123X_WriteReg(0x0B00+(0x19)*2, 0x01);
    MSB123X_WriteReg(0x0B00+(0x10)*2, 0x01);
    MSB123X_WriteReg(0x0C00, 0x00);           //clear MB_CNTL
    MSB123X_MDrv_Timer_Delayus(5 * 1000);

    // enable miu access of mcu gdma
    MSB123X_WriteReg(0x1200+(0x23)*2  , 0xf0);

    MSB123X_MDrv_Timer_Delayus(10);
    MSB123X_WriteReg(0x0B00+(0x10)*2, 0x00);
    MSB123X_WriteReg(0x0B00+(0x19)*2, 0x00);
    MSB123X_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready
    while(u8Data != 0xff)
    {
      MSB123X_MDrv_Timer_Delayus(5 * 1000);
      MSB123X_ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
      if(u32Retry++ > 200)
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
MS_BOOL MSB123X_IIC_Bypass_Mode(MS_U8 u8DemodIndex,MS_BOOL enable)
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
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_RW2_Mutex, MSOS_WAIT_FOREVER);

    status &= MSB123X_WriteReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    status &= MSB123X_WriteReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    status &= MSB123X_WriteReg(REG_MB_CNTL, 0x03);

    do {
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
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    for (idx = 0; idx<((MS_U8)DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
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
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return ;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return ;
    }

    for (idx = T_OPMODE_RFAGC_EN; idx< (MS_U8)DVBT_PARAM_LEN; idx++)
    {
        MSB123X_ReadDspReg(idx,&ret);
        PRINTE(("@msb123x, idx=0x%x, dsp=0x%x, sw=0x%x\n",idx,ret,pMSB123X->pMSB123X_RegDVBT[idx-(MS_U8)T_OPMODE_RFAGC_EN]));
    }
}

MS_BOOL   MSB123X_DTV_DVBT_DSPReg_Init(void)
{
    MS_U8    idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    for (idx = 0; idx<((MS_U8)DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        if( MSB123X_WriteDspReg(idx + (MS_U8)T_OPMODE_RFAGC_EN, pMSB123X->pMSB123X_RegDVBT [idx])!=TRUE)
        {
            PRINTE(("dsp reg init NG\n"));
            return FALSE;
        }
    }

    pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM-(MS_U8)T_OPMODE_RFAGC_EN] = MSB123X_DTV_DVBT_DSPReg_CRC();

    if( MSB123X_WriteDspReg((MS_U8)T_PARAM_CHECK_SUM, pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM-(MS_U8)T_OPMODE_RFAGC_EN])!=TRUE)
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
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
    if(pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }
    else if ((pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        status = MSB123X_WriteReg(0x0C00+(0x0E)*2, 0x0001);  // FSM_EN
        if(status)
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


/********************************************
Function: To control GPIO00 to input_out mode
Parameter:Ctrl_Para  0->switch to output mode  1->input mode
*********************************************/
MS_BOOL MSB123X_DTV_GPIO00_In_Out_Switch(MS_U8 Ctrl_Para)
{
    MS_U8 reg_read,reg_write;
    MS_BOOL status;
    //set GPIO 00 to input/output mode
    //bank 9 0x64 bit 00  reg_gpiob_oen 1'b0
    status = MSB123X_ReadReg(0x09C8, &reg_read);
    reg_write=reg_read&(~0x01);
    reg_write=reg_write|(Ctrl_Para);
    status &= MSB123X_WriteReg(0x09C8, reg_write);
    return TRUE;
}


/********************************************
Function: To get GPIO00 intput value
Parameter:Data_Para:The value for returning the GPIO input value
Attention: Use this function after calling MSB123X_DTV_GPIO00_In_Out_Switch
           for switching to input mode.
*********************************************/
MS_BOOL MSB123X_DTV_GPIO00_Input_Read(MS_U8* Data_Para)
{
    MS_U8 reg_read;
    //MS_BOOL status;

    //bank 9 0x65 bit 00  reg_gpiob_in 1'b0
    MSB123X_ReadReg(0x09CA, &reg_read);
    *Data_Para=(reg_read&0x01);
    return TRUE;
}




MS_BOOL MSB123X_DTV_GPIO00_OutputCtrl(MS_U8 Ctrl_Para)
{
    MS_U8 reg_read,reg_write;
    MS_BOOL status;
    //set GPIO 00 to output mode
    //bank 9 0x64 bit 03  reg_gpiob_oen 1'b0
    status = MSB123X_ReadReg(0x09C8, &reg_read);
    reg_write=reg_read&(~0x01);
    status &= MSB123X_WriteReg(0x09C8, reg_write);
    //set GPIO 0 to hi/low according to Ctrl_Para
    //bank 9 0x63 bit 0  reg_gpiob_output
    status &= MSB123X_ReadReg(0x09C6, &reg_read);
    reg_write=reg_read&(~0x01);
    reg_write=reg_write|(Ctrl_Para);
    status &= MSB123X_WriteReg(0x09C6, reg_write);
    return TRUE;
}






/********************************************
Function: To control GPIO11 to input_out mode
Parameter:Ctrl_Para  0->switch to output mode  1->input mode
*********************************************/
MS_BOOL MSB123X_DTV_GPIO11_In_Out_Switch(MS_U8 Ctrl_Para)
{
    MS_U8 reg_read,reg_write;
    MS_BOOL status;
    //set GPIO 11 to input/output mode
    //bank 9 0x64 bit 11  reg_gpiob_oen 1'b0
    status = MSB123X_ReadReg(0x09C9, &reg_read);
    reg_write=reg_read&(~0x08);
    reg_write=reg_write|(Ctrl_Para<<3);
    status &= MSB123X_WriteReg(0x09C9, reg_write);

    return TRUE;
}


/********************************************
Function: To get GPIO11 intput value
Parameter:Data_Para:The value for returning the GPIO input value
Attention: Use this function after calling MSB123X_DTV_GPIO11_In_Out_Switch
           for switching to input mode.
*********************************************/
MS_BOOL MSB123X_DTV_GPIO11_Input_Read(MS_U8* Data_Para)
{
    MS_U8 reg_read;
    //MS_BOOL status;

    //bank 9 0x65 bit 11  reg_gpiob_in 1'b0
    MSB123X_ReadReg(0x09CB, &reg_read);
    *Data_Para=((reg_read&0x08)>>3);

    return TRUE;
}



/********************************************
Function: To control GPIO11 to hi or low voltage
Parameter:Ctrl_Para  0->Low voltage  1->Hi Voltage
*********************************************/
MS_BOOL MSB123X_DTV_GPIO11_OutputCtrl(MS_U8 Ctrl_Para)
{
    MS_U8 reg_read,reg_write;
    MS_BOOL status;
    //set GPIO 11 to output mode
    //bank 9 0x64 bit 11  reg_gpiob_oen 1'b0
    status = MSB123X_ReadReg(0x09C9, &reg_read);
    reg_write=reg_read&(~0x08);
    status &= MSB123X_WriteReg(0x09C9, reg_write);
    //set GPIO 11 to hi/low according to Ctrl_Para
    //bank 9 0x63 bit 11  reg_gpiob_output
    status &= MSB123X_ReadReg(0x09C7, &reg_read);
    reg_write=reg_read&(~0x08);
    reg_write=reg_write|(Ctrl_Para<<3);
    status &= MSB123X_WriteReg(0x09C7, reg_write);

    return TRUE;
}

/********************************************
Function: To control GPIO03 to input_out mode
Parameter:Ctrl_Para  0->switch to output mode  1->input mode
*********************************************/
MS_BOOL MSB123X_DTV_GPIO03_In_Out_Switch(MS_U8 Ctrl_Para)
{
    MS_U8 reg_read,reg_write;
    MS_BOOL status;
    //set GPIO 03 to input/output mode
    //bank 9 0x64 bit 03  reg_gpiob_oen 1'b0
    status = MSB123X_ReadReg(0x09C8, &reg_read);
    reg_write=reg_read&(~0x08);
    reg_write=reg_write|(Ctrl_Para<<3);
    status &= MSB123X_WriteReg(0x09C8, reg_write);

    return TRUE;
}


/********************************************
Function: To get GPIO03 intput value
Parameter:Data_Para:The value for returning the GPIO input value
Attention: Use this function after calling MSB123X_DTV_GPIO03_In_Out_Switch
           for switching to input mode.
*********************************************/
MS_BOOL MSB123X_DTV_GPIO03_Input_Read(MS_U8* Data_Para)
{
    MS_U8 reg_read;
    //MS_BOOL status;

    //bank 9 0x65 bit 03  reg_gpiob_in 1'b0
    MSB123X_ReadReg(0x09CA, &reg_read);
    *Data_Para=((reg_read&0x08)>>3);

    return TRUE;
}




MS_BOOL MSB123X_DTV_GPIO03_OutputCtrl(MS_U8 Ctrl_Para)
{
    MS_U8 reg_read,reg_write;
    MS_BOOL status;
    //set GPIO 03 to output mode
    //bank 9 0x64 bit 03  reg_gpiob_oen 1'b0
    status = MSB123X_ReadReg(0x09C8, &reg_read);
    reg_write=reg_read&(~0x08);
    status &= MSB123X_WriteReg(0x09C8, reg_write);
    //set GPIO 11 to hi/low according to Ctrl_Para
    //bank 9 0x63 bit 11  reg_gpiob_output
    status &= MSB123X_ReadReg(0x09C6, &reg_read);
    reg_write=reg_read&(~0x08);
    reg_write=reg_write|(Ctrl_Para<<3);
    status &= MSB123X_WriteReg(0x09C6, reg_write);

    return TRUE;
}
 EN_FRONTEND_SIGNAL_CONDITION MSB123X_DTV_GetSNR(void)
{
        MS_U32  snr_out = 0;
        MS_U16  snr_ave_num = 0;
        MS_U8   reg = 0, reg_frz = 0;
        EN_FRONTEND_SIGNAL_CONDITION   fe_falg ;
        float   f_snr = (float)0.0;
        float   fSNRDivider[]={1024*2, 2048*2, 4096*2, 8192*2, 16384*2, 32768*2, 32768*2, 32768*2};
        MS_U32  noise_power;

       MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
       if(!pMSB123X)
        {
            PRINTE(("pMSB123X error !\n"));
            return FALSE;
       }
       if(!pMSB123X->bInited ||!pMSB123X->bOpen)
       {
             PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
            return FALSE;
        }
        MS_U8 status = TRUE;
        switch (pMSB123X->enDemodType)
        {
            case E_DEVICE_DEMOD_DVB_T2:
            {
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
                if (snr_out==0)
                {
                    snr_out=1;
                }
                status &= MSB123X_ReadReg(0x2805, &reg);
                snr_ave_num = (reg>>5)&0x07;

                // unfreeze
                status &= MSB123X_ReadReg(0x2802, &reg);
                reg &= (0xff-0x02);
                status &= MSB123X_WriteReg(0x2802, reg);

                f_snr = (float)10.0 * log10((float)snr_out / fSNRDivider[snr_ave_num]);
               //PRINTE(("[DTV_GetSNR]snr_out=0x%lx, snr_ave_num=0x%x,fSNRDivider[snr_ave_num]=%f\n",snr_out,snr_ave_num,fSNRDivider[snr_ave_num]));
                g_msb123x_fSNR = f_snr;
                if (f_snr > 42) f_snr = 42;
                if (f_snr < 0) f_snr=0;
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
            }

            case E_DEVICE_DEMOD_DVB_T:
            {
                // bank 15 0xfe [0] reg_fdp_freeze
                status &= MSB123X_ReadReg(0x0ffe, &reg_frz);
                status &= MSB123X_WriteReg(0x0ffe, reg_frz|0x01);

                // bank 15 0xff [0] reg_fdp_load
                status &= MSB123X_WriteReg(0x0fff, 0x01);

                // bank 15 0x4a [26:0] reg_snr_accu <27,1>
                status &= MSB123X_ReadReg(0x0f4d, &reg);
                noise_power = reg & 0x07;

                status &= MSB123X_ReadReg(0x0f4c, &reg);
                noise_power = (noise_power << 8)|reg;

                status &= MSB123X_ReadReg(0x0f4b, &reg);
                noise_power = (noise_power << 8)|reg;

                status &= MSB123X_ReadReg(0x0f4a, &reg);
                noise_power = (noise_power << 8)|reg;

                noise_power = noise_power/2;

                // bank 15 0x26 [5:4] reg_transmission_mode
                status &= MSB123X_ReadReg(0x0f26, &reg);

                // bank 15 0xfe [0] reg_fdp_freeze
                status &= MSB123X_WriteReg(0x0ffe, reg_frz);

                // bank 15 0xff [0] reg_fdp_load
                status &= MSB123X_WriteReg(0x0fff, 0x01);

                if ((reg&0x30)==0x00) // 2K
                {
                    if (noise_power<1512)
                        f_snr = 0.0;
                    else
                        f_snr = 10*log10((float)noise_power/1512);
                }
                //else if ((reg&0x30)==0x10)//8K
                else
                {
                    if (noise_power<6048)
                        f_snr = 0.0;
                    else
                        f_snr = 10*log10((float)noise_power/6048);
                }
                g_msb123x_fSNR = f_snr;
                if (f_snr > 42) f_snr = 42;
                if (f_snr < 0) f_snr=0;
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

            }
            default:
                fe_falg = E_FE_SIGNAL_NO;
                break;
        }
        return fe_falg;
}

 MS_BOOL MSB123X_DTV_GetSNR_F(MS_U8 u8DemodIndex,float *fSNR)
{
    MSB123X_DTV_GetSNR();
    *fSNR = g_msb123x_fSNR;
    return TRUE;
}

 MS_BOOL MSB123X_DTV_GetBER(float *fBER)
{
    MS_U8 status = TRUE;
    MS_U8 reg = 0;            // reg_frz;
    MS_U16 BitErrPeriod = 0;
    MS_U32 BitErr = 0;
    MS_U16 FecType = 0;
    float fber = (float)0.0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
    switch(pMSB123X->enDemodType)
    {
         case E_DEVICE_DEMOD_DVB_T2:
         {
             PRINTE(("\nE_DEVICE_DEMOD_DVB_T2\n"));
            /////////// Check lock status /////////////
            MSB123X_ReadDspReg(0x00f0, &reg);
            if ((reg&BIT7) == 0x00)
            {
                return FALSE;
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
            if (FecType&0x0180)
                fber = (float)BitErr/(BitErrPeriod*64800);
            else
                fber = (float)BitErr/(BitErrPeriod*16200);

            PRINTE(("MSB123x Data BER = %8.3e \n ", fber));

            if (status == FALSE)
            {
                PRINTE(("MSB123X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                return FALSE;
            }
         }
        case E_DEVICE_DEMOD_DVB_T:
        case E_DEVICE_DEMOD_DVB_C:
        default:
        {
            MS_U8 u8Data = 0;
                if(MSB123X_ReadReg(0x112C, &u8Data) == FALSE)
                {
                    PRINTE(("MSB123X_DTV_GetSignalQuality MSB123X_ReadReg fail!!! \n"));
                    return 0;
                }
                if (u8Data &0x02 ) /*BIT1*/
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
                        fber = 0.5 / ((float)BitErrPeriod *128 * 188 * 8);
                    else
                        fber = (float)BitErr / ((float)BitErrPeriod *128 * 188 * 8);
                    PRINTE(("MSB123x PostVitBER = %8.3e \n ", fber));
                    PRINTE(("MSB123x PktErr = %d \n ", (int)PktErr));

                    if (status == FALSE)
                    {
                        PRINTE(("MSB123X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                        *fBER = 0;
                        return FALSE;
                    }

            }
            else
            {
               *fBER = 0;
                return FALSE;
            }
            break;
        }
    }  // end of switch
    *fBER = fber;
    return status;
}

float MSB123X_DTV_GetSignalQuality(void)
{
    float fber = (float)0.0;
    float log_ber = (float)0.0;
    float  SQI = (float)0.0;
    static float  PrevSQI = (float)0.0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    if((!MSB123X_DTV_GetBER(&fber)) || (!fber))
    	    return 0;

    switch(pMSB123X->enDemodType)
    {
        case E_DEVICE_DEMOD_DVB_T2:
        {
            log_ber = log10((double)fber);
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
            log_ber = log10((double)fber);
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

        break;
    }  // end of switch
    return 0;
}

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
MS_U16 MSB123X_DTV_GetSignalStrengthWithRFPower(float dBmValue)
{
    MS_U8    ssi = 0;
    MS_U8    status = TRUE;
    MS_U8    reg_tmp = 0;
    MS_U8    i = 0;
    MS_U8    tps_cnstl = 0;
    MS_U8    tps_cr = 0;
    float    ch_power_ref = 0.0;
    float    ch_power_rel = 0.0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if (!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if (!pMSB123X->bInited || !pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        if (dBmValue > -60.0)  // 0dbm ~ 59dbm - 100   //45
        {
            ssi = 100;
        }
        else if (dBmValue > -70.0)  // -60dbm~ -69dbm  90 ~ 99   //49
        {
            ssi = 89 + ((dBmValue - (-70.0)) * 10  / (-60.0 - (-70.0)));
        }
        else if (dBmValue > -80.0)  // -70dbm~ -79dbm  60 ~ 89    //80
        {
            ssi = 59 + ((dBmValue - (-80.0)) * 30  / (-70.0 - (-80.0)));
        }
        else if (dBmValue > -90.0)  // -80dbm ~ 10 ~ 59           //114
        {
            ssi = 9 + ((dBmValue - (-90.0)) * 50  / (-80.0 - (-90.0)));
        }
        else
        {
            ssi = 0;
        }
        PRINTE((">>> SSI = %d <<<\n", (int)ssi));
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    case E_DEVICE_DEMOD_DVB_C:
    {
        ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////
        status &= MSB123X_ReadReg(0x0f24, &reg_tmp);
        tps_cnstl = reg_tmp & 0x03;

        status &= MSB123X_ReadReg(0x0f24 + 1, &reg_tmp);
        tps_cr = (reg_tmp & 0x70) >> 4;

        for (i = 0; i < sizeof(dvbt_ssi_dbm_nordigp1) / sizeof(S_SSI_PREF); i++)
        {
            if ((tps_cnstl == dvbt_ssi_dbm_nordigp1[i].constel)
                    && (tps_cr == dvbt_ssi_dbm_nordigp1[i].code_rate))
            {
                ch_power_ref = dvbt_ssi_dbm_nordigp1[i].cn_ref;
                break;
            }
        }

        ch_power_rel = dBmValue - ch_power_ref;
        if (ch_power_rel < -15.0)
        {
            ssi = 0.0;
        }
        else if (ch_power_rel < 0.0)
        {
            ssi = (ch_power_rel + 15.0) * (10.0 / 15.0);
        }
        else if (ch_power_rel < 20.0)
        {
            ssi = ch_power_rel * 4.0 + 10.0;
        }
        else if (ch_power_rel < 35.0)
        {
            ssi = (ch_power_rel - 20.0) * (10.0 / 15.0) + 90.0;
        }
        else
        {
            ssi = 100.0;
        }

        PRINTE((">>>T SSI = %d <<<\n", (int)ssi));
    }
    break;
    default:
    {
        ssi = 0;
    }
    break;
    }
    return ssi;
}
#endif

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

    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    switch(pMSB123X->enDemodType)
    {

        case E_DEVICE_DEMOD_DVB_T2:
    {
        /***************************************
         * Force u8RFAGC 150 due to unknown SAR port for each demo board.
         ***************************************/
        // u8RFAGC=MDrv_SAR_Adc_GetValue(2);
        u8RFAGC = 150;
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
            MSB123X_WriteReg(0x3e00+0x13*2, 0x03);
            MSB123X_WriteReg(0x3e00+0x02*2, 0x00);   //set freeze & dump [Write]
            MSB123X_WriteReg(0x3e00+0x02*2+1, 0x80);

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

        for (i = 1; i < sizeof(T2IFAGC_SSI_123X) / sizeof(S_IFAGC_SSI); i++)
        {
            if (if_agc_val < T2IFAGC_SSI_123X[i].agc_val)
            {
                ch_power_if = T2IFAGC_SSI_123X[i].power_db - (float)((if_agc_val - T2IFAGC_SSI_123X[i - 1].agc_val) * 5) / (T2IFAGC_SSI_123X[i].agc_val - T2IFAGC_SSI_123X[i - 1].agc_val);
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
            reg_tmp = (reg_tmp&0xf0)|0x03;    // select ifagc_gain
            MSB123X_WriteReg(0x3e26, reg_tmp);

            // reg_agc_gain2_out
            // use only high byte value
            MSB123X_ReadReg(0x3e29, &reg_tmp);
            if_agc_val = reg_tmp;
       PRINTE(("\n IF arg=0x%x,%d\n\n",if_agc_val,if_agc_val));

      #if IF_THIS_TUNER_INUSE(TUNER_MXL201RF)
        float    ch_power_rf = 0;
            for(i = 0; i < sizeof(RFAGC_SSI_123X)/sizeof(S_RFAGC_SSI); i++)
            {
                if (rf_agc_val <= RFAGC_SSI_123X[i].sar3_val)
                {
                    ch_power_rf = RFAGC_SSI_123X[i].power_db;
                    break;
                }
            }
       #endif

            ssi_tbl_len = sizeof(IFAGC_SSI_123X)/sizeof(S_IFAGC_SSI);

            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (if_agc_val <= IFAGC_SSI_123X[i].agc_val)
                {
                    ch_power_if = IFAGC_SSI_123X[i].power_db;
                    break;
                }
            }
    #if IF_THIS_TUNER_INUSE(TUNER_MXL201RF)
            ch_power_db = ch_power_if;
    #else
            ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;
    #endif
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB123X_ReadReg(0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB123X_ReadReg(0x0f24+1, &reg_tmp);
              tps_cr = (reg_tmp & 0x70) >> 4;

              for(i = 0; i < sizeof(SsiPref_123X)/sizeof(S_SSI_PREF); i++)
              {
                if ( (tps_cnstl == SsiPref_123X[i].constel)
                    && (tps_cr == SsiPref_123X[i].code_rate) )
                {
                    ch_power_ref = SsiPref_123X[i].p_ref;
                    break;
                }
               }
        if (i >= sizeof(SsiPref_123X) / sizeof(S_SSI_PREF))
            ch_power_ref = SsiPref_123X[sizeof(SsiPref_123X) / sizeof(S_SSI_PREF) - 1].p_ref;

            ch_power_rel = ch_power_db - ch_power_ref;
            if (ch_power_rel < -15.0)
                ssi = 0.0;
            else if (ch_power_rel < 0.0)
                ssi = (ch_power_rel + 15.0)*(10.0/15.0);
            else if (ch_power_rel < 20.0)
                ssi = ch_power_rel*4.0 + 10.0;
            else if (ch_power_rel < 35.0)
                ssi = (ch_power_rel - 20.0)*(10.0/15.0) + 90.0;
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
        u8Temp = u8Temp &(~0x0F);
    }
    MSB123X_WriteReg(0x0959, u8Temp);
}

MS_BOOL MSB123X_DTV_Serial_Control(MS_BOOL bEnable)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);
    if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
    //FRA_T2_BRINGUP
    }
    else if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T||pMSB123X->enDemodType ==E_DEVICE_DEMOD_DVB_C)
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

MS_U16 MSB123X_Lock(EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
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
  static MS_U32  u32LockTimeStartDVBC =0;
       MS_U32 u32NowTime = MsOS_GetSystemTime();
  MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

      if(!pMSB123X)
      {
            PRINTE(("pMSB123X error !\n"));
            return FALSE;
      }
       if(!pMSB123X->bInited ||!pMSB123X->bOpen)
       {
            PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
            return FALSE;
       }

        switch(pMSB123X->enDemodType)
        {
            case E_DEVICE_DEMOD_DVB_T2:
            {
                u32Timeout = DVBT2_TPS_timeout;

                MSB123X_ReadDspReg(0x00f0, &u8Data);

                if((u8Data & BIT7) != 0x00)
                    bCheckPass = TRUE;
                else
                {
                        u16RegAddress =  0x00f0; //P1 Lock History
                        u8BitMask = BIT5;
                        MSB123X_ReadDspReg(u16RegAddress, &u8Data);
                       if ((u8Data & u8BitMask) != 0)
                       {
                          u32Timeout = DVBT2_FEC_timeout;
                           PRINTE(("====> Short T2 Locking Time, P1 Lock Flag = 0x%x\n", u8Data));
                        }
                }

                if(bCheckPass)
                {
                    u32LockTimeStartDVBT2 = MsOS_GetSystemTime();
                    pMSB123X->bFECLock = TRUE;
                    return E_DEMOD_LOCK;
                }
                else if ((u32NowTime - pMSB123X->u32ChkScanTimeStart < u32Timeout)||(u32NowTime - u32LockTimeStartDVBT2 < u32LockTimeout))
                {
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
                u32Timeout = DVBT2_TPS_timeout;
                u16RegAddress = 0x11E0;
                u8LockState = 0x0B;
                MSB123X_ReadReg(u16RegAddress, &u8Data);
                if(u8Data == u8LockState)
                    bCheckPass = TRUE;
                else if(u8Data == 0xBB)
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
                        u32Timeout = DVBT2_FEC_timeout;
                    }
                }

                //DBG_DEMOD_MSB(printf(">>>MSB123x: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
                if(bCheckPass)
                {
                   u32LockTimeStartDVBT = MsOS_GetSystemTime();
                   pMSB123X->bFECLock = TRUE;
                   return E_DEMOD_LOCK;
                }
                else if((u32NowTime - pMSB123X->u32ChkScanTimeStart < u32Timeout)
                        || (u32NowTime - u32LockTimeStartDVBT < u32LockTimeout))
                {
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
                u32Timeout = DVBT2_TPS_timeout;
                u16RegAddress = 0x11E0;
                u8LockState = 0x0C;
                MSB123X_ReadReg(u16RegAddress, &u8Data);
                //printf("[msb123x]+++++++++DVBC State---->%d\n",u8Data);//mick
                if(u8Data == u8LockState)
                    bCheckPass = TRUE;
                else
                {
                    u16RegAddress =  0x0990; //TPS his Lock,
                    u8BitMask = BIT3;
                    MSB123X_ReadReg(u16RegAddress, &u8Data);
                    if((u8Data & u8BitMask) > 0)
                        u32Timeout = DVBT2_FEC_timeout;
                }
                //DBG_DEMOD_MSB(printf(">>>MSB123x: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
                if(bCheckPass)
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
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    for (idx = 0; idx<sizeof(pMSB123X->pMSB123X_RegDVBT2); idx++)
    {
        if( MSB123X_WriteDspReg(idx + 0x40, pMSB123X->pMSB123X_RegDVBT2[idx])!=TRUE)
        {
            PRINTE(("T2 dsp reg init NG\n"));
            return FALSE;
        }
    }

    if( MSB123X_WriteDspReg((MS_U8)T2_TS_SERIAL, T2_TS_SERIAL_VAL) != TRUE)
    {
      PRINTE(("T2 dsp reg init NG\n"));
    }
    if( MSB123X_WriteDspReg((MS_U8)T2_TS_CLK_RATE, T2_TS_CLK_RATE_VAL) != TRUE)
    {
      PRINTE(("T2 dsp reg init NG\n"));
    }
    if( MSB123X_WriteDspReg((MS_U8)T2_TS_OUT_INV, T2_TS_OUT_INV_VAL) != TRUE)
    {
      PRINTE(("T2 dsp reg init NG\n"));
    }
    if( MSB123X_WriteDspReg((MS_U8)T2_TS_DATA_SWAP, T2_TS_DATA_SWAP_VAL) != TRUE)
    {
      PRINTE(("T2 dsp reg init NG\n"));
    }
    if( MSB123X_WriteDspReg((MS_U8)T2_TS_ERR_POL, T2_TS_ERR_POL_VAL) != TRUE)
    {
      PRINTE(("T2 dsp reg init NG\n"));
    }
    if(MSB123X_WriteDspReg((MS_U16)T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL) != TRUE)
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
MS_BOOL MSB123X_DTV_GetPlpBitMap(MS_U8 u8DemodIndex,MS_U8* u8PlpBitMap)
{
    MS_BOOL   status = TRUE;
    MS_U8     u8Data = 0;
    MS_U8     indx = 0;

    status &= MSB123X_ReadDspReg(0x0120, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
        return FALSE;

    while(indx < 32)
    {
        status &= MSB123X_ReadDspReg(0x0100+indx, &u8Data);
        u8PlpBitMap[indx] = u8Data;
        indx++;
    }
    if(status)
    {
        PRINTE(("\n+++++++++u8PlpBitMap data+++++++++++++++\n"));
        for(indx = 0;indx<32;indx++)
            PRINTE(("[%d] ",u8PlpBitMap[indx]));
        PRINTE(("\n+++++++++u8PlpBitMap end+++++++++++++++\n"));

    }
    //PRINTE(("[end, return %d]",status));
    return status;
}

/*************************************************************************************************
Subject:    Get GROUP_ID upon PLP_ID for MPLP mode
Function:   DTV_GetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB123X_DTV_GetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8* u8GroupID)
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

    if ((u8Data&BIT7) == 0x00)
    {
        PRINTE(("[msb123x]dvbt2 is un-lock\n"));
        return FALSE;
    }
    // assign PLP-ID value
    status &= MSB123X_WriteReg(0x2700+(0x78)*2, u8PlpID);
    status &= MSB123X_WriteReg(0x2700+(0x01)*2 + 1, 0x01);  // MEM_EN
    MSB123X_MDrv_Timer_Delayus(1000);
    status &= MSB123X_ReadReg(0x2700+(0x79)*2, u8GroupID);
    status &= MSB123X_WriteReg(0x2700+(0x01)*2 + 1, 0x00);  // ~MEM_EN
    PRINTE(("[end, return %d]",status));
    return status;
}


/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB123X_DTV_SetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8 u8GroupID)
{

    MS_BOOL   status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    pMSB123X->u32ChkScanTimeStart = MsOS_GetSystemTime();
    PRINTE(("[start]\n"));

    // assign Group-ID and PLP-ID value (must be written in order)
    status &= MSB123X_WriteDspReg(0x0122, u8GroupID);
    status &= MSB123X_WriteDspReg(0x0121, u8PlpID);
    PRINTE(("[end, return %d]\n",status));
    return status;
}

/************************************************************************************************
Subject:    Get PlpID List and PlpID Size for MPLP mode
Function:   MSB123X_GetPlpIDList
Parmeter:   u8PlpID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB123X_GetPlpIDList(MS_U8 u8DemodIndex)
{
    //MS_BOOL bRet;
    MS_U8 i,j,u8PlpBitMap[32];
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    pMSB123X->PlpIDSize =0;

    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));

    MSB123X_DTV_GetPlpBitMap(u8DemodIndex,u8PlpBitMap);
    for(i = 0; i < 32; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if ((u8PlpBitMap[i] >> j) & 1)
            {
                pMSB123X->PlpIDList[pMSB123X->PlpIDSize] = i * 8 + j;
                pMSB123X->PlpIDSize++;
            }
        }
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

#if 0
//????
MS_U16 MSB123X_DTV_GetCellID(void)
{
    MS_U8 id;
    MS_U8 status = TRUE;
    MS_U16  cell_id;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        _qe("pMSB123X error !");
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        _qe("pMSB123X have not inited !!!");
        return FALSE;
    }
    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);

    if (pMSB123X->enDemodType== E_DEVICE_DEMOD_DVB_T2)
    {
        MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
        return 0;
    }
    else if (pMSB123X->enDemodType== E_DEVICE_DEMOD_DVB_T)
    {
        status &= MSB123X_WriteReg(0x0ffe, 0x01);

        status &= MSB123X_ReadReg(0x0f2b, &id);
        cell_id = (MS_U16)id;

        status &= MSB123X_ReadReg(0x0f2a, &id);
        cell_id |= (MS_U16)id << 8;

        status &= MSB123X_WriteReg(0x0ffe, 0x00);

        if(status)
        {
            printf(" @MSB1231_GetCellId OK,cell_id %d\n",cell_id);
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

MS_BOOL MSB123X_T2_SetPlpID(MS_U8 u8DemodIndex,MS_U8 u8PlpID)
{
    MS_BOOL bRet = FALSE;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("pMSB123X have not inited !!!"));
        return FALSE;
    }
    {
        //MS_U32 u32Timeout;
        MS_U8 u8Data;
        MS_U16 u16RegAddress;
        MS_U8 u8LockState;
        MS_BOOL bCheckPass=FALSE;

        //u32Timeout = 1500;
        u16RegAddress = 0x0990;
        u8LockState = 0x09;
        MSB123X_ReadReg(u16RegAddress, &u8Data);
        if(u8Data==u8LockState)
            bCheckPass=TRUE;
        printf("===> T2_SetPlpID = %x\n", u8PlpID);
        if(u8PlpID != 0xFF && pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2&&bCheckPass)
        {

            MS_U16 u16Retry = 0;
            MS_U8 u8GroupId = 0;
            MSB123X_MDrv_Timer_Delayms(500);

            bRet = MSB123X_DTV_GetPlpGroupID(u8DemodIndex,u8PlpID, &u8GroupId);
            while((bRet == FALSE) && (u16Retry < 60))
            {
                u16Retry++;
                PRINTE(("DoSet_DVBT2 get groupid retry %d \n", u16Retry));
                MSB123X_MDrv_Timer_Delayus(100 * 1000);
                bRet = MSB123X_DTV_GetPlpGroupID(u8DemodIndex,u8PlpID, &u8GroupId);
            }
            if(bRet == FALSE)
            {
                PRINTE(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_GetPlpGroupID(%d) Error \n", u8PlpID));
                return FALSE;
            }

            bRet = MSB123X_DTV_SetPlpGroupID(u8DemodIndex, u8PlpID, u8GroupId);
            if(bRet == FALSE)
            {
                PRINTE(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_SetPlpGroupID(%d,%d) Error", u8PlpID, u8GroupId));
                return FALSE;
            }
        }
    }
    return TRUE;
}

MS_BOOL  MSB123X_DTV_SetFrequency(MS_U8 u8DemodIndex,MS_U32 u32Frequency, MS_U8 eBandWidth,MS_U8 u8PlpID)
{
    //MS_BOOL bRet = FALSE;
    MS_U8 bw_t2 = 0, bw_t = 0;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
    PRINTE(("\n[%s]->freq=%ld,bdwh=%d,PlpID %d,type %s\n",__FUNCTION__,u32Frequency,eBandWidth,u8PlpID,E_DEVICE_DEMOD_DVB_T == pMSB123X->enDemodType?"T":"T2"));

    pMSB123X->u32ChkScanTimeStart = MsOS_GetSystemTime();

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

    if(pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        MSB123X_Reset(u8DemodIndex);
        MSB123X_MDrv_Timer_Delayus(500);
        MSB123X_WriteDspReg((MS_U8)T_CONFIG_BW, bw_t);   // BW: 1->6M, 2->7M, 3->8M
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_CONFIG_BW - (MS_U8)T_OPMODE_RFAGC_EN] = (MS_U8)bw_t;
        pMSB123X->pMSB123X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MSB123X_DTV_DVBT_DSPReg_CRC();
        MSB123X_WriteDspReg((MS_U8)T_PARAM_CHECK_SUM, pMSB123X->pMSB123X_RegDVBT [(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]);    // BW: 1->6M, 2->7M, 3->8M
        MSB123X_Active(TRUE);
    }
    else if(pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        MSB123X_Reset(u8DemodIndex);
        MSB123X_MDrv_Timer_Delayus(500);
        MSB123X_WriteDspReg((MS_U16)T2_BW, bw_t2);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB123X_WriteDspReg((MS_U16)T2_PLP_ID, u8PlpID);
        MSB123X_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }

    else if (pMSB123X->enDemodType == E_DEVICE_DEMOD_DVB_C)
    {
        MSB123X_Reset(u8DemodIndex);
    }
    return TRUE;
}

MS_BOOL MSB123x_MEM_switch(MS_U8 mem_type)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8_tmp = 0;
    MS_U16 timeout = 0;

    PRINTE(("[start]MSB123x_MEM_switch, mem_type=0x%x\n",mem_type));

    if(mem_type == 1)
    {

        bRet &= MSB123X_WriteReg(0x2B80,0x10);

        // SRAM_START_ADDR 0x0000
        bRet &= MSB123X_WriteReg2bytes(0x1000,0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x1004,0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= MSB123X_WriteReg2bytes(0x1002,0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x1006,0x7FFF);

        // DRAM_START_ADDR 0x8000
        bRet &= MSB123X_WriteReg2bytes(0x1008,0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100C,0x8000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= MSB123X_WriteReg2bytes(0x100A,0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100E,0xFFFF);

        // Enable SRAM&SDRAM memory map
        bRet &= MSB123X_WriteReg(0x1018,0x05);

        // Wait memory map to be enabled
        do{
            bRet &= MSB123X_ReadReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                PRINTE(("@msb123x, D+S memory mapping failure.!!!\n"));
                return FALSE;
            }
        }while (u8_tmp != 0x05);
    }
    else if(mem_type == 0)
    {
        // Enable SRAM&SDRAM memory map

        bRet &= MSB123X_WriteReg(0x2B80,0x10);

        // DRAM_START_ADDR 0x8000
        bRet &= MSB123X_WriteReg2bytes(0x1008,0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100C,0x0000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= MSB123X_WriteReg2bytes(0x100A,0x0000);
        bRet &= MSB123X_WriteReg2bytes(0x100E,0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= MSB123X_WriteReg(0x1018,0x04);

        // Wait memory map to be enabled
        do{
            bRet &= MSB123X_ReadReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
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
    PRINTE(("[end]MSB123x_MEM_switch, , mem_type=0x%x, bRet=0x%x\n",mem_type,bRet));

    return bRet;
}

// i2c 150Kb/s, average rate 110Kb/s, 32KB, 2436ms.
MS_BOOL MSB123X_Load2Sdram(MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sdram_win_offset_base)
{
    MS_BOOL bRet = TRUE;
    MS_U16  sdram_win_offset = sdram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;
    PRINTE(("[msb123x][beg]MSB123X_Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));
    if (!MSB123X_I2C_CH_Reset(0,FALSE))
    {
        PRINTE((">>>MSB123x CH0 Reset:Fail\n"));
        return FALSE;
    }
    else
    {
        // set xData map upper and low bound for 64k DRAM window
        bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x63)*2, 0x3F24);

        // set xData map offset for 64k DRAM window, 64kbytes alignment
        bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x64)*2, 0x0000);

        // set xData map upper and low bound for 4k DRAM window
        bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x65)*2, 0x2014);

        // set xData map offset for 4k DRAM window, 4kbytes alignment
        bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);

        // enable xData map for DRAM
        bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x62)*2, 0x0007);

        for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
        {
            if (y_cir_addr == 0x6000)
            {
                //set xData map offset for 4k DRAM window, 4kbytes alignment
                // 0x1000, 4096 bytes
                bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                y_cir_addr = SDRAM_BASE;
            }

            // max 0x200, error above.....
            if((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
            {
                bRet &= MSB123X_WriteRegs(y_cir_addr, (u8_ptr + x_data_offset),LOAD_CODE_I2C_BLOCK_NUM);
                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
            }
            else
            {
                bRet &= MSB123X_WriteRegs(y_cir_addr, (u8_ptr + x_data_offset),data_length - x_data_offset);
                y_cir_addr += (data_length - x_data_offset);
                x_data_offset += (data_length - x_data_offset);
            }
        }
       PRINTE(("x_data_offset=%d,y_cir_addr=%d,z_block_num=%d\n",x_data_offset,y_cir_addr,sdram_win_offset));
        //  Release xData map for SDRAM
        bRet &= MSB123X_WriteReg2bytes(0x2B00+(0x62)*2, 0x0000);

        // Channel changed from CH 0x00 to CH 0x03
        if (!MSB123X_I2C_CH_Reset(3,FALSE))   // if (!MSB123X_I2C_CH_Reset(3,TRUE))//HARRY
        {
            PRINTE((">>>MSB123x CH3 Reset:Fail\n"));
            return FALSE;
        }
    }
    PRINTE(("[end]MSB123X_Load2Sdram, len=0x%x, win_offset=0x%x,bRet= 0x%02x\n",data_length,sdram_win_offset_base,bRet));
    return bRet;
}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_Boot(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    PRINTE(("[start] BOOT\n"));

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
         PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    if(!(pMSB123X->u8SdramCode & MSB123x_BOOT))
    {
        if(pMSB123X->u32MSB123XlibSize > WIMBLEDON_BOOT_START_ADDR)
        {
            // boot code
            data_ptr = pMSB123X->pMSB123XLib + WIMBLEDON_BOOT_START_ADDR;
            code_size = WIMBLEDON_BOOT_END_ADDR - WIMBLEDON_BOOT_START_ADDR + 1;
            win_offset = WIMBLEDON_BOOT_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == TRUE)
            {
                pMSB123X->u8SdramCode |= MSB123x_BOOT;
            }
        }
        else
        {
            PRINTE(("@msb123x, boot code is unavailable!!!\n"));
        }
    }
     PRINTE(("[end]BOOT bRet 0x%02x\n",bRet));
    return bRet;

}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_dvbt2(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    PRINTE(("[start] DVBT2\n"));
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
         PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    if( !(pMSB123X->u8SdramCode&MSB123x_DVBT2) )
    {
        if(pMSB123X->u32MSB123XlibSize> WIMBLEDON_DVBT2_P1_START_ADDR)
        {
            // dvbt2_p2
            data_ptr = pMSB123X->pMSB123XLib  + WIMBLEDON_DVBT2_P2_START_ADDR;
            code_size = WIMBLEDON_DVBT2_P2_END_ADDR - WIMBLEDON_DVBT2_P2_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBT2_P2_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr,code_size,win_offset);

            // dvbt2_p1
            data_ptr = pMSB123X->pMSB123XLib  + WIMBLEDON_DVBT2_P1_START_ADDR;
            code_size = WIMBLEDON_DVBT2_P1_END_ADDR - WIMBLEDON_DVBT2_P1_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBT2_P1_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr,code_size,win_offset);

            if(bRet == TRUE)
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
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
         PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    if(!(pMSB123X->u8SdramCode&MSB123x_DVBT))
    {
        // dvbt code
        if(pMSB123X->u32MSB123XlibSize > WIMBLEDON_DVBT_START_ADDR)
        {
         data_ptr = pMSB123X->pMSB123XLib + WIMBLEDON_DVBT_START_ADDR;
         code_size = WIMBLEDON_DVBT_END_ADDR - WIMBLEDON_DVBT_START_ADDR + 1;
         win_offset = WIMBLEDON_DVBT_WINDOWS_OFFSET;
         bRet &= MSB123X_Load2Sdram(data_ptr,code_size,win_offset);
         if(bRet == TRUE)
         {
             pMSB123X->u8SdramCode |= MSB123x_DVBT;
         }
        }
        else
        {
         PRINTE(("@msb123x, dvbt code is unavailable!!!\n"));
        }
    }
    PRINTE(("[end]MSB123X_LoadDspCodeToSDRAM_dvbt,bRet=0x%02x\n",bRet));
    return bRet;
}

MS_BOOL MSB123X_LoadDspCodeToSDRAM_dvbc(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
         PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    if(!(pMSB123X->u8SdramCode&MSB123x_DVBC))
    {
        // dvbc code
        if(pMSB123X->u32MSB123XlibSize> WIMBLEDON_DVBC_START_ADDR)
        {
            data_ptr = pMSB123X->pMSB123XLib  + WIMBLEDON_DVBC_START_ADDR;
            code_size = WIMBLEDON_DVBC_END_ADDR - WIMBLEDON_DVBC_START_ADDR + 1;
            win_offset = WIMBLEDON_DVBC_WINDOWS_OFFSET;
            bRet &= MSB123X_Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == TRUE)
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
    PRINTE(("[start]code_n=0x%02x\n",code_n));

    switch(code_n)
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
    PRINTE(("[end]MSB123X_LoadDspCodeToSDRAM, code_n=0x%x, bRet=0x%x\n",code_n,bRet));
    return bRet;
}

MS_BOOL MSB123X_LoadSdram2Sram(MS_U8 CodeNum)
{
    PRINTE(("[start],CodeNum %d\n",CodeNum));

    MS_BOOL bRet = TRUE;
    MS_U8   u8_tmp = 0;
    MS_U8   u8DoneFlag = 0;
    MS_U32  u32Timeout = 0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
        PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    if(CodeNum == pMSB123X->u8sramCode)
    {
       PRINTE(("MSB123X_LoadSdram2Sram, code is available.\n"));
        return bRet;
    }

    bRet &= MSB123x_MEM_switch(0);

    if(CodeNum == MSB123x_DVBT2)
    {
        u8_tmp = 1|0x10;}
    else if(CodeNum == MSB123x_DVBT)
        u8_tmp = 2|0x10;
    else if(CodeNum == MSB123x_DVBC)
        u8_tmp = 3|0x10;
    else
        u8_tmp = 0|0x10;

    // Assign f/w code type to load => 0x11: dvbt2, 0x12: dvbt, 0x13: dvbc
    bRet &= MSB123X_WriteReg(0x0900+(0x4f)*2, u8_tmp);

    // enable miu access of mcu gdma
    //bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xf0);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x0f);

    bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xf0);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    // enable mcu
    bRet &= MSB123X_WriteReg(0x0b00+(0x19)*2, 0x00);

    do
    {
        bRet &= MSB123X_ReadReg(0x0900+(0x4f)*2, &u8DoneFlag);

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
    bRet &= MSB123X_WriteReg(0x0b00+(0x19)*2, 0x03);

    // enable miu mask, miu, mcu, gdma
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2 + 1, 0x0f);
    bRet &= MSB123X_WriteReg(0x1200 + (0x23) * 2, 0xf0);

    bRet &= MSB123x_MEM_switch(1);

    if(bRet == FALSE)
    {
        pMSB123X->u8sramCode= 0x00;
    }
    else
    {
        pMSB123X->u8sramCode= CodeNum;
    }

    PRINTE(("[end]return %d\n",bRet));
    return bRet;
}

MS_BOOL  MSB123X_PreLoadDSPcode(void)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8CodeNum = 0;

    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;

    if(!pMSB123X)
    {
        PRINTE(("pMSB123X error !\n"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
         PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }

    MsOS_ObtainMutex(pMSB123X->s32_MSB123X_Mutex, MSOS_WAIT_FOREVER);

    bRet &= MSB123X_LoadDspCodeToSDRAM(MSB123x_ALL);
    if(bRet == FALSE)
    {
        PRINTE((" MSB123X_LoadDspCodeToSDRAM failure...\n"));
    }

    switch(pMSB123X->enDemodType)
    {
        case E_DEVICE_DEMOD_DVB_T2:
            u8CodeNum=MSB123x_DVBT2;
            //bRet &= MSB123X_LoadDspCodeToSDRAM(MSB123x_DVBT2);
            break;
        case E_DEVICE_DEMOD_DVB_T:
            u8CodeNum=MSB123x_DVBT;
            //bRet &= MSB123X_LoadDspCodeToSDRAM(MSB123x_DVBT);
            break;
        case E_DEVICE_DEMOD_DVB_C:
            u8CodeNum=MSB123x_DVBC;
            break;
        default:
            u8CodeNum = MSB123x_DVBT;
            MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
            break ;
    }
    // mask miu access for all and
    bRet &= MSB123X_WriteReg(0x1200+(0x23)*2 + 1,0x7f);
    bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xfe);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);

    bRet &= MSB123X_WriteReg(0x0b00+(0x19)*2, 0x03);

    bRet &= MSB123X_LoadSdram2Sram(u8CodeNum);

    // enable miu access of mcu gdma
    bRet &= MSB123X_WriteReg(0x1200+(0x23)*2,0xf0);

    // 10us delay
    MSB123X_MDrv_Timer_Delayus(10);
    // Enable MCU
    bRet &= MSB123X_WriteReg(0x0b00+(0x19)*2, 0x00);

    switch(pMSB123X->enDemodType)
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
    PRINTE(("[end],ret %d\n",bRet));
    return bRet;
}

MS_BOOL MSB123X_GetPreLoadStatus(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    if(!pMSB123X)
    {
       PRINTE(("pMSB123X error !"));
        return FALSE;
    }
    if(!pMSB123X->bInited ||!pMSB123X->bOpen)
    {
         PRINTE(("[%s]pMSB123X have not inited !!!\n",__FUNCTION__));
        return FALSE;
    }
    return pMSB123X->bLoaded;
}

MS_BOOL MSB123X_Finalize(MS_U8 u8DemodIndex)
{
    if(MSB123X.bInited == TRUE)
    {
        //release mutex
        if(MSB123X.s32_MSB123X_Mutex)
            MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);

        if(MSB123X.s32_MSB123X_RW_Mutex)
            MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);

        if(MSB123X.s32_MSB123X_RW2_Mutex)
            MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);

        memset(&MSB123X, 0x0, sizeof(MSB123X));
    }
    return TRUE;
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
    return FALSE;
}

MS_BOOL MSB123X_SetCurrenDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    MS_U8 bRet = TRUE, u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    printf("MSB123X_SetCurrenDemodType start!!\n");
    if (!pMSB123X)
    {
        printf("pMSB123X error !");
        return FALSE;
    }
    if ((!pMSB123X->bInited) || (!pMSB123X->bOpen))
    {
        printf("[%s]pMSB123X have not inited !!!\n", __FUNCTION__);
        return FALSE;
    }
    printf("enDemodType = %x pMSB123X->enDemodType = %x\n",enDemodType,pMSB123X->enDemodType);
    if (enDemodType == pMSB123X->enDemodType)
    {
        if (FALSE == MSB123X_DTV_TUNER_IsIFChanged())
        {
            printf("!!!!!! FALSE == MSB123X_DTV_TUNER_IsIFChanged()\n");
            return TRUE;
        }
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
        printf("######################\n");
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

    printf("pMSB123X->enDemodType = %x\n", pMSB123X->enDemodType);
    switch (pMSB123X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        printf("MSB123X_DTV_DVBT2_DSPReg_Init\n");
        MSB123X_DTV_DVBT2_DSPReg_Init();
        break;
    case E_DEVICE_DEMOD_DVB_T:
        printf("MSB123X_DTV_DVBT_DSPReg_Init\n");
        MSB123X_DTV_DVBT_DSPReg_Init();
        break;
    default :
        printf("default\n");
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


MS_U16 MSB1231_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    MS_U8 status = TRUE;
    //DVB-T
    float   N, FreqB;
    float   FreqCfoTd, FreqCfoFd, FreqIcfo;
    MS_U32  RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8 reg_frz, reg, reg_debug_sel;
    MS_U8 sign_data;

    EN_DEVICE_DEMOD_TYPE eSystems;


    FreqB = (float)u8BW * 8.0 / 7.0;
    eSystems = MSB123X_GetCurrenDemodType();
    PRINTE(("\nBW = %d\n", u8BW));
    if (eSystems == E_DEVICE_DEMOD_DVB_T2)
    {
        //debug select
        status &= MSB123X_ReadReg(0x3E5E, &reg_debug_sel);
        reg_debug_sel &= 0xCF;
        reg_debug_sel |= 0x10;
        status &= MSB123X_WriteReg(0x3E5E, reg_debug_sel);

        // set freeze
        status &= MSB123X_WriteReg(0x3E04, 0x00);
        status &= MSB123X_WriteReg(0x3E05, 0x80);

        status &= MSB123X_ReadReg(0x3E5B, &reg);
        RegCfoTd = reg & 0x0F;
        sign_data = reg;

        status &= MSB123X_ReadReg(0x3E5A, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        status &= MSB123X_ReadReg(0x3E59, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        status &= MSB123X_ReadReg(0x3E58, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        // release freeze
        status &= MSB123X_WriteReg(0x3E04, 0x00);
        status &= MSB123X_WriteReg(0x3E05, 0x00);

        FreqCfoTd = (float)RegCfoTd;
        if (RegCfoTd >= 0x4000000)
            FreqCfoTd = FreqCfoTd - (float)0x8000000;

        *pFreqOff = FreqCfoTd * FreqB * 0.00000745;

        if (!(sign_data & 0x40))
            *pFreqOff = -*pFreqOff;
        *pFreqOff = -*pFreqOff;
        PRINTE(("$$$$$$$$$$$$$$$$$$$$  DVB-T2 CFOE = %f\n", *pFreqOff));
        return status;
    }
    else if (eSystems == E_DEVICE_DEMOD_DVB_T)
    {
        // freeze
        status &= MSB123X_ReadReg(0x0E04 + 1, &reg_frz);
        status &= MSB123X_WriteReg(0x0E04 + 1, reg_frz | 0x80);

        status &= MSB123X_ReadReg(0x0E28 + 2, &reg);
        RegCfoTd = reg;

        status &= MSB123X_ReadReg(0x0E28 + 1, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        status &= MSB123X_ReadReg(0x0E28, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        // release
        status &= MSB123X_ReadReg(0x0E04 + 1, &reg_frz);
        status &= MSB123X_WriteReg(0x0E04 + 1, reg_frz | 0x80);

        FreqCfoTd = (float)RegCfoTd;

        if (RegCfoTd & 0x800000)
            FreqCfoTd = FreqCfoTd - (float)0x1000000;

        FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

        status &= MSB123X_ReadReg(0x0ffe, &reg_frz);
        status &= MSB123X_WriteReg(0x0ffe, reg_frz | 0x01);

        status &= MSB123X_WriteReg(0x0fff, 0x01);

        status &= MSB123X_ReadReg(0x0f33, &reg);
        RegCfoFd = reg;

        status &= MSB123X_ReadReg(0x0f32, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        status &= MSB123X_ReadReg(0x0f31, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        FreqCfoFd = (float)RegCfoFd;

        if (RegCfoFd & 0x800000)
            FreqCfoFd = FreqCfoFd - (float)0x1000000;

        FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

        status &= MSB123X_ReadReg(0x0f19, &reg);
        RegIcfo = reg & 0x07;

        status &= MSB123X_ReadReg(0x0f18, &reg);
        RegIcfo = (RegIcfo << 8) | reg;

        FreqIcfo = (float)RegIcfo;

        if (RegIcfo & 0x400)
            FreqIcfo = FreqIcfo - (float)0x800;

        status &= MSB123X_ReadReg(0x0f26, &reg);
        reg = reg & 0x30;

        switch (reg)
        {
        case 0x00:
            N = 2048;
            break;
        case 0x20:
            N = 4096;
            break;
        case 0x10:
        default:
            N = 8192;
            break;
        }

        FreqIcfo = FreqIcfo * FreqB / N * 1000;     //unit: kHz
        status &= MSB123X_WriteReg(0x0ffe, reg_frz & (~0x01));
        status &= MSB123X_WriteReg(0x0fff, 0x01);

        *pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd) / 1000;

        status &= MSB123X_ReadReg(0x3E5E, &reg);

        if (reg & 0x01)
            *pFreqOff = -*pFreqOff;

        PRINTE(("$$$$$$$$$$$$$$$$$$$$  DVB-T CFOE = %f\n", *pFreqOff));

    }
    else
        ;

    if (status)
    {
        PRINTE((" @MSB1231_Get_FreqOffset OK\n"));
        return  TRUE;
    }
    else
    {
        PRINTE((" @MSB1231_Get_FreqOffset NG\n"));
        return FALSE;
    }
}

MS_U8 MSB1231_Get_Packet_Error(MS_U16 *u16_data)
{
    MS_U8     status = TRUE;
    MS_U8     reg = 0, reg_frz;
    MS_U16      PktErr = 0;

    if (E_DEVICE_DEMOD_DVB_T2 == MSB123X_GetCurrenDemodType())
    {
        //freeze
        status &= MSB123X_WriteReg(0x2604, 0x01);
        //read packet error
        status &= MSB123X_ReadReg(0x265B, &reg);
        PktErr = reg;
        status &= MSB123X_ReadReg(0x265A, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;
        //release
        status &= MSB123X_WriteReg(0x2604, 0x00);
    }
    else
    {
        reg_frz = 0;
        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB123X_ReadReg(0x1132, &reg_frz);
        status &= MSB123X_WriteReg(0x1132, reg_frz | 0x80);

        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //         [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB123X_ReadReg(0x113F, &reg);
        PktErr = reg;

        status &= MSB123X_ReadReg(0x113E, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB123X_WriteReg(0x1132, reg_frz);

        PRINTE(("===================>MSB PktErr = %d \n ", (int)PktErr));
    }
    if (status)
    {
        PRINTE((" @MSB1231_Get_Packet_Error OK\n"));
        return  TRUE;
    }
    else
    {
        PRINTE((" @MSB1231_Get_Packet_Error NG\n"));
        return FALSE;
    }
}

//show firmwear version
MS_BOOL MSB123X_Demod_Get_DVB_FW_VER(MS_U16 *FW_ver)
{
    MS_U8 reg;
    MS_U16 u16tmp;
    MSB123X_ReadDspReg(0x02, &reg);
    u16tmp = reg;
    MSB123X_ReadDspReg(0x03, &reg);
    u16tmp = (u16tmp << 8) + reg;
    *FW_ver = u16tmp;
    return TRUE;
}


MS_U16 MSB123X_Demod_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
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
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1231_Get_FreqOffset(pFreqOff, u8BW);

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_U8 MSB123X_Demod_Get_Packet_Error(MS_U16 *u16_data)
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
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1231_Get_Packet_Error(u16_data);

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_BOOL MSB123X_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
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
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB123X_IIC_Bypass_Mode(u8DemodIndex,bOn);

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_BOOL MSB123X_Demod_SetScanTypeStatus(MS_U8 u8DemodIndex,MS_U8 status)
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
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
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

MS_U8 MSB123X_Demod_GetScanTypeStatus(MS_U8 u8DemodIndex)
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

MS_BOOL MSB123X_Demod_InitParameter(void)
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
    pMSB123X->u8ScanStatus = 0;
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_U8 MSB123X_Demod_GetPlpIDSize(void)
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

MS_U8 MSB123X_Demod_GetPlpIDList(MS_U8 u8DemodIndex)
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

    if (FALSE == MSB123X_GetPlpIDList(u8DemodIndex))
    {
        pMSB123X->PlpIDSize = 0;
    }

    return pMSB123X->PlpIDSize;
}

MS_U8 MSB123X_Demod_GetNextPlpID(MS_U8 u8DemodIndex, MS_U8 u8Index, MS_U8* pu8PLPID)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    MS_U8 u8PlpID;

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
        PRINTE(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u8PlpID = pMSB123X->PlpIDList[u8Index];
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return u8PlpID;
}

MS_BOOL MSB123X_Demod_SetCurrentDemodType(MS_U8 u8DemodIndex,MS_U8  type)
{
    if (E_DEMOD_TYPE_T == type)
    {
        printf("@@@@ Set Current Type DVBT\n");
        return MSB123X_SetCurrenDemodType(E_DEVICE_DEMOD_DVB_T);
    }
    else
    {
        printf("@@@@ Set Current Type DVBT2\n");
        return MSB123X_SetCurrenDemodType(E_DEVICE_DEMOD_DVB_T2);
    }
}

MS_U8 MSB123X_Demod_GetCurrentDemodType(MS_U8 u8DemodIndex)
{
    if (E_DEVICE_DEMOD_DVB_T == MSB123X_GetCurrenDemodType())
        return E_DEMOD_TYPE_T;
    else
        return E_DEMOD_TYPE_T2;
}

MS_BOOL MSB123X_Demod_Open(MS_U8 u8DemodIndex)
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
        PRINTE(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB123X->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return TRUE;
}

MS_BOOL MSB123X_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MSB123X_Demod_Close(void)
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

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB123X_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfSNR)
{
    MS_BOOL bret = TRUE;

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
        PRINTE(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pfSNR = 0;
        if (E_DEMOD_LOCK == MSB123X_DTV_DVB_T_GetLockStatus())
        {
            bret = (MS_U32)MSB123X_DTV_GetSNR_F(u8DemodIndex, pfSNR);
        }
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return bret;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB123X_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL bret = TRUE;

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
        PRINTE(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pfBER = 0;
        if (E_DEMOD_LOCK == MSB123X_DTV_DVB_T_GetLockStatus())
        {
            bret = (MS_U32)MSB123X_DTV_GetBER(pfBER);
        }
    }
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return bret;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB123X_Demod_GetSignalQuality(MS_U8 u8DemodIndex,MS_U16 *pu16quality)
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
        PRINTE(("MDrv_Demod_GetBER:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    *pu16quality = (MS_U16)MSB123X_DTV_GetSignalQuality();
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);

    return TRUE;
}

MS_BOOL MSB123X_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
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
#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
            float fSignal;
            MSB123X_IIC_Bypass_Mode(u8DemodIndex,TRUE);
            if (MXL_SUCCESS != MxLWare603_API_ReqTunerRxPower(0,MXL603_I2C_ADDR, &fSignal))
            {
                PRINTE(("MxLWare603_API_ReqTunerRxPower failed !!!\n"));
                return FALSE;
            }
            MSB123X_IIC_Bypass_Mode(u8DemodIndex,FALSE);

            *ps32Signal = (MS_U32)MSB123X_DTV_GetSignalStrengthWithRFPower(fSignal);
#else
            *ps32Signal = (MS_U32)MSB123X_DTV_GetSignalStrength();
#endif
        }
        if (*ps32Signal > 100)
            *ps32Signal = 100;
        else if (*ps32Signal < 0)
            *ps32Signal = 0;
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

MS_BOOL MSB123X_Demod_GetLock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    MDvr_CofdmDmd_CONFIG *pMSB123X = &MSB123X;
    *peLockStatus = E_DEMOD_CHECKING;
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
        PRINTE(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *peLockStatus =MSB123X_DTV_DVB_T_GetLockStatus();
    }

    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return TRUE;
}

/****************************************************************************
**function:reset bw and Plpid to demod when plp changed
**
**
****************************************************************************/
MS_BOOL MSB123X_Demod_T2MI_Restart(MS_U8 u8DemodIndex)
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

    if (E_DEMOD_TYPE_T2 != MSB123X_Demod_GetCurrentDemodType(u8DemodIndex))
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

/****************************************************************************
**function:set the freq , bw and Plpid to demod and tuner
**
**
****************************************************************************/
MS_BOOL MSB123X_Demod_Restart(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM *pParam,MS_U32 u32BroadCastType)
{
    MS_U32              dwError = TRUE;
    MS_U8               BandWidth = DEMOD_BW_MODE_8MHZ;
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
        PRINTE(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB123X->enTuneBw = pParam->TerParam.eBandWidth;

    if (DEMOD_BW_MODE_1_7MHZ == pParam->TerParam.eBandWidth)
    {
        MSB123X_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T2);
    }
    else if ((E_DEMOD_TYPE_T == pParam->TerParam.u8ScanType) && (pMSB123X->u8CurrScanType != pParam->TerParam.u8ScanType))
    {
        MSB123X_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T);
    }
    else if ((E_DEMOD_TYPE_T2 == pParam->TerParam.u8ScanType) && (pMSB123X->u8CurrScanType != pParam->TerParam.u8ScanType))
    {
        MSB123X_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T2);
    }
    else
        ;

    switch (pParam->TerParam.eBandWidth)
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


    if ((TRUE == pMSB123X->bFECLock) && (pMSB123X->u8CurrScanType == pParam->TerParam.u8ScanType) && \
            (E_DEMOD_TYPE_T2 == pMSB123X->u8CurrScanType) && (pMSB123X->u32CurrFreq == pParam->u32Frequency) && \
            (pMSB123X->enCurrBW == pParam->TerParam.eBandWidth))
    {
    }
    else
    {
        //set freq and bw to tuner
        MSB123X_IIC_Bypass_Mode(u8DemodIndex,TRUE);
#if IF_THIS_TUNER_INUSE(TUNER_MXL603)
        MDrv_Tuner_SetTuner(0,pParam->u32Frequency, BandWidth);
#endif
        MSB123X_IIC_Bypass_Mode(u8DemodIndex,FALSE);
        dwError = MSB123X_DTV_SetFrequency(u8DemodIndex,pParam->u32Frequency, BandWidth, pParam->TerParam.u8PlpID);
    }

    pMSB123X->u32CurrFreq = pParam->u32Frequency;
    pMSB123X->enCurrBW = pParam->TerParam.eBandWidth;
    pMSB123X->u8CurrPlpID = pParam->TerParam.u8PlpID;
    pMSB123X->u8CurrScanType = pParam->TerParam.u8ScanType;

    if (E_DEMOD_TYPE_T2 == MSB123X_Demod_GetCurrentDemodType(u8DemodIndex))
    {
        dwError &= MSB123X_T2_SetPlpID(u8DemodIndex,pParam->TerParam.u8PlpID);
    }
    PRINTE((" ==return %lx==\n", dwError));
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
    return dwError;
}

/****************************************************************************
**
**function: Reset DJB when MPLP T2MI stream occur PAT timeout
**
****************************************************************************/
void MSB123X_Demod_CtrlResetDJBFlag(MS_U8 Ctrl)
{
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
                if (bch_eflag2_sum == 0x00 && ((MsOS_GetSystemTime() - t1) > 1000))
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
    MsOS_ReleaseMutex(pMSB123X->s32_MSB123X_Mutex);
}

/****************************************************************************
**function: init demod
**              init tuner though bypass
**              set the TS mode: parallel or serial
****************************************************************************/
MS_BOOL MSB123X_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet = FALSE;
    if(MSB123X.bInited)
          return TRUE;
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
    MSB123X.u8ScanStatus = 0;
    MSB123X.u32CurrFreq = 0;
    MSB123X.enCurrBW = DEMOD_BW_MODE_8MHZ;
    MSB123X.enTuneBw = DEMOD_BW_MODE_8MHZ;
    MSB123X.u8CurrPlpID = 0xff;
    MSB123X.enDemodType = E_DEVICE_DEMOD_DVB_T;
    MSB123X.u8CurrScanType = E_DEMOD_TYPE_T;
    //MSB123X.bDoReset = FALSE;
    MSB123X.s32_MSB123X_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2", MSOS_PROCESS_SHARED);
    MSB123X.s32_MSB123X_RW_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2RW", MSOS_PROCESS_SHARED);
    MSB123X.s32_MSB123X_RW2_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2RW2", MSOS_PROCESS_SHARED);

    if (MSB123X.s32_MSB123X_Mutex< 0 ||MSB123X.s32_MSB123X_RW_Mutex < 0||MSB123X.s32_MSB123X_RW2_Mutex < 0)
    {
        PRINTE(("creat MSB123X Mutex error!!!\n"));
        return FALSE;
    }
    //void MApi_Demod_HWReset(MS_U8 drv_frontend_index);
    //MApi_Demod_HWReset(u8DemodIndex);
    bRet = MSB123X_I2C_CH_Reset(3, TRUE);
    if (!bRet)
    {
        MSB123X.bInited = FALSE;
        MSB123X.bOpen = FALSE;
        MSB123X.bLoaded = FALSE;

        PRINTE(("[%s]  MSB123X I2C CH reset error!!!\n",__FUNCTION__));
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);
        return FALSE;
    }

    bRet= MSB123X_HW_init();
    if(!bRet)
    {
        MSB123X.bInited = FALSE;
        MSB123X.bOpen = FALSE;
        MSB123X.bLoaded = FALSE;

        PRINTE(("[%s]  MSB123X HW init error!!!\n",__FUNCTION__));
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);
        return FALSE;
    }
    bRet = MSB123X_PreLoadDSPcode();
    if(!bRet)
    {
        MSB123X.bInited = FALSE;
        MSB123X.bOpen = FALSE;
        MSB123X.bLoaded = FALSE;

        PRINTE(("[%s]  MSB123X pre load dspceode error!!!\n",__FUNCTION__));
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW_Mutex);
        MsOS_DeleteMutex(MSB123X.s32_MSB123X_RW2_Mutex);
        return FALSE;
    }
    MSB123X.bLoaded = TRUE;
    MSB123X_Demod_SetCurrentDemodType(u8DemodIndex,MSB123X.u8CurrScanType);
    //show dvbt2 dvbt fw version
/*
    MS_U16 FW_VER;
    printf("Demod Version:%s\n", DemodVersion);
    MSB123X_Demod_Get_DVB_FW_VER(&FW_VER);
    printf("Demod FWT2 Version is %x\n", FW_VER);
    MSB123X_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T);
    MSB123X.u8CurrScanType = E_DEMOD_TYPE_T;
    MSB123X_Demod_Get_DVB_FW_VER(&FW_VER);
    printf("Demod FWT Version is %x\n", FW_VER);
*/

#if TS_PARALLEL_OUTPUT
    MSB123X_DTV_Serial_Control(FALSE);
#else
    MSB123X_DTV_Serial_Control(TRUE);
#endif

    return TRUE;
}

#define MSB1233X_CHIP_ID 0x19
MS_BOOL MSB123X_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;

    if(!MSB123X_I2C_CH_Reset(3,TRUE))
    {
        printf("[MSB123X] I2C_CH_Reset fail \n");
        //sreturn FALSE;
    }

    if (MSB123X_ReadReg(0x0900, &u8_tmp) == FALSE )
    {
        printf("[MSB123X] read ID fail\n");
    }

    printf("[MSB123X] read id :%x \n",u8_tmp );

    return (u8_tmp == MSB1233X_CHIP_ID);
}

MS_BOOL MSB123X_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
#if MS_DVBT2_INUSE
    MS_U8 *Ctrl = 0;
#endif
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_OPEN:
            bret &= MSB123X_Demod_Open(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_RESET:
            MSB123X_Reset(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_FINALIZE:
            bret &= MSB123X_Finalize(u8DemodIndex);
            break;
#if MS_DVBT2_INUSE
        case DEMOD_EXT_FUNC_GET_PLPID_LIST:
            bret &=MSB123X_GetPlpIDList(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_T2MI_RESTART:
            bret &=MSB123X_Demod_T2MI_Restart(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_CTRL_RESET_DJB_FLAG:
            Ctrl=(MS_U8 *)data;
            MSB123X_Demod_CtrlResetDJBFlag(*Ctrl);
            break;
        case DEMOD_EXT_FUNC_INIT_PARAMETER:
            bret &=MSB123X_Demod_InitParameter();
            break;
#endif
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            break;
    }
    return bret;
}

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB123X) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSB123X",
     .data                         = DEMOD_MSB123X,
     .init                         = MSB123X_Demod_Init,
     .GetLock                      = MSB123X_Demod_GetLock,
     .GetSNR                       = MSB123X_Demod_GetSNR,
     .GetBER                       = MSB123X_Demod_GetBER,
     .GetPWR                       = MSB123X_Demod_GetPWR,
     .GetQuality                   = MSB123X_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MSB123X_Demod_Restart,
     .I2CByPass                    = MSB123X_IIC_Bypass_Mode,
     .CheckExist                   = MSB123X_Check_Exist,
     .Extension_Function           = MSB123X_Extension_Function,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MSB123X_Demod_SetCurrentDemodType,
     .GetCurrentDemodType          = MSB123X_Demod_GetCurrentDemodType,
     .GetPlpBitMap                 = MSB123X_DTV_GetPlpBitMap,
     .GetPlpGroupID                = MSB123X_DTV_GetPlpGroupID,
     .SetPlpGroupID                = MSB123X_DTV_SetPlpGroupID,
#endif
     .SetScanTypeStatus            = MSB123X_Demod_SetScanTypeStatus,
     .GetScanTypeStatus            = MSB123X_Demod_GetScanTypeStatus,
     .GetNextPLPID                 = MSB123X_Demod_GetNextPlpID,
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};

#endif
