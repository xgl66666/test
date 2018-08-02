
################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#
# Makefile used for configuring toolchain
#
# Required ENV
#       $(ROOT) e.g //utopia/
#
# Supported TOOLCHAIN
#       mips-linux-gnu
#       mips-sde-elf
#       mips2_fp_le
#       aeon
#       mipsisa32-elf (OBSOLETE)
#
# Effected ENV
#       CFG_STDLIB_DIR =
#       CFG_LINT       =
#       CFG_CC_OPTS    +=
#       CFG_LD_OPTS    +=
#       CFG_AS_OPTS    +=
#
################################################################################


# **********************************************
# Tool Chain Configuration
# **********************************************

ifeq ($(TOOLCHAIN),mips-sde-elf)
CFG_STDLIB_DIR = $(ROOT)/tools/mips-sde-elf/EL_mips32r2_msoft-float
CFG_CC_OPTS += -$(ENDIAN) -mips32r2 -gdwarf-2 -G0

ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mhard-float
CFG_LD_OPTS += -$(ENDIAN) -mhard-float
else
CFG_CC_OPTS += -msoft-float
CFG_LD_OPTS += -$(ENDIAN) -msoft-float
endif

CFG_AS_OPTS += -$(ENDIAN) -mips32 -gdwarf2
CFG_LINT    = $(ROOT)/tools/lint/mips-sde-elf_include
CFG_TOOLCHAIN_VER = 4.3.2
endif


#### OBSOLETE ####
ifeq ($(TOOLCHAIN),mipsisa32-elf)
CFG_STDLIB_DIR = $(ROOT)/tools/mipsisa32-elf/EL_mips32r2_msoft-float
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -$(ENDIAN) -mips32 -gdwarf-2 -mhard-float -G0
else
CFG_CC_OPTS += -$(ENDIAN) -mips32 -gdwarf-2 -msoft-float -G0
endif
CFG_LD_OPTS += -$(ENDIAN)
CFG_AS_OPTS += -$(ENDIAN) -mips32 -gdwarf2
CFG_LINT    = $(ROOT)/tools/lint/mipsisa32-elf_include
CFG_TOOLCHAIN_VER =
endif
#### OBSOLETE ####


ifeq ($(TOOLCHAIN),mips2_fp_le)
CFG_STDLIB_DIR =
CFG_CC_OPTS += -mips32 -fPIC -G0
CFG_LD_OPTS += -$(ENDIAN) -fPIC
CFG_AS_OPTS += -$(ENDIAN) -mips32
CFG_LINT    =
ifeq ($(BLT_TYPE),debug)
CFG_CC_OPTS += -gdwarf-2
CFG_AS_OPTS += -gdwarf2
endif
CFG_TOOLCHAIN_VER =
endif

ifeq ($(TOOLCHAIN),arm-eabi)
ifeq ($(PLATFORM), android)
# All flags in project/xxx_android_x.x/Makefile
else
ifeq ($(CHIP),kaiser)
CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN) -gdwarf-2 -march=armv7-a
CFG_LD_OPTS +=  -$(ENDIAN) -Wl,--fix-cortex-a9
CFG_AS_OPTS += -$(ENDIAN) -gdwarf-2 -march=armv7-a
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=soft
endif
CFG_TOOLCHAIN_VER =
endif
endif
endif

ifeq ($(TOOLCHAIN), arm-unknown-linux-gnueabi)
ifeq ($(PLATFORM), android)
# All flags in project/xxx_googletv_x.x/Makefile
else
CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN)
CFG_LD_OPTS +=  -$(ENDIAN)
CFG_AS_OPTS += -$(ENDIAN) -march=armv7-a -mfpu=neon
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfloat-abi=soft
endif
CFG_TOOLCHAIN_VER = 4.5.3
endif
endif

ifeq ($(TOOLCHAIN),arm-none-eabi)
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -gdwarf-2 -fPIC -march=armv7-a -mcpu=cortex-a9
CFG_LD_OPTS += -$(ENDIAN) -fPIC
CFG_AS_OPTS += -$(ENDIAN) -gdwarf2 -march=armv7-a -mcpu=cortex-a9
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=soft
CFG_TOOLCHAIN_VER =
endif
endif

ifeq ($(TOOLCHAIN),arm-none-linux-gnueabi)
ifeq ($(CHIP), $(filter $(CHIP), eagle madison nadal))
CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN) -gdwarf-2 -fPIC -march=armv7-a -mcpu=cortex-a9
CFG_LD_OPTS +=  -$(ENDIAN) -fPIC
CFG_AS_OPTS +=  -$(ENDIAN) -gdwarf2 -march=armv7-a -mcpu=cortex-a9
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfpu=vfpv3 -mfloat-abi=soft
endif
CFG_TOOLCHAIN_VER = 4.5.1

