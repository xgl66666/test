/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: SYS MNGR TRIGGER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/sys_mngr_trigger_cfg.c $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: sys mngr trigger configuration
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "tkel.h"
#include "tbox.h"
#include "sys_mngr_trigger_cfg.h"

#ifdef MEDIUM_TER
#include "terinstall_cfg.h"
//#define TER_TRIGGER_USE_COUNTRY_TSLIST
#define TER_TRIGGER_USE_BOOTFILE_INFO
#endif

#ifdef TER_TRIGGER_USE_BOOTFILE_INFO
#include "sys_dmd.h"
#include "sys_mngr.h"
#include "sys_mngr_bootfile.h"
#endif

/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Macros                                  */
/********************************************************/

/*
 * 
#define __TRIGGER_TRACE_CONFIG__ 1
 *
 */


/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/

/*--------------------------------------------------------------------*/
/* NOTE: the medium (SAT, CAB, TER) is selected in trigger_cfg.h file */
/*--------------------------------------------------------------------*/

#if defined (kSYS_MNGR_TRIG_CFG_MEDIUM_SAT)
/* MEDIUM_SAT */
static tTRIG_CFG_Download_Parameter TRIG_CFG_DownloadParameters[] = {{12603000, 22000000, eTRIG_CFG_POLAR_HORIZONTAL, eTRIG_CFG_MOD_QPSK, eTRIG_CFG_FEC_AUTO},
                                                                     {10820000, 27500000, eTRIG_CFG_POLAR_HORIZONTAL, eTRIG_CFG_MOD_QPSK, eTRIG_CFG_FEC_AUTO},
                                                                     {12475000, 27500000, eTRIG_CFG_POLAR_HORIZONTAL, eTRIG_CFG_MOD_QPSK, eTRIG_CFG_FEC_AUTO},
                                                                     {0, 0, 0, 0, 0} };

#define kTRIG_CFG_ORBITAL_POSITION  192  /* 19.2° */
#define kTRIG_CFG_DIRECTION         eTRIG_CFG_DIRECTION_EAST
#define kTRIG_CFG_BANDTYPE          eTRIG_CFG_BANDTYPE_KU

#elif defined (kSYS_MNGR_TRIG_CFG_MEDIUM_CAB)
/* MEDIUM_CAB */
static tTRIG_CFG_Download_Parameter TRIG_CFG_DownloadParameters[] = {{330000000, 6900000, eTRIG_CFG_MOD_QAM64},
                                                                     {306000000, 6875000, eTRIG_CFG_MOD_QAM64},
                                                                     {0, 0, 0} };

#elif defined (kSYS_MNGR_TRIG_CFG_MEDIUM_TER)
#ifdef TER_TRIGGER_USE_COUNTRY_TSLIST

/* List of TS could defined here to choose only some of terinstall TS depending on country */

#elif defined(TER_TRIGGER_USE_BOOTFILE_INFO)

#else

