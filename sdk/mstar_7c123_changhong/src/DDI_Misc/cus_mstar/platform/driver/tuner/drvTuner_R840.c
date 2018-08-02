//-----------------------------------------------------//
//
// Filename: R836.c
//
// This file is R836 tuner driver
// Copyright 2013 by Rafaelmicro., Inc.
// Author: Jason Wang
//-----------------------------------------------------//

#include <string.h>
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
#define  R840_DTV_AGC_SLOW   FALSE

static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];
static MS_U8 u8Possible_SLAVE_IDs[2] = {0x74, 0x34};

#define  ADC_MULTI_READ  1
UINT32 ADC_READ_DELAY = 2;
UINT8  ADC_READ_COUNT = 1;
UINT8  VGA_DELAY = 5;
UINT8  FILTER_DELAY = 3;

UINT8 R840_iniArray[R840_REG_NUM] = {

                           0x02, 0x44, 0x40, 0x40, 0x0D,   //R4[3]=1, Mixer off
					  //   0x00, 0x01, 0x02, 0x03, 0x04

	                        0x69, 0x17, 0x75, 0x40, 0xC0, 0x06, 0x08, 0x89, 0xA4,   //R9[7]=1, poly off
					//     0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D

						   0x95, 0x9C, 0x33, 0xC2, 0x72, 0x00, 0xD5, 0x00, 0x00,   //R17[7:6]=11, PLL_A off; LO=800M(16M)
					//     0x0E  0x0F  0x10  0x11  0x12  0x13  0x14  0x15  0x16

						   //0xB0, 0x30, 0x40, 0x87, 0xC2, 0x25, 0x30, 0x26, 0x47}; //R23: LDO1=200R, LDO2=200R
						   0xD9, 0x30, 0x40, 0x87, 0xC2, 0x25, 0x30, 0x26, 0x47}; //PLL, IQ_gen off
					//     0x17  0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F


//UINT8 R840_ADDRESS = 0x74;
UINT8* pR840_Chip = NULL;
UINT8 R840_XtalDiv = XTAL_DIV2;
UINT8 R840_SetTfType = R840_TF_BEAD;   //Depend on HW
UINT8 R840_DetectTfType = R840_UL_USING_BEAD;
UINT8 R840_Fil_Cal_Gap = 8;
UINT32 R840_IF_HIGH = 8500;
UINT8* pR840_Xtal_Pwr = NULL;
UINT8* pR840_Xtal_Pwr_tmp = NULL;

//----------------------------------------------------------//
//                   Internal Parameters                    //
//----------------------------------------------------------//

UINT8* pR840_Array  = NULL;
UINT8* pR840_SBY = NULL;


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
UINT8* pR840_Fil_Cal_LpfLsb = NULL;
UINT8* pR840_IMR_Cal_Result = NULL;  //1: fail, 0: ok
UINT8* pR840_TF_Check_Result = NULL; //1: fail, 0: ok


R840_Standard_Type*  pR840_pre_standard= NULL;
UINT8  R840_Lpf_Lsb = 0;
static UINT8 R840_IMR_Cal_Type = R840_IMR_CAL;
//static UINT8 R840_SBY[R840_REG_NUM];
UINT8 *pR840_PreGainMode = NULL; //RF_AUTO; //for test use
//0x00(8M), 0x40(7M), 0x60(6M)
static UINT8 R840_Fil_Cal_BW_def[R840_STD_SIZE]={
       0x60, 0x60, 0x00, 0x00, 0x40, 0x40, 0x00, 0x40, 0x00, //ATV
	   0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       //ATV (CIF 5M)
	   0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00,       //DVB-T, DVB-T2
	   0x00, 0x40, 0x40, 0x40, 0x40, 0x00, 0x40, 0x40,       //DVB-C, J83B, ISDBT, DTMB, ATSC
	   0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x60,             //DVB-T, DVB-T2 (IF 5M)
	   0x00, 0x40, 0x40, 0x40, 0x00, 0x40, 0x40, 0x40        //DVB-C, J83B, ISDBT, DTMB, ATSC (IF 5M), FM
       };
static UINT8 R840_Fil_Cal_code_def[R840_STD_SIZE]={
       9, 2, 4, 5, 2, 2, 5, 0, 5,     //ATV
       2, 2, 3, 5, 3, 3, 3, 3,        //ATV (CIF 5M)
       6, 2, 0, 6, 2, 0, 2, 2,        //DVB-T, DVB-T2
	   2, 1, 2, 7, 6, 6, 7, 1,        //DVB-C, J83B, ISDBT, DTMB, ATSC
	   4, 6, 5, 4, 6, 5, 11,          //DVB-T, DVB-T2 (IF 5M)
	   2, 0, 2, 3, 4, 2, 1, 6         //DVB-C, J83B, ISDBT, DTMB, ATSC (IF 5M), FM
       };

//0: L270n/68n(ISDB-T, DVB-T/T2)
//1: Bead/68n(DTMB)
//2: Bead/68n(ISDB-T, DVB-T/T2)
//3: L270n/68n(ATV)
//4: Bead/68n(DTMB+ATV)
//5: L270n/68n(ATSC, DVB-C, J83B)
//6: Bead/68n(ATSC, DVB-C, J83B)
UINT32 R840_LNA_HIGH_MID[R840_TF_SIZE] = { 612000, 612000, 612000, 612000, 612000, 500000, 500000};
UINT32 R840_LNA_MID_LOW[R840_TF_SIZE] = { 388000, 388000, 348000, 348000, 348000, 300000, 300000};
UINT32 R840_LNA_LOW_LOWEST[R840_TF_SIZE] = {164000, 164000, 148000, 124000, 124000, 156000, 156000};

