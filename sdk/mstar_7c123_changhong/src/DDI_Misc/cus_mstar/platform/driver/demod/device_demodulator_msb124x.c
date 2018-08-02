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
// 6. Nothing contained herein shall be consTRUEd as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and consTRUEd in accordance with the laws
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
 **********************************************************************/

#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "MsOS.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvGPIO.h"
#include <string.h>
#ifdef DDI_MISC_INUSE
#include "SysBsp.h"
#endif

#define USE_UTOPIA
#if IF_THIS_DEMOD_INUSE(DEMOD_MSB124X)
#include "drvDMD_EXTERN_MSB124x.h"
#include "device_demodulator_msb124x.h"
#include "drvSYS.h"
#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
#include "drvDMD_common.h"
#include "drvMSPI.h"
#ifndef USE_SPI_LOAD_TO_SDRAM
#define USE_SPI_LOAD_TO_SDRAM
#endif
#endif
extern MS_U8 MSB124X_LIB[];

#define UNUSED(var)     (void)((var) = (var))
#define MSB124X_DEMOD_IIC 1
#define MSB124X_SPI_IIC 2
#define MSB124X_DEBUG_ON 1

#define MSB124X_NO_CHANNEL_CHECK 1

#define MSB124X_Delayus   MsOS_DelayTaskUs
#define MDrv_Timer_GetTime0  OS_SYSTEM_TIME

#define PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 0
#define LOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 1
#define SDRAM_DATA_CHECK                 0
#define TIMING_VERIFICATION              0
#define MSB124X_USE_DVBT2_ROM       0

#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define SDRAM_BASE                       0x5000
#define SRAM_BASE                        0x8000

// MSB124X flash size: 1Mbits
#define MAX_MSB124X_LIB_LEN             131072
#define MSB124X_BOOT  0x01
#define MSB124X_DVBT2 0x02
#define MSB124X_DVBT  0x04
#define MSB124X_DVBC  0x08
#define MSB124X_ALL   0x0F

#define REG_MB_CNTL         0x0C80
#define REG_MB_ADDR_L       0x0C82
#define REG_MB_ADDR_H       0x0C84
#define REG_MB_DATA         0x0C86
#define REG_FSM_EN          0x0CB8

#define MSB124X_BOOT_START_ADDR     0x00000
#define MSB124X_BOOT_END_ADDR       0x00FFF
#define MSB124X_DVBT2_P1_START_ADDR 0x01000
#define MSB124X_DVBT2_P1_END_ADDR   0x08FFF
#define MSB124X_DVBT2_P2_START_ADDR 0x09000
#define MSB124X_DVBT2_P2_END_ADDR   0x0FFFF
#define MSB124X_DVBT_START_ADDR     0x10000
#define MSB124X_DVBT_END_ADDR       0x17FFF
#define MSB124X_DVBT2_ROM2_START_ADDR 0x20000
#define MSB124X_DVBT2_ROM2_END_ADDR   0x27FFF
#define MSB124X_DVBT2_ROM1_START_ADDR 0x28000
#define MSB124X_DVBT2_ROM1_END_ADDR   0x2FFFF

#define MSB124X_WINDOWS_BASE                0x100
#define MSB124X_BOOT_WINDOWS_OFFSET         MSB124X_WINDOWS_BASE
#define MSB124X_DVBT2_P2_WINDOWS_OFFSET    (MSB124X_WINDOWS_BASE + 0x08)
#define MSB124X_DVBT2_P1_WINDOWS_OFFSET    (MSB124X_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBT_WINDOWS_OFFSET        (MSB124X_DVBT2_P1_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBT2_ROM1_WINDOWS_OFFSET        (MSB124X_DVBT_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBT2_ROM2_WINDOWS_OFFSET        (MSB124X_DVBT2_ROM1_WINDOWS_OFFSET + 0x08)

#define MSB124X_MAX_FLASH_ON_RETRY_NUM 3
#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define MSB124X_MAX_IIC_PORT_CNT    4 // I2C0~3

//DVBS
#define MSB124X_DEMOD_WAIT_TIMEOUT    (6000)
#define MSB124X_TUNER_WAIT_TIMEOUT    (50)

#define MSB124X_TS_INV                  TS_CLK_INV
#define MSB124X_TS_DATA_SWAP            (0)
#define MSB124X_ADCPLL_IQ_SWAP       1 // //1 is ADCPLL IQ swap enable, for customer issue.

#define MSB124X_NOT_SUPPORT_DVBT2   0x01
#define MSB124X_SUPPORT_DVBT         0x08
#define MSB124X_SUPPORT_DVBS         0x10
#define MSB124X_SUPPORT_DVBS2        0x20

#if MSB124X_TS_DATA_SWAP //for DVBS
static MS_BOOL    _bTSDataSwap=FALSE;
#endif
static float g_MSB124X_fSNR = 0.0;
static MS_U8 u8MSB124X_DevCnt = 0;

static const S_DVBT2_SQI_CN_NORDIGP1 g_msb124x_dvbt2_sqi_cn_nordigp1[] =
{
    {_T2_QPSK, _T2_CR1Y2, 3.5},
    {_T2_QPSK, _T2_CR3Y5, 4.7},
    {_T2_QPSK, _T2_CR2Y3, 5.6},
    {_T2_QPSK, _T2_CR3Y4, 6.6},
    {_T2_QPSK, _T2_CR4Y5, 7.2},
    {_T2_QPSK, _T2_CR5Y6, 7.7},

    {_T2_16QAM, _T2_CR1Y2, 8.7},
    {_T2_16QAM, _T2_CR3Y5, 10.1},
    {_T2_16QAM, _T2_CR2Y3, 11.4},
    {_T2_16QAM, _T2_CR3Y4, 12.5},
    {_T2_16QAM, _T2_CR4Y5, 13.3},
    {_T2_16QAM, _T2_CR5Y6, 13.8},

    {_T2_64QAM, _T2_CR1Y2, 13.0},
    {_T2_64QAM, _T2_CR3Y5, 14.8},
    {_T2_64QAM, _T2_CR2Y3, 16.2},
    {_T2_64QAM, _T2_CR3Y4, 17.7},
    {_T2_64QAM, _T2_CR4Y5, 18.7},
    {_T2_64QAM, _T2_CR5Y6, 19.4},

    {_T2_256QAM, _T2_CR1Y2, 17.0},
    {_T2_256QAM, _T2_CR3Y5, 19.4},
    {_T2_256QAM, _T2_CR2Y3, 20.8},
    {_T2_256QAM, _T2_CR3Y4, 22.9},
    {_T2_256QAM, _T2_CR4Y5, 24.3},
    {_T2_256QAM, _T2_CR5Y6, 25.1},
    {_T2_QAM_UNKNOWN, _T2_CR_UNKNOWN, 0.0}
};

const float MSB124X_fDVBT2_SSI_Pref[][6] =
{
    { -95.7, -94.4, -93.6, -92.6, -92.0, -91.5},
    { -90.8, -89.1, -87.9, -86.7, -85.8, -85.2},
    { -86.9, -84.6, -83.2, -81.4, -80.3, -79.7},
    { -83.5, -80.4, -78.6, -76.0, -74.4, -73.3},
};

#if(TIMING_VERIFICATION == 1)
static MS_U32 tmm_1 = 0x00;
static MS_U32 tmm_2 = 0x00;
static MS_U32 tmm_3 = 0x00;
static MS_U32 tmm_4 = 0x00;
static MS_U32 tmm_5 = 0x00;
static MS_U32 tmm_6 = 0x00;
static MS_U32 tmm_7 = 0x00;
static MS_U32 tmm_8 = 0x00;
static MS_U32 tmm_9 = 0x00;
static MS_U32 tmm_10 = 0x00;
static MS_U32 tmm_11 = 0x00;
static MS_U32 tmm_12 = 0x00;
static MS_U32 tmm_13 = 0x00;
static MS_U32 tmm_14 = 0x00;
static MS_U32 tmm_15 = 0x00;
static MS_U32 tmm_16 = 0x00;
static MS_U32 tmm_17 = 0x00;
static MS_U32 tmm_18 = 0x00;
static MS_U32 tmm_19 = 0x00;
static MS_U32 tmm_20 = 0x00;

#define GIVE_ME_TIME MsOS_GetSystemTime();
#endif

//---------------------------------- for DVB-T
//operation

//configure
#define FS_L                    0x80
#define FS_H                    0x70
#define BW                      0x03
#define DVBT_IF_INV_PWM_OUT_EN  0x00

//#if((FRONTEND_TUNER_TYPE == TUNER_R836))
//#undef FRONTEND_DEMOD_IQ_SWAP
//#define FRONTEND_DEMOD_IQ_SWAP   1
//#endif

#define IQ_SWAP                  FRONTEND_DEMOD_IQ_SWAP
#if TS_PARALLEL_OUTPUT
#define SERIAL_TS               0x00
#else
#define SERIAL_TS               0x01
#endif


#define TS_CLK_SEL           0x00
#define TS_DATA_SWAP       0x00
#define TS_OUT_INV             TS_CLK_INV

// FC: FC = FS = 5000 = 0x1388     (5.00MHz IF)
//#if(FRONTEND_TUNER_TYPE == TUNER_R820T)
//#define FC_L           0xDA
//#define FC_H           0x11  //7M AND 8M     IF=4.57MHZ

//#define FC_L_6M        0xF2
//#define FC_H_6M        0x0D    //6M     IF= 3.57MHZ
//#else
#define FC_L           0x88    //5M
#define FC_H           0x13
//#endif


//configure
//------------------------------------------------ for DVB-T2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL               0x04
#if TS_PARALLEL_OUTPUT
#define T2_TS_SERIAL_VAL       0x00
#else
#define T2_TS_SERIAL_VAL       0x01
#endif
#define T2_TS_OUT_INV_VAL      0x01
#define T2_TS_DATA_SWAP_VAL    0x00
#define T2_TS_CLK_RATE_VAL      0x03
#define T2_TS_ERR_POL_VAL      0x00
#define T2_IF_AGC_INV_PWM_EN_VAL    0x00

// FC: FC = FS = 5000 = 0x1388     (5.00MHz IF)
//#if(FRONTEND_TUNER_TYPE == TUNER_R820T)
//#define T2_FC_L_VAL            0xDA
//#define T2_FC_H_VAL            0x11  //7M and 8M  IF=4.57MHZ

//#define T2_FC_L_VAL_6M          0xF2
//#define T2_FC_H_VAL_6M          0x0D    //6M  IF=3.57MHZ
//#else
#define T2_FC_L_VAL            0x88    //5M
#define T2_FC_H_VAL            0x13
//#endif

#define CUSTOMER_TABLE  1
#if (CUSTOMER_TABLE == 0)
static MS_U8 *MSB124x_DVBT2_DSPREG_TABLE = NULL;
static MS_U8 *MSB124x_DVBT_DSPREG_TABLE = NULL;
#else
static MS_U8 MSB124x_DVBT_DSPREG_TABLE[] =
{
    BW,  FC_L,  FC_H,  SERIAL_TS,  TS_CLK_SEL,  TS_OUT_INV,  TS_DATA_SWAP,
    IQ_SWAP,  DVBT_IF_INV_PWM_OUT_EN
};
static MS_U8 MSB124x_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL, T2_FC_L_VAL, T2_FC_H_VAL,  T2_TS_SERIAL_VAL, T2_TS_CLK_RATE_VAL,
    T2_TS_OUT_INV_VAL, T2_TS_DATA_SWAP_VAL, T2_TS_ERR_POL_VAL
};
#endif
static MS_BOOL* pDemodRest = NULL;

typedef struct
{
    MS_BOOL     bInited;
    MS_BOOL    bOpen;
    EN_DEVICE_DEMOD_TYPE  enDemodType;
    MS_S32     s32_MSB124X_Mutex;
    MS_U8       PlpIDList[256];
    MS_U8       u8sramCode;
    MS_U8     *pMSB124X_RegDVBT;
    MS_U32     u32CurrFreq;
    MS_U8       u8CurrPlpID;
    MS_U8       u8CurrScanType;
    DEMOD_EN_TER_BW_MODE     enCurrBW;
    DEMOD_EN_TER_BW_MODE     enTuneBw;
    MS_U8       PlpIDSize;
    MS_U32     u32ChkScanTimeStart;
    MS_BOOL   bFECLock;
    MS_U8     u8ScanStatus;
    DEMOD_MS_INIT_PARAM          MSB124X_InitParam;
    MS_U8     u8SlaveID;
    MS_U8     u8BondOpt;
    MS_BOOL   bIsDVBS2;
    MS_BOOL   bWO_SDRAM;
    MS_S32    s32DemodHandle;
    MS_U32    u32DmxInputPath;
    MS_BOOL   bIsMCP_DMD;
#if MSB124X_NO_CHANNEL_CHECK
    MS_BOOL noChannelStable;
    MS_BOOL t2NoChannelFlag;
    MS_BOOL tNoChannelFlag;
    MS_U32    u32LockTime;
#endif
} MDvr_CofdmDmd_CONFIG;


MDvr_CofdmDmd_CONFIG* pstMSB124X = NULL;
SLAVE_ID_TBL* pstMSB124X_slave_ID_TBL = NULL;

static SLAVE_ID_USAGE MSB124X_possible_slave_ID[3] =
{
    {0xF2, FALSE},
    {0xD2, FALSE},
    {0xFF, FALSE}
};

static MDvr_CofdmDmd_CONFIG MSB124X_Init =
{
    FALSE,
    FALSE,
    E_DEVICE_DEMOD_NULL,
    -1,
    {0},
    0,
    NULL,
    0,
    0,
    0,
    DEMOD_BW_MODE_8MHZ,
    DEMOD_BW_MODE_8MHZ,
    0,
    0,
    FALSE,
    0,
    {NULL},
    0xF2,
    0,
    FALSE,
    FALSE,
    -1,
    0xFF,
    FALSE,
#if MSB124X_NO_CHANNEL_CHECK
    FALSE,
    FALSE,
    FALSE,
    0
#endif
};

const S_RFAGC_SSI MSB124X_RFAGC_SSI[] =
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

const S_IFAGC_SSI MSB124X_IFAGC_SSI[] =
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

const S_SSI_PREF MSB124X_SsiPref[] =
{
    {_QPSK_1,  _CR1Y2_1, -93.0},
    {_QPSK_1,  _CR2Y3_1, -91.0},
    {_QPSK_1,  _CR3Y4_1, -90.0},
    {_QPSK_1,  _CR5Y6_1, -89.0},
    {_QPSK_1,  _CR7Y8_1, -88.0},

    {_16QAM_1, _CR1Y2_1, -87.0},
    {_16QAM_1, _CR2Y3_1, -85.0},
    {_16QAM_1, _CR3Y4_1, -84.0},
    {_16QAM_1, _CR5Y6_1, -83.0},
    {_16QAM_1, _CR7Y8_1, -82.0},

    {_64QAM_1, _CR1Y2_1, -82.0},
    {_64QAM_1, _CR2Y3_1, -80.0},
    {_64QAM_1, _CR3Y4_1, -78.0},
    {_64QAM_1, _CR5Y6_1, -77.0},
    {_64QAM_1, _CR7Y8_1, -76.0},
};

#define SGLVEL_COUNT 172
static const MS_U16                       _u16SignalLevel[SGLVEL_COUNT][2]=
{
    /*
    {8285,    922},{10224,    902},{12538,    882},{14890,    862},{17343,    842},{19767,    822},{22178,    802},{24618,    782},{27006,    762},{29106,    742},
    {30853,    722},{32289,    702},{33577,    682},{34625,    662},{35632,    642},{36552,    622},{37467,    602},{38520,    582},{39643,    562},{40972,    542},
    {42351,    522},{43659,    502},{44812,    482},{45811,    462},{46703,    442},{47501,    422},{48331,    402},{49116,    382},{49894,    362},{50684,    342},
    {51543,    322},{52442,    302},{53407,    282},{54314,    262},{55208,    242},{56000,    222},{56789,    202},{57544,    182},{58253,    162},{58959,    142},
    {59657,    122},{60404,    102},{61181,    82},{62008,    62},{63032,    42},{65483,    22},{65535,    12}*/
    {27463,    920},{27460,    915},{27474,    910},{27800,    905},{27815,    900},{28013,    895},{28116,    890},{28326,    885},{28368,    880},{28570,    875},
    {28754,    870},{28773,    865},{29017,    860},{29453,    855},{29929,    850},{30266,    845},{30776,    840},{31120,    835},{31566,    830},{31805,    825},
    {32175,    820},{32363,    815},{32466,    805},{32617,    800},{32729,    795},{33013,    790},{33137,    785},{33320,    780},{33518,    775},
    {33751,    770},{33925,    765},{34055,    760},{34156,    755},{34263,    750},{34373,    745},{34547,    740},{34672,    735},{34813,    730},{34946,    725},
    {35049,    720},{35230,    715},{35329,    710},{35450,    705},{35637,    700},{35756,    695},{35866,    690},{35962,    685},{36165,    680},{36345,    675},
    {36443,    670},{36608,    665},{36718,    660},{36886,    655},{37004,    650},{37215,    645},{37425,    615},{37527,    610},{37659,    605},{37743,    600},
    {37912,    595},{37976,    590},{38138,    585},{38244,    580},{38438,    575},{38527,    570},{38739,    540},{38843,    535},{39000,    530},{39124,    525},
    {39205,    520},{39375,    515},{39468,    510},{39567,    505},{39731,    500},{39833,    495},{39834,    490},{40114,    485},{40215,    480},{40326,    475},
    {40428,    470},{40509,    465},{40579,    460},{40683,    455},{40784,    450},{40874,    445},{41032,    440},{41131,    435},{41314,    430},{41421,    425},
    {41578,    420},{41628,    415},{41724,    395},{41783,    390},{41862,    385},{42013,    380},{42041,    375},{42123,    370},{42237,    365},{42308,    360},
    {42405,    355},{42501,    350},{42581,    345},{42665,    340},{42737,    335},{42908,    330},{43031,    325},{43160,    320},{43317,    315},{43423,    310},
    {43587,    305},{43692,    300},{43871,    295},{44021,    290},{44206,    285},{44305,    280},{44459,    275},{44591,    270},{44700,    265},{44844,    260},
    {44935,    255},{45042,    250},{45133,    245},{45278,    240},{45377,    235},{45532,    230},{45626,    225},{45722,    220},{45866,    215},{46016,    210},
    {46115,    205},{46242,    200},{46318,    195},{46445,    190},{46572,    185},{46640,    180},{46759,    175},{46871,    170},{46970,    165},{47093,    160},
    {47156,    155},{47290,    150},{47411,    145},{47477,    140},{47592,    135},{47664,    130},{47808,    125},{47860,    120},{47943,    115},{48022,    110},
    {48057,    105},{48117,    100},{48188,     95},{48206,     90} ,{48261,     85} ,{48319,     80} ,{48366,     75},{48393,     70},{48443,    65 }  ,{48453,     60},
    {48501,     55},{48549,     50} ,{48615,     45},{48635,     40} ,{48693,     35} ,{48765,     30} ,{48789,     25},{48822,     20},{48837,    15 } ,{48898,     10},
    {48925,      5},{48986,      0},
};


//--------------------------------------------------------------------- for DVB-C
#define     C_AUTO_SCAN_SYM_RATE    1
#define     C_AUTO_SCAN_QAM          1
#define     C_IF_INV_PWM_OUT_EN     0
#define     C_ZIF                   0
#define     C_FC_L                  0x88
#define     C_FC_H                  0x13
#define     C_FS_L                  0xC0
#define     C_FS_H                  0x5D
#define     C_BW_L                  0xDB
#define     C_BW_H                  0x1A
#define     C_BW1_L                 0xF4
#define     C_BW1_H                 0x1A
#define     C_BW2_L                 0xDB
#define     C_BW2_H                 0x1A
#define     C_BW3_L                 0x58
#define     C_BW3_H                 0x1B
#define     C_QAM                   2
#define     C_CCI                   0
#if TS_PARALLEL_OUTPUT
#define     C_TS_SERIAL             0
#else
#define     C_TS_SERIAL             1

#endif
#define     C_TS_CLK_RATE           6
#define     C_TS_OUT_INV            TS_CLK_INV  //TS clock inverse
#define     C_TS_DATA_SWAP          0
#define     C_IQ_SWAP               0

static MS_U8 MSB124X_DVBC_DSPREG_TABLE[] =
{
    C_AUTO_SCAN_SYM_RATE,  //0x20
    C_AUTO_SCAN_QAM,
    C_IF_INV_PWM_OUT_EN,
    C_ZIF,
    C_FC_L,
    C_FC_H,
    C_FS_L,
    C_FS_H,

    C_BW_L,             //0x28
    C_BW_H,
    C_BW1_L,
    C_BW1_H,
    C_BW2_L,
    C_BW2_H,
    C_BW3_L,
    C_BW3_H,

    C_QAM,               //0x30
    C_CCI,
    C_TS_SERIAL,
    C_TS_CLK_RATE,
    C_TS_OUT_INV,
    C_TS_DATA_SWAP,
    C_IQ_SWAP,
};

// DVBC SSI define  ========================================================================
const DVBC_RFAGC_SSI MSB124X_DVBC_RFAGC_SSI[] =
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

const DVBC_IFAGC_SSI MSB124X_DVBC_IFAGC_SSI[] =
{
    { -31,   0x86},
    { -32,   0x87},
    { -33,   0x87},
    { -34,   0x88},
    { -35,   0x88},
    { -36,   0x89},
    { -37,   0x8a},
    { -38,   0x8a},
    { -39,   0x8b},
    { -40,   0x8b},
    { -41,   0x8c},
    { -42,   0x8c},
    { -43,   0x8d},
    { -44,   0x8d},
    { -45,   0x8e},
    { -46,   0x8f},
    { -47,   0x8f},
    { -48,   0x90},
    { -49,   0x90},
    { -50,   0x91},
    { -51,   0x91},
    { -52,   0x92},
    { -53,   0x93},
    { -54,   0x93},
    { -55,   0x94},
    { -56,   0x94},
    { -57,   0x95},
    { -58,   0x95},
    { -59,   0x96},
    { -60,   0x97},
    { -61,   0x97},
    { -62,   0x98},
    { -63,   0x98},
    { -64,   0x99},
    { -65,   0x99},
    { -66,   0x9a},
    { -67,   0x9a},
    { -68,   0x9b},
    { -69,   0x9c},
    { -70,   0x9c},
    { -71,   0x9d},
    { -72,   0x9d},
    { -73,   0x9e},
    { -74,   0x9e},
    { -75,   0x9f},
    { -76,   0x9f},
    { -77,   0xa0},
    { -99,   0xFF}
};

const DVBC_SSI_PREF MSB124X_DVBC_SSI_Pow_ref[] =
{

    {DVBC_16QAM, -82.0},
    {DVBC_32QAM, -80.0},
    {DVBC_64QAM, -78.0},
    {DVBC_128QAM, -77.0},
    {DVBC_256QAM, -76.0},

};

static float intern_dvb_c_qam_ref[] = {3.0, 0.0, 0.0, 0.0, 0.0, 80.0}; //16q,32q,64q,128q,256q, and others

// DVBC SSI define  ========================================================================

#if(TIMING_VERIFICATION == 1)
static void show_timer(void)
{
    DMD_DBG(("***************************\n"));
    DMD_DBG(("[tmm1]t2-t1 = %ld (%ld - %ld)\n", tmm_2 - tmm_1, tmm_2, tmm_1));
    DMD_DBG(("[tmm2]t4-t3 = %ld (%ld - %ld)\n", tmm_4 - tmm_3, tmm_4, tmm_3));
    DMD_DBG(("[tmm3]t6-t5 = %ld (%ld - %ld)\n", tmm_6 - tmm_5, tmm_6, tmm_5));
    DMD_DBG(("[tmm4]t8-t7 = %ld (%ld - %ld)\n", tmm_8 - tmm_7, tmm_8, tmm_7));
    DMD_DBG(("[tmm5]t10-t9 = %ld (%ld - %ld)\n", tmm_10 - tmm_9, tmm_10, tmm_9));
    DMD_DBG(("[tmm6]t12-t11 = %ld (%ld - %ld)\n", tmm_12 - tmm_11, tmm_12, tmm_11));
    DMD_DBG(("[tmm7]t14-t13 = %ld (%ld - %ld)\n", tmm_14 - tmm_13, tmm_14, tmm_13));
    DMD_DBG(("[tmm8]t16-t15 = %ld (%ld - %ld)\n", tmm_16 - tmm_15, tmm_16, tmm_15));
    DMD_DBG(("[tmm9]t18-t17 = %ld (%ld - %ld)\n", tmm_18 - tmm_17, tmm_18, tmm_17));
    DMD_DBG(("[tmm10]t20-t19 = %ld (%ld - %ld)\n", tmm_20 - tmm_19, tmm_20, tmm_19));
    DMD_DBG(("[tmm11]lock_time# = %ld (%ld - %ld)\n", tmm_14 - u32StartTime, tmm_14, u32StartTime));
    DMD_DBG(("[tmm12]lock-setf = %ld (%ld - %ld)\n", tmm_14 - tmm_11, tmm_14, tmm_11));
    DMD_DBG(("[tmm13]lock-loaddsp = %ld (%ld - %ld)\n", tmm_14 - tmm_9, tmm_14, tmm_9));
    DMD_DBG(("***************************\n"));
}
#endif


