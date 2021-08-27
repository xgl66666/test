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
#include <common.h>
#ifndef DRVUSBMAIN2__H
#define DRVUSBMAIN2__H

#define HW_BASE         0xbf200000

//=================== 1.Condition Definition  ============================================================
//========================================================================================================

	#define mbFUSBH200_HALFSPEEDEnable_Set()		  (mwHost20Bit_Set2(0x40,BIT2))
	#define mbFUSBH200_Interrupt_OutPut_High_Set()	  (mwHost20Bit_Set2(0x40,BIT3))
       #define mbFUSBH200_Interrupt_OutPut_Low_Set()     (mwHost20Bit_Clr2(0x40,BIT3))
	#define mbFUSBH200_VBUS_OFF_Set()		          (mwHost20Bit_Set2(0x40,BIT4))
	#define mbFUSBH200_VBUS_ON_Set()   		          (mwHost20Bit_Clr2(0x40,BIT4))
	#define mbFUSBH200_VBUS_VLD_Rd()   		          (mwHost20Port2(0x40)&BIT8)

#define USBWrite_REG32(reg_adr, data)      ( (*( (volatile MS_U32 *) (reg_adr) ) ) = ((MS_U32)data) )
#define USBRead_REG32(reg_adr)             ( *( (volatile MS_U32 *) (reg_adr) ) )
#define USBWrite_REG16(reg_adr, data)      ( (*( (volatile MS_U16 *) (reg_adr) ) ) = ((MS_U16)data) )
#define USBRead_REG16(reg_adr)             ( *( (volatile MS_U16 *) (reg_adr) ) )
#define USBWrite_REG8(reg_adr, data)      ( (*( (volatile UINT8 *) (reg_adr) ) ) = ((UINT8)data) )
#define USBRead_REG8(reg_adr)             ( *( (volatile UINT8 *) (reg_adr) ) )
#define  XBYTE   ((unsigned char volatile   *) 0)

//#define UTMIBaseAddr    0xbf807400
//#define UTMI_RDREG8(reg_adr)             ( *( (volatile U8 *) (0xbf806280+reg_adr*2) ) )
//#define UTMI_WRREG8(reg_adr, data)      ( (*( (volatile UINT8 *) (0xbf807400+reg_adr*4) ) ) = ((UINT8)data) )
//#define UTMI_WRREG16(reg_adr, data)      ( (*( (volatile UINT16 *) (0xbf807400+reg_adr*4) ) ) = ((UINT16)data) )

#if defined(CONFIG_KRONUS) 
    #define UTMI1_BASE    0x5400
#else   
    #define UTMI1_BASE    0x7400
#endif

#define UTMI_WRREG8(reg_adr, data)      ( (*( (volatile UINT8 *)  (HW_BASE+UTMI1_BASE+reg_adr*2) ) ) = ((UINT8)data) )
#define UTMI_WRREG_ODD8(reg_adr, data)      ( (*( (volatile UINT8 *)  (HW_BASE+UTMI1_BASE+reg_adr*2-1) ) ) = ((UINT8)data) )
#define UTMI_RDREG8(reg_adr)        (*( (volatile UINT8 *)  (HW_BASE+UTMI1_BASE+reg_adr*2) ) )
#define UTMI_RDREG_ODD8(reg_adr)      (*( (volatile UINT8 *)  (HW_BASE+UTMI1_BASE+reg_adr*2-1) ) )

#endif
