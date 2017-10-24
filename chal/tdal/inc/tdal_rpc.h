/*****************************************************************************
 *         COPYRIGHT 2006 IWEDIA TECHNOLOGIES         *
 *****************************************************************************
 *
 * MODULE NAME: TDAL RPC
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_rpc.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL RPC
 *       On-Board Module protocole RPC communication API definition.
 *       Manage the communication between an Analogue TV Chassis (ATV)
 *       (its soft) and Comedia running on the DTV On-Board Module.
 *
 *
 * For to active this communication in Comedia the compilation directive
 * PRODUCT_USE_TDAL_RPC must be defined.
 * This directive is defined if the variable PRODUCT_USE_RPC is setting to
 * "YES" in product.mak.
 *
 *****************************************************************************/

#ifndef _TDAL_RPC_H_
#define _TDAL_RPC_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

/*
 * Clients management
 */
#define kTDAL_RPC_NB_MAX_CLIENTS    3 /* Number max of clients      */
#define kTDAL_RPC_IDCLI_INVALID   0xFF /* Client id undef => Contex free  */
#define kTDAL_RPC_NB_MAX_KEY_FORCED   5 /* Number max of key forced/client */
#define kTDAL_RPC_SZ_DBGLABEL     64 /* Max size of debug label     */

/*
 * RPC Commands data size in bytes :
 */
#define kTDAL_RPC_CMD_SZ_ADDR      1 /* Address of the target device   */
#define kTDAL_RPC_CMD_SZ_LENGTH     1 /* Length of command       */
#define kTDAL_RPC_CMD_SZ_CMDID     1 /* Command identifier       */
#define kTDAL_RPC_CMD_SZ_STATUS     1 /* Command status         */
#define kTDAL_RPC_CMD_SZ_PAYLOAD   255 /* Maximum size of payload command */
#define kTDAL_RPC_CMD_SZ_CRC      1 /* The CRC            */
#define kTDAL_RPC_CMD_SZ_ADDLEN     2 /* Size head not in field length   */
#define kTDAL_RPC_CMD_SZ_MINI      3 /* Mini size (without payload)   */
#define kTDAL_RPC_CMD_SZ_PARTFIXE    5 /* Size part fixe (with addr + len)*/
#define kTDAL_RPC_CMD_SZMAX      260 /* Size max of request      */

/*
 * RPC COMMANDS - CONSTANT PART - PRE-DEFINED VALUES
 * -------------------------------------------------
 */

/*
 * Adress (first byte of commands).
 *   In the command it's the target device, identify the receiver
 *   In this module identify the client application.
 */
#define kTDAL_RPC_ADDR_ANALOG_TV   0x01 /* Analogue TV chassis  Address   */
#define kTDAL_RPC_ADDR_DTV_ON_BOARD  0x02 /* Digital On-Board Module Address */
#define mTDAL_RPC_IS_ADDR(b) (((b) == kTDAL_RPC_ADDR_ANALOG_TV) || ((b) == kTDAL_RPC_ADDR_DTV_ON_BOARD))

/*
 * Command identifier (bits 0..6 of 3th bytes of commands)
 */
