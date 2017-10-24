#ifndef CYGONCE_PKGCONF_SYSTEM_H
#define CYGONCE_PKGCONF_SYSTEM_H
/*
 * File <pkgconf/system.h>
 *
 * This file is generated automatically by the configuration
 * system. It should not be edited. Any changes to this file
 * may be overwritten.
 */

#define CYGNUM_VERSION_CURRENT 0x7fffff00
#define CYGPKG_HAL_MIPS v2_0_60
#define CYGPKG_HAL_MIPS_v2_0_60
#define CYGNUM_HAL_MIPS_VERSION_MAJOR 2
#define CYGNUM_HAL_MIPS_VERSION_MINOR 0
#define CYGNUM_HAL_MIPS_VERSION_RELEASE 60
#define CYGPKG_HAL_MIPS_MIPS32 v2_0_60
#define CYGPKG_HAL_MIPS_MIPS32_v2_0_60
#define CYGNUM_HAL_MIPS_MIPS32_VERSION_MAJOR 2
#define CYGNUM_HAL_MIPS_MIPS32_VERSION_MINOR 0
#define CYGNUM_HAL_MIPS_MIPS32_VERSION_RELEASE 60
#define CYGBLD_HAL_VARIANT_H   <pkgconf/hal_mips_mips32.h>
#define CYGPKG_HAL_MIPS_MALTA v2_0_60
#define CYGPKG_HAL_MIPS_MALTA_v2_0_60
#define CYGNUM_HAL_MIPS_MALTA_VERSION_MAJOR 2
#define CYGNUM_HAL_MIPS_MALTA_VERSION_MINOR 0
#define CYGNUM_HAL_MIPS_MALTA_VERSION_RELEASE 60
#define CYGBLD_HAL_TARGET_H <pkgconf/hal_mips_mips32.h>
#define CYGBLD_HAL_PLATFORM_H <pkgconf/hal_mips_malta.h>

/* Make sure we get the CORE type definitions for HAL_PLATFORM_CPU */
#include CYGBLD_HAL_TARGET_H
#define HAL_PLATFORM_BOARD    "Malta"
#define HAL_PLATFORM_EXTRA    ""

#if defined(CYGHWR_HAL_MIPS_MIPS32_CORE_4Kc)
#  define HAL_PLATFORM_CPU    "MIPS32 4Kc"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_4KEc)
#  define HAL_PLATFORM_CPU    "MIPS32 4KEc"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_4Kp)
#  define HAL_PLATFORM_CPU    "MIPS32 4Kp"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_4Km)
#  define HAL_PLATFORM_CPU    "MIPS32 4Km"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_34Kc)
#  define HAL_PLATFORM_CPU    "MIPS32 34Kc"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_34Kf)
#  define HAL_PLATFORM_CPU    "MIPS32 34Kf"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_74Kc)
#  define HAL_PLATFORM_CPU    "MIPS32 74Kc"
#elif defined(CYGHWR_HAL_MIPS_MIPS32_CORE_74Kf)
#  define HAL_PLATFORM_CPU    "MIPS32 74Kf"
#elif defined(CYGHWR_HAL_MIPS_MIPS64_CORE_5K)
#  define HAL_PLATFORM_CPU    "MIPS64 5K"
#elif defined(CYGHWR_HAL_MIPS_MIPS64_CORE_20K)
#  define HAL_PLATFORM_CPU    "MIPS64 20K"
#else
#  error Unknown Core
#endif

#define CYG_HAL_STARTUP ROM
#define CYG_HAL_STARTUP_ROM
#define CYGHWR_MEMORY_LAYOUT_LDI <pkgconf/mlt_mips_malta_rom.ldi>
#define CYGHWR_MEMORY_LAYOUT_H <pkgconf/mlt_mips_malta_rom.h>
#define CYGPKG_IO_PCI v2_0_60
#define CYGPKG_IO_PCI_v2_0_60
#define CYGNUM_IO_PCI_VERSION_MAJOR 2
#define CYGNUM_IO_PCI_VERSION_MINOR 0
#define CYGNUM_IO_PCI_VERSION_RELEASE 60
#define CYGPKG_HAL v2_0_60
#define CYGPKG_HAL_v2_0_60
#define CYGNUM_HAL_VERSION_MAJOR 2
#define CYGNUM_HAL_VERSION_MINOR 0
#define CYGNUM_HAL_VERSION_RELEASE 60
#define CYGPKG_IO v2_0_60
#define CYGPKG_IO_v2_0_60
#define CYGNUM_IO_VERSION_MAJOR 2
#define CYGNUM_IO_VERSION_MINOR 0
#define CYGNUM_IO_VERSION_RELEASE 60
#define CYGPKG_IO_SERIAL v2_0_60
#define CYGPKG_IO_SERIAL_v2_0_60
#define CYGNUM_IO_SERIAL_VERSION_MAJOR 2
#define CYGNUM_IO_SERIAL_VERSION_MINOR 0
#define CYGNUM_IO_SERIAL_VERSION_RELEASE 60
#define CYGPKG_INFRA v2_0_60
#define CYGPKG_INFRA_v2_0_60
#define CYGNUM_INFRA_VERSION_MAJOR 2
#define CYGNUM_INFRA_VERSION_MINOR 0
#define CYGNUM_INFRA_VERSION_RELEASE 60
#define CYGPKG_KERNEL v2_0_60
#define CYGPKG_KERNEL_v2_0_60
#define CYGNUM_KERNEL_VERSION_MAJOR 2
#define CYGNUM_KERNEL_VERSION_MINOR 0
#define CYGNUM_KERNEL_VERSION_RELEASE 60
#define CYGPKG_MEMALLOC v2_0_60
#define CYGPKG_MEMALLOC_v2_0_60
#define CYGNUM_MEMALLOC_VERSION_MAJOR 2
#define CYGNUM_MEMALLOC_VERSION_MINOR 0
#define CYGNUM_MEMALLOC_VERSION_RELEASE 60
#define CYGPKG_ISOINFRA v2_0_60
#define CYGPKG_ISOINFRA_v2_0_60
#define CYGNUM_ISOINFRA_VERSION_MAJOR 2
#define CYGNUM_ISOINFRA_VERSION_MINOR 0
#define CYGNUM_ISOINFRA_VERSION_RELEASE 60
#define CYGPKG_LIBC v2_0_60
#define CYGPKG_LIBC_v2_0_60
#define CYGNUM_LIBC_VERSION_MAJOR 2
#define CYGNUM_LIBC_VERSION_MINOR 0
#define CYGNUM_LIBC_VERSION_RELEASE 60
#define CYGPKG_LIBC_I18N v2_0_60
#define CYGPKG_LIBC_I18N_v2_0_60
#define CYGNUM_LIBC_I18N_VERSION_MAJOR 2
#define CYGNUM_LIBC_I18N_VERSION_MINOR 0
#define CYGNUM_LIBC_I18N_VERSION_RELEASE 60
#define CYGPKG_LIBC_SETJMP v2_0_60
#define CYGPKG_LIBC_SETJMP_v2_0_60
#define CYGNUM_LIBC_SETJMP_VERSION_MAJOR 2
#define CYGNUM_LIBC_SETJMP_VERSION_MINOR 0
#define CYGNUM_LIBC_SETJMP_VERSION_RELEASE 60
#define CYGPKG_LIBC_STARTUP v2_0_60
#define CYGPKG_LIBC_STARTUP_v2_0_60
#define CYGNUM_LIBC_STARTUP_VERSION_MAJOR 2
#define CYGNUM_LIBC_STARTUP_VERSION_MINOR 0
#define CYGNUM_LIBC_STARTUP_VERSION_RELEASE 60
#define CYGPKG_LIBC_STDIO v2_0_60
#define CYGPKG_LIBC_STDIO_v2_0_60
#define CYGNUM_LIBC_STDIO_VERSION_MAJOR 2
#define CYGNUM_LIBC_STDIO_VERSION_MINOR 0
#define CYGNUM_LIBC_STDIO_VERSION_RELEASE 60
#define CYGPKG_LIBC_STDLIB v2_0_60
#define CYGPKG_LIBC_STDLIB_v2_0_60
#define CYGNUM_LIBC_STDLIB_VERSION_MAJOR 2
#define CYGNUM_LIBC_STDLIB_VERSION_MINOR 0
#define CYGNUM_LIBC_STDLIB_VERSION_RELEASE 60
#define CYGPKG_LIBC_STRING v2_0_60
#define CYGPKG_LIBC_STRING_v2_0_60
#define CYGNUM_LIBC_STRING_VERSION_MAJOR 2
#define CYGNUM_LIBC_STRING_VERSION_MINOR 0
#define CYGNUM_LIBC_STRING_VERSION_RELEASE 60
#define CYGPKG_LIBC_TIME v2_0_60
#define CYGPKG_LIBC_TIME_v2_0_60
#define CYGNUM_LIBC_TIME_VERSION_MAJOR 2
#define CYGNUM_LIBC_TIME_VERSION_MINOR 0
#define CYGNUM_LIBC_TIME_VERSION_RELEASE 60
#define CYGPKG_LIBM v2_0_60
#define CYGPKG_LIBM_v2_0_60
#define CYGNUM_LIBM_VERSION_MAJOR 2
#define CYGNUM_LIBM_VERSION_MINOR 0
#define CYGNUM_LIBM_VERSION_RELEASE 60
#define CYGPKG_ERROR v2_0_60
#define CYGPKG_ERROR_v2_0_60
#define CYGNUM_ERROR_VERSION_MAJOR 2
#define CYGNUM_ERROR_VERSION_MINOR 0
#define CYGNUM_ERROR_VERSION_RELEASE 60
#define CYGPKG_IO_FILEIO v2_0_60
#define CYGPKG_IO_FILEIO_v2_0_60
#define CYGNUM_IO_FILEIO_VERSION_MAJOR 2
#define CYGNUM_IO_FILEIO_VERSION_MINOR 0
#define CYGNUM_IO_FILEIO_VERSION_RELEASE 60
#define CYGPKG_IO_FILEIO_UNICODE_SUPPORT 1
#define CYGPKG_FS_FAT v2_0_60
#define CYGPKG_FS_FAT_v2_0_60
#define CYGNUM_FS_FAT_VERSION_MAJOR 2
#define CYGNUM_FS_FAT_VERSION_MINOR 0
#define CYGNUM_FS_FAT_VERSION_RELEASE 60
#define CYGPKG_IO_DISK v2_0_60
#define CYGPKG_IO_DISK_v2_0_60
#define CYGNUM_IO_DISK_VERSION_MAJOR 2
#define CYGNUM_IO_DISK_VERSION_MINOR 0
#define CYGNUM_IO_DISK_VERSION_RELEASE 60
#define CYGPKG_LINUX_COMPAT v2_0_60
#define CYGPKG_LINUX_COMPAT_v2_0_60
#define CYGNUM_LINUX_COMPAT_VERSION_MAJOR 2
#define CYGNUM_LINUX_COMPAT_VERSION_MINOR 0
#define CYGNUM_LINUX_COMPAT_VERSION_RELEASE 60
#define CYGPKG_LIBSTDCXX v2_0_60
#define CYGPKG_LIBSTDCXX_v2_0_60
#define CYGNUM_LIBSTDCXX_VERSION_MAJOR 2
#define CYGNUM_LIBSTDCXX_VERSION_MINOR 0
#define CYGNUM_LIBSTDCXX_VERSION_RELEASE 60
#define CYGPKG_CPULOAD v2_0_60
#define CYGPKG_CPULOAD_v2_0_60
#define CYGNUM_CPULOAD_VERSION_MAJOR 2
#define CYGNUM_CPULOAD_VERSION_MINOR 0
#define CYGNUM_CPULOAD_VERSION_RELEASE 60
#define CYGPKG_POSIX v2_0_60
#define CYGPKG_POSIX_v2_0_60
#define CYGNUM_POSIX_VERSION_MAJOR 2
#define CYGNUM_POSIX_VERSION_MINOR 0
#define CYGNUM_POSIX_VERSION_RELEASE 60

#endif
