CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_f4/Include -Imongoose
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c sysinit.c
SOURCES += cmsis_f4/Source/Templates/gcc/startup_stm32f429xx.s

# TinyUSB
SOURCES += tinyusb/src/tusb.c
SOURCES += tinyusb/src/common/tusb_fifo.c
SOURCES += tinyusb/src/device/usbd.c
SOURCES += tinyusb/src/device/usbd_control.c
SOURCES += tinyusb/src/class/net/ecm_rndis_device.c
SOURCES += tinyusb/src/class/net/ncm_device.c
SOURCES += tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.c
SOURCES += tinyusb/lib/networking/rndis_reports.c
SOURCES += usb_descriptors.c
CFLAGS += -Itinyusb/src -Itinyusb/lib/networking
CFLAGS += -DSTM32F429xx
CFLAGS += -Wno-conversion -Wno-sign-conversion

SOURCES += mongoose/mongoose.c

all build example: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	cp ../../../../mongoose.[ch] mongoose/

firmware.elf: cmsis_core cmsis_f4 tinyusb $(SOURCES) hal.h link.ld Makefile mongoose/mongoose.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash'"

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_f4:
	git clone -q -c advice.detachedHead=false --depth 1 -b v2.6.8 https://github.com/STMicroelectronics/cmsis_device_f4 $@
tinyusb:
	git clone -q -c advice.detachedHead=false --depth 1 -b 0.14.0 https://github.com/hathach/tinyusb $@

clean:
	rm -rf firmware.* cmsis_* tinyusb mongoose/mongoose.*
