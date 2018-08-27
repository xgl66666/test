/*
                           RT710.c
Copyright 2012 by Rafaelmicro., Inc.
                 C H A N G E   R E C O R D
  Date        Author           Version      Description
--------   --------------    --------   ----------------------
03/06/2012      Ryan                       v2.1
04/08/2013	   Ryan                       v2.2	   add rfgain Function
07/25/2013	   Ryan				v2.3        vga agc slop from negative to positive
01/15/2014	   Ryan				v2.4		modify  setting Frequency from 1650~2000
02/25/2014	   Ryan				v2.5		add Vga Attenuator Function
03/11/2014	   Ryan				v2.6		modify rssi calculate
01/06/2015	   Ryan				v2.7		Initialization value write-once
01/29/2015	   Ryan				v3.0		add 720 setting
01/29/2015	   Ryan				v3.1		add scan mode for RT720 (RT710 not support)
02/17/2015	   Ryan				v3.2		Change the pll execution time
02/12/2015	   Ryan				v3.2a		Standby function add clock parameter
03/30/2015	   Ryan				v3.3		Add RT720 filter table (with different RT710)
04/07/2015	   Ryan				v3.3a		optimize PLL Setting
04/07/2015	   Ryan				v3.4C		Add Xtal Cap Option only for R720 chip
10/30/2015     Vincent          v3.5        modify IF Fillter bandwidth
*/
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "MsCommon.h"
#include "Board.h"
#if IF_THIS_TUNER_INUSE(TUNER_RT710)
#include "apiDigiTuner.h"
#include "drvIIC.h"
#include "drvTuner_RT710.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "drvDemod.h"

#ifndef UNUSED
#define UNUSED __attribute__ ((unused))
#endif

signed char LNA_RSSI_SHIFT = 0;//(dB*10)
unsigned char RT710_ADDRESS=0xF4;
unsigned char R0TOR3_Write_Arry[4]={0x00,0x00,0x00,0x00};

//===========Xtal Cap set (Range is 0~30pF) ==============
//Suggest Xtal_cap use 30pF when xtal CL value is 16pF , Default.
unsigned char Xtal_cap = 30;  //Unit:pF

I2C_TYPE RT710_Write_Byte;
I2C_LEN_TYPE RT710_Write_Len;
I2C_LEN_TYPE RT710_Read_Len;
static MS_BOOL bUnderExtDMDTest = FALSE;
SLAVE_ID_USAGE* pstRT710_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE RT710_possible_slave_ID[5] =
{
    {0xF4, FALSE},
    {0xB4, FALSE},
    {0x74, FALSE},
    {0x34, FALSE},
    {0xFF, FALSE}
};


RT710_Handle* pstRT710_Handle = NULL;
static MS_U8 u8RT710_Cnt = 0;
RT710_Err_Type RT710_PLL(MS_U8 u8TunerIndex, unsigned long PLL_Freq);


#if(RT710_0DBM_SETTING == TRUE)
	//0dBm ; LT:lna output ; LT:HPF off  ; LT Current High
	UINT8 RT710_Reg_Arry_Init[RT710_Reg_Num] ={0x40, 0x1C, 0xA0, 0x90, 0x41, 0x50, 0xED, 0x25, 0x47, 0x58, 0x39, 0x60, 0x38, 0xE7, 0x90, 0x35};
#else
	//-10~-30dBm ; LT:lna center ; LT:HPF on  ; LT Current Low
	UINT8 RT710_Reg_Arry_Init[RT710_Reg_Num] ={0x40, 0x1D, 0x20, 0x10, 0x41, 0x50, 0xED, 0x25, 0x07, 0x58, 0x39, 0x64, 0x38, 0xE7, 0x90, 0x35};
#endif

UINT8 RT720_Reg_Arry_Init[RT710_Reg_Num] ={0x00, 0x1C, 0x00, 0x10, 0x41, 0x48, 0xDA, 0x4B, 0x07, 0x58, 0x28, 0x40, 0x37, 0xE7, 0x4C, 0x59};


static signed char RT710_Rssi_freqency_shift[6] =
{
	0,-10,-20,-40,-55,-30
};

static unsigned short RT710_Lna_Acc_Gain[19] =
{
 0, 30, 35, 70, 100, 129, 152, 181, 188, 200,  //0~9
 215, 240, 273, 303, 331, 335, 357, 378, 409,	//10~19
};
static unsigned short RT710_Lna_Acc_Gain_1[19] = //<1500MHz
{
 0, 26, 42, 73, 104, 130, 158, 182, 192, 200,  //0~9
 215, 242, 273, 306, 333, 342, 364, 386, 405,	//10~19
};
static unsigned short RT710_Lna_Acc_Gain_2[19] = //>1500MHz
{
 0, 29, 45, 76, 107, 133, 154, 176, 184, 198,  //0~9
 224, 258, 286, 311, 331, 340, 369, 390, 409,	//10~19
};
static unsigned short RT710_Lna2_Acc_Gain[32] =
{
 0, 27, 53, 81, 109, 134, 156, 176, 194, 202,   //0~9
 211, 221, 232, 245, 258, 271, 285, 307, 326, 341,//10~19
 357, 374, 393, 410, 428, 439, 445, 470, 476, 479,//20~29
 495, 507 //30~31
};

static MS_BOOL _RT710_Handle_alloc(void)
{
    MS_U8 i;
    RT710_Handle* pstRT710 = NULL;

    if(NULL == pstRT710_Handle)
        pstRT710_Handle = (RT710_Handle*)malloc(sizeof(RT710_Handle)*MAX_FRONTEND_NUM);
    else
        return TRUE;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
    {
        for(i=0; i<MAX_FRONTEND_NUM; i++)
        {
            pstRT710 = pstRT710_Handle + i;
            pstRT710->I2C_SalveID = RT710_ADDRESS; //temp
            pstRT710->Initial_done_flag = FALSE;
            pstRT710->RT710_Mutex= -1;
        }
        return TRUE;
    }
}

static MS_BOOL _RT710_Handle_free(void)
{
    if(NULL != pstRT710_Handle)
    {
        free(pstRT710_Handle);
        pstRT710_Handle = NULL;
    }
    return TRUE;
}

static  MS_BOOL  _RT710_Decide_LNB_LO(TUNER_MS_SAT_PARAM *pSATParam)
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
         s32Offset = pSATParam->u16LoLOF+ RT710_MAX_INPUT_FREQ - pSATParam->u16HiLOF - RT710_MIN_INPUT_FREQ;
         if(s32Offset < 0)
              s32Offset = 0;
          else
             s32Offset /= 2;

         if( u32MidFreq <= (RT710_MAX_INPUT_FREQ-s32Offset))
              *pSATParam->pbIsHiLOF = FALSE;
          else
              *pSATParam->pbIsHiLOF = TRUE;
     }
     return TRUE;
}

