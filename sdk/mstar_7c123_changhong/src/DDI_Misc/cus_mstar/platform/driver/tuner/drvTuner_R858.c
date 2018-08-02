//-----------------------------------------------------//
//
// Filename: R858.c
//
// This file is R858 tuner driver
// Copyright 2015 by Rafaelmicro., Inc.
// Author: Ryan Chang
//-----------------------------------------------------//
#include <string.h>
#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_R858)
#include "MsCommon.h"
#include "drvIIC.h"
#include "drvTuner.h"
#include "drvTuner_R858.h"
#include "MsOS.h"
#include "Frontend_Enum_Device.h"
#include "drvTunerNull.h"

static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM]; //MAX_FRONTEND_NUM
static MS_U8 u8Possible_SLAVE_IDs[4] = {0xF4, 0xD4, 0x14, 0x34};

//Standby mode
//1. R858_SBY_IND: individual down, LNA on
//2. R858_SBY_ALL: all down
UINT8   R858_Standby_Mode = R858_SBY_IND;

//User mode
//1. R858_SINGLE: one R858
//2. R858_DUAL: two R858
UINT8   R858_User_Mode = R858_DUAL;

R858_ExtTunerNum_Type R858_ExtTunerNum = R858_NUM1;
R858_IntTunerNum_Type  R858_IntTunerNum = R858_TUNER1;

#define  R858_SHARE_XTAL   FALSE
#define  ADC_MULTI_READ   1
#define  FILTER_GAIN_DELAY  5
#define  FILTER_CODE_DELAY 5
#define  XTAL_CHK_DELAY    20
#define  PLL_LOCK_DELAY    10
UINT32 ADC_READ_DELAY = 2;
UINT8  ADC_READ_COUNT = 1;

UINT8 R858_iniArray[2][R858_REG_NUM] = {     //Tuner1/2
    {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     //Reg0~7, read only
    //  0x00  0x01  0x02  0x03 0x04  0x05 0x06  0x07  
       0x00, 0x10, 0xD7, 0x00, 0x06, 0x02, 0x05, 0x85, 0x01, 0xC4,     //Reg R8~41(both) & R42~R47(tuner1)
    //  0x08  0x09  0x0A  0x0B 0x0C  0x0D 0x0E  0x0F  0x10  0x11
       0xD3, 0x87, 0x4A, 0x90, 0xAA, 0x2A, 0x00, 0x51, 0x22, 0x08,    
    //  0x12  0x13  0x14  0x15  0x16   0x17  0x18  0x19  0x1A  0x1B  
       0x70, 0x80, 0x5A, 0x3A, 0x5B, 0x0A, 0x3A, 0x49, 0x39, 0x49,
    //  0x1C  0x1D  0x1E  0x1F  0x20 0x21  0x22  0x23  0x24  0x25
       0x01, 0x16, 0x2E, 0xAA, 0x03, 0xA7, 0x2C, 0x1B, 0xC0, 0x17},
    //  0x26  0x27  0x28  0x29  0x2A  0x2B  0x2C  0x2D  0x2E  0x2F
    
    {   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     //Reg0~7, read only
    //  0x00  0x01  0x02  0x03 0x04  0x05 0x06  0x07  
        0x00, 0x10, 0xD7, 0x00, 0x06, 0x02, 0x05, 0x85, 0x01, 0xC4,     //Reg R8~41(both) & R42~R47(tuner1)
    //  0x08  0x09  0x0A  0x0B 0x0C  0x0D 0x0E  0x0F  0x10  0x11
        0xD3, 0x87, 0x4A, 0x90, 0xAA, 0x2A, 0x00, 0x51, 0x22, 0x08,    
    //  0x12  0x13  0x14  0x15  0x16   0x17  0x18  0x19  0x1A  0x1B  
        0x70, 0x80, 0x5A, 0x3A, 0x5B, 0x0A, 0x3A, 0x49, 0x39, 0x49,
    //  0x1C  0x1D  0x1E  0x1F  0x20 0x21  0x22  0x23  0x24  0x25
        0x01, 0x16, 0x2E, 0xAA, 0x01, 0x40, 0xCC, 0x20, 0xC0, 0x17}
    //  0x26  0x27  0x28  0x29  0x2A  0x2B  0x2C  0x2D  0x2E  0x2F
};


//UINT8  R858_iniArray2[6]={0x17, 0x13, 0x01, 0x07, 0x01, 0x3F};  //Reg R42~R47(tuner2)

UINT8   R858_ADDRESS[R858_MAX_NUM][2] = {{0xF4, 0xD4},{0x14, 0x34}};   
UINT32  R858_Xtal = 24000;             //
UINT8   R858_XtalDiv = XTAL_DIV1_2;
UINT8   R858_Fil_Cal_Gap = 8;
UINT8   Divider_Manual = 0;  //*not release
UINT32  R858_IF_HIGH = 8500; 
UINT32  R858_IF_GOLOBAL = 6000;
UINT8   R858_Xtal_Pwr = R858_XTAL_HIGHEST;
UINT8   R858_Xtal_Pwr_tmp = R858_XTAL_HIGHEST;

//LPF Cal Array 72MHz
UINT8 R858_LPF_CAL_Array[2][R858_REG_NUM] = {   //tuner1/2
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //read only
      0x02, 0x00, 0x87, 0x02, 0x07, 0x01, 0x03, 0x80,
      0x00, 0x00, 0x80, 0x85, 0x02, 0x0A, 0xAA, 0xAA,
      0x00, 0x10, 0x24, 0x08, 0x70, 0x00, 0x5A, 0xFA,
      0x5B, 0x87, 0x3A, 0x09, 0x38, 0x09, 0x09, 0x54,
      0x2E, 0xD0, 0x42, 0xBF, 0x2C, 0x5A, 0xC0, 0x11 },
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //read only					
      0x02, 0x00, 0x87, 0x02, 0x07, 0x01, 0x03, 0x80,
      0x00, 0x00, 0x80, 0x85, 0x02, 0x0A, 0xAA, 0xAA,
      0x00, 0x10, 0x24, 0x08, 0x70, 0x00, 0x5A, 0xFA,
      0x5B, 0x87, 0x3A, 0x09, 0x38, 0x09, 0x09, 0x54,
      0x6E, 0xD0, 0x70, 0x12, 0x3E, 0x24, 0xF0, 0x1F }
};
//IMR Cal array 
UINT8 R858_IMR_CAL_Array[2][R858_REG_NUM] = {   //tuner1/2
    {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //read only					
       0x02, 0x00, 0xC7, 0x01, 0x07, 0x01, 0x03, 0x80,   //R14[1]:RingtoBuf on
       0x01, 0xF4, 0x83, 0x85, 0x7E, 0x93, 0x00, 0x80,
       0x00, 0x12, 0x22, 0x08, 0x70, 0x00, 0x5A, 0xFA,
       0x5B, 0x87, 0x3A, 0x69, 0x38, 0xF9, 0x09, 0x56,					
       0x2E, 0x50, 0x42, 0xBF, 0x2C, 0x5A, 0xE0, 0x19 },
    {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //read only					
       0x02, 0x00, 0xC7, 0x01, 0x07, 0x01, 0x03, 0x80,   //R14[1]:RingtoBuf on
       0x01, 0xF4, 0x83, 0x85, 0x7E, 0x93, 0x00, 0x80,
       0x00, 0x12, 0x22, 0x08, 0x70, 0x00, 0x5A, 0xFA,
       0x5B, 0x87, 0x3A, 0x69, 0x38, 0xF9, 0x09, 0x56,	
       0x6E, 0xE0, 0x14, 0x11, 0xDE, 0x5C, 0xF0, 0x1F }
};  
//----------------------------------------------------------//
//                   Internal Parameters                          //
//----------------------------------------------------------//
UINT8 R858_Array[R858_MAX_NUM][2][48];
R858_SectType R858_IMR_Data[R858_MAX_NUM][2][R842_IMR_POINT_NUM];
                                               
I2C_TYPE  R858_I2C;
I2C_LEN_TYPE R858_I2C_Len;

UINT8  R858_IMR_point_num;
UINT8  R858_Initial_done_flag[R858_MAX_NUM][2] = {{FALSE, FALSE},{FALSE, FALSE}};
UINT8  R858_IMR_done_flag[R858_MAX_NUM][2] = {{FALSE, FALSE},{FALSE, FALSE}};
UINT8  R858_Bandwidth[R858_MAX_NUM][2] = {{0,0},{0,0}};   //8M
UINT8  R858_Lpf_Lsb[R858_MAX_NUM][2] = {{0,0},{0,0}}; 

UINT8  R858_Fil_Cal_flag[R858_MAX_NUM][2][R858_STD_SIZE];
static UINT8 R858_Fil_Cal_BW[R858_MAX_NUM][2][R858_STD_SIZE];
static UINT8 R858_Fil_Cal_code[R858_MAX_NUM][2][R858_STD_SIZE];
static UINT8 R858_Fil_Cal_LpfLsb[R858_MAX_NUM][2][R858_STD_SIZE]; 

static R858_Standard_Type R858_pre_standard[R858_MAX_NUM][2] = {{R858_STD_SIZE, R858_STD_SIZE},{R858_STD_SIZE, R858_STD_SIZE}};
static UINT8 R858_SBY[R858_MAX_NUM][2][R858_REG_NUM];
static UINT8 R858_SBY2[R858_MAX_NUM][6];           //tuner 1, R42~R47
static UINT8 R858_SBY_flag[R858_MAX_NUM][2]={{0,0},{0,0}}; 
//----------------------------------------------------------//
//                   Internal static struct                         //
//----------------------------------------------------------//
static R858_SysFreq_Info_Type  SysFreq_Info1;
static R858_Sys_Info_Type    Sys_Info1;
static R858_Freq_Info_Type   Freq_Info1;
//----------------------------------------------------------//
//                   Internal Functions                            //
//----------------------------------------------------------//
R858_ErrCode R858_InitReg(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum);
R858_ErrCode R858_InitUserMode(R858_Set_Info R858_INFO);
R858_ErrCode R858_Cal_Prepare(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 u1CalFlag);
R858_ErrCode R858_Xtal_Check(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum);  //TUNER1

R858_ErrCode R858_IMR(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 IMR_MEM, UINT8 IM_Flag);
R858_ErrCode R858_IMR_Process(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum);
R858_ErrCode R858_PLL(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 LO_Freq, R858_Standard_Type R858_Standard);
R858_ErrCode R858_MUX(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 LO_KHz, UINT32 RF_KHz, R858_Standard_Type R858_Standard);
R858_ErrCode R858_IQ(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont);
R858_ErrCode R858_IQ_Tree(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R858_SectType* CompareTree);
R858_ErrCode R858_CompreCor(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* CorArry);
R858_ErrCode R858_CompreStep(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* StepArry, UINT8 Pace);
R858_ErrCode R858_Muti_Read(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8* IMR_Result_Data);
R858_ErrCode R858_Section(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* SectionArry);
R858_ErrCode R858_F_IMR(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont);
R858_ErrCode R858_IMR_Cross(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont, UINT8* X_Direct);
R858_ErrCode R858_IMR_Iqcap(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Point);   
R858_ErrCode R858_SetStandard(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_Standard_Type RT_Standard);
R858_ErrCode R858_SetFrequency(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_Set_Info R858_INFO);

R858_Sys_Info_Type R858_Sys_Sel(R858_Standard_Type R858_Standard);
R858_Freq_Info_Type R858_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R858_Standard_Type R858_Standard);
R858_SysFreq_Info_Type R858_SysFreq_Sel(R858_Standard_Type R858_Standard,UINT32 RF_freq);

UINT8 R858_Filt_Cal_ADC(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 IF_Freq, UINT8 R858_BW, UINT8 FilCal_Gap);

static UINT16 R858_Lna_Acc_Gain[4][32] = 
{
    {0, 13, 25, 38, 52, 67, 83, 98, 114, 131, 149, 167, 179, 192, 206, 222, 236, 249, 264, 272, 281, 281, 281, 281, 281, 290, 301, 316, 333, 349, 352, 369},
    {0, 12, 23, 36, 48, 63, 78, 92, 108, 125, 142, 159, 169, 179, 192, 205, 218, 228, 240, 247, 254, 254, 254, 254, 254, 268, 284, 302, 318, 333, 326, 342},
    {0, 12, 23, 35, 48, 62, 77, 91, 106, 122, 139, 155, 165, 176, 186, 197, 206, 213, 221, 225, 229, 229, 229, 229, 229, 243, 258, 274, 289, 302, 301, 316},
    {0, 11, 22, 35, 48, 63, 78, 93, 110, 128, 146, 164, 176, 187, 195, 204, 210, 215, 219, 222, 225, 225, 225, 225, 225, 238, 251, 266, 279, 290, 306, 322},
};

static INT8 Lna_Acc_Gain_offset[86]={28,  7, -10, -9, 5, 19, 24, 15, 1, -12,//45~145
                                    -17, -13, -2, 9, 11, 5, -9, -2, -11, -6,//145~245
                                      7, 22, 25, 12, -6, -22, -24, -6,10,18,//245~345
                                     16, -5, -22,-28, -16, 4, 13, 8, 12, -3,//345~445
                                     -11, -7, 4, 15, 19, 13, 3, -6, -13, -11,//445~545
                                     -2, 11, 17, 11,-3, -15, -21, -13, 4, 16,//545~645
                                     16, 5, -13, -24, -20, -2, 17, 19, 7, -6,//645~745
                                    -20, -19, -4, 12, 15, 9, -1, -11, -14, -10, //745~845
                                    1, 9, 13, 7, -1, -7};                  //845~905
                                    
static UINT16 R858_Rf_Acc_Gain[16] = 
{
   0, 14, 29, 43, 49, 49, 49, 49, 49, 63,//0~9
  76, 89, 102, 117, 129, 140//10~15
};           

static UINT16 R858_Mixer_Acc_Gain[13] = 
{
   0, 0, 0, 0, 10, 22, 34, 46, 58, 70,//0~9
   82, 93, 102,//10~12
};        
//==============================IIC Operation===================================

int R858_Convert(int InvertNum)
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



static MS_BOOL I2C_Read_Len(R858_ExtTunerNum_Type R858_Num, I2C_LEN_TYPE *I2C_Info)
{
    MS_U8 DataCunt = 0;
    MS_U8 u8SlaveID = I2C_Info->I2cAddr;
    MS_U8 regadd = 0x00;
    MS_U8 *paddr = &regadd;
    MS_U16 u16size  = I2C_Info->Len;
    MS_U8 pu8data[50];
    HWI2C_PORT ePort;

    ePort = getI2CPort((MS_U8)R858_Num);

    if (FALSE == MDrv_IIC_ReadBytes(ePort, u8SlaveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("[R858]I2C_Read_Len error, slave ID = 0x%02x\n", u8SlaveID));
        return FALSE;
    }

    for (DataCunt = 0; DataCunt < u16size; DataCunt ++)
    {
        I2C_Info->Data[DataCunt] = R858_Convert(pu8data[DataCunt]);
    }

    return TRUE;
}

static MS_BOOL I2C_Write(R858_ExtTunerNum_Type R858_Num, I2C_TYPE *I2C_Info)
{
    MS_U8 u8SlaveID = I2C_Info->I2cAddr;
    MS_U8 *pu8data = &(I2C_Info->Data);
    MS_U8 *paddr = &(I2C_Info->RegAddr);
    HWI2C_PORT ePort;

    ePort = getI2CPort((MS_U8)R858_Num);
    if (FALSE == MDrv_IIC_WriteBytes(ePort, u8SlaveID, 1, paddr, 1, pu8data))
    {
        TUNER_ERR(("[R858]I2C_Write Error \n"));
        return FALSE;
    }

    R858_Delay_MS(R858_Num, 1); // delay 1ms

    return TRUE;
}

static MS_BOOL I2C_Write_Len(R858_ExtTunerNum_Type R858_Num, I2C_LEN_TYPE *I2C_Info)
{
    MS_U8 u8SlaveID = I2C_Info->I2cAddr;
    MS_U16 u16size  = I2C_Info->Len;
    MS_U8 *paddr = &(I2C_Info->RegAddr);
    MS_U8 *pu8data = I2C_Info->Data;
    HWI2C_PORT ePort;

    ePort = getI2CPort((MS_U8)R858_Num);
    //printf("I2C_Write_Len slave ID = 0x%02x", u8SlaveID);
    if (FALSE == MDrv_IIC_WriteBytes(ePort, u8SlaveID, 1, paddr, u16size, pu8data))
    {
        TUNER_ERR(("[R858]I2C_Write_Len Error \n"));
        return FALSE;
    }

    R858_Delay_MS(R858_Num, 1); // delay 1ms

    return TRUE;
}
//=================================End============================================

R858_Sys_Info_Type R858_Sys_Sel(R858_Standard_Type R858_Standard)
{

    R858_Sys_Info_Type R858_Sys_Info;
    
    switch (R858_Standard)
    {
    case R858_DVB_T_6M: 
    case R858_DVB_T2_6M:
         R858_Sys_Info.IF_KHz=4570;               //IF
         R858_Sys_Info.BW=1;                      //BW(v7M/v6M)=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7450;          //CAL IF
         R858_Sys_Info.HPF_COR=5;                 //R18[6:3] [0~15 ; input: "0~15"]v
         R858_Sys_Info.FILT_EXT_ENA=0;           //R19[6] filter ext disable [off(0), on(1)]v
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;     //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;          //buf 8
         break;
         
    case R858_DVB_T_7M:  
    case R858_DVB_T2_7M:  
         R858_Sys_Info.IF_KHz=4570;               //IF
         R858_Sys_Info.BW=1;                      //BW=7M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7750;          //CAL IF
         R858_Sys_Info.HPF_COR=8;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;            //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;      //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;          //buf 8
         break;
         
    case R858_DVB_T_8M: 
    case R858_DVB_T2_8M: 
         R858_Sys_Info.IF_KHz=4570;                //IF
         R858_Sys_Info.BW=0;                       //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8500;           //CAL IF
         R858_Sys_Info.HPF_COR=12;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;         //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;    //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;         //buf 8
         break;
         
    case R858_DVB_T2_1_7M: 
         R858_Sys_Info.IF_KHz=1900;
         R858_Sys_Info.BW=1;                       //BW=7M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7800;           //CAL IF
         R858_Sys_Info.HPF_COR=8;	             //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;         //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;   //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;      //lna=max-1 & Buf 4
         break;
         
    case R858_DVB_T2_10M: 
         R858_Sys_Info.IF_KHz=5600;
         R858_Sys_Info.BW=0;                      //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=10800;         //CAL IF
         R858_Sys_Info.HPF_COR=12;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;           //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;    //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;        //lna=max-1 & Buf 4
         break;
         
    case R858_DVB_C_8M:   
         R858_Sys_Info.IF_KHz=5070;
         R858_Sys_Info.BW=0;                       //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=9000;           //CAL IF
         R858_Sys_Info.HPF_COR=10;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;         //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;   //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;       //lna=max-1 & Buf 4
         break;
         
    case R858_DVB_C_6M:  
         R858_Sys_Info.IF_KHz=5070;
         R858_Sys_Info.BW=1;                      //BW=7M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8025;          //CAL IF   
         R858_Sys_Info.HPF_COR=3;                 //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;             //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;      //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;          //lna=max-1 & Buf 4
         break;
         
    case R858_J83B:  
         R858_Sys_Info.IF_KHz=5070;
         R858_Sys_Info.BW=1;                       //BW=7M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8025;          //CAL IF  
         R858_Sys_Info.HPF_COR=3;                 //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;            //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;     //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;        //lna=max-1 & Buf 4
         break;
         
    case R858_ISDB_T_4063: 
         R858_Sys_Info.IF_KHz=4063;
         R858_Sys_Info.BW=1;                      //BW=7M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7000;          //CAL IF  //7200
         //if(R858_Chip == R858_MP)
         R858_Sys_Info.HPF_COR=8;             //R18[6:3] [0~15 ; input: "0~15"]
         //else
         //	R858_Sys_Info.HPF_COR=11;            //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;           //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;    //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;        //buf 8
         break;
         
    case R858_ISDB_T_4570:
         R858_Sys_Info.IF_KHz=4570;                    //IF
         R858_Sys_Info.BW=1;                       //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7250;          //CAL IF
         R858_Sys_Info.HPF_COR=5;            //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;        //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;    //R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;    //buf 8
         break;
         
    case R858_DTMB_8M_4570: 
         R858_Sys_Info.IF_KHz=4570;
         R858_Sys_Info.BW=0;                                //BW=8M
         R858_Sys_Info.FILT_CAL_IF=8500;           //CAL IF  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.HPF_COR=12;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;			 //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_DTMB_6M_4500:
         R858_Sys_Info.IF_KHz=4500;
         R858_Sys_Info.BW=1;                      //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7000;           //CAL IF    
         R858_Sys_Info.HPF_COR=5;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		    //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_ATSC:  
         R858_Sys_Info.IF_KHz=5070;
         R858_Sys_Info.BW=1;                      //BW=7M  R17[5:4]  [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7900;          //CAL IF   
         R858_Sys_Info.HPF_COR=4;                 //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_DVB_T_6M_IF_5M: 
    case R858_DVB_T2_6M_IF_5M: 
         R858_Sys_Info.IF_KHz=5000;                    //IF
         R858_Sys_Info.BW=1;                      //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7800;          //CAL IF
         R858_Sys_Info.HPF_COR=4;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		    //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;			//buf 8
         break;
         
    case R858_DVB_T_7M_IF_5M:  
    case R858_DVB_T2_7M_IF_5M:  
         R858_Sys_Info.IF_KHz=5000;                     //IF
         R858_Sys_Info.BW=0;                       //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8300;           //CAL IF
         R858_Sys_Info.HPF_COR=7;                  //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;			//buf 8
         break;
         
    case R858_DVB_T_8M_IF_5M: 
    case R858_DVB_T2_8M_IF_5M:
         R858_Sys_Info.IF_KHz=5000;                     //IF
         R858_Sys_Info.BW=0;                       //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8900;           //CAL IF
         R858_Sys_Info.HPF_COR=10;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;			//buf 8
         break;
         
    case R858_DVB_T2_1_7M_IF_5M: 
         R858_Sys_Info.IF_KHz=5000;
         R858_Sys_Info.BW=2;                       //BW=6M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=5900;           //CAL IF
         R858_Sys_Info.HPF_COR=1;                 //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_DVB_C_8M_IF_5M:  
         R858_Sys_Info.IF_KHz=5000;
         R858_Sys_Info.BW=0;                       //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8900;           //CAL IF 
         R858_Sys_Info.HPF_COR=10;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_DVB_C_6M_IF_5M:  
         R858_Sys_Info.IF_KHz=5000;
         R858_Sys_Info.BW=1;                      //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8100;          //CAL IF   
         R858_Sys_Info.HPF_COR=4;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;			 //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_J83B_IF_5M:  
         R858_Sys_Info.IF_KHz=5000;
         R858_Sys_Info.BW=1;                       //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7800;          //CAL IF  
         R858_Sys_Info.HPF_COR=3;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;			 //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_ISDB_T_IF_5M: 
         R858_Sys_Info.IF_KHz=5000;
         R858_Sys_Info.BW=1;                      //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=7940;          //CAL IF  
         R858_Sys_Info.HPF_COR=3;                //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=1;		     //R19[6] filter ext enable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=3;			//buf 8
         break;
         
    case R858_DTMB_8M_IF_5M: 
         R858_Sys_Info.IF_KHz=5000;
         R858_Sys_Info.BW=0;                      //BW=8M   R17[5:4] [8M(0), 7M(1), 6M(2)]
         R858_Sys_Info.FILT_CAL_IF=8650;          //CAL IF  
         R858_Sys_Info.HPF_COR=9;                 //R18[6:3] [0~15 ; input: "0~15"]
         R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
         R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
         R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
         break;
         
    case R858_DTMB_6M_IF_5M:
        R858_Sys_Info.IF_KHz=5000;
        R858_Sys_Info.BW=1;                       //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
        R858_Sys_Info.FILT_CAL_IF=7700;           //CAL IF  
        R858_Sys_Info.HPF_COR=4;                 //R18[6:3] [0~15 ; input: "0~15"]
        R858_Sys_Info.FILT_EXT_ENA=0;		    //R19[6] filter ext disable [off(0), on(1)]
        R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
        R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
        break;
        
    case R858_ATSC_IF_5M:  
        R858_Sys_Info.IF_KHz=5000;
        R858_Sys_Info.BW=1;                      //BW=7M   R17[5:4] [8M(0), 7M(1), 6M(2)]
        R858_Sys_Info.FILT_CAL_IF=7900;          //CAL IF   
        R858_Sys_Info.HPF_COR=4;                //R18[6:3] [0~15 ; input: "0~15"]
        R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
        R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
        R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
        break;
        
    default:  //R858_DVB_T_8M
        R858_Sys_Info.IF_KHz=4570;                     //IF
        R858_Sys_Info.BW=0;                       //BW=8M  R17[5:4] [8M(0), 7M(1), 6M(2)]
        R858_Sys_Info.FILT_CAL_IF=8330;           //CAL IF
        R858_Sys_Info.HPF_COR=10;                 //R18[6:3] [0~15 ; input: "0~15"]
        R858_Sys_Info.FILT_EXT_ENA=0;		     //R19[6] filter ext disable [off(0), on(1)]
        R858_Sys_Info.FILT_EXT_WIDEST=0x00;		//R30[2]=0, ext normal
        R858_Sys_Info.FILT_EXT_POINT=2;			//lna=max-1 & Buf 4
        break;
    }

    
    R858_Sys_Info.HPF_NOTCH = 0;          //R23[7] notch high  [notch high(0), notch low(1)]
    R858_Sys_Info.AGC_CLK = 0;          //R38[5:4] 1k   [1kHz(0), 512Hz(1), 4kHz(2), 64Hz(3)]
    
    return R858_Sys_Info;
}


R858_Freq_Info_Type R858_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R858_Standard_Type R858_Standard)
{
    R858_Freq_Info_Type R858_Freq_Info;
    
    
    //----- LO dependent parameter --------
    //IMR point 
    if((LO_freq>0) && (LO_freq<130000))
         R858_Freq_Info.IMR_MEM = 0;
    else if((LO_freq>=130000) && (LO_freq<210000))
         R858_Freq_Info.IMR_MEM = 1;
    else if((LO_freq>=210000) && (LO_freq<430000))
         R858_Freq_Info.IMR_MEM = 2;
    else if((LO_freq>=430000) && (LO_freq<560000))
         R858_Freq_Info.IMR_MEM = 3;
    else if((LO_freq>=560000) && (LO_freq<800000))
         R858_Freq_Info.IMR_MEM = 4;
    else 
         R858_Freq_Info.IMR_MEM = 5;
    
    //Diplexer Select R11[1:0]
    if(LO_freq<340000)
        R858_Freq_Info.TF_DIPLEXER = 2; //LPF   R11[1:0]
    else
        R858_Freq_Info.TF_DIPLEXER = 0; //HPF
    
    //TF_HPF_BPF  R11[7:6], R12[0]
    /*	7-lowest:111 ; 6:011 ; 5:101 ; 4:001 ; 3:110 ; 2:010 ; 1:100 ; 0-noBPF:000	*/
    if(LO_freq<580000)
        R858_Freq_Info.TF_HPF_BPF = 7;////7 => 111:lowset BPF  R11[7:6], R12[0]
    else if(LO_freq>=580000 && LO_freq<660000)
        R858_Freq_Info.TF_HPF_BPF = 1;////4 => 001
    else if(LO_freq>=660000 && LO_freq<780000)
        R858_Freq_Info.TF_HPF_BPF = 6;////3 => 110
    else if(LO_freq>=780000 && LO_freq<900000)
        R858_Freq_Info.TF_HPF_BPF = 4;////1 => 100
    else
        R858_Freq_Info.TF_HPF_BPF = 0;////0 => 000
    
    /*
    00: highest band
    01: med band
    10: low band
    11: do not use
    */
    //RF polyfilter band
    if((LO_freq>0) && (LO_freq<133000))  
           R858_Freq_Info.RF_POLY = 2;   // low	=> R14[3:2]
    else if((LO_freq>=133000) && (LO_freq<221000)) 
           R858_Freq_Info.RF_POLY = 1;   //mid => R14[3:2]
    else if((LO_freq>=221000) && (LO_freq<789000))
    	 R858_Freq_Info.RF_POLY = 0;   //highest    => R14[3:2]
    else
		 R858_Freq_Info.RF_POLY = 3;   //do not use	=> R14[3:2]
    
    
    /*
    00: highest
    01: high
    10: low
    11: lowest
    */
    //TF_HPF_Corner
    if((LO_freq>0) && (LO_freq<480000))  
        R858_Freq_Info.TF_HPF_CNR = 3;   //lowest	=> R12[2:1]
    else if((LO_freq>=480000) && (LO_freq<550000))  
        R858_Freq_Info.TF_HPF_CNR = 2;   // low	=> R12[2:1]
    else if((LO_freq>=550000) && (LO_freq<700000))  
        R858_Freq_Info.TF_HPF_CNR = 1;   // high    => R12[2:1]
    else
        R858_Freq_Info.TF_HPF_CNR = 0;   //highest	=> R12[2:1]
    
    
    //LPF Cap, Notch
    switch(R858_Standard)
    {
        case R858_DVB_C_8M:                            //Cable
        case R858_DVB_C_6M:
        case R858_J83B:
        case R858_DVB_C_8M_IF_5M:
        case R858_DVB_C_6M_IF_5M:
        case R858_J83B_IF_5M:
             if(LO_freq<77000)  
             {
                 R858_Freq_Info.LPF_CAP = 15;
                 R858_Freq_Info.LPF_NOTCH = 10;
             }
             else if((LO_freq>=77000) && (LO_freq<85000))
             {
                 R858_Freq_Info.LPF_CAP = 15;
                 R858_Freq_Info.LPF_NOTCH = 4;
             }
             else if((LO_freq>=85000) && (LO_freq<115000))
             {
                 R858_Freq_Info.LPF_CAP = 13;
                 R858_Freq_Info.LPF_NOTCH = 3;
             }
             else if((LO_freq>=115000) && (LO_freq<125000))
             {
                 R858_Freq_Info.LPF_CAP = 11;
                 R858_Freq_Info.LPF_NOTCH = 1;
             }
             else if((LO_freq>=125000) && (LO_freq<141000))
             {
                 R858_Freq_Info.LPF_CAP = 9;
                 R858_Freq_Info.LPF_NOTCH = 0;
             }
             else if((LO_freq>=141000) && (LO_freq<157000))
             {
                 R858_Freq_Info.LPF_CAP = 8;
                 R858_Freq_Info.LPF_NOTCH = 0;
             }
             else if((LO_freq>=157000) && (LO_freq<181000))
             {
                 R858_Freq_Info.LPF_CAP = 6;
                 R858_Freq_Info.LPF_NOTCH = 0;
             }
             else if((LO_freq>=181000) && (LO_freq<205000))
             {
                 R858_Freq_Info.LPF_CAP = 3;
                 R858_Freq_Info.LPF_NOTCH = 0;
             }
             else //LO>=201M
             {
                 R858_Freq_Info.LPF_CAP = 0;
                 R858_Freq_Info.LPF_NOTCH = 0;
             }
             break;
             
        default:  //Air, DTMB (for 180nH)
            if((LO_freq>0) && (LO_freq<73000))  
            {
                R858_Freq_Info.LPF_CAP = 8;
                R858_Freq_Info.LPF_NOTCH = 10;
            }
            else if((LO_freq>=73000) && (LO_freq<81000))
            {
                R858_Freq_Info.LPF_CAP = 8;
                R858_Freq_Info.LPF_NOTCH = 4;
            }
            else if((LO_freq>=81000) && (LO_freq<89000))
            {
                R858_Freq_Info.LPF_CAP = 8;
                R858_Freq_Info.LPF_NOTCH = 3;
            }
            else if((LO_freq>=89000) && (LO_freq<121000))
            {
                R858_Freq_Info.LPF_CAP = 6;
                R858_Freq_Info.LPF_NOTCH = 1;
            }
            else if((LO_freq>=121000) && (LO_freq<145000))
            {
                R858_Freq_Info.LPF_CAP = 4;
                R858_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=145000) && (LO_freq<153000))
            {
                R858_Freq_Info.LPF_CAP = 3;
                R858_Freq_Info.LPF_NOTCH = 0;
            }
            else if((LO_freq>=153000) && (LO_freq<177000))
            {
                R858_Freq_Info.LPF_CAP = 2;
                R858_Freq_Info.LPF_NOTCH = 0;
            } 
            else if((LO_freq>=177000) && (LO_freq<201000))
            {
                R858_Freq_Info.LPF_CAP = 1;
                R858_Freq_Info.LPF_NOTCH = 0;
            }
            else //LO>=201M
            {
                R858_Freq_Info.LPF_CAP = 0;
                R858_Freq_Info.LPF_NOTCH = 0;
            }
            
            break;
            
        }//end switch(standard)   
        return R858_Freq_Info;
}




R858_SysFreq_Info_Type R858_SysFreq_Sel(R858_Standard_Type R858_Standard,UINT32 RF_freq)
{
    
    R858_SysFreq_Info_Type R858_SysFreq_Info;
    
    switch(R858_Standard)
    {
    case R858_DVB_T_6M:
    case R858_DVB_T_7M:
    case R858_DVB_T_8M:
    case R858_DVB_T_6M_IF_5M:
    case R858_DVB_T_7M_IF_5M:
    case R858_DVB_T_8M_IF_5M:
    case R858_DVB_T2_6M:
    case R858_DVB_T2_7M: 
    case R858_DVB_T2_8M:
    case R858_DVB_T2_1_7M:
    case R858_DVB_T2_10M:
    case R858_DVB_T2_6M_IF_5M:
    case R858_DVB_T2_7M_IF_5M:
    case R858_DVB_T2_8M_IF_5M:
    case R858_DVB_T2_1_7M_IF_5M:
        //PW
        R858_SysFreq_Info.NA_PWR_DET = 1;    //R44[1]   ["off" (0), "on" (1)] v  co-use ; tuner 1 control v
        R858_SysFreq_Info.LNA_NRB_DET=0;    //R8[4]    ["on" (0), "off" (1)] v 
        
        //LNA
        R858_SysFreq_Info.LNA_TOP=4;       //R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control v
        R858_SysFreq_Info.LNA_VTL_H=0x5A;   //R30[7:0] LNA VTL/H = 0.84(5) / 1.34(A)  v
        R858_SysFreq_Info.RF_LTE_PSG=1;     //R32[3]  ["no psg" (0), "7.5dB(5~8)" (1)] v
        
        
        //RFBuf
        R858_SysFreq_Info.RF_TOP=4;               //R32[2:0]  ["7~0" (0~7) ; input: "7~0"] v
        R858_SysFreq_Info.RF_VTL_H=0x4A;          //R31[7:0]  RF VTL/H = 0.74(4) / 1.34(A) v
        R858_SysFreq_Info.RF_GAIN_LIMIT=0;        //R13[6:5]   ["max =15" (0), "max =11" (1), "max =13" (2), "max =9" (3)] v
        
        //Mixer and Mixamp
        R858_SysFreq_Info.MIXER_AMP_CURRENT=1;    //R10[1]  ["high" (0), "low" (1)] v
        R858_SysFreq_Info.MIXER_AMP_LPF = 7;     //R47[2:0]  ["normal (widest)" (0), "1" (1), "2" (2), "3" (3), "4" (4), "5" (5), "6" (6), "narrowest" (7)] v
        R858_SysFreq_Info.MIXER_TOP=9;           //R34[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"] v
        R858_SysFreq_Info.MIXER_VTL_H=0x49;      //R35[7:0]	Mixer VTL/H = 0.74(4) / 1.24(9) v
        R858_SysFreq_Info.MIXER_GAIN_LIMIT=3;    //R46[7:6]	["max=6" (0), "max=8" (1), "max=10" (2), "max=12" (3)] v
        
        //Filter
        R858_SysFreq_Info.FILTER_TOP=4;					//R36[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"] v
        R858_SysFreq_Info.FILTER_VTL_H=0x49;			//R37[7:0]  Flter VTL/H = 0.74(4) / 1.24(9) v
        R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)] ? Description different
        
        //Discharge
        R858_SysFreq_Info.BB_DIS_CURR=0;                //R40[0]		["x1" (0) , "x1/2" (1)] v
        R858_SysFreq_Info.MIXER_FILTER_DIS=2;			//R39[7:6]		["highest" (0), "high" (1), "low" (2), "lowest" (3)] v
        R858_SysFreq_Info.BB_DET_MODE=0;				//R39[4]		["average" (0), "peak" (1)]  ? Description different
        
        //Polyphase
        R858_SysFreq_Info.ENB_POLY_GAIN=0;				//R17[3]=0		original ["original" (0), "ctrl by mixamp (>10)" (1)] v
        
        //NRB  
        R858_SysFreq_Info.NRB_TOP=4;					//R33[3:0]     ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"] v
        R858_SysFreq_Info.NRB_BW_HPF=0;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)] v
        R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)] v
        R858_SysFreq_Info.IMG_NRB_ADDER=2;				//R34[2:1]		["original" (0), "top+6" (1), "top+9" (2), "top+11" (3)]v
        break;
        
    case R858_DVB_C_8M:
    case R858_DVB_C_6M:	
    case R858_J83B:
    case R858_DVB_C_8M_IF_5M:
    case R858_DVB_C_6M_IF_5M:	
    case R858_J83B_IF_5M:
        
        //PW
        R858_SysFreq_Info.NA_PWR_DET = 1;				//R44[1]   ["off" (0), "on" (1)]
        R858_SysFreq_Info.LNA_NRB_DET=0;				//R8[4]    ["on" (0), "off" (1)]
        
        //LNA	
        R858_SysFreq_Info.LNA_TOP=5;					//R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control
        R858_SysFreq_Info.LNA_VTL_H=0x5A;				//R30[7:0] LNA VTL/H=0.84(5) / 1.34(A)
        R858_SysFreq_Info.RF_LTE_PSG=0;					//R32[3]  ["no psg" (0), "7.5dB(5~8)" (1)]
        
        //RFBuf
        R858_SysFreq_Info.RF_TOP=4;						//R32[2:0]  ["7~0" (0~7) ; input: "7~0"]
        R858_SysFreq_Info.RF_VTL_H=0x3A;				//R31[7:0] RF VTL/H = 0.64(3) / 1.34(A)
        R858_SysFreq_Info.RF_GAIN_LIMIT=0;				//R13[6:5]v   ["max =15" (0), "max =11" (1), "max =13" (2), "max =9" (3)]
        
        //Mixer and Mixamp
        R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]   ["high" (0), "low" (1)]
        R858_SysFreq_Info.MIXER_AMP_LPF = 4;			//R47[2:0] ["normal (widest)" (0), "1" (1), "2" (2), "3" (3), "4" (4), "5" (5), "6" (6), "narrowest" (7)]
        R858_SysFreq_Info.MIXER_TOP=12;					//R34[7:4] ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.MIXER_VTL_H=0x49;				//R35[7:0]	Mixer VTL/H = 0.74(4) / 1.24(9)
        R858_SysFreq_Info.MIXER_GAIN_LIMIT=2;			//R46[7:6]	["max=6" (0), "max=8" (1), "max=10" (2), "max=12" (3)]
        
        //Filter
        R858_SysFreq_Info.FILTER_TOP=12;				//R36[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.FILTER_VTL_H=0x49;			//R37[7:0]  Flter VTL/H = 0.74(4) / 1.24(9) v
        R858_SysFreq_Info.FILT_3TH_LPF_GAIN=0;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        
        
        //Discharge
        R858_SysFreq_Info.BB_DIS_CURR=1;                //R40[0]		["x1" (0) , "x1/2" (1)]
        R858_SysFreq_Info.MIXER_FILTER_DIS=0;			//R39[7:6]		["highest" (0), "high" (1), "low" (2), "lowest" (3)]
        R858_SysFreq_Info.BB_DET_MODE=0;				//R39[4]		["average" (0), "peak" (1)]
        
        //Polyphase
        R858_SysFreq_Info.ENB_POLY_GAIN=1;				// R17[3]=0		original ["original" (0), "ctrl by mixamp (>10)" (1)]
        
        
        //NRB  
        R858_SysFreq_Info.NRB_BW_HPF=0;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
        R858_SysFreq_Info.IMG_NRB_ADDER=2;				//R34[2:1]		["original" (0), "top+6" (1), "top+9" (2), "top+11" (3)]
        R858_SysFreq_Info.NRB_TOP=5;					//R33[3:0]     ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.NRB_BW_LPF=0;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
        break;
        
    case R858_ISDB_T_4063:	
    case R858_ISDB_T_4570:	
    case R858_ISDB_T_IF_5M:	
        if(RF_freq<=340000)
        {
            //PW
            R858_SysFreq_Info.NA_PWR_DET = 1;				//R44[1]   ["off" (0), "on" (1)]
            
            //RFBuf
            R858_SysFreq_Info.RF_VTL_H = 0x4A;				//R31[7:0] RF VTL/H = 0.74(4) / 1.34(A)
            
            //Mixer and mixamp
            R858_SysFreq_Info.MIXER_TOP=14;					//R34[7:4] ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
            R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]   ["high" (0), "low" (1)]
            
            //Polyphase
            R858_SysFreq_Info.ENB_POLY_GAIN=0;			// R17[3]		original ["original" (0), "ctrl by mixamp (>10)" (1)]
            
            //NRB
            R858_SysFreq_Info.NRB_BW_HPF=3;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
            R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
            
            //Filter
            R858_SysFreq_Info.FILT_3TH_LPF_GAIN=0;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        }
        else if(RF_freq>=680000 && RF_freq<692000)	//LTE
        {
            //PW
            R858_SysFreq_Info.NA_PWR_DET = 0;				//R44[1]   ["off" (0), "on" (1)]
            
            //RFBuf
            R858_SysFreq_Info.RF_VTL_H=0x4A;				//R31[7:0] RF VTL/H = 0.74(4) / 1.34(A)
            
            //Mixer and mixamp
            R858_SysFreq_Info.MIXER_TOP=14;					//R34[7:4] ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
            R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]   ["high" (0), "low" (1)]
            
            //Polyphase
            R858_SysFreq_Info.ENB_POLY_GAIN=0;			// R17[3]		original ["original" (0), "ctrl by mixamp (>10)" (1)]
            
            //NRB
            R858_SysFreq_Info.NRB_BW_HPF=3;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
            R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
            
            //Filter
            R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
       }      
       else if(RF_freq>=692000 && RF_freq<698000)	//LTE
       {
            //PW
            R858_SysFreq_Info.NA_PWR_DET = 0;				//R44[1]   ["off" (0), "on" (1)]
            
            //RFBuf
            R858_SysFreq_Info.RF_VTL_H = 0x5B;				//R31[7:0] RF VTL/H = 0.84(5) / 1.44(B)
            
            //Mixer and mixamp
            R858_SysFreq_Info.MIXER_TOP=12;					//R34[7:4] ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
            R858_SysFreq_Info.MIXER_AMP_CURRENT=0;			//R10[1]   ["high" (0), "low" (1)]
            
            //Polyphase
            R858_SysFreq_Info.ENB_POLY_GAIN=1;				// R17[3]		original ["original" (0), "ctrl by mixamp (>10)" (1)]
            
            //NRB
            R858_SysFreq_Info.NRB_BW_HPF=0;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
            R858_SysFreq_Info.NRB_BW_LPF=3;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
            
            //Filter
            R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        }
        else//other frequency
        {
            //PW
            R858_SysFreq_Info.NA_PWR_DET = 1;				//R44[1]   ["off" (0), "on" (1)]
            
            //RFBuf
            R858_SysFreq_Info.RF_VTL_H = 0x4A;				//R31[7:0] RF VTL/H = 0.74(4) / 1.34(A)
            
            //Mixer and mixamp
            R858_SysFreq_Info.MIXER_TOP=14;					//R34[7:4] ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
            R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]   ["high" (0), "low" (1)]
            
            //Polyphase
            R858_SysFreq_Info.ENB_POLY_GAIN=0;			// R17[3]		original ["original" (0), "ctrl by mixamp (>10)" (1)]
            
            //NRB
            R858_SysFreq_Info.NRB_BW_HPF=3;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
            R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
            
            //Filter
            R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        }
        
        //PW
        R858_SysFreq_Info.LNA_NRB_DET=0;				//R8[4]    ["on" (0), "off" (1)]
        
        //LNA
        R858_SysFreq_Info.LNA_TOP=5;					//R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control
        R858_SysFreq_Info.LNA_VTL_H=0x5B;				//R30[7:0]  LNA VTL/H=0.84(5) / 1.44(B)
        R858_SysFreq_Info.RF_LTE_PSG=1;					//R32[3]  ["no psg" (0), "7.5dB(5~8)" (1)]
        
        //RFBuf
        R858_SysFreq_Info.RF_TOP=5;						//R32[2:0]  ["7~0" (0~7) ; input: "7~0"]
        R858_SysFreq_Info.RF_GAIN_LIMIT=0;				//R13[6:5]v   ["max =15" (0), "max =11" (1), "max =13" (2), "max =9" (3)]
        
        
        //Mixer and Mixamp
        R858_SysFreq_Info.MIXER_AMP_LPF = 7;			//R47[2:0] ["normal (widest)" (0), "1" (1), "2" (2), "3" (3), "4" (4), "5" (5), "6" (6), "narrowest" (7)]
        R858_SysFreq_Info.MIXER_VTL_H=0x59;				//R35[7:0]	Mixer VTL/H = 0.84(5) / 1.24(9)
        R858_SysFreq_Info.MIXER_GAIN_LIMIT=3;			//R46[7:6] ["max=6" (0), "max=8" (1), "max=10" (2), "max=12" (3)]
        
        //Filter
        R858_SysFreq_Info.FILTER_TOP=12;				//R36[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.FILTER_VTL_H=0x4A;			//R37[7:0]  Flter VTL/H = 0.74(4) / 1.34(A) 
        
        //Discharge
        R858_SysFreq_Info.BB_DIS_CURR=1;                //R40[0]		["x1" (0) , "x1/2" (1)]
        R858_SysFreq_Info.MIXER_FILTER_DIS=0;			//R39[7:6]		["highest" (0), "high" (1), "low" (2), "lowest" (3)]
        R858_SysFreq_Info.BB_DET_MODE=0;				//R39[4]		["average" (0), "peak" (1)]
        
        //NRB  
        R858_SysFreq_Info.NRB_TOP=10;					//R33[3:0]     ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.IMG_NRB_ADDER=2;				//R34[2:1]		["original" (0), "top+6" (1), "top+9" (2), "top+11" (3)]
        
        
        
        break;
        
    case R858_DTMB_8M_4570:
    case R858_DTMB_6M_4500:
    case R858_DTMB_8M_IF_5M:
    case R858_DTMB_6M_IF_5M:
        //PW
        R858_SysFreq_Info.NA_PWR_DET = 1;				//R44[1]   ["off" (0), "on" (1)]
        R858_SysFreq_Info.LNA_NRB_DET=0;				//R8[4]    ["on" (0), "off" (1)]
        
        //LNA
        R858_SysFreq_Info.LNA_TOP=4;					//R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control
        R858_SysFreq_Info.LNA_VTL_H=0x5A;				//R30[7:0] LNA VTL/H = 0.84(5) / 1.34(A)
        R858_SysFreq_Info.RF_LTE_PSG=1;					//R32[3]  ["no psg" (0), "7.5dB(5~8)" (1)]
        
        
        //RFBuf
        R858_SysFreq_Info.RF_TOP=4;						//R32[2:0]  ["7~0" (0~7) ; input: "7~0"]
        R858_SysFreq_Info.RF_VTL_H=0x4A;				//R31[7:0] RF VTL/H = 0.74(4) / 1.34(A)
        R858_SysFreq_Info.RF_GAIN_LIMIT=0;				//R13[6:5]v   ["max =15" (0), "max =11" (1), "max =13" (2), "max =9" (3)]
        
        //Mixer and Mixamp
        R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]	["high" (0), "low" (1)]
        R858_SysFreq_Info.MIXER_AMP_LPF = 7;			//R47[2:0]	["normal (widest)" (0), "1" (1), "2" (2), "3" (3), "4" (4), "5" (5), "6" (6), "narrowest" (7)]
        R858_SysFreq_Info.MIXER_TOP=9;					//R34[7:4]	["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.MIXER_VTL_H=0x49;				//R35[7:0]	Mixer VTL/H = 0.74(4) / 1.24(9)
        R858_SysFreq_Info.MIXER_GAIN_LIMIT=3;			//R46[7:6]	["max=6" (0), "max=8" (1), "max=10" (2), "max=12" (3)]
        
        //Filter
        R858_SysFreq_Info.FILTER_TOP=4;					//R36[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.FILTER_VTL_H=0x49;			//R37[7:0]  Flter VTL/H = 0.74(4) / 1.24(9)
        R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        
        //Discharge
        R858_SysFreq_Info.BB_DIS_CURR=0;                //R40[0]		["x1" (0) , "x1/2" (1)]
        R858_SysFreq_Info.MIXER_FILTER_DIS=2;			//R39[7:6]		["highest" (0), "high" (1), "low" (2), "lowest" (3)]
        R858_SysFreq_Info.BB_DET_MODE=0;				//R39[4]		["average" (0), "peak" (1)]
        
        //Polyphase
        R858_SysFreq_Info.ENB_POLY_GAIN=0;				//R17[3]=0		original ["original" (0), "ctrl by mixamp (>10)" (1)]
        
        //NRB  
        R858_SysFreq_Info.NRB_TOP=4;					//R33[3:0]     ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.NRB_BW_HPF=0;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
        R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
        R858_SysFreq_Info.IMG_NRB_ADDER=2;				//R34[2:1]		["original" (0), "top+6" (1), "top+9" (2), "top+11" (3)]
        
        break;
        
    case R858_ATSC:
    case R858_ATSC_IF_5M:
        //PW
        R858_SysFreq_Info.NA_PWR_DET = 1;				//R44[1]   ["off" (0), "on" (1)]
        R858_SysFreq_Info.LNA_NRB_DET=0;				//R8[4]    ["on" (0), "off" (1)]
        
        //LNA
        R858_SysFreq_Info.LNA_TOP=4;					//R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control
        R858_SysFreq_Info.LNA_VTL_H=0x5A;				//R30[7:0]  LNA VTL/H=0.84(5) / 1.34(A)	
        R858_SysFreq_Info.RF_LTE_PSG=1;					//R32[3]  ["no psg" (0), "7.5dB(5~8)" (1)]
        
        //RFBuf
        R858_SysFreq_Info.RF_TOP=4;						//R32[2:0]  ["7~0" (0~7) ; input: "7~0"]
        R858_SysFreq_Info.RF_VTL_H = 0x5A;				//R31[7:0] RF VTL/H = 0.84(5) / 1.34(A)
        R858_SysFreq_Info.RF_GAIN_LIMIT=0;				//R13[6:5]v   ["max =15" (0), "max =11" (1), "max =13" (2), "max =9" (3)]
        
        
        //Mixer and Mixamp
        R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]   ["high" (0), "low" (1)]
        R858_SysFreq_Info.MIXER_AMP_LPF = 0;			//R47[2:0] ["normal (widest)" (0), "1" (1), "2" (2), "3" (3), "4" (4), "5" (5), "6" (6), "narrowest" (7)]
        R858_SysFreq_Info.MIXER_TOP=9;					//R34[7:4] ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.MIXER_VTL_H=0x5B;				//R35[7:0]	Mixer VTL/H = 0.84(5) / 1.44(B)
        R858_SysFreq_Info.MIXER_GAIN_LIMIT=3;			//R46[7:6]	["max=6" (0), "max=8" (1), "max=10" (2), "max=12" (3)]
        
        
        //Filter
        R858_SysFreq_Info.FILTER_TOP=5;					//R36[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.FILTER_VTL_H=0x4A;			//R37[7:0]  Flter VTL/H = 0.74(4) / 1.34(A)
        R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        
        //Discharge
        R858_SysFreq_Info.BB_DIS_CURR=1;                //R40[0]		["x1" (0) , "x1/2" (1)]
        R858_SysFreq_Info.MIXER_FILTER_DIS=0;			//R39[7:6]		["highest" (0), "high" (1), "low" (2), "lowest" (3)]
        R858_SysFreq_Info.BB_DET_MODE=0;				//R39[4]		["average" (0), "peak" (1)]
        
        
        //Polyphase
        R858_SysFreq_Info.ENB_POLY_GAIN=0;				// R17[3]=0		original ["original" (0), "ctrl by mixamp (>10)" (1)]
        
        
        //NRB  
        R858_SysFreq_Info.NRB_TOP=4;					//R33[3:0]     ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.NRB_BW_HPF=0;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
        R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
        R858_SysFreq_Info.IMG_NRB_ADDER=1;				//R34[2:1]		["original" (0), "top+6" (1), "top+9" (2), "top+11" (3)]
        
        break;
        
    default: //DVB-T
        //PW
        R858_SysFreq_Info.NA_PWR_DET = 1;				//R44[1]   ["off" (0), "on" (1)]
        R858_SysFreq_Info.LNA_NRB_DET=0;				//R8[4]    ["on" (0), "off" (1)]
        
        //LNA
        R858_SysFreq_Info.LNA_TOP=4;					//R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control
        R858_SysFreq_Info.LNA_VTL_H=0x5A;				//R30[7:0] LNA VTL/H = 0.84(5) / 1.34(A)
        R858_SysFreq_Info.RF_LTE_PSG=1;					//R32[3]  ["no psg" (0), "7.5dB(5~8)" (1)]
        
        
        //RFBuf
        R858_SysFreq_Info.RF_TOP=4;						//R32[2:0]  ["7~0" (0~7) ; input: "7~0"]
        R858_SysFreq_Info.RF_VTL_H=0x4A;				//R31[7:0] RF VTL/H = 0.74(4) / 1.34(A)
        R858_SysFreq_Info.RF_GAIN_LIMIT=0;				//R13[6:5]v   ["max =15" (0), "max =11" (1), "max =13" (2), "max =9" (3)]
        
        //Mixer and Mixamp
        R858_SysFreq_Info.MIXER_AMP_CURRENT=1;			//R10[1]	["high" (0), "low" (1)]
        R858_SysFreq_Info.MIXER_AMP_LPF = 7;			//R47[2:0]	["normal (widest)" (0), "1" (1), "2" (2), "3" (3), "4" (4), "5" (5), "6" (6), "narrowest" (7)]
        R858_SysFreq_Info.MIXER_TOP=9;					//R34[7:4]	["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.MIXER_VTL_H=0x49;				//R35[7:0]	Mixer VTL/H = 0.74(4) / 1.24(9)
        R858_SysFreq_Info.MIXER_GAIN_LIMIT=3;			//R46[7:6]	["max=6" (0), "max=8" (1), "max=10" (2), "max=12" (3)]
        
        //Filter
        R858_SysFreq_Info.FILTER_TOP=4;					//R36[7:4]  ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.FILTER_VTL_H=0x49;			//R37[7:0]  Flter VTL/H = 0.74(4) / 1.24(9)
        R858_SysFreq_Info.FILT_3TH_LPF_GAIN=3;			//R17[7:6]	[normal (0), +0.5dB (1), +1dB (2), +1.5dB (3)]
        
        //Discharge
        R858_SysFreq_Info.BB_DIS_CURR=0;                //R40[0]		["x1" (0) , "x1/2" (1)]
        R858_SysFreq_Info.MIXER_FILTER_DIS=2;			//R39[7:6]		["highest" (0), "high" (1), "low" (2), "lowest" (3)]
        R858_SysFreq_Info.BB_DET_MODE=0;				//R39[4]		["average" (0), "peak" (1)]
        
        //Polyphase
        R858_SysFreq_Info.ENB_POLY_GAIN=0;				//R17[3]=0		original ["original" (0), "ctrl by mixamp (>10)" (1)]
        
        //NRB  
        R858_SysFreq_Info.NRB_TOP=4;					//R33[3:0]     ["15-highest ~ 0-lowest" (0~15) ; input: "15~0"]
        R858_SysFreq_Info.NRB_BW_HPF=0;					//R33[5:4]     [lowest (0), low (1), high (2), highest (3)]
        R858_SysFreq_Info.NRB_BW_LPF=2;					//R33[7:6]     [widest (0), wide (1), low (2), lowest (3)]
        R858_SysFreq_Info.IMG_NRB_ADDER=2;				//R34[2:1]		["original" (0), "top+6" (1), "top+9" (2), "top+11" (3)]
        
        break;
        
    } //end switch
    
    return R858_SysFreq_Info;

}




R858_ErrCode R858_Init(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum)
{
    
    UINT8 i;
    
    if(R858_Initial_done_flag[R858_Num][IntTunerNum]==FALSE)
    {
        memset(R858_IMR_Data, 0, sizeof(R858_IMR_Data));
        memset(R858_Fil_Cal_flag, 0, sizeof(R858_Fil_Cal_flag));
        //reset filter cal.
        for (i=0; i<R858_STD_SIZE; i++)
        { 
          R858_Fil_Cal_flag[R858_Num][IntTunerNum][i] = FALSE;
          R858_Fil_Cal_code[R858_Num][IntTunerNum][i] = 0;
          R858_Fil_Cal_BW[R858_Num][IntTunerNum][i] = 0x00;
          R858_Fil_Cal_LpfLsb[R858_Num][IntTunerNum][i] = 0;
        }
        
        if(R858_IMR_done_flag[R858_Num][IntTunerNum]==FALSE)
        {
            //start IMR calibration
            if(R858_Cal_Prepare(R858_Num, IntTunerNum, R858_IMR_CAL) != RT_Success)     
                return RT_Fail;
            
            if(R858_IMR_Process(R858_Num, IntTunerNum) != RT_Success)       //Tuner{N,M}
                return RT_Fail;
            
            R858_IMR_done_flag[R858_Num][IntTunerNum] = TRUE;
        }
        
        //do Xtal check (only tuner1,1)
        if(IntTunerNum==R858_TUNER1 && R858_Num==R858_NUM1)
        {
            if(R858_InitReg(R858_Num, R858_TUNER1) != RT_Success)        
                return RT_Fail;

            if(R858_Xtal_Check(R858_Num, R858_TUNER1) != RT_Success)        //only do tuner1
                return RT_Fail;

            if(R858_Xtal_Pwr_tmp==R858_XTAL_HIGHEST)
                R858_Xtal_Pwr = R858_XTAL_HIGHEST;
            else
                R858_Xtal_Pwr = R858_Xtal_Pwr_tmp + 1;
        }
        
            R858_Initial_done_flag[R858_Num][IntTunerNum] = TRUE;
        
    } //end if(check init flag)
        
   //write initial reg
   if(R858_InitReg(R858_Num, IntTunerNum) != RT_Success)        
       return RT_Fail;

   R858_pre_standard[R858_Num][IntTunerNum] = R858_STD_SIZE;
   
   
    return RT_Success;
}



R858_ErrCode R858_InitReg(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum)
{
    UINT8 InitArrayCunt = 0;
    
    //Tuner1 R42~R47 is common part, must set in initial stage
    for(InitArrayCunt = 42; InitArrayCunt<R858_REG_NUM; InitArrayCunt ++)
    {
        R858_I2C_Len.Data[InitArrayCunt-42] = R858_iniArray[R858_TUNER1][InitArrayCunt];
        R858_Array[R858_Num][R858_TUNER1][InitArrayCunt] = R858_iniArray[R858_TUNER1][InitArrayCunt];
    }
    R858_I2C_Len.RegAddr = 42;
    R858_I2C_Len.Len = 6;
    R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
    if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
        return RT_Fail;
    
    //Tuner2 R42~R47 is common part, must set in initial stage
    for(InitArrayCunt = 42; InitArrayCunt<R858_REG_NUM; InitArrayCunt ++)
    {
        R858_I2C_Len.Data[InitArrayCunt-42] = R858_iniArray[R858_TUNER2][InitArrayCunt];
        R858_Array[R858_Num][R858_TUNER2][InitArrayCunt] = R858_iniArray[R858_TUNER2][InitArrayCunt];
    }
    R858_I2C_Len.RegAddr = 42;
    R858_I2C_Len.Len = 6;
    R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER2];
    if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
    	return RT_Fail;
    
    
    
    //write I2C len to R8~R41 of tuner1 or tuner2
    for(InitArrayCunt = 0; InitArrayCunt<(R858_REG_NUM-6); InitArrayCunt ++)
    {
        R858_I2C_Len.Data[InitArrayCunt] = R858_iniArray[IntTunerNum][InitArrayCunt];
        R858_Array[R858_Num][IntTunerNum][InitArrayCunt] = R858_iniArray[IntTunerNum][InitArrayCunt];
    }
    R858_I2C_Len.RegAddr = 0;
    R858_I2C_Len.Len = R858_REG_NUM-6;
    R858_I2C_Len.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];
    if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
        return RT_Fail;
    
    return RT_Success;
}




R858_ErrCode R858_InitUserMode(R858_Set_Info R858_INFO)
{
    R858_ExtTunerNum_Type ExtNum = R858_NUM1;
    R858_IntTunerNum_Type  IntNum = R858_TUNER2;
    int i=0;
    
    if(R858_INFO.R858_User_Mode==R858_DUAL)
    {
        for(i=0; i<3; i++)  //set tuner {1,2}{2,1}{2,2}
        {
            if(i==0)
            {
                ExtNum = R858_NUM1;
                IntNum = R858_TUNER2;			
            }
            else if(i==1)
            {
                ExtNum = R858_NUM2;
                IntNum = R858_TUNER1;			
            }
            else 
            {
                ExtNum = R858_NUM2;
                IntNum = R858_TUNER2;			
            }
            
            //Tuner{1,2},{2,1},{2,2}: Xtal gm off, xtal cap=0, clk_out_en
            R858_I2C.I2cAddr = R858_ADDRESS[ExtNum][IntNum]; //{1,2}
            R858_I2C.RegAddr = 29;
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] | 0x02;   //xtal gm off
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xFE; //cap_xx=0
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xBF; //clk out en
            R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
            if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
                return RT_Fail;
            
            R858_I2C.RegAddr = 28;
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0x87; //cap_x=0	
            R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];	
            if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
            	return RT_Fail;
            
            //do filter cal
            //R858_SetStandard(ExtNum, IntNum, R858_INFO.R858_Standard);
            
        }  //end for(i) loop
        
        //Tuner1: LT 
        ExtNum = R858_NUM1;
        IntNum = R858_TUNER1;	
        R858_I2C.I2cAddr = R858_ADDRESS[ExtNum][IntNum]; //{1,1}
        
        if(R858_INFO.R858_LT==LT_ON)
        {
            R858_I2C.RegAddr = 42;
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = (R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xF9) | 0x06; //active LT & pow on
            R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
            if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
            	return RT_Fail;
        }
        else
        {
            R858_I2C.RegAddr = 42;
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = (R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xF9) | 0x00; //off LT & pow off
            R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
            if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
                return RT_Fail;		
        }
        
        //Tuner2: LT off
        ExtNum = R858_NUM2;
        IntNum = R858_TUNER1;	
        R858_I2C.I2cAddr = R858_ADDRESS[ExtNum][IntNum]; //{2,1}
        R858_I2C.RegAddr = 42;
        R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = (R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xF9) | 0x00; //off LT & pow off
        R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
        if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
            return RT_Fail;	
    }
    else  //single R858
    {
        //Tuner{1,2}: Xtal gm off, xtal cap=0, clk_out_en
        ExtNum = R858_NUM1;
        IntNum = R858_TUNER2;		
        R858_I2C.I2cAddr = R858_ADDRESS[ExtNum][IntNum]; //{1,2}
        
        R858_I2C.RegAddr = 29;
        R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] | 0x02;   //xtal gm off
        R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xFE; //cap_xx=0
        R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xBF; //clk out en
        R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
        if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        R858_I2C.RegAddr = 28;
        R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0x87; //cap_x=0	
        R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];	
        if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        //Tuner1: LT 
        ExtNum = R858_NUM1;
        IntNum = R858_TUNER1;	
        R858_I2C.I2cAddr = R858_ADDRESS[ExtNum][IntNum]; //{1,1}
        
        if(R858_INFO.R858_LT==LT_ON)
        {
            R858_I2C.RegAddr = 42;
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = (R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xF9) | 0x06; //active LT & pow on
            R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
            if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
                return RT_Fail;
        }
        else
        {
            R858_I2C.RegAddr = 42;
            R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] = (R858_Array[ExtNum][IntNum][R858_I2C.RegAddr] & 0xF9) | 0x00; //off LT & pow off
            R858_I2C.Data = R858_Array[ExtNum][IntNum][R858_I2C.RegAddr];
            if(I2C_Write(ExtNum, &R858_I2C) != RT_Success)
                return RT_Fail;		
        }		
    }
    
    return RT_Success;
}

