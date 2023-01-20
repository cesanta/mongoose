TARGET  = firmware
ROOT    ?= $(realpath $(CURDIR)/../../..)
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
						-Wformat-truncation -fno-common -Wconversion \
						-g3 -Os -ffunction-sections -fdata-sections -I. -I$(ROOT) \
            -DMG_ARCH=MG_ARCH_NEWLIB -DMIP_DEBUG=1 -DMG_ENABLE_PACKED_FS=1 \
						-DMG_ENABLE_CUSTOM_MILLIS=1 -DxMG_ENABLE_LINES=1 -DMG_ENABLE_MIP=1 \
						-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = boot.c main.c syscalls.c \
          $(ROOT)/mongoose.c \
          $(ROOT)/examples/device-dashboard/packed_fs.c \
          $(ROOT)/examples/device-dashboard/net.c

all build example: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

$(TARGET).elf: $(SOURCES) mcu.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Note: on "unknown chip id" flash error, wire BOOT0 to VDD and st-flash erase
flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x8000000

clean:
	@rm -rf $(TARGET).* *.su