static MS_BOOL IIC_READ(MS_U8 u8TunerIndex,MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{
    MS_IIC_PORT ePort;

    ePort = getI2CPort(u8TunerIndex);

    if (FALSE == MDrv_IIC_ReadBytes(ePort, u8SlaveID, u8AddrNum, paddr, u16size, pu8data))
    {
        TUNER_ERR(("MDrv_IIC_ReadBytes Error \n"));
        return FALSE;
    }
    return TRUE;
}

MS_BOOL I2C_Read_Len(MS_U8 u8TunerIndex,I2C_LEN_TYPE *I2C_Info)
{
    MS_U8 DataCunt = 0;
    MS_U8 regadd = 0x00;
    MS_U8 *paddr = &regadd;
    MS_U16 u16size  = I2C_Info->Len;
    MS_U8 pu8data[50];
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;


    if (FALSE == IIC_READ(u8TunerIndex, pstRT710->I2C_SalveID, paddr, 1, pu8data, u16size))
    {
        TUNER_ERR(("IIC Read Len error\n"));
        return FALSE;
    }

    for (DataCunt = 0; DataCunt < u16size; DataCunt ++)
    {
        I2C_Info->Data[DataCunt] = RT710_Convert(pu8data[DataCunt]);
    }

    return TRUE;
}


static MS_BOOL I2C_Write(MS_U8 u8TunerIndex,I2C_TYPE *I2C_Info)
{
    MS_U16 u16size  = 1;
    MS_U8 *paddr = &(I2C_Info->RegAddr);
    MS_U8 *pu8data = &(I2C_Info->Data);
    MS_IIC_PORT ePort;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    ePort = getI2CPort(u8TunerIndex);
    if (FALSE == MDrv_IIC_WriteBytes(ePort, pstRT710->I2C_SalveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("devCOFDM_PassThroughIIC_WRITEBytes Error \n"));
        return FALSE;
    }

    //RT710_Delay_MS(1); // delay 1ms

    return TRUE;
}

MS_BOOL I2C_Write_Len(MS_U8 u8TunerIndex,I2C_LEN_TYPE *I2C_Info)
{
    MS_U16 u16size  = I2C_Info->Len;
    MS_U8 *paddr = &(I2C_Info->RegAddr);
    MS_U8 *pu8data = I2C_Info->Data;
    MS_IIC_PORT ePort;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    ePort = getI2CPort(u8TunerIndex);
    if (FALSE == MDrv_IIC_WriteBytes(ePort, pstRT710->I2C_SalveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("devCOFDM_PassThroughIIC_WRITEBytes Error \n"));
        return FALSE;
    }

    //RT710_Delay_MS(1); // delay 1ms

    return TRUE;
}


int RT710_Convert(int InvertNum)
{
	int ReturnNum = 0;
	int AddNum    = 0x80;
	int BitNum    = 0x01;
	int CuntNum   = 0;

	for(CuntNum = 0;CuntNum < 8;CuntNum ++)
	{
		if(BitNum & InvertNum)
			ReturnNum += AddNum;

		AddNum /= 2;
		BitNum *= 2;
	}

	return ReturnNum;
}


RT710_Err_Type RT710_Setting(MS_U8 u8TunerIndex)
{
    MS_U8 fine_tune = 0;
    MS_U8 Coarse = 0;
    MS_U16 bw_offset = 20000;
    MS_U32 offset_range = 0;
    int icount,i;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return RT_Fail;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    if(!pstRT710->Initial_done_flag)
    {
        pstRT710->pre_satellite_bw = 0; //Init BW Value
        pstRT710->satellite_bw = 0;

        I2C_Read_Len(u8TunerIndex,&RT710_Read_Len) ; // read data length
        if((RT710_Read_Len.Data[3]&0xF0)==0x70) //TRUE:RT710(R3[7:4]=7) ; FALSE;RT720(R3[7:4]=0)
            pstRT710->Chip_type_Is_RT710=TRUE;
        else
            pstRT710->Chip_type_Is_RT710=FALSE;

        if(pstRT710->Chip_type_Is_RT710 == TRUE)//TRUE:RT710 ; FALSE;RT720
        {
            for(icount=0;icount<RT710_Reg_Num;icount++)
            {
                pstRT710->Reg_Arry[icount]=RT710_Reg_Arry_Init[icount];
            }
        }
        else
        {
            for(icount=0;icount<RT710_Reg_Num;icount++)
            {
                pstRT710->Reg_Arry[icount]=RT720_Reg_Arry_Init[icount];
            }
        }

        // LOOP_THROUGH(0=on ; 1=off)
        if(pstRT710->Info_Msg.RT710_LoopThrough_Mode != LOOP_THROUGH)
        {
            pstRT710->Reg_Arry[1] |=0x04;
        }
        else
        {
            pstRT710->Reg_Arry[1] &=0xFB;
        }

        //Clock out(1=off ; 0=on)
        if(pstRT710->Info_Msg.RT710_ClockOut_Mode != ClockOutOn)
        {
            pstRT710->Reg_Arry[3] |=0x10;
        }
        else
        {
            pstRT710->Reg_Arry[3] &=0xEF;
        }

        //Output Signal Mode
        if(pstRT710->Info_Msg.RT710_OutputSignal_Mode != DifferentialOut)
        {
            pstRT710->Reg_Arry[11] |=0x10;
        }
        else
        {
            pstRT710->Reg_Arry[11] &=0xEF;
        }

        //AGC Type  //R13[4] Negative=0 ; Positive=1;
        if(pstRT710->Info_Msg.RT710_AGC_Mode != AGC_Positive)
        {
            pstRT710->Reg_Arry[13] &=0xEF;
        }
        else
        {
            pstRT710->Reg_Arry[13] |=0x10;
        }

        //RT710_Vga_Sttenuator_Type
        if(pstRT710->Info_Msg.RT710_AttenVga_Mode != ATTENVGAON)
        {
            pstRT710->Reg_Arry[11] &= 0xF7;
        }
        else
        {
            pstRT710->Reg_Arry[11] |= 0x08;
        }


        if(pstRT710->Chip_type_Is_RT710 == TRUE)//TRUE:RT710 ; FALSE;RT720
        {
            pstRT710->Reg_Arry[14] = (pstRT710->Reg_Arry[14] & 0xFC) | pstRT710->Info_Msg.R710_FineGain;
        }
        else
        {
            if( pstRT710->Info_Msg.R710_FineGain > 1)
                pstRT710->Reg_Arry[14] = (pstRT710->Reg_Arry[14] & 0xFE) | 0x01;
            else
                pstRT710->Reg_Arry[14] = (pstRT710->Reg_Arry[14] & 0xFE) | 0x00;

            pstRT710->Reg_Arry[3] = ((pstRT710->Reg_Arry[3] & 0xF0) | ((30 - Xtal_cap)>>1));
        }


        for(i=0;i<RT710_Write_Len.Len;i++)
        {
            RT710_Write_Len.Data[i]=pstRT710->Reg_Arry[i];
        }

        if(I2C_Write_Len(u8TunerIndex,&RT710_Write_Len) != RT_Success)
            return RT_Fail;

        pstRT710->Initial_done_flag = TRUE;
    }

    // Check Input Frequency Range
    if(pstRT710->Chip_type_Is_RT710 == TRUE)//TRUE:RT710 ; FALSE;RT720
    {
        if((pstRT710->Info_Msg.RF_KHz<600000) || (pstRT710->Info_Msg.RF_KHz>2400000))
            return RT_Fail;
    }
    else
    {
        if((pstRT710->Info_Msg.RF_KHz<200000) || (pstRT710->Info_Msg.RF_KHz>2400000))
            return RT_Fail;
    }

    if(RT710_PLL(u8TunerIndex, pstRT710->Info_Msg.RF_KHz)!=RT_Success)
        return RT_Fail;

    RT710_Delay_MS(10);

    if(pstRT710->Chip_type_Is_RT710 == TRUE)
    {
        if((pstRT710->Info_Msg.RF_KHz >= 1600000) && (pstRT710->Info_Msg.RF_KHz < 1950000))
        {
            pstRT710->Reg_Arry[2] |= 0x40; //LNA Mode with att
            pstRT710->Reg_Arry[8] |= 0x80; //Mixer Buf -3dB
            pstRT710->Reg_Arry[10] =  (RT710_Reg_Arry_Init[10]); //AGC VTH:1.24V  ;  VTL:0.64V
            RT710_Write_Byte.RegAddr = 0x0A;
            RT710_Write_Byte.Data = pstRT710->Reg_Arry[10];
            if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
                    return RT_Fail;
        }
        else
        {
            pstRT710->Reg_Arry[2] &= 0xBF; //LNA Mode no att
            pstRT710->Reg_Arry[8] &= 0x7F; //Mixer Buf off
            if(pstRT710->Info_Msg.RF_KHz >= 1950000)
            {
                pstRT710->Reg_Arry[10] = ((pstRT710->Reg_Arry[10] & 0xF0) | 0x08); //AGC VTH:1.14V
                pstRT710->Reg_Arry[10] = ((pstRT710->Reg_Arry[10] & 0x0F) | 0x20); //AGC VTL:0.54V
                RT710_Write_Byte.RegAddr = 0x0A;
                RT710_Write_Byte.Data = pstRT710->Reg_Arry[10];
                if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
                    return RT_Fail;
            }
            else
            {
                pstRT710->Reg_Arry[10] =  (RT710_Reg_Arry_Init[10]); //AGC VTH:1.24V  ;  VTL:0.64V
                RT710_Write_Byte.RegAddr = 0x0A;
                RT710_Write_Byte.Data = pstRT710->Reg_Arry[10];
                if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
                    return RT_Fail;
            }
        }

        RT710_Write_Byte.RegAddr = 0x02;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[2];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
            return RT_Fail;

        RT710_Write_Byte.RegAddr = 0x08;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[8];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
            return RT_Fail;

        if(pstRT710->Info_Msg.RF_KHz >= 2000000)
        {
            pstRT710->Reg_Arry[14]=( pstRT710->Reg_Arry[14]& 0xF3)|0x08;
        }
        else
        {
            pstRT710->Reg_Arry[14]=( pstRT710->Reg_Arry[14]& 0xF3)|0x00;
        }
        RT710_Write_Byte.RegAddr = 0x0E;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[14];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
            return RT_Fail;
    }
    else
    {
        //Scan_Type
        if(pstRT710->Info_Msg.RT710_Scan_Mode != RT710_AUTO_SCAN)
        {
            pstRT710->Reg_Arry[11] &= 0xFC;
            //claire add
            if(pstRT710->Info_Msg.SymbolRate_KHz >= 15000)
                pstRT710->Info_Msg.SymbolRate_KHz += 6000;
        }
        else
        {
            pstRT710->Reg_Arry[11] |= 0x02;// 3dB
            pstRT710->Info_Msg.SymbolRate_KHz += 10000;
        }
        RT710_Write_Byte.RegAddr = 0x0B;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[11];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
            return RT_Fail;
    }


    switch (pstRT710->Info_Msg.RT710_RollOff_Mode)
    {
        case ROLL_OFF_0_15:
            pstRT710->satellite_bw = (UINT32)((pstRT710->Info_Msg.SymbolRate_KHz * 115)/10);
            break;
        case ROLL_OFF_0_20:
            pstRT710->satellite_bw = (UINT32)((pstRT710->Info_Msg.SymbolRate_KHz * 120)/10);
            break;
        case ROLL_OFF_0_25:
            pstRT710->satellite_bw = (UINT32)((pstRT710->Info_Msg.SymbolRate_KHz * 125)/10);
            break;
        case ROLL_OFF_0_30:
            pstRT710->satellite_bw = (UINT32)((pstRT710->Info_Msg.SymbolRate_KHz * 130)/10);
            break;
        case ROLL_OFF_0_35:
            pstRT710->satellite_bw = (UINT32)((pstRT710->Info_Msg.SymbolRate_KHz * 135)/10);
            break;
        case ROLL_OFF_0_40:
            pstRT710->satellite_bw = (UINT32)((pstRT710->Info_Msg.SymbolRate_KHz * 140)/10);
            break;
    }


    if(pstRT710->satellite_bw != pstRT710->pre_satellite_bw)
    {
        if(pstRT710->Chip_type_Is_RT710 == TRUE)//TRUE:RT710 ; FALSE;RT720
        {
            if((pstRT710->satellite_bw) >=380000)
            {
                fine_tune=1;
                Coarse =(UINT8) (( pstRT710->satellite_bw -380000) /17400)+16;
           if((( pstRT710->satellite_bw -380000) % 17400) > 0)
           Coarse+=1;
       }
       else if(pstRT710->satellite_bw<=50000)
       {
           Coarse=0;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>50000) && (pstRT710->satellite_bw<=73000))
       {
           Coarse=0;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>73000) && (pstRT710->satellite_bw<=96000))
       {
           Coarse=1;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>96000) && (pstRT710->satellite_bw<=104000))
       {
           Coarse=1;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>104000) && (pstRT710->satellite_bw<=116000))
       {
           Coarse=2;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>116000) && (pstRT710->satellite_bw<=126000))
       {
           Coarse=2;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>126000) && (pstRT710->satellite_bw<=134000))
       {
           Coarse=3;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>134000) && (pstRT710->satellite_bw<=146000))
       {
           Coarse=3;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>146000) && (pstRT710->satellite_bw<=158000))
       {
           Coarse=4;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>158000) && (pstRT710->satellite_bw<=170000))
       {
           Coarse=4;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>170000) && (pstRT710->satellite_bw<=178000))
       {
           Coarse=5;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>178000) && (pstRT710->satellite_bw<=190000))
       {
           Coarse=5;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>190000) && (pstRT710->satellite_bw<=202000))
       {
           Coarse=6;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>202000) && (pstRT710->satellite_bw<=212000))
       {
           Coarse=6;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>212000) && (pstRT710->satellite_bw<=218000))
       {
           Coarse=7;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>218000) && (pstRT710->satellite_bw<=234000))
       {
           Coarse=7;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>234000) && (pstRT710->satellite_bw<=244000))
       {
           Coarse=9;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>244000) && (pstRT710->satellite_bw<=246000))
       {
           Coarse=10;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>246000) && (pstRT710->satellite_bw<=262000))
       {
           Coarse=10;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>262000) && (pstRT710->satellite_bw<=266000))
       {
           Coarse=11;
           fine_tune=0;
       }
       else if((pstRT710->satellite_bw>266000) && (pstRT710->satellite_bw<=282000))
       {
           Coarse=11;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>282000) && (pstRT710->satellite_bw<=298000))
       {
           Coarse=12;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>298000) && (pstRT710->satellite_bw<=318000))
       {
           Coarse=13;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>318000) && (pstRT710->satellite_bw<=340000))
       {
           Coarse=14;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>340000) && (pstRT710->satellite_bw<=358000))
       {
           Coarse=15;
           fine_tune=1;
       }
       else if((pstRT710->satellite_bw>358000) && (pstRT710->satellite_bw<380000))
       {
           Coarse=16;
           fine_tune=1;
       }
     }
     else
     {

         if(pstRT710->Info_Msg.RT710_RollOff_Mode>= 2 )
             fine_tune=1;
         else
             fine_tune=0;

         offset_range = fine_tune * bw_offset ;


    /*
         Symbol Rate <= 15MHz =>  +3MHz
         Symbol Rate 15MHz ~ 20MHz  =>  +2MHz
         Symbol Rate <= 30MHz =>  +1MHz
     */
         if(pstRT710->Info_Msg.SymbolRate_KHz<=15000)
             pstRT710->Info_Msg.SymbolRate_KHz += 3000;
         else if(pstRT710->Info_Msg.SymbolRate_KHz <=20000)
             pstRT710->Info_Msg.SymbolRate_KHz += 2000;
         else if(pstRT710->Info_Msg.SymbolRate_KHz <=30000)
             pstRT710->Info_Msg.SymbolRate_KHz += 1000;
         else
             pstRT710->Info_Msg.SymbolRate_KHz += 0;


         if((pstRT710->Info_Msg.SymbolRate_KHz*12) <= (88000+offset_range) )
         {
             Coarse=0;
         }
         else if((pstRT710->Info_Msg.SymbolRate_KHz*12) > (88000+offset_range) && (pstRT710->Info_Msg.SymbolRate_KHz*12) <= (368000+offset_range) )
         {
             Coarse =(UINT8) (( (pstRT710->Info_Msg.SymbolRate_KHz*12) - (88000+offset_range)) / 20000);
             if(((UINT16)((pstRT710->Info_Msg.SymbolRate_KHz*12) -(88000+offset_range)) % 20000) > 0)
                 Coarse+=1;
             if(Coarse>=7)
                 Coarse+=1;
         }
         else if((pstRT710->Info_Msg.SymbolRate_KHz*12) > (368000+offset_range) && (pstRT710->Info_Msg.SymbolRate_KHz*12)<=(764000+offset_range))
         {
             Coarse =(UINT8) (( (pstRT710->Info_Msg.SymbolRate_KHz*12) -(368000+offset_range)) /20000) +15;
             if(((UINT16)( (pstRT710->Info_Msg.SymbolRate_KHz*12) -(368000+offset_range)) % 20000) > 0)
                 Coarse+=1;

             if(Coarse>=33)
                 Coarse+=3;
             else if(Coarse >= 29)
                 Coarse+=2;
             else if(Coarse>=27)
                 Coarse+=3;
             else if(Coarse>=24)
                 Coarse+=2;
             else if(Coarse>=19)
                 Coarse+=1;
             else
                 Coarse+=0;

         }
         else
         {
             Coarse=42;
         }

     }

     //fine tune and coras filter write
     RT710_Write_Byte.RegAddr = 0x0F;
     RT710_Read_Len.Data[15] &= 0x00;
     pstRT710->Reg_Arry[15] = ((RT710_Read_Len.Data[15] | ( fine_tune  ) ) | ( Coarse<<2));
     RT710_Write_Byte.Data = pstRT710->Reg_Arry[15];
     if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
         return RT_Fail;
    }

    pstRT710->pre_satellite_bw = pstRT710->satellite_bw;

    for(icount=0;icount<4;icount++)
    {
        R0TOR3_Write_Arry[icount]=pstRT710->Reg_Arry[icount];
    }
    return RT_Success;
}




