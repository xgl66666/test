/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: SYS MNGR TRIGGER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/sys_mngr_trigger_cfg.h $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: sys mngr trigger configuration header
 *
 *****************************************************************************/

#ifndef _SYS_MNGR_TRIGGER_CFG_H_
#define _SYS_MNGR_TRIGGER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef enum
{
    eTRIG_CFG_STATE_DISABLE,
    eTRIG_CFG_STATE_ENABLE
} tTRIG_CFG_State;

typedef enum
{
    eTRIG_CFG_METHOD_SCANNING,
    eTRIG_CFG_METHOD_TS_LIST
} tTRIG_CFG_Method;

typedef enum
{
	eTRIG_CFG_DIRECTION_EAST,
	eTRIG_CFG_DIRECTION_WEST
} tTRIG_CFG_Direction;

typedef enum
{
	eTRIG_CFG_BANDTYPE_KU,
	eTRIG_CFG_BANDTYPE_C
} tTRIG_CFG_BandType;

typedef enum
{
    eTRIG_CFG_MOD_BPSK = 0,
    eTRIG_CFG_MOD_QPSK,
    eTRIG_CFG_MOD_8PSK,
    eTRIG_CFG_MOD_QAM16,
    eTRIG_CFG_MOD_QAM32,
    eTRIG_CFG_MOD_QAM64,
    eTRIG_CFG_MOD_QAM128,
    eTRIG_CFG_MOD_QAM256,
    eTRIG_CFG_MOD_AUTO
} tTRIG_CFG_Modulation;

typedef enum
{
	eTRIG_CFG_SPECTRUM_AUTO = 0,
	eTRIG_CFG_SPECTRUM_NORMAL,
	eTRIG_CFG_SPECTRUM_INVERTED
} tTRIG_CFG_Spectrum;

typedef enum
{
	eTRIG_CFG_POLAR_HORIZONTAL,
	eTRIG_CFG_POLAR_VERTICAL
} tTRIG_CFG_LNB_Polarization;

typedef enum
{
    eTRIG_CFG_FEC_NONE,
    eTRIG_CFG_FEC_1_2,
    eTRIG_CFG_FEC_2_3,
    eTRIG_CFG_FEC_3_4,
    eTRIG_CFG_FEC_4_5,     /* only used for DVB-T2 */
    eTRIG_CFG_FEC_5_6,
    eTRIG_CFG_FEC_6_7,
    eTRIG_CFG_FEC_7_8,
    eTRIG_CFG_FEC_8_9,
    eTRIG_CFG_FEC_3_5,     /* only used for DVB-T2 */
    eTRIG_CFG_FEC_AUTO
}tTRIG_CFG_Convolution;

typedef enum
{
    eTRIG_CFG_MODE_2K,
    eTRIG_CFG_MODE_8K,
    eTRIG_CFG_MODE_4K,       /* only used for DVB-T2 */
    eTRIG_CFG_MODE_1K,       /* only used for DVB-T2 */
    eTRIG_CFG_MODE_16K,      /* only used for DVB-T2 */
    eTRIG_CFG_MODE_32K,      /* only used for DVB-T2 */
    eTRIG_CFG_MODE_AUTO
}tTRIG_CFG_TerMode;

typedef enum
{
    eTRIG_CFG_GUARD_1_32,
    eTRIG_CFG_GUARD_1_16,
    eTRIG_CFG_GUARD_1_8,
    eTRIG_CFG_GUARD_1_4,
    eTRIG_CFG_GUARD_1_128,    /* only used for DVB-T2 */
    eTRIG_CFG_GUARD_19_128,   /* only used for DVB-T2 */
    eTRIG_CFG_GUARD_19_256,   /* only used for DVB-T2 */
    eTRIG_CFG_GUARD_AUTO
}tTRIG_CFG_TerGuard;

typedef enum
{
    eTRIG_CFG_HIER_NONE,
    eTRIG_CFG_HIER_1,
    eTRIG_CFG_HIER_2,
    eTRIG_CFG_HIER_4,
    eTRIG_CFG_HIER_AUTO
}tTRIG_CFG_TerHierarchy;

typedef enum
{
    eTRIG_CFG_PRIORITY_STREAM_HIGH,
    eTRIG_CFG_PRIORITY_STREAM_LOW
}tTRIG_CFG_TerPrioStream;