static MS_BOOL _MSB124X_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data);
static MS_BOOL _MSB124X_GetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data);

#if MS_DVBS_INUSE
MS_BOOL MSB124X_DiSEqC_Init(MS_U8 u8DemodIndex);
#endif

static MS_BOOL MSB124X_Variables_alloc(void)
{
    MS_U8 i;
    MDvr_CofdmDmd_CONFIG *pMSB124X = NULL;
    SLAVE_ID_TBL* pSlaveIDTBL = NULL;

    if(NULL == pstMSB124X)
    {
        pstMSB124X = (MDvr_CofdmDmd_CONFIG *)malloc(sizeof(MDvr_CofdmDmd_CONFIG) * MAX_FRONTEND_NUM);
        if(NULL == pstMSB124X)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< MAX_FRONTEND_NUM; i++)
            {
                pMSB124X = (pstMSB124X + i);
                memcpy(pMSB124X, &MSB124X_Init, sizeof(MDvr_CofdmDmd_CONFIG));
            }
        }
    }

    if(NULL == pDemodRest)
    {
        pDemodRest = (MS_BOOL*)malloc(sizeof(MS_BOOL) * MAX_FRONTEND_NUM);
        if(NULL == pDemodRest)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< MAX_FRONTEND_NUM; i++)
                *(pDemodRest + i) = TRUE;
        }
    }


    if(NULL == pstMSB124X_slave_ID_TBL)
    {
        pstMSB124X_slave_ID_TBL = (SLAVE_ID_TBL *)malloc(sizeof(SLAVE_ID_TBL) * MSB124X_MAX_IIC_PORT_CNT);
        if(NULL == pstMSB124X_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< MSB124X_MAX_IIC_PORT_CNT ; i++)
            {
                pSlaveIDTBL = (pstMSB124X_slave_ID_TBL + i);
                memcpy(&(pSlaveIDTBL->stID_Tbl), &MSB124X_possible_slave_ID, sizeof(MSB124X_possible_slave_ID));
            }
        }
    }

    return TRUE;

}

static MS_BOOL MSB124X_Variables_free(void)
{
    if(NULL != pstMSB124X)
    {
        free(pstMSB124X);
        pstMSB124X = NULL;
    }

    if(NULL != pDemodRest)
    {
        free(pDemodRest);
        pDemodRest = NULL;
    }

    if(NULL != pstMSB124X_slave_ID_TBL)
    {
        free(pstMSB124X_slave_ID_TBL);
        pstMSB124X_slave_ID_TBL = NULL;
    }
    return TRUE;
}

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
static MS_BOOL _GetDemodIndexByHandle(MS_S32 s32Handle, MS_U8* pu8DemodIndex)
{
    MS_U8 u8DemodIndex;
    MDvr_CofdmDmd_CONFIG *pMSB124X;
    for(u8DemodIndex=0; u8DemodIndex< MAX_FRONTEND_NUM; u8DemodIndex++)
    {
        pMSB124X = (pstMSB124X + u8DemodIndex);
        if(pMSB124X->s32DemodHandle == s32Handle)
        {
            *pu8DemodIndex = u8DemodIndex;
            return TRUE;
        }
    }
    return FALSE;
}
#endif
static void _msb124x_hw_reset(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
#ifdef GPIO_DEMOD_RST
    int rst_pin = 9999;
    if(pMSB124X->bIsMCP_DMD)
    {
        rst_pin = GPIO_DEMOD_RST;
        mdrv_gpio_set_high(rst_pin);
        MsOS_DelayTask(5);
        mdrv_gpio_set_low(rst_pin);
        MsOS_DelayTask(10);
        mdrv_gpio_set_high(rst_pin);
        MsOS_DelayTask(5);
    }
    rst_pin = 9999;
#endif

    pMSB124X->bInited = FALSE;
    pMSB124X->bOpen = FALSE;
    *(pDemodRest + u8DemodIndex) = TRUE;
    MDrv_DMD_MSB124X_Exit_EX(pMSB124X->s32DemodHandle);
}

#ifdef USE_SPI_LOAD_TO_SDRAM
static void msb124x_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void msb124x_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void msb124x_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {msb124x_SPIPAD_TS0_En,\
                                   msb124x_SPIPAD_TS1_En,\
                                   msb124x_SPIPAD_TS2_En};

static void msb124x_SPIPAD_TS0_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_PARALLEL_IN);
    } 

}

static void msb124x_SPIPAD_TS1_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_PARALLEL_IN);
    } 

}

static void msb124x_SPIPAD_TS2_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_PARALLEL_IN);
    } 
}
#endif

/*================================================
==                       IIC write/read interface
=================================================*/
//MS_U16 MSB124X_Demod_ReadReg(MS_U16 RegAddr);

MS_BOOL MSB124X_WriteBytes_demod(MS_U8 u8DemodIndex,MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    bRet = MDrv_IIC_Write(pMSB124X->u8SlaveID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error, slave ID = 0x%x\n", pMSB124X->u8SlaveID));
    }
    return bRet;
}

MS_BOOL MSB124X_ReadBytes_demod(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    bRet = MDrv_IIC_Read(pMSB124X->u8SlaveID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}

MS_BOOL MSB124X_WriteBytes_demod1(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    bRet = MDrv_IIC1_Write(pMSB124X->u8SlaveID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error\n"));
    }
    return bRet;
}

MS_BOOL MSB124X_ReadBytes_demod1(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }

    bRet = MDrv_IIC1_Read(pMSB124X->u8SlaveID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}


MS_BOOL MSB124X_WriteBytes_spi(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Write(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write spi error\n"));
    }
    return bRet;
}

MS_BOOL MSB124X_ReadBytes_spi(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Read(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read spi error\n"));
    }
    return bRet;
}

MS_BOOL MSB124X_WriteBytes_spi1(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC1_Write(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write spi error\n"));
    }
    return bRet;
}

MS_BOOL MSB124X_ReadBytes_spi1(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC1_Read(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read spi error\n"));
    }
    return bRet;
}


typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

static mapi_i2c DemodI2Chandler, SpiI2Chandler;
static mapi_i2c* mapi_i2c_GetI2C_Dev(MS_U32 u32gID)
{
    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        DMD_ERR(("iic device not supported\n"));
    case MSB124X_DEMOD_IIC:
        handler = &DemodI2Chandler;
        handler->WriteBytes = MSB124X_WriteBytes_demod;
        handler->ReadBytes = MSB124X_ReadBytes_demod;
        break;
    case MSB124X_SPI_IIC:
        handler = &SpiI2Chandler;
        handler->WriteBytes = MSB124X_WriteBytes_spi;
        handler->ReadBytes = MSB124X_ReadBytes_spi;
        break;
    }
    return handler;
}

static mapi_i2c* mapi_i2c1_GetI2C_Dev(MS_U32 u32gID)
{
    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        DMD_DBG(("iic device not supported\n"));
    case MSB124X_DEMOD_IIC:
        handler = &DemodI2Chandler;
        handler->WriteBytes = MSB124X_WriteBytes_demod1;
        handler->ReadBytes = MSB124X_ReadBytes_demod1;
        break;
    case MSB124X_SPI_IIC:
        handler = &SpiI2Chandler;
        handler->WriteBytes = MSB124X_WriteBytes_spi1;
        handler->ReadBytes = MSB124X_ReadBytes_spi1;
        break;
    }
    return handler;
}


static MS_BOOL msb124x_I2C_Access(eDMD_MSB124X_DemodI2CSlaveID eSlaveID, eDMD_MSB124X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
    MS_U8 u8DemodIndex = 0;

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
    if(!_GetDemodIndexByHandle(MDrv_DMD_MSB124X_GetCurrentHandle(), &u8DemodIndex))
    {
        DMD_ERR(("124X_I2C_Access get demod index FAIL\n"));
    }
#endif

    switch (eSlaveID)
    {
    case E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2:
        i2c_iptr = mapi_i2c_GetI2C_Dev(MSB124X_SPI_IIC);
        break;
    case E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1:
    default:
        i2c_iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
        break;
    }

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8DemodIndex,u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB124X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8DemodIndex, u8AddrSize, pu8Addr, u16Size, pu8Data);
        default:
            break;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

static MS_BOOL msb124x_I2C1_Access(eDMD_MSB124X_DemodI2CSlaveID eSlaveID, eDMD_MSB124X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c1_GetI2C_Dev(MSB124X_DEMOD_IIC);
    MS_U8 u8DemodIndex = 0;

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
    if(!_GetDemodIndexByHandle(MDrv_DMD_MSB124X_GetCurrentHandle(), &u8DemodIndex))
    {
        DMD_ERR(("124X_I2C1_Access get demod index FAIL\n"));
    }
#endif



    switch (eSlaveID)
    {
    case E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2:
        i2c_iptr = mapi_i2c1_GetI2C_Dev(MSB124X_SPI_IIC);
        break;
    case E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1:
    default:
        i2c_iptr = mapi_i2c1_GetI2C_Dev(MSB124X_DEMOD_IIC);
        break;
    }

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8DemodIndex, u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB124X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8DemodIndex, u8AddrSize, pu8Addr, u16Size, pu8Data);
        default:
            break;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}


/*================================================
==               demod  msb124x  write/read interface
=================================================*/
MS_BOOL MSB124X_WriteReg(MDvr_CofdmDmd_CONFIG *pMSB124X, MS_U16 u16Addr, MS_U8 u8Data)
{
    if(!pMSB124X) return FALSE;
    return MDrv_DMD_MSB124X_SetReg_EX(pMSB124X->s32DemodHandle, u16Addr, u8Data);
}

#if 0
MS_BOOL MSB124X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    return MDrv_DMD_MSB124X_SetRegs(u16Addr, u8pData, data_size);
}
#endif
MS_BOOL MSB124X_WriteReg2bytes(MDvr_CofdmDmd_CONFIG *pMSB124X ,MS_U16 u16Addr, MS_U16 u16Data)
{
    if(!pMSB124X) return FALSE;
    return MDrv_DMD_MSB124X_SetReg2Bytes_EX(pMSB124X->s32DemodHandle, u16Addr, u16Data);
}

MS_BOOL MSB124X_ReadReg(MDvr_CofdmDmd_CONFIG *pMSB124X ,MS_U16 u16Addr, MS_U8 *pu8Data)
{
    if(!pMSB124X) return FALSE;
    return MDrv_DMD_MSB124X_GetReg_EX(pMSB124X->s32DemodHandle, u16Addr, pu8Data);
}

MS_BOOL  MSB124X_WriteDspReg(MDvr_CofdmDmd_CONFIG *pMSB124X ,MS_U16 u16Addr, MS_U8 u8Data)
{
    if(!pMSB124X) return FALSE;
    return MDrv_DMD_MSB124X_SetDSPReg_EX(pMSB124X->s32DemodHandle, u16Addr, u8Data);
}

MS_BOOL MSB124X_ReadDspReg(MDvr_CofdmDmd_CONFIG *pMSB124X ,MS_U16 u16Addr, MS_U8* pData)
{
    if(!pMSB124X) return FALSE;
    return MDrv_DMD_MSB124X_GetDSPReg_EX(pMSB124X->s32DemodHandle, u16Addr, pData);
}


/*================================================
==               demod  MSB124X  base interface
=================================================*/
void MSB124X_Show_Version(MS_U8 u8DemodIndex)
{

    MS_U8  reg = 0;
    MS_U16 ver0 = 0;
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    MS_U8  ver1 = 0;
#endif
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    MSB124X_ReadDspReg(pMSB124X, E_T2_FW_VER_0, &reg);
    ver0 = reg;
    MSB124X_ReadDspReg(pMSB124X, E_T2_FW_VER_1, &reg);
    ver0 = (ver0 << 8) | reg;
    MSB124X_ReadDspReg(pMSB124X, E_T2_FW_VER_2, &reg);
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    ver1 = reg;
#endif
    DMD_DBG(("\n[MSB124X]ver0 = 0x%x, ver1 = 0x%x\n", ver0, ver1));

    return;
}

void MSB124X_Show_BondingOption(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return;
    }

    DMD_DBG(("=== MSB124X Bonding Option ===\n"));
    if(MSB124X_NOT_SUPPORT_DVBT2 & pMSB124X->u8BondOpt)
    {
        DMD_DBG(("DVB-T2: disable\n"));
    }
    else
    {
        DMD_DBG(("DVB-T2: enable\n"));
    }

    if(MSB124X_SUPPORT_DVBT & pMSB124X->u8BondOpt)
    {
        DMD_DBG(("DVB-T: enable\n"));
    }
    else
    {
        DMD_DBG(("DVB-T: disable\n"));
    }

    if(MSB124X_SUPPORT_DVBS & pMSB124X->u8BondOpt)
    {
        DMD_DBG(("DVB-S: enable\n"));
    }
    else
    {
        DMD_DBG(("DVB-S: disable\n"));
    }

    if(MSB124X_SUPPORT_DVBS2 & pMSB124X->u8BondOpt)
    {
        DMD_DBG(("DVB-S2: enable\n"));
    }
    else
    {
        DMD_DBG(("DVB-S2: disable\n"));
    }


}


MS_BOOL MSB124X_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    HWI2C_PORT hwi2c_port;
    mapi_i2c *iptr;

    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        iptr = mapi_i2c1_GetI2C_Dev(MSB124X_DEMOD_IIC);
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSB124X][beg]MSB124X_I2C_CH_Reset, CH=0x%x\n", ch_num));

    if (iptr == NULL)
    {
        return FALSE;
    }

    if(pDemodRest == NULL)
        return FALSE;

    if (*(pDemodRest + u8DemodIndex))
    {
        *(pDemodRest + u8DemodIndex) = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        iptr->WriteBytes(u8DemodIndex, 0, NULL, 5, data);
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    /*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
    */
    DMD_DBG(("[MSB124X][end]MSB124X_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}


void MSB124X_Reset(MS_U8 u8DemodIndex)
{
    DMD_ENTERAPISTR(0);
    MS_U8     u8Data = 0x00;
    MS_U32    u32Retry = 0x00;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return ;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return ;
    }


    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        DMD_DBG(("\nreset T2\n"));
        // mask miu access for all and mcu
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        MSB124X_Delayus(10);

        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x10) * 2, 0x01);
        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x19) * 2, 0x03);
        MSB124X_WriteReg(pMSB124X, REG_MB_CNTL, 0x00);             //clear MB_CNTL
        MSB124X_Delayus(5 * 100);

        // enable miu access of mcu gdma
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        MSB124X_Delayus(10);

        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x19) * 2, 0x00);
        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x10) * 2, 0x00);
        MSB124X_Delayus(5 * 1000);
        MSB124X_ReadReg(pMSB124X, REG_MB_CNTL, &u8Data);           //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            MSB124X_Delayus(5 * 1000);
            MSB124X_ReadReg(pMSB124X, REG_MB_CNTL, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                DMD_ERR((">>>MSB124X: MSB124X_Reset Fail!\n"));
                break;
            }
        }
    }
    else if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        DMD_DBG(("\nreset T\n"));
        // mask miu access for all and mcu
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        MSB124X_Delayus(10);

        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x19) * 2, 0x01);
        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x10) * 2, 0x01);
        MSB124X_WriteReg(pMSB124X, REG_MB_CNTL, 0x00);             //clear MB_CNTL
        MSB124X_Delayus(5 * 1000);

        // enable miu access of mcu gdma
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        MSB124X_Delayus(10);

        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x10) * 2, 0x00);
        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x19) * 2, 0x00);

        MSB124X_ReadReg(pMSB124X, REG_MB_CNTL, &u8Data);       //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            MSB124X_Delayus(5 * 1000);
            MSB124X_ReadReg(pMSB124X, REG_MB_CNTL, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                DMD_ERR((">>>MSB124X: MSB124X_Reset Fail!\n"));
                break;
            }
        }

    }
    else if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_C)
    {
        DMD_DBG(("\nreset C\n"));
        // mask miu access for all and mcu
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        MSB124X_Delayus(10);

        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x19) * 2, 0x01);
        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x10) * 2, 0x01);
        MSB124X_WriteReg(pMSB124X, REG_MB_CNTL, 0x00);             //clear MB_CNTL
        MSB124X_Delayus(5 * 1000);

        // enable miu access of mcu gdma
        MSB124X_WriteReg(pMSB124X, 0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        MSB124X_Delayus(10);

        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x10) * 2, 0x00);
        MSB124X_WriteReg(pMSB124X, 0x0B00 + (0x19) * 2, 0x00);

        MSB124X_ReadReg(pMSB124X, REG_MB_CNTL, &u8Data);       //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            MSB124X_Delayus(5 * 1000);
            MSB124X_ReadReg(pMSB124X, REG_MB_CNTL, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                DMD_ERR((">>>MSB124X: MSB124X_Reset Fail!\n"));
                break;
            }
        }
    }

    pMSB124X->u32ChkScanTimeStart = MsOS_GetSystemTime();
}


static void MSB124X_Driving_Control(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8 u8Temp;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return ;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return ;
    }

    //if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    //{
    //    DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
    //    return ;
    //}

    MSB124X_ReadReg(pMSB124X, 0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    MSB124X_WriteReg(pMSB124X, 0x0958, u8Temp);

    MSB124X_ReadReg(pMSB124X, 0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp & (~0x0F);
    }
    MSB124X_WriteReg(pMSB124X, 0x0959, u8Temp);
    //MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
}

static MS_BOOL MSB124X_DTV_Serial_Control(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    UNUSED(bEnable);
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        //FRA_T2_BRINGUP
    }
    MSB124X_Driving_Control(u8DemodIndex,TRUE);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}

