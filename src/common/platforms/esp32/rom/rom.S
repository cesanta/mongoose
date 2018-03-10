.text
.org 0
.globl _start

// xtensa-esp32-elf-gcc -Wl,-N,-Ttext,0x40000000 -nostdlib rom.S -o rom.elf

here = .
#define PROVIDE(name, addr) name = here + addr - 0x40000000

#include "rom_functions.S"

PROVIDE ( _x_unk_40061b88, 0x40061b88 )
PROVIDE ( _x_unk_spi_400622c0, 0x400622c0 )
PROVIDE ( _c_3ff000c8, 0x40062df0 )
PROVIDE ( _c_3ff5b024, 0x40062e0c )
PROVIDE ( _c_3ff5b000, 0x40062e10 )
PROVIDE ( _c_3ff5b020, 0x40062e14 )
PROVIDE ( _c_3ff5b028, 0x40062e18 )
PROVIDE ( _l_40062e90, 0x40062e90 )
PROVIDE ( _l_SPI_Prepare_Encrypt_Data_loop, 0x40062e34 )
PROVIDE ( _l_SPI_Prepare_Encrypt_Data_wait, 0x40062e54 )
PROVIDE ( _l_SPI_Prepare_Encrypt_Data_out, 0x40062e5e )

.text

_start:
.incbin "rom.bin"
_end:
