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

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "drvIIC.h"
#include "MsOS.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "apiDigiTuner.h"   //<< not good
#include "drvDemodNull.h"
#include "drvGPIO.h"
#include "drvDTC.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1236C)

#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
#define USE_SPI_LOAD_TO_SDRAM
#endif

#ifdef USE_SPI_LOAD_TO_SDRAM
#include "drvSYS.h"
#include "drvMSPI.h"
#define SPI_DEVICE_BUFFER_SIZE    256
extern DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum);
extern DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);
#endif


#include "device_demodulator_msb1236c.h"
MS_U8 MSB1236C_LIB[] =
{
#include "msb1236c_dvbt.dat"
};

#define UNUSED(var)     (void)((var) = (var))
#define MSB1236C_DEMOD_IIC 1
#define MSB1236C_SPI_IIC 2
#define MSB1236C_DEBUG_ON 1

#define MDrv_Timer_Delayms  OS_DELAY_TASK
#define MDrv_Timer_GetTime0  OS_SYSTEM_TIME

#define PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 0
#define LOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 1
#define SDRAM_DATA_CHECK                0
#define MSB1236_DVBT_ONLY                0
#define TIMING_VERIFICATION              0

#define MSB1236C_NO_CHANNEL_CHECK 1

#if MSB1236C_NO_CHANNEL_CHECK
#define NO_CHANNEL_CHECK_INTERVAL             500
#endif

#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define SDRAM_BASE                       0x5000
#define SRAM_BASE                        0x8000

// msb1236c flash size: 1Mbits
#define MAX_MSB1236C_LIB_LEN             131072
#define MSB1236C_BOOT  0x01
#define MSB1236C_DVBT2 0x02
#define MSB1236C_DVBT  0x04
#define MSB1236C_DVBC  0x08
#define MSB1236C_ALL   0x0F

#define REG_MB_CNTL        0x0C00
#define REG_MB_ADDR_L    0x0C02
#define REG_MB_ADDR_H    0x0C03
#define REG_MB_DATA        0x0C04

#define EDINBURGH_BOOT_START_ADDR     0x00000
#define EDINBURGH_BOOT_END_ADDR       0x00FFF
#define EDINBURGH_DVBT2_P1_START_ADDR 0x01000
#define EDINBURGH_DVBT2_P1_END_ADDR   0x08FFF
#define EDINBURGH_DVBT2_P2_START_ADDR 0x09000
#define EDINBURGH_DVBT2_P2_END_ADDR   0x0FFFF
#define EDINBURGH_DVBT_START_ADDR     0x10000
#define EDINBURGH_DVBT_END_ADDR       0x17FFF
#define EDINBURGH_DVBC_START_ADDR     0x18000
#define EDINBURGH_DVBC_END_ADDR       0x1FFFF

#define EDINBURGH_WINDOWS_BASE                0x100
#define EDINBURGH_BOOT_WINDOWS_OFFSET         EDINBURGH_WINDOWS_BASE
#define EDINBURGH_DVBT2_P2_WINDOWS_OFFSET    (EDINBURGH_WINDOWS_BASE + 0x08)
#define EDINBURGH_DVBT2_P1_WINDOWS_OFFSET    (EDINBURGH_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBT_WINDOWS_OFFSET        (EDINBURGH_DVBT2_P1_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBC_WINDOWS_OFFSET        (EDINBURGH_DVBT_WINDOWS_OFFSET + 0x08)

#define MSB1236C_MAX_FLASH_ON_RETRY_NUM 3
#define usleep(x)                   MsOS_DelayTaskUs(x)
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

static float g_msb1236c_fSNR = 0.0;

#define MAX_DEMOD_NUMBER MAX_FRONTEND_NUM
float fDVBT2_SSI_Pref[][6] =
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

#define MSB1236C_ResetDemodTime  50
#define waitFlashTime   50

#if (LOAD_DSP_CODE_FROM_MAIN_CHIP_I2C == 1)
static MS_U8 g_WO_SPI_FLASH = 1;
#else
static MS_U8 g_WO_SPI_FLASH = 0;
#endif

//---------------------------------- for DVB-T
//operation
#define RFAGC_EN                0x00
#define HUMDET_EN               0x00
#define DCR_EN                  0x01
#define IIS_EN                  0x01
#define CCI_EN                  0x01
#define ACI_EN                  0x01
#define IQB_EN                  0x00
#define AUTO_IQ_SWAP            0x01
#define AUTO_RFMAX              0x00
#define AUTO_ACI                0x00
#define MODE_CP_FORCED          0x00
#define TPS_FORCED              0x00
#define AUTO_SCAN               0x00
#define RSV_0D                  0x00
#define RSV_0E                  0x00
#define RSV_0F                  0x00

//configure
#define RSSI                    0x00
#define ZIF                     0x00
#define FREQ                    0x00
#define FS_L                    0x80
#define FS_H                    0x70
#define BW                      0x03
#define MODE                    0x01
#define CP                      0x03
#define LP_SEL                  0x00
#define CSTL                    0x00
#define HIER                    0x00
#define HPCR                    0x00
#define LPCR                    0x00
#define RFMAX                   0x01
#define ATV_SYSTEM              0x01
#define ICFO_RANGE              0x01
#define RFAGC_REF               0x64
#define IFAGC_REF_2K            0x4B
#define IFAGC_REF_8K            0x4B
#define IFAGC_REF_ACI           0x4B
#define IFAGC_REF_IIS_2K        0xA0
#define IFAGC_REF_IIS_8K        0xA0
#define IFAGC_ACI_DET_TH_L      0x9A
#define IFAGC_ACI_DET_TH_H      0x01
#define SFO_2K_H                0x00
#define SFO_2K_L                0xC8
#define SFO_8K_H                0x00
#define SFO_8K_L                0xC8
#define CHECK_CHANNEL           0x00
#define SNR_POS                 0x00
#define CCI_KP                  0x00
#define CCI_FSWEEP              0x00
#define TS_CLK_RATE_AUTO        0x00
#define DVBT_IF_INV_PWM_OUT_EN  0x00
#define DVBT_CRC                0x00
#define IQ_SWAP        FRONTEND_DEMOD_IQ_SWAP
#if TS_PARALLEL_OUTPUT
#define SERIAL_TS               0x00
#else
#define SERIAL_TS               0x01
#endif
#define TS_CLK_SEL             0x06
#define TS_DATA_SWAP       0x00
#define TS_OUT_INV             TS_CLK_INV

// FC: FC = FS = 4800 = 0x12C0     (4.80MHz IF)
#if (IF_THIS_TUNER_INUSE(TUNER_MXL603) || IF_THIS_TUNER_INUSE(TUNER_MS222))
#define FC_L           0x88    //5M
#define FC_H           0x13
#else
#define FC_L           0xC0
#define FC_H           0x12
#endif


//configure
//------------------------------------------------ for DVB-T2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL               0x04

// FC: FC = FS = 4800 = 0x12C0     (4.80MHz IF)
#if (IF_THIS_TUNER_INUSE(TUNER_MXL603) || IF_THIS_TUNER_INUSE(TUNER_MS222))
#define T2_FC_L_VAL            0x88    //5M
#define T2_FC_H_VAL            0x13
#else
#define T2_FC_L_VAL            0xC0    // 4.57M
#define T2_FC_H_VAL            0x12
#endif

#if TS_PARALLEL_OUTPUT
#define T2_TS_SERIAL_VAL       0x00
#else
#define T2_TS_SERIAL_VAL       0x01
#endif
#define T2_TS_OUT_INV_VAL           0x01
#define T2_TS_DATA_SWAP_VAL         0x00
#define T2_TS_CLK_RATE_VAL          0x03
#define T2_TS_ERR_POL_VAL           0x00
#define T2_IF_AGC_INV_PWM_EN_VAL    0x00

MS_U8 MSB1236C_DVBT_DSPREG_TABLE[] = // andy 2009-12-15  3:55:03 TW model
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

typedef enum
{
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1,
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2,
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_3,
    E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_4
} eDMD_MSB123xc_DemodI2CSlaveID;

typedef enum
{
    E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES,
    E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES
} eDMD_MSB123xc_DemodI2CMethod;


typedef struct
{
    MS_BOOL     bInited;
    MS_BOOL    bOpen;
    MS_BOOL     bLoaded;
    EN_DEVICE_DEMOD_TYPE  enDemodType;
    MS_S32     s32_MSB1236C_Mutex;
    MS_U8       u8MsbData[6];
    MS_U8       PlpIDList[256];
    MS_U8       u8SdramCode;
    MS_U8       u8sramCode;
    MS_U8     *pMSB1236CLib;
    MS_U32     u32MSB1236ClibSize;
    MS_U8     *pMSB1236C_RegDVBT;
    MS_U32     u32CurrFreq;
    MS_U8       u8CurrPlpID;
    MS_U8       u8CurrScanType;
    DEMOD_EN_TER_BW_MODE     enCurrBW;
    DEMOD_EN_TER_BW_MODE     enTuneBw;
    MS_U8       PlpIDSize;
    MS_U32     u32ChkScanTimeStart;
    MS_BOOL   bFECLock;
    MS_U8       u8ScanStatus;
    MS_U32     u32DmxInputPath;
#if MSB1236C_NO_CHANNEL_CHECK
    MS_BOOL noChannelStable;
    MS_BOOL t2NoChannelFlag;
    MS_BOOL tNoChannelFlag;
    MS_U32    u32LockTime;
#endif
    MS_BOOL (*fpMSB123xc_I2C_Access)(eDMD_MSB123xc_DemodI2CSlaveID eSlaveID, eDMD_MSB123xc_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    int RstPin;
} MDvr_CofdmDmd_CONFIG;

const S_RFAGC_SSI RFAGC_SSI[] =
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

const S_IFAGC_SSI IFAGC_SSI[] =
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

const S_SSI_PREF SsiPref[] =
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

MDvr_CofdmDmd_CONFIG* ptrMSB1236C = NULL;
DEMOD_MS_INIT_PARAM*  pMSB1236C_InitParam = NULL;
MS_BOOL* pDemodRest = NULL;
static MS_U8 u8MSB1236C_DevCnt = 0;

static MS_U8 DEMOD_DYNAMIC_SLAVE_ID_1  =  0xF2;  // 0xD2 //CMD
static MS_U8 DEMOD_DYNAMIC_SLAVE_ID_2   = 0xA2; // 0xB2 //ISP
//    static MS_U8 DEMOD_DYNAMIC_SLAVE_ID_3 =   0xB2
//    static MS_U8 DEMOD_DYNAMIC_SLAVE_ID_4   = 0xF2

static MS_U8 u8Possible_SLAVE_IDs[2] = {0xF2, 0xD2};

static MS_BOOL MSB1236C_Variables_alloc(void)
{
    MS_U8 i;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = NULL;
    DEMOD_MS_INIT_PARAM*  pInitParam = NULL;

    if(NULL == ptrMSB1236C)
    {
        ptrMSB1236C = (MDvr_CofdmDmd_CONFIG *)malloc(sizeof(MDvr_CofdmDmd_CONFIG) * MAX_DEMOD_NUMBER);
        if(NULL == ptrMSB1236C)
            return FALSE;
        else
        {
            for(i=0; i< MAX_DEMOD_NUMBER; i++)
            {
                pMSB1236C = (ptrMSB1236C + i);
                pMSB1236C ->bInited = FALSE;
                pMSB1236C ->bOpen = FALSE;
                pMSB1236C ->bLoaded = FALSE;
                pMSB1236C ->s32_MSB1236C_Mutex = -1;
                pMSB1236C ->RstPin = MSB1236C_RST_PIN_NOT_SET;
            }
        }
    }

    if(NULL == pDemodRest)
    {
        pDemodRest = (MS_BOOL*)malloc(sizeof(MS_BOOL) * MAX_DEMOD_NUMBER);
        if(NULL == pDemodRest)
            return FALSE;
        else
        {
            for(i=0; i< MAX_DEMOD_NUMBER; i++)
                *(pDemodRest + i) = TRUE;
        }
    }


    if(NULL == pMSB1236C_InitParam)
    {
        pMSB1236C_InitParam = (DEMOD_MS_INIT_PARAM *)malloc(sizeof(DEMOD_MS_INIT_PARAM) * MAX_DEMOD_NUMBER);
        if(NULL == pMSB1236C_InitParam)
            return FALSE;
        else
        {
            for(i=0; i< MAX_DEMOD_NUMBER; i++)
            {
              pInitParam = (pMSB1236C_InitParam + i);
              pInitParam->pstTunertab  = NULL;
              pInitParam->fpCB= NULL;
              pInitParam->u32DmxInputPath= 0;
            }
        }
    }

    return TRUE;

}

static MS_BOOL MSB1236C_Variables_free(void)
{
    if(NULL != ptrMSB1236C)
    {
        free(ptrMSB1236C);
        ptrMSB1236C = NULL;
    }

    if(NULL != pDemodRest)
    {
        free(pDemodRest);
        pDemodRest = NULL;
    }

    if(NULL != pMSB1236C_InitParam)
    {
        free(pMSB1236C_InitParam);
        pMSB1236C_InitParam = NULL;
    }

    return TRUE;
}

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

#ifdef USE_SPI_LOAD_TO_SDRAM
static void msb1236_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void msb1236_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void msb1236_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {msb1236_SPIPAD_TS0_En,\
                                   msb1236_SPIPAD_TS1_En,\
                                   msb1236_SPIPAD_TS2_En};

static void msb1236_SPIPAD_TS0_En(MS_BOOL bOnOff)
{
    if(bOnOff)
    {
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    }
    else
    {
       if(SERIAL_TS)
        {
           MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_SERIAL_IN);
        }
       else
        {
           MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_PARALLEL_IN);
        }
    }

}

static void msb1236_SPIPAD_TS1_En(MS_BOOL bOnOff)
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

static void msb1236_SPIPAD_TS2_En(MS_BOOL bOnOff)
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


static void msb1236C_SPIPAD_En(MS_U8 u8DemodIndex,MS_BOOL bOnOff)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    if(0 == pMSB1236C->u32DmxInputPath)
#else
    if(DMX_INPUT_EXT_INPUT0 == pMSB1236C->u32DmxInputPath)
#endif
    {
        msb1236_SPIPAD_TS0_En(bOnOff);
    }
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    else if(1 == pMSB1236C->u32DmxInputPath)
#else
    else if(DMX_INPUT_EXT_INPUT1 == pMSB1236C->u32DmxInputPath)
#endif
    {
        msb1236_SPIPAD_TS1_En(bOnOff);
    }
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
        else if(2 == pMSB1236C->u32DmxInputPath)
#else
        else if(DMX_INPUT_EXT_INPUT2 == pMSB1236C->u32DmxInputPath)
#endif
        {
            msb1236_SPIPAD_TS2_En(bOnOff);
        }
    else
    {}
}

/*================================================
==                       MSPI write/read interface
=================================================*/
MS_BOOL MDrv_SS_RIU_Write8(MS_U16 u16Addr, MS_U8 data)
{
    MS_BOOL bRet = true;
    MS_U8 Wdata[4];

    Wdata[0] = RIU_W1_CMD;
    Wdata[1] = u16Addr & 0xFF;
    Wdata[2] = (u16Addr >> 8) & 0xFF;
    Wdata[3] = data;

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);

    // send write address & data
    MDrv_MSPI_Write(Wdata,4);

    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}
MS_BOOL MDrv_SS_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata)
{
    MS_BOOL bRet = true;
    MS_U8 Rdata[5];

    Rdata[0] = RIU_R1T_CMD;
    Rdata[1] = u16Addr & 0xFF;
    Rdata[2] = (u16Addr >> 8) & 0xFF;
    Rdata[3] = 0x00;

    MDrv_MSPI_SlaveEnable(TRUE);
    // send read command to read data
    MDrv_MSPI_Write(Rdata,4);
    // read operation
    MDrv_MSPI_Read(pdata, 1);
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;

}

MS_BOOL MDrv_SS_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    MS_BOOL bRet = true;
    MS_U8 Wdata[5];

    Wdata[0] = MIU_W_CMD;
    Wdata[1] = u32Addr & 0xFF;
    Wdata[2] = (u32Addr >> 8) & 0xFF;
    Wdata[3] = (u32Addr >> 16)& 0xFF;
    Wdata[4] = (u32Addr >> 24);
    //printf("MDrv_SS_MIU_Writes\n");

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    // send data
    MDrv_MSPI_Write(pdata,u16Size);
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;

}

MS_BOOL MDrv_SS_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    MS_BOOL bRet = true;
    MS_U8 Rdata[SPI_DEVICE_BUFFER_SIZE];
    MS_U16 dataLen, i, j=0;

    do
    {
        dataLen = (u16Size>16?16:u16Size);//(len>24?24:len);

        Rdata[0] = MIU_R_CMD;
        Rdata[1] = u32Addr & 0xFF;
        Rdata[2] = (u32Addr >> 8) & 0xFF;
        Rdata[3] = (u32Addr >> 16)& 0xFF;
        Rdata[4] = (u32Addr >> 24);
        Rdata[5] = dataLen+1;
        //printf("MDrv_SS_MIU_Reads, addr=0x%lx, dataLen=%d\n", u32Addr, dataLen);

        // send read command to read data
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,6);
        MDrv_MSPI_SlaveEnable(FALSE);

        // read operation
        Rdata[0] = MIU_ST_CMD;
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,1);
        //printf("Read Operation\n");
        MDrv_MSPI_Read(Rdata, dataLen+1);
        MDrv_MSPI_SlaveEnable(FALSE);

        if(Rdata[0] != 0x0A)
        {
            DMD_ERR(("MDrv_SS_MIU_Reads fail, status=0x%x\n", Rdata[0]));
            return false;
        }

        for (i=1; i<dataLen+1; i++, j++)
        {
            pdata[j] = Rdata[i];
            //printf("%x, ", pdata[j]);
        }

        u16Size -= dataLen;
        u32Addr += dataLen;
        //printf("u16Size=%d,  u32Addr=0x%lx\n", u16Size, u32Addr);
    }
    while(u16Size);

    return bRet;
}
#endif

/*================================================
==                       IIC write/read interface
=================================================*/
MS_BOOL MSB1236C_WriteBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Write(DEMOD_DYNAMIC_SLAVE_ID_1, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error\n"));
    }
    return bRet;
}

MS_BOOL MSB1236C_ReadBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;

    bRet = MDrv_IIC_Read(DEMOD_DYNAMIC_SLAVE_ID_1, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}

MS_BOOL MSB1236C_WriteBytes_spi(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Write(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write spi error\n"));
    }
    return bRet;
}

MS_BOOL MSB1236C_ReadBytes_spi(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_Read(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read spi error\n"));
    }
    return bRet;
}
//  Port 1
MS_BOOL MSB1236C_WriteBytes_demod1(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC1_Write(DEMOD_DYNAMIC_SLAVE_ID_1, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC1 write error\n"));
    }
    return bRet;
}

MS_BOOL MSB1236C_ReadBytes_demod1(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;

    bRet = MDrv_IIC1_Read(DEMOD_DYNAMIC_SLAVE_ID_1, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC1 read error\n"));
    }
    return bRet;
}

MS_BOOL MSB1236C_WriteBytes_spi1(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC1_Write(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC1 write spi error\n"));
    }
    return bRet;
}

