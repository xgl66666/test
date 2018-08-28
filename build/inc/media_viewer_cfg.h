/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: MEDIA VIEWER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia/IDC3000/build/include/media_viewer_cfg.h $
 *            $Rev: 79 $
 *            $Date: 2011-08-02 14:26:19 +0200 (Tue, 02 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _MEDIA_VIEWER_CFG_H_
#define _MEDIA_VIEWER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************/
/*                                                      */
/********************************************************/
#define eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT       0x010000
#define eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO   0x020000
#define eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV         0x040000

typedef enum
{
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_UNKNOWN =0,

    eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_BMP  	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT | 0x0001),          /* Bitmap */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_JPEG  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT | 0x0002),          /* Joint Photographic Experts Group */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_GIF  	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT | 0x0004),          /* Graphics Interchange Format */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_PNG  	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT | 0x0008),          /* Portable Network Graphics */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_TIFF  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT | 0x0010),          /* Tag(ged) Image File Format */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_RAW  	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT | 0x0020),          /* Proprietary raw image */

    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_MP3  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO | 0x0001),         /* MPEG-1/2 Audio Layer 3 */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_FLAC = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO | 0x0002),         /* Free Lossless Audio Codec */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_OGG  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO | 0x0004),         /* Ogg Vorbis */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_WMA  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO | 0x0008),         /* Windows Media Audio */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_AAC  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO | 0x0010),         /* Advanced Audio Coding */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_WAV  = (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO | 0x0020),         /* short for Waveform */

    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MPEG2_ES  	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0001),        /* MPEG2 elementray stream */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MPEG2_TS  	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0002),        /* MPEG2 transport stream */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_DIVX      	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0004),        /* Divx */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_AVI       	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0008),        /* Audio Video Interleave */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MATROSKA	= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0010),        /* extensible open standard Audio/Video container */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_3GP	 		= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0020),        /* container format defined by the Third Generation Partnership Project (3GPP) */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MP4			= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0040),        /* iso MPEG4 */
    eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_WMV			= (eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV | 0x0080)         /* Microsoft */

} tMEDIA_VIEWER_CFG_MediaType;


/********************************************************/
/*                                                      */
/********************************************************/
bool MEDIA_VIEWER_CFG_SupportedMediaTypeGet(uint32_t *puiMediaTypeMask,
                                             uint32_t *puiPictureMask,
                                             uint32_t *puiAudioMask,
                                             uint32_t *puiAudioVideoMask);

bool MEDIA_VIEWER_CFG_DisplayFileExtension(void);

bool MEDIA_VIEWER_CFG_MediaTypeFromFileGet(    char                    *pcFilename,
                                                                            tMEDIA_VIEWER_CFG_MediaType *peType);

bool MEDIA_VIEWER_CFG_PreferredOrderMediaTypeGet(tMEDIA_VIEWER_CFG_MediaType** picture_order, int32_t * picture_order_size ,tMEDIA_VIEWER_CFG_MediaType ** audio_order, int32_t * audio_order_size, tMEDIA_VIEWER_CFG_MediaType ** video_order, int32_t * video_order_size );

#ifdef __cplusplus
}
#endif

#endif /* _MEDIA_VIEWER_CFG_H_ */
