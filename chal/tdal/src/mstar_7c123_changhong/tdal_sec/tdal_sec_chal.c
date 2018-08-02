#include <stdio.h>
#include <stdlib.h>

#include "tbox.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "drvAKL.h"
#include "drvCIPHER.h"
#include "drvNGA.h"
#include "list.h"
#include "tdal_sec.h"
#include "tdal_sec_chal.h"

static MS_U32 u32RIUBase;

#define CA_AKL1_BANK (0x1A1A)
#define CA_AKL0_BANK (0x1A19)

#define AKLBASE (u32RIUBase + (CA_AKL0_BANK<<9))
#define CERTBASE (u32RIUBase + (CA_AKL1_BANK<<9))

#define REG16(add) *((volatile unsigned short *)(add))

#define AKL0_REG32(_offset) (*(volatile unsigned int *  )(AKLBASE+((_offset)<<2)))
#define AKL1_REG32(_offset) (*(volatile unsigned int *  )(CERTBASE+((_offset)<<2)))

#define STATUS_CMD(status) (0 & 1)
#define CERT_DATA_WORDS (8)

//Offset
#define REG_AKL0_RESET (0x06)
#define REG_AKL1_CERT_COMMAND (0x11)
#define REG_AKL1_CERT_STATUS_AKL (0x10)

//Register
#define REG_CERT_RESET_AKL (0x01)
#define REG_CERT_RESET_ND  (0x10)

#define DATA_IN(data, index)    do {                                             \
                                    AKL1_REG32(index) = ((data[index*4+0])<<24)  \
                                                       |((data[index*4+1])<<16)  \
                                                       |((data[index*4+2])<<8)  \
                                                       |((data[index*4+3])<<0); \
                                }while(0);

/////////////////////////
#define COMMAND(opcode)         do {                           \
                                    AKL1_REG32(0x11) =  ((opcode[0])<<24)  \
                                                       |((opcode[1])<<16)  \
                                                       |((opcode[2])<<8)   \
                                                       |((opcode[3])<<0);  \
                                }while(0);

/////////////////////////
#define DATA_OUT(data, index)   do {                                              \
                                    data[index*4+0] = (AKL1_REG32(index+8)>>24 & 0xFF); \
                                    data[index*4+1] = (AKL1_REG32(index+8)>>16 & 0xFF); \
                                    data[index*4+2] = (AKL1_REG32(index+8)>>8 & 0xFF);  \
                                    data[index*4+3] = (AKL1_REG32(index+8)>>0 & 0xFF);  \
                                }while(0);

/////////////////////////
#define STATUS_AKL(status)      do {                                           \
                                    status[0] = (AKL1_REG32(REG_AKL1_CERT_STATUS_AKL)>>24 & 0xFF); \
                                    status[1] = (AKL1_REG32(REG_AKL1_CERT_STATUS_AKL)>>16 & 0xFF); \
                                    status[2] = (AKL1_REG32(REG_AKL1_CERT_STATUS_AKL)>>8 & 0xFF);  \
                                    status[3] = (AKL1_REG32(REG_AKL1_CERT_STATUS_AKL)>>0 & 0xFF);  \
                                }while(0);

#define SEC_SESSIONS            256                 // Total number of sessions
#define SEC_KEY_LADDER_LEVEL    3
#define SEC_SESSION_MUTEXES     SEC_SESSIONS

#define _AllocDmaKeySlot(x) _AllocDmaKeySlotEx((MS_U32*)(x))

/* Declare list of session handles */
static LIST(gTdalSecSessionList, SEC_SESSIONS);

static SEC_MUTEX_TYPE gSessionMutex[SEC_SESSION_MUTEXES] = { [0 ... (SEC_SESSION_MUTEXES-1)] = SEC_MUTEX_INITIALIZER};

/* Declare Session List mutex */
SEC_MUTEX_DECLARE(gSessionListMutex);