RT710_Err_Type RT710_PLL(MS_U8 u8TunerIndex, unsigned long Freq)
{
    UINT8  MixDiv   = 2;
    UINT8  Ni       = 0;
    UINT8  Si       = 0;
    UINT8  DivNum   = 0;
    UINT8  Nint     = 0;
    UINT32 VCO_Min  = 2350000;
    UINT32 VCO_Max  = VCO_Min*2;
    UINT32 VCO_Freq = 0;
    UINT32 PLL_Ref = RT710_Xtal;
    UINT32 VCO_Fra = 0;	//VCO contribution by SDM (kHz)
    UINT16 Nsdm = 2;
    UINT16 SDM  = 0;
    UINT16 SDM16to9	= 0;
    UINT16 SDM8to1  = 0;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return RT_Fail;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;


    if(!pstRT710->Initial_done_flag)
        return RT_Fail;

    if(!pstRT710->Chip_type_Is_RT710)
        PLL_Ref = RT720_Xtal;

    if (Freq>=2350000)
    {
         VCO_Min  = Freq;
    }
    while(MixDiv <= 16)  // 2 -> 4 -> 8 -> 16
    {
        if(((Freq * MixDiv) >= VCO_Min) && ((Freq * MixDiv) <= VCO_Max))// Lo_Freq->Freq
        {
            if(MixDiv==2)
                DivNum = 1;
            else if(MixDiv==4)
                DivNum = 0;
            else if(MixDiv==8)
                DivNum = 2;
            else   //16
                DivNum = 3;
            break;
        }
        MixDiv = MixDiv << 1;
    }

    //Divider
    RT710_Write_Byte.RegAddr = 0x04;
    pstRT710->Reg_Arry[4] &= 0xFE;
    pstRT710->Reg_Arry[4] |= (DivNum & 0x01) ;
    RT710_Write_Byte.Data = pstRT710->Reg_Arry[4];
    if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
        return RT_Fail;

    if(pstRT710->Chip_type_Is_RT710 == FALSE)//TRUE:RT710 ; FALSE;RT720
    {
        RT710_Write_Byte.RegAddr = 0x08;
        pstRT710->Reg_Arry[8] &= 0xEF;
        pstRT710->Reg_Arry[8] |= ((DivNum & 0x02)>>1) << 4;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[8];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
             return RT_Fail;

        //Depend on divider setting
        //If Div /2 or /4 : Div Current 150uA(10); other : Div Current 100uA(01) R4[7:6]
        //If Div /2 or /4 : PLL IQ Buf High(1); other : PLL IQ Buf Low(0) R12[4]
       if(DivNum <= 1) // Div/2 or /4
       {
            pstRT710->Reg_Arry[4] &= 0x3F;
            pstRT710->Reg_Arry[4] |= 0x40;
            pstRT710->Reg_Arry[12] |= 0x10;
       }
       else    //Div /8 or /16
       {
           pstRT710->Reg_Arry[4] &= 0x3F;
           pstRT710->Reg_Arry[4] |= 0x80;
           pstRT710->Reg_Arry[12] &= 0xEF;
       }

        RT710_Write_Byte.RegAddr = 0x0C;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[12];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
            return RT_Fail;

        RT710_Write_Byte.RegAddr = 0x04;
        RT710_Write_Byte.Data = pstRT710->Reg_Arry[4];
        if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
            return RT_Fail;
    }

    VCO_Freq = Freq * MixDiv;// Lo_Freq->Freq
    Nint     = (UINT8) (VCO_Freq / 2 / PLL_Ref);
    VCO_Fra  = (UINT16) (VCO_Freq - 2 * PLL_Ref * Nint);

    //boundary spur prevention
    if (VCO_Fra < PLL_Ref/64)             // 2*PLL_Ref/128
        VCO_Fra = 0;
    else if (VCO_Fra > PLL_Ref*127/64)    // 2*PLL_Ref*127/128
    {
        VCO_Fra = 0;
        Nint ++;
    }
    else if((VCO_Fra > PLL_Ref*127/128) && (VCO_Fra < PLL_Ref)) //> 2*PLL_Ref*127/256,  < 2*PLL_Ref*128/256
        VCO_Fra = PLL_Ref*127/128;      // VCO_Fra = 2*PLL_Ref*127/256
    else if((VCO_Fra > PLL_Ref) && (VCO_Fra < PLL_Ref*129/128)) //> 2*PLL_Ref*128/256,  < 2*PLL_Ref*129/256
        VCO_Fra = PLL_Ref*129/128;      // VCO_Fra = 2*PLL_Ref*129/256
    else
        VCO_Fra = VCO_Fra;

    //N & S
    Ni       = (Nint - 13) / 4;
    Si       = Nint - 4 *Ni - 13;
    RT710_Write_Byte.RegAddr = 0x05;
    pstRT710->Reg_Arry[5]  = 0x00;
    pstRT710->Reg_Arry[5] |= (Ni + (Si << 6));
    RT710_Write_Byte.Data  = pstRT710->Reg_Arry[5];
    if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
        return RT_Fail;

    //pw_sdm
    RT710_Write_Byte.RegAddr = 0x04;
    pstRT710->Reg_Arry[4] &= 0xFD;
    if(VCO_Fra == 0)
        pstRT710->Reg_Arry[4] |= 0x02;
    RT710_Write_Byte.Data = pstRT710->Reg_Arry[4];
    if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
        return RT_Fail;

    //SDM calculator
    while(VCO_Fra > 1)
    {
        if (VCO_Fra > (2*PLL_Ref / Nsdm))
        {
            SDM = SDM + 32768 / (Nsdm/2);
            VCO_Fra = VCO_Fra - 2*PLL_Ref / Nsdm;
            if (Nsdm >= 0x8000)
                 break;
        }
        Nsdm = Nsdm << 1;
    }

    SDM16to9 = SDM >> 8;
    SDM8to1 =  SDM - (SDM16to9 << 8);

    RT710_Write_Byte.RegAddr = 0x07;
    pstRT710->Reg_Arry[7]   = (UINT8) SDM16to9;
    RT710_Write_Byte.Data    = pstRT710->Reg_Arry[7];
    if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
        return RT_Fail;
    RT710_Write_Byte.RegAddr = 0x06;
    pstRT710->Reg_Arry[6]   = (UINT8) SDM8to1;
    RT710_Write_Byte.Data    = pstRT710->Reg_Arry[6];
    if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
        return RT_Fail;
    return RT_Success;
}


