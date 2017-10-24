
#include <stdio.h>
#include "error.h"
#include "MsCommon.h"
#include "Board.h"

#include "drvTuner.h"
#include "drvDemod.h"
#include "drvTunerNull.h"
#include "drvDemodNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL254)
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_OEM_Drv.h"
#include "drvTuner_MXL254.h"

#define EXAMPLE_DEV_MAX   2
#define MXL254_I2C_ADDR   0x50 //0x50 0x51 0x52 0x53 depend on HW setting
#define MAX_CHANNEL_COUNT 4


static UINT8 MxL254_LIB[] =
{
  #include "MxL_HRCLS_FW.dat"
};

//#define HRCLS_DEVICE_ID 0 // same as u8TunerIndex
typedef struct
{
  UINT8 i2cAddress;
} mxl254_init_data_t;

mxl254_init_data_t initData;


//-------------------------------------------------------------------------------------------------
//  static function
//---------------------------------------------------------------------------------------------
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];
static MXL_BOOL_E xptSupported[MAX_FRONTEND_NUM];
static MS_BOOL MXL254_detect[MAX_FRONTEND_NUM] =
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

static MS_BOOL MXL254_init[MAX_FRONTEND_NUM] =
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


static MXL_HRCLS_ANNEX_TYPE_E mxl_ANNEX_TYPE_MAP(MS_U32 dvb_type)
{
   MXL_HRCLS_ANNEX_TYPE_E ret;
   switch(dvb_type)
   {
       case DVBC:
           ret = MXL_HRCLS_ANNEX_A;
           break;
       case J83B:
           ret = MXL_HRCLS_ANNEX_B;
           break;
       default:
           ret = MXL_HRCLS_ANNEX_NOT_SUPPORT;
           break;
   }
   return ret;
}

static MXL_HRCLS_QAM_TYPE_E mxl_QAM_TYPE_MAP(DEMOD_EN_CAB_CONSTEL_TYPE eConstellation)
{
   MXL_HRCLS_QAM_TYPE_E ret;
   switch(eConstellation)
   {
       case DEMOD_CAB_QAM16:
           ret = MXL_HRCLS_QAM16;
           break;
       case DEMOD_CAB_QAM32:
           ret = MXL_HRCLS_QAM32;
           break;
       case DEMOD_CAB_QAM64:
           ret = MXL_HRCLS_QAM64;
           break;
       case DEMOD_CAB_QAM128:
           ret = MXL_HRCLS_QAM32;
           break;
       case DEMOD_CAB_QAM256:
           ret = MXL_HRCLS_QAM256;
           break;
       case DEMOD_CAB_QAMAUTO:
           ret = MXL_HRCLS_QAM_AUTO;
           break;
       default:
           ret = MXL_HRCLS_QAM_AUTO;
           break;
   }
   return ret;
}

static MXL_STATUS_E mxl_checkVersion(MS_U8 u8TunerIndex,MXL_HRCLS_DEV_VER_T* verInfoPtr)
{
  
/************************************************************************************
    Read MxLWare, Firmware and Bootloader version.  
************************************************************************************/
  MXL_STATUS_E status = MxLWare_HRCLS_API_ReqDevVersionInfo(u8TunerIndex, verInfoPtr);

  if (status == MXL_SUCCESS)
  {
    TUNER_DBG(("[HRCLS][%d] chipVersion=%d MxLWare: %d.%d.%d.%d-RC%d Firmware:%d.%d.%d.%d-RC%d Bootloader:%d.%d.%d.%d fwDownloaded=%d\n",
        u8TunerIndex,
        verInfoPtr->chipVersion,
        verInfoPtr->mxlWareVer[0],
        verInfoPtr->mxlWareVer[1],
        verInfoPtr->mxlWareVer[2],
        verInfoPtr->mxlWareVer[3],
        verInfoPtr->mxlWareVer[4],
        verInfoPtr->firmwareVer[0],
        verInfoPtr->firmwareVer[1],
        verInfoPtr->firmwareVer[2],
        verInfoPtr->firmwareVer[3],
        verInfoPtr->firmwareVer[4],
        verInfoPtr->bootLoaderVer[0],
        verInfoPtr->bootLoaderVer[1],
        verInfoPtr->bootLoaderVer[2],
        verInfoPtr->bootLoaderVer[3],
        verInfoPtr->firmwareDownloaded
        ));
  }
  return status;
}

