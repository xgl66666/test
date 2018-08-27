/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: MEDIA BROWSER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia/IDC3000/build/src/media_browser_cfg.c $
 *            $Rev: 79 $
 *            $Date: 2011-08-02 14:26:19 +0200 (Tue, 02 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include <string.h>
#include <ctype.h>

#include "media_browser_cfg.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * MEDIA_BROWSER_CFG_IsAvTypeSupported
 *
 * Parameters:
 *      none
 *
 *Description:
 *      none
 *
 * Returns:
 *      none
 *
 *===========================================================================*/

bool MEDIA_BROWSER_CFG_IsAvTypeSupported
(
    tMEDIA_BROWSER_CFG_VideoCodec eVideoCodec,
    tMEDIA_BROWSER_CFG_AudioCodec eAudioCodec,
    uint32_t                      uiVideoWidth,
    uint32_t                      uiVideoHeight
)
{
#ifdef PRODUCT_USE_AVP
    if (eVideoCodec == eMEDIA_BROWSER_CFG_CODEC_VIDEO_MPEG1 || eVideoCodec == eMEDIA_BROWSER_CFG_CODEC_VIDEO_MPEG2)
    {
        if (uiVideoWidth <= 720 && uiVideoHeight <= 576)
        {
            if (eAudioCodec == eMEDIA_BROWSER_CFG_CODEC_AUDIO_MPEG1 || eAudioCodec == eMEDIA_BROWSER_CFG_CODEC_AUDIO_MPEG2)
            {
                return true;
            }
        }
    }
#endif

    return false;
}
