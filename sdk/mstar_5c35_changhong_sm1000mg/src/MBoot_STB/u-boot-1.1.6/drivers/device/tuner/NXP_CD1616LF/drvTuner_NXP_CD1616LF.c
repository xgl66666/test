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


#ifndef DEVTUNER_NXP_CD1616LF_C
#define DEVTUNER_NXP_CD1616LF_C

#include <frontend_config.h>
#if (FRONTEND_TUNER_TYPE == NXP_CD1616LF_TUNER)
#include "sys/common/MsOS.h"
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvIIC.h>

extern void MsOS_DelayTask (MS_U32 u32Ms);
#define DBG_CD1616LF(format, args...) printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)

#define CD1616_ADDR                 0xC2
#define CD1616_FREF                 0x06                                // [2:1] 0=50.0kHz, 1=32.5kHz, 3=62.5kHz
#define CD1616_AGC                  0x10                                // [4:3] 0x00: for external RFAGC, 0x10: 108dBuV internal RFAGC
#define CD1616_IF                   36130                               // KHz
#define CD1616_STATUS_FL            0x78                                // FL=1(0x40) PLL is phase locked
#define CD1616_STATUS_FL_MASK       0x78


#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)


BOOLEAN devCOFDM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
	U16 U16SlaveID=u8SlaveID;

    UNUSED(U16SlaveID);
    IIC_Bypass_Mode(1);

    MDrv_IIC_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data);

    IIC_Bypass_Mode(0);

    return TRUE;
}

BOOLEAN devCOFDM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
	U16 U16SlaveID=u8SlaveID;

    UNUSED(U16SlaveID);

    IIC_Bypass_Mode(1);
    msAPI_Timer_Delayms(5); // delay 5ms

    if ( FALSE == MDrv_IIC_WriteBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("%04d %s devCOFDM_PassThroughI2C_WriteBytes Error \n", __LINE__, __FUNCTION__);
        IIC_Bypass_Mode(0);
        msAPI_Timer_Delayms(5); // delay 5ms
        return FALSE;
    }

    msAPI_Timer_Delayms(5); // delay 5ms
    IIC_Bypass_Mode(0);
    msAPI_Timer_Delayms(5); // delay 5ms

    return TRUE;

}

void devDigitalTuner_Init()
{
    DBG_CD1616LF("\n");
}

void devTunerInit(void)
{
    DBG_CD1616LF("\n");
}


void devTunerSetFreqStep(FREQSTEP eFreqStep)
{
    DBG_CD1616LF("\n");
}

void devTunerSetPLLData(WORD wPLLData, RFBAND eBand)
{
    DBG_CD1616LF("\n");
}


void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    DBG_CD1616LF("\n");
    #define CD1616_FN(rf)               (MS_U16)( (((MS_U32)(rf + CD1616_IF)) * (MS_U32)(1000/62.5)) / 1000)
    MS_BOOL dwError = FALSE;
    MS_U8 config[4];

    MS_U16 divider = CD1616_FN((double)Freq);
    MS_U8 u8Control;

    u8Control = CD1616_AGC;

    if (Freq <= 167.000)
    {
        u8Control |= 0x01;
    }
    else if (Freq <= 454.000)
    {
        u8Control |= 0x02;
    }
    else if (Freq <= 860.000)
    {
        u8Control |= 0x04;
    }

    // DB1 and DB2 (N14 to N0)
    config[0] = (divider >> 8) & 0xff;
    config[1] = divider & 0xff;

    // CB (RSA and RSB)
    config[2] = 0xC0 | CD1616_FREF;

    // BB (AGC, SP)
    config[3] = u8Control;

    DBG_CD1616LF("divider = %u %x u8Data = %02x %02x %02x %02x\n", divider, divider, config[0], config[1], config[2], config[3] );

    dwError = devCOFDM_PassThroughI2C_WriteBytes(_TUNER_I2C_ID, 0, NULL, 4, config);

    MsOS_DelayTask(100);

    if (devCOFDM_PassThroughI2C_ReadBytes(_TUNER_I2C_ID, NULL, 0, 1, config) == FALSE)
    {
        return;
    }
    if ((config[0] & CD1616_STATUS_FL_MASK) == CD1616_STATUS_FL)
    {
        return;
    }
    return;

}
#else


#define CD1616_FN(rf)               (MS_U16)( (((MS_U32)(rf + CD1616_IF)) * (MS_U32)(1000/62.5)) / 1000)

static MS_U8 CD1616_SetFreq(MS_U32 u32Frf, MS_U8 *u8Data)
{
    MS_U16              u16Temp;

    // DB1 and DB2 (N14 to N0)
    u16Temp = CD1616_FN(u32Frf);
    u8Data[0] = (MS_U8)(u16Temp >> 8);
    u8Data[1] = (MS_U8)(u16Temp & 0xFF);

    // CB (RSA and RSB)
    u8Data[2] = 0xC0 | CD1616_FREF;

    // BB (AGC, SP)
    u8Data[3] = CD1616_AGC;
    if ((u32Frf + CD1616_FREF) < 167000) // 49M - 167M
    {
        u8Data[3] |= 0x01;
    }
    else if ((u32Frf + CD1616_FREF) < 454000) // 167M - 454M
    {
        u8Data[3] |= 0x02;
    }
    else // 454M - 860M
    {
        u8Data[3] |= 0x04;
    }

    return 4;
}

static MS_BOOL IIC_WRITE(MS_U8 u8SlaveID, MS_U8* paddr,MS_U8 u8AddrNum, MS_U8* pu8data,  MS_U16 u16size)
{

    if ( FALSE == MDrv_IIC_WriteBytes((MS_U16)u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }

    MsOS_DelayTask(1); // delay 1ms

    return TRUE;


}


static MS_BOOL IIC_READ(MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{

    if ( FALSE == MDrv_IIC_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data))
    {
        return FALSE;
    }

    return TRUE;

}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    DBG_CD1616LF("\n");
}
void devDigitalTuner_Init(void)
{
    DBG_CD1616LF("\n");
}

MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq)
{
    MS_U8                  u8Len, u8Data[4];

    u8Len = CD1616_SetFreq(u32Freq, u8Data);
    if (IIC_WRITE(CD1616_ADDR, NULL, 0, u8Data, u8Len) == FALSE)
    {
        return FALSE;
    }
#if defined(MS_OPTIMIZE)
    // demod manual tune: 195ms avg.
    // demod auto tune: 360ms avg.
    MsOS_DelayTask(15);
    return TRUE;
#else
    // for CD1616 to lock phase
    MsOS_DelayTask(100);
    if (IIC_READ(CD1616_ADDR, NULL, 0, u8Data, 1) == FALSE)
    {
        return FALSE;
    }
    if ((u8Data[0] & CD1616_STATUS_FL_MASK) == CD1616_STATUS_FL)
    {
        return TRUE;
    }
    return FALSE;
#endif

}
#endif
#endif
#endif
