# Environment setup: https://mongoose.ws/documentation/tutorials/tools/
# Nucleo-U5A5ZJ-Q: Mongoose over the USB-C port CN15, OTG_HS with its embedded
# PHY, no TinyUSB. Flash over the ST-LINK USB port

MGDIR = ../../../..

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include -Icmsis_u5/Include
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
CFLAGS += $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections

SOURCES = main.c hal.c $(MGDIR)/mongoose.c
SOURCES += $(MGDIR)/tutorials/device-dashboard/full/dashboard.c
SOURCES += $(MGDIR)/tutorials/device-dashboard/full/file_data.c
SOURCES += cmsis_u5/Source/Templates/gcc/startup_stm32u5a5xx.s

all build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_u5 $(SOURCES) $(wildcard *.h) $(MGDIR)/mongoose.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.elf
	STM32_Programmer_CLI -c port=swd -e all -w $< -hardRst

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_u5:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.4.2 https://github.com/STMicroelectronics/cmsis_device_u5 $@

$(MGDIR)/tutorials/device-dashboard/full/file_data.c:
	cd $(dir $@) && $(MAKE) $(notdir $@)

clean:
	rm -rf firmware.* cmsis_*
