/****************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES            *
 ****************************************************************************
 *
 * MODULE NAME: TDAL_AV
 *
 * FILE NAME:   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_av/tdal_av_p.h $
 *           $Rev: 414 $
 *           $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description: Audio/Video library primitives header.
 *
 *****************************************************************************/

#ifndef _TDAL_AV_P_H_
#define _TDAL_AV_P_H_


#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************
 *  Defines                                       *
 ***************************************************************/
#define kTDAL_AVi_STC_DELAY_INIT_LIVE      (-38000)
#define kTDAL_AVi_STC_DELAY_MAX_LIVE       (-200000)
#define kTDAL_AVi_STC_DELAY_ADJUST_LIVE     (-1500)
#if 0
#define kTDAL_AVi_STC_DELAY_INIT_PLAYBACK   (-1800)
#define kTDAL_AVi_STC_DELAY_MAX_PLAYBACK    (-3800)
#define kTDAL_AVi_STC_DELAY_ADJUST_PLAYBACK  (-200)
#else
#define kTDAL_AVi_STC_DELAY_INIT_PLAYBACK   (-38000)
#define kTDAL_AVi_STC_DELAY_MAX_PLAYBACK    (-200000)
#define kTDAL_AVi_STC_DELAY_ADJUST_PLAYBACK  (-200)
#endif

#define kTDAL_AVi_PTS_MINUS_STC_PREVENT_THRESHOLD   0x100

/* Message queue for AV Event */
typedef struct
{
   tTDAL_AV_Decoder      decoder;
   MS_U32   Event;
   bool               Exit;
   //void*               pData;
}tTDAL_AV_EvtMsg;

IMPORT bool              TDAL_AV_AlreadyInitialized;
IMPORT TDAL_queue_id			 TDAL_AV_EvtQueue_p;
IMPORT TDAL_mutex_id     TDAL_AVi_Mutex;
IMPORT volatile bool     TDAL_AV_Started;

IMPORT tTDAL_AV_Error p_TDAL_AV_Audio_Start( tTDAL_AV_Decoder decoder, tTDAL_AV_StreamType StreamType, bool stopAudioThread );
IMPORT tTDAL_AV_Error p_TDAL_AV_Audio_Stop(tTDAL_AV_Decoder decoder);

IMPORT void p_TDAL_AV_Video_UseEvent(tTDAL_AV_Decoder   decoder, MS_U32 event);
IMPORT void p_TDAL_AV_Audio_UseEvent(tTDAL_AV_Decoder  decoder, MS_U32 event);

#endif /*_TDAL_AV_P_H_*/
