OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    flash(RX) : ORIGIN = 0x0, LENGTH = 0x100000
    flash2(RX) : ORIGIN = 0x100000, LENGTH = 0x100000
    sram(!RX) : ORIGIN = 0x20000000, LENGTH = 0x60000
}

_estack     = ORIGIN(sram) + LENGTH(sram);

SECTIONS
{ 
    .vectors  : { FILL(256) KEEP(*(.isr_vector)) } > flash
    .text     : { *(.text*) } > flash
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
