
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

#include "drvTuner.h"
#include "drvDemod.h"
#include "drvTunerNull.h"
#include "drvDemodNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL683)
#include "drvTuner_MXL683.h"


//-------------------------------------------------------------------------------------------------
//  static function
//---------------------------------------------------------------------------------------------
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];
static MS_BOOL MXL683_detect[MAX_FRONTEND_NUM] =
#if (MAX_FRONTEND_NUM == 1)
{FALSE};
#elif (MAX_FRONTEND_NUM == 2)
{FALSE, FALSE};
#elif (MAX_FRONTEND_NUM == 3)
{FALSE, FALSE,FALSE};
#elif (MAX_FRONTEND_NUM == 4)
{FALSE, FALSE,FALSE,FALSE};
#else
{FALSE, FALSE,FALSE,FALSE};
#endif

static MS_BOOL MXL683_init[MAX_FRONTEND_NUM] =
#if (MAX_FRONTEND_NUM == 1)
{FALSE};
#elif (MAX_FRONTEND_NUM == 2)
{FALSE, FALSE};
#elif (MAX_FRONTEND_NUM == 3)
{FALSE, FALSE,FALSE};
#elif (MAX_FRONTEND_NUM == 4)
{FALSE, FALSE,FALSE,FALSE};
#else
{FALSE, FALSE,FALSE,FALSE};
#endif

mxl68x_oem_data_t mxl683_oem_data[MAX_FRONTEND_NUM];


static unsigned char MxL683_FW[] =
{
  #include "MxL68x_FW_V1_200.h"
  //#include "MxL68x_FW_V1_200.dat"
};


static MXL_STATUS_E _mxl683_FW_download(MS_U8 u8DevIdx)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MS_U32 fwBuffSize;
  unsigned char *fwBuffPtr = NULL;

  fwBuffSize = sizeof(MxL683_FW);
  fwBuffPtr = MxL683_FW;

  status = MxLWare68x_API_CfgDevFirmwareDownload(u8DevIdx,
                                                 (UINT32)fwBuffSize,
                                                 fwBuffPtr);

  if (status != MXL_SUCCESS)
  {
     TUNER_ERR(("Err : Firmware Download %d\n", status));
  }

  return status;
}

