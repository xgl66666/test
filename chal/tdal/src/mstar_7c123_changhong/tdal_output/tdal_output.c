/*******************************************************
*       Copyright 2006 IWEDIA TECHNOLOGIES      *
********************************************************
*
* MODULE NAME: TDAL_OUTPUT
*
* FILE NAME: $ URL $
*      $ Date $
*      $ Rev $
*
* DESCRIPTION  Thin Driver Adaptation Layer (TDAL) of
*        video/audio outputs.
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
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_output_module_priv.h"
#include "tdal_disp_module_priv.h"
#include "tdal_av.h"

#include "MsCommon.h"
#include "MsMemory.h"

#include "apiHDMITx.h"
#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiXC.h"
#include "xc/msAPI_XC.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
//Api
#include "drvTVEncoder.h"
#include "apiVDEC.h"
#include "apiDAC.h"

//MApi
#include "xc/msAPI_VE.h"

/*********************** Embedded debug print ***********************/
#include <stdarg.h>

/*********************** Embedded debug print ***********************/

/********************************************************/
/* Defines */
/********************************************************/

/********************************************************/
/* Macros */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_OUTPUT);

/********************************************************/
/* Typedefs */
/********************************************************/

/********************************************************/
/* Global Variables (GLOBAL/IMPORT) */
/********************************************************/
GLOBAL void TDAL_DISPm_XCInit();

/********************************************************/
/* Local Module Variables (MODULE) */
/********************************************************/
tTDAL_OUTPUTi_Descriptor            TDAL_OUTPUTi_DescriptorTable[kTDAL_OUTPUTi_NB_OUTPUT];
tTDAL_OUTPUTi_VideoDencDescriptor   TDAL_OUTPUTi_VideoDENCDescriptor[kTDAL_OUTPUTi_NB_VIDEO_DENC];
tTDAL_OUTPUTi_AudioAnaDescriptor    TDAL_OUTPUTi_AudioAnaDescriptor[kTDAL_OUTPUTi_NB_AUDIO_ANA];
tTDAL_OUTPUTi_AudioSPDIFDescriptor  TDAL_OUTPUTi_AudioSPDIFDescriptor[kTDAL_OUTPUTi_NB_AUDIO_SPDIF];
tTDAL_OUTPUTi_HDMIDescriptor        TDAL_OUTPUTi_HDMIDescriptor[kTDAL_OUTPUTi_NB_HDMI];
tTDAL_OUTPUTi_RFModDescriptor       TDAL_OUTPUTi_RFModDescriptor[kTDAL_OUTPUTi_NB_RFMOD];

bool                                TDAL_OUTPUT_AlreadyInitialized = FALSE;
#ifdef HD_ENABLE
tTDAL_OUTPUT_VideoHDStandard        TDAL_OUTPUT_CurrentVideoHDStandard = eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;
#else
tTDAL_OUTPUT_VideoHDStandard        TDAL_OUTPUT_CurrentVideoHDStandard = eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50;
#endif
bool                                TDAL_OUTPUTi_isRGBActivated = FALSE;
bool                                TDAL_OUTPUTi_isOutputDISABLING_GFX_ID_1 = FALSE;
bool                                TDAL_GlobalResolutionChangeOnGoing = FALSE;

/********************************************************/
/* Local File Variables (LOCAL) */
/********************************************************/
LOCAL tTDAL_OUTPUT_VideoDencCapability          TDAL_VideoDencMainCapability;
LOCAL tTDAL_OUTPUT_AudioAnaCapability           TDAL_AudioAnaMainCapability;

LOCAL tTDAL_OUTPUT_AudioAnaSpeakerParams        TDAL_AudioAnaMainSpeakerParams;
LOCAL tTDAL_OUTPUT_AudioAnaSpeakerAttenuation   TDAL_AudioAnaMainSpeakerAttenuation;

MODULE tTDAL_OUTPUTi_HDMIDescriptor             TDAL_OUTPUT_HDMIDescriptor;

LOCAL TDAL_mutex_id                             TDAL_OUTPUT_Mutex = NULL;
LOCAL tTDAL_OUTPUT_AudioDigitalMode             TDAL_OUTPUT_CurrentAudioMode = 0xFFFFFFFF;
LOCAL uint32_t                                  TDAL_OUTPUT_CurrentAudioDelay = 100;
LOCAL uint8_t                                   TDAL_OUTPUT_CurrentAudioAdjust = 100;

