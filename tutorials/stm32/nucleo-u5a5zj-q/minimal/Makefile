# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_u5/Include -Imongoose
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c wifi.c mongoose/mongoose.c
SOURCES += cmsis_u5/Source/Templates/gcc/startup_stm32u5a5xx.s


all build example: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	cp ../../../../mongoose.[ch] mongoose/

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash', or use STM32CubeProgrammer"

firmware.elf: cmsis_core cmsis_u5 $(SOURCES) mongoose/mongoose.h $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	STM32_Programmer_CLI -c port=swd -e all -w $< 0x8000000 -hardRst

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_u5:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.4.2 https://github.com/STMicroelectronics/cmsis_device_u5 $@

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*
