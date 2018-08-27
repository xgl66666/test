/*Filename:     RDA5817_1.X.c
 Description:   RDA5817 Digital Satellite Tuner IC driver.
version 1.0     The primary version,  created by hongxin wang                               2014-9-26
version 1.01        Modify the agc setting and add overlap between close gain stage         2014-10-8
version 1.02        Modify the agc setting, enlarge the overlap                             2014-10-9
version 1.03        Modify the agc stage                                                    2014-10-9
version 1.04        Modify the agc stage, Increase the gain stage overlap                   2014-10-12
version 1.05        Add pll setting in initial for pll stability
                            Add 24M crystal code                                            2014-10-15

version 1.06        Modify sleep mode function to enable loopthrough.
version 1.07        Optimize initial function and AGC for B version.                        2014-12-11
version 1.1.0       Modify initial and AGC for D version chip.                              2015-2-2
version 1.1.1       Fix loop shaking issue.Add RSSI function                                2015-2-13
version 1.1.2       Modify RSSI function and AGC                                            2015-2-27
version 1.1.3       Optimize AGC and RSSI for booster performance.                          2015-3-4
version 1.2.0       Optimize AGC for E version chip.                                        2015-5-14
version 1.2.1       For F version chip.                                                     2015-10-26
version 1.2.2       fix AGC mistake.                                                        2015-10-28
version 1.2.3       For mass production                                                     2015-12-30
version 1.2.4       Modify sleep function                                                   2016-01-08
version 2.0       Modify AGC mode                                                           2016-04-06
version 2.1       Modify AGC mode   2016-06-21
version 2.2      Modify AGC mode	                                            2016-12-29
*/

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#endif
#include "error.h"
#include "MsCommon.h"
#include "Board.h"
#include "apiDigiTuner.h"
#include "drvIIC.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_RDA5817)

#define     Xtal_27M
//#define   Xtal_24M
#define     loop_on
//#define   loop_off
#define     RDA5817_SLA             0x18
#define     RDA_Tuner_Sleeping      0x02
#define     RDA_Tuner_Waking        0x03
#define     RDA5817Sleep(x)         MsOS_DelayTask(x)
#define     MAX_INPUT_FREQ          2150 //Mhz
#define     MIN_INPUT_FREQ          950  //Mhz

static TUNER_MS_SAT_PARAM RDA5817_SATParam[MAX_FRONTEND_NUM];

static  MS_BOOL  _DigiTuner_Decide_LNB_LO(TUNER_MS_SAT_PARAM *pSATParam)
{
    MS_S32 s32Freq;
    // calc Mid Freq & LNB control(22K on/off)
    if (pSATParam->u16LoLOF == pSATParam->u16HiLOF)
    {
        *pSATParam->pbIsHiLOF = FALSE;
    }
    else // 2LOF
    {
        if (pSATParam->u16LoLOF < MAX_C_LOF_FREQ) // c band
        {
            if (pSATParam->u8bPorInvert)
            {
                if (pSATParam->u8_Polarity == 1) //V ---> use low LOF
                {
                    *pSATParam->pbIsHiLOF = FALSE;
                }
                else   //H --->use high LOF
                {
                    *pSATParam->pbIsHiLOF = TRUE;
                }
            }
            else
            {
                if (pSATParam->u8_Polarity  == 0) //H ---> use low LOF
                {
                    *pSATParam->pbIsHiLOF = FALSE;
                }
                else   //V --->use high LOF
                {
                    *pSATParam->pbIsHiLOF = TRUE;
                }
            }
        }
        else //Ku band
        {
            MS_U32 u32MidFreq;
            MS_S32 s32Offset = 0;
            s32Freq  = pSATParam->u32RF_FREQ;
            u32MidFreq =  abs(s32Freq - pSATParam->u16LoLOF) ;
            s32Offset = pSATParam->u16LoLOF + MAX_INPUT_FREQ - pSATParam->u16HiLOF - MIN_INPUT_FREQ;
            if (s32Offset < 0)
                s32Offset = 0;
            else
                s32Offset /= 2;

            if (u32MidFreq <= (MAX_INPUT_FREQ - s32Offset))
                *pSATParam->pbIsHiLOF = FALSE;
            else
                *pSATParam->pbIsHiLOF = TRUE;
        }
    }
    return TRUE;
}

