RM = rm -rf
MKBUILD = test -d build || mkdir build
ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
  MKBUILD = if not exist build mkdir build
endif

all example:
	true

build build/firmware.uf2: pico-sdk
	$(MKBUILD)
	cd build && cmake -G "Unix Makefiles" .. && make

pico-sdk:
	git clone --depth 1 -b 1.5.0 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init

# Automated remote test. Requires env variable VCON_API_KEY set. See https://vcon.io/automated-firmware-tests/
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/3
update: build/firmware.uf2
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/ota?uf2=1 --data-binary @$<

test: update
	curl --fail-with-body -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'READY, IP:' /tmp/output.txt       # Check for network init
#	grep 'MQTT connected' /tmp/output.txt   # Check for MQTT connection success

clean:
	$(RM) pico-sdk build
