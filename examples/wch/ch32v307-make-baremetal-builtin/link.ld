ENTRY(_start);
MEMORY {
  flash(rx) : ORIGIN = 0x00000000, LENGTH = 288k
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 32k
}
_estack     = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
  .init    : { _sinit = .; . = ALIGN(4); KEEP(*(SORT_NONE(.init))) . = ALIGN(4); _einit = . ; } > flash
  .vector  : { KEEP(*(.vector)) }  > flash
  .text    : { *(.text .text.* .rodata .rodata.*) }  > flash
  .dalign  : { . = ALIGN(4); PROVIDE(_data_vma = . ); } > sram  AT > flash	
  .dlalign : { . = ALIGN(4); PROVIDE(_data_lma = . ); } > flash AT > flash

  .data : {
    _sdata = .;
    *(.data .data.*) 
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.* .gnu.linkonce.s.*)  /* for weak symbol linking */
    _edata = .;
  } > sram AT > flash

  .bss : { _sbss = .; *(.bss .bss.* COMMON) _ebss = .; } > sram
  . = ALIGN(8);
  _end = .;

  __stack_size = 2048;
  .stack ORIGIN(sram) + LENGTH(sram) - __stack_size : {
    PROVIDE( _heap_end = . );
    . = ALIGN(4);
    PROVIDE(_susrstack = . );
    . = . + __stack_size;
    PROVIDE( _eusrstack = . );
  } > sram
}
