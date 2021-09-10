/******************************************************************************
 *            COPYRIGHT 2001 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME:   TDAL_DESC
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_desc.h $
 *      $Rev: 4832 $
 *      $Date: 2011-01-25 17:28:36 +0100 (mar., 25 janv. 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION:   Header of desc
 *
 *****************************************************************************/

#ifndef _TDAL_DESC_H_
#define _TDAL_DESC_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*       Defines              */
/*******************************************************/
#define kTDAL_DESC_ILLEGAL_DESCRAMBLER 0xFFFFFFFF
#define kTDAL_DESC_ILLEGAL_PID 0x1FFF
#define kTDAL_DESC_MAX_DESCRAMBLERS   5

#define kTDAL_DESC_MAX_KEY_EVEN_ODD_SIZE 8
#define kTDAL_DESC_MAX_KEY_L1_SIZE 16
/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef uint32_t tTDAL_DESC_descrambler;

typedef enum
{
   eTDAL_DESC_NO_ERROR = 0,
   eTDAL_DESC_NOT_DONE,
   eTDAL_DESC_ERROR_UNKNOW_ID,
   eTDAL_DESC_ERROR
}tTDAL_DESC_Error;

/* for compatibility */
typedef enum
{
   TDAL_DESC_RESERVED = 0x00,
   TDAL_DESC_VIDEO = 0x01,
   TDAL_DESC_OTHER_VIDEO = 0x02,
   TDAL_DESC_AUDIO = 0x03,
   TDAL_DESC_OTHER_AUDIO = 0x04,
   TDAL_DESC_PRIVATE_SECTION = 0x05,
   TDAL_DESC_PES_PACKETS_CONTAINING_PRIVATE_DATA  = 0x06,
   TDAL_DESC_MHEG = 0x07,
   TDAL_DESC_DSM_CC = 0x08
}TDAL_DESC_stream_type_e;

typedef enum
{
   TDAL_DESC_PAYLOAD_DVB_CSA2 = 0x0000,
   TDAL_DESC_PAYLOAD_DVB_CSA3 = 0x0001,

   TDAL_DESC_PAYLOAD_AES_IDSA = 0x0020,     // AES-128 ATIS IIF Default Scrambling Algorithm (IDSA)
   TDAL_DESC_PAYLOAD_AES_ECB = 0x0021,      // AES-128 ECB mode. Termination in clear.
   TDAL_DESC_PAYLOAD_AES_CI_PLUS = 0x0022,  // AES-128 CBC mode. IV modifiable. Termination in clear.  (CI+)
   TDAL_DESC_PAYLOAD_TDES_CBC = 0x0040,     // TDES keying option 2 in CBC mode with all bits set to zero IV. DVS-042 termination.
   TDAL_DESC_PAYLOAD_TDES_ECB = 0x0041,     // TDES keying option 2 in ECB mode. Termination in clear.

   TDAL_DESC_RAW_AES_CBC = 0x4020,          // AES-128 in CBC mode with all bits set to zero IV.  Termination in clear.
   TDAL_DESC_RAW_AES_ECB = 0x4021,          // AES-128 in ECB mode. Termination in clear.
   TDAL_DESC_RAW_AES_CBC_CTR_PIFF = 0x4022, //
   TDAL_DESC_RAW_AES_CBC_APPLE = 0x4023,    //
   TDAL_DESC_RAW_AES_CTR_APPLE = 0x4024,    //  AES-128 in CTR  mode
   TDAL_DESC_RAW_TDES_CBC = 0x4040,         // TDES keying option 2 in CBC mode with all bits set to zero IV. Termination in clear.
   TDAL_DESC_RAW_TDES_ECB = 0x4041,         // TDES keying option 2 in ECB mode. Termination in clear.

   TDAL_DESC_INVALID_TYPE = 0xFFFF
}TDAL_DESC_DescType;

typedef enum
{
   TDAL_DESC_ODD_KEY = 0,
   TDAL_DESC_EVEN_KEY,
   TDAL_DESC_GENERIC_KEY
}tTDAL_DESC_KeyType;

typedef struct {
   uint8_t bNbDescrambler_Max;
   bool   isDVB_CSA_Supported;
   bool   isAES_128_CBC_Supported;
   bool   isDES_56_ECB_Supported;
}tTDAL_DESC_Capability;



/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_DESC_Error TDAL_DESC_Init(void);
tTDAL_DESC_Error TDAL_DESC_Terminate(void);

tTDAL_DESC_descrambler TDAL_DESC_Open_Descrambler (TDAL_DESC_stream_type_e stream_type);
tTDAL_DESC_Error TDAL_DESC_Is_Descrambler_Open(tTDAL_DESC_descrambler descrambler);

tTDAL_DESC_Error TDAL_DESC_Close_Descrambler   (tTDAL_DESC_descrambler descrambler);

tTDAL_DESC_Error TDAL_DESC_Stream_Authorisation (int16_t pid, bool authorize);

tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Pid  (tTDAL_DESC_descrambler descrambler, int16_t pid);

tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys (tTDAL_DESC_descrambler descrambler,
                  int16_t odd_key_length, const int8_t *odd_key,
                  int16_t even_key_length, const int8_t *even_key);
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys_L1 (tTDAL_DESC_descrambler descrambler,
                  int16_t L1_key_length, const int8_t *L1_key,
                  int16_t odd_key_length, const int8_t *odd_key,
                  int16_t even_key_length, const int8_t *even_key);

tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys_ETSI_L2(tTDAL_DESC_descrambler descIdx,
                             int16_t L1L2_key_length,
                             const int8_t * L1Key,
                             const int8_t * L2Key,
                             int16_t xOddProtectedKeyLength,
                             const int8_t * pxOddProtectedKey,
                             int16_t xEvenProtectedKeyLength,
                             const int8_t *pxEvenProtectedKey);
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys_Cert(tTDAL_DESC_descrambler descrambler,
                            int16_t keyIdSize,
                            const int8_t *KeyId);

tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Type (tTDAL_DESC_descrambler descrambler,
                     TDAL_DESC_DescType descrambler_type);

const char*      TDAL_DESC_APIRevisionGet( void );
const char*      TDAL_DESC_PlatformRevisionGet( void );
tTDAL_DESC_Error TDAL_DESC_GetCapability(tTDAL_DESC_Capability *pstCapability);


#ifdef TDAL_DESC_DVB_SECURE_CHIPSET
tTDAL_DESC_Error TDAL_DESC_SECURE_GetChipId(uint32_t *chipId);
tTDAL_DESC_Error TDAL_DESC_SECURE_SetCurrentKey(uint8_t *currentKey);
tTDAL_DESC_Error TDAL_DESC_SECURE_DescrambleCW(uint8_t *controlWord, uint8_t **ChipsetValue);
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DESC_H_*/



