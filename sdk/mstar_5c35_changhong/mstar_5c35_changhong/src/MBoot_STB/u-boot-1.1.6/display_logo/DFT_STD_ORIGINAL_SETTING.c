
#if ENABLE_DLC
static MS_U8 tLumaCurve[16] =
//    { 0x06, 0x0F, 0x1E, 0x2D, 0x41, 0x59, 0x6D, 0x7B, 0x8B, 0x9A, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8 };
    { 0x06, 0x12, 0x23, 0x37, 0x4B, 0x5A, 0x6C, 0x7C, 0x8D, 0x9B, 0xAA, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8 };
static MS_U8 tLumaCurve2_a[16] =
//    { 0x06, 0x13, 0x21, 0x32, 0x43, 0x56, 0x66, 0x76, 0x85, 0x95, 0xA4, 0xB5, 0xC5, 0xD8, 0xE8, 0xF8 };
    { 0x03, 0x0F, 0x23, 0x39, 0x4F, 0x5F, 0x71, 0x81, 0x8F, 0x9A, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8 };

static MS_U8 tLumaCurve2_b[16] =
//    { 0x04, 0x1C, 0x32, 0x43, 0x53, 0x61, 0x70, 0x7C, 0x8A, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8 };
    { 0x0D, 0x25, 0x38, 0x48, 0x59, 0x65, 0x70, 0x7C, 0x8A, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8 };
#endif // #if ENABLE_DLC

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

#include "Gamma12BIT_256.c" // include Gamma Talbe
//====================================================================================

