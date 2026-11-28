PICO_SDK_PATH ?= $(HOME)/src/pico-sdk
PICO_BOARD ?= pico2_w
BUILD ?= build/$(PICO_BOARD)
.PHONY: all build flash clean

all: build

build: file_data.c
	PICO_SDK_PATH=$(PICO_SDK_PATH) cmake -S . -B $(BUILD) -DPICO_BOARD=$(PICO_BOARD) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD) -j 4

flash: build
	picotool load -f $(BUILD)/firmware.uf2
	picotool reboot

file_data.c: dashboard.html
	node ../../../resources/html2c.js dashboard.html -o $@

clean:
	rm -rf build file_data.c