typedef enum
{
    eTRIG_CFG_OFFSET_NONE,
    eTRIG_CFG_OFFSET_1_6_MORE, 	    /* + 1/6 Mhz */
    eTRIG_CFG_OFFSET_1_6_MINUS,	    /* - 1/6 Mhz */
    eTRIG_CFG_OFFSET_2_6_MORE,	    /* + 2/6 Mhz */
    eTRIG_CFG_OFFSET_2_6_MINUS, 	/* - 2/6 Mhz */
    eTRIG_CFG_OFFSET_3_6_MORE, 	    /* + 3/6 Mhz */
    eTRIG_CFG_OFFSET_3_6_MINUS,  	/* - 3/6 Mhz */
    eTRIG_CFG_OFFSET_AUTO
}tTRIG_CFG_TerFreqOff;


/* MEDIUM_SAT */
#if defined (MEDIUM_SAT)
#define kSYS_MNGR_TRIG_CFG_MEDIUM_SAT
typedef struct
{
    uint32_t                    frequency;      /* frequency in kHz */
    uint32_t                    symbolRateBW;   /* symbol rate in bauds */
    tTRIG_CFG_LNB_Polarization  polarization;
    tTRIG_CFG_Modulation        modulation;
    tTRIG_CFG_Convolution       convolution;
} tTRIG_CFG_Download_Parameter;

/* MEDIUM_CAB */
#elif defined (MEDIUM_CAB)
#define kSYS_MNGR_TRIG_CFG_MEDIUM_CAB
typedef struct
{
    uint32_t                frequency;      /* frequency in Hz */
    uint32_t                symbolRateBW;   /* symbol rate in bauds */
    tTRIG_CFG_Modulation    modulation;
} tTRIG_CFG_Download_Parameter;

/* MEDIUM_TER */
#elif defined (MEDIUM_TER)
#define kSYS_MNGR_TRIG_CFG_MEDIUM_TER
typedef struct
{
    uint32_t                frequency;      /* frequency in Hz */
    uint32_t                symbolRateBW;   /* bandwidth in Hz */
    tTRIG_CFG_Modulation    modulation;
    tTRIG_CFG_Spectrum      specInv;
    tTRIG_CFG_Convolution   convolutionHP;
    tTRIG_CFG_Convolution   convolutionLP;
    tTRIG_CFG_TerGuard      guard;
    tTRIG_CFG_TerMode       mode;
    tTRIG_CFG_TerPrioStream priority;
    tTRIG_CFG_TerHierarchy  hierarchy;
    tTRIG_CFG_TerFreqOff    offset;
    int32_t                 plpID;
} tTRIG_CFG_Download_Parameter;
#endif


typedef  enum
{
	eSYS_CFG_DOWNLOAD_UPDATE_MODE_ASK,
	eSYS_CFG_DOWNLOAD_UPDATE_MODE_ALLWAYS,
	eSYS_CFG_DOWNLOAD_UPDATE_MODE_NEVER
}tSYS_CFG_DownloadUpdateMode;

/********************************************************/
/*              Macros                                  */
/********************************************************/
#define mTRIG_CFG_CONVERT_MODULATION(x)   ((x==eDMD_MOD_BPSK) ? eTRIG_CFG_MOD_BPSK : \
                                          ((x==eDMD_MOD_QPSK) ? eTRIG_CFG_MOD_QPSK : \
                                          ((x==eDMD_MOD_8PSK) ? eTRIG_CFG_MOD_8PSK : \
                                          ((x==eDMD_MOD_QAM16) ? eTRIG_CFG_MOD_QAM16 : \
                                          ((x==eDMD_MOD_QAM32) ? eTRIG_CFG_MOD_QAM32 : \
                                          ((x==eDMD_MOD_QAM64) ? eTRIG_CFG_MOD_QAM64 : \
                                          ((x==eDMD_MOD_QAM128) ? eTRIG_CFG_MOD_QAM128 : \
                                          ((x==eDMD_MOD_QAM256) ? eTRIG_CFG_MOD_QAM256 : eTRIG_CFG_MOD_AUTO))))))))

#define mTRIG_CFG_CONVERT_SPECTRUM(x)     ((x==eDMD_SPECTRUM_NORMAL) ? eTRIG_CFG_SPECTRUM_NORMAL : \
                                          ((x==eDMD_SPECTRUM_INVERTED) ? eTRIG_CFG_SPECTRUM_INVERTED : eTRIG_CFG_SPECTRUM_AUTO))