static tTDAL_SEC_CertStatus chalCertExchangeTimeout(tTDAL_SEC_CertTimeoutType timeout)
{
	// May need to cache CERT status if read is destructive?
	MS_S32 time;
    switch (timeout)
    {
    case eTDAL_SEC_CERT_TIMEOUT_DEFAULT:
        time = 100;
        break;
    case eTDAL_SEC_CERT_TIMEOUT_OTP:
        time = 100;
        break;
    default:
        time = 100;
    }

    //Offset 0x11, CERT Command
    //Wait until the proper boot-up sequence or the previous CERT command is completed, by
    //polling the busy bit (bit 0 of the CERT COMMAND register) until it is equal to 0.
    while (0 != (AKL1_REG32(REG_AKL1_CERT_COMMAND) & 0x1))
    {
        MsOS_DelayTask(1);
        time--;
        if (time <= 0)
        {
            return eTDAL_SEC_CERT_ERROR_TIMEOUT;
        }
    }

	return eTDAL_SEC_CERT_NO_ERROR;
}

static tTDAL_SEC_CertStatus chalCertStatusAKL(unsigned char *status)
{
	if (!status)
	{
		return eTDAL_SEC_CERT_ERROR;
	}

	STATUS_AKL(status);

    //Read the STATUS_AKL register; if the bit 0 of the STATUS_AKL register is set, it means that
    //the previous command finished with an error.
    if (status[3] & 0x01)
    {
		return eTDAL_SEC_CERT_ERROR;
    }

	return eTDAL_SEC_CERT_NO_ERROR;
}

tTDAL_SEC_CertStatus chalCertExchange(tTDAL_SEC_CertCommand* pxCommand)
{
	if(!pxCommand)
	{
		return eTDAL_SEC_CERT_NO_ERROR;
	}

	int i;

	tTDAL_SEC_CertStatus ret = 0;

    //Step.1
    //Wait until the proper boot-up sequence or the previous CERT command is completed, by
    //polling the busy bit (bit 0 of the CERT COMMAND register) until it is equal to 0.
    if (eTDAL_SEC_CERT_ERROR_TIMEOUT == chalCertExchangeTimeout(pxCommand->timeout))
    {
		return eTDAL_SEC_CERT_ERROR_TIMEOUT;
    }

    // Write the 8 32-bit CERT input register (DATA_IN_X)
	for (i = 0; i < CERT_DATA_WORDS; i++)
	{
		DATA_IN(pxCommand->inputData, i);
	}

    // Write the 32-bit CERT command
	COMMAND(pxCommand->opcodes);

    // Wait for the command processing completion
	if (eTDAL_SEC_CERT_ERROR_TIMEOUT == chalCertExchangeTimeout(pxCommand->timeout))
	{
		return eTDAL_SEC_CERT_ERROR_TIMEOUT;
	}

    ret = chalCertStatusAKL(pxCommand->status);

    // Read the 8 32-bit CERT ouput register (DATA_OUT_X)
	for (i = 0; i < CERT_DATA_WORDS; i++)
	{
		DATA_OUT(pxCommand->outputData, i);
	}

	return ret;
}

void chalCERTInit(void)
{
    u32RIUBase = 0xBF000000;

    // Enable AKL & ND
    AKL0_REG32(REG_AKL0_RESET) |= (REG_CERT_RESET_AKL|REG_CERT_RESET_ND);
}

