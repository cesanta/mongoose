CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I.
# CMSIS Driver specifics, and its dependencies
CFLAGS += -Icmsis_core/CMSIS/Core/Include                       # CMSIS core headers
CFLAGS += -Icmsis_core/CMSIS/Driver/Include -Icmsis_driver/ETH  # CMSIS Driver core and ETH headers
CFLAGS += -Icmsis_mcu/CMSIS/Driver                              # CMSIS Driver device driver headers
CFLAGS += -D__MEMORY_AT\(x\)=                                   # disable using specific memory address for Eth buffers
CFLAGS += -Icmsis_mcu/Drivers/CMSIS/Device/ST/STM32F7xx/Include -DSTM32F746xx # CMSIS device headers
CFLAGS += -Icmsis_mcu/Drivers/STM32F7xx_HAL_Driver/Inc/         # HAL headers, required by CMSIS Driver device driver (HAL-based)
CFLAGS += -Icmsis_mcu/MDK/Templates/Inc/                        # pull stm32f7xx_hal_conf.h (HAL configuration)
CFLAGS += -DRTE_DEVICE_HAL_ETH -DRTE_DEVICE_HAL_GPIO -DRTE_DEVICE_HAL_RCC # configure it
CFLAGS += -DRTE_DEVICE_HAL_COMMON -DRTE_DEVICE_FRAMEWORK_CUBE_MX

CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = main.c syscalls.c sysinit.c
# CMSIS Driver specifics, and its dependencies
SOURCES += cmsis_driver/ETH/PHY_LAN8742A.c                      # CMSIS Driver for the PHY present in this board
CFLAGS += -Wno-conversion                                       # avoid warnings when building it
SOURCES += cmsis_mcu/CMSIS/Driver/EMAC_STM32F7xx.c              # CMSIS Driver for EMAC peripheral
SOURCES += cmsis_mcu/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c # HAL sources required by the driver
SOURCES += cmsis_mcu/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c 
SOURCES += cmsis_mcu/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/startup_stm32f746xx.s # ST startup file. Compiler-dependent!

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

firmware.elf: cmsis_core cmsis_driver cmsis_mcu $(SOURCES) hal.h link.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:     # ARM CMSIS core headers
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_driver:   # ARM CMSIS Driver code and headers
	git clone --depth 1 -b 2.7.2 https://github.com/ARM-software/CMSIS-Driver $@
cmsis_mcu:      # Keil CMSIS headers and drivers for STM32F7 series (CMSIS-pack)
	curl -sL https://www.keil.com/pack/Keil.STM32F7xx_DFP.2.15.2.pack -o $@.zip
	mkdir $@ && cd $@ && unzip -q ../$@.zip
mbedtls:        # mbedTLS library
	git clone --depth 1 -b v2.28.2 https://github.com/mbed-tls/mbedtls $@

ifeq ($(TLS), mbedtls)
CFLAGS += -DMG_TLS=MG_TLS_MBED -Wno-conversion -Imbedtls/include
CFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\" mbedtls/library/*.c
firmware.elf: mbedtls
endif

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/5
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<

test update: CFLAGS += -DUART_DEBUG=USART1
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
#	grep 'MQTT connected' /tmp/output.txt   # Check for MQTT connection success

clean:
	$(RM) firmware.* *.su cmsis_core cmsis_driver cmsis_mcu* mbedtls
