/********************************************************************************
 *                       COPYRIGHT 2004 IWEDIA TECHNOLOGIES                           *
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
#include "MsTypes.h"
#include "drvNGA.h"
//#include "nocs1x_csd_impl.h"
//#include "nocs1x_csd.h"
#include "MsOS.h"

/********************************************************
 *              Defines                                 *
 ********************************************************/
/********************************************************
 *              Local File Variables (LOCAL)            *
 ********************************************************/
mTBOX_SET_MODULE(eTDAL_SEC);

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
/* RAM2RAM Parameter initialization */
const TCsdR2RCipheredProtectingKeys R2RCipheredProtectingKeys = {{0xe4, 0x21, 0x13, 0xa7, 0x99, 0xca, 0xf4, 0xc7, 0xb8, 0x73, 0x07, 0x00, 0x7b, 0xde, 0xb2, 0xbb}, 
                                                           {0xe4, 0x21, 0x13, 0xa7, 0x99, 0xca, 0xf4, 0xc7, 0xb8, 0x73, 0x07, 0x00, 0x7b, 0xde, 0xb2, 0xbb}}; 

const TCsdUnsignedInt8 CipheredContentKey[] = {0xe4, 0x21, 0x13, 0xa7, 0x99, 0xca, 0xf4, 0xc7, 0xb8, 0x73, 0x07, 0x00, 0x7b, 0xde, 0xb2, 0xbb};

LOCAL TDAL_mutex_id TdalSecMutexID = NULL;

