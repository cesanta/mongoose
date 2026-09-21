OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    flash(RX) : ORIGIN = 0x0, LENGTH = 0x180000
    flash2(RX) : ORIGIN = 0x10000000, LENGTH = 0x80000
    sram(RWX) : ORIGIN = 0x1ffe0000, LENGTH = 0x3f000
}

__StackTop     = ORIGIN(sram) + LENGTH(sram);

SECTIONS
{ 
    .vectors  : { FILL(256) KEEP(*(.isr_vector)) } > flash
    . = 0x400 ;
    .FlashConfig : { KEEP(*(.FlashConfig)) } > flash
    .text     : { *(.text.* *.text.*) } > flash
    .rodata   : { *(.rodata*) }      > flash

    .data     : {
        __data_start__ = .;
        *(.first_data)
        *(.data SORT(.data.*))
        __data_end__ = .;
    } > sram AT > flash
    __etext = LOADADDR(.data);

    .bss      : { __bss_start__ = .; *(.bss SORT(.bss.*) COMMON) __bss_end__ = .; } > sram

    . = ALIGN(8);
    _end = .;

}
