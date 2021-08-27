# ****************************************
# TDAL_FLA makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak



# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_fla


# ****************************************
# This define the default command for gmake
# ****************************************
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(addprefix $(INCLUDE_PREFIX), $(SDK_INCLUDE_PATH)) \
	$(INCLUDE_PREFIX)$(TDAL_SRC_ROOT)/tdal_priv_inc \
    $(INCLUDE_PREFIX)$(TDAL_FLA_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_PERSO_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \


ifeq "$(PRODUCT_USE_FLASH_MTD)" "YES"
DIR_INCLUDE += \
    $(INCLUDE_PREFIX)$(KERNEL_INCLUDE)
endif

# ***************************************************
# List of object files needed to create library
# ***************************************************
ifeq "$(PRODUCT_USE_FLASH_EMULATION)" "YES"
LIBOBJS = 	tdal_fla_emul.$(OBJ_SUFFIX)
else
ifeq "$(PRODUCT_USE_FLASH_MTD)" "YES"
LIBOBJS = 	tdal_fla_mtd.$(OBJ_SUFFIX)
else
LIBOBJS = 	tdal_fla.$(OBJ_SUFFIX)
endif
endif


# ****************************************
# List of dependencies
# ****************************************
tdal_fla_emul.$(OBJ_SUFFIX): $(TDAL_FLA_INCLUDE)/tdal_fla.h tdal_fla_emul.c makefile
tdal_fla_mtd.$(OBJ_SUFFIX): $(TDAL_FLA_INCLUDE)/tdal_fla.h tdal_fla_mtd.c makefile
tdal_fla.$(OBJ_SUFFIX): $(TDAL_FLA_INCLUDE)/tdal_fla.h tdal_fla.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak
