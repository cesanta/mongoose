# Copyright (c) 2026 Cesanta Software Limited
# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_g0/Include -Imongoose
CFLAGS += -mcpu=cortex-m0 -mthumb -mfloat-abi=soft $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c
SOURCES += cmsis_g0/Source/Templates/gcc/startup_stm32g031xx.s
SOURCES += mongoose/mongoose.c

CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\"

all build example: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	cp ../../../../mongoose.[ch] mongoose/

firmware.elf: cmsis_core cmsis_g0 hal.h link.ld Makefile $(SOURCES) mongoose/mongoose.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash'"

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_g0:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.4.3 https://github.com/STMicroelectronics/cmsis_device_g0 $@

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/10
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

test update: CFLAGS_EXTRA = -DUART_DEBUG=USART1
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
