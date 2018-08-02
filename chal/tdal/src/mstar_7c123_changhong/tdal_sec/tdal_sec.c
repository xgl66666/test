/********************************************************************************
 *                       COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ********************************************************************************
 *
 * MODULE NAME: TDAL_SEC
 *
 * FILE NAME: $URL:  $
 *            $Rev:  $
 *            $Date:  $
 *
 * DESCRIPTION : TDAL SECurity implementation
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <stdio.h>
#include "crules.h"
#include <string.h> /* for memcpy */
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_sec.h"
#include "tdal_desc.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "drvAKL.h"
#include "drvNGA.h"
#include "nocs_csd_impl.h"
#include "nocs_csd.h"
#include "tdal_sec_chal.h"
#include "drvCA.h"

/********************************************************
 *              Defines                                 *
 ********************************************************/

/********************************************************
 *              Local File Variables (LOCAL)            *
 ********************************************************/
mTBOX_SET_MODULE(eTDAL_SEC);

LOCAL TDAL_mutex_id TdalSecMutexID = NULL;
LOCAL TDAL_mutex_id TdalCertMutexID = NULL;

/* RAM2RAM Parameter initialization */
static MS_U8 KLD_CST[] = {0xA9, 0x32, 0x30, 0x31, 0x31, 0x4E, 0x61, 0x67, 0x72, 0x61, 0x76, 0x69, 0x73, 0x69, 0x6F, 0x6E};

static tTDAL_SEC_CertResourceHandle CERTResource = {.bLock = false};

