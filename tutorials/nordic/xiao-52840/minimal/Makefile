# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

MGDIR = ../../../..

# Flash over USB: the board's Adafruit UF2 bootloader does serial DFU. The
# firmware resets into it on a 1200 baud touch, see usb.c. PORT: the board's
# USB serial port. adafruit-nrfutil: pip install adafruit-nrfutil
PORT ?= /dev/cu.usbmodem11301
NRFUTIL ?= adafruit-nrfutil

CFLAGS  = -W -Wall -Wextra -Werror -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include
CFLAGS += -Inrfx/mdk -Inrfx -Inrfx/hal -Inrfx/templates -Inrfx/drivers/src -Itinyusb/src
CFLAGS += -Wno-cast-align -Wno-shadow -Wno-unused-parameter
CFLAGS += -DNRF52840_XXAA -D__STARTUP_CLEAR_BSS -D__START=main -D__HEAP_SIZE=0
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += $(CFLAGS_EXTRA)

LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lm -lgcc -Wl,--gc-sections

SOURCES = main.c hal.c usb.c $(MGDIR)/mongoose.c
SOURCES += $(MGDIR)/tutorials/mqtt/mqtt-client/mongoose_mqtt.c
SOURCES += nrfx/mdk/system_nrf52840.c nrfx/mdk/gcc_startup_nrf52840.S

# TinyUSB, for the USB serial console: the board has no USB-UART chip
SOURCES += tinyusb/src/tusb.c tinyusb/src/common/tusb_fifo.c
SOURCES += tinyusb/src/device/usbd.c tinyusb/src/device/usbd_control.c
SOURCES += tinyusb/src/class/cdc/cdc_device.c
SOURCES += tinyusb/src/portable/nordic/nrf5x/dcd_nrf5x.c

all build: firmware.hex

firmware.hex: firmware.elf
	arm-none-eabi-objcopy -O ihex $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core nrfx tinyusb $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.zip: firmware.hex
	$(NRFUTIL) dfu genpkg --dev-type 0x0052 --sd-req 0xFFFE --application $< $@

flash: firmware.zip
	$(NRFUTIL) dfu serial --package $< -p $(PORT) -b 115200 --singlebank --touch 1200

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

# nRF52840 device headers and SystemInit(), nrf_* register helpers for TinyUSB
nrfx:
	git clone -q -c advice.detachedHead=false --depth 1 -b v2.11.0 https://github.com/NordicSemiconductor/nrfx $@

tinyusb:
	git clone -q -c advice.detachedHead=false --depth 1 -b 0.14.0 https://github.com/hathach/tinyusb $@

clean:
	rm -rf firmware.* *.dSYM tinyusb nrfx cmsis_core

.PHONY: all build flash clean
