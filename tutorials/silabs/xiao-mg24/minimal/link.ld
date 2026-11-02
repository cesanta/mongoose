/* EFR32MG24B220F1536IM48 memory map, see Simplicity SDK linkerfile_base.ld */
ENTRY(Reset_Handler);
MEMORY {
  flash(rx)  : ORIGIN = 0x08000000, LENGTH = 1536k
  sram(rwx)  : ORIGIN = 0x20000000, LENGTH = 256k
}
__StackTop = ORIGIN(sram) + LENGTH(sram);  /* Stack points here */
__StackLimit = __StackTop - 8192;          /* MSPLIM, see startup */

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) . = ALIGN(4); } > flash
  .text     : { *(.text* .text.* text_*) } > flash  /* text_*: SDK code */
  .rodata   : { *(.rodata*) . = ALIGN(4); } > flash

  /* RAM functions, copied by CopyToRam() in the startup code */
  .ramfuncs : {
    . = ALIGN(4);
    __ramfuncs_start__ = .;
    *(.ram .ram.*)
    . = ALIGN(4);
    __ramfuncs_end__ = .;
  } > sram AT > flash
  __lma_ramfuncs_start__ = LOADADDR(.ramfuncs);
  __lma_ramfuncs_end__ = LOADADDR(.ramfuncs) + SIZEOF(.ramfuncs);

  .data : {
    __data_start__ = .;
    *(.first_data)
    *(.iram .iram.*)  /* Mongoose MG_IRAM functions */
    *(.data SORT(.data.*))
    . = ALIGN(4);
    __data_end__ = .;
  } > sram AT > flash
  __etext = LOADADDR(.data);  /* Startup copies .data from here */

  .bss : {
    __bss_start__ = .;
    *(.bss SORT(.bss.*) COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > sram

  /* Health record, see MG_HEALTH_RAM: not cleared, survives a warm reset */
  .mg_health (NOLOAD) : { KEEP(*(.mg_health .mg_health*)) } > sram

  . = ALIGN(8);
  _end = .;  /* Start of heap, see _sbrk() in hal.c */
}
