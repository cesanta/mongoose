ENTRY(Reset_Handler);
MEMORY {
  flash(rx)  : ORIGIN = 0x08000000, LENGTH = 0x4000000
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 0x130000
}
__StackTop = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
  .boot_hdr : ALIGN(4) { FILL(0xFF); . = 0x400; __FLASH_BASE = .; KEEP(*(.flash_conf)) . = 0x1000; } > flash
  .vectors  : { FILL(256) KEEP(*(.isr_vector)) } > flash
  .text     : { *(.text.* *.text.*) } > flash
  .rodata   : { *(.rodata*) }      > flash

  .iram : {
        _siram = .;
         *(.iram)
        _eiram = .;
  } > sram AT > flash
  _siiram = LOADADDR(.iram);

  .data : {
      _sdata = .;
      *(.first_data)
      *(.data SORT(.data.*))
      _edata = .;
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : { _sbss = .; *(.bss SORT(.bss.*) COMMON) _ebss = .; } > sram
  . = ALIGN(8);
  _end = .;

  _image_start = LOADADDR(.text);
  _image_end = LOADADDR(.data) + SIZEOF(.data);
  _image_size = _image_end - _image_start;
}
