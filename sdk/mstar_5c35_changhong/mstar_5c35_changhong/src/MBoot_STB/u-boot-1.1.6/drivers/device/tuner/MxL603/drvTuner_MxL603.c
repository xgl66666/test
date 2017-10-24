
#include <frontend_config.h>

#if (FRONTEND_TUNER_TYPE == TUNER_MXL603)
#include <common.h>
#include "sys/common/MsTypes.h"
#include "sys/common/MsOS.h"
#include <sys/drv/drvIIC.h>
#include "device/drvTuner.h"

#include "MxL603_TunerApi.h"
#include "MxL603_OEM_Drv.h"

#define EXAMPLE_DEV_MAX   2
#define MXL603_I2C_ADDR   0xC0


/* Increase driving current for IFamp
 */
static MXL_STATUS MDrv_Tuner_IncCurrent(void)
{
    UINT8 status = MXL_SUCCESS;

    status |= MxLWare603_OEM_WriteRegister(MXL603_I2C_ADDR, 0x5B, 0x10);
    status |= MxLWare603_OEM_WriteRegister(MXL603_I2C_ADDR, 0x5C, 0xB1);

    return (MXL_STATUS) status;
}


MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    MS_BOOL ret;
    if (bPowerOn == FALSE)
    {
        ret = MxLWare603_API_CfgDevPowerMode(MXL603_I2C_ADDR, MXL603_PWR_MODE_STANDBY);
    }
    else
    {
        ret = MxLWare603_API_CfgDevPowerMode(MXL603_I2C_ADDR, MXL603_PWR_MODE_ACTIVE);
    }
    if (MxL_ERR_OTHERS == ret)
        return FALSE;
    return TRUE;
}

int MxL603_init_main(void)
{
    MXL_STATUS status;
    UINT8 devId;
    MXL_BOOL singleSupply_3_3V;
    MXL603_XTAL_SET_CFG_T xtalCfg;
    MXL603_IF_OUT_CFG_T ifOutCfg;
    MXL603_AGC_CFG_T agcCfg;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;

    //MXL603_CHAN_TUNE_CFG_T chanTuneCfg;
    //MXL_BOOL refLockPtr = MXL_UNLOCKED;
    //MXL_BOOL rfLockPtr = MXL_UNLOCKED;

    /* If OEM data is implemented, customer needs to use OEM data structure
       related operation. Following code should be used as a reference.
       For more information refer to sections 2.5 & 2.6 of
       MxL603_mxLWare_API_UserGuide document.

      for (devId = 0; devId < EXAMPLE_DEV_MAX; devId++)
      {
        // assigning i2c address for  the device
        device_context[devId].i2c_address = GET_FROM_FILE_I2C_ADDRESS(devId);

        // assigning i2c bus for  the device
        device_context[devId].i2c_bus = GET_FROM_FILE_I2C_BUS(devId);

        // create semaphore if necessary
        device_context[devId].sem = CREATE_SEM();

        // sample stat counter
        device_context[devId].i2c_cnt = 0;

        status = MxLWare603_API_CfgDrvInit(devId, (void *) &device_context[devId]);

        // if you do not want to pass any oem data, just use NULL as a parameter:
        // status = MxLWare603_API_CfgDrvInit(devId, NULL);
      }

    */

    /* If OEM data is not required, customer should treat devId as
     I2C slave Address */
    devId = MXL603_I2C_ADDR;

    //Step 1 : Soft Reset MxL603
    status = MxLWare603_API_CfgDevSoftReset(devId);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgDevSoftReset\n");
        return status;
    }

    //Step 2 : Overwrite Default
    singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;
    status = MxLWare603_API_CfgDevOverwriteDefaults(devId, singleSupply_3_3V);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgDevOverwriteDefaults\n");
        return status;
    }

    //Step 3 : XTAL Setting
    xtalCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    xtalCfg.xtalCap = 0;  //12;
    xtalCfg.clkOutEnable = MXL_DISABLE;
    xtalCfg.clkOutDiv = MXL_DISABLE;
    xtalCfg.clkOutExt = MXL_DISABLE;
    xtalCfg.singleSupply_3_3V = MXL_ENABLE;
    xtalCfg.XtalSharingMode = MXL_DISABLE;
    status = MxLWare603_API_CfgDevXtal(devId, xtalCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgDevXtal\n");
        return status;
    }

    //MxLWare603_API_CfgTunerLoopThrough( devId, MXL_ENABLE);

    //Step 4 : IF Out setting

    ifOutCfg.ifOutFreq = MXL603_IF_5MHz;
    ifOutCfg.manualIFOutFreqInKHz = 5000;
    ifOutCfg.ifInversion = MXL_DISABLE;
    ifOutCfg.gainLevel = 11;     //0-13
    ifOutCfg.manualFreqSet = MXL_DISABLE;//MXL_DISABLE;
    status = MxLWare603_API_CfgTunerIFOutParam(devId, ifOutCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgTunerIFOutParam\n");
        return status;
    }

    //Step 5 : AGC Setting
#if(ENABLE_DVB_C_SYS)
    agcCfg.agcType = MXL603_AGC_SELF;
#else
    agcCfg.agcType = MXL603_AGC_EXTERNAL;
#endif
    agcCfg.setPoint = 66;
    agcCfg.agcPolarityInverstion = MXL_DISABLE;
    status = MxLWare603_API_CfgTunerAGC(devId, agcCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgTunerAGC\n");
        return status;
    }

    //Step 6 : Application Mode setting
    tunerModeCfg.ifOutFreqinKHz = 5000;
