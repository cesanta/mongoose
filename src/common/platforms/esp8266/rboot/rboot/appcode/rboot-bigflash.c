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

typedef unsigned int uint32;
typedef unsigned char uint8;

#include <rboot.h>

#if 0 //def BOOT_BIG_FLASH

// plain sdk defaults to iram
#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void Cache_Read_Disable();
extern uint32 SPIRead(uint32, void*, uint32);
extern void Cache_Read_Enable(uint32, uint32, uint32);

uint8 rBoot_mmap_1 = 0xff;
uint8 rBoot_mmap_2 = 0xff;

// this function must remain in iram
IRAM NOINSTR void __wrap_Cache_Read_Enable_New(void) {
	if (rBoot_mmap_1 == 0xff) {
		uint32 addr;
		rboot_config conf;

		Cache_Read_Disable();

		SPIRead(BOOT_CONFIG_SECTOR * SECTOR_SIZE, &conf, sizeof(rboot_config));

		addr = conf.roms[conf.current_rom];
		addr /= 0x100000;

		rBoot_mmap_2 = addr / 2;
		rBoot_mmap_1 = addr % 2;
	}

	Cache_Read_Enable(rBoot_mmap_1, rBoot_mmap_2, 1);
}

#ifdef __cplusplus
}
#endif

#endif
