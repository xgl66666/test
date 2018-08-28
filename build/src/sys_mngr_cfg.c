/**
* @file    sys_mngr_cfg.c
*
* @brief   Configuration file related to the comedia system manager
*
* @author  SmarDTV Rennes - YP
******************************************************************************
* SmarDTV Copyright 2011
******************************************************************************
* $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/sys_mngr_cfg.c $
* $Rev: 244 $
* $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "sys_mngr_cfg.h"

#include "sys_mngr_trigger_cfg.h"

#include "tdal_common.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
LOCAL int ChannelUpdate_CheckDone = 0;
LOCAL int EitFullCaching_CheckDone = 0;

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
#ifdef EIT_FULL_CACHING_ON
extern bool SYS_MNGR_EIM_CFG_EitCachingActivated(void);
#endif

/** 
 *
 * This function defines if channel update in standby is enabled or not
 *
 *
 * @return ::TRUE if channel update in stand by is enabled.
 * @return ::FALSE if channel update in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_Enabled(void)
{
    /* Available states */
    /* true: enabled */
    /* false: disabled */

    return true;
}

/** 
 *
 *      This function defines time mode used to perform channel update:
 *      - relative time mode : channel update is done after a given number
 *        of time in standby
 *      - absolute time mode : channel update is done at a given hour (e.g.
 *        03:00)
 *
 *
 * @return ::TRUE  : absolute time => channel update starts at a given hour (e.g. 03:00)
 * @return ::FALSE : relative time => channel update starts after a given duration in standby
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckTimeMode(void)
{
    /* Available modes */
    /* true: absolute time => the channel update starts at a given hour (e.g. 03:00) */
    /* false: relative time => the channel update starts after a given duration in standby */
    return false;
}

/** 
 *
 *      Return the time (in seconds) when first check will start
 *      Depending on value returned by SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckTimeMode
 *      The time returned by this function either corresponds to a number of
 *      seconds elapsed in standby (relative mode), or a number of seconds 
 *      corresponding to the hour (e.g. 03:00 => 3*60*60)
 *
 *
 * @return ::Number of seconds
 *
 */
int SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_FirstCheckTime(void)
{
    /* relative => nb seconds since booting in standby */
    /* absolute => nb seconds since midnight */
    /*             e.g. to start channel update at 3 o'clock in the morning, return 3*3600 */
    return(/*2*60*60*/20*60);          /* in seconds */
}

/** 
 *
 *      This function is called each time a channel update is performed during standby.
 *      This is used to count the number of times the channel update has been done so far
 *      and thus change the delay between the checks according to this count
 *      (e.g. perform the first check after 5 min, then after 2 hours, then every 24 hours)
 *
 *
 * @return ::void
 *
 */
void SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckDone(void)
{
   ChannelUpdate_CheckDone++;
}

/** 
 *
 *      This function returns channel update period 
 *      if SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckTimeMode is set in absolute time mode,
 *      period is either 0 (not periodic) or xxh (periodic)
 *
 *
 * @return ::0  : channel update is not periodic
 * @return ::Different than 0 : channel update period
 *
 */
int SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_Period(void)
{
    /* if 0, the action is not periodic */
    /* otherwise, it defines the period of the check (in seconds, min value = 3600 seconds) */

   /* The first channel update is done after 2 hours in standby */
   /* Then, it's done every 24 hours */
    return (24*60*60);
}

/** 
 *
 *      This function returns estimated duration of channel update
 *
 *
 * @return ::Estimated channel update duration in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_EstimatedDuration(void)
{
    return (/*10*60*/15*60); /* seconds */
}


/** 
 *
 * This function defines if trigger in standby is enabled or not
 *
 *
 * @return ::TRUE if trigger in stand by is enabled.
 * @return ::FALSE if trigger in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_Trigger_Enabled(void)
{
    /* Available states */
    /* true: enabled */
    /* false: disabled */

