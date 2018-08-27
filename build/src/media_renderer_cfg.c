/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: APPLI CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia/IDC3000/build/src/media_renderer_cfg.c $
 *            $Rev: 79 $
 *            $Date: 2011-08-02 14:26:19 +0200 (Tue, 02 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "media_renderer_cfg.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define K 1024
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * MEDIA_SOURCE_CFG_JpegInputBufferSizesGet
 *
 * Parameters:
 *      none
 *
 * Description: For JPEG input buffer, give a list of sizes to test.
 *
 *
 * Returns:
 *      none
 *
 *===========================================================================*/
void MEDIA_RENDERER_CFG_JpegInputBufferSizeListGet
(
    uint8_t   *pucNbSize,
    uint32_t **ppuiSizeList
)
{
    /* !!! Sizes must be mulitple of 16*K !!! */
    static uint32_t List[4] =
        {(512*K),
         (256*K),
         ( 64*K),
         ( 32*K)};

    *pucNbSize    = 4;
    *ppuiSizeList = List;
    return;
}


/*===========================================================================
 *
 * MEDIA_SOURCE_CFG_JpegDecodeParamsGet
 *
 * Parameters:
 *      none
 *
 * Description: For JPEG multipass decoding, give a list of sizes to test.
 *
 *
 * Returns:
 *      none
 *
 *===========================================================================*/
void MEDIA_RENDERER_CFG_JpegDecodeParamsGet
(
    MEDIA_RENDERER_CFG_JpegDecodeParams *pstParams
)
{
    pstParams->bSeveralPass     = TRUE;
    
    /* if bSeveralPass = TRUE, number of lines to decode at each pass
     * (It must be a modulo 16 value) */
    pstParams->uiNbLinesPerPass = 64;
    
#if defined(PRODUCT_MEDIA_VIEWER_JPEG_COLOR_TYPE_YUV444)
	pstParams->eColor = eMEDIA_RENDERER_CFG_JPEG_COLOR_YUV444;
#elif defined(PRODUCT_MEDIA_VIEWER_JPEG_COLOR_TYPE_YUV422)
	pstParams->eColor = eMEDIA_RENDERER_CFG_JPEG_COLOR_YUV422;
#else
	pstParams->eColor = eMEDIA_RENDERER_CFG_JPEG_COLOR_RGB;
#endif
    return;
}

/*===========================================================================
 *
 * MEDIA_SOURCE_CFG_Mp3InputBufferSizesGet
 *
 * Parameters:
 *      none
 *
 * Description: For MP3 input buffer, give a list of sizes to test.
 *
 *
 * Returns:
 *      none
 *
 *===========================================================================*/
void MEDIA_RENDERER_CFG_Mp3InputBufferSizeListGet
(
    uint8_t   *pucNbSize,
    uint32_t **ppuiSizeList
)
{
    /* !!! Sizes must be mulitple of 16*K !!! */
    static uint32_t List[3] =
        {(128*K),
         ( 64*K),
         ( 32*K)};

    *pucNbSize    = 3;
    *ppuiSizeList = List;
    return;
}


/*===========================================================================
 *
 * MEDIA_SOURCE_CFG_AvInputBufferSizesGet
 *
 * Parameters:
 *      none
 *
 * Description: For Av input buffer, give a list of sizes to test.
 *
 *
 * Returns:
 *      none
 *
 *===========================================================================*/
void MEDIA_RENDERER_CFG_AvInputBufferSizeListGet
(
    uint8_t   *pucNbSize,
    uint32_t **ppuiSizeList
)
{
    /* !!! Sizes must be mulitple of 16*K !!! */
#if 0
    static uint32_t List[4] =
        {(512*K),
         (256*K),
         ( 64*K),
         ( 32*K)};

    *pucNbSize    = 4;
    *ppuiSizeList = List;
#else
    static uint32_t List[3] =
		{(128*K),
		 (64*K),
		 (32*K)};

    *pucNbSize    = 3;
    *ppuiSizeList = List;
#endif
    return;
}

