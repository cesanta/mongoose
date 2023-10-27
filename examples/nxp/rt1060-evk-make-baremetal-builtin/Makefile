CFLAGS  = -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
CFLAGS += -Wformat-truncation -fno-common -Wconversion -Wno-sign-conversion
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -Icmsis_core/CMSIS/Core/Include -Icmsis_mcu/devices/MIMXRT1062 #-DCPU_MIMXRT1062DVL6B
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16
LDFLAGS ?= -Tlink_ram.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map 

SOURCES = main.c syscalls.c sysinit.c
SOURCES += cmsis_mcu/devices/MIMXRT1062/gcc/startup_MIMXRT1062.S # NXP startup file. Compiler-dependent!
CFLAGS += -D__ATOLLIC__ -D__STARTUP_CLEAR_BSS	# Make startup code work as expected

# Mongoose-specific. See https://mongoose.ws/documentation/#build-options
SOURCES += mongoose.c net.c packed_fs.c
CFLAGS += -DMG_ENABLE_TCPIP=1 -DMG_ARCH=MG_ARCH_NEWLIB -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS += -DMG_ENABLE_CUSTOM_RANDOM=1 -DMG_ENABLE_PACKED_FS=1 -DMG_ENABLE_DRIVER_RT1020=1

CFLAGS += $(CFLAGS_EXTRA)

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

firmware.elf: cmsis_core cmsis_mcu $(SOURCES) hal.h link_ram.ld Makefile
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@
	arm-none-eabi-size $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:     # ARM CMSIS core headers
	git clone -q --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
cmsis_mcu:
	curl -sL https://mcuxpresso.nxp.com/cmsis_pack/repo/NXP.MIMXRT1062_DFP.17.1.0.pack -o $@.zip
	mkdir $@ && cd $@ && unzip -q ../$@.zip
mbedtls:        # mbedTLS library
	git clone --depth 1 -b v2.28.2 https://github.com/mbed-tls/mbedtls $@

ifeq ($(TLS), mbedtls)
CFLAGS += -DMG_TLS=MG_TLS_MBED -Wno-conversion -Imbedtls/include
CFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\" mbedtls/library/*.c
firmware.elf: mbedtls
endif

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/13
update: firmware.bin
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota --data-binary @$<
	curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"init"}'
	curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"wm,e000edf0,a05f0003 wm,e000edfc,1 wm,e000ed0c,5fa0004"}'
	curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"init"}'
	PC=`curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"rm,4"}' | jq -r .resp[5:]` && \
	SP=`curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"rm,0"}' | jq -r .resp[5:]` && \
	REQ="wm,e000ed08,0 wr,d,$$SP wr,f,$$PC" && \
	curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"'"$$REQ"'"}'
	curl -su :$(VCON_API_KEY) $(DEVICE_URL)/rpc/swd.exec -d '{"req":"wm,e000edf0,a05f0001"}'

test update: CFLAGS += -DUART_DEBUG=LPUART3
test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init

clean:
	$(RM) firmware.* *.su cmsis_core cmsis_mcu mbedtls *.zip
