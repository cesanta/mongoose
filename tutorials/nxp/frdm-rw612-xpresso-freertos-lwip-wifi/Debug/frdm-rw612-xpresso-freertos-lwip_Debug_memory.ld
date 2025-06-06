/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright 2008-2013 Code Red Technologies Ltd,
 * Copyright 2013-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 * Generated linker script file for RW612
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v24.12 [Build 148] [2025-01-10] on Jun 6, 2025, 7:53:58 PM
 */

MEMORY
{
  /* Define each memory region */
  QSPI_FLASH (rx) : ORIGIN = 0x8000000, LENGTH = 0x800000 /* 8M bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x130000 /* 1216K bytes (alias RAM) */  
  MBOX1 (rwx) : ORIGIN = 0x41380000, LENGTH = 0x488 /* 1160 bytes (alias RAM2) */  
  TXQ1 (rwx) : ORIGIN = 0x41380488, LENGTH = 0x1000 /* 4K bytes (alias RAM3) */  
  MBOX2 (rwx) : ORIGIN = 0x443c0000, LENGTH = 0x488 /* 1160 bytes (alias RAM4) */  
  TXQ23 (rwx) : ORIGIN = 0x443c0488, LENGTH = 0x1080 /* 4224 bytes (alias RAM5) */  
  TXQ32 (rwx) : ORIGIN = 0x443c1508, LENGTH = 0x1080 /* 4224 bytes (alias RAM6) */  
}

  /* Define a symbol for the top of each memory region */
  __base_QSPI_FLASH = 0x8000000  ; /* QSPI_FLASH */  
  __base_Flash = 0x8000000 ; /* Flash */  
  __top_QSPI_FLASH = 0x8000000 + 0x800000 ; /* 8M bytes */  
  __top_Flash = 0x8000000 + 0x800000 ; /* 8M bytes */  
  __base_SRAM = 0x20000000  ; /* SRAM */  
  __base_RAM = 0x20000000 ; /* RAM */  
  __top_SRAM = 0x20000000 + 0x130000 ; /* 1216K bytes */  
  __top_RAM = 0x20000000 + 0x130000 ; /* 1216K bytes */  
  __base_MBOX1 = 0x41380000  ; /* MBOX1 */  
  __base_RAM2 = 0x41380000 ; /* RAM2 */  
  __top_MBOX1 = 0x41380000 + 0x488 ; /* 1160 bytes */  
  __top_RAM2 = 0x41380000 + 0x488 ; /* 1160 bytes */  
  __base_TXQ1 = 0x41380488  ; /* TXQ1 */  
  __base_RAM3 = 0x41380488 ; /* RAM3 */  
  __top_TXQ1 = 0x41380488 + 0x1000 ; /* 4K bytes */  
  __top_RAM3 = 0x41380488 + 0x1000 ; /* 4K bytes */  
  __base_MBOX2 = 0x443c0000  ; /* MBOX2 */  
  __base_RAM4 = 0x443c0000 ; /* RAM4 */  
  __top_MBOX2 = 0x443c0000 + 0x488 ; /* 1160 bytes */  
  __top_RAM4 = 0x443c0000 + 0x488 ; /* 1160 bytes */  
  __base_TXQ23 = 0x443c0488  ; /* TXQ23 */  
  __base_RAM5 = 0x443c0488 ; /* RAM5 */  
  __top_TXQ23 = 0x443c0488 + 0x1080 ; /* 4224 bytes */  
  __top_RAM5 = 0x443c0488 + 0x1080 ; /* 4224 bytes */  
  __base_TXQ32 = 0x443c1508  ; /* TXQ32 */  
  __base_RAM6 = 0x443c1508 ; /* RAM6 */  
  __top_TXQ32 = 0x443c1508 + 0x1080 ; /* 4224 bytes */  
  __top_RAM6 = 0x443c1508 + 0x1080 ; /* 4224 bytes */  