MS_BOOL MSB1236C_ReadBytes_spi1(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC1_Read(DEMOD_DYNAMIC_SLAVE_ID_2, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC1 read spi error\n"));
    }
    return bRet;
}

typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

static mapi_i2c DemodI2Chandler[2], SpiI2Chandler[2];
mapi_i2c* mapi_i2c_GetI2C_Dev(MS_U32 u32gID)
{

    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        DMD_DBG(("iic device not supported\n"));
    case MSB1236C_DEMOD_IIC:
        handler = &DemodI2Chandler[0];
        handler->WriteBytes = MSB1236C_WriteBytes_demod;
        handler->ReadBytes = MSB1236C_ReadBytes_demod;
        break;
    case MSB1236C_SPI_IIC:
        handler = &SpiI2Chandler[0];
        handler->WriteBytes = MSB1236C_WriteBytes_spi;
        handler->ReadBytes = MSB1236C_ReadBytes_spi;
        break;
    }
    return handler;
}
mapi_i2c* mapi_i2c1_GetI2C_Dev(MS_U32 u32gID)
{
    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        DMD_DBG(("iic device not supported\n"));
    case MSB1236C_DEMOD_IIC:
        handler = &DemodI2Chandler[1];
        handler->WriteBytes = MSB1236C_WriteBytes_demod1;
        handler->ReadBytes = MSB1236C_ReadBytes_demod1;
        break;
    case MSB1236C_SPI_IIC:
        handler = &SpiI2Chandler[1];
        handler->WriteBytes = MSB1236C_WriteBytes_spi1;
        handler->ReadBytes = MSB1236C_ReadBytes_spi1;
        break;
    }
    return handler;
}

static MS_BOOL msb1236c_I2C_Access(eDMD_MSB123xc_DemodI2CSlaveID eSlaveID, eDMD_MSB123xc_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c_GetI2C_Dev(MSB1236C_DEMOD_IIC);

    switch (eSlaveID)
    {
    case E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2:
        i2c_iptr = mapi_i2c_GetI2C_Dev(MSB1236C_SPI_IIC);
        break;
    case E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1:
    default:
        i2c_iptr = mapi_i2c_GetI2C_Dev(MSB1236C_DEMOD_IIC);
        break;
    }

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
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
static MS_BOOL msb1236c_I2C1_Access(eDMD_MSB123xc_DemodI2CSlaveID eSlaveID, eDMD_MSB123xc_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c1_GetI2C_Dev(MSB1236C_DEMOD_IIC);

    switch (eSlaveID)
    {
    case E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2:
        i2c_iptr = mapi_i2c1_GetI2C_Dev(MSB1236C_SPI_IIC);
        break;
    case E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1:
    default:
        i2c_iptr = mapi_i2c1_GetI2C_Dev(MSB1236C_DEMOD_IIC);
        break;
    }

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
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
==               demod  msb1236c  write/read interface
=================================================*/
MS_BOOL MSB1236C_WriteReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{

    MS_BOOL bRet = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);

    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }

    pMSB1236C->u8MsbData[0] = 0x10;
    pMSB1236C->u8MsbData[1] = 0x00;
    pMSB1236C->u8MsbData[2] = 0x00;
    pMSB1236C->u8MsbData[3] = (u16Addr >> 8) & 0xff;
    pMSB1236C->u8MsbData[4] = u16Addr & 0xff;
    pMSB1236C->u8MsbData[5] = u8Data;

    pMSB1236C->u8MsbData[0] = 0x35;
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, pMSB1236C->u8MsbData);

    pMSB1236C->u8MsbData[0] = 0x10;
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, pMSB1236C->u8MsbData);

    pMSB1236C->u8MsbData[0] = 0x34;
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, pMSB1236C->u8MsbData);

    return bRet;
}

MS_BOOL MSB1236C_WriteRegs(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8MsbDataValue[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);


    u8MsbDataValue[0] = 0x10;
    u8MsbDataValue[1] = 0x00;
    u8MsbDataValue[2] = 0x00;
    u8MsbDataValue[3] = (u16Addr >> 8) & 0xff;
    u8MsbDataValue[4] = u16Addr & 0xff;
    for (idx = 0; idx < data_size ; idx++)
    {
        u8MsbDataValue[5 + idx] = u8pData[idx];
    }

    u8MsbDataValue[0] = 0x35;
    //bRet &= iptr->WriteBytes(0, NULL, 1, u8MsbDataValue);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    u8MsbDataValue[0] = 0x10;
    //bRet &= iptr->WriteBytes(0, NULL, 5 + data_size, u8MsbDataValue);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5 + data_size, u8MsbDataValue);

    u8MsbDataValue[0] = 0x34;
    //bRet &= iptr->WriteBytes(0, NULL, 1, u8MsbDataValue);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    return bRet;
}

MS_BOOL MSB1236C_WriteReg2bytes(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL ret = TRUE;
    ret &= MSB1236C_WriteReg(u8DemodIndex,u16Addr, (MS_U8)u16Data & 0x00ff);
    ret &= MSB1236C_WriteReg(u8DemodIndex,u16Addr + 0x0001, (MS_U8)(u16Data >> 8) & 0x00ff);
    return ret;
}

MS_BOOL MSB1236C_ReadReg(MS_U8 u8DemodIndex,MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    /*
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    */
    /*
    mapi_i2c *iptr = mapi_i2c_GetI2C_Dev(MSB1236C_DEMOD_IIC);
    if (iptr == NULL)
    {
        printf("%s(),%d Init Fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    */

    pMSB1236C->u8MsbData[0] = 0x10;
    pMSB1236C->u8MsbData[1] = 0x00;
    pMSB1236C->u8MsbData[2] = 0x00;
    pMSB1236C->u8MsbData[3] = (u16Addr >> 8) & 0xff;
    pMSB1236C->u8MsbData[4] = u16Addr & 0xff;

    pMSB1236C->u8MsbData[0] = 0x35;
    //iptr->WriteBytes(0, NULL, 1, pMSB1236C->u8MsbData);
    bRet = pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, pMSB1236C->u8MsbData);

    pMSB1236C->u8MsbData[0] = 0x10;
    //iptr->WriteBytes(0, NULL, 5, pMSB1236C->u8MsbData);
    //iptr->ReadBytes(0, NULL, 1, pu8Data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, pMSB1236C->u8MsbData);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, pu8Data);

    pMSB1236C->u8MsbData[0] = 0x34;
    //bRet = iptr->WriteBytes(0, NULL, 1, pMSB1236C->u8MsbData);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, pMSB1236C->u8MsbData);

    return bRet;
}

MS_BOOL  MSB1236C_WriteDspReg(MS_U8 u8DemodIndex,MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8     status = TRUE;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;
    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_DATA, u8Data);
    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_ADDR_L, (MS_U8)(u16Addr));
    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_CNTL, 0x04);

    do
    {
        status &= MSB1236C_ReadReg(u8DemodIndex,REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            DMD_ERR(("MSB1236C_MB_WRITE_FAILURE\n"));
            return FALSE;
        }
    }
    while (cntl != 0xff);

    return status;
}

MS_BOOL MSB1236C_ReadDspReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8* pData)
{
    MS_U8     status = TRUE;
    MS_U8     cntl = 0x00;
    MS_U16    cntr = 0x00;

    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_ADDR_L, (MS_U8)(u16Addr));
    status &= MSB1236C_WriteReg(u8DemodIndex,REG_MB_CNTL, 0x03);

    do
    {
        status &= MSB1236C_ReadReg(u8DemodIndex,REG_MB_CNTL, &cntl);
        if (cntr++ > 0x7ff)
        {
            DMD_ERR(("MSB1236C_MB_READ_FAILURE\n"));
            return FALSE;
        }
    }
    while (cntl != 0xff);

    status &= MSB1236C_ReadReg(u8DemodIndex,REG_MB_DATA, pData);
    return status;
}


/*================================================
==               demod  msb1236c  base interface
=================================================*/
void MSB1236C_Show_Version(MS_U8 u8DemodIndex)
{
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    MS_U8  reg = 0;
    MS_U16 ver0 = 0;
    MS_U8  ver1 = 0;

    MSB1236C_ReadDspReg(u8DemodIndex,E_T2_FW_VER_0, &reg);
    ver0 = reg;
    MSB1236C_ReadDspReg(u8DemodIndex,E_T2_FW_VER_1, &reg);
    ver0 = (ver0 << 8) | reg;
    MSB1236C_ReadDspReg(u8DemodIndex,E_T2_FW_VER_2, &reg);
    ver1 = reg;

    DMD_DBG(("\n[msb1236c]ver0 = 0x%x, ver1 = 0x%x\n", ver0, ver1));

    return;
#endif
}

MS_BOOL MSB1236C_I2C_CH_Reset(MS_U8 u8DemodIndex,MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);

    DMD_DBG(("[msb1236c][beg]MSB1236C_I2C_CH_Reset, CH=0x%x , eDemodI2cID[%02x], DemodRest:[%d]\n", ch_num,DEMOD_DYNAMIC_SLAVE_ID_1,*(pDemodRest+u8DemodIndex)));
    /*
        mapi_i2c *iptr = mapi_i2c_GetI2C_Dev(MSB1236C_DEMOD_IIC);
        if (iptr == NULL)
        {
            return FALSE;
        }
    */
    if (*(pDemodRest+u8DemodIndex))
    {
        *(pDemodRest+u8DemodIndex) = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        //bRet &= iptr->WriteBytes(0, NULL, 5, data);
        bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, data);
        if(!bRet)
            return FALSE;
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    //bRet &= iptr->WriteBytes(0, NULL, 1, data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    //bRet &= iptr->WriteBytes(0, NULL, 1, data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    //bRet &= iptr->WriteBytes(0, NULL, 1, data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    //bRet &= iptr->WriteBytes(0, NULL, 1, data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    //bRet &= iptr->WriteBytes(0, NULL, 1, data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    //bRet &= iptr->WriteBytes(0, NULL, 1, data);
    bRet &= pMSB1236C->fpMSB123xc_I2C_Access(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    /*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
    */
    DMD_DBG(("[msb1236c][end]MSB1236C_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}

// 144 ms roughly
MS_BOOL MSB1236C_HW_init(MS_U8 u8DemodIndex)
{
    DMD_ENTERAPISTR(u8DemodIndex);
    MS_BOOL bRet = TRUE;
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    MDvr_CofdmDmd_CONFIG *pMSB1236C;
    pMSB1236C = ptrMSB1236C + u8DemodIndex;
#endif

#if(TIMING_VERIFICATION == 1)
    tmm_1 = GIVE_ME_TIME;
#endif

    usleep(1);

    // MSB1236C_Reset MCU
    // -------------------------------------------------------------------
    // Initialize DMD_ANA_MISC
    // -------------------------------------------------------------------
    // [1:0]    reg_mpll_loop_div_first
    // [15:8]   reg_mpll_loop_div_second
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x0A00 + (0x33) * 2, 0x2400);

    // [2:0]    reg_mpll_ictrl
    // [3]  reg_mpll_in_sel
    // [4]  reg_mpll_xtal2adc_sel
    // [5]  reg_mpll_xtal2next_pll_sel
    // [6]  reg_mpll_vco_offset
    // [7]  reg_mpll_pd
    // [8]  reg_xtal_en
    // [10:9]   reg_xtal_sel
    // [11]     reg_mpll_porst
    // [12]     reg_mpll_MSB1236C_Reset
    // [13]     reg_pd_dmpll_clk
    // [14]     reg_pd_3p3_1
    // [15]     reg_pd_3p3_2

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x0A00 + (0x35) * 2, 0x1804);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x0A00 + (0x35) * 2, 0x0004);


    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x0A00 + (0x34) * 2, 0x4000);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x0A00 + (0x34) * 2, 0x0000);

    // [0]  reg_mpll_clk_dp_pd  dummy
    // [1]  reg_adc_clk_pd      ADC output clock power down
    // [2]  reg_mpll_div2_pd    MPLL_DIV2 power down
    // [3]  reg_mpll_div3_pd    MPLL_DIV3 power down
    // [4]  reg_mpll_div4_pd    MPLL_DIV4 power down
    // [5]  reg_mpll_div8_pd    MPLL_DIV8 power down
    // [6]  reg_mpll_div10_pd   MPLL_DIV10 power down
    // [13:8]  reg_mpll_adc_div_sel select the ADC clock divide ratio,ADC clk=XTAL_IN * (LOOP_DIV_FIRST*LOOP_DIV_SECOND)/div_ratio

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x30) * 2, 0x1e00);

    // -------------------------------------------------------------------
    // Begin Initialize Initialize ADC I/Q
    // -------------------------------------------------------------------
    // [0]  Q channel ADC power down
    // [1]  I channel ADC power down
    // [2]  Q channel clamp enable. 0:enable, 1:disable
    // [3]  I channel clamp enable. 0:enable, 1:disable
    // [6:4]    I channel input mux control;
    //      3'b000=I channel ADC calibration mode input
    //          3'b001=VIF signal from VIFPGA
    //          3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //          all the other combination are only for test mode, don't use without understanding.
    // [10:8]   Q channel input mux control;
    //      3'b000=Q channel ADC calibration mode input
    //          3'b001=VIF signal from VIFPGA 3'b010 = SSIF signal from PAD_SIFP(M)
    //          3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //          all the other combination are only for test mode, don't use without understanding.
    // [12] ADC I,Q swap enable; 1: swap
    // [13] ADC clock out select; 1: ADC_CLKQ
    // [14] ADC linear calibration bypass enable; 1:enable
    // [15] ADC internal 1.2v regulator control always 0 in T3
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x01) * 2, 0x0440);


    // [0]  Channel I ADC power down: 1=power dwon
    // [1]  Channel Q ADC power down: 1=power dwon
    // [2]  power down clamp buffer for test mode
    // [3]  change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x0c) * 2, 0x0000);

    // [3:0]    clamp voltage control
    //          3'b000 = 0.7v
    //          3'b001 = 0.75v
    //          3'b010 = 0.5v
    //          3'b011 = 0.4v
    //          3'b100 = 0.8v
    //          3'b101 = 0.9v
    //          3'b110 = 0.65v
    //          3'b111 = 0.60v
    // [4]  REFERENCE power down
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x20) * 2, 0x0000);

    // Set ADC gain is 1
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x0b) * 2, 0x0009);

    // Disable ADC Sign bit
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x2e) * 2, 0x0000);

    // ADC I channel offset
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x2a) * 2, 0x0c00);

    // ADC Q channel offset
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x2b) * 2, 0x0c00);
    // -------------------------------------------------------------------
    // End Initialize Initialize ADC I/Q
    // -------------------------------------------------------------------


    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x19) * 2, 0x1e00);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x1c) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x1e) * 2, 0x0000);


    // [15:0]   reg_mpll_test
    // [4]  mpll lock detector enable
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0A00 + (0x34) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x09) * 2, 0x0101);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x0a) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x0b) * 2, 0x0030);

    // @0x0910
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x10) * 2, 0x1110);

    // @0x0911
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x11) * 2, 0x0111);

    // @0x0912
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x12) * 2, 0x0618);

    // @0x0913
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x13) * 2, 0x0020);

    // @0x0914
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x14) * 2, 0x0000);

    // @0x0915
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x15) * 2, 0x0111);

    // @0x0916
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x16) * 2, 0x0001);

    // @0x0917
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x17) * 2, 0x0000);

    // @0x0918
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x18) * 2, 0x1000);

    // @0x0919
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x19) * 2, 0x3c00);

    // @0x091a
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x1a) * 2, 0x0000);

    // @0x091b
    // [2:0]    reg_ckg_dvbt2_outer1x
    // [6:4]    reg_ckg_dvbt2_outer2x
    // [10:8]   reg_ckg_syn_miu
    // [14:12]  reg_ckg_syn_ts
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x1b) * 2, 0x0000);


    // @0x091c
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x1c) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x08) * 2, 0x0a01);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x28) * 2, 0x0000);


    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x51) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x52) * 2, 0x0020);


    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x50) * 2, 0x0010);


    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x57) * 2, 0x0003);


    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x55) * 2, 0x0006);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x55) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x59) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x20) * 2, 0x0004);

#if(MSB1236_DVBT_ONLY == 0)
    MS_U8 u8_tmp = 0;
    MS_U8 u8_timeout = 0;

    /*Beg initialize MIU*/

    // dummy register setting
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0000);

    // dummy register setting
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0000);

    // dummy register setting
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0000);

    // miu software MSB1236C_Reset
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0c00);

    // set frequence 130MHz
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x11) * 2, 0x0052);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x10) * 2, 0x352b); // 0x29a378 = 166MHz, 0x352b52 = 130MHz

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x11) * 2, 0x6052);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x11) * 2, 0x0052);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x12) * 2, 0xc000);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x13) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x16) * 2, 0x0030);

    //bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x1c) * 2, 0x0080);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x1c) * 2, 0x00a0);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x1b) * 2, 0x0067);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x01) * 2, 0x8100);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x01) * 2, 0xe100);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x01) * 2, 0x8100);

    // in_phase
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x02) * 2, 0x0360);

    // tREFRESH                      : [7:0]        30
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x03) * 2, 0x0030);

    // reg_tRAS                      : [3:0]         9
    // reg_tRC                       : [7:4]         c
    // reg_tRCD                      : [11:8]        3
    // reg_tRP                       : [15:12]       3
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x04) * 2, 0x33c9);

    // reg_tRRD                      : [3:0]         2
    // tWR                           : [7:4]         3
    // reg_tMRD                      : [11:8]        2
    // reg_tRTP                      : [15:12]       4
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x05) * 2, 0x4232);

    // reg_w2r_dly(tWTR)             : [3:0]         2
    // reg_w2r_oen_dly               : [7:4]         3
    // reg_r2w_dly(tRTW)             : [11:8]        5
    // reg_r2w_oen_dly               : [15:12]       5
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x06) * 2, 0x5532);

    // tRFC                          : [5:0]         c
    // reg_tRAS[4]                   : [6]           0
    // reg_tRC[4]                    : [7]           0
    // reg_write_latency             : [10:8]        0
    // reg_tCCD                      : [15:14]       1
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x07) * 2, 0x400c);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0e) * 2, 0x1800);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x23) * 2, 0x7ffe);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x20) * 2, 0xc001);

    // #40000
    //delay 1
    usleep(1);

    // miu software MSB1236C_Reset
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0c00);

    // miu software MSB1236C_Reset
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x0f) * 2, 0x0c00);

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x01) * 2, 0x010d);

    /*Beg Initial DRAM start here!!!*/

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x00) * 2, 0x0001);

    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    // wait init_done

    bRet &= MSB1236C_ReadReg(u8DemodIndex,0x1201, &u8_tmp);
    DMD_DBG(("[msb1236c]MIU wait init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));
    while (u8_tmp != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            DMD_ERR(("[msb1236c][err]MIU init failure...\n"));
            return FALSE;
        }
        // 10us delay
        usleep(10);
        bRet &= MSB1236C_ReadReg(u8DemodIndex,0x1201, &u8_tmp);
    }
    DMD_DBG(("[msb1236c]MIU init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x08) * 2, 0x0001);

    /*Beg set MIU self test parameter*/

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x71) * 2, 0x0000);

    // length
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x72) * 2, 0x0010);

    // test data
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x74) * 2, 0x5aa5);

    // reg_test_en                   : [0]
    // reg_test_mode                 : [2:1] 00 : address,
    //                                       01 : test data
    //                                       10 : shift data
    // reg_inv_data                  : [3]
    // reg_test_loop                 : [4]
    // reg_force_out                 : [5]
    // reg_force_in                  : [6]
    // reg_read_only                 : [8]
    // reg_write_only                : [9]
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x70) * 2, 0x0001);

    // #10000;
    //delay 1
    usleep(1);

    // wait for test done

    bRet &= MSB1236C_ReadReg(u8DemodIndex,(0x1200 + (0x70) * 2 + 1), &u8_tmp);
    DMD_DBG(("[msb1236c]MIU wait test done, u8_tmp=0x%x,bRet=0x%x\n", u8_tmp, bRet));
    while ((u8_tmp & 0x80) != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            DMD_ERR(("[msb1236c][err]MIU self test NOT finished...\n"));
            return FALSE;
        }
        // 10us delay
        usleep(10);
        bRet &= MSB1236C_ReadReg(u8DemodIndex,(0x1200 + (0x70) * 2 + 1), &u8_tmp);
    }
    // test done.


    // #10000;
    //delay 1
    usleep(1);

    // test result check

    bRet &= MSB1236C_ReadReg(u8DemodIndex,(0x1200 + (0x70) * 2 + 1), &u8_tmp);

    if (u8_tmp == 0x40)
    {
        DMD_ERR(("@msb1236c, error, MIU self test fail !!!!\n"));
        bRet = FALSE;
    }
    else
    {
        DMD_DBG(("@msb1236c, MIU self test successful.\n"));
    }

    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1200 + (0x23) * 2, 0x0000);

    /*End initialize MIU*/
