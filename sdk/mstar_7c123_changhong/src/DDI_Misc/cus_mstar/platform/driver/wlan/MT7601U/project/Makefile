
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
# Makefile used for building application.
#
#
################################################################################

export
unexport VPATH
unexport SRCDIR_CHIP SRCDIR_LIB SRCDIR_SYS
unexport OBJ_C_CHIP0 OBJ_C_ASM0 OBJ_C_LIB0 OBJ_C_SYS0
unexport LIB_INC


# **********************************************
# Environment
# **********************************************
PROJ        = $(CURDIR)
ROOT        = $(shell readlink -f $(PROJ)/..)
ROOTLIB     = $(ROOT)
DDIDRV      = $(ROOT)/source
DDILIB      = $(ROOT)/lib


# **********************************************
# Import configure file
# Required:
#	$(ROOT), $(ROOTLIB)
# **********************************************
-include config

# **********************************************
# Customer specific
# **********************************************

# **********************************************
# Project specific
# **********************************************
# Base Operating System
#ifeq ($(TOOLCHAIN),mips-sde-elf-)
OS_BUILT    =
#else
#ifeq ($(FPU_ENABLE),1)
#OS_BUILT    = ecos_kratos_34kf_fileio_posix_fat_ntfs_mipsisa32_c++
#else
#OS_BUILT    = ecos_kratos_34kc_fileio_posix_fat_ntfs_mipsisa32_c++
#endif
#endif


# **********************************************
# Directories
# **********************************************
# Source files of specific chip in library

# Source files of library



# FW binary files of library

BIN_PATH    = $(PROJ)/bin
OBJ_PATH    = $(PROJ)/obj
BSPLIB_PATH = $(PROJ)/bsp/lib
BSPINC_PATH = $(PROJ)/bsp/include
BSPBIN_PATH = $(PROJ)/bsp/bin
BSPMSC_PATH = $(PROJ)/bsp/misc


# Search source file path
VPATH       = $(SRCDIR_CHIP) $(SRCDIR_SYS) $(SRCDIR_FS) $(SRCDIR_ASM) $(SRCDIR_LIB)

#include $(ROOTLIB)/$(CHIP)_ddi.mk
#include $(ROOTLIB)/ddi.mk
DRV_DIR = MT7601U

# **********************************************
# Files to be compiled
# **********************************************


# **********************************************
# Libraries
# **********************************************

DDI_PATH = $(ROOT)/lib


# Standard Libraries Path


# Middleware Libraries Path


# Libraries
DDI_LIB =$(DDI_PATH)/lib$(OS_TYPE).$(LIB_EXT)

# Include files of output library
LIB_INC     =





# FW Binary files of output library

# Misc files


# **********************************************
# Compiler and linker options
# **********************************************
ifeq ($(OS_TYPE),ecos)
#OS_INC_DIR  = $(ROOT)/../../GreenLion/platform/$(OS_TYPE)/$(OS_BUILT)/include
endif
#USBDRV_INC_DIR = /x/PERFORCE/DAILEO/Hummingbird_Branch/Hummingbird_Kratos_HBOTT/lib_hardfloat/kratos/ecos/drv/include/
#ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR) $(MXLIB) $(USBDRV_INC_DIR)
ALL_INC_DIR =  $(OS_INC) $(USBDRV_INC) $(BSP_INC)

CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir))

CC_DEFS     = $(CFG_CC_DEFS) -D'OS_ECOS' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef
CC_OPTS    += -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions
CC_OPTS    += -ffunction-sections
CC_OPTS    += -fdata-sections
CC_OPTS    += -c -G0

# Check toolchain version and add define
ifeq ($(TOOLCHAIN),mipsisa32-elf)
TOOLCHAIN_VER=$(subst .,_,$(shell $(CC) -dumpversion))
CC_OPTS    += -D'MIPSISA32_GCC_VER_$(TOOLCHAIN_VER)'
endif

# AS options:
AS_OPTS     = $(CFG_AS_OPTS) -G0 -Os


# LD options:
LD_SCRIPT   = $(PROJ)/ecos.ld
LD_LIBS     = -L$(DDI_PATH) -L$(OS_LIB_DIR) -L$(CFG_STDLIB_DIR) -T$(LD_SCRIPT)
LD_OPTS     = $(CFG_LD_OPTS) -nostdlib -Map $(IMAGE_MAP)


ifeq ($(BLT_TYPE),debug)
CC_OPTS    += -O0
CC_DEFS    += -D'MS_DEBUG'
else
CC_OPTS    += -Os
CC_DEFS    += -D'MS_OPTIMIZE'
endif


# **********************************************
# C Helper Utilities
# **********************************************
# Generate relative path for Doxygen

# Generate options list for Lint
ifeq ($(shell uname -o),Cygwin)                                     #TODO: revise cygwin related
LINT_LOG    = $(shell cygpath -m $(CURDIR)/lint_test.log)
LINT_FILES  = $(shell cygpath -m $(CURDIR)/lint_files.lnt)
LINT_INC    = $(foreach dir,$(shell cygpath -m $(ALL_INC_DIR)),-i"$(dir)") -i"$(shell cygpath -m $(CFG_LINT))"
else
LINT_LOG    = $(CURDIR)/lint_test.log
LINT_FILES  = $(CURDIR)/lint_files.lnt
LINT_INC    = $(foreach dir,$(ALL_INC_DIR),-i"$(dir)") -i"$(CFG_LINT)"
endif


