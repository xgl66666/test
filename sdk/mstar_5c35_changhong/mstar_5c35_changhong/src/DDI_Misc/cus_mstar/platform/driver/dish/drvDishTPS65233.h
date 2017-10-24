/*************************************************************************
* Copyright (c) 2013,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
*
* File Name£ºtps65233.h
* File Identify£ºreference to Configuration Manager
* Summary£º Control the LNB chip Tps65233 by I2C
*
* Current Version£º1.0
* Author(s)£ºHowl.
* Example:
/----------------------------------------------------------------------------/
revision             author            reason             date
1.0                    Howl               Original          Sep, 24,2013
/----------------------------------------------------------------------------/
*************************************************************************/
#ifndef __TPS65233_H_
#define __TPS65233_H_

#ifdef __cplusplus
extern "C" {
#endif

    typedef unsigned char  BYTE;
    typedef unsigned int   BOOL;
    #define CSHDI_USB_TUNER_BASE  0x08
    typedef enum
    {
        Tuner_0,                            /**< tuner index 0*/
        Tuner_1,                            /**< tuner index 1*/
        Tuner_2,                            /**< tuner index 2*/
        Tuner_3,                            /**< tuner index 3*/
        Tuner_Mobile_0 = CSHDI_USB_TUNER_BASE, /**< usb tuner index 0 */
        Tuner_Mobile_1,                     /**< usb tuner index 1*/
        Tuner_Mobile_2,                     /**< usb tuner index 2*/
        Tuner_Mobile_3,                     /**< usb tuner index 3*/
        MAX_TUNER_NUM                   /**< maximun number of tuners */
    } CSHDITunerIndex;



    typedef enum
    {
        EM_TPS65233_LNB_DISABLE = 0,
        EM_TPS65233_LNB_V13_0 = 8, //output voltage =  13V
        EM_TPS65233_LNB_V13_4,     //output voltage =  13.4V
        EM_TPS65233_LNB_V13_8,     //output voltage =  13.8V
        EM_TPS65233_LNB_V14_2,     //output voltage =  14.2V
        EM_TPS65233_LNB_V18_0,     //output voltage =  18V
        EM_TPS65233_LNB_V18_6,     //output voltage =  18.6V
        EM_TPS65233_LNB_V19_2,     //output voltage =  19.2V
        EM_TPS65233_LNB_V19_8,     //output voltage =  19.8V

    }
    TPS65233_LNBPower_E;

    typedef enum
    {
        EM_TONE_GATE_OFF = 0,              //tone gate off
        EM_EXTERNAL_TONE_GATE_ON = 2,      //external 22K  && tone gate on
        EM_INTERNAL_TONE_GATE_ON           //internal 22K  && tone gate on
    } TPS65233_ToneGateMode_E;

    typedef enum
    {
        EM_TONE_ABOVE_VOUT = 0,
        EM_TONE_IN_THE_MIDDLE_OF_VOUT,
        EM_TONE_BELOW_VOUT
    } TPS65233_TonePosition_E;

    typedef enum
    {
        EM_CURRENT_LIMIT_400MA = 0,
        EM_CURRENT_LIMIT_600MA,
        EM_CURRENT_LIMIT_750MA,
        EM_CURRENT_LIMIT_1000MA
    } TPS65233_CurrentLimit_E;

    typedef enum
    {
        EM_CURRENT_LIMIT_SET_BY_REGISTER = 0,
        EM_CURRENT_LIMIT_SET_BY_EXTERNAL_RESISTOR
    } TPS65233_CLController_E;

    typedef enum
    {
        EM_I2C_DISABLE,
        EM_I2C_ENABLE
    } TPS65233_I2CContrlMode_E;

    typedef struct
    {
        BYTE m_ucTemperature;   //if die temperature T>125¡æ0:if die temperature T<125¡æ
        BYTE m_ucLDO_ON;        //LDO && boost converter ON 0:LDO OFF && boost converter ON
        BYTE m_ucTSD;           //ThermalShutdown occurs 0:ThermalShutdown not occurs
        BYTE m_ucOCP;           //OverCurrentProtection trigered 0:OverCurrentProtection realeased
        BYTE m_ucCableGood; //output current above 50ma 0:output current below 50ma
        BYTE m_ucVoutGood;      //whether LNB output voltage in range or not 1:in range 0:out of range
    } TPS65233_Status_E;

    BOOL TPS65233_Init(CSHDITunerIndex eTunerIndex);
    BOOL TPS65233_ReadStatus(CSHDITunerIndex eTunerIndex, TPS65233_Status_E *psStatus);
    BOOL TPS65233_SetLNBPower(CSHDITunerIndex eTunerIndex, TPS65233_LNBPower_E eLNBPower);
    BOOL TPS65233_SetTonePosition(CSHDITunerIndex eTunerIndex, TPS65233_TonePosition_E eTonePosition);
    BOOL TPS65233_SetToneGateMode(CSHDITunerIndex eTunerIndex, TPS65233_ToneGateMode_E eToneGateMode);

#ifdef __cplusplus
}
#endif

#endif

