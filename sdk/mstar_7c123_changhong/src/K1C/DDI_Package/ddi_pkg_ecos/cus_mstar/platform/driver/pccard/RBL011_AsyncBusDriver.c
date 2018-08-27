/*
* RBL011_ASYNC_spi.c
*
* Created: 24/06/2013 10:43:48
*  Author: Rabbit Labs
*/

#include "RBL011_AsyncBusDriver.h"
#include <cilib.h>

// RBL011 registers

#define RBL011_ASYNC_REGISTER_FIFO           0x00
#define RBL011_ASYNC_REGISTER_STATUS         0x01
#define RBL011_ASYNC_REGISTER_CONTROL        0x01
#define RBL011_ASYNC_REGISTER_SIZELOW        0x02
#define RBL011_ASYNC_REGISTER_SIZEHIGH       0x03
#define RBL011_ASYNC_REGISTER_COREXPECTED    0x04
#define RBL011_ASYNC_REGISTER_CORVALUE       0x04
#define RBL011_ASYNC_REGISTER_IRQMASKED      0x05
#define RBL011_ASYNC_REGISTER_IRQMASK        0x05
#define RBL011_ASYNC_REGISTER_CIS            0x07
#define RBL011_ASYNC_REGISTER_CONTROL2       0x08
#define RBL011_ASYNC_REGISTER_VERSIONLOW     0x0E
#define RBL011_ASYNC_REGISTER_VERSIONHIGH    0x0F


static unsigned char s_ucControl;
static unsigned char s_ucControl2;
static unsigned char s_ucInterruptMask;

#define RBL011_ASYNC_LPDUBUFFERSIZE 1024

static enum { IF_IDLE, IF_COR, IF_RESET, IF_SIZE_READ, IF_SIZE_WRITE, IF_READY } CAMInterfaceState;
static unsigned char s_ucCOR;
static unsigned char s_ucNextIsSizeWrite;
static unsigned short s_usNegociatedBufferSize;
static unsigned char s_ucTxBuffer[RBL011_ASYNC_LPDUBUFFERSIZE];
static int s_nTxCurrentBufferSize;

// store the pointers to stack function
const CALSSys_PCCardCallBackFunctions_s *pStackFunctions = 0;