R858_ErrCode R858_Xtal_Check(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum)
{
    UINT8 i = 0;
    UINT8 vco_check_bank = 49;
    UINT8 vco_bank = 0;
    
    R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];
    
    //Set PLL auto tune fatest (64k), R38[3:2]=00
    R858_I2C.RegAddr  = 38;								
    R858_Array[R858_Num][IntTunerNum][38] = (R858_Array[R858_Num][IntTunerNum][38] & 0xF3) | 0x00;
       R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][38];
       if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //Set PLL related parameter
    R858_Array[R858_Num][IntTunerNum][21] = (R858_Array[R858_Num][IntTunerNum][21] & 0x80) | 16;     //Ni=16
    R858_Array[R858_Num][IntTunerNum][25] = (R858_Array[R858_Num][IntTunerNum][25] & 0xFC) | 2;      //Si=2
    R858_Array[R858_Num][IntTunerNum][26] = (R858_Array[R858_Num][IntTunerNum][26] & 0xF8) | 2;      //div 8 (div2???)
    R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xCF) | 0x00;  //Xtal div1
    //R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xFD) | 0x00;  //gm on
    //R858_Array[R858_Num][IntTunerNum][47] = (R858_Array[R858_Num][IntTunerNum][47] & 0x7F) | 0x00;  //tuner1 gm2 on (???)
    
    R858_I2C.RegAddr = 21;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][21];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = 25;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][25];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = 26;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][26];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = 29;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][29];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //Set larget cap (41p)
    R858_I2C.RegAddr = 28;
    R858_Array[R858_Num][IntTunerNum][28] = R858_Array[R858_Num][IntTunerNum][28] | 0xF8;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][28];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = 29;
    R858_Array[R858_Num][IntTunerNum][29] = R858_Array[R858_Num][IntTunerNum][29] | 0x01;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][29];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //Set manual VCO bank: 1 000000;	
    R858_I2C.RegAddr = 24;
    R858_Array[R858_Num][IntTunerNum][24] = (R858_Array[R858_Num][IntTunerNum][24] & 0x80) | 0x40;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][24];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //set auto
    R858_I2C.RegAddr = 24;
    R858_Array[R858_Num][IntTunerNum][24] = (R858_Array[R858_Num][IntTunerNum][24] & 0xBF);
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][24];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
    	return RT_Fail;
    
    for(i=0; i<R858_XTAL_CHECK_SIZE; i++)  //(i=0~3)
    {
        //set power,  from weak to strong 
        R858_I2C.RegAddr = 29;
        R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xF3) | ((UINT8)(R858_XTAL_HIGHEST-i)<<2);
        R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][29];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        	return RT_Fail;
        
        R858_Delay_MS(R858_Num, XTAL_CHK_DELAY);
        
        R858_I2C_Len.RegAddr = 0x00;
        R858_I2C_Len.Len = 6;  //R5[6:1]
        R858_I2C_Len.I2cAddr = R858_ADDRESS[R858_Num][R858_IntTunerNum];
        if(I2C_Read_Len(R858_Num , &R858_I2C_Len) != RT_Success)
        {
        	if(I2C_Read_Len(R858_Num , &R858_I2C_Len) != RT_Success)
        		return RT_Fail;
        }
        
        //lock to VCO band ??? if VCO=3792 (LO=474MHz)
        vco_bank = (R858_I2C_Len.Data[5] & 0x7E)>>1;
        if(((R858_I2C_Len.Data[5] & 0x01) == 0x01) && (vco_bank <= (vco_check_bank + 6)) && (vco_bank >= (vco_check_bank-6))) 
        {
            R858_Xtal_Pwr_tmp = i;
            break;
        }
        
          if(i==(R858_XTAL_CHECK_SIZE-1))
        {
              R858_Xtal_Pwr_tmp = i;
        }
    }
    
       return RT_Success;
}	
R858_ErrCode R858_Cal_Prepare(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 u1CalFlag)
{
     UINT8   InitArrayCunt = 0;
     R858_Set_Info R858_INFO;
    
     //do not write both tuner at the same time, or filter cal will affect another tuner 
    
     //Write Full Table, include PLL & RingPLL all settings
     R858_I2C_Len.RegAddr = 0;
     R858_I2C_Len.Len = R858_REG_NUM;
    
     switch(u1CalFlag)
     {
         case R858_IMR_CAL:
        
            //Tuner1 R42~R47 is common part, must set in initial stage
            for(InitArrayCunt = 42; InitArrayCunt<R858_REG_NUM; InitArrayCunt ++)
            {
                R858_I2C_Len.Data[InitArrayCunt-42] = R858_IMR_CAL_Array[R858_TUNER1][InitArrayCunt];
                R858_Array[R858_Num][R858_TUNER1][InitArrayCunt] = R858_IMR_CAL_Array[R858_TUNER1][InitArrayCunt];
            }
            R858_I2C_Len.RegAddr = 42;
            R858_I2C_Len.Len = 6;
            R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
            if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
            	return RT_Fail;
            
            //Tuner2 R42~R47 is common part, must set in initial stage
            for(InitArrayCunt = 42; InitArrayCunt<R858_REG_NUM; InitArrayCunt ++)
            {
                R858_I2C_Len.Data[InitArrayCunt-42] = R858_IMR_CAL_Array[R858_TUNER2][InitArrayCunt];
                R858_Array[R858_Num][R858_TUNER2][InitArrayCunt] = R858_IMR_CAL_Array[R858_TUNER2][InitArrayCunt];
            }
            R858_I2C_Len.RegAddr = 42;
            R858_I2C_Len.Len = 6;
            R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER2];
            if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
            	return RT_Fail;
            
            //write I2C len to R8~R41 of tuner1 or tuner2
            for(InitArrayCunt = 8; InitArrayCunt<(R858_REG_NUM-6); InitArrayCunt ++)
            {
                R858_I2C_Len.Data[InitArrayCunt-8] = R858_IMR_CAL_Array[IntTunerNum][InitArrayCunt];
                R858_Array[R858_Num][IntTunerNum][InitArrayCunt] = R858_IMR_CAL_Array[IntTunerNum][InitArrayCunt];
            }
            R858_I2C_Len.RegAddr = 8;
            R858_I2C_Len.Len = R858_REG_NUM-8-6;
            R858_I2C_Len.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];
            if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
            	return RT_Fail;
            
            break;
            
         case R858_LPF_CAL:
            //Tuner1 R42~R47 is common part, must set in initial stage
            for(InitArrayCunt = 42; InitArrayCunt<R858_REG_NUM; InitArrayCunt ++)
            {
                R858_I2C_Len.Data[InitArrayCunt-42] = R858_LPF_CAL_Array[R858_TUNER1][InitArrayCunt];
                R858_Array[R858_Num][R858_TUNER1][InitArrayCunt] = R858_LPF_CAL_Array[R858_TUNER1][InitArrayCunt];
            }
            R858_I2C_Len.RegAddr = 42;
            R858_I2C_Len.Len = 6;
            R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
            if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
            	return RT_Fail;
            
            //Tuner2 R42~R47 is common part, must set in initial stage
            for(InitArrayCunt = 42; InitArrayCunt<R858_REG_NUM; InitArrayCunt ++)
            {
                R858_I2C_Len.Data[InitArrayCunt-42] = R858_LPF_CAL_Array[R858_TUNER2][InitArrayCunt];
                R858_Array[R858_Num][R858_TUNER2][InitArrayCunt] = R858_LPF_CAL_Array[R858_TUNER2][InitArrayCunt];
            }
            R858_I2C_Len.RegAddr = 42;
            R858_I2C_Len.Len = 6;
            R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER2];
            if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
            	return RT_Fail;
            
            //write I2C len to R8~R41 of tuner1 or tuner2
            for(InitArrayCunt = 8; InitArrayCunt<(R858_REG_NUM-6); InitArrayCunt ++)
            {
                R858_I2C_Len.Data[InitArrayCunt-8] = R858_LPF_CAL_Array[IntTunerNum][InitArrayCunt];
                R858_Array[R858_Num][IntTunerNum][InitArrayCunt] = R858_LPF_CAL_Array[IntTunerNum][InitArrayCunt];
            }
            R858_I2C_Len.RegAddr = 8;
            R858_I2C_Len.Len = R858_REG_NUM-6-8;
            R858_I2C_Len.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];
            if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
                return RT_Fail;
            
            break;
            
         default:
        	 break;
     }
    
     //ring clk must enable R29[7]=0 of both tuner 1 & 2
     R858_I2C.RegAddr=29;
     R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
     R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0x7F;
     R858_I2C.Data=R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr];
     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
     R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER2];
     R858_Array[R858_Num][R858_TUNER2][R858_I2C.RegAddr] = R858_Array[R858_Num][R858_TUNER2][R858_I2C.RegAddr] & 0x7F;
     R858_I2C.Data=R858_Array[R858_Num][R858_TUNER2][R858_I2C.RegAddr];
     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    
     R858_INFO.R858_User_Mode=R858_User_Mode;
     R858_INFO.R858_LT=LT_OFF;
     R858_InitUserMode(R858_INFO);
    
         return RT_Success;
}




R858_ErrCode R858_IMR_Process(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum)
{
    if(IntTunerNum==R858_TUNER1)
    {
        //Select det2=tuner1 in Tuner1 R46[1]
        R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
        R858_I2C.RegAddr = 46;
        R858_Array[R858_Num][R858_TUNER1][46] = (R858_Array[R858_Num][R858_TUNER1][46] & 0xFD) | 0x00; //tuner1
        R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][46];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        //Set Ring to RFBuf in R14[1], Tuner1 ON
        R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
        R858_I2C.RegAddr = 14;
        R858_Array[R858_Num][R858_TUNER1][14] = (R858_Array[R858_Num][R858_TUNER1][14] & 0xFD) | 0x02; //on
        R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][14];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        //Set Ring to RFBuf in R14[1], Tuner2 OFF
        R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER2];
        R858_I2C.RegAddr = 14;
        R858_Array[R858_Num][R858_TUNER2][14] = (R858_Array[R858_Num][R858_TUNER2][14] & 0xFD) | 0x00; //off
        R858_I2C.Data = R858_Array[R858_Num][R858_TUNER2][14];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
    }
    else
    {
        //Select det2=tuner2 in Tuner1 R46
        R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
        R858_I2C.RegAddr = 46;
        R858_Array[R858_Num][R858_TUNER1][46] = (R858_Array[R858_Num][R858_TUNER1][46] & 0xFD) | 0x02;  //tuner2
        R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][46];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        //Set Ring to RFBuf in R14[1], Tuner1 Off
        R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
        R858_I2C.RegAddr = 14;
        R858_Array[R858_Num][R858_TUNER1][14] = (R858_Array[R858_Num][R858_TUNER1][14] & 0xFD) | 0x00; //off
        R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][14];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        //Set Ring to RFBuf in R14[1], Tuner2 On
        R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER2];
        R858_I2C.RegAddr = 14;
        R858_Array[R858_Num][R858_TUNER2][14] = (R858_Array[R858_Num][R858_TUNER2][14] & 0xFD) | 0x02; //on
        R858_I2C.Data = R858_Array[R858_Num][R858_TUNER2][14];
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
    }
    
		if(R858_IMR(R858_Num, IntTunerNum, 2, TRUE) != RT_Success)       //Full K node 
			return RT_Fail;
		
		
		if(R858_IMR(R858_Num, IntTunerNum, 1, FALSE) != RT_Success)
			return RT_Fail;
			
		
		if(R858_IMR(R858_Num, IntTunerNum, 0, FALSE) != RT_Success)
			return RT_Fail;
					
		
		if(R858_IMR(R858_Num, IntTunerNum, 3, FALSE) != RT_Success)
				return RT_Fail;

    return RT_Success;
}
R858_ErrCode R858_IMR(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 IMR_MEM, UINT8 IM_Flag)
{
    UINT32 RingVCO = 0;
    UINT32 RingFreq = 0;
    //UINT8  u1MixerGain = 8;
    UINT32 RingRef = R858_Xtal;
    UINT8   divnum_ring = 0;
    
    R858_SectType IMR_POINT;
    
    if(R858_Xtal==16000)  //16M
    {
        divnum_ring = 25;    
    }
    else if(R858_Xtal==24000) //24M
    {
        if(IMR_MEM==2)
            divnum_ring = 16;
        else
            divnum_ring = 17;
    }
    else //not support
    {
        return RT_Fail;
    }
    
     RingVCO = (divnum_ring)* 8 * RingRef;
     //RingFreq = RingVCO/48;
    
     switch(IMR_MEM)
    {
    case 0: // RingFreq = 68M
		RingFreq = RingVCO/32;
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0xE7) | (0<<3);  // ring_div1 /6 (2),    R42[4:3]
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0x9F) | (3<<5);  // ring_div2 /8 (3),    R42[6:5]
		break;
	case 1: // RingFreq = 326M
		RingFreq = RingVCO/10;
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0xE7) | (1<<3);   // ring_div1 /5 (1)	
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0x9F) | (1<<5);   // ring_div2 /2 (1)	
		break;
	case 2: // RingFreq = 544M
		RingFreq = RingVCO/6;
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0xE7) | (2<<3);  // ring_div1 /6 (2)	
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0x9F) | (0<<5);  // ring_div2 /1 (0)	
		break;
	case 3: // RingFreq = 816M
		RingFreq = RingVCO/4;
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0xE7) | (0<<3);   // ring_div1 /4 (0)	
		R858_Array[R858_Num][R858_TUNER2][42] = (R858_Array[R858_Num][R858_TUNER2][42] & 0x9F) | (0<<5);   // ring_div2 /1 (0)	
		break;
	}	 

	 if(RingVCO>=3200000)	
		R858_Array[R858_Num][R858_TUNER2][42] &= 0xFB;   //high vco_band
	 else	
		R858_Array[R858_Num][R858_TUNER2][42] |= 0x04;      //low vco_band
		 
	 R858_Array[R858_Num][R858_TUNER2][43] = (R858_Array[R858_Num][R858_TUNER2][43] & 0xE0) | divnum_ring; 

	 //------- Set RingPLL I2C in Tuner2 reg ----------//
	 R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER2];	  

	 R858_I2C.RegAddr = 42;
	 R858_I2C.Data = R858_Array[R858_Num][R858_TUNER2][42];
	 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		 return RT_Fail;

	 R858_I2C.RegAddr = 43;
	 R858_I2C.Data = R858_Array[R858_Num][R858_TUNER2][43];
	 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		 return RT_Fail;

	//-----------------------------------------------------//
	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  

	//Must do MUX before PLL() 
	if(R858_MUX(R858_Num, IntTunerNum, RingFreq + R858_IMR_IF, RingFreq, R858_STD_SIZE) != RT_Success)      //IMR MUX (LO, RF)
		return RT_Fail;

	if(R858_PLL(R858_Num, IntTunerNum, (RingFreq + R858_IMR_IF), R858_STD_SIZE) != RT_Success)  //IMR PLL
	    return RT_Fail;

	//Img_R = reverse
	R858_I2C.RegAddr = 17;
	R858_Array[R858_Num][IntTunerNum][17] = (R858_Array[R858_Num][IntTunerNum][17] | 0x04);  //R17[2]=1
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][17];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	//Mixer Amp LPF???

	//clear IQ_cap
	IMR_POINT.Iqcap = 0; 

	if(IM_Flag == TRUE)
	{
	     if(R858_IQ(R858_Num, IntTunerNum, &IMR_POINT) != RT_Success)
		    return RT_Fail;
	}
	else
	{		
	
	if((IMR_MEM==1) || (IMR_MEM==3))		
	{		
		IMR_POINT.Gain_X = R858_IMR_Data[R858_Num][IntTunerNum][2].Gain_X;   //node 3
		IMR_POINT.Phase_Y = R858_IMR_Data[R858_Num][IntTunerNum][2].Phase_Y;			
		IMR_POINT.Value = R858_IMR_Data[R858_Num][IntTunerNum][2].Value;		
	}		
	else if(IMR_MEM==0)		
	{			
		IMR_POINT.Gain_X = R858_IMR_Data[R858_Num][IntTunerNum][1].Gain_X;  			
		IMR_POINT.Phase_Y = R858_IMR_Data[R858_Num][IntTunerNum][1].Phase_Y;			
		IMR_POINT.Value = R858_IMR_Data[R858_Num][IntTunerNum][1].Value;		
	}
		//IMR_POINT.Iqcap = R858_IMR_Data[R858_Num][IntTunerNum][3].Iqcap;
		if(R858_F_IMR(R858_Num, IntTunerNum, &IMR_POINT) != RT_Success)
			return RT_Fail;
	}

	//Save IMR Value
	switch(IMR_MEM)
	{
	case 0:
		R858_IMR_Data[R858_Num][IntTunerNum][0].Gain_X  = IMR_POINT.Gain_X;
		R858_IMR_Data[R858_Num][IntTunerNum][0].Phase_Y = IMR_POINT.Phase_Y;
		R858_IMR_Data[R858_Num][IntTunerNum][0].Value = IMR_POINT.Value;
		R858_IMR_Data[R858_Num][IntTunerNum][0].Iqcap = IMR_POINT.Iqcap;		
		break;
	case 1:
		R858_IMR_Data[R858_Num][IntTunerNum][1].Gain_X  = IMR_POINT.Gain_X;
		R858_IMR_Data[R858_Num][IntTunerNum][1].Phase_Y = IMR_POINT.Phase_Y;
		R858_IMR_Data[R858_Num][IntTunerNum][1].Value = IMR_POINT.Value;
		R858_IMR_Data[R858_Num][IntTunerNum][1].Iqcap = IMR_POINT.Iqcap;
		break;
	case 2:
		R858_IMR_Data[R858_Num][IntTunerNum][2].Gain_X  = IMR_POINT.Gain_X;
		R858_IMR_Data[R858_Num][IntTunerNum][2].Phase_Y = IMR_POINT.Phase_Y;
		R858_IMR_Data[R858_Num][IntTunerNum][2].Value = IMR_POINT.Value;
		R858_IMR_Data[R858_Num][IntTunerNum][2].Iqcap = IMR_POINT.Iqcap;
		break;
	case 3:
		R858_IMR_Data[R858_Num][IntTunerNum][3].Gain_X  = IMR_POINT.Gain_X;
		R858_IMR_Data[R858_Num][IntTunerNum][3].Phase_Y = IMR_POINT.Phase_Y;
		R858_IMR_Data[R858_Num][IntTunerNum][3].Value = IMR_POINT.Value;
		R858_IMR_Data[R858_Num][IntTunerNum][3].Iqcap = IMR_POINT.Iqcap;
		break;
	default:
		R858_IMR_Data[R858_Num][IntTunerNum][3].Gain_X  = IMR_POINT.Gain_X;
		R858_IMR_Data[R858_Num][IntTunerNum][3].Phase_Y = IMR_POINT.Phase_Y;
		R858_IMR_Data[R858_Num][IntTunerNum][3].Value = IMR_POINT.Value;
		R858_IMR_Data[R858_Num][IntTunerNum][3].Iqcap = IMR_POINT.Iqcap;
		break;
	}
	return RT_Success;
}


