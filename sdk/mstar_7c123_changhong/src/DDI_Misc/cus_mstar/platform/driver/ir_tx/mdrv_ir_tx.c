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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvIR.c
/// @brief  IR Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "string.h"
#include "Board.h"
#include <math.h>

#if defined(CHIP_K5TN)
#include "drv_IR_TX.h"
#include "mdrv_ir_tx.h"
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define IR_TX_PRINT(fmt, args...)      printf("[IR_TX] "fmt, ## args)


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static IR_TX_ENCODE_TYPE eCurIRSpec = IR_TX_ENCODE_ALL;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if 0
static MS_BOOL _MDrv_IR_TX_Status_Polling(void)
{
    MS_U32 u32StartTime;
    MS_BOOL bRet = FALSE;

    u32StartTime = MsOS_GetSystemTime();
    do
    {
     //polling done flag not ready
    }while((MsOS_GetSystemTime() - u32StartTime) < IR_TX_TIMEOUT);
    return bRet;
}
#endif
static void _CntRounding(MS_U16* pu16Cnt, MS_U16* pu16CarrierCnt, MS_U16* pu16Duty)
{
    MS_U16 u16Cnt, u16CCnt, u16Duty, u16ExtCnt;

    u16Cnt = *pu16Cnt;
    u16CCnt = *pu16CarrierCnt;
    u16Duty = *pu16Duty;

    if(u16Cnt < u16CCnt)
        return;
    else
    {
        u16Cnt /= u16CCnt;
        u16Cnt *= u16CCnt;
        u16ExtCnt = (u16CCnt * u16Duty)/100 + (u16CCnt * (100 - u16Duty))/200;
        u16Cnt += u16ExtCnt;
        *pu16Cnt = u16Cnt;
    }
    
}

static MS_BOOL _MDrv_IR_TX_SetNECUnit(void)
{
   MS_BOOL bRet = TRUE;
   MS_U8 u8ClkDiv;
   MS_U16 u16CarrierCntH, u16CarrierCntL, u16Duty;
   MS_U16 u16CarrierCnt;
   MS_U16 OneT; //ns
   MS_U16 H_ShotCount;
   MS_U16 L_ShotCount;
   MS_U16 Unit_Value = 0x02;
   MS_U32 u32OneCycleCnt;
   MS_U16 u16OneCycleCnt_H, u16OneCycleCnt_L;

   OneT = 1000000/(SAMPLING_RATE_NEC);
   //set sampling clk
   u8ClkDiv = (MS_U8)((MCU_CLK/SAMPLING_RATE_NEC) - 1);
   bRet &= MDrv_IR_TX_SetClkDiv(u8ClkDiv);
   IR_TX_PRINT("CLK_DIV = %x\n", u8ClkDiv);

   //set carrier
   u16Duty = (MS_U16)NEC_CARRIER_DUTY;
   u16CarrierCnt = (MS_U16)(SAMPLING_RATE_NEC*10 + 5*NEC_CARRIER_CLK);
   u16CarrierCnt /=(NEC_CARRIER_CLK * 10); 
   u16CarrierCntH = u16CarrierCnt* u16Duty;
   u16CarrierCntL = u16CarrierCnt* (100-u16Duty);
   IR_TX_PRINT("Carrier cnt 0x%x\n", (int)u16CarrierCnt);
   bRet &= MDrv_IR_TX_SetCarrierCount(((u16CarrierCntL/100) << 8) | (u16CarrierCntH/100));  

   //set cycle time
   u32OneCycleCnt = NEC_CYCLE_TIME * 1000 /OneT;
   u16OneCycleCnt_H = (MS_U16)((u32OneCycleCnt&0xffff0000) >> 16);
   u16OneCycleCnt_L = (MS_U16)((u32OneCycleCnt&0x0000ffff));
   IR_TX_PRINT("CycleCnt H:0x%x L:%x\n", u16OneCycleCnt_H, u16OneCycleCnt_L);
   bRet &= MDrv_IR_TX_SetDelayCycleTime(u16OneCycleCnt_H, u16OneCycleCnt_L);


   //set unit
   //Unit 0: logic 0
   L_ShotCount = (MS_U16)(NEC_LOGIC_0_L * 1000 / OneT);
   H_ShotCount = (MS_U16)(NEC_LOGIC_0_H * 1000 / OneT);
   _CntRounding(&H_ShotCount, &u16CarrierCnt, &u16Duty);
   IR_TX_PRINT("Unit0 ShotCnt H:0x%x L:0x%x\n", H_ShotCount, L_ShotCount);
   bRet &= MDrv_IR_TX_SetShotCount(H_ShotCount, L_ShotCount, 0);
   bRet &= MDrv_IR_TX_SetUnitValue(Unit_Value, 0);

   //Unit 1: logic 1
   L_ShotCount = (MS_U16)(NEC_LOGIC_1_L * 1000 /OneT);
   H_ShotCount = (MS_U16)(NEC_LOGIC_1_H * 1000 /OneT);
   _CntRounding(&H_ShotCount, &u16CarrierCnt, &u16Duty);
   IR_TX_PRINT("Unit1 ShotCnt H:0x%x L:0x%x\n", H_ShotCount, L_ShotCount);
   bRet &= MDrv_IR_TX_SetShotCount(H_ShotCount, L_ShotCount, 1);
   bRet &= MDrv_IR_TX_SetUnitValue(Unit_Value, 1);

   //Unit 2: header code + off code
   L_ShotCount = (MS_U16)(NEC_OFF_CODE * 1000 /OneT);
   H_ShotCount = (MS_U16)(NEC_HEADER_CODE * 1000 /OneT);
   _CntRounding(&H_ShotCount, &u16CarrierCnt, &u16Duty);
   IR_TX_PRINT("Unit2 ShotCnt H:0x%x L:0x%x\n", H_ShotCount, L_ShotCount);
   bRet &= MDrv_IR_TX_SetShotCount(H_ShotCount, L_ShotCount, 2);
   bRet &= MDrv_IR_TX_SetUnitValue(Unit_Value, 2);

   //Unit 3: header code + off code for repeat
   L_ShotCount = (MS_U16)(NEC_OFF_CODE_REPEAT* 1000 /OneT);
   H_ShotCount = (MS_U16)(NEC_HEADER_CODE * 1000 /OneT);
   _CntRounding(&H_ShotCount, &u16CarrierCnt, &u16Duty);
   IR_TX_PRINT("Unit3 ShotCnt H:0x%x L:0x%x\n", H_ShotCount, L_ShotCount);
   bRet &= MDrv_IR_TX_SetShotCount(H_ShotCount, L_ShotCount, 3);
   bRet &= MDrv_IR_TX_SetUnitValue(Unit_Value, 3);

   return bRet;
}

static MS_BOOL _MDrv_IR_TX_SendNECKey(IR_TX_KEY_CODE* pKeyCode)
{
   MS_BOOL bRet = TRUE;
   MS_U8 u8CusCode, u8CusCodeInv;
   MS_U8 u8CmdCode, u8CmdCOdeInv;
   MS_U8 u8Unit[34],i;
   MS_U32 u32DataOut = 0;
   MS_U16 u16MemData = 0;

   u8CusCode = pKeyCode->NECKeyCode.u8Customer_Code;
   u8CmdCode = pKeyCode->NECKeyCode.u8Command_Code;
   u8CusCodeInv = ~u8CusCode;
   u8CmdCOdeInv = ~u8CmdCode;

   u32DataOut = u8CusCode | (u8CusCodeInv<<8) | (u8CmdCode << 16) | (u8CmdCOdeInv << 24);

   bRet &= MDrv_IR_TX_SetMemStatus(0x5100);//mem auto increase

   if(pKeyCode->NECKeyCode.bRepeat)
   {
       u16MemData = (0x03 + (0xe<<8));
       bRet &= MDrv_IR_TX_SetMemData(u16MemData);
   }
   else
   {
       u8Unit[0] = 0x02; //Unit 2
       for(i=0;i<32;i++)
       {
           u8Unit[i + 1] = (MS_U8)(u32DataOut & 0x01);
           u32DataOut = u32DataOut>>1;
       }
       u8Unit[33] = 0x0e;//end

       bRet &= MDrv_IR_TX_SetMemAddr(0x00);

       for(i=0;i<34;i++)
       {
          u16MemData |= u8Unit[i]<< (4*(i%4));
          if((((i+1)%4) == 0) || (i == 33))
          {
              bRet &= MDrv_IR_TX_SetMemData(u16MemData);
              u16MemData = 0;
          }
       }
   }
   //MsOS_DelayTask(100);
   bRet &= MDrv_IR_TX_SetStatus(IR_TX_STATUS_EN | IR_TX_STATUS_RST_RELEASE | IR_TX_STATUS_INT_EN | IR_TX_STATUS_CARRIER_EN);
   bRet &= MDrv_IR_TX_Trigger();
   return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Initialize IR TX
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IRTX_Init(void)
{
    MS_BOOL bRet = TRUE;
    bRet &= MDrv_IR_TX_Init();
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Set which protocol will be used.
/// @param  eIRSpec \b IN: IR Spec.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IR_TX_SetIRSpec(IR_TX_ENCODE_TYPE eIRSpec)
{
   MS_BOOL bRet = TRUE;
   
   switch(eIRSpec)
   {
     case IR_TX_ENCODE_NEC:
        IR_TX_PRINT("Set for NEC IR protocol ...\n");
        bRet &= _MDrv_IR_TX_SetNECUnit();
        if(bRet)
           eCurIRSpec = IR_TX_ENCODE_NEC;
        break;
     default:
        IR_TX_PRINT("IR_TX not Ready for this Spec.");
        bRet = FALSE;
        break;
   }

   return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Get current IR Spec.
/// @param  peIRSpec \b IN: pointer of IR Spec.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IR_TX_GetIRSpec(IR_TX_ENCODE_TYPE* peIRSpec)
{
   *peIRSpec = eCurIRSpec;
   return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set which key code will be send
/// @param  pParam \b IN: key code
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_IR_TX_SendKey(IR_TX_KEY_CODE* pKeyCode)
{
   MS_BOOL bRet = TRUE;
   
   switch(eCurIRSpec)
   {
     case IR_TX_ENCODE_NEC:
        _MDrv_IR_TX_SendNECKey(pKeyCode);
        break;
     default:
        IR_TX_PRINT("IR_TX not Ready for this Spec.");
        bRet = FALSE;
        break;
   }

   return bRet;
}


#endif

