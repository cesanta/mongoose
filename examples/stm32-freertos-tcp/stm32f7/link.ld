ENTRY(_reset);
MEMORY {
  ram(rwx)  : ORIGIN = 0x20000000, LENGTH = 320k
  rom(rx)   : ORIGIN = 0x08000000, LENGTH = 1024k
}
_estack = 0x20000000 + 320k;
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
