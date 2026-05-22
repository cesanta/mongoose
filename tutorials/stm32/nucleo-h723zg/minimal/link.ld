ENTRY(Reset_Handler);
MEMORY {
  flash(rx) : ORIGIN = 0x08000000, LENGTH = 1024k

  dtcmram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
  ram_d1 (rwx)  : ORIGIN = 0x24000000, LENGTH = 320K
  ram_d2 (rwx)  : ORIGIN = 0x30000000, LENGTH = 32K
  ram_d3 (rwx)  : ORIGIN = 0x38000000, LENGTH = 16K
  itcmram (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
}
_estack     = ORIGIN(ram_d1) + LENGTH(ram_d1);

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) }  > flash
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  /* Note the .iram section, for functions copied to RAM. Required for MG_IRAM OTA support */
  .data : { _sdata = .; *(.first_data) *(.iram .iram* .iram.*) *(.data SORT(.data.*)) _edata = .; } > ram_d1 AT > flash
  _sidata = LOADADDR(.data);

  .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > ram_d1

  .eth_ram : { *(.eth_ram .eth_ram*) } > ram_d1 AT > flash

  . = ALIGN(8);
  _end = .;
}
