/******************************************************************************
 *                   COPYRIGHT 2004 IWEDIA TECHNOLOGIES                   *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMD
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_dmd/tdal_dmd_priv.h $
 *          $Rev: 414 $
 *          $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DMD private header.
 *
 *****************************************************************************/

#ifndef   _TDAL_DMD_PRIV_H_
#define   _TDAL_DMD_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

#include "MsCommon.h"
#include "apiDigiTuner.h"

/*******************************************************/
/*          Defines                     */
/*******************************************************/

#define   kTDAL_DMD_MAX_FRONT_END              1

/*   Front   End   Task   Management   Ressource   */
#define   kTDAL_DMD_FRONT_END_TASK_NAME              "TDAL_DMD_FE_"
#define   kTDAL_DMD_FRONT_END_TASK_FLAG              0
#define   kTDAL_DMD_FRONT_END_NB_MAX_MSG_IN_MSGQ        20

#define   kTDAL_DMD_FRONT_END_NB_RETRY_FOR_SIGNAL_INFO   10


#define   kTDAL_DMD_FRONT_END_TASK_POLLING_TIMEOUT        500          /*   in   ms   */

#define   kTDAL_DMD_FRONT_END_TASK_RELOCK_MECHANISM_TIMEOUT   2              /*   in   second   */

/*   ST   Timeout   for   Tune   /   Scan   */
#define   kTDAL_DMD_DEFAULT_TIMEOUT   0

#define   kTDAL_DMD_ST_DEFAULT_AGC        STTUNER_AGC_ENABLE
#define   kTDAL_DMD_ST_DEFAULT_BAND        0
#define   kTDAL_DMD_ST_DEFAULT_CHANNEL_BW      STTUNER_CHAN_BW_8M
#define   kTDAL_DMD_ST_DEFAULT_ECHO_POS      0
#define   kTDAL_DMD_ST_DEFAULT_FEC_RATE      STTUNER_FEC_1_2
#define   kTDAL_DMD_ST_DEFAULT_FORCE        STTUNER_FORCE_NONE
#define   kTDAL_DMD_ST_DEFAULT_FREQ_OFF      STTUNER_OFFSET_NONE
#if   defined(BOARD_TER_DTT5188)   ||   defined(idtv7109)   ||   defined(dtt7167)
#define   kTDAL_DMD_ST_DEFAULT_GUARD        STTUNER_GUARD_1_32
#else
#define   kTDAL_DMD_ST_DEFAULT_GUARD        STTUNER_GUARD_1_4
#endif
#define   kTDAL_DMD_ST_DEFAULT_HIERARCHY      STTUNER_HIER_NONE
#define   kTDAL_DMD_ST_DEFAULT_IQ_MODE      STTUNER_IQ_MODE_AUTO
#define   kTDAL_DMD_ST_DEFAULT_J83_MODE      STTUNER_J83_NONE
#define   kTDAL_DMD_ST_DEFAULT_LNB_TONE      STTUNER_LNB_TONE_OFF
#define   kTDAL_DMD_ST_DEFAULT_MODE        STTUNER_MODE_8K
#define   kTDAL_DMD_ST_DEFAULT_MODULATION      STTUNER_MOD_64QAM
#define   kTDAL_DMD_ST_DEFAULT_POLARIZATION   STTUNER_PLR_ALL
#define   kTDAL_DMD_ST_DEFAULT_RESIDUAL_OFF   0
#define   kTDAL_DMD_ST_DEFAULT_SPECTRUM      STTUNER_INVERSION_AUTO
#define   kTDAL_DMD_ST_DEFAULT_SYMBOL_RATE   20000000

#define   kTDAL_DMD_DEFAULT_FREQ_STEP        0

#define   kTDAL_DMD_DISEQC_MSEC_BEFORE_NEXT_CMD   50      /*   in   mesc   */

#define   DMD_STTUNER_LOW_BAND   0
#define   DMD_STTUNER_HIGH_BAND   1

/*   SAT   limit   */
#define   kTDAL_DMD_CFG_FREQ_TUNER_MIN_SAT   950000
#define   kTDAL_DMD_CFG_FREQ_TUNER_MAX_SAT   2150000
#define   kTDAL_DMD_CFG_FS_BW_MIN_SAT      1000000
#define   kTDAL_DMD_CFG_FS_BW_MAX_SAT      45000000