#ifndef TRIG_OFF
   return (TRIG_CFG_GetAutomaticTriggerState() == eTRIG_CFG_STATE_ENABLE);
#else
    return false;
#endif
}

/** 
 *
 *      This function defines time mode used for trigger:
 *      - relative time mode : trigger is done after a given number
 *        of time in standby
 *      - absolute time mode : trigger is done at a given hour (e.g.
 *        03:00)
 *
 *
 * @return ::TRUE  : absolute time => trigger starts at a given hour (e.g. 03:00)
 * @return ::FALSE : relative time => trigger starts after a given duration in standby
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_Trigger_CheckTimeMode(void)
{
    /* Available modes */
    /* true: absolute time => the trigger starts at a given hour (e.g. 03:00) */
    /* false: relative time => the trigger starts after a given duration in standby */

#ifndef TRIG_OFF
    return TRIG_CFG_GetAutomaticTriggerCheckTimeMode();
#else
    return false;
#endif
}

/** 
 *
 *      This function returns the first trigger start time (in seconds)
 *      Depending on value returned by SYS_MNGR_CFG_StandbyCheck_Trigger_CheckTimeMode
 *      The time returned by this function either corresponds to a number of
 *      seconds elapsed in standby (relative mode), or a number of seconds 
 *      corresponding to the hour (e.g. 03:00 => 3*60*60)
 *
 *
 * @return ::Number of seconds
 *
 */
int SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime(void)
{
    /* relative => nb seconds since booting in standby */
    /* absolute => nb seconds since midnight */
    /*             e.g. to start Trigger at 3 o'clock in the morning, return 3*3600 */
#ifndef TRIG_OFF
    return TRIG_CFG_GetAutomaticTriggerFirstCheckTime();
#else
    return 0;
#endif
}

/** 
 *
 *      This function is called each time a trigger is performed during standby.
 *      This is used to count the number of times the trigger has been done so far
 *      and thus change the delay between the checks according to this count
 *      (e.g. perform the first check after 5 min, then after 2 hours, then every 24 hours)
 *
 *
 * @return ::void
 *
 */
void SYS_MNGR_CFG_StandbyCheck_Trigger_CheckDone(void)
{
#ifndef TRIG_OFF
   TRIG_CFG_SetCheckDone();
#endif
}

/** 
 *
 *      This function can be called to retrieve the number of triggers that
 *      have been performed during the standby state so far.
 *
 *
 * @return ::uint32_t
 *
 */
uint32_t SYS_MNGR_CFG_StandbyCheck_Trigger_GetNbCheckDone(void)
{
#ifndef TRIG_OFF
   return TRIG_CFG_GetCheckDone();
#else
   return 0;
#endif
}
/** 
 *
 *      This function returns trigger period time
 *      if SYS_MNGR_CFG_StandbyCheck_Trigger_CheckTimeMode is set in absolute time mode,
 *      period is either 0 (not periodic) or xxh (periodic)
 *
 * @return ::0  : trigger is not periodic
 * @return ::Different than 0 : trigger period
 *
 */
int SYS_MNGR_CFG_StandbyCheck_Trigger_Period(void)
{
    /* if 0, the action is not periodic */
    /* otherwise, it defines the period of the check (in seconds, min value = 3600 seconds) */
#ifndef TRIG_OFF
    return TRIG_CFG_GetAutomaticTriggerPeriod();
#else
    return 0;
#endif
}

/** 
 *
 *      This function returns trigger estimated duration
  *
 * @return ::Estimated trigger duration in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_Trigger_EstimatedDuration(void)
{
    return (15*60); /* seconds */
}



/** 
 *
 * This function defines if EIT full caching in standby is enabled or not
 *
 *
 * @return ::TRUE if EIT full caching in stand by is enabled.
 * @return ::FALSE if EIT full caching in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_EitFullCaching_Enabled(void)
{
    /* Available states */
    /* true: enabled */
    /* false: disabled */

