#ifndef __KERNEL__
#include <stdio.h>
#else
#include <linux/vmalloc.h>
#define free vfree
#define malloc(size) vmalloc((size))
#endif
#include "error.h"
#include "MsCommon.h"
#include "Board.h"

#include "drvTuner.h"
#include "drvDemod.h"
#include "drvTunerNull.h"
#include "drvDemodNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL254)
#include "drvTuner_MXL254.h"

#if MxL254_4_WIRE_MODE
static MXL_HRCLS_MPEG_CLK_RATE_E e4WireModeClk = MXL_HRCLS_MPEG_CLK_56_21MHz;
#endif

#if TS_PARALLEL_OUTPUT
static MXL_HRCLS_XPT_MODE_E eXPT_mode = MXL_HRCLS_XPT_MODE_PARALLEL;
#else
static MXL_HRCLS_XPT_MODE_E eXPT_mode = MXL_HRCLS_XPT_MODE_NO_MUX_4;
#endif

static MXL_HRCLS_XPT_HEADER_SIZE_E hdr_size = MXL_HRCLS_XPT_HDR_SIZE_12_BYTES;
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

static MS_U32 extra_hdr[4][3] = 
{
 {0x01010148, 0x01010101, 0x01010101}, 
 {0x02020249, 0x02020202, 0x02020202},
 {0x0303034a, 0x03030303, 0x03030303},
 {0x0404044b, 0x04040404, 0x04040404}
};

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

static MS_U32 MXL254_TSclk[MAX_FRONTEND_NUM][MAX_CHANNEL_COUNT] =
#if (MAX_FRONTEND_NUM == 1)
{{0,0,0,0}};
#elif (MAX_FRONTEND_NUM == 2)
{{0,0,0,0}, {0,0,0,0}};
#elif (MAX_FRONTEND_NUM == 3)
{{0,0,0,0}, {0,0,0,0},{0,0,0,0}};
#elif (MAX_FRONTEND_NUM == 4)
{{0,0,0,0}, {0,0,0,0},{0,0,0,0},{0,0,0,0}};
#else
{{0,0,0,0}, {0,0,0,0},{0,0,0,0},{0,0,0,0}};
#endif

static MS_U32 mxl_GetMaxTSClk(MS_U8 u8DemodIndex, MS_U8 u8StartIdx, MS_U8 u8EndInx)
{
   MS_U8 u8DMDIdx=0;
   MS_U32 u32MaxTSClk=0;
   MS_U8 u8DevID;

   u8DevID = u8DemodIndex/MAX_CHANNEL_COUNT;

   for(u8DMDIdx=u8StartIdx;u8DMDIdx<u8EndInx; u8DMDIdx++)
   {
       if(u32MaxTSClk < MXL254_TSclk[u8DevID][u8DMDIdx])
           u32MaxTSClk = MXL254_TSclk[u8DevID][u8DMDIdx];
   }

   return u32MaxTSClk;
}

