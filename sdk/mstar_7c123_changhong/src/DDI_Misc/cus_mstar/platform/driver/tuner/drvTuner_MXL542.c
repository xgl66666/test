
#include <stdio.h>
#include "error.h"
#include "MsCommon.h"
#include "Board.h"

#include "drvTuner.h"
#include "drvDemod.h"
#include "drvTunerNull.h"
#include "drvDemodNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL542)
#include "drvTuner_MXL542.h"

#define EXAMPLE_DEV_MAX   2
#define MAX_CHANNEL_COUNT 4
#define MXL_XTAL_CAP_VALUE 33 
#define SHOW_PARAM_AFTER_LOCK 0

static UINT8 MxL542_LIB[] =
{
  #include "MxL_5xx_FW.dat"
};


char *standard[MXL_HYDRA_DVBS2+1] = {("DSS"), ("DVB-S"), ("DVB-S2")};
char *fec[MXL_HYDRA_FEC_9_10+1] = {("AUTO"), ("1/2"), ("3/5"), ("2/3"), ("3/4"), ("4/5"), ("5/6"), ("6/7"), ("7/8"), ("8/9"), ("9/10") };
char *modulation[MXL_HYDRA_MOD_8PSK+1] = {("AUTO"), ("QPSK"), ("8PSK")};
char *pilots[MXL_HYDRA_PILOTS_AUTO+1] = {("OFF"), ("ON"), ("AUTO")};
char *rolloff[MXL_HYDRA_ROLLOFF_0_35+1] = {("AUTO"), (".20"), (".25"), (".35")};
char *spectrum[MXL_HYDRA_SPECTRUM_NON_INVERTED+1] = {("AUTO"), ("INVERTED"), ("NORMAL")};

//-----------------------------------------------------------------
//Blind Scan
//-----------------------------------------------------------------
static TONE_DETECTION_INFO* pstTone_Info = NULL;
static BLIND_SCAN_CHAN_INFO* pstCh_Info = NULL;
//static SINT16* pPowerBuf = NULL; //for power spectrum data
//static UINT32 numOfFreqSteps=100;
static UINT8 numOfTones = 30;
static UINT8 Scaned_Cnt = 0;
static UINT8 Find_Cnt = 0;
//-------------------------------------------------------------------------------------------------
//  static function
//---------------------------------------------------------------------------------------------
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];
static MS_BOOL MXL542_detect[MAX_FRONTEND_NUM] =
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

static MS_BOOL MXL542_init[MAX_FRONTEND_NUM] =
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

oem_data_t oem_data[MAX_FRONTEND_NUM];

static MS_BOOL _mxl542_clear_blind_scan_param(MS_U8 u8DemodIndex)
{
   if(pstTone_Info != NULL)
   {
       free(pstTone_Info);
       pstTone_Info = NULL;
   }

   if(pstCh_Info != NULL)
   {
       free(pstCh_Info);
       pstCh_Info = NULL;
   }

   numOfTones = 50;
   Scaned_Cnt = 0;
   Find_Cnt = 0;

   return TRUE;
}

//Modify form MxLWare_HYDRA_API_ReqToneFrequency
static MS_BOOL _mxl542_ReqToneFrequency(UINT8 devId,
                                                MXL_HYDRA_TUNER_ID_E tunerId,
                                                MXL_HYDRA_DEMOD_ID_E demodId,
                                                UINT32 startFrequencyInKhz,
                                                UINT32 endFrequencyInKhz,
                                                SINT16 thresholdLevel,
                                                UINT8  *numOfTones,
                                                TONE_DETECTION_INFO *toneInfoPtr)
{
  MXL_HYDRA_CONTEXT_T *devHandlePtr;
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  MXL_HYDRA_SPECTRUM_ERROR_CODE_E spectrumReadbackStatus;
  SINT16 *powerBuff = NULL;
  UINT16 numOfFreqSteps = 0, equalCount=0, totalCount = 0;
  UINT32 frequency = 0, peakFrequency = 0;
  SINT16 prevValue = 0, peakPower = 0;
  UINT32 freqStepSize = MXL_HYDRA_FREQ_408_20_STEP_SIZE;  // 408.20KHz frequency step size;
  MXL_BOOL_E risingEdge = MXL_FALSE;
  UINT8 maxTonesAllowed = 0;
  UINT16 Cross_Point_1st;
  SINT16 max_slope = 0;

  MXLENTERAPI(MXL_HYDRA_PRINT("tunerId=%d, demodId=%d, startFrequencyInKhz=%d\n", tunerId, demodId, startFrequencyInKhz););

  mxlStatus = MxLWare_HYDRA_Ctrl_GetDeviceContext(devId, &devHandlePtr);
  if (mxlStatus == MXL_SUCCESS)
  {
    // validate input parameter
    if ((toneInfoPtr) && (numOfTones) && (*numOfTones) && ((startFrequencyInKhz >= MXL_HYDRA_SPECTRUM_MIN_FREQ_KHZ) && (startFrequencyInKhz < MXL_HYDRA_SPECTRUM_MAX_FREQ_KHZ)) \
        && ((endFrequencyInKhz > MXL_HYDRA_SPECTRUM_MIN_FREQ_KHZ) && (endFrequencyInKhz <= MXL_HYDRA_SPECTRUM_MAX_FREQ_KHZ)) \
        && (MAX_STEP_SIZE_24_XTAL_408_20_KHZ*sizeof(SINT16) <= MXL_HYDRA_OEM_MAX_BLOCK_WRITE_LENGTH))
    {
      startFrequencyInKhz = ((startFrequencyInKhz*1000/freqStepSize)*freqStepSize)/1000;
      frequency = startFrequencyInKhz*1000;
      DMD_DBG(("StartFrequency: %d\n", startFrequencyInKhz));
      numOfFreqSteps = (endFrequencyInKhz - startFrequencyInKhz)*1000/freqStepSize; // Frequency band and 408.20KHz is step size.
      maxTonesAllowed = *numOfTones;

      powerBuff = (SINT16 *)MxLWare_HYDRA_OEM_MemAlloc(numOfFreqSteps*sizeof(SINT16));

      if (powerBuff)
      {
        mxlStatus = MxLWare_HYDRA_API_ReqTunerPowerSpectrum(devId, tunerId, demodId, startFrequencyInKhz, numOfFreqSteps, powerBuff, &spectrumReadbackStatus);

        DMD_DBG(("numOfFreqSteps: %d spectrumReadbackStatus: %d\n", numOfFreqSteps, spectrumReadbackStatus));

        if ((mxlStatus == MXL_SUCCESS) && (spectrumReadbackStatus == MXL_SPECTRUM_NO_ERROR))
        {
          prevValue = powerBuff[0];
          peakPower = powerBuff[0];
          Cross_Point_1st = numOfFreqSteps;
          for (totalCount = 1, *numOfTones = 0; (totalCount < numOfFreqSteps) && (*numOfTones < maxTonesAllowed); totalCount++)
          {
            if (powerBuff[totalCount] >= thresholdLevel)
            {
              if (powerBuff[totalCount] > prevValue) // Rising
              {
                peakPower = powerBuff[totalCount];
                peakFrequency = frequency;
                risingEdge = MXL_TRUE;
                if(Cross_Point_1st > totalCount)
                {
                    Cross_Point_1st = totalCount;
                }

                if(((powerBuff[totalCount] - prevValue) > max_slope) && ((powerBuff[totalCount] - prevValue) < 5)) // aviod glitch
                {
                    max_slope = (powerBuff[totalCount] - prevValue);
                }
              }
              else if ((powerBuff[totalCount] == prevValue)&& (risingEdge == MXL_TRUE)) // Same Level
              {
                peakFrequency = frequency;
                equalCount++;
              }
              else   // Falling  
              {
                //risingEdge = MXL_FALSE;
              }
            }
            
            if ((peakPower >= thresholdLevel) &&\
                 ((peakPower - powerBuff[totalCount]) >= 3) &&\
                 ((prevValue - powerBuff[totalCount]) >= max_slope))  // Power level should be half to treat it as a tone
            {
              toneInfoPtr[*numOfTones].frequency_inHz= peakFrequency - ((equalCount+1)/2)*freqStepSize;
              toneInfoPtr[*numOfTones].signalLevel = peakPower;

              toneInfoPtr[*numOfTones].ToneBW_N_inHz= equalCount*freqStepSize;

              //toneInfoPtr[*numOfTones].symbol_rate= (toneInfoPtr[*numOfTones].frequency - max_slope_frequency)*2;//equalCount*freqStepSize*11/10;
              //toneInfoPtr[*numOfTones].ToneBW_Wide= (2*totalCount - 2*Cross_Point_1st - equalCount)*freqStepSize;
              toneInfoPtr[*numOfTones].ToneBW_W_inHz= (toneInfoPtr[*numOfTones].frequency_inHz- startFrequencyInKhz*1000 - Cross_Point_1st*freqStepSize)*2;
              //toneInfoPtr[*numOfTones].ToneBW_Wide = toneInfoPtr[*numOfTones].ToneBW_Wide;
              peakPower = 2*thresholdLevel;
              equalCount = 0;
              Cross_Point_1st = numOfFreqSteps;
              max_slope = 0;
              risingEdge = MXL_FALSE;
              (*numOfTones)++;
            }
            
            prevValue = powerBuff[totalCount];
            frequency = frequency + freqStepSize; // 408.20KHz frequency step size;
          }
          DMD_DBG(("Total Tones: %d\n", *numOfTones));
        }
        MxLWare_HYDRA_OEM_MemFree((void *)powerBuff);
      }
      else
        mxlStatus = MXL_FAILURE;
        if (powerBuff == NULL)
          DMD_DBG(("Memory allocation failed\n"));
    }
    else
      mxlStatus = MXL_INVALID_PARAMETER;
  }

  if(MXL_SUCCESS == mxlStatus)
      return TRUE;
  else
      return FALSE;
}

