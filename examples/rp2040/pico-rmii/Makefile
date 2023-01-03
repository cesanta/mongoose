SDK_VERSION ?= 1.4.0
SDK_REPO ?= https://github.com/raspberrypi/pico-sdk

all example:
	true

build: pico-sdk
	test -d build || mkdir build
	cd build && cmake .. && make

pico-sdk:
	git clone --depth 1 -b $(SDK_VERSION) $(SDK_REPO) $@
	cd $@ && git submodule update --init

clean:
	rm -rf pico-sdk build