#endif

    /* Beg Turn on pads */
    // ------Turn off all pad in
    // [0] reg_set_pad_low
    // [1] reg_set_pad_high
    // [2] reg_set_i2cs_pad_low
    // [3] reg_set_i2cs_pad_high
    // [8] reg_turn_off_pad
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x28) * 2, 0x0000);

    // ------I2CM pad on
    // [1:0]    reg_iicm_pad_sel[1:0]   1:iicm enable 2:UART enable
    // [4]      reg_i2c_sbpm_en     1: enable I2CS bypass to I2CM function
    // [12:8]   reg_i2c_sbpm_idle_num[4:0]  a: default
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x08) * 2, 0x0a01);

    // ------Transport Stream pad on (except TS ERR pad)
    // [15:0]   reg_en_ts_pad[15:0] 0x00ff:normal TS location 0xff00:reverse TS location
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x2d) * 2, 0x00ff);

    // ------Transport Stream pad on (TS ERR pad)
    // [0]      reg_en_ts_err_pad   1: enable
    // [4]      reg_ts_err_pol  1: inverse 0:normal
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x2e) * 2, 0x0001);

    // ------AGC pad on
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0a00 + (0x18) * 2, 0x0010);

#ifdef USE_SPI_LOAD_TO_SDRAM
    // ------Enable Slave SPI PAD and swithc to use TS_PAD as SSPI_PAD for Kappa
    // [0:0] reg_en_sspi_pad
    // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    if(MSPI_PATH_NONE == pMSB1236C->u32DmxInputPath)
    {
        //DMD_DBG((" Load DMD FW by I2C\n"));
    }
    else
#endif
    {
        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900+(0x28)*2, 0x0000);
        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900+(0x2d)*2, 0x00ff);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x3b) * 2, 0x0002);
        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x3a)*2+1,&u8_tmp);
        u8_tmp &= 0xFC;
        MSB1236C_WriteReg(u8DemodIndex,0x0900+(0x3a)*2+1, u8_tmp);

        // ------- MSPI driving setting
        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900+(0x2c)*2, 0x07ff);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x3b) * 2, 0x0002);
    }
#endif

    //bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0990, 0x20);

#if(TIMING_VERIFICATION == 1)
    tmm_2 = GIVE_ME_TIME;
#endif
    DMD_EXITAPISTR(u8DemodIndex,bRet);

    return bRet;

}

// mem_type 0: dram, 1:dram+sram
// 28 ms roughly
static MS_BOOL MSB1236C_MEM_switch(MS_U8 u8DemodIndex,MS_U8 mem_type)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8_tmp = 0;
    MS_U16 timeout = 0;

    DMD_DBG(("[msb1236c][beg]MSB1236C_MEM_switch, mem_type=0x%x\n", mem_type));

#if(TIMING_VERIFICATION == 1)
    tmm_15 = GIVE_ME_TIME;
#endif

    if (mem_type == 1)
    {

        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x2B80, 0x10);

        // SRAM_START_ADDR 0x0000
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1000, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1004, 0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1002, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1006, 0x7FFF);

        // DRAM_START_ADDR 0x8000
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1008, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x100C, 0x8000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x100A, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x100E, 0xFFFF);

        // Enable SRAM&SDRAM memory map
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1018, 0x05);

        // Wait memory map to be enabled
        do
        {
            bRet &= MSB1236C_ReadReg(u8DemodIndex,0x1018, &u8_tmp);
            if (timeout++ > 500)
            {
                DMD_ERR(("@msb1236c, D+S memory mapping failure.!!!\n"));
                return FALSE;
            }
        }
        while (u8_tmp != 0x05);
    }
    else if (mem_type == 0)
    {
        // Enable SRAM&SDRAM memory map

        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x2B80, 0x10);

        // DRAM_START_ADDR 0x8000
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1008, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x100C, 0x0000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x100A, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x100E, 0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1018, 0x04);

        // Wait memory map to be enabled
        do
        {
            bRet &= MSB1236C_ReadReg(u8DemodIndex,0x1018, &u8_tmp);
            if (timeout++ > 500)
            {
                DMD_ERR(("@msb1236c, D memory mapping failure.!!!\n"));
                return FALSE;
            }
        }
        while (u8_tmp != 0x04);
    }
    else if (mem_type == 2)
    {
        // Enable only SRAM memory map

        // SRAM_START_ADDR 0x0000
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1000, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1004, 0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1002, 0x0000);
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x1006, 0x7FFF);

        // Enable only SRAM memory map
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1018, 0x01);

        // Wait memory map to be enabled
        do
        {
            bRet &= MSB1236C_ReadReg(u8DemodIndex,0x1018, &u8_tmp);
            if (timeout++ > 500)
            {
                DMD_ERR(("@msb1236c, D+S memory mapping failure.!!!\n"));
                return FALSE;
            }
        }
        while (u8_tmp != 0x01);
    }
    else
    {
        DMD_ERR(("@msb1236c, invalid mem type mapping.\n"));
        return FALSE;
    }
#if(TIMING_VERIFICATION == 1)
    tmm_16 = GIVE_ME_TIME;
#endif

    DMD_DBG(("[msb1236c][end]MSB1236C_MEM_switch, , mem_type=0x%x, bRet=0x%x\n", mem_type, bRet));

    return bRet;
}

MS_BOOL MSB1236C_Reset(MS_U8 u8DemodIndex)
{
    DMD_ENTERAPISTR(u8DemodIndex);
    MS_U8     u8Data = 0x00;
    MS_U32    u32Retry = 0x00;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        DMD_EXITAPISTR(u8DemodIndex, 0);
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        DMD_EXITAPISTR(u8DemodIndex, 0);
        return FALSE;
    }

    DMD_DBG(("Set TSP0 registers...\n"));

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        DMD_DBG(("\nreset T2\n"));
        // mask miu access for all and mcu
        MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        usleep(10);

        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x10) * 2, 0x01);
        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x19) * 2, 0x03);
        MSB1236C_WriteReg(u8DemodIndex,0x0C00, 0x00);             //clear MB_CNTL
        usleep(5 * 100);

        // enable miu access of mcu gdma
        MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        usleep(10);

        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x19) * 2, 0x00);
        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x10) * 2, 0x00);
        usleep(5 * 1000);
        MSB1236C_ReadReg(u8DemodIndex,0x0C00, &u8Data);           //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            usleep(50 * 1000);
            MSB1236C_ReadReg(u8DemodIndex,0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                DMD_ERR((">>>MSB1236C: MSB1236C_Reset Fail!\n"));
                break;
            }
        }
    }
    else if ((pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T) ||
             (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        DMD_DBG(("\nreset T\n"));
        // mask miu access for all and mcu
        MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        usleep(10);

        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x19) * 2, 0x01);
        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x10) * 2, 0x01);
        MSB1236C_WriteReg(u8DemodIndex,0x0C00, 0x00);             //clear MB_CNTL

        usleep(5 * 1000);

        // enable miu access of mcu gdma
        MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        usleep(10);

        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x10) * 2, 0x00);
        MSB1236C_WriteReg(u8DemodIndex,0x0B00 + (0x19) * 2, 0x00);

        MSB1236C_ReadReg(u8DemodIndex,0x0C00, &u8Data);       //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            usleep(50 * 1000);
            MSB1236C_ReadReg(u8DemodIndex,0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                DMD_ERR((">>>MSB1236C: MSB1236C_Reset Fail!\n"));
                break;
            }
        }

    }

    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();
    DMD_EXITAPISTR(u8DemodIndex, 1);
    return TRUE;
}

/*================================================
==               demod  msb1236c loading fw interface
=================================================*/
#if (MSB1236_DVBT_ONLY == 1)
static MS_BOOL MSB1236C_Load2Sram(MS_U8 u8DemodIndex, MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sram_win_offset_base)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL bRet = TRUE;
    MS_U16  sram_win_offset = sram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;

    UNUSED(sram_win_offset);
    DMD_DBG(("[msb1236c][beg]MSB1236C_Load2Sram, len=0x%x, win_offset=0x%x\n", data_length, sram_win_offset_base));

#if(TIMING_VERIFICATION == 1)
    tmm_3 = GIVE_ME_TIME;
#endif

    if (MSB1236C_I2C_CH_Reset(u8DemodIndex,0) == FALSE)
    {
        DMD_ERR((">>>MSB1236C CH0 MSB1236C_Reset:Fail\n"));
        return FALSE;
    }
    else
    {
        //--------------------------------------------------------------------------
        //  Set xData map for Program SRAM
        //--------------------------------------------------------------------------
        //set lower bound "xb_eram_hb[5:0]"
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1000 + (0x70) * 2 + 1, 0x20);

        //set upper bound "xb_eram_lb[5:0]"
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1000 + (0x70) * 2, 0x3f);

        //set "reg_xd2eram_hk_mcu_psram_en"
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1000 + (0x73) * 2, 0x08);

        for (x_data_offset = 0, y_cir_addr = SRAM_BASE; x_data_offset < data_length;)
        {
            if ((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
            {
                bRet &= MSB1236C_WriteRegs(u8DemodIndex, y_cir_addr, (u8_ptr + x_data_offset), LOAD_CODE_I2C_BLOCK_NUM);
                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
            }
            else
            {
                bRet &= MSB1236C_WriteRegs(u8DemodIndex, y_cir_addr, (u8_ptr + x_data_offset), data_length - x_data_offset);
                y_cir_addr += (data_length - x_data_offset);
                x_data_offset += (data_length - x_data_offset);
            }
        }
        DMD_DBG(("[msb1236c]x_data_offset=%d,y_cir_addr=%d\n", x_data_offset, y_cir_addr));

        //--------------------------------------------------------------------------
        //  Release xData map for Program SRAM
        //--------------------------------------------------------------------------
        //clear  "reg_xd2eram_hk_mcu_psram_en"
        // `DBG.iic_write( 2, (`RIUBASE_MCU + 8'he6), 8'h00);
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1000 + (0x73) * 2, 0x00);

        // Channel changed from CH 0x00 to CH 0x03
        if (MSB1236C_I2C_CH_Reset(u8DemodIndex,3) == FALSE)
        {
            DMD_ERR((">>>MSB1236C CH3 MSB1236C_Reset:Fail\n"));
            return FALSE;
        }
    }

#if(TIMING_VERIFICATION == 1)
    tmm_4 = GIVE_ME_TIME;
    show_timer();
#endif

    DMD_DBG(("[msb1236c][end]MSB1236C_Load2Sram, len=0x%x, win_offset=0x%x\n", data_length, sram_win_offset_base));
    return bRet;

}

static MS_BOOL MSB1236C_LoadDSPCodeToSRAM_dvbt(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCodeToSRAM_dvbt\n"));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (!(pMSB1236C->u8sramCode & MSB1236C_DVBT))
    {
        // dvbt code
        if (sizeof(MSB1236C_LIB) > EDINBURGH_DVBT_START_ADDR)
        {
            data_ptr = MSB1236C_LIB + EDINBURGH_DVBT_START_ADDR;
            code_size = EDINBURGH_DVBT_END_ADDR - EDINBURGH_DVBT_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sram(u8DemodIndex, data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB1236C->u8sramCode |= MSB1236C_DVBT;
            }
        }
        else
        {
            DMD_ERR(("@msb1236c, dvbt code is unavailable!!!\n"));
        }
    }
    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCodeToSRAM_dvbt\n"));
    return bRet;
}
#endif

// i2c 150Kb/s, average rate 110Kb/s, 32KB, 2436ms.
static MS_BOOL MSB1236C_Load2Sdram(MS_U8 u8DemodIndex,MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sdram_win_offset_base)
{
    MS_BOOL bRet = TRUE;
    MS_U16  sdram_win_offset = sdram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;
    #if (SDRAM_DATA_CHECK == 1)
    MS_U16 i = 0, j = 0, index = 0;
    MS_U8 buf[SPI_DEVICE_BUFFER_SIZE]= {0};
    #endif
    #if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    MDvr_CofdmDmd_CONFIG *pMSB1236C;
    MS_BOOL bUse_SPI = FALSE;
    pMSB1236C = ptrMSB1236C + u8DemodIndex;
    #endif

    DMD_DBG(("[msb1236c][beg]MSB1236C_Load2Sdram, len=0x%x, win_offset=0x%x\n", data_length, sdram_win_offset_base));

#if(TIMING_VERIFICATION == 1)
    tmm_3 = GIVE_ME_TIME;
#endif

#ifdef USE_SPI_LOAD_TO_SDRAM
    MS_U32 u32Addr = 0;

#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    if(MSPI_PATH_NONE != pMSB1236C->u32DmxInputPath)
        bUse_SPI = TRUE;
#else
    bUse_SPI = TRUE;
#endif

    if(bUse_SPI)
    {
    u32Addr = (MS_U32)sdram_win_offset_base * 0x1000; // unit: 4K
    bRet &= MDrv_SS_MIU_Writes(u32Addr, u8_ptr, data_length);
    DMD_DBG(("****SPI load2sdram****, sdram_addr=0x%lx, data_length=0x%x bRet %d\n", u32Addr, data_length, bRet));

#if (SDRAM_DATA_CHECK == 1)
    if((data_length % SPI_DEVICE_BUFFER_SIZE) == 0)
        index = data_length / SPI_DEVICE_BUFFER_SIZE;
    else
        index = data_length / SPI_DEVICE_BUFFER_SIZE + 1;

    for (i=0; i<index; i++)
    {
        memset(buf, 0x00, SPI_DEVICE_BUFFER_SIZE);

        bRet &= MDrv_SS_MIU_Reads(u32Addr+SPI_DEVICE_BUFFER_SIZE*i, buf, SPI_DEVICE_BUFFER_SIZE);
        DMD_DBG(("[%s][%d] bRet %d\n", __FUNCTION__, __LINE__, bRet));
        for (j=0; j<SPI_DEVICE_BUFFER_SIZE; j++)
        {
            //printf("j=%d, buf=0x%x, data=0x%x", j, buf[j], u8_ptr[j]);
            if (buf[j] != u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j])
            {
                DMD_ERR(("spi miu write&read fail! idx=%x, y=0x%x, x=0x%x\n", (SPI_DEVICE_BUFFER_SIZE*i+j), buf[i], u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j]));
                //break;
            }
        }
    }
#endif
    }
    else
#endif
    {

    if (MSB1236C_I2C_CH_Reset(u8DemodIndex,0) == FALSE)
    {
        DMD_ERR((">>>MSB1236C CH0 MSB1236C_Reset:Fail\n"));
        return FALSE;
    }
    else
    {
        // set xData map upper and low bound for 64k DRAM window
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x2B00 + (0x63) * 2, 0x3F24);

        // set xData map offset for 64k DRAM window, 64kbytes alignment
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x2B00 + (0x64) * 2, 0x0000);

        // set xData map upper and low bound for 4k DRAM window
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x2B00 + (0x65) * 2, 0x2014);

        // set xData map offset for 4k DRAM window, 4kbytes alignment
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x2B00 + (0x66) * 2, sdram_win_offset++);

        // enable xData map for DRAM
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x2B00 + (0x62) * 2, 0x0007);

        for (x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
        {
            if (y_cir_addr == 0x6000)
            {
                //set xData map offset for 4k DRAM window, 4kbytes alignment
                // 0x1000, 4096 bytes
                bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x2B00 + (0x66) * 2, sdram_win_offset++);
                y_cir_addr = SDRAM_BASE;
            }

            // max 0x200, error above.....

            if ((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
            {
                bRet &= MSB1236C_WriteRegs(u8DemodIndex, y_cir_addr, (u8_ptr + x_data_offset), LOAD_CODE_I2C_BLOCK_NUM);
                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
            }
            else
            {
                bRet &= MSB1236C_WriteRegs(u8DemodIndex, y_cir_addr, (u8_ptr + x_data_offset), data_length - x_data_offset);
                y_cir_addr += (data_length - x_data_offset);
                x_data_offset += (data_length - x_data_offset);
            }
        }
        DMD_DBG(("[msb1236c]x_data_offset=%d,y_cir_addr=%d,z_block_num=%d\n", x_data_offset, y_cir_addr, sdram_win_offset));
#if (SDRAM_DATA_CHECK == 1)
        // beg data check.
        PRINTE(("[msb1236c]SDRAM data check...\n"));

        sdram_win_offset = sdram_win_offset_base;

        // set xData map offset for 4k DRAM window, 4kbytes alignment
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x2B00 + (0x66) * 2, sdram_win_offset++);

        for (x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
        {
            MS_U8 u8_tmp;
            if (y_cir_addr == 0x6000)
            {
                //set xData map offset for 4k DRAM window, 4kbytes alignment
                // 0x1000, 4096 bytes
                bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x2B00 + (0x66) * 2, sdram_win_offset++);
                y_cir_addr = SDRAM_BASE;
            }

            bRet &= MSB1236C_ReadReg(u8DemodIndex,y_cir_addr++, &u8_tmp);
            if (u8_tmp != *(u8_ptr + x_data_offset++))
            {
                DMD_ERR(("[msb1236c]error, idx=0x%x, y=0x%x, x=0x%x\n", y_cir_addr - 1, u8_tmp, *(u8_ptr + x_data_offset - 1)));
            }
        }

        DMD_DBG(("[msb1236c]SDRAM data check...Done\n"));

        // end data check
#endif
        //  Release xData map for SDRAM
        bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x2B00 + (0x62) * 2, 0x0000);

        // Channel changed from CH 0x00 to CH 0x03
        if (MSB1236C_I2C_CH_Reset(u8DemodIndex,3) == FALSE)
        {
            DMD_ERR((">>>MSB1236C CH3 MSB1236C_Reset:Fail\n"));
            return FALSE;
        }
    }
    }