static MS_BOOL _mxl683_init(MS_U8 u8DevIdx)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL68x_FW_STATUS_E eFWStatus;
  MS_BOOL bret = TRUE;


   // Init driver context
   //mxl683_oem_data[u8DevIdx].i2cAddress = MXL683_SLAVE_ID;
   mxl683_oem_data[u8DevIdx].Mutex = -1;
  status = MxLWare68x_API_CfgDrvInit(u8DevIdx, &mxl683_oem_data[u8DevIdx]);

  if (status != MXL_SUCCESS)
  {
     TUNER_ERR(("Err : MxLWare68x_API_CfgDrvInit %d\n", status));
     bret = FALSE;
  }
   // Config Power Supply
  status = MxLWare68x_API_CfgDevPowerSupply(u8DevIdx,MXL68x_SINGLE_SUPPLY);
  if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("Err : MxLWare68x_API_CfgDevPowerSupply %d\n", status));
    bret = FALSE;
  }
  // Config XTAL
  status = MxLWare68x_API_CfgDevXtal(u8DevIdx,
                                     MXL68x_XTAL_16MHz,   // 16 MHz
                                     25,                          // 25 pF
                                     MXL_ENABLE,        // Enable CLK out
                                     MXL_FALSE,           // Disable CLK DIV
                                     MXL_FALSE,          // Disable XTAL Sharing
                                     MXL_ENABLE);          // Enable XTAL Cal for Master, Disable for Slave
  if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("Err : MxLWare68x_API_CfgDevXtal %d\n", status));
    bret = FALSE;
  }
  // Config Dev Init, switch clock to PLL clock
  status = MxLWare68x_API_CfgDevInit(u8DevIdx,
                                     MXL68x_IF_NA,          // Do not set IF mux
                                     MXL_FALSE);            // Disable RF Loop-Thru

  if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("Err : MxLWare68x_API_CfgDevInit %d\n", status));
    bret = FALSE;
  }

  eFWStatus = Ctrl_CheckFirmwareStatus(u8DevIdx);
  if (eFWStatus != MXL68x_FW_STATUS_LOADED)
  {
    // Download firmware
    status = _mxl683_FW_download(u8DevIdx);
    if (status != MXL_SUCCESS)
    {
      TUNER_ERR(("Err : _mxl683_FW_download %d\n", status));
      bret = FALSE;
    }
    else
    {
      TUNER_DBG((" [MXL683]Download firmware success"));
    }

    MxLWare68x_OEM_SleepInUs(100000); // wait for 100 ms
  }
  else
 {
    TUNER_DBG((" [MXL683]Firmware has been downloaded. \n"));
 }

   // Change Device Operational Mode to Tuner+Demod
  status = MxLWare68x_API_CfgDevOperationalMode(u8DevIdx, MXL68x_TUNER_DEMOD);
  if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("\nErr: MxLWare68x_API_CfgDevOperationalMode %d\n", status));
    bret = FALSE;
  }

  // Config IF Mux and RF loop through
  status = MxLWare68x_API_CfgDevInit(u8DevIdx,
                                     MXL68x_IF_INTERNAL,    // Initernal IF
                                     MXL_FALSE);            // Disable RF Loop-Thru
  if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("Err : MxLWare68x_API_CfgDevInit %d\n", status));
    bret = FALSE;
  }


  // TS setting
  MXL68x_MPEG_MODE_E mpegMode = MXL68x_MPEG_MODE_NORMAL;
  MXL68x_MPEG_CLK_FREQ_E mpegClkFreq = MXL68x_MPEG_CLK_64MHZ;
  MXL68x_MPEG_DATA_FMT_E mpegDataFmt = MXL68x_MPEG_DATA_MSB_FIRST;
  MXL68x_MPEG_CLK_PHASE_E mpegClkPhase = MXL68x_MPEG_CLK_PHASE_SHIFT_0_DEG;
  MXL68x_MPEG_POLARITY_E mpegSyncPol = MXL68x_POLARITY_NORMAL;
  MXL68x_MPEG_POLARITY_E mpegValidPol = MXL68x_POLARITY_NORMAL;
  MXL68x_MPEG_SYNC_PULSE_E mpegSyncPulseWidth = MXL68x_MPEG_SYNC_PULSE_1BYTE;
  MXL68x_MPEG_ERROR_IND_E mpegErrIndicator = MXL68x_MPEG_ERROR_IND_ENABLE;
  MXL68x_TS_PKT_SIZE_E tsPktSize = MXL68x_TS_PKT_188_BYTES;

  status = MxLWare68x_API_CfgDemodMPEGOut( u8DevIdx,
                                           mpegMode,
                                           mpegClkFreq,
                                           mpegDataFmt,
                                           mpegClkPhase,
                                           mpegSyncPol,
                                           mpegValidPol,
                                           mpegSyncPulseWidth,
                                           mpegErrIndicator,
                                           tsPktSize);

  if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("Err : MxLWare68x_API_CfgDemodMPEGOut %d\n", status));
    bret = FALSE;
  }


  status = MxLWare68x_API_CfgDemodMPEGTSDriveStrength(u8DevIdx,MXL68x_TS_DRV_STR_3X);
 if (status != MXL_SUCCESS)
  {
    TUNER_ERR(("Err : MxLWare68x_API_CfgDemodMPEGTSDriveStrength %d\n", status));
    bret = FALSE;
  }

  status = MxLWare68x_API_CfgDemodNullPacketFilter(u8DevIdx, MXL_ENABLE,
                                                             MXL_ENABLE,
                                                             MXL_ENABLE,
                                                             MXL_DISABLE);

  if (status != MXL_SUCCESS)
  {
     TUNER_ERR(("Err : MxLWare68x_API_CfgDemodNullPacketFilter %d\n", status));
     bret = FALSE;
  }

  if(bret)
  {
    MXL683_init[u8DevIdx] = TRUE;
  }

  return bret;

}


