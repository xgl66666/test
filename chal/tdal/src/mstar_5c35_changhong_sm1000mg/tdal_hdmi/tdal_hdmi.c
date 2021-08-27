/*******************************************************
*      Copyright 2006 IWEDIA TECHNOLOGIES        *
********************************************************
*
* MODULE NAME: TDAL_HDMI
*
* FILE NAME: $ URL $
*        $ Date $
*        $ Rev $
*
* DESCRIPTION  Thin Driver Adaptation Layer (TDAL) of
*           High Definition Multimedia Interface (HDMI).
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
#include "tdal_hdmi.h"


/********************************************************/
/*   Defines   */
/********************************************************/

/********************************************************/
/*   Macros   */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_HDMI);

/********************************************************/
/*   Typedefs   */
/********************************************************/

/***********************************************************************
* Function Name : TDAL_HDMI_Init
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_Init(void)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_Term
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_Term(void)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_CapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_CapabilityGet(tTDAL_HDMI_Capability   *pstCapability)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_Start
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_Start(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_CallbackFct   NotifyFct, bool   AVIDisplayFormatForced)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_Stop
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_Stop(tTDAL_HDMI_SourceId   Id)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_StatusGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_StatusGet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_SinkStatus   *pstStatus)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_EDIDDataGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_EDIDDataGet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_EDIDData   *pstEDIDData)
{
   return (eTDAL_HDMI_NO_ERROR);
}

tTDAL_HDMI_Error   TDAL_HDMI_EDIDDataFree(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_EDIDData   *pstEDIDData)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_EDIDRawBufferGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_EDIDRawBufferGet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_EDIDRawBuffer   *pstEDIDBuffer)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_InfoFrameSPDSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_InfoFrameSPDSet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_InfoFrameSPD   *pstSPD)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_InfoFrameAVISet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_InfoFrameAVISet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_InfoFrameAVI   *pstAVI)
{
    return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_InfoFrameVSSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_InfoFrameVSSet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_InfoFrameVS   *pstVS)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_HDCPEnable
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_HDCPEnable(tTDAL_HDMI_SourceId   Id)
{
   return (eTDAL_HDMI_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_HDMI_HDCPDisable
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_HDCPDisable(tTDAL_HDMI_SourceId   Id)
{
   return (eTDAL_HDMI_NO_ERROR);
}

#ifdef USE_HDCP
/***********************************************************************
* Function Name : TDAL_HDMI_HDCPParamsSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_HDMI_Error   TDAL_HDMI_HDCPParamSet(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_HDCPParams   *pParams)
{
   return (eTDAL_HDMI_NO_ERROR);
}
#endif

bool   TDAL_HDMI_CurrentSinkCapability(uint32_t   *FormatSupportedMask, uint32_t   *StandardSupportedMask)
{
    return true;
}

bool   TDAL_HDMI_StopForResolutionChanged(void)
{
return true;
}

bool   TDAL_HDMIm_RestartAfterResolutionChanged(void)
{
    return true;
}

bool   TDAL_HDMI_isFormatSupported(tTDAL_OUTPUT_VideoHDMIFormat   format)
{
   return   TRUE;
}
bool   TDAL_HDMI_isStandardSupported(tTDAL_OUTPUT_VideoHDStandard   standard)
{
/*   printf("TDAL_HDMI_isStandardSupportedTODO   %s   %d\r\n", __FILE__, __LINE__);*/
   return   TRUE;
}

bool   TDAL_HDMI_isAudioDigitalModeSupported(tTDAL_OUTPUT_AudioDigitalMode   audioDigitalMode)
{
/*   printf("TDAL_HDMI_isAudioDigitalModeSupported   TODO   %s   %d\r\n", __FILE__, __LINE__);*/
   return   TRUE;
}

bool   TDAL_HDMI_Enable(void)
{
   return   TRUE;
}

bool   TDAL_HDMI_Disable(void)
{
   return   TRUE;

}

