//////////////////////////////////////////////////
// rBoot open source boot loader for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////
/* clang-format off */

#ifdef RBOOT_INTEGRATION
#include <rboot-integration.h>
#endif

#include "rboot-private.h"
#include <rboot-hex2a.h>

static uint32 check_image(uint32 readpos) {

	uint8 buffer[BUFFER_SIZE];
	uint8 sectcount;
	uint8 sectcurrent;
	uint8 *writepos;
	uint8 chksum = CHKSUM_INIT;
	uint32 loop;
	uint32 remaining;
	uint32 romaddr;

	rom_header_new *header = (rom_header_new*)buffer;
	section_header *section = (section_header*)buffer;

	if (readpos == 0 || readpos == 0xffffffff) {
		return 0;
	}

	// read rom header
	if (SPIRead(readpos, header, sizeof(rom_header_new)) != 0) {
		return 0;
	}

	// check header type
	if (header->magic == ROM_MAGIC) {
		// old type, no extra header or irom section to skip over
		romaddr = readpos;
		readpos += sizeof(rom_header);
		sectcount = header->count;
	} else if (header->magic == ROM_MAGIC_NEW1 && header->count == ROM_MAGIC_NEW2) {
		// new type, has extra header and irom section first
		romaddr = readpos + header->len + sizeof(rom_header_new);
#ifdef BOOT_IROM_CHKSUM
		// we will set the real section count later, when we read the header
		sectcount = 0xff;
		// just skip the first part of the header
		// rest is processed for the chksum
		readpos += sizeof(rom_header);
#else
		// skip the extra header and irom section
		readpos = romaddr;
		// read the normal header that follows
		if (SPIRead(readpos, header, sizeof(rom_header)) != 0) {
			return 0;
		}
		sectcount = header->count;
		readpos += sizeof(rom_header);
#endif
	} else {
		return 0;
	}

	// test each section
	for (sectcurrent = 0; sectcurrent < sectcount; sectcurrent++) {

		// read section header
		if (SPIRead(readpos, section, sizeof(section_header)) != 0) {
			return 0;
		}
		readpos += sizeof(section_header);

		// get section address and length
		writepos = section->address;
		remaining = section->length;

		while (remaining > 0) {
			// work out how much to read, up to BUFFER_SIZE
			uint32 readlen = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
			// read the block
			if (SPIRead(readpos, buffer, readlen) != 0) {
				return 0;
			}
			// increment next read and write positions
			readpos += readlen;
			writepos += readlen;
			// decrement remaining count
			remaining -= readlen;
			// add to chksum
			for (loop = 0; loop < readlen; loop++) {
				chksum ^= buffer[loop];
			}
		}

#ifdef BOOT_IROM_CHKSUM
		if (sectcount == 0xff) {
			// just processed the irom section, now
			// read the normal header that follows
			if (SPIRead(readpos, header, sizeof(rom_header)) != 0) {
				return 0;
			}
			sectcount = header->count + 1;
			readpos += sizeof(rom_header);
		}
#endif
	}

	// round up to next 16 and get checksum
	readpos = readpos | 0x0f;
	if (SPIRead(readpos, buffer, 1) != 0) {
		return 0;
	}

	// compare calculated and stored checksums
	if (buffer[0] != chksum) {
		return 0;
	}

	return romaddr;
}

#define ETS_UNCACHED_ADDR(addr) (addr)
#define READ_PERI_REG(addr) (*((volatile uint32 *)ETS_UNCACHED_ADDR(addr)))
#define WRITE_PERI_REG(addr, val) (*((volatile uint32 *)ETS_UNCACHED_ADDR(addr))) = (uint32)(val)
#define PERIPHS_RTC_BASEADDR				0x60000700
#define REG_RTC_BASE  PERIPHS_RTC_BASEADDR
#define RTC_GPIO_OUT							(REG_RTC_BASE + 0x068)
#define RTC_GPIO_ENABLE							(REG_RTC_BASE + 0x074)
#define RTC_GPIO_IN_DATA						(REG_RTC_BASE + 0x08C)
#define RTC_GPIO_CONF							(REG_RTC_BASE + 0x090)
#define PAD_XPD_DCDC_CONF						(REG_RTC_BASE + 0x0A0)
static uint32 get_gpio16(void) {
	// set output level to 1
	WRITE_PERI_REG(RTC_GPIO_OUT, (READ_PERI_REG(RTC_GPIO_OUT) & (uint32)0xfffffffe) | (uint32)(1));

	// read level
	WRITE_PERI_REG(PAD_XPD_DCDC_CONF, (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32)0x1);	// mux configuration for XPD_DCDC and rtc_gpio0 connection
	WRITE_PERI_REG(RTC_GPIO_CONF, (READ_PERI_REG(RTC_GPIO_CONF) & (uint32)0xfffffffe) | (uint32)0x0);	//mux configuration for out enable
	WRITE_PERI_REG(RTC_GPIO_ENABLE, READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32)0xfffffffe);	//out disable

	uint32 x = (READ_PERI_REG(RTC_GPIO_IN_DATA) & 1);

	return x;
}

