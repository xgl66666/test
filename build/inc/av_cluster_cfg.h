/******************************************************************************
 *                           COPYRIGHT 2008 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG AVCLUSTER
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/av_cluster_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _AVCLUSTER_CFG_H_
#define _AVCLUSTER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "av_cluster.h"
#include "av_cluster_video_types.h"
#include "av_cluster_video_api.h"

typedef enum
{
    eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_0,
    eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_1,
    eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_2,
    eAVCLUSTER_CFG_OUTPUT_VIDEO_HD_DAC_ID_0,
    eAVCLUSTER_CFG_OUTPUT_VIDEO_HD_DAC_ID_1,
    eAVCLUSTER_CFG_OUTPUT_VIDEO_HD_DAC_ID_2,
    eAVCLUSTER_CFG_OUTPUT_HDMI_ID_0,
    eAVCLUSTER_CFG_OUTPUT_HDMI_ID_1,
    eAVCLUSTER_CFG_OUTPUT_HDMI_ID_2,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_0,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_1,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_2,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_SPDIF_ID_0,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_SPDIF_ID_1,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_SPDIF_ID_2,
	eAVCLUSTER_CFG_OUTPUT_AUDIO_HEADPHONE,
    eAVCLUSTER_CFG_OUTPUT_RF_MOD_ID_0,
    eAVCLUSTER_CFG_OUTPUT_RF_MOD_ID_1,
    eAVCLUSTER_CFG_OUTPUT_RF_MOD_ID_2
} tAVCLUSTER_CFG_OutputId;

typedef enum
{
	eAVCLUSTER_CFG_OUTPUT_MAIN,
	eAVCLUSTER_CFG_OUTPUT_AUX1,
	eAVCLUSTER_CFG_OUTPUT_AUX2,
	eAVCLUSTER_CFG_OUTPUT_AUX3,
	eAVCLUSTER_CFG_OUTPUT_AUX4
	/**....**/
}tAVCLUSTER_CFG_Output;

/*===========================================================================
 *
 * AVCLUSTE_CFG_GetOutputs
 *
 * Parameters:
 *		output IN: 		output requested
 *      ppOutputId IN:  Address of outputId pointer
 *		ppOutputId OUT: outputId pointer on filled outputId array
 *
 * Description:
 *      
 *
 * Returns:
 *      nb outputId
 *
 *===========================================================================*/
int32_t AVCLUSTER_CFG_GetOutputs(tAVCLUSTER_CFG_Output output, tAVCLUSTER_CFG_OutputId **ppOutputId);


/*===========================================================================
 *
 * AVCLUSTER_CFG_GetDefaultPCMOrBitstream
 *
 * Parameters:
 *		None
 *
 * Description:
 *      
 *
 * Returns:
 *      1 in case of bitstream else 0
 *
 *===========================================================================*/
uint8_t AVCLUSTER_CFG_GetDefaultPCMOrBitstream(void);

/*===========================================================================
 *
 * AVCLUSTER_CFG_GetAudioPreferredTrack
 *
 * Parameters:
 *		pstUserPref IN: 		preferences
 *      pstTrackList IN:		audio track list
 *      pstMainTrack IN/OUT:  	Main track found
 *		pstADTrack IN/OUT: 		AD track found
 *
 * Description:
 *      
 *
 * Returns:
 *      Succeed or failed
 *
 *===========================================================================*/
bool AVCLUSTER_CFG_GetAudioPreferredTrack(
			tAVCLUSTER_AUDIO_Format						eDecoderFormatSupported,
			tAVCLUSTER_AUDIO_UserSelectionPreferences 	*pstUserPref,
			tAVCLUSTER_AUDIO_TrackList					*pstTrackList,
			tAVCLUSTER_AUDIO_Track 						*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 						*pstADTrack);

/*===========================================================================
 *
 * AVCLUSTER_CFG_CheckAudioTrack
 *
 * Parameters:
 *		pstTrack IN: 		preferences
 *
 * Description:
 *      
 *
 * Returns:
 *      TRUE if this track must be taken into account
 * 		FALSE if this track must be ignored
 *
 *===========================================================================*/
bool AVCLUSTER_CFG_CheckAudioTrack(tAVCLUSTER_AUDIO_Track *pstTrack);

/*===========================================================================
 *
 * AVCLUSTER_CFG_GetAudioProcessing
 *
 * Parameters:
 *		pstMainTrack IN: 		Main track
 *		pstADTrack 	IN: 		AD Track
 * 		pstOutput	IN:			Output
 * 		pstAudioProcessing IN/OUT:	Audio Processing
 *
 * Description:
 *      
 *
 * Returns:
 *      TRUE 	if Audio processing Found
 * 		else FALSE
 *
 *===========================================================================*/
bool AVCLUSTER_CFG_GetAudioProcessing(
								tAVCLUSTER_AUDIO_Format						eDecoderFormatSupported,
								tAVCLUSTER_AUDIO_UserSelectionPreferences 	*pstUserPref,
								tAVCLUSTER_AUDIO_Track 						*pstMainTrack, 
								tAVCLUSTER_AUDIO_Track 						*pstADTrack, 
								tAVCLUSTER_AUDIO_Output						*pstOutput,
								tAVCLUSTER_AUDIO_Processing 				*pstAudioProcessing);

/*===========================================================================
 *
 * AVCLUSTER_CFG_GetAudioOutputConfiguration
 *
 * Parameters:
 *		pstUserCommonPref IN:				Common output preferences
 *		pstUserPref IN:						output preferences depending on an output type
 *		pstMainTrack IN: 					Main track
 *		pstADTrack 	IN: 					AD Track
 * 		pstAudioOutputConfiguration IN/OUT:	Audio output configuration
 *
 * Description:
 *      
 *
 * Returns:
 *      TRUE 	if Audio configuration Found
 * 		else FALSE
 *
 *===========================================================================*/
bool AVCLUSTER_CFG_GetAudioOutputConfiguration(
								int8_t											cOutputId,
								tAVCLUSTER_AUDIO_UserCommonOutputPreferences 	*pstUserCommonPref,
								tAVCLUSTER_AUDIO_UserOutputPreferences 			*pstUserPref,
								tAVCLUSTER_AUDIO_Track 							*pstMainTrack, 
								tAVCLUSTER_AUDIO_Track 							*pstADTrack, 
								tAVCLUSTER_AUDIO_OutputConfiguration			*pstAudioOutputConfiguration);


#ifdef __cplusplus
}
#endif

#endif /* _AVCLUSTER_CFG_H_ */
