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
   *   Source   file   for   the   Personalization   TDAL.
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

mTBOX_SET_MODULE(eTDAL_PERSO);


/*   ******************************************************/
/*            Macros                           */
/*   ******************************************************/

/*   ******************************************************/
/*            Typedefs                        */
/*   ******************************************************/


/*******************************************************/
/*            Locals                           */
/*******************************************************/
/*******************************************************/
/*            Forwards                        */
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

eTDAL_PERSO_ErrorCode   TDAL_PERSO_Init(   void   )
{
   return (   eTDAL_PERSO_NO_ERROR   );
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
eTDAL_PERSO_ErrorCode   TDAL_PERSO_Term(   void   )
{
   return   eTDAL_PERSO_NO_ERROR;
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
eTDAL_PERSO_ErrorCode   TDAL_PERSO_GetTagLength(   tTDAL_PERSO_Tag   const   tag, uint16_t*   tag_length)
{
    mTBOX_FCT_ENTER(TDAL_PERSO_ReadTag);

    switch(tag)
    {
    case PERSO_TAG_PK_ADDRESS:
    case PERSO_TAG_PK_SIZE:
	case PERSO_TAG_PK_LOCATION:
    case PERSO_TAG_CSCD_ADDRESS:
    case PERSO_TAG_CSCD_SIZE:
	case PERSO_TAG_CSCD_LOCATION:
        *tag_length = 4;
        break;
    case PERSO_TAG_SDTV_SN:
        *tag_length = 256;        

    default:
        *tag_length = 0;
    }
return (   eTDAL_PERSO_NO_ERROR   );
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
uint8_t g_pkData[494]=
{
0x00, 0x00, 0x01, 0x6C, 0x00, 0x01, 0x00, 0x00, 0x03, 0x03, 0xFA, 0x13, 0x35, 0xE8, 0x17, 0xFE, 
0xE0, 0xCE, 0x47, 0x61, 0x0B, 0xDA, 0x6C, 0xC7, 0xF0, 0xFB, 0xDD, 0x07, 0x77, 0x95, 0x91, 0xA8, 
0xF5, 0xBA, 0x1A, 0x3F, 0xAC, 0x22, 0x8A, 0x29, 0x13, 0x1C, 0x19, 0x7E, 0xD4, 0x07, 0xD7, 0xBA, 
0x91, 0x0F, 0xB8, 0x28, 0x27, 0x1D, 0x1F, 0x8E, 0x0F, 0xB0, 0x70, 0xC3, 0x4C, 0x5C, 0x6F, 0x41, 
0xA4, 0x12, 0xC3, 0xE6, 0xF8, 0x38, 0x3E, 0x4A, 0x92, 0xFA, 0x17, 0x71, 0x73, 0x17, 0x01, 0x09, 
0x2E, 0xD5, 0x69, 0x79, 0x31, 0x24, 0x6A, 0xA1, 0xF6, 0xA5, 0xED, 0x6A, 0x54, 0xBC, 0xB3, 0xCA, 
0x25, 0xB9, 0xCA, 0x7A, 0x8B, 0xD7, 0x08, 0x4D, 0x40, 0x3D, 0x9C, 0xA6, 0xA8, 0x86, 0x07, 0x72, 
0x00, 0x51, 0x1E, 0x23, 0x20, 0xA3, 0xC9, 0x7B, 0x37, 0x3E, 0x56, 0x9E, 0xA0, 0x12, 0x45, 0x8B, 
0x37, 0x3D, 0x61, 0x69, 0x1E, 0xC8, 0x49, 0x54, 0x42, 0x67, 0x68, 0x1C, 0x1F, 0x67, 0xCE, 0x01, 
0x57, 0x91, 0x17, 0xF9, 0x8C, 0xEA, 0x93, 0xD6, 0x5D, 0x89, 0x23, 0x02, 0x02, 0xA0, 0x4D, 0x42, 
0x1C, 0x0E, 0xD9, 0x71, 0x84, 0x56, 0x4C, 0xC5, 0x02, 0xD7, 0x54, 0x00, 0x46, 0x02, 0x66, 0x9E, 
0x81, 0x77, 0x12, 0x90, 0xC3, 0x85, 0x31, 0x99, 0x98, 0xAD, 0x64, 0x7D, 0xE3, 0x52, 0xC9, 0xC7, 
0x55, 0x09, 0x94, 0xE7, 0xC2, 0xDE, 0x4D, 0x72, 0xBE, 0x71, 0x08, 0x1E, 0x9A, 0x5B, 0x5B, 0x26, 
0x9E, 0x98, 0xC7, 0x48, 0x60, 0x2E, 0x35, 0x07, 0x36, 0x27, 0xF7, 0x54, 0x23, 0xB9, 0x4C, 0x27, 
0xA5, 0x49, 0x31, 0x1E, 0x34, 0x9B, 0xC6, 0x55, 0xA6, 0xAE, 0xFA, 0xEF, 0x79, 0x44, 0x10, 0xAB, 
0xA6, 0xD2, 0x80, 0x8F, 0x81, 0x51, 0x00, 0x3B, 0x3B, 0x56, 0xB0, 0x29, 0x98, 0x1F, 0xCD, 0xB3, 
0xA7, 0xE0, 0xD3, 0xB7, 0xA5, 0x70, 0x97, 0x4D, 0x7B, 0x87, 0x01, 0x91, 0x6B, 0x5D, 0x8D, 0x2C, 
0x7D, 0x3F, 0x9F, 0xD8, 0xEC, 0xA3, 0x7B, 0xCD, 0x6F, 0xAE, 0x47, 0xBC, 0xF7, 0x0B, 0xDE, 0x11, 
0x40, 0x14, 0xA1, 0xC8, 0x4A, 0x16, 0xAA, 0x6A, 0xBD, 0x66, 0x66, 0xEA, 0x62, 0xBF, 0xA2, 0xBC, 
0xFF, 0x18, 0x1D, 0xC4, 0x19, 0xC9, 0xF3, 0x5E, 0x28, 0x79, 0xFB, 0x92, 0xA6, 0xC4, 0x17, 0xD7, 
0xDA, 0x10, 0xC6, 0xFD, 0xEA, 0x43, 0x74, 0xBA, 0x04, 0xB6, 0xAD, 0xB9, 0x17, 0x74, 0x82, 0xF5, 
0xF8, 0xE7, 0xEC, 0x87, 0xCE, 0x92, 0x53, 0xF3, 0x91, 0xA7, 0x53, 0x22, 0x89, 0x96, 0xCF, 0x2A, 
0xE5, 0x0E, 0x50, 0x42, 0x62, 0x53, 0x3D, 0xE8, 0xD2, 0xA8, 0x64, 0x9D, 0x2C, 0x5F, 0xB0, 0x53, 
0x42, 0x36, 0x91, 0xB0, 0x6D, 0xB1, 0x60, 0xF6, 0x55, 0x0A, 0xEB, 0x56, 0x86, 0x7A, 0xB9, 0x44, 
0x71, 0x70, 0xFA, 0x24, 0x79, 0x03, 0x49, 0x7D, 0x82, 0x59, 0xFD, 0x3B, 0x74, 0x39, 0xDD, 0x1D, 
0x45, 0x78, 0xDD, 0x34, 0x4A, 0x4C, 0x12, 0x96, 0x9F, 0x89, 0xF0, 0x46, 0x3F, 0x46, 0x68, 0xF9, 
0x97, 0xFD, 0x8C, 0x6B, 0xD8, 0x38, 0x32, 0xC3, 0x98, 0xF9, 0x07, 0x6B, 0x0A, 0xF7, 0x6B, 0x0B, 
0x87, 0xDA, 0x54, 0xC8, 0x71, 0x7B, 0x93, 0x2A, 0x03, 0x1E, 0x1D, 0x83, 0x26, 0x57, 0x10, 0xB7, 
0x2F, 0x78, 0x6E, 0xED, 0xBB, 0x72, 0xAB, 0xF5, 0x5D, 0x8E, 0x3E, 0xA1, 0x61, 0xD1, 0x07, 0x13, 
0x92, 0xFE, 0x74, 0xB4, 0xF5, 0xED, 0x6E, 0xD5, 0x20, 0xE9, 0x1A, 0xF7, 0x3E, 0x68, 0x6F, 0x26, 
0xDC, 0xC2, 0xEB, 0xD6, 0x7F, 0xEB, 0xD0, 0xC3, 0x60, 0x81, 0x2E, 0x63, 0x38, 0xEB
};;
uint8_t g_cscData[256] = {0};
eTDAL_PERSO_ErrorCode   TDAL_PERSO_ReadTag(   tTDAL_PERSO_Tag   const   tag, uint16_t*   tag_length,  uint8_t* const tag_content)
{

    uint32_t CSCDataAddress = 0x7d0000;
    uint32_t CSCDataSize	= 0x100;

	uint32_t CSCDataLocation = PERSO_LOCATION_FLASH;
	
#ifdef NASC
    uint32_t PairingDataAddress = 0x83FFF000;
#else

    uint32_t PairingDataAddress = 0x510000;

#endif //NASC

	//TDAL_FLA_Read(0x510000,g_pkData,494);
#if 0 /**for 0904 Version*/
	PairingDataAddress = g_pkData;
#endif
	TDAL_FLA_Read(CSCDataAddress,g_cscData,CSCDataSize);

	uint32_t PairingDataLocation = PERSO_LOCATION_MEMORY;

    #ifdef CAK_MERLIN
    #ifdef NASC
        uint32_t PairingDataSize = 494;
    #else
        uint32_t PairingDataSize = 494;
    #endif
    #else 
    uint32_t PairingDataSize =     494;
    #endif
    uint32_t TmpData;
    uint8_t *TmpInt8Ptr = tag_content;

    uint8_t *SerialAdress = 0x007D002C;
    uint32_t SerialSize = 256;  
    uint8_t * TmpData_PK_Test = (uint8_t * )0x83FFF000 ;

    mTBOX_FCT_ENTER(TDAL_PERSO_ReadTag);
    switch(tag)
    {
    case PERSO_TAG_PK_ADDRESS:
#if 1
		//TmpData = g_pkData;
		memcpy(TmpData_PK_Test,g_pkData,494);
		TmpData = TmpData_PK_Test;

printf("@@@@@@@@@@@@ TmpData_PK_Test: [%x][%x][%x][%x]\n\n",TmpData_PK_Test[0],TmpData_PK_Test[1],TmpData_PK_Test[2],TmpData_PK_Test[3]);

#else
        TmpData = PairingDataAddress;
#endif
        break;
    case PERSO_TAG_PK_SIZE:
        TmpData = PairingDataSize;
        break;
    case PERSO_TAG_CSCD_ADDRESS:
        TmpData = CSCDataAddress;
		printf("@@@@@@@@@@@@ g_cscData: [%x][%x][%x][%x]\n\n",g_cscData[0],g_cscData[1],g_cscData[2],g_cscData[3]);
		
        break;
    case PERSO_TAG_CSCD_SIZE:
        TmpData = CSCDataSize;
        break;
    case PERSO_TAG_CSCD_LOCATION:
        TmpData = CSCDataLocation;
        break;
    case PERSO_TAG_PK_LOCATION:
        TmpData = PairingDataLocation;
        break;
    case PERSO_TAG_SDTV_SN:
        memcpy(tag_content,SerialAdress,*tag_length  );
        
        break;

    default:
        TmpData = 0;
    }
    printf("@@@@@@@@@@@@ TmpData1: %x\n\n",TmpData);
    if(tag != PERSO_TAG_SDTV_SN)
    {
        *TmpInt8Ptr++ =  (uint8_t)(TmpData>>24);
        *TmpInt8Ptr++ =  (uint8_t)((TmpData>>16)&0xFF);
        *TmpInt8Ptr++ =  (uint8_t)((TmpData>>8)&0xFF);
        *TmpInt8Ptr =    (uint8_t)((TmpData)&0xFF);
    }

    mTBOX_RETURN(   eTDAL_PERSO_NO_ERROR   );
}

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