/* MEDIUM_TER */
static tTRIG_CFG_Download_Parameter TRIG_CFG_DownloadParameters[] = { 
                                                                     {  /* Imroz 1 on chanel 21 */
                                                                     	  474000000,
                                                                        8000000,
                                                                        eTRIG_CFG_MOD_QAM64,
                                                                        eTRIG_CFG_SPECTRUM_NORMAL,
                                                                        eTRIG_CFG_FEC_AUTO,
                                                                        eTRIG_CFG_FEC_AUTO,
                                                                        eTRIG_CFG_GUARD_AUTO,
                                                                        eTRIG_CFG_MODE_AUTO,
                                                                        eTRIG_CFG_PRIORITY_STREAM_HIGH,
                                                                        eTRIG_CFG_HIER_AUTO,
                                                                        eTRIG_CFG_OFFSET_AUTO,
                                                                        -1},
                                                                     {  490000000,
                                                                        8000000,
                                                                        eTRIG_CFG_MOD_QAM64,
                                                                        eTRIG_CFG_SPECTRUM_NORMAL,
                                                                        eTRIG_CFG_FEC_AUTO,
                                                                        eTRIG_CFG_FEC_AUTO,
                                                                        eTRIG_CFG_GUARD_AUTO,
                                                                        eTRIG_CFG_MODE_AUTO,
                                                                        eTRIG_CFG_PRIORITY_STREAM_HIGH,
                                                                        eTRIG_CFG_HIER_AUTO,
                                                                        eTRIG_CFG_OFFSET_AUTO,
                                                                        -1},
                                                                     { 706000000,
                                                                        8000000,
                                                                        eTRIG_CFG_MOD_QAM64,
                                                                        eTRIG_CFG_SPECTRUM_NORMAL,
                                                                        eTRIG_CFG_FEC_AUTO,
                                                                        eTRIG_CFG_FEC_AUTO,
                                                                        eTRIG_CFG_GUARD_AUTO,
                                                                        eTRIG_CFG_MODE_AUTO,
                                                                        eTRIG_CFG_PRIORITY_STREAM_HIGH,
                                                                        eTRIG_CFG_HIER_AUTO,
                                                                        eTRIG_CFG_OFFSET_AUTO,
                                                                        -1},
                                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
#endif
#endif

#ifdef TER_TRIGGER_USE_COUNTRY_TSLIST
#define kTRIG_CFG_NB_OF_TS_TO_SCAN TERINSTALL_CFG_GetNumberOfTS()
#elif defined(TER_TRIGGER_USE_BOOTFILE_INFO)
#define kTRIG_CFG_NB_OF_TS_TO_SCAN 1
#else
#define kTRIG_CFG_NB_OF_TS_TO_SCAN  sizeof(TRIG_CFG_DownloadParameters)/sizeof(TRIG_CFG_DownloadParameters[0]) -1
#endif
                                                                      
static uint32_t TRIG_CFG_TsListCurrentIndex = 0;
static uint32_t TRIG_CFG_NbCheckDone = 0;


/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


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
tTRIG_CFG_State TRIG_CFG_GetAutomaticTriggerState(void)
{
   tTRIG_CFG_State state = eTRIG_CFG_STATE_ENABLE;
#ifdef __TRIGGER_TRACE_CONFIG__
   static int _num_get_auto_trigger_state = 0;

   if ((_num_get_auto_trigger_state % 100) == 0)
   {
      printf("TRIG_CFG_GetAutomaticTriggerState: %s\n",
             (state==eTRIG_CFG_STATE_ENABLE)?"ENABLE":"DISABLE");
   }
   _num_get_auto_trigger_state++;
#endif

    return state;
}

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
tTRIG_CFG_Method TRIG_CFG_GetAutomaticTriggerMethod(void)
{
   tTRIG_CFG_Method meth = eTRIG_CFG_METHOD_TS_LIST;
   //tTRIG_CFG_Method meth = eTRIG_CFG_METHOD_SCANNING;

#ifdef __TRIGGER_TRACE_CONFIG__
   static int _num_get_auto_trigger_meth = 0;

   if ((_num_get_auto_trigger_meth % 100) == 0)
   {
      printf("TRIG_CFG_GetAutomaticTriggerMethod: %s\n",
             (meth==eTRIG_CFG_METHOD_SCANNING)?"SCANNING":"TS LIST");
   }
   _num_get_auto_trigger_meth++;
#endif

    return meth;
}

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
bool TRIG_CFG_GetAutomaticTriggerCheckTimeMode(void)
{
   bool is_absolute = FALSE;

#ifdef __TRIGGER_TRACE_CONFIG__
   static int _num_get_check_time_mode = 0;

   if ((_num_get_check_time_mode % 100) == 0)
   {
      printf("TRIG_CFG_GetAutomaticTriggerCheckTimeMode: Mode %s\n",
             (is_absolute)?"ABSOLUTE":"RELATIVE");
   }
   _num_get_check_time_mode++;
#endif

   return is_absolute;
}

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
uint32_t TRIG_CFG_GetAutomaticTriggerFirstCheckTime(void)
{
   uint32_t delay = 10*60;   /* the automatic trigger will start 2h10 after standby */

#ifdef __TRIGGER_TRACE_CONFIG__
   static int _num_get_auto_trigger_first_check_time = 0;

   if ((_num_get_auto_trigger_first_check_time % 100) == 0)
   {
      printf("TRIG_CFG_GetAutomaticTriggerFirstCheckTime: %u sec\n",
             delay);
   }
   _num_get_auto_trigger_first_check_time++;
#endif

   return delay;
}

/*===========================================================================
 *
 * TRIG_CFG_GetAutomaticTriggerPeriod
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the period of the check (in seconds)
 *
 * Returns:
 *      if 0, the automatic trigger is not periodic
 *      otherwise, define the period of the check
 *      Note: if the trigger is configured to start at a given hour (checkTimeMode=true),
 *      the period is either 0 (not periodic) or 24h
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetAutomaticTriggerPeriod(void)
{
   uint32_t period = 60*60; /* Every hour */

#ifdef __TRIGGER_TRACE_CONFIG__
   static int _num_get_auto_trigger_period = 0;

   if ((_num_get_auto_trigger_period % 100) == 0)
   {
      printf("TRIG_CFG_GetAutomaticTriggerPeriod: %u sec\n",
             period);
   }
   _num_get_auto_trigger_period++;
#endif

   return period;
}

