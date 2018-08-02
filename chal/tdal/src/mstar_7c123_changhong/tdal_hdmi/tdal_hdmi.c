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

#include "MsCommon.h"
#include "apiHDMITx.h"
//#include "tdal_common_priv.h"
//#include "MsMemory.h"
//#include "MsOS.h"


#include "drvMVOP.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiXC.h"
#include "msAPI_XC.h"

/********************************************************/
/*   Defines   */
/********************************************************/
#define   kTDAL_HDMI_EVT_TASK_NAME         "tdalHDMIEvt"
//#define   kTDAL_HDMI_EVT_QUEUE_NB_MSG      100
#define   TDAL_PRIORITY(x)   (x)
#define   kTDAL_HDMI_EVT_TASK_STACK_SIZE   (2*4096)
#define   kTDAL_HDMI_EVT_TASK_PRIO         TDAL_PRIORITY(3)

#define BASEADDRESS_HDMI_hdcp_KEY_IN_FLASH    //flash address to store hdcp key

#ifndef HDCP_KEY_FILE
#define HDCP_KEY_FILE "/usr/share/default_hdcp.key.bin"
#endif

#define HDCP_EXTENDED_KEY_SIZE 336
#define HDCP_KEY_SIZE 304


#define CHECK_BIT(var, pos) (((var) & (1 << pos)) != 0)

#define kMAX_SUPPORTED_AUDIO_NB 10 /* 10*3 bytes, 3 bytes for one short audio descriptor */

/********************************************************/
/*   Macros   */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_HDMI);

/********************************************************/
/*   Typedefs   */
/********************************************************/

typedef   enum
{
   eTDAL_HDMI_AUDIO_FREQUENCY_CHANGE,
   eTDAL_HDMI_AUDIO_PLAYER_PARAM_CHANGE,
   eTDAL_HDMI_AUDIO_STREAM_INFO_CHANGE,
   eTDAL_HDMI_STATE
}
tTDAL_HDMI_MsgType;

/*   Message   queue   for   HDMI   Event   */
typedef   struct
{
   tTDAL_HDMI_MsgType      HDMI_MsgType;
   uint32_t                HDMI_params;
   bool            Exit;
} tTDAL_HDMI_EvtMsg;

typedef enum
{
    E_HDMITX_VIDEO_AFD_SameAsPictureAR    = 8, // 1000
    E_HDMITX_VIDEO_AFD_4_3_Center         = 9, // 1001
    E_HDMITX_VIDEO_AFD_16_9_Center        = 10, // 1010
    E_HDMITX_VIDEO_AFD_14_9_Center        = 11, // 1011
    E_HDMITx_VIDEO_AFD_Others = 15, // 0000~ 0111, 1100 ~ 1111
} MsHDMITX_VIDEO_AFD_RATIO;

typedef enum
{
    E_HDMITX_VIDEO_SI_NoData    = 0,
    E_HDMITX_VIDEO_SI_Overscanned         = 1,
    E_HDMITX_VIDEO_SI_Underscanned        = 2,
    E_HDMITX_VIDEO_SI_Reserved    = 3,
} MsHDMITX_VIDEO_SCAN_INFO;

/********************************************************/
/*   Local   Module   Variables   (MODULE)   */
/********************************************************/
bool   TDAL_HDMI_StopForResolutionChanged(void);
bool   TDAL_HDMI_DVI_State = FALSE;

/********************************************************/
/*   Local   File   Variables   (LOCAL)   */
/********************************************************/
//LOCAL   Handle   TDAL_HDMI_EVT_Handle = NULL;
LOCAL   bool   bTDAL_HDMI_AlreadyInitialized = FALSE;
LOCAL   bool   bTDAL_HDMI_Started = FALSE;

LOCAL     MS_S32          TDAL_HDMI_EvtTaskID = -1;

LOCAL   bool              TDAL_HDMI_EvtTaskStop;
LOCAL   volatile bool     TDAL_HDMI_EvtTaskFinished;
LOCAL   unsigned char     TDAL_HDMI_EvtTaskStack[kTDAL_HDMI_EVT_TASK_STACK_SIZE];

/*   State   of   HDMI   Sink   */
LOCAL   tTDAL_HDMI_Event   TDAL_HDMI_State = eTDAL_HDMI_EVENT_NO_RECEIVER;
LOCAL   bool        TDAL_HDMI_HDCP_State = FALSE;

/*   Capabilities   of   HDMI   Sink   */

/*   Client   Notification   Function   */
LOCAL   tTDAL_HDMI_CallbackFct   TDAL_HDMIi_NotifyFct = NULL;
LOCAL bool bTDAL_HDCP_Enabled = false;

//#ifdef USE_HDCP
/*   HDCP parameters  */
LOCAL tTDAL_HDMI_HDCPParams* HDCPParams = NULL;
//#endif

//extern   ST_ErrorCode_t   SYS_PIO_BIT_Set(U32   PIO_Index,U32   PIO_Bit,BOOL   OnOff);

LOCAL   uint32_t videoStandardSupported = eTDAL_OUTPUT_VIDEO_HD_STD_NONE;
LOCAL	bool isVideoModesParsed = FALSE;

/********************************************************/
/*   Local   Functions   Declarations   (LOCAL)   */
/********************************************************/


LOCAL   void   TDAL_HDMIi_EventsTask(MS_U32 argc, void  *arg);

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
    MS_BOOL          RetVal = TRUE;

    mTBOX_FCT_ENTER("TDAL_HDMI_Init");
    mTBOX_TRACE((kTBOX_NIV_WARNING, "mTBOX_TRACE::TDAL_HDMI_Init\n"));

    if   (bTDAL_HDMI_AlreadyInitialized)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_HDMI_Init   :   TDAL   Already   Initialized   Error\n"));
        mTBOX_RETURN(eTDAL_HDMI_ALREADY_INIT_ERROR);
    }
#if 0 //removed to make reinit work
    RetVal = MApi_HDMITx_Init();

    if (RetVal == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s:   MApi_HDMITx_Init is failed\n",
                TBOX_FunctionName   ));

        mTBOX_RETURN(eTDAL_HDMI_NOT_INIT_ERROR);
    }
#endif
    /*   Init   Successful   */
    bTDAL_HDMI_AlreadyInitialized = TRUE;
    bTDAL_HDMI_Started = FALSE;

    TDAL_HDMI_EvtTaskStop = FALSE;

   mTBOX_RETURN(eTDAL_HDMI_NO_ERROR);
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
    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;
    MS_BOOL            RetValue;

    mTBOX_FCT_ENTER("TDAL_HDMI_Term");

    // stop task if it was started
    if (bTDAL_HDMI_Started == TRUE)
    {
        TDAL_HDMI_Stop(eTDAL_HDMI_SOURCE_ID_0);
    }
#if 0 //removed to make reinit work
    RetValue = MApi_HDMITx_Exit();
    if (RetValue == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s: MApi_HDMITx_Exit is failed\n", TBOX_FunctionName));
        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
        mTBOX_RETURN(ReturnCode);
    }
#endif


    bTDAL_HDMI_AlreadyInitialized = FALSE;

    mTBOX_RETURN (ReturnCode);
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
    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_HDMI_CapabilityGet");

#ifdef   TDAL_HDMI_USE
    if   (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    else if (pstCapability == NULL)
    {
        ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
    }
    else
    {
        pstCapability->NbSource = 1;
        pstCapability->EDIDParsingCapable = TRUE;
    }
#else
    {
        pstCapability->NbSource = 0;
        pstCapability->EDIDParsingCapable = FALSE;
    }
#endif

    return(ReturnCode);
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
    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;
    TDAL_HDMI_EvtTaskStop = FALSE;

    mTBOX_FCT_ENTER("TDAL_HDMI_Start");

#ifdef   TDAL_HDMI_USE
    if   (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s:   TDAL_HDML   has   not   been   initialized\n",
            TBOX_FunctionName   ));
        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    else
    {
        if (bTDAL_HDMI_Started == FALSE)
        {
            if   (NotifyFct == NULL)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s:   parameter   'NotifyFct'   is   NULL\n",
                    TBOX_FunctionName   ));
                ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
            }
            else
            {
                if   (   Id != eTDAL_HDMI_SOURCE_ID_0)
                {

                    ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
                }
                else
                {
                    TDAL_HDMIi_NotifyFct = NotifyFct;
                    TDAL_HDMI_EvtTaskID = MsOS_CreateTask(
                                      TDAL_HDMIi_EventsTask,
                                      NULL,
                                      E_TASK_PRI_MEDIUM, //E_TASK_PRI_HIGH,
                                      TRUE, /* Task auto start */
                                      TDAL_HDMI_EvtTaskStack,
                                      kTDAL_HDMI_EVT_TASK_STACK_SIZE,
                                      kTDAL_HDMI_EVT_TASK_NAME);

                    if (TDAL_HDMI_EvtTaskID < 0)
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "task_init failed for HDMI Event\n"));
                        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
                    }
                    else
                    {
                        bTDAL_HDMI_Started = TRUE;
                    }
                }
            }
        }
    }