//-------------------------------------------------------------------------------------------------
//  Global Variables
//---------------------------------------------------------------------------------------------

//======================
MS_BOOL MXL683_Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MS_U8 u8DevIdx = u8TunerIndex;
    MS_BOOL bret;


    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
    {
       InitParam[u8DevIdx].pCur_Broadcast_type = pParam->pCur_Broadcast_type;
    }

    if(MXL683_init[u8DevIdx] == TRUE)
    {
        return TRUE;
    }

    bret = _mxl683_init(u8DevIdx);

    return bret;

}

MS_BOOL MXL683_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_U8 u8DevIdx = u8DemodIndex;
    MS_BOOL bret;

    if(MXL683_init[u8DevIdx] == TRUE)
        return TRUE;

    bret = _mxl683_init(u8DevIdx);

     return bret;
}


MS_BOOL MXL683_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MXL_STATUS_E status;
    MXL68x_BW_E eBW;
    MXL68x_TUNE_TYPE_E eTuneType = MXL68x_VIEW_MODE;
    MXL68x_AUTO_SPEC_INV eAutoSpecInv = MXL68x_AUTO_SPEC_INV_ENABLE;
    UINT32 u32Freq;
    MS_U8 u8DevIdx = u8TunerIndex;


    eBW = (MXL68x_BW_E)ucBw;
    u32Freq = (UINT32)(dwFreq*1000);
    // Set or Change Channel
    status = MxLWare68x_API_CfgTunerChanTune(u8DevIdx, eTuneType, eBW, u32Freq,eAutoSpecInv,MXL_FALSE);

    if (status != MXL_SUCCESS)
    {
        printf ("\nErr: MxLWare68x_API_CfgTunerChanTune %d\n", status);
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

MS_BOOL MXL683_Tuner_GetLock(MS_U8 u8TunerIndex)
{
   MXL_STATUS_E status;
   MXL_BOOL_E rfSynthLock = MXL_FALSE, refSynthLock = MXL_FALSE;

   status = MxLWare68x_API_ReqTunerLockStatus(u8TunerIndex, &rfSynthLock, &refSynthLock);
   if (status != MXL_SUCCESS)
        DMD_ERR(("\nErr: MxLWare68x_API_ReqTunerLockStatus %d\n", status));

   if((rfSynthLock == MXL_TRUE) && (refSynthLock == MXL_TRUE))
       return TRUE;
   else
       return FALSE;
}


MS_BOOL MXL683_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{

   return TRUE;
}

#define MXL683_CHIP_ID1 0x0047
#define MXL683_CHIP_ID2 0x1012

static MS_BOOL _mxl683_read_chip_ID(MS_U8 u8devID)
{
   MXL_STATUS_E status = MXL_FAILURE;
   mxl68x_oem_data_t oemPtr;
   MS_U8 i;
   MS_BOOL bret = FALSE;
   UINT16 regData = 0;
   UINT16 regAddr = 0;
   UINT16 regData1 = 0;

  for(i=0;i<4;i++)
  {
   oemPtr.i2cAddress = MXL683_SLAVE_ID - i;
   oemPtr.Mutex = -1;
   MxL68x_OEM_DataPtr[u8devID] = (void *)&oemPtr;
   printf("@@ try slave ID: 0x%02x\n", oemPtr.i2cAddress);


   regAddr = 0x3000;
   status = MxLWare68x_OEM_ReadRegister(u8devID, regAddr , &regData);
   printf("0x%04x\n", regData);

   regAddr = 0x3001;
   status = MxLWare68x_OEM_ReadRegister(u8devID, regAddr , &regData1);
   printf("0x%04x\n", regData1);

   if((MXL_SUCCESS == status) && (MXL683_CHIP_ID1 == regData) && (MXL683_CHIP_ID2 == regData1))
   {
      TUNER_DBG(("== Get MXL683 ID !!\n"));
      MXL683_detect[u8devID] = TRUE;
      bret = TRUE;
      mxl683_oem_data[u8devID].i2cAddress = oemPtr.i2cAddress;
      break;
   }
   else
   {
       TUNER_ERR(("== Get MXL683 ID FAIL !!\n"));
       bret = FALSE;
   }
  }
    return bret;
}

MS_BOOL MXL683_TunerCheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 u8devID;
    MS_BOOL bret = TRUE;

    u8devID = u8TunerIndex; // here u8TunerIndex means device index

    if(!MXL683_detect[u8devID])
    {
        bret = _mxl683_read_chip_ID(u8devID);
    }
    else
    {
        TUNER_DBG(("== MXL542 have been detected \n"));
    }

    if(bret)
    {
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
    }
    return bret;

}

