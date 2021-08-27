
//#include "Board.h"
//#include "MsCommon.h"
//#include "HbCommon.h"
//#include "drvDemod.h"
//#include "drvIIC.h"
#include <common.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include "sys/drv/drvHWI2C.h"
#include "sys/drv/drvIIC.h"
#include "drvDish.h"
//#include "../../../customer/cus_cfg_info.h"
#if(ENABLE_DVB_S_SYS &&(DISH_TYPE == DISH_A8293))

#define FRONTEND_TUNER_PORT         E_HWI2C_PORT0_1

#define A8293_SLAVE_ADDR  0x10

//   A8293  Address 0 0
#define BIT_POWER_LOW    (0x0 << 3)
#define BIT_POWER_HIGH   (0x1 << 3)

#define DATA_POWER_12V     0x0
#define DATA_POWER_13V     0x1
#define DATA_POWER_14V     0x4
#define DATA_POWER_18V     BIT_POWER_HIGH
#define DATA_POWER_19V     (0x3 | BIT_POWER_HIGH)

#define BIT_ODT                         (0x1 << 4)
#define BIT_LNB_ENABLE           (0x1 << 5)


//   A8293  Address  1 0
#define EXT_TONE     (0x0<<0)
#define INT_TONE     (0x1<<0)

//   A8293  Status Register 1 Bit
#define LNB_OUTPUT_DISABLED    (0x1<<0)
#define OVERCURRENT                    (0x1<<2)
#define POWER_NOT_GOOG            (0x1<<4)
#define THERMAL_SHUTDONW        (0x1<<6)
#define UNDERVOLTAGE                  (0x1<<7)

#define READ_CMD    0x1               //  Bit 0
#define WRITE_CMD  0x0

//   A8293  WRITE ADDRESS
#define ADDRESS_LNB_POWER       (0x0<<1)    // BIT 2(A1)/1(A0)
//#define ADDRESS_TONE                 0x

//   A8293  READ ADDRESS
#define ADDRESS_READ                  (0x0<<1)

#define CMD_PREFIX                       (0x2<<3)
#define CMD_READ                          (CMD_PREFIX | ADDRESS_READ | READ_CMD)
#define CMD_WRITE                        (CMD_PREFIX | ADDRESS_LNB_POWER | WRITE_CMD)

#define DATA_LNBPOWER_OFF       (DATA_POWER_13V | BIT_ODT)					//  BIT_LNB_ENABLE
#define DATA_13V_OUT                  (DATA_POWER_13V | BIT_ODT  | BIT_LNB_ENABLE)
#define DATA_14V_OUT                  (DATA_POWER_14V | BIT_ODT  | BIT_LNB_ENABLE)
#define DATA_18V_OUT                  (DATA_POWER_18V | BIT_ODT  | BIT_LNB_ENABLE)
#define DATA_19V_OUT                  (DATA_POWER_19V | BIT_ODT  | BIT_LNB_ENABLE)


//  22K
#define TMODE_EXTERNAL           (0<<0)
#define TMODE_INTERNAL           (1<<0)

#define TGATE_OFF           (0<<1)
#define TGATE_ON             (1<<1)

#define TONE_PREFIX        (0x20<<2)

#define DATA_22K_OFF     (TONE_PREFIX | TGATE_OFF  | TMODE_EXTERNAL)
#define DATA_22K_ON       (TONE_PREFIX | TGATE_ON   | TMODE_EXTERNAL)

MS_BOOL MDrv_Dish_Init(void)
{
    MS_U8 u8Data ;

    u8Data = DATA_22K_OFF;
    if(!MDrv_IIC_WriteBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data))
    {
         printf("\n MDrv_Dish_Init failed1!");
         return FALSE;
    }

    u8Data = EN_LNBPWR_TYPE_13V;
    if(!MDrv_IIC_WriteBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data))
    {
         printf("\n MDrv_Dish_Init failed2!");
         return FALSE;
    }

    if(MDrv_IIC_ReadBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data))
    {
         printf("\n MDrv_Dish_Init Read 0x%X\n",u8Data);
    }
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
      MS_U8 u8Data = DATA_13V_OUT;
      switch(enLNBPwr)
      {
           case EN_LNBPWR_TYPE_OFF:
               u8Data= DATA_LNBPOWER_OFF;
               break;
           case EN_LNBPWR_TYPE_13V:
               u8Data = DATA_13V_OUT;
               break;
           case EN_LNBPWR_TYPE_14V:
               u8Data = DATA_14V_OUT;
               break;
           case EN_LNBPWR_TYPE_18V:
               u8Data = DATA_18V_OUT;
               break;
           case EN_LNBPWR_TYPE_19V:
                u8Data = DATA_19V_OUT;
               break;
           default:
               printf("----%s error type %d ----",__FUNCTION__,enLNBPwr);
               return FALSE;
      }
      return MDrv_IIC_WriteBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data);
}

MS_BOOL MDrv_Dish_Set22k(DISH_LNB22K_TYPE enLNB22k)
{
      MS_U8 u8Data = DATA_22K_OFF ;
      switch(enLNB22k)
      {
           case EN_LNB22K_TYPE_OFF:
               u8Data = DATA_22K_OFF;
               MDrv_DiSEqC_Set22kOnOff(FALSE);
               break;
           case EN_LNB22K_TYPE_ON:
               u8Data = DATA_22K_ON;
               MDrv_DiSEqC_Set22kOnOff(TRUE);
               break;
           default:
               printf("----%s error type %d----",__FUNCTION__,enLNB22k);
               return FALSE;
      }
      return MDrv_IIC_WriteBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data);
}

MS_BOOL MDrv_Dish_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
     MS_U8 u8Data = DATA_22K_ON;
     MDrv_IIC_WriteBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data);
     return MDrv_DiSEqC_SendCmd(pCmd,u8CmdSize);
}

MS_BOOL MDrv_Dish_IsOverCurrent(void)
{
    MS_U8 u8Status;
    if(MDrv_IIC_WriteBytes(A8293_SLAVE_ADDR, 0, NULL, 1, &u8Status))
    {
         if(u8Status&OVERCURRENT)
         {
             return TRUE;
         }
    }
    return FALSE;
}

#endif