#if(TIMING_VERIFICATION == 1)
    tmm_4 = GIVE_ME_TIME;
    show_timer();
#endif

    DMD_DBG(("[msb1236c][end]MSB1236C_Load2Sdram, len=0x%x, win_offset=0x%x\n", data_length, sdram_win_offset_base));
    return bRet;
}

// 92~95ms roughly
static MS_BOOL MSB1236C_LoadSdram2Sram(MS_U8 u8DemodIndex,MS_U8 CodeNum)
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8_tmp = 0;
    MS_U8   u8DoneFlag = 0;
    MS_U32  u32Timeout = 0;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadSdram2Sram, g_sram_code=0x%x, codeNum=0x%x\n", pMSB1236C->u8sramCode, CodeNum));

#if(TIMING_VERIFICATION == 1)
    tmm_17 = GIVE_ME_TIME;
#endif

    if (CodeNum == pMSB1236C->u8sramCode)
    {
        DMD_ERR(("[msb1236c]MSB1236C_LoadSdram2Sram, code is available.\n"));
        return bRet;
    }

    bRet &= MSB1236C_MEM_switch(u8DemodIndex,0);

    if (CodeNum == MSB1236C_DVBT2)
        u8_tmp = 1 | 0x10;
    else if (CodeNum == MSB1236C_DVBT)
        u8_tmp = 2 | 0x10;
    else if (CodeNum == MSB1236C_DVBC)
        u8_tmp = 3 | 0x10;
    else
        u8_tmp = 0 | 0x10;

    // Assign f/w code type to load => 0x11: dvbt2, 0x12: dvbt, 0x13: dvbc
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0900 + (0x4f) * 2, u8_tmp);

    // enable miu mask, miu, mcu, gdma
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x0f);
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);

    usleep(10);

    // enable mcu
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x00);

    do
    {
        bRet &= MSB1236C_ReadReg(u8DemodIndex,0x0900 + (0x4f) * 2, &u8DoneFlag);

        if (u32Timeout++ > 500)
        {
            DMD_ERR(("@msb1236c, MSB1236C_LoadSdram2Sram boot move code fail.!!!\n"));
            return FALSE;
        }
        usleep(1 * 1000);

    }
    while (u8DoneFlag != 0xaa);

    // mask miu access of mcu
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf2);

    // 10us delay
    usleep(10);

    // Disable MCU
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x03);

    // enable miu mask, miu, mcu, gdma
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x0f);
    bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);

    bRet &= MSB1236C_MEM_switch(u8DemodIndex,1);

    if (bRet == FALSE)
    {
        pMSB1236C->u8sramCode = 0x00;
    }
    else
    {
        pMSB1236C->u8sramCode = CodeNum;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_18 = GIVE_ME_TIME;
#endif

    DMD_DBG(("[msb1236c][end]MSB1236C_LoadSdram2Sram, codeNum=0x%x, g_sram_code=0x%x, bRet=0x%x\n", CodeNum, pMSB1236C->u8sramCode, bRet));

    return bRet;
}

static MS_BOOL MSB1236C_LoadDSPCodeToSDRAM_Boot(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCodeToSDRAM_Boot\n"));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (!(pMSB1236C->u8SdramCode & MSB1236C_BOOT))
    {
        if (sizeof(MSB1236C_LIB) > EDINBURGH_BOOT_START_ADDR)
        {
            // boot code
            data_ptr = MSB1236C_LIB + EDINBURGH_BOOT_START_ADDR;
            code_size = EDINBURGH_BOOT_END_ADDR - EDINBURGH_BOOT_START_ADDR + 1;
            win_offset = EDINBURGH_BOOT_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sdram(u8DemodIndex,data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB1236C->u8SdramCode |= MSB1236C_BOOT;
            }
        }
        else
        {
            DMD_ERR(("@msb1236c, boot code is unavailable!!!\n"));
        }
    }
    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCodeToSDRAM_Boot\n"));
    return bRet;
}

static MS_BOOL MSB1236C_LoadDSPCodeToSDRAM_dvbt2(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCodeToSDRAM_dvbt2\n"));

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (!(pMSB1236C->u8SdramCode & MSB1236C_DVBT2))
    {
        if (sizeof(MSB1236C_LIB) > EDINBURGH_DVBT2_P1_START_ADDR)
        {
            // dvbt2_p2
            data_ptr = MSB1236C_LIB + EDINBURGH_DVBT2_P2_START_ADDR;
            code_size = EDINBURGH_DVBT2_P2_END_ADDR - EDINBURGH_DVBT2_P2_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT2_P2_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sdram(u8DemodIndex,data_ptr, code_size, win_offset);

            // dvbt2_p1
            data_ptr = MSB1236C_LIB + EDINBURGH_DVBT2_P1_START_ADDR;
            code_size = EDINBURGH_DVBT2_P1_END_ADDR - EDINBURGH_DVBT2_P1_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT2_P1_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sdram(u8DemodIndex,data_ptr, code_size, win_offset);

            if (bRet == TRUE)
            {
                pMSB1236C->u8SdramCode |= MSB1236C_DVBT2;
            }
        }
        else
        {
            DMD_ERR(("@msb1236c, dvbt2 code is unavailable!!!\n"));
        }
    }
    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCodeToSDRAM_dvbt2\n"));

    return bRet;
}

static MS_BOOL MSB1236C_LoadDSPCodeToSDRAM_dvbt(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCodeToSDRAM_dvbt\n"));

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (!(pMSB1236C->u8SdramCode & MSB1236C_DVBT))
    {
        // dvbt code
        if (sizeof(MSB1236C_LIB) > EDINBURGH_DVBT_START_ADDR)
        {
            data_ptr = MSB1236C_LIB + EDINBURGH_DVBT_START_ADDR;
            code_size = EDINBURGH_DVBT_END_ADDR - EDINBURGH_DVBT_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sdram(u8DemodIndex,data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB1236C->u8SdramCode |= MSB1236C_DVBT;
            }
        }
        else
        {
            DMD_ERR(("@msb1236c, dvbt code is unavailable!!!\n"));
        }
    }
    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCodeToSDRAM_dvbt\n"));
    return bRet;
}

static MS_BOOL MSB1236C_LoadDSPCodeToSDRAM_dvbc(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCodeToSDRAM_dvbc\n"));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (!(pMSB1236C->u8SdramCode & MSB1236C_DVBC))
    {
        // dvbc code
        if (sizeof(MSB1236C_LIB) > EDINBURGH_DVBC_START_ADDR)
        {
            data_ptr = MSB1236C_LIB + EDINBURGH_DVBC_START_ADDR;
            code_size = EDINBURGH_DVBC_END_ADDR - EDINBURGH_DVBC_START_ADDR + 1;
            win_offset = EDINBURGH_DVBC_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sdram(u8DemodIndex,data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB1236C->u8SdramCode |= MSB1236C_DVBC;
            }
        }
        else
        {
            DMD_ERR(("@msb1236c, dvbc code is unavailable!!!\n"));
        }
    }
    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCodeToSDRAM_dvbc\n"));
    return bRet;
}

// [0] bootloader [1] dvbt2, [2] dvbt, [3] dvbc [4] all
static MS_BOOL MSB1236C_LoadDSPCodeToSDRAM(MS_U8 u8DemodIndex,MS_U8 code_n)
{
    MS_BOOL bRet = TRUE;

    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCodeToSDRAM, code_n=0x%x\n", code_n));

#if(TIMING_VERIFICATION == 1)
    tmm_7 = GIVE_ME_TIME;
#endif

    switch (code_n)
    {
    case MSB1236C_BOOT:
    {
        // boot code
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_Boot(u8DemodIndex);
    }
    break;
    case MSB1236C_DVBT2:
    {
        // dvbt2 code
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_dvbt2(u8DemodIndex);
    }
    break;
    case MSB1236C_DVBT:
    {
        // dvbt
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_dvbt(u8DemodIndex);
    }
    break;
    case MSB1236C_DVBC:
    {
        // dvbtc
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_dvbc(u8DemodIndex);
    }
    break;
    case MSB1236C_ALL:
    default:
    {
        // boot+dvbt2+dvbt+dvbc

        // boot code
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_Boot(u8DemodIndex);
        // dvbt2
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_dvbt2(u8DemodIndex);
        // dvbt
        bRet &= MSB1236C_LoadDSPCodeToSDRAM_dvbt(u8DemodIndex);
        // dvbtc
        //bRet &= MSB1236C_LoadDSPCodeToSDRAM_dvbc();
    }
    break;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_8 = GIVE_ME_TIME;
    show_timer();
#endif

    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCodeToSDRAM, code_n=0x%x, bRet=0x%x\n", code_n, bRet));
    return bRet;
}

/*================================================
==               demod  msb1236c init dvbt and dvbt2 interface
==               serial/parallel setting interface
=================================================*/
MS_U8 MSB1236C_DTV_DVBT_DSPReg_CRC(MS_U8 u8DemodIndex)
{
    MS_U8 crc = 0;
    MS_U8 idx = 0;
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    for (idx = 0; idx < ((MS_U8)MSB1236C_DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        crc ^= pMSB1236C->pMSB1236C_RegDVBT[idx];
    }

    crc = ~crc;

    return crc;
}

MS_BOOL MSB1236C_DTV_DVBT_DSPReg_Init(MS_U8 u8DemodIndex)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8    idx = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    for (idx = 0; idx < ((MS_U8)MSB1236C_DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
    {
        if (MSB1236C_WriteDspReg(u8DemodIndex,idx + (MS_U8)T_OPMODE_RFAGC_EN, pMSB1236C->pMSB1236C_RegDVBT[idx]) != TRUE)
        {
            DMD_ERR(("dsp reg init NG\n"));
            return FALSE;
        }
    }

    pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MSB1236C_DTV_DVBT_DSPReg_CRC(u8DemodIndex);

    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)T_PARAM_CHECK_SUM, pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]) != TRUE)
    {
        DMD_ERR(("dsp reg write crc NG\n"));
        return FALSE;
    }

    DMD_DBG(("dsp reg init ok\n"));

    return TRUE;
}

MS_BOOL MSB1236C_DTV_DVBT2_DSPReg_Init(MS_U8 u8DemodIndex)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_BW, T2_BW_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_FC_L, T2_FC_L_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_FC_H, T2_FC_H_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_TS_SERIAL, T2_TS_SERIAL_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_TS_CLK_RATE, T2_TS_CLK_RATE_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_TS_OUT_INV, T2_TS_OUT_INV_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_TS_DATA_SWAP, T2_TS_DATA_SWAP_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }
    if (MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)E_T2_TS_ERR_POL, T2_TS_ERR_POL_VAL) != TRUE)
    {
        DMD_ERR(("T2 dsp reg init NG\n"));
    }

    DMD_DBG(("T2 dsp reg init ok\n"));

    return TRUE;
}

static void MSB1236C_Driving_Control(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8 u8Temp;
    MSB1236C_ReadReg(u8DemodIndex,0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    MSB1236C_WriteReg(u8DemodIndex,0x0958, u8Temp);

    MSB1236C_ReadReg(u8DemodIndex,0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp & (~0x0F);
    }
    MSB1236C_WriteReg(u8DemodIndex,0x0959, u8Temp);
}

static MS_BOOL MSB1236C_DTV_Serial_Control(MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    UNUSED(bEnable);
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        //FRA_T2_BRINGUP
    }
    MSB1236C_Driving_Control(u8DemodIndex,1);
    return TRUE;
}

/*================================================
==               demod  msb1236c iic bypass interface
=================================================*/
MS_BOOL MSB1236C_IIC_Bypass_Mode(MS_U8 u8DemodIndex, MS_BOOL enable)
{
    MS_U8 u8Retry = 10;
    MS_BOOL bResult;

    DMD_DBG((">>>MSB1236C: IIC_bypass() set %x\n", enable));

    while (u8Retry--)
    {
        if (enable)
            bResult = MSB1236C_WriteReg(u8DemodIndex,0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            bResult = MSB1236C_WriteReg(u8DemodIndex,0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off

        if (bResult)
            break;
    }
    return TRUE;
}

/*================================================
==               demod  msb1236c lock and ssi sqi interface
=================================================*/
MS_BOOL MSB1236C_Lock(MS_U8 u8DemodIndex,EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
{
    MS_U16        u16Address = 0;
    MS_U8         cData = 0;
    MS_U8         cBitMask = 0;
    MS_U8         use_dsp_reg = 0;
    switch (eStatus)
    {
    case COFDM_P1_LOCK_123X:
        u16Address =  0x2482; //Pl lock,
        cBitMask = BIT3;
        break;

    case COFDM_P1_LOCK_HISTORY_123X:
        use_dsp_reg = 1;
        u16Address = E_T2_DVBT2_LOCK_HIS; //Pl ever lock,
        cBitMask = BIT5;
        break;

    case COFDM_L1_CRC_LOCK_123X:
        u16Address =  0x2741; //L1 CRC check,
        cBitMask = BIT5 | BIT6 | BIT7;
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
            MSB1236C_ReadReg(u8DemodIndex,0x11E0, &cData);//addy update 0805
            if (cData == 0x0B)
                return TRUE;
            else
                return FALSE;
            break;
        case E_DEVICE_DEMOD_DVB_C:
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
        case E_DEVICE_DEMOD_DVB_C:
        default:
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    if (!use_dsp_reg)
    {
        if (MSB1236C_ReadReg(u8DemodIndex,u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (MSB1236C_ReadDspReg(u8DemodIndex,u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    //printf("\r\n >>> MSB1236C_Lock cData = %x\n",cData);
    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }
    return FALSE;
}

MS_BOOL MSB1236C_Get_No_Channel_Flag(MS_U8 u8DemodIndex,EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
{
    MS_U16        u16Address = 0;
    MS_U8         cData = 0;
    MS_U8         cBitMask = 0;
    MS_U8         use_dsp_reg = 0;
    switch (eStatus)
    {
    case COFDM_P1_LOCK_123X:
        u16Address =  0x2482; //Pl lock,
        cBitMask = BIT3;
        break;

    case COFDM_P1_LOCK_HISTORY_123X:
        use_dsp_reg = 1;
        u16Address = E_T2_DVBT2_LOCK_HIS; //Pl ever lock,
        cBitMask = BIT5;
        break;

    case COFDM_L1_CRC_LOCK_123X:
        u16Address =  0x2741; //L1 CRC check,
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
            MSB1236C_ReadReg(u8DemodIndex,0x11E0, &cData);//addy update 0805
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
        if (MSB1236C_ReadReg(u8DemodIndex,u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (MSB1236C_ReadDspReg(u8DemodIndex,u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    //printf("\r\n >>> MSB1236C_Lock cData = %x\n",cData);
    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }
    return FALSE;
}

EN_LOCK_STATUS MSB1236C_DTV_DVB_T_GetLockStatus(MS_U8 u8DemodIndex)
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
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);

    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB123X error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB123X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        u32Timeout = DVBT2_TPS_timeout;

        if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T2, COFDM_FEC_LOCK) == TRUE)
            bCheckPass = TRUE;
        else
        {
            if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T2, COFDM_P1_LOCK_HISTORY_123X) == TRUE)
            {
                u32Timeout = DVBT2_FEC_timeout;
                DMD_DBG(("====> Short T2 Locking Time, P1 Lock Flag = 0x%x\n", u8Data));
            }
        }

        if (bCheckPass)
        {
            u32LockTimeStartDVBT2 = MsOS_GetSystemTime();
            pMSB1236C->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB1236C->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBT2 < u32LockTimeout))
        {
            // SET u32LockTimeStartDVBT2 every time reset demodulator to T2
            //PRINTE(("NowTime: %d  -  pMSB123X->u32ChkScanTimeStart  %d = %d  \n",
            //    u32NowTime,pMSB123X->u32ChkScanTimeStart,(u32NowTime-pMSB123X->u32ChkScanTimeStart)));
            //PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB1236C->bFECLock = FALSE;
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB1236C->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        u32Timeout = DVBT_TPS_timeout;
        if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
            bCheckPass = TRUE;
        else
        {
            if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T, COFDM_TPS_LOCK) == TRUE)
            {
                DMD_DBG(("T TPS locked!\n"));
                u32Timeout = DVBT_FEC_timeout;
            }
        }
        if (bCheckPass)
        {
            u32LockTimeStartDVBT = MsOS_GetSystemTime();
            pMSB1236C->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB1236C->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBT < u32LockTimeout))
        {
            //PRINTE(("NowTime: %d  -  pMSB123X->u32ChkScanTimeStart  %d = %d  \n",
            //  u32NowTime,pMSB123X->u32ChkScanTimeStart,(u32NowTime-pMSB123X->u32ChkScanTimeStart)));
            // PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB1236C->bFECLock = FALSE;
#if MSB1236C_NO_CHANNEL_CHECK
            if ( (u32NowTime -pMSB1236C->u32LockTime) >= NO_CHANNEL_CHECK_INTERVAL)
            {
                pMSB1236C->noChannelStable = TRUE;// flag is true means more than 600 ms has been passed and no ch flag has been stable
                pMSB1236C->tNoChannelFlag = MSB1236C_Get_No_Channel_Flag(u8DemodIndex,E_DEVICE_DEMOD_DVB_T,COFDM_DVBT_NOCH_FLAG);
                pMSB1236C->t2NoChannelFlag = MSB1236C_Get_No_Channel_Flag(u8DemodIndex,E_DEVICE_DEMOD_DVB_T,COFDM_DVBT2_NOCH_FLAG);
                //printf("\ntNoChannelFlag [%d][%d] \n\n ",pMSB1236C->tNoChannelFlag,pMSB1236C->t2NoChannelFlag);
                if ( pMSB1236C->tNoChannelFlag )
                {
                    if ( pMSB1236C->t2NoChannelFlag )
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
            pMSB1236C->bFECLock = FALSE;
#if MSB1236C_NO_CHANNEL_CHECK
            if ( u32Timeout == DVBT_FEC_timeout
                    && ( pMSB1236C->noChannelStable && pMSB1236C->t2NoChannelFlag )
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
    case E_DEVICE_DEMOD_DVB_C://mick
    {
        u32Timeout = DVBT2_TPS_timeout;
        u16RegAddress = 0x11E0;
        u8LockState = 0x0C;
        MSB1236C_ReadReg(u8DemodIndex,u16RegAddress, &u8Data);
        //printf("[msb123x]+++++++++DVBC State---->%d\n",u8Data);//mick
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        else
        {
            u16RegAddress =  0x0990; //TPS his Lock,
            u8BitMask = BIT3;
            MSB1236C_ReadReg(u8DemodIndex,u16RegAddress, &u8Data);
            if ((u8Data & u8BitMask) > 0)
                u32Timeout = DVBT2_FEC_timeout;
        }
        //PRINTE((">>>MSB123x: [%s] Lock Status = %d\n",__FUNCTION__, u8Data));
        if (bCheckPass)
        {
            u32LockTimeStartDVBC = MsOS_GetSystemTime();
            pMSB1236C->bFECLock = TRUE;
            return E_DEMOD_LOCK;
        }
        else if ((u32NowTime - pMSB1236C->u32ChkScanTimeStart < u32Timeout)
                 || (u32NowTime - u32LockTimeStartDVBC < u32LockTimeout))
        {
            pMSB1236C->bFECLock = FALSE;
            return  E_DEMOD_CHECKING;
        }
        else
        {
            pMSB1236C->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
    }
    break;
    default:
        return E_DEMOD_UNLOCK;
    }
}

MS_BOOL MSB1236C_DTV_GetSNR(MS_U8 u8DemodIndex)
{
    MS_U32  snr_out = 0;
    MS_U16  snr_ave_num = 0;
    MS_U8   reg = 0, reg_frz = 0;
    MS_U32  noise_power;
    float   f_snr = (float)0.0;
    float   fSNRDivider[] = {1024 * 2, 2048 * 2, 4096 * 2, 8192 * 2, 16384 * 2, 32768 * 2, 32768 * 2, 32768 * 2};
    float   snr_poly = 0.0;     // for SNR polynomial eq.

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    //float   fsnr= 0;
    MS_U8 status = TRUE;
    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        float fsnr1 = 0.0;
        //float fsnr2 = 0.0;
        //float snr_calib_slope = 0.0;
        float snr_calib_bias  = 0.0;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x7f << 1), &reg);
        if ((reg & 0x03) == 0x03)
        {
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x7d << 1) + 1, &reg);
            snr_out = (reg & 0x1f);
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x7d << 1) + 0, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x7d << 1) + 3, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x7d << 1) + 2, &reg);
            snr_out = (snr_out << 8) | reg;

            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x7f << 1), &reg);
            status &= MSB1236C_WriteReg(u8DemodIndex,0x2800 + (0x7f << 1), reg & (0xff - 0x03));
            // printf("\nreg1=0x%lx\n",snr_out);
        }
        else
        {
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x0e << 1) + 3, &reg);
            snr_out = (reg & 0x1f);
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x0e << 1) + 2, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x0e << 1) + 1, &reg);
            snr_out = (snr_out << 8) | reg;
            status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x0e << 1) + 0, &reg);
            snr_out = (snr_out << 8) | reg;
            // printf("\nreg2=0x%lx\n",snr_out);
        }

        status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x0a << 1), &reg);
        snr_ave_num = reg & 0x07;
        // printf("\nsnr_ave_num=%d\n",snr_ave_num);

        fsnr1 = 10.0 * log10((float)snr_out / fSNRDivider[snr_ave_num]);

        status &= MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x07 << 1), &reg);

        // HW cali en?
        if ((reg & 0x10) == 0x10)
        {
            // HW calibration.
            // slope
            MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x08 << 1), &reg);
            //snr_calib_slope = (float)reg / 32.0;
            // bias
            MSB1236C_ReadReg(u8DemodIndex,0x2800 + (0x08 << 1) + 1, &reg);
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
        g_msb1236c_fSNR = f_snr;
        break;
    }

    case E_DEVICE_DEMOD_DVB_T:
    {
        // bank 15 0xfe [0] reg_fdp_freeze
        status &= MSB1236C_ReadReg(u8DemodIndex,0x0ffe, &reg_frz);
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0ffe, reg_frz|0x01);

        // bank 15 0xff [0] reg_fdp_load
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0fff, 0x01);

        // bank 15 0x4a [26:0] reg_snr_accu <27,1>
        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f4d, &reg);
        noise_power = reg & 0x07;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f4c, &reg);
        noise_power = (noise_power << 8)|reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f4b, &reg);
        noise_power = (noise_power << 8)|reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f4a, &reg);
        noise_power = (noise_power << 8)|reg;

        noise_power = noise_power/2;

        // bank 15 0x26 [5:4] reg_transmission_mode
        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f26, &reg);

        // bank 15 0xfe [0] reg_fdp_freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0ffe, reg_frz);

        // bank 15 0xff [0] reg_fdp_load
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0fff, 0x01);

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
        g_msb1236c_fSNR = f_snr;
        break;
    }

    default:
        g_msb1236c_fSNR = 0.0;
        break;
    }
    return status;
}

