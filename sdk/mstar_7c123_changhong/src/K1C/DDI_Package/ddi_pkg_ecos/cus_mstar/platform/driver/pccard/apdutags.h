/**********************************************************
 * apdutags.h
 *
 *   Copyright: Rabbit Labs 2010-2013
 **********************************************************/

#ifndef __APDUTAGS_H__
#define __APDUTAGS_H__

#ifdef __cplusplus
namespace DVBCI {

#define  MK_APDU_TAG(lowercase_rsc, uppercase_rsc, lowercase_apdu_name, uppercase_apdu_name, tag, flags, direction) uppercase_rsc ## _ ## uppercase_apdu_name = tag,

#else

#define  MK_APDU_TAG(lowercase_rsc, uppercase_rsc, lowercase_apdu_name, uppercase_apdu_name, tag, flags, direction) CILIB_APDU_ ## uppercase_rsc ## _ ## uppercase_apdu_name = tag,

#endif

typedef enum {
  SPEC_EN50221                 = 0x0001,
  SPEC_TS101699                = 0x0002,
  SPEC_CIPLUS1_2               = 0x0004,
  SPEC_CIPLUS1_3               = 0x0008,
  SPEC_TS103205                = 0x0010,
  SPEC_TS103205_12             = 0x0020,
  SPEC_TS103205_13             = 0x0040,
  SPEC_TS101699_AND_HIGHER     = 0x007E,
  SPEC_CIPLUS1_2_AND_HIGHER    = 0x007C,
  SPEC_CIPLUS1_3_AND_HIGHER    = 0x0078,
  SPEC_TS103205_AND_HIGHER     = 0x0070,
  SPEC_ALL                     = 0x007F,
  SPEC_PRIVATE                 = 0x8000
} spec_support_e;

typedef enum {
  DIRECTION_HOST_TO_CAM        = 0x01,
  DIRECTION_CAM_TO_HOST        = 0x02,
  DIRECTION_BOTH               = 0x03
} apdu_direction_e;


typedef enum {
#include "apdutagstemplate.h"
} apdu_tags_e;

#ifdef __cplusplus
};
#endif

#endif
