CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections \
            -I. -Iinclude -I../../.. \
            -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c startup.c syscalls.c ../../../mongoose.c

TINYUSB_VERSION ?= 0.14.0
TINYUSB_REPO ?= https://github.com/hathach/tinyusb
SOURCES += tinyusb/src/tusb.c \
           tinyusb/src/common/tusb_fifo.c \
           tinyusb/src/device/usbd.c \
           tinyusb/src/device/usbd_control.c \
           tinyusb/src/class/net/ecm_rndis_device.c \
           tinyusb/src/class/net/ncm_device.c \
           tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.c \
           tinyusb/lib/networking/rndis_reports.c \
           usb_descriptors.c
CFLAGS += -Itinyusb/src -Itinyusb/lib/networking
CFLAGS += -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_MIP=1 -DMG_ENABLE_PACKED_FS=1 -DMG_IO_SIZE=512 -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS += -DSTM32F746xx
CFLAGS += -Wno-conversion -Wno-sign-conversion

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -f
endif

all build example: firmware.bin

tinyusb:
	git clone --depth 1 -b $(TINYUSB_VERSION) $(TINYUSB_REPO) $@

$(SOURCES): tinyusb

firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write firmware.bin 0x8000000

clean:
	$(RM) firmware.*
