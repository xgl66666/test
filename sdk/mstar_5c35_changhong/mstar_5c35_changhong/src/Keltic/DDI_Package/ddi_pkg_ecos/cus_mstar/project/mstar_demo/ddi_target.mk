
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
# The default target (all) builds application in three formats :
#   *.rec : Image in S-record format.
#   *.bin : Image in binary format.
#   *.elf : Image in ELF format.
#   *.map : Linker generated map file.
#   *.dis : Disassembly of image.
#   *.sym : Symbols.
#
# Other targets are :
#   clean :    Deletes all files generated by makefile.
#
#
################################################################################


# **********************************************
# Import configure file
# **********************************************
#-include $(PRJ_DIR)/config
#-include $(PRJ_DIR)/cus_config/configure_define

include $(PRJ_DIR)/ddi_configuration

ifeq ($(PARA),1)
CFG_CC_OPTS =
CFG_LD_OPTS =
CFG_AS_OPTS =
endif

DDI_MAIN_DEMO_DIR = $(PRJ_DIR)/../../../../../../DDI_Main/demo
include  $(DDI_MAIN_DEMO_DIR)/Makefile.demo

DDI_MISC_DIR = $(PRJ_DIR)/../../../../../../DDI_Misc
include  $(DDI_MISC_DIR)/cus_mstar/project/mstar_demo/Makefile
include  $(DDI_MISC_DIR)/cus_mstar/project/mstar_demo/Makefile.feature
# **********************************************
# Project specific
# **********************************************

#OS_BUILT    = ecos_keltic_34kf_fileio_posix_fat_c++_mipsisa32
ifeq ($(CFG_ECOS_VERSION),Normal)
OS_BUILT    = ecos_keltic_34kf_fileio_posix_fat_ntfs_c++_mipsisa32
else
OS_BUILT    = ecos_keltic_34kf_fileio_posix_fat_c++_mipsisa32
endif
CC_APPOPTS  =


# **********************************************
# Directories
# **********************************************
ROOT        = .
TRUNK       = ../../..
BSP_PRJ     = $(TRUNK)
CUS_TOP     = $(PRJ_DIR)/../../../..

# for QMAP
QMAP_SEL    = keltic_mstar

# NDS Verifier
SRCDIR     = \
            $(TRUNK)/cus_mstar/platform/driver/pad             \
            $(TRUNK)/cus_mstar/platform/driver/pwsv            \
            $(TRUNK)/cus_mstar/platform/driver/pq              \
            $(TRUNK)/cus_mstar/platform/driver/pq/hal/$(QMAP_SEL)  \
            $(TRUNK)/cus_mstar/api/vdec                        \
            $(TRUNK)/cus_mstar/api/xc                          \
            $(TRUNK)/cus_mstar/api/sc                          \
            $(TRUNK)/cus_mstar/api/gpio

#include DDI_Main source files
SRCDIR     += $(DDI_MAIN_DEMO_SRCDIR)

#include DDI_MISC files
SRCDIR     += $(DDI_MISC_SRCDIR)
SRCDIR     += $(TRUNK)/bsp/misc

ifeq ($(PARA),0)
SRCDIR     += $(PRJ_DIR)\
              $(TRUNK)/cus_mstar/api/xml/expat-2.0.1
endif

INCDIR     = \
            $(PRJ_DIR)                                              \
            $(PRJ_DIR)/include                                      \
            $(PRJ_DIR)/include/chip                                 \
            $(TRUNK)/cus_mstar/platform/driver/pad             \
            $(TRUNK)/cus_mstar/platform/driver/pwsv            \
            $(TRUNK)/cus_mstar/platform/driver/pq/include      \
            $(TRUNK)/cus_mstar/platform/driver/pq/hal/$(QMAP_SEL)/include  \
            $(TRUNK)/cus_mstar/api/vdec                        \
            $(TRUNK)/cus_mstar/api/xc                          \
            $(TRUNK)/cus_mstar/api/sc                          \
            $(TRUNK)/cus_mstar/api/gpio                        \
            $(TRUNK)/cus_mstar/api/xml/expat-2.0.1             \
            $(CUS_TOP)/cus_info/                               \

