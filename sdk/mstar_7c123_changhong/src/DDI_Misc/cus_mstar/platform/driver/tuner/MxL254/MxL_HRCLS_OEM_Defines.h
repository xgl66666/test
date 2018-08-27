#ifndef __MXL_HRCLS_OEM_DEFINES_H__
#define __MXL_HRCLS_OEM_DEFINES_H__

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/module.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#define MXL_HRCLS_OEM_MAX_BLOCK_WRITE_LENGTH   256     /** maximum number bytes allowed in one I2C block write. Not greater than 256 */
#define MXL_HRCLS_OEM_MAX_BLOCK_READ_LENGTH    800    /** maximum number bytes allowed in one I2C block read. Has to be even number */

#define MXL_MODULE_DEBUG_LEVEL 0 //  3  
#define MXL_MODULE_DEBUG_OPTIONS 0//MXLDBG_ENTER+MXLDBG_EXIT+MXLDBG_ERROR+MXLDBG_API
#define MXL_MODULE_DEBUG_FCT MxL_HRCLS_PRINT 

#ifdef __KERNEL__
#define MxL_HRCLS_DEBUG  printk /** To be replaced by customer's own log function */
#define MxL_HRCLS_ERROR  printk /** To be replaced by customer's own log function */
#define MxL_HRCLS_PRINT  printk /** To be replaced by customer's own log function */
#else
#define MxL_HRCLS_DEBUG  printf /** To be replaced by customer's own log function */
#define MxL_HRCLS_ERROR  printf /** To be replaced by customer's own log function */
#define MxL_HRCLS_PRINT  printf /** To be replaced by customer's own log function */
#endif
#define not_MXL_HRCLS_WAKE_ON_WAN_ENABLED_
#define not_MXL_HRCLS_LITTLE_ENDIAN_

#endif // __MXL_HRCLS_OEM_DEFINES_H__
