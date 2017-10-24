# ******************************************************************************
#    DEFINE ENVIRONMENT OPERATING SYSTEM
# ******************************************************************************

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

# ******************************************************************************
#    DEFINE GOOD AND BAD PATH SEPARTORS
# ******************************************************************************

UNIX_SLASH ?= /
DOS_SLASH  ?= $(subst /,\,$(UNIX_SLASH))

ifeq ($(PRODUCT_ENV),windows)
    SLASH      ?= $(DOS_SLASH)
    GOOD_SLASH ?= $(DOS_SLASH)
    BAD_SLASH  ?= $(UNIX_SLASH)
endif

ifeq ($(PRODUCT_ENV),unix)
    SLASH      ?= $(UNIX_SLASH)
    GOOD_SLASH ?= $(UNIX_SLASH)
    BAD_SLASH  ?= $(DOS_SLASH)
endif

# ******************************************************************************
#    DEFINE DEFAULT EXTENSIONS FOR EXECUTABLES, OBJECTS AND LIBRARIES
# ******************************************************************************

INCLUDE_PREFIX ?= -I
LIBRARY_PREFIX ?= -L

ifeq ($(PRODUCT_OS),linux)
EXE_SUFFIX       ?= out
BIN_SUFFIX       ?= bin
HEX_SUFFIX       ?= hex
LIB_SUFFIX       ?= a
SHAREDLIB_SUFFIX ?= so
LIB_PREFFIX      ?= lib
endif

ifeq ($(PRODUCT_OS),windows)
EXE_SUFFIX       ?= out
BIN_SUFFIX       ?= bin
HEX_SUFFIX       ?= hex
OBJ_SUFFIX       ?= o
LIB_SUFFIX       ?= a
SHAREDLIB_SUFFIX ?= dll
LIB_PREFFIX      ?= lib
endif

# ******************************************************************************
#    DEFINE DEFAULT HOST TOOLS
# ******************************************************************************

ifeq ($(PRODUCT_ENV),unix)
CD    ?= cd
CP    ?= cp -r
ECHO  ?= echo
NULL  ?= /dev/null
RM    ?= -rm -f
MAKE  ?= make
MOVE  ?= mv
MKDIR ?= mkdir
RMDIR ?= -rm -rf
endif

ifeq ($(PRODUCT_ENV),windows)
CD    ?= cd
CP    ?= copy
ECHO  ?= echo
NULL  ?= NUL:
RM    ?= -del /F /Q
MAKE  ?= make
MOVE  ?= move
MKDIR ?= mkdir
RMDIR ?= -rmdir /S /Q
endif

# ******************************************************************************
#    DEFINE DEFAULT HOST TOOL CHAIN
# ******************************************************************************

ifeq ($(PRODUCT_ENV),unix)
CC       ?= gcc
LD       ?= g++
CC_PLUS  ?= g++
AS       ?= as
AR       ?= ar
STRIP    ?= strip
OBJ_COPY ?= objcopy
GDB      ?= gdb
endif

ifeq ($(PRODUCT_ENV),windows)
CC       ?= gcc
LD       ?= g++
CC_PLUS  ?= g++
AS       ?= as
AR       ?= ar
STRIP    ?= strip
OBJ_COPY ?= objcopy
GDB      ?= gdb
endif

# ******************************************************************************
#    DEFINE DEFAULT OPTIONS FOR HOST TOOL CHAIN
# ******************************************************************************

WL_OPTION  ?=

CFLAGS     ?=
CXX_FLAGS  ?=

CC_OPTIONS               ?=
CC_OPTIONS_DEBUG         ?= 
CC_OPTIONS_RELEASE       ?= 
CC_OPTIONS_STACK_CHECK   ?=
CC_OPTIONS_POINTER_CHECK ?=

AR_OPTIONS         ?= 
AR_EXTRACT_OPTIONS ?=
AR_REBUILD_OPTIONS ?=
AR_DELETE_OPTIONS  ?=

AS_OPTIONS ?=