/*   CAB   limit   */
#define   kTDAL_DMD_CFG_FREQ_TUNER_MIN_CAB   50000000
#define   kTDAL_DMD_CFG_FREQ_TUNER_MAX_CAB   860000000
#define   kTDAL_DMD_CFG_FS_BW_MIN_CAB      1000000
#define   kTDAL_DMD_CFG_FS_BW_MAX_CAB      7000000

/*   TER   limit   8   MHZ   */
#define   kTDAL_DMD_CFG_FREQ_TUNER_MIN_TER_8M   450000000
#define   kTDAL_DMD_CFG_FREQ_TUNER_MAX_TER_8M   860000000

/*   TER   limit   7   MHZ   */
#define   kTDAL_DMD_CFG_FREQ_TUNER_MIN_TER_7M   150000000
#define   kTDAL_DMD_CFG_FREQ_TUNER_MAX_TER_7M   230000000

#define   kTDAL_DMD_CFG_FS_BW_MIN_TER      6000000      /*   (Hz)   */
#define   kTDAL_DMD_CFG_FS_BW_MAX_TER      8000000      /*   (Hz)   */


/*   SAT   Band   L   (Tuner   Band)   */
#define   kTDAL_DMD_BAND_L_FREQ_MIN        950000      /*   (MHz)   */
#define   kTDAL_DMD_BAND_L_FREQ_LOW_MAX      1975000      /*   (MHz)   */
#define   kTDAL_DMD_BAND_L_FREQ_HIGH_MIN      1125000      /*   (MHz)   */
#define   kTDAL_DMD_BAND_L_FREQ_MAX        2150000      /*   (MHz)   */

/*   SAT   Dual   Band   KU   */
#define   kTDAL_DMD_BAND_KU_FREQ_MIN        10700000   /*   (MHz)   */
#define   kTDAL_DMD_BAND_KU_FREQ_MID        11725000   /*   (MHz)   */
#define   kTDAL_DMD_BAND_KU_FREQ_MAX        12750000   /*   (MHz)   */

/*   SAT   Mono   Band   C   */
#define   kTDAL_DMD_BAND_C_FREQ_MIN        3600000      /*   (MHz)   */
#define   kTDAL_DMD_BAND_C_FREQ_MAX        4200000      /*   (MHz)   */

#define   kTDAL_DMD_MAX_SB              8
#define   kTDAL_DMD_MAX_BW              5

#define   kTDAL_DMD_BAD_ELEMENT_CONVERSION   (int)0xDEADBEEF
#define   kTDAL_DMD_UNKNOWN_NOTIFY_STATUS      0xDEADBEEF


/*******************************************************/
/*          Macros                         */
/*******************************************************/

#define   mLockAccess(lockKey)   (TDAL_LockMutex(lockKey))
#define   mUnLockAccess(lockKey)   (TDAL_UnlockMutex(lockKey))


#define   DMD_ConvertModulationST2Tdal(x)   TDAL_DMDi_ConvertElement("Modulation",   (int)x,   TRUE)
#define   DMD_ConvertModulationTdal2ST(x)   TDAL_DMDi_ConvertElement("Modulation",   (int)x,   FALSE)

#define   DMD_ConvertSpectrumST2Tdal(x)   TDAL_DMDi_ConvertElement("Spectrum",   (int)x,   TRUE)
#define   DMD_ConvertSpectrumTdal2ST(x)   TDAL_DMDi_ConvertElement("Spectrum",   (int)x,   FALSE)

#define   DMD_ConvertCarrierStatusST2Tdal(x)   TDAL_DMDi_ConvertElement("CarrierStatus",   (int)x,   TRUE)

#define   DMD_ConvertPolarizationST2Tdal(x)   TDAL_DMDi_ConvertElement("Polarization",   (int)x,   TRUE)
#define   DMD_ConvertPolarizationTdal2ST(x)   TDAL_DMDi_ConvertElement("Polarization",   (int)x,   FALSE)

#define   DMD_ConvertFECST2Tdal(x)   TDAL_DMDi_ConvertElement("Convolution",   (int)x,   TRUE)
#define   DMD_ConvertFECTdal2ST(x)   TDAL_DMDi_ConvertElement("Convolution",   (int)x,   FALSE)

