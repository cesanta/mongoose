CFLAGS = -W -Wall -Wextra -Wno-error -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -march=rv32imafc -mabi=ilp32f
CFLAGS += -DSYSCLK_FREQ_144MHz_HSE -I. -Ivendor -g3 -Os $(CFLAGS_EXTRA)

LDFLAGS = -T vendor/link.ld -nostartfiles --specs=nano.specs
LDFLAGS += -lc -lgcc -Wl,--gc-sections

SOURCES = main.c mongoose.c net.c packed_fs.c vendor/system_ch32v30x.c vendor/startup_ch32v30x_D8C.S
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\" -DHTTPS_URL=\"https://0.0.0.0/\"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all: firmware.bin

firmware.bin: firmware.elf
	riscv64-unknown-elf-objcopy -O binary $< $@
	ls -l firmware.*

firmware.elf: $(SOURCES) hal.h Makefile
	riscv64-unknown-elf-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	wchisp flash $<

clean:
	$(RM) firmware.* *.su
