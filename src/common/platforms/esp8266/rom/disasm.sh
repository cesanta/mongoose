#!/bin/bash

xtensa-lx106-elf-gcc -Wl,-N,-Ttext,0x40000000 -nostdlib rom.S -o rom.elf && \
  xtensa-lx106-elf-objdump -d rom.elf > ESP8266_ROM.txt
