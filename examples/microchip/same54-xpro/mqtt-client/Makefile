CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include
CFLAGS += -D__SAME54P20A__ -Icmsis_sam/include
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c startup.c

# Mongoose-specific source code files and build options. See https://mongoose.ws/documentation/#build-options
SOURCES += mongoose.c
CFLAGS += -DMG_ENABLE_TCPIP=1 -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_CUSTOM_MILLIS=1 -DMG_ENABLE_LINES=1
CFLAGS += -DMG_ENABLE_DRIVER_SAME54=1 -DMG_ENABLE_CUSTOM_RANDOM=1 -DMG_ENABLE_PACKED_FS=1 $(CFLAGS_EXTRA)

# Example specific build options. See README.md
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -rf
endif

build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: cmsis_core cmsis_sam hal.h link.ld Makefile $(SOURCES) 
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

flash: firmware.bin
	bossac -p /dev/cu.usb* -w -v -b $<

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_sam:
	curl -sL https://packs.download.microchip.com/Microchip.SAME54_DFP.3.8.234.pack -o $@.zip
	mkdir $@ && cd $@ && unzip ../$@.zip

# Automated test via https://vcon.io/automated-firmware-tests/. Set VCON_API_KEY and update DEVICE_URL
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/9
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

test update: CFLAGS += -DUART_DEBUG=USART1
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
	grep 'RECEIVED hello' /tmp/output.txt   # Check for MQTT success

clean:
	$(RM) firmware.* cmsis_core cmsis_sam *.zip