# **********************************************
# Rules
# **********************************************
.PHONY : all setup clean bsp depend docgen $(MXLIB)


# Project Build
#all :  setup $(MXLIB) syslib
all : $(DRV_DIR)

# Library Package
bsp :
	@rm -f -r                               $(BSPLIB_PATH)
	@rm -f -r                               $(BSPINC_PATH)
	@rm -f -r                               $(BSPBIN_PATH)
	@rm -f -r                               $(BSPMSC_PATH)
	-@mkdir -p                              $(BSPLIB_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPINC_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPBIN_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPMSC_PATH) 2> /dev/null;
	@cp         $(DDI_PATH)/*.$(LIB_EXT)    $(BSPLIB_PATH)
	@cp         $(LIB_INC)                  $(BSPINC_PATH);
#ifeq ($(FW_TYPE),external)
	@cp         $(LIB_FW_BIN)               $(BSPBIN_PATH);
#endif
	@cp         $(LIB_MISC)                 $(BSPMSC_PATH);
ifeq ($(FPU_ENABLE), 1)
	@echo "Add Dummy Hardfloat Section for check in ...";
	for i in `ls $(BSPLIB_PATH)/*.a`; do $(AR) -r $$i hardfloat.o; done
endif
	@echo "Make $(CHIP) $(PROJNAME) board support package...";
	@tar zcvf $(CHIP)_$(OS_TYPE).$(MCU_TYPE)_$(BLT_TYPE).tgz bsp

# Library Document Auto Generation


# Project Lint Report
lint :
	@echo "Generate $(CHIP) $(PROJNAME) lint report $(LINT_LOG)...";
	@echo `grep 'Error' $(LINT_LOG) | wc -l` LINT Errors


# Project Library Build
SYS_MSG :
	@echo [Makefile][$(notdir $(DDI_LIB))]
	@echo OS_BUILT = $(OS_BUILT)

SYS_LIB : $(OBJ_C_CHIP) $(OBJ_C_SYS) $(OBJ_C_ASM) $(OBJ_C_LIB)
#Add section :MsOS
	@$(OBJCOPY) --add-section .mmodule_version=$(ROOTLIB)/msos/version_info $(dir $<)MsOS_$(OS_TYPE).o
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_LIB) $^;
else
	@$(AR) -cru $(DDI_LIB) $^;
endif
endif
	@date;




# Project Source Build
$(OBJ_C_CHIP) $(OBJ_C_ASM) $(OBJ_C_SYS): $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@echo [CC] $(notdir $@)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -o $@ $<;
else
	@echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
	@echo $< >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
	@echo $< >>$(LINT_FILES)
endif
endif
#	@$(AR) -cru $(DDI_LIB) $@;

$(OBJ_C_FS): $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -o $@ $<;
else
	echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
	echo $< >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
	echo $< >>$(LINT_FILES)
endif
endif
#ifeq ($(OS_TYPE),$(notdir $(<D)))
	@$(OBJCOPY) --add-section .mmodule_version=$(subst /$(OS_TYPE),,$(dir $<D))version_info $@

$(OBJ_C_LIB): $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -o $@ $<;
else
	echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
	echo $< >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
	echo $< >>$(LINT_FILES)
endif
endif
	@$(OBJCOPY) --add-section .mmodule_version=$(subst /ecos_nos,,$(dir $<D))version_info $@


# Single Driver Build
$(DRV_DIR): setup
	@echo [ddi.mk] $@
	@$(MAKE) -s -C $(DDIDRV)/$@ clean lib
ifeq ($(FPU_ENABLE),1)
	@$(AR) -r $(DDI_PATH)/* hardfloat.o
endif

# Project Setup
setup :
	-@mkdir -p $(DDI_PATH) 2> /dev/null;
	@echo "Make $(CHIP) $(PROJNAME) library...";
	@date;
ifeq ($(MAKE_TYPE),lint)
	@rm -rf $(LINT_FILES)
	@rm -rf $(LINT_LOG)
endif


# Project Clean
clean :
	@$(MAKE) -s -C $(DDIDRV)/$(DRV_DIR) clean
	@rm -rf $(OBJ_PATH)
	@rm -rf $(BIN_PATH)
#	@rm -rf $(DDI_PATH)
	@rm -rf $(PKG_DIR)/include/$(CHIP);
	@rm -rf $(PKG_DIR)/os;
	@rm -rf lint_files
	@rm -rf dxy_files
#	@echo "Clean $(CHIP) $(PROJNAME) library..."


# Project Dependence
depend :
	$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -M $(SRC_C_CHIP) $(SRC_C_ASM) $(SRC_C_LIB) > $(BIN_PATH)/depend.mk


# Project Dependence Full version
dep :
	$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -M $(SRC_C_CHIP) $(SRC_C_ASM) $(SRC_C_LIB) > $(BIN_PATH)/depend.mk
	cat $(BIN_PATH)/depend.mk | sed -e 's/\(.*\)\.o/\$$\(OBJ_PATH\)\/\1.o/g' > depend.mk


-include depend.mk