static MXL_STATUS_E mxl_downloadFirmware(MS_U8 u8DevIdx,MXL_BOOL_E * xptSupported)
{
  MXL_STATUS_E result = MXL_FAILURE;
  UINT16 firmwareSize = 0;

  //FILE * file_id;

   firmwareSize = sizeof(MxL254_LIB);

  /************************************************************************************
   Open firmware file in binary mode  
  ************************************************************************************/
  //file_id = fopen(HRCLS_FIRMWARE_FILENAME, "rb");
  if (firmwareSize)
  {
    //fseek(file_id, 0, SEEK_END);
    //firmwareSize = (UINT16) ftell(file_id);
    //fseek(file_id, 0, SEEK_SET);

  
  /************************************************************************************
    Allocate memory buffer of the firmware size and transfer whole firmware image to the buffer.
   ************************************************************************************/

    //if (MxL254_LIB)
    //{
      //if (fread(firmwareBuf, sizeof(UINT8), firmwareSize, file_id) == firmwareSize)
      //{
        MXL_STATUS_E status;
       
  /************************************************************************************
         Download firmware. This is a blocking function. Downloading firmware may take a while. Actual execution depends also on target platform's I2C speed.
         Define callback function that will be called after every segment is downloadedif progress information is required.        
   ************************************************************************************/
        status = MxLWare_HRCLS_API_CfgDevFirmwareDownload(u8DevIdx, firmwareSize, MxL254_LIB, NULL);
        if (status == MXL_SUCCESS)
        {
          MXL_HRCLS_DEV_VER_T devVerInfo;
          TUNER_DBG(("[HRCLS][%d] {%s} Firmware download OK\n", u8DevIdx, __FUNCTION__));

         
  /************************************************************************************
           After firmware is successfully downloaded, read its version.         
   ************************************************************************************/
          if (MxLWare_HRCLS_API_ReqDevVersionInfo(u8DevIdx, &devVerInfo) == MXL_SUCCESS)
          {
            result = MXL_SUCCESS;
            TUNER_DBG(("[HRCLS][%d] {%s} Firmware ver. %d.%d.%d.%d-RC%d\n", u8DevIdx, __FUNCTION__,
              devVerInfo.firmwareVer[0], devVerInfo.firmwareVer[1],
              devVerInfo.firmwareVer[2], devVerInfo.firmwareVer[3],
              devVerInfo.firmwareVer[4]));
            if ((devVerInfo.chipId == MXL_HRCLS_HERCULES_CHIP_ID) && (devVerInfo.chipVersion != 1))
            {
              *xptSupported = MXL_TRUE;
            } else *xptSupported = MXL_FALSE;

          }
          else
          {
            TUNER_ERR(("[HRCLS][%d] {%s} Firmware version read FAILED\n", u8DevIdx, __FUNCTION__));
          }
        }
        else
        {
          TUNER_ERR(("[HRCLS][%d] {%s} Firmware download FAILED (error code: %d)\n", u8DevIdx, __FUNCTION__, status));
        }
      //}
      //else
      //{
        //TUNER_DBG(("[HRCLS][%d] {%s} Cannot read %d bytes from successfully open file %s\n", u8DemodIndex, __FUNCTION__, firmwareSize, HRCLS_FIRMWARE_FILENAME);
      //}
      //free(firmwareBuf);
    //}

    //fclose(file_id);
  }
  else
  {
    //TUNER_DBG(("[HRCLS][%d] {%s} Error get FW %s.\n", u8DemodIndex, __FUNCTION__);
  }

  return result;
}