UINT32 R840_TF_Freq_High[R840_TF_SIZE][R840_TF_HIGH_NUM] =
{    { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
     { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
       { 784000, 776000, 680000, 608000, 584000, 536000, 504000},
     { 784000, 776000, 680000, 608000, 584000, 536000, 504000}
};
UINT32 R840_TF_Freq_Mid[R840_TF_SIZE][R840_TF_MID_NUM] =
{     {608000, 584000, 560000, 488000, 464000, 440000, 416000, 392000},
      {608000, 584000, 560000, 488000, 464000, 440000, 416000, 392000},
      {608000, 584000, 560000, 488000, 464000, 440000, 416000, 392000},
      {608000, 560000, 488000, 464000, 440000, 416000, 392000, 352000},
      {608000, 560000, 488000, 464000, 440000, 416000, 392000, 352000},
      {488000, 464000, 440000, 416000, 392000, 352000, 320000, 304000},
	  {488000, 464000, 440000, 416000, 392000, 352000, 320000, 304000}
};
UINT32 R840_TF_Freq_Low[R840_TF_SIZE][R840_TF_LOW_NUM] =
{    {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
      {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
      {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
      {256000, 240000, 232000, 192000, 160000, 152000, 144000, 128000},  //124~348
      {264000, 240000, 192000, 184000, 176000, 152000, 144000, 128000},  //124~348
      {280000, 248000, 232000, 216000, 192000, 176000, 168000, 160000},  //156~300
      {280000, 248000, 232000, 216000, 192000, 176000, 168000, 160000}   //156~300
};
UINT32 R840_TF_Freq_Lowest[R840_TF_SIZE][R840_TF_LOWEST_NUM] =
{    {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
      {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
      {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
	  {120000, 96000,   88000,   80000, 72000, 64000, 56000, 48000},
	  {104000, 96000,   88000,   80000, 72000, 64000, 56000, 48000},
	  {136000, 120000, 104000, 88000, 72000, 64000, 56000, 48000},
	  {128000, 120000, 104000, 96000, 80000, 72000, 64000, 56000}
};


UINT8 R840_TF_Result_High[R840_TF_SIZE][R840_TF_HIGH_NUM] =
{    {0x00, 0x01, 0x03, 0x04, 0x08, 0x0A, 0x0A},
      {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x09},
      {0x00, 0x01, 0x03, 0x04, 0x08, 0x0A, 0x0A},
      {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x09},
      {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x09},
      {0x00, 0x01, 0x05, 0x0A, 0x0C, 0x13, 0x19},
	  {0x00, 0x01, 0x05, 0x0A, 0x0C, 0x13, 0x19}
};
UINT8 R840_TF_Result_Mid[R840_TF_SIZE][R840_TF_MID_NUM] =
{    {0x02, 0x03, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15},
      {0x02, 0x03, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15},
      {0x02, 0x03, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15},
      {0x02, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19},
      {0x02, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19},
      {0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19, 0x1E, 0x1F},
      {0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19, 0x1E, 0x1F}
};
UINT8 R840_TF_Result_Low[R840_TF_SIZE][R840_TF_LOW_NUM] =
{    {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0D, 0x13, 0x16},
      {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0B, 0x0F, 0x16},
      {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0D, 0x13, 0x16},
      {0x05, 0x07, 0x0A, 0x0F, 0x1A, 0x1A, 0x23, 0x2A},
      {0x05, 0x08, 0x10, 0x13, 0x1A, 0x1A, 0x23, 0x2A},
      {0x05, 0x08, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1A},
      {0x05, 0x08, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1A},
};
UINT8 R840_TF_Result_Lowest[R840_TF_SIZE][R840_TF_LOWEST_NUM] =
{    {0x00, 0x06, 0x0C, 0x15, 0x1C, 0x1F, 0x3C, 0x3F},
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08},
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08},
      {0x06, 0x11, 0x15, 0x1C, 0x1F, 0x2F, 0x3C, 0x3F},
      {0x04, 0x08, 0x08, 0x08, 0x10, 0x12, 0x13, 0x13},
      {0x06, 0x09, 0x0E, 0x18, 0x25, 0x2F, 0x3C, 0x3F},
      {0x00, 0x04, 0x04, 0x08, 0x08, 0x10, 0x12, 0x13}
};

UINT8  R840_TF = 0;

static UINT16 R840_Lna_Plain_Acc_Gain[5][25] =
{
 {0, 13, 25, 46, 65,  84, 102, 120, 137, 156, 176, 189, 204, 222, 237, 247, 255, 266, 280, 294, 307, 321, 326, 338, 338},
 {0, 15, 27, 57, 90, 116, 139, 161, 181, 204, 228, 237, 246, 263, 276, 286, 293, 303, 310, 317, 322, 327, 326, 331, 335},
 {0, 14, 31, 57, 80, 103, 124, 145, 164, 184, 206, 214, 224, 242, 256, 266, 274, 284, 291, 299, 305, 311, 307, 312, 317},
 {0,   0,  9,  31, 53,  74,  92, 110, 127, 147, 166, 176, 187, 205, 219, 229, 236, 247, 255, 264, 271, 277, 280, 285, 285},
 {0,   0, 15, 30, 45,  57,  71,  85,   97, 110, 122, 133, 147, 167, 182, 195, 204, 216, 228, 240, 251, 262, 264, 274, 275}
};

static UINT16 R840_Lna_Sharp_Acc_Gain[5][25] =
{
 {105, 105, 105, 125, 145, 164, 182, 200, 218, 237, 258, 257, 258, 277, 291, 301, 309, 320, 320, 320, 320, 320, 326, 338, 338},
 { 82,   82,   84, 105, 135, 159, 180, 201, 223, 244, 268, 268, 270, 287, 300, 310, 318, 327, 327, 327, 327, 327, 326, 331, 335},
 {104, 105, 104, 120, 137, 157, 174, 193, 210, 229, 248, 248, 251, 269, 282, 293, 300, 310, 310, 310, 310, 311, 307, 312, 317},
 { 58,   60,   58,   80, 101, 122, 140, 159, 176, 195, 215, 215, 217, 235, 249, 259, 266, 277, 277, 277, 277, 277, 280, 285, 285},
 { 89,   88,   89, 102, 115, 128, 140, 153, 165, 177, 189, 190, 192, 212, 228, 240, 249, 262, 262, 262, 262, 262, 264, 274, 275}
};

static UINT16 R840_Lna_Pulse_Comp[5][4] =
{
	{  9, 25, 24, 24},
	{14, 40, 40, 39},
	{16, 35, 33, 31},
	{14, 21, 20, 20},
	{12, 18, 17, 17}
};
//----------------------------------------------------------//
//                   Internal static struct                         //
//----------------------------------------------------------//
SysFreq_Info_Type* pSysFreq_Info1 = NULL;
Sys_Info_Type*     pSys_Info1 = NULL;
Freq_Info_Type*    pFreq_Info1 = NULL;
#if 1 // byKOR, kaon
	R840_Set_Info*  pR840_Info = NULL;
#else
static MS_U32 u32CurFreqKhz[MAX_FRONTEND_NUM];
#endif
//----------------------------------------------------------//
//                   Internal Functions                            //
//----------------------------------------------------------//
R840_ErrCode R840_TF_Check(MS_U8 u8TunerIndex);
R840_ErrCode R840_Xtal_Check(MS_U8 u8TunerIndex);
R840_ErrCode R840_InitReg(MS_U8 u8TunerIndex);
R840_ErrCode R840_Cal_Prepare(MS_U8 u8TunerIndex,UINT8 u1CalFlag);
R840_ErrCode R840_IMR(MS_U8 u8TunerIndex,UINT8 IMR_MEM, MS_BOOL IM_Flag);
R840_ErrCode R840_PLL(MS_U8 u8TunerIndex,UINT32 LO_Freq, R840_Standard_Type R840_Standard);
R840_ErrCode R840_MUX(MS_U8 u8TunerIndex,UINT32 LO_KHz, UINT32 RF_KHz, R840_Standard_Type R840_Standard);
R840_ErrCode R840_IQ(MS_U8 u8TunerIndex,R840_SectType* IQ_Pont);
R840_ErrCode R840_IQ_Tree(MS_U8 u8TunerIndex,UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree);
R840_ErrCode R840_IQ_Tree5(MS_U8 u8TunerIndex,UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree);
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

Sys_Info_Type R840_Sys_Sel(MS_U8 u8TunerIndex,R840_Standard_Type R840_Standard);
Freq_Info_Type R840_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R840_Standard_Type R840_Standard);
SysFreq_Info_Type R840_SysFreq_Sel(MS_U8 u8TunerIndex,R840_Standard_Type R840_Standard,UINT32 RF_freq);

UINT8 R840_Filt_Cal_ADC(MS_U8 u8TunerIndex,UINT32 IF_Freq, UINT8 R840_BW, UINT8 FilCal_Gap);

//--------------------------------------------------------------
//           #define R840_DEBUG
//--------------------------------------------------------------
static MS_BOOL R836_Variables_alloc(void)
{

    UINT8 i;
	#if 1 // byKOR, kaon
		if(NULL == pR840_Info)
           pR840_Info = (R840_Set_Info*)malloc(sizeof(R840_Set_Info)*MAX_FRONTEND_NUM);

       if(NULL == pR840_PreGainMode)
          pR840_PreGainMode = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);
	#endif
       if(NULL == pR840_Chip)
          pR840_Chip = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);

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

       if(NULL == pR840_Fil_Cal_LpfLsb)
           pR840_Fil_Cal_LpfLsb = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_STD_SIZE);

       if(NULL == pR840_Fil_Cal_code)
           pR840_Fil_Cal_code = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_STD_SIZE);

       if(NULL == pR840_pre_standard)
           pR840_pre_standard  = (R840_Standard_Type*)malloc(sizeof(R840_Standard_Type)*MAX_FRONTEND_NUM);

       if(NULL == pR840_Array)
           pR840_Array = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_REG_NUM);

       if(NULL == pR840_SBY)
           pR840_SBY = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM*R840_REG_NUM);

       if(NULL == pR840_IMR_Cal_Result)
           pR840_IMR_Cal_Result = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);

       if(NULL == pR840_TF_Check_Result)
           pR840_TF_Check_Result = (UINT8*)malloc(sizeof(UINT8)*MAX_FRONTEND_NUM);


       if((NULL == pSysFreq_Info1) || (NULL == pSys_Info1) || (NULL == pSysFreq_Info1) || (NULL == pR840_Fil_Cal_LpfLsb)||\
           (NULL == pR840_Xtal_Pwr) || (NULL == pR840_Xtal_Pwr_tmp) || (NULL == pR840_Initial_done_flag) ||\
           (NULL == pR840_IMR_done_flag) || (NULL == pR840_Fil_Cal_flag) || (NULL == pR840_Fil_Cal_BW) ||\
           (NULL == pR840_IMR_Cal_Result) || (NULL == pR840_TF_Check_Result) ||\
           (NULL == pR840_Fil_Cal_code) || (NULL == pR840_pre_standard) || (NULL == pR840_Array) || (NULL == pR840_Chip) || (NULL == pR840_SBY))
        {
		#if 1 // byKOR, kaon
			if(NULL != pR840_Info)
				free(pR840_Info);

            if(NULL != pR840_PreGainMode)
                free(pR840_PreGainMode);
		#endif
            if(NULL != pR840_Chip)
                free(pR840_Chip);

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

             if(NULL != pR840_SBY)
               free(pR840_SBY);

             if(NULL != pR840_Fil_Cal_LpfLsb)
                free(pR840_Fil_Cal_LpfLsb);

             if(NULL != pR840_IMR_Cal_Result)
                free(pR840_IMR_Cal_Result);

             if(NULL != pR840_TF_Check_Result)
                free(pR840_TF_Check_Result);

           return FALSE;
        }
        else
        {
           for(i=0;i<MAX_FRONTEND_NUM;i++)
           {
              *(pR840_Chip + i) = R836_R;
              *(pR840_Xtal_Pwr + i) = XTAL_GM3V_STRONG;
              *(pR840_Xtal_Pwr_tmp + i) = XTAL_GM3V_STRONG;
              *(pR840_Initial_done_flag + i) = FALSE;
              *(pR840_IMR_done_flag + i) = FALSE;
			#if 1 // byKOR, kaon
              *(pR840_PreGainMode + i) = RF_AUTO;
			#endif
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

static MS_BOOL I2C_Read_Len(MS_U8 u8TunerIndex,I2C_LEN_TYPE *I2C_Info)
{
    MS_U8 DataCunt = 0;
    MS_U8 u8SlaveID = InitParam[u8TunerIndex].u8SlaveID;
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
    MS_U8 u8SlaveID = InitParam[u8TunerIndex].u8SlaveID;
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

static MS_BOOL I2C_Write_Len(MS_U8 u8TunerIndex,I2C_LEN_TYPE *I2C_Info)
{
    MS_U8 u8SlaveID = InitParam[u8TunerIndex].u8SlaveID;
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


Sys_Info_Type R840_Sys_Sel(MS_U8 u8TunerIndex, R840_Standard_Type R840_Standard)
{
    Sys_Info_Type R840_Sys_Info = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    switch (R840_Standard)
    {

        case R840_DVB_T_6M:
        case R840_DVB_T2_6M:
            R840_Sys_Info.IF_KHz=4570;                    //IF
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7150;          //CAL IF
            R840_Sys_Info.HPF_COR=0x05;              //R11[3:0]=5 (2.0M)
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
            R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
            break;

       case R840_DVB_T_7M:
       case R840_DVB_T2_7M:
           R840_Sys_Info.IF_KHz=4570;                     //IF
           R840_Sys_Info.BW=0x40;                           //BW=7M
           R840_Sys_Info.FILT_CAL_IF=7750;           //CAL IF
           R840_Sys_Info.HPF_COR=0x08;	             //R11[3:0]=8 (1.45M)
           R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
           R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
           break;

        case R840_DVB_T_8M:
        case R840_DVB_T2_8M:
            R840_Sys_Info.IF_KHz=4570;                     //IF
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=8130;           //CAL IF
            R840_Sys_Info.HPF_COR=0x09;              //R11[3:0]=9 (1.15M)
            R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
            break;

        case R840_DVB_T2_1_7M:
            R840_Sys_Info.IF_KHz=1900;
            R840_Sys_Info.BW=0x40;                           //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7800;           //CAL IF
            R840_Sys_Info.HPF_COR=0x08;             //R11[3:0]=8
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+3
            break;

        case R840_DVB_T2_10M:
            R840_Sys_Info.IF_KHz=5600;
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=10800;         //CAL IF
            R840_Sys_Info.HPF_COR=0x0C;             //R11[3:0]=12
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+3
            break;

        case R840_DVB_C_8M:
            R840_Sys_Info.IF_KHz=5070;
            R840_Sys_Info.BW=0x00;                           //BW=8M
            R840_Sys_Info.FILT_CAL_IF=9000;           //CAL IF
            R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+1
            break;

       case R840_DVB_C_6M:
           R840_Sys_Info.IF_KHz=5070;
           R840_Sys_Info.BW=0x40;                          //BW=7M
           R840_Sys_Info.FILT_CAL_IF=8025;          //CAL IF
           R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
           R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+3
           break;

       case R840_J83B:
           R840_Sys_Info.IF_KHz=5070;
           R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7850;          //CAL IF
           R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
           R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
           R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
           break;

    case R840_ISDB_T_4063:
            R840_Sys_Info.IF_KHz=4063;
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7000;          //CAL IF  //7200
            R840_Sys_Info.HPF_COR=0x08;             //R11[3:0]=8
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
            break;

        case R840_ISDB_T_4570:
            R840_Sys_Info.IF_KHz=4570;                    //IF
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7250;          //CAL IF
            R840_Sys_Info.HPF_COR=0x05;	             //R11[3:0]=5 (2.0M)
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
            break;

    case R840_DTMB_8M_4570:
            R840_Sys_Info.IF_KHz=4570;
            R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF
            R840_Sys_Info.HPF_COR=0x0B;             //R11[3:0]=11
            R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
            break;

    case R840_DTMB_6M_4500:
            R840_Sys_Info.IF_KHz=4500;
            R840_Sys_Info.BW=0x40;                           //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7000;           //CAL IF
            R840_Sys_Info.HPF_COR=0x05;              //R11[3:0]=5
            R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+1
            break;

    case R840_ATSC:
            R840_Sys_Info.IF_KHz=5070;
            R840_Sys_Info.BW=0x40;                          //BW=7M
            R840_Sys_Info.FILT_CAL_IF=7900;          //CAL IF
            R840_Sys_Info.HPF_COR=0x04;              //R11[3:0]=4
            R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
            R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
            break;

    case R840_DVB_T_6M_IF_5M:
    case R840_DVB_T2_6M_IF_5M:
           R840_Sys_Info.IF_KHz=5000;                    //IF
           R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7700;          //CAL IF
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4 (2.25M)
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
           break;

    case R840_DVB_T_7M_IF_5M:
    case R840_DVB_T2_7M_IF_5M:
           R840_Sys_Info.IF_KHz=5000;                     //IF
           R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8000;           //CAL IF
		R840_Sys_Info.HPF_COR=0x06;	             //R11[3:0]=6 (1.78M)
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
           break;

    case R840_DVB_T_8M_IF_5M:
    case R840_DVB_T2_8M_IF_5M:
          R840_Sys_Info.IF_KHz=5000;                     //IF
          R840_Sys_Info.BW=0x00;                           //BW=8M
          R840_Sys_Info.FILT_CAL_IF=8500;           //CAL IF
          R840_Sys_Info.HPF_COR=0x08;              //R11[3:0]=8 (1.45M)
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
          break;

    case R840_DVB_T2_1_7M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x60;                           //BW=6M
        R840_Sys_Info.FILT_CAL_IF=5900;           //CAL IF
        R840_Sys_Info.HPF_COR=0x01;	             //R11[3:0]=1
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

    case R840_DVB_C_8M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=9000;           //CAL IF
        R840_Sys_Info.HPF_COR=0x0A;              //R11[3:0]=10
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+1
        break;

   case R840_DVB_C_6M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=8100;          //CAL IF
        R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

   case R840_J83B_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7800;          //CAL IF
        R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

   case R840_ISDB_T_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7700;          //CAL IF
        R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4 (2.25M)
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
        break;

    case R840_DTMB_8M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=8650;           //CAL IF
        R840_Sys_Info.HPF_COR=0x09;              //R11[3:0]=9
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

    case R840_DTMB_6M_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                           //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7700;           //CAL IF
        R840_Sys_Info.HPF_COR=0x04;              //R11[3:0]=4
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

    case R840_ATSC_IF_5M:
        R840_Sys_Info.IF_KHz=5000;
        R840_Sys_Info.BW=0x40;                          //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7900;          //CAL IF
        R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
        R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

   case R840_FM:
        R840_Sys_Info.IF_KHz=2400;
        R840_Sys_Info.BW=0x40;                           //BW=7M
        R840_Sys_Info.FILT_CAL_IF=7200;           //CAL IF
        R840_Sys_Info.HPF_COR=0x02;	             //R11[3:0]=2
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

   default:  //R840_DVB_T_8M
        R840_Sys_Info.IF_KHz=4570;                     //IF
        R840_Sys_Info.BW=0x00;                           //BW=8M
        R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF
        R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
        R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
        R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
        break;

    }


//Set By DTV/ATV
    if (R840_Standard < R840_ATV_SIZE)  //ATV
    {
        R840_Sys_Info.SWCAP_CLK = 0x02;      //8k       (R18[1:0]=2)
		//R840_Sys_Info.NA_PWR_DET = 0x80;   //off     (R30[7]=1)
        R840_Sys_Info.SWBUF_CUR = 0x00;     //high  (R4[2]=0)
        R840_Sys_Info.SD_IM = 0x08;               //2       (R3[3:2]=2'b10)
    }
    else  //other DTV
    {
        R840_Sys_Info.SWCAP_CLK = 0x02;      //8k       (R18[1:0]=2) //AGC 500Hz map to 8k
		//R840_Sys_Info.NA_PWR_DET = 0x00;   //on      (R30[7]=0)
        R840_Sys_Info.SWBUF_CUR = 0x04;     //low  (R4[2]=1)
        R840_Sys_Info.SD_IM = 0x00;              //0     (R3[3:2]=2'b00)
    }

    //NA
	switch(R840_Standard)
	{
		case R840_DTMB_8M_4570:
		case R840_DTMB_6M_4500:
		case R840_DTMB_8M_IF_5M:
		case R840_DTMB_6M_IF_5M:
		case R840_DVB_C_8M:   //836
		case R840_DVB_C_6M:
		case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
		case R840_DVB_C_6M_IF_5M:
		case R840_J83B_IF_5M:
			R840_Sys_Info.NA_PWR_DET = 0x00;   //on         (R30[7]=0)
			break;

		default:
			R840_Sys_Info.NA_PWR_DET = 0x80;   //off        (R30[7]=1)
			break;
	}
    R840_Sys_Info.FILT_CUR = 0x00;         //highest  (R10[6:5]=2'b00)
    R840_Sys_Info.RF_BUF_CUR = 0x00;    //high  (R4[7]=0)
    R840_Sys_Info.TF_CUR = 0x40;                  //low       (R3[6]=1)
    R840_Sys_Info.LNA_DET_MODE=0x01;    //discharge (R31[0]=1)
	R840_Sys_Info.NA_DISCHARGE = 0x04;   //1          (R31[4:2]=3'b001)
	R840_Sys_Info.AGC_CLK = 0x00;              //1k        (R27[5:3]=3'b000)
	R840_Sys_Info.FILT_COMP = 0x20;      //1          (R30[6:5]=2'b01)

    //Filter 3dB
    switch(R840_Standard)
    {
        case R840_DVB_C_8M_IF_5M:
            R840_Sys_Info.FILT_3DB = 0x08;         // ON       (R30[3]=1)
        break;

        default:
            R840_Sys_Info.FILT_3DB = 0x00;         // OFF      (R30[3]=0)
        break;
    }

    //HPF_notch
    if(R840_Standard==R840_DVB_T2_1_7M_IF_5M)
        R840_Sys_Info.HPF_NOTCH = 0x10;          //low        (R6[4]=1)
    else
        R840_Sys_Info.HPF_NOTCH = 0x00;          //high        (R6[4]=0)

    //BW 1.7M
    if((R840_Standard==R840_DVB_T2_1_7M) || (R840_Standard==R840_FM))
        R840_Sys_Info.V17M = 0x80;       //enable, R11[7]=1
    else
        R840_Sys_Info.V17M = 0x00;       //disable, R11[7]=0, (include DVBT2 1.7M IF=5MHz)

    //TF Type select
    switch(R840_Standard)
    {

		case R840_DVB_C_8M:
		case R840_DVB_C_6M:
		case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
		case R840_DVB_C_6M_IF_5M:
		case R840_J83B_IF_5M:
			 if((*(pR840_TF_Check_Result + u8TunerIndex)) ==1) //fail
			 {
				 R840_SetTfType = R840_TF_NARROW_ATSC;   //using 270n
			 }
			 else
			 {
             if(R840_DetectTfType==R840_UL_USING_BEAD)
                R840_SetTfType = R840_TF_BEAD_LIN_ATSC;   //using bead
             else
                R840_SetTfType = R840_TF_NARROW_ATSC;   //using 270n
			 }
             break;

        case R840_ATSC:
        case R840_ATSC_IF_5M:
			 if((*(pR840_TF_Check_Result + u8TunerIndex))==1) //fail
			 {
				 R840_SetTfType = R840_TF_NARROW_ATSC;   //using 270n
			 }
			 else
			 {
             if(R840_DetectTfType==R840_UL_USING_BEAD)
                R840_SetTfType = R840_TF_BEAD_LIN_ATSC;  //using bead
             else
                R840_SetTfType = R840_TF_NARROW_ATSC;  //using 270n
			 }
             break;

        case R840_DTMB_8M_4570:
        case R840_DTMB_6M_4500:
        case R840_DTMB_8M_IF_5M:
        case R840_DTMB_6M_IF_5M:
			 if((*(pR840_TF_Check_Result + u8TunerIndex))==1) //fail
			 {
				 R840_SetTfType = R840_TF_BEAD;   //using Bead
			 }
			 else
			 {
            if(R840_DetectTfType==R840_UL_USING_BEAD)
                  R840_SetTfType = R840_TF_BEAD;       //Force using Bead
             else
                  R840_SetTfType = R840_TF_NARROW;
			 }
             break;

        default:        //DVB-T/T2, ISDB-T
			 if((*(pR840_TF_Check_Result + u8TunerIndex))==1) //fail
			 {
				 R840_SetTfType = R840_TF_NARROW;   //using 270n
			 }
			 else
			 {
            if(R840_DetectTfType==R840_UL_USING_BEAD)
                R840_SetTfType = R840_TF_BEAD_TERR;
            else
                R840_SetTfType = R840_TF_NARROW;
			 }
            break;
    }

	R840_Sys_Info.LNA_CUR = 0;

    return R840_Sys_Info;
}



Freq_Info_Type R840_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R840_Standard_Type R840_Standard)
{
	Freq_Info_Type R840_Freq_Info = {0,0,0,0,0,0,0,0};

    //----- RF dependent parameter --------
    //LNA band
	if((RF_freq>0) && (RF_freq<R840_LNA_LOW_LOWEST[R840_SetTfType]))  //<164
        R840_Freq_Info.LNA_BAND = 0x60;   //R5[6:5]=2'b11; ultra low
    else if((RF_freq>=R840_LNA_LOW_LOWEST[R840_SetTfType]) && (RF_freq<R840_LNA_MID_LOW[R840_SetTfType]))  //164~388
        R840_Freq_Info.LNA_BAND = 0x40;   //R5[6:5]=2'b10; low
    else if((RF_freq>=R840_LNA_MID_LOW[R840_SetTfType]) && (RF_freq<R840_LNA_HIGH_MID[R840_SetTfType]))  //388~612
        R840_Freq_Info.LNA_BAND = 0x20;   //R5[6:5]=2'b01; mid
    else     // >612
        R840_Freq_Info.LNA_BAND = 0x00;   //R5[6:5]=2'b00; high

    //----- LO dependent parameter --------
    //IMR point
	if((LO_freq>0) && (LO_freq<133000))
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
	if((LO_freq>0) && (LO_freq<133000))
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
        case R840_DVB_C_6M:
        case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
        case R840_DVB_C_6M_IF_5M:
        case R840_J83B_IF_5M:
			if((LO_freq>0) && (LO_freq<77000))
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

		default:  //Air, DTMB
			if((LO_freq>0) && (LO_freq<73000))
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

	R840_Freq_Info.CP_CUR = 0;
	R840_Freq_Info.Q_CTRL = 0;
	R840_Freq_Info.XTAL_POW0 = 0;

    return R840_Freq_Info;

}



SysFreq_Info_Type R840_SysFreq_Sel(MS_U8 u8TunerIndex, R840_Standard_Type R840_Standard,UINT32 RF_freq)
{
	SysFreq_Info_Type R840_SysFreq_Info = {0,0,0,0,0,0,0,0,0,0,0,0};

    switch(R840_Standard)
    {

    case R840_DVB_T_6M:
    case R840_DVB_T_8M:
    case R840_DVB_T_6M_IF_5M:
    case R840_DVB_T_8M_IF_5M:

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
        R840_SysFreq_Info.LNA_TOP=0x03;       // LNA TOP=4                    (R27[2:0]=3'b011)
        R840_SysFreq_Info.LNA_VTH_L=0xA5;   // LNA VTH/L=1.34/0.84     (R13=0xA5)
        R840_SysFreq_Info.MIXER_TOP=0x05;       // MIXER TOP=10               (R28[3:0]=4'b0101)
        R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
        R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
        R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
        R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
        R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
        R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
        R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
    }
    else
    {
        if((RF_freq>=782000) && (RF_freq<=790000))
        {
            R840_SysFreq_Info.LNA_TOP=0x03;       // LNA TOP=4                    (R27[2:0]=3'b011)
            R840_SysFreq_Info.LNA_VTH_L=0x52;   // LNA VTH/L=0.84/0.54     (R13=0x52)
            R840_SysFreq_Info.MIXER_TOP=0x05;       // MIXER TOP=10               (R28[3:0]=4'b0101)
            R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
            R840_SysFreq_Info.RF_TOP=0x00;               // RF TOP=7                        (R26[7:5]=3'b000)
            R840_SysFreq_Info.NRB_TOP=0x80;            // Nrb TOP=7                       (R28[7:4]=4'b1000)
            R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
        }
        else
        {
            R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
            R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
            R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
            R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
            R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
            R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
            R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
        }
        R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
        R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
        R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)

    }
    if((RF_freq>=350000) && (RF_freq<=540000))
    {
    	R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
    }
    break;

    case R840_DVB_T2_6M:
    case R840_DVB_T2_8M:
    case R840_DVB_T2_1_7M:
    case R840_DVB_T2_10M:
      case R840_DVB_T2_6M_IF_5M:
    case R840_DVB_T2_8M_IF_5M:
    case R840_DVB_T2_1_7M_IF_5M:

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
#if(DTV_LTE_ONE_EQ==TRUE)
    if((RF_freq>=782000) && (RF_freq<=790000))
    {
        R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
        R840_SysFreq_Info.NRB_TOP=0xB0;            // Nrb TOP=4                       (R28[7:4]=4'b1011)
        R840_SysFreq_Info.NRB_TOP_ADDER=0;    //Nrb Adder: normal
    }
    else
#endif
    {
        R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
        R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
        R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
    }
    R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
    R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
    R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
    R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
    R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
    R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
    R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
   }
   else
   {
       if((RF_freq>=782000) && (RF_freq<=790000))
       {
           R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
           R840_SysFreq_Info.LNA_VTH_L=0x52;		   // LNA VTH/L=0.84/0.54     (R13=0x52)
           R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
           R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
           R840_SysFreq_Info.RF_TOP=0x00;               // RF TOP=7                        (R26[7:5]=3'b000)
           R840_SysFreq_Info.NRB_TOP=0x80;            // Nrb TOP=7                       (R28[7:4]=4'b1000)
           R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
       }
       else
       {
           R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
           R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
           R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
           R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
           R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
           R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
           R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
       }
       R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
       R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
       R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)

   }

    if((RF_freq>=350000) && (RF_freq<=540000))
    {
        R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
    }
    break;

    case R840_DVB_T_7M:
    case R840_DVB_T_7M_IF_5M:
    case R840_DVB_T2_7M:
    case R840_DVB_T2_7M_IF_5M:
    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
#if(DTV_VHF_ACI2_ENHANCE==TRUE)
          R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
          R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
          R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
          R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
          R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
          R840_SysFreq_Info.NRB_TOP=0x30;            // Nrb TOP=12                       (R28[7:4]=4'b0011)
          R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
          R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
          R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
          R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
#else
          R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
          R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
          R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
          R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
          R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
          R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
          R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
          R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
          R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
          R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
#endif
       }
       else if(*(pR840_Chip + u8TunerIndex)==R836)
       {
#if(DTV_VHF_ACI2_ENHANCE==TRUE)
            R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
            R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
            R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
            R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
            R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
            R840_SysFreq_Info.NRB_TOP=0x00;            // Nrb TOP=15                       (R28[7:4]=4'b0000)
            R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
            R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
            R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
            R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
#else
            R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
            R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
            R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
            R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
            R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
            R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
            R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
            R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
            R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
            R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)

            if((RF_freq>=164000) && (RF_freq<=216000))
            {
               R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
               R840_SysFreq_Info.NRB_TOP=0x20;         // Nrb TOP=13                     (R28[7:4]=4'b0010)
               R840_SysFreq_Info.MIXER_TOP=0x04;	   // MIXER TOP=11               (R28[3:0]=4'b0100)
            }
