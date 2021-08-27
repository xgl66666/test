#ifndef __DYNAMIC_FUNCTION__
#define __DYNAMIC_FUNCTION__

#define DYNAMIC_FUNCTAB         __attribute__((section(".dynamic_functab")))
#define DYNAMIC_FUNCTAB_SDRAM   __attribute__((section(".dynamic_functab_sdram")))
#define DYNAMIC_MODULE_FUNCTAB  __attribute__((section(".dynamic_functab")))
#define DYNAMIC_MODULE_FUNCTAB1 __attribute__((section(".dynamic_functab1")))
#define DYNAMIC_MODULE_FUNCTAB2 __attribute__((section(".dynamic_functab2")))
#define DYNAMIC_MODULE_MANAGER  __attribute__((section(".dynamic_manager")))

#endif //__DYNAMIC_FUNCTION__