static  MS_BOOL  _mxl542_Decide_LNB_LO(TUNER_MS_SAT_PARAM *pSATParam)
{
      MS_S32 s32Freq;
     // calc Mid Freq & LNB control(22K on/off)
     if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
     {
        *pSATParam->pbIsHiLOF= FALSE;
     }
     else // 2LOF
     {
         MS_U32 u32MidFreq;
         MS_S32 s32Offset = 0;
         s32Freq  = pSATParam->u32RF_FREQ;
         u32MidFreq =  abs(s32Freq - pSATParam->u16LoLOF) ;
         s32Offset = pSATParam->u16LoLOF+ 2150 - pSATParam->u16HiLOF - 950;
         if(s32Offset < 0)
              s32Offset = 0;
          else
             s32Offset /= 2;

         if( u32MidFreq <= (2150-s32Offset))
              *pSATParam->pbIsHiLOF = FALSE;
          else
              *pSATParam->pbIsHiLOF = TRUE;     
     }
     return TRUE;
}


static MS_BOOL _mxl542_init(MS_U8 u8DevIdx, 
                               MXL_HYDRA_AUX_CTRL_MODE_E lnbInterface,
                               MXL_HYDRA_TS_MUX_TYPE_E tsMuxType,
                               MXL_HYDRA_MPEG_MODE_E tsInterfaceMode,
                               UINT8 maxTsInterfaceClkRate)
{
   MXL_STATUS_E status;
   MXL_HYDRA_DEV_XTAL_T xtalParamPtr;
   UINT32 firmwareSize = 0;
   MXL_HYDRA_VER_INFO_T versionInfo;
   MXL_HYDRA_MPEGOUT_PARAM_T mpegInterfaceCfg;
   MS_U8 j;


   firmwareSize = sizeof(MxL542_LIB);
   oem_data[u8DevIdx].i2cAddress = MXL542_SLAVE_ID;
   oem_data[u8DevIdx].Mutex = -1;
   status = MxLWare_HYDRA_API_CfgDrvInit(u8DevIdx, (void *) &oem_data[u8DevIdx], MXL_HYDRA_DEVICE_542C);
   if (status == MXL_ALREADY_INITIALIZED)
        return TRUE;
  
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Driver initialization FAILED\n", u8DevIdx));
        return FALSE;
   }

   status = MxLWare_HYDRA_API_CfgDevOverwriteDefault(u8DevIdx);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Device Overwrite Default FAILED\n", u8DevIdx));
        return FALSE;
   }


   xtalParamPtr.xtalFreq = MXL_HYDRA_XTAL_24_MHz;
   xtalParamPtr.xtalCap = MXL_XTAL_CAP_VALUE;
   status = MxLWare_HYDRA_API_CfgDevXtal(u8DevIdx, &xtalParamPtr, MXL_FALSE);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Xtal setting FAILED\n", u8DevIdx));
        return FALSE;
   }

   MxLWare_HYDRA_OEM_SleepInMs(50);
   

   status = MxLWare_HYDRA_API_CfgDevFWDownload(u8DevIdx, firmwareSize, MxL542_LIB, NULL);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Firmware download FAILED\n", u8DevIdx));
        return FALSE;
   }
   else
   {
       TUNER_DBG(("[HYDRA][%d] Firmware download Done\n", u8DevIdx));
   }

   status = MxLWare_HYDRA_API_ReqDevVersionInfo(u8DevIdx, &versionInfo);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Req Dev VersionInfo FAILED\n", u8DevIdx));
        return FALSE;
   }
   else
   {
        TUNER_DBG(("[HYDRA][%d] chip=%d MxLWare: %d.%d.%d.%d-RC%d Firmware:%d.%d.%d.%d-RC%d Bootloader:%d.%d.%d.%d-RC%d fwDownloaded=%d\n",
        u8DevIdx,
        versionInfo.chipId,
        versionInfo.mxlWareVer[0],
        versionInfo.mxlWareVer[1],
        versionInfo.mxlWareVer[2],
        versionInfo.mxlWareVer[3],
        versionInfo.mxlWareVer[4],
        versionInfo.firmwareVer[0],
        versionInfo.firmwareVer[1],
        versionInfo.firmwareVer[2],
        versionInfo.firmwareVer[3],
        versionInfo.firmwareVer[4],
        versionInfo.fwPatchVer[0],
        versionInfo.fwPatchVer[1],
        versionInfo.fwPatchVer[2],
        versionInfo.fwPatchVer[3],
        versionInfo.fwPatchVer[4],
        versionInfo.firmwareDownloaded
        ));
   }



   //Select either DiSEqC or FSK - only one interface will be avilable either FSK or DiSEqC
   status = MxLWare_HYDRA_API_CfgDevDiseqcFskOpMode(u8DevIdx, lnbInterface);
   if (status != MXL_SUCCESS)
   {
       TUNER_ERR(("MxLWare_HYDRA_API_CfgDevDiseqcFskOpMode - Error (%d)\n", status));
       return FALSE;
   }
   else
       TUNER_DBG(("Done! - MxLWare_HYDRA_API_CfgDevDiseqcFskOpMode \n"));
   
   if (lnbInterface == MXL_HYDRA_AUX_CTRL_MODE_FSK)
   {
       // Configure FSK operating mode
       status = MxLWare_HYDRA_API_CfgFskOpMode(u8DevIdx, MXL_HYDRA_FSK_CFG_TYPE_39KPBS);
       if (status != MXL_SUCCESS)
       {
           TUNER_ERR(("MxLWare_HYDRA_API_CfgFskOpMode - Error (%d)\n", status));
           return FALSE;
       }
       else
           TUNER_DBG(("Done! - MxLWare_HYDRA_API_CfgFskOpMode \n"));
   }
   else
   {
       // Configure DISEQC operating mode - Only 22 KHz carrier frequency is supported.
       for(j=0; j<MXL_HYDRA_DISEQC_ID_2; j++)
       {
           status = MxLWare_HYDRA_API_CfgDiseqcOpMode(u8DevIdx, 
                    (MXL_HYDRA_DISEQC_ID_E)j,
                    MXL_HYDRA_DISEQC_ENVELOPE_MODE,
                    MXL_HYDRA_DISEQC_1_X,
                    MXL_HYDRA_DISEQC_CARRIER_FREQ_22KHZ);
           
           if (status != MXL_SUCCESS)
           {
               TUNER_ERR(("MxLWare_HYDRA_API_CfgDevDiseqcFskOpMode - Error (%d) DiSeqc_ID (%x)\n", status, j));
               return status;
           }
           else
               TUNER_DBG(("Done! - MxLWare_HYDRA_API_CfgDevDiseqcFskOpMode DiSeqc_ID (%x)\n",j));
           }
   } 


   // Config TS MUX mode - Disable TS MUX feature
   status = MxLWare_HYDRA_API_CfgTSMuxMode(u8DevIdx, tsMuxType);
   if (status != MXL_SUCCESS)
   {
       TUNER_ERR(("MxLWare_HYDRA_API_CfgTSMuxMode - Error (%d)\n", status));
       return FALSE;
   }
   else
       TUNER_DBG(("Done! - MxLWare_HYDRA_API_CfgTSMuxMode \n"));

   // Config TS interface of the device
   #if 0
   mpegInterfaceCfg.enable = MXL_ENABLE;
   mpegInterfaceCfg.lsbOrMsbFirst = MXL_HYDRA_MPEG_SERIAL_LSB_1ST;
   mpegInterfaceCfg.maxMpegClkRate = maxTsInterfaceClkRate; //  supports only (0 - 104 & 139)MHz
   mpegInterfaceCfg.mpegClkPhase = MXL_HYDRA_MPEG_CLK_PHASE_SHIFT_0_DEG;
   mpegInterfaceCfg.mpegClkPol = MXL_HYDRA_MPEG_CLK_NEGATIVE;
   mpegInterfaceCfg.mpegClkType = MXL_HYDRA_MPEG_CLK_CONTINUOUS;
   mpegInterfaceCfg.mpegErrorIndication = MXL_HYDRA_MPEG_ERR_REPLACE_SYNC;
   mpegInterfaceCfg.mpegMode = tsInterfaceMode;
   mpegInterfaceCfg.mpegSyncPol  = MXL_HYDRA_MPEG_ACTIVE_HIGH;
   mpegInterfaceCfg.mpegSyncPulseWidth  = MXL_HYDRA_MPEG_SYNC_WIDTH_BYTE;
   mpegInterfaceCfg.mpegValidPol  = MXL_HYDRA_MPEG_ACTIVE_HIGH;