R858_ErrCode R858_PLL(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 LO_Freq, R858_Standard_Type R858_Standard)
{
    UINT8  MixDiv = 2;
    UINT8  DivBuf = 0;
    UINT8  Ni = 0;
    UINT8  Si = 0;
    UINT8  DivNum = 0;
    UINT16  Nint = 0;
    UINT32 VCO_Min = 2200000;
    UINT32 VCO_Max = 4400000;
    UINT32 VCO_Freq = 0;
    UINT32 PLL_Ref	= R858_Xtal;		
    UINT32 VCO_Fra	= 0;		
    UINT16 Nsdm = 2;
    UINT16 SDM = 0;
    UINT16 SDM16to9 = 0;
    UINT16 SDM8to1 = 0;
    UINT8   XTAL_POW = 0;
    UINT16  u2XalDivJudge;
    UINT8   u1XtalDivRemain;
    UINT8   CP_CUR = 0x00;
    UINT8   CP_OFFSET = 0x00;
    UINT8   SDM_RES = 0x00;
    UINT8   NS_RES = 0x00;
    UINT8   IQGen_Cur = 0;    //DminDmin
    UINT8   IQBias = 1;           //BiasI
    UINT8   IQLoad = 0;          //3.2k/2
    UINT8   OutBuf_Bias = 0;   //max
    UINT8   BiasHf = 0;            //135u
    UINT8	PllArrayCunt = 0;
    
    //Assign device address
    R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
    R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
    
    // VCO current = 0 (max)
    R858_I2C.RegAddr = 25;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | 0x00; // R25[7:6] = 0
    
    // VCO power = auto
    R858_I2C.RegAddr = 24;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x7F) | 0x80; // R24[7] = 1
    
    // HfDiv Buf = 6dB buffer
    R858_I2C.RegAddr = 26;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x7F) | 0x00; // R26[7]=0
    
    // Divider HfDiv current = 135u
    R858_I2C.RegAddr = 27;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xE7) | 0x00; // R27[4:3]=0
    //R858_I2C_Len.Data[R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    /*R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;*/
    
    // PLL LDOA=2.2V
    R858_I2C.RegAddr = 25;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF3) | 0x00; // R25[3:2]=00
    
    // PFD DLDO=4mA
    R858_I2C.RegAddr = 47;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | 0x00; // R47[6:5]=00
    
    // DLDO2=3mA
    R858_I2C.RegAddr = 25;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFC) | 0x01; // R25[1:0]=01
    
    // HF Fiv LDO=7mA (new bonding set this off)
    R858_I2C.RegAddr = 26;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xE7) | 0x00; // R26[4:3]=00
    
    /*gm Tuner #1
    R29[1],R47[7] 
    00: 24 MHz
    01: 16 MHz
    10: 16 MHz
    11: OFF
    */
    /*gm Tuner #2 
    R29[1] ( set off forever )
    0: on
    1: off
    */
    
      //------ Xtal freq depend setting: Xtal Gm & AGC ref clk --------//
    if(R858_Xtal==24000)
    {
        if(IntTunerNum==R858_TUNER1)
        {
            R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xFD) | 0x00;  //gm*2(24)  R29[1]=0
            R858_Array[R858_Num][IntTunerNum][47] = (R858_Array[R858_Num][IntTunerNum][47] & 0x7F) | 0x00;  //gm*2(24)  R47[7]=0
        }
        else
        {
            R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xFD) | 0x02;  //gm*2(24)  R29[1]=1 (???)
        }
        R858_Array[R858_Num][IntTunerNum][47] = (R858_Array[R858_Num][IntTunerNum][47] & 0xDF) | 0x20;  //clk /3 (24) => R47[4]=1 
    }
    else if(R858_Xtal==16000)
    {
        if(IntTunerNum==R858_TUNER1)
        {
        	R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xFD) | 0x02;  //gm*2(24)  R29[1] =1
        	R858_Array[R858_Num][IntTunerNum][47] = (R858_Array[R858_Num][IntTunerNum][47] & 0x7F) | 0x00;  //gm*2(24)  R47[7] =0
        }
        else
        {
        	R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xFD) | 0x02;  //gm*2(24)  R29[1] =1
        }
        R858_Array[R858_Num][IntTunerNum][47] = (R858_Array[R858_Num][IntTunerNum][47] & 0xDF) | 0x00;  //clk /2 (16)	=> R47[4]=0
    }
    else  //not support Xtal freq
        return RT_Fail;
    
    
    //----- Set XTAL Power ------//
#if(R858_SHARE_XTAL==TRUE)
    XTAL_POW = 0;        //highest,       R32[2:1]=0
#else
    
    if(LO_Freq<100000) // Low (10)
    {
        if(R858_Xtal_Pwr <= R858_XTAL_LOW )
        {
            XTAL_POW = 2;  //low
        }
        else
        {
            XTAL_POW = (UINT8) (R858_XTAL_HIGHEST - R858_Xtal_Pwr);
        }
    }
    else if((100000<=LO_Freq) && (LO_Freq<600000)) // High
    {
        if(R858_Xtal_Pwr <= R858_XTAL_HIGH )
        {
            XTAL_POW = 1;   //high
        }
        else
        {
            XTAL_POW = 0;
        }
    }
    else // Highest
    {
        XTAL_POW = 0;  //highest
    }
    
#endif

    R858_I2C.RegAddr = 29;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF3) | (XTAL_POW<<2);	//R32[3:2]
    
    CP_CUR = 0x00;     //0.7m, R27[2:0]=000
    CP_OFFSET = 0x00;  //0u,     R38[1]=0
    if(R858_Xtal == 24000)
    {
        u2XalDivJudge = (UINT16) ((LO_Freq+R858_IF_GOLOBAL)/1000/12);
    //  48, 120 ,264 ,288, 336
        if((u2XalDivJudge==4)||(u2XalDivJudge==10)||(u2XalDivJudge==22)||(u2XalDivJudge==24)||(u2XalDivJudge==28))
        {
        	CP_OFFSET = 0x02;  //30u,     R38[1]=1
        }
        else
        {
        	CP_OFFSET = 0x00;  //0u,     R38[1]=0
        }
    }
    else if(R858_Xtal == 16000)
    {
        u2XalDivJudge = (UINT16) ((LO_Freq+R858_IF_GOLOBAL)/1000/8);
        //
        if((u2XalDivJudge==6)||(u2XalDivJudge==10)||(u2XalDivJudge==12)||(u2XalDivJudge==48))
        {
            CP_OFFSET = 0x02;  //30u,     R38[1]=1
        }
        else
        {
            CP_OFFSET = 0x00;  //0u,     R38[1]=0
        }
    }
    
    R858_I2C.RegAddr = 27;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF8) | (CP_CUR);	//    => R27[2:0]
    
    R858_I2C.RegAddr = 38;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFD) | (CP_OFFSET);	//    => R38[1]
    
    //set pll autotune = 64kHz (fast)  R38[3:2] = 00
    R858_I2C.RegAddr = 38;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF3;
    
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
    
    //IQ Gen block & BiasHF & NS_RES & SDM_Res
    if(MixDiv <= 4)  //Div=2,4
    {
        IQGen_Cur = 0;    //DminDmin
        IQBias = 1;           //BiasI   
        IQLoad = 0;          //3.2k/2 
        OutBuf_Bias = 0;   //0 (max)     
        BiasHf = 0;           //135u
        SDM_RES = 0;     //short  
        NS_RES = 0;        //0R 
    }
    else if(MixDiv == 8) 
    {
        IQGen_Cur = 0;    //DminDmin
        IQBias = 0;           //BiasI/2   
        IQLoad = 1;          //3.2k
        OutBuf_Bias = 1;   //1 
        BiasHf = 1;           //110u
        SDM_RES = 0;     //short
        NS_RES = 1;        //800R 
    }
    else if(MixDiv == 16) 
    {
        IQGen_Cur = 0;    //DminDmin
        IQBias = 0;           //BiasI/2   
        IQLoad = 1;          //3.2k
        OutBuf_Bias = 2;   //2 
        BiasHf = 1;           //110u
        SDM_RES = 0;     //short  
        NS_RES = 0;        //0R
    }
    else if(MixDiv >= 32) //32, 64
    {
        IQGen_Cur = 0;    //DminDmin
        IQBias = 0;           //BiasI/2   
        IQLoad = 1;          //3.2k
        OutBuf_Bias = 3;   //3 (min)
        BiasHf = 1;           //110u
        SDM_RES = 0;     //short
        NS_RES = 0;        //0R
    }
    else
    {
        return RT_Fail;
    }
    
    
    if(Divider_Manual==1)
    {
    	
    }
    else
    {
        //Xtal Div setting
        if(R858_Standard == R858_STD_SIZE) //for cal, RingPLL not support xtal div2
        {
            R858_XtalDiv = XTAL_DIV1; //div1 for calibration
        }
        else  //DTV
        {
            if(R858_Xtal==24000)
               u2XalDivJudge = (UINT16) ((LO_Freq + R858_IF_GOLOBAL)/1000/12);
            else if(R858_Xtal==16000)
               u2XalDivJudge = (UINT16) ((LO_Freq + R858_IF_GOLOBAL)/1000/8);
            else
               return RT_Fail;
            u1XtalDivRemain = (UINT8) (u2XalDivJudge % 2);
            
            if(LO_Freq < (372000+R858_IF_HIGH))	  
            {
                if(u1XtalDivRemain==1) //odd
                {
                	R858_XtalDiv = XTAL_DIV1;
                }
                else  //even, spur area
                {
                	R858_XtalDiv = XTAL_DIV1_2;
                }
            }
            else
            {
                R858_XtalDiv = XTAL_DIV1;
            }
        }
    }
    
    
    
    //R858_XtalDiv = XTAL_DIV2; //div 1, 2, 4
    if(R858_XtalDiv==XTAL_DIV1)
    {
        PLL_Ref = R858_Xtal;	         
        R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xCF) | 0x00; //1st_div2=0, 2nd_div2=0	 R29[5:4]
    }
    else if(R858_XtalDiv==XTAL_DIV1_2)
    {
        PLL_Ref = R858_Xtal/2;   
        R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xCF) | 0x20; //1st_div2=0(R29[4]), 2nd_div2=1(R29[5])
    }
    else if(R858_XtalDiv==XTAL_DIV2_1)
    {
        PLL_Ref = R858_Xtal/2;   
        R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xCF) | 0x10; //1st_div2=1(R29[4]), 2nd_div2=0(R29[5])
    }
    else if(R858_XtalDiv==XTAL_DIV4)  //24MHz
    {
        PLL_Ref = R858_Xtal/4;
        R858_Array[R858_Num][IntTunerNum][29] = (R858_Array[R858_Num][IntTunerNum][29] & 0xCF) | 0x30; //2nd_div2=1, 1st_div2=1  R29[5:4]
    }
    
    //IQ gen current R10[6]
    R858_I2C.RegAddr = 10;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xBF) | (IQGen_Cur<<6); 
    
    //Out Buf Bias R41[7:6]
    R858_I2C.RegAddr = 41;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | (OutBuf_Bias<<6); 
    
    //BiasI R41[5]
    R858_I2C.RegAddr = 41;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xDF) | (IQBias<<5); 
    
    //IQLoad R41[4]
    R858_I2C.RegAddr = 41;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xEF) | (IQLoad<<4); 
    
    //BiasHF R26[6:5]
    R858_I2C.RegAddr = 26;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x9F) | (BiasHf<<5); 
    
    //SDM_RES R28[2]
    R858_I2C.RegAddr = 28;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFB) | (SDM_RES<<2); 
    
    //NS_RES  R21[7]
    R858_I2C.RegAddr = 17;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x7F) | (NS_RES<<7); 
    
    //Divider num R26[2:0]
    R858_I2C.RegAddr = 26;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF8) | (DivNum);  
    
    VCO_Freq = LO_Freq * MixDiv;
    Nint = (UINT16) (VCO_Freq / 2 / PLL_Ref);
    VCO_Fra = (UINT16) (VCO_Freq - 2 * PLL_Ref * Nint);
    
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
    Ni = (UINT8) ((Nint - 13) / 4);
    Si = (UINT8) (Nint - 4 *Ni - 13);
    
    R858_I2C.RegAddr = 21;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x80) | Ni;     //R21[6:0]
    
    R858_I2C.RegAddr = 25;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFC) | Si;  //R25[1:0]
           
    //pw_sdm & pw_dither
    R858_I2C.RegAddr = 28;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] &= 0xFC;        //R28[1:0]
    if(VCO_Fra == 0)
    {
        R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] |= 0x03;
    }
    
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
    
    R858_I2C.RegAddr = 23;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (UINT8) SDM16to9;  //R23
    
    R858_I2C.RegAddr = 22;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (UINT8) SDM8to1;    //R22
    
    R858_I2C_Len.RegAddr = 8;
    R858_I2C_Len.Len = 40;
    
    for(PllArrayCunt = 0 ; PllArrayCunt < 40 ; PllArrayCunt ++)
    {
        R858_I2C_Len.Data[PllArrayCunt] = R858_Array[R858_Num][IntTunerNum][PllArrayCunt + 8];
    }
    if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
        return RT_Fail;
    
    
    if(R858_XtalDiv == XTAL_DIV1)
        R858_Delay_MS(R858_Num, PLL_LOCK_DELAY);
    else if((R858_XtalDiv == XTAL_DIV1_2) || (R858_XtalDiv == XTAL_DIV2_1))
        R858_Delay_MS(R858_Num, PLL_LOCK_DELAY*2);
    else
        R858_Delay_MS(R858_Num, PLL_LOCK_DELAY*4);  
    
    //set pll autotune = 1khz R38[3:2]
    R858_I2C.RegAddr = 38;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF3) | 0x08;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    return RT_Success;

}


R858_ErrCode R858_MUX(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 LO_KHz, UINT32 RF_KHz, R858_Standard_Type R858_Standard)
{
    UINT8 Reg_IMR_Gain   = 0;
    UINT8 Reg_IMR_Phase  = 0;
    UINT8 Reg_IMR_Iqcap  = 0;
    
    //Assign device address
    R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
    R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
    
    //Freq_Info_Type Freq_Info1;
    Freq_Info1 = R858_Freq_Sel(LO_KHz, RF_KHz, R858_Standard);
    
    
    // TF_DIPLEXER
    R858_I2C.RegAddr = 11;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFC) | (Freq_Info1.TF_DIPLEXER);  // R11[1:0]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //TF_HPF_BPF =>  R11[7:6], R12[0]
    R858_I2C.RegAddr = 11;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | ((Freq_Info1.TF_HPF_BPF & 0x06)<<5) ;  // R11[7:6]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    R858_I2C.RegAddr = 12;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFE) | (Freq_Info1.TF_HPF_BPF & 0x01) ;  // R12[0]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    
    //TF_HPF_CNR => R12[2:1]
    R858_I2C.RegAddr = 12;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF9) | (Freq_Info1.TF_HPF_CNR << 1);  // R12[2:1]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    // RF Polyfilter
    R858_I2C.RegAddr = 14;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF3) | (Freq_Info1.RF_POLY << 2);  //R14[3:2]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    // LNA Cap
    R858_I2C.RegAddr = 11;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xC3) | (Freq_Info1.LPF_CAP << 2);	  //R11[5:2]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    // LNA Notch
    R858_I2C.RegAddr = 12;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x07) | (Freq_Info1.LPF_NOTCH << 3);  // R12[7:3]
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //Set_IMR
    if(R858_IMR_done_flag[R858_Num][IntTunerNum] == TRUE)
    {
        Reg_IMR_Gain = R858_IMR_Data[R858_Num][IntTunerNum][Freq_Info1.IMR_MEM].Gain_X;       //b4 + b3:0
        Reg_IMR_Phase = R858_IMR_Data[R858_Num][IntTunerNum][Freq_Info1.IMR_MEM].Phase_Y;     //b4 + b3:0
        Reg_IMR_Iqcap = R858_IMR_Data[R858_Num][IntTunerNum][Freq_Info1.IMR_MEM].Iqcap;          //0,1,2
    }
    else
    {
        Reg_IMR_Gain = 0;
        Reg_IMR_Phase = 0;
        Reg_IMR_Iqcap = 0;
    }
    
    //Gain/Phase, R16[3:0][7:4]
    R858_I2C.RegAddr = 16;                  
    R858_Array[R858_Num][IntTunerNum][16] = (R858_Array[R858_Num][IntTunerNum][16] & 0x00) | (Reg_IMR_Gain & 0x0F) | (Reg_IMR_Phase & 0xF0);
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][16];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
      //Gain/Phase dir, R17[1:0]
    R858_I2C.RegAddr = 17;                  
    R858_Array[R858_Num][IntTunerNum][17] = (R858_Array[R858_Num][IntTunerNum][17] & 0xFC) | ((Reg_IMR_Gain & 0x10)>>4) | ((Reg_IMR_Phase & 0x10)>>3);
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][17];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    //Iqcap, R15[2:1]
    R858_I2C.RegAddr = 15;
    R858_Array[R858_Num][IntTunerNum][15] = (R858_Array[R858_Num][IntTunerNum][15] & 0xF9) | (Reg_IMR_Iqcap<<1);
    R858_I2C.Data =R858_Array[R858_Num][IntTunerNum][15];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    
    return RT_Success;
}



