THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../../..)
DOCKER ?= docker run --rm $(DA) -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf
CMD ?= idf.py build
PORT ?= /dev/ttyUSB0

all: example

example:
	true

build: Makefile $(wildcard main/*) main/net.c main/net.h main/packed_fs.c
	$(DOCKER) $(CMD)

main/net.c: ../../http/device-dashboard/net.c main/net.h
	cp $< $@

main/net.h: ../../http/device-dashboard/net.h
	cp $< $@

main/packed_fs.c: ../../http/device-dashboard/packed_fs.c
	cp $< $@

flash:
flash: CMD = idf.py flash
flash: DA = --device $(PORT)
flash: build

.PHONY: build

dashboard.hex: build
	esputil mkhex \
		0x8000 build/partition_table/partition-table.bin \
		0x1000 build/bootloader/bootloader.bin \
		0x100000 build/mongoose-esp32-example.bin > $@

flash2: dashboard.hex
	esputil -p $(PORT) -b 921600 -fp 0x220 flash dashboard.hex
	esputil -p $(PORT) monitor

ESPTOOL ?= esptool.py

flash3:
	cd build && $(ESPTOOL) --chip esp32 -p $(PORT) -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size 2MB 0x8000 partition_table/partition-table.bin 0x1000 bootloader/bootloader.bin 0x100000 mongoose-esp32-example.bin

clean:
	test -d build && $(DOCKER) rm -rf build sdkconfig || true
	rm -f main/net.c main/net.h main/packed_fs.c
