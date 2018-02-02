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

#ifndef CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ESPTOOL2_ELF_H_
#define CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ESPTOOL2_ELF_H_

#include <stdio.h>

#include "elf.h"

typedef struct {
    Elf32_Off    offset;
    Elf32_Addr   address;
    Elf32_Word   size;
	char        *name;
} MyElf_Section;

typedef struct {
    FILE           *fd;
    Elf32_Ehdr      header;
    char           *strings;
	MyElf_Section  *sections;
} MyElf_File;

MyElf_File* LoadElf(char *infile);
void UnloadElf(MyElf_File *e_object);
MyElf_Section* GetElfSection(MyElf_File *e_object, char *name);
unsigned char* GetElfSectionData(MyElf_File *e_object, MyElf_Section *section);

#endif /* CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ESPTOOL2_ELF_H_ */