#ifdef EIT_FULL_CACHING_ON
    return SYS_MNGR_EIM_CFG_EitCachingActivated();
#else
    return false;
#endif
}

/** 
 *
 *      This function defines time mode used to perform EIT Full caching:
 *      - relative time mode : EIT full caching is done after a given number
 *        of time in standby
 *      - absolute time mode : EIT full caching is done at a given hour (e.g.
 *        03:00)
 *
 *
 * @return ::TRUE  : absolute time => EIT full caching starts at a given hour (e.g. 03:00)
 * @return ::FALSE : relative time => EIT full caching starts after a given duration in standby
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_EitFullCaching_CheckTimeMode(void)
{
    /* Available modes */
    /* true: absolute time => the EIT full caching starts at a given hour (e.g. 03:00) */
    /* false: relative time => the EIT full caching starts after a given duration in standby */

    return false;
}

/** 
 *
 *      This function returns first EIT Full Caching start time (in seconds)
 *      Depending on value returned by SYS_MNGR_CFG_StandbyCheck_EitFullCaching_CheckTimeMode
 *      The time returned by this function either corresponds to a number of
 *      seconds elapsed in standby (relative mode), or a number of seconds 
 *      corresponding to the hour (e.g. 03:00 => 3*60*60)
 *
 *
 * @return ::Number of seconds
 *
 */
int SYS_MNGR_CFG_StandbyCheck_EitFullCaching_FirstCheckTime(void)
{
    /* relative => nb seconds since booting in standby */
    /* absolute => nb seconds since midnight */
    /*             e.g. to start EIT full caching at 3 o'clock in the morning, return 3*3600 */
    
    return (/*2*60*60*/ 30);
}

/** 
 *
 *      This function is called each time a EIT caching is performed during standby.
 *      This is used to count the number of times the EIT caching has been done so far
 *      and thus change the delay between the checks according to this count
 *      (e.g. perform the first check after 20s, then after 1 hour, then every 24 hours)
 *
 *
 * @return ::void
 *
 */
void SYS_MNGR_CFG_StandbyCheck_EitFullCaching_CheckDone(void)
{
   EitFullCaching_CheckDone++;
}

/** 
 *
 *      This function returns EIT full caching period.
 *      If SYS_MNGR_CFG_StandbyCheck_EitFullCaching_CheckTimeMode is set in absolute time mode,
 *      period is either 0 (not periodic) or xxh (periodic)
 *
 *
 * @return ::0  : EIT full caching is not periodic
 * @return ::Different than 0 : EIT full caching period
 *
 */
int SYS_MNGR_CFG_StandbyCheck_EitFullCaching_Period(void)
{
    /* if 0, the action is not periodic */
    /* otherwise, it defines the period of the check (in seconds, min value = 3600 seconds) */

   /* The first EIT caching is done after 20s in standby */
   /* it's done again after 1 hour of standby */
   /* Then, it's done every 24 hours */
#if 0
   if (EitFullCaching_CheckDone > 1)
   {
      return (24*60*60);
   }
   else
   {
      return (40*60);
   }
#else
   return (15 * 60);
#endif
}

/** 
 *
 *      This function returns EIT Full Caching estimated duration.
 *      As the EIT caching is an endless process, the EstimatedDuration
 *      is used to define the delay after which the EIT caching will be
 *      stopped. If value returned is 0, the EIT caching will continue
 *      until it is explicitly stopped (because another action is about
 *      to start or because the STB is woke up).
 *
 *
 * @return ::Estimated EIT Full Caching duration in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_EitFullCaching_EstimatedDuration(void)
{
#if 0
   if (EitFullCaching_CheckDone == 0)
   {
      /* 4 min */
      return (6*60);
   }
   else if (EitFullCaching_CheckDone == 1)
   {
      /* 10 min */
      return (25*60);
   }
   else
   {
    /* 1 hour */
    return (1*60*60);
    }
