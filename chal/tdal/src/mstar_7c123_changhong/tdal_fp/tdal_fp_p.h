/******************************************************************************
 *                   COPYRIGHT 2004 IWEDIA TECHNOLOGIES                   *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_FP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fp/tdal_fp_priv.h $
 *          $Rev: 414 $
 *          $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL FP private header.
 *
 *****************************************************************************/

#ifndef   _TDAL_FP_PRIV_H_
#define   _TDAL_FP_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

/*******************************************************/
/*          Defines                     */
/*******************************************************/
#define SAR_CHANNEL_0 0
#define SAR_CHANNEL_1 1
#define SAR_CHANNEL_2 2
#define IO_NUM_LED_GREEN_LOCK 90//PAD_PM_GPIO1 [FP_DOUT]
#define IO_NUM_LED_RED_POWER 91//PAD_PM_GPIO2 [FP_CLK]
#define IO_NUM_LED_YELLOW_STANDBY 92//PAD_PM_GPIO3 [FP_STB]
/*******************************************************/
/*          Macros                         */
/*******************************************************/

/*******************************************************/
/*          Typedefs                     */
/*******************************************************/

/*******************************************************/
/*          Variables   Declarations   (MODULE)      */
/*******************************************************/

/*******************************************************/
/*          Functions   Declarations              */
/*******************************************************/
tTDAL_FP_ErrorCode   TDAL_FP_key_Init(void);
tTDAL_FP_ErrorCode   TDAL_FP_key_Terminate(void);


#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_FP_PRIV_H_*/

