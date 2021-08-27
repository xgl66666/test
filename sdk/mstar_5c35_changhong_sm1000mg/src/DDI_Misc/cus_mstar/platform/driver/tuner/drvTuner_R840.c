//-----------------------------------------------------//
//
// Filename: R836.c
//
// This file is R836 tuner driver
// Copyright 2013 by Rafaelmicro., Inc.
// Author: Jason Wang
//-----------------------------------------------------//

#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_R836)

#include "MsCommon.h"
#include "drvIIC.h"
#include "drvTuner.h"
#include "drvTuner_R840.h"
#include "MsOS.h"
#include "Frontend_Enum_Device.h"
#include "drvTunerNull.h"

//#ifdef DVBC_STYLE
//MS_U8 dmdConfig[] = {0x00};
//#endif
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];

UINT32 ADC_READ_DELAY = 3;
UINT8  ADC_READ_COUNT = 1;

UINT8 R840_iniArray[R840_REG_NUM] = {0x02, 0x44, 0x40, 0x40, 0x04,      
         0x69, 0x17, 0x75, 0x40, 0x40, 0x06, 0x08, 0x89, 0xA4,   //R7[7]=0, clk out 
         0x95, 0x9C, 0x63, 0x02, 0x72, 0x00, 0x0B, 0xAA, 0xAA,  
         0x28, 0x30, 0x40, 0x87, 0xC2, 0x25, 0x30, 0x26, 0x47};

UINT8 R840_ADDRESS = 0x74;
UINT8 R840_XtalDiv = XTAL_DIV2;
UINT8 R840_SetTfType = R840_TF_BEAD;   //Depend on HW
UINT8 R840_Fil_Cal_Gap = 8;
UINT32 R840_IF_HIGH = 8500;
UINT8* pR840_Xtal_Pwr = NULL;
UINT8* pR840_Xtal_Pwr_tmp = NULL;

//----------------------------------------------------------//
//                   Internal Parameters                    //
//----------------------------------------------------------//

UINT8* pR840_Array  = NULL;

R840_SectType R840_IMR_Data[5] = {
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0}
                                                };//Please keep this array data for standby mode.
I2C_TYPE  R840_I2C;
I2C_LEN_TYPE R840_I2C_Len;

UINT8  R840_IMR_point_num;
UINT8*  pR840_Initial_done_flag = NULL;
UINT8*  pR840_IMR_done_flag = NULL;
UINT8  R840_Bandwidth = 0x00;
UINT8* pR840_Fil_Cal_flag = NULL;
UINT8* pR840_Fil_Cal_BW = NULL;
UINT8* pR840_Fil_Cal_code = NULL;
R840_Standard_Type*  pR840_pre_standard= NULL;

static UINT8 R840_IMR_Cal_Type = R840_IMR_CAL;

//0: L270n/68n(ISDB-T, DVB-T/T2)
//1: Bead/68n(DTMB)
//2: L270n/68n(N/A)
//3: L270n/68n(ATV)
//4: Bead/68n(DTMB+ATV)
//5: L270n/68n(ATSC, DVB-C, J83B)
//6: Bead/68n(ATSC, DVB-C, J83B)
UINT32 R840_LNA_HIGH_MID[R840_TF_SIZE] = { 644000, 644000, 644000, 644000, 644000, 500000, 500000}; 
UINT32 R840_LNA_MID_LOW[R840_TF_SIZE] = { 388000, 388000, 348000, 348000, 348000, 300000, 300000};
UINT32 R840_LNA_LOW_LOWEST[R840_TF_SIZE] = {164000, 164000, 148000, 124000, 124000, 156000, 156000};

UINT32 R840_TF_Freq_High[R840_TF_SIZE][R840_TF_HIGH_NUM] =
{  	{ 784000, 776000, 744000, 712000, 680000, 648000, 647000 },
     { 784000, 776000, 744000, 712000, 680000, 648000, 647000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 647000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 647000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 647000},
       { 784000, 776000, 680000, 608000, 584000, 536000, 504000},
     { 784000, 776000, 680000, 608000, 584000, 536000, 504000}
};
UINT32 R840_TF_Freq_Mid[R840_TF_SIZE][R840_TF_MID_NUM] =
{	  {608000, 584000, 560000, 536000, 488000, 440000, 416000, 392000},
	  {608000, 584000, 560000, 536000, 488000, 440000, 416000, 392000},
	  {608000, 560000, 536000, 488000, 440000, 392000, 376000, 352000},
	  {608000, 560000, 536000, 488000, 440000, 392000, 376000, 352000},
	  {608000, 560000, 536000, 488000, 440000, 392000, 376000, 352000},
      {488000, 464000, 440000, 416000, 392000, 352000, 320000, 304000},
	  {488000, 464000, 440000, 416000, 392000, 352000, 320000, 304000}
};
UINT32 R840_TF_Freq_Low[R840_TF_SIZE][R840_TF_LOW_NUM] =
{    {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
      {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
	  {256000, 240000, 232000, 224000, 216000, 192000, 168000, 160000},  //148~348
	  {256000, 240000, 232000, 192000, 160000, 152000, 144000, 128000},  //124~348
	  {264000, 240000, 192000, 184000, 176000, 152000, 144000, 128000},  //124~348
      {280000, 248000, 232000, 216000, 192000, 176000, 168000, 160000},  //156~300
      {280000, 248000, 232000, 216000, 192000, 176000, 168000, 160000}   //156~300
};
UINT32 R840_TF_Freq_Lowest[R840_TF_SIZE][R840_TF_LOWEST_NUM] =
{    {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
      {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
	  {144000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
	  {120000, 96000,   88000,   80000, 72000, 64000, 56000, 48000},
	  {104000, 96000,   88000,   80000, 72000, 64000, 56000, 48000},
	  {136000, 120000, 104000, 88000, 72000, 64000, 56000, 48000},
	  {128000, 120000, 104000, 96000, 80000, 72000, 64000, 56000}
};


UINT8 R840_TF_Result_High[R840_TF_SIZE][R840_TF_HIGH_NUM] =
{    {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x07},
	  {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x07},
      {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x07},
	  {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x07},
	  {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x07},
      {0x00, 0x01, 0x05, 0x0A, 0x0C, 0x13, 0x19},
	  {0x00, 0x01, 0x05, 0x0A, 0x0C, 0x13, 0x19}
};
UINT8 R840_TF_Result_Mid[R840_TF_SIZE][R840_TF_MID_NUM] =
{    {0x00, 0x01, 0x03, 0x03, 0x06, 0x0B, 0x0E, 0x11},
      {0x00, 0x01, 0x03, 0x03, 0x06, 0x0B, 0x0E, 0x11},
	  {0x00, 0x03, 0x03, 0x06, 0x0B, 0x11, 0x12, 0x19},
	  {0x00, 0x03, 0x03, 0x06, 0x0B, 0x11, 0x12, 0x19},
	  {0x00, 0x03, 0x03, 0x06, 0x0B, 0x11, 0x12, 0x19},
	  {0x06, 0x08, 0x0B, 0x0E, 0x13, 0x17, 0x1E, 0x1F},
      {0x06, 0x08, 0x0B, 0x0E, 0x13, 0x17, 0x1E, 0x1F}
};
UINT8 R840_TF_Result_Low[R840_TF_SIZE][R840_TF_LOW_NUM] =
{    {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0B, 0x0F, 0x16},
      {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0B, 0x0F, 0x16},
	  {0x05, 0x07, 0x0A, 0x0B, 0x0B, 0x0F, 0x16, 0x1A},
	  {0x05, 0x07, 0x0A, 0x0F, 0x1A, 0x1A, 0x23, 0x2A},
	  {0x05, 0x08, 0x10, 0x13, 0x1A, 0x1A, 0x23, 0x2A},
	  {0x05, 0x08, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1A},
	  {0x05, 0x08, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1A},
};
UINT8 R840_TF_Result_Lowest[R840_TF_SIZE][R840_TF_LOWEST_NUM] =
{    {0x00, 0x06, 0x0C, 0x15, 0x1C, 0x1F, 0x3C, 0x3F},
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08},
	  {0x02, 0x06, 0x0C, 0x15, 0x1C, 0x1F, 0x3C, 0x3F},
	  {0x06, 0x11, 0x15, 0x1C, 0x1F, 0x2F, 0x3C, 0x3F},
      {0x04, 0x08, 0x08, 0x08, 0x10, 0x12, 0x13, 0x13},
	  {0x06, 0x09, 0x0E, 0x18, 0x25, 0x2F, 0x3C, 0x3F},
	  {0x00, 0x04, 0x04, 0x08, 0x08, 0x10, 0x12, 0x13}
};

UINT8  R840_TF = 0;
//----------------------------------------------------------//
//                   Internal static struct                         //
//----------------------------------------------------------//
SysFreq_Info_Type* pSysFreq_Info1 = NULL;
Sys_Info_Type*     pSys_Info1 = NULL;
Freq_Info_Type*    pFreq_Info1 = NULL;

//----------------------------------------------------------//
//                   Internal Functions                            //
//----------------------------------------------------------//
R840_ErrCode R840_Xtal_Check(MS_U8 u8TunerIndex);
R840_ErrCode R840_InitReg(MS_U8 u8TunerIndex);
R840_ErrCode R840_Cal_Prepare(MS_U8 u8TunerIndex,UINT8 u1CalFlag);
R840_ErrCode R840_IMR(MS_U8 u8TunerIndex,UINT8 IMR_MEM, MS_BOOL IM_Flag);
R840_ErrCode R840_PLL(MS_U8 u8TunerIndex,UINT32 LO_Freq, R840_Standard_Type R840_Standard);
R840_ErrCode R840_MUX(MS_U8 u8TunerIndex,UINT32 LO_KHz, UINT32 RF_KHz, R840_Standard_Type R840_Standard);
R840_ErrCode R840_IQ(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont);
R840_ErrCode R840_IQ_Tree(MS_U8 u8TunerIndex,UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree);
R840_ErrCode R840_CompreCor(R840_SectType* CorArry);
R840_ErrCode R840_CompreStep(MS_U8 u8TunerIndex,R840_SectType* StepArry, UINT8 Pace);
R840_ErrCode R840_Muti_Read(MS_U8 u8TunerIndex,UINT8* IMR_Result_Data);
R840_ErrCode R840_Section(MS_U8 u8TunerIndex,R840_SectType* SectionArry);
R840_ErrCode R840_F_IMR(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont);
R840_ErrCode R840_IMR_Cross(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont, UINT8* X_Direct);
R840_ErrCode R840_IMR_Iqcap(MS_U8 u8TunerIndex,R840_SectType* IQ_Point);
R840_ErrCode R840_SetTF(MS_U8 u8TunerIndex,UINT32 u4FreqKHz, UINT8 u1TfType);
R840_ErrCode R840_SetStandard(MS_U8 u8TunerIndex,R840_Standard_Type RT_Standard);
R840_ErrCode R840_SetFrequency(MS_U8 u8TunerIndex,R840_Set_Info R840_INFO);

Sys_Info_Type R840_Sys_Sel(R840_Standard_Type R840_Standard);
Freq_Info_Type R840_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R840_Standard_Type R840_Standard);
SysFreq_Info_Type R840_SysFreq_Sel(R840_Standard_Type R840_Standard,UINT32 RF_freq);

UINT8 R840_Filt_Cal_ADC(MS_U8 u8TunerIndex,UINT32 IF_Freq, UINT8 R840_BW, UINT8 FilCal_Gap);

//--------------------------------------------------------------
//           #define R840_DEBUG
//--------------------------------------------------------------
static MS_BOOL R836_Variables_alloc(void)
{

    UINT8 i;
    
       if(NULL == pSysFreq_Info1)
           pSysFreq_Info1 = (SysFreq_Info_Type*)malloc(sizeof(SysFreq_Info_Type)*MAX_FRONTEND_NUM);

       if(NULL == pSys_Info1)
           pSys_Info1 = (Sys_Info_Type*)malloc(sizeof(Sys_Info_Type)*MAX_FRONTEND_NUM);

       if(NULL == pFreq_Info1)
           pFreq_Info1 = (Freq_Info_Type*)malloc(sizeof(Freq_Info_Type)*MAX_FRONTEND_NUM);

       if(NULL == pR840_Xtal_Pwr)
           pR840_Xtal_Pwr = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);

       if(NULL == pR840_Xtal_Pwr_tmp)
           pR840_Xtal_Pwr_tmp = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);

       if(NULL == pR840_Initial_done_flag)
           pR840_Initial_done_flag = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);

       if(NULL == pR840_IMR_done_flag)
           pR840_IMR_done_flag = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);

       if(NULL == pR840_Fil_Cal_flag)
           pR840_Fil_Cal_flag = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_STD_SIZE);

       if(NULL == pR840_Fil_Cal_BW)
           pR840_Fil_Cal_BW = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_STD_SIZE);

       if(NULL == pR840_Fil_Cal_code)
           pR840_Fil_Cal_code = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_STD_SIZE);

       if(NULL == pR840_pre_standard)
           pR840_pre_standard  = (R840_Standard_Type*)malloc(sizeof(R840_Standard_Type)*MAX_FRONTEND_NUM);

       if(NULL == pR840_Array)
           pR840_Array = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_REG_NUM);

       if((NULL == pSysFreq_Info1) || (NULL == pSys_Info1) || (NULL == pSysFreq_Info1) ||\
           (NULL == pR840_Xtal_Pwr) || (NULL == pR840_Xtal_Pwr_tmp) || (NULL == pR840_Initial_done_flag) ||\
           (NULL == pR840_IMR_done_flag) || (NULL == pR840_Fil_Cal_flag) || (NULL == pR840_Fil_Cal_BW) ||\
           (NULL == pR840_Fil_Cal_code) || (NULL == pR840_pre_standard) || (NULL == pR840_Array))
        {
            if(NULL != pSysFreq_Info1)
               free(pSysFreq_Info1);
            
            if(NULL != pSys_Info1)
               free(pSys_Info1);
            
            if(NULL != pSysFreq_Info1)
               free(pSysFreq_Info1);
            
            if(NULL != pR840_Xtal_Pwr)
               free(pR840_Xtal_Pwr);
            
            if(NULL != pR840_Xtal_Pwr_tmp)
               free(pR840_Xtal_Pwr_tmp);
            
            if(NULL != pR840_Initial_done_flag)
               free(pR840_Initial_done_flag);
            
            if(NULL != pR840_IMR_done_flag)
               free(pR840_IMR_done_flag);
            
             if(NULL != pR840_Fil_Cal_flag)
               free(pR840_Fil_Cal_flag);
             
             if(NULL != pR840_Fil_Cal_BW)
               free(pR840_Fil_Cal_BW);
             
             if(NULL != pR840_Fil_Cal_code)
               free(pR840_Fil_Cal_code);
             
             if(NULL != pR840_pre_standard)
               free(pR840_pre_standard);
             
             if(NULL != pR840_Array)
               free(pR840_Array); 
             
           return FALSE;
        }
        else
        {
           for(i=0;i<MAX_FRONTEND_NUM;i++)
           {
              *(pR840_Xtal_Pwr + i) = XTAL_NOR_LOWEST;
              *(pR840_Xtal_Pwr_tmp + i) = XTAL_NOR_LOWEST;
              *(pR840_Initial_done_flag + i) = FALSE;
              *(pR840_IMR_done_flag + i) = FALSE;
           }
           return TRUE;
       }
}


//==============================IIC Operation===================================

