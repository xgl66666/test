/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_UART
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_uart/tdal_uart.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL UART implementation
 *
 *****************************************************************************/

/********************************************************/
/*          Includes                        */
/********************************************************/
#include <string.h>


#include "crules.h"
#include "tbox.h"

#include "tdal_uart.h"


#include "tdal_common.h"

/********************************************************/
/*        Defines                   */
/********************************************************/

/********************************************************/
/*        Macros                   */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_UART);

/********************************************************/
/*        Typedefs                 */
/********************************************************/

/********************************************************/
/*        Global   Variables   (GLOBAL/IMPORT)      */
/********************************************************/
/********************************************************/
/*        Local   Module   Variables   (MODULE)      */
/********************************************************/

/********************************************************/
/*        Local   File   Variables   (LOCAL)      */
/********************************************************/

/********************************************************/
/*        Local   Functions   Declarations   (LOCAL)   */
/********************************************************/

/********************************************************/
/*        Functions   Definitions   (GLOBAL)      */
/********************************************************/

/******************************************************************************
   *   Function   Name   :   TDAL_UART_Init
   *   Description      :   Initialize   of   the   UART   module
   *   Side   effects      :
   *   Comment      :
   *   Inputs      :
   *   Outputs      :
   *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Init()
{
   return (eTDAL_UART_NO_ERROR);
}

/******************************************************************************
   *   Function   Name      :   TDAL_UART_Terminate
   *   Description      :   De-Initialize   of   the   UART   module
   *   Side   effects      :
   *   Comment      :
   *   Inputs        :
   *   Outputs      :
   *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Terminate()
{
      return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
   *   Function   Name      :   TDAL_UART_Open
   *   Description      :
   *   Side   effects      :
   *   Comment      :
   *   Inputs        :
   *   Outputs      :
   *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Open   (tTDAL_UART_Connect_parameters   Connect_parameters)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_Close
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Close   (tTDAL_UART_Instance   instance)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_Write
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Write(tTDAL_UART_Instance   instance,
                   uint8_t*   pBytesToWrite,
                   uint32_t   nbBytesToWrite,
                   uint32_t*   pNbBytesWritten)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_Read
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Read(tTDAL_UART_Instance   instance,
                   uint8_t*   pBytesRead,
                   uint32_t   nbBytesToRead,
                   uint32_t*   pNbBytesRead)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_ReadWithTimeout
 * Description       : Idem to TDAL_UART_Read else the timeout is in
 *                parameter and not the default value.
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_ReadWithTimeout   (tTDAL_UART_Instance   instance,
                        uint8_t*   pBytesRead,
                        uint32_t   nbBytesToRead,
                        uint32_t*   pNbBytesRead,
                        uint32_t   timeoutMS)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_Flush
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_Flush   (tTDAL_UART_Instance   instance)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_GetCapabilities
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_GetCapabilities   (tTDAL_UART_Connect_parameters   *Capabilities)
{
   return(eTDAL_UART_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_UART_ConvertBaudRate
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_ConvertBaudRate   (tTDAL_UART_Baud_Rate   UART_Baud_Rate,   uint32_t   *Baud_Rate)
{
    return(eTDAL_UART_NO_ERROR);

}

#ifdef NEVER
/******************************************************************************
 * Function Name      : TDAL_UART_ConvertDataBits
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_ConvertDataBits   (tTDAL_UART_Data_Bits   Data_Bits,
      tTDAL_UART_Parity   Parity,   STUART_DataBits_t   *STUART_DataBits)
{
   return(eTDAL_UART_BAD_PARAMETER);
}

/******************************************************************************
 * Function Name      : TDAL_UART_ConvertStopBits
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_ConvertStopBits   (tTDAL_UART_Stop_Bits   Stop_Bits,   STUART_StopBits_t   *STUART_StopBits)
{
   return(eTDAL_UART_BAD_PARAMETER);
}

/******************************************************************************
 * Function Name      : TDAL_UART_ConvertFlowControl
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_UART_ErrorCode   TDAL_UART_ConvertFlowControl   (tTDAL_UART_Flow_Ctrl   Flow_Ctrl,   STUART_FlowControl_t   *STUART_FlowControl)
{
      return(eTDAL_UART_BAD_PARAMETER);
}
#endif
