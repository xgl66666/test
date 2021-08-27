#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "Board.h"
#include "drvTuner_AV2018.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_AV2018)
#define  TUNER_AV2018_SLAVE_ID      0xC6 //0xC4 // 0xC0//0xC6
#define  TUNER_CRYSTAL_FREQ         27
static MS_U8 _u8SlaveID = TUNER_AV2018_SLAVE_ID;

static MS_U8 sCurRolloff = E_SAT_RO_35;

// New setting for BBCL.
// Modify R32=4A->4D & R33=9B->BF & R36=36->2E for AV2018 RFAGC issue
MS_U8 TunerInitialSetting[2][42]=
{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x38, 0x00, 0x00, 0x50, 0x1F, 0xA3, 0xFD, 0x58, 0x0E, 0x82, 0x88, 0xB4, 0xD6, 0x40, 0x94, 0x4A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80, 0x0E, 0xF5, 0x7F, 0x4D, 0xBF, 0xE0, 0xE0, 0x2E, 0x02, 0xAB, 0x97, 0xC5, 0xA8}
};

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

MS_BOOL AV2018_WriteReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Value[2];
    u8Value[0]=u8Addr;
    u8Value[1]=u8Data;
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, u8Value, 2);
    if (!bRet)
    {
        TUNER_ERR(("AV2018_WriteReg fail \n"));
    }
    return bRet;
}
MS_BOOL AV2018_ReadReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data)
{
    MS_BOOL bRet = TRUE;
    
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, &u8Addr, 1);
    bRet&=MDrv_IIC_Read(u8SlaveID, 0, 0, u8Data, 1);
    if (!bRet)
    {
        TUNER_ERR(("AV2018_ReadReg fail \n"));
    }
    return bRet;
}

void AV2018_SlaveID_Check(void)
{
     MS_U8 regValue;

      _u8SlaveID = 0xC0;
      do
      {
          regValue=(char) (0x38);
          if(AV2018_WriteReg(_u8SlaveID,0,regValue))
          {
               regValue = 0;
               if(AV2018_ReadReg(_u8SlaveID,0,&regValue))
               {
                     if(regValue == 0x38)
                     {
                         break;
                     }
               }
          }
          _u8SlaveID += 0x02;
      }while(_u8SlaveID <= 0xC6);
      if(_u8SlaveID > 0xC6)
      {
           _u8SlaveID = TUNER_AV2018_SLAVE_ID;
      }
      TUNER_DBG(("\n\n@@@@@@@@@@@@@ AV2018_SlaveID_Check[ 0x%x ] \n\n",_u8SlaveID));
}

