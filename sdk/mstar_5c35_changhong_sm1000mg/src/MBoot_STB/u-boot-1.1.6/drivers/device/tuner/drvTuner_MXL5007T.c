#ifndef DEVTUNER_MXL_5997T_C
#define DEVTUNER_MXL_5997T_C



//#include <stdio.h>
//#include "error.h"
//#include "MsCommon.h"
//#include "Board.h"
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include "apiDigiTuner.h"
#endif
#if (FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER)
#include <device/COFDM_Demodulator.h>
#include <device/drvTuner_MXL5007T.h>
#include "device/MxL5007_API.h"
#include "device/MxL5007_Common.h"
#include "MXL5007/MxL5007_API.c"
#include "MXL5007/MxL5007.c"



#define TUNER_DEV_ADR                   0xC0//MxL_I2C_ADDR_96 << 2 // MT2060, Env77h11d5,FS803A=0xc0, Maxlinear=0xc6, QT1010=0xc4
#define TUNER_IF_FREQ                   (MxL_IF_4_57_MHZ)/1000//(MxL_IF_36_15_MHZ / 1000)     // 36125:  Tuner IF Frequency (KHz) or(36167, (4570 for Maxlinear))

#define ERR_NO_ERROR 0
#define ERR_WRITE_TUNER_FAIL        0x00000007ul

#define SysEnterCriticalSection()
#define SysLeaveCriticalSection()
#define DBG_MSB(x)          x
static MxL5007_TunerConfigS MxL5007_TunerPara;

#define DBG_MxL5007(format, args...) //printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)

//**********************************************************************
// Set Tuner to desired frequency
//
//**********************************************************************
void MXL5007_SetTuner(
    MS_U32      dwFreq,    // KHz
    MxL5007_BW_MHz       ucBw       // MHz
)
{
    MS_BOOL bLock = FALSE;
    DBG_MSB(printf("\n MXL5007 SetTuner dwFreq = %d, ucBw = %d \n",(MS_U32)(dwFreq * 1000),ucBw));
    if (MxL_Tuner_RFTune(&MxL5007_TunerPara, (MS_U32)(dwFreq * 1000), (MxL5007_BW_MHz)ucBw))
    {
        DBG_MSB(printf("\n\n#### %s Fail\n\n", __FUNCTION__));
    }

    MxL_RFSynth_Lock_Status(&MxL5007_TunerPara, &bLock);
    DBG_MSB(printf("RFSynth bLock = %d \n",bLock));

    MxL_REFSynth_Lock_Status(&MxL5007_TunerPara, &bLock);
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

    MxL5007_TunerConfigS myTuner;
    myTuner.I2C_Addr = TUNER_DEV_ADR;
    myTuner.Mode = MxL_MODE_DVBT;
    myTuner.IF_Diff_Out_Level = 0;
    myTuner.Xtal_Freq = MxL_XTAL_24_MHZ;
    myTuner.IF_Freq = MxL_IF_4_57_MHZ;
    myTuner.IF_Spectrum = MxL_NORMAL_IF;
    myTuner.ClkOut_Setting = MxL_CLKOUT_DISABLE;
    myTuner.ClkOut_Amp = MxL_CLKOUT_AMP_0;

    memcpy(&MxL5007_TunerPara, &myTuner, sizeof(MxL5007_TunerConfigS));
    //Init Tuner
    if (MxL_Tuner_Init(&MxL5007_TunerPara))
    {
        DBG_MSB(printf("%s fail \n", __FUNCTION__));
        return (ERR_WRITE_TUNER_FAIL);
    }

    return (ERR_NO_ERROR);

}

MS_BOOL MDrv_Tuner_Init(void)
{
    if(InitTuner() == ERR_NO_ERROR)
    {
        return TRUE;
    }
    else
    {
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
    MxL5007_BW_MHz MxL5007_ucBw;

    DBG_MSB(printf("\n MXL5007T SetTuner \n"));

    switch (ucBw)
    {
        case BW_MODE_6MHZ:
            MxL5007_ucBw = MxL_BW_6MHz;
        break;

        case BW_MODE_7MHZ:
            MxL5007_ucBw = MxL_BW_7MHz;
        break;

        case BW_MODE_8MHZ:
            MxL5007_ucBw = MxL_BW_8MHz;
        break;
        default:
            MxL5007_ucBw = MxL_BW_8MHz;
        break;
    }


    MXL5007_SetTuner(dwFreq,MxL5007_ucBw);


    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    return (dwError);
}


//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 AdjustTuner(void)
{
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 CalibrateTuner(void)
{
    return (ERR_NO_ERROR);
}
// Dummy for compile
void devDigitalTuner_Init()
{
    DBG_MxL5007("\n");
}
void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
    DBG_MxL5007("\n");
}


#endif
#endif

