/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME: 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/play_glue_evt_cfg.h $
 * Header of the System Manager
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC - 
 *
 * DESCRIPTION: this file contain all RC5 code of remote control supported 
 *
 *****************************************************************************/
#ifndef _PLAY_GLUE_EVT_CFG_H_
#define _PLAY_GLUE_EVT_CFG_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "player_keys.h"
#include "comedia_keys.h"

typedef struct
{
  uint8_t sysCode;
  uint8_t rtmCode;
} tPLAY_KeyMapping;

extern tSYS_KeyEvtCode SYS_AllKeyEvtList[];
extern const uint32_t SYS_AllKeyEvtListCount;

uint8_t 	PLAY_NumberOfRtmCode(tSYS_KeyEvtCode KeyCode, tSYS_KeyEvtState KeyStatus);
bool	 	PLAY_SysCodeToRtmCode(tSYS_KeyEvtCode KeyCode, tSYS_KeyEvtState KeyStatus, uint8_t* ptrRtm);

#ifdef __cplusplus
}
#endif


#endif /*_PLAY_GLUE_EVT_CFG_H_*/


