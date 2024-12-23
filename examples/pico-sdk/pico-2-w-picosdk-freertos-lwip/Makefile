RM = rm -rf
MKBUILD = rm -rf build && mkdir -p build # test -d build || mkdir build
ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
  MKBUILD = if not exist build mkdir build
endif

all example:
	true

build: pico-sdk FreeRTOS-Kernel build/firmware.uf2

build/firmware.uf2:
	$(MKBUILD)
	cd build && cmake -DPICO_BOARD="pico2_w" -G "Unix Makefiles" .. && make

pico-sdk:
	git clone --depth 1 -b 2.1.0 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init
FreeRTOS-Kernel:  # FreeRTOS sources
	git clone --depth 1 -b main https://github.com/raspberrypi/FreeRTOS-Kernel $@

clean:
	$(RM) pico-sdk build FreeRTOS-Kernel
