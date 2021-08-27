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
#include <sys/common/MsTypes.h>
#include "sys/common/MsOS.h"
#include <sys/drv/drvGPIO.h>
#include <device/drvDemod.h>
#include <sys/drv/drvIIC.h>
#include <frontend_config.h>
#include <device/drvTuner.h>


//#if(FRONTEND_DEMOD_TYPE ==DEMOD_MSB140X)
#if(FRONTEND_DEMOD_TYPE ==MSTAR_MSB1400_DEMOD)


#include "sys/drv/drvDMD_common.h"
#include "sys/drv/drvDMD_ISDBT.h"      //dan add for integrate ISDBT utopia driver
#include "sys/drv/drvDMD_VD_MBX.h"


#define MSB140x_Delayms  MsOS_DelayTask
#define DEMOD_MSB140X_SLAVE_ID  0xD2
#define ISDBTDMD_MUTEX_TIMEOUT       (2000)

#define DEMOD_DEBUG 1
#if DEMOD_DEBUG
#define PRINTF(p)       printf p
#else
#define PRINTF(p)
#endif

#define TS_PARALLEL_OUTPUT 1

// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_isdbt_ts_clk_divnum
// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#if TS_PARALLEL_OUTPUT
#define ISDBT_TS_SERIAL       0x00
#define ISDBT_TS_CLK_DIVNUM   31
#else
#define ISDBT_TS_SERIAL       0x01
#define ISDBT_TS_CLK_DIVNUM   4
#endif

#define ISDBT_TS_DATA_SWAP  0x00  // TS data inversion
#define ISDBT_TS_CLK_INV      0x01   // TS clock inverseion          //this define will depend on hw main board
//#define ISDBT_IQ_SWAP   0x01          // IQ Swap                    //this define will depend on tuner
#define ISDBT_IFAGC_REF   0x38                                      //this define will depend on tuner
#define ISDBT_IQ_SWAP   0x01          // IQ Swap                    //this define will depend on tuner

#if(FRONTEND_TUNER_TYPE == TUNER_MXL603)    //IF= 6M
#define TUNER_IFFREQ   6000
#elif(FRONTEND_TUNER_TYPE == TUNER_RT820T)   //IF=4.063MHZ
#define TUNER_IFFREQ   4063
#elif(FRONTEND_TUNER_TYPE == TUNER_MXL136)  //IF=4M 
#define TUNER_IFFREQ   4000
#endif

static MS_BOOL bInited = FALSE;

//##########################################################################################################
//       IIC interface
//##########################################################################################################
MS_BOOL   MDrv_MSB140X_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC_WriteBytes(u16BusNumSlaveID, u8addrcount, pu8addr, u16size, pu8data))
    {
        printf("Enter MDrv_MSB140X_IIC_Write error\n");
    }

    return TRUE;
}

MS_BOOL   MDrv_MSB140X_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data)
{
    if (FALSE == MDrv_IIC_ReadBytes(u16BusNumSlaveID, u8AddrNum, paddr, u16size, pu8data))
    {
        printf("Enter MDrv_MSB140X_IIC_Read error\n");
    }

    return TRUE;
}

//##########################################################################################################
//        Public:Common Function Implementation
//##########################################################################################################
/* For FE auto test */
MS_BOOL MDrv_Demod_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet = TRUE;

    bRet = MDrv_DMD_ISDBT_SetReg(u16Addr, u8Data);

    return bRet;
}

/* For FE auto test */
MS_U8 MDrv_Demod_ReadReg(MS_U16 u16Addr)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data;

    bRet = MDrv_DMD_ISDBT_GetReg(u16Addr, &u8Data);

    return u8Data;
}

MS_BOOL MDrv_Demod_Open(void)
{
    MDrv_DMD_PreInit();
    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U32 MApi_FE_GetSNR(void)
{
    MS_U32 u32SNR = 0;

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        u32SNR  = MDrv_DMD_ISDBT_GetSignalQuality();
    }
    if (u32SNR > 100)
    {
        u32SNR = 100;
    }
    else if (u32SNR < 0)
    {
        u32SNR = 0;
    }
    //printf("MDrv_DMD_ISDBT_GetSignalQuality = %ld\n", *pu32SNR);
    return u32SNR;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL bRet = TRUE;

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        bRet = MDrv_DMD_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_A, pfBER);
    }
    else
    {
        *pfBER = 0.0;
    }

    //printf("dan Enter MDrv_Demod_GetBER = %f\n", *pfBER);
    return bRet;
}

MS_U16 MApi_FE_GetSignalQuality(void)
{
    MS_U16  u16SignalStrengthValue;

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
	

    u16SignalStrengthValue = 0;
    if (DMD_ISDBT_LOCK == MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK))
    {
        MDrv_DMD_ISDBT_GetSignalStrength(&u16SignalStrengthValue);
    }

    if (u16SignalStrengthValue > 100)
        u16SignalStrengthValue = 100;
    else if (u16SignalStrengthValue <= 0)
        u16SignalStrengthValue = 0;

    return u16SignalStrengthValue;
}

