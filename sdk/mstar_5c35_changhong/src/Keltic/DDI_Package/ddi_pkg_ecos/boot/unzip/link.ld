OUTPUT(elf32-littlemips)
ENTRY(main)

MEMORY
{
    ram : ORIGIN = 0x81000100, LENGTH = 10M
}

SECTIONS
{
    .text : AT(0x81000100)
    {
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

    .bss (NOLOAD):
    {
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
    } > ram


}
