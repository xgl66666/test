#ifndef _NOCS11_CSD_IMPL_H
#define _NOCS11_CSD_IMPL_H 1

struct SCsdInitParameters {
};

struct SCsdTerminateParameters {
};

enum ECsdBootMode
{
    CSD_BOOT_MODE_FROM_SPI_FLASH = 0,
    LAST_CSD_BOOT_MODE
};


struct SCsdScrKeyPathHandle {
    MS_U32 u32PidFltId;
};

struct SCsdDscKeyPathHandle {
    MS_U32 u32PidFltId;
};

struct SCsdR2RKeyPathHandle {
};

#define CSD_R2R_NB_OF_PROTECTING_KEYS 2
#define CSD_DSC_NB_OF_PROTECTING_KEYS 2
#define CSD_SCR_NB_OF_PROTECTING_KEYS 2


#endif /* _NOCS11_CSD_IMPL_H */