#endif

    return (ReturnCode);
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
    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_HDMI_Stop");

#ifdef   TDAL_HDMI_USE

    if (bTDAL_HDMI_AlreadyInitialized == FALSE || bTDAL_HDMI_Started == FALSE)
    {
        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    else
    {
        if (Id != eTDAL_HDMI_SOURCE_ID_0)
        {
            ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
        }
        else
        {
            TDAL_HDMIi_NotifyFct = NULL;
            TDAL_HDMI_EvtTaskStop = TRUE;
            TDAL_HDMI_EvtTaskFinished = FALSE;

            while (TDAL_HDMI_EvtTaskFinished != TRUE)
            {
                MsOS_DelayTask(100);
                mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_HDMI_Term]  Waiting evt thread to finish\n"));
            }
            if   (TDAL_HDMI_EvtTaskID >=0 )
            {
                MsOS_DeleteTask(TDAL_HDMI_EvtTaskID);
                TDAL_HDMI_EvtTaskID = NULL;
            }
            bTDAL_HDMI_Started = FALSE;
        }
    }
#endif
    return (ReturnCode);
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
    tTDAL_HDMI_Error      ReturnCode = eTDAL_HDMI_NO_ERROR;
    MS_BOOL connected_status;

    mTBOX_FCT_ENTER("TDAL_HDMI_StatusGet");
#ifdef   TDAL_HDMI_USE
    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
       ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    else if (Id == eTDAL_HDMI_SOURCE_ID_0)
    {
        if (pstStatus == NULL)
        {
            ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
        }
        else
        {
            connected_status = MApi_HDMITx_GetRxStatus();
            if (connected_status)
                pstStatus->EventState = eTDAL_HDMI_EVENT_RECEIVER_CONNECTED;
            else
                pstStatus->EventState = eTDAL_HDMI_EVENT_NO_RECEIVER;
            //! TODO: need to hanlde more states

            ReturnCode = eTDAL_HDMI_NO_ERROR;
        }
    }
    else
    {
       ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
    }
    return(ReturnCode);
#else
   if   (pstStatus == NULL)
   {
      return(eTDAL_HDMI_BAD_PARAMETER_ERROR);
   }
   else
   {
      pstStatus->EventState = eTDAL_HDMI_EVENT_NO_RECEIVER;
      return(eTDAL_HDMI_NO_ERROR);
   }
#endif
   return (ReturnCode);
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
    tTDAL_HDMI_Error   Error = eTDAL_HDMI_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_HDMI_EDIDDataGet");

    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        Error = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    else
    {
        /*   check   id*/
        if (Id != eTDAL_HDMI_SOURCE_ID_0)
        {
            mTBOX_RETURN(eTDAL_HDMI_BAD_PARAMETER_ERROR);
        }

        if (pstEDIDData == NULL)
        {
            mTBOX_RETURN(eTDAL_HDMI_BAD_PARAMETER_ERROR);
        }

        tTDAL_HDMI_EDIDRawBuffer stEDIDBuffer;
        if ((Error = TDAL_HDMI_EDIDRawBufferGet(Id, &stEDIDBuffer)) != eTDAL_HDMI_NO_ERROR)
        {
            mTBOX_RETURN(Error);
        }

        /*   Init   stucture   */
        memset(pstEDIDData, 0, sizeof(tTDAL_HDMI_EDIDData));

        /*   Fill   basic   part   */
        memcpy(pstEDIDData->stEDIDBasic.ucHeader, stEDIDBuffer.pucBuffer, 8);

        /* Fill manufacturer name */
        pstEDIDData->stEDIDBasic.stVendorProductIdent.cIDManufacturerName[0] = (stEDIDBuffer.pucBuffer[8] >> 2 & 0x1f) + 'A' - 1;
        pstEDIDData->stEDIDBasic.stVendorProductIdent.cIDManufacturerName[1] = (((stEDIDBuffer.pucBuffer[8] & 0x3) << 3) | ((stEDIDBuffer.pucBuffer[9] & 0xe0) >> 5)) + 'A' - 1;
        pstEDIDData->stEDIDBasic.stVendorProductIdent.cIDManufacturerName[2] = (stEDIDBuffer.pucBuffer[9] & 0x1f) + 'A' - 1 ;

        /* Fill Product ID */
        pstEDIDData->stEDIDBasic.stVendorProductIdent.uiIDProductCode = (stEDIDBuffer.pucBuffer[11] << 8) | stEDIDBuffer.pucBuffer[10];

        /* Fill Serial Number */
        memcpy(&pstEDIDData->stEDIDBasic.stVendorProductIdent.uiIDSerialNumber, &stEDIDBuffer.pucBuffer[12], 4);

        /* Week and Year of Manufacture */
        pstEDIDData->stEDIDBasic.stVendorProductIdent.uiWeekOfManufacture   = stEDIDBuffer.pucBuffer[0x10];
        pstEDIDData->stEDIDBasic.stVendorProductIdent.uiYearOfManufacture   = stEDIDBuffer.pucBuffer[0x11] + 1990;

        /* EDID version */
        pstEDIDData->stEDIDBasic.ucVersion  = stEDIDBuffer.pucBuffer[0x12];
        pstEDIDData->stEDIDBasic.ucRevision = stEDIDBuffer.pucBuffer[0x13];

        /* Fill video input params*/
        pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsDigitalSignal = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 7);

        if (pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsDigitalSignal)
        {
            pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsVesaDFPCompatible = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 0);
        }
        else
        {
            pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsSetupExpected = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 4);
            pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsSepSyncSupported = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 3);
            pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsCompSyncSupported = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 2);
            pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsSyncOnGreenSupported = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 1);
            pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsPulseRequired = CHECK_BIT(stEDIDBuffer.pucBuffer[0x14], 0);

            switch ((stEDIDBuffer.pucBuffer[0x14] >> 5) & 0x03) //bits 6,5
            {
                case 0x00:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMax = 700;
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMin = 300;
                    break;
                case 0x01:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMax = 714;
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMin = 286;
                    break;
                case 0x02:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMax = 100;
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMin = 400;
                    break;
                case 0x03:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMax = 700;
                    pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.uiSignalLevelMin = 0;
                    break;
            }
        }

        /* Fill display params */
        pstEDIDData->stEDIDBasic.stDisplayParams.uiMaxHorizImageSize  = stEDIDBuffer.pucBuffer[0x15];
        pstEDIDData->stEDIDBasic.stDisplayParams.uiMaxVertImageSize   = stEDIDBuffer.pucBuffer[0x16];

        mTBOX_TRACE((kTBOX_NIV_1, "Display size: %dx%d\n", pstEDIDData->stEDIDBasic.stDisplayParams.uiMaxHorizImageSize, pstEDIDData->stEDIDBasic.stDisplayParams.uiMaxVertImageSize));

        /* what is a format of this data? Typically it should be float, but not uint32 */
        pstEDIDData->stEDIDBasic.stDisplayParams.uiDisplayGamma = stEDIDBuffer.pucBuffer[0x17];

        pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsStandby = CHECK_BIT(stEDIDBuffer.pucBuffer[0x18], 7);
        pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsSuspend = CHECK_BIT(stEDIDBuffer.pucBuffer[0x18], 6);
        pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsActiveOff = CHECK_BIT(stEDIDBuffer.pucBuffer[0x18], 5);

        pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsRGBDefaultColorSpace =  CHECK_BIT(stEDIDBuffer.pucBuffer[0x18], 2);
        pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsPreferredTimingMode =  CHECK_BIT(stEDIDBuffer.pucBuffer[0x18], 1);
        pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsDefaultGTFSupported =  CHECK_BIT(stEDIDBuffer.pucBuffer[0x18], 0);

        mTBOX_TRACE((kTBOX_NIV_1, "bIsStandby[%d], bIsSuspend[%d], bIsActiveOff[%d], bIsRGBDefaultColorSpace[%d], bIsPreferredTimingMode[%d], bIsDefaultGTFSupported[%d]\n",
              pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsStandby,
              pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsSuspend,
              pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsActiveOff,
              pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsRGBDefaultColorSpace,
              pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsPreferredTimingMode,
              pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.bIsDefaultGTFSupported));

        if (pstEDIDData->stEDIDBasic.stDisplayParams.stVideoInput.bIsDigitalSignal)
        {
            //if it is digital
            //TODO: TDAL HDMI doesn't have enum for this.
            pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.eDisplayType = (stEDIDBuffer.pucBuffer[0x18] >> 3) & 0x03;
        }
        else
        {
            //if it is analog
            switch((stEDIDBuffer.pucBuffer[0x18] >> 3) & 0x03) //bits 4,3
            {
                case   0x00:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.eDisplayType = eTDAL_HDMI_EDID_DISPLAY_TYPE_MONOCHROME;
                    break;
                case   0x01:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.eDisplayType = eTDAL_HDMI_EDID_DISPLAY_TYPE_RGB;
                    break;
                case   0x02:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.eDisplayType = eTDAL_HDMI_EDID_DISPLAY_TYPE_NON_RGB;
                    break;
                case   0x03:
                    pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.eDisplayType = eTDAL_HDMI_EDID_DISPLAY_TYPE_UNKNOWN;
                    break;
            }
        }
        mTBOX_TRACE((kTBOX_NIV_1, "Display Type 0x%X\n", pstEDIDData->stEDIDBasic.stDisplayParams.stFeatureSupport.eDisplayType));