static MXL_HRCLS_MPEG_CLK_RATE_E mxl_GetTSClkRate( MS_U8 u8DemodIndex, MXL_HRCLS_QAM_TYPE_E eConstellation, UINT16 symbolRatekSps)
{
   MS_U8 u8BitPerSymbol, u8ClkRateIdx=0;
   MS_U32 u32TSClkTab[8]={10550, 21090, 25960, 28160, 42180, 56210, 84370, 112500}; //KHz
   MS_U32 u32TSClk;
   MXL_HRCLS_MPEG_CLK_RATE_E eClkRate = MXL_HRCLS_MPEG_CLK_112_50MHz;
   MXL_HRCLS_DMD_ID_E eDMD_ID;
   MS_U8 u8DevID;

   u8DevID = u8DemodIndex/MAX_CHANNEL_COUNT;
   eDMD_ID = (MXL_HRCLS_DMD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);

   switch(eConstellation)
   {
        case MXL_HRCLS_QAM16:
           u8BitPerSymbol = 4;
           break;
       case MXL_HRCLS_QAM32:
           u8BitPerSymbol = 5;
           break;
       case MXL_HRCLS_QAM64:
           u8BitPerSymbol = 6;
           break;
       case MXL_HRCLS_QAM128:
           u8BitPerSymbol = 7;
           break;
       case MXL_HRCLS_QAM256:
       case MXL_HRCLS_QAM_AUTO:
       default:
           u8BitPerSymbol = 8;
           break;
   }

   u32TSClk = symbolRatekSps * u8BitPerSymbol *188/204; // for DVBC only, J83B not ready
   MXL254_TSclk[u8DevID][eDMD_ID] = u32TSClk;
   
   switch(eXPT_mode)
   {
       case MXL_HRCLS_XPT_MODE_PARALLEL:
           u32TSClk = mxl_GetMaxTSClk(u8DemodIndex,0,4);
           u32TSClk = (u32TSClk*4 * 200)/188/8;
           break;
       case MXL_HRCLS_XPT_MODE_MUX_2:
           if(eDMD_ID < MXL_HRCLS_DEMOD2)
               u32TSClk = mxl_GetMaxTSClk(u8DemodIndex,0,2);
           else
               u32TSClk = mxl_GetMaxTSClk(u8DemodIndex,2,4);

           u32TSClk = (u32TSClk* 2 * 200)/188; // 188 byte extend to 200 byte for extra header
           break;
       case MXL_HRCLS_XPT_MODE_MUX_1:

           u32TSClk = (u32TSClk* 4 * 200)/188; // 188 byte extend to 200 byte for extra header
           break;
       case MXL_HRCLS_XPT_MODE_NO_MUX_4:
       default:
           break;
   }

   for(u8ClkRateIdx=0;u8ClkRateIdx<8; u8ClkRateIdx++)
   {
      if(u32TSClk < u32TSClkTab[u8ClkRateIdx])
        break;
   }

   eClkRate = (MXL_HRCLS_MPEG_CLK_RATE_E)u8ClkRateIdx;
   DMD_DBG(("=== MxL254 TS Clock Rate: "));

   switch(eClkRate)
   {
       case MXL_HRCLS_MPEG_CLK_10_55MHz:
           DMD_DBG((" 10.55 Mhz\n"));
           break;
       case MXL_HRCLS_MPEG_CLK_21_09MHz:
           DMD_DBG((" 21.09 Mhz\n"));
           break;
       case MXL_HRCLS_MPEG_CLK_25_96MHz:
           DMD_DBG((" 25.96 Mhz\n"));
           break;
       case MXL_HRCLS_MPEG_CLK_42_18MHz:
           DMD_DBG((" 42.18 Mhz\n"));
           break;
       case MXL_HRCLS_MPEG_CLK_56_21MHz:
           DMD_DBG((" 56.21 Mhz\n"));
           break;
       case MXL_HRCLS_MPEG_CLK_84_37MHz:
           DMD_DBG((" 84.37 Mhz\n"));
           break;
       case MXL_HRCLS_MPEG_CLK_112_50MHz:
           DMD_DBG((" 112.50 Mhz\n"));
           break;    
       default:
           DMD_DBG((" NULL\n"));
           break;
   }

   return eClkRate;
}

static MS_U8 mxl_GetFreqSrearchRangeIdx( UINT16 symbolRatekSps)
{
    MS_U8 u8Idx = 0;
    __attribute__((unused)) int SearchRangeKhz;//debug only

    u8Idx = (MS_U8)(MxL254_FREQ_SEARCH_RANGE*256/(MS_U32)symbolRatekSps);
    if(u8Idx > 15)
        u8Idx = 15;

    SearchRangeKhz = (int)(symbolRatekSps*u8Idx/256);
    DMD_DBG((" Freq Search Range = +/- %d KHz\n", SearchRangeKhz));

    return u8Idx;
}

