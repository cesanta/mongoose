// Based on a small portion of ELF.h from LLVM

//===-- llvm/Support/ELF.h - ELF constants and data structures --*- C++ -*-===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License.
//
//===----------------------------------------------------------------------===//
//
// This header contains common, non-processor-specific data structures and
// constants for the ELF file format.
//
// The details of the ELF32 bits in this file are largely based on the Tool
// Interface Standard (TIS) Executable and Linking Format (ELF) Specification
// Version 1.2, May 1995. The ELF64 stuff is based on ELF-64 Object File Format
// Version 1.5, Draft 2, May 1998 as well as OpenBSD header files.
//
//===----------------------------------------------------------------------===//

#ifndef CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ELF_H_
#define CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ELF_H_

typedef uint32_t Elf32_Addr; // Program address
typedef uint32_t Elf32_Off;  // File offset
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;

// e_ident size and indices
enum {
   EI_MAG0       = 0,          // File identification index.
   EI_MAG1       = 1,          // File identification index.
   EI_MAG2       = 2,          // File identification index.
   EI_MAG3       = 3,          // File identification index.
   EI_CLASS      = 4,          // File class.
   EI_DATA       = 5,          // Data encoding.
   EI_VERSION    = 6,          // File version.
   EI_OSABI      = 7,          // OS/ABI identification.
   EI_ABIVERSION = 8,          // ABI version.
   EI_PAD        = 9,          // Start of padding bytes.
   EI_NIDENT     = 16          // Number of bytes in e_ident.
};

typedef struct {
	unsigned char e_ident[EI_NIDENT]; // ELF Identification bytes
	Elf32_Half    e_type;      // Type of file (see ET_* below)
	Elf32_Half    e_machine;   // Required architecture for this file (see EM_*)
	Elf32_Word    e_version;   // Must be equal to 1
	Elf32_Addr    e_entry;     // Address to jump to in order to start program
	Elf32_Off     e_phoff;     // Program header table's file offset, in bytes
	Elf32_Off     e_shoff;     // Section header table's file offset, in bytes
	Elf32_Word    e_flags;     // Processor-specific flags
	Elf32_Half    e_ehsize;    // Size of ELF header, in bytes
	Elf32_Half    e_phentsize; // Size of an entry in the program header table
	Elf32_Half    e_phnum;     // Number of entries in the program header table
	Elf32_Half    e_shentsize; // Size of an entry in the section header table
	Elf32_Half    e_shnum;     // Number of entries in the section header table
	Elf32_Half    e_shstrndx;  // Sect hdr table index of sect name string table
} Elf32_Ehdr;

typedef struct {
	Elf32_Word sh_name;      // Section name (index into string table)
	Elf32_Word sh_type;      // Section type (SHT_*)
	Elf32_Word sh_flags;     // Section flags (SHF_*)
	Elf32_Addr sh_addr;      // Address where section is to be loaded
	Elf32_Off  sh_offset;    // File offset of section data, in bytes
	Elf32_Word sh_size;      // Size of section, in bytes
	Elf32_Word sh_link;      // Section type-specific header table index link
	Elf32_Word sh_info;      // Section type-specific extra information
	Elf32_Word sh_addralign; // Section address alignment
	Elf32_Word sh_entsize;   // Size of records contained within the section
} Elf32_Shdr;

#endif /* CS_COMMON_PLATFORMS_ESP8266_RBOOT_ESPTOOL2_ELF_H_ */
