OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    flash(RX) : ORIGIN = 0x0, LENGTH = 0x100000
    flash2(RX) : ORIGIN = 0x100000, LENGTH = 0x100000
    sram(!RX) : ORIGIN = 0x20000000, LENGTH = 0x60000
    sramx(WX) : ORIGIN = 0x04000000, LENGTH = 0x18000
}

_estack     = ORIGIN(sram) + LENGTH(sram);
__StackTop  = _estack;
__StackLimit = __StackTop - 0x2000;

SECTIONS
{ 
    .vectors  : { FILL(256) KEEP(*(.isr_vector)) } > flash
    .text     : { *(.text.* *.text.*) } > flash
    .rodata   : { *(.rodata*) }      > flash

    .data     : {
        _sdata = .;
        *(.first_data)
        *(.data SORT(.data.*))
        _edata = .;
    } > sram AT > flash
    _sidata = LOADADDR(.data);

    .bss      : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > sram

    . = ALIGN(8);
    _end = .;

    .iram     : {
        _siram = .;
         *(.iram)
        _eiram = .;
    } > sramx AT > flash
    _siiram = LOADADDR(.iram);

    /* Symbols expected by the CMSIS pack startup file. */
    __etext = _sidata;
    __data_start__ = _sdata;
    __data_end__ = _edata;
    __bss_start__ = _sbss;
    __bss_end__ = _ebss;
    __HeapBase = _end;
    __HeapLimit = _end;
 }
