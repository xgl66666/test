#include "../../../../../sboot/include/autoconf.h"

#if(ENABLE_PT6964)

#include <config.h>
#include <watchdog.h>
#include <command.h>
#include <malloc.h>
#include <msKeypad.h>
#include <environment.h>
#include "uboot_mmap.h"
#include <sys/common/MsCommon.h>
#include <sys/common/MsOS.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsDevice.h>
#include <sys/drv/drvGPIO.h>
#define COLON_ENABLE      1
#define FP_LED_MODE       0x02
// -------------- DIGITAL LED MACRO -----------------
#define LED_NUM           4
#define FP_DOT_ENABLE     0
#define FP_DOT_SEG_NUM    11
#define LED_WORD1_ADDR    6
#define LED_WORD2_ADDR    4
#define LED_WORD3_ADDR    2
#define LED_WORD4_ADDR    8
#define LED_COLON_ADDR    0
#define LED_COLON_ENABLE_LOW      0xFF
#define LED_COLON_ENABLE_HIGH     0xFF
#define LED_COLON_DISABLE_LOW     0
#define LED_COLON_DISABLE_HIGH    0

#define FRONTPNL_START_TIME_MS      3//((1000 / 50) / LED_NUM)
#define FRONTPNL_PERIOD_TIME_MS     150


typedef struct
{
    MS_U8 u8Char;
    MS_U8 lowByte;
    MS_U8 highByte;
} Char2Segment;

static const Char2Segment _char2SegmentTable[] =
{
    // char, low Byte, High Byte
    {'0', 0xFC, 0x00},
    {'1', 0xC0, 0x00},
    {'I', 0xC0, 0x00},
    {'i', 0xC0, 0x00},
    {'2', 0xB8, 0x01},
    {'3', 0xD8, 0x01},
    {'4', 0xC4, 0x03},
    {'5', 0x5C, 0x01},
    {'6', 0x7C, 0x01},
    {'7', 0xC8, 0x00},
    {'8', 0xFC, 0x01},
    {'9', 0xDC, 0x01},
    {'A', 0xEC, 0x01},
    {'B', 0xFC, 0x01},
    {'C', 0x3C, 0x00},
    {'b', 0x74, 0x01},
    {'c', 0x30, 0x01},
    {'D', 0xFC, 0x00},
    {'d', 0xF0, 0x01},
    {'E', 0x3C, 0x01},
    {'F', 0x2C, 0x01},
    {'H', 0xE4, 0x01},
    {'h', 0x64, 0x01},
    {'L', 0x34, 0x00},
    {'n', 0x60, 0x01},
    {'N', 0xEC, 0x00},
    {'O', 0xFC, 0x00},
    {'o', 0x70, 0x01},
    {'P', 0xAC, 0x01},
    {'R', 0xEC, 0x01},
    {'r', 0x24, 0x01},
    {'T', 0x34, 0x01},
    {'t', 0x34, 0x01},
    {'U', 0xF4, 0x00},
    {'V', 0xF4, 0x00},
    {'-', 0x00, 0x01},
    {' ', 0X00, 0x00},
    {'S', 0x5C, 0x01},
    {'s', 0x5C, 0x01},
};

#define GPIO_FP_DATA  1
#define GPIO_FP_STB   0
#define GPIO_FP_CLK   2

#define PT6964_KEY_L0   10//
#define PT6964_KEY_L1   5//
#define PT6964_KEY_L2   4 //
#define PT6964_KEY_L3   2//
#define PT6964_KEY_L4   1//
#define PT6964_KEY_L5   9 //


#define CMD_SET_DISP_MODE        (0x00)
#define CMD_SET_DATA             (0x40)
#define CMD_SET_ADDR             (0xC0)
#define CMD_DISP_CTRL            (0x80)

#define OPT_WRITE_SAR_RAM        (0x00)
#define OPT_READ_SAR_KEYVAL      (0x02)
#define ADDR_NOT_INC             (0x04)
#define ADDR_AUTO_INC            (0x00)

#define SAR_DISP_ON              (0x8F)
#define SAR_DISP_OFF             (0x87)

#define PM_BASE_ADDR         ((volatile U8*)0xBF000000)
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
    U8 value;                                                                          \
    U32 regOffset;                                                                     \
    regOffset = (u32Reg) * 2 - ((u32Reg) & 1);                                       \
    value = PM_BASE_ADDR[regOffset];                                                   \
    PM_BASE_ADDR[regOffset] = (bEnable) ? (value |  (u8Mask)) : (value & ~(u8Mask));\
    MST_MACRO_END