LOCAL bool                                      TDAL_Outputi_isOutputResolutionChangedForGFX_ID_0 = FALSE;
LOCAL bool                                      TDAL_Outputi_isOutputResolutionChangedForGFX_ID_1 = FALSE;
LOCAL bool                                      TDAL_Outputi_isOutputResolutionChangedForGFX_ID_2 = FALSE;
LOCAL bool                                      TDAL_OUTPUTi_RGBAvailable = FALSE;
/********************************************************/
/* Local File Variables (LOCAL) */
/********************************************************/
/********************************************************/
/* Local Functions Declarations (LOCAL) */
/********************************************************/
LOCAL tTDAL_OUTPUT_Error p_TDAL_OUTPUTi_AnaAudio_Init(void);
LOCAL tTDAL_OUTPUT_Error p_TDAL_OUTPUTi_VideoDENC_Init(void);

/********************************************************/
/* Functions Definitions (LOCAL/GLOBAL) */
/********************************************************/
/***********************************************************************
* Function Name : TDAL_OUTPUT_Init
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_Init(void)
{
    int i;
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_Init");

    /* check if already initialized*/
    if (TDAL_OUTPUT_AlreadyInitialized == TRUE)
    {
       /* nothing to do*/
       mTBOX_RETURN(eTDAL_OUTPUT_NO_ERROR);
    }

    OUTPUT_Error = p_TDAL_OUTPUTi_AnaAudio_Init();
    mTBOX_ASSERT(OUTPUT_Error == eTDAL_OUTPUT_NO_ERROR);

    OUTPUT_Error = p_TDAL_OUTPUTi_VideoDENC_Init();
    mTBOX_ASSERT(OUTPUT_Error == eTDAL_OUTPUT_NO_ERROR);

    /*--------------------*/
    /* output descriptors */
    /*--------------------*/
    i = 0;

    TDAL_OUTPUTi_DescriptorTable[i].Id           = eTDAL_OUTPUT_AUDIO_ANA_ID_0;
    TDAL_OUTPUTi_DescriptorTable[i].IsEnable      = FALSE;
    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioAnaDesc   = &TDAL_OUTPUTi_AudioAnaDescriptor[0];
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstHDMIDesc       = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstRFModDesc = NULL;
    i++;

    TDAL_OUTPUTi_DescriptorTable[i].Id           = eTDAL_OUTPUT_AUDIO_SPDIF_ID_0;
    TDAL_OUTPUTi_DescriptorTable[i].IsEnable      = FALSE;
    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioAnaDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc   = &TDAL_OUTPUTi_AudioSPDIFDescriptor[0];
    TDAL_OUTPUTi_DescriptorTable[i].pstHDMIDesc       = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstRFModDesc = NULL;
    i++;

    TDAL_OUTPUTi_DescriptorTable[i].Id           = eTDAL_OUTPUT_VIDEO_DENC_ID_0;
    TDAL_OUTPUTi_DescriptorTable[i].IsEnable      = FALSE;
    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc   = &TDAL_OUTPUTi_VideoDENCDescriptor[0];
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioAnaDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstHDMIDesc       = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstRFModDesc = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->Standard = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->IsMacrovisionActive = false;
    i++;

    TDAL_OUTPUTi_DescriptorTable[i].Id           = eTDAL_OUTPUT_HDMI_ID_0;
    TDAL_OUTPUTi_DescriptorTable[i].IsEnable      = FALSE;
    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioAnaDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstAudioSPDIFDesc   = NULL;
    TDAL_OUTPUTi_DescriptorTable[i].pstHDMIDesc       = &TDAL_OUTPUT_HDMIDescriptor;
    TDAL_OUTPUTi_DescriptorTable[i].pstRFModDesc = NULL;
    i++;

    TDAL_OUTPUT_HDMIDescriptor.videoMode        = eTDAL_OUTPUT_VIDEO_HD_STD_NONE;
    TDAL_OUTPUT_HDMIDescriptor.audioMode        = 0xFFFFFFFF;
    TDAL_OUTPUT_HDMIDescriptor.uiAudioFrequency = 0;
    TDAL_OUTPUT_HDMIDescriptor.uiAudioDelay     = 0;
    TDAL_OUTPUT_HDMIDescriptor.uiAudioAdjust    = 0;


    // XC Init
    TDAL_DISPm_XCInit();

    //VE Init
    TDAL_OUTPUTm_VEInit();

    //GFX Init
    TDAL_GFXm_Init();

    /***********************/
    /* set Format RGB    */
    /***********************/
    TDAL_OUTPUT_AlreadyInitialized = TRUE; /*Set to true to call the functions*/
    OUTPUT_Error  = TDAL_OUTPUT_VideoDencStandardSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0, eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG );
    mTBOX_ASSERT(OUTPUT_Error == eTDAL_OUTPUT_NO_ERROR);