#if 0
        //TODO: need to know format of data
        pstEDIDData->stEDIDBasic.stColorParams.uiRed_x      = EDIDSink.EDIDBasic.EDIDColorParams.Red_x;
        pstEDIDData->stEDIDBasic.stColorParams.uiRed_y      = EDIDSink.EDIDBasic.EDIDColorParams.Red_y;
        pstEDIDData->stEDIDBasic.stColorParams.uiGreen_x    = EDIDSink.EDIDBasic.EDIDColorParams.Green_x;
        pstEDIDData->stEDIDBasic.stColorParams.uiGreen_y    = EDIDSink.EDIDBasic.EDIDColorParams.Green_y;
        pstEDIDData->stEDIDBasic.stColorParams.uiBlue_x     = EDIDSink.EDIDBasic.EDIDColorParams.Blue_x;
        pstEDIDData->stEDIDBasic.stColorParams.uiBlue_y     = EDIDSink.EDIDBasic.EDIDColorParams.Blue_y;
        pstEDIDData->stEDIDBasic.stColorParams.uiWhite_x    = EDIDSink.EDIDBasic.EDIDColorParams.White_x;
        pstEDIDData->stEDIDBasic.stColorParams.uiWhite_y    = EDIDSink.EDIDBasic.EDIDColorParams.White_y;
#endif

        videoStandardSupported = eTDAL_OUTPUT_VIDEO_HD_STD_NONE;

        /* Fill timing info */
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 0))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_800_600_60;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 1))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_800_600_56;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 2))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_640_480_75;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 3))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_640_480_72;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 4))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_640_480_67;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 5)) {
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_640_480_60;
            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_640_480_P_60;
        }
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 6))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_720_400_88;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x23], 7))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1 |= eTDAL_HDMI_ESTABLISHED_TIMING_720_400_70;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 0))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_1280_1024_75;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 1))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_75;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 2))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_70;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 3))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_60;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 4))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_1024_768_87;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 5))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_832_624_75;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 6))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_800_600_75;
        if (CHECK_BIT(stEDIDBuffer.pucBuffer[0x24], 7))
            pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2 |= eTDAL_HDMI_ESTABLISHED_TIMING_800_600_72;

        mTBOX_TRACE((kTBOX_NIV_1, "eTiming1[0x%X], eTiming2[0x%X]\n",
             pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming1,
              pstEDIDData->stEDIDBasic.stEstablishedTimings.eTiming2));

        //FIXME: I did not found this info in EDID. I hope it can be set to TRUE.
        pstEDIDData->stEDIDBasic.stEstablishedTimings.bIsManufacturerTiming = TRUE;

        int i=0, j=0;
        /* Standard timing information */
        for (i = 0; i < 8; i++)
        {
            pstEDIDData->stEDIDBasic.stStdTiming[i].bIsTimingUsed
                    = (stEDIDBuffer.pucBuffer[0x26 + i*2] != 0x01 || stEDIDBuffer.pucBuffer[0x26 + i*2] != 0x01);
            if (pstEDIDData->stEDIDBasic.stStdTiming[i].bIsTimingUsed)
            {
                pstEDIDData->stEDIDBasic.stStdTiming[i].uiHorizActive = (stEDIDBuffer.pucBuffer[0x26 + i*2] + 31)* 8;
                int num, denum;
                switch ((stEDIDBuffer.pucBuffer[0x26 + i*2 + 1] >> 6) & 0x03) //7,6 bits
                {
                    case 0x00:
                        num = 16; denum = 10;
                        break;
                    case 0x01:
                        num = 4; denum = 3;
                        break;
                    case 0x02:
                        num = 5; denum = 4;
                        break;
                    case 0x03:
                        num = 16; denum = 9;
                        break;
                }
                pstEDIDData->stEDIDBasic.stStdTiming[i].uiVertActive = (pstEDIDData->stEDIDBasic.stStdTiming[i].uiHorizActive * denum) / num;
                pstEDIDData->stEDIDBasic.stStdTiming[i].uiRefreshRate = 60 + (stEDIDBuffer.pucBuffer[0x26 + i*2 + 1] & 0x3F);
            }
        }

        for (i = 0; i < 8; i++)
        {
            if (pstEDIDData->stEDIDBasic.stStdTiming[i].bIsTimingUsed)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Mode[%dx%d] Refresh rate[%d]\n",
                       pstEDIDData->stEDIDBasic.stStdTiming[i].uiHorizActive,
                       pstEDIDData->stEDIDBasic.stStdTiming[i].uiVertActive,
                       pstEDIDData->stEDIDBasic.stStdTiming[i].uiRefreshRate));
            }
        }

		bool bIsDetailedTimingFound = FALSE;
        tTDAL_HDMI_EDIDTimingDescription *pstDetailedTimingDesc = NULL;
		pstDetailedTimingDesc = &(pstEDIDData->stEDIDBasic.stDetailedTimingDesc);

		for(i = 0; i < 4; i++)
		{
			MS_U8 u8DescriptionStart = 0x36 + 18 * i;
			if(126 - u8DescriptionStart < 18)    //Detailed Timing Section need 18 byte
            {
                break;
            }

			if (bIsDetailedTimingFound == TRUE)
            {
                pstDetailedTimingDesc = &(pstEDIDData->stEDIDBasic.stDetailedTimingDesc2nd);
            }
            else
            {
                bIsDetailedTimingFound = TRUE;
            }

			if( (!(stEDIDBuffer.pucBuffer[u8DescriptionStart] == 0x00 && stEDIDBuffer.pucBuffer[u8DescriptionStart+1] == 0x00))/* && (pstEDIDData->stEDIDBasic.bDetailedTimingDesc2ndFill == FALSE) */)    //Monitor
			{
				/*Fill detailed timing description */
		        //pixel clock in Hz
		        pstDetailedTimingDesc->uiPixelClock 		= 10000 * (stEDIDBuffer.pucBuffer[u8DescriptionStart] | stEDIDBuffer.pucBuffer[u8DescriptionStart + 1] << 8);
				pstDetailedTimingDesc->uiHorizActivePixel 	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 2] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 4] & 0xF0) << 4);
		        pstDetailedTimingDesc->uiHorizBlankingPixel = stEDIDBuffer.pucBuffer[u8DescriptionStart + 3] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 4] & 0x0F) << 8);
		        pstDetailedTimingDesc->uiVertActiveLine 	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 5] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 7] & 0xF0) << 4);
		        pstDetailedTimingDesc->uiVertBlankingLine 	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 6] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 7] & 0x0F) << 8);
		        pstDetailedTimingDesc->uiHorizSyncOffset 	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 8] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0B] & 0xC0) << 2);
		        pstDetailedTimingDesc->uiHorizSyncWidth 	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 9] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0B] & 0x03) << 4);
		        pstDetailedTimingDesc->uiVertSyncOffset 	= ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0A] & 0xF0) >> 4)  | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0B] & 0x0C) << 2);
		        pstDetailedTimingDesc->uiVertSyncWidth 		= (stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0A] & 0x0F)  | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0B] & 0x03) << 4);
		        pstDetailedTimingDesc->uiHorizImageSize 	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0C] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0E] & 0xF0) << 4);
		        pstDetailedTimingDesc->uiVertImageSize 		= stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0D] | ((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0E] & 0x0F) << 8);

				pstDetailedTimingDesc->uiHorizBorder      	= stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x0F];
                pstDetailedTimingDesc->uiVertBorder    		= stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x10];
                pstDetailedTimingDesc->bIsInterlaced		= CHECK_BIT(stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x11], 7);
                pstDetailedTimingDesc->bIsHorizPolaPositive = CHECK_BIT(stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x11], 1);
                pstDetailedTimingDesc->bIsVertPolaPositive	= CHECK_BIT(stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x11], 2);

		        switch (((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x11] & 0x60) >> 4) | (stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x11] & 0x01))
        {
            case 0x00:
            case 0x01:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_NO_STEREO;
                break;
            case 0x02:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_STEREO_RIGHT_IMAGE;
                break;
            case 0x04:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_STEREO_LEFT_IMAGE;
                break;
            case 0x03:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_STEREO_2WAY_INTERLEAVED_RIGHT;
                break;
            case 0x05:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_STEREO_2WAY_INTERLEAVED_LEFT;
                break;
            case 0x06:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_STEREO_4WAY_INTERLEAVED;
                break;
            case 0x07:
		                pstDetailedTimingDesc->eDecodeStereo = eTDAL_HDMI_EDID_STEREO_SBS_INTERLEAVED;
                break;
        }

		        switch (((stEDIDBuffer.pucBuffer[u8DescriptionStart + 0x11] & 0x18) >> 3))
        {
            case 0x00:
		                pstDetailedTimingDesc->eSyncSignal = eTDAL_HDMI_EDID_SYNC_ANALOG_COMPOSITE;
                break;
            case 0x01:
		                pstDetailedTimingDesc->eSyncSignal = eTDAL_HDMI_EDID_SYNC_BIP_ANALOG_COMPOSITE;
                break;
            case 0x02:
		                pstDetailedTimingDesc->eSyncSignal = eTDAL_HDMI_EDID_SYNC_DIGITAL_COMPOSITE;
                break;
            case 0x03:
		                pstDetailedTimingDesc->eSyncSignal = eTDAL_HDMI_EDID_SYNC_DIGITAL_SEPARATE;
                break;
        }
