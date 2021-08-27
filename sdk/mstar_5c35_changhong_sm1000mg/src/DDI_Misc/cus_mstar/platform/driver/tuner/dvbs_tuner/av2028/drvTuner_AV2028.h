
#ifndef _AV2026_H
#define _AV2026_H
#include "drvTuner.h"

#ifdef _CPLUSPLUS
extern "C" {
#endif
#define MAX_INPUT_FREQ 2150 //Mhz
#define MIN_INPUT_FREQ 950  //Mhz
/***Data Types**********************************************************************/
#ifndef UINT8
#define UINT8      unsigned char
#endif
#ifndef UINT16
#define UINT16     unsigned short
#endif
#ifndef UINT32
#define UINT32     unsigned int
#endif

/***enumerator structures************************************************************/


typedef enum
{							// I2C Write Address setting by Hardware pins
	ADDR1_L_ADDR0_L = 0xC0, // Hardware Pin17 ADDR1 = GND , Pin14 ADDR0 = GND
	ADDR1_L_ADDR0_H = 0xC2, // Hardware Pin17 ADDR1 = GND , Pin14 ADDR0 = OPEN
	ADDR1_H_ADDR0_L = 0xC4, // Hardware Pin17 ADDR1 = OPEN, Pin14 ADDR0 = GND
	ADDR1_H_ADDR0_H = 0xC6	// Hardware Pin17 ADDR1 = OPEN, Pin14 ADDR0 = OPEN
}
TUNER_I2C_ADDRESS;


typedef enum          // BaseBand programmable amplifier gain setting
{
	PGA_0dB     =0,
	PGA_1_5dB   =1,
	PGA_3dB     =2,
	PGA_4_5dB   =3,
	PGA_6dB     =4,
	PGA_7_5dB   =5,
	PGA_9dB     =6,
	PGA_10_5dB  =7,
	PGA_12dB    =8
}
TUNER_PGA_GAIN;

typedef enum		 // BaseBand programmable amplifier output driving current setting
{
	PGA_500uA =0,
	PGA_1mA   =1,
	PGA_1_5mA =2,
	PGA_2mA   =3
}
TUNER_PGA_DRIVE_CURRENT;

typedef enum
{
	LOW       =0,
	MEDIUM    =1,
	HIGH      =2,
	MAXIMUM   =3
}
TUNER_XO_DRIVE_CURRENT;

typedef enum          // Crystal oscillator ON/OFF for stand-by
{
	XO_OFF = 0,
	XO_ON = 1
}
TUNER_XO_ENABLE;

typedef enum         // RF loop through ON/OFF
{
	RFLP_OFF = 0,
	RFLP_ON = 1
}
TUNER_RFLP_ENABLE;


typedef enum            // Fine-tune function setting
{
	FT_OFF      = 0x00,	// FT_block=0, FT_EN=0, FT_hold=0. The Gain is at best sensitivity gain.
	FT_ON       = 0x02,	// FT_block=0, FT_EN=1, FT_hold=0. Turn on to fine-tuned gain continuously between sensitivity and linearity.
	FT_Hold     = 0x03, // FT_block=0, FT_EN=1, FT_hold=1. Stop fine-tuning the gain and hold the current fine-tuned gain
	FT_Delay_ON = 0x06	// FT_block=1, FT_EN=1, FT_hold=0. Turn on Fine-tune Function after 12ms delay
}
TUNER_FINE_TUNE;

typedef enum            // Error code of tuner at different error status
{
	Tuner_No_Error    =0,
	Tuner_Error		  =1,
}
TUNER_ErrorCode;


/***tuner parameter structures************************************************************/
typedef struct
{
	/** Tuner Hardware variables**/

	UINT32                   crystal_khz;	 // (default 27000 KHz)
	TUNER_I2C_ADDRESS        I2C_ADDR;		 // Tuner I2C address at write mode

	/** Tuner Register varaibles**/

	TUNER_PGA_GAIN           PGA_Gain;       // RXout BaseBand Programmable gain
	TUNER_PGA_DRIVE_CURRENT  PGA_Current;    // RXout BaseBand PGA output driving current
	TUNER_XO_DRIVE_CURRENT   XO_Current;     // Crystal oscillator driving current
	TUNER_XO_ENABLE          XO_EN;			 // Crystal oscillator ON/OFF
	TUNER_RFLP_ENABLE        RFLP_EN;		 // Loopthrough ON/OFF

	TUNER_FINE_TUNE          FT;	         // Front-end Gain fine tune between linearity and sensitvity

	UINT8			         blind_scan;     // System Blind Scan indication for fine-tune function
}
TunerPara, *pTunerPara;


/***Tuner Control Functions********************************************************************/
/*
TUNER_ErrorCode MDrv_Tuner_Initial (void);
TUNER_ErrorCode Tuner_Register_Initial (void);
TUNER_ErrorCode Tuner_Set_Channel_Frequency (pTunerPara pAVtuner_t, UINT32 channel_freq_mhz);
TUNER_ErrorCode Tuner_Set_Filter_Bandwith (pTunerPara pAVtuner_t, UINT32 filter_BW_khz);
TUNER_ErrorCode Tuner_Set_RFLP (pTunerPara pAVtuner_t);
TUNER_ErrorCode Tuner_Set_Fine_Tune (pTunerPara pAVtuner_t);
TUNER_ErrorCode Tuner_Set_RXout_PGA (pTunerPara pAVtuner_t);
*/

/***Customer define Functions********************************************************************/
/*
TUNER_ErrorCode Tuner_I2C_Write (pTunerPara pAVtuner_t, UINT8 reg_start, UINT8* buff, UINT8 length);
TUNER_ErrorCode Tuner_I2C_Read (pTunerPara pAVtuner_t, UINT8 reg_start, UINT8* buff, UINT8 length);
TUNER_ErrorCode Time_DELAY_MS (UINT32 ms);

bool MDrv_Tuner_SetFreq(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
*/
TUNER_ErrorCode Tuner_I2C_Write (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT8 reg_start, UINT8* buff, UINT8 length);
TUNER_ErrorCode Tuner_I2C_Read (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT8 reg_start, UINT8* buff, UINT8 length);
TUNER_ErrorCode Tuner_Set_Fine_Tune (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t);
TUNER_ErrorCode Time_DELAY_MS (UINT32 ms);
MS_BOOL MDrv_Tuner_AV2028_Initial (MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam);
TUNER_ErrorCode Tuner_Register_Initial (MS_U8 u8TunerIndex);
MS_BOOL MDrv_Tuner_AV2028_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
TUNER_ErrorCode Tuner_Set_Channel_Frequency (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 channel_freq_mhz);
TUNER_ErrorCode Tuner_Set_Filter_Bandwith (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 filter_BW_khz);
MS_BOOL MDrv_Tuner_AV2028_CheckLock(MS_U8 u8TunerIndex);
TUNER_ErrorCode Tuner_Set_RFLP (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t);



#ifdef _CPLUSPLUS
}
#endif

#endif //_AV2026_H
