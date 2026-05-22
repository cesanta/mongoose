ENTRY(Reset_Handler);
MEMORY {
  ROM    (xrw)    : ORIGIN = 0x34180400,   LENGTH = 255K
  RAM    (xrw)    : ORIGIN = 0x341C0000,   LENGTH = 256K
}
_estack = ORIGIN(RAM) + LENGTH(RAM);
_sstack = _estack - 4096;

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) }  > ROM
  .text     : { *(.text* .text.*) }     > ROM
  .rodata   : { *(.rodata*) }           > ROM

  .data : { _sdata = .; *(.first_data) *(.data SORT(.data.*)) _edata = .; } > RAM AT > ROM
  _sidata = LOADADDR(.data);

  .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > RAM
  . = ALIGN(8);
  _end = .;
}
