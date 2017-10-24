/******************************************************************************
 *                         COPYRIGHT 2008 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: AVCLUSTER_AUDIO_TYPES
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/av_cluster_video_api.h $
 *            $Rev: 244 $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *
 *****************************************************************************/

#ifndef _AVCLUSTER_VIDEO_API_H_
#define _AVCLUSTER_VIDEO_API_H_


#ifdef __cplusplus
extern "C" {
#endif

bool AVCLUSTER_CFG_GetVideoProcessing(
			tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
			tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
			tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
			tAVCLUSTER_VIDEO_Output							*pstOutput,
			tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing);

bool AVCLUSTER_CFG_GetVideoOutputConfiguration(
			tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
			tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
			tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
			tAVCLUSTER_VIDEO_Output							*pstOutput,
			tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing,
			tAVCLUSTER_VIDEO_Configuration					*pstVideoOuputConfiguration);

#ifdef __cplusplus
}
#endif

#endif

