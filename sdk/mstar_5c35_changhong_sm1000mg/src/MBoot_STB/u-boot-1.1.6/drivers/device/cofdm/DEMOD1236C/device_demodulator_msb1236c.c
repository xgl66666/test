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

#include <common.h>
#include "sys/common/MsTypes.h"
#include "sys/common/MsOS.h"
#include <sys/drv/drvGPIO.h>
#include <device/drvDemod.h>
#include <sys/drv/drvIIC.h>
#include <frontend_config.h>
#include <device/drvTuner.h>
#define USE_UTOPIA

#if(FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
#include "sys/drv/drvDMD_EXTERN_MSB123xc.h"
#include "device_demodulator_msb1236c.h"
extern MS_U8 MSB123xc_LIB[];
#if(FRONTEND_TUNER_TYPE == TUNER_MXL603)
#include "../../tuner/MxL603/MxL603_TunerApi.h"
#define MXL603_I2C_ADDR         0xC0
#endif

#define TS_PARALLEL_OUTPUT 1

#define UNUSED(var)     (void)((var) = (var))
#define MSB1236C_DEMOD_IIC 1
#define MSB1236C_SPI_IIC 2
#define MSB1236C_DEBUG_ON 1

#define MSB1236C_Delayus   MsOS_DelayTaskUs
#define MDrv_Timer_GetTime0  OS_SYSTEM_TIME

#define PRELOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 0
#define LOAD_DSP_CODE_FROM_MAIN_CHIP_I2C 1
#define SDRAM_DATA_CHECK                 0
#define MSB1236_DVBT_ONLY                0
#define TIMING_VERIFICATION              0

#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define SDRAM_BASE                       0x5000
#define SRAM_BASE                        0x8000

// MSB1236C flash size: 1Mbits
#define MAX_MSB1236C_LIB_LEN             131072
#define MSB1236C_BOOT  0x01
#define MSB1236C_DVBT2 0x02
#define MSB1236C_DVBT  0x04
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

#define EDINBURGH_WINDOWS_BASE                0x100
#define EDINBURGH_BOOT_WINDOWS_OFFSET         EDINBURGH_WINDOWS_BASE
#define EDINBURGH_DVBT2_P2_WINDOWS_OFFSET    (EDINBURGH_WINDOWS_BASE + 0x08)
#define EDINBURGH_DVBT2_P1_WINDOWS_OFFSET    (EDINBURGH_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBT_WINDOWS_OFFSET        (EDINBURGH_DVBT2_P1_WINDOWS_OFFSET + 0x08)

#define MSB1236C_MAX_FLASH_ON_RETRY_NUM 3
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

static float g_MSB1236C_fSNR = 0.0;
MS_U8 u8Type, u8Plp;


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

#define MSB1236C_DEBUG   0

#if MSB1236C_DEBUG
#define PRINTE(p)       printf p
#else
#define PRINTE(p)
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
#if (FRONTEND_TUNER_TYPE == TUNER_MXL603)
#define FRONTEND_DEMOD_IQ_SWAP     0
#else
#define FRONTEND_DEMOD_IQ_SWAP     0
#endif
#define IQ_SWAP        FRONTEND_DEMOD_IQ_SWAP
#if TS_PARALLEL_OUTPUT
#define SERIAL_TS               0x00
#else
#define SERIAL_TS               0x01
#endif
#define TS_SERIAL_CLK           0x00
#define TS_DATA_SWAP       0x00
#define TS_OUT_INV             TS_CLK_INV
/*
// FC: FC = FS = 5000 = 0x1388     (5.00MHz IF)
#define FC_L           0x88    //5M
#define FC_H           0x13
*/
#if(FRONTEND_TUNER_TYPE == TUNER_RT820T)
#define FC_L           0xDA
#define FC_H           0x11  //7M AND 8M     IF=4.57MHZ 
	
#define FC_L_6M        0xF2
#define FC_H_6M        0x0D    //6M     IF= 3.57MHZ
#else
#define FC_L           0x88    //5M
#define FC_H           0x13
#endif


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
/*
// FC: FC = FS = 5000 = 0x1388     (5.00MHz IF)
#define T2_FC_L_VAL            0x88    //5M
#define T2_FC_H_VAL            0x13*/
// FC: FC = FS = 5000 = 0x1388     (5.00MHz IF)
#if(FRONTEND_TUNER_TYPE == TUNER_RT820T)
#define T2_FC_L_VAL            0xDA
#define T2_FC_H_VAL            0x11  //7M and 8M  IF=4.57MHZ

#define T2_FC_L_VAL_6M          0xF2
#define T2_FC_H_VAL_6M          0x0D    //6M  IF=3.57MHZ
#else
#define T2_FC_L_VAL            0x88    //5M
#define T2_FC_H_VAL            0x13
#endif



#define CUSTOMER_TABLE  1
#if (CUSTOMER_TABLE == 0)
static MS_U8 *MSB1236c_DVBT2_DSPREG_TABLE = NULL;
static MS_U8 *MSB1236c_DVBT_DSPREG_TABLE = NULL;
#else
static MS_U8 MSB1236c_DVBT_DSPREG_TABLE[] = // andy 2009-12-15  3:55:03 TW model
{
    RFAGC_EN,     HUMDET_EN,    DCR_EN,     IIS_EN,         CCI_EN,      ACI_EN,
    IQB_EN,       AUTO_IQ_SWAP, AUTO_RFMAX, AUTO_ACI,       MODE_CP_FORCED, TPS_FORCED,
    AUTO_SCAN,    RSV_0D,       RSV_0E,     RSV_0F,
    RSSI,         ZIF,          FREQ,       FC_L,           FC_H,        FS_L,      FS_H,
    BW,           MODE,         CP,         LP_SEL,         CSTL,        HIER,      HPCR,
    LPCR,         IQ_SWAP,      RFMAX,      ATV_SYSTEM,     ICFO_RANGE,  RFAGC_REF,
    IFAGC_REF_2K, IFAGC_REF_8K, IFAGC_REF_ACI,  IFAGC_REF_IIS_2K,  IFAGC_REF_IIS_8K, IFAGC_ACI_DET_TH_L,
    IFAGC_ACI_DET_TH_H,         SERIAL_TS,  TS_SERIAL_CLK,  TS_OUT_INV,  TS_DATA_SWAP,
    SFO_2K_H,     SFO_2K_L,     SFO_8K_H,   SFO_8K_L,       CHECK_CHANNEL,          SNR_POS,
    CCI_KP,       CCI_FSWEEP    , TS_CLK_RATE_AUTO, DVBT_CRC
};
static MS_U8 MSB1236c_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL, T2_FC_L_VAL, T2_FC_H_VAL,  T2_TS_SERIAL_VAL, T2_TS_CLK_RATE_VAL,
    T2_TS_OUT_INV_VAL, T2_TS_DATA_SWAP_VAL, T2_TS_ERR_POL_VAL
};
#endif

typedef struct
{
    MS_BOOL     bInited;
    MS_BOOL    bOpen;
    EN_DEVICE_DEMOD_TYPE  enDemodType;
    MS_S32     s32_MSB1236C_Mutex;
    MS_U8       PlpIDList[256];
    MS_U8       u8sramCode;
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
} MDvr_CofdmDmd_CONFIG;
MDvr_CofdmDmd_CONFIG MSB1236C;

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

S_SSI_PREF SsiPref[] =
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

#if(TIMING_VERIFICATION == 1)
static void show_timer(void)
{
    printf("***************************\n");
    printf("[tmm1]t2-t1 = %ld (%ld - %ld)\n", tmm_2 - tmm_1, tmm_2, tmm_1);
    printf("[tmm2]t4-t3 = %ld (%ld - %ld)\n", tmm_4 - tmm_3, tmm_4, tmm_3);
    printf("[tmm3]t6-t5 = %ld (%ld - %ld)\n", tmm_6 - tmm_5, tmm_6, tmm_5);
    printf("[tmm4]t8-t7 = %ld (%ld - %ld)\n", tmm_8 - tmm_7, tmm_8, tmm_7);
    printf("[tmm5]t10-t9 = %ld (%ld - %ld)\n", tmm_10 - tmm_9, tmm_10, tmm_9);
    printf("[tmm6]t12-t11 = %ld (%ld - %ld)\n", tmm_12 - tmm_11, tmm_12, tmm_11);
    printf("[tmm7]t14-t13 = %ld (%ld - %ld)\n", tmm_14 - tmm_13, tmm_14, tmm_13);
    printf("[tmm8]t16-t15 = %ld (%ld - %ld)\n", tmm_16 - tmm_15, tmm_16, tmm_15);
    printf("[tmm9]t18-t17 = %ld (%ld - %ld)\n", tmm_18 - tmm_17, tmm_18, tmm_17);
    printf("[tmm10]t20-t19 = %ld (%ld - %ld)\n", tmm_20 - tmm_19, tmm_20, tmm_19);
    printf("[tmm11]lock_time# = %ld (%ld - %ld)\n", tmm_14 - u32StartTime, tmm_14, u32StartTime);
    printf("[tmm12]lock-setf = %ld (%ld - %ld)\n", tmm_14 - tmm_11, tmm_14, tmm_11);
    printf("[tmm13]lock-loaddsp = %ld (%ld - %ld)\n", tmm_14 - tmm_9, tmm_14, tmm_9);
    printf("***************************\n");
}
#endif

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

/*================================================
==                       IIC write/read interface
=================================================*/
MS_BOOL MSB1236C_WriteBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_WriteBytes(DEMOD_DYNAMIC_SLAVE_ID_1, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        printf("Demod IIC write error\n");
    }
    return bRet;
}
MS_BOOL MSB1236C_ReadBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;

    bRet = MDrv_IIC_ReadBytes(DEMOD_DYNAMIC_SLAVE_ID_1, u8AddrSize, pu8Addr, u16Size , pu8Data);
    if (FALSE == bRet)
    {
        printf("Demod IIC read error\n");
    }
    return bRet;
}

MS_BOOL MSB1236C_WriteBytes_spi(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_WriteBytes(DEMOD_DYNAMIC_SLAVE_ID_2, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        printf("Demod IIC write spi error\n");
    }
    return bRet;
}

MS_BOOL MSB1236C_ReadBytes_spi(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    bRet = MDrv_IIC_ReadBytes(DEMOD_DYNAMIC_SLAVE_ID_2, u8AddrSize, pu8Addr, u16Size , pu8Data);
    if (FALSE == bRet)
    {
        printf("Demod IIC read spi error\n");
    }
    return bRet;
}

typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

mapi_i2c DemodI2Chandler, SpiI2Chandler;
mapi_i2c* mapi_i2c_GetI2C_Dev(MS_U32 u32gID)
{
    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        printf("iic device not supported\n");
    case MSB1236C_DEMOD_IIC:
        handler = &DemodI2Chandler;
        handler->WriteBytes = MSB1236C_WriteBytes_demod;
        handler->ReadBytes = MSB1236C_ReadBytes_demod;
        break;
    case MSB1236C_SPI_IIC:
        handler = &SpiI2Chandler;
        handler->WriteBytes = MSB1236C_WriteBytes_spi;
        handler->ReadBytes = MSB1236C_ReadBytes_spi;
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

/*================================================
==               demod  msb1236c  write/read interface
=================================================*/
MS_BOOL MSB1236C_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_MSB123xc_SetReg(u16Addr, u8Data);
}

