/*****************************************************************************
 *            COPYRIGHT 2009 IWEDIA TECHNOLOGIES       *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_HDMI_CEC MODULE
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


#ifndef _TDAL_HDMI_CEC_H_
#define _TDAL_HDMI_CEC_H_

/*****************************************************************************/
/*  Includes                            */
/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "tdal_hdmi_cec_operands.h"

/*****************************************************************************/
/*  Defines                             */
/*****************************************************************************/

/* From TV @ to RESERVED_2 @ / FreeToUse / Unregistred / Broadcast @out of scope of this define */
#define kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS      14   


/*****************************************************************************/
/*  Macros                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Typedefs                            */
/*****************************************************************************/

/* CEC Errors*/
typedef enum
{
   eTDAL_HDMI_CEC_NO_ERROR,       /* No Error returned */
   eTDAL_HDMI_CEC_NOT_INIT_ERROR,    /* TDAL_HDMI_CEC has not been initialised */
   eTDAL_HDMI_CEC_ALREADY_INIT_ERROR,   /* TDAL_HDMI_CEC already initialised */
   eTDAL_HDMI_CEC_BAD_PARAMETER_ERROR,   /* Bad parameter given to the function */
   eTDAL_HDMI_CEC_DRIVER_ERROR,     /* Low level driver returns an error */
   eTDAL_HDMI_CEC_NO_MEMORY_ERROR,    /* Not enough memory to perform action */
   eTDAL_HDMI_CEC_NOT_AVAILABLE_ERROR,   /* feature not available */
   eTDAL_HDMI_CEC_NO_LOGICAL_ADDRESS_ERROR,
   eTDAL_HDMI_CEC_UNKNOWN_ERROR     /* Unknown error */
}tTDAL_HDMI_CEC_Error;

/* CEC DEVICE TYPES */
typedef enum
{
   eTDAL_HDMI_CEC_DEVICE_TYPE_TV, 
   eTDAL_HDMI_CEC_DEVICE_TYPE_RECORDING_DEVICE,
   eTDAL_HDMI_CEC_DEVICE_TYPE_TUNER,
   eTDAL_HDMI_CEC_DEVICE_TYPE_PLAYBACK_DEVICE,
   eTDAL_HDMI_CEC_DEVICE_TYPE_AUDIO_SYSTEM
}tTDAL_HDMI_CEC_DeviceType;

/* CEC PHYSICAL ADDRESSES */
typedef uint8_t tTDAL_HDMI_CEC_PhysicalAddress[4];

/* CEC LOGICAL ADDRESSES */
typedef enum
{
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV         =  0x0001,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1  =  0x0002,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2  =  0x0004,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_1      =  0x0008,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_1   =  0x0010,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM    =  0x0020,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_2      =  0x0040,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_3      =  0x0080,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_2   =  0x0100,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3  =  0x0200,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_4      =  0x0400,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_3   =  0x0800,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_1     =  0x1000,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_2     =  0x2000,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_FREE_USE      =  0x4000,
   eTDAL_HDMI_CEC_LOGICAL_ADDRESS_UNREGISTERED_BROADCAST    =  0x0000   
}tTDAL_HDMI_CEC_LogicalAddress;

/* CEC POLLING MESSAGE */
typedef struct
{
   bool            bAckNack;      // TRUE = ACK / FALSE = NACK
   tTDAL_HDMI_CEC_LogicalAddress   eLogicalAddress;
}tTDAL_HDMI_CEC_PollingMessage;

