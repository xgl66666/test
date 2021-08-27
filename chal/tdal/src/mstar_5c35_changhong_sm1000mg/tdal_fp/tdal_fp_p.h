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
typedef enum
{
	CHDRV_FP_LED_RED,				///< 红色指示灯
	CHDRV_FP_LED_YELLOW,			///< 黄色指示灯
	CHDRV_FP_LED_GREEN,				///< 绿色指示灯
}CHDRV_FP_LAMP_TYPE_e;
///@brief LED灯状态定义
typedef enum
{
	CHDRV_FP_LAMP_ON,			///< 开
	CHDRV_FP_LAMP_OFF,			///< 关
	CHDRV_FP_LAMP_STATE_MAX		///< 错误判断
}CHDRV_FP_LAMP_STATE_e;


#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_FP_PRIV_H_*/

