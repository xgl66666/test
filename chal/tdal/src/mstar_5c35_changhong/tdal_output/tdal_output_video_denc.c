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
*           video denc outputs.
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
#include "tdal_vbi.h"

#include "MsTypes.h"
#include "drvTVEncoder.h"
#include "apiACP.h"
#include "apiVDEC.h"
#include "apiDAC.h"
#include "MsMemory.h"

//MApi
#include "xc/msAPI_VE.h"
//#include "xc/msAPI_XC.h"
#include "apiPNL.h"


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

typedef enum
{
    MS_WSS_4x3_FULL                    = 0x08,
    MS_WSS_14x9_LETTERBOX_CENTER       = 0x01,
    MS_WSS_14x9_LETTERBOX_TOP          = 0x02,
    MS_WSS_16x9_LETTERBOX_CENTER       = 0x0B,
    MS_WSS_16x9_LETTERBOX_TOP          = 0x04,
    MS_WSS_ABOVE16x9_LETTERBOX_CENTER  = 0x0D,
    MS_WSS_14x9_FULL_CENTER            = 0x0E,
    MS_WSS_16x9_ANAMORPHIC             = 0x07,

    MS_WSS_625_COPYRIGHT               = 0x1000,
    MS_WSS_625_COPY_PROTECTION         = 0x2000
}MS_VE_WSS_TYPE;

/*
 * This Enum type was defined base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
 * - PAL denotes signal standards PAL-B, -D, -G, -H, -I, -K, -L
 * - NTSC TYPE 1/2/3 include NTSC-M, NTSC-J, PAL-M and NTSC-443
 * - PAL TYPE 1/2/3 include PAL, PAL-N, PAL-Nc and SECAM
 */
typedef enum
{
    MS_ACP_CVBS_NTSC_TYPE1 = 0,         //NTSC, AGC only (Table2)
    MS_ACP_CVBS_NTSC_TYPE2,             //NTSC, AGC + 2-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TYPE3,             //NTSC, AGC + 4-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TEST_N01,          //NTSC, RGB Copy Protect on (N01) (Table2)
    MS_ACP_CVBS_NTSC_TEST_N02,          //NTSC, RGB Copy Protect on (N02) (Table2)
    MS_ACP_CVBS_PAL_TYPE1 = 0x10,       //PAL, Type1, BPP = 0x40 (Table2)
    MS_ACP_CVBS_PAL_TYPE2,              //PAL, Type2, BPP = 0x60 (Table2)
    MS_ACP_CVBS_PAL_TYPE3,              //PAL, Type3, BPP = 0x50 (Table2)
    MS_ACP_CVBS_PAL_TEST_P01,           //PAL, RGB Copy Protect on (P01) (Table2)
    MS_ACP_CVBS_PAL_TEST_P02,           //PAL, RGB Copy Protect on (P02) (Table2)
    MS_ACP_COMPONENT_480I_TYPE2 = 0x20, //YPbPr, NTSC-Type2:700/300 Levels (Table4)
    MS_ACP_COMPONENT_480I_TYPE3,        //YPbPr, NTSC-Type3:714/286 Levels (Table4)
    MS_ACP_COMPONENT_480P_EIA_7701,     //YPbPr, 525P EIA-770.1 (Table6)
    MS_ACP_COMPONENT_480P_EIA_7702,     //YPbPr, 525P EIA-770.2 (Table6)
    MS_ACP_COMPONENT_576I_TEST_P01,     //YPbPr, Test P01 (Table8)
    MS_ACP_COMPONENT_576I_TEST_P02,     //YPbPr, Test P02 (Table8)
    MS_ACP_COMPONENT_576P,              //YPbPr, 625P (Table9)
    MS_ACP_SVIDEO_NTSC_TYPE2 = 0x40,    //S-video, NTSC-Type2 (Table4)
    MS_ACP_SVIDEO_NTSC_TYPE3,           //S-video, NTSC-Type3 (Table4)
    MS_ACP_SVIDEO_PAL_TEST_P01,         //S-video, PAL Test P01 (Table8)
    MS_ACP_SVIDEO_PAL_TEST_P02,         //S-video, PAL Test P02 (Table8)
    MS_ACP_TYPE_MAX,
} MS_ACP_TYPE;

