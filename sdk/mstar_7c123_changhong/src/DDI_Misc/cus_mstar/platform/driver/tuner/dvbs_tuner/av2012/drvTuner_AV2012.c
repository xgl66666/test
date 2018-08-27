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
#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "drvTuner_AV2012.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "drvDTC.h"
#include <string.h>

#if IF_THIS_TUNER_INUSE(TUNER_AV2012) || IF_THIS_TUNER_INUSE(TUNER_AV2011)

#define  TUNER_AV2012_SLAVE_ID      0xc4//0xC0//0xC6
#define  TUNER_CRYSTAL_FREQ         27
//static MS_U8 _u8SlaveID = TUNER_AV2012_SLAVE_ID;
static TUNER_MS_INIT_PARAM* pInitParam = NULL;
static MS_U8 u8max_dev_num = 0;

#if IF_THIS_TUNER_INUSE(TUNER_AV2011)
MS_U8 TunerInitialSetting_0[2][42]=
{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x50, 0xA1, 0x2F, 0x50, 0x1F, 0xA3, 0xFD, 0x58, 0x0E, 0x82, 0x88, 0xB4, 0xD6, 0x40, 0x94,0x9A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80,0x02, 0xF5, 0x7F, 0x4A, 0x9B, 0xE0, 0xE0, 0x36, 0x00, 0xAB, 0x97, 0xC5, 0xA8}
};

#else
MS_U8 TunerInitialSetting_0[2][42]=
{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x50, 0xA1, 0x2F, 0x50, 0x1F, 0xA3, 0xFD, 0x00, 0x2E, 0x82, 0x88, 0xB4, 0x96, 0x40, 0x5B,0x6A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80,0x00, 0xF5, 0x7F, 0x4A, 0x9B, 0xE0, 0xE0, 0x36, 0x02, 0xAB, 0x97, 0xC5, 0xA8}
};
#endif
static MS_BOOL _get_dev_param(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM** ppParam)
{
    if((pInitParam == NULL) || (u8TunerIndex > u8max_dev_num))
        return FALSE;

    *ppParam = pInitParam + u8TunerIndex;
    return TRUE;
}

static MS_BOOL _getI2CPort(MS_U8 u8TunerIndex, MS_IIC_PORT* pePort)
{
    TUNER_MS_INIT_PARAM* pParam = NULL;

     if(!_get_dev_param(u8TunerIndex, &pParam))
        return FALSE;

     *pePort = pParam->stTUNCon.eI2C_PORT;
     return TRUE;
}

static MS_BOOL _variable_alloc(void** pp, MS_U32 size)
{
    if(NULL == *pp)
    {
        *pp = malloc(size*u8max_dev_num);
        if(NULL == *pp)
        {
            return FALSE;
        }
        else
        {
           memset(*pp, 0, size*u8max_dev_num);
        }
    }
    return TRUE;
}

static void _variable_free(void** pp)
{
    if(NULL != *pp)
    {
        free(*pp);
        *pp = NULL;
    }
}

static MS_BOOL _variables_alloc(void)
{
    MS_BOOL bRet = TRUE;

    bRet &= _variable_alloc((void*)&pInitParam, sizeof(TUNER_MS_INIT_PARAM));
    return bRet;
}

static MS_BOOL _variables_free(void)
{
    _variable_free((void*)&pInitParam);
    return TRUE;
}

static  MS_BOOL  _DigiTuner_Decide_LNB_LO(TUNER_MS_SAT_PARAM *pSATParam)
{
     MS_S32 s32Freq;
     // calc Mid Freq & LNB control(22K on/off)
     if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
     {
        *pSATParam->pbIsHiLOF= FALSE;
     }
     else // 2LOF
     {
         MS_U32 u32MidFreq;
         MS_S32 s32Offset = 0;
         s32Freq  = pSATParam->u32RF_FREQ;
         u32MidFreq =  abs(s32Freq - pSATParam->u16LoLOF) ;
         s32Offset = pSATParam->u16LoLOF+ MAX_INPUT_FREQ - pSATParam->u16HiLOF - MIN_INPUT_FREQ;
         if(s32Offset < 0)
              s32Offset = 0;
          else
             s32Offset /= 2;

         if( u32MidFreq <= (MAX_INPUT_FREQ-s32Offset))
              *pSATParam->pbIsHiLOF = FALSE;
          else
              *pSATParam->pbIsHiLOF = TRUE;     
     }
     return TRUE;
}

