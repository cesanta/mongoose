ENTRY(Reset_Handler);
MEMORY {
  flash(rx) : ORIGIN = 0x08000000, LENGTH = 1024k
  sram(rwx) : ORIGIN = 0x24000000, LENGTH = 128k  /* AXI SRAM in domain D1 */
  /* 2.3.2: remaining SRAM is in other (non-contiguous) banks,
     DTCM @0x20000000 is in domain D1 and not accessible by the ETH DMA controller in domain D2
          @0x24020000 can be either AXI or ITCM (2.4 Table 8)
     SRAM @0x30000000 is in domain D2 and not directly available at startup to be used as stack (8.5.9 page 366)
     SRAM @0x38000000 is in domain D3 and not directly available at startup to be used as stack (8.5.9 page 366) */
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

  .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > sram
  . = ALIGN(8);
  _end = .;
}