/* CEC OPCODES */
typedef enum
{
   eTDAL_HDMI_CEC_OPCODE_FEATURE_ABORT         =   0x00,
   eTDAL_HDMI_CEC_OPCODE_IMAGE_VIEW_ON         =   0x04,
   eTDAL_HDMI_CEC_OPCODE_TUNER_STEP_INCREMENT       =   0x05,
   eTDAL_HDMI_CEC_OPCODE_TUNER_STEP_DECREMENT       =   0x06,
   eTDAL_HDMI_CEC_OPCODE_TUNER_DEVICE_STATUS       =   0x07,
   eTDAL_HDMI_CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS     =   0x08,
   eTDAL_HDMI_CEC_OPCODE_RECORD_ON            =   0x09,
   eTDAL_HDMI_CEC_OPCODE_RECORD_STATUS         =   0x0A,
   eTDAL_HDMI_CEC_OPCODE_RECORD_OFF           =   0x0B,
   eTDAL_HDMI_CEC_OPCODE_TEXT_VIEW_ON          =   0x0D,
   eTDAL_HDMI_CEC_OPCODE_RECORD_TV_SCREEN         =   0x0F,
   eTDAL_HDMI_CEC_OPCODE_GIVE_DECK_STATUS         =   0x1A,
   eTDAL_HDMI_CEC_OPCODE_DECK_STATUS           =   0x1B,
   eTDAL_HDMI_CEC_OPCODE_SET_MENU_LANGUAGE        =   0x32,
   eTDAL_HDMI_CEC_OPCODE_CLEAR_ANALOGUE_TIMER       =   0x33,
   eTDAL_HDMI_CEC_OPCODE_SET_ANALOGUE_TIMER       =   0x34,
   eTDAL_HDMI_CEC_OPCODE_TIMER_STATUS          =   0x35,
   eTDAL_HDMI_CEC_OPCODE_STANDBY            =   0x36,
   eTDAL_HDMI_CEC_OPCODE_PLAY              =   0x41,
   eTDAL_HDMI_CEC_OPCODE_DECK_CONTROL          =   0x42,
   eTDAL_HDMI_CEC_OPCODE_TIMER_CLEARED_STATUS       =   0x43,
   eTDAL_HDMI_CEC_OPCODE_USER_CONTROL_PRESSED       =   0x44,
   eTDAL_HDMI_CEC_OPCODE_USER_CONTROL_RELEASED      =   0x45,
   eTDAL_HDMI_CEC_OPCODE_GIVE_OSD_NAME         =   0x46,
   eTDAL_HDMI_CEC_OPCODE_SET_OSD_NAME          =   0x47,
   eTDAL_HDMI_CEC_OPCODE_SET_OSD_STRING         =   0x64,
   eTDAL_HDMI_CEC_OPCODE_SET_TIMER_PROGRAM_TITLE      =   0x67,
   eTDAL_HDMI_CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST     =   0x70,
   eTDAL_HDMI_CEC_OPCODE_GIVE_AUDIO_STATUS        =   0x71,
   eTDAL_HDMI_CEC_OPCODE_SET_SYSTEM_AUDIO_MODE      =   0x72,
   eTDAL_HDMI_CEC_OPCODE_REPORT_AUDIO_STATUS       =   0x7A,
   eTDAL_HDMI_CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS   =   0x7D,
   eTDAL_HDMI_CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS     =   0x7E,
   eTDAL_HDMI_CEC_OPCODE_ROUTING_CHANGE         =   0x80,
   eTDAL_HDMI_CEC_OPCODE_ROUTING_INFORMATION       =   0x81,
   eTDAL_HDMI_CEC_OPCODE_ACTIVE_SOURCE         =   0x82,
   eTDAL_HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS      =   0x83,
   eTDAL_HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS      =   0x84,
   eTDAL_HDMI_CEC_OPCODE_REQUEST_ACTIVE_SOURCE      =   0x85,
   eTDAL_HDMI_CEC_OPCODE_SET_STREAM_PATH         =   0x86,
   eTDAL_HDMI_CEC_OPCODE_DEVICE_VENDOR_ID         =   0x87,
   eTDAL_HDMI_CEC_OPCODE_VENDOR_COMMAND         =   0x89,
   eTDAL_HDMI_CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN     =   0x8A,
   eTDAL_HDMI_CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP      =   0x8B,
   eTDAL_HDMI_CEC_OPCODE_GIVE_DEVICE_VENDOR_ID      =   0x8C,
   eTDAL_HDMI_CEC_OPCODE_MENU_REQUEST          =   0x8D,
   eTDAL_HDMI_CEC_OPCODE_MENU_STATUS           =   0x8E,
   eTDAL_HDMI_CEC_OPCODE_GIVE_DEVICE_POWER_STATUS     =   0x8F,
   eTDAL_HDMI_CEC_OPCODE_REPORT_POWER_STATUS       =   0x90,
   eTDAL_HDMI_CEC_OPCODE_GET_MENU_LANGUAGE        =   0x91,
   eTDAL_HDMI_CEC_OPCODE_SELECT_ANALOGUE_SERVICE      =   0x92,
   eTDAL_HDMI_CEC_OPCODE_SELECT_DIGITAL_SERVICE      =   0x93,
   eTDAL_HDMI_CEC_OPCODE_SET_AUDIO_RATE         =   0x9A,
   eTDAL_HDMI_CEC_OPCODE_SET_DIGITAL_TIMER        =   0x97,
   eTDAL_HDMI_CEC_OPCODE_CLEAR_DIGITAL_TIMER       =   0x99,
   eTDAL_HDMI_CEC_OPCODE_INACTIVE_SOURCE         =   0x9D,
   eTDAL_HDMI_CEC_OPCODE_CEC_VERSION           =   0x9E,
   eTDAL_HDMI_CEC_OPCODE_GET_CEC_VERSION         =   0x9F,
   eTDAL_HDMI_CEC_OPCODE_VENDOR_COMMAND_WITH_ID      =   0xA0,
   eTDAL_HDMI_CEC_OPCODE_CLEAR_EXTERNAL_TIMER       =   0xA1,
   eTDAL_HDMI_CEC_OPCODE_SET_EXTERNAL_TIMER       =   0xA2,
   eTDAL_HDMI_CEC_OPCODE_ABORT             =   0xFF,
   eTDAL_HDMI_CEC_OPCODE_POLLING_MESSAGE
}tTDAL_HDMI_CEC_Opcode;


