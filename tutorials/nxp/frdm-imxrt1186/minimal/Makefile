# Environment setup: https://mongoose.ws/documentation/tutorials/tools/

CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wno-conversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -Wno-cast-function-type -Wno-old-style-declaration
CFLAGS += -Wno-unused-parameter -Wno-unused-function
CFLAGS += -I. -Imongoose -Icmsis_core/CMSIS/Core/Include/
CFLAGS += -Icmsis_rt1186/devices/MIMXRT1186/ -Icmsis_rt1186/devices/MIMXRT1186/periph/ -Icmsis_rt1186/devices/MIMXRT1186/drivers/
CFLAGS += -DCPU_MIMXRT1186CVJ8C_cm33 -DCPU_MIMXRT1186CVJ8C -DMIMXRT1186_cm33_SERIES
CFLAGS += -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1
CFLAGS += -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c hal.c flash_image.c mongoose/mongoose.c
SOURCES += cmsis_rt1186/devices/MIMXRT1186/gcc/startup_MIMXRT1186_cm33.S
SOURCES += cmsis_rt1186/devices/MIMXRT1186/drivers/fsl_clock.c
SOURCES += cmsis_rt1186/devices/MIMXRT1186/drivers/fsl_pmu.c
SOURCES += cmsis_rt1186/devices/MIMXRT1186/drivers/fsl_common_arm.c
SOURCES += cmsis_rt1186/devices/MIMXRT1186/drivers/fsl_ele_base_api.c
SOURCES += cmsis_rt1186/devices/MIMXRT1186/drivers/fsl_s3mu.c
SOURCES += cmsis_rt1186/devices/MIMXRT1186/drivers/fsl_trdc.c

CFLAGS += -D__ATOLLIC__ -D__STARTUP_CLEAR_BSS -DMG_HAL_DISABLE_RANDOM

all build: firmware.bin

mongoose/mongoose.c mongoose/mongoose.h:
	mkdir -p mongoose
	cp ../../../../mongoose.[ch] mongoose/

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-size --format=berkeley $<

firmware.elf: cmsis_core cmsis_rt1186 $(SOURCES) $(wildcard *.h) link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_rt1186:
	curl -sL https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.MIMXRT1186_DFP.26.06.00.pack -o NXP.MIMXRT1186_DFP.26.06.00.pack
	unzip -q NXP.MIMXRT1186_DFP.26.06.00.pack -d $@
	rm NXP.MIMXRT1186_DFP.26.06.00.pack

clean:
	rm -rf firmware.* cmsis_* mongoose/mongoose.*