#endif
   memset(&mpegInterfaceCfg, 0, sizeof(mpegInterfaceCfg));    
   mpegInterfaceCfg.enable = MXL_ENABLE;    
   mpegInterfaceCfg.mpegClkType = MXL_HYDRA_MPEG_CLK_CONTINUOUS;    
   mpegInterfaceCfg.mpegClkPol = MXL_HYDRA_MPEG_CLK_POSITIVE;    
   mpegInterfaceCfg.maxMpegClkRate = maxTsInterfaceClkRate;    
   mpegInterfaceCfg.mpegClkPhase = MXL_HYDRA_MPEG_CLK_PHASE_SHIFT_90_DEG;    
   mpegInterfaceCfg.lsbOrMsbFirst = MXL_HYDRA_MPEG_SERIAL_MSB_1ST;    
   mpegInterfaceCfg.mpegSyncPulseWidth = MXL_HYDRA_MPEG_SYNC_WIDTH_BYTE;    
   mpegInterfaceCfg.mpegValidPol = MXL_HYDRA_MPEG_ACTIVE_HIGH;    
   mpegInterfaceCfg.mpegSyncPol = MXL_HYDRA_MPEG_ACTIVE_HIGH;    
   mpegInterfaceCfg.mpegMode = MXL_HYDRA_MPEG_MODE_SERIAL_3_WIRE;    
   mpegInterfaceCfg.mpegErrorIndication = MXL_HYDRA_MPEG_ERR_INDICATION_DISABLED;


    for (j = 0; j < MXL_HYDRA_DEMOD_ID_4; j++)
    {
     // Configure TS interface
         status = MxLWare_HYDRA_API_CfgMpegOutParams(u8DevIdx, (MXL_HYDRA_DEMOD_ID_E)j, &mpegInterfaceCfg);
         if (status != MXL_SUCCESS)
         {
            TUNER_ERR(("MxLWare_HYDRA_API_CfgMpegOutParams for (%d) - Error (%d)\n", j, status));
            return FALSE;
         }
            else
                TUNER_DBG(("Done! - MxLWare_HYDRA_API_CfgMpegOutParams (%d) \n", j));  
    }

   MxLWare_HYDRA_API_CfgTSOutDriveStrength(u8DevIdx, MXL_HYDRA_TS_DRIVE_STRENGTH_3x);  

   status = MxLWare_HYDRA_API_CfgTunerEnable(u8DevIdx, MXL_HYDRA_TUNER_ID_0, MXL_ENABLE);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Tuner Enable FAILED\n", u8DevIdx));
        return FALSE;
   }

   status = MxLWare_HYDRA_API_CfgTunerEnable(u8DevIdx, MXL_HYDRA_TUNER_ID_1, MXL_ENABLE);
   if (status != MXL_SUCCESS)
   {
        TUNER_ERR(("[HYDRA][%d] Tuner Enable FAILED\n", u8DevIdx));
        return FALSE;
   }

   MxLWare_HYDRA_OEM_SleepInMs(600);

   MXL542_init[u8DevIdx] = TRUE;  
   return TRUE;
}

