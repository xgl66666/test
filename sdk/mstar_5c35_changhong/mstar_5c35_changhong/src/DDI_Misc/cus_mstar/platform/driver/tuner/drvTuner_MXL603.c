#include "MsCommon.h"
#include "Board.h"
#include "drvIIC.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)

#include "MxL603_TunerApi.h"
#include "MxL603_OEM_Drv.h"
#include "MxL603_TunerCfg.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];


#define EXAMPLE_DEV_MAX   2
#define MXL603_I2C_ADDR   0xC0

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
/*MS_U8 dmdConfig[] =
{
    0x00
};
*/

//MS_BOOL MDrv_Tuner_Init(void);

/* Example of OEM Data, customers should have
below data structure declared at their appropriate
places as per their software design

typedef struct
{
  UINT8   i2c_address;
  UINT8   i2c_bus;
  sem_type_t sem;
  UINT16  i2c_cnt;
} user_data_t;

user_data_t device_context[EXAMPLE_DEV_MAX];

*/

typedef enum
{
    E_DEVICE_DEMOD_ATV = 0,
    E_DEVICE_DEMOD_DVB_T,
    E_DEVICE_DEMOD_DVB_C,
    E_DEVICE_DEMOD_DVB_S,
    E_DEVICE_DEMOD_DTMB,
    E_DEVICE_DEMOD_ATSC,
    E_DEVICE_DEMOD_ATSC_VSB,
    E_DEVICE_DEMOD_ATSC_QPSK,
    E_DEVICE_DEMOD_ATSC_16QAM,
    E_DEVICE_DEMOD_ATSC_64QAM,
    E_DEVICE_DEMOD_ATSC_256QAM,
    E_DEVICE_DEMOD_DVB_T2,
    E_DEVICE_DEMOD_MAX,
    E_DEVICE_DEMOD_NULL = E_DEVICE_DEMOD_MAX,

} EN_DEVICE_DEMOD_TYPE;

/* Increase driving current for IFamp
 */
static MXL_STATUS MDrv_Tuner_IncCurrent(MS_U8 u8TunerIndex)
{
    UINT8 status = MXL_SUCCESS;

    status |= MxLWare603_OEM_WriteRegister(u8TunerIndex,MXL603_I2C_ADDR, 0x5B, 0x10);
    status |= MxLWare603_OEM_WriteRegister(u8TunerIndex,MXL603_I2C_ADDR, 0x5C, 0xB1);

    return (MXL_STATUS) status;
}

MS_BOOL MDrv_Tuner_PowerOnOff(MS_U8 u8TunerIndex,MS_BOOL bPowerOn)
{
    return TRUE;
}

int MxL603_init_main(MS_U8 u8TunerIndex)
{
    MXL_STATUS status;
    UINT8 devId;
    MXL_BOOL singleSupply_3_3V;
    MXL603_XTAL_SET_CFG_T xtalCfg;
    MXL603_IF_OUT_CFG_T ifOutCfg;
    MXL603_AGC_CFG_T agcCfg;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;

    tunerModeCfg.signalMode = MXL603_DIG_DVB_C; //init value
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

    // if you don’t want to pass any oem data, just use NULL as a parameter:
    // status = MxLWare603_API_CfgDrvInit(devId, NULL);
    }

    */

    /* If OEM data is not required, customer should treat devId as
     I2C slave Address */
    devId = MXL603_I2C_ADDR;

    //Step 1 : Soft Reset MxL603
    status = MxLWare603_API_CfgDevSoftReset(u8TunerIndex,devId);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgDevSoftReset\n"));
        return status;
    }

    //Step 2 : Overwrite Default
    singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;
    status = MxLWare603_API_CfgDevOverwriteDefaults(u8TunerIndex,devId, singleSupply_3_3V);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgDevOverwriteDefaults\n"));
        return status;
    }

    //Step 3 : XTAL Setting
    xtalCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    xtalCfg.xtalCap = 0;//12;
    xtalCfg.clkOutEnable = MXL_DISABLE;
    xtalCfg.clkOutDiv = MXL_DISABLE;
    xtalCfg.clkOutExt = MXL_DISABLE;
    xtalCfg.singleSupply_3_3V = MXL_ENABLE;
    xtalCfg.XtalSharingMode = MXL_DISABLE;
    status = MxLWare603_API_CfgDevXtal(u8TunerIndex,devId, xtalCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgDevXtal\n"));
        return status;
    }

    //MxLWare603_API_CfgTunerLoopThrough( devId, MXL_ENABLE);

    //Step 4 : IF Out setting
    ifOutCfg.ifOutFreq = MXL603_IF_5MHz;//MXL603_IF_4_57MHz;//MXL603_IF_36_15MHz;//MXL603_IF_6Hz;
