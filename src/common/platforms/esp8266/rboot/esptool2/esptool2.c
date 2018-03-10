/**********************************************************************************
*
*    Copyright (c) 2015 Richard A Burton <richardaburton@gmail.com>
*
*    This file is part of esptool2.
*
*    esptool2 is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    esptool2 is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with esptool2.  If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************************/
/* clang-format off */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "esptool2.h"
#include "esptool2_elf.h"

#define IMAGE_PADDING 16
#define SECTION_PADDING 4
#define IROM_SECTION_PADDING 4096
#define CHECKSUM_INIT 0xEF
#define BIN_MAGIC_IROM 0xEA
#define BIN_MAGIC_FLASH 0xE9

typedef struct {
    Elf32_Addr          addr;
    Elf32_Word          size;
} Section_Header;

typedef struct {
    unsigned char       magic;
    unsigned char       count;
    unsigned char       byte2;
	unsigned char       byte3;
    Elf32_Addr          entry;
} Image_Header;

static const char PADDING[IROM_SECTION_PADDING] = {0};

static bool debugon = false;
static bool quieton = false;

// Print a standard info message (unless quiet mode)
void print(const char* format, ...) {
    va_list args;

	if (!quieton) {
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

// Print a debug message (if debug mode)
void debug(const char* format, ...) {
    va_list args;

	if (debugon) {
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

// Print an error message (always)
void error(const char* format, ...) {
    va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

// Write an elf section (by name) to an existing file.
// Parameters:
//   headed - add a header to the output
//   zeroaddr - force zero entry point in header (default is the real entry point)
//   padded - output will be padded to multiple of SECTION_PADDING bytes
//   chksum - pointer to existing checksum to add this data to (zero if not needed)
// Produces error message on failure (so caller doesn't need to).
bool WriteElfSection(MyElf_File *elf, FILE *outfile, char* name, bool headed,
	bool zeroaddr, int padto, unsigned char *chksum) {

	int i, pad = 0;
	bool ret = false;
	unsigned char *bindata = 0;
	Section_Header sechead;
	MyElf_Section *sect;

	// get elf section header
	sect = GetElfSection(elf, name);
	if(!sect) {
		error("Error: Section '%s' not found in elf file.\r\n", name);
		goto end_function;
	}

	// create image section header
	sechead.addr = (zeroaddr ? 0 : sect->address);
	sechead.size = sect->size;
	
	// do we need to pad the section?
	if (padto) {
		pad = sechead.size % padto;
		if (pad > 0) {
			pad = padto - pad;
			sechead.size += pad;
		}
	}

	debug("Adding section '%s', addr: 0x%08x, size: %d (+%d bytes(s) padding).\r\n",
		name, sect->address, sect->size, pad);
	
	// get elf section binary data
	bindata = GetElfSectionData(elf, sect);
	if (!bindata) {
		goto end_function;
	}
	
	// write section (and pad if required)
	if((headed && fwrite(&sechead, 1, sizeof(sechead), outfile) != sizeof(sechead))
		|| fwrite(bindata, 1, sect->size, outfile) != sect->size
		|| (pad > 0 && fwrite(PADDING, 1, pad, outfile) != pad)) {
		error("Error: Failed to write section '%s' to image file.\r\n", name);
		goto end_function;
	}
	
	// include section data in the checksum
	if(chksum) {
		for(i = 0; i < (int)sect->size; i++) {
			*chksum ^= bindata[i];
		}
	}

	ret = true;

end_function:
	if (bindata) free(bindata);
	return ret;
}

// Load an elf file and export a section of it to a new file, without
// header, padding or checksum. For exporting the .irom0.text library.
// Produces error message on failure (so caller doesn't need to).
bool ExportElfSection(char *infile, char *outfile, char *name) {
	
	bool ret = false;
	FILE *fd = 0;
	MyElf_File *elf = 0;

	// load elf file
	elf = LoadElf(infile);
	if (!elf) {
		goto end_function;
	}

	// open output file
	fd = fopen(outfile, "wb");
    if(!fd) {
		error("Error: Can't open output file '%s' for writing.\r\n", outfile);
        goto end_function;
    }

	// actually do the export
	ret = WriteElfSection(elf, fd, name, false, false, false, 0);
        
end_function:
	// clean up
	if (fd) fclose(fd);
	UnloadElf(elf);
	
	return ret;
}

// Create the main binary firmware image, from specified elf sections.
// Can produce for standard standalone app (separate .irom0.text)
// or sdk bootloaded apps (integrated .irom0.text).
// Choice of type requires appropriately linked elf file.
// Produces error message on failure (so caller doesn't need to).
bool CreateHeaderFile(char *elffile, char *imagefile, char *sections[], int numsec) {

	bool ret = false;
	int i;
	unsigned int j, len;
	FILE *outfile = 0;
	MyElf_File *elf = 0;
	MyElf_Section *sect;
	unsigned char *bindata = 0;
	char name[31];
	
	// load elf file
	elf = LoadElf(elffile);
	if (!elf) {
		goto end_function;
	}
    
	// open output file
	outfile = fopen(imagefile, "wb");
	if(outfile == NULL) {
		error("Error: Failed to open output file '%s' for writing.\r\n", imagefile);
		goto end_function;
	}

	// add entry point
	fprintf(outfile, "const uint32 entry_addr = 0x%08x;\r\n", elf->header.e_entry);

	// add sections
	for (i = 0; i < numsec; i++) {
		// get elf section header
		sect = GetElfSection(elf, sections[i]);
		if(!sect) {
			error("Error: Section '%s' not found in elf file.\r\n", sections[i]);
			goto end_function;
		}

		// simple name fix name
		strncpy(name, sect->name, 31);
		len = strlen(name);
		for (j = 0; j < len; j++) {
			if (name[j] == '.') name[j] = '_';
		}

		// add address, length and start the data block
		debug("Adding section '%s', addr: 0x%08x, size: %d.\r\n", sections[i], sect->address, sect->size);
		fprintf(outfile, "\r\nconst uint32 %s_addr = 0x%08x;\r\nconst uint32 %s_len = %d;\r\nconst uint8  %s_data[] = {",
			name, sect->address, name, sect->size, name);

		// get elf section binary data
		bindata = GetElfSectionData(elf, sect);
		if (!bindata) {
			goto end_function;
		}

		// add the data and finish off the block
		for (j = 0; j < sect->size; j++) {
			if (j % 16 == 0) fprintf(outfile, "\r\n  0x%02x,", bindata[j]);
			else fprintf(outfile, " 0x%02x,", bindata[j]);
		}
		fprintf(outfile, "\r\n};\r\n");
		free(bindata);
		bindata = 0;
	}
	
	// if we got this far everything worked!
	ret = true;

end_function:
	// clean up
	if (outfile) fclose(outfile);
	if (elf) UnloadElf(elf);
	if (bindata) free(bindata);
	
	return ret;
}

// Create the main binary firmware image, from specified elf sections.
// Can produce for standard standalone app (separate .irom0.text)
// or sdk bootloaded apps (integrated .irom0.text).
// Choice of type requires appropriately linked elf file.
// Produces error message on failure (so caller doesn't need to).
bool CreateBinFile(char *elffile, char *imagefile, int bootver, unsigned char mode,
	unsigned char clock, unsigned char size, bool iromchksum, char *sections[], int numsec) {

	bool ret = false;
	int i, pad, len;
	unsigned char chksum = CHECKSUM_INIT;
	unsigned char *data = 0;
	FILE *outfile = 0;
	MyElf_File *elf = 0;
	Image_Header imghead;
	
	// load elf file
	elf = LoadElf(elffile);
	if (!elf) {
		goto end_function;
	}
    
	// open output file
	outfile = fopen(imagefile, "wb");
	if(outfile == NULL) {
		error("Error: Failed to open output file '%s' for writing.\r\n", imagefile);
		goto end_function;
	}

	// set options common to standard and boot v1.2+ headers
	imghead.byte2 = mode;
	//imghead.byte3 = (int)((int)size << 4 | clock) && 0xff;
	imghead.byte3 = ((size << 4) | clock) & 0xff;
	imghead.entry = elf->header.e_entry;
	debug("Size = %02x\r\n", size);
	debug("Byte2 = %02x\r\n", imghead.byte2);
	debug("Byte3 = %02x\r\n", imghead.byte3);
	debug("Entry = %08x\r\n", imghead.entry);

	// boot v1.2+ header
	if (bootver == 2) {
		// extra header
		imghead.magic = BIN_MAGIC_IROM;
		imghead.count = 4; // probably a version number here, not a count
		if(fwrite(&imghead, 1, sizeof(imghead), outfile) != sizeof(imghead)) {
			error("Error: Failed to write header to image file.\r\n");
			goto end_function;
		}
		if(!WriteElfSection(elf, outfile, ".irom0.text", true, true, IROM_SECTION_PADDING, (iromchksum ? &chksum : 0))) {
			goto end_function;
		}
	}

	// standard header
    imghead.magic = BIN_MAGIC_FLASH;
    imghead.count = numsec;
	// write header
	if(fwrite(&imghead, 1, sizeof(imghead), outfile) != sizeof(imghead)) {
		error("Error: Failed to write header to image file.\r\n");
		goto end_function;
	}

	// add sections
	for (i = 0; i < numsec; i++) {
		if(!WriteElfSection(elf, outfile, sections[i], true, false, SECTION_PADDING, &chksum)) {
			goto end_function;
		}
	}
	
	// get image length (plus a byte for the checksum)
	len = ftell(outfile) + 1;

	// do we need to pad the image?
	pad = len % IMAGE_PADDING;
	if (pad > 0) {
		pad = IMAGE_PADDING - pad;
		debug("Padding image with %d byte(s).\r\n", pad);
		if(fwrite(PADDING, 1, pad, outfile) != pad) {
			error("Error: Failed to write padding to image file.\r\n");
			goto end_function;
		}
	}

	// write checksum
	if(fwrite(&chksum, 1, 1, outfile) != 1) {
        error("Error: Failed to write checksum to image file.\r\n");
		goto end_function;
    }

	// boot v1.1
	if(bootver == 1) {
		// write 'ff' padding up to the position of the library
		len = 0x10000 - ftell(outfile);
		debug("Adding boot v1.1 padding, %d bytes of '0xff'.\r\n", len);
		data = (unsigned char*)malloc(len);
		memset(data, 0xff, len);
		if(fwrite(data, 1, len, outfile) != len) {
			error("Error: Failed to write boot v1.1 spacer.\r\n");
			goto end_function;
		}

		// write the library
		if(!WriteElfSection(elf, outfile, ".irom0.text", false, false, 0, 0)) {
			goto end_function;
		}
	}

	// if we got this far everything worked!
	ret = true;

end_function:
	// clean up
	if (outfile) fclose(outfile);
	if (data) free(data);
	if (elf) UnloadElf(elf);
	
	return ret;
}

int main(int argc, char *argv[]) {

	int i;
	char *infile;
	char *outfile;
	int numstr;
	bool binfile = false;
	bool libfile = false;
	bool headerfile = false;
	bool paramerror = false;
	bool iromchksum = false;
	int bootver = 0;
	/* Overwrite-friendly by default */
	unsigned char mode = 0xff;
	unsigned char size = 0xff;
	unsigned char clock = 0;
	int opts = 0;

	// parse options
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-bin")) {
			binfile = true;
			opts++;
		} else if (!strcmp(argv[i], "-lib")) {
			libfile = true;
			opts++;
		} else if (!strcmp(argv[i], "-header")) {
			headerfile = true;
			opts++;
		} else if (!strcmp(argv[i], "-quiet")) {
			quieton = true;
		} else if (!strcmp(argv[i], "-debug")) {
			debugon = true;
		} else if (!strcmp(argv[i], "-boot0")) {
			bootver = 0;
		} else if (!strcmp(argv[i], "-boot1")) {
			bootver = 1;
		} else if (!strcmp(argv[i], "-boot2")) {
			bootver = 2;
		} else if (!strcmp(argv[i], "-qio")) {
			mode = 0;
		} else if (!strcmp(argv[i], "-qout")) {
			mode = 1;
		} else if (!strcmp(argv[i], "-dio")) {
			mode = 2;
		} else if (!strcmp(argv[i], "-dout")) {
			mode = 3;
		} else if (!strcmp(argv[i], "-256")) {
			size = 1;
		} else if (!strcmp(argv[i], "-512")) {
			size = 0;
		} else if (!strcmp(argv[i], "-1024")) {
			size = 2;
		} else if (!strcmp(argv[i], "-2048")) {
			size = 3;
		} else if (!strcmp(argv[i], "-4096")) {
			size = 4;
		} else if (!strcmp(argv[i], "-20")) {
			clock = 2;
		} else if (!strcmp(argv[i], "-26.7")) {
			clock = 1;
		} else if (!strcmp(argv[i], "-40")) {
			clock = 0;
		} else if (!strcmp(argv[i], "-80")) {
			clock = 15;
		} else if (!strcmp(argv[i], "-iromchksum")) {
			iromchksum = true;
		} else if (!strcmp(argv[i], "--")) {
			i++;
			break;
		} else if (argv[i][0] == '-') {
			paramerror = true;
			break;
		} else {
			break;
		}
	}

	print("esptool2 v2.0.0 - (c) 2015 Richard A Burton <richardaburton@gmail.com>\r\n");
	print("This program is licensed under the GPL v3.\r\n");
    print("See the file LICENSE for details.\r\n\r\n");

	if (paramerror) {
		error("Error: Unrecognised option '%s'.\r\n", argv[i]);
		return -1;
	}

	if (argc < 2) {
		print("Usage:\r\n");
		print("esptool2 -lib [options] <input_file> <output_file>\r\n");
		print("esptool2 -bin [options] <input_file> <output_file> <elf_section>...\r\n");
		print("esptool2 -header [options] <input_file> <output_file> <elf_section>...\r\n");
		print("\r\n");
		print("  -lib\r\n");
		print("       Export the sdk library (.irom0.text), for a standalone app.\r\n");
		print("       e.g. esptool2 -elf esp8266_iot.out out.bin\r\n");
		print("\r\n");
		print("  -header\r\n");
		print("       Export elf sections as bytes to a C header file.\r\n");
		print("       e.g. esptool2 -elf esp8266_iot.out out.h .text .data .rodata\r\n");
		print("\r\n");
		print("  -bin\r\n");
		print("       Create binary program image, for standalone and bootloaded apps, with\r\n");
		print("       specified elf sections. Includes sdk library for bootloaded apps.\r\n");
		print("       e.g. esptool2 -bin esp8266_iot.out out.bin .text .data .rodata\r\n");
		print("       Options:\r\n");
		print("        bootloader: -boot0 -boot1 -boot2 (default -boot0)\r\n");
		print("          -boot0 = standalone app, not built for bootloader use\r\n");
		print("          -boot1 = built for bootloader v1.1\r\n");
		print("          -boot2 = built for bootloader v1.2+ (use for rBoot roms)\r\n");
		print("          (elf file must have been linked appropriately for chosen option)\r\n");
		print("        spi size (kb): -256 -512 -1024 -2048 -4096 (default -512)\r\n");
		print("        spi mode: -qio -qout -dio -dout (default -qio)\r\n");
		print("        spi speed: -20 -26.7 -40 -80 (default -40)\r\n");
		print("        include irom in checksum: -iromchksum (also needs enabling in rBoot)\r\n");
		print("\r\n");
		print("General options:\r\n");
		print("  -quiet prints only error messages\r\n");
		print("  -debug print extra debug information\r\n");
		print("  -- no more options follow (needed if your elf file starts with a '-')\r\n");
		print("\r\n");
		print("Returns:\r\n");
		print("   0 on success\r\n");
		print("  -1 on failure\r\n");
		print("\r\n");
		return -1;
	}

	// validate command line options
	if (opts != 1) {
		error("Error: You must specify -bin OR -lib OR -header for build type.\r\n");
		return -1;
	}

	if (quieton && debugon) {
		error("Error: You cannot specify -quiet and -debug.\r\n");
		return -1;
	}

	// check enough parameters
	if ((libfile && i + 2 > argc) || ((binfile | headerfile) && i + 3 > argc)) {
		error("Error: Not enough arguments supplied.\r\n");
		return -1;
	} else if (libfile && i + 2 < argc) {
		error("Error: Too many arguments supplied.\r\n");
		return -1;
	}

	// get parameters
	infile = argv[i++];
	outfile = argv[i++];
	numstr = argc - i;

	// do it
	if (binfile) {
		if (!CreateBinFile(infile, outfile, bootver, mode, clock, size, iromchksum, &argv[i], numstr)) {
			remove(outfile);
			return -1;
		}
	} else if (headerfile) {
		if (!CreateHeaderFile(infile, outfile, &argv[i], numstr)) {
			remove(outfile);
			return -1;
		}
	} else {
		if (!ExportElfSection(infile, outfile, ".irom0.text")) {
			remove(outfile);
			return -1;
		}
	
	}
	
	print("Successfully created '%s'.\r\n", outfile);
	return 0;

}