static MS_BOOL _mxl542_Channel_tune(MS_U8 u8TunerIndex, 
                                           MS_U32 u32CenterFreq, 
                                           MS_U32 u32SymbolRate_KHz, 
                                           MXL_HYDRA_BCAST_STD_E estd)
{
    MXL_STATUS_E status;
    MXL_HYDRA_TUNER_ID_E eTUNER_ID;
    MXL_HYDRA_DEMOD_ID_E eDEMOD_ID;
    UINT8 u8devID;
    MXL_HYDRA_TUNE_PARAMS_T chanTuneParamsPtr;
    MXL_HYDRA_DEMOD_SCRAMBLE_INFO_T demodScramblingCode;
    
    memset(&chanTuneParamsPtr, 0, sizeof(MXL_HYDRA_TUNE_PARAMS_T));

    eDEMOD_ID = (MXL_HYDRA_DEMOD_ID_E)(u8TunerIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8TunerIndex/MAX_CHANNEL_COUNT);
    eTUNER_ID = (MXL_HYDRA_TUNER_ID_E)(InitParam[u8devID].u8Cable_Index);
    
    
    chanTuneParamsPtr.standardMask = estd;
    chanTuneParamsPtr.frequencyInHz = (UINT32)u32CenterFreq*1000000;
    chanTuneParamsPtr.freqSearchRangeKHz = 5000;//5000; //5Mhz
    chanTuneParamsPtr.symbolRateKSps= u32SymbolRate_KHz;
    chanTuneParamsPtr.spectrumInfo = MXL_HYDRA_SPECTRUM_AUTO;

          // parameters specific to standard
      switch(chanTuneParamsPtr.standardMask)
      {
        case MXL_HYDRA_DVBS2:
          // DVB-S2 specific parameters
          chanTuneParamsPtr.params.paramsS2.fec        = MXL_HYDRA_FEC_AUTO;
          chanTuneParamsPtr.params.paramsS2.modulation = MXL_HYDRA_MOD_AUTO;
          chanTuneParamsPtr.params.paramsS2.pilots     = MXL_HYDRA_PILOTS_AUTO;
          chanTuneParamsPtr.params.paramsS2.rollOff    = MXL_HYDRA_ROLLOFF_AUTO;

          // For DVB-S2 standard program default scrambling code
          demodScramblingCode.scrambleCode = 0x01;
          demodScramblingCode.scrambleSequence[0] = 0x00;
          demodScramblingCode.scrambleSequence[1] = 0x00;
          demodScramblingCode.scrambleSequence[2] = 0x00;
          demodScramblingCode.scrambleSequence[3] = 0x00;
          demodScramblingCode.scrambleSequence[4] = 0x00;
          demodScramblingCode.scrambleSequence[5] = 0x00;
          demodScramblingCode.scrambleSequence[6] = 0x00;
          demodScramblingCode.scrambleSequence[7] = 0x00;
          demodScramblingCode.scrambleSequence[8] = 0x00;
          demodScramblingCode.scrambleSequence[9] = 0x00;
          demodScramblingCode.scrambleSequence[10] = 0x00;
          demodScramblingCode.scrambleSequence[11] = 0x00;

          // program default scramble code
          status = MxLWare_HYDRA_API_CfgDemodScrambleCode(u8devID,eDEMOD_ID,&demodScramblingCode);

          break;

        case MXL_HYDRA_DVBS:
        default:    
          // DVB-S specific parameters
          chanTuneParamsPtr.params.paramsS.fec        = MXL_HYDRA_FEC_AUTO;
          chanTuneParamsPtr.params.paramsS.modulation = MXL_HYDRA_MOD_AUTO;
          chanTuneParamsPtr.params.paramsS.rollOff    = MXL_HYDRA_ROLLOFF_AUTO;
          break;
      }
    
    status = MxLWare_HYDRA_API_CfgDemodChanTune(u8devID, eTUNER_ID, eDEMOD_ID, &chanTuneParamsPtr);
    if(MXL_SUCCESS == status)
    {
        //TUNER_DBG(("MXL542Tuner_SetTuner %d OK\n", (int)eTUNER_ID));
        return TRUE;
    }
    else
    {
        TUNER_ERR(("MXL542Tuner_SetTuner %d Fail\n", (int)eTUNER_ID));
        return FALSE;
    }

}

//-------------------------------------------------------------------------------------------------
//  Global Variables
//---------------------------------------------------------------------------------------------

//======================
MS_BOOL MXL542_Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID = u8TunerIndex/MAX_CHANNEL_COUNT;
    MS_BOOL bret;
    
    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8devID].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

    InitParam[u8devID].u8Cable_Index = 0;
    if(MXL542_init[u8devID] == TRUE)
    {
        //_blind_scan_test();
        return TRUE;
    }

    bret = _mxl542_init(u8devID, 
                         MXL_HYDRA_AUX_CTRL_MODE_DISEQC, 
                         MXL_HYDRA_TS_MUX_DISABLE,
                         MXL_HYDRA_MPEG_MODE_SERIAL_3_WIRE, 
                         64);

    return bret;
}

MS_BOOL MXL542_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID = u8DemodIndex/MAX_CHANNEL_COUNT;
    MS_BOOL bret;

    if(MXL542_init[u8devID] == TRUE)
        return TRUE;

    bret = _mxl542_init(u8devID, 
                          MXL_HYDRA_AUX_CTRL_MODE_DISEQC, 
                          MXL_HYDRA_TS_MUX_DISABLE,
                          MXL_HYDRA_MPEG_MODE_SERIAL_3_WIRE, 
                          64);

     return bret;
}


MS_BOOL MXL542_Tuner_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz)
{
   MS_BOOL bret = TRUE;

  // bret = _mxl542_Channel_tune(u8TunerIndex,u32CenterFreq,u32SymbolRate_Hz, MXL_HYDRA_DVBS2);
   return bret;
}

#define MXL542_CHIP_ID 0x560
MS_BOOL MXL542_TunerCheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MXL_STATUS_E status = MXL_FAILURE;
    UINT32 regData = 0;
    MS_U8 u8devID;
    oem_data_t oemPtr;
    
    u8devID = u8TunerIndex; // here u8TunerIndex means device index
    
    if(!MXL542_detect[u8devID])
    {
        oemPtr.i2cAddress = MXL542_SLAVE_ID;
        oemPtr.drvIndex  = 0;
        oemPtr.Mutex = -1;
        MxL_HYDRA_OEM_DataPtr[0] = (void *)&oemPtr;
        status = MxLWare_HYDRA_ReadRegister(u8devID, 0x80030000, &regData);
        regData = regData&0x0FFF;

        if((MXL_SUCCESS == status) && (MXL542_CHIP_ID == regData))
        {
            TUNER_DBG(("== Get MXL542 ID !!\n"));
            MXL542_detect[u8devID] = TRUE;
            if(NULL != pu32channel_cnt)
               *(pu32channel_cnt) += MAX_CHANNEL_COUNT; 
            return TRUE;
        }
        else
        {
            TUNER_ERR(("== Get MXL542 ID FAIL !!\n"));
            return FALSE;
        }
    }
    else
    {
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += MAX_CHANNEL_COUNT; 
    }
    TUNER_DBG(("== MXL542 have been detected \n"));
    return TRUE;

}