int R836_Convert(int InvertNum)
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
static MS_BOOL IIC_READ(MS_U8 u8TunerIndex,MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{
    HWI2C_PORT ePort;

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
    MS_U8 u8SlaveID = R840_ADDRESS;
    MS_U8 regadd = 0x00;
    MS_U8 *paddr = &regadd;
    MS_U16 u16size  = I2C_Info->Len;
    MS_U8 pu8data[50];

    if (FALSE == IIC_READ(u8TunerIndex,u8SlaveID, paddr, 1, pu8data, u16size))
    {
        TUNER_ERR(("IIC Read Len error\n"));
        return FALSE;
    }
    
    for (DataCunt = 0; DataCunt < u16size; DataCunt ++)
    {
        I2C_Info->Data[DataCunt] = R836_Convert(pu8data[DataCunt]);
    }

    return TRUE;
}

static MS_BOOL I2C_Write(MS_U8 u8TunerIndex,I2C_TYPE *I2C_Info)
{
    MS_U8 u8SlaveID = R840_ADDRESS;
    MS_U16 u16size  = 1;
    MS_U8 *paddr = &(I2C_Info->RegAddr);
    MS_U8 *pu8data = &(I2C_Info->Data);
    HWI2C_PORT ePort;

    ePort = getI2CPort(u8TunerIndex);   
    if (FALSE == MDrv_IIC_WriteBytes(ePort, u8SlaveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("devCOFDM_PassThroughIIC_WRITEBytes Error \n"));
        return FALSE;
    }

    R840_Delay_MS(1); // delay 1ms

    return TRUE;
}

MS_BOOL I2C_Write_Len(MS_U8 u8TunerIndex,I2C_LEN_TYPE *I2C_Info)
{
    MS_U8 u8SlaveID = R840_ADDRESS;
    MS_U16 u16size  = I2C_Info->Len;
    MS_U8 *paddr = &(I2C_Info->RegAddr);
    MS_U8 *pu8data = I2C_Info->Data;
    HWI2C_PORT ePort;

    ePort = getI2CPort(u8TunerIndex);  
    if (FALSE == MDrv_IIC_WriteBytes(ePort, u8SlaveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("devCOFDM_PassThroughIIC_WRITEBytes Error \n"));
        return FALSE;
    }

    R840_Delay_MS(1); // delay 1ms

    return TRUE;
}
//=================================End============================================


Sys_Info_Type R840_Sys_Sel(R840_Standard_Type R840_Standard)
{
    Sys_Info_Type R840_Sys_Info;

    switch (R840_Standard)
    {

        case R840_DVB_T_6M:
        case R840_DVB_T2_6M:
            R840_Sys_Info.IF_KHz=4570;                    //IF
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7450;          //CAL IF
            R840_Sys_Info.HPF_COR=0x06;             //R11[3:0]=6 (1.78M)
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
            break;
            
       case R840_DVB_T_7M:
       case R840_DVB_T2_7M:
           R840_Sys_Info.IF_KHz=4570;                     //IF
           R840_Sys_Info.BW=0x40;                           //BW=7M
           R840_Sys_Info.FILT_CAL_IF=7750;           //CAL IF
           R840_Sys_Info.HPF_COR=0x08;	             //R11[3:0]=8 (1.45M)
           R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
           R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8
           break;
                
        case R840_DVB_T_8M:
        case R840_DVB_T2_8M:
            R840_Sys_Info.IF_KHz=4570;                     //IF
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF
            R840_Sys_Info.HPF_COR=0x0B;             //R11[3:0]=11 (0.79M)
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
            break;
        
        case R840_DVB_T2_1_7M:
            R840_Sys_Info.IF_KHz=1900;
            R840_Sys_Info.BW=0x40;                           //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7800;           //CAL IF
            R840_Sys_Info.HPF_COR=0x08;             //R11[3:0]=8
            R840_Sys_Info.FILT_EXT_ENA=0x02;      //R11[4]=0, ext disable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+1
            break;
            
        case R840_DVB_T2_10M:
            R840_Sys_Info.IF_KHz=5600;
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=10800;         //CAL IF
            R840_Sys_Info.HPF_COR=0x0C;             //R11[3:0]=12
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+1
            break;
            
        case R840_DVB_C_8M:
        case R840_DVB_C_8M_W_DTMB:
            R840_Sys_Info.IF_KHz=5070;
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=9000;           //CAL IF
            R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+3
            break;

       case R840_DVB_C_6M:
           R840_Sys_Info.IF_KHz=5070;
           R840_Sys_Info.BW=0x40;                          //BW=7M
           R840_Sys_Info.FILT_CAL_IF=8025;          //CAL IF   
           R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
           R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
           R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+1
           break;

       case R840_J83B:
           R840_Sys_Info.IF_KHz=5070;
           R840_Sys_Info.BW=0x40;                          //BW=7M
           R840_Sys_Info.FILT_CAL_IF=8025;          //CAL IF  
           R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
           R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
           R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
           break;
            
        case R840_ISDB_T:
            R840_Sys_Info.IF_KHz=4063;
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7000;          //CAL IF  //7200
            R840_Sys_Info.HPF_COR=0x08;             //R11[3:0]=8
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
            break;
            
        case R840_ISDB_T_4570:
            R840_Sys_Info.IF_KHz=4570;                    //IF
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7250;          //CAL IF
            R840_Sys_Info.HPF_COR=0x05;	             //R11[3:0]=5 (2.0M)
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
            break;
        
     case R840_DTMB_4570:
            R840_Sys_Info.IF_KHz=4570;
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF  //8130
            R840_Sys_Info.HPF_COR=0x0B;             //R11[3:0]=11
            R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;  //R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
            break;

     case R840_DTMB_6000:
            R840_Sys_Info.IF_KHz=6000;
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=9550;           //CAL IF
            R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+3
            break;

     case R840_DTMB_6M_BW_IF_5M:
           R840_Sys_Info.IF_KHz=5000;
           R840_Sys_Info.BW=0x40;                           //BW=7M
           R840_Sys_Info.FILT_CAL_IF=7700;           //CAL IF  
           R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
           R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
           R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
           break;
     
     case R840_DTMB_6M_BW_IF_4500:
          R840_Sys_Info.IF_KHz=4500;
          R840_Sys_Info.BW=0x40;                           //BW=7M
          R840_Sys_Info.FILT_CAL_IF=7000;           //CAL IF  
          R840_Sys_Info.HPF_COR=0x05;	             //R11[3:0]=5
          R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
          R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
          R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+3
          break;

    case R840_ATSC:
          R840_Sys_Info.IF_KHz=5070;
          R840_Sys_Info.BW=0x40;                          //BW=7M
          R840_Sys_Info.FILT_CAL_IF=7900;          //CAL IF   
          R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
          R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
          R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
          R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
          break;
    
    case R840_DVB_T_6M_IF_5M:
    case R840_DVB_T2_6M_IF_5M:
         R840_Sys_Info.IF_KHz=5000;                    //IF
         R840_Sys_Info.BW=0x40;                          //BW=7M
         R840_Sys_Info.FILT_CAL_IF=7800;          //CAL IF
         R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4 (2.25M)
         R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
         R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
         R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
         break;

   case R840_DVB_T_7M_IF_5M:
   case R840_DVB_T2_7M_IF_5M:
         R840_Sys_Info.IF_KHz=5000;                     //IF
         R840_Sys_Info.BW=0x00;                           //BW=8M
         R840_Sys_Info.FILT_CAL_IF=8300;           //CAL IF
         R840_Sys_Info.HPF_COR=0x07;	             //R11[3:0]=7 (1.67M)
         R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
         R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
         R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
         break;

   case R840_DVB_T_8M_IF_5M:
   case R840_DVB_T2_8M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;                     //IF
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=8800;           //CAL IF
        R840_Sys_Info.HPF_COR=0x09;	             //R11[3:0]=9 (1.15M)
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
        break;

   case R840_DVB_T2_1_7M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x60;                           //BW=6M
        R840_Sys_Info.FILT_CAL_IF=5900;           //CAL IF
        R840_Sys_Info.HPF_COR=0x01;	             //R11[3:0]=1
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
   
   case R840_DVB_C_8M_IF_5M:
   case R840_DVB_C_8M_W_DTMB_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=9000;           //CAL IF 
        R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+3
        break;
   
   case R840_DVB_C_6M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=8100;          //CAL IF   
        R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
        
   case R840_J83B_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=8025;          //CAL IF  
        R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3 
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
        
   case R840_ISDB_T_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7940;          //CAL IF
        R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4 (2.25M)
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+3
        break;
        
   case R840_DTMB_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=8650;           //CAL IF  
        R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
        
   case R840_ATSC_IF_5M:
		R840_Sys_Info.IF_KHz=5070;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7900;          //CAL IF   
        R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
        
   case R840_FM:
        R840_Sys_Info.IF_KHz=2400;
        R840_Sys_Info.BW=0x40;                           //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7200;           //CAL IF
        R840_Sys_Info.HPF_COR=0x02;	             //R11[3:0]=2
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
        
   default:  //R840_DVB_T_8M
        R840_Sys_Info.IF_KHz=4570;                     //IF
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF
        R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
        R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+1
        break;
        
    }


//Set By DTV/ATV
    if (R840_Standard < R840_ATV_SIZE)  //ATV
    {
        R840_Sys_Info.SWCAP_CLK = 0x02;      //8k       (R18[1:0]=2)
        R840_Sys_Info.NA_PWR_DET = 0x80;    //off         (R30[7]=1)
    }
    else  //other DTV
    {
        R840_Sys_Info.SWCAP_CLK = 0x02;      //8k       (R18[1:0]=2) //AGC 500Hz map to 8k
        R840_Sys_Info.NA_PWR_DET = 0x00;   //on         (R30[7]=0)
    }
    
    R840_Sys_Info.TF_CUR = 0x40;                  //low       (R3[6]=1)
    R840_Sys_Info.SWBUF_CUR = 0x04;          //low  (R4[2]=1)
    R840_Sys_Info.LNA_DET_MODE=0x00;    //normal (R31[0]=0)
    R840_Sys_Info.RF_DISCHARGE = 0x60;    //3          (R31[7:5]=3'b011)
    R840_Sys_Info.NA_DISCHARGE = 0x04;   //1          (R31[4:2]=3'b001)
    R840_Sys_Info.AGC_CLK = 0x00;              //1k        (R27[5:3]=3'b000)
    
    //Filter 3dB
    switch(R840_Standard)
    {
        case R840_DVB_C_8M_IF_5M:
        case R840_DVB_C_8M_W_DTMB_IF_5M:
            R840_Sys_Info.FILT_3DB = 0x08;         // ON       (R30[3]=1)
        break;
        
        default: 
            R840_Sys_Info.FILT_3DB = 0x00;         // OFF      (R30[3]=0)
        break;
    }
    
    R840_Sys_Info.FILT_COMP = 0x20;      //1          (R30[6:5]=2'b01)
    R840_Sys_Info.FILT_CUR = 0x00;         //highest  (R10[6:5]=2'b00)
    
    //BW 1.7M
    if((R840_Standard==R840_DVB_T2_1_7M) || (R840_Standard==R840_FM))
        R840_Sys_Info.V17M = 0x80;       //enable, R11[7]=1
    else
        R840_Sys_Info.V17M = 0x00;       //disable, R11[7]=0, (include DVBT2 1.7M IF=5MHz)
    
    //TF Type select
    switch(R840_Standard)
    {
    
    case R840_DTMB_4570:
    case R840_DTMB_6000:
    case R840_DTMB_6M_BW_IF_5M:
    case R840_DTMB_6M_BW_IF_4500:
    case R840_DTMB_IF_5M:
    case R840_DVB_C_8M_W_DTMB:
    case R840_DVB_C_8M_W_DTMB_IF_5M:
        R840_SetTfType = R840_TF_BEAD;
     break;
    
    case R840_DVB_C_8M:
    case R840_DVB_C_6M:
    case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
    case R840_DVB_C_6M_IF_5M:
    case R840_J83B_IF_5M:
        R840_SetTfType = R840_TF_NARROW_ATSC;
    break;	
    
    case R840_ATSC:  
    case R840_ATSC_IF_5M:  
        R840_SetTfType = R840_TF_NARROW_ATSC;
     break;
    
    default://DVB-T/T2, ISDB-T
        R840_SetTfType = R840_TF_NARROW;
    break;
    }
    
    return R840_Sys_Info;
}



Freq_Info_Type R840_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R840_Standard_Type R840_Standard)
{
    Freq_Info_Type R840_Freq_Info;
    
    //----- RF dependent parameter --------
    //LNA band
    if((RF_freq>=0) && (RF_freq<R840_LNA_LOW_LOWEST[R840_SetTfType]))  //<164
        R840_Freq_Info.LNA_BAND = 0x60;   //R5[6:5]=2'b11; ultra low
    else if((RF_freq>=R840_LNA_LOW_LOWEST[R840_SetTfType]) && (RF_freq<R840_LNA_MID_LOW[R840_SetTfType]))  //164~388
        R840_Freq_Info.LNA_BAND = 0x40;   //R5[6:5]=2'b10; low
    else if((RF_freq>=R840_LNA_MID_LOW[R840_SetTfType]) && (RF_freq<R840_LNA_HIGH_MID[R840_SetTfType]))  //388~612
        R840_Freq_Info.LNA_BAND = 0x20;   //R5[6:5]=2'b01; mid
    else     // >612
        R840_Freq_Info.LNA_BAND = 0x00;   //R5[6:5]=2'b00; high
    
    //----- LO dependent parameter --------
    //IMR point
    if((LO_freq>=0) && (LO_freq<133000))
        R840_Freq_Info.IMR_MEM = 0;
    else if((LO_freq>=133000) && (LO_freq<221000))
        R840_Freq_Info.IMR_MEM = 1;
    else if((LO_freq>=221000) && (LO_freq<450000))
        R840_Freq_Info.IMR_MEM = 2;
    else if((LO_freq>=450000) && (LO_freq<789000))
        R840_Freq_Info.IMR_MEM = 3;
    else
        R840_Freq_Info.IMR_MEM = 4;
    
    //RF polyfilter band
    if((LO_freq>=0) && (LO_freq<133000))
        R840_Freq_Info.RF_POLY = 0x80;   //R25[7:6]=2'b10; low
    else if((LO_freq>=133000) && (LO_freq<221000))
        R840_Freq_Info.RF_POLY = 0x40;   //R25[7:6]=2'b01; mid
    else if((LO_freq>=221000) && (LO_freq<789000))
        R840_Freq_Info.RF_POLY = 0x00;   //R25[7:6]=2'b00; highest
    else
        R840_Freq_Info.RF_POLY = 0xC0;   //R25[7:6]=2'b11; ultra high
    
    
    //LPF Cap, Notch
    switch(R840_Standard)
    {
        case R840_DVB_C_8M:
        case R840_DVB_C_8M_W_DTMB:
        case R840_DVB_C_6M:
        case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
        case R840_DVB_C_8M_W_DTMB_IF_5M:
        case R840_DVB_C_6M_IF_5M:
        case R840_J83B_IF_5M:
            if((LO_freq>=0) && (LO_freq<77000))
            {
                R840_Freq_Info.LPF_CAP = 16;
                R840_Freq_Info.LPF_NOTCH = 10;
            }
            else if((LO_freq>=77000) && (LO_freq<85000))
            {
                R840_Freq_Info.LPF_CAP = 16;
                R840_Freq_Info.LPF_NOTCH = 4;
            }
            else if((LO_freq>=85000) && (LO_freq<115000))
            {
                R840_Freq_Info.LPF_CAP = 13;
                R840_Freq_Info.LPF_NOTCH = 3;
            }
            else if((LO_freq>=115000) && (LO_freq<125000))
            {
                R840_Freq_Info.LPF_CAP = 11;
                R840_Freq_Info.LPF_NOTCH = 1;
            }
            else if((LO_freq>=125000) && (LO_freq<141000))
            {
                R840_Freq_Info.LPF_CAP = 9;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=141000) && (LO_freq<157000))
            {
                R840_Freq_Info.LPF_CAP = 8;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=157000) && (LO_freq<181000))
            {
                R840_Freq_Info.LPF_CAP = 6;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=181000) && (LO_freq<205000))
            {
                R840_Freq_Info.LPF_CAP = 3;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else //LO>=201M
            {
                R840_Freq_Info.LPF_CAP = 0;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            
            break;
    
        default:
            if((LO_freq>=0) && (LO_freq<73000))
            {
                R840_Freq_Info.LPF_CAP = 8;
                R840_Freq_Info.LPF_NOTCH = 10;
            }
            else if((LO_freq>=73000) && (LO_freq<81000))
            {
                R840_Freq_Info.LPF_CAP = 8;
                R840_Freq_Info.LPF_NOTCH = 4;
            }
            else if((LO_freq>=81000) && (LO_freq<89000))
            {
                R840_Freq_Info.LPF_CAP = 8;
                R840_Freq_Info.LPF_NOTCH = 3;
            }
            else if((LO_freq>=89000) && (LO_freq<121000))
            {
                R840_Freq_Info.LPF_CAP = 6;
                R840_Freq_Info.LPF_NOTCH = 1;
            }
            else if((LO_freq>=121000) && (LO_freq<145000))
            {
                R840_Freq_Info.LPF_CAP = 4;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=145000) && (LO_freq<153000))
            {
                R840_Freq_Info.LPF_CAP = 3;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=153000) && (LO_freq<177000))
            {
                R840_Freq_Info.LPF_CAP = 2;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=177000) && (LO_freq<201000))
            {
                R840_Freq_Info.LPF_CAP = 1;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            else //LO>=201M
            {
                R840_Freq_Info.LPF_CAP = 0;
                R840_Freq_Info.LPF_NOTCH = 0;
            }
            
            break;
            
    }//end switch(standard)
    
    return R840_Freq_Info;
    
}



