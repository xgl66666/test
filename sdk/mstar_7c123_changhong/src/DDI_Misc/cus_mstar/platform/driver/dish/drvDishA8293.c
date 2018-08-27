//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#include "MsCommon.h"
#include "drvDish.h"
#include "drvDishNull.h"
#include "drvDemod.h"
#include "drvIIC.h"

#if MS_DVBS_INUSE
/*
#if(DISH_TYPE == DISH_A8293) ||\
   (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8293)) || \
   (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8293)) || \
   (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8293))
*/

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

#define DATA_LNBPOWER_OFF       (DATA_POWER_13V | BIT_ODT)//  BIT_LNB_ENABLE
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

static DISH_MS_INIT_PARAM DishInitParam[MAX_LNB_SUPPORT];

static MS_IIC_PORT _get_I2C_port(MS_U8 u8DishIndex)
{
   return DishInitParam[u8DishIndex].stLNBCon.eI2C_PORT;
}


MS_BOOL MDrv_Dish_A8293_Init(MS_U8 u8DishIndex, DISH_MS_INIT_PARAM* pParam)
{
    MS_U8 u8Data ;
    HWI2C_PORT ePort;

    if(pParam->pstDemodtab== NULL)
        return FALSE;
    else
    {
        DishInitParam[u8DishIndex].pstDemodtab = pParam->pstDemodtab;
        DishInitParam[u8DishIndex].stLNBCon.eI2C_PORT= pParam->stLNBCon.eI2C_PORT;
    }
        
    u8Data = DATA_22K_OFF;

    ePort = _get_I2C_port(u8DishIndex);

    if(!MDrv_IIC_WriteBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data))
    {
         DISH_ERR(("\n MDrv_Dish_Init failed1!"));
         return FALSE;
    }

    u8Data = EN_LNBPWR_TYPE_13V;
    if(!MDrv_IIC_WriteBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data))
    {
         DISH_ERR(("\n MDrv_Dish_Init failed2!"));
         return FALSE;
    }

    if(MDrv_IIC_ReadBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data))
    {
         DISH_ERR(("\n MDrv_Dish_Init Read 0x%X\n",u8Data));
    }
    return TRUE;
}

MS_BOOL MDrv_Dish_A8293_SetTone(MS_U8 u8DishIndex, EN_TONEBURST_TYPE enToneType)
{
      switch(enToneType)
      {
           case EN_TONEBURST_TYPE_0:
               //return MDrv_DiSEqC_SetTone(FALSE);
               return DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSetTone(u8DishIndex,FALSE);
               break;
           case EN_TONEBURST_TYPE_1:
               //return MDrv_DiSEqC_SetTone(TRUE);
               return DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSetTone(u8DishIndex,TRUE);
               break;
           default:
               DISH_ERR(("----%s error type %d----",__FUNCTION__,enToneType));
               break;
      }
      return FALSE;
}

MS_BOOL MDrv_Dish_A8293_SetLNBPower(MS_U8 u8DishIndex, DISH_LNBPWR_TYPE enLNBPwr)
{
    MS_U8 u8Data = DATA_13V_OUT;
    HWI2C_PORT ePort;

    ePort = _get_I2C_port(u8DishIndex);


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
           DISH_ERR(("----%s error type %d ----",__FUNCTION__,enLNBPwr));
           return FALSE;
    }
    return MDrv_IIC_WriteBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data);
}

MS_BOOL MDrv_Dish_A8293_Set22k(MS_U8 u8DishIndex, DISH_LNB22K_TYPE enLNB22k)
{
    MS_U8 u8Data = DATA_22K_OFF ;
    HWI2C_PORT ePort;

    ePort = _get_I2C_port(u8DishIndex);


    switch(enLNB22k)
    {
       case EN_LNB22K_TYPE_OFF:
           u8Data = DATA_22K_OFF;
           //MDrv_DiSEqC_Set22kOnOff(FALSE);
           DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, FALSE);
           break;
       case EN_LNB22K_TYPE_ON:
           u8Data = DATA_22K_ON;
           //MDrv_DiSEqC_Set22kOnOff(TRUE);
           DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, TRUE);
           break;
       default:
           DISH_ERR(("----%s error type %d----",__FUNCTION__,enLNB22k));
           return FALSE;
    }
    return MDrv_IIC_WriteBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data);
}

MS_BOOL MDrv_Dish_A8293_SendCmd(MS_U8 u8DishIndex, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_U8 u8Data = DATA_22K_ON;
    HWI2C_PORT ePort;

    ePort = _get_I2C_port(u8DishIndex);

    MDrv_IIC_WriteBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Data);
    //return MDrv_DiSEqC_SendCmd(pCmd,u8CmdSize);
    return DishInitParam[u8DishIndex].pstDemodtab->DiSEqC_SendCmd(u8DishIndex, pCmd,u8CmdSize);
}

MS_BOOL MDrv_Dish_A8293_IsOverCurrent(MS_U8 u8DishIndex)
{
    MS_U8 u8Status=0;
    HWI2C_PORT ePort;

    ePort = _get_I2C_port(u8DishIndex);

    if(MDrv_IIC_WriteBytes(ePort, A8293_SLAVE_ADDR, 0, NULL, 1, &u8Status))
    {
         if(u8Status&OVERCURRENT)
         {
             return TRUE;
         }
    }
    return FALSE;
}

MS_BOOL MDrv_Dish_A8293_SetCable(MS_U8 u8DishIndex, EN_CABLE_SELECT eCableIndex)
{       
   return FALSE;
}



DISHTAB_ENTRY(dish_entry_DISH_A8293,"DISH_A8293", DISH_A8293,
            MDrv_Dish_A8293_Init,
            MDrv_Dish_A8293_SetTone,
            MDrv_Dish_A8293_SetLNBPower,
            MDrv_Dish_A8293_Set22k,
            MDrv_Dish_A8293_SendCmd,
            MDrv_Dish_A8293_IsOverCurrent,
            MDrv_Dish_A8293_SetCable
);


#endif