else
ifeq ($(CHIP),kaiser)
CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN) -gdwarf-2 -fPIC -march=armv7-a -mcpu=cortex-a9
CFG_LD_OPTS +=  -$(ENDIAN) -fPIC
CFG_AS_OPTS +=  -$(ENDIAN) -gdwarf2 -march=armv7-a -mcpu=cortex-a9
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfpu=vfpv3 -mfloat-abi=soft
endif
CFG_TOOLCHAIN_VER =
else

ifeq ($(CHIP), $(filter $(CHIP), eiffel nike madison nadal miami kratos))
CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN) -gdwarf-2 -fPIC -march=armv7-a -mcpu=cortex-a9
CFG_LD_OPTS +=  -$(ENDIAN) -fPIC
CFG_AS_OPTS +=  -$(ENDIAN) -gdwarf2 -march=armv7-a -mcpu=cortex-a9
CFG_CC_EXTRA_OPTS += -fno-peephole2 -Wno-unused-but-set-variable
export PATH   :=  /tools/arm/arm-2012.09/bin/:$(PATH)
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfpu=vfpv3 -mfloat-abi=soft
endif
CFG_TOOLCHAIN_VER =
else

CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN) -gdwarf-2 -fPIC -march=armv7-a -mfpu=vfpv3
CFG_LD_OPTS +=  -$(ENDIAN) -fPIC
CFG_AS_OPTS +=  -$(ENDIAN) -gdwarf2 -march=armv7-a -mfpu=vfpv3
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfloat-abi=soft
endif
CFG_TOOLCHAIN_VER = 4.5.1
endif
endif
endif
endif

ifeq ($(TOOLCHAIN), arm-linux)
CFG_STDLIB_DIR =
CFG_CC_OPTS +=  -$(ENDIAN) -gdwarf-2 -fPIC -march=armv7-a -mcpu=cortex-a9
CFG_LD_OPTS +=  -$(ENDIAN) -fPIC
CFG_AS_OPTS +=  -$(ENDIAN) -gdwarf2 -march=armv7-a -mcpu=cortex-a9
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mfpu=neon -mfloat-abi=softfp
else
CFG_CC_OPTS += -mfpu=vfpv3 -mfloat-abi=soft
endif
#CFG_TOOLCHAIN_VER = 4.6.3
endif

ifeq ($(TOOLCHAIN_ALIAS), mips-linux-gnu-uclibc)
TOOLCHAIN  = mips-linux-gnu
ifeq ($(TOOLCHAIN),mips-linux-gnu)
ifeq ($(PLATFORM), android)
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -fPIC -march=mips32r2 -mtune=mips32r2 -mips32r2 -G0 -muclibc
CFG_LD_OPTS += -$(ENDIAN) -fPIC -muclibc
CFG_AS_OPTS += -$(ENDIAN) -march=mips32r2 -mtune=mips32r2 -mips32r2
#CFG_LINT    =
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mhard-float
else
CFG_CC_OPTS += -msoft-float
endif
CFG_TOOLCHAIN_VER = 4.4.3

else # PLATFORM != android
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -gdwarf-2 -fPIC -G0 -muclibc
CFG_LD_OPTS += -$(ENDIAN) -fPIC -muclibc
CFG_AS_OPTS += -$(ENDIAN) -gdwarf2
CFG_LINT    = $(ROOT)/tools/lint/mips-linux-gnu_include
ifeq ($(MIPS16_ENABLE), 1)
CFG_CC_OPTS += -mips16 -D__MIPS16E__
CFG_AS_OPTS += -mips16 -D__MIPS16E__
else
CFG_CC_OPTS += -mips32
CFG_AS_OPTS += -mips32
endif
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mhard-float
else
CFG_CC_OPTS += -msoft-float
endif
CFG_TOOLCHAIN_VER =
endif
endif

else # TOOLCHAIN_ALIAS != mips-linux-gnu-uclibc
ifeq ($(TOOLCHAIN),mips-linux-gnu)
ifeq ($(PLATFORM), android)
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -fPIC -march=mips32r2 -mtune=mips32r2 -mips32r2 -G0
CFG_LD_OPTS += -$(ENDIAN) -fPIC
CFG_AS_OPTS += -$(ENDIAN) -march=mips32r2 -mtune=mips32r2 -mips32r2
#CFG_LINT    =
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mhard-float
else
CFG_CC_OPTS += -msoft-float
endif
CFG_TOOLCHAIN_VER = 4.4.3

