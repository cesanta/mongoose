/* eagle.flash.bin     @ 0x00000 */
/* eagle.irom0text.bin @ 0x10000 */

/*
 * Flash map for 512KB flash.
 *
 * There is 0x8000 (32KB) available between 0x8000 and 0x10000.
 *
 * Note: IROM images starts at 0x10000 (ignore Makefile output which says 0x40000.
 */

MEMORY
{
  dport0_0_seg :                      	org = 0x3FF00000, len = 0x10
  dram0_0_seg :                       	org = 0x3FFE8000, len = 0x18000
  iram1_0_seg :                       	org = 0x40100000, len = 0x8000
  irom0_0_seg :                       	org = 0x40210000, len = 0x6C000
}

INCLUDE "../ld/eagle.app.v6.common.ld"