SysFreq_Info_Type R840_SysFreq_Sel(R840_Standard_Type R840_Standard,UINT32 RF_freq)
{
	SysFreq_Info_Type R840_SysFreq_Info;

	switch(R840_Standard)
	{

	case R840_DVB_T_6M:
	case R840_DVB_T_7M:
	case R840_DVB_T_8M:
	case R840_DVB_T_6M_IF_5M:
	case R840_DVB_T_7M_IF_5M:
	case R840_DVB_T_8M_IF_5M:
	case R840_DVB_T2_6M:
	case R840_DVB_T2_7M:
	case R840_DVB_T2_8M:
	case R840_DVB_T2_1_7M:
	case R840_DVB_T2_10M:
    case R840_DVB_T2_6M_IF_5M:
	case R840_DVB_T2_7M_IF_5M:
	case R840_DVB_T2_8M_IF_5M:
	case R840_DVB_T2_1_7M_IF_5M:
		if((RF_freq>=782000) && (RF_freq<=790000))
		{
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0x62;		   // LNA VTH/L=0.94/0.54     (R13=0x62)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x70;            // Nrb TOP=8                       (R28[7:4]=4'b0111)
			R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
		}
		else
		{
		    R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
			R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		}
		break;


	case R840_DVB_C_8M_W_DTMB:  //Bead/68n
	case R840_DVB_C_8M_W_DTMB_IF_5M:  //Bead/68n
			R840_SysFreq_Info.LNA_TOP=0x04;		       // LNA TOP=3                    (R27[2:0]=3'b100)
			R840_SysFreq_Info.LNA_VTH_L=0x93;		   // LNA VTH/L=1.24/0.64     (R13=0x93)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		break;

    case R840_DVB_C_8M:
	case R840_DVB_C_6M:
	case R840_J83B:
	case R840_DVB_C_8M_IF_5M:
	case R840_DVB_C_6M_IF_5M:
	case R840_J83B_IF_5M:
		if(RF_freq<165000)
		{
			R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74      (R13=0x94)
			R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
			R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
		}
		else if((RF_freq>=165000) && (RF_freq<345000))
		{
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
			R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
			R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
		}
		else
		{
			R840_SysFreq_Info.LNA_TOP=0x04;		       // LNA TOP=3                    (R27[2:0]=3'b100)
			R840_SysFreq_Info.LNA_VTH_L=0x83;		   // LNA VTH/L=1.14/0.64     (R13=0x83)
			R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
		}
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		break;

	case R840_ISDB_T:
	case R840_ISDB_T_4570:	
	case R840_ISDB_T_IF_5M:
		if((RF_freq>=690000) && (RF_freq<=700000))
		{
			R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			R840_SysFreq_Info.LNA_VTH_L=0x62;		   // LNA VTH/L=0.94/0.54     (R13=0x62)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
			R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
		}
		else
		{
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		}
		break;

	case R840_DTMB_4570:
	case R840_DTMB_6000:
	case R840_DTMB_6M_BW_IF_5M:
	case R840_DTMB_6M_BW_IF_4500:
	case R840_DTMB_IF_5M:
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		break;

	case R840_ATSC:
	case R840_ATSC_IF_5M:
		    if(RF_freq<88000)
		   {
		       R840_SysFreq_Info.LNA_TOP=0x02;	 	       // LNA TOP=5                    (R27[2:0]=3'b010)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
			   R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
			   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                    (R28[7:4]=4'b1100)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		   }
		   else if((RF_freq>=88000) && (RF_freq<104000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		   }
		   else if((RF_freq>=104000) && (RF_freq<248000))
		   {
			R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)	
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		   }
		   else if((RF_freq>=248000) && (RF_freq<464000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		   }
		   else if((RF_freq>=464000) && (RF_freq<500000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xB6;	       // LNA VTH/L=1.44/0.94     (R13=0xB6)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		   }
		   else
		   {
		       if(((RF_freq>=685000) && (RF_freq<700000)) || ((RF_freq>=795000) && (RF_freq<805000)) )
		   		{
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)		
			   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
		   }
		   else
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)		
			   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   }
		   }
			R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
			//R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		break;

	case R840_FM:
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		break;

	default: //DVB-T 8M
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
		break;

	} //end switch


	//TF LPF setting
	switch(R840_Standard)
	{
		case R840_DTMB_4570:
	    case R840_DTMB_6000:
		case R840_DTMB_6M_BW_IF_5M:
		case R840_DTMB_6M_BW_IF_4500:
		case R840_DTMB_IF_5M:
			 if(RF_freq<=196000)
 	             R840_SysFreq_Info.BYP_LPF = 0x04;      //low pass  (R1[2]=1)
			 else
				 R840_SysFreq_Info.BYP_LPF = 0x00;      //bypass  (R1[2]=0)

		break;

		default:  //other standard
			 if(RF_freq<=236000)
 	             R840_SysFreq_Info.BYP_LPF = 0x04;      //low pass  (R1[2]=1)
			 else
				 R840_SysFreq_Info.BYP_LPF = 0x00;      //bypass  (R1[2]=0)

        break;
	}//end switch

	return R840_SysFreq_Info;

	}

static void R840_SET_REG_Array(MS_U8 u8TunerIndex, UINT8 reg, UINT8 Mask,UINT8 Value)
{
    UINT8 u8tmp;
    
    u8tmp = *(pR840_Array + u8TunerIndex *R840_REG_NUM + reg);
    u8tmp &= Mask;
    u8tmp |= Value;
    *(pR840_Array + u8TunerIndex *R840_REG_NUM + reg) = u8tmp;
}

static UINT8 R840_GET_REG_Array(MS_U8 u8TunerIndex, UINT8 reg)
{
    return *(pR840_Array + u8TunerIndex *R840_REG_NUM + reg);
}


R840_ErrCode R840_Init(MS_U8 u8TunerIndex)
{
    UINT8 i;

    if(*(pR840_Initial_done_flag + u8TunerIndex)==FALSE)
    {
        //do Xtal check
        if(R840_InitReg(u8TunerIndex) != RT_Success)
            return RT_Fail;
       
        *(pR840_Xtal_Pwr + u8TunerIndex) = XTAL_NOR_LOWEST;
        *(pR840_Xtal_Pwr_tmp +u8TunerIndex) = XTAL_NOR_LOWEST;
       
        for (i=0; i<3; i++)
        {
           if(R840_Xtal_Check(u8TunerIndex) != RT_Success)
               return RT_Fail;
       
           if(*(pR840_Xtal_Pwr_tmp +u8TunerIndex) > *(pR840_Xtal_Pwr + u8TunerIndex))
               *(pR840_Xtal_Pwr + u8TunerIndex) = *(pR840_Xtal_Pwr_tmp +u8TunerIndex);
        }
       
       //reset filter cal. data
       for (i=0; i<R840_STD_SIZE; i++)
       {
           *(pR840_Fil_Cal_flag + u8TunerIndex *R840_STD_SIZE +i) = FALSE;
           *(pR840_Fil_Cal_code + u8TunerIndex *R840_STD_SIZE +i) = 0;
           *(pR840_Fil_Cal_BW + u8TunerIndex *R840_STD_SIZE +i) = 0x00;
       }

       if(*(pR840_IMR_done_flag + u8TunerIndex)==FALSE)
       {
         //start IMR calibration
             if(R840_InitReg(u8TunerIndex) != RT_Success)        //write initial reg before doing IMR Cal
                 return RT_Fail;
       
             if(R840_Cal_Prepare(u8TunerIndex,R840_IMR_CAL) != RT_Success)
                 return RT_Fail;
       
             if(R840_IMR(u8TunerIndex,3, TRUE) != RT_Success)       //Full K node 3
                 return RT_Fail;
       
             if(R840_IMR(u8TunerIndex,0, FALSE) != RT_Success)
                 return RT_Fail;
       
             if(R840_IMR(u8TunerIndex,1, FALSE) != RT_Success)
                 return RT_Fail;
        
             if(R840_IMR(u8TunerIndex,2, FALSE) != RT_Success)
                 return RT_Fail;
        
             if(R840_IMR(u8TunerIndex,4, TRUE) != RT_Success)   //Full K node 4
                 return RT_Fail;
             *(pR840_IMR_done_flag + u8TunerIndex) = TRUE;
       }

          *(pR840_Initial_done_flag + u8TunerIndex) = TRUE;

      } //end if(check init flag)

//write initial reg
    if(R840_InitReg(u8TunerIndex) != RT_Success)
        return RT_Fail;

    *(pR840_pre_standard + u8TunerIndex) = R840_STD_SIZE;

    return RT_Success;
}



R840_ErrCode R840_InitReg(MS_U8 u8TunerIndex)
{
    UINT8 InitArrayCunt = 0;
    //Write Full Table, Set Xtal Power = highest at initial
    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = R840_REG_NUM;
    for(InitArrayCunt = 0; InitArrayCunt<R840_REG_NUM; InitArrayCunt ++)
    {
        R840_I2C_Len.Data[InitArrayCunt] = R840_iniArray[InitArrayCunt];
        R840_SET_REG_Array(u8TunerIndex, InitArrayCunt, 0x00,R840_iniArray[InitArrayCunt]);
    }
    if(I2C_Write_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
        return RT_Fail;

    return RT_Success;
}


R840_ErrCode R840_Xtal_Check(MS_U8 u8TunerIndex)
{
    UINT8 i = 0;
    
    //Xtal_pow=lowest  & Xtal_Gm=Normal
    R840_I2C.RegAddr = 0x0F;
    R840_SET_REG_Array(u8TunerIndex, 15, 0x87, 0x60);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;
    
    //set pll autotune = 128kHz
    R840_I2C.RegAddr = 0x1D;
    R840_SET_REG_Array(u8TunerIndex, 29, 0x3F, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 29);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
    	return RT_Fail;
    
    //set manual initial reg = 1 000000; b5=0 => cap 30p;
    R840_I2C.RegAddr = 0x13;
    R840_SET_REG_Array(u8TunerIndex, 19, 0x80, 0x40);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 19);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
    	return RT_Fail;
    
    //set auto
    R840_I2C.RegAddr = 0x13;
    R840_SET_REG_Array(u8TunerIndex, 19, 0xBF, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 19);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
    	return RT_Fail;
    
    R840_Delay_MS(5);
    
    for(i=0; i<XTAL_CHECK_SIZE; i++)
    {
        //set power
        if(i<=XTAL_NOR_HIGHEST)
            R840_SET_REG_Array(u8TunerIndex, 15, 0x87, ((UINT8)(XTAL_NOR_HIGHEST-i)<<5) | 0x00);//normal Gm
        else if(i==XTAL_GM3V_HIGHEST)
            R840_SET_REG_Array(u8TunerIndex, 15, 0x87, 0x18); //3v Gm, highest
        else
            R840_SET_REG_Array(u8TunerIndex, 15, 0x87, 0x18);//3v Gm, highest
        
        R840_I2C.RegAddr = 0x0F;
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,15);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;
        
        R840_Delay_MS(5);
        
        R840_I2C_Len.RegAddr = 0x00;
        R840_I2C_Len.Len = 3;
        if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success)
            return RT_Fail;
        
        //lock to VCO band 8 if VCO=2768M for 16M Xtal
        //lock to VCO band 46 if VCO=2768M for 24M Xtal
#if(R840_Xtal==16000)
    if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 11) && ((R840_I2C_Len.Data[2] & 0x3F) >= 5))  
#else
    if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 48) && ((R840_I2C_Len.Data[2] & 0x3F) >= 42)) 
#endif
     {
         *(pR840_Xtal_Pwr_tmp + u8TunerIndex) = i;
         break;
     }
     
       if(i==(XTAL_CHECK_SIZE-1))
     {
           *(pR840_Xtal_Pwr_tmp + u8TunerIndex) = i;
     }
  }


    return RT_Success;
}

