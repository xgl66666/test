/*****************************************************************************
*    Airoha DVB-S AV2035 Tuner sample code (Mulitple API Version)
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

#if IF_THIS_TUNER_INUSE(TUNER_AV2035)

#include "drvIIC.h"
#include "drvTuner_AV2035.h"
#include "drvTunerNull.h"
#include "drvDTC.h"


TunerPara AV2035_Para;

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
TUNER_ErrorCode AV2035_Tuner_Register_Initial (MS_U8 u8TunerIndex)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    pTunerPara pAVtuner = &AV2035_Para;

    UINT8 reg[110];

	reg[0]= 0x50; //initial frequency 2150MHz   
	reg[1]= 0xA1; //initial frequency 2150MHz   
	reg[2]= 0x2F; //initial frequency 2150MHz     
	reg[3]= 0x50; //initial frequency 2150MHz   
	reg[4]= 0x1F;
	reg[5]= 0xa3;
	reg[6]= 0xfd;
	reg[7]= 0x58;
	reg[8]= 0x0e;
	reg[9]= 0x82;
	reg[10]= 0x88;
	reg[11]= 0xb4;	
	reg[12]= (0x16)|(pAVtuner->XO_EN <<7)|(pAVtuner->RFLP_EN <<6)|(pAVtuner->PD_EN <<5);
	reg[13]= 0x40;
	reg[14]= 0x52;
	reg[15]= 0xea;
	reg[16]= 0x66;
	reg[17]= 0x80;
	reg[18]= 0x40;
	reg[19]= 0x1a;
	reg[20]= 0x6a;
	reg[21]= 0x56;
	reg[22]= 0x91;
	reg[23]= 0x27;
	reg[24]= 0x8d;
	reg[25]= 0xb0;
	reg[26]= 0x21;
	reg[27]= 0x10;
	reg[28]= 0x9f;
	reg[29]= 0x13;
	reg[30]= 0xf9;
	reg[31]= 0x3d;
	reg[32]= 0x44;
	reg[33]= 0x00;
	reg[34]= 0x00;
	reg[35]= 0xe0;
	reg[36]= 0x00;	
	reg[37]= 0x00;	
	reg[38]= 0xf4;
	reg[39]= 0x2c;
	reg[40]= 0x5c;
	reg[41]= 0xcc;
	reg[42]= 0xff;
	reg[43]= 0x1a;
	reg[44]= 0x3c;
	reg[45]= 0xac;
	reg[46]= 0x00;
	reg[47]= 0x30;
	reg[48]= 0x19;
	reg[49]= 0x5b;
	reg[50]= 0x3a;
	reg[51]= 0x20;
	reg[52]= 0x15;
	reg[53]= 0x00;
	reg[54]= 0x00;
	reg[55]= 0x00;
	reg[56]= 0x00;
	reg[57]= 0x00;
	reg[58]= 0x00;
	reg[59]= 0x00;
	reg[60]= 0x55;
	reg[61]= 0x88;
	reg[62]= 0x10;
	reg[63]= 0x22;
	reg[64]= 0x34;
	reg[65]= 0x55;
	reg[66]= 0x01;
	reg[67]= 0x00;
	reg[68]= 0x00;
	reg[69]= 0x00;
	reg[70]= 0x51;
	reg[71]= 0xfd;
	reg[72]= 0x3f;
	reg[73]= 0x07;
	reg[74]= 0x3f;
	reg[75]= 0x7e;
	reg[76]= 0x08;
	reg[77]= 0x42;
	reg[78]= 0x11;
	reg[79]= 0x00;
	reg[80]= 0x40;
	reg[81]= 0x80;
	reg[82]= 0x00;
	reg[83]= 0x00;
	reg[84]= 0x36;
	reg[85]= 0x2e;
	reg[86]= 0x36;
	reg[87]= 0x2e;
	reg[88]= 0x2f;
	reg[89]= 0x27;
	reg[90]= 0x26;
	reg[91]= 0x27;
	reg[92]= 0x3f;
	reg[93]= 0x3d;
	reg[94]= 0x24;
	reg[95]= 0x65;
	reg[96]= 0x3d;
	reg[97]= 0x04;
	reg[98]= 0x40;
	reg[99]= 0x0a;
	reg[100]= 0x00;
	reg[101]= 0x00;
	reg[102]= 0xfa;
	reg[103]= 0xff;
	reg[104]= 0x00;
	reg[105]= 0x3f;
	reg[106]= 0x07;

	/* Sequence 1*/
	/* Send Reg0 ->Reg11*/
	if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,0,reg,12))
    {
        return I2C_Error;
	}
    
	/* Sequence 2*/
	/* Send Reg13 ->Reg106*/	
	if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,13,reg+13,94))
    {
        return I2C_Error;
	}
  	
    /* Sequence 3*/
	/* Send reg12*/
	if (!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,12,reg+12,1))
    {
        return I2C_Error;
    }

	/* Time delay ms*/			
	AV2035_Time_DELAY_MS(100);		
	/*Reinitial again*/
	{
		/* Sequence 1*/
		/* Send Reg0 ->Reg11*/
		if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,0,reg,12))
        {
            return I2C_Error;
        }
		
		/* Sequence 2*/
		/* Send Reg13 ->Reg106*/	
		if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,13,reg+13,94))
        {
            return I2C_Error;
        }

 		/* Sequence 3*/
 		/* Send reg12*/
 		if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,12,reg+12,1))
        {
            return I2C_Error;
        }
	}		
 	/* Time delay ms*/
	AV2035_Time_DELAY_MS(5);
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
TUNER_ErrorCode AV2035_Tuner_Set_Channel_Frequency (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 channel_freq_mhz)
{
    TUNER_ErrorCode result = Tuner_No_Error;
	UINT8 reg0;
	UINT8 reg1;
	UINT8 reg2;
	UINT8 reg3;
	UINT8 reg16;
	UINT8 reg30;
	UINT32 fracN;	
	UINT32 freq_khz;
	UINT32 XO_khz;

	XO_khz = pAVtuner_t->crystal_khz;
    freq_khz = channel_freq_mhz*1000; 
	
	if((freq_khz%XO_khz) > (XO_khz-501)) // according to XO application , round off the channel setting
    {
	 	freq_khz /= 1000;
		freq_khz *= 1000;
		freq_khz += 1000; 
    }
	else if((freq_khz%XO_khz) < 500)
    {
	     freq_khz /= 1000;
		 freq_khz *= 1000;
	}
	else
	{}

    if( XO_khz != 27000 )
    {		
		if(freq_khz > 1167750) 
           reg16 = 0x56; // VCO Divider=/2
        else
           reg16 = 0x46; // VCO Divider=/4               	       
	  	    
	    /* Send Reg16*/
        if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,16,&reg16,1))
        {
            return I2C_Error;
        }
    }   
	
	fracN = (freq_khz + XO_khz/2)/XO_khz;
	if(fracN > 0xff)
  	   fracN = 0xff;
    reg0=(UINT8) (fracN & 0xff);
  	fracN = ((freq_khz/100)<<17)/(XO_khz/100);
  	fracN = fracN & 0x1ffff;
  	reg1=(UINT8) ((fracN>>9)&0xff);
  	reg2=(UINT8) ((fracN>>1)&0xff);	
	reg3= (0x50)|(fracN<<7)|(pAVtuner_t->IQmode<<2);

    // --------------------------set R3~R0-------------------------
	if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,3,&reg3,1))// Send Reg3
    {
        return I2C_Error;
    }
	
    if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,2,&reg2,1))// Send Reg2
    {
        return I2C_Error;
    }
	
    if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,1,&reg1,1))// Send Reg1
    {
        return I2C_Error;
    }
	
    if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,0,&reg0,1))// Send Reg0
    {
        return I2C_Error;
    }
	
	AV2035_Time_DELAY_MS(4); // Time delay ms
	
	if( (freq_khz%XO_khz)==0)
	{
		reg30 = 0xe9;
	}
	else
	{
		reg30 = 0xf9;
	}
	if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,30,&reg30,1))
    {
        return I2C_Error;
    }

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
TUNER_ErrorCode AV2035_Tuner_Set_Filter_Bandwith (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 filter_BW_khz)
{
    TUNER_ErrorCode result = Tuner_No_Error;
	UINT32 ctrl_clk_khz;
	UINT32 BW_CLK_khz;	
	UINT32 XO_khz;
	UINT32 filter_khz;
	UINT32 BF;
	UINT8 reg5;
	UINT8 reg38;

	XO_khz = pAVtuner_t->crystal_khz;
	filter_khz = filter_BW_khz;
	if( XO_khz < 25000 )       
        ctrl_clk_khz = XO_khz;
	else 
	    ctrl_clk_khz = XO_khz/2;	
	/* Filter Bandwidth reference CLK calculation */
	BW_CLK_khz = (ctrl_clk_khz +64/2)/64;
	/* BF = filter_BW (KHz) * 1 / BW_CLK (KHz)*/	
    BF = (filter_khz*100 + 100*BW_CLK_khz/2)/(100*BW_CLK_khz); 
	if(BF > 0xff)
  	   BF = 0xff;
	reg5 = (UINT8)BF;
    /* Sequence 5*/
    /* Send Reg5*/
    if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,5, &reg5, 1))
    {
        return I2C_Error;
    }
    /* Time delay ms*/
    AV2035_Time_DELAY_MS(4);

	/* Gain mode setting after Filter BW setting when bline scan is needed.*/	
	if (pAVtuner_t->blind_scan == 1)
	{
		reg38=0x74; //WRSSI off
	    if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,38, &reg38, 1))
        {
            return I2C_Error;
        }
	}
	else
	{
		reg38=0xF4; //WRSSI on
		if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,38, &reg38, 1))
        {
            return I2C_Error;
        }
	}
	
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
TUNER_ErrorCode AV2035_Tuner_Set_Fine_Tune (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t)
{
    /*
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
    // Time delay ms
    Time_DELAY_MS(pre_delay);
    // Send Fine-tune Function Control
    result = Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,37, &reg37, 1);
    // Time delay ms
    Time_DELAY_MS(post_delay);
    return result;
    */
    printf("[%s][%d] Error!!! Sync from AV2028, no implementation now\n", __FUNCTION__, __LINE__);
    return Tuner_Error;
}

