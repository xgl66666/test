/*****************************************************************************
*    Airoha DVB-S AV2028 Tuner sample code (Mulitple API Version)
*
*    History:
*          Date         Version    Reason
*
*      ============    =========    =================
*    1.May.25.2011     Ver 0.1    Separate different application interfaces
*****************************************************************************/

//#include "AVhost.h"
#include "MsCommon.h"
#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_AV2028)

#include "drvIIC.h"
#include "drvTuner_AV2028.h"
#include "drvTunerNull.h"
#include "drvDTC.h"


TunerPara AV2028_Para;


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


/*******************************************************************************
* Tuner Power-ON registers initialization
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description: Initialize all registers of the Tuner
********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_Register_Initial (MS_U8 u8TunerIndex)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT8 reg[50];
    pTunerPara pAVtuner = &AV2028_Para;

    reg[0]= 0x38;
    reg[1]= 0x00;
    reg[2]= 0x00;
    reg[3]= 0x50;
    reg[4]= 0x1f;
    reg[5]= 0xa3;
    reg[6]= 0xfd;
    reg[7]= 0x58;
    reg[8]= (0x04)|(pAVtuner->PGA_Gain <<3)|(pAVtuner->PGA_Current);
    reg[9]= 0x82;
    reg[10]= 0x88;
    reg[11]= 0xb4;
    reg[12]= (0x16)|(pAVtuner->XO_EN <<7)|(pAVtuner->RFLP_EN <<6);
    reg[13]= 0x40;
    reg[14]= 0x5b;
    reg[15]= 0x6a;
    reg[16]= 0x66;
    reg[17]= 0x40;
    reg[18]= 0x80;
    reg[19]= 0x2b;
    reg[20]= 0x6a;
    reg[21]= 0x50;
    reg[22]= 0x91;
    reg[23]= 0x27;
    reg[24]= 0x8f;
    reg[25]= 0xcc;
    reg[26]= 0x21;
    reg[27]= 0x10;
    reg[28]= 0x80;
    reg[29]= 0x0e;
    reg[30]= 0xf5;
    reg[31]= 0x7f;
    reg[32]= 0x4d;
    reg[33]= 0xbf;
    reg[34]= 0xe0;
    reg[35]= 0xe0;
    reg[36]= 0x2e;
    reg[37]= (UINT8)(pAVtuner->FT);
    reg[38]= 0xab;
    reg[39]= 0x97;
    reg[40]= 0xc5;
    reg[41]= 0xa8;
    /* Sequence 1*/
    /* Send Reg0 ->Reg11*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,0,reg,12);
    if(result!=Tuner_No_Error){ return result; }

    /* Sequence 2*/
    /* Send Reg13 ->Reg24*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,13,reg+13,12);
    if(result!=Tuner_No_Error){ return result; }

      /* Send Reg25 ->Reg35*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,25,reg+25,11);
    if(result!=Tuner_No_Error){ return result; }

    /* Send Reg36 ->Reg41*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,36,reg+36,6);
    if(result!=Tuner_No_Error){ return result; }

    /* Sequence 3*/
    /* Send reg12*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,12,reg+12,1);
    if(result!=Tuner_No_Error){ return result; }

    /* Time delay ms*/
    AV2028_Time_DELAY_MS(100);
    /*Reinitial again*/
    {
        /* Sequence 1*/
        /* Send Reg0 ->Reg11*/
        result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,0,reg,12);
        if(result!=Tuner_No_Error){ return result; }

        /* Sequence 2*/
        /* Send Reg13 ->Reg24*/
        result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,13,reg+13,12);
        if(result!=Tuner_No_Error){ return result; }

        /* Send Reg25 ->Reg35*/
        result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,25,reg+25,11);
        if(result!=Tuner_No_Error){ return result; }

        /* Send Reg36 ->Reg41*/
        result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,36,reg+36,6);
        if(result!=Tuner_No_Error){ return result; }

         /* Sequence 3*/
         /* Send reg12*/
         result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner,12,reg+12,1);
    }
     /* Time delay ms*/
    AV2028_Time_DELAY_MS(5);
    return result;
}