#include DDI_Main include files
INCDIR     += $(DDI_MAIN_DEMO_INCDIR)

#include DDI_MISC files
INCDIR     += $(DDI_MISC_INCDIR)

ifeq ($(DDI_DEMO_MM), enable)
INCDIR     += $(TRUNK)/cus_mstar/api/mm
endif
ifeq ($(DDI_DEMO_PVR),enable)
INCDIR     += $(TRUNK)/cus_mstar/api/pvr
endif
ifeq ($(CFG_FE_AUTO_TEST), enable)
SRCDIR     += \
            $(DDI_MISC_DIR)/cus_mstar/platform/driver/demod/autotest
INCDIR     += \
            $(DDI_MISC_DIR)/cus_mstar/platform/driver/demod/autotest
endif

BINDIR      = bin
BIN_PATH    = $(ROOT)/$(BINDIR)
OBJDIR      = obj
OBJ_PATH    = $(ROOT)/$(OBJDIR)

# Search source file path
VPATH       = $(SRCDIR)                                       \
              $(DDI_MISC_DIR)/cus_mstar/platform/driver/scart        \


# **********************************************
# Image file names
# **********************************************
IMAGE_BIN   = $(BIN_PATH)/$(IMAGENAME).bin
IMAGE_REC   = $(BIN_PATH)/$(IMAGENAME).rec
IMAGE_ELF   = $(BIN_PATH)/$(IMAGENAME).elf
IMAGE_MAP   = $(BIN_PATH)/$(IMAGENAME).map
IMAGE_DIS   = $(BIN_PATH)/$(IMAGENAME).dis
IMAGE_SIZ   = $(BIN_PATH)/$(IMAGENAME).siz
IMAGE_SYM   = $(BIN_PATH)/$(IMAGENAME).sym


# **********************************************
# Files to be compiled for Parasoft
# **********************************************
ifeq ($(PARA),1)
SRCDIR = $(DDI_MAIN_DEMO_SRCDIR)
endif


