
#ifndef DEVTUNER_NXP_TD1611ALF_C
#define DEVTUNER_NXP_TD1611ALF_C

//#include <stdio.h>
//#include "error.h"
//#include "default.h"
//#include "MsCommon.h"
#include "device/COFDM_Demodulator.h"
extern void MsOS_DelayTask (MS_U32 u32Ms);

#define IN
#define ERR_NO_ERROR 0
#define SysEnterCriticalSection()
#define SysLeaveCriticalSection()
#define DBG_MSB(a) a

#define DBG_TUNER(x)  x
#define DBG_TD1611ALF(format, args...) printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)
#define TUNER_DEV_ADR                   0xC0
#define TUNER_IF_FREQ                   36160
#define NXP_TD1611ALF_INT_AGC	1

#define IIC_WRITE(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   devCOFDM_PassThroughI2C_WriteBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)
#define IIC_READ(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   devCOFDM_PassThroughI2C_ReadBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)

//==================================================================================================
//Following function were added for compatible Chakra2 tuner code base.
//==================================================================================================
void devDigitalTuner_Init()
{
    DBG_TD1611ALF("\n");
}
void devTunerSetFreqStep(FREQSTEP eFreqStep)
{
    DBG_TD1611ALF("\n");
}
void devTunerSetPLLData(WORD wPLLData, RFBAND eBand)
{
    DBG_TD1611ALF("\n");
}
void devTunerInit(void)
{
    DBG_TD1611ALF("\n");
}
void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    MS_U8 CONFIG[5];
    MS_U8 CB0,CB1;
    MS_BOOL dwError = FALSE;
    MS_U8    ucfreq[2];
    MS_U16    wFrequency;
    MS_U8    ucDividerRatio = 6;
    //eMode = eMode;
    // Use critical section to protect Tuner frequency setting
    //SysEnterCriticalSection();

    CB0 = 0xC8;
    CB1 = 0;
    //P5 P4
    if(eBandWidth == E_RF_CH_BAND_8MHz)
    {
          CB0 |= 0x01;
          CB1 |= 0x08;//according to -4 spec only.
          ucDividerRatio = 7;
          printf("BAND 8 \n");
    }
    else
    {
         CB0 |= 0x02; //R2R1R0
         ucDividerRatio = 6;
         printf("BAND != 8 \n");
    }
    Freq*=1000;
    DBG_TUNER(printf("sttest tune %d ,%d \n",(int)Freq,(int)eBandWidth));

    wFrequency = (MS_U16)(((Freq + TUNER_IF_FREQ) * ucDividerRatio + 500) / 1000);
    ucfreq[1] = (MS_U8)((wFrequency >> 8) & 0x00FF);
    ucfreq[0] = (MS_U8)(wFrequency & 0x00FF);

    //P3 P2 P1
    if ( Freq <= 174000 ) // VHF low
    {
        CB1 |= 0x00;
    }
    else if ( Freq  <= 470000 ) // VHF high
    {
        CB1 |= 0x02;
    }
    else  // UHF
    {
        CB1 |= 0x04;
    }

    //Charge Pump  CP2 CP1 CP0
    Freq += TUNER_IF_FREQ;
    if(Freq<=200000)
        CB1|= 0x60;// not used
    else if(Freq<=290000)
        CB1|= 0x60;
    else if(Freq<=420000)
        CB1|= 0xA0;
    else if(Freq<=480000)
        CB1|= 0xC0;
    else if(Freq<=620000)
        CB1|= 0x60;
    else if(Freq<=830000)
        CB1|= 0xA0;
    else
        CB1|= 0xE0;

    CONFIG[0]  = ucfreq[1];
    CONFIG[1]  = ucfreq[0];
    CONFIG[2]  = CB0;
    CONFIG[3]  = CB1;
    //mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(TUNER1);
    //dwError = iptr->WriteBytes(0, NULL, 4, CONFIG);
    dwError = devCOFDM_PassThroughI2C_WriteBytes(TUNER_DEV_ADR, 0, NULL, 4, CONFIG);
    //IIC_WRITE(TUNER_DEV_ADR,NULL,0,CONFIG,4);

    if (dwError)
    {
        MsOS_DelayTask(20);

        #if (NXP_TD1611ALF_INT_AGC)
           CONFIG[2]  = 0x8C;
        #else
           CONFIG[2]  = 0x8E;
        #endif

        //dwError = iptr->WriteBytes(0, NULL, 4, CONFIG);
        dwError = devCOFDM_PassThroughI2C_WriteBytes(TUNER_DEV_ADR, 0, NULL, 4, CONFIG);
        MsOS_DelayTask(100);
        //IIC_WRITE(TUNER_DEV_ADR,NULL,0,CONFIG,4);
        //devCOFDM_PassThroughI2C_WriteBytes(_TUNER_I2C_ID, 0, NULL, 4, config);

    #if 1
        if (dwError)
        {
            MS_U8 n=0;
            while(n<20)
            {
                MsOS_DelayTask(1);
                //dwError=iptr->ReadBytes(0,NULL,1, CONFIG);
                CONFIG[0] = 0x0;
                dwError = devCOFDM_PassThroughI2C_ReadBytes(TUNER_DEV_ADR, 0, NULL, 1, CONFIG);//
                //IIC_READ(TUNER_DEV_ADR,NULL,0,CONFIG,1);
                n++;
                /*if((CONFIG[0]&0x08)==0x08)
                    printf("Internal RF AGC!\n");
                else
                   printf("external RF AGC\n");*/
                //printf("CONFIG [0] %d \n",CONFIG[0]);
                //printf("CONFIG [1] %d \n",CONFIG[1]);
                //printf("CONFIG [2] %d \n",CONFIG[2]);
                //printf("CONFIG [3] %d \n",CONFIG[3]);
                //printf("CONFIG [4] %d \n",CONFIG[4]);
                if((CONFIG[0]==0xFF)||(dwError==FALSE)) // not by pass
                {
                    dwError=FALSE;
                    break;
                }
                else if((CONFIG[0]&0x40)==0x40)
                {
                    printf("Tuner locked %d!\n",n);
                    break;
                }
                else
                {
                    printf("tuner lock failed\n");
                }

            }

            if((n>=20)||(dwError==FALSE))
            {
                printf("tuner config fail; status:%x\n",CONFIG[0]);
                dwError=FALSE;
            }
        }
    #else
        MsOS_DelayTask(1);
    #endif

    }

    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    //return (dwError);

}
#endif