RT710_Err_Type RT710_Standby(MS_U8 u8TunerIndex,RT710_LoopThrough_Type RT710_LTSel,RT710_ClockOut_Type RT710_CLKSel)
{

    UINT8 RT710_Standby_Reg_Arry[RT710_Reg_Num]={0xFF, 0x5C, 0x88, 0x30, 0x41, 0xC8, 0xED, 0x25, 0x47, 0xFC, 0x48, 0xA2, 0x08, 0x0F, 0xF3, 0x59};
    int i;
    int icount;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return RT_Fail;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    if(!pstRT710->Initial_done_flag)
        return RT_Success;

    if(RT710_CLKSel != ClockOutOn)
    {
        RT710_Standby_Reg_Arry[3] |=0x10;
    }
    else
    {
        RT710_Standby_Reg_Arry[3] &=0xEF;
    }

	if(pstRT710->Chip_type_Is_RT710 == FALSE)//TRUE:RT710 ; FALSE;RT720
    {
		RT710_Standby_Reg_Arry[1] |=0x02;//lna off ;can off together
		RT710_Standby_Reg_Arry[3] |=0x20;//Xtal ldo off
		RT710_Write_Byte.RegAddr = 0x03;
		RT710_Write_Byte.Data  = RT710_Standby_Reg_Arry[3];
		if(I2C_Write(u8TunerIndex,&RT710_Write_Byte) != RT_Success)
        return RT_Fail;
	}

    RT710_Write_Len.RegAddr=0x00;
    RT710_Write_Len.Len=0x10;
    for(i=0;i<RT710_Write_Len.Len;i++)
    {
        RT710_Write_Len.Data[i]=RT710_Standby_Reg_Arry[i];
    }

    if(I2C_Write_Len(u8TunerIndex,&RT710_Write_Len) != RT_Success)
        return RT_Fail;

    pstRT710->Initial_done_flag = FALSE;

    for(icount=0;icount<4;icount++)
    {
        R0TOR3_Write_Arry[icount]=RT710_Standby_Reg_Arry[icount];
    }


    return RT_Success;
}

