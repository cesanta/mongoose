SDK_VERSION ?= 1.4.0
SDK_REPO ?= https://github.com/raspberrypi/pico-sdk

all example build build/firmware.uf2: pico-sdk main.c
	test -d build || mkdir build
	cd build && cmake .. && make

pico-sdk:
	git clone --depth 1 -b $(SDK_VERSION) $(SDK_REPO) $@
	cd $@ && git submodule update --init

# Requires env variable VCON_API_KEY set
DEVICE_URL ?= https://dash.vcon.io/api/v3/devices/3
test: update
	curl --fail -su :$(VCON_API_KEY) $(DEVICE_URL)/tx?t=5 | tee /tmp/output.txt
	grep 'Ethernet: up' /tmp/output.txt
	grep 'MQTT connected' /tmp/output.txt

update: build/firmware.uf2
	curl --fail -su :$(VCON_API_KEY) $(DEVICE_URL)/ota?uf2=1 --data-binary @$<

clean:
	rm -rf pico-sdk build