/* ATV -> DTV On Board Module */
#define kTDAL_RPC_OBM_RemoteControlKey      0x01
#define kTDAL_RPC_OBM_GetDateTime        0x02
#define kTDAL_RPC_OBM_SetOutputs         0x03
#define kTDAL_RPC_OBM_GetOutputs         0x04
#define kTDAL_RPC_OBM_SetAspectRatioConv     0x05
#define kTDAL_RPC_OBM_GetAspectRatioConv     0x06
#define kTDAL_RPC_OBM_SetCountry         0x07
#define kTDAL_RPC_OBM_GetCountry         0x08
#define kTDAL_RPC_OBM_GetCountryList       0x09
#define kTDAL_RPC_OBM_SetMenuLanguage      0x0A
#define kTDAL_RPC_OBM_GetMenuLanguage      0x0B
#define kTDAL_RPC_OBM_GetMenuLanguageList     0x0C
#define kTDAL_RPC_OBM_SetDefaultAudLang      0x0D
#define kTDAL_RPC_OBM_GetDefaultAudLang      0x0E
#define kTDAL_RPC_OBM_GetDefaultAudLangList    0x0F
#define kTDAL_RPC_OBM_SetDefaultSubLang      0x10
#define kTDAL_RPC_OBM_GetDefaultSubLang      0x11
#define kTDAL_RPC_OBM_GetDefaultSubLangList    0x12
#define kTDAL_RPC_OBM_GetDownloadParameter    0x13
#define kTDAL_RPC_OBM_AskForStandBy       0x14
#define kTDAL_RPC_OBM_AskForWakeUp       0x15
#define kTDAL_RPC_OBM_GetState         0x16
#define kTDAL_RPC_OBM_SetOSDOpacity       0x17
#define kTDAL_RPC_OBM_GetOSDOpacity       0x18
#define kTDAL_RPC_OBM_SetPinCode         0x19
#define kTDAL_RPC_OBM_GetPinCode         0x1A
#define kTDAL_RPC_OBM_SetMaturityRating      0x1B
#define kTDAL_RPC_OBM_GetMaturityRating      0x1C
#define kTDAL_RPC_OBM_ResetToFactorySettings   0x1D
#define kTDAL_RPC_OBM_MenuGoToViewing      0x1E
#define kTDAL_RPC_OBM_MenuGoToMainMenu      0x1F
#define kTDAL_RPC_OBM_MenuGoToChannelInstallation 0x20
#define kTDAL_RPC_OBM_MenuGoToFavouriteList    0x21
#define kTDAL_RPC_OBM_MenuGoToConfiguration    0x22
#define kTDAL_RPC_OBM_MenuGoToParentalControl   0x23
#define kTDAL_RPC_OBM_MenuGoToTimer       0x24
#define kTDAL_RPC_OBM_MenuGoToEPG        0x25
#define kTDAL_RPC_OBM_MenuGoToChannelList     0x26
#define kTDAL_RPC_OBM_SetSourceState      0x42
#define kTDAL_RPC_OBM_SetDefaultSecondAudLang   0x43
#define kTDAL_RPC_OBM_GetDefaultSecondAudLang   0x44
#define kTDAL_RPC_OBM_SetDefaultSecondSubLang   0x45
#define kTDAL_RPC_OBM_GetDefaultSecondSubLang   0x46
/* DTV On Board Module -> ATV */
#define kTDAL_RPC_OBM_WSSChange         0x27
#define kTDAL_RPC_OBM_ServiceChanged       0x28
#define kTDAL_RPC_OBM_StateChanged       0x29
#define kTDAL_RPC_OBM_DTVOSDMenuEnter      0x2A
#define kTDAL_RPC_OBM_DTVOSDMenuExit       0x2B
#define kTDAL_RPC_OBM_CustomerCmd       0x40
#define kTDAL_RPC_OBM_GetSourceState      0x41
#define kTDAL_RPC_OBM_GetDisplayResolution    0x47
#define kTDAL_RPC_OBM_GetPowerOnReason      0x48
/*
 * Command type (bits 7 of 3th bytes of commands)
 */
#define kTDAL_RPC_TYPE_REQUEST     0 /* Request            */
#define kTDAL_RPC_TYPE_RESPONSE     1 /* Response           */

#ifdef __TDAL_RPC_FOR_PACK_TEST__
/*
 * For test bad request
 * Use value even for request and odd value for response.
 */
#define kTDAL_RPC_TYPE_REQU_BAD_ADD   2 /* Format request with bad address */
#define kTDAL_RPC_TYPE_RESP_BAD_ADD   3 /* Format response with bad addr   */
#define kTDAL_RPC_TYPE_REQU_BAD_LEN   4 /* Format request with invalid len */
#define kTDAL_RPC_TYPE_RESP_BAD_LEN   5 /* Format resp. with invalid len   */
#define kTDAL_RPC_TYPE_REQU_BAD_CRC   6 /* Format request with bad CRC   */
#define kTDAL_RPC_TYPE_RESP_BAD_CRC   7 /* Format response with bad CRC   */
#endif

/*
 * Status of command (4th bytes of commands)
 */