MS_BOOL MSB1236C_DTV_GetPreBER(MS_U8 u8DemodIndex,float *p_preBer)
{
    MS_U16 BitErrPeriod = 0;
    MS_U32 BitErr = 0;
    MS_U32 FecType = 0;
    MS_U8  reg = 0;
    float fber = 0;
    MS_U8 status = TRUE;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    default:
        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x32 Data BER count[15:0]
        // bank 38 0x33 Data BER count[31:16]
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x32 << 1) + 3, &reg);
        BitErr = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x32 << 1) + 2, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x32 << 1) + 1, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x32 << 1) + 0, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
        {
            //protect 0
            BitErrPeriod = 1;
        }

        // status &= MSB1236C_ReadReg(0x268e, &reg);    //FEC Type[8:7]
        status &= MSB1236C_ReadReg(u8DemodIndex,0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x278e, &reg);    //FEC Type[8:7]
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
        DMD_DBG(("MSB1236C DVBT2 PreLDPCBER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            DMD_DBG(("DTV_DVBT2_GetPostViterbiBer Fail!\n"));
            return 0;
        }

        *p_preBer = fber;
        DMD_DBG(("MSB1236C Extend Data Pre BER = %8.3e \n ", fber));

        //printf("[DVBT2] preber=%f, Err_num=%d, block_count=%d, reg=0x%x\n", fber, BitErr, BitErrPeriod, reg);
        break;
    }
    return status;
}


MS_BOOL MSB1236C_DTV_GetPostBER(MS_U8 u8DemodIndex, float *p_postBer)
{
    MS_U16 BitErrPeriod = 0;
    MS_U16 BitErr = 0;
    MS_U16 FecType = 0;
    MS_U8  reg = 0, reg_frz=0;
    float fber = 0;
    MS_U8 status = TRUE;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x34 Data BER count[15:0]
        // bank 38 0x35 Data BER count[31:16]
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x34 << 1) + 3, &reg);
        BitErr = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x34 << 1) + 2, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x34 << 1) + 1, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x2600 + (0x34 << 1) + 0, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
            //protect 0
            BitErrPeriod = 1;

        // status &= MSB1236C_ReadReg(0x268e, &reg);    //FEC Type[8:7]
        status &= MSB1236C_ReadReg(u8DemodIndex,0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x278e, &reg);    //FEC Type[8:7]
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
        //PRINTE(("MSB1236C DVBT2 PostVitBER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            DMD_ERR(("DTV_DVBT2_GetPostViterbiBer Fail!\n"));
            return 0;
        }

        *p_postBer = fber;

        break;
    case E_DEVICE_DEMOD_DVB_T:
        /////////// Post-Viterbi BER /////////////
        // bank 17 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_ReadReg(u8DemodIndex,0x1132, &reg_frz);
        status &= MSB1236C_WriteReg(u8DemodIndex,0x1132, reg_frz | 0x80);
        // bank 17 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= MSB1236C_ReadReg(u8DemodIndex,0x1131, &reg);
        BitErrPeriod = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;
        // bank 17 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 17 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24
        status &= MSB1236C_ReadReg(u8DemodIndex,0x113D, &reg);
        BitErr = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x113C, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x113B, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x113A, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 17 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x1132, reg_frz);
        if (BitErrPeriod == 0)
            //protect 0
            BitErrPeriod = 1;
        if (BitErr <= 0)
            fber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
        else
            fber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);
        //PRINTE(("MSB1236C DVBT PostVitBER = %8.3e \n ", fber));

        if (status == FALSE)
        {
            DMD_ERR(("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }

        *p_postBer = fber;

        break;
    default:
        break;
    }

    return TRUE;
}


MS_U16 MSB1236C_DTV_GetSignalQuality(MS_U8 u8DemodIndex)
{
    MS_U8     status = TRUE;
    float       fber = (float)0.0;
    float       log_ber = (float)0.0;
    float       SQI = (float)0.0;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    status &= MSB1236C_DTV_GetPostBER(u8DemodIndex, &fber);
    if (status == FALSE)
    {
        DMD_ERR(("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
        return 0;
    }
    switch (pMSB1236C->enDemodType)
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

        return SQI;
        break;
    }
    case E_DEVICE_DEMOD_DVB_T:
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
        return SQI;
    }

    default:
        break;
    }  // end of switch
    return 0;
}

MS_U16 MSB1236C_DTV_GetSignalStrength(MS_U8 u8DemodIndex)
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
    MS_U16   ypIFAGC1 = 0;
    //MS_U16   ypIFAGC2 = 0;
    float    ch_power_db = 0;
    float    ch_power_rf = 0;
    float    ch_power_if = 0;
    float    ch_power_ref = 0;
    float    ch_power_rel = 0;
    double fPrel = 0.0, fPinput = 0.0;
    MS_U8  demodState = 0;
    MS_U8  reg = 0;
    const S_IFAGC_SSI     *ifagc_ssi;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    status &= MSB1236C_ReadReg(u8DemodIndex,0x0990, &reg);
    demodState = reg;
    switch (pMSB1236C->enDemodType)
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
        MSB1236C_WriteReg(u8DemodIndex,0x3e00 + 0x13 * 2, 0x03);
        MSB1236C_WriteReg(u8DemodIndex,0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB1236C_WriteReg(u8DemodIndex,0x3e00 + 0x02 * 2 + 1, 0x80);

        // agc gain2 24 Bit
        //MSB1236C_ReadReg(0x3e00+0x14*2+1, &reg_tmp);
        MSB1236C_ReadReg(u8DemodIndex,0x3e00 + 0x14 * 2 + 2, &reg_tmp);
        //ypIFAGC2 = reg_tmp;
        MSB1236C_ReadReg(u8DemodIndex,0x3e00 + 0x14 * 2 + 1, &reg_tmp);
        ypIFAGC1 = reg_tmp;
        MSB1236C_ReadReg(u8DemodIndex,0x3e00 + 0x14 * 2 + 0, &reg_tmp);
        ypIFAGC1 = (ypIFAGC1 << 8) | reg_tmp;
        // printf("============================>[YP] IFAGC_2 = %x, IFAGC_1 = %x\n", ypIFAGC2, ypIFAGC1);
        MSB1236C_ReadReg(u8DemodIndex,0x3e00 + 0x14 * 2 + 1, &reg_tmp);

        MSB1236C_WriteReg(u8DemodIndex,0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB1236C_WriteReg(u8DemodIndex,0x3e00 + 0x02 * 2 + 1, 0x00);
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

        MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x47 * 2), &u8Modulation);
        u8Modulation = (((MS_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
        if (u8Modulation >= 3) u8Modulation = 3;
        MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x47 * 2), &u8CodeRate);
        u8CodeRate = (((MS_U16) u8CodeRate) & (BIT2 | BIT1 | BIT0));
        if (u8CodeRate >= 5) u8CodeRate = 5;

        //Fix me, temp solution; demod doesn't lock, then use fref = -98 to compute SSI
        if (demodState != 0x09)
        {
            fPrel = fPinput - (-50.0f);
        }
        else
        {
            fPrel = fPinput - fDVBT2_SSI_Pref[u8Modulation][u8CodeRate];
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
    case E_DEVICE_DEMOD_DVB_C:
    {
        ///////// RF/IF-AGC Gain Out /////////////
        rf_agc_val = 0x3f;

        // ==== IFAGC_GAIN_OUT (GAIN2) ====

        // reg_agc_debug_sel[3:0]
        MSB1236C_ReadReg(u8DemodIndex,0x3e26, &reg_tmp);
        reg_tmp = (reg_tmp & 0xf0) | 0x03; // select ifagc_gain
        MSB1236C_WriteReg(u8DemodIndex,0x3e26, reg_tmp);

        // reg_agc_gain2_out
        // use only high byte value
        MSB1236C_ReadReg(u8DemodIndex,0x3e29, &reg_tmp);
        if_agc_val = reg_tmp;

        DMD_DBG(("SSI_RFAGC (SAR) = 0x%x\n", (MS_U16)rf_agc_val));
        DMD_DBG(("SSI_IFAGC_H = 0x%x\n", (MS_U16) if_agc_val));

        for (i = 0; i < sizeof(RFAGC_SSI) / sizeof(S_RFAGC_SSI); i++)
        {
            if (rf_agc_val <= RFAGC_SSI[i].sar3_val)
            {
                ch_power_rf = RFAGC_SSI[i].power_db;
                break;
            }
        }

        ifagc_ssi = IFAGC_SSI;
        ssi_tbl_len = sizeof(IFAGC_SSI) / sizeof(S_IFAGC_SSI);

        for (i = 0; i < ssi_tbl_len; i++)
        {
            if (if_agc_val <= ifagc_ssi[i].agc_val)
            {
                ch_power_if = ifagc_ssi[i].power_db;
                break;
            }
        }

        DMD_DBG(("ch_power_rf = %f\n", ch_power_rf));
        DMD_DBG(("ch_power_if = %f\n", ch_power_if));

        ch_power_db = (ch_power_rf > ch_power_if) ? ch_power_rf : ch_power_if;

        if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
        {
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB1236C_ReadReg(u8DemodIndex,0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB1236C_ReadReg(u8DemodIndex,0x0f24 + 1, &reg_tmp);
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
        }
        else
        {
            ch_power_ref = SsiPref[8].p_ref;
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
    default:
        ssi = 0;
        DMD_DBG(("Undefined!!!\n"));
        break;
    }
    return ssi;
}

MS_U16 MSB1236C_DTV_GetSignalStrengthWithRFPower(MS_U8 u8DemodIndex,float dBmValue)
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

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    status &= MSB1236C_ReadReg(u8DemodIndex,0x0990, &reg);
    demodState = reg;
    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        fPinput = dBmValue;

        // Get Modulation and code Rate
        MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x47 * 2), &u8Modulation);
        u8Modulation = (((MS_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
        if (u8Modulation >= 3) u8Modulation = 3;
        MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x47 * 2), &u8CodeRate);
        u8CodeRate = (((MS_U16) u8CodeRate) & (BIT2 | BIT1 | BIT0));
        if (u8CodeRate >= 5) u8CodeRate = 5;

        //Fix me, temp solution; demod doesn't lock, then use fref = -98 to compute SSI
        if (demodState != 0x09)
        {
            fPrel = fPinput - (-50.0f);
        }
        else
        {
            fPrel = fPinput - fDVBT2_SSI_Pref[u8Modulation][u8CodeRate];
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
    case E_DEVICE_DEMOD_DVB_C:
    {
        if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
        {
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB1236C_ReadReg(u8DemodIndex,0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB1236C_ReadReg(u8DemodIndex,0x0f24 + 1, &reg_tmp);
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
        }
        else
        {
            ch_power_ref = SsiPref[8].p_ref;
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
    default:
        ssi = 0;
        DMD_DBG(("Undefined!!!\n"));
        break;
    }
    return ssi;
}


/*================================================
==         demod  msb1236c setting dvbt/dvbt2 type and tuning interface
=================================================*/
MS_BOOL MSB1236C_LoadDSPCode(MS_U8 u8DemodIndex)
{
    MS_U32        u32Timeout = 0;
    MS_U8         u8DoneFlag = 0;
    MS_U8         u8Data = 0;
    MS_BOOL       bRet = TRUE;
    DMD_DBG(("[msb1236c][beg]MSB1236C_LoadDSPCode\n"));

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    DMD_DBG((">>>MSB1236C: Load DSP...\n"));
#if(TIMING_VERIFICATION == 1)
    tmm_9 = GIVE_ME_TIME;
#endif

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        u8Data = 1;
        break;
    case E_DEVICE_DEMOD_DVB_T:
        u8Data = 2;
        break;
    case E_DEVICE_DEMOD_DVB_C:
        u8Data = 3;
        break;
    default:
        u8Data = 2;
        return FALSE;
    }

    if (g_WO_SPI_FLASH == 1)
    {
        MS_U8 u8FirmwareType = MSB1236C_DVBT;

        // mask miu access for all and mcu
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x7f);
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        usleep(10);

        // Disable MCU
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x03);

        switch (u8Data)
        {
        case 1:
            u8FirmwareType = MSB1236C_DVBT2;
            break;
        case 2:
        default:
            u8FirmwareType = MSB1236C_DVBT;
            break;
        case 3:
            u8FirmwareType = MSB1236C_DVBC;
            break;
        }

#if (MSB1236_DVBT_ONLY == 1)
        bRet &= MSB1236C_LoadDSPCodeToSRAM_dvbt(u8DemodIndex);

        bRet &= MSB1236C_MEM_switch(u8DemodIndex,2);
#else
        bRet &= MSB1236C_LoadDSPCodeToSDRAM(u8DemodIndex,u8FirmwareType);

        bRet &= MSB1236C_LoadSdram2Sram(u8DemodIndex,u8FirmwareType);
#endif

        // enable miu access of mcu gdma
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x00);
    }
    else
    {
        DMD_DBG(("\r\n >>> MSB1236C_LoadDSPCode u8Data : %x\n", u8Data));
        // mask miu access for all and mcu
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x7f);
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        usleep(10);

        // Disable MCU
        MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x03);

        // Run code on bootloader
        MSB1236C_WriteReg(u8DemodIndex,0x1000 + (0x0c) * 2, 0x02);

        // Assign f/w code type to load => 0: boot-loader 1: dvbt2, 2: dvbt, 3: dvbc
        MSB1236C_WriteReg(u8DemodIndex,0x0900 + (0x4f) * 2, u8Data);


        // enable miu access of mcu gdma
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x00);

        do
        {
            MSB1236C_ReadReg(u8DemodIndex,0x0900 + (0x4f) * 2, &u8DoneFlag);

            if (u32Timeout++ > 500)
            {
                return FALSE;
            }

            usleep(1 * 1000);
            DMD_DBG(("\r\n >>>Init timeout u8DoneFlag:%x\n", u8DoneFlag));
        }
        while (u8DoneFlag != 0xaa);

        // mask miu access for all and mcu
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2 + 1, 0x7f);
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        usleep(10);

        // Disable MCU
        MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x03);

        // Run code on loaded firmware
        MSB1236C_WriteReg(u8DemodIndex,0x1000 + (0x0c) * 2, 0x05);

        do
        {
            MSB1236C_ReadReg(u8DemodIndex,0x1000 + (0x0c) * 2, &u8DoneFlag);

            if (u32Timeout++ > 500)
                return FALSE;

            usleep(1 * 1000);

        }
        while (u8DoneFlag != 0x05);

        // enable miu access of mcu gdma
        bRet &= MSB1236C_WriteReg(u8DemodIndex,0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        usleep(10);

        // Enable MCU
        MSB1236C_WriteReg(u8DemodIndex,0x0b00 + (0x19) * 2, 0x00);
    }
    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
        MSB1236C_DTV_DVBT2_DSPReg_Init(u8DemodIndex);
        break;
    case E_DEVICE_DEMOD_DVB_T:
        MSB1236C_DTV_DVBT_DSPReg_Init(u8DemodIndex);
        break;
    case E_DEVICE_DEMOD_DVB_C:
        //DTV_DVBC_DSPReg_Init();
        break;
    default:
        return FALSE;
    }

#if(TIMING_VERIFICATION == 1)
    tmm_10 = GIVE_ME_TIME;
