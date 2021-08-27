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
#include <common.h>
#include <sys/common/MsTypes.h>
#include "sys/common/MsOS.h"
#include <sys/drv/drvGPIO.h>
#include <device/drvDemod.h>
#include <sys/drv/drvIIC.h>
#include <frontend_config.h>
#include <device/drvTuner.h>

//#include <math.h>
//#include "MsCommon.h"
//#include "drvIIC.h"
//#include "MsOS.h"
//#include "Board.h"
//#include "drvDemod.h"
#include "device_demodulator_msb131x.h"
//#include "drvGPIO.h"
#if (FRONTEND_TUNER_TYPE == TUNER_AV2012 || FRONTEND_TUNER_TYPE == TUNER_AV2011)
#include "../../tuner/AV2012/drvTuner_AV2012.h"

#elif (FRONTEND_TUNER_TYPE == TUNER_SHARP_BS2S7VZ7801)
#include "drvTuner_Sharp7801.h"
#endif
#define GPIO_FE_RST                     16

MS_U8 MSB131X_LIB[]=
{
#include "msb131x_dvbs.dat"
};

//#if (FRONTEND_DEMOD_TYPE == DEMOD_MSB131X)

#define MSB131X_TS_DATA_SWAP            (0)

#if MSB131X_TS_DATA_SWAP
static MS_BOOL                  _bTSDataSwap=FALSE;
#endif

#define MS131X_DEBUG 0

#if  MS131X_DEBUG
#define PRINTE(p) printf p
#else
#define PRINTE(p)
#endif

#define MDrv_msb131x_IIC_Write(a,b,c,d,e)    MDrv_IIC_WriteBytesPort0(a,c,b,e,d)
#define MDrv_msb131x_IIC_Read(a,b,c,d,e)     MDrv_IIC_ReadBytesPort0(a,c,b,e,d)
#define DEMOD_MSB131X_SLAVE_ID      (0xF2)
#define MSB131X_MUTEX_TIMEOUT       (2000)
#define LOAD_CODE_I2C_BLOCK_NUM          (0x80)
#define SRAM_BASE                           (0x8000)
#define SAMPLING_RATE                  (96000)
#define MSB131X_DEMOD_WAIT_TIMEOUT    (6000)
#define MSB131X_TUNER_WAIT_TIMEOUT    (50)

static  MS_S32                       _s32TaskId     = -1;
static  MS_S32                       _s32MutexId;
static  MS_S32                       _s32FunMutexId;
static  MS_BOOL                  _bInited     = FALSE;
static  MS_BOOL                  _bDemodType=FALSE;
static  MS_U16                       _u16BlindScanStartFreq =0;
static  MS_U16                       _u16BlindScanEndFreq   =0;
static  MS_U16                       _u16TunerCenterFreq    =0;
static  MS_U16                      _u16ChannelInfoIndex    =0;
//Debug Only+
static  MS_U16                      _u16NextCenterFreq=0;
static  MS_U16                      _u16LockedSymbolRate=0;
static  MS_U16                      _u16LockedCenterFreq=0;
static  MS_U16                      _u16PreLockedHB=0;
static  MS_U16                      _u16PreLockedLB=0;
static  MS_U16                      _u16CurrentSymbolRate=0;
static  MS_S16                      _s16CurrentCFO=0;
static  MS_U16                      _u16CurrentStepSize=0;
static MS_U8                        _u8ToneBurstFlag=0;

//Debug Only-
static  MS_U16                      _u16ChannelInfoArray[2][1000];
static MS_U16                       _u16SignalLevel[47][2]=
{
    {8285,    922},{10224,    902},{12538,    882},{14890,    862},{17343,    842},{19767,    822},{22178,    802},{24618,    782},{27006,    762},{29106,    742},
    {30853,    722},{32289,    702},{33577,    682},{34625,    662},{35632,    642},{36552,    622},{37467,    602},{38520,    582},{39643,    562},{40972,    542},
    {42351,    522},{43659,    502},{44812,    482},{45811,    462},{46703,    442},{47501,    422},{48331,    402},{49116,    382},{49894,    362},{50684,    342},
    {51543,    322},{52442,    302},{53407,    282},{54314,    262},{55208,    242},{56000,    222},{56789,    202},{57544,    182},{58253,    162},{58959,    142},
    {59657,    122},{60404,    102},{61181,    82},{62008,    62},{63032,    42},{65483,    22},{65535,    12}
};

static MS_U8  _u8Search_Range[6] = {/*0x09, 0x10,*/0x04, 0x00, 0x0E, 0x08, 0x58, 0x16};
static MS_U8  _u8AGC_Invert[1] = {0x00};	//set AGC Invert 0x01

MS_U16 u16_INTERN_DVBS_Version;

static float Log10Approx(float flt_x)
{
        MS_U32 u32_temp = 1;
        MS_U8 indx = 0;
        do
        {
            u32_temp = u32_temp << 1;
            if (flt_x < (float)u32_temp)
                break;
        }
        while (++indx < 32);
        return (float)0.3 *indx;
}

