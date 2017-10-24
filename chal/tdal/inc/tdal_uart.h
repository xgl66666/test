/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL UART
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_uart.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL UART
 *
 *****************************************************************************/

#ifndef _TDAL_UART_H_
#define _TDAL_UART_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/
#define kTDAL_UART_NB_MAX_INSTANCE  4

#define kTDAL_UART_READ_TIME_OUT   2000   /* in ms -> 10 sec */
#define kTDAL_UART_WRITE_TIME_OUT   2000   /* in ms -> 2 sec */

#define kTDAL_UART_BUFFER_SIZE   4096
#define kTDAL_UART_HIGH_LVL_THD   90
#define kTDAL_UART_LOW_LVL_THD   10
#define kTDAL_UART_XFER_MODE    tmdlUartXferModeAsynchronous
#define kTDAL_UART_LOOP_BACK_MODE   tmdlUartLoopBackModeNo

#define kTDAL_UART_BAUD_RATE_110   110
#define kTDAL_UART_BAUD_RATE_300   300
#define kTDAL_UART_BAUD_RATE_600   600
#define kTDAL_UART_BAUD_RATE_1200   1200
#define kTDAL_UART_BAUD_RATE_2400   2400
#define kTDAL_UART_BAUD_RATE_4800   4800
#define kTDAL_UART_BAUD_RATE_9600   9600
#define kTDAL_UART_BAUD_RATE_14400  14400
#define kTDAL_UART_BAUD_RATE_19200  19200
#define kTDAL_UART_BAUD_RATE_38400  38400
#define kTDAL_UART_BAUD_RATE_57600  57600
#define kTDAL_UART_BAUD_RATE_115200 115200

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum
{
   eTDAL_UART_NO_ERROR,
   eTDAL_UART_NOT_DONE,
   eTDAL_UART_BAD_PARAMETER,
   eTDAL_UART_ERROR
}tTDAL_UART_ErrorCode;

typedef enum
{
   eTDAL_UART_0 = 0x01,
   eTDAL_UART_1 = 0x02,
   eTDAL_UART_2 = 0x04,
   eTDAL_UART_3 = 0x08
}tTDAL_UART_Instance;

typedef enum
{
   eTDAL_UART_BAUD_RATE_110   = 0x0001,
   eTDAL_UART_BAUD_RATE_300   = 0x0002,
   eTDAL_UART_BAUD_RATE_600   = 0x0004,
   eTDAL_UART_BAUD_RATE_1200   = 0x0008,
   eTDAL_UART_BAUD_RATE_2400   = 0x0010,
   eTDAL_UART_BAUD_RATE_4800   = 0x0020,
   eTDAL_UART_BAUD_RATE_9600   = 0x0040,
   eTDAL_UART_BAUD_RATE_14400  = 0x0080,
   eTDAL_UART_BAUD_RATE_19200  = 0x0100,
   eTDAL_UART_BAUD_RATE_38400  = 0x0200,
   eTDAL_UART_BAUD_RATE_57600  = 0x0400,
   eTDAL_UART_BAUD_RATE_115200 = 0x0800
}tTDAL_UART_Baud_Rate;

typedef enum
{
   eTDAL_UART_DATA_BITS_7   = 0x1,
   eTDAL_UART_DATA_BITS_8   = 0x2,
   eTDAL_UART_DATA_BITS_9   = 0x4
}tTDAL_UART_Data_Bits;

typedef enum
{
   eTDAL_UART_STOP_BITS_1   = 0x1,
   eTDAL_UART_STOP_BITS_2   = 0x2
}tTDAL_UART_Stop_Bits;

typedef enum
{
   eTDAL_UART_PARITY_NONE   = 0x1,
   eTDAL_UART_PARITY_EVEN   = 0x2,
   eTDAL_UART_PARITY_ODD    = 0x4
}tTDAL_UART_Parity;

typedef enum
{
   eTDAL_UART_FLOW_CTRL_NONE   = 0x1,
   eTDAL_UART_FLOW_CTRL_HARD   = 0x2,
   eTDAL_UART_FLOW_CTRL_X   = 0x4
}tTDAL_UART_Flow_Ctrl;

typedef struct
{
   tTDAL_UART_Instance  Instance;
   tTDAL_UART_Baud_Rate Baud_Rate;
   tTDAL_UART_Data_Bits Data_Bits;
   tTDAL_UART_Stop_Bits Stop_Bits;
   tTDAL_UART_Parity   Parity;
   tTDAL_UART_Flow_Ctrl Flow_Ctrl;
}tTDAL_UART_Connect_parameters;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_UART_ErrorCode TDAL_UART_Init ( void );
tTDAL_UART_ErrorCode TDAL_UART_Terminate ( void );
tTDAL_UART_ErrorCode TDAL_UART_Open (tTDAL_UART_Connect_parameters  Connect_parameters);
tTDAL_UART_ErrorCode TDAL_UART_Close (tTDAL_UART_Instance  instance);
tTDAL_UART_ErrorCode TDAL_UART_Write(tTDAL_UART_Instance  instance,
   uint8_t*  pBytesToWrite,
   uint32_t  nbBytesToWrite,
   uint32_t* pNbBytesWritten);
tTDAL_UART_ErrorCode TDAL_UART_Read(tTDAL_UART_Instance  instance,
   uint8_t*  pBytesRead,
   uint32_t  nbBytesToRead,
   uint32_t* pNbBytesRead);
tTDAL_UART_ErrorCode TDAL_UART_ReadWithTimeout (tTDAL_UART_Instance  instance,
                     uint8_t*  pBytesRead,
                     uint32_t  nbBytesToRead,
                     uint32_t* pNbBytesRead,
                     uint32_t  timeoutMS);
tTDAL_UART_ErrorCode TDAL_UART_Flush (tTDAL_UART_Instance  instance);
tTDAL_UART_ErrorCode TDAL_UART_GetCapabilities (tTDAL_UART_Connect_parameters  *Capabilities);

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_UART_H_*/