#endif
        }
        break;

    case R840_DVB_C_8M:
    case R840_DVB_C_6M:
    case R840_J83B:
    case R840_DVB_C_8M_IF_5M:
    case R840_DVB_C_6M_IF_5M:
    case R840_J83B_IF_5M:
        if(RF_freq<165000)
        {
            R840_SysFreq_Info.LNA_TOP=0x02;            // LNA TOP=5                    (R27[2:0]=3'b010)
            R840_SysFreq_Info.LNA_VTH_L=0x94;          // LNA VTH/L=1.24/0.74      (R13=0x94)
            R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
            R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
        }
        else if((RF_freq>=165000) && (RF_freq<350000))
        {
            R840_SysFreq_Info.LNA_TOP=0x03;            // LNA TOP=4                    (R27[2:0]=3'b011)
            R840_SysFreq_Info.LNA_VTH_L=0x94;          // LNA VTH/L=1.24/0.74     (R13=0x94)
            R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
            R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
        }
        else if((RF_freq>=350000) && (RF_freq<540000))
        {
            R840_SysFreq_Info.LNA_TOP=0x04;		       // LNA TOP=3                    (R27[2:0]=3'b100)
            R840_SysFreq_Info.LNA_VTH_L=0x93;	       // LNA VTH/L=1.24/0.64     (R13=0x93)
            R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
            R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
        }
        else
        {
            R840_SysFreq_Info.LNA_TOP=0x04;            // LNA TOP=3                    (R27[2:0]=3'b100)
            R840_SysFreq_Info.LNA_VTH_L=0x83;          // LNA VTH/L=1.14/0.64     (R13=0x83)
            R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
            R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
        }

        R840_SysFreq_Info.MIXER_TOP=0x05;          // MIXER TOP=10               (R28[3:0]=4'b0100)
        R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
        R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
        R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
        R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
        R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
        break;

    case R840_ISDB_T_4063:
    case R840_ISDB_T_4570:
    case R840_ISDB_T_IF_5M:

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
        if((RF_freq>=350000) && (RF_freq<=540000))
        {
            R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
        }
        else
        {
            R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
        }
        R840_SysFreq_Info.LNA_TOP=0x03;             // LNA TOP=4                    (R27[2:0]=3'b011)
        R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
        R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
        R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
        R840_SysFreq_Info.NRB_TOP=0x80;            // Nrb TOP=7                       (R28[7:4]=4'b1000)
        R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
        R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
        R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
        R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
    }
    else
    {
        if((RF_freq>=690000) && (RF_freq<=700000))
        {
            R840_SysFreq_Info.LNA_TOP=0x02;            // LNA TOP=5                    (R27[2:0]=3'b010)
            R840_SysFreq_Info.LNA_VTH_L=0x62;          // LNA VTH/L=0.94/0.54     (R13=0x62)
            R840_SysFreq_Info.MIXER_TOP=0x05;          // MIXER TOP=10               (R28[3:0]=4'b0100)
            R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
            R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
            R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
            R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
        }
        else
        {
            R840_SysFreq_Info.LNA_TOP=0x03;            // LNA TOP=4                    (R27[2:0]=3'b011)
            R840_SysFreq_Info.LNA_VTH_L=0xA5;      // LNA VTH/L=1.34/0.84     (R13=0xA5)
            R840_SysFreq_Info.MIXER_TOP=0x05;          // MIXER TOP=10               (R28[3:0]=4'b0100)
            R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
            R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
            R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
            R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
        }
		R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
		R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
		R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)

	}
        break;

    case R840_DTMB_8M_4570:
    case R840_DTMB_6M_4500:
    case R840_DTMB_8M_IF_5M:
    case R840_DTMB_6M_IF_5M:
            R840_SysFreq_Info.LNA_TOP=0x03;            // LNA TOP=4                    (R27[2:0]=3'b011)
            R840_SysFreq_Info.LNA_VTH_L=0xA5;      // LNA VTH/L=1.34/0.84     (R13=0xA5)
            R840_SysFreq_Info.MIXER_TOP=0x05;          // MIXER TOP=10               (R28[3:0]=4'b0100)
            R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
            R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
            R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
            R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
        break;

    case R840_ATSC:
    case R840_ATSC_IF_5M:
       if(R840_DetectTfType==R840_UL_USING_BEAD)
       {
           if(RF_freq<88000)
           {
               R840_SysFreq_Info.LNA_TOP=0x03;             // LNA TOP=4                    (R27[2:0]=3'b011)
               R840_SysFreq_Info.LNA_VTH_L=0xA5;           // LNA VTH/L=1.34/0.84     (R13=0xA5)
               R840_SysFreq_Info.RF_TOP=0xC0;               // RF TOP=1                        (R26[7:5]=3'b110)
               R840_SysFreq_Info.NRB_TOP=0x30;             // Nrb TOP=12                    (R28[7:4]=4'b0011)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=88000) && (RF_freq<104000))
           {
               R840_SysFreq_Info.LNA_TOP=0x02;             // LNA TOP=5                    (R27[2:0]=3'b010)
               R840_SysFreq_Info.LNA_VTH_L=0xA5;           // LNA VTH/L=1.34/0.84     (R13=0xA5)
               R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
               R840_SysFreq_Info.NRB_TOP=0x30;             // Nrb TOP=12                    (R28[7:4]=4'b0011)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=104000) && (RF_freq<156000))
           {
               R840_SysFreq_Info.LNA_TOP=0x01;             // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0xA5;           // LNA VTH/L=1.34/0.84     (R13=0xA5)
               R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
               R840_SysFreq_Info.NRB_TOP=0x30;             // Nrb TOP=12                    (R28[7:4]=4'b0011)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=156000) && (RF_freq<464000))
           {
               R840_SysFreq_Info.LNA_TOP=0x01;             // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0xA5;           // LNA VTH/L=1.34/0.84     (R13=0xA5)
               R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
               R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=464000) && (RF_freq<500000))
           {
               R840_SysFreq_Info.LNA_TOP=0x01;             // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0xB6;           // LNA VTH/L=1.44/0.94     (R13=0xB6)
               R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
               R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=685000) && (RF_freq<700000))
           {
              if(*(pR840_Chip + u8TunerIndex)==R836_R)
              {
            	   R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
               R840_SysFreq_Info.LNA_VTH_L=0x93;	       // LNA VTH/L=1.24/0.64     (R13=0x93)
               R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
               R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
               R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
              }
              else
              {
                   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)
               R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
               R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
               R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
              }
           }
           else if((RF_freq>=795000) && (RF_freq<805000))
           {
               if(*(pR840_Chip + u8TunerIndex)==R836)
               {
                 R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
                 R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)
                 R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
                 R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
                 R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
                 R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
               }
               else
              {
                 R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
                 R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
                 R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
                 R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
                 R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
                 R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
               }
           }
           else
           {
                R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
                R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
                R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
                R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
                R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
                R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
         }
         else  //270n
         {
            if(RF_freq<88000)
           {
               R840_SysFreq_Info.LNA_TOP=0x02;	 	       // LNA TOP=5                    (R27[2:0]=3'b010)
               R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
               R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
               R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                    (R28[7:4]=4'b1100)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=88000) && (RF_freq<104000))
           {
               R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
               R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
               R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
               R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
          }
          else if((RF_freq>=104000) && (RF_freq<248000))
          {
               R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
               R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
               R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=248000) && (RF_freq<464000))
           {
              R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
              R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)
              R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
              R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
              R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
              R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=464000) && (RF_freq<500000))
           {
             R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
             R840_SysFreq_Info.LNA_VTH_L=0xB6;	       // LNA VTH/L=1.44/0.94     (R13=0xB6)
             R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
             R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
             R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
             R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
           }
           else if((RF_freq>=685000) && (RF_freq<700000))
           {
           if(*(pR840_Chip + u8TunerIndex)==R836_R)
           {
             R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
             R840_SysFreq_Info.LNA_VTH_L=0x93;	       // LNA VTH/L=1.24/0.64     (R13=0x93)
             R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
             R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
             R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
             R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
           }
           else
           {
                 R840_SysFreq_Info.LNA_TOP=0x01;              // LNA TOP=6                    (R27[2:0]=3'b001)
             R840_SysFreq_Info.LNA_VTH_L=0x62;           // LNA VTH/L=0.94/0.54     (R13=0x62)
             R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
             R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
             R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
             R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
           }
           }
           else if((RF_freq>=795000) && (RF_freq<805000))
           {
          if(*(pR840_Chip + u8TunerIndex)==R836)
          {
               R840_SysFreq_Info.LNA_TOP=0x01;           // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0x62;           // LNA VTH/L=0.94/0.54     (R13=0x62)
               R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
               R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
               R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
          }
          else
         {
               R840_SysFreq_Info.LNA_TOP=0x01;            // LNA TOP=6                    (R27[2:0]=3'b001)
               R840_SysFreq_Info.LNA_VTH_L=0x94;          // LNA VTH/L=1.24/0.74     (R13=0x94)
               R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
               R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
               R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
               R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
         }
       }
       else
       {
              R840_SysFreq_Info.LNA_TOP=0x01;          // LNA TOP=6                    (R27[2:0]=3'b001)
              R840_SysFreq_Info.LNA_VTH_L=0x94;        // LNA VTH/L=1.24/0.74     (R13=0x94)
              R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
              R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
              R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
              R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
       }

     }

             R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
             R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
             R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
             R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
//			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
    break;

    case R840_FM:
             R840_SysFreq_Info.LNA_TOP=0x03;            // LNA TOP=4                    (R27[2:0]=3'b011)
             R840_SysFreq_Info.LNA_VTH_L=0xA4;      // LNA VTH/L=1.34/0.74     (R13=0xA4)
             R840_SysFreq_Info.MIXER_TOP=0x05;          // MIXER TOP=10               (R28[3:0]=4'b0100)
             R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
             R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
             R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
             R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
             R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
             R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
             R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
             break;

   default: //DVB-T 8M
             R840_SysFreq_Info.LNA_TOP=0x03;         // LNA TOP=4                    (R27[2:0]=3'b011)
             R840_SysFreq_Info.LNA_VTH_L=0xA4;    // LNA VTH/L=1.34/0.74     (R13=0xA4)
             R840_SysFreq_Info.MIXER_TOP=0x05;        // MIXER TOP=10               (R28[3:0]=4'b0100)
             R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
             R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
             R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
             R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
             R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
             R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
             R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
   break;

    } //end switch


    //TF LPF setting
    switch(R840_Standard)
    {
        case R840_DTMB_8M_4570:
        case R840_DTMB_6M_4500:
        case R840_DTMB_8M_IF_5M:
        case R840_DTMB_6M_IF_5M:
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

	R840_SysFreq_Info.TEMP = 0;

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
       //reset filter cal. data
       for (i=0; i<R840_STD_SIZE; i++)
       {
           *(pR840_Fil_Cal_flag + u8TunerIndex *R840_STD_SIZE +i) = FALSE;
           *(pR840_Fil_Cal_code + u8TunerIndex *R840_STD_SIZE +i) = 0;
           *(pR840_Fil_Cal_BW + u8TunerIndex *R840_STD_SIZE +i) = 0x00;
           *(pR840_Fil_Cal_LpfLsb + u8TunerIndex *R840_STD_SIZE +i) = 0x00;
       }


        *(pR840_IMR_Cal_Result + u8TunerIndex) = 0;
        *(pR840_TF_Check_Result + u8TunerIndex) = 0;
       if(*(pR840_IMR_done_flag + u8TunerIndex)==FALSE)
       {
        //do UL TF check
              if(R840_InitReg(u8TunerIndex) != RT_Success)
                  return RT_Fail;

              if(R840_TF_Check(u8TunerIndex) != RT_Success)
                 return RT_Fail;

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

          //do Xtal check
#if(R840_SHARE_XTAL_OUT==TRUE)
          *(pR840_Xtal_Pwr + u8TunerIndex) = XTAL_GM3V_STRONG;
#else
          if(R840_InitReg(u8TunerIndex) != RT_Success)
             return RT_Fail;

          if(R840_Xtal_Check(u8TunerIndex) != RT_Success)
             return RT_Fail;

         if(*(pR840_Xtal_Pwr_tmp + u8TunerIndex)==XTAL_GM3V_STRONG)
             *(pR840_Xtal_Pwr + u8TunerIndex) = XTAL_GM3V_STRONG;
         else
            *(pR840_Xtal_Pwr + u8TunerIndex) = *(pR840_Xtal_Pwr_tmp + u8TunerIndex)+1;
#endif

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

#if(R840_INIT_SMALL_XTAL_CAP==TRUE)
    R840_iniArray[19] = (R840_iniArray[19] & 0xDF) | 0x20;
#else
    R840_iniArray[19] = (R840_iniArray[19] & 0xDF) | 0x00;
#endif

#if(R840_INIT_LT_ON==TRUE)
    R840_iniArray[23] = (R840_iniArray[23] & 0xDF) | 0x00;
#else
    R840_iniArray[23] = (R840_iniArray[23] & 0xDF) | 0x20;
#endif

#if(R840_Xtal==16000)
    R840_iniArray[20] = 0xD5;  //800M
#else
    R840_iniArray[20] = 0x4D;  //792M
#endif

    for(InitArrayCunt = 0; InitArrayCunt<R840_REG_NUM; InitArrayCunt ++)
    {
        R840_I2C_Len.Data[InitArrayCunt] = R840_iniArray[InitArrayCunt];
        R840_SET_REG_Array(u8TunerIndex, InitArrayCunt, 0x00,R840_iniArray[InitArrayCunt]);
    }
    if(I2C_Write_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
        return RT_Fail;

    //poly on, R9[7]=0
    R840_I2C.RegAddr = 0x09;
    R840_SET_REG_Array(u8TunerIndex, 0x09, 0x7F, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0x09);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //Mixer on, R4[3]=0
    R840_I2C.RegAddr = 0x04;
    R840_SET_REG_Array(u8TunerIndex, 0x04, 0xF7, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0x04);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //IQ gen on, R23[0]=0
    R840_I2C.RegAddr = 0x17;
    R840_SET_REG_Array(u8TunerIndex, 0x17, 0xFE, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0x17);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //PLL_LDO_D (LDO1=200R, LDO2=200R), R23[7:6]=10; R23[4:3]=10
    R840_I2C.RegAddr = 0x17;
    R840_SET_REG_Array(u8TunerIndex, 0x17, 0x27, 0x90);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0x17);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //PLL_LDO_A=2.35V, R17[7:6]=00
    R840_I2C.RegAddr = 0x11;
    R840_SET_REG_Array(u8TunerIndex, 0x11, 0x3F, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0x11);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    return RT_Success;
}


R840_ErrCode R840_TF_Check(MS_U8 u8TunerIndex)
{
    UINT32  RingVCO = 0;
    UINT32  RingFreq = 72000;
    UINT32  RingRef = R840_Xtal;
    UINT8   divnum_ring = 0;
    UINT8   VGA_Count = 0;
    UINT8   VGA_Read = 0;

    MsOS_DelayTask(10);

//check status
    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = 4;
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
    {
       I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
    }

    if((R840_I2C_Len.Data[3] & 0xE0) == 0x60)
        *(pR840_Chip + u8TunerIndex) = R836;
    else if((R840_I2C_Len.Data[3] & 0xE0) == 0x40)
        *(pR840_Chip + u8TunerIndex) = R836_R;
    else
        *(pR840_Chip + u8TunerIndex) = R836;

    if(R840_Xtal==16000)  //16M
         divnum_ring = 11;
    else if(R840_Xtal==24000)//24M
         divnum_ring = 2;
    else if(R840_Xtal==27000)//27M
         divnum_ring = 0;
    else
         divnum_ring = 11;

     RingVCO = (16+divnum_ring)* 8 * RingRef;
     RingFreq = RingVCO/48;

     if(R840_Cal_Prepare(u8TunerIndex,R840_TF_LNA_CAL) != RT_Success)
          return RT_Fail;

     //Set Ring PLL, (R24, R25)
     R840_I2C.RegAddr = 0x18;
     R840_SET_REG_Array(u8TunerIndex, 24, 0x00 ,0x80|divnum_ring);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 24);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

     R840_SET_REG_Array(u8TunerIndex, 25, 0x00, 0x8B);//out div=8, RF poly=low band, power=min_lp
     if(RingVCO>=3200000)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xDF, 0x00); //vco_band=high, R25[5]=0
     else
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x20); //vco_band=low, R25[5]=1

     R840_I2C.RegAddr = 0x19;
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 25);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

     //Must do before PLL()
     if(R840_MUX(u8TunerIndex, RingFreq + 5000, RingFreq, R840_STD_SIZE) != RT_Success)     //FilCal MUX (LO_Freq, RF_Freq)
         return RT_Fail;

     //Set PLL
     if(R840_PLL(u8TunerIndex, (RingFreq + 5000), R840_STD_SIZE) != RT_Success)   //FilCal PLL
           return RT_Fail;

     //-----below must set after R840_MUX()-------//
     //Set LNA TF=(1,15), Qctrl=off for VGA training
     R840_I2C.RegAddr = 0x00;
     R840_SET_REG_Array(u8TunerIndex, 0, 0x00, 0x1F);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
          return RT_Fail;

     // FilterComp OFF
     R840_I2C.RegAddr = 0x0F;
     R840_SET_REG_Array(u8TunerIndex, 15, 0xFB, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
          return RT_Fail;

     // Byp-LPF: Bypass
     R840_I2C.RegAddr = 0x01;
     R840_SET_REG_Array(u8TunerIndex, 1, 0xFB, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 1);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
          return RT_Fail;

     //Adc=on; Vga code mode, Gain = -12dB
     R840_I2C.RegAddr = 0x0C;
     R840_SET_REG_Array(u8TunerIndex, 12, 0x00, 0x10);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 12);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
           return RT_Fail;

     // vcomp = 0
     R840_I2C.RegAddr = 0x1E;
     R840_SET_REG_Array(u8TunerIndex, 30, 0x9F, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 30);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

     //------- increase VGA power to let ADC read value significant ---------//
     for(VGA_Count=0; VGA_Count < 16; VGA_Count ++)
     {
        R840_I2C.RegAddr = 0x0C;
        R840_I2C.Data = (R840_GET_REG_Array(u8TunerIndex, 12) & 0xF0) + VGA_Count;
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        R840_Delay_MS(VGA_DELAY); //

        if(R840_Muti_Read(u8TunerIndex, &VGA_Read) != RT_Success)
            return RT_Fail;

        if(VGA_Read > 40*ADC_READ_COUNT)
            break;
     }

     //if VGA_Read<10 or >60, TF_Check_Fail
     if((VGA_Read<10*ADC_READ_COUNT) || (VGA_Read>60*ADC_READ_COUNT))
         (*(pR840_TF_Check_Result + u8TunerIndex)) = 1;  //fail
     else
         (*(pR840_TF_Check_Result + u8TunerIndex)) = 0;

     //Set LNA TF=(0,0)
     R840_I2C.RegAddr = 0x00;
     R840_SET_REG_Array(u8TunerIndex, 0, 0x00, 0x00);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 0);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
          return RT_Fail;

     R840_Delay_MS(10); //

     if(R840_Muti_Read(u8TunerIndex, &VGA_Read) != RT_Success)
          return RT_Fail;

     if(VGA_Read > (36*ADC_READ_COUNT))
        R840_DetectTfType = R840_UL_USING_BEAD;
     else
        R840_DetectTfType = R840_UL_USING_270NH;

    return RT_Success;
}