static MS_BOOL RDA5817WriteReg(MS_U8 u8TunerIndex, MS_U8 paddr, MS_U8 pu8data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Value[2];
    HWI2C_PORT hwi2c_port;

    u8Value[0] = paddr;
    u8Value[1] = pu8data;

    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        bRet &= MDrv_IIC_Write(RDA5817_SLA, 0, 0, u8Value, 2);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        bRet &= MDrv_IIC1_Write(RDA5817_SLA, 0, 0, u8Value, 2);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    if (bRet == FALSE)
    {
        TUNER_ERR((" ERROR in RDA5815WriteReg !!! \n"));
    }
    return bRet;
}

static MS_U8 RDA5817ReadReg(MS_U8 u8TunerIndex, MS_U8 paddr)
{
    MS_U8 pu8data = 0;
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        MDrv_IIC_Read(RDA5817_SLA, &paddr, 1, &pu8data, 1);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        MDrv_IIC1_Read(RDA5817_SLA, &paddr, 1, &pu8data, 1);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    return pu8data;
}

void RDA5817Initial(MS_U8 u8TunerIndex)
{
    //RDA5817WriteReg(u8TunerIndex,register address,register data);

    RDA5817Sleep(1);//Wait 1ms.
    // Chip register soft reset
    RDA5817WriteReg(u8TunerIndex, 0x04, 0x04);
    RDA5817WriteReg(u8TunerIndex, 0x04, 0x05);

    // Initial configuration start

    RDA5817WriteReg(u8TunerIndex, 0x0d, 0x70); // vga_dc_range =10
    RDA5817WriteReg(u8TunerIndex, 0x0e, 0x7e); // set reg xtal to 11 to decrease the xtal harmonic
    RDA5817WriteReg(u8TunerIndex, 0x65, 0xc0); // mix_div_vbit =11
    RDA5817WriteReg(u8TunerIndex, 0x2f, 0x5f); // ldo_vbit  =111
    RDA5817WriteReg(u8TunerIndex, 0x2B, 0x97);
    RDA5817WriteReg(u8TunerIndex, 0x46, 0x21);
    RDA5817WriteReg(u8TunerIndex, 0x47, 0x84);
    RDA5817WriteReg(u8TunerIndex, 0x48, 0x10);
    RDA5817WriteReg(u8TunerIndex, 0x60, 0xA0);
    RDA5817WriteReg(u8TunerIndex, 0x61, 0x60);
    RDA5817WriteReg(u8TunerIndex, 0x6A, 0x20);
    RDA5817WriteReg(u8TunerIndex, 0x69, 0xC0);
    RDA5817WriteReg(u8TunerIndex, 0x57, 0x64);

//pll setting
    RDA5817WriteReg(u8TunerIndex, 0x05, 0xdd);
    RDA5817WriteReg(u8TunerIndex, 0x06, 0xdd);
    RDA5817WriteReg(u8TunerIndex, 0x4a, 0x88);
    RDA5817WriteReg(u8TunerIndex, 0x4b, 0x88);
    RDA5817WriteReg(u8TunerIndex, 0x3e, 0xa4);
    RDA5817WriteReg(u8TunerIndex, 0x3f, 0x24);
    RDA5817WriteReg(u8TunerIndex, 0x44, 0x49);
    RDA5817WriteReg(u8TunerIndex, 0x45, 0x24);
    RDA5817WriteReg(u8TunerIndex, 0x3c, 0x91);
    RDA5817WriteReg(u8TunerIndex, 0x3d, 0x80);
    RDA5817WriteReg(u8TunerIndex, 0x70, 0x00);
    RDA5817WriteReg(u8TunerIndex, 0x71, 0x00);

#ifdef Xtal_27M
    RDA5817WriteReg(u8TunerIndex, 0x72, 0x06); //band1:band2 = 1417:1418
    RDA5817WriteReg(u8TunerIndex, 0x73, 0x8d);

    RDA5817WriteReg(u8TunerIndex, 0x75, 0x05); //band0:band1 = 1201:1202   band2:band3 = 1795:1796
    RDA5817WriteReg(u8TunerIndex, 0x76, 0x80);
    RDA5817WriteReg(u8TunerIndex, 0x77, 0x84);

    RDA5817WriteReg(u8TunerIndex, 0x79, 0x06); //vco div2/3 tha=1417:1418
    RDA5817WriteReg(u8TunerIndex, 0x7a, 0x90);
    RDA5817WriteReg(u8TunerIndex, 0x7b, 0x67);
    RDA5817WriteReg(u8TunerIndex, 0x7c, 0xb4);
#endif

#ifdef Xtal_24M
    RDA5817WriteReg(u8TunerIndex, 0x72, 0x07); //band1:band2 = 1415:1416
    RDA5817WriteReg(u8TunerIndex, 0x73, 0x5f);
    RDA5817WriteReg(u8TunerIndex, 0x74, 0x85);

    RDA5817WriteReg(u8TunerIndex, 0x75, 0x06); //band0:band1 = 1199:1200   band2:band3 = 1799:1800
    RDA5817WriteReg(u8TunerIndex, 0x76, 0x30);
    RDA5817WriteReg(u8TunerIndex, 0x77, 0x95);

    RDA5817WriteReg(u8TunerIndex, 0x79, 0x07); //vco div2/3 tha=1415:1416
    RDA5817WriteReg(u8TunerIndex, 0x7a, 0x5f);
    RDA5817WriteReg(u8TunerIndex, 0x7b, 0x00);
    RDA5817WriteReg(u8TunerIndex, 0x7c, 0x00);
#endif

//agc setting
#ifdef loop_off
    RDA5817WriteReg(u8TunerIndex, 0x5b, 0xa0); //agc_dual_thr14_en=1 1/4

    RDA5817WriteReg(u8TunerIndex, 0x16, 0x00); //i2v filter st1 st2
    RDA5817WriteReg(u8TunerIndex, 0x18, 0x40);
    RDA5817WriteReg(u8TunerIndex, 0x30, 0x10);
    RDA5817WriteReg(u8TunerIndex, 0x5c, 0x80);
    RDA5817WriteReg(u8TunerIndex, 0x6c, 0x90);
    RDA5817WriteReg(u8TunerIndex, 0x6e, 0xa0);
    RDA5817WriteReg(u8TunerIndex, 0x1b, 0x0c);
    RDA5817WriteReg(u8TunerIndex, 0x1d, 0x1c);
    RDA5817WriteReg(u8TunerIndex, 0x1f, 0x5c);
    RDA5817WriteReg(u8TunerIndex, 0x21, 0x3c);
    RDA5817WriteReg(u8TunerIndex, 0x23, 0xec);
    RDA5817WriteReg(u8TunerIndex, 0x25, 0xfc);
    RDA5817WriteReg(u8TunerIndex, 0x27, 0xfc);
    RDA5817WriteReg(u8TunerIndex, 0x29, 0xfc);
    RDA5817WriteReg(u8TunerIndex, 0xb3, 0xff);
    RDA5817WriteReg(u8TunerIndex, 0xb5, 0xff);

    RDA5817WriteReg(u8TunerIndex, 0x17, 0xe3); //11 cap pre post
    RDA5817WriteReg(u8TunerIndex, 0x19, 0xe3);
    RDA5817WriteReg(u8TunerIndex, 0x31, 0xe3);
    RDA5817WriteReg(u8TunerIndex, 0x5d, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x6d, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x6f, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x1c, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x1e, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x20, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x22, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x24, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x26, 0xf3);
    RDA5817WriteReg(u8TunerIndex, 0x28, 0xfb);
    RDA5817WriteReg(u8TunerIndex, 0x2a, 0xff);
    RDA5817WriteReg(u8TunerIndex, 0xb4, 0xff);
    RDA5817WriteReg(u8TunerIndex, 0xb6, 0xff);

    RDA5817WriteReg(u8TunerIndex, 0xb7, 0x06);  //start
    RDA5817WriteReg(u8TunerIndex, 0xb9, 0x0d);
    RDA5817WriteReg(u8TunerIndex, 0xbb, 0x2e);
    RDA5817WriteReg(u8TunerIndex, 0xbd, 0x28);
    RDA5817WriteReg(u8TunerIndex, 0xbf, 0x25);
    RDA5817WriteReg(u8TunerIndex, 0xc1, 0x20);
    RDA5817WriteReg(u8TunerIndex, 0xc3, 0x28);
    RDA5817WriteReg(u8TunerIndex, 0xc5, 0x25);
    RDA5817WriteReg(u8TunerIndex, 0xa3, 0x2f);
    RDA5817WriteReg(u8TunerIndex, 0xa5, 0x16);
    RDA5817WriteReg(u8TunerIndex, 0xa7, 0x00);
    RDA5817WriteReg(u8TunerIndex, 0xa9, 0x00);
    RDA5817WriteReg(u8TunerIndex, 0xab, 0x00);
    RDA5817WriteReg(u8TunerIndex, 0xad, 0x20);
    RDA5817WriteReg(u8TunerIndex, 0xaf, 0x03);
    RDA5817WriteReg(u8TunerIndex, 0xb1, 0x3c);


    RDA5817WriteReg(u8TunerIndex, 0xb8, 0x42); //end
    RDA5817WriteReg(u8TunerIndex, 0xba, 0x4a);
    RDA5817WriteReg(u8TunerIndex, 0xbc, 0x67);
    RDA5817WriteReg(u8TunerIndex, 0xbe, 0x64);
    RDA5817WriteReg(u8TunerIndex, 0xc0, 0x5b);
    RDA5817WriteReg(u8TunerIndex, 0xc2, 0x59);
    RDA5817WriteReg(u8TunerIndex, 0xc4, 0x5e);
    RDA5817WriteReg(u8TunerIndex, 0xc6, 0x5d);
    RDA5817WriteReg(u8TunerIndex, 0xa4, 0x67);
    RDA5817WriteReg(u8TunerIndex, 0xa6, 0x5f);
    RDA5817WriteReg(u8TunerIndex, 0xa8, 0x48);
    RDA5817WriteReg(u8TunerIndex, 0xaa, 0x3f);
    RDA5817WriteReg(u8TunerIndex, 0xac, 0x40);
    RDA5817WriteReg(u8TunerIndex, 0xae, 0x70);
    RDA5817WriteReg(u8TunerIndex, 0xb0, 0x46);
    RDA5817WriteReg(u8TunerIndex, 0xb2, 0x73);
#endif

    RDA5817Sleep(10);//Wait 10ms;
}