//    OUTPUT_Error  = TDAL_OUTPUT_VideoDencFormatSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0, eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS );
//    mTBOX_ASSERT(OUTPUT_Error == eTDAL_OUTPUT_NO_ERROR);
//    TDAL_OUTPUT_AlreadyInitialized = FALSE; /*Set to False */

    /****************************************/
    /* Sets the video output parameters   */
    /* with default values           */
    /****************************************/
    /* TBD */
    /* create a mutex to protect access to the output */
    TDAL_CreateMutex(&TDAL_OUTPUT_Mutex);

    TDAL_OUTPUT_AlreadyInitialized = TRUE;

    mTBOX_RETURN(OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_Term
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_Term(void)
{
    int i=0;
    tTDAL_OUTPUT_Error  Error = eTDAL_OUTPUT_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_Terminate");

    /* check if not initialized */
    if (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        /* nothing to do*/
        mTBOX_RETURN(eTDAL_OUTPUT_NO_ERROR);
    }

    for (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT; i++)
    {
        if (TDAL_OUTPUTi_DescriptorTable[i].Id == eTDAL_OUTPUT_VIDEO_DENC_ID_0)
        {
            break;
        }
    }

    if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable == TRUE)
    {
        /*TBD*/
        TDAL_OUTPUTi_DescriptorTable[i].IsEnable = FALSE;
    }


    for (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT; i++)
    {
        if(TDAL_OUTPUTi_DescriptorTable[i].Id == eTDAL_OUTPUT_VIDEO_HD_DAC_ID_0)
        {
            if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable == TRUE)
            {
                /* TBD */
            }
            /* TBD */
        }
    }

    TDAL_DeleteMutex(TDAL_OUTPUT_Mutex);
    TDAL_OUTPUT_AlreadyInitialized = FALSE;

    mTBOX_RETURN(Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_CapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_CapabilityGet(tTDAL_OUTPUT_Capability *pstCapability)
{
    mTBOX_FCT_ENTER("TDAL_OUTPUT_CapabilityGet");

    /* check if not initialized */
    if (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

   /* set the capabilities */

    pstCapability->nbAudioAna   = kTDAL_OUTPUTi_NB_AUDIO_ANA;
    pstCapability->nbAudioSPDIF = kTDAL_OUTPUTi_NB_AUDIO_SPDIF;
    pstCapability->nbVideoDenc  = kTDAL_OUTPUTi_NB_VIDEO_DENC;
    pstCapability->nbRFMod      = kTDAL_OUTPUTi_NB_RFMOD;
    pstCapability->nbVideoHDDAC = kTDAL_OUTPUTi_NB_VIDEO_HD_DAC;
    pstCapability->nbHDMI       = kTDAL_OUTPUTi_NB_HDMI;

    mTBOX_RETURN(eTDAL_OUTPUT_NO_ERROR);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_HDMICapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_HDMICapabilityGet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_HDMICapability *pstCapability)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   pstCapability->StandardSupportedMask = 0xFFFFFFFF;
   return (OUTPUT_Error);
}
/********************************************************/
/* Functions Definitions (LOCAL) */
/********************************************************/
/***********************************************************************
* Function Name : TDAL_OUTPUTi_HdmiConvertVideoStandardToMsTimings
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
LOCAL bool TDAL_OUTPUTi_HdmiConvertVideoStandardToMsTimings(tTDAL_OUTPUT_VideoHDStandard videoStandard, E_MSAPI_XC_OUTPUT_TIMING_TYPE *pMSTiming)
{
    bool bResult = TRUE;
    mTBOX_FCT_ENTER("TDAL_OUTPUTi_HdmiConvertVideoStandardToMsTimings");
    switch (videoStandard)
    {
        case eTDAL_OUTPUT_VIDEO_HD_STD_640_480_P_60:
            *pMSTiming = E_MSAPI_XC_RES_640x480P_60Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_5994:
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_60:
            *pMSTiming = E_MSAPI_XC_RES_720x480P_60Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_5994:
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_60:
            *pMSTiming = E_MSAPI_XC_RES_720x480I_60Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_576_P_50:
            *pMSTiming = E_MSAPI_XC_RES_720x576P_50Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50:
            *pMSTiming = E_MSAPI_XC_RES_720x576I_50Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_50:
            *pMSTiming = E_MSAPI_XC_RES_1280x720P_50Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_5994:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_60:
            *pMSTiming = E_MSAPI_XC_RES_1280x720P_60Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2398:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_24:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080P_24Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_25:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080P_25Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2997:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_30:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080P_30Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_50:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080P_50Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_5994:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_60:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080P_60Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080I_50Hz;
            break;
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_5994:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_60:
            *pMSTiming = E_MSAPI_XC_RES_1920x1080I_60Hz;
            break;

        case eTDAL_OUTPUT_VIDEO_HD_STD_NONE:
        case eTDAL_OUTPUT_VIDEO_HD_STD_720_483_P_5994:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2398:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_24:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_25:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2997:
        case eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_30:
        default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Unsupported video mode %d\n", videoStandard));
            bResult = FALSE;
            break;

    }
    mTBOX_RETURN(bResult);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_HDMIParamsSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_HDMIParamsSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_HDMIParams *pstParams)
{
    tTDAL_OUTPUT_Error    outError = eTDAL_OUTPUT_NO_ERROR;
    uint8_t               outCnt;
    bool                  outFound = FALSE;
    MS_BOOL               bMSResult;
    E_MSAPI_XC_RESULT     eXCResult;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_HDMIParamsSet");

    if(TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_HDMIParamsSet: Already initialized\n"));
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

    if(Id != eTDAL_OUTPUT_HDMI_ID_0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_HDMIParamsSet: Invalid Id given\n"));
        mTBOX_RETURN(eTDAL_OUTPUT_BAD_PARAMETER_ERROR);
    }

    if(pstParams == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_HDMIParamsSet: Invalid pstParams given\n"));
        mTBOX_RETURN(eTDAL_OUTPUT_BAD_PARAMETER_ERROR);
    }

    TDAL_LockMutex(TDAL_OUTPUT_Mutex);

    if(outError == eTDAL_OUTPUT_NO_ERROR)
    {
        for(outCnt = 0; ((outCnt < kTDAL_OUTPUTi_NB_OUTPUT) && (outFound == FALSE)); outCnt++)
        {
            if(TDAL_OUTPUTi_DescriptorTable[outCnt].Id == Id)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_HDMIParamsSet: outCnt[%d]\n", outCnt));
                outFound = TRUE;
                break;
            }
        }

        if(outFound == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_HDMIParamsSet: Invalid arg given\n"));
            outError = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
        }
    }

    if(TDAL_OUTPUTi_DescriptorTable[outCnt].IsEnable == TRUE && outError == eTDAL_OUTPUT_NO_ERROR)
    {
        if (TDAL_OUTPUTi_DescriptorTable[outCnt].pstHDMIDesc->videoMode != pstParams->VideoStandard)
        {
            //Disable video layer before resolution change
            bool refreshVideoLayer = FALSE;
            if(TDAL_DISPm_VideoLayerEnabled())
            {
                refreshVideoLayer = TRUE;
            }

            TDAL_OUTPUT_Disable(eTDAL_OUTPUT_VIDEO_DENC_ID_0);
            TDAL_OUTPUT_Disable(eTDAL_OUTPUT_HDMI_ID_0);

            if(refreshVideoLayer)
            {
                TDAL_DISP_LayerDisable(eTDAL_DISP_LAYER_VIDEO_ID_0);
            }
            MApi_DAC_Enable(DISABLE, FALSE);

            E_MSAPI_XC_OUTPUT_TIMING_TYPE eMSTimings;
            if (TDAL_OUTPUTi_HdmiConvertVideoStandardToMsTimings(pstParams->VideoStandard, &eMSTimings) == FALSE)
            {
                outError = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
            }


            if ( outError == eTDAL_OUTPUT_NO_ERROR && (bMSResult = msAPI_XC_EnableMiscOutput(DISABLE)) != TRUE)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"msAPI_XC_EnableMiscOutput disable failed\n"));
                outError = eTDAL_OUTPUT_DRIVER_ERROR;
            }

            if(outError == eTDAL_OUTPUT_NO_ERROR)
            {
                MSAPI_XC_DEVICE_ID hdmi_device = {0, E_MSAPI_XC_DEVICE0};
                if ((eXCResult = msAPI_XC_ChangeOutputResolution_EX(&hdmi_device, eMSTimings)) != E_MSAPI_XC_OK)
                {
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL,"msAPI_XC_EnableMiscOutput failed\n"));
                    outError = eTDAL_OUTPUT_DRIVER_ERROR;
                }
            }

            if (outError == eTDAL_OUTPUT_NO_ERROR && (bMSResult = msAPI_XC_EnableMiscOutput(ENABLE)) != TRUE)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"msAPI_XC_EnableMiscOutput enable failed\n"));
                outError = eTDAL_OUTPUT_DRIVER_ERROR;
            }
            else
            {
                //Set CVBS
                outError  = TDAL_OUTPUT_VideoDencStandardSet(eTDAL_OUTPUT_VIDEO_DENC_ID_0, eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG );
                if(outError == eTDAL_OUTPUT_NO_ERROR)
                {
                    TDAL_OUTPUTi_DescriptorTable[outCnt].pstHDMIDesc->videoMode = pstParams->VideoStandard;
                }
                else
                {
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_VideoDencStandardSet enable failed\n"));
                    outError = eTDAL_OUTPUT_DRIVER_ERROR;
                }
				if( (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50) ||
				  (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_5994) ||
				  (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_60) )
				{
				  // I 1920x1080
				  TDAL_DISPi_GFXLayerScale(eTDAL_DISP_LAYER_GFX_ID_0,eTDAL_DISP_GFXScale_1920_1080);

            }
				else if( (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_50) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_30) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_24) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_25) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2997) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2398) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_5994) )
				{
                    // P 1920x1080
                    TDAL_DISPi_GFXLayerScale(eTDAL_DISP_LAYER_GFX_ID_0,eTDAL_DISP_GFXScale_1920_1080);
				}else if( (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_720_576_P_50) ||
					 (pstParams->VideoStandard == eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50))
				{
                    TDAL_DISPi_GFXLayerScale(eTDAL_DISP_LAYER_GFX_ID_0,eTDAL_DISP_GFXScale_720x576);
				}else
				{
                    TDAL_DISPi_GFXLayerScale(eTDAL_DISP_LAYER_GFX_ID_0,eTDAL_DISP_GFXScale_1280_720);
				}
        
            }
            MApi_DAC_Enable(ENABLE, FALSE);

            if(refreshVideoLayer)
            {
                TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0);
            }
            TDAL_OUTPUT_Enable(eTDAL_OUTPUT_VIDEO_DENC_ID_0);
            TDAL_OUTPUT_Enable(eTDAL_OUTPUT_HDMI_ID_0);

        }
        if(TDAL_OUTPUTi_DescriptorTable[outCnt].pstHDMIDesc->audioMode != pstParams->AudioMode)
        {
            MS_BOOL onOff;
            AUDIO_FS_TYPE hdmi_SmpFreq;
            HDMI_TX_OUTPUT_TYPE outType;
            TDAL_OUTPUTi_AudioAnaEnable(FALSE);
            if(pstParams->AudioMode == eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED)
            {
                /* output PCM on HDMI */
                MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_PCM);

                MApi_AUDIO_HDMI_Tx_GetStatus(&onOff,&hdmi_SmpFreq,&outType);
                if(outType != HDMI_OUT_PCM)
                {
                    outError = eTDAL_OUTPUT_DRIVER_ERROR;
                }
            }
            else
            {
                tTDAL_AV_AudioType AudioType;
                AudioType = TDAL_AV_Audio_StreamType();
                switch (AudioType)
                {
                    case eTDAL_AV_AUDIO_TYPE_AC3:
                        MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DrcMode, LINE_MODE, 0);//Line Mod
                        MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DownmixMode, DOLBY_DOWNMIX_MODE_LTRT, 0);
                        break;
                    case eTDAL_AV_AUDIO_TYPE_EAC3:
                        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode, LINE_MODE, 0);//Line Mod
                        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DownmixMode, DOLBY_DOWNMIX_MODE_LTRT, 0);//LtRt
                        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable,TRUE,0);
                        break;
                    default:
                        break;
                }
                /* output Non-PCM on HDMI */
                MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_NONPCM);

                MApi_AUDIO_HDMI_Tx_GetStatus(&onOff, &hdmi_SmpFreq, &outType);
                if (outType != HDMI_OUT_NONPCM)
                {
                    outError = eTDAL_OUTPUT_DRIVER_ERROR;
                }
            }

            if (outError == eTDAL_OUTPUT_NO_ERROR)
            {
                TDAL_OUTPUTi_DescriptorTable[outCnt].pstHDMIDesc->audioMode = pstParams->AudioMode;
            }
        }
    }

    TDAL_UnlockMutex(TDAL_OUTPUT_Mutex);
    return (outError);

}


