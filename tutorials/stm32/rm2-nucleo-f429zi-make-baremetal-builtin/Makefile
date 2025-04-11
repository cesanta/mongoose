CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_mcu/Include
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c sysinit.c
SOURCES += cmsis_mcu/Source/Templates/gcc/startup_stm32f429xx.s # ST startup file. Compiler-dependent!

CFLAGS += -Wno-comment

# Mongoose options are defined in mongoose_config.h
SOURCES += mongoose.c 

RM = rm -rf
ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: cmsis_core cmsis_mcu pico-sdk $(SOURCES) hal.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:     # ARM CMSIS core headers
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_mcu:      # Keil CMSIS headers and drivers for STM32F4 series (CMSIS-pack)
	git clone --depth 1 -b v2.6.9 https://github.com/STMicroelectronics/cmsis_device_f4 $@

pico-sdk:
	git clone --depth 1 --no-checkout -b 2.1.0 https://github.com/raspberrypi/pico-sdk $@ && cd $@ && git sparse-checkout set lib/cyw43-driver && git checkout && git submodule update --init lib/cyw43-driver

clean:
	$(RM) firmware.* *.su cmsis_core cmsis_mcu* pico-sdk
