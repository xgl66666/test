#define _SECTION_LDCFG_            __attribute__((section(".ldcfg")))
typedef struct _LD_Cfg
{
    unsigned int Loader_subversion:8;            // [7 :0 ]
    unsigned int Loader_version:8;               // [15:8 ]
    unsigned int Loader_project:8;               // [23:16]
    unsigned int Loader_reserve:8;               // [31:24]
} LD_Cfg;                                             


const LD_Cfg _ld_cfg _SECTION_LDCFG_ = 
{
    .Loader_project = 1,
    .Loader_version = 2,
    .Loader_subversion = 3,
};                                                 


