RM = rm -rf
MKBUILD = test -d build || mkdir build
ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
  MKBUILD = if not exist build mkdir build
endif

all example:
	true

build build/firmware.uf2: pico-sdk main.c net.c net.h
	$(MKBUILD)
	cd build && cmake -G "Unix Makefiles" .. && make

net.c: ../../mqtt/mqtt-dashboard/device/net.c net.h
	cp $< $@

net.h: ../../mqtt/mqtt-dashboard/device/net.h
	cp $< $@

pico-sdk:
	git clone --depth 1 -b 2.1.0 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init

clean:
	$(RM) pico-sdk build net.c net.h
