/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright 2008-2013 Code Red Technologies Ltd,
 * Copyright 2013-2024 NXP
 * Generated linker script file for MIMXRT1062xxxxB
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.8.1 [Build 1197] [2023-10-27] on Jan 5, 2024, 8:59:47 PM
 */

MEMORY
{
  /* Define each memory region */
  BOARD_FLASH (rx) : ORIGIN = 0x60000000, LENGTH = 0x800000 /* 8M bytes (alias Flash) */  
  SRAM_DTC (rwx) : ORIGIN = 0x20000000, LENGTH = 0x20000 /* 128K bytes (alias RAM) */  
  SRAM_ITC (rwx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128K bytes (alias RAM2) */  
  SRAM_OC2 (rwx) : ORIGIN = 0x20200000, LENGTH = 0x80000 /* 512K bytes (alias RAM3) */  
  SRAM_OC (rwx) : ORIGIN = 0x20280000, LENGTH = 0x40000 /* 256K bytes (alias RAM4) */  
  BOARD_SDRAM (rwx) : ORIGIN = 0x80000000, LENGTH = 0x1e00000 /* 30M bytes (alias RAM5) */  
  NCACHE_REGION (rwx) : ORIGIN = 0x81e00000, LENGTH = 0x200000 /* 2M bytes (alias RAM6) */  
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_FLASH = 0x60000000  ; /* BOARD_FLASH */  
  __base_Flash = 0x60000000 ; /* Flash */  
  __top_BOARD_FLASH = 0x60000000 + 0x800000 ; /* 8M bytes */  
  __top_Flash = 0x60000000 + 0x800000 ; /* 8M bytes */  
  __base_SRAM_DTC = 0x20000000  ; /* SRAM_DTC */  
  __base_RAM = 0x20000000 ; /* RAM */  
  __top_SRAM_DTC = 0x20000000 + 0x20000 ; /* 128K bytes */  
  __top_RAM = 0x20000000 + 0x20000 ; /* 128K bytes */  
  __base_SRAM_ITC = 0x0  ; /* SRAM_ITC */  
  __base_RAM2 = 0x0 ; /* RAM2 */  
  __top_SRAM_ITC = 0x0 + 0x20000 ; /* 128K bytes */  
  __top_RAM2 = 0x0 + 0x20000 ; /* 128K bytes */  
  __base_SRAM_OC2 = 0x20200000  ; /* SRAM_OC2 */  
  __base_RAM3 = 0x20200000 ; /* RAM3 */  
  __top_SRAM_OC2 = 0x20200000 + 0x80000 ; /* 512K bytes */  
  __top_RAM3 = 0x20200000 + 0x80000 ; /* 512K bytes */  
  __base_SRAM_OC = 0x20280000  ; /* SRAM_OC */  
  __base_RAM4 = 0x20280000 ; /* RAM4 */  
  __top_SRAM_OC = 0x20280000 + 0x40000 ; /* 256K bytes */  
  __top_RAM4 = 0x20280000 + 0x40000 ; /* 256K bytes */  
  __base_BOARD_SDRAM = 0x80000000  ; /* BOARD_SDRAM */  
  __base_RAM5 = 0x80000000 ; /* RAM5 */  
  __top_BOARD_SDRAM = 0x80000000 + 0x1e00000 ; /* 30M bytes */  
  __top_RAM5 = 0x80000000 + 0x1e00000 ; /* 30M bytes */  
  __base_NCACHE_REGION = 0x81e00000  ; /* NCACHE_REGION */  
  __base_RAM6 = 0x81e00000 ; /* RAM6 */  
  __top_NCACHE_REGION = 0x81e00000 + 0x200000 ; /* 2M bytes */  
  __top_RAM6 = 0x81e00000 + 0x200000 ; /* 2M bytes */  
