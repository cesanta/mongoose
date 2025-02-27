ENTRY(Reset_Handler);
MEMORY {
  itcram(rx)       : ORIGIN = 0x00000000, LENGTH = 128k
  dtcram(rw)       : ORIGIN = 0x20000000, LENGTH = 128k
  ocram(rw)        : ORIGIN = 0x20200000, LENGTH = 256k   /* Is this cached ? */
}
__StackTop = ORIGIN(dtcram) + LENGTH(dtcram);

SECTIONS {
  .irq  : { KEEP(*(.isr_vector)) }  > itcram
  .text : { *(.text* .text.*) *(.rodata*) ; } > itcram
  .data : { __data_start__ = .; *(.data SORT(.data.*)) __data_end__ = .; } > dtcram AT > itcram
  __etext = LOADADDR(.data);
  .bss  : { __bss_start__ = .; *(.bss SORT(.bss.*) COMMON) __bss_end__ = .; } > dtcram
  _end = .;
}