/*===========================================================================
 *
 * TRIG_CFG_SetCheckDone
 *
 * Parameters:
 *      None
 *
 * Description:
 *      This function is called each time a trigger is performed during standby.
 *      This is used to count the number of times the trigger has been done so far
 *      and thus change the delay between the checks according to this count
 *      (e.g. perform the first check after 5 min, then after 2 hours, then every 24 hours) 
 *
 * Returns:
 *      void
 *
 *===========================================================================*/
void TRIG_CFG_SetCheckDone(void)
{
   TRIG_CFG_NbCheckDone++;
}

/*===========================================================================
 *
 * TRIG_CFG_GetCheckDone
 *
 * Parameters:
 *      None
 *
 * Description:
 *      This function can be called to retrieve the number of triggers that
 *      have been performed during the standby state so far.
 *
 * Returns:
 *      number of triggers done
 *
 *===========================================================================*/
uint32_t TRIG_CFG_GetCheckDone(void)
{
   return TRIG_CFG_NbCheckDone;
}

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
uint32_t TRIG_CFG_GetAutomaticTriggerNumberOfTS(void)
{
    return kTRIG_CFG_NB_OF_TS_TO_SCAN;
}

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
bool TRIG_CFG_SetAutomaticTriggerTsListCurrentIndex(uint32_t index)
{
    if (index >= kTRIG_CFG_NB_OF_TS_TO_SCAN)
    {
        TRIG_CFG_TsListCurrentIndex = 0;
        return false;
    }
    else
    {
        TRIG_CFG_TsListCurrentIndex = index;
        return true;
    }
}

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
void TRIG_CFG_GetAutomaticTriggerTsListParameters(tTRIG_CFG_Download_Parameter* pDownloadParameters)
{
#ifdef TER_TRIGGER_USE_COUNTRY_TSLIST
	tTERINSTALL_CFG_SignalMode 	signalMode;
	tTERINSTALL_CFG_SpecInv		specInv;

   printf("TRIG_CFG_GetAutomaticTriggerTsListParameters: Return List TS from TERINSTALL_CFG\n");
	if (pDownloadParameters!=NULL)
	{
	    pDownloadParameters->frequency = TERINSTALL_CFG_GetTsListFrequency(TRIG_CFG_TsListCurrentIndex);      /* frequency in Hz */
	    pDownloadParameters->symbolRateBW = TERINSTALL_CFG_GetTsListBandwidth(TRIG_CFG_TsListCurrentIndex);   /* bandwidth in Hz */
	    pDownloadParameters->modulation = eTRIG_CFG_MOD_QAM64;

		specInv = TERINSTALL_CFG_GetSpectralInversion();
		if (specInv == eTERINSTALL_CFG_SPECTRUM_AUTO)
		    pDownloadParameters->specInv = eTRIG_CFG_SPECTRUM_AUTO;
		else if(specInv == eTERINSTALL_CFG_SPECTRUM_NORMAL)
			pDownloadParameters->specInv = eTRIG_CFG_SPECTRUM_NORMAL;
		else
			pDownloadParameters->specInv = eTRIG_CFG_SPECTRUM_INVERTED;

	    pDownloadParameters->convolutionHP = eTRIG_CFG_FEC_AUTO;
	    pDownloadParameters->convolutionLP = eTRIG_CFG_FEC_AUTO;
	    pDownloadParameters->guard = eTRIG_CFG_GUARD_AUTO;

		signalMode = TERINSTALL_CFG_GetSignalMode();
		if (signalMode == eTERINSTALL_2K)
	    	pDownloadParameters->mode = eTRIG_CFG_MODE_2K;
		else
			pDownloadParameters->mode = eTRIG_CFG_MODE_8K;

	    pDownloadParameters->priority = eTRIG_CFG_PRIORITY_STREAM_HIGH;
	    pDownloadParameters->hierarchy = eTRIG_CFG_HIER_AUTO;
	    pDownloadParameters->offset = eTRIG_CFG_OFFSET_AUTO;
	}

#elif defined(TER_TRIGGER_USE_BOOTFILE_INFO)
   uint8_t frontEndID;
   tDMD_TunData tunerData;

   if (pDownloadParameters!=NULL)
   {
      SYS_GetBootFileTunerInfo(&frontEndID, &tunerData);

      pDownloadParameters->frequency      = tunerData.TunFrequency;
      pDownloadParameters->symbolRateBW   = tunerData.SymbolRateBw;
      pDownloadParameters->modulation     = mTRIG_CFG_CONVERT_MODULATION(tunerData.Modulation);
      pDownloadParameters->specInv        = mTRIG_CFG_CONVERT_SPECTRUM(tunerData.SpecInv);
      pDownloadParameters->convolutionHP  = eTRIG_CFG_FEC_AUTO;
      pDownloadParameters->convolutionLP  = eTRIG_CFG_FEC_AUTO;
      pDownloadParameters->guard          = eTRIG_CFG_GUARD_AUTO;
      pDownloadParameters->mode           = mTRIG_CFG_CONVERT_MODE(tunerData.Specific.Ter.Mode);
      pDownloadParameters->priority       = mTRIG_CFG_CONVERT_PRIORITY(tunerData.Specific.Ter.Priority);
      pDownloadParameters->hierarchy      = mTRIG_CFG_CONVERT_HIERARCHY(tunerData.Specific.Ter.Hierarchy);
      pDownloadParameters->offset         = mTRIG_CFG_CONVERT_OFFSET(tunerData.Specific.Ter.Offset);
      pDownloadParameters->plpID          = tunerData.Specific.Ter.PlpID;
   }

#else
   printf("TRIG_CFG_GetAutomaticTriggerTsListParameters: Return List TS from local TS list at index %d\n",TRIG_CFG_TsListCurrentIndex);

    *pDownloadParameters = TRIG_CFG_DownloadParameters[TRIG_CFG_TsListCurrentIndex];

#endif
    return;
}

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
#if defined (kSYS_MNGR_TRIG_CFG_MEDIUM_SAT)
uint32_t TRIG_CFG_GetAutomaticTriggerOrbitalPosition(void)
{
    return kTRIG_CFG_ORBITAL_POSITION;
}
#endif

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
#if defined (kSYS_MNGR_TRIG_CFG_MEDIUM_SAT)
tTRIG_CFG_Direction TRIG_CFG_GetAutomaticTriggerDirection(void)
{
    return kTRIG_CFG_DIRECTION;
}
#endif

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
#if defined (kSYS_MNGR_TRIG_CFG_MEDIUM_SAT)
tTRIG_CFG_BandType TRIG_CFG_GetAutomaticTriggerBandType(void)
{
    return kTRIG_CFG_BANDTYPE;
}
#endif





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
tTRIG_CFG_State TRIG_CFG_GetBootTriggerState(void)
{
    /* Available states */
    /* eTRIG_CFG_STATE_DISABLE -> disable */
    /* eTRIG_CFG_STATE_ENABLE -> enable */

    return eTRIG_CFG_STATE_DISABLE;
}

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
void TRIG_CFG_GetBootTriggerDownloadParameters(tTRIG_CFG_Download_Parameter* pDownloadParameters)
{
#ifdef TER_TRIGGER_USE_COUNTRY_TSLIST
	tTERINSTALL_CFG_SignalMode 	signalMode;
	tTERINSTALL_CFG_SpecInv		specInv;

   printf("TRIG_CFG_GetBootTriggerDownloadParameters: Return List TS from TERINSTALL_CFG\n");

	if (pDownloadParameters!=NULL)
	{
	    pDownloadParameters->frequency = TERINSTALL_CFG_GetTsListFrequency(0);      /* frequency in Hz */
	    pDownloadParameters->symbolRateBW = TERINSTALL_CFG_GetTsListBandwidth(0);   /* bandwidth in Hz */
	    pDownloadParameters->modulation = eTRIG_CFG_MOD_QAM64;

		specInv = TERINSTALL_CFG_GetSpectralInversion();
		if (specInv == eTERINSTALL_CFG_SPECTRUM_AUTO)
		    pDownloadParameters->specInv = eTRIG_CFG_SPECTRUM_AUTO;
		else if (specInv == eTERINSTALL_CFG_SPECTRUM_NORMAL)
			pDownloadParameters->specInv = eTRIG_CFG_SPECTRUM_NORMAL;
		else
			pDownloadParameters->specInv = eTRIG_CFG_SPECTRUM_INVERTED;

	    pDownloadParameters->convolutionHP = eTRIG_CFG_FEC_AUTO;
	    pDownloadParameters->convolutionLP = eTRIG_CFG_FEC_AUTO;
	    pDownloadParameters->guard = eTRIG_CFG_GUARD_AUTO;

		signalMode = TERINSTALL_CFG_GetSignalMode();
		if (signalMode == eTERINSTALL_2K)
	    	pDownloadParameters->mode = eTRIG_CFG_MODE_2K;
		else
			pDownloadParameters->mode = eTRIG_CFG_MODE_8K;

	    pDownloadParameters->priority = eTRIG_CFG_PRIORITY_STREAM_HIGH;
	    pDownloadParameters->hierarchy = eTRIG_CFG_HIER_AUTO;
	    pDownloadParameters->offset = eTRIG_CFG_OFFSET_AUTO;
	}
#elif defined(TER_TRIGGER_USE_BOOTFILE_INFO)

#else
    *pDownloadParameters = TRIG_CFG_DownloadParameters[0];
   printf("TRIG_CFG_GetBootTriggerDownloadParameters: Return List TS from local TS list at index 0\n");

#endif
    return;
}


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
tSYS_CFG_DownloadUpdateMode TRIG_CFG_GetDefaultDownloadUpdateMode(void)
{
    return eSYS_CFG_DOWNLOAD_UPDATE_MODE_ASK;
}


/*=======================================================
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
bool TRIG_CFG_Is_Trigger_Nb_Refusal_Supported(void)
{
    return false;

}