MS_BOOL MXL683_DemodCheckExist(MS_U8 u8TunerIndex, MS_U8* pu8SlaveID)
{
   MS_U8 u8devID;
   MS_BOOL bret = TRUE;

   u8devID = u8TunerIndex; // here u8TunerIndex means device index

   if(!MXL683_detect[u8devID])
   {
        bret = _mxl683_read_chip_ID(u8devID);
   }
   else
   {
        DMD_DBG(("== MXL542 have been detected \n"));
   }

   if(bret)
       *pu8SlaveID =  mxl683_oem_data[u8devID].i2cAddress;

   return bret;

}


MS_BOOL MXL683_Demod_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return TRUE;
}

MS_BOOL MXL683_Tuner_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return TRUE;
}


MS_BOOL MXL683_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MXL_STATUS_E status;
    MXL_BOOL_E tmccLock;
    MXL_BOOL_E rsLockC;
    MXL_BOOL_E rsLockB;
    MXL_BOOL_E rsLockA;

    status = MxLWare68x_API_ReqDemodLockStatus(u8DemodIndex, &tmccLock, &rsLockA, &rsLockB, &rsLockC);
    if (status != MXL_SUCCESS)
    {
        DMD_ERR(("\nErr: MxLWare68x_API_ReqDemodLockStatus %d\n", status));
        return FALSE;
    }

    DMD_DBG(("***** Demod Lock status: Frame lock = %d, Layer_A RS = %d, Layer_B RS = %d, Layer_C RS = %d \n", tmccLock, rsLockA, rsLockB, rsLockC));

    if(MXL_TRUE == tmccLock)
    {
        *peLockStatus = E_DEMOD_LOCK;
    }
    else
    {
        *peLockStatus = E_DEMOD_CHECKING;
    }
    return TRUE;
}

MS_BOOL MXL683_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
  MXL_STATUS_E status;
  UINT32 cnrRaw;
  SINT32 cnr;

  //MxLWare68x_OEM_SleepInUs(2000000); // wait for 2s
  status = MxLWare68x_API_ReqDemodCNR(u8DemodIndex, &cnrRaw, &cnr);
  if(MXL_SUCCESS == status)
  {
      *pfSNR = (float)(cnr)/1000;
      return TRUE;
  }
  else
  {
      return FALSE;
  }

}

MS_BOOL MXL683_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MXL_STATUS_E status;
    MXL68x_BER_T layerABer,layerBBer,layerCBer;

    status = MxLWare68x_API_CfgDemodBerPer(u8DemodIndex, 6);
    if(MXL_SUCCESS != status)
    {
      return FALSE;
    }
    else
    {
      //MxLWare68x_OEM_SleepInUs(2000000); // wait for 2s
      status = MxLWare68x_API_ReqDemodBER(u8DemodIndex, &layerABer, &layerBBer, &layerCBer);
      *pfBER = (float)layerABer.bitErrCount_WO_Iter/(float)layerABer.numOfBits;
      return TRUE;
    }
}