/*

				printf("uiPixelClock[%d]\n"
                "uiHorizActivePixel[%d]\n"
                "uiHorizBlankingPixel[%d]\n"
                "uiVertActiveLine[%d]\n"
                "uiVertBlankingLine[%d]\n"
                "uiHorizSyncOffset[%d]\n"
                "uiHorizSyncWidth[%d]\n"
                "uiVertSyncOffset[%d]\n"
                "uiVertSyncWidth[%d]\n"
                "uiHorizImageSize[%d]\n"
                "uiVertImageSize[%d]\n"

				"uiHorizBorder[%d]\n"
				"uiVertBorder[%d]\n"
                "bIsInterlaced[%d]\n"
				"bIsHorizPolaPositive[%d]\n"
				"bIsVertPolaPositive[%d]\n"

                "eDecodeStereo[%d]\n"
                "eSyncSignal[%d]\n",

				pstDetailedTimingDesc->uiPixelClock,
				pstDetailedTimingDesc->uiHorizActivePixel,
				pstDetailedTimingDesc->uiHorizBlankingPixel,
				pstDetailedTimingDesc->uiVertActiveLine,
				pstDetailedTimingDesc->uiVertBlankingLine,
				pstDetailedTimingDesc->uiHorizSyncOffset,
				pstDetailedTimingDesc->uiHorizSyncWidth,
				pstDetailedTimingDesc->uiVertSyncOffset,
				pstDetailedTimingDesc->uiVertSyncWidth,
				pstDetailedTimingDesc->uiHorizImageSize,
				pstDetailedTimingDesc->uiVertImageSize,

				pstDetailedTimingDesc->uiHorizBorder,
				pstDetailedTimingDesc->uiVertBorder,
				pstDetailedTimingDesc->bIsInterlaced,
				pstDetailedTimingDesc->bIsHorizPolaPositive,
				pstDetailedTimingDesc->bIsVertPolaPositive,
				pstDetailedTimingDesc->eDecodeStereo,
				pstDetailedTimingDesc->eSyncSignal);
*/
			}
		}

        pstEDIDData->bIsEDIDExtensionUsed = FALSE;
        pstEDIDData->stEDIDBasic.ucExtensionFlag = stEDIDBuffer.pucBuffer[0x7E];

        mTBOX_TRACE((kTBOX_NIV_1, "Number of extension block is [%d]\n",  pstEDIDData->stEDIDBasic.ucExtensionFlag));
        if(pstEDIDData->stEDIDBasic.ucExtensionFlag > 0)
        {
            MS_U8 n = 0;
            for(n = 1;n <= pstEDIDData->stEDIDBasic.ucExtensionFlag;n++)
            {
                uint8_t* extension_block1 = stEDIDBuffer.pucBuffer + 128*n;

                for (i=0; i<128; i++)
                {
                    if ((i%16)==15) printf("\n");
                }
            }
        }


        /* If we have extension block */
        if (pstEDIDData->stEDIDBasic.ucExtensionFlag != 0
            && pstEDIDData->stEDIDBasic.ucRevision != 0x02
            && stEDIDBuffer.uiLength > 128
            && stEDIDBuffer.pucBuffer[128] == 0x02)
        {
            tTDAL_HDMI_EDIDDataExtension *pstEDIDExtension = NULL;
            uint8_t* extension_block = stEDIDBuffer.pucBuffer + 128;

            pstEDIDExtension  = (tTDAL_HDMI_EDIDDataExtension *)TDAL_Malloc(sizeof(tTDAL_HDMI_EDIDDataExtension));
            if (pstEDIDExtension == NULL)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Unable to allocate memory for Extension Block\n"));
                return eTDAL_HDMI_NO_MEMORY_ERROR;
            }
            pstEDIDData->pstEDIDExtension = pstEDIDExtension;

            if (pstEDIDData->stEDIDBasic.ucRevision == 0x01)
            {
                //TODO: add support of revision 1.
            }
            else if (pstEDIDData->stEDIDBasic.ucRevision == 0x03)
            {
                pstEDIDData->bIsEDIDExtensionUsed = TRUE;
                pstEDIDExtension->uBlock.stCEAExt.uiRevisionNumber = extension_block[0x01];
                if (extension_block[0x02] >= 4) //we have data in this extension block
                {
                    pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsUnderscanSink = CHECK_BIT(extension_block[0x03], 7);
                    pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsBasicAudioSupported = CHECK_BIT(extension_block[0x03], 6);
                    pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsYCbCr444Supported = CHECK_BIT(extension_block[0x03], 5);
                    pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsYCbCr422Supported = CHECK_BIT(extension_block[0x03], 4);
                    pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.ucNbTimingDesc = extension_block[0x03] & 0x0F;
					pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc = TDAL_Malloc(sizeof(tTDAL_HDMI_EDIDTimingDescription)*6);

                    mTBOX_TRACE((kTBOX_NIV_1, "Extension block revisions[%d]\n",  pstEDIDExtension->uBlock.stCEAExt.uiRevisionNumber));
                    mTBOX_TRACE((kTBOX_NIV_1, "bIsUnderscanSink[%d]\n",pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsUnderscanSink));
                    mTBOX_TRACE((kTBOX_NIV_1, "bIsBasicAudioSupported[%d]\n",pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsBasicAudioSupported));
                    mTBOX_TRACE((kTBOX_NIV_1, "bIsYCbCr444Supported[%d]\n",pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsYCbCr444Supported));
                    mTBOX_TRACE((kTBOX_NIV_1, "bIsYCbCr422Supported[%d]\n",pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.bIsYCbCr422Supported));
                    mTBOX_TRACE((kTBOX_NIV_1, "Number of DTD[%d]\n",  pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.ucNbTimingDesc));

                    uint8_t u8LenofData = 4;
                    uint8_t u8AudioFormatCode = 0;
                    uint8_t u8AudioDataLen = 0;
                    uint8_t u8VideoDataLen = 0;
                    tTDAL_HDMI_EDIDCEA_SVD *svdPtr;
                    tTDAL_HDMI_EDIDCEA_SAD *sadPtr;

                    while(u8LenofData != extension_block[0x02])
                    {
                    	if (((extension_block[u8LenofData] & BMASK(7:5)) >> 5) == 1)            //Audio Tag, SAD
                    	{
	                        u8AudioDataLen = u8AudioDataLen + extension_block[u8LenofData] & BMASK(4:0); 	//audio data block length
	                        u8LenofData = u8LenofData +1;
	                        i = 0;
	                        j = 0;
                                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSAD = NULL;

	                        while(i < u8AudioDataLen)
	                        {
	                          if (i == 0)
                                  {
                                    sadPtr = (tTDAL_HDMI_EDIDCEA_SAD *)TDAL_Malloc(sizeof(tTDAL_HDMI_EDIDCEA_SAD));
                                    pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSAD = sadPtr;
                                  }
                                  else
                                  {
                                    sadPtr->pstNext = (tTDAL_HDMI_EDIDCEA_SAD *)TDAL_Malloc(sizeof(tTDAL_HDMI_EDIDCEA_SAD));
                                    sadPtr = sadPtr->pstNext;
                                  }
                                  sadPtr->pstNext = NULL;

                                  u8AudioFormatCode = (extension_block[u8LenofData+i] & BMASK(6:3)) >> 3;
				  switch(u8AudioFormatCode)
				  {
				  case 0x01:    //L-PCM
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_LPCM;
									/*
				    sadPtr->stAudioFormat.bSupportLPCM24bit = (((extension_block[u8LenofData + 2 + i] & BIT(2)) != 0) ? 1 : 0);
				    sadPtr->stAudioFormat.bSupportLPCM20bit = (((extension_block[u8LenofData + 2 + i] & BIT(1)) != 0) ? 1 : 0);
				    sadPtr->stAudioFormat.bSupportLPCM16bit = (((extension_block[u8LenofData + 2 + i] & BIT(0)) != 0) ? 1 : 0);
									*/
				    break;

				  case 0x02:    //AC3
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_AC3;
				    break;
				  case 0x03:    //MPEG1
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG1;
				    break;
				  case 0x04:    //MP3
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_MP3;
				    break;
				  case 0x05:    //MPEG2
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG2;
				    break;
				  case 0x06:    //AAC LC
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_AAC;
				    break;
				  case 0x07:    //DTS
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_DTS;
				    break;
				  case 0x08:    //ATRAC
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_ATRAC;
				    break;
				  case 0x09:    //One Bit Audio
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_OBA;
				    break;
				  case 0x0A:    //Enhanced AC3
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_DBPLUS;
				    break;
				  case 0x0B:    //DTS HD
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_DTS_HD;
				    break;
				  case 0x0C:    //MAT
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_MLP;
				    break;
									/*
				  case 0x0D:    //DST
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_DST;
				    break;
				  case 0x0E:    //WMA Pro
				    sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_WMAP;
				    break;
				  case 0x0F:    //Audio Coding Extension Type
				    switch ((extension_block[u8LenofData + 2 + i] & BMASK(7:3)) >> 3)
				    {
					case 0x04:    //MPEG-4 HE_AAC
					    sadPtr->stAudioFormat.eStreamType =  eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG4HE_AAC;
					    break;
					case 0x05:    //MPEG-4 HE AAC v2
					    sadPtr->stAudioFormat.eStreamType =  eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG4HE_AACv2;
					    break;
					case 0x06:    //MPEG-4 AAC LC
					    sadPtr->stAudioFormat.eStreamType =  eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG4AAC_LC;
					    break;
					case 0x07:    //DRA
					    sadPtr->stAudioFormat.eStreamType =  eTDAL_HDMI_AUDIO_STREAM_TYPE_DRA;
					    break;
					case 0x08:    //MPEG-4 HE AAC + MPEG Surround
					    sadPtr->stAudioFormat.eStreamType =  eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG4HE_AAC_MPEG_SURROUND;
					    break;
					case 0x0A:    //MPEG-4 AAC LC + MPEG Surround
					    sadPtr->stAudioFormat.eStreamType =  eTDAL_HDMI_AUDIO_STREAM_TYPE_MPEG4_AACLC_MPEG_SURROUND;
					    break;
					default:
					    break;
				    }
				    break;
							*/
				  default:
					sadPtr->stAudioFormat.eStreamType = eTDAL_HDMI_AUDIO_STREAM_TYPE_UNKNOWN;
				    break;
				  }

				sadPtr->stAudioFormat.uiSampleFreqMask = eTDAL_HDMI_AUDIO_SAMPLE_FREQ_UNKNOWN;
                                if (((extension_block[u8LenofData + 1 + i] & BIT(0)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_32_KHZ;
                                }
                                if (((extension_block[u8LenofData + 1 + i] & BIT(1)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_44_1_KHZ;
                                }
                                if (((extension_block[u8LenofData +  1 + i] & BIT(2)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_48_KHZ;
                                }
                                if (((extension_block[u8LenofData + 1 + i] & BIT(3)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_88_2_KHZ;
                                }
                                if (((extension_block[u8LenofData + 1 + i] & BIT(4)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_96_KHZ;
                                }
                                if (((extension_block[u8LenofData + 1 + i] & BIT(5)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_176_4_KHZ;
                                }
                                if (((extension_block[u8LenofData + 1 + i] & BIT(6)) != 0) ? 1 : 0)
                                {
                                    sadPtr->stAudioFormat.uiSampleFreqMask |= eTDAL_HDMI_AUDIO_SAMPLE_FREQ_192_KHZ;
                                }

				sadPtr->stAudioFormat.uiMaxChannel = (extension_block[u8LenofData+i] & BMASK(2:0)) + 1;

/*
				if( ( u8AudioFormatCode >= 2 ) && ( u8AudioFormatCode <= 8 ) )
				{
					sadPtr->stAudioFormat.u16MaxBitRate = extension_block[u8LenofData + 2 + i] * 8;
				}
		                    sadPtr->stAudioFormat.bSupport192kHz = (((extension_block[u8LenofData + 1 + i] & BIT(6)) != 0) ? 1 : 0);
		                    sadPtr->stAudioFormat.bSupport176d4kHz = (((extension_block[u8LenofData + 1 + i] & BIT(5)) != 0) ? 1 : 0);
		                    sadPtr->stAudioFormat.bSupport96kHz = (((extension_block[u8LenofData + 1 + i] & BIT(4)) != 0) ? 1 : 0);
		                    sadPtr->stAudioFormat.bSupport88d2kHz= (((extension_block[u8LenofData + 1 + i] & BIT(3)) != 0) ? 1 : 0);
		                    sadPtr->stAudioFormat.bSupport48kHz= (((extension_block[u8LenofData +  1 + i] & BIT(2)) != 0) ? 1 : 0);
		                    sadPtr->stAudioFormat.bSupport44d1kHz= (((extension_block[u8LenofData + 1 + i] & BIT(1)) != 0) ? 1 : 0);
		                    sadPtr->stAudioFormat.bSupport32kHz= (((extension_block[u8LenofData + 1 + i] & BIT(0)) != 0) ? 1 : 0);
								*/

	                            i = i + 3;
	                            j= j + 1;
                        }
                        u8LenofData = u8LenofData + u8AudioDataLen;
                    }
                    else if (((extension_block[u8LenofData] & BMASK(7:5)) >> 5) == 2)               //Video Tag, SVD
                    {
                    	pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSVD = NULL;
                        u8VideoDataLen = extension_block[u8LenofData] & BMASK(4:0);	//length of video bytes
                        u8LenofData = u8LenofData +1;

                        for(i = 0; i < u8VideoDataLen; i = i + 1)
                        {
                        	if (i == 0)
                        {
                            svdPtr = (tTDAL_HDMI_EDIDCEA_SVD *)TDAL_Malloc(sizeof(tTDAL_HDMI_EDIDCEA_SVD));
                            pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSVD = svdPtr;
                        }
                        else
                        {
                            svdPtr->pstNext = (tTDAL_HDMI_EDIDCEA_SVD *)TDAL_Malloc(sizeof(tTDAL_HDMI_EDIDCEA_SVD));
                            svdPtr = svdPtr->pstNext;
                        }
							svdPtr->bIsNative = extension_block[u8LenofData + i] >> 7;
							svdPtr->uiVideoIdentCode = extension_block[u8LenofData + i] & 0x7F;
							svdPtr->pstNext = NULL;

			switch(svdPtr->uiVideoIdentCode) {
                            case   1:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_640_480_P_60;	break;
                            case   2:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_60;	break;
                            case   3:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_P_60;	break;
                            case   4:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_60;	break;
                            case   5:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_60;	break;
                            case   6:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_60;	break;
                            case   7:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_480_I_60;	break;
                            case  16:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_60;	break;
                            case  17:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_576_P_50;	break;
                            case  18:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_576_P_50;	break;
                            case  19:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_50;	break;
                            case  20:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;	break;
                            case  21:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50;	break;
                            case  22:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_720_576_I_50;	break;
                            case  31:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_50;	break;
                            case  32:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2398;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_24;	break;
                            case  33:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_25;	break;
                            case  34:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2997;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_30;	break;
                            case  39:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_I_50;	break;
                            case  60:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2398;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_24;	break;
                            case  61:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_25;	break;
                            case  62:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2997;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_30;	break;
                            case  65:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2398;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_24;	break;
                            case  66:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_25;	break;
                            case  67:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_2997;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_30;	break;
                            case  68:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_50;	break;
                            case  69:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1280_720_P_60;	break;
                            case  72:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2398;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_24;	break;
                            case  73:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_25;	break;
                            case  74:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_2997;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_30;	break;
                            case  75:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_50;	break;
                            case  76:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_5994;
                                            videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_1920_1080_P_60;	break;
#if 0
                            case  94:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_25;	break;
                            case  95:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_30;	break;
                            case  96:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_50;	break;
                            case  97:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_60;	break;
                            case 104:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_25;	break;
                            case 105:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_30;	break;
                            case 106:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_50;	break;
                            case 107:	videoStandardSupported |= eTDAL_OUTPUT_VIDEO_HD_STD_3840_2160_P_60;	break;
#endif
			}

			extension_block[u8LenofData + i] = extension_block[u8LenofData + i] & 0x7F;
 			if(extension_block[u8LenofData + i] == 0 || extension_block[u8LenofData + i] > 127) //127 means CEA861-E,The lower 7-bits are an index associated with the video format supported.
			{
                        	break;
                    	}

                       }
                       u8LenofData = u8LenofData + u8VideoDataLen;
		       isVideoModesParsed = TRUE;

                    }
		    else   //reserved
		    {
			u8LenofData = u8LenofData +  (extension_block[u8LenofData] & BMASK(4:0)) + 1;
		    }

		  }

					for(i = 0; i < 6; i++)		//MAX 123/18~=6
					{
						MS_U8 u8DescriptionStart = extension_block[0x02] + 18 * i;
	                    if(126 - u8DescriptionStart < 18)   //Detailed Timing Section need 18 byte
	                    {
	                        break;
	                    }

						if(extension_block[u8DescriptionStart] != 0x00)
						{
							/*Fill detailed timing description */
					        //pixel clock in Hz
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiPixelClock 		= 10000 * (extension_block[u8DescriptionStart] | extension_block[u8DescriptionStart + 1] << 8);
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizActivePixel 	= extension_block[u8DescriptionStart + 2] | ((extension_block[u8DescriptionStart + 4] & 0xF0) << 4);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizBlankingPixel = extension_block[u8DescriptionStart + 3] | ((extension_block[u8DescriptionStart + 4] & 0x0F) << 8);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertActiveLine 	= extension_block[u8DescriptionStart + 5] | ((extension_block[u8DescriptionStart + 7] & 0xF0) << 4);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertBlankingLine 	= extension_block[u8DescriptionStart + 6] | ((extension_block[u8DescriptionStart + 7] & 0x0F) << 8);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizSyncOffset 	= extension_block[u8DescriptionStart + 8] | ((extension_block[u8DescriptionStart + 0x0B] & 0xC0) << 2);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizSyncWidth 	= extension_block[u8DescriptionStart + 9] | ((extension_block[u8DescriptionStart + 0x0B] & 0x03) << 4);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertSyncOffset 	= ((extension_block[u8DescriptionStart + 0x0A] & 0xF0) >> 4)  | ((extension_block[u8DescriptionStart + 0x0B] & 0x0C) << 2);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertSyncWidth 		= (extension_block[u8DescriptionStart + 0x0A] & 0x0F)  | ((extension_block[u8DescriptionStart + 0x0B] & 0x03) << 4);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizImageSize 	= extension_block[u8DescriptionStart + 0x0C] | ((extension_block[u8DescriptionStart + 0x0E] & 0xF0) << 4);
					        pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertImageSize 		= extension_block[u8DescriptionStart + 0x0D] | ((extension_block[u8DescriptionStart + 0x0E] & 0x0F) << 8);

							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizBorder      	= extension_block[u8DescriptionStart + 0x0F];
			                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertBorder    		= extension_block[u8DescriptionStart + 0x10];
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].bIsInterlaced 		= CHECK_BIT(extension_block[u8DescriptionStart + 0x11], 7);
			                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].bIsHorizPolaPositive = CHECK_BIT(extension_block[u8DescriptionStart + 0x11], 1);
			                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].bIsVertPolaPositive	= CHECK_BIT(extension_block[u8DescriptionStart + 0x11], 2);

					        switch (((extension_block[u8DescriptionStart + 0x11] & 0x60) >> 4) | (extension_block[u8DescriptionStart + 0x11] & 0x01))
					        {
					            case 0x00:
					            case 0x01:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_NO_STEREO;
					                break;
					            case 0x02:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_STEREO_RIGHT_IMAGE;
					                break;
					            case 0x04:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_STEREO_LEFT_IMAGE;
					                break;
					            case 0x03:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_STEREO_2WAY_INTERLEAVED_RIGHT;
					                break;
					            case 0x05:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_STEREO_2WAY_INTERLEAVED_LEFT;
					                break;
					            case 0x06:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_STEREO_4WAY_INTERLEAVED;
					                break;
					            case 0x07:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo = eTDAL_HDMI_EDID_STEREO_SBS_INTERLEAVED;
					                break;
					        }

					        switch (((extension_block[u8DescriptionStart + 0x11] & 0x18) >> 3))
					        {
					            case 0x00:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eSyncSignal = eTDAL_HDMI_EDID_SYNC_ANALOG_COMPOSITE;
					                break;
					            case 0x01:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eSyncSignal = eTDAL_HDMI_EDID_SYNC_BIP_ANALOG_COMPOSITE;
					                break;
					            case 0x02:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eSyncSignal = eTDAL_HDMI_EDID_SYNC_DIGITAL_COMPOSITE;
					                break;
					            case 0x03:
					                pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eSyncSignal = eTDAL_HDMI_EDID_SYNC_DIGITAL_SEPARATE;
					                break;
					        }
/*
							printf("uiPixelClock[%d]\n"
							"uiHorizActivePixel[%d]\n"
							"uiHorizBlankingPixel[%d]\n"
							"uiVertActiveLine[%d]\n"
							"uiVertBlankingLine[%d]\n"
							"uiHorizSyncOffset[%d]\n"
							"uiHorizSyncWidth[%d]\n"
							"uiVertSyncOffset[%d]\n"
							"uiVertSyncWidth[%d]\n"
							"uiHorizImageSize[%d]\n"
							"uiVertImageSize[%d]\n"
							"uiHorizBorder[%d]\n"
							"uiVertBorder[%d]\n"
							"bIsInterlaced[%d]\n"
							"bIsHorizPolaPositive[%d]\n"
							"bIsVertPolaPositive[%d]\n"
							"eDecodeStereo[%d]\n"
							"eSyncSignal[%d]\n",
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiPixelClock,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizActivePixel,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizBlankingPixel,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertActiveLine,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertBlankingLine,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizSyncOffset,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizSyncWidth,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertSyncOffset,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertSyncWidth,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizImageSize,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertImageSize,

							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiHorizBorder,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].uiVertBorder,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].bIsInterlaced,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].bIsHorizPolaPositive,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].bIsVertPolaPositive,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eDecodeStereo,
							pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc[i].eSyncSignal);
*/
						}
					}
                }
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Unsupported revision of HDMI[%d] for EDID Ext block parsing\n"));
            }


        }


#if 0
        /*FIXME: why that function doesn't work? Need to ask MStar .*/
        if (MApi_HDMITx_GetRxIDManufacturerName(pstEDIDData->stEDIDBasic.stVendorProductIdent.cIDManufacturerName) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s:  MApi_HDMITx_GetRxIDManufacturerName is failed \n", TBOX_FunctionName   ));
        }
#endif


        TDAL_Free(stEDIDBuffer.pucBuffer);
    }
    mTBOX_RETURN(Error);
}

tTDAL_HDMI_Error   TDAL_HDMI_EDIDDataFree(tTDAL_HDMI_SourceId   Id, tTDAL_HDMI_EDIDData   *pstEDIDData)
{
    tTDAL_HDMI_Error      Error = eTDAL_HDMI_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_HDMI_EDIDDataFree");

    /*   check   init*/
    if(bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
       Error = eTDAL_HDMI_NOT_INIT_ERROR;
    }

    /*   check   id*/
    if((Id != eTDAL_HDMI_SOURCE_ID_0) || (pstEDIDData == NULL))
    {
        mTBOX_RETURN(eTDAL_HDMI_BAD_PARAMETER_ERROR);
    }

    if(NULL == pstEDIDData->pstEDIDExtension)
    {
        mTBOX_RETURN(eTDAL_HDMI_BAD_PARAMETER_ERROR);
    }

    if(pstEDIDData->stEDIDBasic.ucRevision == 1)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "ucRevision == 1 -> free pstTimingDesc\n"));
        if(NULL != pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "ucRevision == 1 -> free pstTimingDesc\n"));
            TDAL_Free(pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer1.pstTimingDesc);
        }
    }
    else   if(pstEDIDData->stEDIDBasic.ucRevision == 3)
    {
        if(NULL != pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc)
        {
             mTBOX_TRACE((kTBOX_NIV_1, "ucRevision == 3 -> free pstTimingDesc\n"));
             TDAL_Free(pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstTimingDesc);
        }

        if (pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSAD != NULL)
        {
            tTDAL_HDMI_EDIDCEA_SAD* pstCurrentSAD = pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSAD;
            tTDAL_HDMI_EDIDCEA_SAD* pstNextSAD = pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSAD->pstNext;

            mTBOX_TRACE((kTBOX_NIV_1, "ucRevision == 3 -> free pstSAD\n"));

            while(pstCurrentSAD != NULL)
            {
                TDAL_Free(pstCurrentSAD);
                pstCurrentSAD = pstNextSAD;
                if (pstCurrentSAD != NULL)
                {
                    pstNextSAD = pstCurrentSAD->pstNext;
                }
            }
        }
        if (pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSVD != NULL)
        {
            tTDAL_HDMI_EDIDCEA_SVD* pstCurrentSVD = pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSVD;
            tTDAL_HDMI_EDIDCEA_SVD* pstNextSVD = pstEDIDData->pstEDIDExtension->uBlock.stCEAExt.uVersionSpecific.stVer3.pstSVD->pstNext;

            mTBOX_TRACE((kTBOX_NIV_1, "ucRevision == 3 -> free pstNextSVD\n"));

            while(pstCurrentSVD != NULL)
            {
                TDAL_Free(pstCurrentSVD);
                pstCurrentSVD = pstNextSVD;
                if (pstCurrentSVD != NULL)
                {
                    pstNextSVD = pstCurrentSVD->pstNext;
                }
            }
        }

    }

    mTBOX_TRACE((kTBOX_NIV_1, "free pstEDIDExtension\n"));
    TDAL_Free(pstEDIDData->pstEDIDExtension);

    mTBOX_RETURN(Error);
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
    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;
    uint8_t edid_buffer[2][128];
    int num_of_blocks = 0;
    int block;

    mTBOX_FCT_ENTER("TDAL_HDMI_EDIDRawBufferGet");

    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    else if (Id != eTDAL_HDMI_SOURCE_ID_0)
    {
        ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
    }
    else if (pstEDIDBuffer == NULL)
    {
        ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
    }
    else
    {
        memset(edid_buffer, 0, sizeof(edid_buffer));
        for (block = 0; block < 2; block++)
        {
            if (MApi_HDMITx_GetEDIDData(edid_buffer[block], block) == FALSE)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MApi_HDMITx_GetEDIDData is failed\n"));
                ReturnCode = eTDAL_HDMI_NOT_AVAILABLE_ERROR;
            }
            else
            {
                num_of_blocks++;
#ifdef TRACE_IDID_INFO
                int i;
                for (i = 0; i < 128; i++)
                {
                    printf("0x%02X ", edid_buffer[block][i]);
                    if (((i + 1) % 16) == 0)
                    {
                        printf("\r\n");
                    }
                }
#endif
            }
        }

        if (ReturnCode == eTDAL_HDMI_NO_ERROR)
        {
            //alloc memory for EDID buffer
            //don't forget to release it upper layer
            pstEDIDBuffer->pucBuffer = (uint8_t *)TDAL_Malloc(sizeof(edid_buffer));
            if (pstEDIDBuffer->pucBuffer == NULL)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Unable to allocate memory for EDID block\n"));
                ReturnCode = eTDAL_HDMI_NO_MEMORY_ERROR;
            }
            else
            {
                pstEDIDBuffer->uiLength = num_of_blocks * 128;
                memcpy(pstEDIDBuffer->pucBuffer, edid_buffer, num_of_blocks * 128);
            }
        }
    }

    return(ReturnCode);
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
    mTBOX_FCT_ENTER("TDAL_HDMI_InfoFrameSPDSet");

    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        return (eTDAL_HDMI_NOT_INIT_ERROR);
    }
    else if (Id != eTDAL_HDMI_SOURCE_ID_0)
    {
        return (eTDAL_HDMI_BAD_PARAMETER_ERROR);
    }
    else if (pstSPD == NULL)
    {
        return (eTDAL_HDMI_BAD_PARAMETER_ERROR);
    }

    //!TODO: Need to implement it
    // but it is not used now in high level app
    // MStar API doesn't have any functions to set these parameters
    // except of MApi_HDMITx_PKT_User_Define and MApi_HDMITx_PKT_Content_Define which are not documented
    // and don't have any examples.

    return (eTDAL_HDMI_NOT_AVAILABLE_ERROR);
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
    mTBOX_FCT_ENTER("TDAL_HDMI_InfoFrameVSSet");

    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        return (eTDAL_HDMI_NOT_INIT_ERROR);
    }
    if (Id != eTDAL_HDMI_SOURCE_ID_0)
    {
        return (eTDAL_HDMI_BAD_PARAMETER_ERROR);
    }
    if (pstVS == NULL)
    {
        return (eTDAL_HDMI_BAD_PARAMETER_ERROR);
    }
    //!TODO: Need to implement it
    // but it is not used now in high level app
    // MStar API doesn't have any functions to set these parameters
    // except of MApi_HDMITx_PKT_User_Define and MApi_HDMITx_PKT_Content_Define which are not documented
    // and don't have any examples.
    return (eTDAL_HDMI_NOT_AVAILABLE_ERROR);
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
    tTDAL_HDMI_Error    ReturnCode = eTDAL_HDMI_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_HDMI_HDCPEnable");

