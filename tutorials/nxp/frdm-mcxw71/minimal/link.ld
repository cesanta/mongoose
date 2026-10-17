/* MCXW716C memory map, see MCUXpresso SDK gcc/mcxw716_flash.ld */
ENTRY(Reset_Handler);
MEMORY {
  flash(rx)  : ORIGIN = 0x00000000, LENGTH = 1024k
  sram(rwx)  : ORIGIN = 0x20000000, LENGTH = 112k
}
__StackTop = ORIGIN(sram) + LENGTH(sram);  /* Stack points here */
__StackLimit = __StackTop - 8192;          /* MSPLIM, see startup */

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) . = ALIGN(4); } > flash
  .text     : { *(.text* .text.*) } > flash
  .rodata   : { *(.rodata*) . = ALIGN(4); } > flash

  .data : {
    __data_start__ = .;
    _sdata = .;
    *(.first_data)
    *(.data SORT(.data.*))
    . = ALIGN(4);
    __data_end__ = .;
    _edata = .;
  } > sram AT > flash
  __etext = LOADADDR(.data);  /* Startup copies .data from here */

  .bss : {
    __bss_start__ = .;
    _sbss = .;
    *(.bss SORT(.bss.*) COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    _ebss = .;
  } > sram

  . = ALIGN(8);
  _end = .;  /* Start of heap, see _sbrk() in hal.c */
}
