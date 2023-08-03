CFLAGS = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion
CFLAGS += -g -O2 -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include
CFLAGS += -D__SAME54P20A__ -Icmsis_sam/include #-Icmsis_sam/xc32/include
#CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c startup.c
#SOURCES += cmsis_sam/xc32/ATSAME54P20A/startup_atsame54p20a.c

SOURCES += mongoose.c net.c packed_fs.c
CFLAGS += -DMG_ENABLE_TCPIP=1 -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_CUSTOM_MILLIS=1 #-DMG_ENABLE_LINES=1
CFLAGS += -DMG_ENABLE_DRIVER_SAME54=1 -DMG_ENABLE_CUSTOM_RANDOM=1 -DMG_ENABLE_PACKED_FS=1 $(CFLAGS_EXTRA)

VCON_API_KEY=IBrJL5K4arSGMiAXbUKWdG6I2gM

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -rf
endif

build: firmware.bin

firmware.elf: cmsis_core cmsis_sam hal.h link.ld Makefile $(SOURCES) 
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	bossac -p /dev/cu.usb* -w -v -b $<

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_sam:
	curl -sL https://packs.download.microchip.com/Microchip.SAME54_DFP.3.8.234.pack -o $@.zip
	mkdir $@ && cd $@ && unzip ../$@.zip
#	git clone --depth 1 -b master https://github.com/modm-io/cmsis-header-sam $@

clean:
	$(RM) firmware.* cmsis_* *.zip

# Automated test via https://vcon.io/automated-firmware-tests/. Set VCON_API_KEY and update DEVICE_URL
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/9
fota: CFLAGS += -DUART_DEBUG=USART1
fota: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<
test: fota
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	egrep '^tick:.*CPU 180 MHz' /tmp/output.txt
watch: fota
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=999
