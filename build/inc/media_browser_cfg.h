/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: MEDIA BROWSER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia/IDC3000/build/include/media_browser_cfg.h $
 *            $Rev: 79 $
 *            $Date: 2011-08-02 14:26:19 +0200 (Tue, 02 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _MEDIA_BROWSER_CFG_H_
#define _MEDIA_BROWSER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************/
/*                                                      */
/********************************************************/
typedef enum
{
    eMEDIA_BROWSER_CFG_CODEC_VIDEO_MPEG1,
    eMEDIA_BROWSER_CFG_CODEC_VIDEO_MPEG2,
    eMEDIA_BROWSER_CFG_CODEC_VIDEO_H264,
    eMEDIA_BROWSER_CFG_CODEC_VIDEO_MPEG4P2,
    eMEDIA_BROWSER_CFG_CODEC_VIDEO_DIVX,
    eMEDIA_BROWSER_CFG_CODEC_VIDEO_XVID,

    eMEDIA_BROWSER_CFG_CODEC_VIDEO_INVALID,
} tMEDIA_BROWSER_CFG_VideoCodec;

typedef enum
{
    eMEDIA_BROWSER_CFG_CODEC_AUDIO_MPEG1,
    eMEDIA_BROWSER_CFG_CODEC_AUDIO_MPEG2,
    eMEDIA_BROWSER_CFG_CODEC_AUDIO_MP3,

    eMEDIA_BROWSER_CFG_CODEC_AUDIO_INVALID,
} tMEDIA_BROWSER_CFG_AudioCodec;

/********************************************************/
/*                                                      */
/********************************************************/

bool MEDIA_BROWSER_CFG_IsAvTypeSupported(   tMEDIA_BROWSER_CFG_VideoCodec eVideoCodec,
                                            tMEDIA_BROWSER_CFG_AudioCodec eAudioCodec,
                                            uint32_t                      uiVideoWidth,
                                            uint32_t                      uiVideoHeight);

#ifdef __cplusplus
}
#endif

#endif /* _MEDIA_BROWSER_CFG_H_ */
