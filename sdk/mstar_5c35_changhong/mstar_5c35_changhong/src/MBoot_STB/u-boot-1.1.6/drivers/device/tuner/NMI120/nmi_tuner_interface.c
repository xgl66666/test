#include <common.h>
#include <frontend_config.h>
#if(FRONTEND_TUNER_TYPE == TUNER_NMI120 )
#include "sys/common/MsOS.h"
#include "./nmi_tuner_interface.h"
#include "nmi_tuner_os.h"
#include "nmicmn.h"
typedef struct
{
    uint32_t chip_id;
    /*bSleep:if TRUE,it shows the tuner has sleep.You must stop the mosaic_reset operation.
             if FALSE,it shows the tuner is working.The mosaic_reset operation can work */
    bool_t    bSleep;
    /*bInit:This param indicates the tuner is inited,when retune the tuner,you don't need to wait for 4s*/
    bool_t  bInit;
    bool_t     btuned;//FALSE no tune
    bool_t   bdemodlock;
    tNMI_TUNE_PARAM currentparam;
    tNMI_TUNE_PARAM orgparam;
    uint32_t  jiffies;
    uint32_t  mutex;
} tNmiInterfaceGlobal;

#define NML120_MAX_RESET_WAITTIME 4000
#define NML120_MAX_OFFSET           660 //KHz
#define TUNER_NML120_MAX_TIME 15
static tNmiInterfaceGlobal gChipNowInfo;

extern uint8_t rReg8(uint8_t adr);
extern uint32_t rReg32(uint16_t adr);
extern void wReg8(uint8_t adr, uint8_t val);
extern void wReg32(uint16_t adr, uint32_t val);
extern void nmi_demod_aci_config(uint8_t aci);
extern void nm131_demod_T_config(tNmiTvStd std, bool_t enable);
extern void nm120_rfli_track(void);


static uint8_t Nmi_Tuner_Interface_LoopThrough(void)
{
    //int result;
    tTnrInit cfg;
    tTnrLtCtrl ltCtl;
    //tTnrStatus status;

    nmi_tuner_os_memset((void *)&cfg, 0, sizeof(tTnrInit));

    ltCtl.enable = TRUE;
    if (nmi_drv_ctl(NMI_DRV_LT_CTRL, (void *)&ltCtl) < 0)
    {
        return FALSE;
    }

    nmi_drv_ctl(NMI_DRV_WAKE_UP_LT, NULL);
    return TRUE;
}


