CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include
CFLAGS += -Icmsis_mcu/ra/fsp/src/bsp/cmsis/Device/RENESAS/Include
CFLAGS += -Icmsis_mcu/ra/fsp/src/bsp/mcu/all
#CFLAGS += -Icmsis_mcu/ra/fsp/inc
#CFLAGS += -Icmsis_mcu/ra/fsp/inc/api
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c mongoose.c net.c packed_fs.c
#SOURCES += cmsis_mcu/Files/startup_RA.c
#SOURCES += cmsis_mcu/Files/system_RA.c

# Example specific build options. See README.md
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\" -DHTTPS_URL=\"https://0.0.0.0/\"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: cmsis_core cmsis_mcu $(SOURCES) hal.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@
	arm-none-eabi-size $@

flash: firmware.bin
	rfp-cli -d ra -t jlink -v -p -bin 0x0000000 $<
	rfp-cli -d ra -t jlink -rv 0x8000000 256

cmsis_core:     # ARM CMSIS core headers
	git clone -q --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_mcu:
	git clone -q --depth 1 -b v5.1.0 https://github.com/renesas/fsp $@


# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/13
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

update: CFLAGS += -DUART_DEBUG=LPUART3

test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init

clean:
	$(RM) firmware.* *.su cmsis_core cmsis_mcu mbedtls *.zip
