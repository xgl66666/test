#ifndef __DRV_DISH_H__
#define __DRV_Dish_H__
#include <sys/common/MsTypes.h>
#include <sys/common/MsOS.h>
#include <sys/common/MsCommon.h>
#include "frontend_config.h"
typedef enum
{
    EN_LNBPWR_TYPE_OFF,
    EN_LNBPWR_TYPE_13V,
    EN_LNBPWR_TYPE_18V,
    EN_LNBPWR_TYPE_14V,
    EN_LNBPWR_TYPE_19V,
    EN_LNBPWR_TYPE_NUM
}DISH_LNBPWR_TYPE;

typedef enum
{
    EN_LNB22K_TYPE_OFF,
    EN_LNB22K_TYPE_ON,
    EN_LNB22K_TYPE_NUM
}DISH_LNB22K_TYPE;

typedef enum
{
    EN_TONEBURST_TYPE_0,
    EN_TONEBURST_TYPE_1,
    EN_TONEBURST_NUM
}EN_TONEBURST_TYPE;
extern MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
extern MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);
extern MS_BOOL MDrv_DiSEqC_SetLNBOut(MS_BOOL bLow);

extern MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1);

//--------------------------------------------------------
// initialize
// param none
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_Init(void);
//--------------------------------------------------------
// set toneburst
// param enToneType \b IN:toneburst type
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_SetTone(EN_TONEBURST_TYPE enToneType);
//--------------------------------------------------------
// set lnb power type
// param enLNBPwr \b IN:power type
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_SetLNBPower(DISH_LNBPWR_TYPE enLNBPwr);
//--------------------------------------------------------
// set lnb 22k type
// param enLNB22k \b IN:22k type
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_Set22k(DISH_LNB22K_TYPE enLNB22k);
//--------------------------------------------------------
// dish send command
// param pCmd \b IN:command
//          u8CmdSize \b IN:command size
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);

//--------------------------------------------------------
// dish check over current
// param None
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_IsOverCurrent(void);

#endif // #define __DRV_DISH_H__
