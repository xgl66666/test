///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2010-2015 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvFrontPnl.c
/// @brief  FrontPnl driver
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Inlucde Fils
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../../../../sboot/include/autoconf.h"
#if (ENABLE_SM1668)

#include <sys/common/MsTypes.h>

typedef struct
{
    MS_U8 m_clkPinId;
    MS_U8 m_stbPinId;
    MS_U8 m_dataPinId;
    MS_U8 m_clkLevel;
    MS_U8 m_stbLevel;
    MS_U8 m_dataLevel;
    MS_U8 m_powerKeyId;
} FP_INTERFACE_CFG;


static FP_INTERFACE_CFG gFpInterfaceCfg =
{
    .m_stbPinId = 3,
    .m_dataPinId = 2,
    .m_clkPinId = 1,
    .m_clkLevel = 0,
    .m_dataLevel = 1,
    .m_stbLevel = 0,
};


#define CLK_DELAY_TIME           500
#define BIT(x)                   (1<<(x))

#define G_LED_ON                 0x80
#define G_LED_OFF                0x00
#define G_LED_ADDR               0x08

#define OPT_WRITE_WS1668_RAM     (0x00)
#define OPT_READ_WS1668_KEYVAL   (0x02)
#define ADDR_NOT_INC             (0x04)
#define ADDR_AUTO_INC            (0x00)

#define COLON_UP_DOT_ADDR        0x03
#define COLON_UP_DOT_ENABLE      0x01
#define COLON_UP_DOT_DISABLE     0x00

#define COLON_DOWN_DOT_ADDR      0x05
#define COLON_DOWN_DOT_ENABLE    0x01
#define COLON_DOWN_DOT_DISABLE   0x00
#define FP_DISP_ON           (0x8F)
#define FP_DISP_OFF          (0x87)

#define PM_BASE_ADDR         ((volatile MS_U8*)0xBF000000)
#define PM_GPIO_SAR_REG      0x1422
#define PM_OEN_REG           0x1423
#define PM_OUT_REG           0x1424
#define PM_IN_REG            0x1425

#define MST_MACRO_START     do {
#define MST_MACRO_END       }while (0)

#define PM_ReadRegBit( u32Reg, mask )                                               \
        (PM_BASE_ADDR[(u32Reg) * 2 - ((u32Reg) & 1)] & (mask))

#define PM_WriteRegBit( u32Reg, bEnable, u8Mask )                                  \
    MST_MACRO_START                                                                    \
    MS_U8 value;                                                                          \
    MS_U32 regOffset;                                                                     \
    regOffset = (u32Reg) * 2 - ((u32Reg) & 1);                                       \
    value = PM_BASE_ADDR[regOffset];                                                   \
    PM_BASE_ADDR[regOffset] = (bEnable) ? (value |  (u8Mask)) : (value & ~(u8Mask));\
    MST_MACRO_END

#define  SM1668_GPIO_SET_HIGH(_GPIO_)\
    MST_MACRO_START\
    PM_WriteRegBit(PM_OUT_REG, 1, (BIT(_GPIO_)));\
    PM_WriteRegBit(PM_OEN_REG, 0, (BIT(_GPIO_)));\
    MST_MACRO_END

#define SM1668_GPIO_SET_LOW(_GPIO_)\
    MST_MACRO_START\
    PM_WriteRegBit(PM_OUT_REG, 0, (BIT(_GPIO_)));\
    PM_WriteRegBit(PM_OEN_REG, 0, (BIT(_GPIO_)));\
    MST_MACRO_END

#define SM1668_SET_INPUT(_GPIO_)\
        PM_WriteRegBit(PM_OEN_REG, 1, (BIT(_GPIO_)));

#define SM1668_GET_GPIO_INPUT(_GPIO_)\
    PM_ReadRegBit(PM_IN_REG,(BIT(_GPIO_)));

