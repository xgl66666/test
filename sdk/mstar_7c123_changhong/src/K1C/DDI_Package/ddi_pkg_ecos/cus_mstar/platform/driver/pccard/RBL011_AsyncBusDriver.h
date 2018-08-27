#ifndef _RBL011_ASYNCBUSDRIVER_H_
#define _RBL011_ASYNCBUSDRIVER_H_

#include <camalotslavesys.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RBL011_BASEADD
//#error RBL011_BASEADD must be defined
#define mstar_pm_base              0xBF000000
#define REG_PCM_SLAVE_APB_BANK            0x1037UL
#define REG_PCM_SLAVE_BANK            0x1038UL
#define RBL011_BASEADD             mstar_pm_base + ((REG_PCM_SLAVE_APB_BANK*0x100UL)*2)
#endif


#ifndef RBL011_STRIDE
#define RBL011_STRIDE 1
#endif

#ifndef RBL011_WRITEREG
//#define RBL011_WRITEREG(add, value) *((volatile unsigned char *)(RBL011_BASEADD + add * RBL011_STRIDE)) = (value)
#define RBL011_WRITEREG(add, value) *((volatile unsigned char *)(RBL011_BASEADD + ((add << 1) << 1) - ((add << 1) & 1))) = (value)
#endif

#ifndef RBL011_READREG
//#define RBL011_READREG(add) (*((volatile unsigned char *)(RBL011_BASEADD + add * RBL011_STRIDE)))
#define RBL011_READREG(add) (*((volatile unsigned char *)(RBL011_BASEADD + ((add << 1) << 1) - ((add << 1) & 1))))
#endif

#define RBL011_ASYNC_STATUS_DA            0x01
#define RBL011_ASYNC_STATUS_FR            0x02
#define RBL011_ASYNC_STATUS_SW            0x04
#define RBL011_ASYNC_STATUS_SR            0x08
#define RBL011_ASYNC_STATUS_RS            0x10
#define RBL011_ASYNC_STATUS_RESET_START   0x20
#define RBL011_ASYNC_STATUS_RESET_STOP    0x40
#define RBL011_ASYNC_STATUS_COR_WRITTEN   0x80

#define RBL011_ASYNC_CONTROL_IF_ENABLE      0x01
#define RBL011_ASYNC_CONTROL_FR_ENABLE      0x02
#define RBL011_ASYNC_CONTROL_CONTROLBUFFER  0x04
#define RBL011_ASYNC_CONTROL_RESETCIS       0x08
#define RBL011_ASYNC_CONTROL_UNLOCKCIS      0x10
#define RBL011_ASYNC_CONTROL_IIR            0x20
#define RBL011_ASYNC_CONTROL_READY          0x40
#define RBL011_ASYNC_CONTROL_CLEARFLAGS     0x80

#define RBL011_ASYNC_CONTROL2_LOOP          0x01


  int RBL011_ASYNC_Init(void *pSysContext, const CALSSys_PCCardCallBackFunctions_s *pCallBacks);
  void RBL011_ASYNC_Close(void *pSysContext);
  int RBL011_ASYNC_LPDUSend(void *pSysContext, unsigned char *pucData1, int nLength1, unsigned char *pucData2, int nLength2, unsigned char *pucTrailer);
  int RBL011_ASYNC_SignalLDPUPending(void *pSysContext);

  extern const CALSSys_PCCardHalFunctions_s RBL011_ASYNC_PCCardHallFunctions;

  void RBL011_ASYNC_Start(void);
  unsigned short RBL011_ASYNC_Read_Version(void);
  void RBL011_ASYNC_ProcessIT(void);
  void RBL011_ASYNC_UpdateControl(unsigned short usSet, unsigned short usClear);
  void RBL011_ASYNC_UpdateInterruptMask(unsigned char ucSet, unsigned char ucClear);
  void RBL011_ASYNC_ClearFlags(void);
  unsigned char RBL011_ASYNC_ReadStatus(void);
  unsigned char RBL011_ASYNC_ReadIrqStatus(void);
  void RBL011_ASYNC_SetCIS(const unsigned char *pCIS, int CISLength);
  void RBL011_ASYNC_DumpCIS(void);
  void RBL011_ASYNC_BuildCIS(const char *szManufacturerName, const char *szProductName, unsigned short usManufacturerCode, unsigned short usManufacturerInformation, int nCIPlusVersion, int nCIPlusFeatures);
  unsigned char RBL011_ASYNC_ReadCOR(void);
unsigned char RBL011_ASYNC_LoopTest(void);

  // HAL requires a single semaphore to protect the low level hardware from concurent accesses, this semaphore shall be initialized before starting the stack
  void RBL011HAL_ASYNC_GetSemaphore(void);
  void RBL011HAL_ASYNC_ReleaseSemaphore(void);

  // uncomment this if semaphore are not required
   #define RBL011HAL_ASYNC_GetSemaphore()
   #define RBL011HAL_ASYNC_ReleaseSemaphore()

#ifdef __cplusplus
}
#endif

#endif // _RBL011_ASYNCBUSDRIVER_H_
