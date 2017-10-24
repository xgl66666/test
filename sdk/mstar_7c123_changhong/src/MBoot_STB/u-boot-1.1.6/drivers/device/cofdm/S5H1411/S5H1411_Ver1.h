#ifndef _S5H1412_H_
#define _S5H1412_H_

/* ------------------------------------
    Header Files
   ------------------------------------ */

#include <apiDigiTuner.h>
#define GPIO_FE_RST                     16


/* ------------------------------------
    Macro Definitions
   ------------------------------------ */

#ifdef _S5H1412_C_
#define INTERFACE
#else
#define INTERFACE  extern
#endif
/*
typedef enum
{
  DEMOD_MODE_NTSC,
  DEMOD_MODE_256QAM,
  DEMOD_MODE_64QAM,
  DEMOD_MODE_8VSB,
  DEMOD_MODE_NUM
} DEMOD_EN_DEMOD_MODE;
*/
typedef enum
{
  Samsung_SIGNAL_NO     = 0,  /* little or no input power detected  */
  Samsung_SIGNAL_WEAK       = 1,  /* some power detected.         */
  Samsung_SIGNAL_WEAK1    = 2,  /* some power detected.         */
  Samsung_SIGNAL_MODERATE     = 3,  /* lock achieved, SNR < 15 dB (approx)  */
  Samsung_SIGNAL_MODERATE1  = 4,  /* lock achieved, SNR < 15 dB (approx)  */
  Samsung_SIGNAL_MODERATE2  = 5,  /* lock achieved, SNR < 15 dB (approx)  */
  Samsung_SIGNAL_STRONG   = 6,  /* lock achieved, SNR < 24 dB (approx)  */
  Samsung_SIGNAL_STRONG1    = 7,  /* lock achieved, SNR < 24 dB (approx)  */
  Samsung_SIGNAL_STRONG2    = 8,  /* lock achieved, SNR < 24 dB (approx)  */
  Samsung_SIGNAL_VERY_STRONG  = 9,  /* lock achieved, SNR > 24 dB (approx)  */
  Samsung_SIGNAL_VERY_STRONG1 = 10, /* lock achieved, SNR > 24 dB (approx)  */
}SamsungSignalCondition_t;

typedef enum
{
  MPEG_MODE_PARAL,
  MPEG_MODE_SERIAL
} SamsungMpegMode_t;

typedef enum
{
  MPEG_CLK_INVERT,
  MPEG_CLK_NOT_INVERT
} SamsungMpegClk_t;

typedef struct
{
  U8 Reg;
  U16 Value;
} SamsungChipInit_t;


#define IIC_1411x_TOP_RegId     0x32
#define IIC_1411x_QAM_RegId     0x34

/*------------------------------------------------------//
// Function Prototype
//------------------------------------------------------*/

INTERFACE BOOLEAN Sec_1411xInitialize(void);
INTERFACE BOOLEAN Sec_1411xSoftReset(void);
INTERFACE BOOLEAN Sec_1411xRepeaterEnable(void);
INTERFACE BOOLEAN Sec_1411xRepeaterDisable(void);
INTERFACE BOOLEAN Sec_1411xVsbMode(void);
INTERFACE BOOLEAN Sec_1411xQamMode(void);
INTERFACE BOOLEAN Sec_1411x64QamMode(void);
INTERFACE BOOLEAN Sec_1411x256QamMode(void);
INTERFACE BOOLEAN Sec_1411xSleepOn(void);
INTERFACE BOOLEAN Sec_1411xSleepOff(void);
INTERFACE BOOLEAN  Sec_1411xCheckIsSleep(void);
INTERFACE BOOLEAN Sec_1411xAlterMpegMode(SamsungMpegMode_t mode);
INTERFACE BOOLEAN Sec_1411xAlterMpegClk(SamsungMpegClk_t clock);

INTERFACE U16 Sec_1411xSyncLock(void);
INTERFACE U16 Sec_1411xQAMEq_Lock(void);
INTERFACE U16 Sec_1411xVsbQAM_Master_Lock(void);
INTERFACE void Sec_1411xCheckSignalCondition(SamsungSignalCondition_t* pstatus);
INTERFACE double Sec_1411xVsbSnrResultCheck(void);
INTERFACE EN_CONSTEL_MODE MDrv_Demod_GetDemodMode(void);
#undef INTERFACE

#endif