void RBL011_ASYNC_ProcessIT(void)
{
  unsigned char ucMask = RBL011_ASYNC_ReadIrqStatus();
  unsigned char ucLocalBuffer[2];
  unsigned short usRxBufferSize = 0;
  unsigned char ITMaskClear = 0; // interruption to clear
  unsigned char ITMaskSet = 0;  // interruption to set
  unsigned char ucRxBuffer[RBL011_ASYNC_LPDUBUFFERSIZE];
  RBL011_ASYNC_ClearFlags();
  int i;

  if (ucMask & RBL011_ASYNC_STATUS_RESET_START)
  {
    // ITMaskClear |= RBL011_ASYNC_STATUS_RESET_START;
    RBL011_ASYNC_UpdateControl(0, RBL011_ASYNC_CONTROL_IF_ENABLE);
    CAMInterfaceState = IF_IDLE;
    CILIB_DBGPRINTF("RST START\r\n");
  }

  if (ucMask & RBL011_ASYNC_STATUS_RESET_STOP)
  {
    CAMInterfaceState = IF_IDLE;
    RBL011_ASYNC_UpdateControl(RBL011_ASYNC_CONTROL_IF_ENABLE, 0);
    RBL011_ASYNC_ClearFlags();
    ITMaskSet |= RBL011_ASYNC_STATUS_RESET_START | RBL011_ASYNC_STATUS_COR_WRITTEN;
    CILIB_DBGPRINTF("RST STOP\r\n");
  }

  if (ucMask & RBL011_ASYNC_STATUS_COR_WRITTEN)
  {
    CILIB_DBGPRINTF("<COR>");
    s_ucCOR = RBL011_ASYNC_ReadCOR();

    if ((s_ucCOR == 0xF) && (CAMInterfaceState == IF_IDLE)) {
      CAMInterfaceState = IF_COR;
    }

    // ITMaskClear |= RBL011_ASYNC_STATUS_COR_WRITTEN;
    CILIB_DBGPRINTF("</COR>");
  }

  if (ucMask & RBL011_ASYNC_STATUS_RS)
  {
    CILIB_DBGPRINTF("<RS>");
    RBL011_ASYNC_UpdateControl(RBL011_ASYNC_CONTROL_FR_ENABLE, 0);
    CAMInterfaceState = IF_RESET;
    s_nTxCurrentBufferSize = 0;
    s_usNegociatedBufferSize = 0;
    s_ucNextIsSizeWrite = 0;
    if (pStackFunctions->fnReset_cb)
      pStackFunctions->fnReset_cb();
    CILIB_DBGPRINTF("</RS>");
  }

  if (ucMask & RBL011_ASYNC_STATUS_SR)
  {
    ucLocalBuffer[0] = RBL011_ASYNC_LPDUBUFFERSIZE >> 8;
    ucLocalBuffer[1] = (unsigned char)(RBL011_ASYNC_LPDUBUFFERSIZE);
    CILIB_DBGPRINTF("<SR>");
    if (CAMInterfaceState == IF_RESET)
    {
      RBL011_ASYNC_LPDUSend(0, ucLocalBuffer, 2, 0, 0, 0);
      CAMInterfaceState = IF_SIZE_READ;
    }
    else
      CILIB_DBGPRINTF("Spurious SR !\r\n");
    //ITMaskClear |= RBL011_ASYNC_STATUS_SR;
    CILIB_DBGPRINTF("</SR>");
  }


  if (ucMask & RBL011_ASYNC_STATUS_SW)
  {
    s_ucNextIsSizeWrite = 1;
    CILIB_DBGPRINTF("<SW/>");
  }

  if (ucMask & RBL011_ASYNC_STATUS_DA)
  {

    usRxBufferSize = (((unsigned short)RBL011_READREG(RBL011_ASYNC_REGISTER_SIZEHIGH)) << 8) | RBL011_READREG(RBL011_ASYNC_REGISTER_SIZELOW);

    // crop if too big (a bit lame)
    if (usRxBufferSize > RBL011_ASYNC_LPDUBUFFERSIZE)
      usRxBufferSize = RBL011_ASYNC_LPDUBUFFERSIZE;

    for (i = 0; i < usRxBufferSize; i++)
      ucRxBuffer[i] = RBL011_READREG(RBL011_ASYNC_REGISTER_FIFO);

    if (s_ucNextIsSizeWrite)
    {
      CILIB_DBGPRINTF("Size received\r\n");

      if (usRxBufferSize == 2)
      {
        s_usNegociatedBufferSize = (((unsigned short)ucRxBuffer[0]) << 8) | ucRxBuffer[1];
        if (s_usNegociatedBufferSize > RBL011_ASYNC_LPDUBUFFERSIZE)
          s_usNegociatedBufferSize = RBL011_ASYNC_LPDUBUFFERSIZE;

        if (pStackFunctions)
        {
          if (pStackFunctions->fnSizeWrite_cb)
            pStackFunctions->fnSizeWrite_cb(s_usNegociatedBufferSize);
        }
      }
      else
      {
        // workaround for RBL011 issue
        s_usNegociatedBufferSize = RBL011_ASYNC_LPDUBUFFERSIZE;

        if (pStackFunctions)
        {
          if (pStackFunctions->fnSizeWrite_cb)
            pStackFunctions->fnSizeWrite_cb(s_usNegociatedBufferSize);

          if (pStackFunctions->fnLPDU_cb)
            pStackFunctions->fnLPDU_cb(ucRxBuffer, usRxBufferSize);

        }
      }

      usRxBufferSize = 0;

    }

    s_ucNextIsSizeWrite = 0;
  }

  if (ucMask & RBL011_ASYNC_STATUS_FR)
  {
    if (s_nTxCurrentBufferSize)
    {
      for (i = 0; i < s_nTxCurrentBufferSize; i++)
        RBL011_WRITEREG(RBL011_ASYNC_REGISTER_FIFO, s_ucTxBuffer[i]);


      // release buffer
      RBL011_ASYNC_UpdateControl(0, RBL011_ASYNC_CONTROL_CONTROLBUFFER);
      //CILIB_DBGPRINTF("LPDU sent\r\n");

      s_nTxCurrentBufferSize = 0;

      if (pStackFunctions)
      {
        if (pStackFunctions->fnLPDUDone_cb)
          pStackFunctions->fnLPDUDone_cb();
      }

    }
    else
    {
      // CILIB_DBGPRINTF("FR IT, no LPDU to send\r\n");
    }

    // clear FR bit if nothing to send
    if (s_nTxCurrentBufferSize == 0)
      ITMaskClear |= RBL011_ASYNC_STATUS_FR;
  }

  if (ITMaskClear || ITMaskSet)
    RBL011_ASYNC_UpdateInterruptMask(ITMaskSet, ITMaskClear);


  if (pStackFunctions)
  {
    if (usRxBufferSize && pStackFunctions->fnLPDU_cb)
      pStackFunctions->fnLPDU_cb(ucRxBuffer, usRxBufferSize);
  }
}

