ROOT ?= $(realpath $(PWD)/../../..)
PREFIX ?= riscv-none-elf
#PREFIX ?= docker run --platform linux/amd64 --rm -it -v$(ROOT):$(ROOT) -w $(PWD) mdashnet/riscv riscv64-unknown-elf

CFLAGS = -W -Wall -Wextra -Wno-error -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -march=rv32imafc -mabi=ilp32f
CFLAGS += -DSYSCLK_FREQ_144MHz_HSE -I. -Ivendor -g3 -Os $(CFLAGS_EXTRA)

LDFLAGS = -T link.ld -nostartfiles --specs=nano.specs
LDFLAGS += -lc -lgcc -Wl,--gc-sections #-Wl,-Map=$@.map

SOURCES = main.c mongoose.c net.c packed_fs.c
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
	ls -l firmware.*

firmware.elf: $(SOURCES) link.ld hal.h mongoose_custom.h Makefile
	$(PREFIX)-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	wchisp flash $<
#	@echo; echo "IMPORTANT: configure device to 96k RAM / 224k Flash"

zip: firmware.bin
	zip -r ch32v307.modbus.firmware.`date +%Y-%m-%d`.zip  firmware.*

clean:
	$(RM) firmware.* *.zip
