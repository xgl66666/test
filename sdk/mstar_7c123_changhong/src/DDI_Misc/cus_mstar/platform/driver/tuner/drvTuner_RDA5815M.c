#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#else
#include <stdio.h>
#endif
#include "error.h"
#include "MsCommon.h"
#include "Board.h"
#include "apiDigiTuner.h"
#include "drvIIC.h"
#include "drvTuner_RDA5815M.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_RDA5815M)

#define RDA5815M_SLA            0x18
#define RDA5815Sleep(x)         MsOS_DelayTask(x)
#define Xtal_27M                1
#define Xtal_30M                0
#define Xtal_24M                0



static MS_U32 u32Freq = 950;
typedef int INT32;

static  MS_BOOL  _DigiTuner_Decide_LNB_LO(TUNER_MS_SAT_PARAM *pSATParam)
{
     MS_S32 s32Freq;
     // calc Mid Freq & LNB control(22K on/off)
     if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
     {
        *pSATParam->pbIsHiLOF= FALSE;
     }
     else // 2LOF
     {
         MS_U32 u32MidFreq;
         MS_S32 s32Offset = 0;
         s32Freq  = pSATParam->u32RF_FREQ;
         u32MidFreq =  abs(s32Freq - pSATParam->u16LoLOF) ;
         s32Offset = pSATParam->u16LoLOF+ MAX_INPUT_FREQ - pSATParam->u16HiLOF - MIN_INPUT_FREQ;
         if(s32Offset < 0)
              s32Offset = 0;
          else
             s32Offset /= 2;

         if( u32MidFreq <= (MAX_INPUT_FREQ-s32Offset))
              *pSATParam->pbIsHiLOF = FALSE;
          else
              *pSATParam->pbIsHiLOF = TRUE;
     }
     return TRUE;
}

static MS_U8 RDA5815mReadReg(MS_U8 u8TunerIndex, MS_U8 u8addr)
{
    MS_U8 u8data = 0;
    MS_IIC_PORT ePort;

    ePort = getI2CPort(u8TunerIndex);

	MDrv_IIC_ReadBytes(ePort, (MS_U16)RDA5815M_SLA, 1, &u8addr, 1, &u8data);

    return u8data;
}

static MS_BOOL RDA5815WriteReg(MS_U8 u8TunerIndex, MS_U8 paddr, MS_U8 pu8data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Value[2];
    MS_IIC_PORT ePort;

    u8Value[0] = paddr;
    u8Value[1] = pu8data;

    ePort = getI2CPort(u8TunerIndex);
    bRet &= MDrv_IIC_WriteBytes(ePort, (MS_U16)RDA5815M_SLA, 0, NULL, 2, u8Value);

    if (bRet == FALSE)
    {
        TUNER_ERR((" ERROR in RDA5815WriteReg !!! \n"));
    }
    return bRet;
}

