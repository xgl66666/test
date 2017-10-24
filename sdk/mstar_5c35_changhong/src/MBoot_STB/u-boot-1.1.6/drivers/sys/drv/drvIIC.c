////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define _IIC_C_

#include <common.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvIIC.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvSWI2C.h>
#include "sys/api/apiSWI2C.h"
#include <configs/uboot_module_config.h>

#if(MS_BOARD_TYPE_SEL == BD_MST098B_D01A) || (MS_BOARD_TYPE_SEL == BD_MST098B_D01B) || (MS_BOARD_TYPE_SEL == BD_MST128A_D01A)
#define USE_PORT    E_HWI2C_PORT1_5
#elif(MS_BOARD_TYPE_SEL == BD_MST098C_D01A_S)
#define USE_PORT    E_HWI2C_PORT1_5
#elif(MS_BOARD_TYPE_SEL == BD_MST098D_D01A_S)
#define USE_PORT    E_HWI2C_PORT1_1
#elif((MS_BOARD_TYPE_SEL == BD_MST098E_D01A) \
    || (MS_BOARD_TYPE_SEL == BD_MST098E_D01B) \
    || (MS_BOARD_TYPE_SEL == BD_MST124A_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST124SZ) \
    || (MS_BOARD_TYPE_SEL == BD_MST124D_D02A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST124B_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST124E_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST124D_D01A) \
    || (MS_BOARD_TYPE_SEL == BD_SS2000_DT0A) \
    || (MS_BOARD_TYPE_SEL == BD_MST138B_D01A) \
    || (MS_BOARD_TYPE_SEL == BD_MST138B_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST138B_D01D) \
    || (MS_BOARD_TYPE_SEL == BD_MST178A_D01A) \
    || (MS_BOARD_TYPE_SEL == BD_MST178A_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST124G_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST124G_D01A) \
    || (MS_BOARD_TYPE_SEL == BD_MST124E_D01A_SZ) \
    || (MS_BOARD_TYPE_SEL == BD_MST124C_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_MST173A_D01A)\
    || (MS_BOARD_TYPE_SEL == BD_MST204A_D01A)\
    || (MS_BOARD_TYPE_SEL == BD_MST173B_D01A)\
    )

#define USE_PORT    E_HWI2C_PORT0_1
#elif(MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A)
#define USE_PORT    E_HWI2C_PORT0_2
#elif(MS_BOARD_TYPE_SEL == BD_MST138A_D01A_S)
#define USE_PORT    E_HWI2C_PORT1_5

#else
#define USE_PORT    E_HWI2C_PORT1_5
#endif

#if ENABLE_SWI2C
#ifndef HWIIC0_SWMODE_ENABLE
#define HWIIC0_SWMODE_ENABLE 1
#endif
#else
#ifndef HWIIC0_SWMODE_ENABLE
#define HWIIC0_SWMODE_ENABLE 0
#endif
#endif

#ifndef HWIIC1_SWMODE_ENABLE
#define HWIIC1_SWMODE_ENABLE 0
#endif

#if HWIIC0_SWMODE_ENABLE
static MS_U8 _u8SwI2c0BusNum;
#endif
#if HWIIC1_SWMODE_ENABLE
static MS_U8 _u8SwI2c1BusNum;
#endif

void MDrv_IIC_InitPort0(HWI2C_PORT ePort)
{
    MS_BOOL bResult;
    HWI2C_UnitCfg hwI2CCfg;
    memset(&hwI2CCfg,0,sizeof(HWI2C_UnitCfg));
    //MDrv_HWI2C_SetDbgLevel(E_HWI2C_DBGLV_ALL);
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = ePort;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort0: OK\n");
    }
    else
    {
        printf("MDrv_IIC_InitPort0: FAIL\n");
        return;
    }

    MDrv_HWI2C_SelectPort(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClk(hwI2CCfg.eSpeed);

}

void MDrv_IIC_InitPort1(HWI2C_PORT ePort)
{
    MS_BOOL bResult;
    HWI2C_UnitCfg hwI2CCfg;
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = ePort;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort1: OK\n");
    }
    else
    {
        printf("MDrv_IIC_InitPort1: FAIL\n");
        return;
    }

    MDrv_HWI2C_SelectPort1(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClkP1(hwI2CCfg.eSpeed);

}

