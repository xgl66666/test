/***************************************************************************** 
Copyright 2011, AltoBeam Inc. All rights reserved.
     
File Name: atbm885x.h
******************************************************************************/

#ifndef ATBM885X_H
#define ATBM885X_H


#include <frontend_config.h>

#if (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x)

typedef unsigned char u_int8;
typedef unsigned short u_int16;
typedef unsigned int u_int32;

#ifndef PI
#define PI (3.1415927)
#endif
/* 
*struct MPEG_TS_mode_t 
*
*@TS_Transfer_Type: TS stream transfer type, can be set to parallel(8 data bus) or serial(1 data bus) mode
*
*@Demod output edge: demod will send TS data on this edge of TS stream clock
*
*@SPI_clock_constant_output: TS stream clock  can be set to valid for all the time or only during TS data valid (188 bytes) 
*/
struct MPEG_TS_mode_t 
{
#define TS_PARALLEL_MODE 1
#define TS_SERIAL_MODE   0
	u_int8 TS_Transfer_Type;

#define TS_OUTPUT_FALLING_EDGE 1
#define TS_OUTPUT_RISING_EDGE  0
	u_int8 output_edge; 

#define TS_SPI_MSB_ON_DATA_BIT7 1
#define TS_SPI_MSB_ON_DATA_BIT0 0
      u_int8 TS_SPI_MSB_Selection;

#define TS_SSI_OUTPUT_ON_DATA_BIT7 1
#define TS_SSI_OUTPUT_ON_DATA_BIT0 0
      u_int8 TS_SSI_Output_Selection;
      
#define TS_CLOCK_CONST_OUTPUT 1
#define TS_CLOCK_VALID_OUTPUT 0	
	u_int8 SPI_clock_constant_output; 		
};

/*symbol_rate: typicaly use 6875K, usually used range from 3000K to 6952K*/
struct DVBC_Params_t 
{   
	double	symbol_rate;
};

/*
*struct tuner_config_t
*
*@IF_Frequency: tuner IF frequency output in MHz. Most silicon tuners' IF is 0. Most CAN Tuners' are 36M, 36.166M
*
*@or 36.125 MHz (Typical IF used by DVB-C tuners)
*
*@IQ_mode: when tuner IF is zero or, demod needs to know if IQ is swapped or not on hardware board, also related with spectrum inverse
*/
struct tuner_config_t 
{

	/*CAN tuners or some silicon tuners may have these configurations*/      
	/*36.1667, 36, 36.125 MHz*/

	/*silicon tuners may have ZIF configuration, 0 MHz*/

	double IF_Frequency;

#define SWAP_IQ 0
#define NO_SWAP_IQ 1
	u_int8 IQ_mode;	
};

/* 
* struct     custom_config_t 
*
*@tuner_config: struct of tuner configuration
*
*@ts_mode: struct of TS mode
*
*@CRYSTAL_OR_OSCILLATOR: demod can use crystal or oscillator 
*
*@sample_clk_Frequency: crystal or oscillator frequency on hardware board for demod
*
*@dtmb_dvbc_mode:select receiving mode DTMB or DVB-C for ATBM885x
*
*@dvbc_params: DVB-C parameters
*/

struct custom_config_t
{
	struct tuner_config_t tuner_config;

	struct MPEG_TS_mode_t  ts_mode;	

#define CRYSTAL 0
#define OSCILLATOR 1
	u_int8 CRYSTAL_OR_OSCILLATOR;

	double sample_clk_Frequency;

#define ATBM_DTMB_MODE  1
#define ATBM_DVBC_MODE  0    
	u_int8 dtmb_dvbc_mode;	
	struct DVBC_Params_t  dvbc_params;
};

extern unsigned char ATBM_ChipID;

/********DTMB and DVB-C common API functions*************/
u_int8 ATBMChipID(void);
void ATBM_I2CByPassOn(void);
void ATBM_I2CByPassOff(void);
int  ATBMLockedFlag(void);
int ATBMLockedLoopCheck(void);
void ATBM_Standby(void);
void ATBM_Standby_WakeUp(void);
void ATBM_Suspend(void);

