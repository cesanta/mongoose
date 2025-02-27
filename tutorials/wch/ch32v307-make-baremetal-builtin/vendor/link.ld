ENTRY( _start );
__stack_size = 4096;
PROVIDE( _stack_size = __stack_size );

MEMORY {
  /* 
   * Possible configurations: 192/128, 224/96, 256/64, 288/32
   * By default, we're building with 288/32 configuration
   * To build with 256/64, add -Wl,--defsym=RAM_64K=1
   */
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = DEFINED(RAM_64K) ? 256k : 288k
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = DEFINED(RAM_64K) ? 64k : 32k
}

SECTIONS {
  .init : {
    _sinit = .;
    . = ALIGN(4);
    KEEP(*(SORT_NONE(.init)))
    . = ALIGN(4);
    _einit = .;
  } >FLASH AT>FLASH

  .vector : {
    *(.vector);
    . = ALIGN(64);
  } >FLASH AT>FLASH

  .text : {
    . = ALIGN(4);
    *(.text)
      *(.text.*)
      *(.rodata)
      *(.rodata*)
      *(.gnu.linkonce.t.*)
      . = ALIGN(4);
  } >FLASH AT>FLASH 

  .fini : {
    KEEP(*(SORT_NONE(.fini)))
    . = ALIGN(4);
  } >FLASH AT>FLASH

  PROVIDE( _etext = . );
  PROVIDE( _eitcm = . );	

  .preinit_array  : {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH AT>FLASH 

  .init_array     : {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH AT>FLASH 

  .fini_array     : {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH AT>FLASH 

  .ctors          : {
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >FLASH AT>FLASH 

  .dtors          : {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >FLASH AT>FLASH 

  .dalign : {
    . = ALIGN(4);
    PROVIDE(_data_vma = .);
  } >RAM AT>FLASH	

  .dlalign : {
    . = ALIGN(4); 
    PROVIDE(_data_lma = .);
  } >FLASH AT>FLASH

  .data : {
    *(.gnu.linkonce.r.*)
      *(.data .data.*)
      *(.gnu.linkonce.d.*)
      . = ALIGN(8);
      PROVIDE( __global_pointer$ = . + 0x800 );
      *(.sdata .sdata.*)
        *(.sdata2.*)
        *(.gnu.linkonce.s.*)
        . = ALIGN(8);
      *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)
        . = ALIGN(4);
        PROVIDE( _edata = .);
  } >RAM AT>FLASH

  .bss : {
    . = ALIGN(4);
    PROVIDE( _sbss = .);
    *(.sbss*)
      *(.gnu.linkonce.sb.*)
      *(.bss*)
      *(.gnu.linkonce.b.*)		
      *(COMMON*)
      . = ALIGN(4);
      PROVIDE( _ebss = .);
  } >RAM AT>FLASH

  PROVIDE( _end = _ebss);
  PROVIDE( end = . );

  .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size : {
    PROVIDE( _heap_end = . );    
    . = ALIGN(4);
    PROVIDE(_susrstack = . );
    . = . + __stack_size;
    PROVIDE( _eusrstack = .);
  } >RAM 
}
