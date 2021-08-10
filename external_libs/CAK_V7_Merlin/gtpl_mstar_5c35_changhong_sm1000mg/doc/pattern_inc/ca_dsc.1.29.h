/**
**  @file ca_dsc.h
**
**  @brief
**   This file defines the generic descrambler API used by the different
**   NagraVision modules and software to access the hardware descrambling
**   capabilities of the set-top box.
**
**  @ingroup DSC
**
**  $Revision: 1.29 $
**
** COPYRIGHT:
**   1998-2002, 2006, 2007 NagraVision S.A.
**
*/    

/*
** REMARK:
**    Comments in this file use special tags to allow automatic API
**    documentation generation in HTML format, using the
**    GNU-General Public Licensed Doxygen tool.
**    For more information about Doxygen, please check www.doxygen.org
*/

/**
 *  @defgroup DSC Descrambler
 *  @brief
 *    Descrambler component
 *
 *    This component is the interface to the descrambler driver.
*/


#ifndef CA_DSC_H
#define CA_DSC_H

/******************************************************************************/
/*                                                                            */
/*                               INCLUDE FILES                                */
/*                                                                            */
/******************************************************************************/

#ifdef DSC_WRAP
#include "dscwrap.h"
#endif

#include "ca_defs.h"
#include "ca_defsx.h"
#include "ca_mpeg.h"


/******************************************************************************/
/*                                                                            */
/*                              TYPES DEFINITIONS                             */
/*                                                                            */
/******************************************************************************/
/**
 *  @brief
 *    Identifier of DSC channel.
 *
 *  @see DSC_CHANNEL_ID_INVALID
*/
typedef struct SDscChannelId* TDscChannelId;

/**
 *  @brief
 *    Invalid value for TDscChannelId.
 *
 *  @see TDscChannelId
*/
#define DSC_CHANNEL_ID_INVALID ((TDscChannelId) NULL)


/**
 *  @brief
 *    Return values of the DSC methods.
 *
*/
typedef enum
{
  DSC_NO_ERROR,
  /**<  Method call was successful.
   */
  DSC_ERROR_NO_MORE_RESOURCES,
  /**<  No more filter resources are available for the demultiplexer.
   */
  DSC_ERROR_UNKNOWN_ID,
  /**<  The descrambler resource is not open.
   */
  DSC_ERROR_BAD_PID,
  /**<  The given PID is out of range.
   */
  DSC_ERROR_BAD_KEY_LENGTH,
  /**<  The key length is not supported by the hardware.
   */
  DSC_ERROR_NOT_IMPLEMENTED,
  /**<  The descrambler channel is not implemented.
   */
  DSC_ERROR,
  /**<  Other error.
   */
  LAST_DSC_STATUS
  /**< Number of types. Internal use only.
  */
} TDscStatus;


/**
 *  @brief
 *    Descrambling key set in the descrambler channel.
*/
typedef TUnsignedInt8 TDscKey;


/******************************************************************************/
/*                                                                            */
/*                             FUNCTION PROTOTYPES                            */
/*                                                                            */
/******************************************************************************/


/**
 *  @brief
 *    This function opens a descrambler channel.
 *
 *  @param[out] pxDscChannelId
 *                Identificator of the opened channel. This identificator is a
 *                parameter to every function accessing the channel.
 *
 *  @param[in] xTransportSessionId
 *               Transport session identifier selecting which transport should
 *               be descrambled.
 *
 *  @param[in] xMpegStreamType
 *               MPEG stream type as defined in table 2-35 of ISO/IEC 13818-1.
 *
 *  @retval  DSC_NO_ERROR
 *             Success.
 *
 *  @retval  DSC_ERROR_NO_MORE_RESOURCES
 *             No more resources are available.
 *
 *  @retval  DSC_ERROR
 *             Other error.
 *
 *  @remarks
 *   -# This function shall not return any error about the MPEG stream type.
 *      This type is not handled by the CA software. MPEG stream types defined 
 *      in ca_mpeg.h will no longer evolve and are maintained for backward 
 *      compatibility reasons only.
*/
TDscStatus dscOpen
(
  TDscChannelId*        pxDscChannelId,
  TTransportSessionId    xTransportSessionId,
  TMpegStreamType        xMpegStreamType
);