MS_BOOL MSB1236C_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    return MDrv_DMD_MSB123xc_SetRegs(u16Addr, u8pData, data_size);
}

MS_BOOL MSB1236C_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    return MDrv_DMD_MSB123xc_SetReg2Bytes(u16Addr, u16Data);
}

MS_BOOL MSB1236C_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return MDrv_DMD_MSB123xc_GetReg(u16Addr, pu8Data);
}

MS_BOOL  MSB1236C_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_MSB123xc_SetDSPReg(u16Addr, u8Data);
}

MS_BOOL MSB1236C_ReadDspReg(MS_U16 u16Addr, MS_U8* pData)
{
    return MDrv_DMD_MSB123xc_GetDSPReg(u16Addr, pData);
}


/*================================================
==               demod  MSB1236C  base interface
=================================================*/
#if (MSB1236C_DEBUG_ON == 1)
void MSB1236C_Show_Version(void)
{

    MS_U8  reg = 0;
    MS_U16 ver0 = 0;
    MS_U8  ver1 = 0;

    MSB1236C_ReadDspReg(E_T2_FW_VER_0, &reg);
    ver0 = reg;
    MSB1236C_ReadDspReg(E_T2_FW_VER_1, &reg);
    ver0 = (ver0 << 8) | reg;
    MSB1236C_ReadDspReg(E_T2_FW_VER_2, &reg);
    ver1 = reg;

    printf("\n[MSB1236C]ver0 = 0x%x, ver1 = 0x%x\n", ver0, ver1);

    return;
}
#endif

MS_BOOL MSB1236C_I2C_CH_Reset(MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    static MS_BOOL DemodRest = TRUE;

    PRINTE(("[MSB1236C][beg]MSB1236C_I2C_CH_Reset, CH=0x%x\n", ch_num));
    mapi_i2c *iptr = mapi_i2c_GetI2C_Dev(MSB1236C_DEMOD_IIC);
    if (iptr == NULL)
    {
        return FALSE;
    }

    if (DemodRest)
    {
        DemodRest = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        bRet &= iptr->WriteBytes(0, NULL, 5, data);
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    /*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
    */
    PRINTE(("[MSB1236C][end]MSB1236C_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}

// 144 ms roughly
MS_BOOL MSB1236C_HW_init(void)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));

    MS_BOOL bRet = TRUE;

#if(TIMING_VERIFICATION == 1)
    tmm_1 = GIVE_ME_TIME;
#endif

    // 10us delay
    MSB1236C_Delayus(10);

    // MSB1236C_Reset MCU
    // -------------------------------------------------------------------
    // Initialize DMD_ANA_MISC
    // -------------------------------------------------------------------
    // [1:0]    reg_mpll_loop_div_first
    // [15:8]   reg_mpll_loop_div_second
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x33) * 2, 0x2400);

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
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x35) * 2, 0x1804);
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x35) * 2, 0x0004);


    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x34) * 2, 0x4000);
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x34) * 2, 0x0000);

    // [0]  reg_mpll_clk_dp_pd  dummy
    // [1]  reg_adc_clk_pd      ADC output clock power down
    // [2]  reg_mpll_div2_pd    MPLL_DIV2 power down
    // [3]  reg_mpll_div3_pd    MPLL_DIV3 power down
    // [4]  reg_mpll_div4_pd    MPLL_DIV4 power down
    // [5]  reg_mpll_div8_pd    MPLL_DIV8 power down
    // [6]  reg_mpll_div10_pd   MPLL_DIV10 power down
    // [13:8]  reg_mpll_adc_div_sel select the ADC clock divide ratio,ADC clk=XTAL_IN * (LOOP_DIV_FIRST*LOOP_DIV_SECOND)/div_ratio

    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x30) * 2, 0x1e00);

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
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x01) * 2, 0x0440);


    // [0]  Channel I ADC power down: 1=power dwon
    // [1]  Channel Q ADC power down: 1=power dwon
    // [2]  power down clamp buffer for test mode
    // [3]  change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x0c) * 2, 0x0000);

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
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x20) * 2, 0x0000);

    // Set ADC gain is 1
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x0b) * 2, 0x0009);

    // Disable ADC Sign bit
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x2e) * 2, 0x0000);

    // ADC I channel offset
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x2a) * 2, 0x0c00);

    // ADC Q channel offset
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x2b) * 2, 0x0c00);
    // -------------------------------------------------------------------
    // End Initialize Initialize ADC I/Q
    // -------------------------------------------------------------------


    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x19) * 2, 0x1e00);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x1c) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x1e) * 2, 0x0000);


    // [15:0]   reg_mpll_test
    // [4]  mpll lock detector enable
    bRet &= MSB1236C_WriteReg2bytes(0x0A00 + (0x34) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x09) * 2, 0x0101);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x0a) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x0b) * 2, 0x0030);

    // @0x0910
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x10) * 2, 0x1110);

    // @0x0911
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x11) * 2, 0x0111);

    // @0x0912
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x12) * 2, 0x0618);

    // @0x0913
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x13) * 2, 0x0020);

    // @0x0914
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x14) * 2, 0x0000);

    // @0x0915
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x15) * 2, 0x0111);

    // @0x0916
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x16) * 2, 0x0001);

    // @0x0917
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x17) * 2, 0x0000);

    // @0x0918
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x18) * 2, 0x1000);

    // @0x0919
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x19) * 2, 0x3c00);

    // @0x091a
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x1a) * 2, 0x0000);

    // @0x091b
    // [2:0]    reg_ckg_dvbt2_outer1x
    // [6:4]    reg_ckg_dvbt2_outer2x
    // [10:8]   reg_ckg_syn_miu
    // [14:12]  reg_ckg_syn_ts
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x1b) * 2, 0x0000);


    // @0x091c
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x1c) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x08) * 2, 0x0a01);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x28) * 2, 0x0000);


    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x51) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x52) * 2, 0x0020);


    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x50) * 2, 0x0010);


    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x57) * 2, 0x0003);


    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x55) * 2, 0x0006);
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x55) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x59) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x20) * 2, 0x0004);

#if(MSB1236_DVBT_ONLY == 0)
    MS_U8 u8_tmp = 0;
    MS_U8 u8_timeout = 0;

    /*Beg initialize MIU*/

    // dummy register setting
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0000);

    // dummy register setting
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0000);

    // dummy register setting
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0000);

    // miu software MSB1236C_Reset
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c00);

    // set frequence 130MHz
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x11) * 2, 0x0052);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x10) * 2, 0x352b); // 0x29a378 = 166MHz, 0x352b52 = 130MHz

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x11) * 2, 0x6052);
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x11) * 2, 0x0052);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x12) * 2, 0xc000);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x13) * 2, 0x0000);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x16) * 2, 0x0030);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x1c) * 2, 0x0080);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x01) * 2, 0x8100);
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x01) * 2, 0xe100);
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x01) * 2, 0x8100);

    // in_phase
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x02) * 2, 0x0360);

    // tREFRESH                      : [7:0]        30
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x03) * 2, 0x0030);

    // reg_tRAS                      : [3:0]         9
    // reg_tRC                       : [7:4]         c
    // reg_tRCD                      : [11:8]        3
    // reg_tRP                       : [15:12]       3
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x04) * 2, 0x33c9);

    // reg_tRRD                      : [3:0]         2
    // tWR                           : [7:4]         3
    // reg_tMRD                      : [11:8]        2
    // reg_tRTP                      : [15:12]       4
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x05) * 2, 0x4232);

    // reg_w2r_dly(tWTR)             : [3:0]         2
    // reg_w2r_oen_dly               : [7:4]         3
    // reg_r2w_dly(tRTW)             : [11:8]        5
    // reg_r2w_oen_dly               : [15:12]       5
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x06) * 2, 0x5532);

    // tRFC                          : [5:0]         c
    // reg_tRAS[4]                   : [6]           0
    // reg_tRC[4]                    : [7]           0
    // reg_write_latency             : [10:8]        0
    // reg_tCCD                      : [15:14]       1
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x07) * 2, 0x400c);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0e) * 2, 0x1800);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x23) * 2, 0x7ffe);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x20) * 2, 0xc001);

    // #40000
    // 10us delay
    MSB1236C_Delayus(10);

    // miu software MSB1236C_Reset
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c00);

    // miu software MSB1236C_Reset
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c01);
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x0f) * 2, 0x0c00);

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x01) * 2, 0x010d);

    /*Beg Initial DRAM start here!!!*/

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x00) * 2, 0x0001);

    // wait init_done

    bRet &= MSB1236C_ReadReg(0x1201, &u8_tmp);
    PRINTE(("[MSB1236C]MIU wait init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));
    while (u8_tmp != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            PRINTE(("[MSB1236C][err]MIU init failure...\n"));
            return FALSE;
        }
        // 10us delay
        MSB1236C_Delayus(10);
        bRet &= MSB1236C_ReadReg(0x1201, &u8_tmp);
    }
    PRINTE(("[MSB1236C]MIU init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x08) * 2, 0x0001);

    /*Beg set MIU self test parameter*/

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x71) * 2, 0x0000);

    // length
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x72) * 2, 0x0010);

    // test data
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x74) * 2, 0x5aa5);

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
    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x70) * 2, 0x0001);

    // #10000;
    // 10us delay
    MSB1236C_Delayus(10);

    // wait for test done

    bRet &= MSB1236C_ReadReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);
    PRINTE(("[MSB1236C]MIU wait test done, u8_tmp=0x%x,bRet=0x%x\n", u8_tmp, bRet));
    while ((u8_tmp & 0x80) != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            PRINTE(("[MSB1236C][err]MIU self test NOT finished...\n"));
            return FALSE;
        }
        // 10us delay
        MSB1236C_Delayus(10);
        bRet &= MSB1236C_ReadReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);
    }
    // test done.


    // 10us delay
    MSB1236C_Delayus(10);

    // test result check

    bRet &= MSB1236C_ReadReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);

    if (u8_tmp == 0x40)
    {
        PRINTE(("@MSB1236C, error, MIU self test fail !!!!\n"));
        bRet = FALSE;
    }
    else
    {
        PRINTE(("@MSB1236C, MIU self test successful.\n"));
    }

    bRet &= MSB1236C_WriteReg2bytes(0x1200 + (0x23) * 2, 0x0000);

    /*End initialize MIU*/