/********************************************************/
/*   Global   Variables   (GLOBAL/IMPORT)   */
/********************************************************/
/********************************************************/
/*   Local   Module   Variables   (MODULE)   */
/********************************************************/

/********************************************************/
/*   Local   File   Variables   (LOCAL)   */
/********************************************************/

LOCAL bool isVEInitialized = FALSE;

LOCAL tTDAL_VBI_WssAspectRatio TDAL_OUTPUTi_VideoDencWssAspectRatio, * p_TDAL_OUTPUTi_VideoDencWssAspectRatio = NULL;
LOCAL tTDAL_OUTPUT_VideoDencCGMSACopyProtection TDAL_OUTPUTi_VideoDencWssCGMSACopyProt = eTDAL_OUTPUT_VIDEO_DENC_GMS_A_COPY_NEVER;
LOCAL tTDAL_OUTPUT_VideoDencCGMSACopyProtection * p_TDAL_OUTPUTi_VideoDencWssCGMSACopyProt = &TDAL_OUTPUTi_VideoDencWssCGMSACopyProt;


/********************************************************/
/*   Local   Functions   Declarations   (LOCAL)   */
/********************************************************/
LOCAL tTDAL_OUTPUT_Error TDAL_OUTPUTm_VideoDencSetWss(tTDAL_VBI_WssAspectRatio * wssAspectRation, tTDAL_OUTPUT_VideoDencCGMSACopyProtection * wssCgmsACopyProtection);
LOCAL bool TDAL_OUTPUTi_SetMacrovision(bool on);
/********************************************************/
/*   Functions   Definitions   (LOCAL/GLOBAL)   */
/********************************************************/


