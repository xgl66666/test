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

#ifndef _IIC_H_
#define _IIC_H_


#include "drvHWI2C.h"


void MDrv_IIC_InitPort0(HWI2C_PORT ePort);
void MDrv_IIC_InitPort1(HWI2C_PORT ePort);
void MDrv_IIC_Init(void);

MS_BOOL MDrv_IIC_WriteBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf);
MS_BOOL MDrv_IIC_WriteBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf);
MS_BOOL MDrv_IIC_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_IIC_ReadBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf);
MS_BOOL MDrv_IIC_ReadBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf);
MS_BOOL MDrv_IIC_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_IIC_ReadByte(MS_U16 u16BusNumSlaveID, MS_U8 u8RegAddr, MS_U8 *pu8Data);
MS_BOOL MDrv_IIC_WriteByte(MS_U16 u16BusNumSlaveID, MS_U8 u8RegAddr, MS_U8 u8Data);
MS_BOOL MDrv_IIC_SelectCLK(HWI2C_CLKSEL eSpeed);


#endif // _IIC_H_