//------------------------------------------------------------------//
//  RT710_PLL_Lock( ): Read PLL lock status (R2[7])        //
//  Return: 1: PLL lock                                                    //
//          0: PLL unlock                                                //
//------------------------------------------------------------------//
MS_BOOL RT710_PLL_Lock(MS_U8 u8TunerIndex)
{
    MS_BOOL fg_lock = FALSE;
    I2C_LEN_TYPE Dlg_I2C_Len;

    Dlg_I2C_Len.RegAddr = 0x00;
    Dlg_I2C_Len.Len = 3;
    if(I2C_Read_Len(u8TunerIndex,&Dlg_I2C_Len) != RT_Success)
    {
        I2C_Read_Len(u8TunerIndex,&Dlg_I2C_Len);
    }

    if( (Dlg_I2C_Len.Data[2] & 0x80) == 0x00 )
        fg_lock = FALSE;
    else
        fg_lock = TRUE;

    return fg_lock;
}


RT710_Err_Type RT710_GetRfGain(MS_U8 u8TunerIndex, RT710_RF_Gain_Info *RT710_rf_gain)
{
    I2C_LEN_TYPE Dlg_I2C_Len;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return RT_Fail;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    Dlg_I2C_Len.RegAddr = 0x00;
    Dlg_I2C_Len.Len     = 0x04;
    I2C_Read_Len(u8TunerIndex,&Dlg_I2C_Len) ; // read data length
    RT710_rf_gain->RF_gain = (Dlg_I2C_Len.Data[1]>>4); //get rf gain
    RT710_rf_gain->RF_gain +=((Dlg_I2C_Len.Data[1] & 0x01)<<4);

    if(!pstRT710->Initial_done_flag)
        return RT_Fail;

    if(pstRT710->Chip_type_Is_RT710 == TRUE)
    {
        if (RT710_rf_gain->RF_gain <= 2)
        {
            RT710_rf_gain->RF_gain=0;
        }
        else if(RT710_rf_gain->RF_gain > 2 && RT710_rf_gain->RF_gain <= 9)
        {
            RT710_rf_gain->RF_gain -=2;
        }
        else if(RT710_rf_gain->RF_gain >9 && RT710_rf_gain->RF_gain <=12)
        {
            RT710_rf_gain->RF_gain = 7;
        }
        else if(RT710_rf_gain->RF_gain>12 && RT710_rf_gain->RF_gain <= 22)
        {
            RT710_rf_gain->RF_gain -= 5;
        }
        else if(RT710_rf_gain->RF_gain > 22)
        {
            RT710_rf_gain->RF_gain = 18;
        }
    }

    return RT_Success;
}

//----------------------------------------------------------------------//
//  RT710_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                               //
//  2rd parameter: output signal level (dBm*1000)                       //
//  3th parameter: output RF max gain indicator (0:min gain, 1:max gain, 2:active gain)	//
//-----------------------------------------------------------------------//
RT710_Err_Type RT710_GetRfRssi(MS_U8 u8TunerIndex,unsigned long RF_Freq_Khz, int *RfLevelDbm, UINT8 *fgRfGainflag)
{
    RT710_RF_Gain_Info rf_gain_info;
    UINT8  u1Gain1 = 0;
    UINT16  acc_lna_gain = 0;
    UINT16  rf_total_gain = 0;
    UINT16  u2FreqFactor = 0;
    int     rf_rssi = 0;
    int    fine_tune = 0;    //for find tune
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return RT_Fail;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    if(!pstRT710->Initial_done_flag)
        return RT_Fail;

    RT710_GetRfGain(u8TunerIndex,&rf_gain_info);

    u1Gain1 = rf_gain_info.RF_gain;

     //max gain indicator
    if(((u1Gain1==18)&&(pstRT710->Chip_type_Is_RT710==TRUE))||((u1Gain1==31)&&(pstRT710->Chip_type_Is_RT710==FALSE)))
        *fgRfGainflag = 1;
    else if(u1Gain1==0)
        *fgRfGainflag = 0;
    else
        *fgRfGainflag = 2;

    u2FreqFactor = 0;

    //Select LNA freq table
    if(pstRT710->Chip_type_Is_RT710==FALSE)
    {
        u2FreqFactor = 70;
    }
    else if(RF_Freq_Khz<1200000)   //<1200M
    {
        u2FreqFactor = 190;
    }
    else if((RF_Freq_Khz>=1200000)&&(RF_Freq_Khz<1800000))   //1200~1800M
    {
        u2FreqFactor = 170;
    }
    else    // >=2000MHz
    {
        u2FreqFactor = 140;
    }

    //LNA Gain
    if((pstRT710->Chip_type_Is_RT710==FALSE) && (u1Gain1 < (sizeof(RT710_Lna2_Acc_Gain)/sizeof(unsigned short))))
        acc_lna_gain = RT710_Lna2_Acc_Gain[u1Gain1];
    else if((u1Gain1 < (sizeof(RT710_Lna_Acc_Gain)/sizeof(unsigned short))))
        acc_lna_gain = RT710_Lna_Acc_Gain[u1Gain1];

    if((RF_Freq_Khz<1500000) && (u1Gain1 < (sizeof(RT710_Lna_Acc_Gain_1)/sizeof(unsigned short))))   //<1200M
    {
        acc_lna_gain = RT710_Lna_Acc_Gain_1[u1Gain1];
    }
    else if(u1Gain1 < (sizeof(RT710_Lna_Acc_Gain_2)/sizeof(unsigned short)))
    {
        acc_lna_gain = RT710_Lna_Acc_Gain_2[u1Gain1];
    }
    //Add Rf Buf and Mixer Gain
    rf_total_gain = acc_lna_gain;

    rf_rssi = fine_tune - (int) (rf_total_gain + u2FreqFactor);
    *RfLevelDbm = (rf_rssi*100);
    
    return RT_Success;
}