static MXL_STATUS_E mxl_waitForTunerLock(MS_U8 u8TunerIndex, MXL_HRCLS_TUNER_ID_E tunerId)
{
  MXL_STATUS_E status;
  MXL_HRCLS_TUNER_STATUS_E lockStatus = MXL_HRCLS_TUNER_DISABLED;
  do
  {
    MxLWare_HRCLS_OEM_DelayUsec(1000);
    status = MxLWare_HRCLS_API_ReqTunerLockStatus(u8TunerIndex, tunerId, &lockStatus);
  } while ((status == MXL_SUCCESS) && (lockStatus != MXL_HRCLS_TUNER_LOCKED));

  return ((status == MXL_SUCCESS) && (lockStatus == MXL_HRCLS_TUNER_LOCKED))?MXL_SUCCESS:MXL_FAILURE;
}

static MXL_STATUS_E mxl_enableFbTuner(MS_U8 u8TunerIndex)
{
  MXL_STATUS_E status;

  status = MxLWare_HRCLS_API_CfgTunerEnable(u8TunerIndex, MXL_HRCLS_FULLBAND_TUNER);
  if (status != MXL_FAILURE)
  {
    status = mxl_waitForTunerLock(u8TunerIndex, MXL_HRCLS_FULLBAND_TUNER);  
    if (status != MXL_SUCCESS)
    {
      TUNER_ERR(("[HRCLS][%d] {%s} Fullband tuner synth lock TIMEOUT!\n", u8TunerIndex, __FUNCTION__));
    }
    else
    {
      TUNER_ERR(("[HRCLS][%d] {%s} Fullband tuner synth lock !\n", u8TunerIndex, __FUNCTION__));
    }
  }
  else
  {
    TUNER_ERR(("[HRCLS][%d] {%s} Cannot enable fullband tuner\n", u8TunerIndex, __FUNCTION__));
  }

  return MXL_SUCCESS;
}

static MXL_STATUS_E mxl_waitForChannelLock(MS_U8 u8DemodIndex)
{
  MXL_STATUS_E status;
  MXL_HRCLS_CHAN_STATUS_E lockStatus = MXL_HRCLS_CHAN_DISABLED;
  MXL_HRCLS_CHAN_ID_E eCHAN_ID;
  UINT8 u8devID;
  
  eCHAN_ID = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
  u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

  do
  {
    MxLWare_HRCLS_OEM_DelayUsec(1000);
    status = MxLWare_HRCLS_API_ReqTunerChanStatus(u8devID, eCHAN_ID, &lockStatus);
  } while ((status == MXL_SUCCESS) && (lockStatus != MXL_HRCLS_CHAN_LOCKED));

  return ((status == MXL_SUCCESS) && (lockStatus == MXL_HRCLS_CHAN_LOCKED))?MXL_SUCCESS:MXL_FAILURE;
}