int  RBL011_ASYNC_Init(void *pSysContext, const CALSSys_PCCardCallBackFunctions_s *pCallBacks)
{
  pStackFunctions = pCallBacks;

  s_ucControl = 0;
  s_ucControl2 = 0;
  s_ucInterruptMask = 0;
  CAMInterfaceState = IF_IDLE;
  s_nTxCurrentBufferSize = 0;
  s_usNegociatedBufferSize = 0;
  s_ucNextIsSizeWrite = 0;

  return CILIB_STATUS_OK;
}

void RBL011_ASYNC_Close(void *pSysContext)
{
  // clear IT to stop
  RBL011_ASYNC_UpdateInterruptMask(0, 0xFF);
  RBL011_ASYNC_UpdateControl(0, RBL011_ASYNC_CONTROL_READY);
}

void RBL011_ASYNC_Start(void)
{
  // enable IT after task is started
  RBL011_ASYNC_UpdateInterruptMask(RBL011_ASYNC_STATUS_DA | RBL011_ASYNC_STATUS_SW | RBL011_ASYNC_STATUS_SR | RBL011_ASYNC_STATUS_RS | RBL011_ASYNC_STATUS_COR_WRITTEN | RBL011_ASYNC_STATUS_RESET_STOP | RBL011_ASYNC_STATUS_RESET_START, 0);
  RBL011_ASYNC_UpdateControl(RBL011_ASYNC_CONTROL_READY, 0);
}

unsigned short RBL011_ASYNC_Read_Version(void)
{
  return (((unsigned short)RBL011_READREG(RBL011_ASYNC_REGISTER_VERSIONHIGH)) << 8) | RBL011_READREG(RBL011_ASYNC_REGISTER_VERSIONLOW);
}

void RBL011_ASYNC_UpdateControl(unsigned short usSet, unsigned short usClear)
{
  unsigned char ucNewControl;

  RBL011HAL_ASYNC_GetSemaphore();

  ucNewControl = (s_ucControl & (~usClear)) | usSet;

  if (ucNewControl == s_ucControl)
  {
    // don't access hardware if control has not changed
    RBL011HAL_ASYNC_ReleaseSemaphore();
    return;
  }

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, ucNewControl);

  s_ucControl = ucNewControl;

  RBL011HAL_ASYNC_ReleaseSemaphore();
}

void RBL011_ASYNC_ClearFlags(void)
{

  RBL011HAL_ASYNC_GetSemaphore();

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl | RBL011_ASYNC_CONTROL_CLEARFLAGS);
  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl);

  RBL011HAL_ASYNC_ReleaseSemaphore();
}

void RBL011_ASYNC_UpdateInterruptMask(unsigned char ucSet, unsigned char ucClear)
{
  unsigned char ucNewMask;

  RBL011HAL_ASYNC_GetSemaphore();

  ucNewMask = (s_ucInterruptMask & (~ucClear)) | ucSet;

  if (ucNewMask == s_ucInterruptMask)
  {
    // don't access hardware if flags are unmodified
    RBL011HAL_ASYNC_ReleaseSemaphore();
    return;
  }

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_IRQMASK, ucNewMask);

  s_ucInterruptMask = ucNewMask;

  RBL011HAL_ASYNC_ReleaseSemaphore();
}

