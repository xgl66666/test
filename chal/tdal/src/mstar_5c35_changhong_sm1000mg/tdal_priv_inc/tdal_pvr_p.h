/*
 * tdal_pvr_p.h
 *
 *  Created on: Mar 7, 2013
 *      Author: rtrk
 */

#ifndef TDAL_PVR_P_H_
#define TDAL_PVR_P_H_

typedef enum _SI_AudioFormat
{
    SiNOT_VALID                    = 0x00,
    SiMPEG                         = 0x01,
    SiAC3                          = 0x02,
    SiMPEG_AD                      = 0x03,
    SiMPEG4                        = 0x04,
    SiAAC                          = 0x05,
    SiAC3P                         = 0x06,
    SiAC3_AD                       = 0x07,
} SI_AudioFormat;

typedef enum
{
    //MPEG 1/2
    E_DDI_VDEC_CODEC_TYPE_MPEG2 = 0,
    //H264
    E_DDI_VDEC_CODEC_TYPE_H264,
    //MPEG 1/2 for I-Frame
    E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME,
    //H264 for I-Frame
    E_DDI_VDEC_CODEC_TYPE_H264_IFRAME,
}DDI_VDEC_CodecType;


typedef struct
{
    char recPath[32];
}PVR_FSINFO;

typedef enum
{
    PVR_TIMESHIFT_PATH_IDX=0,
    PVR_NORMALREC_PATH_IDX=1,
}PVR_REC_PATH_IDX;

#define INVALID_FILTER_ID                       0xFF

#define PVR_AESBUFFER_LEN                                  16//0x00040000        /* 0.25 M*/
#define PVR_SYNC_DELAY          (0)

#define PVR_DEFAULT_PROGRAM_IDX 0
#define PVR_MAX_RECORDING_FILE EN_APIPVR_DUAL_TUNER
#define PVR_THREAD_NAME "PVR_MSG"
#define MAX_PROGRAM_NUM PVR_MAX_RECORDING_FILE
#define PVR_FILE_LINEAR  0
#define PVR_FILE_CIRCULAR  1
#define APIPVR_PLAYBACK_SPEED_1X  1000
#define PVR_FILE_SIZE_TO_STOP     (1*1024)   /* 1 MB space for USB reserved */
#define PVR_FILE_CHUCK_MAX_SIZE  3*1024*1024 /* 3 GB chunk */
#define PVR_THUMBNAIL_CAPTURE_TIME 30        /* number of seconds before thumbnail capture */
#define PVR_THUMBNAIL_CAPTURE_WIDTH 160      /* width of thumbnail */
#define PVR_THUMBNAIL_CAPTURE_HEIGHT 120     /* height of thumbnail */
#define MIN_TIME_SHIFT_DISK_SPACE_IN_KB (512*1024)  /* 512MB */
#define INVALID_PID 0x1FFF
#define PVR_IS_VALID_PID(x)  ((x!= 0)&& (x< INVALID_PID))
#define PVR_IS_VALID_HANDLER(x) (x!=APIPVR_INVALID_HANDLER)

extern volatile bool                   TDAL_AV_Playback_bIsFileIn;

typedef struct ProgInfoElem_t
{
    uint16_t u16VideoPID;
    uint16_t u16AudioPID;
    uint16_t u16PCRPID;
    uint16_t u16SubsPID;
} tTDAL_ProgInfoElem;

struct PVRProgramInfo_t;
typedef struct tTDAL_PVR_Desc_t
{
    uint8_t pvrRecHandle;
    uint8_t pvrPlyHandle;
    uint8_t progIdx;
    bool    bIsTimeshift;
    bool    bUsed;
    bool    bIsCaptured;
    uint32_t eCurrentSpeed;
    uint8_t FilesCount;
    uint64_t    u64FreeSpaceInKB;
    uint64_t    u64FullRecSizeInKB;
    uint32_t    u32FullRecDuration;
    tTDAL_ProgInfoElem ProgInfoElem;
    PVRProgramInfo_t *pPVRProgInfo;
} tTDAL_PVR_Desc;

#if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

#else
/* VDPLAYER_MEM   */
#define PVR_DOWNLOAD_LEN                                0x0000360000         /* 3.375 M*/
#define PVR_UPLOAD_LEN                                  0x0000120000         /* 1.125 M*/
#define PVR_AUDIO_BUFFER_LEN                            (0x00010000)       /* 64 KB*/
#define PVR_MN_VIDEO_BUFFER_LEN                         (0x000300000)
#define PVR_MN_AUDIO_BUFFER_LEN                         (0x00010000)
#define PVR_TSR_VIDEO_BUFFER_LEN                        (0x000360000)
#define PVR_TSR_AUDIO_BUFFER_LEN                        (0x00020000)
#endif

MS_U8   TDAL_PVRm_GetRecordIndex(MS_U32 u32RecHandler);
void*   TDAL_PVRm_MEMALLOC_FUNC(MS_U32 u32Size);
MS_BOOL TDAL_PVRm_MEMFREE_FUNC(void *pBuf);
MS_U32  TDAL_PVRm_MEMPA2VA_FUNC(MS_U32 u32PhyAdr);
void    TDAL_PVRm_MEMFLUSH_FUNC(MS_U32 u32Adr,MS_U32 u32Len);
MS_BOOL TDAL_PVRm_KLADDER_FUNC(void);
void    TDAL_PVRm_FreezeScreen(MS_BOOL bFrezon);
MS_BOOL TDAL_PVRm_AllocateMem(int pvrId);
MS_BOOL TDAL_PVRm_FreeMem(int pvrId);
MS_BOOL TDAL_PVRm_BatchAllocateMem();
MS_BOOL TDAL_PVRm_BatchFreeMem();
MS_BOOL TDAL_PVRm_GetRecIdx(MS_U8 *u8RecIdx, MS_BOOL bTimeshift);
MS_BOOL TDAL_PVRm_GetRecMountPath(MS_BOOL bTimeshift, char *mountpath);
MS_BOOL TDAL_PVRm_CheckUSB(char *mountPath);
MS_BOOL TDAL_PVRm_GetDiskSpace(char *mountPath,MS_U64* u64FreeSpaceInKB, MS_U64* u64TotalSpaceInKB);
MS_BOOL TDAL_PVRm_IsFileExist(const char *m_mount_path,char *filename);
MS_U32  TDAL_PVRm_IsH264(APIPVR_CODEC_TYPE enPvrVCodec);
MS_BOOL TDAL_PVRm_FileModePlay(MS_U32 u32VCodec, MS_U32 u32ACodec);
void    TDAL_PVRm_PhyClose(void);
APIPVR_ENCRYPTION_TYPE TDAL_PVRm_ConvEncrType(uint8_t eEncrType);
void    TDAL_PVRm_SetCryptKey(uint8_t *u8Crypt);
uint8_t* TDAL_PVRm_GetCryptKey();
void TDAL_PVRi_CaptureWin(uint8_t eSource, uint32_t width, uint32_t height, char *filename);
void TDAL_PVRi_PlaybackNextChunk(tTDAL_PVR_Desc *pPVRDesc, uint16_t curFileIdx);
bool TDAL_PVRi_RecordNextChunk(tTDAL_PVR_Desc *pPVRDesc);

#endif /* TDAL_PVR_P_H_ */
