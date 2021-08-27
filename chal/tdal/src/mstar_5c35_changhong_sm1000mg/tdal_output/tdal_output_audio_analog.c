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
#include "tdal_output_priv.h"

#include "MsCommon.h"
#include "apiAUDIO.h"
#include "MsMemory.h"


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
LOCAL   uint32_t   TDAL_OUTPUT_ANA_volume = 0;
LOCAL   uint32_t   TDAL_OUTPUT_ANA_Delay = 0;
LOCAL   uint32_t   TDAL_OUTPUT_ANA_mixLevel = 100;
LOCAL   uint8_t   TDAL_OUTPUT_ANA_Adjust = 100;


LOCAL MS_U16 TvVolumeTable[] =
{ //    1     2       3        4      5       6        7      8        9       10
                                                                                                                    0x7F00, //  00
    0x4700, 0x4400, 0x4100, 0x3E00, 0x3C00, 0x3A00, 0x3800, 0x3600, 0x3400, 0x3200, //  10
    0x3000, 0x2E00, 0x2D00, 0x2C00, 0x2B00, 0x2A00, 0x2900, 0x2800, 0x2700, 0x2600, //  20
    0x2500, 0x2400, 0x2300, 0x2200, 0x2100, 0x2000, 0x1F00, 0x1E40, 0x1E00, 0x1D40, //  30
    0x1D00, 0x1C40, 0x1C00, 0x1B40, 0x1B00, 0x1A40, 0x1A00, 0x1940, 0x1900, 0x1840, //  40
    0x1800, 0x1740, 0x1700, 0x1640, 0x1600, 0x1540, 0x1520, 0x1500, 0x1460, 0x1440, //  50
    0x1420, 0x1400, 0x1360, 0x1340, 0x1320, 0x1300, 0x1260, 0x1240, 0x1220, 0x1200, //  60
    0x1160, 0x1140, 0x1120, 0x1100, 0x1060, 0x1040, 0x1020, 0x1000, 0x0F70, 0x0F60, //  70
    0x0F50, 0x0F40, 0x0F30, 0x0F20, 0x0F10, 0x0F00, 0x0E70, 0x0E60, 0x0E50, 0x0E40, //  80
    0x0E30, 0x0E20, 0x0E10, 0x0E00, 0x0D70, 0x0D60, 0x0D50, 0x0D40, 0x0D30, 0x0D20, //  90
    0x0D10, 0x0D00, 0x0C70, 0x0C60, 0x0C50, 0x0C40, 0x0C30, 0x0C20, 0x0C10, 0x0C00  //  100
};

/********************************************************/
/*   Local   File   Variables   (LOCAL)   */
/********************************************************/

/********************************************************/
/*   Local   Functions   Declarations   (LOCAL)   */
/********************************************************/

