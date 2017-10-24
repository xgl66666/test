#ifndef TEST_H
#define TEST_H
#include "drvGPIO.h"
#include "Board.h"
/****************************begin for FD650 define struct***************************** */
#define LEDMAPNUM 35
/* LED bitmap */
typedef struct _led_bitmap
{
    MS_U8 character;
    MS_U8 bitmap;
} led_bitmap;


extern const led_bitmap BCD_decode_tab[LEDMAPNUM];


/****************************end for FD650 define struct***************************** */


/* **************************************function dec*********************************************** */
void Delay1(void);
/* *************************************hardware related*********************************************** */
#define     DELAY1    {Delay1();}

#define FD650_SCL        5  // gpio4    //define IO port
#define FD650_SDA        9  // gpio8   //define IO port


#define HIGH 1
#define LOW  0

/* 接口的PIO操作，与平台IO操作有关 */
#define     FD650_SCL_SET       {mdrv_gpio_set_low(FD650_SCL);}   //将SCL设置为高电平
#define     FD650_SCL_CLR       {mdrv_gpio_set_high(FD650_SCL);}   //将SCL设置为低电平
#define     FD650_SCL_D_OUT {}  // 设置SCL为输出方向,对于双向I/O需切换为输出
#define     FD650_SDA_SET       {mdrv_gpio_set_high(FD650_SDA);}   //将SDA设置为高电平
#define     FD650_SDA_CLR       {mdrv_gpio_set_low(FD650_SDA);}   //将SDA设置为低电平
#define     FD650_SDA_IN        mdrv_gpio_get_level(FD650_SDA)  //当SDA设为输入方向时，读取的电平值
#define     FD650_SDA_D_OUT {}  // 设置SDA为输出方向,对于双向I/O需切换为输出
#define     FD650_SDA_D_IN      {mdrv_gpio_set_input(FD650_SDA);}   // 设置SDA为输入方向,对于双向I/O需切换为输入
/* **************************************硬件相关*********************************************** */

/* ********************************************************************************************* */
// 设置系统参数命令

#define FD650_BIT_ENABLE    0x01        // 开启/关闭位
#define FD650_BIT_SLEEP     0x04        // 睡眠控制位
#define FD650_BIT_7SEG      0x08        // 7段控制位
#define FD650_BIT_INTENS1   0x10        // 1级亮度
#define FD650_BIT_INTENS2   0x20        // 2级亮度
#define FD650_BIT_INTENS3   0x30        // 3级亮度
#define FD650_BIT_INTENS4   0x40        // 4级亮度
#define FD650_BIT_INTENS5   0x50        // 5级亮度
#define FD650_BIT_INTENS6   0x60        // 6级亮度
#define FD650_BIT_INTENS7   0x70        // 7级亮度
#define FD650_BIT_INTENS8   0x00        // 8级亮度

#define FD650_SYSOFF    0x0400          // 关闭显示、关闭键盘
#define FD650_SYSON     ( FD650_SYSOFF | FD650_BIT_ENABLE ) // 开启显示、键盘
#define FD650_SLEEPOFF  FD650_SYSOFF    // 关闭睡眠
#define FD650_SLEEPON   ( FD650_SYSOFF | FD650_BIT_SLEEP )  // 开启睡眠
#define FD650_7SEG_ON   ( FD650_SYSON | FD650_BIT_7SEG )    // 开启七段模式
#define FD650_8SEG_ON   ( FD650_SYSON | 0x00 )  // 开启八段模式
#define FD650_SYSON_1   ( FD650_SYSON | FD650_BIT_INTENS1 ) // 开启显示、键盘、1级亮度
//以此类推
#define FD650_SYSON_4   ( FD650_SYSON | FD650_BIT_INTENS4 ) // 开启显示、键盘、4级亮度
//以此类推
#define FD650_SYSON_8   ( FD650_SYSON | FD650_BIT_INTENS8 ) // 开启显示、键盘、8级亮度


// 加载字数据命令
#define FD650_DIG0      0x1400          // 数码管位0显示,需另加8位数据
#define FD650_DIG1      0x1500          // 数码管位1显示,需另加8位数据
#define FD650_DIG2      0x1600          // 数码管位2显示,需另加8位数据
#define FD650_DIG3      0x1700          // 数码管位3显示,需另加8位数据

#define FD650_DOT       0x0080          // 数码管小数点显示

// 读取按键代码命令
#define FD650_GET_KEY   0x0700                  // 获取按键,返回按键代码

MS_BOOL MDrv_FD650_Init(void);
void    MDrv_FD650_Display(char *FPStr, MS_U8 sec_flag);
MS_U8   MDrv_FD650_GetKeyValue(void);
MS_BOOL MDrv_FD650_Term(void);
MS_BOOL MDrv_FD650_Input_Key_Convert(MS_U8 u8key, MS_U32 *key);
#endif
