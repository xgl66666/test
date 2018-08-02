OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips",
              "elf32-littlemips")


OUTPUT_ARCH(mips:isa32)

STARTUP(vectors.o)
ENTRY(reset_vector)
INPUT(extras.o)
GROUP( libtarget.a libgcc.a libsupc++.a libstdc++.a )
MEMORY
{
    ram : ORIGIN = 0x80000400, LENGTH = 0x1fffc00
    rom : ORIGIN = 0x9fc00000, LENGTH = 0x400000
}

SECTIONS
{
   
    .rom_vectors 0x9fc00000 : { KEEP (*(.reset_vector)) . = ALIGN(0x200); KEEP (*(.utlb_vector)) . = . + 0x100; . = ALIGN(0x80); KEEP(*(.other_vector)) . = . + 0x80; . = ALIGN(0x80); KEEP(*(.debug_vector)) } > rom
    .interp ALIGN (0x40) : { *(.interp) } > rom .hash : AT ((LOADADDR (.interp) + SIZEOF (.interp) + 0x40 - 1) & ~ (0x40 - 1)) { *(.hash) } > rom .dynsym : AT ((LOADADDR (.hash) + SIZEOF (.hash) + 0x40 - 1) & ~ (0x40 - 1)) { *(.dynsym) } > rom .dynstr : AT ((LOADADDR (.dynsym) + SIZEOF (.dynsym) + 0x40 - 1) & ~ (0x40 - 1)) { *(.dynstr) } > rom .gnu.version : AT ((LOADADDR (.dynstr) + SIZEOF (.dynstr) + 0x40 - 1) & ~ (0x40 - 1)) { *(.gnu.version) } > rom .gnu.version_d : AT ((LOADADDR (.gnu.version) + SIZEOF (.gnu.version) + 0x40 - 1) & ~ (0x40 - 1)) { *(.gnu.version_d) } > rom .gnu.version_r : AT ((LOADADDR (.gnu.version_d) + SIZEOF (.gnu.version_d) + 0x40 - 1) & ~ (0x40 - 1)) { *(.gnu.version_r) } > rom .plt : AT ((LOADADDR (.gnu.version_r) + SIZEOF (.gnu.version_r) + 0x40 - 1) & ~ (0x40 - 1)) { *(.plt) } > rom
    .rel.text : { *(.rel.text) *(.rel.text.*) *(.rel.gnu.linkonce.t*) } > rom .rela.text : { *(.rela.text) *(.rela.text.*) *(.rela.gnu.linkonce.t*) } > rom .rel.data : { *(.rel.data) *(.rel.data.*) *(.rel.gnu.linkonce.d*) } > rom .rela.data : { *(.rela.data) *(.rela.data.*) *(.rela.gnu.linkonce.d*) } > rom .rel.rodata : { *(.rel.rodata) *(.rel.rodata.*) *(.rel.gnu.linkonce.r*) } > rom .rela.rodata : { *(.rela.rodata) *(.rela.rodata.*) *(.rela.gnu.linkonce.r*) } > rom .rel.got : { *(.rel.got) } > rom .rela.got : { *(.rela.got) } > rom .rel.ctors : { *(.rel.ctors) } > rom .rela.ctors : { *(.rela.ctors) } > rom .rel.dtors : { *(.rel.dtors) } > rom .rela.dtors : { *(.rela.dtors) } > rom .rel.init : { *(.rel.init) } > rom .rela.init : { *(.rela.init) } > rom .rel.fini : { *(.rel.fini) } > rom .rela.fini : { *(.rela.fini) } > rom .rel.bss : { *(.rel.bss) } > rom .rela.bss : { *(.rela.bss) } > rom .rel.plt : { *(.rel.plt) } > rom .rela.plt : { *(.rela.plt) } > rom .rel.dyn : { *(.rel.dyn) } > rom
    .init ALIGN (0x40) : { . = .; KEEP (*(.init)) } > rom =0
    .text ALIGN (0x40) : { _stext = .; _ftext = . ; *(.text) *(.text.*) *(.stub) *(.gnu.warning) *(.gnu.linkonce.t*) *(.mips16.fn.*) *(.mips16.call.*) } > rom =0 _etext = .; PROVIDE (etext = .);
    .fini ALIGN (0x40) : { . = .; KEEP (*(.fini)) } > rom =0
    .rodata ALIGN (0x40) : { . = .; *(.rodata) *(.rodata.*) *(.gnu.linkonce.r*) } > rom
    .rodata1 ALIGN (0x40) : { . = .; *(.rodata1) *(.rodata1.*) } > rom
    .data 0x80000400 : AT ((LOADADDR (.rodata1) + SIZEOF (.rodata1) + 0x40 - 1) & ~ (0x40 - 1)) { __ram_data_start = ABSOLUTE (.); _fdata = . ; *(.data) *(.data.*) *(.gnu.linkonce.d*) *( .2ram.*) . = ALIGN (8); SORT(CONSTRUCTORS) } > ram __rom_data_start = LOADADDR(.data);
    .data1 ALIGN (0x40) : AT ((LOADADDR (.data) + SIZEOF (.data) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.data1) *(.data1.*) } > ram
    .eh_frame ALIGN (0x40) : AT ((LOADADDR (.data1) + SIZEOF (.data1) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame)) __FRAME_END__ = .; . = . + 8; } > ram = 0
    .gcc_except_table ALIGN (0x40) : AT ((LOADADDR (.eh_frame) + SIZEOF (.eh_frame) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; KEEP(*(.gcc_except_table)) *(.gcc_except_table.*) } > ram
    .ctors ALIGN (0x40) : AT ((LOADADDR (.gcc_except_table) + SIZEOF (.gcc_except_table) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; KEEP (*crtbegin.o(.ctors)) __CTOR_LIST__ = .; PROVIDE (__CTOR_LIST__ = .); KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors)) KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) __CTOR_END__ = .; PROVIDE (__CTOR_END__ = .); } > ram
    .dtors ALIGN (0x40) : AT ((LOADADDR (.ctors) + SIZEOF (.ctors) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; KEEP (*crtbegin.o(.dtors)) __DTOR_LIST__ = .; PROVIDE (__DTOR_LIST__ = .); KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors)) KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors)) __DTOR_END__ = .; PROVIDE (__DTOR_END__ = .); } > ram
    .devtab ALIGN (0x40) : AT ((LOADADDR (.dtors) + SIZEOF (.dtors) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; KEEP(*( SORT (.ecos.table.*))) ; } > ram
    _gp = ALIGN(16) + 0x7ff0; .got ALIGN (0x40) : AT ((LOADADDR (.devtab) + SIZEOF (.devtab) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.got.plt) *(.got) } > ram
    .dynamic ALIGN (0x40) : AT ((LOADADDR (.got) + SIZEOF (.got) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.dynamic) } > ram
    .sdata ALIGN (0x40) : AT ((LOADADDR (.dynamic) + SIZEOF (.dynamic) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.sdata) *(.sdata.*) *(.gnu.linkonce.s*) } > ram
    .lit8 ALIGN (0x40) : AT ((LOADADDR (.sdata) + SIZEOF (.sdata) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.lit8) } > ram
    .lit4 : AT ((LOADADDR (.lit8) + SIZEOF (.lit8) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.lit4) } > ram __ram_data_end = .; _edata = . ; PROVIDE (edata = .);
    __bss_start = .; _fbss = .; .sbss ALIGN (0x40) : { . = .; *(.dynsbss) *(.sbss) *(.sbss.*) *(.scommon) *(.gnu.linkonce.sb.*) } > ram
    .bss ALIGN (0x40) : { *(.dynbss) *(.bss) *(.bss.*) *(COMMON) *(.gnu.linkonce.b.*) } > ram __bss_end = .;
    __heap1 = ALIGN (0x8);
    __pci_window = 0x81f00000; . = __pci_window + 0x100000;
    . = ALIGN(4); _end = .; PROVIDE (end = .); .stab 0 : { *(.stab) } .stabstr 0 : { *(.stabstr) } .stab.excl 0 : { *(.stab.excl) } .stab.exclstr 0 : { *(.stab.exclstr) } .stab.index 0 : { *(.stab.index) } .stab.indexstr 0 : { *(.stab.indexstr) } .comment 0 : { *(.comment) } .debug 0 : { *(.debug) } .line 0 : { *(.line) } .debug_srcinfo 0 : { *(.debug_srcinfo) } .debug_sfnames 0 : { *(.debug_sfnames) } .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .debug_weaknames 0 : { *(.debug_weaknames) } .debug_funcnames 0 : { *(.debug_funcnames) } .debug_typenames 0 : { *(.debug_typenames) } .debug_varnames 0 : { *(.debug_varnames) } .gptab.sdata : { *(.gptab.data) *(.gptab.sdata) } .gptab.sbss : { *(.gptab.bss) *(.gptab.sbss) } /DISCARD/ 0 : { *(.mdebug) }
}

hal_vsr_table = 0x80000200;
hal_virtual_vector_table = 0x80000300;