R840_ErrCode R840_Xtal_Check(MS_U8 u8TunerIndex)
{
    UINT8 i = 0;

    // TF force sharp mode (for stable read)
    R840_I2C.RegAddr = 0x01;
    R840_SET_REG_Array(u8TunerIndex, 1, 0xFF, 0x01);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 1);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    // NA det off (for stable read)
    R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xFF, 0x80);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 30);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    //Nint=57 (Si2c=0; Ni2c=11; Nint=Ni2c*4+Si2c+13)
    R840_I2C.RegAddr = 0x14;
    R840_SET_REG_Array(u8TunerIndex, 20, 0x00, 0x0B);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 20);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //Xtal_div=1 & Mixer_div=16
    R840_I2C.RegAddr = 0x10;
    R840_SET_REG_Array(u8TunerIndex, 16, 0x1B, 0x60);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //Xtal_pow1=low
    R840_I2C.RegAddr = 0x10;
    R840_SET_REG_Array(u8TunerIndex, 16, 0xFF, 0x10);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    //Xtal_pow0=highest
    R840_I2C.RegAddr = 0x0F;
    R840_SET_REG_Array(u8TunerIndex, 15, 0x9F, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
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

    for(i=0; i<XTAL_CHECK_SIZE; i++)  //from weak to strong
    {
        if(i==XTAL_GM3V_STRONG)  //
        {
            R840_SET_REG_Array(u8TunerIndex, 15, 0x9F, 0x00);
            R840_SET_REG_Array(u8TunerIndex, 16, 0xFF, 0x10);
        }
        else
        {
            R840_SET_REG_Array(u8TunerIndex, 15, 0x9F, ((UINT8)(XTAL_NOR_HIGHEST-i)<<5));
            R840_SET_REG_Array(u8TunerIndex, 16, 0xEF, 0x00);
        }

        R840_I2C.RegAddr = 0x0F;
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,15);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        R840_I2C.RegAddr = 0x10;
        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex, 16);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

        R840_Delay_MS(20);

        R840_I2C_Len.RegAddr = 0x00;
        R840_I2C_Len.Len = 3;
        if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success)
        {
            if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
               return RT_Fail;
         }

        //depend on init Nint & Div value (N=59.6667, Div=16)
        //lock to VCO band 8 if VCO=1845M for 16M Xtal
        //lock to VCO band 46 if VCO=2768M for 24M Xtal
        //lock to VCO band 53 if VCO=2919M for 27M Xtal
#if(R840_Xtal==16000)
        if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 14) && ((R840_I2C_Len.Data[2] & 0x3F) >= 2))
#elif (R840_Xtal==24000)
       if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 51) && ((R840_I2C_Len.Data[2] & 0x3F) >= 39))
#elif (R840_Xtal==27000)
       if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 59) && ((R840_I2C_Len.Data[2] & 0x3F) >= 47))
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
                Cal_Info.LNA_GAIN = 0x80;                  //manual: min(0)
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

	  // Set filter +0/6dB; NA det=OFF
    R840_I2C.RegAddr  = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, Cal_Info.FILTER_6DB | 0x80);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 30);
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
    R840_SET_REG_Array(u8TunerIndex, 1, 0xFF, 0x01);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 1);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    //RingPLL power ON
    R840_I2C.RegAddr = 0x18;
    R840_SET_REG_Array(u8TunerIndex, 24, 0xEF, 0);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 24);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

	 //LPF filter code = 8
    R840_I2C.RegAddr = 0x0A;
    R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, 0x08);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 10);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

     //HPF corner=1; LPF coarse=6M; 1.7M disable
    R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0x00, 0x61);
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

	UINT8  IMR_Gain = 0;
	UINT8  IMR_Phase = 0;

       R840_SectType IMR_POINT;

    R840_SET_REG_Array(u8TunerIndex, 24, 0x3F, 0x00);
    R840_SET_REG_Array(u8TunerIndex, 25, 0xFC, 0x00);
	if(R840_Xtal==16000)  //16M
         divnum_ring = 9;
	else if(R840_Xtal==24000)//24M
		 divnum_ring = 1;
	else if(R840_Xtal==27000)//27M
		 divnum_ring = 0;
	else
         divnum_ring = 9;

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
		RingFreq = RingVCO/4;
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x00);// ring_div1 /6 (2)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x00);// ring_div2 /1 (0)
		u1MixerGain = 8;
		break;
    default:
        RingFreq = RingVCO/4;
        R840_SET_REG_Array(u8TunerIndex, 24, 0xFF, 0x00);// ring_div1 /4 (0)
        R840_SET_REG_Array(u8TunerIndex, 25, 0xFF, 0x00);// ring_div2 /1 (0)
		u1MixerGain = 8;
        break;
    }

      //Mixer Amp Gain
      R840_I2C.RegAddr = 0x07;
      R840_SET_REG_Array(u8TunerIndex, 7, 0xE0, u1MixerGain);// ring_div1 /4 (0)
      R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 7);
      if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
          return RT_Fail;

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
           R840_SET_REG_Array(u8TunerIndex, 25, 0xF3, 0x00); //R25[3:2]=2'b00; min

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

    IMR_Gain = R840_IMR_Data[IMR_MEM].Gain_X & 0x2F;
    IMR_Phase = R840_IMR_Data[IMR_MEM].Phase_Y & 0x2F;

    if(((IMR_Gain & 0x0F)>6) || ((IMR_Phase & 0x0F)>6))
    {
        *(pR840_IMR_Cal_Result + u8TunerIndex) = 1; //fail
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
    UINT16 u2XalDivJudge;
    UINT8   u1XtalDivRemain;
    UINT8   VCO_current_trial = 0;
    UINT8   u1RfFlag = 0;
    UINT8   u1PulseFlag = 0;
    UINT8   CP_I2 = 0x00;
    UINT8   NS_RES = 0x00;
	UINT8   BoundDiv = 1;
	UINT8   CentBoundDiv = 1;
	UINT32  BoundFreq = 0;
	UINT32  CentBoundFreq = 0;

    //TF, NA fix
    u1RfFlag = (R840_GET_REG_Array(u8TunerIndex,1) & 0x01);         //R1[0]
    u1PulseFlag = (R840_GET_REG_Array(u8TunerIndex,30) & 0x80);   //R30[7]

    R840_I2C.RegAddr = 0x01;
    R840_SET_REG_Array(u8TunerIndex, 1, 0xFF, 0x01); // TF force sharp mode
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,1);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xFF, 0x80); // NA det off
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    //cp cur & offset setting
    if(R840_Standard < R840_ATV_SIZE) //ATV
    {
        //current
      if(LO_Freq < (48000+R840_IF_HIGH))
            CP_CUR = 0x10;        //0.5m, [5:3]=010
      else if(LO_Freq < (64000+R840_IF_HIGH))
            CP_CUR = 0x10;        //0.5m, [5:3]=010
      else if(LO_Freq < (80000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
      else if(LO_Freq < (96000+R840_IF_HIGH))
            CP_CUR = 0x28;        //0.2m, [5:3]=101
      else if(LO_Freq < (112000+R840_IF_HIGH))
            CP_CUR = 0x28;        //0.2m, [5:3]=101
      else if(LO_Freq < (128000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
      else if(LO_Freq < (176000+R840_IF_HIGH))
            CP_CUR = 0x18;        //0.4m, [5:3]=011
      else if(LO_Freq < (432000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
      else if(LO_Freq < (592000+R840_IF_HIGH))
            CP_CUR = 0x28;        //0.2m, [5:3]=101
      else if(LO_Freq < (744000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
      else if(LO_Freq < (752000+R840_IF_HIGH))
            CP_CUR = 0x20;        //0.3m, [5:3]=100
      else
            CP_CUR = 0x20;        //0.3m, [5:3]=100

//CP_i2
      if(LO_Freq < (48000+R840_IF_HIGH))
          CP_I2 = 0x04;            //x2
      else if(LO_Freq < (64000+R840_IF_HIGH))
          CP_I2 = 0x00;
      else if(LO_Freq < (96000+R840_IF_HIGH))
          CP_I2 = 0x04;           //x2
      else
          CP_I2 = 0x00;

          //offset
      if(LO_Freq < (88000+R840_IF_HIGH))
          CP_OFFSET = 0x04;  //30u,   [2]=1
      else if(LO_Freq < (96000+R840_IF_HIGH))
          CP_OFFSET = 0x00;  //0u,     [2]=0
      else if(LO_Freq < (160000+R840_IF_HIGH))
          CP_OFFSET = 0x04;  //30u,   [2]=1
      else if((LO_Freq < (240000+R840_IF_HIGH)) && (LO_Freq > (232000+R840_IF_HIGH)))
          CP_OFFSET = 0x04;  //30u,   [2]=1
      else if((LO_Freq < (272000+R840_IF_HIGH)) && (LO_Freq > (264000+R840_IF_HIGH)))
          CP_OFFSET = 0x04;  //30u,   [2]=1
      else
          CP_OFFSET = 0x00;  //0u,     [2]=0

      if((LO_Freq < (64000+R840_IF_HIGH)) && (LO_Freq > (48000+R840_IF_HIGH)))
          NS_RES = 0x02;        //100R
      else
          NS_RES = 0x00;        //short
    }
    else  //DTV
    {
       if(LO_Freq < (96000+R840_IF_HIGH))
           CP_CUR = 0x28;        //0.2m, [5:3]=101
       else
           CP_CUR = 0x00;        //0.7m, [5:3]=000

       if((LO_Freq < (80000+R840_IF_HIGH)) && (LO_Freq > (72000+R840_IF_HIGH)))
          CP_OFFSET = 0x04;  //30u,   [2]=1
       else
          CP_OFFSET = 0x00;  //0u,     [2]=0

       CP_I2 = 0x00;
       NS_RES = 0x00;        //short
    }

    //CP ix2 & Ns_RES
     R840_I2C.RegAddr = 0x13;
     R840_SET_REG_Array(u8TunerIndex, 19, 0xF9, CP_I2 | NS_RES);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 19);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;

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
#if(R840_SHARE_XTAL_OUT==TRUE)
     XTAL_POW1 = 0x10;        //low,       R16[4]=1
     XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
#else
    if(*(pR840_Initial_done_flag + u8TunerIndex)==TRUE)
    {
        if(*(pR840_Xtal_Pwr +u8TunerIndex)<XTAL_NOR_HIGH)
        {
            XTAL_POW1 = 0x00;        //high,       R16[4]=0
            XTAL_POW0 = 0x20;        //high,       R15[6:5]=01
        }
        else if(*(pR840_Xtal_Pwr +u8TunerIndex) <= XTAL_NOR_HIGHEST)
        {
            XTAL_POW1 = 0x00;        //high,       R16[4]=0
            XTAL_POW0 = ((UINT8)(XTAL_NOR_HIGHEST-(*(pR840_Xtal_Pwr +u8TunerIndex)))<<5);
        }
        else
        {
            XTAL_POW1 = 0x10;        //low,       R16[4]=1
            XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
         }
    }
    else
    {
            XTAL_POW1 = 0x10;        //low,       R16[4]=1
            XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
    }
#endif
   //set POW1 first to prevent dramatical swing drop when Inti_Reg(low/highest) then SetPllData
    R840_I2C.RegAddr = 0x10;
    R840_SET_REG_Array(u8TunerIndex, 16, 0xEF, XTAL_POW1);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    R840_I2C.RegAddr = 0x0F;
    R840_SET_REG_Array(u8TunerIndex, 15, 0x9F, XTAL_POW0);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);
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
       SDM_RES = 0x10;   //200R, R19[4:3]=10
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
    #if(R840_Xtal==16000)
        u2XalDivJudge = (UINT16) (LO_Freq/1000/8);
    #elif(R840_Xtal==24000)
        u2XalDivJudge = (UINT16) (LO_Freq/1000/12);
    #elif(R840_Xtal==27000)
       u2XalDivJudge = (UINT16) (LO_Freq/13500);
    #endif
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
#if(R840_DTV_AGC_SLOW==TRUE)
		R840_XtalDiv = XTAL_DIV2;
	    R840_SET_REG_Array(u8TunerIndex, 16, 0xFF, 0x04);   //b2=1
	    PLL_Ref = R840_Xtal / 2;
#else
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
#endif
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

#if 1
	if(PLL_Ref==16000)
	{
		if(MixDiv==2)
		{
			BoundDiv = 4;
			CentBoundDiv = 2;
		}
		else if(MixDiv==4)
		{
			BoundDiv = 2;
			CentBoundDiv = 1;
		}
		else
		{
			BoundDiv = 1;
			CentBoundDiv = 1;
		}
	}
	else if(PLL_Ref==8000)
	{
		if(MixDiv==2)
		{
			BoundDiv = 2;
			CentBoundDiv = 1;
		}
		else if(MixDiv==4)
		{
			BoundDiv = 1;
			CentBoundDiv = 1;
		}
		else
		{
			BoundDiv = 1;
			CentBoundDiv = 1;
		}
	}
	else if((PLL_Ref==24000) || (PLL_Ref==27000))
	{
		if(MixDiv==2)
		{
			BoundDiv = 6;
			CentBoundDiv = 3;
		}
		else if(MixDiv==4)
		{
			BoundDiv = 3;
			CentBoundDiv = 2;
		}
		else if(MixDiv==8)
		{
			BoundDiv = 2;
			CentBoundDiv = 1;
		}
		else
		{
			BoundDiv = 1;
			CentBoundDiv = 1;
		}
	}
	else if((PLL_Ref==12000) || (PLL_Ref==13500))
	{
		if(MixDiv==2)
		{
			BoundDiv = 3;
			CentBoundDiv = 2;
		}
		else if(MixDiv==4)
		{
			BoundDiv = 2;
			CentBoundDiv = 1;
		}
		else if(MixDiv==8)
		{
			BoundDiv = 1;
			CentBoundDiv = 1;
		}
		else
		{
			BoundDiv = 1;
			CentBoundDiv = 1;
		}
	}
	else
	{
		BoundDiv = 1;
		CentBoundDiv = 1;
	}


	BoundFreq = PLL_Ref/64/BoundDiv;             //2*PLL_Ref/128/BoundDiv
	CentBoundFreq = PLL_Ref/128/CentBoundDiv;    //2*PLL_Ref/128/CentBoundDiv*0.5

	//CP current to lower spur
	if((VCO_Fra>=BoundFreq) && (VCO_Fra<(BoundFreq*2)))
		CP_CUR = 0x28;        //0.2m, [5:3]=101
	else if((VCO_Fra<=(2*PLL_Ref - BoundFreq)) && (VCO_Fra>(2*PLL_Ref - BoundFreq/2)))
		CP_CUR = 0x28;        //0.2m, [5:3]=101
	else
		CP_CUR = CP_CUR;

	//CP current
	R840_I2C.RegAddr = 0x11;
	R840_SET_REG_Array(u8TunerIndex, 17, 0xC7, CP_CUR);  // [5:3]=CP_CUR
	R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 17);
	if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
	    return RT_Fail;
#endif
    //Boundary spur prevention
	if (VCO_Fra < BoundFreq)  //2*PLL_Ref/128/BoundDiv
	{
		VCO_Fra = 0;
	}
	else if (VCO_Fra > (2*PLL_Ref - BoundFreq))  //2*PLL_Ref*(1-1/128/BoundDiv)
	{
		VCO_Fra = 0;
		Nint ++;
	}
	else if((VCO_Fra > (PLL_Ref - CentBoundFreq)) && (VCO_Fra < PLL_Ref)) //> 2*PLL_Ref*(1-1/128/CentBoundDiv)*0.5,  < 2*PLL_Ref*128/256
	{
		VCO_Fra = (PLL_Ref - CentBoundFreq);      // VCO_Fra = 2*PLL_Ref*(1-1/128/CentBoundDiv)*0.5
	}
	else if((VCO_Fra > PLL_Ref) && (VCO_Fra < (PLL_Ref + CentBoundFreq))) //> 2*PLL_Ref*128/256,  < 2*PLL_Ref*(1+1/128/CentBoundDiv)*0.5
	{
		VCO_Fra = (PLL_Ref + CentBoundFreq);      // VCO_Fra = 2*PLL_Ref*(1+1/128/CentBoundDiv)*0.5
	}
	else
	{
		VCO_Fra = VCO_Fra;
	}

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
        {
            I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
        }

        if( (R840_I2C_Len.Data[2] & 0x40) == 0x00 )
        {
            R840_I2C.RegAddr = 0x12;
            R840_SET_REG_Array(u8TunerIndex, 18, 0x1F, ((2-VCO_current_trial) << 5));
            R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 18);
            if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                 return RT_Fail;

			R840_Delay_MS(10);
		}
        else
        {
            break;
        }
    }

    if(VCO_current_trial>=2)
    {
		R840_Delay_MS(10);

     //check PLL lock status
         R840_I2C_Len.RegAddr = 0x00;
         R840_I2C_Len.Len = 3;
         if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success)
         {
             I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
         }

         if( (R840_I2C_Len.Data[2] & 0x40) == 0x00)
         {
             XTAL_POW1 = 0x10;        //low,       R16[4]=1
             XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00

             R840_I2C.RegAddr = 0x0F;
             R840_SET_REG_Array(u8TunerIndex, 15, 0x9F, XTAL_POW0);
             R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);
             if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                 return RT_Fail;
             R840_I2C.RegAddr = 0x10;
             R840_SET_REG_Array(u8TunerIndex, 16, 0xEF, XTAL_POW1);
             R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 16);
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

    //restore TF, NA det setting
    R840_I2C.RegAddr = 0x01;
    R840_SET_REG_Array(u8TunerIndex, 1, 0xFE, u1RfFlag);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 1);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0x7F, u1PulseFlag);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 30);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

     return RT_Success;

}


