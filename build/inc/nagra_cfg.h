/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://ren-svn-01/svncomedia/idecode/product/comedia/branches/snt_cos/main/build/inc/nagra_cfg.h $
 *            $Rev: 9175 $
 *            $Date: 2012-02-14 11:37:55 +0100 (mar., 14 févr. 2012) $
 *
 * PUBLIC
 *
 * DESCRIPTION: functions needed to get information for configuring NAGRA
 *
 *****************************************************************************/

#ifndef _NAGRA_CFG_H_
#define _NAGRA_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

typedef enum
{
    CA_CFG_Location_Unknown,
    CA_CFG_Location_Memory,
    CA_CFG_Location_Flash,
} CA_CFG_Location;

/*******************************************************/
/*              Functions                              */
/*******************************************************/

CA_CFG_Location CA_CFG_GetCSCLocation(void);
CA_CFG_Location CA_CFG_GetPKLocation(void);


#ifdef __cplusplus
}
#endif

#endif /*_NAGRA_CFG_H_*/
