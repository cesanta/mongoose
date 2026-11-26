# Environment setup: https://mongoose.ws/documentation/tutorials/tools/
# Portenta H7: Mongoose over the USB-C port, no TinyUSB. Flash over USB with
# dfu-util: Arduino's bootloader stays at 0x08000000, the firmware goes to
# 0x08040000. Double-tap RESET to enter the bootloader, then `make flash`

MGDIR = ../../../..

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include -Icmsis_h7/Include
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DCORE_CM7
CFLAGS += $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections

SOURCES = main.c hal.c $(MGDIR)/mongoose.c
SOURCES += cmsis_h7/Source/Templates/gcc/startup_stm32h747xx.s

all build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_h7 $(SOURCES) $(wildcard *.h) $(MGDIR)/mongoose.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	dfu-util -w -a 0 -s 0x8040000:leave -D $<

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_h7:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.10.4 https://github.com/STMicroelectronics/cmsis_device_h7 $@

clean:
	rm -rf firmware.* cmsis_*
