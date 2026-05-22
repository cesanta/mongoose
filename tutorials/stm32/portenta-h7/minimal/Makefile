# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_h7/Include -Imongoose
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DCORE_CM7
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c wifi.c hal.c
SOURCES += cmsis_h7/Source/Templates/gcc/startup_stm32h747xx.s
SOURCES += mongoose/mongoose.c


# Wi-Fi chip firmware stuff
CFLAGS += -Imbed/targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD # nvram header includes a text file

all build example: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	cp ../../../../mongoose.[ch] mongoose/

firmware.elf: cmsis_core cmsis_h7 $(SOURCES) mongoose/mongoose.h mbed $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash', or use STM32CubeProgrammer"

# Wi-Fi chip firmware
mbed:
	git clone --depth 1 --no-checkout -b extrapatches-6.17.0 https://github.com/arduino/mbed-os $@ && cd $@ && git sparse-checkout set targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources && git checkout
	touch wiced_resource.h

flash: firmware.bin
	dfu-util -a 0 -s 0x8040000:leave -D $<

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_h7:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.10.4 https://github.com/STMicroelectronics/cmsis_device_h7 $@

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.* mbed wiced_resource.h