static MS_BOOL _mxl254_init(MS_U8 u8DevIdx)
{
   MXL_STATUS_E status;
   MXL_HRCLS_DEV_VER_T devVerInfo;

   initData.i2cAddress = MXL254_I2C_ADDR; 
   status = MxLWare_HRCLS_API_CfgDrvInit(u8DevIdx, (void *) &initData, MXL_HRCLS_DEVICE_254);
   if (status == MXL_ALREADY_INITIALIZED)
        return TRUE;
  
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HRCLS][%d] Driver initialization FAILED\n", u8DevIdx));
        return FALSE;
   }

   status = MxLWare_HRCLS_API_CfgDevReset(u8DevIdx);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HRCLS][%d] Device reset FAILED\n", u8DevIdx));
        return FALSE;
   }

   status = MxLWare_HRCLS_API_CfgDevXtalSetting(u8DevIdx, 0);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HRCLS][%d] Xtal setting FAILED\n", u8DevIdx));
        return FALSE;
   }

   status = mxl_checkVersion(u8DevIdx, &devVerInfo);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HRCLS][%d] Version checking FAILED!\n", u8DevIdx));
        return FALSE;
   }

   if (devVerInfo.firmwareDownloaded == MXL_TRUE)
   {
        // Something must be wrong, because we just went through reset
        TUNER_ERR(("[HRCLS][%d] Firmware already running. Forgot about h/w reset?\n", u8DevIdx));
        return FALSE;
   }

   status = mxl_downloadFirmware(u8DevIdx, &xptSupported[u8DevIdx]);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HRCLS][%d] Firmware download FAILED\n", u8DevIdx));
        return FALSE;
   }
   else
   {
       TUNER_DBG(("[HRCLS][%d] Firmware download Done\n", u8DevIdx));
   }

   status = mxl_enableFbTuner(u8DevIdx);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HRCLS][%d] Enable FB tuner FAILED\n", u8DevIdx));
        return FALSE;
   }

   
   if (xptSupported[u8DevIdx] == MXL_TRUE)
   {
        status = MxLWare_HRCLS_API_CfgXpt(u8DevIdx, MXL_HRCLS_XPT_MODE_NO_MUX_4); 
   }
   else
   {
        status = MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams(u8DevIdx, MXL_HRCLS_MPEG_CLK_POSITIVE, MXL_HRCLS_MPEG_DRV_MODE_1X, MXL_HRCLS_MPEG_CLK_56_21MHz);
   }
  
   if (status != MXL_SUCCESS)
   {
        TUNER_DBG(("[HRCLS][%d] Global MPEG params setting failed\n", u8DevIdx));
        return FALSE;
   }

   MXL254_init[u8DevIdx] = TRUE;
   return TRUE;
}
//-------------------------------------------------------------------------------------------------
//  Global Variables
//---------------------------------------------------------------------------------------------

//======================
MS_BOOL MXL254Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID = u8TunerIndex/MAX_CHANNEL_COUNT;

    if(MXL254_init[u8devID] == TRUE)
        return TRUE;
    
    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex/MAX_CHANNEL_COUNT].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

   return _mxl254_init(u8devID);
}

MS_BOOL MXL254_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID = u8DemodIndex/MAX_CHANNEL_COUNT;

    if(MXL254_init[u8devID] == TRUE)
        return TRUE;
    else
       return _mxl254_init(u8devID);   
}


MS_BOOL MXL254Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
  MXL_STATUS_E status;
  MXL_HRCLS_CHAN_ID_E eCHAN_ID;
  UINT8 u8devID;

  eCHAN_ID = (MXL_HRCLS_CHAN_ID_E)(u8TunerIndex % MAX_CHANNEL_COUNT);
  u8devID = (UINT8)(u8TunerIndex/MAX_CHANNEL_COUNT);

  switch(ucBw)
  {
    case TUNER_BW_MODE_6MHZ:
        ucBw = 6;
        break;
    case TUNER_BW_MODE_7MHZ:
        ucBw = 7;
        break;
    case TUNER_BW_MODE_8MHZ:
    default:    
        ucBw = 8;
        break;
  }
  
  
  status = MxLWare_HRCLS_API_CfgTunerChanTune(u8devID, eCHAN_ID, ucBw, dwFreq * 1000);
  if(MXL_SUCCESS == status)
  {
      TUNER_DBG(("MXL254Tuner_SetTuner OK\n"));
      return TRUE;
  }
  else
  {
      TUNER_ERR(("MXL254Tuner_SetTuner Fail\n"));
      return FALSE;
  }
}

