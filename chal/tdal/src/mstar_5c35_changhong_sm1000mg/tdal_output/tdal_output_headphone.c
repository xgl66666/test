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
*           audio analogue outputs.
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
tTDAL_OUTPUT_HeadphoneHotplugNotifyCbk   TDAL_OUTPUTm_HeadphoneHotplugNotifyCbk = NULL;

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
* Function Name : TDAL_OUTPUT_AudioAnaCapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_HeadphoneHotplugDetectionSupported
(
   tTDAL_OUTPUT_OutputId   Id,
   bool            *pbSupported
)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);
}

GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_HeadphoneHotplugDetectionRegister
(
   tTDAL_OUTPUT_OutputId            Id,
   tTDAL_OUTPUT_HeadphoneHotplugNotifyCbk   pCbk
)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);
}

tTDAL_OUTPUT_Error   
TDAL_OUTPUT_HeadphoneHotplugDetectionStateGet(tTDAL_OUTPUT_OutputId   Id,   bool   *pbState)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);
}


