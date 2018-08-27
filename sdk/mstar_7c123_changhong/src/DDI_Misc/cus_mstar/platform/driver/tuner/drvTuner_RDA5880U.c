
#include "MsCommon.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "drvIIC.h"

#if IF_THIS_TUNER_INUSE(TUNER_RDA5880U)
#include "drvTuner_RDA5880U.h"

#define RDATV_XTAL              (27)                //XTAL clock(MHz)
#define RDATV_IF_FREQ           (5000)              //IF output(KHz)
#define RDATV_DIG_CONTRAL       1                   // =1,external AGC;  =0 internal AGC,Output amplitude reference RDATV_DIG_POWER
#define RDATV_LOOP_STA          0                   // =1, LOOP on;  =0, LOOP off
#define RDA588U_SLA             0xC2
#define RDATV_DIG_POWER         0x380

//RDA5880U fw
static MS_U8 const rda_code[] =
{
    0x02, 0x05, 0x83, 0x75, 0x0C, 0x00, 0x75, 0x0D, 0x01, 0x75, 0x0E, 0x00, 0x75, 0x0F, 0x08, 0x75,
    0x10, 0x07, 0x75, 0x11, 0xD0, 0x7F, 0x03, 0x12, 0x04, 0xCE, 0x8F, 0x09, 0xE5, 0x09, 0xD3, 0x95,
    0x27, 0x50, 0x0E, 0xE5, 0x09, 0xC3, 0x95, 0x21, 0x40, 0x07, 0x7B, 0x40, 0x7D, 0xA1, 0x02, 0x01,
    0x46, 0xE5, 0x09, 0xD3, 0x95, 0x27, 0x50, 0x03, 0x02, 0x00, 0xE0, 0xE5, 0x09, 0xD3, 0x95, 0x26,
    0x50, 0x03, 0x02, 0x01, 0xA6, 0x7D, 0x99, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0A, 0xE5, 0x0A,
    0xC4, 0x13, 0x54, 0x03, 0xF5, 0x0A, 0x7D, 0x7F, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0B, 0xD3,
    0xEF, 0x95, 0x0F, 0xE5, 0x0E, 0x64, 0x80, 0xF8, 0x74, 0x80, 0x98, 0x50, 0x29, 0xE5, 0x0A, 0x94,
    0x00, 0x50, 0x13, 0x7B, 0x41, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xE4, 0xF5, 0x0B, 0xAB,
    0x0B, 0x7D, 0x7F, 0x02, 0x01, 0x46, 0x7B, 0x42, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x15,
    0x0A, 0x75, 0x0B, 0x80, 0x80, 0x10, 0x7B, 0x43, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xC3,
    0xE5, 0x0B, 0x95, 0x0F, 0xF5, 0x0B, 0xAB, 0x0B, 0x7D, 0x7F, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x7D,
    0x99, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x09, 0xE5, 0x0A, 0xC4, 0x33, 0x54, 0x60, 0xF5, 0x0A,
    0xE5, 0x09, 0x54, 0x9F, 0x25, 0x0A, 0xF5, 0x0A, 0xFB, 0x7D, 0x99, 0x7F, 0x01, 0x12, 0x07, 0x3C,
    0xAB, 0x11, 0xAA, 0x10, 0x7D, 0x01, 0x7F, 0x03, 0x12, 0x04, 0xD4, 0x8F, 0x09, 0x02, 0x00, 0x3B,
    0xE5, 0x09, 0xC3, 0x95, 0x21, 0x40, 0x03, 0x02, 0x01, 0xA6, 0xE5, 0x09, 0xC3, 0x95, 0x26, 0x40,
    0x03, 0x02, 0x01, 0xA6, 0x7D, 0x99, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0A, 0xE5, 0x0A, 0xC4,
    0x13, 0x54, 0x07, 0xF5, 0x0A, 0x53, 0x0A, 0x03, 0x7D, 0x7F, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F,
    0x0B, 0xC3, 0x74, 0xFF, 0x95, 0x0F, 0xFF, 0xE4, 0x95, 0x0E, 0xFE, 0xC3, 0xE5, 0x0B, 0x9F, 0xEE,
    0x64, 0x80, 0xF8, 0x74, 0x80, 0x98, 0x40, 0x35, 0xE5, 0x0A, 0x95, 0x0D, 0xE5, 0x0C, 0x64, 0x80,
    0xF8, 0x74, 0x80, 0x98, 0x40, 0x17, 0x7B, 0x44, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x75,
    0x0B, 0xFF, 0xAB, 0x0B, 0x7D, 0x7F, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x80, 0x59, 0x7B, 0x45, 0x7D,
    0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x05, 0x0A, 0x75, 0x0B, 0x80, 0x80, 0x0F, 0x7B, 0x46, 0x7D,
    0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xE5, 0x0F, 0x25, 0x0B, 0xF5, 0x0B, 0xAB, 0x0B, 0x7D, 0x7F,
    0x7F, 0x01, 0x12, 0x07, 0x3C, 0x7D, 0x99, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x09, 0xE5, 0x0A,
    0xC4, 0x33, 0x54, 0x60, 0xF5, 0x0A, 0xE5, 0x09, 0x54, 0x9F, 0x25, 0x0A, 0xF5, 0x0A, 0xFB, 0x7D,
    0x99, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xAB, 0x11, 0xAA, 0x10, 0x7D, 0x01, 0x7F, 0x03, 0x12, 0x04,
    0xD4, 0x8F, 0x09, 0x02, 0x00, 0xEA, 0x7F, 0x01, 0x02, 0x07, 0x25, 0x75, 0x0B, 0x0F, 0x75, 0x0C,
    0xA0, 0x75, 0x0D, 0x00, 0x75, 0x0E, 0x01, 0x75, 0x0F, 0x00, 0x75, 0x10, 0x02, 0x7B, 0x20, 0x7A,
    0x4E, 0x7D, 0x01, 0x7F, 0x02, 0x12, 0x04, 0xD4, 0x8F, 0x09, 0x12, 0x06, 0x8C, 0x8F, 0x22, 0xE5,
    0x22, 0x70, 0x05, 0x7B, 0x50, 0x02, 0x02, 0xBB, 0xE5, 0x09, 0xD3, 0x95, 0x23, 0x50, 0x0C, 0xE5,
    0x09, 0xC3, 0x95, 0x1F, 0x40, 0x05, 0x7B, 0x51, 0x02, 0x02, 0xBB, 0xE5, 0x09, 0xC3, 0x95, 0x1F,
    0x40, 0x03, 0x02, 0x02, 0x89, 0xE5, 0x09, 0xC3, 0x95, 0x1F, 0x50, 0x62, 0x12, 0x06, 0x8C, 0xEF,
    0x4E, 0x70, 0x03, 0x02, 0x03, 0x3E, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0A, 0xE5,
    0x0A, 0xC4, 0x54, 0x0F, 0xF5, 0x0A, 0xC3, 0x94, 0x0F, 0x40, 0x05, 0x7B, 0x52, 0x02, 0x02, 0xBB,
    0x7B, 0x53, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x05, 0x0A, 0x7D, 0x68, 0x7F, 0x01, 0x12,
    0x07, 0x4E, 0x8F, 0x09, 0xE5, 0x0A, 0xC4, 0x54, 0xF0, 0xF5, 0x0A, 0xE5, 0x09, 0x54, 0x0F, 0xFF,
    0xE5, 0x0A, 0x54, 0xF0, 0x2F, 0xF5, 0x0A, 0xFB, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xAB,
    0x0C, 0xAA, 0x0B, 0x7D, 0x01, 0x7F, 0x02, 0x12, 0x04, 0xD4, 0x8F, 0x09, 0x80, 0x97, 0xE5, 0x0A,
    0xC4, 0x54, 0x0F, 0xF5, 0x0A, 0xC3, 0x74, 0x0F, 0x95, 0x10, 0xFF, 0xE4, 0x95, 0x0F, 0xFE, 0xC3,
    0xE5, 0x0A, 0x9F, 0xEE, 0x64, 0x80, 0xF8, 0x74, 0x80, 0x98, 0x40, 0x06, 0x75, 0x0A, 0x0F, 0x02,
    0x03, 0x1A, 0x05, 0x0A, 0x05, 0x0A, 0x02, 0x03, 0x1A, 0xE5, 0x09, 0xD3, 0x95, 0x23, 0x50, 0x03,
    0x02, 0x03, 0x3E, 0xE5, 0x09, 0xD3, 0x95, 0x23, 0x40, 0x63, 0x12, 0x06, 0x8C, 0xEF, 0x4E, 0x70,
    0x03, 0x02, 0x03, 0x3E, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0A, 0xE5, 0x0A, 0xC4,
    0x54, 0x0F, 0xF5, 0x0A, 0xD3, 0x94, 0x00, 0x50, 0x06, 0x7B, 0x53, 0x7D, 0xA1, 0x80, 0x7A, 0x7B,
    0x54, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x15, 0x0A, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07,
    0x4E, 0x8F, 0x09, 0xE5, 0x0A, 0xC4, 0x54, 0xF0, 0xF5, 0x0A, 0xE5, 0x09, 0x54, 0x0F, 0xFF, 0xE5,
    0x0A, 0x54, 0xF0, 0x2F, 0xF5, 0x0A, 0xFB, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xAB, 0x0C,
    0xAA, 0x0B, 0x7D, 0x01, 0x7F, 0x02, 0x12, 0x04, 0xD4, 0x8F, 0x09, 0x80, 0x96, 0xE5, 0x0A, 0xC4,
    0x54, 0x0F, 0xF5, 0x0A, 0xD3, 0x95, 0x10, 0xE5, 0x0F, 0x64, 0x80, 0xF8, 0x74, 0x80, 0x98, 0x50,
    0x05, 0xE4, 0xF5, 0x0A, 0x80, 0x04, 0x15, 0x0A, 0x15, 0x0A, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07,
    0x4E, 0x8F, 0x09, 0xE5, 0x0A, 0xC4, 0x54, 0xF0, 0xF5, 0x0A, 0xE5, 0x09, 0x54, 0x0F, 0xFF, 0xE5,
    0x0A, 0x54, 0xF0, 0x2F, 0xF5, 0x0A, 0xFB, 0x7D, 0x68, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x7F, 0x01,
    0x02, 0x07, 0x25, 0x75, 0x0C, 0x00, 0x75, 0x0D, 0x0A, 0x7F, 0x01, 0x12, 0x04, 0xCE, 0x8F, 0x09,
    0xE5, 0x09, 0xC3, 0x95, 0x20, 0x40, 0x0E, 0xE5, 0x09, 0xD3, 0x95, 0x25, 0x50, 0x07, 0x7B, 0x30,
    0x7D, 0xA1, 0x02, 0x04, 0x6B, 0xE5, 0x09, 0xC3, 0x95, 0x20, 0x40, 0x03, 0x02, 0x04, 0x14, 0xE5,
    0x09, 0xC3, 0x95, 0x24, 0x40, 0x03, 0x02, 0x04, 0xC9, 0x7D, 0x4E, 0x7F, 0x01, 0x12, 0x07, 0x4E,
    0x8F, 0x0A, 0x7D, 0x4B, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0B, 0x53, 0x0B, 0x07, 0xC3, 0x74,
    0xFF, 0x95, 0x0D, 0xFF, 0xE4, 0x95, 0x0C, 0xFE, 0xC3, 0xE5, 0x0A, 0x9F, 0xEE, 0x64, 0x80, 0xF8,
    0x74, 0x80, 0x98, 0x40, 0x2E, 0xE5, 0x0B, 0x94, 0x07, 0x40, 0x13, 0x7B, 0x31, 0x7D, 0xA1, 0x7F,
    0x01, 0x12, 0x07, 0x3C, 0x75, 0x0A, 0xFF, 0xAB, 0x0A, 0x7D, 0x4E, 0x02, 0x04, 0x6B, 0x7B, 0x32,
    0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x05, 0x0B, 0xE5, 0x1E, 0xC3, 0x13, 0x24, 0x80, 0xF5,
    0x0A, 0x80, 0x0F, 0x7B, 0x33, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xE5, 0x0D, 0x25, 0x0A,
    0xF5, 0x0A, 0xAB, 0x0A, 0x7D, 0x4E, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x7D, 0x4B, 0x7F, 0x01, 0x12,
    0x07, 0x4E, 0x8F, 0x09, 0xE5, 0x09, 0x54, 0xF8, 0x25, 0x0B, 0xF5, 0x0B, 0xFB, 0x7D, 0x4B, 0x7F,
    0x01, 0x12, 0x07, 0x3C, 0x7B, 0xD0, 0x7A, 0x07, 0x7D, 0x01, 0x7F, 0x01, 0x12, 0x04, 0xD4, 0x8F,
    0x09, 0x02, 0x03, 0x6F, 0xE5, 0x09, 0xD3, 0x95, 0x25, 0x50, 0x03, 0x02, 0x04, 0xC9, 0xE5, 0x09,
    0xD3, 0x95, 0x24, 0x50, 0x03, 0x02, 0x04, 0xC9, 0x7D, 0x4E, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F,
    0x0A, 0x7D, 0x4B, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x0B, 0x53, 0x0B, 0x07, 0x7E, 0x00, 0xE5,
    0x0D, 0x25, 0x1E, 0xFF, 0xEE, 0x35, 0x0C, 0xFE, 0xD3, 0xE5, 0x0A, 0x9F, 0xEE, 0x64, 0x80, 0xF8,
    0x74, 0x80, 0x98, 0x50, 0x32, 0xE5, 0x0B, 0x94, 0x00, 0x50, 0x17, 0x7B, 0x34, 0x7D, 0xA1, 0x7F,
    0x01, 0x12, 0x07, 0x3C, 0x85, 0x1E, 0x0A, 0xAB, 0x0A, 0x7D, 0x4E, 0x7F, 0x01, 0x12, 0x07, 0x3C,
    0x80, 0x57, 0x7B, 0x35, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x15, 0x0B, 0xE5, 0x1E, 0xC3,
    0x13, 0x24, 0x80, 0xF5, 0x0A, 0x80, 0x10, 0x7B, 0x36, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07, 0x3C,
    0xC3, 0xE5, 0x0A, 0x95, 0x0D, 0xF5, 0x0A, 0xAB, 0x0A, 0x7D, 0x4E, 0x7F, 0x01, 0x12, 0x07, 0x3C,
    0x7D, 0x4B, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x09, 0xE5, 0x09, 0x54, 0xF8, 0x25, 0x0B, 0xF5,
    0x0B, 0xFB, 0x7D, 0x4B, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0x7B, 0xD0, 0x7A, 0x07, 0x7D, 0x01, 0x7F,
    0x01, 0x12, 0x04, 0xD4, 0x8F, 0x09, 0x02, 0x04, 0x1E, 0x7F, 0x01, 0x02, 0x07, 0x25, 0x7B, 0x20,
    0x7A, 0x4E, 0x7D, 0x01, 0x8F, 0x17, 0x8D, 0x18, 0x8A, 0x19, 0x8B, 0x1A, 0xE4, 0xFF, 0xF5, 0x1B,
    0xE5, 0x17, 0xB4, 0x01, 0x31, 0xE4, 0xF5, 0x1C, 0xF5, 0x1D, 0xC3, 0xE5, 0x1D, 0x95, 0x1A, 0xE5,
    0x19, 0x64, 0x80, 0xF8, 0xE5, 0x1C, 0x64, 0x80, 0x98, 0x50, 0x19, 0x7D, 0x2B, 0x7F, 0x01, 0x12,
    0x07, 0x4E, 0xEF, 0xD3, 0x95, 0x1B, 0x40, 0x02, 0x8F, 0x1B, 0x05, 0x1D, 0xE5, 0x1D, 0x70, 0xDA,
    0x05, 0x1C, 0x80, 0xD6, 0xAF, 0x1B, 0xE5, 0x17, 0xB4, 0x03, 0x31, 0xE4, 0xF5, 0x1C, 0xF5, 0x1D,
    0xC3, 0xE5, 0x1D, 0x95, 0x1A, 0xE5, 0x19, 0x64, 0x80, 0xF8, 0xE5, 0x1C, 0x64, 0x80, 0x98, 0x50,
    0x19, 0x7D, 0x8D, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0xEF, 0xD3, 0x95, 0x1B, 0x40, 0x02, 0x8F, 0x1B,
    0x05, 0x1D, 0xE5, 0x1D, 0x70, 0xDA, 0x05, 0x1C, 0x80, 0xD6, 0xAF, 0x1B, 0xE5, 0x17, 0xB4, 0x02,
    0x31, 0xE4, 0xF5, 0x1C, 0xF5, 0x1D, 0xC3, 0xE5, 0x1D, 0x95, 0x1A, 0xE5, 0x19, 0x64, 0x80, 0xF8,
    0xE5, 0x1C, 0x64, 0x80, 0x98, 0x50, 0x19, 0x7D, 0x6B, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0xEF, 0xD3,
    0x95, 0x1B, 0x40, 0x02, 0x8F, 0x1B, 0x05, 0x1D, 0xE5, 0x1D, 0x70, 0xDA, 0x05, 0x1C, 0x80, 0xD6,
    0xAF, 0x1B, 0x22, 0x78, 0x7F, 0xE4, 0xF6, 0xD8, 0xFD, 0x75, 0x81, 0x27, 0x02, 0x05, 0xCA, 0x02,
    0x06, 0x0F, 0xE4, 0x93, 0xA3, 0xF8, 0xE4, 0x93, 0xA3, 0x40, 0x03, 0xF6, 0x80, 0x01, 0xF2, 0x08,
    0xDF, 0xF4, 0x80, 0x29, 0xE4, 0x93, 0xA3, 0xF8, 0x54, 0x07, 0x24, 0x0C, 0xC8, 0xC3, 0x33, 0xC4,
    0x54, 0x0F, 0x44, 0x20, 0xC8, 0x83, 0x40, 0x04, 0xF4, 0x56, 0x80, 0x01, 0x46, 0xF6, 0xDF, 0xE4,
    0x80, 0x0B, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x90, 0x07, 0x06, 0xE4, 0x7E, 0x01,
    0x93, 0x60, 0xBC, 0xA3, 0xFF, 0x54, 0x3F, 0x30, 0xE5, 0x09, 0x54, 0x1F, 0xFE, 0xE4, 0x93, 0xA3,
    0x60, 0x01, 0x0E, 0xCF, 0x54, 0xC0, 0x25, 0xE0, 0x60, 0xA8, 0x40, 0xB8, 0xE4, 0x93, 0xA3, 0xFA,
    0xE4, 0x93, 0xA3, 0xF8, 0xE4, 0x93, 0xA3, 0xC8, 0xC5, 0x82, 0xC8, 0xCA, 0xC5, 0x83, 0xCA, 0xF0,
    0xA3, 0xC8, 0xC5, 0x82, 0xC8, 0xCA, 0xC5, 0x83, 0xCA, 0xDF, 0xE9, 0xDE, 0xE7, 0x80, 0xBE, 0xE4,
    0xF5, 0x08, 0xFB, 0x7D, 0x76, 0x7F, 0x01, 0x12, 0x07, 0x3C, 0xAB, 0x08, 0x7D, 0x77, 0x7F, 0x01,
    0x12, 0x07, 0x3C, 0x7D, 0x71, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x25, 0x7D, 0x70, 0x7F, 0x01,
    0x12, 0x07, 0x4E, 0x8F, 0x20, 0x7D, 0x72, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x24, 0x7D, 0x73,
    0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x1E, 0x7D, 0x65, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x23,
    0x7D, 0x67, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x1F, 0x7D, 0x85, 0x7F, 0x01, 0x12, 0x07, 0x4E,
    0x8F, 0x26, 0x7D, 0x91, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x8F, 0x08, 0xE5, 0x26, 0x25, 0x08, 0xF5,
    0x27, 0xC3, 0xE5, 0x26, 0x95, 0x08, 0xF5, 0x21, 0x7B, 0x20, 0x7D, 0xA1, 0x7F, 0x01, 0x12, 0x07,
    0x3C, 0x12, 0x03, 0x43, 0x12, 0x01, 0xAB, 0x12, 0x00, 0x03, 0x80, 0xEC, 0xE4, 0xF5, 0x13, 0xF5,
    0x14, 0xF5, 0x15, 0xF5, 0x16, 0x7B, 0x20, 0x7A, 0x4E, 0xF5, 0x22, 0x7D, 0x01, 0x7F, 0x01, 0x12,
    0x04, 0xD4, 0x8F, 0x11, 0xE5, 0x11, 0xC3, 0x95, 0x20, 0x40, 0x0D, 0xE5, 0x11, 0xD3, 0x95, 0x25,
    0x50, 0x06, 0x7E, 0x00, 0x7F, 0x01, 0x80, 0x04, 0x7E, 0x00, 0x7F, 0x00, 0x8E, 0x15, 0x8F, 0x16,
    0xE5, 0x16, 0x64, 0x01, 0x45, 0x15, 0x70, 0x02, 0x80, 0x34, 0x7D, 0x4E, 0x7F, 0x01, 0x12, 0x07,
    0x4E, 0x8F, 0x12, 0x7D, 0x4B, 0x7F, 0x01, 0x12, 0x07, 0x4E, 0x53, 0x07, 0x07, 0xE5, 0x11, 0xC3,
    0x95, 0x20, 0x50, 0x0B, 0xE5, 0x12, 0xB4, 0xFF, 0x06, 0xBF, 0x07, 0x03, 0x75, 0x22, 0x01, 0xE5,
    0x11, 0xD3, 0x95, 0x25, 0x40, 0x0B, 0xE5, 0x12, 0xB5, 0x1E, 0x06, 0xEF, 0x70, 0x03, 0x75, 0x22,
    0x01, 0xAF, 0x22, 0x7E, 0x00, 0x22, 0x01, 0x27, 0x68, 0x01, 0x21, 0x60, 0x01, 0x26, 0x64, 0x01,
    0x23, 0x00, 0x01, 0x1F, 0x00, 0x01, 0x25, 0x70, 0x01, 0x20, 0x20, 0x01, 0x24, 0x40, 0x01, 0x22,
    0x00, 0x01, 0x1E, 0x00, 0x00, 0x7D, 0xFA, 0x7C, 0x00, 0xAE, 0x07, 0x1F, 0xEE, 0x60, 0x0C, 0xED,
    0x1D, 0xAA, 0x04, 0x70, 0x01, 0x1C, 0x4A, 0x60, 0xF0, 0x80, 0xF4, 0x22, 0x75, 0x90, 0xFF, 0x8F,
    0x80, 0xC2, 0xB5, 0xD2, 0xB5, 0x8D, 0x90, 0x8B, 0x80, 0xC2, 0xB5, 0xD2, 0xB5, 0x22, 0x75, 0x90,
    0xFF, 0x8F, 0x80, 0xC2, 0xB5, 0xD2, 0xB5, 0x8D, 0x90, 0xAF, 0x90, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static struct   rda_tuner_cfg RDA_tuner_cfg[MAX_FRONTEND_NUM];
static MS_U16   freq_if_tbl[RDA_TV_MODE_QTY] = {2900, 2650, 2400, 2400, 2000, 2000, 2900, 2900, 0, 0, 0};
static MS_U16   const  rfpll_sel_tbl[FREQ_LO_INDEX_QTY] = {0x10, 0x8, 0x4, 0x2, 0x1};
static MS_U8    const  mixer_lo_sel_tbl[FREQ_LO_INDEX_QTY] = {0x1, 0x0, 0x0, 0x0, 0x0};
static MS_U32   const  freq_vco_core_tbl[FREQ_LO_INDEX_QTY] = {2, 4, 8, 16, 32};
static MS_U32   const  freq_adc_tbl[3] = {162000, 151875, 172125};
static MS_U32   freq_if_out_tbl[RDA_TV_MODE_QTY] = {4400, 4650, 4200, 4200, 3100, 3100, 4100, 4100, RDATV_IF_FREQ , RDATV_IF_FREQ , RDATV_IF_FREQ };
static RDA_TV_ADC_BY_FRE const  adc_tbl[] = //dtv
{
    {52500 , 2000, ADC_162},
    {60500 , 2000, ADC_172},       //DTMB 60.5M  172   update 20160802           
    {68500 , 2000, ADC_162},
    {80000 , 2000, ADC_172},
    {171000 , 2000, ADC_152},
    {179000 , 2000, ADC_162},
    {219000 , 2000, ADC_162},
//  {538000 ,2000,ADC_172},    // can optimize 0.5dB
//  {562000 ,2000,ADC_172},    // can optimize 0.5dB
    {610000 , 2000, ADC_162},
    {642000 , 2000, ADC_152},
    {674000 , 2000, ADC_162},
    {690000 , 2000, ADC_162},
    {714000 , 2000, ADC_172},
    {722000 , 2000, ADC_172},
    {738000 , 2000, ADC_172},
    {770000 , 2000, ADC_162},
};
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM]; //MAX_FRONTEND_NUM

