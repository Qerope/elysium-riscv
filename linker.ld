ENTRY(_start)

SECTIONS
{
    . = 0x80200000;
    .text : { *(.text.entry) *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { _bss = .; *(.bss*) *(COMMON) _ebss = .; }
    . = ALIGN(4096);
    _kend = .;
}
