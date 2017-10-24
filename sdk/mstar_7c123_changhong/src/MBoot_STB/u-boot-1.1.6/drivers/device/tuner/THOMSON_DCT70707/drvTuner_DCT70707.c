////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (“MStar Confidential Information? by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvTuner_DCT70707.c
/// @brief  THOMSON DCT70707 DVBC Tuner Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <frontend_config.h>
#if (FRONTEND_TUNER_TYPE == TUNER_THOMSON_DCT70707)
#include <device/drvDemod.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvIIC.h>
#include <common.h>


#define DBG_M(fmt, args...)         printf(fmt, ##args)		//FRA_DBG  //{}


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define IIC_WRITE(u8SlaveId,  pu8Addr, u8AddrSize,  pu8Buf,  u32BufSize)   MDrv_IIC_WriteBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)
#define IIC_READ(u8SlaveId,   pu8Addr, u8AddrSize,  pu8Buf,  u32BufSize)   MDrv_IIC_ReadBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define DCT70707_ADDR                 0xC2 //0xC2
#define DCT70707_FREF                 0x03                                // [2:0] 0x00=166.667kHz, 0x01=142.857kHz, 0x02=80kHz, 0x03=62.5kHz, 0x04=31.25kHz, 0x05=50kHz
#define DCT70707_AGC                  0x08                                // [5:3] 0x00=112dBuV, 0x08=109dBuV, 0x10=106dBuV, 0x18=103dBuV, 0x20=100dBuV, 0x28=97dBuV, 0x30=94dBuV, 0x38=Internal RF-AGC disable
#define DCT70707_IF                   36000                               // kHz
#define DCT70707_FN(rf)               (MS_U16)( ((MS_U32)(rf + DCT70707_IF) * (MS_U32)(1000/62.5)) / 1000)
// #define DCT70707_FN(rf)             (MS_U16)( (MS_U32)(rf + DCT70707_IF) * (MS_U32) 6 / 1000) // 1000/6 = 166.667KHz

#define DCT70707_STATUS_POR           0x80                                // POR=1(0x80) on power-on
#define DCT70707_STATUS_POR_MASK      0x80
#define DCT70707_STATUS_FL            0x40                                // FL=1(0x40) PLL is phase locked
#define DCT70707_STATUS_FL_MASK       0x40

#define DCT70707_LOCK_TIME           1000

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
// DVBC device internal parameter table
MS_U8 dmdConfig[] =
{
    // SZ 5015 board - DCT70707 internal RFAGC
    // Rev.: [0x3f] 0x82, [0x20] 0x72, [0x29] 0x00, [0x2a] 0x55, [0x30] 0x07
    0x02, 0xa0, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
    0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0x00, 0x55, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
    0x07, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
    0x52, 0x52, 0x55,
};


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

/*
static MS_U8 DCT70707_Init(MS_U8 *u8Data)
{

    // Power on reset
    u8Data[0] = 0x00;
	u8Data[1] = 0x00;
    u8Data[2] = 0x80;
    u8Data[3] = 0x00;
    u8Data[4] = 0xC2;
    return 5;
}
*/


static MS_U8 DCT70707_SetFreq(MS_U32 u32Frf, MS_U8 *u8Data)
{
    MS_U16              u16Temp;

    // DB1 and DB2 (N14 to N0)
    u16Temp = DCT70707_FN(u32Frf);
    u8Data[0] = (MS_U8)(u16Temp >> 8) & 0x7F;
    u8Data[1] = (MS_U8)(u16Temp & 0xFF);

    // CB1 (ATP and RS)
    u8Data[2] = 0x80 | DCT70707_AGC | DCT70707_FREF;

    // BB (BS)
    u8Data[3] = 0;
    if ((u32Frf + DCT70707_IF) < 185000)          // 85M - 185M; u32Frf + 36MHz
    {
        u8Data[3] |= 0x01;
    }
    else if ((u32Frf + DCT70707_IF) < 465000)     // 185M - 465M
    {
        u8Data[3] |= 0x06;
    }
    else                                          // 465M - 896M
    {
        u8Data[3] |= 0x0C;
    }

    // CB2 (Const C3)
    u8Data[4] = 0xC3;

    return 5;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq)
{

    MS_U8                  u8Len, u8Data[5];
	MS_U8				   u8Acc;
	printf("\n===============%s===%s============\n",__FILE__,__func__);
    u8Len = DCT70707_SetFreq(u32Freq, u8Data);
    if (IIC_WRITE(DCT70707_ADDR, NULL, 0, u8Data, u8Len) == FALSE)
    {
        DBG_M("%s(at %s): tuner write fail\n", __FUNCTION__, __FILE__);
        return FALSE;
    }


#if defined(MS_OPTIMIZE)
    // demod manual tune: 195ms avg.
    // demod auto tune: 360ms avg.
    MsOS_DelayTask(15);
    return TRUE;
#else
    // for DCT70707 to lock phase
    for (u8Acc = 0; u8Acc < 10; u8Acc++)	//20*10ms time-out
    {
        MsOS_DelayTask(20);    //ref.: acquisitiion time (tuning + AGC) < 70ms
        if (IIC_READ(DCT70707_ADDR, NULL, 0, u8Data, 1) == FALSE)
        {
            DBG_M("%s: tuner read fail\n", __FUNCTION__);
            return FALSE;
        }
        if ((u8Data[0] & DCT70707_STATUS_FL_MASK) == DCT70707_STATUS_FL)
        {
            return TRUE;
        }
    }
    DBG_M("%s: tuner FL fail\n", __FUNCTION__);
    return FALSE;
#endif

}


MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    //[TBD]
    return TRUE;
}


MS_BOOL MDrv_Tuner_Init(void)
{
    //[TBD]
    return TRUE;
}

#endif // TUNER_THOMSON_DCT70707




