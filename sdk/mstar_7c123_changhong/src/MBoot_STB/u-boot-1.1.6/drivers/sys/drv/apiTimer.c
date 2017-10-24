#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>

extern MS_U32 volatile gSystemTimeCount;

MS_U32 msAPI_Timer_GetSystemTime(void)
{
    return gSystemTimeCount;
}

/******************************************************************************/
/// API for Get system time ticks
/// @return -Current system time counts, the unit is system default.
/******************************************************************************/
MS_U32 msAPI_Timer_GetTime0(void)
{
    return MsOS_GetSystemTime();
}

/******************************************************************************/
/// API for Calculating the difference between time in parameter and current time.
/// @param -u32TaskTimer \b IN  2nd time value to calculate
/// @return -Time difference between current time and task time
/******************************************************************************/
MS_U32 msAPI_Timer_DiffTimeFromNow(MS_U32 u32TaskTimer) //unit = ms
{
    return MsOS_Timer_DiffTimeFromNow(u32TaskTimer);
}

/******************************************************************************/
/// API for Delay function
/// @param -u32DelayTime \b IN  required time period to delay.
/******************************************************************************/
void msAPI_Timer_Delayms(MS_U32 u32DelayTime) //unit = ms
{
    MsOS_DelayTask(u32DelayTime);
}