else # PLATFORM != android
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -gdwarf-2 -fPIC -G0
CFG_LD_OPTS += -$(ENDIAN) -fPIC
CFG_AS_OPTS += -$(ENDIAN) -gdwarf2
CFG_LINT    = $(ROOT)/tools/lint/mips-linux-gnu_include
ifeq ($(MIPS16_ENABLE), 1)
CFG_CC_OPTS += -mips16 -D__MIPS16E__
CFG_AS_OPTS += -mips16 -D__MIPS16E__
else
CFG_CC_OPTS += -mips32
CFG_AS_OPTS += -mips32
endif
ifeq ($(FPU_ENABLE), 1)
CFG_CC_OPTS += -mhard-float
else
CFG_CC_OPTS += -msoft-float
endif
CFG_TOOLCHAIN_VER =
endif
endif
endif

ifeq ($(TOOLCHAIN),aeon-linux)
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -march=aeonR2 -fPIC -G0
CFG_LD_OPTS +=
CFG_AS_OPTS += -$(ENDIAN)
CFG_LINT    = $(ROOT)/tools/lint/aeon_include
ifeq ($(COPRO_TYPE), R2)
CFG_CC_OPTS += -march=aeonR2
CFG_TOOLCHAIN_VER = 4.4.3
PATH:=/tools/aeon-elf443/aeon-elf-linux/bin:$(PATH)
export PATH
endif
ifeq ($(COPRO_TYPE), AEON)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
ifeq ($(CHIP), maria10)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
ifeq ($(CHIP), macaw12)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
endif

ifeq ($(TOOLCHAIN),aeon)
CFG_STDLIB_DIR =
CFG_CC_OPTS += -$(ENDIAN) -mredzone-size=4 -mhard-mul -mhard-div -G0
CFG_LD_OPTS +=
CFG_AS_OPTS += -$(ENDIAN)
CFG_LINT    = $(ROOT)/tools/lint/aeon_include
ifeq ($(COPRO_TYPE), R2)
CFG_CC_OPTS += -march=aeonR2
CFG_TOOLCHAIN_VER = 4.4.3
PATH:=/tools/aeon-elf443/aeon-elf-linux/bin:$(PATH)
export PATH
endif
ifeq ($(COPRO_TYPE), AEON)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
ifeq ($(CHIP), maria10)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
ifeq ($(CHIP), macaw12)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
ifeq ($(CHIP), eden)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
ifeq ($(CHIP), euler)
CFG_TOOLCHAIN_VER = 4.1.2
PATH:=/tools/aeon/bin:$(PATH)
export PATH
endif
endif

ifeq ($(GCOV), 1)
CFG_CC_OPTS += -fprofile-arcs -ftest-coverage
endif

ifeq ($(PARA), 1)
CC          = cpptestscan --cpptestscanProjectName=unittest $(TOOLCHAIN)-gcc
AS          = cpptestscan --cpptestscanProjectName=unittest $(TOOLCHAIN)-as
LD          = cpptestscan --cpptestscanProjectName=unittest $(TOOLCHAIN)-ld
AR          = cpptestscan --cpptestscanProjectName=unittest $(TOOLCHAIN)-ar
else ifeq ($(PARA), 3)
CC          = cpptestscan --cpptestscanProjectName=unittest gcc
AS          = cpptestscan --cpptestscanProjectName=unittest as
LD          = cpptestscan --cpptestscanProjectName=unittest ld
AR          = cpptestscan --cpptestscanProjectName=unittest ar
else

ifeq ($(CFG_TOOLCHAIN_VER),)
CC          = $(TOOLCHAIN)-gcc
AS          = $(TOOLCHAIN)-as
else

ifeq ($(PLATFORM),Sourcery.CodeBench.Lite.2012.09-64)
CC          = $(TOOLCHAIN)-gcc
AS          = $(TOOLCHAIN)-as
else
CC          = $(TOOLCHAIN)-gcc -V $(CFG_TOOLCHAIN_VER)
AS          = $(TOOLCHAIN)-as
endif

endif

LD          = $(TOOLCHAIN)-ld
AR          = $(TOOLCHAIN)-ar
endif
OBJCOPY     = $(TOOLCHAIN)-objcopy
OBJDUMP     = $(TOOLCHAIN)-objdump
SIZE        = $(TOOLCHAIN)-size
NM          = $(TOOLCHAIN)-nm
STRIP       = $(TOOLCHAIN)-strip

ifeq ($(DEBUG_LOG),disable)
    CC_OPTS    += -D 'printf(x,...)= null'
endif