R840_ErrCode R840_Cal_Prepare(MS_U8 u8TunerIndex,UINT8 u1CalFlag)
{
    R840_Cal_Info_Type  Cal_Info;
    
    switch(u1CalFlag)
    {
       case R840_IMR_CAL:
            Cal_Info.FILTER_6DB = 0x08;              //+6dB
            Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
            Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB	OFF
            Cal_Info.MIXER_AMP_GAIN = 0x08;    //manual +8
            Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
            Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
            R840_IMR_Cal_Type = R840_IMR_CAL;
            break;
        case R840_IMR_LNA_CAL:
            Cal_Info.FILTER_6DB = 0x08;              //+6dB
            Cal_Info.RFBUF_OUT = 0x00;              //from RF_Buf ON, RF_Buf pwr on
            Cal_Info.LNA_POWER = 0xE0;             // LNA power OFF, TF cal ON, -6dB ON
            Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
            Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
            Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
            R840_IMR_Cal_Type = R840_IMR_LNA_CAL;
            break;
        case R840_TF_CAL: //TBD
            Cal_Info.FILTER_6DB = 0x08;              //+6dB
            Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
            Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB OFF
            Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
            Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
            Cal_Info.LNA_GAIN = 0x9F;                  //manual: max
            break;
       case R840_TF_LNA_CAL:
            Cal_Info.FILTER_6DB = 0x08;              //+6dB
            Cal_Info.RFBUF_OUT = 0x00;              //from RF_Buf ON, RF_Buf pwr on
            Cal_Info.LNA_POWER = 0xE0;              //LNA power OFF, TF cal ON, -6dB ON
            Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
            Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
            Cal_Info.LNA_GAIN = 0x9F;                  //manual: max
            break;
      case R840_LPF_CAL:
           Cal_Info.FILTER_6DB = 0x08;              //+6dB
           Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
           Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB OFF
           Cal_Info.MIXER_AMP_GAIN = 0x08;    //manual +8
           Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
           Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
           R840_IMR_Cal_Type = R840_LPF_CAL;
           break;
      case R840_LPF_LNA_CAL:
           Cal_Info.FILTER_6DB = 0x08;              //+6dB
           Cal_Info.RFBUF_OUT = 0x00;               //from RF_Buf ON, RF_Buf pwr on
           Cal_Info.LNA_POWER = 0xA0;              //LNA power OFF, TF cal ON, -6dB OFF
           Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
           Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
           Cal_Info.LNA_GAIN = 0x80;                  //manual: min
           break;
      default:
           Cal_Info.FILTER_6DB = 0x08;              //+6dB
           Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
           Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB OFF
           Cal_Info.MIXER_AMP_GAIN = 0x08;    //manual +8
           Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
           Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
    }
     
    //Ring From RF_Buf Output & RF_Buf Power
    R840_I2C.RegAddr = 0x04;
    R840_SET_REG_Array(u8TunerIndex, 4, 0xDF, Cal_Info.RFBUF_OUT);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 4);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //TF cal (LNA power ON/OFF , TF cal ON/OFF, TF_-6dB ON/OFF)
    R840_I2C.RegAddr = 0x06;
    R840_SET_REG_Array(u8TunerIndex, 6, 0x1F, Cal_Info.LNA_POWER);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 6);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //LNA gain
    R840_I2C.RegAddr = 0x05;
    R840_SET_REG_Array(u8TunerIndex, 5, 0x60, Cal_Info.LNA_GAIN);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 5);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //Mixer Amp Gain
    R840_I2C.RegAddr = 0x07;
    R840_SET_REG_Array(u8TunerIndex, 7, 0xE0, Cal_Info.MIXER_AMP_GAIN);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 7);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //Mixer Buffer Gain
    R840_I2C.RegAddr = 0x1A;
    R840_SET_REG_Array(u8TunerIndex, 26, 0xE0, Cal_Info.MIXER_BUFFER_GAIN);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 26);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    // Set filter +0/6dB
    R840_I2C.RegAddr  = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, Cal_Info.FILTER_6DB);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 6);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
      //---- General calibration setting ----//
      //IMR IQ cap=0
    R840_I2C.RegAddr = 0x03;
    R840_SET_REG_Array(u8TunerIndex, 3, 0xFC, 0);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 3);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    // Set RF_Flag ON(%)
    R840_I2C.RegAddr = 0x01;
    R840_SET_REG_Array(u8TunerIndex, 1, 0x01, 0);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 1);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //RingPLL power ON
    R840_I2C.RegAddr = 0x18;
    R840_SET_REG_Array(u8TunerIndex, 24, 0xEF, 0);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 24);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //LPF filter code = 15
    R840_I2C.RegAddr = 0x0A;
    R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, 0x0F);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 10);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //HPF corner=narrowest; LPF coarse=6M; 1.7M disable
    R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0x00, 0x60);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 11);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //ADC/VGA PWR on; Vga code mode(b4=1), Gain = 26.5dB; Large Code mode Gain(b5=1)
    R840_I2C.RegAddr = 0x0C;
    R840_SET_REG_Array(u8TunerIndex, 12, 0x00, 0x3B);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 12);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
     
    //LNA, RF, Nrb dector pw on; det2 cap=IF_det
    R840_I2C.RegAddr = 0x1D;
    R840_SET_REG_Array(u8TunerIndex, 29, 0xF0, 0x02);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 29);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;
     
    return RT_Success;
}




R840_ErrCode R840_IMR(MS_U8 u8TunerIndex,UINT8 IMR_MEM, MS_BOOL IM_Flag)
{
    UINT32 RingVCO = 0;
    UINT32 RingFreq = 0;
	UINT8  u1MixerGain = 8;
	UINT32 RingRef = R840_Xtal;
	UINT8   divnum_ring = 0;

    R840_SectType IMR_POINT;
    
    R840_SET_REG_Array(u8TunerIndex, 24, 0x3F, 0x00);
    R840_SET_REG_Array(u8TunerIndex, 25, 0xFC, 0x00);
	if(R840_Xtal==16000)  //16M
	{
         divnum_ring = 9;
	}
	else  //24M
	{
		 divnum_ring = 1;
	}
	RingVCO = (16+divnum_ring)* 8 * RingRef;
	
	if(RingVCO>=3200000)
	    R840_SET_REG_Array(u8TunerIndex, 25, 0xDF, 0x00);//vco_band=high, R25[5]=0
	else
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x20); //vco_band=low, R25[5]=1

    switch(IMR_MEM)
    {
    case 0: // RingFreq = 66.66M
        RingFreq = RingVCO/48;
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x80);//ring_div1 /6 (2)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x03);// ring_div2 /8 (3)
		u1MixerGain = 8;
        break;
    case 1: // RingFreq = 200M
        RingFreq = RingVCO/16;
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x00);// ring_div1 /4 (0)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x02);// ring_div2 /4 (2)
		u1MixerGain = 6;
        break;
    case 2: // RingFreq = 400M
        RingFreq = RingVCO/8;
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x00);// ring_div1 /4 (0)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x01);// ring_div2 /2 (1)
		u1MixerGain = 6;
        break;
    case 3: // RingFreq = 533.33M
        RingFreq = RingVCO/6;
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x80);// ring_div1 /6 (2)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x00);// ring_div2 /1 (0)
		u1MixerGain = 8;
        break;
    case 4: // RingFreq = 800M
    default:
        RingFreq = RingVCO/4; 
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x00);// ring_div1 /4 (0)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x00);// ring_div2 /1 (0)
		u1MixerGain = 8;
        break;
    }

    //write I2C to set RingPLL
    R840_I2C.RegAddr = 0x18;
     R840_SET_REG_Array(u8TunerIndex, 24, 0xF0, divnum_ring);//ring_div_num, R24[3:0]
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 24);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    R840_I2C.RegAddr = 0x19;
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 25);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //Ring PLL power
    if((RingFreq>=0) && (RingFreq<R840_RING_POWER_FREQ)) 
            R840_SET_REG_Array(u8TunerIndex, 25, 0xF3, 0x08);//R25[3:2]=2'b10; min_lp
    else
           R840_SET_REG_Array(u8TunerIndex, 24, 0xF3, 0x00); //R25[3:2]=2'b00; min

    R840_I2C.RegAddr = 0x19;
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 25);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //Must do MUX before PLL()
    if(R840_MUX(u8TunerIndex,RingFreq - R840_IMR_IF, RingFreq, R840_STD_SIZE) != RT_Success)      //IMR MUX (LO, RF)
        return RT_Fail;
    
    if(R840_PLL(u8TunerIndex,(RingFreq - R840_IMR_IF), R840_STD_SIZE) != RT_Success)  //IMR PLL
        return RT_Fail;
    
    //Set TF, place after R840_MUX( )
    //TF is dependent to LNA/Mixer Gain setting
    if(R840_SetTF(u8TunerIndex,RingFreq, (UINT8)R840_SetTfType) != RT_Success)
        return RT_Fail;
    
    //clear IQ_cap
    IMR_POINT.Iqcap = R840_GET_REG_Array(u8TunerIndex,3) & 0xFC;
    if(IM_Flag == TRUE)
    {
         if(R840_IQ(u8TunerIndex,&IMR_POINT) != RT_Success)
             return RT_Fail;
    }
    else
    {
        IMR_POINT.Gain_X = R840_IMR_Data[3].Gain_X;
        IMR_POINT.Phase_Y = R840_IMR_Data[3].Phase_Y;
        IMR_POINT.Value = R840_IMR_Data[3].Value;
        //IMR_POINT.Iqcap = R840_IMR_Data[3].Iqcap;
        if(R840_F_IMR(u8TunerIndex,&IMR_POINT) != RT_Success)
            return RT_Fail;
    }
    
    //Save IMR Value
    switch(IMR_MEM)
    {
    case 0:
        R840_IMR_Data[0].Gain_X  = IMR_POINT.Gain_X;
        R840_IMR_Data[0].Phase_Y = IMR_POINT.Phase_Y;
        R840_IMR_Data[0].Value = IMR_POINT.Value;
        R840_IMR_Data[0].Iqcap = IMR_POINT.Iqcap;
        break;
    case 1:
        R840_IMR_Data[1].Gain_X  = IMR_POINT.Gain_X;
        R840_IMR_Data[1].Phase_Y = IMR_POINT.Phase_Y;
        R840_IMR_Data[1].Value = IMR_POINT.Value;
        R840_IMR_Data[1].Iqcap = IMR_POINT.Iqcap;
        break;
    case 2:
        R840_IMR_Data[2].Gain_X  = IMR_POINT.Gain_X;
        R840_IMR_Data[2].Phase_Y = IMR_POINT.Phase_Y;
        R840_IMR_Data[2].Value = IMR_POINT.Value;
        R840_IMR_Data[2].Iqcap = IMR_POINT.Iqcap;
        break;
    case 3:
        R840_IMR_Data[3].Gain_X  = IMR_POINT.Gain_X;
        R840_IMR_Data[3].Phase_Y = IMR_POINT.Phase_Y;
        R840_IMR_Data[3].Value = IMR_POINT.Value;
        R840_IMR_Data[3].Iqcap = IMR_POINT.Iqcap;
        break;
    case 4:
        R840_IMR_Data[4].Gain_X  = IMR_POINT.Gain_X;
        R840_IMR_Data[4].Phase_Y = IMR_POINT.Phase_Y;
        R840_IMR_Data[4].Value = IMR_POINT.Value;
        R840_IMR_Data[4].Iqcap = IMR_POINT.Iqcap;
        break;
    default:
        R840_IMR_Data[4].Gain_X  = IMR_POINT.Gain_X;
        R840_IMR_Data[4].Phase_Y = IMR_POINT.Phase_Y;
        R840_IMR_Data[4].Value = IMR_POINT.Value;
        R840_IMR_Data[4].Iqcap = IMR_POINT.Iqcap;
        break;
    }
    return RT_Success;
}