#define MXL254_CHIP_ID 0x100
MS_BOOL MXL254_TunerCheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
  MXL_STATUS_E status = MXL_FAILURE;
  UINT16 regData = 0;
  MS_U8 u8devID;

  u8devID = u8TunerIndex; // here u8TunerIndex means device index
  //return TRUE;
  if(!MXL254_detect[u8devID])
  {
      status = MxLWare_HRCLS_OEM_ReadRegister(u8devID, 0x60E4, &regData);
      if((MXL_SUCCESS == status) && (MXL254_CHIP_ID == regData))
      {
          TUNER_DBG(("== Get MXL254 ID !!\n"));
          MXL254_detect[u8devID] = TRUE;
          if(NULL != pu32channel_cnt)
             *(pu32channel_cnt) += MAX_CHANNEL_COUNT; 
          return TRUE;
      }
      else
      {
          TUNER_ERR(("== Get MXL254 ID FAIL !!\n"));
          return FALSE;
      }
  }
  else
  {
      if(NULL != pu32channel_cnt)
          *(pu32channel_cnt) += MAX_CHANNEL_COUNT; 
  }
  TUNER_DBG(("== MXL254 have been detected \n"));
  return TRUE;
}

MS_BOOL MXL254_DemodCheckExist(MS_U8 u8TunerIndex)
{
  MXL_STATUS_E status = MXL_FAILURE;
  UINT16 regData = 0;
  MS_U8 u8devID;

  u8devID = u8TunerIndex; // here u8TunerIndex means device index
  //return TRUE;
  if(!MXL254_detect[u8devID])
  {
      status = MxLWare_HRCLS_OEM_ReadRegister(u8devID, 0x60E4, &regData);
      if((MXL_SUCCESS == status) && (MXL254_CHIP_ID == regData))
      {
          TUNER_DBG(("== Get MXL254 ID !!\n"));
          MXL254_detect[u8devID] = TRUE;
          return TRUE;
      }
      else
      {
          TUNER_ERR(("== Get MXL254 ID FAIL !!\n"));
          return FALSE;
      }
  }
  TUNER_DBG(("== MXL254 have been detected \n"));
  return TRUE;
}


MS_BOOL MXL254_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    //MS_U8 tmp;
    switch(fuction_type)
    {
        //case DEMOD_EXT_FUNC_SET_CHANNEL_ID:
            // channel ID = Demod ID
            //tmp = *data;
            //eCur_channel = *data;
            //eCur_DMD_ID = *data;
            //break;
        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
            
    }
    return TRUE;
}

MS_BOOL MXL254_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MXL_STATUS_E status;
    MXL_BOOL_E qamLockStatus = MXL_FALSE;     
    MXL_BOOL_E fecLockStatus = MXL_FALSE;    
    MXL_BOOL_E mpegLockStatus = MXL_FALSE;
    MXL_BOOL_E retuneRequired = MXL_FALSE;
    MXL_HRCLS_DMD_ID_E eCHAN_ID;
    UINT8 u8devID;

    eCHAN_ID = (MXL_HRCLS_DMD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    

    status = MxLWare_HRCLS_API_ReqDemodAllLockStatus(u8devID,eCHAN_ID,&qamLockStatus,&fecLockStatus,&mpegLockStatus,&retuneRequired);
    status |= MxLWare_HRCLS_API_CfgUpdateDemodSettings(u8devID, eCHAN_ID);
   if(status == MXL_SUCCESS)
   {
       DMD_DBG(("[MXL254]qamLockStatus = %x, fecLockStatus = %x, mpegLockStatus = %x\n", qamLockStatus, fecLockStatus, mpegLockStatus));
       if(qamLockStatus && fecLockStatus && mpegLockStatus)
         *peLockStatus = E_DEMOD_LOCK;
       else
         *peLockStatus = E_DEMOD_CHECKING;
       return TRUE;
   }
   else
    {
       DMD_ERR(("MxLWare_HRCLS_API_ReqDemodAllLockStatus NG\n"));
       return FALSE;
    }
}

MS_BOOL MXL254_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
   MXL_STATUS_E status;
   UINT16  u16SNR;
   MXL_HRCLS_DMD_ID_E eDMD_ID;
   UINT8 u8devID;
   
   eDMD_ID = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
   u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

   status = MxLWare_HRCLS_API_ReqDemodSnr(u8devID, eDMD_ID, &u16SNR);

   if(status == MXL_SUCCESS)
   {
     *pfSNR = (float)(u16SNR)/10;
     return TRUE;
   }
   else
    return FALSE;
}