#endif

    /* Beg Turn on pads */
    // ------Turn off all pad in
    // [0] reg_set_pad_low
    // [1] reg_set_pad_high
    // [2] reg_set_i2cs_pad_low
    // [3] reg_set_i2cs_pad_high
    // [8] reg_turn_off_pad
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x28) * 2, 0x0000);

    // ------I2CM pad on
    // [1:0]    reg_iicm_pad_sel[1:0]   1:iicm enable 2:UART enable
    // [4]      reg_i2c_sbpm_en     1: enable I2CS bypass to I2CM function
    // [12:8]   reg_i2c_sbpm_idle_num[4:0]  a: default
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x08) * 2, 0x0a01);

    // ------Transport Stream pad on (except TS ERR pad)
    // [15:0]   reg_en_ts_pad[15:0] 0x00ff:normal TS location 0xff00:reverse TS location
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x2d) * 2, 0x00ff);

    // ------Transport Stream pad on (TS ERR pad)
    // [0]      reg_en_ts_err_pad   1: enable
    // [4]      reg_ts_err_pol  1: inverse 0:normal
    bRet &= MSB1236C_WriteReg2bytes(0x0900 + (0x2e) * 2, 0x0001);

    // ------AGC pad on
    bRet &= MSB1236C_WriteReg2bytes(0x0a00 + (0x18) * 2, 0x0010);

    bRet &= MSB1236C_WriteReg(0x0990, 0x20);

#if(TIMING_VERIFICATION == 1)
    tmm_2 = GIVE_ME_TIME;
#endif
    PRINTE(("[MSB1236C][end]MSB1236C_HW_init, bRet=0x%x\n", bRet));
    return bRet;

}

void MSB1236C_Reset(void)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8     u8Data = 0x00;
    MS_U32    u32Retry = 0x00;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return ;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return ;
    }

    printf("Set TSP0 registers...\n");

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        printf("\nreset T2\n");
        // mask miu access for all and mcu
        MSB1236C_WriteReg(0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB1236C_WriteReg(0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        MSB1236C_Delayus(10);

        MSB1236C_WriteReg(0x0B00 + (0x10) * 2, 0x01);
        MSB1236C_WriteReg(0x0B00 + (0x19) * 2, 0x03);
        MSB1236C_WriteReg(0x0C00, 0x00);             //clear MB_CNTL
        MSB1236C_Delayus(5 * 100);

        // enable miu access of mcu gdma
        MSB1236C_WriteReg(0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        MSB1236C_Delayus(10);

        MSB1236C_WriteReg(0x0B00 + (0x19) * 2, 0x00);
        MSB1236C_WriteReg(0x0B00 + (0x10) * 2, 0x00);
        MSB1236C_Delayus(5 * 1000);
        MSB1236C_ReadReg(0x0C00, &u8Data);           //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            MSB1236C_Delayus(5 * 1000);
            MSB1236C_ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                PRINTE((">>>MSB1236C: MSB1236C_Reset Fail!\n"));
                break;
            }
        }
    }
    else if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        printf("\nreset T\n");
        // mask miu access for all and mcu
        MSB1236C_WriteReg(0x1200 + (0x23) * 2 + 1, 0x7f);
        MSB1236C_WriteReg(0x1200 + (0x23) * 2, 0xfe);
        // 10us delay
        MSB1236C_Delayus(10);

        MSB1236C_WriteReg(0x0B00 + (0x19) * 2, 0x01);
        MSB1236C_WriteReg(0x0B00 + (0x10) * 2, 0x01);
        MSB1236C_WriteReg(0x0C00, 0x00);             //clear MB_CNTL
        MSB1236C_Delayus(5 * 1000);

        // enable miu access of mcu gdma
        MSB1236C_WriteReg(0x1200 + (0x23) * 2, 0xf0);
        // 10us delay
        MSB1236C_Delayus(10);

        MSB1236C_WriteReg(0x0B00 + (0x10) * 2, 0x00);
        MSB1236C_WriteReg(0x0B00 + (0x19) * 2, 0x00);

        MSB1236C_ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
        while (u8Data != 0xff)
        {
            MSB1236C_Delayus(5 * 1000);
            MSB1236C_ReadReg(0x0C00, &u8Data);       //check MB_CNTL ready
            if (u32Retry++ > 200)
            {
                PRINTE((">>>MSB1236C: MSB1236C_Reset Fail!\n"));
                break;
            }
        }

    }

    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();
}

/*================================================
==               demod  MSB1236C loading fw interface
=================================================*/
#if (MSB1236_DVBT_ONLY == 1)
static MS_BOOL MSB1236C_Load2Sram(MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sram_win_offset_base)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL bRet = TRUE;
    MS_U16  sram_win_offset = sram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;

    UNUSED(sram_win_offset);
    PRINTE(("[MSB1236C][beg]MSB1236C_Load2Sram, len=0x%x, win_offset=0x%x\n", data_length, sram_win_offset_base));

#if(TIMING_VERIFICATION == 1)
    tmm_3 = GIVE_ME_TIME;
#endif

    if (MSB1236C_I2C_CH_Reset(0) == FALSE)
    {
        PRINTE((">>>MSB1236C CH0 MSB1236C_Reset:Fail\n"));
        return FALSE;
    }
    else
    {
        //--------------------------------------------------------------------------
        //  Set xData map for Program SRAM
        //--------------------------------------------------------------------------
        //set lower bound "xb_eram_hb[5:0]"
        bRet &= MSB1236C_WriteReg(0x1000 + (0x70) * 2 + 1, 0x20);

        //set upper bound "xb_eram_lb[5:0]"
        bRet &= MSB1236C_WriteReg(0x1000 + (0x70) * 2, 0x3f);

        //set "reg_xd2eram_hk_mcu_psram_en"
        bRet &= MSB1236C_WriteReg(0x1000 + (0x73) * 2, 0x08);

        for (x_data_offset = 0, y_cir_addr = SRAM_BASE; x_data_offset < data_length;)
        {
            if ((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
            {
                bRet &= MSB1236C_WriteRegs(y_cir_addr, (u8_ptr + x_data_offset), LOAD_CODE_I2C_BLOCK_NUM);
                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
            }
            else
            {
                bRet &= MSB1236C_WriteRegs(y_cir_addr, (u8_ptr + x_data_offset), data_length - x_data_offset);
                y_cir_addr += (data_length - x_data_offset);
                x_data_offset += (data_length - x_data_offset);
            }
        }
        PRINTE(("[MSB1236C]x_data_offset=%d,y_cir_addr=%d\n", x_data_offset, y_cir_addr));

        //--------------------------------------------------------------------------
        //  Release xData map for Program SRAM
        //--------------------------------------------------------------------------
        //clear  "reg_xd2eram_hk_mcu_psram_en"
        // `DBG.iic_write( 2, (`RIUBASE_MCU + 8'he6), 8'h00);
        bRet &= MSB1236C_WriteReg(0x1000 + (0x73) * 2, 0x00);

        // Channel changed from CH 0x00 to CH 0x03
        if (MSB1236C_I2C_CH_Reset(3) == FALSE)
        {
            PRINTE((">>>MSB1236C CH3 MSB1236C_Reset:Fail\n"));
            return FALSE;
        }
    }

#if(TIMING_VERIFICATION == 1)
    tmm_4 = GIVE_ME_TIME;
    show_timer();
#endif

    PRINTE(("[MSB1236C][end]MSB1236C_Load2Sram, len=0x%x, win_offset=0x%x\n", data_length, sram_win_offset_base));
    return bRet;

}

static MS_BOOL MSB1236C_LoadDSPCodeToSRAM_dvbt(void)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL bRet = TRUE;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    PRINTE(("[MSB1236C][beg]MSB1236C_LoadDSPCodeToSRAM_dvbt\n"));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (!(pMSB1236C->u8sramCode & MSB1236C_DVBT))
    {
        // dvbt code
        if (sizeof(MSB123xc_LIB) > EDINBURGH_DVBT_START_ADDR)
        {
            data_ptr = MSB123xc_LIB + EDINBURGH_DVBT_START_ADDR;
            code_size = EDINBURGH_DVBT_END_ADDR - EDINBURGH_DVBT_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT_WINDOWS_OFFSET;
            bRet &= MSB1236C_Load2Sram(data_ptr, code_size, win_offset);
            if (bRet == TRUE)
            {
                pMSB1236C->u8sramCode |= MSB1236C_DVBT;
            }
        }
        else
        {
            printf("@MSB1236C, dvbt code is unavailable!!!\n");
        }
    }
    PRINTE(("[MSB1236C][end]MSB1236C_LoadDSPCodeToSRAM_dvbt\n"));
    return bRet;
}
#endif

static void MSB1236C_Driving_Control(MS_BOOL bEnable)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8 u8Temp;
    MSB1236C_ReadReg(0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    MSB1236C_WriteReg(0x0958, u8Temp);

    MSB1236C_ReadReg(0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp & (~0x0F);
    }
    MSB1236C_WriteReg(0x0959, u8Temp);
}

static MS_BOOL MSB1236C_DTV_Serial_Control(MS_BOOL bEnable)
{
    UNUSED(bEnable);
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        //FRA_T2_BRINGUP
    }
    MSB1236C_Driving_Control(1);
    return TRUE;
}

/*================================================
==               demod  MSB1236C iic bypass interface
=================================================*/
MS_BOOL MSB1236C_IIC_Bypass_Mode(MS_BOOL enable)
{
    MS_U8 u8Retry = 10;
    MS_BOOL bResult;

    PRINTE((">>>MSB1236C: IIC_bypass() set %x\n", enable));

    while (u8Retry--)
    {
        if (enable)
            bResult = MSB1236C_WriteReg(0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            bResult = MSB1236C_WriteReg(0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off

        if (bResult)
            break;
    }
    return TRUE;
}

/*================================================
==               demod  MSB1236C lock and ssi sqi interface
=================================================*/
MS_BOOL MSB1236C_Lock(EN_DEVICE_DEMOD_TYPE system, COFDM_LOCK_STATUS eStatus)
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
            MSB1236C_ReadReg(0x11E0, &cData);//addy update 0805
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
        if (MSB1236C_ReadReg(u16Address, &cData) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        if (MSB1236C_ReadDspReg(u16Address, &cData) == FALSE)
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

EN_LOCK_STATUS MSB1236C_DTV_DVB_T_GetLockStatus(void)
{
    MS_U32    u32Timeout = 0;
    MS_U32    u32LockTimeout = 2000;
    MS_BOOL   bCheckPass = FALSE;
    static MS_U32 u32LockTimeStartDVBT = 0;
    static MS_U32 u32LockTimeStartDVBT2 = 0;
    MS_U32 u32NowTime = MsOS_GetSystemTime();
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;

    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        u32Timeout = 1500;

        if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T2, COFDM_FEC_LOCK) == TRUE)
            bCheckPass = TRUE;
        else
        {
            if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T2, COFDM_P1_LOCK_HISTORY_123X) == TRUE)
            {
                u32Timeout = 6000;
                PRINTE(("====> Short T2 Locking Time, P1 Lock Flag\n"));
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
            //PRINTE(("NowTime: %d  -  pMSB1236C->u32ChkScanTimeStart  %d = %d  \n",
            //    u32NowTime,pMSB1236C->u32ChkScanTimeStart,(u32NowTime-pMSB1236C->u32ChkScanTimeStart)));
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
        u32Timeout = 1500;
        if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
            bCheckPass = TRUE;
        else
        {
            if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T, COFDM_TPS_LOCK) == TRUE)
            {
                PRINTE(("T TPS locked!\n"));
                u32Timeout = 6000;
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
            //PRINTE(("NowTime: %d  -  pMSB1236C->u32ChkScanTimeStart  %d = %d  \n",
            //  u32NowTime,pMSB1236C->u32ChkScanTimeStart,(u32NowTime-pMSB1236C->u32ChkScanTimeStart)));
            // PRINTE(("u32LockTimeout %d \n ",u32LockTimeout));
            pMSB1236C->bFECLock = FALSE;
            return E_DEMOD_CHECKING;
        }
        else
        {
            pMSB1236C->bFECLock = FALSE;
            return E_DEMOD_UNLOCK;
        }
        break;
    }
    default:
        return E_DEMOD_UNLOCK;
    }
}