#define kTDAL_RPC_STATUS_OK           0x00
#define kTDAL_RPC_STATUS_PARAMETER_NOT_SUPPORTED  0x01
#define kTDAL_RPC_STATUS_CRC_FAILED       0x02

/*
 * RPC COMMANDS - PAYLOAD PART - PRE-DEFINED VALUES
 * ------------------------------------------------
 */

/*
 * RPC_RemoteControlKey : Key code
 */
#define kTDAL_RPC_RC_KEY_0      0x30 /* Key 0            */
#define kTDAL_RPC_RC_KEY_1      0x31 /* Key 1            */
#define kTDAL_RPC_RC_KEY_2      0x32 /* Key 2            */
#define kTDAL_RPC_RC_KEY_3      0x33 /* Key 3            */
#define kTDAL_RPC_RC_KEY_4      0x34 /* Key 4            */
#define kTDAL_RPC_RC_KEY_5      0x35 /* Key 5            */
#define kTDAL_RPC_RC_KEY_6      0x36 /* Key 6            */
#define kTDAL_RPC_RC_KEY_7      0x37 /* Key 7            */
#define kTDAL_RPC_RC_KEY_8      0x38 /* Key 8            */
#define kTDAL_RPC_RC_KEY_9      0x39 /* Key 9            */
#define kTDAL_RPC_RC_KEY_CHANN_UP   0x2B /* Key P+ (next channel)      */
#define kTDAL_RPC_RC_KEY_CHANN_DOWN  0x2D /* Key P- (previous channel)    */
#define kTDAL_RPC_RC_KEY_ARROW_UP   0x18 /* Key Up            */
#define kTDAL_RPC_RC_KEY_ARROW_DOWN  0x19 /* Key Down           */
#define kTDAL_RPC_RC_KEY_ARROW_LEFT  0x1B /* Key Left           */
#define kTDAL_RPC_RC_KEY_ARROW_RIGHT 0x1A /* Key Rigth           */
#define kTDAL_RPC_RC_KEY_SELECT_OK   0x0D /* Key Ok            */
#define kTDAL_RPC_RC_KEY_BACK    0x08 /* Key Back (previ menu or chann)  */
#define kTDAL_RPC_RC_KEY_EXIT    0x09 /* Key Exit/Cancel        */
#define kTDAL_RPC_RC_KEY_VOL_UP   0x80 /* Key V+ (volume up)       */
#define kTDAL_RPC_RC_KEY_VOL_DOWN   0x81 /* Key V- (volume down)      */
#define kTDAL_RPC_RC_KEY_MENU    0x82 /* Key Menu           */
#define kTDAL_RPC_RC_KEY_EPG     0x83 /* Key EPG            */
#define kTDAL_RPC_RC_KEY_INFO    0x84 /* Key Info           */
#define kTDAL_RPC_RC_KEY_TEXT    0x85 /* Key Teletex         */
#define kTDAL_RPC_RC_KEY_AUDIO    0x86 /* Key Audio           */
#define kTDAL_RPC_RC_KEY_SUBTITLE   0x87 /* Key Subtitle         */
#define kTDAL_RPC_RC_KEY_TV_RADIO   0x88 /* Key TV/Radio         */
#define kTDAL_RPC_RC_KEY_TIMER    0x89 /* Key timer list         */
#define kTDAL_RPC_RC_KEY_RED     0x8A /* Key color Red         */
#define kTDAL_RPC_RC_KEY_GREEN    0x8B /* Key color Green        */
#define kTDAL_RPC_RC_KEY_YELLOW   0x8C /* Key color Yellow       */
#define kTDAL_RPC_RC_KEY_BLUE    0x8D /* Key color Blue         */
#define kTDAL_RPC_RC_KEY_PAGE_UP   0x8E /* Key Page +          */
#define kTDAL_RPC_RC_KEY_PAGE_DOWN   0x8F /* Key Page -          */
/* Hidden key for test (not in protocol) */
#define kTDAL_RPC_RC_HIDKEY_POWER   0x62 /* Key Power (On-Board Module ON)  */
#define kTDAL_RPC_RC_HIDKEY_STANDBY  0x66 /* Key Off   (On-Board Module OFF) */

