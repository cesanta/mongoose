ENTRY(Reset_Handler)

MEMORY {
  boot_hdr (rx)  : ORIGIN = 0x04000000, LENGTH = 0x0000b000
  flash_irq (rx) : ORIGIN = 0x0400b000, LENGTH = 0x00000400
  flash (rx)     : ORIGIN = 0x0400b400, LENGTH = 0x00ff4c00

  itcm (rwx)     : ORIGIN = 0x0ffe0000, LENGTH = 128k
  dtcm (rwx)     : ORIGIN = 0x20000000, LENGTH = 128k
  ocram1 (rw)    : ORIGIN = 0x204c0000, LENGTH = 256k
}

__StackSize = 4k;
__StackTop = ORIGIN(dtcm) + LENGTH(dtcm);
__StackLimit = __StackTop - __StackSize;

SECTIONS {
  .boot_hdr : {
    FILL(0x00)
    . = 0x0400; KEEP(*(.boot_hdr.conf))
    . = 0x0800; KEEP(*(.boot_hdr.xmcd_data))
    . = 0x1000; __boot_hdr_container__ = ABSOLUTE(.); KEEP(*(.boot_hdr.container))
    . = 0xb000;
  } > boot_hdr

  .isr_vector ALIGN(1024) : { KEEP(*(.isr_vector)) } > flash_irq
  .text : { *(.text .text.*) *(.rodata .rodata.*) *(.constdata .constdata.*) } > flash

  .iram : { _siram = .; *(.iram .iram.*) . = ALIGN(4); _eiram = .; } > itcm AT > flash
  _siiram = LOADADDR(.iram);

  .data : {
    __data_start__ = .;
    *(.data SORT(.data.*)) *(.sdata SORT(.sdata.*))
    . = ALIGN(4);
    __data_end__ = .;
  } > dtcm AT > flash
  __data_flash_start = LOADADDR(.data);

  .bss (NOLOAD) : {
    __bss_start__ = .;
    *(.bss SORT(.bss.*)) *(.sbss SORT(.sbss.*)) *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > dtcm

  _end = ALIGN(8);

  /* Required by startup only; hal.c manages the dynamic heap from _end. */
  __HeapBase = _end;
  __HeapLimit = _end;

  .eth_ram (NOLOAD) : {
    . = ALIGN(64);
    __eth_ram_start__ = .;
    *(.eth_ram .eth_ram.*)
    . = ALIGN(64);
    __eth_ram_end__ = .;
  } > ocram1

  _image_size = LOADADDR(.data) + SIZEOF(.data) - ADDR(.isr_vector);
  _image_loadaddr = ADDR(.isr_vector);
  _image_offset = ADDR(.isr_vector) - __boot_hdr_container__;
}
