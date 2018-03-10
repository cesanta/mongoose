#ifndef CS_COMMON_PLATFORMS_ESP8266_RBOOT_RBOOT_APPCODE_RBOOT_API_H_
#define CS_COMMON_PLATFORMS_ESP8266_RBOOT_RBOOT_APPCODE_RBOOT_API_H_

//////////////////////////////////////////////////
// rBoot OTA and config API for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
// OTA code based on SDK sample from Espressif.
//////////////////////////////////////////////////

#include "../rboot.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32 start_addr;
	uint32 start_sector;
	//uint32 max_sector_count;
	uint32 last_sector_erased;
	uint8 extra_count;
	uint8 extra_bytes[4];
} rboot_write_status;

rboot_config ICACHE_FLASH_ATTR rboot_get_config(void);
bool ICACHE_FLASH_ATTR rboot_set_config(rboot_config *conf);
uint8 ICACHE_FLASH_ATTR rboot_get_current_rom(void);
bool ICACHE_FLASH_ATTR rboot_set_current_rom(uint8 rom);
rboot_write_status ICACHE_FLASH_ATTR rboot_write_init(uint32 start_addr);
bool ICACHE_FLASH_ATTR rboot_write_flash(rboot_write_status *status, uint8 *data, uint16 len);

#ifdef __cplusplus
}
#endif

#endif /* CS_COMMON_PLATFORMS_ESP8266_RBOOT_RBOOT_APPCODE_RBOOT_API_H_ */