#define   DMD_ConvertOffsetTdal2ST(x)   TDAL_DMDi_ConvertElement("Offset",   (int)x,   FALSE)
#define   DMD_ConvertResidualOffsetTdal2ST(x)   TDAL_DMDi_ConvertElement("ResOffset",   (int)x,   FALSE)

#define   DMD_ConvertGuardST2Tdal(x)   TDAL_DMDi_ConvertElement("Guard",   (int)x,   TRUE)
#define   DMD_ConvertGuardTdal2ST(x)   TDAL_DMDi_ConvertElement("Guard",   (int)x,   FALSE)

#define   DMD_ConvertHierarchyST2Tdal(x)   TDAL_DMDi_ConvertElement("Hierarchy",   (int)x,   TRUE)
#define   DMD_ConvertHierarchyTdal2ST(x)   TDAL_DMDi_ConvertElement("Hierarchy",   (int)x,   FALSE)

#define   DMD_ConvertModeST2Tdal(x)   TDAL_DMDi_ConvertElement("Mode",   (int)x,   TRUE)
#define   DMD_ConvertModeTdal2ST(x)   TDAL_DMDi_ConvertElement("Mode",   (int)x,   FALSE)

#define   DMD_ConvertPriorityST2Tdal(x)   TDAL_DMDi_ConvertElement("Priority",   (int)x,   TRUE)
#define   DMD_ConvertPriorityTdal2ST(x)   TDAL_DMDi_ConvertElement("Priority",   (int)x,   FALSE)


/*******************************************************/
/*          Typedefs                     */
/*******************************************************/

typedef   enum
{
   /*   Events   from   API   */
   eTDAL_DMD_FRONT_END_EVENT_TYPE_TUNE,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_UNLOCK,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_SCAN,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_STOP_SCAN,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_END_OF_TASK,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_SOURCE_TRANSITION_ATV,
   eTDAL_DMD_FRONT_END_EVENT_TYPE_SOURCE_TRANSITION_DTV,

   /*   Events   from   ST   Tuner   */
   eTDAL_DMD_FRONT_END_ST_TUNER_EVENT_TYPE_LOCKED,
   eTDAL_DMD_FRONT_END_ST_TUNER_EVENT_TYPE_UNLOCKED,
   eTDAL_DMD_FRONT_END_ST_TUNER_EVENT_TYPE_SCAN_FAILED,
   eTDAL_DMD_FRONT_END_ST_TUNER_EVENT_TYPE_TIMEOUT
}   tTDAL_DMD_FrontEndMsgQEventType;




typedef   enum
{
	eTDAL_DMD_FRONT_END_ACTION_REQUEST_TUNING,
	eTDAL_DMD_FRONT_END_ACTION_TUNNING_IN_PROGRESS,
    eTDAL_DMD_FRONT_END_ACTION_TUNED,
    eTDAL_DMD_FRONT_END_ACTION_NOT_TUNED,
    eTDAL_DMD_FRONT_END_ACTION_SILENT_TUNE,
    eTDAL_DMD_FRONT_END_ACTION_SCANNING_REQUEST,
    eTDAL_DMD_FRONT_END_ACTION_SCANNING_IN_PROGRESS,
    eTDAL_DMD_FRONT_END_ACTION_SCANNING_WAITING_NEXT,
    eTDAL_DMD_FRONT_END_ACTION_SCANNING_NEXT_FREQ,
    eTDAL_DMD_FRONT_END_ACTION_END_OF_TASK,
    eTDAL_DMD_FRONT_END_ACTION_NONE,
}tTDAL_DMD_ActionInProgess;

typedef   struct
{
   tTDAL_DMD_ScanData   stScanData;
   unsigned   int        uiStartFreq;
   unsigned   int        uiEndFreq;
}tTDAL_DMDi_CompleteScanData;



typedef   struct
{
   tTDAL_DMD_FrontEndMsgQEventType   eTypeOfEvent;
   unsigned   int                uiFrequency;
   union
   {
      /*   Associated   with   eTDAL_DMD_FRONT_END_EVENT_TYPE_TUNE   */
      tTDAL_DMD_TunData        stTunData;

      /*   Associated   with   eTDAL_DMD_FRONT_END_EVENT_TYPE_SCAN   */
      tTDAL_DMDi_CompleteScanData   stCompleteScanData;

      /*   No   data   associated   with   follwoing   type   :
        eTDAL_DMD_FRONT_END_EVENT_TYPE_UNLOCK,
        eTDAL_DMD_FRONT_END_EVENT_TYPE_CONT_SCAN,
        eTDAL_DMD_FRONT_END_EVENT_TYPE_STOP_SCAN
        or   ST   events
      */
 }uDataType;
}tTDAL_DMD_FrontEndMsgQStruct;



