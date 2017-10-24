/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: MEDIUM CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/medium_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _MEDIUM_CFG_H_
#define _MEDIUM_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*               typedef                               */
/*******************************************************/
uint16_t MEDIUM_CFG_GetMaxNumberOfMultiplex(void);
uint16_t MEDIUM_CFG_GetMaxNumberOfAnalogMultiplex(void);
uint16_t MEDIUM_CFG_GetMaxNumberOfServices(void);
uint8_t MEDIUM_CFG_GetMaxNumberOfESDescriptors(void);
int16_t MEDIUM_CFG_GetManualInstallationSignalQuality(void);


#ifdef __cplusplus
}
#endif

#endif /* _MEDIUM_CFG_H_ */
