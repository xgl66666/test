
/*****************************************************************************
 *            COPYRIGHT 2009 IWEDIA TECHNOLOGIES       *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_HDMI_CEC_OPERANDS MODULE
 *
 * FILE NAME:   TDAL_HDMI_CEC.H
 *   $URL: ... $
 *   $Date: ... $
 *   $Rev: ... $
 *
 * DESCRIPTION
 *
 * - .............. -
 *
 *
 *****************************************************************************/


#ifndef _TDAL_HDMI_CEC_OPERANDS_H_
#define _TDAL_HDMI_CEC_OPERANDS_H_

/*****************************************************************************/
/*  Includes                            */
/*****************************************************************************/

#include "tdal_hdmi_cec.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*  Defines                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Macros                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Typedefs                            */
/*****************************************************************************/


/* CEC OPERANDS */
   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_ABORT_REASON_UNRECOGNIZED_OPCODE      =  0x00,
    eTDAL_HDMI_CEC_OPERAND_ABORT_REASON_NOT_IN_CORRECT_MODE_TO_RESPOND  =  0x01,
    eTDAL_HDMI_CEC_OPERAND_ABORT_REASON_CANNOT_PROVIDE_SOURCE      =  0x02,
    eTDAL_HDMI_CEC_OPERAND_ABORT_REASON_INVALID_OPERAND        =  0x03,
    eTDAL_HDMI_CEC_OPERAND_ABORT_REASON_REFUSED            =  0x04
   }tTDAL_HDMI_CEC_OperandAbortReason;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_ANALOGUE_BROADCAST_TYPE_CABLE    = 0x00,
    eTDAL_HDMI_CEC_OPERAND_ANALOGUE_BROADCAST_TYPE_SATELLITE   = 0x01,
    eTDAL_HDMI_CEC_OPERAND_ANALOGUE_BROADCAST_TYPE_TERRESTRIAL = 0x02,
   }tTDAL_HDMI_CEC_OperandAnalogueBroadcastType;

   typedef uint16_t tTDAL_HDMI_CEC_OperandAnalogueFrequency;

   typedef uint8_t tTDAL_HDMI_CEC_OperandASCIIDigit;

   typedef uint8_t tTDAL_HDMI_CEC_OperandASCII;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_RATE_CONTROL_OFF   =  0x00,
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_STANDARD_RATE_WRC   =  0x01,
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_FAST_RATE_WRC     =  0x02,
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_SLOW_RATE_WRC     =  0x03,
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_STANDARD_RATE_NRC   =  0x04,
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_FAST_RATE_NRC     =  0x05,
    eTDAL_HDMI_CEC_OPERAND_AUDIO_RATE_SLOW_RATE_NRC     =  0x06
   }tTDAL_HDMI_CEC_OperandAudioRate;

   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_AUDIO_MUTE_STATUS_OFF   =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_AUDIO_MUTE_STATUS_ON   =  0x01
   }tTDAL_HDMI_CEC_SubOperandAudioMuteStatus;

   typedef uint8_t tTDAL_HDMI_CEC_SubOperandAudioVolumeStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandAudioMuteStatus   AudioMuteStatus;
    tTDAL_HDMI_CEC_SubOperandAudioVolumeStatus  AudioVolumeStatus;
   }tTDAL_HDMI_CEC_OperandAudioStatus;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_BOOLEAN_FALSE = 0x00,
    eTDAL_HDMI_CEC_OPERAND_BOOLEAN_TRUE  = 0x01
   }tTDAL_HDMI_CEC_OperandBoolean;

   typedef uint8_t tTDAL_HDMI_CEC_OperandBroadcastSystem;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_CEC_VERSION_1_1   =  0x00,
    eTDAL_HDMI_CEC_OPERAND_CEC_VERSION_1_2   =  0x01,
    eTDAL_HDMI_CEC_OPERAND_CEC_VERSION_1_2A  =  0x02,
    eTDAL_HDMI_CEC_OPERAND_CEC_VERSION_1_3   =  0x03,
    eTDAL_HDMI_CEC_OPERAND_CEC_VERSION_1_3A  =  0x04
   }tTDAL_HDMI_CEC_OperandCECVersion;

   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_CHANNEL_NUMBER_FORMAT_1_PART = 0x01,
    eTDAL_HDMI_CEC_SUB_OPERAND_CHANNEL_NUMBER_FORMAT_2_PART = 0x02
   }tTDAL_HDMI_CEC_SubOperandChannelNumberFormat;

   typedef uint16_t tTDAL_HDMI_CEC_SubOperandMajorChannelNumber;

   typedef uint16_t tTDAL_HDMI_CEC_SubOperandMinorChannelNumber;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandChannelNumberFormat  ChannelNumberFormat;
    tTDAL_HDMI_CEC_SubOperandMajorChannelNumber   MajorChannelNumber;
    tTDAL_HDMI_CEC_SubOperandMinorChannelNumber   MinorChannelNumber;
   }tTDAL_HDMI_CEC_OperandChannelIdentifier;

   typedef uint8_t tTDAL_HDMI_CEC_OperandDayOfMonth;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_DECK_CONTROL_MODE_SKIP_FORWARD_WIND   =  0x01,
    eTDAL_HDMI_CEC_OPERAND_DECK_CONTROL_MODE_SKIP_REVERSE_REWIND   =  0x02,
    eTDAL_HDMI_CEC_OPERAND_DECK_CONTROL_MODE_STOP         =  0x03, 
    eTDAL_HDMI_CEC_OPERAND_DECK_CONTROL_MODE_EJECT        =  0x04
   }tTDAL_HDMI_CEC_OperandDeckControlMode;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_PLAY         =  0x11,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_RECORD        =  0x12,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_PLAY_REVERSE      =  0x13, 
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_STILL         =  0x14,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_SLOW         =  0x15,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_SLOW_REVERSE      =  0x16,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_FAST_FORWARD      =  0x17,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_FAST_REVERSE      =  0x18,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_NO_MEDIA       =  0x19,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_STOP         =  0x1A,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_SKIP_FORWARD_WIND   =  0x1B,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_SKIP_REVERSE_REWIND   =  0x1C,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_INDEX_SEARCH_FORWARD   =  0x1D,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_INDEX_SEARCH_REVERSE   =  0x1E,
    eTDAL_HDMI_CEC_OPERAND_DECK_INFO_OTHER_STATUS      =  0x1F
   }tTDAL_HDMI_CEC_OperandDeckInfo;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_DEVICE_TYPE_TV         =  0x00,
    eTDAL_HDMI_CEC_OPERAND_DEVICE_TYPE_RECORDING_DEVICE   =  0x01,
    eTDAL_HDMI_CEC_OPERAND_DEVICE_TYPE_RESERVED      =  0x02,
    eTDAL_HDMI_CEC_OPERAND_DEVICE_TYPE_TUNER       =  0x03,
    eTDAL_HDMI_CEC_OPERAND_DEVICE_TYPE_PLAYBACK_DEVICE   =  0x04,
    eTDAL_HDMI_CEC_OPERAND_DEVICE_TYPE_AUDIO_SYSTEM     =  0x05
   }tTDAL_HDMI_CEC_OperandDeviceType;


   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_SERVICE_IDENTIFICATION_METHOD_DIGITAL_IDS  =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_SERVICE_IDENTIFICATION_METHOD_CHANNEL   =  0x01
   }tTDAL_HDMI_CEC_SubOperandServiceIdentificationMethod;

   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ARIB_GENERIC   =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ATSC_GENERIC   =  0x01,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_DVB_GENERIC    =  0x02,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ARIB_BS      =  0x08,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ARIB_CS      =  0x09,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ARIB_T      =  0x0A,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ATSC_CABLE    =  0x10,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ATSC_SATELLITE   =  0x11,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_ATSC_TERRESTRIAL  =  0x12,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_DVB_C      =  0x18,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_DVB_S      =  0x19,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_DVB_S2      =  0x1A,
    eTDAL_HDMI_CEC_SUB_OPERAND_DIGITAL_BROADCAST_SYSTEM_DVB_T      =  0x1B
   }tTDAL_HDMI_CEC_SubOperandDigitalBroadcastSystem;

   typedef struct
   {
    uint16_t TransportStreamID;
    uint16_t ServiceID;
    uint16_t OriginalNetworkID;
   }tTDAL_HDMI_CEC_SubOperandARIBData; 

   typedef struct
   {
    uint16_t TransportStreamID;
    uint16_t ProgramNumber;
    uint16_t Reserve;
   }tTDAL_HDMI_CEC_SubOperandATSCData; 

   typedef struct
   {
    uint16_t TransportStreamID;
    uint16_t ServiceID;
    uint16_t OriginalNetworkID;
   }tTDAL_HDMI_CEC_SubOperandDVBData; 

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandChannelIdentifier   ChannelIdentifier;
    uint16_t           Reserved;
   }tTDAL_HDMI_CEC_SubOperandChannelData; 


   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandARIBData   ARIBData;
    tTDAL_HDMI_CEC_SubOperandATSCData   ATSCData;
    tTDAL_HDMI_CEC_SubOperandDVBData   DVBData;
    tTDAL_HDMI_CEC_SubOperandChannelData  ChannelData;
   }tTDAL_HDMI_CEC_SubOperandServiceIdentification;


   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandServiceIdentificationMethod   ServiceIdentificationMethod;
    tTDAL_HDMI_CEC_SubOperandDigitalBroadcastSystem    DigitalBroadcastSystem;
    tTDAL_HDMI_CEC_SubOperandServiceIdentification     ServiceIdentification;
   }tTDAL_HDMI_CEC_OperandDigitalServiceIdentification;


   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_DISPLAY_CONTROL_DISPLAY_FOR_DEFAULT_TIME    =  0x00,
    eTDAL_HDMI_CEC_OPERAND_DISPLAY_CONTROL_DISPLAY_UNTIL_CLEARED      =  0x20,
    eTDAL_HDMI_CEC_OPERAND_DISPLAY_CONTROL_DISPLAY_CLEAR_PREVIOUS_MESSAGE  =  0x40,
    eTDAL_HDMI_CEC_OPERAND_DISPLAY_CONTROL_RESERVED_FOR_FUTUR_USE     =  0x60
   }tTDAL_HDMI_CEC_OperandDisplayControl;

   typedef uint8_t tTDAL_HDMI_CEC_OperandMinute;

   typedef uint8_t tTDAL_HDMI_CEC_SubOperandDurationHours;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandDurationHours  DurationHours;
    tTDAL_HDMI_CEC_OperandMinute      Minute;
   }tTDAL_HDMI_CEC_OperandDuration;

   typedef uint8_t tTDAL_HDMI_CEC_SubOperandPortID;

   typedef tTDAL_HDMI_CEC_SubOperandPortID tTDAL_HDMI_CEC_OperandPhysicalAddress[4];

   typedef tTDAL_HDMI_CEC_OperandPhysicalAddress tTDAL_HDMI_CEC_OperandExternalPhysicalAddress; 

   typedef uint8_t tTDAL_HDMI_CEC_OperandExternalPlug;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_EXTERNAL_SOURCE_SPECIFIER_EXTERNAL_PLUG       = 0x04,
    eTDAL_HDMI_CEC_OPERAND_EXTERNAL_SOURCE_SPECIFIER_EXTERNAL_PHYSICAL_ADDRESS  = 0x05
   } tTDAL_HDMI_CEC_OperandExternalSourceSpecifier;

   typedef uint8_t tTDAL_HDMI_CEC_OperandHour;

   typedef uint8_t tTDAL_HDMI_CEC_OperandFeatureOpcode;

   typedef tTDAL_HDMI_CEC_OperandASCII tTDAL_HDMI_CEC_OperandLanguage[3];

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_MENU_REQUEST_TYPE_ACTIVATE   =  0x00,
    eTDAL_HDMI_CEC_OPERAND_MENU_REQUEST_TYPE_DEACTIVATE =  0x01,
    eTDAL_HDMI_CEC_OPERAND_MENU_REQUEST_TYPE_QUERY   =  0x02
   }tTDAL_HDMI_CEC_OperandMenuRequestType;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_MENU_STATE_ACTIVATE   =  0x00,
    eTDAL_HDMI_CEC_OPERAND_MENU_STATE_DEACTIVATE =  0x01,
   }tTDAL_HDMI_CEC_OperandMenuState;

   typedef uint8_t tTDAL_HDMI_CEC_OperandMonthOfYear; 

   typedef tTDAL_HDMI_CEC_OperandPhysicalAddress tTDAL_HDMI_CEC_OperandNewAddress;

   typedef tTDAL_HDMI_CEC_OperandPhysicalAddress tTDAL_HDMI_CEC_OperandOriginalAddress; 

   typedef struct
   {
    uint8_t       ASCIILength;
    tTDAL_HDMI_CEC_OperandASCII  ASCII[14];
   }tTDAL_HDMI_CEC_OperandOSDName;

   typedef struct
   {
    uint8_t       ASCIILength;
    tTDAL_HDMI_CEC_OperandASCII  ASCII[13];
   }tTDAL_HDMI_CEC_OperandOSDString;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_PLAY_FORWARD       =  0x24,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_PLAY_REVERSE       =  0x20,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_PLAY_STILL         =  0x25,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_FAST_FORWARD_MIN_SPEED   =  0x05,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_FAST_FORWARD_MEDIUM_SPEED   =  0x06,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_FAST_FORWARD_MAX_SPEED   =  0x07,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_FAST_REVERSE_MIN_SPEED   =  0x09,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_FAST_REVERSE_MEDIUM_SPEED   =  0x0A,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_FAST_REVERSE_MAX_SPEED   =  0x0B,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_SLOW_FORWARD_MIN_SPEED   =  0x15,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_SLOW_FORWARD_MEDIUM_SPEED   =  0x16,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_SLOW_FORWARD_MAX_SPEED   =  0x17,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_SLOW_REVERSE_MIN_SPEED   =  0x19,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_SLOW_REVERSE_MEDIUM_SPEED   =  0x1A,
    eTDAL_HDMI_CEC_OPERAND_PLAY_MODE_SLOW_REVERSE_MAX_SPEED   =  0x1B
   }tTDAL_HDMI_CEC_OperandPlayMode;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_POWER_STATUS_ON            =  0x00,
    eTDAL_HDMI_CEC_OPERAND_POWER_STATUS_STANDBY          =  0x01,
    eTDAL_HDMI_CEC_OPERAND_POWER_STATUS_IN_TRANSITION_STANDBY_TO_ON  =  0x02,
    eTDAL_HDMI_CEC_OPERAND_POWER_STATUS_IN_TRANSITION_ON_TO_STANBY   =  0x03,
   }tTDAL_HDMI_CEC_OperandPowerStatus;


   typedef struct
   {
    uint8_t       ASCIILength;
    tTDAL_HDMI_CEC_OperandASCII  ASCII[14];
   }tTDAL_HDMI_CEC_OperandProgramTitleString;

   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_RECORD_SOURCE_TYPE_OWN_SOURCE        =  0x01,
    eTDAL_HDMI_CEC_SUB_OPERAND_RECORD_SOURCE_TYPE_DIGITAL_SERVICE      =  0x02,
    eTDAL_HDMI_CEC_SUB_OPERAND_RECORD_SOURCE_TYPE_ANALOGUE_SERVICE      =  0x03,
    eTDAL_HDMI_CEC_SUB_OPERAND_RECORD_SOURCE_TYPE_EXTERNAL_PLUG       =  0x04,
    eTDAL_HDMI_CEC_SUB_OPERAND_RECORD_SOURCE_TYPE_EXTERNAL_PHYSICAL_ADDRESS  =  0x05
   }tTDAL_HDMI_CEC_SubOperandRecordSourceType;

   typedef struct
   {

   }tTDAL_HDMI_CEC_OperandStructRecordSource1;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDigitalServiceIdentification  DigitalServiceIdentification;
   }tTDAL_HDMI_CEC_OperandStructRecordSource2;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandAnalogueBroadcastType  AnalogueBroadcastType;
    tTDAL_HDMI_CEC_OperandAnalogueFrequency   AnalogueFrequency;
    tTDAL_HDMI_CEC_OperandBroadcastSystem    BroadcastSystem;
   }tTDAL_HDMI_CEC_OperandStructRecordSource3;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandExternalPlug    ExternalPlug;
   }tTDAL_HDMI_CEC_OperandStructRecordSource4;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandExternalPhysicalAddress  ExternalPhysicalAddress;
   }tTDAL_HDMI_CEC_OperandStructRecordSource5;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandRecordSourceType RecordSourceType;
    union
    {
      tTDAL_HDMI_CEC_OperandStructRecordSource1 StructRecordSource1;
      tTDAL_HDMI_CEC_OperandStructRecordSource2 StructRecordSource2;
      tTDAL_HDMI_CEC_OperandStructRecordSource3 StructRecordSource3;
      tTDAL_HDMI_CEC_OperandStructRecordSource4 StructRecordSource4;
      tTDAL_HDMI_CEC_OperandStructRecordSource5 StructRecordSource5;
    } StructRecordSource;
   }tTDAL_HDMI_CEC_OperandRecordSource;


   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_RECORDING_CURRENTLY_SELECTED_SOURCE       =  0x01,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_RECORDING_DIGITAL_SERVICE            =  0x02,  
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_RECORDING_ANALOGUE_SERVICE           =  0x03,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_RECORDING_EXTERNAL_INPUT            =  0x04,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_UNABLE_TO_RECORD_DIGITAL_SERVICE   =  0x05,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_UNABLE_TO_RECORD_ANALOGUE_SERVICE   =  0x06,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_UNABLE_TO_SELECT_REQUIRED_SERVICE   =  0x07,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_INVALID_EXTERNAL_PLUG_NUMBER     =  0x09,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_INVALID_EXTERNAL_PHYSICAL_ADDRESS   =  0x0A,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_CA_SYSTEM_NOT_SUPPORTED       =  0x0B,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_NO_OR_INSUFFICIENT_CA_INTITLEMENTS   =  0x0C,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_NOT_ALLOWED_TO_COPY_SOURCE      =  0x0D,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_NO_FURTHER_COPIES_ALLOWED      =  0x0E,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_NO_MEDIA             =  0x10,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_PLAYING              =  0x11,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_ALREADY_RECORDING         =  0x12,  
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_MEDIA_PROTECTED          =  0x13,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_NO_SOURCE_SIGNAL         =  0x14,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_MEDIA_PROBLEM           =  0x15,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_NOT_ENOUGH_SPACE_AVAILABLE      =  0x16,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_PARENTAL_LOCK_ON         =  0x17,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_RECORDING_RECORDING_TERMINATED_NORMALLY      =  0x1A,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_RECORDING_HAS_ALREADY_TERMINATED         =  0x1B,
    eTDAL_HDMI_CEC_OPERAND_RECORD_STATUS_INFO_NO_RECORDING_OTHER_REASON            =  0x1F
   }tTDAL_HDMI_CEC_OperandRecordStatusInfo;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_SUNDAY    =  0x01,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_MONDAY    =  0x02,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_TUESDAY    =  0x04,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_WEDNESDAY   =  0x08,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_THURSDAY   =  0x10,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_FRIDAY    =  0x20,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_SATURDAY   =  0x40,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_ONCE_ONLY   =  0x00,
    eTDAL_HDMI_CEC_OPERAND_RECORDING_SEQUENCE_BIT_RESERVED  =  0x08
   }tTDAL_HDMI_CEC_OperandRecordingSequence;

   typedef uint8_t tTDAL_HDMI_CEC_OperandReservedBit;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_STATUS_REQUEST_ON   =  0x01,
    eTDAL_HDMI_CEC_OPERAND_STATUS_REQUEST_OFF   =  0x02,
    eTDAL_HDMI_CEC_OPERAND_STATUS_REQUEST_ONCE  =  0x03
   }tTDAL_HDMI_CEC_OperandStatusRequest;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandHour Hour;
    tTDAL_HDMI_CEC_OperandMinute Minute;
   }tTDAL_HDMI_CEC_OperandTime;

   typedef tTDAL_HDMI_CEC_OperandTime tTDAL_HDMI_CEC_OperandStartTime;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_SYSTEM_AUDIO_STATUS_OFF = 0x00,
    eTDAL_HDMI_CEC_OPERAND_SYSTEM_AUDIO_STATUS_ON  = 0x01
   }tTDAL_HDMI_CEC_OperandSystemAudioStatus;

   typedef enum
   {
    eTDAL_HDMI_CEC_OPERAND_TIMER_CLEARED_STATUS_DATA_TIMER_NOT_CLEARED_RECORDING     =  0x00,
    eTDAL_HDMI_CEC_OPERAND_TIMER_CLEARED_STATUS_DATA_TIMER_NOT_CLEARED_NO_MATCHING    =  0x01,
    eTDAL_HDMI_CEC_OPERAND_TIMER_CLEARED_STATUS_DATA_TIMER_NOT_CLEARED_NO_INFO_AVAILABLE  =  0x02,
    eTDAL_HDMI_CEC_OPERAND_TIMER_CLEARED_STATUS_DATA_TIMER_NOT_CLEARED_TIMER_CLEARED   =  0x80
   }tTDAL_HDMI_CEC_OperandTimerClearedStatusData;


   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_TIMER_OVERLAP_WARNING_NO_OVERLAP      =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_TIMER_OVERLAP_WARNING_TIMER_BLOCKS_OVERLAP  =  0x01
   }tTDAL_HDMI_CEC_SubOperandTimerOverlapWarning;

   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_MEDIA_INFO_MEDIA_PRESENT_AND_NOT_PROTECTED  =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_MEDIA_INFO_MEDIA_PRESENT_BUT_PROTECTED   =  0x01,
    eTDAL_HDMI_CEC_SUB_OPERAND_MEDIA_INFO_MEDIA_NOT_PRESENT       =  0x02,
    eTDAL_HDMI_CEC_SUB_OPERAND_MEDIA_INFO_MEDIA_FUTURE_USE        =  0x03
   }tTDAL_HDMI_CEC_SubOperandMediaInfo;


   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_PROGRAMMED_INDICATOR_NOT_PROGRAMMED  =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_PROGRAMMED_INDICATOR_PROGRAMMED   =  0x01
   }tTDAL_HDMI_CEC_SubOperandProgrammedIndicator;

   typedef uint8_t tTDAL_HDMI_CEC_SubOperandProgrammedInfo;

   typedef uint8_t tTDAL_HDMI_CEC_SubOperandNotProgrammedErrorInfo;

   typedef tTDAL_HDMI_CEC_OperandDuration tTDAL_HDMI_CEC_SubOperandDurationAvailable;

   typedef union
   {
    tTDAL_HDMI_CEC_SubOperandProgrammedIndicator   ProgrammedIndicator;
    tTDAL_HDMI_CEC_SubOperandProgrammedInfo      ProgrammedInfo;
    tTDAL_HDMI_CEC_SubOperandNotProgrammedErrorInfo   NotProgrammedErrorInfo;
    tTDAL_HDMI_CEC_SubOperandDurationAvailable    DurationAvailable;
   }tTDAL_HDMI_CEC_SubOperandUnionTimerProgrammedInfo;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandProgrammedInfo   ProgrammedInfo;
    tTDAL_HDMI_CEC_SubOperandDurationAvailable   DurationAvailable;
   }tTDAL_HDMI_CEC_SubOperandStructTimerProgrammedProgrammed;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandNotProgrammedErrorInfo  NotProgrammedErrorInfo;
    tTDAL_HDMI_CEC_SubOperandDurationAvailable    DurationAvailable;
   }tTDAL_HDMI_CEC_SubOperandStructTimerProgrammedNotProgrammed;


   typedef struct
   {
    bool                    IsDurationAvailable;
    tTDAL_HDMI_CEC_SubOperandProgrammedIndicator   ProgrammedIndicator;
    union
    {
      tTDAL_HDMI_CEC_SubOperandStructTimerProgrammedProgrammed TimerProgrammedProgrammed;
      tTDAL_HDMI_CEC_SubOperandStructTimerProgrammedNotProgrammed TimerProgrammedNotProgrammed;
    }TimerProgrammedProgrammedOrNotProgrammed;

   }tTDAL_HDMI_CEC_SubOperandTimerProgrammedInfo;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandTimerOverlapWarning   TimerOverlapWarning;
    tTDAL_HDMI_CEC_SubOperandMediaInfo      MediaInfo;
    tTDAL_HDMI_CEC_SubOperandTimerProgrammedInfo   TimerProgrammedInfo;
   }tTDAL_HDMI_CEC_OperandTimerStatusData;

   typedef uint8_t tTDAL_HDMI_CEC_SubOperandRecordingFlag;

   typedef enum
   {
    eTDAL_HDMI_CEC_SUB_OPERAND_TUNER_DISPLAY_INFO_DISPLAYING_DIGITAL_TUNER   =  0x00,
    eTDAL_HDMI_CEC_SUB_OPERAND_TUNER_DISPLAY_INFO_NO_DISPLAYING_TUNER    =  0x01,
    eTDAL_HDMI_CEC_SUB_OPERAND_TUNER_DISPLAY_INFO_DISPLAYING_ANALOGUE_TUNER  =  0x02
   } tTDAL_HDMI_CEC_SubOperandTunerDisplayInfo;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandAnalogueBroadcastType     AnalogueBroadcastType;
    tTDAL_HDMI_CEC_OperandAnalogueFrequency      AnalogueFrequency;
    tTDAL_HDMI_CEC_OperandBroadcastSystem       BroadcastSystem;
   }tTDAL_HDMI_CEC_OperandStructTunerDeviceInfoAnalogue;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDigitalServiceIdentification  DigitalServiceIdentification;
   }tTDAL_HDMI_CEC_OperandStructTunerDeviceInfoDigital;

   typedef struct
   {
    tTDAL_HDMI_CEC_SubOperandRecordingFlag       RecordingFlag;
    tTDAL_HDMI_CEC_SubOperandTunerDisplayInfo      TunerDisplayInfo;
    union
    {
      tTDAL_HDMI_CEC_OperandStructTunerDeviceInfoAnalogue  TunerDeviceInfoAnalogue;
      tTDAL_HDMI_CEC_OperandStructTunerDeviceInfoDigital   TunerDeviceInfoDigital;
    } TunerDeviceInfoAnalogueOrDigital;
   }tTDAL_HDMI_CEC_OperandTunerDeviceInfo;