void TDAL_OUTPUTm_VEInit()
{
    if (isVEInitialized)
        return;

    // VE init
    MSAPI_VE_INITDATA msAPI_VE_InitData;

    memset(&msAPI_VE_InitData, 0, sizeof(MSAPI_VE_INITDATA));
    msAPI_VE_InitData.u32VEMemAddress   = VE_FRAME_BUF_ADR;
    msAPI_VE_InitData.u32VEMemSize      = VE_FRAME_BUF_LEN;
    msAPI_VE_InitData.eOutputVideoStd   = MS_VE_PAL;
    msAPI_VE_InitData.eInputSrcType     = MS_VE_SRC_SCALER;
    msAPI_VE_InitData.eOutputDstType    = MS_VE_DEST_CVBS;

    msAPI_VE_InitData.bEnableDacSD = TRUE;
    msAPI_VE_InitData.bBootLogoEnable   = FALSE;

    if(FALSE == msAPI_VE_Init(msAPI_VE_InitData))
    {
        printf("msAPI_VE_Init resturn FALSE\n");
    }

    //turn off SD OSD at initial step and turn on it after GOP is initialized
    MDrv_VE_SetOSD(FALSE);

    MS_Switch_VE_Dest_Info DestInfo;
    DestInfo.OutputDstType = MS_VE_DEST_CVBS;
    MDrv_VE_SwitchOuputDest(&DestInfo);

    isVEInitialized = TRUE;
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencCapabilityGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoDencCapabilityGet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_VideoDencCapability   *pstCapability)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool   desc_found = FALSE;
    uint32_t   i;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencCapabilityGet");
    
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
            case   eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
            {
                *pstCapability = TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->stCapability;
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

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencStandardSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoDencStandardSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_VideoSDStandard   Std)
{
    tTDAL_OUTPUT_Error      OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool                    desc_found = FALSE;
    uint32_t                i,   vmix,vtg;
    MS_BOOL                 msAPI_err = 0;
    VE_DrvStatus            MSCurrentVeStatus;
    VE_DrvStatus            MSNewCurrentVeStatus;
    VE_Result               msDRV_err = E_VE_FAIL;
    MS_VE_WINDOW_TYPE       dispWin = { 0, 0, 0, 0};
    MS_VE_WINDOW_TYPE       xc_dispWin = { 0, 0, 0, 0};
    bool                    bStandardChanged = FALSE;
    uint32_t                eResType; //E_MSAPI_XC_OUTPUT_TIMING_TYPE eResType;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencStandardSet");
    
    mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_OUTPUT_VideoDencStandardSet] videoStandard = %d\n", Std));
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
    
    msDRV_err = MDrv_VE_GetStatus(&MSCurrentVeStatus);
    mTBOX_ASSERT(msDRV_err == E_VE_OK);
    
    if (desc_found == TRUE)
    {
        switch   (Id)
        {
            case eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
            {
                switch (Std)
                {
                    case eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG:
                    {
                        MSNewCurrentVeStatus.VideoSystem = MS_VE_PAL;
                        eResType = 2; //E_MSAPI_XC_RES_720x576I_50Hz
                        //eResType = 3; //E_MSAPI_XC_RES_720x576P_50Hz;
                        break;
                    }
                    case eTDAL_OUTPUT_VIDEO_SD_STD_PAL_I:
                    {
                        MSNewCurrentVeStatus.VideoSystem = MS_VE_PAL;
                        eResType = 2; //E_MSAPI_XC_RES_720x576I_50Hz
                        //eResType = 3; //E_MSAPI_XC_RES_720x576P_50Hz;
                        break;
                    }
                    case eTDAL_OUTPUT_VIDEO_SD_STD_PAL_M:
                    {
                        MSNewCurrentVeStatus.VideoSystem = MS_VE_PAL_M;
                        eResType = 0; //E_MSAPI_XC_RES_720x480I_60Hz;
                        //eResType = 1; //E_MSAPI_XC_RES_720x480P_60Hz;
                        break;
                    }
                    case eTDAL_OUTPUT_VIDEO_SD_STD_PAL_N:
                    {
                        MSNewCurrentVeStatus.VideoSystem = MS_VE_PAL_N;
                        eResType = 2; //E_MSAPI_XC_RES_720x576I_50Hz
                        //eResType = 3; //E_MSAPI_XC_RES_720x576P_50Hz;
                        break;
                    }
                    case eTDAL_OUTPUT_VIDEO_SD_STD_SECAM :
                    {
                        MSNewCurrentVeStatus.VideoSystem = MS_VE_SECAM;
                        eResType = 2; //E_MSAPI_XC_RES_720x576I_50Hz
                        //eResType = 3; //E_MSAPI_XC_RES_720x576P_50Hz;
                        break;
                    }
                    case eTDAL_OUTPUT_VIDEO_SD_STD_NTSC_M :
                    {
                        MSNewCurrentVeStatus.VideoSystem = MS_VE_NTSC; /* should be checked( MS_VE_NTSC, MS_VE_NTSC_443, MS_VE_NTSC_J)*/
                        eResType = 0; //E_MSAPI_XC_RES_720x480I_60Hz;
                        //eResType = 1; //E_MSAPI_XC_RES_720x480P_60Hz;
                        break;
                    }
                    default:
                    {
                        OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                        break;
                    }
                }
                
                if (TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->Standard == NULL || 
                        MSNewCurrentVeStatus.VideoSystem != MSCurrentVeStatus.VideoSystem)
                {
                    bStandardChanged = TRUE;
                }

                if (TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->Standard != Std)
                {  
                    if (!bStandardChanged)
                    {
                        mTBOX_TRACE((kTBOX_NIV_5,"Something messy in Output Standard Set bookkeeping\n"));
                    }
                }
                else
                {
                    if (bStandardChanged)
                    {
                        mTBOX_TRACE((kTBOX_NIV_5,"Something messy in Output Standard Set bookkeeping\n"));
                    }
                }
                TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->Standard = Std;
                break;
            }
            default:
            {
                OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                break;
            }
        }
        
        if (bStandardChanged)
        {
            bool VE_Enabled = false;
            DAC_ApiStatus dacStatus;

            if (msAPI_VE_CheckInitStatus() == FALSE)
            {
                mTBOX_TRACE((kTBOX_NIV_5,"VE is not initialized\n"));
            }
            
            if (MApi_DAC_GetStatus(&dacStatus))
            {
                VE_Enabled = dacStatus.bDac_VE_enabled;
                if (VE_Enabled)
                {
                    msAPI_err = msAPI_VE_EnableMiscOutput(DISABLE);
                    mTBOX_ASSERT(msAPI_err == TRUE);

                    msAPI_err = msAPI_XC_EnableMiscOutput(DISABLE);
                    mTBOX_ASSERT(msAPI_err == TRUE);
                }
            }

            /*************************************/
            /*   Set   the   requested   encoding   mode   */
            /*************************************/
            /* TBD */

            switch(MSNewCurrentVeStatus.VideoSystem)
            {
                case MS_VE_NTSC:
                case MS_VE_NTSC_443:
                case MS_VE_NTSC_J:
                case MS_VE_PAL_M:
                    dispWin.height = 480;
                    dispWin.width = 720;
                    xc_dispWin.x = 0 + g_IPanel.HStart();
                    xc_dispWin.y = 0 + g_IPanel.VStart();
                    xc_dispWin.height = 480;
                    xc_dispWin.width = 720;
                    break;
                case MS_VE_SECAM:
                //case MS_VE_PAL_M:
                case MS_VE_PAL_N:
                case MS_VE_PAL_NC:
                case MS_VE_PAL:
                    dispWin.height = 576;
                    dispWin.width = 720;
                    xc_dispWin.x = 0 + g_IPanel.HStart();
                    xc_dispWin.y = 0 + g_IPanel.VStart();
                    xc_dispWin.height = 576;
                    xc_dispWin.width = 720;
                    break;
                default:
                    dispWin.height = 576;
                    dispWin.width = 720;
                    xc_dispWin.x = 0 + g_IPanel.HStart();
                    xc_dispWin.y = 0 + g_IPanel.VStart();
                    xc_dispWin.height = 576;
                    xc_dispWin.width = 720;
                    mTBOX_ASSERT(FALSE);
                    break;
            }

            //Reset XC Timing
            msAPI_XC_ChangeOutputResolution(eResType);

            //Set Display Window
            MApi_XC_SetDispWinToReg(&xc_dispWin, 0/*MAIN_WINDOW*/);

            msAPI_err = msAPI_XC_EnableMiscOutput(ENABLE);
            mTBOX_ASSERT(msAPI_err == TRUE);

            msAPI_err = msAPI_VE_SetOutputVideoStd(MSNewCurrentVeStatus.VideoSystem);
            mTBOX_ASSERT(msAPI_err == TRUE);
            msAPI_err = msAPI_VE_SetMode();
            mTBOX_ASSERT(msAPI_err == TRUE);

            mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_OUTPUT_VideoDencStandardSet] msAPI_VE_SetWin(%d, %d, %d, %d) = %d\n", dispWin.x, dispWin.y, dispWin.width, dispWin.height, Std));
            msAPI_err = msAPI_VE_SetWin(NULL, &dispWin);
            mTBOX_ASSERT(msAPI_err == TRUE);
            if (VE_Enabled)
            {
                msAPI_err = msAPI_VE_EnableMiscOutput(ENABLE);
                mTBOX_ASSERT(msAPI_err == TRUE);

                //Set VDEC FRC
                msAPI_err = msAPI_XC_DTV_SetMode();
                mTBOX_ASSERT(msAPI_err == TRUE);
                //Set Scaler(HD PATH)
                msAPI_XC_SetWin(NULL, NULL, NULL, 0/*MAIN_WINDOW*/);

                msAPI_err = msAPI_VE_SetMode();
                mTBOX_ASSERT(msAPI_err == TRUE);

                //Wait at least 4 V-syncs(VE setting time + wait 2 VSync) for SCALER to fill frame buffer
                MApi_XC_WaitOutputVSync(2, 100, 0/*MAIN_WINDOW*/);
            }
            TDAL_OUTPUTi_SetMacrovision(TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->IsMacrovisionActive);
        }

    }
    else
    {
        OutErrorCode = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
    }

    mTBOX_RETURN(OutErrorCode);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencFormatSet
