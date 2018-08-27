/**********************************************************
 * cilib.h
 *
 *  Created on: 24 oct. 2010
 *      Author: Gilles Gautier
 *   Copyright: Rabbit Labs 2010-2013
 **********************************************************/

#ifndef CILIB_H_
#define CILIB_H_

#include "apdutags.h"
#include <cyg/infra/diag.h>               // diag_printf
#ifdef __cplusplus
extern "C" {
#endif

#define CILIB_UNUSED_VARIABLE(var) (void)(var)

// Error codes from the CI+ 1.3.1 spec.
#define CILIB_CIPERR_NONE            0
#define CILIB_CIPERR_MODULEREVOKED   1
#define CILIB_CIPERR_HOSTREVOKED     2
#define CILIB_CIPERR_SACFAILED       3
#define CILIB_CIPERR_CCKFAILED       4
#define CILIB_CIPERR_FWUPGRADEBOOT   5
#define CILIB_CIPERR_FWUPGRADELOCT   6
#define CILIB_CIPERR_FWUPGRADESIGN   7
#define CILIB_CIPERR_AUTHRETRIES     8
#define CILIB_CIPERR_AUTHSIGNATURE   9
#define CILIB_CIPERR_AUTHKEYVERIF   10
#define CILIB_CIPERR_AUTHKEYCOMPUT  11
#define CILIB_CIPERR_AUTHDHFAILED   12
#define CILIB_CIPERR_CAMCERTSYNTAX  13
#define CILIB_CIPERR_CAMCERTEXPIRD  14
#define CILIB_CIPERR_CAMCERTSIGN    15
#define CILIB_CIPERR_HOSTCERTSYNTX  16
#define CILIB_CIPERR_HOSTCERTEXPRD  17
#define CILIB_CIPERR_HOSTCERTSIGN   18
#define CILIB_CIPERR_SVCOPCERTSYNTX 19
#define CILIB_CIPERR_SVCOPCERTEXPRD 20
#define CILIB_CIPERR_SVCOPCERTSIGN  21
#define CILIB_CIPERR_CAMUPDATEREQD  22

// Generic API error codes
#define CILIB_STATUS_OK                  0
#define CILIB_STATUS_MOREDATA            1
#define CILIB_STATUS_ERROR          0x1001
#define CILIB_ERROR_ALLOC           0x1002
#define CILIB_ERROR_NOTREADY        0x1003
#define CILIB_ERROR_NOTAVAIL        0x1004
#define CILIB_ERROR_PARAM           0x1005
#define CILIB_ERROR_OUTOFRANGE      0x1006
#define CILIB_ERROR_BUSY            0x1007
#define CILIB_ERROR_OVERFLOW        0x1008
#define CILIB_ERROR_OUTOFRSC        0x1009
#define CILIB_ERROR_NOTINITIALIZED  0x1010
#define CILIB_ERROR_TIMEOUT         0x1011
#define CILIB_ERROR_HOSTSTATE       0x1012
#define CILIB_ERROR_CLOSED          0x1013
#define CILIB_ERROR_NOTFOUND        0x1014
#define CILIB_LOOPTEST_ERROR    1

#define CILIB_ERROR_INTERNALERROR          0x2000
#define CILIB_ERROR_WARNAPDUTIMEOUT        0x2001
#define CILIB_ERROR_CRITAPDUTIMEOUT        0x2002
#define CILIB_ERROR_CERTLOADINGFAIL        0x2003
#define CILIB_ERROR_NOCALLBACK             0x2004
#define CILIB_ERROR_BADAPDUDATA            0x2005
#define CILIB_ERROR_ROOTCERTSYNTX          0x2020
#define CILIB_ERROR_ROOTCERTEXPIRD         0x2021
#define CILIB_ERROR_ROOTCERTSIGN           0x2022
#define CILIB_ERROR_BRANDCERTSYNTX         0x2023
#define CILIB_ERROR_BRANDCERTEXPIRD        0x2024
#define CILIB_ERROR_BRANDCERTSIGN          0x2025
#define CILIB_ERROR_DEVICECERTSYNTX        0x2026
#define CILIB_ERROR_DEVICECERTEXPIRD       0x2027
#define CILIB_ERROR_DEVICECERTSIGN         0x2028
#define CILIB_ERROR_SIGNATUREACREATE       0x2040
#define CILIB_ERROR_SIGNATUREAVERIFY       0x2041
#define CILIB_ERROR_SIGNATUREBCREATE       0x2042
#define CILIB_ERROR_SIGNATUREBVERIFY       0x2043
#define CILIB_ERROR_PRNGFAILED             0x2050
#define CILIB_ERROR_SACFORMAT              0x2060
#define CILIB_ERROR_SACCOUNTER             0x2061
#define CILIB_ERROR_URIVERSIONFAIL         0x2062
#define CILIB_ERROR_URITRANSMITFAIL        0x2064
#define CILIB_ERROR_SRMTRANSMITFAIL        0x2064

#define CILIB_SUCCEEDED(rc) ((rc) < 0x1000)
#define CILIB_FAILED(rc) ((rc) >= 0x1000)

#define CILIB_RESID_APPINFO        0x00020041
#define CILIB_RESID_APPMMI         0x00410042
#define CILIB_RESID_AUTHENTICATION 0x00100041
#define CILIB_RESID_CAMUPGRADE     0x008E1001
#define CILIB_RESID_CASUPPORT      0x00030041
#define CILIB_RESID_CONTENTCONTROL 0x008C1001
#define CILIB_RESID_DATETIME       0x00240041
#define CILIB_RESID_HOSTCONTROL    0x00200042
#define CILIB_RESID_HOSTLANGUAGE   0x008D1001
#define CILIB_RESID_LOWSPEEDCOM    0x00600003
#define CILIB_RESID_MMI            0x00400041
#define CILIB_RESID_OPPROFILE      0x008F1001
#define CILIB_RESID_SAS            0x00961001
#define CILIB_RESID_RESOURCEMGR    0x00010041

//private resource
#define CILIB_RESID_FACTORYSUPPORT 0xC4200001

#define CILIB_APPINFO_MAXVERSION        3
#define CILIB_APPMMI_MAXVERSION         2
#define CILIB_AUTHENTICATION_MAXVERSION 1
#define CILIB_CAMUPGRADE_MAXVERSION     1
#define CILIB_CASUPPORT_MAXVERSION      1
#define CILIB_CONTENTCONTROL_MAXVERSION 2
#define CILIB_DATETIME_MAXVERSION       1
#define CILIB_HOSTCONTROL_MAXVERSION    2
#define CILIB_HOSTLANG_MAXVERSION       1
#define CILIB_LOWSPEEDCOM_MAXVERSION    3
#define CILIB_MMI_MAXVERSION            1
#define CILIB_OPPROFILE_MAXVERSION      1
#define CILIB_RESOURCEMGR_MAXVERSION    2
#define CILIB_SAS_MAXVERSION            1

#define RBLMIN(a, b) (a < b ? a : b)
#define RBLMAX(a, b) (a > b ? a : b)

const char *CILib_GetErrorString(unsigned int code);
const char *CILib_ResourceName(unsigned long ulResID);
const char *CILib_DebugGetAPDUName(unsigned long ulTag);

void CILib_MemCopy(unsigned char *pTarget, const unsigned char *pSource, int nLength);
int CILib_MemCmp(const unsigned char *pTarget, const unsigned char *pSource, int nLength);
void CILib_MemSet(unsigned char *pTarget, unsigned char ucValue, int nLength);
int CILib_IsZero(unsigned char *pData, int nLength);

int CILib_StrLen(const char *pString);
void CILib_StrCopy(char *pTarget, const char *pSource, int nLength);
int CILib_StrCmp(const char *pString1, const char *pString2);
int CILib_StrNCmp(const char *pString1, const char *pString2, int n);
#define CILib_IsSpace(c) (((c)==0x20)||((c)==0x09))

int CILib_ReadASN1Size(unsigned char *pData, int *pSizeLength);
int CILib_WriteASN1Size(unsigned char *pData, int nSize);
int CILib_ComputeASN1Size(int nSize);

int CILib_ConvertToInteger(const char *szString, int *pnValue);

typedef struct {
  int nYear;    // year since 1900
  int nMonth;   // month range 1 to 12
  int nDay;     // day range 1 to 31
  int nHour;    // hour range 0 to 23
  int nMin;     // minutes range 0 to 59
  int nSec;     // seconds range 0 to 59
} CILib_DateTime_s;

int CILib_ConvertMJD(CILib_DateTime_s *pDateTime, unsigned short usMJD, unsigned long ulTime);
void CILIB_ConvertToMJD(CILib_DateTime_s *pDateTime, unsigned short *pusMJD, unsigned long *pulTime);

/*   Buffer Structure
 *
 *   -----------------------------------------------------------------------
 *   |               | ====== DATA ============== |                        |
 *   -----------------------------------------------------------------------
 *   ^<-------------> <-----------------^-------->
 *   |  Offset                UsedSize  |<------->
 *   pBuffer                            |  RemainingSize
 *                                      pCurrentLocation
*/
typedef struct {
  unsigned char *pBuffer;
  unsigned char *pCurrentLocation;
  unsigned long ulTotalSize;
  unsigned long ulOffset;
  unsigned long ulUsedSize;
  unsigned long ulRemainingSize;
} CILib_Buffer_s;

/* generic buffer functions */
// init a buffer structure
int CILib_BufferInit(CILib_Buffer_s *pBuffer, unsigned char *pData, unsigned long ulTotalSize, unsigned long ulOffset, unsigned long ulUsedSize);
// init a buffer structure for read only operation
int CILib_BufferInitReadOnly(CILib_Buffer_s *pBuffer, const unsigned char *pData, unsigned long ulSize);
// reset buffer to empry state (keep offset)
void CILib_BufferClear(CILib_Buffer_s *pBuffer);
// rewind at beginning of buffer
void CILib_BufferMoveToHead(CILib_Buffer_s *pBuffer);
// move at end of buffer
void CILib_BufferMoveToEnd(CILib_Buffer_s *pBuffer);
// move to specific buffer location
int CILib_BufferMoveTo(CILib_Buffer_s *pBuffer, unsigned long ulPosition);
// add data at head of buffer (reduce offset)
int CILib_BufferExpandHead(CILib_Buffer_s *pBuffer, unsigned long ulExpandSize);
// remove data at head of buffer (increase offset)
int CILib_BufferShrinkHead(CILib_Buffer_s *pBuffer, unsigned long ulShrinkSize);
// check if enough data is available
int CILib_BufferCheckData(CILib_Buffer_s *pBuffer, unsigned long ulRequiredData);

// clear failure flag
void CILib_BufferClearFailed(CILib_Buffer_s *pBuffer);
// read failure flag
int CILib_BufferGetFailed(CILib_Buffer_s *pBuffer);


/* read functions */
int CILib_BufferReadByte(CILib_Buffer_s *pBuffer, unsigned char *pucByte);
int CILib_BufferPeekByte(CILib_Buffer_s *pBuffer, unsigned char *pucByte);
int CILib_BufferReadShort(CILib_Buffer_s *pBuffer, unsigned short *pusShort);
int CILib_BufferReadSignedShort(CILib_Buffer_s *pBuffer, short *psShort);
int CILib_BufferReadWord24(CILib_Buffer_s *pBuffer, unsigned long *pulLong);
int CILib_BufferReadLong(CILib_Buffer_s *pBuffer, unsigned long *pulLong);
int CILib_BufferReadByteArray(CILib_Buffer_s *pBuffer, unsigned char *pucData, unsigned long ulLength);
int CILib_BufferReadString(CILib_Buffer_s *pBuffer, char *szString, unsigned long ulMaxBufferSize);
int CILib_BufferReadASN1Size(CILib_Buffer_s *pBuffer, unsigned long *pulSize);
int CILib_BufferSkip(CILib_Buffer_s *pBuffer, unsigned long ulLength);

/* write functions */
int CILib_BufferWriteByte(CILib_Buffer_s *pBuffer, unsigned char ucByte);
int CILib_BufferWriteShort(CILib_Buffer_s *pBuffer, unsigned short usShort);
int CILib_BufferWriteSignedShort(CILib_Buffer_s *pBuffer, short sShort);
int CILib_BufferWriteWord24(CILib_Buffer_s *pBuffer, unsigned long ulLong);
int CILib_BufferWriteLong(CILib_Buffer_s *pBuffer, unsigned long ulLong);
int CILib_BufferWriteByteArray(CILib_Buffer_s *pBuffer, const unsigned char *pData, unsigned long ulLength);
int CILib_BufferWriteString(CILib_Buffer_s *pBuffer, const char *pString);
int CILib_BufferWriteStringNo0(CILib_Buffer_s *pBuffer, const char *pString);
int CILib_BufferWriteASN1Size(CILib_Buffer_s *pBuffer, unsigned long ulSize);

int CILib_BufferEnsureCapacity(CILib_Buffer_s *pBuffer, unsigned long ulLength);
int CILib_BufferUpdateWritePosition(CILib_Buffer_s *pBuffer, unsigned long ulLength);

unsigned short CILib_ReadBEWord(const unsigned char *pData);
unsigned long CILib_ReadBEWord24(const unsigned char *pData);
unsigned long CILib_ReadBELong(const unsigned char *pData);
void CILib_WriteBEWord(unsigned char *pData, unsigned short usWord);
void CILib_WriteBEWord24(unsigned char *pData, unsigned long lData);

// format some number to HEX string (szOutput buffer shall be nDigit + 1)
void CILib_FormatHexFixed(char *szOutput, unsigned long ulValue, int nDigit);


unsigned long CILib_CRC32(const unsigned char *data, unsigned int datalen);

void CILib_DebugPrintf(const char *srcfile, int line, const char *fmt, ...);
void CILib_SetDebugPrintfRedirect(void (*fptr)(const char *));

/*
 * define CILIB_DBGPRINTF(...) to get debug output from CILIB.
 */

#if defined(_DEBUG) || defined(DEBUG)
  #define CILIB_DBGPRINTF(...) CILib_DebugPrintf(__FILE__, __LINE__, __VA_ARGS__)
#else
  #define CILIB_DBGPRINTF(...) diag_printf("%s, %d, %s", __FUNCTION__, __LINE__, __VA_ARGS__)
#endif

#define INVALID_PROGRAM_NUMBER 0xFFFF


// ========================= CIS HELPER ========================

#define CIS_VERSION_DVB_CI  0
#define CIS_VERSION_CI12  12
#define CIS_VERSION_CI13  13
#define CIS_VERSION_DVB_CI14  14

#define CIS_CIPLUSFEATURE_OPPROFILE 1


typedef struct {
  // parameters for callbacks (blindly passed to callback functions)
  void *pParam;
  int nParam;
  // parameters
  const char *szManufacturerName;
  int nManufacturerNameLength;  // if set to 0 szManufacturerNAme shall be 0 terminated
  const char *szProductName;
  int nProductNameLength; // if set to 0 szProductName shall be 0 terminated
  unsigned short usManufacturerCode;
  unsigned short usManufacturerInformation;
  int nCIPlusVersion;
  int nCIPlusFeatures;
  // function to push CIS out
  void(*pfnStart)(void *pParam, int nParam);
  void(*pfnPushByte)(void *pParam, int nParam, int nIndex, unsigned char ucByte);
  void(*pfnPushByteArray)(void *pParam, int nParam, int nIndex, const unsigned char *pucBytes, int nLength);
  void(*pfnComplete)(void *pParam, int nParam, int nIndex);
} CILib_CISBuilderParam_s;


int CILib_CISBuilder_Build(const CILib_CISBuilderParam_s *pBuilderParam);

#ifdef __cplusplus
}
#endif

#endif /* CILIB_H_ */