static EN_CAB_CONSTEL_TYPE MSB124X_DVB_C_GetSignalModulation(MS_U8 u8DemodIndex)
{
    MS_U8  u8_reg = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    // 0:16q,1:32q,2:64q,3:128q,4:256q
    if(MSB124X_ReadReg(pMSB124X,0x2302, &u8_reg))
        u8_reg = u8_reg&0x07;
    else
    {
        MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
        return CAB_QAMAUTO;
    }

    switch (u8_reg)
    {
    case 0:
        return CAB_QAM16;
    case 1:
        return CAB_QAM32;
    case 2:
        return CAB_QAM64;
    case 3:
        return CAB_QAM128;
    case 4:
        return CAB_QAM256;
    default:
        return CAB_QAMAUTO;
    }


}
/*================================================
==               demod  MSB124X iic bypass interface
=================================================*/
MS_BOOL MSB124X_IIC_Bypass_Mode(MS_U8 u8DemodIndex, MS_BOOL enable)
{
    MS_U8 u8Retry = 10;
    MS_BOOL bResult;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    DMD_DBG((">>>MSB124X: IIC_bypass() set %x\n", enable));

    while (u8Retry--)
    {
        if (enable)
            bResult = MSB124X_WriteReg(pMSB124X,0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            bResult = MSB124X_WriteReg(pMSB124X, 0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off

        if (bResult)
            break;
    }
    return bResult;
}

/*================================================
==               demod  MSB124X lock and ssi sqi interface
=================================================*/
MS_BOOL MSB124X_Lock(MS_U8 u8DemodIndex, EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
{
    MS_U16        u16Address = 0;
    MS_U8         cData = 0;
    MS_U8         cBitMask = 0;
    MS_U8         use_dsp_reg = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    switch (eStatus)
    {
    case COFDM_P1_LOCK:
        u16Address =  0x2482; //T2 P1 lock,
        cBitMask = BIT3;
        break;

    case COFDM_P1_LOCK_HISTORY:
        use_dsp_reg = 1;
        u16Address = E_T2_DVBT2_LOCK_HIS; //T2 P1 ever lock,
        cBitMask = BIT5;
        break;

    case COFDM_L1_CRC_LOCK:
        u16Address =  0x2741; //T2 L1 CRC check,
        cBitMask = BIT5 | BIT6 | BIT7;
        break;

    case COFDM_DVBT2_NOCH_FLAG:
        use_dsp_reg = 1;
        u16Address = T_DVBT2_NOCHAN_Flag; //Pl ever lock,
        cBitMask = BIT0;
        break;

    case COFDM_DVBT_NOCH_FLAG:
        use_dsp_reg = 1;
        u16Address = T_DVBT_NOCHAN_Flag; // No DVBT CH Flag,
        cBitMask = BIT0;
        break;

    case COFDM_FEC_LOCK:
        switch (system)
        {
        case E_DEVICE_DEMOD_DVB_T2:
            use_dsp_reg = 1;
            u16Address =  E_T2_DVBT2_LOCK_HIS; //FEC lock,
            cBitMask = BIT7;
            break;
        case E_DEVICE_DEMOD_DVB_T:
            MSB124X_ReadReg(pMSB124X, 0x11E0, &cData);//DVBT state
            if (cData == 0x0B)
                return TRUE;
            else
                return FALSE;
            break;
        default:
            return FALSE;
        }
        break;

    case COFDM_TPS_LOCK:
        switch (system)
        {
        case E_DEVICE_DEMOD_DVB_T:
            use_dsp_reg = 0;
            u16Address =  0x0f22; //TPS Lock,ok
            cBitMask = BIT1;
            break;
        case E_DEVICE_DEMOD_DVB_T2:
        default:
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    if (!use_dsp_reg)
    {
        if (MSB124X_ReadReg(pMSB124X, u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (MSB124X_ReadDspReg(pMSB124X, u16Address, &cData) == FALSE)
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

MS_BOOL dvbs2_MSB124X_TS_Enable(MS_U8 u8DemodIndex,MS_BOOL bTsEnable)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16Address;
    MS_U8   u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (bTsEnable == TRUE)
    {
        bRet&= MSB124X_WriteReg(pMSB124X, 0x095A, 0xFF);
    }
    else
    {
        bRet&= MSB124X_WriteReg(pMSB124X, 0x095A, 0x00);
    }

    u16Address=0x0924;
    bRet &= MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
#if MSB124X_TS_INV
    u8Data |= (0x20);
#else
    u8Data &= ~(0x20);
#endif
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    return bRet;
}

MS_BOOL MSB124X_Lock_S(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u16Address=0xB55;
        bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        if((u8Data>=15) && (u8Data!=0xcd))
        {
            if (u8Data==15)
            {
                pMSB124X->bIsDVBS2=FALSE;   //S
                bRet = TRUE;
            }
            else if(u8Data==16)
            {
                pMSB124X->bIsDVBS2=TRUE;    //S2
                bRet = TRUE;
            }
            else
            {
                bRet = FALSE;
            }
        }
        else
        {
            bRet = FALSE;
        }
#if MSB124X_TS_DATA_SWAP
        if (bRet==FALSE)
        {
            _bTSDataSwap=FALSE;
        }
        else
        {
            if (_bTSDataSwap==FALSE)
            {
                _bTSDataSwap= TRUE;
                u16Address=0x2A80;
                bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
                u8Data^=0x20;
                bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
            }
        }
#endif
    }
    else
    {
        bRet = TRUE;
    }
    dvbs2_MSB124X_TS_Enable(u8DemodIndex, 1);
    return bRet;
}



MS_BOOL MSB124X_Get_No_Channel_Flag(MS_U8 u8DemodIndex, EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
{
    MS_U16        u16Address = 0;
    MS_U8         cData = 0;
    MS_U8         cBitMask = 0;
    MS_U8         use_dsp_reg = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (eStatus)
    {
    case COFDM_P1_LOCK:
        u16Address =  0x2482; //Pl lock,
        cBitMask = BIT3;
        break;

    case COFDM_P1_LOCK_HISTORY:
        use_dsp_reg = 1;
        u16Address = E_T2_DVBT2_LOCK_HIS; //Pl ever lock,
        cBitMask = BIT5;
        break;

    case COFDM_L1_CRC_LOCK:
        u16Address =  0x2741; //L1 CRC check,
        cBitMask = BIT5 | BIT6 | BIT7;
        break;
#if MSB124X_NO_CHANNEL_CHECK
    case COFDM_DVBT2_NOCH_FLAG:
        use_dsp_reg = 1;
        u16Address = T_DVBT2_NOCHAN_Flag; //Pl ever lock,
        cBitMask = BIT0;
        break;

    case COFDM_DVBT_NOCH_FLAG:
        use_dsp_reg = 1;
        u16Address = T_DVBT_NOCHAN_Flag; // No DVBT CH Flag,
        cBitMask = BIT0;
        break;

    case COFDM_DETECT_DONE_FLAG:
        use_dsp_reg = 1;
        u16Address = T_DETECT_DONE_Flag; // No DVBT CH Flag,
        cBitMask = BIT0;
        break;
#endif
    case COFDM_FEC_LOCK:
        switch (system)
        {
        case E_DEVICE_DEMOD_DVB_T2:
            use_dsp_reg = 1;
            u16Address =  E_T2_DVBT2_LOCK_HIS; //FEC lock,
            cBitMask = BIT7;
            break;
        case E_DEVICE_DEMOD_DVB_T:
            MSB124X_ReadReg(pMSB124X, 0x11E0, &cData);//addy update 0805
            if (cData == 0x0B)
                return TRUE;
            else
                return FALSE;
            break;
        default:
            return FALSE;
        }
        break;

    case COFDM_TPS_LOCK:
        switch (system)
        {
        case E_DEVICE_DEMOD_DVB_T:
            use_dsp_reg = 0;
            u16Address =  0x0f22; //TPS Lock,ok
            cBitMask = BIT1;
            break;
        case E_DEVICE_DEMOD_DVB_T2:
        default:
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    if (!use_dsp_reg)
    {
        if (MSB124X_ReadReg(pMSB124X, u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (MSB124X_ReadDspReg(pMSB124X, u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    //printf("\r\n >>> MSB124X_Lock cData = %x\n",cData);
    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }
    return FALSE;
}

MS_U32 gTimeoutFlag = 0;
EN_LOCK_STATUS MSB124X_DTV_GetLockStatus(MS_U8 u8DemodIndex)
{
    MS_U32    u32Timeout = 0;
    MS_U32    u32LockTimeout = 2000;
    MS_BOOL   bCheckPass = FALSE;
    static MS_U32 u32LockTimeStartDVBT = 0;
    static MS_U32 u32LockTimeStartDVBT2 = 0;
    static MS_U32 u32LockTimeStartDVBC = 0;
    MS_U32 u32NowTime = MsOS_GetSystemTime();
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    MS_U8 u8Data;

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        u32Timeout = DVBT2_TPS_timeout;
        if (MSB124X_Lock(u8DemodIndex, E_DEVICE_DEMOD_DVB_T2, COFDM_FEC_LOCK) == TRUE)
            bCheckPass = TRUE;
        else
        {
            if (MSB124X_Lock(u8DemodIndex, E_DEVICE_DEMOD_DVB_T2, COFDM_P1_LOCK_HISTORY) == TRUE)
            {
                u32Timeout = DVBT2_FEC_timeout;
                DMD_DBG(("====> T2 P1 Locked!\n"));
            }
        }

        if (bCheckPass)
        {
            u32LockTimeStartDVBT2 = MsOS_GetSystemTime();
            pMSB124X->bFECLock = TRUE;
            DMD_DBG(("====> T2 FEC Locked!\n"));
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB124X->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBT2 < u32LockTimeout))
        {
            // SET u32LockTimeStartDVBT2 every time reset demodulator to T2
            //PRINTE(("NowTime: %d  -  pMSB124X->u32ChkScanTimeStart  %d = %d  \n",
            //    u32NowTime,pMSB124X->u32ChkScanTimeStart,(u32NowTime-pMSB124X->u32ChkScanTimeStart)));
            //PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB124X->bFECLock = FALSE;
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB124X->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        u32Timeout = DVBT_TPS_timeout;
        gTimeoutFlag = DVBT_TPS_timeout;
        if (MSB124X_Lock(u8DemodIndex, E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
            bCheckPass = TRUE;
        else
        {
            if (MSB124X_Lock(u8DemodIndex, E_DEVICE_DEMOD_DVB_T, COFDM_TPS_LOCK) == TRUE)
            {
                DMD_DBG(("T TPS locked!\n"));
                u32Timeout = DVBT_FEC_timeout;
                gTimeoutFlag = DVBT_FEC_timeout;
            }
        }
        if (bCheckPass)
        {
            u32LockTimeStartDVBT = MsOS_GetSystemTime();
            pMSB124X->bFECLock = TRUE;
            DMD_DBG(("T FEC locked!\n"));
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB124X->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBT < u32LockTimeout))
        {
            //PRINTE(("NowTime: %d  -  pMSB124X->u32ChkScanTimeStart  %d = %d  \n",
            //  u32NowTime,pMSB124X->u32ChkScanTimeStart,(u32NowTime-pMSB124X->u32ChkScanTimeStart)));
            // PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB124X->bFECLock = FALSE;
#if MSB124X_NO_CHANNEL_CHECK
            if (MSB124X_Get_No_Channel_Flag(u8DemodIndex, E_DEVICE_DEMOD_DVB_T,COFDM_DETECT_DONE_FLAG))
            {
                pMSB124X->noChannelStable = TRUE;// flag is true means more than 600 ms has been passed and no ch flag has been stable
                pMSB124X->tNoChannelFlag = MSB124X_Get_No_Channel_Flag(u8DemodIndex, E_DEVICE_DEMOD_DVB_T,COFDM_DVBT_NOCH_FLAG);
                pMSB124X->t2NoChannelFlag = MSB124X_Get_No_Channel_Flag(u8DemodIndex, E_DEVICE_DEMOD_DVB_T,COFDM_DVBT2_NOCH_FLAG);

                DMD_DBG(("NoChannel stable .... T[%d] T2[%d] \n",pMSB124X->tNoChannelFlag,pMSB124X->t2NoChannelFlag));

                if ( pMSB124X->tNoChannelFlag )
                {
                    if ( pMSB124X->t2NoChannelFlag )
                    {
                        return E_DEMOD_T_T2_UNLOCK;
                    }
                    else
                    {
                        return E_DEMOD_UNLOCK;
                    }
                }
            }
#endif
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB124X->bFECLock = FALSE;
#if MSB124X_NO_CHANNEL_CHECK
            if ( u32Timeout == DVBT_FEC_timeout
                    && ( pMSB124X->noChannelStable && pMSB124X->t2NoChannelFlag )
               )
            {
                return E_DEMOD_T_T2_UNLOCK;
            }
            else
#endif
            {
                return E_DEMOD_UNLOCK;
            }
        }
        break;
    }

    case E_DEVICE_DEMOD_DVB_S:
        if(MSB124X_Lock_S(u8DemodIndex))
            return E_DEMOD_LOCK;
        else
            return E_DEMOD_CHECKING;


    case E_DEVICE_DEMOD_DVB_C:
    {
        u32Timeout=1500;
        MSB124X_ReadReg(pMSB124X, 0x11E0, &u8Data);
        if(u8Data==0x0C)
            bCheckPass= TRUE;

        DMD_DBG((">>>MSB124x DVBC: [%s] Lock Status = %d\n", __FUNCTION__, u8Data));
        if (bCheckPass)
        {
            u32LockTimeStartDVBC=MsOS_GetSystemTime();
            pMSB124X->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB124X->u32ChkScanTimeStart < u32Timeout)||(u32NowTime - u32LockTimeStartDVBC < u32LockTimeout))
        {
            pMSB124X->bFECLock = FALSE;
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB124X->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
    }
    default:
        return E_DEMOD_UNLOCK;
    }
}

void MSB124X_DTV_GetSNR(MS_U8 u8DemodIndex)
{
    MS_U8   status = TRUE;
    MS_U32  snr_out = 0;
    MS_U16  snr_ave_num = 0;
    MS_U8   reg = 0, reg_frz;
    MS_U32  noise_power;
    float   f_snr = (float)0.0;
    float     fSNRDivider[] = {1024 * 2, 2048 * 2, 4096 * 2, 8192 * 2, 16384 * 2, 32768 * 2, 32768 * 2, 32768 * 2};
    float     snr_poly = 0.0;     // for SNR polynomial eq.
    //DVBS
    MS_U16 u16Address;
    MS_U32 u32Data;
    MS_U8  u8Data;
    float  fvariance;
    //DVBC
    EN_CAB_CONSTEL_TYPE QAM_Mode;
    MS_U8 u8tmp;


    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return ;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return ;
    }

    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {

        float fsnr1 = 0.0;
        //float fsnr2 = 0.0;
        //float snr_calib_slope = 0.0;
        float snr_calib_bias  = 0.0;

        status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x7f << 1), &reg);
        if ((reg & 0x03) == 0x03)
        {
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x7d << 1) + 1, &reg);
            snr_out = (reg & 0x1f);
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x7d << 1) + 0, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x7d << 1) + 3, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x7d << 1) + 2, &reg);
            snr_out = (snr_out << 8) | reg;

            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x7f << 1), &reg);
            status &= MSB124X_WriteReg(pMSB124X, 0x2800 + (0x7f << 1), reg & (0xff - 0x03));
            // printf("\nreg1=0x%lx\n",snr_out);
        }
        else
        {
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x0e << 1) + 3, &reg);
            snr_out = (reg & 0x1f);
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x0e << 1) + 2, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x0e << 1) + 1, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x0e << 1) + 0, &reg);
            snr_out = (snr_out << 8) | reg;
            // printf("\nreg2=0x%lx\n",snr_out);
        }

        status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x0a << 1), &reg);
        snr_ave_num = reg & 0x07;
        // printf("\nsnr_ave_num=%d\n",snr_ave_num);

        fsnr1 = 10.0 * log10((float)snr_out / fSNRDivider[snr_ave_num]);

        status &= MSB124X_ReadReg(pMSB124X, 0x2800 + (0x07 << 1), &reg);

        // HW cali en?
        if ((reg & 0x10) == 0x10)
        {
            // HW calibration.
            // slope
            MSB124X_ReadReg(pMSB124X, 0x2800 + (0x08 << 1), &reg);
            //snr_calib_slope = (float)reg / 32.0;
            // bias
            MSB124X_ReadReg(pMSB124X, 0x2800 + (0x08 << 1) + 1, &reg);
            snr_calib_bias = (float)((MS_S8)((reg & 0x1f) << 3)) / 32.0;
            snr_calib_bias = pow(2.0, snr_calib_bias);
            //fsnr2 = snr_calib_slope * fsnr1 + 10.0 * log10(snr_calib_bias);
        }
        //else
        //fsnr2 = fsnr1;

        f_snr = fsnr1;

        // printf("\n[dvbt2]slope=%.3f,bias=%.3f,fsnr1=%.2f,fsnr2=%.2f\n",snr_calib_slope,snr_calib_bias,fsnr1,fsnr2);

        //use Polynomial curve fitting to fix snr
        //snr_poly = 0.0027945*pow(*fSNR,3) - 0.2266*pow(*fSNR,2) + 6.0101*(*fSNR) - 53.3621;
        f_snr = f_snr + snr_poly;
        g_MSB124X_fSNR = f_snr;
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        // bank 15 0xfe [0] reg_fdp_freeze
        status &= MSB124X_ReadReg(pMSB124X, 0x0ffe, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x0ffe, reg_frz|0x01);

        // bank 15 0xff [0] reg_fdp_load
        status &= MSB124X_WriteReg(pMSB124X, 0x0fff, 0x01);

        // bank 15 0x4a [26:0] reg_snr_accu <27,1>
        status &= MSB124X_ReadReg(pMSB124X, 0x0f4d, &reg);
        noise_power = reg & 0x07;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f4c, &reg);
        noise_power = (noise_power << 8)|reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f4b, &reg);
        noise_power = (noise_power << 8)|reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f4a, &reg);
        noise_power = (noise_power << 8)|reg;

        noise_power = noise_power/2;

        // bank 15 0x26 [5:4] reg_transmission_mode
        status &= MSB124X_ReadReg(pMSB124X, 0x0f26, &reg);

        // bank 15 0xfe [0] reg_fdp_freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x0ffe, reg_frz);

        // bank 15 0xff [0] reg_fdp_load
        status &= MSB124X_WriteReg(pMSB124X, 0x0fff, 0x01);

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
        g_MSB124X_fSNR = f_snr;
    }
    break;
    case E_DEVICE_DEMOD_DVB_S:
    {
        u16Address=0x1B08;
        status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        u8Data|=0x10;
        status&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

        u16Address=0x1BA5;
        status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        u32Data=(u8Data&0x3F);

        u16Address=0x1BA4;
        status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        u32Data=(u32Data<<8)|u8Data;

        u16Address=0x1BA3;
        status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        u32Data=(u32Data<<8)|u8Data;

        u16Address=0x1BA2;
        status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        u32Data=(u32Data<<8)|u8Data;

        u16Address=0x1B08;
        status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        u8Data&=~(0x10);
        status&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
        if (status==FALSE)
        {
            f_snr = 0;
        }
        fvariance=(float)u32Data/0x20000000;
        if (fvariance==0)
            fvariance=1;
        f_snr=10*log10(1/fvariance);
        g_MSB124X_fSNR = f_snr;
    }
    break;
    case E_DEVICE_DEMOD_DVB_C:
    {

        QAM_Mode = MSB124X_DVB_C_GetSignalModulation(u8DemodIndex);//0:16q,1:32q,2:64q,3:128q,4:256q

        status &= MSB124X_WriteReg(pMSB124X, 0x2300+0x3a,0x20);
        status &= MSB124X_WriteReg(pMSB124X, 0x2200+0x05, 0x80);

        status &= MSB124X_ReadReg(pMSB124X, 0x2300+0x44, &u8Data);
        u8tmp = u8Data;
        status &= MSB124X_ReadReg(pMSB124X, 0x2300+0x45, &u8Data);
        noise_power = (u8Data<<8) | u8tmp;

        if (noise_power==0)//protect value 0
            noise_power=1;

        status &= MSB124X_WriteReg(pMSB124X, 0x2300+ 0x3a,0x00);
        status &= MSB124X_WriteReg(pMSB124X, 0x2200+ 0x05, 0x00);

        //f_snr=10*log10(65536/noise_power);

        switch(QAM_Mode)
        {
        case CAB_QAM16:
            f_snr=10*log10(40966/noise_power);
            break;
        case CAB_QAM32:
            f_snr=10*log10(36700/noise_power);
            break;
        case CAB_QAM64:
            f_snr=10*log10(43306/noise_power);
            break;
        case CAB_QAM128:
            f_snr=10*log10(36680/noise_power);
            break;
        case CAB_QAM256:
        default:
            f_snr=10*log10(43578/noise_power);
            break;
        }

        g_MSB124X_fSNR = f_snr;
    }
    break;

    default:
        f_snr = 0.0;
        g_MSB124X_fSNR = f_snr;
        break;
    }
}

MS_BOOL MSB124X_DTV_GetPreBER(MS_U8 u8DemodIndex, float *p_preBer)
{
    MS_U16 BitErrPeriod = 0;
    MS_U32 BitErr = 0;
    MS_U32 FecType = 0;
    MS_U8  reg = 0;
    float fber = 0;
    MS_U8 status = TRUE;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB124X_ReadReg(pMSB124X, 0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x32 Data BER count[15:0]
        // bank 38 0x33 Data BER count[31:16]
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x32 << 1) + 3, &reg);
        BitErr = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x32 << 1) + 2, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x32 << 1) + 1, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x32 << 1) + 0, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
        {
            //protect 0
            BitErrPeriod = 1;
        }

        // status &= MSB124X_ReadReg(0x268e, &reg);    //FEC Type[8:7]
        status &= MSB124X_ReadReg(pMSB124X, 0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x278e, &reg);    //FEC Type[8:7]
        FecType = (FecType << 8) | reg;
        // if (reg&0x080)
        if (FecType & 0x0180)
        {
            if (BitErr == 0)
                fber = (float)0.5 / (float)(BitErrPeriod * 64800);
            else
                fber = (float)BitErr / (float)(BitErrPeriod * 64800);
        }
        else
        {
            if (BitErr == 0)
                fber = (float)0.5 / (float)(BitErrPeriod * 16200);
            else
                fber = (float)BitErr / (float)(BitErrPeriod * 16200);
        }
        DMD_DBG(("MSB124X DVBT2 PreLDPCBER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            DMD_ERR(("DTV_DVBT2_GetPostViterbiBer Fail!\n"));
            return 0;
        }

        *p_preBer = fber;

        DMD_DBG(("[DVBT2] preber=%f, Err_num=%d, block_count=%d, reg=0x%x\n", fber, (int)BitErr, (int)BitErrPeriod, reg));
        break;

    default:
        *p_preBer = 0.0;
    }
    return status;
}


MS_BOOL MSB124X_DTV_GetPostBER(MS_U8 u8DemodIndex,float *p_postBer)
{
    MS_U16 BitErrPeriod = 0;
    MS_U16 BitErr = 0;
    MS_U16 FecType = 0;
    MS_U8  reg = 0, reg_frz;
    float fber = 0;
    MS_U8 status = TRUE;
    MS_U16 u16Address;
    MS_U8  u8Data;
    MS_U16 u16BitErrPeriod;
    MS_U32 u32BitErr;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB124X_ReadReg(pMSB124X, 0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x34 Data BER count[15:0]
        // bank 38 0x35 Data BER count[31:16]
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 3, &reg);
        BitErr = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 2, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 1, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 0, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
            //protect 0
            BitErrPeriod = 1;

        // status &= MSB124X_ReadReg(0x268e, &reg);    //FEC Type[8:7]
        status &= MSB124X_ReadReg(pMSB124X, 0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x278e, &reg);    //FEC Type[8:7]
        FecType = (FecType << 8) | reg;
        // if (reg&0x080)
        if (FecType & 0x0180)
        {
            if (BitErr == 0)
                fber = (float)0.5 / (float)(BitErrPeriod * 64800);
            else
                fber = (float)BitErr / (float)(BitErrPeriod * 64800);
        }
        else
        {
            if (BitErr == 0)
                fber = (float)0.5 / (float)(BitErrPeriod * 16200);
            else
                fber = (float)BitErr / (float)(BitErrPeriod * 16200);
        }
        DMD_DBG(("MSB124X DVBT2 PostVitBER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            DMD_ERR(("DTV_DVBT2_GetPostViterbiBer Fail!\n"));
            return 0;
        }

        *p_postBer = fber;

        break;
    case E_DEVICE_DEMOD_DVB_T:
    case E_DEVICE_DEMOD_DVB_C:
        /////////// Post-Viterbi BER /////////////
        // bank 17 0x32 [7] reg_bit_err_num_freeze
        status &= MSB124X_ReadReg(pMSB124X, 0x1132, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz | 0x80);
        // bank 17 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= MSB124X_ReadReg(pMSB124X, 0x1131, &reg);
        BitErrPeriod = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;
        // bank 17 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 17 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24
        status &= MSB124X_ReadReg(pMSB124X, 0x113D, &reg);
        BitErr = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x113C, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x113B, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x113A, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 17 0x32 [7] reg_bit_err_num_freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz);
        if (BitErrPeriod == 0)
            //protect 0
            BitErrPeriod = 1;
        if (BitErr <= 0)
            fber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
        else
            fber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);
        DMD_DBG(("MSB124X DVBT/C PostVitBER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            DMD_ERR(("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }

        *p_postBer = fber;

        break;
    case E_DEVICE_DEMOD_DVB_S:
        if (pMSB124X->bIsDVBS2 ==FALSE)//S
        {
            u16Address=0x112C;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            if (status==FALSE)
            {
                DMD_DBG(("dvbs2_MSB124X_DTV_GetSignalQuality fail!!! \n"));
                return 0;
            }
            if ((u8Data&0x02)!=0x02)
            {
                return 0;
            }
            u16Address=0x1132;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u8Data|=0x80;
            status&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
            u16Address=0x1131;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u16BitErrPeriod=u8Data;
            u16Address=0x1130;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
            u16Address=0x113D;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=u8Data;
            u16Address=0x113C;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=(u32BitErr<<8)|u8Data;
            u16Address=0x113B;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=(u32BitErr<<8)|u8Data;
            u16Address=0x113A;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=(u32BitErr<<8)|u8Data;
            u16Address=0x1132;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u8Data&=~(0x80);
            status&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
            if (u16BitErrPeriod == 0)
                u16BitErrPeriod = 1;
            if (u32BitErr <= 0)
                fber = 0.5 / ((float)u16BitErrPeriod *128 * 188 * 8);
            else
                fber = (float)u32BitErr / ((float)u16BitErrPeriod *128 * 188 * 8);

            *p_postBer = fber;
        }
        else    //S2
        {
            u16Address=0x2604;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u8Data|=0x01;
            status&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
            u16Address=0x2625;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u16BitErrPeriod=u8Data;
            u16Address=0x2624;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;

            u16Address=0x2667;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=u8Data;
            u16Address=0x2666;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=(u32BitErr<<8)|u8Data;
            u16Address=0x2665;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=(u32BitErr<<8)|u8Data;
            u16Address=0x2664;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u32BitErr=(u32BitErr<<8)|u8Data;

            u16Address=0x2604;
            status&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
            u8Data&=~(0x01);
            status&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
            if (u16BitErrPeriod == 0)
            {
                u16BitErrPeriod = 1;
            }
            fber = (float)u32BitErr/(u16BitErrPeriod*64800);
            *p_postBer = fber;
        }
        break;
    default:
        break;
    }

    return TRUE;
}

MS_U32 MSB124X_DTV_GetBER(MS_U8 u8DemodIndex)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8         status = TRUE;
    MS_U8             reg = 0, reg_frz = 0;
    MS_U16            BitErrPeriod = 0;
    MS_U32            BitErr = 0;
    MS_U16            PktErr = 0;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return 0;
    }
    else if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        /////////// Post-Viterbi BER /////////////

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB124X_ReadReg(pMSB124X, 0x1132, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz | 0x80);

        // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= MSB124X_ReadReg(pMSB124X, 0x1131, &reg);
        BitErrPeriod = reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 7 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 7 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24

        status &= MSB124X_ReadReg(pMSB124X, 0x113D, &reg);
        BitErr = reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x113C, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x113B, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x113A, &reg);
        BitErr = (BitErr << 8) | reg;

#if 1
        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //             [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB124X_ReadReg(pMSB124X, 0x113F, &reg);
        PktErr = reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x113E, &reg);
        PktErr = (PktErr << 8) | reg;
#endif

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz);

        if (BitErrPeriod == 0)  //protect 0
        {
            BitErrPeriod = 1;
        }
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