static MXL_HRCLS_XPT_OUTPUT_ID_E mxl_Get_XPT_ID(MXL_HRCLS_DMD_ID_E eDMD_ID)
{
   MXL_HRCLS_XPT_OUTPUT_ID_E eXPT_ID;
   
   switch(eXPT_mode)
   {
       case MXL_HRCLS_XPT_MODE_PARALLEL:
           eXPT_ID = MXL_HRCLS_XPT_OUT_0;
           break;
       case MXL_HRCLS_XPT_MODE_MUX_2:
           if(eDMD_ID < MXL_HRCLS_DEMOD2)
               eXPT_ID = MXL_HRCLS_XPT_OUT_1;
           else
               eXPT_ID = MXL_HRCLS_XPT_OUT_2;

           break;
       case MXL_HRCLS_XPT_MODE_MUX_1:
           eXPT_ID = MXL_HRCLS_XPT_OUT_2;
           break;
       case MXL_HRCLS_XPT_MODE_NO_MUX_4:
       default:
	   	   if(eDMD_ID == MXL_HRCLS_DEMOD0)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_0;
		   else if(eDMD_ID == MXL_HRCLS_DEMOD1)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_1;
		   else if(eDMD_ID == MXL_HRCLS_DEMOD2)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_2;
	       else if(eDMD_ID == MXL_HRCLS_DEMOD3)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_3;
		   else if(eDMD_ID == MXL_HRCLS_DEMOD4)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_4;
           else if(eDMD_ID == MXL_HRCLS_DEMOD5)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_5;
		   else if(eDMD_ID == MXL_HRCLS_DEMOD6)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_6;
	       else if(eDMD_ID == MXL_HRCLS_DEMOD7)
		       eXPT_ID = MXL_HRCLS_XPT_OUT_7;
		   else
		       eXPT_ID = MXL_HRCLS_XPT_OUT_0;

           break;
   }

   return eXPT_ID;
}


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
           ret = MXL_HRCLS_ANNEX_A;
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
           ret = MXL_HRCLS_QAM128;
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

extern MXL_STATUS_E MxL_HRCLS_Ctrl_ReadRegisterField(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 * valuePtr);
static MS_BOOL mxl_reqDevType(MS_U8 u8DevIdx, MXL_HRCLS_DEVICE_E* peDevType)
{
   MXL_STATUS_E status;
   UINT16 flavorId;
   
   status = MxL_HRCLS_Ctrl_ReadRegisterField(u8DevIdx, ANA_DIG_SOC_FLVR, &flavorId);
   if (status != MXL_SUCCESS)
       return FALSE;
   
   flavorId >>= 4;
   flavorId &= 0x0f;
   switch(flavorId)
   {
       case MXL_FLVR_MXL254:
              *peDevType = MXL_HRCLS_DEVICE_254;
              TUNER_DBG(("[HRCLS][%d] DeviceType: MXL254\n", u8DevIdx));
              break;
       case MXL_FLVR_MXL214:
              *peDevType = MXL_HRCLS_DEVICE_214;
              TUNER_DBG(("[HRCLS][%d] DeviceType: MXL214\n", u8DevIdx));
              break;
       default:
              TUNER_ERR(("[HRCLS][%d] DeviceType NOT SUPPORT so far\n", u8DevIdx));
              return FALSE;              
   }

   return TRUE;
}

