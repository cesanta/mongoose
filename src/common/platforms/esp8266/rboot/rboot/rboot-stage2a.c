//////////////////////////////////////////////////
// rBoot open source boot loader for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#ifdef RBOOT_INTEGRATION
#include <rboot-integration.h>
#endif

#include "rboot-private.h"

usercode* NOINLINE load_rom(uint32 readpos) {
	
	uint8 buffer[BUFFER_SIZE];
	uint8 sectcount;
	uint8 *writepos;
	uint32 remaining;
	usercode* usercode;
	
	rom_header *header = (rom_header*)buffer;
	section_header *section = (section_header*)buffer;
	
	// read rom header
	SPIRead(readpos, header, sizeof(rom_header));
	readpos += sizeof(rom_header);

	// create function pointer for entry point
	usercode = header->entry;
	
	// copy all the sections
	for (sectcount = header->count; sectcount > 0; sectcount--) {
		
		// read section header
		SPIRead(readpos, section, sizeof(section_header));
		readpos += sizeof(section_header);

		// get section address and length
		writepos = section->address;
		remaining = section->length;
		
		while (remaining > 0) {
			// work out how much to read, up to 16 bytes at a time
			uint32 readlen = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
			// read the block
			SPIRead(readpos, buffer, readlen);
			readpos += readlen;
			// copy the block
			ets_memcpy(writepos, buffer, readlen);
			// increment next write position
			writepos += readlen;
			// decrement remaining count
			remaining -= readlen;
		}
	}

	return usercode;
}

#ifdef BOOT_NO_ASM

void call_user_start(uint32 readpos) {
	usercode* user;
	user = load_rom(readpos);
	user();
}

#else

void call_user_start(uint32 readpos) {
	__asm volatile (
		"mov a15, a0\n"     // store return addr, we already splatted a15!
		"call0 load_rom\n"  // load the rom
		"mov a0, a15\n"     // restore return addr
		"jx a2\n"           // now jump to the rom code
	);
}

#endif