MS_BOOL MXL542_DemodCheckExist(MS_U8 u8TunerIndex, MS_U8* pu8SlaveID)
{
  MXL_STATUS_E status = MXL_FAILURE;
  UINT32 regData = 0;
  MS_U8 u8devID, i;
  oem_data_t oemPtr;
  MS_BOOL bret = TRUE;
    
  u8devID = u8TunerIndex; // here u8TunerIndex means device index

  if(!MXL542_detect[u8devID])
  {
      for(i=0;i<4;i++)
      {
          oemPtr.i2cAddress = MXL542_SLAVE_ID + i;
          oemPtr.drvIndex  = 0;
          oemPtr.Mutex = -1;
          MxL_HYDRA_OEM_DataPtr[u8TunerIndex] = (void *)&oemPtr;
          printf("@@ try slave ID: 0x%02x\n", oemPtr.i2cAddress);
          status = MxLWare_HYDRA_ReadRegister(u8devID, 0x80030000, &regData);
          regData = regData&0x0FFF;
          if((MXL_SUCCESS == status) && (MXL542_CHIP_ID == regData))
          {
              TUNER_DBG(("== Get MXL542 ID !!\n"));
              MXL542_detect[u8devID] = TRUE;
              bret = TRUE;
              *pu8SlaveID = oemPtr.i2cAddress;
              break;
          }
          else
          {
              TUNER_ERR(("== Get MXL542 ID FAIL !!\n"));
              bret = FALSE;
          }
      }

  }
  else
  {
      DMD_DBG(("== MXL542 have been detected \n"));
      *pu8SlaveID = MXL542_SLAVE_ID;
  }
  return bret;
}


MS_BOOL MXL542_Demod_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_U8 u8data;
    
    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_SET_DISEQC_TX:
            u8data = *(MS_U8*)data;
            InitParam[u8DemodIndex].u8Cable_Index = u8data; //Only called by Dish driver, u8DemodIndex = u8DishIndex = Device ID
            
            break;
        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
            
    }
    return TRUE;
}

MS_BOOL MXL542_Tuner_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_U8 u8devID = u8TunerIndex/MAX_CHANNEL_COUNT;
    MS_BOOL bret=TRUE;
    MS_U8 u8data;
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_SET_CABLE_INDEX:
            u8data = *(MS_U8*)data;
            if(!u8data)
               bret = FALSE;
            else if(u8data> 2)
                bret = FALSE;
            else
                InitParam[u8devID].u8Cable_Index = u8data-1;//Called by API  layer, Index need modified
            
            break;
        case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _mxl542_Decide_LNB_LO(SAT_PARAM);
            break;    
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return bret;
}


MS_BOOL MXL542_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MXL_STATUS_E status;
    MXL_HYDRA_DEMOD_ID_E eDMD_ID;
    MXL_HYDRA_DEMOD_LOCK_T stLockStatus;
    UINT8 u8devID;
#if SHOW_PARAM_AFTER_LOCK
    MXL_HYDRA_TUNE_PARAMS_T chanTuneParams;
    MXL_HYDRA_DEMOD_STATUS_T berStats;
    MXL_HYDRA_DEMOD_SIG_OFFSET_INFO_T demodSigOffsetInfo;
#endif
    eDMD_ID = (MXL_HYDRA_DEMOD_ID_E)(u8DemodIndex % MAX_CHANNEL_COUNT);
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    


    status = MxLWare_HYDRA_API_ReqDemodLockStatus(u8devID,eDMD_ID,&stLockStatus);
    if (stLockStatus.fecLock == MXL_TRUE)
    {
      *peLockStatus = E_DEMOD_LOCK;
#if SHOW_PARAM_AFTER_LOCK
      MxLWare_HYDRA_API_ReqDemodChanParams(u8devID, eDMD_ID, &chanTuneParams);
      MxLWare_HYDRA_API_ReqDemodErrorCounters(u8devID, eDMD_ID, &berStats);
      
      MxLWare_HYDRA_API_ReqDemodSignalOffsetInfo(u8devID, eDMD_ID, &demodSigOffsetInfo);
      
      printf("CFO = %d\n", demodSigOffsetInfo.carrierOffsetInHz);
      printf("SRO = %d\n", demodSigOffsetInfo.symbolOffsetInSymbol);

      if(chanTuneParams.standardMask == MXL_HYDRA_DVBS2)
      {
        DMD_DBG(("Demod%d - MXL_HYDRA_DVBS2 LOCKED {%s, Freq = %d, SymbolRate = %d, spectrum = %s, Modulation = %s, CR = %s, pilots = %s, rolloff = %s} \n",
            eDMD_ID,
            standard[chanTuneParams.standardMask],
            chanTuneParams.frequencyInHz,
            chanTuneParams.symbolRateKSps,
            spectrum[chanTuneParams.spectrumInfo],
            modulation[chanTuneParams.params.paramsS2.modulation],
            fec[chanTuneParams.params.paramsS2.fec],
            pilots[chanTuneParams.params.paramsS2.pilots],
            rolloff[chanTuneParams.params.paramsS2.rollOff]));
      }
      else if (chanTuneParams.standardMask == MXL_HYDRA_DVBS)
      {
        DMD_DBG(("Deomd%d - MXL_HYDRA_DVBS LOCKED {%s, Freq = %d, SymbolRate = %d, spectrum = %s, Modulation = %s, CR = %s, rolloff = %s} \n",
            eDMD_ID,
            standard[chanTuneParams.standardMask],
            chanTuneParams.frequencyInHz,
            chanTuneParams.symbolRateKSps,
            spectrum[chanTuneParams.spectrumInfo],
            modulation[chanTuneParams.params.paramsS.modulation],
            fec[chanTuneParams.params.paramsS.fec],
            rolloff[chanTuneParams.params.paramsS.rollOff]));

        DMD_DBG(("BER = %d %d\n", berStats.u.demodStatus_DVBS.berCount, berStats.u.demodStatus_DVBS.berWindow)); 
        DMD_DBG(("BER = %lf\n", ((double)berStats.u.demodStatus_DVBS.berCount/(double)berStats.u.demodStatus_DVBS.berWindow))); 
        DMD_DBG(("BER_Iter1 = %d %d\n", berStats.u.demodStatus_DVBS.berCount_Iter1, berStats.u.demodStatus_DVBS.berWindow_Iter1)); 

      }
      else
      {
        DMD_DBG(("Deomd%d - OTHER! LOCKED {%s, Freq = %d, SymbolRate = %d, spectrum = %s, Modulation = QPSK, CR = %s} \n",
            eDMD_ID,
            standard[chanTuneParams.standardMask],
            chanTuneParams.frequencyInHz,
            chanTuneParams.symbolRateKSps,
            spectrum[chanTuneParams.spectrumInfo],
            fec[chanTuneParams.params.paramsDSS.fec]));
      }
#endif
    }
    else
    {
      *peLockStatus = E_DEMOD_CHECKING;
      //DMD_DBG(("Deomd%d - UNLOCKED!  \n", eDMD_ID));
    }
        
   if(status == MXL_SUCCESS)
   {
       return TRUE;
   }
   else
   {
       DMD_ERR(("MxLWare_HRCLS_API_ReqDemodAllLockStatus NG\n"));
       return FALSE;
   }
}