R858_ErrCode R858_IQ(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont)
{
    R858_SectType Compare_IQ[3];
    UINT8   X_Direction;  // 1:X, 0:Y
    
    UINT8   ADC_Read_Value;
    UINT8   LPF_Count;
    
    R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
    R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  
    
    //manual LPF gain
    R858_I2C.RegAddr = 19;
    R858_Array[R858_Num][IntTunerNum][19] = (R858_Array[R858_Num][IntTunerNum][19] & 0xFD);
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][19];  
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
     // increase LPF power to let image significant
     for(LPF_Count=0; LPF_Count<16; LPF_Count++)  //start from 5 (??)
     {
        R858_I2C.RegAddr = 37;
        R858_Array[R858_Num][IntTunerNum][37] = (R858_Array[R858_Num][IntTunerNum][37] & 0x0F) | (LPF_Count<<4);
        R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][37];  
        if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
            return RT_Fail;
        
        R858_Delay_MS(R858_Num, FILTER_GAIN_DELAY); //
        
        if(R858_Muti_Read(R858_Num, IntTunerNum, &ADC_Read_Value) != RT_Success)
            return RT_Fail;
        
        if(ADC_Read_Value > 40*ADC_READ_COUNT)
            break;
     }
    
    Compare_IQ[0].Gain_X  = (R858_Array[R858_Num][IntTunerNum][16] & 0x0F) | ((R858_Array[R858_Num][IntTunerNum][17] & 0x01)<<4); 
    Compare_IQ[0].Phase_Y = ((R858_Array[R858_Num][IntTunerNum][16] & 0xF0)>>4) | ((R858_Array[R858_Num][IntTunerNum][17] & 0x02)<<3);
    
           // Determine X or Y
    if(R858_IMR_Cross(R858_Num, IntTunerNum, &Compare_IQ[0], &X_Direction) != RT_Success)
        return RT_Fail;
        
    if(X_Direction==1)
    {
    //compare and find min of 3 points. determine I/Q direction
        if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
        return RT_Fail;
        
        //increase step to find min value of this direction
        if(R858_CompreStep(R858_Num, IntTunerNum, &Compare_IQ[0], 0x10) != RT_Success)  //X
        return RT_Fail;	
    }
    else
    {
       //compare and find min of 3 points. determine I/Q direction
       if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
           return RT_Fail;
    
       //increase step to find min value of this direction
       if(R858_CompreStep(R858_Num, IntTunerNum, &Compare_IQ[0], 0x11) != RT_Success)  //Y
           return RT_Fail;	
    }
    
    //Another direction
    if(X_Direction==1)
    {  
        if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success) //Y	
            return RT_Fail;	
    
       //compare and find min of 3 points. determine I/Q direction
       if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
           return RT_Fail;
    
       //increase step to find min value of this direction
       if(R858_CompreStep(R858_Num, IntTunerNum, &Compare_IQ[0], 0x11) != RT_Success)  //Y
           return RT_Fail;	
    }
    else
    {
       if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x11, &Compare_IQ[0]) != RT_Success) //X
         return RT_Fail;	
         
       //compare and find min of 3 points. determine I/Q direction
       if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
         return RT_Fail;
    
          //increase step to find min value of this direction
       if(R858_CompreStep(R858_Num, IntTunerNum, &Compare_IQ[0], 0x10) != RT_Success) //X
         return RT_Fail;	
    }
    
    
    //--- Check 3 points again---//
    if(X_Direction==1)
    {
        if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x11, &Compare_IQ[0]) != RT_Success) //X
            return RT_Fail;	
    }
    else
    {
       if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success) //Y
           return RT_Fail;		
    }
    
        if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
            return RT_Fail;
        
         //Section-9 check
         //if(R858_F_IMR(&Compare_IQ[0]) != RT_Success)
    if(R858_Section(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
        return RT_Fail;
    
    //clear IQ_Cap = 0   
    Compare_IQ[0].Iqcap = 0;
    
    if(R858_IMR_Iqcap(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
        return RT_Fail;
    
    *IQ_Pont = Compare_IQ[0];
    
    //reset gain/phase/iqcap control setting
    R858_I2C.RegAddr = 16;	// R16[7:0]  
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x00;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = 17;	// R17[1:0]  
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFC;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = 15;	//  R15[2:1] 
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF9;
    R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    return RT_Success;
}



//--------------------------------------------------------------------------------------------
// Purpose: record IMC results by input gain/phase location
//          then adjust gain or phase positive 1 step and negtive 1 step, both record results
// input: FixPot: phase or gain
//        FlucPot phase or gain
//        PotReg: 0x10 or 0x11 for R858
//        CompareTree: 3 IMR trace and results
// output: TREU or FALSE
//--------------------------------------------------------------------------------------------

//20131217-Ryan


R858_ErrCode R858_IQ_Tree(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R858_SectType* CompareTree)
{
    UINT8 TreeCunt  = 0;
    UINT8 TreeTimes = 3;
    //UINT8 PntReg = 0;
    
    R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
    R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  
    
    //PntReg is reg to change; FlucPot is change value
    /*
    if(PotReg == 0x10)
        PntReg = 0x11; //phase control
    else
        PntReg = 0x10; //gain control
    */
    for(TreeCunt = 0; TreeCunt<TreeTimes; TreeCunt++)
    {
        if(PotReg==0x10)  //change phase (FlucPot is phase)
        {
            R858_I2C.RegAddr = 16;
            R858_Array[R858_Num][IntTunerNum][16] = (R858_Array[R858_Num][IntTunerNum][16] & 0x00) | ((FlucPot & 0x0F)<<4) | (FixPot & 0x0F);
            R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][16];
            if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
                return RT_Fail;
            
            R858_I2C.RegAddr = 17;
            R858_Array[R858_Num][IntTunerNum][17] = (R858_Array[R858_Num][IntTunerNum][17] & 0xFD) | ((FlucPot & 0x10)>>3) | ((FixPot & 0x10)>>4);
            R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][17];
            if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
                return RT_Fail;		
        }
        else //change gain (FlucPot is gain)
        {
            R858_I2C.RegAddr = 16;
            R858_Array[R858_Num][IntTunerNum][16] = (R858_Array[R858_Num][IntTunerNum][16] & 0x00) | ((FlucPot & 0x0F)<<0) | ((FixPot & 0x0F)<<4);
            R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][16];
            if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
                return RT_Fail;
            
            R858_I2C.RegAddr = 17;
            R858_Array[R858_Num][IntTunerNum][17] = (R858_Array[R858_Num][IntTunerNum][17] & 0xFE) | ((FlucPot & 0x10)>>4) | ((FixPot & 0x10)>>3);
            R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][17];
            if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
                return RT_Fail;				
        }
/*
    R858_I2C.RegAddr = PotReg;
    R858_I2C.Data    = FixPot;
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
    
    R858_I2C.RegAddr = PntReg;
    R858_I2C.Data    = FlucPot;
    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
        return RT_Fail;
*/
    if(R858_Muti_Read(R858_Num, IntTunerNum, &CompareTree[TreeCunt].Value) != RT_Success)
        return RT_Fail;
    
    
    if(PotReg == 0x10) //change phase
    {
        CompareTree[TreeCunt].Gain_X  = FixPot;
        CompareTree[TreeCunt].Phase_Y = FlucPot;
    }
    else  //change gain
    {
        CompareTree[TreeCunt].Phase_Y  = FixPot;
        CompareTree[TreeCunt].Gain_X = FlucPot;
    }
    
    if(TreeCunt == 0)   //try right-side point
        FlucPot ++; 
    else if(TreeCunt == 1) //try left-side point
    {
        if((FlucPot & 0x0F) == 1) //if absolute location is 1, change I/Q direction
        {
            if(FlucPot & 0x10) //b[4]:I/Q selection. 0:Q-path, 1:I-path
            {
                FlucPot = (FlucPot & 0xE0) | 0x01;			
            }
            else
            {
                FlucPot = (FlucPot & 0xE0) | 0x11;
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
R858_ErrCode R858_CompreCor(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* CorArry)
{
    UINT8 CompCunt = 0;
    R858_SectType CorTemp;
    
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
R858_ErrCode R858_CompreStep(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* StepArry, UINT8 Pace)
{
	R858_SectType StepTemp;
	
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  

	//min value already saved in StepArry[0]
	StepTemp.Phase_Y = StepArry[0].Phase_Y;
	StepTemp.Gain_X  = StepArry[0].Gain_X;
	//StepTemp.Iqcap  = StepArry[0].Iqcap;

	while(((StepTemp.Gain_X & 0x0F) < R858_IMR_TRIAL) && ((StepTemp.Phase_Y & 0x0F) < R858_IMR_TRIAL))  
	{
		if(Pace == 0x10)	
		{
			StepTemp.Gain_X++;    //gain direction
		}
		else
			StepTemp.Phase_Y++;   //phase direction
	
		R858_I2C.RegAddr = 16;	
		R858_I2C.Data = (StepTemp.Gain_X & 0x0F) + ((StepTemp.Phase_Y & 0x0F)<<4);
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;
/*
		R858_I2C.RegAddr = 16;	
		R858_I2C.Data = StepTemp.Phase_Y;
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;
*/
		if(R858_Muti_Read(R858_Num, IntTunerNum, &StepTemp.Value) != RT_Success)
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
// Purpose: read multiple IMR results for stability
// input: IMR_Reg: IMR result address
//        IMR_Result_Data: result 
// output: TRUE or FALSE
//-----------------------------------------------------------------------------------/
R858_ErrCode R858_Muti_Read(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8* IMR_Result_Data)
{
#if (ADC_MULTI_READ==3)
	UINT8 ReadCunt     = 0;
	UINT16 ReadAmount  = 0;
	UINT8 ReadMax = 0;
	UINT8 ReadMin = 255;
	UINT8 ReadData = 0;
	
    R858_Delay_MS(R858_Num, ADC_READ_DELAY);//2

	for(ReadCunt = 0; ReadCunt < (ADC_READ_COUNT+2); ReadCunt ++)
	{
		R858_I2C_Len.RegAddr = 0x00;
		R858_I2C_Len.Len = 5;              // read R4
		R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

		if(I2C_Read_Len(R858_Num , &R858_I2C_Len) != RT_Success)
		{
			I2C_Read_Len(R858_Num , &R858_I2C_Len);
		}

		ReadData = (R858_I2C_Len.Data[4] & 0x3F);
		
		ReadAmount = ReadAmount + (UINT16)ReadData;
		
		if(ReadData < ReadMin)
			ReadMin = ReadData;
		
        if(ReadData > ReadMax)
			ReadMax = ReadData;
	}
	*IMR_Result_Data = (UINT8) (ReadAmount - (UINT16)ReadMax - (UINT16)ReadMin);
#else
	R858_Delay_MS(R858_Num, ADC_READ_DELAY);//2

	R858_I2C_Len.RegAddr = 0;
	R858_I2C_Len.Len = 5;              // read R4
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	if(I2C_Read_Len(R858_Num, &R858_I2C_Len) != RT_Success)
	{
		I2C_Read_Len(R858_Num, &R858_I2C_Len);
	}
	*IMR_Result_Data = (R858_I2C_Len.Data[4] & 0x3F);
#endif

	return RT_Success;
}


R858_ErrCode R858_Section(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont)
{
	R858_SectType Compare_IQ[3];
	R858_SectType Compare_Bet[3];

	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  

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

	if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;		

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;	

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
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

	if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;		

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];
	
	return RT_Success;
}


R858_ErrCode R858_IMR_Cross(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont, UINT8* X_Direct)
{

	R858_SectType Compare_Cross[9]; //(0,0)(0,Q-1)(0,I-1)(Q-1,0)(I-1,0)+(0,Q-2)(0,I-2)(Q-2,0)(I-2,0)
	R858_SectType Compare_Temp;
	UINT8 CrossCount = 0;
//	UINT8 Reg16 = R858_Array[R858_Num][IntTunerNum][16] & 0x00;	
	UINT8 Reg17 = R858_Array[R858_Num][IntTunerNum][17] & 0xFC;	

	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  

	Compare_Temp.Gain_X = 0;
	Compare_Temp.Phase_Y = 0;
	Compare_Temp.Iqcap = 0;
	Compare_Temp.Value = 255;

	for(CrossCount=0; CrossCount<9; CrossCount++)
	{
		if(CrossCount==0)
		{
		  Compare_Cross[CrossCount].Gain_X = 0;   //(0, 0)
		  Compare_Cross[CrossCount].Phase_Y = 0;
		}
		else if(CrossCount==1)
		{
		  Compare_Cross[CrossCount].Gain_X = 0;   //(0, Q-1)
		  Compare_Cross[CrossCount].Phase_Y = 1;      
		}
		else if(CrossCount==2)
		{
		  Compare_Cross[CrossCount].Gain_X = 0;   // (0, I-1)
		  Compare_Cross[CrossCount].Phase_Y = 1+(1<<4); 
		}
		else if(CrossCount==3)
		{
		  Compare_Cross[CrossCount].Gain_X = 1;          //(Q-1, 0)
		  Compare_Cross[CrossCount].Phase_Y = 0;
		}
		else if(CrossCount==4)
		{
		  Compare_Cross[CrossCount].Gain_X = 1+(1<<4);          //(I-1,0)
		  Compare_Cross[CrossCount].Phase_Y = 0;
		}
		else if(CrossCount==5)
		{
		  Compare_Cross[CrossCount].Gain_X = 0;  //(0, Q-2)
		  Compare_Cross[CrossCount].Phase_Y = 2;  
		}
		else if(CrossCount==6)
		{
		  Compare_Cross[CrossCount].Gain_X = 0;   //(0, I-2)
		  Compare_Cross[CrossCount].Phase_Y = 2+(1<<4); 
		}
		else if(CrossCount==7)
		{
		  Compare_Cross[CrossCount].Gain_X = 2;        //(Q-2,0)
		  Compare_Cross[CrossCount].Phase_Y = 0;
		}
		else if(CrossCount==8)
		{
		  Compare_Cross[CrossCount].Gain_X = 2+(1<<4);        //(I-2,0)
		  Compare_Cross[CrossCount].Phase_Y = 0;
		}

    	R858_I2C.RegAddr = 16;	
	    R858_I2C.Data = (Compare_Cross[CrossCount].Gain_X & 0x0F) | ((Compare_Cross[CrossCount].Phase_Y & 0x0F)<<4);
	    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		   return RT_Fail;

	    R858_I2C.RegAddr = 17;	
	    R858_I2C.Data = Reg17 | ((Compare_Cross[CrossCount].Gain_X & 0x10)>>4) | ((Compare_Cross[CrossCount].Phase_Y & 0x10)>>3);
	    if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		  return RT_Fail;
	
        if(R858_Muti_Read(R858_Num, IntTunerNum, &Compare_Cross[CrossCount].Value) != RT_Success)
		  return RT_Fail;

		if( Compare_Cross[CrossCount].Value < Compare_Temp.Value)
		{
		  Compare_Temp.Value = Compare_Cross[CrossCount].Value;
		  Compare_Temp.Gain_X = Compare_Cross[CrossCount].Gain_X;
		  Compare_Temp.Phase_Y = Compare_Cross[CrossCount].Phase_Y;		
		}
	} //end for loop


    if(((Compare_Temp.Phase_Y & 0x1F)==0x01) || (Compare_Temp.Phase_Y & 0x1F)==0x02)  //phase Q1 or Q2
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
	else if(((Compare_Temp.Phase_Y & 0x1F)==0x11) || (Compare_Temp.Phase_Y & 0x1F)==0x12)  //phase I1 or I2
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
	else if(((Compare_Temp.Gain_X & 0x1F)==0x01) || (Compare_Temp.Gain_X & 0x1F)==0x02)  //gain Q1 or Q2
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
	else if(((Compare_Temp.Gain_X & 0x1F)==0x11) || (Compare_Temp.Gain_X & 0x1F)==0x12)  //gain I1 or I2
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
R858_ErrCode R858_F_IMR(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Pont)
{
	R858_SectType Compare_IQ[3];
	R858_SectType Compare_Bet[3];

	UINT8   ADC_Read_Value;
	UINT8   LPF_Count;

	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  

	 //manual LPF gain
	 R858_I2C.RegAddr = 19;
	 R858_Array[R858_Num][IntTunerNum][19] = (R858_Array[R858_Num][IntTunerNum][19] & 0xFD);
	 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][19];  
	 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		 return RT_Fail;

	 // increase LPF power to let image significant
	 for(LPF_Count=0; LPF_Count<16; LPF_Count++)  //start from 5 (??)
	 {
		R858_I2C.RegAddr = 37;
		R858_Array[R858_Num][IntTunerNum][37] = (R858_Array[R858_Num][IntTunerNum][37] & 0x0F) | (LPF_Count<<4);
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][37];  
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		R858_Delay_MS(R858_Num, FILTER_GAIN_DELAY); //
		
		if(R858_Muti_Read(R858_Num, IntTunerNum, &ADC_Read_Value) != RT_Success)
			return RT_Fail;

		if(ADC_Read_Value > 40*ADC_READ_COUNT)
			break;
	 }

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x0F) == 0x00)
	{
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xEF) + 1;  //Q-path, Gain=1
	}
	else
	{
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	}
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;	

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;	

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[1].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[1].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[1].Value = Compare_IQ[0].Value;

	//Try X+1 column and save min result to Compare_Bet[2]
	if((IQ_Pont->Gain_X & 0x0F) == 0x00)		
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) | 0x10) + 1;  //I-path, Gain=1
	else
	    Compare_IQ[0].Gain_X = IQ_Pont->Gain_X + 1;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R858_IQ_Tree(R858_Num, IntTunerNum, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x10, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;		

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R858_CompreCor(R858_Num, IntTunerNum, &Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	//clear IQ_Cap = 0
	Compare_Bet[0].Iqcap = 0;	

	if(R858_IMR_Iqcap(R858_Num, IntTunerNum, &Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];
	
	return RT_Success;
}


R858_ErrCode R858_IMR_Iqcap(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_SectType* IQ_Point)   
{
    R858_SectType Compare_Temp;
	int i = 0;

	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];	  

	//Set Gain/Phase to right setting
	R858_I2C.RegAddr = 16;	
	R858_Array[R858_Num][IntTunerNum][16]=(IQ_Point->Gain_X & 0x0F) + ((IQ_Point->Phase_Y & 0x0F)<<4); 
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][16];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	R858_I2C.RegAddr = 17;	// R17[5:0]  
	R858_Array[R858_Num][IntTunerNum][17]=(R858_Array[R858_Num][IntTunerNum][17] & 0xFC) | ((IQ_Point->Gain_X & 0x10)>>4) | ((IQ_Point->Phase_Y & 0x10)>>3);
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][17];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	//try iqcap
	for(i=0; i<3; i++)	
	{
		Compare_Temp.Iqcap = (UINT8) i;  
		R858_I2C.RegAddr = 15;		// R15[2:1] 
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr ] & 0xF9) | (Compare_Temp.Iqcap<<1);  
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];  
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			   return RT_Fail;

		if(R858_Muti_Read(R858_Num, IntTunerNum, &(Compare_Temp.Value)) != RT_Success)
			   return RT_Fail;

		if(Compare_Temp.Value < IQ_Point->Value)
		{
			IQ_Point->Value = Compare_Temp.Value; 
			IQ_Point->Iqcap = Compare_Temp.Iqcap;
		}
	}

    return RT_Success;
}

R858_ErrCode R858_SetStandard(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_Standard_Type RT_Standard)
{	 
	
	UINT8 u1FilCalGap = 8;

	//Assign device address
	R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];


	if(RT_Standard != R858_pre_standard[R858_Num][IntTunerNum])
	{
		 if(R858_InitReg(R858_Num, IntTunerNum) != RT_Success)      
		     return RT_Fail;
	}
    else
    {
         printf("@@@ have set standard\n");
    }
    R858_pre_standard[R858_Num][IntTunerNum] = RT_Standard;


    Sys_Info1 = R858_Sys_Sel(RT_Standard);

	// Filter Calibration	
	 u1FilCalGap = 8;

	//HPF ext protection

    if(R858_Fil_Cal_flag[R858_Num][IntTunerNum][RT_Standard] == FALSE)
	{
	    printf(" @@@ First in R858_Fil_Cal_flag %x %x %x\n", R858_Num, IntTunerNum, RT_Standard);
		R858_Fil_Cal_code[R858_Num][IntTunerNum][RT_Standard] = R858_Filt_Cal_ADC(R858_Num, IntTunerNum, Sys_Info1.FILT_CAL_IF, Sys_Info1.BW, u1FilCalGap);
		R858_Fil_Cal_BW[R858_Num][IntTunerNum][RT_Standard] = R858_Bandwidth[R858_Num][IntTunerNum];        
        R858_Fil_Cal_LpfLsb[R858_Num][IntTunerNum][RT_Standard] = R858_Lpf_Lsb[R858_Num][IntTunerNum];

		R858_Fil_Cal_flag[R858_Num][IntTunerNum][RT_Standard] = TRUE;

	    //Reset register and Array
	    if(R858_InitReg(R858_Num, IntTunerNum) != RT_Success)
		   return RT_Fail;
	}
    else
    {
        printf("@@@ R858_Fil_Cal_flag = TRUE\n");
    }
	// Set LPF Lsb bit
	R858_I2C.RegAddr = 20;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFB) | (R858_Fil_Cal_LpfLsb[R858_Num][IntTunerNum][RT_Standard]<<2);  //R20[2]
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
	    return RT_Fail;

	// Set LPF fine code
	R858_I2C.RegAddr = 20;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x83) | (R858_Fil_Cal_code[R858_Num][IntTunerNum][RT_Standard]<<3);  //R20[6:3]
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	// Set LPF coarse BW
	R858_I2C.RegAddr = 17;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xCF) | (R858_Fil_Cal_BW[R858_Num][IntTunerNum][RT_Standard]<<4);  //R17[5:4]
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	//Set HPF notch  R18[7]
	R858_I2C.RegAddr = 18;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x7F) | (Sys_Info1.HPF_NOTCH<<7);  
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	// Set HPF corner 
	R858_I2C.RegAddr = 18;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x87) | (Sys_Info1.HPF_COR<<3);	//R18[6:3]
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	// Set Filter Auto Ext  
	R858_I2C.RegAddr = 19;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xBF) | (Sys_Info1.FILT_EXT_ENA<<6);  //R19[6]
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	// Set AGC clk R38[5:4]
	R858_I2C.RegAddr = 38;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xCF) | (Sys_Info1.AGC_CLK<<4);  
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