static MS_BOOL _mxl254_init(MS_U8 u8DevIdx)
{
   MXL_STATUS_E status;
   MXL_HRCLS_DEV_VER_T devVerInfo = {0};
   MXL_HRCLS_DEVICE_E requiredDeviceType = MXL_HRCLS_DEVICE_254;

   initData.i2cAddress = MXL254_I2C_ADDR; 

   if(!mxl_reqDevType(u8DevIdx, &requiredDeviceType))
       return FALSE;
   
   status = MxLWare_HRCLS_API_CfgDrvInit(u8DevIdx, (void *) &initData, requiredDeviceType);
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

     status = MxLWare_HRCLS_API_CfgTunerDsCalDataLoad(u8DevIdx);
     if (status != MXL_SUCCESS)
     {
       printf("[HRCLS][%d] Calibration data not available. Power reporting will not be accurate\n", u8DevIdx);
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

   status = MxLWare_HRCLS_API_CfgXpt(u8DevIdx, eXPT_mode); 

 #if MxL254_4_WIRE_MODE
   status &= MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams(u8DevIdx, MXL_HRCLS_MPEG_CLK_POSITIVE, MXL_HRCLS_MPEG_DRV_MODE_1X, e4WireModeClk);
 #endif

  
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
    
    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8devID].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

 
    if(MXL254_init[u8devID] == TRUE)
        return TRUE;

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
  MXL_HRCLS_DEVICE_E eDevType = MXL_HRCLS_DEVICE_254;

  u8devID = u8TunerIndex; // here u8TunerIndex means device index

  if(!MXL254_detect[u8devID])
  {
      status = MxLWare_HRCLS_OEM_ReadRegister(u8devID, 0x60E4, &regData);
      if((MXL_SUCCESS == status) && (MXL254_CHIP_ID == regData))
      {
          if(mxl_reqDevType(u8devID, &eDevType))
          {
             if(eDevType == MXL_HRCLS_DEVICE_214)
             {
                GET_TUNER_ENTRY_NODE(TUNER_MXL254).name = "TUNER_MXL214"; 
                GET_DEMOD_ENTRY_NODE(DEMOD_MXL254).name = "DEMOD_MXL214"; 
                TUNER_DBG(("== Get MXL214 ID !!\n"));
             }
             else
             {
                TUNER_DBG(("== Get MXL254 ID !!\n"));
             }
          }
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
  TUNER_DBG(("== MXL254/MXL214 have been detected \n"));
  return TRUE;
}

MS_BOOL MXL254_DemodCheckExist(MS_U8 u8TunerIndex, MS_U8* pu8SlaveID)
{
  MXL_STATUS_E status = MXL_FAILURE;
  UINT16 regData = 0;
  MS_U8 u8devID;
  MXL_HRCLS_DEVICE_E eDevType = MXL_HRCLS_DEVICE_254;

  u8devID = u8TunerIndex; // here u8TunerIndex means device index

  if(!MXL254_detect[u8devID])
  {
      status = MxLWare_HRCLS_OEM_ReadRegister(u8devID, 0x60E4, &regData);
      if((MXL_SUCCESS == status) && (MXL254_CHIP_ID == regData))
      {
          if(mxl_reqDevType(u8devID, &eDevType))
          {
             if(eDevType == MXL_HRCLS_DEVICE_214)
             {
                GET_TUNER_ENTRY_NODE(TUNER_MXL254).name = "TUNER_MXL214"; 
                GET_DEMOD_ENTRY_NODE(DEMOD_MXL254).name = "DEMOD_MXL214"; 
                TUNER_DBG(("== Get MXL214 ID !!\n"));
             }
             else
             {
                TUNER_DBG(("== Get MXL254 ID !!\n"));
             }
           }

          MXL254_detect[u8devID] = TRUE;
          *pu8SlaveID = MXL254_SLAVE_ID;
          return TRUE;
      }
      else
      {
          DMD_ERR(("== Get MXL254 ID FAIL !!\n"));
          return FALSE;
      }
  }
  DMD_DBG(("== MXL254/MXL214 have been detected \n"));
  *pu8SlaveID = MXL254_SLAVE_ID;
  return TRUE;
}

MS_BOOL MXL254_Demod_SetPIDFilter(MS_U8 u8DemodIndex, MXL_HRCLS_XPT_PID_ENTRY_T *pPIDFlt)
{
    MXL_STATUS_E status = MXL_SUCCESS;
    MXL_HRCLS_DMD_ID_E eDMD_ID;
    UINT8 u8devID;

    eDMD_ID = (MXL_HRCLS_DMD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    status &= MxLWare_HRCLS_API_CfgXptPidTable(u8devID, eDMD_ID, 1, pPIDFlt, MXL_FALSE);

    if(MXL_SUCCESS == status)
        return TRUE;
    else
        return FALSE;
}


MS_BOOL MXL254_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret=TRUE;
    MS_U8 u8devID;
    DEMOD_PID_FILTER stPIDFlt;
    MXL_HRCLS_XPT_PID_ENTRY_T stMXL_PIDFlt;

    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_FINALIZE:
            MXL254_init[u8devID] = FALSE;
            MxLWare_HRCLS_OEM_DrvDeInit(u8devID);
            break;
        case DEMOD_EXT_FUNC_SET_PID_FILTER:
            stPIDFlt = *(DEMOD_PID_FILTER*)data;
            stMXL_PIDFlt.pidValue = (UINT16)stPIDFlt.u16PIDValue;
            stMXL_PIDFlt.pidRemapValue = (UINT16)stPIDFlt.u16PIDRemapValue;
            stMXL_PIDFlt.pidDrop = (MXL_BOOL_E)stPIDFlt.bPIDDrop;
            bret &= MXL254_Demod_SetPIDFilter(u8DemodIndex, &stMXL_PIDFlt);
            break;            
        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
            
    }
    return bret;
}

MS_BOOL MXL254_Tuner_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_S32 s32PWR;
    
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            if(!MXL254_Demod_GetPWR(u8TunerIndex, &s32PWR))
                return FALSE;
            else
                *(int*)data = (int)s32PWR;
            
            break;

        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
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

#ifdef __KERNEL__
MS_BOOL MXL254_Demod_GetSNR(MS_U8 u8DemodIndex, MS_S32 *ps32SNR)
#else
MS_BOOL MXL254_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
#endif
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
#ifdef __KERNEL__
     *ps32SNR = (MS_S32)(u16SNR)/10;
#else
     *pfSNR = (float)(u16SNR)/10;
#endif
     return TRUE;
   }
   else
    return FALSE;
}

