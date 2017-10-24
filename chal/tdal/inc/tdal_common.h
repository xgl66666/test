/*******************************************************
 *      COPYRIGHT 2001 IWEDIA TECHNOLOGIES    *
 *******************************************************
 *
 * MODULE NAME: TDAL_Common
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_common.h $
 *      $Rev: 5847 $
 *      $Date: 2011-05-16 12:00:51 +0200 (lun., 16 mai 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 ********************************************************/

#ifndef _TDAL_COMMON_H_
#define _TDAL_COMMON_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

/*******************************************************/
/*       Defines              */
/*******************************************************/
#define MSTAR_QUEUE 1

#ifdef __DEBUG_MEM_LEAK__ /* Add utitily for memory leak debbuging */
#define TDAL_Malloc(_b)   TDAL_DebugMalloc(__FILE__,__LINE__,(_b))
#define TDAL_Free(_b)    TDAL_DebugFree(__FILE__,__LINE__,(_b))
#endif
/*******************************************************/
/*       Macros               */
/*******************************************************/


/*******************************************************/
/*       Typedefs              */
/*******************************************************/

/* Module trace identifiers */
typedef enum
{
   eTDAL_UART = 200,
   eTDAL_DESC,
   eTDAL_VBI,
   eTDAL_SMC,
   eTDAL_PTSM,
   eTDAL_GFX,
   eTDAL_COMMON,
   eTDAL_DMX,
   eTDAL_AVS,
   eTDAL_KBD,
   eTDAL_NVM,
   eTDAL_CRYPT,
   eTDAL_FLA,
   eTDAL_MP,
   eTDAL_DMD,
   eTDAL_DISP,
   eTDAL_OUTPUT,
   eTDAL_FP,
   eTDAL_AV,
   eTDAL_CI,
   eTDAL_DENC,
   eTDAL_TSOUT,
   eTDAL_TSIN,
   eTDAL_TS_ROUTE,
   eTDAL_HDMI,
   eTDAL_HDMI_CEC,
   eTDAL_TS_SOURCE,
   eTDAL_TS_SINK,
   eTDAL_ATA,
   eTDAL_FS,
   eTDAL_RPC,
   eTDAL_RPC_COMM,
   eTDAL_SPEAKERS,
   eTDAL_PANEL,
   eTDAL_VBI_SLICER,
   eTDAL_ANA_INPUT,
   eTDAL_ANA_AUD,
   eTDAL_ANA_VID,
   eTDAL_HDMI_IN,
   eTDAL_SCART_INPUT,
   eTDAL_SEC,
   eTDAL_PERSO,
   eTDAL_TS_MERGE,
   eTDAL_OUTPUT_RFMOD,
   eTDAL_PVR_EM,
   eTDAL_SSU
} tTDAL_ModuleTraceID;


typedef enum
{
   eTDAL_JUMP_BOOTLOADER,
   eTDAL_JUMP_APPLI,
   eTDAL_JUMP_MAX
}tTDAL_JUMP_Type;


typedef enum
{
   eTDAL_POWER_ON_FRONT_POWER_KEY = 0,   /* 1 */
   eTDAL_POWER_ON_RCU_POWER_KEY,     /* 2 */
   eTDAL_POWER_ON_RCU_0_KEY = 0x10,    /* 0x10 */
   eTDAL_POWER_ON_RCU_1_KEY,       /* 0x11 */
   eTDAL_POWER_ON_RCU_2_KEY,       /* 0x12 */
   eTDAL_POWER_ON_RCU_3_KEY,       /* 0x13 */
   eTDAL_POWER_ON_RCU_4_KEY,       /* 0x14 */
   eTDAL_POWER_ON_RCU_5_KEY,       /* 0x15 */
   eTDAL_POWER_ON_RCU_6_KEY,       /* 0x16 */
   eTDAL_POWER_ON_RCU_7_KEY,       /* 0x17 */
   eTDAL_POWER_ON_RCU_8_KEY,       /* 0x18 */
   eTDAL_POWER_ON_RCU_9_KEY,       /* 0x19 */
   eTDAL_POWER_ON_AUTO_RESET = 0x20,
   eTDAL_POWER_ON_AC_POWER_SUPPLY,
   eTDAL_POWER_ON_WAKEUP_TIMER,
   eTDAL_POWER_ON_CEC_ACTIVATION,
   eTDAL_POWER_ON_MISC
}tTDAL_PowerOnStatus;

#define LOADER_FALG_ADDR (0x7f0000)
#define OTA_DIAGNOSTIC_STATUS_OFFSET (0x1B3)
#define OTA_INTI_FLAG_OFFSET (0x1D6)

/*
* \depricated Structure is going to be removed from the API
*/

typedef void* tTDAL_Diag_Handle;