#ifdef BOOT_CONFIG_CHKSUM
// calculate checksum for block of data
// from start up to (but excluding) end
static uint8 calc_chksum(uint8 *start, uint8 *end) {
	uint8 chksum = CHKSUM_INIT;
	while(start < end) {
		chksum ^= *start;
		start++;
	}
	return chksum;
}
#endif

#define UART_CLKDIV_26MHZ(B) (52000000 + B / 2) / B

// prevent this function being placed inline with main
// to keep main's stack size as small as possible
// don't mark as static or it'll be optimised out when
// using the assembler stub
uint32 NOINLINE find_image(void) {

	uint8 flag;
	uint32 runAddr;
	uint32 flashsize;
	int32 romToBoot;
	uint8 gpio_boot = FALSE;
	uint8 updateConfig = FALSE;
	uint8 buffer[SECTOR_SIZE];

	rboot_config *romconf = (rboot_config*)buffer;
	rom_header *header = (rom_header*)buffer;

	// delay to slow boot (help see messages when debugging)
	//ets_delay_us(2000000);

	uart_div_modify(0, UART_CLKDIV_26MHZ(115200));
	ets_delay_us(1000);

	ets_printf("\r\nrBoot v1.2.1 - richardaburton@gmail.com\r\n");

	// read rom header
	SPIRead(0, header, sizeof(rom_header));

	// print and get flash size
	ets_printf("Flash Size:   ");
	flag = header->flags2 >> 4;
	if (flag == 0) {
		ets_printf("4 Mbit\r\n");
		flashsize = 0x80000;
	} else if (flag == 1) {
		ets_printf("2 Mbit\r\n");
		flashsize = 0x40000;
	} else if (flag == 2) {
		ets_printf("8 Mbit\r\n");
		flashsize = 0x100000;
	} else {
#ifdef BOOT_BIG_FLASH
		if (flag == 3) {
			ets_printf("16 Mbit\r\n");
			flashsize = 0x200000;
		} else if (flag == 4) {
			ets_printf("32 Mbit\r\n");
			flashsize = 0x400000;
		} else if (flag == 8) {
			ets_printf("64 Mbit\r\n");
			flashsize = 0x800000;
		} else if (flag == 9) {
			ets_printf("128 Mbit\r\n");
			flashsize = 0x1000000;
		} else {
			ets_printf("unknown\r\n");
			flashsize = 0x100000;  // assume 8Mbit
		}
#else
		ets_printf("8 Mbit\r\n");
		flashsize = 0x100000; // limit to 8Mbit
#endif
	}

	// print spi mode
	ets_printf("Flash Mode:   ");
	if (header->flags1 == 0) {
		ets_printf("QIO\r\n");
	} else if (header->flags1 == 1) {
		ets_printf("QOUT\r\n");
	} else if (header->flags1 == 2) {
		ets_printf("DIO\r\n");
	} else if (header->flags1 == 3) {
		ets_printf("DOUT\r\n");
	} else {
		ets_printf("unknown\r\n");
	}

	// print spi speed
	ets_printf("Flash Speed:  ");
	flag = header->flags2 & 0x0f;
	if (flag == 0) ets_printf("40 MHz\r\n");
	else if (flag == 1) ets_printf("26.7 MHz\r\n");
	else if (flag == 2) ets_printf("20 MHz\r\n");
	else if (flag == 0x0f) ets_printf("80 MHz\r\n");
	else ets_printf("unknown\r\n");

	// print enabled options
#ifdef BOOT_BIG_FLASH
	ets_printf("rBoot Option: Big flash\r\n");
#endif
#ifdef BOOT_CONFIG_CHKSUM
	ets_printf("rBoot Option: Config chksum\r\n");
#endif
#ifdef BOOT_IROM_CHKSUM
	ets_printf("rBoot Option: irom chksum\r\n");
#endif

	ets_printf("\r\n");

	// read boot config
	SPIRead(BOOT_CONFIG_SECTOR * SECTOR_SIZE, buffer, SECTOR_SIZE);
	// fresh install or old version?
	if (romconf->magic != BOOT_CONFIG_MAGIC || romconf->version != BOOT_CONFIG_VERSION
#ifdef BOOT_CONFIG_CHKSUM
		|| romconf->chksum != calc_chksum((uint8*)romconf, (uint8*)&romconf->chksum)
#endif
		) {
		/* Modified by Cesanta */
		ets_printf("Writing default boot config @ 0x%x.\r\n", BOOT_CONFIG_SECTOR * SECTOR_SIZE);
		ets_memset(romconf, 0x00, sizeof(rboot_config));
		romconf->magic = BOOT_CONFIG_MAGIC;
		romconf->version = BOOT_CONFIG_VERSION;
		romconf->count = 2;
		romconf->mode = MODE_STANDARD;
		/* FWx_ADDR, FWx_FS_ADDR and FS_SIZE, FW_SIZE must be defined by -D */
		romconf->roms[0] = FW1_ADDR;
		romconf->roms[1] = FW2_ADDR;
		romconf->fs_addresses[0] = FW1_FS_ADDR;
		romconf->fs_addresses[1] = FW2_FS_ADDR;
		romconf->fs_sizes[0] = romconf->fs_sizes[1] = FS_SIZE;
		romconf->roms_sizes[0] = romconf->roms_sizes[1] = FW_SIZE;
#ifdef BOOT_CONFIG_CHKSUM
		romconf->chksum = calc_chksum((uint8*)romconf, (uint8*)&romconf->chksum);
#endif
		// write new config sector
		SPIEraseSector(BOOT_CONFIG_SECTOR);
		SPIWrite(BOOT_CONFIG_SECTOR * SECTOR_SIZE, buffer, SECTOR_SIZE);
	}

	// if gpio mode enabled check status of the gpio
	if ((romconf->mode & MODE_GPIO_ROM) && (get_gpio16() == 0)) {
		ets_printf("Booting GPIO-selected.\r\n");
		romToBoot = romconf->previous_rom;
		/*
		 * Modified by Cesanta
		 * Make FD current
		 */
		updateConfig = TRUE;
		romconf->fw_updated = 0;
		romconf->is_first_boot = 0;
		gpio_boot = TRUE;
	} else if (romconf->current_rom >= romconf->count) {
		// if invalid rom selected try rom 0
		ets_printf("Invalid rom selected, defaulting.\r\n");
		romToBoot = 0;
		romconf->current_rom = 0;
		romconf->fw_updated = 0;
		romconf->is_first_boot = 0;
		updateConfig = TRUE;
	} else {
		/* Modified by Cesanta */
		if (romconf->is_first_boot != 0) {
				ets_printf("First boot, attempt %d\n", romconf->boot_attempts);
			/* boot is unconfirmed */
			if (romconf->boot_attempts == 0) {
				/* haven't try to load yes */
				ets_printf("Boot is unconfirmed\r\n");
				romconf->boot_attempts++;
			} else {
				ets_printf("Boot failed, fallback to fw #%d\r\n",
											romconf->previous_rom);
				romconf->current_rom = romconf->previous_rom;
				/* clear fw update flag, to avoid post-update acttions */
				romconf->fw_updated = 0;
				romconf->boot_attempts = 0;
			}

			updateConfig = TRUE;
		}
		/* End of Cesanta modifications */
		// try rom selected in the config
		romToBoot = romconf->current_rom;
	}

	// try to find a good rom
	do {
		runAddr = check_image(romconf->roms[romToBoot]);
		if (runAddr == 0) {
			ets_printf("Rom %d is bad.\r\n", romToBoot);
			if (gpio_boot) {
				// don't switch to backup for gpio-selected rom
				ets_printf("GPIO boot failed.\r\n");
				return 0;
			} else {
				// for normal mode try each previous rom
				// until we find a good one or run out
				updateConfig = TRUE;
				romToBoot--;
				if (romToBoot < 0) romToBoot = romconf->count - 1;
				if (romToBoot == romconf->current_rom) {
					// tried them all and all are bad!
					ets_printf("No good rom available.\r\n");
					return 0;
				}
			}
		}
	} while (runAddr == 0);

	// re-write config, if required
	if (updateConfig) {
		romconf->current_rom = romToBoot;
#ifdef BOOT_CONFIG_CHKSUM
		romconf->chksum = calc_chksum((uint8*)romconf, (uint8*)&romconf->chksum);
#endif
		SPIEraseSector(BOOT_CONFIG_SECTOR);
		SPIWrite(BOOT_CONFIG_SECTOR * SECTOR_SIZE, buffer, SECTOR_SIZE);
	}

	ets_printf("Booting rom %d (0x%x).\r\n", romToBoot, romconf->roms[romToBoot]);
	// copy the loader to top of iram
	ets_memcpy((void*)_text_addr, _text_data, _text_len);
	// return address to load from
	return runAddr;

}

#ifdef BOOT_NO_ASM

// small stub method to ensure minimum stack space used
void call_user_start(void) {
	uint32 addr;
	stage2a *loader;

	addr = find_image();
	if (addr != 0) {
		loader = (stage2a*)entry_addr;
		loader(addr);
	}
}

#else

// assembler stub uses no stack space
// works with gcc
void call_user_start(void) {
	__asm volatile (
		"mov a15, a0\n"          // store return addr, hope nobody wanted a15!
		"call0 find_image\n"     // find a good rom to boot
		"mov a0, a15\n"          // restore return addr
		"bnez a2, 1f\n"          // ?success
		"ret\n"                  // no, return
		"1:\n"                   // yes...
		"movi a3, entry_addr\n"  // actually gives us a pointer to entry_addr
		"l32i a3, a3, 0\n"       // now really load entry_addr
		"jx a3\n"                // now jump to it
	);
}

#endif
