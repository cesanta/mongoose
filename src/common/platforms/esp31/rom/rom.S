.text
.org 0
.globl _start

// xtensa-esp108-elf-gcc -Wl,-N,-Ttext,0x40000000 -nostdlib rom.S -o rom.elf

here = .
#define PROVIDE(name, addr) name = here + addr - 0x40000000

#include "rom_functions.S"

.text

_start:
.incbin "rom.bin"
_end:
