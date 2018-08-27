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
#include "drvDemod.h"
#include "drvIIC.h"
#include "drvDishNull.h"

#if MS_DVBS_INUSE
/*
#if(DISH_TYPE == DISH_A8302) ||\
   (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8302)) || \
   (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8302)) || \
   (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8302))
*/
#define A8302_SLAVE_ADDR_R   0x11
#define A8302_SLAVE_ADDR   0x10

//   A8296  control register
/*
bit7 bit6 bit5 bit4
0000 //unused

bit3
0: Disable LNB Output
1: Enable LNB Output

bit2 bit1 bit0
VSEL2 VSEL1 VSEL0 LNB (V)
0x0//0 0 0     13.333
0x1//0 0 1     13.667
0x2//0 1 0     14.000
0x3//0 1 1     14.333
0x4//1 0 0     18.667
0x5//1 0 1     19.000
0x6//1 1 0     19.667
0x7//1 1 1     20.000
*/
//bit3 bit2 bit1 bit0
#define DATA_POWER_13V     0x3  //13.667
#define DATA_POWER_18V     0xB  //18.667

//bit4  0: Disable LNB Output  1: Enable LNB Output
#define BIT_LNB_OFF           (0x0 << 4)
#define BIT_LNB_ON            (0x1 << 4)


//Status Register 0x01
#define LNB1_OUTPUT_DISABLED   (0x1<<0)
#define LNB2_OUTPUT_DISABLED   (0x1<<1)
#define LNB1_OVERCURRENT        (0x1<<2)
#define LNB2_OVERCURRENT        (0x1<<3)
#define POWER1_NOT_GOOD         (0x1<<4)
#define POWER2_NOT_GOOD         (0x1<<5)

//Status Register 0x01
#define LNB1_TONE_DETECT (0x01<<2)
#define LNB2_TONE_DETECT (0x01<<3)


#define DATA_LNBPOWER_OFF            (DATA_POWER_13V | BIT_LNB_OFF)
#define DATA_13V_OUT                  (DATA_POWER_13V | BIT_LNB_ON)
#define DATA_18V_OUT                  (DATA_POWER_18V | BIT_LNB_ON)

#define DMD_Diseqc_TX_NONE         0xFF

 //static MS_U8 u8PowerData = DATA_LNBPOWER_OFF;

/************************************************************************************/
static DISH_MS_INIT_PARAM DishInitParam[MAX_LNB_SUPPORT];
static MS_U8 TCTRL2DiSeqcTX[MAX_LNB_SUPPORT][2];
static MS_BOOL TCTRL2DiSeqcTX_Check[MAX_LNB_SUPPORT];

static MS_IIC_PORT _get_I2C_port(MS_U8 u8DishIndex)
{
    return DishInitParam[u8DishIndex].stLNBCon.eI2C_PORT;
}

static MS_BOOL _read_Status_Reg(MS_U8 u8DishIndex, MS_U8 u8Addr, MS_U8* pu8Data)
{
    MS_U8 u8Data=0;
    MS_U8 u8BuffTemp[2];
    MS_BOOL bRet = TRUE;

    u8BuffTemp[0] = u8Addr;
    bRet &= MDrv_IIC_WriteBytes(_get_I2C_port(u8DishIndex), A8302_SLAVE_ADDR, 0, NULL, 1, &u8BuffTemp[0]);
    bRet &= MDrv_IIC_ReadBytes(_get_I2C_port(u8DishIndex),A8302_SLAVE_ADDR_R, 0, NULL, 1, &u8Data);
    *pu8Data = u8Data;
    return bRet;
}

static MS_BOOL _check_LNB_Status(MS_U8 u8DishIndex, MS_U8 u8CableIndex, MS_BOOL* pbLNBReady)
{
    MS_U8 u8Data=0;

    if(!_read_Status_Reg(u8DishIndex, 0x00, &u8Data))
        return FALSE;

    *pbLNBReady = TRUE;

    switch(u8CableIndex)
    {
         case 0:
             if((u8Data & LNB1_OUTPUT_DISABLED) || (u8Data & POWER1_NOT_GOOD))
             {
                 *pbLNBReady = FALSE;
                 DISH_DBG(("[A8302] LNB1 NOT Ready\n"));
             }
             break;
         case 1:
             if((u8Data & LNB2_OUTPUT_DISABLED) || (u8Data & POWER2_NOT_GOOD))
             {
                 *pbLNBReady = FALSE;
                  DISH_DBG(("[A8302] LNB2 NOT Ready\n"));
             }
             break;
         default:
             *pbLNBReady = FALSE;
             break;
    }

    return TRUE;
}

static MS_BOOL _do_TCTRL2DiSeqcTX_Check(MS_U8 u8DishIndex)
{
    MS_U8 u8Data=0;
    MS_BOOL bRet = TRUE, bLNBReady=FALSE;
    MS_U8 u8DMD_DiSeqcTX, u8TCTRL;
    MS_U8 u8ToneDetect;

    if(TCTRL2DiSeqcTX_Check[u8DishIndex])
         return TRUE;

    DISH_DBG(("####### [A8302] LNB out Pin mapping to DMD Diseqc TX ######\n"));
    for(u8TCTRL=0;u8TCTRL<2;u8TCTRL++)
    {
        bRet &= _check_LNB_Status(u8DishIndex, u8TCTRL, &bLNBReady);
        if(!bLNBReady)
            continue;

        if(u8TCTRL)
           u8ToneDetect = LNB2_TONE_DETECT;
        else
           u8ToneDetect = LNB1_TONE_DETECT;

        for(u8DMD_DiSeqcTX=0;u8DMD_DiSeqcTX<2;u8DMD_DiSeqcTX++)
        {
            bRet &= DishInitParam[u8DishIndex].pstDemodtab->Extension_Function(u8DishIndex, DEMOD_EXT_FUNC_SET_DISEQC_TX, &u8DMD_DiSeqcTX);

            //Check diable 22k, status reg 0x01 TDET1/2 shoud be 0
            bRet &= DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, FALSE);
            _read_Status_Reg(u8DishIndex, 0x01, &u8Data);
            if(u8Data & u8ToneDetect)
                continue;

            //Check enable 22k, status reg 0x01 TDET1/2 shoud be 1
            bRet &= DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, TRUE);
            _read_Status_Reg(u8DishIndex, 0x01, &u8Data);
            if(u8Data & u8ToneDetect)
            {
                TCTRL2DiSeqcTX[u8DishIndex][u8TCTRL] = u8DMD_DiSeqcTX;
                break;
            }
            else
            {
                TCTRL2DiSeqcTX[u8DishIndex][u8TCTRL] = DMD_Diseqc_TX_NONE;
            }

        }

        DISH_DBG(("[A8302] LNB%x ==> DMD_DiseqcTX%x\n", u8TCTRL+1, TCTRL2DiSeqcTX[u8DishIndex][u8TCTRL]));
    }

     TCTRL2DiSeqcTX_Check[u8DishIndex] = TRUE;
     return bRet;

}