void RDA_DELAY_MS(MS_U32 cnt)
{
    MsOS_DelayTask(cnt);
}

MS_BOOL i2cSendData(MS_U8 u8TunerIndex, MS_U8 paddr, MS_U16 pu8data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Value[3];
    HWI2C_PORT hwi2c_port;

    u8Value[0] = paddr;
    u8Value[1] = pu8data >> 8;
    u8Value[2] = pu8data & 0xff;

    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        bRet &= MDrv_IIC_Write(RDA588U_SLA, 0, 0, u8Value, 3);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        bRet &= MDrv_IIC1_Write(RDA588U_SLA, 0, 0, u8Value, 3);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    if (bRet == FALSE)
    {
        TUNER_ERR((" ERROR in RDA5880U WriteReg !!! \n"));
    }
    return bRet;
}

MS_U16 i2cGetData(MS_U8 u8TunerIndex, MS_U8 paddr)
{
    MS_BOOL bRet = TRUE;
    MS_U8 pu8data[2] = {0};
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
       bRet &= MDrv_IIC_Read(RDA588U_SLA, &paddr, 1, pu8data, 2);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
       bRet &= MDrv_IIC1_Read(RDA588U_SLA, &paddr, 1, pu8data, 2);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    if (bRet == FALSE)
    {
        TUNER_ERR((" ERROR in RDA5880U ReadReg !!! \n"));
    }

    return (MS_U16)(pu8data[0] << 8 | pu8data[1]);
}

