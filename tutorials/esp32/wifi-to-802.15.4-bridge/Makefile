# Build: ESP-IDF from the espressif/idf Docker image. Flash: esptool
# Wi-Fi credentials: make WIFI_NAME=... WIFI_PASS=... build, or set them in
# the environment. Unset, the defaults in main/main.c apply

THISDIR = $(realpath $(CURDIR))
ROOTDIR = $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm -e WIFI_NAME -e WIFI_PASS -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf
PORT ?= /dev/cu.usbmodem101

all build:
	$(DOCKER) idf.py reconfigure build  # reconfigure picks up WIFI_* changes

flash: build
	cd build && esptool --chip esp32c6 -p $(PORT) -b 460800 write-flash @flash_args

menuconfig:
	docker run --rm -it -v $(ROOTDIR):$(ROOTDIR) -w $(THISDIR) espressif/idf idf.py menuconfig

clean:
	test -d build && rm -rf build sdkconfig || true

.PHONY: all build flash menuconfig clean
