/******************************************************************************
 *            COPYRIGHT 2001 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME:   TDAL_NVM
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_nvm.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC 
 *
 * DESCRIPTION:   
 *
 *****************************************************************************/

#ifndef _TDAL_NVM_H_
#define _TDAL_NVM_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************
 *  Typedefs                   *
 *******************************************************/
typedef enum
{
   eTDAL_NVM_NO_ERROR,
   eTDAL_NVM_NOT_DONE
}tTDAL_NVM_ErrorCode;

/*******************************************************
 *  Functions Declarations             *
 *******************************************************/
tTDAL_NVM_ErrorCode   TDAL_NVM_Init( void );
tTDAL_NVM_ErrorCode   TDAL_NVM_Term( void );
tTDAL_NVM_ErrorCode   TDAL_NVM_Open( void );
tTDAL_NVM_ErrorCode   TDAL_NVM_Close( void );
uint32_t       TDAL_NVM_Read(  uint32_t  Address,
                  uint32_t  NumberToRead,
                  uint8_t * Buffer );
uint32_t       TDAL_NVM_Write( uint32_t  Address,
                  uint32_t  NumberToWrite,
                  uint8_t * Buffer);


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_NVM_H_*/