#else
	return 0;
#endif

}

/** 
 *
 *      This function returns guard time interval between the scheduled actions
 *      and the first user timer.
 *
 *
 * @return ::Guard time interval in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_User_Timer_Guard_Time_Interval(void)
{
    /* 15 min */
    return (15*60);
}

/** 
 *
 *      This function returns date & time acquisition timeout in standby state.
 *
 *
 * @return ::Acquisition timeout in milliseconds.
 *
 */
int SYS_MNGR_CFG_Standby_DateTime_Acquisition_Timeout(void)
{
    /* > 30 s */
    return (30500);
}

/** 
 *
 *      This function returns minimum time interval to the first user timer that comedia
 *      must have to enter the standby state.
 *
 *
 * @return ::Time interval in seconds.
 *
 */
int SYS_MNGR_CFG_Standby_To_UserTimer_Minimum_Interval(void)
{
    /* 100 s */
    return (100);
}

/** 
 *
 *      This function returns standby exit time offset.
 *      This value makes comedia to exit the standby state in advance
 *      to anticipate the user timer start.
 *
 *
 * @return ::Time offset in seconds.
 *
 */
int SYS_MNGR_CFG_Standby_Exit_Time_Offset(void)
{
    /* 5 s */
    return (10);
}

/** 
 *
 *      This function returns estimated start-up duration.
 *      This value may include hardware reset duration 
 *      if there is one.
 *
 *
 * @return ::Estimated duration in seconds.
 *
 */
int SYS_MNGR_CFG_Estimated_StartUp_Duration(void)
{
    /* 13 s */
    return (13);
}

/** 
 *
 * This function defines if the low power is enabled or not
 *
 *
 * @return ::TRUE if low power is enabled.
 * @return ::FALSE if low power is disabled.
 *
 */
bool SYS_MNGR_CFG_LowPower_Is_Enabled(void)
{
    return (FALSE);
}

/** 
 *
 * This function returns guard time interval between the low power start
 *      and the first user timer.
 *      That prevents comedia from starting the low power just before
 *      a user timer.
 *      If the value is -1, the low power is not started as long as
 *      user timers are scheduled.
 *
 *
 * @return ::Guard time interval in seconds.
 *
 */
int SYS_MNGR_CFG_LowPower_User_Timer_Guard_Time_Interval(void)
{
    int result = -1;
#if 0

    /*
     * to be worth, the guard time interval should exceed
     * the minimum low power duration plus the lower exit duration
     * and the minimum time to the first user timer.
     */

    result = SYS_MNGR_CFG_LowPower_Minimum_Duration()
           + SYS_MNGR_CFG_LowPower_Exit_Time_Offset()
           + 1;

    if (result < SYS_MNGR_CFG_Standby_To_UserTimer_Minimum_Interval())
    {
        result = SYS_MNGR_CFG_Standby_To_UserTimer_Minimum_Interval();
    }
#endif
    return (result);
}

/** 
 *
 * This function returns guard time interval between the low power start
 *      and the first scheduled action.
 *      That prevents comedia from starting the low power just before
 *      a scheduled action.
 *      If the value is -1, the low power is not started as long as
 *      actions are scheduled.
 *
 *
 * @return ::Guard time interval in seconds.
 *
 */