/*
 * RPC_RemoteControlKey : Key status
 */
#define kTDAL_RPC_RC_KEY_PRESSED   0x01 /* Key pressed but not released   */
#define kTDAL_RPC_RC_KEY_RELEASED   0x02 /* Previous key pressed released   */
#define kTDAL_RPC_RC_KEY_PRESREL   0x03 /* Key pressed and released    */

/*
 * RPC_SetOutputs and RPC_GetOutputs : Outputs Values
 */
#define kTDAL_RPC_OUTPUT_DISABLE   0x00 /* Output disable         */
#define kTDAL_RPC_OUTPUT_ON_VIDEO   0x01 /* Enables Video         */
#define kTDAL_RPC_OUTPUT_ON_AUDIO   0x02 /* Enables Audio         */
#define kTDAL_RPC_OUTPUT_ON_OSD   0x04 /* Enables OnScreenDisplay     */
#define kTDAL_RPC_OUTPUT_ON_ADIGITAL 0x08 /* Enables Audio Digital      */
#define kTDAL_RPC_OUTPUT_ON_SUBTITLE 0x10 /* Enables Subtitle       */
#define kTDAL_RPC_OUTPUT_ON_ANTEN_P  0x20 /* Enables Antenna Power      */

/*
 * RPC_SetAspectRatio and RPC_GetAspectRatio : Aspect Ratio Values
 */
#define kTDAL_RPC_ASPRAT_4_3_LETBOX   0x01 /* 4/3 Letter Box */
#define kTDAL_RPC_ASPRAT_4_3_PANSCAN   0x02 /* 4/3 Pan scan   */
#define kTDAL_RPC_ASPRAT_16_9      0x03 /* 16/9  Normal   */
#define kTDAL_RPC_ASPRAT_16_9_ZOOM   0x04 /* 16/9  Zoom   */
#define kTDAL_RPC_ASPRAT_16_9_STRETCH   0x05 /* 16/9  Stretcg  */

/*
 * RPC_GetCountry, RPC_SetCountry and RPC_GetCountryList : Country code
 * Country Code on 3 bytes, it's the 3 letters in ASCII of the code
 * ISO 3166-1 Alpha 3 of the country.
 * Sample :
 * - Finland    = "FIN" = 0x46 0x49 0x4E
 * - France    = "FRA" = 0x46 0x52 0x41
 * - Germany    = "DEU" = 0x44 0x45 0x55
 * - Great Britain = "GBR" = 0x47 0x42 0x52
 * - Italy     = "ITA" = 0x49 0x54 0x41
 * - Spain     = "SPA" = 0x53 0x45 0x41
 * - Sweden    = "SWE" = 0x53 0x57 0x45
 * - PAN Europe   = "PEU" = 0x50 0x45 0x55
 * ...
 */
#define kTDAL_RPC_COUNTRY_CODE_SZ    3

/*
 * RPC_SetMenuLanguage, RPC_GetMenuLanguage, RPC_GetMenuLanguageList,
 * RPC_SetDefaultAudLang, RPC_GetDefaultAudLang, RPC_GetDefaultAudLangList,
 * RPC_SetDefaultSubLang, RPC_GetDefaultSubLang and
 * RPC_GetDefaultSubLangList : Language Code on 3 bytes, it's the 3 letters
 * in ASCII of the code ISO 639-2 of the languege. Code use for the Menu,
 * Audio and Subtitle language
 * Sample :
 * - Engligh    = "eng" = 0x65 0x6E 0x67
 * - Finnish    = "fin" = 0x66 0x69 0x6E
 * - French    = "fre" = 0x66 0x72 0x65
 * - German    = "ger" = 0x67 0x65 0x72
 * - Italian    = "ita" = 0x69 0x74 0x61
 * - Spanish    = "spa" = 0x73 0x70 0x61
 * - Swedish    = "swe" = 0x73 0x77 0x65
 * ...
 */
#define kTDAL_RPC_LANG_CODE_SZ     3

/*
 * RPC_GetDownloadParameter : parameters size in the payload
 */