MS_BOOL MXL254_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{
    MXL_HRCLS_DMD_STAT_CNT_T stState;
    MXL_STATUS_E status;
    MXL_HRCLS_DMD_ID_E eDMD_ID;
    UINT8 u8devID;
   
    eDMD_ID = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    status = MxLWare_HRCLS_API_ReqDemodErrorStat(u8devID, eDMD_ID, &stState);

   if(status == MXL_SUCCESS)
   {
     switch(*InitParam[u8devID].pCur_Broadcast_type)
     {
        case DVBC:
        default:
            *pfBER = 9*(float)(stState.ErrMpeg)/188/8/(float)(stState.ReceivedMpeg);
            break;
        case J83B:
            *pfBER = 10 *(float)(stState.ErrMpeg)/188/8/(float)(stState.ReceivedMpeg);
            break;
     }
     return TRUE;
   }
   else
   {
        *pfBER = 0;
        return FALSE;
   }

}


MS_BOOL MXL254_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_ANNEX_TYPE_E annexType;
  MXL_HRCLS_QAM_TYPE_E qamType = mxl_QAM_TYPE_MAP(pParam->CabParam.eConstellation);
  MXL_BOOL_E iqFlip = (MXL_BOOL_E)pParam->CabParam.eIQMode;
  UINT16 symbolRatekSps = (UINT16)pParam->CabParam.u16SymbolRate;
  MXL_HRCLS_DMD_ID_E eDMD_ID;
  UINT8 u8devID;
  MXL_HRCLS_MPEGOUT_PARAM_T mpegParams;
  MXL_HRCLS_XPT_MPEGOUT_PARAM_T mpegXptParams;
  
  eDMD_ID = (MXL_HRCLS_DMD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
  u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
  annexType = mxl_ANNEX_TYPE_MAP(*InitParam[u8devID].pCur_Broadcast_type);

  //TUNER_DBG(("### MXL254_Demod_Restart eDMD_ID = %x, u8devID = %x\n", (UINT8)eDMD_ID, u8devID);

  if(annexType == MXL_HRCLS_ANNEX_NOT_SUPPORT)
     return FALSE;

  if (xptSupported[u8devID] == MXL_FALSE)
  {
      //MXL_HRCLS_MPEGOUT_PARAM_T mpegParams;

      // sample settings
      mpegParams.enable = MXL_ENABLE;
      mpegParams.lsbOrMsbFirst = MXL_HRCLS_MPEG_SERIAL_LSB_1ST; 
      mpegParams.mpegSyncPulseWidth = MXL_HRCLS_MPEG_SYNC_WIDTH_BIT;
      mpegParams.mpegValidPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegParams.mpegSyncPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegParams.mpegErrorIndication = MXL_DISABLE;
      mpegParams.mpeg3WireModeEnable = MXL_ENABLE;
      mpegParams.mpegPadDrv.padDrvMpegSyn = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpegPadDrv.padDrvMpegDat = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpegPadDrv.padDrvMpegVal = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpeg3WireModeClkPol = MXL_HRCLS_MPEG_CLK_NEGATIVE;  

      status = MxLWare_HRCLS_API_CfgDemodMpegOutParams(u8devID, eDMD_ID, &mpegParams); 
  }
  else
  {
      //MXL_HRCLS_MPEGOUT_PARAM_T mpegParams;
      //MXL_HRCLS_XPT_MPEGOUT_PARAM_T mpegXptParams;

      // sample settings
      #if 0
      mpegParams.enable = MXL_ENABLE;
      mpegParams.lsbOrMsbFirst = MXL_HRCLS_MPEG_SERIAL_LSB_1ST; 
      mpegParams.mpegSyncPulseWidth = MXL_HRCLS_MPEG_SYNC_WIDTH_BIT;
      mpegParams.mpegValidPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegParams.mpegSyncPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegParams.mpegErrorIndication = MXL_DISABLE;
      mpegParams.mpeg3WireModeEnable = MXL_ENABLE;
      mpegParams.mpegPadDrv.padDrvMpegSyn = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpegPadDrv.padDrvMpegDat = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpegPadDrv.padDrvMpegVal = MXL_HRCLS_MPEG_DRV_MODE_1X;
      mpegParams.mpeg3WireModeClkPol = MXL_HRCLS_MPEG_CLK_NEGATIVE;  

      status = MxLWare_HRCLS_API_CfgDemodMpegOutParams(u8devID, eDMD_ID, &mpegParams); 
      #endif
      // sample MPEG configuration
      mpegXptParams.enable = MXL_ENABLE;
      mpegXptParams.lsbOrMsbFirst = MXL_HRCLS_MPEG_SERIAL_MSB_1ST;
      mpegXptParams.mpegSyncPulseWidth = MXL_HRCLS_MPEG_SYNC_WIDTH_BYTE;
      mpegXptParams.mpegValidPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegXptParams.mpegSyncPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegXptParams.mpegClkPol = MXL_HRCLS_MPEG_CLK_NEGATIVE;
      mpegXptParams.clkFreq = MXL_HRCLS_MPEG_CLK_42_18MHz; 
      mpegXptParams.mpegPadDrv.padDrvMpegSyn = MXL_HRCLS_MPEG_DRV_MODE_2X;  
      mpegXptParams.mpegPadDrv.padDrvMpegDat = MXL_HRCLS_MPEG_DRV_MODE_2X;  
      mpegXptParams.mpegPadDrv.padDrvMpegVal = MXL_HRCLS_MPEG_DRV_MODE_2X;  

      // In MxL254 and NO_MUX_4 mode, outputId = demodId
      // Check MxLWare API User Guide for other modes' mappings
      status = MxLWare_HRCLS_API_CfgXptOutput(u8devID, (MXL_HRCLS_XPT_OUTPUT_ID_E) eDMD_ID, &mpegXptParams);
  }

  status = mxl_waitForChannelLock(u8devID);
  if (status == MXL_SUCCESS)
  {
    status = MxLWare_HRCLS_API_CfgDemodEnable(u8devID, eDMD_ID, MXL_TRUE);
    if (status == MXL_SUCCESS)
    {
      status = MxLWare_HRCLS_API_CfgDemodAdcIqFlip(u8devID, eDMD_ID, iqFlip);
      if (status == MXL_SUCCESS)
      {
        status = MxLWare_HRCLS_API_CfgDemodAnnexQamType(u8devID, eDMD_ID, annexType, qamType);
        if (status == MXL_SUCCESS)
        {
          status = MxLWare_HRCLS_API_CfgDemodSymbolRate(u8devID, eDMD_ID, symbolRatekSps * 1000, symbolRatekSps * 1000);

          if (status == MXL_SUCCESS)
          {
            status = MxLWare_HRCLS_API_CfgDemodRestart(u8devID, eDMD_ID);
          }
        }
      }
    }
  }

  if(status == MXL_SUCCESS)
      return TRUE;
  else
      return FALSE;
}


DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL254) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL254",          // demod system name
    .data               = TUNER_MXL254,            // private data value
    .Init               = MXL254Tuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MXL254Tuner_SetTuner,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .CheckExist         = MXL254_TunerCheckExist,
    .Extension_Function = MDrv_Tuner_Null_Extension_Function
};

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MXL254) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MXL254",
     .data                         = DEMOD_MXL254,
     .init                         = MXL254_Demod_Init,
     .GetLock                      = MXL254_Demod_GetLock,
     .GetSNR                       = MXL254_Demod_GetSNR,
     .GetBER                       = MXL254_Demod_GetBER,
     .GetPWR                       = MDrv_Demod_null_GetPWR,
     .GetQuality                   = MDrv_Demod_null_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MXL254_Demod_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .CheckExist                   = MXL254_DemodCheckExist,
     .Extension_Function           = MXL254_Extension_Function,
 #if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
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
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
 #endif
};

#endif
