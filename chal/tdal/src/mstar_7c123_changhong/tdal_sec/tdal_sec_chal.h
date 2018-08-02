#ifndef _TDAL_SEC_CHAL_H_
#define _TDAL_SEC_CHAL_H_

#include "MsTypes.h"
#include "list.h"

#define TSID_INVALID                ((unsigned int) -1)
#define LIST_INDEX_INVALID          ((index_t)0xffffffff)

#define SEC_MUTEX_TYPE              MS_S32
#define SEC_MUTEX_INITIALIZER       0

#define SEC_MUTEX_DECLARE(mutex)                SEC_MUTEX_TYPE mutex = SEC_MUTEX_INITIALIZER
#define SEC_MUTEX_DECLARE_REF(mutex_p, mutex)   SEC_MUTEX_TYPE * mutex_p = &(mutex)

#define SEC_MUTEX_INIT(mutex_p)   SEC_NO_ERROR
#define SEC_MUTEX_LOCK(mutex_p)   (MsOS_ObtainMutex(*mutex_p, MSOS_WAIT_FOREVER) ? eTDAL_SEC_NOT_DONE : eTDAL_SEC_NO_ERROR)
#define SEC_MUTEX_UNLOCK(mutex_p) (MsOS_ReleaseMutex(*mutex_p) ? eTDAL_SEC_NOT_DONE : eTDAL_SEC_NO_ERROR)

typedef enum
{
    EMI_NGA_MPEG_DVB_CSA2             = 0x0000,
    EMI_NGA_MPEG_DVB_CSA3             = 0x0001,

    EMI_NGA_MPEG_ASA64                = 0x0010,
    EMI_NGA_MPEG_ASA128               = 0x0011,

    EMI_NGA_MPEG_AES128_IDSA          = 0x0020,
    EMI_NGA_MPEG_AES128_ECB_CLR       = 0x0021,
    EMI_NGA_MPEG_AES128_CBC_CLR       = 0x0022,

    EMI_NGA_MPEG_TDES128_CBC_DVS042   = 0x0040,
    EMI_NGA_MPEG_TDES128_ECB_CLR      = 0x0041,


    EMI_NGA_AES128_CBC_CLR_IV0        = 0x4020,
    EMI_NGA_AES128_ECB_CLR            = 0x4021,
    EMI_NGA_AES128_CBC_DASH           = 0x4022,
    EMI_NGA_AES128_CBC_APPLE          = 0x4023,
    EMI_NGA_DASH_AES128_CTR           = 0x4024,
    EMI_NGA_AES128_CBC_CLR            = 0x4026,
    EMI_NGA_AES128_CTR                = 0x4027,

    EMI_NGA_TDES128_CBC_CLR_IV0       = 0x4040,
    EMI_NGA_TDES128_ECB_CLR           = 0x4041,
    EMI_NGA_TDES128_CBC_CLR           = 0x4043,

    EMI_PLTF_MPEG_ESA                 = 0x8000,
    EMI_PLTF_MPEG_TSA                 = 0x8001,

    EMI_RESERVED                      = 0xffff,
} TEmiAlgo_t;

typedef enum
{
    CIPHER_SESSION_TYPE_RAM2RAM = 0,
    CIPHER_SESSION_TYPE_STREAM  = 1,
    CIPHER_SESSION_TYPE_INVALID = 2,
} ECipherSessionType;

typedef struct SMstarSecStreamSession
{
    unsigned int   xDscmbID;
    unsigned int   xDmaID;
    unsigned char  xIsDmaAlloc;
    unsigned short xEmi;
    unsigned char  xIsDsc;
}TMstarSecStreamSession;

struct SSecCipherSession
{
    ECipherSessionType             xType;               /**< Session Type: RAM2RAM or Stream. */
    unsigned char                  xIsEncrypt;          /**< Type of operation: encryption or decryption. */
    TMstarSecStreamSession *       pxTransportSession;  /**< Transport Session pointer (for Stream Session type only) */
    unsigned short                 xEmi;                /**< Algorithm/EMI */

    size_t                         xContentKeySize;     //keep key size
    unsigned char                  xClearTextKey[16];   //keep clear text key
    unsigned char *                pxClearTextKey;
    unsigned int                   xDscmbID;
    unsigned int                   xDmaID;
    unsigned char                  xIsDmaAlloc;

    /* Do we need to cache the following params? */
    size_t                         xKeyIdSize;          /**<  */
    unsigned char *                pxKeyId;             /**<  */
    size_t                         xMetadataSize;       /**<  */
    const unsigned char *          pxMetadata;          /**<  */
};
typedef struct SSecCipherSession* tDalSecCipherSession;

tTDAL_SEC_CertStatus chalCertExchange(tTDAL_SEC_CertCommand* pxCommand);
void chalCERTInit(void);
tTDAL_SEC_ErrorCode chalPrivSecOpenSession(tDalSecCipherSession *pxSession, tDalSecCipherSession *ppxSecCipherSession, ECipherSessionType xType, unsigned char xIsEncrypt, unsigned int xTransportSessionId);
tTDAL_SEC_ErrorCode chalPrivSecCloseSession(tDalSecCipherSession xSession);
tTDAL_SEC_ErrorCode chalPrivSecSetEncryptionMethod(tDalSecCipherSession xSession, unsigned short xEmi);
tTDAL_SEC_ErrorCode chalPrivSecSessionEncryptDecrypt(tDalSecCipherSession xSession, const unsigned char *pxInput, unsigned char *pxOutput, size_t xMessageSize, const unsigned char *pxInitVector, size_t xInitVectorSize, unsigned char xIsEncrypt);
tTDAL_SEC_ErrorCode chalSecSetEncryptDecryptCertKey(tDalSecCipherSession xSession, size_t xKeyIdSize, unsigned char *pxKeyId, MS_BOOL IsEncrypt);

extern MS_S32 gs32NonCachedPoolID;

#endif /* _TDAL_SEC_CHAL_H_ */
