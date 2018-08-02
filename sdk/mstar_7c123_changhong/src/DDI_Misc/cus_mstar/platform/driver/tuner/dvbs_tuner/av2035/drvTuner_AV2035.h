

#ifndef _DRVTUNER_AV2035_H_
#define _DRVTUNER_AV2035_H_

#include "drvTuner.h"

#ifdef _CPLUSPLUS
extern "C" {
#endif

#define MAX_INPUT_FREQ 2150 //Mhz
#define MIN_INPUT_FREQ 950  //Mhz

/***Data Types**********************************************************************/
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;

/***enumerator structures************************************************************/
typedef enum
{							// I2C Write Address setting by Hardware pins
	ADDR1_L_ADDR0_L = 0xC0, // Hardware Pin17 ADDR1 = GND , Pin14 ADDR0 = GND 
	ADDR1_L_ADDR0_H = 0xC2, // Hardware Pin17 ADDR1 = GND , Pin14 ADDR0 = OPEN
	ADDR1_H_ADDR0_L = 0xC4, // Hardware Pin17 ADDR1 = OPEN, Pin14 ADDR0 = GND
	ADDR1_H_ADDR0_H = 0xC6	// Hardware Pin17 ADDR1 = OPEN, Pin14 ADDR0 = OPEN
} 
TUNER_I2C_ADDRESS; 

typedef enum           
{                      // For Tuner model name =1 only
	Differential = 0,  // IQ Differential mode
	Single = 1         // IQ Single end mode, only output at RXIP, RXQP
}
TUNER_IQ_MODE;

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

typedef enum        // power down setting for stand-by
{
	Wake_Up = 0,    //0 = Wake up Tuner
	Power_Down = 1  //1 = Power down Tuner	
}
TUNER_SOFTWARE_POWERDOWN;

typedef enum            // Error code of tuner at different error status 
{
	Tuner_No_Error    =0,
	Tuner_Error		  =1,	
	PLL_Lock_Error    =3,
	Filter_Lock_Error =5,
	I2C_Error         =9
}
TUNER_ErrorCode;


/***tuner parameter structures************************************************************/
typedef struct
{                                              
	/** Tuner Hardware variables**/
	UINT32                   crystal_khz;	 // (Unit:KHz) Tuner_crystal supporting range: 13000~37000KHz 	
	
	TUNER_I2C_ADDRESS        I2C_ADDR;		 // Tuner I2C address at write mode 

	/** Tuner Register varaibles**/
	TUNER_IQ_MODE            IQmode;		 // RXIQ differential out or Single out 
	TUNER_PGA_DRIVE_CURRENT  PGA_Current;    // RXout BaseBand PGA output driving current
	TUNER_XO_DRIVE_CURRENT   XO_Current;     // Crystal oscillator driving current 
	TUNER_XO_ENABLE          XO_EN;			 // Crystal oscillator ON/OFF	
	TUNER_RFLP_ENABLE        RFLP_EN;		 // Loopthrough ON/OFF   
	TUNER_SOFTWARE_POWERDOWN PD_EN;          // Tuner software power down ON/OFF  

	UINT8			         blind_scan;     // System Blind Scan indication for fine-tune function
}
TunerPara, *pTunerPara;


/***Tuner Control Functions********************************************************************/
TUNER_ErrorCode AV2035_Tuner_Parameter_Initial (pTunerPara pAVtuner);
TUNER_ErrorCode AV2035_Tuner_Register_Initial (MS_U8 u8TunerIndex);
TUNER_ErrorCode AV2035_Tuner_Set_Channel_Frequency (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 channel_freq_mhz);
TUNER_ErrorCode AV2035_Tuner_Set_Filter_Bandwith (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t, UINT32 filter_BW_khz);
TUNER_ErrorCode AV2035_Tuner_Set_RFLP_PD (UINT8 u8TunerIndex, pTunerPara pAVtuner);
TUNER_ErrorCode AV2035_Tuner_Set_RXout_PGA (MS_U8 u8TunerIndex, pTunerPara pAVtuner_t);

MS_BOOL MDrv_Tuner_AV2035_CheckLock(MS_U8 u8TunerIndex);
MS_BOOL MDrv_Tuner_AV2035_Initial (MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam);


/***Customer define Functions********************************************************************/
MS_BOOL AV2035_WriteReg (MS_U8 u8TunerIndex, MS_U8 u8SlaveID, UINT8 reg_start, UINT8* buff, UINT8 length);
MS_BOOL AV2035_ReadReg(MS_U8 u8TunerIndex, MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data);
TUNER_ErrorCode AV2035_Time_DELAY_MS (UINT32 ms);

#ifdef _CPLUSPLUS
}
#endif

#endif