uint32_t Nmi_Tuner_Interface_init_chip(tTnrInit* pcfg)
{
    int result;
    nmi_tuner_os_memset((void *)&gChipNowInfo, 0, sizeof(tNmiInterfaceGlobal));

    nmi_tuner_os_chip_power_on();
    nmi_tuner_os_chip_enable();

    result = nmi_drv_ctl(NMI_DRV_INIT, (void *)pcfg);

    if (result < 0)
    {
        nmi_tuner_os_log(">> NMI_DRV_INIT failed. result = %d\n", result);
        gChipNowInfo.bInit = FALSE;
        return FALSE;
    }
    //Leo else
    {
        uint32_t chipid;
        tNmiDriverVer ver;

        nmi_drv_ctl(NMI_DRV_VERSION, &ver);
        nmi_tuner_os_log("[NMI] driver version (%d.%d.%d.%d) build %d\n", ver.major, ver.minor, ver.rev1, ver.rev2, ver.buildrev);

        nmi_drv_ctl(NMI_DRV_GET_CHIPID, &chipid);
        nmi_tuner_os_log("[NMI] Chip ID:");
        nmi_tuner_os_log("%4x:", (uint16_t)(chipid >> 16));
        nmi_tuner_os_log("%4x\n", (uint16_t)((chipid << 16) >> 16));
        gChipNowInfo.chip_id =  chipid;


    }

    gChipNowInfo.bSleep = FALSE;
    gChipNowInfo.bInit =    TRUE;
    gChipNowInfo.btuned = FALSE;
    gChipNowInfo.jiffies = nmi_tuner_os_get_tick();

    if (ISNM120(gChipNowInfo.chip_id)) //only 120
    {
        Nmi_Tuner_Interface_LoopThrough();
        nmi_tuner_os_log("before nmi_tuner_os_create_task\n");
    }

    return TRUE;
}
uint32_t Nmi_Tuner_Interface_deinit_chip(void)
{
    if (ISNM120(gChipNowInfo.chip_id)) //only 120
    {
        nmi_tuner_os_log("Nmi_Tuner_Interface_deinit_chip\n");
    }

    nmi_drv_ctl(NMI_DRV_DEINIT, NULL);
    nmi_tuner_os_chip_power_off();
    gChipNowInfo.bSleep = FALSE;
    gChipNowInfo.bInit =   FALSE;
    gChipNowInfo.btuned = FALSE;
    return TRUE;
}
static uint8_t Nmi_Tuner_Interface_SelfTune(tNMI_TUNE_PARAM* param)
{
    uint8_t ret = TRUE;
    tTnrTune tune;
    nmi_tuner_os_memset((void *)&tune, 0x00, sizeof(tTnrTune));
    tune.aif = param->aif;
    tune.dacSel = param->dacsel;
    tune.is_stereo = param->is_stereo;
    tune.output = param->output;
    tune.rf = param->freq;
    tune.vif = param->if_freq;
    tune.rfinvert = param->freq_invert;
    //tune.bretune = param->bretune;
    if (gChipNowInfo.bInit != TRUE)
    {
        return FALSE;
    }

    switch (param->tvstd)
    {
    case NMI_NTSC:
    {
        //add codes
        tune.std = nNTSC;
    }
    break;
    case NMI_PAL_G:
    {
        //add codes
    }
    break;
    case NMI_PAL_M:
    {
        //add codes
    }
    break;
    case NMI_PAL_N:
    {
        //add codes
    }
    break;
    case NMI_PAL_K:
    {
        //add codes
    }
    break;
    case NMI_PAL_L:
    {
        //add codes
    }
    break;
    case NMI_PAL_D:
    {
        //add codes
    }
    break;
    case NMI_SECAM_L:
    {
        //add codes
    }
    break;
    case NMI_SECAM_B:
    {
        //add codes
    }
    break;
    case NMI_SECAM_D:
    {
        //add codes
    }
    break;
    case NMI_ATSC_8VSB:
    case NMI_ATSC_64QAM:
    case NMI_ATSC_256QAM:
    {
        //add codes
        nmi_tuner_os_log("mode NMI_ATSC %d\n", param->ucBw);
        switch (param->ucBw)
        {
        case 0:
        case 6:
            tune.std = nDTV_6;
            break;
        case 1:
        case 7:
            tune.std = nDTV_7;
            break;
        case 2:
        case 8:
        default:
            tune.std = nDTV_8;
            break;
        }
    }
    break;
    case NMI_DVBT:
    case NMI_DVBT2:
    {
        //add codes
        switch (param->ucBw)
        {
        case 0:
        case 6:
            tune.std = nDTV_6;
            break;
        case 1:
        case 7:
            tune.std = nDTV_7;
            break;
        case 2:
        case 8:
        default:
            tune.std = nDTV_8;
            break;
        }
    }
    break;
    case NMI_DVBC:
    {
        //add codes
        uint32_t tempfreq;
        tempfreq = tune.rf / 1000000;
        if (tempfreq % 3 != 0)
        {
            tune.rf = tune.rf + 1000;
        }

        switch (param->ucBw)
        {
        case 0:
        case 6:
            tune.std = nDTV_6;
            break;
        case 1:
        case 7:
            tune.std = nDTV_7;
            break;
        case 2:
        case 8:
        default:
            tune.std = nDTV_8;
            break;
        }
    }
    break;
    case NMI_DTMB:
    {
        //add codes
        switch (param->ucBw)
        {
        case 0:
        case 6:
            tune.std = nDTV_6;
            break;
        case 1:
        case 7:
            tune.std = nDTV_7;
            break;
        case 2:
        case 8:
        default:
            tune.std = nDTV_8;
            break;
        }
    }
    break;
    case NMI_ISDBT:
    {
        //add codes
    }
    break;
    default:
    {
        //add codes
    }
    break;
    }

    if (param->tvstd <= NMI_SECAM_D && (param->output == nCvbsSif || param->output == nCvbsBBAud))
    {
        if (nmi_drv_ctl(NMI_DRV_TUNE, &tune) == 0)
        {
            nmi_tuner_os_log("\n tuneFail......\n");
            ret =  FALSE;
        }
    }
    else
    {
        if (nmi_drv_ctl(NMI_DRV_TUNE, &tune) < 0)
        {
            nmi_tuner_os_log("\n tuneFail......\n");
            ret = FALSE;
        }

        if (param->if_freq_invert == TRUE)
        {
            int spectrum = 1;
            nmi_drv_ctl(NMI_DRV_INVERT_SPECTRUM, (void *)(&spectrum));
            nmi_tuner_os_log("\n swan if invert \n");
        }
    }
    if (param->tvstd == NMI_DVBC) //yao jin  masic bugs  //swan 2012_5_14  honestar
    {
        uint32_t tempfreq;
        tempfreq = tune.rf / 1000000;
        if (tempfreq % 3 == 0)
        {
            Nmi_Tuner_Interface_Wreg(0x1b, 0x08);
        }
        else
        {
            Nmi_Tuner_Interface_Wreg(0x1b, 0x0e);
        }
    }
    if (param->tvstd == NMI_DVBT || param->tvstd == NMI_DVBT2) //yao jin  masic bugs  //swan 2012_5_14  honestar
    {
        if ((tune.rf > 191500000 - 1500000) && (tune.rf < 191500000 + 1500000))
        {
            Nmi_Tuner_Interface_Wreg(0x05, 0x87);
        }
    }
    nmi_tuner_os_log("gChipNowInfo.currentparam.freq(%d) \n", gChipNowInfo.currentparam.freq);
    nmi_tuner_os_memcpy((void*)&gChipNowInfo.currentparam, param, sizeof(tNMI_TUNE_PARAM));
    return ret;
}