/*********************************************************************************
* Channel frequency setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Paramter2:    UINT32 : channel frequency (unit: MHz)
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. Send reg3,2,1,0 to set tuner at the channel frequency.
*    reg0 is int<7:0>; reg1 is frac<16:9>; reg2 is frac<8:1>; reg3_D7 is frac<0>
**********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_Set_Channel_Frequency (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 channel_freq_mhz)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT8 reg0;
    UINT8 reg1;
    UINT8 reg2;
    UINT8 reg3;

    UINT32 fracN;
    UINT32 freq_khz;
    UINT32 XO_khz;

    freq_khz = channel_freq_mhz*1000;
    XO_khz = pAVtuner_t->crystal_khz;

    fracN = (freq_khz + XO_khz/2)/XO_khz;

    if(fracN > 0xff)
         fracN = 0xff;
    reg0=(UINT8) (fracN & 0xff);
      fracN = ((freq_khz/100)<<17)/(XO_khz/100);
      fracN = fracN & 0x1ffff;
      reg1=(UINT8) ((fracN>>9)&0xff);
      reg2=(UINT8) ((fracN>>1)&0xff);

      reg3= (0x50)|(fracN<<7);
    /* Sequence 4*/
    /* Send Reg3*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,3,&reg3,1);
    if(result!=Tuner_No_Error){ return result; }
    /* Send Reg2*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,2,&reg2,1);
    if(result!=Tuner_No_Error){ return result; }
    /* Send Reg1*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,1,&reg1,1);
    if(result!=Tuner_No_Error){ return result; }
    /* Send Reg0*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,0,&reg0,1);
    /* Time delay ms*/
    AV2028_Time_DELAY_MS(4);
    return result;
}

/*********************************************************************************
* ZIF low pass filter bandwidth setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Paramter2:    UINT32 : LPF bandwidth (unit: kHz)
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. Calculate the filter reference clk from XO. Transfer Filter BW setting to register5
* 2. Add Fine-tune function after Bandwidth setting.
**********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_Set_Filter_Bandwith (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 filter_BW_khz)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT32 ctrl_clk_khz;
    UINT32 BW_CLK_khz;
    UINT32 XO_khz;
    UINT32 filter_khz;
    UINT32 BF;
    UINT8 reg5;

    XO_khz = pAVtuner_t->crystal_khz;
    filter_khz = filter_BW_khz*135/200;
    filter_khz = filter_khz + 2000;
    filter_khz = filter_khz*108/100;
    ctrl_clk_khz = XO_khz/2;
    /* Filter Bandwidth reference CLK calculation */
    BW_CLK_khz = (ctrl_clk_khz +64/2)/64;
    /* BF = filter_BW (KHz) * 1.27 / BW_CLK (KHz)*/
    BF = (filter_khz*127 + 100*BW_CLK_khz/2)/(100*BW_CLK_khz);
    if(BF > 0xff)
         BF = 0xff;
    reg5 = (UINT8)BF;
    /* Sequence 5*/
    /* Send Reg5*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,5, &reg5, 1);
    if(result!=Tuner_No_Error){ return result; }
    /* Time delay ms*/
    AV2028_Time_DELAY_MS(4);

    /* Reset FT after Filter BW setting */
    if (pAVtuner_t->blind_scan == 1)
    {
        /* choose one for different blind scan mechanism */
        // pAVtuner->FT = FT_OFF;
        // pAVtuner->FT = FT_Hold;
        pAVtuner_t->FT = FT_ON;
    }
    else
    {
        pAVtuner_t->FT = FT_Delay_ON;
    }
    result = AV2028_Tuner_Set_Fine_Tune (u8TunerIndex, pAVtuner_t);
    if(result!=Tuner_No_Error){ return result; }

    return result;
}

