////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¨MStar Confidential Information〃) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvFrontPnl.c
/// @brief  FrontPanel Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "../../../../../sboot/include/autoconf.h"
#if(ENABLE_HD1650)

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

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define FRONTPNL_START_TIME_MS      3   //((1000 / 50) / LED_NUM)
#define FRONTPNL_PERIOD_TIME_MS     150
#define FP_LED_MAX_NUM 5
#define DBG_LED(msg)                    msg

// 设置系统参数命令
#define HD1650_BIT_ENABLE   0x4801    // 开启/关闭位
#define HD1650_BIT_SLEEP  0x4804    // 睡眠控制位
#define HD1650_BIT_7SEG 0x4809    // 7段控制位
#define HD1650_BIT_INTENS1  0x4811    // 1级亮度
#define HD1650_BIT_INTENS2  0x4821    // 2级亮度
#define HD1650_BIT_INTENS3  0x4831    // 3级亮度
#define HD1650_BIT_INTENS4  0x4841    // 4级亮度
#define HD1650_BIT_INTENS5  0x4851    // 5级亮度
#define HD1650_BIT_INTENS6  0x4861    // 6级亮度
#define HD1650_BIT_INTENS7  0x4871    // 7级亮度
#define HD1650_BIT_INTENS8  0x4801    // 8级亮度

// 加载字数据命令
#define HD1650_DIG0   0x6800      // 数码管位0显示,需另加8位数据
#define HD1650_DIG1   0x6A00      // 数码管位1显示,需另加8位数据
#define HD1650_DIG2   0x6C00      // 数码管位2显示,需另加8位数据
#define HD1650_DIG3   0x6E00      // 数码管位3显示,需另加8位数据

#define HD1650_DOT    0x0080      // 数码管小数点显示

// 读取按键代码命令
#define HD1650_GET_KEY  0x4F        // 获取按键,返回按键代码
#define LEDMAPNUM 35
//key mapping
typedef struct
{
    MS_U8 keyMapData;
    MS_U8 keyMapLevel;
} MS_KEYMAP;

//led mapping
typedef struct _led_bitmap
{
    MS_U8 character;
    MS_U8 bitmap;
} led_bitmap;


#define GPIO_FP_DATA                  18
#define GPIO_FP_CLK                     19

#define CLK_DELAY_TIME       100
#define  HD1650_SCL_SET   \
      mdrv_gpio_set_low(GPIO_FP_CLK);
#define  HD1650_SCL_CLR  \
      mdrv_gpio_set_high(GPIO_FP_CLK);
#define HD1650_SCL_D_OUT
#define  HD1650_SDA_SET   \
      mdrv_gpio_set_high(GPIO_FP_DATA);
#define  HD1650_SDA_CLR    \
      mdrv_gpio_set_low(GPIO_FP_DATA);
#define  HD1650_SDA_IN    \
      mdrv_gpio_get_level(GPIO_FP_DATA)
#define  HD1650_SDA_D_OUT
#define  HD1650_SDA_D_IN   \
      mdrv_gpio_set_input(GPIO_FP_DATA);
#define  HD1650_GPIO_INIT  \
      mdrv_gpio_init();
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