unsigned char RBL011_ASYNC_ReadStatus(void)
{
  return  RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS);
}

unsigned char RBL011_ASYNC_ReadIrqStatus(void)
{
  return  RBL011_READREG(RBL011_ASYNC_REGISTER_IRQMASKED);
}

void RBL011_ASYNC_SetCIS(const unsigned char *pCIS, int CISLength)
{
  int i;
  RBL011HAL_ASYNC_GetSemaphore();

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl | RBL011_ASYNC_CONTROL_RESETCIS | RBL011_ASYNC_CONTROL_UNLOCKCIS);
  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl | RBL011_ASYNC_CONTROL_UNLOCKCIS);

  for (i = 0; i < CISLength; i++)
    RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CIS, pCIS[i]);

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl);

  RBL011HAL_ASYNC_ReleaseSemaphore();

}
void RBL011_ASYNC_DumpCIS(void)
{
  int i;
  CILIB_DBGPRINTF("Dump CIS\r\n");
  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL,  RBL011_ASYNC_CONTROL_RESETCIS | RBL011_ASYNC_CONTROL_UNLOCKCIS| RBL011_ASYNC_CONTROL_IF_ENABLE);
  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL,  RBL011_ASYNC_CONTROL_UNLOCKCIS| RBL011_ASYNC_CONTROL_IF_ENABLE);

  for(i=0;i<256;i++)
    diag_printf("%02X ",RBL011_READREG(RBL011_ASYNC_REGISTER_CIS));

}
static void RBL011_ASYNC_BuildCIS_PushByte(int *pIndex, unsigned char ucByte);
static void RBL011_ASYNC_BuildCIS_PushByteArray(int *pIndex, const unsigned char *pucBytes, int nLength);

static const unsigned char Tuple1D1C[] = {
  0x1D, 0x04, 0x00, 0xDB, 0x08, 0xFF,
  0x1C, 0x03, 0x00, 0x08, 0xFF
};

static const unsigned char Tuple1A[] = {
  0x1A, 0x15, 0x01, 0x0F, 0x00, 0x02, 0x01, 0xC0, 0x0E, 0x41, 0x02, 'D', 'V', 'B', '_', 'C', 'I', '_' , 'V', '1', '.', '0', '0'
};

static const unsigned char Tuple1B[] = {
  0x1B, 0x11, 0xC9, 0x41, 0x19, 0x37, 0x55, 0x4E, 0x5E, 0x1D, 0x56, 0xAA, 0x60, 0x20, 0x03, 0x03, 0x50, 0xFF, 0xFF,
  0x1B, 0x26, 0xCF, 0x04, 0x19, 0x37, 0x55, 0x4D, 0x5D, 0x1D, 0x56, 0x22, 0x20
};

static const unsigned char TupleC0[] = {
  0xC0, 0x09, 'D', 'V', 'B', '_', 'H', 'O', 'S', 'T', 0
};

static const unsigned char TupleC1[] = {
  0xC1, 0x0E, 'D', 'V', 'B', '_', 'C', 'I', '_', 'M', 'O', 'D', 'U', 'L', 'E', 0
};


static const unsigned char Tuple14[] = {
  0x14, 0x00, 0xFF, 0x00
};

#define CIPLUSSTRING_START "$COMPATIBLE[CIPLUS=1"
#define CIPLUSSTRING_STARTSIZE sizeof(CIPLUSSTRING_START)-1
#define CIPLUSSTRING_PROF " CIPROF="
#define CIPLUSSTRING_PROFSIZE sizeof(CIPLUSSTRING_PROF)-1
#define CIPLUSSTRING_END "]$"
#define CIPLUSSTRING_ENDSIZE sizeof(CIPLUSSTRING_END)-1
#define CIS_BUFFERSIZE  257

static void RBL011_ASYNC_BuildCIS_PushByte(int *pnIndex, unsigned char ucByte)
{
  if (*pnIndex >= 256)
    return;

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CIS, ucByte);
  (*pnIndex)++;
}