static MS_BOOL AV2012_WriteReg(MS_U8 u8TunerIndex, MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Value[2];
    MS_IIC_PORT ePort;
    
    if(!_getI2CPort(u8TunerIndex, &ePort))
        return FALSE;

    u8Value[0]=u8Addr;
    u8Value[1]=u8Data;
    //bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, u8Value, 2);
    bRet &= MDrv_IIC_WriteBytes(ePort, u8SlaveID, 0, 0, 2, u8Value);

    return bRet;
}
static MS_BOOL AV2012_ReadReg(MS_U8 u8TunerIndex, MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_IIC_PORT ePort;
    
    if(!_getI2CPort(u8TunerIndex, &ePort))
        return FALSE;

    //bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, &u8Addr, 1);
    bRet &= MDrv_IIC_WriteBytes(ePort, u8SlaveID, 0, 0, 1, &u8Addr);
    //bRet&=MDrv_IIC_Read(u8SlaveID, 0, 0, u8Data, 1);
    bRet &= MDrv_IIC_ReadBytes(ePort, u8SlaveID, 0, 0, 1, u8Data);
    return bRet;
}

static  void AV2012_SlaveID_Check(MS_U8 u8TunerIndex)
{
     MS_U8 regValue;
     TUNER_MS_INIT_PARAM* pParam = NULL;

     if(!_get_dev_param(u8TunerIndex, &pParam))
        return;
     
     pParam->u8SlaveID = 0xC0;
      do
      {
          regValue=(char) (0x38);
          if(AV2012_WriteReg(u8TunerIndex, pParam->u8SlaveID,0,regValue))
          {
               regValue = 0;
               if(AV2012_ReadReg(u8TunerIndex, pParam->u8SlaveID,0,&regValue))
               {
                     if(regValue == 0x38)
                     {
                         break;
                     }
               }
          }
          pParam->u8SlaveID += 0x02;
      }while(pParam->u8SlaveID <= 0xC6);
      if(pParam->u8SlaveID > 0xC6)
      {
           pParam->u8SlaveID = TUNER_AV2012_SLAVE_ID;
      }
}

