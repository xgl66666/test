/*

 User defined Data Types and API Functions for MxL201RF Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL201RF_API.c
 Date Created:  Jan. 20, 2009

*/


#ifndef __MxL_USER_DEFINE_H
#define __MxL_USER_DEFINE_H

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
#ifndef UINT8
#define UINT8       unsigned char
#endif
#ifndef UINT16
#define UINT16      unsigned short
#endif
#ifndef UINT32
#define UINT32      unsigned int
#endif
#ifndef SINT8
#define SINT8       char
#endif
#ifndef SINT16
#define SINT16      short
#endif
#ifndef SINT32
#define SINT32      int
#endif
#ifndef REAL32
#define REAL32      float
#endif

#if !defined(TRUE) && !defined(FALSE)
/// definition for TRUE
#define TRUE                        1
/// definition for FALSE
#define FALSE                       0
#endif


/*******************************************************************************************************/
/* common-use types */
/*******************************************************************************************************/
#ifndef BOOL
typedef int BOOL;
#endif  /* BOOL */



/******************************************************************************
**
**  Name: MxL_I2C_Write
**
**  Description:    I2C write operations
**
**  Parameters:    	
**					DeviceAddr	- MxL201RF Device address
**					pArray		- Write data array pointer
**					count		- total number of array
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
UINT32 MxL_I2C_Write(UINT8 DeviceAddr, UINT8* pArray, UINT32 count);

/******************************************************************************
**
**  Name: MxL_I2C_Read
**
**  Description:    I2C read operations
**
**  Parameters:    	
**					DeviceAddr	- MxL201RF Device address
**					Addr		- register address for read
**					*Data		- data return
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
UINT32 MxL_I2C_Read(UINT8 DeviceAddr, UINT8 Addr, UINT8* mData);

/******************************************************************************
**
**  Name: MxL_Delay
**
**  Description:    Delay function in milli-second
**
**  Parameters:    	
**					mSec		- milli-second to delay
**
**  Returns:        0
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
void MxL_Delay(UINT32 mSec);

# endif //__MxL_USER_DEFINE_H
