/* nRF52840 on the XIAO nRF52840, with the Adafruit UF2 bootloader. Flash:
 * MBR and SoftDevice S140 below 0x27000, the bootloader from 0xED000 up: the
 * application in between. The SoftDevice is not enabled, so all RAM is ours,
 * except the first 24k it would take: kept free, as the Arduino core does */
ENTRY(Reset_Handler);
MEMORY {
  flash(rx)  : ORIGIN = 0x00027000, LENGTH = 0xC6000
  sram(rwx)  : ORIGIN = 0x20006000, LENGTH = 0x3A000
}

SECTIONS {
  .vectors  : { KEEP(*(.isr_vector)) . = ALIGN(4); } > flash
  .text     : { *(.text* .text.*) } > flash
  .rodata   : { *(.rodata*) . = ALIGN(4); } > flash

  .data : {
    _sdata = .;
    *(.first_data)
    *(.iram .iram.*)  /* Mongoose MG_IRAM functions */
    *(.data SORT(.data.*))
    . = ALIGN(4);
    _edata = .;
  } > sram AT > flash
  __data_start = _sdata;
  __data_end = _edata;
  __data_load_start = LOADADDR(.data);
  /* Unused data regions expected by Nordic's standard startup. */
  __sdata_start = _edata;
  __sdata_end = _edata;
  __sdata_load_start = _edata;
  __tdata_start = _edata;
  __tdata_end = _edata;
  __tdata_load_start = _edata;
  __fast_start = _edata;
  __fast_end = _edata;
  __fast_load_start = _edata;

  .bss : {
    _sbss = .;
    *(.bss SORT(.bss.*) COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > sram
  __bss_start__ = _sbss;
  __bss_end__ = _ebss;
  __tbss_start__ = _ebss;
  __tbss_end__ = _ebss;
  __sbss_start__ = _ebss;
  __sbss_end__ = _ebss;

  /* Health record, see MG_HEALTH_RAM: not cleared, survives a warm reset */
  .mg_health (NOLOAD) : { KEEP(*(.mg_health .mg_health*)) } > sram

  . = ALIGN(8);
  _end = .;  /* Start of heap, see _sbrk() in hal.c */

  /* Nordic startup reserves its default 16 KiB stack at the top of RAM. */
  .stack ORIGIN(sram) + LENGTH(sram) - 0x4000 (NOLOAD) : {
    KEEP(*(.stack))
  } > sram
  ASSERT(_end <= __StackLimit, "RAM sections overlap the stack")
}
