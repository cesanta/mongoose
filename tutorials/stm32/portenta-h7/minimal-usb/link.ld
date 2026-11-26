/* DTCMRAM (rwx)  : ORIGIN = 0x20000400, LENGTH = 128k - 1k */ /* bootloader starts at 0x20000000 */

ENTRY(Reset_Handler);
MEMORY {
  flash(rx) : ORIGIN = 0x08040000, LENGTH = 2048k - 0x40000 /* bootloader starts at 0x08000000 */
  sram(rwx) : ORIGIN = 0x24000000, LENGTH = 512k
}
_estack     = ORIGIN(sram) + LENGTH(sram);    /* stack points to end of SRAM */

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) }  > flash
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  .data : {
    _sdata = .;
    *(.first_data)
    *(.data SORT(.data.*))
    *(.iram .iram* .iram.*)
    _edata = .;
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;
  } > sram

  . = ALIGN(8);
  _end = .;
}