#define kTDAL_RPC_DL_PARAM_OUI_SZ    3 /* Organiz. unique Ident (3 bytes) */
#define kTDAL_RPC_DL_PARAM_HW_MODEL_SZ  2 /* Hardware model (2 bytes)    */
#define kTDAL_RPC_DL_PARAM_HW_VERS_SZ   2 /* Hardware version (2 bytes)   */
#define kTDAL_RPC_DL_PARAMS_SZ (kTDAL_RPC_DL_PARAM_OUI_SZ+kTDAL_RPC_DL_PARAM_HW_MODEL_SZ+kTDAL_RPC_DL_PARAM_HW_VERS_SZ)

/*
 * RPC_GetState : State value
 */
#define kTDAL_RPC_STATE_OFF     0x00 /* Power Off, normaly never set   */
#define kTDAL_RPC_STATE_BOOTING   0x01 /* Booting in curr        */
#define kTDAL_RPC_STATE_ON      0x02 /* Runing mode         */
#define kTDAL_RPC_STATE_STANDBY   0x03 /* Passive standby        */
#define kTDAL_RPC_STATE_ACT_STANDBY  0x04 /* Active standby         */
#define kTDAL_RPC_STATE_ACT_SB_TIMER 0x05 /* Timer is forecasted      */
#define kTDAL_RPC_STATE_ACT_SB_DOWNL 0x06 /* Download is forecasted     */
#define kTDAL_RPC_STATE_ACT_SB_TIM_P 0x07 /* Timer is in progress      */
#define kTDAL_RPC_STATE_ON_FTI    0x08 /* Runing mode: First time instal. */
#define kTDAL_RPC_STATE_ON_TIMER_P   0x09 /* Runing mode: Timer in progress  */
#define kTDAL_RPC_STATE_TRIGGER_STANDBY   0x0A /* standby mode: trigger action  */
#define kTDAL_RPC_STATE_CHANNEL_UPDATE_STANDBY   0x0B /* standby mode: channel update  */
#define kTDAL_RPC_STATE_FULL_EITCACHING_STANDBY   0x0C /* standby mode: full EIT caching  */
#define kTDAL_RPC_STATE_LOW_POWER   0x0D /* standby mode: low power mode is possible */

/*
 * RPC_SourceState : Source State value
 */
#define kTDAL_RPC_SOURCE_IS_ATV         0x01
#define kTDAL_RPC_SOURCE_IS_DTV         0x02

/*
 * RPC_SetOSDOpacity and RPC_GetOSDOpacity : Opacity value
 */
#define kTDAL_RPC_OSD_OPAQUE_FULL   0x00 /* Opaque total         */
#define kTDAL_RPC_OSD_OPAQUE_HIGH   0x01 /* Opaque 90%          */
#define kTDAL_RPC_OSD_OPAQUE_MEDIUM  0x02 /* Opaque 80%          */
#define kTDAL_RPC_OSD_OPAQUE_LOW   0x03 /* Opaque 70%          */

/*
 * RPC_SetPinCode and RPC_GetPinCode : PIN code (for to manage maturity
 * level) on 4 bytes ASCII
 */
#define kTDAL_RPC_PIN_CODE_SZ      4

/*
 * RPC_SetMaturityRating : Maturity rating values
 */
#define kTDAL_RPC_MATURITY_RATING_MIN   0
#define kTDAL_RPC_MATURITY_RATING_MAX  18

/*
 * RPC_WSSChanged : WSS Value
 */
#define kTDAL_RPC_WSS_14_9_LBOX_CEN  0x01 /*  14:9 - Letterbox Center    */
#define kTDAL_RPC_WSS_14_9_LBOX_TOP  0x02 /*  14:9 - Letterbox Top      */
#define kTDAL_RPC_WSS_16_9_LBOX_TOP  0x04 /*  16:9 - Letterbox Top      */
#define kTDAL_RPC_WSS_16_9_FULL_FORM 0x07 /*  16:9 - Full Format      */
#define kTDAL_RPC_WSS_4_3_FULL_FORM  0x08 /*   4:3 - Full format      */
#define kTDAL_RPC_WSS_16_9_LBOX_CEN  0x0B /*  16:9 - Letterbox Center    */
#define kTDAL_RPC_WSS_UP16_9_LBOX_C  0x0D /* >16:9 - Letterbox Center    */
#define kTDAL_RPC_WSS_14_9_FULL_FORM 0x0E /*  14:9 - Full Format      */

