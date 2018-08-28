/******************************************************************************
 *                    COPYRIGHT 2008 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: DVB SUBTITLE CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/dvb_subtitle_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: DVB Subtitle configuration header
 *
 *****************************************************************************/

#ifndef _DVB_SUBTITLE_CFG_H_
#define _DVB_SUBTITLE_CFG_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/


/*******************************************************/
/*              Macros                                 */
/*******************************************************/


/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef struct
{
    uint32_t uCodedDataBufferSize;      /* Size of the Coded Data Buffer (default: 30 kBytes on SD platforms, 100 kBytes on HD platforms) */
    uint32_t uPixelBufferSize;          /* Size of the Pixel Buffer (default: 80 kBytes on SD platforms, 320 kBytes on HD platforms) */
    uint32_t uCompositionBufferSize;    /* Size of the Composition Buffer (default: 8 kBytes) */
    uint32_t uWorkingBufferSize;        /* Size of the Working Buffer (default: 8 kBytes) */
}
tDS_CFG_SubtitleConfig;


/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/


/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/


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
 *      tDS_CFG_Configuration struct otherwise
 *
 *===========================================================================*/
tDS_CFG_SubtitleConfig* DS_CFG_GetSubtitleConfig(void);



#ifdef __cplusplus
}
#endif

#endif /* _DVB_SUBTITLE_CFG_H_ */
