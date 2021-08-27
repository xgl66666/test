.PHONY: all zip setup

UNAME=$(shell uname)

IMAGGE_ZIP ?=
ZIP_APP_BIN ?=
ZIP_APP_BIN_ENTRY ?=

CFLAGS := -EL -mips32r2 -msoft-float -gdwarf-2 -O2 -G0 -nostdlib -static \
	-fno-optimize-sibling-calls -ffunction-sections -fdata-sections -fno-exceptions \
	-I. -I../sboot/cpu/mips/mips34k -Wa,-I../sboot/cpu/mips/mips34k \
	-I. -I../sboot/chip/keltic -Wa,-I../sboot/chip/keltic

SRC := LzmaDecMain.c LzmaDec.c ../sboot/cpu/mips/cache.s out/zip.o
SBOOT_APP_BIN := ../sboot/bin/app_Q.bin

all: zip setup
	@echo '  [BUILD] $(SBOOT_APP_BIN)'
	@$(OBJCOPY) -I binary -O elf32-littlemips -B mips --rename-section .data=.zipbin $(IMAGE_ZIP) out/zip.o
	@$(CC) $(CFLAGS) -Wl,-Map,out/unzip.map -T link.ld -o out/unzip.elf $(SRC)
	@$(OBJDUMP) -S out/unzip.elf > out/unzip.dis
	@$(OBJCOPY) -O binary out/unzip.elf $(SBOOT_APP_BIN)
	@ls -l ../sboot/bin/app_Q.bin
	@echo

zip:
	@echo '  [LZMA] $(IMAGE_BIN)'
ifeq ($(UNAME),Linux)
	@./lzma e $(IMAGE_BIN) $(IMAGE_ZIP)
else
	@./lzma.exe e '$(shell cygpath -w $(IMAGE_BIN))' '$(shell cygpath -w $(IMAGE_ZIP))'
endif


setup:
	@mkdir out 2> /dev/null || true
	@echo '#define ZIP_APP_BIN $(ZIP_APP_BIN)' > zipbin.h
	@echo '#define ZIP_APP_BIN_ENTRY $(ZIP_APP_BIN_ENTRY)' >> zipbin.h
