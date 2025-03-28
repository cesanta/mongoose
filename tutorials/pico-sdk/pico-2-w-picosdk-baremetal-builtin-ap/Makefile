CWD = $(realpath $(PWD))
SDK_PATH ?= $(CWD)/pico-sdk

all example:
	true

build: build/firmware.uf2

build/firmware.uf2: pico-sdk main.c
	mkdir -p build && cd build && PICO_SDK_PATH=$(SDK_PATH) cmake -DPICO_BOARD="pico2_w" -G "Unix Makefiles" .. && make

pico-sdk:
	git clone --depth 1 -b 2.1.0 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init

clean:
	rm -rf build pico-sdk