EN_FRONTEND_SIGNAL_CONDITION MSB1236C_DTV_GetSNR(void)
{
    MS_U32  snr_out = 0;
    MS_U16  snr_ave_num = 0;
    MS_U8   reg = 0;
    float   f_snr = (float)0.0;
    float   fSNRDivider[] = {1024 * 2, 2048 * 2, 4096 * 2, 8192 * 2, 16384 * 2, 32768 * 2, 32768 * 2, 32768 * 2};

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    //float   fsnr= 0;
    MS_U8 status = TRUE;
    EN_FRONTEND_SIGNAL_CONDITION eSignalCondition;
    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:

        // freeze
        status &= MSB1236C_ReadReg(0x2802, &reg);
        reg |= 0x02;
        status &= MSB1236C_WriteReg(0x2802, reg);

        // load
        status &= MSB1236C_ReadReg(0x2802, &reg);
        reg |= 0x04;
        status &= MSB1236C_WriteReg(0x2802, reg);

        status &= MSB1236C_ReadReg(0x280D, &reg);
        snr_out = reg;
        status &= MSB1236C_ReadReg(0x280C, &reg);
        snr_out = (snr_out << 8) | reg;
        status &= MSB1236C_ReadReg(0x280B, &reg);
        snr_out = (snr_out << 8) | reg;
        status &= MSB1236C_ReadReg(0x280A, &reg);
        snr_out = (snr_out << 8) | reg;
        snr_out &= 0x1FFFFFFF;
        if (snr_out == 0)
        {
            snr_out = 1;
        }
        status &= MSB1236C_ReadReg(0x2805, &reg);
        snr_ave_num = (reg >> 5) & 0x07;

        // unfreeze
        status &= MSB1236C_ReadReg(0x2802, &reg);
        reg &= (0xff - 0x02);
        status &= MSB1236C_WriteReg(0x2802, reg);

        f_snr = (float)10.0 * Log10Approx((float)snr_out / fSNRDivider[snr_ave_num]);
        g_MSB1236C_fSNR = f_snr;
        if (f_snr > 42) f_snr = 42;
        if (f_snr < 0) f_snr = 0;
        if (f_snr > 25)
            eSignalCondition = E_FE_SIGNAL_STRONG;
        else if (f_snr > 20)
            eSignalCondition = E_FE_SIGNAL_MODERATE;
        else if (f_snr > 15)
            eSignalCondition = E_FE_SIGNAL_WEAK;
        else
            eSignalCondition = E_FE_SIGNAL_NO;
        break;
    default:
        eSignalCondition = E_FE_SIGNAL_NO;
        break;
    }
    return eSignalCondition;
}

