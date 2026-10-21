# Copyright (c) 2026 Cesanta Software Limited
# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

MGDIR = ../../../..

CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include -Icmsis_c5/Include -DSTM32C5A3xx
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections

SOURCES = main.c hal.c
SOURCES += cmsis_c5/Source/startup_stm32c5a3xx.c
SOURCES += $(MGDIR)/mongoose.c

all build example: firmware.bin

firmware.elf: cmsis_core cmsis_c5 hal.h link.ld Makefile $(SOURCES) $(MGDIR)/mongoose.h mongoose_config.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash', or use STM32CubeProgrammer"

firmware.signed.bin: firmware.bin
	test -f private.pem || node $(MGDIR)/resources/sign.js keygen
	node $(MGDIR)/resources/sign.js sign firmware.bin

# Show all symbols sorted by size
show_symbols_sorted_by_size: firmware.bin
	arm-none-eabi-readelf -sW firmware.elf | sed 1,3d | sed 's,^.*: ,,' | tr -s ' ' | cut -d ' ' -f 2,7 | sort -nk1 | grep -v '^0 '

# If the board gets stuck: STM32_Programmer_CLI -c port=swd -e all mode=UR
flash: firmware.bin
	STM32_Programmer_CLI -c port=swd -w firmware.elf mode=UR -rst

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b v6.4.0 https://github.com/ARM-software/CMSIS_6 $@

cmsis_c5:
	git clone -q -c advice.detachedHead=false --depth 1 -b 2.1.0 https://github.com/STMicroelectronics/stm32c5xx-dfp $@

clean:
	rm -rf firmware.* cmsis_*