/*
 * Command family identifier for subscription
 * On bits by family => 32 family possibles
 */
#define kTDAL_RPC_FAMCMD_KEYS  0x00000001 /* Only OBM_RemoteControlKey    */
#define kTDAL_RPC_FAMCMD_SYST  0x00000002 /* Command system (all else key)   */
#define kTDAL_RPC_FAMCMD_NONE  0x00000000 /* M'en fout des reponses     */
#define kTDAL_RPC_FAMCMD_ALL   0xFFFFFFFF /* All commands         */


/*
 * definition o ATV wake up reason
 * if ATV low power suppports low power mode
 */

#define kTDAL_RPC_USER_WAKE_UP      0x01 /* the atv leaves low power on end-user action */
#define kTDAL_RPC_AC_POWER_SUPPLY_WAKE_UP  0x02 /* the atv leaves low power because of a power supply interrupt */
#define kTDAL_RPC_TIMER_END_WAKE_UP    0x03 /* the atv leaves low power the wake up timer has expired */
#define kTDAL_RPC_MISC_WAKE_UP      0x04 /* the atv leaves low power fot other reason */

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/

/*
 * Return Code : Execution status / Error code
 */
typedef enum
{
   eTDAL_RPC_NO_ERROR = 0,   /* Execution successful           */
   eTDAL_RPC_ERR_MEMORY,    /* Memory error (allocation failed, tkel)   */
   eTDAL_RPC_ERR_SYSTEM,    /* System error (on another ipc, tkel)    */
   eTDAL_RPC_ERR_PARAM,    /* Bad parameter              */
   eTDAL_RPC_ERR_NORESS,    /* No ress dispo, no context free to start   */
   eTDAL_RPC_ERR_COMM,     /* Error communication (open, read, write...) */
   eTDAL_RPC_ERR_TIMEOUT,   /* Wait response timeout          */
   eTDAL_RPC_ERROR
} tTDAL_RPC_ErrorCode;

/*
 * Command definition, memory implementation of command
 */
typedef struct
{
   uint8_t  ucIdent;      /* Command identifier (kTDAL_RPC_xxxx)    */
   uint8_t  ucType;      /* Type request or resp.(kTDAL_RPC_TYPE_xx)   */
   uint8_t  ucStatus;     /* Status of command (kTDAL_RPC_STATUS_xxx)   */
   uint8_t  tucPayload[kTDAL_RPC_CMD_SZ_PAYLOAD]; /* The payload      */
   uint8_t  ucSzPayload;    /* Size of payload in bytes         */
} tTDAL_RPC_Command;

/*
 * Serial port identifier
 */
typedef enum
{
   eTDAL_RPC_SP_COM0 = 0,
   eTDAL_RPC_SP_COM1,
   eTDAL_RPC_SP_COM2,
   eTDAL_RPC_SP_COM3,
   eTDAL_RPC_SP_NBMAXI    /* Not use for to known the number max of port */
} tTDAL_RPC_SerialPort;

/*
 * Serial port configuration
 */
typedef enum
{
   eTDAL_RPC_SP_SPEED_110,
   eTDAL_RPC_SP_SPEED_300,
   eTDAL_RPC_SP_SPEED_600,
   eTDAL_RPC_SP_SPEED_1200,
   eTDAL_RPC_SP_SPEED_2400,
   eTDAL_RPC_SP_SPEED_4800,
   eTDAL_RPC_SP_SPEED_9600,
   eTDAL_RPC_SP_SPEED_14400,
   eTDAL_RPC_SP_SPEED_19200,
   eTDAL_RPC_SP_SPEED_38400,
   eTDAL_RPC_SP_SPEED_57600,
   eTDAL_RPC_SP_SPEED_115200
} tTDAL_RPC_SP_Speed;

typedef enum
{
   eTDAL_RPC_SP_DATA_BITS_7,
   eTDAL_RPC_SP_DATA_BITS_8,
   eTDAL_RPC_SP_DATA_BITS_9
} tTDAL_RPC_SP_DataBits;

