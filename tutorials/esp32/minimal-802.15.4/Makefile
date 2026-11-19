# Environment setup: https://mongoose.ws/documentation/tutorials/tools/
# Build: ESP-IDF from the espressif/idf Docker image. Flash: esptool.
# Monitor: esputil, https://github.com/cpq/esputil

THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf
PORT ?= /dev/cu.usbmodem2101

all build:
	$(DOCKER) idf.py build

flash: build
	cd build && esptool --chip esp32c6 -p $(PORT) -b 460800 write-flash @flash_args

monitor:
	esputil -p $(PORT) monitor

menuconfig:
	docker run --rm -it -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf idf.py menuconfig

clean:
	test -d build && $(DOCKER) rm -rf build sdkconfig || true