/***********************************************************************
* Function Name : TDAL_OUTPUT_Enable
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_Enable(tTDAL_OUTPUT_OutputId Id)
{
    tTDAL_OUTPUT_Error    OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t       i;
    bool        AV_Error;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_Enable");

    /* check if not initialized */
    if (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

    /* lock ressources */
    TDAL_LockMutex(TDAL_OUTPUT_Mutex);

    /* find the output descriptor */
    for (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT && desc_found==FALSE; i++)
    {
        if (TDAL_OUTPUTi_DescriptorTable[i].Id == Id)
        {
            desc_found = TRUE;
            break;
        }
    }

    if (desc_found == TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_1," TDAL_OUTPUT_Enable(%d)\n", Id));
        switch (Id)
        {
            case eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
            {

                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != TRUE)
                {
                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = TRUE;
                    MApi_DAC_Enable(ENABLE, FALSE);
                }
                break;
            }
            case eTDAL_OUTPUT_AUDIO_ANA_ID_0 :
            {
                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != TRUE)
                {
                	OutErrorCode = TDAL_OUTPUTi_AudioAnaEnable(true);

                	if(OutErrorCode != eTDAL_OUTPUT_NO_ERROR)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_AVm_EnableAudio() failed\n"));
                    }
                    else
                    {
                        mTBOX_TRACE((kTBOX_NIV_1,"Audio enabled, descriptor %d\n", i));
                        TDAL_OUTPUTi_DescriptorTable[i].IsEnable = TRUE;
                    }
                }
                else
                {
                	mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_OUTPUT_AUDIO_ANA_ID_0 already enabled, skipping...\n"));
                }
                break;
            }

            case eTDAL_OUTPUT_AUDIO_SPDIF_ID_0 :
            {
                /* get the instance of the audio decoders */
                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != TRUE)
                {
                    if(AV_Error == FALSE)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_AVm_EnableAudio() failed\n" ));
                    }
                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = TRUE;
                }
                break;
            }


            case eTDAL_OUTPUT_HDMI_ID_0 :
            {
                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != TRUE)
                {
                    MApi_HDMITx_DisableTMDSCtrl(FALSE);
                    MApi_HDMITx_SetTMDSOnOff(TRUE);
                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = TRUE;
                }

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

    TDAL_UnlockMutex(TDAL_OUTPUT_Mutex);
    mTBOX_RETURN(OutErrorCode);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_Disable
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_Disable(tTDAL_OUTPUT_OutputId Id)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t       i;
    bool        AV_Error;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_Disable");

    /* check if not initialized */
    if (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

    /* lock ressources */
    TDAL_LockMutex(TDAL_OUTPUT_Mutex);

    /* find the output descriptor */
    for (i=0; i<kTDAL_OUTPUTi_NB_OUTPUT && desc_found==FALSE; i++)
    {
        if (TDAL_OUTPUTi_DescriptorTable[i].Id == Id)
        {
            desc_found = TRUE;
            break;
        }
    }

    if (desc_found == TRUE)
    {
        mTBOX_TRACE((kTBOX_NIV_1,"TDAL_OUTPUT_Disable(%d)\n", Id));

        switch (Id)
        {
            case eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
            {

                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != FALSE)
                {
                    {
                        MApi_DAC_Enable(DISABLE, FALSE);
                    }

                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = FALSE;
                }
                break;
            }
            case eTDAL_OUTPUT_AUDIO_ANA_ID_0 :
            {
                /* get the instance of the audio decoders */
                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != FALSE)
                {
                   	OutErrorCode = TDAL_OUTPUTi_AudioAnaEnable(false);

					if(OutErrorCode != eTDAL_OUTPUT_NO_ERROR)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_AVm_DisableAudio() failed\n" ));
                    }
                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = FALSE;
                }
                else
                {
                	mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_OUTPUT_AUDIO_ANA_ID_0 already disabled, skipping...\n"));
                }
                break;
            }

            case eTDAL_OUTPUT_AUDIO_SPDIF_ID_0 :
            {
                /* get the instance of the audio decoders */
                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != FALSE)
                {
                    if(AV_Error == FALSE)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_AVm_DisableAudio() failed\n" ));
                    }
                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = FALSE;
                }
                break;
            }

            
            case eTDAL_OUTPUT_HDMI_ID_0 :
            {
                if(TDAL_OUTPUTi_DescriptorTable[i].IsEnable != FALSE)
                {
                    MApi_HDMITx_SetTMDSOnOff(FALSE);
                    MApi_HDMITx_DisableTMDSCtrl(TRUE);
                    TDAL_OUTPUTi_DescriptorTable[i].IsEnable = FALSE;
                }
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

    TDAL_UnlockMutex(TDAL_OUTPUT_Mutex);
    mTBOX_RETURN(OutErrorCode);
}