#define PM_GPIO_ENABLE(_GPIO_)\
    MST_MACRO_START\
    PM_WriteRegBit(PM_GPIO_SAR_REG, 0, (BIT(_GPIO_)));\
    MST_MACRO_END

#define FP_DIN_LEVEL    SM1668_GET_GPIO_INPUT(gFpInterfaceCfg.m_dataPinId)
#define FP_DIN_INPUT    SM1668_SET_INPUT(gFpInterfaceCfg.m_dataPinId)

#define SM1668_CLK_DELAY            \
    MST_MACRO_START\
    MS_U16 volatile i; for(i = 100; i > 0; i--);\
    MST_MACRO_END


void FP_STB_High(void)
{
    if (gFpInterfaceCfg.m_stbLevel)
        SM1668_GPIO_SET_HIGH(gFpInterfaceCfg.m_stbPinId);
    else
        SM1668_GPIO_SET_LOW(gFpInterfaceCfg.m_stbPinId);
}

void FP_STB_Low(void)
{
    if (gFpInterfaceCfg.m_stbLevel)
        SM1668_GPIO_SET_LOW(gFpInterfaceCfg.m_stbPinId);
    else
        SM1668_GPIO_SET_HIGH(gFpInterfaceCfg.m_stbPinId);
}

void FP_CLK_High(void)
{
    if (gFpInterfaceCfg.m_clkLevel)
        SM1668_GPIO_SET_HIGH(gFpInterfaceCfg.m_clkPinId);
    else
        SM1668_GPIO_SET_LOW(gFpInterfaceCfg.m_clkPinId);
}

void FP_CLK_Low(void)
{
    if (gFpInterfaceCfg.m_clkLevel)
        SM1668_GPIO_SET_LOW(gFpInterfaceCfg.m_clkPinId);
    else
        SM1668_GPIO_SET_HIGH(gFpInterfaceCfg.m_clkPinId);
}


void FP_DATA_High(void)
{
    if (gFpInterfaceCfg.m_dataLevel)
        SM1668_GPIO_SET_HIGH(gFpInterfaceCfg.m_dataPinId);
    else
        SM1668_GPIO_SET_LOW(gFpInterfaceCfg.m_dataPinId);
}

void FP_DATA_Low(void)
{
    if (gFpInterfaceCfg.m_dataLevel)
        SM1668_GPIO_SET_LOW(gFpInterfaceCfg.m_dataPinId);
    else
        SM1668_GPIO_SET_HIGH(gFpInterfaceCfg.m_dataPinId);
}

#define LED_NUM                  (4)
#define DOT_ENABLE_CODE          (0x80)

#define CMD_SET_DISP_MODE        (0x00)
#define CMD_SET_DATA             (0x40)
#define CMD_SET_ADDR             (0xC0)
#define CMD_DISP_CTRL            (0x80)

#define OPT_WRITE_SM1668_RAM     (0x00)
#define OPT_READ_SM1668_KEYVAL   (0x02)
#define ADDR_NOT_INC             (0x04)
#define ADDR_AUTO_INC            (0x00)

#define SM1668_DISP_ON           (0x8F)
#define SM1668_DISP_OFF          (0x87)

enum
{
    DISP_MODE_4BITS_13SEG = 0,
    DISP_MODE_5BITS_12SEG,
    DISP_MODE_6BITS_11SEG,
    DISP_MODE_7BITS_10SEG,
};

//=====================MARCOS FOR SM1668 END===================================

typedef struct
{
    MS_U8 ledBitPos;
    MS_U8 ledCharDisp;
    MS_U8 enableDisp;
} LED_DISP_INFO;