RT710_Err_Type RT710_Mstar_GetIfAgc(MS_U8 u8TunerIndex,signed int  Demod_IF_ADC, signed int  *VgaGain)
{
    unsigned long adc_temp;
    //version2
    signed int  vga_table_1[256] = {//*100
    80,  80,   80,  80,  80,  80,  80,   80,   80,   80,//0
    80,  80,   80,  80,  80,  80,  80,   80,   80,   80,//1
    80,  80,   80,  80,  80,  80,  80,   80,   81,   81,//2
    82,  82,   83,  83,  84,  85,  86,   87,   88,   89,//3
    89,  90,   91,  92,  92,  93,  94,   95,   96,   97,//4
    99,  101, 102, 104, 105, 107, 108,  109,  111,  112,//5
    114, 116, 117, 119, 121, 123, 124,  126,  128,  130,//6
    132, 134, 136, 138, 140, 141, 142,  146,  148,  150,//7
    151, 152, 154, 156, 158, 160, 162,  164,  166,  167,//8
    168, 170, 173, 176, 178, 180, 182,  183,  184,  185,//9
    186, 188, 190, 191, 193, 195, 197,  199,  201,  203,//10
    205, 207, 208, 209, 210, 212, 213,  215,  217,  219,//11
    220, 221, 223, 224, 225, 226, 227,  229,  230,  231,//12
    232, 234, 235, 236, 237, 238, 239,  240,  241,  242,//13
    243, 244, 245, 246, 247, 248, 249,  250,  251,  252,//14
    253, 254, 254, 255, 256, 257, 259,  260,  261,  262,//15
    263, 264, 264, 265, 266, 267, 268,  269,  270,  271,//16
    272, 273, 274, 275, 276, 277, 278,  279,  280,  280,//17
    281, 282, 283, 284, 285, 286, 287,  288,  289,  290,//18
    290, 291, 291, 292, 292, 293, 293,  294,  294,  295,//19
    295, 296, 296, 297, 297, 298, 298,  299,  300,  300,//20
    301, 301, 302, 302, 303, 303, 304,  304,  305,  305,//21
    306, 306, 307, 307, 308, 308, 309,  309,  310,  311,//22
    311, 312, 312, 313, 313, 314, 315,  316,  317,  319,//23
    320, 322, 324, 325, 327, 328, 329,  330,  332,  334,//24
    335, 336, 338, 340, 341, 342};                      //25

    signed int   vga_table_2[64] = {//*100
    343, 345, 348, 353, 355, 358, 363, 365, 368, 373,//0
    378, 383, 385, 388, 390, 392, 398, 400, 403, 405,//1
    408, 413, 415, 418, 420, 423, 425, 426, 428, 430,//2
    433, 435, 438, 440, 441, 443, 444, 446, 448, 450,//3
    452, 453, 454, 456, 458, 459, 460, 461, 462, 463,//4
    464, 465, 466, 467, 468, 468, 469, 470, 470, 471,//5
    471, 472, 472, 473};

    if((Demod_IF_ADC & 0x8000)!=0x8000)
    {

        if(Demod_IF_ADC<24576)
            Demod_IF_ADC=24576;
        else
            Demod_IF_ADC=Demod_IF_ADC;

        adc_temp=((Demod_IF_ADC & 0x1FE0)>>5);
        *VgaGain = (signed int )vga_table_1[adc_temp];

    }
    else
    {
        if(Demod_IF_ADC>36850)
            Demod_IF_ADC=36850;
        else
            Demod_IF_ADC=Demod_IF_ADC;

        adc_temp=((Demod_IF_ADC & 0x1FC0)>>6);
        *VgaGain = (signed int )vga_table_2[adc_temp];
    }

    return RT_Success;
}

//----------------------------------------------------------------------//
//  RT710_GetRfRssi( ): Get RF RSSI Mstar Patch                         //
//  1st parameter: input RF Freq    (KHz)                               //
//  2rd parameter: output signal level (dBm*1000)                       //
//  3th parameter: output RF max gain indicator (0:min gain, 1:max gain, 2:active gain)	//
//-----------------------------------------------------------------------//

RT710_Err_Type RT710_Mstar_GetRfRssi(MS_U8 u8TunerIndex, unsigned long RF_Freq_Khz, unsigned long IF_ADC, int *RfLevelDbm, UINT8 *fgRfGainflag)
{
    RT710_RF_Gain_Info rf_gain_info;
    MS_U8 u1Gain1 = 0;
    MS_U16 acc_lna_gain = 0, rf_total_gain = 0;
    signed short u2FreqFactor = 0;
    signed int rf_rssi, modify_rf_rssi = 0, fine_tune = 0;
    unsigned long quotient = 0, remainder = 0;
    signed int if_vga_gain = 0;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return RT_Fail;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    RT710_GetRfGain(u8TunerIndex, &rf_gain_info);
    u1Gain1 = rf_gain_info.RF_gain;

      //max gain indicator
    if(((u1Gain1==18)&&(pstRT710->Chip_type_Is_RT710==TRUE))||((u1Gain1==31)&&(pstRT710->Chip_type_Is_RT710==FALSE)))
        *fgRfGainflag = 1;
    else if(u1Gain1==0)
        *fgRfGainflag = 0;
    else
        *fgRfGainflag = 2;

    //Select LNA freq table
    if(pstRT710->Chip_type_Is_RT710==FALSE)
    {
        u2FreqFactor = 70;
    }
    else if(RF_Freq_Khz<1050000)   //<1050M
    {
        u2FreqFactor = RT710_Rssi_freqency_shift[0];
    }
    else if(RF_Freq_Khz>=1050000 && RF_Freq_Khz<1400000)   //1050M~1400M
    {
        u2FreqFactor = RT710_Rssi_freqency_shift[1];
    }
    else if(RF_Freq_Khz>=1400000 && RF_Freq_Khz<1550000)   //1400M~1550M
    {
        u2FreqFactor = RT710_Rssi_freqency_shift[2];
    }
    else if(RF_Freq_Khz>=1550000 && RF_Freq_Khz<1700000)   //1550M~1700M
    {
        u2FreqFactor = RT710_Rssi_freqency_shift[3];
    }
    else if(RF_Freq_Khz>=1700000 && RF_Freq_Khz<2000000 )   //1700M~2000M
    {
        u2FreqFactor = RT710_Rssi_freqency_shift[4];
    }
    else    // >2000MHz
    {
        u2FreqFactor = RT710_Rssi_freqency_shift[5];
    }

    //LNA Gain
    if(pstRT710->Chip_type_Is_RT710==FALSE)
        acc_lna_gain = RT710_Lna2_Acc_Gain[u1Gain1];
    else
    {
        if(RF_Freq_Khz<1500000 && (u1Gain1 < (sizeof(RT710_Lna_Acc_Gain_1)/sizeof(unsigned short))))   //<1200M
        {
            acc_lna_gain = RT710_Lna_Acc_Gain_1[u1Gain1];
        }
        else if(u1Gain1 < (sizeof(RT710_Lna_Acc_Gain_2)/sizeof(unsigned short)))
        {
            acc_lna_gain = RT710_Lna_Acc_Gain_2[u1Gain1];
        }
    }

    RT710_Mstar_GetIfAgc(u8TunerIndex, IF_ADC, &if_vga_gain);

    //Add Rf Buf and Mixer Gain
    rf_total_gain = acc_lna_gain;

    rf_rssi = fine_tune - (signed int) (rf_total_gain + u2FreqFactor+if_vga_gain + LNA_RSSI_SHIFT);

    quotient = (-rf_rssi)/10;
    remainder = (-rf_rssi)%10;

    if(remainder>5)
    modify_rf_rssi = -(quotient+1)*10;
    else
    modify_rf_rssi = -(quotient)*10;

    if(modify_rf_rssi>-50)
        rf_rssi = -50;
    else if(modify_rf_rssi<-850)
        modify_rf_rssi = -850;
    else
        modify_rf_rssi = modify_rf_rssi;

    *RfLevelDbm = (rf_rssi*100);

    return RT_Success;
}