#ifdef __KERNEL__
MS_BOOL MXL254_Demod_GetBER(MS_U8 u8DemodIndex, MS_S32 *ps32BER)
#else
MS_BOOL MXL254_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
#endif
{
    MXL_HRCLS_DMD_STAT_CNT_T stState;
    MXL_STATUS_E status;
    MXL_HRCLS_DMD_ID_E eDMD_ID;
    UINT8 u8devID;
   
    eDMD_ID = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    status = MxLWare_HRCLS_API_CfgDemodErrorStatClear(u8devID, eDMD_ID);
    status = MxLWare_HRCLS_API_ReqDemodErrorStat(u8devID, eDMD_ID, &stState);

   if(status == MXL_SUCCESS)
   {
     switch(*InitParam[u8devID].pCur_Broadcast_type)
     {
        case DVBC:
        default:
#ifdef __KERNEL__
            *ps32BER = 9*(((MS_S64)(stState.ErrMpeg))*1000)/188/8/(MS_S64)(stState.ReceivedMpeg)/1000;
#else
            *pfBER = 9*(float)(stState.ErrMpeg)/188/8/(float)(stState.ReceivedMpeg);
#endif
            break;
        case J83B:
#ifdef __KERNEL__
            *ps32BER = 10 *(((MS_S64)(stState.ErrMpeg))*1000)/188/8/(MS_S64)(stState.ReceivedMpeg)/1000;
#else
            *pfBER = 10 *(float)(stState.ErrMpeg)/188/8/(float)(stState.ReceivedMpeg);
#endif
            break;
     }
     return TRUE;
   }
   else
   {
#ifdef __KERNEL__
        *ps32BER = 0;
#else
        *pfBER = 0;
#endif
        return FALSE;
   }

}

MS_BOOL MXL254_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MXL_HRCLS_CHAN_ID_E chanId;
    UINT8 u8devID;
    UINT16 rxPwrIndBuV;
        
    MXL_HRCLS_RX_PWR_ACCURACY_E accuracy = MXL_HRCLS_PWR_AVERAGED;
   
    chanId = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    if(MXL_SUCCESS != MxLWare_HRCLS_API_ReqTunerRxPwr(u8devID, chanId, &rxPwrIndBuV, &accuracy))
    {
       return FALSE;
    }
    else
    {
       *ps32Signal = (((MS_S32)(rxPwrIndBuV) - 1070) + 5)/10;
       return TRUE;
    }
}

