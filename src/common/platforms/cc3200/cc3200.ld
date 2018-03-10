/*****************************************************************************
*
* GCC Linker script for CC3200. Based on TI's example "blinky.ld".
*
* Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

ORG = DEFINED(ORG) ? ORG : 0x20004000;
RAM_SIZE = DEFINED(RAM_SIZE) ? RAM_SIZE : 0x3C000;

MEMORY
{
    /* SRAM size of 240KB for cc3200 ES 1.33 device onward */
    SRAM (rwx) : ORIGIN = ORG, LENGTH = RAM_SIZE
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.intvecs))
        *(.text*)
        *(.rodata*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(8);
        _etext = .;
    } > SRAM

    .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
    } > SRAM

    __init_data = .;

    .data : AT(__init_data)
    {
        _data = .;
        *(.data*)
        . = ALIGN (8);
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .heap :
    {
        _heap = .;
        . = . + (LENGTH(SRAM) - SIZEOF(.text) - SIZEOF(.ARM) - SIZEOF(.data) - SIZEOF(.bss));
        . = ALIGN(8);
        _eheap = .;

    } > SRAM
}