/**
 *  @brief
 *    This function closes a descrambler channel.
 *
 *  @param[in] xDscChannelId
 *               Channel to close.
 *
 *  @retval  DSC_NO_ERROR
 *             Success.
 *
 *  @retval  DSC_ERROR_UNKNOWN_ID
 *             The descrambler ID is not currently opened.
 *
 *  @retval  DSC_ERROR
 *             Other error.
 *
*/
TDscStatus dscClose
(
  TDscChannelId   xDscChannelId
);


/**
 *  @brief
 *    This function sets the PID to descramble on a particular channel.
 *
 *  @param[in] xDscChannelId
 *               Channel holding the PID to descramble.
 *
 *  @param[in] xPid
 *               PID to descramble.
 *
 *  @retval DSC_NO_ERROR
 *            Success.
 *
 *  @retval DSC_ERROR_UNKNOWN_ID
 *            The descrambler ID is not currently opened.
 *
 *  @retval DSC_ERROR_BAD_PID
 *            This PID value is not allowed.
 *
 *  @retval DSC_ERROR
 *            Other error.
 *
*/
TDscStatus dscSetPid
(
  TDscChannelId   xDscChannelId,
  TPid            xPid
);


/**
 *  @brief
 *    Set the keys on the specified channel, in non-protected mode, i.e. CWs in 
 *    clear-text form.
 *
 *  @param[in] xDscChannelId
 *               Channel for which the keys must be updated.
 *
 *  @param[in] xOddKeyLength
 *               Length of the odd key in bytes.
 *
 *  @param[in] pxOddKey
 *               Buffer containing the odd key to be stored in the key register.
 *
 *  @param[in] xEvenKeyLength
 *               Length of the even key in bytes.
 *
 *  @param[in] pxEvenKey
 *               Buffer containing the even key to be stored in the key 
 *               register.
 *
 *  @retval  DSC_NO_ERROR
 *             Success.
 *
 *  @retval  DSC_ERROR_UNKNOWN_ID
 *             The descrambler ID is not currently opened.
 *
 *  @retval  DSC_ERROR_BAD_KEY_LENGTH
 *             The key length is not supported by the hardware.
 *
 *  @retval  DSC_ERROR
 *             Other error.
 *
 *  @remarks
 *   -# If one of the key is not available, the given pointer is set to \c NULL 
 *      and the corresponding length to 0. The driver should not change the key 
 *      in this case.
*/
TDscStatus dscSetKeys
(
        TDscChannelId   xDscChannelId,
        TSize           xOddKeyLength,
  const TDscKey*        pxOddKey,
        TSize           xEvenKeyLength,
  const TDscKey*        pxEvenKey
);


