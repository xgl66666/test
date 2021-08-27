/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_UART
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_uart/tdal_uart_p.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * PRIVATE / PUBLIC - delete the non-applicable mention
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef   _TDAL_UART_P_H_
#define   _TDAL_UART_P_H_

/*******************************************************/
/*        Includes                 */
/*******************************************************/

#ifdef   __cplusplus
extern   "C"   {
#endif

/*******************************************************/
/*        Defines                 */
/*******************************************************/
#define   kTDAL_UART_DEFAULT_INSTANCE      eTDAL_UART_0
#define   kTDAL_UART_DEFAULT_BAUD_RATE   115200
#define   kTDAL_UART_DEFAULT_DATA_BITS   eTDAL_UART_DATA_BITS_8
#define   kTDAL_UART_DEFAULT_STOP_BITS   eTDAL_UART_STOP_BITS_1
#define   kTDAL_UART_DEFAULT_PARITY      eTDAL_UART_PARITY_NONE
#define   kTDAL_UART_DEFAULT_FLOW_CONTROL   eTDAL_UART_FLOW_CTRL_NONE

/*******************************************************/
/*        Macros                   */
/*******************************************************/

/*******************************************************/
/*        Typedefs                 */
/*******************************************************/

/*******************************************************/
/*        Variables   Declarations   (IMPORT)      */
/*******************************************************/

/*******************************************************/
/*        Functions   Declarations            */
/*******************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_ConvertBaudRate   (tTDAL_UART_Baud_Rate   UART_Baud_Rate,   uint32_t   *Baud_Rate);
tTDAL_UART_ErrorCode   TDAL_UART_ConvertDataBits   (tTDAL_UART_Data_Bits   Data_Bits,
      tTDAL_UART_Parity   Parity,   STUART_DataBits_t   *STUART_DataBits);
tTDAL_UART_ErrorCode   TDAL_UART_ConvertStopBits   (tTDAL_UART_Stop_Bits   Stop_Bits,   STUART_StopBits_t   *STUART_StopBits);
tTDAL_UART_ErrorCode   TDAL_UART_ConvertFlowControl   (tTDAL_UART_Flow_Ctrl   Flow_Ctrl,   STUART_FlowControl_t   *STUART_FlowControl);

#ifdef   __cplusplus
}
#endif


#endif   /*_TDAL_UART_P_H_*/