static const  led_bitmap bcd_decode_tab[LEDMAPNUM] =
{
    {'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
    {'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
    {'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
    {'b', 0x7C}, {'B', 0x7C}, {'c', 0x58}, {'C', 0x39},
    {'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
    {'f', 0x71}, {'F', 0x71}, {'o', 0x5C}, {'t', 0x78},
    {'l', 0x30}, {'L', 0x38}, {'n', 0x37}, {'p', 0x73},
    {'P', 0x73}, {'O', 0x3F}, {'u', 0x1C}, {'U', 0x3E},
    {'S', 0x6D}, {'s', 0x6D}, {'-', 0x40}
};//BCD decode

static U8 gDispBuf[5] = {0};
static U8 num = 0;
MS_BOOL sec_flag;

void Delay1(void)
{
    MS_U32 iCount = 1000;
    while (iCount)
    {
        iCount--;
        if (iCount == 0)
            return;
    }
}

/****************************************************************
 *
 *   Function Name:HD1650_Start
 *
 *   Description:
 *
 *   Parameter:
 *
 *   return：
****************************************************************/
void MDrv_HD1650_Start(void)
{
    HD1650_SDA_SET;
    HD1650_SCL_SET;
    Delay1();
    HD1650_SDA_CLR;
    Delay1();
    HD1650_SCL_CLR;
}
/****************************************************************
 *
 *    Function Name:HD1650_Stop
 *
 *    Description:
 *
 *    Parameter:
 *
 *    return：
****************************************************************/
void MDrv_HD1650_Stop(void)
{
    HD1650_SDA_CLR;
    Delay1();
    HD1650_SCL_SET;
    Delay1();
    HD1650_SDA_SET;
    Delay1();
}

/****************************************************************
 *
 *    Function Name:HD1650_WrByte
 *
 *    Description: Write One Byte data
 *
 *    Parameter: data
 *
 *    return：
****************************************************************/
void MDrv_HD1650_WrByte(MS_U8 dat)
{
    MS_U8 i;
    Delay1();
    for (i = 0; i != 8; i++)
    {
        if (dat & 0x80)
        {
            HD1650_SDA_SET;
        }
        else
        {
            HD1650_SDA_CLR;
        }
        Delay1();
        HD1650_SCL_SET;
        dat <<= 1;
        Delay1();  // choose delay
        HD1650_SCL_CLR;
    }
    HD1650_SDA_CLR;
    Delay1();
    HD1650_SCL_SET;
    Delay1();
    HD1650_SCL_CLR;
}

/****************************************************************
 *
 *    Function Name:HD1650_WrByte
 *
 *    Description: Read One Byte data
 *
 *    Parameter:
 *
 *    return：data
****************************************************************/
MS_U8 MDrv_HD1650_RdByte(void)
{
    MS_U8 dat, i;
    HD1650_SDA_D_IN;
    dat = 0;
    for (i = 0; i != 8; i++)
    {
        Delay1();  //choose delay
        HD1650_SCL_SET;
        Delay1();  // choose delay
        dat <<= 1;
        if (HD1650_SDA_IN) dat++;
        HD1650_SCL_CLR;
    }
    HD1650_SDA_CLR;
    Delay1();
    HD1650_SCL_SET;
    Delay1();
    HD1650_SCL_CLR;
    return dat;
}

/****************************************HD1650 function**********************/

/****************************************************************
 *
 *    Function Name:HD1650_Write
 *
 *    Description:by cmd
 *
 *    Parameter: cmd HD1650.H
 *
 *    return：
****************************************************************/
void MDrv_HD1650_Write(MS_U16 cmd)   //write cmd
{
    MDrv_HD1650_Start();
    MDrv_HD1650_WrByte((MS_U8)(cmd >> 8));
    MDrv_HD1650_WrByte((MS_U8)cmd);
    MDrv_HD1650_Stop();
    return;
}

/****************************************************************
 *
 *    Function Name:HD1650_Read
 *
 *    Description:read key value
 *
 *    Parameter:
 *
 *    return：key value
****************************************************************/
MS_U8 MDrv_HD1650_Read(void)
{
    MS_U8 keycode = 0;
    MS_U32 value = 0;

    MDrv_HD1650_Start();
    value = HD1650_GET_KEY ;
    MDrv_HD1650_WrByte((MS_U8)value);
    keycode = MDrv_HD1650_RdByte();
    MDrv_HD1650_Stop();

    if ((keycode & 0x00000040) == 0)
        keycode = 0X2E;

    return keycode;
}

MS_BOOL MDrv_FrontPnl_EnableLED(MS_BOOL  eEnableLED)
{
    if (eEnableLED)
    {
        mdrv_gpio_set_high(GPIO_FP_CLK);
    }
    else
    {
        mdrv_gpio_set_low(GPIO_FP_CLK);
    }
    return TRUE;
}


/****************************************************************
 *
 *   Function Name:Led_Get_Code
 *
 *   Description:转换字符为数码管的显示码，如遇到无法转换的字符返回0
 *
 *   Parameter: cTemp  待转换为显示码的字符
 *
 *   return：显示码值
****************************************************************/
static MS_U8 MDrv_Led_Get_Code(MS_U8 cTemp)
{
    MS_U8 i, bitmap = 0x00;

    for (i = 0; i < LEDMAPNUM; i++)
    {
        if (bcd_decode_tab[i].character == cTemp)
        {
            bitmap = bcd_decode_tab[i].bitmap;
            break;
        }
    }
    return bitmap;
}

void MDrv_HD1650_Init(void)
{
    MDrv_HD1650_Write(HD1650_BIT_ENABLE); //系统使能
    MDrv_HD1650_Write(HD1650_BIT_7SEG);
    MDrv_HD1650_Write(HD1650_BIT_INTENS8);
}

void MDrv_FP_WriteAddrRam(MS_U8 addr, MS_U8 value) //write cmd
{
    addr = addr;
    value = value;
    return;
}

void MDrv_FP_ClearDispBuf(void)
{
    memset(gDispBuf, 0x00, 5);
    num = 0;
}

void MDrv_FP_EnableDot(U8 isEnabled, U8 index)
{
    index = index;
    if (isEnabled)
    {
        sec_flag = TRUE;
    }
    else
        sec_flag = FALSE;
}

void MDrv_FP_SetDigital(char ch, MS_U8 index)
{
    MS_U8      addr = 0;
    switch (index)
    {
    case 0:
        addr = 0;
        break;
    case 1:
        addr = 1;
        break;
    case 2:
        addr = 2;
        break;
    case 3:
        addr = 3;
        break;
    default:
        return;
        break;
    }
    num += 1;
    gDispBuf[addr] = ch;

}

MS_U8 MDrv_FP_GetKey(void)
{
    MS_U8 KeyValue = 0;
    return KeyValue;
}

//-------------------------------------------------------------------------------------------------
/// initialze Front Panel display control.
/// Function Name:MDrv_FrontPnl_Update
///
/// Description:简单的数码管显示应用示例
///
///  Parameter:  FPStr 显示的字符串；
///                  sec_flag开启小数点标志位；
///  return：无
//-------------------------------------------------------------------------------------------------
void MDrv_FP_DispUpdate(void)
{
    int i;
    int data[4] = {0x00, 0x00, 0x00, 0x00};

    if (0 == num)
        return ;

    if (num > 4)
        num = 4;

    for (i = 0; i < num; i++)
    {
        data[i] = MDrv_Led_Get_Code(gDispBuf[i]);
    }



    MDrv_HD1650_Write(HD1650_BIT_INTENS8);// 开启显示和键盘，8段显示方式
    //发显示数据
    MDrv_HD1650_Write(HD1650_DIG0 | (MS_U8)data[0]);   //点亮第一个数码管

    if (sec_flag)
        MDrv_HD1650_Write(HD1650_DIG1 | (MS_U8)data[1] | HD1650_DOT);   //点亮第二个数码管
    else
        MDrv_HD1650_Write(HD1650_DIG1 | (MS_U8)data[1]);

    MDrv_HD1650_Write(HD1650_DIG2 | (MS_U8)data[2]);  //点亮第三个数码管
    MDrv_HD1650_Write(HD1650_DIG3 | (MS_U8)data[3]);   //点亮第四个数码管
}

void MDrv_FP_Init(void)
{
	  HD1650_GPIO_INIT
    MDrv_HD1650_Init();
}
#endif