#endif
    DMD_DBG(("[msb1236c][end]MSB1236C_LoadDSPCode\n"));
    return bRet;
}

EN_DEVICE_DEMOD_TYPE MSB1236C_GetCurrentDemodulatorType(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    return pMSB1236C->enDemodType ;
}

MS_BOOL MSB1236C_SetCurrentDemodulatorType(MS_U8 u8DemodIndex,EN_DEVICE_DEMOD_TYPE enDemodType)
{

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (enDemodType == pMSB1236C->enDemodType)
    {
        return TRUE;
    }
    pMSB1236C->enDemodType = enDemodType;
    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();

    if (MSB1236C_LoadDSPCode(u8DemodIndex) == FALSE)
    {
        DMD_ERR((">>>>MSB1236C:Fail\n"));
        return FALSE;
    }
    else
    {
        DMD_DBG((">>>MSB1236C:OK\n"));
    }
    return TRUE;
}

MS_BOOL MSB1236C_Active(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    UNUSED(bEnable);
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }


    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }
    else if ((pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T) || (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {

        MS_BOOL status = MSB1236C_WriteReg(u8DemodIndex,0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN

        if (status)
        {
            DMD_DBG((" @MSB1236C_MSB1236C_Active OK\n"));
        }
        else
        {
            DMD_ERR((" @MSB1236C_MSB1236C_Active NG\n"));
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
MS_BOOL MSB1236C_DTV_GetPlpBitMap(MS_U8 u8DemodIndex,MS_U8* u8PlpBitMap)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL   status = TRUE;
    MS_U8     u8Data = 0;
    MS_U8     indx = 0;

    status &= MSB1236C_ReadDspReg(u8DemodIndex,0x0120, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
        return FALSE;

    while (indx < 32)
    {
        status &= MSB1236C_ReadDspReg(u8DemodIndex,0x0100 + indx, &u8Data);
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

/************************************************************************************************
Subject:    Get PlpID List and PlpID Size for MPLP mode
Function:   MSB1236C_GetPlpIDList
Parmeter:   u8PlpID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB1236C_GetPlpIDList(MS_U8 u8DemodIndex)
{

    MS_U8 i, j, u8PlpBitMap[32];
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    pMSB1236C->PlpIDSize = 0;
    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));
    MSB1236C_DTV_GetPlpBitMap(u8DemodIndex,u8PlpBitMap);
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((u8PlpBitMap[i] >> j) & 1)
            {
                pMSB1236C->PlpIDList[pMSB1236C->PlpIDSize] = i * 8 + j;
                pMSB1236C->PlpIDSize++;
            }
        }
    }
    return TRUE;
}

/*************************************************************************************************
Subject:    Get GROUP_ID upon PLP_ID for MPLP mode
Function:   DTV_GetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB1236C_DTV_GetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL   status = TRUE;
    MS_U8 u8Data = 0;
    status &= MSB1236C_ReadDspReg(u8DemodIndex,0x0120, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        DMD_ERR(("[msb1236c]dvbt2 L1 not ready yet\n"));
        return FALSE;
    }
    MSB1236C_ReadDspReg(u8DemodIndex,0x00f0, &u8Data);

    if ((u8Data & BIT7) == 0x00)
    {
        DMD_ERR(("[msb1236c]dvbt2 is un-lock\n"));
        return FALSE;
    }
    // assign PLP-ID value
    status &= MSB1236C_WriteReg(u8DemodIndex,0x2700 + (0x78) * 2, u8PlpID);
    status &= MSB1236C_WriteReg(u8DemodIndex,0x2700 + (0x01) * 2 + 1, 0x01); // MEM_EN
    usleep(1000);
    status &= MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x79) * 2, u8GroupID);
    status &= MSB1236C_WriteReg(u8DemodIndex,0x2700 + (0x01) * 2 + 1, 0x00); // ~MEM_EN

    return status;
}


/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB1236C_DTV_SetPlpGroupID(MS_U8 u8DemodIndex,MS_U8 u8PlpID, MS_U8 u8GroupID)
{

    MS_BOOL   status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();
    DMD_DBG(("[start]\n"));

    // assign Group-ID and PLP-ID value (must be written in order)
    status &= MSB1236C_WriteDspReg(u8DemodIndex,0x0122, u8GroupID);
    status &= MSB1236C_WriteDspReg(u8DemodIndex,0x0121, u8PlpID);
    DMD_DBG(("[end, return %d]\n", status));
    return status;
}

MS_BOOL MSB1236C_Demod_SetScanTypeStatus(MS_U8 u8DemodIndex, MS_U8 status)
{
    printf("#### In MSB1236C_Demod_SetScanTypeStatus, %x %x\n", u8DemodIndex, status);
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    switch (status)
    {
    case 0:
        pMSB1236C->u8ScanStatus = 0;
        break;
    case 1:
        pMSB1236C->u8ScanStatus = 1;
        break;

    case 2:
        pMSB1236C->u8ScanStatus = 2;
        break;

    case 3:
        pMSB1236C->u8ScanStatus = 3;
        break;
    default:
        MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return FALSE;

    }
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_U8 MSB1236C_Demod_GetScanTypeStatus(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    return pMSB1236C->u8ScanStatus;
}

MS_BOOL MSB1236C_DTV_GetNextPLPID(MS_U8 u8DemodIndex, MS_U8 Index, MS_U8* pu8PLPID)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);

    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    *pu8PLPID = pMSB1236C->PlpIDList[Index];
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;

}

MS_BOOL MSB1236C_T2_SetPlpID(MS_U8 u8DemodIndex,MS_U8 u8PlpID)
{
    MS_BOOL bRet = FALSE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    {
        //MS_U32 u32Timeout;
        MS_U8 u8Data=0;
        MS_U16 u16RegAddress;
        MS_U8 u8LockState;
        MS_BOOL bCheckPass = FALSE;

        //u32Timeout = 1500;
        u16RegAddress = 0x0990;
        u8LockState = 0x09;
        MSB1236C_ReadReg(u8DemodIndex,u16RegAddress, &u8Data);
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        if ((u8PlpID != 0xFF) && (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2) && (bCheckPass))
        {

            MS_U16 u16Retry = 0;
            MS_U8 u8GroupId = 0;
            usleep(500);

            bRet = MSB1236C_DTV_GetPlpGroupID(u8DemodIndex,u8PlpID, &u8GroupId);
            while ((bRet == FALSE) && (u16Retry < 60))
            {
                u16Retry++;
                DMD_DBG(("DoSet_DVBT2 get groupid retry %d \n", u16Retry));
                usleep(100 * 1000);
                bRet = MSB1236C_DTV_GetPlpGroupID(u8DemodIndex,u8PlpID, &u8GroupId);
            }
            if (bRet == FALSE)
            {
                DMD_ERR(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_GetPlpGroupID(%d) Error \n", u8PlpID));
                return FALSE;
            }

            bRet = MSB1236C_DTV_SetPlpGroupID(u8DemodIndex,u8PlpID, u8GroupId);
            if (bRet == FALSE)
            {
                DMD_ERR(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_SetPlpGroupID(%d,%d) Error", u8PlpID, u8GroupId));
                return FALSE;
            }
        }
    }
    return TRUE;
}


// DVBT2 95~101ms, DVBT 38~39ms
MS_BOOL MSB1236C_DTV_SetFrequency(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U8 eBandWidth)
{
    MS_U8 bw_t2 = 0, bw_t = 0;
    MS_U8 u8PlpID;
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    MS_U32 u32Frequency;
#endif
    MS_U32 u32TunerIF = 0;
    DEMOD_MS_INIT_PARAM* pInitParam = (pMSB1236C_InitParam + u8DemodIndex);

    u8PlpID = pParam->TerParam.u8PlpID;
#if (DMD_DEBUG_OPTIONS & DMD_EN_DBG)
    u32Frequency = pParam->u32Frequency;
#endif

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    DMD_DBG(("\n[%s]->freq=%"DTC_MS_U32_d",bdwh=%d,PlpID %d,type %s\n", __FUNCTION__, u32Frequency, eBandWidth, u8PlpID, E_DEVICE_DEMOD_DVB_T == pMSB1236C->enDemodType ? "T" : "T2"));


    pMSB1236C->bFECLock = FALSE;
    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();

#if(TIMING_VERIFICATION == 1)
    tmm_11 = GIVE_ME_TIME;
#endif

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

    MSB1236C_IIC_Bypass_Mode(u8DemodIndex, TRUE);
    if(!pInitParam->pstTunertab->GetTunerIF(u8DemodIndex, &u32TunerIF))
    {
        if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
            u32TunerIF = (MS_U32)(T2_FC_L_VAL | (T2_FC_H_VAL << 8));
        else
            u32TunerIF = (MS_U32)(FC_L | (FC_H << 8));

        DMD_DBG(("MSB1236C Get Tuner IF FAIL , use default IF = %d kHz\n", (int)u32TunerIF));
    }
    else
    {
        DMD_DBG(("MSB1236C Get Tuner IF OK, IF = %d kHz\n", (int)u32TunerIF));
    }
    MSB1236C_IIC_Bypass_Mode(u8DemodIndex, FALSE);

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        DMD_DBG(("\nsetfreq T2\n"));
        MSB1236C_Reset(u8DemodIndex);
        usleep(500);
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_BW, bw_t2);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_PLP_ID, u8PlpID);
        //MSB1236C_WriteDspReg((MS_U16)T2_TH_A_NP1, T2_TH_A_NP1_VAL);
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_IF_AGC_INV_PWM_EN, T2_IF_AGC_INV_PWM_EN_VAL);
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_FC_L, (MS_U8)(u32TunerIF & 0xff));
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_FC_H, (MS_U8)((u32TunerIF & 0xff00) >> 8));
        MSB1236C_WriteReg(u8DemodIndex,0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }
    else if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        DMD_DBG(("\nsetfreq T\n"));
        MSB1236C_Reset(u8DemodIndex);
        usleep(500);

        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)T_CONFIG_BW, bw_t);      // BW: 1->6M, 2->7M, 3->8M

        if(pParam->TerParam.eLevelSel == (DEMOD_EN_TER_LEVEL_SEL)TER_LP_SEL)
            MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)T_CONFIG_LP_SEL, 1);
        else
            MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)T_CONFIG_LP_SEL, 0);

        pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_CONFIG_BW - (MS_U8)T_OPMODE_RFAGC_EN] = (MS_U8)bw_t;
        pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MSB1236C_DTV_DVBT_DSPReg_CRC(u8DemodIndex);
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U8)T_PARAM_CHECK_SUM, pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]);    // BW: 1->6M, 2->7M, 3->8M
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)T_CONFIG_FC_L, (MS_U8)(u32TunerIF & 0xff));
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)T_CONFIG_FC_H, (MS_U8)((u32TunerIF & 0xff00) >> 8));
        MSB1236C_Active(u8DemodIndex,1);
    }

#if(TIMING_VERIFICATION == 1)
    tmm_12 = GIVE_ME_TIME;
    show_timer();
#endif
    return TRUE;
}

/*================================================
==         demod  msb1236c getting dvbt/dvbt2 param interface
=================================================*/
MS_BOOL MSB1236C_DTV_DVB_T2_Get_L1_Parameter(MS_U8 u8DemodIndex,MS_U16 * pu16L1_parameter, E_SIGNAL_TYPE eSignalType)
{
    MS_U8         u8Data = 0;

    if (MSB1236C_Lock(u8DemodIndex,E_DEVICE_DEMOD_DVB_T2, COFDM_FEC_LOCK) == TRUE)
    {

        if (eSignalType == TS_MODUL_MODE)
        {

            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x47 * 2), &u8Data) == FALSE)
                return FALSE;

            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT5 | BIT4 | BIT3)) >> 3;

        }

        else  if (eSignalType == TS_CODE_RATE)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x47 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT2 | BIT1 | BIT0));

        }
        else if (eSignalType == TS_PREAMBLE)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT4)) >> 4;
        }
        else if (eSignalType == TS_S1_SIGNALLING)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT3 | BIT2 | BIT1)) >> 1;
        }
        else if (eSignalType == TS_PILOT_PATTERN)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x36 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0x0F);
        }
        else if (eSignalType == TS_BW_EXT)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT0));
        }
        else if (eSignalType == TS_PAPR_REDUCTION)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & 0xF0) >> 4;
        }
        else if (eSignalType == TS_OFDM_SYMBOLS_PER_FRAME)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x3C * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (MS_U16) u8Data;
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x3C * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter |= (((MS_U16) u8Data) & 0x0F) << 8;
        }
        else if (eSignalType == TS_T2_FFT_VALUE)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            if ((u8Data & (BIT3 | BIT2)) >> 2)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT7 | BIT6 | BIT5)) >> 5;
        }
        else if (eSignalType == TS_T2_GUARD_INTERVAL)
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x2700 + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16L1_parameter  = (((MS_U16) u8Data) & (BIT3 | BIT2 | BIT1)) >> 1;
        }
        else
        {
            return FALSE;
        }

        return TRUE;

    }
    return FALSE;
}