//MS_BOOL MDrv_Demod_GetFWVer(MS_U16 *ver)
MS_BOOL MDrv_DMD_DVBS_GetFWVer(MS_U16 *ver)
{
  MS_BOOL bRet=TRUE;
  
	*ver=u16_INTERN_DVBS_Version;
	return bRet;
}
MS_BOOL MSB131X_GetFWVer(MS_U16 *ver)
{
	  MS_BOOL bRet=TRUE;
    MS_U8 u8Data;
    MS_U16 u16Address;
    MS_U16 u16Data;
    
    u16Address=0x0B69;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x0B68;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    
    u16_INTERN_DVBS_Version=u16Data;
    *ver=u16_INTERN_DVBS_Version;
    return bRet;
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

MS_BOOL MDrv_Demod_Init(void)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;
    MS_U16 u16Address;
    MS_U16 u16Data;
    MS_U16 u16dat_size;

    _bInited = FALSE;
    if (_s32TaskId >= 0)
    {
        PRINTE(("MSB131X TaskID 0x%x\n",_s32TaskId));
        return TRUE;
    }
    if (_bInited==TRUE)
    {
        PRINTE(("MSB131X Already Init 0x%x\n",_bInited));
        return TRUE;
    }
     _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_MSB131X",MSOS_PROCESS_SHARED);
     _s32FunMutexId=MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_MSB131X_Function",MSOS_PROCESS_SHARED);
      if ((_s32MutexId < 0) || (_s32FunMutexId <0))
    {
        PRINTE(("creat  MSB131X  Mutex error!!! _s32MutexId:%d _s32FunMutexId:%d\n", _s32MutexId, _s32FunMutexId));
        if (_s32MutexId >=0)
            MsOS_DeleteMutex(_s32MutexId);
        if (_s32FunMutexId >=0)
            MsOS_DeleteMutex(_s32FunMutexId);
        return FALSE;
    }
	//extern void MApi_Demod_HWReset(void);
    MApi_Demod_HWReset();
     PRINTE(("MSB131X Init\n"));
    bRet&=MSB131X_I2C_Channel_Set(0);
    if (bRet==FALSE)
    {
        PRINTE(("MSB131X i2c channel set fail!!!\n"));
    }
    else
    {
        PRINTE(("MSB131X i2c channel set success!!!\n"));
    }
    //HW version Check
    u16Address=0x0900;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if (bRet==FALSE)
    {
        PRINTE(("MSB131X i2c read reg  fail!!!\n"));
    }
    else
    {
        PRINTE(("MSB131X HW version: 0x%x=0x%x\n", u16Address, u8Data));
    }

    u16dat_size=sizeof(MSB131X_LIB);
    bRet&=MSB131X_LoadDSPCode(MSB131X_LIB, u16dat_size);
    if (bRet==FALSE)
    {
        PRINTE(("MSB131X Load FW  fail!!!\n"));
    }
    else
    {
        PRINTE(("MSB131X Load FW  success!!!\n"));
    }
    MsOS_DelayTask(10);
    MSB131X_IIC_Bypass_Mode(TRUE);
    extern MS_BOOL MDrv_Tuner_Initial(void);
    bRet&=MDrv_Tuner_Initial();
    MSB131X_IIC_Bypass_Mode(FALSE);
    bRet&=MSB131X_DiSEqC_Init();
    //FW version Check
    bRet&=MSB131X_GetFWVer(&u16Data);
    //bRet&=MDrv_Demod_GetFWVer(&u16Data);
    bRet&=MDrv_DMD_DVBS_GetFWVer(&u16Data);
    printf("MSB131X  FW  Version:%d.%d\n", ((u16Data>>8)&0x00FF),(u16Data&0x00FF));//
    //PRINTE(("MSB131X  FW  Version:%d.%d\n", ((u16Data>>8)&0x00FF),(u16Data&0x00FF)));
    if (bRet==FALSE)
    {
        MsOS_DeleteMutex(_s32MutexId);
        MsOS_DeleteMutex(_s32FunMutexId);
    }
    _bInited = TRUE;
    return bRet;
}
MS_BOOL MSB131X_IIC_Bypass_Mode(MS_BOOL enable)
{
    MS_U8 u8Retry=10;

    while(u8Retry--)
    {
        if (enable)
            MSB131X_WriteReg(0x0900+(0x08)*2, 0x10);// IIC by-pass mode on
        else
            MSB131X_WriteReg(0x0900+(0x08)*2, 0x00);// IIC by-pass mode off
    }
    return TRUE;
}
MS_BOOL  MSB131X_I2C_Channel_Set(MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    if (MsOS_ObtainMutex(_s32MutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
     {
        PRINTE(("%s mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    //Exit
    Data[0] = 0x34;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0]=(ch_num & 0x01)? 0x36 : 0x45;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    //Init
    Data[0] = 0x53;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 5);
    Data[0]=(ch_num & 0x04)? 0x80 : 0x81;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    if ((ch_num==4)||(ch_num==5)||(ch_num==1))
        Data[0]=0x82;
    else
        Data[0] = 0x83;
     bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

    if ((ch_num==4)||(ch_num==5))
        Data[0]=0x85;
    else
        Data[0] = 0x84;
     bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0]=(ch_num & 0x01)? 0x51 : 0x53;
     bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0]=(ch_num & 0x01)? 0x37 : 0x7F;
     bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0] = 0x35;
     bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     Data[0] = 0x71;
     bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
     MsOS_ReleaseMutex(_s32MutexId);
     return bRet;
}
MS_BOOL  MSB131X_I2C_Channel_Change(MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    if (MsOS_ObtainMutex(_s32MutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    Data[0] = (ch_num & 0x01)? 0x81 : 0x80;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0] = (ch_num & 0x02)? 0x83 : 0x82;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0] = (ch_num & 0x04)? 0x85 : 0x84;
    bRet&=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}
