ROOT    ?= $(realpath $(CURDIR)/../../..)
DOCKER  ?= docker run --platform linux/amd64 --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/armgcc
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
            -I. -I../../../ $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c boot.c syscalls.c ../../../mongoose.c

FREERTOS_VERSION ?= V10.5.0
FREERTOS_REPO ?= https://github.com/FreeRTOS/FreeRTOS-Kernel 

build example: firmware.bin

firmware.elf: FreeRTOS-Kernel $(SOURCES) 
	$(DOCKER) arm-none-eabi-gcc -o $@ $(SOURCES) $(CFLAGS)\
	  -IFreeRTOS-Kernel/include \
	  -IFreeRTOS-Kernel/portable/GCC/ARM_CM4F \
	  -Wno-conversion \
	  $(wildcard FreeRTOS-Kernel/*.c) \
	  FreeRTOS-Kernel/portable/MemMang/heap_4.c \
	  FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c \
	  $(LDFLAGS) 
	
firmware.bin: firmware.elf
	$(DOCKER) arm-none-eabi-objcopy -O binary $< $@

FreeRTOS-Kernel:
	git clone --depth 1 -b $(FREERTOS_VERSION) $(FREERTOS_REPO) $@

clean:
	rm -rf firmware.* FreeRTOS-Kernel