void rdaTVPowerOn(MS_U8 u8TunerIndex)
{
    //power_on
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0xe2, 0x0002);
    i2cSendData(u8TunerIndex, 0xe2, 0x003a);
    i2cSendData(u8TunerIndex, 0xe4, 0xae6c);        //ana
    i2cSendData(u8TunerIndex, 0xe4, 0xaeec);
    i2cSendData(u8TunerIndex, 0xe2, 0x001a);
    i2cSendData(u8TunerIndex, 0xe2, 0x003a);
    i2cSendData(u8TunerIndex, 0x30, 0x811c);
    i2cSendData(u8TunerIndex, 0x4E, 0xFCFF);
    i2cSendData(u8TunerIndex, 0x4C, 0x83F9);
    i2cSendData(u8TunerIndex, 0x50, 0x006C);
    i2cSendData(u8TunerIndex, 0x60, 0x0800);        //pll_cal_enable_clk=1
    i2cSendData(u8TunerIndex, 0x22, 0x8F08);
    i2cSendData(u8TunerIndex, 0x24, 0xC380);
    i2cSendData(u8TunerIndex, 0x26, 0x240B);
    i2cSendData(u8TunerIndex, 0x2A, 0x0060);        //BUFF_GAIN=1
    //BBPLL
    i2cSendData(u8TunerIndex, 0x2E, 0x0288);
    i2cSendData(u8TunerIndex, 0x40, 0x3089);    //bbpll_cp_bit=001
    i2cSendData(u8TunerIndex, 0x42, 0x8a06);
    i2cSendData(u8TunerIndex, 0x46, 0x0387);
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x46, 0x0800);        //bbpll_sdm_int_dec_sel=000
    //RFPLL
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x44, 0xEECC);
    i2cSendData(u8TunerIndex, 0x48, 0xC828);
    i2cSendData(u8TunerIndex, 0x4A, 0xC06C);        //rfpll_phase_ctrl_dly = 11
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0x0C, 0xC3A0);        //clk_gate_enb = 1,LO 7GHz_Tone
    i2cSendData(u8TunerIndex, 0x0E, 0x0780);
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x1C, 0xA020);            //mixer_i2v_rauchen=1
    i2cSendData(u8TunerIndex, 0x1E, 0x2F3F);
    //DC setting
    i2cSendData(u8TunerIndex, 0xFF, 0x0002);
    i2cSendData(u8TunerIndex, 0x36, 0x0C7F);
    i2cSendData(u8TunerIndex, 0x38, 0xF480);
    i2cSendData(u8TunerIndex, 0x3A, 0xE480);
    i2cSendData(u8TunerIndex, 0x3C, 0x3003);
    i2cSendData(u8TunerIndex, 0x0C, 0x22CF);
    i2cSendData(u8TunerIndex, 0x0E, 0x40C0);
    i2cSendData(u8TunerIndex, 0x10, 0x0880);
    //LNA_volt
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    //i2cSendData(u8TunerIndex, 0x12,0x8001);
    i2cSendData(u8TunerIndex, 0x12, 0x4001);         //lna_ibit =010,ÓÅ»¯¹¦ºÄ    20160802
    i2cSendData(u8TunerIndex, 0x16, 0xF088);      //lna_reg_ibit = 1000,lna_lpf_res = 111
    //dig_outside  RAM_delay
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x40, 0x0000);     //ram_delay_timer = 0,DTV
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x2C, 0x4402);     //pdt_sdm_gain_dgcvin = 010,extern agc£¬0V non-oscillation
    //imbalace
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x02, 0x0080);     //notch_bypass = 1
    i2cSendData(u8TunerIndex, 0x1C, 0x0040);     //imbala_bypass = 0
    i2cSendData(u8TunerIndex, 0x20, 0xC0CC);
    i2cSendData(u8TunerIndex, 0x22, 0x0404);
}

