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
#include "drvTuner_STV6111.h"

#if IF_THIS_TUNER_INUSE(TUNER_STV6111) || IF_THIS_TUNER_INUSE(TUNER_STV6111)

SLAVE_ID_USAGE* pstSTV6111_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE STV6111_possible_slave_ID[5] =
{
    {0xC6, FALSE},
    {0xC4, FALSE},
    {0xC2, FALSE},
    {0xC0, FALSE},
    {0xFF, FALSE}
};

static MS_U8 STV6111_Initial_Value[11]=
{
	
   //RSTV6111_CTRL1, RSTV6111_CTRL2, RSTV6111_CTRL3, RSTV6111_CTRL4,RSTV6111_CTRL5,
   //RSTV6111_CTRL6, RSTV6111_CTRL7, RSTV6111_CTRL8, RSTV6111_CTRL9, RSTV6111_CTRL10,
   //RSTV6111_CTRL11,
   0x7c, 0x41, 0x8f, 0x02, 0xce, 0x54, 0x55, 0x45, 0x46, 0xbd, 0x11
};

static STV6111_Handle* pstSTV6111_Handle = NULL;
static MS_U8 u8max_dev_num = 0;
static MS_BOOL bUnderExtDMDTest = FALSE;
static MS_U8 u8STV6111_Cnt = 0;



static MS_BOOL _get_handle(MS_U8 u8TunerIndex, STV6111_Handle** ppHandle)
{
    if((pstSTV6111_Handle == NULL) || (u8TunerIndex > u8max_dev_num))
    {
        TUNER_ERR(("[STV6111] Get Handle FAIL\n"));
        return FALSE;
    }
	
    *ppHandle = pstSTV6111_Handle + u8TunerIndex;
    return TRUE;
}

static MS_BOOL _getI2CPort(MS_U8 u8TunerIndex, MS_IIC_PORT* pePort)
{
    STV6111_Handle* pHandle = NULL;

     if(!_get_handle(u8TunerIndex, &pHandle))
        return FALSE;

     *pePort = pHandle->Init_Param.stTUNCon.eI2C_PORT;
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

static MS_BOOL _stv6111_variables_alloc(void)
{
    MS_BOOL bRet = TRUE;

    bRet &= _variable_alloc((void*)&pstSTV6111_Handle, sizeof(STV6111_Handle));
    return bRet;
}

static MS_BOOL _stv6111_variables_free(void)
{
    _variable_free((void*)&pstSTV6111_Handle);
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
         s32Offset = pSATParam->u16LoLOF+ STV6111_MAX_INPUT_FREQ - pSATParam->u16HiLOF - STV6111_MIN_INPUT_FREQ;
         if(s32Offset < 0)
              s32Offset = 0;
          else
             s32Offset /= 2;

         if( u32MidFreq <= (STV6111_MAX_INPUT_FREQ-s32Offset))
              *pSATParam->pbIsHiLOF = FALSE;
          else
              *pSATParam->pbIsHiLOF = TRUE;     
     }
     return TRUE;
}