#if(ENABLE_DVB_C_SYS)
    tunerModeCfg.signalMode = MXL603_DIG_DVB_C;
#elif ((ENABLE_DVB_T_SYS) || (ENABLE_DVB_T2_SYS))
    tunerModeCfg.signalMode = MXL603_DIG_DVB_T;
#endif

    tunerModeCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    status = MxLWare603_API_CfgTunerMode(devId, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgTunerMode\n");
        return status;
    }

    return MXL_SUCCESS;
}

#if (ENABLE_DVB_T2_SYS || ENABLE_DVB_C_SYS || ENABLE_DVB_T_SYS)
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


MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MXL_STATUS status = MXL_FALSE;
    MXL603_BW_E eBw = ucBw - 6 ;
    MXL603_CHAN_TUNE_CFG_T chanTuneCfg;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;
    MXL_BOOL refLockPtr = MXL_UNLOCKED;
    MXL_BOOL rfLockPtr = MXL_UNLOCKED;
    UINT8 regData = 0;

    //Step 7 : Channel mode & bandwidth & frequency setting
#if(ENABLE_DVB_C_SYS)
    chanTuneCfg.signalMode = MXL603_DIG_DVB_C;
#elif ((ENABLE_DVB_T_SYS) || (ENABLE_DVB_T2_SYS))
    chanTuneCfg.signalMode = MXL603_DIG_DVB_T;
#endif

    switch (chanTuneCfg.signalMode)
    {
    case MXL603_DIG_DVB_C:
    {
        switch (eBw)
        {
        case MXL603_CABLE_BW_6MHz:
        case MXL603_CABLE_BW_7MHz:
        case MXL603_CABLE_BW_8MHz:
        {
            chanTuneCfg.bandWidth = eBw;
            break;
        }
        default:
        {
            chanTuneCfg.bandWidth = MXL603_CABLE_BW_8MHz;
            break;
        }
        }
        break;
    }
    case MXL603_DIG_DVB_T:
    {
        eBw = eBw + MXL603_TERR_BW_6MHz;
        switch (eBw)
        {
        case MXL603_TERR_BW_6MHz:
        case MXL603_TERR_BW_7MHz:
        case MXL603_TERR_BW_8MHz:
        {
            chanTuneCfg.bandWidth = eBw;
            break;
        }
        default:
        {
            chanTuneCfg.bandWidth = MXL603_TERR_BW_8MHz;
            break;
        }
        }
        break;
    }
    default:
    {
        chanTuneCfg.bandWidth = MXL603_TERR_BW_8MHz;
        break;
    }
    }

    //Step 8 : Final Application Mode setting
    tunerModeCfg.ifOutFreqinKHz = 5000;
#if (ENABLE_DVB_C_SYS)
    tunerModeCfg.signalMode = MXL603_DIG_DVB_C;
#elif ((ENABLE_DVB_T_SYS) || (ENABLE_DVB_T2_SYS))
    tunerModeCfg.signalMode = MXL603_DIG_DVB_T;
#endif

    tunerModeCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    status = MxLWare603_API_CfgTunerMode(MXL603_I2C_ADDR, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgTunerMode\n");
        return FALSE;
    }
    /* Only T needs to increase pad cuurent*/
    if (E_TUNE_TYPE_T == TunerMode.Fetype)
    {
        status = MDrv_Tuner_IncCurrent();
        if (status != MXL_SUCCESS)
        {
            printf("Error! MDrv_Tuner_IncCurrent\n");
        }
    }

    chanTuneCfg.freqInHz = dwFreq * 1000;
    chanTuneCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
#if (ENABLE_DVB_C_SYS)
    status |= MxLWare603_OEM_WriteRegister(MXL603_I2C_ADDR, 0xC2, 0xA8);
#endif
    status = MxLWare603_API_CfgTunerChanTune(MXL603_I2C_ADDR, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        printf("Error! MxLWare603_API_CfgTunerChanTune\n");
        return FALSE;
    }

    // Wait 15 ms
    MxLWare603_OEM_Sleep(15);

    // Read back Tuner lock status
    status = MxLWare603_API_ReqTunerLockStatus(MXL603_I2C_ADDR, &rfLockPtr, &refLockPtr);
    if (status == MXL_TRUE)
    {
        if ((MXL_LOCKED == rfLockPtr) && (MXL_LOCKED == refLockPtr))
        {
            printf("Tuner locked\n");

            MxLWare603_OEM_ReadRegister(MXL603_I2C_ADDR, 0x5B, &regData);
            printf("MxL603_0x5B : %x\n", regData);
            MxLWare603_OEM_ReadRegister(MXL603_I2C_ADDR, 0x5C, &regData);
            printf("MxL603_0x5C : %x\n", regData);
            MxLWare603_OEM_ReadRegister(MXL603_I2C_ADDR, 0xC2, &regData);
            printf("MxL603_0xC2 : %x\n", regData);
        }
        else
        {
            printf("Tuner unlocked\n");
        }
    }

    return TRUE;
}

MS_BOOL MDrv_Tuner_Init(void)
{
    if (MxL603_init_main() == MXL_SUCCESS)
    {
        printf("\n MXL603 INIT OK\n");
        return TRUE;
    }
    else
    {
        printf("\n MXL603 INIT FAILED\n");
        return FALSE;
    }
}
#endif

