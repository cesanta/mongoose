THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../..)

all: example

example: main/main.c Makefile
	docker run --rm -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf idf.py build

COMPORT ?= /dev/cu.SLAB_USBtoUART
ESPTOOL ?= esptool.py
flash:
	cd build && $(ESPTOOL) --chip esp32 -p $(COMPORT) -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size 2MB 0x8000 partition_table/partition-table.bin 0x1000 bootloader/bootloader.bin 0x10000 mongoose-esp32-example.bin

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb mongoose mongoose_* mongoose.* build sdkconfig
