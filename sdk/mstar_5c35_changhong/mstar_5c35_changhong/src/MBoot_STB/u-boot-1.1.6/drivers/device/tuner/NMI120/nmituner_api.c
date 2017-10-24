//#include <string.h>

#include <common.h>
#include <frontend_config.h>
#if(FRONTEND_TUNER_TYPE == TUNER_NMI120 )
#include "./nmi_tuner_os.h"

#include "./nmiioctl.h"
#include <sys/common/MsTypes.h>
#include <apiDigiTuner.h>
#include <msAPI_Tuner.h>

#include "sys/common/MsOS.h"
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include "device/drvTuner.h"

#include "nmituner_api.h"


#define NMI_USE_TUNER_INTERFACE  1

#ifdef NMI_USE_TUNER_INTERFACE
#include "nmi_tuner_interface.h"
#endif


typedef enum
{
    NMI_DEMOD_TYPE_NTSC,
    NMI_DEMOD_TYPE_256QAM,
    NMI_DEMOD_TYPE_64QAM,
    NMI_DEMOD_TYPE_8VSB,
    NMI_DEMOD_TYPE_NUM
} NMI_MODU_TYPE;

#define XO_OUT_EN 1
#define XO_OUT_DIS 0

#if 1
#define NMI_LOG     nmi_tuner_os_log
#else
#define NMI_LOG     nmi_tuner_os_log
#endif
static uint32_t chipid;

#if 1
FEMode TunerMode;
void MDrv_Tuner_SetTunerMode(FEMode mode)
{
    TunerMode.Fetype = mode.Fetype;
}

EN_TUNE_TYPE MDrv_Tuner_GetTuneType(void)
{
    return TunerMode.Fetype;
}
#endif

uint8_t MDrv_Tuner_Init(void)
{
    tTnrInit cfg;
    uint32_t ret;
    cfg.ldobypass = 0;
    cfg.xo = 24;
    cfg.i2c_adr = 0xce;//0xce;
    cfg.init_bus_only = 0;
    cfg.xo_out_en = 0;
    cfg.disableOneKhzShift = FALSE;
    cfg.ltGain = nLtGainMax;

    ret = Nmi_Tuner_Interface_init_chip(&cfg);
    //ret = Nmi_Tuner_Interface_init_chip(&cfg);
    if (ret == FALSE)
    {
        //nmi_tuner_os_log(">> NMI_DRV_INIT failed. result = %d\n", result);
        //nmi_tuner_os_log("Nmi_Tuner_Interface_init_chip error\n");
        return FALSE;
    }
    else
    {
        chipid = Nmi_Tuner_Interface_Get_ChipID();

        return TRUE;
    }
}

