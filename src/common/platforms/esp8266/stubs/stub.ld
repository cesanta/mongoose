/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 */

MEMORY {
  iram : org = 0x40100000, len = 0x8000
  dram : org = 0x3FFE8000, len = 0x14000
}

ENTRY(stub_main)

SECTIONS {
  .params 0x40100000 : {
    _params_start = ABSOLUTE(.);
    *(.params)
    _params_end = ABSOLUTE(.);
  } > iram

  .text : ALIGN(4) {
    _code_start = ABSOLUTE(.);
    *(.literal)
    *(.text .text.*)
  } > iram

  .bss : ALIGN(4) {
    _bss_start = ABSOLUTE(.);
    *(.bss)
    _bss_end = ABSOLUTE(.);
  } > dram

  .data : ALIGN(4) {
    _data_start = ABSOLUTE(.);
    *(.data)
    *(.rodata .rodata.*)
  } > dram
}

INCLUDE "eagle.rom.addr.v6.ld"

PROVIDE(SPIFlashModeConfig = 0x40004568);
PROVIDE(SPI_erase_sector = 0x400040c0);
PROVIDE(SPI_erase_block = 0x40004120);
PROVIDE(uart_div_modify = 0x400039d8);