MS_BOOL MXL542_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
     return TRUE;
}

MS_BOOL MXL542_Demod_GetBER(MS_U8 u8DemodIndex, float *pfBER)
{

     return TRUE;

}

MS_BOOL MXL542_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
       return TRUE;
}


MS_BOOL MXL542_Demod_GetParam(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{     

    return TRUE;
}

MS_BOOL MXL542_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
   MXL_HYDRA_BCAST_STD_E eSTD = MXL_HYDRA_DVBS;
   BLIND_SCAN_CHAN_INFO* pstChInfo = NULL;

   if(DVBS2 == u32BroadCastType)
        eSTD = MXL_HYDRA_DVBS2;

   if(Scaned_Cnt) // while doing blind scan
    {
       pstChInfo = pstCh_Info + Scaned_Cnt - 1;
       printf("1. %d   %d\n", (pstChInfo->u32SymbolRate/1000), (pParam->SatParam.u32SymbolRate/1000));
       printf("2. %d   %d\n", (pstChInfo->stTone_Info.frequency_inHz/1000000), (pParam->u32Frequency));
       printf("3. %d   %d\n", pstChInfo->eBCAST_STD, eSTD);
       if(((pstChInfo->u32SymbolRate/1000) == (pParam->SatParam.u32SymbolRate/1000)) &&\
          ((pstChInfo->stTone_Info.frequency_inHz/1000000) == (pParam->u32Frequency)) &&\
          (u8DemodIndex == 0) && (pstChInfo->eBCAST_STD == eSTD))       
       {
           //printf("[Blind Scan]Do Not need set demod\n");
           return TRUE;
       }

    }
  _mxl542_Channel_tune(u8DemodIndex,pParam->u32Frequency,(pParam->SatParam.u32SymbolRate/1000), eSTD);

   return TRUE;
}

MS_BOOL MxL542_Demod_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex, MS_BOOL b22kOn)
{
    MXL_STATUS_E status;
    UINT8 u8devID;
    MXL_HYDRA_DISEQC_ID_E eDiSeqc_ID;

    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    eDiSeqc_ID = (MXL_HYDRA_DISEQC_ID_E)InitParam[u8devID].u8Cable_Index;
 
    status = MxLWare_HYDRA_API_CfgDiseqcContinuousToneCtrl(u8devID, eDiSeqc_ID, (MXL_BOOL_E)b22kOn);
    return (MS_BOOL)status;
}

MS_BOOL MxL542_Demod_DiSEqC_SendCmd(MS_U8 u8DemodIndex, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MXL_STATUS_E status;
    UINT8 u8devID;
    MXL_HYDRA_DISEQC_TX_MSG_T stDiSeqcCfg;

    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    stDiSeqcCfg.diseqcId = (MXL_HYDRA_DISEQC_ID_E)InitParam[u8devID].u8Cable_Index;
    stDiSeqcCfg.nbyte = (UINT32)u8CmdSize;
    stDiSeqcCfg.toneBurst = MXL_HYDRA_DISEQC_TONE_NONE;
    memcpy(&stDiSeqcCfg.bufMsg[0], pCmd, u8CmdSize);

    status = MxLWare_HYDRA_API_CfgDiseqcWrite(u8devID, &stDiSeqcCfg);
    return (MS_BOOL)status;

}

MS_BOOL MxL542_Demod_DiSEqC_SetTone(MS_U8 u8DemodIndex, MS_BOOL bTone1)
{
    MXL_STATUS_E status;
    UINT8 u8devID;
    MXL_HYDRA_DISEQC_TX_MSG_T stDiSeqcCfg;

    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);
    stDiSeqcCfg.diseqcId = (MXL_HYDRA_DISEQC_ID_E)InitParam[u8devID].u8Cable_Index;
    stDiSeqcCfg.nbyte = 0;

    if(bTone1)
        stDiSeqcCfg.toneBurst = MXL_HYDRA_DISEQC_TONE_SB;
    else
        stDiSeqcCfg.toneBurst = MXL_HYDRA_DISEQC_TONE_SA;

    status = MxLWare_HYDRA_API_CfgDiseqcWrite(u8devID, &stDiSeqcCfg);
    return (MS_BOOL)status;
}

MS_BOOL MxL542_Demod_BlindScan_Start(MS_U8 u8DemodIndex, MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL bRet=TRUE;
    UINT8 u8devID, i;
    TONE_DETECTION_INFO* pstToneInfo = NULL;
    BLIND_SCAN_CHAN_INFO* pstChInfo = NULL;
    SINT16 s16TONE_THRESHOLD = -75;//fix me

    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);


    if(pstTone_Info == NULL)
        pstTone_Info = malloc(sizeof(TONE_DETECTION_INFO)*numOfTones);
    
    if(pstTone_Info == NULL)
    {
        return FALSE;
    }

    MxLWare_HYDRA_API_CfgDemodDisable(u8devID, MXL_HYDRA_DEMOD_ID_0);
    DMD_DBG((" Scan Range: %d ~ %d (Mhz)\n", (int)u16StartFreq, (int)u16EndFreq));
    bRet = _mxl542_ReqToneFrequency(u8devID,
                                       (MXL_HYDRA_TUNER_ID_E)InitParam[u8devID].u8Cable_Index,
                                       MXL_HYDRA_DEMOD_ID_0,
                                       u16StartFreq*1000,
                                       u16EndFreq*1000,
                                       s16TONE_THRESHOLD,
                                       &numOfTones,
                                       pstTone_Info);
    if(bRet)
    {
        Scaned_Cnt = 0;
        if(pstCh_Info == NULL)
            pstCh_Info = malloc(sizeof(BLIND_SCAN_CHAN_INFO)*numOfTones);
        
        if(pstCh_Info == NULL)
        {
            free(pstTone_Info);
            pstTone_Info = NULL;
            return FALSE;
        }

        //DMD_DBG((" Scan Range: %d ~ %d (Mhz)\n", (int)u16StartFreq, (int)u16EndFreq));
        DMD_DBG(("==================== Tone List ======================\n"));
   
        for(i=0;i<numOfTones;i++)
        {
            pstToneInfo = pstTone_Info + i;
            pstChInfo = pstCh_Info + i;
            pstChInfo->stTone_Info.frequency_inHz= pstToneInfo->frequency_inHz;
            pstChInfo->stTone_Info.signalLevel = pstToneInfo->signalLevel;
            pstChInfo->stTone_Info.ToneBW_N_inHz = pstToneInfo->ToneBW_N_inHz;
            pstChInfo->stTone_Info.ToneBW_W_inHz= pstToneInfo->ToneBW_W_inHz;
            pstChInfo->u32SymbolRate = 0;
            DMD_DBG(("%02x frequency(KHz) %d Level (dBm)%d SR(KHz) %d ~ %d\n", i,\
                     (int)pstToneInfo->frequency_inHz/1000,\
                     (int)pstToneInfo->signalLevel,\
                     (int)pstToneInfo->ToneBW_N_inHz/1000,\
                     (int)pstToneInfo->ToneBW_W_inHz/1000));
        }
        DMD_DBG(("======================================================\n"));
    }
    else
    {
       free(pstTone_Info);
       pstTone_Info = NULL;
       bRet = FALSE;
    }
    
    return bRet;
}

