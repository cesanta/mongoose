ENTRY(Reset_Handler);
MEMORY {
  flash(rx) : ORIGIN = 0x08000000, LENGTH = 1024k
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 256k - 1k
  health(rw) : ORIGIN = 0x2003fc00, LENGTH = 1k  /* Retained health record */
}
__StackTop = ORIGIN(sram) + LENGTH(sram);  /* End of RAM. stack points here */
_estack = __StackTop;                      /* Used by the health fault handler */
__StackLimit = __StackTop - 16384;         /* Stack limit, MSPLIM. TLS needs > 4 KB */

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) }     > flash
  .text     : { *(.text* .text.*) }     > flash
  .rodata   : { *(.rodata*) }           > flash

  /* Copy and zero tables for the CMSIS startup code, see __cmsis_start() */
  .copy.table : {
    __copy_table_start__ = .;
    LONG(LOADADDR(.data)) LONG(ADDR(.data)) LONG(SIZEOF(.data) / 4)
    __copy_table_end__ = .;
  } > flash
  .zero.table : {
    __zero_table_start__ = .;
    LONG(ADDR(.bss)) LONG(SIZEOF(.bss) / 4)
    __zero_table_end__ = .;
  } > flash

  /* Note the .iram section, for functions copied to RAM. Required for MG_IRAM OTA support */
  .data : { _sdata = .; *(.first_data) *(.iram .iram* .iram.*) *(.data SORT(.data.*)) . = ALIGN(4); _edata = .; } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) . = ALIGN(4); _ebss = .; } > sram
  _end = ALIGN(_ebss, 8);  /* Heap starts after .bss */

  /* Retained health record. Startup code must neither copy nor clear it. */
  .mg_health (NOLOAD) : { KEEP(*(.mg_health .mg_health*)) } > health
}