void MDrv_IIC_Init(void)
{

    if(USE_PORT >= E_HWI2C_PORT_1)
    {
         
    #if HWIIC1_SWMODE_ENABLE
         MDrv_HW2SWI2C_Init(USE_PORT,&_u8SwI2c1BusNum);
    #else
         MDrv_IIC_InitPort1(USE_PORT);
    #endif
    }
    else
    {
    #if HWIIC0_SWMODE_ENABLE

         MDrv_HW2SWI2C_Init(USE_PORT,&_u8SwI2c0BusNum);
    #else
         MDrv_IIC_InitPort0(USE_PORT);
    #endif
    }

}


MS_BOOL MDrv_IIC_ReadBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    return MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
}


MS_BOOL MDrv_IIC_ReadBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    return MDrv_HWI2C_ReadBytesP1(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
}

MS_BOOL MDrv_IIC_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    if(USE_PORT >= E_HWI2C_PORT_1)
    {
    #if HWIIC1_SWMODE_ENABLE
        return MDrv_SWI2C_ReadBytes(_u8SwI2c1BusNum,u16BusNumSlaveID,ucSubAdr, paddr,ucBufLen, pBuf);
    #else
        return MDrv_IIC_ReadBytesPort1(u16BusNumSlaveID,ucSubAdr, paddr,ucBufLen, pBuf);
    #endif
    }
    else
    {
    #if HWIIC0_SWMODE_ENABLE
        return MDrv_SWI2C_ReadBytes(_u8SwI2c0BusNum,u16BusNumSlaveID,ucSubAdr, paddr,ucBufLen, pBuf);
    #else
        return MDrv_IIC_ReadBytesPort0(u16BusNumSlaveID,ucSubAdr, paddr,ucBufLen, pBuf);
    #endif
    }
}

MS_BOOL MDrv_IIC_WriteBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    return MDrv_HWI2C_WriteBytes(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
}

MS_BOOL MDrv_IIC_WriteBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    return MDrv_HWI2C_WriteBytesP1(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
}

MS_BOOL MDrv_IIC_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    if(USE_PORT >= E_HWI2C_PORT_1)
    {
#if HWIIC1_SWMODE_ENABLE
            return MDrv_SWI2C_WriteBytes(_u8SwI2c1BusNum,u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
#else
            return MDrv_IIC_WriteBytesPort1(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
#endif
    }
    else
    {
#if HWIIC0_SWMODE_ENABLE
            return MDrv_SWI2C_WriteBytes(_u8SwI2c0BusNum,u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
#else
            return MDrv_IIC_WriteBytesPort0(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
#endif
    }
}

MS_BOOL MDrv_IIC_SelectCLK(HWI2C_CLKSEL eSpeed)
{

#if HWIIC1_SWMODE_ENABLE || HWIIC0_SWMODE_ENABLE
        MS_U32 u32Speed;
        switch(eSpeed)
        {
             case E_HWI2C_HIGH:
                 u32Speed = SWI2C_SPEED_MAPPING_250K;
                 break;
             case E_HWI2C_NORMAL:
                 u32Speed = SWI2C_SPEED_MAPPING_150K;
                 break;
             case E_HWI2C_SLOW:
             default:
                 u32Speed = SWI2C_SPEED_MAPPING_100K;
                 break;
        }
#endif


    if(USE_PORT >= E_HWI2C_PORT_1)
    {
#if HWIIC1_SWMODE_ENABLE
             return MDrv_SWI2C_Speed_Setting(_u8SwI2c1BusNum,u32Speed);
#else
             return MDrv_HWI2C_SetClkP1(eSpeed);
#endif

    }
    else
    {
#if HWIIC0_SWMODE_ENABLE
            return MDrv_SWI2C_Speed_Setting(_u8SwI2c0BusNum,eSpeed);
#else
            return MDrv_HWI2C_SetClk(eSpeed);
#endif
    }
}

void IIC_Stop(void)
{
    printf("IIC_N");
}

MS_BOOL IIC_Start(void)
{
    printf("IIC_N");
    return 0;
}

MS_BOOL IIC_SendByte(MS_U8 u8dat)
{
    printf("IIC_N");
    return 0;
}

#undef _IIC_C_
