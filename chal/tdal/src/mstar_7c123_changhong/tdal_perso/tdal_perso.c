/*****************************************************************************
 *                         COPYRIGHT 2011 SMARDTV                            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_PERSO
 *
 * FILE NAME:   TDAL_PERSO->C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_perso/tdal_perso.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 ****************************************************************************/

/**   @file   tdal_perso.c
 *
 *   Source file for the Personalization TDAL.
 *
 */

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#include <tbox.h>

#include <tdal_common.h>
#include <tdal_perso.h>

#include <flash_cfg.h>


/*******************************************************/
/*              Defines                                */
/*******************************************************/
#if defined(KAON_MSD5C59_BOOTLOADER)
#define FLASH_PAIRING_DATA_ADDRESS  (0x00000160)    // FLASH offset address of PK
#define FLASH_CSC_DATA_ADDRESS      (0x00000400)    // FLASH offset address of CSC
#else
#define FLASH_PAIRING_DATA_ADDRESS  (0x000D0000)    // FLASH offset address of PK
#define FLASH_CSC_DATA_ADDRESS      (0x000C0000)    // FLASH offset address of CSC
#endif

#define FLASH_STB_SN_ADDRES         (0x5D0000)  // FLASH offset address of STB SN

mTBOX_SET_MODULE(eTDAL_PERSO);


/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/


/*******************************************************/
/*              Locals                                 */
/*******************************************************/
uint32_t PairingDataSize;
uint32_t CSCDataSize;

/*******************************************************/
/*              Forwards                               */
/*******************************************************/


/** Initializing the TDAL_PERSO module.
 *
 * Retrieves the location of the personalization area in the flash, and access the first
 * bytes to check if a magic marker is there.
 *
 * @return eTDAL_PERSO_NO_ERROR if successful.
 * @return eTDAL_PERSO_ERROR if internals could not be allocated, or if personalization area length is null.
 *
 * @note Calling TDAL_PERSO_Init a second time without calling TDAL_PERSO_Term in between is
 * allowed, and will read the flash mapping and magic marker again.
 *
 * @warning Not thread-safe.
 * 
 */
eTDAL_PERSO_ErrorCode TDAL_PERSO_Init(void)
{
    PairingDataSize = 600;
    CSCDataSize = 3000;

    return eTDAL_PERSO_NO_ERROR;
}


/** Terminating the TDAL_PERSO module.
 *
 * Clears and frees the module internals.
 *
 * @return eTDAL_PERSO_NO_ERROR in all cases.
 *
 * @warning Not thread-safe.
 *
 */
eTDAL_PERSO_ErrorCode TDAL_PERSO_Term(void)
{
    PairingDataSize = 0;
    CSCDataSize = 0;

    return eTDAL_PERSO_NO_ERROR;
}


/** Searching for the tag_length of the data associated to a tag.
 *
 * @param[in] tag is the identifier of the tag.
 *
 * @param[in] tag_length as input is a pointer to the length of the buffer provided to store the data associated
 * with the tag. If that length is smaller than the tag content length, the returned content will be
 * truncated. If that length is larger, only the tag content will be stored. If that length is zero, no content
 * will be copied and only the length read from the personalization area will be returned. The pointer itself
 * can be null, but no length will be returned in that case, only the return code will be valid.
 *
 * @param[out] tag_length as output is a pointer to the length the data associated with the tag as read from the
 * personalization area, if not provided null.
 *
 *
 * @return eTDAL_PERSO_NO_ERROR if tag identifies a readable header tag in the personalization area.
 * @return eTDAL_PERSO_BAD_PARAMETER if tag does not identify a tag in the personalization area.
 * @return eTDAL_PERSO_NOT_INITIALIZED if the module was not initialized beforehand.
 *
 */