int SYS_MNGR_CFG_LowPower_Scheduled_Action_Guard_Time_Interval(void)
{
    int         result = 900; /* 15 minutes */
#if 0
    uint32_t    timeToFirstAction = 0;
    uint32_t    timeToSecondAction = 0;
    uint32_t    timeToThirdAction = 0;
    int32_t     duration = 0;

    /*
     * to be worth, the guard time interval should exceed
     * the minimum low power duration plus the lower exit duration
     * and should not occur the between actions scheduled from
     * the beginning of the standby state (i.e. relative time mode).
     */

    result = SYS_MNGR_CFG_LowPower_Minimum_Duration()
           + SYS_MNGR_CFG_LowPower_Exit_Time_Offset()
           + 1;

    if (SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_Enabled() &&
        !SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckTimeMode())
    {
        timeToFirstAction =
            SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_FirstCheckTime();
    }

    if (SYS_MNGR_CFG_StandbyCheck_EitFullCaching_Enabled() &&
        !SYS_MNGR_CFG_StandbyCheck_EitFullCaching_FirstCheckTime())
    {
        if (SYS_MNGR_CFG_StandbyCheck_EitFullCaching_FirstCheckTime() < timeToFirstAction)
        {
            timeToSecondAction = timeToFirstAction;
            timeToFirstAction =
                SYS_MNGR_CFG_StandbyCheck_EitFullCaching_FirstCheckTime();
        }
        else
        {
            timeToSecondAction =
                SYS_MNGR_CFG_StandbyCheck_EitFullCaching_FirstCheckTime();
        }
    }

    if (SYS_MNGR_CFG_StandbyCheck_Trigger_Enabled() &&
        !SYS_MNGR_CFG_StandbyCheck_Trigger_CheckTimeMode())
    {
        if (SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime() < timeToFirstAction)
        {
            timeToThirdAction = timeToSecondAction;
            timeToSecondAction = timeToFirstAction;
            timeToFirstAction =
                SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime();
        }
        else if (SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime() < timeToSecondAction)
        {
            timeToThirdAction = timeToSecondAction;
            timeToSecondAction =
                SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime();
        }
        else
        {
            timeToThirdAction =
                SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime();
        }
    }

    duration = timeToFirstAction;
    if (result < duration)
    {
        result = duration;
    }

    duration = (timeToSecondAction - timeToFirstAction);
    if (result < duration)
    {
        result = duration;
    }

    duration = (timeToThirdAction - timeToSecondAction);
    if (result < duration)
    {
        result = duration;
    }
#endif
    return (result);
}

/** 
 *
 * This function returns duration below which it is not worth starting the lower power.
 *      Below this value, comedia keeps going or restarts the standby instead of
 *      starting the low power.
 *
 *
 * @return ::Minimum duration in seconds.
 *
 */
int SYS_MNGR_CFG_LowPower_Minimum_Duration(void)
{
    /* 100 s */
    return (100);
}

/** 
 *
 * This function returns low power exit time offset.
 *      This value makes the low power to terminate in advance to anticipate
 *      the start-up duration. It may include the start-up duration,
 *      the date & time acquisition timeout, etc.
 *
 *
 * @return ::Time offset in seconds.
 *
 */
int SYS_MNGR_CFG_LowPower_Exit_Time_Offset(void)
{
    int result;

    result = SYS_MNGR_CFG_Estimated_StartUp_Duration()
           + (SYS_MNGR_CFG_Standby_DateTime_Acquisition_Timeout() / 1000) + 1
           + SYS_MNGR_CFG_Standby_Exit_Time_Offset();

    return (result);
}

/** 
 *
 * This function goes into low power for a maximum duration. The low power
 *      will be terminate by an user action or by an auto power on
 *      when the timeout will expire.
 *      According to the product it either calls the TDAL.
 *      If duration is 0, the product will never auto power on.
 *      If duration is low, the product may auto power on immediately.
 *
 * @param[in] duration: the maximum duration the product, or the OBM,
 *      will stay in low power.
 *
 */
void SYS_MNGR_CFG_EnterLowPowerTimeout(unsigned long duration)
{
    TDAL_EnterLowPowerTimeout((uint32_t)duration);
    return;   
}

/** 
 *
 * This function goes into low power for a maximum duration. The low power
 *      will be terminate by an user action or by an auto power on
 *      when the timeout will expire.
 *      According to the product it either calls the TDAL.
 *      If duration is 0, the product will never auto power on.
 *      If duration is low, the product may auto power on immediately.
 *
 * @param[out] status: reason of power on.
 * @param[out] duration: duration since power off.
 *
 * @return ::TRUE status is updated.
 * @return ::FALSE status is not accurate
 *
 */
