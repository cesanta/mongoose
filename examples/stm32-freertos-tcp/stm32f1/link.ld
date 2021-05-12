ENTRY(_reset);
MEMORY {
  ram(rwx)  : ORIGIN = 0x20000000, LENGTH = RAM_SIZE
  rom(rx)   : ORIGIN = 0x08000000, LENGTH = ROM_SIZE
}
_estack = 0x20000000 + RAM_SIZE;
SECTIONS {
  .text : { *(.text*) } > rom
  .rodata : { *(.rodata*) } > rom

  .bss : {
    _bss_start = .;
    *(.bss SORT(.bss.*) COMMON)
    _bss_end = .;
  } > ram

  .data : {
    _data_start = .;
    *(.data SORT(.data.*))
    _data_end = .;
  } > ram AT > rom
  _data_flash_start = LOADADDR(.data);
}
