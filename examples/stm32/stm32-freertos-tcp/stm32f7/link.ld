ENTRY(_reset);
MEMORY {
  ram(rwx)  : ORIGIN = 0x20000000, LENGTH = 307k
  rom(rx)   : ORIGIN = 0x08000000, LENGTH = 1024k
}
_estack     = ORIGIN(ram) + LENGTH(ram);    /* boot.s and init_heap() */
SECTIONS {
  .vectortab  : { KEEP(*(.vectortab)) } > rom
  .text       : { *(.text*) } > rom
  .rodata     : { *(.rodata*) } > rom

  .data : {
    _sdata = .;   /* for init_ram() */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;  /* for init_ram() */
  } > ram AT > rom
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;              /* for init_ram() */
    __bss_start__ = _sbss;  /* for libc */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;              /* for init_ram() */
    __bss_end__ = _ebss;    /* for libc */
  } > ram

  . = ALIGN(8);
  end = .;      /* for syscalls.c */
  _end = .;     /* for cmsis_gcc.h and init_ram() */
  __end__ = .;  /* for libc */
}
