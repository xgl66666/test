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


#include "stlayer.h"
#include "stcommon.h"
#include "stlite.h"   /*   os20   for   standard   definitions,      */
#include "stdevice.h"   /*   hardware   definitions   for   the   55XX      */
#include "stuart.h"
#include "sttuner.h"

#include "crules.h"
#include "tbox.h"
#include "pltf_hardcfg.h"

#include "tdal_uart.h"
#include "tdal_uart_p.h"


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
IMPORT   ST_DeviceName_t      SYS_UartName[];
IMPORT   ST_DeviceName_t      SYS_PioName[];

#if   defined(BOOTLOADER)
IMPORT   BOOL   TDAL_COMMON_bJumpWithReset;
#endif
/********************************************************/
/*        Local   Module   Variables   (MODULE)      */
/********************************************************/
LOCAL   bool      gTDAL_uartInitialized   = FALSE;
LOCAL   bool      gTDAL_uartOpen[kTDAL_UART_NB_MAX_INSTANCE];
LOCAL   STUART_Handle_t   gTDAL_uartHandle[kTDAL_UART_NB_MAX_INSTANCE];

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
   uint8_t   i;
   ST_ClockInfo_t      ClockInfo;   

   mTBOX_FCT_ENTER("TDAL_UART_Init");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
