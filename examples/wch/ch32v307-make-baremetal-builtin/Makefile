CFLAGS = -W -Wall -Wextra -Wno-error -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -ffunction-sections -fdata-sections #-fmessage-length=0 -fsigned-char
CFLAGS += -march=rv32imafc -mabi=ilp32f #-msmall-data-limit=8 -mno-save-restore
CFLAGS += -DSYSCLK_FREQ_144MHz_HSE -I. -Ivendor -g3 -Os $(CFLAGS_EXTRA)

LDFLAGS = -T vendor/link.ld -nostartfiles --specs=nano.specs #--specs=nosys.specs
LDFLAGS += -lc -lgcc -Wl,--gc-sections #-Wl,-Map=$@.map

SOURCES = main.c mongoose.c vendor/system_ch32v30x.c vendor/startup_ch32v30x_D8C.S

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
