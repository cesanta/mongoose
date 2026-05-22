# Environment setup: https://mongoose.ws/docs/getting-started/build-environment/

CWD ?= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SDK_PATH ?= $(CWD)/pico-sdk
RM = rm -rf
MKBUILD = test -d build || mkdir build

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
  MKBUILD = if not exist build mkdir build
endif

.PHONY: build

all build example: build/firmware.uf2

build/firmware.uf2: pico_sdk_import.cmake mongoose/mongoose.c mongoose/mongoose.h

pico_sdk_import.cmake: $(SDK_PATH)
	cp $(SDK_PATH)/external/$@ $@

mongoose/mongoose.c mongoose/mongoose.h:
	cp ../../../$(notdir $@) $@

build/firmware.uf2: $(SDK_PATH) $(wildcard *.[ch]) CMakeLists.txt Makefile
	$(MKBUILD)
	cd build && PICO_SDK_PATH=$(SDK_PATH) cmake -G "Unix Makefiles" .. && make

flash: build/firmware.uf2
	picotool load $< -f

clean:
	$(RM) build pico_sdk_import.cmake mongoose/mongoose.c mongoose/mongoose.h $(SDK_PATH)

$(SDK_PATH):
	git clone -c advice.detachedHead=false --quiet --depth 1 -b 2.1.1 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --quiet --init

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/3
# stdio has to be changed from USB to UART in CMakeLists.txt
update:
	perl -pi -e 's/pico_enable_stdio_usb(firmware 1)/pico_enable_stdio_usb(firmware 0)/' CMakeLists.txt
	$(MAKE) build
	perl -pi -e 's/pico_enable_stdio_usb(firmware 0)/pico_enable_stdio_usb(firmware 1)/' CMakeLists.txt
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota?uf2=1 --data-binary @build/firmware.uf2

test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=15 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