int  MDrv_RDA5815M_RSSI(MS_U8 u8TunerIndex)
{

   unsigned char buffer;
    /* Gain stage limits                           st1  pre st2    post i2v fil  */
#define RDA5815m_Gain_Stage__0 0xc00 /* '11   00    00    00     00     00    ' */
#define RDA5815m_Gain_Stage__1 0xc00 /* '11   00    00    00     00     00 ' */
#define RDA5815m_Gain_Stage__2 0xc01 /* '11   00    00    00     00     01 ' */
#define RDA5815m_Gain_Stage__3 0xc02 /* '11   00    00    00     00     10 ' */
#define RDA5815m_Gain_Stage__4 0xc03 /* '11   00    00    00     00     11 ' */
#define RDA5815m_Gain_Stage__5 0xc13 /* '11   00    00    01     00     11 ' */
#define RDA5815m_Gain_Stage__6 0xd17 /* '11   01    00    01     01     11 ' */
#define RDA5815m_Gain_Stage__7 0xd5b /* '11   01    01    01     10     11 ' */
#define RDA5815m_Gain_Stage__8 0xe5b /* '11   10    01    01     10     11 ' */
#define RDA5815m_Gain_Stage__9 0xf9b /* '11   11    10    01     10     11 ' */
#define RDA5815m_Gain_Stage_10 0xfab /* '11   11    10    10     10     11 ' */
#define RDA5815m_Gain_Stage_11 0xfaf /* '11   11    10    10     11     11 ' */
#define RDA5815m_Gain_Stage_12 0xfef /* '11   11    11    10     11     11 ' */
#define RDA5815m_Gain_Stage_13 0xfff /* '11   11    11    11     11     11 ' */
#define RDA5815m_Gain_Stage_14 0xfff /* '11   11    11    11     11     11 ' */
#define RDA5815m_Gain_Stage_15 0xfff /* '11   11    11    11     11     11 ' */

  unsigned char data16,data17,st1,pre,st2,post,i2v,filter,vga;
  unsigned int  stage_code;
  unsigned char gain_stage;
  unsigned char band,offset;
  double vga_gain, total_gain;


 double gain[13][16] = {    
                                         { -7.6,   -7.7,  -1.7,  4.3,  10.2,  15.8,  25.4, 34.2, 40.7, 49.4, 55.2, 61.4, 65.5, 71.6, 71.6, 71.6},  /* 1  950MHz<=Freq<1000MHz */
                                         { -7.2,   -7.2,  -1.2,  4.8,  10.8,  16.4,  25.9, 34.2, 40.8, 49.3, 55.3, 61.4, 65.5, 71.8, 71.8, 71.8},  /* 2  1000MHz<=Freq<1100MHz */
                                         { -6.5,   -6.6,  -0.5,  5.5,  11.5 , 17.1,  26.5, 34.8, 41.1, 49.2, 55.1, 61.1, 65.4, 71.6, 71.6, 71.6},  /* 3  1100MHz<=Freq<1200MHz */
                                         { -5.9,   -5.8,   0  ,  6.1,  12.1 , 17.7,  27.0, 35.3, 41.4, 49.0, 54.9, 61.0, 65.1, 71.5, 71.5, 71.5},  /* 4  1200MHz<=Freq<1300MHz */
                                         { -5.8,   -5.8,  0.2,   6.2,  12.2 , 17.9,  27.1, 35.5, 40.9, 48.4, 54.3, 60.4, 64.5, 71.0, 71.0, 71.0},  /* 5  1300MHz<=Freq<1400MHz */
                                         { -5.7,   -6.3,  0.4,   6.4,  12.4 , 18.1,  27.2, 35.3, 41.3, 48.0, 53.9, 60.1, 64.1, 70.7, 70.7, 70.7},  /* 6  1400MHz<=Freq<1500MHz */
                                         { -5.0,   -6.3,  1.0,   7.1,  13.0 , 18.7,  27.8, 36.0, 41.2, 48.0, 54.1, 60.2, 64.2, 71.8, 70.8, 71.8},  /* 7  1500MHz<=Freq<1600MHz */
                                         { -4.2,   -5.7,  1.7,   7.7,  13.8 , 19.4,  28.4, 36.8, 41.7, 47.9, 54.2, 60.2, 64.1, 70.9, 70.9, 70.9},  /* 8  1600MHz<=Freq<1700MHz */
                                         { -4.2,   -6.0,  1.8,   7.8,  13.8 , 19.5,  28.3, 36.4, 41.3, 47.4, 53.6, 59.8, 63.4, 70.3, 70.3, 70.3},  /* 9  1700MHz<=Freq<1800MHz */
                                         { -4.4,   -5.2,  1.5,   7.5,  13.5 , 19.2,  27.9, 36.5, 40.6, 46.6, 53.0, 59.1, 62.5, 69.6, 69.6, 69.6},  /* 10  1800MHz<=Freq<1900MHz */
                                         { -4.3,   -6.1,  1.7,   7.6,  13.7 , 19.4,  28.0, 35.6, 40.3, 46.4, 52.7, 58.9, 62.2, 69.2, 69.2, 69.2},  /* 11  1900MHz<=Freq<2000MHz */
                                         { -4.2,   -4.6,  1.9,   7.8,  13.8 , 19.6,  28.0, 36.2, 40.4, 46.0, 52.6, 58.6, 61.7, 68.7, 68.7, 68.7},  /* 12  2000MHz<=Freq<2100MHz */
                                         { -4.6,   -5.3,  1.4,   7.4,  13.4 , 19.2,  27.5, 35.6, 39.5, 45.2, 51.7, 57.8, 60.6, 67.7, 67.6, 67.7}   /* 13  2100MHz<=Freq<=2150MHz */
                        };


  double gain_offset[] = {3,3,3,2,1,1,2,2,1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0};
  data16 = RDA5815mReadReg(u8TunerIndex, 0x16);
  i2v    = (data16&0xc0)>>6;
  filter = (data16&0x30)>>4;
  st1    = (data16&0x0c)>>2;
  st2    = (data16&0x03)>>0;
  //printf("\033[34m\033[1m data16=0x%x\033[0m\n",data16);

  data17 = RDA5815mReadReg(u8TunerIndex, 0x17);
  pre    = (data17&0x0c)>>2;
  post   = (data17&0x03)>>0;
 // printf("\033[34m\033[1m data17=0x%x\033[0m\n",data17);

  stage_code = (st1<<10) + (pre<<8) + (st2<<6) + (post<<4) + (i2v<<2) + (filter<<0);

       if (stage_code == RDA5815m_Gain_Stage_13 ) { gain_stage = 13; }
  else if (stage_code == RDA5815m_Gain_Stage_12 ) { gain_stage = 12; }
  else if (stage_code == RDA5815m_Gain_Stage_11 ) { gain_stage = 11; }
  else if (stage_code == RDA5815m_Gain_Stage_10 ) { gain_stage = 10; }
  else if (stage_code == RDA5815m_Gain_Stage__9 ) { gain_stage =  9; }
  else if (stage_code == RDA5815m_Gain_Stage__8 ) { gain_stage =  8; }
  else if (stage_code == RDA5815m_Gain_Stage__7 ) { gain_stage =  7; }
  else if (stage_code == RDA5815m_Gain_Stage__6 ) { gain_stage =  6; }
  else if (stage_code == RDA5815m_Gain_Stage__5 ) { gain_stage =  5; }
  else if (stage_code == RDA5815m_Gain_Stage__4 ) { gain_stage =  4; }
  else if (stage_code == RDA5815m_Gain_Stage__3 ) { gain_stage =  3; }
  else if (stage_code == RDA5815m_Gain_Stage__2 ) { gain_stage =  2; }
  else if (stage_code == RDA5815m_Gain_Stage__1 ) { gain_stage =  1; }
  else  { gain_stage =  0; }

                                                            
  if ( u32Freq < 950 )           return 1;
    else if ( u32Freq < 1000 )             {band =0;}
    else if ( u32Freq < 1100 )             {band =1;}
    else if ( u32Freq < 1200 )             {band =2;}
    else if ( u32Freq < 1300 )             {band =3;}
    else if ( u32Freq < 1400 )             {band =4;}
    else if ( u32Freq < 1500 )             {band =5;}
    else if ( u32Freq < 1600 )             {band =6;}
    else if ( u32Freq < 1700 )             {band =7;}
    else if ( u32Freq < 1800 )             {band =8;}
    else if ( u32Freq < 1900 )             {band =9;}
    else if ( u32Freq < 2000 )             {band =10;}
    else if ( u32Freq < 2100 )             {band =11;}
    else if ( u32Freq <= 2150 )            {band =12;}
    else                                        return 1;

   if ( u32Freq < 950 )   return 1;
   else if ( u32Freq < 1000 ) {offset =0;}
   else if ( u32Freq < 1050 ) {offset =1;}
   else if ( u32Freq < 1100 ) {offset =2;}
   else if ( u32Freq < 1150 ) {offset =3;}
   else if ( u32Freq < 1200 ) {offset =4;}
   else if ( u32Freq < 1250 ) {offset =5;}
   else if ( u32Freq < 1300 ) {offset =6;}
   else if ( u32Freq < 1350 ) {offset =7;}
   else if ( u32Freq < 1400 ) {offset =8;}
   else if ( u32Freq < 1450 ) {offset =9;}
   else if ( u32Freq < 1500 ) {offset =10;}
   else if ( u32Freq < 1550 ) {offset =11;}
   else if ( u32Freq < 1600 ) {offset =12;}
   else if ( u32Freq < 1650 ) {offset =13;}
   else if ( u32Freq < 1700 ) {offset =14;}
   else if ( u32Freq < 1750 ) {offset =15;}
   else if ( u32Freq < 1800 ) {offset =16;}
   else if ( u32Freq < 1850 ) {offset =17;}
   else if ( u32Freq < 1900 ) {offset =18;}
   else if ( u32Freq < 1950 ) {offset =19;}
   else if ( u32Freq < 2000 ) {offset =20;}
   else if ( u32Freq < 2050 ) {offset =21;}
   else if ( u32Freq < 2100 ) {offset =22;}
   else if ( u32Freq <= 2150) {offset =23;}
   else return 1;


  
   buffer = RDA5815mReadReg(u8TunerIndex, 0xB7);
   //printf("\033[34m\033[1m buffer=0x%x\033[0m\n",buffer);
   vga =buffer;
  
  vga_gain = vga*30.0/255;

  total_gain = gain[band][gain_stage] + vga_gain - gain_offset[offset];

  if (1) {
   
    TUNER_DBG(("RDA5815m RSSI = gain[%d][%d] + vga_gain+ gain_offset = %f + %f - %f = %f\n", band, gain_stage, gain[band][gain_stage], vga_gain, gain_offset[offset], total_gain));
  }

  return  -total_gain;
}


