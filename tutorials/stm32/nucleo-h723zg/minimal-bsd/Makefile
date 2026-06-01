# Copyright (c) 2026 Cesanta Software Limited
# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_h7/Include -Imongoose
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c
SOURCES += cmsis_h7/Source/Templates/gcc/startup_stm32h723xx.s
SOURCES += mongoose/mongoose.c

# FreeRTOS. H7 has a Cortex-M7 r1p0 core, FreeRTOS recommends using CM4F port for non-r0p1 CM7 micros
SOURCES += FreeRTOS-Kernel/tasks.c FreeRTOS-Kernel/list.c FreeRTOS-Kernel/queue.c
SOURCES += FreeRTOS-Kernel/portable/MemMang/heap_4.c
SOURCES += FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
CFLAGS += -IFreeRTOS-Kernel/include
CFLAGS += -IFreeRTOS-Kernel/portable/GCC/ARM_CM4F -Wno-conversion

all build example: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	cp ../../../../mongoose.[ch] mongoose/

firmware.elf: cmsis_core cmsis_h7 FreeRTOS-Kernel hal.h link.ld Makefile $(SOURCES) mongoose/mongoose.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash, run 'make flash', or use STM32CubeProgrammer"

flash: firmware.bin
	STM32_Programmer_CLI -c port=swd -w firmware.elf -hardRst

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_h7:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.10.6 https://github.com/STMicroelectronics/cmsis_device_h7 $@

FreeRTOS-Kernel:
	git clone --depth 1 -b V11.3.0 https://github.com/FreeRTOS/FreeRTOS-Kernel $@

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*


# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/10
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

test update: CFLAGS_EXTRA = -DUART_DEBUG=USART1
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=15 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
