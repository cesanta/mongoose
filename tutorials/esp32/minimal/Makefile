THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm $(DA) -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf
ARCH ?= esp32
PORT ?= /dev/ttyUSB0

all: example

example:
	true

build: Makefile $(wildcard main/*)
	test -d build || $(DOCKER) idf.py set-target $(ARCH)
	$(DOCKER) idf.py build

flash:
	cd build && esptool --chip $(ARCH) --baud 460800 --port $(PORT) write-flash @flash_args

clean:
	test -d build && (rm -rf build sdkconfig || true)

.PHONY: build