R840_ErrCode R840_MUX(MS_U8 u8TunerIndex,UINT32 LO_KHz, UINT32 RF_KHz, R840_Standard_Type R840_Standard)
{

    UINT8 Reg08_IMR_Gain   = 0;
    UINT8 Reg09_IMR_Phase  = 0;
    UINT8 Reg03_IMR_Iqcap  = 0;
    Freq_Info_Type* pFreq_Info;
    //Freq_Info_Type Freq_Info1;
    *(pFreq_Info1 + u8TunerIndex) = R840_Freq_Sel(LO_KHz, RF_KHz, R840_Standard);
    pFreq_Info = pFreq_Info1 + u8TunerIndex;

    // LNA band (depend on RF_KHz)
    R840_I2C.RegAddr = 0x05;
      R840_SET_REG_Array(u8TunerIndex, 5, 0x9F, pFreq_Info->LNA_BAND);
      R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 5);

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

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
       // LNA Notch
       R840_I2C.RegAddr = 0x02;
       R840_SET_REG_Array(u8TunerIndex, 2, 0xF0, pFreq_Info->LPF_NOTCH);
       R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 2);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;

       //Set_IMR
       if(*(pR840_IMR_done_flag + u8TunerIndex) == TRUE)
       {
           Reg08_IMR_Gain = R840_IMR_Data[pFreq_Info->IMR_MEM].Gain_X & 0x2F;
           Reg09_IMR_Phase = R840_IMR_Data[pFreq_Info->IMR_MEM].Phase_Y & 0x2F;
           Reg03_IMR_Iqcap = R840_IMR_Data[pFreq_Info->IMR_MEM].Iqcap & 0x03;

            if((*(pR840_IMR_Cal_Result + u8TunerIndex))==1)
            {
                Reg08_IMR_Gain = 0;
                Reg09_IMR_Phase = 0;
                Reg03_IMR_Iqcap = 0;
            }
       }
       else
       {
           Reg08_IMR_Gain = 0;
           Reg09_IMR_Phase = 0;
           Reg03_IMR_Iqcap = 0;
       }

       R840_I2C.RegAddr = 0x08;
       R840_SET_REG_Array(u8TunerIndex, 8, 0xD0, Reg08_IMR_Gain);
       R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 8);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

       R840_I2C.RegAddr = 0x09;
       R840_SET_REG_Array(u8TunerIndex, 9, 0xD0, Reg09_IMR_Phase);
       R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 9);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;
    }
    else
    {
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

        if((*(pR840_IMR_Cal_Result + u8TunerIndex))==1)
        {
            Reg08_IMR_Gain = 0;
            Reg09_IMR_Phase = 0;
            Reg03_IMR_Iqcap = 0;
        }

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
    }
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
    //UINT8    u1TFCalNum = R840_TF_HIGH_NUM;

	if((u4FreqKHz>0) && (u4FreqKHz<R840_LNA_LOW_LOWEST[R840_SetTfType]))  //Ultra Low
    {
        //u1TFCalNum = R840_TF_LOWEST_NUM;
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
         //u1TFCalNum = R840_TF_LOW_NUM;
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
        //u1TFCalNum = R840_TF_HIGH_NUM;
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
    UINT8   X_Direction;  // 1:X, 0:Y

    R840_I2C.RegAddr = 0x0C;
    R840_SET_REG_Array(u8TunerIndex, 12, 0xF0, 0x0F);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 12);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;


    Compare_IQ[0].Gain_X  = R840_GET_REG_Array(u8TunerIndex, 8) & 0xC0;
    Compare_IQ[0].Phase_Y = R840_GET_REG_Array(u8TunerIndex, 9) & 0xC0;

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
        if(R840_IQ_Tree5(u8TunerIndex, Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
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

      if(R840_IQ_Tree5(u8TunerIndex, Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
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

R840_ErrCode R840_IQ_Tree5(MS_U8 u8TunerIndex, UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree)
{
    UINT8 TreeCunt  = 0;
    UINT8 TreeTimes = 5;
    UINT8 TempPot   = 0;
    UINT8 PntReg    = 0;
    UINT8 CompCunt = 0;
    R840_SectType CorTemp[5];
    R840_SectType Compare_Temp;
    UINT8 CuntTemp = 0;

    //memset(&Compare_Temp,0, sizeof(R840_SectType));
    Compare_Temp.Gain_X=0;
    Compare_Temp.Phase_Y=0;
    Compare_Temp.Iqcap=0;
    Compare_Temp.Value = 255;

    for(CompCunt=0; CompCunt<3; CompCunt++)
    {
       CorTemp[CompCunt].Gain_X = CompareTree[CompCunt].Gain_X;
       CorTemp[CompCunt].Phase_Y = CompareTree[CompCunt].Phase_Y;
       CorTemp[CompCunt].Value = CompareTree[CompCunt].Value;
    }

    if(PotReg == 0x08)
        PntReg = 0x09; //phase control
    else
        PntReg = 0x08; //gain control

    for(TreeCunt = 0;TreeCunt < TreeTimes;TreeCunt ++)
    {
        R840_I2C.RegAddr = PotReg;
        R840_I2C.Data    = FixPot;
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        R840_I2C.RegAddr = PntReg;
        R840_I2C.Data    = FlucPot;
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        if(R840_Muti_Read(u8TunerIndex, &CorTemp[TreeCunt].Value) != RT_Success)
            return RT_Fail;

        if(PotReg == 0x08)
        {
            CorTemp[TreeCunt].Gain_X  = FixPot;
            CorTemp[TreeCunt].Phase_Y = FlucPot;
        }
        else
        {
            CorTemp[TreeCunt].Phase_Y  = FixPot;
            CorTemp[TreeCunt].Gain_X = FlucPot;
        }

        if(TreeCunt == 0)   //next try right-side 1 point
            FlucPot ++;     //+1
        else if(TreeCunt == 1)   //next try right-side 2 point
            FlucPot ++;     //1+1=2
        else if(TreeCunt == 2)   //next try left-side 1 point
        {
            if((FlucPot & 0x1F) == 0x02) //if absolute location is 2, change I/Q direction and set to 1
            {
                TempPot = 1;
                if((FlucPot & 0x20)==0x20) //b[5]:I/Q selection. 0:Q-path, 1:I-path
                {
                    FlucPot = (FlucPot & 0xC0) | 0x01;  //Q1
                }
                else
                {
                    FlucPot = (FlucPot & 0xC0) | 0x21;  //I1
                }
            }
            else
            	FlucPot -= 3;  //+2-3=-1
        }
        else if(TreeCunt == 3) //next try left-side 2 point
        {
            if(TempPot==1)  //been chnaged I/Q
            {
                FlucPot += 1;
            }
            else if((FlucPot & 0x1F) == 0x00) //if absolute location is 0, change I/Q direction
            {
                TempPot = 1;
                if((FlucPot & 0x20)==0x20) //b[5]:I/Q selection. 0:Q-path, 1:I-path
                {
                    FlucPot = (FlucPot & 0xC0) | 0x01;  //Q1
                }
                else
                {
                    FlucPot = (FlucPot & 0xC0) | 0x21;  //I1
                }
            }
            else
                FlucPot -= 1;  //-1-1=-2
        }

        if(CorTemp[TreeCunt].Value < Compare_Temp.Value)
        {
          Compare_Temp.Value = CorTemp[TreeCunt].Value;
          Compare_Temp.Gain_X = CorTemp[TreeCunt].Gain_X;
          Compare_Temp.Phase_Y = CorTemp[TreeCunt].Phase_Y;
          CuntTemp = TreeCunt;
        }
    }



    for(CompCunt=0; CompCunt<3; CompCunt++)
    {
        if(CuntTemp==3 || CuntTemp==4)
        {
            CompareTree[CompCunt].Gain_X = CorTemp[2+CompCunt].Gain_X;  //2,3,4
            CompareTree[CompCunt].Phase_Y = CorTemp[2+CompCunt].Phase_Y;
            CompareTree[CompCunt].Value = CorTemp[2+CompCunt].Value;
        }
        else
        {
            CompareTree[CompCunt].Gain_X = CorTemp[CompCunt].Gain_X;    //0,1,2
            CompareTree[CompCunt].Phase_Y = CorTemp[CompCunt].Phase_Y;
            CompareTree[CompCunt].Value = CorTemp[CompCunt].Value;
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
#if (ADC_MULTI_READ==3)
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
        {
            I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
        }

        ReadData = (R840_I2C_Len.Data[1] & 0x3F);

        ReadAmount = ReadAmount + (UINT16)ReadData;

        if(ReadData < ReadMin)
            ReadMin = ReadData;

        if(ReadData > ReadMax)
            ReadMax = ReadData;
    }
    *IMR_Result_Data = (UINT8) (ReadAmount - (UINT16)ReadMax - (UINT16)ReadMin);
#else
    R840_Delay_MS(ADC_READ_DELAY);

    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = 2;              // read 0x01
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
    {
        I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
    }
    *IMR_Result_Data = (R840_I2C_Len.Data[1] & 0x3F);
#endif

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

    //memset(&Compare_Temp,0, sizeof(R840_SectType));
    Compare_Temp.Gain_X=0;
    Compare_Temp.Phase_Y=0;
    Compare_Temp.Iqcap=0;
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


    R840_I2C.RegAddr = 0x0C;
    R840_SET_REG_Array(u8TunerIndex, 12, 0xF0, 0x0F);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 12);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

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
        R840_SET_REG_Array(u8TunerIndex, 15, 0xFF, 0x01);
    else
        R840_SET_REG_Array(u8TunerIndex, 15, 0xFE, 0x00);

    R840_I2C.RegAddr = 0x0F;
      R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 15);

    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
    	return RT_Fail;

    return RT_Success;
}


R840_ErrCode R840_SetStandard(MS_U8 u8TunerIndex,R840_Standard_Type RT_Standard)
{
    UINT8 u1FilCalGap = 8;
    Sys_Info_Type* pSysInfo;

    *(pSys_Info1 + u8TunerIndex) = R840_Sys_Sel(u8TunerIndex, RT_Standard);
    pSysInfo = pSys_Info1 + u8TunerIndex;

    // Filter Calibration
    if(RT_Standard<R840_ATV_SIZE)    //ATV
        u1FilCalGap = R840_Fil_Cal_Gap;
    else
        u1FilCalGap = 8;   //? 13

    if(*(pR840_Fil_Cal_flag + u8TunerIndex*R840_STD_SIZE + RT_Standard) == FALSE)
    {
        *(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard) = R840_Filt_Cal_ADC( u8TunerIndex,pSysInfo->FILT_CAL_IF, pSysInfo->BW, u1FilCalGap);
        *(pR840_Fil_Cal_BW + u8TunerIndex*R840_STD_SIZE + RT_Standard) = R840_Bandwidth;
        *(pR840_Fil_Cal_flag + u8TunerIndex*R840_STD_SIZE + RT_Standard) = TRUE;
        *(pR840_Fil_Cal_LpfLsb + u8TunerIndex *R840_STD_SIZE +RT_Standard) = R840_Lpf_Lsb;  //respin

        //protection
        if((*(pR840_IMR_Cal_Result + u8TunerIndex))==1) //fail
        {
            if(((*(pR840_Fil_Cal_BW + u8TunerIndex*R840_STD_SIZE + RT_Standard)) == 0x60) &&\
                ((*(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard)) == 15))  //6M/15
            {
                *(pR840_Fil_Cal_BW + u8TunerIndex*R840_STD_SIZE + RT_Standard) = R840_Fil_Cal_BW_def[RT_Standard];
                *(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + RT_Standard) = R840_Fil_Cal_code_def[RT_Standard];
                *(pR840_Fil_Cal_LpfLsb + u8TunerIndex *R840_STD_SIZE +RT_Standard) = 0;
            }
        }

        //Reset register and Array
        if(R840_InitReg(u8TunerIndex) != RT_Success)
            return RT_Fail;
    }

    if(RT_Standard != *(pR840_pre_standard + u8TunerIndex))
    {
        if(R840_InitReg(u8TunerIndex) != RT_Success)
             return RT_Fail;
        // Set TF current
        R840_I2C.RegAddr = 0x03;
        R840_SET_REG_Array(u8TunerIndex, 3, 0xBF, pSysInfo->TF_CUR);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 3);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set Filter current
        R840_I2C.RegAddr = 0x0A;
        R840_SET_REG_Array(u8TunerIndex, 10, 0x9F, pSysInfo->FILT_CUR);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 10);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set Switch Buffer current
        R840_I2C.RegAddr = 0x04;
        R840_SET_REG_Array(u8TunerIndex, 4, 0xFB, pSysInfo->SWBUF_CUR);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 4);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set Filter Comp
        R840_I2C.RegAddr = 0x1E;
        R840_SET_REG_Array(u8TunerIndex, 30, 0x9F, pSysInfo->FILT_COMP);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 30);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set sw cap clk
        R840_I2C.RegAddr = 0x12;
        R840_SET_REG_Array(u8TunerIndex, 18, 0xFC, pSysInfo->SWCAP_CLK);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 18);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set NA det power
        R840_I2C.RegAddr = 0x1E;
        R840_SET_REG_Array(u8TunerIndex, 30, 0x7F, pSysInfo->NA_PWR_DET);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 30);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

	   // Set LNA det mode & NA_Discharge
        R840_I2C.RegAddr = 0x1F;
        R840_SET_REG_Array(u8TunerIndex, 31, 0xE2, pSysInfo->LNA_DET_MODE | pSysInfo->NA_DISCHARGE);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 31);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set AGC clk
        R840_I2C.RegAddr = 0x1B;
        R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, pSysInfo->AGC_CLK);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 27);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

        // Set RF Buf Current
        R840_I2C.RegAddr = 0x04;
        R840_SET_REG_Array(u8TunerIndex, 4, 0x7F, pSysInfo->RF_BUF_CUR);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 4);
        if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

       // Set SD IM
        R840_I2C.RegAddr = 0x03;
        R840_SET_REG_Array(u8TunerIndex, 3, 0xF3, pSysInfo->SD_IM);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 3);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;
    }

    *(pR840_pre_standard + u8TunerIndex) = RT_Standard;

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

        if((pSysInfo->FILT_EXT_POINT & 0x02)==0x00)  //HPF+3
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

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
    // Set LPF Lsb bit
        R840_I2C.RegAddr = 0x06;
        R840_SET_REG_Array(u8TunerIndex, 6, 0xF7, *(pR840_Fil_Cal_LpfLsb + u8TunerIndex *R840_STD_SIZE +RT_Standard)<<3);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex, 6);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
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

    // Set Filter 3dB
    R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, pSysInfo->FILT_3DB);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    // Set Filter Ext Condition (%)
    R840_I2C.RegAddr = 0x1E;
    R840_SET_REG_Array(u8TunerIndex, 30, 0xF8, 0x04 | pSysInfo->FILT_EXT_POINT);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
        //Set HPF notch
        R840_I2C.RegAddr = 0x06;
        R840_SET_REG_Array(u8TunerIndex, 6, 0xEF, pSysInfo->HPF_NOTCH);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,6);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
    }

    // Record SW Version
    R840_I2C.RegAddr = 0x0C;
    R840_SET_REG_Array(u8TunerIndex, 12, 0xF0, VER_NUM);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;

    // Record init flag
    R840_I2C.RegAddr = 0x1A;
    R840_SET_REG_Array(u8TunerIndex, 26, 0xF0, (*(pR840_Initial_done_flag + u8TunerIndex)<<1) | (*(pR840_Fil_Cal_flag + u8TunerIndex*R840_STD_SIZE + RT_Standard)<<2)|(*(pR840_IMR_done_flag + u8TunerIndex)) | (R840_DetectTfType<<3));
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,26);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
       return RT_Fail;

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
        //disable IF_AGC read, R8[7]=0
        R840_I2C.RegAddr = 0x08;
        R840_SET_REG_Array(u8TunerIndex, 8, 0x7F, 0);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,8);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

        //ADC power off, R12[7]=1
        R840_I2C.RegAddr = 0x0C;
        R840_SET_REG_Array(u8TunerIndex, 12, 0xFF, 0x80);
        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
    }

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
     UINT8   VGA_Count = 0;
     UINT8   VGA_Read = 0;

    if(R840_Xtal==16000)  //16M
        divnum_ring = 11;
    else if(R840_Xtal==24000)//24M
        divnum_ring = 2;
    else if(R840_Xtal==27000)//27M
        divnum_ring = 0;
    else
        divnum_ring = 11;

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
     if((R840_SetTfType==R840_TF_NARROW) || (R840_SetTfType==R840_TF_NARROW_ATSC))   //UL use 270n setting
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

        R840_Delay_MS(VGA_DELAY); //

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

     R840_Delay_MS(FILTER_DELAY); //delay ms

     if(R840_Muti_Read(u8TunerIndex,&u1FilterCalValuePre) != RT_Success)
         return RT_Fail;

     //read code 13
     R840_I2C.RegAddr = 0x0A;
     R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, 13);
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;

     R840_Delay_MS(FILTER_DELAY); //delay ms

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

          R840_Delay_MS(FILTER_DELAY); //delay ms

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

     if(*(pR840_Chip + u8TunerIndex)==R836_R)
     {
         if(u1FilterCodeResult>0)   //try code-1 & lsb=1
         {
              //extra LSB bit, R6[3]=1
              R840_I2C.RegAddr = 0x06;
              R840_SET_REG_Array(u8TunerIndex, 6, 0xFF, 8);
              R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,6);
              if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                  return RT_Fail;

              R840_I2C.RegAddr = 0x0A;
              R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, (u1FilterCodeResult-1));
              R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
              if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                  return RT_Fail;

              R840_Delay_MS(10); //delay ms

              if(R840_Muti_Read( u8TunerIndex,&u1FilterCalValue) != RT_Success)
                  return RT_Fail;

              if((u1FilterCalValue+FilCal_Gap*ADC_READ_COUNT) < u1FilterCalValuePre)
              {
                   u1FilterCodeResult = u1FilterCodeResult - 1;
                   R840_Lpf_Lsb = 1;
              }
              else
              {
              //u1FilterCodeResult = u1FilterCodeResult;
                   R840_Lpf_Lsb = 0;
              }
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
    R840_SET_REG_Array(u8TunerIndex, 4, 0xFD, 0x00);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,4);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;


   //Get Sys-Freq parameter
   *(pSysFreq_Info1 + u8TunerIndex) = R840_SysFreq_Sel(u8TunerIndex, R840_INFO.R840_Standard, R840_INFO.RF_KHz);


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


    if(*(pR840_Chip + u8TunerIndex)==R836_R)
    {
        //RF discharge
         R840_I2C.RegAddr = 0x1F;
         R840_SET_REG_Array(u8TunerIndex, 31, 0x1F, psysFreq_Info->RF_DISCHARGE);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,31);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

         //LNA mode     /*must override Xtal GM! */
         R840_I2C.RegAddr = 0x0F;
         R840_SET_REG_Array(u8TunerIndex, 15, 0xF7, 0x08);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,15);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

         //LNA pulse flag    /*must override Xtal GM!, Fixed at Disable */
         R840_I2C.RegAddr = 0x0F;
         R840_SET_REG_Array(u8TunerIndex, 15, 0xEF, 0x00);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,15);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

         //LNA img select /*fixed at  disch+img, R15[3]=1*/
         R840_I2C.RegAddr = 0x10;
         R840_SET_REG_Array(u8TunerIndex, 16, 0xF7, 0x08);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,16);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

         //LNA discharge current
         R840_I2C.RegAddr = 0x05;
         R840_SET_REG_Array(u8TunerIndex, 5, 0xE0, psysFreq_Info->LNA_DISCHARGE);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,5);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

         //Img NRB TOP adder //*must set after R840_MUX() *//
         if(psysFreq_Info->NRB_TOP_ADDER==0)
         {
             R840_SET_REG_Array(u8TunerIndex, 8, 0xEF, 0x00);
             R840_SET_REG_Array(u8TunerIndex, 15, 0x7F, 0x00);
         }
         else if(psysFreq_Info->NRB_TOP_ADDER==1)
         {
             R840_SET_REG_Array(u8TunerIndex, 8, 0xEF, 0x10);
             R840_SET_REG_Array(u8TunerIndex, 15, 0x7F, 0x00);
         }
         else if(psysFreq_Info->NRB_TOP_ADDER==2)
         {
             R840_SET_REG_Array(u8TunerIndex, 8, 0xEF, 0x00);
             R840_SET_REG_Array(u8TunerIndex, 15, 0x7F, 0x80);
         }
         else
         {
         	    R840_SET_REG_Array(u8TunerIndex, 8, 0xEF, 0x10);
              R840_SET_REG_Array(u8TunerIndex, 15, 0x7F, 0x80);
         }

         R840_I2C.RegAddr = 0x08;
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,8);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;

         R840_I2C.RegAddr = 0x0F;
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,15);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
    }
    else  //R836
    {
         //Det Mode = Discharge; Current = 1
         R840_I2C.RegAddr = 0x1F;
         R840_SET_REG_Array(u8TunerIndex, 31, 0x1E, 0x21);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,31);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
             return RT_Fail;
    }


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
       //Hys = 1.4+/-0.6
       R840_I2C.RegAddr = 0x06;
       R840_SET_REG_Array(u8TunerIndex, 6, 0xFB, 0x00);
       R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,6);
       if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
           return RT_Fail;

       //NAT_det_sour = mixer buff out
       R840_I2C.RegAddr = 0x18;
       R840_SET_REG_Array(u8TunerIndex, 24, 0xDF, 0x00);
       R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,24);
       if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
           return RT_Fail;

