.PHONY: all zip setup clean

UNAME=$(shell uname)

UNZIP_RAM_BASE ?= 0x82F70044
IMAGE_BIN_SIZE ?= 3452928
IMAGE_BIN      ?= $(BUILD_ROOT)/bin/comedia.bin
IMAGE_ZIP      ?= out/comedia.lzma
IMAGE_OUT      ?= $(BUILD_ROOT)/bin/comedia_lzma
IMAGE_OUT_OTA  ?= $(BUILD_ROOT)/bin/comedia_lzma_ota
IMAGE_RES      ?= $(BUILD_ROOT)/bin/resources/resources.lzma
ZIP_APP_BIN       ?= 0x80000180
ZIP_APP_BIN_ENTRY ?= 0x80000224

#TOOLCHAIN  ?= mips-linux-gnu
TOOLCHAIN  ?= mipsisa32-elf
CC         := $(TOOLCHAIN)-gcc
LD         := $(TOOLCHAIN)-ld
OBJCOPY    := $(TOOLCHAIN)-objcopy
OBJDUMP    := $(TOOLCHAIN)-objdump
SIZE       := $(TOOLCHAIN)-size
NM         := $(TOOLCHAIN)-nm
AS         := $(TOOLCHAIN)-as
TOOL_ENTRY := ./tools

CFLAGS := -EL -mips32r2 -mhard-float -gdwarf-2 -O2 -G0 -nostdlib -static \
	-fno-optimize-sibling-calls -ffunction-sections -fdata-sections -fno-exceptions \
	-I. -I./cpu/mips34k -Wa,-I./cpu/mips34k

SRC := LzmaDecMain.c LzmaDec.c zipbin.s cache.s
#KRONUS_U04 = enable
KRONUS_U04 = disable
ifeq ($(KRONUS_U04),enable)
CFLAGS += -D'ENABLE_KRONUS_U04'
endif

all: setup zip
	@$(CC) -P -E '-DUNZIP_RAM_BASE=$(UNZIP_RAM_BASE)' -o out/app_link.ld app_link.ld.S
	@$(CC) $(CFLAGS) -Wl,-Map,$(IMAGE_OUT).map -T out/app_link.ld -o $(IMAGE_OUT).out $(SRC)
#	@$(OBJDUMP) -S $(IMAGE_OUT).out > $(IMAGE_OUT).dis
#	@rm out/$(IMAGE_ZIP)
	@$(OBJCOPY) -O binary $(IMAGE_OUT).out $(IMAGE_OUT).bin
	@rm $(IMAGE_ZIP)
	@sh add_header_app.sh $(IMAGE_OUT).bin
	dd if=$(IMAGE_OUT).bin.signed.bin ibs=$(IMAGE_BIN_SIZE) count=1 of=$(IMAGE_OUT)_padding.bin conv=sync
	cat $(IMAGE_OUT)_padding.bin > $(IMAGE_OUT_OTA).bin
	cat $(IMAGE_RES) >> $(IMAGE_OUT_OTA).bin

zip:
	@echo "  [ZIP] $(IMAGE_BIN)"
ifeq ($(UNAME),Linux)
	@$(TOOL_ENTRY)/lzma e $(IMAGE_BIN) $(IMAGE_ZIP)
else
	@$(TOOL_ENTRY)/lzma.exe e '$(shell cygpath -w $(IMAGE_BIN))' '$(shell cygpath -w $(IMAGE_ZIP))'
endif

setup:
	@mkdir out 2> /dev/null || true
	@echo '.section .zipbin, "a"' > zipbin.s
	@echo '.incbin' '"$(IMAGE_ZIP)"' >> zipbin.s
	@echo '#define ZIP_APP_BIN $(ZIP_APP_BIN)' > zipbin.h
	@echo '#define ZIP_APP_BIN_ENTRY $(ZIP_APP_BIN_ENTRY)' >> zipbin.h

clean:
	rm -rf out/*