MS_U16 MSB124X_DTV_GetSignalQuality(MS_U8 u8DemodIndex)
{
#define DVBT2_BER_TH_HY 0.1
    MS_U8     status = TRUE;
    MS_U8     reg = 0; //, reg_frz;
    MS_U16    BitErrPeriod = 0;
    MS_U32    BitErr = 0;
    MS_U16    FecType = 0;
    MS_U8     u8_index = 0;
    float       fber = (float)0.0;
    float       SQI = (float)0.0;
    float       BER_SQI = (float)0.0;
    float       cn_ref = (float)0.0;
    float       cn_rec = (float)0.0;
    float       cn_rel = (float)0.0;
    static MS_U8 u8State = 0;
    float fBerTH1[] = {1E-4, 1E-4*(1.0-DVBT2_BER_TH_HY), 1E-4*(1.0+DVBT2_BER_TH_HY), 1E-4};
    float fBerTH2[] = {3E-7, 3E-7, 3E-7*(1.0-DVBT2_BER_TH_HY), 3E-7*(1.0+DVBT2_BER_TH_HY)};
    static float fBerFiltered = -1.0;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        E_T2_CONSTEL  constel =  _T2_QAM_UNKNOWN;
        E_T2_CODERATE coderate = _T2_CR_UNKNOWN;

        /////////// Check lock status /////////////
        MSB124X_ReadDspReg( pMSB124X, 0x00f0, &reg);
        if ((reg & BIT7) == 0x00)
        {
            return 0;
        }
        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB124X_ReadReg(pMSB124X, 0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x32 Data BER count[15:0]
        // bank 38 0x33 Data BER count[31:16]
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 3, &reg);
        BitErr = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 2, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 1, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x2600 + (0x34 << 1) + 0, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
            //protect 0
            BitErrPeriod = 1;

        // status &= MSB124X_ReadReg(0x268e, &reg);    //FEC Type[8:7]
        status &= MSB124X_ReadReg(pMSB124X, 0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x278e, &reg);    //FEC Type[8:7]
        FecType = (FecType << 8) | reg;
        // if (reg&0x080)
        if (FecType & 0x0180)
        {
            if (BitErr == 0)
                fber = (float)0.5 / (float)(BitErrPeriod * 64800);
            else
                fber = (float)BitErr / (float)(BitErrPeriod * 64800);
        }
        else
        {
            if (BitErr == 0)
                fber = (float)0.5 / (float)(BitErrPeriod * 16200);
            else
                fber = (float)BitErr / (float)(BitErrPeriod * 16200);
        }

        if (status == FALSE)
        {
            DMD_ERR(("DTV_GetSignalQuality_DVBT2_GetPostViterbiBer Fail!\n"));
            return 0;
        }

        if ((fBerFiltered <= 0.0) || ((fBerFiltered/fber) > 30.0 || (fBerFiltered/fber) < 0.03))
            fBerFiltered = fber;
        else
            fBerFiltered = 0.9f*fBerFiltered+0.1f*fber;
        fber = fBerFiltered;

        if (fber > fBerTH1[u8State])
        {
            BER_SQI = 0.0;
            u8State = 1;
        }
        else if (fber >=fBerTH2[u8State])
        {
            BER_SQI = 100.0/15;
            u8State = 2;
        }
        else
        {
            BER_SQI = 100.0/6;
            u8State = 3;
        }
        MSB124X_ReadReg(pMSB124X, 0x2700 + 0x47 * 2, &reg);
        coderate = (E_T2_CODERATE)(reg & 0x07);
        constel = (E_T2_CONSTEL)((reg >> 3) & 0x07);

        MSB124X_DTV_GetSNR(u8DemodIndex);
        cn_rec = g_MSB124X_fSNR;
        cn_ref = (float) - 1.0;
        while (g_msb124x_dvbt2_sqi_cn_nordigp1[u8_index].constel != _T2_QAM_UNKNOWN)
        {
            if ((g_msb124x_dvbt2_sqi_cn_nordigp1[u8_index].constel == constel)
                    && (g_msb124x_dvbt2_sqi_cn_nordigp1[u8_index].code_rate == coderate))
            {
                cn_ref = g_msb124x_dvbt2_sqi_cn_nordigp1[u8_index].cn_ref;
                break;
            }
            else
            {
                u8_index++;
            }
        }

        if (cn_ref == -1.0)
            SQI = (float)0.0;
        else
        {
            cn_rel = cn_rec - cn_ref;
            if (cn_rel > 3.0)
                SQI = 100;
            else if (cn_rel >= -3)
            {
                SQI = (cn_rel + 3) * BER_SQI;
                if (SQI > 100.0) SQI = 100.0;
                else if (SQI < 0.0) SQI = 0.0;
            }
            else
                SQI = (float)0.0;
        }

        return SQI;
        break;
    }
    case E_DEVICE_DEMOD_DVB_T:
    {
        float fber = 0.0;
        float log_ber = 0.0;
        MS_U8 u8Data = 0;
        if (MSB124X_ReadReg(pMSB124X, 0x112C, &u8Data) == FALSE)
        {
            DMD_ERR(("DTV_DVBT_GetSignalQuality ReadReg fail!!! \n"));
            return 0;
        }
        if (u8Data & 0x02 /*BIT1*/)
        {
            MS_U8 status = TRUE;
            MS_U8 reg = 0, reg_frz = 0;
            MS_U16 BitErrPeriod = 0;
            MS_U32 BitErr = 0;
            MS_U16 PktErr = 0;
            /////////// Post-Viterbi BER /////////////
            // bank 7 0x32 [7] reg_bit_err_num_freeze
            status &= MSB124X_ReadReg(pMSB124X, 0x1132, &reg_frz);
            status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz | 0x80);
            // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
            //             [15:8] reg_bit_err_sblprd_15_8
            status &= MSB124X_ReadReg(pMSB124X, 0x1131, &reg);
            BitErrPeriod = reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x1130, &reg);
            BitErrPeriod = (BitErrPeriod << 8) | reg;
            // bank 7 0x3a [7:0] reg_bit_err_num_7_0
            //             [15:8] reg_bit_err_num_15_8
            // bank 7 0x3c [7:0] reg_bit_err_num_23_16
            //             [15:8] reg_bit_err_num_31_24
            status &= MSB124X_ReadReg(pMSB124X, 0x113D, &reg);
            BitErr = reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x113C, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x113B, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x113A, &reg);
            BitErr = (BitErr << 8) | reg;
            // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
            //             [15:8] reg_uncrt_pkt_num_15_8
            status &= MSB124X_ReadReg(pMSB124X, 0x113F, &reg);
            PktErr = reg;
            status &= MSB124X_ReadReg(pMSB124X, 0x113E, &reg);
            PktErr = (PktErr << 8) | reg;
            // bank 7 0x32 [7] reg_bit_err_num_freeze
            status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz);
            if (BitErrPeriod == 0)
                //protect 0
                BitErrPeriod = 1;
            if (BitErr <= 0)
                fber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
            else
                fber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);
            DMD_DBG(("MSB124X PostVitBER = %8.3e \n ", fber));
            DMD_DBG(("MSB124X PktErr = %d \n ", (int)PktErr));

            if (status == FALSE)
            {
                DMD_ERR(("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                return 0;
            }

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
            return SQI;
        }
        else
        {
            return 0;
        }
        break;
    }
    case E_DEVICE_DEMOD_DVB_S:
    {
        float log_ber = 0.0;
        status &= MSB124X_DTV_GetPostBER(u8DemodIndex, &fber);
        if(status)
        {
            log_ber = ( - 1) *log10(1 / fber);
            if (pMSB124X->bIsDVBS2 ==FALSE)//S
            {
                if (log_ber <= ( - 7.0))
                {
                    return 100;
                }
                else if (log_ber < ( - 3.7))
                {
                    SQI = (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
                }
                else if (log_ber < ( - 2.7))
                {
                    SQI = (10+((( - 2.7) - log_ber) / (( - 2.7) - ( - 3.7))*(60-10)));
                }
                else
                {
                    SQI = 10;
                }
            }
            else //S2
            {
                if (log_ber <= ( - 7.0))
                {
                    SQI = 100;
                }
                else if (log_ber < ( - 3.7))
                {
                    SQI = (80+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-80)));
                }
                else if (log_ber < ( - 1.7))
                {
                    SQI = (40+((( - 1.7) - log_ber) / (( - 1.7) - ( - 3.7))*(80-40)));
                }
                else if (log_ber < ( - 0.7))
                {
                    SQI = (10+((( - 0.7) - log_ber) / (( - 0.7) - ( - 1.7))*(40-10)));
                }
                else
                {
                    SQI = 5;
                }
            }
            return SQI;
        }
        else
        {
            return 0;
        }
        break;
    }

    case E_DEVICE_DEMOD_DVB_C:
    {
        float   log_ber;
        float   f_snr = 0;
        EN_CAB_CONSTEL_TYPE QAM_Mode;

        status &= MSB124X_DTV_GetPostBER(u8DemodIndex, &fber);//get DVBT post ber

        MSB124X_DTV_GetSNR(u8DemodIndex);
        f_snr = g_MSB124X_fSNR;

        log_ber = (-1.0f)*log10(1.0f/fber); // Log10Approx() provide 1~2^32 input range only

        QAM_Mode = MSB124X_DVB_C_GetSignalModulation(u8DemodIndex);//0:16q,1:32q,2:64q,3:128q,4:256q

        if (QAM_Mode == CAB_QAM16)//DMD_DVBC_QAM16
        {
            if(log_ber  <= (-5.5f))
                SQI = 100;
            else if(log_ber  <= (-5.1f))
                SQI = (90.0f + ((-5.1f)-log_ber)*10.0f/((-5.1f)-(-5.5f)));
            else if(log_ber  <= (-4.9f))
                SQI = (80.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.5f))
                SQI = (70.0f + ((-4.5f)-log_ber)*10.0f/((-4.5f)-(-4.9f)));
            else if(log_ber  <= (-3.7f))
                SQI = (60.0f + ((-3.7f)-log_ber)*10.0f/((-3.7f)-(-4.5f)));
            else if(log_ber  <= (-3.2f))
                SQI = (50.0f + ((-3.2f)-log_ber)*10.0f/((-3.2f)-(-3.7f)));
            else if(log_ber  <= (-2.9f))
                SQI = (40.0f + ((-2.9f)-log_ber)*10.0f/((-2.9f)-(-3.2f)));
            else if(log_ber  <= (-2.5f))
                SQI = (30.0f + ((-2.5f)-log_ber)*10.0f/((-2.5f)-(-2.9f)));
            else if(log_ber  <= (-2.2f))
                SQI = (20.0f + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.5f)));
            else if(log_ber  <= (-2.0f))
                SQI = (0.0f + ((-2.0f)-log_ber)*10.0f/((-2.0f)-(-2.2f)));
            else
                SQI = 0;
        }
        else if (QAM_Mode == CAB_QAM32)//DMD_DVBC_QAM32
        {
            if(log_ber  <= (-5.0f))
                SQI = 100;
            else if(log_ber  <= (-4.7f))
                SQI = (90.0f  + ((-4.7f)-log_ber)*10.0f/((-4.7f)-(-5.0f)));
            else if(log_ber  <= (-4.5f))
                SQI = (80.0f  + ((-4.5f)-log_ber)*10.0f/((-4.5f)-(-4.7f)));
            else if(log_ber  <= (-3.8f))
                SQI = (70.0f  + ((-3.8f)-log_ber)*10.0f/((-3.8f)-(-4.5f)));
            else if(log_ber  <= (-3.5f))
                SQI = (60.0f  + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-3.8f)));
            else if(log_ber  <= (-3.0f))
                SQI = (50.0f  + ((-3.0f)-log_ber)*10.0f/((-3.0f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                SQI = (40.0f  + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.0f)));
            else if(log_ber  <= (-2.4f))
                SQI = (30.0f  + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.7f)));
            else if(log_ber  <= (-2.2f))
                SQI = (20.0f  + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.4f)));
            else if(log_ber  <= (-2.0f))
                SQI = (0.0f  + ((-2.0f)-log_ber)*10.0f/((-2.0f)-(-2.2f)));
            else
                SQI = 0;
        }
        else if (QAM_Mode == CAB_QAM64)//DMD_DVBC_QAM64
        {
            if(log_ber  <= (-5.4f))
                SQI = 100;
            else if(log_ber  <= (-5.1f))
                SQI = (90.0f + ((-5.1f)-log_ber)*10.0f/((-5.1f)-(-5.4f)));
            else if(log_ber  <= (-4.9f))
                SQI = (80.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.3f))
                SQI = (70.0f + ((-4.3f)-log_ber)*10.0f/((-4.3f)-(-4.9f)));
            else if(log_ber  <= (-3.7f))
                SQI = (60.0f + ((-3.7f)-log_ber)*10.0f/((-3.7f)-(-4.3f)));
            else if(log_ber  <= (-3.2f))
                SQI = (50.0f + ((-3.2f)-log_ber)*10.0f/((-3.2f)-(-3.7f)));
            else if(log_ber  <= (-2.9f))
                SQI = (40.0f + ((-2.9f)-log_ber)*10.0f/((-2.9f)-(-3.2f)));
            else if(log_ber  <= (-2.4f))
                SQI = (30.0f + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.9f)));
            else if(log_ber  <= (-2.2f))
                SQI = (20.0f + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.4f)));
            else if(log_ber  <= (-2.05f))
                SQI = (0.0f + ((-2.05f)-log_ber)*10.0f/((-2.05f)-(-2.2f)));
            else
                SQI = 0;
        }
        else if (QAM_Mode == CAB_QAM128)//DMD_DVBC_QAM128
        {
            if(log_ber  <= (-5.1f))
                SQI = 100;
            else if(log_ber  <= (-4.9f))
                SQI = (90.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.7f))
                SQI = (80.0f + ((-4.7f)-log_ber)*10.0f/((-4.7f)-(-4.9f)));
            else if(log_ber  <= (-4.1f))
                SQI = (70.0f + ((-4.1f)-log_ber)*10.0f/((-4.1f)-(-4.7f)));
            else if(log_ber  <= (-3.5f))
                SQI = (60.0f + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-4.1f)));
            else if(log_ber  <= (-3.1f))
                SQI = (50.0f + ((-3.1f)-log_ber)*10.0f/((-3.1f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                SQI = (40.0f + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.1f)));
            else if(log_ber  <= (-2.5f))
                SQI = (30.0f + ((-2.5f)-log_ber)*10.0f/((-2.5f)-(-2.7f)));
            else if(log_ber  <= (-2.06f))
                SQI = (20.0f + ((-2.06f)-log_ber)*10.0f/((-2.06f)-(-2.5f)));
            else
            {
                if (f_snr >= 27.2f)
                    SQI = 20;
                else if (f_snr >= 25.1f)
                    SQI = (0.0f + (f_snr - 25.1f)*20.0f/(27.2f-25.1f));
                else
                    SQI = 0;
            }
        }
        else //256QAM
        {
            if(log_ber  <= (-4.8f))
                SQI= 100;
            else if(log_ber  <= (-4.6f))
                SQI = (90.0f + ((-4.6f)-log_ber)*10.0f/((-4.6f)-(-4.8f)));
            else if(log_ber  <= (-4.4f))
                SQI = (80.0f + ((-4.4f)-log_ber)*10.0f/((-4.4f)-(-4.6f)));
            else if(log_ber  <= (-4.0f))
                SQI = (70.0f + ((-4.0f)-log_ber)*10.0f/((-4.0f)-(-4.4f)));
            else if(log_ber  <= (-3.5f))
                SQI = (60.0f + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-4.0f)));
            else if(log_ber  <= (-3.1f))
                SQI = (50.0f + ((-3.1f)-log_ber)*10.0f/((-3.1f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                SQI = (40.0f + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.1f)));
            else if(log_ber  <= (-2.4f))
                SQI = (30.0f + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.7f)));
            else if(log_ber  <= (-2.06f))
                SQI = (20.0f + ((-2.06f)-log_ber)*10.0f/((-2.06f)-(-2.4f)));
            else
            {
                if (f_snr >= 29.6f)
                    SQI = 20;
                else if (f_snr >= 27.3f)
                    SQI = (0.0f + (f_snr - 27.3f)*20.0f/(29.6f-27.3f));
                else
                    SQI = 0;
            }
        }

        return SQI;
        break;
    }
    default:
        break;
    }  // end of switch
    return 0;
}

MS_U16 MSB124X_DTV_GetSignalStrength(MS_U8 u8DemodIndex)
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
    MS_U8    u8RFAGC = 0;
    MS_U16   ypIFAGC1 = 0, u16Data = 0;//, ypIFAGC2 = 0;
    MS_U16   u16Signal;
    float    ch_power_db = 0;
    float    ch_power_rf = 0;
    float    ch_power_if = 0;
    float    ch_power_ref = 0;
    float    ch_power_rel = 0;
    double fPrel = 0.0, fPinput = 0.0;
    MS_U8  demodState = 0;
    MS_U8  reg = 0;
    MS_U8  reg_agc_tmp;
    MS_U8  reg_agc_tmp2;
    MS_U8   reg_frz;
    MS_U8   IF_agc_gain_HB;
    MS_U8   IF_agc_gain_LB;
    MS_U8  reg_agc_sel;
    int ii2;
    int IF_AGC_table_size;
    float dBmvalue_from_IF_AGC=0.0;
//    S_IFAGC_SSI     *ifagc_ssi;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    status &= MSB124X_ReadReg( pMSB124X, 0x0990, &reg);
    demodState = reg;
    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        /***************************************
        * Force u8RFAGC 150 due to unknown SAR port for each demo board.
        ***************************************/
        // u8RFAGC=MDrv_SAR_Adc_GetValue(2);
        u8RFAGC = 150;
        DMD_DBG((">>> RFAGC = %d <<<\n", (int)u8RFAGC));
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
        MSB124X_WriteReg(pMSB124X, 0x3e00 + 0x13 * 2, 0x03);
        MSB124X_WriteReg(pMSB124X, 0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB124X_WriteReg(pMSB124X, 0x3e00 + 0x02 * 2 + 1, 0x80);

        // agc gain2 24 Bit
        //MSB124X_ReadReg( pMSB124X, 0x3e00+0x14*2+1, &reg_tmp);
        MSB124X_ReadReg(pMSB124X, 0x3e00 + 0x14 * 2 + 2, &reg_tmp);
        //ypIFAGC2 = reg_tmp;
        MSB124X_ReadReg(pMSB124X, 0x3e00 + 0x14 * 2 + 1, &reg_tmp);
        ypIFAGC1 = reg_tmp;
        MSB124X_ReadReg(pMSB124X, 0x3e00 + 0x14 * 2 + 0, &reg_tmp);
        ypIFAGC1 = (ypIFAGC1 << 8) | reg_tmp;
        // printf("============================>[YP] IFAGC_2 = %x, IFAGC_1 = %x\n", ypIFAGC2, ypIFAGC1);
        MSB124X_ReadReg(pMSB124X, 0x3e00 + 0x14 * 2 + 1, &reg_tmp);

        MSB124X_WriteReg(pMSB124X, 0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB124X_WriteReg(pMSB124X, 0x3e00 + 0x02 * 2 + 1, 0x00);
        DMD_DBG((">>> reg_tmp = %x <<<\n", (int)reg_tmp));
        if_agc_val = reg_tmp;
        DMD_DBG((">>> if_agc_val = %d <<<\n", (int)if_agc_val));

        if (if_agc_val < 67)
        {
            ch_power_if = -55;
        }
        else if (if_agc_val < 76)
        {
            ch_power_if = (-55) - (float)((if_agc_val - 67) * 5) / (76 - 67);
        }
        else if (if_agc_val < 88)
        {
            ch_power_if = (-60) - (float)((if_agc_val - 76) * 5) / (88 - 76);
        }
        else if (if_agc_val < 99)
        {
            ch_power_if = (-65) - (float)((if_agc_val - 88) * 5) / (99 - 88);
        }
        else if (if_agc_val < 113)
        {
            ch_power_if = (-70) - (float)((if_agc_val - 99) * 5) / (113 - 99);
        }
        else if (if_agc_val < 128)
        {
            ch_power_if = (-75) - (float)((if_agc_val - 113) * 5) / (128 - 113);
        }
        else if (if_agc_val < 145)
        {
            ch_power_if = (-80) - (float)((if_agc_val - 128) * 5) / (145 - 128);
        }
        else if (if_agc_val < 165)
        {
            ch_power_if = (-85) - (float)((if_agc_val - 145) * 5) / (165 - 145);
        }
        else if (if_agc_val < 183)
        {
            ch_power_if = (-90) - (float)((if_agc_val - 165) * 5) / (183 - 165);
        }
        else
        {
            ch_power_if = -96;
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
        DMD_DBG(("ch_power_rf = %f\n", ch_power_rf));
        DMD_DBG(("ch_power_if = %f\n", ch_power_if));
        DMD_DBG(("ch_power_db = %f\n", ch_power_db));
        // -50 dB ~ -95 dB
        // 100 ~ 0

        if (ch_power_db > -50)
        {
            ssi = 100;
        }
        else if (ch_power_db < -95)
        {
            ssi = 0;;
        }
        else
        {
            ssi = 100 - ((-50 - ch_power_db) * 2.22);
        }
        MS_U8 u8Modulation = 0, u8CodeRate = 0;

        MSB124X_ReadReg(pMSB124X, 0x2700 + (0x47 * 2), &u8Modulation);
        u8Modulation = (((MS_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
        if (u8Modulation >= 3) u8Modulation = 3;
        MSB124X_ReadReg(pMSB124X, 0x2700 + (0x47 * 2), &u8CodeRate);
        u8CodeRate = (((MS_U16) u8CodeRate) & (BIT2 | BIT1 | BIT0));
        if (u8CodeRate >= 5) u8CodeRate = 5;

        //Fix me, temp solution; demod doesn't lock, then use fref = -98 to compute SSI
        if (demodState != 0x09)
        {
            fPrel = fPinput - (-50.0f);
        }
        else
        {
            fPrel = fPinput - MSB124X_fDVBT2_SSI_Pref[u8Modulation][u8CodeRate];
        }

        if (fPrel < -15.0)
        {
            ssi = 0;
        }
        else if (fPrel < 0.0)
        {
            ssi = (MS_U8)(((double)2.0 / 3) * (fPrel + 15.0));
        }
        else if (fPrel < 20.0)
        {
            ssi = 4 * fPrel + 10;
        }
        else if (fPrel < 35.0)
        {
            ssi = (MS_U8)(((double)2.0 / 3) * (fPrel - 20.0) + 90);
        }
        else
        {
            ssi = 100;
        }
        //printf("=============================>SSI: u8Modulation= %d, u8CodeRate=%d\n, fPinput=%f, fDVBT2_SSI_Pref=%f\n, fPrel=%f, ssi=%d\n",u8Modulation, u8CodeRate, fPinput, fDVBT2_SSI_Pref[u8Modulation][u8CodeRate], fPrel, ssi);

        DMD_DBG((">>> SSI = %d <<<\n", (int)ssi));
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        ///////// RF/IF-AGC Gain Out /////////////
        rf_agc_val = 0x3f;

        // ==== IFAGC_GAIN_OUT (GAIN2) ====

        // reg_agc_debug_sel[3:0]
        MSB124X_ReadReg(pMSB124X, 0x3e26, &reg_tmp);
        reg_tmp = (reg_tmp & 0xf0) | 0x03; // select ifagc_gain
        MSB124X_WriteReg(pMSB124X, 0x3e26, reg_tmp);

        // reg_agc_gain2_out
        // use only high byte value
        MSB124X_ReadReg(pMSB124X, 0x3e29, &reg_tmp);
        if_agc_val = reg_tmp;

        DMD_DBG(("SSI_RFAGC (SAR) = 0x%x\n", (MS_U16)rf_agc_val));
        DMD_DBG(("SSI_IFAGC_H = 0x%x\n", (MS_U16) if_agc_val));

        for (i = 0; i < sizeof(MSB124X_RFAGC_SSI) / sizeof(S_RFAGC_SSI); i++)
        {
            if (rf_agc_val <= MSB124X_RFAGC_SSI[i].sar3_val)
            {
                ch_power_rf = MSB124X_RFAGC_SSI[i].power_db;
                break;
            }
        }

//        ifagc_ssi = IFAGC_SSI;
        ssi_tbl_len = sizeof(MSB124X_IFAGC_SSI) / sizeof(S_IFAGC_SSI);

        for (i = 0; i < ssi_tbl_len; i++)
        {
            if (if_agc_val <= MSB124X_IFAGC_SSI[i].agc_val)
            {
                ch_power_if = MSB124X_IFAGC_SSI[i].power_db;
                break;
            }
        }

        DMD_DBG(("ch_power_rf = %f\n", ch_power_rf));
        DMD_DBG(("ch_power_if = %f\n", ch_power_if));

        ch_power_db = (ch_power_rf > ch_power_if) ? ch_power_rf : ch_power_if;

        if (MSB124X_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
        {
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB124X_ReadReg(pMSB124X, 0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB124X_ReadReg(pMSB124X, 0x0f24 + 1, &reg_tmp);
            tps_cr = (reg_tmp & 0x70) >> 4;

            for (i = 0; i < sizeof(MSB124X_SsiPref) / sizeof(S_SSI_PREF); i++)
            {
                if ((tps_cnstl == MSB124X_SsiPref[i].constel)
                        && (tps_cr == MSB124X_SsiPref[i].code_rate))
                {
                    ch_power_ref = MSB124X_SsiPref[i].p_ref;
                    break;
                }
            }
        }
        else
        {
            ch_power_ref = MSB124X_SsiPref[8].p_ref;
        }
        //printf("tps_cnstl=%d, tps_cr = %d\n", tps_cnstl, tps_cr);

        ch_power_rel = ch_power_db - ch_power_ref;

        //printf("ch_power_db = %f, ch_power_ref=%f, ch_power_rel=%f\n", ch_power_db, ch_power_ref, ch_power_rel);

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

        DMD_DBG((">>> SSI = %d <<<\n", (int)ssi));

    }
    break;
    case E_DEVICE_DEMOD_DVB_S:
    {

        status &= MSB124X_ReadReg(pMSB124X,0x3E26, &reg_tmp);
        reg_tmp = (reg_tmp&0xF0)|0x03;
        status &= MSB124X_WriteReg(pMSB124X,0x3E26, reg_tmp);

        status &= MSB124X_ReadReg(pMSB124X,0x3E04, &reg_tmp);
        reg_tmp |= 0x80;
        status &= MSB124X_WriteReg(pMSB124X,0x3E04, reg_tmp);

        status &= MSB124X_ReadReg(pMSB124X,0x3E29, &reg_tmp);
        u16Data = reg_tmp;

        status &= MSB124X_ReadReg(pMSB124X,0x3E28, &reg_tmp);
        u16Data = (u16Data<<8)|reg_tmp;

        status &= MSB124X_ReadReg(pMSB124X,0x3E04, &reg_tmp);
        reg_tmp &= ~(0x80);
        status &= MSB124X_WriteReg(pMSB124X,0x3E04, reg_tmp);

        if (status == FALSE)
        {
            DMD_DBG(("MSB124X_DTV_GetSignalStrength fail!!! \n"));
            return 0;
        }
        for (i = 0; i < SGLVEL_COUNT; i++)
        {
            if ((65535 - u16Data) <= _u16SignalLevel[i][0])
            {
                u16Signal = _u16SignalLevel[i][1];
                ssi = u16Signal/10;
                break;
            }
        }

    }
    break;

//START:  Windermere MCP DVBC   ================
    case     E_DEVICE_DEMOD_DVB_C:
    {
        if (MSB124X_DTV_GetLockStatus(u8DemodIndex) == E_DEMOD_LOCK)
        {
            ///////// Get Constellation to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            tps_cnstl=MSB124X_DVB_C_GetSignalModulation(u8DemodIndex);
            DMD_DBG(("tps_cnstl=%d \n", tps_cnstl));


            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x26, &reg_agc_sel);
            status &= MSB124X_WriteReg(pMSB124X,0x3E00 + 0x26, reg_agc_sel|0x03);

            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x03, &reg_frz);
            status &= MSB124X_WriteReg(pMSB124X,0x3E00 + 0x03, reg_frz|0x80);

            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x29, &IF_agc_gain_HB);
            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x28, &IF_agc_gain_LB);

            status &= MSB124X_WriteReg(pMSB124X,0x3E00 + 0x03, reg_agc_sel);


            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x28, &reg_agc_tmp);
            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x29, &reg_agc_tmp2);

            IF_AGC_table_size=sizeof(MSB124X_DVBC_IFAGC_SSI)/sizeof(MSB124X_DVBC_IFAGC_SSI[0]);

            for (ii2=0; ii2<=IF_AGC_table_size-1; ii2++)
            {
                if ( IF_agc_gain_HB> MSB124X_DVBC_IFAGC_SSI[ii2].agc_val)
                {

                }
                else
                {
                    dBmvalue_from_IF_AGC= MSB124X_DVBC_IFAGC_SSI[ii2].power_db;
                    break;
                }
            }


            if( (MS_U8)tps_cnstl <= 4)
            {
                ch_power_rel = dBmvalue_from_IF_AGC + intern_dvb_c_qam_ref[(MS_U8)tps_cnstl];
            }
            else
            {
                ch_power_rel = -100.0f;
            }




//DMD_DBG("ch_power_db = %f, ch_power_ref=%f, ch_power_rel=%f\n", ch_power_db, ch_power_ref, ch_power_rel);
//===================================
            if(ch_power_rel <= -85.0f)
            {
                ssi = 0;
            }
            else if (ch_power_rel <= -80.0f)
            {
                ssi = (MS_U8)(0.0f + (ch_power_rel+85.0f)*10.0f/5.0f);
            }
            else if (ch_power_rel <= -75.0f)
            {
                ssi = (MS_U8)(10.0f + (ch_power_rel+80.0f)*20.0f/5.0f);
            }
            else if (ch_power_rel <= -70.0f)
            {
                ssi = (MS_U8)(30.0f + (ch_power_rel+75.0f)*30.0f/5.0f);
            }
            else if (ch_power_rel <= -65.0f)
            {
                ssi = (MS_U8)(60.0f + (ch_power_rel+70.0f)*10.0f/5.0f);
            }
            else if (ch_power_rel <= -55.0f)
            {
                ssi = (MS_U8)(70.0f + (ch_power_rel+65.0f)*20.0f/10.0f);
            }
            else if (ch_power_rel <= -45.0f)
            {
                ssi = (MS_U8)(90.0f + (ch_power_rel+55.0f)*10.0f/10.0f);
            }
            else
            {
                ssi = 100;
            }



//===================================

            DMD_DBG((">>> SSI = %d <<<\n", (int)ssi));

        }
    }
    break;
//END: Windermere MCP DVBC   ================



    default:
        ssi = 0;
        DMD_DBG(("Undefined!!!\n"));
        break;
    }
    return ssi;
}