/*
Function: InitTuner
Description: initialize tuner setting
Parameter: void *    pData

*/


MS_BOOL  MDrv_Tuner_RT710_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    int icount, i;
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    if(NULL == pParam)
        return FALSE;
    else
    {
        pstRT710->Init_Param.pCur_Broadcast_type = pParam->pCur_Broadcast_type;
        pstRT710->Init_Param.pstDemodtab = pParam->pstDemodtab;
    }
    printf("%s Tuner Init\n", VERSION);


         //   printf(" MDrv_Tuner_SetFreq Freq %d, ucBw %d \n",dwFreq,ucBw);
    pstRT710->Info_Msg.RT710_LoopThrough_Mode = LOOP_THROUGH; //Set LoopThrough ON
    pstRT710->Info_Msg.RT710_ClockOut_Mode = ClockOutOn; //Set Clock Out
    pstRT710->Info_Msg.RT710_OutputSignal_Mode = DifferentialOut;
    pstRT710->Info_Msg.RT710_AGC_Mode = AGC_Negative;
    pstRT710->Info_Msg.RT710_AttenVga_Mode = ATTENVGAOFF;
    pstRT710->Info_Msg.R710_FineGain = FINEGAIN_3DB;
    pstRT710->Info_Msg.RT710_Scan_Mode = RT710_MANUAL_SCAN;

    RT710_Write_Len.RegAddr=0x00;
    RT710_Write_Len.Len=0x10;

    RT710_Read_Len.RegAddr=0x00;
    RT710_Read_Len.Len=0x04;

    if(pstRT710->Initial_done_flag == FALSE)
    {
        pstRT710->RT710_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex_RT710", MSOS_PROCESS_SHARED);
        if(pstRT710->RT710_Mutex < 0)
        {
            TUNER_ERR(("creat RT710 Mutex error!!!\n"));
            return FALSE;
        }
        pstRT710->pre_satellite_bw = 0; //Init BW Value
        pstRT710->satellite_bw = 0;

        I2C_Read_Len(u8TunerIndex,&RT710_Read_Len) ; // read data length
        if((RT710_Read_Len.Data[3]&0xF0)==0x70) //TRUE:RT710(R3[7:4]=7) ; FALSE;RT720(R3[7:4]=0)
            pstRT710->Chip_type_Is_RT710=TRUE;
        else
            pstRT710->Chip_type_Is_RT710=FALSE;


        if(pstRT710->Chip_type_Is_RT710 == TRUE)//TRUE:RT710 ; FALSE;RT720
        {
            for(icount=0;icount<RT710_Reg_Num;icount++)
            {
                pstRT710->Reg_Arry[icount]=RT710_Reg_Arry_Init[icount];
            }
        }
        else
        {
            for(icount=0;icount<RT710_Reg_Num;icount++)
            {
                pstRT710->Reg_Arry[icount]=RT720_Reg_Arry_Init[icount];
            }
        }


        // LOOP_THROUGH(0=on ; 1=off)
        if(pstRT710->Info_Msg.RT710_LoopThrough_Mode != LOOP_THROUGH)
        {
            pstRT710->Reg_Arry[1] |=0x04;
        }
        else
        {
            pstRT710->Reg_Arry[1] &=0xFB;
        }

        //Clock out(1=off ; 0=on)
        if(pstRT710->Info_Msg.RT710_ClockOut_Mode != ClockOutOn)
        {
            pstRT710->Reg_Arry[3] |=0x10;
        }
        else
        {
            pstRT710->Reg_Arry[3] &=0xEF;
        }

        //Output Signal Mode
        if(pstRT710->Info_Msg.RT710_OutputSignal_Mode != DifferentialOut)
        {
            pstRT710->Reg_Arry[11] |=0x10;
        }
        else
        {
            pstRT710->Reg_Arry[11] &=0xEF;
        }

        //AGC Type  //R13[4] Negative=0 ; Positive=1;
        if(pstRT710->Info_Msg.RT710_AGC_Mode != AGC_Positive)
        {
            pstRT710->Reg_Arry[13] &=0xEF;
        }
        else
        {
            pstRT710->Reg_Arry[13] |=0x10;
        }

        //RT710_Vga_Sttenuator_Type
        if(pstRT710->Info_Msg.RT710_AttenVga_Mode != ATTENVGAON)
        {
            pstRT710->Reg_Arry[11] &= 0xF7;
        }
        else
        {
            pstRT710->Reg_Arry[11] |= 0x08;
        }


        if(pstRT710->Chip_type_Is_RT710 == TRUE)//TRUE:RT710 ; FALSE;RT720
        {
            pstRT710->Reg_Arry[14] = (pstRT710->Reg_Arry[14] & 0xFC) | pstRT710->Info_Msg.R710_FineGain;
        }
        else
        {
            if( pstRT710->Info_Msg.R710_FineGain > 1)
                pstRT710->Reg_Arry[14] = (pstRT710->Reg_Arry[14] & 0xFE) | 0x01;
            else
                pstRT710->Reg_Arry[14] = (pstRT710->Reg_Arry[14] & 0xFE) | 0x00;


            pstRT710->Reg_Arry[3] = ((pstRT710->Reg_Arry[3] & 0xF0) | ((30 - Xtal_cap)>>1));
        }


        for(i=0;i<RT710_Write_Len.Len;i++)
        {
            RT710_Write_Len.Data[i]=pstRT710->Reg_Arry[i];
        }

        if(I2C_Write_Len(u8TunerIndex,&RT710_Write_Len) != RT_Success)
        {
            MsOS_DeleteMutex(pstRT710->RT710_Mutex);
            pstRT710->RT710_Mutex = -1;
            return FALSE;
        }

        pstRT710->Initial_done_flag = TRUE;
    }

    return TRUE;
}


