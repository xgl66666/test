/******************************************************************************
 *                           COPYRIGHT 2006 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: MHEG CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/mheg_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _MHEG_CFG_H_
#define _MHEG_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*               typedef                               */
/*******************************************************/

/********************************************************/
/*              Global File Variables                   */
/********************************************************/  

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/
void            MHEG_CFG_MEM_Init(void);

uint32_t        MHEG_CFG_GetMhegMemorySize(void);
uint8_t *       MHEG_CFG_GetMhegMemoryPartition(void);


uint16_t MHEG_CFG_GetReceiverKeyCount(void);
uint16_t MHEG_CFG_GetICStreamingKeyCount(void);
uint16_t 			MHEG_CFG_GetAllKeyCount(void);
uint16_t 			MHEG_CFG_GetDigitKeyCount(void);
uint16_t 			MHEG_CFG_GetArrowKeyCount(void);
uint16_t 			MHEG_CFG_GetAlwaysMHEGKeyCount(void);
bool				MHEG_CFG_GetKeyCodeArrayFromIndex(uint8_t index, tSYS_KeyEvtCode** ppKeyCodeArray);


#ifdef __cplusplus
}
#endif

#endif /* _MHEG_CFG_H_ */