MS_BOOL MSB1236C_DTV_GetPreBER(float *p_preBer)
{
    MS_U16 BitErrPeriod = 0;
    MS_U16 BitErr = 0;
    MS_U8  reg = 0;
    float fber = 0;
    MS_U8 status = TRUE;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    default:
        /////////// Data BER /////////////

        status &= MSB1236C_ReadReg(0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB1236C_ReadReg(0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        status &= MSB1236C_ReadReg(0x2633, &reg);
        BitErr = reg;
        status &= MSB1236C_ReadReg(0x2632, &reg);
        BitErr = (BitErr << 8) | reg;


        if (BitErrPeriod == 0)
        {
            BitErrPeriod = 1;
        }
        status &= MSB1236C_ReadReg(0x278e, &reg);    //FEC Type[8:7]

        if ((reg & 0x80) == 0x80)
        {
            fber = (float)BitErr / (BitErrPeriod * 64800);
        }
        else
        {
            fber = (float)BitErr / (BitErrPeriod * 16200);
        }

        if (BitErr == 0)
        {
            fber = 1E-8;
        }

        *p_preBer = fber;
        printf("MSB1236C Extend Data Pre BER = %8.3e \n ", fber);

        printf("[DVBT2] preber=%f, Err_num=%d, block_count=%d, reg=0x%x\n", fber, BitErr, BitErrPeriod, reg);
        break;
    }
    return status;
}


MS_BOOL MSB1236C_DTV_GetPostBER(float *p_postBer)
{
    MS_U16 BitErrPeriod = 0;
    MS_U16 BitErr = 0;
    MS_U8  reg = 0;
    float fber = 0;
    MS_U8 status = TRUE;
    MS_U16 block_size = 1;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    default:
        /////////// Data BER /////////////

        status &= MSB1236C_ReadReg(0x264b, &reg);
        BitErrPeriod = reg;
        status &= MSB1236C_ReadReg(0x264a, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        status &= MSB1236C_ReadReg(0x264d, &reg);
        BitErr = reg;
        status &= MSB1236C_ReadReg(0x264c, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB1236C_ReadReg(0x278e, &reg);    //FEC Type[8:7]

        if ((reg & 0x80) == 0x80)
        {
            // 64800
            switch (reg & 0x07)
            {
            case 0:
                block_size = 32400;
                break;
            case 1:
                block_size = 38880;
                break;
            case 2:
                block_size = 43200;
                break;
            case 3:
                block_size = 48600;
                break;
            case 4:
                block_size = 51840;
                break;
            case 5:
                block_size = 54000;
                break;
            default:
                block_size = 1;
                break;
            }
        }
        else
        {
            // 16200
            switch (reg & 0x07)
            {
            case 0:
                block_size = 7200;
                break;
            case 1:
                block_size = 9720;
                break;
            case 2:
                block_size = 10800;
                break;
            case 3:
                block_size = 11880;
                break;
            case 4:
                block_size = 12600;
                break;
            case 5:
                block_size = 13320;
                break;
            default:
                block_size = 1;
            }
        }

        if (BitErrPeriod == 0)
        {
            BitErrPeriod = 1;
        }

        fber = (float)BitErr / (BitErrPeriod * block_size);

        if (BitErr == 0)
        {
            fber = 1E-8;
        }

        *p_postBer = fber;

        printf("[DVBT2] postber=%f, Err_num=%d, block_count=%d, block_size=%d\n", fber, BitErr, BitErrPeriod, block_size);
        break;
    }

    return TRUE;
}

MS_U32 MSB1236C_DTV_GetBER(void)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8         status = TRUE;
    MS_U8             reg = 0, reg_frz = 0;
    MS_U16            BitErrPeriod = 0;
    MS_U32            BitErr = 0;
    MS_U16            PktErr = 0;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return 0;
    }
    else if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        /////////// Post-Viterbi BER /////////////

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_ReadReg(0x1132, &reg_frz);
        status &= MSB1236C_WriteReg(0x1132, reg_frz | 0x80);

        // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
        //             [15:8] reg_bit_err_sblprd_15_8
        status &= MSB1236C_ReadReg(0x1131, &reg);
        BitErrPeriod = reg;

        status &= MSB1236C_ReadReg(0x1130, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 7 0x3a [7:0] reg_bit_err_num_7_0
        //             [15:8] reg_bit_err_num_15_8
        // bank 7 0x3c [7:0] reg_bit_err_num_23_16
        //             [15:8] reg_bit_err_num_31_24

        status &= MSB1236C_ReadReg(0x113D, &reg);
        BitErr = reg;

        status &= MSB1236C_ReadReg(0x113C, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB1236C_ReadReg(0x113B, &reg);
        BitErr = (BitErr << 8) | reg;

        status &= MSB1236C_ReadReg(0x113A, &reg);
        BitErr = (BitErr << 8) | reg;

#if 1
        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //             [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB1236C_ReadReg(0x113F, &reg);
        PktErr = reg;

        status &= MSB1236C_ReadReg(0x113E, &reg);
        PktErr = (PktErr << 8) | reg;
#endif

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_WriteReg(0x1132, reg_frz);

        if (BitErrPeriod == 0)  //protect 0
        {
            BitErrPeriod = 1;
        }
        if (status)
        {
            printf(" @MSB1236C_GetPostViterbiBer OK\n");
            return  TRUE;
        }
        else
        {
            printf(" @MSB1236C_GetPostViterbiBer NG\n");
            return FALSE;
        }
    }
    return FALSE;
}

MS_U16 MSB1236C_DTV_GetSignalQuality(void)
{
    MS_U8     status = TRUE;
    MS_U8     reg = 0; //, reg_frz;
    MS_U16    BitErrPeriod = 0;
    MS_U32    BitErr = 0;
    MS_U16    FecType = 0;
    //MS_U8     u8_index = 0;
    float       fber = (float)0.0;
    float       log_ber = (float)0.0;
    float       SQI = (float)0.0;
    //float       BER_SQI = (float)0.0;
    //float       cn_ref = (float)0.0;
    //float       cn_rec = (float)0.0;
    // float       cn_rel = (float)0.0;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        //  E_T2_CONSTEL  constel =  _T2_QAM_UNKNOWN;
        // E_T2_CODERATE coderate = _T2_CR_UNKNOWN;

        /////////// Check lock status /////////////
        MSB1236C_ReadDspReg(0x00f0, &reg);
        if ((reg & BIT7) == 0x00)
        {
            return 0;
        }
        /////////// Data BER /////////////
        // bank 38 0x02 [0] freeze
        status &= MSB1236C_WriteReg(0x2604, 0x01);    // avoid confliction

        // bank 38 0x12 Data BER Window[15:0]
        status &= MSB1236C_ReadReg(0x2625, &reg);
        BitErrPeriod = reg;
        status &= MSB1236C_ReadReg(0x2624, &reg);
        BitErrPeriod = (BitErrPeriod << 8) | reg;

        // bank 38 0x32 Data BER count[15:0]
        // bank 38 0x33 Data BER count[31:16]
        status &= MSB1236C_ReadReg(0x2667, &reg);
        BitErr = reg;
        status &= MSB1236C_ReadReg(0x2666, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(0x2665, &reg);
        BitErr = (BitErr << 8) | reg;
        status &= MSB1236C_ReadReg(0x2664, &reg);
        BitErr = (BitErr << 8) | reg;

        // bank 38 0x02 [0] freeze
        status &= MSB1236C_WriteReg(0x2604, 0x00);     // avoid confliction

        if (BitErrPeriod == 0)
            //protect 0
            BitErrPeriod = 1;

        // status &= MSB1236C_ReadReg(0x268e, &reg);    //FEC Type[8:7]
        status &= MSB1236C_ReadReg(0x278f, &reg);    //FEC Type[8:7]
        FecType = reg;
        status &= MSB1236C_ReadReg(0x278e, &reg);    //FEC Type[8:7]
        FecType = (FecType << 8) | reg;
        // if (reg&0x080)
        if (FecType & 0x0180)
        {
            fber = (float)BitErr / (BitErrPeriod * 64800);
        }
        else
        {
            fber = (float)BitErr / (BitErrPeriod * 16200);
        }
        if (status == FALSE)
        {
            PRINTE(("DTV_GetSignalQuality_DVBT2_GetPostViterbiBer Fail!\n"));
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

        return SQI;
        break;
    }
    case E_DEVICE_DEMOD_DVB_T:
    {
        float fber = 0.0;
        float log_ber = 0.0;
        MS_U8 u8Data = 0;
        if (MSB1236C_ReadReg(0x112C, &u8Data) == FALSE)
        {
            PRINTE(("DTV_DVBT_GetSignalQuality ReadReg fail!!! \n"));
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
            status &= MSB1236C_ReadReg(0x1132, &reg_frz);
            status &= MSB1236C_WriteReg(0x1132, reg_frz | 0x80);
            // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
            //             [15:8] reg_bit_err_sblprd_15_8
            status &= MSB1236C_ReadReg(0x1131, &reg);
            BitErrPeriod = reg;
            status &= MSB1236C_ReadReg(0x1130, &reg);
            BitErrPeriod = (BitErrPeriod << 8) | reg;
            // bank 7 0x3a [7:0] reg_bit_err_num_7_0
            //             [15:8] reg_bit_err_num_15_8
            // bank 7 0x3c [7:0] reg_bit_err_num_23_16
            //             [15:8] reg_bit_err_num_31_24
            status &= MSB1236C_ReadReg(0x113D, &reg);
            BitErr = reg;
            status &= MSB1236C_ReadReg(0x113C, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= MSB1236C_ReadReg(0x113B, &reg);
            BitErr = (BitErr << 8) | reg;
            status &= MSB1236C_ReadReg(0x113A, &reg);
            BitErr = (BitErr << 8) | reg;
            // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
            //             [15:8] reg_uncrt_pkt_num_15_8
            status &= MSB1236C_ReadReg(0x113F, &reg);
            PktErr = reg;
            status &= MSB1236C_ReadReg(0x113E, &reg);
            PktErr = (PktErr << 8) | reg;
            // bank 7 0x32 [7] reg_bit_err_num_freeze
            status &= MSB1236C_WriteReg(0x1132, reg_frz);
            if (BitErrPeriod == 0)
                //protect 0
                BitErrPeriod = 1;
            if (BitErr <= 0)
                fber = 0.5 / ((float)BitErrPeriod * 128 * 188 * 8);
            else
                fber = (float)BitErr / ((float)BitErrPeriod * 128 * 188 * 8);
            PRINTE(("MSB1236C PostVitBER = %8.3e \n ", fber));
            PRINTE(("MSB1236C PktErr = %d \n ", (int)PktErr));

            if (status == FALSE)
            {
                PRINTE(("DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
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
            return SQI;
        }
        else
        {
            return 0;
        }
        break;
    }
    default:
        break;
    }  // end of switch
    return 0;
}

MS_U16 MSB1236C_DTV_GetSignalStrength(void)
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
    MS_U16   ypIFAGC1 = 0, ypIFAGC2 = 0;
    float    ch_power_db = 0;
    float    ch_power_rf = 0;
    float    ch_power_if = 0;
    float    ch_power_ref = 0;
    float    ch_power_rel = 0;
    double fPrel = 0.0, fPinput = 0.0;
    MS_U8  demodState = 0;
    MS_U8  reg = 0;
    S_IFAGC_SSI     *ifagc_ssi;

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    status &= MSB1236C_ReadReg(0x0990, &reg);
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
        PRINTE((">>> RFAGC = %d <<<\n", (int)u8RFAGC));
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
        MSB1236C_WriteReg(0x3e00 + 0x13 * 2, 0x03);
        MSB1236C_WriteReg(0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB1236C_WriteReg(0x3e00 + 0x02 * 2 + 1, 0x80);

        // agc gain2 24 Bit
        //MSB1236C_ReadReg(0x3e00+0x14*2+1, &reg_tmp);
        MSB1236C_ReadReg(0x3e00 + 0x14 * 2 + 2, &reg_tmp);
        ypIFAGC2 = reg_tmp;
        MSB1236C_ReadReg(0x3e00 + 0x14 * 2 + 1, &reg_tmp);
        ypIFAGC1 = reg_tmp;
        MSB1236C_ReadReg(0x3e00 + 0x14 * 2 + 0, &reg_tmp);
        ypIFAGC1 = (ypIFAGC1 << 8) | reg_tmp;
        // printf("============================>[YP] IFAGC_2 = %x, IFAGC_1 = %x\n", ypIFAGC2, ypIFAGC1);
        MSB1236C_ReadReg(0x3e00 + 0x14 * 2 + 1, &reg_tmp);

        MSB1236C_WriteReg(0x3e00 + 0x02 * 2, 0x00); //set freeze & dump [Write]
        MSB1236C_WriteReg(0x3e00 + 0x02 * 2 + 1, 0x00);
        PRINTE((">>> reg_tmp = %x <<<\n", (int)reg_tmp));
        if_agc_val = reg_tmp;
        PRINTE((">>> if_agc_val = %d <<<\n", (int)if_agc_val));

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
        PRINTE(("ch_power_rf = %f\n", ch_power_rf));
        PRINTE(("ch_power_if = %f\n", ch_power_if));
        PRINTE(("ch_power_db = %f\n", ch_power_db));
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

        MSB1236C_ReadReg(0x2700 + (0x47 * 2), &u8Modulation);
        u8Modulation = (((MS_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
        if (u8Modulation >= 3) u8Modulation = 3;
        MSB1236C_ReadReg(0x2700 + (0x47 * 2), &u8CodeRate);
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

        PRINTE((">>> SSI = %d <<<\n", (int)ssi));
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        ///////// RF/IF-AGC Gain Out /////////////
        rf_agc_val = 0x3f;

        // ==== IFAGC_GAIN_OUT (GAIN2) ====

        // reg_agc_debug_sel[3:0]
        MSB1236C_ReadReg(0x3e26, &reg_tmp);
        reg_tmp = (reg_tmp & 0xf0) | 0x03; // select ifagc_gain
        MSB1236C_WriteReg(0x3e26, reg_tmp);

        // reg_agc_gain2_out
        // use only high byte value
        MSB1236C_ReadReg(0x3e29, &reg_tmp);
        if_agc_val = reg_tmp;

        PRINTE(("SSI_RFAGC (SAR) = 0x%x\n", (MS_U16)rf_agc_val));
        PRINTE(("SSI_IFAGC_H = 0x%x\n", (MS_U16) if_agc_val));

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

        PRINTE(("ch_power_rf = %f\n", ch_power_rf));
        PRINTE(("ch_power_if = %f\n", ch_power_if));

        ch_power_db = (ch_power_rf > ch_power_if) ? ch_power_rf : ch_power_if;

        if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
        {
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB1236C_ReadReg(0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB1236C_ReadReg(0x0f24 + 1, &reg_tmp);
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

        PRINTE((">>> SSI = %d <<<\n", (int)ssi));

    }
    break;
    default:
        ssi = 0;
        PRINTE(("Undefined!!!\n"));
        break;
    }
    return ssi;
}

#if(FRONTEND_TUNER_TYPE == TUNER_MXL603)
MS_U16 MSB1236C_DTV_GetSignalStrengthWithRFPower(float dBmValue)
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

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    status &= MSB1236C_ReadReg(0x0990, &reg);
    demodState = reg;
    switch (pMSB1236C->enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T2:
    {
        fPinput = dBmValue;

        // Get Modulation and code Rate
        MSB1236C_ReadReg(0x2700 + (0x47 * 2), &u8Modulation);
        u8Modulation = (((MS_U16) u8Modulation) & (BIT5 | BIT4 | BIT3)) >> 3;
        if (u8Modulation >= 3) u8Modulation = 3;
        MSB1236C_ReadReg(0x2700 + (0x47 * 2), &u8CodeRate);
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

        PRINTE((">>> SSI = %d <<<\n", (int)ssi));
    }
    break;
    case E_DEVICE_DEMOD_DVB_T:
    {
        if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
        {
            ///////// Get Constellation and Code Rate to determine Ref. Power //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.6) /////

            status &= MSB1236C_ReadReg(0x0f24, &reg_tmp);
            tps_cnstl = reg_tmp & 0x03;

            status &= MSB1236C_ReadReg(0x0f24 + 1, &reg_tmp);
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

        PRINTE((">>> SSI = %d <<<\n", (int)ssi));

    }
    break;
    default:
        ssi = 0;
        PRINTE(("Undefined!!!\n"));
        break;
    }
    return ssi;
}
#endif

EN_DEVICE_DEMOD_TYPE MSB1236C_GetCurrentDemodulatorType(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    return pMSB1236C->enDemodType ;
}

MS_BOOL MSB1236C_SetCurrentDemodulatorType(EN_DEVICE_DEMOD_TYPE enDemodType)
{

    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (enDemodType == pMSB1236C->enDemodType)
    {
        return TRUE;
    }
    pMSB1236C->enDemodType = enDemodType;
    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();

    switch (enDemodType)
    {
    case E_DEVICE_DEMOD_DVB_T:
        MDrv_DMD_MSB123xc_SetCurrentDemodulatorType(E_DMD_MSB123xc_DEMOD_DVBT);
        break;
    case E_DEVICE_DEMOD_DVB_T2:
        MDrv_DMD_MSB123xc_SetCurrentDemodulatorType(E_DMD_MSB123xc_DEMOD_DVBT2);
    default:
        break;

    }
    return MDrv_DMD_MSB123xc_LoadDSPCode();
}

MS_BOOL MSB1236C_Active(MS_BOOL bEnable)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));

    UNUSED(bEnable);
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }


    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return TRUE;
    }
    else if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {

        MS_BOOL status = MSB1236C_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN

        if (status)
        {
            PRINTE((" @MSB1236C_MSB1236C_Active OK\n"));
        }
        else
        {
            PRINTE((" @MSB1236C_MSB1236C_Active NG\n"));
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
MS_BOOL MSB1236C_DTV_GetPlpBitMap(MS_U8* u8PlpBitMap)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL   status = TRUE;
    MS_U8     u8Data = 0;
    MS_U8     indx = 0;

    status &= MSB1236C_ReadDspReg(0x0120, &u8Data);     // check L1 ready
    if (u8Data != 0x30)
        return FALSE;

    while (indx < 32)
    {
        status &= MSB1236C_ReadDspReg(0x0100 + indx, &u8Data);
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

/************************************************************************************************
Subject:    Get PlpID List and PlpID Size for MPLP mode
Function:   MSB1236C_GetPlpIDList
Parmeter:   u8PlpID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB1236C_GetPlpIDList(MS_U8* u8PlpID)
{
    MS_BOOL bRet;
    MS_U8 i, j, u8PlpBitMap[32];
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    pMSB1236C->PlpIDSize = 0;
    memset(u8PlpBitMap, 0xff, sizeof(u8PlpBitMap));
    bRet = MSB1236C_DTV_GetPlpBitMap(u8PlpBitMap);
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((u8PlpBitMap[i] >> j) & 1)
            {
                u8PlpID[pMSB1236C->PlpIDSize] = i * 8 + j;
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
MS_BOOL MSB1236C_DTV_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    PRINTE(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_BOOL   status = TRUE;
    MS_U8 u8Data = 0;
    status &= MSB1236C_ReadDspReg(0x0120, &u8Data);         // check L1 ready
    if (u8Data != 0x30)
    {
        PRINTE(("[MSB1236C]dvbt2 L1 not ready yet\n"));
        return FALSE;
    }
    MSB1236C_ReadDspReg(0x00f0, &u8Data);

    if ((u8Data & BIT7) == 0x00)
    {
        PRINTE(("[MSB1236C]dvbt2 is un-lock\n"));
        return FALSE;
    }
    // assign PLP-ID value
    status &= MSB1236C_WriteReg(0x2700 + (0x78) * 2, u8PlpID);
    status &= MSB1236C_WriteReg(0x2700 + (0x01) * 2 + 1, 0x01); // MEM_EN
    MSB1236C_Delayus(1000);
    status &= MSB1236C_ReadReg(0x2700 + (0x79) * 2, u8GroupID);
    status &= MSB1236C_WriteReg(0x2700 + (0x01) * 2 + 1, 0x00); // ~MEM_EN

    return status;
}


/************************************************************************************************
Subject:    Select PLP_ID, GROUP_ID for MPLP mode
Function:   DTV_SetPlpGroupID
Parmeter:   u8PlpID, u8GroupID
Return:     BOOLEAN :
Remark:
 *************************************************************************************************/
MS_BOOL MSB1236C_DTV_SetPlpGroupID(MS_U8 u8PlpID, MS_U8 u8GroupID)
{

    MS_BOOL   status = TRUE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    pMSB1236C->u32ChkScanTimeStart = MsOS_GetSystemTime();
    PRINTE(("[start]\n"));

    // assign Group-ID and PLP-ID value (must be written in order)
    status &= MSB1236C_WriteDspReg(0x0122, u8GroupID);
    status &= MSB1236C_WriteDspReg(0x0121, u8PlpID);
    PRINTE(("[end, return %d]\n", status));
    return status;
}

MS_BOOL MSB1236C_T2_SetPlpID(MS_U8 u8PlpID)
{
    MS_BOOL bRet = FALSE;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
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
        MSB1236C_ReadReg(u16RegAddress, &u8Data);
        if (u8Data == u8LockState)
            bCheckPass = TRUE;
        if ((u8PlpID != 0xFF) && (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2) && (bCheckPass))
        {

            MS_U16 u16Retry = 0;
            MS_U8 u8GroupId = 0;
            MSB1236C_Delayus(500 * 1000);

            bRet = MSB1236C_DTV_GetPlpGroupID(u8PlpID, &u8GroupId);
            while ((bRet == FALSE) && (u16Retry < 60))
            {
                u16Retry++;
                PRINTE(("DoSet_DVBT2 get groupid retry %d \n", u16Retry));
                MSB1236C_Delayus(100 * 1000);
                bRet = MSB1236C_DTV_GetPlpGroupID(u8PlpID, &u8GroupId);
            }
            if (bRet == FALSE)
            {
                PRINTE(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_GetPlpGroupID(%d) Error \n", u8PlpID));
                return FALSE;
            }

            bRet = MSB1236C_DTV_SetPlpGroupID(u8PlpID, u8GroupId);
            if (bRet == FALSE)
            {
                PRINTE(("MW_DTV_FrontEnd_DVBT::DoSet_DVBT2() DTV_DVB_T2_SetPlpGroupID(%d,%d) Error", u8PlpID, u8GroupId));
                return FALSE;
            }
        }
    }
    return TRUE;
}


// DVBT2 95~101ms, DVBT 38~39ms
MS_BOOL MSB1236C_DTV_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth, MS_U8 u8PlpID)
{
    MS_U8 bw_t2 = 0, bw_t = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    printf("\n[%s]->freq=%d,bdwh=%d,PlpID %d,type %s\n", __FUNCTION__, u32Frequency, eBandWidth, u8PlpID, E_DEVICE_DEMOD_DVB_T == pMSB1236C->enDemodType ? "T" : "T2");


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

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        printf("\nsetfreq T2\n");
        MSB1236C_Reset();
        MSB1236C_Delayus(500);
        MSB1236C_WriteDspReg((MS_U16)E_T2_BW, bw_t2);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB1236C_WriteDspReg((MS_U16)E_T2_PLP_ID, u8PlpID);
        //MSB1236C_WriteDspReg((MS_U16)T2_TH_A_NP1, T2_TH_A_NP1_VAL);
        MSB1236C_WriteDspReg((MS_U16)E_T2_IF_AGC_INV_PWM_EN, 0);
        MSB1236C_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }
    else if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        printf("\nsetfreq T\n");
        MSB1236C_Reset();
        MSB1236C_Delayus(500);

        MSB1236C_WriteDspReg((MS_U8)T_CONFIG_BW, bw_t);      // BW: 1->6M, 2->7M, 3->8M

        pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_CONFIG_BW - (MS_U8)T_OPMODE_RFAGC_EN] = (MS_U8)bw_t;
        pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN] = MDrv_DMD_MSB123xc_DTV_DVBT_DSPReg_CRC();
        MSB1236C_WriteDspReg((MS_U8)T_PARAM_CHECK_SUM, pMSB1236C->pMSB1236C_RegDVBT[(MS_U8)T_PARAM_CHECK_SUM - (MS_U8)T_OPMODE_RFAGC_EN]);    // BW: 1->6M, 2->7M, 3->8M
        MSB1236C_Active(1);
    }

#if(TIMING_VERIFICATION == 1)
    tmm_12 = GIVE_ME_TIME;
    show_timer();
#endif
    return TRUE;
}

/*================================================
==         demod  MSB1236C getting dvbt/dvbt2 param interface
=================================================*/
MS_BOOL MSB1236C_DTV_DVB_T_Get_TPS_Parameter(MS_U16 * pu16TPS_parameter, E_SIGNAL_TYPE eSignalType)
{
    MS_U8         u8Data = 0;

    if (MSB1236C_Lock(E_DEVICE_DEMOD_DVB_T, COFDM_FEC_LOCK) == TRUE)
    {

        if (eSignalType == TS_MODUL_MODE)
        {

            if (MSB1236C_ReadReg(0x2700 + (0x47 * 2), &u8Data) == FALSE)
                return FALSE;

            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT5 | BIT4 | BIT3)) >> 3;

        }

        else  if (eSignalType == TS_CODE_RATE)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x47 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT2 | BIT1 | BIT0));

        }

        else if (eSignalType == TS_GUARD_INTERVAL)
        {
            if (MSB1236C_ReadReg(0x2400 + 0x40 * 2 + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter |= (((MS_U16) u8Data) & (BIT6 | BIT5 | BIT4)) >> 4 ;

        }

        else if (eSignalType == TS_FFT_VALUE)
        {
            if (MSB1236C_ReadReg(0x2400 + 0x40 * 2 + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter |= ((MS_U16) u8Data) & (BIT2 | BIT1 | BIT0);

            if ((*pu16TPS_parameter) == DEV_MSB1236C_FFT_32K_)
                *pu16TPS_parameter = DEV_MSB1236C_FFT_32K;

            if ((*pu16TPS_parameter) == DEV_MSB1236C_FFT_8K_)
                *pu16TPS_parameter = DEV_MSB1236C_FFT_8K;

            if (MSB1236C_ReadReg(0x2510, &u8Data)  == FALSE)
            {
                return FALSE;
            }

            *pu16TPS_parameter |= (((MS_U16) u8Data) & BIT3) ;
            return FALSE;
        }
        else if (eSignalType == TS_PREAMBLE)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT4)) >> 4;
        }
        else if (eSignalType == TS_S1_SIGNALLING)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT3 | BIT2 | BIT1)) >> 1;
        }
        else if (eSignalType == TS_PILOT_PATTERN)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x36 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & 0x0F);
        }
        else if (eSignalType == TS_BW_EXT)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT0));
        }
        else if (eSignalType == TS_PAPR_REDUCTION)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & 0xF0) >> 4;
        }
        else if (eSignalType == TS_OFDM_SYMBOLS_PER_FRAME)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x3C * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (MS_U16) u8Data;
            if (MSB1236C_ReadReg(0x2700 + (0x3C * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter |= (((MS_U16) u8Data) & 0x0F) << 8;
        }
        else if (eSignalType == TS_T2_FFT_VALUE)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x30 * 2) + 1, &u8Data) == FALSE)
            {
                return FALSE;
            }
            if ((u8Data & (BIT3 | BIT2)) >> 2)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT7 | BIT6 | BIT5)) >> 5;
        }
        else if (eSignalType == TS_T2_GUARD_INTERVAL)
        {
            if (MSB1236C_ReadReg(0x2700 + (0x31 * 2), &u8Data) == FALSE)
            {
                return FALSE;
            }
            *pu16TPS_parameter  = (((MS_U16) u8Data) & (BIT3 | BIT2 | BIT1)) >> 1;
        }
        else
        {
            return FALSE;
        }

        return TRUE;

    }
    return FALSE;
}

