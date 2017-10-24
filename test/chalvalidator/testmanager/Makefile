# ****************************************
# TestManager makefile
# ****************************************
include $(BUILD_MAKE)/environnement.mak

# ****************************************
# Define module name
# ****************************************
MODULE  = testmanager

# ****************************************
# This define the default command for gmake
# ****************************************
all: $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE = \
    $(INCLUDE_PREFIX)$(TEST_MANAGER_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TKEL_INCLUDE) 

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBSOURCES = $(wildcard src/*.c)

LIBOBJS = ${LIBSOURCES:%.c=%.$(OBJ_SUFFIX)}

# ****************************************
# List of dependencies
# ****************************************
TestManager.$(OBJ_SUFFIX): $(TEST_MANAGER_ROOT)/src/TestManager.c makefile
TestRunner.$(OBJ_SUFFIX): $(TEST_MANAGER_ROOT)/src/TestRunner.c makefile

# ****************************************
# Always include crules.mak
# ****************************************
include $(BUILD_MAKE)/crules.mak

clean_all:
	@ echo Cleaning testmanager lib
	$(MAKE) -ikC src clean

