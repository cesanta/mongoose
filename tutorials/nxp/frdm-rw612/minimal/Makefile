# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

MGDIR = ../../../..

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type -Wno-old-style-declaration
CFLAGS += -Wno-unused-parameter
CFLAGS += -I. -I$(MGDIR) -Imongoose -Icmsis_core/CMSIS/Core/Include/
CFLAGS += -Icmsis_rw612/devices/RW612/ -Icmsis_rw612/devices/RW612/periph/ -Icmsis_rw612/devices/RW612/drivers/
CFLAGS += -DCPU_RW612ETA2I -DCPU_RW612ETA2I_cm33_no_dsp
CFLAGS += -DBOOT_HEADER_ENABLE=1
CFLAGS += -mcpu=cortex-m33+nodsp -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c startup.c flexspi.c flash_config.c $(MGDIR)/mongoose.c
SOURCES += cmsis_rw612/devices/RW612/drivers/fsl_clock.c
SOURCES += cmsis_rw612/devices/RW612/drivers/fsl_trng.c
SOURCES += cmsis_rw612/devices/RW612/drivers/fsl_reset.c

all build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_rw612 $(SOURCES) $(wildcard *.h) link.ld Makefile mongoose_config.h
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_rw612:
	curl -sL https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.RW612_DFP.25.12.00.pack -o NXP.RW612_DFP.25.12.00.pack
	unzip -q NXP.RW612_DFP.25.12.00.pack -d $@
	rm NXP.RW612_DFP.25.12.00.pack

flash: firmware.bin
	(echo 'loadfile firmware.elf'; echo 'reset'; echo 'exit';) | JLinkExe -device RW612 -if SWD -speed 4000 -autoconnect 1 -NoGui 1 -ExitOnError 1
#	JRun --nogui --device RW612 --if SWD firmware.elf

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*
