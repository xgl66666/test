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

/* �ӿڵ�PIO��������ƽ̨IO�����й� */
#define     FD650_SCL_SET       {mdrv_gpio_set_low(FD650_SCL);}   //��SCL����Ϊ�ߵ�ƽ
#define     FD650_SCL_CLR       {mdrv_gpio_set_high(FD650_SCL);}   //��SCL����Ϊ�͵�ƽ
#define     FD650_SCL_D_OUT {}  // ����SCLΪ�������,����˫��I/O���л�Ϊ���
#define     FD650_SDA_SET       {mdrv_gpio_set_high(FD650_SDA);}   //��SDA����Ϊ�ߵ�ƽ
#define     FD650_SDA_CLR       {mdrv_gpio_set_low(FD650_SDA);}   //��SDA����Ϊ�͵�ƽ
#define     FD650_SDA_IN        mdrv_gpio_get_level(FD650_SDA)  //��SDA��Ϊ���뷽��ʱ����ȡ�ĵ�ƽֵ
#define     FD650_SDA_D_OUT {}  // ����SDAΪ�������,����˫��I/O���л�Ϊ���
#define     FD650_SDA_D_IN      {mdrv_gpio_set_input(FD650_SDA);}   // ����SDAΪ���뷽��,����˫��I/O���л�Ϊ����
/* **************************************Ӳ�����*********************************************** */

/* ********************************************************************************************* */
// ����ϵͳ��������

#define FD650_BIT_ENABLE    0x01        // ����/�ر�λ
#define FD650_BIT_SLEEP     0x04        // ˯�߿���λ
#define FD650_BIT_7SEG      0x08        // 7�ο���λ
#define FD650_BIT_INTENS1   0x10        // 1������
#define FD650_BIT_INTENS2   0x20        // 2������
#define FD650_BIT_INTENS3   0x30        // 3������
#define FD650_BIT_INTENS4   0x40        // 4������
#define FD650_BIT_INTENS5   0x50        // 5������
#define FD650_BIT_INTENS6   0x60        // 6������
#define FD650_BIT_INTENS7   0x70        // 7������
#define FD650_BIT_INTENS8   0x00        // 8������

#define FD650_SYSOFF    0x0400          // �ر���ʾ���رռ���
#define FD650_SYSON     ( FD650_SYSOFF | FD650_BIT_ENABLE ) // ������ʾ������
#define FD650_SLEEPOFF  FD650_SYSOFF    // �ر�˯��
#define FD650_SLEEPON   ( FD650_SYSOFF | FD650_BIT_SLEEP )  // ����˯��
#define FD650_7SEG_ON   ( FD650_SYSON | FD650_BIT_7SEG )    // �����߶�ģʽ
#define FD650_8SEG_ON   ( FD650_SYSON | 0x00 )  // �����˶�ģʽ
#define FD650_SYSON_1   ( FD650_SYSON | FD650_BIT_INTENS1 ) // ������ʾ�����̡�1������
//�Դ�����
#define FD650_SYSON_4   ( FD650_SYSON | FD650_BIT_INTENS4 ) // ������ʾ�����̡�4������
//�Դ�����
#define FD650_SYSON_8   ( FD650_SYSON | FD650_BIT_INTENS8 ) // ������ʾ�����̡�8������


// ��������������
#define FD650_DIG0      0x1400          // �����λ0��ʾ,�����8λ����
#define FD650_DIG1      0x1500          // �����λ1��ʾ,�����8λ����
#define FD650_DIG2      0x1600          // �����λ2��ʾ,�����8λ����
#define FD650_DIG3      0x1700          // �����λ3��ʾ,�����8λ����

#define FD650_DOT       0x0080          // �����С������ʾ

// ��ȡ������������
#define FD650_GET_KEY   0x0700                  // ��ȡ����,���ذ�������

MS_BOOL MDrv_FD650_Init(void);
void    MDrv_FD650_Display(char *FPStr, MS_U8 sec_flag);
MS_U8   MDrv_FD650_GetKeyValue(void);
MS_BOOL MDrv_FD650_Term(void);
MS_BOOL MDrv_FD650_Input_Key_Convert(MS_U8 u8key, MS_U32 *key);
#endif
