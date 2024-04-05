CFLAGS  = -W -Wall -Werror -Wextra  -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common # -Wconversion is not SDK-friendly
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_xmc7/devices/COMPONENT_CAT1C/include/
CFLAGS += -Icmsis_xmc7/devices/COMPONENT_CAT1C/include/ip/
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 $(CFLAGS_EXTRA)
LDSCRIPT = link.ld
LDFLAGS ?= -T$(LDSCRIPT) -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map 

SOURCES = main.c hal.c startup.c
CFLAGS += -D__ATOLLIC__ -D__STARTUP_CLEAR_BSS	# Make startup code work as expected

# Mongoose options are defined in mongoose_custom.h
SOURCES += mongoose.c net.c packed_fs.c

# Example specific build options. See README.md
CFLAGS += -DHTTP_URL=\"http://0.0.0.0/\" -DHTTPS_URL=\"https://0.0.0.0/\"

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: cmsis_core cmsis_xmc7 $(SOURCES) hal.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@
	arm-none-eabi-size $@

cmsis_core:     # ARM CMSIS core headers
	git clone -q --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_xmc7:
	git clone https://github.com/Infineon/mtb-pdl-cat1.git $@
	echo "" > cmsis_xmc7/devices/COMPONENT_CAT1C/include/ip/system_cat1c.h

clean:
	$(RM) firmware.* *.su cmsis_core cmsis_xmc7 *.zip
