OUTPUT_FORMAT("elf32-tradlittlemips")


OUTPUT_ARCH(mips)

/* STARTUP(vectors.o) */
ENTRY(reset_vector)

MEMORY
{
    ram : ORIGIN = 0x835F0180, LENGTH = 0xA0FE80 /* 10MB */
}

SECTIONS
{
    .rom_vectors : { __ram_code_start = .; KEEP(*(.other_vector)) . = . + 0x80; . = ALIGN (0x100); KEEP (*(.reset_vector))} > ram
    . = ALIGN(0x2000);_readonly_start = .;
    .logo ALIGN (0x40) : { *(.logo.size) *(.logo)} > ram
    .text ALIGN (0x40) : { *(.text) *(.text.*) } > ram
    .rodata ALIGN (0x40) : { . = .; *(.rodata) *(.rodata.*) } > ram
    . = ALIGN(0x2000);_readonly_end = .;
	.data ALIGN (0x40) : { *(.data) *(.data.*)} > ram
	.reginfo ALIGN (0x40) : { *(.reginfo) } > ram
	.eh_frame ALIGN (0x40) : { *(.eh_frame) } > ram
	.MIPS ALIGN (0x40) : { *(.MIPS.*) } > ram

	_gp = ALIGN(16);
	__got_start = .;
	.got  : { *(.got) } > ram
	__got_end = .;

	__u_boot_cmd_start = .;
	.u_boot_cmd : { *(.u_boot_cmd) } > ram
	__u_boot_cmd_end = .;

	__bss_start = .;
	uboot_end_data = .;
	.bss ALIGN (0x40) : { *(.dynbss) *(.bss) *(.bss.*) *(.common) } > ram
	.sbss ALIGN (0x40) : { *(.dynsbss) *(.sbss) *(.sbss.*) *(.scommon) } > ram
	uboot_end = .;
	__bss_end = .;

	/* Stack is 16384-byte, ISR and user uses the same stack */
    . = ALIGN(0x40); .stack_end = .; . = . + 0x12000; __stack_begin = .;

    /* for sbrk.c */
    . = ALIGN (0x8);
    __heap = .; __heap_end = __heap + 0x630000;
}