void rdaTVAGC(MS_U8 u8TunerIndex)
{

    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x24, 0xc320);     //adc_gain=0010 0dB
    i2cSendData(u8TunerIndex, 0x2c, 0x4002);     //pdt_sdm_gain_mix=100,pdt_sdm_gain_lna=000,pdt_sdm_gain_dgcvin=010
    //lna
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x10, 0x7400);    //lna_mos_sel=101,overtop 100£¬LO no 7GHZ Tone
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0x20, 0x0018);     //range_mode & sdm_out_pesk_inv=11
    i2cSendData(u8TunerIndex, 0x24, 0x0208);    //lna_tmr_bw_index = 0x08
    i2cSendData(u8TunerIndex, 0x26, 0x75ff);
    i2cSendData(u8TunerIndex, 0x28, 0x41ff);    //agc_lna_g1_polarity[11]=0
    i2cSendData(u8TunerIndex, 0x2c, 0x133f);    //agc_lna_peak_avg_shift_ct=01
    i2cSendData(u8TunerIndex, 0x2e, 0x622f);
    i2cSendData(u8TunerIndex, 0x32, 0x1e00);
    i2cSendData(u8TunerIndex, 0x34, 0x0b00);
    i2cSendData(u8TunerIndex, 0x36, 0x0580);
    i2cSendData(u8TunerIndex, 0x38, 0x0000);     //agc_lna_thd_hold_index=0
    i2cSendData(u8TunerIndex, 0x4c, 0x00ff);    //agc_lna_g1_ini&max
    //lna_thd
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0x50, 0x4000);
    i2cSendData(u8TunerIndex, 0x52, 0x4000);
    i2cSendData(u8TunerIndex, 0x54, 0x4000);
    i2cSendData(u8TunerIndex, 0x56, 0x4000);
    i2cSendData(u8TunerIndex, 0x58, 0x4000);
    i2cSendData(u8TunerIndex, 0x5a, 0x4000);
    i2cSendData(u8TunerIndex, 0x5c, 0x4000);
    i2cSendData(u8TunerIndex, 0x5e, 0x4000);
    //lna_gain_stage
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0xc0, 0xc0c0);
    i2cSendData(u8TunerIndex, 0xc2, 0xd1c0);
    i2cSendData(u8TunerIndex, 0xc4, 0xefe3);
    i2cSendData(u8TunerIndex, 0xc6, 0xfff8);
    i2cSendData(u8TunerIndex, 0xc8, 0x7631);
    i2cSendData(u8TunerIndex, 0xca, 0x7777);
    //mixer
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0x60, 0x0010);        //range_mode & sdm_out_pesk_inv=10
    i2cSendData(u8TunerIndex, 0x62, 0x0211);
    i2cSendData(u8TunerIndex, 0x64, 0x7400);
    i2cSendData(u8TunerIndex, 0x66, 0x2c50);
    i2cSendData(u8TunerIndex, 0x68, 0x400f);        //agc_mixer_tp_sel =010
    //mixer gain stage 1dB
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x7e, 0x0001);
    i2cSendData(u8TunerIndex, 0x7c, 0x0307);
    i2cSendData(u8TunerIndex, 0x7a, 0x0f1f);
    i2cSendData(u8TunerIndex, 0x78, 0x3f41);
    i2cSendData(u8TunerIndex, 0x76, 0x4347);
    i2cSendData(u8TunerIndex, 0x74, 0x4f5f);
    i2cSendData(u8TunerIndex, 0x72, 0x7f7f);
    i2cSendData(u8TunerIndex, 0x70, 0x7f7f);
    //mixer_cap=1111
    i2cSendData(u8TunerIndex, 0x3a, 0xffff);
    i2cSendData(u8TunerIndex, 0x64, 0xff00);
    i2cSendData(u8TunerIndex, 0x6a, 0xff01);
    i2cSendData(u8TunerIndex, 0x6c, 0xffff);
    i2cSendData(u8TunerIndex, 0x6e, 0xffff);
    //flt gain stage
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x3c, 0x0300);
    i2cSendData(u8TunerIndex, 0x3e, 0x0303);
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0x82, 0x0000);        //agc_flt_vga_thd_hold_prog_up&dn=0
    i2cSendData(u8TunerIndex, 0x86, 0x0275);
    i2cSendData(u8TunerIndex, 0x84, 0x6404);
    i2cSendData(u8TunerIndex, 0x88, 0x2080);
    i2cSendData(u8TunerIndex, 0x8A, 0x655F);        //agc_flt_vga_tp_sel=110, peak value
    i2cSendData(u8TunerIndex, 0x8e, 0xe433);
    i2cSendData(u8TunerIndex, 0x90, 0x0500);
    i2cSendData(u8TunerIndex, 0x92, 0x0100);
    i2cSendData(u8TunerIndex, 0x94, 0x0080);
    i2cSendData(u8TunerIndex, 0x96, 0x2704);
    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0xa2, 0x0240);
    i2cSendData(u8TunerIndex, 0xa4, 0x80ff);
    i2cSendData(u8TunerIndex, 0xa6, 0x2808);
    i2cSendData(u8TunerIndex, 0xa8, 0x0200);
    i2cSendData(u8TunerIndex, 0xac, 0x0390);
    i2cSendData(u8TunerIndex, 0xae, 0x4338);
    i2cSendData(u8TunerIndex, 0xb0, 0x6688);
    i2cSendData(u8TunerIndex, 0xB2, 0x2000);
    i2cSendData(u8TunerIndex, 0xDA, 0x7400);        // dig gain offset {-2dB,30dB}

}

void rdaDTVBW_8M(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    if (RDA_tuner_cfg[u8TunerIndex].mdlt_mode == LNA_DTMB_MODE)
    {
        i2cSendData(u8TunerIndex, 0x20, 0x0027);        //filter cap=0111,4.1M; CN based on filter BW
    }
    else
    {
        i2cSendData(u8TunerIndex, 0x20, 0x0029);            //filter cap=1001,3.8M
    }
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x28, 0x01dd);                //channel=111
}

void rdaDTVBW_7M(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x20, 0x002d);                //filter cap=1101,3.4M
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x28, 0x0199);                //channel=110
}

void rdaDTVBW_6M(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x20, 0x002f);            //filter cap=1111,3.2M
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x28, 0x0155);            //channel=101
}

void rdaTVAGCReset(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    i2cSendData(u8TunerIndex, 0x20, 0x0008);
    i2cSendData(u8TunerIndex, 0x20, 0x0028);                //fitler reset
    i2cSendData(u8TunerIndex, 0x22, 0x8F00);
    i2cSendData(u8TunerIndex, 0x22, 0x8F08);            //vga reset
    i2cSendData(u8TunerIndex, 0x28, 0x9188);
    i2cSendData(u8TunerIndex, 0x28, 0x91a8);                //dac reset
    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x06, 0x000b);
    i2cSendData(u8TunerIndex, 0x06, 0x0007);                //adc reset
}

void rdaTV8051Init(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    i2cSendData(u8TunerIndex, 0x70, 0x7221);
    i2cSendData(u8TunerIndex, 0x72, 0x0040);
}