MS_U16 MSB124X_DTV_GetSignalStrengthWithRFPower(MS_U8 u8DemodIndex, float dBmValue)
{
    MS_U8    status = TRUE;
    MS_U8    reg_tmp = 0;
    MS_U8    tps_cnstl = 0;
    MS_U8    tps_cr = 0;
    MS_U8    ssi = 0;
    MS_U8    i = 0;
    float    ch_power_ref = 0;
    float    ch_power_rel = 0;
    float    fPrel = 0.0, fPinput = 0.0;
    MS_U8    demodState = 0;
    MS_U8    reg = 0;
    MS_U8    u8Modulation = 0, u8CodeRate = 0;

    MS_U8  reg_agc_tmp;
    MS_U8  reg_agc_tmp2;
    MS_U8   reg_frz;
    MS_U8   IF_agc_gain_HB;
    MS_U8   IF_agc_gain_LB;
    MS_U8  reg_agc_sel;
    int ii2;
    int IF_AGC_table_size;
    float dBmvalue_from_tuner=0.0;
    float dBmvalue_from_IF_AGC=0.0;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    status &= MSB124X_ReadReg(pMSB124X, 0x0990, &reg);
    demodState = reg;
    switch (pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        fPinput = dBmValue;

        // Get Modulation and code Rate
        MSB124X_ReadReg(pMSB124X, 0x2700 + (0x47 * 2), &u8Modulation);
        u8Modulation = (((MS_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
        if (u8Modulation >= 3) u8Modulation = 3;
        MSB124X_ReadReg(pMSB124X, 0x2700 + (0x47 * 2), &u8CodeRate);
        u8CodeRate = (((MS_U16) u8CodeRate) & (BIT2 | BIT1 | BIT0));
        if (u8CodeRate >= 5) u8CodeRate = 5;

        //Fix me, temp solution; demod doesn't lock, then use fref = -98 to compute SSI
        if (demodState != 0x09)
        {
            fPrel = fPinput - (-50.0f);
        }
        else
        {
            fPrel = fPinput - MSB124X_fDVBT2_SSI_Pref[u8Modulation][u8CodeRate];
        }

        if (fPrel < -15.0)
        {
            ssi = 0;
        }
        else if (fPrel < 0.0)
        {
            ssi = (MS_U8)(((double)2.0 / 3) * (fPrel + 15.0));
        }
        else if (fPrel < 20.0)
        {
            ssi = 4 * fPrel + 10;
        }
        else if (fPrel < 35.0)
        {
            ssi = (MS_U8)(((double)2.0 / 3) * (fPrel - 20.0) + 90);
        }
        else
        {
            ssi = 100;
        }
        //printf("=============================>SSI: u8Modulation= %d, u8CodeRate=%d\n, fPinput=%f, fDVBT2_SSI_Pref=%f\n, fPrel=%f, ssi=%d\n",u8Modulation, u8CodeRate, fPinput, fDVBT2_SSI_Pref[u8Modulation][u8CodeRate], fPrel, ssi);

        DMD_DBG((">>> SSI = %d <<<\n", (int)ssi));
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        if (MSB124X_Lock(u8DemodIndex, E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
        {
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB124X_ReadReg(pMSB124X, 0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB124X_ReadReg(pMSB124X, 0x0f24 + 1, &reg_tmp);
            tps_cr = (reg_tmp & 0x70) >> 4;

            for (i = 0; i < sizeof(MSB124X_SsiPref) / sizeof(S_SSI_PREF); i++)
            {
                if ((tps_cnstl == MSB124X_SsiPref[i].constel)
                        && (tps_cr == MSB124X_SsiPref[i].code_rate))
                {
                    ch_power_ref = MSB124X_SsiPref[i].p_ref;
                    break;
                }
            }
        }
        else
        {
            ch_power_ref = MSB124X_SsiPref[8].p_ref;
        }
        //printf("tps_cnstl=%d, tps_cr = %d\n", tps_cnstl, tps_cr);

        ch_power_rel = dBmValue - ch_power_ref;

        //printf("ch_power_db = %f, ch_power_ref=%f, ch_power_rel=%f\n", ch_power_db, ch_power_ref, ch_power_rel);

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

        DMD_DBG((">>> SSI = %d <<<\n", (int)ssi));

    }
    break;



// START: Windermere MCP DVBC ====================
    case E_DEVICE_DEMOD_DVB_C:
    {
        dBmvalue_from_tuner=dBmValue*100.0;

        if (MSB124X_DTV_GetLockStatus(u8DemodIndex) == E_DEMOD_LOCK)
        {
            ///////// Get Constellation to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            tps_cnstl=MSB124X_DVB_C_GetSignalModulation(u8DemodIndex);
            DMD_DBG(("tps_cnstl=%d \n", tps_cnstl));


            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x26, &reg_agc_sel);
            status &= MSB124X_WriteReg(pMSB124X,0x3E00 + 0x26, reg_agc_sel|0x03);

            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x03, &reg_frz);
            status &= MSB124X_WriteReg(pMSB124X,0x3E00 + 0x03, reg_frz|0x80);

            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x29, &IF_agc_gain_HB);
            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x28, &IF_agc_gain_LB);

            status &= MSB124X_WriteReg(pMSB124X,0x3E00 + 0x03, reg_agc_sel);


            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x28, &reg_agc_tmp);
            status &= MSB124X_ReadReg(pMSB124X,0x3E00 + 0x29, &reg_agc_tmp2);

            IF_AGC_table_size=sizeof(MSB124X_DVBC_IFAGC_SSI)/sizeof(MSB124X_DVBC_IFAGC_SSI[0]);

            for (ii2=0; ii2<=IF_AGC_table_size-1; ii2++)
            {
                if ( IF_agc_gain_HB> MSB124X_DVBC_IFAGC_SSI[ii2].agc_val)
                {

                }
                else
                {
                    dBmvalue_from_IF_AGC= MSB124X_DVBC_IFAGC_SSI[ii2].power_db;
                    break;
                }
            }


            if( (MS_U8)tps_cnstl <= 4)
            {
                if (dBmvalue_from_IF_AGC > MSB124X_DVBC_IFAGC_SSI[1].power_db
                        &&  dBmvalue_from_IF_AGC < dBmvalue_from_tuner)
                    ch_power_rel = dBmvalue_from_tuner + intern_dvb_c_qam_ref[(MS_U8)tps_cnstl];
                else
                    ch_power_rel = dBmvalue_from_IF_AGC + intern_dvb_c_qam_ref[(MS_U8)tps_cnstl];
            }
            else
            {
                ch_power_rel = -100.0f;
            }




//DMD_DBG("ch_power_db = %f, ch_power_ref=%f, ch_power_rel=%f\n", ch_power_db, ch_power_ref, ch_power_rel);
//===================================
            if(ch_power_rel <= -85.0f)
            {
                ssi = 0;
            }
            else if (ch_power_rel <= -80.0f)
            {
                ssi = (MS_U8)(0.0f + (ch_power_rel+85.0f)*10.0f/5.0f);
            }
            else if (ch_power_rel <= -75.0f)
            {
                ssi = (MS_U8)(10.0f + (ch_power_rel+80.0f)*20.0f/5.0f);
            }
            else if (ch_power_rel <= -70.0f)
            {
                ssi = (MS_U8)(30.0f + (ch_power_rel+75.0f)*30.0f/5.0f);
            }
            else if (ch_power_rel <= -65.0f)
            {
                ssi = (MS_U8)(60.0f + (ch_power_rel+70.0f)*10.0f/5.0f);
            }
            else if (ch_power_rel <= -55.0f)
            {
                ssi = (MS_U8)(70.0f + (ch_power_rel+65.0f)*20.0f/10.0f);
            }
            else if (ch_power_rel <= -45.0f)
            {
                ssi = (MS_U8)(90.0f + (ch_power_rel+55.0f)*10.0f/10.0f);
            }
            else
            {
                ssi = 100;
            }



//===================================

            DMD_DBG((">>> SSI = %d <<<\n", (int)ssi));

        }
    }
    break;
// START: Windermere MCP DVBC ====================


    default:
        ssi = 0;
        DMD_DBG(("Undefined!!!\n"));
        break;
    }
    return ssi;
}

EN_DEVICE_DEMOD_TYPE MSB124X_GetCurrentDemodulatorType(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    return pMSB124X->enDemodType ;
}

MS_BOOL MSB124X_SetCurrentDemodulatorType(MS_U8 u8DemodIndex, EN_DEVICE_DEMOD_TYPE enDemodType)
{

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (enDemodType == pMSB124X->enDemodType)
    {
        return TRUE;
    }
    pMSB124X->enDemodType = enDemodType;
    pMSB124X->u32ChkScanTimeStart = MsOS_GetSystemTime();

    switch (enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T:
        DMD_DBG((" == Config T\n"));
        if(MSB124X_SUPPORT_DVBT & pMSB124X->u8BondOpt)
        {
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle, E_DMD_MSB124X_DEMOD_DVBT);
        }
        else
        {
            DMD_DBG((" == Config T FAIL, Not Support\n"));
            return FALSE;
        }
        break;
    case E_DEVICE_DEMOD_DVB_T2:
        DMD_DBG((" == Config T2\n"));
        if(MSB124X_NOT_SUPPORT_DVBT2 & pMSB124X->u8BondOpt)
        {
            DMD_DBG((" == Config T2 FAIL, Not Support\n"));
            return FALSE;
        }
        else
        {
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle, E_DMD_MSB124X_DEMOD_DVBT2);
        }
        break;
    case E_DEVICE_DEMOD_DVB_S:
        DMD_DBG((" == Config S\n"));
        if((MSB124X_SUPPORT_DVBS |  MSB124X_SUPPORT_DVBS2) & pMSB124X->u8BondOpt)
        {
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle, E_DMD_MSB124X_DEMOD_DVBS2);
        }
        else
        {
            DMD_DBG((" == Config S FAIL, Not Support\n"));
            return FALSE;
        }
        break;
    case E_DEVICE_DEMOD_DVB_C:
        DMD_DBG((" == Config C\n"));
        MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle, E_DMD_MSB124X_DEMOD_DVBC);
        break;
    default:
        break;

    }
    return MDrv_DMD_MSB124X_LoadDSPCode_EX(pMSB124X->s32DemodHandle);
}

MS_BOOL MSB124X_Active(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    UNUSED(bEnable);
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }
    else if ((pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {

        MS_BOOL status = MSB124X_WriteReg(pMSB124X, REG_FSM_EN, 0x01); // FSM_EN

        if (status)
        {
            DMD_DBG((" @MSB124X_MSB124X_Active OK\n"));
        }
        else
        {
            DMD_DBG((" @MSB124X_MSB124X_Active NG\n"));
        }
        return status;
    }
    return FALSE;
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
MS_BOOL MSB124X_DTV_GetPlpBitMap(MS_U8 u8DemodIndex,MS_U8* u8PlpBitMap)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL   status = TRUE;
    MS_U8     u8Data = 0;
    MS_U8     indx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    status &= MSB124X_ReadDspReg(pMSB124X, 0x0120, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
        return FALSE;

    while (indx < 32)
    {
        status &= MSB124X_ReadDspReg(pMSB124X, 0x0100 + indx, &u8Data);
        u8PlpBitMap[indx] = u8Data;
        indx++;
    }

    if (status)
    {
        DMD_DBG(("\n+++++++++u8PlpBitMap data+++++++++++++++\n"));
        for (indx = 0; indx < 32; indx++)
            DMD_DBG(("[%d] ", u8PlpBitMap[indx]));
        DMD_DBG(("\n+++++++++u8PlpBitMap end+++++++++++++++\n"));
    }
    return status;
}

MS_BOOL MSB124X_Demod_InitParameter(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!"));
        return FALSE;
    }
    pMSB124X->u8ScanStatus = 0;
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}

/************************************************************************************************
Subject:    Get PlpID List and PlpID Size for MPLP mode
Function:   MSB124X_GetPlpIDList
Parmeter:   u8PlpID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB124X_GetPlpIDList(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet;
    MS_U8 i, j, u8PlpBitMap[32];
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    pMSB124X->PlpIDSize = 0;
    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));
    bRet = MSB124X_DTV_GetPlpBitMap(u8DemodIndex,u8PlpBitMap);
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((u8PlpBitMap[i] >> j) & 1)
            {
                pMSB124X->PlpIDList[pMSB124X->PlpIDSize] = i * 8 + j;
                pMSB124X->PlpIDSize++;
            }
        }
    }
    return bRet;
}

/*************************************************************************************************
Subject:    Get GROUP_ID upon PLP_ID for MPLP mode
Function:   DTV_GetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB124X_DTV_GetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL   status = TRUE;
    MS_U8 u8Data = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    status &= MSB124X_ReadDspReg(pMSB124X, 0x0120, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        DMD_ERR(("[MSB124X]dvbt2 L1 not ready yet\n"));
        return FALSE;
    }
    MSB124X_ReadDspReg(pMSB124X, 0x00f0, &u8Data);

    if ((u8Data & BIT7) == 0x00)
    {
        DMD_ERR(("[MSB124X]dvbt2 is un-lock\n"));
        return FALSE;
    }
    // assign PLP-ID value
    status &= MSB124X_WriteReg(pMSB124X, 0x2700 + (0x78) * 2, u8PlpID);
    status &= MSB124X_WriteReg(pMSB124X, 0x2700 + (0x01) * 2 + 1, 0x01); // MEM_EN
    MSB124X_Delayus(1000);
    status &= MSB124X_ReadReg(pMSB124X, 0x2700 + (0x79) * 2, u8GroupID);
    status &= MSB124X_WriteReg(pMSB124X, 0x2700 + (0x01) * 2 + 1, 0x00); // ~MEM_EN

    return status;
}


/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB124X_DTV_SetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8 u8GroupID)
{

    MS_BOOL   status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    pMSB124X->u32ChkScanTimeStart = MsOS_GetSystemTime();
    DMD_DBG(("[start]\n"));

    // assign Group-ID and PLP-ID value (must be written in order)
    status &= MSB124X_WriteDspReg(pMSB124X, 0x0122, u8GroupID);
    status &= MSB124X_WriteDspReg(pMSB124X, 0x0121, u8PlpID);
    DMD_DBG(("[end, return %d]\n", status));
    return status;
}

MS_BOOL MSB124X_Demod_SetScanTypeStatus(MS_U8 u8DemodIndex, MS_U8 status)
{
    DMD_DBG(("#### In MSB124X_Demod_SetScanTypeStatus, %x %x\n", u8DemodIndex, status));
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    switch (status)
    {
    case 0:
        pMSB124X->u8ScanStatus = 0;
        break;
    case 1:
        pMSB124X->u8ScanStatus = 1;
        break;

    case 2:
        pMSB124X->u8ScanStatus = 2;
        break;

    case 3:
        pMSB124X->u8ScanStatus = 3;
        break;
    default:
        MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
        return FALSE;

    }
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}

MS_U8 MSB124X_Demod_GetScanTypeStatus(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    return pMSB124X->u8ScanStatus;
}

MS_BOOL MSB124X_DTV_GetNextPLPID(MS_U8 u8DemodIndex, MS_U8 Index, MS_U8* pu8PLPID)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    *pu8PLPID = pMSB124X->PlpIDList[Index];
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;

}


MS_BOOL MSB124X_T2_SetPlpID(MS_U8 u8DemodIndex,MS_U8 u8PlpID)
{
    MS_BOOL bRet = FALSE;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }
    {
        //MS_U32 u32Timeout;
        MS_U8 u8Data;
        MS_U16 u16RegAddress;
        MS_U8 u8LockState;
        MS_BOOL bCheckPass = FALSE;

        //u32Timeout = 1500;
        u16RegAddress = 0x0990;
        u8LockState = 0x09;
        MSB124X_ReadReg(pMSB124X, u16RegAddress, &u8Data);
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        if ((u8PlpID != 0xFF) && (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2) && (bCheckPass))
        {

            MS_U16 u16Retry = 0;
            MS_U8 u8GroupId = 0;
            MSB124X_Delayus(500 * 1000);

            bRet = MSB124X_DTV_GetPlpGroupID(u8DemodIndex,u8PlpID, &u8GroupId);
            while ((bRet == FALSE) && (u16Retry < 60))
            {
                u16Retry++;
                DMD_DBG(("DoSet_DVBT2 get groupid retry %d \n", u16Retry));
                MSB124X_Delayus(100 * 1000);
                bRet = MSB124X_DTV_GetPlpGroupID(u8DemodIndex,u8PlpID, &u8GroupId);
            }
            if (bRet == FALSE)
            {
                DMD_ERR(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_GetPlpGroupID(%d) Error \n", u8PlpID));
                return FALSE;
            }

            bRet = MSB124X_DTV_SetPlpGroupID(u8DemodIndex,u8PlpID, u8GroupId);
            if (bRet == FALSE)
            {
                DMD_ERR(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_SetPlpGroupID(%d,%d) Error", u8PlpID, u8GroupId));
                return FALSE;
            }
        }
    }
    return TRUE;
}



MS_BOOL  MSB124X_DTV_TUNER_IsIFChanged(MS_U8 u8DemodIndex,MS_U8 eBandWidth)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    printf("Demod CurrBW=%d, TuneBW=%d\n", pMSB124X->enCurrBW, pMSB124X->enTuneBw);
    if (pMSB124X->enCurrBW == pMSB124X->enTuneBw)
    {
        return FALSE;
    }
    if (((DEMOD_BW_MODE_6MHZ != pMSB124X->enCurrBW) && (DEMOD_BW_MODE_6MHZ == pMSB124X->enTuneBw)) || \
            ((DEMOD_BW_MODE_6MHZ == pMSB124X->enCurrBW) && (DEMOD_BW_MODE_6MHZ != pMSB124X->enTuneBw)))
    {


        switch (eBandWidth)
        {
        case 6:

            pMSB124X->enCurrBW= DEMOD_BW_MODE_6MHZ;
            break;
        case 7:

            pMSB124X->enCurrBW= DEMOD_BW_MODE_7MHZ;
            break;
        case 8:
        default:
            pMSB124X->enCurrBW= DEMOD_BW_MODE_8MHZ;
            break;
        }

        return TRUE;
    }
    return FALSE;
}

// DVBT2 95~101ms, DVBT 38~39ms
MS_BOOL MSB124X_DTV_SetFrequency(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U8 eBandWidth)
{
    MS_U8 bw_t2 = 0, bw_t = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    DMD_DBG(("\n[%s]->freq=%d,bdwh=%d,PlpID %d,type %s\n", __FUNCTION__, (int)(pParam->u32Frequency), (int)eBandWidth, (int)(pParam->TerParam.u8PlpID), E_DEVICE_DEMOD_DVB_T == pMSB124X->enDemodType ? "T" : "T2"));


    pMSB124X->bFECLock = FALSE;
    pMSB124X->u32ChkScanTimeStart = MsOS_GetSystemTime();

#if(TIMING_VERIFICATION == 1)
    tmm_11 = GIVE_ME_TIME;
#endif

//set freq.bw and plp id to demod

    switch (eBandWidth)
    {
    case 6:
        bw_t2 = E_DEMOD_BW_6M;
        bw_t   = E_DEMOD_BW_6M - 1;
        pMSB124X->enTuneBw = DEMOD_BW_MODE_6MHZ;
        break;
    case 7:
        bw_t2 = E_DEMOD_BW_7M;
        bw_t  = E_DEMOD_BW_7M - 1;
        pMSB124X->enTuneBw = DEMOD_BW_MODE_7MHZ;
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
        pMSB124X->enTuneBw = DEMOD_BW_MODE_8MHZ;
        break;
    }

    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        DMD_DBG(("\nsetfreq T2\n"));
        MSB124X_Reset(u8DemodIndex);
        MSB124X_Delayus(500);
        MSB124X_WriteDspReg(pMSB124X, (MS_U16)E_T2_BW, bw_t2);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB124X_WriteDspReg(pMSB124X, (MS_U16)E_T2_PLP_ID, pParam->TerParam.u8PlpID);

        MSB124X_DTV_TUNER_IsIFChanged(u8DemodIndex,eBandWidth);//fix when 820t tuner's IF changes with bandwidth,MSB124X_SetCurrentDemodulatorType will set IF to default
//#if(FRONTEND_TUNER_TYPE == TUNER_R820T)
#if 0
        MS_U8 u8FcLow,u8FcHigh;

        if (DEMOD_BW_MODE_6MHZ == pMSB124X->enTuneBw)  //6M
        {
            u8FcLow = T2_FC_L_VAL_6M;
            u8FcHigh = T2_FC_H_VAL_6M;
        }
        else     //7, 8M
        {
            u8FcLow = T2_FC_L_VAL;
            u8FcHigh = T2_FC_H_VAL;
        }
        MSB124X_WriteDspReg( pMSB124X, (MS_U16)E_T2_FC_L, (MS_U8)u8FcLow);
        MSB124X_WriteDspReg( pMSB124X, (MS_U16)E_T2_FC_H, (MS_U8)(u8FcHigh));

#endif
        if(pMSB124X->bIsMCP_DMD)
        {
            MSB124X_WriteReg2bytes(pMSB124X, 0x0A00+(0x18)*2, 0x0100);
        }
        else
        {
            MSB124X_WriteReg2bytes(pMSB124X, 0x0A00+(0x18)*2, 0x0001);
        }

        MSB124X_WriteDspReg(pMSB124X, (MS_U16)E_T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL);

        MSB124X_WriteReg(pMSB124X, REG_FSM_EN, 0x01); // FSM_EN
        MSB124X_Show_Version(u8DemodIndex);//show dvbt2 fw version
    }
    else if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        DMD_DBG(("\nsetfreq T\n"));
        MSB124X_Reset(u8DemodIndex);
        MSB124X_Delayus(500);

        MSB124X_WriteDspReg(pMSB124X, (MS_U8)T_CONFIG_BW, bw_t);      // BW: 1->6M, 2->7M, 3->8M
        MSB124X_WriteDspReg(pMSB124X, (MS_U8)T_CONFIG_LP_SEL, pParam->TerParam.eLevelSel);
        MSB124X_DTV_TUNER_IsIFChanged(u8DemodIndex,eBandWidth);//fix when 820t tuner's IF changes with bandwidth,MSB124X_SetCurrentDemodulatorType will set IF to default
//#if(FRONTEND_TUNER_TYPE == TUNER_R820T)
#if 0
        MS_U8 u8FcLow,u8FcHigh;

        if (DEMOD_BW_MODE_6MHZ == pMSB124X->enTuneBw)  //6M
        {
            u8FcLow = FC_L_6M;
            u8FcHigh = FC_H_6M;
        }
        else     //7, 8M
        {
            u8FcLow = FC_L;
            u8FcHigh = FC_H;
        }
        MSB124X_WriteDspReg( pMSB124X, (MS_U16)T_CONFIG_FC_L, (MS_U8)u8FcLow);
        MSB124X_WriteDspReg( pMSB124X, (MS_U16)T_CONFIG_FC_H, (MS_U8)(u8FcHigh));
#endif

        //pMSB124X->pMSB124X_RegDVBT[(MS_U8)T_CONFIG_BW - (MS_U8)T_OPMODE_RFAGC_EN] = (MS_U8)bw_t;
        //pMSB124X->pMSB124X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MDrv_DMD_MSB124X_DTV_DVBT_DSPReg_CRC();
        //MSB124X_WriteDspReg((MS_U8)T_PARAM_CHECK_SUM, pMSB124X->pMSB124X_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]);    // BW: 1->6M, 2->7M, 3->8M

        // for MCP demod, set AGC to DVBS AGC pad, otherwise set AGC to DVBT AGC pad
        if(pMSB124X->bIsMCP_DMD)
        {
            MSB124X_WriteReg2bytes(pMSB124X, 0x0A00+(0x18)*2, 0x0100);
        }
        else
        {
            MSB124X_WriteReg2bytes(pMSB124X, 0x0A00+(0x18)*2, 0x0001);
        }

        MSB124X_Active(u8DemodIndex, TRUE);
        MSB124X_Show_Version(u8DemodIndex);//show dvbt fw version
    }

#if(TIMING_VERIFICATION == 1)
    tmm_12 = GIVE_ME_TIME;
    show_timer();
#endif
    return TRUE;
}

MS_BOOL  MSB124X_DVBS_SetFrequency(MS_U8 u8DemodIndex, MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16LockCount;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    bRet &= pMSB124X->MSB124X_InitParam.pstTunertab->SetFreqS2(u8DemodIndex,u16CenterFreq, u32SymbolRate_Hz);

    if (bRet==FALSE)
        return bRet;

    u16LockCount=0;
    do
    {
        bRet&= pMSB124X->MSB124X_InitParam.pstTunertab->GetLock(u8DemodIndex);
        MsOS_DelayTask(1);
        u16LockCount++;
    }
    while((bRet==FALSE) && (u16LockCount<MSB124X_TUNER_WAIT_TIMEOUT)) ;

    if (bRet==TRUE)
    {
        DMD_DBG(("Tuner PLL Lock\n"));
    }
    else
    {
        DMD_DBG(("Tuner PLL Unlock\n"));
    }
    return bRet;
}


MS_U32 MSB124X_GET_SPECTRUM_INV(MS_U8 u8DemodIndex, MS_BOOL *spectrum_inv)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8         status = TRUE;
    MS_U8         reg = 0;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("pMSB124X have not inited !!!"));
        return FALSE;
    }

    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        status &= MSB124X_ReadReg(pMSB124X,0x2400+0x41*2, &reg);
        *spectrum_inv = (reg >> 2) & 0x01;
        return TRUE;
    }
    else if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        status &= MSB124X_ReadReg(pMSB124X, 0x1400+0x54*2, &reg);
        *spectrum_inv = (reg >> 0) & 0x01;
        return TRUE;
    }
    return FALSE;
}


/*================================================
==         demod  MSB124X getting dvbt/dvbt2 param interface
=================================================*/
MS_BOOL MSB124X_DTV_DVB_T2_Get_L1_Parameter(MS_U8 u8DemodIndex, MS_U16 * pu16L1_parameter, E_SIGNAL_TYPE eSignalType)
{
    MS_U8         u8Data = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MSB124X_Lock(u8DemodIndex, E_DEVICE_DEMOD_DVB_T2, COFDM_FEC_LOCK) == TRUE)
    {

        if (eSignalType == T2_MODUL_MODE)
        {

            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x47 * 2), &u8Data) == FALSE)
                return FALSE;

            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT5 | BIT4 | BIT3)) >> 3;

        }

        else  if (eSignalType == T2_CODE_RATE)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x47 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT2 | BIT1 | BIT0));

        }
        else if (eSignalType == T2_PREAMBLE)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT4)) >> 4;
        }
        else if (eSignalType == T2_S1_SIGNALLING)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT3 | BIT2 | BIT1)) >> 1;
        }
        else if (eSignalType == T2_PILOT_PATTERN)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x36 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0x0F);
        }
        else if (eSignalType == T2_BW_EXT)
        {
            if (MSB124X_ReadReg( pMSB124X, 0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT0));
        }
        else if (eSignalType == T2_PAPR_REDUCTION)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0xF0) >> 4;
        }
        else if (eSignalType == T2_OFDM_SYMBOLS_PER_FRAME)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x3C * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (MS_U16) u8Data;
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x3C * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter |= (((MS_U16) u8Data) & 0x0F) << 8;
        }
        else if (eSignalType == T2_FFT_VALUE)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            if ((u8Data & (BIT3 | BIT2)) >> 2)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT7 | BIT6 | BIT5)) >> 5;
        }
        else if (eSignalType == T2_GUARD_INTERVAL)
        {
            if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT3 | BIT2 | BIT1)) >> 1;
        }
        else if (eSignalType == T2_PLP_TYPE)        
       {            
           if (MSB124X_ReadReg(pMSB124X, 0x2700 + (0x45 * 2) + 1, &u8Data) == FALSE)            
           {                
                return FALSE;            
           }            
           *pu16L1_parameter  = ((MS_U16) u8Data) & (BIT2 | BIT1 | BIT0);                
       }
       else
       {
            return FALSE;
       }

        return TRUE;

    }
    return FALSE;
}

