ENTRY(Reset_Handler);
MEMORY {
  flash_cfg(rx)  : ORIGIN = 0x60000000, LENGTH = 4k
  flash_ivt(rx)  : ORIGIN = 0x60001000, LENGTH = 4k
  flash_irq(rx)  : ORIGIN = 0x60002000, LENGTH = 1k
  flash_code(rx) : ORIGIN = 0x60002400, LENGTH = 8183k

  ram0(rx)       : ORIGIN = 0x00000000, LENGTH = 64k
  ram1(rw)       : ORIGIN = 0x20000000, LENGTH = 64k 
  ram2(rw)       : ORIGIN = 0x20200000, LENGTH = 128k 
}
__StackTop = ORIGIN(ram2) + LENGTH(ram2);

SECTIONS {
  .cfg  : { __FLASH_BASE = .; KEEP(* (.cfg)) } > flash_cfg
  .ivt  : { KEEP(*(.ivt)) } > flash_ivt
  .irq  : { KEEP(*(.isr_vector)) }  > flash_irq
  .text : { *(.text* .text.*) *(.rodata*) __etext = .; } > flash_code
  .data : { __data_start__ = .; *(.data SORT(.data.*)) __data_end__ = .; } > ram1 AT > flash_code
  .bss  : { __bss_start__ = .; *(.bss SORT(.bss.*) COMMON) __bss_end__ = .; } > ram1
  _end = .;
}