*
* Description :
*
* Side effects :
*
* Comment :   Because we can change the output mode only at driver's
*            initialization : we only authorized RGB mode.
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoDencFormatSet(tTDAL_OUTPUT_OutputId   Id,   uint32_t   FormatMask)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t      i;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencFormatSet");
    
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

    if (desc_found == TRUE)
    {
        switch (Id)
        {
            case eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
            {
                if (((FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC) && (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS)) ||
                   ((FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC) && (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB))   ||
                   ((FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC) && (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV))   ||
                   ((FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB) && (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV)))
                {
                    /*   scheme   not   available   */
                    OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                }
                else
                {
                    if ((!(TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS))   &&
                       (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_CVBS))
                    {
                        /*printf("VideoFormatSet   =>   TO   BE   DONE   With   the   scart   ??");*/
                        /*   scheme   not   available   */
                        OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                    }

                    if ((!(TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB))   &&
                       (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_RGB))
                    {
                        /*printf("VideoFormatSet   =>   TO   BE   DONE   With   the   scart   ??");*/
                        /*nothing   to   do*/
                    }
        
                    if ((!(TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV))   &&
                    (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YUV))
                    {
                        /*printf("VideoFormatSet   =>   TO   BE   DONE   With   the   scart   ??");*/
                        /*   scheme   not   available   */
                        OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                    }

                    if ((!(TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC))   &&
                            (FormatMask & eTDAL_OUTPUT_VIDEO_DENC_FORMAT_YC))
                    {
                        /*printf("VideoFormatSet   =>   TO   BE   DONE   With   the   scart   ??");*/
                        /*   scheme   not   available   */
                        OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                    }
                    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->FormatMask = FormatMask;
                }
                break;
            }
            case   eTDAL_OUTPUT_VIDEO_DENC_ID_1 :
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

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencColorBarControl
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoDencColorBarControl(tTDAL_OUTPUT_OutputId   Id,   bool   On)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t      i;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencColorBarControl");
    
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
            case   eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
            {
                if   (TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->IsColorBarActive   !=   On)
                {
                    if   (On == TRUE)
                    {
                    }
                    else
                    {
                    }
                    TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->IsColorBarActive = On;
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
    mTBOX_RETURN(OutErrorCode);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencBCSParamsSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_VideoDencBCSParamsSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_VideoDencBCSParams   *pstBcs)
{
    tTDAL_OUTPUT_Error   Error = eTDAL_OUTPUT_NO_ERROR;
    
    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencBCSParamsSet");
    
    /*   not   supported   */
    mTBOX_RETURN(Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencGSMCGMSACopyProtectionSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencGSMCACopyProtectionSet(tTDAL_OUTPUT_OutputId Id, tTDAL_OUTPUT_VideoDencCGMSACopyProtection cgmsaCopyProtection)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t      i;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencColorBarControl");

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

    if (desc_found)
    {
        switch   (Id)
         {
             case   eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
             {
                 TDAL_OUTPUTi_VideoDencWssCGMSACopyProt = cgmsaCopyProtection;
                 p_TDAL_OUTPUTi_VideoDencWssCGMSACopyProt = &TDAL_OUTPUTi_VideoDencWssCGMSACopyProt;
                 OutErrorCode = TDAL_OUTPUTm_VideoDencSetWss(p_TDAL_OUTPUTi_VideoDencWssAspectRatio, p_TDAL_OUTPUTi_VideoDencWssCGMSACopyProt);
             }
             break;
             default:
             {
                 OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
             }
             break;
         }
    }
    else
    {
        OutErrorCode = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
    }

    mTBOX_RETURN(OutErrorCode);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_VideoDencMacrovisionCopyProtectionSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL tTDAL_OUTPUT_Error TDAL_OUTPUT_VideoDencMacrovisionCopyProtectionSet(tTDAL_OUTPUT_OutputId Id, bool On)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t      i;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_VideoDencColorBarControl");

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

    if (desc_found)
    {
        switch   (Id)
         {
             case   eTDAL_OUTPUT_VIDEO_DENC_ID_0 :
             {
                 if (TDAL_OUTPUTi_SetMacrovision(On))
                 {
                     TDAL_OUTPUTi_DescriptorTable[i].pstVideoDencDesc->IsMacrovisionActive = On;
                 }
                 else
                 {
                     OutErrorCode = eTDAL_OUTPUT_DRIVER_ERROR;
                 }
             }
             break;
             default:
             {
                 OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
             }
             break;
         }
    }
    else
    {
        OutErrorCode = eTDAL_OUTPUT_BAD_PARAMETER_ERROR;
    }

    mTBOX_RETURN(OutErrorCode);
}

LOCAL bool TDAL_OUTPUTi_SetMacrovision(bool on)
{
    VE_DrvStatus MSCurrentVeStatus;
    VE_Result   msDRV_err;
    bool result = true;
    MS_BOOL bEnable =  (MS_BOOL) on;
    MS_ACP_TYPE eACPType;
    ACP_Result eRet = E_ACP_FAIL;

    mTBOX_FCT_ENTER(TDAL_OUTPUTi_SetMacrovision);

    msDRV_err = MDrv_VE_GetStatus(&MSCurrentVeStatus);

    if (msDRV_err != E_VE_OK)
    {
        result = false;
    }

    if (result)
    {
        switch (MSCurrentVeStatus.VideoSystem)
        {
        case MS_VE_NTSC:     ///< NTSC
        case MS_VE_NTSC_443: ///< NTSC443
        case MS_VE_NTSC_J:   ///< NTSC_J
            eACPType = MS_ACP_CVBS_NTSC_TYPE2;
            break;
        case MS_VE_PAL_M:    ///< PAL_M
        case MS_VE_PAL_N:    ///< PAL_N
        case MS_VE_PAL_NC:   ///< PAL_Nc
        case MS_VE_PAL:      ///< PAL_B
        case MS_VE_SECAM:    ///< SECAM
        default:
            eACPType = MS_ACP_CVBS_PAL_TYPE2;
            break;
        }

        eRet = MApi_ACP_SetProtection(bEnable, FALSE, eACPType);

        if( eRet != E_ACP_OK )
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ACP set Macrovision Failed, return value = %d \n", eRet));
            return FALSE;
        }
    }

    mTBOX_RETURN(result);

}

GLOBAL tTDAL_VBI_ErrorCode TDAL_OUTPUTm_VideoDencSetWssAspectRatio(tTDAL_VBI_WssAspectRatio * wssAspectRation)
{
    tTDAL_OUTPUT_Error errorCode;

    if (wssAspectRation == NULL)
    {
        p_TDAL_OUTPUTi_VideoDencWssAspectRatio = NULL;
    }
    else
    {
        TDAL_OUTPUTi_VideoDencWssAspectRatio = *wssAspectRation;
        p_TDAL_OUTPUTi_VideoDencWssAspectRatio = &TDAL_OUTPUTi_VideoDencWssAspectRatio;
    }

    errorCode = TDAL_OUTPUTm_VideoDencSetWss(p_TDAL_OUTPUTi_VideoDencWssAspectRatio, p_TDAL_OUTPUTi_VideoDencWssCGMSACopyProt);
    switch(errorCode)
    {
    case eTDAL_OUTPUT_NO_ERROR:
        return eTDAL_VBI_NO_ERROR;
    case eTDAL_OUTPUT_BAD_PARAMETER_ERROR:
        return eTDAL_VBI_BAD_PARAMETER;
    default:
        return eTDAL_VBI_ERROR;
    }
}

LOCAL tTDAL_OUTPUT_Error TDAL_OUTPUTm_VideoDencSetWss(tTDAL_VBI_WssAspectRatio * wssAspectRation, tTDAL_OUTPUT_VideoDencCGMSACopyProtection * wssCgmsACopyProtection)
{
    MS_U16 u16WSSAspectRatioData, u16WSSData, u16CgmsACopyProtection;
    tTDAL_OUTPUT_Error returnValue = eTDAL_OUTPUT_NO_ERROR;

    mTBOX_FCT_ENTER(TDAL_OUTPUTm_VideoDencSetWss);

    u16WSSAspectRatioData = 0;
    u16WSSData = 0;
    u16CgmsACopyProtection = 0;

    if (wssAspectRation != NULL)
    {
        switch (*wssAspectRation)
        {
            case eTDAL_VBI_4_3_FULL_FORMAT:
                u16WSSAspectRatioData = MS_WSS_4x3_FULL;
                break;
            case eTDAL_VBI_14_9_LETTERBOX_CENTER:
                u16WSSAspectRatioData =  MS_WSS_14x9_LETTERBOX_CENTER;
                break;
            case eTDAL_VBI_14_9_LETTERBOX_TOP:
                u16WSSAspectRatioData =  MS_WSS_14x9_LETTERBOX_TOP;
                break;
            case eTDAL_VBI_16_9_LETTERBOX_CENTER:
                u16WSSAspectRatioData =  MS_WSS_16x9_LETTERBOX_CENTER;
                break;
            case eTDAL_VBI_16_9_LETTERBOX_TOP:
                u16WSSAspectRatioData =  MS_WSS_16x9_LETTERBOX_TOP;
                break;
            case eTDAL_VBI_HIGHER_THAN_16_9_LETTERBOX_CENTER:
                u16WSSAspectRatioData =  MS_WSS_ABOVE16x9_LETTERBOX_CENTER;
                break;
            case eTDAL_VBI_14_9_FULL_FORMAT:
                u16WSSAspectRatioData =  MS_WSS_14x9_FULL_CENTER;
                break;
            case eTDAL_VBI_16_9_FULL_FORMAT:
                u16WSSAspectRatioData =  MS_WSS_16x9_ANAMORPHIC;
                break;
            default:
                u16WSSAspectRatioData = 0;
                break;
        }
    }

    if (wssCgmsACopyProtection != NULL)
    {
        switch (*wssCgmsACopyProtection)
        {
        case eTDAL_OUTPUT_VIDEO_DENC_CGMS_A_COPY_NO_MORE:
        {
            // CGMS-A: 01
            u16CgmsACopyProtection = (u16CgmsACopyProtection
                    & ~(MS_WSS_625_COPYRIGHT)) | MS_WSS_625_COPY_PROTECTION;
        }
            break;
        case eTDAL_OUTPUT_VIDEO_DENC_CGMS_A_COPY_ONCE:
        {
            // CGMS-A: 10
            u16CgmsACopyProtection = (u16CgmsACopyProtection | MS_WSS_625_COPYRIGHT)
                    & ~(MS_WSS_625_COPY_PROTECTION);
        }
            break;
        case eTDAL_OUTPUT_VIDEO_DENC_GMS_A_COPY_NEVER:
        {
            // CGMS-A: 11
            u16CgmsACopyProtection = u16CgmsACopyProtection | MS_WSS_625_COPYRIGHT
                    | MS_WSS_625_COPY_PROTECTION;
        }
            break;
        case eTDAL_OUTPUT_VIDEO_DENC_CGMS_A_COPY_FREELY:
        default:
        {
            // CGMS-A: 00
            u16CgmsACopyProtection = u16CgmsACopyProtection
                    & ~(MS_WSS_625_COPYRIGHT) & ~(MS_WSS_625_COPY_PROTECTION);
        }
            break;
        }
    }

    if (returnValue == eTDAL_OUTPUT_NO_ERROR)
    {
        u16WSSData = u16CgmsACopyProtection | u16WSSAspectRatioData;
        if (u16WSSData != 0)
        {
            MDrv_VE_SetWSSData(ENABLE, u16WSSData);
        }
        else
        {
            MDrv_VE_SetWSSData(DISABLE, u16WSSData);
        }
    }

    mTBOX_RETURN(returnValue);
}