#if (FRONTEND_DEMOD_IQ_SWAP == 1)
    ifOutCfg.ifInversion = MXL_ENABLE;//MXL_ENABLE;
#else
    ifOutCfg.ifInversion = MXL_DISABLE;//MXL_ENABLE;
#endif
    #if defined(KGC_USE_ON_CHIP_MSB1237) && (KGC_USE_ON_CHIP_MSB1237 == 1)
    ifOutCfg.gainLevel = 13;//3;
    #else
    ifOutCfg.gainLevel = 11;//3;
    #endif
    ifOutCfg.manualFreqSet = MXL_DISABLE;//MXL_DISABLE;
    ifOutCfg.manualIFOutFreqInKHz = 5000;//4570;
    status = MxLWare603_API_CfgTunerIFOutParam(u8TunerIndex,devId, ifOutCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgTunerIFOutParam\n"));
        return status;
    }

    //Step 5 : AGC Setting
    agcCfg.agcType = MXL603_AGC_EXTERNAL;
    agcCfg.setPoint = 66;
    agcCfg.agcPolarityInverstion = MXL_DISABLE;
    status = MxLWare603_API_CfgTunerAGC(u8TunerIndex,devId, agcCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgTunerAGC\n"));
        return status;
    }

    //Step 6 : Application Mode setting
    if (*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBC)
    {
        tunerModeCfg.signalMode = MXL603_DIG_DVB_C;
	}
	else if ((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT)||(*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == DTMB))
	{
		tunerModeCfg.signalMode = MXL603_DIG_DVB_T;
	}
	else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == ISDBT|| *InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_ISDBT||*InitParam[u8TunerIndex].pCur_Broadcast_type == ATSC)
	{
		tunerModeCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
	}
	else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == J83B)
    {
        tunerModeCfg.signalMode = MXL603_DIG_J83B;
	}

    tunerModeCfg.ifOutFreqinKHz = 5000;//4570;//6000;//4000;
    tunerModeCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    #if defined(KGC_USE_ON_CHIP_MSB1237) && (KGC_USE_ON_CHIP_MSB1237 == 1)
    tunerModeCfg.ifOutGainLevel = 13;
    #else
    tunerModeCfg.ifOutGainLevel = 11;
    #endif
    status = MxLWare603_API_CfgTunerMode(u8TunerIndex,devId, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgTunerMode\n"));
        return status;
    }

    // To Change Channel, GOTO Step #7

    // To change Application mode settings, GOTO Step #6

    return status;
}

MS_BOOL MDrv_Tuner_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam)
{

    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

    if(MxL603_init_main(u8TunerIndex) == MXL_SUCCESS)
    {
        TUNER_ERR(("\n MXL603 INIT OK\n"));
        return TRUE;
    }
    else
    {
        TUNER_ERR(("\n MXL603 INIT FAILED\n"));
        return FALSE;
    }
}

/*
MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{
    return 0;
}

MS_U32 MDrv_Tuner_Stand_By(void)
{
    return 0;
}

MS_U32 MDrv_Tuner_Wake_Up(void)
{
    return 0;
}
*/

