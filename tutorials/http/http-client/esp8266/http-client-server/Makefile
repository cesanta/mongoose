THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
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

buildall:
buildall: CMD = make -C src all
buildall: build

flashall:
flashall: CMD = python /esp/components/esptool_py/esptool/esptool.py \
	--chip esp8266 --port $(PORT) --baud 115200 --before default_reset \
	--after hard_reset write_flash -z --flash_mode dio --flash_freq 40m \
	--flash_size 2MB 0x0 $(THISDIR)/src/build/bootloader/bootloader.bin \
	0x10000 $(THISDIR)/src/build/mongoose-example.bin \
	0x8000 $(THISDIR)/src/build/partitions_singleapp.bin
flashall: DA = --device $(PORT)
flashall: build

monitor:
monitor: CMD = /esp/tools/idf_monitor.py --port $(PORT) $(THISDIR)/src/build/mongoose-example.elf
monitor: build

.PHONY: build

clean:
	test -d src/build && $(DOCKER) rm -rf src/build || true