MS_BOOL MDrv_Dish_A8302_Init(MS_U8 u8DishIndex,DISH_MS_INIT_PARAM* pParam)
{
    MS_U8 u8Data=0;
    MS_U8 u8BuffTemp[2];
    //note: before write need to read first when init

    if(pParam->pstDemodtab== NULL)
        return FALSE;
    else
    {
        DishInitParam[u8DishIndex].pstDemodtab = pParam->pstDemodtab;
        DishInitParam[u8DishIndex].stLNBCon.eI2C_PORT= pParam->stLNBCon.eI2C_PORT;
    }

    MDrv_HWI2C_SetReadMode(E_HWI2C_READ_MODE_DIRECTION_CHANGE);
    MDrv_IIC_Init(_get_I2C_port(u8DishIndex));

    DishInitParam[u8DishIndex].u8CableIndex= 0;
    TCTRL2DiSeqcTX[u8DishIndex][0] = DMD_Diseqc_TX_NONE;
    TCTRL2DiSeqcTX[u8DishIndex][1] = DMD_Diseqc_TX_NONE;
    TCTRL2DiSeqcTX_Check[u8DishIndex] = FALSE;
    if(!MDrv_IIC_ReadBytes(_get_I2C_port(u8DishIndex),A8302_SLAVE_ADDR_R, 0, NULL, 1, &u8Data))
    {
        DISH_ERR(("\n MDrv_Dish_Init failed-1-ReadBytes fail!\n"));
    }

/*  Turn on if initial value is necessary for Control Register 0
    u8Data = 0;         //Initial Control Register to 0
    u8BuffTemp[1]=u8Data;
    u8BuffTemp[0]=0x00; //write to Control Register 0
    if(!MDrv_IIC_WriteBytes(_get_I2C_port(u8DishIndex), A8302_SLAVE_ADDR, 0, NULL, 2, &u8BuffTemp[0]))
    {
        DISH_ERR(("\n MDrv_Dish_Init failed-2-WriteBytes fail!"));
        return FALSE;
    }
*/

    u8Data = DATA_13V_OUT;
    u8BuffTemp[1]=u8Data;
    u8BuffTemp[0]=0x01; //write to Control Register 1
    DishInitParam[u8DishIndex].u8CurControlReg = u8Data;
    if(!MDrv_IIC_WriteBytes(_get_I2C_port(u8DishIndex), A8302_SLAVE_ADDR, 0, NULL, 2, &u8BuffTemp[0]))
    {
        DISH_ERR(("\n MDrv_Dish_Init failed-2-WriteBytes fail!"));
        return FALSE;
    }

    u8BuffTemp[0]=0x02; //write to Control Register 2
    if(!MDrv_IIC_WriteBytes(_get_I2C_port(u8DishIndex), A8302_SLAVE_ADDR, 0, NULL, 2, &u8BuffTemp[0]))
    {
        DISH_ERR(("\n MDrv_Dish_Init failed-2-WriteBytes fail!"));
        return FALSE;
    }

    u8BuffTemp[0]=0x00; //read Status Register 0
    MDrv_IIC_WriteBytes(_get_I2C_port(u8DishIndex), A8302_SLAVE_ADDR, 0, NULL, 1, &u8BuffTemp[0]);
    if(!MDrv_IIC_ReadBytes(_get_I2C_port(u8DishIndex),A8302_SLAVE_ADDR_R, 0, NULL, 1, &u8Data))
    {
        DISH_ERR(("\n MDrv_Dish_Init failed-3-ReadBytes fail!"));
    }

    return TRUE;
}
/************************************************************************************/
MS_BOOL MDrv_Dish_A8302_SetTone(MS_U8 u8DishIndex,EN_TONEBURST_TYPE enToneType)
{
    MS_BOOL bRet = TRUE;
    MS_U8  u8DMDTxIndex;

    bRet &= _do_TCTRL2DiSeqcTX_Check(u8DishIndex);
    u8DMDTxIndex = TCTRL2DiSeqcTX[u8DishIndex][DishInitParam[u8DishIndex].u8CableIndex];
    if(DMD_Diseqc_TX_NONE == u8DMDTxIndex)
        return FALSE;

    bRet &= DishInitParam[u8DishIndex].pstDemodtab->Extension_Function(u8DishIndex, DEMOD_EXT_FUNC_SET_DISEQC_TX, &u8DMDTxIndex);

      switch(enToneType)
      {
           case EN_TONEBURST_TYPE_0:
               bRet &=  DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSetTone(u8DishIndex,FALSE);
               break;
           case EN_TONEBURST_TYPE_1:
               bRet &= DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSetTone(u8DishIndex,TRUE);
               break;
           default:
               DISH_ERR(("----%s error type %d----",__FUNCTION__,enToneType));
               break;
      }

      return bRet;
}
/************************************************************************************/
MS_BOOL MDrv_Dish_A8302_SetLNBPower(MS_U8 u8DishIndex, DISH_LNBPWR_TYPE enLNBPwr)
{
    MS_U8 u8Data = 0;
    MS_BOOL b8result=0;
    MS_U8 u8BuffTemp[2];
    MS_U8 u8Index;
    u8BuffTemp[0]=0x00;

    u8Index = DishInitParam[u8DishIndex].u8CableIndex;
    u8Data = DishInitParam[u8DishIndex].u8CurControlReg;
    if(u8Index == 0)
    {
        u8BuffTemp[0]=0x01;
    }
    else if(u8Index == 1)
    {
        u8BuffTemp[0]=0x02;
    }
    u8Data &= (~(0x1f));

      switch(enLNBPwr)
      {
           case EN_LNBPWR_TYPE_OFF:
               u8Data |= DATA_LNBPOWER_OFF;
               break;
           case EN_LNBPWR_TYPE_13V:
               u8Data |= DATA_13V_OUT;
               break;
           case EN_LNBPWR_TYPE_18V:
               u8Data |= DATA_18V_OUT;
               break;
           default:
           u8Data = DATA_13V_OUT;
               DISH_ERR(("----%s error type %d ----",__FUNCTION__,enLNBPwr));
               return FALSE;
      }

    u8BuffTemp[1]=u8Data;
    DishInitParam[u8DishIndex].u8CurControlReg = u8Data;
    b8result=MDrv_IIC_WriteBytes(_get_I2C_port(u8DishIndex), A8302_SLAVE_ADDR, 0, NULL, 2, &u8BuffTemp[0]);
    if(0==b8result)
    {
        DISH_ERR(("\n MDrv_Dish_SetLNBPower failed-!"));
    }

    return b8result;

}
/************************************************************************************/
MS_BOOL MDrv_Dish_A8302_Set22k(MS_U8 u8DishIndex,DISH_LNB22K_TYPE enLNB22k)
{
      MS_BOOL bRet = TRUE;
      MS_U8 u8DMDTxIndex;

      bRet &= _do_TCTRL2DiSeqcTX_Check(u8DishIndex);
      u8DMDTxIndex = TCTRL2DiSeqcTX[u8DishIndex][DishInitParam[u8DishIndex].u8CableIndex];
      if(DMD_Diseqc_TX_NONE == u8DMDTxIndex)
          return FALSE;

      bRet &= DishInitParam[u8DishIndex].pstDemodtab->Extension_Function(u8DishIndex, DEMOD_EXT_FUNC_SET_DISEQC_TX, &u8DMDTxIndex);

      switch(enLNB22k)
      {
           case EN_LNB22K_TYPE_OFF:
               DISH_DBG(("EN_LNB22K_TYPE_OFF\n"));
               bRet &= DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, FALSE);
               break;
           case EN_LNB22K_TYPE_ON:
               DISH_DBG(("EN_LNB22K_TYPE_ON\n"));
               bRet &= DishInitParam[u8DishIndex].pstDemodtab->DiSEqCSet22kOnOff(u8DishIndex, TRUE);
               break;
           default:
               DISH_ERR(("----%s error type %d----",__FUNCTION__,enLNB22k));
               bRet = FALSE;
               break;
      }

       return bRet;
}
/************************************************************************************/
MS_BOOL MDrv_Dish_A8302_SendCmd(MS_U8 u8DishIndex,MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet = TRUE;
   MS_U8 u8DMDTxIndex;

    bRet &= _do_TCTRL2DiSeqcTX_Check(u8DishIndex);
    u8DMDTxIndex = TCTRL2DiSeqcTX[u8DishIndex][DishInitParam[u8DishIndex].u8CableIndex];
    if(DMD_Diseqc_TX_NONE == u8DMDTxIndex)
        return FALSE;

    bRet &= DishInitParam[u8DishIndex].pstDemodtab->Extension_Function(u8DishIndex, DEMOD_EXT_FUNC_SET_DISEQC_TX, &u8DMDTxIndex);
    bRet &= DishInitParam[u8DishIndex].pstDemodtab->DiSEqC_SendCmd(u8DishIndex, pCmd,u8CmdSize);
     return bRet;
}
/************************************************************************************/
MS_BOOL MDrv_Dish_A8302_IsOverCurrent(MS_U8 u8DishIndex)
{
   MS_U8 u8Status;

    if( _read_Status_Reg(u8DishIndex, 0x00, &u8Status))
    {
         if((u8Status&LNB1_OVERCURRENT) || (u8Status&LNB2_OVERCURRENT))
         {
         //    HB_printf("\n----MDrv_Dish_IsOverCurrent=0x%x ----",u8Status);
             return TRUE;
         }
    }
    return FALSE;
}

MS_BOOL MDrv_Dish_A8302_SetCable(MS_U8 u8DishIndex, EN_CABLE_SELECT eCableIndex)
{
    if((eCableIndex < EN_CABLE_LNB_NUM) && (eCableIndex != EN_CABLE_LNB_NOT_SET))
    {
        DishInitParam[u8DishIndex].u8CableIndex = (MS_U8)(eCableIndex)-1;
        DISH_DBG(("[A8302] set Cable %x\n", DishInitParam[u8DishIndex].u8CableIndex));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


DISHTAB_ENTRY(dish_entry_DISH_A8302,"DISH_A8302", DISH_A8302,
            MDrv_Dish_A8302_Init,
            MDrv_Dish_A8302_SetTone,
            MDrv_Dish_A8302_SetLNBPower,
            MDrv_Dish_A8302_Set22k,
            MDrv_Dish_A8302_SendCmd,
            MDrv_Dish_A8302_IsOverCurrent,
            MDrv_Dish_A8302_SetCable
);

#endif

