# Copyright (c) 2026 Cesanta Software Limited
# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_h5/Include
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c
SOURCES += cmsis_h5/Source/Templates/gcc/startup_stm32h563xx.s

TOP = ../../../..
CFLAGS += -I$(TOP)
SOURCES += $(TOP)/mongoose.c

all: firmware.bin

firmware.elf: cmsis_core cmsis_h5 hal.h link.ld Makefile $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash', or use STM32CubeProgrammer"

flash: firmware.bin
	STM32_Programmer_CLI -c port=swd mode=UR -w firmware.elf -hardRst

gdb: firmware.elf
	arm-none-eabi-gdb -q $^ -ex='set confirm off' -ex 'target remote :61234'

# Directory where STM32_Programmer_CLI binary lives
CUBEPROG_DIR ?= $(dir $(shell which STM32_Programmer_CLI))
gdb_server:
	ST-LINK_gdbserver --port-number 61234 --log-level 1 --swd --verify --stm32cubeprogrammer-path $(CUBEPROG_DIR) --apid 1 --initialize-reset


cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_h5:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.6.0 https://github.com/STMicroelectronics/cmsis-device-h5 $@

clean:
	rm -rf firmware.*
