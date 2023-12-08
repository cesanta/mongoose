RM = rm -rf
MKBUILD = test -d build || mkdir build
ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
  MKBUILD = if not exist build mkdir build
endif

all example:
	true

build: pico-sdk
	$(MKBUILD)
	cd build && cmake -G "Unix Makefiles" .. && make

pico-sdk:
	git clone --depth 1 -b 1.4.0 https://github.com/raspberrypi/pico-sdk $@
	cd $@ && git submodule update --init

clean:
	$(RM) pico-sdk build
