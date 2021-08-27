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

#ifndef _MSKEYPAD_H_
#define _MSKEYPAD_H_

#if (CONFIG_COMMANDS & CFG_CMD_KEYPAD)

#ifdef  _MSKEYPAD_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//#define KeypadRepeatTimerCount 1 //100ms based

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if 0
#define BIT0	                    0x00000001
#define BIT1	                    0x00000002
#define BIT2	                    0x00000004
#define BIT3	                    0x00000008
#define BIT4	                    0x00000010
#define BIT5	                    0x00000020
#define BIT6	                    0x00000040
#define BIT7	                    0x00000080
#endif
/// MS Return Error
#define MSRET_ERROR     0
/// MS Return OK
#define MSRET_OK        1

// definition for TRUE
#define TRUE            1
// definition for FALSE
#define FALSE           0

//typedef unsigned char  U8;
//typedef unsigned int   U16;
//typedef unsigned long  U32;
//typedef U8      BOOLEAN;   ///< BOOLEAN
#define BOOL    BOOLEAN
#define ENABLE          1
#define DISABLE         0
#ifndef NULL
#define NULL	((void *) 0)
#endif

//-------------------------------------------------------------------------------------------------
//  enum
//-------------------------------------------------------------------------------------------------
enum KEYPAD_ADC_CHANNEL
{
    KEYPAD_ADC_CHANNEL_1 = 0,
    KEYPAD_ADC_CHANNEL_2,
    KEYPAD_ADC_CHANNEL_3,
    KEYPAD_ADC_CHANNEL_4,
    KEYPAD_ADC_CHANNEL_5,
    KEYPAD_ADC_CHANNEL_6,
    KEYPAD_ADC_CHANNEL_7,
    KEYPAD_ADC_CHANNEL_8,
};

#define SAR_SIGNAL_CH                   KEYPAD_ADC_CHANNEL_5

#define IRKEY_UP                            0x52
#define IRKEY_MENU                      0x07
#define IRKEY_LEFT                      0x06
#define IRKEY_MUTE                      0x5A
#define IRKEY_POWER                     0x46
#define IRKEY_INPUT_SOURCE      0x04
#define IRKEY_RIGHT                     0x1A
#define IRKEY_DOWN                      0x13

//-------------------------------------------------------------------------------------------------
//  register define
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_TITANIA4))
#define RIU_MAP  0xBF000000
#define RIU8    ((unsigned char  volatile *) RIU_MAP)
#define RIUBASE_PM_SAR              0x001400    //0x001400 ~ 0x0014ff
#else
#error "ERROR! The Current msKeypad doesn't support this CHIP config..."
#endif
#define RIU      ((unsigned short volatile *) RIU_MAP)
#define MDrv_WriteByte(u32Reg, u8Value)     RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value
#define MDrv_ReadByte(u32Reg)                   RIU8[(u32Reg << 1) - (u32Reg & 1)]
//------------------------------------------------------------------------------
// SAR define
#if defined(CONFIG_TITANIA4)
// SAR define
#define REG_SAR_CTRL0                   (RIUBASE_PM_SAR+0x00)
    #define SAR_SINGLE_CH_MSK           (BIT2|BIT1|BIT0)//[2:0] //select channel for single channel modech0~ch7
    #define SAR_LEVEL_TRIGGER           (BIT3) //keypad level trigger enable. 0:  use edge trigger, 1:  use level trigger
    #define SAR_SINGLE_CH_EN            (BIT4) //enable single channel mode. 0: disable1: enable
    #define SAR_MODE_FREERUN            (BIT5) //select sar digital operation mode. 0: one-shot, 1: freerun
    #define SAR_DIGITAL_PWRDN           (BIT6) //sar digital power down
    #define SAR_START                   (BIT7) //sar start signal
#define REG_SAR_CTRL1                   (RIUBASE_PM_SAR+0x01)
    #define SAR_ADC_PWRDN               (BIT0)
    #define SAR_ADC_FREERUN             (BIT1)