bool SYS_MNGR_CFG_GetPowerOnStatus(
            tSYS_MNGR_CFG_PowerOnstatus  *status,
            unsigned long                *duration)
{
     bool result = true;
     tTDAL_PowerOnStatus TDALstatus;

    if(status == NULL || duration == NULL)
    {
        return false;
    }        
    

    result = TDAL_GetPowerOnStatus(&TDALstatus, (uint32_t*)duration, 0);

    if(result)
    {
        switch(TDALstatus)
        {
            case eTDAL_POWER_ON_FRONT_POWER_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_FRONT_POWER_KEY;
                break;
            case eTDAL_POWER_ON_RCU_POWER_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_POWER_KEY;
                break;
            case eTDAL_POWER_ON_RCU_0_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_0_KEY;
                break;
            case eTDAL_POWER_ON_RCU_1_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_1_KEY;
                break;
            case eTDAL_POWER_ON_RCU_2_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_2_KEY;
                break;
            case eTDAL_POWER_ON_RCU_3_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_3_KEY;
                break;
            case eTDAL_POWER_ON_RCU_4_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_4_KEY;
                break;
            case eTDAL_POWER_ON_RCU_5_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_5_KEY;
                break;
            case eTDAL_POWER_ON_RCU_6_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_6_KEY;
                break;
            case eTDAL_POWER_ON_RCU_7_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_7_KEY;
                break;
            case eTDAL_POWER_ON_RCU_8_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_8_KEY;
                break;
            case eTDAL_POWER_ON_RCU_9_KEY:
                *status = eSYS_MNGR_CFG_POWER_ON_RCU_9_KEY;
                break;
            case eTDAL_POWER_ON_AUTO_RESET:
                *status = eSYS_MNGR_CFG_POWER_ON_AUTO_RESET;
                break;
            case eTDAL_POWER_ON_AC_POWER_SUPPLY:
                *status = eSYS_MNGR_CFG_POWER_ON_AC_POWER_SUPPLY;
                break;
            case eTDAL_POWER_ON_WAKEUP_TIMER:     
                *status = eSYS_MNGR_CFG_POWER_ON_WAKEUP_TIMER;
                break;
            case eTDAL_POWER_ON_MISC:
            default:
                *status = eSYS_MNGR_CFG_POWER_ON_MISC;
                break;        
        }    
    }
    else
    {
        *status = eSYS_MNGR_CFG_POWER_ON_MISC;
        *duration = 0;
    }        

    return result;
}    

char *SYS_MNGR_CFG_GetHardwareDriverVersion(void)
{
    //return (TDAL_GetDriverVersion());
}

#if defined(EMM_WAKEUP) && defined(PRODUCT_USE_CA)
/** 
 *
 * This function defines if EMM wake up in standby is enabled or not
 *
 *
 * @return ::TRUE if EMM wake up in stand by is enabled.
 * @return ::FALSE if EMM wake up in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_Enabled(void)
{
    /* Available states */
    /* true: enabled */
    /* false: disabled */

    return NAGRA_EMMWakeUpActivated();
}

bool SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_CheckTimeMode(void)
{
    /* Available modes */
    /* true: absolute time => the EMM processing starts at a given hour (e.g. 03:00) */
    /* false: relative time => the EMM processing starts after a given duration in standby */

    return false;
}

int SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_FirstCheckTime(void)
{ 
   
   return NAGRA_EMMWakeUpGetSleepingDuration();
}

int SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_Period(void)
{

   return  (NAGRA_EMMWakeUpGetSleepingDuration()+ NAGRA_EMMWakeUpGetWakingDuration());
}

int SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_EstimatedDuration(void)
{

   return NAGRA_EMMWakeUpGetWakingDuration();
}

#endif