MS_BOOL MXL254_Demod_GetSSI(MS_U8 u8DemodIndex,MS_U16 *pu16SSI)
{
    MXL_HRCLS_CHAN_ID_E chanId;
    UINT8 u8devID;
    UINT16 rxPwrIndBuV;
#ifdef __KERNEL__
    MS_S64 rxPwrIndBm;
#else
    float rxPwrIndBm;
#endif

    MXL_HRCLS_RX_PWR_ACCURACY_E accuracy = MXL_HRCLS_PWR_AVERAGED;
   
    chanId = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    if(MXL_SUCCESS != MxLWare_HRCLS_API_ReqTunerRxPwr(u8devID, chanId, &rxPwrIndBuV, &accuracy))
    {
       return FALSE;
    }
    else
    {
#ifdef __KERNEL__
       rxPwrIndBm = (((((MS_S64)rxPwrIndBuV) - 1070) + 5)*1000)/10;
#else
       rxPwrIndBm = (((float)(rxPwrIndBuV) - 1070) + 5)/10;
#endif
    }

#ifdef __KERNEL__
     if(rxPwrIndBm <= -85*1000)
        {*pu16SSI = 0;}
    else if (rxPwrIndBm <= -80*1000)
        {*pu16SSI = (MS_U16)((0*1000 + (rxPwrIndBm+85*1000)*10/5)/1000);}
    else if (rxPwrIndBm <= -75*1000)
        {*pu16SSI = (MS_U16)((10*1000 + (rxPwrIndBm+80*1000)*20/5)/1000);}
    else if (rxPwrIndBm <= -70*1000)
        {*pu16SSI = (MS_U16)((30*1000 + (rxPwrIndBm+75*1000)*30/5)/1000);}
    else if (rxPwrIndBm <= -65*1000)
        {*pu16SSI = (MS_U16)((60*1000 + (rxPwrIndBm+70*1000)*10/5)/1000);}
    else if (rxPwrIndBm <= -55*1000)
        {*pu16SSI = (MS_U16)((70*1000 + (rxPwrIndBm+65*1000)*20/10)/1000);}
    else if (rxPwrIndBm <= -45*1000)
        {*pu16SSI = (MS_U16)((90*1000 + (rxPwrIndBm+55*1000)*10/10)/1000);}
    else
        {*pu16SSI = 100;}
#else
     if(rxPwrIndBm <= -85.0f)
        {*pu16SSI = 0;}
    else if (rxPwrIndBm <= -80.0f)
        {*pu16SSI = (MS_U16)(0.0f + (rxPwrIndBm+85.0f)*10.0f/5.0f);}
    else if (rxPwrIndBm <= -75.0f)
        {*pu16SSI = (MS_U16)(10.0f + (rxPwrIndBm+80.0f)*20.0f/5.0f);}
    else if (rxPwrIndBm <= -70.0f)
        {*pu16SSI = (MS_U16)(30.0f + (rxPwrIndBm+75.0f)*30.0f/5.0f);}
    else if (rxPwrIndBm <= -65.0f)
        {*pu16SSI = (MS_U16)(60.0f + (rxPwrIndBm+70.0f)*10.0f/5.0f);}
    else if (rxPwrIndBm <= -55.0f)
        {*pu16SSI = (MS_U16)(70.0f + (rxPwrIndBm+65.0f)*20.0f/10.0f);}
    else if (rxPwrIndBm <= -45.0f)
        {*pu16SSI = (MS_U16)(90.0f + (rxPwrIndBm+55.0f)*10.0f/10.0f);}
    else
        {*pu16SSI = 100;}
#endif
    return TRUE;
}