typedef struct
{
    MS_U8 u8Char;
    MS_U8 lowByte;
    MS_U8 highByte;
} Char2Segment;
//b0 b1 b2 b3 b4 b5 b6 b7
//A   B   C   D   E   F   G DP
static const Char2Segment _char2SegmentTable[] =
{
    //seg A B C D E F G H
    //SEG 0 1 2 3 4 5 6 7
    {'0', 0x3F, 0x00/*0*/},
    {'1', 0x06, 0x00/*1*/},
    {'2', 0x5B, 0x00/*2*/},
    {'3', 0x4F, 0x00/*3*/},
    {'4', 0x66, 0x00/*4*/},
    {'5', 0x6D, 0x00/*5*/},
    {'6', 0x7D, 0x00/*6*/},
    {'7', 0x07, 0x00/*7*/},
    {'8', 0x7F, 0x00/*8*/},
    {'9', 0x6F, 0x00/*9*/},
    {'A', 0x77, 0x00/*A*/},
    {'b', 0x7C, 0x00/*b*/},
    {'C', 0x39, 0x00/*C*/},
    {'c', 0x58, 0x00/*c*/},
    {'d', 0x5E, 0x00/*d*/},
    {'E', 0x79, 0x00/*E*/},
    {'F', 0x71, 0x00/*F*/},
    {'H', 0x76, 0x00/*H*/},
    {'h', 0x74, 0x00/*h*/},
    {'L', 0x38, 0x00/*L*/},
    {'n', 0x54, 0x00/*n*/},
    {'N', 0x37, 0x00/*N*/},
    {'o', 0x5C, 0x00/*o*/},
    {'P', 0x73, 0x00/*P*/},
    {'q', 0x67, 0x00/*q*/},
    {'r', 0x50, 0x00/*r*/},
    {'t', 0x78, 0x00/*t*/},
    {'U', 0x3E, 0x00/*U*/},
    {'y', 0x6E, 0x00/*y*/},
    {'-', 0x40, 0x00/*-*/}
};


static MS_U8 gDispBuf[14] = {0};
static MS_U8 gdevFpInited = 0;

/////////////////////////////////////////////////////////////////////////////
//WS1668 turn on display
/////////////////////////////////////////////////////////////////////////////

static void _delaySomeNop(MS_U16 cnt)
{
    while ((cnt--) > 0)
    {
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");

    }
}

static void __WriteByte_2_WS1668(MS_U8 value)
{
    MS_U8 i;

    //FP_CLK_High();
    //_delaySomeNop(CLK_DELAY_TIME);

    for (i = 0; i < 8; i++)
    {

        if (value & 0x01)
            FP_DATA_High();
        else
            FP_DATA_Low();

        FP_CLK_Low();
        _delaySomeNop(CLK_DELAY_TIME);
        FP_CLK_High();
        _delaySomeNop(CLK_DELAY_TIME);

        value >>= 1;
    }

}

static void __WS1668_DispOn(void)
{

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(SM1668_DISP_ON);
    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

}

/////////////////////////////////////////////////////////////////////////////
//clear all the WS1668 ram, if we want to display a new char
/////////////////////////////////////////////////////////////////////////////
static void MDrv_SM1668_ClearRam(void)
{
    MS_U16 i;

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_DATA | ADDR_AUTO_INC);
    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_ADDR | 0x00);


    for (i = 14; i != 0; i--)
    {
        __WriteByte_2_WS1668(0x00);
    }

    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//change a char to BCD code for led disp