void RDA5815Initial(MS_U8 u8TunerIndex)
{
//RDA5815WriteReg(register address,register data);

    RDA5815Sleep(1);//Wait 1ms.
    // Chip register soft reset
    RDA5815WriteReg(u8TunerIndex,0x04, 0x04);
    RDA5815WriteReg(u8TunerIndex,0x04, 0x05);

    // Initial configuration start

    //pll setting
    RDA5815WriteReg(u8TunerIndex,0x1a, 0x13);
    RDA5815WriteReg(u8TunerIndex,0x41, 0x53);
    RDA5815WriteReg(u8TunerIndex,0x38, 0x9B);
    RDA5815WriteReg(u8TunerIndex,0x39, 0x15);
    RDA5815WriteReg(u8TunerIndex,0x3A, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x3B, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x3C, 0x0c);
    RDA5815WriteReg(u8TunerIndex,0x0c, 0xE2);
    RDA5815WriteReg(u8TunerIndex,0x2e, 0x6F);

#if (Xtal_27M == 1)
    RDA5815WriteReg(u8TunerIndex,0x72, 0x07); // v1.1, 1538~1539
    RDA5815WriteReg(u8TunerIndex,0x73, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x74, 0x71);
    RDA5815WriteReg(u8TunerIndex,0x75, 0x06); // v1.1, 1363~1364, 1862~1863
    RDA5815WriteReg(u8TunerIndex,0x76, 0x40);
    RDA5815WriteReg(u8TunerIndex,0x77, 0x89);
    RDA5815WriteReg(u8TunerIndex,0x79, 0x04); // v1.1, 900
    RDA5815WriteReg(u8TunerIndex,0x7A, 0x2A);
    RDA5815WriteReg(u8TunerIndex,0x7B, 0xAA);
    RDA5815WriteReg(u8TunerIndex,0x7C, 0xAB);
#endif
#if (Xtal_30M == 1)
    RDA5815WriteReg(u8TunerIndex,0x72, 0x06); // v1.2, 1544~1545
    RDA5815WriteReg(u8TunerIndex,0x73, 0x60);
    RDA5815WriteReg(u8TunerIndex,0x74, 0x66);
    RDA5815WriteReg(u8TunerIndex,0x75, 0x05); // v1.2, 1364~1365, 1859~1860
    RDA5815WriteReg(u8TunerIndex,0x76, 0xA0);
    RDA5815WriteReg(u8TunerIndex,0x77, 0x7B);
    RDA5815WriteReg(u8TunerIndex,0x79, 0x03); // v1.2, 901
    RDA5815WriteReg(u8TunerIndex,0x7A, 0xC0);
    RDA5815WriteReg(u8TunerIndex,0x7B, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x7C, 0x00);
#endif
#if (Xtal_24M == 1)
    RDA5815WriteReg(u8TunerIndex,0x72, 0x08); // v1.3, 1547~1548
    RDA5815WriteReg(u8TunerIndex,0x73, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x74, 0x80);
    RDA5815WriteReg(u8TunerIndex,0x75, 0x07); // v1.3, 1367~1368, 1859~1860
    RDA5815WriteReg(u8TunerIndex,0x76, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x77, 0x9A);
    RDA5815WriteReg(u8TunerIndex,0x79, 0x04); // v1.3, 901
    RDA5815WriteReg(u8TunerIndex,0x7A, 0xB0);
    RDA5815WriteReg(u8TunerIndex,0x7B, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x7C, 0x00);
#endif

    RDA5815WriteReg(u8TunerIndex,0x2f, 0x57);
    RDA5815WriteReg(u8TunerIndex,0x0d, 0x70);
    RDA5815WriteReg(u8TunerIndex,0x18, 0x4B);
    RDA5815WriteReg(u8TunerIndex,0x30, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0x5c, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0x65, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x70, 0x3F);
    RDA5815WriteReg(u8TunerIndex,0x71, 0x3F);
    RDA5815WriteReg(u8TunerIndex,0x53, 0xA8);
    RDA5815WriteReg(u8TunerIndex,0x46, 0x21);
    RDA5815WriteReg(u8TunerIndex,0x47, 0x84);
    RDA5815WriteReg(u8TunerIndex,0x48, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x49, 0x08);
    RDA5815WriteReg(u8TunerIndex,0x60, 0x80);
    RDA5815WriteReg(u8TunerIndex,0x61, 0x80);
    RDA5815WriteReg(u8TunerIndex,0x6A, 0x08);
    RDA5815WriteReg(u8TunerIndex,0x6B, 0x63);
    RDA5815WriteReg(u8TunerIndex,0x69, 0xF8);
    RDA5815WriteReg(u8TunerIndex,0x57, 0x64);
    RDA5815WriteReg(u8TunerIndex,0x05, 0xaa);
    RDA5815WriteReg(u8TunerIndex,0x06, 0xaa);
    RDA5815WriteReg(u8TunerIndex,0x15, 0xAE);
    RDA5815WriteReg(u8TunerIndex,0x4a, 0x67);
    RDA5815WriteReg(u8TunerIndex,0x4b, 0x77);

    //agc setting

    RDA5815WriteReg(u8TunerIndex,0x4f, 0x40);
    RDA5815WriteReg(u8TunerIndex,0x5b, 0x20);

    RDA5815WriteReg(u8TunerIndex,0x16, 0x0C);
    RDA5815WriteReg(u8TunerIndex,0x18, 0x0C);
    RDA5815WriteReg(u8TunerIndex,0x30, 0x1C);
    RDA5815WriteReg(u8TunerIndex,0x5c, 0x2C);
    RDA5815WriteReg(u8TunerIndex,0x6c, 0x3C);
    RDA5815WriteReg(u8TunerIndex,0x6e, 0x3C);
    RDA5815WriteReg(u8TunerIndex,0x1b, 0x7C);
    RDA5815WriteReg(u8TunerIndex,0x1d, 0xBD);
    RDA5815WriteReg(u8TunerIndex,0x1f, 0xBD);
    RDA5815WriteReg(u8TunerIndex,0x21, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x23, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x25, 0xFE);
    RDA5815WriteReg(u8TunerIndex,0x27, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0x29, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0xb3, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0xb5, 0xFF);

    RDA5815WriteReg(u8TunerIndex,0x17, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x19, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x31, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x5d, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x6d, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x6f, 0xF1);
    RDA5815WriteReg(u8TunerIndex,0x1c, 0xF5);
    RDA5815WriteReg(u8TunerIndex,0x1e, 0x35);
    RDA5815WriteReg(u8TunerIndex,0x20, 0x79);
    RDA5815WriteReg(u8TunerIndex,0x22, 0x9D);
    RDA5815WriteReg(u8TunerIndex,0x24, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x26, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x28, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x2a, 0xCF);
    RDA5815WriteReg(u8TunerIndex,0xb4, 0xDF);
    RDA5815WriteReg(u8TunerIndex,0xb6, 0x0F);

    RDA5815WriteReg(u8TunerIndex,0xb7, 0x15); //start
    RDA5815WriteReg(u8TunerIndex,0xb9, 0x6c);
    RDA5815WriteReg(u8TunerIndex,0xbb, 0x63);
    RDA5815WriteReg(u8TunerIndex,0xbd, 0x5a);
    RDA5815WriteReg(u8TunerIndex,0xbf, 0x5a);
    RDA5815WriteReg(u8TunerIndex,0xc1, 0x55);
    RDA5815WriteReg(u8TunerIndex,0xc3, 0x55);
    RDA5815WriteReg(u8TunerIndex,0xc5, 0x47);
    RDA5815WriteReg(u8TunerIndex,0xa3, 0x53);
    RDA5815WriteReg(u8TunerIndex,0xa5, 0x4f);
    RDA5815WriteReg(u8TunerIndex,0xa7, 0x4e);
    RDA5815WriteReg(u8TunerIndex,0xa9, 0x4e);
    RDA5815WriteReg(u8TunerIndex,0xab, 0x54);
    RDA5815WriteReg(u8TunerIndex,0xad, 0x31);
    RDA5815WriteReg(u8TunerIndex,0xaf, 0x43);
    RDA5815WriteReg(u8TunerIndex,0xb1, 0x9f);

    RDA5815WriteReg(u8TunerIndex,0xb8, 0x6c); //end
    RDA5815WriteReg(u8TunerIndex,0xba, 0x92);
    RDA5815WriteReg(u8TunerIndex,0xbc, 0x8a);
    RDA5815WriteReg(u8TunerIndex,0xbe, 0x8a);
    RDA5815WriteReg(u8TunerIndex,0xc0, 0x82);
    RDA5815WriteReg(u8TunerIndex,0xc2, 0x93);
    RDA5815WriteReg(u8TunerIndex,0xc4, 0x85);
    RDA5815WriteReg(u8TunerIndex,0xc6, 0x77);
    RDA5815WriteReg(u8TunerIndex,0xa4, 0x82);
    RDA5815WriteReg(u8TunerIndex,0xa6, 0x7e);
    RDA5815WriteReg(u8TunerIndex,0xa8, 0x7d);
    RDA5815WriteReg(u8TunerIndex,0xaa, 0x6f);
    RDA5815WriteReg(u8TunerIndex,0xac, 0x65);
    RDA5815WriteReg(u8TunerIndex,0xae, 0x43);
    RDA5815WriteReg(u8TunerIndex,0xb0, 0x9f);
    RDA5815WriteReg(u8TunerIndex,0xb2, 0xf0);

    RDA5815WriteReg(u8TunerIndex,0x81, 0x92); //rise
    RDA5815WriteReg(u8TunerIndex,0x82, 0xb4);
    RDA5815WriteReg(u8TunerIndex,0x83, 0xb3);
    RDA5815WriteReg(u8TunerIndex,0x84, 0xac);
    RDA5815WriteReg(u8TunerIndex,0x85, 0xba);
    RDA5815WriteReg(u8TunerIndex,0x86, 0xbc);
    RDA5815WriteReg(u8TunerIndex,0x87, 0xaf);
    RDA5815WriteReg(u8TunerIndex,0x88, 0xa2);
    RDA5815WriteReg(u8TunerIndex,0x89, 0xac);
    RDA5815WriteReg(u8TunerIndex,0x8a, 0xa9);
    RDA5815WriteReg(u8TunerIndex,0x8b, 0x9b);
    RDA5815WriteReg(u8TunerIndex,0x8c, 0x7d);
    RDA5815WriteReg(u8TunerIndex,0x8d, 0x74);
    RDA5815WriteReg(u8TunerIndex,0x8e, 0x9f);
    RDA5815WriteReg(u8TunerIndex,0x8f, 0xf0);

    RDA5815WriteReg(u8TunerIndex,0x90, 0x15); //fall
    RDA5815WriteReg(u8TunerIndex,0x91, 0x39);
    RDA5815WriteReg(u8TunerIndex,0x92, 0x30);
    RDA5815WriteReg(u8TunerIndex,0x93, 0x27);
    RDA5815WriteReg(u8TunerIndex,0x94, 0x29);
    RDA5815WriteReg(u8TunerIndex,0x95, 0x0d);
    RDA5815WriteReg(u8TunerIndex,0x96, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x97, 0x1e);
    RDA5815WriteReg(u8TunerIndex,0x98, 0x1a);
    RDA5815WriteReg(u8TunerIndex,0x99, 0x19);
    RDA5815WriteReg(u8TunerIndex,0x9a, 0x19);
    RDA5815WriteReg(u8TunerIndex,0x9b, 0x32);
    RDA5815WriteReg(u8TunerIndex,0x9c, 0x1f);
    RDA5815WriteReg(u8TunerIndex,0x9d, 0x31);
    RDA5815WriteReg(u8TunerIndex,0x9e, 0x43);

    RDA5815Sleep(10);//Wait 10ms;

    // Initial configuration end
}