MS_BOOL MSB124X_DTV_DVB_C_GetSignalTpsInfo(MS_U8 u8DemodIndex, MS_U64 *L1_Info )
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    MS_U8  u8_qam_mode;
    MS_U8  u8_temp1;

    MS_U8 u8_sym_temp0,u8_sym_temp1,u8_sym_temp2,u8_sym_temp3;
    MS_U32 reg_intp_ratem1;
    MS_U16 u16_symbol_rate;
    float f_symbol_rate,Intp_ratem1;
    float FS=0.0;

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MSB124X_DTV_GetLockStatus(u8DemodIndex) == E_DEMOD_LOCK)
    {

// START: QAM mode ================================================================
        if (MSB124X_ReadReg(pMSB124X,0x2300+0x02,&u8_temp1)==FALSE)
            return FALSE;
        else
            u8_qam_mode= u8_temp1 & 0x07;

// END: QAM mode ================================================================

//START:  DVBC symbol rate ===========================================================
        if(MSB124X_ReadReg(pMSB124X,0x2C00+0x27*2+0,&u8_sym_temp0)==FALSE) return FALSE;
        if(MSB124X_ReadReg(pMSB124X,0x2C00+0x27*2+1,&u8_sym_temp1)==FALSE) return FALSE;
        if(MSB124X_ReadReg(pMSB124X,0x2C00+0x27*2+2,&u8_sym_temp2)==FALSE) return FALSE;
        if(MSB124X_ReadReg(pMSB124X,0x2C00+0x27*2+3,&u8_sym_temp3)==FALSE) return FALSE;
        else
        {
            reg_intp_ratem1=
                ((MS_U32)(u8_sym_temp0))+
                (((MS_U32)(u8_sym_temp1))<<8)+
                (((MS_U32)(u8_sym_temp2))<<16)+
                (((MS_U32)(u8_sym_temp3))<<24);
            Intp_ratem1=(float)reg_intp_ratem1/(float)(1<<22);


        }
        FS=(float)(C_FS_H<<8) + (float)C_FS_L;
        f_symbol_rate=FS/(2.0*(Intp_ratem1+1.0));
        u16_symbol_rate=(MS_U16) f_symbol_rate;

//END:  DVBC symbol rate ===========================================================

        *L1_Info = u16_symbol_rate;
        *L1_Info |= u8_qam_mode<<16;

    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


MS_BOOL MSB124X_DTV_DVB_T_GetSignalTpsInfo(MS_U8 u8DemodIndex, MS_U16 *TPS_parameter)
{
    MS_U8 u8Temp;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MSB124X_ReadReg(pMSB124X, 0x0F00 + 0x22, &u8Temp) == FALSE)
        return FALSE;

    if ((u8Temp & 0x02) != 0x02)
    {
        return FALSE; //TPS unlock
    }
    else
    {
        if (MSB124X_ReadReg( pMSB124X, 0x0F00 + 0x24, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter = u8Temp & 0x03;         //Constellation (b2 ~ b0)
        *TPS_parameter |= (u8Temp & 0x70) >> 1; //Hierarchy (b5 ~ b3)

        if (MSB124X_ReadReg(pMSB124X, 0x0F00 + 0x25, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x07) << 6; //LP Code Rate (b8 ~ b6)
        *TPS_parameter |= (MS_U16)(u8Temp & 0x70) << 5; //HP Code Rate (b11 ~ b9)

        if (MSB124X_ReadReg(pMSB124X, 0x0F00 + 0x26, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x03) << 12; //GI (b13 ~ b12)
        *TPS_parameter |= (MS_U16)(u8Temp & 0x30) << 10;  //FFT ( b14)

        if (MSB124X_ReadReg(pMSB124X, 0x0F00 + 0x0C, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x08) << 12; //Priority(bit 15)

    }
    return TRUE;
}

MS_BOOL MSB124X_DTV_DVB_T2_GetSignalL1Info(MS_U8 u8DemodIndex, MS_U64 *L1_Info)
{
    MS_BOOL bRet = TRUE;
    MS_U16    u16Data = 0;
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    char*  cConStr[] = {"qpsk", "16qam", "64qam", "256qam"};
    char*  cCRStr[] = {"1_2", "3_5", "2_3", "3_4", "4_5", "5_6"};
    char*  cGIStr[] = {"1_32", "1_16", "1_8", "1_4", "1_128", "19_128", "19_256"};
    char*  cFFTStr[] = {"2k", "8k", "4k", "1k", "16k", "32k", "8k", "32k"};
    char*  cPreAStr[] = {"mixed", "not_mixed"};
    char*  cS1SStr[] = {"t2_siso", "t2_miso", "non_t2", "reserved"};
    char*  cPPSStr[] = {"PP1", "PP2", "PP3", "PP4", "PP5", "PP6", "PP7", "PP8", "reserved"};
    char*  cBWStr[] = {"normal", "extension"};
    char*  cPAPRStr[] = {"none", "ace", "tr", "tr_and_ace", "reserved"};
#endif
    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_MODUL_MODE) == FALSE)
    {
        DMD_ERR(("TS_MODUL_MODE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info = (MS_U64)(u16Data);
    DMD_DBG(("Constellation:%s\n", cConStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_CODE_RATE) == FALSE)
    {
        DMD_ERR(("TS_CODE_RATE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info |= (MS_U64)(u16Data << 3);
    DMD_DBG(("Code Rate:%s\n", cCRStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_GUARD_INTERVAL) == FALSE)
    {
        DMD_ERR(("TS_T2_GUARD_INTERVAL Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info |= (MS_U64)(u16Data << 6);
    DMD_DBG(("GI:%s\n", cGIStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_FFT_VALUE) == FALSE)
    {
        DMD_ERR(("TS_T2_FFT_VALUE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info |= (MS_U64)(u16Data << 9);
    DMD_DBG(("FFT:%s\n", cFFTStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_PREAMBLE) == FALSE)
    {
        DMD_ERR(("TS_PREAMBLE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x01;
    *L1_Info |= (MS_U64)(u16Data << 12);
    DMD_DBG(("Preamble:%s\n", cPreAStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_S1_SIGNALLING) == FALSE)
    {
        DMD_ERR(("TS_S1_SIGNALLING Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x03;
    if (u16Data > 2)
        u16Data = 3;
    *L1_Info |= (MS_U64)(u16Data << 13);
    DMD_DBG(("S1Signalling:%s\n", cS1SStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_PILOT_PATTERN) == FALSE)
    {
        DMD_ERR(("TS_PILOT_PATTERN Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x0F;
    if (u16Data > 7)
        u16Data = 8;
    *L1_Info |= (MS_U64)(u16Data << 15);
    DMD_DBG(("PilotPattern:%s\n", cPPSStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_BW_EXT) == FALSE)
    {
        DMD_ERR(("TS_BW_EXT Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x01;
    *L1_Info |= (MS_U64)(u16Data << 19);
    DMD_DBG(("BW EXT:%s\n", cBWStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_PAPR_REDUCTION) == FALSE)
    {
        DMD_ERR(("TS_PAPR_REDUCTION Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    if (u16Data > 3)
        u16Data = 4;
    *L1_Info |= (MS_U64)(u16Data << 20);
    DMD_DBG(("PAPR:%s\n", cPAPRStr[u16Data]));

    if (MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16Data, T2_OFDM_SYMBOLS_PER_FRAME) == FALSE)
    {
        DMD_ERR(("TS_OFDM_SYMBOLS_PER_FRAME Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0xFFF;
    *L1_Info |= (MS_U64)(u16Data << 23);
    DMD_DBG(("OFDM Symbols:%u\n", u16Data));

    return bRet;
}

MS_BOOL MSB124X_DTV_GetPLPType(MS_U8 u8DemodIndex, DEMOD_DVBT2_PLP_TYPE* ePLP_TYPE)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    MS_U16 u16data=0;
    MS_BOOL bret = TRUE;

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    bret &= MSB124X_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex, &u16data, T2_PLP_TYPE);

    if(!bret)
        *ePLP_TYPE = DEMOD_DVBT2_PLP_TYPE_INVALID;
    else
    {
        switch(u16data)
        {
            case 0:
                *ePLP_TYPE = DEMOD_DVBT2_PLP_TYPE_COMMON;
                break;
            case 1:
                *ePLP_TYPE = DEMOD_DVBT2_PLP_TYPE_1;
                break;
            case 2:
                *ePLP_TYPE = DEMOD_DVBT2_PLP_TYPE_2;
                break;
            default:
                *ePLP_TYPE = DEMOD_DVBT2_PLP_TYPE_INVALID;
                break;
        }

    }
        
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bret;

}


MS_BOOL MSB124X_DTV_GetCellID(MS_U8 u8DemodIndex, MS_U16 *pCellID)
{
    MS_U8 id = 0;
    MS_U8 status = TRUE;
    MS_U16  cell_id  = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        status &= MSB124X_ReadReg(pMSB124X, 0x2771, &id);
        cell_id = (MS_U16)id;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f70, &id);
        cell_id |= (MS_U16)id << 8;
    }
    else if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        status &= MSB124X_WriteReg(pMSB124X, 0x0ffe, 0x01);

        status &= MSB124X_ReadReg(pMSB124X, 0x0f2b, &id);
        cell_id = (MS_U16)id;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f2a, &id);
        cell_id |= (MS_U16)id << 8;

        status &= MSB124X_WriteReg(pMSB124X, 0x0ffe, 0x00);

    }

    *pCellID = cell_id;

    if (status)
    {
        DMD_DBG((" @MSB124X_GetCellId OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_DBG((" @MSB124X_GetCellId NG\n"));
        return FALSE;
    }

}

MS_BOOL MSB124X_DTV_GetSNR_F(MS_U8 u8DemodIndex,float *fSNR)
{
    MSB124X_DTV_GetSNR(u8DemodIndex);
    *fSNR = g_MSB124X_fSNR;
    return TRUE;
}

MS_BOOL MSB124X_DTV_Get_Current_Plp_Id(MS_U8 u8DemodIndex, MS_U8 *plp_id)
{
    MS_BOOL  rbet = TRUE;
    MS_U8    reg;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    rbet &= MSB124X_ReadDspReg(pMSB124X, (MS_U16)E_T2_PLP_ID, &reg);
    *plp_id = reg;
    return rbet;
}

void MSB124X_DTV_ControlTsOutput(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return;
    }

    if (bEnable)
    {
        MSB124X_WriteReg(pMSB124X, 0x0900 + (0x2D * 2), 0xFF);
    }
    else
    {
        MSB124X_WriteReg(pMSB124X, 0x0900 + (0x2D * 2), 0x00);
    }
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U16 MSB124X_Get_FreqOffset(MS_U8 u8DemodIndex, float *pFreqOff, MS_U8 u8BW)
{
    MS_U8 status = TRUE;
    //DVB-T
    float   N, FreqB;
    float   FreqCfoTd, FreqCfoFd, FreqIcfo;
    MS_U32  RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8 reg_frz, reg, reg_debug_sel;
    MS_S32 RegTotalCfo;
    EN_DEVICE_DEMOD_TYPE eSystems;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    FreqB = (float)u8BW * 8.0 / 7.0;
    eSystems = MSB124X_GetCurrentDemodulatorType(u8DemodIndex);
    DMD_DBG(("\nBW = %d\n", u8BW));
    if (eSystems == E_DEVICE_DEMOD_DVB_T2)
    {
        //debug select
        status &= MSB124X_ReadReg(pMSB124X, 0x3E5E, &reg_debug_sel);
        reg_debug_sel &= 0xCF;
        reg_debug_sel |= 0x10;
        status &= MSB124X_WriteReg(pMSB124X, 0x3E5E, reg_debug_sel);

        // set freeze
        status &= MSB124X_WriteReg2bytes(pMSB124X, 0x3E00 + (0x02) * 2, 0x8000);
        status &= MSB124X_ReadReg(pMSB124X, 0x3E5B, &reg);
        RegTotalCfo = reg & 0x0F;

        status &= MSB124X_ReadReg(pMSB124X, 0x3E5A, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x3E59, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x3E58, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;

        // release freeze
        status &= MSB124X_WriteReg2bytes(pMSB124X, 0x3E00 + (0x02) * 2, 0x0000);

        if (RegTotalCfo >= 0x4000000)
            RegTotalCfo = RegTotalCfo - 0x8000000;

        *pFreqOff = (float)RegTotalCfo * FreqB * 0.00000745;

        DMD_DBG(("$$$$$$$$$$$$$$$$$$$$  DVB-T2 CFOE = %f\n", *pFreqOff));
        return status;
    }
    else if (eSystems == E_DEVICE_DEMOD_DVB_T)
    {
        // freeze
        status &= MSB124X_ReadReg(pMSB124X, 0x0E04 + 1, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x0E04 + 1, reg_frz | 0x80);

        status &= MSB124X_ReadReg(pMSB124X, 0x0E28 + 2, &reg);
        RegCfoTd = reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x0E28 + 1, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x0E28, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        // release
        status &= MSB124X_ReadReg(pMSB124X, 0x0E04 + 1, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x0E04 + 1, reg_frz | 0x80);

        FreqCfoTd = (float)RegCfoTd;

        if (RegCfoTd & 0x800000)
            FreqCfoTd = FreqCfoTd - (float)0x1000000;

        FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

        status &= MSB124X_ReadReg(pMSB124X, 0x0ffe, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x0ffe, reg_frz | 0x01);

        status &= MSB124X_WriteReg(pMSB124X, 0x0fff, 0x01);

        status &= MSB124X_ReadReg(pMSB124X, 0x0f33, &reg);
        RegCfoFd = reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f32, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f31, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        FreqCfoFd = (float)RegCfoFd;

        if (RegCfoFd & 0x800000)
            FreqCfoFd = FreqCfoFd - (float)0x1000000;

        FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f19, &reg);
        RegIcfo = reg & 0x07;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f18, &reg);
        RegIcfo = (RegIcfo << 8) | reg;

        FreqIcfo = (float)RegIcfo;

        if (RegIcfo & 0x400)
            FreqIcfo = FreqIcfo - (float)0x800;

        status &= MSB124X_ReadReg(pMSB124X, 0x0f26, &reg);
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
        status &= MSB124X_WriteReg(pMSB124X, 0x0ffe, reg_frz & (~0x01));
        status &= MSB124X_WriteReg(pMSB124X, 0x0fff, 0x01);

        *pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd) / 1000;

        status &= MSB124X_ReadReg(pMSB124X, 0x3E5E, &reg);

        if (reg & 0x01)
            *pFreqOff = -*pFreqOff;

        DMD_DBG(("$$$$$$$$$$$$$$$$$$$$  DVB-T CFOE = %f\n", *pFreqOff));

    }
    else
        ;

    if (status)
    {
        DMD_DBG((" @MSB124X_Get_FreqOffset OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_ERR((" @MSB124X_Get_FreqOffset NG\n"));
        return FALSE;
    }
}

MS_U8 MSB124X_Get_Packet_Error(MS_U8 u8DemodIndex, MS_U16 *u16_data)
{
    MS_U8     status = TRUE;
    MS_U8     reg = 0, reg_frz;
    MS_U16      PktErr = 0;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (E_DEVICE_DEMOD_DVB_T2 == MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        //freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x01);
        //read packet error
        status &= MSB124X_ReadReg(pMSB124X, 0x265B, &reg);
        PktErr = reg;
        status &= MSB124X_ReadReg(pMSB124X, 0x265A, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;
        //release
        status &= MSB124X_WriteReg(pMSB124X, 0x2604, 0x00);
    }
    else
    {
        reg_frz = 0;
        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB124X_ReadReg(pMSB124X, 0x1132, &reg_frz);
        status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz | 0x80);

        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //         [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB124X_ReadReg(pMSB124X, 0x113F, &reg);
        PktErr = reg;

        status &= MSB124X_ReadReg(pMSB124X, 0x113E, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB124X_WriteReg(pMSB124X, 0x1132, reg_frz);

        DMD_DBG(("===================>MSB PktErr = %d \n ", (int)PktErr));
    }
    if (status)
    {
        DMD_DBG((" @MSB124X_Get_Packet_Error OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_ERR((" @MSB124X_Get_Packet_Error NG\n"));
        return FALSE;
    }
}

/********************************************
Function: To control MSB1233C/1236C GPIO pin for input_out mode
Parameter: Pin_sel   0->GPIO0, 1->RFAGC
                 Ctrl_Para  0->switch to output mode  1->input mode
                 Data_Para  input/output data
*********************************************/
MS_BOOL MSB124X_DTV_GPIO_Control(MS_U8 u8DemodIndex, MS_U8 Pin_sel, MS_U8 Ctrl_Para, MS_U8 *pData_Para)
{
    MS_U16 pin_addr, in_addr, out_addr;
    MS_U8 u8_read, u8_write, bit_num;
    MS_BOOL status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    //select GPIO pin contol
    switch (Pin_sel)
    {
    case 0:
        //set GPIO0 pin
        pin_addr = 0x0900 + (0x64 * 2);
        in_addr = 0x0900 + (0x65 * 2);
        out_addr = 0x0900 + (0x63 * 2);
        bit_num = 0x00;
        break;
    case 1:
        //set RFAGC pin
        pin_addr = 0x0a00 + (0x22 * 2);
        in_addr = 0x0a00 + (0x22 * 2) + 1;
        out_addr = 0x0a00 + (0x22 * 2);
        bit_num = 0x01;
        break;
    default:
        DMD_DBG((" @MSB124X_DTV_GPIO_Control unknown pin selection\n"));
        return FALSE;
    }

    //set input/output mode
    status &= MSB124X_ReadReg(pMSB124X, pin_addr, &u8_read);
    u8_write = u8_read & ~(1 << bit_num);
    u8_write = u8_write | (Ctrl_Para << bit_num);
    status &= MSB124X_WriteReg(pMSB124X, pin_addr, u8_write);

    //if RFAGC pin, set different bit_num
    if (Pin_sel == 1)
        bit_num = 0x00;

    //read/write data
    if (Ctrl_Para == 0)
    {
        //write output data
        status &= MSB124X_ReadReg(pMSB124X, out_addr, &u8_read);
        u8_write = u8_read & ~(1 << bit_num);
        u8_write = u8_write | (*pData_Para << bit_num);
        status &= MSB124X_WriteReg(pMSB124X, out_addr, u8_write);
    }
    else
    {
        //read input data
        status &= MSB124X_ReadReg(pMSB124X, in_addr, &u8_read);
        *pData_Para = u8_read & (1 << bit_num);
        *pData_Para = *pData_Para >> bit_num;
    }

    if (status)
    {
        DMD_DBG((" @MSB124X_DTV_GPIO_Control OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_ERR((" @MSB124X_DTV_GPIO_Control NG\n"));
        return FALSE;
    }
}


/********************************************
Function: To get chip version
Parameter: pu8Chip   0x01->MSB1230, 0x02->MSB124X,
                    0xff ->unknown
*********************************************/
//MS_BOOL MSB124X_Demod_GetChip(MS_U8 *pu8Chip)
//{
//    MSB124X_I2C_CH_Reset(3);
//    *pu8Chip = MSB124X_DTV_GetHWVer();
//    return TRUE;
//}
MS_BOOL MSB124X_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    MS_BOOL bret = TRUE;
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        bret &= MSB124X_I2C_CH_Reset(u8DemodIndex,3);
        bret &= _MSB124X_SetReg(u8DemodIndex,0x0951, 0x00);
        bret &= _MSB124X_SetReg(u8DemodIndex,0x090C, 0x10);
        bret &= _MSB124X_SetReg(u8DemodIndex,0x090E, 0x10);
        if(bOn)
        {
            bret &= _MSB124X_SetReg(u8DemodIndex,0x0910, 0x10);
            DMD_DBG(("set MSB124X I2C bypass ON\n"));
        }
        else
        {
            bret &= _MSB124X_SetReg(u8DemodIndex,0x0910, 0x00);
            DMD_DBG(("set MSB124X I2C bypass OFF\n"));
        }
        return bret;
    }
    //if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    //{
    //   DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
    //   return FALSE;
    //}

    MSB124X_IIC_Bypass_Mode(u8DemodIndex, bOn);

    //MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}

MS_BOOL MSB124X_Demod_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8  type)
{
    MS_BOOL bRet = TRUE;
    if (E_DEMOD_TYPE_T == type)
    {
        bRet &= MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_T);
    }
    else if(E_DEMOD_TYPE_T2 == type)
    {
        bRet &= MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_T2);
    }
#if MS_DVBS_INUSE
    else if(E_DEMOD_TYPE_S == type)
    {
        bRet &= MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_S);
        bRet &= MSB124X_DiSEqC_Init(u8DemodIndex);
    }
#endif
    else if(E_DEMOD_TYPE_C == type)
    {
        bRet &= MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_C);
    }
    else
        bRet =  FALSE;

    return bRet;
}

MS_U8 MSB124X_Demod_GetCurrentDemodType(MS_U8 u8DemodIndex)
{
    if (E_DEVICE_DEMOD_DVB_T == MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        return E_DEMOD_TYPE_T;
    }
    else if (E_DEVICE_DEMOD_DVB_T2 == MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        return E_DEMOD_TYPE_T2;
    }
    else if (E_DEVICE_DEMOD_DVB_S == MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        return E_DEMOD_TYPE_S;
    }
    else if (E_DEVICE_DEMOD_DVB_C == MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        return E_DEMOD_TYPE_C;
    }
    else
        return E_DEMOD_TYPE_T;
}

MS_BOOL MSB124X_Demod_Open(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB124X->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);

    return TRUE;
}

MS_BOOL MSB124X_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MSB124X_Demod_Close(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    pMSB124X->bOpen = FALSE;
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);

    return TRUE;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB124X_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pfSNR = 0;
        if (E_DEMOD_LOCK == MSB124X_DTV_GetLockStatus(u8DemodIndex))
        {
            MSB124X_DTV_GetSNR_F(u8DemodIndex,pfSNR);
        }
    }
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);

    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB124X_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL status = TRUE;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetBER:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    //*pfBER = (float)MSB124X_DTV_GetSignalQuality();
    status = MSB124X_DTV_GetPostBER(u8DemodIndex,pfBER);

    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);

    return status;
}

MS_BOOL MSB124X_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    float fSignal = 0;

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *ps32Signal = 0;
        if (E_DEMOD_LOCK == MSB124X_DTV_GetLockStatus(u8DemodIndex))
        {
            if(TUNER_MXL603 == pMSB124X->MSB124X_InitParam.pstTunertab->data)
            {
                MSB124X_IIC_Bypass_Mode(u8DemodIndex, TRUE);
                if (TRUE != pMSB124X->MSB124X_InitParam.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, &fSignal))
                {
                    DMD_ERR(("MxLWare603_API_ReqTunerRxPower failed !!!\n"));
                    MSB124X_IIC_Bypass_Mode(u8DemodIndex, FALSE);
                    return FALSE;
                }
                MSB124X_IIC_Bypass_Mode(u8DemodIndex, FALSE);
                *ps32Signal = (MS_U32)MSB124X_DTV_GetSignalStrengthWithRFPower(u8DemodIndex,fSignal);
            }
            else if(TUNER_MXL608 == pMSB124X->MSB124X_InitParam.pstTunertab->data)
            {

                MSB124X_IIC_Bypass_Mode(u8DemodIndex, TRUE);
                if (TRUE != pMSB124X->MSB124X_InitParam.pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, &fSignal))
                {
                    DMD_ERR(("MxLWare608_API_ReqTunerRxPower failed !!!\n"));
                    MSB124X_IIC_Bypass_Mode(u8DemodIndex, FALSE);
                    return FALSE;
                }

                MSB124X_IIC_Bypass_Mode(u8DemodIndex, FALSE);
                *ps32Signal = (MS_U32)MSB124X_DTV_GetSignalStrengthWithRFPower(u8DemodIndex,fSignal);
            }
            else
                *ps32Signal  = (MS_U32)MSB124X_DTV_GetSignalStrength(u8DemodIndex);
        }

        if (*ps32Signal > 100)
        {
            *ps32Signal = 100;
        }
        else if (*ps32Signal < 0)
        {
            *ps32Signal = 0;
        }
    }

    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}

MS_BOOL MSB124X_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pu16quality = 0;
        if (E_DEMOD_LOCK == MSB124X_DTV_GetLockStatus(u8DemodIndex))
        {
            *pu16quality  = MSB124X_DTV_GetSignalQuality(u8DemodIndex);
        }

    }
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);

    return TRUE;
}


MS_BOOL MSB124X_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    *peLockStatus = E_DEMOD_CHECKING;

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *peLockStatus = MSB124X_DTV_GetLockStatus(u8DemodIndex);
    }

    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}


MS_BOOL MSB124X_DVBT_T2_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    MS_U32              dwError = TRUE;
    MS_U8               BandWidth = DEMOD_BW_MODE_8MHZ;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    //MCP demod select IF input to dvbs_I
    if(pMSB124X->bIsMCP_DMD)
    {
        MSB124X_WriteReg2bytes(pMSB124X,0x0A00+(0x51)*2, 0x0021);
    }

    pMSB124X->enTuneBw = pParam->TerParam.eBandWidth;

    switch (pParam->TerParam.eBandWidth)
    {
    case DEMOD_BW_MODE_6MHZ:
        BandWidth = 6;
        break;
    case DEMOD_BW_MODE_7MHZ:
        BandWidth = 7;
        break;
    case DEMOD_BW_MODE_1_7MHZ: // 1.7MHz
        BandWidth = 17;
        break;
    case DEMOD_BW_MODE_8MHZ:
    default:
        BandWidth = 8;
        break;
    }

    dwError = MSB124X_DTV_SetFrequency(u8DemodIndex, pParam, BandWidth);
    pMSB124X->u32CurrFreq = pParam->u32Frequency;
    pMSB124X->enCurrBW = pParam->TerParam.eBandWidth;
    pMSB124X->u8CurrPlpID = pParam->TerParam.u8PlpID;
    pMSB124X->u8CurrScanType = pParam->TerParam.u8ScanType;

    if (E_DEMOD_TYPE_T2 == MSB124X_Demod_GetCurrentDemodType(u8DemodIndex))
    {
        dwError &= MSB124X_T2_SetPlpID(u8DemodIndex,pParam->TerParam.u8PlpID);
    }
    DMD_DBG((" ==return %d==\n", (int)dwError));
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return dwError;
}

#if MS_DVBS_INUSE
MS_BOOL MSB124X_DVBS_S2_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16SymbolRate;
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    MS_U16 u16CenterFreq;
#endif

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }


    u16SymbolRate=(pParam->SatParam.u32SymbolRate/1000);
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    u16CenterFreq=pParam->u32Frequency;
#endif
    DMD_DBG(("MSB124X MDrv_CofdmDmd_Restart+ Fc:%d SymbolRate %d\n", u16CenterFreq, u16SymbolRate));
    MSB124X_IIC_Bypass_Mode(u8DemodIndex, TRUE);
    bRet&= MSB124X_DVBS_SetFrequency(u8DemodIndex, pParam->u32Frequency,(pParam->SatParam.u32SymbolRate/1000));
    MSB124X_IIC_Bypass_Mode(u8DemodIndex, FALSE);

    if(E_DEVICE_DEMOD_DVB_S != MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        bRet &= MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_S);
        bRet &= MSB124X_DiSEqC_Init(u8DemodIndex);
    }

    u16Address=0x0990;
    bRet&=MSB124X_ReadReg( pMSB124X, u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB124X_WriteReg(pMSB124X,u16Address,u8Data);
    MsOS_DelayTask(50);

    u16Address=0x0B52;//DIG_DBG_2
    u8Data=(u16SymbolRate&0xFF);
    bRet&=MSB124X_WriteReg(pMSB124X,u16Address,u8Data);
    u16Address=0x0B53;//DIG_DBG_3
    u8Data=((u16SymbolRate>>8)&0xFF);
    bRet&=MSB124X_WriteReg(pMSB124X,u16Address,u8Data);
    //ADCPLL IQ swap
    //ADCPLL IQ swap
    u16Address= 0x0A03;
    bRet&= MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if(MSB124X_ADCPLL_IQ_SWAP==1)
        u8Data |= (0x10);
    else
        u8Data &= (0xEF);
    bRet&= MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    //TS serial mode
    u16Address= 0x2A40;
    bRet&= MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if(SERIAL_TS==1)
        u8Data |= (0x01);
    else
        u8Data &= (0xFE);
    bRet&= MSB124X_WriteReg(pMSB124X, u16Address, u8Data);


    u16Address=0x0990;
    bRet&=MSB124X_ReadReg( pMSB124X, u16Address, &u8Data);
    u8Data&=0xF0;
    u8Data|=0x01;
    bRet&=MSB124X_WriteReg(pMSB124X,u16Address,u8Data);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}
#endif

MS_BOOL MSB124X_Set_Config_dvbc_auto (MS_U8 u8DemodIndex, MS_U8 bAutoDetect)
{
    DMD_DBG(("%s(),%d\n",__FUNCTION__,__LINE__));
    MS_U8 status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if( MSB124X_WriteDspReg( pMSB124X, C_opmode_auto_scan_sym_rate, bAutoDetect)!=TRUE)
    {
        DMD_ERR((" @MSB1240_Set_Config_dvbc_auto NG 1\n"));
    }
    if( MSB124X_WriteDspReg( pMSB124X, C_opmode_auto_scan_qam, bAutoDetect)!=TRUE)
    {
        DMD_ERR((" @MSB1240_Set_Config_dvbc_auto NG 2\n"));
    }

    if(bAutoDetect)
    {
        status &= MSB124X_WriteDspReg( pMSB124X, C_config_bw_l, C_BW_L);
        status &= MSB124X_WriteDspReg( pMSB124X, C_config_bw_h, C_BW_H);
    }
    else
    {
        status &= MSB124X_WriteDspReg( pMSB124X, C_config_bw_l, C_BW1_L);
        status &= MSB124X_WriteDspReg( pMSB124X, C_config_bw_h, C_BW1_L);
    }

    if(status)
    {
        DMD_DBG((" @MSB1240_Set_Config_dvbc_auto OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_ERR((" @MSB1240_Set_Config_dvbc_auto NG 3\n"));
        return FALSE;
    }

}

MS_BOOL MSB124X_DVBC_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
    MS_BOOL status=TRUE;
    MS_U8  reg_symrate_l = 0;
    MS_U8  reg_symrate_h = 0;
    MS_U32 u32DMD_IfFreq = 0;
    DEMOD_EN_CAB_CONSTEL_TYPE             eConstellation;
    //MS_U32 u32Frequency;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if(E_DEVICE_DEMOD_DVB_C != MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
        status &= MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_C);

    //MCP demod select IF input to dvbs_I
    if(pMSB124X->bIsMCP_DMD)
    {
        status &= MSB124X_WriteReg2bytes(pMSB124X,0x0A00+(0x51)*2, 0x0021);
    }

    //u32Frequency = pParam->u32Frequency;
    reg_symrate_l = (MS_U8) (pParam->CabParam.u16SymbolRate & 0xff);
    reg_symrate_h = (MS_U8) (pParam->CabParam.u16SymbolRate >> 8);

    MSB124X_IIC_Bypass_Mode(u8DemodIndex, TRUE);
    if(!pMSB124X->MSB124X_InitParam.pstTunertab->GetTunerIF(u8DemodIndex, &u32DMD_IfFreq))
    {
        u32DMD_IfFreq = 5000;
    }

    MSB124X_IIC_Bypass_Mode(u8DemodIndex, FALSE);
    MSB124X_Reset(u8DemodIndex);
    MsOS_DelayTask(50);
    //// DSP Register Overwrite ///////////////////
    status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_fc_l, (MS_U8)u32DMD_IfFreq);
    status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_fc_h, (MS_U8)(u32DMD_IfFreq>>8));

    switch (pParam->CabParam.eConstellation)
    {
    case DEMOD_CAB_QAM16:
        eConstellation = DEMOD_CAB_QAM16;
        break;
    case DEMOD_CAB_QAM32:
        eConstellation = DEMOD_CAB_QAM32;
        break;
    case DEMOD_CAB_QAM64:
        eConstellation = DEMOD_CAB_QAM64;
        break;
    case DEMOD_CAB_QAM128:
        eConstellation = DEMOD_CAB_QAM128;
        break;
    case DEMOD_CAB_QAM256:
        eConstellation = DEMOD_CAB_QAM256;
        break;
    default:
        eConstellation = DEMOD_CAB_QAMAUTO;
        break;
    }

    if (eConstellation == DEMOD_CAB_QAMAUTO)//Auto SR QAM Mode
    {
        MSB124X_Set_Config_dvbc_auto(u8DemodIndex, TRUE);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_opmode_auto_scan_sym_rate, 1);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_opmode_auto_scan_qam, 1);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_bw_l, C_BW_L);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_bw_h, C_BW_H);
    }
    else //Manual Mode
    {
        MSB124X_Set_Config_dvbc_auto(u8DemodIndex, FALSE);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_opmode_auto_scan_sym_rate, 0);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_opmode_auto_scan_qam, 0);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_bw_l, reg_symrate_l);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_bw_h, reg_symrate_h);
        status &= MSB124X_WriteDspReg( pMSB124X, (MS_U16)C_config_qam, eConstellation);
    }

    // for MCP demod, set AGC to DVBS AGC pad, otherwise set AGC to DVBT AGC pad
    if(pMSB124X->bIsMCP_DMD)
    {
        MSB124X_WriteReg2bytes(pMSB124X, 0x0A00+(0x18)*2, 0x0100);
    }
    else
    {
        MSB124X_WriteReg2bytes(pMSB124X, 0x0A00+(0x18)*2, 0x0001);
    }


    status &= MSB124X_Active(u8DemodIndex,TRUE);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return status;
}