static MS_BOOL STV6111_I2C_Write_Len(MS_U8 u8TunerIndex, STV6111_I2C_LEN_TYPE *pI2C_Info)
{
    MS_U16 u16size  = pI2C_Info->Len;
    MS_U8 *paddr = &(pI2C_Info->RegAddr);
    MS_U8 *pu8data = pI2C_Info->Data;
    MS_IIC_PORT ePort;
    STV6111_Handle* pstSTV6111= NULL;

    if(_get_handle(u8TunerIndex, &pstSTV6111) == FALSE)
        return FALSE;

    _getI2CPort(u8TunerIndex, &ePort);
    if (FALSE == MDrv_IIC_WriteBytes(ePort, pstSTV6111->I2C_SalveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("devCOFDM_PassThroughIIC_WRITEBytes Error \n"));
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL STV6111_I2C_Read_Len(MS_U8 u8TunerIndex,STV6111_I2C_LEN_TYPE *pI2C_Info)
{
    MS_U8 *paddr = &(pI2C_Info->RegAddr);
    MS_U16 u16size  = pI2C_Info->Len;
	MS_U8 *pu8data = &(pI2C_Info->Data[0]);
    MS_IIC_PORT ePort;
    STV6111_Handle* pstSTV6111= NULL;

    if(_get_handle(u8TunerIndex, &pstSTV6111) == FALSE)
        return FALSE;

    _getI2CPort(u8TunerIndex, &ePort);
    if (FALSE == MDrv_IIC_ReadWithMode(ePort, pstSTV6111->I2C_SalveID, 1, paddr, u16size, pu8data, E_MS_IIC_READ_MODE_DIRECTION_CHANGE_STOP_START))
    {
        TUNER_ERR(("IIC Read Len error, Slave ID = 0x%x, port = %x\n", pstSTV6111->I2C_SalveID, ePort));
        return FALSE;
    }

    return TRUE;
}

#if 0
static MS_BOOL STV6111_RegDump(MS_U8 u8TunerIndex)
{
    MS_U8 i;
	STV6111_I2C_LEN_TYPE I2C_Info;
	MS_BOOL bret = TRUE;

	I2C_Info.Len = 11;
	I2C_Info.RegAddr = RSTV6111_CTRL1;
    bret = STV6111_I2C_Read_Len(u8TunerIndex, &I2C_Info);
	for(i=0; i<I2C_Info.Len ;i++)
    {
      printf("Addr 0x%02x, Value = 0x%02x\n", i, I2C_Info.Data[i]);
	}
	
    return bret;
}
#endif
static MS_BOOL STV6111_SetFieldImage(MS_U8 u8TunerIndex, unsigned int value, MS_U8  u8Value)
{
	MS_U8 field = 0;
	MS_U8 position;
	STV6111_I2C_LEN_TYPE I2C_Info;
	MS_BOOL bret = TRUE;
	
	I2C_Info.RegAddr = (MS_U8)(value>>16);
	I2C_Info.Len = 1;
	field = (MS_U8)value;
	bret &= STV6111_I2C_Read_Len(u8TunerIndex, &I2C_Info);
	
	position = 0;
	while(((field>>position)&0x01)==0x00)
	{
		position++;
	}

	I2C_Info.Data[0] &= ~ (field);

	I2C_Info.Data[0] = (I2C_Info.Data[0]|(u8Value<<position));
	
	bret &= STV6111_I2C_Write_Len(u8TunerIndex, &I2C_Info);

	return bret;
}

static MS_BOOL STV6111_GetFieldImage(MS_U8 u8TunerIndex, unsigned int value, MS_U8* pu8Value)
{
	MS_U8 field = 0;
	MS_U8 position;
    STV6111_I2C_LEN_TYPE I2C_Info;
	MS_BOOL bret = TRUE;
	
	I2C_Info.RegAddr  = (MS_U8)(value>>16);
    I2C_Info.Len = 1;
	field = (MS_U8)value;
	bret &= STV6111_I2C_Read_Len(u8TunerIndex, &I2C_Info);

	position = 0;
	while(((field>>position)&0x01)==0x00)
	{
		position++;
	}
	position = position;

	*pu8Value = (I2C_Info.Data[0]& field)>>position;

	return bret;

}


static MS_BOOL STV6111InitTuner(MS_U8 u8TunerIndex)
{
	STV6111_I2C_LEN_TYPE I2C_Info;
	MS_BOOL bret = TRUE;

    I2C_Info.RegAddr = RSTV6111_CTRL1;
	I2C_Info.Len = sizeof(STV6111_Initial_Value);
	memcpy(&I2C_Info.Data[0], &STV6111_Initial_Value[0], I2C_Info.Len);
	
	bret &= STV6111_I2C_Write_Len(u8TunerIndex, &I2C_Info);
    MsOS_DelayTask(1);         
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_ODIV, 0);	

	if(FXTAL > 27000000)
	{
		STV6111_SetFieldImage(u8TunerIndex, FSTV6111_RDIV, 1);
	}
	else
		STV6111_SetFieldImage(u8TunerIndex, FSTV6111_RDIV, 0);

	return bret;        	
}

static MS_BOOL STV6111_TunerAGCMode(MS_U8 u8TunerIndex, STV6111_AGC_MODE mode)
{
   MS_BOOL bret = TRUE;
   
   if(CAHNNEL_AGC_MODE == mode)  /* Channel AGC Mode */
   {
       bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_BB_MODE, 3);
	   bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_LNA_AGC_MODE, 1);
   }
   else	/* RF AGC Mode) */ 
   {
	   bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_BB_MODE, 0);
	   bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_LNA_AGC_MODE, 0);
    }

	return bret;
}