typedef union
{
   tTDAL_HDMI_CEC_OperandContainerActiveSource       ActiveSource;
   tTDAL_HDMI_CEC_OperandContainerImageViewOn        ImageViewOn;
   tTDAL_HDMI_CEC_OperandContainerTextViewOn         TextViewOn;
   tTDAL_HDMI_CEC_OperandContainerRequestActiveSource     RequestActiveSource;
   tTDAL_HDMI_CEC_OperandContainerSource          Source;
   tTDAL_HDMI_CEC_OperandContainerRecordOff         RecordOff;
   tTDAL_HDMI_CEC_OperandContainerRecordTvScreen       RecordTvScreen;
   tTDAL_HDMI_CEC_OperandContainerGetCECVersion       GetCECVersion;
   tTDAL_HDMI_CEC_OperandContainerGivePhysicalAddress     GivePhysicalAddress;
   tTDAL_HDMI_CEC_OperandContainerGetMenuLanguage      GetMenuLanguage;
   tTDAL_HDMI_CEC_OperandContainerPollingMessage       PollingMessage;
   tTDAL_HDMI_CEC_OperandContainerTunerStepDecrement     TunerStepDecrement;
   tTDAL_HDMI_CEC_OperandContainerTunerStepIncrement     TunerStepIncrement;
   tTDAL_HDMI_CEC_OperandContainerGiveDeviceVendorID     GiveDeviceVendorID;
   tTDAL_HDMI_CEC_OperandContainerVendorRemoteButtonUp    VendorRemoteButtonUp;
   tTDAL_HDMI_CEC_OperandContainerGiveOSDName        GiveOSDName;
   tTDAL_HDMI_CEC_OperandContainerUserControlReleased     UserControlReleased;
   tTDAL_HDMI_CEC_OperandContainerGiveDevicePowerStatus    GiveDevicePowerStatus;
   tTDAL_HDMI_CEC_OperandContainerAbort           Abort;
   tTDAL_HDMI_CEC_OperandContainerGiveAudioStatus      GiveAudioStatus;
   tTDAL_HDMI_CEC_OperandContainerGiveSystemAudioModeStatus   GiveSystemAudioModeStatus;
   tTDAL_HDMI_CEC_OperandContainerInactiveSource       InactiveSource;
   tTDAL_HDMI_CEC_OperandContainerRoutingChange       RoutingChange;
   tTDAL_HDMI_CEC_OperandContainerRoutingInformation     RoutingInformation;
   tTDAL_HDMI_CEC_OperandContainerSetStreamPath       SetStreamPath;
   tTDAL_HDMI_CEC_OperandContainerRecordOn         RecordOn;
   tTDAL_HDMI_CEC_OperandContainerRecordStatus       RecordStatus;
   tTDAL_HDMI_CEC_OperandContainerClearAnalogueTimer     ClearAnalogueTimer;
   tTDAL_HDMI_CEC_OperandContainerClearDigitalTimer      ClearDigitalTimer;
   tTDAL_HDMI_CEC_OperandContainerClearExternalTimer     ClearExternalTimer;
   tTDAL_HDMI_CEC_OperandContainerSetAnalogueTimer      SetAnalogueTimer;
   tTDAL_HDMI_CEC_OperandContainerSetDigitalTimer      SetDigitalTimer;
   tTDAL_HDMI_CEC_OperandContainerSetExternalTimer      SetExternalTimer;
   tTDAL_HDMI_CEC_OperandContainerStandby         Standby;
   tTDAL_HDMI_CEC_OperandContainerSetTimerProgramTitle    SetTimerProgramTitle;
   tTDAL_HDMI_CEC_OperandContainerTimerClearedStatus     TimerClearedStatus;
   tTDAL_HDMI_CEC_OperandContainerTimerStatus        TimerStatus;
   tTDAL_HDMI_CEC_OperandContainerCECVersion         CECVersion;
   tTDAL_HDMI_CEC_OperandContainerReportPhysicalAddress    ReportPhysicalAddress;
   tTDAL_HDMI_CEC_OperandContainerSetMenuLanguage      SetMenuLanguage;
   tTDAL_HDMI_CEC_OperandContainerDeckControl        DeckControl;
   tTDAL_HDMI_CEC_OperandContainerDeckStatus         DeckStatus;
   tTDAL_HDMI_CEC_OperandContainerGiveDeckStatus       GiveDeckStatus;
   tTDAL_HDMI_CEC_OperandContainerPlay           Play;
   tTDAL_HDMI_CEC_OperandContainerSelectAnalogueService    SelectAnalogueService;
   tTDAL_HDMI_CEC_OperandContainerSelectDigitalService    SelectDigitalService;
   tTDAL_HDMI_CEC_OperandContainerGiveTunerDeviceStatus    GiveTunerDeviceStatus;
   tTDAL_HDMI_CEC_OperandContainerTunerDeviceStatus      TunerDeviceStatus;
   tTDAL_HDMI_CEC_OperandContainerDeviceVendorID       DeviceVendorID;
   tTDAL_HDMI_CEC_OperandContainerVendorCommand       VendorCommand;
   tTDAL_HDMI_CEC_OperandContainerVendorCommandWithID     VendorCommandWithID;
   tTDAL_HDMI_CEC_OperandContainerVendorRemoteButtonDown   VendorRemoteButtonDown;
   tTDAL_HDMI_CEC_OperandContainerSetOSDString       SetOSDString;
   tTDAL_HDMI_CEC_OperandContainerSetOSDName         SetOSDName;
   tTDAL_HDMI_CEC_OperandContainerMenuRequest        MenuRequest;
   tTDAL_HDMI_CEC_OperandContainerMenuStatus         MenuStatus;
   tTDAL_HDMI_CEC_OperandContainerUserControlPressed     UserControlPressed;
   tTDAL_HDMI_CEC_OperandContainerReportPowerStatus      ReportPowerStatus;
   tTDAL_HDMI_CEC_OperandContainerFeatureAbort       FeatureAbort;
   tTDAL_HDMI_CEC_OperandContainerReportAudioStatus      ReportAudioStatus;
   tTDAL_HDMI_CEC_OperandContainerSetSystemAudioMode     SetSystemAudioMode;
   tTDAL_HDMI_CEC_OperandContainerSystemAudioModeRequest   SystemAudioModeRequest;
   tTDAL_HDMI_CEC_OperandContainerSystemAudioModeStatus    SystemAudioModeStatus;
   tTDAL_HDMI_CEC_OperandContainerSetAudioRate       SetAudioRate;
   void*                        EmptyContainer;
}tTDAL_HDMI_CEC_OperandContainer;