MS_BOOL  MApi_FE_CheckLock(void)
{
    MS_BOOL bLock = FALSE;
    DMD_ISDBT_LOCK_STATUS eLockStatus;

    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK);
    switch (eLockStatus) // _UTOPIA
    {
    case DMD_ISDBT_LOCK:
        bLock = TRUE;
        break;
    case DMD_ISDBT_CHECKING:
        bLock = FALSE;
        break;
    case DMD_ISDBT_CHECKEND:
        bLock = FALSE;
        break;
    case DMD_ISDBT_UNLOCK:
        bLock = FALSE;
        break;
    default:
        PRINTF(("ISDBTT_GetLockStatus error\n"));
        break;
    }
    return bLock;
}

void MApi_FE_SetFrequency(MS_U32 u32Frequency, MS_U8 eBandWidth)
{
    if (FALSE == bInited)
    {
        PRINTF(("[%s]MSB140X have not inited !!!\n", __FUNCTION__));
        return ;
    }

    printf("FE %d\n", u32Frequency);
    //set freq and bw to tuner
    MDrv_DMD_ISDBT_IIC_BYPASS_MODE(TRUE);
    MDrv_Tuner_SetTuner(u32Frequency, 6);
    MDrv_DMD_ISDBT_IIC_BYPASS_MODE(FALSE);
    MDrv_DMD_ISDBT_SetConfig(TRUE);
}

void MApi_FE_Power_On_Initialization(void)
{
    MS_BOOL bRet = TRUE;
    DMD_ISDBT_InitData sDMD_ISDBT_InitData;
    static MS_U8 u8DMD_ISDBT_InitExt[] = {1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
    printf("\nbRet=0x%x\n",bRet);
    bRet &= MDrv_DMD_PreInit();
	    printf("\nbRet1=0x%x\n",bRet);
    printf("\n check 2 \n");
    // copy tuner config
    //mapi_interface::Get_mapi_pcb()->GetIsdbTuner(0)->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_IQ_SWAP, E_TUNER_DTV_ISDB_MODE, NULL, &u8IqSwap);
    //mapi_interface::Get_mapi_pcb()->GetIsdbTuner(0)->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_DTV_IF_FREQ, E_TUNER_DTV_ISDB_MODE, NULL, &u16DtvIFFreqKHz);

	sDMD_ISDBT_InitData.u16IF_KHZ = TUNER_IFFREQ;
    sDMD_ISDBT_InitData.bIQSwap = ISDBT_IQ_SWAP;
    sDMD_ISDBT_InitData.u16ISDBTFECLockCheckTime = 2000;
    sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime = 500;
    sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt = u8DMD_ISDBT_InitExt;
    sDMD_ISDBT_InitData.u16AgcReferenceValue = ISDBT_IFAGC_REF;

    //TS[begin]
    sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = ISDBT_TS_CLK_DIVNUM;
    sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = ISDBT_TS_CLK_INV;
    sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = ISDBT_TS_DATA_SWAP;
    sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = ISDBT_TS_SERIAL;
    //TS[end]

    //I2C[begin]
    sDMD_ISDBT_InitData.u8I2CSlaveAddr = DEMOD_MSB140X_SLAVE_ID;
    sDMD_ISDBT_InitData.u8I2CSlaveBus = 0;
    sDMD_ISDBT_InitData.bIsExtDemod = TRUE;

    sDMD_ISDBT_InitData.I2C_WriteBytes = MDrv_MSB140X_IIC_Write;
    sDMD_ISDBT_InitData.I2C_ReadBytes = MDrv_MSB140X_IIC_Read;
    //I2C[end]

    bRet &= MDrv_DMD_ISDBT_Initial_Hal_Interface();
	    printf("\nbRet2=0x%x\n",bRet);
    bRet &= MDrv_DMD_ISDBT_Init(&sDMD_ISDBT_InitData, sizeof(sDMD_ISDBT_InitData));
	    printf("\nbRet3=0x%x\n",bRet);
    if (!bRet)
    {
    printf("\ncheck 3\n");
        bInited = FALSE;
        return ;
    }

    MDrv_DMD_ISDBT_IIC_BYPASS_MODE(TRUE);
    extern MS_BOOL MDrv_Tuner_Init(void);
    if (MDrv_Tuner_Init() == FALSE)
    {    printf("\ncheck 4\n");
        MDrv_DMD_ISDBT_IIC_BYPASS_MODE(FALSE);
        bInited = FALSE;
        return ;
    }
    MDrv_DMD_ISDBT_IIC_BYPASS_MODE(FALSE);

    bInited = TRUE;
}


DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
    return DEMOD_INTERFACE_PARALLEL;
}

#endif