static MS_BOOL AV2012_Init(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 index, u8SlaveID;
    TUNER_MS_INIT_PARAM* pParam = NULL;

     if(!_get_dev_param(u8TunerIndex, &pParam))
        return FALSE;

    //AV2012_SlaveID_Check(u8TunerIndex);
    u8SlaveID = pParam->u8SlaveID;
    
    for (index=0; index < 12; index++)
    {
        bRet&=AV2012_WriteReg(u8TunerIndex, u8SlaveID, TunerInitialSetting_0[0][index], TunerInitialSetting_0[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2012_WriteReg(u8TunerIndex, u8SlaveID, TunerInitialSetting_0[0][index], TunerInitialSetting_0[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2012_WriteReg(u8TunerIndex, u8SlaveID, TunerInitialSetting_0[0][12], TunerInitialSetting_0[1][12]);
    MsOS_DelayTask(100);
    for (index=0; index < 12; index++)
    {
        bRet&=AV2012_WriteReg(u8TunerIndex, u8SlaveID, TunerInitialSetting_0[0][index], TunerInitialSetting_0[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2012_WriteReg(u8TunerIndex, u8SlaveID, TunerInitialSetting_0[0][index], TunerInitialSetting_0[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2012_WriteReg(u8TunerIndex, u8SlaveID, TunerInitialSetting_0[0][12], TunerInitialSetting_0[1][12]);
    MsOS_DelayTask(50);
    return bRet;
}

MS_BOOL MDrv_Tuner_AV2012_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    TUNER_MS_INIT_PARAM* pstParam = NULL;

     if(!_get_dev_param(u8TunerIndex, &pstParam))
        return FALSE;

    if((pParam->pCur_Broadcast_type == NULL) || (pParam->pstDemodtab == NULL))
        return FALSE;
    else
    {
        pstParam->pCur_Broadcast_type = pParam->pCur_Broadcast_type;
        pstParam->pstDemodtab = pParam->pstDemodtab;
        pstParam->stTUNCon.eI2C_PORT = pParam->stTUNCon.eI2C_PORT;
        pstParam->stTUNCon.u32HW_ResetPin= pParam->stTUNCon.u32HW_ResetPin;
    }
    
    return AV2012_Init(u8TunerIndex);
}

MS_BOOL MDrv_Tuner_AV2012_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bRet=TRUE;
    MS_BOOL bAutoScan=FALSE;
    MS_U32 u32FracN;
    MS_U32 BW;
    MS_U32 BF;
    MS_U8 u8Reg[8];
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    if(!_get_dev_param(u8TunerIndex, &pstParam))
        return FALSE;

    TUNER_DBG(("u16CenterFreq:%"DTC_MS_U32_d" u32SymbolRate_Hz:%"DTC_MS_U32_d"\n",u32CenterFreq,u32SymbolRate_Hz));
    if((u32CenterFreq > MAX_INPUT_FREQ) || (u32CenterFreq < MIN_INPUT_FREQ))
        return FALSE;
    if (u32SymbolRate_Hz == 0 || u32SymbolRate_Hz == 45000) //auto-scan mode
    {
        bAutoScan = TRUE;
    }

    u32FracN = (u32CenterFreq + TUNER_CRYSTAL_FREQ/2)/TUNER_CRYSTAL_FREQ;
    if(u32FracN > 0xff)
    {
        u32FracN = 0xff;
    }
    u8Reg[0]=(MS_U8) (u32FracN & 0xff);
    u32FracN = (u32CenterFreq<<17)/TUNER_CRYSTAL_FREQ;
    u32FracN = (u32FracN & 0x1ffff);
    u8Reg[1]=(MS_U8) ((u32FracN>>9)&0xff);
    u8Reg[2]=(MS_U8) ((u32FracN>>1)&0xff);
    u8Reg[3]=(MS_U8) (((u32FracN<<7)&0x80) | 0x50); // default is 0x50
    // Channel Filter Bandwidth Setting.
    if(bAutoScan==TRUE)//requested by BB
    {
        u8Reg[5] = 0xA3; //BW=27MHz
    }
    else
    {
   	    // rolloff is 35%
   	    BW = u32SymbolRate_Hz*135/200;
   	    // monsen 20080726, BB request low IF when sym < 6.5MHz
   	    // add 6M when Rs<6.5M,
   	    if(u32SymbolRate_Hz<6500)
   	    {
   	    	BW = BW + 6000;
   	    }
   	    	// add 2M for LNB frequency shifting
            BW = BW + 1000;
   	    	// add 8% margin since fc is not very accurate
   	   // BW = BW*108/100;
   	    	// Bandwidth can be tuned from 4M to 40M
   	    if( BW< 4000)
   	    {
   	    	BW = 4000;
   	    }
   	    if( BW> 40000)
   	    {
   	    	BW = 40000;
   	    }
   	    BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
   	    u8Reg[5] = (MS_U8)BF;
    }
    // Sequence 4
    // Send Reg0 ->Reg4
    MsOS_DelayTask(5);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x00, u8Reg[0]);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x01, u8Reg[1]);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x02, u8Reg[2]);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x03, u8Reg[3]);
    MsOS_DelayTask(100);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x00, u8Reg[0]);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x01, u8Reg[1]);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x02, u8Reg[2]);
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x03, u8Reg[3]);
    MsOS_DelayTask(5);
    // Sequence 5
    // Send Reg5
    bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x05, u8Reg[5]);
    MsOS_DelayTask(5);
    // Fine-tune Function Control
    //Tuner fine-tune gain function block. bit2.
    //not auto-scan case. enable block function. FT_block=1, FT_EN=1
    if (bAutoScan==FALSE)
    {
         u8Reg[6] = 0x06;
         bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x25, u8Reg[6]);
         MsOS_DelayTask(5);
         //Disable RFLP at Lock Channel sequence after reg[37]
         //RFLP=OFF at Lock Channel sequence
         // RFLP can be Turned OFF, only at Receving mode.
         u8Reg[7] = 0xD6;
         bRet&=AV2012_WriteReg(u8TunerIndex, pstParam->u8SlaveID, 0x0C, u8Reg[7]);

    }
    return bRet;
}
MS_BOOL MDrv_Tuner_AV2012_CheckLock(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    if(!_get_dev_param(u8TunerIndex, &pstParam))
        return FALSE;

    bRet&=AV2012_ReadReg(u8TunerIndex, pstParam->u8SlaveID, 0x0B, &u8Data);
    if (bRet==FALSE)
    {
        return bRet;
    }
    else
    {
        if ((u8Data&0x03)!=0x03)
        {
            bRet=FALSE;
        }
    }
    //printf("Tuner Status 0x%x\n", u8Data);
    return bRet;
}