/*
* \depricated Structure is going to be removed from the API
*/

typedef enum
{
	eTDAL_OTA_STATUS_NO_ERROR,
	eTDAL_OTA_STATUS_ERROR
} tTDAL_OTA_ErrorCode;


/*
* \depricated Structure is going to be removed from the API
*/
typedef enum
{
	eTDAL_DIAG_HW_INFO,
	eTDAL_DIAG_SW_INFO
} tTDAL_Diag_InfoType;


/*
* \depricated Structure is going to be removed from the API
*/
typedef struct stTDAL_Diag_HWInfo
{
    char      cHardwareVersion[16];        /* HardwareVersion */
    uint32_t  uiHardwareVersionId;         /* HardwareVersion Id */
    char      cHardwareModel[16];          /* HardwareModel */
    uint32_t  uiHardwareModelId;           /* HardwareModel Id */
    char      cHardwareDeviceName[16];     /* DeviceName */
    uint32_t  uiHardwareDeviceNameId;      /* DeviceName Id */
    char      cHardwareManufacturer[16];   /* Manufacturer */   
    uint32_t  uiHardwareManufacturerId;    /* Manufacturer Id */
    char      cHardwareSTBSerialNumber[16];/* STB Serial Number */
} tTDAL_Diag_HWInfo;

/*
* \depricated Structure is going to be removed from the API
*/
typedef struct stTDAL_Diag_SWInfo
{
    char    cDriverVersion[16];            /* CHAL version */
    uint32_t uiDriverVersionId;            /* CHAL version Id */
    char    cLoaderVersion[16];            /* Loader version */
    uint32_t uiLoaderVersionId;            /* Loader version Id */
    char    cLauncherVersion[16];          /* Launcher version */
    uint32_t uiLauncherVersionId;          /* Launcher version Id */
} tTDAL_Diag_SWInfo;

/*
* \depricated Structure is going to be removed from the API
*/
typedef enum
{
	eTDAL_NORMAL_OTA = 0, 
	eTDAL_FORCED_OTA
} tTDAL_OTAType;

/**  OTA Data type of hardware required by MW
* \depricated Structure is going to be removed from the API
*/
typedef enum
{
    eTDAL_HW_PARAMETERS_OTA = 0, 
    eTDAL_HW_PARAMETERS_DIAGNOSTIC
} tTDAL_HWDataType;

/** Handle for the HW parameters
* \depricated Structure is going to be removed from the API
*/
typedef void* tTDAL_HW_Handle;

/** Hardware parameters for diagnostics banner
* \depricated Structure is going to be removed from the API
*/

typedef struct stTDAL_DiagnosticHWParameters
{
    char                    cHardwareVersion[10];
    char                    cHardwareModel[10];   
    char                    cProductVersion[10];     
    char                    cProductModel[10];    
    char                    cDeviceBrandName[10];  
    char                    cHardwareDriverVersion[10];
    char                    cManufacturerID[10];
}tTDAL_DiagnosticHWParameters; 

/** HW parameters for driver info banner
* \depricated Structure is going to be removed from the API
*/
typedef struct stTDAL_OTA_HWParameters
{
    uint32_t                uiOUI;
    uint16_t                uiHardwareVersion;   
    uint16_t                uiHardwareModel;     
    uint16_t                uiProductVersion;    
    uint16_t                uiProductModel; 
}tTDAL_OTA_HWParameters; 

/** Hardware parameters for Last download status
* \depricated Structure is going to be removed from the API
*/
typedef struct stTDAL_DiagnosticOTAStatus
{
    char                    cStatus[10];
    char                    cErrorCode[5];
    char                    cDateAndTime[20];            
} tTDAL_DiagnosticOTAStatus; 

/** whether the box is in standby state or not
* \depricated Structure is going to be removed from the API
*/
typedef enum
{
    eTDAL_STANDBY_OFF = 0, 
    eTDAL_STANDBY_ON
} tTDAL_OTAStandbyState;

/** \depricated Structure is going to be removed from the API */
typedef struct stTDAL_OTA_ControlParameters
{
    uint8_t                 bForced;    /* True if by field operator */
    uint8_t                 bInStandBy; /* MW was in standby */
    uint32_t                uiFreqKHz;  /* Freq of TS carrying SSU data */
    uint16_t                uiSymbRateK;/* SR of stream in kilo of units */
    uint8_t                 ucQAMMode;  /* Modulation */
    uint16_t                uiPID;      /* PID of SSU carousel */
    uint32_t                uiUpdateID; /* To be saved in DA,RELEASE NUMBER = 1.X Build A.B.C,for example:V1.1Build5.0.5a= 0x 01 00 05 0A*/
}tTDAL_OTA_ControlParameters;