UINT8  R858_Filt_Cal_ADC(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 IF_Freq, UINT8 R858_BW, UINT8 FilCal_Gap)
{
	 UINT8     u1FilterCodeResult = 0;
	 UINT8     u1FilterCode = 0;
	 UINT8     u1FilterCalValue = 0;
	 UINT8     u1FilterCalValuePre = 0;
	 UINT32   u4RingFreq = 72000;
	 UINT8     initial_cnt = 0;
	 UINT8     i = 0;
	 UINT8     ADC_Read_Value = 0;
	 UINT8     LPF_Count = 0;

	 if(R858_Cal_Prepare(R858_Num, IntTunerNum, R858_LPF_CAL) != RT_Success)      
		 return RT_Fail;

	if(IntTunerNum==R858_TUNER1)
	{
		//Select det2=tuner1 in Tuner1 R46[1]
		R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
		R858_I2C.RegAddr = 46;
		R858_Array[R858_Num][R858_TUNER1][46] = (R858_Array[R858_Num][R858_TUNER1][46] & 0xFD) | 0x00; //tuner1
		R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][46];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//Set Ring to RFBuf in R14[1], Tuner1 ON
		R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
		R858_I2C.RegAddr = 14;
		R858_Array[R858_Num][R858_TUNER1][14] = (R858_Array[R858_Num][R858_TUNER1][14] & 0xFD) | 0x02; //on
		R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][14];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//Set Ring to RFBuf in R14[1], Tuner2 OFF
		R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER2];
		R858_I2C.RegAddr = 14;
		R858_Array[R858_Num][R858_TUNER2][14] = (R858_Array[R858_Num][R858_TUNER2][14] & 0xFD) | 0x00; //off
		R858_I2C.Data = R858_Array[R858_Num][R858_TUNER2][14];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;
	}
	else   //Tuner2
	{
		//Select det2=tuner2 in Tuner1 R46
		R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
		R858_I2C.RegAddr = 46;
		R858_Array[R858_Num][R858_TUNER1][46] = (R858_Array[R858_Num][R858_TUNER1][46] & 0xFD) | 0x02;  //tuner2
		R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][46];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//Set Ring to RFBuf in R14[1], Tuner1 Off
		R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER1];
		R858_I2C.RegAddr = 14;
		R858_Array[R858_Num][R858_TUNER1][14] = (R858_Array[R858_Num][R858_TUNER1][14] & 0xFD) | 0x00; //off
		R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][14];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//Set Ring to RFBuf in R14[1], Tuner2 On
		R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][R858_TUNER2];
		R858_I2C.RegAddr = 14;
		R858_Array[R858_Num][R858_TUNER2][14] = (R858_Array[R858_Num][R858_TUNER2][14] & 0xFD) | 0x02; //on
		R858_I2C.Data = R858_Array[R858_Num][R858_TUNER2][14];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;
	}

	R858_I2C.I2cAddr = R858_ADDRESS[R858_Num][IntTunerNum];

	 //manual LPF gain
	 R858_I2C.RegAddr = 19;
	 R858_Array[R858_Num][IntTunerNum][19] = (R858_Array[R858_Num][IntTunerNum][19] & 0xFD);
	 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][19];  
	 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		 return RT_Fail;

	 for(LPF_Count=0; LPF_Count<16; LPF_Count++)  //start from 5 (??)
	 {
		R858_I2C.RegAddr = 37;
		R858_Array[R858_Num][IntTunerNum][37] = (R858_Array[R858_Num][IntTunerNum][37] & 0x0F) | (LPF_Count<<4);
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][37];  
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		R858_Delay_MS(R858_Num, FILTER_GAIN_DELAY); //
		
		if(R858_Muti_Read(R858_Num, IntTunerNum, &ADC_Read_Value) != RT_Success)
			return RT_Fail;

		if(ADC_Read_Value > 40*ADC_READ_COUNT)
			break;
	 }

     //Must do before PLL() 
	 //if(R858_MUX(IntTunerNum, u4RingFreq + IF_Freq, u4RingFreq, R858_STD_SIZE) != RT_Success)     //FilCal MUX (LO_Freq, RF_Freq)
	   //  return RT_Fail;

	 //Set PLL
	 if(R858_PLL(R858_Num, IntTunerNum, (u4RingFreq + IF_Freq), R858_STD_SIZE) != RT_Success)   //FilCal PLL
	       return RT_Fail;

	 //------- Try suitable BW --------//
	 if(R858_BW==2) //6M
         initial_cnt = 1;  //try 7M first
	 else
		 initial_cnt = 0;  //try 8M first

	 for(i=initial_cnt; i<3; i++)
	 {
         if(i==0)
             R858_Bandwidth[R858_Num][IntTunerNum] = 0; //8M
		 else if(i==1)
			 R858_Bandwidth[R858_Num][IntTunerNum] = 2; //7M
		 else
			 R858_Bandwidth[R858_Num][IntTunerNum] = 3; //6M

		 //Set BW
		 R858_I2C.RegAddr = 17;	
	     R858_Array[R858_Num][IntTunerNum][17] = (R858_Array[R858_Num][IntTunerNum][17] & 0xCF) | (R858_Bandwidth[R858_Num][IntTunerNum]<<4);
         R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][17];		
         if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		      return RT_Fail;	

		 // read code 0 (4 MSB bits)
		 R858_I2C.RegAddr = 20;
		 R858_Array[R858_Num][IntTunerNum][20] = (R858_Array[R858_Num][IntTunerNum][20] & 0x87) | (0<<3);  //code 0<<1
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][20];
		 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			  return RT_Fail;

		 R858_Delay_MS(R858_Num, FILTER_CODE_DELAY); //delay ms
	     
		 if(R858_Muti_Read(R858_Num, IntTunerNum, &u1FilterCalValuePre) != RT_Success)
			  return RT_Fail;

		 //read code 13*2 (4 MSB bits)
		 R858_I2C.RegAddr = 20;
		 R858_Array[R858_Num][IntTunerNum][20] = (R858_Array[R858_Num][IntTunerNum][20] & 0x87) | (13<<3);  //code 13<<1
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][20];
		 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			  return RT_Fail;

		 R858_Delay_MS(R858_Num, FILTER_CODE_DELAY); //delay ms
	     
		 if(R858_Muti_Read(R858_Num, IntTunerNum, &u1FilterCalValue) != RT_Success)
			  return RT_Fail;

		 if(u1FilterCalValuePre > (u1FilterCalValue+8))  //suitable BW found
			 break;
	 }

     //-------- Try LPF filter code ---------//
	 u1FilterCalValuePre = 0;
	 for(u1FilterCode=0; u1FilterCode<16; u1FilterCode++)
	 {
		 R858_I2C.RegAddr = 20;
		 R858_Array[R858_Num][IntTunerNum][20] = (R858_Array[R858_Num][IntTunerNum][20] & 0x87) | (u1FilterCode<<3);  //code<<1
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][20];
		 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			  return RT_Fail;

		 R858_Delay_MS(R858_Num, FILTER_CODE_DELAY); //delay ms

		 if(R858_Muti_Read(R858_Num, IntTunerNum, &u1FilterCalValue) != RT_Success)
		      return RT_Fail;

		 if(u1FilterCode==0)
              u1FilterCalValuePre = u1FilterCalValue;

		 if((u1FilterCalValue+FilCal_Gap*ADC_READ_COUNT) < u1FilterCalValuePre)
		 {
			 u1FilterCodeResult = u1FilterCode;
			  break;
		 }

	 }

     //Try LSB bit
	 if(u1FilterCodeResult>0)   //try code-1 & lsb=1
	 {		
		 R858_I2C.RegAddr = 20;
		 R858_Array[R858_Num][IntTunerNum][20] = (R858_Array[R858_Num][IntTunerNum][20] & 0x83) | ((u1FilterCodeResult-1)<<3) | (1<<2);  
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][20];
		 if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			  return RT_Fail;

		 R858_Delay_MS(R858_Num, FILTER_CODE_DELAY); //delay ms

		 if(R858_Muti_Read(R858_Num, IntTunerNum, &u1FilterCalValue) != RT_Success)
			  return RT_Fail;

		 if((u1FilterCalValue+FilCal_Gap*ADC_READ_COUNT) < u1FilterCalValuePre)
		 {
			 u1FilterCodeResult = u1FilterCodeResult - 1;
			 R858_Lpf_Lsb[R858_Num][IntTunerNum] = 1;
		 }
		 else
		 {		 	 
			 R858_Lpf_Lsb[R858_Num][IntTunerNum] = 0;
		 }
	 }
     
	 if(u1FilterCode==16)
          u1FilterCodeResult = 15;

	  return u1FilterCodeResult;

}
R858_ErrCode R858_SetFrequency(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_Set_Info R858_INFO)
{

	//Assign device address
	R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];


	 UINT32	LO_KHz;
	 UINT8    Img_R;
	 UINT8 SetFreqArrayCunt;

     // Check Input Frequency Range
	 if((R858_INFO.RF_KHz<40000) || (R858_INFO.RF_KHz>1002000))
	 {
	          return RT_Fail;
	 }

	  LO_KHz = R858_INFO.RF_KHz - Sys_Info1.IF_KHz;
	  Img_R = 1;

	  R858_I2C.RegAddr = 17;  //R17[2]
	  R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFB) | (Img_R<<2);  //R17[2]
	  R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	  if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		 return RT_Fail;

	 //Set MUX dependent var. Must do before PLL( ) 
     if(R858_MUX(R858_Num, IntTunerNum, LO_KHz, R858_INFO.RF_KHz, R858_INFO.R858_Standard) != RT_Success)   //normal MUX
        return RT_Fail;

	 R858_IF_GOLOBAL = Sys_Info1.IF_KHz;
     //Set PLL
     if(R858_PLL(R858_Num, IntTunerNum, LO_KHz, R858_INFO.R858_Standard) != RT_Success) //noraml PLL
        return RT_Fail;

     R858_IMR_point_num = Freq_Info1.IMR_MEM;

     //Get Sys-Freq parameter
     SysFreq_Info1 = R858_SysFreq_Sel(R858_INFO.R858_Standard, R858_INFO.RF_KHz);
	

	//LNA_NRB_DET R8[4]
	R858_I2C.RegAddr = 8;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xEF) | (SysFreq_Info1.LNA_NRB_DET<<4);  

	//LNA
	// LNA VTL/H R30[7:0]
	R858_I2C.RegAddr = 30;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x00) | SysFreq_Info1.LNA_VTL_H;

	// RF_LTE_PSG R32[3] 
	R858_I2C.RegAddr = 32;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF7) | (SysFreq_Info1.RF_LTE_PSG<<3);

	//RF
	// RF TOP R32[2:0]
	R858_I2C.RegAddr = 32;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF8) | ((7 - SysFreq_Info1.RF_TOP));

	// RF VTL/H R31[7:0]
	R858_I2C.RegAddr = 31;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x00) | SysFreq_Info1.RF_VTL_H;

	// RF Gain Limt  R13[6:5]
	R858_I2C.RegAddr = 13;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x9F) | (SysFreq_Info1.RF_GAIN_LIMIT<<5);

	//MIXER & FILTER
	//MIXER_AMP_CURRENT =>R10[1]
	R858_I2C.RegAddr = 10;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFD) | (SysFreq_Info1.MIXER_AMP_CURRENT<<1);  

	//MIXER_AMP_LPF  R47[2:0]
	R858_I2C.RegAddr = 47;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF8) | SysFreq_Info1.MIXER_AMP_LPF;  
	
    // MIXER TOP R34[7:4]
	R858_I2C.RegAddr = 34;
    R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x0F) | ((15 - SysFreq_Info1.MIXER_TOP)<<4); 

	// Filter TOP R36[7:4]
	R858_I2C.RegAddr = 36;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x0F) | ((15-SysFreq_Info1.FILTER_TOP)<<4);

	// 3th_LPF_Gain R17[7:6]
	R858_I2C.RegAddr = 17;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | (SysFreq_Info1.FILT_3TH_LPF_GAIN<<6);  

	// MIXER VTL / VTH R35[7:0]
	R858_I2C.RegAddr = 35;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = ((R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x00) | SysFreq_Info1.MIXER_VTL_H );


	// Filter VTL / VTH R37[7:0]
	R858_I2C.RegAddr = 37;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = ((R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x00) | SysFreq_Info1.FILTER_VTL_H);


	// Mixer Gain Limt R46[7:6]
	R858_I2C.RegAddr = 46;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | (SysFreq_Info1.MIXER_GAIN_LIMIT << 6);
 
	//BB disch current R40[0]
	R858_I2C.RegAddr = 40;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFE) | (SysFreq_Info1.BB_DIS_CURR);  
   
	//Mixer/Filter disch  R39[7:6]
	R858_I2C.RegAddr = 39;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x3F) | (SysFreq_Info1.MIXER_FILTER_DIS<<6);  
 
	//BB Det Mode  R39[4]
	R858_I2C.RegAddr = 39;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xEF) | (SysFreq_Info1.BB_DET_MODE<<4);  

	//Polyphase
	//ENB_POLY_GAIN  R17[3]
	R858_I2C.RegAddr = 17;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF7) | (SysFreq_Info1.ENB_POLY_GAIN<<3);

	//NRB  
	// NRB TOP R33[3:0]
	R858_I2C.RegAddr = 33;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF0) | ((15 - SysFreq_Info1.NRB_TOP));

	//NRB LPF & HPF BW  R33[7:6 & 5:4]
	R858_I2C.RegAddr = 33;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x0F) | (SysFreq_Info1.NRB_BW_HPF<<4) | (SysFreq_Info1.NRB_BW_LPF<<6);

	//NBR Image TOP adder R34[2:1]
	R858_I2C.RegAddr = 34;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF9) | (SysFreq_Info1.IMG_NRB_ADDER<<1);  

	//other setting

    //IF AGC1
	R858_I2C.RegAddr = 25;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xDF;

	//================================Tuner 1 only=============================

	R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];

	//PW
	// Set NA det power R44[1]  
	R858_I2C.RegAddr = 44;
	R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = (R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0xBF) | (SysFreq_Info1.NA_PWR_DET<<1);
	R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	//LNA 
	// LNA_TOP R45[2:0] ["7~0" (0~7) ; input: "7~0"] ; co-use ; tuner 1 control	
	R858_I2C.RegAddr = 45;
	R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = (R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0xF8) | (7 - SysFreq_Info1.LNA_TOP);
	R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;


//=============================Genernal Register====================================
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	R858_I2C_Len.RegAddr = 8;
	R858_I2C_Len.Len = 40;

	for(SetFreqArrayCunt = 0 ; SetFreqArrayCunt < 40 ; SetFreqArrayCunt ++)
	{
		R858_I2C_Len.Data[SetFreqArrayCunt] = R858_Array[R858_Num][IntTunerNum][SetFreqArrayCunt + 8];
	}
	if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
		return RT_Fail;



	 return RT_Success;
}




R858_ErrCode R858_SetPllData(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_Set_Info R858_INFO)
{
	if(R858_Initial_done_flag[R858_Num][IntTunerNum]==FALSE)
	{
		R858_Init(R858_Num, IntTunerNum);
	}


	if(R858_SetStandard(R858_Num, IntTunerNum, R858_INFO.R858_Standard) != RT_Success)
	{
	  return RT_Fail;
	}

	if(R858_SetFrequency(R858_Num, IntTunerNum, R858_INFO) != RT_Success)
	{
	  return RT_Fail;
	}


      return RT_Success;
}


R858_ErrCode R858_Standby(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_LT_Type R858_LT)
{
	UINT8 i;

	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
	R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	if(R858_SBY_flag[R858_Num][IntTunerNum]==0)
	{
		for(i=8; i<R858_REG_NUM; i++)  //store R8~R47
		{
			R858_SBY[R858_Num][IntTunerNum][i]=R858_Array[R858_Num][IntTunerNum][i];
		}

		 for(i=42; i<R858_REG_NUM; i++)  //store tuner1 R42~R47
		{
			R858_SBY2[R858_Num][i-42]=R858_Array[R858_Num][R858_TUNER1][i];
		 }

		//R8
		R858_I2C.RegAddr = 8;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x00) | 0xFF;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//IQ gen, AGC, ADC
		R858_I2C.RegAddr = 9;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xF8) | 0x07;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//PLL LDO div1
		R858_I2C.RegAddr = 26;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xE7) | 0x18;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//PLL LDO div2
		R858_I2C.RegAddr = 14;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xCF) | 0x30;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//PLL LDO A, PLL LDO D2
		R858_I2C.RegAddr = 25;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xC3) | 0x3C;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//PLL LDO D1
		R858_I2C.RegAddr = 47;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x9F) | 0x60;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//VGA
		R858_I2C.RegAddr = 46;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFE) | 0x01;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//ADC=VAGC
		R858_I2C.RegAddr = 41;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFC) | 0x02;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//AGC clk off
		R858_I2C.RegAddr = 20;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x7F) | 0x80;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		//clk out enable
		R858_I2C.RegAddr = 29;
		R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xBF) | 0x00;
		R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
		if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
			return RT_Fail;

		if(R858_Standby_Mode == R858_SBY_ALL)
		{
			//Set Internal Tuner 1
			R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];

			//LNA off, off LT, actLT off, LNA det off
			R858_I2C.RegAddr = 42;
			R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = (R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0xF0) | 0x08;
			R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr];
			if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
				return RT_Fail;

			//NAT off
			R858_I2C.RegAddr = 44;
			R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = (R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0xFD) | 0x00;
			R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr];
			if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
				return RT_Fail;
		
		}

		R858_SBY_flag[R858_Num][IntTunerNum]=1;
	}

	return RT_Success;
}



R858_ErrCode R858_Wakeup(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum)
{
	UINT8 i;

	if(R858_SBY_flag[R858_Num][IntTunerNum]==1)
	{
		R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
		R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];
		R858_I2C_Len.RegAddr = 8;
		R858_I2C_Len.Len = R858_REG_NUM-8;
		for(i = 8; i<R858_REG_NUM; i ++)  //write R8~R47
		{
			R858_I2C_Len.Data[i-8] = R858_SBY[R858_Num][IntTunerNum][i];
			R858_Array[R858_Num][IntTunerNum][i] = R858_SBY[R858_Num][IntTunerNum][i];
		}
		if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
			return RT_Fail;

		if(R858_Standby_Mode == R858_SBY_ALL)
		{
			//tuner1
			R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
			R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
			R858_I2C_Len.RegAddr = 42;
			R858_I2C_Len.Len = R858_REG_NUM-6;
			for(i = 42; i<R858_REG_NUM; i ++)  //write R42~R47
			{
				R858_I2C_Len.Data[i-42] = R858_SBY2[R858_Num][i-42];
				R858_Array[R858_Num][R858_TUNER1][i] = R858_SBY2[R858_Num][i-42];
			}
			if(I2C_Write_Len(R858_Num, &R858_I2C_Len) != RT_Success)
				return RT_Fail;
		}

		R858_SBY_flag[R858_Num][IntTunerNum]=0;
	}
	return RT_Success;
}

R858_ErrCode R858_GetRfGain(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_RF_Gain_Info *pR858_rf_gain)
{

	UINT16 acc_lna_gain[32] = {  0, 15, 26, 34, 50, 61, 75, 87, 101, 117,			//0~9
	                            130, 144, 154, 164, 176, 188, 199, 209, 220, 226,	//10~19
								233, 232, 232, 232, 232, 247, 262, 280, 296, 311,	//20~29
								296, 308											//30~31
                               }; 
	UINT16 acc_rf_gain[16] = {  0, 15, 26, 34, 50, 61, 75, 87, 101, 117,			//0~9
								130, 144, 154, 164, 176, 188 };						//10~15

	UINT16 acc_mixer_gain[16] = {  0, 0, 0, 0, 9, 22, 32, 44, 56, 68,				//0~9
								 80, 90, 100, 100, 100, 100 };						//10~15

	//Assign device address
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	R858_I2C_Len.RegAddr = 0x00;
	R858_I2C_Len.Len = 4;  //xx

	if(I2C_Read_Len(R858_Num , &R858_I2C_Len) != RT_Success)
	{
		I2C_Read_Len(R858_Num , &R858_I2C_Len);
	}
	pR858_rf_gain->RF_gain1 = ((R858_I2C_Len.Data[1] & 0xF8)>>3);       //lna
	pR858_rf_gain->RF_gain2 = (R858_I2C_Len.Data[2] & 0x0F);            //rf
	pR858_rf_gain->RF_gain3 = (R858_I2C_Len.Data[2] & 0xF0)>>4;       //mixer
	pR858_rf_gain->RF_gain4 = (R858_I2C_Len.Data[3] & 0x0F);             //filter
/*
	if(pR858_rf_gain->RF_gain1 > 24) 
        pR858_rf_gain->RF_gain1 = 24;  

	//Mixer Amp Gain
	if(pR858_rf_gain->RF_gain3 > 10)
        pR858_rf_gain->RF_gain3 = 10;  //MixerAmp gain max is 10	
	
	pR858_rf_gain->RF_gain_comb = (acc_lna_gain[pR858_rf_gain->RF_gain1] + acc_rfbuf_gain + pR858_rf_gain->RF_gain3*12);
*/
	pR858_rf_gain->RF_gain_comb = acc_lna_gain[pR858_rf_gain->RF_gain1]
		                        + acc_rf_gain[pR858_rf_gain->RF_gain2]
		                        + acc_mixer_gain[pR858_rf_gain->RF_gain3]
								+ pR858_rf_gain->RF_gain4*14;

    return RT_Success;
}


R858_ErrCode R858_RfGainMode(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, R858_RF_Gain_TYPE R858_RfGainType)
{
    UINT8 MixerGain = 0;
	UINT8 RfGain = 0;
	UINT8 LnaGain = 0;
	UINT8 FilterGain = 0;

	//Assign device address
	R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	if(R858_RfGainType==RF_MANUAL)
	{
		//LNA auto off
		 R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
	     R858_I2C.RegAddr = 46;
	     R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] | 0x40;   // R46[6] = 1   co-use control by tuner 1
		 R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][5];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;
		 R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

		 //Mixer buffer off
	     R858_I2C.RegAddr = 13;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] | 0x01;  // R13[0]  = 1 
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][26];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer gain auto off
	     R858_I2C.RegAddr = 15;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFE;  //R15[0] = 0
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		//Filter gain auto off
	     R858_I2C.RegAddr = 19;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFD;  //R19[1] = 0
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		R858_I2C_Len.RegAddr = 0x00;
		R858_I2C_Len.Len = 4; 
		R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

		if(I2C_Read_Len(R858_Num , &R858_I2C_Len) != RT_Success)
		{
			I2C_Read_Len(R858_Num , &R858_I2C_Len);
		}

		LnaGain = ((R858_I2C_Len.Data[1] & 0xF8) >>3);  //lna    // R1[7:3]  
		MixerGain = ((R858_I2C_Len.Data[2] & 0xF0) >> 4); //mixer // R2[7:4]
		RfGain = (R858_I2C_Len.Data[2] & 0x0F);   //rf		 // R2[3:0] 
		FilterGain = (R858_I2C_Len.Data[3] & 0x0F);   //filter		 // R3[3:0] 


		//set LNA gain
		 R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
	     R858_I2C.RegAddr = 43;
	     R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = (R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0xE0) | LnaGain;  // R43[4:0]  
		 R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;
		 R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

		 //set Mixer Buffer gain
	     R858_I2C.RegAddr = 31;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = ((R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x0F) | (RfGain << 4));  //R31[7:4] 
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		 //set Mixer gain
	     R858_I2C.RegAddr = 35;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = ((R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x0F) | (MixerGain << 4)); // R35[7:4]  
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		//set Filter gain
	     R858_I2C.RegAddr = 37;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = ((R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0x0F) | (FilterGain << 4)); // R37[7:4]  
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;
	}
	else
	{
	    //LNA auto on
		 R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][R858_TUNER1];
	     R858_I2C.RegAddr = 46;
	     R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] = R858_Array[R858_Num][R858_TUNER1][R858_I2C.RegAddr] & 0xBF;   // R46[6] = 0   co-use control by tuner 1
		 R858_I2C.Data = R858_Array[R858_Num][R858_TUNER1][5];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;
		 R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

		 //Mixer buffer on
	     R858_I2C.RegAddr = 13;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] & 0xFE;  // R13[0]  = 0
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][26];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer gain auto on
	     R858_I2C.RegAddr = 15;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] | 0x01;  //R15[0] = 1
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;

		 //Filter gain auto on
		 R858_I2C.RegAddr = 19;
	     R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] | 0x02;  //R19[1] = 1
		 R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	     if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		       return RT_Fail;
	}

    return RT_Success;
}