/********************************************************************************/
//  Function to Set the RDA5815
//  fPLL:       Frequency           unit: MHz from 250 to 2300
//  fSym:   SymbolRate          unit: KSps from 1000 to 60000
/********************************************************************************/

INT32 RDA5815Set(MS_U8 u8TunerIndex, unsigned long fPLL, unsigned long fSym)
{
    unsigned char buffer;
    unsigned long temp_value = 0;
    unsigned long bw;/*,temp_value1 = 0,temp_value2=0 ;*/
    unsigned char Filter_bw_control_bit;

    RDA5815WriteReg(u8TunerIndex,0x04, 0xc1); //add by rda 2011.8.9,RXON = 0 , change normal working state to idle state
    RDA5815WriteReg(u8TunerIndex,0x2b, 0x95); //clk_interface_27m=0  add by rda 2012.1.12

    //set frequency start
#if (Xtal_27M == 1)     // v1.1
    temp_value = (unsigned long)fPLL * 77672; //((2^21) / RDA5815_XTALFREQ);
#endif
#if (Xtal_30M == 1)     // v1.2
    temp_value = (unsigned long)fPLL * 69905; //((2^21) / RDA5815_XTALFREQ);
#endif
#if (Xtal_24M == 1)     // v1.3
    temp_value = (unsigned long)fPLL * 87381; //((2^21) / RDA5815_XTALFREQ);
#endif

    buffer = ((unsigned char)((temp_value >> 24) & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x07, buffer);
    buffer = ((unsigned char)((temp_value >> 16) & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x08, buffer);
    buffer = ((unsigned char)((temp_value >> 8) & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x09, buffer);
    buffer = ((unsigned char)(temp_value & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x0a, buffer);
    //set frequency end

    // set Filter bandwidth start
    bw = fSym;      //kHz

    Filter_bw_control_bit = (unsigned char)((bw * 135 / 200 + 4000) / 1000);

    if (Filter_bw_control_bit < 4)
        Filter_bw_control_bit = 4;    // MHz
    else if (Filter_bw_control_bit > 40)
        Filter_bw_control_bit = 40;   // MHz

    Filter_bw_control_bit &= 0x3f;
    Filter_bw_control_bit |= 0x40;      //v1.5

    RDA5815WriteReg(u8TunerIndex,0x0b, Filter_bw_control_bit);
    // set Filter bandwidth end

    RDA5815WriteReg(u8TunerIndex,0x04, 0xc3);    //add by rda 2011.8.9,RXON = 0 ,rxon=1,normal working
    RDA5815WriteReg(u8TunerIndex,0x2b, 0x97);    //clk_interface_27m=1  add by rda 2012.1.12
    RDA5815Sleep(5);//Wait 5ms;

    return 1;
}


/********************************************************************************/
// Function: InitTuner
// Description: initialize tuner setting
// Parameter: void *    pData

/********************************************************************************/

MS_BOOL  MDrv_Tuner_RDA5815M_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    printf("RDA5815M v1.5 Tuner Init\n");
    RDA5815Initial(u8TunerIndex);

    return 1;
}


MS_BOOL MDrv_Tuner_RDA5815M_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 dwFreq, MS_U32 u32SymbolRate_kHz)
{
//    MS_U32 u32BW;
//    printf("RDA5815 Freq:%lu,Bandwidth:%u\n",dwFreq,ucBw);
    //u32BW = (6+ucBw)*1000;
    RDA5815Set(u8TunerIndex, dwFreq, u32SymbolRate_kHz);

    return 1;
}

#define RDA5815M_CHIP_ID0 0x58
#define RDA5815M_CHIP_ID1 0xf8
MS_BOOL MDrv_Tuner_RDA5815M_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 u8data[2] = {0};
    MS_U8 u8addr = 0;
    MS_IIC_PORT ePort;

    ePort = getI2CPort(u8TunerIndex);
    MDrv_IIC_ReadBytes(ePort, (MS_U16)RDA5815M_SLA, 1, &u8addr, 2, u8data);

    if((u8data[1] != RDA5815M_CHIP_ID1) || (u8data[0] != RDA5815M_CHIP_ID0))
        return FALSE;

    if(NULL != pu32channel_cnt)
        *(pu32channel_cnt) += 1;

    return TRUE;
}


MS_BOOL RDA5815M_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE;
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _DigiTuner_Decide_LNB_LO(SAT_PARAM);
            break;

        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            *((int*)data)=MDrv_RDA5815M_RSSI(u8TunerIndex);
            break;

        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_RDA5815M) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_RDA5815M",          // demod system name
    .data               = TUNER_RDA5815M,            // private data value
    .Init               = MDrv_Tuner_RDA5815M_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_RDA5815M_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_RDA5815M_CheckExist,
    .Extension_Function = RDA5815M_Extension_Function,
};



#endif


