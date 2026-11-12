
THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
PORT ?= /dev/ttyUSB0
DOCKER ?= docker run --rm $(DA) -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf:v5.0.4
# Note that the esp32 port needs the extra .. for relative paths due to the location of its main CMakeLists.txt file
CMD ?= bash -c '$(MAKE) -C micropython/ports/esp32 submodules && $(MAKE) -C micropython/ports/esp32 USER_C_MODULES=../../../../mongoose/micropython.cmake'

all: example

example:
	true

build: micropython
	$(DOCKER) $(CMD)

micropython:
	$(DOCKER) git clone --depth 1 -b v1.22.2 https://github.com/micropython/micropython.git

clean:
	test -d micropython && $(DOCKER) rm -rf micropython || true

flash:
flash: DA = --device $(PORT)
flash: CMD = bash -c '$(MAKE) -C micropython/ports/esp32 erase deploy'
flash: build
