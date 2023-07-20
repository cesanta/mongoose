CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_f4/Include
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c sysinit.c
SOURCES += cmsis_f4/Source/Templates/gcc/startup_stm32f429xx.s # ST startup file. Compiler-dependent!

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

# Mongoose-specific. See https://mongoose.ws/documentation/#build-options
SOURCES += mongoose.c 
CFLAGS += -DMG_ENABLE_TCPIP=1 -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS += -DMG_ENABLE_CUSTOM_RANDOM=1 -DMG_IO_SIZE=512 -DMG_ENABLE_DRIVER_STM32=1 $(CFLAGS_EXTRA)

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: cmsis_core cmsis_f4 tinyusb $(SOURCES) hal.h link.ld 
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:     # ARM CMSIS core headers
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_f4:       # ST CMSIS headers for STM32F4 series
	git clone --depth 1 -b v2.6.8 https://github.com/STMicroelectronics/cmsis_device_f4 $@
tinyusb:        # TinyUSB sources
	git clone --depth 1 -b 0.14.0 https://github.com/hathach/tinyusb $@

clean:
	$(RM) firmware.* cmsis_core cmsis_f4 tinyusb
