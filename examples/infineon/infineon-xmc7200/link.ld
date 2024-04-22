OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    flash(RX) : ORIGIN = 0x10080000, LENGTH = 0x80000
    sram(!RX) : ORIGIN = 0x28000800, LENGTH = 510K
}

_estack     = ORIGIN(sram) + LENGTH(sram);

SECTIONS
{ 
    .vectors  : { . = ALIGN(4); KEEP(*(.vectors)); . = ALIGN(4); } > flash
    .text     : { *(.text*) } > flash
    .rodata   : { *(.rodata*) } > flash

    .data : {
        _sdata = .;
        *(.first_data)
        *(.data SORT(.data.*))
        _edata = .;
    } > sram AT > flash
    _sidata = LOADADDR(.data);

    .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > sram

    . = ALIGN(8);
    _end = .;
}
