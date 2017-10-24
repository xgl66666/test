OUTPUT(elf32-littlemips)


ENTRY(main)

MEMORY
{
    ram : ORIGIN = 0x82F70044, LENGTH = 10M
}

SECTIONS
{
    .text : AT(0x82F70044)
    {
 . = . + 0xBC;

        *(.text.entry)
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        . = ALIGN(4);
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        *(.data)
        *(.data.*)
        . = ALIGN(32);
    } > ram

    .zipbin : AT(LOADADDR(.text) + SIZEOF(.text))
    {
        _ld_zipbin_start = .;
        KEEP(*(.zipbin))
        _ld_zipbin_end = .;
        . = ALIGN(16);
    } > ram

    .reginfo ALIGN (0x40) : { *(.reginfo) } > ram
    .eh_frame ALIGN (0x40) : { *(.eh_frame) } > ram
    .MIPS ALIGN (0x40) : { *(.MIPS.*) } > ram
    .MIPS.pic_stubs : { *(.MIPS.pic_stubs) } > ram

    _gp = ALIGN(16);
    __got_start = .;
    .got : { *(.got) } > ram
    __got_end = .;

    .bss (NOLOAD):
    {
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
    } > ram

    . = ALIGN(0x40);
    .stack_end = .;
    . = . + 0x10000;
    __stack_begin = .;

}
