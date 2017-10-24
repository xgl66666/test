/*@ <Include> @*/
#include "device/COFDM_Demodulator.h"
#define INTERFACE extern
extern void MsOS_DelayTask (MS_U32 u32Ms);

#if (FRONTEND_TUNER_TYPE == TDVG_H252F_TUNER)

int RunMOPLL(double frequency) //frequency : MHz unit
{
    unsigned char ucTunerByte[5];
    unsigned char ucMopllI2cAddr;
    long lDivider;
    int i;

printf("RunMOPLL freq %.3f \n",frequency);
    ucMopllI2cAddr = 0xC0; //ADB
    ucTunerByte[0] = 0x00; //DB1
    ucTunerByte[1] = 0x00; //DB2
    ucTunerByte[2] = 0x00; //CB1
    ucTunerByte[3] = 0x00; //BB
    ucTunerByte[4] = 0xD0; //CB2

    lDivider = (((double)(frequency*1000) + (double)(44*1000) + (double)((62500/1000)>>1) ) *1000 )/62500;

    ucTunerByte[0] = (unsigned char)( (0x7F) & (lDivider >> 8) );
    ucTunerByte[1] = (unsigned char)( (0xFF) & (lDivider) );

    ucTunerByte[2] = 0x9B;

    if(frequency <165.01)
        ucTunerByte[3] = 0x01; //VHF low
    else if(frequency < 450.01)
        ucTunerByte[3] = 0x02; //VHF high
    else
        ucTunerByte[3] = 0x08; //UHF

    ucTunerByte[4] = 0xC0;
    if (devCOFDM_PassThroughI2C_WriteBytes(ucMopllI2cAddr, 0, 0, 5,ucTunerByte)== FALSE)
    {
        printf("%s: tuner write fail\n", __FUNCTION__);
        return FALSE;
    }

    for(i=0;i<100;i++) //Tuner Lock check
    {
       if (devCOFDM_PassThroughI2C_ReadBytes(ucMopllI2cAddr,0,0,1,ucTunerByte)== FALSE)
        {
                printf("%s: tuner read fail\n", __FUNCTION__);
                return FALSE;
        }

        if(ucTunerByte[0] & 0x40)
        {
            if(ucTunerByte[0] != 0xFF)
            {
                printf("Tuner is Locked");
                return 0;
                break;
            }
        }
        // wait(1); //wait 1ms
        MsOS_DelayTask(1);
        if(i==99)
        {
            printf("Tuner is Unlocked");
            return 1;
        }
    }
    return 1;
}

void devDigitalTuner_Init()
{

}

void devTunerInit(void)
{
}


void devTunerSetFreqStep(FREQSTEP eFreqStep)
{

}

void devTunerSetPLLData(WORD wPLLData, RFBAND eBand)
{
}

void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    RunMOPLL((double)(Freq));
}

#endif