#define mTRIG_CFG_CONVERT_MODE(x)         ((x==eDMD_MODE_2K) ? eTRIG_CFG_MODE_2K : \
                                          ((x==eDMD_MODE_8K) ? eTRIG_CFG_MODE_8K : \
                                          ((x==eDMD_MODE_4K) ? eTRIG_CFG_MODE_4K : \
                                          ((x==eDMD_MODE_1K) ? eTRIG_CFG_MODE_1K : \
                                          ((x==eDMD_MODE_16K) ? eTRIG_CFG_MODE_16K : \
                                          ((x==eDMD_MODE_32K) ? eTRIG_CFG_MODE_32K : eTRIG_CFG_MODE_AUTO))))))

#define mTRIG_CFG_CONVERT_PRIORITY(x)     ((x==eDMD_PRIORITY_STREAM_HIGH) ? eTRIG_CFG_PRIORITY_STREAM_HIGH : eTRIG_CFG_PRIORITY_STREAM_LOW)

#define mTRIG_CFG_CONVERT_HIERARCHY(x)    ((x==eDMD_HIER_NONE) ? eTRIG_CFG_HIER_NONE : \
                                          ((x==eDMD_HIER_1) ? eTRIG_CFG_HIER_1 : \
                                          ((x==eDMD_HIER_2) ? eTRIG_CFG_HIER_2 : \
                                          ((x==eDMD_HIER_4) ? eTRIG_CFG_HIER_4 : eTRIG_CFG_HIER_AUTO))))

#define mTRIG_CFG_CONVERT_OFFSET(x)       ((x==eDMD_OFFSET_NONE) ? eTRIG_CFG_OFFSET_NONE : \
                                          ((x==eDMD_OFFSET_1_6_MORE) ? eTRIG_CFG_OFFSET_1_6_MORE : \
                                          ((x==eDMD_OFFSET_1_6_MINUS) ? eTRIG_CFG_OFFSET_1_6_MINUS : \
                                          ((x==eDMD_OFFSET_2_6_MORE) ? eTRIG_CFG_OFFSET_2_6_MORE : \
                                          ((x==eDMD_OFFSET_2_6_MINUS) ? eTRIG_CFG_OFFSET_2_6_MINUS : \
                                          ((x==eDMD_OFFSET_3_6_MORE) ? eTRIG_CFG_OFFSET_3_6_MORE : \
                                          ((x==eDMD_OFFSET_3_6_MINUS) ? eTRIG_CFG_OFFSET_3_6_MINUS : eTRIG_CFG_OFFSET_AUTO)))))))

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/

/*----------------------------------------------------------------------------------*/
/* AUTOMATIC TRIGGER: During the standby, the STB looks for a new software update.  */
/* This search can be done either by scanning or using a pre-defined TS list. See   */
/* function TRIG_CFG_GetAutomaticTriggerMethod().                                   */
/* This functionality can be disabled using TRIG_CFG_GetAutomaticTriggerState().    */
/* The timings of the search (delay after which the first check is done and the     */
/* period) are defined by TRIG_CFG_GetAutomaticTriggerFirstCheckTime() and          */
/* TRIG_CFG_GetAutomaticTriggerPeriod().                                            */
/*----------------------------------------------------------------------------------*/


/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerState
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the automatic trigger state (enable or disable)
 *
 * Returns:
 *      eTRIG_CFG_STATE_DISABLE
 *      eTRIG_CFG_STATE_ENABLE
 *
 *===========================================================================*/
