/******************************************************************************
 *                     COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_VBI
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_vbi/tdal_vbi.c $
 *            $Rev: 414 $
 *            $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL VBI implementation
 *
 *****************************************************************************/

/********************************************************/
/*             Includes                                 */
/********************************************************/
#include <string.h>


#include "crules.h"
#include "tbox.h"
#include "tdal_disp.h"

#include "tdal_vbi.h"

#include "tdal_common.h"


/********************************************************/
/*            Defines                           */
/********************************************************/

/********************************************************/
/*            Macros                           */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_VBI);

/********************************************************/
/*            Typedefs                        */
/********************************************************/

/********************************************************/
/*            Global   Variables   (GLOBAL/IMPORT)      */
/********************************************************/

GLOBAL tTDAL_VBI_ErrorCode TDAL_OUTPUTm_VideoDencSetWssAspectRatio(tTDAL_VBI_WssAspectRatio * wssAspectRation);

/********************************************************/
/*            Local   Module   Variables   (MODULE)         */
/********************************************************/

/********************************************************/
/*            Local   File   Variables   (LOCAL)         */
/********************************************************/

LOCAL tTDAL_VBI_WssAspectRatio         TDAL_VBIi_WssAspectRatio = -1;
LOCAL bool TDAL_VBIi_WssAspectRatioSet = false;

/********************************************************/
/*            Local   Functions   Declarations   (LOCAL)   */
/********************************************************/

/********************************************************/
/*            Functions   Definitions   (GLOBAL)         */
/********************************************************/

/******************************************************************************
 * Function Name    : TDAL_VBI_Init
 * Description      : Initialize of the VBI module
 * Side effects     :
 * Comment          :
 * Inputs           :
 * Outputs          :
 *****************************************************************************/
tTDAL_VBI_ErrorCode   TDAL_VBI_Init()
{
   return(eTDAL_VBI_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_VBI_Terminate
 * Description        : De-Initialize of the vbi module
 * Side effects       :
 * Comment            :
 * Inputs             :
 * Outputs            :
 *****************************************************************************/
tTDAL_VBI_ErrorCode   TDAL_VBI_Terminate(void)
{
return(eTDAL_VBI_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_VBI_Open
 * Description        :
 * Side effects       :
 * Comment            :
 * Inputs             :
 * Outputs            :
 *****************************************************************************/
tTDAL_VBI_ErrorCode   TDAL_VBI_Open(tTDAL_VBI_Type   VBI_Type)
{
    if (VBI_Type == eTDAL_VBI_WSS)
    {
        TDAL_VBIi_WssAspectRatioSet = false;
        TDAL_OUTPUTm_VideoDencSetWssAspectRatio(NULL);
    }
    return(eTDAL_VBI_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_VBI_Close
 * Description        :
 * Side effects       :
 * Comment            :
 * Inputs             :
 * Outputs            :
 *****************************************************************************/
tTDAL_VBI_ErrorCode   TDAL_VBI_Close(tTDAL_VBI_Type   VBI_Type)
{
    if (VBI_Type == eTDAL_VBI_WSS)
    {
        TDAL_VBIi_WssAspectRatioSet = false;
        TDAL_OUTPUTm_VideoDencSetWssAspectRatio(NULL);
    }
    return(eTDAL_VBI_NO_ERROR);
}

/******************************************************************************
   *   Function   Name      :   TDAL_VBI_Start
   *   Description      :   Start   Teletext   Insertion
   *   Side   effects      :
   *   Comment         :
   *   Inputs            :
   *   Outputs         :
   *****************************************************************************/
tTDAL_VBI_ErrorCode   TDAL_VBI_Start(void)
{
return(eTDAL_VBI_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_VBI_Stop
 * Description        : Stop Teletext Insertion
 * Side effects       :
 * Comment            :
 * Inputs             :
 * Outputs            :
 *****************************************************************************/
tTDAL_VBI_ErrorCode   TDAL_VBI_Stop(void)
{
return(eTDAL_VBI_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_VBI_SetWSSAspectRatio
 * Description        : Insert the WSS aspect ratio
 * Side effects       :
 * Comment            :
 * Inputs             :
 * Outputs            :
 *****************************************************************************/
tTDAL_VBI_ErrorCode TDAL_VBI_SetWSSAspectRatio(tTDAL_VBI_WssAspectRatio   WssAspectRatio)
{
    TDAL_VBIi_WssAspectRatio = WssAspectRatio;
    TDAL_VBIi_WssAspectRatioSet = true;
    return TDAL_OUTPUTm_VideoDencSetWssAspectRatio(TDAL_VBIi_WssAspectRatioSet ? &TDAL_VBIi_WssAspectRatio : NULL);
}

#if   defined(TTXT_OSD_ON)   &&   defined(TTXT_VBI_ON)
/******************************************************************************
 * Function Name      : TDAL_VBI_PESArrivalCallBack
 * Description        : Call by DEMUX to transmit the PES to STTX
 * Side effects       :
 * Comment            :
 * Inputs             :
 * Outputs            :
 *****************************************************************************/
void   TDAL_VBI_PESArrivalCallBack(signed   char         ChannelId,   /*   Unused*/
                           signed   char         FilterId,      /*   Unused*/
                           uint32_t            Filter_track,   /*   Unused*/
                           uint32_t            Length,      /*   length   of   the   buffer   */
                           uint8_t            *Buffer)      /*   Data   */
{
}
#endif