/*********************************************************************************
* Fine-tune function setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. Fine-tune the gain for sensitivity and linearity.
*    reg[37]_D2 is FT_block,Tp=12ms. reg[37]_D1 is FT_EN. reg[37]_D0 is FT_hold.
* 2. FT_OFF      = 0x00  : FT_block=0, FT_EN=0, FT_hold=0. The Gain is at best sensitivity gain.
* 3. FT_ON       = 0x02  : FT_block=0, FT_EN=1, FT_hold=0. Turn on to fine-tuned gain continuously between sensitivity and linearity.
* 4. FT_Hold     = 0x03  : FT_block=0, FT_EN=1, FT_hold=1. Stop fine-tuning the gain and hold the current fine-tuned gain
* 5. FT_Delay_ON = 0x06     : FT_block=1, FT_EN=1, FT_hold=0. FT_block=1 trigger the FT hardware delay.
*    After the  delay time 12ms, Fine-tune Function is turned on. And,the chip hardware resets FT_block=1 to FT_block=0 internally.
*    Then, FT function is continuously fine-tuning the gain by the voltage level of pin RFAGC
* 6. Fint-tune function take a reference to RFAGC voltage level of Hardware Pin5.
*     When Fine-tune is enable, make sure the RFAGC do not have a sharp jump that cause a longer AGC settling time.
***********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_Set_Fine_Tune (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT32 pre_delay;
    UINT32 post_delay;
    UINT8 reg37;

    switch ( pAVtuner_t->FT )
    {
        case  FT_ON:{
                pre_delay  = 1;
                post_delay = 5;
                break;}
        case  FT_Hold:{
                pre_delay  = 10;
                post_delay = 1;
                break;}
        case  FT_Delay_ON:{
                pre_delay  = 1;
                post_delay = 15;
                break;}
        default:{ //OFF
                pre_delay  = 1;
                post_delay = 1;}
    }

    reg37 = pAVtuner_t->FT;
    /* Time delay ms*/
    AV2028_Time_DELAY_MS(pre_delay);
    /* Send Fine-tune Function Control*/
    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,37, &reg37, 1);
    /* Time delay ms*/
    AV2028_Time_DELAY_MS(post_delay);
    return result;
}

/*********************************************************************************
* Loopthrough setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. reg[12]_D7 is xocore_ena. Enable/disable XO section
* 2. reg[12]_D6 is RFLP_ena.   Enable/disable Loop-through section
**********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_Set_RFLP (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT8 reg12;

    reg12 = (0x16)|( pAVtuner_t->XO_EN <<7)|(pAVtuner_t->RFLP_EN <<6);

    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,12, &reg12, 1);
       /* Time delay ms*/
    AV2028_Time_DELAY_MS(5);
    return result;
}

/*********************************************************************************
* RX output baseband programmable gain amplifier setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. reg[8]_D6~D3 is gc. PGA gain setting
* 2. reg[8]_D1~0  is PGAout_cs. PGA output driving current setting
**********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_Set_RXout_PGA (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT8 reg8;

    reg8 = (0x04)|(pAVtuner_t->PGA_Gain <<3)|(pAVtuner_t->PGA_Current);

    result = AV2028_Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,8, &reg8, 1);
    return result;
}


/*********************************************************************************
* Tuner IIC interface writting function
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Paramter2:    UINT8      : register start address
* Paramter3:    UINT8*     : pointer of sending register data buffer
* Paramter4:    UINT8      : the length of sending register data
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. User define
**********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_I2C_Write (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT8 reg_start, UINT8* buff, UINT8 length)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT8 queue[16];
    UINT8 i;
	MS_IIC_PORT ePort;
    ePort = getI2CPort(u8TunerIndex);

    queue[0]=reg_start;
    for (i=0;i<length+1;i++)
    {
        queue[i+1]= buff[i];
    }

    /*
    printf("I2C write queue");
    for (i=0;i<length+1;i++)
        printf("_%02x_", queue[i]);
    printf("\n");
    */

    if( MDrv_IIC_WriteBytes(ePort, pAVtuner_t->I2C_ADDR, 0, 0, length+1, queue) == true )
    {
        return Tuner_No_Error;
    }
    else
    {
        TUNER_ERR(("MDrv_IIC_WriteBytes fail \n"));
        return Tuner_Error;
    }

    return result;
}

