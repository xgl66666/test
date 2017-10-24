#ifndef _MXL603_TUNER_H_
#define _MXL603_TUNER_H_

#define EXAMPLE_DEV_MAX   2
#define MXL603_I2C_ADDR   0xC0

typedef enum
{
    CAB_IQ_NORMAL,                                                      ///< Normal
    CAB_IQ_INVERT                                                       ///< Inverse
} EN_CAB_IQ_MODE;

extern MXL_STATUS MxLWare603_API_ReqTunerRxPower(UINT8 devId, REAL32* rxPwrPtr);

#endif//_MXL603_TUNER_H_