void rdaTV8051Start(MS_U8 u8TunerIndex)
{
    MS_U16 tmp;
    //dr gain
    i2cSendData(u8TunerIndex, 0xFF, 0x0001); //LNA_Index
    tmp = i2cGetData(u8TunerIndex, 0x4A);
    tmp = tmp | 0x1000;
    i2cSendData(u8TunerIndex, 0x4A, tmp);

    tmp = i2cGetData(u8TunerIndex, 0x4E);       //LNA_gain
    tmp = tmp | 0x0100;
    i2cSendData(u8TunerIndex, 0x4E, tmp);

    tmp = i2cGetData(u8TunerIndex, 0x68);       //Mixer_Index
    tmp = tmp | 0x1000;
    i2cSendData(u8TunerIndex, 0x68, tmp);

    tmp = i2cGetData(u8TunerIndex, 0x98);       //Filter & VGA gain
    tmp = tmp | 0x8100;
    i2cSendData(u8TunerIndex, 0x98, tmp);


    // open 8051
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    i2cSendData(u8TunerIndex, 0x30, 0x7221);
    i2cSendData(u8TunerIndex, 0x30, 0x011D);
}

void rdaTV8051Stop(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    i2cSendData(u8TunerIndex, 0x30, 0x011C);
    i2cSendData(u8TunerIndex, 0x30, 0x811C);

    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    i2cSendData(u8TunerIndex, 0x4A, 0x0707);
    i2cSendData(u8TunerIndex, 0x4E, 0x0080);
    i2cSendData(u8TunerIndex, 0x68, 0x408F);
    i2cSendData(u8TunerIndex, 0x98, 0x6000);
}

void rdaDemodFirmwareDL(MS_U8 u8TunerIndex)
{
    MS_U32 size = 0;
    MS_U32 i = 0;
    MS_U16 buff = 0;

    size = sizeof(rda_code);
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    i2cSendData(u8TunerIndex, 0x30, 0x011E);
    RDA_DELAY_MS(100);

    for (i = 0; i < size; i += 2)
    {
        buff = rda_code[i];
        buff = buff << 8;
        buff = buff | rda_code[i + 1];
        i2cSendData(u8TunerIndex, 0xFE, buff);
    }
    size = 0;
    i2cSendData(u8TunerIndex, 0x30, 0x011C);
    i2cSendData(u8TunerIndex, 0x30, 0x811C);
    RDA_DELAY_MS(100);
}

void rdaTVSoftwareVersion(MS_U8 u8TunerIndex)
{
    i2cSendData(u8TunerIndex, 0xFF, 0x0002);
    i2cSendData(u8TunerIndex, 0x70, RDA_SW_VERSION);
}

void rdaTVSetDemod(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U16 tmp = 0;
    tuner_cfg->freq_if_out = RDATV_IF_FREQ;     //set IF_out
    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0xAC);
    tmp = tmp & 0xFC00;
    tmp += RDATV_DIG_POWER;
    i2cSendData(u8TunerIndex, 0xAC, tmp);                   //set IF_amplitude

    //set lna_mos_sel based on modulation
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x10);
    tmp = tmp & 0xE3FF;
    if (tuner_cfg->mdlt_mode == LNA_DVBC_MODE)
        tmp += 0x1400;              // lna_mos_sel = 101;
    else
        tmp += 0x1400;              // lna_mos_sel = 101;
    i2cSendData(u8TunerIndex, 0x10, tmp);
}

void rdaTVIQSwap(MS_U8 u8TunerIndex, MS_U8 flag)
{
    MS_U16 tmp;

    i2cSendData(u8TunerIndex, 0xFF, 0x0002);
    tmp = i2cGetData(u8TunerIndex, 0x1C);

    if (flag)
    {
        tmp |= 0x0010;
    }
    else
    {
        tmp &= 0xffef;
    }
    i2cSendData(u8TunerIndex, 0xFF, 0x0002);
    i2cSendData(u8TunerIndex, 0x1C, tmp);
}

void rdaTVTunerOpen(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    rdaTVPowerOn(u8TunerIndex);
    rdaTVAGC(u8TunerIndex);
    rdaTVSoftwareVersion(u8TunerIndex);

    rdaTVI2vTh(u8TunerIndex, tuner_cfg);
    rdaTVSetDemod(u8TunerIndex, tuner_cfg);
    rdaTVSetAGCMode(u8TunerIndex);  //set AGC Mode base on Demod
    rdaTVSetLoopSta(u8TunerIndex);  //set LOOP status
    rdaTV8051Init(u8TunerIndex);            //8051 AGC Setting
    rdaTV8051Stop(u8TunerIndex);
    rdaDemodFirmwareDL(u8TunerIndex);       //8051 download
    rdaTVAGCReset(u8TunerIndex);
}

void rdaTVSetAGCMode(MS_U8 u8TunerIndex)

{
    MS_U16 tmp = 0;

    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0xAC);
    if (RDATV_DIG_CONTRAL == 1)
    {
        tmp = tmp | 0x0400;             //AGC_Outside
    }
    else
    {
        tmp = tmp & 0xFBFF;             //AGC_Inside
    }
    i2cSendData(u8TunerIndex, 0xAC, tmp);
}

void rdaTVSetLoopSta(MS_U8 u8TunerIndex)
{
    //MS_U16 tmp=0;

    i2cSendData(u8TunerIndex,0xFF,0x0000); 
    if(RDATV_LOOP_STA == 1)
    {
        i2cSendData(u8TunerIndex,0x4E,0xFCFF); //LOOP_on
    }
    else
    {
        i2cSendData(u8TunerIndex,0x4E,0xFCDF); //LOOP_off
    }
}
void rdaTVLNAIndexGet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U32 freq_lo;

    freq_lo = tuner_cfg->freq_lo;
    if ((freq_lo > 710000) && (freq_lo <= 1000000))
    {
        tuner_cfg->lna_index = LNA_SEL_L1;
    }
    else if ((freq_lo > 290000) && (freq_lo <= 710000))
    {
        tuner_cfg->lna_index = LNA_SEL_L2;
    }
    else if ((freq_lo > 133000) && (freq_lo <= 290000))
    {
        tuner_cfg->lna_index = LNA_SEL_L3;
    }
    else if ((freq_lo > 30000) && (freq_lo <= 133000))
    {
        tuner_cfg->lna_index = LNA_SEL_L4;
    }
    else
    {
        tuner_cfg->lna_index = LNA_SEL_L0;
    }
}

void rdaTVFreqloIndexSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U32 freq_lo_m;
    MS_U32 freq_max = 1000000;

    freq_lo_m = tuner_cfg->freq_lo;

    if (freq_lo_m <= freq_max && freq_lo_m > (freq_max / 2))
    {
        tuner_cfg->freq_lo_index = FREQ_LO_INDEX_2;
    }
    else if (freq_lo_m <= (freq_max / 2) && freq_lo_m > (freq_max / 4))
    {
        tuner_cfg->freq_lo_index = FREQ_LO_INDEX_4;
    }
    else if (freq_lo_m <= (freq_max / 4) && freq_lo_m > (freq_max / 8))
    {
        tuner_cfg->freq_lo_index = FREQ_LO_INDEX_8;
    }
    else if (freq_lo_m <= (freq_max / 8) && freq_lo_m > (freq_max / 16))
    {
        tuner_cfg->freq_lo_index = FREQ_LO_INDEX_16;
    }
    else if (freq_lo_m <= (freq_max / 16) && freq_lo_m > (freq_max / 32))
    {
        tuner_cfg->freq_lo_index = FREQ_LO_INDEX_32;
    }
    else
    {
        tuner_cfg->freq_lo_index = FREQ_LO_INDEX_QTY;
    }
}

/*
MS_U32 rdaTVDetected(void)
{
    MS_U16 tmp;

    tmp = i2cGetData(u8TunerIndex,0xe0);
    if(tmp == RDA5880DM_PRODUCT_ID)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
*/

void rdaTVPllCalSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U16 tmp;
    MS_U32 pll_cal_freq_compare;

    i2cSendData(u8TunerIndex, 0xff, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x66);
    tmp = (tmp & 0xFC00) + tuner_cfg->xtal * 2;
    i2cSendData(u8TunerIndex, 0x66, tmp);

    pll_cal_freq_compare = (tuner_cfg->freq_vco_core * 2) / 1000;

    i2cSendData(u8TunerIndex, 0x64, (i2cGetData(u8TunerIndex, 0x64) & 0xfffe) + (pll_cal_freq_compare >> 16)); //write 64
    i2cSendData(u8TunerIndex, 0x62, pll_cal_freq_compare);  // write 62


    i2cSendData(u8TunerIndex, 0x66, tmp & 0xefff);
    i2cSendData(u8TunerIndex, 0x66, tmp | 0x1000); //pll cal reset
}

void rdaTVRFPLLSDMSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U32 rfpll_sdm_freq = 0;
    MS_U32 freq_tmp = tuner_cfg->freq_vco_core;
    MS_U16 tmp;

    rfpll_sdm_freq = rdaTVMulDivCompensate(u8TunerIndex, freq_tmp, tuner_cfg->xtal * 1000, 21); //fix(freq_vco_core/xtal*2^21)

    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x50, rfpll_sdm_freq >> 16);
    i2cSendData(u8TunerIndex, 0x4E, rfpll_sdm_freq);

    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0x0A);
    tmp &= 0xfffd;
    i2cSendData(u8TunerIndex, 0x0A, tmp);
    tmp |= 0x0002;
    i2cSendData(u8TunerIndex, 0x0A, tmp);     //rfpll_sdm reset
}

