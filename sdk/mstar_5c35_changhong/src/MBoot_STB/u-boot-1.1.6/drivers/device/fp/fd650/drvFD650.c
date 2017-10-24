#include "../../../../../sboot/include/autoconf.h"
#if(ENABLE_FD650)
#include <common.h>
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
//#include "../drvFP.h"

//led mapping
typedef struct _led_bitmap
{
    MS_U8 character;
    MS_U8 bitmap;
} led_bitmap;



#define FD650_BIT_ENABLE  0x01    // 开启/关闭位
#define FD650_BIT_SLEEP   0x04    // 睡眠控制位
#define FD650_BIT_7SEG    0x08    // 7段控制位
#define FD650_BIT_INTENS1 0x10    // 1级亮度
#define FD650_BIT_INTENS2 0x20    // 2级亮度
#define FD650_BIT_INTENS3 0x30    // 3级亮度
#define FD650_BIT_INTENS4 0x40    // 4级亮度
#define FD650_BIT_INTENS5 0x50    // 5级亮度
#define FD650_BIT_INTENS6 0x60    // 6级亮度
#define FD650_BIT_INTENS7 0x70    // 7级亮度
#define FD650_BIT_INTENS8 0x00    // 8级亮度

#define FD650_SYSOFF  0x0400      // 关闭显示、关闭键盘
#define FD650_SYSON ( FD650_SYSOFF | FD650_BIT_ENABLE ) // 开启显示、键盘
#define FD650_SLEEPOFF  FD650_SYSOFF  // 关闭睡眠
#define FD650_SLEEPON ( FD650_SYSOFF | FD650_BIT_SLEEP )  // 开启睡眠
#define FD650_7SEG_ON ( FD650_SYSON | FD650_BIT_7SEG )  // 开启七段模式
#define FD650_8SEG_ON ( FD650_SYSON | 0x00 )  // 开启八段模式
#define FD650_SYSON_1 ( FD650_SYSON | FD650_BIT_INTENS1 ) // 开启显示、键盘、1级亮度
//以此类推
#define FD650_SYSON_4 ( FD650_SYSON | FD650_BIT_INTENS4 ) // 开启显示、键盘、4级亮度
//以此类推
#define FD650_SYSON_8 ( FD650_SYSON | FD650_BIT_INTENS8 ) // 开启显示、键盘、8级亮度


// 加载字数据命令
#define FD650_DIG0    0x1400      // 数码管位0显示,需另加8位数据
#define FD650_DIG1    0x1500      // 数码管位1显示,需另加8位数据
#define FD650_DIG2    0x1600      // 数码管位2显示,需另加8位数据
#define FD650_DIG3    0x1700      // 数码管位3显示,需另加8位数据

#define FD650_DOT     0x0080      // 数码管小数点显示

// 读取按键代码命令
#define FD650_GET_KEY 0x0700          // 获取按键,返回按键代码

#define GPIO_FP_DATA                   18
#define GPIO_FP_CLK                     19
#define LEDMAPNUM 36

#define CLK_DELAY_TIME       100
#define  FD650_SCL_SET   \
      mdrv_gpio_set_high(GPIO_FP_CLK);
#define  FD650_SCL_CLR  \
      mdrv_gpio_set_low(GPIO_FP_CLK);
#define FD650_SCL_D_OUT
#define  FD650_SDA_SET   \
      mdrv_gpio_set_high(GPIO_FP_DATA);
#define  FD650_SDA_CLR    \
      mdrv_gpio_set_low(GPIO_FP_DATA);
#define  FD650_SDA_IN    \
      mdrv_gpio_get_level(GPIO_FP_DATA)
#define  FD650_SDA_D_OUT
#define  FD650_SDA_D_IN   \
      mdrv_gpio_set_input(GPIO_FP_DATA);
#define  FD650_GPIO_INIT  \
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
    {'S', 0x6D}, {'s', 0x6D}, {'-', 0x40}, {' ', 0x00}
};//BCD decode

static U8 gDispBuf[5] = {0};
static U8 num = 0;
MS_BOOL sec_flag;

/*************************************************************************************/

static void _delaySomeNop(MS_U16 cnt)
{
    while ((cnt--) > 0)
    {
        //FD650_CLK_DELAY;
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
        asm __volatile__("nop");
    }
}

/****************************************************************
 *
 *   Function Name:FD650_Start
 *
 *   Description:
 *
 *   Parameter:
 *
 *   return：
****************************************************************/
void MDrv_FD650_Start(void)
{
    FD650_SDA_SET;
    FD650_SDA_D_OUT;
    FD650_SCL_SET;
    FD650_SCL_D_OUT;
    _delaySomeNop(500);
    FD650_SDA_CLR;
    _delaySomeNop(500);
    FD650_SCL_CLR;
}

/****************************************************************
 *
 *    Function Name:FD650_Stop
 *
 *    Description:
 *
 *    Parameter:
 *
 *    return：
****************************************************************/
void MDrv_FD650_Stop(void)
{
    FD650_SDA_CLR;
    FD650_SDA_D_OUT;
    _delaySomeNop(500);
    FD650_SCL_SET;
    _delaySomeNop(500);
    FD650_SDA_SET;
    _delaySomeNop(500);
    FD650_SDA_D_IN;
}