/* CEC USER CONTROL CODES*/

   typedef enum
   {
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SELECT          =   0x00,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_UP            =   0x01,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_DOWN           =   0x02,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_LEFT           =   0x03,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_RIGHT           =   0x04,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_RIGHT_UP         =   0x05,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_RIGHT_DOWN         =   0x06,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_LEFT_UP         =   0x07,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_LEFT_DOWN         =   0x08,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_ROOT_MENU         =   0x09,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SETUP_MENU         =   0x0A,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_CONTENTS_MENU       =   0x0B,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_FAVORITE_MENU       =   0x0C,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_EXIT           =   0x0D,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_0         =   0x20,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_1         =   0x21,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_2         =   0x22,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_3         =   0x23,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_4         =   0x24,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_5         =   0x25,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_6         =   0x26,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_7         =   0x27,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_8         =   0x28,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NUMBER_9         =   0x29,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_DOT            =   0x2A,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_ENTER           =   0x2B,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_CLEAR           =   0x2C,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_NEXT_FAVORITE       =   0x2F,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_CHANNEL_UP         =   0x30,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_CHANNEL_DOWN       =   0x31,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PREVIOUS_CHANNEL      =   0x32,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SOUND_SELECT       =   0x33,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_INPUT_SELECT       =   0x34,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_DISPLAY_INFORMATION     =   0x35,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_HELP           =   0x36,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PAGE_UP         =   0x37,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PAGE_DOWN         =   0x38,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_POWER           =   0x40,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_VOLUME_UP         =   0x41,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_VOLUME_DOWN        =   0x42,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_MUTE           =   0x43,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PLAY           =   0x44,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_STOP           =   0x45,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PAUSE           =   0x46,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_RECORD          =   0x47,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_REWIND          =   0x48,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_FAST_FORWARD       =   0x49,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_EJECT           =   0x4A,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_FORWARD         =   0x4B,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_BACKWARD         =   0x4C,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_STOP_RECORD        =   0x4D,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PAUSE_RECORD       =   0x4E,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_ANGLE           =   0x50,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SUB_PICTURE        =   0x51,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_VIDEO_ON_DEMAND      =   0x52,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_ELECTRONIC_PROGRAM_GUIDE   =   0x53,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_TIMER_PROGRAMMING      =   0x54,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_INITIAL_CONFIGURATION    =   0x55,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PLAY_FUNCTION       =   0x60,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PAUSE_PLAY_FUNCTION     =   0x61,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_RECORD_FUNCTION      =   0x62,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_PAUSE_RECORD_FUNCTION    =   0x63,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_STOP_FUNCTION       =   0x64,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_MUTE_FUNCTION       =   0x65,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_RESTORE_VOLUME_FUNCTION   =   0x66,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_TUNE_FUNCTION       =   0x67,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SELECT_MEDIA_FUNCTION    =   0x68,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SELECT_AV_INPUT_FUNCTION   =   0x69,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_SELECT_AUDIO_INPUT_FUNCTION =   0x6A,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_POWER_TOGGLE_FUNCTION    =   0x6B,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_POWER_OFF_FUNCTION     =   0x6C,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_POWER_ON_FUNCTION      =   0x6D,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_F1            =   0x71,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_F2            =   0x72,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_F3            =   0x73,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_F4            =   0x74,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_F5            =   0x75,
    eTDAL_HDMI_CEC_USER_CONTROL_CODE_DATA           =   0x76
   }tTDAL_HDMI_CEC_UserControlCode;

   typedef tTDAL_HDMI_CEC_UserControlCode tTDAL_HDMI_CEC_OperandUICommand;

   typedef uint8_t tTDAL_HDMI_CEC_OperandUIFunctionMedia;

   typedef uint8_t tTDAL_HDMI_CEC_OperandUISelectAVInput;

   typedef uint8_t tTDAL_HDMI_CEC_OperandUIFunctionSelectAudioInput;

   typedef uint32_t tTDAL_HDMI_CEC_OperandVendorID;

   typedef struct
   {
    uint8_t       ASCIILength;
    tTDAL_HDMI_CEC_OperandASCII  ASCII[14];
   }tTDAL_HDMI_CEC_OperandVendorSpecificData;

   typedef struct
   {
    uint8_t       ASCIILength;
    tTDAL_HDMI_CEC_OperandASCII  ASCII[14];
   }tTDAL_HDMI_CEC_OperandVendorSpecificRCCode;

   typedef void tTDAL_HDMI_CEC_OperandNULL;

