 /*******************************************************************************
 *                 COPYRIGHT 2004 IWEDIA                        *
 ********************************************************************************
 *
 * MODULE NAME: TDAL_UART
 *
 * FILE NAME: $Workfile:   tdal_uart_stlinux.c  $
 *        $Revision: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description           : UART HAL implementation
 * Target              : Linux
 * Author              : Julien Flatrès
 * Date of creation      : 16 / 11 / 2004.
 *
 *******************************************************************************/

/********************************************************/
/*            Includes                     */
/********************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <strings.h>

#include "tbox.h"
#include "tdal_common.h"

#include "tdal_uart.h"


/********************************************************/
/*        Defines                   */
/********************************************************/
#define   TDAL_UART_WITH_TTY_USB0      1
#define   kINVALID_INDEX      0xFF




mTBOX_SET_MODULE(eTDAL_UART);


/********************************************************/
/*        Local   File   Variables   (LOCAL)      */
/********************************************************/
LOCAL   int16_t      gTDAL_uartHandle[kTDAL_UART_NB_MAX_INSTANCE];

LOCAL   bool      gTDAL_uartInitialized   = FALSE;
LOCAL   bool      gTDAL_uartOpen[kTDAL_UART_NB_MAX_INSTANCE];

LOCAL   struct   termios   oldtio[kTDAL_UART_NB_MAX_INSTANCE];
LOCAL   struct   termios   newtio[kTDAL_UART_NB_MAX_INSTANCE];

LOCAL   char   *      gTDAL_devFilename[kTDAL_UART_NB_MAX_INSTANCE] = {
            "/dev/ttyS0"   ,
            "/dev/ttyS1"   ,
            #if   TDAL_UART_WITH_TTY_USB0
            "/dev/ttyUSB0"   ,
            #else
            NULL   ,
            #endif   /*   TDAL_UART_WITH_TTY_USB0   */
            NULL
            };

/********************************************************/
/*        Functions   Definitions   (LOCAL/GLOBAL)   */
/********************************************************/
LOCAL   uint8_t        TDAL_UARTi_GetIndexFromInstance(   tTDAL_UART_Instance   instance   );
LOCAL   tTDAL_UART_Instance   TDAL_UARTi_GetInstanceFromIndex(   uint8_t   index   );

/*===========================================================================
 *
 * TDAL_UART_Init
 *
 * Parameters:
 *
 * Description:
 *      initialize the TDAL UART module
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *      eTDAL_UART_NOT_DONE       command not done
 *
 *===========================================================================*/


