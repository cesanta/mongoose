ENTRY(Reset_Handler);
MEMORY {
  flash (rx)  : ORIGIN = 0x70100000, LENGTH = 64M - 1M
  sram  (rwx) : ORIGIN = 0x34000000, LENGTH = 2048K
}
_estack = ORIGIN(sram) + LENGTH(sram);
_sstack = _estack - 4096;

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) }  > flash
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  /* Note the .iram section, for functions copied to RAM */
  .data : { _sdata = .; *(.first_data) *(.iram .iram* .iram.*) *(.data SORT(.data.*)) _edata = .; } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > sram
  . = ALIGN(8);
  _end = .;
}