/** \depricated Structure is going to be removed from the API */
typedef struct stTDAL_OTA_StatusParameters
{
    char                  cUtcTime[5];    /* UTC time encoded as specified in TDT */
    uint32_t              uiLastUpdateID; /* TBD */
    uint8_t               bInStandBy;    /* Loader was in standby */
    uint8_t               bFirstBoot;    /* First boot after SSU */
    char                  cStatus[10];   /* Success, Failed, Suspended */
    char                  cErrorCode[5]; /* Error code format: EXXX */
    tTDAL_OTA_ErrorCode   eErrorStatus;  /* Status of finished SSU */
}tTDAL_OTA_StatusParameters;


typedef  struct _TDAL_mutex_t
{
	uint32_t task_id;
	uint32_t count;
	void* mutex_id;
}TDAL_mutex_t;

typedef TDAL_mutex_t* TDAL_mutex_id;

typedef void* TDAL_task_id;

typedef enum
{
    eTDAL_PRIORITY_REALTIME,
    eTDAL_PRIORITY_HIGH,
    eTDAL_PRIORITY_NORMAL,
    eTDAL_PRIORITY_LOW,
} TDAL_TaskPriority;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

GLOBAL void TDAL_Init(void);
GLOBAL void TDAL_Term(void);
GLOBAL void TDAL_EnterLowPowerTimeout(uint32_t timeout);
GLOBAL bool TDAL_GetPowerOnStatus(tTDAL_PowerOnStatus   *status,
               uint32_t       *lowPowerDuration,
               uint32_t       ulStandbyReference);
GLOBAL void TDAL_Reset(void);
GLOBAL void TDAL_Jump(tTDAL_JUMP_Type eJumpType);
GLOBAL void TDAL_SetMemoryPartition(void* pPartition);

GLOBAL bool TDAL_GetUpTime(uint32_t   *upTime);
GLOBAL bool TDAL_GetTime(uint32_t   *time);
GLOBAL bool TDAL_SetTime(uint32_t   time);

#ifdef __DEBUG_MEM_LEAK__ /* Add utitily for memory leak debbuging */
GLOBAL void* TDAL_DebugMalloc(const char *zFile, int iLine, uint32_t memorySize);
GLOBAL void TDAL_DebugFree(const char *zFile, int iLine, void*buffer);
#else
GLOBAL void* TDAL_Malloc(uint32_t memorySize);
GLOBAL void TDAL_Free(void*buffer);
#endif

GLOBAL void TDAL_StackInfo(void);
GLOBAL void TDAL_TaskStart(unsigned long, void*);
GLOBAL void TDAL_TaskDelete(unsigned long, void*);
GLOBAL void TDAL_MemoryInfo(void);

GLOBAL void TDAL_CreateMutex(TDAL_mutex_id * MutexID);
GLOBAL void TDAL_DeleteMutex(TDAL_mutex_id MutexID);
GLOBAL void TDAL_LockMutex(TDAL_mutex_id MutexID);
GLOBAL void TDAL_UnlockMutex(TDAL_mutex_id MutexID);


GLOBAL TDAL_task_id TDAL_CreateTask(TDAL_TaskPriority priority,
        char *taskName,
        unsigned char * stack,
        uint32_t stackSize,
        void (*entry) (void*),
        void *arg
        );
GLOBAL void TDAL_DeleteTask(TDAL_task_id taskId);
GLOBAL void TDAL_DelayTask(uint32_t timeInMs);
GLOBAL uint32_t TDAL_GetRunningThreadSystemId(void);

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
GLOBAL tTDAL_OTA_ErrorCode TDAL_Diag_GetInfo(tTDAL_Diag_InfoType diagInfoType, tTDAL_Diag_Handle pProperStructure);
/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_Initiate(tTDAL_OTAType otaType, tTDAL_OTAStandbyState otaStandbyState);
/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_GetDiagnosticStatus(tTDAL_DiagnosticOTAStatus *pOTAStatus);
/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
GLOBAL void TDAL_GetLoadVersion(unsigned char *pcLoadVersion);
/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
tTDAL_OTA_ErrorCode TDAL_GetHWParameters(tTDAL_HWDataType hwDataType, tTDAL_HW_Handle pProperStructure);
/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
GLOBAL tTDAL_OTAStandbyState TDAL_OTA_GetStandbyState(void);

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*
* \note check TDAL SSU
*/
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_GetParameters(tTDAL_OTA_StatusParameters *otaParam);

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*
* \note check TDAL SSU
*/
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_SetParameters(tTDAL_OTA_ControlParameters *otaParam);

#if defined(SELECT_STB_PBI_SDQW_5105)
/*Common Interface to access to the PIO extended*/
extern void EXPANDED_PIO_OUTPUT(int PIN, int VALUE)   ;
#endif

#endif /*_TDAL_COMMON_H_*/



