#!/bin/bash

xtensa-esp32-elf-gcc -Wl,-N,-Ttext,0x40000000 -nostdlib rom.S -o rom.elf && \
  xtensa-esp32-elf-objdump -d rom.elf > ESP32_ROM.txt