//#ifdef USE_HDCP

    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
       ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    if (Id != eTDAL_HDMI_SOURCE_ID_0)
    {
       ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
    }

    MS_U8 MSThdcpKeytmp[HDCP_KEY_SIZE] ={0,};


    MS_U16 u16TxHDCPKeySize = HDCP_KEY_SIZE;

#if 0 //use internal hdcp key instead of open file

#if 0
    //update the hdcp key from flash, customer should get valid hdcp key from hhdmi association, and then use MsGenKey.exe to generate mstar required format
    if(!MDrv_SERFLASH_Read(BASEADDRESS_HDMI_hdcp_KEY_IN_FLASH, u16TxHDCPKeySize, MSThdcpKeytmp))
    {
        HB_printf("MDrv_SERFLASH_Read hdcp key failed !! \n");
        return FLASE;
    }
#else
    //just read HDCP key from file
    int fd = open(HDCP_KEY_FILE, O_RDONLY);
    if (fd < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Can't open key file: %s\n", strerror(errno)));
        ReturnCode =  eTDAL_HDMI_NOT_AVAILABLE_ERROR;
    }
    else
    {
        if (read(fd, MSThdcpKeytmp, HDCP_KEY_SIZE) != HDCP_KEY_SIZE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Can't read %d bytes from key file\n", HDCP_KEY_SIZE));
            ReturnCode =  eTDAL_HDMI_NOT_AVAILABLE_ERROR;
        }
        close(fd);
    }