/* CEC OPERAND CONTAINERS */

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPhysicalAddress PhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerActiveSource;

   typedef void* tTDAL_HDMI_CEC_OperandContainerImageViewOn;

   typedef void* tTDAL_HDMI_CEC_OperandContainerTextViewOn;

   typedef void* tTDAL_HDMI_CEC_OperandContainerRequestActiveSource;

   typedef void* tTDAL_HDMI_CEC_OperandContainerSource;

   typedef void* tTDAL_HDMI_CEC_OperandContainerRecordOff;

   typedef void* tTDAL_HDMI_CEC_OperandContainerRecordTvScreen;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGetCECVersion;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGivePhysicalAddress;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGetMenuLanguage;

   typedef void* tTDAL_HDMI_CEC_OperandContainerPollingMessage;

   typedef void* tTDAL_HDMI_CEC_OperandContainerTunerStepDecrement;

   typedef void* tTDAL_HDMI_CEC_OperandContainerTunerStepIncrement;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGiveDeviceVendorID;

   typedef void* tTDAL_HDMI_CEC_OperandContainerVendorRemoteButtonUp;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGiveOSDName;

   typedef void* tTDAL_HDMI_CEC_OperandContainerUserControlReleased;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGiveDevicePowerStatus;

   typedef void* tTDAL_HDMI_CEC_OperandContainerAbort;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGiveAudioStatus;

   typedef void* tTDAL_HDMI_CEC_OperandContainerGiveSystemAudioModeStatus;

   typedef void* tTDAL_HDMI_CEC_OperandContainerStandby;


   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPhysicalAddress PhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerInactiveSource;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandOriginalAddress  OriginalAddress;
    tTDAL_HDMI_CEC_OperandNewAddress    NewAddress;
   }tTDAL_HDMI_CEC_OperandContainerRoutingChange;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPhysicalAddress PhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerRoutingInformation;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPhysicalAddress PhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerSetStreamPath;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandRecordSource RecordSource;
   }tTDAL_HDMI_CEC_OperandContainerRecordOn;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandRecordStatusInfo RecordStatusInfo;
   }tTDAL_HDMI_CEC_OperandContainerRecordStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDayOfMonth       DayOfMonth;
    tTDAL_HDMI_CEC_OperandMonthOfYear      MonthOfYear;
    tTDAL_HDMI_CEC_OperandStartTime       StartTime;
    tTDAL_HDMI_CEC_OperandDuration       Duration;
    tTDAL_HDMI_CEC_OperandRecordingSequence    RecordingSequence;
    tTDAL_HDMI_CEC_OperandAnalogueBroadcastType   AnalogueBroadcastType;
    tTDAL_HDMI_CEC_OperandAnalogueFrequency    AnalogueFrequency;
    tTDAL_HDMI_CEC_OperandBroadcastSystem     BroadcastSystem;
   }tTDAL_HDMI_CEC_OperandContainerClearAnalogueTimer;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDayOfMonth         DayOfMonth;
    tTDAL_HDMI_CEC_OperandMonthOfYear         MonthOfYear;
    tTDAL_HDMI_CEC_OperandStartTime          StartTime;
    tTDAL_HDMI_CEC_OperandDuration           Duration;
    tTDAL_HDMI_CEC_OperandRecordingSequence       RecordingSequence;
    tTDAL_HDMI_CEC_OperandDigitalServiceIdentification   DigitalServiceIdentification;
   }tTDAL_HDMI_CEC_OperandContainerClearDigitalTimer;

   typedef union 
   {
    tTDAL_HDMI_CEC_OperandExternalPlug      ExternalPlug;
    tTDAL_HDMI_CEC_OperandExternalPhysicalAddress  ExternalPhysicalAddress;
   }tTDAL_HDMI_CEC_OperandExternalPlugOrPhysicalAddress;


   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDayOfMonth         DayOfMonth;
    tTDAL_HDMI_CEC_OperandMonthOfYear         MonthOfYear;
    tTDAL_HDMI_CEC_OperandStartTime          StartTime;
    tTDAL_HDMI_CEC_OperandDuration           Duration;
    tTDAL_HDMI_CEC_OperandRecordingSequence       RecordingSequence;
    tTDAL_HDMI_CEC_OperandExternalSourceSpecifier    ExternalSourceSpecifier;
    tTDAL_HDMI_CEC_OperandExternalPlugOrPhysicalAddress  ExternalPlugOrPhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerClearExternalTimer;



   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDayOfMonth       DayOfMonth;
    tTDAL_HDMI_CEC_OperandMonthOfYear      MonthOfYear;
    tTDAL_HDMI_CEC_OperandStartTime       StartTime;
    tTDAL_HDMI_CEC_OperandDuration       Duration;
    tTDAL_HDMI_CEC_OperandRecordingSequence    RecordingSequence;
    tTDAL_HDMI_CEC_OperandAnalogueBroadcastType   AnalogueBroadcastType;
    tTDAL_HDMI_CEC_OperandAnalogueFrequency    AnalogueFrequency;
    tTDAL_HDMI_CEC_OperandBroadcastSystem     BroadcastSystem;
   }tTDAL_HDMI_CEC_OperandContainerSetAnalogueTimer;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDayOfMonth         DayOfMonth;
    tTDAL_HDMI_CEC_OperandMonthOfYear         MonthOfYear;
    tTDAL_HDMI_CEC_OperandStartTime          StartTime;
    tTDAL_HDMI_CEC_OperandDuration           Duration;
    tTDAL_HDMI_CEC_OperandRecordingSequence       RecordingSequence;
    tTDAL_HDMI_CEC_OperandDigitalServiceIdentification   DigitalServiceIdentification;
   }tTDAL_HDMI_CEC_OperandContainerSetDigitalTimer;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDayOfMonth         DayOfMonth;
    tTDAL_HDMI_CEC_OperandMonthOfYear         MonthOfYear;
    tTDAL_HDMI_CEC_OperandStartTime          StartTime;
    tTDAL_HDMI_CEC_OperandDuration           Duration;
    tTDAL_HDMI_CEC_OperandRecordingSequence       RecordingSequence;
    tTDAL_HDMI_CEC_OperandExternalSourceSpecifier    ExternalSourceSpecifier;
    tTDAL_HDMI_CEC_OperandExternalPlugOrPhysicalAddress  ExternalPlugOrPhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerSetExternalTimer;


   typedef struct
   {
    tTDAL_HDMI_CEC_OperandProgramTitleString ProgramTitleString;
   }tTDAL_HDMI_CEC_OperandContainerSetTimerProgramTitle;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandTimerClearedStatusData TimerClearedStatusData;
   }tTDAL_HDMI_CEC_OperandContainerTimerClearedStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandTimerStatusData TimerStatusData;
   }tTDAL_HDMI_CEC_OperandContainerTimerStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandCECVersion CECVersion;
   }tTDAL_HDMI_CEC_OperandContainerCECVersion;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPhysicalAddress PhysicalAddress;
    tTDAL_HDMI_CEC_OperandDeviceType DeviceType;
   }tTDAL_HDMI_CEC_OperandContainerReportPhysicalAddress;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandLanguage Language;
   }tTDAL_HDMI_CEC_OperandContainerSetMenuLanguage;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDeckControlMode DeckControlMode;
   }tTDAL_HDMI_CEC_OperandContainerDeckControl;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDeckInfo DeckInfo;
   }tTDAL_HDMI_CEC_OperandContainerDeckStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandStatusRequest StatusRequest;
   }tTDAL_HDMI_CEC_OperandContainerGiveDeckStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPlayMode PlayMode;
   }tTDAL_HDMI_CEC_OperandContainerPlay;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandStatusRequest StatusRequest;
   }tTDAL_HDMI_CEC_OperandContainerGiveTunerDeviceStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandAnalogueBroadcastType  AnalogueBroadcastType;
    tTDAL_HDMI_CEC_OperandAnalogueFrequency   AnalogueFrequency;
    tTDAL_HDMI_CEC_OperandBroadcastSystem    BroadcastSystem;
   }tTDAL_HDMI_CEC_OperandContainerSelectAnalogueService;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDigitalServiceIdentification DigitalServiceIdentification;
   }tTDAL_HDMI_CEC_OperandContainerSelectDigitalService;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandTunerDeviceInfo TunerDeviceInfo;
   }tTDAL_HDMI_CEC_OperandContainerTunerDeviceStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandVendorID VendorID;
   }tTDAL_HDMI_CEC_OperandContainerDeviceVendorID;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandVendorSpecificData VendorSpecificData;
   }tTDAL_HDMI_CEC_OperandContainerVendorCommand;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandVendorID      VendorID;
    tTDAL_HDMI_CEC_OperandVendorSpecificData  VendorSpecificData;
   }tTDAL_HDMI_CEC_OperandContainerVendorCommandWithID;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandVendorSpecificRCCode VendorSpecificRCCode;
   }tTDAL_HDMI_CEC_OperandContainerVendorRemoteButtonDown;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandDisplayControl DisplayControl;
    tTDAL_HDMI_CEC_OperandOSDString   OSDString;
   }tTDAL_HDMI_CEC_OperandContainerSetOSDString;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandOSDName OSDName;
   }tTDAL_HDMI_CEC_OperandContainerSetOSDName;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandMenuRequestType MenuRequestType;
   }tTDAL_HDMI_CEC_OperandContainerMenuRequest;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandMenuState MenuState;
   }tTDAL_HDMI_CEC_OperandContainerMenuStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandUICommand UICommand;
   }tTDAL_HDMI_CEC_OperandContainerUserControlPressed;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPowerStatus PowerStatus;
   }tTDAL_HDMI_CEC_OperandContainerReportPowerStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandFeatureOpcode FeatureOpcode;
    tTDAL_HDMI_CEC_OperandAbortReason AbortReason;
   }tTDAL_HDMI_CEC_OperandContainerFeatureAbort;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandAudioStatus AudioStatus;
   }tTDAL_HDMI_CEC_OperandContainerReportAudioStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandSystemAudioStatus SystemAudioStatus;
   }tTDAL_HDMI_CEC_OperandContainerSetSystemAudioMode;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandPhysicalAddress PhysicalAddress;
   }tTDAL_HDMI_CEC_OperandContainerSystemAudioModeRequest;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandSystemAudioStatus SystemAudioStatus;
   }tTDAL_HDMI_CEC_OperandContainerSystemAudioModeStatus;

   typedef struct
   {
    tTDAL_HDMI_CEC_OperandAudioRate AudioRate;
   }tTDAL_HDMI_CEC_OperandContainerSetAudioRate;


/*****************************************************************************/
/*  Variables Declarations (IMPORT)                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Functions Definitions                       */
/*****************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_HDMI_CEC_OPERANDS_H_*/
