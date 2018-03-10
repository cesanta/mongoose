#
# Makefile for esptool2
# https://github.com/raburton/esp8266
#

CFLAGS = -O2 -Wall
CC = gcc
LD = gcc
BUILD_DIR = ../../build

all: $(BUILD_DIR) $(BUILD_DIR)/esptool2

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/esptool2.o: esptool2.c esptool2.h esptool2_elf.h elf.h
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/esptool2_elf.o: esptool2_elf.c esptool2.h esptool2_elf.h elf.h
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/esptool2: $(BUILD_DIR)/esptool2.o $(BUILD_DIR)/esptool2_elf.o
	@echo "LD $@"
	$(LD) -o $@ $^
