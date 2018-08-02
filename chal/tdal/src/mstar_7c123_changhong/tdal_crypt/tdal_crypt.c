/******************************************************************************
 *                         COPYRIGHT 2007 IWEDIA                              *
 ******************************************************************************
 *
 * MODULE NAME:     TDAL_CRYPT
 *
 * FILE NAME:       $Workfile:   tdal_crypt.c  $
 *                  $Revision: 414 $
 *                  $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

/********************************************************/
/*              Includes                                */
/********************************************************/

#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tdal_common.h"
#include "tdal_crypt.h"


/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Typedefs                                */
/********************************************************/



/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/

/********************************************************/
/*              Local Module Variables (MODULE)         */
/********************************************************/

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===================================================================
 *
 * TDAL_CRYPT_Init
 *
 * Parameters:
 *  none.
 *
 * Description:
 *
 *
 * Returns: tTDAL_CRYPT_Error
 *    eTDAL_CRYPT_ERROR if an error occured,
 *    eTDAL_CRYPT_NO_ERROR otherwise.
 *
 *==================================================================*/
tTDAL_CRYPT_Error TDAL_CRYPT_Init (void)
{
   tTDAL_CRYPT_Error error = eTDAL_CRYPT_NO_ERROR;

   return (error);
}

/******************************************************************************
 * Function Name : TDAL_CRYPT_Term
 *
 * Description   :
 *
 * Side effects  :
 *
 * Returns: tTDAL_CRYPT_Error
 *    eTDAL_CRYPT_ERROR if an error occured,
 *    eTDAL_CRYPT_NO_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_CRYPT_Error TDAL_CRYPT_Term(void)
{
   tTDAL_CRYPT_Error         returnError = eTDAL_CRYPT_NO_ERROR;

   return(returnError);
}


/******************************************************************************
 * Function Name : TDAL_CRYPT_Decrypt
 *
 * Description   :
 *
 * Side effects  :
 *
 * Returns: tTDAL_CRYPT_Error
 *    eTDAL_CRYPT_ERROR if an error occured,
 *    eTDAL_CRYPT_NO_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/   
tTDAL_CRYPT_Error TDAL_CRYPT_Encrypt(tTDAL_CRYPT_Str *params)
{
   tTDAL_CRYPT_Error         returnError = eTDAL_CRYPT_NO_ERROR;

   return (returnError);
}

/******************************************************************************
 * Function Name : TDAL_CRYPT_Decrypt
 *
 * Description   :
 *
 * Side effects  :
 *
 * Returns: tTDAL_CRYPT_Error
 *    eTDAL_CRYPT_ERROR if an error occured,
 *    eTDAL_CRYPT_NO_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/   
tTDAL_CRYPT_Error TDAL_CRYPT_Decrypt(tTDAL_CRYPT_Str *params)
{
   tTDAL_CRYPT_Error         returnError = eTDAL_CRYPT_NO_ERROR;

   return (returnError);
}
