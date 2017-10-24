/******************************************************************************
 *                     COPYRIGHT 2011 SMARDTV                                 *
 ******************************************************************************
 * MODULE NAME: sys_mngr_cfg
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/sys_mngr_cfg.h $
 * Header file for system state configuration Interface
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 */
/** @section SYS_MNGR_CFG Configuration Interface
 *
 *
 * @see The file build/inc/sys_mngr_cfg.h details this interface technically.
 *
 * @file
 * This is the system manager configuration interface.
 *
 *
 *
 *****************************************************************************/


#ifndef _SYS_MNGR_CFG_H_
#define _SYS_MNGR_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*               typedef                               */
/*******************************************************/


/** Reason of power on. */
typedef enum
{
    eSYS_MNGR_CFG_POWER_ON_FRONT_POWER_KEY = 0,    /* 1 */
	eSYS_MNGR_CFG_POWER_ON_RCU_POWER_KEY,          /* 2 */
	eSYS_MNGR_CFG_POWER_ON_RCU_0_KEY = 0x10,       /* 0x10 */
	eSYS_MNGR_CFG_POWER_ON_RCU_1_KEY,              /* 0x11 */
	eSYS_MNGR_CFG_POWER_ON_RCU_2_KEY,              /* 0x12 */
	eSYS_MNGR_CFG_POWER_ON_RCU_3_KEY,              /* 0x13 */
	eSYS_MNGR_CFG_POWER_ON_RCU_4_KEY,              /* 0x14 */
	eSYS_MNGR_CFG_POWER_ON_RCU_5_KEY,              /* 0x15 */
	eSYS_MNGR_CFG_POWER_ON_RCU_6_KEY,              /* 0x16 */
	eSYS_MNGR_CFG_POWER_ON_RCU_7_KEY,              /* 0x17 */
	eSYS_MNGR_CFG_POWER_ON_RCU_8_KEY,              /* 0x18 */
	eSYS_MNGR_CFG_POWER_ON_RCU_9_KEY,              /* 0x19 */
	eSYS_MNGR_CFG_POWER_ON_AUTO_RESET = 0x20,
	eSYS_MNGR_CFG_POWER_ON_AC_POWER_SUPPLY,
	eSYS_MNGR_CFG_POWER_ON_WAKEUP_TIMER,
	eSYS_MNGR_CFG_POWER_ON_MISC
}tSYS_MNGR_CFG_PowerOnstatus;
    
/********************************************************/
/*              Global File Variables                   */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/** 
 *
 * This function defines if channel update in standby is enabled or not
 *
 *
 * @return ::TRUE if channel update in stand by is enabled.
 * @return ::FALSE if channel update in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_Enabled(void);

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
bool SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckTimeMode(void);


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
int SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_FirstCheckTime(void);

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
int SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_Period(void);

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
void SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_CheckDone(void);

/** 
 *
 *      This function returns estimated duration of channel update
 *
 *
 * @return ::Estimated channel update duration in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_ChannelUpdate_EstimatedDuration(void);

/** 
 *
 * This function defines if trigger in standby is enabled or not
 *
 *
 * @return ::TRUE if trigger in stand by is enabled.
 * @return ::FALSE if trigger in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_Trigger_Enabled(void);

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
bool SYS_MNGR_CFG_StandbyCheck_Trigger_CheckTimeMode(void);

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
int SYS_MNGR_CFG_StandbyCheck_Trigger_FirstCheckTime(void);

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
void SYS_MNGR_CFG_StandbyCheck_Trigger_CheckDone(void);

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
int SYS_MNGR_CFG_StandbyCheck_Trigger_Period(void);

/** 
 *
 *      This function returns trigger estimated duration
  *
 * @return ::Estimated trigger duration in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_Trigger_EstimatedDuration(void);


/** 
 *
 * This function defines if EIT full caching in standby is enabled or not
 *
 *
 * @return ::TRUE if EIT full caching in stand by is enabled.
 * @return ::FALSE if EIT full caching in stand by is disabled.
 *
 */
bool SYS_MNGR_CFG_StandbyCheck_EitFullCaching_Enabled(void);

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
bool SYS_MNGR_CFG_StandbyCheck_EitFullCaching_CheckTimeMode(void);

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
int SYS_MNGR_CFG_StandbyCheck_EitFullCaching_FirstCheckTime(void);

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
void SYS_MNGR_CFG_StandbyCheck_EitFullCaching_CheckDone(void);

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
int SYS_MNGR_CFG_StandbyCheck_EitFullCaching_Period(void);

/** 
 *
 *      This function returns EIT Full Caching estimated duration.
 *
 *
 * @return ::Estimated EIT Full Caching duration in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_EitFullCaching_EstimatedDuration(void);


/** 
 *
 *      This function returns guard time interval between the scheduled actions
 *      and the first user timer.
 *
 *
 * @return ::Guard time interval in seconds.
 *
 */
int SYS_MNGR_CFG_StandbyCheck_User_Timer_Guard_Time_Interval(void);

/** 
 *
 *      This function returns date & time acquisition timeout in standby state.
 *
 *
 * @return ::Acquisition timeout in milliseconds.
 *
 */
int SYS_MNGR_CFG_Standby_DateTime_Acquisition_Timeout(void);

/** 
 *
 *      This function returns minimum time interval to the first user timer that comedia
 *      must have to enter the standby state.
 *
 *
 * @return ::Time interval in seconds.
 *
 */
int SYS_MNGR_CFG_Standby_To_UserTimer_Minimum_Interval(void);

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
int SYS_MNGR_CFG_Standby_Exit_Time_Offset(void);

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
int SYS_MNGR_CFG_Estimated_StartUp_Duration(void);

/** 
 *
 * This function defines if the low power is enabled or not
 *
 *
 * @return ::TRUE if low power is enabled.
 * @return ::FALSE if low power is disabled.
 *
 */
bool SYS_MNGR_CFG_LowPower_Is_Enabled(void);

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
int SYS_MNGR_CFG_LowPower_User_Timer_Guard_Time_Interval(void);

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
int SYS_MNGR_CFG_LowPower_Scheduled_Action_Guard_Time_Interval(void);

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
int SYS_MNGR_CFG_LowPower_Minimum_Duration(void);


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
int SYS_MNGR_CFG_LowPower_Exit_Time_Offset(void);

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
void SYS_MNGR_CFG_EnterLowPowerTimeout(unsigned long duration);

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
            unsigned long                *duration);

char *SYS_MNGR_CFG_GetHardwareDriverVersion(void);


#ifdef EMM_WAKEUP
bool SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_Enabled(void);
bool SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_CheckTimeMode(void);
int SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_FirstCheckTime(void);
int SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_Period(void);
int SYS_MNGR_CFG_StandbyCheck_EMMWakeUp_EstimatedDuration(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _SYS_MNGR_CFG_H_ */