LOCAL tTDAL_OUTPUT_Error p_TDAL_OUTPUTi_Audio_SetVolume(MS_U8 VolumePercent);

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
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioAnaCapabilityGet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_AudioAnaCapability   *pstCapability)
{
    tTDAL_OUTPUT_Error   OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool   desc_found = FALSE;
    uint32_t   i;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_AudioAnaCapabilityGet");

    /*   check   if   not   initialized   */
    if   (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"AudioAnaCapabilityGet:   no   init\n"));
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
            case   eTDAL_OUTPUT_AUDIO_ANA_ID_0   :
            {
                *pstCapability = TDAL_OUTPUTi_DescriptorTable[i].pstAudioAnaDesc->stCapability;
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
* Function Name : TDAL_OUTPUT_AudioAnaSpeakerParamsSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioAnaSpeakerParamsSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_AudioAnaSpeakerParams   *pstParams)
{
   tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
   return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_AudioAnaSpeakerAttenuation   *pstAttenuation)
{
    tTDAL_OUTPUT_Error      OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t        i;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet");

    /*   check   if   not   initialized   */
    if   (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"AudioAnaSpeakerAttenuationSet:   no   init\n"));
        mTBOX_RETURN(eTDAL_OUTPUT_NOT_INIT_ERROR);
    }

    if(pstAttenuation->FrontCenterAttenuation > kTDAL_OUTPUT_AUDIO_VOLUME_MAX)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet:   client   volume   bad   value   [%d]\n",   pstAttenuation->FrontCenterAttenuation));
        mTBOX_RETURN(eTDAL_OUTPUT_BAD_PARAMETER_ERROR);
    }

    mTBOX_TRACE((kTBOX_NIV_1,"TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet:   vol=%d   in   [0   to   %d]\n",   pstAttenuation->FrontCenterAttenuation,   kTDAL_OUTPUT_AUDIO_VOLUME_MAX));

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
            case   eTDAL_OUTPUT_AUDIO_ANA_ID_0   :
            {
                TDAL_OUTPUT_ANA_volume = pstAttenuation->FrontCenterAttenuation;
                TDAL_OUTPUT_ANA_mixLevel = pstAttenuation->MixLevel;

                if (TDAL_OUTPUTi_DescriptorTable[i].IsEnable == TRUE)
                {
                	p_TDAL_OUTPUTi_Audio_SetVolume(TDAL_OUTPUT_ANA_volume);
                }
                else
                {
                	mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_OUTPUT_AUDIO_ANA_ID_0 disabled, id = %d, not calling set volume\n", i));
                }
                break;
            }
            case   eTDAL_OUTPUT_AUDIO_SPDIF_ID_0   :
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
* Function Name : TDAL_OUTPUT_AudioAnaSpeakerAttenuationGet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioAnaSpeakerAttenuationGet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_AudioAnaSpeakerAttenuation   *pstAttenuation)
{
    tTDAL_OUTPUT_Error      OutErrorCode = eTDAL_OUTPUT_NO_ERROR;
    bool        desc_found = FALSE;
    uint32_t        i;

    mTBOX_FCT_ENTER("TDAL_OUTPUT_AudioAnaSpeakerAttenuationGet");

    /*   check   if   not   initialized   */
    if   (TDAL_OUTPUT_AlreadyInitialized == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"AudioAnaSpeakerAttenuationGet:   no   init\n"));
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
            case   eTDAL_OUTPUT_AUDIO_ANA_ID_0   :
            {
                pstAttenuation->FrontRLAttenuation = 0;
                pstAttenuation->SurroundRLAttenuation = 0;
                pstAttenuation->LFEAttenuation = 0;
                pstAttenuation->RearRLAttenuation = 0;
                pstAttenuation->RearCenterAttenuation = 0;
                pstAttenuation->MixLevel = 0;
                pstAttenuation->FrontCenterAttenuation = TDAL_OUTPUT_ANA_volume;
                pstAttenuation->MixLevel = TDAL_OUTPUT_ANA_mixLevel;
            }
            break;
            case   eTDAL_OUTPUT_AUDIO_SPDIF_ID_0   :
            /*   No   attenuation   managed   on   SPDIF   */
            default:
                OutErrorCode = eTDAL_OUTPUT_NOT_AVAILABLE_ERROR;
                break;
        }
    }
    mTBOX_RETURN(OutErrorCode);

}

/***********************************************************************
* Function Name : TDAL_OUTPUT_AudioAnaSpeakerDelaySet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioAnaSpeakerDelaySet(tTDAL_OUTPUT_OutputId   Id,   tTDAL_OUTPUT_AudioAnaSpeakerDelay   *pstDelay)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);
}

/***********************************************************************
* Function Name : TDAL_OUTPUT_AudioAnaSpeakerAdjustSet
*
* Description :
*
* Side effects :
*
* Comment :
*
**********************************************************************/
GLOBAL   tTDAL_OUTPUT_Error   TDAL_OUTPUT_AudioAnaSpeakerAdjustSet(tTDAL_OUTPUT_OutputId   Id,   uint8_t   AudioAdjust)
{
    tTDAL_OUTPUT_Error    OUTPUT_Error = eTDAL_OUTPUT_NO_ERROR;
    return (OUTPUT_Error);
}

