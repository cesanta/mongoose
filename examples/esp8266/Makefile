THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../..)
DOCKER ?= docker run --rm $(DA) -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) docker.io/mdashnet/8266
CMD ?= make -C src defconfig app
PORT ?= /dev/ttyUSB0

all: example

example:
	true

build: Makefile $(wildcard src/main/*)
	$(DOCKER) $(CMD)

flash:
flash: CMD = python /esp/components/esptool_py/esptool/esptool.py \
	--chip esp8266 --port $(PORT) --baud 115200 --before default_reset \
	--after hard_reset write_flash -z --flash_mode dio --flash_freq 40m \
	--flash_size 2MB 0x10000 $(THISDIR)/src/build/mongoose-example.bin
flash: DA = --device $(PORT)
flash: build

.PHONY: build

clean:
clean: CMD = rm -rf src/build	
clean: build

