/* 
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 * GCC linker script for stm32f429. Based on startup sctipt 
 * (startup_stm32f429xx.s -  COPYRIGHT 2015 STMicroelectronics)
 */
ENTRY(Reset_Handler)

_estack = 0x2002FFFF; /* required by startup sctipt */

MEMORY
{
  FLASH (rx)     : ORIGIN = 0x8000000, LENGTH = 1024K
  RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 192K
  CCMRAM (rw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    *(.isr_vector)
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)           
    *(.text*)                  
    *(.glue_7t)    
    *(.eh_frame)
    *(.init)
    *(.fini)
    . = ALIGN(4);
    _etext = .;        
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         
    *(.rodata*)        
    . = ALIGN(4);
  } >FLASH

  _sidata = LOADADDR(.data);  /* required by startup sctipt */

  .data : 
  {
    . = ALIGN(4);
    _sdata = .;       
    *(.data)           
    *(.data*)          
    . = ALIGN(4);
    _edata = .;        
  } >RAM AT> FLASH

  _siccmram = LOADADDR(.ccmram); /* required by startup sctipt */

  .ccmram :
  {
    . = ALIGN(4);
    _sccmram = .;       
    *(.ccmram)
    *(.ccmram*)
    
    . = ALIGN(4);
    _eccmram = .;       
  } >CCMRAM AT> FLASH

  . = ALIGN(4);
  .bss :
  {
    _sbss = .;    
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .; 
    __bss_end__ = _ebss;
  } >RAM

  ._user_heap_stack :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    . = ALIGN(4);
  } >RAM
}