R840_ErrCode R840_PLL(MS_U8 u8TunerIndex,UINT32 LO_Freq, R840_Standard_Type R840_Standard)
{
    UINT8  MixDiv = 2;
    UINT8  DivBuf = 0;
    UINT8  Ni = 0;
    UINT8  Si = 0;
    UINT8  DivNum = 0;
    UINT8  Nint = 0;
	UINT32 VCO_Min = 1730000;
    UINT32 VCO_Max = VCO_Min*2;
    UINT32 VCO_Freq = 0;
    UINT32 PLL_Ref	= R840_Xtal;
    UINT32 VCO_Fra	= 0;
    UINT16 Nsdm = 2;
    UINT16 SDM = 0;
    UINT16 SDM16to9 = 0;
    UINT16 SDM8to1 = 0;
    UINT8   CP_CUR = 0x00;
    UINT8   CP_OFFSET = 0x00;
    UINT8   SDM_RES = 0x00;
    UINT8   XTAL_POW1 = 0x00;
    UINT8   XTAL_POW0 = 0x00;
	UINT8   XTAL_GM = 0x00;
    UINT16 u2XalDivJudge;
    UINT8   u1XtalDivRemain;
	UINT8   VCO_current_trial = 0;
    
    //cp cur & offset setting
    if(R840_Standard < R840_ATV_SIZE) //ATV
    {
        //current
		if(LO_Freq < (48000+R840_IF_HIGH))
			CP_CUR = 0x00;        //0.7m, [5:3]=000
		else if(LO_Freq < (64000+R840_IF_HIGH))
			CP_CUR = 0x20;        //0.3m, [5:3]=100
		else if(LO_Freq < (80000+R840_IF_HIGH))
            CP_CUR = 0x00;        //0.7m, [5:3]=000
        else if(LO_Freq < (128000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
        else if(LO_Freq < (176000+R840_IF_HIGH))
            CP_CUR = 0x18;        //0.4m, [5:3]=011
        else if(LO_Freq < (432000+R840_IF_HIGH))
            CP_CUR = 0x00;        //0.7m, [5:3]=000
        else if(LO_Freq < (592000+R840_IF_HIGH))
            CP_CUR = 0x28;        //0.2m, [5:3]=101
        else if(LO_Freq < (744000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
		else if(LO_Freq < (752000+R840_IF_HIGH))
			CP_CUR = 0x00;        //0.7m, [5:3]=000
		else
			CP_CUR = 0x20;        //0.3m, [5:3]=100
        
          //offset
        if(LO_Freq < (160000+R840_IF_HIGH))
            CP_OFFSET = 0x04;  //30u,   [2]=1
        else
            CP_OFFSET = 0x00;  //0u,     [2]=0
    }
    else  //DTV
    {
        CP_CUR = 0x00;        //0.7m, [5:3]=000
        CP_OFFSET = 0x00;  //0u,     [2]=0
    }
    
    //CP current & Div Cuurent:100uA(10)
    R840_I2C.RegAddr = 0x11;
    R840_SET_REG_Array(u8TunerIndex, 17, 0xC4, 0x02 | CP_CUR);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 17);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    // VCO current = 3(011) & CP Offset
    R840_I2C.RegAddr = 0x12;
    R840_SET_REG_Array(u8TunerIndex, 18, 0x1B, 0x60 | CP_OFFSET);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 18);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //set XTAL Power
	if((*(pR840_Xtal_Pwr +u8TunerIndex)<XTAL_NOR_HIGH) && (LO_Freq > (64000+R840_IF_HIGH)))
	{
			XTAL_POW1 = 0x00;        //high,       R16[4]=0
			XTAL_POW0 = 0x20;        //high,       R15[6:5]=01
			XTAL_GM = 0x00;            //normal,   R15[4:3]=00
	}
	else
	{
		if(*(pR840_Xtal_Pwr +u8TunerIndex) <= XTAL_NOR_HIGHEST)
		{
		    XTAL_POW1 = 0x00;        //high,       R16[4]=0
			XTAL_POW0 = ((UINT8)(XTAL_NOR_HIGHEST-*(pR840_Xtal_Pwr +u8TunerIndex))<<5);
			XTAL_GM = 0x00;            //normal,   R15[4:3]=00
		}
		else if(*(pR840_Xtal_Pwr +u8TunerIndex) == XTAL_GM3V_HIGHEST)
		{
		    XTAL_POW1 = 0x00;        //high,       R16[4]=0
			XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
			XTAL_GM = 0x18;            //3V,         R15[4:3]=11
		}
		else
		{
		    XTAL_POW1 = 0x00;        //high,       R16[4]=0
			XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
			XTAL_GM = 0x18;            //3V,         R15[4:3]=11
		}
	}
    R840_I2C.RegAddr = 0x0F;
    R840_SET_REG_Array(u8TunerIndex, 15, 0x87,  XTAL_POW0 | XTAL_GM);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    R840_I2C.RegAddr = 0x10;
    R840_SET_REG_Array(u8TunerIndex, 16, 0xEF, XTAL_POW1);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //IQ gen ON & current:Dmin, Bmin(00)
    R840_I2C.RegAddr = 0x17;
    R840_SET_REG_Array(u8TunerIndex, 23, 0xF8, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 23);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //set pll autotune = 62.5kHz (fast)
    R840_I2C.RegAddr = 0x1D;
    R840_SET_REG_Array(u8TunerIndex, 29, 0x3F, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 29);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
    	return RT_Fail;
    
    //Divider
    while(MixDiv <= 64)
    {
        if(((LO_Freq * MixDiv) >= VCO_Min) && ((LO_Freq * MixDiv) < VCO_Max))
        {
            DivBuf = MixDiv;
            while(DivBuf > 2)
            {
                DivBuf = DivBuf >> 1;
                DivNum ++;
            }
            break;
        }
        MixDiv = MixDiv << 1;
    }
    
    //SDM_Res
    if(MixDiv <= 4)  //Div=2,4
    {
        SDM_RES = 0x00;    //short, R19[4:3]=00
    }
    else
    {
        SDM_RES = 0x18;   //400R, R19[4:3]=11
    }
    R840_I2C.RegAddr = 0x13;
    R840_SET_REG_Array(u8TunerIndex, 19, 0xE7, SDM_RES);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 19);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //Xtal Div
    if(R840_Standard == R840_STD_SIZE) //for cal
    {
        R840_XtalDiv = XTAL_DIV1;
        R840_SET_REG_Array(u8TunerIndex, 16, 0xFB, 0x00);
        PLL_Ref = R840_Xtal;
    }
    else if(R840_Standard < R840_ATV_SIZE)    //for A normal operation
    {
        u2XalDivJudge = (UINT16) (LO_Freq/1000/8);
        u1XtalDivRemain = (UINT8) (u2XalDivJudge % 2);
        if(u1XtalDivRemain==1) //odd
        {
             R840_XtalDiv = XTAL_DIV1;
             R840_SET_REG_Array(u8TunerIndex, 16, 0xFB, 0x00);
             PLL_Ref = R840_Xtal;
        }
        else  // div2, note that agc clk also div2
        {
            R840_XtalDiv = XTAL_DIV2;
            R840_SET_REG_Array(u8TunerIndex, 16, 0xFF, 0x04);
            PLL_Ref = R840_Xtal / 2;
        }
    }
    else
    {
        if(LO_Freq < (432000+R840_IF_HIGH))
        {
             R840_XtalDiv = XTAL_DIV2;
             R840_SET_REG_Array(u8TunerIndex, 16, 0xFF, 0x04);
             PLL_Ref = R840_Xtal / 2;
        }
        else
        {
              R840_XtalDiv = XTAL_DIV1;
              R840_SET_REG_Array(u8TunerIndex, 16, 0xFB, 0x00);
              PLL_Ref = R840_Xtal;
        }
    }
    
    R840_I2C.RegAddr = 0x10;
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;
    
    //Divider num
    R840_I2C.RegAddr = 0x10;
    R840_SET_REG_Array(u8TunerIndex, 16, 0x1F, (DivNum << 5));
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    VCO_Freq = LO_Freq * MixDiv;
    Nint     = (UINT8) (VCO_Freq / 2 / PLL_Ref);
    VCO_Fra  = (UINT16) (VCO_Freq - 2 * PLL_Ref * Nint);
    
    //Boundary spur prevention
    if (VCO_Fra < PLL_Ref/64)           //2*PLL_Ref/128
    	VCO_Fra = 0;
    else if (VCO_Fra > PLL_Ref*127/64)  //2*PLL_Ref*127/128
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
    
    //Ni & Si
    Ni = (Nint - 13) / 4;
    Si = Nint - 4 *Ni - 13;
    R840_I2C.RegAddr = 0x14;
    R840_SET_REG_Array(u8TunerIndex, 20, 0x00, (Ni + (Si << 6)));
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 20);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;
    
    //pw_sdm
    R840_I2C.RegAddr = 0x12;
    R840_SET_REG_Array(u8TunerIndex, 18, 0xF7, 0x00);
    if(VCO_Fra == 0)
        R840_SET_REG_Array(u8TunerIndex, 18, 0xFF, 0x08);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 18);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
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
    
    R840_I2C.RegAddr = 0x16;
     R840_SET_REG_Array(u8TunerIndex, 22, 0x00, (UINT8) SDM16to9);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 22);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
    	return RT_Fail;
    
    R840_I2C.RegAddr = 0x15;
     R840_SET_REG_Array(u8TunerIndex, 21, 0x00, (UINT8) SDM8to1);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 21);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
    	return RT_Fail;
    
    
    if(R840_XtalDiv == XTAL_DIV2)
        R840_Delay_MS(20);
    else
        R840_Delay_MS(10);
    
    for(VCO_current_trial=0; VCO_current_trial<3; VCO_current_trial++)
    {
        //check PLL lock status
        R840_I2C_Len.RegAddr = 0x00;
        R840_I2C_Len.Len = 3;
        if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success)
            return RT_Fail;
        
    if( (R840_I2C_Len.Data[2] & 0x40) == 0x00 )
    {
        R840_I2C.RegAddr = 0x12;
        R840_SET_REG_Array(u8TunerIndex, 18, 0x1F, ((2-VCO_current_trial) << 5));
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 18);
         if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
       	return RT_Fail;
       }
    }
    
    if(VCO_current_trial==2)
    {
     //check PLL lock status
     R840_I2C_Len.RegAddr = 0x00;
     R840_I2C_Len.Len = 3;
     if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success)
         return RT_Fail;
     
     if( (R840_I2C_Len.Data[2] & 0x40) == 0x00)
     {
         if(*(pR840_Xtal_Pwr +u8TunerIndex) <= XTAL_NOR_HIGHEST)
             XTAL_GM = 0x18;  //3V
     
         R840_I2C.RegAddr = 0x0F;
         R840_SET_REG_Array(u8TunerIndex, 15, 0xE7, XTAL_GM);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);
         if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
             return RT_Fail;
      }
   }
     
     //set pll autotune = slow (2'b11)
     R840_I2C.RegAddr = 0x1D;
     R840_SET_REG_Array(u8TunerIndex, 29, 0x3F, 0xC0);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 29);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
         return RT_Fail;
     
     return RT_Success;
     
}


R840_ErrCode R840_MUX(MS_U8 u8TunerIndex,UINT32 LO_KHz, UINT32 RF_KHz, R840_Standard_Type R840_Standard)
{

	//Freq_Info_Type Freq_Info1;
	Freq_Info_Type* pFreq_Info;

    *(pFreq_Info1 + u8TunerIndex) = R840_Freq_Sel(LO_KHz, RF_KHz, R840_Standard);
    pFreq_Info = pFreq_Info1 + u8TunerIndex;

	UINT8 Reg08_IMR_Gain   = 0;
	UINT8 Reg09_IMR_Phase  = 0;
	UINT8 Reg03_IMR_Iqcap  = 0;

	// LNA band (depend on RF_KHz)
	R840_I2C.RegAddr = 0x05;
    R840_SET_REG_Array(u8TunerIndex, 5, 0x9F, pFreq_Info->LNA_BAND);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// RF Polyfilter
	R840_I2C.RegAddr = 0x19;
    
    R840_SET_REG_Array(u8TunerIndex, 25, 0x3F, pFreq_Info->RF_POLY);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 25);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// LNA Cap
	R840_I2C.RegAddr = 0x01;
    R840_SET_REG_Array(u8TunerIndex, 1, 0x07, pFreq_Info->LPF_CAP << 3);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 1);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// LNA Notch
	R840_I2C.RegAddr = 0x02;
    R840_SET_REG_Array(u8TunerIndex, 2, 0xE0, pFreq_Info->LPF_NOTCH);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 2);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	//Set_IMR
	if(*(pR840_IMR_done_flag + u8TunerIndex) == TRUE)
	{
		Reg08_IMR_Gain = R840_IMR_Data[pFreq_Info->IMR_MEM].Gain_X & 0x3F;
		Reg09_IMR_Phase = R840_IMR_Data[pFreq_Info->IMR_MEM].Phase_Y & 0x3F;
		Reg03_IMR_Iqcap = R840_IMR_Data[pFreq_Info->IMR_MEM].Iqcap & 0x03;
	}
	else
	{
		Reg08_IMR_Gain = 0;
	    Reg09_IMR_Phase = 0;
		Reg03_IMR_Iqcap = 0;
	}

	R840_I2C.RegAddr = 0x08;
    R840_SET_REG_Array(u8TunerIndex, 8, 0xC0, Reg08_IMR_Gain);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 8);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
    R840_SET_REG_Array(u8TunerIndex, 9, 0xC0, Reg09_IMR_Phase);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 9);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x03;
    R840_SET_REG_Array(u8TunerIndex, 3, 0xFC, Reg03_IMR_Iqcap);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 3);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