#endif

#endif
    if (ReturnCode != eTDAL_HDMI_NO_ERROR)
    {
         return(ReturnCode);
    }

    if (E_MSAPI_XC_OK != msAPI_XC_SetHDMITxHDCPKey(MSThdcpKeytmp, u16TxHDCPKeySize))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "msAPI_XC_SetHDMITxHDCPKey failed\n"));
        ReturnCode =  eTDAL_HDMI_NOT_AVAILABLE_ERROR;
    }
    else
    {
        //open the HCP authentication
        MApi_HDMITx_SetHDCPOnOff(TRUE);
        MApi_HDMITx_HDCP_StartAuth(TRUE);
        MApi_HDMITx_ForceHDMIOutputMode(TRUE, HDMITX_HDMI_HDCP);
        MApi_HDMITx_Exhibit();
        //HDCP output behavior control
        MApi_HDMITx_UnHDCPRxControl(E_UNHDCPRX_BLUE_SCREEN);
        MApi_HDMITx_HDCPRxFailControl(E_HDCPRXFail_BLUE_SCREEN);
        //MApi_HDMITx_SetDbgLevel(0xFF);
        //MsOS_DelayTask(10000);
    }

    TDAL_HDMI_HDCP_State = TRUE;
//#endif

   return(ReturnCode);
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
    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_HDMI_HDCPDisable");

