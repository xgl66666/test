#if(FRONTEND_DEMOD_TYPE ==LGDT3305_DEMOD)
#ifndef LGTUNER_H
#define LGTUNER_H

#define LGIT_3305_DEMOD_I2C_ADDR    0x1C
#define LGIT_3305_TUNER_I2C_ADDR    0xC0

typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_DCR_LOCK,
    COFDM_AGC_LOCK,
    COFDM_MODE_DET,

} COFDM_LOCK_STATUS;


void lgdt3305Init(int _mode);
int lgdt3305Carriercheck(int _mode);
int lgdt3305Errorcheck(void);
int lgdt3305inlockcheck(void);
int lgdt3305trlockcheck(void);
void lgdt3305Mpegout(int _mode);
void lgdt3305Mpegclk(int _mode);
void lgdt3305Mpegval(int _mode);
float lgdt3305_SignalStatus(int _mode);
int lgdt3305_MpegError(void);
void lgdt3305QamSpecInv(int _mode);
void Lgdt3305ResetBER(void);
double lgdt3305_Ber(void);

void TunerSetFrequency(unsigned int frequency);

void RegSetOneRegister(int reg_id, unsigned char _Data);
int RegGetOneRegister(int reg_id);
MS_BOOL MDrv_Demod_GetLockStatus(void);
#endif	/* LGTUNER_H */
#endif