RDA_TV_ADC_INDEX rdaTVGetADCbyFreqMode(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{

    MS_U16 strADCQuotient[3] = {0, 0, 0}; 
    MS_U32 strADCRemainder[3] = {0, 0, 0}; 

    MS_U16 i, ADC = 0, ADC_right = 0, tmp_bw;
    MS_U32 tmp, tmp_right;
    MS_U16 dwD1;   
    MS_U32 dwR1;  

    if ((tuner_cfg->tv_mode_index == RDA_M) | (tuner_cfg->tv_mode_index == RDA_N) | (tuner_cfg->tv_mode_index == RDA_DTV_6M))
        tmp_bw = 3000 * 12;
    else
        tmp_bw = 4000 * 12;

    for (i = 0; i < 3; ++i)
    {
        dwD1 = (tuner_cfg->freq_lo * 12) / freq_adc_tbl[i];
        dwR1 = (tuner_cfg->freq_lo * 12) % freq_adc_tbl[i];
        //check ADC_tone at  BW?
        if (dwR1 <= tmp_bw)
        {
            strADCQuotient[i] = dwD1;
            if ((freq_adc_tbl[i]*strADCQuotient[i] >= tuner_cfg->freq * 12))
            {
                strADCRemainder[i] = freq_adc_tbl[i] * strADCQuotient[i] - tuner_cfg->freq * 12;
            }
            else
            {
                if (tuner_cfg->tv_mode_index >= RDA_DTV_6M)
                {
                    strADCRemainder[i] = tuner_cfg->freq * 12 - freq_adc_tbl[i] * strADCQuotient[i];
                }
                else
                {
                    strADCRemainder[i] = 0xffffffff - (tuner_cfg->freq - freq_adc_tbl[i]) * strADCQuotient[i];
                }
            }
        }
        else if (dwR1 >= (freq_adc_tbl[i] - tmp_bw))
        {
            strADCQuotient[i] = dwD1 + 1;
            if ((freq_adc_tbl[i]*strADCQuotient[i] >= tuner_cfg->freq * 12))
            {
                strADCRemainder[i] = freq_adc_tbl[i] * strADCQuotient[i] - tuner_cfg->freq * 12;
            }
            else
            {
                if (tuner_cfg->tv_mode_index >= RDA_DTV_6M)
                {
                    strADCRemainder[i] = tuner_cfg->freq * 12 - freq_adc_tbl[i] * strADCQuotient[i];
                }
                else
                {
                    strADCRemainder[i] = 0xffffffff - (tuner_cfg->freq - freq_adc_tbl[i]) * strADCQuotient[i];
                }
            }
        }
        else
        {
            ADC = i;
            goto result;
        }
    }

//  ADC_Tone at Y_Right
    tmp_right = 0xffffffff;
    for (i = 0; i < 3; ++i)
    {
        if (strADCRemainder[i] > 0x7fffffff)
        {
            if (strADCQuotient[i] % 6 > 0)
            {
                if (strADCRemainder[i] < tmp_right)
                {
                    tmp_right = strADCRemainder[i];
                    ADC_right = i;
                }
            }
        }
    }
    if (tuner_cfg->mdlt_mode == LNA_ATV_MODE)
    {
        if (tmp_right != 0xffffffff)
        {
            ADC = ADC_right;
            goto result;
        }
    }
    else
    {
        tmp_right = 0xffffffff - tmp_right;
    }


//  ADC_Tone at Y_Left
    tmp = 0;
    for (i = 0; i < 3; ++i)
    {
        if (strADCRemainder[i] < 0x7fffffff)
        {
            if (strADCQuotient[i] % 6 > 0)
            {
                if (strADCRemainder[i] > tmp)
                {
                    tmp = strADCRemainder[i];
                    ADC = i;
                }
            }
        }
    }

    if (tmp_right > tmp)
    {
        ADC = ADC_right;
    }

result:
    for (i = 0; i < (sizeof(adc_tbl) / sizeof(RDA_TV_ADC_BY_FRE)); i++)
    {
        if ((tuner_cfg->freq >= adc_tbl[i].freq - adc_tbl[i].delta) && (tuner_cfg->freq <= adc_tbl[i].freq + adc_tbl[i].delta))
            ADC = adc_tbl[i].adc;
    }

    return ADC;

}

void rdaTVBBPLLSDMSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U32 bbpll_sdm_freq;
    MS_U16 tmp;
    bbpll_sdm_freq = rdaTVMulDivCompensate(u8TunerIndex, freq_adc_tbl[tuner_cfg->freq_adc_index], tuner_cfg->xtal * 1000, 24);

    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    tmp = i2cGetData(u8TunerIndex, 0x44);
    tmp |= 0x1000;
    i2cSendData(u8TunerIndex, 0x44, tmp);
    tmp = tmp & 0xf000;
    i2cSendData(u8TunerIndex, 0x44, tmp + (bbpll_sdm_freq >> 16));
    i2cSendData(u8TunerIndex, 0x42, bbpll_sdm_freq + 1);

    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0x0A);

    tmp &= 0xfffe;
    i2cSendData(u8TunerIndex, 0x0A, tmp);

    tmp |= 0x0001;
    i2cSendData(u8TunerIndex, 0x0A, tmp);     //bbpll_sdm reset

}

void rdaTVTunerModeSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U16 tmp;

    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0x0C);

    //set if_out_mode & mode
    if (tuner_cfg->out_mode == TUNER_OUT_MODE_IF)
    {
        if (tuner_cfg->tv_mode_index >= RDA_DTV_6M)
        {
            tmp |= 0x4000;
            tmp &= 0xcfff;
        }
        else
        {
            tmp |= 0x1000;
            tmp &= 0x9fff;
        }
    }
    else
    {
        if (tuner_cfg->tv_mode_index >= RDA_DTV_6M)
        {
            tmp |= 0x3000;
            tmp &= 0xbfff;
        }
        else
        {
            tmp |= 0x2000;
            tmp &= 0xafff;
        }
    }

    tmp &= 0xfc7f;
    if (tuner_cfg->tv_mode_index >= RDA_DTV_6M)
    {
        tmp |= (0x0007 << 7);  //set tv mode
    }
    else
    {
        tmp |= ((tuner_cfg->tv_mode_index) << 7);  //set tv mode
    }

    //set clock mode
    tmp &= 0xff8f;
    if (tuner_cfg->freq_adc_index == ADC_152)
    {
        tmp |= ((tuner_cfg->freq_adc_index) << 4);
    }
    else if (tuner_cfg->freq_adc_index == ADC_162)
    {
        tmp |= (((tuner_cfg->freq_adc_index) + 2) << 4);
    }
    else if (tuner_cfg->freq_adc_index == ADC_172)
    {
        tmp |= (((tuner_cfg->freq_adc_index) + 1) << 4);
    }
    i2cSendData(u8TunerIndex, 0x0C, tmp);
}

void rdaTVIfOutSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U32 freq_shift;
    MS_U16 tmp;

    if (tuner_cfg->tv_mode_index >= RDA_DTV_6M)
    {
        freq_shift = tuner_cfg->freq_if_out;
    }
    else
    {
        freq_shift = freq_if_out_tbl[tuner_cfg->tv_mode_index];
    }

    tmp = rdaTVMulDivCompensate(u8TunerIndex, freq_shift, freq_adc_tbl[tuner_cfg->freq_adc_index], 17);

    tmp |= 0x8000;    //set angle in

    i2cSendData(u8TunerIndex, 0xff, 0x0002);
    i2cSendData(u8TunerIndex, 0x32, tmp);

    i2cSendData(u8TunerIndex, 0xff, 0x0001);
    i2cSendData(u8TunerIndex, 0x02, 0x0000);
    i2cSendData(u8TunerIndex, 0x02, 0x0001); //dsp_reset reset
}

void large_int_64div32(MS_U32* psa, MS_U32 dwsb, MS_U32* psd, MS_U32* psr)
{
    MS_U32 dwR, dwD0, dwR0;
    MS_U8 i = 0, ns = 0;
    MS_U32 qwR[2] = {0};
    MS_U32 dwA0 = psa[1];
    MS_U32 dwA1 = psa[0];
    MS_U32 dwD1 = dwA1 / dwsb;
    MS_U32 dwR1 = dwA1 % dwsb;

    dwR = dwR1;
    i = 0;
    while (dwR)
    {
        dwR >>= 1;
        i ++;
    }
    if (i)
    {
        ns = 32 - i;
        dwR = (dwA0 << ns) >> ns;
        dwA0 = (dwA0 >> i) | (dwR1 << ns) ;
    }

    dwD0 = dwA0 / dwsb;
    dwR0 = dwA0 % dwsb;

    /*
        ULARGE_INTEGER ulD = { dwD0  , 0  };
        ulD.QuadPart <<= i;
        ulD.HighPart += dwD1;

          QWORD qwR = ((QWORD)dwR0 << i) + dwR ;
          */

    if (i > 0)
    {
        qwR[0] = dwR0 >> ns;
    }

    qwR[1] = dwR0 << i;
    qwR[1] += dwR;

    //    qwR = dwD0<<i|(dwD1<<32)
    if ((qwR[1] > dwsb) || (qwR[0] > 0))
    {
        large_int_64div32((MS_U32 *)qwR, dwsb, (MS_U32 *)psd, psr);
        //*(QWORD*)psd = dwD0<<i|(dwD1<<32) + *(QWORD*)psd;
        *psd += dwD0 >> ns;
        *psd += dwD1;
        if (*(psd + 1) + dwD0 < dwD0)
        {
            *psd += 1;
        }
        *(psd + 1) += (dwD0 << i);
    }
    else if (qwR[1] == dwsb)
    {
        //*(QWORD*)psd = dwD0<<i|(dwD1<<32) + 1;
        *psd = dwD0 >> ns;
        *(psd + 1) += dwD0 << i;
        *psd += dwD1;
        if (*(psd + 1) == 0xffffffff)
        {
            *psd += 1;
        }
        *(psd + 1) += 1;
        *psr = 0;
    }
    else
    {
        //*(QWORD*)psd = dwD0<<i|(dwD1<<32);
        if (i > 0)
        {
            *psd = dwD0 >> ns;
        }

        *(psd + 1) += dwD0 << i;
        *psd += dwD1;
        *psr = *((MS_U32 *)qwR);
    }

    //*psr = (DWORD)( *(QWORD*)psa - *(QWORD*)psd * dwsb );
}