MS_BOOL  MSB131X_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
   MS_BOOL bRet=TRUE;
    MS_U8 Data[5];

    if (MsOS_ObtainMutex(_s32MutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    Data[0] = 0x10;
    Data[1] = 0x00;
    Data[2] = 0x00;
    Data[3] = (u16Addr >> 8) &0xff;
    Data[4] = u16Addr &0xff;

    Data[0] = 0x35;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0] = 0x10;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 5);
    bRet &=MDrv_msb131x_IIC_Read(DEMOD_MSB131X_SLAVE_ID, 0,0, pu8Data, 1);
    Data[0] = 0x34;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}
MS_BOOL MSB131X_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 Data[6];
    if (MsOS_ObtainMutex(_s32MutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    Data[0] = 0x10;
    Data[1] = 0x00;
    Data[2] = 0x00;
    Data[3] = (u16Addr >> 8) & 0xff;
    Data[4] = u16Addr & 0xff;
    Data[5] = u8Data;

    Data[0] = 0x35;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);

    Data[0] = 0x10;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 6);

    Data[0] = 0x34;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}
MS_BOOL MSB131X_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;
    bRet &= MSB131X_WriteReg(u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= MSB131X_WriteReg(u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);
    return bRet;
}
MS_BOOL MSB131X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet=TRUE;
    MS_U16 index;
    MS_U8 Data[LOAD_CODE_I2C_BLOCK_NUM+5];
    if (MsOS_ObtainMutex(_s32MutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    Data[0] = 0x10;
    Data[1] = 0x00;
    Data[2] = 0x00;
    Data[3] = (u16Addr >> 8) &0xff;
    Data[4] = u16Addr &0xff;

    for(index = 0; index < data_size ; index++)
    {
    Data[5+index] = u8pData[index];
    }
    Data[0] = 0x35;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    Data[0] = 0x10;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 5 + data_size);
    Data[0] = 0x34;
    bRet &=MDrv_msb131x_IIC_Write(DEMOD_MSB131X_SLAVE_ID, 0, 0, Data, 1);
    MsOS_ReleaseMutex(_s32MutexId);
    return bRet;
}
MS_BOOL MSB131X_LoadDSPCode(MS_U8  *pu8Buf, MS_U16 u16Len)
{
    MS_BOOL bRet=TRUE;
    MS_U16 index;
    MS_U16 SRAM_Address;

    bRet&=MSB131X_I2C_Channel_Change(3);
    bRet&=MSB131X_WriteReg(0x0B32, 0x01);
    bRet&=MSB131X_I2C_Channel_Change(0);
    bRet&=MSB131X_WriteReg(0x3008, 0x00);
    bRet&=MSB131X_WriteReg(0x3009, 0x00);
    bRet&=MSB131X_WriteReg(0x300A, 0x00);
    bRet&=MSB131X_WriteReg(0x300B, 0x00);
    bRet&=MSB131X_I2C_Channel_Change(3);
    bRet&=MSB131X_WriteReg(0x10E1, 0x20);
    bRet&=MSB131X_WriteReg(0x10E0, 0x3F);
    bRet&=MSB131X_WriteReg(0x10E6, 0x08);
    bRet&=MSB131X_I2C_Channel_Change(0);
    for (index = 0; index < u16Len;)
    {
            SRAM_Address = SRAM_BASE + index;
            if (index+LOAD_CODE_I2C_BLOCK_NUM-1<u16Len)
            {
                bRet&= MSB131X_WriteRegs(SRAM_Address,pu8Buf+index,LOAD_CODE_I2C_BLOCK_NUM);
                index=index+LOAD_CODE_I2C_BLOCK_NUM-1;
            }
            else
            {
                bRet&=MSB131X_WriteRegs(SRAM_Address,pu8Buf+index,u16Len-index);
                index=u16Len;
            }
    }
    
    SRAM_Address = SRAM_BASE + 0x7E00;
    bRet&= MSB131X_WriteRegs(SRAM_Address, _u8Search_Range, sizeof(_u8Search_Range)); //test
    SRAM_Address += sizeof(_u8Search_Range);
    bRet&= MSB131X_WriteRegs(SRAM_Address, _u8AGC_Invert, sizeof(_u8AGC_Invert));
    
    bRet&=MSB131X_I2C_Channel_Change(3);
    bRet&=MSB131X_WriteReg(0x10E6, 0x00);
    bRet&=MSB131X_WriteReg(0x1004, 0x00);
    bRet&=MSB131X_WriteReg(0x1005, 0x00);
    bRet&=MSB131X_WriteReg(0x1000, 0x00);
    bRet&=MSB131X_WriteReg(0x1006, 0x7F);
    bRet&=MSB131X_WriteReg(0x1007, 0xFF);
    bRet&=MSB131X_WriteReg(0x1002, 0x00);
    bRet&=MSB131X_WriteReg(0x1018, 0x01);
    bRet&=MSB131X_WriteReg(0x0B32, 0x00);
    return bRet;
}
MS_BOOL  MSB131X_SetFrequency(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{
     MS_BOOL bRet=TRUE;
     MS_U16 u16LockCount;

#if (FRONTEND_TUNER_TYPE == TUNER_AV2012 || FRONTEND_TUNER_TYPE == TUNER_AV2011)
    bRet&=MDrv_Tuner_SetFreq(u16CenterFreq, u32SymbolRate_Hz);
#elif (FRONTEND_TUNER_TYPE==TUNER_SHARP_BS2S7VZ7801)
    bRet&=MDrv_Tuner_SetFreq(u16CenterFreq, u32SymbolRate_Hz/1000);
#endif
    if (bRet==FALSE)
        return bRet;

    u16LockCount=0;
    do
    {
        bRet=TRUE;
        extern MS_BOOL MDrv_Tuner_CheckLock(void);
        bRet&=MDrv_Tuner_CheckLock();
        MsOS_DelayTask(1);
        u16LockCount++;
    }while((bRet==FALSE) && (u16LockCount<MSB131X_TUNER_WAIT_TIMEOUT)) ;

    if (bRet==TRUE)
    {
        PRINTE(("Tuner PLL Lock\n"));
    }
    else
    {
        PRINTE(("Tuner PLL Unlock\n"));
    }
     return bRet;
}
MS_BOOL MSB131X_DiSEqC_Init(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    u16Address=0x0BCD; //Clear Status
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&(~0x20))|(0x20);
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0BC0;
    u8Data=0x00;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0BC0;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&(~0x02))|(0x02);
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    return bRet;
}
MS_U16    MSB131X_DTV_GetSignalStrength(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U16 u16Data;
    MS_U8  u8Data;
    MS_U8  u8Index;

    u16Address=0x3E26;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&0xF0)|0x03;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x3E04;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data|=0x80;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x3E29;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x3E28;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    u16Address=0x3E04;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=~(0x80);
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    if (bRet==FALSE)
    {
        PRINTE(("MSB131X_DTV_GetSignalStrength fail!!! \n"));
        return 0;
    }
    for (u8Index=0; u8Index < 47; u8Index++)
    {
        if ((65535 - u16Data) <= _u16SignalLevel[u8Index][0])
        {
            return _u16SignalLevel[u8Index][1];
        }
    }
    return 0;
}
MS_U32    MSB131X_DTV_GetSignalNoiseRatio(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U32 u32Data;
    MS_U8  u8Data;
    float  fvariance;
    float  log_snr;

    if (MSB131X_DTV_GetLock()==FALSE)
            return 0;

    u16Address=0x2408;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data|=0x10;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x24A5;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32Data=(u8Data&0x3F);
    u16Address=0x24A4;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;
    u16Address=0x24A3;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;
    u16Address=0x24A2;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;
    u16Address=0x2408;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=~(0x10);
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    if (bRet==FALSE)
    {
        PRINTE(("MSB131X_DTV_GetSignalNoiseRatio fail!!! \n"));
        return 0;
    }
    fvariance=(float)u32Data/0x20000000;
    if (fvariance==0)
        fvariance=1;
    log_snr=10*Log10Approx(1/fvariance);

    return (MS_U32)log_snr;
}
MS_U16    MSB131X_DTV_GetSignalQuality(MS_BOOL bDemodType)
{
        MS_BOOL bRet=TRUE;
        MS_U16 u16Address;
        MS_U8  u8Data;
        MS_U16 u16BitErrPeriod;
        MS_U32 u32BitErr;
        float fber;
        float log_ber;

        if (MSB131X_DTV_GetLock()==FALSE)
            return 0;

        if (bDemodType==FALSE)//S
        {
        u16Address=0x112C;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        if (bRet==FALSE)
        {
            PRINTE(("MSB131X_DTV_GetSignalQuality fail!!! \n"));
                                return 0;
        }
        if ((u8Data&0x02)!=0x02)
        {
            return 0;
        }
        u16Address=0x1132;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data|=0x80;
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
        u16Address=0x1131;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1130;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
        u16Address=0x113D;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x113C;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x113B;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x113A;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1132;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=~(0x80);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
        if (u16BitErrPeriod == 0)
            u16BitErrPeriod = 1;
        if (u32BitErr <= 0)
            fber = 0.5 / ((float)u16BitErrPeriod *128 * 188 * 8);
        else
            fber = (float)u32BitErr / ((float)u16BitErrPeriod *128 * 188 * 8);
        if (bRet==FALSE)
        {
            PRINTE(("MSB131X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }
        log_ber = ( - 1) *Log10Approx(1 / fber);
        if (log_ber <= ( - 7.0))
        {
            return 100;
        }
        else if (log_ber < ( - 3.7))
                      {
                            return (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
                      }
                       else if (log_ber < ( - 2.7))
                       {
                        return (10+((( - 2.7) - log_ber) / (( - 2.7) - ( - 3.7))*(60-10)));
                      }
                      else
                      {
                            return 10;
                      }
                      return 0;

    }
    else    //S2
    {
        u16Address=0x2604;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data|=0x01;
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
        u16Address=0x2625;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x2624;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
        u16Address=0x265B;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x265A;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x2659;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x2658;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x2604;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=~(0x01);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
        if (u16BitErrPeriod == 0)
            u16BitErrPeriod = 1;
        fber = (float)u32BitErr/(u16BitErrPeriod*64800);
        if (bRet==FALSE)
        {
            PRINTE(("MSB131X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
            return 0;
        }
        log_ber = ( - 1) *Log10Approx(1 / fber);
        if (log_ber <= ( - 7.0))
        {
            return 100;
         }
         else if (log_ber < ( - 3.7))
         {
            return (80+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-80)));
         }
         else if (log_ber < ( - 1.7))
         {
            return (40+((( - 1.7) - log_ber) / (( - 1.7) - ( - 3.7))*(80-40)));
         }
         else if (log_ber < ( - 0.7))
         {
            return (10+((( - 0.7) - log_ber) / (( - 0.7) - ( - 1.7))*(40-10)));
         }
         else
          {
            return 5;
          }
          return 0;
    }
}
MS_BOOL    MSB131X_DTV_GetLock(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u16Address=0x13FE;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        if((u8Data>=14) && (u8Data!=0xcd))
        {
            if (u8Data==14)
            {
                _bDemodType=FALSE;   //S
            }
            else
            {
                _bDemodType=TRUE;    //S2
            }
            bRet = TRUE;
        }
        else
        {
            bRet = FALSE;
        }

      #if MSB131X_TS_DATA_SWAP
       if (bRet==FALSE)
       {
           _bTSDataSwap=FALSE;
       }
       else
       {
           if (_bTSDataSwap==FALSE)
           {
               _bTSDataSwap=TRUE;
               u16Address=0x2A80;
               bRet&=MSB131X_ReadReg(u16Address, &u8Data);
               u8Data^=0x20;
               bRet&=MSB131X_WriteReg(u16Address, u8Data);
           }
       }
      #endif
    }
    else
    {
        bRet = TRUE;
    }
    return bRet;
}
MS_BOOL MDrv_Demod_SetTsSerial(MS_BOOL bSerial)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_TPSGetLock(MS_BOOL *pbLock)
{
    MS_BOOL bRet=TRUE;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pbLock=MSB131X_DTV_GetLock();
    PRINTE(("MSB131X MDrv_Demod_TPSGetLock  0x%x\n", *pbLock));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_MPEGGetLock(MS_BOOL *pbLock)
{
    MS_BOOL bRet=TRUE;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pbLock=MSB131X_DTV_GetLock();
    PRINTE(("MSB131X MDrv_Demod_MPEGGetLock 0x%x\n", *pbLock));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetRollOff(MS_U8 *pRollOff)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x241E;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x03)==0x00)
        *pRollOff = 0;  //Rolloff 0.35
    else if (((u8Data&0x03)==0x01) || ((u8Data&0x03)==0x03))
        *pRollOff = 1;  //Rolloff 0.25
    else
        *pRollOff = 2;  //Rolloff 0.20
    PRINTE(("MDrv_Demod_GetRollOff:%d\n", *pRollOff));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetRFOffset(MS_S16 *ps16RFOff)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U32 u32FreqOffset;
    float FreqOffset;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u32FreqOffset=0;
    u16Address=0x135E;
    u8Data=0x08;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x3E05;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data|=0x80;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);

    u16Address=0x1368;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32FreqOffset=u8Data;
    u16Address=0x1367;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32FreqOffset=(u32FreqOffset<<8)|u8Data;
    u16Address=0x1366;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u32FreqOffset=(u32FreqOffset<<8)|u8Data;
    u16Address=0x3E05;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=~(0x80);
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    FreqOffset=(float)u32FreqOffset;
    if (FreqOffset>=2048)
    {
        FreqOffset=FreqOffset-4096;
    }
    FreqOffset=(FreqOffset/4096)*SAMPLING_RATE;
    *ps16RFOff=FreqOffset/1000;
    PRINTE(("MSB131X FreqOffset:%f[MHz]\n", FreqOffset/1000));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_Start+\n"));
    _u16BlindScanStartFreq=u16StartFreq;
    _u16BlindScanEndFreq=u16EndFreq;
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0992;
    bRet&=MSB131X_WriteReg2bytes(u16Address, _u16BlindScanStartFreq);
    PRINTE(("MDrv_Demod_BlindScan_Start- _u16BlindScanStartFreq%d u16StartFreq %d u16EndFreq %d\n", _u16BlindScanStartFreq, u16StartFreq, u16EndFreq));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U32  u32TunerCutOffFreq;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_NextFreq+\n"));
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x02)
    {
            u8Data|=0x08;
            bRet&=MSB131X_WriteReg(u16Address, u8Data);
            u16WaitCount=0;
            do
            {
                u16Address=0x13FE;
                bRet&=MSB131X_ReadReg(u16Address, &u8Data);
                u16WaitCount++;
                PRINTE(("MDrv_Demod_BlindScan_NextFreq u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
                MsOS_DelayTask(1);
            }while((u8Data!=0x01)&&(u16WaitCount<MSB131X_DEMOD_WAIT_TIMEOUT));
     }
    u16WaitCount=0;

    * bBlindScanEnd=FALSE;
    _u16TunerCenterFreq=0;
    u16Address=0x0993;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    _u16TunerCenterFreq=u8Data;
    u16Address=0x0992;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    _u16TunerCenterFreq=(_u16TunerCenterFreq<<8)|u8Data;

    if (_u16TunerCenterFreq >=_u16BlindScanEndFreq)
    {
        PRINTE(("MDrv_Demod_BlindScan_NextFreq . _u16TunerCenterFreq %d _u16BlindScanEndFreq%d\n", _u16TunerCenterFreq, _u16BlindScanEndFreq));
        * bBlindScanEnd=TRUE;
        MsOS_ReleaseMutex(_s32FunMutexId);
        return bRet;
    }
#if (FRONTEND_TUNER_TYPE == TUNER_AV2011)
    u32TunerCutOffFreq=44000;
#else
    u32TunerCutOffFreq=34000;
#endif
    MSB131X_IIC_Bypass_Mode(TRUE);
    bRet&=MSB131X_SetFrequency(_u16TunerCenterFreq,u32TunerCutOffFreq);
    MSB131X_IIC_Bypass_Mode(FALSE);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)
    {
        u8Data&=~(0x08);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
        u8Data|=0x02;
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
        u8Data|=0x01;
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
    }
    else
    {
        u8Data&=~(0x08);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
    }
    PRINTE(("MDrv_Demod_BlindScan_NextFreq _u16TunerCenterFreq:%d-\n", _u16TunerCenterFreq));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U32  u32Data;
    MS_U16 u16Data;
    MS_U8   u8Data;
    MS_U16  u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished+\n"));
    u16WaitCount=0;
    *u8FindNum=0;
    *u8Progress=0;
    do
    {
        u16Address=0x13FE;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u16WaitCount++;
        PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished+1 u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
        MsOS_DelayTask(1);
    }while((u8Data!=16)&&(u16WaitCount<MSB131X_DEMOD_WAIT_TIMEOUT));
    u16Address=0x0B8B;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x0B8A;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished Last state:0x%x substate:0x%x\n", (u16Data>>8)&0x00FF, (u16Data)&0x00FF));
    u16Address=0x0B8F;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x0B8E;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished BlindState:0x%x\n", u16Data));
    u16Address=0x0B8F;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=u8Data;
    u16Address=0x0B8E;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u16Data=(u16Data<<8)|u8Data;
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished OuterCheckStatus:0x%x\n", u16Data));
    if (u16WaitCount>=MSB131X_DEMOD_WAIT_TIMEOUT)
    {
        bRet=FALSE;
    }
    else
    {
        u16Address=0x13FF;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        if (u8Data==0)
        {
            u16Address=0x0B86;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u32Data=u8Data;
            u16Address=0x0B85;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            u16Address=0x0B84;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000); //Center Freq
            _u16LockedCenterFreq=((u32Data+500)/1000);
            u16Address=0x0B89;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B88;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(u16Data);//Symbol Rate
            _u16LockedSymbolRate=u16Data;
            _u16ChannelInfoIndex++;
            *u8FindNum=_u16ChannelInfoIndex;

            u16Address=0x0B8B;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B8A;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            if (u16Data*1000 >= 0x8000)
            {
                u16Data=0x10000- u16Data*1000;
                _s16CurrentCFO=-1*u16Data/1000;
            }
            else
            {
                _s16CurrentCFO=u16Data;
            }

            u16Address=0x0B8D;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B8C;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;
            u16Address=0x0B91;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B90;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;
            u16Address=0x0B93;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B92;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedLB=u16Data;
            PRINTE(("Current Locked CF:%d BW:%d BWH:%d BWL:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB, _s16CurrentCFO, _u16CurrentStepSize));
        }
        else if (u8Data==1)
        {
            u16Address=0x0993;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0992;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16NextCenterFreq=u16Data;
            u16Address=0x0B85;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B84;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;
            u16Address=0x0B87;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B86;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedLB=u16Data;
            u16Address=0x0B89;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B88;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentSymbolRate=u16Data;
            u16Address=0x0B8B;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B8A;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            if (u16Data*1000 >= 0x8000)
            {
                u16Data=0x1000- u16Data*1000;
                _s16CurrentCFO=-1*u16Data/1000;
            }
            else
            {
                _s16CurrentCFO=u16Data;
            }
            u16Address=0x0B8D;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=u8Data;
            u16Address=0x0B8C;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;
            PRINTE(("Pre Locked CF:%d BW:%d HBW:%d LBW:%d Current CF:%d BW:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB,  _u16NextCenterFreq-_u16CurrentStepSize, _u16CurrentSymbolRate, _s16CurrentCFO, _u16CurrentStepSize));
        }
    }
    *u8Progress=100;
    PRINTE(("MDrv_Demod_BlindScan_WaitCurFreqFinished u8Progress%d u8FindNum %d-\n", *u8Progress, *u8FindNum));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_Cancel(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_Cancel+\n"));
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0992;
    u16Data=0x0000;
    bRet&=MSB131X_WriteReg2bytes(u16Address, u16Data);
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    PRINTE(("MDrv_Demod_BlindScan_Cancel-\n"));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_End(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    PRINTE(("MDrv_Demod_BlindScan_End+\n"));
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0992;
    u16Data=0x0000;
    bRet&=MSB131X_WriteReg2bytes(u16Address, u16Data);
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;
    PRINTE(("MDrv_Demod_BlindScan_End-\n"));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_BOOL bRet=TRUE;
    MS_U16  u16TableIndex;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *u16TPNum=_u16ChannelInfoIndex-u16ReadStart;
    for(u16TableIndex = 0; u16TableIndex < (*u16TPNum); u16TableIndex++)
    {
        pTable[u16TableIndex].u32Frequency = _u16ChannelInfoArray[0][_u16ChannelInfoIndex-1];
        pTable[u16TableIndex].SatParam.u32SymbolRate = _u16ChannelInfoArray[1][_u16ChannelInfoIndex-1];
       PRINTE(("MDrv_Demod_BlindScan_GetChannel Freq:%d SymbolRate:%d\n", pTable[u16TableIndex].u32Frequency, pTable[u16TableIndex].SatParam.u32SymbolRate));
    }
    PRINTE(("MS131X u16TPNum:%d\n", *u16TPNum));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *u32CurrentFeq=_u16TunerCenterFreq;
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U8 u8ReSet22k;
   
   u16Address=0x0BC4;
    u8Data=0x01;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    
    u16Address=0x0BC0;
    u8Data=0x4E;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0BCC;
    u8Data=0x80;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    
    u16Address=0x0BC2;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8ReSet22k=u8Data;
    	
    if (bTone1==TRUE)
    {
       u8Data=0x19;
       bRet&=MSB131X_WriteReg(u16Address, u8Data);
        _u8ToneBurstFlag=1;
    }
    else
    {
       u8Data=0x11;
       bRet&=MSB131X_WriteReg(u16Address, u8Data);
       _u8ToneBurstFlag=2;
    }
    u16Address=0x0BCD;
    u8Data=u8Data&~(0x01);
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x02;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data&~(0x02);
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(1);
    u8Data=u8Data|0x01;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    
    u16Address=0x0BC2;
    u8Data=u8ReSet22k;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    
    MsOS_DelayTask(100);
    return bRet;
}
/*
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
         MsOS_DelayTask(1);
         u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB131X_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB131X_DEMOD_WAIT_TIMEOUT)
    {
        PRINTE(("MSB131X DiSEqC Send Tone Busrt Busy!!!\n"));
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }

    u16Address=0x0BC4;
    u8Data=0x01;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    u16Address=0x0B54;
    if (bTone1==TRUE)
    {
        u8Data=(0x40|0xA0);
    }
    else
    {
        u8Data=(0x40|0x20);
    }
    bRet&=MSB131X_WriteReg(u16Address, u8Data);

    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
*/
MS_BOOL MDrv_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x0BC2;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if (bLow==TRUE)
    {
        u8Data=(u8Data|0x40);
    }
    else
    {
        u8Data=(u8Data&(~0x40));
    }
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    u16Address=0x0BC2;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if( (u8Data&0x40)==0x40)
    {
        * bLNBOutLow=TRUE;
    }
    else
    {
        * bLNBOutLow=FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x0BC2;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if (b22kOn==TRUE)
    {
        u8Data=(u8Data|0x08);
    }
    else
    {
        u8Data=(u8Data&(~0x08));
    }
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Address=0x0BC2;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x08)==0x08)
    {
        *b22kOn=TRUE;
    }
    else
    {
        *b22kOn=FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        MsOS_DelayTask(1);
        u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB131X_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB131X_DEMOD_WAIT_TIMEOUT)
    {
        PRINTE(("MSB131X DiSEqC Send Command Busy!!!\n"));
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }

    //u16Address=0x0BC4;
    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u16Address=0x0BC4+u8Index;
        u8Data=*(pCmd+u8Index);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
    }
    u16Address=0x0B54;
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    if (_u8ToneBurstFlag==1)
    {
        u8Data|=0x80;//0x20;
    }
    else if (_u8ToneBurstFlag==2)
    {
        u8Data|=0x20;//0x80;
    }   
    _u8ToneBurstFlag=0; 
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);

    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
/*
MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16 u16WaitCount;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
         MsOS_DelayTask(1);
         u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB131X_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB131X_DEMOD_WAIT_TIMEOUT)
    {
        PRINTE(("MSB131X DiSEqC Send Command Busy!!!\n"));
        MsOS_ReleaseMutex(_s32FunMutexId);
        return FALSE;
    }

    //u16Address=0x0BC4;
    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u16Address=0x0BC4+u8Index;
        u8Data=*(pCmd+u8Index);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);
    }
    u16Address=0x0B54;
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB131X_WriteReg(u16Address, u8Data);

    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
*/
MS_BOOL MDrv_Demod_Open(void)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Close(void)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Reset(void)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);	//PRINTE(("MDrv_Demod_Reset\n"));
    return bRet;
}
MS_BOOL MDrv_Demod_TsOut(MS_BOOL bEnable)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
	mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_SetBW(MS_U32 u32BW)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetBW(MS_U32 *pu32BW)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    MS_BOOL bRet=TRUE;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pbLock=MSB131X_DTV_GetLock();
    PRINTE(("MSB131X MDrv_Demod_GetLock 0x%x\n", *pbLock));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *pu32SNR=MSB131X_DTV_GetSignalNoiseRatio();
    PRINTE(("MSB131X SNR %d\n", *pu32SNR));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Data;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    u16Data=MSB131X_DTV_GetSignalQuality(_bDemodType);
    *pfBER=(float)u16Data;
    PRINTE(("MSB131X BER %f\n", *pfBER));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    *ps32Signal=(MS_S32)MSB131X_DTV_GetSignalStrength();
    *ps32Signal=(-1)*(*ps32Signal)/10;
    PRINTE(("MSB131X Signal Level %d\n", *ps32Signal));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U16 u16SymbolRate;
    MS_U16 u16CenterFreq;

    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }

    u16SymbolRate=(pParam->SatParam.u32SymbolRate/1000);
    u16CenterFreq=pParam->u32Frequency;
    PRINTE(("MSB131X MDrv_Demod_Restart+ Fc:%d SymbolRate %d\n", u16CenterFreq, u16SymbolRate));
    MSB131X_IIC_Bypass_Mode(TRUE);
    bRet&=MSB131X_SetFrequency(pParam->u32Frequency,(pParam->SatParam.u32SymbolRate/1000));
    MSB131X_IIC_Bypass_Mode(FALSE);
    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    bRet&=MSB131X_WriteReg(u16Address,u8Data);
    MsOS_DelayTask(50);
    u16Address=0x0B9E;
    u8Data=(u16SymbolRate&0xFF);
    bRet&=MSB131X_WriteReg(u16Address,u8Data);
    u16Address=0x0B9F;
    u8Data=((u16SymbolRate>>8)&0xFF);
    bRet&=MSB131X_WriteReg(u16Address,u8Data);

    u16Address=0x0990;
    bRet&=MSB131X_ReadReg(u16Address, &u8Data);
    u8Data&=0xF0;
    u8Data|=0x01;
    bRet&=MSB131X_WriteReg(u16Address,u8Data);
    #if MSB131X_TS_DATA_SWAP
    _bTSDataSwap=FALSE;
    #endif
    PRINTE(("MSB131X MDrv_Demod_Restart-\n"));
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}
MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    MS_BOOL bRet=TRUE;
    if (MsOS_ObtainMutex(_s32FunMutexId, MSB131X_MUTEX_TIMEOUT)==FALSE)
    {
        PRINTE(("%s function mutex timeout\n", __FUNCTION__));
        return FALSE;
    }
    MsOS_ReleaseMutex(_s32FunMutexId);
    return bRet;
}