unsigned char RDA5817Set(MS_U8 u8TunerIndex, unsigned long fPLL, unsigned long fSym)
{
    unsigned char buffer;
    unsigned long temp_value = 0;
    unsigned long bw;
    unsigned char Filter_bw_control_bit;

    RDA5817WriteReg(u8TunerIndex, 0x04, 0xc1);

    //set frequency start

#ifdef Xtal_27M
    temp_value = (unsigned long)fPLL * 77672;
#endif

#ifdef Xtal_24M
    temp_value = (unsigned long)fPLL * 87381;
#endif


    buffer = ((unsigned char)((temp_value >> 24) & 0xff));
    RDA5817WriteReg(u8TunerIndex, 0x07, buffer);
    buffer = ((unsigned char)((temp_value >> 16) & 0xff));
    RDA5817WriteReg(u8TunerIndex, 0x08, buffer);
    buffer = ((unsigned char)((temp_value >> 8) & 0xff));
    RDA5817WriteReg(u8TunerIndex, 0x09, buffer);
    buffer = ((unsigned char)(temp_value & 0xff));
    RDA5817WriteReg(u8TunerIndex, 0x0a, buffer);
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

    RDA5817WriteReg(u8TunerIndex, 0x0b, Filter_bw_control_bit);
    // set Filter bandwidth end

    RDA5817WriteReg(u8TunerIndex, 0x04, 0xc3);

    RDA5817Sleep(5);//Wait 5ms;

    return 0;
}

