/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

MEMORY {
  iram     : org = 0x40090000, len = 0x10000
  /* DRAM startin at 0x3FFC0000 gets stomped by something before mem_finish
   * and is thus not suitable for initialized data, but works fine for BSS. */
  dram_bss : org = 0x3FFC0000, len = 0x10000
  dram     : org = 0x3FFD0000, len = 0x10000
}

ENTRY(stub_main)

SECTIONS {
  .params 0x40090000 : {
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

INCLUDE "components/esp32/ld/esp32.rom.ld"
INCLUDE "components/esp32/ld/esp32.rom.spiram_incompatible_fns.ld"

PROVIDE(esp_rom_spiflash_attach = 0x40062a6c);
PROVIDE(esp_rom_spiflash_config_clk = 0x40062bc8);