#define  SAR_GPIO_SET_HIGH(_GPIO_)\
    MST_MACRO_START\
    PM_WriteRegBit(PM_OUT_REG, 1, (BIT(_GPIO_)));\
    PM_WriteRegBit(PM_OEN_REG, 0, (BIT(_GPIO_)));\
    MST_MACRO_END

#define SAR_GPIO_SET_LOW(_GPIO_)\
    MST_MACRO_START\
    PM_WriteRegBit(PM_OUT_REG, 0, (BIT(_GPIO_)));\
    PM_WriteRegBit(PM_OEN_REG, 0, (BIT(_GPIO_)));\
    MST_MACRO_END

#define SAR_GPIO_SET_INPUT(_GPIO_)\
        PM_WriteRegBit(PM_OEN_REG, 1, (BIT(_GPIO_)));

#define SAR_GPIO_GET_INPUT(_GPIO_)\
    PM_ReadRegBit(PM_IN_REG,(BIT(_GPIO_)));

#define PM_GPIO_ENABLE(_GPIO_)\
    MST_MACRO_START\
    PM_WriteRegBit(PM_GPIO_SAR_REG, 0, (BIT(_GPIO_)));\
    MST_MACRO_END


#define CLK_DELAY_TIME       100
#define  PT6964_DIN_H   \
    _delaySomeNop(500); \
    SAR_GPIO_SET_HIGH(GPIO_FP_DATA);
#define  PT6964_DIN_L   \
    _delaySomeNop(500); \
    SAR_GPIO_SET_LOW(GPIO_FP_DATA);
#define  PT6964_STB_L   \
    _delaySomeNop(500); \
    SAR_GPIO_SET_HIGH(GPIO_FP_STB);
#define  PT6964_STB_H   \
    _delaySomeNop(500); \
    SAR_GPIO_SET_LOW(GPIO_FP_STB);
#define  PT6964_CLK_H   \
    _delaySomeNop(500); \
    SAR_GPIO_SET_LOW(GPIO_FP_CLK);
#define  PT6964_CLK_L   \
    _delaySomeNop(500); \
    SAR_GPIO_SET_HIGH(GPIO_FP_CLK);

static U8 gDispBuf[14] = {0};

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DEBUG_KEYPAD(x) x
#else
#define DEBUG_KEYPAD(x) //x
#endif