uint8_t MDrv_NMI120_Tuner_SetTuner(uint32_t dwFreq, uint8_t ucBw, uint8_t type) //MHZ
{
    tNMI_TUNE_PARAM param;
    uint8_t ret = FALSE;
    nmi_tuner_os_memset((void *)&param, 0x00, sizeof(tNMI_TUNE_PARAM));
    nmi_tuner_os_log("nmi %x\n", type);
    if (type != NMI_DEMOD_TYPE_NTSC)
    {
        param.freq = dwFreq * 1000;
        param.if_freq = 4800000;
        param.dacsel =  nDac2;
        param.if_freq_invert = FALSE;
        param.freq_invert = FALSE;
        param.ucBw = ucBw;
        param.output = nIf;
        param.freq_invert = FALSE;
        if (type == NMI_DEMOD_TYPE_8VSB)
        {
            //param.if_freq_invert = TRUE;
            param.tvstd = NMI_ATSC_8VSB;
            nmi_tuner_os_log("nmi 8vsb\n");
        }
        else if (type == NMI_DEMOD_TYPE_64QAM)
        {
            param.tvstd = NMI_ATSC_64QAM;
            nmi_tuner_os_log("nmi 64qam\n");
        }
        else if (type == NMI_DEMOD_TYPE_256QAM)
        {
            param.tvstd = NMI_ATSC_256QAM;
            nmi_tuner_os_log("nmi 256qam\n");
        }
        else
        {
            param.tvstd = type;
        }
        ret = Nmi_Tuner_Interface_Tuning(&param);
        //nmi_tuner_os_log("after Nmi_Tuner_Interface_Tuning~~~2222222222222222~~~~~~~~~~\n");
        if (type == NMI_DEMOD_TYPE_8VSB)
        {
            Nmi_Tuner_Interface_Wreg(0x05, 0x85);
            //Nmi_Tuner_Interface_Wreg(0x35,0x54);
            //Nmi_Tuner_Interface_Wreg(0x164,0x500);
            Nmi_Tuner_Interface_Wreg(0x35, 0x58);
            Nmi_Tuner_Interface_Wreg(0x164, 0x350);
        }
        else if (ISNM120(chipid))
        {
            //nmi_tuner_os_log("after Nmi_Tuner_Interface_Tuning~~~11111111111111~~~~~~~~~~\n");
            /*if(((dwFreq> 808000) && (dwFreq < 812000))||
                ((dwFreq > 816000) && (dwFreq < 820000))||
                ((dwFreq > 840000) && (dwFreq < 844000)))
            {
                Nmi_Tuner_Interface_Wreg(0x35,0x18);
                Nmi_Tuner_Interface_Wreg(0x164,0x399);
            }
            else
            {
                Nmi_Tuner_Interface_Wreg(0x35,0x53);
                Nmi_Tuner_Interface_Wreg(0x164,0x399);
            }*/

            Nmi_Tuner_Interface_Wreg(0x35, 0x18); //200
            Nmi_Tuner_Interface_Wreg(0x164, 0x399);

        }
        else
        {
            //Nmi_Tuner_Interface_Wreg(0x35,0x57);
            //Nmi_Tuner_Interface_Wreg(0x164,0x600);
            Nmi_Tuner_Interface_Wreg(0x35, 0x58);
            Nmi_Tuner_Interface_Wreg(0x164, 0x350);
        }

    }
    else
    {
        if (ISNM131(chipid))
        {
            param.freq = dwFreq * 1000;
            param.dacsel =  nDacDefault;
            param.if_freq_invert = FALSE;
            param.freq_invert = FALSE;
            param.output = nCvbsSif;
            param.is_stereo = 0;
            param.tvstd = NMI_NTSC;
            param.freq_invert = FALSE;
            nmi_tuner_os_log("131ntsc\n");
            ret = Nmi_Tuner_Interface_Tuning(&param);
            nmi_tuner_os_log("ret=%x\n", ret);
            if (ret != FALSE)
            {
                ret = TRUE;
            }
        }
        else
        {
            nmi_tuner_os_log("this is not NM131,can't surport NTSC CVBS\n");
            return FALSE;
        }
    }
    return ret;

}

MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq, MS_U8 ucBw)
{
    uint8_t type = NMI_DVBT2;
    if (E_TUNE_TYPE_T2 == MDrv_Tuner_GetTuneType())
        type = NMI_DVBT2;
    else
        type = NMI_DVBT;
    nmi_tuner_os_log("120 set tuner dwFreq(%d),ucBw(%d)\n", dwFreq, ucBw);
    return MDrv_NMI120_Tuner_SetTuner(dwFreq, ucBw, type);
}

int NMI120_GetLockStatus(void)
{
    tTnrStatus status;

    if (nmi_drv_ctl(NMI_DRV_GET_STATUS, (void *)&status) < 0)
    {
        return 0;
    }

    return status.ds.agclock;
}
static int ABS(int x)
{
    return x < 0 ? -x : x;
}

int NMI120_GetRSSI(uint8_t outputChoice)
{
    int rssi;
    tTnrStatus status;
    //NMI120_ShowAsicPara();
    if (nmi_drv_ctl(NMI_DRV_GET_STATUS, (void *)&status) < 0)
    {
        return 0;
    }

    rssi = (int)status.gain.rssix100 / 100;
    if (1 == outputChoice) // for percent value
    {
        if ((rssi <= 0) && (rssi >= -100))
        {
            rssi = ABS(rssi);
            if (rssi > 85)
                rssi = 0;
            else if (rssi > 45)
                rssi = (85 - rssi) * 100 / 40;
            else
                rssi = 100;
        }
        else
        {
            if (rssi > 0)
            {
                rssi = 100;
            }

            if (rssi < -100)
            {
                rssi = 0;
            }
        }
    }
//    nmi_tuner_os_log("[NMI]SSI=%d\n",rssi);
    return rssi;
}


uint8_t MDrv_Tuner_PowerOnOff(uint8_t bPowerOn)
{
    if (bPowerOn == FALSE)
    {
        //if need loop through, passing LOOP_THROUGH,else passing SIGLE_IN
        //tTnrStatus status;

        //nmi_drv_ctl(NMI_DRV_SLEEP,NULL);
        Nmi_Tuner_Interface_Sleep_Lt();
    }
    return TRUE;
}

#endif