/****DTMB API Functions******************************/
void ATBMInit_DTMB_Mode(void); 
u_int16 ATBMSignalStrength(void);	
double ATBMSignalNoiseRatio(void);
int  ATBMSignalQuality(void);
u_int16 ATBMSignalStrength_Sharp2093(void); //sample for reference of signal strength evaluation
u_int16 ATBMSignalStrength_TDAC2_C02A(void); //sample for reference of signal strength evaluation
u_int16 ATBMSignalStrength_Maxim2165(void);	//sample for reference of signal strength evaluation
double ATBMFrameErrorRatio(void);

/****DVB-C API Functions******************************/
void ATBMInit_DVBC_Mode(void);
double   ATBM_DVBC_SNR(void);
double   ATBM_DVBC_BER(u_int32 *BerExponent);
double   ATBM_DVBC_UncorrectablePER(u_int32 *PktsExponent);
double   ATBM_DVBC_CarrierOffset(void);
u_int8   ATBM_DVBC_GetQAM(void);
u_int16 ATBM_DVBC_SignalStrength(void);
u_int16 ATBM_DVBC_SignalStrength_TDAC2_C02A(void);

/*************DVB-C internal functions************************/
void ATBM_DVBC_Init(struct custom_config_t custom_config);
void ATBM_DVBC_SetSymbolRate(double OSC_Freq, double dSymbolRateM);
void ATBM_DVBC_SetCarrier(double OSC_Freq, double dSymbolRateM);
void ATBM_DVBC_SetQAM(void);

/******************Demodulator Internal functions***********************/
void ATBMInit(void);
void ATBMConfig(struct custom_config_t custom_config);
u_int8 ATBM_Check_Demod_Status(void);
u_int8 PLL_Lock_Flag(void);
u_int8 ATBM_Check_PLL_Status(void);
void ATBMReset(u_int8 cry_or_osc);
void ATBMSetTSMode(struct MPEG_TS_mode_t ts_mode);   /*Default SPI , it can be configured to Serial mode*/
int ATBMSetOSC(struct tuner_config_t tuner_config, double sample_clk_Frequency); 
u_int8  ATBMGetTPS(void);
void ATBMDebugRegister(void);

/****************Testing utility functions*******************/
double ATBMPreBCH_BlockErrorRatio(void);
void ATBM_PPM_Test(void);
double ATBMCarrierOffset(void);
void ATBM_GPO_PWM1_Output(u_int8 level);
void ATBM_GPO_I2CINT_Output(u_int8 level);
void ATBM_GPO_TestIO20_Output(u_int8 level);
void ATBM_GPO_TestIO23_Output(u_int8 level);
void ATBM_TestIO23_Indicate_TS_Lock(void);
void ATBM_TestIO23_Indicate_FEC_No_Error(void);
void ATBM_TestIO20_Indicate_FEC_Error(void);
void ATBM_TestIO20_Indicate_TS_Unlock(void);
void ATBMErrorOnDurationMillisecond(u_int16 ms);
void ATBMLockOffDurationMillisecond(u_int16 ms);

/****DTMB I2C interface functions*********************/
void ATBMWriteRegArray(u_int8 *reg_table, u_int32 table_len);
void ATBMDebugRegArray(u_int8 *reg_table, u_int32 table_len);
u_int8  ATBMRead(u_int8 base_addr, u_int8 register_addr);
int  ATBMWrite(u_int8 base_addr, u_int8 register_addr, u_int8 data);

/****DVB-C I2C interface functions*********************/
u_int8  ATBMTransRead(u_int8 base_addr, u_int8 register_addr);
int  ATBMTransWrite(u_int8 base_addr, u_int8 register_addr, u_int8 data);
void ATBM_DVBC_Write(u_int32 uAddress, u_int32 u32Data);
u_int32 ATBM_DVBC_Read(u_int32  uAddress);

extern void Delayms (int ms);
extern void DemodHardwareReset(void);

extern int I2CWrite(u_int8 I2CSlaveAddr, u_int8 *pdata, int length);
extern int I2CRead(u_int8 I2CSlaveAddr, u_int8 *pdata, int length);

#endif

#endif