/* CEC MESSAGES */
typedef struct
{
   tTDAL_HDMI_CEC_LogicalAddress   Source;
   tTDAL_HDMI_CEC_LogicalAddress   Destination;
   tTDAL_HDMI_CEC_Opcode      Opcode;
   tTDAL_HDMI_CEC_OperandContainer  OperandContainer;
}tTDAL_HDMI_CEC_Message;

/* CEC Event */
typedef enum
{
   eTDAL_HDMI_CEC_EVENT_MESSAGE_RECEIVED       = 0x0,
   eTDAL_HDMI_CEC_EVENT_NEW_LOGICAL_ADDRESS,
   eTDAL_HDMI_CEC_EVENT_NEW_PHYSICAL_ADDRESS,
   eTDAL_HDMI_CEC_EVENT_ACKNOWLEDGE_POLLING_MESSAGE
}tTDAL_HDMI_CEC_Event;


typedef void (*tTDAL_HDMI_CEC_CallbackProc) (tTDAL_HDMI_CEC_Event eEvent, void *pData);


typedef struct
{
   tTDAL_HDMI_CEC_CallbackProc MessageReceived;
   tTDAL_HDMI_CEC_CallbackProc LogicalAddress;
   tTDAL_HDMI_CEC_CallbackProc PhysicalAddress;
   tTDAL_HDMI_CEC_CallbackProc AcknowledgeReceived;
}tTDAL_HDMI_CEC_CallbackContainer;