/****************************************************************
 *
 *    Function Name:FD650_WrByte
 *
 *    Description: Write One Byte data
 *
 *    Parameter: data
 *
 *    return：
****************************************************************/
void MDrv_FD650_WrByte(MS_U8 dat)
{
    MS_U8 i;
    FD650_SDA_D_OUT;
    for (i = 0; i != 8; i++)
    {
        if (dat & 0x80)
        {
            FD650_SDA_SET;
        }
        else
        {
            FD650_SDA_CLR;
        }
        _delaySomeNop(500);
        FD650_SCL_SET;
        dat <<= 1;
        _delaySomeNop(500);  // choose delay
        FD650_SCL_CLR;
    }
    FD650_SDA_D_IN;
    FD650_SDA_SET;
    _delaySomeNop(500);
    FD650_SCL_SET;
    _delaySomeNop(500);
    FD650_SCL_CLR;
}

/****************************************************************
 *
 *    Function Name:FD650_WrByte
 *
 *    Description: Read One Byte data
 *
 *    Parameter:
 *
 *    return：data
****************************************************************/
MS_U8 MDrv_FD650_RdByte(void)
{
    MS_U8 dat, i;
    FD650_SDA_SET;
    FD650_SDA_D_IN;
    dat = 0;
    for (i = 0; i != 8; i++)
    {
        _delaySomeNop(500);  //choose delay
        FD650_SCL_SET;
        _delaySomeNop(500);  // choose delay
        dat <<= 1;
        if (FD650_SDA_IN) dat++;
        _delaySomeNop(500); //
        FD650_SCL_CLR;
    }
    FD650_SDA_SET;
    _delaySomeNop(500);
    FD650_SCL_SET;
    _delaySomeNop(500);
    FD650_SCL_CLR;
    return dat;
}

/****************************************FD650 function**********************/

/****************************************************************
 *
 *    Function Name:FD650_Write
 *
 *    Description:by cmd
 *
 *    Parameter: cmd FD650.H
 *
 *    return：
****************************************************************/
void MDrv_FD650_Write(MS_U16 cmd)   //write cmd
{
    MDrv_FD650_Start();
    MDrv_FD650_WrByte(((MS_U8)(cmd >> 7) & 0x3E) | 0x40);
    MDrv_FD650_WrByte((MS_U8)cmd);
    _delaySomeNop(1000);
    MDrv_FD650_Stop();
    return;
}

/****************************************************************
 *
 *    Function Name:FD650_Read
 *
 *    Description:read key value
 *
 *    Parameter:
 *
 *    return：key value
****************************************************************/
MS_U8 MDrv_FD650_Read(void)
{
    MS_U8 keycode = 0;
    MS_U32 value = 0;
    MDrv_FD650_Start();
    value = ((FD650_GET_KEY >> 7) & 0x3E) | 0x01 | 0x40 ;
    MDrv_FD650_WrByte((MS_U8)value);
    keycode = MDrv_FD650_RdByte();
    //printf("keycode = 0X%X \n",keycode);
    _delaySomeNop(1000);
    MDrv_FD650_Stop();

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
    printf("SetDigital inside==========================================\n");
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

//-------------------------------------------------------------------------------------------------
/// 650Get Key
/// @return None
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_FP_GetKey(void)
{
    MS_U8 KeyValue = 0;
    return KeyValue;
}

void MDrv_FP_DispUpdate(void)
{
	  
    int i;
    int data[4] = {0x00, 0x00, 0x00, 0x00};
    printf("DispUpdate inside==================================================\n");
    if (0 == num)
        return ;

    if (num > 4)
        num = 4;

    for (i = 0; i < num; i++)
    {
        data[i] = MDrv_Led_Get_Code(gDispBuf[i]);
    }

    MDrv_FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式

    //发显示数据
    MDrv_FD650_Write(FD650_DIG0 | (MS_U8)data[0]);   //点亮第一个数码管

    if (sec_flag)
        MDrv_FD650_Write(FD650_DIG1 | (MS_U8)data[1] | FD650_DOT);   //点亮第二个数码管
    else
        MDrv_FD650_Write(FD650_DIG1 | (MS_U8)data[1]);

    MDrv_FD650_Write(FD650_DIG2 | (MS_U8)data[2]);  //点亮第三个数码管
    MDrv_FD650_Write(FD650_DIG3 | (MS_U8)data[3]);   //点亮第四个数码管
}

//-------------------------------------------------------------------------------------------------
/// Initialize 650
/// @return TRUE  - Success
///         FALSE - Failure
//-------------------------------------------------------------------------------------------------
void MDrv_FP_Init(void)
{
	  
	  printf("hello MDrv_FP_Init8||||||||||||||||||||||||||||||||||||||||||\n");
	  FD650_GPIO_INIT
	  MDrv_FrontPnl_EnableLED(TRUE);
	  MDrv_FD650_Write(FD650_SYSON_8);
    
    
}
#endif

