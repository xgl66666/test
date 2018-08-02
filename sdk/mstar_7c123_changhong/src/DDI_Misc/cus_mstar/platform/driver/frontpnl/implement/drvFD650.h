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

/* ½Ó¿ÚµÄPIO²Ù×÷£¬ÓëÆ½Ì¨IO²Ù×÷ÓÐ¹Ø */
#define     FD650_SCL_SET       {mdrv_gpio_set_low(FD650_SCL);}   //½«SCLÉèÖÃÎª¸ßµçÆ½
#define     FD650_SCL_CLR       {mdrv_gpio_set_high(FD650_SCL);}   //½«SCLÉèÖÃÎªµÍµçÆ½
#define     FD650_SCL_D_OUT {}  // ÉèÖÃSCLÎªÊä³ö·½Ïò,¶ÔÓÚË«ÏòI/OÐèÇÐ»»ÎªÊä³ö
#define     FD650_SDA_SET       {mdrv_gpio_set_high(FD650_SDA);}   //½«SDAÉèÖÃÎª¸ßµçÆ½
#define     FD650_SDA_CLR       {mdrv_gpio_set_low(FD650_SDA);}   //½«SDAÉèÖÃÎªµÍµçÆ½
#define     FD650_SDA_IN        mdrv_gpio_get_level(FD650_SDA)  //µ±SDAÉèÎªÊäÈë·½ÏòÊ±£¬¶ÁÈ¡µÄµçÆ½Öµ
#define     FD650_SDA_D_OUT {}  // ÉèÖÃSDAÎªÊä³ö·½Ïò,¶ÔÓÚË«ÏòI/OÐèÇÐ»»ÎªÊä³ö
#define     FD650_SDA_D_IN      {mdrv_gpio_set_input(FD650_SDA);}   // ÉèÖÃSDAÎªÊäÈë·½Ïò,¶ÔÓÚË«ÏòI/OÐèÇÐ»»ÎªÊäÈë
/* **************************************Ó²¼þÏà¹Ø*********************************************** */

/* ********************************************************************************************* */
// ÉèÖÃÏµÍ³²ÎÊýÃüÁî

#define FD650_BIT_ENABLE    0x01        // ¿ªÆô/¹Ø±ÕÎ»
#define FD650_BIT_SLEEP     0x04        // Ë¯Ãß¿ØÖÆÎ»
#define FD650_BIT_7SEG      0x08        // 7¶Î¿ØÖÆÎ»
#define FD650_BIT_INTENS1   0x10        // 1¼¶ÁÁ¶È
#define FD650_BIT_INTENS2   0x20        // 2¼¶ÁÁ¶È
#define FD650_BIT_INTENS3   0x30        // 3¼¶ÁÁ¶È
#define FD650_BIT_INTENS4   0x40        // 4¼¶ÁÁ¶È
#define FD650_BIT_INTENS5   0x50        // 5¼¶ÁÁ¶È
#define FD650_BIT_INTENS6   0x60        // 6¼¶ÁÁ¶È
#define FD650_BIT_INTENS7   0x70        // 7¼¶ÁÁ¶È
#define FD650_BIT_INTENS8   0x00        // 8¼¶ÁÁ¶È

#define FD650_SYSOFF    0x0400          // ¹Ø±ÕÏÔÊ¾¡¢¹Ø±Õ¼üÅÌ
#define FD650_SYSON     ( FD650_SYSOFF | FD650_BIT_ENABLE ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ
#define FD650_SLEEPOFF  FD650_SYSOFF    // ¹Ø±ÕË¯Ãß
#define FD650_SLEEPON   ( FD650_SYSOFF | FD650_BIT_SLEEP )  // ¿ªÆôË¯Ãß
#define FD650_7SEG_ON   ( FD650_SYSON | FD650_BIT_7SEG )    // ¿ªÆôÆß¶ÎÄ£Ê½
#define FD650_8SEG_ON   ( FD650_SYSON | 0x00 )  // ¿ªÆô°Ë¶ÎÄ£Ê½
#define FD650_SYSON_1   ( FD650_SYSON | FD650_BIT_INTENS1 ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ¡¢1¼¶ÁÁ¶È
//ÒÔ´ËÀàÍÆ
#define FD650_SYSON_4   ( FD650_SYSON | FD650_BIT_INTENS4 ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ¡¢4¼¶ÁÁ¶È
//ÒÔ´ËÀàÍÆ
#define FD650_SYSON_8   ( FD650_SYSON | FD650_BIT_INTENS8 ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ¡¢8¼¶ÁÁ¶È


// ¼ÓÔØ×ÖÊý¾ÝÃüÁî
#define FD650_DIG0      0x1400          // ÊýÂë¹ÜÎ»0ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý
#define FD650_DIG1      0x1500          // ÊýÂë¹ÜÎ»1ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý
#define FD650_DIG2      0x1600          // ÊýÂë¹ÜÎ»2ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý
#define FD650_DIG3      0x1700          // ÊýÂë¹ÜÎ»3ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý

#define FD650_DOT       0x0080          // ÊýÂë¹ÜÐ¡ÊýµãÏÔÊ¾

// ¶ÁÈ¡°´¼ü´úÂëÃüÁî
#define FD650_GET_KEY   0x0700                  // »ñÈ¡°´¼ü,·µ»Ø°´¼ü´úÂë

MS_BOOL MDrv_FD650_Init(void);
void    MDrv_FD650_Display(char *FPStr, MS_U8 sec_flag);
MS_U8   MDrv_FD650_GetKeyValue(void);
MS_BOOL MDrv_FD650_Term(void);
MS_BOOL MDrv_FD650_Input_Key_Convert(MS_U8 u8key, MS_U32 *key);
#endif