static MS_BOOL STV6111_Tuner_Set_Channel_Frequency(MS_U8 u8TunerIndex, MS_U32 u32FreqKHz)
{
	MS_U32 fequencyVco = 0;
	MS_U32 u32P = 0, u32R = 0, u32F = 0, u32N = 0;
	MS_U8 regR_DIV =0, u8ICP = 0, u8Temp = 0, i;
	MS_BOOL bret = TRUE;

    bret &= STV6111_GetFieldImage(u8TunerIndex,FSTV6111_RDIV, &regR_DIV);
	u32R = regR_DIV + 1;

	if (u32FreqKHz<=1300000)
	{
		u32P = 4;
	}
	else 
	{
		u32P = 2;
	}

	u32N = (u32FreqKHz * u32P * u32R)/(FXTAL/1000);  /*(N + F/2^18) = (Frequency *P*R)/Fxtal */ 
	u32F = ((u32FreqKHz * u32P * u32R)%(FXTAL/1000))*0x40000;
	u32F/= (FXTAL/1000);

	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_LO_DIV, (MS_U8)(u32P/2 - 1));
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_N_LSB, (MS_U8)(u32N & 0xFF));
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_N_MSB, (MS_U8)((u32N>>8) && 0x01));
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_F_H, (MS_U8)((u32F>>15) & 0x07));
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_F_M, (MS_U8)((u32F>>7) & 0xFF));
    bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_F_L, (MS_U8)(u32F & 0x7F));
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_K, FXTAL/1000000-15);


	/*Set charge pump current according to the VCO freq*/	
    fequencyVco = u32FreqKHz*u32P/1000; //MHz
	if (fequencyVco<2700) u8ICP=0;
	else if (fequencyVco<2950) u8ICP=1;
	else if (fequencyVco<3300) u8ICP=2;
	else if (fequencyVco<3700) u8ICP=3;
	else if (fequencyVco<4200) u8ICP=5;
	else if (fequencyVco<4800) u8ICP=6;
	else u8ICP=7;

    bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_PLL_ICP,u8ICP);     
	bret &= STV6111_TunerAGCMode(u8TunerIndex,RFAGC_MODE);
    //bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_PLL_ICP,7);           
	/*LNA calibration IIP3 vs NF mode*/	
    bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_LNA_SEL_GAIN,1);	// forece IIP3 mode

	/*VCO auto calibration to verify*/
	u8Temp = 1;
	bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_VCO_CAL_START,u8Temp);
	i=0;
	u8Temp = 0;
	while(i<10)
	{
	    bret &= STV6111_GetFieldImage(u8TunerIndex,FSTV6111_VCO_CAL_START, &u8Temp);
	    if(!u8Temp)
	    {
            TUNER_DBG(("STV6111 VCO Cal. Done !!\n"));
			break;
		}
	    MsOS_DelayTask(1);
	    i++;
	}  
	/*LNA calibration IIP3 vs NF mode*/  
	MsOS_DelayTask(10);
	bret &= STV6111_GetFieldImage(u8TunerIndex,FSTV6111_LNA_AGC_LOW, &u8Temp);	
	if(u8Temp)
	{
	    // low signal, choose NF mode 
        bret &= STV6111_SetFieldImage(u8TunerIndex,FSTV6111_LNA_SEL_GAIN,0);  	
    }


    i=0;
	u8Temp = 0;
	while(i<10)
	{
	    bret &= STV6111_GetFieldImage(u8TunerIndex, FSTV6111_PLL_LOCK, &u8Temp);
		if(u8Temp)
	    {
            TUNER_DBG(("STV6111 PLL Lock !!\n"));
			break;
		}
	    MsOS_DelayTask(1);
	    i++;
	}  

    return bret;
}

