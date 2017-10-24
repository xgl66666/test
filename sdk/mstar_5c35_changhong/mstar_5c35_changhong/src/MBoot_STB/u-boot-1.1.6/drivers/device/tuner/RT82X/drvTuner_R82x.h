#ifndef  _R828_H_
#define _R828_H_

//***************************************************************
//*                       INCLUDES.H
//***************************************************************
#if (FRONTEND_TUNER_TYPE == TUNER_RT828)
#define VERSION   "R828S_v1.47B"
#define VER_NUM  47
#elif (FRONTEND_TUNER_TYPE == TUNER_RT820T)
#define VERSION   "R820T_v1.48B"
#define VER_NUM  48
#endif

#pragma once


#define USE_16M_XTAL TRUE

//----------------------------------------------------------//
//                   Type Define                            //
//----------------------------------------------------------//

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
#define R828_Delay_MS MsOS_DelayTask

typedef enum _R828_ErrCode
{
    RT_Success = TRUE,
    RT_Fail    = FALSE
}R828_ErrCode;

typedef enum _Rafael_Chip_Type  //Don't modify chip list
{
     R828 = 0,
     R828D,
     R828S,
     R820T,
     R820C,
     R620D,
     R620S
}Rafael_Chip_Type;
//----------------------------------------------------------//
//                   R828 Parameter                        //
//----------------------------------------------------------//

#define R828_Xtal   16000
//#define R828_Xtal   24000
#define DIP_FREQ      320000
#define IMR_TRIAL    9
#if (FRONTEND_TUNER_TYPE == TUNER_RT828)
#define VCO_pwr_ref 0x01
#elif (FRONTEND_TUNER_TYPE == TUNER_RT820T)
#define VCO_pwr_ref 0x02
#endif

extern UINT32 R828_IF_khz;
extern UINT32 R828_CAL_LO_khz;
extern UINT8  R828_IMR_point_num;
extern UINT8  R828_IMR_done_flag;
extern UINT8  Rafael_Chip;

typedef enum _R828_Standard_Type  //Don't remove standand list!!
{
    NTSC_MN = 0,
    PAL_I,
    PAL_DK,
    PAL_B_7M,
    PAL_GH_8M,
    SECAM_L,
    SECAM_L1_INV,
    SECAM_L1,
    ATV_SIZE,
    DVB_T_6M = ATV_SIZE,
    DVB_T_7M,
    DVB_T_7M_2,
    DVB_T_8M,
    DVB_T2_6M,
    DVB_T2_7M,
    DVB_T2_7M_2,
    DVB_T2_8M,
    DVB_T2_1_7M,
    DVB_T2_10M,
    DVB_C_8M,
    DVB_C_6M,
    ISDB_T,
    DTMB,
    ATSC,
    FM,
    STD_SIZE
}R828_Standard_Type;

extern UINT8  R828_Fil_Cal_flag[STD_SIZE];


typedef enum _R828_LoopThrough_Type
{
    LOOP_THROUGH = TRUE,
    SIGLE_IN     = FALSE
}R828_LoopThrough_Type;

typedef enum _R828_LoopThroughAtt_Type
{
	LT_ATT_ON = TRUE,
	LT_ATT_OFF = FALSE
}R828_LoopThroughAtt_Type;
typedef enum _R828_InputMode_Type
{
     AIR_IN = 0,
     CABLE_IN_1,
     CABLE_IN_2
}R828_InputMode_Type;

typedef enum _R828_GPIO_Type
{
     HI_SIG = TRUE,
     LO_SIG = FALSE
}R828_GPIO_Type;

typedef struct _R828_Set_Info
{
     UINT32        RF_KHz;
     R828_Standard_Type R828_Standard;
     R828_LoopThrough_Type RT_Input;
     R828_InputMode_Type   RT_InputMode;
     //R828_IfAgc_Type R828_IfAgc_Select;
}R828_Set_Info;

typedef struct _R828_RF_Gain_Info
{
    UINT8   RF_gain1;
    UINT8   RF_gain2;
    UINT8   RF_gain_comb;
}R828_RF_Gain_Info;

typedef enum _R828_RF_Gain_TYPE
{
     RF_AUTO = 0,
     RF_MANUAL
}R828_RF_Gain_TYPE;

typedef struct _I2C_LEN_TYPE
{
    UINT8 RegAddr;
    UINT8 Data[50];
    UINT8 Len;
}I2C_LEN_TYPE;

typedef struct _I2C_TYPE
{
    UINT8 RegAddr;
    UINT8 Data;
}I2C_TYPE;

#endif