// to get the lock status of tuner VCO & PLL
unsigned char RDA5817_LockStatus(MS_U8 u8TunerIndex, unsigned char tuner_addr)
{
    unsigned char buffer, i ;

    for (i = 0; i < 100; i++)   // Loop times: 100 is recommanded; at least 30.
    {
        buffer = RDA5817ReadReg(u8TunerIndex, 0x03);
        if ((buffer & 0x03) != 0x03)
        {
            return 1;
        }
    }
    return 0;
}

// to set RDA tuner to sleep status (low power consumption)
unsigned char RDA5817_Sleep(MS_U8 u8TunerIndex, unsigned char tuner_addr)
{
    unsigned char buffer;

    buffer = RDA5817ReadReg(u8TunerIndex, 0x04);
    if ((buffer == 0xff) || (buffer == 0))
    {
        return 1;
    }
    else
    {
        RDA5817WriteReg(u8TunerIndex, 0x04, buffer & 0x7F);
        RDA5817WriteReg(u8TunerIndex, 0x2c, 0x0A);
        RDA5817WriteReg(u8TunerIndex, 0x2d, 0x40);
        RDA5817WriteReg(u8TunerIndex, 0x2e, 0x20);
        RDA5817WriteReg(u8TunerIndex, 0x0f, 0x44);
        RDA5817WriteReg(u8TunerIndex, 0x2c, 0x08); //close st1
        RDA5817WriteReg(u8TunerIndex, 0x2d, 0x00); //close dc_dac
    }

    return 0;
}

