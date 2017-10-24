/******************************************************************************
 *            COPYRIGHT 2005 IWEDIA TECHNOLOGIES       *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DIAG
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_diag.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * Description     : Diagnotic TDAL library primitives header.
 * Target      : All
 * Author      : Guy SCRUIGNEC.
 * Date   of creation   : 20 / 05 /   2005.
 *
 *****************************************************************************/

#ifndef _TDAL_DIAG_H_
#define _TDAL_DIAG_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
   eTDAL_DIAG_NO_ERROR = 0,
   eTDAL_DIAG_ERROR,
   eTDAL_DIAG_BAD_PARAMETER
} tTDAL_DIAG_Error;

typedef struct
{
   uint32_t   pollingTime; /* in ms */
   bool    audioActive;
   bool    videoActive;
   bool    demuxActive;
   bool    tsbuffActive;
   bool   TaskStackUsage;
}tTDAL_DIAG_InitParams;


IMPORT tTDAL_DIAG_Error TDAL_DIAG_Init(tTDAL_DIAG_InitParams *psInitParams);

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_DIAG_H_*/