MS_BOOL MDrv_Tuner_RT710_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 dwFreq, MS_U32 u32SymbolRate_kHz)
{
    RT710_Handle* pstRT710= NULL;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    if (!pstRT710->Initial_done_flag)
    {
        TUNER_ERR(("[%s]RT710(RT720) have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pstRT710->RT710_Mutex, RT710_MUTEX_TIMEOUT) == FALSE)
    {
        TUNER_ERR(("[%s]RT710(RT720) Obtain mutex failed !!!\n",__FUNCTION__));
        return FALSE;
    }

 //   printf(" MDrv_Tuner_SetFreq Freq %d, ucBw %d \n",dwFreq,ucBw);
    pstRT710->Info_Msg.RF_KHz = dwFreq*1000; // Set RF Freq. unit: KHz
    pstRT710->Info_Msg.SymbolRate_KHz= u32SymbolRate_kHz; // Set BW Freq. unit: KHz, assume roll-off factor = 0.35
    pstRT710->Info_Msg.RT710_RollOff_Mode = ROLL_OFF_0_35;

    if(RT710_Setting(u8TunerIndex) == RT_Success)
    {
        MsOS_ReleaseMutex(pstRT710->RT710_Mutex);
        return TRUE;
    }

    MsOS_ReleaseMutex(pstRT710->RT710_Mutex);
    return FALSE;
}

MS_BOOL MDrv_Tuner_RT710_GetLock(MS_U8 u8TunerIndex)
{
    RT710_Handle* pstRT710= NULL;
    MS_BOOL bret;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;


    if (!pstRT710->Initial_done_flag)
    {
        TUNER_ERR(("[%s]RT710(RT720) have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pstRT710->RT710_Mutex, RT710_MUTEX_TIMEOUT) == FALSE)
    {
        TUNER_ERR(("[%s]RT710(RT720) Obtain mutex failed !!!\n",__FUNCTION__));
        return FALSE;
    }

    bret = RT710_PLL_Lock(u8TunerIndex);
    MsOS_ReleaseMutex(pstRT710->RT710_Mutex);

    return bret;
}

#define RT710_CHIP_ID 0x96
MS_BOOL MDrv_Tuner_RT710_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    UINT8 regData = 0;
    I2C_LEN_TYPE RT710_Read_Len;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_IIC_PORT ePort;
    MS_U8 u8I2C_Port = 0, u8MaxI2CPort, i = 0, j;
    RT710_Handle* pstRT710= NULL;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    ePort = getI2CPort(u8TunerIndex);
    if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
    {
        u8I2C_Port = (MS_U8)ePort/8;
    }
    else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
    {
       u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
    }

    if(pstRT710_slave_ID_TBL == NULL)
    {
        pstRT710_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(u8MaxI2CPort * sizeof(RT710_possible_slave_ID));
        if(NULL == pstRT710_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(RT710_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstRT710_slave_ID_TBL + i*sizeof(RT710_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &RT710_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                }
            }
        }
    }


    if(!_RT710_Handle_alloc())
        return FALSE;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    i=0;

    do
    {
        pSlaveIDTBL = pstRT710_slave_ID_TBL + u8I2C_Port*sizeof(RT710_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        if(pSlaveIDTBL->bInUse)
        {
            TUNER_DBG(("[RT710(RT720)]I2C Slave ID 0x%x Have Used on the same I2C Port\n", pSlaveIDTBL->u8SlaveID));
        }
        else if((pSlaveIDTBL->u8SlaveID) == 0xFF)
        {
            break;
        }
        else
        {
            pstRT710->I2C_SalveID = pSlaveIDTBL->u8SlaveID;
            RT710_Read_Len.RegAddr=0x00;
            RT710_Read_Len.Len=1;
            if(I2C_Read_Len(u8TunerIndex,&RT710_Read_Len) != RT_Success)
            {
                TUNER_ERR(("[RT710(RT720)] Read chip ID fail with slave ID 0x%x\n", pstRT710->I2C_SalveID));
            }
            else
            {
               TUNER_DBG(("[RT710(RT720)] Read chip ID OK with slave ID 0x%x\n", pstRT710->I2C_SalveID));
               regData = RT710_Read_Len.Data[0];
               break;
            }
        }
        i++;
    }while((pSlaveIDTBL->u8SlaveID) != 0xFF);

    if(regData == RT710_CHIP_ID)
    {
        u8RT710_Cnt++;
        if(!bUnderExtDMDTest)
            pSlaveIDTBL->bInUse = TRUE;

        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
    }
    else
    {
       if(!u8RT710_Cnt)
           _RT710_Handle_free();
    }
    return (regData == RT710_CHIP_ID);

}


MS_BOOL RT710_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_SAT_PARAM* SAT_PARAM;
    MS_BOOL bret = TRUE, bEnable;
    RT710_Handle* pstRT710= NULL;
    int RfLevelDbm;
    MS_U16 u16IF_AGC_OUT=0;
    UINT8 RfGainflag;

    if(NULL == pstRT710_Handle)
        return FALSE;
    else
        pstRT710 = pstRT710_Handle + u8TunerIndex;

    switch(fuction_type)
    {
         case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            if(pstRT710->Init_Param.pstDemodtab->data == DEMOD_MSB1245)
            {
                pstRT710->Init_Param.pstDemodtab->Extension_Function(u8TunerIndex, DEMOD_EXT_FUNC_GET_IFAGC_OUT, &u16IF_AGC_OUT);
                if( RT_Success != RT710_Mstar_GetRfRssi(u8TunerIndex, pstRT710->Info_Msg.RF_KHz, (unsigned long)u16IF_AGC_OUT, (int*)(data), &RfGainflag))
                    bret = FALSE;
            }
 #if defined(CHIP_KAYLA) || defined(CHIP_KRIS)
            else if(pstRT710->Init_Param.pstDemodtab->data == DEMOD_MSINTERN_DVBS)
            {
                pstRT710->Init_Param.pstDemodtab->Extension_Function(u8TunerIndex, DEMOD_EXT_FUNC_GET_IFAGC_OUT, &u16IF_AGC_OUT);
                if( RT_Success != RT710_Mstar_GetRfRssi(u8TunerIndex, pstRT710->Info_Msg.RF_KHz, (unsigned long)u16IF_AGC_OUT, (int*)(data), &RfGainflag))
                    bret = FALSE;
            }
 #endif
            else
            {
                if( RT_Success != RT710_GetRfRssi(u8TunerIndex, pstRT710->Info_Msg.RF_KHz, (int*)(data), &RfGainflag))
                    bret = FALSE;
            }
            *(int*)(data)/=1000;
            break;

         case TUNER_EXT_FUNC_GET_RFAGC_STATUS:
            if(pstRT710->Init_Param.pstDemodtab->data == DEMOD_MSB1245)
            {
                pstRT710->Init_Param.pstDemodtab->Extension_Function(u8TunerIndex, DEMOD_EXT_FUNC_GET_IFAGC_OUT, &u16IF_AGC_OUT);
                if( RT_Success != RT710_Mstar_GetRfRssi(u8TunerIndex, pstRT710->Info_Msg.RF_KHz, (UINT32)u16IF_AGC_OUT, &RfLevelDbm, (UINT8*)(data)))
                    bret = FALSE;

            }
            else
            {
                if( RT_Success != RT710_GetRfRssi(u8TunerIndex, pstRT710->Info_Msg.RF_KHz, &RfLevelDbm, (UINT8*)(data)))
                    bret = FALSE;
            }

            break;

         case TUNER_EXT_FUNC_BLIND_SCAN_MODE:
             bEnable = *((MS_BOOL*)data);
             if(bEnable)
                pstRT710->Info_Msg.RT710_Scan_Mode = RT710_AUTO_SCAN;
             else
                pstRT710->Info_Msg.RT710_Scan_Mode = RT710_MANUAL_SCAN;

             break;
         case TUNER_EXT_FUNC_FINALIZE:
             if(pstRT710->RT710_Mutex >= 0)
             {
                 bret &= MsOS_DeleteMutex(pstRT710->RT710_Mutex);
                 if(bret)
                     pstRT710->RT710_Mutex = -1;
             }

             pstRT710->Initial_done_flag = FALSE;
            break;
         case TUNER_EXT_FUNC_DECIDE_LNB_LO:
            SAT_PARAM = data;
            bret &= _RT710_Decide_LNB_LO(SAT_PARAM);
            break;
         case TUNER_EXT_FUNC_UNDER_EXT_DMD_TEST:
            bUnderExtDMDTest = *(MS_BOOL*)data;
            break;
         default:
            break;
    }
    return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_RT710) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_RT710(RT720)",          // demod system name
    .data               = TUNER_RT710,            // private data value
    .Init               = MDrv_Tuner_RT710_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_RT710_SetFreq_S2,
    .GetLock            = MDrv_Tuner_RT710_GetLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_RT710_CheckExist,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .Extension_Function = RT710_Extension_Function
};
#endif