/****************************************************************************
**function:reset bw and Plpid to demod when plp changed
**
**
****************************************************************************/
MS_BOOL MSB124X_Demod_T2MI_Restart(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    MS_U8    PlpId, BandWidth;

    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if (E_DEVICE_DEMOD_DVB_T2 != MSB124X_GetCurrentDemodulatorType(u8DemodIndex))
    {
        MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
        return TRUE;
    }

    MSB124X_ReadDspReg(pMSB124X, (MS_U16)E_T2_PLP_ID, &PlpId);
    if (pMSB124X->u8CurrPlpID != PlpId)
    {
        switch (pMSB124X->enCurrBW)
        {
        case DEMOD_BW_MODE_6MHZ:
            BandWidth = E_DEMOD_BW_6M;
            break;
        case DEMOD_BW_MODE_7MHZ:
            BandWidth = E_DEMOD_BW_7M;
            break;
        case DEMOD_BW_MODE_1_7MHZ: // 1.7MHz
            BandWidth = E_DEMOD_BW_17M;
            break;
        case DEMOD_BW_MODE_8MHZ:
        default:
            BandWidth = E_DEMOD_BW_8M;
            break;
        }
        DMD_DBG(("Curr_PLP_ID=%d, GET_PLP_ID=%d\n", pMSB124X->u8CurrPlpID, PlpId));
        MSB124X_WriteDspReg(pMSB124X, (MS_U16)E_T2_BW, BandWidth);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB124X_WriteDspReg(pMSB124X, (MS_U16)E_T2_PLP_ID, pMSB124X->u8CurrPlpID);
        MSB124X_WriteDspReg(pMSB124X, (MS_U16)E_T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL);

    }

    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return TRUE;
}


MS_BOOL MSB124X_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
    MS_BOOL ret = FALSE;
#if MSB124X_NO_CHANNEL_CHECK
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
#endif

    if(u32BroadCastType == DVBC)
    {
        ret =  MSB124X_DVBC_Demod_Restart(u8DemodIndex, pParam);
    }
#if MS_DVBS_INUSE
    else if(u32BroadCastType == DVBS)
    {
        ret =  MSB124X_DVBS_S2_Demod_Restart(u8DemodIndex, pParam);
    }
#endif
    else
    {
        if((u32BroadCastType == DVBT) && (E_DEMOD_TYPE_T != MSB124X_Demod_GetCurrentDemodType(u8DemodIndex)))
        {
            MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_T);
        }
        else if((u32BroadCastType == DVBT2) && (E_DEMOD_TYPE_T2 != MSB124X_Demod_GetCurrentDemodType(u8DemodIndex)))
        {
            MSB124X_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_T2);
        }
        ret =  MSB124X_DVBT_T2_Demod_Restart(u8DemodIndex, pParam);
    }
#if MSB124X_NO_CHANNEL_CHECK
    pMSB124X->noChannelStable = FALSE;
    pMSB124X->t2NoChannelFlag = FALSE;
    pMSB124X->tNoChannelFlag = FALSE;
    pMSB124X->u32LockTime = MsOS_GetSystemTime();
#endif

    return ret;
}

void MSB124X_Demod_CtrlResetDJBFlag(MS_U8 u8DemodIndex,MS_U8 Ctrl)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (pMSB124X->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {

    }
}

MS_U8 MSB124X_Demod_GetPlpIDList(MS_U8 u8DemodIndex, MS_U8* u8PlpID)
{
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if (0 == pMSB124X->PlpIDSize)
    {
        if (FALSE == MSB124X_GetPlpIDList(u8DemodIndex))
            pMSB124X->PlpIDSize = 0;
    }
    memcpy(u8PlpID , pMSB124X->PlpIDList, pMSB124X->PlpIDSize);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);

    *u8PlpID = pMSB124X->PlpIDSize;
    return pMSB124X->PlpIDSize;
}

MS_BOOL MSB124X_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    MS_U8     status = TRUE;
    sDMD_MSB124X_InitData sMSB124X_InitData;
    HWI2C_PORT hwi2c_port;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);


    if(NULL == pParam)
        return FALSE;

    pMSB124X->MSB124X_InitParam.pstTunertab = pParam->pstTunertab;

#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    DMD_DBG(("MSPI path is set by detected result\n"));
#else
    pMSB124X->u32DmxInputPath = pParam->u32DmxInputPath;
#endif
#endif

    if (pMSB124X->bInited)
        return TRUE;

#if defined(SUPPORT_MULTI_DEMOD) && (SUPPORT_MULTI_DEMOD == 1)
    if (pMSB124X->s32DemodHandle == -1)
    {
        MDrv_MDM_MSB124X_CreateNode(&pMSB124X->s32DemodHandle);
    }

    if (pMSB124X->s32DemodHandle==-1)
    {
        DMD_ERR(("MDrv_MDM_MSB124X_CreateNode failed!!!\n"));
        return FALSE;
    }
#endif

    if(MSB124X_SUPPORT_DVBT & pMSB124X->u8BondOpt)
    {
        pMSB124X->enDemodType = E_DEVICE_DEMOD_DVB_T;
        pMSB124X->u8CurrScanType = E_DEMOD_TYPE_T;
    }
    else if((MSB124X_SUPPORT_DVBS | MSB124X_SUPPORT_DVBS2) & pMSB124X->u8BondOpt)
    {
        pMSB124X->enDemodType = E_DEVICE_DEMOD_DVB_S;
        pMSB124X->u8CurrScanType = E_DEMOD_TYPE_S;
    }
    else if(!(MSB124X_NOT_SUPPORT_DVBT2 & pMSB124X->u8BondOpt))
    {
        pMSB124X->enDemodType = E_DEVICE_DEMOD_DVB_T2;
        pMSB124X->u8CurrScanType = E_DEMOD_TYPE_T2;
    }
    else
    {
        pMSB124X->enDemodType = E_DEVICE_DEMOD_DVB_C;
        pMSB124X->u8CurrScanType = E_DEMOD_TYPE_C;
    }

    _msb124x_hw_reset(u8DemodIndex);
    pMSB124X->bInited = TRUE;
    pMSB124X->bOpen = TRUE;
    pMSB124X->u8sramCode = 0;
    pMSB124X->PlpIDSize = 0;
    pMSB124X->bFECLock = FALSE;
    pMSB124X->u8ScanStatus = 0;
    pMSB124X->u32CurrFreq = 0;
    pMSB124X->enCurrBW = DEMOD_BW_MODE_8MHZ;
    pMSB124X->enTuneBw = DEMOD_BW_MODE_8MHZ;
    pMSB124X->u8CurrPlpID = 0xff;
    pMSB124X->pMSB124X_RegDVBT = MSB124x_DVBT_DSPREG_TABLE ;

    pMSB124X->s32_MSB124X_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2", MSOS_PROCESS_SHARED);
    if (pMSB124X->s32_MSB124X_Mutex < 0)
    {
        pMSB124X->bInited = FALSE;
        pMSB124X->bOpen = FALSE;
        DMD_ERR(("creat MSB124X Mutex error!!!\n"));
        return FALSE;
    }

    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        sMSB124X_InitData.fpMSB124X_I2C_Access = msb124x_I2C_Access;
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        sMSB124X_InitData.fpMSB124X_I2C_Access = msb124x_I2C1_Access;
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    sMSB124X_InitData.u8WO_SPI_Flash = TRUE;
    sMSB124X_InitData.bPreloadDSPCodeFromMainCHIPI2C = FALSE;
    sMSB124X_InitData.bFlashWPEnable = TRUE;
    sMSB124X_InitData.fpGPIOReset = NULL;
    sMSB124X_InitData.pDVBC_DSP_REG = MSB124X_DVBC_DSPREG_TABLE;
    sMSB124X_InitData.pDVBT_DSP_REG = MSB124x_DVBT_DSPREG_TABLE;
    sMSB124X_InitData.pDVBT2_DSP_REG = MSB124x_DVBT2_DSPREG_TABLE;
    sMSB124X_InitData.u8WO_Sdram = pMSB124X->bWO_SDRAM;

