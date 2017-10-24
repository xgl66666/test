#ifndef __DIBCOM_PLATFORM_H__
#define __DIBCOM_PLATFORM_H__

#if 0
#include <stdio.h>
#include <math.h>
#include <string.h>
#endif
// embed the code on zoran platform.
// If your platform is not zoran, please remove the define.

#include <sys/common/MsCommon.h>

extern void MsOS_DelayTask (MS_U32 u32Ms);
extern void MsOS_DelayTaskUs (MS_U32 u32Us);
extern MS_U32 MsOS_GetSystemTime (void);

#define DIBCOM_EXTENDED_MONITORING
#define systime MsOS_GetSystemTime
#define Out(x)          //x
//#define Out_(name) { printf("%-30s",name); }
#define DibZeroMemory(buf, len) memset(buf, 0, len)
//#define DibMSleep(ms) MsOS_DelayTask(ms)
#define DibMSleep(ms) MsOS_DelayTaskUs(ms)
#define DibUSleep(us) MsOS_DelayTaskUs(us)
#define DBG_DIB8096(format, args...) //printf("%04d %s " format, __LINE__, __FUNCTION__, ##args)


#endif