/*
lna_cap = ((10^12)/(((2*Pi*freq)^2)*A)-B*1000)/D
*/
MS_U16 rdaTVLNACapGet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U32 freq;
    MS_U32 div_larg[2] = {0x2330429A, 0x7CAE2117};   //0x2330429A7CAE2116+1
//    MS_U32 div_larg[2] = {0x5a1525,0x6286dc7e};    //0x5A15256286DC7D+1
    MS_U32 ret[2] = {0};
    MS_U32 psr, auto_A, auto_B, auto_D;
    MS_U32 tmp;
    auto_A = 0;
    auto_B = 0;
    auto_D = 0;

    div_larg[0] = 0x2330429A;
    div_larg[1] = 0x7CAE2117;
    freq = tuner_cfg->freq_lo;

    if (tuner_cfg->lna_index == LNA_SEL_L1)
    {
        auto_A = 75;
        auto_B = 31200;             //*10000
        auto_D = 414;               //*10
        goto DIV_BG;
    }

    if (tuner_cfg->lna_index == LNA_SEL_L2)
    {
        auto_A = 220;
        auto_B = 27900;
        auto_D = 541;
    }
    else if (tuner_cfg->lna_index == LNA_SEL_L3)
    {
        auto_A = 820;
        auto_B = 32140;
        auto_D = 803;
    }
    else if (tuner_cfg->lna_index == LNA_SEL_L4)
    {
        auto_A = 2700;
        auto_B = 39600;
        auto_D = 1436;
    }

DIV_BG:
    large_int_64div32((MS_U32 *)div_larg, freq, (MS_U32 *)ret, (MS_U32 *)&psr);
    div_larg[0] = 0;
    div_larg[1] = 0;

    psr = ret[0];
    psr = ret[1];
    large_int_64div32((MS_U32 *)ret, freq, (MS_U32 *)div_larg, (MS_U32 *)&psr);

    psr = div_larg[0];
    psr = div_larg[1];

    tmp = div_larg[1];

    tmp = tmp / auto_A;

    if (tmp > auto_B)
    {
        tmp = tmp - auto_B;
    }
    else
    {
        tmp = 0;
    }

    if (tmp < (auto_D * 0xff))
    {
        tmp = tmp / auto_D;
    }
    else
    {
        tmp = 0x13ff;
    }
    tmp = tmp;


    return tmp;
}

void rdaTVI2vTh(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    MS_U16 tmp, M, N;
    tmp = 0;
    M = 0;
    N = 0;
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    i2cSendData(u8TunerIndex, 0x4E, 0xFC7F);
    RDA_DELAY_MS(50);

    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0x6A);
    tmp &= 0xfff0;
    M = tmp >> 4;
    N = M + 0x700;

    if (N >= 0xE00)
    {
        N = 0xE00;
    }

    tmp = tmp | (N << 4);
    tuner_cfg->i2vth_dtvh = tmp;
    tuner_cfg->i2vth_dtvl = tuner_cfg->i2vth_dtvh >> 4;
    tuner_cfg->i2vth_dtvl = tuner_cfg->i2vth_dtvl >> 1;
    tuner_cfg->i2vth_dtvl = (tuner_cfg->i2vth_dtvl << 4);

    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    i2cSendData(u8TunerIndex, 0x4E, 0xFCFF);
    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    i2cSendData(u8TunerIndex, 0x64, tuner_cfg->i2vth_dtvh);
    i2cSendData(u8TunerIndex, 0x66, tuner_cfg->i2vth_dtvl);

    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x16);
    M = tmp;
    tmp = tmp & 0xff0f;
    i2cSendData(u8TunerIndex, 0x16, tmp);       //Close LNA

    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    tmp = i2cGetData(u8TunerIndex, 0x2A);           //read LNA
    tuner_cfg->LNA_Sel_off = (tmp >> 4) & 0x0fff;

    i2cSendData(u8TunerIndex, 0xFF, 0x0000);         //Open LNA
    i2cSendData(u8TunerIndex, 0x16, M);
}

MS_U32 rdaTVMulDivCompensate(MS_U8 u8TunerIndex, MS_U32 par1, MS_U32 par2, MS_U32 par3)
{
    MS_U32 result = 0;
    MS_U32 div_int, div_remain, div_remain1;
    MS_U32 mremain, mb, fremain;

    div_int = par1 / par2;
    div_remain = par1 % par2;

    div_int <<= par3;
    result += div_int;

    mremain = ((MS_U32)1 << (32 - par3)) - 1;
    mb = div_remain / mremain;
    fremain = div_remain % mremain;

    div_int = (fremain << par3) / par2;
    result += div_int;

    div_remain1 = (fremain << par3) % par2;

    div_int = (mremain << par3) / par2;
    div_int *= mb;
    result += div_int;

    div_remain = (mremain << par3) % par2;
    div_remain *= mb;
    div_remain += div_remain1;

    div_int = div_remain / par2;
    result += div_int;

    div_remain1 = div_remain % par2;
    div_remain1 *= 2;

    if (div_remain1 >= par2)
        result += 1;

    return result;

}

void rdaTVSleepnoloop(MS_U8 u8TunerIndex)           //powerdown£¬all pu=0
{
    rdaTV8051Stop(u8TunerIndex);		//ÏÈ¹Ø±Õ8051ÔÙÖ´ÐÐ´ý»ú²Ù×÷
    //53uA
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    i2cSendData(u8TunerIndex, 0xe4, 0xA66C);
}

void rdaTVSleepwithloop(MS_U8 u8TunerIndex)     //powerup
{
    rdaTV8051Stop(u8TunerIndex);		//ÏÈ¹Ø±Õ8051ÔÙÖ´ÐÐ´ý»ú²Ù×÷
    //57mA
    //i2cSendData(u8TunerIndex,0xFF,0x0000);
    //i2cSendData(u8TunerIndex,0x4E,0x00A0);	

    //20mA    close LNA_band_sel   updata by lina  20170308
    i2cSendData(u8TunerIndex,0xFF,0x0000);
    i2cSendData(u8TunerIndex,0xE4,0xACCC);
    i2cSendData(u8TunerIndex,0xE2,0x001A);
    i2cSendData(u8TunerIndex,0xE2,0x003A);
    i2cSendData(u8TunerIndex,0x30,0x811C);
    i2cSendData(u8TunerIndex,0x4E,0x00A0);
    i2cSendData(u8TunerIndex,0x10,0x6000);
    i2cSendData(u8TunerIndex,0x12,0x0003);
    i2cSendData(u8TunerIndex,0x16,0xf008);
    i2cSendData(u8TunerIndex,0x50,0x0000);

}

MS_U32  rdaTVGetIntensity(MS_U8 u8TunerIndex)
{
    MS_U16 tmp = 0;
    MS_U32 Lna_Index = 0, lna_gain = 0;
    MS_U32 i2v_gain = 0;
    MS_U32 filter_gain = 0;
    MS_U32 vga_gain = 0;
    MS_U32 digital_gain_p = 0;
    MS_U32 digital_gain_n = 0;
    MS_U32 intensity = 0;

    i2cSendData(u8TunerIndex, 0xFF, 0x0001);
    //lna gain
    tmp = i2cGetData(u8TunerIndex, 0x4A);
    Lna_Index = (tmp >> 8) & 0x0007;
    Lna_Index = (Lna_Index * 3) + 1;
    tmp = i2cGetData(u8TunerIndex, 0x4E);
    lna_gain =  tmp & 0x00FF;
    lna_gain = (lna_gain * 11) >> 8;
    lna_gain += Lna_Index;

    //i2v gain
    tmp = i2cGetData(u8TunerIndex, 0x68);
    i2v_gain = (tmp >> 4) & 0x000F;

    //filter gain
    tmp = i2cGetData(u8TunerIndex, 0x98);
    tmp = (tmp >> 13) & 0x3;
    if (tmp == 0)
        filter_gain = 0;
    else
        filter_gain = 12;

    //vga gain
    tmp = i2cGetData(u8TunerIndex, 0x7E);
    tmp = (tmp >> 8) & 0x00ff;
    vga_gain = (tmp * 18) >> 8;


    //digital gain
    tmp = i2cGetData(u8TunerIndex, 0xE6);
    tmp = tmp & 0x7fff;
    if (tmp < 0x4000)           //dig_gain is positive gain
    {
        digital_gain_p = tmp >> 8;
        digital_gain_n = 0;
    }
    else                        //dig_gain is negative gain
    {
        digital_gain_n = (0x7fff - tmp) >> 8;
        digital_gain_p = 0;
    }

    intensity = lna_gain + i2v_gain + filter_gain + vga_gain;
    intensity = intensity + digital_gain_p - digital_gain_n - 10;

    return intensity;
}

