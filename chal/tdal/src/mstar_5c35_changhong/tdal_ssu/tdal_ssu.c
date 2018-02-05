/******************************************************************************
 *                   COPYRIGHT 2004 IWEDIA TECHNOLOGIES                       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL SSU
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/********************************************************
 *        Includes                                      *
 ********************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_ssu.h"

/********************************************************
 *        Defines                                       *
 ********************************************************/

/********************************************************
 *        Macros                                        *
 ********************************************************/

/********************************************************
 *        Global   Variables (GLOBAL/IMPORT)            *
 ********************************************************/

/********************************************************
 *        Local   Module   Variables   (MODULE)         *
 ********************************************************/

/********************************************************
 *        Local   File   Variables   (LOCAL)            *
 ********************************************************/

mTBOX_SET_MODULE(eTDAL_SSU);

/********************************************************
 *        Module   functions                            *
 ********************************************************/

/********************************************************
 *          Local functions                             *
 ********************************************************/

GLOBAL void TDAL_SSU_Init(void)
{
    mTBOX_FCT_ENTER("TDAL_SSU_Init");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_Init NOT IMPLEMENTED\n"));
    mTBOX_RETURN;
}

GLOBAL void TDAL_SSU_Term(void)
{
    mTBOX_FCT_ENTER("TDAL_SSU_Term");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_Term NOT IMPLEMENTED\n"));
    mTBOX_RETURN;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_GetPlatformInfo(tTDAL_SSU_PlatformInfo *pstSsuInfo)
{
    mTBOX_FCT_ENTER("TDAL_SSU_GetPlatformInfo");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetPlatformInfo NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_GetStatusInfo(tTDAL_SSU_StatusInfo *pstSsuStatus)
{
    mTBOX_FCT_ENTER("TDAL_SSU_GetStatus");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_SetParameters(tTDAL_SSU_ControlParameters *pstSsuParams)
{
    mTBOX_FCT_ENTER("TDAL_SSU_SetParameters");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}

GLOBAL tTDAL_SSU_Error TDAL_SSU_Initiate()
{
    mTBOX_FCT_ENTER("TDAL_SSU_Initiate");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_SSU_GetStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_SSU_NO_ERROR;
}
