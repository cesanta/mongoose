TARGET  = firmware
ROOT    ?= $(realpath $(CURDIR)/../../..)
D ?= docker
DOCKER 	?= $(D) run --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/armgcc
CROSS   ?= arm-none-eabi
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
						-Wformat-truncation -fno-common -Wconversion \
						-g3 -Os -ffunction-sections -fdata-sections -I. -I$(ROOT) \
            -DMG_ARCH=MG_ARCH_NEWLIB -DMIP_DEBUG=1 -DMG_ENABLE_PACKED_FS=1 \
						-DMG_ENABLE_CUSTOM_MILLIS=1 -DxMG_ENABLE_LINES=1 -DMG_ENABLE_MIP=1 \
						-mcpu=cortex-m7 -mthumb -mfloat-abi=soft $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = boot.c main.c syscalls.c \
          $(ROOT)/mongoose.c \
          $(ROOT)/examples/device-dashboard/packed_fs.c \
          $(ROOT)/examples/device-dashboard/net.c

all build example: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(DOCKER) $(CROSS)-objcopy -O binary $< $@

$(TARGET).elf: $(SOURCES) mcu.h
	$(DOCKER) $(CROSS)-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Build on Windows. Download Win32 Zip from ARM GCC downloads, unzip to c:\armgcc
ARMGCC ?= c:/armgcc
windows: $(TARGET).bin
windows: CROSS = $(ARMGCC)/bin/arm-none-eabi
windows: DOCKER =

# Show top 10 stack-hungry functions
su: CFLAGS += -fstack-usage
su: $(TARGET).elf
	cat *.su | sort -rnk2 | head -10

# Note: on "unknown chip id" flash error, wire BOOT0 to VDD and st-flash erase
flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x8000000

clean:
	@rm -rf $(TARGET).* *.su