MS_BOOL MSB1236C_DTV_DVB_T_GetSignalTpsInfo(MS_U16 *TPS_Info)
{
    MS_U16 u16Data;
    MS_BOOL bRet = TRUE;
    MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_CODE_RATE);
    *TPS_Info |= (u16Data & 0x07);
    MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_MODUL_MODE);
    *TPS_Info |= ((u16Data & 0x07) << 3);
    MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_GUARD_INTERVAL);
    *TPS_Info |= ((u16Data & 0x07) << 6);
    MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_FFT_VALUE);
    *TPS_Info |= ((u16Data & 0x07) << 9);
    return bRet;
}

MS_BOOL MSB1236C_DTV_DVB_T2_GetSignalTpsInfo(MS_U64 *TPS_Info)
{
    MS_BOOL bRet = TRUE;
    MS_U16    u16Data = 0;
#if MSB1236C_DEBUG
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
    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_MODUL_MODE) == FALSE)
    {
        PRINTE(("TS_MODUL_MODE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *TPS_Info = (MS_U64)(u16Data);
    PRINTE(("Constellation:%s\n", cConStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_CODE_RATE) == FALSE)
    {
        PRINTE(("TS_CODE_RATE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *TPS_Info |= (MS_U64)(u16Data << 3);
    PRINTE(("Code Rate:%s\n", cCRStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_T2_GUARD_INTERVAL) == FALSE)
    {
        PRINTE(("TS_T2_GUARD_INTERVAL Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *TPS_Info |= (MS_U64)(u16Data << 6);
    PRINTE(("GI:%s\n", cGIStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_T2_FFT_VALUE) == FALSE)
    {
        PRINTE(("TS_T2_FFT_VALUE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    *TPS_Info |= (MS_U64)(u16Data << 9);
    PRINTE(("FFT:%s\n", cFFTStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_PREAMBLE) == FALSE)
    {
        PRINTE(("TS_PREAMBLE Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x01;
    *TPS_Info |= (MS_U64)(u16Data << 12);
    PRINTE(("Preamble:%s\n", cPreAStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_S1_SIGNALLING) == FALSE)
    {
        PRINTE(("TS_S1_SIGNALLING Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x03;
    if (u16Data > 2)
        u16Data = 3;
    *TPS_Info |= (MS_U64)(u16Data << 13);
    PRINTE(("S1Signalling:%s\n", cS1SStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_PILOT_PATTERN) == FALSE)
    {
        PRINTE(("TS_PILOT_PATTERN Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x0F;
    if (u16Data > 7)
        u16Data = 8;
    *TPS_Info |= (MS_U64)(u16Data << 15);
    PRINTE(("PilotPattern:%s\n", cPPSStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_BW_EXT) == FALSE)
    {
        PRINTE(("TS_BW_EXT Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x01;
    *TPS_Info |= (MS_U64)(u16Data << 19);
    PRINTE(("BW EXT:%s\n", cBWStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_PAPR_REDUCTION) == FALSE)
    {
        PRINTE(("TS_PAPR_REDUCTION Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0x07;
    if (u16Data > 3)
        u16Data = 4;
    *TPS_Info |= (MS_U64)(u16Data << 20);
    PRINTE(("PAPR:%s\n", cPAPRStr[u16Data]));

    if (MSB1236C_DTV_DVB_T_Get_TPS_Parameter(&u16Data, TS_OFDM_SYMBOLS_PER_FRAME) == FALSE)
    {
        PRINTE(("TS_OFDM_SYMBOLS_PER_FRAME Error!\n"));
        bRet = FALSE;
    }
    u16Data &= 0xFFF;
    *TPS_Info |= (MS_U64)(u16Data << 23);
    PRINTE(("OFDM Symbols:%u\n", u16Data));

    return bRet;
}

MS_U16 MSB1236C_DTV_GetCellID(void)
{
    MS_U8 id = 0;
    MS_U8 status = TRUE;
    MS_U16  cell_id  = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {
        return 0;
    }
    else if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T)
    {
        status &= MSB1236C_WriteReg(0x0ffe, 0x01);

        status &= MSB1236C_ReadReg(0x0f2b, &id);
        cell_id = (MS_U16)id;

        status &= MSB1236C_ReadReg(0x0f2a, &id);
        cell_id |= (MS_U16)id << 8;

        status &= MSB1236C_WriteReg(0x0ffe, 0x00);

        if (status)
        {
            printf(" @MSB1236C_GetCellId OK\n");
            return  TRUE;
        }
        else
        {
            printf(" @MSB1236C_GetCellId NG\n");
            return FALSE;
        }
    }
    return cell_id;

}

MS_BOOL MSB1236C_DTV_GetSNR_F(float *fSNR)
{
    MSB1236C_DTV_GetSNR();
    *fSNR = g_MSB1236C_fSNR;
    return TRUE;
}

MS_BOOL MSB1236C_DTV_Get_Current_Plp_Id(MS_U8 *plp_id)
{
    MS_BOOL  rbet = TRUE;
    MS_U8    reg;

    rbet &= MSB1236C_ReadDspReg((MS_U16)E_T2_PLP_ID, &reg);
    *plp_id = reg;
    return rbet;
}

void MSB1236C_DTV_ControlTsOutput(MS_BOOL bEnable)
{
    if (bEnable)
    {
        MSB1236C_WriteReg(0x0900 + (0x2D * 2), 0xFF);
    }
    else
    {
        MSB1236C_WriteReg(0x0900 + (0x2D * 2), 0x00);
    }
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U16 MSB1236C_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    MS_U8 status = TRUE;
    //DVB-T
    float   N, FreqB;
    float   FreqCfoTd, FreqCfoFd, FreqIcfo;
    MS_U32  RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8 reg_frz, reg, reg_debug_sel;
    MS_S32 RegTotalCfo;
    EN_DEVICE_DEMOD_TYPE eSystems;

    FreqB = (float)u8BW * 8.0 / 7.0;
    eSystems = MSB1236C_GetCurrentDemodulatorType();
    PRINTE(("\nBW = %d\n", u8BW));
    if (eSystems == E_DEVICE_DEMOD_DVB_T2)
    {
        //debug select
        status &= MSB1236C_ReadReg(0x3E5E, &reg_debug_sel);
        reg_debug_sel &= 0xCF;
        reg_debug_sel |= 0x10;
        status &= MSB1236C_WriteReg(0x3E5E, reg_debug_sel);

        // set freeze
        status &= MSB1236C_WriteReg2bytes(0x3E00 + (0x02) * 2, 0x8000);
        status &= MSB1236C_ReadReg(0x3E5B, &reg);
        RegTotalCfo = reg & 0x0F;

        status &= MSB1236C_ReadReg(0x3E5A, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;
        status &= MSB1236C_ReadReg(0x3E59, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;
        status &= MSB1236C_ReadReg(0x3E58, &reg);
        RegTotalCfo = (RegTotalCfo << 8) | reg;

        // release freeze
        status &= MSB1236C_WriteReg2bytes(0x3E00 + (0x02) * 2, 0x0000);

        if (RegTotalCfo >= 0x4000000)
            RegTotalCfo = RegTotalCfo - 0x8000000;

        *pFreqOff = (float)RegTotalCfo * FreqB * 0.00000745;

        PRINTE(("$$$$$$$$$$$$$$$$$$$$  DVB-T2 CFOE = %f\n", *pFreqOff));
        return status;
    }
    else if (eSystems == E_DEVICE_DEMOD_DVB_T)
    {
        // freeze
        status &= MSB1236C_ReadReg(0x0E04 + 1, &reg_frz);
        status &= MSB1236C_WriteReg(0x0E04 + 1, reg_frz | 0x80);

        status &= MSB1236C_ReadReg(0x0E28 + 2, &reg);
        RegCfoTd = reg;

        status &= MSB1236C_ReadReg(0x0E28 + 1, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        status &= MSB1236C_ReadReg(0x0E28, &reg);
        RegCfoTd = (RegCfoTd << 8) | reg;

        // release
        status &= MSB1236C_ReadReg(0x0E04 + 1, &reg_frz);
        status &= MSB1236C_WriteReg(0x0E04 + 1, reg_frz | 0x80);

        FreqCfoTd = (float)RegCfoTd;

        if (RegCfoTd & 0x800000)
            FreqCfoTd = FreqCfoTd - (float)0x1000000;

        FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

        status &= MSB1236C_ReadReg(0x0ffe, &reg_frz);
        status &= MSB1236C_WriteReg(0x0ffe, reg_frz | 0x01);

        status &= MSB1236C_WriteReg(0x0fff, 0x01);

        status &= MSB1236C_ReadReg(0x0f33, &reg);
        RegCfoFd = reg;

        status &= MSB1236C_ReadReg(0x0f32, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        status &= MSB1236C_ReadReg(0x0f31, &reg);
        RegCfoFd = (RegCfoFd << 8) | reg;

        FreqCfoFd = (float)RegCfoFd;

        if (RegCfoFd & 0x800000)
            FreqCfoFd = FreqCfoFd - (float)0x1000000;

        FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

        status &= MSB1236C_ReadReg(0x0f19, &reg);
        RegIcfo = reg & 0x07;

        status &= MSB1236C_ReadReg(0x0f18, &reg);
        RegIcfo = (RegIcfo << 8) | reg;

        FreqIcfo = (float)RegIcfo;

        if (RegIcfo & 0x400)
            FreqIcfo = FreqIcfo - (float)0x800;

        status &= MSB1236C_ReadReg(0x0f26, &reg);
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
        status &= MSB1236C_WriteReg(0x0ffe, reg_frz & (~0x01));
        status &= MSB1236C_WriteReg(0x0fff, 0x01);

        *pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd) / 1000;

        status &= MSB1236C_ReadReg(0x3E5E, &reg);

        if (reg & 0x01)
            *pFreqOff = -*pFreqOff;

        PRINTE(("$$$$$$$$$$$$$$$$$$$$  DVB-T CFOE = %f\n", *pFreqOff));

    }
    else
        ;

    if (status)
    {
        PRINTE((" @MSB1236C_Get_FreqOffset OK\n"));
        return  TRUE;
    }
    else
    {
        PRINTE((" @MSB1236C_Get_FreqOffset NG\n"));
        return FALSE;
    }
}

MS_U8 MSB1236C_Get_Packet_Error(MS_U16 *u16_data)
{
    MS_U8     status = TRUE;
    MS_U8     reg = 0, reg_frz;
    MS_U16      PktErr = 0;

    if (E_DEVICE_DEMOD_DVB_T2 == MSB1236C_GetCurrentDemodulatorType())
    {
        //freeze
        status &= MSB1236C_WriteReg(0x2604, 0x01);
        //read packet error
        status &= MSB1236C_ReadReg(0x265B, &reg);
        PktErr = reg;
        status &= MSB1236C_ReadReg(0x265A, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;
        //release
        status &= MSB1236C_WriteReg(0x2604, 0x00);
    }
    else
    {
        reg_frz = 0;
        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_ReadReg(0x1132, &reg_frz);
        status &= MSB1236C_WriteReg(0x1132, reg_frz | 0x80);

        // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
        //         [15:8] reg_uncrt_pkt_num_15_8
        status &= MSB1236C_ReadReg(0x113F, &reg);
        PktErr = reg;

        status &= MSB1236C_ReadReg(0x113E, &reg);
        PktErr = (PktErr << 8) | reg;

        *u16_data = PktErr;

        // bank 7 0x32 [7] reg_bit_err_num_freeze
        status &= MSB1236C_WriteReg(0x1132, reg_frz);

        PRINTE(("===================>MSB PktErr = %d \n ", (int)PktErr));
    }
    if (status)
    {
        PRINTE((" @MSB1236C_Get_Packet_Error OK\n"));
        return  TRUE;
    }
    else
    {
        PRINTE((" @MSB1236C_Get_Packet_Error NG\n"));
        return FALSE;
    }
}

MS_U16 MDrv_Demod_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1236C_Get_FreqOffset(pFreqOff, u8BW);

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_U8 MDrv_Demod_Get_Packet_Error(MS_U16 *u16_data)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1236C_Get_Packet_Error(u16_data);

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetScanTypeStatus(MS_U8 status)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
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

MS_U8 MDrv_Demod_GetScanTypeStatus(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }

    return pMSB1236C->u8ScanStatus;
}

MS_BOOL MDrv_cofdmDmd_InitParameter(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    pMSB1236C->PlpIDSize = 0;
    pMSB1236C->u8ScanStatus = 0;
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

MS_U8 MDrv_Demod_GetPlpIDSize(void)
{
    MS_U8    u8Size = 0;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("pMSB1236C have not inited !!!"));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }
    u8Size = pMSB1236C->PlpIDSize;
    PRINTE(("[end],PlpID size %d", u8Size));
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return u8Size;
}

MS_U8 MApi_FE_GetPlpIDList(MS_U8* u8PlpID)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return 0;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return 0;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    if (0 == pMSB1236C->PlpIDSize)
    {
        if (FALSE == MSB1236C_GetPlpIDList(u8PlpID))
            pMSB1236C->PlpIDSize = 0;
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return pMSB1236C->PlpIDSize;
}

MS_BOOL MApi_FE_ClearPlpList(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!"));
        return FALSE;
    }

    pMSB1236C->PlpIDSize = 0;

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

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
        return MSB1236C_SetCurrentDemodulatorType(E_DEVICE_DEMOD_DVB_T);
    }
    else
    {
        mode.Fetype = E_TUNE_TYPE_T2;
        MDrv_Tuner_SetTunerMode(mode);
        return MSB1236C_SetCurrentDemodulatorType(E_DEVICE_DEMOD_DVB_T2);
    }
}

MS_U8 msAPI_GetDemodType(void)
{
    if (E_DEVICE_DEMOD_DVB_T == MSB1236C_GetCurrentDemodulatorType())
        return E_DEMOD_TYPE_T;
    else
        return E_DEMOD_TYPE_T2;
}

MS_BOOL MDrv_Demod_Open(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB1236C->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("Obtain mutex failed !!!\n"));
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
MS_U32 MApi_FE_GetSNR(void)
{
    MS_U32 pu32SNR;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        pu32SNR = 0;
        if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus())
        {
            pu32SNR  = MSB1236C_DTV_GetSignalQuality();
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
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return pu32SNR;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U16 MApi_FE_GetSignalQuality(void)
{
    MS_U16 u16Quality;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;

    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetBER:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Quality = 0;
    if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus())
    {
        u16Quality = (MS_U16)MSB1236C_DTV_GetSignalQuality();
    }
    if (u16Quality > 100)
    {
        u16Quality = 100;
    }
    else if (u16Quality < 0)
    {
        u16Quality = 0;
    }
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);

    return u16Quality;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *ps32Signal = 0;
        if (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus())
        {
#if(FRONTEND_TUNER_TYPE == TUNER_MXL603)
            float fSignal;

            MSB1236C_IIC_Bypass_Mode(TRUE);
            if (MXL_SUCCESS != MxLWare603_API_ReqTunerRxPower(MXL603_I2C_ADDR, &fSignal))
            {
                PRINTE(("MxLWare603_API_ReqTunerRxPower failed !!!\n"));
                MSB1236C_IIC_Bypass_Mode(FALSE);
                return FALSE;
            }
            MSB1236C_IIC_Bypass_Mode(FALSE);

            *ps32Signal = (MS_U32)MSB1236C_DTV_GetSignalStrengthWithRFPower(fSignal);
#else
            *ps32Signal  = (MS_U32)MSB1236C_DTV_GetSignalStrength();
#endif
        }
        if (*ps32Signal > 100)
            *ps32Signal = 100;
        else if (*ps32Signal < 0)
            *ps32Signal = 0;
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

EN_LOCK_STATUS MApi_FE_GetT2LockStatus(void)
{
    EN_LOCK_STATUS LockStatus = E_DEMOD_UNLOCK;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetLockStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        LockStatus = MSB1236C_DTV_DVB_T_GetLockStatus();
    }
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return LockStatus;
}

MS_BOOL  MApi_FE_CheckLock(void)
{
    MS_BOOL pbLock;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        pbLock = (E_DEMOD_LOCK == MSB1236C_DTV_DVB_T_GetLockStatus()) ? TRUE : FALSE;
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return pbLock;
}

void MApi_FE_SetT2Param(MS_U8 u8ScanType, MS_U8 u8PlpId)
{
    u8Type = u8ScanType;
    u8Plp = u8PlpId;
}
/****************************************************************************
**function:reset bw and Plpid to demod when plp changed
**
**
****************************************************************************/
MS_BOOL MApi_FE_T2MI_Restart(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    MS_U8    PlpId, BandWidth;

    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return FALSE;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    if (E_DEVICE_DEMOD_DVB_T2 != pMSB1236C->enDemodType)
    {
        MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
        return TRUE;
    }

    MSB1236C_ReadDspReg((MS_U16)E_T2_PLP_ID, &PlpId);
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
        PRINTE(("Curr_PLP_ID=%d, GET_PLP_ID=%d\n", pMSB1236C->u8CurrPlpID, PlpId));
        MSB1236C_WriteDspReg((MS_U16)E_T2_BW, BandWidth);      // BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
        MSB1236C_WriteDspReg((MS_U16)E_T2_PLP_ID, pMSB1236C->u8CurrPlpID);
        MSB1236C_WriteReg(0x0C00 + (0x0E) * 2, 0x0001); // FSM_EN
    }

    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
    return TRUE;
}

void MApi_FE_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth)
{
    MS_U32              dwError = TRUE;
    MS_U8               BandWidth = DEMOD_BW_MODE_8MHZ;
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (!pMSB1236C)
    {
        PRINTE(("pMSB1236C error !\n"));
        return ;
    }
    if ((!pMSB1236C->bInited) || (!pMSB1236C->bOpen))
    {
        PRINTE(("[%s]pMSB1236C have not inited !!!\n", __FUNCTION__));
        return ;
    }
    if (MsOS_ObtainMutex(pMSB1236C->s32_MSB1236C_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        PRINTE(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return ;
    }

    eBandWidth -= 1;
    pMSB1236C->enTuneBw = eBandWidth;
    printf("\n[%s]tuner demod Freq=%d, Bw=%d,type=%d,plpid=%d\n", __FUNCTION__, u32Frequency, eBandWidth , u8Type, u8Plp);

    if (DEMOD_BW_MODE_1_7MHZ == eBandWidth)
    {
        MDrv_Demod_SetCurrentDemodType(E_DEMOD_TYPE_T2);
    }
    else if ((E_DEMOD_TYPE_T == u8Type) && (pMSB1236C->u8CurrScanType != u8Type))
    {
        MDrv_Demod_SetCurrentDemodType(E_DEMOD_TYPE_T);
    }
    else if ((E_DEMOD_TYPE_T2 == u8Type) && (pMSB1236C->u8CurrScanType != u8Type))
    {
        MDrv_Demod_SetCurrentDemodType(E_DEMOD_TYPE_T2);
    }
    else
        ;

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

    if ((TRUE == pMSB1236C->bFECLock) && (pMSB1236C->u8CurrScanType == u8Type) && \
            (E_DEMOD_TYPE_T2 == pMSB1236C->u8CurrScanType) && (pMSB1236C->u32CurrFreq == u32Frequency) && \
            (pMSB1236C->enCurrBW == eBandWidth))
    {
    }
    else
    {
        extern  MS_U32 MDrv_Tuner_SetTuner(MS_U32 u32Freq, MS_U8 feBw);
        //set freq and bw to tuner
        MSB1236C_IIC_Bypass_Mode(TRUE);
        MDrv_Tuner_SetTuner(u32Frequency, BandWidth);
        MSB1236C_IIC_Bypass_Mode(FALSE);
    }

    pMSB1236C->u32CurrFreq = u32Frequency;
    pMSB1236C->enCurrBW = eBandWidth;
    pMSB1236C->u8CurrPlpID = u8Plp;
    pMSB1236C->u8CurrScanType = u8Type;

    dwError = MSB1236C_DTV_SetFrequency(u32Frequency, eBandWidth, u8Plp);
    if (E_DEMOD_TYPE_T2 == pMSB1236C->enDemodType)
    {
        dwError &= MSB1236C_T2_SetPlpID(u8Plp);
    }
    PRINTE((" ==return %d==\n", dwError));
    MsOS_ReleaseMutex(pMSB1236C->s32_MSB1236C_Mutex);
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

void MDrv_Demod_CtrlResetDJBFlag(MS_U8 Ctrl)
{
    MDvr_CofdmDmd_CONFIG *pMSB1236C = &MSB1236C;
    if (pMSB1236C->enDemodType == E_DEVICE_DEMOD_DVB_T2)
    {

    }
}

void MApi_FE_Power_On_Initialization(void)
{
    MS_U8     status = TRUE;
    sDMD_MSB123xc_InitData sMSB1236_InitData;
	extern MS_BOOL MDrv_SYS_Init(void);
	MDrv_SYS_Init();//for chipID bonding

    if (MSB1236C.bInited)
        return ;
    MSB1236C.bInited = TRUE;
    MSB1236C.bOpen = TRUE;
    MSB1236C.u8sramCode = 0;
    MSB1236C.PlpIDSize = 0;
    MSB1236C.bFECLock = FALSE;
    MSB1236C.u8ScanStatus = 0;
    MSB1236C.u32CurrFreq = 0;
    MSB1236C.enCurrBW = DEMOD_BW_MODE_8MHZ;
    MSB1236C.enTuneBw = DEMOD_BW_MODE_8MHZ;
    MSB1236C.u8CurrPlpID = 0xff;
    MSB1236C.enDemodType = E_DEVICE_DEMOD_DVB_T;
    MSB1236C.u8CurrScanType = E_DEMOD_TYPE_T;
    MSB1236C.pMSB1236C_RegDVBT = MSB1236c_DVBT_DSPREG_TABLE ;

    MSB1236C.s32_MSB1236C_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex DVBT2", MSOS_PROCESS_SHARED);
    if (MSB1236C.s32_MSB1236C_Mutex < 0)
    {
        PRINTE(("creat MSB1236C Mutex error!!!\n"));
        return ;
    }
    void MApi_Demod_HWReset(void);
    MApi_Demod_HWReset();

    sMSB1236_InitData.u8WO_SPI_Flash = TRUE;
    sMSB1236_InitData.bPreloadDSPCodeFromMainCHIPI2C = FALSE;
    sMSB1236_InitData.bFlashWPEnable = TRUE;
    sMSB1236_InitData.fpGPIOReset = NULL;
    sMSB1236_InitData.fpMSB123xc_I2C_Access = msb1236c_I2C_Access;
    sMSB1236_InitData.pDVBC_DSP_REG = NULL;
    sMSB1236_InitData.pDVBT_DSP_REG = MSB1236c_DVBT_DSPREG_TABLE;
    sMSB1236_InitData.pDVBT2_DSP_REG = MSB1236c_DVBT2_DSPREG_TABLE;

    status &= MDrv_DMD_MSB123xc_Init(&sMSB1236_InitData, sizeof(sDMD_MSB123xc_InitData));
    if (status)
    {
        MDrv_DMD_MSB123xc_SetCurrentDemodulatorType(E_DMD_MSB123xc_DEMOD_DVBT);
        status &= MDrv_DMD_MSB123xc_Power_On_Initialization();
    }

    if (!status)
    {
        MsOS_DeleteMutex(MSB1236C.s32_MSB1236C_Mutex);
        return ;
    }

    MSB1236C_Show_Version();
#if TS_PARALLEL_OUTPUT
    MSB1236C_DTV_Serial_Control(FALSE);
#else
    MSB1236C_DTV_Serial_Control(TRUE);
#endif

    MSB1236C_IIC_Bypass_Mode(TRUE);
    extern MS_BOOL MDrv_Tuner_Init(void);
    if (MDrv_Tuner_Init() == FALSE)
    {
        MSB1236C_IIC_Bypass_Mode(FALSE);
        return ;
    }
    MSB1236C_IIC_Bypass_Mode(FALSE);
}
#endif

