###################################################
#	Current compilation environnement system
###################################################

ifeq ($(OS),Windows_NT)
    PRODUCT_ENV ?= windows
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
        PRODUCT_ENV ?= unix
    endif
    ifeq ($(UNAME),Darwin)
        PRODUCT_ENV ?= android
    endif
endif

UNIX_SLASH = /
DOS_SLASH = $(subst /,\,$(UNIX_SLASH))

ifeq ($(PRODUCT_ENV),windows)
        SLASH = $(DOS_SLASH)
        GOOD_SLASH = $(DOS_SLASH)
        BAD_SLASH = $(UNIX_SLASH)
endif

ifeq ($(PRODUCT_ENV),unix)
        SLASH = $(UNIX_SLASH)
        GOOD_SLASH = $(UNIX_SLASH)
        BAD_SLASH = $(DOS_SLASH)
endif

# *************************
#     DEFINE TOOLS
# *************************

INCLUDE_PREFIX = -I
LIBRARY_PREFIX = -L

ifeq ($(PRODUCT_ENV),windows)
$(info NO TOOLCHAIN_PATH defined!)
endif

ifeq ($(PRODUCT_ENV),unix)
CC = mipsisa32-elf-gcc
LD = mipsisa32-elf-ld
CC_PLUS = mipsisa32-elf-g++
AS = mipsisa32-elf-as
AR = mipsisa32-elf-ar
STRIP = mipsisa32-elf-strip
OBJ_COPY = mipsisa32-elf-objcopy
TOOLCHAIN_PATH:=/tools/mstar/gnutools/mipsisa32-elf-3.4.4/bin/
export PATH:=$(TOOLCHAIN_PATH):$(PATH)
endif

# ***********************************
#     DEFINE C COMPILING OPTIONS
# ***********************************
CC_OPTIONS = -EL -mips32 -mhard-float -Wpointer-arith -Winline -G0 -mlong-calls \
             -Wundef -fno-strict-aliasing -fno-optimize-sibling-calls -DMS_C_STDLIB -fno-builtin -c

CXX_FLAGS=   -mips32 -mhard-float -Wpointer-arith -Winline -mlong-calls -EL -G0 \
              -Wno-undef -fno-strict-aliasing -fno-optimize-sibling-calls -fno-builtin

# *****************************
#    DEFINE DEBUG options
# *****************************
CC_OPTIONS_DEBUG = -g -O0
CC_OPTIONS_RELEASE = -Os -Wuninitialized -Wall
# -ffunction-sections -fdata-sections
CC_OPTIONS_STACK_CHECK =
CC_OPTIONS_POINTER_CHECK = 

# *****************************
#    DEFINE ARCHIVE OPTIONS
# *****************************
AR_OPTIONS = -rv $@ $^
AR_EXTRACT_OPTIONS = -x
AR_REBUILD_OPTIONS = -rv    
AR_DELETE_OPTIONS = -d

# ****************************************
#     DEFINE ASSEMBLY COMPILING OPTIONS
# ****************************************
AS_OPTIONS = \
    -EL -mips32 \
    -I. \
    -o \
    $@ \
    $<


# *************************************************
#    DEFINE EXTENSION FOR EXECUTABLE
# *************************************************
ifeq ($(PRODUCT_ENV),unix)
EXE_SUFFIX = out
BIN_SUFFIX = bin
HEX_SUFFIX = hex
endif

ifeq ($(PRODUCT_ENV),windows)
EXE_SUFFIX = out
BIN_SUFFIX = bin
HEX_SUFFIX = hex
endif

# *************************************************
#    DEFINE EXTENSION FOR OBJECTS
# *************************************************
OBJ_SUFFIX=o

# *************************************************
#    DEFINE EXTENSION FOR LIBRAIRIES
# *************************************************
LIB_SUFFIX=a
LIB_PREFFIX=lib

CD   = cd
CP   = cp -r
ECHO = echo
NULL = /dev/null
RM   = -rm -f
MAKE = make
MOVE = mv
MKDIR= mkdir
RMDIR= -rm -rf 

# *************************************************
#    DEFINE BOOTLOADER TOOLS
# *************************************************
ifeq ($(PRODUCT_ENV),unix)
    ifneq "$(PRODUCT_BOOTLOADER)" "BOOTLOADER_OFF"
        $(warning "No Unix bootloader tools defined ")
    endif
endif

ifeq ($(PRODUCT_ENV),windows)
    ifneq "$(PRODUCT_BOOTLOADER)" "BOOTLOADER_OFF"
        $(warning "No Windows bootloader tools defined ")
    endif
endif
