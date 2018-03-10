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

#include <stdlib.h>
#include <string.h>

#include "esptool2.h"
#include "esptool2_elf.h"

// Find a section in an elf file by name.
// Returns pointer to section if found, else returns zero.
// Does not produce any messages.
MyElf_Section* GetElfSection(MyElf_File *elf, char *name) {

	int i;

    for(i = 0; i < elf->header.e_shnum - 1; i++) {
		if(!strcmp(name, elf->sections[i].name)) {
			debug("Found section '%s'.\r\n", name);
			return &elf->sections[i];
		}
	}

	debug("Could not find section '%s'.\r\n", name);
	return 0;
}

// Reads an elf section (actual data) from the elf file.
// Returns a pointer to newly allocated memory (or zero on error),
// which should be freed by the caller when finished with.
// Produces error message on failure (so caller doesn't need to).
unsigned char* GetElfSectionData(MyElf_File *elf, MyElf_Section *section) {

    unsigned char *data = 0;
    
	if (section->size && section->offset) {

		data = (unsigned char*)malloc(section->size);
		if(!data) {
	        error("Error: Out of memory!\r\n");
			return 0;
		}
            
		if(fseek(elf->fd, section->offset, SEEK_SET) ||
		   fread(data, 1, section->size, elf->fd) != section->size) {
			error("Error: Can't read section '%s' data from elf file.\r\n", section->name);
			free(data);
			return 0;
	    }

	} else {
		error("Error: Section '%s' has no data to read.\r\n", section->name);
	}

    return data;
}

// Opens an elf file and reads the string table and file & section headers.
// Returns a pointer to a MyElf_File structure (or zero on error).
// UnloadElf should be called to dispose of the MyElf_File structure.
// Produces error message on failure (so caller doesn't need to).
MyElf_File* LoadElf(char *infile) {

	int i;
	MyElf_File *elf;
	Elf32_Shdr temp;

	// allocate the elf structure
	elf = (MyElf_File*)malloc(sizeof(MyElf_File));
	if(!elf) {
		error("Error: Out of memory!\r\n");
        goto error_exit;
	}
	memset(elf, 0, sizeof(MyElf_File));

	// open the file
    elf->fd = fopen(infile, "rb");
    if(!elf->fd) {
		error("Error: Can't open elf file '%s'.\r\n", infile);
		goto error_exit;
	}

	// read the header
	if(fread(&elf->header, 1, sizeof(Elf32_Ehdr), elf->fd) != sizeof(Elf32_Ehdr)) {
        error("Error: Can't read elf file header.\r\n");
		goto error_exit;
    }
    
	// check the file header
	if (memcmp(elf->header.e_ident, "\x7f" "ELF", 4)) {
		error("Error: Input files doesn't look like an elf file (bad header).\r\n");
		goto error_exit;
	}
    
	// is there a string table section (we need one)
    if(!elf->header.e_shstrndx) {
		error("Error: Elf file does not contain a string table.\r\n");
		goto error_exit;
	}

	// get the string table section header
    if(fseek(elf->fd, elf->header.e_shoff + (elf->header.e_shentsize * elf->header.e_shstrndx), SEEK_SET) ||
	   fread(&temp, 1, sizeof(Elf32_Shdr), elf->fd) != sizeof(Elf32_Shdr)) {

        error("Error: Can't read string table section from elf file.\r\n");
		goto error_exit;
    }
 
	// read the actual string table
    if(!temp.sh_size) {
		error("Error: Elf file contains an empty string table.\r\n");
		goto error_exit;
	}
    elf->strings = (char*)malloc(temp.sh_size);
    if(!elf->strings) {
        error("Error: Out of memory!\r\n");
		goto error_exit;
    }
    if(fseek(elf->fd, temp.sh_offset, SEEK_SET) ||
	   fread(elf->strings, 1, temp.sh_size, elf->fd) != temp.sh_size) {
        error("Error: Failed to read string stable from elf file.\r\n");
		goto error_exit;
    }

	// read section headers
	elf->sections = (MyElf_Section*)malloc(sizeof(MyElf_Section) * elf->header.e_shnum);
	if(!elf->sections) {
		error("Error: Out of memory!\r\n");
        goto error_exit;
	}
	for(i = 1; i < elf->header.e_shnum; i++) {
        if(fseek(elf->fd, elf->header.e_shoff + (elf->header.e_shentsize * i), SEEK_SET)
		   || fread(&temp, 1, sizeof(Elf32_Shdr), elf->fd) != sizeof(Elf32_Shdr)) {
            error("Error: Can't read section %d from elf file.\r\n", i);
            break;
		}
		debug("Read section %d '%s'.\r\n", i, elf->strings + temp.sh_name);
		elf->sections[i-1].address = temp.sh_addr;
		elf->sections[i-1].offset = temp.sh_offset;
		elf->sections[i-1].size = temp.sh_size;
		elf->sections[i-1].name = elf->strings + temp.sh_name;
	}

    return elf;

error_exit:
	if (elf) {
		if (elf->fd) fclose(elf->fd);
		if (elf->strings) free(elf->strings);
		free(elf);
	}
	return 0;

}

// Close an elf file and dispose of the MyElf_File structure.
void UnloadElf(MyElf_File *elf) {
	if (elf) {
		debug("Unloading elf file.\r\n");
		if(elf->fd) fclose(elf->fd);
		if(elf->strings) free(elf->strings);
		if(elf->sections) free(elf->sections);
		free(elf);
	}
}