#define AV2012_CHIP_ID 0x7f

MS_BOOL MDrv_Tuner_AV2012_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 regData = 0;
    MS_U8 regData2 = 0;
    MS_U8 regData3 = 0;
    MS_U8 i=0;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    if(!_variables_alloc())
    {
        _variables_free();
        return FALSE;
    }

    AV2012_SlaveID_Check(u8TunerIndex);
    if(!_get_dev_param(u8TunerIndex, &pstParam))
        return FALSE;

    for (;i<5;i++)
    {
        if(!AV2012_WriteReg(u8TunerIndex,pstParam->u8SlaveID, 0x0C, 0xF6))
            continue;
        if(!AV2012_WriteReg(u8TunerIndex,pstParam->u8SlaveID, 0x1C, 0x00))
            continue;
        if(!AV2012_ReadReg(u8TunerIndex,pstParam->u8SlaveID, 0x1C, &regData))
            continue;
        if(!AV2012_ReadReg(u8TunerIndex,pstParam->u8SlaveID, 0x33, &regData2))
            continue;
        if(!AV2012_ReadReg(u8TunerIndex,pstParam->u8SlaveID, 0x34, &regData3))
            continue;
        TUNER_DBG(("[av2012] read id =0x%x reg0x33=0x%x reg0x34=0x%x\n",regData,regData2,regData3));
        
        if ( regData == AV2012_CHIP_ID )
        {
            if(NULL != pu32channel_cnt)
                *(pu32channel_cnt) += 1;
            return TRUE;
        }
    }
    
    return FALSE;
}


MS_BOOL AV2012_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE;
    MS_U8 regData = 0;
    TUNER_EXT_FUNCTION_PARAM* pstData = NULL;
    TUNER_MS_INIT_PARAM* pstParam = NULL;
    TUNER_CON_CONFIG* pstCon = NULL;
    
    switch(fuction_type)
    {
         case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _DigiTuner_Decide_LNB_LO(SAT_PARAM);
            break;
            
         case TUNER_EXT_FUNC_POWER_ON_OFF:
            if(!_get_dev_param(u8TunerIndex, &pstParam))
                return FALSE;
            
             bret &= AV2012_ReadReg(u8TunerIndex,pstParam->u8SlaveID, 0x0C, &regData);
            if(FALSE == *(MS_BOOL *)data)   //power off
            {
               regData |= (0x1<<5);
               bret &= AV2012_WriteReg(u8TunerIndex,pstParam->u8SlaveID, 0x0C, regData);
            }
            else
            {
               if((regData & (0x1<<5)) >>5)
               {
                  bret &= AV2012_Init(u8TunerIndex);
               }
            }
            break;
            
         case TUNER_EXT_FUNC_LOOP_THROUGH:
             if(!_get_dev_param(u8TunerIndex, &pstParam))
                return FALSE;
             
             bret &= AV2012_ReadReg(u8TunerIndex,pstParam->u8SlaveID, 0x0C, &regData);
            if(FALSE == *(MS_BOOL *)data)   //LT off
            {
               regData &= (~(0x1<<6));
            }
            else
            {
               regData |= (0x1<<6);
            }

            bret &= AV2012_WriteReg(u8TunerIndex,pstParam->u8SlaveID, 0x0C, regData);
            break;
         case TUNER_EXT_FUNC_SET_CON_INFO:
            pstData = (TUNER_EXT_FUNCTION_PARAM*)data;
            u8max_dev_num = pstData->u32Param1;
            pstCon = (TUNER_CON_CONFIG*)(pstData->pParam);
            if(!_variables_alloc())
            {
                _variables_free();
                return FALSE;
            }
            else
            {
                pstParam = pInitParam + u8TunerIndex;
                memcpy(&pstParam->stTUNCon, pstCon,sizeof(TUNER_CON_CONFIG));
            }
        break;    
         default:
            break;
    }
    return bret;
}


DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_AV2012) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_AV2012",          // demod system name
    .data               = TUNER_AV2012,            // private data value
    .Init               = MDrv_Tuner_AV2012_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_AV2012_SetFreq_S2,
    .GetLock            = MDrv_Tuner_AV2012_CheckLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_AV2012_CheckExist,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .Extension_Function = AV2012_Extension_Function
};


#endif