extern char const * DBG_TCsdStatus( TCsdStatus s );
extern MS_S32 gs32NonCachedPoolID;

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * TDAL_SEC_Init
 *
 * Parameters:
 *		none
 * Description:
 *      initialize TDAL_SEC module
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_Init( void)
{
    TCsdStatus csdStatus;
    TCsd4BytesVector xNuid;
    TCsd4BytesVector xCheckNumber;
    TCsd20CharsString xChipRevision;
    mTBOX_FCT_ENTER("TDAL_SEC_Init");

    TDAL_CreateMutex(&TdalSecMutexID);
    mTBOX_ASSERT(TdalSecMutexID > 0);

    TDAL_CreateMutex(&TdalCertMutexID);
    mTBOX_ASSERT(TdalCertMutexID > 0);

    csdStatus = csdGetNuid(xNuid);
    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_WARNING,"csdGetNUID. Error code: %d.\n",csdStatus));

    }
    mTBOX_TRACE(( kTBOX_NIV_1, "NUID  : 0x%02X 0x%02X 0x%02X 0x%02X\n", xNuid[0],xNuid[1],xNuid[2],xNuid[3]));

    /** Get the NUID check number **/
    /*******************************/
    csdStatus = csdGetNUIDCheckNumber(xCheckNumber );

    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_WARNING,"csdGetCheckNumber. Error code: %d.\n",csdStatus));
    }
    mTBOX_TRACE(( kTBOX_NIV_1,"NUID CheckNumber : 0x%02X 0x%02X 0x%02X 0x%02X\n", xCheckNumber[0],xCheckNumber[1],xCheckNumber[2],xCheckNumber[3]));

    csdStatus = csdGetChipRevision(xChipRevision);
    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_WARNING,"csdGetChipRevision Error code: %d.\n",csdStatus));
    }
    mTBOX_TRACE(( kTBOX_NIV_1,"ChipRevision : %s\n", xChipRevision));

    chalCERTInit();

    mTBOX_RETURN(eTDAL_SEC_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_SEC_Terminate
 *
 * Parameters:
 *		none
 * Description:
 *      terminate TDAL_SEC module
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_Terminate( void)
{
    TDAL_DeleteMutex(TdalSecMutexID);
    TdalSecMutexID = NULL;

    TDAL_DeleteMutex(TdalCertMutexID);
    TdalCertMutexID = NULL;

    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_GetUniqueID
 *
 * Parameters:
 *
 * Description:
 *      get unique ID on each platform
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_GetUniqueID (tTDAL_SEC_Nuid* pUid)
{
    if(pUid!=NULL)
    { 
        if(csdGetNuid(*pUid)==CSD_NO_ERROR)
        {       
           return eTDAL_SEC_NO_ERROR;
        }
    }
    return eTDAL_SEC_NOT_DONE;
}

/*===========================================================================
 *
 * TDAL_SEC_GetUnique64ID
 *
 * Parameters:
 *
 * Description:
 *      This function returns a 64 bit unique ID. A different one for each boxes.
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_GetUnique64ID (tTDAL_SEC_Nuid64* pUid)
{
    if(pUid!=NULL)
    {
        if(csdGetNuid64(*pUid)==CSD_NO_ERROR)
        {
           return eTDAL_SEC_NO_ERROR;
        }
    }
    return eTDAL_SEC_NOT_DONE;
}

/*===========================================================================
 *
 * TDAL_SEC_GetChipRevision
 *
 * Parameters:
 *
 * Description:
 *      get chipset revision
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_GetChipRevision (char *pxChipRevision, size_t *pxChipRevisionLength)
{
    if(NULL!=pxChipRevision && NULL!=pxChipRevisionLength)
    {
        if(csdGetChipRevision(pxChipRevision)==CSD_NO_ERROR)
        {
            *pxChipRevisionLength = strlen(pxChipRevision);
            return eTDAL_SEC_NO_ERROR;
        }
    }
    return eTDAL_SEC_NOT_DONE;
}

/*===========================================================================
 *
 * TDAL_SEC_GetChipExtension
 *
 * Parameters:
 *
 * Description:
 *      This function returns a null-terminated string containing the extension of the chipset.
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_GetChipExtension (char *pxChipExtension, size_t *pxChipExtensionLength)
{
    if(NULL!=pxChipExtension && NULL!=pxChipExtensionLength)
    {
#if 0 // NO extension!!!
        if(csdGetChipExtension(pxChipExtension)==CSD_NO_ERROR)
        {
            *pxChipExtensionLength = strlen(pxChipExtension);
            return eTDAL_SEC_NO_ERROR;
        }
#else // fixed for MSD5C59
        strcpy(pxChipExtension, "x-A01-xxx");
        *pxChipExtensionLength = strlen(pxChipExtension);
        return eTDAL_SEC_NO_ERROR;
#endif
    }
    return eTDAL_SEC_NOT_DONE;
}

/*===========================================================================
 *
 * TDAL_SEC_Seed
 *
 * Parameters:
 *
 * Description:
 *      get a number for key diversification. Can be the same for all platform
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_GetSeed (uint8_t *pBuffer, int *pSize)
{
    return eTDAL_SEC_NO_ERROR;
}

void TDAL_SECi_InvalidateCacheAddr(uint8_t * p, size_t len)
{
    MS_U32 q = (MS_U32) p;
    uint8_t * startAddr = (uint8_t *)((q / 16) * 16);
    uint8_t * endAddr = (uint8_t *)(((q + len) / 16 + 1) * 16);
    size_t newLen = endAddr - startAddr;
    MS_BOOL r;

    r = MsOS_Dcache_Flush(MS_PA2KSEG0(MS_VA2PA((MS_U32) startAddr)),(MS_U32) newLen);
    if (!r)
    {
        mTBOX_TRACE(( kTBOX_NIV_1,"Cache invalidation failed \n"));
    }
    mTBOX_TRACE(( kTBOX_NIV_1,"Cache invalidation done \n"));
}

/*===========================================================================
 *
 * TDAL_SEC_Encryptdata
 *
 * Parameters:
 *		pBufferSrc : pointer to source buffer
 *		lenght : size of source and destination buffer
 * Description:
 *      fill destination buffer with encrypted version of a source buffer
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *      and
 *      encrypted data in destination buffer
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_Encryptdata(uint8_t *pBufferSrc, uint8_t *pBufferDst, size_t length)
{
    TCsdStatus csdStatus = CSD_NO_ERROR;
    MS_U32 u32CipherID = 0;
    MS_U8 *pu8Buf = NULL;

    if(pBufferSrc == NULL || pBufferDst == NULL)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if(length % 8 != 0)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    pu8Buf = MsOS_AllocateMemory(length, gs32NonCachedPoolID);
    if (NULL == pu8Buf)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    memset(pu8Buf, 0, length);
    memcpy(pu8Buf, pBufferSrc, length);

    if (TRUE != MDrv_NGA_R2RAlloc(&u32CipherID))
    {
        MsOS_FreeMemory(pu8Buf, gs32NonCachedPoolID);
        return eTDAL_SEC_NOT_DONE;
    }

    TDAL_LockMutex(TdalSecMutexID);

    do
    {
        if (FALSE == MDrv_NGA_R2R_SetContentKey(0x4041, sizeof(KLD_CST), KLD_CST, sizeof(KLD_CST), KLD_CST, KLD_CST, 0, NULL, u32CipherID, 3, FALSE))
        {
            csdStatus = CSD_ERROR;
            break;
        }
        if (FALSE == MDrv_NGA_R2R_Fire(pu8Buf, pu8Buf, length, u32CipherID))
        {
            csdStatus = CSD_ERROR;
            break;
        }
        memcpy(pBufferDst, pu8Buf, length);
    } while(0);

    if (FALSE == MDrv_NGA_R2RFree(u32CipherID))
    {
        csdStatus = CSD_ERROR;
    }

    MsOS_FreeMemory(pu8Buf, gs32NonCachedPoolID);

    TDAL_UnlockMutex(TdalSecMutexID);

    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Encryptdata csdEncryptData, ERROR %s\n", DBG_TCsdStatus(csdStatus)));
        return eTDAL_SEC_NOT_DONE;
    }

    mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Encryptdata csdEncryptData %s\n", DBG_TCsdStatus(csdStatus)));
    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_Decryptdata
 *
 * Parameters:
 *		pBufferSrc : pointer to source buffer
 *		lenght : size of source and destination buffer
 * Description:
 *      fill destination buffer with decrypted version of a source buffer
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *      and
 *      decrypted data in destination buffer
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_Decryptdata(uint8_t *pBufferSrc, uint8_t *pBufferDst, size_t length)
{
    TCsdStatus csdStatus = CSD_NO_ERROR;
    MS_U32 u32CipherID = 0;
    MS_U8 *pu8Buf = NULL;

    if(pBufferSrc == NULL || pBufferDst == NULL)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if(length % 8 != 0)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    pu8Buf = MsOS_AllocateMemory(length, gs32NonCachedPoolID);
    if (NULL == pu8Buf)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    memset(pu8Buf, 0, length);
    memcpy(pu8Buf, pBufferSrc, length);

    if (TRUE != MDrv_NGA_R2RAlloc(&u32CipherID))
    {
        MsOS_FreeMemory(pu8Buf, gs32NonCachedPoolID);
        return eTDAL_SEC_NOT_DONE;
    }

    TDAL_LockMutex(TdalSecMutexID);

    do
    {
        if (FALSE == MDrv_NGA_R2R_SetContentKey(0x4041, sizeof(KLD_CST), KLD_CST, sizeof(KLD_CST), KLD_CST, KLD_CST, 0, NULL, u32CipherID, 3, TRUE))
        {
            csdStatus = CSD_ERROR;
            break;
        }
        if (FALSE == MDrv_NGA_R2R_Fire(pu8Buf, pu8Buf, length, u32CipherID))
        {
            csdStatus = CSD_ERROR;
            break;
        }
        memcpy(pBufferDst, pu8Buf, length);
    } while(0);

    if (FALSE == MDrv_NGA_R2RFree(u32CipherID))
    {
        csdStatus = CSD_ERROR;
    }

    MsOS_FreeMemory(pu8Buf, gs32NonCachedPoolID);

    TDAL_UnlockMutex(TdalSecMutexID);

    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Decryptdata csdDecryptData, ERROR %s\n", DBG_TCsdStatus(csdStatus)));
        return eTDAL_SEC_NOT_DONE;
    }

    mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Decryptdata csdDecryptData %s\n", DBG_TCsdStatus(csdStatus)));
    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_SetEncryptClearTextKey
 *
 * Parameters:
 *		pxClearTextKey : Buffer containing the clear-text key. The buffer is allocated by the caller.
 *		lenght : Size in bytes of the clear-text key
 * Description:
 *      This function sets a clear-text key in a crypto-engine for RAM2RAM encrypt operations.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptClearTextKey(uint8_t *pxClearTextKey, size_t length)
{
    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_SetDecryptClearTextKey
 *
 * Parameters:
 *		pxClearTextKey : Buffer containing the clear-text key. The buffer is allocated by the caller.
 *		lenght : Size in bytes of the clear-text key
 * Description:
 *      This function sets a clear-text key in a crypto-engine for RAM2RAM decrypt operations.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_SetDecryptClearTextKey(uint8_t *pxClearTextKey, size_t length)
{
    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_EncryptWithIV
 *
 * Parameters:
 *      sessionId: ID of opened encrypt session
 *		pxInput: Pointer to source buffer
 *		pxOutput: Pointer to output buffer
 *      xMessageSize: Size of source and destination buffer
 *      pxInitVector: Pointer to buffer with IV
 *      xInitVectorSize: Size of IV buffer
 * Description:
 *      This function performs a RAM2RAM data encryption. It encrypts the data contained in pxInput buffer and stores the resulted encrypted data in pxOutput buffer.
 *      This function may be called several times during a given session in order to encrypt a message split in several chunks.
 *      If the block cipher modes of operation requests an initialization vector (IV), the caller is in charge of the consistency of the IV passed at each chunk.
 *      The size of message to be encrypted does not need to be a multiple of the cipher block size.
 *      In such a case, the n residual bytes (with n = xMessageSize mod cipher block size) are left as is and simply copied from input to output buffer provided the mode of operation is ECB or CBC.
 *      For CTR mode, encryption actually consists in a simple XOR, thus residual bytes are encrypted as any other bytes of the message. This function does not manage any padding.
 *      If the plaintext message was padded by the client, it is also in charge of removing it after the return of this function call.
 *      If the caller provides the same input and output buffer (pxInput=pxOutput), the function must perform an in-place encryption.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error and encrypted data in destination buffer
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_EncryptWithIV(uint32_t sessionId, uint8_t *pxInput, uint8_t *pxOutput, size_t xMessageSize, uint8_t *pxInitVector, size_t xInitVectorSize)
{
    if (chalPrivSecSessionEncryptDecrypt((tDalSecCipherSession)sessionId, pxInput, pxOutput, xMessageSize, pxInitVector, xInitVectorSize, TRUE) == eTDAL_SEC_NO_ERROR)
    {
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_DecryptWithIV
 *
 * Parameters:
 *      sessionId: ID of opened decrypt session
 *		pxInput: Pointer to source buffer
 *		pxOutput: Pointer to output buffer
 *      xMessageSize: Size of source and destination buffer
 *      pxInitVector: Pointer to buffer with IV
 *      xInitVectorSize: Size of IV buffer
 * Description:
 *      This function performs a RAM2RAM data decryption. It decrypts the data contained in pxInput buffer and stores the resulted decrypted data in pxOutput buffer.
 *      This function may be called several times during a given session in order to decrypt a message split in several chunks.
 *      If the block cipher modes of operation requests an initialization vector (IV), the caller is in charge of the consistency of the IV passed at each chunk.
 *      The size of message to be decrypted does not need to be a multiple of the cipher block size.
 *      In such a case, the n residual bytes (with n = xMessageSize mod cipher block size) are left as is and simply copied from input to output buffer provided the mode of operation is ECB or CBC.
 *      For CTR mode, decryption actually consists in a simple XOR, thus residual bytes are decrypted as any other bytes of the message. This function does not manage any padding.
 *      If the plaintext message was padded by the client, it is also in charge of removing it after the return of this function call.
 *      If the caller provides the same input and output buffer (pxInput=pxOutput), the function must perform an in-place decryption.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error and decrypted data in destination buffer
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_DecryptWithIV(uint32_t sessionId, uint8_t *pxInput, uint8_t *pxOutput, size_t xMessageSize, uint8_t *pxInitVector, size_t xInitVectorSize)
{
    if (chalPrivSecSessionEncryptDecrypt((tDalSecCipherSession)sessionId, pxInput, pxOutput, xMessageSize, pxInitVector, xInitVectorSize, FALSE) == eTDAL_SEC_NO_ERROR)
    {
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_SetDecryptCertKey
 *
 * Parameters:
 *      sessionId: ID of opened decrypt session
 *      xKeyIdSize: size of key buffer
 *      pxKeyId: pointer to bufer with keyId
 * Description:
 *      This function copies the key available on the CERT key output interface in a crypto-engine for RAM2RAM decryption operations.
 *      The EMI provided as input parameter clearly identifies the cryptographic algorithm and chaining mode.
 *      At this stage the driver have all the information required to allocate hardware resources. This function also checks the usage rules related to the key.
 *      Finally, it acknowledges the CERT block to allow it to output another key. EMIs to be supported depend on the type of session.
 *      RAM2RAM sessions cover all EMIs 0x40xx while stream processing sessions cover all EMIs 0x40xx and all MPEG TS EMIs 0x00xx, including ASA.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_SetDecryptCertKey(uint32_t sessionId, size_t xKeyIdSize, int8_t *pxKeyId)
{
    if (chalSecSetEncryptDecryptCertKey((tDalSecCipherSession)sessionId, xKeyIdSize, pxKeyId, FALSE) == eTDAL_SEC_NO_ERROR)
    {
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_SetEncryptCertKey
 *
 * Parameters:
 *      sessionId: ID of opened encrypt session
 *      xKeyIdSize: size of key buffer
 *      pxKeyId: pointer to bufer with keyId
 * Description:
 *      This function copies the key available on the CERT key output interface in a crypto-engine for RAM2RAM encryption operations.
 *      The EMI provided as input parameter clearly identifies the cryptographic algorithm and chaining mode.
 *      At this stage the driver have all the information required to allocate hardware resources. This function also checks the usage rules related to the key.
 *      Finally, it acknowledges the CERT block to allow it to output another key. EMIs to be supported depend on the type of session.
 *      RAM2RAM sessions cover all EMIs 0x40xx while stream processing sessions cover all EMIs 0x40xx and all MPEG TS EMIs 0x00xx, including ASA.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptCertKey(uint32_t sessionId, size_t xKeyIdSize, int8_t *pxKeyId)
{
    if (chalSecSetEncryptDecryptCertKey((tDalSecCipherSession)sessionId, xKeyIdSize, pxKeyId, TRUE) == eTDAL_SEC_NO_ERROR)
    {
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_OpenSession
 *
 * Parameters:
 *      sessionId: pointer of allocated session
 * Description:
 *      This function allocate encrypt/decrypt session in chal.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_OpenSession(uint32_t *sessionId)
{
    tDalSecCipherSession pxSecCipherSession = NULL;

    if (sessionId == NULL)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    *sessionId = LIST_INDEX_INVALID;

    if (chalPrivSecOpenSession((tDalSecCipherSession *)sessionId, &pxSecCipherSession, CIPHER_SESSION_TYPE_INVALID, FALSE, TSID_INVALID) == eTDAL_SEC_NO_ERROR)
    {
        pxSecCipherSession->xIsDmaAlloc = FALSE;
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_CloseSession
 *
 * Parameters:
 *      sessionId: ID of allocated session
 * Description:
 *      This function release encrypt/decrypt session in chal.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_CloseSession(uint32_t sessionId)
{
    if (chalPrivSecCloseSession((tDalSecCipherSession)sessionId) == eTDAL_SEC_NO_ERROR)
    {
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_SetEncryptionMethod
 *
 * Parameters:
 *      sessionId: ID of allocated session
 *      xEmi: encryption method
 * Description:
 *      This function sets encryption method for encrypt/decrypt session.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptionMethod(uint32_t sessionId, uint16_t xEmi)
{
    if (chalPrivSecSetEncryptionMethod((tDalSecCipherSession)sessionId, xEmi) == eTDAL_SEC_NO_ERROR)
    {
        return eTDAL_SEC_NO_ERROR;
    }
    else
    {
        return eTDAL_SEC_NOT_DONE;
    }
}

/*===========================================================================
 *
 * TDAL_SEC_SendManualAckToCert
 *
 * Parameters:
 * Description:
 *      This function sends manual acknowledgment to CERT block.
 *      The CERT block must be acknowledged even in case the EMI provided is not supported or in case of bad key usage.
 *      Otherwise the CERT block will no longer be able output keys on the key output interface.
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_SendManualAckToCert(void)
{
    MDrv_AKL_Init();
    MDrv_AKL_SetManualACKMode(TRUE);
    MDrv_AKL_SetManualACK();
    MDrv_AKL_SetManualACKMode(FALSE);

    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_GenerateRandomBytes
 *
 * Parameters:
 *      pxOutput: Buffer where to write the xInitVectorSize generated random bytes.
 *      xInitVectorSize: Number of random bytes to be generated. The maximum number is limited to 1024 bytes.
 * Description:
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       if error occurs
 *      and
 *      random bytes in pxOutput buffer
 *
 *===========================================================================*/
tTDAL_SEC_ErrorCode TDAL_SEC_GenerateRandomBytes(uint8_t *pxOutput, size_t xInitVectorSize)
{
    unsigned char *buf_p = pxOutput;
    size_t len = xInitVectorSize;
    MS_U32 r;

    if (pxOutput == NULL)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if (xInitVectorSize > 1024 || xInitVectorSize == 0)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    while (len)
    {
        r = MDrv_CA_Random();

        *buf_p++ = r; r>>=8;
        if(--len == 0) break;
        *buf_p++ = r; r>>=8;
        if(--len == 0) break;
        *buf_p++ = r; r>>=8;
        if(--len == 0) break;
        *buf_p++ = r; r>>=8;
        if(--len == 0) break;
    }

    return eTDAL_SEC_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_CERTLock
 *
 * Parameters:
 *      [out] ppxResourceHandle: Handle assigned to the CERT resource.
 *
 * Description:
 *      This function reserves the CERT resource for exclusive use to the caller.
 *      It returns a handle to the CERT resource if it is not in use. Otherwise, the function blocks until the resource is freed.
 *
 *      CERTResource variable is used to handle case when TDAL_SEC_CERT_Unlock function is called while the CERT resource has not been locked.
 *      Explanation: since TDAL_SEC_CERT_Unlock doesn't have [out] we need internal variable to save locked state.
 *
 * Returns:
 *      eTDAL_SEC_CERT_NO_ERROR:   The CERT resource was locked successfully.
 *      eTDAL_SEC_CERT_ERROR:      Errors occurred and operation could not be completed successfully (e.g. ppxResourceHandle is NULL).
 *                                 This status has nothing to do with the availability of the CERT resource.
 *
 *===========================================================================*/
tTDAL_SEC_CertStatus TDAL_SEC_CERT_Lock(tTDAL_SEC_CertResourceHandle** ppxResourceHandle)
{
    if (ppxResourceHandle == NULL)
    {
        return eTDAL_SEC_CERT_ERROR;
    }

    TDAL_LockMutex(TdalCertMutexID);
    CERTResource.bLock = true;
    *ppxResourceHandle = &CERTResource;

    return eTDAL_SEC_CERT_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_CERT_Unlock
 *
 * Parameters:
 *      [in] pxResourceHandle: Handle to the CERT resource.
 *
 * Description:
 *      This function releases the CERT resource previously locked by TDAL_SEC_CERTLock().
 *
 *      CERTResource variable is used to handle case when TDAL_SEC_CERT_Unlock function is called while the CERT resource has not been locked.
 *      Explanation: since TDAL_SEC_CERT_Unlock doesn't have [out] we need internal variable to save locked state.
 *
 * Returns:
 *      eTDAL_SEC_CERT_NO_ERROR: The operation was completed successfully and the CERT lock released.
 *      eTDAL_SEC_CERT_ERROR_BAD_HANDLE: Handle provided is invalid or unknown.
 *                                       One typically drops in this case if this function is called while the CERT resource has not been locked.
 *      eTDAL_SEC_CERT_ERROR: Errors occurred and the operation could not be completed successfully.
 *
 *===========================================================================*/
tTDAL_SEC_CertStatus TDAL_SEC_CERT_Unlock(tTDAL_SEC_CertResourceHandle *pxResourceHandle)
{
    if (pxResourceHandle != (&CERTResource))
    {
        return eTDAL_SEC_CERT_ERROR_BAD_HANDLE;
    }
    if (pxResourceHandle->bLock == false)
    {
        return eTDAL_SEC_CERT_ERROR_BAD_HANDLE;
    }

    pxResourceHandle->bLock = false;
    TDAL_UnlockMutex(TdalCertMutexID);

    return eTDAL_SEC_CERT_NO_ERROR;
}

/*===========================================================================
 *
 * TDAL_SEC_CERT_Exchange
 *
 * Parameters:
 *      [in] pxResourceHandle: Handle to the CERT resource.
 *      [in] xNumOfCommands: Number of commands to be processed by the CERT block.
 *      [in,out] pxCommands: Buffer containing xNumOfCommands instantiations of tTDAL_SEC_CertCommand structure.
 *                           This structure is used to accommodate input parameters of each command as well as resulting output.
 *                           The memory is allocated by the caller.
 *      [out] pxNumOfProcessedCommands: Number of commands actually processed by the CERT block.
 *                                      If all commands are processed successfully, this number is equal to xNumOfCommands.
 *                                      If a command fails or times out, this parameter indicates the number of commands processed so far,
 *                                      including the faulty command but not the timed out command.
 *
 * Description:
 *      This function is used to send a series of commands to the CERT block.
 *      For each command part pxCommands buffer, the CERT driver has to systematically write the 8 32-bit CERT input registers (DATA_IN_X),
 *      wait for the command processing completion and read the 8 32-bit CERT output registers (DATA_OUT_X)
 *      as well as the 32-bit status register (STATUS_AKL).
 *      If a command fails (least significant bit of STATUS_AKL set to 1) or lasts longer than expected (timeout),
 *      TDAL_SEC_CERT_Exchange must return to the caller without handling remaining commands, if any.
 *      The number of commands processed so far, including the command that failed, must be signaled accordingly in pxNumOfProcessedCommands.
 *      A command that timed out is not considered as processed, thus must not be counted in pxNumOfProcessedCommands.
 *      Values of DATA_OUT_X registers and STATUS_AKL register of the faulty command must be returned as well.
 *
 * Returns:
 *      eTDAL_SEC_CERT_NO_ERROR: All commands have been processed successfully.
 *                               A command is successful if the least significant bit of STATUS_AKL is equal to 0 after the command completion.
 *      eTDAL_SEC_CERT_ERROR_BAD_HANDLE: Handle provided is invalid or unknown.
 *                                       One typically drops in this case if this function is called while the CERT resource has not been locked.
 *      eTDAL_SEC_CERT_ERROR_TIMEOUT: The timeout has expired and the CERT block is still processing a command (least significant bit of COMMAND register still to 1).
 *      eTDAL_SEC_CERT_ERROR: Any other errors occurred at driver level or reported by the CERT block during the processing of a command
 *                            (least significant bit of STATUS_AKL equal to 1).
 *
 *===========================================================================*/
tTDAL_SEC_CertStatus TDAL_SEC_CERT_Exchange(
        tTDAL_SEC_CertResourceHandle* pxResourceHandle,
        size_t xNumOfCommands,
        tTDAL_SEC_CertCommand* pxCommands,
        size_t* pxNumOfProcessedCommands)
{
    tTDAL_SEC_CertStatus Ret = eTDAL_SEC_CERT_NO_ERROR;
    unsigned int i;

    if (pxResourceHandle != (&CERTResource))
    {
        return eTDAL_SEC_CERT_ERROR_BAD_HANDLE;
    }
    if (pxResourceHandle->bLock != true)
    {
        return eTDAL_SEC_CERT_ERROR_BAD_HANDLE;
    }

    if (xNumOfCommands == 0 || pxCommands == NULL || pxNumOfProcessedCommands == NULL)
    {
        return eTDAL_SEC_CERT_ERROR;
    }

    for (i = 0; i < xNumOfCommands && eTDAL_SEC_CERT_NO_ERROR == Ret; i++)
    {
        Ret = chalCertExchange((tTDAL_SEC_CertCommand *)pxCommands + i);
    }

    if (eTDAL_SEC_CERT_ERROR_TIMEOUT == Ret)
    {
        *pxNumOfProcessedCommands = i - 1;
    }
    else
    {
        *pxNumOfProcessedCommands = i;
    }

    if (Ret == eTDAL_SEC_CERT_NO_ERROR)
    {
        return eTDAL_SEC_CERT_NO_ERROR;
    }
    else if (Ret == eTDAL_SEC_CERT_ERROR_TIMEOUT)
    {
        return eTDAL_SEC_CERT_ERROR_TIMEOUT;
    }
    else
    {
        return eTDAL_SEC_CERT_ERROR;
    }
}