// to wake up RDA tuner from sleep status
unsigned char RDA5817_Wakeup(MS_U8 u8TunerIndex, unsigned char tuner_addr)
{
    unsigned char buffer;

    buffer = RDA5817ReadReg(u8TunerIndex, 0x04);
    if ((buffer == 0xff) || (buffer == 0))
    {
        return 1;
    }
    else
    {
        RDA5817WriteReg(u8TunerIndex, 0x2c, 0x0A); //open st1
        RDA5817WriteReg(u8TunerIndex, 0x2d, 0x40); //open dc_dac
        RDA5817WriteReg(u8TunerIndex, 0x0f, 0x04);
        RDA5817WriteReg(u8TunerIndex, 0x04, buffer | 0x80);
    }
    return 0;
}

// to get the running status of RDA tuner -- "Sleeping" or "Waking".
unsigned char RDA5817_RunningStatus(MS_U8 u8TunerIndex, unsigned char tuner_addr)
{
    unsigned char buffer;

    buffer = RDA5817ReadReg(u8TunerIndex, 0x04);
    if ((buffer == 0xff) || (buffer == 0))
    {
        return 1;
    }
    else
    {
        if (buffer & 0x80)
        {
            return RDA_Tuner_Waking;
        }
        else
        {
            return RDA_Tuner_Sleeping;
        }
    }
    return 0;
}