#define REG_SAR_SELCH                   (RIUBASE_PM_SAR+0x01)
    #define SAR_SEL                     (BIT2)
    #define SAR_NCH_EN                  (BIT3)
#define REG_SAR_CKSAMP_PRD              (RIUBASE_PM_SAR+0x02)
#define REG_SAR1_UPB                    (RIUBASE_PM_SAR+0x0A)
#define REG_SAR1_LOB                    (RIUBASE_PM_SAR+0x0B)
#define REG_SAR2_UPB                    (RIUBASE_PM_SAR+0x0C)
#define REG_SAR2_LOB                    (RIUBASE_PM_SAR+0x0D)
#define REG_SAR3_UPB                    (RIUBASE_PM_SAR+0x0E)
#define REG_SAR3_LOB                    (RIUBASE_PM_SAR+0x0F)
#define REG_SAR4_UPB                    (RIUBASE_PM_SAR+0x10)
#define REG_SAR4_LOB                    (RIUBASE_PM_SAR+0x11)
#define REG_SAR5_UPB                    (RIUBASE_PM_SAR+0x12)
#define REG_SAR5_LOB                    (RIUBASE_PM_SAR+0x13)
#define REG_SAR6_UPB                    (RIUBASE_PM_SAR+0x14)
#define REG_SAR6_LOB                    (RIUBASE_PM_SAR+0x15)
#define REG_SAR7_UPB                    (RIUBASE_PM_SAR+0x16)
#define REG_SAR7_LOB                    (RIUBASE_PM_SAR+0x17)
#define REG_SAR8_UPB                    (RIUBASE_PM_SAR+0x18)
#define REG_SAR8_LOB                    (RIUBASE_PM_SAR+0x19)
#define REG_SAR_ADCOUT1                 (RIUBASE_PM_SAR+0x1A)
#define REG_SAR_ADCOUT2                 (RIUBASE_PM_SAR+0x1B)
#define REG_SAR_ADCOUT3                 (RIUBASE_PM_SAR+0x1C)
#define REG_SAR_ADCOUT4                 (RIUBASE_PM_SAR+0x1D)
#define REG_SAR_ADCOUT5                 (RIUBASE_PM_SAR+0x1E)
#define REG_SAR_ADCOUT6                 (RIUBASE_PM_SAR+0x1F)
#define REG_SAR_ADCOUT7                 (RIUBASE_PM_SAR+0x20)
#define REG_SAR_ADCOUT8                 (RIUBASE_PM_SAR+0x21)
    #define MASK_SAR_ADCOUT             (0xFF)
#define REG_SAR_AISEL                   (RIUBASE_PM_SAR+0x22)
    #define SAR_AISEL_MSK               (0x1F)
    #define SAR_AISEL_CH0_MSK           (BIT0)
    #define SAR_AISEL_CH1_MSK           (BIT1)
    #define SAR_AISEL_CH2_MSK           (BIT2)
    #define SAR_AISEL_CH3_MSK           (BIT3)

#define REG_SAR_GPIOOEN                 (RIUBASE_PM_SAR+0x23)
    #define SAR_GPIOOEN_MSK             (0x1F)
    #define SAR_GPIOOEN_CH0_MSK         (BIT0)
    #define SAR_GPIOOEN_CH1_MSK         (BIT1)
    #define SAR_GPIOOEN_CH2_MSK         (BIT2)
    #define SAR_GPIOOEN_CH3_MSK         (BIT3)

#define REG_SAR_INT_MASK                (RIUBASE_PM_SAR+0x28)
    #define SAR_INT_MASK                (BIT0)
#endif

INTERFACE void  msKeypad_Init(void);
INTERFACE unsigned char msKeypad_GetKey(unsigned char *pkey, unsigned char *pflag);

#undef INTERFACE

#endif
#endif

