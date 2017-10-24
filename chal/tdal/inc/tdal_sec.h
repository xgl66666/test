/******************************************************************************
 *            COPYRIGHT 2009 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME:   TDAL_SEC
 *
 * FILE NAME: $URL:$
 *      $Rev:$
 *      $Date:$
 *
 * PUBLIC
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

#ifndef _TDAL_SEC_H_
#define _TDAL_SEC_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************
 *  Typedefs                   *
 *******************************************************/
typedef enum
{
   eTDAL_SEC_NO_ERROR,
   eTDAL_SEC_NOT_DONE
}tTDAL_SEC_ErrorCode;

typedef enum
{
    eTDAL_SEC_CERT_NO_ERROR,
    /**< Requested operation was completed successfully
    */
    eTDAL_SEC_CERT_ERROR_BAD_HANDLE,
    /**< Handle provided is invalid or unknown
    */
    eTDAL_SEC_CERT_ERROR_BAD_EMI,
    /**< @deprecated
    */
    eTDAL_SEC_CERT_ERROR_BAD_USAGE,
    /**< @deprecated
    */
    eTDAL_SEC_CERT_ERROR_TIMEOUT,
    /**< Execution of a CERT command lasted too long
    */
    eTDAL_SEC_CERT_ERROR,
    /**< Requested operation failed
    */
    eTDAL_SEC_LAST_CERT_STATUS
    /**< Internal use only.
    */
}tTDAL_SEC_CertStatus;

typedef enum
{
    eTDAL_SEC_CERT_TIMEOUT_DEFAULT,
    /**<  Default timeout. Indicates that the associated command does not write
    *    in OTP.
    */
    eTDAL_SEC_CERT_TIMEOUT_OTP,
    /**<  The associated command is writing in OTP memory.
    */
    eTDAL_SEC_LAST_CERT_TIMEOUT
    /**<  Internal use only.
    */
} tTDAL_SEC_CertTimeoutType;

typedef struct
{
    uint8_t inputData[32];
    /**< Input data to be written in the 8 32-bit CERT common interface DATA_IN_X
    *   registers. First 4 MSB inputData[0:3] shall be written in DATA_IN_0
    *   register, next 4 MSB inputData[4:7] in DATA_IN_1 register and so on.
    */
    uint8_t outputData[32];
    /**< Buffer where to write values of the 8 32-bit CERT common interface
    *   DATA_OUT_X registers following the processing of a command. DATA_OUT_0
    *   register value shall be written in the first 4 MSB outputData[0:3],
    *   DATA_OUT_1 register in next 4 MSB outputData[4:7] and so on.
    */
    uint8_t status[4];
    /**< Buffer where to write the value of the CERT common interface STATUS_AKL
    *   register following the processing of a command.
    */
    uint8_t opcodes[4];
    /**< Command operation codes to be written in the CERT common interface
    *   COMMAND register. The least significant bit that acts as the command
    *   start bit is already set to 1.
    */
    tTDAL_SEC_CertTimeoutType timeout;
    /**<  This field characterizes the processing duration of the command. It is
    *    not expressed as a duration. Associated timeout values is to be defined
    *    by the entity in charge of developing the CERT driver. This field is
    *    mainly used to know whether the command is going to write in OTP or
    *    not.
    */
} tTDAL_SEC_CertCommand;

typedef struct  {
    bool         bLock;
}tTDAL_SEC_CertResourceHandle;

typedef uint8_t tTDAL_SEC_Nuid[4];
typedef uint8_t tTDAL_SEC_Nuid64[8];
/*******************************************************
 *  Functions Declarations             *
 *******************************************************/
tTDAL_SEC_ErrorCode TDAL_SEC_Init( void);
tTDAL_SEC_ErrorCode TDAL_SEC_Terminate( void);

tTDAL_SEC_ErrorCode TDAL_SEC_GetUniqueID(  tTDAL_SEC_Nuid* pUid);
tTDAL_SEC_ErrorCode TDAL_SEC_GetUnique64ID ( tTDAL_SEC_Nuid64* pUid);

tTDAL_SEC_ErrorCode TDAL_SEC_GetChipRevision (char *pxChipRevision, size_t * pxChipRevisionLength);
tTDAL_SEC_ErrorCode TDAL_SEC_GetChipExtension (char *pxChipExtension, size_t * pxChipExtensionLength);

tTDAL_SEC_ErrorCode TDAL_SEC_GetSeed(  uint8_t *pBuffer, int *pSize);

tTDAL_SEC_ErrorCode TDAL_SEC_Encryptdata( uint8_t *pBufferSrc, uint8_t *pBufferDst, size_t length);
tTDAL_SEC_ErrorCode TDAL_SEC_Decryptdata( uint8_t *pBufferSrc, uint8_t *pBufferDst, size_t length);

tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptClearTextKey(uint8_t *pxClearTextKey, size_t length);
tTDAL_SEC_ErrorCode TDAL_SEC_SetDecryptClearTextKey(uint8_t *pxClearTextKey, size_t length);
tTDAL_SEC_ErrorCode TDAL_SEC_EncryptWithIV(uint32_t sessionId, uint8_t *pxInput, uint8_t *pxOutput,size_t xMessageSize, uint8_t *pxInitVector,size_t xInitVectorSize);
tTDAL_SEC_ErrorCode TDAL_SEC_DecryptWithIV(uint32_t sessionId, uint8_t *pxInput, uint8_t *pxOutput,size_t xMessageSize, uint8_t *pxInitVector,size_t xInitVectorSize);

tTDAL_SEC_ErrorCode TDAL_SEC_SetDecryptCertKey(uint32_t sessionId, size_t xKeyIdSize, int8_t *pxKeyId);
tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptCertKey(uint32_t sessionId, size_t xKeyIdSize, int8_t *pxKeyId);

tTDAL_SEC_ErrorCode TDAL_SEC_OpenSession(uint32_t *session);
tTDAL_SEC_ErrorCode TDAL_SEC_CloseSession(uint32_t session);
tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptionMethod(uint32_t sessionId, uint16_t xEmi);


tTDAL_SEC_ErrorCode TDAL_SEC_SendManualAckToCert(void);

tTDAL_SEC_ErrorCode TDAL_SEC_GenerateRandomBytes(uint8_t *pxOutput, size_t xInitVectorSize);

tTDAL_SEC_CertStatus TDAL_SEC_CERT_Lock(tTDAL_SEC_CertResourceHandle **ppxResourceHandle);
tTDAL_SEC_CertStatus TDAL_SEC_CERT_Unlock(tTDAL_SEC_CertResourceHandle *pxResourceHandle);
tTDAL_SEC_CertStatus TDAL_SEC_CERT_Exchange(tTDAL_SEC_CertResourceHandle *pxResourceHandle, size_t xNumOfCommands, tTDAL_SEC_CertCommand* pxCommands, size_t* pxNumOfProcessedCommands);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_SEC_H_*/
