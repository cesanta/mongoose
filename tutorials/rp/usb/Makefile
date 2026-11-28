PICO_SDK_PATH ?= $(HOME)/src/pico-sdk
PICO_BOARD ?= pico2
BUILD ?= build/$(PICO_BOARD)
.PHONY: all build flash clean
all: build
build: ../../device-dashboard/full/file_data.c
	PICO_SDK_PATH=$(PICO_SDK_PATH) cmake -S . -B $(BUILD) -DPICO_BOARD=$(PICO_BOARD) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD) -j 4
flash: build
	picotool load -f $(BUILD)/firmware.uf2
	picotool reboot
../../device-dashboard/full/file_data.c:
	cd $(dir $@) && $(MAKE) $(notdir $@)
clean:
	rm -rf build
