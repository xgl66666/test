/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: MEDIA RENDERER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia/IDC3000/build/include/media_renderer_cfg.h $
 *            $Rev: 79 $
 *            $Date: 2011-08-02 14:26:19 +0200 (Tue, 02 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _MEDIA_RENDERER_CFG_H_
#define _MEDIA_RENDERER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************/
/*                                                      */
/********************************************************/
typedef enum
{
    eMEDIA_RENDERER_CFG_JPEG_COLOR_RGB,
    eMEDIA_RENDERER_CFG_JPEG_COLOR_YUV444,
    eMEDIA_RENDERER_CFG_JPEG_COLOR_YUV422
} MEDIA_RENDERER_CFG_JpegColorType;

typedef struct
{
    bool     bSeveralPass;
    uint32_t uiNbLinesPerPass; /* if bSeveralPass = TRUE, number of lines to
                                * decode at each pass (must be a modulo 16
                                * value) */
	MEDIA_RENDERER_CFG_JpegColorType eColor; /* color space of decode */
} MEDIA_RENDERER_CFG_JpegDecodeParams;

/********************************************************/
/*                                                      */
/********************************************************/
void MEDIA_RENDERER_CFG_JpegInputBufferSizeListGet(uint8_t   *pucNbSize,
                                                   uint32_t **ppuiSizeList);

void MEDIA_RENDERER_CFG_Mp3InputBufferSizeListGet(uint8_t   *pucNbSize,
                                                  uint32_t **ppuiSizeList);


void MEDIA_RENDERER_CFG_JpegDecodeParamsGet(MEDIA_RENDERER_CFG_JpegDecodeParams *pstParams);

void MEDIA_RENDERER_CFG_AvInputBufferSizeListGet(uint8_t   *pucNbSize,
												 uint32_t **ppuiSizeList);

#ifdef __cplusplus
}
#endif

#endif /* _MEDIA_RENDERER_CFG_H_ */