#if(R840_DTV_AGC_SLOW == FALSE)
		 //AGC CLK to 1khz
		 R840_I2C.RegAddr = 0x1B;
		 R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x00);  //R27[5:3]=000
		 R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
		 if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
			 return RT_Fail;
#else
		//AGC CLK to 60hz
       R840_I2C.RegAddr = 0x1B;
       R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x30);  //R27[5:3]=110
       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;
#endif


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

#if(R840_DTV_AGC_SLOW == FALSE)
                 R840_Delay_MS(100);

                 //AGC clk
                 R840_I2C.RegAddr = 0x1B;
                 if(*(pR840_Chip + u8TunerIndex)==R836)
                     R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x20);//[5:3]=100, 125Hz
                 else
                     R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x38);//[5:3]=111, 250Hz
                 R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
                 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                     return RT_Fail;
#endif
                 //VHF N+2 ACI
                if(DTV_VHF_ACI2_ENHANCE==TRUE)
                {
                    if(R840_INFO.RF_KHz<=300000)   //VHF band
                    {
                        //Mixer Amp current: highest
                        R840_I2C.RegAddr = 0x08;
                        R840_SET_REG_Array(u8TunerIndex, 8, 0xBF, 0x00);
                        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,8);
                        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                            return RT_Fail;

                        //force sharp mode
                        R840_I2C.RegAddr = 0x03;
                        R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,3);
                        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                           return RT_Fail;

                        R840_I2C.RegAddr = 0x02;
                        R840_SET_REG_Array(u8TunerIndex, 2, 0xFF, 0x20);
                        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,2);
                        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                            return RT_Fail;

                         //TF
                        if(R840_TF!=0)
                        {
                            R840_I2C.RegAddr = 0x00;
                            R840_SET_REG_Array(u8TunerIndex, 0, 0x80, (R840_TF-1));
                            R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,0);
                            if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                                return RT_Fail;
                        }
                    }
                    else //UHF band
                    {
                    //Mixer Amp current: lowest
                        R840_I2C.RegAddr = 0x08;
                        R840_SET_REG_Array(u8TunerIndex, 8, 0xFF, 0x40);
                        R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,8);
                        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                            return RT_Fail;

                         //force plain mode
                         R840_I2C.RegAddr = 0x03;
                         R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,3);
                         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                             return RT_Fail;

                         R840_I2C.RegAddr = 0x02;
                         R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0x00);
                         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,2);
                         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                             return RT_Fail;
                    }
                }
                else //DTV_VHF_ACI2_ENHANCE==FALSE
                {
                    //force plain mode
                    R840_I2C.RegAddr = 0x03;
                    R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,3);
                    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;
                    R840_I2C.RegAddr = 0x02;
                    R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0x00);
                    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,2);
                    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;
                }

                  //Q1.5K
                 if((R840_INFO.RF_KHz>=170000) && (R840_INFO.RF_KHz<=210000))
                 {
                     R840_I2C.RegAddr = 0x00;
                     R840_SET_REG_Array(u8TunerIndex, 0, 0x7F, 0x00);
                     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,0);
                     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;
                 }

                //filter +3dB off
                R840_I2C.RegAddr = 0x1E;
                R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, 0x00);
                R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,30);
                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                    return RT_Fail;

                 //Lo buffer dc=highest
                R840_I2C.RegAddr = 0x07;
                R840_SET_REG_Array(u8TunerIndex, 7, 0x9F, 0x60);
                R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,7);
                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                    return RT_Fail;

                if(*(pR840_Chip + u8TunerIndex)==R836)
                {
                if((R840_INFO.RF_KHz>=782000) && (R840_INFO.RF_KHz<=790000))
                {
                    //force sharp mode
                    R840_I2C.RegAddr = 0x03;
                    R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                        return RT_Fail;

                    R840_I2C.RegAddr = 0x02;
                    R840_SET_REG_Array(u8TunerIndex, 2, 0xFF, 0x20);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                        return RT_Fail;

                    //filter +3dB
                   R840_I2C.RegAddr = 0x1E;
                   R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, 0x08);
                   R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);
                   if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                       return RT_Fail;

                    //Lo buffer dc=lowest
                   R840_I2C.RegAddr = 0x07;
                   R840_SET_REG_Array(u8TunerIndex, 7, 0x9F, 0x00);
                   R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
                   if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                       return RT_Fail;
                }
                }  //end of if(R840 || R836)

#if(DTV_LTE_ONE_EQ==TRUE)

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
     {
         if((R840_INFO.RF_KHz>=782000) && (R840_INFO.RF_KHz<=790000))
         {
                //force sharp mode
                R840_I2C.RegAddr = 0x03;
                R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                    return RT_Fail;

                R840_I2C.RegAddr = 0x02;
                R840_SET_REG_Array(u8TunerIndex, 2, 0xFF, 0x20);
                R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);

                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                     return RT_Fail;

                //filter +3dB
                R840_I2C.RegAddr = 0x1E;
                R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, 0x08);
                R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);

                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                    return RT_Fail;

                //AGC 1KHz
                R840_I2C.RegAddr = 0x1B;
                R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x00);
                R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);
                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                     return RT_Fail;

                //Filter comp: OFF
                R840_I2C.RegAddr = 0x0F;
                R840_SET_REG_Array(u8TunerIndex, 15, 0xFB, 0x00);
                R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,15);
                if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                    return RT_Fail;
         }
         else
         {
                 //Filter comp: ON
                 R840_I2C.RegAddr = 0x0F;
                 R840_SET_REG_Array(u8TunerIndex, 15, 0xFF, 0x04);
                 R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,15);
                 if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                     return RT_Fail;
         }
     }  //end of if(R836R)

#endif
                break;

            case R840_DVB_T_6M:
            case R840_DVB_T_7M:
            case R840_DVB_T_8M:
            case R840_DVB_T_6M_IF_5M:
            case R840_DVB_T_7M_IF_5M:
            case R840_DVB_T_8M_IF_5M:
                //VHF N+2 ACI
               if(DTV_VHF_ACI2_ENHANCE==TRUE)
               {
                   if(R840_INFO.RF_KHz<=300000) //VHF band
                   {
                        //Mixer Amp current: highest
                        R840_I2C.RegAddr = 0x08;
                        R840_SET_REG_Array(u8TunerIndex, 8, 0xBF, 0x00);
                        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,8);
                        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;

                        //force sharp mode
                         R840_I2C.RegAddr = 0x03;
                         R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                             return RT_Fail;

                         R840_I2C.RegAddr = 0x02;
                         R840_SET_REG_Array(u8TunerIndex, 2, 0xFF, 0x20);
                         R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                             return RT_Fail;
                         //TF
                         if(R840_TF!=0)
                         {
                             R840_I2C.RegAddr = 0x00;
                             R840_SET_REG_Array(u8TunerIndex, 0, 0x80, (R840_TF-1));
                             R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,0);
                             if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                                  return RT_Fail;
                         }
                   }
                   else
                   {
                       //Mixer Amp current: lowest
                       R840_I2C.RegAddr = 0x08;
                       R840_SET_REG_Array(u8TunerIndex, 8, 0xFF, 0x40);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,8);
                       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                           return RT_Fail;

                       //TF Auto
                       R840_I2C.RegAddr = 0x03;
                       R840_SET_REG_Array(u8TunerIndex, 3, 0x7F, 0x00);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                           return RT_Fail;

                       R840_I2C.RegAddr = 0x02;
                       R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0x00);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                           return RT_Fail;
                   }
               }
               else //DTV_VHF_ACI2_ENHANCE==FALSE
               {
                   //TF Auto
                    R840_I2C.RegAddr = 0x03;
                    R840_SET_REG_Array(u8TunerIndex, 3, 0x7F, 0x00);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;

                    R840_I2C.RegAddr = 0x02;
                    R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0x00);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;
               }

                 //Q1.5K
                if((R840_INFO.RF_KHz>=170000) && (R840_INFO.RF_KHz<=210000))
                {
                    R840_I2C.RegAddr = 0x00;
                    R840_SET_REG_Array(u8TunerIndex, 0, 0x7F, 0x00);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,0);
                    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                        return RT_Fail;
                }

               //filter +3dB off
               R840_I2C.RegAddr = 0x1E;
               R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, 0x00);
               R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);
               if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                   return RT_Fail;

                //Lo buffer dc=highest
               R840_I2C.RegAddr = 0x07;
               R840_SET_REG_Array(u8TunerIndex, 7, 0x9F, 0x60);
               R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
               if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                   return RT_Fail;

               //must put after if(RF<300), or TF_mode overridded!!
               if(*(pR840_Chip + u8TunerIndex)==R836)
               {
                   if((R840_INFO.RF_KHz>=782000) && (R840_INFO.RF_KHz<=790000))
                   {
                     //force sharp mode
                     R840_I2C.RegAddr = 0x03;
                     R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                         return RT_Fail;

                     R840_I2C.RegAddr = 0x02;
                     R840_SET_REG_Array(u8TunerIndex, 2, 0xFF, 0x20);
                     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                         return RT_Fail;

                     //filter +3dB
                    R840_I2C.RegAddr = 0x1E;
                    R840_SET_REG_Array(u8TunerIndex, 30, 0xF7, 0x08);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);
                    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                        return RT_Fail;

                     //Lo buffer dc=lowest
                    R840_I2C.RegAddr = 0x07;
					R840_SET_REG_Array(u8TunerIndex, 7, 0x9F, 0x00);
                    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
                    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                        return RT_Fail;
                   }
                }
           break;

          case R840_ATSC:
          case R840_ATSC_IF_5M:
              if(*(pR840_Chip + u8TunerIndex)==R836_R)
              {
                  if((R840_INFO.RF_KHz>=685000) && (R840_INFO.RF_KHz<700000))
                  {
                       //force plain mode
                       R840_I2C.RegAddr = 0x03;
                       R840_SET_REG_Array(u8TunerIndex, 3, 0xFF, 0x80);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                           return RT_Fail;

                       R840_I2C.RegAddr = 0x02;
                       R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0x00);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                       if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
                           return RT_Fail;
                  }
                 else
                 {
                       //TF Auto
                       R840_I2C.RegAddr = 0x03;
                       R840_SET_REG_Array(u8TunerIndex, 3, 0x7F, 0x00);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,3);
                       if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                           return RT_Fail;

                       R840_I2C.RegAddr = 0x02;
                       R840_SET_REG_Array(u8TunerIndex, 2, 0xDF, 0x00);
                       R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
                       if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
                           return RT_Fail;
                 }
                }
                 break;
           default:
               break;
       }//end switch
	}
	return RT_Success;
}