MS_BOOL MxL542_Demod_BlindScan_NextFreq(MS_U8 u8DemodIndex, MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet=TRUE, bLock=FALSE;
    UINT8 i,j;
    MS_U32 u32CenterFreq_inHz; 
    MS_U32 u32SymbolRate_Hi_inHz, u32SymbolRate_Low_inHz, u32SymbolRate_inHz;
    MS_U32 u32SymbolRate_up_try_inHz, u32SymbolRate_dwn_try_inHz;
    BLIND_SCAN_CHAN_INFO* pstChInfo = NULL;
    MS_U16 u16SR_Try_Step;
    MS_U32 u32SR_Step = 90000;
    UINT8 allDemodLock=0, u8devID;
    MXL_HYDRA_DEMOD_SIG_OFFSET_INFO_T demodSigOffsetInfo;
    MS_U32 u32systime;
    MXL_HYDRA_DEMOD_ID_E eDMD_ID;
    MXL_HYDRA_BCAST_STD_E eBCAST_STD;
    MS_BOOL bMiddle_Search = TRUE;
    
    u8devID = (UINT8)(u8DemodIndex/MAX_CHANNEL_COUNT);

    if(Scaned_Cnt == numOfTones)
    {
        printf(" === BLind Scan End ===\n");
        *bBlindScanEnd = TRUE;
    }
    else
    {
        *bBlindScanEnd = FALSE;
        pstChInfo = pstCh_Info + Scaned_Cnt;
        u32CenterFreq_inHz = pstChInfo->stTone_Info.frequency_inHz;         
        Scaned_Cnt++;

        u32SymbolRate_Hi_inHz = pstChInfo->stTone_Info.ToneBW_W_inHz;
        u32SymbolRate_Low_inHz = pstChInfo->stTone_Info.ToneBW_N_inHz;


        DMD_DBG(("\n==============================================================\n"));
        DMD_DBG(("Try frequency : %d\n",(int)u32CenterFreq_inHz));
        DMD_DBG(("Symbol Rate : %d ~ %d (Khz)\n", (int)u32SymbolRate_Low_inHz/1000, (int)u32SymbolRate_Hi_inHz/1000));
   
       if((u32SymbolRate_Hi_inHz < 25000000) || (u32SymbolRate_Low_inHz < 5000000))
       {
           pstChInfo->u32SymbolRate = 0;
           printf("Abort try !!!!\n");  
           return bRet;
       }

       if(u32SymbolRate_Hi_inHz > 35000000)
            u32SymbolRate_Hi_inHz = 35000000;

       if(u32SymbolRate_Low_inHz < 15000000)
            u32SymbolRate_Low_inHz = 15000000;
   
       u16SR_Try_Step = (u32SymbolRate_Hi_inHz - u32SymbolRate_Low_inHz)/u32SR_Step; 

       DMD_DBG(("=== try steps: %d\n", (int)u16SR_Try_Step));
       u32systime = MsOS_GetSystemTime();
     
       for(i=0;i<u16SR_Try_Step;i++)
       {
           if(bMiddle_Search)
           {
               u32SymbolRate_up_try_inHz = u32SymbolRate_Low_inHz/2 + u32SymbolRate_Hi_inHz/2  + u32SR_Step*i;
               u32SymbolRate_dwn_try_inHz = u32SymbolRate_Low_inHz/2 + u32SymbolRate_Hi_inHz/2  - u32SR_Step*(i+1);
           }
           else
           {
               u32SymbolRate_up_try_inHz = u32SymbolRate_Low_inHz + u32SR_Step*i;
               u32SymbolRate_dwn_try_inHz = u32SymbolRate_Hi_inHz - u32SR_Step*i;
           }


           DMD_DBG(("SR: %d  %d (Khz)\n", u32SymbolRate_up_try_inHz/1000, u32SymbolRate_dwn_try_inHz/1000));
           //if((u32SymbolRate_up_try_inHz > u32SymbolRate_Hi_inHz) || (u32SymbolRate_dwn_try_inHz < u32SymbolRate_Low_inHz))
           if(((u32SymbolRate_up_try_inHz > u32SymbolRate_dwn_try_inHz) && (!bMiddle_Search)) ||\
               (((u32SymbolRate_up_try_inHz > u32SymbolRate_Hi_inHz) || (u32SymbolRate_dwn_try_inHz < u32SymbolRate_Low_inHz)) && bMiddle_Search))
           {
              DMD_DBG(("=== try end"));
              break;
           }
           
           _mxl542_Channel_tune(0, 
                                   u32CenterFreq_inHz/1000000, 
                                   u32SymbolRate_up_try_inHz/1000, 
                                   MXL_HYDRA_DVBS);
           
           _mxl542_Channel_tune(1, 
                                   u32CenterFreq_inHz/1000000, 
                                   u32SymbolRate_up_try_inHz/1000,  
                                   MXL_HYDRA_DVBS2);

            _mxl542_Channel_tune(2, 
                                   u32CenterFreq_inHz/1000000, 
                                   u32SymbolRate_dwn_try_inHz/1000, 
                                   MXL_HYDRA_DVBS);
           
           _mxl542_Channel_tune(3, 
                                   u32CenterFreq_inHz/1000000, 
                                   u32SymbolRate_dwn_try_inHz/1000,  
                                   MXL_HYDRA_DVBS2);
           
           
           MsOS_DelayTask(300);
           MxLWare_HYDRA_API_ReqAllDemodLockStatus(u8devID, &allDemodLock);
           

           if(allDemodLock != 0)
           {
              bLock = TRUE;
              for(j=0;j<4;j++)
              {
                  if(allDemodLock & (1<<j))
                  {
                     eDMD_ID = (MXL_HYDRA_DEMOD_ID_E)j;
                     switch(j)
                     {
                         case 0:
                             u32SymbolRate_inHz = u32SymbolRate_up_try_inHz;
                             eBCAST_STD = MXL_HYDRA_DVBS;
                             break;
                         case 1:
                             u32SymbolRate_inHz = u32SymbolRate_up_try_inHz;
                             eBCAST_STD = MXL_HYDRA_DVBS2;
                             break;
                         case 2:
                             u32SymbolRate_inHz = u32SymbolRate_dwn_try_inHz;
                             eBCAST_STD = MXL_HYDRA_DVBS;
                             break;
                         case 3:
                             u32SymbolRate_inHz = u32SymbolRate_dwn_try_inHz;
                             eBCAST_STD = MXL_HYDRA_DVBS2;
                             break;
                         default:
                             break;
                     }
                     break;
                  }
              }

              //fine tune CFO/SRO
                 MxLWare_HYDRA_API_ReqDemodSignalOffsetInfo(u8devID, eDMD_ID, &demodSigOffsetInfo);
                 u32CenterFreq_inHz = (UINT32)((SINT32)u32CenterFreq_inHz + demodSigOffsetInfo.carrierOffsetInHz);
                 u32SymbolRate_inHz = (UINT32)((SINT32)u32SymbolRate_inHz + demodSigOffsetInfo.symbolOffsetInSymbol); 

                 eDMD_ID = MXL_HYDRA_DEMOD_ID_0; //Must use demod 0 to avoid TS mux switch
                 u32systime = MsOS_GetSystemTime();
                 do
                 {
                     _mxl542_Channel_tune(eDMD_ID, 
                                         u32CenterFreq_inHz/1000000, 
                                         u32SymbolRate_inHz/1000, 
                                         eBCAST_STD);


                     MsOS_DelayTask(30); 
                     MxLWare_HYDRA_API_ReqDemodSignalOffsetInfo(u8devID, eDMD_ID, &demodSigOffsetInfo);
                     DMD_DBG(("fine tune SR:%d (Khz)  SR: SRO = %d\n",  u32SymbolRate_inHz/1000, demodSigOffsetInfo.symbolOffsetInSymbol));
                     if((demodSigOffsetInfo.symbolOffsetInSymbol < 1000) && (demodSigOffsetInfo.symbolOffsetInSymbol > (-1000)))
                        break;
                     else
                        u32SymbolRate_inHz = (UINT32)((SINT32)u32SymbolRate_inHz + demodSigOffsetInfo.symbolOffsetInSymbol);

                }while((MsOS_GetSystemTime() - u32systime) < 200);


                 break;
           }

       }

       if(bLock)
       {
           Find_Cnt++;
           pstChInfo->u32SymbolRate = u32SymbolRate_inHz; 
           pstChInfo->eBCAST_STD = eBCAST_STD;
           pstChInfo->stTone_Info.frequency_inHz= u32CenterFreq_inHz;  
           if(eBCAST_STD == MXL_HYDRA_DVBS2)
           {
               DMD_DBG(("Frequency %d, Symbol rate %d Lock DVBS2\n", (int)u32CenterFreq_inHz ,(int)u32SymbolRate_inHz));
           }
           else
           {
               DMD_DBG(("Frequency %d, Symbol rate %d Lock DVBS\n", (int)u32CenterFreq_inHz ,(int)u32SymbolRate_inHz));
           }
       }
       else
       {
           pstChInfo->u32SymbolRate = 0;
           DMD_DBG(("Unlock !!!!\n"));          
       }

    }

    return bRet;
}