MS_BOOL MDrv_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    UINT8 devId = MXL603_I2C_ADDR;
    MXL_STATUS status = MXL_FALSE;
    MXL603_BW_E eBw = 0;//(ucBw - 6);
    MXL603_CHAN_TUNE_CFG_T chanTuneCfg;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;
    MXL_BOOL refLockPtr = MXL_UNLOCKED;
    MXL_BOOL rfLockPtr = MXL_UNLOCKED;
    UINT8 regData = 0;

    switch (ucBw)
    {
        case TUNER_BW_MODE_6MHZ:
            eBw = MXL603_CABLE_BW_6MHz;
            break;
        case TUNER_BW_MODE_7MHZ:
            eBw = MXL603_CABLE_BW_7MHz;
            break;
        case TUNER_BW_MODE_8MHZ:
            eBw = MXL603_CABLE_BW_8MHz;
            break;
        default:
           eBw = MXL603_CABLE_BW_8MHz;
           TUNER_DBG(("[warning] Bandwidth enum:%x not support using defalut 8Mhz\n",ucBw));
           break;
    }

    chanTuneCfg.signalMode = MXL603_DIG_DVB_C;  //init value
    tunerModeCfg.signalMode = MXL603_DIG_DVB_C; //init value

    //Step 7 : Channel mode & bandwidth & frequency setting
    if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBC)
    {
        chanTuneCfg.signalMode = MXL603_DIG_DVB_C;
    }
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == J83B)
    {
        chanTuneCfg.signalMode = MXL603_DIG_J83B;
        eBw = MXL603_CABLE_BW_6MHz;
    }
    else if ((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT)||(*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == DTMB))
    {
        chanTuneCfg.signalMode = MXL603_DIG_DVB_T;
    }
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == ISDBT||*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_ISDBT||*InitParam[u8TunerIndex].pCur_Broadcast_type == ATSC)
    {
        chanTuneCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
    }


    switch(chanTuneCfg.signalMode)
    {
        case MXL603_DIG_DVB_C:
        case MXL603_DIG_J83B:
        {
            switch(eBw)
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
            switch(eBw)
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
        case MXL603_DIG_ISDBT_ATSC:
        {
            eBw = eBw + MXL603_TERR_BW_6MHz;
            switch(eBw)
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
                    chanTuneCfg.bandWidth = MXL603_TERR_BW_6MHz;
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
    if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBC)
    {
        tunerModeCfg.signalMode = MXL603_DIG_DVB_C;
    }
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == J83B)
    {
        tunerModeCfg.signalMode = MXL603_DIG_J83B;
    }
    else if ((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT)||(*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == DTMB))
    {
        tunerModeCfg.signalMode = MXL603_DIG_DVB_T;
    }
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == ISDBT||*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_ISDBT||*InitParam[u8TunerIndex].pCur_Broadcast_type == ATSC)
    {
        tunerModeCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
    }


    tunerModeCfg.ifOutFreqinKHz = 5000;//4570;//6000;//4000;
    tunerModeCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    #if defined(KGC_USE_ON_CHIP_MSB1237) && (KGC_USE_ON_CHIP_MSB1237 == 1)
    tunerModeCfg.ifOutGainLevel = 13;
    #else
    tunerModeCfg.ifOutGainLevel = 11;
    #endif
    status = MxLWare603_API_CfgTunerMode(u8TunerIndex,devId, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgTunerMode\n"));
        return FALSE;
    }

	/* Only T needs to increase pad cuurent*/
	if ((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT)|| (*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_DVBT))
	{
		status = MDrv_Tuner_IncCurrent(u8TunerIndex);
		if(status != MXL_SUCCESS)
		{
			TUNER_ERR(("Error! MDrv_Tuner_IncCurrent\n"));
			return FALSE;
		}
	}


    chanTuneCfg.freqInHz = dwFreq * 1000;
    chanTuneCfg.xtalFreqSel = MXL603_XTAL_24MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
    if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBC)
    {
        status |= MxLWare603_OEM_WriteRegister(u8TunerIndex,devId, 0xC2, 0xA8);
    }
    status = MxLWare603_API_CfgTunerChanTune(u8TunerIndex,devId, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare603_API_CfgTunerChanTune\n"));
        return FALSE;
    }

    // Wait 15 ms
    MxLWare603_OEM_Sleep(15);

    // Read back Tuner lock status
    status = MxLWare603_API_ReqTunerLockStatus( u8TunerIndex,devId, &rfLockPtr, &refLockPtr);
    if (status == MXL_TRUE)
    {
        if ((MXL_LOCKED == rfLockPtr) && (MXL_LOCKED == refLockPtr))
        {
            TUNER_DBG(("Tuner locked\n"));

            MxLWare603_OEM_ReadRegister(u8TunerIndex,devId, 0x5B, &regData);
            TUNER_DBG( ("MxL603_0x5B : %x\n", regData));
            MxLWare603_OEM_ReadRegister(u8TunerIndex,devId, 0x5C, &regData);
            TUNER_DBG( ("MxL603_0x5C : %x\n", regData));
            MxLWare603_OEM_ReadRegister(u8TunerIndex,devId, 0xC2, &regData);
            TUNER_DBG( ("MxL603_0xC2 : %x\n", regData));
        }
        else
        {
            TUNER_DBG(("Tuner unlocked\n"));
        }
    }

    return TRUE;
}