//#ifdef USE_HDCP

    if (bTDAL_HDMI_AlreadyInitialized == FALSE)
    {
        ReturnCode = eTDAL_HDMI_NOT_INIT_ERROR;
    }
    if (Id != eTDAL_HDMI_SOURCE_ID_0)
    {
        ReturnCode = eTDAL_HDMI_BAD_PARAMETER_ERROR;
    }

    MApi_HDMITx_SetHDCPOnOff(FALSE);

    TDAL_HDMI_HDCP_State = FALSE;

//#endif
    return (ReturnCode);
}


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

    tTDAL_HDMI_Error   ReturnCode = eTDAL_HDMI_NO_ERROR;

    mTBOX_FCT_ENTER("TDAL_HDMI_HDCPParamSet");

    //TODO: it is not implemented now.
    //we need to know format of HDCP key from MStart to implement it.
    //This function is not used from highlevel application.
    return eTDAL_HDMI_NOT_AVAILABLE_ERROR;

    return (ReturnCode);
}



/***********************************************************************
* Function Name : TDAL_HDMIi_EventsTask
*
* Description : called by HDMI callack when HDMI state change
*
* Side effects :
*
* Comment :
*
**********************************************************************/
LOCAL   void   TDAL_HDMIi_EventsTask(MS_U32 argc, void  *arg)
{
    tTDAL_HDMI_Event    EvtMsg;
    MS_BOOL  Ret_GetRxStatus = FALSE;
    MS_BOOL  Check_GetRxStatus = FALSE;
    HDMITX_HDCP_STATUS Ret_GetHDCPStatus = E_HDCP_DISABLE;
    HDMITX_HDCP_STATUS Check_GetHDCPStatus = E_HDCP_DISABLE;
    HDMITX_INT_HDCP_STATUS Ret_GetINTHDCPStatus = HDMITX_INT_HDCP_DISABLE;
    HDMITX_INT_HDCP_STATUS Check_GetINTHDCPStatus = HDMITX_INT_HDCP_DISABLE;

    mTBOX_FCT_ENTER_T("TDAL_HDMIi_EventsTask");

#ifdef   TDAL_HDMI_USE
    while(TDAL_HDMI_EvtTaskStop == FALSE)
    {
        //MS_BOOL MApi_HDMITx_GetRxStatus (void)
        //Get the connection status the HDMI receiver.
        Check_GetRxStatus = MApi_HDMITx_GetRxStatus();

        if (Ret_GetRxStatus != Check_GetRxStatus)
        {
            Ret_GetRxStatus = Check_GetRxStatus;
            if (Ret_GetRxStatus)
            {
                EvtMsg = eTDAL_HDMI_EVENT_RECEIVER_CONNECTED;
            }
            else
            {
                EvtMsg = eTDAL_HDMI_EVENT_NO_RECEIVER;
            }
            if (TDAL_HDMIi_NotifyFct != NULL)
            {
                TDAL_HDMIi_NotifyFct(eTDAL_HDMI_SOURCE_ID_0, EvtMsg);
            }
            isVideoModesParsed = FALSE;
        }

        Check_GetHDCPStatus = MApi_HDMITx_GetHDCPStatus();
        //HDMITX_HDCP_STATUS MApi_HDMITx_GetHDCPStatus (void)
        //Get the HDCP authentification status.
        if (Ret_GetHDCPStatus != Check_GetHDCPStatus)
        {
            Ret_GetHDCPStatus = Check_GetHDCPStatus;
            switch (Check_GetHDCPStatus)
            {
                case E_HDCP_DISABLE:
                    EvtMsg = eTDAL_HDMI_EVENT_RECEIVER_NOT_HDCP; //eTDAL_HDMI_EVENT_NO_ENCRYPTION
                    break;
                case E_HDCP_FAIL:
                    EvtMsg = eTDAL_HDMI_EVENT_AUTHENTICATION_FAILED;
                    break;
                case E_HDCP_PASS:
                    EvtMsg = eTDAL_HDMI_EVENT_AUTHENTICATION_SUCCEED;
                    break;
            }
            if (TDAL_HDMIi_NotifyFct != NULL)
            {
                TDAL_HDMIi_NotifyFct(eTDAL_HDMI_SOURCE_ID_0, EvtMsg);
            }
        }

        Check_GetINTHDCPStatus = MApi_HDMITx_GetINTHDCPStatus();

        if (Ret_GetINTHDCPStatus != Check_GetINTHDCPStatus)
        {
            Ret_GetINTHDCPStatus = Check_GetINTHDCPStatus;
            switch (Check_GetINTHDCPStatus)
            {
                case HDMITX_INT_HDCP_DISABLE:
                    EvtMsg = eTDAL_HDMI_EVENT_NO_ENCRYPTION;
                    break;
                case HDMITX_INT_HDCP_FAIL:
                    EvtMsg = eTDAL_HDMI_EVENT_AUTHENTICATION_FAILED;
                    break;
                case HDMITX_INT_HDCP_PASS:
                    EvtMsg = eTDAL_HDMI_EVENT_AUTHENTICATION_SUCCEED;
                    break;
                case HDMITX_INT_HDCP_PROCESS:
                    EvtMsg = eTDAL_HDMI_EVENT_AUTHENTICATION_IN_PROGRESS;
                    break;
            }

            //printf("\e[31m=== rtrk DS test ===[%s][%d] ****** Call TDAL_HDMIi_NotifyFct *****\n\e[0m",__FUNCTION__,__LINE__);
            if (TDAL_HDMIi_NotifyFct != NULL)
            {
                TDAL_HDMIi_NotifyFct(eTDAL_HDMI_SOURCE_ID_0, EvtMsg);
            }
        }

        MsOS_DelayTask(1000);
    }

    TDAL_HDMI_EvtTaskFinished = TRUE;
    //STOS_TaskExit(arg);
#if   defined(   ST_OSLINUX   )
    return;
#else
    //task_exit(0);
#endif
#endif
    return;
}




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
