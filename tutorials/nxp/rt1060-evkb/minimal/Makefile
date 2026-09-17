# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type
CFLAGS += -I. -Imongoose -Icmsis_core/CMSIS/Core/Include/ -DCPU_MIMXRT1062DVL6B
CFLAGS += -Icmsis_rt1060/devices/MIMXRT1062/ -Icmsis_rt1060/devices/MIMXRT1062/periph/ -Icmsis_rt1060/devices/MIMXRT1062/drivers/
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c flash_image.c mongoose/mongoose.c
SOURCES += cmsis_rt1060/devices/MIMXRT1062/gcc/startup_MIMXRT1062.S

CFLAGS += -D__ATOLLIC__ -D__STARTUP_CLEAR_BSS	# Make startup code work as expected

all build: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	mkdir -p mongoose/
	cp ../../../../mongoose.[ch] mongoose/

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_rt1060 $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_rt1060:
	curl -sL https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.MIMXRT1062_DFP.26.06.00.pack -o NXP.MIMXRT1062_DFP.26.06.00.pack
	unzip -q NXP.MIMXRT1062_DFP.26.06.00.pack -d $@
	rm NXP.MIMXRT1062_DFP.26.06.00.pack

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*