MS_BOOL MSB1236C_DTV_DVB_T_GetSignalTpsInfo(MS_U8 u8DemodIndex,MS_U16 *TPS_parameter)
{
    MS_U8 u8Temp=0;

    if (MSB1236C_ReadReg(u8DemodIndex,0x0F00 + 0x22, &u8Temp) == FALSE)
        return FALSE;

    if ((u8Temp & 0x02) != 0x02)
    {
        return FALSE; //TPS unlock
    }
    else
    {
        if (MSB1236C_ReadReg(u8DemodIndex,0x0F00 + 0x24, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter = u8Temp & 0x03;         //Constellation (b1 ~ b0) 0:QPSK, 1:16-QAM, 2:64-QAM
        *TPS_parameter |= (u8Temp & 0x70) >> 1; //Hierarchy (b5 ~ b3)
        //0: no hierarchy
        //1: alpha = 1
        //2: alpha = 2
        //3: alpha = 4

        if (MSB1236C_ReadReg(u8DemodIndex,0x0F00 + 0x25, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x07) << 6; //LP Code Rate (b8 ~ b6)
        //0: 1/2
        //1: 2/3
        //2: 3/4
        //3: 5/6
        //4: 7/8
        *TPS_parameter |= (MS_U16)(u8Temp & 0x70) << 5; //HP Code Rate (b11 ~ b9)

        if (MSB1236C_ReadReg(u8DemodIndex,0x0F00 + 0x26, &u8Temp) == FALSE)
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x03) << 12; //GI (b13 ~ b12)
        //0: 1/32
        //1: 1/16
        //2: 1/8
        //3: 1/4
        *TPS_parameter |= (MS_U16)(u8Temp & 0x30) << 10;  //FFT ( b14~b15)
        //0: 2k mode
        //1: 8k mode
        //2: 4k mode

        //if (MSB1236C_ReadReg(u8DemodIndex,0x0F00 + 0x0C, &u8Temp) == FALSE)
        //   return FALSE;

        //*TPS_parameter |= (MS_U16)(u8Temp & 0x08) << 12; //Priority(bit 15)

    }
    return TRUE;
}

MS_BOOL MSB1236C_DTV_DVB_T2_GetSignalL1Info(MS_U8 u8DemodIndex,MS_U64 *L1_Info)
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
    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_MODUL_MODE) == FALSE)
    {
        DMD_ERR(("TS_MODUL_MODE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info = (MS_U64)(u16Data);
    DMD_DBG(("Constellation:%s\n", cConStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_CODE_RATE) == FALSE)
    {
        DMD_ERR(("TS_CODE_RATE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info |= (MS_U64)(u16Data << 3);
    DMD_DBG(("Code Rate:%s\n", cCRStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_T2_GUARD_INTERVAL) == FALSE)
    {
        DMD_ERR(("TS_T2_GUARD_INTERVAL Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info |= (MS_U64)(u16Data << 6);
    DMD_DBG(("GI:%s\n", cGIStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_T2_FFT_VALUE) == FALSE)
    {
        DMD_ERR(("TS_T2_FFT_VALUE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *L1_Info |= (MS_U64)(u16Data << 9);
    DMD_DBG(("FFT:%s\n", cFFTStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_PREAMBLE) == FALSE)
    {
        DMD_ERR(("TS_PREAMBLE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x01;
    *L1_Info |= (MS_U64)(u16Data << 12);
    DMD_DBG(("Preamble:%s\n", cPreAStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_S1_SIGNALLING) == FALSE)
    {
        DMD_ERR(("TS_S1_SIGNALLING Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x03;
    if (u16Data > 2)
        u16Data = 3;
    *L1_Info |= (MS_U64)(u16Data << 13);
    DMD_DBG(("S1Signalling:%s\n", cS1SStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_PILOT_PATTERN) == FALSE)
    {
        DMD_ERR(("TS_PILOT_PATTERN Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x0F;
    if (u16Data > 7)
        u16Data = 8;
    *L1_Info |= (MS_U64)(u16Data << 15);
    DMD_DBG(("PilotPattern:%s\n", cPPSStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_BW_EXT) == FALSE)
    {
        DMD_ERR(("TS_BW_EXT Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x01;
    *L1_Info |= (MS_U64)(u16Data << 19);
    DMD_DBG(("BW EXT:%s\n", cBWStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_PAPR_REDUCTION) == FALSE)
    {
        DMD_ERR(("TS_PAPR_REDUCTION Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    if (u16Data > 3)
        u16Data = 4;
    *L1_Info |= (MS_U64)(u16Data << 20);
    DMD_DBG(("PAPR:%s\n", cPAPRStr[u16Data]));

    if (MSB1236C_DTV_DVB_T2_Get_L1_Parameter(u8DemodIndex,&u16Data, TS_OFDM_SYMBOLS_PER_FRAME) == FALSE)
    {
        DMD_ERR(("TS_OFDM_SYMBOLS_PER_FRAME Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0xFFF;
    *L1_Info |= (MS_U64)(u16Data << 23);
    DMD_DBG(("OFDM Symbols:%u\n", u16Data));

    return bRet;
}

MS_U16 MSB1236C_DTV_GetCellID(MS_U8 u8DemodIndex)
{
    MS_U8 id = 0;
    MS_U8 status = TRUE;
    MS_U16  cell_id  = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return 0;
    }
    else if ((pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
             || (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_C))
    {
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0ffe, 0x01);

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f2b, &id);
        cell_id = (MS_U16)id;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f2a, &id);
        cell_id |= (MS_U16)id << 8;

        status &= MSB1236C_WriteReg(u8DemodIndex,0x0ffe, 0x00);

        if (status)
        {
            DMD_DBG((" @MSB1236C_GetCellId OK\n"));
            return  TRUE;
        }
        else
        {
            DMD_ERR((" @MSB1236C_GetCellId NG\n"));
            return FALSE;
        }
    }
    return cell_id;

}

MS_BOOL MSB1236C_DTV_GetSNR_F(MS_U8 u8DemodIndex,float *fSNR)
{
    MSB1236C_DTV_GetSNR(u8DemodIndex);
    *fSNR = g_msb1236c_fSNR;
    return TRUE;
}

MS_BOOL MSB1236C_DTV_Get_Current_Plp_Id(MS_U8 u8DemodIndex,MS_U8 *plp_id)
{
    MS_BOOL  rbet = TRUE;
    MS_U8    reg=0;

    rbet &= MSB1236C_ReadDspReg(u8DemodIndex,(MS_U16)E_T2_PLP_ID, &reg);
    *plp_id = reg;
    return rbet;
}

void MSB1236C_DTV_ControlTsOutput(MS_U8 u8DemodIndex,MS_BOOL bEnable)
{
    if (bEnable)
    {
        MSB1236C_WriteReg(u8DemodIndex,0x0900 + (0x2D * 2), 0xFF);
    }
    else
    {
        MSB1236C_WriteReg(u8DemodIndex,0x0900 + (0x2D * 2), 0x00);
    }
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U16 MSB1236C_Get_FreqOffset(MS_U8 u8DemodIndex,float *pFreqOff, MS_U8 u8BW)
{
    MS_U8 status = TRUE;
    //DVB-T
    float   N, FreqB;
    float   FreqCfoTd, FreqCfoFd, FreqIcfo;
    MS_U32  RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8 reg_frz=0, reg=0, reg_debug_sel=0;
    MS_S32 RegTotalCfo;
    EN_DEVICE_DEMOD_TYPE eSystems;

    FreqB = (float)u8BW * 8.0 / 7.0;
    eSystems = MSB1236C_GetCurrentDemodulatorType(u8DemodIndex);
    DMD_DBG(("\nBW = %d\n", u8BW));
    if (eSystems == E_DEVICE_DEMOD_DVB_T2)
    {
        //debug select
        status &= MSB1236C_ReadReg(u8DemodIndex,0x3E5E, &reg_debug_sel);
        reg_debug_sel &= 0xCF;
        reg_debug_sel |= 0x10;
        status &= MSB1236C_WriteReg(u8DemodIndex,0x3E5E, reg_debug_sel);

        // set freeze
        status &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x3E00 + (0x02) * 2, 0x8000);
        status &= MSB1236C_ReadReg(u8DemodIndex,0x3E5B, &reg);
        RegTotalCfo = reg & 0x0F;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x3E5A, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x3E59, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x3E58, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;

        // release freeze
        status &= MSB1236C_WriteReg2bytes(u8DemodIndex, 0x3E00 + (0x02) * 2, 0x0000);

        if (RegTotalCfo >= 0x4000000)
            RegTotalCfo = RegTotalCfo - 0x8000000;

        *pFreqOff = (float)RegTotalCfo * FreqB * 0.00000745;

        DMD_DBG(("$$$$$$$$$$$$$$$$$$$$  DVB-T2 CFOE = %f\n", *pFreqOff));
        return status;
    }
    else if (eSystems == E_DEVICE_DEMOD_DVB_T)
    {
        // freeze
        status &= MSB1236C_ReadReg(u8DemodIndex,0x0E04 + 1, &reg_frz);
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0E04 + 1, reg_frz | 0x80);

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0E28 + 2, &reg);
        RegCfoTd = reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0E28 + 1, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0E28, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        // release
        status &= MSB1236C_ReadReg(u8DemodIndex,0x0E04 + 1, &reg_frz);
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0E04 + 1, reg_frz | 0x80);

        FreqCfoTd = (float)RegCfoTd;

        if (RegCfoTd & 0x800000)
            FreqCfoTd = FreqCfoTd - (float)0x1000000;

        FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0ffe, &reg_frz);
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0ffe, reg_frz | 0x01);

        status &= MSB1236C_WriteReg(u8DemodIndex,0x0fff, 0x01);

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f33, &reg);
        RegCfoFd = reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f32, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f31, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        FreqCfoFd = (float)RegCfoFd;

        if (RegCfoFd & 0x800000)
            FreqCfoFd = FreqCfoFd - (float)0x1000000;

        FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f19, &reg);
        RegIcfo = reg & 0x07;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f18, &reg);
        RegIcfo = (RegIcfo << 8) | reg;

        FreqIcfo = (float)RegIcfo;

        if (RegIcfo & 0x400)
            FreqIcfo = FreqIcfo - (float)0x800;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x0f26, &reg);
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
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0ffe, reg_frz & (~0x01));
        status &= MSB1236C_WriteReg(u8DemodIndex,0x0fff, 0x01);

        *pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd) / 1000;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x3E5E, &reg);

        if (reg & 0x01)
            *pFreqOff = -*pFreqOff;

        DMD_DBG(("$$$$$$$$$$$$$$$$$$$$  DVB-T CFOE = %f\n", *pFreqOff));

    }
    else
        ;

    if (status)
    {
        DMD_DBG((" @MSB1236C_Get_FreqOffset OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_ERR((" @MSB1236C_Get_FreqOffset NG\n"));
        return FALSE;
    }
}

MS_U8 MSB1236C_Get_Packet_Error(MS_U8 u8DemodIndex, MS_U16 *u16_data)
{
    MS_U8     status = TRUE;
    MS_U8     reg = 0, reg_frz;
    MS_U16      PktErr = 0;

    if (E_DEVICE_DEMOD_DVB_T2 == MSB1236C_GetCurrentDemodulatorType(u8DemodIndex))
    {
        //freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x2604, 0x01);
        //read packet error
        status &= MSB1236C_ReadReg(u8DemodIndex,0x265B, &reg);
        PktErr = reg;
        status &= MSB1236C_ReadReg(u8DemodIndex,0x265A, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;
        //release
        status &= MSB1236C_WriteReg(u8DemodIndex,0x2604, 0x00);
    }
    else
    {
        reg_frz = 0;
        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_ReadReg(u8DemodIndex,0x1132, &reg_frz);
        status &= MSB1236C_WriteReg(u8DemodIndex,0x1132, reg_frz | 0x80);

        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //         [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB1236C_ReadReg(u8DemodIndex,0x113F, &reg);
        PktErr = reg;

        status &= MSB1236C_ReadReg(u8DemodIndex,0x113E, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_WriteReg(u8DemodIndex,0x1132, reg_frz);

        DMD_DBG(("===================>MSB PktErr = %d \n ", (int)PktErr));
    }
    if (status)
    {
        DMD_DBG((" @MSB1236C_Get_Packet_Error OK\n"));
        return  TRUE;
    }
    else
    {
        DMD_ERR((" @MSB1236C_Get_Packet_Error NG\n"));
        return FALSE;
    }
}

MS_U16 MSB1236c_CHIP_MATCH_TABLE[] =
{
    //Kaiser, Kaiserin, Keltic, Kronus
    0x56,       0x41,     0x72,  0x2F, 0x75,
};

MS_BOOL MSB1236C_Set_bonding_option(MS_U8 u8DemodIndex,MS_U16 u16ChipID)
{
    MS_BOOL  bRet = TRUE;
    MS_U8    u8Idx;
    MS_U8    u8MatchFlag  = 0;
    MS_U8    u8Data  = 0;
    MS_U32    u32_timeout = 0;

    DMD_DBG(("%s(),%d\n",__func__,__LINE__));

    //printf("MSB123xc_set_bonding_option u16ChipID %x\n", u16ChipID);

    for (u8Idx = 0 ; u8Idx < (sizeof( MSB1236c_CHIP_MATCH_TABLE)/sizeof(MS_U16)) ; u8Idx++)
    {
        if(u16ChipID == MSB1236c_CHIP_MATCH_TABLE[u8Idx])
        {
            u8MatchFlag = 0x01;
            break;
        }
        else
        {
            u8MatchFlag = 0x00;
        }
    }

    if (MSB1236C_I2C_CH_Reset(u8DemodIndex,3) == FALSE)
    {
        DMD_ERR((">>>MSB1236c CH3 Reset:Fail\n"));
        return FALSE;
    }

    // MSB123xc : 0x0902[8]=0 , 0x0902[0]=0;
    // MSB1236c : 0x0902[8]=1 , 0x0902[0]=1; (before overwrite, SDRAM not enable)
    //                  0x0902[8]=1 , 0x0902[0]=0; (after overwrite, SDRAM enable)
    // check bonding value, 0x0902[8]
    bRet &= MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x02)*2+1, &u8Data);
    if((u8Data & 0x01) == 0x01) //for MSB1236C
    {
        if(u8MatchFlag == 0x01)
        {
            //check overwrite or not
            //0x0902[0] : reg_bonding[0]
            bRet &= MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x02)*2, &u8Data);
            if((u8Data & 0x01) != 0x00)
            {
                //0x0905[0] : reg_bond_ov_en[0] = 1
                //0x0905[8] : reg_bond_ov[0] = 0
                // set overwrite enable
                bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0900+(0x05)*2, 0x01);
                // set overwrite value
                bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0900+(0x05)*2+1, 0x00);
            }

            do
            {
                bRet &= MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x02)*2, &u8Data);
                if(u32_timeout++ > 500)
                {
                    DMD_ERR(("@msb1236c, Set bonding option failure.!!!\n"));
                    return FALSE;
                }
            }
            while((u8Data & 0x01) == 0x01);

            DMD_DBG(("@ Set bonding option for MSB1236c \n"));
        }
        else
        {
            return FALSE;
        }
    }
    else  // for MSB123xc
    {
        //check overwrite or not
        //0x0902[0] : reg_bonding[0]
        bRet &= MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x02)*2, &u8Data);
        if((u8Data & 0x01) != 0x00)
        {
            //0x0905[0] : reg_bond_ov_en[0] = 1
            //0x0905[8] : reg_bond_ov[0] = 0
            // set overwrite enable
            bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0900+(0x05)*2, 0x01);
            // set overwrite value
            bRet &= MSB1236C_WriteReg(u8DemodIndex,0x0900+(0x05)*2+1, 0x00);
        }

        do
        {
            bRet &= MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x02)*2, &u8Data);
            if(u32_timeout++ > 500)
            {
                DMD_ERR(("@msbMSB123xc, Set bonding option failure.!!!\n"));
                return FALSE;
            }
        }
        while((u8Data & 0x01) == 0x01);

        DMD_DBG(("@ Set bonding option for MSB123xc \n"));
    }

    if (!bRet) DMD_ERR(("%s %d Error\n", __func__, __LINE__));
    return bRet;
}


MS_U16 MDrv_Demod_Get_FreqOffset(MS_U8 u8DemodIndex, float *pFreqOff, MS_U8 u8BW)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1236C_Get_FreqOffset(u8DemodIndex,pFreqOff, u8BW);

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_U8 MSB1236C_Demod_Get_Packet_Error(MS_U8 u8DemodIndex,MS_U16 *u16_data)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1236C_Get_Packet_Error(u8DemodIndex,u16_data);

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetScanTypeStatus(MS_U8 u8DemodIndex, MS_U8 status)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    switch (status)
    {
    case 0:
        pMSB1236C->u8ScanStatus = 0;
        break;
    case 1:
        pMSB1236C->u8ScanStatus = 1;
        break;

    case 2:
        pMSB1236C->u8ScanStatus = 2;
        break;

    case 3:
        pMSB1236C->u8ScanStatus = 3;
        break;
    default:
        MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return FALSE;

    }
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_U8 MDrv_Demod_GetScanTypeStatus(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    return pMSB1236C->u8ScanStatus;
}

MS_BOOL MDrv_Demod_InitParameter(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!"));
        return FALSE;
    }
    pMSB1236C->u8ScanStatus = 0;
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_U8 MDrv_Demod_GetPlpIDSize(MS_U8 u8DemodIndex)
{
    MS_U8    u8Size = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!"));
        return FALSE;
    }
    u8Size = pMSB1236C->PlpIDSize;
    DMD_DBG(("[end],PlpID size %d", u8Size));
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return u8Size;
}

MS_U8 MDrv_Demod_GetPlpIDList(MS_U8 u8DemodIndex, MS_U8* u8PlpID)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return 0;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return 0;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if (FALSE == MSB1236C_GetPlpIDList(u8DemodIndex))
        pMSB1236C->PlpIDSize = 0;

    //memcpy(u8PlpID , pMSB1236C->PlpIDList, pMSB1236C->PlpIDSize);
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    *u8PlpID = pMSB1236C->PlpIDSize;

    return pMSB1236C->PlpIDSize;
}

MS_BOOL MDrv_Demod_ClearPlpList(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!"));
        return FALSE;
    }

    pMSB1236C->PlpIDSize = 0;

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return TRUE;
}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_U8 u8DemodIndex, MS_BOOL bOn)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    MS_BOOL bret = TRUE;
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB123X error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        bret &= MSB1236C_I2C_CH_Reset(u8DemodIndex,3);
        bret &= MSB1236C_WriteReg(u8DemodIndex,0x0951, 0x00);
        bret &= MSB1236C_WriteReg(u8DemodIndex,0x090C, 0x10);
        bret &= MSB1236C_WriteReg(u8DemodIndex,0x090E, 0x10);
        if(bOn)
            bret &= MSB1236C_WriteReg(u8DemodIndex,0x0910, 0x10);
        else
            bret &= MSB1236C_WriteReg(u8DemodIndex,0x0910, 0x00);
        return bret;
    }
    /*
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    */
    MSB1236C_IIC_Bypass_Mode(u8DemodIndex,bOn);

    //MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetCurrentDemodType(MS_U8 u8DemodIndex, MS_U8  type)
{
    if (E_DEMOD_TYPE_T == type)
    {
        return MSB1236C_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_T);
    }
    else
    {
        return MSB1236C_SetCurrentDemodulatorType(u8DemodIndex,E_DEVICE_DEMOD_DVB_T2);
    }
}

MS_U8 MDrv_CofdmDmd_GetCurrentDemodType(MS_U8 u8DemodIndex)
{
    if (E_DEVICE_DEMOD_DVB_T == MSB1236C_GetCurrentDemodulatorType(u8DemodIndex))
        return E_DEMOD_TYPE_T;
    else
        return E_DEMOD_TYPE_T2;
}

MS_BOOL MDrv_Demod_Open(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB1236C->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_U8 u8DemodIndex, MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    pMSB1236C->bOpen = FALSE;
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return TRUE;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MDrv_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
    MS_BOOL bret = FALSE;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pfSNR = 0;
        if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus(u8DemodIndex))
        {
            bret  = MSB1236C_DTV_GetSNR_F(u8DemodIndex, pfSNR);
        }

    }
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return bret;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MDrv_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MS_BOOL bret = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);

    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetBER:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    bret = MSB1236C_DTV_GetPostBER(u8DemodIndex, pfBER);
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return bret;
}

MS_BOOL MDrv_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    DEMOD_MS_INIT_PARAM* pInitParam = (pMSB1236C_InitParam + u8DemodIndex);
    int PowerLevel = 0;

    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus(u8DemodIndex))
        {
            MSB1236C_IIC_Bypass_Mode(u8DemodIndex, TRUE);
            pInitParam->pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, &PowerLevel);
            MSB1236C_IIC_Bypass_Mode(u8DemodIndex, FALSE);
            *ps32Signal = (MS_S32)PowerLevel;
        }

    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    DEMOD_MS_INIT_PARAM* pInitParam = (pMSB1236C_InitParam + u8DemodIndex);
    int PowerLevel = 0;
    float fSignal = 0;


    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus(u8DemodIndex))
        {
            MSB1236C_IIC_Bypass_Mode(u8DemodIndex, TRUE);
            pInitParam->pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_GET_POWER_LEVEL, &PowerLevel);
            MSB1236C_IIC_Bypass_Mode(u8DemodIndex, FALSE);
            fSignal = (float)PowerLevel;
            if(fSignal != 0)
               *pu16SSI = MSB1236C_DTV_GetSignalStrengthWithRFPower(u8DemodIndex,fSignal);
            else
               *pu16SSI  = MSB1236C_DTV_GetSignalStrength(u8DemodIndex);
        }

        if (*pu16SSI > 100)
            *pu16SSI = 100;
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}


MS_BOOL MDrv_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{

    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pu16quality = 0;
        if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus(u8DemodIndex))
        {
            *pu16quality  = MSB1236C_DTV_GetSignalQuality(u8DemodIndex);
        }

    }
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return TRUE;
}


EN_LOCK_STATUS MDrv_Demod_GetT2LockStatus(MS_U8 u8DemodIndex)
{
    EN_LOCK_STATUS LockStatus = E_DEMOD_UNLOCK;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLockStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        LockStatus = MSB1236C_DTV_DVB_T_GetLockStatus(u8DemodIndex);
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return LockStatus;
}