R840_ErrCode R840_SetPllData(MS_U8 u8TunerIndex,R840_Set_Info R840_INFO)
{
#if 1 // byKOR, kaon
	R840_Set_Info*  pCurR840_Info = NULL;
    pCurR840_Info = pR840_Info + u8TunerIndex;
#endif
      if(*(pR840_Initial_done_flag + u8TunerIndex)==FALSE)
      {
          R840_Init(u8TunerIndex);
      }

#if 1 // byKOR, kaon, for auto RF gain mode
     if(R840_RfGainMode(u8TunerIndex, RF_AUTO) != RT_Success)
     {
         TUNER_ERR(("FAIL!!!!!! \n"));
         return RT_Fail;
	 }
#endif
      if(R840_SetStandard(u8TunerIndex,R840_INFO.R840_Standard) != RT_Success)
          return RT_Fail;

      if(R840_SetFrequency(u8TunerIndex,R840_INFO) != RT_Success)
          return RT_Fail;

#if 1 // byKOR, kaon
	memcpy(pCurR840_Info, &R840_INFO, sizeof(R840_Set_Info));
//	pCurR840_Info->R840_Standard = R840_INFO.R840_Standard;
//	pCurR840_Info->RF_KHz = R840_INFO.RF_KHz;
#endif
      return RT_Success;
}

R840_ErrCode R840_SetPllData_AutoSearch(MS_U8 u8TunerIndex,R840_Set_Info R840_INFO)
{
      UINT8 u1LpfCode, u1HpfCode;
      Sys_Info_Type* psys_Info;

      psys_Info = pSys_Info1 + u8TunerIndex;

      if(*(pR840_Initial_done_flag + u8TunerIndex)==FALSE)
      {
          R840_Init(u8TunerIndex);
      }

      if(R840_SetStandard(u8TunerIndex, R840_INFO.R840_Standard) != RT_Success)
          return RT_Fail;

      if(R840_SetFrequency(u8TunerIndex, R840_INFO) != RT_Success)
          return RT_Fail;

      // Set Filter Comp 0
      R840_I2C.RegAddr = 0x1E;
      R840_SET_REG_Array(u8TunerIndex, 30, 0x9F, 0x00);
      R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,30);
      if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
          return RT_Fail;

      //LPF/HPF wider
      if(psys_Info->FILT_EXT_ENA==0x10)  //(%)
      {
          if(*(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + R840_INFO.R840_Standard)< 4)
             u1LpfCode = 2;
          else
             u1LpfCode = *(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + R840_INFO.R840_Standard) - 2;

          u1HpfCode = psys_Info->HPF_COR;
      }
      else
      {
          if(*(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + R840_INFO.R840_Standard)< 2)
             u1LpfCode = 0;
          else
             u1LpfCode = *(pR840_Fil_Cal_code + u8TunerIndex*R840_STD_SIZE + R840_INFO.R840_Standard) - 2;

          if(psys_Info->HPF_COR>14)
             u1HpfCode = 15;
          else
             u1HpfCode = psys_Info->HPF_COR+1;
      }
     //Set LPF
     R840_I2C.RegAddr = 0x0A;
     R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, u1LpfCode);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,10);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
         return RT_Fail;

     //Set HPF
     R840_I2C.RegAddr = 0x0B;
     R840_SET_REG_Array(u8TunerIndex, 11, 0xF0, u1HpfCode);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,11);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
         return RT_Fail;

      return RT_Success;
}


R840_ErrCode R840_Standby(MS_U8 u8TunerIndex,R840_LoopThrough_Type R840_LoopSwitch)
{
	UINT8 i;

    for(i=0; i<R840_REG_NUM; i++)
    {
        *(pR840_SBY + u8TunerIndex *R840_REG_NUM + i)= *(pR840_Array + u8TunerIndex *R840_REG_NUM + i);
    }

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

    if(*(pR840_Chip + u8TunerIndex) ==R836_R)
    {
        //Xtal PW off
        R840_I2C.RegAddr = 0x02;
        R840_SET_REG_Array(u8TunerIndex, 2, 0xFF, 0x10);
        R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,2);
        if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
            return RT_Fail;
    }
    else
    {
     //Mixer Amp PW off
     R840_I2C.RegAddr = 0x08;
     R840_SET_REG_Array(u8TunerIndex, 8, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,8);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
         return RT_Fail;
    }

	 //Polyphase/Amp PW off
     R840_I2C.RegAddr = 0x09;
     R840_SET_REG_Array(u8TunerIndex, 9, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,9);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

	 //Filter/VGA PW off
     R840_I2C.RegAddr = 0x0A;
     R840_SET_REG_Array(u8TunerIndex, 10, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,10);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

	 //All/AGC, ADC PW off
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

     //LPF bypass
     R840_I2C.RegAddr = 0x01;
     R840_SET_REG_Array(u8TunerIndex, 1, 0xFB, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,1);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
            return RT_Fail;

    //Manual Gain
	//LNA auto off
    if((*(pR840_Chip + u8TunerIndex)==R836_R) || (*(pR840_Chip + u8TunerIndex)==R840_R))
	{
     R840_I2C.RegAddr = 0x0F;
     R840_SET_REG_Array(u8TunerIndex, 15, 0xF7, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,15);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;
	}
     R840_I2C.RegAddr = 0x05;
     R840_SET_REG_Array(u8TunerIndex, 5, 0xFF, 0x80);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,5);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;

	 //Mixer buffer off
     R840_I2C.RegAddr = 0x1A;
     R840_SET_REG_Array(u8TunerIndex, 26, 0xFF, 0x10);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,26);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;

	 //Mixer auto off
     R840_I2C.RegAddr = 0x07;
     R840_SET_REG_Array(u8TunerIndex, 7, 0xEF, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;

	//set LNA gain
     R840_I2C.RegAddr = 0x05;
     R840_SET_REG_Array(u8TunerIndex, 5, 0xE0, 31);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,5);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;

	 //set Mixer Buffer gain
     R840_I2C.RegAddr = 0x1A;
     R840_SET_REG_Array(u8TunerIndex, 26, 0xF0, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,26);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;

	 //set Mixer gain
     R840_I2C.RegAddr = 0x07;
     R840_SET_REG_Array(u8TunerIndex, 7, 0xF0, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
     if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	       return RT_Fail;

	 //set VGA pin, +32.5dB
	 R840_I2C.RegAddr = 0x0C;
     R840_SET_REG_Array(u8TunerIndex, 12, 0xE0, 0x0F);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,12);
	 if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
		 return RT_Fail;

	 //set LO DC=lowest
	 R840_I2C.RegAddr = 0x07;
     R840_SET_REG_Array(u8TunerIndex, 7, 0x9F, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,7);
	 if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
		 return RT_Fail;

	 //set LNA band = high
	 R840_I2C.RegAddr = 0x05;
     R840_SET_REG_Array(u8TunerIndex, 5, 0x9F, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,5);
	 if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
		 return RT_Fail;

	 //set Byp_lpf=bypass
	 R840_I2C.RegAddr = 0x01;
     R840_SET_REG_Array(u8TunerIndex, 1, 0xFB, 0x00);
     R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,1);
	 if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
		 return RT_Fail;

    return RT_Success;
}

//-----------------------------------------------------------------------//
//  This function is used to wake up tuner from Standby mode   //
//-----------------------------------------------------------------------//
R840_ErrCode R840_WakeUp( MS_U8 u8TunerIndex)
{
    UINT8 i;
    UINT8 Pll_Dldo, Pll_Aldo;

    //Pll_Dldo = R840_SBY[23] & 0xD8;
    //Pll_Aldo = R840_SBY[17] & 0xC0;
    Pll_Dldo = *(pR840_SBY + u8TunerIndex *R840_REG_NUM + 23) & 0xD8;
	Pll_Aldo = *(pR840_SBY + u8TunerIndex *R840_REG_NUM + 17) & 0xC0;

    *(pR840_SBY + u8TunerIndex *R840_REG_NUM + 9) = (*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 9) & 0x7F) | 0x80;
	*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 4) = (*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 4) & 0xF7) | 0x08;
	*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 23) = (*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 23) & 0xFE) | 0x01;
	*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 23) = (*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 23) & 0x27) | 0xD8;
	*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 17) = (*(pR840_SBY + u8TunerIndex *R840_REG_NUM + 17) & 0x3F) | 0xC0;
    //R840_SBY[9] = (R840_SBY[9] & 0x7F) | 0x80;    //poly off, R9[7]=1
    //R840_SBY[4] = (R840_SBY[4] & 0xF7) | 0x08;    //Mixer off, R4[3]=1
    //R840_SBY[23] = (R840_SBY[23] & 0xFE) | 0x01;  //IQ gen off, R23[0]=1
    //R840_SBY[23] = (R840_SBY[23] & 0x27) | 0xD8;  //PLL_LDO_D off, R23[7:6]=11, [4:3]=11
    //R840_SBY[17] = (R840_SBY[17] & 0x3F) | 0xC0;  //PLL_LDO_A off, R17[7:6]=11

    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = R840_REG_NUM;
    for(i = 0; i<R840_REG_NUM; i ++)
    {
        R840_I2C_Len.Data[i] = *(pR840_SBY + u8TunerIndex *R840_REG_NUM + i);
        *(pR840_Array + u8TunerIndex *R840_REG_NUM + i) = *(pR840_SBY + u8TunerIndex *R840_REG_NUM + i);
    }
    if(I2C_Write_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
        return RT_Fail;

	//poly on, R9[7]=0
	R840_I2C.RegAddr = 0x09;
    R840_SET_REG_Array(u8TunerIndex, 9, 0x7F, 0x00);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,9);
	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	    return RT_Fail;

	//Mixer on, R4[3]=0
	R840_I2C.RegAddr = 0x04;
    R840_SET_REG_Array(u8TunerIndex, 4, 0xF7, 0x00);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,4);
	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	    return RT_Fail;

	//IQ gen on, R23[0]=0
	R840_I2C.RegAddr = 0x17;
    R840_SET_REG_Array(u8TunerIndex, 23, 0xFE, 0x00);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,23);
	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	    return RT_Fail;

	//PLL_LDO_D (LDO1=200R, LDO2=200R), R23[7:6]=10; R23[4:3]=10
	R840_I2C.RegAddr = 0x17;
    R840_SET_REG_Array(u8TunerIndex, 23, 0x27, Pll_Dldo);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,23);
	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	    return RT_Fail;

	//PLL_LDO_A=2.35V, R17[7:6]=00
	R840_I2C.RegAddr = 0x11;
    R840_SET_REG_Array(u8TunerIndex, 17, 0x3F, Pll_Aldo);
    R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,17);
	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
	    return RT_Fail;

    return RT_Success;
}

R840_ErrCode R840_GetRfGain(MS_U8 u8TunerIndex,R840_RF_Gain_Info *pR840_rf_gain)
{
     UINT16 acc_lna_gain[25] = {0, 12, 23, 46, 66, 86, 103, 120, 136, 154,              //0~9
                                 173, 184, 196, 217, 233, 245, 254, 267, 278, 289,  //10~19
                                 300, 311, 321, 332, 332         //20~23
                                              };  //0~10

    UINT16 acc_rfbuf_gain;

    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = 5;
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
    {
        I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
    }

    pR840_rf_gain->RF_gain1 = (R840_I2C_Len.Data[3] & 0x1F);            //lna
    pR840_rf_gain->RF_gain2 = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);  //rf
    pR840_rf_gain->RF_gain3 = (R840_I2C_Len.Data[4] & 0x0F);             //mixer

    if(*(pR840_Chip + u8TunerIndex) == R836_R)
    {
       if(pR840_rf_gain->RF_gain1 > 24)
           pR840_rf_gain->RF_gain1 = 24;  //LNA gain max is mid band:24, others 23
    }
    else
    {
       if(pR840_rf_gain->RF_gain1 > 22)
          pR840_rf_gain->RF_gain1 = 22;  //LNA gain max is 22
    }

	//Mixer Amp Gain
    if(pR840_rf_gain->RF_gain3 > 10)
          pR840_rf_gain->RF_gain3 = 10;  //MixerAmp gain max is 10

	//Rf Buf Gain
    if(pR840_rf_gain->RF_gain2 < 14)
        acc_rfbuf_gain = pR840_rf_gain->RF_gain2*12;
    else if(pR840_rf_gain->RF_gain2 == 14)
        acc_rfbuf_gain = 163; //13*12+7
    else  //15
        acc_rfbuf_gain = 170; //13*12+14

    pR840_rf_gain->RF_gain_comb = (acc_lna_gain[pR840_rf_gain->RF_gain1] + acc_rfbuf_gain + pR840_rf_gain->RF_gain3*12);

      return RT_Success;
}





R840_ErrCode R840_RfGainMode(MS_U8 u8TunerIndex,R840_RF_Gain_TYPE R840_RfGainType)
{
    UINT8 MixerGain = 0;
    UINT8 RfGain = 0;
    UINT8 LnaGain = 0;
//    R840_RF_Gain_TYPE *ipR840_PreGainMode =  (R840_RF_Gain_TYPE*)(pR840_PreGainMode+u8TunerIndex);

    if(R840_RfGainType==RF_MANUAL)
    {
		if(*(pR840_PreGainMode+u8TunerIndex)==RF_AUTO)
		{
			R840_I2C_Len.RegAddr = 0x00;
			R840_I2C_Len.Len = 5;
			if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success)
			{
				I2C_Read_Len(u8TunerIndex, &R840_I2C_Len);
			}

			MixerGain = (R840_I2C_Len.Data[4] & 0x0F);
			RfGain = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);
			LnaGain = (R840_I2C_Len.Data[3] & 0x1F);
		}

        //LNA auto off
        if(*(pR840_Chip + u8TunerIndex) == R836_R)
        {
           R840_I2C.RegAddr = 0x0F;
           R840_SET_REG_Array(u8TunerIndex, 15, 0xF7, 0x00);
           R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,15);
           if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
               return RT_Fail;
        }

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
        if(*(pR840_Chip + u8TunerIndex)==R836_R)
        {
           R840_I2C.RegAddr = 0x0F;
           R840_SET_REG_Array(u8TunerIndex, 15, 0xFF, 0x08);
           R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,15);
           if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
               return RT_Fail;
        }
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

//--------- Set IC_Internal_cap------------------------------------//
//   Xtal loading match formula:
//   Xtal_cap_load*2 = IC_Internal_cap + External_cap
//   (default is for Xtal 16pF load)
//--------------------------------------------------------------------//
R840_ErrCode R840_SetXtalIntCap(MS_U8 u8TunerIndex, R840_Xtal_Cap_TYPE R840_XtalCapType)
{
     //Set internal Xtal cap
    if(R840_XtalCapType==XTAL_CAP_LARGE)  //for Xtal CL >16pF
        R840_SET_REG_Array(u8TunerIndex, 19, 0xDF, 0x00);
    else       //for Xtal CL < 16pF
        R840_SET_REG_Array(u8TunerIndex, 19, 0xFF, 0x20);

     R840_I2C.RegAddr = 0x13;
     R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,19);
     if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
           return RT_Fail;

    return RT_Success;
}

//--------- IF VGA mode control---------------------------------------------------------------//
//   1st input parameter (a) R840_VgaModeType
//   - VGA_AUTO: VGA controlled by chip self
//   - VGA_MANUAL: Manual mode
//
//   2nd input parameter (b) u1ManualCode
//   - 0(smallest gain)~15(largest gain), only used in VGA_MANUAL mode
//--------------------------------------------------------------------------------------------------//
R840_ErrCode R840_VgaCtrl(MS_U8 u8TunerIndex, R840_Vga_Mode_TYPE R840_VgaModeType, UINT8 u1ManualCode)
{

    if(R840_VgaModeType==VGA_MANUAL)
    {
        //VGA auto off
         R840_I2C.RegAddr = 0x0C;
         R840_SET_REG_Array(u8TunerIndex, 12, 0xF0, 0x10 | u1ManualCode);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
               return RT_Fail;
    }
    else
    {
        //VGA auto on
         R840_I2C.RegAddr = 0x0C;
          R840_SET_REG_Array(u8TunerIndex, 12, 0xEF, 0x00);
         R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);
         if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
               return RT_Fail;

    }

    return RT_Success;
}
//------------------------------------------------------------------//
//  R840_PLL_Lock( ): Read PLL lock status (R3[6])        //
//  Return: 1: PLL lock                                                    //
//          0: PLL unlock                                                //
//------------------------------------------------------------------//
UINT8 R840_PLL_Lock(MS_U8 u8TunerIndex)
{
    UINT8 fg_lock = 0;

    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = 3;
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
    {
        I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
    }

    if( (R840_I2C_Len.Data[2] & 0x40) == 0x00 )
        fg_lock = 0;
    else
        fg_lock = 1;

    return fg_lock;
}

//------------------------------------------------------------------//
//  R840_SetIfFreq( ): Update tuner IF parameter               //
//  Input para1: desired IF output freq                               //
//  Input para2: RF freq                                                   //
//  Input para3: TV system number                                   //
//------------------------------------------------------------------//
R840_ErrCode R840_SetIfFreq(MS_U8 u8TunerIndex, UINT16 IF_KHz, UINT32 RF_KHz, R840_Standard_Type u1SystemStd)
{
    Sys_Info_Type     Sys_Info_temp;
    R840_Set_Info     R840_Info;

    //Sys_Info1.IF_KHz = IF_KHz;

    R840_Info.R840_ClkOutMode = CLK_OUT_OFF;
    R840_Info.R840_IfAgc_Select = IF_AGC1;
    R840_Info.R840_LT = LT_OFF;
    R840_Info.R840_Standard = u1SystemStd;

    Sys_Info_temp = R840_Sys_Sel(u8TunerIndex, u1SystemStd);
    R840_Info.RF_KHz = RF_KHz + IF_KHz - Sys_Info_temp.IF_KHz;

    R840_SetFrequency(u8TunerIndex,R840_Info);

    return RT_Success;
}

