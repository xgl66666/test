

#ifndef _DMD_PARAM_H_
#define _DMD_PARAM_H_
#define RESERVED 0x00
#define ENABLE 1
#define DISABLE 0
//------------------------------DEMOD_IQ-----------------
#define DEMOD_IQ_I_IN               0
#define DEMOD_IQ_Q_IN               1
//------------------------------DEMOD_DVBT_timeout//msb122x-----------------
#define DVBT_TPS_timeout   1500//2000
#define DVBT_FEC_timeout   5000//2200//5000

//------------------------------DEMOD_DVBT2_timeout/msb1238/msb123x/msb1236c-
#define DVBT2_TPS_timeout   600//2000
#define DVBT2_FEC_timeout   6000//2200//5000

//------------------------------ISDBT/msb1400-----------------
#define ISDBT_ChExist_timeout   300//  msb1400:500 , Keres:300
#define ISDBT_FEC_timeout   2000//msb1400:6000 , Keres:2000

//------------------------------DEMOD_DVBC_timeout/DEMOD_DVBS_timeout--------
#define AutoSymbol_Timeout   10000
#define FixSymbol_AutoQam_Timeout   2000
#define FixSymbol_FixQam_Timeout   2000

//------------------------------DTMB/msb1238-----------------
#define DTMB_PreLock_timeout   300
#define DTMB_PNMLock_timeout   1200
#define DTMB_FEC_timeout   5000
//------------------------------DEMOD_DVBC_timeout-----------------
#define AutoSymbol_Timeout   10000
#define FixSymbol_AutoQam_Timeout   2000
#define FixSymbol_FixQam_Timeout   2000
//------------------------------INTERNAL DVBC DMD PARAMETERS-----------------
#define INIT_TBL_VERIOSN 4
#define DMD_DVBC_TS_CLK  0x15
#define DMD_DVBC_TRI_STATE 1
#define DMD_DVBC_PAD_SEL   0 //0=Normal, 1=analog pad
#define DMD_DVBC_PGA_EN  DISABLE // 0=disable, 1=enable
#define DMD_DVBC_PGA_GAIN 5
#ifdef SET_DSP_REG
#define DMD_DVBC_SET_DSP_REG ENABLE
#else
#define DMD_DVBC_SET_DSP_REG DISABLE
#endif
#endif