/*****************************************************************************/
/*  Variables Declarations (IMPORT)                  */
/*****************************************************************************/




/*****************************************************************************/
/*  Functions Definitions                       */
/*****************************************************************************/

/* CHAL API */

/* Generic API*/
tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_Init(tTDAL_HDMI_CEC_DeviceType eDeviceType);

tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_Term(void);

const char *      TDAL_HDMI_CEC_APIRevisionGet(void );

const char *      TDAL_HDMI_CEC_PlatformRevisionGet(void );

tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_Start(void);
tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_Stop(void);

/* Communication API*/
tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_EventSubscribe(tTDAL_HDMI_CEC_Event eEvent,tTDAL_HDMI_CEC_CallbackProc cbNotifyFunction);

tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_EventUnsubscribe(tTDAL_HDMI_CEC_Event eEvent);

tTDAL_HDMI_CEC_Error   TDAL_HDMI_CEC_SendMessage(tTDAL_HDMI_CEC_Message *pstMessage);


/* For debug purpose only */
const char*      TDAL_HDMI_CEC_TranslateLogicalAddressInString(tTDAL_HDMI_CEC_LogicalAddress eLogicalAdress);
const char*      TDAL_HDMI_CEC_TranslateOpcodeInString(tTDAL_HDMI_CEC_Opcode eOpcode);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_HDMI_CEC_H_*/

