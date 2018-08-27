/**********************************************************
 * camalotslavesys.h
 *
 *  Created on: 23 oct. 2010
 *      Author: Gilles Gautier
 *   Copyright: Rabbit Labs 2010-2013
 **********************************************************/

#ifndef CAMALOTSLAVESYS_H_
#define CAMALOTSLAVESYS_H_

#ifdef __cplusplus
extern "C" {
#endif

  /* =============================================== GLOBAL ========================================================================*/
#define CAL_CURRENTVERSION 0x100

#define CAL_LOCK_BUFFERIN             1
#define CAL_LOCK_BUFFEROUT            2
#define CAL_LOCK_CIRESOURCES          3
#define CAL_LOCK_LOWSPEEDCOM_SESSION 10
#define CAL_LOCK_LOWSPEEDCOM_APDURCV 11
#define CAL_SEM_LOWSPEEDCOM_EXTNAPI  20
#define CAL_SEM_LOWSPEEDCOM_RECVDAT  21
#define CAL_SEM_LOWSPEEDCOM_HOSTFLW  22

/* ================================================== PCCARD abstraction ========================================================= */
// structure poiting to stack function that can be called by HAL
typedef struct {
  // reset received from host
  int (*fnReset_cb)(void);
  // LPDU has been sent
  int (*fnLPDUDone_cb)(void);
  // LPDU has been received from Host
  int (*fnLPDU_cb)(unsigned char *pData, int nLength);
  // size has been received from Host
  int (*fnSizeWrite_cb)(unsigned short usSize);
} CALSSys_PCCardCallBackFunctions_s;

// structure pointing to HAL functions that can be called from stack
typedef struct {
  /* This function is called by CAMaLot stack at startup to initialize low layer and register callbacks */
  int (*fnInit)(void *pSysContext, const CALSSys_PCCardCallBackFunctions_s *pCallBacks);
  /* This function is called by CAMaLot stack on exit. It shall releases all resources allocated when calling calsys_Init. If you    */
  /* plan to never call function cal_Close to close CAMaLot stack you can leave this function empty. */
  void (*fnClose)(void *pSysContext);
  /* send LPDU to host */
  /* pHeader is a buffer of size 2 to put at head of LPDU */
  /* pTrailer is a buffer of size 4 to put at end of LPDU (can be NULL) */
  int (*fnSendLPDU)(void *pSysContext, unsigned char *pHeader, int nHeaderSize, unsigned char *pData, int nLength, unsigned char *pTrailer);
  /* signal that an LPDU is pending for sending (used for local polling) */
  int (*fnSignalLDPUPending)(void *pSysContext);
} CALSSys_PCCardHalFunctions_s;

/* ================================================== Serial Port abstraction ========================================================= */
// structure poiting to stack function that can be called by serial HAL
typedef struct {
  // some data has been received from serial port
  int(*fnDataReceived_cb)(void *pTransportContext, unsigned char *pData, int nLength);
  // send is complete (to tell the stack the data sent by fnSend have been send out to hardware)
  int(*fnSendComplete_cb)(void *pTransportContext);
} CALSSys_SerialCallBackFunctions_s;

// structure pointing to serial HAL functions that can be called from stack
typedef struct {
  /* This function is called by CAMaLot stack at startup to initialize low layer and register callbacks */
  int(*fnOpen)(void *pTransportContext, void *pDriverContext, const CALSSys_SerialCallBackFunctions_s *pCallBacks);
  /* This function is called by CAMaLot stack on exit. It shall releases all resources allocated when calling calsys_Init. If you    */
  /* plan to never call function cal_Close to close CAMaLot stack you can leave this function empty. */
  void(*fnClose)(void *pDriverContext);
  /* send serial data to host return amount of data sent */
  int(*fnSend)(void *pDriverContext, unsigned char *pData, int nLength);
} CALSSys_SerialHalFunctions_s;

/* ================================================== USB abstraction ========================================================= */
// structure poiting to stack function that can be called by USB HAL
typedef struct {
  // some data has been received from USB endpoint
  int(*fnDataReceived_cb)(void *pTransportContext, unsigned char *pData, int nDataSize);
  // send is complete (to tell the stack the data sent by fnSend have been send out to hardware, read by Host and that buffer is ready for next transfer)
  int(*fnSendComplete_cb)(void *pTransportContext);
  // interface is up
  int(*fnInterfaceUp_cb)(void *pTransportContext, int nEndpointSize);
  // interface is down
  int(*fnInterfaceDown_cb)(void *pTransportContext);
} CALSSys_USBCallBackFunctions_s;

// structure pointing to USB HAL functions that can be called from stack
typedef struct {
  /* This function is called by CAMaLot stack at startup to initialize low layer, register callbacks and return endpoint size*/
  int(*fnOpen)(void *pTransportContext, void *pDriverContext, const CALSSys_USBCallBackFunctions_s *pCallBacks);
  /* This function is called by CAMaLot stack on exit. It shall releases all resources allocated when calling calsys_Init. If you    */
  /* plan to never call function cal_Close to close CAMaLot stack you can leave this function empty. */
  void(*fnClose)(void *pDriverContext);
  /* send serial data to host return amount of data sent, maximum write size is end point size, a size of 0 is allowed to terminate a buffer
  in that case a 0 size buffer must be sent over the interface */
  int(*fnSend)(void *pDriverContext, unsigned char *pData, int nLength);
} CALSSys_USBHalFunctions_s;

/* ================================================== OS abstraction ========================================================= */

// structure pointing to OS HAL functions that can be called from stack
typedef struct {
  /* initialize locks */
  int(*fnInitializeLocks)(void *pSysContext);
  /* lock resources when required */
  int(*fnAcquireLock)(void *pSysContext, int nLockIdx);
  /* unlock resource */
  int(*fnReleaseLock)(void *pSysContext, int nLockIdx);
  /* semaphore wait */
  int(*fnSemaphoreWait)(void *pSysContext, int nSemIdx, unsigned long ulMillis);
  /* semaphore post */
  int(*fnSemaphorePost)(void *pSysContext, int nSemIdx);
} CALSSys_OSHalFunctions_s;


#ifdef __cplusplus
}
#endif

#endif /* CAMALOTSLAVESYS_H_ */