typedef   struct
{
   tTDAL_DMD_SatInitData      Init;
   tTDAL_DMD_SatOlBand        OlBand;
   tTDAL_DMD_ODU_ToneBurst      ToneBurst;
   tTDAL_DMD_ODU_DiSEqC      DiSEqC;
   tTDAL_DMD_ODU_ToneState      ToneState;
   tTDAL_DMD_ODU_Power        Power;
   uint32_t*              pOther;
}tTDAL_DMD_SatConfData;



typedef   struct
{
   tTDAL_DMD_CabInitData      Init;
   uint32_t*              pOther;
}   tTDAL_DMD_CabConfData;



typedef   struct
{
   tTDAL_DMD_TerInitData      Init;
   uint32_t*              pOther;
}tTDAL_DMD_TerConfData;



typedef   struct
{
   union
   {
      tTDAL_DMD_SatConfigType   eSatType;
      tTDAL_DMD_CabConfigType   eCabType;
      tTDAL_DMD_TerConfigType   eTerType;
 }   uCfgType;

   union
   {
      tTDAL_DMD_SatConfData   stSatData;
      tTDAL_DMD_CabConfData   stCabData;
      tTDAL_DMD_TerConfData   stTerData;
 }   uCfg;
}   tTDAL_DMD_ConfData;


typedef   bool   (*tTDAL_DMD_TuneFunction)   (unsigned   char        ucFeID,
                              tTDAL_DMD_TunData      *psTunData,
                              unsigned   int        uiFrequency);

typedef   bool   (*tTDAL_DMD_ScanFunction)   (unsigned   char      ucFeID,
                              tTDAL_DMD_ScanData   *psScanData,
                              unsigned   int        uiStartFreq,
                              unsigned   int        uiEndFreq);


typedef struct
{
	volatile MS_BOOL TaskFinished;
} tTDAL_DMDi_TaskArgs;

typedef   struct
{
   bool                bFrontEndUsed;

   /*   Os   Ressource   for   each   Front   End   */
   TDAL_mutex_id          pFrontEndAccess;
   //task_t                *pstFrontEndMngtTaskID;
   void                *pFrontEndMngtTaskStack;
   //tdesc_t                *pstFrontEndMngtTaskDesc;
   MS_S32          FrontEndMngtMsgQ;
   TDAL_task_id 			FrontEndTask;
   tTDAL_DMDi_TaskArgs FrontEndTaskArgs;

   tTDAL_DMD_Type          eType;
   tTDAL_DMD_SourceSupport      eSourceSupport;

   tTDAL_DMD_NotifyStatusFct   pNotifyStatusFct;
   tTDAL_DMD_NotifyProgressFct   pNotifyProgressFct;

   tTDAL_DMD_ConfData        stConfigData;

   uint32_t              ulValueAgcMax;
   uint32_t              ulValueQualityMax;
   uint32_t              ulValueBerMax;

   //STTUNER_Handle_t        SThandle;
   //ST_DeviceName_t          STname;

   bool                bConfigInitDone;
   bool                bScanInProgress;

   tTDAL_DMD_TuneFunction      pTuneFunction;
   tTDAL_DMD_ScanFunction      pScanFunction;

#ifdef   MEDIUM_SAT
   bool                bSatConfigOLBandDone;
   unsigned   long          pulSatSymbolRates[kTDAL_DMD_MAX_SB];
   unsigned   char          ucSatNbSymbRates;
#endif

#ifdef   MEDIUM_CAB
   unsigned   long          pulCabSymbolRates[kTDAL_DMD_MAX_SB];
   unsigned   char          ucCabNbSymbRates;
#endif

#ifdef   MEDIUM_TER
   unsigned   long          pulTerBandWidths[kTDAL_DMD_MAX_BW];
   unsigned   char          ucTerNbBandWidths;
#endif

   tTDAL_DMD_ActionInProgess eTdalDmdActionInProgress;
   tTDAL_DMD_FE				 frontEnd;

}tTDAL_DMD_Front_End;



