# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_mcu/Include
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 $(CFLAGS_EXTRA) -DCORE_CM7
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

# Wi-Fi chip firmware stuff
CFLAGS += -Imbed/targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD # nvram header includes a text file

CFLAGS += -DMG_HAL_DISABLE_ETHERNET

SOURCES = main.c hal.c mongoose.c
SOURCES += cmsis_mcu/Source/Templates/gcc/startup_stm32h747xx.s

all build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_mcu mbed $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	STM32_Programmer_CLI -c port=swd -w $< 0x8040000 -hardRst

cmsis_core:     # ARM CMSIS core headers
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_mcu:      # Keil CMSIS headers and drivers for STM32F7 series (CMSIS-pack)
	git clone --depth 1 -b v1.10.4 https://github.com/STMicroelectronics/cmsis_device_h7 $@

# Wi-Fi chip firmware
mbed:
	git clone --depth 1 --no-checkout -b extrapatches-6.17.0 https://github.com/arduino/mbed-os $@ && cd $@ && git sparse-checkout set targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources && git checkout
	touch wiced_resource.h

clean:
	rm -rf firmware.* cmsis_core cmsis_mcu mbed

