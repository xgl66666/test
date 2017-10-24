/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: DVB SUBTITLE CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/dvb_subtitle_cfg.c $
 *            $Rev: 673 $
 *            $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: DVB Subtitle configuration
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "dvb_subtitle_cfg.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Typedefs                                */
/********************************************************/

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
#if 0
LOCAL tDS_CFG_SubtitleConfig DS_CFG_SubtitleConfiguration;
#endif
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * DS_CFG_GetSubtitleConfig
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Return the configuration of the DVB subtitle decoder
 *
 * Returns:
 *      NULL to use default configuation,
 *      tDS_CFG_SubtitleConfig struct otherwise
 *
 *===========================================================================*/
tDS_CFG_SubtitleConfig* DS_CFG_GetSubtitleConfig(void)
{
#if 1
    /********************************/
    /* to use default configuration */
    /********************************/
    return NULL; 

#else
    /***************************************/
    /* to change the default configuration */
    /***************************************/

    /* Size of the Coded Data Buffer (default: 30 kBytes on SD platforms, 100 kBytes on HD platforms) */
#ifdef DVB_SUBTITLE_HD
    DS_CFG_SubtitleConfiguration.uCodedDataBufferSize = (100*1024);
#else
    DS_CFG_SubtitleConfiguration.uCodedDataBufferSize = (30*1024);
#endif

    /* Size of the Pixel Buffer (default: 80 kBytes on SD platforms, 320 kBytes on HD platforms) */
    /* note: this buffer can be reallocated dynamically by the subtitle decoder if the space is not enough */
#ifdef DVB_SUBTITLE_HD
    DS_CFG_SubtitleConfiguration.uPixelBufferSize = (320*1024);
#else
    DS_CFG_SubtitleConfiguration.uPixelBufferSize = (80*1024);
#endif

    /* Size of the Composition Buffer (default: 8 kBytes) */
    DS_CFG_SubtitleConfiguration.uCompositionBufferSize = (8*1024);
    
    /* Size of the Working Buffer (default: 8 kBytes) */
    DS_CFG_SubtitleConfiguration.uWorkingBufferSize = (8*1024);
    
    return &DS_CFG_SubtitleConfiguration;
#endif
}

