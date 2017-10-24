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


#ifndef DEVTUNER_DIBCOM_DIB8096_C
#define DEVTUNER_DIBCOM_DIB8096_C

#include <device/COFDM_Demodulator.h>

#define DBG_DIB8096(format, args...) //printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)

void devDigitalTuner_Init()
{
    DBG_DIB8096("\n");
}

void devTunerInit(void)
{
    DBG_DIB8096("\n");
}


void devTunerSetFreqStep(FREQSTEP eFreqStep)
{
    DBG_DIB8096("\n");
}

void devTunerSetPLLData(WORD wPLLData, RFBAND eBand)
{
    DBG_DIB8096("\n");
}


void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    DBG_DIB8096("\n");
}

#endif
