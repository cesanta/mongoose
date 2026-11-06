# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

MGDIR = ../../../..
SDK = silabs_sdk/platform
RAIL = $(SDK)/radio/rail_lib

CFLAGS  = -W -Wall -Wextra -Werror -Wshadow  # No -Wundef: RAIL headers
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -I$(MGDIR) -Icmsis_core/CMSIS/Core/Include
CFLAGS += -I$(SDK)/Device/SiliconLabs/EFR32MG24/Include -I$(SDK)/common/inc
CFLAGS += -I$(RAIL)/common -I$(RAIL)/protocol/ieee802154
CFLAGS += -I$(RAIL)/chip/efr32/efr32xg2x
CFLAGS += -DEFR32MG24B220F1536IM48
CFLAGS += -D__START=main  # For startup_efr32mg24.c
CFLAGS += -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mcmse
CFLAGS += $(CFLAGS_EXTRA)

LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lm -lgcc -Wl,--gc-sections

SOURCES = main.c hal.c $(MGDIR)/mongoose.c

SOURCES += $(MGDIR)/tutorials/mqtt/mqtt-client/mongoose_mqtt.c
SOURCES += $(SDK)/Device/SiliconLabs/EFR32MG24/Source/startup_efr32mg24.c
SOURCES += $(SDK)/Device/SiliconLabs/EFR32MG24/Source/system_efr32mg24.c

# RAIL radio library, a binary blob. Simplicity SDK keeps it in Git LFS,
# served by Silicon Labs. The oid is the file's sha256, see the LFS pointer
# platform/radio/rail_lib/autogen/librail_release/librail_efr32xg24_gcc_release.a
SDK_VERSION = v2025.6.3
LIBRAIL = librail_efr32xg24_gcc_release.a
LIBRAIL_SHA256 = 381b123649b66e9e3bbe6d1363c31ee0457b3274207b837ea49685a3a0a2c387
LIBRAIL_URL = https://artifacts.silabs.net/api/lfs/gsdk/object/$(LIBRAIL_SHA256)

all build: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core silabs_sdk $(LIBRAIL) $(SOURCES) $(wildcard *.h) $(MGDIR)/mongoose.h link.ld Makefile mesh-config
	arm-none-eabi-gcc $(SOURCES) $(LIBRAIL) $(CFLAGS) $(LDFLAGS) -o $@

# Rebuild when overriding the board address or timing.
.PHONY: mesh-config
mesh-config:

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

# Device headers, startup code and RAIL headers from Simplicity SDK
silabs_sdk:
	GIT_LFS_SKIP_SMUDGE=1 git clone -q -c advice.detachedHead=false --depth 1 -b $(SDK_VERSION) --filter=blob:none --no-checkout https://github.com/SiliconLabs/simplicity_sdk $@
	cd $@ && git sparse-checkout set --no-cone /platform/Device/SiliconLabs/EFR32MG24/ /platform/common/inc/ /platform/radio/rail_lib/common/ /platform/radio/rail_lib/protocol/ieee802154/ /platform/radio/rail_lib/chip/efr32/efr32xg2x/
	cd $@ && GIT_LFS_SKIP_SMUDGE=1 git checkout -q

$(LIBRAIL):
	curl -sfL $(LIBRAIL_URL) -o $@.tmp
	echo "$(LIBRAIL_SHA256)  $@.tmp" | shasum -a 256 -c -s
	mv $@.tmp $@

# Show all symbols sorted by size
show_symbols_sorted_by_size: firmware.bin
	arm-none-eabi-readelf -sW firmware.elf | sed 1,3d | sed 's,^.*: ,,' | tr -s ' ' | cut -d ' ' -f 2,7 | sort -nk1 | grep -v '^0 '

# Flash over the on-board CMSIS-DAP probe. pyocd needs the EFR32MG24 pack:
#   pyocd pack install efr32mg24b220f1536im48
# With several probes attached, choose one: make flash PYOCD_FLAGS="-u ID"
flash: firmware.bin
	pyocd flash -t efr32mg24b220f1536im48 $(PYOCD_FLAGS) firmware.elf

clean:
	rm -rf firmware.* cmsis_* silabs_* librail_*
