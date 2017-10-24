ECOS_GLOBAL_CFLAGS = -EL -fno-optimize-sibling-calls -mips32r2 -mfp32 -mhard-float -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -Woverloaded-virtual -gdwarf-2 -O2 -ffunction-sections -fdata-sections -fno-exceptions -G0 -frtti
ECOS_GLOBAL_LDFLAGS = -EL -mhard-float -g -nostdlib -Wl,--gc-sections -Wl,-static
ECOS_COMMAND_PREFIX = mipsisa32-elf-