typedef   struct
{
   int                   ST_value;
   int                   tdal_value;
}   tTDAL_DMD_TableElement;

#if   defined(BOARD_TER_DTT5188)
typedef   struct
{
   U8   RPLLDIV;   /*PLLNDIV   register   value*/
   U8   TRLNORMRATELSB;   /*TRL   Normrate   registers   value*/
   U8   TRLNORMRATELO;
   U8   TRLNORMRATEHI;
   U8   INCDEROT1;/*INC   derotator   register   value*/
   U8   INCDEROT2;
}tTDAL_DMD_30MhzReg_t   ;
#endif
/*******************************************************/
/*          Variables   Declarations   (MODULE)      */
/*******************************************************/

/*   The   following   arrays   are   used   to   convert   values   between   TDAL   API   and   ST   */
/*
tTDAL_DMD_TableElement   TDAL_DMDi_ConvolutionTable[11]   =   {
   { STTUNER_FEC_ALL,   eTDAL_DMD_FEC_AUTO },
   { STTUNER_FEC_1_2,   eTDAL_DMD_FEC_1_2 },
   { STTUNER_FEC_2_3,   eTDAL_DMD_FEC_2_3 },
   { STTUNER_FEC_3_4,   eTDAL_DMD_FEC_3_4 },
   { STTUNER_FEC_4_5,   eTDAL_DMD_FEC_4_5 },
   { STTUNER_FEC_5_6,   eTDAL_DMD_FEC_5_6 },
   { STTUNER_FEC_6_7,   eTDAL_DMD_FEC_6_7 },
   { STTUNER_FEC_7_8,   eTDAL_DMD_FEC_7_8 },
   { STTUNER_FEC_8_9,   eTDAL_DMD_FEC_8_9 },
   { STTUNER_FEC_NONE,   eTDAL_DMD_FEC_MAX },
   { STTUNER_FEC_NONE,   eTDAL_DMD_FEC_NONE },
};
*/
tTDAL_DMD_TableElement   TDAL_DMDi_ModulationTable[]   =   {
	{ CAB_QAM16, eTDAL_DMD_MOD_QAM16 },
	{ CAB_QAM32, eTDAL_DMD_MOD_QAM32 },
	{ CAB_QAM64, eTDAL_DMD_MOD_QAM64 },
	{ CAB_QAM128,  eTDAL_DMD_MOD_QAM128 },
	{ CAB_QAM256,  eTDAL_DMD_MOD_QAM256 },
	{ CAB_QAMAUTO, eTDAL_DMD_MOD_AUTO },
};
/*
tTDAL_DMD_TableElement   TDAL_DMDi_OffsetTable[9]   =   {
   { STTUNER_OFFSET_NONE,      eTDAL_DMD_OFFSET_NONE },
   { STTUNER_OFFSET,        eTDAL_DMD_OFFSET_AUTO },
   { STTUNER_OFFSET_POSITIVE,   eTDAL_DMD_OFFSET_1_6_MORE },
   { STTUNER_OFFSET_NEGATIVE,   eTDAL_DMD_OFFSET_1_6_MINUS },
   { STTUNER_OFFSET_POSITIVE,   eTDAL_DMD_OFFSET_2_6_MORE },
   { STTUNER_OFFSET_NEGATIVE,   eTDAL_DMD_OFFSET_2_6_MINUS },
   { STTUNER_OFFSET_POSITIVE,   eTDAL_DMD_OFFSET_3_6_MORE },
   { STTUNER_OFFSET_NEGATIVE,   eTDAL_DMD_OFFSET_3_6_MINUS },
   { STTUNER_OFFSET_NONE,      eTDAL_DMD_OFFSET_MAX },
};

tTDAL_DMD_TableElement   TDAL_DMDi_ResidualOffsetTable[9]   =   {
   { 0,   eTDAL_DMD_OFFSET_NONE },
   { 0,   eTDAL_DMD_OFFSET_AUTO },
   { 166,   eTDAL_DMD_OFFSET_1_6_MORE },
   { -166,   eTDAL_DMD_OFFSET_1_6_MINUS },
   { 333,   eTDAL_DMD_OFFSET_2_6_MORE },
   { -333,   eTDAL_DMD_OFFSET_2_6_MINUS },
   { 500,   eTDAL_DMD_OFFSET_3_6_MORE },
   { -500,   eTDAL_DMD_OFFSET_3_6_MINUS },
   { 0,   eTDAL_DMD_OFFSET_MAX },
};

tTDAL_DMD_TableElement   TDAL_DMDi_GuardTable[6]   =   {
   { STTUNER_GUARD_1_32,        eTDAL_DMD_GUARD_1_32 },
   { STTUNER_GUARD_1_16,        eTDAL_DMD_GUARD_1_16 },
   { STTUNER_GUARD_1_8,        eTDAL_DMD_GUARD_1_8 },
   { STTUNER_GUARD_1_4,        eTDAL_DMD_GUARD_1_4 },
   { kTDAL_DMD_ST_DEFAULT_GUARD,   eTDAL_DMD_GUARD_AUTO },
   { kTDAL_DMD_ST_DEFAULT_GUARD,   eTDAL_DMD_GUARD_MAX },
};


tTDAL_DMD_TableElement   TDAL_DMDi_ModeTable[4]   =   {
   { STTUNER_MODE_2K,          eTDAL_DMD_MODE_2K },
   { STTUNER_MODE_8K,          eTDAL_DMD_MODE_8K },
   { kTDAL_DMD_ST_DEFAULT_MODE,   eTDAL_DMD_MODE_AUTO },
   { kTDAL_DMD_ST_DEFAULT_MODE,   eTDAL_DMD_MODE_MAX },
};

tTDAL_DMD_TableElement   TDAL_DMDi_CarrierStatusTable[7]   =   {
   { STTUNER_STATUS_UNLOCKED,   eTDAL_DMD_SIGNAL_LOST },
   { STTUNER_STATUS_NOT_FOUND,   eTDAL_DMD_LOCK_FAILED },
   { STTUNER_STATUS_SCANNING,   eTDAL_DMD_LOCK_FAILED },
   { STTUNER_STATUS_LOCKED,   eTDAL_DMD_LOCKED },
   { STTUNER_STATUS_LOCKED,   eTDAL_DMD_FOUND },
   { STTUNER_STATUS_NOT_FOUND,   eTDAL_DMD_SIGNAL_LOST },
   { STTUNER_STATUS_NOT_FOUND,   eTDAL_DMD_END_OF_RANGE },
};

tTDAL_DMD_TableElement   TDAL_DMDi_SpectrumTable[3]   =   {
   { STTUNER_INVERSION_NONE,   eTDAL_DMD_SPECTRUM_NORMAL },
   { STTUNER_INVERSION,      eTDAL_DMD_SPECTRUM_INVERTED },
   { STTUNER_INVERSION_AUTO,   eTDAL_DMD_SPECTRUM_AUTO },
};

tTDAL_DMD_TableElement   TDAL_DMDi_PolarizationTable[3]   =   {
   { STTUNER_PLR_HORIZONTAL,   eTDAL_DMD_POLAR_HORIZONTAL },
   { STTUNER_PLR_VERTICAL,      eTDAL_DMD_POLAR_VERTICAL },
   { STTUNER_PLR_ALL,        eTDAL_DMD_POLAR_ALL },
};

#ifdef   MEDIUM_TER
tTDAL_DMD_TableElement   TDAL_DMDi_HierarchyTable[5]   =   {
   { STTUNER_HIER_ALPHA_1,              eTDAL_DMD_HIER_1 },
   { STTUNER_HIER_ALPHA_2,              eTDAL_DMD_HIER_2 },
   { STTUNER_HIER_ALPHA_4,              eTDAL_DMD_HIER_4 },
   { kTDAL_DMD_ST_DEFAULT_HIERARCHY,   eTDAL_DMD_HIER_AUTO },
   { STTUNER_HIER_ALPHA_NONE,          eTDAL_DMD_HIER_NONE },
};
#elif   defined(MEDIUM_SAT)   ||   defined(MEDIUM_CAB)
tTDAL_DMD_TableElement   TDAL_DMDi_HierarchyTable[1]   =   {
   { 0,              0 },
};
#endif
*/

/*******************************************************/
/*          Functions   Declarations              */
/*******************************************************/



#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_DMD_PRIV_H_*/

