CWD = $(realpath $(CURDIR))
MNT = $(realpath $(CURDIR)/../../..)
PORT ?= /dev/ttyUSB0
CMD ?= build

all: example

example:
	true

build: Makefile $(wildcard main/*)
	docker run --rm $(DA) -v $(MNT):$(MNT) -w $(CWD) espressif/idf idf.py $(CMD)

flash:
flash: CMD = flash
flash: DA = --device $(PORT)
flash: build

.PHONY: build

bridge.hex: build
	esputil mkhex \
		0x8000 build/partition_table/partition-table.bin \
		0x1000 build/bootloader/bootloader.bin \
		0x100000 build/mongoose-esp32-example.bin > $@

flash2: bridge.hex
	esputil -p $(PORT) -b 921600 -fp 0x220 flash bridge.hex
	esputil -p $(PORT) monitor

clean:
	rm -rf build