/*********************************************************************************
* Loopthrough setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description:
* 1. reg[12]_D7 is xocore_ena. Enable/disable XO section
* 2. reg[12]_D6 is RFLP_ena.   Enable/disable Loop-through section
**********************************************************************************/
TUNER_ErrorCode AV2035_Tuner_Set_RFLP (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t)
{
    /*
    TUNER_ErrorCode result = Tuner_No_Error;
    UINT8 reg12;

    reg12 = (0x16)|( pAVtuner_t->XO_EN <<7)|(pAVtuner_t->RFLP_EN <<6);

    result = Tuner_I2C_Write(u8TunerIndex, pAVtuner_t,12, &reg12, 1);
    //Time delay ms
    Time_DELAY_MS(5);
    return result;
    */
    printf("[%s][%d] Error!!! Sync from AV2028, no implementation now\n", __FUNCTION__, __LINE__);
    return Tuner_Error;
}

/*********************************************************************************
* Loopthrough and power-down setting
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition 
* Description:      
* 1. reg[12]_D7 is xocore_ena. Enable/disable XO section
* 2. reg[12]_D6 is RFLP_ena.   Enable/disable Loop-through section
* 3. reg[12]_D5 is PD_soft.	   Power ON/OFF Receiver,Synthesizer,VCO section
* 4. For tuner model name =1, hardware Power-down is controled by Hardware Pin13, which turn off Receiver,Synthesizer,VCO,XO,Control section.
*	 For tuner model name =1, make sure Pin13 is at low when sending registers.
**********************************************************************************/
TUNER_ErrorCode AV2035_Tuner_Set_RFLP_PD (UINT8 u8TunerIndex, pTunerPara pAVtuner)
{
	TUNER_ErrorCode result = Tuner_No_Error;
	UINT8 reg12;
	UINT8 reg38;
	UINT8 RFLP_EN_1=(pAVtuner->RFLP_EN );
	UINT8 PD_EN_1=(pAVtuner->PD_EN );

	reg12 = (0x16)|( pAVtuner->XO_EN <<7)|(pAVtuner->RFLP_EN <<6)|(pAVtuner->PD_EN <<5);

	if(RFLP_EN_1==1 && PD_EN_1==1)
	{
		reg38=0x74; //WRSSI off
	    if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,38, &reg38, 1))
        {
            return I2C_Error;
        }
	}
	else{}

	if(!AV2035_WriteReg(u8TunerIndex, pAVtuner->I2C_ADDR,12, &reg12, 1))
    {
        return I2C_Error;
    }
   	/* Time delay ms*/
	AV2035_Time_DELAY_MS(5);
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
TUNER_ErrorCode AV2035_Tuner_Set_RXout_PGA (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t)
{   
    TUNER_ErrorCode result = Tuner_No_Error;	
	UINT8 reg8;

	reg8 = (0x04)|(pAVtuner_t->PGA_Current);      	

	if(!AV2035_WriteReg(u8TunerIndex, pAVtuner_t->I2C_ADDR,8, &reg8, 1))
    {
        return I2C_Error;
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
TUNER_ErrorCode AV2035_Time_DELAY_MS (UINT32 ms)
{
    TUNER_ErrorCode result = Tuner_No_Error;
    MsOS_DelayTask(ms);
   // printf("Time dalay %02d ms\n",ms);
    return result;
}


/*********************************************************************************
* Tuner IIC interface writting function
* Paramter1:    MS_U8      : tuner index
* Paramter2:    pTunerPara : pointer of Tuner parameter structure
* Paramter3:    UINT8      : register start address
* Paramter4:    UINT8*     : pointer of sending register data buffer
* Paramter5:    UINT8      : the length of sending register data
* Return value: Boolean    : TRUE: write success; FALSE: write fail
* Description:
* 1. User define
**********************************************************************************/
MS_BOOL AV2035_WriteReg (MS_U8 u8TunerIndex, MS_U8 u8SlaveID, UINT8 reg_start, UINT8* buff, UINT8 length)
{
    UINT8 queue[100]={0};
    UINT8 i;
	MS_IIC_PORT ePort;
    ePort = getI2CPort(u8TunerIndex);

    queue[0]=reg_start;
    for (i=0;i<length;i++)
    {
        queue[i+1]= buff[i];
    }

    if( MDrv_IIC_WriteBytes(ePort, u8SlaveID, 0, 0, length+1, queue) == true )
    {
        return TRUE;
    }
    else
    {
        TUNER_ERR(("MDrv_IIC_WriteBytes fail \n"));
        return FALSE;
    }

    return FALSE;
}




/*********************************************************************************
* Tuner IIC interface reading function
* Paramter1:    MS_U8      : tuner index
* Paramter2:    pTunerPara : pointer of Tuner parameter structure
* Paramter3:    UINT8      : register start address
* Paramter4:    UINT8*     : pointer of reading register data buffer
* Return value: Boolean    : TRUE: write success; FALSE: write fail
* Description:
* 1. User define
**********************************************************************************/
MS_BOOL AV2035_ReadReg(MS_U8 u8TunerIndex, MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data)
{
    TUNER_DBG(("[%s][%d]\n", __FUNCTION__, __LINE__));
    MS_BOOL bRet=TRUE;
	MS_IIC_PORT ePort;
    ePort = getI2CPort(u8TunerIndex);
    //bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, &u8Addr, 1);
	bRet&=MDrv_IIC_WriteBytes(ePort, u8SlaveID, 0, 0, 1, &u8Addr);
    //bRet&=MDrv_IIC_Read(u8SlaveID, 0, 0, u8Data, 1);
	bRet&=MDrv_IIC_ReadBytes(ePort, u8SlaveID, 0, 0, 1, u8Data);
    return bRet;
}

/*******************************************************************
* Tuner parameter initialization:
* Paramter1:    pTunerPara : pointer of Tuner parameter structure
* Return value: TUNER_ErrorCode : error code of definition
* Description: Initialize all the parameters in the data structure of pAVtuner
********************************************************************/

MS_BOOL MDrv_Tuner_AV2035_Initial (MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    pTunerPara pAVtuner = &AV2035_Para;

	pAVtuner->crystal_khz   = 27000;				// (Unit: KHz)Tuner_crystal
	
	pAVtuner->I2C_ADDR      = ADDR1_H_ADDR0_H;	// Tuner I2C address at write mode 
	pAVtuner->IQmode		= Differential;     // IQ_MODE
	pAVtuner->PGA_Current   = PGA_1_5mA ;		// PGA_DRIVE_CURRENT
	pAVtuner->XO_Current    = HIGH;             // XO_DRIVE_CURRENT	
	pAVtuner->XO_EN		    = XO_ON;			// XO_ENABLE Setting
    pAVtuner->RFLP_EN       = RFLP_OFF;			// RFLP_ENABLE Setting
	pAVtuner->PD_EN         = Wake_Up;			// SOFTWARE_POWERDOWN   
	pAVtuner->blind_scan    = 0;					// blind_scan		
    
    if (AV2035_Tuner_Register_Initial(u8TunerIndex) == Tuner_Error )
    {
        TUNER_ERR(("Tuner_Register_Initial error \n"));
        return FALSE;
    }
    
	return TRUE;
}

MS_BOOL MDrv_Tuner_AV2035_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    pTunerPara pAVtuner = &AV2035_Para;
    
    TUNER_DBG(("u16CenterFreq:%"DTC_MS_U32_d" u32SymbolRate_Hz:%"DTC_MS_U32_d"\n",u32CenterFreq,u32SymbolRate_Hz));
    if((u32CenterFreq > MAX_INPUT_FREQ) || (u32CenterFreq < MIN_INPUT_FREQ))
    {
        return FALSE;
    }
    
    if (AV2035_Tuner_Set_Channel_Frequency(u8TunerIndex, pAVtuner,u32CenterFreq) == Tuner_Error )
    {
        TUNER_ERR(("Tuner_Set_Channel_Frequency error \n"));
        return FALSE;
    }

    if (AV2035_Tuner_Set_Filter_Bandwith(u8TunerIndex, pAVtuner,u32SymbolRate_Hz) == Tuner_Error )
    {
        TUNER_ERR(("Tuner_Set_Filter_Bandwith error \n"));
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_Tuner_AV2035_CheckLock(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;

    bRet&=AV2035_ReadReg(u8TunerIndex, AV2035_Para.I2C_ADDR, 0x0B, &u8Data);
    
    TUNER_DBG(("Tuner Status 0x%x\n", u8Data));
    
    if (bRet==FALSE)
    {
        return FALSE;
    }
    else
    {
        //check if Filter bandwidth lock[1], Channel lock[0] locked
        if ((u8Data&0x03)!=0x03)
        {
            //Filter bandwidth or Channel not locked 
            return FALSE;
        }
    }
    
    return TRUE;
}

MS_BOOL MDrv_Tuner_AV2035_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 regData = 0;
    MS_U8 regData2 = 0;
    MS_U8 regData3 = 0;
    MS_U8 writeregData = 0;
    MS_U8 i=0;
    
    for (;i<5;i++)
    {
        writeregData = 0xF6;
        if(!AV2035_WriteReg(u8TunerIndex, ADDR1_H_ADDR0_H, 0x0C, &writeregData, 1))
        //if(!AV2035_WriteReg1(u8TunerIndex, ADDR1_H_ADDR0_H, 0x0C, writeregData))
        {
            TUNER_DBG(("[av2035] AV2035_WriteReg 0x0C, 0xF6 error\n"));
            continue;
        }
        writeregData = 0x00;
        if(!AV2035_WriteReg(u8TunerIndex, ADDR1_H_ADDR0_H, 0x1C, &writeregData, 1))
        //if(!AV2035_WriteReg1(u8TunerIndex, ADDR1_H_ADDR0_H, 0x1C, writeregData))
        {
            TUNER_DBG(("[av2035] AV2035_WriteReg 0x1C, 0x00 error\n"));
            continue;
        }
        if(!AV2035_ReadReg(u8TunerIndex,ADDR1_H_ADDR0_H, 0x1C, &regData))
        {
            TUNER_DBG(("[av2035] AV2035_ReadReg regData error\n"));
            continue;   
        }
        if(!AV2035_ReadReg(u8TunerIndex,ADDR1_H_ADDR0_H, 0x33, &regData2))
        {
            TUNER_DBG(("[av2035] AV2035_ReadReg regData2 error\n"));
            continue;   
        }
        if(!AV2035_ReadReg(u8TunerIndex,ADDR1_H_ADDR0_H, 0x34, &regData3))
        {
            TUNER_DBG(("[av2035] AV2035_ReadReg regData3 error\n"));
            continue;
        }
        TUNER_DBG(("[av2035] read id =0x%x reg0x33=0x%x reg0x34=0x%x\n",regData,regData2,regData3));
        
        if ( regData == 0x7f )
        {
            if(NULL != pu32channel_cnt)
            {
                *(pu32channel_cnt) += 1;
            }
            return TRUE;
        }
    }
    
    return FALSE;
}

MS_BOOL AV2035_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
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
             bret &= AV2035_ReadReg(u8TunerIndex,AV2035_Para.I2C_ADDR, 0x0C, &regData);
            if(FALSE == *(MS_BOOL *)data)   //power off
            {
               regData |= (0x1<<5);
               bret &= AV2035_WriteReg(u8TunerIndex,AV2035_Para.I2C_ADDR, 0x0C, &regData,1);
            }
            else
            {
               if((regData & (0x1<<5)) >>5)
               {
                  bret &= AV2035_Tuner_Register_Initial(u8TunerIndex);
               }
            }
            break;
            
         case TUNER_EXT_FUNC_LOOP_THROUGH:
             bret &= AV2035_ReadReg(u8TunerIndex,AV2035_Para.I2C_ADDR, 0x0C, &regData);
            if(FALSE == *(MS_BOOL *)data)   //LT off
            {
               regData &= (~(0x1<<6));
            }
            else
            {
               regData |= (0x1<<6);
            }

            bret &= AV2035_WriteReg(u8TunerIndex,AV2035_Para.I2C_ADDR, 0x0C, &regData, 1);
            break;

         default:
            break;
    }
    return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_AV2035) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =      
{                                                                       
    .name               = "TUNER_AV2035",          // demod system name
    .data               = TUNER_AV2035,            // private data value
    .Init               = MDrv_Tuner_AV2035_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_AV2035_SetFreq_S2,
    .GetLock            = MDrv_Tuner_AV2035_CheckLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_AV2035_CheckExist,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .Extension_Function = AV2035_Extension_Function
};

#endif
