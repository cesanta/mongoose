CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
						-Wformat-truncation -fno-common -Wconversion \
						-g3 -Os -ffunction-sections -fdata-sections -I. \
						-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = boot.c main.c syscalls.c

# Add Mongoose-specific flags and source files
CFLAGS  += -I../../.. -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_CUSTOM_MILLIS=1 -DMG_ENABLE_MIP=1 -DMG_ENABLE_PACKED_FS=1
SOURCES += ../../../mongoose.c ../../device-dashboard/net.c ../../device-dashboard/packed_fs.c

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: $(SOURCES) mcu.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Note: on "unknown chip id" flash error, wire BOOT0 to VDD and st-flash erase
flash: firmware.bin
	st-flash --reset write $< 0x8000000

# Requires env variable VCON_API_KEY set
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/2

# Upload firmware to a remote test device
update: firmware.bin
	curl --fail -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

# Read serial port on a remote test device for 5 seconds, store in a
# temporary file, and check the output for expected patterns
test:	EXTRA_CFLAGS += -DUART_DEBUG=UART1
test: update
	curl --fail -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'Ethernet: up' /tmp/output.txt     # Check for network init
	grep 'MQTT connected' /tmp/output.txt   # Check for MQTT connection success

# Read serial port on a remote test device for 5 seconds
tx:
	curl -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5

clean:
	@rm -rf firmware.* *.su