static MS_BOOL STV6111_Tuner_Set_Filter_Bandwidth(MS_U8 u8TunerIndex, MS_U32 u32SR_KHz)
{
    MS_U32 u32filter_hz = 0;
	MS_U8 u8reg = 0, i;
	MS_BOOL bret = TRUE;
	MS_U8 u8Temp;

	u32filter_hz = 10000000+1350*u32SR_KHz;

	if((u32filter_hz/2) > 50000000) /* if BW/2>50 MHz clamp to 50 MHz! */
	{
		u8reg = 44;
	}
	else if((u32filter_hz/2) < 6000000)
	{
		u8reg = 0;
	}
	else
	{
		u8reg = (u32filter_hz/2)/1000000-6;
	}	
 
    bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_BB_FILT_SEL, u8reg);
	//bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_BB_FILT_SEL, 0x11);
	u8Temp = 1;
    bret &= STV6111_SetFieldImage(u8TunerIndex, FSTV6111_BB_CAL_START, u8Temp) ;
	i=0;
	/* Wait for FSTV6111_BB_CAL_START reset at the end of the calibration */
	u8Temp = 0;
	while(i<10)
	{
	    bret &= STV6111_GetFieldImage(u8TunerIndex,FSTV6111_BB_CAL_START, &u8Temp);
	    if(!u8Temp)
	    {
            TUNER_DBG(("STV6111 BB Cal. Done !!\n"));
			break;
		}
	    MsOS_DelayTask(1);
	    i++;
	}  	

	return bret;
}

MS_BOOL MDrv_Tuner_STV6111_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    STV6111_Handle* pstSTV6111= NULL;
	
	 if(_get_handle(u8TunerIndex, &pstSTV6111) == FALSE)
		 return FALSE;

    if(pstSTV6111->Initial_done_flag)
    {
        TUNER_DBG(("STV6111 have been initialized\n"));
		return TRUE;
    }
	
    if((pParam->pCur_Broadcast_type == NULL) || (pParam->pstDemodtab == NULL))
        return FALSE;
    else
    {
        pstSTV6111->Init_Param.pCur_Broadcast_type = pParam->pCur_Broadcast_type;
        pstSTV6111->Init_Param.pstDemodtab = pParam->pstDemodtab;
        pstSTV6111->Init_Param.stTUNCon.eI2C_PORT = pParam->stTUNCon.eI2C_PORT;
        pstSTV6111->Init_Param.stTUNCon.u32HW_ResetPin= pParam->stTUNCon.u32HW_ResetPin;
    }

	if(STV6111InitTuner(u8TunerIndex))
	{
	    pstSTV6111->Initial_done_flag = TRUE;
        return TRUE;
	}
	else
	{
        return FALSE;
    }
}

MS_BOOL MDrv_Tuner_STV6111_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u32CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bret = TRUE;

	bret &= STV6111_Tuner_Set_Channel_Frequency(u8TunerIndex, u32CenterFreq*1000);
	bret &= STV6111_Tuner_Set_Filter_Bandwidth(u8TunerIndex, u32SymbolRate_Hz);  
	return bret;
}

MS_BOOL MDrv_Tuner_STV6111_CheckLock(MS_U8 u8TunerIndex)
{
    MS_BOOL bLock = FALSE;;
	MS_U8 u8reg = 0;
	
	if(STV6111_GetFieldImage(u8TunerIndex, FSTV6111_PLL_LOCK, &u8reg))
	{
        if(u8reg)
	        bLock = TRUE;
	}

    return bLock;
}

