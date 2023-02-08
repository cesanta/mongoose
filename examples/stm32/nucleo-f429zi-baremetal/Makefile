# Download CMSIS header files from Github on demand
CMSIS_CORE_VERSION ?= 5.9.0      # ARM Cortex-M definitions
CMSIS_CORE_REPO ?= https://github.com/ARM-software/CMSIS_5
CMSIS_DEVICE_VERSION ?= v2.6.8   # ST MCU peripheral definitions
CMSIS_DEVICE_REPO ?= https://github.com/STMicroelectronics/cmsis_device_f4

CFLAGS ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
          -Wformat-truncation -fno-common -Wconversion \
          -g3 -Os -ffunction-sections -fdata-sections \
          -I . -I cmsis_core/CMSIS/Core/Include -I cmsis_device_f4/Include \
          -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c syscalls.c cmsis_device_f4/Source/Templates/gcc/startup_stm32f429xx.s

# Mongoose-specific build flags and source code files
# Build options reference: https://mongoose.ws/documentation/#build-options
CFLAGS  += -I../../.. -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_CUSTOM_MILLIS=1 -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_PACKED_FS=1
SOURCES += ../../../mongoose.c ../../device-dashboard/net.c ../../device-dashboard/packed_fs.c

# Build flashable .bin file
all build example: firmware.bin

# .bin file is made from .elf file, by concatenating .text and .data sections
firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

# .elf file is produced by compiling sources
firmware.elf: $(SOURCES) hal.h link.ld cmsis_core cmsis_device_f4
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Flash .bin file to the target board via the built-in debugger
flash: firmware.bin
	st-flash --reset write $< 0x8000000

# Download ST's CMSIS headers with peripheral definitions
cmsis_device_f4/Source/Templates/gcc/startup_stm32f429xx.s: cmsis_device_f4
cmsis_device_f4:
	git clone --depth 1 -b $(CMSIS_DEVICE_VERSION) $(CMSIS_DEVICE_REPO) $@

# Download ARM's CMSIS headers with core Cortex-M definitions
cmsis_core:
	git clone --depth 1 -b $(CMSIS_CORE_VERSION) $(CMSIS_CORE_REPO) $@

# Requires env variable VCON_API_KEY set
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/2

# Upload firmware to a remote test device
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

# Read serial port on a remote test device for 5 seconds, store in a
# temporary file, and check the output for expected patterns
test:	EXTRA_CFLAGS += -DUART_DEBUG=UART1
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'Ethernet: up' /tmp/output.txt     # Check for network init
	grep 'MQTT connected' /tmp/output.txt   # Check for MQTT connection success

clean:
	@rm -rf firmware.* *.su cmsis_core cmsis_device_f4
