ENTRY(Reset_Handler);
MEMORY {
  flash_hdr(rx)  : ORIGIN = 0x60000000, LENGTH = 8k
  flash_irq(rx)  : ORIGIN = 0x60002000, LENGTH = 1k
  flash_code(rx) : ORIGIN = 0x60002400, LENGTH = 8183k

  itcram(rx)     : ORIGIN = 0x00000000, LENGTH = 64k
  dtcram(rw)     : ORIGIN = 0x20000000, LENGTH = 64k 
  ocram(rw)      : ORIGIN = 0x20200000, LENGTH = 128k 
}
__StackTop = ORIGIN(dtcram) + LENGTH(dtcram);

/* TODO(): separate itcram and go back to using dtcram for data and bss when ota is finished */

SECTIONS {
  .hdr  : { FILL(0xff) ; KEEP(*(.cfg)) . = 0x1000 ; KEEP(*(.ivt)) . = 0x1020 ;
            KEEP(*(.dat)) . = 0x1030 ; KEEP(*(.dcd)) . = 0x2000 ;} >flash_hdr
  .irq  : { KEEP(*(.isr_vector)) }  > flash_irq
  .text : { *(.text* .text.*) *(.rodata*) ; } > flash_code
  .data : { __data_start__ = .; *(.data SORT(.data.*)) *(.iram) __data_end__ = .; } > itcram AT > flash_code
  __etext = LOADADDR(.data);
  .bss  : { __bss_start__ = .; *(.bss SORT(.bss.*) COMMON) __bss_end__ = .; } > itcram
  _end = .;
}
