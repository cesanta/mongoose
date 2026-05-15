# Copyright (c) 2026 Cesanta Software Limited
# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_n6/Include
CFLAGS += -mcpu=cortex-m55 -mthumb -mfloat-abi=soft -mfpu=fpv5-d16 -mcmse
CFLAGS += $(CFLAGS_EXTRA)
LDFLAGS ?= -nostdlib -nostartfiles --specs nosys.specs -lc -lgcc -Wl,--gc-sections

SOURCES = cmsis_n6/Source/Templates/gcc/startup_stm32n657xx.s
SOURCES += main.c hal.c

FSBL_SOURCES = cmsis_n6/Source/Templates/gcc/startup_stm32n657xx_fsbl.s
FSBL_SOURCES += fsbl.c

TOP = ../../../..
CFLAGS += -I$(TOP)
SOURCES += $(TOP)/mongoose.c

all: fsbl.signed.bin firmware.bin

firmware.elf: cmsis_core cmsis_n6 hal.h link.ld Makefile $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -Tlink.ld -o $@

fsbl.elf: cmsis_core cmsis_n6 hal.h fsbl.ld Makefile $(FSBL_SOURCES)
	arm-none-eabi-gcc $(FSBL_SOURCES) $(CFLAGS) $(LDFLAGS) -Tfsbl.ld -o $@

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@
	@echo
	@echo "To flash:"
	@echo "   1. set the BOOT1 jumper to position 1, click reset button"
	@echo "   2. run 'make flash'"
	@echo "   3. set the BOOT1 jumper to position 0, click reset button"
	@echo "   NOTE: STM32_Programmer_CLI and STM32_SigningTool_CLI should be in your PATH"

fsbl.signed.bin: fsbl.bin
	STM32_SigningTool_CLI -s -nk -of 0x80000000 -t fsbl -hv 2.3 -align -bin $^ -o $@

flash: fsbl.signed.bin firmware.bin
	STM32_Programmer_CLI -c port=swd mode=HOTPLUG -el MX25UM51245G_STM32N6570-NUCLEO.stldr -d firmware.bin 0x70100000 -hardRst
	STM32_Programmer_CLI -c port=swd mode=HOTPLUG -el MX25UM51245G_STM32N6570-NUCLEO.stldr -d fsbl.signed.bin 0x70000000 -hardRst

gdb: fsbl.elf
	arm-none-eabi-gdb -q -ex='set confirm off' -ex 'target remote :61234' -ex 'load' $^

# Directory where STM32_Programmer_CLI binary lives
CUBEPROG_DIR ?= $(dir $(shell which STM32_Programmer_CLI))
gdb_server:
	ST-LINK_gdbserver --port-number 61234 --log-level 1 --swd --verify --stm32cubeprogrammer-path $(CUBEPROG_DIR) --apid 1 --initialize-reset

cmsis_core:
	git clone -q -c advice.detachedHead=false --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_n6:
	git clone -q -c advice.detachedHead=false --depth 1 -b v1.3.0 https://github.com/STMicroelectronics/cmsis-device-n6 $@

clean:
	rm -rf *.bin *.map *.elf