#define STV6111_CHIP_ID 0x05
MS_BOOL MDrv_Tuner_STV6111_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 regData = 0;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_IIC_PORT ePort;
    MS_U8 u8I2C_Port = 0, u8MaxI2CPort, i = 0, j;
    STV6111_Handle* pstSTV6111= NULL;

    if(!_stv6111_variables_alloc())
    {
        TUNER_ERR(("[STV6111] _stv6111_variables_alloc fail\n"));
        return FALSE;
    }
	
	if(_get_handle(u8TunerIndex, &pstSTV6111) == FALSE)
    {
		return FALSE;
    }
	
    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    _getI2CPort(u8TunerIndex, &ePort);
    if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
    {
        u8I2C_Port = (MS_U8)ePort/8;
    }
    else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
    {
       u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
    }

    if(pstSTV6111_slave_ID_TBL == NULL)
    {
        pstSTV6111_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(u8MaxI2CPort * sizeof(STV6111_possible_slave_ID));
        if(NULL == pstSTV6111_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(STV6111_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstSTV6111_slave_ID_TBL + i*sizeof(STV6111_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &STV6111_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                }
            }
        }
    }

    i=0;
    do
    {
        pSlaveIDTBL = pstSTV6111_slave_ID_TBL + u8I2C_Port*sizeof(STV6111_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        if(pSlaveIDTBL->bInUse)
        {
            TUNER_DBG(("[STV6111]I2C Slave ID 0x%x Have Used on the same I2C Port\n", pSlaveIDTBL->u8SlaveID));
        }
        else if((pSlaveIDTBL->u8SlaveID) == 0xFF)
        {
            break;
        }
        else
        {
            pstSTV6111->I2C_SalveID = pSlaveIDTBL->u8SlaveID;
            if(STV6111_GetFieldImage(u8TunerIndex, FSTV6111_ID, &regData) == FALSE)
            {
                TUNER_ERR(("[STV6111] Read chip ID fail with slave ID 0x%x\n", pstSTV6111->I2C_SalveID));
            }
            else
            {
               TUNER_DBG(("[STV6111] Read chip ID OK with slave ID 0x%x\n", pstSTV6111->I2C_SalveID));
               break;
            }
        }
        i++;
    }while((pSlaveIDTBL->u8SlaveID) != 0xFF);

    if(regData == STV6111_CHIP_ID)
    {
        if(!bUnderExtDMDTest)
        {
            pSlaveIDTBL->bInUse = TRUE;
			u8STV6111_Cnt++;
        }

        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
    }
    else
    {
       if((!u8STV6111_Cnt) && (!bUnderExtDMDTest))
       {
           _stv6111_variables_free();
       }
    }
    return (regData == STV6111_CHIP_ID);

}


MS_BOOL STV6111_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE;
    TUNER_EXT_FUNCTION_PARAM* pstData = NULL;
    TUNER_CON_CONFIG* pstCon = NULL;
	STV6111_Handle* pstSTV6111= NULL;
    
    switch(fuction_type)
    {
         case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _DigiTuner_Decide_LNB_LO(SAT_PARAM);
            break;

		 case TUNER_EXT_FUNC_FINALIZE:			   
			if(_get_handle(u8TunerIndex, &pstSTV6111) == FALSE)
		        return FALSE;

            pstSTV6111->Initial_done_flag = FALSE;
			break;

         case TUNER_EXT_FUNC_UNDER_EXT_DMD_TEST:
            bUnderExtDMDTest = *(MS_BOOL*)data;
            break;

         case TUNER_EXT_FUNC_SET_CON_INFO:
            pstData = (TUNER_EXT_FUNCTION_PARAM*)data;
            u8max_dev_num = (MS_U8)pstData->u32Param1;
            pstCon = (TUNER_CON_CONFIG*)(pstData->pParam);
            if(!_stv6111_variables_alloc())
            {
                _stv6111_variables_free();
                return FALSE;
            }
            else
            {
                if(_get_handle(u8TunerIndex, &pstSTV6111) == FALSE)
                    return FALSE;
				
                memcpy(&pstSTV6111->Init_Param.stTUNCon, pstCon,sizeof(TUNER_CON_CONFIG));
            }
        break;    
         default:
            break;
    }
    return bret;
}


DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_STV6111) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_STV6111",          // demod system name
    .data               = TUNER_STV6111,            // private data value
    .Init               = MDrv_Tuner_STV6111_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_STV6111_SetFreq_S2,
    .GetLock            = MDrv_Tuner_STV6111_CheckLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_STV6111_CheckExist,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .Extension_Function = STV6111_Extension_Function
};


#endif