uint8_t Nmi_Tuner_Interface_Tuning(tNMI_TUNE_PARAM* param)
{
    uint8_t ret;
    gChipNowInfo.bdemodlock = FALSE;
    ret = Nmi_Tuner_Interface_SelfTune(param);
    gChipNowInfo.btuned = TRUE;
    nmi_tuner_os_memcpy((void*)&gChipNowInfo.orgparam, param, sizeof(tNMI_TUNE_PARAM));
    return ret;
}

int Nmi_Tuner_Interface_GetLockStatus(void)
{
    tTnrStatus status;

    if (nmi_drv_ctl(NMI_DRV_GET_STATUS, &status) < 0)
    {
        return 0;
    }

    return status.ds.agclock;
}
int16_t Nmi_Tuner_Interface_GetRSSI(void)
{
    int16_t rssi;
    tTnrStatus status;

    if (nmi_drv_ctl(NMI_DRV_GET_STATUS, &status) < 0)
    {
        return 0;
    }

    rssi = (int16_t)status.gain.rssix100 / 100;

    return rssi;
}

void Nmi_Tuner_Interface_Sleep_Lt(void)
{
    gChipNowInfo.bSleep = TRUE;
    nmi_tuner_os_log("Nmi_Tuner_Interface_Sleep_Lt ~~~~~~~~~~~~\n");
    nmi_drv_ctl(NMI_DRV_SLEEP_LT, NULL);
}

void Nmi_Tuner_Interface_Wake_Up_Lt(void)
{
    gChipNowInfo.bSleep = FALSE;

    nmi_drv_ctl(NMI_DRV_WAKE_UP_LT, NULL);
    if (ISNM120(gChipNowInfo.chip_id)) //only 120
    {
    }

}

void  Nmi_Tuner_Interface_Wreg(uint32_t addr, uint32_t value)
{
    if (addr < 0x100)
    {
        wReg8((uint8_t)addr, (uint8_t)value);
    }
    else
    {
        wReg32(addr, value);
    }
}

uint32_t  Nmi_Tuner_Interface_Rreg(uint32_t addr)
{
    if (addr < 0x100)
    {
        return rReg8((uint8_t)addr);
    }
    else
    {
        return rReg32(addr);
    }
}

uint32_t Nmi_Tuner_Interface_Get_ChipID(void)
{
    if (gChipNowInfo.bInit != TRUE)
    {
        return 0;
    }
    return gChipNowInfo.chip_id;
}

void Nmi_Tuner_Interface_Demod_Lock(void)
{
    if (gChipNowInfo.bInit != TRUE)
    {
        return;
    }
    if (gChipNowInfo.currentparam.tvstd == NMI_ATSC_64QAM || gChipNowInfo.currentparam.tvstd == NMI_ATSC_256QAM)
    {
        if (gChipNowInfo.bdemodlock == FALSE)
        {
            nmi_demod_aci_config(0x01);
            gChipNowInfo.bdemodlock = TRUE;
        }
    }
}
#endif