static void _delaySomeNop(MS_U16 cnt)
{
    while ((cnt--) > 0)
    {
        //WS1668_CLK_DELAY;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//change a char to BCD code for led disp
/////////////////////////////////////////////////////////////////////////////////////////////////////
const Char2Segment* __GetCharCode(U8 ch)
{
    const Char2Segment *ptbl;

    for (ptbl = _char2SegmentTable; \
            ptbl < (_char2SegmentTable + sizeof(_char2SegmentTable) / sizeof(Char2Segment)); ptbl++)
    {
        if (ch == ptbl->u8Char)
        {
            return ptbl;
        }
    }

    return NULL;
}

//-------------------------------------------------------------------------------------------------
/// 6961 Write Data
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PT6964_WriteData(MS_U8 Value)
{
    MS_U8 i , j = 0;

    for (i = 0; i < 8; i++)
    {
        if (Value & 0x01)
        {
            PT6964_DIN_H;
        }
        else
        {
            PT6964_DIN_L;
        }
        PT6964_CLK_L;
        PT6964_CLK_H;
        Value >>= 1;
        j++;
    }
}


//-------------------------------------------------------------------------------------------------
/// 6961 Clear Ram ( if you want to display new Char , you need Clear All)
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PT6964_ClearRAM(void)
{
    MS_U8  i;

    PT6964_STB_L;
    MDrv_PT6964_WriteData(0x40);            //Command 1, increment address
    PT6964_STB_H;

    PT6964_STB_L;
    MDrv_PT6964_WriteData(0xc0);            //Command 2, RAM address = 0
    for (i = 0; i <= 13; i++)                        //22 bytes
    {
        MDrv_PT6964_WriteData(0x00);
    }
    PT6964_STB_H;
}


//-------------------------------------------------------------------------------------------------
/// 6964 display on
/// @return TRUE  - Success
///         FALSE - Failure
//-------------------------------------------------------------------------------------------------
void MDrv_PT6964_Display_On(void)

{
    PT6964_STB_L;
    MDrv_PT6964_WriteData(0x8F);
    PT6964_STB_H;
}

void MDrv_FP_EnableColon(U8 bEnble)
{
    if (bEnble)
    {
        gDispBuf[LED_COLON_ADDR] = 0xFF;
        gDispBuf[LED_COLON_ADDR + 1] = 0xFF;
    }
    else
    {
        gDispBuf[LED_COLON_ADDR] = 0;
        gDispBuf[LED_COLON_ADDR + 1] = 0;
    }

}

//-------------------------------------------------------------------------------------------------
/// 6961 Write Data
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_FP_WriteAddrRam(MS_U8 addr, MS_U8 value)
{
    PT6964_STB_L;
    MDrv_PT6964_WriteData(0x44);
    PT6964_STB_H;

    PT6964_STB_L;
    MDrv_PT6964_WriteData(addr | 0xc0);
    MDrv_PT6964_WriteData(value);
    PT6964_STB_H;
}

void MDrv_FP_ClearDispBuf(void)
{
    memset(gDispBuf, 0x00, 14);
}

void MDrv_FP_EnableDot(U8 isEnabled, U8 index)
{
    MS_U8              addr = 0;

    switch (index)
    {
    case 0:
        addr = LED_WORD2_ADDR;
        break;
    case 1:
        addr = LED_WORD1_ADDR;
        break;
    case 2:
        addr = LED_WORD3_ADDR;
        break;
    case 3:
        addr = LED_WORD4_ADDR;
        break;
    default:
        return;
        break;
    }

    if (isEnabled)
    {
        gDispBuf[addr + 1] |= 0x01;
    }
    else
        gDispBuf[addr + 1] &= 0xFE;

}

void MDrv_FP_SetDigital(char ch, MS_U8 index)
{
    MS_U8              addr = 0;
    const Char2Segment *pcharSegt;

    switch (index)
    {
    case 0:
        addr = LED_WORD2_ADDR;
        break;
    case 1:
        addr = LED_WORD1_ADDR;
        break;
    case 2:
        addr = LED_WORD3_ADDR;
        break;
    case 3:
        addr = LED_WORD4_ADDR;
        break;
    default:
        return;
        break;
    }

    pcharSegt = __GetCharCode(ch);

    if (pcharSegt)
    {
        gDispBuf[addr] = pcharSegt->lowByte;
        gDispBuf[addr + 1] = pcharSegt->highByte;
    }
}

//-------------------------------------------------------------------------------------------------
/// 6961 Get Key
/// @return None
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_FP_GetKey(void)
{
    MS_U8 i, value = 0;
    MS_U8 bitLevel;

    PT6964_STB_L;

    MDrv_PT6964_WriteData(0x42);
    _delaySomeNop(10);
    SAR_GPIO_SET_INPUT(GPIO_FP_DATA);

    for (i = 0; i < 40; i++)
    {
        PT6964_CLK_L;
        value++;
        _delaySomeNop(CLK_DELAY_TIME * 2);
        bitLevel = SAR_GPIO_GET_INPUT(GPIO_FP_DATA);
        if (bitLevel != 0)
        {
            PT6964_CLK_H;
            PT6964_STB_H;
            _delaySomeNop(CLK_DELAY_TIME);
            return (value);
        }

        PT6964_CLK_H;
        _delaySomeNop(CLK_DELAY_TIME * 2);
    }
    PT6964_STB_H;
    _delaySomeNop(CLK_DELAY_TIME);
    value = 0;
    return(value);
}

void MDrv_FP_DispUpdate(void)
{
    MS_U8 i;

    PT6964_STB_L;
    MDrv_PT6964_WriteData(CMD_SET_DATA | ADDR_AUTO_INC);
    PT6964_STB_H;

    PT6964_STB_L;
    MDrv_PT6964_WriteData(CMD_SET_ADDR | 0x00);

    for (i = 0; i < 14; i++)
    {
        MDrv_PT6964_WriteData(gDispBuf[i]);
    }
    PT6964_STB_H;

    MDrv_PT6964_Display_On();
}

//-------------------------------------------------------------------------------------------------
/// Initialize 6964
/// @return TRUE  - Success
///         FALSE - Failure
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_FP_Init(void)
{
    MS_U16 i , j ;

    PM_GPIO_ENABLE(0);
    PM_GPIO_ENABLE(1);
    PM_GPIO_ENABLE(2);
    PT6964_STB_H;                   //Initial state
    PT6964_CLK_H;                   //Intial state
    PT6964_DIN_H;
    for (i = 0; i < 1000; i++)
        for (j = 0; j < 1000; j++)
        {
            ;
        }

    MDrv_PT6964_ClearRAM();
    PT6964_STB_L;
    MDrv_PT6964_WriteData(0x02);//0x02
    PT6964_STB_H;
    PT6964_STB_L;
    MDrv_PT6964_WriteData(0x8F);
    PT6964_STB_H;
    return TRUE;
}
#endif