MS_BOOL MXL254_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{     
    MXL_HRCLS_ANNEX_TYPE_E  annexType = 0;
    MXL_HRCLS_QAM_TYPE_E    qamType = 0;
    MXL_HRCLS_IQ_FLIP_E adcIqFlip = MXL_HRCLS_IQ_NORMAL;
    MXL_HRCLS_DMD_ID_E eDMD_ID;
    UINT8 u8devID;
    int CFO;

    eDMD_ID = (MXL_HRCLS_DMD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    if(MXL_SUCCESS != MxLWare_HRCLS_API_ReqDemodAnnexQamType(u8devID,eDMD_ID,&annexType,&qamType))
    {
        return FALSE;
    }

    if(MXL_SUCCESS != MxLWare_HRCLS_API_ReqDemodAdcIqFlip(u8devID,eDMD_ID,&adcIqFlip))
    {
        return FALSE;
    }
    
    if(MXL_SUCCESS != MxLWare_HRCLS_API_ReqDemodCarrierOffset(u8devID,eDMD_ID,&CFO))
    {
       return FALSE;
    }

    
    switch(qamType)
    {
        case MXL_HRCLS_QAM16:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM16;
            break;
        case MXL_HRCLS_QAM32:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM32;
            break;
        case MXL_HRCLS_QAM64:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM64;
            break;
        case MXL_HRCLS_QAM128:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM128;
            break;
        case MXL_HRCLS_QAM256:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM256;
            break;
        default:
            pParam->CabParam.eConstellation = DEMOD_CAB_QAM64;
            break;
    }

	if(adcIqFlip == MXL_HRCLS_IQ_NORMAL)
        pParam->CabParam.eIQMode = DEMOD_CAB_IQ_NORMAL;
	else if(adcIqFlip == MXL_HRCLS_IQ_FLIPPED)
		pParam->CabParam.eIQMode = DEMOD_CAB_IQ_INVERT;
#ifdef __KERNEL__
    pParam->CabParam.s64CFO = (CFO)/1000;
#else
    pParam->CabParam.fCFO = (float)(CFO)/1000;
#endif
    return TRUE;

}

MS_BOOL MXL254_Demod_GetPacketError(MS_U8 u8DemodIndex, MS_U16 *u16_data)
{
     MXL_HRCLS_DMD_STAT_CNT_T stState;
     MXL_STATUS_E status;
     MXL_HRCLS_DMD_ID_E eDMD_ID;
     UINT8 u8devID;
     MS_U8 u8PollingCnt=0;
    
     eDMD_ID = (MXL_HRCLS_CHAN_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
     u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    
     status = MxLWare_HRCLS_API_CfgDemodErrorStatClear(u8devID, eDMD_ID);

     do
     {
         status = MxLWare_HRCLS_API_ReqDemodErrorStat(u8devID, eDMD_ID, &stState);
         if(stState.ReceivedMpeg >= 6)
            break;
         MsOS_DelayTask(1);
         u8PollingCnt++;
     }while(u8PollingCnt<10);

    DMD_DBG(("Pkg Error = %d/%d\n", (int)stState.ErrMpeg, (int)stState.ReceivedMpeg));
    if(status == MXL_SUCCESS)
    {
      *u16_data = (MS_U16)stState.ErrMpeg;
      return TRUE;
    }
    else
    {
      *u16_data = 0;
      return FALSE;
    }    
}


MS_BOOL MXL254_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  MXL_HRCLS_ANNEX_TYPE_E annexType;
  MXL_HRCLS_QAM_TYPE_E qamType = mxl_QAM_TYPE_MAP(pParam->CabParam.eConstellation);
  UINT16 symbolRatekSps = (UINT16)pParam->CabParam.u16SymbolRate;
  MXL_HRCLS_DMD_ID_E eDMD_ID;
  UINT8 u8devID, u8SreachRangeIdx;
  MXL_HRCLS_MPEGOUT_PARAM_T mpegParams;
  MXL_HRCLS_XPT_MPEGOUT_PARAM_T mpegXptParams;
  MS_U32* pu32hdr = NULL;
  MS_U8 u8hdr_size_in_U32, u8hdr_idx;
  MXL_HRCLS_MPEG_CLK_RATE_E eClkRate;
  MXL_HRCLS_DEVICE_E eDevType = MXL_HRCLS_DEVICE_254;

  u8hdr_size_in_U32 = (MS_U8)hdr_size;
  eDMD_ID = (MXL_HRCLS_DMD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
  u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
  annexType = mxl_ANNEX_TYPE_MAP(*InitParam[u8devID].pCur_Broadcast_type);

  //TUNER_DBG(("### MXL254_Demod_Restart eDMD_ID = %x, u8devID = %x\n", (UINT8)eDMD_ID, u8devID);

  eClkRate = mxl_GetTSClkRate(eDMD_ID, qamType,symbolRatekSps);
  u8SreachRangeIdx = mxl_GetFreqSrearchRangeIdx(symbolRatekSps);
  #if MxL254_4_WIRE_MODE
  // in 4-wire mode, this value has to be the same as the value in MxLWare_HRCLS_API_CfgDemodMpegOutGlobalParams 
  eClkRate = e4WireModeClk;
  #endif
    
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
      // sample MPEG configuration
      mpegXptParams.enable = MXL_ENABLE;
      mpegXptParams.lsbOrMsbFirst = MXL_HRCLS_MPEG_SERIAL_MSB_1ST;
      mpegXptParams.mpegSyncPulseWidth = MXL_HRCLS_MPEG_SYNC_WIDTH_BYTE;
      mpegXptParams.mpegValidPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegXptParams.mpegSyncPol = MXL_HRCLS_MPEG_ACTIVE_HIGH;
      mpegXptParams.mpegClkPol = MXL_HRCLS_MPEG_CLK_NEGATIVE;
      mpegXptParams.clkFreq = eClkRate;
      mpegXptParams.mpegPadDrv.padDrvMpegSyn = MXL_HRCLS_MPEG_DRV_MODE_2X;  
      mpegXptParams.mpegPadDrv.padDrvMpegDat = MXL_HRCLS_MPEG_DRV_MODE_2X;  
      mpegXptParams.mpegPadDrv.padDrvMpegVal = MXL_HRCLS_MPEG_DRV_MODE_2X;  

      // In MxL254 and NO_MUX_4 mode, outputId = demodId
      // Check MxLWare API User Guide for other modes' mappings
      status = MxLWare_HRCLS_API_CfgXptOutput(u8devID, mxl_Get_XPT_ID(eDMD_ID), &mpegXptParams);
  }


  status = mxl_waitForChannelLock(u8DemodIndex);
  if (status == MXL_SUCCESS)
  {
    status = MxLWare_HRCLS_API_CfgDemodEnable(u8devID, eDMD_ID, MXL_TRUE);
    if (status == MXL_SUCCESS)
    {
      status = MxLWare_HRCLS_API_CfgDemodAdcIqFlip(u8devID, eDMD_ID, MXL_HRCLS_IQ_AUTO);
      if (status == MXL_SUCCESS)
      {
        status = MxLWare_HRCLS_API_CfgDemodAnnexQamType(u8devID, eDMD_ID, annexType, qamType);
        if (status == MXL_SUCCESS)
        {
          status = MxLWare_HRCLS_API_CfgDemodSymbolRate(u8devID, eDMD_ID, symbolRatekSps * 1000, symbolRatekSps * 1000);

          if (status == MXL_SUCCESS)
          {
            status = MxLWare_HRCLS_API_CfgDemodFreqSearchRange(u8devID, eDMD_ID, u8SreachRangeIdx);
          
            if (status == MXL_SUCCESS)
            {
              status = MxLWare_HRCLS_API_CfgDemodRestart(u8devID, eDMD_ID);
            }
          }
        }
       }
     }
    }

  //if(MXL_HRCLS_XPT_MODE_PARALLEL == eXPT_mode)
  if (MXL_HRCLS_XPT_MODE_NO_MUX_4 != eXPT_mode)
  {
      if(mxl_reqDevType(u8devID, &eDevType))
      {
          if(MXL_HRCLS_DEVICE_214 == eDevType)
          {
             DMD_ERR(("[HRCLS] MXL214 NOT support XPT and parallel mode\n"));
            return FALSE;
          }
      }
      else
        return FALSE;
      
      pu32hdr = (MS_U32*)malloc(sizeof(MS_U32)*u8hdr_size_in_U32);
      if(NULL == pu32hdr)
        return FALSE;
      
      for(u8hdr_idx = 0; u8hdr_idx < u8hdr_size_in_U32; u8hdr_idx++)
      {
          *(pu32hdr + u8hdr_idx) = extra_hdr[eDMD_ID][u8hdr_idx];
      }

      status &= MxLWare_HRCLS_API_CfgXptHeaderWords(u8devID, eDMD_ID, hdr_size, (UINT32*)pu32hdr);
	  free(pu32hdr);
      if(status != MXL_SUCCESS)
      {
          DMD_DBG(("[HRCLS] Set Extra Header API Fail  <<---------------------------------\n"));
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
    .Extension_Function = MXL254_Tuner_Extension_Function
};

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MXL254) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MXL254",
     .data                         = DEMOD_MXL254,
     .SupportINT                   = FALSE,
     .init                         = MXL254_Demod_Init,
     .GetLock                      = MXL254_Demod_GetLock,
     .GetSNR                       = MXL254_Demod_GetSNR,
     .GetBER                       = MXL254_Demod_GetBER,
     .GetPWR                       = MXL254_Demod_GetPWR,
     .GetSSI                       = MXL254_Demod_GetSSI,
     .GetQuality                   = MDrv_Demod_null_GetSignalQuality,
     .GetParam                     = MXL254_Demod_GetParam,
     .Restart                      = MXL254_Demod_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .CheckExist                   = MXL254_DemodCheckExist,
     .Extension_Function           = MXL254_Extension_Function,
     .Get_Packet_Error             = MXL254_Demod_GetPacketError,
 #ifdef FE_AUTO_TEST
    .ReadReg                       = MDrv_Demod_null_ReadReg,
    .WriteReg                      = MDrv_Demod_null_WriteReg,
 #endif    
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