typedef enum
{
   eTDAL_RPC_SP_STOP_BITS_1,
   eTDAL_RPC_SP_STOP_BITS_2
} tTDAL_RPC_SP_StopBits;

typedef enum
{
   eTDAL_RPC_SP_PARITY_NONE,
   eTDAL_RPC_SP_PARITY_EVEN,
   eTDAL_RPC_SP_PARITY_ODD
} tTDAL_RPC_SP_Parity;

typedef enum
{
   eTDAL_RPC_SP_FLOW_CTRL_NONE,
   eTDAL_RPC_SP_FLOW_CTRL_HARD,
   eTDAL_RPC_SP_FLOW_CTRL_X
} tTDAL_RPC_SP_FlowCtrl;

typedef struct
{
   tTDAL_RPC_SP_Speed   eSpeed;
   tTDAL_RPC_SP_DataBits eDataBits;
   tTDAL_RPC_SP_StopBits eStopBits;
   tTDAL_RPC_SP_Parity   eParity;
   tTDAL_RPC_SP_FlowCtrl eFlowCtrl;
} tTDAL_RPC_SP_Config;

/*
 * Client module identifier
 */
typedef uint8_t tTDAL_RPC_ClientId;

/*
 * Callback for client receive command/response
 */
typedef void (*tTDAL_RPC_CmdRecvFct) (tTDAL_RPC_ClientId idCli,
                 tTDAL_RPC_Command *pCommand);

/*
 * TDAL RPC Capability
 */
typedef struct
{
   bool   bIsInit;
   int16_t iNbMaxClient;
   int16_t iNbClientsOpened;
   int16_t iNbMaxPort;
   int16_t iNbPortOpened;
}tTDAL_RPC_Capabilities;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_RPC_ErrorCode TDAL_RPC_Init    (void);
tTDAL_RPC_ErrorCode TDAL_RPC_Terminate  (void);
tTDAL_RPC_ErrorCode TDAL_RPC_PortConfig (tTDAL_RPC_SerialPort eSerialPort,
                  tTDAL_RPC_SP_Config *pConfig);
tTDAL_RPC_ErrorCode TDAL_RPC_Open    (tTDAL_RPC_SerialPort eSerialPort,
                  uint8_t ucLocalAd,
                  uint8_t ucTargetAd,
                  uint32_t uiCmdFamily,
                  tTDAL_RPC_CmdRecvFct pCmdRecvFct,
                  tTDAL_RPC_ClientId *pIdCli);
tTDAL_RPC_ErrorCode TDAL_RPC_Close   (tTDAL_RPC_ClientId idCli);
tTDAL_RPC_ErrorCode TDAL_RPC_SendCmd   (tTDAL_RPC_ClientId idCli,
                  tTDAL_RPC_Command *pCommand);
tTDAL_RPC_ErrorCode TDAL_RPC_ExecCmd   (tTDAL_RPC_ClientId idCli,
                  tTDAL_RPC_Command *pCommand,
                  tTDAL_RPC_Command *pResponse,
                  int16_t iTimeoutInt,
                  int16_t iTimeoutNum);
tTDAL_RPC_ErrorCode TDAL_RPC_ForceKey   (uint8_t ucKeyCode,
                  bool bExec);

tTDAL_RPC_ErrorCode TDAL_RPC_GetCapabilities (tTDAL_RPC_Capabilities *pCapa);

/*
 * Debug function Code to String
 * pLabel must be allocated by caller (kTDAL_RPC_SZ_DBGLABEL char + 1
 * for terminator)
 */
char *TDAL_RPC_DBG_LabelCmd    (char *pLabel, uint8_t ucIdent);
char *TDAL_RPC_DBG_LabelCmdType   (char *pLabel, uint8_t ucType);
char *TDAL_RPC_DBG_LabelCmdStatus (char *pLabel, uint8_t ucStatus);
char *TDAL_RPC_DBG_LabelKey    (char *pLabel, uint8_t ucKey);
char *TDAL_RPC_DBG_LabelState   (char *pLabel, uint8_t ucState);

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_RPC_H_*/