static int gSecTrngFd;
#define O_RDONLY     (1<<0)   /* Open for reading only */

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
tTDAL_SEC_ErrorCode TDAL_SEC_GetUniqueID ( tTDAL_SEC_Nuid* pUid)
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
tTDAL_SEC_ErrorCode TDAL_SEC_GetChipRevision (char *pxChipRevision, size_t * pxChipRevisionLength)
{
    if(NULL!=pxChipRevision && NULL!=pxChipRevisionLength)
    {
        if(csdGetChipRevision(pxChipRevision)==CSD_NO_ERROR)
        {
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
 *      get pxChipExtension
 *
 * Returns:
 *      eTDAL_SEC_NO_ERROR       no error
 *      eTDAL_SEC_NOT_DONE       command not done
 *
 *===========================================================================*/

tTDAL_SEC_ErrorCode TDAL_SEC_GetChipExtension (char *pxChipExtension, size_t * pxChipExtensionLength)

{   
    
    if(NULL!=pxChipExtension && NULL!=pxChipExtensionLength)
    {
       TDAL_LockMutex(TdalSecMutexID);
        if(csdGetChipExtension(pxChipExtension)==CSD_NO_ERROR)
        {
		    TDAL_UnlockMutex(TdalSecMutexID);
            return eTDAL_SEC_NO_ERROR;
        }
		TDAL_UnlockMutex(TdalSecMutexID);
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
    uint8_t * startAddr = (q / 16) * 16;
    uint8_t * endAddr = ((q + len) / 16 + 1) * 16;
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
tTDAL_SEC_ErrorCode TDAL_SEC_Encryptdata(uint8_t *pBufferSrc,uint8_t *pBufferDst, size_t length)
{

    TCsdStatus csdStatus = CSD_ERROR;
    int i;

    TCsdUnsignedInt8 * srcNonCached;
    TCsdUnsignedInt8 * dstNonCached;

    if(pBufferSrc == NULL || pBufferDst == NULL)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if(length % 8 != 0)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    TDAL_LockMutex(TdalSecMutexID);

    srcNonCached = MS_PA2KSEG1(MS_VA2PA(pBufferSrc));
    dstNonCached = MS_PA2KSEG1(MS_VA2PA(pBufferDst));

    TDAL_SECi_InvalidateCacheAddr(pBufferDst, length);
    TDAL_SECi_InvalidateCacheAddr(pBufferSrc, length);

    MDrv_AESDMA_Lock();
    csdStatus = csdEncryptDataWithSecretContentKey( CSD_R2R_ALGORITHM_TDES_K1K2K1, CSD_R2R_CRYPTO_OPERATION_MODE_ECB, 
                                        R2RCipheredProtectingKeys, &CipheredContentKey, 16, NULL, 0, 
                                        FALSE, NULL, 
                                        srcNonCached, dstNonCached, (TCsdSize)length);
    MDrv_AESDMA_Unlock();
    
    TDAL_UnlockMutex(TdalSecMutexID);

    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Encryptdata csdEncryptData not done,ERROR %s\n",DBG_TCsdStatus(csdStatus)));
        return eTDAL_SEC_NOT_DONE;
    }
    mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Encryptdata csdEncryptData %s\n",DBG_TCsdStatus(csdStatus)));

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
tTDAL_SEC_ErrorCode TDAL_SEC_Decryptdata( uint8_t *pBufferSrc,uint8_t *pBufferDst, size_t length)
{
    TCsdStatus csdStatus = CSD_ERROR;
    int i;
    TCsdUnsignedInt8 * srcNonCached;
    TCsdUnsignedInt8 * dstNonCached;

    if(pBufferSrc == NULL || pBufferDst == NULL)
    {
        return eTDAL_SEC_NOT_DONE;
    }
    if(length % 8 != 0)
    {
        return eTDAL_SEC_NOT_DONE;
    }

    TDAL_LockMutex(TdalSecMutexID);

    srcNonCached = MS_PA2KSEG1(MS_VA2PA(pBufferSrc));
    dstNonCached = MS_PA2KSEG1(MS_VA2PA(pBufferDst));

    TDAL_SECi_InvalidateCacheAddr(pBufferDst, length);
    TDAL_SECi_InvalidateCacheAddr(pBufferSrc, length);

    MDrv_AESDMA_Lock();
    csdStatus = csdDecryptDataWithSecretContentKey( CSD_R2R_ALGORITHM_TDES_K1K2K1, CSD_R2R_CRYPTO_OPERATION_MODE_ECB, 
                                        R2RCipheredProtectingKeys, &CipheredContentKey, 16, NULL, 0, 
                                        FALSE, NULL, 
                                        srcNonCached, dstNonCached, (TCsdSize)length);
    MDrv_AESDMA_Unlock();
    
    TDAL_UnlockMutex(TdalSecMutexID);

    if(csdStatus != CSD_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Decryptdata csdDecryptData, ERROR%s\n",DBG_TCsdStatus(csdStatus)));
        return eTDAL_SEC_NOT_DONE;
    }

    mTBOX_TRACE(( kTBOX_NIV_1,"TDAL_SEC_Decryptdata csdDecryptData %s\n",DBG_TCsdStatus(csdStatus)));
    return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode TDAL_SEC_GenerateRandomBytes(uint8_t *pxOutput, size_t xInitVectorSize)
{    
 	int i=0;
	
    srand(rand());
	for(i=0;i<xInitVectorSize;i++)
	{
		pxOutput[i]=rand()%(0xFF+1);
	}
	return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode TDAL_SEC_CloseSession(uint32_t session)
{
	return eTDAL_SEC_NO_ERROR;

}

tTDAL_SEC_ErrorCode TDAL_SEC_OpenSession(uint32_t *session)
{
	return eTDAL_SEC_NO_ERROR;
}


tTDAL_SEC_ErrorCode TDAL_SEC_EncryptWithIV(uint32_t sessionId, uint8_t *pxInput, uint8_t *pxOutput,size_t xMessageSize, uint8_t *pxInitVector,size_t xInitVectorSize)
{
	return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode TDAL_SEC_DecryptWithIV(uint32_t sessionId, uint8_t *pxInput, uint8_t *pxOutput,size_t xMessageSize, uint8_t *pxInitVector,size_t xInitVectorSize)
{
	return eTDAL_SEC_NO_ERROR;
}

tTDAL_SEC_ErrorCode TDAL_SEC_SetEncryptClearTextKey(uint8_t *pxClearTextKey, size_t length)
{
	return eTDAL_SEC_NO_ERROR;
}
tTDAL_SEC_ErrorCode TDAL_SEC_SetDecryptClearTextKey(uint8_t *pxClearTextKey, size_t length)
{
	return eTDAL_SEC_NO_ERROR;
}



