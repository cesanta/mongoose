# By default, this firmware is built with 288/32 flash/ram device configuration
# To build it with 256/64 configuration, run the following:
# make CFLAGS_EXTRA="-Wl,--defsym -Wl,RAM_64K=1"

ROOT ?= $(realpath $(PWD)/../../..)
PREFIX ?= riscv-none-elf
#PREFIX ?= docker run --platform linux/amd64 --rm -it -v$(ROOT):$(ROOT) -w $(PWD) mdashnet/riscv riscv-none-elf
CFLAGS = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -Wconversion -Wno-sign-conversion
CFLAGS += -ffunction-sections -fdata-sections -fno-common
CFLAGS += -DSYSCLK_FREQ_144MHz_HSE -I. -Ivendor -g3 -Os
CFLAGS += -march=rv32imafc -mabi=ilp32
LDFLAGS = -T vendor/link.ld -nostartfiles --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections
SOURCES = hal.c main.c mongoose.c net.c packed_fs.c
SOURCES += vendor/system_ch32v30x.c vendor/startup_ch32v30x_D8C.S
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\" -DHTTPS_URL=\"https://0.0.0.0/\"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all: firmware.bin

firmware.bin: firmware.elf
	$(PREFIX)-objcopy -O binary $< $@
	$(PREFIX)-size --format=berkeley $<

firmware.elf: $(SOURCES) hal.h mongoose_config.h Makefile
	$(PREFIX)-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) $(CFLAGS_EXTRA) -o $@

flash: firmware.bin
	wchisp flash $<
#	@echo; echo "IMPORTANT: configure device to 96k RAM / 224k Flash"

zip: firmware.bin
	zip -r ch32v307.modbus.firmware.`date +%Y-%m-%d`.zip  firmware.*

clean:
	$(RM) firmware.* *.zip