tTRIG_CFG_State  TRIG_CFG_GetAutomaticTriggerState(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerMethod
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the automatic trigger method (TS list or scanning)
 *
 * Returns:
 *      eTRIG_CFG_METHOD_TS_LIST if the automatic trigger is done using a TS list
 *      eTRIG_CFG_METHOD_SCANNING if it is done using a scanning
 *
 *===========================================================================*/
tTRIG_CFG_Method TRIG_CFG_GetAutomaticTriggerMethod(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerCheckTimeMode
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the automatic trigger mode
 *
 * Returns:
 *      true: absolute time => the trigger starts at a given hour (e.g. 03:00)
 *      false: relative time => the trigger starts after a given duration in standby
 *
 *===========================================================================*/
bool TRIG_CFG_GetAutomaticTriggerCheckTimeMode(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerFirstCheckTime
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the time (in seconds) when the first check will start
 *      Depending on the value return by TRIG_CFG_GetAutomaticTriggerCheckTimeMode
 *      The time returned by this function either corresponds to a number of
 *      seconds elapsed in standby (relative mode), or a number of seconds 
 *      corresponding to the hour (e.g. 03:00 => 3*60*60)
 *
 * Returns:
 *      delay (in seconds)
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetAutomaticTriggerFirstCheckTime(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerPeriod
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the period of the check
 *
 * Returns:
 *      if 0, the automatic trigger is not periodic
 *      otherwise, define the period of the check
 *      Note: if the trigger is configured to start at a given hour (checkTimeMode=true),
 *      the period is either 0 (not periodic) either 24h
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetAutomaticTriggerPeriod(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerNumberOfTS
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the number of TS in the TS list used by the automatic trigger
 *      (if TS list mode is activated instead of scanning mode)
 *
 * Returns:
 *      number of TS defined in the TS list
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetAutomaticTriggerNumberOfTS(void);

/*===========================================================================
 *
 * TRIG_CFG_SetAutomaticTriggerTsListCurrentIndex
 *
 * Parameters:
 *      index
 *
 * Description:
 *      If the automatic trigger is configured in TS list mode, this function
 *      is used to set the current index in the TS list. This index will be used 
 *      by TRIG_CFG_GetAutomaticTriggerTsListParameters().
 *
 * Returns:
 *      true if parameter is correct, false otherwise
 *
 *===========================================================================*/
bool TRIG_CFG_SetAutomaticTriggerTsListCurrentIndex(uint32_t index);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerTsListParameters
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the parameters of the TS selected by a prior call to the function
 *      TRIG_CFG_SetAutomaticTriggerTsListCurrentIndex(). This function is called
 *      if the automatic trigger is configured in TS list mode 
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
void TRIG_CFG_GetAutomaticTriggerTsListParameters(tTRIG_CFG_Download_Parameter* pDownloadParameters);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerOrbitalPosition
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the orbital position of the TS checked by the automatic trigger (SAT only)
 *
 * Returns:
 *      orbital position in tenth of degrees (e.g. 192 for 19.2°)
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetAutomaticTriggerOrbitalPosition(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerDirection
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the direction (east/west) of the TS checked by the automatic trigger (SAT only)
 *
 * Returns:
 *      tTRIG_CFG_Direction
 *
 *===========================================================================*/
tTRIG_CFG_Direction TRIG_CFG_GetAutomaticTriggerDirection(void);

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerBandType
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the band type (KU/C) of the TS checked by the automatic trigger (SAT only)
 *
 * Returns:
 *      tTRIG_CFG_BandType
 *
 *===========================================================================*/
tTRIG_CFG_BandType TRIG_CFG_GetAutomaticTriggerBandType(void);






/*-------------------------------------------------------------------------------*/
/* BOOT TRIGGER: during the boot, the STB checks if a new software download is   */
/* available on the TS defined by TRIG_CFG_GetBootTriggerDownloadParameters()    */
/* This check will be done only if the factory_OAD_flag has been set to 1.       */
/* This functionality can be disabled using TRIG_CFG_GetBootTriggerState().      */
/* the boot trigger is typically used for factory OAD.                           */
/*-------------------------------------------------------------------------------*/


/*===========================================================================
 *
 * TRIG_CFG_GetBootTriggerState
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the boot trigger state (enable or disable)
 *
 * Returns:
 *      eTRIG_CFG_STATE_DISABLE
 *      eTRIG_CFG_STATE_ENABLE
 *
 *===========================================================================*/
tTRIG_CFG_State TRIG_CFG_GetBootTriggerState(void);

/*===========================================================================
 *
 * TRIG_CFG_GetBootTriggerDownloadParameters
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the parameters of the TS checked by the boot trigger
 *
 * Returns:
 *      Nothing
 *
 *===========================================================================*/
void TRIG_CFG_GetBootTriggerDownloadParameters(tTRIG_CFG_Download_Parameter* pDownloadParameters);
                                                    


/*===========================================================================
 *
 * TRIG_CFG_GetDefaultDownloadUpdateMode
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the default download update mode
 *
 * Returns:
 *      eSYS_CFG_DOWNLOAD_UPDATE_MODE_NEVER
 *      eSYS_CFG_DOWNLOAD_UPDATE_MODE_ASK
 *      eSYS_CFG_DOWNLOAD_UPDATE_MODE_ALLWAYS
 *
 *===========================================================================*/
tSYS_CFG_DownloadUpdateMode TRIG_CFG_GetDefaultDownloadUpdateMode(void);


/*===========================================================================
 *
 * TRIG_CFG_Is_Trigger_Nb_Refusal_Supported
 *
 * Parameters:
 *      None
 *
 * Description: Allow to enable or disable the user to refuse a limited number 
 * 	of the same version 
 *      
 *
 * Returns: TRUE/FALSE
 *
 *===========================================================================*/
bool TRIG_CFG_Is_Trigger_Nb_Refusal_Supported(void);


#ifdef __cplusplus
}
#endif

#endif /* _SYS_MNGR_TRIGGER_CFG_H_ */
