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
*           audio SPDIF outputs.
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
#include "tdal_output_priv.h"

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
* Function Name : TDAL_OUTPUT_AudioSPDIFModeSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioSPDIFModeSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_AudioDigitalMode   Mode)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool   desc_found = FALSE;
    uint32_t   i;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_AudioSPDIFModeSet");
    
    /*   check   if   not   initialized   */
    if   (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }
    
    /*   find   the   output   descriptor   */
    for   (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT && desc_found==FALSE; i++)
    {
        if   (TDAL_OUTPUTi_DescriptorTable[i].Id == Id)
        {
            desc_found = TRUE;
            break;
        }
    }

    if   (desc_found == TRUE)
    {
        switch   (Id)
        {
            case   eTDAL_OUTPUT_AUDIO_SPDIF_ID_0   :
            {
                if   (Mode != eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED)
                {
                    /*
                    ErrorCode = STAUD_OPSetDigitalMode(TDAL_AVm_GetAudioHandle(),
                                 STAUD_OBJECT_OUTPUT_SPDIF0,
                                 STAUD_DIGITAL_MODE_COMPRESSED);
                    */
                }
                if   (Mode == eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED)
                {
                    /*
                    ErrorCode = STAUD_OPSetDigitalMode(TDAL_AVm_GetAudioHandle(),
                                 STAUD_OBJECT_OUTPUT_SPDIF0,
                                 STAUD_DIGITAL_MODE_NONCOMPRESSED);
                    */
               }
                TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc->DigitalMode = Mode;
                break;
            }
            default:
            {
                OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                break;
            }
        }        
    }
    else
    {
        OutErrorCode = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
    }
   
    mTBOX_RETURN(OutErrorCode);
}

tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioSPDIFDelaySet(tTDAL_OUTPUT_OutputId   Id,   uint32_t   AudioDelay)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool   desc_found = FALSE;
    uint32_t   i;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_AudioSPDIFDelaySet");
    
    /*   check   if   not   initialized   */
    if   (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

    /*   find   the   output   descriptor   */
    for   (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT && desc_found==FALSE; i++)
    {
        if   (TDAL_OUTPUTi_DescriptorTable[i].Id == Id)
        {
            desc_found = TRUE;
            break;
        }
    }

    if   (desc_found == TRUE)
    {
        switch   (Id)
        {
            case   eTDAL_OUTPUT_AUDIO_SPDIF_ID_0   :
            {
                /* TBD */
//                if   ()
//                {
//                    TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc->AudioDelay = AudioDelay;
//                }   
//                else
//                {
//                    mTBOX_TRACE((kTBOX_NIV_WARNING,"AudioSPDIFDelaySet:   err\r\n"));
//                }   
                break;
            }
            default:
            {
                OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                break;
            }
        }
    }
    else
    {
        OutErrorCode = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
    }
    
    mTBOX_RETURN(OutErrorCode);
}

tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioSPDIFAdjustSet(tTDAL_OUTPUT_OutputId   Id,   uint8_t   AudioAdjust)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool   desc_found = FALSE;
    uint32_t   i;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_AudioSPDIFAdjustSet");

    /*   check   if   not   initialized   */
    if   (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

    /*   find   the   output   descriptor   */
    for   (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT && desc_found==FALSE; i++)
    {
        if   (TDAL_OUTPUTi_DescriptorTable[i].Id == Id)
        {
            desc_found = TRUE;
            break;
        }
    }

    if   (desc_found == TRUE)
    {
        switch   (Id)
        {
            case   eTDAL_OUTPUT_AUDIO_SPDIF_ID_0   :
            {
                TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc->AudioAdjust = AudioAdjust;
                break;
            }
            default:
            {
                OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                break;
            }
        }
    }
    else
    {
        OutErrorCode = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
    }
    mTBOX_RETURN(OutErrorCode);
}