# **********************************************
# Files to be compiled
# **********************************************
INC_C_LIB   = $(foreach dir, $(INCDIR_LIB),   $(wildcard $(dir)/*.h))
INC_C       = $(foreach dir, $(INCDIR),       $(wildcard $(dir)/*.h))
INC         = $(INC_C_LIB) $(INC_C)

ifeq ($(PARA),0)
ifeq ('',$(findstring 'MST183', $(CFG_BOARD)))
SRC_C      += $(DDI_MISC_DIR)/cus_mstar/platform/driver/scart/drvScart_STV6419.c
else
SRC_C      += $(DDI_MISC_DIR)/cus_mstar/platform/driver/scart/drvScart_BD_MST.c
endif
endif
SRC_C      += $(foreach dir, $(SRCDIR),       $(wildcard $(dir)/*.c))
SRC         = $(SRC_C)

OBJ_C_0     = $(notdir $(patsubst %.c, %.o,   $(SRC_C)))
OBJ_C       = $(foreach file, $(OBJ_C_0),     $(OBJ_PATH)/$(file))
OBJ         = $(OBJ_C)


# **********************************************
# Libraries
# **********************************************

# Standard Libraries Path
BSP_DIR     = $(BSP_PRJ)/bsp

# Middleware Libraries Path

# Libraries
#BSP_LIBS    = $(BSP_DIR)/lib/libdrvNDS.a # for testing TODO: fix it
BSP_LIBS   += $(wildcard $(BSP_DIR)/lib/libapi*.a)      # api lib
BSP_LIBS   += $(wildcard $(BSP_DIR)/remove/libapi*.a)   # api lib
BSP_LIBS   += $(wildcard $(BSP_DIR)/lib/libdrv*.a)	# drv lib
BSP_LIBS   += $(BSP_DIR)/lib/lib$(OS_TYPE).a		# sys lib
BSP_LIBS   += $(BSP_DIR)/lib/libMsFS.a                  # MsFs lib
BSP_LIBS   += $(wildcard $(TRUNK)/cus_mstar/api/xc/*.a)	#Scaler/VE lib
BSP_LIBS   += $(wildcard $(BSP_DIR)/lib/libcashmere.a)	# DTMB lib

ifeq ($(DDI_DEMO_MM),enable)
BSP_LIBS   += $(wildcard $(TRUNK)/cus_mstar/api/mm/*.a)      # mm lib
MW_LIBS   += $(CFG_STDLIB_DIR)/libstdc++.a $(CFG_STDLIB_DIR)/libsupc++.a
endif
ifeq ($(DDI_DEMO_PVR),enable)
BSP_LIBS   += $(wildcard $(TRUNK)/cus_mstar/api/pvr/*.a)      # pvr lib
MW_LIBS   += $(CFG_STDLIB_DIR)/libstdc++.a $(CFG_STDLIB_DIR)/libsupc++.a
endif
ifeq ($(CFG_AUDIO_LIB_TYPE),Audio_Normal)
NEW_AUDIO_LIB = $(BSP_DIR)/lib/libapiAUDIO_lite.a
NEW_AUDSP_LIB = $(BSP_DIR)/lib/libdrvAUDSP_lite.a
BSP_LIBS      := $(filter-out $(NEW_AUDIO_LIB) $(NEW_AUDSP_LIB), $(BSP_LIBS))
endif
ifeq ($(CFG_AUDIO_LIB_TYPE),Audio_Lite)
OLD_AUDIO_LIB = $(BSP_DIR)/lib/libapiAUDIO.a
OLD_AUDSP_LIB = $(BSP_DIR)/lib/libdrvAUDSP.a
NEW_AUDIO_LIB = $(BSP_DIR)/lib/libapiAUDIO_lite.a
NEW_AUDSP_LIB = $(BSP_DIR)/lib/libdrvAUDSP_lite.a
BSP_LIBS      := $(filter-out $(OLD_AUDIO_LIB) $(OLD_AUDSP_LIB), $(BSP_LIBS))
BSP_LIBS      += $(NEW_AUDIO_LIB) $(NEW_AUDSP_LIB)
endif

# Middlware Libraries
# **********************************************
# Compiler and linker options
# **********************************************
ifeq ($(OS_TYPE),ecos)
OS_INC_DIR  = $(TRUNK)/$(OS_TYPE)/$(OS_BUILT)/include
OS_LIB_DIR  = $(TRUNK)/$(OS_TYPE)/$(OS_BUILT)/lib
OS_EX_BUILD_OBJ = $(wildcard $(TRUNK)/$(OS_TYPE)/$(OS_BUILT)/libntfs/*.o)
endif
ifeq ($(OS_TYPE),ucos)
OS_INC_DIR  = $(TRUNK)/$(OS_TYPE)/include
OS_LIB_DIR  = $(TRUNK)/$(OS_TYPE)/lib
endif

ALL_INC_DIR = $(OS_INC_DIR) $(INCDIR) $(BSP_DIR)/include $(BSP_DIR)/lib/fw

CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir))

CC_DEFS     = $(CFG_CC_DEFS)
# GCC options:
ifeq ($(PARA),0)
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef
CC_OPTS    += -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions -fno-builtin
#CC_OPTS    += -ffunction-sections
#CC_OPTS    += -fdata-sections
CC_OPTS    += -c -G0 -Werror

ifeq ($(BLT_TYPE),debug)
CC_OPTS    += -O0
CC_DEFS    += -D'MS_DEBUG'
else
CC_OPTS    += -Os
CC_DEFS    += -D'MS_OPTIMIZE'
endif
else
CC_OPTS     = -c
endif

# AS options:
ifeq ($(PARA),0)
AS_OPTS     = -mips32 -$(ENDIAN) -G0 -O2 -gdwarf2
endif


# LD options:
ifeq ($(TOOLCHAIN),mips-sde-elf)
LD_SCRIPT   = ecos_mipssde.ld
endif
ifeq ($(TOOLCHAIN),mipsisa32-elf)
LD_SCRIPT   = ecos_mipsisa32.ld
endif
LD_LIBS     = -L$(OS_LIB_DIR) -L$(CFG_STDLIB_DIR) -L$(BSP_DIR)/lib -T$(LD_SCRIPT) # libtarget.a, libgcc.a, UART+EMAC
#LD_OPTS     = $(CFG_LD_OPTS) -nostdlib -$(ENDIAN) --gc-sections -static -Map $(IMAGE_MAP)
LD_OPTS     = $(CFG_LD_OPTS) -$(ENDIAN) --gc-sections -static -Map $(IMAGE_MAP)

# **********************************************
# Rules
# **********************************************
.PHONY : all clean library package depend $(IMAGE_ELF)

# Project Build
ifeq ($(PARA),0)
all : setup makeImg
else
all : setup $(OBJ_C)
	@echo '  [Generate BDF finish ... ]'

endif

makeElf : $(IMAGE_ELF)

# makeImg : $(IMAGE_ELF) $(IMAGE_BIN)
makeImg : $(IMAGE_ELF) $(IMAGE_BIN)

# Note: It's slow to produce .dis file w/o -gdwarf-2 set or original OS source code
makeImgInfo : $(IMAGE_ELF)
	$(OBJDUMP) -S $(IMAGE_ELF) > $(IMAGE_DIS);
	$(SIZE) $(IMAGE_ELF) > $(IMAGE_SIZ);
	$(NM) -a -S $(IMAGE_ELF) > $(IMAGE_SYM);

# Utility makefile for standalone library
makeFont : fontsetup fontlib

# Project Image Build

# Project Image Build
$(IMAGE_BIN) : $(IMAGE_ELF)
	$(OBJCOPY) -O binary $(IMAGE_ELF) $(IMAGE_BIN);

ifeq ($(DDI_DEMO_PVR),enable)
$(IMAGE_ELF) : $(OBJ_C)
	@echo '  [LD] $@'
	@$(LD) -G0 $(LD_OPTS) -o $(IMAGE_ELF) $(OBJ_C) --start-group $(BSP_LIBS) $(OS_EX_BUILD_OBJ) --end-group $(MW_LIBS) $(LD_LIBS);
else ifeq ($(DDI_DEMO_MM),enable)
$(IMAGE_ELF) : $(OBJ_C)
	@echo '  [LD] $@'
	@$(LD) -G0 $(LD_OPTS) -o $(IMAGE_ELF) $(OBJ_C) --start-group $(BSP_LIBS) $(OS_EX_BUILD_OBJ) --end-group $(MW_LIBS) $(LD_LIBS);
else
$(IMAGE_ELF) : $(OBJ_C)
	@echo '  [LD] $@'
	@$(LD) $(LD_OPTS) -o $(IMAGE_ELF) $(OBJ_C) --start-group $(BSP_LIBS) $(OS_EX_BUILD_OBJ) --end-group $(MW_LIBS) $(LD_LIBS);
endif

# Project Source Build
$(OBJ_C) : $(OBJ_PATH)/%.o : %.c
	@echo '  [CC] $@'
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -o $@ $<;

# Project Setup
setup :
	-@mkdir -p $(OBJDIR) 2> /dev/null;
	-@mkdir -p $(BINDIR) 2> /dev/null;
	@echo "Make $(CHIP) $(PROJNAME) project...";
	@k=0; for i in $(BSP_LIBS); do j=`$(AR) t $$i | grep -ic 'hardfloat'`; if(($$j==0)); then k=1;echo $$i" use soft-float! Please use hard-float instead."; fi ; done ; if((k>0)); then false ; fi

# Project Clean
clean :
	@echo "Clean $(CHIP) $(PROJNAME) project..."
	@rm -f -r $(OBJ_PATH)
	@rm -f -r $(BIN_PATH)
	@rm -f -r DDI_MAIN_KELTIC_ECOS.bdf
	@echo "Clean $(CHIP) $(PROJNAME) ..."

# Project Dependence
depend :
	$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -M $(SRC) > $(BIN_PATH)/depend.mk


# Project Dependence Full version
dep :
	$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -M $(SRC) > $(BIN_PATH)/depend.mk
	cat $(BIN_PATH)/depend.mk | sed -e 's/\(.*\)\.o/\$$\(OBJ_PATH\)\/\1.o/g' > depend.mk


-include depend.mk
