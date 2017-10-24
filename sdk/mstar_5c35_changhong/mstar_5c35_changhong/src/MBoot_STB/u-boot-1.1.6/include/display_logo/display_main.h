#ifndef _DISPLAY_MAIN_H
#define _DISPLAY_MAIN_H
//#include <common.h>


typedef struct
{
    U16 u16DispX;
    U16 u16DispY;
    U8 bHorStretch;
    U8 bVerStretch;
}LogoDispConfigParams;

extern void logo_display_main(LogoDispConfigParams *stConfig);
extern void logo_display_osdDestroy(void);
extern U8 logo_display_modeNTSC(void);

#endif