eTDAL_PERSO_ErrorCode TDAL_PERSO_GetTagLength(tTDAL_PERSO_Tag const tag, uint16_t* tag_length)
{
    mTBOX_FCT_ENTER(TDAL_PERSO_GetTagLength);

    if (tag_length == NULL)
    {
        return eTDAL_PERSO_BAD_PARAMETER;
    }

    switch(tag)
    {
    case PERSO_TAG_PK_ADDRESS:
    case PERSO_TAG_PK_SIZE:
    case PERSO_TAG_CSCD_ADDRESS:
    case PERSO_TAG_CSCD_SIZE:
        *tag_length = 4;
        break;
    case PERSO_TAG_SDTV_SN:
        *tag_length = 256;
        break;
    default:
        return eTDAL_PERSO_BAD_PARAMETER;
    }
    return eTDAL_PERSO_NO_ERROR;
}


/** Searching for the tag_length of the data associated to a tag.
 *
 * @param[in] tag is the identifier of the tag.
 *
 * @param[in] tag_length as input is a pointer to the length of the buffer provided to store the data associated
 * with the tag. If that length is smaller than the tag content length, the returned content will be
 * truncated. If that length is larger, only the tag content will be stored. If that length is zero, no content
 * will be copied and only the length read from the personalization area will be returned. The pointer itself
 * can be null, but no length will be returned in that case, only the return code will be valid.
 *
 * @param[out] tag_length as output is a pointer to the length the data associated with the tag as read from the
 * personalization area, if not provided null.
 *
 *
 * @return eTDAL_PERSO_NO_ERROR if tag identifies a readable header tag in the personalization area.
 * @return eTDAL_PERSO_BAD_PARAMETER if tag does not identify a tag in the personalization area.
 * @return eTDAL_PERSO_NOT_INITIALIZED if the module was not initialized beforehand.
 *
 */
eTDAL_PERSO_ErrorCode TDAL_PERSO_ReadTag(tTDAL_PERSO_Tag const tag, uint16_t* tag_length, uint8_t* const tag_content)
{
    uint32_t CSCDataAddress = FLASH_CSC_DATA_ADDRESS;
    uint32_t PairingDataAddress = FLASH_PAIRING_DATA_ADDRESS;
    uint32_t TmpData;
    uint8_t *TmpInt8Ptr = tag_content;
    uint8_t *SerialAdress = FLASH_STB_SN_ADDRES;
    uint32_t SerialSize = 256;

    mTBOX_FCT_ENTER(TDAL_PERSO_ReadTag);

    switch(tag)
    {
    case PERSO_TAG_PK_ADDRESS:
        TmpData = PairingDataAddress;
        break;
    case PERSO_TAG_PK_SIZE:
        TmpData = PairingDataSize;
        break;
    case PERSO_TAG_CSCD_ADDRESS:
        TmpData = CSCDataAddress;
        break;
    case PERSO_TAG_CSCD_SIZE:
        TmpData = CSCDataSize;
        break;
    case PERSO_TAG_SDTV_SN:
        memcpy(tag_content,SerialAdress,*tag_length);
        break;
    default:
        return eTDAL_PERSO_BAD_PARAMETER;
    }

    if(tag != PERSO_TAG_SDTV_SN)
    {
        *TmpInt8Ptr++ = (uint8_t)(TmpData>>24);
        *TmpInt8Ptr++ = (uint8_t)((TmpData>>16)&0xFF);
        *TmpInt8Ptr++ = (uint8_t)((TmpData>>8)&0xFF);
        *TmpInt8Ptr = (uint8_t)((TmpData)&0xFF);
    }

    mTBOX_RETURN(eTDAL_PERSO_NO_ERROR);
}

#if 0
/** Convert a tag into the targeted CA tag
 *
 * @param[in] tag is the identifier of the tag.
 *
 * @return tTDAL_PERSO_Tag the targeted CA tag.
 * @return 0 if tag does not identify a tag in the personalization area.
 *
 */
static tTDAL_PERSO_Tag TDAL_PERSO_ConverTag(unsigned long tag)
{
	return 0;
}
#endif