#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    if(MSPI_PATH_NONE != pMSB124X->u32DmxInputPath)
    {
        sMSB124X_InitData.bEnableSPILoadCode = TRUE;
        sMSB124X_InitData.fpMSB124x_SPIPAD_En = SPIPAD_EN[pMSB124X->u32DmxInputPath];
    }
    else
#else
    if(DMX_INPUT_EXT_INPUT0 == pMSB124X->u32DmxInputPath)
    {
        sMSB124X_InitData.bEnableSPILoadCode = TRUE;
        sMSB124X_InitData.fpMSB124x_SPIPAD_En = msb124x_SPIPAD_TS0_En;
    }
    else if(DMX_INPUT_EXT_INPUT1 == pMSB124X->u32DmxInputPath)
    {
        sMSB124X_InitData.bEnableSPILoadCode = TRUE;
        sMSB124X_InitData.fpMSB124x_SPIPAD_En = msb124x_SPIPAD_TS1_En;
    }
    else
#endif
#endif
    {
        sMSB124X_InitData.bEnableSPILoadCode = FALSE;
        sMSB124X_InitData.fpMSB124x_SPIPAD_En = NULL;
    }

    status &= MDrv_DMD_MSB124X_Init_EX(pMSB124X->s32DemodHandle, &sMSB124X_InitData, sizeof(sDMD_MSB124X_InitData));

    if (status)
    {
        switch(pMSB124X->enDemodType)
        {
        case E_DEVICE_DEMOD_DVB_T:
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle,E_DMD_MSB124X_DEMOD_DVBT);
            break;
        case E_DEVICE_DEMOD_DVB_T2:
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle,E_DMD_MSB124X_DEMOD_DVBT2);
            break;
        case E_DEVICE_DEMOD_DVB_C:
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle,E_DMD_MSB124X_DEMOD_DVBC);
            break;
        case E_DEVICE_DEMOD_DVB_S:
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle,E_DMD_MSB124X_DEMOD_DVBS2);
            break;
        default:
            MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(pMSB124X->s32DemodHandle,E_DMD_MSB124X_DEMOD_DVBT);
            break;

        }
        status &= MDrv_DMD_MSB124X_Power_On_Initialization_EX(pMSB124X->s32DemodHandle);

    }

    if (!status)
    {
        pMSB124X->bInited = FALSE;
        pMSB124X->bOpen = FALSE;
        MsOS_DeleteMutex(pMSB124X->s32_MSB124X_Mutex);
        return FALSE;
    }
    MSB124X_Show_Version(u8DemodIndex);
    MSB124X_Show_BondingOption(u8DemodIndex);
#if TS_PARALLEL_OUTPUT
    MSB124X_DTV_Serial_Control(u8DemodIndex,FALSE);
#else
    MSB124X_DTV_Serial_Control(u8DemodIndex,TRUE);
#endif
#if MS_DVBS_INUSE
    if(E_DEVICE_DEMOD_DVB_S == pMSB124X->enDemodType)
        MSB124X_DiSEqC_Init(u8DemodIndex);
#endif
    return TRUE;
}

static MS_BOOL _MSB124X_GetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    HWI2C_PORT hwi2c_port;
    mapi_i2c *iptr;

    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        iptr = mapi_i2c1_GetI2C_Dev(MSB124X_DEMOD_IIC);
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 5, u8MsbData);
    bRet &=iptr->ReadBytes(u8DemodIndex, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    return bRet;
}

static MS_BOOL _MSB124X_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    HWI2C_PORT hwi2c_port;
    mapi_i2c *iptr;

    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        iptr = mapi_i2c_GetI2C_Dev(MSB124X_DEMOD_IIC);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        iptr = mapi_i2c1_GetI2C_Dev(MSB124X_DEMOD_IIC);
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);
    return bRet;
}

#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
static MS_BOOL _MSB124X_SetReg2Bytes(MS_U8 u8DemodIndex,MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    bRet &= _MSB124X_SetReg(u8DemodIndex, u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= _MSB124X_SetReg(u8DemodIndex, u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}
#endif
#endif

//MS_U16 MSB124X_Demod_ReadReg(MS_U16 RegAddr)
//{
// MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
//MS_U8 RegData = 0;

//if (!pMSB124X)
//{
//    DMD_ERR(("pMSB124X error !\n"));
//    return FALSE;
//}

// MSB124X_ReadReg(RegAddr, &RegData);

//return (MS_U16) RegData;
//}

//MS_BOOL MSB124X_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
//{
//MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

//if (!pMSB124X)
//{
//    DMD_ERR(("pMSB124X error !\n"));
//    return FALSE;
//}

// return MSB124X_WriteReg(RegAddr, RegData);
//}

#define MSB124X_CHIP_ID 0x6f
MS_BOOL MSB124X_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;
    MS_U8 i, u8I2C_Port = 0;
    MDvr_CofdmDmd_CONFIG *pMSB124X;
    SLAVE_ID_TBL *pMSB124X_ID_TBL;
    HWI2C_PORT hwi2c_port;

    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        u8I2C_Port = 0;
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        u8I2C_Port = 1;
    }


    if(!MSB124X_Variables_alloc())
    {
        MSB124X_Variables_free();
        return FALSE;
    }
    else
    {
        pMSB124X = (pstMSB124X + u8DemodIndex);
        pMSB124X_ID_TBL = (pstMSB124X_slave_ID_TBL + u8I2C_Port);
    }

    for(i=0 ; i<(sizeof(pMSB124X_ID_TBL->stID_Tbl)/sizeof(SLAVE_ID_USAGE)); i++)
    {
        DMD_DBG(("### %x\n",i));
        if(pMSB124X_ID_TBL->stID_Tbl[i].u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSB124X] All Slave ID have tried but not detect\n"));
            return FALSE;
        }

        if(pMSB124X_ID_TBL->stID_Tbl[i].bInUse)
        {
            DMD_DBG(("[MSB124X] Slave ID 0x%x have been used\n", pMSB124X_ID_TBL->stID_Tbl[i].u8SlaveID));
            continue;
        }
        else
        {
            pMSB124X->u8SlaveID = pMSB124X_ID_TBL->stID_Tbl[i].u8SlaveID;
            DMD_DBG(("[MSB124X] Try slave ID 0x%x\n",pMSB124X_ID_TBL->stID_Tbl[i].u8SlaveID));
        }

        _msb124x_hw_reset(u8DemodIndex);
        if(!MSB124X_I2C_CH_Reset(u8DemodIndex,3))
        {
            DMD_ERR(("[MSB124X] I2C_CH_Reset fail \n"));
        }
        else
        {
            DMD_DBG(("[MSB124X] I2C slave id :%x \n",pMSB124X->u8SlaveID ));
            if(_MSB124X_GetReg(u8DemodIndex,0x0900,&u8_tmp))
            {
                DMD_DBG(("[MSB124X] read id :%x \n",u8_tmp ));
                if(u8_tmp == MSB124X_CHIP_ID)
                {
                    pMSB124X_ID_TBL->stID_Tbl[i].bInUse = TRUE;
                    break;
                }
            }
        }
    }


    if(u8_tmp == MSB124X_CHIP_ID)
    {
        u8MSB124X_DevCnt++;

        // Check 124x is MCP or EXT
        _MSB124X_GetReg(u8DemodIndex,0x0905,&u8_tmp);
        if((u8_tmp>>2) & 0x01)
        {
            DMD_DBG(("[MSB1245] This is External DMD \n"));
            pMSB124X->bIsMCP_DMD = FALSE;
        }
        else
        {
            DMD_DBG(("[MSB1245] This is MCP DMD \n"));
            pMSB124X->bIsMCP_DMD = TRUE;
        }

        // Check 124x bonding option
        _MSB124X_GetReg(u8DemodIndex,0x0900+(0x02*2), &u8_tmp);
        pMSB124X->u8BondOpt= u8_tmp;
        if(pMSB124X->u8BondOpt & MSB124X_NOT_SUPPORT_DVBT2) //Not Support DVBT2
        {
            DMD_DBG(("[MSB124X] NOT support DVBT2, might be without sdram \n"));
            pMSB124X->bWO_SDRAM= TRUE;
        }
        else
        {
            DMD_DBG(("[MSB124X] sdram exist\n"));
            pMSB124X->bWO_SDRAM = FALSE;
        }
        *pu8SlaveID = pMSB124X->u8SlaveID;

#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
        //check TS path
        MDrv_DMD_SSPI_Init(0);
        MDrv_MasterSPI_CsPadConfig(0, 0xff);
        MDrv_MasterSPI_MaxClkConfig(0, 54);
        _MSB124X_SetReg2Bytes(u8DemodIndex,0x0900+(0x28)*2, 0x0000);
        _MSB124X_SetReg2Bytes(u8DemodIndex,0x0900+(0x2d)*2, 0x00ff);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MSB124X_SetReg2Bytes(u8DemodIndex,0x0900+(0x3b)*2, 0x0002);

        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        _MSB124X_GetReg(u8DemodIndex,0x0900+(0x3a)*2+1,&u8_tmp);
        u8_tmp &= 0xFC;
        _MSB124X_SetReg(u8DemodIndex,0x0900+(0x3a)*2+1, u8_tmp);

        // ------- MSPI driving setting
        _MSB124X_SetReg2Bytes(u8DemodIndex,0x0900+(0x2c)*2, 0x07ff);

        for(i=0; i<MSPI_PATH_MAX; i++)
        {
            SPIPAD_EN[i](TRUE);    
            // Note: Must read twice and ignore 1st data because pad switch noise
            // might cause wrong MSPI signal
            MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
            MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
            if(u8_tmp == MSB124X_CHIP_ID)
            {
                pMSB124X->u32DmxInputPath = (MS_U32)i;
                SPIPAD_EN[i](FALSE);                    
                DMD_DBG(("Get MSB124X chip ID by MSPI on TS%lx\n", pMSB124X->u32DmxInputPath));
                break;
            }
            else
            {
                DMD_DBG(("Cannot get MSB124X chip ID by MSPI on TS%x\n", i));
                if( i == (MSPI_PATH_MAX - 1))
                    pMSB124X->u32DmxInputPath = MSPI_PATH_NONE;
            }
            SPIPAD_EN[i](FALSE); 
        }

        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MSB124X_SetReg2Bytes(u8DemodIndex, 0x0900 + (0x3b) * 2, 0x0001);
#endif //MSPI_PATH_DETECT
#endif
        return TRUE;
    }
    else
    {
        if(!u8MSB124X_DevCnt)
            MSB124X_Variables_free();
        return FALSE;
    }
}

MS_BOOL MSB124X_Demod_GetRollOff(MS_U8 u8DemodIndex, MS_U8 *pRollOff)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT)==FALSE)
    {
        DMD_ERR(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    u16Address=0x1B1E;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if ((u8Data&0x03)==0x00)
        *pRollOff = 0;  //Rolloff 0.35
    else if (((u8Data&0x03)==0x01) || ((u8Data&0x03)==0x03))
        *pRollOff = 1;  //Rolloff 0.25
    else
        *pRollOff = 2;  //Rolloff 0.20
    DMD_DBG(("MSB124X_Demod_GetRollOff:%d\n", *pRollOff));
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}

MS_BOOL MSB124X_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    switch(fuction_type)
    {
    case DEMOD_EXT_FUNC_OPEN:
        bret &= MSB124X_Demod_Open(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_CLOSE:
        bret &= MSB124X_Demod_Close(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_FINALIZE:
        if(pMSB124X->s32_MSB124X_Mutex >= 0)
        {
            bret &= MsOS_DeleteMutex(pMSB124X->s32_MSB124X_Mutex);
            pMSB124X->s32_MSB124X_Mutex = -1;
        }
        _msb124x_hw_reset(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_GET_ROLL_OFF:
        bret &= MSB124X_Demod_GetRollOff(u8DemodIndex, (MS_U8 *)data);
        break;
#ifdef MS_DVBT2_INUSE
    case DEMOD_EXT_FUNC_CTRL_RESET_DJB_FLAG:
        MSB124X_Demod_CtrlResetDJBFlag(u8DemodIndex, *(MS_BOOL *)data);
        break;
    case DEMOD_EXT_FUNC_T2MI_RESTART:
        bret &= MSB124X_Demod_T2MI_Restart(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_INIT_PARAMETER:
        bret &= MSB124X_Demod_InitParameter(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_GET_PLPID_LIST:
        bret &= MSB124X_Demod_GetPlpIDList(u8DemodIndex, (MS_U8 *)data);
        break;
#endif
    case DEMOD_EXT_FUNC_GET_PACKGE_INFO:
        *(MS_BOOL *)(data) = pMSB124X->bIsMCP_DMD;
        break;

    default:
        DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
        break;
    }
    return bret;
}
#if MS_DVBS_INUSE
MS_BOOL MSB124X_DiSEqC_Init(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    u16Address=0x0DCD; //Clear Status
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8Data=(u8Data&(~0x2E))|(0x2E);
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x00;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    //u16Address=0x0DC0;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8Data=(u8Data&(~0x02))|(0x02);
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    if(pMSB124X->bIsMCP_DMD)
    {
        //Set MSPI_DO pin as DiSeqc_out, tone mode
        bRet&=MSB124X_WriteReg(pMSB124X, 0x9D6, 0x80);
        bRet&=MSB124X_WriteReg(pMSB124X, 0x9D7, 0x01);
        bRet&=MSB124X_ReadReg(pMSB124X, 0x976, &u8Data);
        u8Data=(u8Data&(~0x01));
        bRet&=MSB124X_WriteReg(pMSB124X, 0x976, u8Data);
        //Disable DiSeqc tone mode, depend on LNB control IC
        bRet&=MSB124X_ReadReg(pMSB124X, 0xDD7, &u8Data);
        u8Data|= 0x80;
        bRet&=MSB124X_WriteReg(pMSB124X, 0xDD7, u8Data);
    }
    return bRet;
}

MS_BOOL MSB124X_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex,MS_BOOL bLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if (bLow==TRUE)
    {
        u8Data=(u8Data|0x40);
    }
    else
    {
        u8Data=(u8Data&(~0x40));
    }
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}
MS_BOOL MSB124X_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex,MS_BOOL* bLNBOutLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }


    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if( (u8Data&0x40)==0x40)
    {
        * bLNBOutLow=TRUE;
    }
    else
    {
        * bLNBOutLow=FALSE;
    }
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}
MS_BOOL MSB124X_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex,MS_BOOL b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if (b22kOn==TRUE)
    {
        u8Data=(u8Data|0x08);
    }
    else
    {
        u8Data=(u8Data&(~0x08));
    }
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}
MS_BOOL MSB124X_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex,MS_BOOL* b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    if ((u8Data&0x08)==0x08)
    {
        *b22kOn=TRUE;
    }
    else
    {
        *b22kOn=FALSE;
    }
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}

MS_BOOL MSB124X_DiSEqC_SendCmd(MS_U8 u8DemodIndex,MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16 u16WaitCount;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8Data=(u8Data&~(0x10));
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    //u16Address=0x0BC4;
    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u16Address=0x0DC4+u8Index;
        u8Data=*(pCmd+u8Index);
        bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    }
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    //Tone and Burst switch,Mantis 0232220
    if(((*pCmd)==0xE0)&&((*(pCmd + 1))==0x10)&&((*(pCmd + 2))==0x38)&&((((*(pCmd + 3))&0x0C)==0x0C)||(((*(pCmd + 3))&0x04)==0x04)))
    {
        u8Data|=0x80;   //u8Data|=0x20;Tone Burst1
    }
    else if(((*pCmd)==0xE0)&&((*(pCmd + 1))==0x10)&&((*(pCmd + 2))==0x38))
    {
        u8Data|=0x20;   //u8Data|=0x80;ToneBurst0
    }

    u16Address=0x0B54;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

#if 1       //For Unicable command timing,mick
    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
        MsOS_DelayTask(1);
        u16WaitCount++;
    }
    while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB124X_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB124X_DEMOD_WAIT_TIMEOUT)
    {
        DMD_ERR(("MSB1240 DVBS DiSEqC Send Command Busy!!!\n"));
        return FALSE;
    }
#endif      //For Unicable command timing,mick

    DMD_DBG(("\r\n MDrv_DiSEqC_SendCmd----"));
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}

MS_BOOL MSB124X_DiSEqC_SetTone(MS_U8 u8DemodIndex,MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U8 u8ReSet22k;

    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);
    if (!pMSB124X)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB124X->bInited) || (!pMSB124X->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB124X->s32_MSB124X_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC4;
    u8Data=0x01;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x4E;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    u16Address=0x0DCC;
    u8Data=0x80;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    u16Address=0x0DC2;
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8ReSet22k=u8Data;

    if (bTone1==TRUE)
    {
        u8Data=0x19;
        bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
        // _u8ToneBurstFlag=1;
    }
    else
    {
        u8Data=0x11;
        bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
        //_u8ToneBurstFlag=2;
    }
    u16Address=0x0DCD;
    u8Data=u8Data&~(0x01);
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8Data=u8Data|0x3E;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    MsOS_DelayTask(10);
    bRet&=MSB124X_ReadReg(pMSB124X, u16Address, &u8Data);
    u8Data=u8Data&~(0x3E);
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    MsOS_DelayTask(1);
    u8Data=u8Data|0x01;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);
    MsOS_DelayTask(100);
    u16Address=0x0DC2;
    u8Data=u8ReSet22k;
    bRet&=MSB124X_WriteReg(pMSB124X, u16Address, u8Data);

    //MsOS_DelayTask(100);
    MsOS_ReleaseMutex(pMSB124X->s32_MSB124X_Mutex);
    return bRet;
}

#endif

MS_BOOL MSB124X_Demod_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    MS_U16 TPS_parameter=0;
    MS_U64 L1_Info=0;
    MDvr_CofdmDmd_CONFIG *pMSB124X = (pstMSB124X + u8DemodIndex);

    switch(pMSB124X->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T:
        bRet = MSB124X_DTV_DVB_T_GetSignalTpsInfo(u8DemodIndex, &TPS_parameter);
        if(bRet)
        {
            // TPS_parameter
            // Constellation (b2 ~ b0)
            // Hierarchy (b5 ~ b3)
            // LP Code Rate (b8 ~ b6)
            // HP Code Rate (b11 ~ b9)
            // GI (b13 ~ b12)
            // FFT ( b14)
            pParam->TerParam.eConstellation = TPS_parameter&0x3;
            pParam->TerParam.eHierarchy = (TPS_parameter & (0x7 << 3))>>3;
            pParam->TerParam.eLPCodeRate = (TPS_parameter & (0x7 <<6))>>6;
            pParam->TerParam.eHPCodeRate = (TPS_parameter & (0x7 << 9))>>9;
            pParam->TerParam.eGuardInterval= (TPS_parameter & (0x3 << 12))>>12;
            pParam->TerParam.eFFT_Mode = (TPS_parameter & (0x3<<14))>>14;
            if(pParam->TerParam.eFFT_Mode == 2)
                pParam->TerParam.eFFT_Mode = DEMOD_TER_FFT_4K;
        }
        break;
    case E_DEVICE_DEMOD_DVB_T2:
        bRet = MSB124X_DTV_DVB_T2_GetSignalL1Info(u8DemodIndex, &L1_Info);
        if(bRet)
        {
            pParam->TerParam.eConstellation = L1_Info&0x7;
            pParam->TerParam.eHierarchy = 0;
            pParam->TerParam.eHPCodeRate = (L1_Info & (0x7 <<3))>>3;
            pParam->TerParam.eLPCodeRate = pParam->TerParam.eHPCodeRate;

            if(pParam->TerParam.eHPCodeRate == 1)
                pParam->TerParam.eHPCodeRate = DEMOD_CONV_CODE_RATE_3_5;
            else if(pParam->TerParam.eHPCodeRate == 2)
                pParam->TerParam.eHPCodeRate = DEMOD_CONV_CODE_RATE_2_3;
            else if(pParam->TerParam.eHPCodeRate == 3)
                pParam->TerParam.eHPCodeRate = DEMOD_CONV_CODE_RATE_3_4;
            else if(pParam->TerParam.eHPCodeRate == 4)
                pParam->TerParam.eHPCodeRate = DEMOD_CONV_CODE_RATE_4_5;
            else if(pParam->TerParam.eHPCodeRate == 5)
                pParam->TerParam.eHPCodeRate = DEMOD_CONV_CODE_RATE_5_6;

            pParam->TerParam.eGuardInterval= (L1_Info & (0x7 << 6))>>6;
            pParam->TerParam.eFFT_Mode = (L1_Info & (0x7<<9))>>9;
            if(pParam->TerParam.eFFT_Mode)
                pParam->TerParam.eFFT_Mode += 1;
        }
    case E_DEVICE_DEMOD_DVB_C:
        bRet = MSB124X_DTV_DVB_C_GetSignalTpsInfo(u8DemodIndex, &L1_Info);
        if(bRet)
        {
            pParam->CabParam.u16SymbolRate = L1_Info&0xffff;
            pParam->CabParam.eConstellation = (L1_Info>>16)&0xff;
        }
        break;
    default:
        break;
    }
    return bRet;
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB124X) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSB124X",
    .data                         = DEMOD_MSB124X,
    .init                         = MSB124X_Demod_Init,
    .GetLock                      = MSB124X_Demod_GetLock,
    .GetSNR                       = MSB124X_Demod_GetSNR,
    .GetBER                       = MSB124X_Demod_GetBER,
    .GetPWR                       = MSB124X_Demod_GetPWR,
    .GetQuality                   = MSB124X_Demod_GetSignalQuality,
    .GetParam                     = MSB124X_Demod_GetParam,
    .Restart                      = MSB124X_Demod_Restart,
    .I2CByPass                    = MSB124X_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .CheckExist                   = MSB124X_Check_Exist,
    .Extension_Function           = MSB124X_Extension_Function,
    .Extension_FunctionPreSetting = NULL,
    .Get_Packet_Error              = MSB124X_Get_Packet_Error,
#if MS_DVBT2_INUSE
    .SetCurrentDemodType          = MSB124X_Demod_SetCurrentDemodType,
    .GetCurrentDemodType          = MSB124X_Demod_GetCurrentDemodType,
    .GetPlpBitMap                 = MSB124X_DTV_GetPlpBitMap,
    .GetPlpGroupID                = MSB124X_DTV_GetPlpGroupID,
    .SetPlpGroupID                = MSB124X_DTV_SetPlpGroupID,
    .SetScanTypeStatus            = MSB124X_Demod_SetScanTypeStatus,
    .GetScanTypeStatus            = MSB124X_Demod_GetScanTypeStatus,
    .GetNextPLPID                 = MSB124X_DTV_GetNextPLPID,
    .GetPLPType                   = MSB124X_DTV_GetPLPType,
#endif
#if MS_DVBS_INUSE
    .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
    .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
    .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
    .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
    .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
    .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
    .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
    .DiSEqCSetTone                = MSB124X_DiSEqC_SetTone,
    .DiSEqCSetLNBOut              = MSB124X_DiSEqC_SetLNBOut,
    .DiSEqCGetLNBOut              = MSB124X_DiSEqC_GetLNBOut,
    .DiSEqCSet22kOnOff            = MSB124X_DiSEqC_Set22kOnOff,
    .DiSEqCGet22kOnOff            = MSB124X_DiSEqC_Get22kOnOff,
    .DiSEqC_SendCmd               = MSB124X_DiSEqC_SendCmd
#endif

};

#endif

