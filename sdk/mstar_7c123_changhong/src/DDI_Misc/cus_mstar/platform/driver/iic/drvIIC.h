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

#ifndef _IIC_H_
#define _IIC_H_

#include "MsCommon.h"
#include "drvHWI2C.h"

#ifndef SWI2C_INUSE
#define SWI2C_INUSE 1
#endif

#if SWI2C_INUSE
#include "apiSWI2C.h"
#endif
  // Speed Mapping: Base on the Driver loading, maybe you must to use the oscilloscope to measure this Value
  // 250K => SwIicDly =1
  // 200K => SwIicDly =10
  // 150K => SwIicDly =20
  // 100K => SwIicDly =40

#define I2C_SPEED_MAPPING_250K          1
#define I2C_SPEED_MAPPING_200K          10
#define I2C_SPEED_MAPPING_150K          20
#define I2C_SPEED_MAPPING_100K          40

typedef enum {
    E_I2C_BUS_DDCROM,
    E_I2C_BUS_SYS = E_I2C_BUS_DDCROM,
    E_I2C_BUS_DDCA0,
    E_I2C_BUS_DDCD0,
    E_I2C_BUS_DDCD1,
    E_I2C_BUS_DDCD2,
    E_I2C_BUS_USR0,
    E_I2C_BUS_USR1,
    E_I2C_BUS_USR2,
    E_I2C_BUS_USR3,
} E_I2C_BUS_CHANNEL;

typedef enum
{
    E_MS_IIC_PORT_0 = 0, /// port 0_0 //disable port 0
    E_MS_IIC_PORT0_1,    /// port 0_1
    E_MS_IIC_PORT0_2,    /// port 0_2
    E_MS_IIC_PORT0_3,    /// port 0_3
    E_MS_IIC_PORT0_4,    /// port 0_4
    E_MS_IIC_PORT0_5,    /// port 0_5
    E_MS_IIC_PORT0_6,    /// port 0_6
    E_MS_IIC_PORT0_7,    /// port 0_7

    E_MS_IIC_PORT_1 = 8, /// port 1_0 //disable port 1
    E_MS_IIC_PORT1_1,    /// port 1_1
    E_MS_IIC_PORT1_2,    /// port 1_2
    E_MS_IIC_PORT1_3,    /// port 1_3
    E_MS_IIC_PORT1_4,    /// port 1_4
    E_MS_IIC_PORT1_5,    /// port 1_5
    E_MS_IIC_PORT1_6,    /// port 1_6
    E_MS_IIC_PORT1_7,    /// port 1_7

    E_MS_IIC_PORT_2 = 16,/// port 2_0 //disable port 2
    E_MS_IIC_PORT2_1,    /// port 2_1
    E_MS_IIC_PORT2_2,    /// port 2_2
    E_MS_IIC_PORT2_3,    /// port 2_3
    E_MS_IIC_PORT2_4,    /// port 2_4
    E_MS_IIC_PORT2_5,    /// port 2_5
    E_MS_IIC_PORT2_6,    /// port 2_6
    E_MS_IIC_PORT2_7,    /// port 2_7

    E_MS_IIC_PORT_3 = 24,/// port 3_0 //disable port 3
    E_MS_IIC_PORT3_1,    /// port 3_1
    E_MS_IIC_PORT3_2,    /// port 3_2
    E_MS_IIC_PORT3_3,    /// port 3_3
    E_MS_IIC_PORT3_4,    /// port 3_4
    E_MS_IIC_PORT3_5,    /// port 3_5
    E_MS_IIC_PORT3_6,    /// port 3_6
    E_MS_IIC_PORT3_7,    /// port 3_7

    E_MS_IIC_PORT_4 = 32,/// port 4_0 //disable port 4
    E_MS_IIC_PORT4_1,    /// port 4_1
    E_MS_IIC_PORT4_2,    /// port 4_2
    E_MS_IIC_PORT4_3,    /// port 4_3
    E_MS_IIC_PORT4_4,    /// port 4_4
    E_MS_IIC_PORT4_5,    /// port 4_5
    E_MS_IIC_PORT4_6,    /// port 4_6
    E_MS_IIC_PORT4_7,    /// port 4_7

    E_MS_IIC_PORT_5 = 40,/// port 5_0 //disable port 5
    E_MS_IIC_PORT5_1,    /// port 5_1
    E_MS_IIC_PORT5_2,    /// port 5_2
    E_MS_IIC_PORT5_3,    /// port 5_3
    E_MS_IIC_PORT5_4,    /// port 5_4
    E_MS_IIC_PORT5_5,    /// port 5_5
    E_MS_IIC_PORT5_6,    /// port 5_6
    E_MS_IIC_PORT5_7,    /// port 5_7

    E_MS_IIC_SW_PORT_0 = 48,   /// swiic port0
    E_MS_IIC_SW_PORT_1,        /// swiic port1
    E_MS_IIC_SW_PORT_2,        /// swiic port2
    E_MS_IIC_SW_PORT_3,        /// swiic port3
    E_MS_IIC_SW_PORT_4,        /// swiic port4
    E_MS_IIC_SW_PORT_5,        /// swiic port5
    E_MS_IIC_SW_PORT_6,        /// swiic port6
    E_MS_IIC_SW_PORT_7,        /// swiic port7
    E_MS_IIC_SW_PORT_MAX = E_MS_IIC_SW_PORT_7,

    E_MS_IIC_PORT_NOSUP  /// non-support port

} MS_IIC_PORT;

