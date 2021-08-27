

#if (FRONTEND_TUNER_TYPE == TUNER_MXL201RF)
#include <device/COFDM_Demodulator.h>
#include <sys/drv/drvIIC.h>
#include "MXL201RF/MxL201RF_API.h"
#include "MXL201RF/MxL201RF_Common.h"
#include "MXL201RF/MxL201RF.c"
#include "MXL201RF/MxL201RF_API.c"

#define TUNER_DEV_ADR                   0xC0//MxL_I2C_ADDR_96 << 2 // MT2060, Env77h11d5,FS803A=0xc0, Maxlinear=0xc6, QT1010=0xc4
#define TUNER_IF_FREQ                   (MxL_IF_4_57_MHZ)/1000//(MxL_IF_36_15_MHZ / 1000)     // 36125:  Tuner IF Frequency (KHz) or(36167, (4570 for Maxlinear))

#define ERR_NO_ERROR 0
#define ERR_WRITE_TUNER_FAIL        0x00000007ul


#define SysEnterCriticalSection()
#define SysLeaveCriticalSection()
#define DBG_MSB(x)          x

static MxL201RF_TunerConfigS MxL201RF_TunerPara;


MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    MS_BOOL ret;
    if(bPowerOn==FALSE)
    {
        ret = MxL_Stand_By(&MxL201RF_TunerPara);
        if(MxL_ERR_OTHERS == ret)
            return FALSE;
        return TRUE;
    }
    else
    {
        ret = MxL_Wake_Up(&MxL201RF_TunerPara);
        if(MxL_OK == ret)
             return TRUE;
        return FALSE;
    }

}


//**********************************************************************
// Set Tuner to desired frequency
//
//**********************************************************************
void MXL201RF_SetTuner(
    MS_U32      dwFreq,    // KHz
    MxL201RF_BW_MHz       ucBw       // MHz
)
{
    MS_BOOL bLock = FALSE;
    DBG_MSB(printf("\n MXL201RF SetTuner dwFreq = %d, ucBw = %d \n",(MS_U32)(dwFreq * 1000),ucBw));
    if (MxL_Tuner_RFTune(&MxL201RF_TunerPara, (MS_U32)(dwFreq * 1000), (MxL201RF_BW_MHz)ucBw))
    {
        DBG_MSB(printf("\n\n#### %s Fail\n\n", __FUNCTION__));
    }

    MxL_RFSynth_Lock_Status(&MxL201RF_TunerPara, &bLock);
    DBG_MSB(printf("RFSynth bLock = %d \n",bLock));

    MxL_REFSynth_Lock_Status(&MxL201RF_TunerPara, &bLock);
    DBG_MSB(printf("REFSynth bLock = %d \n",bLock));

    // Release ownership of the critical section
    SysLeaveCriticalSection();
}

//**********************************************************************
// This function is not needed for LG_TDTMG551D.
//
// Please just keep it empty.
//
//**********************************************************************
static MS_U32 InitTuner(void)
{

    MxL201RF_TunerConfigS myTuner;
    myTuner.I2C_Addr = TUNER_DEV_ADR;
    myTuner.Mode = MxL_MODE_DVBT;
    //myTuner.IF_Diff_Out_Level = 0;
    myTuner.Xtal_Freq = MxL_XTAL_24_MHZ;
    myTuner.Xtal_Cap = MxL_XTAL_CAP_25_PF;
    myTuner.IF_Freq = MxL_IF_4_57_MHZ;
    myTuner.IF_Spectrum = MxL_NORMAL_IF;
    myTuner.ClkOut_Setting = MxL_CLKOUT_DISABLE;
    myTuner.ClkOut_Amp = MxL_CLKOUT_AMP_0;

    memcpy(&MxL201RF_TunerPara, &myTuner, sizeof(MxL201RF_TunerConfigS));
    //Init Tuner
    if (MxL_Tuner_Init(&MxL201RF_TunerPara))
    {
        DBG_MSB(printf("%s fail \n", __FUNCTION__));
        return (ERR_WRITE_TUNER_FAIL);
    }
    printf("\n MXL201RF INIT OK\n");
    return (ERR_NO_ERROR);

}

MS_BOOL MDrv_Tuner_Init(void)
{
    if(InitTuner() == ERR_NO_ERROR)
    {

        printf("\n MXL201RF INIT OK\n");
        return TRUE;
    }
    else
    {

        printf("\n MXL201RF INIT FAILED\n");
        return FALSE;
    }
}


//**********************************************************************
// Set Tuner to desired frequency
//
//**********************************************************************
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MS_U32   dwError = ERR_NO_ERROR;
    MxL201RF_BW_MHz MxL201RF_ucBw;

    DBG_MSB(printf("\n MXL201RF SetTuner \n"));

    switch (ucBw)
    {
        case MxL_BW_6MHz:
            MxL201RF_ucBw = MxL_BW_6MHz;
        break;

        case MxL_BW_7MHz:
            MxL201RF_ucBw = MxL_BW_7MHz;
        break;

        case MxL_BW_8MHz:
            MxL201RF_ucBw = MxL_BW_8MHz;
        break;
        default:
            MxL201RF_ucBw = MxL_BW_8MHz;
        break;
    }


    MXL201RF_SetTuner(dwFreq,MxL201RF_ucBw);


    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    return (dwError);
}


//**********************************************************************
// This function is not needed for MXL201RF.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 AdjustTuner(void)
{
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for MXL201RF.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 CalibrateTuner(void)
{
    return (ERR_NO_ERROR);
}

MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{
    return TRUE;
}


MS_U32 MDrv_Tuner_Stand_By(void)
{
    return TRUE;
}


MS_U32 MDrv_Tuner_Wake_Up(void)
{
    return TRUE;
}


// Dummy for compile
void devDigitalTuner_Init()
{
    MDrv_Tuner_Init();
}

void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    MS_U8 ucBw;
    ucBw = eBandWidth+5;

    MDrv_Tuner_SetTuner((MS_U32)Freq,ucBw);
}


#endif