R840_ErrCode R840_SetTF(MS_U8 u8TunerIndex,UINT32 u4FreqKHz, UINT8 u1TfType)
{
    UINT8    u1FreqCount = 0;
	UINT32  u4Freq1 = 0;
	UINT32  u4Freq2 = 0;
	UINT32  u4Ratio;
	UINT8    u1TF_Set_Result1 = 0;
	UINT8    u1TF_Set_Result2 = 0;
	UINT8    u1TF_tmp1, u1TF_tmp2;
	UINT8    u1TFCalNum = R840_TF_HIGH_NUM;

	if((u4FreqKHz>=0) && (u4FreqKHz<R840_LNA_LOW_LOWEST[R840_SetTfType]))  //Ultra Low
	{
		 u1TFCalNum = R840_TF_LOWEST_NUM;
         while((u4FreqKHz < R840_TF_Freq_Lowest[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_LOWEST_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_Lowest[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_LOWEST_NUM)
        {
			 R840_TF = R840_TF_Result_Lowest[u1TfType][R840_TF_LOWEST_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_Lowest[u1TfType][u1FreqCount-1];
		     u1TF_Set_Result2 = R840_TF_Result_Lowest[u1TfType][u1FreqCount];
		     u4Freq1 = R840_TF_Freq_Lowest[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_Lowest[u1TfType][u1FreqCount];

			 //u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             //R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);

			 u1TF_tmp1 = ((u1TF_Set_Result1 & 0x40)>>2)*3 + (u1TF_Set_Result1 & 0x3F);  //b6 is 3xb4
			 u1TF_tmp2 = ((u1TF_Set_Result2 & 0x40)>>2)*3 + (u1TF_Set_Result2 & 0x3F);
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
			 R840_TF = u1TF_tmp1 + (UINT8)((u1TF_tmp2 - u1TF_tmp1)*u4Ratio/100);
			 if(R840_TF>=0x40)
				 R840_TF = (R840_TF + 0x10);

		 }
	}
	else if((u4FreqKHz>=R840_LNA_LOW_LOWEST[R840_SetTfType]) && (u4FreqKHz<R840_LNA_MID_LOW[R840_SetTfType]))  //Low
	{
		 u1TFCalNum = R840_TF_LOW_NUM;
         while((u4FreqKHz < R840_TF_Freq_Low[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_LOW_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_Low[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_LOW_NUM)
        {
			 R840_TF = R840_TF_Result_Low[u1TfType][R840_TF_LOW_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_Low[u1TfType][u1FreqCount-1];
		     u1TF_Set_Result2 = R840_TF_Result_Low[u1TfType][u1FreqCount];
		     u4Freq1 = R840_TF_Freq_Low[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_Low[u1TfType][u1FreqCount];

			 //u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             //R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);

			 u1TF_tmp1 = ((u1TF_Set_Result1 & 0x40)>>2) + (u1TF_Set_Result1 & 0x3F);  //b6 is 1xb4
			 u1TF_tmp2 = ((u1TF_Set_Result2 & 0x40)>>2) + (u1TF_Set_Result2 & 0x3F);
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
			 R840_TF = u1TF_tmp1 + (UINT8)((u1TF_tmp2 - u1TF_tmp1)*u4Ratio/100);
			 if(R840_TF>=0x40)
				 R840_TF = (R840_TF + 0x30);
		 }
	}
	else if((u4FreqKHz>=R840_LNA_MID_LOW[R840_SetTfType]) && (u4FreqKHz<R840_LNA_HIGH_MID[R840_SetTfType]))  //Mid
    {
		 u1TFCalNum = R840_TF_MID_NUM;
         while((u4FreqKHz < R840_TF_Freq_Mid[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_MID_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_Mid[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_MID_NUM)
        {
			 R840_TF = R840_TF_Result_Mid[u1TfType][R840_TF_MID_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_Mid[u1TfType][u1FreqCount-1];
		     u1TF_Set_Result2 = R840_TF_Result_Mid[u1TfType][u1FreqCount];
		     u4Freq1 = R840_TF_Freq_Mid[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_Mid[u1TfType][u1FreqCount];
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);
		 }
	}
	else  //HIGH
	{
		 u1TFCalNum = R840_TF_HIGH_NUM;
         while((u4FreqKHz < R840_TF_Freq_High[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_HIGH_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_High[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_HIGH_NUM)
        {
			 R840_TF = R840_TF_Result_High[u1TfType][R840_TF_HIGH_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_High[u1TfType][u1FreqCount-1];
		     u1TF_Set_Result2 = R840_TF_Result_High[u1TfType][u1FreqCount];
		     u4Freq1 = R840_TF_Freq_High[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_High[u1TfType][u1FreqCount];
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);
		 }
	}


	R840_I2C.RegAddr = 0x00;
    R840_SET_REG_Array(u8TunerIndex, 0, 0x80, R840_TF);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

    return RT_Success;
}


R840_ErrCode R840_IQ(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont)
{
	R840_SectType Compare_IQ[3];
	UINT8   VGA_Count = 0;
	UINT8   VGA_Read = 0;
	UINT8   X_Direction;  // 1:X, 0:Y

	// increase VGA power to let image significant
	for(VGA_Count=11; VGA_Count < 16; VGA_Count ++)
	{
		R840_I2C.RegAddr = 0x0C;
        R840_I2C.Data    = (R840_GET_REG_Array(u8TunerIndex, 12) & 0xF0) + VGA_Count;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_Delay_MS(10); //

		if(R840_Muti_Read(u8TunerIndex,&VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*ADC_READ_COUNT)
			break;
	}

    Compare_IQ[0].Gain_X  = R840_GET_REG_Array(u8TunerIndex, 8) & 0xC0;
    Compare_IQ[0].Phase_Y = R840_GET_REG_Array(u8TunerIndex, 9) & 0xC0;
	//Compare_IQ[0].Iqcap = R840_iniArray[3] & 0xFC;

	    // Determine X or Y
	    if(R840_IMR_Cross(u8TunerIndex,&Compare_IQ[0], &X_Direction) != RT_Success)
			return RT_Fail;

		if(X_Direction==1)
		{
			//compare and find min of 3 points. determine I/Q direction
		    if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
			  return RT_Fail;

		    //increase step to find min value of this direction
		    if(R840_CompreStep(u8TunerIndex,&Compare_IQ[0], 0x08) != RT_Success)  //X
			  return RT_Fail;
		}
		else
		{
		   //compare and find min of 3 points. determine I/Q direction
		   if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		   	 return RT_Fail;

		   //increase step to find min value of this direction
		   if(R840_CompreStep(u8TunerIndex,&Compare_IQ[0], 0x09) != RT_Success)  //Y
			 return RT_Fail;
		}

		//Another direction
		if(X_Direction==1)
		{
           if(R840_IQ_Tree(u8TunerIndex, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
		     return RT_Fail;

		   //compare and find min of 3 points. determine I/Q direction
		   if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		   	 return RT_Fail;

		   //increase step to find min value of this direction
		   if(R840_CompreStep(u8TunerIndex,&Compare_IQ[0], 0x09) != RT_Success)  //Y
			 return RT_Fail;
		}
		else
		{
		   if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
		     return RT_Fail;

		   //compare and find min of 3 points. determine I/Q direction
		   if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		     return RT_Fail;

	       //increase step to find min value of this direction
		   if(R840_CompreStep(u8TunerIndex,&Compare_IQ[0], 0x08) != RT_Success) //X
		     return RT_Fail;
		}


		//--- Check 3 points again---//
		if(X_Direction==1)
		{
		    if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
			  return RT_Fail;
		}
		else
		{
		   if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
			return RT_Fail;
		}

		if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

    //Section-9 check
    //if(R840_F_IMR(&Compare_IQ[0]) != RT_Success)
	if(R840_Section(u8TunerIndex,&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

	//clear IQ_Cap = 0
    Compare_IQ[0].Iqcap = R840_GET_REG_Array(u8TunerIndex, 3) & 0xFC;

	if(R840_IMR_Iqcap(u8TunerIndex,&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

	*IQ_Pont = Compare_IQ[0];

	//reset gain/phase/iqcap control setting
	R840_I2C.RegAddr = 0x08;
    R840_SET_REG_Array(u8TunerIndex, 8, 0xC0, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 8);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
    R840_SET_REG_Array(u8TunerIndex, 9, 0xC0, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 9);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x03;
    R840_SET_REG_Array(u8TunerIndex, 3, 0xFC, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 3);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}



//--------------------------------------------------------------------------------------------
// Purpose: record IMC results by input gain/phase location
//          then adjust gain or phase positive 1 step and negtive 1 step, both record results
// input: FixPot: phase or gain
//        FlucPot phase or gain
//        PotReg: 0x08 or 0x09
//        CompareTree: 3 IMR trace and results
// output: TREU or FALSE
//--------------------------------------------------------------------------------------------
R840_ErrCode R840_IQ_Tree(MS_U8 u8TunerIndex,UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree)
{
	UINT8 TreeCunt  = 0;
	UINT8 TreeTimes = 3;
	UINT8 PntReg    = 0;

	if(PotReg == 0x08)
		PntReg = 0x09; //phase control
	else
		PntReg = 0x08; //gain control

	for(TreeCunt = 0;TreeCunt < TreeTimes;TreeCunt ++)
	{
		R840_I2C.RegAddr = PotReg;
		R840_I2C.Data    = FixPot;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_I2C.RegAddr = PntReg;
		R840_I2C.Data    = FlucPot;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		if(R840_Muti_Read(u8TunerIndex,&CompareTree[TreeCunt].Value) != RT_Success)
			return RT_Fail;

		if(PotReg == 0x08)
		{
			CompareTree[TreeCunt].Gain_X  = FixPot;
			CompareTree[TreeCunt].Phase_Y = FlucPot;
		}
		else
		{
			CompareTree[TreeCunt].Phase_Y  = FixPot;
			CompareTree[TreeCunt].Gain_X = FlucPot;
		}

		if(TreeCunt == 0)   //try right-side point
			FlucPot ++;
		else if(TreeCunt == 1) //try left-side point
		{
			if((FlucPot & 0x1F) == 1) //if absolute location is 1, change I/Q direction
			{
				if(FlucPot & 0x20) //b[5]:I/Q selection. 0:Q-path, 1:I-path
				{
					FlucPot = (FlucPot & 0xC0) | 0x01;
				}
				else
				{
					FlucPot = (FlucPot & 0xC0) | 0x21;
				}
			}
			else
				FlucPot = FlucPot - 2;

		}
	}

	return RT_Success;
}




//-----------------------------------------------------------------------------------/
// Purpose: compare IMC result aray [0][1][2], find min value and store to CorArry[0]
// input: CorArry: three IMR data array
// output: TRUE or FALSE
//-----------------------------------------------------------------------------------/
R840_ErrCode R840_CompreCor(R840_SectType* CorArry)
{
	UINT8 CompCunt = 0;
	R840_SectType CorTemp;

	for(CompCunt = 3;CompCunt > 0;CompCunt --)
	{
		if(CorArry[0].Value > CorArry[CompCunt - 1].Value) //compare IMC result [0][1][2], find min value
		{
			CorTemp = CorArry[0];
			CorArry[0] = CorArry[CompCunt - 1];
			CorArry[CompCunt - 1] = CorTemp;
		}
	}

	return RT_Success;
}


//-------------------------------------------------------------------------------------//
// Purpose: if (Gain<9 or Phase<9), Gain+1 or Phase+1 and compare with min value
//          new < min => update to min and continue
//          new > min => Exit
// input: StepArry: three IMR data array
//        Pace: gain or phase register
// output: TRUE or FALSE
//-------------------------------------------------------------------------------------//
R840_ErrCode R840_CompreStep(MS_U8 u8TunerIndex,R840_SectType* StepArry, UINT8 Pace)
{
	R840_SectType StepTemp;

	//min value already saved in StepArry[0]
	StepTemp.Phase_Y = StepArry[0].Phase_Y;
	StepTemp.Gain_X  = StepArry[0].Gain_X;
	//StepTemp.Iqcap  = StepArry[0].Iqcap;

	while(((StepTemp.Gain_X & 0x1F) < R840_IMR_TRIAL) && ((StepTemp.Phase_Y & 0x1F) < R840_IMR_TRIAL))
	{
		if(Pace == 0x08)
			StepTemp.Gain_X ++;
		else
			StepTemp.Phase_Y ++;

		R840_I2C.RegAddr = 0x08;
		R840_I2C.Data    = StepTemp.Gain_X ;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_I2C.RegAddr = 0x09;
		R840_I2C.Data    = StepTemp.Phase_Y;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		if(R840_Muti_Read(u8TunerIndex,&StepTemp.Value) != RT_Success)
			return RT_Fail;

		if(StepTemp.Value <= StepArry[0].Value)
		{
			StepArry[0].Gain_X  = StepTemp.Gain_X;
			StepArry[0].Phase_Y = StepTemp.Phase_Y;
			//StepArry[0].Iqcap = StepTemp.Iqcap;
			StepArry[0].Value   = StepTemp.Value;
		}
		else if((StepTemp.Value - 2*ADC_READ_COUNT) > StepArry[0].Value)
		{
			break;
		}

	} //end of while()

	return RT_Success;
}


//-----------------------------------------------------------------------------------/
// Purpose: read multiple IMC results for stability
// input: IMR_Reg: IMC result address
//        IMR_Result_Data: result
// output: TRUE or FALSE
//-----------------------------------------------------------------------------------/
R840_ErrCode R840_Muti_Read(MS_U8 u8TunerIndex,UINT8* IMR_Result_Data)
{
	UINT8 ReadCunt     = 0;
	UINT16 ReadAmount  = 0;
	UINT8 ReadMax = 0;
	UINT8 ReadMin = 255;
	UINT8 ReadData = 0;

    R840_Delay_MS(ADC_READ_DELAY);//3

	for(ReadCunt = 0; ReadCunt < (ADC_READ_COUNT+2); ReadCunt ++)
	{
		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 2;              // read 0x01
		if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
			return RT_Fail;

		ReadData = (R840_I2C_Len.Data[1] & 0x3F);

		ReadAmount = ReadAmount + (UINT16)ReadData;

		if(ReadData < ReadMin)
			ReadMin = ReadData;

        if(ReadData > ReadMax)
			ReadMax = ReadData;
	}
	*IMR_Result_Data = (UINT8) (ReadAmount - (UINT16)ReadMax - (UINT16)ReadMin);


	return RT_Success;
}


R840_ErrCode R840_Section(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont)
{
	R840_SectType Compare_IQ[3];
	R840_SectType Compare_Bet[3];

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
	{
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xDF) + 1;  //Q-path, Gain=1
	}
	else
	{
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	}
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[1].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[1].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[1].Value = Compare_IQ[0].Value;

	//Try X+1 column and save min result to Compare_Bet[2]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) | 0x20) + 1;  //I-path, Gain=1
	else
	    Compare_IQ[0].Gain_X = IQ_Pont->Gain_X + 1;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R840_CompreCor(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];

	return RT_Success;
}


R840_ErrCode R840_IMR_Cross(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont, UINT8* X_Direct)
{

	R840_SectType Compare_Cross[9]; //(0,0)(0,Q-1)(0,I-1)(Q-1,0)(I-1,0)+(0,Q-2)(0,I-2)(Q-2,0)(I-2,0)
	R840_SectType Compare_Temp;
	UINT8 CrossCount = 0;
    UINT8 Reg08 = R840_GET_REG_Array(u8TunerIndex, 8) & 0xC0;
    UINT8 Reg09 = R840_GET_REG_Array(u8TunerIndex, 9) & 0xC0;

	memset(&Compare_Temp,0, sizeof(R840_SectType));
	Compare_Temp.Value = 255;

	for(CrossCount=0; CrossCount<9; CrossCount++)
	{

		if(CrossCount==0)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==1)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;       //0
		  Compare_Cross[CrossCount].Phase_Y = Reg09 + 1;  //Q-1
		}
		else if(CrossCount==2)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;               //0
		  Compare_Cross[CrossCount].Phase_Y = (Reg09 | 0x20) + 1; //I-1
		}
		else if(CrossCount==3)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08 + 1; //Q-1
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==4)
		{
		  Compare_Cross[CrossCount].Gain_X = (Reg08 | 0x20) + 1; //I-1
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==5)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;       //0
		  Compare_Cross[CrossCount].Phase_Y = Reg09 + 2;  //Q-2
		}
		else if(CrossCount==6)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;               //0
		  Compare_Cross[CrossCount].Phase_Y = (Reg09 | 0x20) + 2; //I-2
		}
		else if(CrossCount==7)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08 + 2; //Q-2
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==8)
		{
		  Compare_Cross[CrossCount].Gain_X = (Reg08 | 0x20) + 2; //I-2
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}

    	R840_I2C.RegAddr = 0x08;
	    R840_I2C.Data    = Compare_Cross[CrossCount].Gain_X;
	    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		   return RT_Fail;

	    R840_I2C.RegAddr = 0x09;
	    R840_I2C.Data    = Compare_Cross[CrossCount].Phase_Y;
	    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		  return RT_Fail;

        if(R840_Muti_Read(u8TunerIndex,&Compare_Cross[CrossCount].Value) != RT_Success)
		  return RT_Fail;

		if( Compare_Cross[CrossCount].Value < Compare_Temp.Value)
		{
		  Compare_Temp.Value = Compare_Cross[CrossCount].Value;
		  Compare_Temp.Gain_X = Compare_Cross[CrossCount].Gain_X;
		  Compare_Temp.Phase_Y = Compare_Cross[CrossCount].Phase_Y;
		}
	} //end for loop


    if(((Compare_Temp.Phase_Y & 0x3F)==0x01) || (Compare_Temp.Phase_Y & 0x3F)==0x02)  //phase Q1 or Q2
	{
      *X_Direct = (UINT8) 0;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[1].Gain_X;    //0
	  IQ_Pont[1].Phase_Y = Compare_Cross[1].Phase_Y; //Q1
	  IQ_Pont[1].Value = Compare_Cross[1].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[5].Gain_X;   //0
	  IQ_Pont[2].Phase_Y = Compare_Cross[5].Phase_Y;//Q2
	  IQ_Pont[2].Value = Compare_Cross[5].Value;
	}
	else if(((Compare_Temp.Phase_Y & 0x3F)==0x21) || (Compare_Temp.Phase_Y & 0x3F)==0x22)  //phase I1 or I2
	{
      *X_Direct = (UINT8) 0;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[2].Gain_X;    //0
	  IQ_Pont[1].Phase_Y = Compare_Cross[2].Phase_Y; //Q1
	  IQ_Pont[1].Value = Compare_Cross[2].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[6].Gain_X;   //0
	  IQ_Pont[2].Phase_Y = Compare_Cross[6].Phase_Y;//Q2
	  IQ_Pont[2].Value = Compare_Cross[6].Value;
	}
	else if(((Compare_Temp.Gain_X & 0x3F)==0x01) || (Compare_Temp.Gain_X & 0x3F)==0x02)  //gain Q1 or Q2
	{
      *X_Direct = (UINT8) 1;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[3].Gain_X;    //Q1
	  IQ_Pont[1].Phase_Y = Compare_Cross[3].Phase_Y; //0
	  IQ_Pont[1].Value = Compare_Cross[3].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[7].Gain_X;   //Q2
	  IQ_Pont[2].Phase_Y = Compare_Cross[7].Phase_Y;//0
	  IQ_Pont[2].Value = Compare_Cross[7].Value;
	}
	else if(((Compare_Temp.Gain_X & 0x3F)==0x21) || (Compare_Temp.Gain_X & 0x3F)==0x22)  //gain I1 or I2
	{
      *X_Direct = (UINT8) 1;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[4].Gain_X;    //I1
	  IQ_Pont[1].Phase_Y = Compare_Cross[4].Phase_Y; //0
	  IQ_Pont[1].Value = Compare_Cross[4].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[8].Gain_X;   //I2
	  IQ_Pont[2].Phase_Y = Compare_Cross[8].Phase_Y;//0
	  IQ_Pont[2].Value = Compare_Cross[8].Value;
	}
	else //(0,0)
	{
	  *X_Direct = (UINT8) 1;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y;
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[3].Gain_X;    //Q1
	  IQ_Pont[1].Phase_Y = Compare_Cross[3].Phase_Y; //0
	  IQ_Pont[1].Value = Compare_Cross[3].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[4].Gain_X;   //I1
	  IQ_Pont[2].Phase_Y = Compare_Cross[4].Phase_Y; //0
	  IQ_Pont[2].Value = Compare_Cross[4].Value;
	}
	return RT_Success;
}


//----------------------------------------------------------------------------------------//
// purpose: search surrounding points from previous point
//          try (x-1), (x), (x+1) columns, and find min IMR result point
// input: IQ_Pont: previous point data(IMR Gain, Phase, ADC Result, RefRreq)
//                 will be updated to final best point
// output: TRUE or FALSE
//----------------------------------------------------------------------------------------//
R840_ErrCode R840_F_IMR(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont)
{
	R840_SectType Compare_IQ[3];
	R840_SectType Compare_Bet[3];
	UINT8 VGA_Count = 0;
	UINT8 VGA_Read = 0;

	//VGA
	for(VGA_Count=11; VGA_Count < 16; VGA_Count ++)
	{
		R840_I2C.RegAddr = 0x0C;
        R840_I2C.Data    = (R840_GET_REG_Array(u8TunerIndex, 12) & 0xF0) + VGA_Count;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_Delay_MS(10);

		if(R840_Muti_Read(u8TunerIndex,&VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*ADC_READ_COUNT)
		break;
	}

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
	{
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xDF) + 1;  //Q-path, Gain=1
	}
	else
	{
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	}
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[1].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[1].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[1].Value = Compare_IQ[0].Value;

	//Try X+1 column and save min result to Compare_Bet[2]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) | 0x20) + 1;  //I-path, Gain=1
	else
	    Compare_IQ[0].Gain_X = IQ_Pont->Gain_X + 1;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(u8TunerIndex,Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R840_CompreCor(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	//clear IQ_Cap = 0
	Compare_Bet[0].Iqcap = R840_GET_REG_Array(u8TunerIndex, 3) & 0xFC;

	if(R840_IMR_Iqcap(u8TunerIndex,&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];

	return RT_Success;
}


R840_ErrCode R840_IMR_Iqcap(MS_U8 u8TunerIndex,R840_SectType* IQ_Point)
{
    R840_SectType Compare_Temp;
	int i = 0;

	//Set Gain/Phase to right setting
	R840_I2C.RegAddr = 0x08;
	R840_I2C.Data = IQ_Point->Gain_X;
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
	R840_I2C.Data = IQ_Point->Phase_Y;
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	//try iqcap
	for(i=0; i<3; i++)
	{
		Compare_Temp.Iqcap = (UINT8) i;
		R840_I2C.RegAddr = 0x03;
        R840_SET_REG_Array(u8TunerIndex, 3, 0xFC, Compare_Temp.Iqcap);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 3);

		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			   return RT_Fail;

		if(R840_Muti_Read(u8TunerIndex,&(Compare_Temp.Value)) != RT_Success)
			   return RT_Fail;

		if(Compare_Temp.Value < IQ_Point->Value)
		{
			IQ_Point->Value = Compare_Temp.Value;
			IQ_Point->Iqcap = Compare_Temp.Iqcap;
		}
	}

    return RT_Success;
}


R840_ErrCode R840_GPIO(MS_U8 u8TunerIndex,R840_GPIO_Type R840_GPIO_Conrl)
{
	//R19[0]=0
	R840_I2C.RegAddr = 0x13;
    R840_SET_REG_Array(u8TunerIndex, 19, 0xFE, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,19);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

    //R15[0]
	if(R840_GPIO_Conrl == HI_SIG)
		R840_SET_REG_Array(u8TunerIndex, 19, 0xFF, 0x01);
	else
		R840_SET_REG_Array(u8TunerIndex, 19, 0xFE, 0x00);

	R840_I2C.RegAddr = 0x0F;
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


R840_ErrCode R840_SetStandard(MS_U8 u8TunerIndex,R840_Standard_Type RT_Standard)
{
    Sys_Info_Type* pSysInfo;
	if(RT_Standard != *(pR840_pre_standard + u8TunerIndex))
	{
		 if(R840_InitReg(u8TunerIndex) != RT_Success)
		     return RT_Fail;
	}
    *(pR840_pre_standard + u8TunerIndex) = RT_Standard;

	*(pSys_Info1 + u8TunerIndex) = R840_Sys_Sel(RT_Standard);
    pSysInfo = pSys_Info1 + u8TunerIndex;

	// Filter Calibration
	UINT8 u1FilCalGap = 8;

	if(RT_Standard<R840_ATV_SIZE)    //ATV
	    u1FilCalGap = R840_Fil_Cal_Gap;
	else
	    u1FilCalGap = 8;

    if(*(pR840_Fil_Cal_flag + u8TunerIndex*R840_STD_SIZE + RT_Standard) == FALSE)
	{
		*(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard) = R840_Filt_Cal_ADC(u8TunerIndex,pSysInfo->FILT_CAL_IF, pSysInfo->BW, u1FilCalGap);
		*(pR840_Fil_Cal_BW + u8TunerIndex*R840_STD_SIZE + RT_Standard) = R840_Bandwidth;
        *(pR840_Fil_Cal_flag + u8TunerIndex*R840_STD_SIZE + RT_Standard) = TRUE;

	    //Reset register and Array
	    if(R840_InitReg(u8TunerIndex) != RT_Success)
		   return RT_Fail;
	}

	// Set Filter Auto Ext
	R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0xEF, pSysInfo->FILT_EXT_ENA);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	if(pSysInfo->FILT_EXT_ENA==0x10)  //(%)
	{
			 if(*(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard)< 2)
			   *(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard) = 2;

			 if((pSysInfo->FILT_EXT_POINT & 0x02)==0x02)  //HPF+3
			 {
				  if(pSysInfo->HPF_COR>12)
				  {    pSysInfo->HPF_COR = 12; }
			 }
			 else  //HPF+1
			 {
				  if(pSysInfo->HPF_COR>14)
				  {    pSysInfo->HPF_COR = 15; }
			 }
	}

	// Set LPF fine code
	R840_I2C.RegAddr = 0x0A;
    R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, *(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard));
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set LPF coarse BW
	R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0x9F, *(pR840_Fil_Cal_BW + u8TunerIndex*R840_STD_SIZE + RT_Standard));
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set HPF corner & 1.7M mode
	R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0x70, pSysInfo->HPF_COR | pSysInfo->V17M);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set TF current
	R840_I2C.RegAddr = 0x03;
    R840_SET_REG_Array(u8TunerIndex, 3, 0xBF, pSysInfo->TF_CUR);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,3);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set Filter current
	R840_I2C.RegAddr = 0x0A;
    R840_SET_REG_Array(u8TunerIndex, 10, 0x9F, pSysInfo->FILT_CUR);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);

	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set Switch Buffer current
	R840_I2C.RegAddr = 0x04;
    R840_SET_REG_Array(u8TunerIndex, 4, 0xFB, pSysInfo->SWBUF_CUR);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,4);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set Filter Comp
	R840_I2C.RegAddr = 0x1E;
        R840_SET_REG_Array(u8TunerIndex, 30, 0x9F, pSysInfo->FILT_COMP);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

   // Set Filter 3dB
	R840_I2C.RegAddr = 0x1E;
        R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, pSysInfo->FILT_3DB);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
		return RT_Fail;

	// Set Filter Ext Condition (%)
	R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 10, 0xF8, 0x04 | pSysInfo->FILT_EXT_POINT);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set sw cap clk
	R840_I2C.RegAddr = 0x12;
    R840_SET_REG_Array(u8TunerIndex, 18, 0xFC, pSysInfo->SWCAP_CLK);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,18);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set NA det power
	R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0x7F, pSysInfo->NA_PWR_DET);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

    // Set LNA det mode & RF_Discharge & NA_Discharge
	R840_I2C.RegAddr = 0x1F;
    R840_SET_REG_Array(u8TunerIndex, 31, 0x2, pSysInfo->LNA_DET_MODE | pSysInfo->RF_DISCHARGE | pSysInfo->NA_DISCHARGE);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,31);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set AGC clk
	R840_I2C.RegAddr = 0x1B;
    R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, pSysInfo->AGC_CLK);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,27);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	// Record SW Version
	R840_I2C.RegAddr = 0x0C;
    R840_SET_REG_Array(u8TunerIndex, 12, 0xF0, VER_NUM);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;

	// Record init flag
	R840_I2C.RegAddr = 0x1A;
    R840_SET_REG_Array(u8TunerIndex, 26, 0xF0, (*(pR840_Initial_done_flag + u8TunerIndex)<<1) | (*(pR840_Fil_Cal_flag + u8TunerIndex*R840_STD_SIZE + RT_Standard)<<2)|  (*(pR840_IMR_done_flag + u8TunerIndex)));
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,26);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;


	return RT_Success;
}


UINT8  R840_Filt_Cal_ADC(MS_U8 u8TunerIndex,UINT32 IF_Freq, UINT8 R840_BW, UINT8 FilCal_Gap)
{
	 UINT8     u1FilterCodeResult = 0;
	 UINT8     u1FilterCode = 0;
	 UINT8     u1FilterCalValue = 0;
	 UINT8     u1FilterCalValuePre = 0;
	 UINT8     initial_cnt = 0;
	 UINT8     i = 0;
	 UINT32   RingVCO = 0;
	 UINT32   RingFreq = 72000;
	 UINT32   RingRef = R840_Xtal;
	 UINT8     divnum_ring = 0;


	if(R840_Xtal==16000)  //16M
	{
         divnum_ring = 11;
	}
	else  //24M
	{
		 divnum_ring = 2;
	}
	 RingVCO = (16+divnum_ring)* 8 * RingRef;
	 RingFreq = RingVCO/48;

	 //Write initial reg before doing calibration
	 if(R840_InitReg(u8TunerIndex) != RT_Success)
		return RT_Fail;

	 if(R840_Cal_Prepare(u8TunerIndex,R840_LPF_CAL) != RT_Success)
	      return RT_Fail;

	 //Set Ring PLL, (R24, R25)
	 R840_I2C.RegAddr = 0x18;
     R840_SET_REG_Array(u8TunerIndex, 24, 0x00, 0x80 | divnum_ring);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,24);
	 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

     R840_SET_REG_Array(u8TunerIndex, 25, 0x00, 0x8B);
	 if(RingVCO>=3200000)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xDF, 0x00);
	 else
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x20);

	 R840_I2C.RegAddr = 0x19;
	 R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,25);;
	 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;


     //Must do before PLL()
	 if(R840_MUX(u8TunerIndex,RingFreq + IF_Freq, RingFreq, R840_STD_SIZE) != RT_Success)     //FilCal MUX (LO_Freq, RF_Freq)
	     return RT_Fail;

	 //Set PLL
	 if(R840_PLL(u8TunerIndex,(RingFreq + IF_Freq), R840_STD_SIZE) != RT_Success)   //FilCal PLL
	       return RT_Fail;

	 //-----below must set after R840_MUX()-------//
	 //Set LNA TF for RF=72MHz. no use
	 if((R840_SetTfType==R840_TF_NARROW) || (R840_SetTfType==R840_TF_NARROW_LIN))   //UL use 270n setting
	 {
	    R840_I2C.RegAddr = 0x00;
        R840_SET_REG_Array(u8TunerIndex, 0, 0x80, 0x1F);
	 }
	 else
	 {
	    R840_I2C.RegAddr = 0x00;
        R840_SET_REG_Array(u8TunerIndex, 0, 0x80, 0x00);
	 }
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
          return RT_Fail;

     //Adc=on; Vga code mode, Gain = -12dB
     R840_I2C.RegAddr = 0x0C;
     R840_SET_REG_Array(u8TunerIndex, 12, 0x00, 0x10);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);

     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;

	 // vcomp = 0
	 R840_I2C.RegAddr = 0x1E;
     R840_SET_REG_Array(u8TunerIndex, 30, 0x9F, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);

	 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	 //Set BW=8M, HPF corner narrowest; 1.7M disable
     R840_I2C.RegAddr = 0x0B;
     R840_SET_REG_Array(u8TunerIndex, 11, 0x00, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		return RT_Fail;

	 //------- increase VGA power to let ADC read value significant ---------//
	 UINT8   VGA_Count = 0;
	 UINT8   VGA_Read = 0;

	 R840_I2C.RegAddr = 0x0A;
     R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
          return RT_Fail;

	 for(VGA_Count=0; VGA_Count < 16; VGA_Count ++)
	 {
		R840_I2C.RegAddr = 0x0C;
        R840_I2C.Data = (R840_GET_REG_Array(u8TunerIndex,12) & 0xF0) + VGA_Count;
		if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_Delay_MS(10); //

		if(R840_Muti_Read(u8TunerIndex,&VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*ADC_READ_COUNT)
			break;
	 }

	 //------- Try suitable BW --------//

	 if(R840_BW==0x60) //6M
         initial_cnt = 1;  //try 7M first
	 else
		 initial_cnt = 0;  //try 8M first

	 for(i=initial_cnt; i<3; i++)
	 {
         if(i==0)
             R840_Bandwidth = 0x00; //8M
		 else if(i==1)
			 R840_Bandwidth = 0x40; //7M
		 else
			 R840_Bandwidth = 0x60; //6M

		 R840_I2C.RegAddr = 0x0B;
         R840_SET_REG_Array(u8TunerIndex, 11, 0x00, R840_Bandwidth);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);

         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
		      return RT_Fail;

		 // read code 0
		 R840_I2C.RegAddr = 0x0A;
         R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, 0);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);

		 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			  return RT_Fail;

		 R840_Delay_MS(10); //delay ms

		 if(R840_Muti_Read(u8TunerIndex,&u1FilterCalValuePre) != RT_Success)
			  return RT_Fail;

		 //read code 13
		 R840_I2C.RegAddr = 0x0A;
         R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, 13);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);

		 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
			  return RT_Fail;

		 R840_Delay_MS(10); //delay ms

		 if(R840_Muti_Read(u8TunerIndex,&u1FilterCalValue) != RT_Success)
			  return RT_Fail;

		 if(u1FilterCalValuePre > (u1FilterCalValue+8))  //suitable BW found
			 break;
	 }

     //-------- Try LPF filter code ---------//
	 u1FilterCalValuePre = 0;
	 for(u1FilterCode=0; u1FilterCode<16; u1FilterCode++)
	 {
         R840_I2C.RegAddr = 0x0A;
         R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, u1FilterCode);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);

         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
              return RT_Fail;

		 R840_Delay_MS(10); //delay ms

		 if(R840_Muti_Read(u8TunerIndex,&u1FilterCalValue) != RT_Success)
		      return RT_Fail;

		 if(u1FilterCode==0)
              u1FilterCalValuePre = u1FilterCalValue;

		 if((u1FilterCalValue+FilCal_Gap*ADC_READ_COUNT) < u1FilterCalValuePre)
		 {
			 u1FilterCodeResult = u1FilterCode;
			  break;
		 }

	 }

	 if(u1FilterCode==16)
          u1FilterCodeResult = 15;

	  return u1FilterCodeResult;

}

