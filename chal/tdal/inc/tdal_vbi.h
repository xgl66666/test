/******************************************************************************
 *         COPYRIGHT 2004 IWEDIA TECHNOLOGIES      *
 ******************************************************************************
 *
 * MODULE NAME: tdal_vbi
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_vbi.h $
 *     $Rev: 66 $
 *     $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * Description     : Graphic library primitives header.
 * Target      : All
 * Author      : Olivier Le Bozec.
 * Date of creation    : 28/04/2004 18:05
 *
 *****************************************************************************/

#ifndef _TDAL_VBI_H_
#define _TDAL_VBI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------*/
/*    Public types definition   */
/*--------------------------------------*/

typedef enum
{
   eTDAL_VBI_NO_ERROR,
   eTDAL_VBI_NOT_DONE,
   eTDAL_VBI_BAD_PARAMETER,
   eTDAL_VBI_ERROR
}tTDAL_VBI_ErrorCode;

typedef enum
{
   eTDAL_VBI_TELETEXT,
   eTDAL_VBI_WSS,
}tTDAL_VBI_Type;

typedef enum {
   eTDAL_VBI_14_9_FULL_FORMAT, /* Full Format 4:3 (Shoot&Protect 14:9 center) */
   eTDAL_VBI_14_9_LETTERBOX_CENTER,
   eTDAL_VBI_14_9_LETTERBOX_TOP,
   eTDAL_VBI_16_9_FULL_FORMAT,
   eTDAL_VBI_16_9_LETTERBOX_CENTER,
   eTDAL_VBI_16_9_LETTERBOX_TOP,
   eTDAL_VBI_4_3_FULL_FORMAT,
   eTDAL_VBI_HIGHER_THAN_16_9_LETTERBOX_CENTER
} tTDAL_VBI_WssAspectRatio;

/*------------------------------------------*/
/*   Public Functions Declarations   */
/*------------------------------------------*/

tTDAL_VBI_ErrorCode TDAL_VBI_Init(void);
tTDAL_VBI_ErrorCode TDAL_VBI_Terminate(void);
tTDAL_VBI_ErrorCode TDAL_VBI_Open(tTDAL_VBI_Type VBI_Type);
tTDAL_VBI_ErrorCode TDAL_VBI_Close(tTDAL_VBI_Type VBI_Type);
tTDAL_VBI_ErrorCode TDAL_VBI_Start(void);
tTDAL_VBI_ErrorCode TDAL_VBI_Stop(void);
tTDAL_VBI_ErrorCode TDAL_VBI_SetWSSAspectRatio(tTDAL_VBI_WssAspectRatio WssAspectRatio);

const char *   TDAL_VBI_APIRevisionGet(void );
const char *   TDAL_VBI_PlatformRevisionGet(void );

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_VBI_H_*/
