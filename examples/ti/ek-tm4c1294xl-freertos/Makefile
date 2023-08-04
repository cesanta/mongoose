CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_tm4c/Device/TI/TM4C/Include
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c sysinit.c
SOURCES += startup.c # startup file. Compiler-dependent!

# FreeRTOS
SOURCES += FreeRTOS-Kernel/portable/MemMang/heap_4.c
SOURCES += FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
CFLAGS += -IFreeRTOS-Kernel/include 
CFLAGS += -IFreeRTOS-Kernel/portable/GCC/ARM_CM4F -Wno-conversion

SOURCES += mongoose.c  net.c packed_fs.c
CFLAGS += $(CFLAGS_EXTRA)       # Mongoose options are defined in mongoose_custom.h

# Example specific build options. See README.md
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: FreeRTOS-Kernel cmsis_core cmsis_tm4c $(SOURCES) hal.h link.ld
	arm-none-eabi-gcc $(SOURCES) $(wildcard FreeRTOS-Kernel/*.c) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	@echo "This requires Uniflash"

cmsis_core:     # ARM CMSIS core headers
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_tm4c:     # CMSIS headers for TM4C series
	git clone --depth 1 --no-checkout https://github.com/speters/CMSIS $@ && cd $@ && git sparse-checkout set Device/TI/TM4C && git checkout
FreeRTOS-Kernel:  # FreeRTOS sources
	git clone --depth 1 -b V10.5.0 https://github.com/FreeRTOS/FreeRTOS-Kernel $@

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/1
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

test update: CFLAGS += -DUART_DEBUG=UART0
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
#	grep 'MQTT connected' /tmp/output.txt   # Check for MQTT connection success

clean:
	$(RM) firmware.* *.su cmsis_core cmsis_tm4c FreeRTOS-Kernel