MS_BOOL MXL683_Demod_GetPacketError(MS_U8 u8DemodIndex,MS_U16 *u16PktErr)
{
    MXL_STATUS_E status;
    MXL68x_PER_T layerAPer,layerBPer,layerCPer;

    status = MxLWare68x_API_CfgDemodBerPer(u8DemodIndex, 6);
    if(MXL_SUCCESS != status)
    {
      return FALSE;
    }
    else
    {
      //MxLWare68x_OEM_SleepInUs(2000000); // wait for 2s
      status = MxLWare68x_API_ReqDemodPER(u8DemodIndex, &layerAPer, &layerBPer, &layerCPer);
      *u16PktErr = (MS_U16)layerAPer.pktErrCount_WO_Iter;
      return TRUE;
    }

}

MS_BOOL MXL683_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MXL_STATUS_E status;
    SINT32 rxPwr = 0;

    status = MxLWare68x_API_ReqTunerRxPower(u8DemodIndex, &rxPwr);
    if(MXL_SUCCESS == status)
    {
        *ps32Signal = (MS_S32)(rxPwr)/100;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


MS_BOOL MXL683_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MXL_STATUS_E status;
    MXL_BOOL_E ewsAlarm;
    MXL68x_GUARD_INTERVAL_E gi;
    MXL68x_ISDBT_MODE_E isdbtMode;
    MXL68x_TMCC_INFO_T tmccA;
    MXL68x_TMCC_INFO_T tmccB;
    MXL68x_TMCC_INFO_T tmccC;

     // Inquiring TMCC information
    status = MxLWare68x_API_ReqDemodTMCCParams(u8DemodIndex, &ewsAlarm, &gi, &isdbtMode, &tmccA, &tmccB, &tmccC);
    if (status != MXL_SUCCESS)
    {
        DMD_ERR(("\nErr: MxLWare68x_API_ReqDemodLockStatus %d\n", status));
        return FALSE;
    }

    DMD_DBG(("***** TMCC info:  ewsAlarm %d, isdbtMode %d, gi %d\n", ewsAlarm, isdbtMode, gi));
    DMD_DBG(("Layer A: Mod %d, CR %d, IL %d, Segs %d\n",tmccA.modulationInfo,tmccA.codeRateInfo,tmccA.interleaverLenInfo,tmccA.numOfSegments));
    DMD_DBG(("Layer B: Mod %d, CR %d, IL %d, Segs %d\n",tmccB.modulationInfo,tmccB.codeRateInfo,tmccB.interleaverLenInfo,tmccB.numOfSegments));
    DMD_DBG(("Layer C: Mod %d, CR %d, IL %d, Segs %d\n",tmccC.modulationInfo,tmccC.codeRateInfo,tmccC.interleaverLenInfo,tmccC.numOfSegments));

    return TRUE;
}


DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL683) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL683",          // demod system name
    .data               = TUNER_MXL683,            // private data value
    .Init               = MXL683_Tuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MXL683_Tuner_GetLock,
    .SetTuner           = MXL683_Tuner_SetTuner,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .CheckExist         = MXL683_TunerCheckExist,
    .Extension_Function = MXL683_Tuner_Extension_Function
};

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MXL683) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MXL683",
     .data                         = DEMOD_MXL683,
     .init                         = MXL683_Demod_Init,
     .GetLock                      = MXL683_Demod_GetLock,
     .GetSNR                       = MXL683_Demod_GetSNR,
     .GetBER                       = MXL683_Demod_GetBER,
     .GetPWR                       = MXL683_Demod_GetPWR,
     .GetSSI                       = MDrv_Demod_null_GetSSI,
     .GetQuality                   = MDrv_Demod_null_GetSignalQuality,
     .GetParam                     = MXL683_Demod_GetParam,
     .Restart                      = MXL683_Demod_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .CheckExist                   = MXL683_DemodCheckExist,
     .Extension_Function           = MXL683_Demod_Extension_Function,
     .Get_Packet_Error             = MXL683_Demod_GetPacketError,
 #if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
     .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
     .GetPLPType                   = MDrv_Demod_null_GetPLPType,
 #endif
 #if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
     .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
     .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
     .SetISID                      = MDrv_Demod_null_SetVCM_ISID
 #endif
};

#endif