static void RBL011_ASYNC_BuildCIS_PushByteArray(int *pnIndex, const unsigned char *pucBytes, int nLength)
{
  if (*pnIndex >= (256 - nLength))
    return;

  // copy array
  for (; nLength; nLength--) {
    RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CIS, *(pucBytes++));

    (*pnIndex)++;
  }
}

void RBL011_ASYNC_BuildCIS(const char *szManufacturerName, const char *szProductName, unsigned short usManufacturerCode, unsigned short usManufacturerInformation, int nCIPlusVersion, int nCIPlusFeatures)
{
  int nManufacturerNameLength;
  int nProductNameLength;
  int CIFeatureStringLength = 0;
  int nIndex = 0;

  nManufacturerNameLength = CILib_StrLen(szManufacturerName);
  nProductNameLength = CILib_StrLen(szProductName);

  RBL011HAL_ASYNC_GetSemaphore();

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl | RBL011_ASYNC_CONTROL_RESETCIS | RBL011_ASYNC_CONTROL_UNLOCKCIS);
  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl | RBL011_ASYNC_CONTROL_UNLOCKCIS);

  RBL011_ASYNC_BuildCIS_PushByteArray(&nIndex, Tuple1D1C, 11);

  if (nCIPlusVersion > CIS_VERSION_DVB_CI)
  {
    CIFeatureStringLength = CIPLUSSTRING_STARTSIZE + CIPLUSSTRING_ENDSIZE;

    if (nCIPlusVersion >= CIS_VERSION_CI13)
      CIFeatureStringLength += CIPLUSSTRING_PROFSIZE + 1;
  }

  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x15);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, nManufacturerNameLength + nProductNameLength + 6 + CIFeatureStringLength);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x05);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x00);
  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, (const unsigned char *)szManufacturerName, nManufacturerNameLength);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x00);
  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, (const unsigned char *)szProductName, nProductNameLength);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x00);

  if (nCIPlusVersion > CIS_VERSION_DVB_CI)
  {
    RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, (const unsigned char *)CIPLUSSTRING_START, CIPLUSSTRING_STARTSIZE);

    if (nCIPlusVersion >= CIS_VERSION_CI13)
    {
      RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, (const unsigned char *)CIPLUSSTRING_PROF, CIPLUSSTRING_PROFSIZE);
      if (nCIPlusFeatures)
        RBL011_ASYNC_BuildCIS_PushByte(&nIndex, '1');
      else
        RBL011_ASYNC_BuildCIS_PushByte(&nIndex, '0');
    }
    RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, (const unsigned char *)CIPLUSSTRING_END, CIPLUSSTRING_ENDSIZE);
    RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x00);
  }

  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0xFF);

  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x20);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, 0x04);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, (unsigned char)(usManufacturerCode >> 8));
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, (unsigned char)usManufacturerCode);
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, (unsigned char)(usManufacturerInformation >> 8));
  RBL011_ASYNC_BuildCIS_PushByte(&nIndex, (unsigned char)usManufacturerInformation);

  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, Tuple1A, 23);
  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, Tuple1B, 32);
  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, TupleC0, 11);
  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, TupleC1, 16);
  RBL011_ASYNC_BuildCIS_PushByteArray( &nIndex, Tuple14, 4);

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl);

  RBL011HAL_ASYNC_ReleaseSemaphore();
}


unsigned char RBL011_ASYNC_ReadCOR(void)
{

  RBL011HAL_ASYNC_GetSemaphore();

  s_ucCOR = RBL011_READREG(RBL011_ASYNC_REGISTER_CORVALUE);

  RBL011HAL_ASYNC_ReleaseSemaphore();

  return s_ucCOR;
}

