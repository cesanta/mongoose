CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_f7/Include
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES += cmsis_f7/Source/Templates/gcc/startup_stm32f746xx.s

SOURCES += main.c hal.c mongoose.c packed_fs.c net.c
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\" -DHTTPS_URL=\"https://0.0.0.0/\"

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_f7 $(SOURCES) hal.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_f7:
	git clone --depth 1 -b v1.2.8 https://github.com/STMicroelectronics/cmsis_device_f7 $@

clean:
	rm -rf firmware.* cmsis_core cmsis_f7
