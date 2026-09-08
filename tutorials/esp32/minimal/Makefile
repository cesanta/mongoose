THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm $(DA) -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf
CMD ?= idf.py build
PORT ?= /dev/ttyUSB0

all: example

example:
	true

build: Makefile $(wildcard main/*)
	$(DOCKER) $(CMD)

flash:
flash: CMD = idf.py flash
flash: DA = --device $(PORT)
flash: build

clean:
	test -d build && $(DOCKER) rm -rf build sdkconfig || true

.PHONY: build