/*********************************************************************************
* Tuner IIC interface reading function
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Paramter2:    UINT8      : register start address
* Paramter3:    UINT8*     : pointer of reading register data buffer
* Paramter4:    UINT8      : the length of reading register data
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. User define
**********************************************************************************/
TUNER_ErrorCode AV2028_Tuner_I2C_Read (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT8 reg_start, UINT8* buff, UINT8 length)
{
    TUNER_ErrorCode result = Tuner_No_Error;

    UINT8 queue[16];
    UINT8 i;
	MS_IIC_PORT ePort;
    ePort = getI2CPort(u8TunerIndex);

    queue[0]=pAVtuner_t->I2C_ADDR; // I2C write address
    queue[1]=reg_start;            // register start address
    queue[2]=pAVtuner_t->I2C_ADDR | 0x01; // I2C read address


    if( MDrv_IIC_ReadBytes(ePort, pAVtuner_t->I2C_ADDR, 0, 0, length+2, queue+1) == FALSE )
    {
        TUNER_ERR(("MDrv_IIC_ReadBytes fail \n"));
        return Tuner_Error;
    }


    TUNER_DBG(("I2C read queue"));
    for (i=0;i<length+3;i++)
        TUNER_DBG(("_%02x_", queue[i]));
    TUNER_DBG(("\n"));

    for (i=0;i<length;i++)
    {
        buff[i] = queue[3+i];
    }

    return result;
}

/*********************************************************************************
* Time delay function
* Paramter1:    UINT32     : delay time. Unit is millisecond
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. User define
**********************************************************************************/
TUNER_ErrorCode AV2028_Time_DELAY_MS (UINT32 ms)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    MsOS_DelayTask(ms);
   // printf("Time dalay %02d ms\n",ms);
    return result;
}

MS_BOOL AV2028_ReadReg(MS_U8 u8TunerIndex, MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data)
{
    MS_BOOL bRet=TRUE;
	MS_IIC_PORT ePort;
    ePort = getI2CPort(u8TunerIndex);
    //bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, &u8Addr, 1);
	bRet&=MDrv_IIC_WriteBytes(ePort, u8SlaveID, 0, 0, 1, &u8Addr);
    //bRet&=MDrv_IIC_Read(u8SlaveID, 0, 0, u8Data, 1);
	bRet&=MDrv_IIC_ReadBytes(ePort, u8SlaveID, 0, 0, 1, u8Data);
    return bRet;
}

MS_BOOL AV2028_WriteReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Value[2];
    u8Value[0]=u8Addr;
    u8Value[1]=u8Data;
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, u8Value, 2);
    if (!bRet)
    {
        TUNER_ERR(("AV2028_WriteReg fail \n"));
    }
    return bRet;
}

/*******************************************************************
* Tuner parameter initialization:
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description: Initialize all the parameters in the data structure of pAVtuner
********************************************************************/

