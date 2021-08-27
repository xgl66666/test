
//#include "Board.h"
//#include "MsCommon.h"
//#include "HbCommon.h"
#include "drvDish.h"
#include "../../../include/device/drvDemod.h"
//#include "drvGPIO.h"
//#include "../../../customer/cus_cfg_info.h"
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include <sys/drv/drvHWI2C.h>
#include "../../../include/prana2/stdio.h"


#if(ENABLE_DVB_S_SYS&&(DISH_TYPE == DISH_DEMOD))

//#define GPIO_ANT_OVERCURRENT            9999
#define GPIO_ANT_PWR_CTRL               20
#define GPIO_ANT_OVERCURRENT            42

#define ANT_PWR_SET_ON()   mdrv_gpio_set_high(GPIO_ANT_PWR_CTRL)
#define ANT_PWR_SET_OFF()   mdrv_gpio_set_low(GPIO_ANT_PWR_CTRL)
#define IS_ANT_OVERCURRENT() (!mdrv_gpio_get_level(GPIO_ANT_OVERCURRENT))

MS_BOOL MDrv_Dish_Init(void)
{
    return TRUE;
}

MS_BOOL MDrv_Dish_SetTone(EN_TONEBURST_TYPE enToneType)
{
      switch(enToneType)
      {
           case EN_TONEBURST_TYPE_0:
               return MDrv_DiSEqC_SetTone(FALSE);
               break;
           case EN_TONEBURST_TYPE_1:
               return MDrv_DiSEqC_SetTone(TRUE);
               break;
           default:
               printf("----%s error type %d----",__FUNCTION__,enToneType);
               break;
      }
      return FALSE;
}

MS_BOOL MDrv_Dish_SetLNBPower(DISH_LNBPWR_TYPE enLNBPwr)
{
      switch(enLNBPwr)
      {
           case EN_LNBPWR_TYPE_OFF:
               ANT_PWR_SET_OFF();
               break;
           case EN_LNBPWR_TYPE_13V:
           case EN_LNBPWR_TYPE_14V:
               ANT_PWR_SET_ON();
               return MDrv_DiSEqC_SetLNBOut(FALSE);
               break;
           case EN_LNBPWR_TYPE_18V:
           case EN_LNBPWR_TYPE_19V:
               ANT_PWR_SET_ON();
               return MDrv_DiSEqC_SetLNBOut(TRUE);
               break;
           default:
               printf("----%s error type %d ----",__FUNCTION__,enLNBPwr);
               break;
      }
      return FALSE;
}

MS_BOOL MDrv_Dish_Set22k(DISH_LNB22K_TYPE enLNB22k)
{
      switch(enLNB22k)
      {
           case EN_LNB22K_TYPE_OFF:
               return MDrv_DiSEqC_Set22kOnOff(FALSE);
               break;
           case EN_LNB22K_TYPE_ON:
               return MDrv_DiSEqC_Set22kOnOff(TRUE);
               break;
           default:
               printf("----%s error type %d----",__FUNCTION__,enLNB22k);
               break;
      }
      return FALSE;
}

MS_BOOL MDrv_Dish_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
     return MDrv_DiSEqC_SendCmd(pCmd,u8CmdSize);
}

MS_BOOL MDrv_Dish_IsOverCurrent(void)
{
     mdrv_gpio_set_input(GPIO_ANT_OVERCURRENT);
     return IS_ANT_OVERCURRENT();
}

#endif