/////////////////////////////////////////////////////////////////////////////////////////////////////
static MS_U8 __GetCharCode(MS_U8 ch)
{
    const Char2Segment *ptbl;

    for (ptbl = _char2SegmentTable; \
            ptbl < (_char2SegmentTable + sizeof(_char2SegmentTable) / sizeof(Char2Segment)); ptbl++)
    {
        if (ch == ptbl->u8Char)
        {
            return ptbl->lowByte;
        }
    }
    //need promt invalid char?
    return 0xFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Frontpnl update a specified led
////////////////////////////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_SM1668_UpdateLedDisp(MS_U8 ledPos, MS_U8 ch, MS_U8 bEnableDot)
{
    MS_U8 charCode;

    charCode = __GetCharCode(ch);
    if (bEnableDot)
    {
        charCode |= DOT_ENABLE_CODE;
    }

    MDrv_SM1668_WriteAddrRam(charCode, ledPos * 2);
    MDrv_SM1668_WriteAddrRam(0x00, ledPos * 2 + 1);

    __WS1668_DispOn();

    return TRUE;
}

static MS_U8 __Get_WS1668_KeyVal(void)
{
    MS_U8 i;
    MS_U8 bitLevel;

    FP_DIN_INPUT;
    FP_DATA_Low();

    for (i = 0; i < 40; i++)
    {
        FP_CLK_Low();
        _delaySomeNop(CLK_DELAY_TIME * 4);
        bitLevel = FP_DIN_LEVEL;
        if (bitLevel != 0)
        {
            FP_CLK_High();
            _delaySomeNop(CLK_DELAY_TIME);
            return (i);
        }

        FP_CLK_High();
        _delaySomeNop(CLK_DELAY_TIME * 4);
    }
    return (0xFF);
}

////////////////////////////////////////////////////////////////////////////////////
//@control led on and off
//@bLightOn 1 ON, 0 OFF
////////////////////////////////////////////////////////////////////////////////////
void  MDrv_SM1668_GreenLedOn(MS_U8 bLightOn)
{
    MS_U8 value;

    value = bLightOn ? G_LED_ON : G_LED_OFF;
    gDispBuf[G_LED_ADDR] &= (~G_LED_ON);
    gDispBuf[G_LED_ADDR] |= value;
    MDrv_SM1668_WriteAddrRam(gDispBuf[G_LED_ADDR], G_LED_ADDR);
    __WS1668_DispOn();

}

/////////////////////////////////////////////////////////////////////////////////////
//time colon control by GR3 and GR5
/////////////////////////////////////////////////////////////////////////////////////
void MDrv_SM1668_TimeColonEn(MS_U8 bEnable)
{
    if (!gdevFpInited)
    {
        return;
    }
    if (bEnable)
    {
        MDrv_SM1668_WriteAddrRam(COLON_UP_DOT_ENABLE, COLON_UP_DOT_ADDR);
        MDrv_SM1668_WriteAddrRam(COLON_DOWN_DOT_ENABLE, COLON_DOWN_DOT_ADDR);
        gDispBuf[COLON_UP_DOT_ADDR] |= COLON_UP_DOT_ENABLE;
        gDispBuf[COLON_UP_DOT_ENABLE] |= COLON_UP_DOT_ENABLE;
    }
    else
    {
        MDrv_SM1668_WriteAddrRam(COLON_UP_DOT_DISABLE, COLON_UP_DOT_ADDR);
        MDrv_SM1668_WriteAddrRam(COLON_UP_DOT_DISABLE, COLON_DOWN_DOT_ADDR);
        gDispBuf[COLON_UP_DOT_ADDR] &= ~COLON_UP_DOT_ENABLE;
        gDispBuf[COLON_UP_DOT_ENABLE] &= ~COLON_UP_DOT_ENABLE;
    }
    __WS1668_DispOn();
}

/////////////////////////////////////////////////////////////////////////////////////
//@RedLedOnOff
//@LED ON all high address bit2 should be high level(seg10)
/////////////////////////////////////////////////////////////////////////////////////
void MDrv_SM1668_RedLed_OnOff(MS_U8 bLightOn)
{
    MS_U8 value;
    MS_U8 i;

    value = bLightOn ? (0x00) : (0x02);
    for (i = 0; i < 14; i += 2)
    {
        gDispBuf[i + 1] &= (~0x02);
        gDispBuf[i + 1] |= value;
        MDrv_SM1668_WriteAddrRam(gDispBuf[i + 1], (i + 1));
    }
}

//////////////////////////////////////////////////////////////////////////
//write data to the specific ram address of WS1668
//////////////////////////////////////////////////////////////////////////
void MDrv_FP_WriteAddrRam(MS_U8 addr, MS_U8 value)
{

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_DATA | ADDR_NOT_INC);
    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_ADDR | addr);
    __WriteByte_2_WS1668(value);
    gDispBuf[addr] |= value;

    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);
    __WS1668_DispOn();
}

