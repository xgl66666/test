/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PVR
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_PVR abstraction layer
 *
 *****************************************************************************/

#ifndef TDAL_PVR_H_
#define TDAL_PVR_H_

/*******************************************************/
/*       Defines                                       */
/*******************************************************/

#define kTDAL_PVR_MAX_SUBTITLEINFO_NUM    12
#define kTDAL_PVR_MAX_AUDIOINFO_NUM       16

/*******************************************************/
/*       Macros                                        */
/*******************************************************/

/*******************************************************/
/*       Typedefs                                      */
/*******************************************************/

typedef enum TDAL_PVR_Error_e {
    eTDAL_PVR_NO_ERROR = 0,     /* no error                  */
    eTDAL_PVR_ERROR_NOT_INIT,   /* driver is not initialised */
    eTDAL_PVR_ERROR_BAD_ARG,    /* wrong argument value      */
    eTDAL_PVR_ERROR_IN_DRIVER,  /* error in the driver       */
    eTDAL_PVR_ERROR_NOT_DONE    /* command not done          */
} tTDAL_PVR_Error;

typedef enum TDAL_PVR_Encr_e {
    eTDAL_PVR_ENCR_NONE,
    eTDAL_PVR_ENCR_DEFAULT,
    eTDAL_PVR_ENCR_USER,
    eTDAL_PVR_ENCR_CIPLUS,
    eTDAL_PVR_ENCR_SMARTCARD
} tTDAL_PVR_Encr;

/* pvr events codes */
typedef enum tTDAL_PVR_Event_e {
    eTDAL_PVR_END_FILE = 0,     /* playback reached the end of file     */
    eTDAL_PVR_BEGIN_FILE,       /* playback reached the end of file     */
    eTDAL_PVR_OPEN_FILE_FAIL,   /* open file failed                     */
    eTDAL_PVR_READ_FILE_FAIL,   /* read file failed                     */
    eTDAL_PVR_WRITE_FILE_FAIL,  /* write file failed                    */
    eTDAL_PVR_NO_DISK_SPACE,    /* no disk space for recording          */
    eTDAL_PVR_RECORDING_ERROR,  /* something went wrong with recording  */
    eTDAL_PVR_PLAYBACK_ERROR,   /* something went wrong with playback   */
    eTDAL_PVR_FILE_THRESHOLD    /* threshold for file size reached      */
} tTDAL_PVR_Event;

typedef enum tTDAL_PVR_Speed_e {
    eTDAL_PVR_SPEED_64X_BACKWARD ,
    eTDAL_PVR_SPEED_32X_BACKWARD ,
    eTDAL_PVR_SPEED_16X_BACKWARD ,
    eTDAL_PVR_SPEED_8X_BACKWARD ,
    eTDAL_PVR_SPEED_4X_BACKWARD ,
    eTDAL_PVR_SPEED_2X_BACKWARD ,
    eTDAL_PVR_SPEED_1X_BACKWARD ,
    eTDAL_PVR_SPEED_0X5_BACKWARD ,
    eTDAL_PVR_SPEED_0X25_BACKWARD ,
    eTDAL_PVR_SPEED_0X ,
    eTDAL_PVR_SPEED_FREEZE ,
    eTDAL_PVR_SPEED_0X25_FORWARD ,
    eTDAL_PVR_SPEED_0X5_FORWARD ,
    eTDAL_PVR_SPEED_1X_FORWARD ,
    eTDAL_PVR_SPEED_2X_FORWARD ,
    eTDAL_PVR_SPEED_4X_FORWARD ,
    eTDAL_PVR_SPEED_8X_FORWARD ,
    eTDAL_PVR_SPEED_16X_FORWARD ,
    eTDAL_PVR_SPEED_32X_FORWARD ,
    eTDAL_PVR_SPEED_64X_FORWARD
} tTDAL_PVR_Speed;

typedef struct tTDAL_PVR_AudioInfo_t
{
    uint8_t  isoLangInfo[3];    /* Language info */
    uint8_t  u8ISOIsValid;      /* */ 
    uint16_t u16AudioPID;       /* Audio PID*/
    uint8_t  u8AudioType;       /* TDAL AUDIO type */
    
} tTDAL_PVR_AudioInfo;

typedef struct tTDAL_PVR_SubtitleInfo_t
{
    uint8_t   u8ComponentTag;
    uint16_t  u16Sub_Pid;
    uint8_t   u8lanuage_code[3];
    uint8_t   u8subtitling_type; // EN_TTX_TYPE
    uint16_t  u16composition_page_id;
    uint16_t  u16ancillary_page_id;
    
} tTDAL_PVR_SubsInfo;

