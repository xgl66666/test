/**
**  @file nocs11_ctd.h
**
**  @brief
**    Chipset Test Driver API for NOCS 1.1 Secure chipset
**
**  @ingroup
**    CTD
**
**  @author
**    NOCS team
**
**  @version
**    Revision: 1.1
**
** COPYRIGHT:
**   2008 NagraVision S.A.
**
** HISTORY:
**   $Log: __________ $
**   Revision 1.0 2008/05/22 11:30:00 Nocs team
**   First published version
**
**   Revision 1.1 2008/11/17 09:30:00 Nocs team
**	 Change one enum type: SCtdConfigurationEnumID to SCtdBitFieldHandle
*/
/*
** REMARK:
**    Comments in this file use special tags to allow automatic API
**    documentation generation in HTML format, using the
**    GNU-General Public Licensed Doxygen tool.
**    For more information about Doxygen, please check www.doxygen.org
*/
/**
 * @defgroup
 *    CTD Chipset Test Driver
 * @brief
 *    API to allow tests on a given chipset
 *
*/

#ifndef CTD_H
#define CTD_H

/******************************************************************************/
/*                                                                            */
/*                               INCLUDE FILES                                */
/*                                                                            */
/******************************************************************************/



/******************************************************************************/
/*                                                                            */
/*                              TYPES DEFINITIONS                             */
/*                                                                            */
/******************************************************************************/

/**
 *  @brief
 *    Define the enumeration type of the different OTP fields.
 *
 *  TCtdBitFieldHandle holds all relevant information needed by the CTD driver 
 *  in order to identify the bit field. It is an opaque type definition. The 
 *  chipset manufacturer has to define it in the nocs11_ctd_impl.h file.
*/

typedef SCtdBitFieldHandle TCtdBitFieldHandle;

/**
 *  @brief
 *    Define the byte as an 8 bits vector.
 *
 *  Definition of the byte as an 8 bits vector. This type is the smallest piece
 *  of data in the API.
*/
typedef unsigned char TCtdUnsignedInt8;

/**
 *  @brief
 *    Define the integer type.
 *
 *  Re-definition of the integer type.
*/
typedef unsigned int TCtdUnsignedInt16;

/*
 * EXTERNAL_CTD_STATUS_OFFSET could be defined in nocs11_ctd_impl.h and so,
 * allows the chipset manufacturer to define an offset on the lowest status
 * value returned by the CTD driver.
*/
#ifdef EXTERNAL_CTD_STATUS_OFFSET
#define CTD_STATUS_OFFSET   EXTERNAL_CTD_STATUS_OFFSET
#else
#define CTD_STATUS_OFFSET   0
#endif

/**
 *  @brief
 *    Return values of the CTD methods.
 *
*/
typedef enum
{
  CTD_NO_ERROR = CTD_STATUS_OFFSET,
    /**< The intended operation was executed successfully.
     */
  CTD_ERROR,
    /**< The function terminated abnormally. The intended operation failed.
     */
  LAST_CTD_STATUS
    /**< TCtdStatus type upper boundary.
     */
} TCtdStatus;




/******************************************************************************/
/*                                                                            */
/*                             FUNCTION PROTOTYPES                            */
/*                                                                            */
/******************************************************************************/


/**
 *  @brief
 *    Configure the bit fields.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param[in]      xBitFieldHandle
 *                   Identifier of the field to modify.
 *  @param[in]      xBitFieldLength
 *                   Length in bit of the modified field.
 *  @param[in]      pxBitFieldValue
 *                   Pointer to the location where to read the value of the
 *                   field.
 *  @param[in]      pxBitFieldMask
 *                   Pointer to the location where to read the mask value.
 *
 *  @retval   CTD_NO_ERROR
 *              Success.
 *  @retval   CTD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function requests to switch the state of one of the NOCS related
 *  Persistent Value bit fields identified by xBitFieldHandle to the state
 *  specified in pxBitFieldValue masked by pxBitFieldMask.
 *  xBitFieldLength specifies the length of the field in bits.
 *  pxBitFieldValue contains the value to be programmed if the corresponding
 *  bits in pxBitFieldMask are set to b'1. If the corresponding bits in
 *  pxBitFieldMask are set to b'0, the bits shall be left unchanged regardless
 *  of the value in pxBitFieldValue.
 *  pxBitFieldValue and pxBitFieldMask are arrays of bytes. The number of byte
 *  of these fields is defined with the following pseudo-code:
 *      if (xBitFieldLength mod 8 bits) == 0
 *          number of bytes = (xBitFieldLength / 8 bits)
 *      else
 *		    number of bytes = round inf(xBitFieldLength / 8 bits) + 1
 *
 *  xBitFieldLength represents the Less Significant Bit position in
 *  pxBitFieldValue.
 *  If the Persistent Value configuration bit field is already in the requested
 *  state when this function is called or if the function has successfully set
 *  the Persistent Value configuration bit field in the requested state, the
 *  function shall return CTD_NO_ERROR. In all other cases, the call shall have
 *  no effect and the function shall return CTD_ERROR.
*/
TCtdStatus ctdSetConfigurationBitField
(
   TCtdBitFieldHandle		 xBitFieldHandle,
   TCtdUnsignedInt16	   xBitFieldLength,
   TCtdUnsignedInt8*		pxBitFieldValue,
   TCtdUnsignedInt8*		pxBitFieldMask
);



/**
 *  @brief
 *    Read the bit fields.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param[in]      xBitFieldHandle
 *                   Identifier of the field to modify.
 *  @param[in]      xBitFieldLength
 *                   Length in bit of the modified field.
 *  @param[out]     pxBitFieldValue
 *                   Pointer to the location where to read the value of the
 *                   field.
 *
 *  @retval   CTD_NO_ERROR
 *              Success.
 *  @retval   CTD_ERROR
 *              The function terminated abnormally. The intended operation
 *              failed.
 *
 *  This function returns the state of the Persistent Value configuration bit
 *  field corresponding to the handle xBitFieldHandle passed as argument.
 *  xBitFieldLength specifies the length of the field.
 *  The function shall read the bit field indicated by xBitFieldHandle and
 *  xBitFieldLength and return the value in pxBitFieldValue.
 *  For some chipset, due to the Persistent Value technology used to store the
 *  configuration bits, the state of the Persistent Value could be unprogrammed.

*/
TCtdStatus ctdGetConfigurationBitField
(
   TCtdBitFieldHandle    xBitFieldHandle,
   TCtdUnsignedInt16		 xBitFieldLength,
   TCtdUnsignedInt8*		pxBitFieldValue
);

#endif /* CTD_H */

/* nocs11_ctd.h */
