
#------------------------------------------------------------------------------
#
# Purpose of this makefile is to add all external 
# libraries to linking stage, so they do not need 
# to be added separately in different places
#
#------------------------------------------------------------------------------

EXTERN_LIBS = 
EXTERN_LIBS_PATH = 

#------------------------------------------
# Notify library
#------------------------------------------

ifeq "$(PRODUCT_USE_DDM)" "YES"
EXTERN_MW_LIBS+=$(NOTIFY_LIB)/$(LIB_PREFFIX)notify.$(LIB_SUFFIX)
endif

ifeq "$(PRODUCT_APP)" "CELINE"
#ifeq "$(PRODUCT_USE_LUA)" "YES"
EXTERN_MW_LIBS+=$(LUA_LIB)/$(LIB_PREFFIX)lua.$(LIB_SUFFIX)

ifeq "$(CELINE_SIMULATOR)" "YES"
ifeq ($(OS),Windows_NT)
#needed on WINDOWS for lua debugger to work
EXTERN_LIBS+=$(LIB_PREFFIX)ws2_32.$(LIB_SUFFIX)
endif
endif

#TOLUA++
EXTERN_MW_LIBS+=$(TOLUAPP_LIB)/$(LIB_PREFFIX)toluapp.$(LIB_SUFFIX)

#endif

# MINIZIP
EXTERN_MW_LIBS+=$(MINIZIP_LIB)/$(LIB_PREFFIX)minizip.$(LIB_SUFFIX)


# jpeg
EXTERN_MW_LIBS+=$(JPEG_LIB)/$(LIB_PREFFIX)jpeg.$(LIB_SUFFIX)


# SILLY
EXTERN_MW_LIBS+=$(SILLY_LIB)/$(LIB_PREFFIX)SILLY.$(LIB_SUFFIX)
CFLAGS+=-DSILLY_OPT_INLINE

# TinyXML or Expat
ifeq "$(CELINE_XML_LIB)" "TINY_XML"
EXTERN_MW_LIBS+=$(TiXML_LIB)/$(LIB_PREFFIX)tinyxml.$(LIB_SUFFIX)
else
EXTERN_MW_LIBS+=$(EXPAT_LIB)/$(LIB_PREFFIX)expat.$(LIB_SUFFIX)
endif

endif

ifneq "$(PRODUCT_LIBMW_INCLUDE_EXTERNLIBS)" "YES"
EXTERN_LIBS+=$(EXTERN_MW_LIBS)
endif

EXTERN_LIBS_PATH+=$(EXTERN_MW_LIBS_PATH)