MS_BOOL  MDrv_Tuner_RDA5817_Initial(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam)
{
    RDA5817Initial(u8TunerIndex);

    return TRUE;
}

MS_BOOL MDrv_Tuner_RDA5817_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 dwFreq, MS_U32 u32SymbolRate_kHz)
{
    RDA5817_SATParam[u8TunerIndex].u32RF_FREQ = dwFreq;
    RDA5817Set(u8TunerIndex, dwFreq, u32SymbolRate_kHz);

    return TRUE;
}

MS_S32  MDrv_Tuner_RDA5817_RSSI(MS_U8 u8TunerIndex, unsigned long fPLL)
{
    unsigned char buffer;
    /* Gain stage limits                    st1 cap pre st2 post i2v filter*/
#define RDA5817_Gain_Stage__0 0x0430 /* '00 01  00  00  11   00  00 ' */
#define RDA5817_Gain_Stage__1 0x0434 /* '00 01  00  00  11   01  00 ' */
#define RDA5817_Gain_Stage__2 0x0431 /* '00 01  00  00  11   00  01 ' */
#define RDA5817_Gain_Stage__3 0x0439 /* '00 01  00  00  11   10  01 ' */
#define RDA5817_Gain_Stage__4 0x043a /* '00 01  00  00  11   10  10 ' */
#define RDA5817_Gain_Stage__5 0x043b /* '00 01  00  00  11   10  11 ' */
#define RDA5817_Gain_Stage__6 0x3425 /* '11 01  00  00  10   01  01 ' */
#define RDA5817_Gain_Stage__7 0x341a /* '11 01  00  00  01   10  10 ' */
#define RDA5817_Gain_Stage__8 0x341b /* '11 01  00  00  01   10  11 ' */
#define RDA5817_Gain_Stage__9 0x341f /* '11 01  00  00  01   11  11 ' */
#define RDA5817_Gain_Stage_10 0x342f /* '11 01  00  00  10   11  11 ' */
#define RDA5817_Gain_Stage_11 0x3c3f /* '11 11  00  00  11   11  11 ' */
#define RDA5817_Gain_Stage_12 0x3cbf /* '11 11  00  10  11   11  11 ' */
#define RDA5817_Gain_Stage_13 0x3cff /* '11 11  00  11  11   11  11 ' */
#define RDA5817_Gain_Stage_14 0x3fff /* '11 11  11  11  11   11  11 ' */
#define RDA5817_Gain_Stage_15 0x3fff /* '11 11  11  11  11   11  11 ' */

    unsigned char data16, data17, st1, cap, pre, st2, post, i2v, filter, vga;
    unsigned int  stage_code;
    unsigned char gain_stage;
    unsigned char band;
    double        vga_gain, total_gain;

    /* gain band/gain_stage:    0       1      2      3     4     5      6     7     8     9    10    11    12    13     14    15 */
    double gain[13][16] =
    {
        { -6.9,   -2.5,   -0.7,  8.5, 14.5,  20.6,  23.2, 28.0, 34.0, 39.7, 45.9, 58.3, 62.2, 64.7, 74.3, 74.3},  /* 1  950MHz<=Freq<1000MHz */
        { -6.7,   -2.5,   -0.7,  8.6, 14.6,  20.6,  23.2, 27.9, 34.5, 40.0, 46.2, 58.5, 62.3, 64.8, 74.6, 74.6},  /* 2  1000MHz<=Freq<1100MHz */
        { -6.7,   -2.4,   -0.6,  8.6, 14.7,  20.7,  23.4, 28.8, 34.0, 40.1, 46.3, 58.8, 62.4, 64.8, 75.0, 75.0},  /* 3  1100MHz<=Freq<1200MHz */
        { -6.5,   -2.5,   -0.6,  8.7, 14.7,  20.7,  23.5, 28.0, 34.0, 40.0, 46.4, 59.0, 62.4, 64.7, 75.2, 75.2},  /* 4  1200MHz<=Freq<1300MHz */
        { -6.6,   -2.5,   -0.6,  8.6, 14.6,  20.6,  23.4, 27.6, 33.9, 39.9, 46.2, 59.0, 62.2, 64.4, 75.3, 75.3},  /* 5  1300MHz<=Freq<1400MHz */
        { -7.0,   -2.9,   -1.0,  8.3, 14.4,  20.4,  23.2, 28.2, 33.7, 39.7, 46.1, 58.9, 62.0, 64.0, 75.3, 75.3},  /* 6  1400MHz<=Freq<1500MHz */
        { -7.0,   -2.9,   -1.0,  8.2, 14.2,  20.2,  23.1, 27.6, 33.5, 39.5, 45.9, 58.9, 61.8, 63.8, 75.3, 75.3},  /* 7  1500MHz<=Freq<1600MHz */
        { -7.3,   -2.9,   -1.1,  8.2, 14.2,  20.2,  23.1, 27.5, 33.6, 39.4, 46.1, 59.0, 61.8, 63.6, 75.4, 75.4},  /* 8  1600MHz<=Freq<1700MHz */
        { -7.1,   -2.8,   -0.8,  8.3, 14.3,  20.3,  23.2, 27.6, 33.6, 39.3, 46.2, 59.2, 61.9, 63.6, 75.5, 75.5},  /* 9  1700MHz<=Freq<1800MHz */
        { -7.2,   -2.9,   -1.2,  8.2, 14.2,  20.2,  23.0, 27.1, 33.3, 39.2, 45.9, 59.0, 61.5, 63.1, 75.2, 75.2},  /* 10  1800MHz<=Freq<1900MHz */
        { -7.6,   -3.4,   -1.6,  7.8, 13.7,  19.7,  22.5, 26.8, 32.9, 38.7, 45.4, 58.6, 61.0, 62.5, 74.6, 74.6},  /* 11  1900MHz<=Freq<2000MHz */
        { -7.9,   -3.8,   -1.9,  7.4, 13.4,  19.4,  22.2, 26.4, 32.4, 38.4, 45.0, 58.3, 60.5, 61.9, 74.0, 74.0},  /* 12  2000MHz<=Freq<2100MHz */
        { -8.1,   -3.9,   -2.1,  7.1, 13.2,  19.2,  22.0, 27.0, 32.2, 37.7, 44.8, 58.1, 60.1, 61.4, 73.5, 73.5}   /* 13  2100MHz<=Freq<=2150MHz */
    };

    data16 = RDA5817ReadReg(u8TunerIndex, 0x16);
    i2v    = (data16 & 0xc0) >> 6;
    filter = (data16 & 0x30) >> 4;
    st1    = (data16 & 0x0c) >> 2;
    st2    = (data16 & 0x03) >> 0;

    data17 = RDA5817ReadReg(u8TunerIndex, 0x17);
    cap    = (data17 & 0x30) >> 4;
    pre    = (data17 & 0x0c) >> 2;
    post   = (data17 & 0x03) >> 0;

    stage_code = (st1 << 12) + (cap << 10) + (pre << 8) + (st2 << 6) + (post << 4) + (i2v << 2) + (filter << 0);

    if (stage_code == RDA5817_Gain_Stage_14)
    {
        gain_stage = 14;
    }
    else if (stage_code == RDA5817_Gain_Stage_13)
    {
        gain_stage = 13;
    }
    else if (stage_code == RDA5817_Gain_Stage_12)
    {
        gain_stage = 12;
    }
    else if (stage_code == RDA5817_Gain_Stage_11)
    {
        gain_stage = 11;
    }
    else if (stage_code == RDA5817_Gain_Stage_10)
    {
        gain_stage = 10;
    }
    else if (stage_code == RDA5817_Gain_Stage__9)
    {
        gain_stage =  9;
    }
    else if (stage_code == RDA5817_Gain_Stage__8)
    {
        gain_stage =  8;
    }
    else if (stage_code == RDA5817_Gain_Stage__7)
    {
        gain_stage =  7;
    }
    else if (stage_code == RDA5817_Gain_Stage__6)
    {
        gain_stage =  6;
    }
    else if (stage_code == RDA5817_Gain_Stage__5)
    {
        gain_stage =  5;
    }
    else if (stage_code == RDA5817_Gain_Stage__4)
    {
        gain_stage =  4;
    }
    else if (stage_code == RDA5817_Gain_Stage__3)
    {
        gain_stage =  3;
    }
    else if (stage_code == RDA5817_Gain_Stage__2)
    {
        gain_stage =  2;
    }
    else if (stage_code == RDA5817_Gain_Stage__1)
    {
        gain_stage =  1;
    }
    else
    {
        gain_stage =  0;
    }

    if (fPLL < 950)
    {
        return 1;     //select band accroding to frequency
    }
    else if (fPLL < 1000)
    {
        band = 0;
    }
    else if (fPLL < 1100)
    {
        band = 1;
    }
    else if (fPLL < 1200)
    {
        band = 2;
    }
    else if (fPLL < 1300)
    {
        band = 3;
    }
    else if (fPLL < 1400)
    {
        band = 4;
    }
    else if (fPLL < 1500)
    {
        band = 5;
    }
    else if (fPLL < 1600)
    {
        band = 6;
    }
    else if (fPLL < 1700)
    {
        band = 7;
    }
    else if (fPLL < 1800)
    {
        band = 8;
    }
    else if (fPLL < 1900)
    {
        band = 9;
    }
    else if (fPLL < 2000)
    {
        band = 10;
    }
    else if (fPLL < 2100)
    {
        band = 11;
    }
    else if (fPLL <= 2150)
    {
        band = 12;
    }
    else
    {
        return 1;
    }

    buffer = RDA5817ReadReg(u8TunerIndex, 0xb7);
    vga = buffer;

    vga_gain = vga * 30.0 / 255;

    total_gain = gain[band][gain_stage] + vga_gain;

    TUNER_DBG(("RDA5817 RSSI = gain[%d][%d] + vga_gain = %f + %f = %f\n", band, gain_stage, gain[band][gain_stage], vga_gain, total_gain));

    return  -total_gain;
}

#define RDA5817_CHIP_ID0 0xb0
#define RDA5817_CHIP_ID1 0xf0
MS_BOOL MDrv_Tuner_RDA5817_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 u8data[2] = {0};
    MS_U8 u8addr = 0;
    HWI2C_PORT hwi2c_port;

    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        MDrv_IIC_Read(RDA5817_SLA, &u8addr, 1, u8data, 2);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        MDrv_IIC1_Read(RDA5817_SLA, &u8addr, 1, u8data, 2);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    if (((u8data[1] & 0xf0) != RDA5817_CHIP_ID1) || (u8data[0] != RDA5817_CHIP_ID0))
        return FALSE;

    if (NULL != pu32channel_cnt)
        *(pu32channel_cnt) += 1;

    return TRUE;
}

MS_BOOL MDrv_Tuner_RDA5817_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE;
    switch (fuction_type)
    {
        case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _DigiTuner_Decide_LNB_LO(SAT_PARAM);
            break;
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            *((float*)data) = MDrv_Tuner_RDA5817_RSSI(u8TunerIndex, RDA5817_SATParam[u8TunerIndex].u32RF_FREQ);
            break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n", fuction_type));
            break;
    }
    return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_RDA5817) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_RDA5817",          // tuner system name
    .data               = TUNER_RDA5817,            // private data value
    .Init               = MDrv_Tuner_RDA5817_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_RDA5817_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_RDA5817_CheckExist,
    .Extension_Function = MDrv_Tuner_RDA5817_Extension_Function,
};

#endif