MS_BOOL MSB131X_GPIO_SetLevel(MS_U8 u8Pin, MS_BOOL bLevel)
{
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_BOOL bRet = FALSE;

    if (u8Pin==13)
    {
        u16Address=0x0955;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x20);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //set RF to GPIO
        u16Address=0x09CE;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x10);//u8Data|=0x10;
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //GPIO output enable
        if (bLevel==FALSE)
        {
            u16Address=0x09CC;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u8Data|=0x10;
            bRet&=MSB131X_WriteReg(u16Address, u8Data); //Set level to high
        }
        else
        {
            u16Address=0x09CC;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u8Data&=(~0x10);
            bRet&=MSB131X_WriteReg(u16Address, u8Data); //Set level to high
        }
    }
    else if (u8Pin==11)
    {
        u16Address=0x095C;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //set TS_ERR to GPIO
        u16Address=0x09C3;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x08);//u8Data|=0x08;
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //GPIO output enable
        if (bLevel==FALSE)
        {
            u16Address=0x09C3;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u8Data|=0x08;
            bRet&=MSB131X_WriteReg(u16Address, u8Data); //Set level to high
        }
        else
        {
            u16Address=0x09C3;
            bRet&=MSB131X_ReadReg(u16Address, &u8Data);
            u8Data&=(~0x08);
            bRet&=MSB131X_WriteReg(u16Address, u8Data); //Set level to high
        }
    }
    else
    {
        return FALSE;
    }
    return bRet;
}
MS_BOOL MSB131X_GPIO_GetLevel(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_BOOL bRet =FALSE;
    if (u8Pin==13)
    {
        u16Address=0x0955;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x20);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //set RF to GPIO

        u16Address=0x09CE;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x10);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //GPIO output disable

        u16Address=0x09D0;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        if ((u8Data&0x10)==0x10)
        {
            *bLevel=TRUE;
        }
        else
        {
            *bLevel=FALSE;
        }
    }
    else if (u8Pin==11)
    {
        u16Address=0x095C;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //set TS_ERR to GPIO

        u16Address=0x09C1;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x08);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //GPIO output disable

        u16Address=0x09C5;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        if ((u8Data&0x08)==0x08)
        {
            *bLevel=TRUE;
        }
        else
        {
            *bLevel=FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    return bRet;
}

MS_BOOL  MSB131X_GPIO_Disable(MS_U8 u8Pin)
{
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_BOOL bRet =FALSE;
    if (u8Pin==13)
    {
        u16Address=0x0955;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x20);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //set RF to GPIO

        u16Address=0x09CE;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x10);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //GPIO output disable

    }
    else if (u8Pin==11)
    {
        u16Address=0x095C;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //set TS_ERR to GPIO

        u16Address=0x09C1;
        bRet&=MSB131X_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x08);
        bRet&=MSB131X_WriteReg(u16Address, u8Data);    //GPIO output disable
    }
    else
    {
        return FALSE;
    }
    return bRet;
}

//#endif