//------------------------------------------------------------------------------- //
//  R840_SetLpfBw( ): Set LPF coarse tune parameter: LPF_BW         //
//  cover range if LPF_BW is:                                                          //
//      0: 7.60M~11.30MHz                                                              //
//      1: 6.45M~ 9.00MHz                                                               //
//      2: 5.60M~ 7.45MHz                                                                //
//--------------------------------------------------------------------------------//
R840_ErrCode R840_SetLpfBw(MS_U8 u8TunerIndex, UINT8 LPF_BW)
{
    UINT8 u1LpfBw;

    if(LPF_BW==0)
         u1LpfBw = 0x00;     //8M
    else if(LPF_BW==1)
         u1LpfBw = 0x40;     //7M
    else
         u1LpfBw = 0x60;     //6M

    // Set LPF coarse BW, R11[6:5]
    R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0x9F, u1LpfBw);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    return RT_Success;
}

//------------------------------------------------------------------------------- //
//  R840_SetLpfOffset( ): Set LPF fine tune parameter: LPF_Offset     //
//  range: 0x00~0x0F    (R10[3:0])                                                    //
//  15 is narrowest; 0 is widest                                                           //
//--------------------------------------------------------------------------------//
R840_ErrCode R840_SetLpfOffset(MS_U8 u8TunerIndex, UINT8 LPF_Offset)
{
    //Set LPF fine tune parameter
    R840_I2C.RegAddr = 0x0A;
    R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, LPF_Offset);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    return RT_Success;
}

//---------------------------------------------------------------------//
//  R840_SetHpfOffset( ): Set HPF parameter: HPF_Offset     //
//  range is: 0x00~0x0F    (R11[3:0])                                    //
//  0 is narrowest; 15 is widest                                              //
//	0: 4.80M		4: 2.25M		8: 1.45M		12: 0.67M			//
//	1: 2.97M		5: 2.00M		9: 1.15M		13: 0.54M			//
//	2: 2.65M		6: 1.78M		10: 0.90M		14: 0.41M			//
//	3: 2.50M		7: 1.67M		11: 0.79M		15: 0.33M			//
//---------------------------------------------------------------------//
R840_ErrCode R840_SetHpfOffset(MS_U8 u8TunerIndex, UINT8 HPF_Offset)
    {

    //Set HPF
    R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0xF0, HPF_Offset);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
          return RT_Fail;

    return RT_Success;
}

//------------------------------------------------------------------//
//  R840_SetIfLpf( ): Set LPF                                          //
//  Input parameter: LPF_Cor_Freq    (unit: KHz)              //
//------------------------------------------------------------------//
R840_ErrCode R840_SetIfLpf(MS_U8 u8TunerIndex, UINT32 LPF_Cor_Freq)
{
    UINT8     u1FilterCode;

    u1FilterCode = R840_Filt_Cal_ADC(u8TunerIndex, LPF_Cor_Freq, 0x40, 8);

    // Set LPF fine code
    R840_I2C.RegAddr = 0x0A;
    R840_SET_REG_Array(u8TunerIndex, 10, 0xF0, u1FilterCode);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,10);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    // Set LPF coarse BW
    R840_I2C.RegAddr = 0x0B;
    R840_SET_REG_Array(u8TunerIndex, 11, 0x9F, R840_Bandwidth);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,11);
    if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
        return RT_Fail;

    return RT_Success;
}

//------------------------------------------------------------------------------- //
//  R840_SetLnaTop( ): Set LNA target                                             //
//  range of LNA_Top is 0~7 (R27[2:0])                                            //
//	0 is max; 7 is min                                                                        //
//--------------------------------------------------------------------------------//
R840_ErrCode R840_SetLnaTop(MS_U8 u8TunerIndex,UINT8 LNA_Top)
{
    R840_I2C.RegAddr = 0x1B;
    R840_SET_REG_Array(u8TunerIndex, 27, 0xF8, LNA_Top);
    R840_I2C.Data = R840_GET_REG_Array(u8TunerIndex,27);
    if(I2C_Write(u8TunerIndex,&R840_I2C) != RT_Success)
        return RT_Fail;

    return RT_Success;
}
//----------------------------------------------------------------------//
//  R840_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                                //
//  2nd parameter: input Standard                                           //
//  3rd parameter: output signal level (dBm*1000)                    //
//  4th parameter: output RF max gain indicator (1:max gain)    //
//-----------------------------------------------------------------------//
R840_ErrCode R840_GetRfRssi(MS_U8 u8TunerIndex, UINT32 RF_Freq_Khz, R840_Standard_Type RT_Standard, int *RfLevelDbm, UINT8 *fgRfMaxGain)
{
    UINT8  bRfFlag, bPulseFlag;
    UINT8  u1TF_Mode1, u1TF_Mode2;
    UINT8  u1Gain1, u1Gain2, u1Gain3;
    UINT16  acc_lna_gain;
    UINT16  acc_rfbuf_gain;
    UINT16  rf_total_gain;
    UINT16  u2PulseComp;
    UINT8   u1FreqIndex;
    UINT16  u2FreqFactor;
    int   rf_rssi;
    int   fine_tune = 0;    //for find tune

    R840_I2C_Len.RegAddr = 0x00;
    R840_I2C_Len.Len = 5;
    if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
    {
    	I2C_Read_Len(u8TunerIndex,&R840_I2C_Len);
    }
    u1Gain1 = (R840_I2C_Len.Data[3] & 0x1F);            //lna
    u1Gain2 = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);  //rf
    u1Gain3 = (R840_I2C_Len.Data[4] & 0x0F);             //mixer
    bRfFlag = ((R840_I2C_Len.Data[1] & 0x80) >> 7);
    bPulseFlag = ((R840_I2C_Len.Data[1] & 0x40) >> 6);
    u1TF_Mode1 = ((R840_GET_REG_Array(u8TunerIndex,3)) & 0x80) >> 7;          //R3[7], 0:auto    1:manuel
    u1TF_Mode2 = ((R840_GET_REG_Array(u8TunerIndex,2)) & 0x20) >> 5;          //R2[5], 0:plain   1:sharp

    //Limit LNA, MixerAmp max gain
    if(u1Gain3 > 10)
           u1Gain3 = 10;

     if(*(pR840_Chip + u8TunerIndex)==R836_R)
     {
    	if(u1Gain1 > 24)
    		u1Gain1 = 24;

          //max gain indicator
    	if((u1Gain1==24) && (u1Gain2==15) && (u1Gain3==10))
    		*fgRfMaxGain = 1;
    	else
    		*fgRfMaxGain = 0;
     }
     else
     {
    		if(u1Gain1 > 22)
    		u1Gain1 = 22;

    	//max gain indicator
    	if((u1Gain1==22) && (u1Gain2==15) && (u1Gain3==10))
    		*fgRfMaxGain = 1;
    	else
    		*fgRfMaxGain = 0;
     }

    //Select LNA freq table
    if(RF_Freq_Khz<350000)   //<350M
    {
    	u1FreqIndex = 0;
    	u2FreqFactor = 20;
    }
    else if((RF_Freq_Khz>=350000)&&(RF_Freq_Khz<515000))   //350~515M
    {
    	u1FreqIndex = 1;
    	u2FreqFactor = 130;
    }
    else if((RF_Freq_Khz>=515000)&&(RF_Freq_Khz<615000))   //515~615M
    {
    	u1FreqIndex = 2;
    	u2FreqFactor = 120;
    }
    else if((RF_Freq_Khz>=615000)&&(RF_Freq_Khz<705000))   //615~705M
    {
    	u1FreqIndex = 3;
    	u2FreqFactor = 110;
    }
    else    // >=705
    {
    	u1FreqIndex = 4;
    	u2FreqFactor = 80;
    }

    //Rf Buf Gain Table
    if(u1Gain2 < 14)
    	acc_rfbuf_gain = u1Gain2*12;
    else if(u1Gain2 == 14)
    	acc_rfbuf_gain = 163; //13*12+7
    else  //15
    	acc_rfbuf_gain = 170; //13*12+14

    //LNA Gain
    if((bRfFlag==1 && u1TF_Mode1==0) || (u1TF_Mode1 & u1TF_Mode2))   //Sharp
    	acc_lna_gain = R840_Lna_Sharp_Acc_Gain[u1FreqIndex][u1Gain1];
    else
    	acc_lna_gain = R840_Lna_Plain_Acc_Gain[u1FreqIndex][u1Gain1];

       //Pulse Flag
    if(bPulseFlag==1)
    {
    	if((u1Gain1>=17) && (u1Gain2<=21))
    		u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][0];
    	else if(u1Gain1==22)
    		u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][1];
    	else if(u1Gain1==23)
    		u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][2];
    	else if(u1Gain1==24)
    		u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][3];
    	else
    		u2PulseComp = 0;
    }
    else
    	u2PulseComp = 0;

    acc_lna_gain = acc_lna_gain + u2PulseComp;

    //Add Rf Buf and Mixer Gain
    rf_total_gain = acc_lna_gain +acc_rfbuf_gain + u1Gain3*12;

    rf_rssi = fine_tune - (int) (rf_total_gain - u2FreqFactor);

    *RfLevelDbm = rf_rssi*100;
#if 0 // byKOR, kaon, for debug only
printf("\t RF = %d ", rf_rssi*100);
#endif
       return RT_Success;
}

//----------------------------------------------------------------------//
//  R840_GetIfRssi( ): Get IF VGA GAIN                               //
//  1st parameter: return IF VGA Gain     (dB*100)                   //
//-----------------------------------------------------------------------//
R840_ErrCode R840_GetIfRssi(MS_U8 u8TunerIndex,int *VgaGain)
{
    UINT8   adc_read;
    I2C_TYPE Dlg_I2C;
    I2C_LEN_TYPE Dlg_I2C_Len;
    int   vga_table[64] = {                        //*100
		0, 0, 0, 0, 0, 0, 10, 10, 10, 10,   //0~9
		30, 50, 60, 100, 150, 300, 410, 520, 650, 780,   //10~19
		1030, 1260, 1350, 1440, 1520, 1600, 1670, 1750, 1900, 2090,   //20~29
		2180, 2350, 2450, 2640, 2790, 2940, 3100, 3270, 3430, 3550,   //30~39
		3670, 3790, 3870, 3950, 4030, 4110, 4150, 4190, 4220, 4250,   //40~49
		4300, 4350, 4400, 4440, 4470, 4500, 4520, 4540, 4560, 4580,   //50~59
		4600, 4610, 4610, 4620              //60~63
    };

    //enable IF_AGC read, R8[7]=1
    Dlg_I2C.RegAddr = 0x08;
    R840_SET_REG_Array(u8TunerIndex, 8, 0xFF, 0x80);
    Dlg_I2C.Data = R840_GET_REG_Array(u8TunerIndex,8);
    if(I2C_Write(u8TunerIndex,&Dlg_I2C) != RT_Success)
        return RT_Fail;

    //ADC power on, R12[7]=0
    Dlg_I2C.RegAddr = 0x0C;
    R840_SET_REG_Array(u8TunerIndex, 12, 0x7F, 0x00);
    Dlg_I2C.Data = R840_GET_REG_Array(u8TunerIndex,12);
    if(I2C_Write(u8TunerIndex,&Dlg_I2C) != RT_Success)
        return RT_Fail;

    //read adc value
    Dlg_I2C_Len.RegAddr = 0x00;
    Dlg_I2C_Len.Len = 2;
    if(I2C_Read_Len(u8TunerIndex,&Dlg_I2C_Len) != RT_Success) // read data length
    {
        I2C_Read_Len(u8TunerIndex,&Dlg_I2C_Len);
    }
    adc_read = (Dlg_I2C_Len.Data[1] & 0x3F);

    *VgaGain = vga_table[adc_read];
#if 0 // byKOR, kaon, for debug only
printf("\t IF[%d] = %d ", adc_read, vga_table[adc_read]);
#endif
    return RT_Success;
}

//----------------------------------------------------------------------//
//  R840_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                                //
//  2nd parameter: input Standard                                           //
//  3rd parameter: return signal level indicator (dBm)               //
//-----------------------------------------------------------------------//
R840_ErrCode R840_GetTotalRssi(MS_U8 u8TunerIndex,UINT32 RF_Freq_Khz, R840_Standard_Type RT_Standard, int *RssiDbm)
{
    int   rf_rssi;
    int   if_rssi;
    int   rem, total_rssi;
    int   ssi_offset = 0;   //need to fine tune by platform
    int   total_rssi_dbm;
    UINT8  rf_max_gain_flag;

    R840_GetRfRssi( u8TunerIndex,RF_Freq_Khz, RT_Standard, &rf_rssi, &rf_max_gain_flag);

    if(*(pR840_Chip + u8TunerIndex)==R836_R)
        R840_GetIfRssi( u8TunerIndex,&if_rssi);  //vga gain
    else
        if_rssi = 0;

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
#if 1 // byKOR, kaon, for dBuV
*RssiDbm += 109;
#endif
    return RT_Success;
}

R840_ErrCode R840_AGC_Slow(MS_U8 u8TunerIndex)  //Set AGC clock to 60Hz
{
	//AGC CLK to 60hz
	R840_I2C.RegAddr = 0x1B;
	R840_SET_REG_Array(u8TunerIndex, 27, 0xC7, 0x30);  //[5:3]=110
	R840_I2C.Data =  R840_GET_REG_Array(u8TunerIndex,27);

	if(I2C_Write(u8TunerIndex, &R840_I2C) != RT_Success)
		return RT_Fail;

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
               R840_Info.R840_Standard = R840_DVB_C_8M_IF_5M;
               break;
           }
    }
    else if(ATSC == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
           R840_Info.R840_Standard = R840_ATSC_IF_5M;
    }
    else if(ISDBT == *InitParam[u8TunerIndex].pCur_Broadcast_type)
    {
            R840_Info.R840_Standard = R840_ISDB_T_IF_5M;
    }
    else
    {
        R840_Info.R840_Standard = R840_DVB_T_8M_IF_5M;
    }


    //R840 Frequency Setting
    R840_Info.RF_KHz = u32Freq; // unit: kHz
//    u32CurFreqKhz[u8TunerIndex] = u32Freq;
    //Loop Through
#if 1 // byKOR, kaon, L/T never use at R836 ( use external L/T )
    R840_Info.R840_LT = LT_OFF; // Loop through OFF
#endif
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
     *(pSys_Info1 + u8TunerIndex) = R840_Sys_Sel(u8TunerIndex, R840_ATSC_IF_5M); //set as default
     TUNER_DBG(("SUCCESS!!!!! \n"));
     return RT_Success;
}

MS_BOOL R836_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
#if 1 // byKOR, kaon
	R840_Set_Info*  pCurR840_Info = NULL;
    pCurR840_Info = pR840_Info + u8TunerIndex;
#endif
    MS_BOOL* pbLT_ON;
    switch(fuction_type)
    {

         case TUNER_EXT_FUNC_GET_POWER_LEVEL:
#if 1 // byKOR, kaon
            if( RT_Success != R840_GetTotalRssi( u8TunerIndex, pCurR840_Info->RF_KHz, pCurR840_Info->R840_Standard, (int*)data))
#else
//            if( RT_Success != R840_GetTotalRssi( u8TunerIndex,u32CurFreqKhz[u8TunerIndex],(*(pR840_pre_standard + u8TunerIndex)), (int*)data))
#endif
            {
                return FALSE;
            }

            break;
        case TUNER_EXT_FUNC_LOOP_THROUGH:
            pbLT_ON = (MS_BOOL*)data;
            if(*pbLT_ON)
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
                return FALSE;
            break;
    #if 1 // byKOR, kaon, for debug registers
    	case TUNER_EXT_FUNC_FINALIZE:
    	{
			int i;
			R840_I2C_Len.RegAddr = 0x00;
			R840_I2C_Len.Len 	 = 32;
			if(I2C_Read_Len(u8TunerIndex, &R840_I2C_Len) != RT_Success) // read data length
			{
printf(" [L:%d] R840_ERROR ", __LINE__);
			    return FALSE;
			}

    		printf("================================================\n");
    		for(i=0; i<16; i++) printf(",%02X", i);
    		printf("\n");
    		printf("================================================\n");
    		for(i=0; i<R840_REG_NUM; i++)
    		{
			printf(",%02X", R840_I2C_Len.Data[i]);
			if(i%16==15) printf("\n");
			}
    		printf("\n================================================\n");
    		break;
		}
    #endif

        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
    }
    return TRUE;
}

#define R836_CHIP_ID 0x96
MS_BOOL R836_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    UINT8 regData = 0;
    MS_U8 i;

    for(i=0; i< sizeof(u8Possible_SLAVE_IDs); i++)
    {
        InitParam[u8TunerIndex].u8SlaveID = u8Possible_SLAVE_IDs[i];
        R840_I2C_Len.Len = 1;
        R840_I2C_Len.Data[0] = 0;
        if(I2C_Read_Len(u8TunerIndex,&R840_I2C_Len) != RT_Success)
        {
            TUNER_ERR(("[R836] Read chip ID fail with slave ID 0x%x\n", InitParam[u8TunerIndex].u8SlaveID));
        }
        else
        {
             break;
        }
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

  if(pR840_Initial_done_flag == NULL)
      return FALSE;

  if(*(pR840_Initial_done_flag + u8TunerIndex)==FALSE)
      return FALSE;

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

