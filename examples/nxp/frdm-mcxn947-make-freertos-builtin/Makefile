CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common # -Wconversion is not SDK-friendly
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_mcu/devices/MCXN947/
CFLAGS += -Icmsis_mcu/devices/MCXN947/drivers
CFLAGS += -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0
CFLAGS += -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)
CFLAGS += -Wno-old-style-declaration -Wno-unused-parameter # due to NXP FSL code

LDSCRIPT = link.ld
LDFLAGS ?= -T$(LDSCRIPT) -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map 

SOURCES = main.c hal.c
SOURCES += startup.c
SOURCES += cmsis_mcu/devices/MCXN947/drivers/fsl_clock.c cmsis_mcu/devices/MCXN947/drivers/fsl_spc.c cmsis_mcu/devices/MCXN947/drivers/fsl_common_arm.c # NXP support files

# FreeRTOS. MCXN947 has a Cortex-M33 (ARMv8) core, the CM4F port can be used if TrustZone and the MPU are not to be used
SOURCES += FreeRTOS-Kernel/portable/MemMang/heap_4.c
SOURCES += FreeRTOS-Kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
SOURCES += FreeRTOS-Kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
CFLAGS += -IFreeRTOS-Kernel/include 
CFLAGS += -IFreeRTOS-Kernel/portable/GCC/ARM_CM33_NTZ/non_secure -Wno-conversion -Wno-unused-parameter

# Mongoose options are defined in mongoose_config.h
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

firmware.elf: FreeRTOS-Kernel cmsis_core cmsis_mcu $(SOURCES) hal.h link.ld mongoose_config.h FreeRTOSConfig.h Makefile
	arm-none-eabi-gcc $(SOURCES) $(wildcard FreeRTOS-Kernel/*.c) $(CFLAGS) $(LDFLAGS) -o $@
	arm-none-eabi-size $@

cmsis_core:     # ARM CMSIS core headers
	git clone -q --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_mcu:
	wget -O $@.zip https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.MCXN947_DFP.17.0.0.pack
	mkdir $@ && cd $@ && unzip -q ../$@.zip
FreeRTOS-Kernel:  # FreeRTOS sources
	git clone --depth 1 -b V10.5.0 https://github.com/FreeRTOS/FreeRTOS-Kernel $@
clean:
	$(RM) firmware.* *.su cmsis_core cmsis_mcu *.zip FreeRTOS-Kernel