MS_BOOL MxL542_Demod_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex, MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    *u8FindNum = Find_Cnt;
    *u8Progress = 100;
    return TRUE;
}

MS_BOOL MxL542_Demod_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_U8  i, TP_Cnt=0;
    BLIND_SCAN_CHAN_INFO* pstChInfo = NULL;
    BLIND_SCAN_CHAN_INFO* pstTPInfo = NULL;

    if(Scaned_Cnt < u16ReadStart)
        return FALSE;

    pstTPInfo = malloc(numOfTones*sizeof(BLIND_SCAN_CHAN_INFO));
  
    for(i=0;i<numOfTones;i++)
    {
       pstChInfo = pstCh_Info + i;
       if(pstChInfo->u32SymbolRate)
       {
          memcpy(pstTPInfo + TP_Cnt, pstChInfo, sizeof(BLIND_SCAN_CHAN_INFO));
          TP_Cnt++;  
       }
    }


    *u16TPNum = TP_Cnt - u16ReadStart;
    //printf("TP_Cnt = %d, u16TPNum = %d\n", TP_Cnt, *u16TPNum);
    for(i= u16ReadStart; i< TP_Cnt; i++)
    {
       pTable[i-u16ReadStart].u32Frequency = (MS_U32)(pstTPInfo + i)->stTone_Info.frequency_inHz/1000000;
       pTable[i-u16ReadStart].SatParam.u32SymbolRate = (pstTPInfo + i)->u32SymbolRate/1000;
       pTable[i-u16ReadStart].SatParam.bIsDVBS2 = (MS_BOOL)((pstTPInfo + i)->eBCAST_STD - MXL_HYDRA_DVBS);

       DMD_DBG(("Is DVBS2 ? %x\n", pTable[i-u16ReadStart].SatParam.bIsDVBS2));
       DMD_DBG(("Freq:%ld SymbolRate:%ld\n", (long int)pTable[i-u16ReadStart].u32Frequency, (long int)pTable[i-u16ReadStart].SatParam.u32SymbolRate));
    }

    //if(Scaned_Cnt == numOfTones)
        // _mxl542_clear_blind_scan_param(u8DemodIndex);

    return TRUE;
}

MS_BOOL MxL542_Demod_BlindScan_End(MS_U8 u8DemodIndex)
{
    _mxl542_clear_blind_scan_param(u8DemodIndex);
    return TRUE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL542) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL542",          // demod system name
    .data               = TUNER_MXL542,            // private data value
    .Init               = MXL542_Tuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MXL542_Tuner_SetTuner,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .CheckExist         = MXL542_TunerCheckExist,
    .Extension_Function = MXL542_Tuner_Extension_Function
};

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MXL542) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MXL542",
     .data                         = DEMOD_MXL542,
     .init                         = MXL542_Demod_Init,
     .GetLock                      = MXL542_Demod_GetLock,
     .GetSNR                       = MXL542_Demod_GetSNR,
     .GetBER                       = MXL542_Demod_GetBER,
     .GetPWR                       = MXL542_Demod_GetPWR,
     .GetQuality                   = MDrv_Demod_null_GetSignalQuality,
     .GetParam                     = MXL542_Demod_GetParam,
     .Restart                      = MXL542_Demod_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .CheckExist                   = MXL542_DemodCheckExist,
     .Extension_Function           = MXL542_Demod_Extension_Function,
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
     .BlindScanStart               = MxL542_Demod_BlindScan_Start,
     .BlindScanNextFreq            = MxL542_Demod_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MxL542_Demod_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MxL542_Demod_BlindScan_End,
     .BlindScanGetChannel          = MxL542_Demod_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MxL542_Demod_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MxL542_Demod_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MxL542_Demod_DiSEqC_SendCmd
 #endif
};

#endif