//----------------------------------------------------------------------//
//  R858_SetXtalCap( ): Set R858 Internal Xtal Cap                //
//  1st parameter: Xtal Cap value; range 0~41(pF)                    //
//-----------------------------------------------------------------------//
R858_ErrCode R858_SetXtalCap(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT8 u8XtalCap)
{
	UINT8 XtalCap;
	UINT8 Capx;
	UINT8 Capx_3_0, Capxx;

	//Assign device address
	R858_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	if(u8XtalCap>31)
	{
		XtalCap = 1;  //10
		Capx = u8XtalCap-10;
	}
	else
	{
		XtalCap = 0; //0
        Capx = u8XtalCap;
	}

	Capxx = Capx & 0x01;
    Capx_3_0 = Capx >> 1;
		
	// Set Xtal Cap  R28[6:3], R29[0]     XtalCap => R28[7]
	R858_I2C.RegAddr = 28;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr ] & 0x07) | (Capx_3_0 << 3) | ( XtalCap << 7);  
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;
	R858_I2C.RegAddr = 29;
	R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr ] & 0xFE) | Capxx;  
	R858_I2C.Data = R858_Array[R858_Num][IntTunerNum][R858_I2C.RegAddr];
	if(I2C_Write(R858_Num, &R858_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}
//----------------------------------------------------------------------//
//  R858_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                                //
//  2nd parameter: input Standard                                           //
//  3rd parameter: output signal level (dBm*1000)                    //
//  4th parameter: output RF max gain indicator (1:max gain)    //
//-----------------------------------------------------------------------//
R858_ErrCode R858_GetRfRssi(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 RF_Freq_Khz, R858_Standard_Type RT_Standard, INT32 *RfLevelDbm, UINT8 *fgRfMaxGain)
{ 	
	//UINT8 bPulseFlag;
	R858_RF_Gain_Info rf_gain_info;
	UINT16  acc_lna_gain;
	UINT16  acc_rfbuf_gain;
	UINT16  acc_mixer_gain;
	UINT16  rf_total_gain;
	UINT8   u1FreqIndex;
	INT16  u2FreqFactor=0;
	UINT8  u1LnaGainqFactorIdx;
	INT32     rf_rssi;
	INT32    fine_tune = 0;    //for find tune 
//	UINT8 	rf_limit;
	INT8 R858_Start_Gain_Cal_By_Freq[7] = {0, -20, -50, -20, 0, 20,40};
		//{50~135, 135~215, 215~265, 265~315, 315~325, 325~345, 345~950}

	//Assign device address
	R858_I2C_Len.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];

	R858_I2C_Len.RegAddr = 0x00;
	R858_I2C_Len.Len = 4;
	if(I2C_Read_Len(R858_Num , &R858_I2C_Len) != RT_Success)
	{
		I2C_Read_Len(R858_Num , &R858_I2C_Len);
	}
	
	//bPulseFlag = ((R858_I2C_Len.Data[1] & 0x40) >> 6);

	rf_gain_info.RF_gain1 = ((R858_I2C_Len.Data[1] & 0xF8)>>3);       //lna
	rf_gain_info.RF_gain2 = (R858_I2C_Len.Data[2] & 0x0F);            //rf
	rf_gain_info.RF_gain3 = (R858_I2C_Len.Data[2] & 0xF0)>>4;       //mixer
	rf_gain_info.RF_gain4 = (R858_I2C_Len.Data[3] & 0x0F);             //filter

/*
	//max RF buffer
	rf_limit = (((R858_Array[R858_Num][18]&0x04)>>1) + ((R858_Array[R858_Num][16]&0x40)>>6));
	if(rf_limit==0)
		rf_gain_info.RF_gain2 = 15;
	else if(rf_limit==1)
		rf_gain_info.RF_gain2 = 11;
	else if(rf_limit==2)
		rf_gain_info.RF_gain2 = 13;
	else
		rf_gain_info.RF_gain2 = 9;


	//max Mixer		
	if((R858_Array[R858_Num][22]&0xC0)==0x00)
		rf_gain_info.RF_gain3 = 6;
	else if((R858_Array[R858_Num][22]&0xC0)==0x40)
		rf_gain_info.RF_gain3 = 8;
	else if((R858_Array[R858_Num][22]&0xC0)==0x80)
		rf_gain_info.RF_gain3 = 10;
	else
		rf_gain_info.RF_gain3 = 12;
*/

	//Limit LNA, MixerAmp max gain
	if(rf_gain_info.RF_gain3 > 12)
        rf_gain_info.RF_gain3 = 12; 


   //max gain indicator
	if((rf_gain_info.RF_gain1==31) && (rf_gain_info.RF_gain2==15) && (rf_gain_info.RF_gain3==12) && (rf_gain_info.RF_gain4==15))  
		*fgRfMaxGain = 1;
	else
		*fgRfMaxGain = 0;

/*
//TF_HPF_Corner
if((LO_freq>0) && (LO_freq<480000))  
     R858_Freq_Info.TF_HPF_CNR = 3;   //lowest	=> R16[4:3]
else if((LO_freq>=480000) && (LO_freq<550000))  
     R858_Freq_Info.TF_HPF_CNR = 2;   // low	=> R16[4:3]
else if((LO_freq>=550000) && (LO_freq<700000))  
	 R858_Freq_Info.TF_HPF_CNR = 1;   // high    => R16[4:3]
else
	 R858_Freq_Info.TF_HPF_CNR = 0;   //highest	=> R16[4:3]
*/
/*
<230 => u1FreqIndex = 0
230~420 => u1FreqIndex = 1
420~710 => u1FreqIndex = 2
>710 => u1FreqIndex = 3
*/

/*
		//RF polyfilter band
	if((LO_freq>0) && (LO_freq<133000))  
         R858_Freq_Info.RF_POLY = 2;   //R17[6:5]=2; low	=> R18[1:0]
	else if((LO_freq>=133000) && (LO_freq<221000))  
         R858_Freq_Info.RF_POLY = 1;   //R17[6:5]=1; mid	=> R18[1:0]
	else if((LO_freq>=221000) && (LO_freq<789000))  
		 R858_Freq_Info.RF_POLY = 0;   //R17[6:5]=0; highest    => R18[1:0]
	else
		 R858_Freq_Info.RF_POLY = 3;   //R17[6:5]=3; ultra high	=> R18[1:0]
*/

/*
Freq	SFC-U Start		RFGain Start									RF RSSI show value	(dBm)		SFC-U Start - RF RSSI show value (dBm)
50			 0dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							+1
100			-1dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							0
130			-1dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							0

140			-2dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-1
150			-3dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-2
200			-3dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-2
210			-4dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-3
	
220			-5dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-4
230			-7dB		lna=0    rf=0	 mixer = 2->5  filter = 0		0 -> -2							-5
240			-7dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-6
250			-6dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-5
260			-5dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-4

270			-4dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-3
280			-4dB		lna=0    rf=0	 mixer = 2->5  filter = 0		0 -> -2							-2
290			-3dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-2
300			-3dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-2
310			-3dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							-2

320			-1dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							0
330			+1dB		lna=0    rf=0	 mixer = 2->4  filter = 0		0 -> -1							+2
340			-1dB		lna=0    rf=0	 mixer = 5->6  filter = 0		-2 -> -3						+2

350			+1dB		lna=0    rf=0	 mixer = 7->8  filter = 0		-4 -> -5						+6
400			+0dB		lna=0    rf=0	 mixer = 6->7  filter = 0		-3 -> -4						+4
450			+1dB		lna=0    rf=0	 mixer = 4->5  filter = 0		-1 -> -2						+3
500			+0dB		lna=0    rf=0	 mixer = 7->8  filter = 0		-4 -> -5						+5
550			+0dB		lna=0    rf=0	 mixer = 6->7  filter = 0		-3 -> -4						+4
600			+1dB		lna=0    rf=0	 mixer = 6->7  filter = 0		-3 -> -4						+5
650			+1dB		lna=0    rf=0	 mixer = 5->6  filter = 0		-2 -> -3						+4

700			+0dB		lna=0    rf=0	 mixer = 4->5  filter = 0		-1 -> -2						+2
750			+1dB		lna=0    rf=0	 mixer = 6->7  filter = 0	    -3 -> -4						+5	
800			+1dB		lna=0    rf=0	 mixer = 5->6  filter = 0		-2 -> -3						+4
850			+0dB		lna=0    rf=0	 mixer = 4->5  filter = 0		-1 -> -2						+2
900			+1dB		lna=0    rf=0	 mixer = 5->6  filter = 0		-2 -> -3						+4
950			+1dB		lna=0    rf=0	 mixer = 5->6  filter = 0		-2 -> -3						+4

Result
INT8 R858_Start_Gain_By_Freq[7] = {0,      -20,     -50,     -20,     0,       20,      40};
								  {50~135, 135~215, 215~265, 265~315, 315~325, 325~345, 345~950}
*/

//coarse adjustment
	if(RF_Freq_Khz<135000)   //<135M
	{
		u1FreqIndex = 0;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[0];
	}
	else if((RF_Freq_Khz>=135000)&&(RF_Freq_Khz<215000))   //135~215M
	{
		u1FreqIndex = 0;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[1];
	}
	else if((RF_Freq_Khz>=215000)&&(RF_Freq_Khz<265000))   //215~265M
	{
		u1FreqIndex = 1;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[2];
	}
	else if((RF_Freq_Khz>=265000)&&(RF_Freq_Khz<315000))   //265~315M
	{
		u1FreqIndex = 1;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[3];
	}
	else if((RF_Freq_Khz>=315000)&&(RF_Freq_Khz<325000))   //315~325M
	{
		u1FreqIndex = 1;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[4];
	}
	else if((RF_Freq_Khz>=325000)&&(RF_Freq_Khz<345000))   //325~345M
	{
		u1FreqIndex = 1;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[5];
	}
	else if((RF_Freq_Khz>=345000)&&(RF_Freq_Khz<420000))   //345~420M
	{
		u1FreqIndex = 1;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[6];
	}
	else if((RF_Freq_Khz>=420000)&&(RF_Freq_Khz<710000))   //420~710M
	{
		u1FreqIndex = 2;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[6];

	}
	else    // >=710
	{
		u1FreqIndex = 3;
		u2FreqFactor = R858_Start_Gain_Cal_By_Freq[6];
	}


	//LNA Gain
	acc_lna_gain = R858_Lna_Acc_Gain[u1FreqIndex][rf_gain_info.RF_gain1];



//fine adjustment
	//Cal LNA Gain	by Freq 
//Method 2 : All frequencies are finely adjusted..				
	if(rf_gain_info.RF_gain1 >= 10)
	{
		u1LnaGainqFactorIdx = (UINT8) ((RF_Freq_Khz-50000) / 10000);

		if( ((RF_Freq_Khz-50000)  - (u1LnaGainqFactorIdx * 10000))>=5000)
			u1LnaGainqFactorIdx +=1;

		acc_lna_gain = acc_lna_gain + (UINT16)(Lna_Acc_Gain_offset[u1LnaGainqFactorIdx]);

	}

	//RF buf
	acc_rfbuf_gain = R858_Rf_Acc_Gain[rf_gain_info.RF_gain2];
	if(RF_Freq_Khz<=300000)
		acc_rfbuf_gain += (rf_gain_info.RF_gain2 * 1);
	else if (RF_Freq_Khz>=600000)
		acc_rfbuf_gain -= (rf_gain_info.RF_gain2 * 1);

	//Mixer 
	acc_mixer_gain = R858_Mixer_Acc_Gain [rf_gain_info.RF_gain3]  ;

	//Add Rf Buf and Mixer Gain
	rf_total_gain = acc_lna_gain + acc_rfbuf_gain + acc_mixer_gain + rf_gain_info.RF_gain4*15;

	rf_rssi = fine_tune - (INT32) (rf_total_gain - u2FreqFactor);

	*RfLevelDbm = rf_rssi*100;

    return RT_Success;
}

//-----------------------------------------------------------------------//
//  R858_GetIfRssi( ): Get IF VGA GAIN                                   //
//  1st parameter: return IF VGA Gain     (dB*100)                       //
//-----------------------------------------------------------------------//
R858_ErrCode R858_GetIfRssi(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, INT32 *VgaGain)
{
	UINT8   adc_read;
    I2C_TYPE     Dlg_I2C;
	I2C_LEN_TYPE Dlg_I2C_Len;

	//Assign device address
	Dlg_I2C.I2cAddr=R858_ADDRESS[R858_Num][IntTunerNum];


//Optimize value
	UINT16   vga_table[64] = {                        //*100
		0, 0, 20, 20, 30, 50, 60, 80, 110, 130, 130, 160,   //0~11
		200, 240, 280, 330, 380, 410, 430, 480, 530, 590,   //12~21
		640, 690, 730, 760, 780, 810, 840, 890, 930, 950,   //22~31
		980, 1010, 1010, 1030, 1060, 1100, 1120, 1140, 1170, 1180,   //32~41
		1190, 1210, 1220, 1260, 1270, 1300, 1320, 1320, 1340, 1340,   //42~51
		1360, 1390, 1400, 1420, 1440, 1450, 1460, 1480, 1490, 1510,   //52~61
		1550, 1600            //62~63
	};

	//ADC sel : vagc, 41[1:0]=2
	Dlg_I2C.RegAddr = 41;
	R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr] = (R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr] & 0xFC) | 0x02;
	Dlg_I2C.Data = R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr];
	if(I2C_Write(R858_Num, &Dlg_I2C) != RT_Success)
		return RT_Fail;

	//IF_AGC read, R41[3]=1
	Dlg_I2C.RegAddr = 41;
	R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr] | 0x08;
	Dlg_I2C.Data = R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr];
	if(I2C_Write(R858_Num, &Dlg_I2C) != RT_Success)
		return RT_Fail;

	//ADC power on, R9[2]=0
	Dlg_I2C.RegAddr = 9;
	R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr] = R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr] & 0xFD;
	Dlg_I2C.Data = R858_Array[R858_Num][IntTunerNum][Dlg_I2C.RegAddr];
	if(I2C_Write(R858_Num, &Dlg_I2C) != RT_Success)
		return RT_Fail;

	//read adc value
	Dlg_I2C_Len.RegAddr = 0;
	Dlg_I2C_Len.Len = 5;
	if(I2C_Read_Len(R858_Num , &Dlg_I2C_Len) != RT_Success) // read data length
	{
		I2C_Read_Len(R858_Num , &Dlg_I2C_Len);
	}
	adc_read = (Dlg_I2C_Len.Data[4] & 0x3F);

	*VgaGain = vga_table[adc_read];

	return RT_Success;
}

//----------------------------------------------------------------------//
//  R858_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                                //
//  2nd parameter: input Standard                                           //
//  3rd parameter: return signal level indicator (dBm)               //
//-----------------------------------------------------------------------//
R858_ErrCode R858_GetTotalRssi(R858_ExtTunerNum_Type R858_Num, R858_IntTunerNum_Type IntTunerNum, UINT32 RF_Freq_Khz, R858_Standard_Type RT_Standard, INT32 *RssiDbm)
{
	INT32   rf_rssi;
	INT32   if_rssi;
	INT32   rem, total_rssi;
	INT32   ssi_offset = 0;   //need to fine tune by platform
	INT32   total_rssi_dbm;
	UINT8  rf_max_gain_flag;

	R858_GetRfRssi(R858_Num, IntTunerNum, RF_Freq_Khz, RT_Standard, &rf_rssi, &rf_max_gain_flag);

	R858_GetIfRssi(R858_Num, IntTunerNum, &if_rssi);  //vga gain        

    total_rssi = rf_rssi - (if_rssi*10);
	rem = total_rssi - (total_rssi/1000)*1000; //for rounding
	if((rem>-500) && (rem<500))
		total_rssi_dbm = (total_rssi/1000);
	else if(rem<=-500)
		total_rssi_dbm = (total_rssi/1000)-1;
	else
		total_rssi_dbm = (total_rssi/1000)+1;

	//for different platform, need to fine tune offset value
	*RssiDbm = total_rssi_dbm + ssi_offset;   

	return RT_Success;
}

//================================================
// Mstar Drv
//================================================
MS_BOOL MDrv_Tuner_R858_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    R858_ExtTunerNum_Type R858_Num;
    R858_IntTunerNum_Type IntTunerNum;
    MS_U8 i;

    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

    R858_Num = (R858_ExtTunerNum_Type)(u8TunerIndex/R858_TUNER_MAX_NUM);
    IntTunerNum = (R858_ExtTunerNum_Type)(u8TunerIndex%R858_TUNER_MAX_NUM);
    TUNER_DBG(("R858 Init.....%d %d\n", (int)R858_Num, (int)IntTunerNum));

    for(i=0; i<R858_TUNER_MAX_NUM; i++)
    {
        if(RT_Fail == R858_Init(R858_Num, i))
        {
           TUNER_ERR(("R858 Init FAIL..... \n"));
           return FALSE;
        }
    }

    
    for(i=0; i<R858_TUNER_MAX_NUM; i++)
    {
        if(RT_Fail ==  R858_SetStandard(R858_Num, i, R858_DVB_C_8M_IF_5M))
        {
           TUNER_ERR(("R858 Preset standrad FAIL..... \n"));
           return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL MDrv_Tuner_R858_SetTuner(MS_U8 u8TunerIndex, MS_U32 u32Freq, MS_U8 feBw)
{
    R858_ExtTunerNum_Type R858_Num;
    R858_IntTunerNum_Type IntTunerNum;
    R858_Set_Info R858_Info;

    R858_Num = (R858_ExtTunerNum_Type)(u8TunerIndex/R858_TUNER_MAX_NUM);
    IntTunerNum = (R858_ExtTunerNum_Type)(u8TunerIndex%R858_TUNER_MAX_NUM);

    //R840 Standard Setting
    if(DVBT2 == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
         switch(feBw)
         {
          case TUNER_BW_MODE_6MHZ:
              R858_Info.R858_Standard = R858_DVB_T2_6M_IF_5M;
              break;
          case TUNER_BW_MODE_7MHZ:
              R858_Info.R858_Standard = R858_DVB_T2_7M_IF_5M;
              break;
          case TUNER_BW_MODE_8MHZ:
          default:
              R858_Info.R858_Standard = R858_DVB_T2_8M_IF_5M;
              break;
          }
    }
    else if(DVBT == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
          switch(feBw)
           {
           case TUNER_BW_MODE_6MHZ:
               R858_Info.R858_Standard = R858_DVB_T_6M_IF_5M;
               break;
           case TUNER_BW_MODE_7MHZ:
               R858_Info.R858_Standard = R858_DVB_T_7M_IF_5M;
               break;
           case TUNER_BW_MODE_8MHZ:
           default:
               R858_Info.R858_Standard = R858_DVB_T_8M_IF_5M;
               break;
           }
    }
    else if(DVBC == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
           switch(feBw)
           {
           case TUNER_BW_MODE_6MHZ:
               R858_Info.R858_Standard = R858_DVB_C_6M_IF_5M;
               break;
           case TUNER_BW_MODE_7MHZ:
               R858_Info.R858_Standard = R858_DVB_C_6M_IF_5M;
               break;
           case TUNER_BW_MODE_8MHZ:
           default:
               R858_Info.R858_Standard = R858_DVB_C_8M_IF_5M;
               break;
           }
    }
    else if(ATSC == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
           R858_Info.R858_Standard = R858_ATSC_IF_5M;
    }
    else
    {
        R858_Info.R858_Standard = R858_DVB_T_8M_IF_5M;
    }

    //R858 Frequency Setting
    R858_Info.RF_KHz = u32Freq; // unit: kHz
    //Loop Through
    R858_Info.R858_LT = LT_ON; // Loop through ON
    //Clk output
    R858_Info.R858_ClkOutMode = CLK_OUT_OFF; // No tuner clock output for other IC

    R858_Info.R858_User_Mode= R858_DUAL; // Select IF AGC1
    //Configure Tuner
    if(R858_SetPllData(R858_Num, IntTunerNum,R858_Info) != RT_Success)
        return FALSE;


    return TRUE;
}

MS_BOOL R858_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            TUNER_DBG(("[R858]Request extension function (%x) does not exist\n",fuction_type));
    }
    return TRUE;
}

MS_BOOL MDrv_Tuner_R858_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
   *pu32IF_Freq = (MS_U32)Sys_Info1.IF_KHz;
    return TRUE;
}

#define R858_CHIP_ID 0x96
MS_BOOL R858_I2C_SLAVE_ID_CHK(MS_U8 u8TunerIndex, MS_U8* pu8SlveID)
{
    MS_U8 i;
    MS_BOOL bRet = FALSE;

    for(i=0; i< sizeof(u8Possible_SLAVE_IDs); i++)
    {
        if(u8Possible_SLAVE_IDs[i] == InitParam[u8TunerIndex].u8SlaveID)
            continue;
        R858_I2C_Len.I2cAddr= u8Possible_SLAVE_IDs[i];
        R858_I2C_Len.RegAddr= 0;
        R858_I2C_Len.Len = 1;
        R858_I2C_Len.Data[0] = 0;
        if(I2C_Read_Len(u8TunerIndex,&R858_I2C_Len) != RT_Success)
        {
            TUNER_ERR(("[R858] Read chip ID fail with slave ID 0x%x\n", R858_I2C_Len.I2cAddr));
        }
        else
        {
             bRet = TRUE;
             *pu8SlveID = u8Possible_SLAVE_IDs[i];
             break;
        }
    }

    return bRet;

}
MS_BOOL R858_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    UINT8 regData = 0;
    MS_U8 u8slaveID;

    InitParam[u8TunerIndex].u8SlaveID = 0;
    InitParam[u8TunerIndex].u8SlaveID_EXT = 0;

    if(R858_I2C_SLAVE_ID_CHK(u8TunerIndex, &u8slaveID))
    {
       InitParam[u8TunerIndex].u8SlaveID = u8slaveID;
       TUNER_ERR(("[R858]Got 1st tuner slave ID 0x%02x\n", u8slaveID));
       if(R858_I2C_SLAVE_ID_CHK(u8TunerIndex, &u8slaveID))
       {
          InitParam[u8TunerIndex].u8SlaveID_EXT = u8slaveID;
          TUNER_ERR(("[R858]Got 2nd tuner slave ID 0x%02x\n", u8slaveID));
       }
       else
       {
          TUNER_ERR(("[R858]Try 2nd tuner slave ID Fail\n"));
          return FALSE;
       }
    }
    else
    {
        TUNER_ERR(("[R858]Try 1st tuner slave ID Fail\n"));
        return FALSE;
    }
    
    regData = R858_I2C_Len.Data[0];
    TUNER_ERR(("[R858] read id =0x%x\n",regData));

    if(regData == R858_CHIP_ID)
    {
        R858_ADDRESS[u8TunerIndex][0] = InitParam[u8TunerIndex].u8SlaveID;
        R858_ADDRESS[u8TunerIndex][1] = InitParam[u8TunerIndex].u8SlaveID_EXT; 
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;//R858_TUNER_MAX_NUM;
    }
    return (regData == R858_CHIP_ID);

}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_R858) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_R858",          // demod system name
    .data               = TUNER_R858,            // private data value
    .Init               = MDrv_Tuner_R858_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_R858_SetTuner,
    .CheckExist         = R858_CheckExist,
    .GetTunerIF         = MDrv_Tuner_R858_GetTunerIF,
    .Extension_Function = R858_Extension_Function
};


#endif