////////////////////////////////////////////////////////////////////////////////////////
//@clear the dispbuf
//@call the MDrv_SM1668_DispUpdate to update the frontpna display
/////////////////////////////////////////////////////////////////////////////////////////
void MDrv_FP_ClearDispBuf(void)
{
    MS_U8 i;

    for (i = 0; i < 8; i++)
    {
        gDispBuf[i] = 0x00;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//@digital: display a dot
//@index:   from rigth to left(0, 1, 2, 3)
//@pls call MDrv_SM1668_DispUpdate to display it
/////////////////////////////////////////////////////////////////////////////////////////
void MDrv_FP_EnableDot(MS_U8 isEnabled, MS_U8 index)
{
    MS_U8 value;

    value = isEnabled ? DOT_ENABLE_CODE : 0;
    gDispBuf[index * 2] &= (~DOT_ENABLE_CODE);
    gDispBuf[index * 2] |= DOT_ENABLE_CODE;
}

////////////////////////////////////////////////////////////////////////////////////////
//@digital: a char to disp('0', '1', '2', '3'.....)
//@index:   from rigth to left(0, 1, 2, 3)
//@pls call MDrv_SM1668_DispUpdate to display it
/////////////////////////////////////////////////////////////////////////////////////////
void MDrv_FP_SetDigital(MS_U8 ch, MS_U8 index)
{
    gDispBuf[index * 2] &= DOT_ENABLE_CODE;
    gDispBuf[index * 2] |=  __GetCharCode(ch);
}

MS_U8 MDrv_FP_GetKey(void)
{
    MS_U8 keyval;

    if (!gdevFpInited)
    {
        return TRUE;
    }

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_DATA | OPT_READ_WS1668_KEYVAL);
    _delaySomeNop(10);

    keyval = __Get_WS1668_KeyVal();

    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

    //__WS1668_DispOn();//must add this, if not the console will not work ok.

    return keyval;
}

////////////////////////////////////////////////////////////////////////////////////////
//@update the display of frontpnl
/////////////////////////////////////////////////////////////////////////////////////////
void MDrv_FP_DispUpdate(void)
{
    MS_U8 i;

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_DATA | ADDR_AUTO_INC);
    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_ADDR | 0x00);

    for (i = 0; i < 14; i++)
    {
        __WriteByte_2_WS1668(gDispBuf[i]);
    }
    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

    __WS1668_DispOn();
}

MS_U8 MDrv_FP_Init(void)
{
    PM_GPIO_ENABLE(gFpInterfaceCfg.m_stbPinId);
    SM1668_CLK_DELAY;
    PM_GPIO_ENABLE(gFpInterfaceCfg.m_clkPinId);
    SM1668_CLK_DELAY;
    PM_GPIO_ENABLE(gFpInterfaceCfg.m_dataPinId);
    SM1668_CLK_DELAY;

    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);
    FP_CLK_High();
    _delaySomeNop(CLK_DELAY_TIME);
    FP_DATA_High();
    _delaySomeNop(CLK_DELAY_TIME);

    MDrv_SM1668_ClearRam();

    FP_STB_Low();
    _delaySomeNop(CLK_DELAY_TIME);
    __WriteByte_2_WS1668(CMD_SET_DISP_MODE | DISP_MODE_4BITS_13SEG);
    FP_STB_High();
    _delaySomeNop(CLK_DELAY_TIME);

    __WS1668_DispOn();
    gdevFpInited = 1;

    return TRUE;
}
#endif