MS_U16 rdaTVSingalStrength(MS_U8 u8TunerIndex)
{
    MS_U32 vgain = 0;
    MS_U16 signal_strength = 0;

    vgain = rdaTVGetIntensity(u8TunerIndex);
    if (vgain > 1000)
    {
        vgain = 0;
    }
    if (vgain > 100)
        vgain = 100;
    signal_strength = 100 - vgain;

    if (signal_strength > 90)
    {
        signal_strength = 90;
    }
    if (signal_strength < 20)
    {
        signal_strength = 20;
    }

    return signal_strength;
}

void rdaTVGetLObyMode(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg)
{
    tuner_cfg->freq_lo = tuner_cfg->freq + freq_if_tbl[tuner_cfg->tv_mode_index];
}

void rdaTVSetFreq_tune(MS_U8 u8TunerIndex, MS_U32 freq, MS_U32 bw, RDA_TUNER_CFG tuner_cfg)
{
    MS_U16 tmp, lna_cap, tmp_a;

    tuner_cfg->freq = freq;

    rdaTVGetLObyMode(u8TunerIndex, tuner_cfg);
    rdaTVLNAIndexGet(u8TunerIndex, tuner_cfg);
    rdaTVFreqloIndexSet(u8TunerIndex, tuner_cfg);

    //set LNA_CAP   0x010[7:0]
    lna_cap = rdaTVLNACapGet(u8TunerIndex, tuner_cfg);
    lna_cap &= 0x00ff;

    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x10);
    tmp &= 0xff00;
    tmp |= lna_cap;
    i2cSendData(u8TunerIndex, 0x10, tmp);

    //set LNA_Band  0x016[7:4]
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x16);
    tmp &= 0xff0f;
    tmp_a = 0x0008;
    tmp_a = tmp_a >> (tuner_cfg->lna_index);
    tmp_a = tmp_a << 4;
    tmp |= tmp_a;
    i2cSendData(u8TunerIndex, 0x16, tmp);

    //set rfpll_sel 0x04a[14:10]
    tuner_cfg->freq_vco_core = (tuner_cfg->freq_lo) << (tuner_cfg->freq_lo_index + 1);
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x4a);
    tmp &= 0x83ff;
    tmp_a = rfpll_sel_tbl[tuner_cfg->freq_lo_index] << 10;
    tmp = tmp_a | tmp;
    i2cSendData(u8TunerIndex, 0x4a, tmp);

    //set mixer_lo_sel  0x01c[7]
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0x1c);
    tmp &= 0xff7f;
    tmp |= mixer_lo_sel_tbl[tuner_cfg->freq_lo_index] << 7;
    i2cSendData(u8TunerIndex, 0x1c, tmp);

    //set div23     0x04a[15]  0x04a[3:2]
    tmp = i2cGetData(u8TunerIndex, 0x4a);
    if (tuner_cfg->freq_vco_core >= RDA_VCOBIT)
    {
        if (tuner_cfg->freq_vco_core >= 1490000)
            tmp &= 0x7FFB;
        else
        {
            tmp &= 0x7FFF;
            tmp |= 0x000C;
        }
    }
    else
    {
        tmp |= 0x800C;
    }
    i2cSendData(u8TunerIndex, 0x4a, tmp);

    rdaTVPllCalSet(u8TunerIndex, tuner_cfg);

    rdaTVRFPLLSDMSet(u8TunerIndex, tuner_cfg);

    // tuner_cfg->freq_adc_index = rdaTVGetAdcByFreq(tuner_cfg->freq);
    tuner_cfg->freq_adc_index = rdaTVGetADCbyFreqMode(u8TunerIndex, tuner_cfg);
    rdaTVBBPLLSDMSet(u8TunerIndex, tuner_cfg);
    rdaTVTunerModeSet(u8TunerIndex, tuner_cfg);
    rdaTVIfOutSet(u8TunerIndex, tuner_cfg);

    //AGV_reset
    //rdaTVAGCReset();
    i2cSendData(u8TunerIndex, 0xff, 0x0001);        //set dig step&updata to fast
    i2cSendData(u8TunerIndex, 0xB0, 0x6688);

    RDA_DELAY_MS(50);    //wait 50ms

    i2cSendData(u8TunerIndex, 0xff, 0x0001);        //set dig step&updata to slow
    i2cSendData(u8TunerIndex, 0xB0, 0xCC54);
}

void rdaTVInitial(MS_U8 u8TunerIndex, RDA_TV_DEMOD_MACH mDemod, RDA_MODULUTION_MODE mmdlt_mode)
{
    RDA_tuner_cfg[u8TunerIndex].demod_mach = mDemod;
    RDA_tuner_cfg[u8TunerIndex].mdlt_mode = mmdlt_mode;
    RDA_tuner_cfg[u8TunerIndex].xtal = RDATV_XTAL;
    RDA_tuner_cfg[u8TunerIndex].out_mode = TUNER_OUT_MODE_IF;
    RDA_tuner_cfg[u8TunerIndex].tv_mode_index = RDA_TV_MODE_QTY;
    rdaTVTunerOpen(u8TunerIndex, &RDA_tuner_cfg[u8TunerIndex]);
}

void rdaTVDtvFreqTuneSet(MS_U8 u8TunerIndex, MS_U32 freq, MS_U16 eBand)
{
    rdaTV8051Stop(u8TunerIndex);

    switch (eBand)
    {
        case 6000:
            RDA_tuner_cfg[u8TunerIndex].tv_mode_index =  RDA_DTV_6M;
            rdaDTVBW_6M(u8TunerIndex);
            break;

        case 7000:
            RDA_tuner_cfg[u8TunerIndex].tv_mode_index = RDA_DTV_7M;
            rdaDTVBW_7M(u8TunerIndex);
            break;

        case 8000:
            RDA_tuner_cfg[u8TunerIndex].tv_mode_index = RDA_DTV_8M;
            rdaDTVBW_8M(u8TunerIndex);
            break;

        default:
            RDA_tuner_cfg[u8TunerIndex].tv_mode_index = RDA_DTV_8M;
            rdaDTVBW_8M(u8TunerIndex);
            break;
    }

    //rdaTVIQSwap(u8TunerIndex, 0);//updata delete by lina 20170223

    rdaTVSetFreq_tune(u8TunerIndex, freq, 0, &RDA_tuner_cfg[u8TunerIndex]);
    rdaTV8051Start(u8TunerIndex);
}

MS_BOOL MDrv_RDA5880UTuner_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam)
{
    RDA_MODULUTION_MODE mmdlt_mode;

    if (pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
    {
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;
    }

    if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBC)
        mmdlt_mode = LNA_DVBC_MODE;
    else if ((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2))
        mmdlt_mode = LNA_DVBT_MODE;
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == ISDBT)
        mmdlt_mode = LNA_ISDBT_MODE;
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DTMB)
        mmdlt_mode = LNA_DTMB_MODE;
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == ATSC)
        mmdlt_mode = LNA_ATSC_MODE;
    else
        mmdlt_mode = LNA_ATV_MODE;

    rdaTVInitial(u8TunerIndex, 0, mmdlt_mode);
    return TRUE;
}

MS_BOOL MDrv_RDA5880UTuner_SetTuner(MS_U8 u8TunerIndex, MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MS_U32 RF_KHz;
    MS_U32 eBandWidth_RDA;

    RF_KHz = dwFreq;
    eBandWidth_RDA = ucBw * 1000;
    if(RF_KHz == ~0)
    {
        return FALSE;
    }
    rdaTVDtvFreqTuneSet(u8TunerIndex, RF_KHz, eBandWidth_RDA);
    return TRUE;
}

#define RDA588U_CHIP_ID0 0xAC
#define RDA588U_CHIP_ID1 0x4C
MS_BOOL MDrv_RDA5880U_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 u8data[2] = {0};
    MS_U16 tmp;
    i2cSendData(u8TunerIndex, 0xFF, 0x0000);
    tmp = i2cGetData(u8TunerIndex, 0xe4);
    u8data[0] = tmp >> 8;
    u8data[1] = tmp & 0xff;
    // if (((u8data[1]) != RDA588U_CHIP_ID1) || (u8data[0] != RDA588U_CHIP_ID0))
    if((u8data[0] >> 4) != 0xA)
        return FALSE;

    if (NULL != pu32channel_cnt)
        *(pu32channel_cnt) += 1;

    return TRUE;
}

MS_BOOL MDrv_RDA5880U_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* u32IF_Freq)
{
    if (RDA_tuner_cfg[u8TunerIndex].tv_mode_index >= RDA_DTV_6M)
    {
        *u32IF_Freq = RDA_tuner_cfg[u8TunerIndex].freq_if_out;
    }
    else
    {
        *u32IF_Freq = freq_if_out_tbl[RDA_tuner_cfg[u8TunerIndex].tv_mode_index];
    }

    return TRUE;
}

MS_BOOL MDrv_RDA5880U_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch (fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            *(float*)data = rdaTVSingalStrength(u8TunerIndex);
            break;

        case TUNER_EXT_FUNC_POWER_ON_OFF:
            if (FALSE == *(MS_BOOL *)data) //power off
            {
                rdaTVSleepnoloop(u8TunerIndex);
            }
            else
            {
                rdaTVSleepwithloop(u8TunerIndex);
            }
            break;

        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n", fuction_type));
            break;
    }
    return TRUE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_RDA5880U) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_RDA5880U",          // tuner system name
    .data               = TUNER_RDA5880U,            // private data value
    .Init               = MDrv_RDA5880UTuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_RDA5880UTuner_SetTuner,
    .CheckExist         = MDrv_RDA5880U_CheckExist,
    .GetTunerIF         = MDrv_RDA5880U_GetTunerIF,
    .Extension_Function = MDrv_RDA5880U_Extension_Function
};

#endif