int RBL011_ASYNC_LPDUSend(void *pSysContext, unsigned char *pucData1, int nLength1, unsigned char *pucData2, int nLength2, unsigned char *pucTrailer)
{
  int nTotalLength = nLength1 + nLength2 + (pucTrailer ? 4 : 0);
  int i;

  if (nTotalLength > RBL011_ASYNC_LPDUBUFFERSIZE)
  {
    CILIB_DBGPRINTF("LPDU Fail: too big\r\n");
    return CILIB_ERROR_OVERFLOW;
  }

  RBL011HAL_ASYNC_GetSemaphore();

  if (s_nTxCurrentBufferSize != 0)
  {
    RBL011HAL_ASYNC_ReleaseSemaphore();
    CILIB_DBGPRINTF("LPDU Fail: busy\r\n");
    return CILIB_ERROR_BUSY;
  }

  if (nLength1 && pucData1)
    CILib_MemCopy(s_ucTxBuffer, pucData1, nLength1);
  if (nLength2 && pucData2)
    CILib_MemCopy(s_ucTxBuffer + nLength1, pucData2, nLength2);
  if (pucTrailer)
    CILib_MemCopy(s_ucTxBuffer + nLength1 + nLength2, pucTrailer, 4);

  s_nTxCurrentBufferSize = nTotalLength;

  s_ucControl |= RBL011_ASYNC_CONTROL_CONTROLBUFFER;

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl);

  // check if ready to write immediately
  if (RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS) & RBL011_ASYNC_STATUS_FR)
  {
    // write data
    for (i = 0; i < s_nTxCurrentBufferSize; i++)
      RBL011_WRITEREG(RBL011_ASYNC_REGISTER_FIFO, s_ucTxBuffer[i]);

    s_ucControl &= ~RBL011_ASYNC_CONTROL_CONTROLBUFFER;

    RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL, s_ucControl);

    //CILIB_DBGPRINTF("LPDU sent\r\n");

    s_nTxCurrentBufferSize = 0;

    if (pStackFunctions)
    {
      if (pStackFunctions->fnLPDUDone_cb)
        pStackFunctions->fnLPDUDone_cb();
    }
  }
  else
  {
    s_ucInterruptMask |= RBL011_ASYNC_STATUS_FR;

    RBL011_WRITEREG(RBL011_ASYNC_REGISTER_IRQMASK, s_ucInterruptMask);
  }

  RBL011HAL_ASYNC_ReleaseSemaphore();

  return CILIB_STATUS_OK;
}

int RBL011_ASYNC_SignalLDPUPending(void *pSysContext)
{
  return CILIB_STATUS_OK;
}
unsigned char RBL011_ASYNC_LoopTest(void)
{
  int i;
   CILIB_DBGPRINTF("LoopTest\r\n");

   RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL2, RBL011_ASYNC_CONTROL2_LOOP); //set loop mode
   RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL,  RBL011_ASYNC_CONTROL_CONTROLBUFFER | RBL011_ASYNC_CONTROL_UNLOCKCIS| RBL011_ASYNC_CONTROL_IF_ENABLE);

   if((RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS) & RBL011_ASYNC_STATUS_FR) == 0)
        return CILIB_LOOPTEST_ERROR;

  for (i = 0; i < 256; i++)
       RBL011_WRITEREG(RBL011_ASYNC_REGISTER_FIFO, i);

  RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL,   RBL011_ASYNC_CONTROL_UNLOCKCIS| RBL011_ASYNC_CONTROL_IF_ENABLE); //push data

 //CILIB_DBGPRINTF("read loop data\r\n");

   if(((RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS) & RBL011_ASYNC_STATUS_FR) == 1)  ||
       ((RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS) & RBL011_ASYNC_STATUS_DA) == 0))
       return CILIB_LOOPTEST_ERROR;

   for(i=0;i<256;i++)
    {
        if(i != RBL011_READREG(RBL011_ASYNC_REGISTER_FIFO)) //check data
            return CILIB_LOOPTEST_ERROR;
    }

   if(((RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS) & RBL011_ASYNC_STATUS_FR) == 0) ||
       ((RBL011_READREG(RBL011_ASYNC_REGISTER_STATUS) & RBL011_ASYNC_STATUS_DA) == 1))
       return CILIB_LOOPTEST_ERROR;

   RBL011_WRITEREG(RBL011_ASYNC_REGISTER_CONTROL2, 0);

   return CILIB_STATUS_OK;
}
const CALSSys_PCCardHalFunctions_s RBL011_ASYNC_PCCardHallFunctions = {
  .fnInit = RBL011_ASYNC_Init,
  .fnClose = RBL011_ASYNC_Close,
  .fnSendLPDU = RBL011_ASYNC_LPDUSend,
  .fnSignalLDPUPending = RBL011_ASYNC_SignalLDPUPending
};
