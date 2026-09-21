# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type -Wno-old-style-declaration
CFLAGS += -Wno-unused-parameter
CFLAGS += -I. -Imongoose -Icmsis_core/CMSIS/Core/Include/
CFLAGS += -Icmsis_mcxn947/devices/MCXN947/ -Icmsis_mcxn947/devices/MCXN947/periph/ -Icmsis_mcxn947/devices/MCXN947/drivers/
CFLAGS += -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0
CFLAGS += -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c mongoose/mongoose.c
SOURCES += cmsis_mcxn947/devices/MCXN947/gcc/startup_MCXN947_cm33_core0.S
SOURCES += cmsis_mcxn947/devices/MCXN947/drivers/fsl_clock.c
SOURCES += cmsis_mcxn947/devices/MCXN947/drivers/fsl_spc.c
SOURCES += cmsis_mcxn947/devices/MCXN947/drivers/fsl_common_arm.c

CFLAGS += -D__ATOLLIC__ -D__STARTUP_CLEAR_BSS

all build: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	mkdir -p mongoose
	cp ../../../../mongoose.[ch] mongoose/

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_mcxn947 $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_mcxn947:
	curl -sL https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.MCXN947_DFP.26.06.00.pack -o NXP.MCXN947_DFP.26.06.00.pack
	unzip -q NXP.MCXN947_DFP.26.06.00.pack -d $@
	rm NXP.MCXN947_DFP.26.06.00.pack

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*