R840_ErrCode R840_SetFrequency(MS_U8 u8TunerIndex,R840_Set_Info R840_INFO)
{

    UINT32	LO_KHz;
    Sys_Info_Type* psys_Info;
    Freq_Info_Type* pFreq_Info;
    SysFreq_Info_Type* psysFreq_Info;

    psys_Info = pSys_Info1 + u8TunerIndex;
    pFreq_Info = pFreq_Info1 + u8TunerIndex;
    psysFreq_Info = pSysFreq_Info1 + u8TunerIndex;
      // Check Input Frequency Range
    if((R840_INFO.RF_KHz<40000) || (R840_INFO.RF_KHz>1002000))
    {
        return RT_Fail;
    }
    
    if(R840_INFO.R840_Standard==R840_SECAM_L1_INV)
        LO_KHz = R840_INFO.RF_KHz - psys_Info->IF_KHz;
    else
        LO_KHz = R840_INFO.RF_KHz + psys_Info->IF_KHz;
    
    //Set MUX dependent var. Must do before PLL( )
    if(R840_MUX(u8TunerIndex,LO_KHz, R840_INFO.RF_KHz, R840_INFO.R840_Standard) != RT_Success)   //normal MUX
        return RT_Fail;
    
    //Set PLL
    if(R840_PLL(u8TunerIndex,LO_KHz, R840_INFO.R840_Standard) != RT_Success) //noraml PLL
        return RT_Fail;

   //Set TF
    if(R840_SetTF(u8TunerIndex,R840_INFO.RF_KHz, R840_SetTfType) != RT_Success)
        return RT_Fail;

    R840_IMR_point_num = pFreq_Info->IMR_MEM;

    //Q1.5K
    if(R840_INFO.RF_KHz<R840_LNA_MID_LOW[R840_TF_NARROW])
        R840_SET_REG_Array(u8TunerIndex, 0, 0xFF, 0x80);
    else
        R840_SET_REG_Array(u8TunerIndex, 0, 0x7F, 0x00);
    R840_I2C.RegAddr = 0x00;
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,0);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //set Xtal AAC OFF
    R840_I2C.RegAddr = 0x04;
    R840_SET_REG_Array(u8TunerIndex, 4, 0xFD, 0);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,4);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;


   //Get Sys-Freq parameter
   *(pSysFreq_Info1 + u8TunerIndex) = R840_SysFreq_Sel(R840_INFO.R840_Standard, R840_INFO.RF_KHz);


   // Set LNA_TOP
   R840_I2C.RegAddr = 0x1B;
   R840_SET_REG_Array(u8TunerIndex, 27, 0xF8, psysFreq_Info->LNA_TOP);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,27);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;

  // Set LNA VTHL
   R840_I2C.RegAddr = 0x0D;
   R840_SET_REG_Array(u8TunerIndex, 13, 0x00, psysFreq_Info->LNA_VTH_L);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,13);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

   // Set MIXER TOP
   R840_I2C.RegAddr = 0x1C;
   R840_SET_REG_Array(u8TunerIndex, 28, 0xF0, psysFreq_Info->MIXER_TOP);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,28);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

   // Set MIXER VTHL
   R840_I2C.RegAddr = 0x0E;
   R840_SET_REG_Array(u8TunerIndex, 14, 0x00, psysFreq_Info->MIXER_VTH_L);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,14);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

   // Set RF TOP
   R840_I2C.RegAddr = 0x1A;
   R840_SET_REG_Array(u8TunerIndex, 26, 0x1F, psysFreq_Info->RF_TOP);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,26);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

   // Set Nrb TOP
   R840_I2C.RegAddr = 0x1C;
   R840_SET_REG_Array(u8TunerIndex, 28, 0x0f, psysFreq_Info->NRB_TOP);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,28);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

   // Set Nrb BW
   R840_I2C.RegAddr = 0x1B;
   R840_SET_REG_Array(u8TunerIndex, 27, 0x3F, psysFreq_Info->NRB_BW);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,27);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

 // Set TF LPF
   R840_I2C.RegAddr = 0x01;
   R840_SET_REG_Array(u8TunerIndex, 1, 0xFB, psysFreq_Info->BYP_LPF);
   R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,1);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;

   //Set LT
   if(R840_INFO.R840_LT==LT_ON)
   {
       R840_SET_REG_Array(u8TunerIndex, 23, 0xDF, 0x00);
   }
   else
   {
       R840_SET_REG_Array(u8TunerIndex, 23, 0xFF, 0x20);
   }
   R840_I2C.RegAddr = 0x17;
   R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,23);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
      return RT_Fail;
   //Set Clk Out
   if(R840_INFO.R840_ClkOutMode==CLK_OUT_OFF)
   {
        R840_SET_REG_Array(u8TunerIndex, 7, 0xFF, 0x80);
   }
   else
   {
        R840_SET_REG_Array(u8TunerIndex, 7, 0x7F, 0x00);
   }
   R840_I2C.RegAddr = 0x07;
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;

   //IF AGC1
   R840_I2C.RegAddr = 0x1E;
   R840_SET_REG_Array(u8TunerIndex, 30, 0xEF, 0x00);
   R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);

   if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;

   if(R840_INFO.R840_Standard<R840_ATV_SIZE)   //ATV
   {
   //Det Mode: Discharge 2
       R840_I2C.RegAddr = 0x1F;
      R840_SET_REG_Array(u8TunerIndex, 31, 0x1E, 0x61);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,31);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

       if(R840_XtalDiv == XTAL_DIV1)
       {
       //AGC CLK to 500hz
            R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x28);
       }
       else
       {
       //AGC CLK to 1khz
           R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x00);
       }
    R840_I2C.RegAddr = 0x1B;
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
   }
   else //DTV
   {
        //AGC CLK to 1khz
       R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x00);
       R840_I2C.RegAddr = 0x1B;
       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;
       
       //Det Mode: Discharge 1
       R840_I2C.RegAddr = 0x1F;
       R840_SET_REG_Array(u8TunerIndex, 31, 0x1E, 0x21);
       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,31);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;
       