MS_BOOL MDrv_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    DEMOD_MS_INIT_PARAM* pInitParam = (pMSB1236C_InitParam + u8DemodIndex);

    if ((!ptrMSB1236C) || (!pMSB1236C_InitParam))
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    usleep(10);
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *peLockStatus = MSB1236C_DTV_DVB_T_GetLockStatus(u8DemodIndex);
    }

    if(((*peLockStatus) == E_DEMOD_LOCK))
    {
        MDrv_Demod_I2C_ByPass(u8DemodIndex, TRUE);
        pInitParam->pstTunertab->Extension_Function(u8DemodIndex,TUNER_EXT_FUNC_RESET_RFAGC, NULL);
        MDrv_Demod_I2C_ByPass(u8DemodIndex, FALSE);
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

/****************************************************************************
**function:reset bw and Plpid to demod when plp changed
**
**
****************************************************************************/
MS_BOOL MDrv_Demod_T2MI_Restart(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    MS_U8    PlpId=0, BandWidth;

    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    if (E_DEVICE_DEMOD_DVB_T2 != MSB1236C_GetCurrentDemodulatorType(u8DemodIndex))
    {
        MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return TRUE;
    }

    MSB1236C_ReadDspReg(u8DemodIndex,(MS_U16)E_T2_PLP_ID, &PlpId);
    if (pMSB1236C->u8CurrPlpID != PlpId)
    {
        switch (pMSB1236C->enCurrBW)
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
        DMD_ERR(("Curr_PLP_ID=%d, GET_PLP_ID=%d\n", pMSB1236C->u8CurrPlpID, PlpId));
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_BW, BandWidth);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB1236C_WriteDspReg(u8DemodIndex,(MS_U16)E_T2_PLP_ID, pMSB1236C->u8CurrPlpID);
        MSB1236C_WriteReg(u8DemodIndex,0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam,MS_U32 u32BroadCastType)
{
    MS_U32              dwError = TRUE;
    MS_U8               BandWidth = DEMOD_BW_MODE_8MHZ;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (!ptrMSB1236C)
    {
        DMD_ERR(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        DMD_ERR(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if((u32BroadCastType != DVBT) && (u32BroadCastType != DVBT2))
    {
        DMD_ERR(("BroadCast Type not Support !!!\n"));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB1236C->enTuneBw = pParam->TerParam.eBandWidth;

    DMD_DBG(("\n[%s]tuner demod Freq=%"DTC_MS_U32_d", Bw=%d,plpid=%d\n", __FUNCTION__, pParam->u32Frequency, pParam->TerParam.eBandWidth, pParam->TerParam.u8PlpID));

    if (DEMOD_BW_MODE_1_7MHZ == pParam->TerParam.eBandWidth)
    {
        MDrv_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T2);
    }
    else if ((E_DEMOD_TYPE_T == pParam->TerParam.u8ScanType) && (pMSB1236C->u8CurrScanType != pParam->TerParam.u8ScanType))
    {
        MDrv_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T);
    }
    else if ((E_DEMOD_TYPE_T2 == pParam->TerParam.u8ScanType) && (pMSB1236C->u8CurrScanType != pParam->TerParam.u8ScanType))
    {
        MDrv_Demod_SetCurrentDemodType(u8DemodIndex,E_DEMOD_TYPE_T2);
    }


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

    dwError = MSB1236C_DTV_SetFrequency(u8DemodIndex,pParam, BandWidth);

    if ((TRUE == pMSB1236C->bFECLock) && (pMSB1236C->u8CurrScanType == pParam->TerParam.u8ScanType) && \
            (E_DEMOD_TYPE_T2 == pMSB1236C->u8CurrScanType) && (pMSB1236C->u32CurrFreq == pParam->u32Frequency) && \
            (pMSB1236C->enCurrBW == pParam->TerParam.eBandWidth))
    {
    }
    else
    {
        //set freq and bw to tuner
        //MSB1236C_IIC_Bypass_Mode(TRUE);
        //MDrv_Tuner_SetTuner(pParam->u32Frequency, BandWidth);
        //MSB1236C_IIC_Bypass_Mode(FALSE);
        dwError = MSB1236C_DTV_SetFrequency(u8DemodIndex,pParam, BandWidth);
    }
    pMSB1236C->u8CurrScanType = pParam->TerParam.u8ScanType;

    pMSB1236C->u32CurrFreq = pParam->u32Frequency;
    pMSB1236C->enCurrBW = pParam->TerParam.eBandWidth;
    pMSB1236C->u8CurrPlpID = pParam->TerParam.u8PlpID;


    if (E_DEMOD_TYPE_T2 == MDrv_CofdmDmd_GetCurrentDemodType(u8DemodIndex))
    {
        dwError &= MSB1236C_T2_SetPlpID(u8DemodIndex,pParam->TerParam.u8PlpID);
    }

#if MSB1236C_NO_CHANNEL_CHECK
    pMSB1236C->noChannelStable = FALSE;
    pMSB1236C->t2NoChannelFlag = FALSE;
    pMSB1236C->tNoChannelFlag = FALSE;
    pMSB1236C->u32LockTime = MsOS_GetSystemTime();
#endif

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return dwError;
}

void MDrv_Demod_CtrlResetDJBFlag(MS_U8 u8DemodIndex, MS_U8 Ctrl)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {

    }
}

MS_BOOL MDrv_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_U8     status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);
    DEMOD_MS_INIT_PARAM* pInitParam = (pMSB1236C_InitParam + u8DemodIndex);
    MS_IIC_PORT ePort;

    if (pMSB1236C->bInited)
        return TRUE;

    if(pParam == NULL)
        return FALSE;
    pInitParam->pstTunertab = pParam->pstTunertab;
#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    DMD_DBG(("MSPI path is set by detected result\n"));
#else
    pMSB1236C->u32DmxInputPath = pParam->u32DmxInputPath;
#endif
#endif

    pMSB1236C->bInited = TRUE;
    pMSB1236C->bOpen = TRUE;
    pMSB1236C->bLoaded = FALSE;
    pMSB1236C->u8SdramCode = 0;
    pMSB1236C->u8sramCode = 0;
    pMSB1236C->PlpIDSize = 0;
    pMSB1236C->bFECLock = FALSE;
    pMSB1236C->pMSB1236CLib = MSB1236C_LIB ;
    pMSB1236C->u32MSB1236ClibSize = sizeof(MSB1236C_LIB);
    pMSB1236C->pMSB1236C_RegDVBT = MSB1236C_DVBT_DSPREG_TABLE ;
    pMSB1236C->u8ScanStatus = 0;
    pMSB1236C->u32CurrFreq = 0;
    pMSB1236C->enCurrBW = DEMOD_BW_MODE_8MHZ;
    pMSB1236C->enTuneBw = DEMOD_BW_MODE_8MHZ;
    pMSB1236C->u8CurrPlpID = 0xff;
    pMSB1236C->enDemodType = E_DEVICE_DEMOD_DVB_T;
    pMSB1236C->u8CurrScanType = E_DEMOD_TYPE_T;

    pMSB1236C->s32_MSB1236C_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2", MSOS_PROCESS_SHARED);
    if (pMSB1236C->s32_MSB1236C_Mutex < 0)
    {
        DMD_ERR(("creat MSB1236C Mutex error!!!\n"));
        return FALSE;
    }

    ePort = getI2CPort(u8DemodIndex);
    if (ePort < E_MS_IIC_PORT_1)
    {
        pMSB1236C->fpMSB123xc_I2C_Access = msb1236c_I2C_Access;
    }
    else if (ePort < E_MS_IIC_PORT_2)
    {
        pMSB1236C->fpMSB123xc_I2C_Access = msb1236c_I2C1_Access;
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }


    if ( MSB1236C_Set_bonding_option( u8DemodIndex, 0x75 ) == FALSE )
    {
        return TRUE;
    }

    status = MSB1236C_I2C_CH_Reset(u8DemodIndex,3);
    if (!status)
    {
        DMD_ERR(("[%s]  MSB1236C I2C CH reset error!!!\n", __FUNCTION__));
    }

    status = MSB1236C_HW_init(u8DemodIndex);
    if (!status)
    {
        pMSB1236C->bInited = FALSE;
        pMSB1236C->bOpen = FALSE;
        pMSB1236C->bLoaded = FALSE;
        DMD_ERR(("[%s]  MSB1236C HW init error!!!\n", __FUNCTION__));
        MsOS_DeleteMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return FALSE;
    }

#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
    if(MSPI_PATH_NONE == pMSB1236C->u32DmxInputPath)
    {
       DMD_DBG((" Load DMD FW by I2C\n"));
    }
    else
#endif
    {
        // Set PADS_TS0_MODE as PAD_TS0_D7
        msb1236C_SPIPAD_En(u8DemodIndex,TRUE);
        //Initialize SPI0 for MSPI
        MDrv_MSPI_Init_Ext(0);
    }
#endif

#if (MSB1236_DVBT_ONLY == 1)
    status = MSB1236C_LoadDSPCodeToSRAM_dvbt(u8DemodIndex);
#else
    status = MSB1236C_LoadDSPCodeToSDRAM(u8DemodIndex,MSB1236C_ALL);
#endif
    if (!status)
    {
        pMSB1236C->bInited = FALSE;
        pMSB1236C->bOpen = FALSE;
        pMSB1236C->bLoaded = FALSE;
        DMD_ERR(("[%s]  MSB1236C LoadDSPCodeToSDRAM fail!!!\n", __FUNCTION__));
        MsOS_DeleteMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return FALSE;
    }

    if (MSB1236C_LoadDSPCode(u8DemodIndex) == FALSE)
    {

        pMSB1236C->bInited = FALSE;
        pMSB1236C->bOpen = FALSE;
        pMSB1236C->bLoaded = FALSE;
        DMD_ERR(("[%s]  MSB1236C LoadDSPCode fail!!!\n", __FUNCTION__));
        MsOS_DeleteMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return FALSE;
    }

#ifdef USE_SPI_LOAD_TO_SDRAM
    MS_BOOL     bRet = TRUE;
    // ------disable to use TS_PAD as SSPI_PAD after load code
    // [0:0] reg_en_sspi_pad
    // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
    bRet &= MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x3b) * 2, 0x0001);

    // Set PADS_TS0_MODE as PAD_TS0_CLK
    msb1236C_SPIPAD_En(u8DemodIndex,FALSE);

#endif

    MSB1236C_Show_Version(u8DemodIndex);
#if TS_PARALLEL_OUTPUT
    MSB1236C_DTV_Serial_Control(u8DemodIndex,FALSE);
#else
    MSB1236C_DTV_Serial_Control(u8DemodIndex,TRUE);
#endif
    return TRUE;
}

MS_BOOL MDrv_Demod_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=FALSE;
    MS_U16 TPS_parameter=0;
    MS_U64 L1_Info=0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);


    switch(pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T:
        bRet = MSB1236C_DTV_DVB_T_GetSignalTpsInfo(u8DemodIndex,&TPS_parameter);
        if(bRet)
        {
            // TPS_parameter
            // Constellation (b2 ~ b0)
            // Hierarchy (b5 ~ b3)
            // LP Code Rate (b8 ~ b6)
            // HP Code Rate (b11 ~ b9)
            // GI (b13 ~ b12)
            // FFT ( b14)
            pParam->TerParam.eConstellation = (DEMOD_EN_TER_CONSTEL_TYPE)(TPS_parameter&0x3);
            pParam->TerParam.eHierarchy = (DEMOD_EN_TER_CONSTEL_TYPE)((TPS_parameter & (0x7 << 3))>>3);
            pParam->TerParam.eLPCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)((TPS_parameter & (0x7 <<6))>>6);
            pParam->TerParam.eHPCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)((TPS_parameter & (0x7 << 9))>>9);
            pParam->TerParam.eGuardInterval= (DEMOD_EN_TER_GI_TYPE)((TPS_parameter & (0x3 << 12))>>12);
            pParam->TerParam.eFFT_Mode = (DEMOD_EN_TER_FFT_MODE)((TPS_parameter & (0x3<<14))>>14);
            if(pParam->TerParam.eFFT_Mode == 2)
                pParam->TerParam.eFFT_Mode = DEMOD_TER_FFT_4K;
        }
        break;
    case E_DEVICE_DEMOD_DVB_T2:
        bRet = MSB1236C_DTV_DVB_T2_GetSignalL1Info(u8DemodIndex,&L1_Info);
        if(bRet)
        {
            pParam->TerParam.eConstellation = (DEMOD_EN_TER_CONSTEL_TYPE)(L1_Info&0x7);
            pParam->TerParam.eHierarchy = TER_HIE_NONE;
            pParam->TerParam.eHPCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)((L1_Info & (0x7 <<3))>>3);
            pParam->TerParam.eLPCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)(pParam->TerParam.eHPCodeRate);

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

            pParam->TerParam.eGuardInterval= (DEMOD_EN_TER_GI_TYPE)((L1_Info & (0x7 << 6))>>6);
            pParam->TerParam.eFFT_Mode = (DEMOD_EN_TER_FFT_MODE)((L1_Info & (0x7<<9))>>9);
            if(pParam->TerParam.eFFT_Mode)
                pParam->TerParam.eFFT_Mode = (DEMOD_EN_TER_FFT_MODE)(pParam->TerParam.eFFT_Mode + 1);
        }
        break;
    default:
        break;
    }
    return bRet;
}

static void _msb1236c_hw_reset(MS_U8 u8DemodIndex)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C;

    if((ptrMSB1236C == NULL) || (pDemodRest == NULL))
        return;

    pMSB1236C = ptrMSB1236C + u8DemodIndex;

    mdrv_gpio_set_high(pMSB1236C->RstPin);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(pMSB1236C->RstPin);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(pMSB1236C->RstPin);
    MsOS_DelayTask(5);

    pMSB1236C->bInited = FALSE;
    pMSB1236C->bOpen = FALSE;
    *(pDemodRest+u8DemodIndex) = TRUE;
}

#define MSB1236C_CHIP_ID 0x55
static MS_BOOL _msb1236c_I2C_CH3_reset(MS_U8 u8DemodIndex)
{
    //Reset twice to check if reset pin is correct
    _msb1236c_hw_reset(u8DemodIndex);
    if(!MSB1236C_I2C_CH_Reset(u8DemodIndex,3))
    {
        DMD_ERR(("[MSB1236C] I2C_CH_Reset fail \n"));
        return FALSE;
    }
    else
    {
        _msb1236c_hw_reset(u8DemodIndex);
        if(!MSB1236C_I2C_CH_Reset(u8DemodIndex,3))
        {
            DMD_ERR(("[MSB1236C] I2C_CH_Reset fail \n"));
            return FALSE;
        }
   }
   return TRUE;
}

MS_BOOL MSB1236C_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0,i;
    MDvr_CofdmDmd_CONFIG *pMSB1236C;
    MS_IIC_PORT ePort;
    if(!MSB1236C_Variables_alloc())
    {
        MSB1236C_Variables_free();
        return FALSE;
    }

    pMSB1236C = ptrMSB1236C + u8DemodIndex;
    ePort = getI2CPort(u8DemodIndex);
    if (ePort < E_MS_IIC_PORT_1)
    {
        pMSB1236C->fpMSB123xc_I2C_Access = msb1236c_I2C_Access;
    }
    else if (ePort < E_MS_IIC_PORT_2)
    {
        pMSB1236C->fpMSB123xc_I2C_Access = msb1236c_I2C1_Access;
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    for(i=0 ; i<sizeof(u8Possible_SLAVE_IDs); i++)
    {
        DEMOD_DYNAMIC_SLAVE_ID_1= u8Possible_SLAVE_IDs[i];
        if(!_msb1236c_I2C_CH3_reset(u8DemodIndex))
        {
            u8_tmp = 0xff;
            //sreturn FALSE;
        }
        else
        {
            if (MSB1236C_ReadReg(u8DemodIndex,0x0900, &u8_tmp))
            {
                DMD_DBG(("[MSB1236C] read id :%x \n",u8_tmp ));
                if(u8_tmp == MSB1236C_CHIP_ID)
                    break;
            }
        }
    }

    DMD_DBG(("[MSB1236C] read id :%x \n",u8_tmp ));

    if(u8_tmp == MSB1236C_CHIP_ID)
    {
        u8MSB1236C_DevCnt++;
        *pu8SlaveID = DEMOD_DYNAMIC_SLAVE_ID_1;
#ifdef USE_SPI_LOAD_TO_SDRAM
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
        //check TS path
        MDrv_DMD_SSPI_Init(0);
        MDrv_MasterSPI_CsPadConfig(0, 0xff);
        MDrv_MasterSPI_MaxClkConfig(0, 27);

        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900+(0x28)*2, 0x0000);
        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900+(0x2d)*2, 0x00ff);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
       MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900 + (0x3b) * 2, 0x0002);
        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        MSB1236C_ReadReg(u8DemodIndex,0x0900+(0x3a)*2+1,&u8_tmp);
        u8_tmp &= 0xFC;
        MSB1236C_WriteReg(u8DemodIndex,0x0900+(0x3a)*2+1, u8_tmp);

        // ------- MSPI driving setting
        MSB1236C_WriteReg2bytes(u8DemodIndex,0x0900+(0x2c)*2, 0x07ff);

        for(i=0; i<MSPI_PATH_MAX; i++)
        {
            SPIPAD_EN[i](TRUE);
            // Note: Must read twice and ignore 1st data because pad switch noise
            // might cause wrong MSPI signal
            MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
            MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
            if(u8_tmp == MSB1236C_CHIP_ID)
            {
                pMSB1236C->u32DmxInputPath = (MS_U32)i;
                if(i)
                    msb1236_SPIPAD_TS1_En(FALSE);
                else
                    msb1236_SPIPAD_TS0_En(FALSE);

                DMD_DBG(("Get MSB1236C chip ID by MSPI on TS%lx\n", pMSB1236C->u32DmxInputPath));
                break;
            }
            else
            {
                DMD_DBG(("Cannot get MSB1236C chip ID by MSPI on TS%x\n", i));
                if( i == (MSPI_PATH_MAX - 1))
                    pMSB1236C->u32DmxInputPath = MSPI_PATH_NONE;
            }

            SPIPAD_EN[i](FALSE);
        }

        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        MSB1236C_WriteReg2bytes(u8DemodIndex, 0x0900 + (0x3b) * 2, 0x0001);
#endif //MSPI_PATH_DETECT
#endif
        return TRUE;
    }
    else
    {
        if(!u8MSB1236C_DevCnt)
            MSB1236C_Variables_free();
        return FALSE;
    }
}

MS_BOOL MSB1236C_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = (ptrMSB1236C + u8DemodIndex);

    switch(fuction_type)
    {
    case DEMOD_EXT_FUNC_RESET:
        bret &= MSB1236C_Reset(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_OPEN:
        bret &= MDrv_Demod_Open(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_FINALIZE:
        if(pMSB1236C ->s32_MSB1236C_Mutex >= 0)
        {
            bret &= MsOS_DeleteMutex(pMSB1236C->s32_MSB1236C_Mutex);
            pMSB1236C->s32_MSB1236C_Mutex = -1;
        }
        pMSB1236C->bInited = FALSE;
        pMSB1236C->bOpen = FALSE;
        pMSB1236C->bLoaded = FALSE;
        *(pDemodRest+u8DemodIndex) = TRUE;
        break;
#ifdef MS_DVBT2_INUSE
    case DEMOD_EXT_FUNC_CTRL_RESET_DJB_FLAG:
        MDrv_Demod_CtrlResetDJBFlag(u8DemodIndex, *(MS_BOOL *)data);
        break;
    case DEMOD_EXT_FUNC_T2MI_RESTART:
        bret &= MDrv_Demod_T2MI_Restart(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_INIT_PARAMETER:
        bret &= MDrv_Demod_InitParameter(u8DemodIndex);
        break;
    case DEMOD_EXT_FUNC_GET_PLPID_LIST:
        bret &= MDrv_Demod_GetPlpIDList(u8DemodIndex, (MS_U8 *)data);
        break;
#endif
    case DEMOD_EXT_FUNC_SET_RESET_PIN:
        if(!MSB1236C_Variables_alloc())
        {
            MSB1236C_Variables_free();
            return FALSE;
        }
        else
        {
            pMSB1236C = (ptrMSB1236C + u8DemodIndex);
            pMSB1236C->RstPin = *(int*)data;
        }
        break;

    default:
        DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
        break;
    }
    return bret;
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1236C) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSB1236C",
    .data                         = DEMOD_MSB1236C,
    .SupportINT                   = FALSE,
    .init                         = MDrv_Demod_Init,
    .GetLock                      = MDrv_Demod_GetLock,
    .GetSNR                       = MDrv_Demod_GetSNR,
    .GetBER                       = MDrv_Demod_GetBER,
    .GetPWR                       = MDrv_Demod_GetPWR,
    .GetSSI                       = MDrv_Demod_GetSSI,
    .GetQuality                   = MDrv_Demod_GetSignalQuality,
    .GetParam                     = MDrv_Demod_GetParam,
    .Restart                      = MDrv_Demod_Restart,
    .I2CByPass                    = MDrv_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .CheckExist                   = MSB1236C_Check_Exist,
    .Extension_Function           = MSB1236C_Extension_Function,
    .Extension_FunctionPreSetting = NULL,
    .Get_Packet_Error             = MSB1236C_Demod_Get_Packet_Error,
#if MS_DVBT2_INUSE
    .SetCurrentDemodType          = MDrv_Demod_SetCurrentDemodType,
    .GetCurrentDemodType          = MDrv_CofdmDmd_GetCurrentDemodType,
    .GetPlpBitMap                 = MSB1236C_DTV_GetPlpBitMap,
    .GetPlpGroupID                = MSB1236C_DTV_GetPlpGroupID,
    .SetPlpGroupID                = MSB1236C_DTV_SetPlpGroupID,
    .SetScanTypeStatus            = MSB1236C_Demod_SetScanTypeStatus,
    .GetScanTypeStatus            = MSB1236C_Demod_GetScanTypeStatus,
    .GetNextPLPID                 = MSB1236C_DTV_GetNextPLPID,
    .GetPLPType                   = MDrv_Demod_null_GetPLPType,
#endif
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
    .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
    .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
    .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
    .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

#endif

