#ifndef __MXL_DEBUG__
#define __MXL_DEBUG__

#define MXLDBG_ERROR 0x01
#define MXLDBG_ENTER 0x02
#define MXLDBG_EXIT 0x04
#define MXLDBG_API 0x08

#if (MXL_MODULE_DEBUG_LEVEL_MxL542 > 2)
#define MXLDBG3(x) x
#else
#define MXLDBG3(x)
#endif

#if (MXL_MODULE_DEBUG_LEVEL_MxL542 > 1)
#define MXLDBG2(x) x
#else
#define MXLDBG2(x)
#endif

#if (MXL_MODULE_DEBUG_LEVEL_MxL542 > 0)
#define MXLDBG1(x) x
#else
#define MXLDBG1(x)
#endif

#if (MXL_MODULE_DEBUG_OPTIONS_MxL542 & MXLDBG_ERROR)
#define MXLERR(x) x
#else
#define MXLERR(x) 
#endif

#if (MXL_MODULE_DEBUG_OPTIONS_MxL542 & MXLDBG_ENTER)
#define MXLENTER(x) x
#define MXLENTERSTR MXL_MODULE_DEBUG_FCT_MxL542("+++++ %s \n\r", __FUNCTION__);
#else
#define MXLENTER(x) 
#define MXLENTERSTR
#endif

#if (MXL_MODULE_DEBUG_OPTIONS_MxL542 & MXLDBG_EXIT)
#define MXLEXIT(x) x
#define MXLEXITSTR(retcode) MXL_MODULE_DEBUG_FCT_MxL542("----- %s (retcode: %d)\n\r", __FUNCTION__, retcode);
#else
#define MXLEXIT(x) 
#define MXLEXITSTR(retcode)
#endif

#if (MXL_MODULE_DEBUG_OPTIONS_MxL542 & MXLDBG_API)
#define MXLENTERAPI(x) x
#define MXLENTERAPISTR(devId) MXL_MODULE_DEBUG_FCT_MxL542("+++ %s [%d] \n\r", __FUNCTION__, devId);
#define MXLEXITAPI(x) x
#define MXLEXITAPISTR(devId, retcode) MXL_MODULE_DEBUG_FCT_MxL542("--- %s [%d] (retcode: %d)\n\r", __FUNCTION__, devId, retcode);
#else
#define MXLENTERAPI(x)
#define MXLENTERAPISTR(devId)
#define MXLEXITAPI(x)
#define MXLEXITAPISTR(devId, retcode)
#endif

#endif