bool TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_0(void)
{
   return TDAL_Outputi_isOutputResolutionChangedForGFX_ID_0;
}

void TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_0(void)
{
   TDAL_Outputi_isOutputResolutionChangedForGFX_ID_0 = FALSE;
}

bool TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_1(void)
{
   return TDAL_Outputi_isOutputResolutionChangedForGFX_ID_1;
}

void TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_1(void)
{
   TDAL_Outputi_isOutputResolutionChangedForGFX_ID_1 = FALSE;
}

bool TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_2(void)
{
   return TDAL_Outputi_isOutputResolutionChangedForGFX_ID_2;
}

void TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_2(void)
{
   TDAL_Outputi_isOutputResolutionChangedForGFX_ID_2 = FALSE;
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_APIRevisionGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL const char *TDAL_OUTPUT_APIRevisionGet(void)
{
   static const char API_Revision[] = "TDAL_OUTPUT_V1.1";

   mTBOX_FCT_ENTER("TDAL_OUTPUT_APIRevisionGet");

   mTBOX_RETURN((const char *)API_Revision );
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_PlatformRevisionGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL const char *TDAL_OUTPUT_PlatformRevisionGet(void)
{
   static const char PLTF_Revision[] = "BD_MST5017_SZ";

   mTBOX_FCT_ENTER("TDAL_AV_PlatformRevisionGet");

   mTBOX_RETURN((const char *)PLTF_Revision);
}

/*****************************************************************************/
/************************ LOCAL FUNCTION *************************************/
 /****************************************************************************/

/***********************************************************************
* Function Name : TDAL_OUTPUTm_HDMIAudioFreqUpdate
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
tTDAL_OUTPUT_Error TDAL_OUTPUTm_HDMIAudioFreqUpdate
(
   tTDAL_OUTPUT_OutputId Id,
   uint32_t        Frequency
)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}
/******************************************************************************
 * Function Name   : TDAL_OUTPUTm_GetVtgHandle
 * Description    :
 * Side effects   :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
bool TDAL_OUTPUTi_EnableAnalogRGB(bool bStatus)
{
    return true;
}


/***********************************************************************
 * Function Name : TDAL_OUTPUT_VideoDigitalCapabilityGet
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment      :
 *
 **********************************************************************/
tTDAL_OUTPUT_Error
TDAL_OUTPUT_VideoDigitalCapabilityGet(tTDAL_OUTPUT_OutputId Id,
                    tTDAL_OUTPUT_VideoDigitalCapability *pstCapability)
{
   mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDigitalCapabilityGet");

   /* TODO */

   mTBOX_RETURN(eTDAL_OUTPUT_NO_ERROR);
}

/***********************************************************************
 * Function Name : TDAL_OUTPUT_VideoDigitalParamsSet
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment      :
 *
 **********************************************************************/
tTDAL_OUTPUT_Error
TDAL_OUTPUT_VideoDigitalParamsSet(tTDAL_OUTPUT_OutputId Id,
                  tTDAL_OUTPUT_VideoDigitalParams *pstParams)
{
   mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDigitalParamsSet");

   /* TODO */

   mTBOX_RETURN(eTDAL_OUTPUT_NO_ERROR);
}

LOCAL
tTDAL_OUTPUT_Error p_TDAL_OUTPUTi_AnaAudio_Init(void)
{
    tTDAL_OUTPUT_Error Error = eTDAL_OUTPUT_NO_ERROR;
    mTBOX_FCT_ENTER("p_TDAL_OUTPUTi_AnaAudio_Init");

    /* get driver capabilities */
    /* initialize audio capabilities */
    TDAL_AudioAnaMainCapability.IsFrontRLPresenceSupported      = FALSE;
    TDAL_AudioAnaMainCapability.IsFrontCenterPresenceSupported   = TRUE;
    TDAL_AudioAnaMainCapability.IsSurroundRLPresenceSupported   = FALSE;
    TDAL_AudioAnaMainCapability.IsLFEPresenceSupported      = FALSE;
    TDAL_AudioAnaMainCapability.IsRearRLPresenceSupported      = FALSE;
    TDAL_AudioAnaMainCapability.IsRearCenterPresenceSupported   = FALSE;
    TDAL_AudioAnaMainCapability.IsFrontRLAttenuationSupported   = FALSE;
    TDAL_AudioAnaMainCapability.IsFrontCenterAttenuationSupported= TRUE;
    TDAL_AudioAnaMainCapability.IsSourroundRLAttenuationSupported= FALSE;
    TDAL_AudioAnaMainCapability.IsLFEAttenuationSupported      = FALSE;
    TDAL_AudioAnaMainCapability.IsRearRLAttenuationSupported   = FALSE;
    TDAL_AudioAnaMainCapability.IsRearCenterAttenuationSupported = FALSE;
    TDAL_AudioAnaMainCapability.IsFrontRLDelaySupported       = FALSE;
    TDAL_AudioAnaMainCapability.IsFrontCenterDelaySupported    = TRUE;
    TDAL_AudioAnaMainCapability.IsSurroundRLCenterDelaySupported = FALSE;
    TDAL_AudioAnaMainCapability.IsLFEDelaySupported        = FALSE;
    TDAL_AudioAnaMainCapability.IsRearRLDelaySupported      = FALSE;
    TDAL_AudioAnaMainCapability.IsRearCenterDelaySupported      = FALSE;
    TDAL_AudioAnaMainCapability.AttenuationMax            = kTDAL_OUTPUT_AUDIO_VOLUME_MAX;
    TDAL_AudioAnaMainCapability.DelayMax              = 0;

    TDAL_AudioAnaMainSpeakerParams.FrontRL    = eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_LARGE;
    TDAL_AudioAnaMainSpeakerParams.FrontCenter  = eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_NONE;
    TDAL_AudioAnaMainSpeakerParams.SurroundRL   = eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_NONE;
    TDAL_AudioAnaMainSpeakerParams.RearRL      = eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_NONE;
    TDAL_AudioAnaMainSpeakerParams.RearCenter   = eTDAL_OUTPUT_AUDIO_SPEAKER_TYPE_NONE;
    TDAL_AudioAnaMainSpeakerParams.IsLFEPresent = FALSE;
    TDAL_AudioAnaMainSpeakerAttenuation.FrontRLAttenuation    = 0;
    TDAL_AudioAnaMainSpeakerAttenuation.FrontCenterAttenuation  = 0;
    TDAL_AudioAnaMainSpeakerAttenuation.SurroundRLAttenuation   = 0;
    TDAL_AudioAnaMainSpeakerAttenuation.LFEAttenuation       = 0;
    TDAL_AudioAnaMainSpeakerAttenuation.RearRLAttenuation      = 0;
    TDAL_AudioAnaMainSpeakerAttenuation.RearCenterAttenuation   = 0;
    /* initialize audio descriptors */
    TDAL_OUTPUTi_AudioAnaDescriptor[0].stCapability       = TDAL_AudioAnaMainCapability;
    TDAL_OUTPUTi_AudioAnaDescriptor[0].stSpeakerParams    = TDAL_AudioAnaMainSpeakerParams;
    TDAL_OUTPUTi_AudioAnaDescriptor[0].stSpeakerAttenuation = TDAL_AudioAnaMainSpeakerAttenuation;

    /* SPDIF */
    TDAL_OUTPUTi_AudioSPDIFDescriptor[0].DigitalMode   = eTDAL_OUTPUT_AUDIO_DIGITAL_MODE_UNCOMPRESSED;
    TDAL_OUTPUTi_AudioSPDIFDescriptor[0].AudioDelay    = 0;

    mTBOX_RETURN(Error);
}

LOCAL
tTDAL_OUTPUT_Error p_TDAL_OUTPUTi_VideoDENC_Init(void)
{
    tTDAL_OUTPUT_Error Error = eTDAL_OUTPUT_NO_ERROR;
    mTBOX_FCT_ENTER("p_TDAL_OUTPUTi_VideoDENC_Init");

    /* initialize video capabilities internal (0) and external denc (1) */
    TDAL_VideoDencMainCapability.FormatSupportedMask   =  eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS; // only one DAC in 5C35
                                                          //eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC |
                                                          //eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB |
                                                          //eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV;

    TDAL_VideoDencMainCapability.StandardSupportedMask =  eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG |
                                                          eTDAL_OUTPUT_VIDEO_SD_STD_PAL_I |
                                                          eTDAL_OUTPUT_VIDEO_SD_STD_PAL_M |
                                                          eTDAL_OUTPUT_VIDEO_SD_STD_PAL_N |
                                                          eTDAL_OUTPUT_VIDEO_SD_STD_SECAM |
                                                          eTDAL_OUTPUT_VIDEO_SD_STD_NTSC_M;
    TDAL_VideoDencMainCapability.IsColorBarSupported   = TRUE;
    TDAL_VideoDencMainCapability.IsBrigthnessSupported = FALSE;
    TDAL_VideoDencMainCapability.IsContrastSupported   = FALSE;
    TDAL_VideoDencMainCapability.IsSaturationSupported = FALSE;

    /* initialize video descriptors */
    TDAL_OUTPUTi_VideoDENCDescriptor[0].stCapability = TDAL_VideoDencMainCapability;
    TDAL_OUTPUTi_VideoDENCDescriptor[0].Standard = eTDAL_OUTPUT_VIDEO_SD_STD_SECAM;
    TDAL_OUTPUTi_VideoDENCDescriptor[0].FormatMask = eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS;

    TDAL_OUTPUTi_VideoDENCDescriptor[0].IsColorBarActive = FALSE;

    mTBOX_RETURN(Error);
}

