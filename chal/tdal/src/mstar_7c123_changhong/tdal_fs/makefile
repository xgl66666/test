# ****************************************
# TDAL_FS makefile
# ****************************************

include $(TDAL_MAKE)/environnement.mak
include $(SDK_ROOT)/drivers.mak


# ****************************************
# Define module name
# ****************************************
MODULE  = tdal_fs


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
    $(INCLUDE_PREFIX)$(TDAL_FS_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = tdal_fs.$(OBJ_SUFFIX) \
	tdal_fs_lvm.$(OBJ_SUFFIX) \
	tdal_fs_vfs.$(OBJ_SUFFIX)


ifeq "$(PRODUCT_USE_USB)" "YES"
#LIBOBJS += tdal_fs_usb.$(OBJ_SUFFIX)
endif

# ****************************************
# List of dependencies
# ****************************************
tdal_fs.$(OBJ_SUFFIX): $(TDAL_FS_INCLUDE)/tdal_fs.h tdal_fs.c makefile
tdal_fs_vfs.$(OBJ_SUFFIX): $(TDAL_FS_INCLUDE)/tdal_fs.h tdal_fs_vfs.c makefile
tdal_fs_lvm.$(OBJ_SUFFIX): $(TDAL_FS_INCLUDE)/tdal_fs.h tdal_fs_lvm.c tdal_fs_lvm_p.h makefile

ifeq "$(PRODUCT_USE_USB)" "YES"
tdal_fs_usb.$(OBJ_SUFFIX): $(TDAL_FS_INCLUDE)/tdal_fs.h tdal_fs_usb.c makefile
endif

# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

