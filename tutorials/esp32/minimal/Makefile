THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm $(DA) -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf
CMD ?= idf.py -DCFLAGS_EXTRA="$(CFLAGS_EXTRA)" build
PORT ?= /dev/ttyUSB0

CFLAGS_EXTRA ?= -DMG_TLS=MG_TLS_BUILTIN

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
