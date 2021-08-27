/*****************************************************************************
 *                      COPYRIGHT 2009 IWEDIA TECHNOLOGIES              *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_connection_manager
 *
 * FILE NAME: URL:   $
 *          Rev:   $
 *          Date:  $
 *
 * Description : Common TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
   *   INCLUDES                                           *
   ****************************************************************************/
#include <string.h>              /*   memcpy()   function   */

#include "crules.h"
#include "tbox.h"

#include "tdal_common.h"
#include "tdal_connection_manager.h"



/****************************************************************************
   *   DEFINES                                              *
   ****************************************************************************/


/****************************************************************************
   *   MACROS                                              *
   ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_COMMON);

/****************************************************************************
   *   TYPEDEFS                                           *
   ****************************************************************************/




typedef   enum
{
   eTDAL_USE_CASE_SWITCH_IN_PROGRESS,
   eTDAL_USE_CASE_SWITCH_COMPLETED
}tTDAL_UseCaseState_t;

/****************************************************************************
   *   GLOBAL   VARIABLES   (GLOBAL/IMPORT)                           *
   ****************************************************************************/

/****************************************************************************
   *   LOCAL   MODULE   VARIABLES   (MODULE/IMPORT)                         *
   ****************************************************************************/

/****************************************************************************
   *   LOCAL   FILE   VARIABLES   (LOCAL)                              *
   ****************************************************************************/

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                                *
 ****************************************************************************/

/******************************************************************************
 * Function Name   : pTDAL_Common_ConnectionManagerInit
 * Description      :
 * Side effects    :
 * Comment         :
 * Inputs          :
 * Outputs         :
 *****************************************************************************/

/******************************************************************************
 * Function Name   : TDAL_CommonSelectUseCase
 * Description      :
 * Side effects    :
 * Comment         :
 * Inputs          :
 * Outputs         :
 *****************************************************************************/
bool TDAL_ConnectionManager_GetCurrentUseCase(tTDAL_UseCaseInfo *pstUseCaseInfo)
{
   bool bStatus = TRUE;
   
   return( bStatus );
}


/******************************************************************************
 * Function Name   : TDAL_CommonSelectUseCase
 * Description      :
 * Side effects    :
 * Comment         :
 * Inputs          :
 * Outputs         :
 *****************************************************************************/
bool
TDAL_ConnectionManager_SelectUseCase(tTDAL_UseCaseInfo *pstUseCaseInfo)
{
   bool bStatus = TRUE;
   
   return( bStatus );
}

