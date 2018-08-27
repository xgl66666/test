# ****************************************
# Compilation & Generation Rules
# ****************************************

ifeq "$(DEBUG)" "1"
CC_OPTIONS += $(CC_OPTIONS_DEBUG)
else
CC_OPTIONS += $(CC_OPTIONS_RELEASE)
endif

ifeq "$(DEBUG_OS_STACK)" "1"
CC_OPTIONS += $(CC_OPTIONS_STACK_CHECK)
endif

ifeq "$(DEBUG_OS_POINTER)" "1"
CC_OPTIONS += $(CC_OPTIONS_POINTER_CHECK)
endif

# Check each  path include directive is defined
#$(foreach include_directive,  $(filter -I, $(DIR_INCLUDE)), $(warning undefined include_directive ($(include_directive)) in $(DIR_INCLUDE)))

%.$(OBJ_SUFFIX): %.c
	$(ECHO) Compiling $<
	$(CC) $< $(CFLAGS) $(INCLUDE_PREFIX)$(BUILD_INCLUDE) $(DIR_INCLUDE) $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) $(CC_OPTIONS) -o $@

%.$(OBJ_SUFFIX): %.cpp
	$(ECHO) Compiling $<
	$(CC_PLUS) $(CXX_FLAGS) $(CFLAGS) $(INCLUDE_PREFIX)$(BUILD_INCLUDE) $(DIR_INCLUDE) $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) $(CC_OPTIONS) -o $@ -c $<

%.$(OBJ_SUFFIX): %.s
	$(ECHO) Compiling $<
	$(AS) $(AS_OPTIONS)

$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX): $(LIBOBJS)
	$(ECHO) Exporting $@
	$(AR) $(AR_OPTIONS)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX))

extract:
	@$(ECHO) --- EXTRACT $(MODULE) $(notdir $(LIBOBJS)) ---
	$(foreach LIBR, $(LIBOBJS), $(shell $(AR) $(AR_EXTRACT_OPTIONS) $(LIBR)))

rebuild:
	@$(ECHO) ---- REBUILD ---- 
	$(AR) $(AR_REBUILD_OPTIONS) $(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX) *.$(OBJ_SUFFIX)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
	$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(LIB_PREFFIX)$(MODULE).$(LIB_SUFFIX))
	$(RM) *.$(OBJ_SUFFIX)

vpath %.h  $(subst -I,:,$(DIR_INCLUDE))

clean:
	@$(ECHO) ---- Cleaning [$(MODULE)] ----
	$(RM) *.tco *.o *.lib *.ab *.abg *.bdx *.hex *.map *.elf *.lku *.$(OBJ_SUFFIX) *.$(LIB_SUFFIX)