//for DVB-T2
       switch(R840_INFO.R840_Standard)
       {
           case R840_DVB_T2_6M:
           case R840_DVB_T2_7M:
           case R840_DVB_T2_8M:
           case R840_DVB_T2_1_7M:
           case R840_DVB_T2_10M:
           case R840_DVB_T2_6M_IF_5M:
           case R840_DVB_T2_7M_IF_5M:
           case R840_DVB_T2_8M_IF_5M:
           case R840_DVB_T2_1_7M_IF_5M:
               R840_Delay_MS(100);
               //AGC clk 60Hz
               R840_I2C.RegAddr = 0x1B;
               R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x30);
               //R840_I2C.Data = R840_Array[u8TunerIndex][27];
               R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
               if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                   return RT_Fail;
               //force plain mode
               R840_I2C.RegAddr = 0x03;
               R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
               R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
               if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                   return RT_Fail;
               
               R840_I2C.RegAddr = 0x02;
               R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0);
               R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
               if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                   return RT_Fail;
                break;
           default:
               break;
       }//end switch
 }
 return RT_Success;
}




R840_ErrCode R840_SetPllData(MS_U8 u8TunerIndex,R840_Set_Info R840_INFO)
{
      if(R840_SetStandard(u8TunerIndex,R840_INFO.R840_Standard) != RT_Success)
          return RT_Fail;

      if(R840_SetFrequency(u8TunerIndex,R840_INFO) != RT_Success)
          return RT_Fail;

      return RT_Success;
}


R840_ErrCode R840_Standby(MS_U8 u8TunerIndex,R840_LoopThrough_Type R840_LoopSwitch)
{
 //set LT switch
    if(R840_LoopSwitch==LT_ON)
    {
        R840_SET_REG_Array(u8TunerIndex, 23, 0xDF, 0x00);
        R840_SET_REG_Array(u8TunerIndex, 6, 0x7F, 0x00);
        R840_SET_REG_Array(u8TunerIndex, 29, 0xF7, 0x00);
    }
    else
    {
        R840_SET_REG_Array(u8TunerIndex, 23, 0xFF, 0x20);
        R840_SET_REG_Array(u8TunerIndex, 6, 0xFF, 0x80);
        R840_SET_REG_Array(u8TunerIndex, 29, 0xFF, 0x08);
    }
    
    R840_I2C.RegAddr = 0x17;
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,23);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    R840_I2C.RegAddr = 0x06;
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,6);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //RF, Nrb Det PW off
    R840_I2C.RegAddr = 0x1D;
    R840_SET_REG_Array(u8TunerIndex, 29, 0xFF, 0x05);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,29);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

     //LNA current = lowest, R6[1:0]=11
     R840_I2C.RegAddr = 0x06;
     R840_SET_REG_Array(u8TunerIndex, 6, 0xFF, 0x03);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,6);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;


    //NAT PW off
    R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xFF, 0x80);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
    //Mixer & Mixer Buffer PW off
    R840_I2C.RegAddr = 0x04;
    R840_SET_REG_Array(u8TunerIndex, 4, 0xFF, 0x48);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,4);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
     //Mixer Amp PW off
     R840_I2C.RegAddr = 0x08;
     R840_SET_REG_Array(u8TunerIndex, 8, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,8);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;
    
     //Polyphase PW off
     R840_I2C.RegAddr = 0x09;
     R840_SET_REG_Array(u8TunerIndex, 9, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,9);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
     //Filter PW off
     R840_I2C.RegAddr = 0x0A;
     R840_SET_REG_Array(u8TunerIndex, 10, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,10);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
     //VGA, ADC PW off
     R840_I2C.RegAddr = 0x0C;
     R840_SET_REG_Array(u8TunerIndex, 12, 0xFF, 0xC0);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,12);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;
    
        //PLL LDO A off
     R840_I2C.RegAddr = 0x11;
     R840_SET_REG_Array(u8TunerIndex, 17, 0xFF, 0xC0);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,17);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;
    
     //PLL DLDO 1& 2 off
     R840_I2C.RegAddr = 0x17;
     R840_SET_REG_Array(u8TunerIndex, 23, 0xFF, 0xD8);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,23);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    
     //Ping PLL off
     R840_I2C.RegAddr = 0x18;
     R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x10);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,24);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;
        
    return RT_Success;
}



R840_ErrCode R840_GetRfGain(MS_U8 u8TunerIndex,R840_RF_Gain_Info *pR840_rf_gain)
{

    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = 5;
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
        return RT_Fail;
    
    pR840_rf_gain->RF_gain1 = (R840_I2C_Len.Data[3] & 0x1F);            //lna
    pR840_rf_gain->RF_gain2 = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);  //rf
    pR840_rf_gain->RF_gain3 = (R840_I2C_Len.Data[4] & 0x0F);             //mixer
    
    if(pR840_rf_gain->RF_gain1 > 22)
          pR840_rf_gain->RF_gain1 = 22;  //LNA gain max is 22
    
    if(pR840_rf_gain->RF_gain3 > 10)
          pR840_rf_gain->RF_gain3 = 10;  //MixerAmp gain max is 10
    
    pR840_rf_gain->RF_gain_comb = (pR840_rf_gain->RF_gain1*14 + pR840_rf_gain->RF_gain2*12 + pR840_rf_gain->RF_gain3*12);
    
      return RT_Success;
}





R840_ErrCode R840_RfGainMode(MS_U8 u8TunerIndex,R840_RF_Gain_TYPE R840_RfGainType)
{
    UINT8 MixerGain = 0;
    UINT8 RfGain = 0;
    UINT8 LnaGain = 0;
    
    if(R840_RfGainType==RF_MANUAL)
    {
        //LNA auto off
        R840_I2C.RegAddr = 0x05;
        R840_SET_REG_Array(u8TunerIndex, 5, 0xFF, 0x80);
        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,5);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
         //Mixer buffer off
        R840_I2C.RegAddr = 0x1A;
        R840_SET_REG_Array(u8TunerIndex, 26, 0xFF, 0x10);
        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,26);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
         //Mixer auto off
        R840_I2C.RegAddr = 0x07;
        R840_SET_REG_Array(u8TunerIndex, 7, 0xEF, 0x00);
        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
        R840_I2C_Len.RegAddr = 0x00;
        R840_I2C_Len.Len     = 4;
        if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
        	  return RT_Fail;
        MixerGain = (((R840_I2C_Len.Data[1] & 0x40) >> 6)<<3)+((R840_I2C_Len.Data[3] & 0xE0)>>5);
        RfGain = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);
        LnaGain = R840_I2C_Len.Data[3] & 0x1F;
        
        //set LNA gain
        R840_I2C.RegAddr = 0x05;
        R840_SET_REG_Array(u8TunerIndex, 5, 0xE0, LnaGain);
        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,5);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
        
         //set Mixer Buffer gain
         R840_I2C.RegAddr = 0x1A;
         R840_SET_REG_Array(u8TunerIndex, 26, 0xF0, RfGain);
         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,26);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
             return RT_Fail;
        
         //set Mixer gain
         R840_I2C.RegAddr = 0x07;
         R840_SET_REG_Array(u8TunerIndex, 7, 0xF0, MixerGain);
         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
             return RT_Fail;
    }
    else
    {
        //LNA auto on
         R840_I2C.RegAddr = 0x05;
         R840_SET_REG_Array(u8TunerIndex, 5, 0x7F, 0x00);
         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,5);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
             return RT_Fail;
       //Mixer buffer on
         R840_I2C.RegAddr = 0x1A;
         R840_SET_REG_Array(u8TunerIndex, 26, 0xEF, 0x00);
         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,26);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
             return RT_Fail;
      
       //Mixer auto on
         R840_I2C.RegAddr = 0x07;
         R840_SET_REG_Array(u8TunerIndex, 7, 0xFF, 0x10);
         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
    	       return RT_Fail;
    }
    
       return RT_Success;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


MS_BOOL MDrv_Tuner_R836_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U8 feBw)
{
    R840_Set_Info R840_Info;
    //R840 Standard Setting
    if(DVBT2 == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
         switch(feBw)
         {
          case TUNER_BW_MODE_6MHZ:
              R840_Info.R840_Standard = R840_DVB_T2_6M_IF_5M;
              break;
          case TUNER_BW_MODE_7MHZ:
              R840_Info.R840_Standard = R840_DVB_T2_7M_IF_5M;
              break;
          case TUNER_BW_MODE_8MHZ:
          default:
              R840_Info.R840_Standard = R840_DVB_T2_8M_IF_5M;
              break;
          }
    }
    else if(DVBT == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
          switch(feBw)
           {
           case TUNER_BW_MODE_6MHZ:
               R840_Info.R840_Standard = R840_DVB_T_6M_IF_5M;
               break;
           case TUNER_BW_MODE_7MHZ:
               R840_Info.R840_Standard = R840_DVB_T_7M_IF_5M;
               break;
           case TUNER_BW_MODE_8MHZ:
           default:
               R840_Info.R840_Standard = R840_DVB_T_8M_IF_5M;
               break;
           }
    }
    else if(DVBC == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
           switch(feBw)
           {
           case TUNER_BW_MODE_6MHZ:
               R840_Info.R840_Standard = R840_DVB_C_6M_IF_5M;
               break;
           case TUNER_BW_MODE_7MHZ:
               R840_Info.R840_Standard = R840_DVB_C_6M_IF_5M;
               break;
           case TUNER_BW_MODE_8MHZ:
           default:
               R840_Info.R840_Standard = R840_DVB_T_8M_IF_5M;
               break;
           }
    }
    else if(ATSC == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
           R840_Info.R840_Standard = R840_ATSC_IF_5M;
    }
    else
    {
        R840_Info.R840_Standard = R840_DVB_T_8M_IF_5M;
    }

    
    //R840 Frequency Setting
    R840_Info.RF_KHz = u32Freq; // unit: kHz
    //Loop Through
    R840_Info.R840_LT = LT_ON; // Loop through ON
    //Clk output
    R840_Info.R840_ClkOutMode = CLK_OUT_OFF; // No tuner clock output for other IC

   //IF AGC select
    R840_Info.R840_IfAgc_Select = IF_AGC1; // Select IF AGC1
    //Configure Tuner
    if(R840_SetPllData(u8TunerIndex,R840_Info) != RT_Success)
        return FALSE;

    return TRUE;
}

MS_BOOL MDrv_Tuner_R836_PowerOnOff(MS_U8 u8TunerIndex,MS_BOOL bPowerOn)
{
    if (bPowerOn == FALSE)
    {
        //if need loop through, passing LOOP_THROUGH,else passing SIGLE_IN
        R840_Standby(u8TunerIndex,LT_ON);
    }
    return TRUE;
}

MS_BOOL MDrv_Tuner_R836_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
     TUNER_DBG(("R840 Init..... \n"));
    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;
    
     if(R840_Init(u8TunerIndex) != RT_Success)
     {
         TUNER_ERR(("FAIL!!!!!! \n"));
         return RT_Fail;
     }
     *(pSys_Info1 + u8TunerIndex) = R840_Sys_Sel(R840_ATSC_IF_5M); //set as default
     TUNER_DBG(("SUCCESS!!!!! \n"));
     return RT_Success;
}

MS_BOOL R836_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            return TRUE;
    }

}

#define R836_CHIP_ID 0x96
MS_BOOL R836_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    UINT8 regData = 0;

    R840_I2C_Len.Len = 1;
    R840_I2C_Len.Data[0] = 0;
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
    {
        TUNER_ERR(("[R836] Read chip ID fail \n"));
    }
    regData = R840_I2C_Len.Data[0];
    TUNER_ERR(("[R836] read id =0x%x\n",regData));

    if(regData == R836_CHIP_ID)
    {
        if(!R836_Variables_alloc())
            return FALSE;
        
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1; 
    }
    return (regData == R836_CHIP_ID);
}

MS_BOOL MDrv_Tuner_R836_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
  Sys_Info_Type* psys_info;

  psys_info = pSys_Info1 + u8TunerIndex;
  *pu32IF_Freq = psys_info->IF_KHz;
  return TRUE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_R836) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_R836",          // demod system name
    .data               = TUNER_R836,            // private data value
    .Init               = MDrv_Tuner_R836_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_R836_SetTuner,
    .CheckExist         = R836_CheckExist,
    .GetTunerIF         = MDrv_Tuner_R836_GetTunerIF,
    .Extension_Function = R836_Extension_Function
};

#endif

