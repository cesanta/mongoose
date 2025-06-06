/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright 2008-2013 Code Red Technologies Ltd,
 * Copyright 2013-2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 * Generated linker script file for RW612
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v24.12 [Build 148] [2025-01-10] on Jun 6, 2025, 7:53:58 PM
 */

INCLUDE "frdm-rw612-xpresso-freertos-lwip_Debug_library.ld"
INCLUDE "frdm-rw612-xpresso-freertos-lwip_Debug_memory.ld"

ENTRY(ResetISR)

SECTIONS
{
     /* Offset .text by 0x1000 bytes, which whill be added by the image tool*/
    .boot_hdr : ALIGN(4)
    {
        FILL(0xFF)
        . = 0x400;
        __FLASH_BASE = .;
        KEEP(*(.flash_conf))
        . = 0x1000;
    } > QSPI_FLASH

    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_RAM5));
        LONG(    ADDR(.data_RAM5));
        LONG(  SIZEOF(.data_RAM5));
        LONG(LOADADDR(.data_RAM6));
        LONG(    ADDR(.data_RAM6));
        LONG(  SIZEOF(.data_RAM6));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_RAM5));
        LONG(  SIZEOF(.bss_RAM5));
        LONG(    ADDR(.bss_RAM6));
        LONG(  SIZEOF(.bss_RAM6));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

        *(EXCLUDE_FILE(*mflash_drv.o *fsl_flexspi.o) .text*)       KEEP(*freertos*/tasks.o(.rodata*)) /* FreeRTOS Debug Config */
       *(.rodata)
       *(EXCLUDE_FILE(*mflash_drv.o *fsl_flexspi.o).rodata.*)
       *(.constdata .constdata.*)
       . = ALIGN(4);

    } > QSPI_FLASH
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > QSPI_FLASH

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > QSPI_FLASH
 
    _etext = .;
        
    /* DATA section for MBOX1 */

    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        PROVIDE(__start_data_MBOX1 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$MBOX1)
        *(.data.$RAM2)
        *(.data.$MBOX1)
        *(.data.$RAM2.*)
        *(.data.$MBOX1.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
        PROVIDE(__end_data_MBOX1 = .) ;
     } > MBOX1 AT>QSPI_FLASH

    /* DATA section for TXQ1 */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        PROVIDE(__start_data_TXQ1 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$TXQ1)
        *(.data.$RAM3)
        *(.data.$TXQ1)
        *(.data.$RAM3.*)
        *(.data.$TXQ1.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
        PROVIDE(__end_data_TXQ1 = .) ;
     } > TXQ1 AT>QSPI_FLASH

    /* DATA section for MBOX2 */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        PROVIDE(__start_data_MBOX2 = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$MBOX2)
        *(.data.$RAM4)
        *(.data.$MBOX2)
        *(.data.$RAM4.*)
        *(.data.$MBOX2.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
        PROVIDE(__end_data_MBOX2 = .) ;
     } > MBOX2 AT>QSPI_FLASH

    /* DATA section for TXQ23 */

    .data_RAM5 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM5 = .) ;
        PROVIDE(__start_data_TXQ23 = .) ;
        *(.ramfunc.$RAM5)
        *(.ramfunc.$TXQ23)
        *(.data.$RAM5)
        *(.data.$TXQ23)
        *(.data.$RAM5.*)
        *(.data.$TXQ23.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM5 = .) ;
        PROVIDE(__end_data_TXQ23 = .) ;
     } > TXQ23 AT>QSPI_FLASH

    /* DATA section for TXQ32 */

    .data_RAM6 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM6 = .) ;
        PROVIDE(__start_data_TXQ32 = .) ;
        *(.ramfunc.$RAM6)
        *(.ramfunc.$TXQ32)
        *(.data.$RAM6)
        *(.data.$TXQ32)
        *(.data.$RAM6.*)
        *(.data.$TXQ32.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM6 = .) ;
        PROVIDE(__end_data_TXQ32 = .) ;
     } > TXQ32 AT>QSPI_FLASH

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM AT> SRAM

    /* Main DATA section (SRAM) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       PROVIDE(__start_data_RAM = .) ;
       PROVIDE(__start_data_SRAM = .) ;
       *(vtable)
       *(.ramfunc*)
       KEEP(*(CodeQuickAccess))
       KEEP(*(DataQuickAccess))
       *(RamFunction)
       *mflash_drv.o(.text .text* .rodata .rodata*)
       *fsl_flexspi.o(.text .text* .rodata .rodata*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_RAM = .) ;
       PROVIDE(__end_data_SRAM = .) ;
    } > SRAM AT>QSPI_FLASH

    /* BSS section for MBOX1 */
    .bss_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       PROVIDE(__start_bss_MBOX1 = .) ;
       *(.bss.$RAM2)
       *(.bss.$MBOX1)
       *(.bss.$RAM2.*)
       *(.bss.$MBOX1.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
       PROVIDE(__end_bss_MBOX1 = .) ;
    } > MBOX1 AT> MBOX1

    /* BSS section for TXQ1 */
    .bss_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       PROVIDE(__start_bss_TXQ1 = .) ;
       *(.bss.$RAM3)
       *(.bss.$TXQ1)
       *(.bss.$RAM3.*)
       *(.bss.$TXQ1.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
       PROVIDE(__end_bss_TXQ1 = .) ;
    } > TXQ1 AT> TXQ1

    /* BSS section for MBOX2 */
    .bss_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       PROVIDE(__start_bss_MBOX2 = .) ;
       *(.bss.$RAM4)
       *(.bss.$MBOX2)
       *(.bss.$RAM4.*)
       *(.bss.$MBOX2.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
       PROVIDE(__end_bss_MBOX2 = .) ;
    } > MBOX2 AT> MBOX2

    /* BSS section for TXQ23 */
    .bss_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM5 = .) ;
       PROVIDE(__start_bss_TXQ23 = .) ;
       *(.bss.$RAM5)
       *(.bss.$TXQ23)
       *(.bss.$RAM5.*)
       *(.bss.$TXQ23.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM5 = .) ;
       PROVIDE(__end_bss_TXQ23 = .) ;
    } > TXQ23 AT> TXQ23

    /* BSS section for TXQ32 */
    .bss_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM6 = .) ;
       PROVIDE(__start_bss_TXQ32 = .) ;
       *(.bss.$RAM6)
       *(.bss.$TXQ32)
       *(.bss.$RAM6.*)
       *(.bss.$TXQ32.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM6 = .) ;
       PROVIDE(__end_bss_TXQ32 = .) ;
    } > TXQ32 AT> TXQ32

    /* MAIN BSS SECTION */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .) ;
        PROVIDE(__start_bss_SRAM = .) ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end_bss_RAM = .) ;
        PROVIDE(__end_bss_SRAM = .) ;
        PROVIDE(end = .);
    } > SRAM AT> SRAM

    /* NOINIT section for MBOX1 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM2 = .) ;
       PROVIDE(__start_noinit_MBOX1 = .) ;
       *(.noinit.$RAM2)
       *(.noinit.$MBOX1)
       *(.noinit.$RAM2.*)
       *(.noinit.$MBOX1.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM2 = .) ;
       PROVIDE(__end_noinit_MBOX1 = .) ;
    } > MBOX1 AT> MBOX1

    /* NOINIT section for TXQ1 */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM3 = .) ;
       PROVIDE(__start_noinit_TXQ1 = .) ;
       *(.noinit.$RAM3)
       *(.noinit.$TXQ1)
       *(.noinit.$RAM3.*)
       *(.noinit.$TXQ1.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM3 = .) ;
       PROVIDE(__end_noinit_TXQ1 = .) ;
    } > TXQ1 AT> TXQ1

    /* NOINIT section for MBOX2 */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM4 = .) ;
       PROVIDE(__start_noinit_MBOX2 = .) ;
       *(.noinit.$RAM4)
       *(.noinit.$MBOX2)
       *(.noinit.$RAM4.*)
       *(.noinit.$MBOX2.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM4 = .) ;
       PROVIDE(__end_noinit_MBOX2 = .) ;
    } > MBOX2 AT> MBOX2

    /* NOINIT section for TXQ23 */
    .noinit_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM5 = .) ;
       PROVIDE(__start_noinit_TXQ23 = .) ;
       *(.noinit.$RAM5)
       *(.noinit.$TXQ23)
       *(.noinit.$RAM5.*)
       *(.noinit.$TXQ23.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM5 = .) ;
       PROVIDE(__end_noinit_TXQ23 = .) ;
    } > TXQ23 AT> TXQ23

    /* NOINIT section for TXQ32 */
    .noinit_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM6 = .) ;
       PROVIDE(__start_noinit_TXQ32 = .) ;
       *(.noinit.$RAM6)
       *(.noinit.$TXQ32)
       *(.noinit.$RAM6.*)
       *(.noinit.$TXQ32.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM6 = .) ;
       PROVIDE(__end_noinit_TXQ32 = .) ;
    } > TXQ32 AT> TXQ32
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .) ;
        PROVIDE(__start_noinit_SRAM = .) ;
        *(.noinit*)
        . = ALIGN(4) ;
        _end_noinit = .;
        PROVIDE(__end_noinit_RAM = .) ;
        PROVIDE(__end_noinit_SRAM = .) ;
    } > SRAM AT> SRAM

    .smu_cpu13_mbox (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu13_mbox)
        KEEP (*(.smu_cpu13_mbox))
        . = ALIGN(4);
    } > MBOX1 AT> MBOX1

    .smu_cpu31_txq (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu31_txq)
        KEEP (*(.smu_cpu31_txq))
        . = ALIGN(4);
    } > TXQ1 AT> TXQ1

    .smu_cpu23_mbox (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu23_mbox)
        KEEP (*(.smu_cpu23_mbox))
        . = ALIGN(4);
    } > MBOX2 AT> MBOX2

    .smu_cpu32_txq (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu32_txq)
        KEEP (*(.smu_cpu32_txq))
        . = ALIGN(4);
    } > TXQ32 AT> TXQ32

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x15f90;
    .heap (NOLOAD) :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > SRAM

     _StackSize = 0x800;
     /* Reserve space in memory for Stack */
    .heap2stackfill (NOLOAD) :
    {
        . += _StackSize;
    } > SRAM
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM) + LENGTH(SRAM) - _StackSize - 0 (NOLOAD) :  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > SRAM

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
}