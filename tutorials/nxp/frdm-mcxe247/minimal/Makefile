# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type -Wno-old-style-declaration
CFLAGS += -Wno-unused-parameter
CFLAGS += -I. -Imongoose -Icmsis_core/CMSIS/Core/Include/
CFLAGS += -Icmsis_mcxe247/devices/MCXE247/ -Icmsis_mcxe247/devices/MCXE247/periph0/ -Icmsis_mcxe247/devices/MCXE247/drivers/
CFLAGS += -DCPU_MCXE247VLQ -DCPU_MCXE247VLQ_cm4
CFLAGS += -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c mongoose/mongoose.c
SOURCES += cmsis_mcxe247/devices/MCXE247/gcc/startup_MCXE247.S
SOURCES += cmsis_mcxe247/devices/MCXE247/drivers/fsl_clock.c

CFLAGS += -D__ATOLLIC__ -D__STARTUP_CLEAR_BSS

all build: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	mkdir -p mongoose
	cp ../../../../mongoose.[ch] mongoose/

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_mcxe247 $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_mcxe247:
	curl -sL https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.MCXE247_DFP.26.06.00.pack -o NXP.MCXE247_DFP.26.06.00.pack
	unzip -q NXP.MCXE247_DFP.26.06.00.pack -d $@
	rm NXP.MCXE247_DFP.26.06.00.pack

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*
