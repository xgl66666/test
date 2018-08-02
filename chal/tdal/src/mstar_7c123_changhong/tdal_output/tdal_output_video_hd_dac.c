/*******************************************************
*      Copyright 2006 IWEDIA TECHNOLOGIES        *
********************************************************
*
* MODULE NAME: TDAL_OUTPUT
*
* FILE NAME: $ URL $
*        $ Date $
*        $ Rev $
*
* DESCRIPTION  Thin Driver Adaptation Layer (TDAL) of
*           video HD DAC outputs.
*
********************************************************/
/********************************************************/
/* Includes */
/********************************************************/
#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_output.h"
/********************************************************/
/*   Defines   */
/********************************************************/

/********************************************************/
/*   Macros   */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_OUTPUT);

/********************************************************/
/*   Typedefs   */
/********************************************************/

/********************************************************/
/*   Global   Variables   (GLOBAL/IMPORT)   */
/********************************************************/
/********************************************************/
/*   Local   Module   Variables   (MODULE)   */
/********************************************************/

/********************************************************/
/*   Local   File   Variables   (LOCAL)   */
/********************************************************/

/********************************************************/
/*   Local   Functions   Declarations   (LOCAL)   */
/********************************************************/

/********************************************************/
/*   Functions   Definitions   (LOCAL/GLOBAL)   */
/********************************************************/
/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoHDDACCapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoHDDACCapabilityGet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_VideoHDDACCapability   *pstCapability)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoHDDACParamsSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoHDDACParamsSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_VideoHDDACParams   *pstParams)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}