#define MXL603_CHIP_ID 0x01
MS_BOOL MXL603_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    UINT8 regData = 0;

    if(MxLWare603_OEM_ReadRegister(u8TunerIndex,MXL603_I2C_ADDR, 0x18, &regData)!=MXL_TRUE )
    {
        TUNER_ERR(("[mxl603] Read chip ID fail \n"));
    }

    TUNER_DBG(("[mxl603] read id =0x%x\n",regData));

    if(regData == MXL603_CHIP_ID)
    {
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
        return TRUE;
    }
    else
        return FALSE;
}

MS_BOOL MXL603_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* u32IF_Freq)
{
    UINT16 ifFcw;
    UINT8 status = MXL_SUCCESS;
    UINT8 readData = 0;

  // Read back register for manual IF Out
  status |= MxLWare603_OEM_ReadRegister(u8TunerIndex,MXL603_I2C_ADDR, IF_FREQ_SEL_REG, &readData);
     // IF out manual setting : bit<5>
  if((readData & 0x20) == 0x20)
  {
      //Get low 8 bit
      status |= MxLWare603_OEM_ReadRegister(u8TunerIndex,MXL603_I2C_ADDR, IF_FCW_LOW_REG, &readData);
      ifFcw = (UINT16)readData;
      // Get high 4 bit
      status |= MxLWare603_OEM_ReadRegister(u8TunerIndex,MXL603_I2C_ADDR, IF_FCW_HIGH_REG, &readData);
      ifFcw |= ((UINT16)(readData) & 0x0f) << 8;
      *u32IF_Freq = (MS_U32)((ifFcw * 216000)/8192);
  }
  else
  {
      // IF Freq <4:0>
      status |= MxLWare603_OEM_ReadRegister(u8TunerIndex,MXL603_I2C_ADDR, IF_FREQ_SEL_REG, &readData);
      readData &= 0x1f;
      switch(readData)
      {
          case MXL603_IF_3_65MHz:
              *u32IF_Freq = 3650;
              break;
          case MXL603_IF_4MHz:
              *u32IF_Freq = 4000;
              break;
          case MXL603_IF_4_1MHz:
              *u32IF_Freq = 4100;
              break;
          case MXL603_IF_4_15MHz:
              *u32IF_Freq = 4150;
              break;
          case MXL603_IF_4_5MHz:
              *u32IF_Freq = 4500;
              break;
          case MXL603_IF_4_57MHz:
              *u32IF_Freq = 4570;
              break;
          case MXL603_IF_5MHz:
              *u32IF_Freq = 5000;
              break;
          case MXL603_IF_5_38MHz:
              *u32IF_Freq = 5380;
              break;
          case MXL603_IF_6MHz:
              *u32IF_Freq = 6000;
              break;
          case MXL603_IF_6_28MHz:
              *u32IF_Freq = 6280;
              break;
          case MXL603_IF_7_2MHz:
               *u32IF_Freq = 7200;
              break;
          case MXL603_IF_8_25MHz:
              *u32IF_Freq = 8250;
              break;
          case MXL603_IF_35_25MHz:
              *u32IF_Freq = 35250;
              break;
          case MXL603_IF_36MHz:
              *u32IF_Freq = 36000;
              break;
          case MXL603_IF_36_15MHz:
              *u32IF_Freq = 36150;
              break;
          case MXL603_IF_36_65MHz:
              *u32IF_Freq = 36650;
              break;
          case MXL603_IF_44MHz:
              *u32IF_Freq = 44000;
              break;
          default:
              *u32IF_Freq = 5000;
              break;
      }
  }

  if(MXL_SUCCESS == status)
    return TRUE;
  else
    return FALSE;
  
}
MS_BOOL TUNER_MXL603_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{   
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            if( MXL_SUCCESS != MxLWare603_API_ReqTunerRxPower(u8TunerIndex,MXL603_I2C_ADDR, (float*)data))
                return FALSE;
            break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }
    return TRUE;

}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL603) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL603",          // demod system name
    .data               = TUNER_MXL603,            // private data value
    .Init               = MDrv_Tuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_SetTuner,
    .CheckExist         = MXL603_CheckExist,
    .GetTunerIF         = MXL603_GetTunerIF,
    .Extension_Function = TUNER_MXL603_Extension_Function
};

#endif