/**
 *  @brief
 *    Set the keys on the specified channel in protected mode, i.e. CWs input in 
 *    cipher-text form and decrypted in a secure way by the hardware key ladder.
 *     
 *    The content of the buffer pointed to by \c pxL1ProtectingKey complies with 
 *    the format of the 128-bit input key defined "TDES Key Format" section.
 *    
 *    The contents of the buffers pointed at by \c pxOddProtectedKey and 
 *    \c pxEvenProtectedKey are each one 8-byte block in memory, encoded in MSBF 
 *    (Most-Significant Byte at lowest address).
 *    
 *    Beware that underlying hardware may use a byte ordering other than what is 
 *    specified in this document. For example, some DMA-based engines may read 
 *    and write data in a little-endian way, whereas this API requires 
 *    big-endian representation. Similar issues may appear with non-DMA hardware 
 *    engines. If the underlying hardware is not accessed directly but through a 
 *    software library provided by the chip vendor, similar issues may arise 
 *    with the chip vendor's API. In all such cases, the DAL implementation 
 *    shall change the byte ordering of input data so it is interpreted 
 *    correctly by the underlying hardware.
 *    
 *  @param[in] xDscChannelId
 *               Channel for which the keys must be updated.
 *
 *  @param[in] xL1ProtectingKeyLength
 *               Length of the Level 1 Key cipher-text in bytes.
 *
 *  @param[in] pxL1ProtectingKey
 *               Cipher-text input to Level 1 key of Key Ladder
 *
 *  @param[in] xOddProtectedKeyLength
 *               Length of the odd key cipher-text in bytes.
 *
 *  @param[in] pxOddProtectedKey
 *               Buffer containing the odd keys to store in the key register
 *
 *  @param[in] xEvenProtectedKeyLength
 *               Length of the even key cipher-text in bytes.
 *
 *  @param[in] pxEvenProtectedKey
 *               Buffer containing the even keys to store in the key register
 *
 *  @retval  DSC_NO_ERROR
 *             Success.
 *
 *  @retval  DSC_ERROR_UNKNOWN_ID
 *             The descrambler ID is not currently opened.
 *
 *  @retval  DSC_ERROR_BAD_KEY_LENGTH
 *             The key length is not supported by the hardware.
 *
 *  @retval  DSC_ERROR_NOT_IMPLEMENTED
 *             The function is not implemented.
 *
 *  @retval  DSC_ERROR
 *             Other error.
 *
 *  @remarks
 *   -# If one of the protected keys is not available, the given pointer is set 
 *      to \c NULL and the corresponding length to 0. The driver should not 
 *      change the key in this case
*/
TDscStatus dscSet1LevelProtectedKeys
(
        TDscChannelId    xDscChannelId,
        TSize            xL1ProtectingKeyLength,
  const TUnsignedInt8*  pxL1ProtectingKey,
        TSize            xOddProtectedKeyLength,
  const TUnsignedInt8*  pxOddProtectedKey,
        TSize            xEvenProtectedKeyLength,
  const TUnsignedInt8*  pxEvenProtectedKey
);


/**
 *  @brief
 *    This function provides the descrambler channel with the metadata 
 *    associated to the program. The program metadata is conveyed into the ECM 
 *    stream from the head-end to set-top box. It consists in a collection of 
 *    TLV descriptors complying with the format defined "CAK, Metadata Format, 
 *    Specification". The copy protection information is a typical usage of 
 *    program metadata.
 *
 *  @param[in] xDscChannelId
 *               Descrambler channel identifier
 *
 *  @param[in] xMetadataLength
 *             	 Length in bytes of the metadata buffer.
 *
 *  @param[in] pxMetadata
 *               Buffer containing the metadata descriptors
 *
 *  @retval  DSC_NO_ERROR
 *             Success.
 *
 *  @retval  DSC_ERROR_UNKNOWN_ID
 *             The descrambler ID is not currently opened.
 *
 *  @retval  DSC_ERROR_NOT_IMPLEMENTED
 *             The function is not implemented.
 *
 *  @retval  DSC_ERROR
 *             Other error.
 *
 *  @remarks
 *   -# This function is called only if access to the content is granted. In 
 *      such a case, it is called just before setting the keys into the 
 *      descrambler (\c dscSetKeys or \c dscSet1LevelProtectedKeys), even if 
 *      there is no metadata. In this latter case, pxMetadata is set to \c NULL 
 *      and \c xMetadataLength is set to 0.
*/
TDscStatus dscSetMetadata
(
        TDscChannelId       xDscChannelId,
        TSize               xMetadataLength,
  const TUnsignedInt8*     pxMetadata
);
#endif /* CA_DSC_H */

/* ca_dsc.h */