tTDAL_UART_ErrorCode   TDAL_UART_Init(   void   )
{
   uint8_t   i;

   mTBOX_FCT_ENTER("TDAL_UART_Init");

   /*   Test   if   TDAL_UART   has   already   been   initialized   */
   if   (gTDAL_uartInitialized == TRUE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Init   error :   already   initialized\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   for   (i = 0   ;   i   <   kTDAL_UART_NB_MAX_INSTANCE   ;   i++)
      gTDAL_uartOpen[i] = FALSE;

   gTDAL_uartInitialized = TRUE;

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_UART_Terminate
 *
 * Parameters:
 *
 * Description:
 *      terminate the TDAL UART module
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *      eTDAL_UART_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_UART_ErrorCode   TDAL_UART_Terminate(void)
{
   bool            ErrorFound;
   tTDAL_UART_ErrorCode   ErrorCode;
   uint8_t        ucIndex;

   mTBOX_FCT_ENTER("TDAL_UART_Terminate");

   /*   Test   if   TDAL_UART   is   still   initialised   */
   if   (gTDAL_uartInitialized == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate   error :   already   terminated\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   ErrorFound = FALSE;
      
   /*   close   open   instances   */
      for(   ucIndex=0   ;   ucIndex<kTDAL_UART_NB_MAX_INSTANCE   ;   ucIndex++   )
      {
      if   (   gTDAL_uartOpen[ucIndex] == TRUE   )
      {
        ErrorCode = TDAL_UART_Close(   TDAL_UARTi_GetInstanceFromIndex(ucIndex)   );
        if   (ErrorCode != eTDAL_UART_NO_ERROR)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Terminate:   TDAL_UART_Close   UART_%d   error\n",   ucIndex));
            ErrorFound = TRUE;
        }
        gTDAL_uartOpen[ucIndex] = FALSE;
      }
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


/*===========================================================================
 *
 * TDAL_UART_Open
 *
 * Parameters:
 *      instance   instance of UART to open
 *
 * Description:
 *      open an instance of the Uart device driver
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *      eTDAL_UART_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_UART_ErrorCode   TDAL_UART_Open   (tTDAL_UART_Connect_parameters   Connect_parameters)
{
   int16_t   errorCode=0;
   tTDAL_UART_Connect_parameters   Capabilities;
   uint8_t   handleIndex;

   mTBOX_FCT_ENTER("TDAL_UART_Open");

   /*   test   if   TDAL   UART   has   been   initialized   */
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
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Bad   instance\n"));
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
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Bad   Baud_Rate\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Data_Bits   */
   if   ((Connect_parameters.Data_Bits == 0)   ||
      ((Connect_parameters.Data_Bits != eTDAL_UART_DATA_BITS_7)   &&
      (Connect_parameters.Data_Bits != eTDAL_UART_DATA_BITS_8)   &&
      (Connect_parameters.Data_Bits != eTDAL_UART_DATA_BITS_9)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Bad   Data_Bits\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Stop_Bits   */
   if   ((Connect_parameters.Stop_Bits == 0)   ||
      ((Connect_parameters.Stop_Bits != eTDAL_UART_STOP_BITS_1)   &&
      (Connect_parameters.Stop_Bits != eTDAL_UART_STOP_BITS_2)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Bad   Stop_Bits\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Parity   */
   if   ((Connect_parameters.Parity == 0)   ||
      ((Connect_parameters.Parity != eTDAL_UART_PARITY_NONE)   &&
      (Connect_parameters.Parity != eTDAL_UART_PARITY_EVEN)   &&
      (Connect_parameters.Parity != eTDAL_UART_PARITY_ODD)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Bad   Parity\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   Flow_Ctrl   */
   if   ((Connect_parameters.Flow_Ctrl == 0)   ||
      ((Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_NONE)   &&
      (Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_HARD)   &&
      (Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_X)))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Bad   Flow_Ctrl\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   check   capabilities   */
   if   (TDAL_UART_GetCapabilities(&Capabilities) != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   TDAL_UART_GetCapabilities   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   if   (((Capabilities.Instance & Connect_parameters.Instance) != Connect_parameters.Instance)   ||
      ((Capabilities.Baud_Rate & Connect_parameters.Baud_Rate) != Connect_parameters.Baud_Rate)   ||
      ((Capabilities.Data_Bits & Connect_parameters.Data_Bits) != Connect_parameters.Data_Bits)   ||
      ((Capabilities.Stop_Bits & Connect_parameters.Stop_Bits) != Connect_parameters.Stop_Bits)   ||
      ((Capabilities.Parity & Connect_parameters.Parity) != Connect_parameters.Parity)   ||
      ((Capabilities.Flow_Ctrl & Connect_parameters.Flow_Ctrl) != Connect_parameters.Flow_Ctrl))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   Capability   override\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }
   
   /*   specific   limitation   of   our   Linux   implementation   */
   if   (   Connect_parameters.Flow_Ctrl != eTDAL_UART_FLOW_CTRL_NONE   )
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_UART_Open() :   Flow   Control   is   not   managed   on   Linux   OS\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }
   else   if   (   Connect_parameters.Data_Bits == eTDAL_UART_DATA_BITS_9   )
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_UART_Open() :   9   data   bits   is   not   managed   on   Linux   OS\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }
   else   if   (   Connect_parameters.Baud_Rate == eTDAL_UART_BAUD_RATE_14400   )
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_UART_Open() :   Baud   rate   14400   is   not   managed   on   Linux   OS\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }


   handleIndex = TDAL_UARTi_GetIndexFromInstance(   Connect_parameters.Instance   );

   /*   check   if   instance   is   valid   and   already   opened   */
   if   (   handleIndex == kINVALID_INDEX   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   invalid   instance=%d\n",   TBOX_FunctionName,   Connect_parameters.Instance));
      mTBOX_RETURN(   eTDAL_UART_BAD_PARAMETER   );
   }
   else   if(   handleIndex >= kTDAL_UART_NB_MAX_INSTANCE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   handleIndex=%d   too   big\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }
   else   if(   gTDAL_uartOpen[handleIndex] == TRUE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   UART[%d]   not   opened\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }

   /*   open   an   instance   of   UART   driver   */
   if   (   gTDAL_devFilename[handleIndex] == NULL   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   UART   %d   not   managed   on   Linux   OS\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }
   
   gTDAL_uartHandle[handleIndex] = open(   gTDAL_devFilename[handleIndex],   O_RDWR   |   O_NOCTTY   |   O_NDELAY);
   if   (gTDAL_uartHandle[handleIndex]   <   0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   open_port:   Unable   to   open   %s\n",   TBOX_FunctionName,   gTDAL_devFilename[handleIndex]));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   errorCode+=fcntl(gTDAL_uartHandle[handleIndex],   F_SETFL,   0);

   /*   Save   the   current   UART   config   */
   errorCode+=tcgetattr(gTDAL_uartHandle[handleIndex],   &oldtio[handleIndex]);
      if(errorCode<0)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Open() :   Impossible   to   save   the   current   UART   config\n"));
      }
   /*   Reset   struct   */
   bzero(&newtio[handleIndex],   sizeof(newtio[handleIndex]));

   newtio[handleIndex].c_cflag |= (CLOCAL   |   CREAD);

   switch   (Connect_parameters.Baud_Rate)
   {
      case   eTDAL_UART_BAUD_RATE_110 :   cfsetispeed(&newtio[handleIndex],   B110);
                 break;
      case   eTDAL_UART_BAUD_RATE_300 :   cfsetispeed(&newtio[handleIndex],   B300);
                 break;
      case   eTDAL_UART_BAUD_RATE_600 :   cfsetispeed(&newtio[handleIndex],   B600);
                 break;
      case   eTDAL_UART_BAUD_RATE_1200 :   cfsetispeed(&newtio[handleIndex],   B1200);
                 break;
      case   eTDAL_UART_BAUD_RATE_2400 :   cfsetispeed(&newtio[handleIndex],   B2400);
                 break;
      case   eTDAL_UART_BAUD_RATE_4800 :   cfsetispeed(&newtio[handleIndex],   B4800);
                 break;
      case   eTDAL_UART_BAUD_RATE_9600 :   cfsetispeed(&newtio[handleIndex],   B9600);
                 break;
      case   eTDAL_UART_BAUD_RATE_14400 :   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Open() :   Baud   rate   14400   is   not   managed   on   Linux   OS\n"));
                 mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
      case   eTDAL_UART_BAUD_RATE_19200 :   cfsetispeed(&newtio[handleIndex],   B19200);
                 break;
      case   eTDAL_UART_BAUD_RATE_38400 :   cfsetispeed(&newtio[handleIndex],   B38400);
                 break;
      case   eTDAL_UART_BAUD_RATE_57600 :   cfsetispeed(&newtio[handleIndex],   B57600);
                 break;
      case   eTDAL_UART_BAUD_RATE_115200 :   cfsetispeed(&newtio[handleIndex],   B115200);
                 break;
   }

   switch   (Connect_parameters.Parity)
   {
      case   eTDAL_UART_PARITY_NONE :   newtio[handleIndex].c_cflag &= ~PARENB;   /*   No   parity   */
                 break;

      case   eTDAL_UART_PARITY_EVEN :   newtio[handleIndex].c_cflag |= PARENB;   /*   Even   parity   */
                 newtio[handleIndex].c_cflag &= ~PARODD;
                 newtio[handleIndex].c_iflag |= INPCK;   /*   Check   parity   */
                 break;

      case   eTDAL_UART_PARITY_ODD :   newtio[handleIndex].c_cflag |= PARENB;   /*   Odd   parity   */
                 newtio[handleIndex].c_cflag |= PARODD;
                 newtio[handleIndex].c_iflag |= INPCK;   /*   Check   parity   */
                 break;
   }

   switch   (Connect_parameters.Stop_Bits)
   {
      case   eTDAL_UART_STOP_BITS_1 :   newtio[handleIndex].c_cflag &= ~CSTOPB;   /*   1   stop   bit   */
                 break;
      case   eTDAL_UART_STOP_BITS_2 :   newtio[handleIndex].c_cflag |= CSTOPB;   /*   2   stop   bits   */
                 break;
   }

   switch   (Connect_parameters.Data_Bits)
   {
      case   eTDAL_UART_DATA_BITS_7 :   newtio[handleIndex].c_cflag &= ~CSIZE;
                 newtio[handleIndex].c_cflag |= CS7;   /*   7   data   bits   */
                 break;
      case   eTDAL_UART_DATA_BITS_8 :   newtio[handleIndex].c_cflag &= ~CSIZE;
                 newtio[handleIndex].c_cflag |= CS8;   /*   8   data   bits   */
                 break;
      case   eTDAL_UART_DATA_BITS_9 :   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Open() :   9   data   bits   is   not   managed   on   Linux   OS\n"));
                 mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
                 break;
   }

   switch   (Connect_parameters.Flow_Ctrl)
   {
      case   eTDAL_UART_FLOW_CTRL_NONE :   break;
      case   eTDAL_UART_FLOW_CTRL_HARD :
      case   eTDAL_UART_FLOW_CTRL_X :mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_UART_Open() :   Flow   Control   is   not   managed   on   Linux   OS\n"));
               mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   /*   Output   without   processing   (RAW)   */
   newtio[handleIndex].c_oflag = 0;

   /*   Disable   canonical   mode,   no   echo   ...   */
   newtio[handleIndex].c_lflag = 0;

   /*   Minimum   number   of   char   to   read   set   to   1   */
   newtio[handleIndex].c_cc[VMIN] = 1;

   /*   Data   ready   immediately   */
   newtio[handleIndex].c_cc[VTIME] = 0;

   /*   Empty   the   modem   line   and   set   the   UART   config   */
   errorCode+=tcflush(gTDAL_uartHandle[handleIndex],   TCIFLUSH);
   if(errorCode<0)
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Open() :   Impossible   to   flush   the   input   UART   buffer\n"));
   errorCode+=tcflush(gTDAL_uartHandle[handleIndex],   TCOFLUSH);
   if(errorCode<0)
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Open() :   Impossible   to   flush   the   output   UART   buffer\n"));

   errorCode+=tcsetattr(gTDAL_uartHandle[handleIndex],TCSANOW,&newtio[handleIndex]);
   if   (errorCode<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Open() :   Impossible   to   et   the   new   UART   config\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }
   else
   {
      gTDAL_uartOpen[handleIndex] = TRUE;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }
}

/*===========================================================================
 *
 * TDAL_UART_Close
 *
 * Parameters:
 *      instance   instance of UART to close
 *
 * Description:
 *      close an instance of the driver opened by TDAL_UART_Open
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *      eTDAL_UART_NOT_DONE       command not done
 *
 *===========================================================================*/

tTDAL_UART_ErrorCode   TDAL_UART_Close   (tTDAL_UART_Instance   instance)
{
   int16_t   errorCode=0;
   uint8_t   handleIndex;   

   mTBOX_FCT_ENTER("TDAL_UART_Close");

   /*   Test   if   TDAL   UART   has   been   initialized   */
   if   (!gTDAL_uartInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_UART   not   initialized\n",   TBOX_FunctionName));
      mTBOX_RETURN(eTDAL_UART_NOT_DONE);
   }

   handleIndex = TDAL_UARTi_GetIndexFromInstance(   instance   );
   
   /*   check   if   instance   is   valid   and   opened   */
   if   (   handleIndex == kINVALID_INDEX   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   invalid   instance=%d\n",   TBOX_FunctionName,   instance));
      mTBOX_RETURN(   eTDAL_UART_BAD_PARAMETER   );
   }
   else   if(   handleIndex >= kTDAL_UART_NB_MAX_INSTANCE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   handleIndex=%d   too   big\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }
   else   if(   gTDAL_uartOpen[handleIndex] == FALSE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   UART[%d]   not   opened\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }


   /*   Wait   until   everything   has   been   trasnmitted   to   recover   the   old   params   */
   errorCode+=tcsetattr(gTDAL_uartHandle[handleIndex],TCSADRAIN,&oldtio[handleIndex]);
   errorCode+=close(gTDAL_uartHandle[handleIndex]);

   if   (errorCode<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Close()   -   Error :   Unable   to   close   file\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   gTDAL_uartOpen[handleIndex] = FALSE;

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);

}


/*===========================================================================
 *
 * TDAL_UART_Read
 *
 * Parameters:
 *      instance       instance of UART to read from
 *      pBytesRead    pointer to the buffer used to store data from the serial port
 *      nbBytesToRead  number of bytes to read
 *      pNbBytesRead   number of bytes read
 *
 * Description:
 *      read bytes from the serial port
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *      eTDAL_UART_NOT_DONE       command not done
 *
 *===========================================================================*/

tTDAL_UART_ErrorCode   TDAL_UART_Read(
   tTDAL_UART_Instance   instance,
   uint8_t*   pBytesRead,
   uint32_t   nbBytesToRead,
   uint32_t*   pNbBytesRead)
{
   int32_t   NbBytesRead=0;
   uint8_t   handleIndex;
   int   i = 0;

   mTBOX_FCT_ENTER("TDAL_UART_Read");

   /*   Test   if   TDAL   UART   has   been   initialized   */
   if   (!gTDAL_uartInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_UART   not   initialized\n",   TBOX_FunctionName));
      mTBOX_RETURN(eTDAL_UART_NOT_DONE);
   }

   handleIndex = TDAL_UARTi_GetIndexFromInstance(   instance   );
   
   /*   check   if   instance   is   valid   and   opened   */
   if   (   handleIndex == kINVALID_INDEX   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   invalid   instance=%d\n",   TBOX_FunctionName,   instance));
      mTBOX_RETURN(   eTDAL_UART_BAD_PARAMETER   );
   }
   else   if(   handleIndex >= kTDAL_UART_NB_MAX_INSTANCE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   handleIndex=%d   too   big\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }
   else   if(   gTDAL_uartOpen[handleIndex] == FALSE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   UART[%d]   not   opened\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }


   /*   read   */
   //NbBytesRead=read(gTDAL_uartHandle[handleIndex],pBytesRead,nbBytesToRead);

   while(i   <   nbBytesToRead)
   {
      NbBytesRead=read(gTDAL_uartHandle[handleIndex],&pBytesRead[i],1);
      if   (NbBytesRead==-1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Read()   -   Error :   Unable   to   read   the   UART   buffer\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
      }
      i++;
   }

   *pNbBytesRead=NbBytesRead;
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
   tTDAL_UART_ErrorCode      eReadErr;

   mTBOX_FCT_ENTER("TDAL_UART_ReadWithTimeout");

   /*   just   call   TDAL_UART_Read()   for   the   moment   ???   */
   eReadErr = TDAL_UART_Read(instance,pBytesRead,nbBytesToRead,pNbBytesRead);
   if   (   eReadErr != eTDAL_UART_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_UART_Read()   failed,   eReadErr=%d\n",   TBOX_FunctionName,   eReadErr));
   }

   mTBOX_RETURN(   eReadErr   );
}


/*===========================================================================
 *
 * TDAL_UART_Write
 *
 * Parameters:
 *      instance       instance of UART to write to
 *      pBytesToWrite   pointer to the buffer to send to serial port
 *      nbBytesToWrite  number of bytes to write
 *      pNbBytesWritten number of bytes written
 *
 * Description:
 *      write bytes on the serial port
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *      eTDAL_UART_NOT_DONE       command not done
 *
 *===========================================================================*/

tTDAL_UART_ErrorCode   TDAL_UART_Write(
   tTDAL_UART_Instance   instance,
   uint8_t*   pBytesToWrite,
   uint32_t   nbBytesToWrite,
   uint32_t*   pNbBytesWritten)
{
   int32_t   NbBytesWritten=0;
   uint8_t   handleIndex;

   mTBOX_FCT_ENTER("TDAL_UART_Write");

   /*   Test   if   TDAL   UART   has   been   initialized   */
   if   (!gTDAL_uartInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_UART   not   initialized\n",   TBOX_FunctionName));
      mTBOX_RETURN(eTDAL_UART_NOT_DONE);
   }

   handleIndex = TDAL_UARTi_GetIndexFromInstance(   instance   );
   
   /*   check   if   instance   is   valid   and   opened   */
   if   (   handleIndex == kINVALID_INDEX   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   invalid   instance=%d\n",   TBOX_FunctionName,   instance));
      mTBOX_RETURN(   eTDAL_UART_BAD_PARAMETER   );
   }
   else   if(   handleIndex >= kTDAL_UART_NB_MAX_INSTANCE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   handleIndex=%d   too   big\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }
   else   if(   gTDAL_uartOpen[handleIndex] == FALSE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   UART[%d]   not   opened\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }

   tcflush(gTDAL_uartHandle[handleIndex],   TCOFLUSH);

   /*   write   */
   NbBytesWritten=write(gTDAL_uartHandle[handleIndex],pBytesToWrite,nbBytesToWrite);

   if   (NbBytesWritten==-1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Write()   -   Error :   Unable   to   write   in   the   UART   buffer\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }
   else   if   (nbBytesToWrite!=NbBytesWritten)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_UART_Write()   -   Error :   nbBytesToWrite!=NbBytesWritten\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }
   else
   {
      *pNbBytesWritten=NbBytesWritten;
      mTBOX_RETURN(eTDAL_UART_NO_ERROR);
   }


}


/*===========================================================================
 *
 * TDAL_UART_GetCapabilities
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *
 *===========================================================================*/

tTDAL_UART_ErrorCode   TDAL_UART_GetCapabilities(tTDAL_UART_Connect_parameters   *Capabilities)
{
   mTBOX_FCT_ENTER("TDAL_UART_GetCapabilities");

   if   (Capabilities == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_GetCapabilities   error   bad   parameters\n"));
      mTBOX_RETURN(eTDAL_UART_BAD_PARAMETER);
   }

   Capabilities->Instance = eTDAL_UART_0 + eTDAL_UART_1;
   #if   TDAL_UART_WITH_TTY_USB0
   Capabilities->Instance   +=   eTDAL_UART_2;
   #endif   /*   TDAL_UART_WITH_TTY_USB0   */
   
   Capabilities->Baud_Rate = eTDAL_UART_BAUD_RATE_110
              + eTDAL_UART_BAUD_RATE_300
              + eTDAL_UART_BAUD_RATE_600
              + eTDAL_UART_BAUD_RATE_1200
              + eTDAL_UART_BAUD_RATE_2400
              + eTDAL_UART_BAUD_RATE_4800
              + eTDAL_UART_BAUD_RATE_9600
               /*+   eTDAL_UART_BAUD_RATE_14400*/
              + eTDAL_UART_BAUD_RATE_19200
              + eTDAL_UART_BAUD_RATE_38400
              + eTDAL_UART_BAUD_RATE_57600
              + eTDAL_UART_BAUD_RATE_115200;
   Capabilities->Data_Bits = eTDAL_UART_DATA_BITS_7
              + eTDAL_UART_DATA_BITS_8;
               /* + eTDAL_UART_DATA_BITS_9;*/
   Capabilities->Stop_Bits = eTDAL_UART_STOP_BITS_1 + eTDAL_UART_STOP_BITS_2;
   Capabilities->Parity = eTDAL_UART_PARITY_NONE
              + eTDAL_UART_PARITY_EVEN
              + eTDAL_UART_PARITY_ODD;
   Capabilities->Flow_Ctrl = eTDAL_UART_FLOW_CTRL_NONE;
               /*+   eTDAL_UART_FLOW_CTRL_HARD
              + eTDAL_UART_FLOW_CTRL_X;*/

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
}


/*===========================================================================
 *
 * TDAL_UART_Flush
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *      eTDAL_UART_NO_ERROR       no error
 *      eTDAL_UART_BAD_PARAMETER  wrong argument value
 *
 *===========================================================================*/

tTDAL_UART_ErrorCode   TDAL_UART_Flush   (tTDAL_UART_Instance   instance)
{
   int16_t   errorCode=0;
   uint8_t   handleIndex;

   mTBOX_FCT_ENTER("TDAL_UART_Flush");

   /*   Test   if   TDAL   UART   has   been   initialized   */
   if   (!gTDAL_uartInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_UART   not   initialized\n",   TBOX_FunctionName));
      mTBOX_RETURN(eTDAL_UART_NOT_DONE);
   }

   handleIndex = TDAL_UARTi_GetIndexFromInstance(   instance   );
   
   /*   check   if   instance   is   valid   and   opened   */
   if   (   handleIndex == kINVALID_INDEX   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   invalid   instance=%d\n",   TBOX_FunctionName,   instance));
      mTBOX_RETURN(   eTDAL_UART_BAD_PARAMETER   );
   }
   else   if(   handleIndex >= kTDAL_UART_NB_MAX_INSTANCE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   handleIndex=%d   too   big\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }
   else   if(   gTDAL_uartOpen[handleIndex] == FALSE   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "%s:   UART[%d]   not   opened\n",   TBOX_FunctionName,   handleIndex));
      mTBOX_RETURN(   eTDAL_UART_ERROR   );
   }

   /*   flush   instance   */
   errorCode+=tcflush(gTDAL_uartHandle[handleIndex],   TCIFLUSH);
   errorCode+=tcflush(gTDAL_uartHandle[handleIndex],   TCOFLUSH);

   if   (errorCode<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "TDAL_UART_Open()   STUART_Flush   error\n"));
      mTBOX_RETURN(eTDAL_UART_ERROR);
   }

   mTBOX_RETURN(eTDAL_UART_NO_ERROR);
}





/*===========================================================================
 *
 * TDAL_UARTi_GetIndexFromInstance
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *      the index       no error
 *      -1           wrong argument value
 *
 *===========================================================================*/
LOCAL   uint8_t   TDAL_UARTi_GetIndexFromInstance(   tTDAL_UART_Instance   instance   )
{
   switch(   instance   )
   {
      case   eTDAL_UART_0:      return   0;
      case   eTDAL_UART_1:      return   1;
      case   eTDAL_UART_2:      return   2;
      case   eTDAL_UART_3:      return   3;
      default:            return   kINVALID_INDEX;
   }
}

/*===========================================================================
 *
 * TDAL_UARTi_GetInstanceFromIndex
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *      the instance
 *
 *===========================================================================*/
LOCAL   tTDAL_UART_Instance   TDAL_UARTi_GetInstanceFromIndex(   uint8_t   index   )
{
   switch(   index   )
   {
      case   0:      return   eTDAL_UART_0;
      case   1:      return   eTDAL_UART_1;
      case   2:      return   eTDAL_UART_2;
      case   3:      return   eTDAL_UART_3;
   }
}