MS_BOOL MDrv_Tuner_AV2028_Initial (MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    //TUNER_ErrorCode result = Tuner_No_Error;
    pTunerPara pAVtuner = &AV2028_Para;
    pAVtuner->crystal_khz   = 27000;                // (default 27000 KHz)
    pAVtuner->I2C_ADDR      = ADDR1_H_ADDR0_H;    // Tuner I2C address at write mode
    pAVtuner->PGA_Gain      = PGA_1_5dB ;            // PGA_GAIN
    pAVtuner->PGA_Current   = PGA_1_5mA ;            // PGA_DRIVE_CURRENT
    pAVtuner->XO_Current    = HIGH;              // XO_DRIVE_CURRENT
    pAVtuner->XO_EN         = XO_ON;                // XO_ENABLE Setting
    pAVtuner->RFLP_EN       = RFLP_OFF;            // RFLP_ENABLE Setting

    pAVtuner->FT            = FT_ON;                // TUNER_Fine_Tune
    pAVtuner->blind_scan    = 0;                    // blind_scan
    TUNER_DBG(("%s, %d \n", __FUNCTION__,__LINE__));

    if (AV2028_Tuner_Register_Initial(u8TunerIndex) == Tuner_Error )
    {
        TUNER_ERR(("Tuner_Register_Initial error \n"));
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_Tuner_AV2028_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    pTunerPara pAVtuner = &AV2028_Para;
    TUNER_DBG(("u16CenterFreq:%"DTC_MS_U32_d" u32SymbolRate_Hz:%"DTC_MS_U32_d"\n",u32CenterFreq,u32SymbolRate_Hz));

    if((u32CenterFreq > MAX_INPUT_FREQ) || (u32CenterFreq < MIN_INPUT_FREQ))
        return FALSE;
    
    if (AV2028_Tuner_Set_Channel_Frequency(u8TunerIndex, pAVtuner,u32CenterFreq) == Tuner_Error )
    {
        TUNER_ERR(("Tuner_Set_Channel_Frequency error \n"));
        return FALSE;
    }

    if (AV2028_Tuner_Set_Filter_Bandwith(u8TunerIndex, pAVtuner,u32SymbolRate_Hz) == Tuner_Error )
    {
        TUNER_ERR(("Tuner_Set_Filter_Bandwith error \n"));
        return FALSE;
    }


    pAVtuner->RFLP_EN = RFLP_ON;
    AV2028_Tuner_Set_RFLP(u8TunerIndex, pAVtuner);

    return TRUE;
}

MS_BOOL MDrv_Tuner_AV2028_CheckLock(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;

    bRet&=AV2028_ReadReg(u8TunerIndex, AV2028_Para.I2C_ADDR, 0x0B, &u8Data);
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
    TUNER_DBG(("Tuner Status 0x%x\n", u8Data));
    return bRet;
}


#define AV2028_CHIP_ID_0 0x21
#define AV2028_CHIP_ID_1 0x15
MS_BOOL MDrv_Tuner_AV2028_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 regData = 0;
    MS_U8 regData2 = 0;
    MS_U8 regData3 = 0;
    MS_U8 i=0;

    for (;i<5;i++)
    {
        if(!AV2028_WriteReg(ADDR1_H_ADDR0_H, 0x0C, 0xF6))
            continue;
        if(!AV2028_WriteReg(ADDR1_H_ADDR0_H, 0x1C, 0x00))
            continue;
        if(!AV2028_ReadReg(u8TunerIndex,ADDR1_H_ADDR0_H, 0x0, &regData))
            continue;   
        if(!AV2028_ReadReg(u8TunerIndex,ADDR1_H_ADDR0_H, 0x33, &regData2))
            continue;   
        if(!AV2028_ReadReg(u8TunerIndex,ADDR1_H_ADDR0_H, 0x34, &regData3))
            continue;
        TUNER_DBG(("[av2028] read id =0x%x reg0x33=0x%x reg0x34=0x%x\n",regData,regData2,regData3));
        
        if ( (regData2 == AV2028_CHIP_ID_0) && (regData3 == AV2028_CHIP_ID_1) )
        {
            if(NULL != pu32channel_cnt)
                *(pu32channel_cnt) += 1;
            return TRUE;
        }
    }
    
    return FALSE;
}

MS_BOOL AV2028_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE;
    MS_U8 regData = 0;
    
    switch(fuction_type)
    {
         case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _DigiTuner_Decide_LNB_LO(SAT_PARAM);
            break;
            
         case TUNER_EXT_FUNC_POWER_ON_OFF:
             bret &= AV2028_ReadReg(u8TunerIndex,AV2028_Para.I2C_ADDR, 0x0C, &regData);
            if(FALSE == *(MS_BOOL *)data)   //power off
            {
               regData |= (0x1<<5);
               bret &= AV2028_WriteReg(AV2028_Para.I2C_ADDR, 0x0C, regData);
            }
            else
            {
               if((regData & (0x1<<5)) >>5)
               {
                  bret &= AV2028_Tuner_Register_Initial(u8TunerIndex);
               }
            }
            break;
            
         case TUNER_EXT_FUNC_LOOP_THROUGH:
             bret &= AV2028_ReadReg(u8TunerIndex,AV2028_Para.I2C_ADDR, 0x0C, &regData);
            if(FALSE == *(MS_BOOL *)data)   //LT off
            {
               regData &= (~(0x1<<6));
            }
            else
            {
               regData |= (0x1<<6);
            }

            bret &= AV2028_WriteReg(AV2028_Para.I2C_ADDR, 0x0C, regData);
            break;
         default:
            break;
    }
    return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_AV2028) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =      
{                                                                       
    .name               = "TUNER_AV2028",          // demod system name
    .data               = TUNER_AV2028,            // private data value
    .Init               = MDrv_Tuner_AV2028_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_AV2028_SetFreq_S2,
    .GetLock            = MDrv_Tuner_AV2028_CheckLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_AV2028_CheckExist,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .Extension_Function = AV2028_Extension_Function
};

#endif