#endif

   /*   Test   if   TDAL_UART   has   already   been   initialised   */
   if   (gTDAL_uartInitialized == TRUE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Init   error   :   already   initialized\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   for   (i = 0   ;   i   <   kTDAL_UART_NB_MAX_INSTANCE   ;   i++)
      gTDAL_uartOpen[i] = FALSE;

   gTDAL_uartInitialized = TRUE;

   ST_GetClockInfo(&ClockInfo);
   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   tTDAL_UART_ErrorCode   ErrorCode;
   bool            ErrorFound;

   mTBOX_FCT_ENTER("TDAL_UART_Terminate");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
#endif

   /*   Test   if   TDAL_UART   is   still   initialised   */
   if   (gTDAL_uartInitialized == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate   error   :   already   terminated\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   ErrorFound = FALSE;
   /*   close   open   instances   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (gTDAL_uartOpen[0] == TRUE))
   {
      ErrorCode = TDAL_UART_Close(eTDAL_UART_0);
      if   (ErrorCode != eTDAL_UART_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate   TDAL_UART_Close   UART_0   error\n"));
      ErrorFound = TRUE;
      }
      gTDAL_uartOpen[0] = FALSE;
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (gTDAL_uartOpen[1] == TRUE))
   {
      ErrorCode = TDAL_UART_Close(eTDAL_UART_1);
      if   (ErrorCode != eTDAL_UART_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate   TDAL_UART_Close   UART_1   error\n"));
      ErrorFound = TRUE;
      }
      gTDAL_uartOpen[1] = FALSE;
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (gTDAL_uartOpen[2] == TRUE))
   {
      ErrorCode = TDAL_UART_Close(eTDAL_UART_2);
      if   (ErrorCode != eTDAL_UART_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate   TDAL_UART_Close   UART_2   error\n"));
      ErrorFound = TRUE;
      }
      gTDAL_uartOpen[2] = FALSE;
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (gTDAL_uartOpen[3] == TRUE))
   {
      ErrorCode = TDAL_UART_Close(eTDAL_UART_3);
      if   (ErrorCode != eTDAL_UART_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate   TDAL_UART_Close   UART_3   error\n"));
      ErrorFound = TRUE;
      }
      gTDAL_uartOpen[3] = FALSE;
   }

   gTDAL_uartInitialized = FALSE;

   if   (ErrorFound)
   {
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }
   else
   {
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
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
   uint8_t            i;
   tTDAL_UART_ErrorCode      ErrorCode;
   ST_ErrorCode_t        ST_ErrorCode = ST_NO_ERROR;
   ST_DeviceName_t        UartDeviceName[kMAX_ASC+1];
   tTDAL_UART_Connect_parameters   Capabilities;
   uint32_t            BaudRate;
   STUART_DataBits_t        STUART_DataBits;
   STUART_StopBits_t        STUART_StopBits;
   STUART_FlowControl_t      STUART_FlowControl;
   STUART_Params_t        UartDefaultParams;
   STUART_OpenParams_t      UartOpenParams;

   mTBOX_FCT_ENTER("TDAL_UART_Open");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   /*   check   if   module   has   been   initialized   */
   if   (gTDAL_uartInitialized == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   check   Instance   */
   if   ((Connect_parameters.Instance == 0)   ||
      ((Connect_parameters.Instance != eTDAL_UART_0)   &&
      (Connect_parameters.Instance != eTDAL_UART_1)   &&
      (Connect_parameters.Instance != eTDAL_UART_2)   &&
      (Connect_parameters.Instance != eTDAL_UART_3)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Bad   instance\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Baud_Rate   */
   if   ((Connect_parameters.Baud_Rate == 0)   ||
      ((Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_110)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_300)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_600)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_1200)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_2400)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_4800)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_9600)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_14400)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_19200)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_38400)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_57600)   &&
      (Connect_parameters.Baud_Rate != eTDAL_UART_BAUD_RATE_115200)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Bad   Baud_Rate\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Data_Bits   */
   if   ((Connect_parameters.Data_Bits == 0)   ||
      ((Connect_parameters.Data_Bits != eTDAL_UART_DATA_BITS_7)   &&
      (Connect_parameters.Data_Bits != eTDAL_UART_DATA_BITS_8)   &&
      (Connect_parameters.Data_Bits != eTDAL_UART_DATA_BITS_9)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Bad   Data_Bits\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Stop_Bits   */
   if   ((Connect_parameters.Stop_Bits == 0)   ||
      ((Connect_parameters.Stop_Bits != eTDAL_UART_STOP_BITS_1)   &&
      (Connect_parameters.Stop_Bits != eTDAL_UART_STOP_BITS_1)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Bad   Stop_Bits\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Parity   */
   if   ((Connect_parameters.Parity == 0)   ||
      ((Connect_parameters.Parity != eTDAL_UART_PARITY_NONE)   &&
      (Connect_parameters.Parity != eTDAL_UART_PARITY_EVEN)   &&
      (Connect_parameters.Parity != eTDAL_UART_PARITY_ODD)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Bad   Parity\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Flow_Ctrl   */
   if   ((Connect_parameters.Flow_Ctrl == 0)   ||
      ((Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_NONE)   &&
      (Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_HARD)   &&
      (Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_X)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Bad   Flow_Ctrl\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   capabilities   */
   ErrorCode = TDAL_UART_GetCapabilities(&Capabilities);
   if   (ErrorCode != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   TDAL_UART_GetCapabilities   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   (((Capabilities.Instance   &   Connect_parameters.Instance) != Connect_parameters.Instance)   ||
      ((Capabilities.Baud_Rate   &   Connect_parameters.Baud_Rate) != Connect_parameters.Baud_Rate)   ||
      ((Capabilities.Data_Bits   &   Connect_parameters.Data_Bits) != Connect_parameters.Data_Bits)   ||
      ((Capabilities.Stop_Bits   &   Connect_parameters.Stop_Bits) != Connect_parameters.Stop_Bits)   ||
      ((Capabilities.Parity   &   Connect_parameters.Parity) != Connect_parameters.Parity)   ||
      ((Capabilities.Flow_Ctrl   &   Connect_parameters.Flow_Ctrl) != Connect_parameters.Flow_Ctrl))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   Capability   override\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   if   Instance   is   allready   open   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (Connect_parameters.Instance == eTDAL_UART_0)   &&
      (gTDAL_uartOpen[0] == TRUE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   UART_0   allready   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 1)   &&
      (Connect_parameters.Instance == eTDAL_UART_1)   &&
      (gTDAL_uartOpen[1] == TRUE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   UART_1   allready   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 2)   &&
      (Connect_parameters.Instance == eTDAL_UART_2)   &&
      (gTDAL_uartOpen[2] == TRUE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   UART_2   allready   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 3)   &&
      (Connect_parameters.Instance == eTDAL_UART_3)   &&
      (gTDAL_uartOpen[3] == TRUE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   UART_3   allready   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   open   instance   */
   ErrorCode = TDAL_UART_ConvertBaudRate   (Connect_parameters.Baud_Rate,   &BaudRate);
   if   (ErrorCode != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   TDAL_UART_ConvertDataBits   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   ErrorCode = TDAL_UART_ConvertDataBits   (eTDAL_UART_DATA_BITS_8,
      eTDAL_UART_PARITY_NONE,   &STUART_DataBits);
   if   (ErrorCode != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   TDAL_UART_ConvertDataBits   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   ErrorCode = TDAL_UART_ConvertStopBits   (eTDAL_UART_STOP_BITS_1,   &STUART_StopBits);
   if   (ErrorCode != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   TDAL_UART_ConvertStopBits   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   ErrorCode = TDAL_UART_ConvertFlowControl(eTDAL_UART_FLOW_CTRL_NONE,   &STUART_FlowControl);
   if   (ErrorCode != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   TDAL_UART_ConvertFlowControl   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   set   default   values   */
   UartDefaultParams.RxMode.BaudRate   = BaudRate;
   UartDefaultParams.RxMode.DataBits   = STUART_DataBits;
   UartDefaultParams.RxMode.StopBits   = STUART_StopBits;
   UartDefaultParams.RxMode.FlowControl = STUART_FlowControl;
   UartDefaultParams.RxMode.TermString   = NULL;
   UartDefaultParams.RxMode.TimeOut   = 1;      /*   As   short   as   possible   */
   UartDefaultParams.RxMode.NotifyFunction = NULL;   /*   No   callback   */

   UartDefaultParams.TxMode.BaudRate   = BaudRate;
   UartDefaultParams.TxMode.DataBits   = STUART_DataBits;
   UartDefaultParams.TxMode.StopBits   = STUART_StopBits;
   UartDefaultParams.TxMode.FlowControl = STUART_FlowControl;
   UartDefaultParams.TxMode.TermString   = NULL;
   UartDefaultParams.TxMode.TimeOut   = 0;      /*   No   time-out   */
   UartDefaultParams.TxMode.NotifyFunction = NULL;   /*   No   callback   */

   UartDefaultParams.SmartCardModeEnabled = (BOOL)0;
   UartDefaultParams.GuardTime       = 0;

   UartOpenParams.LoopBackEnabled       = FALSE;
   UartOpenParams.FlushIOBuffers       = FALSE;
   UartOpenParams.DefaultParams       = &UartDefaultParams;

   for   (i = 0   ;   i   <   kMAX_ASC   ;   i++)      /*   UART   */
   {
      strcpy((char   *)UartDeviceName[i],   (char   *)SYS_UartName[i]);
   }
   UartDeviceName[i][0] = 0;

   if   (Connect_parameters.Instance == eTDAL_UART_0)
      ST_ErrorCode = STUART_Open(UartDeviceName[0],   &UartOpenParams,   &(gTDAL_uartHandle[0]));
   else   if   (Connect_parameters.Instance == eTDAL_UART_1)
      ST_ErrorCode = STUART_Open(UartDeviceName[1],   &UartOpenParams,   &(gTDAL_uartHandle[1]));
   else   if   (Connect_parameters.Instance == eTDAL_UART_2)
      ST_ErrorCode = STUART_Open(UartDeviceName[2],   &UartOpenParams,   &(gTDAL_uartHandle[2]));
   else   if   (Connect_parameters.Instance == eTDAL_UART_3)
      ST_ErrorCode = STUART_Open(UartDeviceName[3],   &UartOpenParams,   &(gTDAL_uartHandle[3]));

   if(ST_ErrorCode != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   STUART_Open   error   :   %s\n",   TBOX_GetDriverErrorText(ST_ErrorCode)));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   (Connect_parameters.Instance == eTDAL_UART_0)
      gTDAL_uartOpen[0] = TRUE;
   else   if   (Connect_parameters.Instance == eTDAL_UART_1)
      gTDAL_uartOpen[1] = TRUE;
   else   if   (Connect_parameters.Instance == eTDAL_UART_2)
      gTDAL_uartOpen[2] = TRUE;
   else   if   (Connect_parameters.Instance == eTDAL_UART_3)
      gTDAL_uartOpen[3] = TRUE;

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   ST_ErrorCode_t   ST_ErrorCode = ST_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_UART_Close");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   /*   check   if   instance   is   open   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (instance == eTDAL_UART_0)   &&
      (gTDAL_uartOpen[0] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Close   UART_0   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 1)   &&
      (instance == eTDAL_UART_1)   &&
      (gTDAL_uartOpen[1] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Close   UART_1   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 2)   &&
      (instance == eTDAL_UART_2)   &&
      (gTDAL_uartOpen[2] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Close   UART_2   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 3)   &&
      (instance == eTDAL_UART_3)   &&
      (gTDAL_uartOpen[3] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Close   UART_3   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   close   instance   */
   if   (instance == eTDAL_UART_0)
      ST_ErrorCode = STUART_Close(gTDAL_uartHandle[0]);
   else   if   (instance == eTDAL_UART_1)
      ST_ErrorCode = STUART_Close(gTDAL_uartHandle[1]);
   else   if   (instance == eTDAL_UART_2)
      ST_ErrorCode = STUART_Close(gTDAL_uartHandle[2]);
   else   if   (instance == eTDAL_UART_3)
      ST_ErrorCode = STUART_Close(gTDAL_uartHandle[3]);

   if(ST_ErrorCode != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   STUART_Close   error   :%s\n",   TBOX_GetDriverErrorText(ST_ErrorCode)));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   (instance == eTDAL_UART_0)
      gTDAL_uartOpen[0] = FALSE;
   else   if   (instance == eTDAL_UART_1)
      gTDAL_uartOpen[1] = FALSE;
   else   if   (instance == eTDAL_UART_2)
      gTDAL_uartOpen[2] = FALSE;
   else   if   (instance == eTDAL_UART_3)
      gTDAL_uartOpen[3] = FALSE;

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   ST_ErrorCode_t   ST_ErrorCode=ST_ERROR_BAD_PARAMETER;

   mTBOX_FCT_ENTER("TDAL_UART_Write");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   /*   check   if   instance   is   open   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (instance == eTDAL_UART_0)   &&
      (gTDAL_uartOpen[0] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Write   UART_0   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 1)   &&
      (instance == eTDAL_UART_1)   &&
      (gTDAL_uartOpen[1] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Write   UART_1   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 2)   &&
      (instance == eTDAL_UART_2)   &&
      (gTDAL_uartOpen[2] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Write   UART_2   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 3)   &&
      (instance == eTDAL_UART_3)   &&
      (gTDAL_uartOpen[3] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Write   UART_3   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   write   */
   if   (instance == eTDAL_UART_0)
      ST_ErrorCode = STUART_Write(gTDAL_uartHandle[0],
                 pBytesToWrite,
                 nbBytesToWrite,
                 pNbBytesWritten,
                 kTDAL_UART_WRITE_TIME_OUT);
   else   if   (instance == eTDAL_UART_1)
      ST_ErrorCode = STUART_Write(gTDAL_uartHandle[1],
                 pBytesToWrite,
                 nbBytesToWrite,
                 pNbBytesWritten,
                 kTDAL_UART_WRITE_TIME_OUT);
   else   if   (instance == eTDAL_UART_2)
      ST_ErrorCode = STUART_Write(gTDAL_uartHandle[2],
                 pBytesToWrite,
                 nbBytesToWrite,
                 pNbBytesWritten,
                 kTDAL_UART_WRITE_TIME_OUT);
   else   if   (instance == eTDAL_UART_3)
      ST_ErrorCode = STUART_Write(gTDAL_uartHandle[3],
                 pBytesToWrite,
                 nbBytesToWrite,
                 pNbBytesWritten,
                 kTDAL_UART_WRITE_TIME_OUT);
   if(ST_ErrorCode != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Write   STUART_Write   error   :   %s\n",   TBOX_GetDriverErrorText(ST_ErrorCode)));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

#if   defined(BOOTLOADER)
   /*   Bug#2980:   Workaorund   to   indicate   in   TDAL_jump(-)
      *   to   jump   via   a   reset   and   not   directly   in   application
      */
   TDAL_COMMON_bJumpWithReset = TRUE;
#endif

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   ST_ErrorCode_t   ST_ErrorCode = ST_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_UART_Read");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   /*   check   if   instance   is   open   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (instance == eTDAL_UART_0)   &&
      (gTDAL_uartOpen[0] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Read   UART_0   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 1)   &&
      (instance == eTDAL_UART_1)   &&
      (gTDAL_uartOpen[1] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Read   UART_1   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 2)   &&
      (instance == eTDAL_UART_2)   &&
      (gTDAL_uartOpen[2] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Read   UART_2   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 3)   &&
      (instance == eTDAL_UART_3)   &&
      (gTDAL_uartOpen[3] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Read   UART_3   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   read   */
   if   (instance == eTDAL_UART_0)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[0],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 kTDAL_UART_READ_TIME_OUT);
   else   if   (instance == eTDAL_UART_1)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[1],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 kTDAL_UART_READ_TIME_OUT);
   else   if   (instance == eTDAL_UART_2)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[2],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 kTDAL_UART_READ_TIME_OUT);
   else   if   (instance == eTDAL_UART_3)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[3],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 kTDAL_UART_READ_TIME_OUT);

   /*   @DEL#20060215   :                        */
   /*      Problem   timeout   return   error   even   if   there's   characters   in   buffer   */
   /*   It   could   be   a   timeout,   'caus   there's   not   enought   character   received      */
   /*   BUT   it   is   NOT   an   error                        */

   if   (   (ST_ErrorCode != ST_NO_ERROR)   &&   (ST_ErrorCode != ST_ERROR_TIMEOUT)   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Read   STUART_Read   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   ST_ErrorCode_t   ST_ErrorCode = ST_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_UART_ReadWithTimeout");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   /*   check   if   instance   is   open   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (instance == eTDAL_UART_0)   &&
      (gTDAL_uartOpen[0] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ReadWithTimeout   UART_0   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 1)   &&
      (instance == eTDAL_UART_1)   &&
      (gTDAL_uartOpen[1] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ReadWithTimeout   UART_1   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 2)   &&
      (instance == eTDAL_UART_2)   &&
      (gTDAL_uartOpen[2] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ReadWithTimeout   UART_2   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 3)   &&
      (instance == eTDAL_UART_3)   &&
      (gTDAL_uartOpen[3] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ReadWithTimeout   UART_3   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   read   */
   if   (instance == eTDAL_UART_0)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[0],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 timeoutMS);
   else   if   (instance == eTDAL_UART_1)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[1],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 timeoutMS);
   else   if   (instance == eTDAL_UART_2)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[2],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 timeoutMS);
   else   if   (instance == eTDAL_UART_3)
      ST_ErrorCode = STUART_Read(gTDAL_uartHandle[3],
                 pBytesRead,
                 nbBytesToRead,
                 pNbBytesRead,
                 timeoutMS);

   /*   @DEL#20060215   :                                 */
   /*      Problem   timeout   return   error   even   if   there's   characters   in   buffer   */
   /*   It   could   be   a   timeout,   'caus   there's   not   enought   character   received      */
   /*   BUT   it   is   NOT   an   error                              */

   if   (   (ST_ErrorCode != ST_NO_ERROR)   &&   (ST_ErrorCode != ST_ERROR_TIMEOUT)   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ReadWithTimeout   STUART_Read   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   ST_ErrorCode_t   ST_ErrorCode = ST_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_UART_Flush");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   /*   check   if   instance   is   open   */
   if   ((kTDAL_UART_NB_MAX_INSTANCE > 0)   &&
      (instance == eTDAL_UART_0)   &&
      (gTDAL_uartOpen[0] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Flush   UART_0   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 1)   &&
      (instance == eTDAL_UART_1)   &&
      (gTDAL_uartOpen[1] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Flush   UART_1   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 2)   &&
      (instance == eTDAL_UART_2)   &&
      (gTDAL_uartOpen[2] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Flush   UART_2   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   ((kTDAL_UART_NB_MAX_INSTANCE > 3)   &&
      (instance == eTDAL_UART_3)   &&
      (gTDAL_uartOpen[3] == FALSE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Flush   UART_3   not   open\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   /*   flush   instance   */
   if   (instance == eTDAL_UART_0)
      ST_ErrorCode = STUART_Flush(gTDAL_uartHandle[0]);
   else   if   (instance == eTDAL_UART_1)
      ST_ErrorCode = STUART_Flush(gTDAL_uartHandle[1]);
   else   if   (instance == eTDAL_UART_2)
      ST_ErrorCode = STUART_Flush(gTDAL_uartHandle[2]);
   else   if   (instance == eTDAL_UART_3)
      ST_ErrorCode = STUART_Flush(gTDAL_uartHandle[3]);

   if(ST_ErrorCode != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open   STUART_Flush   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   mTBOX_FCT_ENTER("TDAL_UART_GetCapabilities");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   if   (Capabilities == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_GetCapabilities   error   bad   parameters\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   Capabilities->Instance = eTDAL_UART_0   +   eTDAL_UART_1   +
               eTDAL_UART_2   +   eTDAL_UART_3;
   Capabilities->Baud_Rate = eTDAL_UART_BAUD_RATE_110   +
               eTDAL_UART_BAUD_RATE_300   +
               eTDAL_UART_BAUD_RATE_600   +
               eTDAL_UART_BAUD_RATE_1200   +
               eTDAL_UART_BAUD_RATE_2400   +
               eTDAL_UART_BAUD_RATE_4800   +
               eTDAL_UART_BAUD_RATE_9600   +
               eTDAL_UART_BAUD_RATE_14400   +
               eTDAL_UART_BAUD_RATE_19200   +
               eTDAL_UART_BAUD_RATE_38400   +
               eTDAL_UART_BAUD_RATE_57600   +
               eTDAL_UART_BAUD_RATE_115200;
   Capabilities->Data_Bits = eTDAL_UART_DATA_BITS_7   +   eTDAL_UART_DATA_BITS_8   +   eTDAL_UART_DATA_BITS_9;
   Capabilities->Stop_Bits = eTDAL_UART_STOP_BITS_1   +   eTDAL_UART_STOP_BITS_2;
   Capabilities->Parity = eTDAL_UART_PARITY_NONE   +
               eTDAL_UART_PARITY_EVEN   +
               eTDAL_UART_PARITY_ODD;
   Capabilities->Flow_Ctrl = eTDAL_UART_FLOW_CTRL_NONE   +
               eTDAL_UART_FLOW_CTRL_HARD   +
               eTDAL_UART_FLOW_CTRL_X;

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
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
   mTBOX_FCT_ENTER("TDAL_UART_ConvertBaudRate");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   if   (Baud_Rate == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ConvertBaudRate   error   bad   parameters\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_110)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_110;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_300)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_300;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_600)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_600;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_1200)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_1200;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_2400)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_2400;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_4800)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_4800;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_9600)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_9600;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_14400)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_14400;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_19200)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_19200;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_38400)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_38400;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_57600)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_57600;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (UART_Baud_Rate == eTDAL_UART_BAUD_RATE_115200)
   {
      *Baud_Rate = kTDAL_UART_BAUD_RATE_115200;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }

   mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
}

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
   mTBOX_FCT_ENTER("TDAL_UART_ConvertDataBits");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   if   (STUART_DataBits == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ConvertParameters   error   bad   parameters\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   if   (Data_Bits == eTDAL_UART_DATA_BITS_7)
   {
      if   (Parity == eTDAL_UART_PARITY_EVEN)
      {
      *STUART_DataBits = STUART_7BITS_EVEN_PARITY;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
      }
      else   if   (Parity == eTDAL_UART_PARITY_ODD)
      {
      *STUART_DataBits = STUART_7BITS_ODD_PARITY;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
      }
      else
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }
   else   if   (Data_Bits == eTDAL_UART_DATA_BITS_8)
   {
      if   (Parity == eTDAL_UART_PARITY_NONE)
      {
      *STUART_DataBits = STUART_8BITS_NO_PARITY;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
      }
      else   if   (Parity == eTDAL_UART_PARITY_EVEN)
      {
      *STUART_DataBits = STUART_8BITS_EVEN_PARITY;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
      }
      else   if   (Parity == eTDAL_UART_PARITY_ODD)
      {
      *STUART_DataBits = STUART_8BITS_ODD_PARITY;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
      }
      else
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }
   else   if   (Data_Bits == eTDAL_UART_DATA_BITS_9)
   {
      if   (Parity != eTDAL_UART_PARITY_NONE)
      {
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
      }
      else
      {
      *STUART_DataBits = STUART_9BITS_NO_PARITY;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
      }
   }

   mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
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
   mTBOX_FCT_ENTER("TDAL_UART_ConvertStopBits");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   if   (STUART_StopBits == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ConvertStopBits   error   bad   parameters\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   if   (Stop_Bits == eTDAL_UART_STOP_BITS_1)
   {
      *STUART_StopBits = STUART_STOP_1_0;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (Stop_Bits == eTDAL_UART_STOP_BITS_2)
   {
      *STUART_StopBits = STUART_STOP_2_0;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }

   mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
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
   mTBOX_FCT_ENTER("TDAL_UART_ConvertFlowControl");
#if   defined(ST_OSLINUX)
   mTBOX_RETURN(eTDAL_UART_ERROR);
#endif

   if   (STUART_FlowControl == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_ConvertFlowControl   error   bad   parameters\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   if   (Flow_Ctrl == eTDAL_UART_FLOW_CTRL_NONE)
   {
      *STUART_FlowControl = STUART_NO_FLOW_CONTROL;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (Flow_Ctrl == eTDAL_UART_FLOW_CTRL_HARD)
   {
      *STUART_FlowControl = STUART_SW_FLOW_CONTROL;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else   if   (Flow_Ctrl == eTDAL_UART_FLOW_CTRL_X)
   {
      *STUART_FlowControl = STUART_RTSCTS_FLOW_CONTROL;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
   else
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
}
