////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

#ifndef _TITANIA_I2C_H_
#define _TITANIA_I2C_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define BIT0	                    0x00000001
#define BIT1	                    0x00000002
#define BIT2	                    0x00000004
#define BIT3	                    0x00000008
#define BIT4	                    0x00000010
#define BIT5	                    0x00000020
#define BIT6	                    0x00000040
#define BIT7	                    0x00000080
#define BIT8	                    0x00000100
#define BIT9	                    0x00000200
#define BIT10	                    0x00000400
#define BIT11	                    0x00000800
#define BIT12	                    0x00001000
#define BIT13	                    0x00002000
#define BIT14	                    0x00004000
#define BIT15  	                    0x00008000

// definition for TRUE
#define TRUE            1
// definition for FALSE
#define FALSE           0

#define MHal_IIC_DELAY()        { udelay(1000); udelay(1000); udelay(1000); udelay(1000); udelay(1000); }//delay 5ms

#define REG_IIC_BASE              0xBF806840    // 0xBF800000 + (0x1A10*4)

#define REG_IIC_CTRL               0x00
#define REG_IIC_CLK_SEL            0x01
#define REG_IIC_WDATA              0x02
#define REG_IIC_RDATA              0x03
#define REG_IIC_STATUS             0x04         // reset, clear and status

#define MHal_IIC_REG(addr)      (*(volatile U32*)(REG_IIC_BASE + ((addr)<<2)))

#define REG_CHIP_BASE              0xBF803C00
#define REG_IIC_MODE               0x50
#define REG_DDCR_GPIO_SEL          0x70
#define MHal_CHIP_REG(addr)     (*(volatile U32*)(REG_CHIP_BASE + ((addr)<<2)))
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------



#endif // _TITANIA_I2C_H_