typedef struct
{
    MS_IIC_PORT ePort;
    MS_U16 u16SclPad;
    MS_U16 u16SdaPad;
    MS_U16 u16Delay;
}MS_IIC_SwBus;

typedef enum {
    E_MS_IIC_READ_MODE_DIRECT,                 ///< first transmit slave address + reg address and then start receive the data */
    E_MS_IIC_READ_MODE_DIRECTION_CHANGE,       ///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device
    E_MS_IIC_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device
    E_MS_IIC_READ_MODE_MAX
} MS_IIC_ReadMode;

void MDrv_IIC_InitPort0(MS_IIC_PORT ePort);
void MDrv_IIC_InitPort1(MS_IIC_PORT ePort);
void MDrv_IIC_Init(MS_IIC_PORT ePort);

#if SWI2C_INUSE
MS_BOOL MDrv_IIC_SetupSwPort(MS_U8 u8BusCount, MS_IIC_SwBus* pstConfig);
#endif
MS_BOOL MDrv_IIC_WriteBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf);
MS_BOOL MDrv_IIC_WriteBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf);
MS_BOOL MDrv_IIC_WriteBytes(MS_IIC_PORT ePort,MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_IIC_ReadBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf);
MS_BOOL MDrv_IIC_ReadBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf);
MS_BOOL MDrv_IIC_ReadBytes(MS_IIC_PORT ePort,MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_IIC_SelectCLK(MS_IIC_PORT ePort,HWI2C_CLKSEL eSpeed);
MS_BOOL MDrv_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size);
MS_BOOL MDrv_IIC1_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size);
MS_BOOL MDrv_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size);
MS_BOOL MDrv_IIC1_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size);
MS_BOOL MDrv_IIC_ReadWithMode(MS_IIC_PORT ePort, MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf, MS_IIC_ReadMode eMode);


MS_U8 IIC_GetByte (MS_U16  bAck);
MS_BOOL IIC_SendByte(MS_U8 u8dat);
MS_BOOL IIC_AccessStart(MS_U8 ucSlaveAdr, MS_U8 trans_t);
void IIC_Stop(void);
MS_BOOL IIC_Start(void);

MS_IIC_PORT getI2CPort(MS_U8 drv_frontend_index);
MS_IIC_PORT getDishI2CPort(MS_U8 drv_frontend_index);

#endif // _IIC_H_
