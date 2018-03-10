#ifndef CS_COMMON_PLATFORMS_ESP8266_RBOOT_RBOOT_RBOOT_PRIVATE_H_
#define CS_COMMON_PLATFORMS_ESP8266_RBOOT_RBOOT_RBOOT_PRIVATE_H_

//////////////////////////////////////////////////
// rBoot open source boot loader for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

typedef int int32;
typedef unsigned int uint32;
typedef unsigned char uint8;

#include "rboot.h"

#define NOINLINE __attribute__ ((noinline))

#define ROM_MAGIC	   0xe9
#define ROM_MAGIC_NEW1 0xea
#define ROM_MAGIC_NEW2 0x04

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// buffer size, must be at least 0x10 (size of rom_header_new structure)
#define BUFFER_SIZE 0x100

// esp8266 built in rom functions
extern void ets_printf(const char*, ...);
extern uint32 SPIRead(uint32 addr, void *outptr, uint32 len);
extern uint32 SPIEraseSector(int);
extern uint32 SPIWrite(uint32 addr, void *inptr, uint32 len);

// functions we'll call by address
typedef void stage2a(uint32);
typedef void usercode(void);

// standard rom header
typedef struct {
	// general rom header
	uint8 magic;
	uint8 count;
	uint8 flags1;
	uint8 flags2;
	usercode* entry;
} rom_header;

typedef struct {
	uint8* address;
	uint32 length;
} section_header;

// new rom header (irom section first) there is
// another 8 byte header straight afterward the
// standard header
typedef struct {
	// general rom header
	uint8 magic;
	uint8 count; // second magic for new header
	uint8 flags1;
	uint8 flags2;
	uint32 entry;
	// new type rom, lib header
	uint32 add; // zero
	uint32 len; // length of irom section
} rom_header_new;

#endif /* CS_COMMON_PLATFORMS_ESP8266_RBOOT_RBOOT_RBOOT_PRIVATE_H_ */
