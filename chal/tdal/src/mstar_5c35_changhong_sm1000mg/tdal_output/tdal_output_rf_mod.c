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
*           video/audio RF modulator outputs.
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


/*
** macro definition to select translation table 
** calls shall be enchain with a simple "else" and without any ";"
*/   
#define mCONVERSION_TABLE(table,pointer,size,element,search) \
	if (strcmp(element,search)==0){\
      pointer= table; \
      size=(int)(sizeof(table)/sizeof(table[0]));}

/********************************************************/
/*   Typedefs   */
/********************************************************/
/*
/******************************************************************************
 * Function Name    : TDAL_OUTPUTi_RFMOD_Init
 * Description      :
 * Side effects     :
 * Comment          :
 * Inputs           :
 * Outputs          :
 *****************************************************************************/
void TDAL_OUTPUTi_RFMOD_Init(void)
{
}
/******************************************************************************
 * Function Name    : TDAL_OUTPUTi_RFMOD_Term
 * Description      :
 * Side effects     :
 * Comment          :
 * Inputs           :
 * Outputs          :
 *****************************************************************************/
void TDAL_OUTPUTi_RFMOD_Term(void)
{
}


/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModCapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_RFModCapabilityGet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_RFModCapability   *pstCapability)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModBandSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_RFModBandSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_RFModBand   Band)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModChannelSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_RFModChannelSet(tTDAL_OUTPUT_OutputId   Id,   uint8_t   Channel)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModStandardSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_RFModStandardSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_VideoSDStandard   Std)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}
/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModEnable
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModEnable(void)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModDisable
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModDisable(void)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}
/***********************************************************************
* Function Name : TDAL_OUTPUT_RFModTestGenControl
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_RFModTestGenControl(tTDAL_OUTPUT_OutputId   Id,   bool   On)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);

}