GLOBAL  tTDAL_OUTPUT_Error TDAL_OUTPUTi_AudioAnaEnable(bool enable)
{
	MS_BOOL mute;

	mTBOX_FCT_ENTER("TDAL_OUTPUTi_AudioAnaEnable");

	mute = enable? FALSE: TRUE;

	mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_OUTPUTi_AudioAnaEnable]   enable = %s\n", enable == true ? "TRUE" : "FALSE"));

	MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, mute);
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, mute);
    MApi_AUDIO_SPDIF_SetMute(mute);
    MApi_AUDIO_HDMI_Tx_SetMute(mute);

#ifdef AUDIO_MUTE_GPIO 
    if (mute)
    {
        AUDIO_MUTE_SET_GPIO_DOWN(AUDIO_MUTE_GPIO);
    }
    else
    {
        AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
    }
#endif

    if (!mute)
    {
		mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_OUTPUTi_AudioAnaEnable] Configuring HDMI audio\n"));
	    int volume;
		/* TODO START:  HDMITx Audio*/
		/* Ivica: Disabled because of crash */
		//MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
		//MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
		//MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
		/* TODO END:    HDMITx Audio*/

		for(volume=0;volume<=TDAL_OUTPUT_ANA_volume;volume+=10)
		{
			// printf("volume: %d\n",volume);
			p_TDAL_OUTPUTi_Audio_SetVolume(volume);
			MsOS_DelayTask(10);
		}
		p_TDAL_OUTPUTi_Audio_SetVolume(TDAL_OUTPUT_ANA_volume);
    }

    mTBOX_RETURN(eTDAL_OUTPUT_NO_ERROR);
}

LOCAL tTDAL_OUTPUT_Error p_TDAL_OUTPUTi_Audio_SetVolume(MS_U8 VolumePercent)
{
	tTDAL_OUTPUT_Error Error = eTDAL_OUTPUT_NO_ERROR;

    MS_U8 value1, value2;
    MS_U16 VolumeValue;

    mTBOX_FCT_ENTER("p_TDAL_OUTPUTi_Audio_SetVolume");

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_OUTPUTi_Audio_SetVolume] Volume = %d\n", (int) VolumePercent));

    if( VolumePercent > kTDAL_OUTPUT_AUDIO_VOLUME_MAX )
    {
        VolumePercent= kTDAL_OUTPUT_AUDIO_VOLUME_MAX;
    }

    if(VolumePercent== 0)
    {
    	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_OUTPUTi_Audio_SetVolume] Muting everything\n"));
    	MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, TRUE);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, TRUE );
        MApi_AUDIO_SPDIF_SetMute(TRUE);
        MApi_AUDIO_HDMI_Tx_SetMute(TRUE);
    }
    else
    {
    	mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_OUTPUTi_Audio_SetVolume] Unmuting everything\n"));
    	MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, FALSE);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, FALSE);
        MApi_AUDIO_SPDIF_SetMute(FALSE);
        MApi_AUDIO_HDMI_Tx_SetMute(FALSE);
    }


//#if(ENABLE_SOUND_NONLINEAR_CURVE)
    //VolumePercent = MApp_NonLinearCalculate(MApp_GetNonLinearCurve(NONLINEAR_CURVE_VOLUME),VolumePercent);
//#endif

    VolumeValue = TvVolumeTable[VolumePercent];
    value1 = (MS_U8)(VolumeValue>>8);
    value2 = (MS_U8)((VolumeValue&0x00FF)>>4);

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_OUTPUTi_Audio_SetVolume] MApi_AUDIO_SetAbsoluteVolume\n"));
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_MAIN_SPEAKER, value1, value2);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_HDMI, value1, value2);

#ifdef AUDIO_MUTE_GPIO
    if (VolumePercent == 0)
    {
        AUDIO_MUTE_SET_GPIO_DOWN(AUDIO_MUTE_GPIO);
    }
    else
    {
        AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
    }
#endif

    mTBOX_RETURN(Error);
}


