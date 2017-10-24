/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_CRYP
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_crypt.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * Description     : Cryptography TDAL implementation
 * Target      : All
 * Author      : Xavier RAZAVET.
 * Date   of creation   : 16 / 07 /   2004.
 *
 *****************************************************************************/

#ifndef _TDAL_CRYPT_H_
#define _TDAL_CRYPT_H_

/*--------------------------*/
/*   Header needed.    */
/*--------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------*/
/*      Public types definition    */
/*------------------------------------------*/

typedef enum
{
   eTDAL_CRYPT_NO_ERROR = 0,
   eTDAL_CRYPT_BAD_PARAMETER,
   eTDAL_CRYPT_ERROR
} tTDAL_CRYPT_Error;

typedef enum
{
   eTDAL_CRYPT_TDES,
   eTDAL_CRYPT_AES
}tTDAL_CRYPT_Algos;


typedef struct
{
   tTDAL_CRYPT_Algos   algo;
   uint8_t     *pKey;
   uint16_t     Keylength;
   uint8_t      *pDest;
   uint8_t     *pSource;
   uint32_t     sizeToProcess;
} tTDAL_CRYPT_Str;
   


/*------------------------------------------*/
/*    Public Functions Declarations   */
/*------------------------------------------*/


tTDAL_CRYPT_Error TDAL_CRYPT_Init (void);
tTDAL_CRYPT_Error TDAL_CRYPT_Term(void);


tTDAL_CRYPT_Error TDAL_CRYPT_Encrypt(tTDAL_CRYPT_Str *params);
tTDAL_CRYPT_Error TDAL_CRYPT_Decrypt(tTDAL_CRYPT_Str *params);

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_CRYPT_H_*/
