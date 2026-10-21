# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

MGDIR = ../../../..

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include
CFLAGS += -Icmsis_mcxw71/MCXW/MCXW716C -Icmsis_mcxw71/MCXW/periph2
CFLAGS += -DCPU_MCXW716CMFTA
CFLAGS += -D__START=main -D__STARTUP_CLEAR_BSS  # For startup_MCXW716C.S
CFLAGS += -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)

LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lm -lgcc -Wl,--gc-sections

SOURCES = main.c hal.c $(MGDIR)/mongoose.c
SOURCES += $(MGDIR)/tutorials/mqtt/mqtt-client/mongoose_mqtt.c
SOURCES += cmsis_mcxw71/MCXW/MCXW716C/gcc/startup_MCXW716C.S

all build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_mcxw71 $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

# Device headers and startup code from MCUXpresso SDK, only MCXW716C files
cmsis_mcxw71:
	git clone -q --depth 1 --filter=blob:none --sparse https://github.com/nxp-mcuxpresso/mcux-devices-mcx $@
	cd $@ && git sparse-checkout set MCXW/MCXW716C MCXW/periph2

# Show all symbols sorted by size
show_symbols_sorted_by_size: firmware.bin
	arm-none-eabi-readelf -sW firmware.elf | sed 1,3d | sed 's,^.*: ,,' | tr -s ' ' | cut -d ' ' -f 2,7 | sort -nk1 | grep -v '^0 '

flash: firmware.bin
	LinkServer flash MCXW716C:FRDM-MCXW71 load firmware.elf

clean:
	rm -rf firmware.* cmsis_*
