#!/bin/bash

xtensa-esp108-elf-gcc -Wl,-N,-Ttext,0x40000000 -nostdlib rom.S -o rom.elf && \
  xtensa-esp108-elf-objdump -d rom.elf > ESP31B_ROM.txt