MS_BOOL MDrv_Tuner_AV2018_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;
    MS_U8 index;

    AV2018_SlaveID_Check();

    for (index=0; index < 12; index++)
    {
        bRet&=AV2018_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2018_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2018_WriteReg(_u8SlaveID, TunerInitialSetting[0][12], TunerInitialSetting[1][12]);
    MsOS_DelayTask(100);
    for (index=0; index < 12; index++)
    {
        bRet&=AV2018_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2018_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2018_WriteReg(_u8SlaveID, TunerInitialSetting[0][12], TunerInitialSetting[1][12]);
    MsOS_DelayTask(50);
    return bRet;
}

MS_BOOL MDrv_Tuner_AV2018_SetFreq_S2(MS_U8 u8TunerIndex,MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bRet=TRUE;
    MS_BOOL bAutoScan=FALSE;
    MS_U32 u32FracN;
    MS_U32 BW;
    MS_U32 BF;
    MS_U8 u8Reg[7];
    MS_U8 u8RolloffFacter = 135;

    TUNER_DBG(("u32CenterFreq:%ld u32SymbolRate_Hz:%ld\n",u32CenterFreq,u32SymbolRate_Hz));
    if((u32CenterFreq > MAX_INPUT_FREQ) || (u32CenterFreq < MIN_INPUT_FREQ))
        return FALSE;

    if (u32SymbolRate_Hz == 0 || u32SymbolRate_Hz == 45000) //auto-scan mode
    {
	    bAutoScan = TRUE;
    }
    //MsOS_DelayTask(50);
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
   	    //BW = u32SymbolRate_Hz*135/200;
   	    switch(sCurRolloff)
        {
            case E_SAT_RO_35:
                u8RolloffFacter = 135;
                break;
            case E_SAT_RO_25:
                u8RolloffFacter = 125;
                break;
            case E_SAT_RO_20:
                u8RolloffFacter = 120;
                break;
        }
        //u8RolloffFacter = 135;
   	    BW = u32SymbolRate_Hz*u8RolloffFacter/200;
   	    // monsen 20080726, BB request low IF when sym < 6.5MHz
   	    // add 6M when Rs<6.5M,
   	    if(u32SymbolRate_Hz<6500)
   	    {
   	    	BW = BW + 6000;
   	    }
   	    	// add 2M for LNB frequency shifting
   	    BW = BW + 2000;
   	    	// add 8% margin since fc is not very accurate
   	    BW = BW*108/100;
            //BW = BW*150/100;
            //printf("AV2012 Tuner ................ BW = %d......\n", BW);
   	    	// Bandwidth can be tuned from 4M to 40M
   	    if( BW< 4000)
   	    {
   	    	BW = 4000;
   	    }
   	    //if( BW> 42300) // 42300 can set BF to 0xFF
   	    //{
   	    //	BW = 42300;
   	    //}
   	    if( BW> 40000)
   	    {
   	    	BW = 40000;
   	    }

   	    BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
   	    u8Reg[5] = (MS_U8)BF;
   	    //u8Reg[5] = 0x46;
    }
    // Sequence 4
    // Send Reg0 ->Reg4
    MsOS_DelayTask(5);
    bRet&=AV2018_WriteReg(_u8SlaveID, 0x00, u8Reg[0]);
    bRet&=AV2018_WriteReg(_u8SlaveID, 0x01, u8Reg[1]);
    bRet&=AV2018_WriteReg(_u8SlaveID, 0x02, u8Reg[2]);
    bRet&=AV2018_WriteReg(_u8SlaveID, 0x03, u8Reg[3]);
    MsOS_DelayTask(5);
    // Sequence 5
    // Send Reg5
    bRet&=AV2018_WriteReg(_u8SlaveID, 0x05, u8Reg[5]);
    MsOS_DelayTask(5);
    // Fine-tune Function Control
    //Tuner fine-tune gain function block. bit2.
    //not auto-scan case. enable block function. FT_block=1, FT_EN=1
    if (bAutoScan==FALSE)
    {
         u8Reg[6] = 0x06;
         bRet&=AV2018_WriteReg(_u8SlaveID, 0x25, u8Reg[6]);
         MsOS_DelayTask(5);
         //Disable RFLP at Lock Channel sequence after reg[37]
         //RFLP=OFF at Lock Channel sequence
         // RFLP can be Turned OFF, only at Receving mode.
	u8Reg[7] = 0xD6;
         bRet&=AV2018_WriteReg(_u8SlaveID, 0x0C, u8Reg[7]);
         //MsOS_DelayTask(5);
    }
    return bRet;
}
MS_BOOL MDrv_Tuner_AV2018_CheckLock(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;

    bRet&=AV2018_ReadReg(_u8SlaveID, 0x0B, &u8Data);
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

#if 0
void devDigitalTuner_Init()
{

}

void devTunerInit(void)
{
 //   MDrv_Tuner_Initial();

}


void devTunerSetFreqStep(FREQSTEP eFreqStep)
{

}

void devTunerSetPLLData(WORD wPLLData, RFBAND eBand)
{
}

void devDigitalTuner_SetFreq ( double Freq, RF_CHANNEL_BANDWIDTH eBandWidth)
{
}
#endif

void MDrv_Tuner_SetRolloff(MS_U8 u8Rolloff)
{
    sCurRolloff = u8Rolloff;
}

MS_BOOL AV2018_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE;
    switch(fuction_type)
    {
         case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _DigiTuner_Decide_LNB_LO(SAT_PARAM);
            break;
         default:
            break;
    }
    return bret;
}

#define AV2018_CHIP_ID_0 0x21
#define AV2018_CHIP_ID_1 0x15
MS_BOOL MDrv_Tuner_AV2018_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 regData = 0;
    MS_U8 regData2 = 0;
    MS_U8 regData3 = 0;
    MS_U8 i=0;

    AV2018_SlaveID_Check();

    for (;i<5;i++)
    {
        if(!AV2018_WriteReg(_u8SlaveID, 0x0C, 0xF6))
            continue;
        if(!AV2018_WriteReg(_u8SlaveID, 0x1C, 0x00))
            continue;
        if(!AV2018_ReadReg(_u8SlaveID, 0x0, &regData))
            continue;
        if(!AV2018_ReadReg(_u8SlaveID, 0x33, &regData2))
            continue;
        if(!AV2018_ReadReg(_u8SlaveID, 0x34, &regData3))
            continue;
        TUNER_DBG(("[av2018] read id =0x%x reg0x33=0x%x reg0x34=0x%x\n",regData,regData2,regData3));
        
        if ( (regData2 == AV2018_CHIP_ID_0) && (regData3 == AV2018_CHIP_ID_1) )
        {
            if(NULL != pu32channel_cnt)
                *(pu32channel_cnt) += 1;
            return TRUE;
        }
    }
    
    return FALSE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_AV2018) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =      
{                                                                       
    .name               = "TUNER_AV2018",          // demod system name
    .data               = TUNER_AV2018,            // private data value
    .Init               = MDrv_Tuner_AV2018_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_AV2018_SetFreq_S2,
    .GetLock            = MDrv_Tuner_AV2018_CheckLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_AV2018_CheckExist,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .Extension_Function = AV2018_Extension_Function
};


#endif