typedef struct tTDAL_PVR_BasicProgInfo_t
{
    uint32_t u32PCRPid; 
    uint32_t u32VideoPid;
    uint32_t u32AudioPid;
    uint32_t u32VCodec;
    uint32_t u32ACodec;
    uint32_t u32PmtPid;
    uint32_t u32LCN;
    uint8_t     nAudioInfoCount;
    tTDAL_PVR_AudioInfo pAudioInfo[kTDAL_PVR_MAX_AUDIOINFO_NUM];
    uint8_t     nSubsInfoCount;
    tTDAL_PVR_SubsInfo  pSubsInfo[kTDAL_PVR_MAX_SUBTITLEINFO_NUM];
    char*    fileName;
    uint32_t fileThreshold;
    uint8_t  filesCount;
    tTDAL_PVR_Encr eEncryptionType;
    bool     bIsEncrypted;
    uint8_t  u8CryptKey[4];
} tTDAL_PVR_BasicProgInfo;

typedef struct tTDAL_PVR_ComplexProgInfo_t
{
    tTDAL_PVR_BasicProgInfo stPVRBasicProgInfo;
    uint32_t                u32duration;
    uint32_t                u32StartTime;
    uint64_t                u64FileLength;
    uint8_t                 u8Age;
    bool                    bLocked;
    uint32_t                u32LastPlayPositionInSec;
} tTDAL_PVR_ComplexProgInfo;

typedef uint32_t tTDAL_PVR_Handle;

typedef void (*tTDAL_PVR_Callback)(int, void*);

/*******************************************************/
/*       Variables Declarations (IMPORT)               */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations                        */
/*******************************************************/

tTDAL_PVR_Error TDAL_PVR_Init(void);
tTDAL_PVR_Error TDAL_PVR_Term(void);
tTDAL_PVR_Error TDAL_PVR_Open(tTDAL_PVR_Handle *pvrHandle, bool bIsTimeshift, const char* pPath);
tTDAL_PVR_Error TDAL_PVR_Close(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_RegisterCallback(tTDAL_PVR_Callback PVR_CB);

tTDAL_PVR_Error TDAL_PVR_Record_Start(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Record_Stop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Record_Pause(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Record_Resume(tTDAL_PVR_Handle pvrHandle);

tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStart(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Timeshift_RecordStop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Timeshift_Stop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_Timeshift_PlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec);
tTDAL_PVR_Error TDAL_PVR_Timeshift_PlaybackStop(tTDAL_PVR_Handle pvrHandle);

tTDAL_PVR_Error TDAL_PVR_PlaybackStart(tTDAL_PVR_Handle pvrHandle, uint32_t u32PlaybackTimeInSec);
tTDAL_PVR_Error TDAL_PVR_PlaybackStop(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackPause(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackResume(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackFastForward(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed enTrick);
tTDAL_PVR_Error TDAL_PVR_PlaybackFastBackward(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed enTrick);
tTDAL_PVR_Error TDAL_PVR_PlaybackGetSpeed(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_Speed* pSpeed);
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpToTime(tTDAL_PVR_Handle pvrHandle, int32_t *u32Hour, uint32_t *u32Minute, uint32_t *u32Second);
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpForward(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_PlaybackJumpBackward(tTDAL_PVR_Handle pvrHandle);

tTDAL_PVR_Error TDAL_PVR_SetProgramInfo(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_BasicProgInfo *pBasicProgInfo);
tTDAL_PVR_Error TDAL_PVR_GetProgramInfo(tTDAL_PVR_Handle pvrHandle, tTDAL_PVR_ComplexProgInfo *pPVRComplexInfo);
void            TDAL_PVR_SetFreeSpaceThreshold(uint8_t percent);
uint8_t         TDAL_PVR_GetFreeSpaceThreshold();
bool            TDAL_PVR_IsRecording(tTDAL_PVR_Handle pvrHandle);
tTDAL_PVR_Error TDAL_PVR_RemoveFile(char *fileName, int filesCount);
uint32_t 		TDAL_PVR_GetPlaybackTime();

tTDAL_PVR_Error TDAL_PVR_SetPlaybackRetentionLimit(tTDAL_PVR_Handle pvrHandle, const uint32_t *u32RetentionLimitTimeInSec);

#endif /* TDAL_PVR_H_ */
