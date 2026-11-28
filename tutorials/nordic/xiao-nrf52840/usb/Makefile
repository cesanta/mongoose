# Seeed XIAO nRF52840: Mongoose over USB CDC-NCM, no TinyUSB.
# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

MGDIR = ../../../..

# Flash over USB: the board's Adafruit bootloader does serial DFU. Double-tap
# RESET to enter it, then `make flash`. PORT: the bootloader's USB serial
# port. adafruit-nrfutil: pip install adafruit-nrfutil
PORT ?= /dev/cu.usbmodem11301
NRFUTIL ?= adafruit-nrfutil

CFLAGS  = -W -Wall -Wextra -Werror -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include -Inrfx/mdk
CFLAGS += -DNRF52840_XXAA -D__STARTUP_CLEAR_BSS -D__START=main -D__HEAP_SIZE=0
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += $(CFLAGS_EXTRA)

LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lm -lgcc
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = hal.c main.c $(MGDIR)/mongoose.c
SOURCES += $(MGDIR)/tutorials/device-dashboard/full/dashboard.c
SOURCES += $(MGDIR)/tutorials/device-dashboard/full/file_data.c
SOURCES += nrfx/mdk/system_nrf52840.c nrfx/mdk/gcc_startup_nrf52840.S

all build: firmware.hex

firmware.elf: cmsis_core nrfx $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@
	arm-none-eabi-size --format=berkeley $@

firmware.hex: firmware.elf
	arm-none-eabi-objcopy -O ihex $< $@

firmware.zip: firmware.hex
	$(NRFUTIL) dfu genpkg --dev-type 0x0052 --sd-req 0xFFFE --application $< $@

flash: firmware.zip
	$(NRFUTIL) dfu serial --package $< -p $(PORT) -b 115200 --singlebank

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

# nRF52840 device headers, SystemInit() and the startup code
nrfx:
	git clone -q -c advice.detachedHead=false --depth 1 -b v2.11.0 https://github.com/NordicSemiconductor/nrfx $@

$(MGDIR)/tutorials/device-dashboard/full/file_data.c:
	cd $(dir $@) && $(MAKE) $(notdir $@)

clean:
	rm -rf firmware.* cmsis_core nrfx

.PHONY: all build flash clean