tTDAL_SEC_ErrorCode chalPrivSecOpenSession(tDalSecCipherSession *pxSession, tDalSecCipherSession *ppxSecCipherSession, ECipherSessionType xType, unsigned char xIsEncrypt, unsigned int xTransportSessionId)
{
    tDalSecCipherSession pxSecCipherSession = NULL;
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR;

    /* Validate input parameter */
    if (NULL == pxSession)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if (NULL == ppxSecCipherSession)
    {
        return eTDAL_SEC_NOT_DONE; /* internal error */
    }

    pxSecCipherSession = malloc(sizeof(struct SSecCipherSession));
    if (NULL == pxSecCipherSession)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    /* Initialise session */
    memset(pxSecCipherSession, 0, sizeof(struct SSecCipherSession));
    pxSecCipherSession->xEmi = EMI_RESERVED;
    pxSecCipherSession->xIsDmaAlloc = FALSE;
    pxSecCipherSession->xType = xType;
    pxSecCipherSession->xIsEncrypt = xIsEncrypt;
    if (TSID_INVALID != xTransportSessionId)
    {
        pxSecCipherSession->xDscmbID = pxSecCipherSession->pxTransportSession->xDscmbID;
    }
    else
    {
        pxSecCipherSession->pxTransportSession = (TMstarSecStreamSession *)xTransportSessionId;
    }
    pxSecCipherSession->pxClearTextKey = NULL;

    e = SEC_MUTEX_LOCK(&gSessionListMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }

    index_t index = ListAdd(gTdalSecSessionList, pxSecCipherSession);

    e = SEC_MUTEX_UNLOCK(&gSessionListMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }
    if (LIST_INDEX_INVALID == index)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    *ppxSecCipherSession = pxSecCipherSession;
    *pxSession = (tDalSecCipherSession)index;

    return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode chalPrivSecCloseSession(tDalSecCipherSession xSession)
{
    tDalSecCipherSession pxSecCipherSession = NULL;
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR;

    do
    {
        /* Validate session handle and retrieve associated session */
        e = SEC_MUTEX_LOCK(&gSessionListMutex);
        if (eTDAL_SEC_NO_ERROR != e)
        {
            break;
        }

        pxSecCipherSession = ListRemove(gTdalSecSessionList, (index_t)xSession);
        e = SEC_MUTEX_UNLOCK(&gSessionListMutex);
        if (eTDAL_SEC_NO_ERROR != e)
        {
            break;
        }
    } while(0);

    if (NULL == pxSecCipherSession)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    if (pxSecCipherSession->xType != CIPHER_SESSION_TYPE_RAM2RAM)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    if ((pxSecCipherSession->xIsDmaAlloc) && (FALSE == MDrv_NGA_R2RFree(pxSecCipherSession->xDmaID)))
    {
        return eTDAL_SEC_NOT_DONE;
    }
    else
    {
        pxSecCipherSession->xIsDmaAlloc = FALSE;
    }

    free(pxSecCipherSession);
    xSession = NULL;

    return eTDAL_SEC_NO_ERROR;
}

static tTDAL_SEC_ErrorCode _AllocDmaKeySlotEx(MS_U32 *pxDmaID)
{
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR;
    MS_U32 u32TimeOut = MsOS_GetSystemTime() + 3500;

    //do more retry for slot full (4?) situation
    while (MsOS_GetSystemTime() < u32TimeOut)
    {
        if (FALSE == MDrv_NGA_R2RAlloc(pxDmaID))
        {
            *pxDmaID = 0xFFFFFFFF;
            e = eTDAL_SEC_NOT_DONE;
            MsOS_DelayTask(100);
        }
        else
        {
            e = eTDAL_SEC_NO_ERROR;
            break;
        }
    }

    return e;
}

static tTDAL_SEC_ErrorCode chalPrivGetSecCipherSession(tDalSecCipherSession xSession, tDalSecCipherSession *ppxSecCipherSession)
{
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR;
    tDalSecCipherSession pxSecCipherSession = NULL;

    e = SEC_MUTEX_LOCK(&gSessionListMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }

    pxSecCipherSession = ListGet(gTdalSecSessionList, (index_t)xSession);
    *ppxSecCipherSession = pxSecCipherSession;

    e = SEC_MUTEX_UNLOCK(&gSessionListMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }

    return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode chalPrivSecSetEncryptionMethod(tDalSecCipherSession xSession, unsigned short xEmi)
{
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR, e2 = eTDAL_SEC_NO_ERROR;
    tDalSecCipherSession pxSecCipherSession;

    SEC_MUTEX_DECLARE_REF(pxSessionMutex, gSessionMutex[(unsigned int)xSession % SEC_SESSION_MUTEXES]);

    e = SEC_MUTEX_LOCK(pxSessionMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }

    chalPrivGetSecCipherSession(xSession, &pxSecCipherSession);

    /* Save info for this session */
    pxSecCipherSession->xEmi = xEmi;

    /* Set session type */
    if (xEmi < 0x4000)
    {
        pxSecCipherSession->xType = CIPHER_SESSION_TYPE_STREAM;
    }
    else
    {
        pxSecCipherSession->xType = CIPHER_SESSION_TYPE_RAM2RAM;
    }

    e2 = SEC_MUTEX_UNLOCK(pxSessionMutex);

    return e;
}

static size_t chalEmiGetModifiableIVSize(unsigned short xEmi)
{
    size_t iv_size = 0;

    switch (xEmi)
    {
    case EMI_NGA_MPEG_AES128_CBC_CLR:
    case EMI_NGA_AES128_CBC_DASH:
    case EMI_NGA_AES128_CBC_APPLE:
    case EMI_NGA_AES128_CBC_CLR:
    case EMI_NGA_AES128_CTR:
        iv_size = 16;
        break;

    case EMI_NGA_TDES128_CBC_CLR:
        iv_size = 8;
        break;
    }

    return iv_size;
}

static tTDAL_SEC_ErrorCode chalSecParamCheck_secSessionEncryptDecrypt(tDalSecCipherSession pxSecCipherSession, const unsigned char *pxInput, const unsigned char *pxOutput, size_t xMessageSize, const unsigned char *pxInitVector, size_t xInitVectorSize, unsigned char xIsEncrypt)
{
    unsigned short xEmi = EMI_RESERVED;

    /* Validate session handle and retrieve associated session */
    if (NULL == pxSecCipherSession)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if (CIPHER_SESSION_TYPE_RAM2RAM != pxSecCipherSession->xType)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if (xIsEncrypt != pxSecCipherSession->xIsEncrypt)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    /* Validate input/output buffers */
    if (NULL == pxInput)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if (NULL == pxOutput)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if (0 == xMessageSize)
    {
        /* Unspecified, To review: or SEC_ERROR or SEC_NO_ERROR? */
        return eTDAL_SEC_NOT_DONE;
    }

    /* Validate the IV */
    xEmi = pxSecCipherSession->xEmi;
    if (EMI_RESERVED == xEmi)
    {
        /* could be replaced by proper state machine, hence better SEC_ERROR than SEC_ERROR_BAD_PARAMETER */
        return eTDAL_SEC_NOT_DONE;
    }
    if (chalEmiGetModifiableIVSize(xEmi) != xInitVectorSize)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    /* If no IV is required (ECB mode), xInitVectorSize is expected to be set zero and
     *  pxInitVector is expected to be set to NULL. */

    /* Check IV parameters consistency */
    if ((0 != xInitVectorSize) && (NULL == pxInitVector))
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if ((0 == xInitVectorSize) && (NULL != pxInitVector))
    {
        return eTDAL_SEC_NOT_DONE;
    }

    return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode chalPrivSecSessionEncryptDecrypt(tDalSecCipherSession xSession, const unsigned char *pxInput, unsigned char *pxOutput, size_t xMessageSize, const unsigned char *pxInitVector, size_t xInitVectorSize, unsigned char xIsEncrypt)
{
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR, e2 = eTDAL_SEC_NO_ERROR;
    tDalSecCipherSession pxSecCipherSession = NULL;

    SEC_MUTEX_DECLARE_REF(pxSessionMutex, gSessionMutex[(unsigned int)xSession % SEC_SESSION_MUTEXES]);

    e = SEC_MUTEX_LOCK(pxSessionMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }
    do
    {
        /* Validate session handle and retrieve associated session */
        e = chalPrivGetSecCipherSession(xSession, &pxSecCipherSession);
        if (eTDAL_SEC_NO_ERROR != e)
        {
            break;
        }

        /* Validate other parameters */
        e = chalSecParamCheck_secSessionEncryptDecrypt(pxSecCipherSession, pxInput, pxOutput, xMessageSize, pxInitVector, xInitVectorSize, xIsEncrypt);
        if (eTDAL_SEC_NO_ERROR != e)
        {
            break;
        }

        MS_U8 *puBuf = MsOS_AllocateMemory(xMessageSize, gs32NonCachedPoolID);
        if (NULL == puBuf)
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        memset(puBuf, 0, xMessageSize);
        memcpy(puBuf, pxInput, xMessageSize);

        /* At last, let's do what we came here for. */
        if (FALSE == MDrv_NGA_R2R_UpdateIV((MS_U8 *)pxInitVector, xInitVectorSize, pxSecCipherSession->xDmaID))
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        if (FALSE == MDrv_NGA_R2R_Fire(puBuf, puBuf, xMessageSize, pxSecCipherSession->xDmaID))
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        memcpy(pxOutput, puBuf, xMessageSize);
        MsOS_FreeMemory(puBuf, gs32NonCachedPoolID);
    } while (0);
    e2 = SEC_MUTEX_UNLOCK(pxSessionMutex);

    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }
    if (eTDAL_SEC_NO_ERROR != e2)
    {
        return e2;
    }

    return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode chalSecSetEncryptDecryptCertKey(tDalSecCipherSession xSession, size_t xKeyIdSize, unsigned char *pxKeyId, MS_BOOL IsEncrypt)
{
    tTDAL_SEC_ErrorCode e = eTDAL_SEC_NO_ERROR, e2 = eTDAL_SEC_NO_ERROR;
    tDalSecCipherSession pxSecCipherSession = NULL;
    DRV_CIPHER_DMACFG stCfg;
    MS_BOOL bRet = TRUE;
    MS_U8 *puBuf_In = NULL, *puBuf_Out = NULL;
    MS_U8 pxInitVector[16] = {0,};
    MS_U32 u32CmdId = 0, u32Exception = 0;

    SEC_MUTEX_DECLARE_REF(pxSessionMutex, gSessionMutex[(unsigned int)xSession % SEC_SESSION_MUTEXES]);

    e = SEC_MUTEX_LOCK(pxSessionMutex);
    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }

    do
    {
        /* Validate session handle and retrieve associated session */
        e = chalPrivGetSecCipherSession(xSession, &pxSecCipherSession);
        if (eTDAL_SEC_NO_ERROR != e)
        {
            break;
        }

        if (CIPHER_SESSION_TYPE_STREAM == pxSecCipherSession->xType)
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        pxSecCipherSession->xIsEncrypt = IsEncrypt;

        //allocate DMA keyslot if we have not do
        if (FALSE == pxSecCipherSession->xIsDmaAlloc)
        {
            e = _AllocDmaKeySlot(&pxSecCipherSession->xDmaID);
            if (eTDAL_SEC_NO_ERROR != e)
            {
                break;
            }
            pxSecCipherSession->xIsDmaAlloc = TRUE;
        }

        //Init CERT AKL driver
        MDrv_AKL_Init();

        //cert key to DMA
        bRet = MDrv_AKL_SetKey2DMA(pxSecCipherSession->xDmaID);
        if (TRUE != bRet)
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        memset(&stCfg, 0, sizeof(DRV_CIPHER_DMACFG));
        stCfg.stKey.eKeySrc = E_CIPHER_KSRC_KL;
        stCfg.stKey.u8KeyIdx = pxSecCipherSession->xDmaID;
        MDrv_CIPHER_DMAConfigure(pxSecCipherSession->xDmaID, stCfg);

        if (FALSE == MDrv_NGA_R2R_SetAlgo(pxSecCipherSession->xEmi, pxSecCipherSession->xDmaID, !(pxSecCipherSession->xIsEncrypt)))
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        /* At last, let's do what we came here for. */
        if (FALSE == MDrv_NGA_R2R_UpdateIV((MS_U8 *)pxInitVector, 16, pxSecCipherSession->xDmaID))
        {
            e = eTDAL_SEC_NOT_DONE;
            break;
        }

        memset(&stCfg, 0, sizeof(DRV_CIPHER_DMACFG));

        puBuf_In = MsOS_AllocateMemory(16, gs32NonCachedPoolID);
        puBuf_Out = MsOS_AllocateMemory(16, gs32NonCachedPoolID);

        stCfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)puBuf_In);
        stCfg.stInput.u32Size = 16;

        stCfg.stOutput.u32Addr = MsOS_VA2PA((MS_U32)puBuf_Out);
        stCfg.stOutput.u32Size = 16;

        if (DRV_CIPHER_OK != MDrv_CIPHER_DMAConfigure(pxSecCipherSession->xDmaID, stCfg))
        {
            bRet = FALSE;
        }

        MsOS_FlushMemory();
        MsOS_ReadMemory();

        if (DRV_CIPHER_OK != MDrv_CIPHER_DMAStart(pxSecCipherSession->xDmaID, &u32CmdId))
        {
            bRet = FALSE;
        }

        while (FALSE == MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception))
        {
            MsOS_DelayTask(1);
        }

        MsOS_FlushMemory();
        MsOS_ReadMemory();

        if (u32Exception & DRV_CIPHER_EXCEPT_DMA_KEY)
        {
            e = eTDAL_SEC_NOT_DONE;
        }

        MsOS_FreeMemory(puBuf_In, gs32NonCachedPoolID);
        MsOS_FreeMemory(puBuf_Out, gs32NonCachedPoolID);
    } while (0);
    e2 = SEC_MUTEX_UNLOCK(pxSessionMutex);

    if (eTDAL_SEC_NO_ERROR != e)
    {
        return e;
    }
    if (eTDAL_SEC_NO_ERROR != e2)
    {
        return e2;
    }

    return eTDAL_SEC_NO_ERROR;
}
