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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define _DRV_SWI2C_C_

#include "sys/common/MsCommon.h"
//#include "HbCommon.h"
#include "sys/drv/drvSWI2C.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAX_SWI2C_BUSNUM    16

#define GPIO_I2CM0_SDA   29//38
#define GPIO_I2CM0_SCL   30//39
#define GPIO_I2CM1_SDA   41
#define GPIO_I2CM1_SCL   40

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static SWI2C_BusCfg _stSwI2cBusCfg[MAX_SWI2C_BUSNUM];
static MS_U8 _u8CurSwI2cCount = 0;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SWI2C_Init(SWI2C_BusCfg stSWI2CCBusCfg,MS_U8* pu8BusNum)
{
    MS_U8 i;

    for(i = 0;i<_u8CurSwI2cCount;i++)
    {
        if((_stSwI2cBusCfg[_u8CurSwI2cCount].padSCL == stSWI2CCBusCfg.padSCL)
            && (_stSwI2cBusCfg[_u8CurSwI2cCount].padSDA== stSWI2CCBusCfg.padSDA))
        {
            *pu8BusNum = i;
            return TRUE;;
        }
    }
    if(_u8CurSwI2cCount >= MAX_SWI2C_BUSNUM)
    {
        return FALSE;
    }
    _stSwI2cBusCfg[_u8CurSwI2cCount] = stSWI2CCBusCfg;
    *pu8BusNum = _u8CurSwI2cCount;
    _u8CurSwI2cCount ++;
    MApi_SWI2C_Init(_stSwI2cBusCfg,_u8CurSwI2cCount);
    return TRUE;
}

MS_BOOL MDrv_HW2SWI2C_Init(HWI2C_PORT ePort,MS_U8* pu8BusNum)
{
    SWI2C_BusCfg stBusCfg;
    
    if (E_HWI2C_PORT_NOSUP == ePort)
    {
        return FALSE;
    }

    if(ePort >= E_HWI2C_PORT_1)
    {
         stBusCfg.defDelay = 440;
         stBusCfg.padSDA= GPIO_I2CM1_SDA;
         stBusCfg.padSCL = GPIO_I2CM1_SCL;
         MDrv_SWI2C_Init(stBusCfg,pu8BusNum);
    }
    else
    {
         stBusCfg.defDelay = 440;
         stBusCfg.padSDA= GPIO_I2CM0_SDA;
         stBusCfg.padSCL = GPIO_I2CM0_SCL;
         MDrv_SWI2C_Init(stBusCfg,pu8BusNum);
    }
    return TRUE;
}

MS_U32 MDrv_SWI2C_Speed_Setting(MS_U8 u8BusNum, MS_U32 u32Speed_K)
{
    return MDrv_SWI2C_Speed_Setting(u8BusNum,u32Speed_K);
}

MS_BOOL MDrv_SWI2C_SetReadMode(SWI2C_ReadMode eReadMode)
{
   return MApi_SWI2C_SetReadMode(eReadMode);
}


MS_BOOL MDrv_SWI2C_WriteByte(MS_U8 u8BusNum,MS_U8 u8SlaveID, MS_U8 u8RegAddr, MS_U8 u8Data)
{
    MS_U16 u16BusNumSlaveId = u8BusNum;

    u16BusNumSlaveId  = u16BusNumSlaveId <<8 | u8SlaveID;
    return MApi_SWI2C_WriteByte(u16BusNumSlaveId,u8RegAddr,u8Data);
}


MS_BOOL MDrv_SWI2C_WriteBytes(MS_U8 u8BusNum,MS_U8 u8SlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    MS_U16 u16BusNumSlaveId = u8BusNum;

    u16BusNumSlaveId  = u16BusNumSlaveId <<8 | u8SlaveID;
    return MApi_SWI2C_WriteBytes(u16BusNumSlaveId,AddrCnt,pu8addr,u16size,pBuf);
}

MS_BOOL MDrv_SWI2C_ReadByte(MS_U8 u8BusNum,MS_U8 u8SlaveID,MS_U8 u8RegAddr, MS_U8 *pu8Data)
{
    MS_U16 u16BusNumSlaveId = u8BusNum;
    
    u16BusNumSlaveId  = u16BusNumSlaveId <<8 | u8SlaveID;
    return MApi_SWI2C_ReadByte(u16BusNumSlaveId,u8RegAddr,pu8Data);
}

MS_BOOL MDrv_SWI2C_ReadBytes(MS_U8 u8BusNum,MS_U8 u8SlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    MS_U16 u16BusNumSlaveId = u8BusNum;
    
    u16BusNumSlaveId  = u16BusNumSlaveId <<8 | u8SlaveID;
    return MApi_SWI2C_ReadBytes(u16BusNumSlaveId,ucSubAdr,paddr,ucBufLen,pBuf);
}

void  MDrv_SWI2C_SetDbgLevel(SWI2C_DbgLvl eLevel)
{
    MApi_SWI2C_SetDbgLevel(eLevel);
}


